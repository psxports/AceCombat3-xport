#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    uint16 lifetime[3];
    uint16 turn_limit[3];
    uint16 launch_delay[3];
} EFFECT_LAUNCH_PROFILE;

static const EFFECT_LAUNCH_PROFILE effect_launch_profiles[2] = {
    {{120, 135, 150}, {26, 29, 32}, {8, 7, 6}},
    {{180, 240, 300}, {22, 30, 38}, {16, 13, 10}},
};

EFFECT_SPAWN_CALLBACK const effect_spawn_callbacks[21] = {
    effect_spawn_kind_0, effect_spawn_kind_1, effect_spawn_kind_2, effect_spawn_kind_3, effect_spawn_kind_4, effect_spawn_kind_5, effect_spawn_kind_6, effect_spawn_kind_7, effect_spawn_kind_8, effect_spawn_kind_9, effect_spawn_kind_9, effect_spawn_kind_11, effect_spawn_kind_12, effect_spawn_kind_13, effect_spawn_kind_14, effect_spawn_kind_15, effect_spawn_kind_16, effect_spawn_kind_17, effect_spawn_kind_18, effect_spawn_kind_0, effect_spawn_kind_20,
};

EFFECT_OBJECT_POOL effect_objects;

static EFFECT_OBJECT_HANDLER const effect_object_handlers[3][5] = {{effect_object_launch_1, effect_object_launch_2, effect_object_launch_3, effect_object_launch_4, effect_object_launch_5}, {effect_object_guide_1, effect_object_guide_2, effect_object_guide_3, effect_object_guide_4, effect_object_guide_5}, {effect_object_impact_1, effect_object_impact_2, effect_object_impact_3, effect_object_impact_4, 0}};

