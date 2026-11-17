#include "game_functions.h"
#include <string.h>

static void mission_tick_flight(void);
static void mission_tick_phase_8(void);
static void mission_tick_replay(void);
static void mission_tick_phase_10(void);
static void mission_tick_phase_13(void);
static void mission_tick_idle(void);
static void mission_render_frame(void (*callback)(void));

void mission_rebuild_render_arena(uint32 cursor)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags;
    FUNCTION_MARKER(0x800323F8, "ACE3.BIN");
    arena_set_cursor(cursor);
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(0x4000);
        (void)arena_alloc(0x1B800);
        mission_bind_packet_bank(index, 4096, 112640);
    }
    actor_index_table = 0;
    for (index = 0; index < 64; ++index)
    {
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags;
        if ((flags & 0x4000) != 0)
            actor_traits_object->flags = (uint32)(flags | 0x2000);
    }
    mission_replay_initialize();
}

void mission_allocate_packet_banks(uint32 cursor)
{
    uint32 index;
    FUNCTION_MARKER(0x800324AC, "ACE3.BIN");
    arena_set_cursor(cursor);
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(0x4000);
        (void)arena_alloc(0x1B800);
        mission_bind_packet_bank(index, 4096, 112640);
    }
    return;
}

static void mission_tick_flight(void)
{
    FLIGHT_STATUS *state;
    MISSION_TARGET_FRAME record;
    uint32 result, value;
    FUNCTION_MARKER(0x80032F3C, "ACE3.BIN");
    result = ~game_runtime.flags & 0x20000000;
    if (result == 0)
        return;
    flight_dynamics_update();
    mission_actor_tick_damage();
    mission_actor_tick_primary();
    mission_secondary_tick();
    mission_actor_update_links_and_trace();
    effect_system_update();
    effect_trails_update();
    effect_tick_frame();
    state = &flight_status;
    record.ammunition = state->ammunition;
    record.angle_limit = state->target_angle_limit;
    record.distance_limit[0] = state->target_distance_limit[0];
    record.distance_limit[1] = state->target_distance_limit[1];
    mission_target_prepare_frame(&record);
    state = &flight_status;
    value = state->target_index;
    if (value != record.locked_target_index && (game_runtime.mission_control.flags & 2) != 0)
        mission_replay_record_target(record.locked_target_index);
    state = &flight_status;
    game_runtime.camera_control.look_target = (uint32)(record.camera_target_index);
    state->target_index = (uint32)(record.locked_target_index);
    result = game_runtime.mission_control.flags & 2;
    if (result != 0)
    {
        mission_replay_record_frame();
        return;
    }
    return;
}

static void mission_tick_phase_8(void)
{
    uint32 result;
    FUNCTION_MARKER(0x8003307C, "ACE3.BIN");
    scene_script_playback_update();
    result = ~game_runtime.flags & 0x20000000;
    if (result == 0)
        return;
    particle_update_simple();
    {
        effect_trails_update();
        return;
    }
}

static void mission_tick_replay(void)
{
    FUNCTION_MARKER(0x800330CC, "ACE3.BIN");
    mission_replay_step();
    particle_update_simple();
    effect_trails_update();
    {
        effect_tick_frame();
        return;
    }
}

static void mission_tick_phase_10(void)
{
    FLIGHT_STATUS *state;
    uint32 result;
    /* Defines the three input words the original caller leaves uninitialized */
    MISSION_TARGET_FRAME record = {0};
    FUNCTION_MARKER(0x80033104, "ACE3.BIN");
    mission_objectives_update();
    result = ~game_runtime.flags & 0x20000000;
    if (result == 0)
        return;
    result = ~game_runtime.mission_control.mode & 0x1000;
    if (result == 0)
        return;
    flight_dynamics_update();
    mission_actor_tick_damage();
    mission_actor_tick_primary();
    mission_secondary_tick();
    mission_actor_update_links_and_trace();
    effect_system_update();
    effect_trails_update();
    effect_tick_frame();
    state = &flight_status;
    record.ammunition = state->ammunition;
    mission_target_prepare_frame(&record);
    state = &flight_status;
    game_runtime.camera_control.look_target = (uint32)(record.camera_target_index);
    result = record.locked_target_index;
    state->target_index = (uint32)(result);
    return;
}

static void mission_tick_phase_13(void)
{
    FLIGHT_STATUS *state;
    uint32 result;
    /* Defines the three input words the original caller leaves uninitialized */
    MISSION_TARGET_FRAME record = {0};
    FUNCTION_MARKER(0x800331E0, "ACE3.BIN");
    result = ~game_runtime.flags & 0x20000000;
    if (result == 0)
        return;
    flight_dynamics_update();
    mission_actor_tick_damage();
    mission_actor_tick_primary();
    mission_secondary_tick();
    mission_actor_update_links_and_trace();
    effect_system_update();
    effect_trails_update();
    effect_tick_frame();
    state = &flight_status;
    record.ammunition = state->ammunition;
    mission_target_prepare_frame(&record);
    state = &flight_status;
    game_runtime.camera_control.look_target = (uint32)(record.camera_target_index);
    result = record.locked_target_index;
    state->target_index = (uint32)(result);
    return;
}

static void mission_tick_idle(void)
{
    FUNCTION_MARKER(0x8003329C, "ACE3.BIN");
}

typedef char mission_frame_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(DR_MODE) == 12 && offsetof(DR_MODE, code) == 4 ? 1 : -1];

