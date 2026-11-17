#include "game_functions.h"

typedef struct
{
    uint32 origin[3], origin_pad;
    uint32 velocity[3], velocity_pad;
} EFFECT_EMISSION_PUBLICATION;

typedef char effect_emission_publication_layout[sizeof(EFFECT_EMISSION_PUBLICATION) == 32 && offsetof(EFFECT_EMISSION_PUBLICATION, origin) == 0 && offsetof(EFFECT_EMISSION_PUBLICATION, origin_pad) == 12 && offsetof(EFFECT_EMISSION_PUBLICATION, velocity) == 16 && offsetof(EFFECT_EMISSION_PUBLICATION, velocity_pad) == 28 ? 1 : -1];

typedef struct
{
    uint32 tag, mode;
    RGBC color;
    POINT_T point;
    uint32 wh;
} FRAMEBUFFER_COPY_PACKET;

typedef char framebuffer_copy_packet_layout_check[sizeof(FRAMEBUFFER_COPY_PACKET) == 24 && offsetof(FRAMEBUFFER_COPY_PACKET, mode) == 4 && offsetof(FRAMEBUFFER_COPY_PACKET, color) == 8 && offsetof(FRAMEBUFFER_COPY_PACKET, point) == 12 && offsetof(FRAMEBUFFER_COPY_PACKET, wh) == 20 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(DR_OFFSET) == 12 && offsetof(DR_OFFSET, code) == 4 && offsetof(DR_STP, code) == 4 ? 1 : -1];

typedef char screen_transition_packet_layout_check[sizeof(DR_MODE) == 12 && offsetof(DR_MODE, code) == 4 && sizeof(TILE) == 16 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 ? 1 : -1];

typedef struct
{
    uint32 radius_duration;
    uint32 style_flags;
} EFFECT_DRIFT_STYLE;

void effect_emit_debris(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    FUNCTION_MARKER(0x8008074C, "ACE3.BIN");
    mission_replay_record_vector_effect(0, native_position, native_vector, kind);
    {
        (void)(effect_spawn_debris(native_position, native_vector));
        return;
    }
}

void effect_emit_surface_hit(uint32 kind, const uint32 *native_position, const uint32 *native_vector, const uint16 *native_normal)
{
    uint32 direction;
    FUNCTION_MARKER(0x800809F0, "ACE3.BIN");
    direction = game_normal_angles(native_normal);
    mission_replay_record_surface_hit(kind, direction, native_position, native_vector);
    {
        (void)(effect_spawn_surface_hit(direction, native_position, native_vector));
        return;
    }
}

void effect_emit_burst(uint32 kind, const uint32 *native_position)
{
    FUNCTION_MARKER(0x80080F10, "ACE3.BIN");
    mission_replay_record_position_effect(5, native_position, kind, 0);
    {
        (void)(effect_spawn_burst(kind, native_position));
        return;
    }
}

void effect_emit_trail(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 mission;
    FUNCTION_MARKER(0x800829BC, "ACE3.BIN");
    if (kind == 1)
    {
        mission = (sint16)game_runtime.selection.mission;
        if (mission != 42)
            kind = 10;
    }
    mission_replay_record_vector_effect(6, native_position, native_vector, kind);
    {
        (void)(effect_emit_secondary_drift(kind, native_position, native_vector));
        return;
    }
}

static void effect_vector_style_0(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *record;
    uint32 dx, dy, dz, x, y, z, seed, random, value, result;
    FUNCTION_MARKER(0x8008136C, "ACE3.BIN");
    dx = ((native_vector[0]));
    dy = ((native_vector[1]));
    dz = ((native_vector[2]));
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    x = ((native_position[0])) + ((random & 127) << 11);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    y = ((native_position[1])) + ((random & 127) << 11);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    z = ((native_position[2])) + ((random & 127) << 11);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        {
            uint32 packed_record_word = 0x280000;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->style = (uint16)(0x210);
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        record->radius = (uint16)((random & 63) + 128);
    }
    if ((random & 1) != 0)
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record != 0)
        {
            {
                uint32 packed_record_word = 0xCFFFF;
                record->ticks = (uint16)packed_record_word;
                record->duration = (uint16)(packed_record_word >> 16);
            }
            record->position[0] = (uint32)(x - dx);
            record->position[1] = (uint32)(y - dy);
            record->position[2] = (uint32)(z - dz);
            record->style = (uint16)(256);
            record->flags = (uint16)(0);
            record->radius = (uint16)((((uint32)((sint32)random >> 3)) & 63) + 88);
        }
    }
    result = random & 3;
    if (result == 0)
        return;
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    record->position[0] = (uint32)(x);
    record->position[1] = (uint32)(y);
    record->position[2] = (uint32)(z);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (uint32)((sint32)dx >> 8) - 128 + (random & 255);
    record->vector[0] = (uint16)(value);
    value = (uint32)((sint32)dy >> 8) - 128 + (((uint32)((sint32)random >> 3)) & 255);
    record->vector[1] = (uint16)(value);
    value = (uint32)((sint32)dz >> 8) - 128 + (((uint32)((sint32)random >> 6)) & 255);
    record->vector[2] = (uint16)(value);
    if ((random & 1) != 0)
    {
        {
            uint32 packed_record_word = 0x120000;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->flags = (uint16)(0x8B);
        record->style = (uint16)(0x1504);
        result = (random & 31) + 100;
    }
    else
    {
        {
            uint32 packed_record_word = 0x1E0000;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->flags = (uint16)(0x18C);
        result = (random & 7) + 20;
        record->style = (uint16)(0);
    }
    record->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

static void effect_vector_style_1(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *record;
    uint32 dx, dy, dz, x, y, z, a, b, c, seed, random, value, result;
    FUNCTION_MARKER(0x8008158C, "ACE3.BIN");
    dx = ((native_vector[0]));
    a = ((native_position[0]));
    dy = ((native_vector[1]));
    dz = ((native_vector[2]));
    c = ((native_position[2]));
    x = a + dx;
    b = ((native_position[1]));
    z = c + dz;
    y = b + dy;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        record->ticks = (uint16)(0xFFFF);
        record->duration = (uint16)((random & 7) + 9);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 9));
        value = ((uint32)((sint32)random >> 3) & 255) - 128;
        record->position[1] = (uint32)(y + (value << 9));
        value = ((uint32)((sint32)random >> 6) & 255) - 128;
        record->position[2] = (uint32)(z + (value << 9));
        record->style = (uint16)(256);
        record->flags = (uint16)(0);
        record->radius = (uint16)((random & 63) + 90);
    }
    result = random & 1;
    if (result == 0)
        return;
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    result = random & 7;
    if (record == 0)
    {
        (void)(result);
        return;
    }
    record->ticks = (uint16)(0);
    record->duration = (uint16)(result + 16);
    record->position[0] = (uint32)(x);
    record->position[1] = (uint32)(y);
    record->position[2] = (uint32)(z);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (uint32)((sint32)dx >> 8) - 64 + (random & 127);
    record->vector[0] = (uint16)(value);
    value = (uint32)((sint32)dy >> 8) - 64 + ((uint32)((sint32)random >> 3) & 127);
    record->vector[1] = (uint16)(value);
    value = (uint32)((sint32)dz >> 8) - 64 + ((uint32)((sint32)random >> 6) & 127);
    record->vector[2] = (uint16)(value);
    if ((random & 1) != 0)
    {
        record->flags = (uint16)(0x8D);
        record->style = (uint16)(0x1504);
        result = (random & 63) + 150;
    }
    else
    {
        record->flags = (uint16)(0x18C);
        result = (random & 7) + 20;
        record->style = (uint16)(0);
    }
    record->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

static void effect_vector_style_2(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *record;
    uint32 dx, dy, dz, x, y, z, value, seed, random, rotated, lifetime, count, vx, vy, vz, i, result;
    FUNCTION_MARKER(0x80081760, "ACE3.BIN");
    dx = ((native_vector[0]));
    dy = ((native_vector[1]));
    value = ((native_position[0]));
    dz = ((native_vector[2]));
    x = value + dx;
    vx = (sint32)(0 - dx) >> 11;
    value = ((native_position[1]));
    y = value + dy;
    vy = (sint32)(0 - dy) >> 11;
    value = ((native_position[2]));
    z = value + dz;
    vz = (sint32)(0 - dz) >> 11;
    result = 0;
    for (i = 0; i < 2; ++i)
    {
        record = effect_drift_allocate(i == 0 ? &effect_drift_primary_pool : &effect_drift_secondary_pool);
        result = 0;
        if (record == 0)
            continue;
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        count = (random & (i == 0 ? 31 : 15)) + (i == 0 ? 30 : 18);
        rotated = (uint32)((sint32)random >> 8) | (random << 7);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 9));
        value = vx - (i == 0 ? 128 : 64) + (rotated & (i == 0 ? 255 : 127));
        record->vector[0] = (uint16)(value);
        record->vector[1] = (uint16)(vy - 256);
        value = vz - (i == 0 ? 128 : 64) + (((uint32)((sint32)rotated >> 7)) & (i == 0 ? 255 : 127));
        record->flags = (uint16)(i == 0 ? 0x84 : 0x83);
        record->style = (uint16)(i == 0 ? 0x210 : 0x1604);
        lifetime = (rotated & (i == 0 ? 127 : 63)) + (i == 0 ? 120 : 100);
        record->ticks = (uint16)(0);
        record->duration = (uint16)(count);
        record->position[1] = (uint32)(y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 9));
        record->position[2] = (uint32)(z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 9));
        record->vector[2] = (uint16)(value);
        record->radius = (uint16)(lifetime);
        result = lifetime;
    }
    {
        (void)(result);
        return;
    }
}

static void effect_vector_style_3(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *record;
    uint32 dx, dy, dz, x, y, z, vx, vy, vz, value, seed, random, rotated, a, b, c, count, result;
    FUNCTION_MARKER(0x80081994, "ACE3.BIN");
    dx = ((native_vector[0]));
    dy = ((native_vector[1]));
    value = ((native_position[0]));
    dz = ((native_vector[2]));
    b = ((native_position[1]));
    x = value - dx;
    vx = (sint32)(0 - dx) >> 7;
    y = b - dy;
    vy = (sint32)(0 - dy) >> 7;
    value = ((native_position[2]));
    z = value - dz;
    vz = (sint32)(0 - dz) >> 7;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record != 0)
    {
        count = (random & 15) + 30;
        a = x + (((random & 255) - 128) << 8);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        random = (uint32)((sint32)random >> 8) | (random << 7);
        record->position[0] = (uint32)(a);
        record->position[1] = (uint32)(b);
        record->vector[0] = (uint16)(vx - 128 + (random & 255));
        record->vector[1] = (uint16)(vy - 192);
        value = vz - 128 + (((uint32)((sint32)random >> 7)) & 255);
        record->flags = (uint16)(0x86);
        record->ticks = (uint16)(0);
        record->duration = (uint16)(count);
        record->position[2] = (uint32)(c);
        record->vector[2] = (uint16)(value);
        record->style = (uint16)(0x1804);
        record->radius = (uint16)((random & 255) + 200);
    }
    result = random & 1;
    if (result == 0)
        return;
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record == 0)
        return;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    count = (random & 31) + 50;
    a = x + (((random & 255) - 128) << 8);
    b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
    c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
    rotated = (uint32)((sint32)random >> 8) | (random << 7);
    record->ticks = (uint16)(0xFFFF);
    record->position[1] = (uint32)(b);
    record->vector[0] = (uint16)(vx - 64 + (rotated & 127));
    record->position[0] = (uint32)(a);
    record->vector[1] = (uint16)(vy - 192);
    value = vz - 64 + (((uint32)((sint32)rotated >> 7)) & 127);
    record->flags = (uint16)(0x87);
    record->duration = (uint16)(count);
    record->position[2] = (uint32)(c);
    record->vector[2] = (uint16)(value);
    record->style = (uint16)(0x210);
    record->radius = (uint16)((rotated & 127) + 200);
    return;
}

