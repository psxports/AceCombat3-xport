#include <stddef.h>
#include <string.h>

#include "game_functions.h"
#include "psx_gte.h"

static const uint32 timed_effect_positions_a[7][3] = {
    {0xFE308000u, 0xFFD44000u, 0xFF2CC000u}, {0xFE2BE000u, 0xFFE70000u, 0xFE2D7000u}, {0xFE2CB000u, 0xFFE70000u, 0x01CC7000u}, {0x00CFC000u, 0xFFD44000u, 0xFE2D0000u}, {0x01D36000u, 0xFFE70000u, 0xFE27B000u}, {0xFD120000u, 0xFFF06000u, 0xFF63C000u}, {0x00000000u, 0xFFD44000u, 0xFD6FC000u},
};

static const uint32 timed_effect_positions_b[5][3] = {
    {0x00A4C000u, 0xFFED4000u, 0xFFF38000u}, {0xFF4AC000u, 0xFFF38000u, 0xFE34A000u}, {0xFE9BC000u, 0xFFED4000u, 0x01478000u}, {0x01E14000u, 0xFFF9C000u, 0xFF2F4000u}, {0x01D6A000u, 0xFFF7E000u, 0x0055E000u},
};

static const uint32 timed_effect_positions_c[12][3] = {
    {0x15B8C000u, 0xFFE70000u, 0x12E3B000u}, {0x1528A000u, 0xFFF10000u, 0x154D8000u}, {0x122F1000u, 0xFFEA2000u, 0x12494000u}, {0x109A0000u, 0xFFB50000u, 0x10F7C000u}, {0x0501E000u, 0xFFE70000u, 0x08CA0000u}, {0x04F97000u, 0xFFE0C000u, 0x05D4E000u}, {0x003E8000u, 0xFFB82000u, 0xFF5A6000u}, {0xFDFB2000u, 0xFFDE4000u, 0x02580000u}, {0x01238000u, 0xFFCCC000u, 0x01002000u}, {0x02328000u, 0xFFE98000u, 0xFF5CE000u}, {0xFE30C000u, 0xFFE70000u, 0xFECAA000u}, {0xFF9F9000u, 0xFFE0C000u, 0xF444C000u},
};

static const uint32 timed_effect_positions_d[10][3] = {
    {0x0CFE8000u, 0xFFF38000u, 0xFFC18000u}, {0x003E8000u, 0xFFB82000u, 0xFF5A6000u}, {0xFDFB2000u, 0xFFDE4000u, 0x02580000u}, {0x01238000u, 0xFFCCC000u, 0x01002000u}, {0x02328000u, 0xFFE98000u, 0xFF5CE000u}, {0xFE30C000u, 0xFFE70000u, 0xFECAA000u}, {0x053FF000u, 0xFFF38000u, 0x0BAFB000u}, {0x003B7000u, 0xFFF38000u, 0x08E0A000u}, {0xF7791000u, 0xFFE70000u, 0x00904000u}, {0xF850D000u, 0xFFD12000u, 0x06CDD000u},
};

EFFECT_DRIFT_STATE effect_drift = {EFFECT_DRIFT_SEED_RESET};

EFFECT_DRIFT_POOL const effect_drift_primary_pool = {effect_drift.primary_mask, effect_drift.primary, 3};
EFFECT_DRIFT_POOL const effect_drift_secondary_pool = {effect_drift.secondary_mask, effect_drift.secondary, 10};

EFFECT_LIGHT effect_light;

EFFECT_NOISE effect_noise;

EFFECT_EVENT_STATE effect_events;

static NATIVE_POSE player_spawn_pose;
EFFECT_ACTOR_TRACKING effect_actor_tracking;

uint32 effect_spawn_enqueue(uint32 kind, const uint32 *velocity, uint32 mode, const NATIVE_POSE *native_position)
{
    EFFECT_SPAWN_REQUEST *record;
    uint32 *mask_address;
    uint32 count, bits, group, bit;
    FUNCTION_MARKER(0x8006B034, "ACE3.BIN");
    count = effect_events.spawn_count;
    if ((sint32)count >= 8)
        return 0;
    mask_address = particles.live;
    bits = *mask_address;
    group = 0;
    while (bits == 0xFFFFFFFF)
    {
        group += 32;
        ++mask_address;
        if (group >= 128)
            return 0;
        bits = *mask_address;
    }
    bit = 0;
    while ((bits & (1u << bit)) != 0)
        ++bit;
    bits = *mask_address;
    *mask_address = bits | (1u << bit);
    count = effect_events.spawn_count;
    effect_events.spawn_count = count + 1;
    record = effect_events.spawn + count;
    record->particle_index = (uint16)(bit + group);
    record->effect_kind = (uint16)(mode);
    record->flags = (uint32)(kind);
    record->pose = native_position;
    record->velocity = velocity;
    return 1;
}

void effect_spawn_process(void)
{
    uint16 first[3], second[3];
    MATRIX matrix;
    NATIVE_POSE pose;
    const EFFECT_SPAWN_REQUEST *queue;
    const NATIVE_POSE *source_pose;
    PARTICLE *slot;
    uint32 offsets[3], delta[3], count, scan, index, flags, kind, a, b, c, x, y, z, jitter_x, jitter_y, jitter_z, random;
    FUNCTION_MARKER(0x8006B110, "ACE3.BIN");
    scan = 0;
    count = effect_events.spawn_count;
    queue = effect_events.spawn;
    if ((sint32)count > 0)
    {
        do
        {
            index = (sint16)queue->particle_index;
            source_pose = queue->pose;
            slot = particles.slot + index;
            flags = queue->flags;
            slot->actor_flags = (uint32)(flags);
            a = (((sint16)source_pose->angles[0]));
            b = (((sint16)source_pose->angles[1]));
            c = (((sint16)source_pose->angles[2]));
            first[0] = a;
            first[1] = b;
            first[2] = c;
            rotation_build(&matrix, first);
            a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
            b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
            a = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
            b = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
            c = (uint16)matrix.m[2][2];
            SetRotMatrix(&matrix);
            kind = (sint16)queue->effect_kind;
            switch (kind)
            {
                case 0:
                    slot->flags = (uint32)(0x20410);
                    slot->lifetime = (uint32)(37);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 5504;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 2048;
                    break;
                case 1:
                    slot->flags = (uint32)(0x20820);
                    slot->lifetime = (uint32)(24);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 6592;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 2448;
                    break;
                case 2:
                    slot->flags = (uint32)(0x41040);
                    slot->lifetime = (uint32)(62);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 4128;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 1216;
                    break;
                case 3:
                    slot->flags = (uint32)(0x82080);
                    slot->lifetime = (uint32)(35);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 4384;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 0;
                    break;
                case 4:
                    slot->flags = (uint32)(0x94103);
                    slot->lifetime = (uint32)(16);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 13760;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 0;
                    break;
                case 5:
                    slot->flags = (uint32)(0x98202);
                    slot->lifetime = (uint32)(16);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 11008;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 2048;
                    break;
                default:
                    break;
            }
            xport_gte_write_data(0, first[0] | ((uint32)first[1] << 16));
            xport_gte_write_data(1, first[2]);
            xport_gte_execute(0x486012);
            offsets[0] = xport_gte_read_data(25);
            offsets[1] = xport_gte_read_data(26);
            offsets[2] = xport_gte_read_data(27);
            xport_gte_write_data(0, second[0] | ((uint32)second[1] << 16));
            xport_gte_write_data(1, second[2]);
            xport_gte_execute(0x486012);
            delta[0] = xport_gte_read_data(25);
            delta[1] = xport_gte_read_data(26);
            delta[2] = xport_gte_read_data(27);
            jitter_x = 0;
            jitter_y = 0;
            jitter_z = 0;
            flags = slot->flags;
            if ((flags & 0x10000) == 0)
            {
                random = game_random();
                jitter_x = ((sint32)(0x4000 - random) >> 1);
                random = game_random();
                jitter_y = ((sint32)(0x4000 - random) >> 1);
                random = game_random();
                jitter_z = ((sint32)(0x4000 - random) >> 1);
            }
            flags = slot->flags;
            if ((flags & 2) == 0)
                jitter_y -= 13106;
            x = queue->velocity[0];
            slot->velocity[0] = (uint32)((offsets[0] << 7) + x + jitter_x);
            y = queue->velocity[1];
            slot->velocity[1] = (uint32)((offsets[1] << 7) + y + jitter_y);
            z = queue->velocity[2];
            z = (offsets[2] << 7) + z + jitter_z;
            slot->velocity[2] = (uint32)(z);
            a = ((source_pose->position[0]));
            x = slot->velocity[0];
            b = ((source_pose->position[1]));
            x = a - x;
            y = slot->velocity[1];
            a = ((source_pose->position[2]));
            slot->position[0] = (uint32)(x);
            y = b - y;
            z = a - z;
            slot->position[1] = (uint32)(y);
            slot->position[2] = (uint32)(z);
            slot->endpoint[0] = (uint32)(x + (delta[0] << 8));
            flags = slot->actor_flags;
            slot->endpoint[1] = (uint32)(y + (delta[1] << 8));
            slot->endpoint[2] = (uint32)(z + (delta[2] << 8));
            if ((flags & 0x2000) != 0)
            {
                pose.position[0] = x;
                pose.position[1] = slot->position[1];
                pose.position[2] = slot->position[2];
                pose.angles[0] = ((source_pose->angles[0]));
                pose.angles[1] = ((source_pose->angles[1]));
                pose.angles[2] = ((source_pose->angles[2]));
                index = (sint16)queue->particle_index;
                kind = (sint16)queue->effect_kind;
                mission_replay_record_particle_spawn(index, kind, &pose, slot->velocity);
            }
            scan += 1;
            count = effect_events.spawn_count;
            ++queue;
        } while ((sint32)scan < (sint32)count);
    }
    effect_events.spawn_count = 0;
}

EFFECT_DELAYED_EVENT *effect_event_schedule(uint32 mode, uint32 flags, uint32 parameter, uint32 delay)
{
    EFFECT_DELAYED_EVENT *target;
    uint32 mask, index;
    FUNCTION_MARKER(0x80068454, "ACE3.BIN");
    mask = effect_events.delayed_mask;
    if (mask == 0xFFFFFFFF)
        return 0;
    index = 0;
    while (mask & (1u << (index & 31)))
        ++index;
    mask |= 1u << (index & 31);
    target = effect_events.delayed + index;
    effect_events.delayed_mask = mask;
    target->delay = (uint16)(delay);
    target->event_bits = (uint16)(parameter);
    target->actor_flags = (uint32)(flags);
    target->condition_flags = (uint32)(mode);
    return target;
}

void effect_event_tick(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    EFFECT_DELAYED_EVENT *record;
    uint32 mask, index, bit, value, flags, condition, mode;
    FUNCTION_MARKER(0x800684D4, "ACE3.BIN");
    mask = effect_events.delayed_mask;
    if (mask == 0)
        return;
    actor_index_table = 0;
    record = effect_events.delayed;
    for (index = 0; index < 32; ++index, ++record)
    {
        bit = 1u << index;
        if ((mask & bit) == 0)
            continue;
        value = record->delay - 1;
        record->delay = (uint16)value;
        if ((sint32)(value << 16) >= 0)
            continue;
        value = record->actor_flags;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((value & 0x3F) * 4) >> 2))));
        flags = actor_traits_object->flags;
        mask &= ~bit;
        if ((flags & 0x4200) != 0x4200)
            continue;
        condition = record->condition_flags;
        if (flags & (condition & 0x1C0))
            continue;
        mode = record->event_bits;
        value = actor_traits_object->events;
        actor_traits_object->events = (uint32)(value | mode);
        value = record->condition_flags;
        actor_traits_object->damage_source = (uint32)(value);
    }
    effect_events.delayed_mask = mask;
    return;
}

intptr effect_event_schedule_from_object(const EFFECT_OBJECT *state)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 value, flags, mode, numerator, divisor, parameter, delay;
    FUNCTION_MARKER(0x8006AAFC, "ACE3.BIN");
    value = state->flags & 1;
    if (value)
        return value;
    value = state->target_flags;
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((value & 0x3F) * 4) >> 2))));
    flags = actor_traits_object->flags;
    if ((flags & 0x4200) != 0x4200)
        return 0x4200;
    mode = state->source_flags;
    value = flags & (mode & 0x1C0);
    if (value)
        return value;
    numerator = state->area_radius;
    divisor = state->delay_divisor;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    delay = ((sint64)(sint32)numerator / (sint32)divisor);
    parameter = state->event_bits;
    return (intptr)effect_event_schedule(mode, flags, parameter, delay);
}

EFFECT_TIMED_EVENT *effect_timed_allocate(void)
{
    uint32 mask, bit, index;

    mask = effect_events.timed_mask;
    if (mask == 0xFFFFFFFF)
        return NULL;
    bit = 1;
    index = 0;
    while ((mask & bit) != 0)
    {
        bit <<= 1;
        ++index;
    }
    effect_events.timed_mask = mask | bit;
    return effect_events.timed + index;
}

void effect_timed_draw_fade(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    uint32 numerator, intensity, value, ordering, packet;
    FUNCTION_MARKER(0x8008812C, "ACE3.BIN");
    numerator = (24 - ticks) * 2560;
    intensity = (sint32)numerator / 24;
    if ((sint32)intensity < 256)
    {
        value = *duration;
        *counter = (value);
        {
            (void)(value);
            return;
        }
    }
    ordering = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    value = ui_draw_scripted_flash(ordering + 8, packet, intensity);
    game_runtime.mission_control.packet_cur = (uint32)(value);
    effect_light.flags = (uint32)(1);
    return;
}

void effect_timed_emit_0(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    EFFECT_DRIFT *record;
    uint32 random, value, z, x, y, index, result;
    FUNCTION_MARKER(0x80088454, "ACE3.BIN");
    if ((sint32)ticks < 38 && (ticks & 3) == 0)
    {
        random = game_random();
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record != 0)
        {
            record->ticks = (uint16)(0);
            record->duration = (uint16)((random & 31) + 30);
            value = position[0];
            record->position[0] = (uint32)(value - 0x800000);
            value = position[1];
            record->position[1] = (uint32)(value);
            z = position[2];
            record->vector[0] = (uint16)((random & 255) - 128);
            record->vector[1] = (uint16)(((uint32)((sint32)random >> 3) & 255) - 144);
            record->vector[2] = (uint16)(((uint32)((sint32)random >> 6) & 255) - 128);
            record->flags = (uint16)(136);
            record->style = (uint16)(0x1704);
            record->radius = (uint16)((random & 255) + 200);
            record->position[2] = (uint32)(z);
        }
    }
    if (ticks != 90)
        return;
    z = position[2];
    x = position[0];
    y = position[1];
    x -= 0x200000;
    y += 0x5E6000;
    index = 0;
    do
    {
        effect_spawn_mixed_drift_group(x, y, z);
        x -= 0x140000;
        index += 1;
        y -= 0x78000;
    } while (index < 5);
    result = game_runtime.mission_control.flags & 2;
    if (result != 0)
    {
        (void)(sound_play_spatial_reuse(24, 1, position));
        return;
    }
    {
        (void)(result);
        return;
    }
}

void effect_timed_emit_1(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    EFFECT_DRIFT *record;
    uint32 flags, delta, spread, z, x, y, index, random, px, pz;
    FUNCTION_MARKER(0x80088748, "ACE3.BIN");
    if (ticks == 80)
    {
        flags = game_runtime.mission_control.flags;
        if ((flags & 2) != 0)
            sound_play_spatial_reuse(24, 1, position);
    }
    delta = ticks - 80;
    if (delta < 16)
    {
        spread = (((delta << 3) - delta) * 32 + delta) << 11;
        z = position[2];
        x = position[0];
        y = position[1];
        x -= spread;
        y += 0x3E8000;
        index = 0;
        do
        {
            record = effect_drift_allocate(&effect_drift_primary_pool);
            if (record == 0)
                break;
            random = game_random();
            record->ticks = (uint16)(0xFFFF);
            record->duration = (uint16)((random & 15) + 20);
            px = x + (((random & 255) - 128) << 11);
            pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11);
            record->position[2] = (uint32)(pz);
            record->style = (uint16)(0x130);
            record->position[0] = (uint32)(px);
            record->position[1] = (uint32)(y);
            record->flags = (uint16)(0);
            record->radius = (uint16)((random & 255) + 320);
            if (index == 1)
                x += spread << 1;
            z += (index & 1) != 0 ? 0x5DC000 : 0xFFA24000;
            index += 1;
        } while (index < 4);
    }
    if ((sint32)ticks >= 80)
        return;
    random = game_random();
    x = position[0];
    y = position[1];
    z = position[2];
    y += ((ticks << 1) + ticks) << 14;
    if ((random & 1) != 0)
        z -= 0x5DC000;
    x += ((random & 0xFFF) - 2048) << 11;
    if ((random & 2) != 0)
    {
        (void)(effect_spawn_primary_group_1(x, y, z));
        return;
    }
    if ((ticks & 3) != 0)
    {
        (void)(effect_spawn_secondary_group_1(x, y, z));
        return;
    }
    {
        (void)(effect_spawn_secondary_group_0(x, y + 0x50000, z));
        return;
    }
}

void effect_timed_emit_2(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    uint32 result, x, y, z, index;
    FUNCTION_MARKER(0x80088A84, "ACE3.BIN");
    result = (uint32)(sint16)*duration - 1;
    if (ticks != result)
    {
        (void)(result);
        return;
    }
    y = position[1];
    x = position[0];
    z = position[2];
    x -= 0x1F4000;
    index = 0;
    do
    {
        effect_spawn_mixed_drift_group(x, y, z);
        index += 1;
        x += 0xFA000;
    } while (index < 4);
    result = game_runtime.mission_control.flags & 2;
    if (result != 0)
    {
        (void)(sound_play_spatial_reuse(24, 1, position));
        return;
    }
    {
        (void)(result);
        return;
    }
}

void effect_timed_emit_3(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    uint32 random, x, y, z, flags, delta, result, offset;
    FUNCTION_MARKER(0x800891EC, "ACE3.BIN");
    if ((sint32)ticks < 40 && (ticks & 3) == 0)
    {
        random = game_random();
        if ((sint32)ticks < 20 || (random & 1) != 0)
        {
            x = position[0];
            x += ((random & 255) - 128) << 11;
            z = position[2];
            y = position[1];
            effect_spawn_secondary_group_1(x, y, z + ((random & 1) << 22));
        }
    }
    if (ticks == 82)
    {
        flags = game_runtime.mission_control.flags;
        if ((flags & 2) != 0)
            sound_play_spatial_reuse(24, 1, position);
    }
    delta = ticks - 82;
    result = ticks & 1;
    if (delta >= 11 || result == 0)
    {
        (void)(result);
        return;
    }
    offset = (sint32)(delta << 22) / 10;
    y = position[1];
    x = position[0];
    y += 0x2D0000;
    z = position[2];
    {
        (void)(effect_spawn_mixed_drift_group(x, y, z + offset));
        return;
    }
}

void effect_timed_emit_4(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    uint32 random, x, y, z, flags, delta, result, offset;
    FUNCTION_MARKER(0x80089378, "ACE3.BIN");
    if ((sint32)ticks < 40 && (ticks & 3) == 0)
    {
        random = game_random();
        if ((sint32)ticks < 20 || (random & 1) != 0)
        {
            x = position[0];
            x += (random & 1) << 22;
            z = position[2];
            y = position[1];
            effect_spawn_secondary_group_1(x, y, z + (((random & 255) - 128) << 11));
        }
    }
    if (ticks == 82)
    {
        flags = game_runtime.mission_control.flags;
        if ((flags & 2) != 0)
            sound_play_spatial_reuse(24, 1, position);
    }
    delta = ticks - 82;
    result = ticks & 1;
    if (delta >= 11 || result == 0)
    {
        (void)(result);
        return;
    }
    offset = (sint32)(delta << 22) / 10;
    y = position[1];
    z = position[2];
    y += 0x2D0000;
    x = position[0];
    {
        (void)(effect_spawn_mixed_drift_group(x + offset, y, z));
        return;
    }
}

void effect_timed_emit_5(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    MISSION_ACTOR *pose_owner_player;
    uint32 actor_index_table;

    uint32 result, flags, mode;
    FUNCTION_MARKER(0x80089C40, "ACE3.BIN");
    result = ticks & 7;
    if (result != 0)
    {
        (void)(result);
        return;
    }
    flags = game_runtime.mission_control.flags;
    mode = (uint32)((sint32)ticks >> 3) & 1;
    if ((flags & 2) == 0)
    {
        (void)(effect_emit_position_style(mode, 3, position));
        return;
    }
    actor_index_table = 0;
    pose_owner_player = mission_actor(actor_index_table);
    result = game_measure_distance_gte(position, pose_owner_player->pose.position);
    if ((sint32)result > 0x7FFF)
    {
        (void)(result);
        return;
    }
    {
        (void)(effect_emit_position_periodic(mode, 3, position));
        return;
    }
}

void effect_timed_emit_6(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position)
{
    MISSION_ACTOR *pose_owner_player;
    uint32 actor_index_table;

    uint32 result, flags, mode;
    FUNCTION_MARKER(0x80089CD8, "ACE3.BIN");
    result = ticks & 7;
    if (result != 0)
    {
        (void)(result);
        return;
    }
    flags = game_runtime.mission_control.flags;
    mode = (uint32)((sint32)ticks >> 3) & 1;
    if ((flags & 2) == 0)
    {
        (void)(effect_emit_position_style(mode, 2, position));
        return;
    }
    actor_index_table = 0;
    pose_owner_player = mission_actor(actor_index_table);
    result = game_measure_distance_gte(position, pose_owner_player->pose.position);
    if ((sint32)result > 0x7FFF)
    {
        (void)(result);
        return;
    }
    {
        (void)(effect_emit_position_periodic(mode, 2, position));
        return;
    }
}

void effect_timed_tick(void)
{
    EFFECT_TIMED_EVENT *event;
    EFFECT_TIMED_CALLBACK callback;
    uint32 mask, index, value, counter, limit;

    FUNCTION_MARKER(0x80089D70, "ACE3.BIN");
    mask = effect_events.timed_mask;
    event = effect_events.timed;
    if (mask == 0)
        return;
    index = 0;
    do
    {
        if ((mask & 1) != 0)
        {
            value = event->ticks + 1;
            event->ticks = (uint16)value;
            counter = (sint16)value;
            limit = (sint16)event->duration;
            if ((sint32)counter >= (sint32)limit)
            {
                value = effect_events.timed_mask;
                effect_events.timed_mask = value & ~(1u << (index & 31));
            }
            else
            {
                callback = event->callback;
                if (callback != NULL)
                    callback(counter, &event->ticks, &event->duration, event->position);
            }
        }
        mask >>= 1;
        ++index;
        ++event;
    } while (mask != 0);
}

uint32 effect_burst_start(const uint32 *pos, const uint32 *vel)
{
    EFFECT_BURST *state = &effect_events.burst;
    uint32 output[3], vx, vy, vz, value, x, y, z, dx, dz, result;
    FUNCTION_MARKER(0x800833C8, "ACE3.BIN");
    state->active = (uint32)(1);
    state->ticks = (uint16)(0);
    vx = (vel[0]);
    vy = (vel[1]);
    vz = (vel[2]);
    value = (pos[0]);
    x = value + (vx << 3);
    value = (pos[1]);
    dx = (sint32)x >> 8;
    y = value + (vy << 3);
    value = (pos[2]);
    state->position[0] = (uint32)(x);
    state->position[1] = (uint32)(y);
    z = value + (vz << 3);
    state->start[0] = (uint32)(x - dx);
    dz = (sint32)z >> 8;
    state->start[1] = (uint32)(y + 0xFD8F0000);
    state->start[2] = (uint32)(z - dz);
    state->end[0] = (uint32)(x + dx);
    state->end[1] = (uint32)(y + 0x02710000);
    state->position[2] = (uint32)(z);
    state->end[2] = (uint32)(z + dz);
    result = collision_query_segment(state->start, state->end, output);
    if (result != 0x7FFFFFFF)
    {
        state->end[0] = (uint32)(output[0]);
        state->end[1] = (uint32)(output[1]);
        state->end[2] = (uint32)(output[2]);
    }
    state->start[0] = (uint32)(x - (dx << 3));
    state->start[1] = (uint32)(y + 0xEC780000);
    result = z - (dz << 3);
    state->start[2] = (uint32)(result);
    return result;
}