static void mission_render_frame(void (*callback)(void))
{
    uint32 *frame_middle_tag;
    uint32 selected, head, packet, table, tag, value, other, width, height, x, y;
    uint32 offset_x, offset_y, center_x, center_y;
    const DRAWENV *environment;
    uint32 *head_entries, *ordering_entries, *table_entry;
    FUNCTION_MARKER(0x800339E0, "ACE3.BIN");
    selected = game_runtime.display_buffer;
    head = game_runtime.mission_control.banks[selected].ot;
    head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
    game_runtime.mission_control.ot = (uint32)(head);
    packet = game_runtime.mission_control.banks[selected].packet_begin;
    game_runtime.mission_control.packet_cur = (uint32)(packet);
    ClearOTagR(mission_ot_span(head, 16384), 4096);
    head = game_runtime.mission_control.ot;
    head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
    table = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    table_entry = frame_middle_tag;
    tag = head_entries[0];
    value = *table_entry;
    head_entries[0] = (tag & 0xFF000000) | (value & 0xFFFFFF);
    value = game_runtime.mission_control.ot;
    table = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    table_entry = frame_middle_tag;
    tag = *table_entry;
    *table_entry = (tag & 0xFF000000) | ((value + 0x3FFC) & 0xFFFFFF);
    game_runtime.mission_view.offset[0] = (uint16)(0);
    game_runtime.mission_view.offset[1] = (uint16)(0);
    game_runtime.mission_view.shift[0] = (uint16)(0);
    game_runtime.mission_view.shift[1] = (uint16)(0);
    callback();
    camera_update();
    matrix_build_transpose(game_runtime.camera_view.pose.angles, &game_runtime.mission_matrices.camera, &game_runtime.mission_matrices.view);
    selected = game_runtime.display_buffer;
    environment = game_runtime.surfaces.bank[selected].draw;
    packet = game_runtime.mission_control.packet_cur;
    ordering_entries = mission_ot_span(game_runtime.mission_control.ot, 4096 * sizeof(*ordering_entries));
    x = environment->ofs[0];
    y = environment->ofs[1];
    width = (uint16)environment->clip.w;
    height = (uint16)environment->clip.h;
    offset_x = (uint16)game_runtime.mission_view.offset[0];
    offset_y = (uint16)game_runtime.mission_view.offset[1];
    game_runtime.mission_view.origin.x = (sint16)(uint16)(offset_x + x);
    center_x = x + (uint32)((sint32)(width << 16) >> 17);
    game_runtime.mission_view.origin.y = (sint16)(uint16)(offset_y + y);
    value = (uint16)game_runtime.mission_view.shift[0];
    center_x += value;
    other = (uint16)game_runtime.mission_view.shift[1];
    center_y = y + (uint32)((sint32)(height << 16) >> 17) + other;
    game_runtime.mission_view.center.y = (sint16)(uint16)(center_y);
    value = (sint16)game_runtime.mission_view.shift[0];
    width = (sint16)width;
    game_runtime.mission_view.center.x = (sint16)(uint16)(center_x);
    if (value != 0)
    {
        TILE *bar = mission_packet_span(packet, sizeof(*bar));
        bar->tag = (bar->tag & 0xFFFFFF) | 0x03000000;
        bar->rgbc.code = (uint8)(96);
        bar->rgbc.r = (uint8)(0);
        bar->rgbc.g = (uint8)(0);
        bar->rgbc.b = (uint8)(0);
        bar->p[0].x = (sint16)(uint16)(0);
        bar->p[0].y = (sint16)(uint16)(0);
        value = (uint16)game_runtime.mission_view.shift[0];
        bar->h = (sint16)(uint16)(240);
        bar->w = (sint16)(uint16)(value);
        head = game_runtime.mission_control.ot;
        head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
        tag = bar->tag;
        value = head_entries[0];
        bar->tag = (tag & 0xFF000000) | (value & 0xFFFFFF);
        head = game_runtime.mission_control.ot;
        head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
        tag = head_entries[0];
        head_entries[0] = (tag & 0xFF000000) | (packet & 0xFFFFFF);
        packet += 16;
    }
    if ((sint16)game_runtime.mission_view.shift[1] != 0)
    {
        TILE *bar = mission_packet_span(packet, sizeof(*bar));
        bar->tag = (bar->tag & 0xFFFFFF) | 0x03000000;
        bar->rgbc.code = (uint8)(96);
        bar->rgbc.r = (uint8)(0);
        bar->rgbc.g = (uint8)(0);
        bar->rgbc.b = (uint8)(0);
        bar->p[0].x = (sint16)(uint16)(0);
        bar->p[0].y = (sint16)(uint16)(0);
        bar->w = (sint16)(uint16)(width);
        bar->h = (sint16)(uint16)((uint16)game_runtime.mission_view.shift[1]);
        head = game_runtime.mission_control.ot;
        head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
        tag = bar->tag;
        value = head_entries[0];
        bar->tag = (tag & 0xFF000000) | (value & 0xFFFFFF);
        head = game_runtime.mission_control.ot;
        head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
        tag = head_entries[0];
        head_entries[0] = (tag & 0xFF000000) | (packet & 0xFFFFFF);
        packet += 16;
    }
    value = (y << 11) | x | 0xE5000000;
    game_runtime.mission_control.packet_cur = (uint32)(packet);
    DR_MODE *offsets = mission_packet_span(packet, 2 * sizeof(*offsets));
    offsets[0].code[0] = value;
    offsets[0].code[1] = 0;
    value = ordering_entries[0];
    offsets[0].tag = value | 0x2000000;
    head = game_runtime.mission_control.ot;
    head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
    head_entries[0] = packet & 0xFFFFFF;
    x = (sint16)game_runtime.mission_view.origin.x;
    y = (sint16)game_runtime.mission_view.origin.y;
    other = packet + 12;
    game_runtime.mission_control.packet_cur = (uint32)(other);
    offsets[1].code[1] = 0;
    offsets[1].code[0] = (y << 11) | x | 0xE5000000;
    value = ordering_entries[1];
    offsets[1].tag = value | 0x2000000;
    head = game_runtime.mission_control.ot;
    head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
    head_entries[1] = other & 0xFFFFFF;
    game_runtime.mission_control.packet_cur = (uint32)(packet + 24);
    xport_gte_write_control(24, 0);
    xport_gte_write_control(25, 0);
    effect_draw_mission_trails();
    particle_draw_frame();
    scene_geometry_draw_mission_models();
    scene_geometry_render_mission_terrain();
    packet = game_runtime.mission_control.packet_cur;
    ordering_entries = mission_ot_span(game_runtime.mission_control.ot, 4096 * sizeof(*ordering_entries));
    y = (sint16)game_runtime.mission_view.center.y;
    x = (sint16)game_runtime.mission_view.center.x;
    DR_MODE *center_mode = mission_packet_span(packet, sizeof(*center_mode));
    center_mode->code[0] = (y << 11) | x | 0xE5000000;
    center_mode->code[1] = 0;
    value = ordering_entries[4095];
    center_mode->tag = value | 0x2000000;
    head = game_runtime.mission_control.ot;
    head_entries = mission_ot_span(head, 4096 * sizeof(*head_entries));
    head_entries[4095] = packet & 0xFFFFFF;
    game_runtime.mission_control.packet_cur = (uint32)(packet + 12);
    xport_gte_write_control(24, 160 << 16);
    xport_gte_write_control(25, 120 << 16);
    hud_update_panels_and_fade();
    mission_status_text_frame();
    subtitles_submit_frame();
    value = ~game_runtime.mission_control.flags & 1;
    if (value != 0)
    {
        hud_draw();
        return;
    }
    return;
}

uint32 mission_frame_flight(void)
{
    uint32 flags, context, packet, result, value, requested;
    FUNCTION_MARKER(0x80033EF0, "ACE3.BIN");
    game_runtime.mission_control.flags = (uint32)(game_runtime.mission_control.flags | 2);
    mission_render_frame(mission_tick_flight);
    flags = game_runtime.flags;
    if ((flags & 0x20000000) != 0)
    {
        mission_menu_update_game_options();
        context = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        packet = game_runtime.mission_control.packet_cur;
        result = mission_menu_draw_options(context, packet);
        game_runtime.mission_control.packet_cur = (uint32)(result);
        if ((game_runtime.flags & 0x8000000) != 0)
            resource_pause_stream(0);
    }
    else if ((flags & 0x10000000) == 0)
    {
        value = game_input.status;
        requested = 0;
        if ((value & 1) != 0)
        {
            game_input.status = (uint16)(value & 0xFFFE);
            requested = 1;
        }
        if ((game_input.pressed & 64) != 0)
            requested = 1;
        if (requested != 0)
        {
            game_runtime.flags = game_runtime.flags | 0x4000000;
            resource_pause_stream(1);
        }
    }
    return mission_finish_frame();
}

