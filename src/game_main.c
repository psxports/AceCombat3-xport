#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

static uint32 game_frame_contains(const PSX_RECT *draw, const PSX_RECT *display)
{
    return draw->w > 0 && draw->h > 0 && display->w > 0 && display->h > 0 && draw->x <= display->x && draw->y <= display->y && (sint32)draw->x + draw->w >= (sint32)display->x + display->w && (sint32)draw->y + draw->h >= (sint32)display->y + display->h;
}

uint32 game_frame_display_ready(PSX_RECT *previous_draw, uint32 *previous_complete)
{
    const GAME_DISPLAY_BANK *bank = &game_runtime.surfaces.bank[game_runtime.display_buffer];
    uint32 movie = game_runtime.flags & 4;
    uint32 complete = !movie;
    uint32 ready;
    PSX_RECT draw, display;
    draw = bank->draw->clip;
    display = bank->display->disp;
    /* Present only a completed draw page or movie frame */
    ready = movie || (complete && game_frame_contains(&draw, &display)) || (*previous_complete && game_frame_contains(previous_draw, &display));
    if (movie)
        *previous_complete = 0;
    else if (complete)
    {
        *previous_draw = draw;
        *previous_complete = 1;
    }
    return ready;
}

#if AP_WEB && defined(_DEBUG)
    #include <emscripten.h>

EMSCRIPTEN_KEEPALIVE const uint32 *game_web_debug_snapshot(void)
{
    static uint32 snapshot[34];
    DISPENV display = {0};
    uint32 index, nonzero = 0;
    uint32 game = game_runtime.initialized;
    uint32 scene = game_runtime.scene_initialized;
    RESOURCE_QUEUE_STATUS queue;
    snapshot[0] = game;
    snapshot[1] = game ? game_runtime.flags : 0;
    snapshot[2] = game ? game_runtime.frame_counter : 0;
    snapshot[3] = game ? game_runtime.field_counter : 0;
    snapshot[4] = scene;
    snapshot[5] = scene ? game_runtime.mission_control.flags : 0;
    snapshot[6] = scene ? game_runtime.mission_control.mode : 0;
    queue = resource_queue_status();
    snapshot[7] = queue.consumed;
    snapshot[8] = queue.queued;
    snapshot[9] = queue.priority_active;
    snapshot[10] = queue.current_id;
    snapshot[11] = game_resource_music_active();
    snapshot[12] = game ? game_input.flags : 0;
    snapshot[13] = game ? game_input.kind : 0;
    snapshot[14] = game ? game_input.held : 0;
    snapshot[15] = game ? game_input.pressed : 0;
    snapshot[17] = movie_control_debug_phase();
    snapshot[23] = g_xport_audio_submitted_buffers;
    snapshot[24] = g_xport_audio_nonzero_buffers;
    snapshot[25] = g_xport_audio_peak;
    gpu_get_display_env(&display);
    for (index = 0; index < GPU_VRAM_WIDTH * GPU_VRAM_HEIGHT; ++index)
        nonzero += VRAM[index] != 0;
    snapshot[27] = nonzero;
    snapshot[28] = (uint16)display.disp.x | ((uint32)(uint16)display.disp.y << 16);
    snapshot[29] = (uint16)display.disp.w | ((uint32)(uint16)display.disp.h << 16);
    snapshot[30] = display.isrgb24;
    snapshot[31] = 0;
    snapshot[32] = 0;
    snapshot[33] = gpu_get_raster();
    return snapshot;
}
#endif

#if !defined(XPORT_NATIVE_GPU_CD)
/* use the shared rasterizer's standard GPU type */
const uint32 xport_gpu_graph_type_address = 0;
#endif

static void game_initialize_boot_memory(void)
{
    const uint32 bss_end = 0x800CBFE8;
    const uint32 ram_base = 0x80000000;
    const uint32 stack_top = 0x00200000;
    const uint32 stack_reserve = 0x8000;
    InitHeap(bss_end + 4, stack_top - stack_reserve - (bss_end - ram_base));
}