uint32 effect_burst_trigger(const uint32 *pos, const uint32 *vel)
{
    FUNCTION_MARKER(0x8008350C, "ACE3.BIN");
    mission_replay_record_vector_effect(9, pos, vel, 0);

    return effect_burst_start(pos, vel);
}

uint32 effect_burst_tick(void)
{
    EFFECT_RENDER *render_effect;
    EFFECT_BURST *state = &effect_events.burst;
    uint32 value, counter, intensity, ot, packet, a;
    FUNCTION_MARKER(0x800928F4, "ACE3.BIN");
    value = state->active;
    if (value == 0)
        return 0;
    counter = (sint16)state->ticks;
    value = counter + 1;
    if ((sint32)counter >= 49)
    {
        state->active = (uint32)(0);
        return value;
    }
    state->ticks = (uint16)(value);
    if ((counter - 4) < 13)
    {
        if ((sint32)counter < 8)
            intensity = (8 - counter) << 8;
        else
            intensity = (counter - 8) << 6;
        effect_segment_emit(intensity, 2, state->end, state->start);
    }
    if ((counter - 4) < 5)
    {
        intensity = (0x400u >> ((counter - 4) & 31)) + 0x100;
        PSX_RECT *rect = (PSX_RECT *)getScratchAddr(0);
        rect->x = -160;
        rect->y = -120;
        rect->w = 320;
        rect->h = 240;
        ot = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        packet = ui_draw_blend_rectangle(ot + 8, packet, *rect, intensity);
        game_runtime.mission_control.packet_cur = (uint32)(packet);
    }
    if (counter == 9)
    {
        render_effect = effect_render_allocate();
        if (render_effect)
        {
            render_effect->ticks = (uint16)(0);
            value = game_random();
            render_effect->duration = (uint16)((value & 7) + 60);
            value = state->position[0];
            render_effect->position[0] = (uint32)(value);
            value = state->position[1];
            render_effect->position[1] = (uint32)(value);
            a = state->position[2];
            render_effect->angles[0] = (uint16)(0x400);
            render_effect->angles[1] = (uint16)(0);
            render_effect->kind = (uint16)(1);
            render_effect->position[2] = (uint32)(a);
            value = game_random();
            render_effect->radius = (uint16)((value & 0xFF) + 2000);
        }
    }
    if ((sint32)counter < 16)
        return 48;
    intensity = (48 - counter) << 5;
    value = game_random();
    intensity += value & 0x1FF;
    ot = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    packet = ui_draw_noise_overlay(ot + 8, packet, intensity);
    game_runtime.mission_control.packet_cur = (uint32)(packet);
    return packet;
}

void effect_segment_emit(uint32 scale, uint32 kind, const uint32 *origin, const uint32 *finish)
{
    EFFECT_SEGMENT *target;
    uint32 a, b, c;
    FUNCTION_MARKER(0x80083054, "ACE3.BIN");
    target = effect_segment_allocate();
    if (target == 0)
        return;
    target->phase = (uint16)(scale);
    target->kind = (uint16)(kind);
    {
        a = origin[0];
        b = origin[1];
        c = origin[2];
    }
    target->origin[0] = (uint32)(a);
    target->origin[1] = (uint32)(b);
    target->origin[2] = (uint32)(c);
    {
        a = finish[0];
        b = finish[1];
        c = finish[2];
    }
    target->finish[0] = (uint32)(a);
    target->finish[1] = (uint32)(b);
    target->finish[2] = (uint32)(c);
    return;
}

EFFECT_SEGMENT *effect_segment_allocate(void)
{
    uint32 index = 0, flags, common, bit;
    FUNCTION_MARKER(0x80082FF0, "ACE3.BIN");
    flags = effect_events.segment_flags;
    if ((sint32)flags < 0)
        return 0;
    common = flags & (flags >> 16);
    bit = 1;
    while (common & bit)
    {
        bit <<= 1;
        ++index;
    }
    effect_events.segment_flags = (uint32)(flags | bit | (bit << 16));
    return effect_events.segments + index;
}

void effect_segment_update(void)
{
    uint32 active, pending, cursor, bit;
    FUNCTION_MARKER(0x80091654, "ACE3.BIN");
    active = (uint16)effect_events.segment_flags;
    if (active == 0)
        return;
    pending = (uint16)(effect_events.segment_flags >> 16);
    cursor = active;
    bit = 1;
    do
    {
        if (cursor & 1)
        {
            if (pending & bit)
                pending &= ~bit;
            else
                active &= ~bit;
        }
        cursor >>= 1;
        bit <<= 1;
    } while (cursor);
    effect_events.segment_flags = (effect_events.segment_flags & 0xFFFF0000u) | (uint16)active;
    effect_events.segment_flags = (effect_events.segment_flags & 0xFFFFu) | ((uint32)(uint16)pending << 16);
}

typedef struct
{
    uint32 clip;
    XY scr;
} EFFECT_SEGMENT_EDGE;

typedef struct
{
    uint32 radius;
    uint32 color;
    uint32 center_clip;
    XY center;
    EFFECT_SEGMENT_EDGE edge[2];
} EFFECT_SEGMENT_END;

typedef struct
{
    uint32 rotation[8];
    uint32 angle;
    uint32 spread;
    XY direction[3];
    XY scr[3];
    uint32 clip[2];
} EFFECT_SEGMENT_DRAW_WORK;

typedef struct
{
    VECTOR corner[4];
    VECTOR normal;
} EFFECT_SEGMENT_CLIP_WORK;

typedef struct
{
    uint32 *ot_entry;
    VECTOR endpoint[2];
    EFFECT_SEGMENT_CLIP_WORK clip;
    EFFECT_SEGMENT_END end[2];
    EFFECT_SEGMENT_DRAW_WORK draw;
    uint32 rotation[5];
    uint32 identity[5];
} EFFECT_SEGMENT_CONTEXT;

static uint32 effect_segment_plane_distance(const VECTOR *first, const VECTOR *second)
{
    uint32 x, y, z, a, b, c, dot, sx, sy, sz, sum, divisor;
    FUNCTION_MARKER(0x80092014, "ACE3.BIN");
    x = (uint32)first->vx;
    y = (uint32)first->vy;
    z = (uint32)first->vz;
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    a = (uint32)second->vx;
    b = (uint32)second->vy;
    xport_gte_execute(0xA00428);
    dot = x * a;
    a = b * y;
    c = (uint32)second->vz;
    b = c * z;
    dot += a;
    dot += b;
    sx = xport_gte_read_data(25);
    sy = xport_gte_read_data(26);
    sz = xport_gte_read_data(27);
    sum = sx + sy + sz;
    xport_gte_write_data(30, sum);
    if ((sint32)sum <= 0)
        return 0x7FFFFFFF;
    divisor = SquareRoot0((sint32)sum);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && dot == 0x80000000)
        xport_mips_break(6);
    return (sint32)dot / (sint32)divisor;
}

static void effect_segment_interpolate(const VECTOR *first, const VECTOR *second, VECTOR *out, uint32 weight)
{
    uint32 value, y, z, x;
    FUNCTION_MARKER(0x8009211C, "ACE3.BIN");
    xport_gte_write_data(8, weight);
    value = (uint32)first->vx;
    xport_gte_write_data(9, value);
    value = (uint32)first->vy;
    xport_gte_write_data(10, value);
    value = (uint32)first->vz;
    xport_gte_write_data(11, value);
    y = (uint32)second->vy;
    z = (uint32)second->vz;
    x = (uint32)second->vx;
    xport_gte_write_control(21, x);
    xport_gte_write_control(22, y);
    xport_gte_write_control(23, z);
    xport_gte_execute(0x980011);
    value = xport_gte_read_data(25);
    out->vx = (sint32)(value);
    value = xport_gte_read_data(26);
    out->vy = (sint32)(value);
    value = xport_gte_read_data(27);
    out->vz = (sint32)(value);
    {
        (void)(y);
        return;
    }
}

static uint32 effect_segment_clip(VECTOR *first, VECTOR *second, uint32 near_plane, EFFECT_SEGMENT_CLIP_WORK *workspace)
{
    uint32 first_depth, second_depth, numerator, divisor, weight, h, index, first_side, second_side;
    FUNCTION_MARKER(0x80092168, "ACE3.BIN");
    first_depth = (uint32)first->vz;
    second_depth = (uint32)second->vz;
    if ((sint32)first_depth < (sint32)near_plane && (sint32)second_depth < (sint32)near_plane)
        return 0;
    if (((sint32)first_depth < (sint32)near_plane) != ((sint32)second_depth < (sint32)near_plane))
    {
        numerator = (near_plane - first_depth) << 12;
        divisor = second_depth - first_depth;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        weight = (sint32)numerator / (sint32)divisor;
        effect_segment_interpolate(first, second, (sint32)first_depth < (sint32)second_depth ? first : second, weight);
    }
    h = xport_gte_read_control(26);
    workspace->corner[1].vx = (sint32)(0 - (uint32)184);
    workspace->corner[2].vx = (sint32)(0 - (uint32)184);
    workspace->corner[0].vx = (sint32)(184);
    workspace->corner[0].vy = (sint32)(144);
    workspace->corner[0].vz = (sint32)(h);
    workspace->corner[1].vy = (sint32)(144);
    workspace->corner[1].vz = (sint32)(h);
    workspace->corner[2].vy = (sint32)(0 - (uint32)144);
    workspace->corner[2].vz = (sint32)(h);
    workspace->corner[3].vx = (sint32)(184);
    workspace->corner[3].vy = (sint32)(0 - (uint32)144);
    workspace->corner[3].vz = (sint32)(h);
    index = 0;
    do
    {
        OuterProduct0(&workspace->corner[index], &workspace->corner[(index + 1) & 3], &workspace->normal);
        first_side = effect_segment_plane_distance(&workspace->normal, first);
        second_side = effect_segment_plane_distance(&workspace->normal, second);
        if ((sint32)first_side < 0 && (sint32)second_side < 0)
            return 0;
        if (((sint32)first_side < 0) != ((sint32)second_side < 0))
        {
            numerator = first_side << 12;
            divisor = first_side - second_side;
            if (divisor == 0)
                xport_mips_break(7);
            if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                xport_mips_break(6);
            weight = (sint32)numerator / (sint32)divisor;
            effect_segment_interpolate(first, second, (sint32)first_side < (sint32)second_side ? first : second, weight);
        }
        index += 1;
    } while (index < 4);
    return 1;
}

typedef char effect_render_packet_layout_check[sizeof(POLY_G4) == 36 && offsetof(POLY_G4, tag) == 0 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, rgbc) == 0 && offsetof(POINT_C, xy) == 4 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, tag) == 0 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, xy) == 0 && offsetof(POINT_T, uv_attr) == 4 && offsetof(POINT_T, uv) == 4 && offsetof(XY, y) == 2 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 ? 1 : -1];

static uint32 effect_segment_draw_circle(uint32 packet, EFFECT_SEGMENT_DRAW_WORK *workspace, const EFFECT_SEGMENT_END *vertex)
{
    POLY_G4 *quad;
    uint32 radius, screen, angle, trig, center_clip, x, y, a, b, c, index, offset, mask, next;
    FUNCTION_MARKER(0x80091A70, "ACE3.BIN");
    radius = vertex->radius;
    screen = vertex->center.xy;
    if (radius == 0)
        return packet;
    xport_gte_write_control(5, (sint32)(screen << 16) >> 16);
    angle = workspace->angle;
    xport_gte_write_control(6, (sint32)screen >> 16);
    trig = ((uint32)(uint16)rsin(((angle & 0xFFF) * 4) >> 2) | ((uint32)(uint16)rcos(((angle & 0xFFF) * 4) >> 2) << 16));
    xport_gte_write_data(8, radius);
    xport_gte_write_data(9, (sint32)(trig << 16) >> 16);
    xport_gte_write_data(10, (sint32)trig >> 16);
    workspace->direction[0].xy = 0x10000000;
    xport_gte_execute(0x198003D);
    center_clip = game_screen_clip(vertex->center.xy);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26) & 0xFFFF;
    xport_gte_write_control(0, ((0 - x) << 16) | y);
    xport_gte_write_control(2, y);
    xport_gte_write_control(1, x << 16);
    xport_gte_write_control(3, 0);
    xport_gte_write_control(4, 4096);
    index = 0;
    offset = 0x400;
    workspace->direction[1].xy = 0x0EC8061F;
    workspace->direction[2].xy = 0x0B500B50;
    do
    {
        xport_gte_write_data(0, workspace->direction[0].xy);
        xport_gte_write_data(2, workspace->direction[1].xy);
        xport_gte_write_data(4, workspace->direction[2].xy);
        a = vertex->color;
        b = vertex->center.xy;
        xport_gte_execute(0x280030);
        quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
        quad->p[2].rgbc.rgbc = (uint32)(0x30303);
        quad->p[3].rgbc.rgbc = (uint32)(0x30303);
        quad->p[0].rgbc.rgbc = (uint32)(0x3A030303);
        quad->p[1].rgbc.rgbc = (uint32)(a);
        quad->p[1].xy.xy = (uint32)(b);
        if ((index & 1) != 0)
        {
            a = ((uint32)(uint16)rsin((offset) >> 2) | ((uint32)(uint16)rcos((offset) >> 2) << 16));
            b = ((uint32)(uint16)rsin(((offset) >> 2) + 256) | ((uint32)(uint16)rcos(((offset) >> 2) + 256) << 16));
            c = ((uint32)(uint16)rsin(((offset) >> 2) + 512) | ((uint32)(uint16)rcos(((offset) >> 2) + 512) << 16));
            workspace->direction[0].xy = a;
            workspace->direction[1].xy = b;
            workspace->direction[2].xy = c;
        }
        else
        {
            a = workspace->direction[0].x;
            b = workspace->direction[1].x;
            c = workspace->direction[2].x;
            workspace->direction[0].x = (sint16)(uint16)(0 - a);
            workspace->direction[1].x = (sint16)(uint16)(0 - b);
            workspace->direction[2].x = (sint16)(uint16)(0 - c);
        }
        if (center_clip != 0)
        {
            workspace->scr[0].xy = xport_gte_read_data(12);
            workspace->scr[1].xy = xport_gte_read_data(13);
            workspace->scr[2].xy = xport_gte_read_data(14);
            mask = center_clip & game_screen_clip(workspace->scr[0].xy);
            if (mask != 0)
                mask &= game_screen_clip(workspace->scr[1].xy);
            if (mask != 0)
                mask &= game_screen_clip(workspace->scr[2].xy);
            if (mask != 0)
                goto next_sector;
            a = workspace->scr[0].xy;
            b = workspace->scr[1].xy;
            c = workspace->scr[2].xy;
            quad->p[0].xy.xy = (uint32)(a);
            quad->p[2].xy.xy = (uint32)(b);
            quad->p[3].xy.xy = (uint32)(c);
        }
        else
        {
            quad->p[0].xy.xy = (uint32)(xport_gte_read_data(12));
            quad->p[2].xy.xy = (uint32)(xport_gte_read_data(13));
            quad->p[3].xy.xy = (uint32)(xport_gte_read_data(14));
        }
        next = packet + 36;
        quad->tag = (uint32)((next & 0xFFFFFF) | 0x08000000);
        packet = next;
    next_sector:
        index += 1;
        offset += 0x400;
    } while (index < 8);
    return packet;
}

static uint32 effect_segment_draw_cap(uint32 packet, EFFECT_SEGMENT_DRAW_WORK *workspace, EFFECT_SEGMENT_END *vertex)
{
    POLY_G4 *quad;
    uint32 radius, screen, angle, trig, center_clip, x, y, a, b, c, spread, twice, third, two_thirds, edge_value, side_value, mask;
    FUNCTION_MARKER(0x800916A8, "ACE3.BIN");
    radius = vertex->radius;
    screen = vertex->center.xy;
    if (radius == 0)
    {
        a = game_screen_clip(vertex->center.xy);
        vertex->center_clip = a;
        a = vertex->center_clip;
        b = vertex->center.xy;
        vertex->edge[1].clip = a;
        vertex->edge[1].scr.xy = b;
        a = vertex->edge[1].clip;
        b = vertex->edge[1].scr.xy;
        vertex->edge[0].clip = a;
        vertex->edge[0].scr.xy = b;
        return packet;
    }
    angle = workspace->angle;
    xport_gte_write_control(5, (sint32)(screen << 16) >> 16);
    xport_gte_write_control(6, (sint32)screen >> 16);
    trig = ((uint32)(uint16)rsin(((angle & 0xFFF) * 4) >> 2) | ((uint32)(uint16)rcos(((angle & 0xFFF) * 4) >> 2) << 16));
    xport_gte_write_data(8, radius);
    xport_gte_write_data(9, (sint32)(trig << 16) >> 16);
    xport_gte_write_data(10, (sint32)trig >> 16);
    xport_gte_execute(0x198003D);
    center_clip = game_screen_clip(vertex->center.xy);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26) & 0xFFFF;
    xport_gte_write_control(0, ((0 - x) << 16) | y);
    xport_gte_write_control(2, y);
    xport_gte_write_control(1, x << 16);
    xport_gte_write_control(3, 0);
    xport_gte_write_control(4, 4096);
    xport_gte_write_data(0, 0x10000000);
    vertex->center_clip = center_clip;
    spread = workspace->spread;
    xport_gte_execute(0x180001);
    twice = spread << 1;
    two_thirds = (sint32)twice / 3;
    third = (sint32)spread / 3;
    b = ((uint32)(uint16)rsin(((two_thirds & 0xFFF) * 4) >> 2) | ((uint32)(uint16)rcos(((two_thirds & 0xFFF) * 4) >> 2) << 16));
    c = ((uint32)(uint16)rsin(((spread & 0xFFF) * 4) >> 2) | ((uint32)(uint16)rcos(((spread & 0xFFF) * 4) >> 2) << 16));
    a = ((uint32)(uint16)rsin(((third & 0xFFF) * 4) >> 2) | ((uint32)(uint16)rcos(((third & 0xFFF) * 4) >> 2) << 16));
    workspace->direction[1].xy = b;
    workspace->direction[2].xy = c;
    workspace->direction[0].xy = a;
    workspace->scr[0].xy = xport_gte_read_data(14);
    xport_gte_write_data(0, workspace->direction[0].xy);
    xport_gte_write_data(2, workspace->direction[1].xy);
    xport_gte_write_data(4, workspace->direction[2].xy);
    edge_value = workspace->direction[0].x;
    side_value = workspace->direction[1].x;
    xport_gte_execute(0x280030);
    a = workspace->direction[2].x;
    workspace->direction[0].x = (sint16)(uint16)(0 - edge_value);
    workspace->direction[1].x = (sint16)(uint16)(0 - side_value);
    workspace->direction[2].x = (sint16)(uint16)(0 - a);
    if (center_clip != 0)
    {
        a = game_screen_clip(workspace->scr[0].xy);
        workspace->clip[0] = a;
    }
    workspace->scr[1].xy = xport_gte_read_data(12);
    workspace->scr[2].xy = xport_gte_read_data(13);
    vertex->edge[0].scr.xy = xport_gte_read_data(14);
    xport_gte_write_data(0, workspace->direction[0].xy);
    xport_gte_write_data(2, workspace->direction[1].xy);
    xport_gte_write_data(4, workspace->direction[2].xy);
    edge_value = game_screen_clip(vertex->edge[0].scr.xy);
    xport_gte_execute(0x280030);
    vertex->edge[0].clip = edge_value;
    if (center_clip != 0)
    {
        side_value = game_screen_clip(workspace->scr[1].xy);
        workspace->clip[1] = side_value;
        side_value &= center_clip & edge_value;
        if (side_value != 0)
            side_value &= game_screen_clip(workspace->scr[2].xy);
    }
    mask = center_clip & side_value;
    if (mask == 0)
    {
        quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
        packet += 36;
        quad->tag = (uint32)((packet & 0xFFFFFF) | 0x08000000);
        edge_value = vertex->edge[0].scr.xy;
        side_value = vertex->center.xy;
        a = vertex->color;
        quad->p[0].xy.xy = (uint32)(edge_value);
        quad->p[1].xy.xy = (uint32)(side_value);
        quad->p[1].rgbc.rgbc = (uint32)(a);
        edge_value = workspace->scr[2].xy;
        side_value = workspace->scr[1].xy;
        quad->p[3].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[2].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[0].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[2].xy.xy = (uint32)(edge_value);
        quad->p[3].xy.xy = (uint32)(side_value);
    }
    workspace->scr[2].xy = xport_gte_read_data(12);
    mask = center_clip & edge_value;
    if (center_clip != 0)
    {
        center_clip &= game_screen_clip(workspace->scr[2].xy);
        edge_value = workspace->clip[0];
        side_value = workspace->clip[1];
        mask = center_clip & edge_value;
    }
    if ((mask & side_value) == 0)
    {
        quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
        packet += 36;
        quad->tag = (uint32)((packet & 0xFFFFFF) | 0x08000000);
        edge_value = workspace->scr[1].xy;
        side_value = vertex->center.xy;
        a = vertex->color;
        quad->p[0].xy.xy = (uint32)(edge_value);
        quad->p[1].xy.xy = (uint32)(side_value);
        quad->p[1].rgbc.rgbc = (uint32)(a);
        edge_value = workspace->scr[0].xy;
        side_value = workspace->scr[2].xy;
        quad->p[3].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[2].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[0].rgbc.rgbc = (uint32)(0x3A020202);
        quad->p[2].xy.xy = (uint32)(edge_value);
        quad->p[3].xy.xy = (uint32)(side_value);
    }
    workspace->scr[1].xy = xport_gte_read_data(13);
    vertex->edge[1].scr.xy = xport_gte_read_data(14);
    a = game_screen_clip(vertex->edge[1].scr.xy);
    center_clip &= a;
    vertex->edge[1].clip = a;
    if (center_clip != 0)
    {
        center_clip &= game_screen_clip(workspace->scr[1].xy);
        if (center_clip != 0)
            return packet;
    }
    quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
    packet += 36;
    quad->tag = (uint32)((packet & 0xFFFFFF) | 0x08000000);
    edge_value = vertex->edge[1].scr.xy;
    side_value = vertex->center.xy;
    a = vertex->color;
    quad->p[0].xy.xy = (uint32)(edge_value);
    quad->p[1].xy.xy = (uint32)(side_value);
    quad->p[1].rgbc.rgbc = (uint32)(a);
    edge_value = workspace->scr[1].xy;
    side_value = workspace->scr[2].xy;
    quad->p[3].rgbc.rgbc = (uint32)(0x3A020202);
    quad->p[2].rgbc.rgbc = (uint32)(0x3A020202);
    quad->p[0].rgbc.rgbc = (uint32)(0x3A020202);
    quad->p[2].xy.xy = (uint32)(edge_value);
    quad->p[3].xy.xy = (uint32)(side_value);
    return packet;
}

