#include "game_functions.h"

enum { PARTICLE_PROJECTED_LISTS = 6, PARTICLE_PROJECTED_RECORDS = PARTICLE_CAPACITY + 1 };

typedef struct
{
    uint32 xy[2];
    uint16 depth[2];
    uint32 lifetime;
} PARTICLE_PROJECTED_SEGMENT;

PARTICLE_POOL particles;

void particle_tick_simple(void)
{
    PARTICLE *record;
    uint32 *masks;
    uint32 group, bits, index, bit, value, vx, vy, vz, a, b, flags;
    FUNCTION_MARKER(0x8006C9B8, "ACE3.BIN");
    masks = particles.live;
    xport_gte_write_data(8, 4000);
    for (group = 0; group < 128; group += 32)
    {
        bits = *masks;
        if (bits != 0)
        {
            for (index = 0; index < 32; ++index)
            {
                bit = 1u << index;
                if ((bits & bit) == 0)
                    continue;
                record = particles.slot + group + index;
                value = record->lifetime - 1;
                record->lifetime = (uint32)(value);
                if ((sint32)value < 0)
                {
                    bits &= ~bit;
                    continue;
                }
                vx = record->velocity[0];
                vy = record->velocity[1];
                a = record->position[0];
                vz = record->velocity[2];
                b = record->position[2];
                record->position[0] = (uint32)(a + vx);
                a = record->position[1];
                record->position[2] = (uint32)(b + vz);
                b = record->endpoint[1];
                record->position[1] = (uint32)(a + vy);
                a = record->endpoint[0];
                record->endpoint[1] = (uint32)(b + vy);
                flags = record->flags;
                record->endpoint[0] = (uint32)(a + vx);
                a = record->endpoint[2];
                record->endpoint[2] = (uint32)(a + vz);
                if ((flags & 2) == 0)
                    vy += 0x444;
                vx = (sint32)vx >> 6;
                vy = (sint32)vy >> 6;
                vz = (sint32)vz >> 6;
                if ((flags & 1) == 0)
                {
                    xport_gte_write_data(9, vx);
                    xport_gte_write_data(10, vy);
                    xport_gte_write_data(11, vz);
                    xport_gte_execute(0x198003D);
                    vx = xport_gte_read_data(25);
                    vy = xport_gte_read_data(26);
                    vz = xport_gte_read_data(27);
                }
                record->velocity[0] = (uint32)(vx << 6);
                record->velocity[1] = (uint32)(vy << 6);
                record->velocity[2] = (uint32)(vz << 6);
            }
            *masks = bits;
        }
        ++masks;
    }
    return;
}

void particle_expire(uint32 index)
{
    FUNCTION_MARKER(0x8006C99C, "ACE3.BIN");
    particles.slot[index].lifetime = 0;

}

