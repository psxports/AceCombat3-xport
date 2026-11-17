#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

GAME_RUNTIME game_runtime;

typedef struct
{
    uint32 ordering[4096];
    uint32 packets[28160];
} MISSION_GPU_STORAGE;

static MISSION_GPU_STORAGE mission_gpu_storage[2];

void mission_bind_packet_bank(uint32 index, uint32 ordering_words, uint32 packet_bytes)
{
    GAME_MISSION_PACKET_BANK *bank;
    MISSION_GPU_STORAGE *storage;

    if (index >= 2 || !ordering_words || ordering_words > 4096 || !packet_bytes || packet_bytes > 112640 || (packet_bytes & 3))
        abort();
    if (!gpu_register_packet_range(mission_gpu_storage, sizeof(mission_gpu_storage)))
        abort();
    bank = &game_runtime.mission_control.banks[index];
    storage = &mission_gpu_storage[index];
    bank->ot = gpu_packet_link((uintptr_t)storage->ordering);
    bank->ot_view.entries = storage->ordering;
    bank->ot_view.word_count = ordering_words;
    bank->packet_begin = gpu_packet_link((uintptr_t)storage->packets);
    bank->packet_data = (uint8 *)storage->packets;
    bank->packet_capacity = packet_bytes;
}

uint32 *mission_native_ot_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        const GAME_MISSION_PACKET_BANK *bank = &game_runtime.mission_control.banks[index];
        const GAME_MISSION_ORDERING_VIEW *view = &bank->ot_view;
        uint32 base = bank->ot;
        size_t capacity = (size_t)view->word_count * sizeof(uint32);

        if (view->entries && bytes && address >= base && !((address - base) & 3) && address - base <= capacity && bytes <= capacity - (address - base))
            return view->entries + (address - base) / sizeof(uint32);
    }
    return NULL;
}

void *mission_native_packet_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        const GAME_MISSION_PACKET_BANK *bank = &game_runtime.mission_control.banks[index];
        uint32 base = bank->packet_begin;

        if (bank->packet_data && bytes && address >= base && address - base <= bank->packet_capacity && bytes <= bank->packet_capacity - (address - base))
            return bank->packet_data + (address - base);
    }
    return NULL;
}

const struct SCENE_TERRAIN_MODEL_HEADER *terrain_model_header(const TERRAIN_MODEL_TABLE *models, uint32 id)
{
    if (!models->headers || id >= models->entry_count || !models->headers[id])
        abort();
    return models->headers[id];
}

const SCENE_MESH_MODEL_HEADER *const *scene_aircraft_composite_headers(const GAME_SCENE_ASSETS *assets)
{
    if (!assets->aircraft_headers)
        abort();
    return assets->aircraft_headers;
}

const SCENE_SPRITE_MODEL_HEADER *scene_sprite_model_header(RESOURCE_ASSET_SOURCE source, const SCENE_SPRITE_MODEL_HEADER *header, uint32 bytes)
{
    uintptr_t begin = (uintptr_t)source.data;
    uintptr_t address = (uintptr_t)header;
    size_t offset = address - begin;
    if (!source.data || !header || address < begin || bytes < sizeof(SCENE_SPRITE_MODEL_HEADER) || offset > source.bytes || bytes > source.bytes - offset || (address & 3))
        abort();
    return header;
}

const uint8 *scene_normal_data(SCENE_NORMAL_REFERENCE source, uint32 bytes)
{
    if (!source.view.data || source.offset > source.view.bytes || bytes > source.view.bytes - source.offset)
        abort();
    return source.view.data + source.offset;
}

uint32 *mission_ot_span(uint32 address, size_t bytes)
{
    uint32 *entries = mission_native_ot_span(address, bytes);

    if (!entries)
        entries = ui_native_ot_span(address, bytes);
    if (!entries)
        entries = scene_movie_native_ot_span(address, bytes);
    if (!entries && !(address & 3))
        entries = hud_native_packet_span(address, bytes);
    if (!entries)
        entries = mission_effect_native_ot_span(address, bytes);
    if (entries)
        return entries;
    return game_frame_packet_span(address, bytes);
}

