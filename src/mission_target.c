#include <stdlib.h>

#include "game_functions.h"

static uint32 mission_target_update_held(void);
static uint32 mission_target_find_nearest_eligible(void);
static uint32 mission_target_locked_index(void);
static void mission_target_update_incoming_warning(void);

MISSION_TARGET_STATE mission_target;

void mission_target_update(void)
{
    uint32 selected, flags, value, mode, variant;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003AA60, "ACE3.BIN");
    selected = hud_control.selected_target_idx;
    if (selected != 255)
    {
        flags = (*mission_actor_flags(selected));
        if ((flags & 1) == 0)
            value = 1;
        else
        {
            target = mission_actor_target(selected);
            value = (((target->flags) & 1) == 0) || ((flags & 0x40000) != 0);
        }
        if (value)
            hud_control.selected_target_idx = (uint8)(mission_target_links_release(0));
    }
    mode = game_runtime.mission_control.mode;
    if (mode & 4)
    {
        if ((sint16)game_runtime.selection.mission == 3 && game_runtime.mission_control.argument == 0)
        {
            value = hud_control.selected_target_idx;
            if (value == 2)
                return;
            hud_control.selected_target_idx = (uint8)(2);
            mission_target_links_release(0);
            {
                (void)(uint32)(mission_target_links_assign(0, 2));
                return;
            }
        }
        mission_target_cycle();
        {
            (void)(mission_target_acquire());
            return;
        }
    }
    if (mode & 0x800)
    {
        variant = mission_flow.scenario_idx;
        selected = mission_scenarios[variant].fixed_target_idx;
        value = hud_control.selected_target_idx;
        if (selected == value)
        {
            (void)(uint32)(value);
            return;
        }
        hud_control.selected_target_idx = (uint8)(selected);
        mission_target_links_release(0);
        {
            (void)(uint32)(mission_target_links_assign(0, selected));
            return;
        }
    }
    hud_control.selected_target_idx = (uint8)(255);
    mission_target_links_release(0);
    {
        (void)(uint32)(mission_target_links_assign(0, 255));
        return;
    }
}

void mission_target_cycle(void)
{
    uint32 flags, direction, selected, current;
    FUNCTION_MARKER(0x8003ABEC, "ACE3.BIN");
    flags = game_input.pressed;
    direction = 0;
    if ((flags & 0x200000) == 0)
    {
        if ((flags & 0x400000) == 0)
            return;
        direction = 1;
    }
    current = hud_control.selected_target_idx;
    selected = mission_target_find(current, direction, 0);
    if (selected == 255)
        return;
    current = hud_control.selected_target_idx;
    if (selected != current)
    {
        hud_control.selected_target_idx = (uint8)(selected);
        mission_target_links_release(0);
        mission_target_links_assign(0, selected);
    }
    return;
}

void mission_target_acquire(void)
{
    uint32 current, value, selected;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003AC8C, "ACE3.BIN");
    current = hud_control.selected_target_idx;
    if (current != 255)
    {
        current = hud_control.selected_target_idx;
        target = mission_actor_target(current);
        if (((target->flags) & 2) || ((target->projection_flags) & 0x20000))
        {
            mission_target.retarget_ticks = 0;
            return;
        }
        value = mission_target.retarget_ticks;
        mission_target.retarget_ticks = value + 1;
    }
    selected = mission_target_find(255, 0, 1);
    if (selected == 255)
        return;
    current = hud_control.selected_target_idx;
    if (selected == current)
    {
        (void)(uint32)(current);
        return;
    }
    if (current != 255)
    {
        if ((sint32)mission_target.retarget_ticks < 31)
            return;
        target = mission_actor_target(selected);
        if (((sint16)target->cone_ticks) < 31)
            return;
    }
    hud_control.selected_target_idx = (uint8)(selected);
    mission_target_links_release(0);
    value = mission_target_links_assign(0, selected);
    mission_target.retarget_ticks = 0;
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 mission_target_score(uint32 index)
{
    uint32 flags, distance;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003B2E4, "ACE3.BIN");
    target = mission_actor_target(index);
    if (target == 0)
        return index;
    flags = (target->flags);
    if ((flags & 1) == 0)
        return index;
    if (flags & 2)
        index += 0x400000;
    distance = (target->distance);
    return index + ((0x8000 - distance) << 6);
}

