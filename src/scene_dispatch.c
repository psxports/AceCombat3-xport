#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint32 pending;
    uint32 resource;
    uint32 active;
} SCENE_OVERLAY_LOAD;

static SCENE_OVERLAY_LOAD overlay_load;

static void game_overlay_load_begin(uint32 resource);
static void game_overlay_load_complete(void);

void scene_overlay_reset(void)
{
    memset(&overlay_load, 0, sizeof(overlay_load));
}

typedef uint32 (*GAME_SCENE_CALLBACK)(void);

typedef struct
{
    uint32 original_address;
    GAME_SCENE_CALLBACK callback;
} GAME_SCENE_PHASE;

typedef struct
{
    const GAME_SCENE_PHASE *phases;
    uint32 phase_count;
    uint32 overlay;
    uint8 *arena_begin;
} GAME_SCENE_DESCRIPTOR;

static const GAME_SCENE_PHASE scene_boot_phases[] = {
    {0x80026044, scene_return_frontend_command},
};

static const GAME_SCENE_PHASE scene_main_phases[] = {
    {0x800358F8, mission_load_begin}, {0x800357C0, mission_asset_load_poll}, {0x80035A38, mission_unpack_assets}, {0x80035F18, aircraft_load_begin}, {0x800357C0, mission_asset_load_poll}, {0x80035F98, aircraft_unpack_assets}, {0x80032754, mission_initialize_scene}, {0x80033EF0, mission_frame_flight}, {0x80034000, mission_frame_phase_8}, {0x80034040, mission_frame_replay}, {0x80034160, mission_frame_phase_10}, {0x8003423C, mission_frame_overlay_movie}, {0x80034274, mission_frame_movie}, {0x800342AC, mission_frame_phase_13}, {0x800342EC, mission_frame_confirm}, {0x800361BC, mission_demo_load_start}, {0x800357C0, mission_asset_load_poll}, {0x8003623C, mission_demo_load_finish},
};

static const GAME_SCENE_PHASE scene_resource002_phases[] = {
    {0x800F60F0, mission_frontend_initialize}, {0x800F6848, mission_debrief_frame}, {0x800FBC48, mission_frontend_begin_status_screen}, {0x800FBD74, mission_frontend_status_frame}, {0x800FBF70, mission_frontend_loading_frame}, {0x800FD634, mission_frontend_begin_debrief}, {0x800FD698, mission_frontend_debrief_frame}, {0x800CDCD4, mission_frontend_initialize_step}, {0x800CDF40, mission_frontend_frame},
};

static const GAME_SCENE_PHASE scene_resource001_phases[] = {
    {0x800CCE98, title_bootstrap_begin},
    {0x800CD07C, title_bootstrap_tick},
    {0x800CC5D8, title_menu_initialize},
    {0x800CC764, title_menu_frame},
};

static const GAME_SCENE_PHASE scene_movie_phases[] = {
    {0x80057CD0, movie_frontend_prepare},
    {0x80057E34, movie_caption_load_poll},
    {0x80057EBC, movie_frontend_initialize_display},
    {0x800581C4, movie_frontend_frame},
};

static const GAME_SCENE_PHASE scene_secondary_phases[] = {
    {0x8005B308, scene_movie_begin_menu},
    {0x8005B4E4, scene_movie_update_menu},
};

static const GAME_SCENE_PHASE scene_resource003_phases[] = {
    {0x8011BB7C, NULL},
    {0x8011E744, NULL},
};

static const GAME_SCENE_PHASE scene_resource004_phases[] = {
    {0x800CED74, NULL},
    {0x800CEEFC, NULL},
};