void *mission_packet_span(uint32 address, size_t bytes)
{
    void *packets = mission_native_packet_span(address, bytes);

    if (!packets)
        packets = ui_native_packet_span(address, bytes);
    if (!packets)
        packets = scene_movie_native_packet_span(address, bytes);
    if (!packets)
        packets = hud_native_packet_span(address, bytes);
    if (packets)
        return packets;
    return game_frame_packet_span(address, bytes);
}

void game_runtime_reset(void)
{
    memset(&game_runtime, 0, sizeof(game_runtime));
    mission_script_reset_region();
    game_frame_packets_reset();
    scene_overlay_reset();
    title_renderer_reset_storage();
    memset(&title_text, 0, sizeof(title_text));
    memset(&title_menu, 0, sizeof(title_menu));
    memset(&title_card, 0, sizeof(title_card));
    memset(&mission_card, 0, sizeof(mission_card));
    memset(&mission_save_menu, 0, sizeof(mission_save_menu));
    memset(&mission_detail_list, 0, sizeof(mission_detail_list));
    memset(&mission_sound_test, 0, sizeof(mission_sound_test));
    memset(&menu_model_preview, 0, sizeof(menu_model_preview));
    memset(&mission_briefing, 0, sizeof(mission_briefing));
    memset(&mission_briefing_camera, 0, sizeof(mission_briefing_camera));
    memset(&mission_briefing_height_map, 0, sizeof(mission_briefing_height_map));
    mission_menu_exit_fade_ticks = 0;
    effect_trails_reset_storage();
    effect_drift.seed_phase = EFFECT_DRIFT_SEED_RESET;
    mission_actor_reset();
    mission_events_reset();
    mission_world_settings_reset();
    mission_links_reset();
    mission_groups_reset();
    menu_options_reset();
    menu_audio_reset();
    menu_geometry_reset();
    menu_model_reset();
    dialogue_sequence_reset();
    dialogue_ui_reset();
    dialogue_video_reset();
    text_renderer_reset();
    debug_text_reset_state();
    menu_bindings_reset();
    menu_state_reset();
    ui_widgets_reset();
    ui_map_reset();
    ui_animation_reset();
    ui_control_reset();
    ui_packets_reset();
    ui_items_reset();
    menu_orbits_reset();
    scene_palette_reset();
    mission_secondary_reset();
    mission_replay_reset_state();
    memset(&hud_control, 0, sizeof(hud_control));
    memset(&mission_target, 0, sizeof(mission_target));
    memset(&proximity_audio, 0, sizeof(proximity_audio));
    memset(&sound_common_attr, 0, sizeof(sound_common_attr));
    memset(&sound_schedule, 0, sizeof(sound_schedule));
    memset(&sound_reverb_attr, 0, sizeof(sound_reverb_attr));
    memset(&mission_prompt, 0, sizeof(mission_prompt));
    sound_reset_storage();
    memset(&sound_script, 0, sizeof(sound_script));
    memset(&speech_queue, 0, sizeof(speech_queue));
    memset(&engine_audio, 0, sizeof(engine_audio));
    memset(&scene_audio_routing, 0, sizeof(scene_audio_routing));
}

static uint32 arena_bookmark(const uint8 *pointer)
{
    if (!pointer)
        return 0;
    return 0x80000000u + (uint32)(pointer - DRAM);
}

uint32 arena_cursor(void)
{
    return arena_bookmark(game_runtime.arena.cursor);
}

void game_set_arena(uint8 *arena)
{
    FUNCTION_MARKER(0x800273FC, "ACE3.BIN");
    game_runtime.arena.cursor = arena;
    game_runtime.arena.begin = arena;
    return;
}

uint32 arena_alloc(uint32 bytes)
{
    uint32 begin;
    uint32 size;
    uint32 end;

    FUNCTION_MARKER(0x80027410, "ACE3.BIN");
    size = ((bytes + 3) >> 2) << 2;
    begin = arena_cursor();
    end = begin + size;
    if (game_runtime.arena.cursor && bytes <= UINT32_MAX - 3 && arena_cursor() >= arena_bookmark(game_runtime.arena.begin) && arena_cursor() <= 0x801FB700 && size <= 0x801FB700 - arena_cursor())
    {
        game_runtime.arena.cursor += size;
        return begin;
    }
#ifndef NDEBUG
    printf("Error: MemAlloc: Overflow %d bytes (REQ: %d bytes)\n", (sint32)(end + 0x7FE04900), (sint32)(size));
#endif
    return 0;
}