uint32 mission_target_find(uint32 current, uint32 direction, uint32 automatic)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 scores[64], current_score, preferred, lower, upper, best, nearest, index, flags, state, score;
    uint32 maximum, minimum, below, above;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003AFC0, "ACE3.BIN");
    current_score = 0;
    preferred = 0;
    lower = 255;
    upper = 255;
    best = 255;
    nearest = 255;
    maximum = 0;
    below = 0;
    minimum = 0x800000;
    above = 0x800000;
    if ((current & 0xFF) != 255)
    {
        current_score = mission_target_score(current & 0xFF);
        preferred = (sint32)current_score > 0x3FFFFF;
    }
    for (index = 2; index < 64; ++index)
    {
        scores[index] = 0xFFFFFFFF;
        if (index == (current & 0xFF))
            continue;
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags;
        if ((flags & 0x4000) == 0)
            continue;
        state = (*mission_actor_flags(index));
        if ((state & 1) == 0 || (state & 0x8000) == 0 || (flags & 0x40))
            continue;
        if ((state & 0x40000) && (state & 0x100) == 0)
            continue;
        if (automatic == 1 && (flags & 0x100))
            continue;
        target = mission_actor_target(index);
        if (((target->flags) & 1) == 0)
            continue;
        score = mission_target_score(index);
        scores[index] = score;
        if (!preferred && (sint32)score > 0x3FFFFF)
        {
            current = 255;
            preferred = 1;
        }
    }
    for (index = 2; index < 64; ++index)
    {
        score = scores[index];
        if (score == 0xFFFFFFFF || (preferred == 1 && (sint32)score <= 0x3FFFFF))
            continue;
        if ((sint32)maximum < (sint32)score)
        {
            maximum = score;
            best = index;
        }
        if ((current & 0xFF) == 255)
            continue;
        if ((sint32)score < (sint32)minimum)
        {
            minimum = score;
            nearest = index;
        }
        if ((sint32)current_score < (sint32)score && (sint32)score < (sint32)above)
        {
            above = score;
            upper = index;
        }
        else if ((sint32)below < (sint32)score && (sint32)score < (sint32)current_score)
        {
            below = score;
            lower = index;
        }
    }
    if ((current & 0xFF) == 255)
        lower = best;
    if ((lower & 0xFF) == 255)
        lower = best;
    if ((upper & 0xFF) == 255)
        upper = nearest;
    if ((lower & 0xFF) == 255)
        lower = current;
    if ((upper & 0xFF) == 255)
        upper = current;
    return (direction ? upper : lower) & 0xFF;
}