static void particle_project_segments(PARTICLE_PROJECTED_SEGMENT outputs[PARTICLE_PROJECTED_LISTS][PARTICLE_PROJECTED_RECORDS])
{
    PARTICLE *record;
    PARTICLE_PROJECTED_SEGMENT *target;
    uint32 *bitmap;
    uint32 counts[PARTICLE_PROJECTED_LISTS], camera_x, camera_y, camera_z, limit, group, bit, mask, x, y, z, a, b, c, magnitude, sum, shift, flags, list, value, depth, tag, i;
    FUNCTION_MARKER(0x8006C20C, "ACE3.BIN");
    for (i = 0; i < 6; ++i)
        counts[i] = 0;
    bitmap = particles.live;
    camera_x = game_runtime.camera_view.pose.position[0];
    camera_y = game_runtime.camera_view.pose.position[1];
    value = ((sint16)game_runtime.camera_proj.distance);
    camera_z = game_runtime.camera_view.pose.position[2];
    limit = (value << 2) + (value << 3);
    for (group = 0; group < 128; group += 32, ++bitmap)
    {
        mask = *bitmap;
        if (mask == 0)
            continue;
        for (bit = 0; bit < 32; ++bit)
        {
            if ((mask & (1u << bit)) == 0)
                continue;
            record = particles.slot + group + bit;
            a = record->position[0];
            b = record->position[1];
            x = (sint32)(a - camera_x) >> 12;
            value = b - camera_y;
            c = record->position[2];
            z = (sint32)(c - camera_z) >> 12;
            a = (sint32)x < 0 ? 0 - x : x;
            y = (sint32)value >> 12;
            if ((sint32)a > 32767)
                continue;
            b = (sint32)y < 0 ? 0 - y : y;
            if ((sint32)b > 32767)
                continue;
            c = (sint32)z < 0 ? 0 - z : z;
            if ((sint32)c > 32767)
                continue;
            sum = a + b + c;
            shift = 0;
            if ((sint32)limit >= (sint32)sum)
                shift = ((sint32)limit >> 1) < (sint32)sum ? 1 : 2;
            x <<= shift;
            y <<= shift;
            z <<= shift;
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            a = record->endpoint[0];
            b = record->endpoint[1];
            x = (sint32)(a - camera_x) >> 12;
            value = b - camera_y;
            c = record->endpoint[2];
            z = (sint32)(c - camera_z) >> 12;
            magnitude = (sint32)x < 0 ? 0 - x : x;
            y = (sint32)value >> 12;
            if ((sint32)magnitude > 32767)
                continue;
            magnitude = (sint32)y < 0 ? 0 - y : y;
            if ((sint32)magnitude > 32767)
                continue;
            magnitude = (sint32)z < 0 ? 0 - z : z;
            if ((sint32)magnitude > 32767)
                continue;
            x <<= shift;
            y <<= shift;
            z <<= shift;
            xport_gte_write_data(2, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(3, z);
            xport_gte_execute(0x280030);
            flags = record->flags;
            if ((flags & 0x10) != 0)
                list = 0;
            else if ((flags & 0x20) != 0)
                list = 1;
            else if ((flags & 0x40) != 0)
                list = 2;
            else if ((flags & 0x80) != 0)
                list = 3;
            else if ((flags & 0x100) != 0)
                list = 4;
            else
                list = 5;
            target = &outputs[list][counts[list]];
            ++counts[list];
            tag = record->lifetime;
            depth = xport_gte_read_data(17);
            if (depth - 1 >= 0x77F7)
                tag = 0;
            depth = (sint32)depth >> shift;
            x = xport_gte_read_data(12);
            target->xy[0] = x;
            target->depth[0] = (uint16)depth;
            xport_gte_read_data(12);
            depth = xport_gte_read_data(18);
            if (depth - 1 >= 0x77F7)
                tag = 0;
            depth = (sint32)depth >> shift;
            x = xport_gte_read_data(13);
            target->xy[1] = x;
            target->depth[1] = (uint16)depth;
            target->lifetime = tag;
        }
    }
    for (i = 0; i < 6; ++i)
    {
        outputs[i][counts[i]].lifetime = 0xFFFFFFFF;
    }
    return;
}

void particle_update(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *target;
    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_target;
    uint32 actor_index_objects;

    ACTOR_COLLISION_CANDIDATE candidates[MISSION_ACTOR_COUNT];
    uint32 contact[3], hit[3], cells[4];
    uint32 velocity_radius, velocity_radius_squared, actor_filter;
    uint32 *bitmap;
    PARTICLE *queued_particles[9];
    MISSION_ACTOR *queued_targets[9];
    uint8 expired_indices[PARTICLE_CAPACITY];
    PARTICLE *state;
    const uint16 *map;
    uint32 count, queued, index, flags, x, y, z, size, base, bit, mask, dx, dy, dz, ax, ay, az, range, filter, j, sx, sy, sz, sum, radius, value, sound, mode, effect, a, b, c, result, material, old_x, old_y, old_z, cell, height, k, expired;
    FUNCTION_MARKER(0x8006B504, "ACE3.BIN");
    count = 0;
    queued = 0;
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
    bitmap = particles.live;
    for (base = 0; base < 128; base += 32)
    {
        mask = *bitmap;
        if (mask != 0)
            for (bit = 0; bit < 32; ++bit)
            {
                if ((mask & ((uint32)1 << bit)) == 0)
                    continue;
                state = particles.slot + base + bit;
                dx = state->velocity[0];
                dy = state->velocity[1];
                dz = state->velocity[2];
                ax = (sint32)dx < 0 ? 0 - dx : dx;
                ay = (sint32)dy < 0 ? 0 - dy : dy;
                az = (sint32)dz < 0 ? 0 - dz : dz;
                range = (sint32)(ax + ay + az) >> 13;
                velocity_radius = range;
                value = range * range;
                a = state->position[1];
                y = (sint32)(a + dy) >> 12;
                filter = state->actor_flags & 0x1C0;
                a = state->position[0];
                x = (sint32)(a + dx) >> 12;
                a = state->position[2];
                z = (sint32)(a + dz) >> 12;
                velocity_radius_squared = (sint32)value / 9;
                if ((sint32)count <= 0)
                    continue;
                actor_filter = ~filter & 0x1C0;
                for (j = 0; j < count; ++j)
                {
                    flags = snapshot[j].flags;
                    if ((flags & actor_filter) == 0)
                        continue;
                    sx = snapshot[j].x;
                    sz = snapshot[j].z;
                    range = velocity_radius;
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
                    radius = snapshot[j].radius_squared + velocity_radius_squared;
                    if ((sint32)radius < (sint32)sum)
                        continue;
                    actor_index_objects = 0;
                    target = mission_actor((actor_index_objects + ((uint32)((sint32)((flags & 63) * 4) >> 2))));
                    actor_traits_target = &target->traits;
                    if ((flags & 0x1800) != 0)
                    {
                        queued_particles[queued] = state;
                        queued_targets[queued] = target;
                        if (queued < 8)
                            ++queued;
                        continue;
                    }
                    flags = state->flags;
                    for (k = 0; k < 6; ++k)
                        if ((flags & (0x400 << k)) != 0)
                        {
                            value = actor_traits_target->events;
                            actor_traits_target->events = (uint32)(value | ((uint32)1 << k));
                            break;
                        }
                    value = state->actor_flags;
                    actor_traits_target->damage_source = (uint32)(value);
                    state->position[0] = (uint32)(x << 12);
                    state->position[1] = (uint32)(y << 12);
                    state->position[2] = (uint32)(z << 12);
                    state->lifetime = (uint32)(0);
                    sound = 30;
                    mode = 2;
                    flags = state->flags;
                    if (j == 0)
                    {
                        sound = 31;
                        if ((flags & 0xC0000) != 0)
                            sound = 42;
                    }
                    else if ((flags & 0x40000) != 0)
                        sound = 40;
                    else if ((flags & 0x80000) != 0)
                    {
                        sound = 41;
                        mode = 1;
                    }
                    sound_play_spatial_reuse(sound, mode, state->position);
                    flags = state->flags;
                    effect = 0;
                    for (k = 0; k < 5; ++k)
                        if ((flags & (32 << k)) != 0)
                        {
                            effect = k + 1;
                            break;
                        }
                    effect_emit_position_type0(effect, state->position);
                }
            }
        ++bitmap;
    }
    for (index = 0; index < queued; ++index)
    {
        target = queued_targets[index];
        actor_traits_target = &target->traits;
        flags = actor_traits_target->flags;
        state = queued_particles[index];
        if ((flags & 0x1020) == 0x1020 && (actor_traits_target->render_flags & 2) == 0)
            result = collision_trace_actor(target, contact, state->position, state->velocity);
        else
        {
            contact[0] = state->position[0];
            contact[1] = state->position[1];
            contact[2] = state->position[2];
            result = 0;
        }
        if (result == 0x7FFFFFFF)
            continue;
        flags = state->flags;
        for (k = 0; k < 3; ++k)
            if ((flags & (0x400 << k)) != 0)
            {
                value = actor_traits_target->events;
                actor_traits_target->events = (uint32)(value | ((uint32)1 << k));
                break;
            }
        sound = 30;
        value = state->actor_flags;
        mode = 2;
        actor_traits_target->damage_source = (uint32)(value);
        flags = state->flags;
        state->lifetime = (uint32)(0);
        if ((flags & 0x20000) != 0)
        {
            value = game_runtime.collision_result.flags;
            if ((value & 15) == 1)
                sound = 35;
        }
        else if ((flags & 0x40000) != 0)
        {
            sound = 40;
            value = game_runtime.collision_result.flags;
            if ((value & 15) == 1)
                sound = 35;
        }
        else if ((flags & 0x80000) != 0)
        {
            sound = 41;
            mode = 1;
        }
        sound_play_spatial_reuse(sound, mode, contact);
        flags = state->flags;
        effect = 0;
        for (k = 0; k < 5; ++k)
            if ((flags & (32 << k)) != 0)
            {
                effect = k + 1;
                break;
            }
        material = game_runtime.collision_result.flags;
        effect_emit_material_hit(material, effect, contact);
    }
    bitmap = particles.live;
    map = game_runtime.scene_assets.tile_map;
    for (base = 0; base < 128; base += 32)
    {
        mask = *bitmap;
        if (mask != 0)
            for (bit = 0; bit < 32; ++bit)
            {
                if ((mask & ((uint32)1 << bit)) == 0)
                    continue;
                state = particles.slot + base + bit;
                old_x = state->position[0];
                old_y = state->position[1];
                dx = state->velocity[0];
                dy = state->velocity[1];
                old_z = state->position[2];
                x = dx + old_x;
                y = dy + old_y;
                dz = state->velocity[2];
                value = game_runtime.mission_bounds.min[0];
                z = dz + old_z;
                material = 0;
                if ((sint32)x < (sint32)value)
                {
                    material = 4;
                    x = value;
                }
                value = game_runtime.mission_bounds.max[0];
                if ((sint32)value < (sint32)x)
                {
                    material = 4;
                    x = value;
                }
                value = game_runtime.mission_bounds.min[1];
                if ((sint32)y < (sint32)value)
                {
                    material = 3;
                    y = value;
                }
                value = game_runtime.mission_bounds.max[1];
                if ((sint32)value < (sint32)y)
                {
                    material = 1;
                    y = value;
                }
                value = game_runtime.mission_bounds.min[2];
                if ((sint32)z < (sint32)value)
                {
                    material = 4;
                    z = value;
                }
                value = game_runtime.mission_bounds.max[2];
                if ((sint32)value < (sint32)z)
                {
                    material = 4;
                    z = value;
                }
                if (material != 0)
                {
                    sound = 30;
                    state->lifetime = (uint32)(0);
                    hit[0] = x;
                    hit[1] = y;
                    hit[2] = z;
                    flags = state->flags;
                    game_runtime.collision_result.flags = (uint32)(material);
                    mode = 2;
                    if ((flags & 0x20000) != 0)
                    {
                        if ((material & 15) == 1)
                            sound = 35;
                    }
                    else if ((flags & 0x40000) != 0)
                        sound = 40;
                    else if ((flags & 0x80000) != 0)
                    {
                        sound = 41;
                        mode = 1;
                    }
                    sound_play_spatial_reuse(sound, mode, hit);
                }
                else
                {
                    cells[0] = (uint32)((sint32)old_x >> 23) + 128;
                    cells[1] = (uint32)((sint32)old_z >> 23) + 128;
                    cells[2] = (uint32)((sint32)x >> 23) + 128;
                    cells[3] = (uint32)((sint32)z >> 23) + 128;
                    for (k = 0; k < 4; ++k)
                        if ((sint32)cells[k] < 0)
                            cells[k] &= 7;
                    for (k = 0; k < 4; ++k)
                        if ((sint32)cells[k] >= 256)
                            cells[k] = (cells[k] & 7) + 248;
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
                        result = collision_query_tile(state->position, contact, hit);
                    }
                    else
                        result = collision_query_segment(state->position, contact, hit);
                    if (result == 0x7FFFFFFF)
                        continue;
                    sound = 30;
                    flags = state->flags;
                    mode = 2;
                    state->lifetime = (uint32)(0);
                    if ((flags & 0x20000) != 0)
                    {
                        value = game_runtime.collision_result.flags;
                        if ((value & 15) == 1)
                            sound = 35;
                    }
                    else if ((flags & 0x40000) != 0)
                    {
                        sound = 40;
                        value = game_runtime.collision_result.flags;
                        if ((value & 15) == 1)
                            sound = 35;
                    }
                    else if ((flags & 0x80000) != 0)
                    {
                        sound = 41;
                        mode = 1;
                    }
                    sound_play_spatial_reuse(sound, mode, hit);
                }
                flags = state->flags;
                effect = 0;
                for (k = 0; k < 5; ++k)
                    if ((flags & (32 << k)) != 0)
                    {
                        effect = k + 1;
                        break;
                    }
                material = game_runtime.collision_result.flags;
                effect_emit_material_hit(material, effect, hit);
            }
        ++bitmap;
    }
    bitmap = particles.live;
    xport_gte_write_data(8, 4000);
    expired = 0;
    for (base = 0; base < 128; base += 32)
    {
        mask = *bitmap;
        if (mask != 0)
        {
            for (bit = 0; bit < 32; ++bit)
            {
                if ((mask & ((uint32)1 << bit)) == 0)
                    continue;
                index = base + bit;
                state = particles.slot + index;
                value = state->lifetime - 1;
                state->lifetime = (uint32)(value);
                if ((sint32)value < 0)
                {
                    mask &= ~((uint32)1 << bit);
                    expired_indices[expired] = index;
                    ++expired;
                    continue;
                }
                dx = state->velocity[0];
                dy = state->velocity[1];
                a = state->position[0];
                dz = state->velocity[2];
                c = state->position[2];
                state->position[0] = (uint32)(a + dx);
                b = state->position[1];
                state->position[2] = (uint32)(c + dz);
                c = state->endpoint[1];
                state->position[1] = (uint32)(b + dy);
                a = state->endpoint[0];
                state->endpoint[1] = (uint32)(c + dy);
                flags = ~state->flags;
                state->endpoint[0] = (uint32)(a + dx);
                c = state->endpoint[2];
                state->endpoint[2] = (uint32)(c + dz);
                if ((flags & 2) != 0)
                    dy += 1092;
                dx = (sint32)dx >> 6;
                dy = (sint32)dy >> 6;
                dz = (sint32)dz >> 6;
                if ((flags & 1) != 0)
                {
                    xport_gte_write_data(9, dx);
                    xport_gte_write_data(10, dy);
                    xport_gte_write_data(11, dz);
                    xport_gte_execute(0x198003D);
                    dx = xport_gte_read_data(25);
                    dy = xport_gte_read_data(26);
                    dz = xport_gte_read_data(27);
                }
                state->velocity[0] = (uint32)(dx << 6);
                state->velocity[1] = (uint32)(dy << 6);
                state->velocity[2] = (uint32)(dz << 6);
            }
            *bitmap = mask;
        }
        ++bitmap;
    }
    for (index = 0; index < expired; ++index)
    {
        value = expired_indices[index];
        flags = particles.slot[value].actor_flags;
        if ((flags & 0x2000) != 0)
            mission_replay_record_particle_expiration(value);
    }
}