uint32 mission_frame_phase_8(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x80034000, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_render_frame(mission_tick_phase_8);
    return mission_finish_frame();
}

uint32 mission_frame_replay(void)
{
    uint32 flags, mode, ordering, packet, result;
    FUNCTION_MARKER(0x80034040, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_render_frame(mission_tick_replay);
    mode = game_runtime.playback.demo_mode;
    if (mode != 0)
    {
        ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        packet = game_runtime.mission_control.packet_cur;
        result = hud_draw_brand_footer(ordering, packet, 4096);
        game_runtime.mission_control.packet_cur = (uint32)(result);
        flags = game_runtime.mission_control.flags;
        if ((flags & 0x10000000) == 0)
            return mission_finish_frame();
        mission_audio_stop_groups();
        resource_init_queue(0, 0);
        mode = game_runtime.playback.demo_mode;
        if (mode != 1)
        {
            flags = game_input.held;
            if ((flags & 16) == 0)
                return 0x50000000;
        }
        game_runtime.playback.demo_mode = (uint8)(0);
        result = game_runtime.playback.return_mode;
        return result | 0x60000000;
    }
    flags = game_runtime.options.hud_hidden;
    if (flags == 0)
    {
        ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        packet = game_runtime.mission_control.packet_cur;
        result = mission_menu_draw_status_banner(ordering, packet);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    return mission_finish_frame();
}

uint32 mission_frame_phase_10(void)
{
    uint32 flags, object, packet, result;
    FUNCTION_MARKER(0x80034160, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_render_frame(mission_tick_phase_10);
    flags = game_runtime.mission_control.mode;
    if ((flags & 0x1000) != 0)
    {
        mission_retry_wait_start();
        object = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        packet = game_runtime.mission_control.packet_cur;
        result = mission_retry_draw_prompt(object, packet);
        game_runtime.mission_control.packet_cur = (uint32)(result);
        flags = game_runtime.flags;
        game_runtime.flags = flags | 0x02000000;
        flags = game_runtime.mission_control.flags;
        if ((flags & 32) != 0)
        {
            game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFDF);
            sound_stop_all();
            pad_reset_slots();
            object = game_runtime.mission_control.argument;
            mission_request_mode(1024, object);
            flight_restore_player_health();
        }
    }
    return mission_finish_frame();
}

uint32 mission_frame_phase_13(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x800342AC, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_render_frame(mission_tick_phase_13);
    return mission_finish_frame();
}

uint32 mission_frame_confirm(void)
{
    uint32 flags, object, packet, result;
    FUNCTION_MARKER(0x800342EC, "ACE3.BIN");
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x02000000;
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_confirm_update();
    mission_render_frame(mission_tick_idle);
    object = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
    packet = game_runtime.mission_control.packet_cur;
    result = mission_menu_draw_confirm(object, packet);
    game_runtime.mission_control.packet_cur = (uint32)(result);
    return mission_finish_frame();
}

void mission_transition_update(void)
{
    MISSION_ACTOR *pose_owner_object;

    uint32 flags, stage, ticks, value, other, packet, ordering, reason, remaining, high, quotient;
    sint32 mission;
    /* Defines the padding copied into the transition object */
    ACTOR_POSE pose = {0};
    FUNCTION_MARKER(0x800371C4, "ACE3.BIN");
    pose_owner_object = mission_actor(0);
    mission_events_update();
    flags = game_runtime.mission_control.mode;
    if ((flags & 2) != 0)
    {
        value = game_runtime.flags;
        game_runtime.flags = value | 0x10000000;
        if (mission_flow.request.dispatch_count == 1 && game_runtime.mission_control.argument == 0)
        {
            ticks = game_runtime.mission_control.ticks;
            if ((sint32)ticks < 16)
            {
                ordering = game_runtime.mission_control.ot;
                packet = game_runtime.mission_control.packet_cur;
                value = effect_draw_noise_overlay(ordering + 4, packet, 4096 - (ticks << 8));
                game_runtime.mission_control.packet_cur = (uint32)(value);
            }
        }
        if (game_runtime.mission_control.ticks == 0)
        {
            mission = (sint16)game_runtime.selection.mission;
            if (mission == 42)
                flight_dynamics_enter_alternate_control();
            else if (game_runtime.mission_control.argument == 0 && (mission == 6 || mission == 14 || mission == 44))
                flight_dynamics_select_idle_control(0);
            else
                flight_dynamics_enter_transition_control();
            camera_apply_option();
        }
        if ((flight_dynamics_flags() & 0x7000) != 0)
        {
            mission_flow.request.mode = (uint8)(9);
            mission_flow.request.flags = (uint8)(5);
            mission_request_mode(8, 1);
            value = game_runtime.flags;
            game_runtime.flags = value | 0x10000000;
            speech_queue_stop();
            resource_cancel_streams();
            return;
        }
        if ((sint32)game_runtime.mission_control.ticks < 61 || speech_queue_mode() != 0)
            return;
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 42)
            flight_dynamics_enter_direct_control();
        else
            flight_dynamics_enter_standard_control();
        camera_apply_player_option(game_runtime.options.camera_mode);
        mission_request_mode(4, game_runtime.mission_control.argument);
        value = game_runtime.flags;
        game_runtime.flags = value & 0xEFFFFFFF;
        stage = game_runtime.mission_control.argument;
        mission_status_text_show(mission_phase_message(stage));
        return;
    }
    if ((flags & 4) != 0)
    {
        if ((flight_dynamics_flags() & 0x7000) != 0)
        {
            mission_flow.request.flags = (uint8)(5);
            mission_flow.request.mode = (uint8)(9);
            mission_request_mode(8, 1);
            value = game_runtime.flags;
            game_runtime.flags = value | 0x10000000;
            goto abort_streams;
        }
        stage = game_runtime.mission_control.argument;
        value = game_runtime.metrics.phase_ticks;
        other = mission_phase_timeout(stage);
        if (value == other)
        {
            mission_flow.request.flags = (uint8)(5);
            mission_flow.request.mode = (uint8)(9);
            mission_request_mode(8, 4);
            value = game_runtime.flags;
            game_runtime.flags = value | 0x10000000;
            goto abort_streams;
        }
        value = pose_owner_object->pose.position[0];
        other = game_runtime.world_settings.minimum_x;
        if ((sint32)value < (sint32)other)
            goto outside_area;
        other = game_runtime.world_settings.maximum_x;
        if ((sint32)other < (sint32)value)
            goto outside_area;
        value = pose_owner_object->pose.position[2];
        other = game_runtime.world_settings.minimum_z;
        if ((sint32)value < (sint32)other)
            goto outside_area;
        other = game_runtime.world_settings.maximum_z;
        if ((sint32)other < (sint32)value)
            goto outside_area;
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 58 && (sint32)pose_owner_object->pose.position[1] > (sint32)0xFFD00000)
            goto outside_area;
        if (mission == 42 && pose_owner_object->pose.position[1] + 0x0EA60000 > 0x124F8000)
            goto outside_area;
        if (mission_flow.request.mode != 9)
            goto countdown;
        mission_request_mode(8, mission_flow.request.restore_arg);
        value = game_runtime.flags;
        mission = (sint16)game_runtime.selection.mission;
        game_runtime.flags = value | 0x10000000;
        if (mission == 6)
            speech_queue_stop();
        goto cancel_streams;
    outside_area:
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 32 || mission == 47 || mission == 53)
            goto countdown;
        mission_flow.request.flags = (uint8)(5);
        mission_flow.request.mode = (uint8)(9);
        mission_request_mode(8, 8);
        value = game_runtime.flags;
        game_runtime.flags = value | 0x10000000;
    abort_streams:
        speech_queue_stop();
    cancel_streams:
        resource_cancel_streams();
    countdown:
    {
    }
        stage = game_runtime.mission_control.argument;
        value = mission_phase_timeout(stage);
        other = game_runtime.metrics.phase_ticks;
        remaining = value - other;
        if (remaining == 1800)
        {
            mission_status_text_show(10);
            game_runtime.metrics.timer_mode = (uint8)(1);
            game_runtime.metrics.display_ticks = (uint32)(remaining);
        }
        if ((sint32)remaining >= 1800)
        {
            if (game_runtime.metrics.timer_mode == 1)
            {
                value = game_runtime.metrics.display_ticks;
                if ((sint32)value < 1800)
                    remaining = value;
            }
            if ((sint32)remaining >= 1800)
                return;
        }
        high = (((sint64)(sint32)remaining * (sint32)0x88888889) >> 32);
        quotient = (uint32)((sint32)(high + remaining) >> 4) - (uint32)((sint32)remaining >> 31);
        value = ((quotient << 4) - quotient) << 1;
        if (remaining == value)
            sound_play((sint32)remaining < 301 ? 57 : 56);
        return;
    }
    if ((flags & 8) != 0)
    {
        value = game_runtime.flags;
        game_runtime.flags = value | 0x10000000;
        stage = game_runtime.mission_control.argument;
        if ((stage & 0x62) == 0 && stage != 0)
            goto staged_transition;
        if ((flight_dynamics_flags() & 0x7000) != 0)
        {
            mission_flow.request.mode = (uint8)(9);
            mission_request_mode(8, 1);
            value = game_runtime.flags;
            game_runtime.flags = value | 0x10000000;
            game_runtime.metrics.fade.active = (uint8)(0);
            speech_queue_stop();
            mission_flow.request.flags = (uint8)(5);
            return;
        }
        if (game_runtime.mission_control.argument == 0)
            goto unstaged_transition;
    staged_transition:
    {
    }
        stage = game_runtime.mission_control.argument;
        reason = 0;
        if ((stage & 2) == 0)
        {
            reason = 1;
            if ((stage & 0x20) == 0)
            {
                reason = 2;
                if ((stage & 0x40) != 0)
                    reason = 50;
            }
        }
        if (game_runtime.mission_control.ticks == 0)
        {
            mission = (sint16)game_runtime.selection.mission;
            if (mission == 58 && (game_runtime.mission_control.argument & 8) != 0)
                flight_dynamics_select_idle_control(0);
            else
            {
                if ((game_runtime.mission_control.argument & 1) == 0)
                {
                    mission = (sint16)game_runtime.selection.mission;
                    if (mission == 42)
                        flight_dynamics_enter_alternate_control();
                    else
                        flight_dynamics_enter_transition_control();
                }
            }
            mission_flow.transition.fade_started = (uint8)(0);
        }
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 6)
        {
            mission_flow.transition.fade_tick = (uint32)(60);
            mission_flow.transition.notice_tick = (uint32)(0);
            value = 90;
        }
        else
        {
            mission_flow.transition.notice_tick = (uint32)(60);
            mission_flow.transition.fade_tick = (uint32)(180);
            value = 210;
        }
        mission_flow.transition.finish_tick = (uint32)(value);
        ticks = game_runtime.mission_control.ticks;
        value = mission_flow.transition.notice_tick;
        if (ticks == value)
            mission_status_text_show(reason);
        value = mission_flow.transition.fade_tick;
        ticks = game_runtime.mission_control.ticks;
        if ((sint32)ticks >= (sint32)value && mission_flow.transition.fade_started == 0 && speech_queue_mode() == 0)
        {
            game_runtime.metrics.fade.mode = (uint8)(2);
            game_runtime.metrics.fade.active = (uint8)(1);
            mission_flow.transition.fade_started = (uint8)(1);
            game_runtime.metrics.fade.duration = (uint16)(30);
            game_runtime.metrics.fade.ticks = (uint16)(0);
        }
        value = mission_flow.transition.finish_tick;
        ticks = game_runtime.mission_control.ticks;
        if ((sint32)ticks < (sint32)value || mission_flow.transition.fade_started != 1 || speech_queue_mode() != 0)
            return;
        if (game_runtime.metrics.fade.active != 0)
            return;
        if (mission_flow.request.mode != 0)
            mission_audio_stop_groups();
        else
            mission_audio_stop_scene_groups();
        speech_queue_stop();
        hud_notice_reset_pair();
        mission_dispatch_scene_request();
        return;
    unstaged_transition:
        if (game_runtime.mission_control.ticks == 0)
        {
            value = mission_flow.request.mode;
            other = 0xFFFFFFFF;
            reason = 90;
            if (value != 0)
            {
                mission = (sint16)game_runtime.selection.mission;
                flight_dynamics_enter_transition_control();
                if (mission == 6)
                {
                    if ((mission_flow.request.flags & 2) != 0)
                    {
                        other = 0;
                        reason = 30;
                    }
                    else
                        other = 60;
                }
                else if (mission == 23)
                {
                    other = 120;
                    reason = 150;
                }
                else
                    other = 60;
            }
            mission_flow.transition.fade_tick = (uint32)(other);
            mission_flow.transition.finish_tick = (uint32)(reason);
        }
        ticks = game_runtime.mission_control.ticks;
        value = mission_flow.transition.fade_tick;
        if (ticks == value)
        {
            game_runtime.metrics.fade.mode = (uint8)(2);
            game_runtime.metrics.fade.active = (uint8)(1);
            game_runtime.metrics.fade.duration = (uint16)(30);
            game_runtime.metrics.fade.ticks = (uint16)(0);
        }
        value = mission_flow.transition.finish_tick;
        ticks = game_runtime.mission_control.ticks;
        if ((sint32)ticks < (sint32)value || speech_queue_mode() != 0)
            return;
        if (game_runtime.metrics.fade.active != 0)
            return;
        if (mission_flow.request.mode != 0)
        {
            mission_audio_stop_groups();
            effect_pools_reset();
            mission_actor_finish_deactivation();
        }
        else
            mission_audio_stop_scene_groups();
        hud_notice_reset_pair();
        speech_queue_stop();
        mission_dispatch_scene_request();
        return;
    }
    if ((flags & 0x10) != 0)
    {
        if (game_runtime.mission_control.ticks == 0)
        {
            if ((sint16)game_runtime.selection.mission == 46 && game_runtime.mission_control.argument == 2)
                mission_entity_events_trigger(0);
        }
        return;
    }
    if ((flags & 0x21FFE0) != 0 || (flags & 0x100000) == 0)
        return;
    if (game_runtime.mission_control.ticks == 0)
    {
        mission_flow.transition.stream_phase = (uint8)(0);
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 6)
        {
            pose.pose.position[0] = 0xC7C00000;
            pose.pose.position[2] = 0xD7C00000;
            pose.pose.position[1] = 0xFF448000;
            pose.pose.angles[0] = 0;
            pose.pose.angles[1] = 0xE000;
            pose.pose.angles[2] = 0;
            mission_player_set_pose(&pose, NULL);
        }
        else if (mission == 9)
        {
            pose.pose.position[0] = 0;
            pose.pose.position[1] = 0xFE2B4000;
            pose.pose.position[2] = 0;
            pose.pose.angles[0] = 0;
            pose.pose.angles[1] = 0;
            pose.pose.angles[2] = 0;
            mission_player_set_pose(&pose, NULL);
        }
        flight_dynamics_select_idle_control(0);
        camera_apply_player_option(game_runtime.options.camera_mode);
        mission_audio_initialize_groups();
    }
    if (game_runtime.mission_control.ticks == 30)
    {
        speech_queue_push(mission_flow.request.effect_id, 0);
    }
    if (mission_flow.transition.stream_phase == 0)
    {
        if ((sint32)game_runtime.mission_control.ticks >= 151 && speech_queue_mode() == 0)
            mission_flow.transition.stream_phase = (uint8)(1);
    }
    if (mission_flow.transition.stream_phase == 1)
    {
        game_runtime.metrics.fade.mode = (uint8)(2);
        game_runtime.metrics.fade.active = (uint8)(1);
        mission_flow.transition.stream_phase = (uint8)(2);
        game_runtime.metrics.fade.duration = (uint16)(30);
        game_runtime.metrics.fade.ticks = (uint16)(0);
    }
    if (mission_flow.transition.stream_phase == 2)
    {
        if (game_runtime.metrics.fade.active == 0)
        {
            mission_dispatch_scene_request();
            mission_audio_stop_groups();
        }
    }
    return;
}

