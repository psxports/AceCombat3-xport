#include "game_functions.h"

void mission_result_init(void)
{
    MISSION_ACTOR *pose_owner_object;

    uint32 value;
    sint32 coordinate;
    FUNCTION_MARKER(0x8004856C, "ACE3.BIN");
    game_runtime.mission_result.reserved[0] = 0;
    game_runtime.mission_result.reserved[1] = 0;
    game_runtime.mission_result.elapsed_ticks = 0;
    value = (uint8)game_runtime.selection.mission;
    game_runtime.mission_result.mission = (uint8)value;
    value = (uint8)game_runtime.selection.mission;
    game_runtime.mission_result.rating = 4;
    game_runtime.mission_result.event_count = 0;
    game_runtime.mission_result.route_mission = (uint8)value;
    game_runtime.mission_result.sample_count = 0;
    game_runtime.account.result_kind = (uint8)(0);
    pose_owner_object = mission_actor(0);
    coordinate = ((sint32)pose_owner_object->pose.position[0] >> 12) + 0x40000;
    coordinate >>= 11;
    if (coordinate < 0)
        coordinate = 0;
    if (coordinate >= 256)
        coordinate = 255;
    game_runtime.mission_result.trace[0].x = (uint8)coordinate;
    coordinate = ((sint32)pose_owner_object->pose.position[2] >> 12) + 0x40000;
    coordinate >>= 11;
    if (coordinate < 0)
        coordinate = 0;
    if (coordinate >= 256)
        coordinate = 255;
    game_runtime.mission_result.trace[0].y = (uint8)coordinate;
    return;
}

void mission_result_record_trace(void)
{
    MISSION_ACTOR *pose_owner_player;
    uint32 actor_index_table;

    uint32 tick, high, quotient, count, value, coordinate;
    FUNCTION_MARKER(0x80048658, "ACE3.BIN");
    tick = game_runtime.mission_control.ticks;
    actor_index_table = 0;
    pose_owner_player = mission_actor(actor_index_table);
    high = (((sint64)(sint32)tick * (sint32)0x88888889) >> 32);
    quotient = (uint32)((sint32)(high + tick) >> 6) - (uint32)((sint32)tick >> 31);
    if (tick == (((quotient << 4) - quotient) << 3))
    {
        count = (sint16)game_runtime.mission_result.sample_count;
        if ((sint32)count < 450)
        {
            coordinate = pose_owner_player->pose.position[0];
            value = (uint32)((sint32)coordinate >> 12) + 0x40000;
            value = ((sint32)value >> 11);
            if ((sint32)value < 0)
                value = 0;
            if ((sint32)value >= 256)
                value = 255;
            game_runtime.mission_result.trace[count].x = (uint8)value;
            coordinate = pose_owner_player->pose.position[2];
            value = (uint32)((sint32)coordinate >> 12) + 0x40000;
            value = ((sint32)value >> 11);
            if ((sint32)value < 0)
                value = 0;
            if ((sint32)value >= 256)
                value = 255;
            count = (sint16)game_runtime.mission_result.sample_count;
            game_runtime.mission_result.trace[count].y = (uint8)value;
            count = game_runtime.mission_result.sample_count;
            game_runtime.mission_result.sample_count = (uint16)(count + 1);
        }
    }
    value = game_runtime.mission_result.elapsed_ticks + 1;
    game_runtime.mission_result.elapsed_ticks = (uint16)value;
    {
        (void)(uint32)(value);
        return;
    }
}