void xport_main(void)
{
    /* SYSTEM.CNF numbers use the reviewed BIOS hexadecimal parser */
    if (!SetConf(0x16, 4, 0x801FFF00))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unable to initialize reviewed BIOS kernel configuration\n");
#endif
        return;
    }
    game_initialize_boot_memory();
    game_runtime_reset();
    game_input_reset();
    memset(&audio_settings, 0, sizeof(audio_settings));
    mission_flow_reset();
    random_seed = 0;
    memset(&resource_metadata, 0, sizeof(resource_metadata));
    memset(&resource_controller, 0, sizeof(resource_controller));
    resource_queue_reset();
    resource_stream_reset();
    resource_ring_reset_storage();
    scene_script_reset_frames();
    scene_script_reset_restore();
    scene_texture_reset();
    movie_decoder_reset();
    game_bind_sdk();
    if (!game_mount_data(1))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unable to open extracted DATA/1 files\n");
#endif
        return;
    }
    game_run();
}

void game_run(void)
{
    uint32 flags;
    uint32 args;
    uint32 result;
    uint32 frame_ready;
    uint32 previous_draw_complete = 0;
    PSX_RECT previous_draw = {0};

    FUNCTION_MARKER(0x800261A8, "ACE3.BIN");
    /* The reviewed __main replacement is supplied by the native CRT */
    game_init();
    VSync(0);
    SetDispMask(1);
    for (;;)
    {
        gpu_frame_begin();
        game_apply_display_changes();
        game_begin_frame_packets();
        pad_poll();
        pad_update_input(&pad_state[0], &game_input, pad_profiles);
        sound_update();
        scene_dispatch();
        resource_poll();
        args = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        result = game_frame_packet_cursor();
        result = debug_text_draw(args, result);
        game_frame_set_packet_cursor(result);
        result = GetRCnt(0xF2000001);
        game_runtime.frame_timing.submit_counter = (uint32)(result);
        DrawSync(0);
        result = GetRCnt(0xF2000001);
        game_runtime.frame_timing.complete_counter = (uint32)(result);
        VSync((sint32)game_runtime.display.field_wait);
        result = VSync(-1);
        game_runtime.field_counter = result;
        ResetRCnt(0xF2000001);
        game_submit_frame();
        flags = game_runtime.flags;
        if (flags & 8)
        {
            game_runtime.flags = flags & 0xFFFFFFF7;
            DrawSync(0);
        }
        frame_ready = game_frame_display_ready(&previous_draw, &previous_draw_complete);
        gpu_frame_end();
        if (frame_ready)
            gpu_present();
        flags = game_runtime.flags;
        if (flags & 0x10000000)
        {
            game_runtime.flags = flags & 0xD3FFFFFF;
            continue;
        }
        if (flags & 0x04000000)
            game_runtime.flags = (flags & 0xFBFFFFFF) | 0x20000000;
        flags = game_runtime.flags;
        if (flags & 0x08000000)
            game_runtime.flags = flags & 0xD7FFFFFF;
    }
}

void game_init(void)
{
    uint32 flags;

    FUNCTION_MARKER(0x80025F68, "ACE3.BIN");
    game_init_env();
    pad_init_profiles(pad_profiles);
    flags = game_input.flags;
    game_input.flags = (uint32)(flags | 1);
    game_init_devices();
    game_initialize_frame_timing();
    sound_initialize_mix();
    resource_init();
    game_load_boot_assets();
    sound_initialize();
    subtitles_initialize();
    sound_set_cd_mix(1);
}