static void effect_position_style_0(uint32 extra, const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, seed, random, result, i, height, speed, duration, packed, a, b, c, vertical, lifetime, counter;
    FUNCTION_MARKER(0x80083554, "ACE3.BIN");
    y = (native_position[1]);
    x = (native_position[0]);
    z = (native_position[2]);
    y += 0xFFBE6000;
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    result = 0;
    if (effect != 0)
    {
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        effect->ticks = (uint16)(10);
        effect->duration = (uint16)((random & 15) + 40);
        effect->vector[0] = (uint16)(random & 63);
        effect->vector[1] = (uint16)(-352);
        effect->vector[2] = (uint16)(((uint32)((sint32)random >> 6)) & 63);
        effect->flags = (uint16)(133);
        result = (random & 127) + 200;
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->style = (uint16)(0x1804);
        effect->radius = (uint16)(result);
    }
    if (extra == 0)
    {
        (void)(result);
        return;
    }
    height = y;
    speed = -320;
    duration = 200;
    for (i = 0; i < 5; ++i)
    {
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        if (effect == 0)
            return;
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        b = height + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        height -= 0x50000;
        vertical = speed - 128;
        speed -= 96 + (packed & 15);
        lifetime = duration + (packed & 63);
        duration += 70 + (packed & 7);
        counter = i * 4 + (random & 3) + 16;
        a = x + (((random & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        effect->ticks = (uint16)(0xFFFF);
        effect->vector[0] = (uint16)((packed & 255) - 128);
        vertical += ((uint32)((sint32)packed >> 3)) & 255;
        effect->vector[2] = (uint16)((((uint32)((sint32)packed >> 6)) & 255) - 128);
        effect->flags = (uint16)(133);
        effect->style = (uint16)(256);
        effect->duration = (uint16)(counter);
        effect->position[0] = (uint32)(a);
        effect->position[1] = (uint32)(b);
        effect->position[2] = (uint32)(c);
        effect->vector[1] = (uint16)(vertical);
        effect->radius = (uint16)(lifetime);
    }
    return;
}

static void effect_position_style_1(const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, i, seed, random, lifetime;
    FUNCTION_MARKER(0x80083764, "ACE3.BIN");
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    y -= 0xF0000;
    for (i = 0; i < 2; ++i)
    {
        effect = effect_drift_allocate(&effect_drift_secondary_pool);
        if (effect == 0)
            return;
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        lifetime = 220 + i * 80 + (random & 127);
        effect->duration = (uint16)(i + (random & 3) + 30);
        effect->flags = (uint16)(397);
        effect->style = (uint16)(0x1E00);
        effect->ticks = (uint16)(0);
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->vector[0] = (uint16)(0);
        effect->vector[1] = (uint16)(0);
        effect->vector[2] = (uint16)(0);
        effect->radius = (uint16)(lifetime);
    }
    return;
}

static void effect_position_style_2(uint32 extra, const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, seed, random, packed, a, b, c, counter, result;
    FUNCTION_MARKER(0x80083844, "ACE3.BIN");
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    result = 0;
    if (effect != 0)
    {
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        counter = (random & 15) + 40;
        a = x + (((random & 255) - 128) << 10);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
        effect->ticks = (uint16)(10);
        effect->vector[0] = (uint16)((packed + 20) & 127);
        effect->vector[2] = (uint16)(((uint32)((sint32)packed >> 6) + 50) & 127);
        effect->vector[1] = (uint16)(-400);
        effect->flags = (uint16)(135);
        effect->style = (uint16)(0x1a04);
        result = (packed & 127) + 500;
        effect->duration = (uint16)(counter);
        effect->position[0] = (uint32)(a);
        effect->position[1] = (uint32)(b);
        effect->position[2] = (uint32)(c);
        effect->radius = (uint16)(result);
    }
    if (extra != 0)
    {
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        result = 0;
        if (effect != 0)
        {
            seed = random_seed;
            random = psyq_rand_next(&seed);
            random_seed = seed;
            effect->ticks = (uint16)(10);
            effect->duration = (uint16)((random & 15) + 100);
            a = x + (((random & 255) - 128) << 11);
            effect->position[0] = (uint32)(a);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 11);
            effect->position[1] = (uint32)(b);
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 11);
            effect->position[2] = (uint32)(c);
            seed = random_seed;
            random = psyq_rand_next(&seed);
            random_seed = seed;
            effect->vector[0] = (uint16)((random & 127) + 20);
            effect->vector[2] = (uint16)((((uint32)((sint32)random >> 7)) & 127) + 50);
            effect->vector[1] = (uint16)(0 - 400 - (random & 15));
            effect->flags = (uint16)(128);
            effect->style = (uint16)(0x210);
            result = (((uint32)((sint32)random >> 4)) & 127) + 500;
            effect->radius = (uint16)(result);
        }
    }
    {
        (void)(result);
        return;
    }
}

static void effect_position_style_3(uint32 extra, const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, seed, random, packed, a, b, c, counter, result;
    FUNCTION_MARKER(0x80083A40, "ACE3.BIN");
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    result = 0;
    if (effect != 0)
    {
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        counter = (random & 7) + 30;
        a = x + (((random & 255) - 128) << 10);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
        effect->ticks = (uint16)(10);
        effect->vector[0] = (uint16)((packed + 20) & 127);
        effect->vector[2] = (uint16)(((uint32)((sint32)packed >> 6) + 50) & 127);
        effect->vector[1] = (uint16)(-400);
        effect->flags = (uint16)(135);
        effect->style = (uint16)(0x1904);
        result = (packed & 127) + 400;
        effect->duration = (uint16)(counter);
        effect->position[0] = (uint32)(a);
        effect->position[1] = (uint32)(b);
        effect->position[2] = (uint32)(c);
        effect->radius = (uint16)(result);
    }
    if (extra != 0)
    {
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        result = 0;
        if (effect != 0)
        {
            seed = random_seed;
            random = psyq_rand_next(&seed);
            random_seed = seed;
            effect->ticks = (uint16)(8);
            effect->duration = (uint16)((random & 7) + 68);
            a = x + (((random & 255) - 128) << 10);
            effect->position[0] = (uint32)(a);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
            effect->position[1] = (uint32)(b);
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
            effect->position[2] = (uint32)(c);
            seed = random_seed;
            random = psyq_rand_next(&seed);
            random_seed = seed;
            effect->vector[0] = (uint16)((random & 127) + 20);
            effect->vector[2] = (uint16)((((uint32)((sint32)random >> 7)) & 127) + 50);
            effect->vector[1] = (uint16)(0 - 400 - (random & 31));
            effect->flags = (uint16)(128);
            effect->style = (uint16)(0x210);
            result = (((uint32)((sint32)random >> 4)) & 127) + 400;
            effect->radius = (uint16)(result);
        }
    }
    {
        (void)(result);
        return;
    }
}

void effect_emit_vector_style(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    FUNCTION_MARKER(0x80081BC8, "ACE3.BIN");
    if (kind >= 4)
        return;
    switch (kind)
    {
        case 0:
        {
            (void)(effect_vector_style_0(native_position, native_vector));
            return;
        }
        case 1:
        {
            (void)(effect_vector_style_1(native_position, native_vector));
            return;
        }
        case 2:
        {
            (void)(effect_vector_style_2(native_position, native_vector));
            return;
        }
        case 3:
        {
            (void)(effect_vector_style_3(native_position, native_vector));
            return;
        }
        default:
            return;
    }
}

void effect_emit_position_style(uint32 value, uint32 mode, const uint32 *native_position)
{
    FUNCTION_MARKER(0x80083C3C, "ACE3.BIN");
    if (mode >= 4)
        return;
    switch (mode)
    {
        case 0:
        {
            (void)(effect_position_style_0(value, native_position));
            return;
        }
        case 1:
        {
            (void)(effect_position_style_1(native_position));
            return;
        }
        case 2:
        {
            (void)(effect_position_style_2(value, native_position));
            return;
        }
        case 3:
        {
            (void)(effect_position_style_3(value, native_position));
            return;
        }
        default:
            return;
    }
}

void effect_emit_vector_periodic(uint32 tick, uint32 kind, const uint32 *native_vector, const uint32 *native_position)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80081C30, "ACE3.BIN");
    if (kind < 2)
    {
        flags = game_runtime.mission_control.flags;
        if ((flags & 0x100) != 0)
        {
            kind = 1;
            tick = 0;
        }
    }
    switch (kind)
    {
        case 0:
            result = tick & 3;
            if (result != 0)
            {
                (void)(result);
                return;
            }
            break;
        case 1:
            result = tick & 1;
            if (result != 0)
            {
                (void)(result);
                return;
            }
            break;
        case 2:
            result = tick & 3;
            if (result != 0)
            {
                (void)(result);
                return;
            }
            result = game_random() & 1;
            if (result != 0)
            {
                (void)(result);
                return;
            }
            break;
        case 3:
            result = tick & 1;
            if (result != 0)
            {
                (void)(result);
                return;
            }
            result = game_random() & 3;
            if (result == 0)
            {
                (void)(result);
                return;
            }
            break;
    }
    mission_replay_record_vector_effect(2, native_position, native_vector, kind);
    {
        (void)(effect_emit_vector_style(kind, native_position, native_vector));
        return;
    }
}

void effect_emit_trail_periodic(uint32 tick, const uint32 *native_vector, const uint32 *native_position)
{
    uint32 flags, value;
    FUNCTION_MARKER(0x800821EC, "ACE3.BIN");
    flags = game_runtime.mission_control.flags & 0x100;
    if (flags != 0)
    {
        effect_emit_trail(9, native_position, native_vector);
        {
            (void)(effect_emit_vector_periodic(0, 1, native_vector, native_position));
            return;
        }
    }
    value = tick & 1;
    if (value != 0)
    {
        (void)(value);
        return;
    }
    mission_replay_record_vector_effect(4, native_position, native_vector, tick);
    {
        (void)(effect_spawn_vector_type4(tick, native_position, native_vector));
        return;
    }
}

void effect_emit_position_periodic(uint32 value, uint32 mode, const uint32 *native_position)
{
    uint32 variant, result;
    FUNCTION_MARKER(0x80083CA4, "ACE3.BIN");
    variant = value;
    if (mode == 0)
    {
        result = ((sint32)value >> 4);
        if (value & 15)
        {
            (void)(result);
            return;
        }
        variant = 0;
        if ((result & 7) == 0)
            variant = (game_random() & 7) != 0;
    }
    else if (mode == 1)
    {
        result = value & 3;
        if (result)
        {
            (void)(result);
            return;
        }
    }
    mission_replay_record_position_effect(6, native_position, variant, mode);
    {
        (void)(effect_emit_position_style(variant, mode, native_position));
        return;
    }
}

void effect_emit_position_type0(uint32 kind, const uint32 *native_position)
{
    FUNCTION_MARKER(0x800800E0, "ACE3.BIN");
    mission_replay_record_position_effect(0, native_position, kind, 0);
    {
        (void)(effect_spawn_position_type0(kind, native_position));
        return;
    }
}

void effect_emit_material_hit(uint32 material, uint32 mode, const uint32 *native_position)
{
    uint32 kind;
    FUNCTION_MARKER(0x800804B8, "ACE3.BIN");
    kind = material & 15;
    mission_replay_record_position_effect(1, native_position, mode, kind);
    {
        (void)(effect_spawn_material_hit(kind, mode, native_position));
        return;
    }
}

void effect_emit_position_type2(uint32 tick, const uint32 *native_position)
{
    uint32 flags;
    FUNCTION_MARKER(0x8008276C, "ACE3.BIN");
    flags = game_runtime.mission_control.flags & 0x100;
    if (flags != 0)
    {
        (void)(flags);
        return;
    }
    mission_replay_record_position_effect(2, native_position, tick, 0);
    {
        (void)(effect_spawn_position_type2(tick, native_position));
        return;
    }
}

void effect_packet_emit_segment(uint32 parameter, uint32 kind, const uint32 *native_start, const uint32 *native_end)
{
    uint32 ex, sx, ey, ez, sy, sz, dx, dy, dz, shift;
    uint32 direction[3];
    FUNCTION_MARKER(0x800830DC, "ACE3.BIN");
    ex = (native_end[0]);
    sx = (native_start[0]);
    ey = (native_end[1]);
    ez = (native_end[2]);
    dx = ex - sx;
    sy = (native_start[1]);
    sz = (native_start[2]);
    dy = ey - sy;
    dz = ez - sz;
    shift = 0;
    if (dx + 0x800000 > 0x1000000 || (sint32)dy > 0x800000 || (sint32)dy < -8388608 || (sint32)dz > 0x800000 || (sint32)dz < -8388608)
    {
        dx = (sint32)dx >> 4;
        dy = (sint32)dy >> 4;
        dz = (sint32)dz >> 4;
        shift = 4;
    }
    direction[2] = dz;
    direction[1] = dy;
    direction[0] = dx;
    mission_replay_record_segment(parameter, kind, native_start, direction, shift);
}

void effect_emit_position_type4(uint32 kind, const uint32 *native_position)
{
    FUNCTION_MARKER(0x8008130C, "ACE3.BIN");
    mission_replay_record_position_effect(4, native_position, kind, 0);
    {
        (void)(effect_spawn_callback_kind(kind, native_position));
        return;
    }
}

void effect_emit_height_scaled(uint32 height, const uint32 *native_vector, const uint32 *native_position)
{
    FLIGHT_STATUS *base;
    uint32 value, scale, input[3], x, y, z, speed;
    FUNCTION_MARKER(0x80082E80, "ACE3.BIN");
    value = game_runtime.mission_control.flags & 0x100;
    if (value)
    {
        (void)(value);
        return;
    }
    value = ((sint32)(((((uint32)native_position[1])))-height) >> 12);
    scale = value + 0x400;
    if ((sint32)scale < 0)
    {
        (void)(value);
        return;
    }
    base = &flight_status;
    value = base->surface_flags & 15;
    if (value != 1)
    {
        (void)(value);
        return;
    }
    x = ((native_vector[0]));
    y = ((native_vector[1]));
    z = ((native_vector[2]));
    speed = game_vector_length_gte((uint32)((sint32)x >> 4), (uint32)((sint32)y >> 4), (uint32)((sint32)z >> 4)) << 4;
    if ((sint32)speed <= 0x88BF)
        return;
    scale = scale - (uint32)((sint32)scale >> 2) + (uint32)((sint32)(speed + 0xFFFD543E) >> 9);
    value = (sint32)scale < 0x400;
    if ((sint32)scale < 0)
    {
        (void)(value);
        return;
    }
    if ((sint32)scale < 0x400)
    {
        value = (uint32)rsin((sint32)(0x400 - scale));
        scale = ((sint32)(0x1000 - value) >> 4);
    }
    value = (sint32)scale < 0x100;
    if ((sint32)scale < 80)
    {
        (void)(value);
        return;
    }
    if ((sint32)scale >= 0x100)
        scale = 255;
    x = ((native_vector[0]));
    y = (((native_position[0])));
    input[0] = y - (x << 1);
    input[1] = height;
    x = ((native_vector[2]));
    y = (((native_position[2])));
    input[2] = y - (x << 1);
    mission_replay_record_position_effect(3, input, scale, 0);
    {
        (void)(effect_drift_emit_scaled(input, scale));
        return;
    }
}

void effect_spawn_debris(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 x, y, z, random, sx, sy, sz, index, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x80080534, "ACE3.BIN");
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        {
            uint32 packed_record_word = 0xEFFFF;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        {
            uint32 packed_record_word = 0x1000100;
            record->style = (uint16)packed_record_word;
            record->radius = (uint16)(packed_record_word >> 16);
        }
    }
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        random = game_random();
        render_effect->ticks = (uint16)(0);
        render_effect->duration = (uint16)((random & 3) + 9);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->kind = (uint16)(0);
        render_effect->radius = (uint16)((random & 63) + 400);
    }
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    sx = (sint32)sx >> 9;
    sy = (sint32)sy >> 9;
    sz = ((uint32)((sint32)sz >> 9)) - 128;
    for (index = 0; index < 8; ++index)
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
        {
            (void)(index + 32);
            return;
        }
        record->ticks = (uint16)(0);
        record->duration = (uint16)(index + 32);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 8);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 8);
        record->position[0] = (uint32)(px);
        record->vector[0] = (uint16)(sx - 128 + (rotated & 255));
        record->vector[1] = (uint16)(sy + ((uint32)((sint32)rotated >> 3) & 255) - 256);
        record->vector[2] = (uint16)(sz + ((uint32)((sint32)rotated >> 6) & 255));
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        if (index < 2)
        {
            record->flags = (uint16)(0x93);
            record->style = (uint16)(0x1504);
            value = (rotated & 31) + 240;
        }
        else
        {
            record->flags = (uint16)(0x188);
            record->style = (uint16)(0);
            value = index + 20;
        }
        record->radius = (uint16)(value);
    }
    return;
}