void mission_result_record_event(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 slot, value, flags, mode, kind, counter, coordinate;
    GAME_MISSION_EVENT *entry;
    FUNCTION_MARKER(0x80048758, "ACE3.BIN");
    actor_index_table = 0;
    slot = game_runtime.mission_result.event_count;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    value = (uint8)actor_traits_object->kind;
    entry = &game_runtime.mission_result.events[slot];
    entry->kind = (uint8)(value);
    flags = (*mission_actor_flags(index));
    if ((flags & 0x800) == 0)
        return;
    kind = (sint16)actor_traits_object->kind;
    if (kind == 45)
    {
        value = game_runtime.metrics.kind45_count;
        mode = (sint16)game_runtime.selection.mission;
        game_runtime.metrics.kind45_count = (uint8)(value + 1);
        value = game_runtime.metrics.kind45_count;
        if (value < 4)
            return;
        if (mode != 17 && mode != 31 && mode != 38 && mode != 46 && mode != 52)
            return;
    }
    flags = actor_traits_object->flags;
    if ((flags & 0x80) == 0)
        entry->kind = (uint8)(0x74);
    else
    {
        kind = entry->kind;
        if (kind == 48)
            entry->kind = (uint8)(0x72);
        else if (kind == 49)
            entry->kind = (uint8)(0x73);
        else if (kind > 49)
        {
            flags = (*mission_actor_flags(index));
            entry->kind = (uint8)((flags & 0x400) != 0 ? 0x70 : 0x71);
        }
    }
    mode = (sint16)game_runtime.selection.mission;
    if (mode == 33 && (sint16)actor_traits_object->kind == 27)
        goto active_record;
    flags = (*mission_actor_flags(index));
    if ((flags & 0x100) != 0)
    {
        value = actor_traits_object->damage_source;
        if ((value & 63) != 0)
            goto active_record;
        entry->category = (uint8)(4);
        goto empty_record;
    }
    flags = actor_traits_object->flags;
    if ((flags & 0x100) != 0)
    {
        entry->category = (uint8)(3);
        value = actor_traits_object->damage_source;
        if ((value & 63) == 0)
        {
            value = game_runtime.metrics.category3_count;
            game_runtime.metrics.category3_count = (uint8)(value + 1);
        }
        goto finish_record;
    }
    value = actor_traits_object->damage_source;
    if ((value & 63) == 0)
    {
        entry->category = (uint8)(0);
        goto empty_record;
    }
    if ((flags & 0x40) != 0)
    {
        entry->category = (uint8)(2);
        goto finish_record;
    }
active_record:
    entry->category = (uint8)(1);
    value = game_runtime.metrics.category1_count;
    game_runtime.metrics.category1_count = (uint8)(value + 1);
    goto finish_record;
empty_record:
    value = game_runtime.metrics.category0_4_count;
    game_runtime.metrics.category0_4_count = (uint8)(value + 1);
finish_record:
    entry->object_index = (uint8)(index);
    coordinate = pose_owner_object->pose.position[0];
    coordinate = (uint32)((sint32)coordinate >> 12) + 0x40000;
    coordinate = (sint32)coordinate >> 11;
    if ((sint32)coordinate < 0)
        coordinate = 0;
    if ((sint32)coordinate >= 256)
        coordinate = 255;
    entry->x = (uint8)(coordinate);
    coordinate = pose_owner_object->pose.position[2];
    coordinate = (uint32)((sint32)coordinate >> 12) + 0x40000;
    coordinate = (sint32)coordinate >> 11;
    if ((sint32)coordinate < 0)
        coordinate = 0;
    if ((sint32)coordinate >= 256)
        coordinate = 255;
    entry->y = (uint8)(coordinate);
    value = game_runtime.mission_result.elapsed_ticks;
    counter = (uint32)(((uint64)value * 0x88888889) >> 32) >> 4;
    entry->seconds = (uint16)(counter);
    value = game_runtime.mission_result.event_count + 1;
    game_runtime.mission_result.event_count = (uint8)value;
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 mission_result_last_object(void)
{
    uint32 count, value;
    const GAME_MISSION_EVENT *entry;
    FUNCTION_MARKER(0x80048A80, "ACE3.BIN");
    count = game_runtime.mission_result.event_count;
    if (count == 0)
        return 0xFFFFFFFF;
    do
    {
        entry = &game_runtime.mission_result.events[count - 1];
        value = entry->category;
        --count;
        if (value == 0)
            return entry->object_index;
    } while (count != 0);
    return 0xFFFFFFFF;
}

uint32 mission_result_compute_rating(void)
{
    uint32 mode, a, b, limit, value;
    FUNCTION_MARKER(0x80054DBC, "ACE3.BIN");
    mode = game_runtime.mission_result.rating_rules.mode;
    if (mode == 255)
        return 5;
    if (mode == 0)
        return 4;
    if (mode == 1)
    {
        a = game_runtime.metrics.category3_count;
        b = game_runtime.metrics.flagged_count[1];
        limit = game_runtime.mission_result.rating_rules.count_threshold[2];
        if ((sint32)limit < (sint32)(a + b))
            return 3;
        a = game_runtime.metrics.category0_4_count;
        b = game_runtime.metrics.category1_count;
        limit = game_runtime.mission_result.rating_rules.count_threshold[1];
        value = a + b;
        if ((sint32)value < (sint32)limit)
            return 3;
        limit = game_runtime.mission_result.rating_rules.count_threshold[0];
        if ((sint32)value < (sint32)limit)
            return 2;
        return 1;
    }
    if (mode == 2)
    {
        value = game_runtime.metrics.elapsed_ticks;
        limit = game_runtime.mission_result.rating_rules.value_threshold[0];
        if ((sint32)value < (sint32)limit)
            return 1;
        limit = game_runtime.mission_result.rating_rules.value_threshold[1];
        if ((sint32)value < (sint32)limit)
            return 2;
        return 3;
    }
    if (mode == 4)
    {
        value = game_runtime.metrics.score;
        limit = game_runtime.mission_result.rating_rules.value_threshold[0];
        if ((sint32)value >= (sint32)limit)
            return 1;
        limit = game_runtime.mission_result.rating_rules.value_threshold[1];
        if ((sint32)value < (sint32)limit)
            return 3;
        return 2;
    }
    return 4;
}

void mission_result_finish(void)
{
    uint32 value;
    FUNCTION_MARKER(0x80048A40, "ACE3.BIN");
    value = mission_result_compute_rating();
    game_runtime.mission_result.rating = (uint8)value;
    value = (uint16)game_runtime.metrics.elapsed_ticks;
    game_runtime.mission_result.elapsed_ticks = (uint16)value;
    {
        (void)(uint32)(value);
        return;
    }
}

static struct
{
    const uint8 *data;
    uint32 bytes;
} world_phase_input;

void mission_world_settings_reset(void)
{
    world_phase_input.data = NULL;
    world_phase_input.bytes = 0;
}

uint32 mission_phase_timeout(uint32 phase)
{
    if (phase < 4)
        return game_runtime.world_settings.phase_timeout[phase];
    uint32 offset = (phase << 2) + 36;
    uint32 timeout;
    if (!world_phase_input.data || offset > world_phase_input.bytes || sizeof(timeout) > world_phase_input.bytes - offset)
        abort();
    memcpy(&timeout, world_phase_input.data + offset, sizeof(timeout));
    return timeout;
}

uint8 mission_phase_message(uint32 phase)
{
    if (phase < 4 && game_runtime.world_settings.has_phase_messages)
        return game_runtime.world_settings.phase_message[phase];
    uint32 offset = phase + 72;
    if (!world_phase_input.data || offset > world_phase_input.bytes || 1 > world_phase_input.bytes - offset)
        abort();
    return world_phase_input.data[offset];
}

void mission_metrics_initialize(MISSION_VARIANT_INPUT native_variant)
{
    uint32 wire_word_0;
    uint32 variant = native_variant.selected.legacy_address;
    const uint8 *settings;
    const MISSION_WORLD_RECORD *record;
    uint32 source, value, i, span;
    FUNCTION_MARKER(0x80053BAC, "ACE3.BIN");
    if (!native_variant.selected.bytes || native_variant.selected.size < 20)
        abort();
    source = variant + (memcpy(&wire_word_0, native_variant.selected.bytes + 16, sizeof(wire_word_0)), wire_word_0);
    span = game_runtime.scene_assets.entities.legacy_base > source ? game_runtime.scene_assets.entities.legacy_base - source : 0;
    /* the four short world records continue into the first eight entity bytes */
    if (span == 68 && game_runtime.scene_assets.entities.bytes >= 8 && source >= 0x80000000u && source <= 0x80000000u + sizeof(DRAM) - 76)
        span = 76;
    game_runtime.world_settings.has_fourth_initial_timer = span >= 72;
    game_runtime.world_settings.has_phase_messages = span >= 76;
    span = span >= 76 ? 76 : (span >= 72 ? 72 : 68);
    uint32 settings_offset = source - native_variant.selected.legacy_address;
    if (source < native_variant.selected.legacy_address || settings_offset > native_variant.selected.size || span > native_variant.selected.size - settings_offset)
        abort();
    settings = native_variant.selected.bytes + settings_offset;
    world_phase_input.data = settings;
    world_phase_input.bytes = native_variant.selected.size - settings_offset;
    record = (const MISSION_WORLD_RECORD *)settings;
    game_runtime.world_settings.ground_offset = record->ground_offset;
    game_runtime.world_settings.minimum_y = record->minimum_y;
    game_runtime.world_settings.altitude_limit = record->altitude_limit;
    game_runtime.world_settings.minimum_x = record->minimum_x;
    game_runtime.world_settings.maximum_x = record->maximum_x;
    game_runtime.world_settings.minimum_z = record->minimum_z;
    game_runtime.world_settings.maximum_z = record->maximum_z;
    game_runtime.world_settings.warning_limit = record->warning_limit;
    game_runtime.world_settings.alternate_warning_limit = record->alternate_warning_limit;
    for (i = 0; i < 4; ++i)
    {
        game_runtime.world_settings.phase_timeout[i] = record->phase_timeout[i];
        game_runtime.world_settings.phase_timer_mode[i] = record->phase_timer_mode[i];
    }
    for (i = 0; i < 3; ++i)
        game_runtime.world_settings.phase_initial_ticks[i] = record->phase_initial_ticks[i];
    if (game_runtime.world_settings.has_fourth_initial_timer)
        game_runtime.world_settings.phase_initial_ticks[3] = record->phase_initial_ticks[3];
    if (game_runtime.world_settings.has_phase_messages)
        for (i = 0; i < 4; ++i)
            game_runtime.world_settings.phase_message[i] = record->phase_message[i];
    arena_alloc(64);
    game_runtime.metrics.proximity_active = (uint8)(0);
    game_runtime.metrics.flags = (uint32)(0);
    game_runtime.metrics.score = (uint32)(0);
    game_runtime.metrics.proximity_count = (uint16)(0);
    game_runtime.metrics.category3_count = (uint8)(0);
    game_runtime.metrics.timer_mode = (uint8)(0);
    game_runtime.metrics.elapsed_ticks = (uint32)(0);
    game_runtime.metrics.phase_ticks = (uint32)(0);
    game_runtime.metrics.display_ticks = (uint32)(0);
    game_runtime.metrics.timer_reserved = (uint8)(0);
    game_runtime.metrics.warning_ticks = (uint16)(0);
    game_runtime.metrics.fade.ticks = (uint16)(0);
    game_runtime.metrics.flagged_target[0] = (uint8)(0);
    game_runtime.metrics.flagged_target[1] = 0;
    game_runtime.metrics.flagged_count[0] = 0;
    game_runtime.metrics.flagged_count[1] = 0;
    game_runtime.metrics.script_count = 0;
    mission_flow.request.mission_arg = (uint8)(0);
    mission_flow.request.current_mission_arg = (uint8)(0);
    mission_flow.request.script_arg = 0;
    mission_flow.request.scene_arg = 0;
    mission_flow.request.movie_arg = 0;
    mission_flow.request.restore_arg = 0;
    mission_flow.request.saved_scene_arg = 0;
    mission_flow.request.mode = 0;
    mission_flow.request.flags = 0;
    mission_flow.request.event_id = (uint8)(255);
    value = mission_audio_music_index((sint16)game_runtime.selection.mission);
    mission_flow.request.music_id = (uint8)(value);
    game_runtime.metrics.category1_count = 0;
    game_runtime.metrics.category0_4_count = 0;
    game_runtime.metrics.special_result = 0;
    game_runtime.metrics.fade.active = 0;
    game_runtime.metrics.fade.mode = 0;
    game_runtime.mission_result.rating_rules.mode = 0;
    game_runtime.metrics.fade.duration = (uint16)(0);
    game_runtime.mission_result.rating_rules.count_threshold[0] = 0;
    game_runtime.mission_result.rating_rules.count_threshold[1] = 0;
    game_runtime.mission_result.rating_rules.count_threshold[2] = 0;
    game_runtime.metrics.kind45_count = (uint8)(0);
    game_runtime.mission_result.rating_rules.value_threshold[0] = 0;
    game_runtime.mission_result.rating_rules.value_threshold[1] = 0;
    mission_flow.request.effect_id = (uint8)(0);
    mission_flow.request.dispatch_count = (uint8)(0);
    {
        mission_script_start_legacy(mission_event_script(MISSION_EVENT_INITIAL, 0));
        return;
    }
}

void mission_objectives_update(void)
{
    uint32 selection, phase, buttons, kind, eligible, ticks, flags, ordering, packet, result, value;
    const MISSION_SCENARIO *record;
    FUNCTION_MARKER(0x8005C270, "ACE3.BIN");
    selection = mission_flow.scenario_idx;
    record = &mission_scenarios[selection];
    phase = game_runtime.mission_control.mode;
    if (phase == 0x400)
    {
        (void)mission_scenario_intro_update();
        return;
    }
    if (phase == 0x2000)
    {
        mission_objectives_update_scenario();
        return;
    }
    if (phase == 0x1000)
        return;
    buttons = flight_dynamics_flags();
    kind = record->kind;
    if (kind != 7 && (buttons & 0x200) == 0)
    {
        eligible = kind - 2 < 2;
        if (eligible == 0 && (kind < 2 || kind == 5 || kind == 6 || kind == 4))
        {
            ticks = game_runtime.mission_control.ticks;
            eligible = (sint32)ticks < 300;
        }
        if (eligible != 0 && (buttons & 0x40) == 0)
        {
            flags = game_input.pressed;
            if ((flags & 0x40) != 0)
            {
                flight_dynamics_advance_mode();
                hud_notice_show(9);
                pad_reset_slots();
                game_runtime.metrics.display_ticks = (uint32)(0);
                game_runtime.metrics.timer_mode = (uint8)(255);
            }
            ticks = game_runtime.mission_control.ticks;
            if ((ticks & 0x10) != 0)
            {
                ordering = game_runtime.mission_control.ot;
                packet = game_runtime.mission_control.packet_cur;
                result = ui_draw_compact_label(ordering, packet, 80, 208, 0x8300, (const uint8 *)"PRESS START BUTTON");
                game_runtime.mission_control.packet_cur = (uint32)(result);
                ordering = game_runtime.mission_control.ot;
                packet = game_runtime.mission_control.packet_cur;
                result = ui_draw_compact_label(ordering, packet, 96, 216, 0x8300, (const uint8 *)"FOR AUTO PILOT");
                game_runtime.mission_control.packet_cur = (uint32)(result);
            }
        }
    }
    ticks = game_runtime.mission_control.ticks;
    if (ticks == 3600)
        mission_status_text_show(10);
    if ((buttons & 0x7000) != 0)
    {
        (void)mission_request_mode(0x2000, 1);
        return;
    }
    ticks = game_runtime.mission_control.ticks;
    if ((sint32)ticks >= 5400)
    {
        (void)mission_request_mode(0x2000, 4);
        return;
    }
    if ((buttons & 0x80) != 0)
    {
        (void)mission_request_mode(0x2000, 2);
        return;
    }
    if ((buttons & 0x100) != 0)
    {
        (void)mission_request_mode(0x2000, 16);
        return;
    }
    if ((buttons & 0x200) != 0)
    {
        (void)mission_request_mode(0x2000, 0);
        return;
    }
    kind = record->kind;
    if (kind != 7)
        return;
    value = (sint16)game_runtime.metrics.proximity_count;
    if ((sint32)value >= 390)
    {
        mission_request_mode(0x2000, 2);
        game_runtime.metrics.special_result = (uint8)(1);
    }
    value = (sint16)game_runtime.metrics.proximity_count;
    if ((sint32)value < 0)
    {
        mission_request_mode(0x2000, 32);
        game_runtime.metrics.special_result = (uint8)(0);
    }
    ticks = game_runtime.mission_control.ticks;
    if (ticks != 900)
        return;
    value = game_runtime.metrics.proximity_active;
    if (value == 1)
    {
        mission_request_mode(0x2000, 2);
        game_runtime.metrics.special_result = (uint8)(value);
    }
    else
    {
        mission_request_mode(0x2000, 32);
        game_runtime.metrics.special_result = (uint8)(0);
    }
    return;
}

void mission_objectives_update_scenario(void)
{
    uint32 input, stage, scenario, mask, threshold, fade_time, value, result;
    FUNCTION_MARKER(0x8005BE0C, "ACE3.BIN");
    input = flight_dynamics_flags();
    stage = game_runtime.mission_control.argument;
    scenario = 255;
    if ((stage & 2) != 0)
    {
        if ((input & 0x7000) != 0)
        {
            speech_queue_stop();
            value = mission_flow.request.saved_scene_arg;
            mission_flow.request.scene_arg = (uint8)(value);
            value = mission_flow.request.saved_scene_arg;
            game_runtime.mission_control.argument = (uint32)(value);
            mission_request_mode(0x2000, 1);
            result = game_runtime.flags | 0x10000000;
            game_runtime.flags = result;
            return;
        }
        mask = mission_flow.scenario_mask;
        if ((mask & 0x300) != 0)
        {
            scenario = 12;
            threshold = 270;
            fade_time = threshold;
            if (game_runtime.mission_control.ticks == 0)
            {
                sound_set_voice_fade(0, 0, 150);
                sound_set_voice_fade(1, 30, 150);
                sound_set_voice_fade(2, 20, 150);
            }
        }
        else if ((mask & 0x400) != 0)
        {
            scenario = 20;
            threshold = 360;
            fade_time = threshold;
        }
        else if ((mask & 0x800) != 0)
        {
            scenario = 43;
            threshold = 360;
            fade_time = threshold;
        }
        else if ((mask & 0x6000) != 0)
        {
            scenario = 49;
            threshold = 120;
            fade_time = 0xFFFFFFFF;
        }
        else if ((mask & 0x1000) != 0)
        {
            scenario = 47;
            threshold = 180;
            fade_time = threshold;
        }
        else if ((mask & 0x8000) != 0)
        {
            threshold = 90;
            fade_time = threshold;
            input = 0x200;
        }
        if (game_runtime.mission_control.ticks == fade_time)
            goto start_fade;
    after_fade:
        if ((input & 0x200) == 0)
            goto finish;
        value = game_runtime.mission_control.ticks;
        if ((sint32)threshold >= (sint32)value)
            goto finish;
    }
    else if ((stage & 0x20) != 0)
    {
        if ((input & 0x7000) != 0)
        {
            speech_queue_stop();
            value = mission_flow.request.saved_scene_arg;
            mission_flow.request.scene_arg = (uint8)(value);
            value = mission_flow.request.saved_scene_arg;
            game_runtime.mission_control.argument = (uint32)(value);
            mission_request_mode(0x2000, 1);
            value = game_runtime.flags | 0x10000000;
            game_runtime.flags = value;
        }
        if (game_runtime.mission_control.ticks == 120)
            goto start_fade;
    after_stage_fade:
    {
    }
        if ((sint32)game_runtime.mission_control.ticks < 120)
            goto finish;
    }
    else if (stage != 0)
    {
        mask = mission_flow.scenario_mask;
        if ((mask & 0x300) != 0)
            scenario = 41;
        else if ((mask & 0x400) != 0)
            scenario = 42;
        else if ((mask & 0x800) != 0)
            scenario = 44;
        else if ((mask & 0x1000) != 0)
            scenario = 48;
        if (game_runtime.mission_control.ticks == 0)
            speech_queue_stop();
        if ((sint32)game_runtime.mission_control.ticks >= 120)
        {
            mission_primary_restart_scenario_ai();
            mission_audio_stop_groups();
            speech_queue_stop();
            value = mission_flow.request.saved_scene_arg;
            mission_flow.request.scene_arg = (uint8)(value);
            value = mission_flow.request.saved_scene_arg;
            mission_request_mode(0x1000, value);
            camera_set_scripted_mode();
        }
        goto finish;
    }
    else
    {
        if ((sint32)game_runtime.mission_control.ticks < 150 || speech_queue_mode() != 0)
            goto finish;
        goto end_scene;
    }
    if (speech_queue_mode() != 0)
        goto finish;
    if (game_runtime.metrics.fade.active != 0)
        goto finish;
end_scene:
    mission_primary_restart_scenario_ai();
    if (mission_flow.request.mode == 0)
        mission_result_scene_stop_hook();
    else
        mission_audio_stop_groups();
    mission_dispatch_scene_request();
finish:
{
}
    value = game_runtime.mission_control.ticks;
    if (value == 0 && scenario != 255)
        mission_status_text_show(scenario);
    return;
start_fade:
    game_runtime.metrics.fade.mode = (uint8)(2);
    game_runtime.metrics.fade.active = (uint8)(1);
    game_runtime.metrics.fade.duration = (uint16)(30);
    game_runtime.metrics.fade.ticks = (uint16)(0);
    if ((stage & 2) != 0)
        goto after_fade;
    goto after_stage_fade;
}

void mission_events_update(void)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_objects;

    uint32 mode, submode, mission, value, other, flags, offset, i, phase, limit, timer, result, draw, coordinate, random, update, packet, ordering;
    MISSION_SCRIPT_REFERENCE script;
    MISSION_ACTOR_TARGET *target_state;
    FUNCTION_MARKER(0x800540E0, "ACE3.BIN");
    mode = game_runtime.mission_control.mode;
    submode = game_runtime.mission_control.argument;
    if (mode == 2)
    {
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 15 && submode == 0)
        {
            value = game_runtime.mission_control.ticks;
            if (value == 0)
                effect_schedule_timed_positions(3);
        }
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 35)
        {
            value = game_runtime.mission_control.argument;
            if (value == 0)
            {
                value = game_runtime.mission_control.ticks;
                if (value == 0)
                    effect_schedule_timed_positions(4);
            }
        }
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 46)
        {
            value = game_runtime.mission_control.ticks;
            if (value == 0)
            {
                value = game_runtime.mission_control.argument;
                if (value == 0)
                    effect_schedule_timed_positions(1);
                value = game_runtime.mission_control.argument;
                if (value == 1)
                    effect_schedule_timed_positions(2);
            }
        }
    }
    if (mode != 4)
        goto final_timer;
    flags = game_runtime.flags;
    if ((flags & 0x20000000) == 0)
    {
        for (i = 0; i < 16; ++i)
        {
            value = mission_event_tick(submode, i);
            other = game_runtime.metrics.phase_ticks;
            if (other == value)
            {
                script = mission_event_timed_script(submode, i);
                mission_script_start_legacy(script);
            }
        }
        value = game_runtime.metrics.flagged_target[0];
        if (value != 0)
        {
            other = game_runtime.metrics.flagged_count[0];
            if (value == other)
            {
                script = mission_event_script(MISSION_EVENT_FIRST_TARGET_COUNT, submode);
                mission_script_start_legacy(script);
            }
        }
        value = game_runtime.metrics.flagged_target[1];
        if (value != 0)
        {
            other = game_runtime.metrics.flagged_count[1];
            if (value == other)
            {
                script = mission_event_script(MISSION_EVENT_SECOND_TARGET_COUNT, submode);
                mission_script_start_legacy(script);
            }
        }
        mission = (sint16)game_runtime.selection.mission;
        value = 0;
        if (mission == 9)
        {
            value = game_runtime.mission_control.argument;
        }
        if (mission != 9 || value == 0)
        {
            value = game_runtime.metrics.elapsed_ticks;
            game_runtime.metrics.elapsed_ticks = (uint32)(value + 1);
        }
        value = game_runtime.metrics.phase_ticks;
        other = game_runtime.metrics.timer_mode;
        game_runtime.metrics.phase_ticks = (uint32)(value + 1);
        if (other == 0)
        {
            value = game_runtime.metrics.elapsed_ticks;
            game_runtime.metrics.display_ticks = (uint32)(value);
        }
        else if (other == 1)
        {
            value = game_runtime.metrics.display_ticks;
            game_runtime.metrics.display_ticks = (uint32)((sint32)value > 0 ? value - 1 : 0);
        }
    }
    flags = game_runtime.metrics.flags;
    game_runtime.metrics.flags = (uint32)(flags & 0xFFFFFFEF);
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 3)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            flags = game_runtime.flags;
            target_state = mission_actor_target(2);
            if ((flags & 0x20000000) != 0)
                goto final_timer;
            value = target_state->distance;
            if ((sint32)value < 4096)
            {
                other = game_runtime.metrics.score;
                offset = (sint32)value < 256 ? 3 : (sint32)value < 1024 ? 2 : 1;
                game_runtime.metrics.score = (uint32)(other + offset);
                value = game_runtime.metrics.flags;
                game_runtime.metrics.flags = (uint32)(value | 16);
            }
        }
    }
    flags = game_runtime.flags;
    if ((flags & 0x20000000) != 0)
        goto final_timer;
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 14 || mission == 53)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            phase = game_runtime.mission_control.ticks;
            if (phase == 60)
            {
                if (mission == 14)
                {
                    value = game_runtime.world_settings.warning_limit;
                    game_runtime.metrics.warning_ticks = (uint16)(value - 151);
                }
                mission = (sint16)game_runtime.selection.mission;
                if (mission == 53)
                {
                    value = game_runtime.world_settings.warning_limit;
                    game_runtime.metrics.warning_ticks = (uint16)(value - 121);
                }
                flags = game_runtime.metrics.flags;
                game_runtime.metrics.flags = (uint32)(flags & 0xFFFFFFFD);
            }
            value = game_runtime.metrics.warning_ticks;
            flags = game_runtime.metrics.flags & 2;
            game_runtime.metrics.warning_ticks = (uint16)(value + 1);
            if (flags != 0)
            {
                game_runtime.metrics.timer_mode = (uint8)(1);
                limit = (sint16)game_runtime.world_settings.alternate_warning_limit;
                timer = (sint16)game_runtime.metrics.warning_ticks;
                game_runtime.metrics.display_ticks = (uint32)(limit - timer);
                mission = (sint16)game_runtime.selection.mission;
                if (mission == 14)
                {
                    limit = (sint16)game_runtime.world_settings.alternate_warning_limit;
                    timer = (sint16)game_runtime.metrics.warning_ticks;
                    if (timer == limit - 600)
                    {
                        script = mission_event_script(MISSION_EVENT_SIGNAL, 2);
                        mission_script_start_legacy(script);
                    }
                    limit = (sint16)game_runtime.world_settings.alternate_warning_limit;
                    timer = (sint16)game_runtime.metrics.warning_ticks;
                    if ((sint32)timer >= (sint32)(limit - 600))
                    {
                        actor_index_objects = 0;
                        pose_owner_object = mission_actor(actor_index_objects);
                        value = pose_owner_object->pose.position[1];
                        if ((sint32)value > -81920000)
                            hud_notice_show_pair(6);
                    }
                }
                timer = (sint16)game_runtime.metrics.warning_ticks;
                limit = (sint16)game_runtime.world_settings.alternate_warning_limit;
                if (timer == limit)
                {
                    mission = (sint16)game_runtime.selection.mission;
                    if (mission == 53)
                    {
                        script = mission_event_script(MISSION_EVENT_SIGNAL, 2);
                        mission_script_start_legacy(script);
                    }
                    flags = game_runtime.metrics.flags;
                    game_runtime.metrics.warning_ticks = (uint16)(0);
                    game_runtime.metrics.flags = (uint32)(flags & 0xFFFFFFFD);
                }
            }
            else
            {
                game_runtime.metrics.timer_mode = (uint8)(0);
                limit = (sint16)game_runtime.world_settings.warning_limit;
                timer = (sint16)game_runtime.metrics.warning_ticks;
                if (timer == limit - 150)
                {
                    script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
                    mission_script_start_legacy(script);
                }
                timer = (sint16)game_runtime.metrics.warning_ticks;
                limit = (sint16)game_runtime.world_settings.warning_limit;
                if (timer == limit)
                {
                    script = mission_event_script(MISSION_EVENT_SIGNAL, 1);
                    mission_script_start_legacy(script);
                    flags = game_runtime.metrics.flags;
                    game_runtime.metrics.warning_ticks = (uint16)(0);
                    game_runtime.metrics.flags = (uint32)(flags | 2);
                    mission = (sint16)game_runtime.selection.mission;
                    mission_status_text_show(mission == 53 ? 32 : 17);
                }
            }
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 14)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            flags = game_runtime.metrics.flags;
            if ((flags & 1) == 0)
            {
                draw = flags & 2;
                if (draw == 0)
                {
                    actor_index_objects = 0;
                    pose_owner_object = mission_actor(actor_index_objects);
                    value = pose_owner_object->pose.position[1];
                    if ((sint32)value > -81920000)
                    {
                        game_runtime.metrics.flags = (uint32)(flags | 1);
                        script = mission_event_script(MISSION_EVENT_SIGNAL, 3);
                        mission_script_start_legacy(script);
                    }
                    draw = game_runtime.metrics.flags & 2;
                }
                if (draw != 0)
                {
                    actor_index_objects = 0;
                    pose_owner_object = mission_actor(actor_index_objects);
                    value = pose_owner_object->pose.position[1];
                    if ((sint32)value > -81920000)
                    {
                        ordering = game_runtime.mission_control.ot;
                        packet = game_runtime.mission_control.packet_cur;
                        result = ui_draw_noise_overlay(ordering + 4, packet, 765);
                        game_runtime.mission_control.packet_cur = (uint32)(result);
                    }
                }
            }
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 6)
    {
        actor_index_objects = 0;
        pose_owner_object = mission_actor(actor_index_objects);
        value = pose_owner_object->pose.position[1];
        if ((sint32)value < -10485760)
        {
            speech_queue_stop();
            script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
            mission_script_start_legacy(script);
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 13)
    {
        flags = game_runtime.metrics.flags;
        if ((flags & 32) != 0)
        {
            value = game_runtime.metrics.empty_ammo_ticks;
            game_runtime.metrics.empty_ammo_ticks = (uint32)(value + 1);
        }
        else
            game_runtime.metrics.empty_ammo_ticks = (uint32)(0);
        value = game_runtime.metrics.empty_ammo_ticks;
        if (value == 18000)
        {
            script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
            mission_script_start_legacy(script);
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 44)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            flags = game_runtime.metrics.flags;
            if ((flags & 1) == 0)
            {
                actor_index_objects = 0;
                pose_owner_object = mission_actor(actor_index_objects);
                value = pose_owner_object->pose.position[1];
                if ((sint32)value < -6291456)
                    hud_notice_show_pair(4);
                value = pose_owner_object->pose.position[1];
                if ((sint32)value < -8388608)
                {
                    flags = game_runtime.metrics.flags;
                    game_runtime.metrics.flags = (uint32)(flags | 1);
                    script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
                    mission_script_start_legacy(script);
                }
                value = pose_owner_object->pose.position[0];
                if (value + 32767999 <= 0x7147FFE)
                {
                    coordinate = pose_owner_object->pose.position[2];
                    if ((sint32)coordinate > -73728000 && (sint32)coordinate <= 20479999)
                    {
                        flags = game_runtime.metrics.flags;
                        game_runtime.metrics.flags = (uint32)(flags | 1);
                        script = mission_event_script(MISSION_EVENT_SIGNAL, 1);
                        mission_script_start_legacy(script);
                    }
                }
            }
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 30)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            phase = game_runtime.mission_control.ticks;
            if (phase - 1200 < 0xA8D)
            {
                random = game_random();
                flags = game_runtime.flags;
                game_runtime.flags = flags | 0x10000000;
                phase = game_runtime.mission_control.ticks;
                if (phase == 1200)
                {
                    scene_audio_start_transition();
                    game_runtime.metrics.event_fade_strength = (uint32)(0);
                }
                else
                {
                    update = 0;
                    if ((sint32)phase < 1320)
                    {
                        value = game_runtime.metrics.event_fade_strength;
                        if ((sint32)value < 2048)
                        {
                            value += 18;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 1440)
                    {
                        value = 2048;
                        update = 1;
                    }
                    else if ((sint32)phase < 3150)
                    {
                        value = (random & 255) + 768;
                        update = 1;
                    }
                    else if ((sint32)phase < 3450)
                    {
                        value = game_runtime.metrics.event_fade_strength;
                        if ((sint32)value < 2048)
                        {
                            value += 8;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 3540)
                    {
                        value = game_runtime.metrics.event_fade_strength;
                        if ((sint32)value < 2048)
                        {
                            value += 16;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 3720)
                    {
                        value = 0xFFFFF800;
                        update = 1;
                    }
                    else if ((sint32)phase < 3900)
                    {
                        value = game_runtime.metrics.event_fade_strength;
                        if ((sint32)value < 0)
                        {
                            value += 16;
                            update = 1;
                        }
                    }
                    if (update != 0)
                        game_runtime.metrics.event_fade_strength = (uint32)(value);
                }
                value = game_runtime.metrics.event_fade_strength;
                ordering = game_runtime.mission_control.ot;
                packet = game_runtime.mission_control.packet_cur;
                result = ui_draw_noise_overlay(ordering + 4, packet, value);
                game_runtime.mission_control.packet_cur = (uint32)(result);
            }
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 42)
    {
        actor_index_objects = 0;
        pose_owner_object = mission_actor(actor_index_objects);
        phase = game_runtime.mission_control.ticks;
        coordinate = pose_owner_object->pose.position[1];
        phase &= 31;
        if ((sint32)coordinate >= 0 && phase == 0)
            flight_apply_player_damage_tick();
        if ((sint32)coordinate > 20479999 && phase == 16)
            flight_apply_player_damage_tick();
        if ((sint32)coordinate > 32767999 && (phase == 8 || phase == 24))
            flight_apply_player_damage_tick();
    }
    flags = game_runtime.metrics.flags;
    flags &= 0xFFFFFFFB;
    game_runtime.metrics.flags = (uint32)(flags);
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 23)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            actor_index_objects = 0;
            pose_owner_object = mission_actor(actor_index_objects);
            value = pose_owner_object->pose.position[0];
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 155647999)
            {
                value = pose_owner_object->pose.position[2];
                if ((sint32)value < 0)
                    value = 0 - value;
                if ((sint32)value <= 155647999)
                {
                    value = pose_owner_object->pose.position[1];
                    if ((sint32)value > -62914560)
                        game_runtime.metrics.flags = (uint32)(flags | 4);
                }
            }
        }
    }
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 55)
    {
        value = game_runtime.selection.difficulty;
        actor_index_objects = 0;
        pose_owner_object = mission_actor(actor_index_objects);
        coordinate = pose_owner_object->pose.position[2];
        if (value == 0)
        {
            if ((sint32)coordinate > 299008000)
            {
                script = mission_event_script(MISSION_EVENT_SIGNAL, 1);
                mission_script_start_legacy(script);
            }
        }
        else if ((sint32)coordinate > 876544000)
        {
            script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
            mission_script_start_legacy(script);
        }
    }