static void effect_segment_draw_span(uint32 *ordering, EFFECT_SEGMENT_END *first, EFFECT_SEGMENT_END *second, EFFECT_SEGMENT_DRAW_WORK *workspace, EFFECT_FRAME_CONTEXT *frame)
{
    POLY_G4 *quad;
    uint32 dx, dy, angle, distance, a, b, c, value, packet, magnitude, numerator, parameter, common, result;
    FUNCTION_MARKER(0x80091CE8, "ACE3.BIN");
    a = first->center.x;
    b = second->center.x;
    c = first->center.y;
    value = second->center.y;
    dx = a - b;
    dy = c - value;
    angle = (uint32)ratan2(0 - dx, dy);
    workspace->angle = angle;
    distance = game_vector_length_gte(dx, dy, 0);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    workspace->rotation[0] = a;
    workspace->rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    workspace->rotation[2] = a;
    workspace->rotation[3] = b;
    workspace->rotation[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    workspace->rotation[5] = a;
    workspace->rotation[6] = b;
    workspace->rotation[7] = c;
    value = frame->projection;
    xport_gte_write_data(1, value);
    xport_gte_write_data(3, value);
    xport_gte_write_data(5, value);
    xport_gte_write_control(7, 0);
    a = first->radius;
    b = second->radius;
    packet = frame->packet;
    dx = a - b;
    magnitude = dx;
    if ((sint32)dx < 0)
        magnitude = 0 - dx;
    if ((sint32)magnitude >= (sint32)distance)
        packet = effect_segment_draw_circle(packet, workspace, (sint32)dx < 0 ? second : first);
    else
    {
        numerator = dx << 14;
        if (distance == 0)
            xport_mips_break(7);
        if (distance == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = (sint32)numerator / (sint32)distance;
        value = game_angle_asin(value);
        parameter = (sint32)value >> 2;
        workspace->spread = parameter + 1024;
        packet = effect_segment_draw_cap(packet, workspace, first);
        a = workspace->angle;
        workspace->spread = 1024 - parameter;
        workspace->angle = a + 2048;
        packet = effect_segment_draw_cap(packet, workspace, second);
        a = first->center_clip;
        b = second->center_clip;
        c = first->edge[0].clip;
        common = a & b;
        b = second->edge[1].clip;
        if ((common & c & b) == 0)
        {
            quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
            packet += 36;
            quad->tag = (uint32)((packet & 0xFFFFFF) | 0x08000000);
            a = first->center.xy;
            b = second->center.xy;
            quad->p[0].xy.xy = (uint32)(a);
            quad->p[1].xy.xy = (uint32)(b);
            a = first->edge[0].scr.xy;
            b = second->edge[1].scr.xy;
            quad->p[2].xy.xy = (uint32)(a);
            quad->p[3].xy.xy = (uint32)(b);
            a = first->color;
            b = second->color;
            quad->p[3].rgbc.rgbc = (uint32)(0x20202);
            quad->p[2].rgbc.rgbc = (uint32)(0x20202);
            quad->p[0].rgbc.rgbc = (uint32)((a & 0xFFFFFF) | 0x3A000000);
            quad->p[1].rgbc.rgbc = (uint32)(b);
        }
        a = first->edge[1].clip;
        b = second->edge[0].clip;
        if ((common & a & b) == 0)
        {
            quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
            packet += 36;
            quad->tag = (uint32)((packet & 0xFFFFFF) | 0x08000000);
            a = first->center.xy;
            b = second->center.xy;
            quad->p[0].xy.xy = (uint32)(a);
            quad->p[1].xy.xy = (uint32)(b);
            a = first->edge[1].scr.xy;
            b = second->edge[0].scr.xy;
            quad->p[2].xy.xy = (uint32)(a);
            quad->p[3].xy.xy = (uint32)(b);
            a = first->color;
            b = second->color;
            quad->p[3].rgbc.rgbc = (uint32)(0x20202);
            quad->p[2].rgbc.rgbc = (uint32)(0x20202);
            quad->p[0].rgbc.rgbc = (uint32)((a & 0xFFFFFF) | 0x3A000000);
            quad->p[1].rgbc.rgbc = (uint32)(b);
        }
    }
    result = frame->packet;
    if (result != packet)
    {
        uint32 *ot_tag = ordering;
        POLY_G4 *tail = mission_packet_span(packet - sizeof(*tail), sizeof(*tail));
        value = *ot_tag;
        result <<= 8;
        tail->tag = (tail->tag & 0xFF000000) | (value & 0xFFFFFF);
        *ot_tag = (*ot_tag & 0xFF000000) | (result >> 8);
        frame->packet = packet;
    }
    a = workspace->rotation[0];
    b = workspace->rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = workspace->rotation[2];
    b = workspace->rotation[3];
    c = workspace->rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    {
        (void)(result);
        return;
    }
}

static void effect_segment_draw_record(EFFECT_FRAME_CONTEXT *frame, EFFECT_SEGMENT_CONTEXT *matrix, const EFFECT_SEGMENT *record)
{
    uint32 kind, phase, shift, origin_x, origin_y, origin_z, x, y, z, value, near_plane, a, b, c, first_depth, second_depth, bucket, intensity, numerator, divisor, first_radius, second_radius, color, inner_color;
    uint32 *ordering;
    FUNCTION_MARKER(0x8009236C, "ACE3.BIN");
    kind = (sint16)record->kind;
    phase = (sint16)record->phase;
    shift = kind == 2 ? 15 : 12;
    value = record->origin[0];
    origin_x = frame->camera.word[0];
    x = (sint32)(value - origin_x) >> shift;
    if (x + 0x8000 > 0xFFFF)
        return;
    value = record->origin[1];
    origin_y = frame->camera.word[1];
    y = (sint32)(value - origin_y) >> shift;
    if (y + 0x8000 > 0xFFFF)
        return;
    value = record->origin[2];
    origin_z = frame->camera.word[2];
    z = (sint32)(value - origin_z) >> shift;
    if (z + 0x8000 > 0xFFFF)
        return;
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    value = record->finish[0];
    x = (sint32)(value - origin_x) >> shift;
    xport_gte_execute(0x49E012);
    if (x + 0x8000 > 0xFFFF)
        return;
    value = record->finish[1];
    y = (sint32)(value - origin_y) >> shift;
    if (y + 0x8000 > 0xFFFF)
        return;
    value = record->finish[2];
    z = (sint32)(value - origin_z) >> shift;
    if (z + 0x8000 > 0xFFFF)
        return;
    matrix->endpoint[0].vx = (sint32)(xport_gte_read_data(25));
    matrix->endpoint[0].vy = (sint32)(xport_gte_read_data(26));
    matrix->endpoint[0].vz = (sint32)(xport_gte_read_data(27));
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    value = frame->projection;
    near_plane = (sint32)(value << 16) >> 18;
    xport_gte_execute(0x49E012);
    if ((sint32)near_plane <= 0)
        near_plane = 1;
    matrix->endpoint[1].vx = (sint32)(xport_gte_read_data(25));
    matrix->endpoint[1].vy = (sint32)(xport_gte_read_data(26));
    matrix->endpoint[1].vz = (sint32)(xport_gte_read_data(27));
    if (effect_segment_clip(matrix->endpoint, matrix->endpoint + 1, near_plane, &matrix->clip) == 0)
        return;
    matrix->identity[0] = 4096;
    matrix->identity[2] = 4096;
    matrix->identity[4] = 4096;
    matrix->identity[1] = 0;
    matrix->identity[3] = 0;
    a = matrix->identity[0];
    b = matrix->identity[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = matrix->identity[2];
    b = matrix->identity[3];
    c = matrix->identity[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    y = (uint32)matrix->endpoint[0].vy;
    first_depth = (uint32)matrix->endpoint[0].vz;
    x = (uint16)matrix->endpoint[0].vx;
    xport_gte_write_data(0, x | (y << 16));
    xport_gte_write_data(1, first_depth);
    y = (uint32)matrix->endpoint[1].vy;
    second_depth = (uint32)matrix->endpoint[1].vz;
    x = (uint16)matrix->endpoint[1].vx;
    xport_gte_write_data(2, x | (y << 16));
    xport_gte_write_data(3, second_depth);
    bucket = (sint32)first_depth < (sint32)second_depth ? first_depth : second_depth;
    xport_gte_execute(0x280030);
    bucket = (sint32)bucket >> ((15 - shift) & 31);
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    value = frame->ot;
    matrix->ot_entry = mission_ot_span(value + (bucket << 2), sizeof(uint32));
    if (kind == 3)
        intensity = phase;
    else
    {
        intensity = rsin(((phase & 0xFFF) * 4) >> 2);
        if (intensity == 0)
            intensity = 0 - (uint32)4096;
    }
    matrix->end[0].center.xy = xport_gte_read_data(12);
    matrix->end[1].center.xy = xport_gte_read_data(13);
    a = matrix->rotation[0];
    b = matrix->rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = matrix->rotation[2];
    b = matrix->rotation[3];
    c = matrix->rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(8, intensity);
    xport_gte_write_data(6, 0);
    xport_gte_write_data(20, 0x279327);
    xport_gte_write_data(21, 0x838383);
    if (first_depth == 0)
        first_depth = 1;
    if (second_depth == 0)
        second_depth = 1;
    xport_gte_execute(0xF8002A);
    if (kind == 3)
    {
        a = (record->origin[0] & 255u);
        b = (sint16)frame->projection;
        numerator = a * b;
        divisor = first_depth;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        first_radius = (sint32)numerator / (sint32)divisor;
        a = (record->origin[1] & 255u);
        numerator = a * b;
    }
    else
    {
        a = kind == 2 ? 12 : 8;
        b = (sint16)frame->projection;
        numerator = a * b;
        divisor = first_depth;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        first_radius = (sint32)numerator / (sint32)divisor;
    }
    divisor = second_depth;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    second_radius = (sint32)numerator / (sint32)divisor;
    if ((sint32)first_radius < 2)
        first_radius = 2;
    if ((sint32)second_radius < 2)
        second_radius = 2;
    matrix->end[0].radius = first_radius;
    matrix->end[1].radius = second_radius;
    color = xport_gte_read_data(20);
    inner_color = xport_gte_read_data(21);
    ordering = matrix->ot_entry;
    matrix->end[1].color = color;
    matrix->end[0].color = color;
    effect_segment_draw_span(ordering, matrix->end, matrix->end + 1, &matrix->draw, frame);
    ordering = matrix->ot_entry;
    matrix->end[0].radius = (sint32)first_radius >> 1;
    matrix->end[1].color = inner_color;
    matrix->end[0].color = inner_color;
    matrix->end[1].radius = (sint32)second_radius >> 1;
    {
        (void)(effect_segment_draw_span(ordering, matrix->end, matrix->end + 1, &matrix->draw, frame));
        return;
    }
}

void effect_segment_draw(EFFECT_FRAME_CONTEXT *frame)
{
    EFFECT_SEGMENT_CONTEXT context;
    const EFFECT_SEGMENT *record;
    uint32 mask, a, b, c;
    FUNCTION_MARKER(0x80092850, "ACE3.BIN");
    mask = (uint16)effect_events.segment_flags;
    record = effect_events.segments;
    if (mask == 0)
        return;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
    do
    {
        if (mask & 1)
            effect_segment_draw_record(frame, &context, record);
        mask >>= 1;
        ++record;
    } while (mask);
}

void effect_aircraft_start(uint32 kind, uint32 value)
{
    FUNCTION_MARKER(0x8007F9CC, "ACE3.BIN");
    effect_events.aircraft.ticks = (uint32)(1);
    effect_events.aircraft.control = (effect_events.aircraft.control & 0xFFFF0000u) | (uint16)kind;
    effect_events.aircraft.control = (effect_events.aircraft.control & 0xFFFFu) | ((uint32)(uint16)value << 16);
    return;
}

void effect_aircraft_init(void)
{
    EFFECT_AIRCRAFT *state = &effect_events.aircraft;
    uint32 i;
    FUNCTION_MARKER(0x800900A4, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
        state->line_xy[i] = game_runtime.aircraft_mission_metadata.effect_lines[i];
    state->trail_xy[0] = game_runtime.aircraft_mission_metadata.effect_trails[0];
    state->trail_xy[1] = game_runtime.aircraft_mission_metadata.effect_trails[1];
    state->center_xy = game_runtime.aircraft_mission_metadata.effect_center;
}

void effect_aircraft_trigger(uint32 value)
{
    FUNCTION_MARKER(0x80090110, "ACE3.BIN");
    effect_events.aircraft.control = (uint32)(value);
    effect_events.aircraft.ticks = (uint32)(1);
    mission_replay_record_aircraft_effect(value & 15, (sint32)value >> 16);
}

void effect_aircraft_update(void)
{
    EFFECT_AIRCRAFT *state = &effect_events.aircraft;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 count, packed, scale, value, mode;
    FUNCTION_MARKER(0x80090148, "ACE3.BIN");
    count = state->ticks;
    if ((sint32)count <= 0)
        return;
    packed = state->control;
    state->ticks = (uint32)((sint32)count < ((sint32)packed >> 16) ? count + 1 : 0);
    if ((game_runtime.mission_control.flags & 2) == 0 || (packed & 0xFFFF) != 5)
        return;
    scale = (count - 1) << 6;
    value = game_runtime.camera_control.mode;
    mode = value == 1;
    value = game_input.held;
    if ((value & 0x800) == 0)
    {
        scale += 256;
        if ((sint32)scale >= 1025)
            scale = 1024;
    }
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    effect_segment_trace_emit(scale, mode, &pose_owner_object->pose);
}

typedef struct
{
    uint32 kind;
    uint32 strength;
    uint32 tick;
    uint32 noise;
    EFFECT_QUAD quad;
    POINT_T corner[4];
} EFFECT_AIRCRAFT_DRAW_CONTEXT;

void effect_quad_build(POINT_T *vertices, const EFFECT_QUAD *state)
{
    uint32 x1, x0, y1, y0, value, scale, pair, corner, x, y, vx, vy, result;
    SVECTOR normal;
    const XY *endpoint;
    POINT_T *output;
    FUNCTION_MARKER(0x8008DE18, "ACE3.BIN");
    x1 = (uint16)state->endpoint[1].x;
    x0 = (uint16)state->endpoint[0].x;
    y1 = (uint16)state->endpoint[1].y;
    y0 = (uint16)state->endpoint[0].y;
    normal.vx = (sint16)(uint16)(x1 - x0);
    normal.vy = (sint16)(uint16)(y1 - y0);
    value = ((uint32)(uint16)normal.vx | ((uint32)(uint16)normal.vy << 16));
    normal.vz = 0;
    if (value != 0)
    {
        VectorNormalSS(&normal, &normal);
    }
    else
        normal.vx = 0x1000;
    for (pair = 0; pair < 2; ++pair)
    {
        scale = state->scale[pair];
        endpoint = &state->endpoint[pair];
        output = vertices + pair * 2;
        if (scale == 0)
        {
            result = endpoint->xy;
            output[1].xy.xy = result;
            output[0].xy.xy = result;
            continue;
        }
        xport_gte_write_data(8, scale);
        for (corner = 0; corner < 2; ++corner)
        {
            value = (uint16)endpoint->x;
            xport_gte_write_data(25, value);
            value = (uint16)endpoint->y;
            xport_gte_write_data(26, value);
            x = normal.vx;
            y = normal.vy;
            if (pair == 0)
            {
                vx = corner == 0 ? 0 - x - y : 0 - x + y;
                vy = corner == 0 ? x - y : 0 - x - y;
            }
            else
            {
                vx = corner == 0 ? x - y : x + y;
                vy = corner == 0 ? x + y : y - x;
            }
            xport_gte_write_data(9, vx);
            xport_gte_write_data(10, vy);
            xport_gte_execute(0x1A8003E);
            value = xport_gte_read_data(25);
            output[corner].xy.x = (sint16)(uint16)(value);
            result = xport_gte_read_data(26);
            output[corner].xy.y = (sint16)(uint16)(result);
        }
    }
    return;
}

static void effect_aircraft_apply_lighting(uint32 strength, uint32 kind)
{
    MATRIX native_matrix;
    uint32 saved[8];
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 a, b, c, red, green, blue, scale, previous17, previous18;
    FUNCTION_MARKER(0x80090210, "ACE3.BIN");
    if ((sint32)strength < 0 || kind >= 6)
        return;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    saved[0] = a;
    saved[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    saved[2] = a;
    saved[3] = b;
    saved[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    saved[5] = a;
    saved[6] = b;
    saved[7] = c;
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    rotation_build(&native_matrix, pose_owner_object->pose.angles);
    a = ((uint32)(uint16)native_matrix.m[0][0] | ((uint32)(uint16)native_matrix.m[0][1] << 16));
    b = ((uint32)(uint16)native_matrix.m[0][2] | ((uint32)(uint16)native_matrix.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)native_matrix.m[1][1] | ((uint32)(uint16)native_matrix.m[1][2] << 16));
    b = ((uint32)(uint16)native_matrix.m[2][0] | ((uint32)(uint16)native_matrix.m[2][1] << 16));
    c = (uint16)native_matrix.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0xFF9C0000);
    xport_gte_write_data(1, 0xF38);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    game_runtime.mission_lighting.directions.m[2][0] = (sint16)(uint16)(a);
    game_runtime.mission_lighting.directions.m[2][1] = (sint16)(uint16)(b);
    game_runtime.mission_lighting.directions.m[2][2] = (sint16)(uint16)(c);
    a = saved[0];
    b = saved[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = saved[2];
    b = saved[3];
    c = saved[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    switch (kind)
    {
        case 0:
        case 1:
            red = 0xA00;
            green = 0x600;
            blue = 0x300;
            scale = strength << 11;
            break;
        case 2:
            red = 0xE00;
            green = 0x800;
            blue = 0x200;
            scale = strength << 11;
            break;
        case 3:
            red = 0x800;
            green = 0x600;
            blue = 0xA00;
            scale = strength << 10;
            break;
        case 4:
            red = 0x200;
            green = 0x400;
            blue = 0xE00;
            scale = strength << 10;
            break;
        case 5:
            red = 0x200;
            green = 0xC00;
            blue = 0x200;
            scale = strength << 8;
            break;
    }
    xport_gte_write_data(9, red);
    xport_gte_write_data(10, green);
    xport_gte_write_data(11, blue);
    xport_gte_write_data(8, scale);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    previous17 = xport_gte_read_control(17);
    previous18 = xport_gte_read_control(18);
    xport_gte_execute(0x980011);
    red = xport_gte_read_data(25);
    green = xport_gte_read_data(26);
    blue = xport_gte_read_data(27);
    xport_gte_write_control(17, (previous17 & 0xFFFF0000) | (red & 0xFFFF));
    xport_gte_write_control(18, (previous18 & 0xFFFF) | (green << 16));
    xport_gte_write_control(20, blue);
}

typedef char aircraft_effect_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && offsetof(POLY_FT4, rgbc) == 4 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 ? 1 : -1];

void effect_aircraft_draw(EFFECT_FRAME_CONTEXT *frame)
{
    uint32 *ordering_entry;
    EFFECT_AIRCRAFT_DRAW_CONTEXT context;
    const EFFECT_AIRCRAFT *effect = &effect_events.aircraft;
    const uint16 *random;
    POLY_FT4 *quad;
    uint32 kind, strength, mode, root, packet, tick, value, a, b, c, d, target, head, color, result, size_x, size_y, i, start, next, center;
    FUNCTION_MARKER(0x8009044C, "ACE3.BIN");
    kind = (sint16)(uint16)effect->control;
    strength = effect->ticks;
    strength -= 2;
    context.kind = kind;
    context.strength = strength;
    mode = game_runtime.camera_control.mode;
    if (mode != 1)
        effect_aircraft_apply_lighting(strength, kind);
    result = (sint32)kind < 5;
    if (result == 0 || (sint32)strength < 0)
        return;
    mode = game_runtime.camera_control.mode;
    if (mode != 1)
        return;
    result = game_runtime.camera_control.flags & 0x200;
    if (result != 0)
        return;
    packet = frame->packet;
    tick = game_runtime.gameplay_counter;
    random = effect_noise.sample;
    value = ((uint32)random[(tick & 255) * 2] | ((uint32)random[(tick & 255) * 2 + 1] << 16));
    strength = context.strength;
    context.tick = tick;
    context.noise = value;
    if (strength == 0)
    {
        kind = context.kind;
        if (kind != 3)
        {
            result = (sint32)kind < 5;
            if (result == 0)
            {
                frame->packet = packet;
                return;
            }
            root = frame->ot;
            ordering_entry = mission_ot_span(root + 8, sizeof(*ordering_entry));
            head = *ordering_entry;
            quad = mission_packet_span(packet, sizeof(*quad));
            quad->tag = head | 0x09000000;
            root = frame->ot;
            ordering_entry = mission_ot_span(root + 8, sizeof(*ordering_entry));
            *ordering_entry = packet & 0xFFFFFF;
            value = effect->line_xy[0];
            kind = context.kind;
            context.quad.endpoint[0].x = (sint16)(uint16)(value);
            value = effect->line_xy[1];
            context.quad.endpoint[0].y = (sint16)(uint16)(value);
            value = effect->line_xy[2];
            context.quad.endpoint[1].x = (sint16)(uint16)(value);
            value = effect->line_xy[3];
            target = packet;
            packet += 40;
            context.quad.endpoint[1].y = (sint16)(uint16)(value);
            switch (kind)
            {
                case 0:
                    context.quad.scale[0] = 1;
                    context.quad.scale[1] = 3;
                    quad->p[1].uv_attr = 0x003860AF;
                    quad->p[2].uv.uv = (uint16)(0x9FA0);
                    quad->rgbc.rgbc = 0x2E838383;
                    quad->p[0].uv_attr = 0x445660A0;
                    quad->p[3].uv.uv = (uint16)(0x9FAF);
                    break;
                case 1:
                    context.quad.scale[0] = 2;
                    context.quad.scale[1] = 4;
                    quad->p[2].uv.uv = (uint16)(0x9FB0);
                    quad->rgbc.rgbc = 0x2E838383;
                    quad->p[0].uv_attr = 0x449660B0;
                    quad->p[1].uv_attr = 0x003860BF;
                    quad->p[3].uv.uv = (uint16)(0x9FBF);
                    break;
                case 2:
                    context.quad.scale[1] = 4;
                    context.quad.scale[0] = 3;
                    quad->p[2].uv.uv = (uint16)(0x9FC0);
                    quad->rgbc.rgbc = 0x2E838383;
                    quad->p[0].uv_attr = 0x44D660C0;
                    quad->p[1].uv_attr = 0x003860CF;
                    quad->p[3].uv.uv = (uint16)(0x9FCF);
                    break;
                case 4:
                    context.quad.scale[0] = 6;
                    context.quad.scale[1] = 7;
                    quad->p[2].uv.uv = (uint16)(0x9FD0);
                    quad->rgbc.rgbc = 0x2E838383;
                    quad->p[0].uv_attr = 0x455660D0;
                    quad->p[1].uv_attr = 0x003860DF;
                    quad->p[3].uv.uv = (uint16)(0x9FDF);
                    break;
            }
            if (kind == 0 || kind == 1)
            {
                a = (uint16)context.noise;
                b = (uint16)context.quad.endpoint[0].x;
                c = context.noise;
                context.quad.endpoint[0].x = (sint16)(uint16)(b + (a & 1));
                value = (uint16)context.quad.endpoint[0].y;
                context.quad.endpoint[0].y = (sint16)(uint16)(value + (((sint32)c >> 1) & 1));
            }
            effect_quad_build(quad->p, &context.quad);
        }
    }
    kind = context.kind;
    result = (sint32)kind < 5;
    if (result == 0)
    {
        frame->packet = packet;
        return;
    }
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    if ((sint32)kind < 3)
    {
        value = context.noise & 0x7FF;
        xport_gte_write_data(8, value);
        if (kind == 0)
        {
            color = 0x2E838383;
            a = 14;
            b = 16;
        }
        else if (kind == 1)
        {
            color = 0x2E738393;
            a = 19;
            b = 21;
        }
        else
        {
            color = 0x2E737383;
            a = 15;
            b = 18;
        }
        xport_gte_write_data(6, color);
        context.quad.scale[0] = a;
        context.quad.scale[1] = b;
        target = packet;
        quad = mission_packet_span(target, sizeof(*quad));
        packet += 40;
        xport_gte_execute(0x780010);
        root = frame->ot;
        ordering_entry = mission_ot_span(root + 8, sizeof(*ordering_entry));
        head = *ordering_entry;
        quad->tag = head | 0x09000000;
        root = frame->ot;
        ordering_entry = mission_ot_span(root + 8, sizeof(*ordering_entry));
        *ordering_entry = target & 0xFFFFFF;
        tick = context.tick;
        value = (uint32)((sint32)tick % 5) << 5;
        quad->p[0].uv_attr = value | 0x40566000;
        quad->p[1].uv_attr = (value + 31) | 0x00386000;
        quad->p[2].uv.uv = (uint16)(value | 0xFFFF9F00);
        quad->p[3].uv.uv = (uint16)((value + 31) | 0xFFFF9F00);
        a = effect->trail_xy[0];
        b = effect->trail_xy[1];
        context.quad.endpoint[0].xy = a;
        context.quad.endpoint[1].xy = b;
        value = xport_gte_read_data(22);
        quad->rgbc.rgbc = value;
        effect_quad_build(context.corner, &context.quad);
        result = context.tick & 1;
        if (result != 0)
        {
            a = context.corner[1].xy.xy;
            b = context.corner[0].xy.xy;
            c = context.corner[3].xy.xy;
            d = context.corner[2].xy.xy;
        }
        else
        {
            a = context.corner[0].xy.xy;
            b = context.corner[1].xy.xy;
            c = context.corner[2].xy.xy;
            d = context.corner[3].xy.xy;
        }
        quad->p[0].xy.xy = a;
        quad->p[1].xy.xy = b;
        quad->p[2].xy.xy = c;
        quad->p[3].xy.xy = d;
        frame->packet = packet;
        return;
    }
    strength = context.strength;
    value = context.noise;
    xport_gte_write_data(8, (strength << 10) + (value & 0x3FF));
    color = kind == 3 ? 0x2E231333 : 0x2E332313;
    xport_gte_write_data(6, color);
    start = packet;
    center = effect->center_xy;
    xport_gte_execute(0x780010);
    value = context.noise;
    value = ((sint32)value >> 7) & 7;
    size_x = 100 - value;
    size_y = 90 - value;
    context.corner[0].xy.xy = center;
    value = (uint16)context.corner[0].xy.x;
    a = value;
    context.corner[1].xy.x = (sint16)(uint16)(value - size_x);
    value = (uint16)context.corner[0].xy.y;
    context.corner[2].xy.x = (sint16)(uint16)(a + size_x);
    context.corner[1].xy.y = (sint16)(uint16)(value - size_y);
    context.corner[2].xy.y = (sint16)(uint16)(value + size_y);
    color = xport_gte_read_data(22);
    for (i = 0; i < 4; ++i, packet += 40)
    {
        quad = mission_packet_span(packet, sizeof(*quad));
        quad->tag = ((packet + 40) & 0xFFFFFF) | 0x09000000;
        quad->rgbc.rgbc = color;
        quad->p[0].uv_attr = 0x459680E0;
        quad->p[1].uv_attr = 0x003880FF;
        quad->p[2].uv.uv = (uint16)(0x9FE0);
        quad->p[3].uv.uv = (uint16)(0x9FFF);
        quad->p[0].xy.xy = center;
        value = (uint16)context.corner[0].xy.x;
        quad->p[2].xy.x = (sint16)(uint16)(value);
        value = (uint16)context.corner[0].xy.y;
        quad->p[1].xy.y = (sint16)(uint16)(value);
        value = (uint16)context.corner[(i & 1) != 0 ? 2 : 1].xy.x;
        quad->p[3].xy.x = (sint16)(uint16)(value);
        quad->p[1].xy.x = (sint16)(uint16)(value);
        value = (uint16)context.corner[(i & 2) != 0 ? 2 : 1].xy.y;
        quad->p[3].xy.y = (sint16)(uint16)(value);
        quad->p[2].xy.y = (sint16)(uint16)(value);
    }
    root = frame->ot;
    target = root + 8;
    next = packet - 40;
    result = effect_light_prepend_chain(mission_ot_span((target + 2) & 0xFFFFFFFC, sizeof(uint32)), (target + 2) & 3, start, next, 0x003880FF);
    frame->packet = packet;
    return;
}

EFFECT_RENDER *effect_render_allocate(void)
{
    uint32 mask = effect_events.render_mask;
    uint32 bit = 1, index = 0;
    if (mask == 0xFFFFFFFF)
        return 0;
    while (mask & bit)
    {
        bit <<= 1;
        ++index;
    }
    effect_events.render_mask = mask | bit;
    return effect_events.render + index;
}

void effect_render_update(void)
{
    EFFECT_RENDER *record;
    uint32 mask, index, value, limit, current;
    FUNCTION_MARKER(0x80089ED8, "ACE3.BIN");
    mask = effect_events.render_mask;
    record = effect_events.render;
    index = 0;
    while (mask)
    {
        if (mask & 1)
        {
            value = record->ticks;
            limit = (sint16)record->duration;
            ++value;
            record->ticks = (uint16)(value);
            if ((sint32)(sint16)value >= (sint32)limit)
            {
                current = effect_events.render_mask;
                effect_events.render_mask = (uint32)(current & ~(1u << (index & 31)));
            }
        }
        ++index;
        mask >>= 1;
        ++record;
    }
}

typedef struct
{
    uint32 color;
    uint16 radius;
    uint16 inner_radius;
    uint32 flags;
    uint32 *ot_entry;
    sint16 position[3];
    XY center;
    XY scr[2];
    sint16 sine[41];
    uint32 mask_command;
    XY first_scr[2];
    uint32 first_flags;
    uint32 rotation[5];
    MATRIX matrix;
    SVECTOR angles;
    uint32 scale[3];
} EFFECT_RENDER_CONTEXT;

typedef char effect_ring_packet_layout_check[sizeof(TILE_1) == 12 && offsetof(TILE_1, p) == 8 && sizeof(POLY_G3) == 28 && offsetof(POLY_G3, p) == 4 && sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

static void effect_render_draw_ring(const EFFECT_RENDER *effect, EFFECT_RENDER_CONTEXT *context, EFFECT_FRAME_CONTEXT *frame)
{
    uint32 *ordering_entry;
    uint32 i, value, other, component, magnitude, duration, counter, product, depth, quotient, size, bucket, ordering, color, high, packet, cursor, next, last, shift, count, angle, sample, radius, x, y, seed, result;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x80089F44, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = effect->position[i];
        other = frame->camera.word[i];
        component = (sint32)(value - other) >> 12;
        magnitude = (sint32)component < 0 ? 0 - component : component;
        context->position[i] = (sint16)(uint16)(component);
        if ((sint32)magnitude > 32767)
            return;
    }
    gte_ldv0(context->position);
    xport_gte_execute(0x180001);
    duration = (sint16)effect->duration;
    counter = (sint16)effect->ticks;
    if ((sint32)duration <= 0)
        duration = 1;
    value = 36;
    if (((sint32)duration >> 1) < (sint32)counter)
        value = (sint32)((duration - counter) * 72) / (sint32)duration;
    context->color = value;
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return;
    depth = xport_gte_read_data(19);
    value = xport_gte_read_data(14);
    context->center.xy = value;
    if (depth == 0)
        depth = 1;
    quotient = (sint32)(counter << 10) / (sint32)duration;
    value = frame->projection;
    other = (sint16)effect->radius;
    product = other * (uint32)(sint32)(sint16)value;
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    product = rsin((sint32)quotient) * size;
    if ((sint32)product < 0)
        product += 4095;
    size = (sint32)product >> 12;
    value = context->center.x;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = size + 120;
    if ((sint32)(size + 160) < (sint32)magnitude)
        return;
    value = context->center.y;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = (sint32)depth >> 3;
    if ((sint32)(size + 120) < (sint32)magnitude)
        return;
    context->radius = (uint16)(size);
    bucket = result - 12;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    ordering = frame->ot;
    color = context->color;
    context->ot_entry = mission_ot_span(ordering + bucket * 4, sizeof(uint32));
    if ((sint32)depth >= 10001)
    {
        product = color * (0xB3B0 - depth);
        high = (((sint64)(sint32)product * (sint64)(sint32)0xB65C6D59) >> 32);
        value = (uint32)((sint32)(high + product) >> 15) - (uint32)((sint32)product >> 31);
        color = value & 255;
    }
    color |= (color << 8) | (color << 16);
    value = (sint16)context->radius;
    context->color = color;
    if ((sint32)value < 2)
    {
        ot_entry = context->ot_entry;
        ordering_entry = ot_entry;
        value = *ordering_entry;
        packet = frame->packet;
        TILE_1 *pixel = mission_packet_span(packet, sizeof(*pixel));
        pixel->tag = value | 0x02000000;
        ot_entry = context->ot_entry;
        ordering_entry = ot_entry;
        *ordering_entry = packet & 0xFFFFFF;
        value = context->color;
        pixel->rgbc.rgbc = value | 0x6A000000;
        result = context->center.xy;
        pixel->p[0].xy = result;
        frame->packet = packet + 12;
        return;
    }
    packet = frame->packet;
    DR_STP *mask_on = mission_packet_span(packet, 8);
    mask_on->tag = ((packet + 8) & 0xFFFFFF) | 0x01000000;
    mask_on->code[0] = 0xE6000001;
    value = (sint16)context->radius;
    shift = 3;
    if ((sint32)value >= 5)
    {
        shift = 2;
        if ((sint32)value >= 10)
            shift = (sint32)value < 44;
    }
    count = 32 >> shift;
    value = (sint16)context->radius;
    other = context->radius;
    cursor = packet + 8;
    if ((sint32)value < 25)
    {
        POLY_G3 *triangle = mission_packet_span(cursor, count * sizeof(*triangle));
        POLY_G3 *last_triangle = triangle + count - 1;
        last = cursor + count * 28 - 28;
        value = (uint16)context->center.x;
        last_triangle->p[2].xy.x = (sint16)(uint16)(value);
        triangle->p[1].xy.x = (sint16)(uint16)(value);
        value = (uint16)context->center.y;
        radius = context->radius;
        value += radius;
        last_triangle->p[2].xy.y = (sint16)(uint16)(value);
        triangle->p[1].xy.y = (sint16)(uint16)(value);
        for (i = 1; i < count; ++i)
        {
            POLY_G3 *following = triangle + 1;
            next = cursor + 28;
            triangle->tag = (next & 0xFFFFFF) | 0x06000000;
            triangle->p[0].rgbc.rgbc = 0x32000000;
            value = context->color;
            triangle->p[2].rgbc.rgbc = value;
            triangle->p[1].rgbc.rgbc = value;
            value = context->center.xy;
            triangle->p[0].xy.xy = value;
            angle = i << shift;
            sample = context->sine[angle];
            radius = (sint16)context->radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = (uint16)context->center.x + (uint32)((sint32)product >> 12);
            following->p[1].xy.x = (sint16)(uint16)(x);
            triangle->p[2].xy.x = (sint16)(uint16)(x);
            sample = context->sine[angle + 8];
            radius = (sint16)context->radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = (uint16)context->center.y + (uint32)((sint32)product >> 12);
            following->p[1].xy.y = (sint16)(uint16)(y);
            triangle->p[2].xy.y = (sint16)(uint16)(y);
            triangle = following;
            cursor = next;
        }
        next = cursor + 28;
        triangle->tag = (next & 0xFFFFFF) | 0x06000000;
        triangle->p[0].rgbc.rgbc = 0x32000000;
        value = context->color;
        triangle->p[2].rgbc.rgbc = value;
        triangle->p[1].rgbc.rgbc = value;
        value = context->center.xy;
        triangle->p[0].xy.xy = value;
        cursor = next;
    }
    else
    {
        context->inner_radius = (uint16)(other - 24);
        POLY_G4 *quad = mission_packet_span(cursor, count * sizeof(*quad));
        POLY_G4 *last_quad = quad + count - 1;
        last = cursor + count * 36 - 36;
        value = (uint16)context->center.x;
        last_quad->p[3].xy.x = (sint16)(uint16)(value);
        last_quad->p[2].xy.x = (sint16)(uint16)(value);
        quad->p[1].xy.x = (sint16)(uint16)(value);
        quad->p[0].xy.x = (sint16)(uint16)(value);
        value = (uint16)context->center.y;
        radius = context->inner_radius;
        value += radius;
        last_quad->p[2].xy.y = (sint16)(uint16)(value);
        quad->p[0].xy.y = (sint16)(uint16)(value);
        value = (uint16)context->center.y;
        radius = context->radius;
        value += radius;
        last_quad->p[3].xy.y = (sint16)(uint16)(value);
        quad->p[1].xy.y = (sint16)(uint16)(value);
        for (i = 1; i < count; ++i)
        {
            POLY_G4 *following = quad + 1;
            next = cursor + 36;
            quad->tag = (next & 0xFFFFFF) | 0x08000000;
            quad->p[0].rgbc.rgbc = 0x3A000000;
            quad->p[2].rgbc.rgbc = 0;
            value = context->color;
            angle = i << shift;
            quad->p[3].rgbc.rgbc = value;
            quad->p[1].rgbc.rgbc = value;
            sample = context->sine[angle];
            radius = (sint16)context->inner_radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = (uint16)context->center.x + (uint32)((sint32)product >> 12);
            following->p[0].xy.x = (sint16)(uint16)(x);
            quad->p[2].xy.x = (sint16)(uint16)(x);
            sample = context->sine[angle + 8];
            radius = (sint16)context->inner_radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = (uint16)context->center.y + (uint32)((sint32)product >> 12);
            following->p[0].xy.y = (sint16)(uint16)(y);
            quad->p[2].xy.y = (sint16)(uint16)(y);
            sample = context->sine[angle];
            radius = (sint16)context->radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = (uint16)context->center.x + (uint32)((sint32)product >> 12);
            following->p[1].xy.x = (sint16)(uint16)(x);
            quad->p[3].xy.x = (sint16)(uint16)(x);
            sample = context->sine[angle + 8];
            radius = (sint16)context->radius;
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = (uint16)context->center.y + (uint32)((sint32)product >> 12);
            following->p[1].xy.y = (sint16)(uint16)(y);
            quad->p[3].xy.y = (sint16)(uint16)(y);
            quad = following;
            cursor = next;
        }
        next = cursor + 36;
        quad->tag = (next & 0xFFFFFF) | 0x08000000;
        quad->p[0].rgbc.rgbc = 0x3A000000;
        quad->p[2].rgbc.rgbc = 0;
        value = context->color;
        quad->p[3].rgbc.rgbc = value;
        quad->p[1].rgbc.rgbc = value;
        cursor = next;
    }
    DR_STP *mask_off = mission_packet_span(cursor, 8);
    mask_off->tag = 0x01000000;
    seed = context->mask_command;
    mask_off->code[0] = seed;
    ot_entry = context->ot_entry;
    value = frame->packet;
    value = effect_light_prepend_chain(ot_entry, 2, value, cursor, seed);
    result = cursor + 8;
    frame->packet = result;
    return;
}

typedef struct
{
    uint32 tag;
    uint32 mask_on;
    POINT_C p[4];
    uint32 mask_off;
} EFFECT_MASKED_QUAD_PACKET;

typedef char effect_masked_quad_packet_layout_check[sizeof(EFFECT_MASKED_QUAD_PACKET) == 44 && offsetof(EFFECT_MASKED_QUAD_PACKET, mask_on) == 4 && offsetof(EFFECT_MASKED_QUAD_PACKET, p) == 8 && offsetof(EFFECT_MASKED_QUAD_PACKET, mask_off) == 40 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

static void effect_render_draw_mesh(const EFFECT_RENDER *effect, EFFECT_RENDER_CONTEXT *context, EFFECT_FRAME_CONTEXT *frame)
{
    uint32 i, j, value, other, component, magnitude, duration, counter, product, scale[3], low, high, a, b, c, d, e, x, y, z, u, v, w, p, q, r, packet, end, depth, bucket;
    uint32 *ordering;
    EFFECT_MASKED_QUAD_PACKET *quad;
    uint32 *entry;
    FUNCTION_MARKER(0x8008A61C, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = effect->position[i];
        other = frame->camera.word[i];
        component = (sint32)(value - other) >> 12;
        magnitude = (sint32)component < 0 ? 0 - component : component;
        context->position[i] = (sint16)(uint16)(component);
        if ((sint32)magnitude > 32767)
            return;
    }
    gte_ldv0(context->position);
    xport_gte_execute(0x486012);
    duration = (sint16)effect->duration;
    counter = (sint16)effect->ticks;
    if ((sint32)duration <= 0)
        duration = 1;
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    value = effect->angles[0];
    context->angles.vx = (sint16)(uint16)(value);
    value = effect->angles[1];
    context->angles.vz = (sint16)(uint16)(counter << 7);
    context->angles.vy = (sint16)(uint16)(value);
    RotMatrixYXZ(&context->angles, &context->matrix);
    value = (sint32)(counter << 10) / (sint32)duration;
    product = rsin((sint32)value);
    value = (sint16)effect->radius;
    product *= value;
    if ((sint32)product < 0)
        product += 4095;
    value = (sint32)product >> 12;
    context->scale[2] = value;
    context->scale[1] = value;
    context->scale[0] = value;
    scale[0] = context->scale[0];
    scale[1] = context->scale[1];
    scale[2] = context->scale[2];
    for (i = 0; i < 4; ++i)
    {
        value = ((uint32)(uint16)context->matrix.m[(i * 2) / 3][(i * 2) % 3] | ((uint32)(uint16)context->matrix.m[(i * 2 + 1) / 3][(i * 2 + 1) % 3] << 16));
        j = (i * 2) % 3;
        low = (uint32)(sint32)(sint16)value * scale[j];
        high = (uint32)((sint32)value >> 16) * scale[(j + 1) % 3];
        context->matrix.m[(i * 2) / 3][(i * 2) % 3] = (sint16)(uint16)((sint32)low >> 12);
        context->matrix.m[(i * 2 + 1) / 3][(i * 2 + 1) % 3] = (sint16)(uint16)((sint32)high >> 12);
    }
    value = (uint16)context->matrix.m[2][2];
    low = (uint32)(sint32)(sint16)value * scale[2];
    context->matrix.m[2][2] = (sint16)(uint16)((sint32)low >> 12);
    a = context->rotation[0];
    b = context->rotation[1];
    c = context->rotation[2];
    d = context->rotation[3];
    e = context->rotation[4];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = (uint16)context->matrix.m[0][0];
    b = ((uint32)(uint16)context->matrix.m[0][2] | ((uint32)(uint16)context->matrix.m[1][0] << 16));
    c = ((uint32)(uint16)context->matrix.m[2][0] | ((uint32)(uint16)context->matrix.m[2][1] << 16));
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = (uint16)context->matrix.m[0][1];
    b = ((uint32)(uint16)context->matrix.m[1][1] | ((uint32)(uint16)context->matrix.m[1][2] << 16));
    c = context->matrix.m[2][1];
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = (uint16)context->matrix.m[0][2];
    b = ((uint32)(uint16)context->matrix.m[1][1] | ((uint32)(uint16)context->matrix.m[1][2] << 16));
    c = (uint16)context->matrix.m[2][2];
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    w = xport_gte_read_data(11);
    xport_gte_write_control(0, (x & 0xFFFF) | (p << 16));
    xport_gte_write_control(1, (u & 0xFFFF) | (y << 16));
    xport_gte_write_control(2, (q & 0xFFFF) | (v << 16));
    xport_gte_write_control(3, (z & 0xFFFF) | (r << 16));
    xport_gte_write_control(4, w);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0);
    xport_gte_write_data(2, 0x0C000000);
    xport_gte_write_data(3, 0);
    xport_gte_write_data(4, 0x10000000);
    xport_gte_write_data(5, 0);
    packet = frame->packet;
    xport_gte_execute(0x280030);
    value = 36;
    if (((sint32)duration >> 1) < (sint32)counter)
        value = (sint32)((duration - counter) * 72) / (sint32)duration;
    context->color = value;
    value = xport_gte_read_control(31);
    context->flags = value;
    value = context->flags;
    if ((sint32)value >= 0)
    {
        value = xport_gte_read_data(13);
        context->scr[0].xy = value;
        value = xport_gte_read_data(14);
        context->scr[1].xy = value;
        a = context->scr[0].xy;
        b = context->scr[1].xy;
        context->first_scr[0].xy = a;
        context->first_scr[1].xy = b;
        depth = xport_gte_read_data(19);
        bucket = (uint32)((sint32)depth >> 3) - 12;
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        value = frame->ot;
        context->ot_entry = mission_ot_span(value + bucket * 4, sizeof(uint32));
        /* The original far-fade branch requires a clamped bucket above 10000 */
    }
    a = context->flags;
    b = context->color;
    end = packet + 36;
    context->first_flags = a;
    context->color = b | (b << 8) | (b << 16);
    for (i = 0; i < 15; ++i)
    {
        a = context->sine[2 + i * 2];
        product = a * 3;
        if ((sint32)product < 0)
            product += 3;
        low = (sint32)product >> 2;
        b = context->sine[10 + i * 2];
        product = b * 3;
        if ((sint32)product < 0)
            product += 3;
        high = (sint32)product >> 2;
        xport_gte_write_data(2, (low & 0xFFFF) | (high << 16));
        xport_gte_write_data(4, (a & 0xFFFF) | (b << 16));
        xport_gte_execute(0x280030);
        value = context->flags;
        if ((sint32)value < 0)
        {
            value = xport_gte_read_control(31);
            context->flags = value;
            value = context->flags;
            if ((sint32)value < 0)
                continue;
            value = xport_gte_read_data(13);
            context->scr[0].xy = value;
            value = xport_gte_read_data(14);
            context->scr[1].xy = value;
        }
        else
        {
            quad = mission_packet_span(packet, sizeof(*quad));
            quad->p[0].rgbc.rgbc = 0x3A000000;
            quad->p[2].rgbc.rgbc = 0;
            value = context->scr[0].xy;
            quad->p[0].xy.xy = value;
            a = context->scr[1].xy;
            quad->mask_on = 0xE6000001;
            quad->mask_off = 0xE6000000;
            quad->p[1].xy.xy = a;
            value = xport_gte_read_control(31);
            context->flags = value;
            value = context->flags;
            if ((sint32)value < 0)
                continue;
            value = context->color;
            quad->p[1].rgbc.rgbc = value;
            value = context->color;
            quad->p[3].rgbc.rgbc = value;
            value = xport_gte_read_data(13);
            context->scr[0].xy = value;
            value = xport_gte_read_data(14);
            context->scr[1].xy = value;
            value = context->scr[0].xy;
            quad->p[2].xy.xy = value;
            value = context->scr[1].xy;
            quad->p[3].xy.xy = value;
            ordering = context->ot_entry;
            entry = ordering;
            value = *entry;
            end += 44;
            quad->tag = value | 0x0A000000;
            value = packet & 0xFFFFFF;
            packet += 44;
            ordering = context->ot_entry;
            entry = ordering;
            *entry = value;
        }
        depth = xport_gte_read_data(19);
        bucket = (uint32)((sint32)depth >> 3) - 12;
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        value = frame->ot;
        context->ot_entry = mission_ot_span(value + bucket * 4, sizeof(uint32));
    }
    value = context->flags;
    if ((sint32)value >= 0)
    {
        value = context->first_flags;
        if ((sint32)value >= 0)
        {
            quad = mission_packet_span(packet, sizeof(*quad));
            quad->p[0].rgbc.rgbc = 0x3A000000;
            quad->p[2].rgbc.rgbc = 0;
            value = context->color;
            quad->mask_on = 0xE6000001;
            quad->p[3].rgbc.rgbc = value;
            quad->p[1].rgbc.rgbc = value;
            value = context->mask_command;
            quad->mask_off = value;
            value = context->scr[0].xy;
            quad->p[0].xy.xy = value;
            value = context->scr[1].xy;
            quad->p[1].xy.xy = value;
            value = context->first_scr[0].xy;
            quad->p[2].xy.xy = value;
            value = context->first_scr[1].xy;
            quad->p[3].xy.xy = value;
            ordering = context->ot_entry;
            entry = ordering;
            value = *entry;
            quad->tag = value | 0x0A000000;
            value = packet & 0xFFFFFF;
            packet += 44;
            ordering = context->ot_entry;
            entry = ordering;
            *entry = value;
        }
    }
    a = context->rotation[0];
    b = context->rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = context->rotation[2];
    d = context->rotation[3];
    e = context->rotation[4];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    frame->packet = packet;
    return;
}

typedef char effect_disc_packet_layout_check[sizeof(POLY_G3) == 28 && offsetof(POLY_G3, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && offsetof(DR_MODE, code) == 4 ? 1 : -1];

static void effect_render_draw_disc(const EFFECT_RENDER *object, EFFECT_RENDER_CONTEXT *state, EFFECT_FRAME_CONTEXT *frame)
{
    P_TAG *ordering_link;
    uint32 a, b, x, y, z, numerator, denominator, ratio, height, scale, product, sine, flags, depth, screen, radius, value, magnitude, bucket, ordering, color, shift, count, packet, cursor, last, index, next, lookup, tag, head;
    POLY_G3 *triangle, *last_triangle;
    DR_MODE *mask_on, *mode_on, *mode_off, *mask_off;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x8008ABF0, "ACE3.BIN");
    a = object->position[0];
    b = frame->camera.word[0];
    x = (sint32)(a - b) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return;
    a = object->position[1];
    b = frame->camera.word[1];
    y = (sint32)(a - b) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return;
    a = object->position[2];
    b = frame->camera.word[2];
    z = (sint32)(a - b) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    a = (sint16)object->ticks;
    denominator = (sint16)object->duration;
    xport_gte_execute(0x180001);
    if ((sint32)denominator <= 0)
        denominator = 1;
    numerator = a << 12;
    ratio = (sint32)numerator / (sint32)denominator;
    height = frame->projection;
    scale = (sint16)object->radius;
    height = (sint32)(height << 16) >> 16;
    product = scale * height;
    sine = rsin((sint32)ratio / 4);
    product *= sine;
    scale = (sint32)product / 4096;
    flags = xport_gte_read_control(31);
    if ((sint32)flags < 0)
        return;
    depth = xport_gte_read_data(19);
    screen = xport_gte_read_data(14);
    if (depth == 0)
        depth = 1;
    if (depth == 0xFFFFFFFF && scale == 0x80000000)
        xport_mips_break(6);
    radius = (sint32)scale / (sint32)depth;
    if ((sint32)radius <= 0)
        return;
    if ((sint32)radius >= 181)
        radius = 180;
    value = (sint32)screen >> 16;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)(magnitude - radius) >= 121)
        return;
    value = (sint32)(screen << 16) >> 16;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)(magnitude - radius) >= 161)
        return;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, 0x326F2FAF);
    bucket = (uint32)((sint32)depth >> 3) - 12;
    xport_gte_execute(0x780010);
    bucket = (uint32)((sint32)bucket >> 3) - 12;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    ordering = frame->ot;
    ordering += bucket << 2;
    state->ot_entry = mission_ot_span(ordering, sizeof(uint32));
    color = xport_gte_read_data(22);
    xport_gte_write_data(8, ratio);
    xport_gte_write_data(6, color);
    state->center.xy = screen;
    state->radius = (uint16)(radius);
    xport_gte_execute(0x780010);
    if ((sint32)radius < 5)
        shift = 3;
    else if ((sint32)radius < 10)
        shift = 2;
    else
        shift = (sint32)radius < 44;
    count = 32 >> shift;
    color = xport_gte_read_data(22);
    packet = frame->packet;
    mask_on = mission_packet_span(packet, 8);
    mode_on = mission_packet_span(packet + 8, 8);
    cursor = packet + 16;
    triangle = mission_packet_span(cursor, count * sizeof(*triangle));
    mask_on->tag = ((packet + 8) & 0xFFFFFF) | 0x01000000;
    mode_on->tag = (cursor & 0xFFFFFF) | 0x01000000;
    last = cursor + count * 28 - 28;
    last_triangle = triangle + count - 1;
    mask_on->code[0] = 0xE6000001;
    mode_on->code[0] = 0xE1000240;
    value = (uint16)state->center.x;
    last_triangle->p[2].xy.x = (sint16)(uint16)(value);
    triangle->p[1].xy.x = (sint16)(uint16)(value);
    a = (uint16)state->center.y;
    b = state->radius;
    value = a + b;
    last_triangle->p[2].xy.y = (sint16)(uint16)(value);
    index = 1;
    triangle->p[1].xy.y = (sint16)(uint16)(value);
    while ((sint32)index < (sint32)count)
    {
        POLY_G3 *following = triangle + 1;
        next = cursor + 28;
        lookup = index << shift;
        triangle->tag = (next & 0xFFFFFF) | 0x06000000;
        triangle->p[0].rgbc.rgbc = color;
        triangle->p[2].rgbc.rgbc = 0x00030303;
        triangle->p[1].rgbc.rgbc = 0x00030303;
        triangle->p[0].xy.xy = screen;
        a = state->sine[lookup];
        b = (sint16)state->radius;
        product = a * b;
        a = (uint16)state->center.x;
        value = a + (uint32)((sint32)product / 4096);
        following->p[1].xy.x = (sint16)(uint16)(value);
        triangle->p[2].xy.x = (sint16)(uint16)(value);
        a = state->sine[lookup + 8];
        b = (sint16)state->radius;
        product = a * b;
        index += 1;
        a = (uint16)state->center.y;
        value = a + (uint32)((sint32)product / 4096);
        following->p[1].xy.y = (sint16)(uint16)(value);
        triangle->p[2].xy.y = (sint16)(uint16)(value);
        triangle = following;
        cursor = next;
    }
    next = cursor + 28;
    last = cursor + 36;
    mode_off = mission_packet_span(next, 8);
    mask_off = mission_packet_span(last, 8);
    triangle->tag = (next & 0xFFFFFF) | 0x06000000;
    triangle->p[0].rgbc.rgbc = color;
    triangle->p[2].rgbc.rgbc = 0x00030303;
    triangle->p[1].rgbc.rgbc = 0x00030303;
    triangle->p[0].xy.xy = screen;
    mode_off->tag = (last & 0xFFFFFF) | 0x01000000;
    mode_off->code[0] = 0xE1000220;
    mask_off->tag = 0x01000000;
    value = state->mask_command;
    mask_off->code[0] = value;
    ot_entry = state->ot_entry;
    head = frame->packet;
    ordering_link = (P_TAG *)ot_entry;
    tag = ordering_link->addr;
    ordering_link = mission_packet_span(last, sizeof(uint32));
    ordering_link->addr = tag;
    ordering_link = (P_TAG *)ot_entry;
    ordering_link->addr = head & 0xFFFFFF;
    value = cursor + 44;
    frame->packet = value;
    return;
}

typedef void (*EFFECT_RENDER_CALLBACK)(const EFFECT_RENDER *effect, EFFECT_RENDER_CONTEXT *context, EFFECT_FRAME_CONTEXT *frame);

static EFFECT_RENDER_CALLBACK const effect_render_callbacks[3] = {
    effect_render_draw_ring,
    effect_render_draw_mesh,
    effect_render_draw_disc,
};

void effect_render_draw(EFFECT_FRAME_CONTEXT *frame)
{
    EFFECT_RENDER_CONTEXT context;
    EFFECT_RENDER_CALLBACK callback;
    const EFFECT_RENDER *record;
    uint32 mask, i, value, a, b, c, kind;
    FUNCTION_MARKER(0x8008B048, "ACE3.BIN");
    mask = effect_events.render_mask;
    if (mask == 0)
        return;
    for (i = 0; i < 32; ++i)
    {
        value = (uint16)rsin((i * 512) >> 2);
        context.sine[i] = (sint16)(uint16)(value);
        if (i < 9)
            context.sine[32 + i] = (sint16)(uint16)(value);
    }
    value = effect_light.flags;
    context.mask_command = (value & 4) != 0 ? 0xE6000001 : 0xE6000000;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
    record = effect_events.render;
    do
    {
        if ((mask & 1) != 0)
        {
            kind = (sint16)record->kind;
            if (kind < 3)
            {
                callback = effect_render_callbacks[kind];
                callback(record, &context, frame);
            }
        }
        mask >>= 1;
        ++record;
    } while (mask != 0);
}

static uint32 effect_drift_seed_offset(uint32 seed_phase, uint32 slot)
{
    return (seed_phase + slot * 28) & 0x1FC;
}

EFFECT_DRIFT *effect_drift_allocate(const EFFECT_DRIFT_POOL *pool)
{
    EFFECT_DRIFT *target;
    uint32 *masks = pool->mask;
    uint32 group, mask, bit;
    FUNCTION_MARKER(0x8007FDB4, "ACE3.BIN");
    target = pool->slots;
    if ((sint32)pool->group_count <= 0)
        return 0;
    for (group = 0; (sint32)group < (sint32)pool->group_count; ++group, ++masks, target += 32)
    {
        mask = *masks;
        if (mask == 0xFFFFFFFF)
            continue;
        bit = 1;
        while (mask & bit)
        {
            bit <<= 1;
            ++target;
        }
        *masks = (mask | bit);
        return target;
    }
    return 0;
}

void effect_drift_update(const EFFECT_DRIFT_POOL *pool)
{
    EFFECT_DRIFT *record;
    uint32 *masks;
    uint32 group, mask, index, value, limit, flags, x, y, z, px, pz, py, shift, gravity;
    FUNCTION_MARKER(0x80084D04, "ACE3.BIN");
    if ((sint32)pool->group_count <= 0)
        return;
    masks = pool->mask;
    record = pool->slots;
    for (group = 0; (sint32)group < (sint32)pool->group_count; ++group, ++masks)
    {
        mask = *masks;
        index = 0;
        while (mask)
        {
            if (mask & 1)
            {
                value = record->ticks + 1;
                record->ticks = (uint16)(value);
                limit = (sint16)(record->duration);
                if ((sint32)(sint16)value >= (sint32)limit)
                {
                    value = *masks;
                    *masks = (value & ~(1u << (index & 31)));
                }
                else
                {
                    flags = record->flags;
                    if (flags & 0xFF)
                    {
                        x = (uint32)(sint16)(record->vector[0]) << 8;
                        y = (uint32)(sint16)(record->vector[1]) << 8;
                        z = (uint32)(sint16)(record->vector[2]) << 8;
                        px = record->position[0];
                        pz = record->position[2];
                        record->position[0] = (uint32)(px + x);
                        py = record->position[1];
                        record->position[2] = (uint32)(pz + z);
                        record->position[1] = (uint32)(py + y);
                        if (flags & 0x1F)
                        {
                            shift = flags & 7;
                            if (shift)
                            {
                                x -= (uint32)((sint32)x >> shift);
                                y -= (uint32)((sint32)y >> shift);
                                z -= (uint32)((sint32)z >> shift);
                            }
                            gravity = flags & 0x18;
                            if (gravity == 8)
                                y += 0x8AA;
                            else if (gravity == 16)
                                y += 0x1A00;
                            else if (gravity)
                                y += 0x3400;
                            record->vector[0] = (uint16)((uint32)((sint32)x >> 8));
                            record->vector[1] = (uint16)((uint32)((sint32)y >> 8));
                            record->vector[2] = (uint16)((uint32)((sint32)z >> 8));
                        }
                    }
                }
            }
            ++index;
            ++record;
            mask >>= 1;
        }
        record = pool->slots + (group + 1) * 32;
    }
}

EFFECT_DRIFT *effect_drift_allocate_pending(void)
{
    EFFECT_DRIFT *record;
    uint32 *masks;
    uint32 group, offset, bits, mask, pending;
    FUNCTION_MARKER(0x80092B04, "ACE3.BIN");
    masks = effect_drift.primary_mask;
    offset = 0;
    for (group = 0; group < 3; ++group)
    {
        bits = *masks;
        if (bits != 0xFFFFFFFF)
        {
            mask = 1;
            record = effect_drift.primary + offset;
            while ((bits & mask) != 0)
            {
                mask <<= 1;
                ++record;
            }
            pending = effect_drift.primary_pending[group];
            *masks = (bits | mask);
            effect_drift.primary_pending[group] = (uint32)(pending | mask);
            return record;
        }
        ++masks;
        offset += 32;
    }
    return 0;
}

typedef struct
{
    XY scr;
    uint16 bucket;
    uint16 size;
} EFFECT_DRIFT_PROJECTED;

typedef struct
{
    uint32 *ot;
    uint32 packet;
    uint32 flags;
    uint32 mode;
    uint32 angle;
    uint16 width;
    uint16 height;
    XY offset;
    XY center;
    uint16 flip;
    uint16 uv_base;
    uint32 visible_mask[3];
    uint32 rotation[5];
    uint32 angle_cache;
    RGBC color;
    EFFECT_DRIFT_PROJECTED projected[96];
} EFFECT_DRIFT_PROJECT_CONTEXT;

typedef char drift_sprite_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

static uint32 effect_drift_project_quad(EFFECT_DRIFT_PROJECT_CONTEXT *state, POLY_FT4 *quad)
{
    uint32 center_x, center_y, flags, angle, previous, offset, trig, cosine, width, height, packed, ax, ay, bx, by, value;
    FUNCTION_MARKER(0x8008591C, "ACE3.BIN");
    center_x = state->center.x;
    flags = state->angle;
    center_y = state->center.y;
    angle = ((uint32)((sint32)flags >> 2)) & 0xFFF;
    if (angle != 0)
    {
        previous = state->angle_cache;
        offset = state->offset.xy;
        if (angle != previous)
        {
            trig = ((uint32)(uint16)rsin(((angle << 2)) >> 2) | ((uint32)(uint16)rcos(((angle << 2)) >> 2) << 16));
            state->angle_cache = angle;
            cosine = trig >> 16;
            xport_gte_write_control(0, ((0 - trig) << 16) | cosine);
            xport_gte_write_control(2, cosine);
            xport_gte_write_control(1, trig << 16);
            xport_gte_write_control(3, 0);
            xport_gte_write_control(4, 0x1000);
        }
        if (offset != 0)
        {
            xport_gte_write_data(0, offset);
            xport_gte_execute(0x486012);
            ax = xport_gte_read_data(25);
            ay = xport_gte_read_data(26);
            center_x += ax;
            center_y += ay;
        }
        height = state->height;
        width = state->width;
        height = (sint16)height;
        width >>= 1;
        packed = width | ((uint32)((sint32)height >> 1) << 16);
        xport_gte_write_data(0, packed);
        by = (sint32)(height + 1) >> 1;
        xport_gte_execute(0x486012);
        packed = width | ((0 - by) << 16);
        ax = xport_gte_read_data(25);
        ay = xport_gte_read_data(26);
        xport_gte_write_data(0, packed);
        xport_gte_execute(0x486012);
        bx = xport_gte_read_data(25);
        by = xport_gte_read_data(26);
    }
    else
    {
        value = state->offset.x;
        offset = state->offset.y;
        center_x += value;
        width = state->width;
        center_y += offset;
        ax = (sint32)(width << 16) >> 17;
        bx = ax;
        height = state->height;
        ay = (sint32)(height << 16) >> 17;
        height = (sint16)height;
        by = (sint32)(1 - height) >> 1;
    }
    quad->p[0].xy.x = (sint16)(uint16)(center_x - ax);
    quad->p[0].xy.y = (sint16)(uint16)(center_y - ay);
    quad->p[1].xy.x = (sint16)(uint16)(center_x + bx);
    quad->p[1].xy.y = (sint16)(uint16)(center_y + by);
    quad->p[2].xy.x = (sint16)(uint16)(center_x - bx);
    quad->p[2].xy.y = (sint16)(uint16)(center_y - by);
    quad->p[3].xy.x = (sint16)(uint16)(center_x + ax);
    value = center_y + ay;
    quad->p[3].xy.y = (sint16)(uint16)(value);
    return value;
}

static uint32 effect_drift_project_style_0(EFFECT_DRIFT_PROJECT_CONTEXT *state, const EFFECT_DRIFT_PROJECTED *geometry, const EFFECT_DRIFT *frame)
{
    uint32 packed, packet, divisor, numerator, index, value, size, coordinate, shift, u, right, v, bottom, mode, bucket, result;
    uint32 *table;
    POLY_FT4 *quad;
    FUNCTION_MARKER(0x80085A94, "ACE3.BIN");
    packed = ((uint32)frame->ticks | ((uint32)frame->duration << 16));
    packet = state->packet;
    quad = mission_packet_span(packet, sizeof(*quad));
    divisor = (sint32)packed >> 16;
    if (divisor == 0)
        divisor = 1;
    value = (sint16)packed;
    numerator = ((value << 2) + value) << 1;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    index = (sint32)numerator / (sint32)divisor;
    coordinate = geometry->scr.xy;
    size = (sint16)geometry->size;
    state->offset.xy = 0;
    state->center.xy = coordinate;
    state->height = (uint16)(size);
    index += 6;
    state->width = (uint16)(size);
    if ((sint32)size < 8)
        state->angle = 0;
    effect_drift_project_quad(state, quad);
    shift = (sint32)size < 17;
    value = (index & 3) << 6;
    u = (sint32)value >> shift;
    right = (uint32)((sint32)(value + 64) >> shift) - 1;
    value = (uint32)((sint32)index >> 2) << 6;
    v = (sint32)value >> shift;
    quad->p[1].uv.v = (uint8)(v);
    quad->p[0].uv.v = (uint8)(v);
    bottom = (uint32)((sint32)(value + 64) >> shift) - 1;
    quad->p[2].uv.u = (uint8)(u);
    quad->p[0].uv.u = (uint8)(u);
    quad->p[3].uv.u = (uint8)(right);
    quad->p[1].uv.u = (uint8)(right);
    quad->p[3].uv.v = (uint8)(bottom);
    quad->p[2].uv.v = (uint8)(bottom);
    mode = state->flags;
    quad->rgbc.rgbc = (uint32)(0x2E838383);
    if (mode != 0)
    {
        quad->p[0].attr = (uint16)(0x4016);
        value = ((mode & 3) << 5) | 0x17;
    }
    else
    {
        quad->p[0].attr = (uint16)(0x4056);
        value = 0x17;
    }
    quad->p[1].attr = (uint16)(value);
    if ((mode & 1) == 0)
        *(uint32 *)mission_packet_span(packet + 40, sizeof(uint32)) = 0xE1000220;
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    value = table[bucket];
    quad->tag = (uint32)(value | ((mode & 1) != 0 ? 0x09000000 : 0x0A000000));
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    table[bucket] = packet & 0xFFFFFF;
    result = packet + ((mode & 1) != 0 ? 40 : 44);
    state->packet = result;
    return result;
}

static uint32 effect_drift_project_style_1(EFFECT_DRIFT_PROJECT_CONTEXT *state, const EFFECT_DRIFT_PROJECTED *geometry, const EFFECT_DRIFT *frame)
{
    uint32 packed, packet, divisor, low, numerator, quotient, size, mode, flags, value, index, bucket, result, shift, u, right, v, bottom, coordinate;
    uint32 *table;
    POLY_FT4 *quad;
    FUNCTION_MARKER(0x80085C8C, "ACE3.BIN");
    packed = ((uint32)frame->ticks | ((uint32)frame->duration << 16));
    packet = state->packet;
    quad = mission_packet_span(packet, sizeof(*quad));
    divisor = (sint32)packed >> 16;
    if (divisor == 0)
        divisor = 1;
    low = (sint16)packed;
    numerator = low << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    size = (sint16)geometry->size;
    xport_gte_write_data(25, 6);
    xport_gte_write_data(9, 10);
    xport_gte_write_data(8, quotient);
    mode = state->mode;
    xport_gte_write_data(26, size);
    if (mode != 0)
        size <<= 1;
    xport_gte_write_data(10, size);
    value = state->angle;
    state->angle = value + (low << ((mode + 5) & 31));
    xport_gte_execute(0x1A8003E);
    flags = state->flags;
    xport_gte_write_control(21, 60);
    xport_gte_write_control(22, 60);
    xport_gte_write_control(23, 60);
    xport_gte_write_data(6, 0x2E535353);
    size = xport_gte_read_data(26);
    index = xport_gte_read_data(25);
    state->height = (uint16)(size);
    state->width = (uint16)(size);
    xport_gte_execute(0x780010);
    if ((flags & 1) == 0)
    {
        quad->p[1].attr = (uint16)(((flags & 3) << 5) | 0x17);
        *(uint32 *)mission_packet_span(packet + 40, sizeof(uint32)) = 0xE1000220;
    }
    else
        quad->p[1].attr = (uint16)(0x37);
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    value = table[bucket];
    quad->tag = (uint32)(value | ((flags & 1) != 0 ? 0x09000000 : 0x0A000000));
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    table[bucket] = packet & 0xFFFFFF;
    result = packet + ((flags & 1) != 0 ? 40 : 44);
    state->packet = result;
    if ((sint32)size < 8)
        state->angle = 0;
    shift = (sint32)size < 17;
    value = (index & 3) << 6;
    u = (sint32)value >> shift;
    right = (uint32)((sint32)(value + 64) >> shift) - 1;
    value = (uint32)((sint32)index >> 2) << 6;
    v = (sint32)value >> shift;
    quad->p[1].uv.v = (uint8)(v);
    quad->p[0].uv.v = (uint8)(v);
    bottom = (uint32)((sint32)(value + 64) >> shift) - 1;
    quad->p[3].uv.v = (uint8)(bottom);
    quad->p[2].uv.v = (uint8)(bottom);
    quad->p[2].uv.u = (uint8)(u);
    quad->p[0].uv.u = (uint8)(u);
    quad->p[3].uv.u = (uint8)(right);
    quad->p[1].uv.u = (uint8)(right);
    quad->p[0].attr = (uint16)(0x40D6);
    value = xport_gte_read_data(22);
    quad->rgbc.rgbc = (uint32)(value);
    coordinate = geometry->scr.xy;
    state->offset.xy = 0;
    state->center.xy = coordinate;
    return effect_drift_project_quad(state, quad);
}

static uint32 effect_drift_project_style_2(EFFECT_DRIFT_PROJECT_CONTEXT *state, const EFFECT_DRIFT_PROJECTED *geometry, const EFFECT_DRIFT *frame)
{
    uint32 packed, mode, packet, divisor, counter, numerator, u, v, flags, color, index, value, height;
    uint32 *ordering;
    POLY_FT4 *quad;
    FUNCTION_MARKER(0x80085EAC, "ACE3.BIN");
    packed = ((uint32)frame->ticks | ((uint32)frame->duration << 16));
    mode = state->mode;
    packet = state->packet;
    quad = mission_packet_span(packet, sizeof(*quad));
    divisor = (sint32)packed >> 16;
    counter = (sint16)packed;
    if (divisor == 0)
        divisor = 1;
    quad->p[0].attr = (uint16)(((mode + 260) << 6) | 0x16);
    numerator = mode == 3 ? counter << 3 : ((counter << 1) + counter) << 1;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    u = (uint32)((sint32)numerator / (sint32)divisor) << 5;
    v = (sint32)mode < 2 ? 0 : mode == 3 ? 64 : 32;
    quad->p[1].uv.uv = (uint16)((v << 8) | (u + 31));
    quad->p[0].uv.uv = (uint16)((v << 8) | u);
    quad->p[2].uv.uv = (uint16)(((v + 31) << 8) | u);
    quad->p[3].uv.uv = (uint16)(((v + 31) << 8) | (u + 31));
    flags = state->flags;
    color = state->color.rgbc;
    quad->rgbc.rgbc = (uint32)(color);
    if ((flags & 1) == 0)
    {
        quad->p[1].attr = (uint16)(((flags & 3) << 5) | 0x18);
        *(uint32 *)mission_packet_span(packet + 40, sizeof(uint32)) = 0xE1000220;
        index = (sint16)geometry->bucket;
        ordering = state->ot;
        value = ordering[index];
        quad->tag = (uint32)(value | 0x0A000000);
        index = (sint16)geometry->bucket;
        ordering = state->ot;
        ordering[index] = packet & 0xFFFFFF;
        value = packet + 44;
    }
    else
    {
        quad->p[1].attr = (uint16)(0x38);
        index = (sint16)geometry->bucket;
        ordering = state->ot;
        value = ordering[index];
        quad->tag = (uint32)(value | 0x09000000);
        index = (sint16)geometry->bucket;
        ordering = state->ot;
        ordering[index] = packet & 0xFFFFFF;
        value = packet + 40;
    }
    state->packet = value;
    height = (sint16)geometry->size;
    state->width = (uint16)(height);
    if ((sint32)v < 64)
        height += (uint32)((sint32)height >> 1);
    value = ((sint32)((uint32)((sint32)height >> 3) - height) >> 1);
    state->height = (uint16)(height);
    state->offset.x = (sint16)(uint16)(0);
    state->offset.y = (sint16)(uint16)(value);
    value = geometry->scr.xy;
    state->center.xy = value;
    return effect_drift_project_quad(state, quad);
}

static uint32 effect_drift_project_style_3(EFFECT_DRIFT_PROJECT_CONTEXT *state, const EFFECT_DRIFT_PROJECTED *geometry, const EFFECT_DRIFT *frame)
{
    uint32 packed, packet, divisor, low, numerator, ratio, value, wave, color, size, width, height, product, high, shift, coordinate, flip, u, v, bucket, result;
    uint32 *table;
    POLY_FT4 *quad;
    FUNCTION_MARKER(0x800860A4, "ACE3.BIN");
    packed = ((uint32)frame->ticks | ((uint32)frame->duration << 16));
    packet = state->packet;
    quad = mission_packet_span(packet, sizeof(*quad));
    divisor = (sint32)packed >> 16;
    low = (sint16)packed;
    state->flip = (uint16)((low + divisor) & 2);
    state->uv_base = (uint16)(((divisor & 1) << 6) + 128);
    if (divisor == 0)
        divisor = 1;
    numerator = low << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (sint32)numerator / (sint32)divisor;
    if ((sint32)ratio < 2048)
    {
        value = rcos((((ratio << 1) & 0x3FFC)) >> 2);
        wave = 4096 - value;
    }
    else
        wave = rcos(((((ratio - 2048) << 1) & 0x3FFC)) >> 2);
    value = 0x1200 - (uint32)((sint32)wave >> 1);
    color = state->color.rgbc;
    xport_gte_write_data(8, value);
    xport_gte_write_data(6, color);
    xport_gte_write_control(21, (color & 255) << 2);
    color = (sint32)color >> 8;
    xport_gte_write_control(22, (color & 255) << 2);
    xport_gte_write_control(23, (uint32)((sint32)color >> 6) & 0x3FC);
    size = (sint16)geometry->size;
    if ((sint32)ratio < 2048)
    {
        product = size * (ratio + 0x2AA);
        high = (((sint64)(sint32)product * (sint64)(sint32)0xC00C00C1) >> 32);
        high += product;
        width = (uint32)((sint32)high >> 11) - (uint32)((sint32)product >> 31);
    }
    else
        width = size;
    xport_gte_execute(0x780010);
    product = size * (wave + 0x2AA);
    if ((sint32)product < 0)
        product += 0xFFF;
    height = (sint32)product >> 12;
    state->width = (uint16)(width);
    shift = (sint32)height < 17;
    state->height = (uint16)(height);
    coordinate = geometry->scr.xy;
    value = (uint32)((sint32)product >> 15) - height;
    value = (sint32)value >> 1;
    state->offset.y = (sint16)(uint16)(value);
    state->offset.x = (sint16)(uint16)(0);
    state->center.xy = coordinate;
    quad->p[0].attr = (uint16)(0x40D6);
    quad->p[1].attr = (uint16)(0x37);
    value = (64 >> shift) - 1;
    quad->p[1].uv.v = (uint8)(0);
    quad->p[0].uv.v = (uint8)(0);
    quad->p[3].uv.v = (uint8)(value);
    quad->p[2].uv.v = (uint8)(value);
    flip = (sint16)state->flip;
    u = (sint16)state->uv_base;
    if (flip != 0)
    {
        value = (sint32)u >> shift;
        quad->p[2].uv.u = (uint8)(value);
        quad->p[0].uv.u = (uint8)(value);
        v = (uint32)((sint32)(u + 64) >> shift) - 1;
    }
    else
    {
        value = (uint32)((sint32)(u + 64) >> shift) - 1;
        quad->p[2].uv.u = (uint8)(value);
        quad->p[0].uv.u = (uint8)(value);
        v = (sint32)u >> shift;
    }
    quad->p[3].uv.u = (uint8)(v);
    quad->p[1].uv.u = (uint8)(v);
    value = xport_gte_read_data(22);
    quad->rgbc.rgbc = (uint32)(value);
    effect_drift_project_quad(state, quad);
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    value = table[bucket];
    quad->tag = (uint32)(value | 0x09000000);
    bucket = (sint16)geometry->bucket;
    table = state->ot;
    table[bucket] = packet & 0xFFFFFF;
    result = packet + 40;
    state->packet = result;
    return result;
}

typedef struct
{
    uint32 rotation[5];
    MATRIX matrix;
    GAME_ANGLES angles;
    GAME_FIXED_VECTOR translation;
    uint32 depth;
    uint32 bucket;
    uint32 shift;
    uint32 radius;
    uint32 projected_size;
    uint32 progress;
    RGBC colors[3];
    SVECTOR vertices[3];
} EFFECT_DRIFT_DRAW_CONTEXT;

typedef struct
{
    uint32 tag;
    POINT_C dots[6];
} EFFECT_DRIFT_DOT_BATCH;

typedef char drift_draw_packet_layout_check[sizeof(TILE_1) == 12 && offsetof(TILE_1, rgbc) == 4 && offsetof(TILE_1, p) == 8 && sizeof(POLY_F3) == 20 && offsetof(POLY_F3, p) == 8 && sizeof(POLY_FT3) == 32 && offsetof(POLY_FT3, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(EFFECT_DRIFT_DOT_BATCH) == 52 && offsetof(EFFECT_DRIFT_DOT_BATCH, dots) == 4 ? 1 : -1];

static void effect_drift_draw_sprite(const EFFECT_DRIFT *effect, EFFECT_FRAME_CONTEXT *frame, EFFECT_DRIFT_DRAW_CONTEXT *context)
{
    uint32 *ordering_entry;
    const uint16 *seed_sample;
    uint32 value, camera, x, y, z, a, b, c, d, e, seed_phase, radius, h, product, depth, size, near_depth, screen, bucket, packet, ordering, color, angle, shift, tx, ty, tz, words, result;
    FUNCTION_MARKER(0x80086320, "ACE3.BIN");
    value = effect->position[0];
    camera = frame->camera.word[0];
    x = (sint32)(value - camera) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return;
    value = effect->position[1];
    camera = frame->camera.word[1];
    y = (sint32)(value - camera) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return;
    value = effect->position[2];
    camera = frame->camera.word[2];
    z = (sint32)(value - camera) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return;
    a = context->rotation[0];
    b = context->rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = context->rotation[2];
    d = context->rotation[3];
    e = context->rotation[4];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    seed_phase = effect_drift.seed_phase;
    seed_sample = effect_noise.sample + (effect_drift_seed_offset(seed_phase, (uint32)(effect - effect_drift.secondary)) >> 1);
    angle = ((uint32)seed_sample[0] | ((uint32)seed_sample[1] << 16));
    xport_gte_execute(0x180001);
    context->angles.value[0] = (uint16)(angle);
    context->angles.value[1] = (uint16)((angle) >> 16);
    value = effect->flags;
    if ((value & 0x100) != 0)
    {
        value = effect->ticks;
        context->angles.value[2] = (uint16)(value << 8);
    }
    else
        context->angles.value[2] = (uint16)(0);
    radius = (sint16)(effect->radius);
    context->radius = radius;
    h = (sint16)frame->projection;
    product = radius * h;
    depth = xport_gte_read_data(19);
    if ((sint32)depth <= 0)
    {
        (void)(h);
        return;
    }
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    near_depth = depth - size;
    if ((sint32)near_depth < ((sint32)h >> 4))
        return;
    screen = xport_gte_read_data(14);
    y = (sint32)screen >> 16;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return;
    x = (sint16)screen;
    if ((sint32)x < 0)
        x = 0 - x;
    result = (sint32)size < 3;
    if ((sint32)(size + 160) < (sint32)x)
    {
        (void)(result);
        return;
    }
    if ((sint32)size < 3)
    {
        bucket = (sint32)depth >> 3;
        packet = frame->packet;
        TILE_1 *dot = mission_packet_span(packet, sizeof(*dot));
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        ordering = frame->ot;
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        value = *ordering_entry;
        dot->tag = (uint32)(value | 0x02000000);
        ordering = frame->ot;
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        *ordering_entry = packet & 0xFFFFFF;
        angle = ((uint32)context->angles.value[0] | ((uint32)context->angles.value[1] << 16));
        value = (sint16)context->angles.value[2];
        value += (uint32)((sint32)angle >> 16);
        value += angle;
        dot->rgbc.rgbc = (uint32)((value & 0x800) != 0 ? 0x68636363 : 0x68181818);
        dot->p[0].xy = (uint32)(screen);
        result = packet + 12;
        frame->packet = result;
        {
            (void)(result);
            return;
        }
    }
    context->projected_size = size;
    context->depth = depth;
    h = (sint16)frame->projection;
    if ((sint32)near_depth < (sint32)h)
    {
        value = context->radius;
        context->shift = 4;
        context->radius = value << 4;
    }
    else
        context->shift = 0;
    tx = xport_gte_read_data(25);
    ty = xport_gte_read_data(26);
    tz = xport_gte_read_data(27);
    shift = context->shift;
    if (shift != 0)
    {
        tx <<= shift & 31;
        ty <<= shift & 31;
        tz <<= shift & 31;
    }
    value = context->radius;
    game_build_scaled_rotation(&context->angles, value, &context->matrix);
    game_compose_rotation(&context->matrix);
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xFFFFF800);
    xport_gte_write_data(2, 0x04000000);
    xport_gte_write_data(3, 512);
    xport_gte_write_data(4, 0xFD000000);
    value = context->angles.value[0];
    xport_gte_write_data(5, (value & 7) << 9);
    size = context->projected_size;
    depth = context->depth;
    xport_gte_execute(0x280030);
    bucket = (sint32)depth >> 3;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    if ((sint32)size >= 7)
    {
        ordering = frame->ot;
        angle = (sint16)context->angles.value[0];
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        value = *ordering_entry;
        packet = frame->packet;
        POLY_FT3 *triangle = mission_packet_span(packet, sizeof(*triangle));
        triangle->tag = (uint32)(value | 0x07000000);
        ordering = frame->ot;
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        *ordering_entry = packet & 0xFFFFFF;
        triangle->p[0].uv_attr = (uint32)((angle & 1) != 0 ? 0x45D610E0 : 0x45D61FEF);
        triangle->p[1].uv_attr = (uint32)(0x003810EF);
        triangle->p[2].uv.uv = (uint16)(0x1FE0);
        words = 8;
        color = 0x24000000;
        value = xport_gte_read_data(12);
        triangle->p[0].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        triangle->p[1].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        triangle->p[2].xy.xy = (uint32)(value);
    }
    else
    {
        ordering = frame->ot;
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        value = *ordering_entry;
        packet = frame->packet;
        POLY_F3 *flat = mission_packet_span(packet, sizeof(*flat));
        flat->tag = (uint32)(value | 0x04000000);
        ordering = frame->ot;
        value = frame->packet;
        ordering_entry = mission_ot_span(ordering + bucket * 4, sizeof(*ordering_entry));
        *ordering_entry = value & 0xFFFFFF;
        words = 5;
        color = 0x20000000;
        value = xport_gte_read_data(12);
        flat->p[0].xy = (uint32)(value);
        value = xport_gte_read_data(13);
        flat->p[1].xy = (uint32)(value);
        value = xport_gte_read_data(14);
        flat->p[2].xy = (uint32)(value);
    }
    xport_gte_execute(0x1400006);
    value = xport_gte_read_data(24);
    color |= (sint32)value < 0 ? 0x636363 : 0x181818;
    packet = frame->packet;
    RGBC *packet_color = mission_packet_span(packet + 4, sizeof(*packet_color));
    packet_color->rgbc = color;
    result = frame->packet + words * 4;
    frame->packet = result;
    {
        (void)(result);
        return;
    }
}

static const uint32 drift_style_colors[7][3] = {
    {0x00209060u, 0x00708070u, 0x0040A010u}, {0x00A05080u, 0x00808080u, 0x009020A0u}, {0x00707070u, 0x00805010u, 0x00901040u}, {0x00908080u, 0x00905020u, 0x00A04010u}, {0x00051360u, 0x00030290u, 0x00038090u}, {0x00904030u, 0x00A08090u, 0x00B02060u}, {0x00194850u, 0x00182870u, 0x002010C0u},
};

static void effect_drift_draw_shape(const EFFECT_DRIFT *effect, EFFECT_FRAME_CONTEXT *frame, EFFECT_DRIFT_DRAW_CONTEXT *context)
{
    const uint16 *seed_sample;
    const uint32 *geometry;
    const uint16 *noise;
    uint32 value, camera, x, y, z, a, b, c, d, e, seed_phase, angle, counter, divisor, radius, numerator, ratio, product, result, depth, h, size, near_depth, screen, fog, material, count, limit, bucket, shift, tx, ty, tz, packet, next, phase, modulation, word, lo, mid, hi, seed, ordering, start, tail;
    FUNCTION_MARKER(0x80086A0C, "ACE3.BIN");
    value = effect->position[0];
    camera = frame->camera.word[0];
    x = (sint32)(value - camera) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return;
    value = effect->position[1];
    camera = frame->camera.word[1];
    y = (sint32)(value - camera) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return;
    value = effect->position[2];
    camera = frame->camera.word[2];
    z = (sint32)(value - camera) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return;
    a = context->rotation[0];
    b = context->rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = context->rotation[2];
    d = context->rotation[3];
    e = context->rotation[4];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    seed_phase = effect_drift.seed_phase;
    geometry = effect_noise.vector;
    xport_gte_execute(0x180001);
    seed_sample = effect_noise.sample + (effect_drift_seed_offset(seed_phase, (uint32)(effect - effect_drift.secondary)) >> 1);
    angle = ((uint32)seed_sample[0] | ((uint32)seed_sample[1] << 16));
    context->angles.value[0] = (uint16)(angle);
    context->angles.value[1] = (uint16)((angle) >> 16);
    value = effect->flags;
    counter = (sint16)(effect->ticks);
    context->angles.value[2] = (uint16)((value & 0x100) != 0 ? counter << 6 : 0);
    geometry += ((uint32)((sint32)angle >> 4) & 0x1C) >> 2;
    noise = effect_noise.sample + (angle & 63) * 2;
    divisor = (sint16)(effect->duration);
    radius = (sint16)(effect->radius);
    if (divisor == 0)
        divisor = 1;
    numerator = counter << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (sint32)numerator / (sint32)divisor;
    context->progress = ratio;
    if ((sint32)ratio >= 0xAAB)
        noise += ((counter << 1) & 12) >> 1;
    value = (uint32)rsin((sint32)(ratio >> 2));
    product = radius * value;
    result = product;
    if ((sint32)result < 0)
        result += 4095;
    radius = (sint32)result >> 12;
    depth = xport_gte_read_data(19);
    context->radius = radius;
    if ((sint32)depth <= 0)
    {
        (void)(result);
        return;
    }
    h = (sint16)frame->projection;
    product = radius * h;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    near_depth = depth - size;
    if ((sint32)near_depth < ((sint32)h >> 4))
        return;
    screen = xport_gte_read_data(14);
    y = (sint32)screen >> 16;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return;
    x = (sint16)screen;
    if ((sint32)x < 0)
        x = 0 - x;
    result = depth - 1024;
    if ((sint32)(size + 160) < (sint32)x)
    {
        (void)(result);
        return;
    }
    fog = (sint32)result >> 2;
    if ((sint32)fog >= 4096)
        return;
    if ((sint32)fog < 0)
        fog = 0;
    xport_gte_write_data(8, fog);
    value = xport_gte_read_data(25);
    context->translation.word[0] = value;
    value = xport_gte_read_data(26);
    context->translation.word[1] = value;
    value = xport_gte_read_data(27);
    context->translation.word[2] = value;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, 0x6A000000);
    material = effect->style;
    value = material & 255;
    if (value >= 7)
        value = 6;
    xport_gte_write_data(20, drift_style_colors[value][0]);
    xport_gte_write_data(21, drift_style_colors[value][1]);
    xport_gte_write_data(22, drift_style_colors[value][2]);
    count = (material >> 8) & 31;
    xport_gte_execute(0xF8002A);
    h = (sint16)frame->projection;
    if ((sint32)near_depth < (sint32)h)
    {
        value = context->radius;
        context->shift = 4;
        context->radius = value << 4;
    }
    else
        context->shift = 0;
    if ((count & 16) != 0)
    {
        limit = (sint32)(size - 4) >> 1;
        count &= 15;
        if ((sint32)limit <= 0)
            limit = 1;
        if ((sint32)limit < (sint32)count)
            count = limit;
    }
    bucket = (sint32)depth >> 3;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    context->bucket = bucket;
    value = xport_gte_read_data(20);
    context->colors[0].rgbc = value;
    value = xport_gte_read_data(21);
    context->colors[1].rgbc = value;
    value = xport_gte_read_data(22);
    context->colors[2].rgbc = value;
    value = context->radius;
    game_build_scaled_rotation(&context->angles, value << 2, &context->matrix);
    game_compose_rotation(&context->matrix);
    seed = xport_gte_read_control(2);
    tx = context->translation.word[0];
    ty = context->translation.word[1];
    shift = context->shift;
    tz = context->translation.word[2];
    if (shift != 0)
    {
        tx <<= shift & 31;
        ty <<= shift & 31;
        tz <<= shift & 31;
    }
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    noise += 2;
    phase = context->progress - 512;
    packet = frame->packet;
    modulation = phase + (tx & 1023);
    while ((sint32)count > 0)
    {
        EFFECT_DRIFT_DOT_BATCH *batch = mission_packet_span(packet, sizeof(*batch));
        xport_gte_write_data(8, modulation);
        value = context->colors[0].rgbc;
        xport_gte_write_data(20, value);
        value = context->colors[1].rgbc;
        xport_gte_write_data(21, value);
        value = context->colors[2].rgbc;
        xport_gte_write_data(22, value);
        word = *geometry;
        ++geometry;
        xport_gte_execute(0xF8002A);
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[0].vy = (sint16)(uint16)(lo);
        context->vertices[0].vz = (sint16)(uint16)(hi);
        context->vertices[0].vx = (sint16)(uint16)(mid);
        word = *geometry;
        ++geometry;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[1].vx = (sint16)(uint16)(lo);
        context->vertices[1].vy = (sint16)(uint16)(hi);
        context->vertices[1].vz = (sint16)(uint16)(mid);
        word = *geometry;
        ++geometry;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[2].vx = (sint16)(uint16)(hi);
        context->vertices[2].vy = (sint16)(uint16)(mid);
        context->vertices[2].vz = (sint16)(uint16)(lo);
        value = xport_gte_read_data(20);
        batch->dots[0].rgbc.rgbc = (uint32)(value);
        value = xport_gte_read_data(20);
        batch->dots[1].rgbc.rgbc = (uint32)(value);
        value = xport_gte_read_data(21);
        batch->dots[2].rgbc.rgbc = (uint32)(value);
        value = xport_gte_read_data(21);
        batch->dots[3].rgbc.rgbc = (uint32)(value);
        value = xport_gte_read_data(22);
        batch->dots[4].rgbc.rgbc = (uint32)(value);
        value = xport_gte_read_data(22);
        batch->dots[5].rgbc.rgbc = (uint32)(value);
        gte_ldv3(context->vertices, context->vertices + 1, context->vertices + 2);
        word = *geometry;
        ++geometry;
        xport_gte_execute(0x280030);
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[0].vy = (sint16)(uint16)(mid);
        context->vertices[0].vz = (sint16)(uint16)(lo);
        context->vertices[0].vx = (sint16)(uint16)(hi);
        word = *geometry;
        ++geometry;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[1].vx = (sint16)(uint16)(mid);
        context->vertices[1].vy = (sint16)(uint16)(lo);
        context->vertices[1].vz = (sint16)(uint16)(hi);
        word = *geometry;
        ++geometry;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        context->vertices[2].vx = (sint16)(uint16)(lo);
        context->vertices[2].vy = (sint16)(uint16)(hi);
        context->vertices[2].vz = (sint16)(uint16)(mid);
        value = xport_gte_read_data(12);
        batch->dots[0].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        batch->dots[1].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        batch->dots[2].xy.xy = (uint32)(value);
        gte_ldv3(context->vertices, context->vertices + 1, context->vertices + 2);
        next = packet + 52;
        batch->tag = (uint32)((next & 0xFFFFFF) | 0x0C000000);
        xport_gte_execute(0x280030);
        value = ((uint32)noise[0] | ((uint32)noise[1] << 16));
        noise += 2;
        modulation = phase + (value & 1023);
        value = xport_gte_read_data(12);
        batch->dots[3].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        batch->dots[4].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        batch->dots[5].xy.xy = (uint32)(value);
        --count;
        packet = next;
        seed = next;
    }
    tail = packet - 52;
    bucket = context->bucket;
    ordering = frame->ot;
    ordering += bucket * 4;
    start = frame->packet;
    result = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, start, tail, seed);
    frame->packet = packet;
    {
        (void)(result);
        return;
    }
}