RESOURCE_STREAM_BUFFER arena_alloc_backed(uint32 bytes)
{
    RESOURCE_STREAM_BUFFER allocation;
    uint8 *begin = game_runtime.arena.cursor;
    size_t offset;

    allocation.encoded_begin = arena_alloc(bytes);
    allocation.buffer.data = allocation.encoded_begin ? begin : DRAM;
    offset = (size_t)(allocation.buffer.data - DRAM);
    if (offset >= sizeof(DRAM))
        abort();
    allocation.buffer.bytes = (uint32)(sizeof(DRAM) - offset);
    return allocation;
}

uint32 arena_peek(uint32 bytes)
{
    uint32 aligned;
    uint32 end;

    FUNCTION_MARKER(0x80027478, "ACE3.BIN");
    aligned = (bytes + 3) & 0xFFFFFFFC;
    end = arena_cursor() + aligned;
    if (bytes <= UINT32_MAX - 3 && arena_cursor() >= arena_bookmark(game_runtime.arena.begin) && arena_cursor() <= 0x801FB700 && aligned <= 0x801FB700 - arena_cursor())
        return arena_cursor();
#ifndef NDEBUG
    printf("Error: MemNoAlloc: Overflow %d bytes (REQ: %d bytes)\n", (sint32)(end + 0x7FE04900), (sint32)(aligned));
#endif
    return 0;
}

uint32 arena_check_release(uint32 bytes)
{
    uint32 released, limit, cursor, candidate;
    FUNCTION_MARKER(0x800274E0, "ACE3.BIN");
    released = (bytes >> 2) << 2;
    cursor = arena_cursor();
    limit = arena_bookmark(game_runtime.arena.begin);
    candidate = cursor + released;
    if (candidate >= limit)
        return arena_cursor();
#ifndef NDEBUG
    printf("Error: MemFree: Underflow %d bytes (REQ: %d bytes)\n", (sint32)(limit - candidate), (sint32)(released));
#endif
    return 0;
}

uint32 arena_set_cursor(uint32 cursor)
{
    uint32 begin;

    FUNCTION_MARKER(0x80027538, "ACE3.BIN");
    begin = arena_bookmark(game_runtime.arena.begin);
    if (game_runtime.arena.begin && cursor >= begin && cursor <= 0x801FB700)
    {
        game_runtime.arena.cursor = game_runtime.arena.begin + (cursor - begin);
        return cursor;
    }
#ifndef NDEBUG
    printf("Error: MemSet: Underflow %d bytes\n", (sint32)(begin - cursor));
#endif
    return 0;
}

uint32 arena_available(void)
{
    FUNCTION_MARKER(0x80027584, "ACE3.BIN");
    return 0x801FB700 - arena_cursor();
}

char *game_account_set_name(const char *name)
{
    size_t bytes;
    FUNCTION_MARKER(0x800D4158, "ACE3.BIN");
    if (!name)
        return 0;
    bytes = strlen(name) + 1;
    if (bytes > sizeof(game_runtime.account.name))
        abort();
    memcpy(game_runtime.account.name, name, bytes);
    return game_runtime.account.name;
}