static const GAME_SCENE_DESCRIPTOR scene_descriptors[] = {
    {scene_boot_phases, sizeof(scene_boot_phases) / sizeof(scene_boot_phases[0]), 0xFFFFFFFF, DRAM + 0xCBFB8},
    {scene_boot_phases, sizeof(scene_boot_phases) / sizeof(scene_boot_phases[0]), 0xFFFFFFFF, DRAM + 0xCBFB8},
    {scene_main_phases, sizeof(scene_main_phases) / sizeof(scene_main_phases[0]), 0xFFFFFFFF, DRAM + 0xCBFB8},
    {scene_resource002_phases, sizeof(scene_resource002_phases) / sizeof(scene_resource002_phases[0]), 0x00000002, DRAM + 0x112E34},
    {scene_resource001_phases, sizeof(scene_resource001_phases) / sizeof(scene_resource001_phases[0]), 0x00000001, DRAM + 0xDDA80},
    {scene_movie_phases, sizeof(scene_movie_phases) / sizeof(scene_movie_phases[0]), 0xFFFFFFFF, DRAM + 0xCBFB8},
    {scene_secondary_phases, sizeof(scene_secondary_phases) / sizeof(scene_secondary_phases[0]), 0xFFFFFFFF, DRAM + 0xCBFB8},
    {scene_resource003_phases, sizeof(scene_resource003_phases) / sizeof(scene_resource003_phases[0]), 0x00000003, DRAM + 0x11EAC8},
    {scene_resource004_phases, sizeof(scene_resource004_phases) / sizeof(scene_resource004_phases[0]), 0x00000004, DRAM + 0xD1550},
};

static const GAME_SCENE_DESCRIPTOR *scene_descriptor(uint32 mode)
{
    if (mode >= sizeof(scene_descriptors) / sizeof(scene_descriptors[0]))
        abort();
    return scene_descriptors + mode;
}

void scene_dispatch_set_arena(void)
{
    game_set_arena(scene_descriptor(game_runtime.scene.mode)->arena_begin);
}

void scene_dispatch(void)
{
    uint32 state;
    const GAME_SCENE_PHASE *phase;
    uint32 command;
    uint32 value;
    const GAME_SCENE_DESCRIPTOR *descriptor;

    FUNCTION_MARKER(0x80026350, "ACE3.BIN");
    descriptor = scene_descriptor(game_runtime.scene.mode);
    state = descriptor->overlay;
    if (state == game_runtime.scene.loaded_overlay)
    {
        if (game_runtime.scene.phase >= descriptor->phase_count)
            abort();
        phase = descriptor->phases + game_runtime.scene.phase;
        if ((descriptor->overlay != 0xFFFFFFFF && !game_overlay_is_active(descriptor->overlay)) || !phase->callback)
        {
            GAME_SCENE_CALLBACK callback = NULL;
            if (game_overlay_is_active(1))
            {
                if (state == 1)
                    callback = phase->callback;
            }
            else if (game_overlay_is_active(2))
            {
                if (state == 2)
                    callback = phase->callback;
            }
            if (!callback)
            {
#ifndef NDEBUG
                fprintf(stderr, "Unbound guest callback %08X/%u resource=%u active=%u\n", phase->original_address, 3u, overlay_load.resource, overlay_load.active);
#endif
                abort();
            }
            command = callback();
        }
        else
            command = phase->callback();
    }
    else
    {
        switch (game_runtime.scene.phase)
        {
            case 0:
                command = scene_overlay_begin_load(state);
                break;
            case 1:
                command = scene_overlay_poll_load();
                break;
            default:
                abort();
        }
        if (command & 0x02000000)
        {
            game_runtime.scene.phase = 0;
            game_runtime.scene.loaded_overlay = state;
        }
    }
    if (!command)
        return;
    if (command & 0x20000000)
    {
        game_runtime.scene.mode = command & 255;
        game_runtime.scene.phase = 0;
    }
    else if (command & 0x08000000)
    {
        value = game_runtime.scene.mode;
        game_runtime.scene.phase = 0;
        game_runtime.scene.mode = value + 1;
    }
    else if (command & 0x10000000)
    {
        game_runtime.scene.phase = command & 255;
    }
    else if (command & 0x04000000)
    {
        game_runtime.scene.phase = game_runtime.scene.phase + 1;
    }
    if ((command & 0x28000000) && (sint32)scene_descriptor(game_runtime.scene.mode)->overlay < 0)
        game_runtime.scene.loaded_overlay = 0xFFFFFFFF;
    if (command & 0x40000000)
    {
        scene_dispatch_set_arena();
        game_runtime.flags = game_runtime.flags | 8;
    }
    return;
}