static EFFECT_OBJECT *effect_object_allocate(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    EFFECT_OBJECT *slot;
    uint32 used, index, value, x, y, z, speed, owner_index, i;
    FUNCTION_MARKER(0x800685C0, "ACE3.BIN");
    used = effect_objects.live;
    if (used == 0xFFFFFFFF)
        return 0;
    index = 0;
    while ((used & (1u << (index & 31))) != 0)
        ++index;
    used |= 1u << (index & 31);
    effect_objects.live = (uint32)(used);
    value = effect_actor_tracking.source_count[flags & 63];
    effect_actor_tracking.source_count[flags & 63] = (uint8)(value + 1);
    slot = effect_objects.slot + index;
    slot->kind = (uint8)(0);
    slot->phase = (uint16)(0);
    slot->ticks = (uint16)(0);
    slot->flags = (uint32)(0);
    slot->model_kind = (uint8)(1);
    slot->render_flags = (uint8)(0);
    slot->event_bits = (uint16)(64);
    slot->source_flags = (uint32)(flags);
    for (i = 0; i < 3; ++i)
    {
        value = (native_position->position[i]);
        slot->pose.position[i] = (uint32)value;
    }
    for (i = 0; i < 3; ++i)
    {
        value = (native_position->angles[i]);
        slot->pose.angles[i] = (uint16)value;
    }
    value = (native_vector[0]);
    slot->velocity[0] = (uint32)(value);
    y = (native_vector[1]);
    x = slot->velocity[0];
    slot->velocity[1] = (uint32)(y);
    z = (native_vector[2]);
    slot->velocity[2] = (uint32)(z);
    speed = game_vector_length_gte((sint32)x >> 12, (sint32)y >> 12, (sint32)z >> 12) << 12;
    slot->initial_speed = (uint32)(speed);
    slot->local_velocity[2] = (uint32)(speed);
    slot->acceleration = (uint32)(0);
    slot->local_velocity[0] = (uint32)(0);
    slot->local_velocity[1] = (uint32)(0);
    slot->delta_angles[0] = (uint16)(0);
    slot->delta_angles[1] = (uint16)(0);
    slot->delta_angles[2] = (uint16)(0);
    slot->distance = (uint32)(0x1D4C0);
    slot->target_flags = (uint32)(owner);
    if (owner != 0xFFFFFFFF)
    {
        owner_index = owner & 63;
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((owner_index << 2)) >> 2))));
        actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((owner_index << 2)) >> 2))));
        value = effect_actor_tracking.target_count[owner_index];
        effect_actor_tracking.target_count[owner_index] = (uint8)(value + 1);
        effect_actor_tracking.last_effect[owner_index] = (uint8)index;
        slot->target_position = pose_owner_object->pose.position;
        slot->target_velocity = actor_motion_object->velocity;
        if ((owner & 0x20) != 0)
        {
            value = effect_actor_tracking.target_mask[1];
            effect_actor_tracking.target_mask[1] = value | (1u << (owner & 31));
        }
        else
        {
            value = effect_actor_tracking.target_mask[0];
            effect_actor_tracking.target_mask[0] = value | (1u << (owner_index & 31));
        }
    }
    else
    {
        value = slot->flags;
        slot->flags = (uint32)(value | 1);
        slot->target_position = slot->pose.position;
        slot->target_velocity = slot->pose.position;
    }
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_1(uint32 flags, uint32 owner, uint32 mode, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *slot;
    uint32 value, kind, first_index, second_index, a, b, c, speed, limit, threshold;
    FUNCTION_MARKER(0x80068800, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    if ((mode & 0x1000000) == 0)
    {
        value = slot->render_flags;
        slot->render_flags = (uint8)(value | 8);
    }
    value = slot->render_flags;
    slot->render_flags = (uint8)(value | ((mode & 0x2000000) != 0 ? 0x20 : 0x10));
    if ((mode & 0x4000000) != 0)
    {
        value = slot->flags;
        slot->flags = (uint32)(value | 0x80);
    }
    if ((mode & 0x8000000) != 0)
    {
        value = slot->flags;
        slot->flags = (uint32)(value | 0x100);
    }
    value = slot->flags;
    kind = mode & 255;
    slot->guide_kind = (uint8)(0);
    slot->impact_kind = (uint8)(0);
    slot->flags = (uint32)(value | 8);
    first_index = (mode >> 8) & 255;
    second_index = (mode >> 16) & 255;
    if (kind == 2)
    {
        value = game_measure_distance_gte(native_position->position, slot->target_position);
        kind = (sint32)value >= 5000;
    }
    if (kind == 0)
    {
        slot->launch_kind = (uint8)(0);
        a = effect_launch_profiles[0].lifetime[first_index];
        slot->lifetime = (uint16)(a);
        b = effect_launch_profiles[0].turn_limit[second_index];
        slot->turn_limit = (uint16)(b);
        c = effect_launch_profiles[0].launch_delay[second_index];
        speed = 0x50000;
        limit = 0x1E00;
        threshold = 0xE00;
    }
    else if (kind == 1)
    {
        slot->launch_kind = (uint8)(1);
        a = effect_launch_profiles[1].lifetime[first_index];
        slot->lifetime = (uint16)(a);
        b = effect_launch_profiles[1].turn_limit[second_index];
        slot->turn_limit = (uint16)(b);
        c = effect_launch_profiles[1].launch_delay[second_index];
        speed = 0x78000;
        limit = 0x1400;
        threshold = 0x1400;
    }
    else
        return slot;
    slot->maximum_speed = (uint32)(speed);
    slot->launch_acceleration = (uint32)(limit);
    slot->guidance_acceleration = (uint32)(threshold);
    slot->launch_delay = (uint16)(c);
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_2(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *slot;
    uint32 value;
    FUNCTION_MARKER(0x80068C0C, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    slot->lifetime = (uint16)(0x53);
    slot->turn_limit = (uint16)(20);
    slot->launch_delay = (uint16)(1);
    slot->maximum_speed = (uint32)(0xA0000);
    value = slot->render_flags;
    slot->pose.angles[2] = (uint16)(0);
    slot->guide_kind = (uint8)(0);
    slot->impact_kind = (uint8)(0);
    slot->launch_kind = (uint8)(0);
    slot->launch_acceleration = (uint32)(0x5A00);
    slot->guidance_acceleration = (uint32)(0x5A00);
    slot->render_flags = (uint8)(value | 0x18);
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_3(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *slot;
    uint32 value;
    FUNCTION_MARKER(0x80068C80, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    slot->event_bits = (uint16)(128);
    slot->model_kind = (uint8)(3);
    slot->guide_kind = (uint8)(2);
    slot->launch_kind = (uint8)(1);
    slot->lifetime = (uint16)(150);
    slot->turn_limit = (uint16)(18);
    slot->launch_delay = (uint16)(7);
    value = slot->render_flags;
    slot->maximum_speed = (uint32)(0x3E000);
    slot->impact_kind = (uint8)(0);
    slot->launch_acceleration = (uint32)(0x5A00);
    slot->guidance_acceleration = (uint32)(0x5A00);
    slot->render_flags = (uint8)(value | 0x18);
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_4(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    MISSION_ACTOR *pose_owner_target;
    uint32 actor_index_table;

    EFFECT_OBJECT *slot;
    uint32 value, distance;
    FUNCTION_MARKER(0x80068D0C, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    slot->event_bits = (uint16)(256);
    slot->model_kind = (uint8)(2);
    slot->guide_kind = (uint8)(3);
    slot->launch_kind = (uint8)(1);
    slot->lifetime = (uint16)(150);
    slot->turn_limit = (uint16)(30);
    slot->launch_delay = (uint16)(7);
    slot->launch_acceleration = (uint32)(0x1200);
    value = slot->render_flags;
    slot->maximum_speed = (uint32)(0x48000);
    slot->impact_kind = (uint8)(0);
    slot->guidance_acceleration = (uint32)(0xA00);
    slot->render_flags = (uint8)(value | 0x18);
    if (owner != 0xFFFFFFFF)
    {
        actor_index_table = 0;
        pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)(((owner & 63) << 2)) >> 2))));
        distance = game_measure_distance_gte(native_position->position, pose_owner_target->pose.position);
        if ((owner & 0x20) != 0)
            value = (uint32)((sint32)distance >> 2) + (uint32)((sint32)distance >> 1);
        else
        {
            value = (distance << 4) - distance;
            value = (value << 2) + distance;
            value = ((value << 2) - distance) << 2;
            value = (sint32)value >> 10;
        }
        slot->split_distance = (uint32)(value);
        slot->distance = (uint32)(distance);
    }
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_5(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *slot;
    uint32 x, y, z, value;
    FUNCTION_MARKER(0x80068A58, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    x = slot->velocity[0];
    slot->event_bits = (uint16)(512);
    slot->model_kind = (uint8)(4);
    slot->launch_kind = (uint8)(2);
    slot->impact_kind = (uint8)(3);
    slot->lifetime = (uint16)(240);
    slot->area_radius = (uint32)(1024);
    slot->delay_divisor = (uint32)(192);
    y = slot->velocity[1];
    slot->guide_kind = (uint8)(0);
    slot->velocity[0] = (uint32)(x + (uint32)((sint32)x >> 1));
    z = slot->velocity[2];
    slot->velocity[1] = (uint32)(y + (uint32)((sint32)y >> 1));
    value = slot->flags;
    slot->velocity[2] = (uint32)(z + (uint32)((sint32)z >> 1));
    slot->flags = (uint32)(value | 0x10);
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_6(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *slot;
    uint32 x, y, z, value;
    FUNCTION_MARKER(0x80068AFC, "ACE3.BIN");
    slot = effect_object_allocate(flags, owner, native_position, native_vector);
    if (slot == 0)
        return 0;
    x = slot->velocity[0];
    slot->event_bits = (uint16)(1024);
    slot->model_kind = (uint8)(4);
    slot->launch_kind = (uint8)(2);
    slot->impact_kind = (uint8)(1);
    slot->lifetime = (uint16)(240);
    slot->area_radius = (uint32)(1260);
    slot->delay_divisor = (uint32)(192);
    y = slot->velocity[1];
    slot->guide_kind = (uint8)(0);
    slot->velocity[0] = (uint32)(x + (uint32)((sint32)x >> 1));
    slot->velocity[1] = (uint32)(y + (uint32)((sint32)y >> 1));
    value = slot->flags;
    z = slot->velocity[2];
    value |= 0x10;
    slot->flags = (uint32)(value);
    slot->velocity[2] = (uint32)(z + (uint32)((sint32)z >> 1));
    slot->flags = (uint32)(value | 0x40);
    return slot;
}

EFFECT_OBJECT *effect_object_create_variant_7(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *effect;

    FUNCTION_MARKER(0x80068BA8, "ACE3.BIN");
    effect = effect_object_allocate(flags, owner, native_position, native_vector);
    if (effect == 0)
        return 0;
    effect->event_bits = (uint16)(4096);
    effect->launch_kind = (uint8)(3);
    effect->launch_acceleration = (uint32)(5120);
    effect->guidance_acceleration = (uint32)(5120);
    effect->guide_kind = (uint8)(0);
    effect->impact_kind = (uint8)(0);
    effect->maximum_speed = (uint32)(0x12C000);
    effect->lifetime = (uint16)(90);
    return effect;
}

EFFECT_OBJECT *effect_object_create_variant_8(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *effect;
    uint32 value;
    FUNCTION_MARKER(0x800689E8, "ACE3.BIN");
    effect = effect_object_allocate(flags, owner, native_position, native_vector);
    if (effect == 0)
        return 0;
    effect->event_bits = (uint16)(8192);
    effect->model_kind = (uint8)(4);
    effect->launch_kind = (uint8)(2);
    effect->lifetime = (uint16)(300);
    effect->area_radius = (uint32)(400);
    value = effect->flags;
    effect->guide_kind = (uint8)(0);
    effect->impact_kind = (uint8)(0);
    effect->delay_divisor = (uint32)(192);
    effect->flags = (uint32)(value | 0x10);
    return effect;
}

EFFECT_OBJECT *effect_object_create_variant_9(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *effect;
    uint32 value, seed, random;
    FUNCTION_MARKER(0x80068E28, "ACE3.BIN");
    effect = effect_object_allocate(flags, owner, native_position, native_vector);
    if (effect == 0)
        return 0;
    value = effect->render_flags;
    effect->event_bits = (uint16)(256);
    effect->model_kind = (uint8)(2);
    effect->guide_kind = (uint8)(4);
    effect->impact_kind = (uint8)(0);
    effect->launch_kind = (uint8)(0);
    effect->render_flags = (uint8)(value | 24);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    effect->launch_delay = (uint16)(random & 3);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = effect->pose.angles[2];
    effect->pose.angles[2] = (uint16)(value - 512 + (random & 1023));
    if ((owner & 32) != 0)
    {
        effect->lifetime = (uint16)(150);
        effect->turn_limit = (uint16)(24);
        effect->launch_acceleration = (uint32)(4096);
        effect->guidance_acceleration = (uint32)(4096);
        effect->area_radius = (uint32)(640);
        value = effect->flags;
        effect->maximum_speed = (uint32)(0x3C000);
        effect->delay_divisor = (uint32)(192);
        effect->flags = (uint32)(value | 16);
    }
    else
    {
        effect->lifetime = (uint16)(240);
        effect->turn_limit = (uint16)(48);
        effect->launch_acceleration = (uint32)(512);
        effect->maximum_speed = (uint32)(0x6E000);
        effect->guidance_acceleration = (uint32)(3072);
    }
    return effect;
}

EFFECT_OBJECT *effect_object_create_variant_10(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *effect;
    uint32 value;
    FUNCTION_MARKER(0x80068F34, "ACE3.BIN");
    effect = effect_object_allocate(flags, owner, native_position, native_vector);
    if (effect == 0)
        return 0;
    effect->event_bits = (uint16)(2048);
    effect->launch_kind = (uint8)(4);
    effect->impact_kind = (uint8)(2);
    effect->area_radius = (uint32)(16);
    value = effect->flags;
    effect->model_kind = (uint8)(0);
    effect->lifetime = (uint16)(0);
    effect->velocity[0] = (uint32)(0);
    effect->velocity[1] = (uint32)(0);
    effect->velocity[2] = (uint32)(0);
    effect->delay_divisor = (uint32)(1);
    effect->flags = (uint32)(value | 0x20);
    return effect;
}

EFFECT_OBJECT *effect_object_create_variant_11(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    EFFECT_OBJECT *effect;
    uint32 value;
    FUNCTION_MARKER(0x80068FA8, "ACE3.BIN");
    effect = effect_object_allocate(flags, owner, native_position, native_vector);
    if (effect == 0)
        return 0;
    effect->event_bits = (uint16)(0x4000);
    effect->launch_kind = (uint8)(4);
    effect->impact_kind = (uint8)(3);
    value = effect->flags;
    effect->model_kind = (uint8)(0);
    effect->lifetime = (uint16)(0);
    effect->velocity[0] = (uint32)(0);
    effect->velocity[1] = (uint32)(0);
    effect->velocity[2] = (uint32)(0);
    effect->area_radius = (uint32)(1);
    effect->delay_divisor = (uint32)(1);
    effect->flags = (uint32)(value | 0x20);
    return effect;
}

intptr effect_object_launch_1(EFFECT_OBJECT *state)
{
    uint32 phase, raw, counter, delay, flags, value, a, b, result, limit, angle, maximum, magnitude;
    FUNCTION_MARKER(0x8006903C, "ACE3.BIN");
    phase = (sint16)state->phase;
    raw = state->phase;
    if (phase == 0 || phase == 1)
    {
        if (phase == 0)
            state->phase = (uint16)(raw + 1);
        counter = (sint16)state->ticks;
        delay = (sint16)state->launch_delay;
        if ((sint32)delay < (sint32)counter)
        {
            flags = state->flags & 1;
            if (flags != 0)
            {
                value = state->phase;
                state->phase = (uint16)(value + 1);
            }
            else
            {
                state->kind = (uint8)(1);
                state->phase = (uint16)(0);
                state->ticks = (uint16)(0xFFFF);
            }
        }
        a = state->acceleration;
        b = state->launch_acceleration;
        value = state->initial_speed;
        a += b;
        flags = state->flags;
        state->acceleration = (uint32)(a);
        state->local_velocity[2] = (uint32)(a + value);
        state->flags = (uint32)(flags | 0x4000);
    }
    result = state->flags & 8;
    if (result == 0)
        return result;
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->pose.angles[2];
    raw = state->pose.angles[2];
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        state->delta_angles[2] = (uint16)(maximum);
        if ((sint32)angle >= 0)
            state->delta_angles[2] = (uint16)(0 - maximum);
    }
    else
    {
        state->delta_angles[2] = (uint16)(0 - raw);
        flags = state->flags;
        state->flags = (uint32)(flags & 0xFFFFFFF7);
    }
    result = state->flags | 0x2000;
    state->flags = (uint32)(result);
    return result;
}

intptr effect_object_launch_2(EFFECT_OBJECT *state)
{
    uint32 phase, raw, counter, delay, flags, value, a, b, result, limit, angle, maximum, magnitude;
    FUNCTION_MARKER(0x80069158, "ACE3.BIN");
    phase = (sint16)state->phase;
    raw = state->phase;
    if (phase == 0 || phase == 1)
    {
        if (phase == 0)
            state->phase = (uint16)(raw + 1);
        counter = (sint16)state->ticks;
        delay = (sint16)state->launch_delay;
        if ((sint32)delay < (sint32)counter)
        {
            flags = state->flags & 1;
            if (flags != 0)
            {
                value = state->phase;
                state->phase = (uint16)(value + 1);
            }
            else
            {
                state->kind = (uint8)(1);
                state->phase = (uint16)(0);
                state->ticks = (uint16)(0xFFFF);
            }
            state->local_velocity[1] = (uint32)(0);
        }
        counter = (sint16)state->ticks;
        if ((sint32)counter < 7)
        {
            value = state->local_velocity[1];
            state->local_velocity[1] = (uint32)(value + 669);
        }
        else
        {
            a = state->local_velocity[2];
            b = state->maximum_speed;
            if ((sint32)a < (sint32)b)
            {
                a = state->acceleration;
                b = state->launch_acceleration;
                state->acceleration = (uint32)(a + b);
            }
            a = state->acceleration;
            b = state->initial_speed;
            state->local_velocity[2] = (uint32)(a + b);
        }
        flags = state->flags;
        state->flags = (uint32)(flags | 0x4000);
    }
    result = state->flags & 8;
    if (result == 0)
        return result;
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->pose.angles[2];
    raw = state->pose.angles[2];
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        state->delta_angles[2] = (uint16)(maximum);
        if ((sint32)angle >= 0)
            state->delta_angles[2] = (uint16)(0 - maximum);
    }
    else
    {
        state->delta_angles[2] = (uint16)(0 - raw);
        flags = state->flags;
        state->flags = (uint32)(flags & 0xFFFFFFF7);
    }
    result = state->flags | 0x2000;
    state->flags = (uint32)(result);
    return result;
}

intptr effect_object_launch_3(EFFECT_OBJECT *state)
{
    uint32 phase, value, angle;
    FUNCTION_MARKER(0x80069D18, "ACE3.BIN");
    phase = (sint16)state->phase;
    value = state->phase + 1;
    if (phase != 0)
    {
        if (phase != 1)
            return 1;
    }
    else
        state->phase = (uint16)(value);
    value = state->velocity[1];
    angle = (sint16)state->pose.angles[0];
    state->velocity[1] = (uint32)(value + 669);
    value = state->pose.angles[0] - 32;
    if ((sint32)angle >= -2047)
        state->pose.angles[0] = (uint16)(value);
    return value;
}

intptr effect_object_launch_4(EFFECT_OBJECT *state)
{
    uint32 phase, value, a, b, flags, result;
    FUNCTION_MARKER(0x80069F48, "ACE3.BIN");
    phase = (sint16)state->phase;
    value = state->phase + 1;
    if (phase != 0)
    {
        if (phase != 1)
            return 1;
    }
    else
        state->phase = (uint16)(value);
    a = state->local_velocity[2];
    b = state->maximum_speed;
    if ((sint32)a < (sint32)b)
    {
        a = state->acceleration;
        b = state->launch_acceleration;
        state->acceleration = (uint32)(a + b);
    }
    a = state->acceleration;
    b = state->initial_speed;
    flags = state->flags;
    result = a + b;
    state->local_velocity[2] = (uint32)(result);
    state->flags = (uint32)(flags | 0x4000);
    return result;
}

intptr effect_object_launch_5(EFFECT_OBJECT *state)
{
    uint32 phase, result;
    FUNCTION_MARKER(0x80069D6C, "ACE3.BIN");
    phase = (sint16)state->phase;
    result = state->phase + 1;
    if (phase == 0)
        state->phase = (uint16)(result);
    return result;
}

intptr effect_object_guide_1(EFFECT_OBJECT *state)
{
    uint32 divisor, scale, numerator, flags, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x800694CC, "ACE3.BIN");
    divisor = state->local_velocity[2];
    scale = 1;
    if (divisor != 0)
    {
        numerator = state->distance << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
    }
    flags = state->flags;
    if ((flags & 0x100) != 0)
        scale = 0;

    x = state->target_velocity[0] * scale;
    y = state->target_velocity[1] * scale;
    value = state->target_position[0];
    z = state->target_velocity[2] * scale;
    target[0] = value + x;
    value = state->target_position[1];
    target[1] = value + y;
    value = state->target_position[2];
    target[2] = value + z;
    distance = game_pose_target_angles(state->delta_angles, target, state->pose.position, state->pose.angles);
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->delta_angles[0];
    raw = state->delta_angles[0];
    state->distance = (uint32)(distance);
    state->delta_angles[2] = (uint16)(0);
    clamped = 0 - limit;
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    state->delta_angles[0] = (uint16)(clamped);
    clamped = 0 - limit;
    angle = (sint16)state->delta_angles[1];
    raw = state->delta_angles[1];
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    speed = state->local_velocity[2];
    maximum = state->maximum_speed;
    state->delta_angles[1] = (uint16)(clamped);
    if ((sint32)speed < (sint32)maximum)
    {
        acceleration = state->acceleration;
        value = state->guidance_acceleration;
        state->acceleration = (uint32)(acceleration + value);
    }
    acceleration = state->acceleration;
    value = state->initial_speed;
    flags = state->flags;
    result = acceleration + value;
    flags |= 0x5000;
    state->flags = (uint32)(flags);
    state->local_velocity[2] = (uint32)(result);
    if ((flags & 8) == 0)
        return result;
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->pose.angles[2];
    raw = state->pose.angles[2];
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        state->delta_angles[2] = (uint16)(maximum);
        if ((sint32)angle >= 0)
            state->delta_angles[2] = (uint16)(0 - maximum);
    }
    else
    {
        state->delta_angles[2] = (uint16)(0 - raw);
        value = state->flags;
        state->flags = (uint32)(value & 0xFFFFFFF7);
    }
    result = state->flags | 0x2000;
    state->flags = (uint32)(result);
    return result;
}

intptr effect_object_guide_2(EFFECT_OBJECT *state)
{
    uint32 divisor, scale, numerator, flags, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x800692C4, "ACE3.BIN");
    divisor = state->local_velocity[2];
    scale = 1;
    if (divisor != 0)
    {
        numerator = state->distance << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
    }
    flags = state->flags;
    if ((flags & 0x100) != 0)
        scale = 0;

    x = state->target_velocity[0] * scale;
    y = state->target_velocity[1] * scale;
    value = state->target_position[0];
    z = state->target_velocity[2] * scale;
    target[0] = value + x;
    value = state->target_position[1];
    target[1] = value + y;
    value = state->target_position[2];
    target[2] = value + z;
    distance = game_pose_target_angles(state->delta_angles, target, state->pose.position, state->pose.angles);
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->delta_angles[0];
    raw = state->delta_angles[0];
    state->distance = (uint32)(distance);
    state->delta_angles[2] = (uint16)(0);
    clamped = 0 - limit;
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    state->delta_angles[0] = (uint16)(clamped);
    clamped = 0 - limit;
    angle = (sint16)state->delta_angles[1];
    raw = state->delta_angles[1];
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    speed = state->local_velocity[2];
    maximum = state->maximum_speed;
    state->delta_angles[1] = (uint16)(clamped);
    if ((sint32)speed < (sint32)maximum)
    {
        acceleration = state->acceleration;
        value = state->guidance_acceleration;
        state->acceleration = (uint32)(acceleration + value);
    }
    acceleration = state->acceleration;
    value = state->initial_speed;
    flags = state->flags;
    result = acceleration + value;
    flags |= 0x5000;
    state->flags = (uint32)(flags);
    state->local_velocity[2] = (uint32)(result);
    if ((flags & 8) == 0)
        return result;
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->pose.angles[2];
    raw = state->pose.angles[2];
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        state->delta_angles[2] = (uint16)(maximum);
        if ((sint32)angle >= 0)
            state->delta_angles[2] = (uint16)(0 - maximum);
    }
    else
    {
        state->delta_angles[2] = (uint16)(0 - raw);
        value = state->flags;
        state->flags = (uint32)(value & 0xFFFFFFF7);
    }
    result = state->flags | 0x2000;
    state->flags = (uint32)(result);
    return result;
}