uint32 mission_initialize_scene(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 scene, table, flags, value, cursor;
    uint32 i, j, w0, w1, w2, near_plane, far_plane, projection_distance;
    uint32 index, limit, amount;
    sint32 signed_value;
    static const uint8 aircraft_kinds[38] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 4, 6, 16, 20, 22, 23, 0};
    SVECTOR angles;
    MATRIX matrix = {0};
    FUNCTION_MARKER(0x80032754, "ACE3.BIN");
    if ((sint16)game_runtime.playback.resume_count == 0)
        mission_asset_load_poll();
    flags = game_runtime.display.flags;
    game_runtime.display.field_wait = (uint32)(2);
    game_runtime.display.mode = (uint32)(0);
    game_runtime.display.flags = (uint32)(flags | 5);
    flags = game_runtime.flags;
    value = game_runtime.playback.resume_mode;
    game_runtime.flags = flags | 0x10000000;
    game_runtime.resource_context.pending_mode = (uint32)(value);
    game_runtime.resource_context.pending_argument = 0;
    DrawSync(0);
    arena_set_cursor(game_runtime.resource_context.arena_mark);
    // retain arena addresses until remaining buffer owners migrate
    scene = arena_alloc(700);
    game_runtime.scene_initialized = (scene != 0);
    game_runtime.mission_control.flags = 0;
    game_runtime.mission_control.mode = 0;
    game_runtime.mission_control.argument = 0;
    game_runtime.mission_control.ticks = 0;
    game_runtime.mission_menu.options_sel = (uint16)(0);
    game_runtime.mission_menu.confirm_sel = (uint16)(0);
    game_runtime.mission_bounds.min[0] = (uint32)(0x80000001);
    game_runtime.mission_bounds.max[0] = (uint32)(0x7FFFFFFF);
    game_runtime.mission_bounds.min[1] = (uint32)(0x80000001);
    game_runtime.mission_bounds.max[1] = (uint32)(0x7FFFFFFF);
    game_runtime.mission_bounds.min[2] = (uint32)(0x80000001);
    game_runtime.mission_bounds.max[2] = (uint32)(0x7FFFFFFF);
    game_runtime.scene_assets.tile_map = game_runtime.resource_context.mission_assets.tile_map;
    game_runtime.scene_assets.terrain_models = game_runtime.resource_context.mission_assets.terrain_models;
    game_runtime.scene_assets.model_headers = game_runtime.resource_context.mission_assets.model_headers;
    game_runtime.scene_assets.model_header_sources = game_runtime.resource_context.mission_assets.model_header_sources;
    game_runtime.scene_assets.model_header_count = game_runtime.resource_context.mission_assets.model_header_count;
    game_runtime.scene_assets.animated_headers = game_runtime.resource_context.mission_assets.animated_headers;
    game_runtime.scene_assets.animated_header_count = game_runtime.resource_context.mission_assets.animated_header_count;
    game_runtime.scene_assets.animated_model_source = game_runtime.resource_context.mission_assets.animated_model_source;
    game_runtime.scene_assets.normals = game_runtime.resource_context.mission_assets.normals;
    game_runtime.scene_assets.scripts = game_runtime.resource_context.mission_assets.scripts;
    mission_script_publish_scene();
    game_runtime.scene_assets.entities = game_runtime.resource_context.mission_assets.entities;
    game_runtime.scene_assets.environment = game_runtime.resource_context.mission_assets.environment;
    game_runtime.scene_assets.aircraft_headers = game_runtime.resource_context.aircraft_headers;
    game_runtime.scene_assets.aircraft_model_source = game_runtime.resource_context.aircraft_model_source;
    table = arena_alloc(256);
    for (i = 0; i < 2; ++i)
    {
        (void)arena_alloc(88);
    }
    for (i = 2; i < 32; ++i)
    {
        (void)arena_alloc(92);
    }
    for (i = 0; i < 32; ++i)
    {
        (void)arena_alloc(148);
    }
    for (i = 0; i < 64; ++i)
    {
        pose_owner_object = mission_actor(((uint32)((sint32)(i * 4) >> 2)));
        actor_motion_object = mission_actor_motion(((uint32)((sint32)(i * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(i * 4) >> 2)));
        actor_traits_object->flags = (uint32)(0);
        actor_traits_object->model = (uint8)(0);
        actor_traits_object->render_flags = (uint8)(0);
        actor_traits_object->parameter = (uint16)(0);
        pose_owner_object->pose.position[0] = (uint32)(0);
        pose_owner_object->pose.position[1] = (uint32)(0);
        pose_owner_object->pose.position[2] = (uint32)(0);
        pose_owner_object->pose.angles[0] = (uint16)(0);
        pose_owner_object->pose.angles[1] = (uint16)(0);
        pose_owner_object->pose.angles[2] = (uint16)(0);
    }
    actor_index_table = 0;
    for (i = 0; i < 64; ++i)
    {
        actor_motion_object = mission_actor_motion(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        actor_motion_object->angular_delta[0] = (uint16)(0);
        actor_motion_object->angular_delta[1] = (uint16)(0);
        actor_motion_object->angular_delta[2] = (uint16)(0);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_traits_object->kind = (uint16)(0);
        actor_traits_object->radius[0] = (uint16)(0);
        actor_traits_object->radius[1] = (uint16)(0);
        actor_traits_object->health = (uint16)(0);
        actor_traits_object->damage_source = (uint32)(0);
        actor_traits_object->events = (uint32)(0);
        actor_index_table += 1;
    }
    actor_motion_object = mission_actor_motion(0);
    actor_traits_object = mission_actor_traits(0);
    value = actor_traits_object->render_flags;
    actor_traits_object->render_flags = (uint8)(value | 1);
    near_plane = game_runtime.render_settings.near_plane;
    far_plane = game_runtime.render_settings.far_plane;
    camera_initialize(296, near_plane, far_plane, game_runtime.aircraft_mission_metadata.camera_offsets);
    game_runtime.mission_matrices.camera.t[2] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.camera.t[1] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.camera.t[0] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[2] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[1] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[0] = (sint32)(uint32)(0);
    xport_gte_write_control(24, 0);
    xport_gte_write_control(25, 0);
    xport_gte_write_control(26, ((sint16)game_runtime.camera_proj.distance));
    near_plane = ((sint16)game_runtime.camera_proj.near_plane);
    far_plane = ((sint16)game_runtime.camera_proj.far_plane);
    projection_distance = ((sint16)game_runtime.camera_proj.distance);
    scene_geometry_set_depth_cue(near_plane, far_plane, projection_distance);
    (void)arena_alloc(36);
    near_plane = game_runtime.render_settings.near_plane;
    far_plane = game_runtime.render_settings.far_plane;
    scene_build_depth_buckets(near_plane, far_plane, game_runtime.mission_depth_buckets);
    game_runtime.mission_lighting.colors.m[0][0] = game_runtime.render_settings.light_colors[0][0];
    game_runtime.mission_lighting.colors.m[1][0] = game_runtime.render_settings.light_colors[0][1];
    game_runtime.mission_lighting.colors.m[2][0] = game_runtime.render_settings.light_colors[0][2];
    game_runtime.mission_lighting.colors.m[0][1] = game_runtime.render_settings.light_colors[1][0];
    game_runtime.mission_lighting.colors.m[1][1] = game_runtime.render_settings.light_colors[1][1];
    value = (uint16)game_runtime.render_settings.light_colors[1][2];
    game_runtime.mission_lighting.colors.m[0][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[1][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[2][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[2][1] = (sint16)(uint16)(value);
    for (i = 0; i < 9; ++i)
        game_runtime.mission_lighting.directions.m[i / 3][i % 3] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.back_color[0] = (uint32)game_runtime.render_settings.ambient_color[0];
    game_runtime.mission_lighting.back_color[1] = (uint32)game_runtime.render_settings.ambient_color[1];
    game_runtime.mission_lighting.back_color[2] = (uint32)game_runtime.render_settings.ambient_color[2];
    for (j = 0; j < 2; ++j)
    {
        angles.vx = game_runtime.render_settings.light_angles[j][0];
        angles.vy = game_runtime.render_settings.light_angles[j][1];
        angles.vz = 0;
        angles.pad = 0;
        RotMatrixYXZ(&angles, &matrix);
        i = 5;
        SetRotMatrix(&matrix);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 4096);
        xport_gte_execute(0x486012);
        w0 = xport_gte_read_data(9);
        w1 = xport_gte_read_data(10);
        w2 = xport_gte_read_data(11);
        game_runtime.mission_lighting.directions.m[j][0] = (sint16)(uint16)(w0);
        game_runtime.mission_lighting.directions.m[j][1] = (sint16)(uint16)(w1);
        game_runtime.mission_lighting.directions.m[j][2] = (sint16)(uint16)(w2);
    }
    mission_status_text_init();
    mission_initialize_variant();
    actor_index_table = 0;
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    actor_traits_object->model = (uint8)(0);
    index = (sint16)game_runtime.selection.aircraft;
    actor_traits_object->kind = (uint16)(aircraft_kinds[(sint32)index]);
    hud_initialize_mission();
    effect_trails_initialize();
    effect_system_init();
    effect_system_initialize();
    flight_initialize_actor(mission_actor_traits(0));
    mission_player_apply_initial_pose();
    actor_index_table = 2;
    for (i = 2; i < 64; ++i)
    {
        actor_motion_object = mission_actor_motion(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        flags = actor_traits_object->flags;
        if (flags & 0x4000)
        {
            index = actor_traits_object->model;
            if ((flags & 0x20) && !(actor_traits_object->render_flags & 2))
            {
                limit = game_runtime.asset_counts.animated_model_entries;
                if ((sint32)index >= (sint32)limit)
                    goto next_object;
                if (!game_runtime.scene_assets.animated_headers || index >= game_runtime.scene_assets.animated_header_count)
                    abort();
                const SCENE_SPRITE_MODEL_HEADER *prefix = scene_sprite_model_header(game_runtime.scene_assets.animated_model_source, game_runtime.scene_assets.animated_headers[index], sizeof(*prefix));
                amount = prefix->radius;
            }
            else
            {
                limit = game_runtime.asset_counts.model_entries;
                if ((sint32)(index * 4) >= (sint32)limit)
                    goto next_object;
                uint32 model_slot = index * 4;
                const GAME_SCENE_ASSETS *models = &game_runtime.scene_assets;
                if (!models->model_headers || model_slot >= models->model_header_count || !models->model_headers[model_slot])
                    abort();
                uint16 radius = models->model_headers[model_slot]->radius;
                value = radius;
                signed_value = (sint16)value >> 2;
                if (signed_value < 15)
                    signed_value = 15;
                amount = signed_value;
            }
            if (actor_traits_object->flags & 0x20)
            {
                signed_value = (sint16)actor_traits_object->radius[0];
                value = actor_traits_object->radius[0];
                if (signed_value <= 0)
                    actor_traits_object->radius[0] = (uint16)(amount - value);
                signed_value = (sint16)actor_traits_object->radius[1];
                value = actor_traits_object->radius[1];
                if (signed_value <= 0)
                    actor_traits_object->radius[1] = (uint16)(amount - value);
            }
            else
            {
                w0 = actor_traits_object->radius[0];
                w1 = actor_traits_object->radius[1];
                actor_traits_object->radius[0] = (uint16)(w0 + amount);
                actor_traits_object->radius[1] = (uint16)(w1 + amount);
            }
        }
    next_object:
        actor_index_table += 1;
    }
    actor_index_table = 0;
    for (i = 0; i < 2; ++i)
    {
        actor_motion_object = mission_actor_motion(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        actor_traits_object->radius[0] = (uint16)(54);
        actor_traits_object->radius[1] = (uint16)(54);
        actor_index_table += 1;
    }
    mission_apply_hud_visibility(1);
    mission_bounds_initialize((sint16)game_runtime.selection.mission);
    cursor = arena_peek(0);
    game_runtime.mission_control.render_arena = (uint32)(cursor);
    if (game_runtime.playback.demo_mode)
    {
        mission_allocate_packet_banks(cursor);
        game_runtime.resource_context.pending_mode = (uint32)(0x4000000);
        game_runtime.resource_context.pending_argument = (uint32)(0);
    }
    else
    {
        mission_rebuild_render_arena(cursor);
    }
    game_runtime.mission_control.mode = (uint32)(game_runtime.resource_context.pending_mode);
    game_runtime.mission_control.argument = (uint32)(game_runtime.resource_context.pending_argument);
    return mission_apply_mode_transition(0, 0);
}

static void mission_enter_mode_2(uint32 index)
{
    FUNCTION_MARKER(0x80031E64, "ACE3.BIN");
    mission_script_start_legacy(mission_event_script(MISSION_EVENT_ENTER_MODE_2, index));
    {
        mission_spawn_begin_phase();
        return;
    }
}

static void mission_leave_mode_8(void)
{
    FUNCTION_MARKER(0x80031E9C, "ACE3.BIN");
}

static void mission_enter_mode_16(uint32 index)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80031EA4, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_16, index);
    mission_script_start_legacy(script);
    {
        (void)(mission_actor_pause_begin());
        return;
    }
}

static void mission_leave_mode_64(void)
{
    FUNCTION_MARKER(0x80031EDC, "ACE3.BIN");
    {
        (void)(camera_initialize_mission());
        return;
    }
}

static void mission_enter_mode_128(void)
{
    FUNCTION_MARKER(0x80031EFC, "ACE3.BIN");
    {
        (void)(resource_cancel_streams());
        return;
    }
}

static void mission_leave_mode_512(void)
{
    FUNCTION_MARKER(0x80031F1C, "ACE3.BIN");
}

static void mission_enter_mode_1024(uint32 index)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80031F24, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_1024, index);
    mission_script_start_legacy(script);
    mission_spawn_apply();
}

static void mission_leave_mode_8192(void)
{
    FUNCTION_MARKER(0x80031F5C, "ACE3.BIN");
}

static void mission_enter_mode_16384(uint32 index)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80031F64, "ACE3.BIN");
    camera_initialize_mission();
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_16384, index);
    {
        mission_script_start_legacy(script);
        return;
    }
}