void mission_bounds_initialize(uint32 index)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80032518, "ACE3.BIN");
    switch (index)
    {
        case 6:
            flags = game_runtime.mission_control.flags;
            game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[1] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[1] = (uint32)(0);
            game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_control.flags = (uint32)(flags | 24);
            game_runtime.camera_view.min[0] = (uint32)(0x80000001);
            game_runtime.camera_view.min[1] = (uint32)(0xFF6E0000);
            game_runtime.camera_view.min[2] = (uint32)(0x80000001);
            game_runtime.camera_view.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_view.max[1] = (uint32)(0xFFEF8000);
            game_runtime.camera_view.max[2] = (uint32)(0x7FFFFFFF);
            return;
        case 32:
        case 47:
        case 53:
            flags = game_runtime.mission_control.flags;
            game_runtime.mission_bounds.min[1] = (uint32)(0xFF48A000);
            game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.max[1] = (uint32)(0x01770000);
            game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_control.flags = (uint32)(flags | 16);
            game_runtime.camera_view.min[0] = (uint32)(0xDE96E000);
            game_runtime.camera_view.min[1] = (uint32)(0xFF5D4000);
            game_runtime.camera_view.min[2] = (uint32)(0xEA906000);
            game_runtime.camera_view.max[0] = (uint32)(0x056F0000);
            game_runtime.camera_view.max[1] = (uint32)(0xFFEF8000);
            game_runtime.camera_view.max[2] = (uint32)(0x11764000);
            return;
        case 55:
            flags = game_runtime.mission_control.flags;
            game_runtime.mission_bounds.min[1] = (uint32)(0xFF48A000);
            game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.max[1] = (uint32)(0x01770000);
            game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_control.flags = (uint32)(flags | 16);
            game_runtime.camera_view.min[0] = (uint32)(0x80000001);
            game_runtime.camera_view.min[1] = (uint32)(0xFF5D4000);
            game_runtime.camera_view.min[2] = (uint32)(0x80000001);
            game_runtime.camera_view.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_view.max[1] = (uint32)(0xFFEF8000);
            game_runtime.camera_view.max[2] = (uint32)(0x7FFFFFFF);
            return;
        case 42:
        case 58:
            game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[1] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[1] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
            flags = game_runtime.camera_control.flags;
            game_runtime.camera_view.min[0] = (uint32)(0x80000001);
            game_runtime.camera_view.min[1] = (uint32)(0x80000001);
            game_runtime.camera_view.min[2] = (uint32)(0x80000001);
            game_runtime.camera_view.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_view.max[1] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_view.max[2] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_control.flags = (uint32)(flags | 0x1000);
            result = game_runtime.mission_control.flags | 0x300;
            game_runtime.mission_control.flags = (uint32)(result);
            return;
        default:
            game_runtime.camera_view.min[0] = (uint32)(0x80000001);
            game_runtime.camera_view.min[1] = (uint32)(0x80000001);
            game_runtime.camera_view.min[2] = (uint32)(0x80000001);
            game_runtime.camera_view.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.camera_view.max[1] = (uint32)(0);
            game_runtime.camera_view.max[2] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[1] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[1] = (uint32)(0x7FFFFFFF);
            game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
            game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
            return;
    }
}

void resource_rewind_loaded(uint32 loaded)
{
    FUNCTION_MARKER(0x800ECD28, "ACE3.BIN");
    {
        (void)(uint32)(arena_set_cursor(loaded));
        return;
    }
}

void resource_rewind_current_asset(void)
{
    FUNCTION_MARKER(0x800ECD20, "ACE3.BIN");
    {
        (void)(resource_rewind_loaded(game_runtime.resource_slot.encoded_base));
        return;
    }
}

uint32 game_gpu_get_ode(void)
{
    FUNCTION_MARKER(0x800A8EF0, "ACE3.BIN");
    return (uint32)GetODE();
}

void game_stop_pad_communication(void)
{
    FUNCTION_MARKER(0x80027064, "ACE3.BIN");
    PadStopCom();
}

RESOURCE_ASSET_SOURCE terrain_model_vertices(const TERRAIN_MODEL_TABLE *models, const struct SCENE_TERRAIN_MODEL_HEADER *header)
{
    uint32 address = header->vertices;
    uint32 offset = address - models->guest_base;

    if (!models->data || address < models->guest_base || models->bytes < 4 || offset > models->bytes - 4)
        abort();
    return (RESOURCE_ASSET_SOURCE){models->data + offset, models->bytes - offset, address};
}

SCENE_FACE_GROUP terrain_model_faces(const TERRAIN_MODEL_TABLE *models, const struct SCENE_TERRAIN_MODEL_HEADER *header, uint32 flat)
{
    uint32 address = flat ? header->flat_quads : header->gouraud_quads;
    uint32 offset = address - models->guest_base;
    uint32 stride = flat ? 36 : 48;
    uint32 count;

    if (!models->data || address < models->guest_base || models->bytes < 4 || offset > models->bytes - 4 || (offset & 3))
        abort();
    memcpy(&count, models->data + offset, sizeof(count));
    if ((sint32)count > 0 && count > (models->bytes - offset - 4) / stride)
        abort();
    return (SCENE_FACE_GROUP){models->data + offset + 4, count, stride};
}