intptr effect_object_guide_3(EFFECT_OBJECT *state)
{
    const uint16 *map;
    uint32 result, value, magnitude, dx, dy, dz, px, py, pz, x, y, z, limit, ix, iz, tile, height, product, lower, upper, angle;
    FUNCTION_MARKER(0x800696D4, "ACE3.BIN");
    effect_object_guide_1(state);
    value = state->distance;
    result = (sint32)value < 4097;
    if (result != 0)
        return result;
    value = (sint16)state->pose.angles[2];
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = (sint32)magnitude < 227;
    if (result == 0)
        return result;
    dx = state->velocity[0];
    dy = state->velocity[1];
    px = state->pose.position[0];
    x = dx * 20 + px;
    py = state->pose.position[1];
    y = dy * 20 + py;
    dz = state->velocity[2];
    map = game_runtime.scene_assets.tile_map;
    state->delta_angles[0] = (uint16)(0);
    pz = state->pose.position[2];
    z = dz * 20 + pz;
    limit = game_runtime.mission_bounds.min[0];
    if ((sint32)x < (sint32)limit)
        x = limit;
    limit = game_runtime.mission_bounds.max[0];
    if ((sint32)limit < (sint32)x)
        x = limit;
    limit = game_runtime.mission_bounds.min[1];
    if ((sint32)y < (sint32)limit)
        y = limit;
    limit = game_runtime.mission_bounds.max[1];
    if ((sint32)limit < (sint32)y)
        y = limit;
    limit = game_runtime.mission_bounds.min[2];
    if ((sint32)z < (sint32)limit)
        z = limit;
    limit = game_runtime.mission_bounds.max[2];
    if ((sint32)limit < (sint32)z)
        z = limit;
    ix = (uint32)((sint32)x >> 23) + 128;
    iz = (uint32)((sint32)z >> 23) + 128;
    if ((sint32)ix < 0)
        ix &= 7;
    if ((sint32)iz < 0)
        iz &= 7;
    if ((sint32)ix >= 256)
        ix = (ix & 7) + 248;
    if ((sint32)iz >= 256)
        iz = (iz & 7) + 248;
    tile = map[ix + (iz << 8)] & 511;
    result = tile << 2;
    if (tile == 0)
        return result;
    limit = (sint16)state->turn_limit;
    const SCENE_TERRAIN_MODEL_HEADER *header = terrain_model_header(&game_runtime.scene_assets.terrain_models, tile);
    height = header->height;
    product = ((uint32)((sint32)y >> 12) + 400 - height) * (limit << 2);
    value = (sint32)product / 200;
    state->delta_angles[0] = (uint16)(value);
    lower = (0 - (limit << 2)) << 2;
    if ((sint32)(sint16)value < (sint32)lower)
        state->delta_angles[0] = (uint16)(lower);
    angle = (sint16)state->delta_angles[0];
    upper = limit << 4;
    result = (sint32)upper < (sint32)angle;
    if (result != 0)
        state->delta_angles[0] = (uint16)(upper);
    return result;
}