typedef uint32 (*EFFECT_DRIFT_PROJECT_CALLBACK)(EFFECT_DRIFT_PROJECT_CONTEXT *state, const EFFECT_DRIFT_PROJECTED *geometry, const EFFECT_DRIFT *frame);

static EFFECT_DRIFT_PROJECT_CALLBACK const effect_drift_project_callbacks[4] = {
    effect_drift_project_style_0,
    effect_drift_project_style_1,
    effect_drift_project_style_2,
    effect_drift_project_style_3,
};

void effect_drift_project(EFFECT_FRAME_CONTEXT *frame)
{
    EFFECT_DRIFT_PROJECT_CONTEXT context;
    EFFECT_DRIFT_PROJECTED *output;
    const EFFECT_DRIFT_PROJECTED *geometry;
    EFFECT_DRIFT_PROJECT_CALLBACK callback;
    const EFFECT_DRIFT *record;
    uint32 total, group, mask, bit, value, camera, x, y, z, radius, ax, ay, az, doubled, packed, h, product, depth, size, screen, bucket, a, b, c, d, e, count, kind;
    FUNCTION_MARKER(0x800854B8, "ACE3.BIN");
    total = 0;
    for (group = 0; group < 3; ++group)
    {
        mask = effect_drift.primary_mask[group];
        context.visible_mask[group] = 0;
        record = effect_drift.primary + group * 32;
        bit = 0;
        output = context.projected + total;
        while (mask != 0)
        {
            if ((mask & 1) == 0 || (sint16)(record->ticks) < 0)
                goto next_effect;
            value = record->position[0];
            camera = frame->camera.word[0];
            x = (sint32)(value - camera) >> 12;
            if (x + 0x8000 > 0xFFFF)
                goto next_effect;
            value = record->position[1];
            camera = frame->camera.word[1];
            y = (sint32)(value - camera) >> 12;
            if (y + 0x8000 > 0xFFFF)
                goto next_effect;
            value = record->position[2];
            camera = frame->camera.word[2];
            z = (sint32)(value - camera) >> 12;
            if (z + 0x8000 > 0xFFFF)
                goto next_effect;
            radius = record->radius;
            output->size = (uint16)(radius);
            radius = (sint16)radius;
            doubled = 0;
            if ((sint32)radius < 256)
            {
                ax = (sint32)x < 0 ? 0 - x : x;
                ay = (sint32)y < 0 ? 0 - y : y;
                az = (sint32)z < 0 ? 0 - z : z;
                if ((sint32)(ax + ay + az) < 1024)
                    doubled = 1;
            }
            if (doubled != 0)
            {
                packed = ((x << 1) & 0xFFFF) | (y << 17);
                z <<= 1;
            }
            else
                packed = (x & 0xFFFF) | (y << 16);
            xport_gte_write_data(0, packed);
            xport_gte_write_data(1, z);
            h = (sint16)frame->projection;
            xport_gte_execute(0x180001);
            product = radius * h;
            depth = xport_gte_read_data(27);
            value = (sint32)depth < (sint32)h;
            depth = (sint32)depth >> doubled;
            if (value != 0)
                goto next_effect;
            if (depth == 0)
                xport_mips_break(7);
            if (depth == 0xFFFFFFFF && product == 0x80000000)
                xport_mips_break(6);
            size = (sint32)product / (sint32)depth;
            if (size == 0)
                goto next_effect;
            screen = xport_gte_read_data(14);
            x = (sint16)screen;
            if ((sint32)x < 0)
                x = 0 - x;
            value = (sint32)(size + 160) < (sint32)x;
            output->size = (uint16)(size);
            if (value != 0)
                goto next_effect;
            y = (sint32)screen >> 16;
            if ((sint32)y < 0)
                y = 0 - y;
            if ((sint32)(size + 120) < (sint32)y)
                goto next_effect;
            bucket = (uint32)((sint32)depth >> 3) - 1;
            output->scr.xy = screen;
            if ((sint32)bucket >= 4095)
                bucket = 4094;
            else if ((sint32)bucket < 0)
                bucket = 0;
            output->bucket = (uint16)(bucket);
            ++output;
            ++total;
            value = context.visible_mask[group];
            context.visible_mask[group] = value | (1u << (bit & 31));
        next_effect:
            ++bit;
            ++record;
            mask >>= 1;
        }
    }
    if (total == 0)
        return;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    c = xport_gte_read_control(2);
    d = xport_gte_read_control(3);
    e = xport_gte_read_control(4);
    context.rotation[2] = c;
    context.rotation[3] = d;
    context.rotation[4] = e;
    xport_gte_write_data(1, 0);
    a = frame->ot;
    b = frame->packet;
    context.angle_cache = 0xFFFFFFFF;
    context.ot = mission_ot_span(a, 4095 * sizeof(*context.ot));
    context.packet = b;
    context.color.rgbc = game_runtime.scene_assets.environment.drift_color | 0x2E000000;
    count = 0;
    for (group = 0; group < 2; ++group)
    {
        mask = context.visible_mask[group];
        record = effect_drift.primary + group * 32;
        output = context.projected + count;
        while (mask != 0)
        {
            if ((mask & 1) != 0)
            {
                value = (sint16)frame->angles.value[2];
                context.angle = 0 - value;
                packed = record->style;
                kind = (packed >> 4) & 15;
                if (kind >= 4)
                    kind = 3;
                context.flags = (packed >> 8) & 3;
                context.mode = packed & 15;
                callback = effect_drift_project_callbacks[kind];
                ++count;
                geometry = output;
                ++output;
                callback(&context, geometry, record);
            }
            mask >>= 1;
            ++record;
        }
    }
    value = context.packet;
    frame->packet = value;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = context.rotation[2];
    d = context.rotation[3];
    e = context.rotation[4];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    return;
}