static void mission_leave_mode_65536(void)
{
    FUNCTION_MARKER(0x80031FA0, "ACE3.BIN");
}

static void mission_enter_mode_8(uint32 index)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80031FA8, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_8, index);
    {
        mission_script_start_legacy(script);
        return;
    }
}

static void mission_enter_mode_64(uint32 flags)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80031FD8, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_64, flags);
    {
        mission_script_start_legacy(script);
        return;
    }
}

static void mission_enter_mode_512(void)
{
    FUNCTION_MARKER(0x80032008, "ACE3.BIN");
}

static void mission_enter_mode_8192(uint32 flags)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80032010, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_ENTER_MODE_8192, flags);
    {
        mission_script_start_legacy(script);
        return;
    }
}

static void mission_run_shared_mode_script(uint32 flags)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80032040, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_SHARED_MODE, flags);
    {
        mission_script_start_legacy(script);
        return;
    }
}

static void mission_enter_mode_1048576(void)
{
    FUNCTION_MARKER(0x80032070, "ACE3.BIN");
}

static void mission_enter_mode_2097152(void)
{
    FUNCTION_MARKER(0x80032078, "ACE3.BIN");
}

uint32 mission_apply_mode_transition(uint32 old_mode, uint32 old_argument)
{
    uint32 new_mode, new_argument, result = 0, value;
    FUNCTION_MARKER(0x80032080, "ACE3.BIN");
    new_mode = game_runtime.mission_control.mode;
    new_argument = game_runtime.mission_control.argument;
    if (old_mode == 8)
        mission_leave_mode_8();
    if (old_mode == 64)
        mission_leave_mode_64();
    if (old_mode == 512)
        mission_leave_mode_512();
    if (old_mode == 0x2000)
        mission_leave_mode_8192();
    if (old_mode == 0x10000)
        mission_leave_mode_65536();
    if (new_mode == 2)
        mission_enter_mode_2(new_argument);
    if (new_mode == 16)
        mission_enter_mode_16(new_argument);
    if (new_mode == 128)
        mission_enter_mode_128();
    if (new_mode == 1024)
        mission_enter_mode_1024(new_argument);
    if (new_mode == 0x4000)
        mission_enter_mode_16384(new_argument);
    if (new_mode == 8)
        mission_enter_mode_8(old_argument);
    if (new_mode == 64)
        mission_enter_mode_64(old_argument);
    if (new_mode == 512)
        mission_enter_mode_512();
    if (new_mode == 0x2000)
        mission_enter_mode_8192(old_argument);
    if (new_mode == 0x10000)
        mission_run_shared_mode_script(old_argument);
    if (new_mode == 0x80000)
        mission_run_shared_mode_script(old_argument);
    if (new_mode == 0x100000)
        mission_enter_mode_1048576();
    if (new_mode == 0x200000)
        mission_enter_mode_2097152();
    switch (new_mode)
    {
        case 1:
            result = 0x60000001;
            resource_init_queue(0, 0);
            break;
        case 2:
            result = 0x10000007;
            break;
        case 16:
            result = 0x10000008;
            break;
        case 128:
            result = 0x10000009;
            break;
        case 1024:
            result = 0x1000000A;
            break;
        case 0x4000:
            result = 0x1000000B;
            break;
        case 0x20000:
            result = 0x1000000C;
            break;
        case 0x100000:
            result = 0x1000000D;
            break;
        case 0x200000:
            result = 0x1000000E;
            break;
        case 0x4000000:
            result = 0x1000000F;
            break;
        case 0x10000000:
            value = game_runtime.playback.resume_count;
            result = 0x10000006;
            game_runtime.playback.resume_count = (uint16)(value + 1);
            mission_audio_stop_groups();
            resource_init_queue(0, 0);
            movie_stop_output();
            break;
        case 0x400000:
            result = 0x60000004;
            break;
        case 0x800000:
            result = 0x60000003;
            break;
        case 0x1000000:
            result = 0x60000005;
            break;
        case 0x2000000:
            result = 0x50000000;
            break;
    }
    if (new_mode == 0x400000 || new_mode == 0x800000 || new_mode == 0x1000000 || new_mode == 0x2000000)
    {
        mission_audio_stop_groups();
        resource_init_queue(0, 0);
        movie_stop_output();
    }
    return result;
}