void effect_emit_drift_group(uint32 mode, const uint32 *native_vector, const uint32 *native_position)
{
    uint32 flags;
    FUNCTION_MARKER(0x80081298, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    if (flags & 0x100)
        mode = 1;
    mission_replay_record_vector_effect(1, native_position, native_vector, mode);
    {
        (void)(effect_spawn_drift_group(mode, native_position, native_vector));
        return;
    }
}

void effect_spawn_surface_hit(uint32 direction, const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 x, y, z, random, sx, sy, sz, index, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x800807A4, "ACE3.BIN");
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        {
            uint32 packed_record_word = 0xEFFFF;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        {
            uint32 packed_record_word = 0x1000100;
            record->style = (uint16)packed_record_word;
            record->radius = (uint16)(packed_record_word >> 16);
        }
    }
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        random = game_random();
        render_effect->duration = (uint16)((random & 3) + 9);
        render_effect->ticks = (uint16)(0);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->angles[0] = (uint16)(direction);
        render_effect->angles[1] = (uint16)(direction >> 16);
        render_effect->kind = (uint16)(1);
        render_effect->radius = (uint16)((random & 63) + 400);
    }
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    sx = (sint32)(0 - sx) >> 11;
    sy = (sint32)(0 - sy) >> 11;
    sz = (sint32)(0 - sz) >> 11;
    for (index = 0; index < 8; ++index)
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
        {
            (void)(index + 24);
            return;
        }
        record->ticks = (uint16)(0);
        record->duration = (uint16)(index + 24);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 9);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 9);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 9);
        record->vector[0] = (uint16)(sx + (((rotated & 255) - 128) << 1));
        record->vector[1] = (uint16)(sy + ((uint32)((sint32)rotated >> 2) & 0x1FE) - 304);
        record->vector[2] = (uint16)(sz + ((((uint32)((sint32)rotated >> 6) & 255) - 128) << 1));
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        if (index < 2)
        {
            record->flags = (uint16)(0x8B);
            record->style = (uint16)(0x1504);
            value = record->vector[1];
            record->radius = (uint16)((rotated & 31) + 240);
            record->vector[1] = (uint16)(value - 128);
        }
        else
        {
            record->flags = (uint16)(0x188);
            record->style = (uint16)(0);
            record->radius = (uint16)(index + 20);
        }
    }
    return;
}

void effect_spawn_drift_group(uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 x, y, z, random, index, sx, sy, sz, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x80080F70, "ACE3.BIN");
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    if (mode == 1)
    {
        (void)(effect_spawn_kind_18(x, y, z));
        return;
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        random = game_random();
        {
            uint32 packed_record_word = 0x1CFFFF;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        record->style = (uint16)(0x210);
        record->radius = (uint16)((random & 63) + 588);
    }
    for (index = 0; index < 4; ++index)
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            break;
        random = game_random();
        record->ticks = (uint16)(0 - (random & 1) - (index << 1));
        record->duration = (uint16)(((uint32)((sint32)random >> 1) & 3) + 15);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 12));
        record->position[1] = (uint32)(y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12));
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12));
        record->style = (uint16)(0x100);
        record->radius = (uint16)(((uint32)((sint32)random >> 7) & 127) + 300 - (index << 5));
        record->flags = (uint16)(0);
    }
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        random = game_random();
        render_effect->ticks = (uint16)(0);
        render_effect->duration = (uint16)((random & 3) + 16);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->kind = (uint16)(0);
        render_effect->radius = (uint16)((random & 255) + 1600);
    }
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    sx = (sint32)sx >> 9;
    sy = (sint32)sy >> 9;
    sz = ((uint32)((sint32)sz >> 9)) - 128;
    for (index = 0; index < 16; ++index)
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
        {
            (void)(index + 28);
            return;
        }
        record->ticks = (uint16)(0);
        record->duration = (uint16)(index + 28);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 9);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 9) + 0xFFFD8000;
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 9);
        record->position[0] = (uint32)(px);
        record->vector[0] = (uint16)(sx - 128 + (rotated & 255));
        record->vector[1] = (uint16)(sy + ((uint32)((sint32)rotated >> 3) & 255) - 192);
        record->vector[2] = (uint16)(sz + ((uint32)((sint32)rotated >> 6) & 255));
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        if (index < 3)
        {
            record->flags = (uint16)(0x8C);
            record->style = (uint16)(0x1504);
            value = record->vector[1];
            record->radius = (uint16)((index << 5) + 280);
            record->vector[1] = (uint16)(value - 128);
        }
        else
        {
            record->flags = (uint16)(0x188);
            record->style = (uint16)(0);
            record->radius = (uint16)(index + 24);
        }
    }
    return;
}

void effect_spawn_position_type0(uint32 kind, const uint32 *native_position)
{
    EFFECT_DRIFT *record;
    uint32 x, y, z, base, remaining, random, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x8007FE28, "ACE3.BIN");
    if (kind >= 6)
        return;
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        base = 50;
        {
            uint32 packed_record_word = 0x8FFFF;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        if (kind == 2)
            base = 100;
        record->style = (uint16)(0x100);
        random = game_random();
        record->radius = (uint16)(base + (random & 31));
    }
    remaining = kind == 2 ? 5 : 2;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            break;
        record->ticks = (uint16)(0);
        record->duration = (uint16)(remaining + 29);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 127) - 64) << 10);
        py = y + ((((uint32)((sint32)random >> 3) & 127) - 64) << 10);
        pz = z + ((((uint32)((sint32)random >> 6) & 127) - 64) << 10);
        record->vector[0] = (uint16)((rotated & 127) - 64);
        record->vector[1] = (uint16)(((uint32)((sint32)rotated >> 3) & 127) - 80);
        record->vector[2] = (uint16)(((uint32)((sint32)rotated >> 6) & 127) - 64);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        if (remaining == 1)
        {
            record->style = (uint16)(0x1604);
            record->radius = (uint16)((rotated & 63) + 110);
            record->flags = (uint16)(0);
            value = 0x91;
        }
        else
        {
            record->radius = (uint16)(remaining + (rotated & 3) + 11);
            record->style = (uint16)(0);
            value = 0x188;
        }
        --remaining;
        record->flags = (uint16)(value);
    } while (remaining != 0);
    if (kind < 3)
        return;
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    if (kind == 3)
    {
        {
            uint32 packed_record_word = 0xE0001;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->style = (uint16)(0x801);
        random = game_random();
        value = (random & 31) + 144;
    }
    else
    {
        {
            uint32 packed_record_word = 0x120001;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->style = (uint16)(kind == 4 ? 0xC00 : 0xC03);
        random = game_random();
        value = (random & 31) + 184;
    }
    record->radius = (uint16)(value);
    record->position[0] = (uint32)(x);
    record->position[1] = (uint32)(y);
    record->position[2] = (uint32)(z);
    random = game_random();
    record->vector[0] = (uint16)((random & 63) - 32);
    record->vector[1] = (uint16)(((uint32)((sint32)random >> 3) & 63) - 132);
    record->vector[2] = (uint16)(((uint32)((sint32)random >> 6) & 63) - 32);
    record->flags = (uint16)(144);
    return;
}

void effect_spawn_vector_type7(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *record;
    uint32 random, x, y, z, base, value, result;
    FUNCTION_MARKER(0x80082A3C, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    random = game_random();
    {
        uint32 packed_record_word = ((random & 3) + 9) << 16;
        record->ticks = (uint16)packed_record_word;
        record->duration = (uint16)(packed_record_word >> 16);
    }
    record->style = (uint16)(0x1C04);
    record->radius = (uint16)((random & 63) + 50);
    random = game_random();
    x = ((native_vector[0]));
    base = (native_position[0]);
    y = ((native_vector[1]));
    z = ((native_vector[2]));
    value = base + x + (((random & 255) - 128) << 8);
    record->position[0] = (uint32)(value);
    base = (native_position[1]);
    value = base + y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
    x = (sint32)x >> 10;
    y = ((uint32)((sint32)y >> 10)) - 144;
    record->position[1] = (uint32)(value);
    base = (native_position[2]);
    record->vector[2] = (uint16)((sint32)z >> 10);
    record->flags = (uint16)(0x8A);
    result = (((uint32)((sint32)random >> 6) & 255) - 128) << 8;
    record->vector[0] = (uint16)(x);
    record->vector[1] = (uint16)(y);
    record->position[2] = (uint32)(base + z + result);
    {
        (void)(uint32)(result);
        return;
    }
}

void effect_emit_contact(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    FUNCTION_MARKER(0x80082B54, "ACE3.BIN");
    mission_replay_record_vector_effect(7, native_position, native_vector, kind);
    {
        (void)(effect_spawn_vector_type7(native_position, native_vector));
        return;
    }
}

void effect_spawn_callback_kind(uint32 kind, const uint32 *native_position)
{
    uint32 x, y, z;
    EFFECT_SPAWN_CALLBACK callback;
    FUNCTION_MARKER(0x80089BB4, "ACE3.BIN");
    if (kind >= 21)
    {
        (void)(kind << 2);
        return;
    }
    callback = effect_spawn_callbacks[kind];
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    {
        (void)(callback(x, y, z));
        return;
    }
}

void effect_spawn_burst(uint32 kind, const uint32 *native_position)
{
    uint32 x, y, z;
    EFFECT_SPAWN_CALLBACK callback;
    FUNCTION_MARKER(0x80080EB4, "ACE3.BIN");
    callback = (kind & 1) != 0 ? effect_spawn_mixed_drift_burst : effect_spawn_secondary_drift_burst;
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    {
        (void)(callback(x, y, z));
        return;
    }
}

void effect_spawn_position_type2(uint32 mode, const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 seed, random, value, x, y, z, result;
    FUNCTION_MARKER(0x80082594, "ACE3.BIN");
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[0]);
    x = value + (((random & 63) - 32) << 12);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[1]);
    y = value + (((random & 63) - 32) << 12);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[2]);
    z = value + (((random & 63) - 32) << 12);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    if ((mode & 1) != 0)
    {
        if (effect != 0)
        {
            {
                uint32 packed_record_word = 0x820000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            effect->vector[0] = (uint16)(random & 63);
            effect->vector[1] = (uint16)(-112);
            effect->vector[2] = (uint16)(((uint32)((sint32)random >> 6)) & 63);
            effect->flags = (uint16)(128);
            effect->style = (uint16)(0x210);
            value = (((uint32)((sint32)random >> 3)) & 63) + 78;
            effect->position[0] = (uint32)(x);
            effect->position[1] = (uint32)(y);
            effect->position[2] = (uint32)(z);
            effect->radius = (uint16)(value);
        }
    }
    else if (effect != 0)
    {
        {
            uint32 packed_record_word = 0xAFFFC;
            effect->ticks = (uint16)packed_record_word;
            effect->duration = (uint16)(packed_record_word >> 16);
        }
        effect->style = (uint16)(256);
        value = (random & 63) + 48;
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->flags = (uint16)(0);
        effect->radius = (uint16)(value);
    }
    result = random & 1;
    if ((mode & 1) != 0 && result != 0)
    {
        effect = effect_drift_allocate(&effect_drift_secondary_pool);
        result = 10;
        if (effect != 0)
        {
            effect->ticks = (uint16)(10);
            effect->duration = (uint16)((random & 31) + 40);
            effect->vector[0] = (uint16)(random & 63);
            effect->vector[1] = (uint16)(-112);
            effect->vector[2] = (uint16)(((uint32)((sint32)random >> 6)) & 63);
            effect->flags = (uint16)(128);
            effect->style = (uint16)(0x1604);
            result = (((uint32)((sint32)random >> 3)) & 63) + 100;
            effect->position[0] = (uint32)(x);
            effect->position[1] = (uint32)(y);
            effect->position[2] = (uint32)(z);
            effect->radius = (uint16)(result);
        }
    }
    {
        (void)(result);
        return;
    }
}