void effect_drift_draw(EFFECT_FRAME_CONTEXT *frame)
{
    EFFECT_DRIFT_DRAW_CONTEXT context;
    const EFFECT_DRIFT *effect;
    uint32 a, b, c, group, bits, offset = 0;
    FUNCTION_MARKER(0x8008700C, "ACE3.BIN");
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
    for (group = 0; group < 10; ++group)
    {
        bits = effect_drift.secondary_mask[group];
        if (bits != 0)
        {
            effect = effect_drift.secondary + offset;
            do
            {
                if ((bits & 1) != 0)
                {
                    if ((sint16)(effect->style) == 0)
                        effect_drift_draw_sprite(effect, frame, &context);
                    else
                        effect_drift_draw_shape(effect, frame, &context);
                }
                ++effect;
                bits >>= 1;
            } while (bits != 0);
        }
        offset += 32;
    }
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
}

typedef struct
{
    XY xy;
    uint16 z;
} EFFECT_LIGHT_INPUT;

typedef struct
{
    SVECTOR angles;
    MATRIX matrix;
    EFFECT_LIGHT_INPUT vector;
} EFFECT_LIGHT_INIT_CONTEXT;

typedef struct
{
    EFFECT_LIGHT_INPUT vector;
    XY center;
    uint32 scale[3];
    uint16 angles[3];
    MATRIX matrix;
    GAME_FIXED_VECTOR translation;
    XY ring[17];
} EFFECT_LIGHT_HALO_CONTEXT;

