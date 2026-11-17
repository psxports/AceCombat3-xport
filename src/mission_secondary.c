#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

static const uint32 secondary_projectile_velocity[3] = {0x00000000u, 0x00000000u, 0x00000000u};

typedef struct
{
    MISSION_SECONDARY_BINDING binding;
    uint8 motion_phase;
    uint8 anim_frame;
    uint8 weapon_phase;
    uint8 target_index;
    uint32 motion_ticks;
    uint32 anim_ticks;
    uint32 weapon_ticks;
    uint32 anim_step;
    NATIVE_POSE launch_pose;
    uint32 target_pos[3];
} MISSION_SECONDARY_ACTOR;

static MISSION_SECONDARY_ACTOR mission_secondary[32];

typedef struct
{
    uint32 signal_flags;
    uint32 proximity_strength;
    uint16 charge[3];
    uint16 nearest_point;
} MISSION_SECONDARY_CONTROL;

static MISSION_SECONDARY_CONTROL secondary_control;

static const uint32 secondary_points[3][3] = {
    {0x09C00000, 0xFFB00000, 0xFE400000},
    {0x0DC00000, 0xFFB00000, 0xE6400000},
    {0xF9C00000, 0xFFB00000, 0xF2400000},
};

enum
{
    SECONDARY_MOTION_BYTES = 48
};

static struct
{
    MISSION_SECONDARY_MOTION_PROFILE records[256];
    uint32 count;
} secondary_motion_catalog;

static MISSION_SECONDARY_MOTION_PROFILE mission_secondary_decode_motion(const uint8 *record)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    MISSION_SECONDARY_MOTION_PROFILE profile;
    uint32 point, axis;

    profile.kind = record[0];
    profile.phase = (memcpy(&wire_halfword_0, record + 2, sizeof(wire_halfword_0)), wire_halfword_0);
    profile.vertical_velocity = (memcpy(&wire_word_1, record + 4, sizeof(wire_word_1)), wire_word_1);
    profile.magnitude = (memcpy(&wire_word_2, record + 8, sizeof(wire_word_2)), wire_word_2);
    for (point = 0; point < 3; ++point)
        for (axis = 0; axis < 3; ++axis)
            profile.waypoint[point].word[axis] = (memcpy(&wire_word_3, record + 12 + point * 12 + axis * 4, sizeof(wire_word_3)), wire_word_3);
    return profile;
}

void mission_secondary_bind_motion_catalog(const uint8 *records, uint32 bytes)
{
    uint32 count = bytes / SECONDARY_MOTION_BYTES;
    uint32 index;

    secondary_motion_catalog.count = 0;
    if (!records || bytes % SECONDARY_MOTION_BYTES || count > sizeof(secondary_motion_catalog.records) / sizeof(secondary_motion_catalog.records[0]))
        return;
    secondary_motion_catalog.count = count;
    for (index = 0; index < count; ++index)
        secondary_motion_catalog.records[index] = mission_secondary_decode_motion(records + index * SECONDARY_MOTION_BYTES);
}

void mission_secondary_select_motion(uint32 actor, uint32 selector)
{
    MISSION_SECONDARY_MOTION_PROFILE *profile = &mission_secondary_binding(actor)->motion;

    if (selector >= secondary_motion_catalog.count)
        abort();
    *profile = secondary_motion_catalog.records[selector];
}

void mission_secondary_reset(void)
{
    memset(mission_secondary, 0, sizeof(mission_secondary));
    memset(&secondary_motion_catalog, 0, sizeof(secondary_motion_catalog));
    memset(&secondary_control, 0, sizeof(secondary_control));
}

static MISSION_SECONDARY_ACTOR *mission_secondary_get(uint32 index)
{
    if (index >= 32)
        abort();
    return &mission_secondary[index];
}

MISSION_SECONDARY_BINDING *mission_secondary_binding(uint32 index)
{
    return &mission_secondary_get(index)->binding;
}