void effect_spawn_material_hit(uint32 kind, uint32 mode, const uint32 *native_position)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, seed, random, result, flags, material, i, packed, a, b, c, lifetime;
    FUNCTION_MARKER(0x80080140, "ACE3.BIN");
    if (mode >= 6)
        return;
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    if (mode >= 3)
    {
        effect = effect_drift_allocate(&effect_drift_secondary_pool);
        if (effect == 0)
            return;
        effect->flags = (uint16)(144);
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->vector[2] = (uint16)(0);
        if (mode == 3)
        {
            {
                uint32 packed_record_word = 0x100000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            {
                uint32 packed_record_word = 0xFF800000;
                effect->vector[0] = (uint16)packed_record_word;
                effect->vector[1] = (uint16)(packed_record_word >> 16);
            }
            effect->style = (uint16)(0x601);
        }
        else
        {
            {
                uint32 packed_record_word = 0x140000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            {
                uint32 packed_record_word = 0xFF600000;
                effect->vector[0] = (uint16)packed_record_word;
                effect->vector[1] = (uint16)(packed_record_word >> 16);
            }
            effect->style = (uint16)(mode == 4 ? 0x800 : 0x803);
        }
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        result = (random & 63) + (mode == 3 ? 208 : 268);
        effect->radius = (uint16)(result);
        {
            (void)(result);
            return;
        }
    }
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    effect->position[0] = (uint32)(x);
    effect->position[1] = (uint32)(y);
    effect->position[2] = (uint32)(z);
    if (mode == 2 && kind != 1)
    {
        {
            uint32 packed_record_word = 0xBFFFF;
            effect->ticks = (uint16)packed_record_word;
            effect->duration = (uint16)(packed_record_word >> 16);
        }
        effect->flags = (uint16)(0);
        effect->style = (uint16)(256);
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        effect->radius = (uint16)((random & 127) + 120);
        for (i = 0; i < 4; ++i)
        {
            effect = effect_drift_allocate(&effect_drift_secondary_pool);
            result = i + 16;
            if (effect == 0)
            {
                (void)(result);
                return;
            }
            effect->ticks = (uint16)(0);
            effect->duration = (uint16)(result);
            seed = random_seed;
            random = psyq_rand_next(&seed);
            random_seed = seed;
            packed = (uint32)((sint32)random >> 8) | (random << 7);
            lifetime = (i & 3) + 14;
            a = x + (((random & 255) - 128) << 8);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8) - 0x28000;
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
            effect->vector[0] = (uint16)(((packed & 255) - 128) << 2);
            effect->vector[1] = (uint16)((((uint32)((sint32)packed >> 1)) & 0x3FC) - 640);
            effect->vector[2] = (uint16)((((((uint32)((sint32)packed >> 6)) & 255) - 128) << 2));
            effect->flags = (uint16)(394);
            result = i + 1 < 4;
            effect->position[0] = (uint32)(a);
            effect->position[1] = (uint32)(b);
            effect->position[2] = (uint32)(c);
            effect->style = (uint16)(0);
            effect->radius = (uint16)(lifetime);
        }
        {
            (void)(result);
            return;
        }
    }
    if (mode == 2)
    {
        flags = 0xEFFFF;
        material = 0x640120;
    }
    else
    {
        switch (kind)
        {
            case 1:
                flags = 0xCFFFF;
                material = 0x500120;
                break;
            case 2:
                flags = 0xAFFFF;
                material = 0x500121;
                break;
            case 3:
                flags = 0xAFFFF;
                material = 0x500122;
                break;
            case 4:
            {
                uint32 packed_record_word = 0xCFFFF;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
                seed = random_seed;
                random = psyq_rand_next(&seed);
                random_seed = seed;
                effect->vector[0] = (uint16)((random & 63) - 32);
                a = (((uint32)((sint32)random >> 3)) & 63) - 32;
                b = (((uint32)((sint32)random >> 6)) & 63) - 32;
                effect->vector[2] = (uint16)(b);
                effect->vector[1] = (uint16)(a);
                effect->flags = (uint16)(136);
                {
                    uint32 packed_record_word = 0x640123;
                    effect->style = (uint16)packed_record_word;
                    effect->radius = (uint16)(packed_record_word >> 16);
                }
                return;
            case 5:
                flags = 0xAFFFF;
                material = 0x500124;
                break;
            default:
            {
                uint32 packed_record_word = 0xAFFFF;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
                effect->flags = (uint16)(0);
                {
                    uint32 packed_record_word = 0x500000;
                    effect->style = (uint16)packed_record_word;
                    effect->radius = (uint16)(packed_record_word >> 16);
                }
                return;
        }
    }
    {
        uint32 packed_record_word = flags;
        effect->ticks = (uint16)packed_record_word;
        effect->duration = (uint16)(packed_record_word >> 16);
    }
    effect->flags = (uint16)(0);
    {
        uint32 packed_record_word = material;
        effect->style = (uint16)packed_record_word;
        effect->radius = (uint16)(packed_record_word >> 16);
    }
    {
        (void)(flags);
        return;
    }
}

void effect_spawn_vector_type8(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *effect;
    uint32 x, y, z, value, seed, random, result;
    FUNCTION_MARKER(0x80082C1C, "ACE3.BIN");
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    if (effect == 0)
        return;
    x = (native_vector[0]);
    y = (native_vector[1]);
    value = (native_position[0]);
    z = (native_vector[2]);
    effect->position[0] = (uint32)(value - x);
    value = (native_position[1]);
    effect->position[1] = (uint32)(value - y);
    value = (native_position[2]);
    effect->flags = (uint16)(135);
    effect->position[2] = (uint32)(value - z);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    effect->ticks = (uint16)((random & 1) + 4);
    effect->duration = (uint16)((random & 3) + 40);
    x = (uint32)((sint32)(0 - x) >> 6) - 64 + (random & 127);
    y = (uint32)((sint32)(0 - y) >> 6) - 64 + (((uint32)((sint32)random >> 3)) & 127);
    z = (uint32)((sint32)(0 - z) >> 6) - 64 + (((uint32)((sint32)random >> 6)) & 127);
    result = (random & 63) + 240;
    effect->vector[0] = (uint16)(x);
    effect->vector[1] = (uint16)(y);
    effect->vector[2] = (uint16)(z);
    effect->style = (uint16)(0x111);
    effect->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

void effect_spawn_vector_type3(const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *effect;
    uint32 flags, x, y, z, vx, vy, vz, value, i, seed, random, packed, a, b, c, result;
    FUNCTION_MARKER(0x80081D4C, "ACE3.BIN");
    x = (native_position[0]);
    y = (native_position[1]);
    flags = game_runtime.mission_control.flags;
    z = (native_position[2]);
    if ((flags & 256) != 0)
    {
        (void)(effect_spawn_kind_18(x, y, z));
        return;
    }
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    if (effect != 0)
    {
        {
            uint32 packed_record_word = 0xFFFFF;
            effect->ticks = (uint16)packed_record_word;
            effect->duration = (uint16)(packed_record_word >> 16);
        }
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->flags = (uint16)(0);
        {
            uint32 packed_record_word = 0x1900100;
            effect->style = (uint16)packed_record_word;
            effect->radius = (uint16)(packed_record_word >> 16);
        }
    }
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        render_effect->ticks = (uint16)(0xC0000);
        render_effect->duration = (uint16)(0xC0000 >> 16);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->kind = (uint16)(0x2BC0000);
        render_effect->radius = (uint16)(0x2BC0000 >> 16);
    }
    value = (native_vector[1]);
    vy = (sint32)(0 - value) >> 12;
    value = (native_vector[2]);
    vz = (sint32)(0 - value) >> 12;
    value = (native_vector[0]);
    vx = (sint32)(0 - value) >> 12;
    for (i = 0; i < 20; ++i)
    {
        effect = effect_drift_allocate(&effect_drift_secondary_pool);
        result = i + 20;
        if (effect == 0)
        {
            (void)(result);
            return;
        }
        effect->ticks = (uint16)(0);
        effect->duration = (uint16)(result);
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        a = x + (((random & 255) - 128) << 8);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        effect->vector[0] = (uint16)(vx + (((packed & 255) - 128) << 1));
        effect->vector[1] = (uint16)(vy + (((uint32)((sint32)packed >> 2)) & 0x1FE) - 288);
        effect->vector[2] = (uint16)(vz + (((((uint32)((sint32)packed >> 6)) & 255) - 128) << 1));
        effect->position[0] = (uint32)(a);
        effect->position[1] = (uint32)(b);
        effect->position[2] = (uint32)(c);
        if (i < 6)
        {
            effect->flags = (uint16)(138);
            effect->style = (uint16)(0x1804);
            effect->radius = (uint16)((vx & 63) + 600);
        }
        else
        {
            effect->flags = (uint16)(392);
            effect->style = (uint16)(0);
            effect->radius = (uint16)((i >> 1) + 18);
        }
    }
    return;
}

void effect_spawn_vector_type4(uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *effect;
    uint32 vx, vy, vz, x, y, z, value, seed, random, noise, result;
    FUNCTION_MARKER(0x80081FDC, "ACE3.BIN");
    vx = (native_vector[0]);
    vy = (native_vector[1]);
    vz = (native_vector[2]);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[0]);
    x = value + vx + ((random & 63) << 11);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[1]);
    y = value + vy + ((random & 63) << 11);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = (native_position[2]);
    z = value + vz + ((random & 63) << 11);
    seed = random_seed;
    noise = psyq_rand_next(&seed);
    random_seed = seed;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    noise |= random << 15;
    if ((mode & 7) == 0)
    {
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        if (effect != 0)
        {
            {
                uint32 packed_record_word = 0xE0000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            effect->style = (uint16)(256);
            effect->position[0] = (uint32)(x);
            effect->position[1] = (uint32)(y);
            effect->position[2] = (uint32)(z);
            effect->flags = (uint16)(0);
            effect->radius = (uint16)((noise & 63) + 58);
        }
    }
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    if (effect != 0)
    {
        {
            uint32 packed_record_word = 0x100000;
            effect->ticks = (uint16)packed_record_word;
            effect->duration = (uint16)(packed_record_word >> 16);
        }
        effect->style = (uint16)(0x210);
        effect->position[0] = (uint32)(x);
        effect->position[1] = (uint32)(y);
        effect->position[2] = (uint32)(z);
        effect->flags = (uint16)(0);
        effect->radius = (uint16)((noise & 63) + 68);
    }
    result = noise & 3;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    if (effect == 0)
        return;
    {
        uint32 packed_record_word = 0x180000;
        effect->ticks = (uint16)packed_record_word;
        effect->duration = (uint16)(packed_record_word >> 16);
    }
    effect->vector[0] = (uint16)((uint32)((sint32)vx >> 9) - 64 + (noise & 127));
    effect->vector[1] = (uint16)((uint32)((sint32)vy >> 9) + (((uint32)((sint32)noise >> 3)) & 127) - 96);
    value = (uint32)((sint32)vz >> 9) - 64 + (((uint32)((sint32)noise >> 6)) & 127);
    effect->position[0] = (uint32)(x);
    effect->position[1] = (uint32)(y);
    effect->position[2] = (uint32)(z);
    effect->vector[2] = (uint16)(value);
    if ((noise & 1) != 0)
    {
        effect->flags = (uint16)(392);
        {
            uint32 packed_record_word = 0xE0000;
            effect->style = (uint16)packed_record_word;
            effect->radius = (uint16)(packed_record_word >> 16);
        }
        return;
    }
    effect->flags = (uint16)(139);
    effect->style = (uint16)(0x1704);
    result = (noise & 31) + 60;
    effect->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

void effect_emit_vector_type3(const uint32 *native_vector, const uint32 *native_position)
{
    FUNCTION_MARKER(0x80081F94, "ACE3.BIN");
    mission_replay_record_vector_effect(3, native_position, native_vector, 0);

    {
        (void)(effect_spawn_vector_type3(native_position, native_vector));
        return;
    }
}

void effect_emit_vector_type8(const uint32 *native_vector, const uint32 *native_position)
{
    FUNCTION_MARKER(0x80082D38, "ACE3.BIN");
    mission_replay_record_vector_effect(8, native_position, native_vector, 0);

    {
        (void)(effect_spawn_vector_type8(native_position, native_vector));
        return;
    }
}

void effect_emit_secondary_drift(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *effect;
    static const EFFECT_DRIFT_STYLE styles[11] = {
        {0x1E0F0014, 0x01831403}, {0x3C1F0028, 0x01851303}, {0x487F0096, 0x018C1400}, {0x3F780032, 0x018C1300}, {0x5A7F00B4, 0x018C1606}, {0x5A7F00B4, 0x018C1605}, {0x483F008C, 0x018D1400}, {0x483F008C, 0x018D1400}, {0x28FF01F4, 0x00831C04}, {0x1E3F00C8, 0x00841A04}, {0x1E0F001E, 0x01851403},
    };
    uint32 x, y, z, a, b, c, word, material, seed, random, result;
    FUNCTION_MARKER(0x800827E0, "ACE3.BIN");
    if (kind >= 11)
        return;
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    if (effect == 0)
        return;
    x = native_vector[0];
    a = native_position[0];
    y = native_vector[1];
    z = native_vector[2];
    effect->position[0] = (uint32)(a - x - (uint32)((sint32)x >> 2));
    b = native_position[1];
    effect->position[1] = (uint32)(b - y - (uint32)((sint32)y >> 2));
    c = native_position[2];
    effect->vector[0] = (uint16)((sint32)x >> 8);
    effect->vector[1] = (uint16)((sint32)y >> 8);
    effect->vector[2] = (uint16)((sint32)z >> 8);
    effect->position[2] = (uint32)(c - z - (uint32)((sint32)z >> 2));
    word = styles[kind].radius_duration;
    material = styles[kind].style_flags;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    {
        uint32 packed_record_word = (uint32)((sint32)word >> 24) << 16;
        effect->ticks = (uint16)packed_record_word;
        effect->duration = (uint16)(packed_record_word >> 16);
    }
    effect->flags = (uint16)(material >> 16);
    result = word + (random & ((word >> 16) & 255));
    effect->style = (uint16)(material);
    effect->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

void effect_drift_emit_velocity_burst(uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    EFFECT_DRIFT *effect;
    uint32 vx, vy, vz, x, y, z, value, seed, random, noise, result;
    FUNCTION_MARKER(0x800822A4, "ACE3.BIN");
    vx = native_vector[0];
    vy = native_vector[1];
    vz = native_vector[2];
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    value = native_position[0];
    x = value + vx + (((random & 63) - 32) << 12);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    vx = (sint32)vx >> 9;
    value = native_position[1];
    y = value + vy + (((random & 63) - 32) << 12);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    vy = (sint32)vy >> 9;
    value = native_position[2];
    z = value + vz + (((random & 63) - 32) << 12);
    vz = (sint32)vz >> 9;
    seed = random_seed;
    noise = psyq_rand_next(&seed);
    random_seed = seed;
    if (mode == 1)
        effect_drift_emit_scaled(native_position, 512);
    else
    {
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        if (effect != 0)
        {
            {
                uint32 packed_record_word = 0x100000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            effect->style = (uint16)(256);
            effect->position[0] = (uint32)(x);
            effect->position[1] = (uint32)(y);
            effect->position[2] = (uint32)(z);
            effect->flags = (uint16)(0);
            effect->radius = (uint16)((noise & 15) + 90);
        }
        effect = effect_drift_allocate(&effect_drift_primary_pool);
        if (effect != 0)
        {
            {
                uint32 packed_record_word = 0x640000;
                effect->ticks = (uint16)packed_record_word;
                effect->duration = (uint16)(packed_record_word >> 16);
            }
            effect->vector[0] = (uint16)(0 - vx - 64 + (noise & 127));
            effect->vector[1] = (uint16)((((uint32)((sint32)noise >> 3)) & 127) - 176);
            value = 0 - vz - 64 + (((uint32)((sint32)noise >> 6)) & 127);
            effect->flags = (uint16)(128);
            effect->style = (uint16)(0x210);
            effect->position[0] = (uint32)(x);
            effect->position[1] = (uint32)(y);
            effect->position[2] = (uint32)(z);
            effect->vector[2] = (uint16)(value);
            effect->radius = (uint16)((noise & 63) + 68);
        }
    }
    result = noise & 1;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    effect = effect_drift_allocate(&effect_drift_secondary_pool);
    if (effect == 0)
        return;
    {
        uint32 packed_record_word = 0x180000;
        effect->ticks = (uint16)packed_record_word;
        effect->duration = (uint16)(packed_record_word >> 16);
    }
    effect->position[0] = (uint32)(x);
    effect->position[1] = (uint32)(y);
    effect->position[2] = (uint32)(z);
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    effect->vector[0] = (uint16)(0 - vx - 128 + (random & 255));
    effect->vector[1] = (uint16)(0 - vy - 128 + (((uint32)((sint32)random >> 3)) & 255));
    value = 0 - vz - 128 + (((uint32)((sint32)random >> 6)) & 255);
    effect->flags = (uint16)(392);
    result = (random & 7) + 16;
    effect->vector[2] = (uint16)(value);
    effect->style = (uint16)(0);
    effect->radius = (uint16)(result);
    {
        (void)(result);
        return;
    }
}

uint32 effect_draw_screen_transition(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 value, page, first, current, tail, i, left, right, top, bottom, result;
    static const uint16 uv[16] = {0x0203, 0x03A1, 0x7905, 0x79A1, 0x7705, 0x77A1, 0xEE03, 0xEDA1, 0x031F, 0x02BD, 0x791F, 0x79BB, 0x771F, 0x77BB, 0xED1F, 0xEEBD};
    DR_MODE *mode, *restore;
    TILE *fill;
    POLY_FT4 *quads;
    FUNCTION_MARKER(0x8008FC9C, "ACE3.BIN");
    if ((sint32)strength < 0)
        return packet;
    if ((sint32)strength >= 4097)
        strength = 4096;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x62FFFFFF);
    value = (sint32)rsin((sint32)strength >> 1) >> 1;
    if ((sint32)strength < 2048)
        value = 0x1000 - value;
    xport_gte_write_data(8, value);
    page = game_runtime.display_buffer;
    page = (page == 0) << 8;
    xport_gte_execute(0x780010);
    first = packet + 28;
    mode = mission_packet_span(packet, sizeof(*mode));
    fill = mission_packet_span(packet + 12, sizeof(*fill));
    mode->tag = ((packet + 12) & 0xFFFFFF) | 0x02000000;
    fill->tag = (first & 0xFFFFFF) | 0x03000000;
    mode->code[0] = 0xE6000001;
    mode->code[1] = 0xE1000340;
    fill->p[0].xy = 0;
    fill->wh = 0x00F00140;
    if (strength < 4096)
        value = xport_gte_read_data(22);
    else
        value = 0x60000000;
    fill->rgbc.rgbc = value;
    tail = first;
    if ((strength & 0xFFF) != 0)
    {
        value = 0x1000 - rsin((sint32)strength >> 1);
        xport_gte_write_data(8, value);
        xport_gte_write_data(6, 0x2E7C7F7D);
        tail = first + 160;
        xport_gte_execute(0x780010);
        current = first;
        quads = mission_packet_span(first, 4 * sizeof(*quads));
        for (i = 0; i < 4; ++i, current += 40)
        {
            POLY_FT4 *quad = quads + i;
            quad->tag = ((current + 40) & 0xFFFFFF) | 0x09000000;
            quad->p[1].attr = (uint16)((page >> 4) | (i & 2) | 0x120);
            left = (i & 2) != 0 ? 160 : 0;
            right = left + 160;
            quad->p[2].xy.x = (sint16)(uint16)(left);
            quad->p[0].xy.x = (sint16)(uint16)(left);
            quad->p[3].xy.x = (sint16)(uint16)(right);
            quad->p[1].xy.x = (sint16)(uint16)(right);
            top = (i & 1) != 0 ? 120 : 0;
            bottom = top + 120;
            quad->p[1].xy.y = (sint16)(uint16)(top);
            quad->p[0].xy.y = (sint16)(uint16)(top);
            quad->p[3].xy.y = (sint16)(uint16)(bottom);
            quad->p[2].xy.y = (sint16)(uint16)(bottom);
        }
        value = xport_gte_read_data(22);
        quads[3].rgbc.rgbc = value;
        quads[2].rgbc.rgbc = value;
        quads[1].rgbc.rgbc = value;
        quads[0].rgbc.rgbc = value;
        for (i = 0; i < 16; ++i)
            quads[i >> 2].p[i & 3].uv.uv = (uint16)(uv[i]);
    }
    result = tail + 12;
    restore = mission_packet_span(tail, sizeof(*restore));
    restore->tag = (result & 0xFFFFFF) | 0x02000000;
    restore->code[0] = 0xE6000000;
    restore->code[1] = 0xE1000220;
    /* The LWL seed is 02000000 and contributes no retained low bits */
    value = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, tail, 0);
    return result;
}

uint32 effect_draw_noise_overlay(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 *setup, *restore;
    DR_LOAD *upload;
    uint16 *clut;
    DR_STP *mask;
    uint32 count, remaining, current, next, tail, value, i;
    FUNCTION_MARKER(0x80084560, "ACE3.BIN");
    count = (sint32)strength >> 8;
    if ((sint32)count <= 0)
        return packet;
    next = packet + 60;
    remaining = 16 - count;
    current = packet + 8;
    mask = (DR_STP *)mission_packet_span(packet, sizeof(*mask));
    mask->tag = (current & 0xFFFFFF) | 0x01000000;
    mask->code[0] = 0xE6000000;
    upload = (DR_LOAD *)mission_packet_span(packet + 8, sizeof(*upload));
    upload->tag = (next & 0xFFFFFF) | 0x0C000000;
    upload->code[0] = 0xA0000000;
    upload->code[1] = 0x010001D0;
    upload->code[2] = 0x10010;
    upload->p[8] = 0x01000000;
    clut = (uint16 *)upload->p;
    for (i = 0; i < 16; ++i, current += 2)
        clut[i] = ((sint32)i < (sint32)remaining ? 0 : 0x8000);
    current = next + 16;
    setup = (uint32 *)mission_packet_span(next, 4 * sizeof(*setup));
    setup[0] = (current & 0xFFFFFF) | 0x03000000;
    setup[1] = 0xE1000238;
    setup[2] = 0xE20C4318;
    setup[3] = 0;
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, 0);
    tail = ui_packets_emit_noise_tiles(current, 0x401D, 0xFF88FF60, 0xF00140, 0);
    restore = (uint32 *)mission_packet_span(tail, 4 * sizeof(*restore));
    restore[1] = 0xE1000220;
    restore[0] = 0x03000000;
    restore[2] = 0xE2000000;
    restore[3] = 0;
    /* The E2000000 LWL seed has no retained low bits */
    value = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, tail, 0);
    return tail + 16;
}