typedef struct
{
    EFFECT_LIGHT_INPUT vector;
    XY center;
    uint32 ratio;
    uint32 power;
    SVECTOR angles;
    SVECTOR vertex[3];
    MATRIX matrix;
    MATRIX quarter;
} EFFECT_LIGHT_FLARE_CONTEXT;

void effect_light_init(EFFECT_LIGHT *state)
{
    EFFECT_LIGHT_INIT_CONTEXT context;
    uint32 value, a, b, c, d, e, index;
    FUNCTION_MARKER(0x8008B178, "ACE3.BIN");
    context.angles.vz = 0;
    context.angles.vx = game_runtime.render_settings.light_angles[0][0];
    context.angles.vy = game_runtime.render_settings.light_angles[0][1];
    RotMatrixYXZ(&context.angles, &context.matrix);
    a = ((uint32)(uint16)context.matrix.m[0][0] | ((uint32)(uint16)context.matrix.m[0][1] << 16));
    b = ((uint32)(uint16)context.matrix.m[0][2] | ((uint32)(uint16)context.matrix.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)context.matrix.m[1][1] | ((uint32)(uint16)context.matrix.m[1][2] << 16));
    d = ((uint32)(uint16)context.matrix.m[2][0] | ((uint32)(uint16)context.matrix.m[2][1] << 16));
    e = (uint16)context.matrix.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x4000);
    state->flags = (uint32)(0);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    c = xport_gte_read_data(11);
    context.vector.xy.x = (sint16)(uint16)(a);
    context.vector.xy.y = (sint16)(uint16)(b);
    context.vector.z = (uint16)(c);
    a = context.vector.xy.xy;
    b = (uint16)context.vector.z;
    state->dir_xy = (uint32)(a);
    state->dir_z = (uint16)(b);
    for (index = 0; index < 4; ++index)
    {
        value = ((index & 1) << 1) - 1;
        context.vector.xy.x = (sint16)(uint16)(value * 442);
        value = (index & 2) - 1;
        context.vector.xy.y = (sint16)(uint16)(value * 442);
        context.vector.z = (uint16)(0x4000);
        xport_gte_write_data(0, context.vector.xy.xy);
        xport_gte_write_data(1, (uint32)(uint16)context.vector.z);
        xport_gte_execute(0x486012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        state->corner[index].x = (uint16)(a);
        state->corner[index].y = (uint16)(b);
        state->corner[index].z = (uint16)(c);
    }
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    c = xport_gte_read_data(11);
    context.vector.xy.x = (sint16)(uint16)(a);
    context.vector.xy.y = (sint16)(uint16)(b);
    context.vector.z = (uint16)(c);
    a = context.vector.xy.xy;
    b = (uint16)context.vector.z;
    state->inner_xy = (uint32)(a);
    state->inner_z = (uint16)(b);
    return;
}