void particle_initialize(uint32 index, uint32 mode, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    MATRIX matrix;
    uint16 direction[3];
    PARTICLE *slot;
    uint32 *address;
    uint32 flags, a, b, c, transformed[3], value;
    FUNCTION_MARKER(0x8006C760, "ACE3.BIN");
    address = particles.live + (uint32)((sint32)index >> 5);
    flags = *address;
    *address = flags | ((uint32)1 << (index & 31));
    rotation_build(&matrix, native_position->angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    a = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    b = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    c = (uint16)matrix.m[2][2];
    SetRotMatrix(&matrix);
    slot = particles.slot + index;
    switch (mode)
    {
        case 0:
            slot->flags = (uint32)(0x20410);
            slot->lifetime = (uint32)(37);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 2048;
            break;
        case 1:
            slot->flags = (uint32)(0x20820);
            slot->lifetime = (uint32)(24);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 2448;
            break;
        case 2:
            slot->flags = (uint32)(0x41040);
            slot->lifetime = (uint32)(62);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 1216;
            break;
        case 3:
            slot->flags = (uint32)(0x82080);
            slot->lifetime = (uint32)(35);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 0;
            break;
        case 4:
            slot->flags = (uint32)(0x94103);
            slot->lifetime = (uint32)(16);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 0;
            break;
        case 5:
            slot->flags = (uint32)(0x98202);
            slot->lifetime = (uint32)(16);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 2048;
            break;
        default:
            break;
    }
    xport_gte_write_data(0, direction[0] | ((uint32)direction[1] << 16));
    xport_gte_write_data(1, direction[2]);
    xport_gte_execute(0x486012);
    transformed[0] = xport_gte_read_data(25);
    transformed[1] = xport_gte_read_data(26);
    transformed[2] = xport_gte_read_data(27);
    value = (native_position->position[0]);
    slot->position[0] = (uint32)(value);
    value = (native_position->position[1]);
    slot->position[1] = (uint32)(value);
    value = (native_position->position[2]);
    slot->position[2] = (uint32)(value);
    value = slot->position[0];
    slot->endpoint[0] = (uint32)(value + (transformed[0] << 8));
    value = slot->position[1];
    slot->endpoint[1] = (uint32)(value + (transformed[1] << 8));
    value = slot->position[2];
    slot->endpoint[2] = (uint32)(value + (transformed[2] << 8));
    value = (native_vector[0]);
    slot->velocity[0] = (uint32)(value);
    value = (native_vector[1]);
    slot->velocity[1] = (uint32)(value);
    value = (native_vector[2]);
    slot->velocity[2] = (uint32)(value);
    { (void)(uint32)(value); return; }
}


typedef char particle_packet_layout_check[
    sizeof(LINE_G2) == 20 && offsetof(LINE_G2, p) == 4 && sizeof(POINT_C) == 8 &&
    offsetof(POINT_C, xy) == 4 && sizeof(RGBC) == 4 &&
    sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 &&
    sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

static uint32 particle_draw_lines(uint32 ordering, uint32 packet, const PARTICLE_PROJECTED_SEGMENT *segments)
{
    uint32 *gpu_word;
    uint32 index, count, angle, depth, first, second, target, value, color;
    sint32 z0, z1, x0, x1, y0, y1;
    FUNCTION_MARKER(0x80090A74, "ACE3.BIN");
    index = 0;
    count = segments[0].lifetime;
    if ((sint32)count < 0)
        return packet;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x52000000);
    angle = game_runtime.camera_proj.distance;
    angle = ((sint32)(angle << 16) >> 18);
    do
    {
        if (count)
        {
            z0 = (sint16)segments[index].depth[0];
            if ((sint32)angle < z0)
            {
                depth = (sint32)count < 28 ? (28 - count) << 7 : 0;
                xport_gte_write_data(8, depth);
                xport_gte_write_data(20, 0x6433D);
                xport_gte_write_data(21, 0x14CFBF);
                first = segments[index].xy[0];
                second = segments[index].xy[1];
                xport_gte_execute(0xF8002A);
                y0 = (sint32)first >> 16;
                y1 = (sint32)second >> 16;
                x0 = (sint16)first;
                x1 = (sint16)second;
                if ((y0 >= 121 && y1 >= 121) || (y0 < -120 && y1 < -120) || (x0 >= 161 && x1 >= 161) || (x0 < -160 && x1 < -160))
                {
                    xport_gte_read_data(25);
                }
                else
                {
                    LINE_G2 *line = mission_packet_span(packet, sizeof(*line));
                    line->p[0].xy.xy = (uint32)(first);
                    line->p[1].xy.xy = (uint32)(second);
                    z1 = (sint16)segments[index].depth[1];
                    target = ordering + ((uint32)((z0 + z1) >> 4) << 2);
                    gpu_word = mission_ot_span(target, sizeof(*gpu_word));
                    value = *gpu_word;
                    line->tag = (uint32)(value | 0x04000000);
                    *gpu_word = packet & 0xFFFFFF;
                    color = xport_gte_read_data(20);
                    line->p[0].rgbc.rgbc = (uint32)(color);
                    color = xport_gte_read_data(21);
                    line->p[1].rgbc.rgbc = (uint32)(color);
                    packet += 20;
                }
            }
        }
        ++index;
        count = segments[index].lifetime;
    } while ((sint32)count >= 0);
    return packet;
}


static uint32 particle_draw_ribbons_variant_1(uint32 ordering, uint32 packet, const PARTICLE_PROJECTED_SEGMENT *segments)
{
    uint32 *gpu_word;
    EFFECT_QUAD quad;
    uint32 index, tag, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    POLY_FT4 *output;
    FUNCTION_MARKER(0x80090BF8, "ACE3.BIN");
    index = 0;
    tag = segments[0].lifetime;
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    value = game_runtime.camera_proj.distance;
    value = (sint16)value;
    scale = value << 1;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_90bf8;
        first_depth = (sint16)segments[index].depth[0];
        second_depth = (sint16)segments[index].depth[1];
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_90bf8;
        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
        xport_gte_write_data(8, value);
        first_xy = segments[index].xy[0];
        second_xy = segments[index].xy[1];
        xport_gte_execute(0x780010);
        quad.endpoint[1].xy = first_xy;
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        quad.endpoint[0].xy = second_xy;
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        quad.scale[0] = second_width;
        quad.scale[1] = first_width;
        value = xport_gte_read_data(22);
        RGBC *early_color = mission_packet_span(packet + 4, sizeof(*early_color));
        early_color->rgbc = (uint32)(value);
        x = (uint16)quad.endpoint[0].x;
        x = (sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_90bf8;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_90bf8;
        }
        y = (uint16)quad.endpoint[0].y;
        y = (sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_90bf8;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_90bf8;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        output = mission_packet_span(packet, sizeof(*output));
        gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
        value = *gpu_word;
        output->tag = (uint32)(value | 0x09000000);
        *gpu_word = packet & 0xFFFFFF;
        output->p[2].uv.uv = (uint16)(0x9FB0);
        output->p[1].uv_attr = (uint32)(0x003860BF);
        output->p[0].uv_attr = (uint32)(0x449660B0);
        output->p[3].uv.uv = (uint16)(0x9FBF);
        effect_quad_build(output->p, &quad);
        packet += 40;
    next_record_90bf8:
        ++index;
        tag = segments[index].lifetime;
    }
    return packet;
}


static uint32 particle_draw_ribbons_variant_2(uint32 ordering, uint32 packet, const PARTICLE_PROJECTED_SEGMENT *segments)
{
    uint32 *gpu_word;
    EFFECT_QUAD quad;
    uint32 index, tag, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    POLY_FT4 *output;
    FUNCTION_MARKER(0x80090ED0, "ACE3.BIN");
    index = 0;
    tag = segments[0].lifetime;
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    value = game_runtime.camera_proj.distance;
    value = (sint16)value;
    scale = (value << 1) + value;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_90ed0;
        first_depth = (sint16)segments[index].depth[0];
        second_depth = (sint16)segments[index].depth[1];
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_90ed0;
        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
        xport_gte_write_data(8, value);
        first_xy = segments[index].xy[0];
        second_xy = segments[index].xy[1];
        xport_gte_execute(0x780010);
        quad.endpoint[1].xy = first_xy;
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        quad.endpoint[0].xy = second_xy;
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        quad.scale[0] = second_width;
        quad.scale[1] = first_width;
        value = xport_gte_read_data(22);
        RGBC *early_color = mission_packet_span(packet + 4, sizeof(*early_color));
        early_color->rgbc = (uint32)(value);
        x = (uint16)quad.endpoint[0].x;
        x = (sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_90ed0;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_90ed0;
        }
        y = (uint16)quad.endpoint[0].y;
        y = (sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_90ed0;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_90ed0;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        output = mission_packet_span(packet, sizeof(*output));
        gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
        value = *gpu_word;
        output->tag = (uint32)(value | 0x09000000);
        *gpu_word = packet & 0xFFFFFF;
        output->p[2].uv.uv = (uint16)(0x9FC0);
        output->p[1].uv_attr = (uint32)(0x003860CF);
        output->p[0].uv_attr = (uint32)(0x44D660C0);
        output->p[3].uv.uv = (uint16)(0x9FCF);
        effect_quad_build(output->p, &quad);
        packet += 40;
    next_record_90ed0:
        ++index;
        tag = segments[index].lifetime;
    }
    return packet;
}


static uint32 particle_draw_endpoint_sprites(uint32 ordering, uint32 packet, const PARTICLE_PROJECTED_SEGMENT *segments)
{
    uint32 *gpu_word;
    uint32 index, tag, value, near_limit, scale, depth, packed, x, y, magnitude, bucket, size, half;
    FUNCTION_MARKER(0x800911B0, "ACE3.BIN");
    index = 0;
    tag = segments[index].lifetime;
    if ((sint32)tag < 0)
        return packet;
    value = game_runtime.camera_proj.distance;
    value = (sint16)value;
    scale = ((value << 2) + value) << 2;
    near_limit = (sint32)value >> 1;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    do
    {
        if (tag != 0)
        {
            depth = (sint16)segments[index].depth[1];
            packed = segments[index].xy[1];
            y = (sint32)packed >> 16;
            x = (sint16)packed;
            if ((sint32)near_limit < (sint32)depth)
            {
                magnitude = (sint32)x < 0 ? 0 - x : x;
                if ((sint32)magnitude < 171)
                {
                    magnitude = (sint32)y < 0 ? 0 - y : y;
                    if ((sint32)magnitude < 131)
                    {
                        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
                        xport_gte_write_data(8, value);
                        bucket = ordering + ((uint32)((sint32)depth >> 3) << 2);
                        gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
                        value = *gpu_word;
                        POLY_FT4 *output = mission_packet_span(packet, sizeof(*output));
                        output->tag = (uint32)(value | 0x09000000);
                        *gpu_word = packet & 0xFFFFFF;
                        xport_gte_execute(0x780010);
                        if (depth == 0)
                            xport_mips_break(7);
                        if (depth == 0xFFFFFFFF && scale == 0x80000000)
                            xport_mips_break(6);
                        size = (sint32)scale / (sint32)depth;
                        if ((sint32)size < 2)
                            size = 2;
                        half = (sint32)size >> 1;
                        x -= half;
                        y -= half;
                        output->p[3].xy.x = (sint16)(uint16)(x + size);
                        output->p[1].xy.x = (sint16)(uint16)(x + size);
                        output->p[3].xy.y = (sint16)(uint16)(y + size);
                        output->p[2].xy.y = (sint16)(uint16)(y + size);
                        output->p[2].uv.uv = (uint16)(0x7FE0);
                        output->p[2].xy.x = (sint16)(uint16)(x);
                        output->p[0].xy.x = (sint16)(uint16)(x);
                        output->p[1].xy.y = (sint16)(uint16)(y);
                        output->p[0].xy.y = (sint16)(uint16)(y);
                        output->p[0].uv_attr = (uint32)(0x451660E0);
                        output->p[1].uv_attr = (uint32)(0x003860FF);
                        output->p[3].uv.uv = (uint16)(0x7FFF);
                        value = xport_gte_read_data(22);
                        output->rgbc.rgbc = (uint32)(value);
                        packet += 40;
                    }
                }
            }
        }
        ++index;
        tag = segments[index].lifetime;
    } while ((sint32)tag >= 0);
    return packet;
}


static uint32 particle_draw_ribbons_variant_3(uint32 ordering, uint32 packet, const PARTICLE_PROJECTED_SEGMENT *segments)
{
    uint32 *gpu_word;
    EFFECT_QUAD quad;
    uint32 index, tag, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    POLY_FT4 *output;
    FUNCTION_MARKER(0x80091370, "ACE3.BIN");
    index = 0;
    tag = segments[0].lifetime;
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    value = game_runtime.camera_proj.distance;
    value = (sint16)value;
    scale = ((value << 1) + value) << 1;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_91370;
        first_depth = (sint16)segments[index].depth[0];
        second_depth = (sint16)segments[index].depth[1];
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_91370;
        value = (sint32)tag < 14 ? (14 - tag) << 8 : 0;
        xport_gte_write_data(8, value);
        first_xy = segments[index].xy[0];
        second_xy = segments[index].xy[1];
        xport_gte_execute(0x780010);
        quad.endpoint[1].xy = first_xy;
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        quad.endpoint[0].xy = second_xy;
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        quad.scale[0] = second_width;
        quad.scale[1] = first_width;
        value = xport_gte_read_data(22);
        RGBC *early_color = mission_packet_span(packet + 4, sizeof(*early_color));
        early_color->rgbc = (uint32)(value);
        x = (uint16)quad.endpoint[0].x;
        x = (sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_91370;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = (uint16)quad.endpoint[1].x;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_91370;
        }
        y = (uint16)quad.endpoint[0].y;
        y = (sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_91370;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = (uint16)quad.endpoint[1].y;
            value = (sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_91370;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        output = mission_packet_span(packet, sizeof(*output));
        gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
        value = *gpu_word;
        output->tag = (uint32)(value | 0x09000000);
        *gpu_word = packet & 0xFFFFFF;
        output->p[2].uv.uv = (uint16)(0x9FD0);
        output->p[1].uv_attr = (uint32)(0x003860DF);
        output->p[0].uv_attr = (uint32)(0x455660D0);
        output->p[3].uv.uv = (uint16)(0x9FDF);
        effect_quad_build(output->p, &quad);
        packet += 40;
    next_record_91370:
        ++index;
        tag = segments[index].lifetime;
    }
    return packet;
}


uint32 particle_draw_segments(uint32 ordering, uint32 packet)
{
    PARTICLE_PROJECTED_SEGMENT records[PARTICLE_PROJECTED_LISTS][PARTICLE_PROJECTED_RECORDS];
    FUNCTION_MARKER(0x8006C5EC, "ACE3.BIN");
    particle_project_segments(records);
    packet = particle_draw_lines(ordering, packet, records[0]);
    packet = particle_draw_ribbons_variant_1(ordering, packet, records[1]);
    packet = particle_draw_ribbons_variant_2(ordering, packet, records[2]);
    packet = particle_draw_endpoint_sprites(ordering, packet, records[3]);
    return particle_draw_ribbons_variant_3(ordering, packet, records[5]);
}


void particle_draw_frame(void)
{
    uint32 a, b, c, d, e, ot, packet;
    FUNCTION_MARKER(0x800333D8, "ACE3.BIN");
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    e = (uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = (uint32)psx_identity_matrix.t[0];
    b = (uint32)psx_identity_matrix.t[1];
    c = (uint32)psx_identity_matrix.t[2];
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    ot = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    packet = particle_draw_segments(ot + 4, packet);
    game_runtime.mission_control.packet_cur = (uint32)(packet);
    { (void)(uint32)(packet); return; }
}


void particle_update_simple(void)
{
    FUNCTION_MARKER(0x8006CB10, "ACE3.BIN");
    { (void)(particle_tick_simple()); return; }
}