void game_init_env(void)
{
    sint32 y;

    FUNCTION_MARKER(0x800257E8, "ACE3.BIN");
    game_runtime.initialized = 1;
    SetDefDrawEnv(&game_runtime.surfaces.draw[0], 0, 0, 320, 240);
    SetDefDispEnv(&game_runtime.surfaces.display[0], 0, 256, 320, 240);
    SetDefDrawEnv(&game_runtime.surfaces.draw[1], 0, 256, 320, 240);
    SetDefDispEnv(&game_runtime.surfaces.display[1], 0, 0, 320, 240);
    SetDefDrawEnv(&game_runtime.surfaces.draw[2], 0, 0, 320, 480);
    SetDefDispEnv(&game_runtime.surfaces.display[2], 0, 0, 320, 480);
    game_runtime.surfaces.display[2].isinter = (uint8)(1);
    game_runtime.surfaces.draw[0].r0 = (uint8)(4);
    game_runtime.surfaces.draw[0].g0 = (uint8)(56);
    game_runtime.surfaces.draw[0].b0 = (uint8)(72);
    game_runtime.surfaces.draw[1].r0 = (uint8)(4);
    game_runtime.surfaces.draw[1].g0 = (uint8)(56);
    game_runtime.surfaces.draw[1].b0 = (uint8)(72);
    game_runtime.surfaces.draw[2].r0 = (uint8)(4);
    game_runtime.surfaces.draw[2].g0 = (uint8)(56);
    game_runtime.surfaces.draw[2].b0 = (uint8)(72);
    game_runtime.surfaces.draw[0].isbg = (uint8)(0);
    game_runtime.surfaces.draw[1].isbg = (uint8)(0);
    game_runtime.surfaces.draw[2].isbg = (uint8)(0);
    game_runtime.surfaces.display[0].screen.x = (sint16)((uint16)(0xFFFE));
    game_runtime.surfaces.display[0].screen.y = (sint16)((uint16)(0));
    game_runtime.display.flags = (uint32)(0);
    game_runtime.display.mode = (uint32)(0);
    game_runtime.display.screen[0] = (uint32)(game_runtime.surfaces.display[0].screen.x);
    y = game_runtime.surfaces.display[0].screen.y;
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.screen[1] = (uint32)(y);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    game_runtime.flags = 0;
    game_runtime.resource_pending = 0;
    game_runtime.display_buffer = 0;
    game_runtime.field_counter = 0;
    game_runtime.scene.loaded_overlay = 0xFFFFFFFF;
    audio_settings.volume[0] = (uint8)(0xD0);
    game_runtime.frame_counter = 0;
    game_runtime.gameplay_counter = 0;
    game_runtime.scene.mode = 0;
    game_runtime.scene.phase = 0;
    game_runtime.selection.mission = (uint16)(1);
    game_runtime.selection.aircraft = (uint16)(1);
    game_runtime.selection.saved_mission = (uint16)(1);
    game_runtime.selection.saved_aircraft = (uint16)(1);
    game_runtime.selection.difficulty = (uint32)(1);
    audio_settings.volume[1] = (uint8)(0xD0);
    audio_settings.flags = (uint8)(0);
    game_runtime.options.captions = (uint8)(1);
    game_runtime.options.kill_camera = (uint8)(0);
    game_runtime.options.hud_hidden = (uint8)(0);
    game_runtime.options.camera_mode = (uint8)(0);
    game_runtime.playback.intro_count = (uint8)(0);
    game_runtime.playback.return_mode = (uint32)(0);
    game_runtime.playback.demo_mode = (uint8)(0);
    game_runtime.playback.demo_idx = (uint8)(0);
    game_runtime.options.overlay_toggles = 0;
    game_runtime.playback.caption_gate = (uint8)(1);
    game_runtime.playback.movie_idx = (uint8)(0);
    game_runtime.selection.weapon_code[0] = (uint8)(1);
    game_runtime.selection.weapon_code[1] = (uint8)(1);
    game_runtime.selection.weapon_code[2] = (uint8)(1);
    game_runtime.progress.sequence_idx = (uint8)(0);
    game_runtime.playback.resume_count = 0;
    game_runtime.playback.resume_mode = 0;
    game_runtime.account.route_mode = (uint16)(0);
    game_input.repeat_tick = (uint8)(0);
    game_runtime.progress.flow_flags = (uint32)(0);
    game_runtime.playback.script_id = 0;
    game_input.flags = (uint32)(0);
    game_input.delay_tick[1] = (uint8)(0);
    game_input.delay_tick[0] = (uint8)(0);
    game_runtime.asset_counts.unobserved[0] = (uint32)(0);
    game_runtime.asset_counts.unobserved[9] = (uint32)(0);
    game_runtime.frame_timing.complete_counter = (uint32)(0);
    game_runtime.frame_timing.submit_counter = (uint32)(0);
    game_runtime.asset_counts.unobserved[8] = (uint32)(0);
    game_runtime.asset_counts.unobserved[7] = (uint32)(0);
    game_runtime.asset_counts.unobserved[6] = (uint32)(0);
    game_runtime.asset_counts.unobserved[5] = (uint32)(0);
    game_runtime.asset_counts.unobserved[4] = (uint32)(0);
    game_runtime.asset_counts.unobserved[3] = (uint32)(0);
    game_runtime.asset_counts.unobserved[2] = (uint32)(0);
    game_runtime.asset_counts.unobserved[1] = (uint32)(0);
    game_runtime.asset_counts.unobserved[11] = (uint32)(0);
    game_runtime.asset_counts.unobserved[10] = (uint32)(0);
    game_runtime.resource_context.pending_mode = (uint32)(1);
    game_runtime.resource_context.pending_argument = (uint32)(0);
    scene_dispatch_set_arena();
}