uint32 effect_light_prepend_chain(uint32 *ot_tag, uint32 ot_lane, uint32 first, uint32 tail, uint32 seed)
{
    uint32 tail_lane = (tail + 2) & 3;
    uint32 ot_shift = 24 - ot_lane * 8;
    uint32 tail_shift = 24 - tail_lane * 8;
    uint32 ot_mask = 0xFFFFFFFFu >> ot_shift;
    uint32 tail_mask = 0xFFFFFFFFu >> tail_shift;
    uint32 *tail_tag = mission_packet_span((tail + 2) & 0xFFFFFFFC, sizeof(*tail_tag));
    uint32 merged = *ot_tag << ot_shift;
    uint32 first_link = first << 8;

    if (ot_lane != 3)
        merged |= seed & (0xFFFFFFu >> (ot_lane * 8));
    *tail_tag = (*tail_tag & ~tail_mask) | (merged >> tail_shift);
    *ot_tag = (*ot_tag & ~ot_mask) | (first_link >> ot_shift);
    return first_link;
}

void effect_light_draw_halo(EFFECT_LIGHT *state, uint32 color, EFFECT_FRAME_CONTEXT *frame)
{
    POLY_G4 *quad;
    DR_TPAGE *mask_packet;
    EFFECT_LIGHT_HALO_CONTEXT context;
    uint32 camera, a, b, c, d, e, gap, product, size, third, value, x, y, start, packet, i, j, angle, next, low, high, ordering;
    FUNCTION_MARKER(0x8008B310, "ACE3.BIN");
    camera = frame->camera.word[1];
    if ((sint32)camera < (sint32)0xF8800000)
        return;
    a = state->dir_xy;
    b = state->dir_z;
    context.vector.xy.xy = a;
    context.vector.z = (uint16)(b);
    value = (sint16)context.vector.xy.y;
    gap = 0 - (uint32)((sint32)camera >> 12) - value;
    if ((sint32)gap > 32767)
    {
        a = (uint16)context.vector.xy.x;
        context.vector.xy.y = (sint16)(uint16)((sint32)gap >> 1);
        b = (uint16)context.vector.z;
        context.vector.xy.x = (sint16)(uint16)((sint16)a >> 1);
        context.vector.z = (uint16)((sint16)b >> 1);
    }
    else
        context.vector.xy.y = (sint16)(uint16)(gap);
    a = context.vector.xy.xy;
    xport_gte_write_data(0, a);
    a = (uint16)context.vector.z;
    xport_gte_write_data(1, a);
    xport_gte_execute(0x180001);
    value = (sint16)frame->projection;
    product = (((value << 2) + value) * 16 - ((value << 2) + value)) << 4;
    if ((sint32)product < 0)
        product += 4095;
    size = (sint32)product >> 12;
    context.scale[1] = size;
    if ((sint32)gap <= 8192)
    {
        third = (uint32)(((sint64)(sint32)size * 0x55555556) >> 32) - (uint32)((sint32)product >> 31);
        value = (size - third) * gap;
        if ((sint32)value < 0)
            value += 8191;
        context.scale[0] = (uint32)((sint32)value >> 13) + third;
    }
    else
    {
        context.scale[0] = size;
        if ((sint32)gap > 16384)
            gap = 16384;
    }
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return;
    value = xport_gte_read_data(14);
    context.center.xy = value;
    x = (sint16)context.center.x;
    size = context.scale[1];
    if ((sint32)x < 0)
        x = 0 - x;
    if ((sint32)(size + 160) < (sint32)x)
        return;
    y = (sint16)context.center.y;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, color);
    xport_gte_write_data(8, (sint32)gap >> 3);
    start = frame->packet;
    packet = start + 8;
    xport_gte_execute(0x780010);
    mask_packet = (DR_TPAGE *)mission_packet_span(start, sizeof(*mask_packet));
    mask_packet->tag = (packet & 0xFFFFFF) | 0x01000000;
    mask_packet->code[0] = 0xE6000002;
    color = xport_gte_read_data(22);
    value = frame->angles.value[2];
    context.angles[0] = 0;
    context.angles[1] = 0;
    context.angles[2] = (uint16)(0 - value);
    rotation_build(&context.matrix, context.angles);
    context.scale[2] = 0;
    for (i = 0; i < 4; ++i)
    {
        value = ((uint32)(uint16)context.matrix.m[(i * 2) / 3][(i * 2) % 3] | ((uint32)(uint16)context.matrix.m[(i * 2 + 1) / 3][(i * 2 + 1) % 3] << 16));
        j = (i * 2) % 3;
        low = (uint32)(sint32)(sint16)value * context.scale[j];
        high = (uint32)((sint32)value >> 16) * context.scale[(j + 1) % 3];
        context.matrix.m[(i * 2) / 3][(i * 2) % 3] = (sint16)(uint16)((sint32)low >> 12);
        context.matrix.m[(i * 2 + 1) / 3][(i * 2 + 1) % 3] = (sint16)(uint16)((sint32)high >> 12);
    }
    value = (uint16)context.matrix.m[2][2];
    low = (uint32)(sint32)(sint16)value * context.scale[2];
    context.matrix.m[2][2] = (sint16)(uint16)((sint32)low >> 12);
    a = ((uint32)(uint16)context.matrix.m[0][0] | ((uint32)(uint16)context.matrix.m[0][1] << 16));
    b = ((uint32)(uint16)context.matrix.m[0][2] | ((uint32)(uint16)context.matrix.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)context.matrix.m[1][1] | ((uint32)(uint16)context.matrix.m[1][2] << 16));
    d = ((uint32)(uint16)context.matrix.m[2][0] | ((uint32)(uint16)context.matrix.m[2][1] << 16));
    e = (uint16)context.matrix.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    x = (sint16)context.center.x;
    y = (sint16)context.center.y;
    context.translation.word[2] = 0;
    context.translation.word[0] = x;
    context.translation.word[1] = y;
    a = context.translation.word[0];
    b = context.translation.word[1];
    xport_gte_write_control(5, a);
    c = context.translation.word[2];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    angle = (game_runtime.frame_counter << 6) & 4095;
    a = (uint16)rcos((sint32)angle);
    angle = (angle << 1) & 4095;
    b = (uint16)context.center.x;
    context.center.x = (sint16)(uint16)(b + ((sint16)a >> 9));
    a = (uint16)rsin((sint32)angle);
    context.vector.xy.y = (sint16)(uint16)(4096);
    context.vector.z = (uint16)(0);
    context.vector.xy.x = (sint16)(uint16)((sint16)a >> 4);
    for (i = 0; i < 16; ++i)
    {
        a = context.vector.xy.xy;
        xport_gte_write_data(0, a);
        a = (uint16)context.vector.z;
        xport_gte_write_data(1, a);
        xport_gte_execute(0x480012);
        angle = (angle + 0x555) & 4095;
        next = i + 1;
        if (next < 16)
        {
            a = (uint16)rsin((sint32)(next << 8));
            b = (uint16)rsin((sint32)angle);
            context.vector.xy.y = (sint16)rcos((sint32)(next << 8));
            context.vector.xy.x = (sint16)(uint16)(a + ((sint16)b >> 3));
        }
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        context.ring[i].x = (sint16)(uint16)a;
        context.ring[i].y = (sint16)(uint16)b;
        context.ring[i + 1].x = (sint16)(uint16)c;
    }
    value = context.ring[0].xy;
    context.ring[16].xy = value;
    for (i = 0; i < 16; i += 2)
    {
        next = packet + 36;
        quad = (POLY_G4 *)mission_packet_span(packet, sizeof(*quad));
        quad->tag = (uint32)((next & 0xFFFFFF) | 0x08000000);
        quad->p[0].rgbc.rgbc = (uint32)(0x3A030303);
        quad->p[1].rgbc.rgbc = (uint32)(color);
        quad->p[2].rgbc.rgbc = (uint32)(0x00030303);
        quad->p[3].rgbc.rgbc = (uint32)(0x00030303);
        value = context.ring[i].xy;
        quad->p[0].xy.xy = (uint32)(value);
        value = context.center.xy;
        quad->p[1].xy.xy = (uint32)(value);
        value = context.ring[(i + 1)].xy;
        quad->p[2].xy.xy = (uint32)(value);
        value = context.ring[(i + 2)].xy;
        quad->p[3].xy.xy = (uint32)(value);
        packet = next;
    }
    mask_packet = (DR_TPAGE *)mission_packet_span(packet, sizeof(*mask_packet));
    mask_packet->tag = 0x01000000;
    mask_packet->code[0] = 0xE6000000;
    ordering = frame->ot;
    start = frame->packet;
    ordering += 120;
    effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, start, packet, packet);
    frame->packet = packet + 8;
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    e = (uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = (uint32)game_runtime.mission_matrices.view.t[0];
    b = (uint32)game_runtime.mission_matrices.view.t[1];
    xport_gte_write_control(5, a);
    c = (uint32)game_runtime.mission_matrices.view.t[2];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return;
}

typedef struct
{
    XY projected;
    uint32 uv_attr[2];
    uint16 uv[2];
} EFFECT_LIGHT_RING_WORK;

typedef struct
{
    uint8 type;
    uint8 threshold;
    uint16 palette;
    sint16 radius;
    sint16 coefficient;
} LIGHT_RING_STYLE;

static const LIGHT_RING_STYLE light_ring_styles[10] = {
    {0, 48, 17238, 46, -4500}, {1, 16, 17110, 22, -3600}, {3, 48, 16982, 8, -2700}, {1, 32, 17046, 16, -2100}, {2, 52, 17046, 6, -1700}, {2, 40, 17046, 9, -1400}, {3, 48, 16982, 3, -900}, {3, 28, 16982, 2, 400}, {1, 56, 17046, 9, 2000}, {1, 0, 17174, 14, 2800},
};

static void effect_light_draw_rings(uint32 center, uint32 size, EFFECT_FRAME_CONTEXT *frame)
{
    POLY_FT4 *quad;
    EFFECT_LIGHT_RING_WORK work;
    uint32 packet, index, threshold, strength, coefficient, value, product, type, color, uv, edge, palette, radius, link, corner, x, y, tail_seed, ordering, start, tail, result;
    FUNCTION_MARKER(0x8008B7AC, "ACE3.BIN");
    packet = frame->packet;
    for (index = 0; index < 10; ++index)
    {
        const LIGHT_RING_STYLE *style = &light_ring_styles[index];
        threshold = style->threshold;
        value = size;
        strength = value - threshold;
        tail_seed = strength << 8;
        if ((sint32)strength <= 0)
            continue;
        coefficient = style->coefficient;
        value = (sint16)center;
        product = value * coefficient;
        color = strength | (strength << 16) | (strength << 8);
        if ((sint32)product < 0)
            product += 0xFFF;
        value = ((sint32)center >> 16);
        y = value * coefficient;
        work.projected.x = (sint16)(uint16)((sint32)product >> 12);
        if ((sint32)y < 0)
            y += 0xFFF;
        work.projected.y = (sint16)(uint16)((sint32)y >> 12);
        type = style->type;
        if (type < 2)
        {
            uv = (type << 5) + 192;
            edge = (type << 5) + 223;
            color |= 0x2E000000;
            link = (packet << 8) + 0x2800;
            palette = style->palette;
            work.uv[0] = (uint16)(uv | 0x3F00);
            work.uv_attr[1] = edge | 0x382000;
            work.uv[1] = (uint16)(edge | 0x3F00);
            work.uv_attr[0] = (palette << 16) | uv | 0x2000;
            radius = style->radius;
            for (corner = 0; corner < 4; ++corner)
            {
                quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
                quad->tag = (uint32)((link >> 8) | 0x09000000);
                quad->rgbc.rgbc = (uint32)(color);
                value = work.uv_attr[0];
                quad->p[0].uv_attr = (uint32)(value);
                value = work.uv_attr[1];
                quad->p[1].uv_attr = (uint32)(value);
                value = work.uv[0];
                quad->p[2].uv.uv = (uint16)(value);
                value = work.uv[1];
                quad->p[3].uv.uv = (uint16)(value);
                value = work.projected.xy;
                quad->p[3].xy.xy = (uint32)(value);
                x = work.projected.x;
                value = (corner & 1) != 0 ? x + radius : x - radius;
                quad->p[2].xy.x = (sint16)(uint16)(value);
                value = (uint16)quad->p[2].xy.x;
                quad->p[0].xy.x = (sint16)(uint16)(value);
                value = (uint16)work.projected.x;
                quad->p[1].xy.x = (sint16)(uint16)(value);
                y = work.projected.y;
                value = (corner >> 1) != 0 ? y + radius : y - radius;
                quad->p[1].xy.y = (sint16)(uint16)(value);
                link += 0x2800;
                value = (uint16)quad->p[1].xy.y;
                quad->p[0].xy.y = (sint16)(uint16)(value);
                value = (uint16)work.projected.y;
                quad->p[2].xy.y = (sint16)(uint16)(value);
                packet += 40;
            }
            tail_seed = y;
        }
        else
        {
            uv = ((type & 1) << 4) + 224;
            edge = ((type & 1) << 4) + 239;
            quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
            quad->tag = (uint32)(((packet + 40) & 0xFFFFFF) | 0x09000000);
            quad->rgbc.rgbc = (uint32)(color | 0x2E000000);
            palette = style->palette;
            quad->p[1].uv_attr = (uint32)(edge | 0x380000);
            quad->p[2].uv.uv = (uint16)(uv | 0xF00);
            quad->p[3].uv.uv = (uint16)(edge | 0xF00);
            quad->p[0].uv_attr = (uint32)((palette << 16) | uv);
            radius = style->radius;
            value = (uint16)work.projected.x;
            quad->p[0].xy.x = (sint16)(uint16)(value - radius);
            value = (uint16)work.projected.y;
            quad->p[0].xy.y = (sint16)(uint16)(value - radius);
            value = (uint16)work.projected.x;
            quad->p[1].xy.x = (sint16)(uint16)(value + radius);
            value = (uint16)work.projected.y;
            quad->p[1].xy.y = (sint16)(uint16)(value - radius);
            value = (uint16)work.projected.x;
            quad->p[2].xy.x = (sint16)(uint16)(value - radius);
            value = (uint16)work.projected.y;
            quad->p[2].xy.y = (sint16)(uint16)(value + radius);
            value = (uint16)work.projected.x;
            quad->p[3].xy.x = (sint16)(uint16)(value + radius);
            value = (uint16)work.projected.y;
            quad->p[3].xy.y = (sint16)(uint16)(value + radius);
            packet += 40;
            tail_seed = edge;
        }
    }
    tail = packet - 40;
    ordering = frame->ot;
    start = frame->packet;
    result = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, start, tail, tail_seed);
    frame->packet = packet;
    {
        (void)(result);
        return;
    }
}