uint32 effect_draw_signed_noise_overlay(uint32 ordering, uint32 packet, uint32 factor)
{
    uint32 *setup, *restore;
    uint32 mode, result, carrier, value;
    FUNCTION_MARKER(0x8008488C, "ACE3.BIN");
    if (factor == 0)
        return packet;
    mode = 1;
    if ((sint32)factor < 0)
    {
        mode = 2;
        factor = 0 - factor;
    }
    setup = (uint32 *)mission_packet_span(packet, 4 * sizeof(*setup));
    setup[0] = ((packet + 16) & 0xFFFFFF) | 0x3000000;
    setup[1] = 0xE1000218 | (mode << 5);
    setup[2] = 0xE20C4318;
    setup[3] = 0;
    xport_gte_write_data(6, 0x66008300);
    xport_gte_write_data(8, 0x1000 - factor);
    result = ui_packets_emit_noise_tiles(packet + 16, 0x46D6, 0xFF88FF60, 0xF00140, &carrier);
    restore = (uint32 *)mission_packet_span(result, 4 * sizeof(*restore));
    restore[1] = 0xE1000220;
    restore[0] = 0x3000000;
    restore[2] = 0xE2000000;
    restore[3] = 0;
    value = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, result, carrier);
    return result + 16;
}

void effect_update_screen_overlays(EFFECT_FRAME_CONTEXT *frame)
{
    uint32 toggles, input, flags, first, packet, result;
    FUNCTION_MARKER(0x8008FFAC, "ACE3.BIN");
    toggles = (sint16)game_runtime.options.overlay_toggles;
    input = pad_state[0].pressed;
    if ((toggles & 4) != 0)
    {
        flags = effect_light.flags;
        toggles &= 0xFFFFFFFB;
        effect_light.flags = (uint32)(flags | 1);
        game_runtime.options.overlay_toggles = (uint16)toggles;
    }
    if ((input & 0x300) != 0)
    {
        if ((input & 0x200) != 0)
            toggles ^= 1;
        if ((input & 0x100) != 0)
        {
            if ((toggles & 2) != 0)
                toggles |= 4;
            toggles ^= 2;
        }
        game_runtime.options.overlay_toggles = (uint16)toggles;
    }
    if ((toggles & 1) != 0)
    {
        first = frame->ot;
        packet = frame->packet;
        result = effect_draw_framebuffer_copy(first - 4, packet, NULL);
        frame->packet = result;
    }
    result = toggles & 6;
    if (result != 0)
    {
        flags = effect_light.flags;
        effect_light.flags = (uint32)(flags | 1);
        result = toggles & 2;
        if (result != 0)
        {
            first = frame->ot;
            packet = frame->packet;
            result = effect_draw_signed_noise_overlay(first, packet, 0x400);
            frame->packet = result;
        }
    }
    {
        (void)(result);
        return;
    }
}

uint32 effect_draw_framebuffer_copy(uint32 ordering, uint32 packet, const XY *offset)
{
    uint32 page, current, next, tail, x, value, merge_seed;
    FUNCTION_MARKER(0x8008F950, "ACE3.BIN");
    current = packet + 8;
    page = game_runtime.display_buffer;
    DR_STP *mask = mission_packet_span(packet, 8);
    mask->tag = (current & 0xFFFFFF) | 0x01000000;
    mask->code[0] = 0xE6000001;
    page = (page == 0) << 8;
    if (offset != 0)
    {
        current = packet + 20;
        DR_OFFSET *shift = mission_packet_span(packet + 8, sizeof(*shift));
        shift->tag = (current & 0xFFFFFF) | 0x02000000;
        shift->code[0] = (((page + 256) & 256) << 11) | 0xE5000000;
        shift->code[1] = 0;
    }
    for (x = 0; x < 320; x += 256)
    {
        FRAMEBUFFER_COPY_PACKET *copy = mission_packet_span(current, sizeof(*copy));
        next = current + 24;
        copy->tag = (next & 0xFFFFFF) | 0x05000000;
        value = ((page & 256) >> 4) | ((x & 1023) >> 6) | 256 | ((page & 512) << 2) | 0xE1000200;
        copy->mode = value;
        copy->color.rgbc = 0x66838383;
        copy->point.xy.xy = x;
        copy->point.uv.uv = (uint16)(0);
        copy->wh = 0x00F00100;
        current = next;
    }
    if (offset != 0)
    {
        tail = current;
        value = offset->xy;
        merge_seed = 0xE5000000;
        DR_OFFSET *restore = mission_packet_span(current, sizeof(*restore));
        restore->tag = 0x02000000;
        restore->code[1] = 0;
        value = (value & 0x7FF) | (((value << 5) >> 21) << 11) | 0xE5000000;
        restore->code[0] = value;
        current += 12;
    }
    else
    {
        tail = current - 24;
        merge_seed = current;
    }
    /* Preserve the original LWL and two SWL merges for every byte alignment */
    value = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, tail, merge_seed);
    return current;
}

void effect_emit_player_exhaust(uint32 mission, uint32 variant)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    const uint32 *source_velocity;

    uint32 actor_index_table;

    FLIGHT_CONTROL *base;
    uint32 minimum, speed_minimum, kind, vector, x, y, z, value, px, py, pz;
    FUNCTION_MARKER(0x80084F6C, "ACE3.BIN");
    EFFECT_EMISSION_PUBLICATION *published = (EFFECT_EMISSION_PUBLICATION *)getScratchAddr(0);
    uint32 *origin = published->origin;
    if (mission == 58)
    {
        minimum = 0x780;
        speed_minimum = 30;
        kind = 5;
    }
    else
    {
        if ((variant - 25) >= 2)
            return;
        minimum = 0xC00;
        speed_minimum = 60;
        kind = variant - 19;
    }
    base = &flight_control;
    if ((sint16)base->thrust < (sint32)minimum)
        return;
    actor_index_table = 0;
    actor_motion_object = mission_actor_motion(actor_index_table);
    vector = 0;
    source_velocity = actor_motion_object->velocity;
    x = actor_motion_object->velocity[0];
    y = source_velocity[1];
    z = source_velocity[2];
    value = game_vector_length_gte((uint32)((sint32)x >> 12), (uint32)((sint32)y >> 12), (uint32)((sint32)z >> 12));
    if ((sint32)value < (sint32)speed_minimum)
        return;
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    px = pose_owner_object->pose.position[0];
    origin[0] = px;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    py = pose_owner_object->pose.position[1];
    origin[1] = py;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    pz = pose_owner_object->pose.position[2];
    origin[2] = pz;
    if ((variant - 25) >= 3)
    {
        origin[0] = px - (x - (uint32)((sint32)x >> 2));
        origin[1] = py - (y - (uint32)((sint32)y >> 2));
        origin[2] = pz - (z - (uint32)((sint32)z >> 2));
    }
    {
        (void)(effect_emit_trail(kind, origin, source_velocity));
        return;
    }
}

void effect_drift_emit_scaled(const uint32 position[3], uint32 scale)
{
    EFFECT_DRIFT *effect;
    uint32 value, size, leading, mask, half, offset, coordinate;
    FUNCTION_MARKER(0x80082D80, "ACE3.BIN");
    value = game_random();
    value = scale + (value & 0x3F);
    size = value - 32;
    if ((sint32)size <= 0)
    {
        (void)(value);
        return;
    }
    effect = effect_drift_allocate(&effect_drift_primary_pool);
    if (effect == 0)
        return;
    xport_gte_write_data(30, size);
    value = (uint32)((sint32)size >> 4) + 4;
    {
        uint32 packed_record_word = (value << 16) | 1;
        effect->ticks = (uint16)packed_record_word;
        effect->duration = (uint16)(packed_record_word >> 16);
    }
    {
        uint32 packed_record_word = (size << 16) | 0x130;
        effect->style = (uint16)packed_record_word;
        effect->radius = (uint16)(packed_record_word >> 16);
    }
    effect->flags = (uint16)(0);
    leading = xport_gte_read_data(31);
    mask = (1u << ((31 - leading) & 31)) - 1;
    half = 1u << ((30 - leading) & 31);
    value = game_random();
    offset = ((value & mask) - half) << 10;
    coordinate = position[0];
    effect->position[0] = (uint32)(coordinate + offset);
    value = game_random();
    offset = ((value & mask) - half) << 10;
    coordinate = position[2];
    effect->position[2] = (uint32)(coordinate + offset);
    value = position[1];
    effect->position[1] = (uint32)(value);
    {
        (void)(value);
        return;
    }
}