intptr effect_object_guide_4(EFFECT_OBJECT *state)
{
    NATIVE_POSE pose;
    intptr result;
    uint32 distance, threshold, flags, kind, owner;
    FUNCTION_MARKER(0x80069900, "ACE3.BIN");
    effect_object_guide_1(state);
    distance = state->distance;
    threshold = state->split_distance;
    result = (sint32)distance < (sint32)threshold;
    if (result == 0)
        return result;
    pose.position[0] = state->pose.position[0];
    pose.position[1] = state->pose.position[1];
    pose.position[2] = state->pose.position[2];
    pose.angles[0] = state->pose.angles[0];
    pose.angles[1] = state->pose.angles[1];
    pose.angles[2] = state->pose.angles[2];
    flags = state->target_flags & 32;
    if (flags != 0)
    {
        pose.angles[0] = (uint16)(pose.angles[0] + 682);
        pose.angles[1] = (uint16)(pose.angles[1] + 682);
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[1] = (uint16)(pose.angles[1] - 1365);
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[0] = (uint16)(pose.angles[0] - 682);
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[1] = (uint16)(pose.angles[1] + 1365);
    }
    else
    {
        pose.angles[2] = 0;
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[0] = (uint16)(pose.angles[0] + 1137);
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[0] = (uint16)(pose.angles[0] - 2275);
        pose.angles[1] = (uint16)(pose.angles[1] + 2048);
        kind = state->source_flags;
        owner = state->target_flags;
        effect_object_create_variant_9(kind, owner, &pose, state->velocity);
        pose.angles[1] = (uint16)(pose.angles[1] - 4096);
    }
    kind = state->source_flags;
    owner = state->target_flags;
    result = (intptr)effect_object_create_variant_9(kind, owner, &pose, state->velocity);
    state->lifetime = (uint16)(0);
    return result;
}