void effect_light_draw_flare(EFFECT_LIGHT *state, uint32 flags, uint32 distance, EFFECT_FRAME_CONTEXT *frame)
{
    uint32 *ordering_entry;
    POLY_FT4 *quad;
    TILE *tile;
    EFFECT_LIGHT_FLARE_CONTEXT context;
    uint32 packet, a, b, c, d, e, value, result, x, y, sum, numerator, high, ratio, remaining, size, color, link, i, ordering, start, tail, seed, random;
    FUNCTION_MARKER(0x8008BA8C, "ACE3.BIN");
    a = state->dir_xy;
    b = state->dir_z;
    context.vector.xy.xy = a;
    context.vector.z = (uint16)(b);
    a = context.vector.xy.xy;
    xport_gte_write_data(0, a);
    b = (uint16)context.vector.z;
    xport_gte_write_data(1, b);
    xport_gte_execute(0x180001);
    packet = frame->packet;
    quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
    quad->p[1].uv_attr = (uint32)(0x0039209F);
    quad->p[2].uv.uv = (uint16)(0x3F80);
    quad->rgbc.rgbc = (uint32)(0x2E808080);
    quad->p[0].uv_attr = (uint32)(0x4F162080);
    quad->p[3].uv.uv = (uint16)(0x3F9F);
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return;
    value = xport_gte_read_data(14);
    context.center.xy = value;
    x = (sint16)context.center.x;
    if ((sint32)x < 0)
        x = 0 - x;
    if ((sint32)x >= 177)
        return;
    y = (sint16)context.center.y;
    if ((sint32)y < 0)
        y = 0 - y;
    result = flags & 0xFF00;
    if ((sint32)y >= 137)
    {
        (void)(result);
        return;
    }
    if (result != 0)
    {
        a = ((uint32)state->corner[0].x | ((uint32)state->corner[0].y << 16));
        xport_gte_write_data(0, a);
        a = state->corner[0].z;
        xport_gte_write_data(1, a);
        a = ((uint32)state->corner[1].x | ((uint32)state->corner[1].y << 16));
        xport_gte_write_data(2, a);
        a = state->corner[1].z;
        xport_gte_write_data(3, a);
        a = ((uint32)state->corner[2].x | ((uint32)state->corner[2].y << 16));
        xport_gte_write_data(4, a);
        a = state->corner[2].z;
        xport_gte_write_data(5, a);
        xport_gte_execute(0x280030);
        sum = x + y;
        numerator = (312 - sum) << 12;
        high = (((sint64)(sint32)numerator * (sint64)(sint32)0xD20D20D3) >> 32);
        ratio = (uint32)((sint32)(high + numerator) >> 8) - (uint32)((sint32)numerator >> 31);
        context.ratio = ratio;
        value = xport_gte_read_data(12);
        quad->p[0].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        quad->p[1].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        quad->p[2].xy.xy = (uint32)(value);
        a = ((uint32)state->corner[3].x | ((uint32)state->corner[3].y << 16));
        xport_gte_write_data(0, a);
        a = state->corner[3].z;
        xport_gte_write_data(1, a);
        xport_gte_execute(0x180001);
        link = (packet + 40) << 8;
        x = (sint16)context.center.x;
        y = (sint16)context.center.y;
        quad->tag = (uint32)((link >> 8) | 0x09000000);
        if ((sint32)x < 0)
            x = 0 - x;
        if ((sint32)y < 0)
            y = 0 - y;
        sum = x + y;
        remaining = 208 - sum;
        value = xport_gte_read_data(14);
        quad->p[3].xy.xy = (uint32)(value);
        packet += 40;
        seed = sum;
        if ((sint32)remaining > 0)
        {
            a = (uint16)context.center.x;
            b = (uint16)context.center.y;
            context.angles.vx = 0;
            context.angles.vy = 0;
            context.angles.vz = (sint16)(uint16)((a << 2) + (b << 3));
            RotMatrix(&context.angles, &context.matrix);
            a = ((uint32)(uint16)context.matrix.m[0][0] | ((uint32)(uint16)context.matrix.m[0][1] << 16));
            b = ((uint32)(uint16)context.matrix.m[0][2] | ((uint32)(uint16)context.matrix.m[1][0] << 16));
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = ((uint32)(uint16)context.matrix.m[1][1] | ((uint32)(uint16)context.matrix.m[1][2] << 16));
            d = ((uint32)(uint16)context.matrix.m[2][0] | ((uint32)(uint16)context.matrix.m[2][1] << 16));
            e = (uint16)context.matrix.m[2][2];
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            color = (uint32)((sint32)remaining >> 2) + 23;
            size = (uint32)((sint32)remaining >> 2) + 14;
            context.power = color | (color << 8) | (color << 16) | 0x2E000000;
            context.angles.vz = 1024;
            RotMatrix(&context.angles, &context.quarter);
            a = (sint16)context.center.x;
            xport_gte_write_control(5, a);
            a = (sint16)context.center.y;
            xport_gte_write_control(6, a);
            a = frame->projection;
            xport_gte_write_control(7, a);
            value = 0 - size;
            context.vertex[0].vx = (sint16)(uint16)(value);
            context.vertex[0].vy = (sint16)(uint16)(value);
            context.vertex[0].vz = (sint16)(uint16)(0);
            context.vertex[1].vx = (sint16)(uint16)(0);
            context.vertex[1].vy = (sint16)(uint16)(value);
            context.vertex[1].vz = (sint16)(uint16)(0);
            context.vertex[2].vx = (sint16)(uint16)(value);
            context.vertex[2].vy = (sint16)(uint16)(0);
            context.vertex[2].vz = (sint16)(uint16)(0);
            a = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
            xport_gte_write_data(0, a);
            a = (uint16)context.vertex[0].vz;
            xport_gte_write_data(1, a);
            a = ((uint32)(uint16)context.vertex[1].vx | ((uint32)(uint16)context.vertex[1].vy << 16));
            xport_gte_write_data(2, a);
            a = (uint16)context.vertex[1].vz;
            xport_gte_write_data(3, a);
            a = ((uint32)(uint16)context.vertex[2].vx | ((uint32)(uint16)context.vertex[2].vy << 16));
            xport_gte_write_data(4, a);
            a = (uint16)context.vertex[2].vz;
            xport_gte_write_data(5, a);
            link += 0x2800;
            for (i = 0; i < 4; ++i)
            {
                if (i != 0)
                    game_compose_rotation(&context.quarter);
                value = context.center.xy;
                quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
                quad->p[3].xy.xy = (uint32)(value);
                xport_gte_execute(0x280030);
                quad->tag = (uint32)((link >> 8) | 0x09000000);
                quad->p[2].uv.uv = (uint16)(0x3FA0);
                quad->p[0].uv_attr = (uint32)(0x4F5620A0);
                quad->p[1].uv_attr = (uint32)(0x003920BF);
                quad->p[3].uv.uv = (uint16)(0x3FBF);
                value = context.power;
                quad->rgbc.rgbc = (uint32)(value);
                value = xport_gte_read_data(12);
                quad->p[0].xy.xy = (uint32)(value);
                value = xport_gte_read_data((i & 1) != 0 ? 14 : 13);
                quad->p[1].xy.xy = (uint32)(value);
                value = xport_gte_read_data((i & 1) != 0 ? 13 : 14);
                quad->p[2].xy.xy = (uint32)(value);
                link += 0x2800;
                packet += 40;
            }
            a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
            b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
            d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
            e = (uint16)game_runtime.mission_matrices.view.m[2][2];
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            a = (uint32)game_runtime.mission_matrices.view.t[0];
            b = (uint32)game_runtime.mission_matrices.view.t[1];
            xport_gte_write_control(5, a);
            c = (uint32)game_runtime.mission_matrices.view.t[2];
            xport_gte_write_control(6, b);
            xport_gte_write_control(7, c);
            seed = 0x003920BF;
        }
        tail = packet - 40;
        ordering = frame->ot;
        start = frame->packet;
        ordering += 0x3FF8;
        result = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, start, tail, seed);
        frame->packet = packet;
    }
    else
    {
        if ((flags & 255) == 0)
            return;
        numerator = (312 - (x + y)) << 12;
        high = (((sint64)(sint32)numerator * (sint64)(sint32)0xD20D20D3) >> 32);
        result = ((sint32)numerator >> 31);
        ratio = (uint32)((sint32)(high + numerator) >> 8) - result;
        context.ratio = ratio;
    }
    flags &= 255;
    if (flags == 0)
    {
        (void)(result);
        return;
    }
    ratio = context.ratio;
    if ((sint32)ratio > 2048)
    {
        if (flags == 1)
        {
            value = state->flags;
            state->flags = (uint32)(value | 1);
            ratio = context.ratio;
            value = ((ratio - 2048) * 5) << 4;
            if ((sint32)value < 0)
                value += 2047;
            color = (sint32)value >> 11;
            color |= (color << 16) | (color << 8);
            ordering = frame->ot;
            ordering_entry = mission_ot_span(ordering, sizeof(*ordering_entry));
            value = *ordering_entry;
            packet = frame->packet;
            tile = (TILE *)mission_packet_span(packet, sizeof(*tile));
            tile->tag = (uint32)(value | 0x03000000);
            ordering = frame->ot;
            ordering_entry = mission_ot_span(ordering, sizeof(*ordering_entry));
            *ordering_entry = packet & 0xFFFFFF;
            tile->p[0].x = (sint16)(uint16)(-160);
            tile->rgbc.rgbc = (uint32)(color | 0x62000000);
            tile->p[0].y = (sint16)(uint16)(-120);
            tile->wh = (uint32)(0x00F00140);
            frame->packet = packet + 16;
        }
        else
        {
            value = state->flags;
            state->flags = (uint32)(value | 2);
            ratio = context.ratio;
            state->intensity = (uint32)((ratio - 2048) << 1);
        }
        ratio = context.ratio;
        if ((sint32)ratio > 3072)
            context.ratio = 12288 - ((ratio << 1) + ratio);
    }
    ratio = context.ratio;
    size = (uint32)((sint32)(ratio - 512) >> 5) + 24;
    if ((sint32)size >= 241)
        size = 240;
    else
    {
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        size += random & 7;
    }
    value = frame->camera.word[1];
    distance += 0xFFC80000;
    result = value - distance;
    if ((sint32)distance < (sint32)value)
    {
        result = (sint32)result >> 14;
        size -= result;
    }
    if ((sint32)size <= 0)
    {
        (void)(result);
        return;
    }
    context.power = size;
    {
        (void)(effect_light_draw_rings(context.center.xy, context.power, frame));
        return;
    }
}

void effect_noise_init(EFFECT_NOISE *state)
{
    uint32 index, axis, sample, seed, value, sign, sum, packed, components[3];
    FUNCTION_MARKER(0x80084B84, "ACE3.BIN");
    for (index = 0; index < 528; ++index)
    {
        seed = random_seed;
        value = psyq_rand_next(&seed);
        random_seed = seed;
        seed = random_seed;
        sign = psyq_rand_next(&seed);
        random_seed = seed;
        if ((sign & 1) != 0)
            value = 0 - value;
        state->sample[index] = (uint16)(value);
    }
    for (index = 0; index < 106; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            sum = 0;
            for (sample = 0; sample < 4; ++sample)
            {
                seed = random_seed;
                value = psyq_rand_next(&seed);
                random_seed = seed;
                sum += value & 0x7FF;
            }
            components[axis] = sum >> 2;
        }
        packed = ((components[0] - 1024) & 0x7FF) | (((components[1] - 1024) & 0x7FE) << 10) | ((components[2] - 1024) << 21);
        state->vector[index] = (uint32)(packed);
    }
    return;
}

void effect_pools_reset(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800849EC, "ACE3.BIN");
    for (index = 0; index < 3; ++index)
    {
        effect_drift.primary_mask[index] = (uint32)(0);
        effect_drift.primary_pending[index] = (uint32)(0);
    }
    for (index = 0; index < 10; ++index)
        effect_drift.secondary_mask[9 - index] = (uint32)(0);
    effect_events.timed_mask = 0;
    for (index = 0; index < 32; ++index)
        effect_events.timed[31 - index].callback = NULL;
    effect_events.render_mask = (uint32)(0);
    effect_events.aircraft.ticks = (uint32)(0);
    effect_events.aircraft.control = (uint32)(0);
    effect_events.segment_flags = (uint32)(0);
    effect_events.burst.active = (uint32)(0);
}

void effect_transients_clear(void)
{
    uint32 index, mask, flags;
    FUNCTION_MARKER(0x80092AB8, "ACE3.BIN");
    index = 0;
    do
    {
        mask = effect_drift.primary_pending[index];
        flags = effect_drift.primary_mask[index];
        index += 1;
        effect_drift.primary_pending[index - 1] = (uint32)(0);
        effect_drift.primary_mask[index - 1] = (uint32)(flags & ~mask);
    } while (index < 3);
    effect_events.render_mask = (uint32)(0);
    effect_events.segment_flags = (uint32)(0);
    effect_events.burst.active = (uint32)(0);
}

void effect_system_init(void)
{
    uint32 state, keys, value;
    sint32 mission;
    FUNCTION_MARKER(0x80084A6C, "ACE3.BIN");
    state = arena_alloc(15188);
    effect_drift.seed_phase = (state + 0x10C0) & 0x1FC;
    effect_noise_init(&effect_noise);
    effect_light_init(&effect_light);
    effect_aircraft_init();
    effect_pools_reset();
    value = game_runtime.progress.flow_flags & 16;
    if (value == 0)
        return;
    mission = (sint16)game_runtime.selection.mission;
    keys = pad_state[0].buttons;
    if (mission == 13)
        return;
    if (mission == 42 || mission == 56)
        return;
    if ((keys & 0x1008) == 0x1008 && mission != 32 && mission != 47 && mission != 53 && mission != 55)
    {
        value = 7;
        game_runtime.selection.weapon_code[1] = (uint8)(value);
    }
    else if ((keys & 0x4008) == 0x4008)
    {
        value = 10;
        game_runtime.selection.weapon_code[1] = (uint8)(value);
    }
    else if ((keys & 0x8008) == 0x8008)
    {
        value = 5;
        game_runtime.selection.weapon_code[1] = (uint8)(value);
    }
    else
    {
        if ((keys & 0x2008) == 0x2008)
            game_runtime.selection.weapon_code[2] = (uint8)(9);
        return;
    }
    game_runtime.selection.weapon_code[2] = (uint8)(value);
    return;
}

typedef struct
{
    uint32 count;
    uint32 height;
    uint32 step;
} EFFECT_LAYER_PARAM;

typedef struct
{
    XY xy;
    uint16 z;
} EFFECT_LAYER_VERTEX;

typedef struct
{
    EFFECT_LAYER_PARAM layer[2];
    SVECTOR source;
    EFFECT_LAYER_VERTEX vertex[3];
    uint32 base_color;
    uint32 draw_color;
    uint32 uv_attr[2];
    uint16 uv[2];
} EFFECT_LAYER_CONTEXT;

void effect_layer_draw(EFFECT_FRAME_CONTEXT *frame)
{
    EFFECT_LAYER_CONTEXT context;
    EFFECT_LAYER_PARAM *params;
    const uint16 *random;
    uint32 packet, value, height_offset, i, word, count, color, opcode, high_opcode, layer, px, pz, x_shift, z_shift, height, brightness, remaining, position, zword, delta, xyword, flags, xy, magnitude, texture, u, right, far_u, depth, length, ordering, bucket, head;
    FUNCTION_MARKER(0x8008D9F4, "ACE3.BIN");
    packet = frame->packet;
    value = frame->camera.word[1];
    height_offset = 0 - (uint32)((sint32)value >> 12);
    for (i = 0; i < 2; ++i)
    {
        count = game_runtime.scene_assets.environment.layer[i].count;
        value = game_runtime.scene_assets.environment.layer[i].height;
        params = &context.layer[i];
        params->height = (sint32)(height_offset + value) >> (i + 1);
        if (count >= 129)
            count = 128;
        params->step = 0 - ((game_runtime.scene_assets.environment.layer[i].slope >> 1));
        params->count = count;
    }
    word = game_runtime.scene_assets.environment.layer_color;
    context.base_color = (word & 0xFFFFFF) | 0x2E000000;
    opcode = (sint32)word >> 24;
    opcode = opcode != 0 ? 89 : 57;
    high_opcode = opcode << 16;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    for (layer = 0; layer < 2; ++layer)
    {
        params = &context.layer[layer];
        px = frame->camera.word[0];
        pz = frame->camera.word[2];
        x_shift = (sint32)px >> 13;
        z_shift = (sint32)pz >> 13;
        if (layer == 0)
        {
            x_shift += (uint32)((sint32)px >> 15);
            z_shift += (uint32)((sint32)pz >> 15);
        }
        height = params->height;
        color = context.base_color;
        brightness = (sint32)height >> 2;
        if ((sint32)brightness >= 4096)
            continue;
        if ((sint32)brightness > 0)
        {
            xport_gte_write_data(6, color);
            xport_gte_write_data(8, brightness);
            xport_gte_execute(0x780010);
            color = xport_gte_read_data(22);
        }
        if ((color << 8) == 0)
            continue;
        xport_gte_write_data(6, color);
        context.draw_color = color;
        remaining = params->count;
        random = effect_noise.sample;
        while ((sint32)remaining > 0)
        {
            do
            {
                word = ((uint32)random[0] | ((uint32)random[1] << 16));
                context.source.vy = (sint16)(uint16)(height);
                position = (sint32)word >> 16;
                context.source.vx = (sint16)(uint16)(((word - x_shift) & 0x7FFF) - 0x4000);
                context.source.vz = (sint16)(uint16)(((position - z_shift) & 0x7FFF) - 0x4000);
                value = ((uint32)(uint16)context.source.vx | ((uint32)(uint16)context.source.vy << 16));
                xport_gte_write_data(0, value);
                value = (uint16)context.source.vz;
                xport_gte_write_data(1, value);
                delta = ((remaining & 7) << 4) + 0x26A;
                xport_gte_execute(0x180001);
                zword = (uint16)context.source.vz;
                if ((remaining & 1) != 0)
                    delta = 0 - delta;
                delta += zword;
                context.vertex[2].z = (uint16)(delta);
                context.vertex[1].z = (uint16)(delta);
                xyword = ((uint32)(uint16)context.source.vx | ((uint32)(uint16)context.source.vy << 16));
                context.vertex[0].z = (uint16)(zword);
                delta = xyword + ((remaining & 15) << 4) + 0x555;
                context.vertex[1].xy.xy = xyword;
                context.vertex[2].xy.xy = delta;
                context.vertex[0].xy.xy = delta;
                flags = xport_gte_read_control(31);
                if ((sint32)flags < 0)
                    break;
                xy = xport_gte_read_data(14);
                magnitude = (sint16)xy;
                if ((sint32)magnitude < 0)
                    magnitude = 0 - magnitude;
                if ((sint32)magnitude >= 241)
                    break;
                magnitude = (sint32)xy >> 16;
                if ((sint32)magnitude < 0)
                    magnitude = 0 - magnitude;
                if ((sint32)magnitude >= 201)
                    break;
                gte_ldv3(context.vertex, context.vertex + 1, context.vertex + 2);
                POLY_FT4 *quad = mission_packet_span(packet, 38);
                quad->p[0].xy.xy = xy;
                xport_gte_execute(0x280030);
                texture = (layer << 6) + 0x4DD6;
                if ((sint32)height > 0)
                    texture += 128;
                u = (remaining & 3) << 5;
                right = u + 31;
                far_u = (remaining & 3) << 6;
                quad->p[0].uv_attr = (texture << 16) | u | 0x2000;
                quad->p[1].uv_attr = high_opcode | right | 0x2000;
                quad->p[2].uv.uv = (uint16)(u | 0x2F00);
                quad->p[3].uv.uv = (uint16)(right | 0x2F00);
                context.uv_attr[0] = (texture << 16) | far_u;
                context.uv_attr[1] = high_opcode | (far_u + 63);
                context.uv[0] = (uint16)(far_u | 0x1F00);
                context.uv[1] = (uint16)((far_u + 63) | 0x1F00);
                flags = xport_gte_read_control(31);
                if ((sint32)flags < 0)
                    break;
                xport_gte_execute(0x168002E);
                length = 0x09000000;
                if (opcode == 89)
                {
                    length = 0x0A000000;
                    uint32 *draw_mode = mission_packet_span(packet + 40, sizeof(*draw_mode));
                    *draw_mode = 0xE1000220;
                }
                depth = xport_gte_read_data(7);
                if ((sint32)depth >= 557)
                {
                    value = context.draw_color;
                    quad->rgbc.rgbc = value;
                    if ((sint32)depth >= 4095)
                        depth = 4094;
                }
                else
                {
                    if ((sint32)depth < 300)
                        break;
                    xport_gte_write_data(8, (556 - depth) << 4);
                    xport_gte_execute(0x780010);
                    value = xport_gte_read_data(22);
                    quad->rgbc.rgbc = value;
                }
                ordering = frame->ot;
                bucket = ordering + (depth << 2);
                uint32 *ot_in = mission_ot_span(bucket, sizeof(*ot_in));
                head = *ot_in;
                quad->tag = head | length;
                ordering = frame->ot;
                uint32 *ot_out = mission_ot_span(ordering + (depth << 2), sizeof(*ot_out));
                *ot_out = packet & 0xFFFFFF;
                if ((sint32)depth < 1536)
                {
                    quad = mission_packet_span(packet, sizeof(*quad));
                    value = context.uv_attr[0];
                    word = context.uv_attr[1];
                    quad->p[0].uv_attr = value;
                    quad->p[1].uv_attr = word;
                    value = (uint32)context.uv[0];
                    word = (uint32)context.uv[1];
                    quad->p[2].uv_attr = value;
                    quad->p[3].uv_attr = word;
                }
                value = xport_gte_read_data(12);
                quad->p[1].xy.xy = value;
                value = xport_gte_read_data(13);
                quad->p[2].xy.xy = value;
                value = xport_gte_read_data(14);
                quad->p[3].xy.xy = value;
                packet += 40;
                if (opcode == 89)
                    packet += 4;
            } while (0);
            --remaining;
            value = params->step;
            random += 2;
            height += value;
        }
    }
    frame->packet = packet;
    return;
}

typedef struct
{
    uint32 screen[2], depth, color, scale;
} EFFECT_RIBBON_PROJECTION;

uint32 effect_spawn_player(uint32 mode)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    MATRIX matrix;
    uint32 a, b, c, d, e, x, y, z, value, kind;
    FUNCTION_MARKER(0x800713B0, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    rotation_build(&matrix, pose_owner_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    a = game_runtime.aircraft_mission_metadata.spawn_vector[0];
    xport_gte_write_data(0, a);
    b = game_runtime.aircraft_mission_metadata.spawn_vector[1];
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    player_spawn_pose.position[0] = (uint32)(x);
    y = xport_gte_read_data(26);
    player_spawn_pose.position[1] = (uint32)(y);
    z = xport_gte_read_data(27);
    player_spawn_pose.position[2] = (uint32)(z);
    x = player_spawn_pose.position[0] << 4;
    player_spawn_pose.position[0] = (uint32)(x);
    y = player_spawn_pose.position[1];
    z = player_spawn_pose.position[2];
    y <<= 4;
    z <<= 4;
    player_spawn_pose.position[1] = (uint32)(y);
    player_spawn_pose.position[2] = (uint32)(z);
    value = pose_owner_object->pose.position[0];
    x += value;
    player_spawn_pose.position[0] = (uint32)(x);
    value = pose_owner_object->pose.position[1];
    y += value;
    player_spawn_pose.position[1] = (uint32)(y);
    value = pose_owner_object->pose.position[2];
    z += value;
    player_spawn_pose.position[2] = (uint32)(z);
    value = pose_owner_object->pose.angles[0];
    player_spawn_pose.angles[0] = (uint16)(value);
    value = pose_owner_object->pose.angles[1];
    player_spawn_pose.angles[1] = (uint16)(value);
    value = pose_owner_object->pose.angles[2];
    player_spawn_pose.angles[2] = (uint16)(value);
    kind = actor_traits_object->flags;
    return effect_spawn_enqueue(kind, actor_motion_object->velocity, mode, &player_spawn_pose);
}

uint32 effect_tick_frame(void)
{
    MISSION_ACTOR *pose_owner_player;
    MISSION_ACTOR_MOTION *actor_motion_player;

    MISSION_ACTOR_TRAITS *actor_traits_player;
    uint32 actor_index_table;

    uint32 value, mission, variant;
    FUNCTION_MARKER(0x800850D8, "ACE3.BIN");
    if (game_runtime.mission_control.flags & 2)
    {
        actor_index_table = 0;
        pose_owner_player = mission_actor(actor_index_table);
        actor_motion_player = mission_actor_motion(actor_index_table);
        actor_traits_player = mission_actor_traits(actor_index_table);
        value = game_runtime.world_settings.ground_offset;
        effect_emit_height_scaled(0 - value, actor_motion_player->velocity, pose_owner_player->pose.position);
        actor_index_table = 0;
        actor_motion_player = mission_actor_motion(actor_index_table);
        actor_traits_player = mission_actor_traits(actor_index_table);
        if ((sint16)actor_traits_player->health > 0)
        {
            mission = (sint16)game_runtime.selection.mission;
            variant = (sint16)game_runtime.selection.aircraft;
            effect_emit_player_exhaust(mission, variant);
        }
    }
    if ((sint16)game_runtime.selection.mission == 42)
    {
        actor_index_table = 0;
        pose_owner_player = mission_actor(actor_index_table);
        actor_motion_player = mission_actor_motion(actor_index_table);
        actor_traits_player = mission_actor_traits(actor_index_table);
        if ((sint16)actor_traits_player->health > 0)
            effect_emit_ground_trail(pose_owner_player->pose.position, actor_motion_player->velocity);
    }
    effect_timed_tick();
    effect_drift_update(&effect_drift_primary_pool);
    effect_drift_update(&effect_drift_secondary_pool);
    effect_render_update();
    effect_aircraft_update();
    effect_segment_update();
    return effect_burst_tick();
}

void effect_schedule_timed_positions(uint32 kind)
{
    EFFECT_TIMED_EVENT *event;
    const uint32(*positions)[3];
    uint32 count, i, value;
    FUNCTION_MARKER(0x8009423C, "ACE3.BIN");
    switch (kind)
    {
        case 0:
            positions = timed_effect_positions_a;
            count = 7;
            break;
        case 1:
            positions = timed_effect_positions_a;
            count = 5;
            break;
        case 2:
            positions = timed_effect_positions_b;
            count = 5;
            break;
        case 3:
            positions = timed_effect_positions_c;
            count = 12;
            break;
        case 4:
            positions = timed_effect_positions_d;
            count = 10;
            break;
        default:
            return;
    }
    for (i = 0; i < count; ++i)
    {
        event = effect_timed_allocate();
        if (event == 0)
            return;
        value = game_random();
        event->ticks = (uint16)(i * 2 + (value & 1));
        event->duration = (uint16)(0x7FFF);
        event->position[0] = positions[i][0];
        event->position[1] = positions[i][1];
        event->callback = effect_timed_emit_6;
        event->position[2] = positions[i][2];
    }
}