void effect_draw_mission_trails(void)
{
    uint32 a, b, c, d, e, ot, packet;
    FUNCTION_MARKER(0x80033324, "ACE3.BIN");
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
    a = (uint32)game_runtime.mission_matrices.view.t[0];
    b = (uint32)game_runtime.mission_matrices.view.t[1];
    c = (uint32)game_runtime.mission_matrices.view.t[2];
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    a = game_runtime.mission_lighting.back_color[0];
    b = game_runtime.mission_lighting.back_color[1];
    c = game_runtime.mission_lighting.back_color[2];
    xport_gte_write_control(13, a << 4);
    xport_gte_write_control(14, b << 4);
    xport_gte_write_control(15, c << 4);
    a = ((uint32)(uint16)game_runtime.mission_lighting.colors.m[0][0] | ((uint32)(uint16)game_runtime.mission_lighting.colors.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_lighting.colors.m[0][2] | ((uint32)(uint16)game_runtime.mission_lighting.colors.m[1][0] << 16));
    c = ((uint32)(uint16)game_runtime.mission_lighting.colors.m[1][1] | ((uint32)(uint16)game_runtime.mission_lighting.colors.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_lighting.colors.m[2][0] | ((uint32)(uint16)game_runtime.mission_lighting.colors.m[2][1] << 16));
    e = (uint16)game_runtime.mission_lighting.colors.m[2][2];
    xport_gte_write_control(16, a);
    xport_gte_write_control(17, b);
    xport_gte_write_control(18, c);
    xport_gte_write_control(19, d);
    xport_gte_write_control(20, e);
    ot = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    packet = effect_trails_draw(ot + 4, packet);
    game_runtime.mission_control.packet_cur = (uint32)(packet);
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
    a = (uint32)game_runtime.mission_matrices.view.t[0];
    b = (uint32)game_runtime.mission_matrices.view.t[1];
    c = (uint32)game_runtime.mission_matrices.view.t[2];
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    ot = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    packet = effect_draw_scene_frame(ot + 4, packet);
    game_runtime.mission_control.packet_cur = (uint32)(packet);
    {
        (void)(uint32)(packet);
        return;
    }
}

void effect_spawn_secondary_drift_burst(uint32 x, uint32 y, uint32 z)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 index, duration, random, mixed, result, px, py, pz;
    FUNCTION_MARKER(0x80080A94, "ACE3.BIN");
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        render_effect->ticks = (uint16)(0x0029000A);
        render_effect->duration = (uint16)(0x0029000A >> 16);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->kind = (uint16)(0x03200002);
        render_effect->radius = (uint16)(0x03200002 >> 16);
    }
    index = 0;
    duration = 800;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        result = index & 3;
        if (record == 0)
        {
            (void)(result);
            return;
        }
        record->ticks = (uint16)(0);
        record->duration = (uint16)(result + 60);
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        record->radius = (uint16)(duration);
        duration += 2;
        index += 1;
        px = x + (((random & 255) - 128) << 8);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 8);
        record->vector[0] = (uint16)(((mixed & 255) - 128) << 1);
        record->vector[1] = (uint16)(((uint32)((sint32)mixed >> 2) & 0x1FE) - 384);
        record->vector[2] = (uint16)((((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        record->flags = (uint16)(398);
        record->style = (uint16)(0x1A02);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
    } while (index < 20);
    return;
}

void effect_spawn_mixed_drift_burst(uint32 x, uint32 y, uint32 z)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 random, index, header, level_y;
    FUNCTION_MARKER(0x80080C08, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        random = game_random();
        index = 0;
        header = 0xFFFFFFFF;
        record->ticks = (uint16)(header);
        record->duration = (uint16)((random & 7) + 40);
        record->flags = (uint16)(149);
        record->vector[1] = (uint16)(0 - (uint32)512 - (random & 63));
        record->style = (uint16)(528);
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->vector[0] = (uint16)(0);
        record->vector[2] = (uint16)(0);
        record->radius = (uint16)((random & 127) + 720);
        do
        {
            record = effect_drift_allocate(&effect_drift_primary_pool);
            if (record == 0)
                break;
            index += 1;
            random = game_random();
            record->ticks = (uint16)(header);
            header -= 2;
            record->duration = (uint16)((random & 3) + 12);
            record->position[0] = (uint32)(x + (((random & 255) - 128) << 14));
            record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 14));
            record->style = (uint16)(256);
            record->radius = (uint16)(((uint32)((sint32)random >> 7) & 127) + 500);
            record->position[1] = (uint32)(y);
            record->flags = (uint16)(0);
        } while (index < 7);
    }
    render_effect = effect_render_allocate();
    if (render_effect != 0)
    {
        random = game_random();
        render_effect->duration = (uint16)((random & 3) + 24);
        render_effect->kind = (uint16)(1);
        render_effect->radius = (uint16)((random & 127) + 1200);
        render_effect->ticks = (uint16)(0);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->angles[0] = (uint16)(1024);
        render_effect->angles[1] = (uint16)(1024 >> 16);
    }
    level_y = y - 0x50000;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        index += 1;
        random = game_random();
        record->duration = (uint16)((random & 15) + 30);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 13));
        record->ticks = (uint16)(0);
        record->position[1] = (uint32)(level_y);
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 4) & 255) - 128) << 13));
        random = game_random();
        record->vector[0] = (uint16)(((random & 255) - 128) << 1);
        record->vector[1] = (uint16)(((uint32)((sint32)random >> 2) & 0x1FE) - 576);
        record->vector[2] = (uint16)((((uint32)((sint32)random >> 6) & 255) - 128) << 1);
        record->flags = (uint16)(141);
        record->style = (uint16)(0x1C04);
        record->radius = (uint16)(((uint32)((sint32)random >> 8) & 127) + 800);
    } while (index < 10);
    return;
}

void effect_spawn_scripted_drift(uint32 x, uint32 y, uint32 z, uint32 mode)
{
    EFFECT_DRIFT *record;
    uint32 random, variant, result, offset;
    FUNCTION_MARKER(0x80093D1C, "ACE3.BIN");
    if ((mode & 1) != 0)
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record != 0)
        {
            random = game_random();
            variant = random & 1;
            record->ticks = (uint16)(0xFFFF);
            random = game_random();
            record->duration = (uint16)((random & 7) + (variant != 0 ? 12 : 30));
            random = game_random();
            record->position[0] = (uint32)(x + ((random - 0x4000) << 9));
            record->position[1] = (uint32)(y);
            random = game_random();
            record->position[2] = (uint32)(z + ((random - 0x4000) << 9));
            record->flags = (uint16)(0);
            record->style = (uint16)(variant != 0 ? 256 : 528);
            random = game_random();
            result = variant != 0 ? (random & 0x1FF) + 1000 : (random & 255) + 1600;
            record->radius = (uint16)(result);
        }
    }
    random = game_random();
    if ((random & 1) != 0)
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record != 0)
        {
            record->ticks = (uint16)(0);
            random = game_random();
            record->duration = (uint16)((random & 15) + 60);
            random = game_random();
            record->position[0] = (uint32)(x + ((random - 0x4000) << 9));
            record->position[1] = (uint32)(y - 0x320000);
            random = game_random();
            record->position[2] = (uint32)(z + ((random - 0x4000) << 9));
            record->flags = (uint16)(152);
            random = game_random();
            record->vector[0] = (uint16)((random & 255) - 128);
            random = game_random();
            record->vector[2] = (uint16)((random & 255) - 128);
            record->vector[1] = (uint16)(0);
            record->style = (uint16)(0);
            random = game_random();
            record->radius = (uint16)((random & 255) + 240);
        }
    }
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    record->ticks = (uint16)(30);
    random = game_random();
    record->duration = (uint16)((random & 7) + 80);
    random = game_random();
    offset = (random - 0x4000) << 9;
    record->position[0] = (uint32)(x + (uint32)((sint32)offset >> 1));
    record->position[1] = (uint32)(y - 0xC8000);
    random = game_random();
    offset = (random - 0x4000) << 9;
    record->position[2] = (uint32)(z + (uint32)((sint32)offset >> 1));
    record->flags = (uint16)(152);
    random = game_random();
    record->vector[0] = (uint16)((random & 127) - 64);
    random = game_random();
    record->vector[2] = (uint16)((random & 127) - 64);
    record->vector[1] = (uint16)(0);
    record->style = (uint16)(0x1E04);
    random = game_random();
    result = (random & 0x3FF) + 2000;
    record->radius = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void effect_tick_scripted_flash(uint32 ticks)
{
    uint32 result, sine, ordering, packet, random, value, x, y, z, parameter, product, scaled;
    FUNCTION_MARKER(0x80093FC0, "ACE3.BIN");
    if ((sint32)ticks < 11)
        return;
    if ((sint32)ticks >= 736)
        return;
    sine = rsin(ticks << 6);
    if ((sint32)sine < 0)
        sine = 0 - sine;
    if ((sint32)ticks >= 390)
        sine = (uint32)((sint32)sine >> 1) + 2048;
    ordering = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    result = ui_draw_scripted_flash(ordering + 8, packet, 4096 - sine);
    game_runtime.mission_control.packet_cur = (uint32)(result);
    effect_light.flags = (uint32)(1);
    if ((sint32)ticks >= 391)
        return;
    random = game_random();
    if ((random & 1) != 0)
    {
        value = (uint16)game_runtime.mission_view.shift[0];
        game_runtime.mission_view.shift[0] = (uint16)(value + (((uint32)((sint32)random >> 1)) & 1));
        value = (uint16)game_runtime.mission_view.shift[1];
        game_runtime.mission_view.shift[1] = (uint16)(value + (((uint32)((sint32)random >> 3)) & 7));
    }
    if ((ticks & 1) == 0)
    {
        (void)(uint32)((sint32)ticks < 111);
        return;
    }
    if ((sint32)ticks < 111)
    {
        x = 0xFDAE4000;
        y = 0xFF6A0000;
        z = 0xFEC78000;
    }
    else if ((sint32)ticks < 201)
    {
        x = 0xFE4A8000;
        y = 0xFF63C000;
        z = 0xFF254000;
    }
    else if ((sint32)ticks < 281)
    {
        parameter = (sint32)((ticks - 220) << 12) / 50;
        scaled = parameter * 125;
        product = 0 - (scaled << 3);
        x = ((uint32)((sint32)product / 4096) - 16000) << 12;
        product = scaled << 2;
        y = ((uint32)((sint32)product / 4096) - 2500) << 12;
        product = (((((parameter << 1) + parameter) << 4) - parameter) * 8 - parameter) << 5;
        z = ((uint32)((sint32)product / 4096) + 55000) << 12;
    }
    else
    {
        parameter = (sint32)((ticks - 320) << 12) / 40;
        scaled = parameter * 125;
        product = scaled << 3;
        x = ((uint32)((sint32)product / 4096) - 18000) << 12;
        product = ((((parameter << 1) + parameter) << 3) + parameter) << 5;
        y = ((uint32)((sint32)product / 4096) - 2500) << 12;
        product = ((scaled << 3) - scaled) << 3;
        z = ((uint32)((sint32)product / 4096) + 63000) << 12;
    }
    {
        (void)(effect_spawn_scripted_drift(x, y, z, (sint32)ticks >> 1));
        return;
    }
}

void effect_emit_ground_trail(const uint32 *native_position, const uint32 *native_vector)
{
    uint32 y, vy, base_y, height, threshold, random, x, z, vx, vz, flags;
    FUNCTION_MARKER(0x80084E6C, "ACE3.BIN");
    EFFECT_EMISSION_PUBLICATION *published = (EFFECT_EMISSION_PUBLICATION *)getScratchAddr(0);
    uint32 *origin = published->origin;
    uint32 *velocity = published->velocity;
    y = (native_position[1]);
    vy = (native_vector[1]);
    base_y = game_runtime.world_settings.ground_offset;
    height = (sint32)(y - base_y) >> 12;
    threshold = height + (uint32)((sint32)vy >> 10);
    if ((sint32)threshold < 0)
    {
        (void)(height);
        return;
    }
    random = game_random() & 0xFFF;
    if ((sint32)threshold < (sint32)random)
        return;
    x = (native_position[0]);
    z = (native_position[2]);
    vx = (native_vector[0]);
    vz = (native_vector[2]);
    origin[1] = y + vy;
    velocity[1] = vy;
    origin[0] = x + (vx << 1);
    origin[2] = z + (vz << 1);
    velocity[0] = vx;
    velocity[2] = vz;
    flags = game_runtime.mission_control.flags;
    if ((flags & 2) != 0)
    {
        (void)(effect_emit_trail(8, origin, velocity));
        return;
    }
    {
        (void)(effect_emit_secondary_drift(8, origin, velocity));
        return;
    }
}

void effect_segment_trace_emit(uint32 scale, uint32 mode, const NATIVE_POSE *pose)
{
    uint32 x, y, z, rx, ry, rz, result, flags;
    uint32 start[3], end[3], hit[3];
    const uint32 *finish;
    MATRIX *rotation = (MATRIX *)getScratchAddr(0);
    FUNCTION_MARKER(0x800831D0, "ACE3.BIN");
    rotation_build(rotation, pose->angles);
    SetRotMatrix(rotation);
    xport_gte_write_data(0, 0xFFF80000);
    xport_gte_write_data(1, 55);
    xport_gte_execute(0x486012);
    x = pose->position[0];
    y = pose->position[1];
    z = pose->position[2];
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 6880);
    xport_gte_execute(0x486012);
    start[0] = x + (rx << 12);
    start[1] = y + (ry << 12);
    start[2] = z + (rz << 12);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    end[0] = x + (rx << 12);
    end[1] = y + (ry << 12);
    end[2] = z + (rz << 12);
    if (mode == 1)
    {
        xport_gte_write_data(0, 0x00300000);
        xport_gte_write_data(1, 130);
        xport_gte_execute(0x486012);
        rx = xport_gte_read_data(25);
        ry = xport_gte_read_data(26);
        rz = xport_gte_read_data(27);
        x += rx << 12;
        y += ry << 12;
        z += rz << 12;
    }
    result = collision_query_segment(start, end, hit);
    finish = (sint32)result > 0x7FFFFFFE ? end : hit;
    flags = game_runtime.mission_control.flags;
    if ((flags & 2) != 0)
        effect_packet_emit_segment(scale, mode, start, finish);
    if (mode == 1)
    {
        start[0] = x;
        start[1] = y;
        start[2] = z;
    }
    {
        effect_segment_emit(scale, mode, start, finish);
        return;
    }
}

void effect_spawn_render_flash(uint32 x, uint32 y, uint32 z, uint32 parameters)
{
    EFFECT_RENDER *render_effect;
    uint32 random, kind, duration;
    FUNCTION_MARKER(0x80087168, "ACE3.BIN");
    render_effect = effect_render_allocate();
    if (render_effect == 0)
        return;
    random = game_random();
    kind = (parameters & 255) + (random & 3);
    render_effect->duration = (uint16)(kind);
    duration = (uint32)((sint32)parameters >> 16) + (random & 255);
    render_effect->ticks = (uint16)(1);
    render_effect->position[0] = (uint32)(x);
    render_effect->position[1] = (uint32)(y);
    render_effect->position[2] = (uint32)(z);
    render_effect->kind = (uint16)(1);
    render_effect->radius = (uint16)(duration);
    render_effect->angles[0] = (uint16)(1024);
    render_effect->angles[1] = (uint16)(1024 >> 16);
    return;
}

void effect_spawn_kind_0(uint32 x, uint32 y, uint32 z)
{
    FUNCTION_MARKER(0x80087520, "ACE3.BIN");
    effect_spawn_primary_group_0(x, y, z);
    effect_spawn_secondary_group_0(x, y, z);
    {
        (void)(effect_spawn_render_flash(x, y, z, 0x05140010));
        return;
    }
}

void effect_spawn_kind_20(uint32 x, uint32 y, uint32 z)
{
    EFFECT_RENDER *render_effect;
    uint32 random, kind, duration;
    FUNCTION_MARKER(0x80087584, "ACE3.BIN");
    effect_spawn_primary_group_0(x, y, z);
    effect_spawn_secondary_group_0(x, y, z);
    render_effect = effect_render_allocate();
    if (render_effect == 0)
        return;
    random = game_random();
    kind = (random & 3) + 12;
    duration = (random & 255) + 1200;
    render_effect->ticks = (uint16)(0);
    render_effect->duration = (uint16)(kind);
    render_effect->position[0] = (uint32)(x);
    render_effect->position[1] = (uint32)(y);
    render_effect->position[2] = (uint32)(z);
    render_effect->kind = (uint16)(0);
    render_effect->radius = (uint16)(duration);
    {
        (void)(duration);
        return;
    }
}