void game_init_devices(void)
{
    FUNCTION_MARKER(0x80025B00, "ACE3.BIN");
    /* The original stack copies at 0x80025B10 are unused local storage */
    ResetCallback();
    while (!CdInit())
    {
    }
    CdSetDebug(0);
    VSync(3);
    ResetGraph(0);
    SetDispMask(1);
    InitGeom();
    SpuInit();
#if 0
    InitCARD(0);
    StartCARD();
    _bu_init();
#endif
    pad_init_ports();
    bios_exit_critical();
}

sint32 game_initialize_frame_timing(void)
{
    FUNCTION_MARKER(0x80025DDC, "ACE3.BIN");
    geom_set_offset(0, 0);
    geom_set_screen(0x115);
    game_runtime.display.flags = (uint32)(11);
    game_apply_display_changes();
    SetRCnt(0xF2000001, 0x1E00, 0x2000);
    return StartRCnt(0xF2000001);
}

void game_apply_display_changes(void)
{
    uint32 flags;
    uint32 mode;
    uint32 color_r;
    uint32 color_g;
    uint32 color_b;
    uint32 x;

    FUNCTION_MARKER(0x80025BC8, "ACE3.BIN");
    flags = game_runtime.display.flags;
    if (!flags)
        return;
    if (flags & 0x40)
    {
        mode = game_runtime.display.mode;
        game_runtime.display.flags = (uint32)(flags & 0xFFFFFFBF);
        if (!mode)
        {
            game_runtime.surfaces.bank[0].display = &game_runtime.surfaces.display[0];
            game_runtime.surfaces.bank[1].display = &game_runtime.surfaces.display[1];
        }
        else if (mode == 1)
        {
            game_runtime.surfaces.bank[0].display = &game_runtime.surfaces.display[2];
            game_runtime.surfaces.bank[1].display = &game_runtime.surfaces.display[2];
        }
        flags = game_runtime.display.flags;
    }
    if (flags & 1)
    {
        mode = game_runtime.display.mode;
        game_runtime.display.flags = (uint32)((flags & 0xFFFFFFFE) | 0x40);
        if (!mode)
        {
            game_runtime.surfaces.bank[0].draw = &game_runtime.surfaces.draw[0];
            game_runtime.surfaces.bank[1].draw = &game_runtime.surfaces.draw[1];
        }
        else if (mode == 1)
        {
            game_runtime.surfaces.bank[0].draw = &game_runtime.surfaces.draw[2];
            game_runtime.surfaces.bank[1].draw = &game_runtime.surfaces.draw[2];
        }
    }
    flags = game_runtime.display.flags;
    if (flags & 0x10)
    {
        game_runtime.display.flags = (uint32)(flags & 0xFFFFFFEF);
        game_runtime.surfaces.bank[0].display = &game_runtime.surfaces.movie_display;
        game_runtime.surfaces.bank[1].display = &game_runtime.surfaces.movie_display;
        game_runtime.display.flags = (uint32)(game_runtime.display.flags | 1);
        flags = game_runtime.display.flags;
    }
    if (flags & 0x20)
    {
        game_runtime.display.flags = (uint32)(flags & 0xFFFFFFDF);
        game_runtime.surfaces.bank[0].draw = &game_runtime.surfaces.movie_draw;
        game_runtime.surfaces.bank[1].draw = &game_runtime.surfaces.movie_draw;
        game_runtime.display.flags = (uint32)(game_runtime.display.flags | 1);
    }
    flags = game_runtime.display.flags;
    if (flags & 2)
    {
        x = (uint16)game_runtime.display.screen[0];
        game_runtime.display.flags = (uint32)(flags & 0xFFFFFFFD);
        game_runtime.surfaces.display[0].screen.x = (sint16)((uint16)(x));
        game_runtime.surfaces.display[0].screen.y = (sint16)((uint16)((uint16)game_runtime.display.screen[1]));
        game_runtime.surfaces.display[1].screen.x = (sint16)((uint16)((uint16)game_runtime.display.screen[0]));
        game_runtime.surfaces.display[1].screen.y = (sint16)((uint16)((uint16)game_runtime.display.screen[1]));
        game_runtime.surfaces.display[2].screen.x = (sint16)((uint16)((uint16)game_runtime.display.screen[0]));
        game_runtime.surfaces.display[2].screen.y = (sint16)((uint16)((uint16)game_runtime.display.screen[1]));
    }
    flags = game_runtime.display.flags;
    if (flags & 4)
    {
        game_runtime.display.flags = (uint32)(flags & 0xFFFFFFFB);
        game_runtime.surfaces.draw[2].isbg = (uint8)(0);
        game_runtime.surfaces.draw[1].isbg = (uint8)(0);
        game_runtime.surfaces.draw[0].isbg = (uint8)(0);
        flags = game_runtime.display.flags;
    }
    if (!(flags & 8))
        return;
    color_r = game_runtime.display.background[0];
    color_g = game_runtime.display.background[1];
    game_runtime.display.flags = (uint32)(flags & 0xFFFFFFF7);
    color_b = game_runtime.display.background[2];
    game_runtime.surfaces.draw[2].isbg = (uint8)(1);
    game_runtime.surfaces.draw[1].isbg = (uint8)(1);
    game_runtime.surfaces.draw[0].isbg = (uint8)(1);
    game_runtime.surfaces.draw[0].r0 = (uint8)(color_r);
    game_runtime.surfaces.draw[0].g0 = (uint8)(color_g);
    game_runtime.surfaces.draw[0].b0 = (uint8)(color_b);
    game_runtime.surfaces.draw[1].r0 = (uint8)(color_r);
    game_runtime.surfaces.draw[1].g0 = (uint8)(color_g);
    game_runtime.surfaces.draw[1].b0 = (uint8)(color_b);
    game_runtime.surfaces.draw[2].r0 = (uint8)(color_r);
    game_runtime.surfaces.draw[2].g0 = (uint8)(color_g);
    game_runtime.surfaces.draw[2].b0 = (uint8)(color_b);
    return;
}