static void transform_direction(MATRIX *matrix, GAME_FIXED_VECTOR *output, const uint32 *native_origin, const uint32 *native_vector)
{
    uint32 input[3], high[3], low[3], result[3], i, value, base, magnitude;
    FUNCTION_MARKER(0x80045024, "ACE3.BIN");
    value = (((native_vector[0])));
    base = (((native_origin[0])));
    input[0] = ((sint32)(value - base) >> 12);
    value = (((native_vector[1])));
    base = (((native_origin[1])));
    input[1] = ((sint32)(value - base) >> 12);
    base = (((native_origin[2])));
    value = (((native_vector[2])));
    input[2] = ((sint32)(value - base) >> 12);
    SetRotMatrix(matrix);
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = ((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    for (i = 0; i < 3; ++i)
        output->component[i] = (sint32)result[i];
}

void mission_target_update_geometry(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_player;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags, distance, value, angle, threshold, cached;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80045278, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_player = mission_actor(actor_index_table);
    game_build_rotation_via_gte(&mission_target.direction_matrix, pose_owner_player->pose.angles);
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags;
        target = mission_actor_target(index);
        flags &= 0xFFFFDFFF;
        actor_traits_object->flags = (uint32)(flags);
        if ((flags & 0x4000) == 0)
            continue;
        if ((*mission_actor_flags(index)) & 0x400000)
            continue;
        distance = (target->distance);
        target->flags = (uint32)(0);
        if (distance == 0x100000)
        {
            value = game_runtime.mission_control.ticks;
            if (index != (value & 0x3F))
                continue;
        }
        else if ((sint32)distance > 34000)
        {
            value = game_runtime.mission_control.ticks;
            if ((index & 15) != (value & 15))
                continue;
        }
        distance = game_measure_distance_gte(pose_owner_object->pose.position, pose_owner_player->pose.position);
        target->distance = (uint32)(distance);
        if ((index < 32 && (sint32)distance <= 33999) || (index >= 32 && (sint32)distance < 0x3000))
        {
            flags = (target->flags);
            target->flags = (uint32)(flags | 0x80);
        }
        if (((sint32)target->distance) > 0x7FFF)
            continue;
        flags = (target->flags);
        target->flags = (uint32)(flags | 1);
        flags = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(flags | 0x2000);
        if (((sint32)target->distance) > 0x7FFF)
            continue;
        flags = (target->flags);
        target->flags = (uint32)(flags | 0x1000000);
        transform_direction(&mission_target.direction_matrix, &target->direction, pose_owner_player->pose.position, pose_owner_object->pose.position);
        angle = game_vector_axis_angle(target->direction.word);
        target->angle = (uint16)(angle);
        if ((sint16)angle < 0x600)
        {
            flags = (target->flags);
            target->flags = (uint32)(flags | 2);
        }
        if (((sint32)target->distance) < 0x3000 && ((sint16)target->angle) < 0x600)
        {
            flags = (target->flags);
            target->flags = (uint32)(flags | 0x40);
        }
        if (((sint32)target->distance) < 0x2000 && ((sint16)target->angle) < 0x600)
        {
            flags = (target->flags);
            target->flags = (uint32)(flags | 4);
        }
        distance = (target->distance);
        threshold = mission_target.distance_limit;
        if ((sint32)distance < (sint32)threshold)
        {
            cached = (target->flags);
            target->flags = (uint32)(cached | 0x100);
            angle = ((sint16)target->angle);
            threshold = mission_target.angle_limit;
            if ((sint32)angle < (sint32)threshold)
                target->flags = (uint32)(cached | 0x108);
            if (((sint16)target->angle) < 0x600)
            {
                flags = (target->flags);
                target->flags = (uint32)(flags | 0x10);
            }
        }
        distance = (target->distance);
        threshold = hud_control.aim_distance_parameter;
        if ((sint32)distance < (sint32)threshold)
        {
            flags = (target->flags);
            if (flags & 2)
                target->flags = (uint32)(flags | 0x20);
        }
    }
    return;
}

static uint32 mission_target_update_held(void)
{
    uint32 selected, value;
    FUNCTION_MARKER(0x8003A444, "ACE3.BIN");
    selected = hud_control.held_target_idx;
    if (selected == 255)
    {
        value = hud_control.selected_target_idx;
        if (value != selected)
            selected = value;
        else if ((game_input.pressed & 0x2000) != 0)
            selected = mission_target_find_nearest_eligible();
    }
    if ((game_input.held & 0x2000) == 0)
        selected = hud_control.selected_target_idx;
    hud_control.held_target_idx = selected;
    return selected == 255 ? 0xFFFFFFFF : selected;
}

static uint32 mission_target_locked_index(void)
{
    uint32 selected;
    FUNCTION_MARKER(0x8003A598, "ACE3.BIN");
    selected = hud_control.locked_target_idx;
    return selected == 255 ? 0xFFFFFFFF : selected;
}

uint32 mission_target_uses_alternate_weapon(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 selected;
    FUNCTION_MARKER(0x8003A5C0, "ACE3.BIN");
    selected = hud_control.selected_target_idx;
    if (selected == 255)
        return 0;
    actor_traits_object = mission_actor_traits(((uint32)((sint32)((selected << 2)) >> 2)));
    if ((sint16)actor_traits_object->kind == 81)
        return 0;
    return ((*mission_actor_flags(selected)) & 0x400) == 0;
}

void mission_target_prepare_frame(MISSION_TARGET_FRAME *record)
{
    uint32 value, result;
    FUNCTION_MARKER(0x8003A23C, "ACE3.BIN");
    mission_target_update();
    mission_target_update_incoming_warning();
    value = mission_target_update_held();
    record->camera_target_index = value;
    value = mission_target_locked_index();
    record->locked_target_index = value;
    value = record->angle_limit;
    mission_target.angle_limit = value;
    result = mission_target_uses_alternate_weapon();
    if (result == 0)
        value = record->distance_limit[0];
    else
        value = record->distance_limit[1];
    mission_target.distance_limit = value;
    {
        (void)(uint32)(value);
        return;
    }
}

static uint32 mission_target_find_nearest_eligible(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, value, best = 0x1000000, selected = 255;
    MISSION_ACTOR_TARGET *state;
    FUNCTION_MARKER(0x8003A4E8, "ACE3.BIN");
    actor_index_table = 0;
    for (index = 2; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        if (((*mission_actor_flags(index)) & 0x800000) == 0)
            continue;
        state = mission_actor_target(index);
        if (state == 0)
            continue;
        value = (state->distance);
        if ((sint32)value < (sint32)best)
        {
            best = value;
            selected = index;
        }
    }
    return selected;
}

static void mission_target_update_incoming_warning(void)
{
    MISSION_ACTOR *pose_owner_player;
    uint32 actor_index_data;

    EFFECT_OBJECT *record;
    uint32 value, flags, byte, voice, distance, vector[3], divisor;
    FUNCTION_MARKER(0x8003ADB4, "ACE3.BIN");
    value = game_runtime.metrics.display_ticks;
    hud_control.display_ticks = (uint32)(value);
    flags = mission_target.warning_flags;
    byte = game_runtime.metrics.timer_mode;
    mission_target.warning_flags = flags & 0xFFFFFFFD;
    hud_control.timer_mode = byte;
    flags = game_runtime.mission_control.mode;
    value = (~flags) & 4;
    if (value)
    {
        (void)(uint32)(value);
        return;
    }
    if (game_runtime.mission_control.ticks == 0)
        audio_aux_init(&mission_target.aux, 3, 3, 0x400, 16);
    if ((effect_actor_tracking.target_mask[0] & 1) == 0)
        return;
    value = effect_actor_tracking.last_effect[0];
    if (value >= EFFECT_OBJECT_CAPACITY)
        abort();
    record = effect_objects.slot + value;
    actor_index_data = 0;
    pose_owner_player = mission_actor(actor_index_data);
    audio_spatial_update(&mission_target.spatial, record->pose.position, game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles);
    audio_aux_update(&mission_target.aux, &mission_target.spatial);
    voice = mission_target.aux.voice;
    if (voice != 255)
    {
        sound_restore_voice_defaults(voice);
        audio_spatial_pan(mission_target.aux.voice, &mission_target.spatial);
    }
    distance = game_measure_distance_gte(record->pose.position, pose_owner_player->pose.position);
    if ((sint32)distance >= 0x2000)
        return;
    game_pose_relative_direction(pose_owner_player->pose.position, vector, record->pose.position, record->pose.angles);
    value = vector[2];
    if ((sint32)value < 0)
    {
        (void)(uint32)(value);
        return;
    }
    value = game_vector_axis_angle(vector);
    if ((sint32)value >= 0x600 && (sint32)distance >= 0x800)
        return;
    flags = mission_target.warning_flags;
    divisor = ((sint32)distance >> 9);
    mission_target.warning_flags = flags | 2;
    if ((sint32)divisor < 2)
        divisor = 2;
    value = hud_control.display_ticks;
    if ((sint32)value % (sint32)divisor)
    {
        (void)(uint32)(value);
        return;
    }
    {
        (void)(uint32)(sound_play_reuse(44));
        return;
    }
}