void effect_spawn_kind_2(uint32 x, uint32 y, uint32 z)
{
    FUNCTION_MARKER(0x80087C18, "ACE3.BIN");
    effect_spawn_primary_group_2(x, y, z);
    effect_spawn_secondary_group_2(x, y, z);
    {
        (void)(effect_spawn_render_flash(x, y, z, 0x07080018));
        return;
    }
}

void effect_spawn_kind_3(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    FUNCTION_MARKER(0x800881BC, "ACE3.BIN");
    effect_spawn_primary_group_3(x, y, z);
    effect_spawn_secondary_group_3(x, y, z);
    effect_spawn_render_flash(x, y, z, 0x0BB8001E);
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x18FFFF), event->duration = (uint16)((uint32)(0x18FFFF) >> 16));
    event->callback = effect_timed_draw_fade;
    return;
}

void effect_spawn_primary_group_0(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, vertical, index, offset;
    FUNCTION_MARKER(0x8008721C, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_primary_pool);
    vertical = 0 - (uint32)100;
    if (record != 0)
    {
        random = game_random();
        {
            uint32 packed_record_word = 0x28FFFF;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        record->flags = (uint16)(134);
        record->vector[0] = (uint16)(((uint32)((sint32)random >> 2) & 127) - 64);
        record->vector[1] = (uint16)(-240);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 5) & 127) - 64);
        record->style = (uint16)(528);
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->radius = (uint16)((random & 127) + 436);
    }
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        record->ticks = (uint16)(0xFFFF);
        record->duration = (uint16)(index + (random & 1) + 18);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 11));
        record->position[1] = (uint32)(y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 11));
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11));
        random = game_random();
        record->vector[1] = (uint16)(vertical);
        vertical -= 80;
        offset = index << 6;
        index += 1;
        record->vector[0] = (uint16)((random & 255) - 128);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 7) & 255) - 128);
        record->flags = (uint16)(133);
        record->style = (uint16)(256);
        record->radius = (uint16)(offset + ((uint32)((sint32)random >> 4) & 127) + 116);
    } while (index < 5);
    return;
}

void effect_spawn_secondary_group_0(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, nibble, kind;
    FUNCTION_MARKER(0x800873E0, "ACE3.BIN");
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        random = game_random();
        low = (random & 255) - 128;
        y_offset = low << 8;
        vx = low << 2;
        low = ((uint32)((sint32)random >> 6) & 255) - 128;
        x_offset = low << 8;
        vz = low << 2;
        low = (uint32)((sint32)random >> 3) & 255;
        z_offset = (low - 128) << 8;
        vy = (low - 224) << 2;
        record->position[1] = (uint32)(y + y_offset);
        record->vector[0] = (uint16)(vx);
        record->position[0] = (uint32)(x + x_offset);
        record->vector[2] = (uint16)(vz);
        record->position[2] = (uint32)(z + z_offset);
        nibble = random & 15;
        if (index < 8)
        {
            vy -= 384;
            record->duration = (uint16)(nibble + 24);
            record->style = (uint16)(0x1604);
            kind = 139;
            record->ticks = (uint16)(0);
            record->radius = (uint16)((nibble << 2) + 600);
        }
        else
        {
            record->radius = (uint16)(nibble + 25);
            kind = 405;
            record->ticks = (uint16)(0);
            record->duration = (uint16)(nibble + 30);
            record->style = (uint16)(0);
        }
        record->flags = (uint16)(kind);
        index += 1;
        record->vector[1] = (uint16)(vy);
    } while (index < 16);
    return;
}

void effect_spawn_primary_group_2(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, mixed, vertical, level_y, index, kind, duration, px, py, pz, vy;
    FUNCTION_MARKER(0x800878B4, "ACE3.BIN");
    vertical = 0;
    level_y = y;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        record->vector[1] = (uint16)(vertical);
        vertical -= 220;
        record->position[1] = (uint32)(level_y);
        level_y -= 0x100000;
        kind = (index << 1) + (random & 7) + 40;
        duration = (index << 8) + (random & 255) + 500;
        index += 1;
        record->ticks = (uint16)(0xFFFF);
        record->duration = (uint16)(kind);
        record->vector[0] = (uint16)((random & 255) - 128);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 7) & 255) - 128);
        record->flags = (uint16)(135);
        record->style = (uint16)(528);
        record->position[0] = (uint32)(x);
        record->position[2] = (uint32)(z);
        record->radius = (uint16)(duration);
    } while (index < 3);
    vertical = 0 - (uint32)140;
    level_y = y;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        vy = vertical - 128;
        vertical -= 110;
        py = level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 13);
        level_y -= 0x50000;
        kind = index + (random & 3) + 22;
        duration = (index << 7) + ((uint32)((sint32)mixed >> 4) & 127) + 260;
        index += 1;
        px = x + (((random & 255) - 128) << 13);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 13);
        record->ticks = (uint16)(0xFFFF);
        record->vector[0] = (uint16)((mixed & 255) - 128);
        vy += (uint32)((sint32)mixed >> 3) & 255;
        record->vector[2] = (uint16)(((uint32)((sint32)mixed >> 6) & 255) - 128);
        record->flags = (uint16)(133);
        record->style = (uint16)(256);
        record->duration = (uint16)(kind);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        record->vector[1] = (uint16)(vy);
        record->radius = (uint16)(duration);
    } while (index < 8);
    return;
}

void effect_spawn_secondary_group_2(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x80087AD0, "ACE3.BIN");
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        random = game_random();
        low = (random & 255) - 128;
        y_offset = low << 8;
        vx = low << 2;
        low = ((uint32)((sint32)random >> 6) & 255) - 128;
        x_offset = low << 8;
        vz = low << 2;
        low = ((uint32)((sint32)random >> 3) & 255) - 128;
        z_offset = low << 8;
        vy = (low << 2) - 384;
        record->position[1] = (uint32)(y + y_offset);
        record->vector[0] = (uint16)(vx);
        record->position[0] = (uint32)(x + x_offset);
        record->vector[2] = (uint16)(vz);
        record->position[2] = (uint32)(z + z_offset);
        if (index < 15)
        {
            vy -= (index << 6) + 400;
            record->duration = (uint16)((random & 15) + index + 24);
            record->style = (uint16)(0x1804);
            record->radius = (uint16)((random & 255) + 700);
            kind = 141;
            record->ticks = (uint16)(0);
        }
        else
        {
            record->radius = (uint16)((random & 15) + 25);
            kind = 405;
            record->ticks = (uint16)(0);
            record->duration = (uint16)((random & 15) + 30);
            record->style = (uint16)(0);
        }
        record->flags = (uint16)(kind);
        index += 1;
        record->vector[1] = (uint16)(vy);
    } while (index < 20);
    return;
}

void effect_spawn_primary_group_3(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, vertical, level_y, index, vy, kind, duration;
    FUNCTION_MARKER(0x80087C7C, "ACE3.BIN");
    vertical = 0 - (uint32)384;
    level_y = y;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        record->ticks = (uint16)(0xFFFF);
        record->duration = (uint16)(index + 42);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 13));
        record->position[1] = (uint32)(level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 13));
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 13));
        random = game_random();
        vy = vertical - 128;
        vertical -= 100;
        level_y -= 0x100000;
        index += 1;
        record->vector[0] = (uint16)((random & 255) - 128);
        vy += (uint32)((sint32)random >> 3) & 255;
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 6) & 255) - 128);
        record->flags = (uint16)(134);
        record->style = (uint16)(528);
        record->vector[1] = (uint16)(vy);
        record->radius = (uint16)((random & 255) + 900);
    } while (index < 3);
    vertical = 0 - (uint32)340;
    level_y = y + 0x1F4000;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        kind = (index << 1) + (random & 7) + 24;
        record->ticks = (uint16)(0xFFFF);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 14));
        record->position[1] = (uint32)(level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 14));
        record->duration = (uint16)(kind);
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 14));
        random = game_random();
        record->vector[1] = (uint16)(vertical);
        vertical -= 98 + (random & 7);
        level_y -= 0x50000;
        duration = (index << 7) + ((uint32)((sint32)random >> 2) & 127) + 496;
        index += 1;
        record->vector[0] = (uint16)(((random & 255) - 128) << 1);
        record->vector[2] = (uint16)((((uint32)((sint32)random >> 7) & 255) - 128) << 1);
        record->flags = (uint16)(133);
        record->style = (uint16)(256);
        record->radius = (uint16)(duration);
    } while (index < 9);
    return;
}

void effect_spawn_secondary_group_3(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, mixed, vertical, index, kind, duration, px, py, pz, vy;
    FUNCTION_MARKER(0x80087EE0, "ACE3.BIN");
    vertical = 0 - (uint32)300;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            break;
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        vy = vertical + ((((uint32)((sint32)mixed >> 3) & 255) - 128) << 2);
        vertical -= 100;
        kind = index + 74;
        duration = (index << 5) + ((uint32)((sint32)mixed >> 1) & 255) + 1200;
        index += 1;
        px = x + (((random & 255) - 128) << 12);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        record->ticks = (uint16)(10);
        record->vector[0] = (uint16)(((mixed & 255) - 128) << 2);
        record->vector[2] = (uint16)((((uint32)((sint32)mixed >> 6) & 255) - 128) << 2);
        record->flags = (uint16)(150);
        record->style = (uint16)(0x1C04);
        record->duration = (uint16)(kind);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        record->vector[1] = (uint16)(vy);
        record->radius = (uint16)(duration);
    } while (index < 16);
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        kind = index + 50;
        duration = (index << 1) + (mixed & 31) + 40;
        index += 1;
        px = x + (((random & 255) - 128) << 12);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        record->duration = (uint16)(kind);
        record->vector[0] = (uint16)(((mixed & 255) - 128) << 1);
        record->vector[1] = (uint16)(((((uint32)((sint32)mixed >> 3) & 255) - 128) << 1) - 256);
        record->vector[2] = (uint16)((((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        record->flags = (uint16)(149);
        record->ticks = (uint16)(0);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        record->style = (uint16)(0);
        record->radius = (uint16)(duration);
    } while (index < 8);
    return;
}

void effect_spawn_secondary_group_1(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x8008762C, "ACE3.BIN");
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        random = game_random();
        vx = (random & 255) - 128;
        y_offset = vx << 8;
        vz = ((uint32)((sint32)random >> 6) & 255) - 128;
        record->vector[0] = (uint16)(vx);
        x_offset = vz << 8;
        low = (uint32)((sint32)random >> 3) & 255;
        record->vector[2] = (uint16)(vz);
        z_offset = (low - 128) << 8;
        vy = low - 272;
        record->position[1] = (uint32)(y + y_offset);
        record->position[0] = (uint32)(x + x_offset);
        record->position[2] = (uint32)(z + z_offset);
        if (index < 4)
        {
            record->duration = (uint16)((random & 15) + 26);
            record->style = (uint16)(0x1804);
            record->radius = (uint16)((random & 127) + 280);
            kind = 141;
            record->ticks = (uint16)(0);
        }
        else
        {
            record->radius = (uint16)((random & 15) + 25);
            kind = 400;
            record->ticks = (uint16)(0);
            record->duration = (uint16)((random & 15) + 30);
            record->style = (uint16)(0);
        }
        record->flags = (uint16)(kind);
        index += 1;
        record->vector[1] = (uint16)(vy);
    } while (index < 8);
    return;
}

void effect_spawn_primary_group_1(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, duration;
    FUNCTION_MARKER(0x8008775C, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record == 0)
        return;
    random = game_random();
    record->ticks = (uint16)(0xFFFF);
    record->duration = (uint16)((random & 7) + 20);
    duration = (random & 127) + 220;
    record->position[0] = (uint32)(x);
    record->position[1] = (uint32)(y);
    record->position[2] = (uint32)(z);
    record->flags = (uint16)(0);
    record->style = (uint16)(256);
    record->radius = (uint16)(duration);
    {
        (void)(duration);
        return;
    }
}

void effect_spawn_kind_1(uint32 x, uint32 y, uint32 z)
{
    EFFECT_RENDER *render_effect;
    uint32 random, duration;
    FUNCTION_MARKER(0x800877FC, "ACE3.BIN");
    effect_spawn_primary_group_1(x, y, z);
    effect_spawn_secondary_group_1(x, y, z);
    render_effect = effect_render_allocate();
    if (render_effect == 0)
        return;
    random = game_random();
    render_effect->duration = (uint16)((random & 3) + 10);
    render_effect->kind = (uint16)(1);
    duration = (random & 127) + 600;
    render_effect->ticks = (uint16)(0);
    render_effect->position[0] = (uint32)(x);
    render_effect->position[1] = (uint32)(y);
    render_effect->position[2] = (uint32)(z);
    render_effect->radius = (uint16)(duration);
    render_effect->angles[0] = (uint16)(random);
    random = game_random();
    render_effect->angles[1] = (uint16)(random);
    {
        (void)(random);
        return;
    }
}

void effect_spawn_mixed_drift_group(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, mixed, index, kind, px, py, pz;
    FUNCTION_MARKER(0x80088254, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_primary_pool);
    index = 0;
    if (record != 0)
    {
        random = game_random();
        record->ticks = (uint16)(~(random & 3));
        record->duration = (uint16)((random & 31) + 32);
        record->position[0] = (uint32)(x + (((random & 255) - 128) << 12));
        record->position[1] = (uint32)(y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12));
        record->position[2] = (uint32)(z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12));
        random = game_random();
        record->vector[0] = (uint16)((random & 127) - 64);
        record->vector[1] = (uint16)(((uint32)((sint32)random >> 3) & 127) - 320);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 6) & 127) - 64);
        record->flags = (uint16)(132);
        record->style = (uint16)(528);
        record->radius = (uint16)((random & 255) + 600);
    }
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        index += 1;
        random = game_random();
        kind = (random & 31) + 48;
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        px = x + (((random & 255) - 128) << 12);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        record->ticks = (uint16)(8);
        record->vector[0] = (uint16)((mixed & 127) - 64);
        record->vector[1] = (uint16)(((uint32)((sint32)mixed >> 3) & 127) - 560);
        record->vector[2] = (uint16)(((uint32)((sint32)mixed >> 6) & 127) - 64);
        record->flags = (uint16)(141);
        record->style = (uint16)(0x1804);
        record->radius = (uint16)((mixed & 255) + 700);
        record->duration = (uint16)(kind);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
    } while (index < 3);
    return;
}

void effect_spawn_kind_8(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    EFFECT_DRIFT *record;
    uint32 random, index;
    FUNCTION_MARKER(0x800885DC, "ACE3.BIN");
    effect_spawn_kind_20(x + 0x1E000, y + 0x17C000, z);
    index = 0;
    do
    {
        random = game_random();
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            break;
        record->duration = (uint16)((random & 15) + 40);
        record->ticks = (uint16)(0);
        record->position[0] = (uint32)((index & 1) != 0 ? x + 0x400000 : x - 0x800000);
        index += 1;
        record->vector[0] = (uint16)((random & 255) - 128);
        record->vector[1] = (uint16)(((uint32)((sint32)random >> 3) & 255) - 176);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 6) & 255) - 128);
        record->flags = (uint16)(136);
        record->style = (uint16)(0x1804);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->radius = (uint16)((random & 255) + 300);
    } while (index < 8);
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x5BFFFF), event->duration = (uint16)((uint32)(0x5BFFFF) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_0;
    return;
}