intptr effect_object_guide_5(EFFECT_OBJECT *state)
{
    uint32 divisor, scale, numerator, flags, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x80069A9C, "ACE3.BIN");
    divisor = state->local_velocity[2];
    scale = 1;
    if (divisor != 0)
    {
        numerator = state->distance << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
    }

    x = state->target_velocity[0] * scale;
    y = state->target_velocity[1] * scale;
    value = state->target_position[0];
    z = state->target_velocity[2] * scale;
    target[0] = value + x;
    value = state->target_position[1];
    target[1] = value + y;
    value = state->target_position[2];
    target[2] = value + z;
    distance = game_pose_target_angles(state->delta_angles, target, state->pose.position, state->pose.angles);
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->delta_angles[0];
    raw = state->delta_angles[0];
    state->distance = (uint32)(distance);
    state->delta_angles[2] = (uint16)(0);
    clamped = 0 - limit;
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    state->delta_angles[0] = (uint16)(clamped);
    clamped = 0 - limit;
    angle = (sint16)state->delta_angles[1];
    raw = state->delta_angles[1];
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    speed = state->local_velocity[2];
    maximum = state->maximum_speed;
    state->delta_angles[1] = (uint16)(clamped);
    if ((sint32)speed < (sint32)maximum)
    {
        acceleration = state->acceleration;
        value = state->guidance_acceleration;
        state->acceleration = (uint32)(acceleration + value);
    }
    acceleration = state->acceleration;
    value = state->initial_speed;
    flags = state->flags;
    result = acceleration + value;
    flags |= 0x5000;
    state->flags = (uint32)(flags);
    state->local_velocity[2] = (uint32)(result);
    if ((flags & 8) == 0)
        return result;
    limit = (sint16)state->turn_limit;
    angle = (sint16)state->pose.angles[2];
    raw = state->pose.angles[2];
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        state->delta_angles[2] = (uint16)(maximum);
        if ((sint32)angle >= 0)
            state->delta_angles[2] = (uint16)(0 - maximum);
    }
    else
    {
        state->delta_angles[2] = (uint16)(0 - raw);
        value = state->flags;
        state->flags = (uint32)(value & 0xFFFFFFF7);
    }
    result = state->flags | 0x2000;
    state->flags = (uint32)(result);
    return result;
}

intptr effect_object_impact_1(EFFECT_OBJECT *state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069C8C, "ACE3.BIN");
    flags = state->flags;
    if (flags & 4)
    {
        effect_emit_surface_hit(0, state->pose.position, state->impact_velocity, state->normal);
        return sound_play_spatial(29, 0, state->pose.position);
    }
    effect_emit_debris(0, state->pose.position, state->impact_velocity);
    result = (state->target_flags & 63) < 2;
    if (result == 0)
        return result;
    return sound_play_spatial(28, 0, state->pose.position);
}

intptr effect_object_impact_2(EFFECT_OBJECT *state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069D88, "ACE3.BIN");
    flags = state->flags & 4;
    effect_emit_burst(0, state->pose.position);
    if (flags != 0)
        return sound_play_spatial(37, 0, state->pose.position);
    result = (state->target_flags & 63) < 2;
    if (result == 0)
        return result;
    return sound_play_spatial(28, 0, state->pose.position);
}

intptr effect_object_impact_3(EFFECT_OBJECT *state)
{
    MISSION_ACTOR *pose_owner_source;
    uint32 actor_index_table;

    uint32 result, value;
    FUNCTION_MARKER(0x80069E10, "ACE3.BIN");
    result = state->flags & 1;
    if (result != 0)
        return result;
    value = state->target_flags;
    actor_index_table = 0;
    pose_owner_source = mission_actor((actor_index_table + ((uint32)((sint32)((value & 63) * 4) >> 2))));
    value = pose_owner_source->pose.position[0];
    state->pose.position[0] = (uint32)(value);
    value = pose_owner_source->pose.position[1];
    state->pose.position[1] = (uint32)(value);
    value = pose_owner_source->pose.position[2];
    state->pose.position[2] = (uint32)(value);
    value = pose_owner_source->pose.angles[0];
    state->pose.angles[0] = (uint16)(value);
    value = pose_owner_source->pose.angles[1];
    state->pose.angles[1] = (uint16)(value);
    value = pose_owner_source->pose.angles[2];
    state->pose.angles[2] = (uint16)(value);
    effect_emit_surface_hit(0, state->pose.position, state->impact_velocity, state->normal);
    return sound_play_spatial(29, 0, state->pose.position);
}

intptr effect_object_impact_4(EFFECT_OBJECT *state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069EC0, "ACE3.BIN");
    flags = state->flags & 4;
    effect_emit_burst(1, state->pose.position);
    if (flags != 0)
        return sound_play_spatial(29, 0, state->pose.position);
    result = (state->target_flags & 63) < 2;
    if (result == 0)
        return result;
    return sound_play_spatial(28, 0, state->pose.position);
}

void effect_object_noop(void)
{
    FUNCTION_MARKER(0x80069FC0, "ACE3.BIN");
}