uint32 mission_request_mode(uint32 mode, uint32 argument)
{
    uint32 result;
    FUNCTION_MARKER(0x80032F18, "ACE3.BIN");
    game_runtime.resource_context.pending_mode = (uint32)(mode);
    game_runtime.resource_context.pending_argument = (uint32)(argument);
    result = game_runtime.mission_control.flags | 0x20000000;
    game_runtime.mission_control.flags = (uint32)(result);
    return result;
}

uint32 mission_finish_frame(void)
{
    uint32 flags, value, mode, argument;
    FUNCTION_MARKER(0x80033DC0, "ACE3.BIN");
    if (game_runtime.playback.demo_mode == 0)
        mission_transition_update();
    if ((game_runtime.flags & 0x20000000) == 0)
    {
        value = game_runtime.mission_control.ticks;
        game_runtime.mission_control.ticks = (uint32)(value + 1);
    }
    flags = game_runtime.mission_control.flags;
    if (flags & 0x40000000)
    {
        game_runtime.mission_control.flags = (uint32)(flags & 0xBFFFFFFF);
        mission_request_mode(0x10000000, 0);
    }
    else if ((sint32)flags < 0)
    {
        game_runtime.account.mission_failed = (uint8)(1);
        value = game_runtime.mission_control.flags;
        game_runtime.mission_control.flags = (uint32)(value & 0x7FFFFFFF);
        mission_request_mode(0x800000, 0);
    }
    flags = game_runtime.mission_control.flags;
    if ((flags & 0x20000000) == 0)
        return 0;
    mode = game_runtime.mission_control.mode;
    argument = game_runtime.mission_control.argument;
    game_runtime.mission_control.flags = (uint32)(flags & 0xDFFFFFFF);
    value = game_runtime.resource_context.pending_mode;
    game_runtime.mission_control.mode = (uint32)(value);
    value = game_runtime.resource_context.pending_argument;
    game_runtime.mission_control.ticks = (uint32)(0);
    game_runtime.mission_control.argument = (uint32)(value);
    return mission_apply_mode_transition(mode, argument);
}