static void bootstrap_tim_group(uint32 address, const uint8 *group, uint32 size)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 count, previous, offset, index;

    if (size < 4)
        abort();
    count = (memcpy(&wire_word_0, group, sizeof(wire_word_0)), wire_word_0);
    if (count > (size - 4) / 4)
        abort();
    previous = 4 + count * 4;
    for (index = 0; index < count; ++index)
    {
        offset = (memcpy(&wire_word_1, group + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
        if (offset < previous || offset > size - 8)
            abort();
        previous = offset + 8;
    }
    scene_texture_upload_tim_group(address, count, group, size);
}

void game_load_boot_assets(void)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 bytes, resource, index, offset[6];
    uint8 *bundle;

    FUNCTION_MARKER(0x80025EA0, "ACE3.BIN");
    bytes = resource_get_size(0);
    resource = arena_peek(bytes);
    bundle = malloc(bytes);
    if (!bundle)
        abort();
    resource_queue_read(0, resource, 0, (RESOURCE_READ_BUFFER){bundle, bytes});
    resource_wait(0);
    if (!resource || bytes < 24)
        abort();
    if ((memcpy(&wire_word_0, bundle, sizeof(wire_word_0)), wire_word_0) != 5)
        abort();
    offset[5] = bytes;
    for (index = 0; index < 5; ++index)
        offset[index] = (memcpy(&wire_word_1, bundle + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
    for (index = 0; index < 5; ++index)
        if (offset[index] < 24 || offset[index] >= offset[index + 1])
            abort();
    sound_load_bank((SOUND_BANK_INPUT){bundle, bytes, resource + offset[4] - resource});
    bootstrap_tim_group(resource + offset[0], bundle + offset[0], offset[1] - offset[0]);
    scene_texture_upload_tim(resource + offset[1], bundle + offset[1], bytes - offset[1]);
    scene_texture_upload_tim(resource + offset[2], bundle + offset[2], bytes - offset[2]);
    bootstrap_tim_group(resource + offset[3], bundle + offset[3], offset[4] - offset[3]);
    DrawSync(0);
    free(bundle);
    {
        (void)(debug_text_initialize());
        return;
    }
}

static GAME_FRAME_PACKET_PAGE frame_packet_pages[2];
static uint8 frame_packet_links_ready;

void game_frame_packets_reset(void)
{
    memset(frame_packet_pages, 0, sizeof(frame_packet_pages));
    frame_packet_links_ready = 0;
}

void *game_frame_packet_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; frame_packet_links_ready && index < 2; ++index)
    {
        GAME_FRAME_PACKET_PAGE *page = &frame_packet_pages[index];
        uint32 base = gpu_packet_link((uintptr_t)page);

        if (page && bytes && address >= base && address - base <= sizeof(*page) && bytes <= sizeof(*page) - (address - base))
            return (uint8 *)page + (address - base);
    }
#ifndef NDEBUG
    fprintf(stderr, "Unbound native frame packet %08X (%zu bytes)\n", address, bytes);
#endif
    abort();
}