final_timer:
    if (mode == 2048)
    {
        flags = game_runtime.flags;
        if ((flags & 0x20000000) == 0)
        {
            value = game_runtime.metrics.timer_mode;
            if (value == 0)
            {
                other = game_runtime.metrics.elapsed_ticks;
                game_runtime.metrics.display_ticks = (uint32)(other);
            }
            else if (value == 1)
            {
                other = game_runtime.metrics.display_ticks;
                game_runtime.metrics.display_ticks = (uint32)((sint32)other > 0 ? other - 1 : 0);
            }
            else
                game_runtime.metrics.display_ticks = (uint32)(0);
        }
    }
    result = game_runtime.mission_control.mode & 0x102C0E;
    if (result != 0)
    {
        (void)(speech_queue_update());
        return;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_metrics_begin_phase(void)
{
    uint32 index, value, offset = 0;
    FUNCTION_MARKER(0x80053DA0, "ACE3.BIN");
    game_runtime.metrics.phase_ticks = 0;
    index = game_runtime.mission_control.argument;
    if (index < 4)
        value = game_runtime.world_settings.phase_timer_mode[index];
    else
    {
        offset = index + 52;
        if (!world_phase_input.data || offset > world_phase_input.bytes || 1 > world_phase_input.bytes - offset)
            abort();
        value = world_phase_input.data[offset];
    }
    game_runtime.metrics.timer_mode = (uint8)value;
    if (index >= 4)
        value = world_phase_input.data[offset];
    if (value == 1)
    {
        if (index < 3 || (index == 3 && game_runtime.world_settings.has_fourth_initial_timer))
            game_runtime.metrics.display_ticks = game_runtime.world_settings.phase_initial_ticks[index];
        else
        {
            offset = (index << 2) + 56;
            if (!world_phase_input.data || offset > world_phase_input.bytes || sizeof(value) > world_phase_input.bytes - offset)
                abort();
            memcpy(&value, world_phase_input.data + offset, sizeof(value));
            game_runtime.metrics.display_ticks = value;
        }
    }
}

void mission_scenario_intro_update(void)
{
    uint32 mask, timer, value, first, second, scenario, result;
    FUNCTION_MARKER(0x8005BC64, "ACE3.BIN");
    value = mission_flow.request.dispatch_count;
    mask = mission_flow.scenario_mask;
    if (value == 1 && (mask & 0x6000) != 0)
    {
        timer = game_runtime.mission_control.ticks;
        if ((sint32)timer < 16)
        {
            first = game_runtime.mission_control.ot;
            second = game_runtime.mission_control.packet_cur;
            value = effect_draw_noise_overlay(first + 8, second, 4096 - (timer << 8));
            game_runtime.mission_control.packet_cur = (uint32)(value);
        }
    }
    timer = game_runtime.mission_control.ticks;
    if ((sint32)timer >= 61 && speech_queue_mode() == 0)
    {
        if ((mask & 0x8000) != 0)
            flight_dynamics_enter_transition_control();
        else
            flight_dynamics_enter_selected_mode();
        if ((mask & 0x300) != 0)
            scenario = 11;
        else if ((mask & 0x400) != 0)
            scenario = 19;
        else if ((mask & 0x800) != 0)
            scenario = 29;
        else if ((mask & 0x2000) != 0)
            scenario = 15;
        else if ((mask & 0x4000) != 0)
            scenario = 27;
        else if ((mask & 0x1000) != 0)
            scenario = 39;
        else if ((mask & 0x8000) != 0)
            scenario = 45;
        mission_status_text_show(scenario);
        value = game_runtime.mission_control.argument;
        mission_request_mode(0x800, value);
    }
    result = flight_dynamics_flags() & 0x7000;
    if (result != 0)
    {
        speech_queue_stop();
        value = mission_flow.request.saved_scene_arg;
        mission_flow.request.scene_arg = (uint8)(value);
        mission_request_mode(0x2000, 1);
        result = game_runtime.flags | 0x10000000;
        game_runtime.flags = result;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_result_scene_stop_hook(void)
{
    FUNCTION_MARKER(0x800304EC, "ACE3.BIN");
    return;
}