void scene_overlay_activate(void)
{
    FUNCTION_MARKER(0x8009B6AC, "ACE3.BIN");
    game_overlay_load_complete();
}

uint32 scene_overlay_begin_load(uint32 id)
{
    uint32 flags;
    uint32 mode;
    uint32 next;

    FUNCTION_MARKER(0x8009B6B8, "ACE3.BIN");
    flags = game_runtime.display.flags;
    mode = game_runtime.display.mode;
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.flags = (uint32)(flags | 5);
    if (!mode)
    {
        if (game_runtime.display_buffer)
            next = gpu_copy_rect(gpu_packet_link((uintptr_t)game_runtime.frame_packets.last_tag), game_frame_packet_cursor(), 0, 256, 0, 0, 320, 240);
        else
            next = gpu_copy_rect(gpu_packet_link((uintptr_t)game_runtime.frame_packets.last_tag), game_frame_packet_cursor(), 0, 0, 0, 256, 320, 240);
        game_frame_set_packet_cursor(next);
    }
    game_overlay_load_begin(id);
    return 0x04000000;
}

uint32 scene_overlay_poll_load(void)
{
    FUNCTION_MARKER(0x8009B7CC, "ACE3.BIN");
    if (overlay_load.pending)
        scene_overlay_activate();
    if (!game_runtime.display.mode)
    {
        PSX_RECT clear = {0, 240, 320, 16};
        VSync(0);
        ClearImage(&clear, 0, 0, 0);
        DrawSync(0);
    }
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    game_runtime.display.flags = (uint32)(game_runtime.display.flags | 8);
    return 0x02000000;
}

uint32 scene_return_frontend_command(void)
{
    FUNCTION_MARKER(0x80026044, "ACE3.BIN");
    return 0x60000004;
}

sint32 game_overlay_is_active(uint32 resource)
{
    return (resource == 1 || resource == 2) && overlay_load.active == resource;
}

static void game_overlay_load_begin(uint32 resource)
{
    overlay_load.active = 0;
    overlay_load.pending = 1;
    overlay_load.resource = resource;
}

static void game_overlay_load_complete(void)
{
    if (!overlay_load.pending)
        return;
    overlay_load.pending = 0;
    if (overlay_load.resource == 1)
    {
        memset(&title_card, 0, sizeof(title_card));
        title_renderer_reset_storage();
        memset(&title_text, 0, sizeof(title_text));
        memset(&title_menu, 0, sizeof(title_menu));
        overlay_load.active = 1;
    }
    else if (overlay_load.resource == 2)
    {
        dialogue_ui.arena_begin = 0;
        dialogue_ui.caption.packets = NULL;
        dialogue_video.decoder.packet_link = 0;
        dialogue_video.decoder.packets = NULL;
        text_renderer_reset();
        mission_flow.menu_arrow_delay = 0;
        mission_flow.scene_archive.arena_begin = 0;
        mission_flow.archive_browser.packet_link = 0;
        mission_flow.archive_browser.packets = NULL;
        overlay_movie_reset_bookmark();
        game_runtime.menu_confirmation_gate = 0;
        memset(&mission_card, 0, sizeof(mission_card));
        memset(&mission_save_menu, 0, sizeof(mission_save_menu));
        memset(&mission_detail_list, 0, sizeof(mission_detail_list));
        memset(&mission_sound_test, 0, sizeof(mission_sound_test));
        memset(&menu_model_preview, 0, sizeof(menu_model_preview));
        memset(&mission_briefing, 0, sizeof(mission_briefing));
        memset(&mission_briefing_camera, 0, sizeof(mission_briefing_camera));
        memset(&mission_briefing_height_map, 0, sizeof(mission_briefing_height_map));
        mission_menu_exit_fade_ticks = 0;
        overlay_load.active = 2;
    }
}