void effect_objects_run_modes(void)
{
    EFFECT_OBJECT *state;
    EFFECT_OBJECT_HANDLER target;
    uint32 mask, index, kind, selector, counter;
    FUNCTION_MARKER(0x80069FC8, "ACE3.BIN");
    mask = effect_objects.live;
    if (mask == 0)
        return;
    mask = ~mask;
    state = effect_objects.slot;
    for (index = 0; index < 32; ++index)
    {
        if ((mask & ((uint32)1 << index)) == 0)
        {
            kind = state->kind;
            if (kind == 0)
            {
                selector = state->launch_kind;
                if (selector >= 5)
                    abort();
                target = effect_object_handlers[0][selector];
            }
            else if (kind == 1)
            {
                selector = state->guide_kind;
                if (selector >= 5)
                    abort();
                target = effect_object_handlers[1][selector];
            }
            else if (kind == 2)
            {
                selector = state->impact_kind;
                if (selector >= 5)
                    abort();
                target = effect_object_handlers[2][selector];
            }
            else
                target = 0;
            if (target != 0)
                target(state);
            else
                effect_object_noop();
            counter = state->ticks;
            state->ticks = (uint16)(counter + 1);
        }
        ++state;
    }
    return;
}

void effect_objects_collide(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *target;
    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_target;
    uint32 actor_index_objects;

    ACTOR_COLLISION_CANDIDATE candidates[MISSION_ACTOR_COUNT];
    uint32 contact[3], hit[3], initial_mask;
    EFFECT_OBJECT *queued_objects[9];
    MISSION_ACTOR *queued_targets[9];
    EFFECT_OBJECT *state;
    const uint16 *map;
    uint32 mask, count, queued, radius, flags, x, y, z, size, value, index, dx, dy, dz, ax, ay, az, range, radius_squared, filter, j, sx, sy, sz, sum, old_x, old_y, old_z, a, b, c, result, collision, material, nx, ny, nz, cells[4], k, cell, height;
    FUNCTION_MARKER(0x8006A11C, "ACE3.BIN");
    mask = effect_objects.live;
    queued = 0;
    initial_mask = mask;
    if (mask == 0)
        return;
    count = 0;
    actor_index_objects = 0;
    {
        ACTOR_COLLISION_PUBLICATION *published = (ACTOR_COLLISION_PUBLICATION *)getScratchAddr(0);
        for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
        {
            pose_owner_object = mission_actor((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
            flags = actor_traits_object->flags;
            if ((flags & 0x4200) != 0x4200)
                continue;
            x = pose_owner_object->pose.position[0];
            y = pose_owner_object->pose.position[1];
            z = pose_owner_object->pose.position[2];
            size = (sint16)actor_traits_object->radius[0];
            candidates[count].x = (uint32)((sint32)x >> 12);
            candidates[count].y = (uint32)((sint32)y >> 12);
            candidates[count].z = (uint32)((sint32)z >> 12);
            candidates[count].flags = flags;
            published[count].x = candidates[count].x;
            published[count].y = candidates[count].y;
            published[count].z = candidates[count].z;
            published[count].flags = candidates[count].flags;
            candidates[count].radius_times_three = size * 3;
            candidates[count].radius_squared = size * size;
            ++count;
        }
    }
    /* nested queries reuse SDK storage while their actor inputs stay immutable */
    const ACTOR_COLLISION_CANDIDATE *snapshot = candidates;
    mask = initial_mask;
    mask = ~mask;
    state = effect_objects.slot;
    for (index = 0; index < 32; ++index, ++state)
    {
        if ((mask & ((uint32)1 << index)) != 0)
            continue;
        dx = state->velocity[0];
        dy = state->velocity[1];
        dz = state->velocity[2];
        ax = (sint32)dx < 0 ? 0 - dx : dx;
        ay = (sint32)dy < 0 ? 0 - dy : dy;
        az = (sint32)dz < 0 ? 0 - dz : dz;
        range = (sint32)(ax + ay + az) >> 13;
        value = range * range;
        radius_squared = (sint32)value / 9;
        value = state->pose.position[1];
        y = (sint32)(value + dy) >> 12;
        filter = state->source_flags & 0x1C0;
        value = state->pose.position[0];
        x = (sint32)(value + dx) >> 12;
        value = state->pose.position[2];
        z = (sint32)(value + dz) >> 12;
        if ((sint32)count <= 0)
            continue;
        filter = ~filter & 0x1C0;
        for (j = 0; j < count; ++j)
        {
            flags = snapshot[j].flags;
            if ((flags & filter) == 0)
                continue;
            sx = snapshot[j].x;
            sz = snapshot[j].z;
            dx = sx - x;
            dz = sz - z;
            ax = (sint32)dx < 0 ? 0 - dx : dx;
            sy = snapshot[j].y;
            az = (sint32)dz < 0 ? 0 - dz : dz;
            dy = sy - y;
            ay = (sint32)dy < 0 ? 0 - dy : dy;
            sum = ax + ay + az;
            radius = snapshot[j].radius_times_three + range;
            if ((sint32)radius < (sint32)sum)
                continue;
            sum = dx * dx + dy * dy + dz * dz;
            radius = snapshot[j].radius_squared + radius_squared;
            if ((sint32)radius < (sint32)sum)
                continue;
            actor_index_objects = 0;
            target = mission_actor((actor_index_objects + ((uint32)((sint32)((flags & 63) * 4) >> 2))));
            actor_traits_target = &target->traits;
            if ((flags & 0x1800) != 0)
            {
                queued_objects[queued] = state;
                queued_targets[queued] = target;
                if (queued < 8)
                    ++queued;
            }
            else
            {
                value = state->event_bits;
                a = actor_traits_target->events;
                actor_traits_target->events = (uint32)(a | value);
                value = state->source_flags;
                actor_traits_target->damage_source = (uint32)(value);
                a = state->velocity[0];
                b = state->velocity[1];
                c = state->velocity[2];
                state->pose.position[0] = (uint32)(x << 12);
                state->pose.position[1] = (uint32)(y << 12);
                state->pose.position[2] = (uint32)(z << 12);
                state->velocity[0] = (uint32)(0);
                state->velocity[1] = (uint32)(0);
                state->velocity[2] = (uint32)(0);
                state->impact_velocity[0] = (uint32)(a);
                state->impact_velocity[1] = (uint32)(b);
                state->impact_velocity[2] = (uint32)(c);
                value = state->flags;
                state->flags = (uint32)(value | 2);
                state->lifetime = (uint16)(0);
                state->kind = (uint8)(2);
                state->phase = (uint16)(0);
                state->ticks = (uint16)(0);
            }
        }
    }
    for (index = 0; index < queued; ++index)
    {
        target = queued_targets[index];
        actor_traits_target = &target->traits;
        flags = actor_traits_target->flags;
        state = queued_objects[index];
        if ((flags & 0x1020) == 0x1020 && (actor_traits_target->render_flags & 2) == 0)
            result = collision_trace_actor(target, contact, state->pose.position, state->velocity);
        else
        {
            contact[0] = state->pose.position[0];
            contact[1] = state->pose.position[1];
            contact[2] = state->pose.position[2];
            result = 0;
        }
        if (result == 0x7FFFFFFF)
            continue;
        value = state->event_bits;
        a = actor_traits_target->events;
        actor_traits_target->events = (uint32)(a | value);
        value = state->source_flags;
        actor_traits_target->damage_source = (uint32)(value);
        x = contact[0];
        a = state->velocity[0];
        b = state->velocity[1];
        c = state->velocity[2];
        state->pose.position[0] = (uint32)(x);
        state->pose.position[1] = (uint32)(contact[1]);
        z = contact[2];
        flags = state->flags;
        state->impact_velocity[0] = (uint32)(a);
        state->impact_velocity[1] = (uint32)(b);
        state->impact_velocity[2] = (uint32)(c);
        state->velocity[0] = (uint32)(0);
        state->velocity[1] = (uint32)(0);
        state->velocity[2] = (uint32)(0);
        state->lifetime = (uint16)(0);
        state->phase = (uint16)(0);
        state->ticks = (uint16)(0);
        state->flags = (uint32)(flags | 2);
        state->kind = (uint8)(2);
        state->pose.position[2] = (uint32)(z);
    }
    state = effect_objects.slot;
    map = game_runtime.scene_assets.tile_map;
    for (index = 0; index < 32; ++index, ++state)
    {
        if ((~initial_mask & ((uint32)1 << index)) != 0)
            continue;
        collision = 0;
        material = 0;
        old_x = state->pose.position[0];
        old_y = state->pose.position[1];
        dx = state->velocity[0];
        dy = state->velocity[1];
        old_z = state->pose.position[2];
        x = dx + old_x;
        y = dy + old_y;
        dz = state->velocity[2];
        value = game_runtime.mission_bounds.min[0];
        z = dz + old_z;
        if ((sint32)x < (sint32)value)
        {
            collision = 1;
            material = 4;
            x = value;
        }
        value = game_runtime.mission_bounds.max[0];
        if ((sint32)value < (sint32)x)
        {
            collision = 2;
            material = 4;
            x = value;
        }
        value = game_runtime.mission_bounds.min[1];
        if ((sint32)y < (sint32)value)
        {
            collision = 3;
            material = 3;
            y = value;
        }
        value = game_runtime.mission_bounds.max[1];
        if ((sint32)value < (sint32)y)
        {
            collision = 4;
            material = 1;
            y = value;
        }
        value = game_runtime.mission_bounds.min[2];
        if ((sint32)z < (sint32)value)
        {
            collision = 5;
            material = 4;
            z = value;
        }
        value = game_runtime.mission_bounds.max[2];
        if ((sint32)value < (sint32)z)
        {
            collision = 6;
            material = 4;
            z = value;
        }
        if (collision != 0)
        {
            flags = state->flags;
            state->flags = (uint32)(flags | 4);
            state->lifetime = (uint16)(0);
            state->pose.position[0] = (uint32)(x);
            state->pose.position[1] = (uint32)(y);
            state->pose.position[2] = (uint32)(z);
            nx = scene_bounds_normals[collision - 1][0];
            c = state->velocity[2];
            state->normal[0] = (uint16)(nx);
            ny = scene_bounds_normals[collision - 1][1];
            state->normal[1] = (uint16)(ny);
            nz = scene_bounds_normals[collision - 1][2];
            a = state->velocity[0];
            b = state->velocity[1];
            game_runtime.collision_result.flags = (uint32)(material);
            state->material = (uint32)(material);
            state->impact_velocity[2] = (uint32)(c);
            state->velocity[0] = (uint32)(0);
            state->velocity[1] = (uint32)(0);
            state->velocity[2] = (uint32)(0);
            state->phase = (uint16)(0);
            state->ticks = (uint16)(0);
            state->impact_velocity[0] = (uint32)(a);
            state->impact_velocity[1] = (uint32)(b);
            state->kind = (uint8)(2);
            state->normal[2] = (uint16)(nz);
            continue;
        }
        cells[0] = (uint32)((sint32)old_x >> 23) + 128;
        cells[1] = (uint32)((sint32)old_z >> 23) + 128;
        cells[2] = (uint32)((sint32)x >> 23) + 128;
        cells[3] = (uint32)((sint32)z >> 23) + 128;
        for (k = 0; k < 4; ++k)
        {
            if ((sint32)cells[k] < 0)
                cells[k] &= 7;
        }
        for (k = 0; k < 4; ++k)
        {
            if ((sint32)cells[k] >= 256)
                cells[k] = (cells[k] & 7) + 248;
        }
        cell = cells[0] + (cells[1] << 8);
        contact[0] = x;
        contact[1] = y;
        contact[2] = z;
        if (cell == cells[2] + (cells[3] << 8))
        {
            value = map[cell] & 511;
            if (value == 0)
                continue;
            const SCENE_TERRAIN_MODEL_HEADER *header = terrain_model_header(&game_runtime.scene_assets.terrain_models, value);
            height = header->height;
            if (((sint32)old_y >> 12) < (sint32)height && ((sint32)y >> 12) < (sint32)height)
                continue;
            result = collision_query_tile(state->pose.position, contact, hit);
        }
        else
            result = collision_query_segment(state->pose.position, contact, hit);
        if (result == 0x7FFFFFFF)
            continue;
        flags = state->flags;
        state->flags = (uint32)(flags | 4);
        state->lifetime = (uint16)(0);
        x = hit[0];
        a = state->velocity[0];
        c = state->velocity[2];
        state->pose.position[0] = (uint32)(x);
        state->pose.position[1] = (uint32)(hit[1]);
        z = hit[2];
        b = state->velocity[1];
        state->impact_velocity[0] = (uint32)(a);
        state->impact_velocity[2] = (uint32)(c);
        state->velocity[0] = (uint32)(0);
        state->velocity[1] = (uint32)(0);
        state->velocity[2] = (uint32)(0);
        state->impact_velocity[1] = (uint32)(b);
        state->pose.position[2] = (uint32)(z);
        nx = game_runtime.collision_result.normal[0];
        state->normal[0] = (uint16)(nx);
        ny = game_runtime.collision_result.normal[1];
        state->normal[1] = (uint16)(ny);
        material = game_runtime.collision_result.flags;
        nz = game_runtime.collision_result.normal[2];
        state->phase = (uint16)(0);
        state->ticks = (uint16)(0);
        state->material = (uint32)(material);
        state->kind = (uint8)(2);
        state->normal[2] = (uint16)(nz);
    }
    return;
}

void effect_objects_advance(void)
{
    EFFECT_OBJECT *record;
    uint32 mask, index, a, b, c, d, e, f;
    FUNCTION_MARKER(0x8006ABB8, "ACE3.BIN");
    mask = effect_objects.live;
    if (mask == 0)
        return;
    record = effect_objects.slot;
    for (index = 0; index < 32; ++index, ++record)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        a = record->pose.position[0];
        b = record->velocity[0];
        c = record->velocity[1];
        d = record->velocity[2];
        record->pose.position[0] = (uint32)(a + b);
        e = record->pose.position[1];
        f = record->pose.position[2];
        record->pose.position[1] = (uint32)(e + c);
        record->pose.position[2] = (uint32)(f + d);
    }
    return;
}

void effect_objects_expire(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_base;

    EFFECT_OBJECT *record;
    EFFECT_OBJECT *link;
    uint32 mask, index, bit, flags, a, b, value, owner, source_index;
    FUNCTION_MARKER(0x8006AC34, "ACE3.BIN");
    mask = effect_objects.live;
    if (mask == 0)
        return;
    record = effect_objects.slot;
    link = effect_objects.slot;
    for (index = 0; index < 32; ++index, ++record, ++link)
    {
        bit = 1u << index;
        if ((mask & bit) == 0)
            continue;
        flags = record->flags;
        if (flags & 0x2000)
        {
            a = link->pose.angles[2];
            b = link->delta_angles[2];
            flags &= 0xFFFFDFFF;
            link->pose.angles[2] = (uint16)(a + b);
        }
        if (flags & 0x1000)
        {
            game_angles_apply_delta(record->delta_angles, record->pose.angles);
            flags &= 0xFFFFEFFF;
        }
        if (flags & 0x4000)
        {
            game_rotate_vector(record->pose.angles, record->local_velocity, record->velocity);
            flags &= 0xFFFFBFFF;
        }
        if (flags & 0x40)
            effect_emit_trail(0, record->pose.position, record->velocity);
        if (flags & 0x80)
            effect_emit_trail(1, record->pose.position, record->velocity);
        record->flags = (uint32)(flags);
        value = link->lifetime - 1;
        link->lifetime = (uint16)(value);
        if ((sint32)(value << 16) >= 0)
        {
            if (link->source_flags & 0x2000)
                mission_replay_record_effect_pose(index, (sint16)(link->model_kind | ((uint32)link->render_flags << 8)), &record->pose);
            continue;
        }
        if (record->flags & 0x10)
            effect_object_schedule_area(record, link->area_radius);
        if (record->flags & 0x20)
            effect_event_schedule_from_object(record);
        if (link->source_flags & 0x2000)
            mission_replay_record_effect_hide(index);
        value = link->source_flags;
        source_index = value & 0x3F;
        value = effect_actor_tracking.source_count[source_index];
        effect_actor_tracking.source_count[source_index] = (uint8)(value - 1);
        owner = link->target_flags;
        mask &= ~bit;
        if (owner == 0xFFFFFFFF)
            continue;
        value = effect_actor_tracking.target_count[owner & 0x3F] - 1;
        effect_actor_tracking.target_count[owner & 0x3F] = (uint8)value;
        if (value & 0xFF)
            continue;
        value = effect_actor_tracking.target_mask[(owner & 0x20) ? 1 : 0];
        effect_actor_tracking.target_mask[(owner & 0x20) ? 1 : 0] = value & ~(1u << (owner & 31));
        if (record->flags & 2)
            continue;
        actor_index_base = 0;
        actor_traits_object = mission_actor_traits((actor_index_base + ((uint32)((sint32)((owner & 0x3F) * 4) >> 2))));
        flags = actor_traits_object->events;
        if ((flags & 0x7FC0) == 0)
        {
            actor_traits_object->events = (uint32)(flags | 0x8000);
            value = link->source_flags;
            actor_traits_object->damage_source = (uint32)(value);
        }
    }
    effect_objects.live = (uint32)(mask);
    return;
}

void effect_objects_update(void)
{
    FUNCTION_MARKER(0x8006AF20, "ACE3.BIN");
    effect_objects_collide();
    effect_objects_advance();
    effect_objects_run_modes();
    effect_objects_expire();
}

void effect_object_schedule_area(EFFECT_OBJECT *state, uint32 value)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 bound, x, y, flags, z, mask, i, object_flags, dx, dy, dz, radius, distance, divisor, quotient, mode, parameter;
    FUNCTION_MARKER(0x8006A978, "ACE3.BIN");
    bound = value * 3;
    x = state->pose.position[0];
    y = state->pose.position[1];
    flags = state->source_flags;
    z = state->pose.position[2];
    mask = flags & 0x1C0;
    for (i = 0; i < 64; ++i)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((i << 2)) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((i << 2)) >> 2))));
        object_flags = actor_traits_object->flags;
        if ((object_flags & 0x4200) != 0x4200 || (object_flags & mask) != 0)
            continue;
        dx = pose_owner_object->pose.position[0];
        dy = pose_owner_object->pose.position[1];
        dz = pose_owner_object->pose.position[2];
        dx = (sint32)(dx - x) >> 12;
        dy = (sint32)(dy - y) >> 12;
        dz = (sint32)(dz - z) >> 12;
        if ((sint32)dx < 0)
            dx = 0 - dx;
        if ((sint32)dy < 0)
            dy = 0 - dy;
        if ((sint32)dz < 0)
            dz = 0 - dz;
        radius = (sint16)actor_traits_object->radius[0];
        if ((sint32)bound < (sint32)(dx + dy + dz + radius))
            continue;
        distance = game_measure_distance_gte(pose_owner_object->pose.position, state->pose.position);
        if ((sint32)value < (sint32)distance)
            continue;
        divisor = state->delay_divisor;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && distance == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)distance / (sint32)divisor;
        mode = state->source_flags;
        parameter = state->event_bits;
        effect_event_schedule(mode, object_flags, parameter, quotient);
    }
    return;
}