uint32 game_frame_packet_cursor(void)
{
    uint32 base = gpu_packet_link((uintptr_t)game_runtime.frame_packets.commands);
    return base + game_runtime.frame_packets.command_bytes;
}

void game_frame_set_packet_cursor(uint32 encoded)
{
    uint32 base = gpu_packet_link((uintptr_t)game_runtime.frame_packets.commands);
    game_runtime.frame_packets.command_bytes = encoded - base;
}

void game_begin_frame_packets(void)
{
    uint32 index;
    uint32 buffer;
    uint32 ot;

    FUNCTION_MARKER(0x80026050, "ACE3.BIN");
    if (!gpu_register_packet_range(frame_packet_pages, sizeof(frame_packet_pages)))
        abort();
    frame_packet_links_ready = 1;
    index = game_runtime.display_buffer ^ 1;
    if (index >= 2)
        abort();
    game_runtime.display_buffer = index;
    buffer = gpu_packet_link((uintptr_t)&frame_packet_pages[index]);
    ot = buffer + 8;
    game_runtime.frame_packets.middle_tag = &frame_packet_pages[index].ordering[1];
    game_runtime.frame_packets.first_tag = &frame_packet_pages[index].ordering[0];
    game_runtime.frame_packets.last_tag = &frame_packet_pages[index].ordering[2];
    game_runtime.frame_packets.commands = frame_packet_pages[index].commands;
    game_runtime.frame_packets.command_bytes = 0;
    ClearOTagR(frame_packet_pages[index].ordering, 3);
    {
        (void)(uint32)(ot);
        return;
    }
}

void game_submit_frame(void)
{
    uint32 index;
    uint32 flags;
    uint32 frame;

    FUNCTION_MARKER(0x800260C0, "ACE3.BIN");
    index = game_runtime.display_buffer;
    if (index >= 2)
        abort();
    PutDispEnv(game_runtime.surfaces.bank[index].display);
    PutDrawEnv(game_runtime.surfaces.bank[index].draw);
    if (!(game_runtime.flags & 4))
        DrawOTag(&frame_packet_pages[index].ordering[2]);
    flags = game_runtime.flags;
    if (!(flags & 0x20000000))
    {
        if (!(flags & 0x02000000))
            game_runtime.gameplay_counter = game_runtime.gameplay_counter + 1;
        else
            game_runtime.flags = flags & 0xFDFFFFFF;
    }
    frame = game_runtime.frame_counter + 1;
    game_runtime.frame_counter = frame;
    {
        (void)(uint32)(frame);
        return;
    }
}