void mission_secondary_initialize(void)
{
    MISSION_ACTOR *pose_owner_object;
    const MISSION_ACTOR_DEFINITION *definition_attributes;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_BINDING *record;
    const MISSION_SECONDARY_PARAMETERS *parameters;
    MISSION_SECONDARY_ACTOR *secondary;
    uint32 actor_index_first_table;
    uint32 actor_index_table;

    uint32 index, kind, value;
    uint32 w0, w1, w2, count;
    FUNCTION_MARKER(0x8004E3A8, "ACE3.BIN");
    actor_index_table = 32;
    for (index = 0; index < 32; ++index)
    {
        actor_motion_object = mission_actor_motion(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        actor_index_table += 1;
        actor_traits_object->flags = (uint32)(index + 32);
        *mission_actor_flags(index + 32) = (uint32)(0);
    }
    count = game_runtime.actor_resources.secondary_count;
    for (index = 0; index < count; ++index)
    {
        actor_index_table = 32;

        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        secondary = mission_secondary_get(index);
        record = mission_secondary_binding(index);
        parameters = &record->parameters;
        value = actor_traits_object->flags;
        definition_attributes = &record->definition;
        actor_traits_object->flags = (uint32)(value | 0x4000);
        value = arena_alloc(12);
        const RESOURCE_ASSET_SOURCE *marker_owner = &game_runtime.resource_context.aircraft_backing;
        const MISSION_MARKER_SEED *marker_seed = NULL;
        if (value)
        {
            uint32 marker_offset = value - marker_owner->encoded_base;
            if (!marker_owner->data || value < marker_owner->encoded_base || marker_offset > marker_owner->bytes || 6 > marker_owner->bytes - marker_offset)
                abort();
            marker_seed = (const MISSION_MARKER_SEED *)(marker_owner->data + marker_offset + 2);
        }
        mission_actor_bind_marker(index + 32, marker_seed);
        value = arena_alloc(40);
        mission_actor_bind_target(index + 32, value != 0);
        mission_actor_initialize_target_state(index + 32);
        kind = parameters->category;
        if (kind < 3)
            actor_traits_object->flags = (uint32)(actor_traits_object->flags | (0x40 << kind));
        actor_traits_object->events = (uint32)(0);
        actor_traits_object->render_flags = (uint8)(0);
        w0 = parameters->pose.position[0];
        w1 = parameters->pose.position[1];
        w2 = parameters->pose.position[2];
        pose_owner_object->pose.position[0] = (uint32)(w0);
        pose_owner_object->pose.position[1] = (uint32)(w1);
        pose_owner_object->pose.position[2] = (uint32)(w2);
        w0 = (parameters->pose.angles[0] | ((uint32)parameters->pose.angles[1] << 16));
        w1 = (parameters->pose.angles[2]);
        {
            uint32 pose_word = (w0);
            pose_owner_object->pose.angles[0] = (uint16)pose_word;
            pose_owner_object->pose.angles[1] = (uint16)(pose_word >> 16);
        }
        {
            uint32 pose_word = (w1);
            pose_owner_object->pose.angles[2] = (uint16)pose_word;
        }
        value = parameters->model;
        actor_traits_object->model = (uint8)(value == 255 ? 0 : value);
        value = parameters->render_mode;
        if (value == 1)
            actor_traits_object->render_flags = (uint8)(actor_traits_object->render_flags | 2);
        actor_traits_object->parameter = (uint16)(0);
        value = parameters->kind;
        actor_index_first_table = 0;
        actor_traits_object->kind = (uint16)(value);
        value = definition_attributes->health;
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        actor_motion_object->angular_delta[0] = (uint16)(0);
        actor_motion_object->angular_delta[1] = (uint16)(0);
        actor_motion_object->angular_delta[2] = (uint16)(0);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_traits_object->events = (uint32)(0x80000000);
        actor_traits_object->health = (uint16)(value);
        value = game_measure_distance_gte(pose_owner_object->pose.position, mission_actor(actor_index_first_table)->pose.position);
        mission_actor_target(index + 32)->distance = value;
        secondary->motion_phase = (uint8)(0);
        secondary->anim_frame = (uint8)(0);
        secondary->weapon_phase = (uint8)(0);
        secondary->target_index = (uint8)(255);
        count = game_runtime.actor_resources.secondary_count;
        secondary->motion_ticks = (uint32)(0);
        secondary->anim_ticks = (uint32)(0);
        secondary->weapon_ticks = (uint32)(0);
        secondary->anim_step = (uint32)(1);
    }
    return;
}

static MISSION_SECONDARY_ANIMATION secondary_animation_decode(const uint8 *wire)
{
    uint16 wire_halfword_0;
    MISSION_SECONDARY_ANIMATION animation = {0};
    uint32 i;

    animation.has_descriptor = wire != NULL;
    if (!wire)
        return animation;
    animation.effect = wire[0];
    animation.start_mode = wire[1];
    animation.end_mode = wire[2];
    animation.frame_count = wire[3];
    for (i = 0; i < SECONDARY_ANIMATION_DURATION_COUNT; ++i)
        animation.duration[i] = (sint16)(memcpy(&wire_halfword_0, wire + 4 + i * 2, sizeof(wire_halfword_0)), wire_halfword_0);
    animation.replacement_model = wire[14];
    animation.entity_event = wire[15];
    return animation;
}

static sint32 secondary_animation_duration(const MISSION_SECONDARY_ANIMATION *animation, uint32 frame)
{
    if (animation->has_descriptor)
    {
        if (frame < SECONDARY_ANIMATION_DURATION_COUNT)
            return animation->duration[frame];
        if (frame == SECONDARY_ANIMATION_DURATION_COUNT)
            return (sint16)(animation->replacement_model | ((uint16)animation->entity_event << 8));
    }
    abort();
}

void mission_secondary_animate(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_ACTOR *secondary;
    uint32 actor_index_table;

    const MISSION_SECONDARY_ANIMATION *animation;
    uint32 member, flags, frame, elapsed, divisor, numerator, quotient, increment, next_frame, limit, count, kind, value;
    uint32 *slot;
    FUNCTION_MARKER(0x80051BA0, "ACE3.BIN");
    actor_index_table = 32;
    animation = &mission_secondary_binding(index)->animation;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    secondary = mission_secondary_get(index);
    member = index + 32;
    if (!animation->has_descriptor)
        return;
    flags = actor_traits_object->render_flags;
    frame = (sint8)secondary->anim_frame;
    elapsed = secondary->anim_ticks;
    actor_traits_object->render_flags = (uint8)(flags | 4);
    divisor = secondary_animation_duration(animation, frame);
    numerator = elapsed << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    elapsed = secondary->anim_ticks;
    increment = secondary->anim_step;
    elapsed += increment;
    next_frame = (sint8)secondary->anim_frame;
    secondary->anim_ticks = (uint32)(elapsed);
    actor_traits_object->parameter = (uint16)(quotient + (frame << 12));
    limit = secondary_animation_duration(animation, next_frame);
    frame = secondary->anim_frame;
    if ((sint32)limit < (sint32)elapsed)
    {
        secondary->anim_ticks = (uint32)(elapsed - limit);
        secondary->anim_frame = (uint8)(frame + 1);
    }
    else if ((sint32)elapsed < 0)
    {
        next_frame = frame - 1;
        secondary->anim_frame = (uint8)(next_frame);
        next_frame = (sint32)(next_frame << 24) >> 24;
        if ((sint32)next_frame >= 0)
        {
            value = secondary_animation_duration(animation, next_frame);
            secondary->anim_ticks = (uint32)(value);
        }
    }
    count = animation->frame_count;
    frame = (sint8)secondary->anim_frame;
    if (frame == count - 1)
    {
        kind = animation->end_mode;
        if (kind == 1 || kind == 2)
        {
            slot = mission_actor_flags(member);
            flags = (*slot);
            *slot = (flags & 0xFFFDFFFF);
            if (kind == 2)
                mission_secondary_begin_deactivation(index);
        }
        else if (kind == 3)
        {
            secondary->anim_frame = (uint8)(0);
            secondary->anim_ticks = (uint32)(0);
        }
        else
        {
            next_frame = count - 2;
            secondary->anim_frame = (uint8)(next_frame);
            next_frame = (sint32)(next_frame << 24) >> 24;
            value = secondary_animation_duration(animation, next_frame);
            secondary->anim_step = (uint32)(0xFFFFFFFF);
            secondary->anim_ticks = (uint32)(value);
        }
    }
    frame = (sint8)secondary->anim_frame;
    if ((sint32)frame >= 0)
    {
        (void)(uint32)(frame);
        return;
    }
    kind = animation->end_mode;
    if (kind == 4)
    {
        slot = mission_actor_flags(member);
        flags = (*slot);
        *slot = (flags & 0xFFFDFFFF);
        kind = animation->end_mode;
    }
    if (kind == 5)
    {
        slot = mission_actor_flags(member);
        flags = (*slot);
        *slot = (flags & 0xFFFDFFFF);
        {
            (void)(mission_secondary_begin_deactivation(index));
            return;
        }
    }
    secondary->anim_frame = (uint8)(0);
    secondary->anim_ticks = (uint32)(0);
    secondary->anim_step = (uint32)(1);
    return;
}

void mission_secondary_stop_idle(uint32 index)
{
    MISSION_SECONDARY_ACTOR *secondary;
    const MISSION_SECONDARY_MOTION_PROFILE *descriptor;
    uint32 actor_index_table;

    uint32 value;
    FUNCTION_MARKER(0x80052060, "ACE3.BIN");
    actor_index_table = 32;
    descriptor = &mission_secondary_binding(index)->motion;
    value = descriptor->kind;
    (void)mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    secondary = mission_secondary_get(index);
    if (value != 6)
    {
        (void)(uint32)(value);
        return;
    }
    value = (sint8)secondary->motion_phase;
    if (value != 0)
    {
        (void)(uint32)(value);
        return;
    }
    {
        (void)(mission_secondary_begin_deactivation(index));
        return;
    }
}

void mission_secondary_move(uint32 index)
{
    MISSION_ACTOR *pose_owner_a;
    MISSION_ACTOR *pose_owner_b;
    MISSION_ACTOR *pose_owner_c;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_pointer;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_ACTOR *secondary;
    const MISSION_SECONDARY_PARAMETERS *parameters;
    MISSION_SECONDARY_MOTION_PROFILE selected_motion;
    const MISSION_SECONDARY_MOTION_PROFILE *data;
    const uint32 *waypoint;
    uint32 actor_index_objects;
    uint32 actor_index_pointer;

    uint32 first[3], target[3], direction[3];
    uint32 flags, mode, phase, value, a, b, c, mission, distance, speed, result, scan, nearest, selected, timer, strength;
    uint32 *actor_flags;
    FUNCTION_MARKER(0x800522B0, "ACE3.BIN");
    actor_index_objects = 32;
    flags = (*mission_actor_flags(index + 32));
    pose_owner_object = mission_actor((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
    secondary = mission_secondary_get(index);
    /* keep this selection across scripts that retarget the binding */
    selected_motion = mission_secondary_binding(index)->motion;
    data = &selected_motion;
    if ((flags & 0x400000) != 0)
        return;
    mode = data->kind;
    switch (mode)
    {
        case 0:
            actor_motion_object->velocity[0] = (uint32)(0);
            actor_motion_object->velocity[1] = (uint32)(0);
            actor_motion_object->velocity[2] = (uint32)(0);
            return;
        case 1:
            a = data->phase;
            b = game_wave_at((a & 0x3FFF) * 2);
            c = data->magnitude;
            actor_motion_object->velocity[1] = (uint32)(0);
            value = ((sint32)(c * b) >> 14);
            actor_motion_object->velocity[0] = (uint32)(value);
            a = data->phase;
            b = game_wave_at(((a & 0x3FFF) + 0x1000) * 2);
            c = data->magnitude;
            result = ((sint32)(b * c) >> 14);
            actor_motion_object->velocity[2] = (uint32)(result);
            {
                (void)(uint32)(result);
                return;
            }
        case 2:
            actor_motion_object->velocity[0] = (uint32)(0);
            result = data->vertical_velocity;
            actor_motion_object->velocity[2] = (uint32)(0);
            actor_motion_object->velocity[1] = (uint32)(result);
            {
                (void)(uint32)(result);
                return;
            }
        case 3:
            phase = (sint8)secondary->motion_phase;
            value = actor_motion_object->velocity[1];
            value += phase == 0 || phase == 3 ? 0xFFFFFF00 : 256;
            actor_motion_object->velocity[1] = (uint32)(value);
            result = secondary->motion_ticks + 1;
            secondary->motion_ticks = (uint32)(result);
            if (result == 64)
            {
                result = secondary->motion_phase + 1;
                secondary->motion_phase = (uint8)(result);
                result = (sint8)result;
                if (result == 4)
                    secondary->motion_phase = (uint8)(0);
                secondary->motion_ticks = (uint32)(0);
            }
            actor_motion_object->velocity[0] = (uint32)(0);
            actor_motion_object->velocity[2] = (uint32)(0);
            {
                (void)(uint32)(result);
                return;
            }
        case 4:
            phase = (sint8)secondary->motion_phase;
            if (phase == 0 || phase == 1 || phase == 2)
            {
                first[0] = pose_owner_object->pose.position[0];
                first[1] = pose_owner_object->pose.position[1];
                first[2] = pose_owner_object->pose.position[2];
            }
            else
            {
                mission = (sint16)game_runtime.selection.mission;
                if (mission == 43)
                {
                    first[0] = data->waypoint[1].word[0];
                    first[1] = data->waypoint[1].word[1];
                    first[2] = data->waypoint[1].word[2];
                }
                else
                {
                    first[0] = pose_owner_object->pose.position[0];
                    first[1] = pose_owner_object->pose.position[1];
                    first[2] = pose_owner_object->pose.position[2];
                }
            }
            waypoint = data->waypoint[phase == 0 ? 0 : phase == 1 ? 1 : 2].word;
            target[0] = waypoint[0];
            target[1] = waypoint[1];
            target[2] = waypoint[2];
            distance = game_measure_direction_14(first, target, direction);
            phase = (sint8)secondary->motion_phase;
            a = secondary->motion_phase;
            if ((sint32)phase < 3)
            {
                speed = data->magnitude;
                if ((sint32)distance < (sint32)speed)
                    secondary->motion_phase = (uint8)(a + 1);
            }
            mission = (sint16)game_runtime.selection.mission;
            if (mission == 43 && (sint8)secondary->motion_phase == 3 && distance == 0)
            {
                flags = actor_traits_object->flags;
                if ((flags & 0x40) != 0 && (secondary_control.signal_flags & 0x40) == 0)
                {
                    mission_script_start_legacy(mission_event_script(MISSION_EVENT_SIGNAL, 0));
                    value = secondary_control.signal_flags;
                    secondary_control.signal_flags = (uint32)(value | 0x40);
                }
                else
                {
                    flags = actor_traits_object->flags;
                    if ((flags & 0x80) != 0 && (secondary_control.signal_flags & 0x80) == 0)
                    {
                        mission_script_start_legacy(mission_event_script(MISSION_EVENT_SIGNAL, 1));
                        value = secondary_control.signal_flags;
                        secondary_control.signal_flags = (uint32)(value | 0x80);
                    }
                }
            }
            speed = data->magnitude;
            actor_motion_object->velocity[0] = (uint32)((uint32)((sint32)(speed * direction[0]) >> 14));
            speed = data->magnitude;
            actor_motion_object->velocity[1] = (uint32)((uint32)((sint32)(speed * direction[1]) >> 14));
            speed = data->magnitude;
            result = ((sint32)(speed * direction[2]) >> 14);
            actor_motion_object->velocity[2] = (uint32)(result);
            {
                (void)(uint32)(result);
                return;
            }
        case 5:
            phase = (sint8)secondary->motion_phase;
            if (phase == 0 || phase == 1)
            {
                waypoint = data->waypoint[phase == 0 ? 0 : 1].word;
                target[0] = waypoint[0];
                target[1] = waypoint[1];
                target[2] = waypoint[2];
            }
            distance = game_measure_direction_14(pose_owner_object->pose.position, target, direction);
            speed = data->magnitude;
            if ((sint32)distance < (sint32)speed)
            {
                phase = (sint8)secondary->motion_phase;
                secondary->motion_phase = (uint8)(phase == 0 ? 1 : 0);
                speed = data->magnitude;
            }
            actor_motion_object->velocity[0] = (uint32)((uint32)((sint32)(speed * direction[0]) >> 14));
            speed = data->magnitude;
            actor_motion_object->velocity[1] = (uint32)((uint32)((sint32)(speed * direction[1]) >> 14));
            speed = data->magnitude;
            result = ((sint32)(speed * direction[2]) >> 14);
            actor_motion_object->velocity[2] = (uint32)(result);
            {
                (void)(uint32)(result);
                return;
            }
        case 6:
            flags = actor_traits_object->render_flags;
            phase = (sint8)secondary->motion_phase;
            actor_traits_object->render_flags = (uint8)(flags | 4);
            if (phase == 0)
            {
                actor_motion_object->velocity[0] = (uint32)(0);
                value = data->vertical_velocity;
                actor_motion_object->velocity[2] = (uint32)(0);
                actor_motion_object->velocity[1] = (uint32)(value);
                a = pose_owner_object->pose.position[1];
                b = data->waypoint[0].word[1];
                if ((sint32)(a - b) >= -1048576)
                    actor_traits_object->parameter = (uint16)(12288);
                a = pose_owner_object->pose.position[1];
                b = data->waypoint[0].word[1];
                result = (sint32)a < (sint32)b;
                if (result == 0)
                {
                    actor_traits_object->parameter = (uint16)(4096);
                    pose_owner_object->pose.position[1] = (uint32)(b);
                    secondary->motion_phase = (uint8)(1);
                    secondary->anim_ticks = (uint32)(0);
                    return;
                }
                {
                    (void)(uint32)(result);
                    return;
                }
            }
            if (phase == 1)
            {
                timer = secondary->anim_ticks;
                if ((sint32)timer < 32)
                    actor_traits_object->parameter = (uint16)(8192);
                else if ((sint32)timer < 64)
                    actor_traits_object->parameter = (uint16)(12288);
                else
                {
                    secondary->motion_phase = (uint8)(2);
                    secondary->anim_step = (uint32)(512);
                }
                result = secondary->anim_ticks + 1;
                secondary->anim_ticks = (uint32)(result);
                {
                    (void)(uint32)(result);
                    return;
                }
            }
            if (phase != 2)
                return;
            a = data->waypoint[0].word[1];
            b = pose_owner_object->pose.position[1];
            if ((sint32)a < (sint32)b)
                pose_owner_object->pose.position[1] = (uint32)(a);
            target[0] = data->waypoint[0].word[0];
            target[1] = data->waypoint[0].word[1];
            target[2] = data->waypoint[0].word[2];
            distance = game_measure_direction_14(pose_owner_object->pose.position, target, direction);
            flags = game_runtime.metrics.flags;
            if ((flags & 0x40) == 0 && distance == 0)
            {
                mission_script_start_legacy(mission_event_script(MISSION_EVENT_SIGNAL, 0));
                flags = game_runtime.metrics.flags;
                game_runtime.metrics.flags = (uint32)(flags | 0x40);
            }
            speed = data->magnitude;
            actor_motion_object->velocity[0] = (uint32)((uint32)((sint32)(speed * direction[0]) >> 14));
            speed = data->magnitude;
            actor_motion_object->velocity[1] = (uint32)((uint32)((sint32)(speed * direction[1]) >> 14));
            speed = data->magnitude;
            phase = (sint16)actor_traits_object->parameter;
            actor_motion_object->velocity[2] = (uint32)((uint32)((sint32)(speed * direction[2]) >> 14));
            if (phase == 0x4000)
                secondary->anim_step = (uint32)(0xFFFFFE00);
            else if (phase == 0x2000)
                secondary->anim_step = (uint32)(512);
            a = actor_traits_object->parameter;
            b = (uint16)secondary->anim_step;
            result = a + b;
            actor_traits_object->parameter = (uint16)(result);
            {
                (void)(uint32)(result);
                return;
            }
        case 7:
            actor_index_pointer = 0;
            pose_owner_a = mission_actor(actor_index_pointer);
            a = ((sint32)pose_owner_a->pose.position[0] >> 12);
            pose_owner_b = mission_actor(actor_index_pointer);
            b = ((sint32)pose_owner_b->pose.position[1] >> 12);
            pose_owner_c = mission_actor(actor_index_pointer);
            c = ((sint32)pose_owner_c->pose.position[2] >> 12);
            result = (sint32)a < -20479;
            if (result != 0)
            {
                (void)(uint32)(result);
                return;
            }
            result = (sint32)a < -18432;
            if (result == 0)
            {
                (void)(uint32)(result);
                return;
            }
            result = (sint32)b < -1023;
            if (result != 0)
            {
                (void)(uint32)(result);
                return;
            }
            result = (sint32)c < -6144;
            if (result == 0)
            {
                (void)(uint32)(result);
                return;
            }
            result = (sint32)c < -22527;
            if (result != 0)
            {
                (void)(uint32)(result);
                return;
            }
            goto mark_object;
        case 8:
            actor_index_pointer = 0;
            pose_owner_pointer = mission_actor(actor_index_pointer);
            a = pose_owner_object->pose.position[2];
            b = pose_owner_pointer->pose.position[2];
            c = data->waypoint[0].word[0];
            result = (sint32)(a - b) < (sint32)c;
            if (result == 0)
            {
                (void)(uint32)(result);
                return;
            }
        mark_object:
            flags = actor_traits_object->render_flags;
            result = ~flags & 4;
            if (result != 0)
            {
                actor_flags = mission_actor_flags(index + 32);
                result = (*actor_flags) | 0x20000;
                *actor_flags = (result);
            }
            {
                (void)(uint32)(result);
                return;
            }
        case 9:
            actor_index_pointer = 0;
            pose_owner_pointer = mission_actor(actor_index_pointer);
            if (index == 0)
            {
                nearest = 0x7FFFFFFF;
                for (scan = 0; scan < 3; ++scan)
                {
                    distance = game_measure_distance_gte(secondary_points[scan], pose_owner_pointer->pose.position);
                    if ((sint32)distance < (sint32)nearest)
                    {
                        nearest = distance;
                        secondary_control.nearest_point = (uint16)(scan);
                    }
                    a = (sint16)secondary_control.charge[scan];
                    b = secondary_control.charge[scan];
                    if ((sint32)a > 0)
                    {
                        timer = game_runtime.mission_control.ticks;
                        if ((timer & 1) != 0)
                            secondary_control.charge[scan] = (uint16)(b - 1);
                    }
                }
                selected = (sint16)secondary_control.nearest_point;
                a = (sint16)secondary_control.charge[selected];
                b = secondary_control.charge[selected];
                if ((sint32)a > 0 && (sint32)nearest < 0x4000)
                    secondary_control.charge[selected] = (uint16)(b - 16);
                selected = (sint16)secondary_control.nearest_point;
                a = (sint16)secondary_control.charge[selected];
                if ((sint32)a < 0)
                    secondary_control.charge[selected] = (uint16)(0);
                value = game_runtime.mission_control.mode;
                if (value == 4)
                {
                    if ((sint32)nearest < 12288)
                    {
                        flags = game_runtime.metrics.flags;
                        strength = secondary_control.proximity_strength;
                        game_runtime.metrics.flags = (uint32)(flags | 8);
                        a = game_runtime.mission_control.ot;
                        b = game_runtime.mission_control.packet_cur;
                        value = effect_draw_screen_transition(a, b, strength);
                        strength = secondary_control.proximity_strength;
                        game_runtime.mission_control.packet_cur = (uint32)(value);
                        if ((sint32)strength < 1024)
                            secondary_control.proximity_strength = (uint32)(strength + 16);
                        timer = game_runtime.mission_control.ticks;
                        if ((timer & 31) == 0)
                            flight_apply_player_damage_tick();
                    }
                    else
                    {
                        strength = secondary_control.proximity_strength;
                        flags = game_runtime.metrics.flags;
                        strength -= 64;
                        secondary_control.proximity_strength = (uint32)(strength);
                        game_runtime.metrics.flags = (uint32)(flags & 0xFFFFFFF7);
                        if ((sint32)strength < 0)
                            secondary_control.proximity_strength = (uint32)(0);
                        strength = secondary_control.proximity_strength;
                        if ((sint32)strength > 0)
                        {
                            a = game_runtime.mission_control.ot;
                            b = game_runtime.mission_control.packet_cur;
                            value = effect_draw_screen_transition(a, b, strength);
                            game_runtime.mission_control.packet_cur = (uint32)(value);
                        }
                    }
                }
            }
            flags = actor_traits_object->render_flags;
            actor_traits_object->render_flags = (uint8)(flags | 4);
            selected = (sint16)secondary_control.nearest_point;
            a = secondary_control.charge[selected];
            parameters = &mission_secondary_binding(index)->parameters;
            b = 3 - (uint32)((sint32)(a << 16) >> 28);
            a = parameters->model;
            actor_traits_object->model = (uint8)(a + (b << 1));
            selected = (sint16)secondary_control.nearest_point;
            a = secondary_control.charge[selected];
            actor_traits_object->parameter = (uint16)(a & 0xFFF);
            waypoint = secondary_points[selected];
            a = waypoint[0];
            pose_owner_object->pose.position[0] = (uint32)(a + ((index & 1) << 24) - 0x800000);
            a = waypoint[1];
            pose_owner_object->pose.position[1] = (uint32)(a);
            a = waypoint[2];
            result = 0x800000;
            pose_owner_object->pose.position[2] = (uint32)(a - ((uint32)((sint32)index >> 1) << 24) + result);
            {
                (void)(uint32)(result);
                return;
            }
        default:
            return;
    }
}

void mission_secondary_update_target(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_ACTOR *secondary;
    uint32 actor_index_table;

    const MISSION_SECONDARY_WEAPON_PROFILE *data;
    uint32 enabled, mission, kind, x, selected, member, result, flags, target_x, object_x, object_y, target_y, target_z, object_z, dx, dy, dz, distance, limit;
    MISSION_ACTOR_TARGET *pointer;
    FUNCTION_MARKER(0x80052FA8, "ACE3.BIN");
    actor_index_table = 32;
    data = &mission_secondary_binding(index)->weapon_profile;
    enabled = data->kind;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    secondary = mission_secondary_get(index);
    if (enabled == 0)
        return;
    mission = (sint16)game_runtime.selection.mission;
    if (mission == 12)
    {
        kind = (sint16)actor_traits_object->kind;
        if (kind == 54)
        {
            x = pose_owner_object->pose.position[0];
            if (x + 0x1B000000 <= 0x02000000)
                return;
            if (x + 0x14000000 <= 0x04000000)
                return;
            if (x + 0x07000000 <= 0x02000000)
                return;
            if (x + 0xFF800000 <= 0x03000000)
                return;
        }
    }
    selected = secondary->target_index;
    member = index + 32;
    if (selected == 255)
    {
        result = mission_ai_find_target(member);
        selected = result & 255;
        secondary->target_index = (uint8)(result);
        if (selected == 255)
        {
            flags = actor_traits_object->flags;
            if ((flags & 0x80) != 0)
                secondary->target_index = (uint8)(0);
            result = secondary->target_index;
            if (result == selected)
                return;
        }
    }
    selected = secondary->target_index;
    if (selected == 0)
    {
        pointer = mission_actor_target(member);
        distance = (pointer->distance);
    }
    else
    {
        selected = secondary->target_index;
        actor_index_table = 0;
        flags = (*mission_actor_flags(selected));
        pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
        if ((flags & 1) == 0)
            secondary->target_index = (uint8)(255);
        object_x = pose_owner_object->pose.position[0];
        target_x = pose_owner_target->pose.position[0];
        object_y = pose_owner_object->pose.position[1];
        dx = (sint32)(object_x - target_x) >> 12;
        if ((sint32)dx < 0)
            dx = 0 - dx;
        target_y = pose_owner_target->pose.position[1];
        target_z = pose_owner_target->pose.position[2];
        dy = (sint32)(object_y - target_y) >> 12;
        if ((sint32)dy < 0)
            dy = 0 - dy;
        object_z = pose_owner_object->pose.position[2];
        dz = (sint32)(object_z - target_z) >> 12;
        if ((sint32)dz < 0)
            dz = 0 - dz;
        distance = dx + dy + dz;
    }
    limit = data->range;
    if (limit < distance)
    {
        result = mission_target_links_release(member);
        secondary->target_index = (uint8)(result);
        return;
    }
    flags = data->kind;
    if ((flags & 0x10) != 0)
        result = mission_secondary_fire_weapon(index);
    else
        result = mission_secondary_fire_particles(index, distance);
    if (result != 0)
    {
        selected = mission_target_links_release(member);
        secondary->target_index = (uint8)(selected);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 mission_secondary_fire_particles(uint32 index, uint32 distance)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_MOTION *actor_motion_target;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_ACTOR *secondary;
    uint32 actor_index_table;

    const MISSION_SECONDARY_WEAPON_PROFILE *data;
    uint32 selected, first, second, third, ticks, velocity, value, gravity, old_y, old_x, target_x, target_y, old_z, target_z, difficulty, flags, spread, mode, random, angle, current, delta, lower, upper, phase, divisor, counter, kind;
    uint32 direction[3];
    FUNCTION_MARKER(0x80053214, "ACE3.BIN");
    ticks = (sint32)distance / 144;
    actor_index_table = 32;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    secondary = mission_secondary_get(index);
    selected = secondary->target_index;
    actor_index_table = 0;
    data = &mission_secondary_binding(index)->weapon_profile;
    pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    actor_motion_target = mission_actor_motion((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    first = pose_owner_object->pose.position[0];
    second = pose_owner_object->pose.position[1];
    third = pose_owner_object->pose.position[2];
    secondary->launch_pose.position[0] = (uint32)(first);
    secondary->launch_pose.position[1] = (uint32)(second);
    secondary->launch_pose.position[2] = (uint32)(third);
    first = pose_owner_target->pose.position[0];
    second = pose_owner_target->pose.position[1];
    third = pose_owner_target->pose.position[2];
    secondary->target_pos[0] = (uint32)(first);
    secondary->target_pos[1] = (uint32)(second);
    secondary->target_pos[2] = (uint32)(third);
    velocity = actor_motion_target->velocity[0];
    value = secondary->target_pos[0];
    secondary->target_pos[0] = (uint32)(value + velocity * ticks);
    velocity = actor_motion_target->velocity[1];
    value = secondary->target_pos[1];
    secondary->target_pos[1] = (uint32)(value + velocity * ticks);
    gravity = ticks * (ticks * 669);
    velocity = actor_motion_target->velocity[2];
    gravity = (sint32)gravity >> 1;
    value = secondary->target_pos[1];
    old_x = pose_owner_object->pose.position[0];
    target_x = secondary->target_pos[0];
    secondary->target_pos[1] = (uint32)(value - gravity);
    value = secondary->target_pos[2];
    secondary->target_pos[2] = (uint32)(value + velocity * ticks);
    direction[0] = (sint32)(target_x - old_x) >> 12;
    target_y = secondary->target_pos[1];
    old_y = pose_owner_object->pose.position[1];
    direction[1] = (sint32)(target_y - old_y) >> 12;
    target_z = secondary->target_pos[2];
    old_z = pose_owner_object->pose.position[2];
    direction[2] = (sint32)(target_z - old_z) >> 12;
    game_vector_angles(direction, secondary->launch_pose.angles);
    difficulty = game_runtime.selection.difficulty;
    spread = 600;
    if (difficulty == 0)
    {
        flags = actor_traits_object->flags;
        if ((flags & 0x80) != 0)
            goto spread_ready;
    }
    mode = data->accuracy;
    if (mode == 0)
        spread = 0;
    else if (mode == 1)
        spread = 200;
spread_ready:
    if (spread != 0)
    {
        random = game_random();
        angle = secondary->launch_pose.angles[0];
        secondary->launch_pose.angles[0] = (uint16)(angle + (uint32)((sint32)random % (sint32)(spread << 1)) - spread);
        random = game_random();
        angle = secondary->launch_pose.angles[1];
        secondary->launch_pose.angles[1] = (uint16)(angle + (uint32)((sint32)random % (sint32)(spread << 1)) - spread);
    }
    angle = (sint16)secondary->launch_pose.angles[0];
    current = (sint16)pose_owner_object->pose.angles[0];
    delta = angle - current;
    if ((sint32)delta > 4096)
        delta = 0x2000 - delta;
    else if ((sint32)delta < -4096)
        delta = 0 - 8192 - delta;
    lower = data->min_pitch;
    if ((sint32)delta < (sint32)lower)
        return 1;
    upper = data->max_pitch;
    if ((sint32)upper < (sint32)delta)
        return 1;
    angle = (sint16)secondary->launch_pose.angles[1];
    current = (sint16)pose_owner_object->pose.angles[1];
    lower = data->min_yaw;
    delta = (angle - current) & 0x3FFF;
    if ((sint32)delta < (sint32)lower)
        delta += 0x4000;
    upper = data->max_yaw;
    if ((sint32)upper < (sint32)delta)
        return 1;
    phase = (sint8)secondary->weapon_phase;
    if (phase == 0)
    {
        divisor = data->shot_interval;
        counter = secondary->weapon_ticks;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && counter == 0x80000000)
            xport_mips_break(6);
        if ((sint32)counter % (sint32)divisor == 0)
        {
            kind = data->kind;
            flags = actor_traits_object->flags;
            effect_spawn_enqueue(flags, secondary_projectile_velocity, kind - 1, &secondary->launch_pose);
        }
        upper = data->attack_ticks;
        counter = secondary->weapon_ticks;
        if ((sint32)counter >= (sint32)upper)
        {
            secondary->weapon_phase = (uint8)(1);
            secondary->weapon_ticks = (uint32)(0);
        }
    }
    else
    {
        upper = data->recovery_ticks;
        counter = secondary->weapon_ticks;
        if ((sint32)counter >= (sint32)upper)
        {
            secondary->weapon_phase = (uint8)(0);
            secondary->weapon_ticks = (uint32)(0);
        }
    }
    counter = secondary->weapon_ticks;
    secondary->weapon_ticks = (uint32)(counter + 1);
    return 0;
}

uint32 mission_secondary_fire_weapon(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target_object;
    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_target_object;
    MISSION_SECONDARY_ACTOR *secondary;
    uint32 actor_index_table;

    const MISSION_SECONDARY_WEAPON_PROFILE *data;
    uint32 selected, mask, saved_phase, saved_counter, first, second, third, end, start, angle, current, delta, lower, upper, kind, mode, difficulty, flags, phase, delay, counter;
    uint32 direction[3];
    FUNCTION_MARKER(0x800535D4, "ACE3.BIN");
    actor_index_table = 32;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    secondary = mission_secondary_get(index);
    selected = secondary->target_index;
    actor_index_table = 0;
    data = &mission_secondary_binding(index)->weapon_profile;
    pose_owner_target_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    actor_traits_target_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    if (selected < 32)
    {
        mask = effect_actor_tracking.target_mask[0];
        if ((mask & ((uint32)1 << selected)) != 0)
            return 1;
    }
    else
    {
        mask = effect_actor_tracking.target_mask[1];
        if ((mask & ((uint32)1 << ((selected - 32) & 31))) != 0)
            return 1;
    }
    saved_phase = (sint8)secondary->weapon_phase;
    saved_counter = secondary->weapon_ticks;
    secondary->weapon_phase = (uint8)(0);
    secondary->weapon_ticks = (uint32)(0);
    first = pose_owner_object->pose.position[0];
    second = pose_owner_object->pose.position[1];
    third = pose_owner_object->pose.position[2];
    secondary->launch_pose.position[0] = (uint32)(first);
    secondary->launch_pose.position[1] = (uint32)(second);
    secondary->launch_pose.position[2] = (uint32)(third);
    first = pose_owner_target_object->pose.position[0];
    second = pose_owner_target_object->pose.position[1];
    third = pose_owner_target_object->pose.position[2];
    secondary->target_pos[0] = (uint32)(first);
    secondary->target_pos[1] = (uint32)(second);
    secondary->target_pos[2] = (uint32)(third);
    end = secondary->target_pos[0];
    start = pose_owner_object->pose.position[0];
    direction[0] = (sint32)(end - start) >> 12;
    end = secondary->target_pos[1];
    start = pose_owner_object->pose.position[1];
    direction[1] = (sint32)(end - start) >> 12;
    end = secondary->target_pos[2];
    start = pose_owner_object->pose.position[2];
    direction[2] = (sint32)(end - start) >> 12;
    game_vector_angles(direction, secondary->launch_pose.angles);
    angle = (sint16)secondary->launch_pose.angles[0];
    current = (sint16)pose_owner_object->pose.angles[0];
    delta = angle - current;
    if ((sint32)delta > 4096)
        delta = 0x2000 - delta;
    else if ((sint32)delta < -4096)
        delta = 0 - 8192 - delta;
    lower = data->min_pitch;
    if ((sint32)delta < (sint32)lower)
        return 1;
    upper = data->max_pitch;
    if ((sint32)upper < (sint32)delta)
        return 1;
    angle = (sint16)secondary->launch_pose.angles[1];
    current = (sint16)pose_owner_object->pose.angles[1];
    lower = data->min_yaw;
    delta = (angle - current) & 0x3FFF;
    if ((sint32)delta < (sint32)lower)
        delta += 0x4000;
    upper = data->max_yaw;
    if ((sint32)upper < (sint32)delta)
        return 1;
    secondary->weapon_phase = (uint8)(saved_phase);
    secondary->weapon_ticks = (uint32)(saved_counter);
    kind = data->kind;
    mode = 0;
    if (kind == 17)
    {
        difficulty = game_runtime.selection.difficulty;
        mode = 0x100;
        if (difficulty == 0)
        {
            flags = actor_traits_object->flags;
            if ((flags & 0x80) != 0)
                goto mode_ready;
        }
        kind = data->accuracy;
        if (kind == 0)
            mode = 0x20100;
        else if (kind == 1)
            mode = 0x10100;
    }
mode_ready:
    phase = (sint8)secondary->weapon_phase;
    if (phase == 0)
    {
        flags = actor_traits_object->flags;
        delay = data->attack_ticks;
        if ((flags & 0x80) != 0)
        {
            difficulty = game_runtime.selection.difficulty;
            if (difficulty == 0)
                delay <<= 1;
            if ((sint32)difficulty >= 2)
                delay = (sint32)delay >> 1;
        }
        counter = secondary->weapon_ticks;
        if (counter == (uint32)((sint32)delay >> 1))
        {
            flags = mission_target.warning_flags;
            mission_target.warning_flags = flags | 1;
        }
        counter = secondary->weapon_ticks;
        if ((sint32)counter < (sint32)delay)
            goto advance_counter;
        kind = data->kind;
        kind -= 17;
        if (kind < 9)
        {
            // dispatch the original weapon IDs 17..25
            switch (kind)
            {
                case 0:
                case 3:
                    flags = actor_traits_object->flags;
                    selected = actor_traits_target_object->flags;
                    effect_object_create_variant_1(flags, selected, mode, &secondary->launch_pose, secondary_projectile_velocity);
                    break;
                case 1:
                    flags = actor_traits_object->flags;
                    selected = actor_traits_target_object->flags;
                    effect_object_create_variant_2(flags, selected, &secondary->launch_pose, secondary_projectile_velocity);
                    break;
                case 2:
                    flags = actor_traits_object->flags;
                    selected = actor_traits_target_object->flags;
                    effect_object_create_variant_3(flags, selected, &secondary->launch_pose, secondary_projectile_velocity);
                    break;
                case 7:
                    flags = actor_traits_object->flags;
                    selected = actor_traits_target_object->flags;
                    effect_object_create_variant_7(flags, selected, &secondary->launch_pose, secondary_projectile_velocity);
                    break;
                case 8:
                    flags = actor_traits_object->flags;
                    selected = actor_traits_target_object->flags;
                    effect_object_create_variant_8(flags, selected, &secondary->launch_pose, secondary_projectile_velocity);
                    break;
                case 4:
                case 5:
                case 6:
                    break;
            }
        }
        secondary->weapon_phase = (uint8)(1);
        secondary->weapon_ticks = (uint32)(0);
    }
    else
    {
        upper = data->recovery_ticks;
        counter = secondary->weapon_ticks;
        if ((sint32)counter < (sint32)upper)
            goto advance_counter;
        secondary->weapon_phase = (uint8)(0);
        secondary->weapon_ticks = (uint32)(0);
    }
advance_counter:
    counter = secondary->weapon_ticks;
    secondary->weapon_ticks = (uint32)(counter + 1);
    return 0;
}

void mission_secondary_bind_resources(const MISSION_ACTOR_CATALOG_INPUT *input)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint32 wire_word_10;
    uint16 wire_halfword_11;
    uint16 wire_halfword_12;
    uint16 wire_halfword_13;
    uint16 wire_halfword_14;
    uint16 wire_halfword_15;
    uint16 wire_halfword_16;
    uint16 wire_halfword_17;
    MISSION_ACTOR_DEFINITION *definition;
    const uint8 *source, *definition_source, *weapon_input;

    MISSION_SECONDARY_BINDING *out;
    MISSION_SECONDARY_PARAMETERS *parameters;
    uint32 result, count, i = 0, source_offset, index, packed;
    FUNCTION_MARKER(0x8004D680, "ACE3.BIN");
    result = arena_alloc(game_runtime.actor_resources.secondary_count * 40);
    count = game_runtime.actor_resources.secondary_count;
    if (count != 0)
    {
        source_offset = 0;
        do
        {
            (void)mission_actor((32 + ((uint32)((sint32)(i * 4) >> 2))));
            out = mission_secondary_binding(i);
            parameters = &out->parameters;
            source = mission_actor_catalog_record(&input->secondary_parameters, source_offset, 36);
            parameters->category = source[0];
            parameters->kind = source[1];
            parameters->model = source[2];
            parameters->pose.position[0] = (memcpy(&wire_word_0, source + 4, sizeof(wire_word_0)), wire_word_0);
            parameters->pose.position[1] = (memcpy(&wire_word_1, source + 8, sizeof(wire_word_1)), wire_word_1);
            parameters->pose.position[2] = (memcpy(&wire_word_2, source + 12, sizeof(wire_word_2)), wire_word_2);
            packed = (memcpy(&wire_word_4, source + 20, sizeof(wire_word_4)), wire_word_4);
            parameters->pose.angles[0] = (uint16)packed;
            parameters->pose.angles[1] = (uint16)(packed >> 16);
            packed = (memcpy(&wire_word_5, source + 24, sizeof(wire_word_5)), wire_word_5);
            parameters->pose.angles[2] = (uint16)packed;
            parameters->flags = source[28];
            parameters->selectors[0] = source[29];
            parameters->selectors[1] = source[30];
            parameters->render_mode = source[31];
            parameters->attributes_index = source[32];
            parameters->weapon_index = source[33];
            parameters->motion_index = source[34];
            parameters->animation_index = source[35];
            index = parameters->attributes_index;
            definition_source = mission_actor_catalog_record(&input->definitions, index << 4, 16);
            definition = &out->definition;
            definition->health = (memcpy(&wire_halfword_6, definition_source, sizeof(wire_halfword_6)), wire_halfword_6);
            definition->label = definition_source[3];
            definition->shape = definition_source[4];
            definition->highlight = definition_source[5];
            definition->target_flags = definition_source[6];
            definition->radar_kind = definition_source[7];
            definition->lock_kind = definition_source[8];
            definition->render_mode = definition_source[9];
            definition->damage_mode = definition_source[10];
            definition->radius[0] = (memcpy(&wire_halfword_7, definition_source + 12, sizeof(wire_halfword_7)), wire_halfword_7);
            definition->radius[1] = (memcpy(&wire_halfword_8, definition_source + 14, sizeof(wire_halfword_8)), wire_halfword_8);
            index = parameters->weapon_index;
            weapon_input = index == 255 ? NULL : mission_actor_catalog_record(&input->secondary_weapon, index * 24, 24);
            out->has_weapon_profile = weapon_input != NULL;
            if (weapon_input)
            {
                out->weapon_profile.kind = weapon_input[0];
                out->weapon_profile.accuracy = weapon_input[1];
                out->weapon_profile.damage = (memcpy(&wire_halfword_9, weapon_input + 2, sizeof(wire_halfword_9)), wire_halfword_9);
                out->weapon_profile.range = (memcpy(&wire_word_10, weapon_input + 4, sizeof(wire_word_10)), wire_word_10);
                out->weapon_profile.min_pitch = (sint16)(memcpy(&wire_halfword_11, weapon_input + 8, sizeof(wire_halfword_11)), wire_halfword_11);
                out->weapon_profile.max_pitch = (sint16)(memcpy(&wire_halfword_12, weapon_input + 10, sizeof(wire_halfword_12)), wire_halfword_12);
                out->weapon_profile.min_yaw = (sint16)(memcpy(&wire_halfword_13, weapon_input + 12, sizeof(wire_halfword_13)), wire_halfword_13);
                out->weapon_profile.max_yaw = (sint16)(memcpy(&wire_halfword_14, weapon_input + 14, sizeof(wire_halfword_14)), wire_halfword_14);
                out->weapon_profile.shot_interval = (sint16)(memcpy(&wire_halfword_15, weapon_input + 16, sizeof(wire_halfword_15)), wire_halfword_15);
                out->weapon_profile.attack_ticks = (sint16)(memcpy(&wire_halfword_16, weapon_input + 18, sizeof(wire_halfword_16)), wire_halfword_16);
                out->weapon_profile.recovery_ticks = (sint16)(memcpy(&wire_halfword_17, weapon_input + 20, sizeof(wire_halfword_17)), wire_halfword_17);
            }
            index = parameters->motion_index;
            if (index >= secondary_motion_catalog.count)
                abort();
            out->motion = secondary_motion_catalog.records[index];
            index = parameters->animation_index;
            out->animation = secondary_animation_decode(index == 255 ? NULL : mission_actor_catalog_record(&input->secondary_animation, index << 4, 16));
            out->stage_script = mission_script_reference(0);
            out->visible_script = mission_script_reference(0);
            out->destroy_script = mission_script_reference(0);
            count = game_runtime.actor_resources.secondary_count;
            ++i;
            result = i < count;
            source_offset += 36;
        } while (result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_secondary_update_health(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 flags, health, value;
    MISSION_SCRIPT_REFERENCE script;
    uint32 *slot;
    FUNCTION_MARKER(0x800520C8, "ACE3.BIN");
    slot = mission_actor_flags(index + 32);
    actor_index_table = 32;
    flags = (*slot);
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    if ((flags & 1) == 0)
        return;
    health = (sint16)actor_traits_object->health;
    if ((sint32)health > 0)
        return;
    flags = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(flags & 0xFFFFFDFF);
    if ((*slot) & 0x100)
    {
        value = game_runtime.metrics.flagged_count[0];
        game_runtime.metrics.flagged_count[0] = (uint8)(value + 1);
    }
    if ((*slot) & 0x200)
    {
        value = game_runtime.metrics.flagged_count[1];
        game_runtime.metrics.flagged_count[1] = (uint8)(value + 1);
    }
    if (((*slot) & 0x800) && (actor_traits_object->damage_source & 0x3F) == 0)
    {
        flags = actor_traits_object->flags;
        if ((flags & 0x40) == 0)
        {
            if (game_runtime.options.kill_camera & 1)
                camera_try_kill_view(flags);
            if ((sint16)game_runtime.selection.mission == 22 && (sint16)actor_traits_object->kind == 70)
                mission_status_text_show(46);
            else if (actor_traits_object->flags & 0x80)
                mission_status_text_show(7);
        }
    }
    mission_result_record_event(index + 32);
    flags = (*slot);
    *slot = ((flags & 0xFF000000) | 2);
    mission_secondary_apply_animation_change(index);
    script = mission_secondary_binding(index)->destroy_script;
    if (mission_script_reference_present(script))
        mission_script_start_legacy(script);
    return;
}

void mission_secondary_swap_animation(uint32 first_index, uint32 second_index)
{
    MISSION_SECONDARY_BINDING *first, *second;
    MISSION_SECONDARY_ANIMATION animation;
    FUNCTION_MARKER(0x80053B24, "ACE3.BIN");
    first = mission_secondary_binding(first_index);
    second = mission_secondary_binding(second_index);
    animation = first->animation;
    first->animation = second->animation;
    second->animation = animation;
}

void mission_secondary_reset_points(void)
{
    FUNCTION_MARKER(0x80053B5C, "ACE3.BIN");
    secondary_control.charge[2] = (uint16)(0x4000);
    secondary_control.charge[1] = (uint16)(0x4000);
    secondary_control.charge[0] = (uint16)(0x4000);
    secondary_control.nearest_point = (uint16)(0);
    return;
}

void mission_secondary_reset_signals(void)
{
    FUNCTION_MARKER(0x80053BA0, "ACE3.BIN");
    secondary_control.signal_flags = (uint32)(0);
}

void mission_secondary_shuffle_animations(void)
{
    static const uint8 pair_groups[4] = {0, 2, 4, 6};
    static const uint8 triple_groups[6] = {10, 13, 16, 19, 22, 25};
    uint32 i, index, random, remainder;
    FUNCTION_MARKER(0x800539D0, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
    {
        index = pair_groups[i];
        random = game_random();
        if ((random & 1) != 0)
            mission_secondary_swap_animation(index, index + 1);
    }
    for (i = 0; i < 6; ++i)
    {
        index = triple_groups[i];
        random = game_random();
        remainder = (sint32)random % 6;
        if (remainder < 6)
        {
            switch (remainder)
            {
                case 0:
                    break;
                case 1:
                    mission_secondary_swap_animation(index + 1, index + 2);
                    break;
                case 2:
                    mission_secondary_swap_animation(index, index + 1);
                    break;
                case 3:
                    mission_secondary_swap_animation(index, index + 1);
                    mission_secondary_swap_animation(index + 1, index + 2);
                    break;
                case 4:
                    mission_secondary_swap_animation(index + 1, index + 2);
                    mission_secondary_swap_animation(index, index + 1);
                    break;
                case 5:
                    mission_secondary_swap_animation(index, index + 2);
                    break;
            }
        }
    }
    return;
}

void mission_secondary_update_all(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index;
    FUNCTION_MARKER(0x80051414, "ACE3.BIN");
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 32;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~actor_traits_object->flags & 0xC000) == 0)
            mission_secondary_update_actor(index);
    }
    if ((sint16)game_runtime.selection.mission == 41)
        return;
    {
        (void)(proximity_audio_update());
        return;
    }
}

void mission_secondary_update_actor(uint32 index)
{
    MISSION_SECONDARY_BINDING *state;
    const MISSION_SECONDARY_ANIMATION *animation;
    uint32 flags, value;
    uint32 *slot;
    FUNCTION_MARKER(0x80051A54, "ACE3.BIN");
    state = mission_secondary_binding(index);
    slot = mission_actor_flags(index + 32);
    flags = (*slot);
    animation = &state->animation;
    if (flags & 0x20000)
        mission_secondary_animate(index);
    flags = (*slot);
    if ((flags & 1) == 0)
    {
        mission_secondary_stop_idle(index);
        return;
    }
    if (animation->has_descriptor && animation->start_mode == 1 && animation->frame_count != 0)
        *slot = (flags | 0x20000);
    mission_secondary_update_health(index);
    if (((*slot) & 1) == 0)
        return;
    mission_secondary_move(index);
    value = game_runtime.mission_control.mode;
    if (value != 4)
        return;
    if (!state->has_weapon_profile)
        return;
    {
        (void)(mission_secondary_update_target(index));
        return;
    }
}

void mission_secondary_activate_actor(uint32 index)
{
    const MISSION_ACTOR_DEFINITION *definition_attributes;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_BINDING *record;
    const MISSION_SECONDARY_PARAMETERS *data;
    uint32 actor_index_table;

    uint32 initial, value, mode, raw, result;
    uint32 *status;
    FUNCTION_MARKER(0x8004E94C, "ACE3.BIN");
    status = mission_actor_flags((index) + 32);
    actor_index_table = 0;

    record = mission_secondary_binding(index);
    actor_traits_object = mission_actor_traits(((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))) + 32));
    data = &record->parameters;
    initial = (*status);
    definition_attributes = &record->definition;
    *status = (initial | 1);
    value = data->flags;
    if ((value & 1) != 0)
        *status = (initial | 0x101);
    value = data->flags;
    if ((value & 2) != 0)
    {
        value = (*status);
        *status = (value | 0x200);
    }
    value = definition_attributes->radar_kind;
    if (value != 0)
    {
        value = (*status);
        *status = (value | 0x1000);
    }
    value = definition_attributes->lock_kind;
    if (value != 0)
    {
        value = (*status);
        *status = (value | 0x2000);
    }
    value = definition_attributes->target_flags;
    if ((value & 2) != 0)
    {
        value = (*status);
        *status = (value | 0x4000);
    }
    value = definition_attributes->target_flags;
    if ((value & 1) != 0)
    {
        value = (*status);
        *status = (value | 0x8000);
    }
    value = definition_attributes->damage_mode;
    if (value == 0)
    {
        value = (*status);
        *status = (value | 0x10);
    }
    value = definition_attributes->label;
    if (value != 255)
    {
        value = (*status);
        *status = (value | 0x800000);
    }
    value = definition_attributes->shape;
    if (value == 0)
    {
        value = (*status);
        *status = (value | 0x400);
    }
    value = (*status);
    if ((value & 0x1000) != 0)
        *status = (value | 0x800);
    mission_actor_initialize_marker(index + 32, definition_attributes);
    hud_radar_bind_contact(index + 32, definition_attributes);
    mode = definition_attributes->render_mode;
    if (mode == 1)
    {
        value = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(value | 0x800);
    }
    else if (mode == 2)
    {
        value = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(value | 0x1000);
    }
    value = (sint16)definition_attributes->radius[0];
    raw = definition_attributes->radius[0];
    if (value != 0xFFFFFFFF)
    {
        value = actor_traits_object->flags;
        actor_traits_object->radius[0] = (uint16)(raw);
        actor_traits_object->flags = (uint32)(value | 0x200);
    }
    else
        actor_traits_object->radius[0] = (uint16)(0);
    result = (sint16)definition_attributes->radius[1];
    raw = definition_attributes->radius[1];
    if (result != 0xFFFFFFFF)
    {
        result = actor_traits_object->flags;
        actor_traits_object->radius[1] = (uint16)(raw);
        result |= 0x400;
        actor_traits_object->flags = (uint32)(result);
    }
    else
        actor_traits_object->radius[1] = (uint16)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_secondary_prepare_scenario_actors(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    const MISSION_SECONDARY_PARAMETERS *descriptor;
    uint32 actor_index_table;

    uint32 selection, kind, index, flags, health, mode;
    FUNCTION_MARKER(0x8004E80C, "ACE3.BIN");
    selection = mission_flow.scenario_idx;
    kind = mission_scenarios[selection].kind;
    if (kind - 5 >= 2)
        return;
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits(((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))) + 32));
        flags = actor_traits_object->flags;
        if ((flags & 0x4000) == 0)
            continue;
        health = (sint16)actor_traits_object->health;
        if ((sint32)health <= 0)
            continue;
        descriptor = &mission_secondary_binding(index)->parameters;
        mode = descriptor->selectors[0];
        if (mode == 17 || mode == 16)
            mission_actor_show_secondary(index);
    }
    return;
}

void mission_secondary_update_activity_flags(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_ACTOR_TRAITS *actor_traits_descriptor;
    uint32 actor_index_descriptor;
    uint32 actor_index_slots;

    uint32 index, flags, state;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80051498, "ACE3.BIN");
    actor_index_slots = 32;
    for (index = 0; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits((actor_index_slots + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags & 0xFFFF7FFF;
        actor_traits_object->flags = (uint32)(flags);
        if ((flags & 0x4000) == 0)
            continue;
        state = (*mission_actor_flags(index + 32));
        if (state & 1)
        {
            if (state & 0x10)
            {
                actor_traits_object->flags = (uint32)(flags | 0x8000);
                continue;
            }
            target = mission_actor_target(index + 32);
            if ((target->flags) & 0x80)
            {
                actor_traits_object->flags = (uint32)(flags | 0x8000);
                continue;
            }
            actor_index_descriptor = 0;
            actor_traits_descriptor = mission_actor_traits((actor_index_descriptor + ((uint32)((sint32)((index + 32) * 4) >> 2))));
            if ((sint16)actor_traits_descriptor->health <= 0)
                actor_traits_object->flags = (uint32)(flags | 0x8000);
        }
        else if (state & 2)
            actor_traits_object->flags = (uint32)(flags | 0x8000);
    }
    return;
}

void mission_secondary_advance_positions(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, table, flags, a, b, c, d, e, f, scene, mode;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8005157C, "ACE3.BIN");
    actor_index_table = 32;
    for (index = 0; index < 32; ++index)
    {
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        if ((*mission_actor_flags(index + 32)) & 0x400000)
            continue;
        flags = actor_traits_object->events;
        if (flags & 0x80000000)
        {
            a = pose_owner_object->pose.position[0];
            b = actor_motion_object->velocity[0];
            c = actor_motion_object->velocity[1];
            d = actor_motion_object->velocity[2];
            pose_owner_object->pose.position[0] = (uint32)(a + b);
            e = pose_owner_object->pose.position[1];
            f = pose_owner_object->pose.position[2];
            pose_owner_object->pose.position[1] = (uint32)(e + c);
            pose_owner_object->pose.position[2] = (uint32)(f + d);
        }
        flags = actor_traits_object->events;
        actor_traits_object->events = (uint32)(flags & 0xFF000000);
    }
    scene = (sint16)game_runtime.selection.mission;
    if (scene == 8 || scene == 48)
    {
        mode = scene == 8 ? 0 : 1;
        for (index = 0; index < 32; ++index)
        {
            actor_index_table = 32;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((actor_traits_object->flags & 0x4000) == 0)
                continue;
            if (((*mission_actor_flags(index + 32)) & 1) == 0)
                continue;
            target = mission_actor_target(index + 32);
            if (((sint32)target->distance) >= 0x3001)
                continue;
            if ((sint16)actor_traits_object->kind != (mode == 0 ? 65 : 62))
                continue;
            table = game_runtime.gameplay_counter;
            effect_emit_position_periodic(table + index * 8, mode, pose_owner_object->pose.position);
        }
    }
    {
        (void)(uint32)(scene == 48 ? 0 : 48);
        return;
    }
}

void mission_secondary_tick(void)
{
    FUNCTION_MARKER(0x800517F4, "ACE3.BIN");
    mission_secondary_update_activity_flags();
    mission_secondary_update_all();
    {
        (void)(mission_secondary_advance_positions());
        return;
    }
}

void mission_secondary_emit_animation_effect(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    const MISSION_SECONDARY_ANIMATION *animation;
    uint32 value;
    FUNCTION_MARKER(0x80052000, "ACE3.BIN");
    actor_index_table = 32;
    animation = &mission_secondary_binding(index)->animation;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    if (!animation->has_descriptor)
        return;
    value = animation->effect;
    if (value == 0)
        return;
    {
        (void)(effect_emit_position_type4((value & 0x7F) - 1, pose_owner_object->pose.position));
        return;
    }
}

void mission_secondary_apply_animation_change(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const MISSION_SECONDARY_ANIMATION *animation;
    uint32 flags, byte, status, value;
    uint32 *slot;
    FUNCTION_MARKER(0x80051E34, "ACE3.BIN");
    slot = mission_actor_flags(index + 32);
    actor_index_table = 32;
    flags = (*slot);
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    *slot = (flags & 0xFFFDFFFF);
    byte = actor_traits_object->render_flags;
    status = actor_traits_object->events;
    actor_traits_object->parameter = (uint16)(0);
    actor_traits_object->render_flags = (uint8)(byte & 0xFB);
    actor_traits_object->events = (uint32)(status & 0x7FFFFFFF);
    animation = &mission_secondary_binding(index)->animation;
    if (!animation->has_descriptor)
        return;
    value = animation->replacement_model;
    if (value == 254)
        mission_secondary_begin_deactivation(index);
    else if (value != 255)
    {
        flags = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(flags | 0x10000);
        value = animation->replacement_model;
        actor_traits_object->model = (uint8)(value);
    }
    value = animation->entity_event;
    if (value != 255)
        mission_entity_events_trigger(value);
    if (animation->start_mode == 3 && animation->frame_count != 0)
    {
        flags = (*slot);
        *slot = (flags | 0x20000);
    }
    {
        (void)(mission_secondary_emit_animation_effect(index));
        return;
    }
}

void mission_secondary_initialize_frame(void)
{
    FUNCTION_MARKER(0x80038AD4, "ACE3.BIN");
    {
        (void)(mission_secondary_initialize());
        return;
    }
}