void mission_initialize_variant(void)
{
    uint32 source, seed, random, offset;
    MISSION_VARIANT_INPUT native_variant;
    FUNCTION_MARKER(0x80038B2C, "ACE3.BIN");
    source = game_runtime.scene_assets.scripts.legacy_base;
    native_variant.root = mission_script_scene_source();
    if (!native_variant.root.bytes || !native_variant.root.size || native_variant.root.legacy_address != source)
        native_variant.root = mission_script_reference(source);
    native_variant.selected = native_variant.root;
    uint32 variant_count;
    if (!native_variant.root.bytes || native_variant.root.legacy_address != source || native_variant.root.size < sizeof(variant_count))
        abort();
    const uint8 *variant_header = native_variant.root.bytes;
    memcpy(&variant_count, variant_header, sizeof(variant_count));
    if (variant_count == 3)
    {
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        if (native_variant.root.size < 4 + 3 * sizeof(offset))
            abort();
        const uint8 *variant_offsets = native_variant.root.bytes + 4;
        memcpy(&offset, variant_offsets + (random % 3) * sizeof(offset), sizeof(offset));
        source += offset;
        native_variant.selected = mission_script_reference(source);
    }
    if (!native_variant.selected.bytes || native_variant.selected.size < 20)
        abort();
    mission_actor_bind_scene_resources(native_variant);
    mission_event_scripts_initialize(native_variant);
    mission_metrics_initialize(native_variant);
    subtitles_initialize();
    uint32 speech_root = game_runtime.scene_assets.scripts.legacy_base;
    uint32 speech_limit = game_runtime.scene_assets.entities.legacy_base;
    uint32 speech_source = native_variant.selected.legacy_address;
    if ((speech_root & 3) || speech_limit <= speech_root || speech_source < speech_root || speech_source >= speech_limit)
        abort();
    uint32 speech_bytes = speech_limit - speech_root;
    if (speech_bytes < 4)
        abort();
    if (native_variant.root.legacy_address != speech_root || speech_bytes > native_variant.root.size)
        abort();
    const uint8 *speech_data = native_variant.root.bytes;
    speech_queue_init(speech_data, speech_bytes, speech_source - speech_root);
    mission_status_text_reset();
    mission_spawn_reset();
    mission_result_init();
    mission_primary_initialize_menu_actor();
    mission_secondary_initialize_frame();
    mission_target_links_initialize();
    camera_bind_player();
    mission_prompt_init();
    {
        (void)(proximity_audio_reset());
        return;
    }
}