void effect_spawn_kind_13(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 level_y, level_z, index;
    FUNCTION_MARKER(0x80088968, "ACE3.BIN");
    level_y = y + 0xC8000;
    level_z = z;
    index = 0;
    do
    {
        effect_spawn_kind_20(x, level_y + 0xC8000, level_z);
        effect_spawn_primary_group_0(x - 0x708000, level_y, level_z);
        effect_spawn_primary_group_0(x + 0x708000, level_y, level_z);
        index += 1;
        level_z -= 0x5DC000;
    } while (index < 2);
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x61FFFF), event->duration = (uint16)((uint32)(0x61FFFF) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_1;
    return;
}

void effect_spawn_primary_group_12(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, mixed, vertical, level_y, index, kind, duration, px, py, pz;
    FUNCTION_MARKER(0x80088B48, "ACE3.BIN");
    vertical = 0 - (uint32)100;
    level_y = y;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        record->vector[1] = (uint16)(vertical);
        vertical -= 220;
        record->position[1] = (uint32)(level_y);
        level_y -= 0x100000;
        kind = (index << 1) + (random & 7) + 40;
        duration = (index << 8) + (random & 255) + 500;
        index += 1;
        record->ticks = (uint16)(0xFFFF);
        record->duration = (uint16)(kind);
        record->vector[0] = (uint16)((random & 255) - 128);
        record->vector[2] = (uint16)(((uint32)((sint32)random >> 7) & 255) - 128);
        record->flags = (uint16)(135);
        record->style = (uint16)(528);
        record->position[0] = (uint32)(x);
        record->position[2] = (uint32)(z);
        record->radius = (uint16)(duration);
    } while (index < 2);
    vertical = 0 - (uint32)100;
    level_y = y;
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            return;
        random = game_random();
        kind = index + (random & 3) + 24;
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        record->vector[1] = (uint16)(vertical);
        vertical -= 110 + (mixed & 7);
        py = level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        level_y -= 0x5A000;
        duration = (index << 7) + ((uint32)((sint32)mixed >> 4) & 127) + 240;
        index += 1;
        px = x + (((random & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        record->ticks = (uint16)(0xFFFF);
        record->vector[0] = (uint16)((mixed & 255) - 128);
        record->vector[2] = (uint16)(((uint32)((sint32)mixed >> 7) & 255) - 128);
        record->flags = (uint16)(133);
        record->style = (uint16)(256);
        record->duration = (uint16)(kind);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        record->radius = (uint16)(duration);
    } while (index < 5);
    return;
}

void effect_spawn_secondary_group_12(uint32 x, uint32 y, uint32 z)
{
    EFFECT_DRIFT *record;
    uint32 random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x80088D60, "ACE3.BIN");
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        random = game_random();
        low = (random & 255) - 128;
        y_offset = low << 8;
        vx = low << 2;
        low = ((uint32)((sint32)random >> 6) & 255) - 128;
        x_offset = low << 8;
        vz = low << 2;
        low = ((uint32)((sint32)random >> 3) & 255) - 128;
        z_offset = low << 8;
        vy = (low << 2) - 384;
        record->position[1] = (uint32)(y + y_offset);
        record->vector[0] = (uint16)(vx);
        record->position[0] = (uint32)(x + x_offset);
        record->vector[2] = (uint16)(vz);
        record->position[2] = (uint32)(z + z_offset);
        if (index < 12)
        {
            vy -= index << 5;
            record->ticks = (uint16)(8);
            record->duration = (uint16)((random & 15) + index + 44);
            record->style = (uint16)(0x1804);
            record->radius = (uint16)((random & 255) + 600);
            kind = 141;
        }
        else
        {
            record->radius = (uint16)((random & 15) + 25);
            kind = 405;
            record->ticks = (uint16)(0);
            record->duration = (uint16)((random & 15) + 36);
            record->style = (uint16)(0);
        }
        record->flags = (uint16)(kind);
        index += 1;
        record->vector[1] = (uint16)(vy);
    } while (index < 18);
    return;
}

void effect_spawn_kind_12(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80088EA8, "ACE3.BIN");
    effect_spawn_primary_group_12(x, y, z);
    effect_spawn_secondary_group_12(x, y, z);
    effect_spawn_render_flash(x, y, z, 0x07080018);
    event = effect_timed_allocate();
    if (event != 0)
    {
        event->position[0] = (uint32)(x + 0x384000);
        (event->ticks = (uint16)(0x79FFFF), event->duration = (uint16)((uint32)(0x79FFFF) >> 16));
        event->position[1] = (uint32)(y + 0x3E8000);
        event->position[2] = (uint32)(z);
        event->callback = effect_timed_emit_2;
    }
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x03840001), event->duration = (uint16)((uint32)(0x03840001) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_14(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80088FBC, "ACE3.BIN");
    effect_spawn_primary_group_0(x, y, z);
    effect_spawn_secondary_group_0(x, y, z);
    effect_spawn_render_flash(x, y, z, 0x07080018);
    event = effect_timed_allocate();
    if (event != 0)
    {
        event->position[0] = (uint32)(x - 0x258000);
        event->position[2] = (uint32)(z + 0x190000);
        (event->ticks = (uint16)(0x8DFFFF), event->duration = (uint16)((uint32)(0x8DFFFF) >> 16));
        event->position[1] = (uint32)(y + 0x3E8000);
        event->callback = effect_timed_emit_2;
    }
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    event->duration = (uint16)(3600);
    event->ticks = (uint16)(0);
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_15(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x800890D8, "ACE3.BIN");
    effect_spawn_primary_group_0(x, y, z);
    effect_spawn_secondary_group_0(x, y, z);
    effect_spawn_render_flash(x, y, z, 0x07080018);
    event = effect_timed_allocate();
    if (event != 0)
    {
        event->position[1] = (uint32)(y + 0x1EA000);
        (event->ticks = (uint16)(0x97FFFF), event->duration = (uint16)((uint32)(0x97FFFF) >> 16));
        event->position[0] = (uint32)(x);
        event->position[2] = (uint32)(z - 0x258000);
        event->callback = effect_timed_emit_2;
    }
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    event->duration = (uint16)(3600);
    event->ticks = (uint16)(0);
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_16(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    FUNCTION_MARKER(0x800892FC, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x5DFFFF), event->duration = (uint16)((uint32)(0x5DFFFF) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_3;
    return;
}

void effect_spawn_kind_17(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    FUNCTION_MARKER(0x80089488, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x5DFFFF), event->duration = (uint16)((uint32)(0x5DFFFF) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_4;
    return;
}

void effect_spawn_kind_18(uint32 x, uint32 y, uint32 z)
{
    EFFECT_RENDER *render_effect;
    EFFECT_DRIFT *record;
    uint32 random, mixed, index, header, kind, px, py, pz, duration;
    FUNCTION_MARKER(0x80089504, "ACE3.BIN");
    index = 0;
    do
    {
        record = effect_drift_allocate(&effect_drift_primary_pool);
        if (record == 0)
            break;
        random = game_random();
        kind = (random & 7) + 24;
        mixed = (random << 8) | (uint32)((sint32)random >> 7);
        header = ~index;
        index += 1;
        px = x + (((random & 255) - 128) << 11);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 11);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11);
        record->position[0] = (uint32)(px);
        record->ticks = (uint16)(header);
        record->vector[0] = (uint16)(((mixed & 255) - 128) << 1);
        record->vector[1] = (uint16)((((uint32)((sint32)mixed >> 3) & 255) - 128) << 1);
        record->vector[2] = (uint16)((((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        record->flags = (uint16)(133);
        record->style = (uint16)(256);
        record->duration = (uint16)(kind);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        record->radius = (uint16)((mixed & 255) + 300);
    } while (index < 6);
    render_effect = effect_render_allocate();
    index = 0;
    if (render_effect != 0)
    {
        random = game_random();
        render_effect->ticks = (uint16)(0);
        render_effect->duration = (uint16)((random & 3) + 20);
        render_effect->position[0] = (uint32)(x);
        render_effect->position[1] = (uint32)(y);
        render_effect->position[2] = (uint32)(z);
        render_effect->kind = (uint16)(0);
        render_effect->radius = (uint16)((random & 255) + 1600);
    }
    duration = 580;
    do
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        kind = index + 50;
        if (record == 0)
        {
            (void)(kind);
            return;
        }
        record->ticks = (uint16)(0);
        record->duration = (uint16)(kind);
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        px = x + (((random & 255) - 128) << 11);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 11);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11);
        record->vector[0] = (uint16)((mixed & 255) - 128);
        record->vector[1] = (uint16)(((uint32)((sint32)mixed >> 3) & 255) - 128);
        record->vector[2] = (uint16)(((uint32)((sint32)mixed >> 6) & 255) - 128);
        record->position[0] = (uint32)(px);
        record->position[1] = (uint32)(py);
        record->position[2] = (uint32)(pz);
        if ((index & 1) != 0)
        {
            record->flags = (uint16)(135);
            record->style = (uint16)(0x1804);
            record->radius = (uint16)(duration);
        }
        else
        {
            record->flags = (uint16)(384);
            record->style = (uint16)(0);
            record->radius = (uint16)(index + 30);
        }
        index += 1;
        duration += 32;
    } while (index < 24);
    return;
}

void effect_spawn_kind_9(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x8008978C, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    event = effect_timed_allocate();
    if (event != 0)
    {
        event->position[0] = (uint32)(x + 0xC8000);
        event->position[2] = (uint32)(z - 0x190000);
        (event->ticks = (uint16)(0x5BFFFF), event->duration = (uint16)((uint32)(0x5BFFFF) >> 16));
        event->position[1] = (uint32)(y + 0xB4000);
        event->callback = effect_timed_emit_2;
    }
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x012C0001), event->duration = (uint16)((uint32)(0x012C0001) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_11(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80089880, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    event = effect_timed_allocate();
    if (event != 0)
    {
        event->position[0] = (uint32)(x + 0xFA000);
        event->position[2] = (uint32)(z - 0x190000);
        (event->ticks = (uint16)(0x59FFFF), event->duration = (uint16)((uint32)(0x59FFFF) >> 16));
        event->position[1] = (uint32)(y + 0x226000);
        event->callback = effect_timed_emit_2;
    }
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x03840001), event->duration = (uint16)((uint32)(0x03840001) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_4(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80089974, "ACE3.BIN");
    effect_spawn_kind_1(x, y, z);
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x03840000), event->duration = (uint16)((uint32)(0x03840000) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_5(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80089A04, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x03840000), event->duration = (uint16)((uint32)(0x03840000) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_6;
    return;
}

void effect_spawn_kind_6(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80089A94, "ACE3.BIN");
    effect_spawn_kind_0(x, y, z);
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x23280000), event->duration = (uint16)((uint32)(0x23280000) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_5;
    return;
}

void effect_spawn_kind_7(uint32 x, uint32 y, uint32 z)
{
    EFFECT_TIMED_EVENT *event;
    uint32 result;
    FUNCTION_MARKER(0x80089B24, "ACE3.BIN");
    effect_spawn_kind_2(x, y, z);
    result = game_runtime.mission_control.flags & 2;
    if (result == 0)
    {
        (void)(result);
        return;
    }
    event = effect_timed_allocate();
    if (event == 0)
        return;
    (event->ticks = (uint16)(0x15180000), event->duration = (uint16)((uint32)(0x15180000) >> 16));
    event->position[0] = (uint32)(x);
    event->position[1] = (uint32)(y);
    event->position[2] = (uint32)(z);
    event->callback = effect_timed_emit_6;
    return;
}

uint32 effect_draw_scene_frame(uint32 ot, uint32 packet)
{
    EFFECT_FRAME_CONTEXT frame;
    SCENE_DETAIL_CALLBACK callback;
    EFFECT_LIGHT *state;
    uint32 value, flags, distance, color, mission, selector;
    sint32 count;
    FUNCTION_MARKER(0x80085204, "ACE3.BIN");
    frame.ot = ot;
    frame.packet = packet;
    value = game_runtime.camera_view.pose.position[0];
    frame.camera.word[0] = value;
    value = game_runtime.camera_view.pose.position[1];
    frame.camera.word[1] = value;
    value = game_runtime.camera_view.pose.position[2];
    frame.camera.word[2] = value;
    value = game_runtime.camera_view.pose.angles[0];
    frame.angles.value[0] = (uint16)(value);
    value = game_runtime.camera_view.pose.angles[1];
    frame.angles.value[1] = (uint16)(value);
    value = game_runtime.camera_view.pose.angles[2];
    frame.angles.value[2] = (uint16)(value);
    value = game_runtime.camera_proj.distance;
    frame.projection = (uint16)(value);
    flags = game_runtime.camera_control.mode & 0x870;
    if (flags == 0)
    {
        flags = game_runtime.flags & 0x20000000;
    }
    if (flags != 0)
    {
        value = effect_light.flags;
        effect_light.flags = (uint32)(value | 1);
    }
    count = game_runtime.render_settings.flare_flags;
    if (count != 0)
    {
        state = &effect_light;
        distance = game_runtime.world_settings.ground_offset;
        effect_light_draw_flare(state, count, 0 - distance, &frame);
        color = game_runtime.render_settings.halo_color;
        if (color != 0)
        {
            mission = (sint16)game_runtime.selection.aircraft;
            if (mission == 25)
            {
                flags = game_runtime.camera_control.mode;
                if (flags == 2)
                {
                    value = effect_light.flags;
                    effect_light.flags = (uint32)(value | 4);
                }
                else
                    effect_light_draw_halo(state, color, &frame);
            }
            else
                effect_light_draw_halo(state, color, &frame);
        }
    }
    value = game_runtime.render_settings.layer_enabled;
    if (value != 0)
        effect_layer_draw(&frame);
    selector = game_runtime.render_settings.detail_kind;
    if (selector > 0 && selector < 6)
    {
        callback = scene_geometry_detail_callbacks[selector - 1];
        /* frame is borrowed only during this detail pass */
        callback(&frame);
    }
    effect_drift_project(&frame);
    effect_drift_draw(&frame);
    effect_render_draw(&frame);
    effect_aircraft_draw(&frame);
    effect_segment_draw(&frame);
    value = game_runtime.playback.demo_mode;
    if (value == 0)
    {
        value = game_runtime.camera_control.mode & 0x200;
    }
    if (value != 0)
        effect_update_screen_overlays(&frame);
    return frame.packet;
}