void effect_object_publish_pose(uint32 index, uint32 kind, const NATIVE_POSE *native_source)
{
    EFFECT_OBJECT *record;
    uint32 flags, value;
    FUNCTION_MARKER(0x8006C6B8, "ACE3.BIN");
    if (index >= EFFECT_OBJECT_CAPACITY)
        abort();
    record = effect_objects.slot + index;
    flags = effect_objects.live;
    effect_objects.live = (uint32)(flags | (1u << (index & 31)));
    record->model_kind = (uint8)(kind);
    record->render_flags = (uint8)((sint32)kind >> 8);
    value = ((native_source->position[0]));
    record->pose.position[0] = (uint32)(value);
    value = ((native_source->position[1]));
    record->pose.position[1] = (uint32)(value);
    value = ((native_source->position[2]));
    record->pose.position[2] = (uint32)(value);
    value = ((native_source->angles[0]));
    record->pose.angles[0] = (uint16)(value);
    value = ((native_source->angles[1]));
    record->pose.angles[1] = (uint16)(value);
    value = ((native_source->angles[2]));
    record->pose.angles[2] = (uint16)(value);
    {
        (void)(value);
        return;
    }
}

void effect_object_hide(uint32 index)
{
    uint32 mask, flags;
    FUNCTION_MARKER(0x8006C740, "ACE3.BIN");
    mask = 1u << (index & 31);
    flags = effect_objects.live;
    mask = ~mask;
    effect_objects.live = (uint32)(flags & mask);
    {
        (void)(mask);
        return;
    }
}

void effect_objects_reset(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8006C530, "ACE3.BIN");
    effect_actor_tracking.target_mask[1] = 0;
    effect_actor_tracking.target_mask[0] = 0;
    for (index = 0; index < 64; ++index)
    {
        effect_actor_tracking.last_effect[index] = (uint8)255;
        effect_actor_tracking.source_count[index] = (uint8)0;
        effect_actor_tracking.target_count[index] = (uint8)0;
    }
    effect_events.spawn_count = 0;
    for (index = 0; index < 4; ++index)
        particles.live[3 - index] = 0;
    effect_objects.live = (uint32)(0);
    effect_events.delayed_mask = 0;
}

void effect_system_initialize(void)
{
    FUNCTION_MARKER(0x8006C588, "ACE3.BIN");
    /* preserve later arena phases until their address dependencies retire */
    arena_alloc(0x3564);
    effect_objects_reset();
}

void effect_system_update(void)
{
    FUNCTION_MARKER(0x8006C5B4, "ACE3.BIN");
    effect_spawn_process();
    particle_update();
    effect_objects_update();
    effect_event_tick();
}

void effect_objects_clear(void)
{
    FUNCTION_MARKER(0x80038E48, "ACE3.BIN");
    effect_objects.live = (uint32)(0);
}