void mission_apply_hud_visibility(uint32 mode)
{
    uint32 value;
    FUNCTION_MARKER(0x80035740, "ACE3.BIN");
    if (mode != 0 && game_runtime.options.hud_hidden == 0)
    {
        if ((game_runtime.camera_control.mode & 0x800) == 0)
        {
            value = game_runtime.mission_control.flags & 0xFFFFFFFE;
            game_runtime.mission_control.flags = (uint32)(value);
            {
                (void)(uint32)(value);
                return;
            }
        }
    }
    value = game_runtime.mission_control.flags | 1;
    game_runtime.mission_control.flags = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 mission_frame_overlay_movie(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x8003423C, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_movie_overlay_frame();
    return mission_finish_frame();
}

uint32 mission_frame_movie(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x80034274, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFD);
    mission_movie_update_frame();
    return mission_finish_frame();
}

void mission_confirm_update(void)
{
    uint32 flags, value, result, selected;
    FUNCTION_MARKER(0x80035044, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags | 64);
    flags = game_input.repeat;
    if ((flags & 1) != 0)
    {
        value = (uint16)game_runtime.mission_menu.confirm_sel;
        game_runtime.mission_menu.confirm_sel = (uint16)(value - 1);
    }
    flags = game_input.repeat;
    if ((flags & 2) != 0)
    {
        value = (uint16)game_runtime.mission_menu.confirm_sel;
        game_runtime.mission_menu.confirm_sel = (uint16)(value + 1);
    }
    value = (sint16)game_runtime.mission_menu.confirm_sel;
    if ((sint32)value < 0)
    {
        game_runtime.mission_menu.confirm_sel = (uint16)(1);
        value = (sint16)game_runtime.mission_menu.confirm_sel;
    }
    if ((sint32)value >= 2)
        game_runtime.mission_menu.confirm_sel = (uint16)(0);
    result = game_input.pressed & 16;
    if (result == 0)
        return;
    selected = (sint16)game_runtime.mission_menu.confirm_sel;
    if (selected != 0 && selected != 1)
        return;
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags | (selected == 0 ? 0x40000000 : 0x80000000));
    return;
}

void mission_retry_wait_start(void)
{
    uint32 flags, pressed, result;
    FUNCTION_MARKER(0x8003514C, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags | 64);
    pressed = game_input.pressed & 64;
    result = flags | 96;
    if (pressed != 0)
        game_runtime.mission_control.flags = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 mission_retry_draw_prompt(uint32 object, uint32 packet)
{
    uint32 ticks, value, color;
    FUNCTION_MARKER(0x800353E8, "ACE3.BIN");
    ticks = game_runtime.mission_control.ticks;
    value = (uint16)game_wave_at(((ticks << 9) & 0x7E00));
    value = (sint32)(value << 16) >> 18;
    if ((sint32)value < 0)
        value = 0 - value;
    color = ((uint32)((sint32)value >> 6)) << 8;
    return ui_draw_compact_label(object, packet, 40, 212, color, (const uint8 *)"PRESS START BUTTON TO RETRY");
}
