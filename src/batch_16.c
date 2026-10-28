#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8004e80c(void)
{
    uint32 selection, kind, index, table, object, flags, health, descriptors, descriptor, mode;
    FUNCTION_MARKER(0x8004E80C, "ACE3.BIN");
    selection = r_u32(0x800BCEA8);
    kind = r_u8(0x800B1FC4 + (selection << 2));
    if (kind - 5 >= 2)
        return 0;
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (index << 2) + 128);
        flags = r_u32(object);
        if ((flags & 0x4000) == 0)
            continue;
        health = r_s16(object + 78);
        if ((sint32)health <= 0)
            continue;
        descriptors = r_u32(0x800BCEFC);
        descriptor = r_u32(descriptors + index * 40 + 8);
        mode = r_u8(descriptor + 29);
        if (mode == 17 || mode == 16)
            sub_8004e900(index);
    }
    return 0;
}

uint32 sub_8004fba4(uint32 index)
{
    uint32 controls, state, phase, flags;
    FUNCTION_MARKER(0x8004FBA4, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        w_u8(state + 62, 1);
        w_u8(state + 99, 31);
        return 31;
    }
    flags = r_u32(state);
    if ((flags & 0x400000) != 0)
        w_u8(state + 61, 255);
    return 255;
}

uint32 sub_8004fcf0(uint32 index)
{
    uint32 controls, state, phase;
    FUNCTION_MARKER(0x8004FCF0, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        w_u8(state + 62, 1);
        w_u8(state + 99, 25);
    }
    w_u8(state + 97, 9);
    return 9;
}

uint32 sub_8004fd38(uint32 index)
{
    uint32 controls, state, table, phase, object, value;
    FUNCTION_MARKER(0x8004FD38, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    table = r_u32(0x800BCE5C);
    phase = r_u8(state + 62);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        w_u8(state + 62, 1);
        value = r_u32(object + 12);
        w_u32(object + 12, value - 0x01000000);
        w_u8(state + 99, 26);
    }
    w_u8(state + 97, 9);
    return 9;
}

uint32 sub_8004fd9c(uint32 index)
{
    uint32 controls, state, table, phase, object, selected, base, other, value, random, high, quotient, entry, root, script, result;
    sint64 product;
    FUNCTION_MARKER(0x8004FD9C, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    table = r_u32(0x800BCE5C);
    phase = r_u8(state + 62);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        base = 0;
        selected = sub_80048a80();
        if (selected != 0xFFFFFFFF)
        {
            table = r_u32(0x800BCE5C);
            other = r_u32(table + (selected << 2));
            value = r_u32(other + 8);
            if ((sint32)value < 0)
                base = 3;
        }
        random = game_random();
        product = (sint64)(sint32)random * 0x55555556;
        high = (uint32)(product >> 32);
        quotient = high - (uint32)((sint32)random >> 31);
        entry = base + random - quotient * 3;
        value = r_u32(0x800B2048 + (entry << 2));
        w_u32(object + 8, value);
        value = r_u32(0x800B2060 + (entry << 2));
        w_u16(object + 24, 4096);
        w_u16(object + 26, 0x2000);
        w_u16(object + 28, 0);
        w_u32(object + 16, value);
        w_u8(state + 62, 1);
    }
    phase = r_u8(state + 62);
    if (phase == 1)
    {
        w_u8(state + 97, 9);
        w_u8(state + 99, 26);
        value = r_u32(object + 12);
        if ((sint32)value < (sint32)0xFEC00000)
            w_u8(state + 62, 2);
        phase = r_u8(state + 62);
    }
    if (phase == 2)
    {
        w_u8(state + 97, 0);
        w_u8(state + 99, 0);
        value = r_u32(object + 16);
        if ((sint32)value < (sint32)0xE0000000)
        {
            w_u8(state + 62, 3);
            value = r_u32(object + 8);
            w_u32(state + 108, 0xE7000000);
            w_u32(state + 112, 0xC7000000);
            w_u32(state + 104, value);
        }
    }
    phase = r_u8(state + 62);
    if (phase != 3)
        return 3;
    w_u8(state + 97, 0);
    w_u8(state + 99, 5);
    value = r_u32(object + 12);
    result = (sint32)value < (sint32)0xE7000000;
    if (result != 0)
    {
        w_u8(state + 62, 4);
        root = r_u32(0x800BCEAC);
        script = r_u32(root + 328);
        result = sub_80054ebc(script);
    }
    return result;
}

uint32 sub_8004ff88(uint32 index)
{
    uint32 controls, state, table, selected, object, phase, target, first, second, third, fourth, distance, scene, ticks, angle, oscillation, sine, cosine, x_product, z_product, value, root, script, result;
    FUNCTION_MARKER(0x8004FF88, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    table = r_u32(0x800BCE5C);
    selected = r_u8(state + 63);
    object = r_u32(table + (index << 2));
    phase = r_u8(state + 62);
    target = r_u32(table + (selected << 2));
    if (phase == 0)
    {
        w_u8(state + 62, 1);
        w_u8(state + 99, 4);
        w_u8(state + 97, 0);
        phase = r_u8(state + 62);
    }
    if (phase != 1)
        return 1;
    first = r_u32(target + 8);
    second = r_u32(target + 12);
    third = r_u32(target + 16);
    fourth = r_u32(target + 20);
    w_u32(state + 104, first);
    w_u32(state + 108, second);
    w_u32(state + 112, third);
    w_u32(state + 116, fourth);
    distance = r_u32(state + 136);
    if ((sint32)distance >= 8193)
    {
        scene = r_u32(0x800BCE54);
        ticks = r_u32(scene + 12);
        angle = r_u16(object + 26);
        oscillation = r_s16(0x800104AC + ((ticks << 8) & 0x7F00));
        sine = r_s16(0x800104AC + ((angle & 0x3FFF) << 1));
        x_product = sine * oscillation;
        angle = r_u16(object + 26);
        cosine = r_s16(0x800104AC + (((angle & 0x3FFF) + 4096) << 1));
        z_product = cosine * oscillation;
        value = r_u32(object + 8);
        w_u32(object + 8, value + (uint32)((sint32)x_product >> 14));
        value = r_u32(object + 16);
        w_u32(object + 16, value + (uint32)((sint32)z_product >> 14));
    }
    distance = r_u32(state + 136);
    result = (sint32)distance < 512;
    if (result != 0)
    {
        root = r_u32(0x800BCEAC);
        script = r_u32(root + 328);
        result = sub_80054ebc(script);
    }
    return result;
}

uint32 sub_800500ec(uint32 index)
{
    uint32 controls, state, phase;
    FUNCTION_MARKER(0x800500EC, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase != 0)
        return 27;
    w_u8(state + 99, 27);
    w_u8(state + 97, 9);
    w_u8(state + 62, 1);
    return 1;
}

uint32 sub_80050134(uint32 index)
{
    uint32 controls, state, phase;
    FUNCTION_MARKER(0x80050134, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        w_u16(state + 128, 0);
        w_u16(state + 130, 0);
        w_u16(state + 132, 0);
        w_u8(state + 62, 1);
    }
    w_u8(state + 97, 8);
    w_u8(state + 99, 0);
    return 8;
}

uint32 sub_80050184(uint32 index)
{
    uint32 controls, state, phase, game, mission, ticks, flags, descriptors, signed_ticks, result;
    FUNCTION_MARKER(0x80050184, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        w_u8(state + 99, 50);
        if (mission == 7)
            ticks = 150;
        else if (mission == 31)
            ticks = 360;
        else if (mission == 41)
            ticks = index == 2 ? 240 : 420;
        else
            ticks = mission == 50 ? 210 : 40;
        w_u16(state + 64, ticks);
        w_u8(state + 62, 1);
    }
    else if (phase == 1)
    {
        ticks = r_s16(state + 64);
        if (ticks == 0)
            w_u8(state + 62, 2);
    }
    else if (phase == 2)
    {
        w_u8(state + 99, 52);
        game = r_u32(0x800BCE60);
        w_u32(state + 108, 0);
        w_u32(state + 104, 0);
        mission = r_s16(game + 908);
        w_u32(state + 112, mission == 31 ? 0x200000 : 0x800000);
        w_u8(state + 62, 3);
    }
    else
    {
        flags = r_u32(state);
        if ((flags & 0x400000) != 0)
        {
            w_u8(state + 61, 255);
            descriptors = r_u32(0x800BCEF8);
            w_u32(descriptors + (index << 6) + 36, 0xFFFFFFFF);
        }
    }
    w_u8(state + 97, 9);
    signed_ticks = r_s16(state + 64);
    ticks = r_u16(state + 64);
    result = ticks - 1;
    if ((sint32)signed_ticks > 0)
        w_u16(state + 64, result);
    return result;
}

uint32 sub_800502d4(uint32 index)
{
    uint32 controls, state, phase, scene, flags, value, object, distance, controller, counter, game, mission, kind;
    FUNCTION_MARKER(0x800502D4, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        w_u8(state + 99, 0);
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene + 4);
        if ((flags & 0x800) != 0)
        {
            value = r_u16(state + 64);
            w_u16(state + 64, value + 1);
        }
        value = r_s16(state + 64);
        if ((sint32)value >= 151)
            w_u8(state + 62, 1);
    }
    else if (phase == 1)
    {
        w_u8(state + 99, 12);
        w_u16(state + 124, 2048);
        w_u16(state + 120, 1024);
        w_u16(state + 122, 0);
        w_u8(state + 62, 2);
    }
    else if (phase == 2)
    {
        flags = r_u32(state);
        if ((flags & 0x400000) != 0)
            w_u8(state + 62, 3);
    }
    else if (phase == 3)
        w_u8(state + 99, 0);
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 4);
    if ((flags & 0x800) != 0)
    {
        object = r_u32(0x800C6ED0 + (index << 2));
        distance = r_u32(object + 4);
        value = (sint32)distance < 4097;
        if (value == 0)
        {
            flags = r_u32(object);
            value = flags & 2;
        }
        if (value != 0)
        {
            controller = r_u32(0x800BCEB4);
            counter = r_u16(controller + 8);
            if ((counter & 15) == 0)
            {
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                kind = mission == 7 ? 7 : mission == 44 ? 8 : 6;
                sub_8004291c(kind);
            }
            controller = r_u32(0x800BCEB4);
            w_u8(controller + 10, 1);
            controller = r_u32(0x800BCEB4);
            counter = r_u16(controller + 8);
            w_u16(controller + 8, counter + 1);
        }
        else
        {
            controller = r_u32(0x800BCEB4);
            w_u8(controller + 10, 0);
            controller = r_u32(0x800BCEB4);
            counter = r_u16(controller + 8);
            w_u16(controller + 8, counter - 1);
        }
    }
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 4);
    if ((flags & 0x2000) != 0)
    {
        controller = r_u32(0x800BCEB4);
        value = r_u8(controller + 47);
        if (value == 1)
        {
            value = r_u32(scene + 12);
            if ((value & 15) == 0)
            {
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                kind = mission == 7 ? 15 : mission == 44 ? 16 : 14;
                sub_8004291c(kind);
            }
        }
    }
    w_u8(state + 97, 8);
    return 8;
}

uint32 sub_80050534(uint32 index)
{
    uint32 controls, state, phase, flags;
    FUNCTION_MARKER(0x80050534, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 62);
    if (phase == 0)
    {
        w_u8(state + 99, 0);
        w_u8(state + 62, 1);
    }
    else if (phase == 1)
    {
        w_u8(state + 99, 12);
        w_u16(state + 124, 0xF800);
        w_u16(state + 120, 2048);
        w_u16(state + 122, 0);
        w_u8(state + 62, 2);
    }
    else if (phase == 2)
    {
        flags = r_u32(state);
        if ((flags & 0x400000) != 0)
            w_u8(state + 62, 3);
    }
    else if (phase == 3)
        w_u8(state + 99, 0);
    w_u8(state + 97, 8);
    return 8;
}

uint32 sub_80050bb4(uint32 index)
{
    uint32 controls, state, selected, table, object, health, other, slot, flags, count, descriptors, descriptor, pointer, limit, mode, progress, first, second, third, fourth, toggle, scene, ticks, reset_count;
    FUNCTION_MARKER(0x80050BB4, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    selected = r_u8(state + 31);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + (selected << 2));
    health = r_s16(object + 78);
    if ((sint32)health <= 0)
    {
        w_u8(state + 28, 255);
        return 0;
    }
    other = r_u8(state + 31);
    w_u8(state + 63, selected);
    w_u8(state + 96, other);
    slot = 0x800C6FD0 + (index << 2);
    flags = r_u32(slot);
    w_u32(slot, flags | 0x10000);
    count = r_u32(state + 56);
    if (count == 0)
    {
        reset_count = 0;
        goto initialize_target;
    }
    descriptors = r_u32(0x800BCEF8);
    descriptor = descriptors + (index << 6);
    pointer = r_u32(descriptor + 24);
    limit = r_s16(pointer + 20);
    if (count == limit)
    {
        w_u8(state + 61, 0);
        table = r_u32(0x800BCE5C);
        object = r_u32(table);
        first = r_u32(object + 8);
        second = r_u32(object + 12);
        third = r_u32(object + 16);
        fourth = r_u32(object + 20);
        w_u32(state + 68, first);
        w_u32(state + 72, second);
        w_u32(state + 76, third);
        w_u32(state + 80, fourth);
        goto advance_count;
    }
    mode = r_u8(state + 60);
    if (mode == 1)
    {
        pointer = r_u32(descriptor + 28);
        progress = r_u32(state + 160);
        limit = r_u32(pointer + 8);
        if (progress < limit)
            w_u8(state + 61, 4);
        goto advance_count;
    }
    if (mode == 4)
    {
        pointer = r_u32(descriptor + 28);
        progress = r_u32(state + 160);
        limit = r_u32(pointer + 8);
        if (limit < progress)
            w_u8(state + 61, 1);
        goto advance_count;
    }
    if (mode == 0)
        return 1;
    reset_count = 1;
initialize_target:
    selected = r_u8(state + 31);
    toggle = r_u8(state + 29);
    w_u8(state + 61, 1);
    w_u32(state + 68, 0);
    w_u32(state + 72, 0);
    w_u8(state + 63, selected);
    if (toggle == 0)
    {
        w_u8(state + 29, 1);
        scene = r_u32(0x800BCE54);
        ticks = r_u32(scene + 12);
        ticks <<= 13;
    }
    else
    {
        w_u8(state + 29, 0);
        scene = r_u32(0x800BCE54);
        ticks = r_u32(scene + 12);
        ticks = ((ticks << 1) - 20000) << 12;
    }
    w_u32(state + 76, ticks);
    if (reset_count != 0)
    {
        w_u32(state + 56, 0);
        return 1;
    }
advance_count:
    count = r_u32(state + 56);
    w_u32(state + 56, count + 1);
    return 1;
}

uint32 sub_80050fa8(uint32 state, uint32 command)
{
    uint32 kind, first, second, third, fourth;
    FUNCTION_MARKER(0x80050FA8, "ACE3.BIN");
    kind = r_u8(command + 1);
    w_u8(state + 99, kind);
    first = r_u32(command + 8);
    second = r_u32(command + 12);
    third = r_u32(command + 16);
    fourth = r_u32(command + 20);
    w_u32(state + 104, first);
    w_u32(state + 108, second);
    w_u32(state + 112, third);
    w_u32(state + 116, fourth);
    first = r_u32(command + 24);
    second = r_u32(command + 28);
    w_u32(state + 120, first);
    w_u32(state + 124, second);
    kind = r_u8(state + 99);
    if (kind - 50 < 2)
    {
        w_u8(state + 97, 9);
        return 9;
    }
    w_u8(state + 97, 0);
    return 0;
}

uint32 sub_80050de8(uint32 index)
{
    uint32 controls, state, descriptors, descriptor, type, base, phase, count, table, object, kind, first_object, health, flags, command, condition, value, limit, advance;
    FUNCTION_MARKER(0x80050DE8, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    descriptors = r_u32(0x800BCEF8);
    descriptor = r_u32(descriptors + (index << 6) + 48);
    type = r_u8(descriptor + 2);
    phase = r_u8(state + 29);
    base = r_u32(0x800C7110 + (type << 2));
    if (phase == 0)
    {
        w_u32(state + 56, 0);
        count = r_u32(state + 56);
        w_u8(state + 29, 1);
        sub_80050fa8(state, base + (count << 5));
        return 1;
    }
    table = r_u32(0x800BCE5C);
    object = r_u32(table + (index << 2));
    kind = r_s16(object + 72);
    first_object = r_u32(table);
    if (kind == 44)
    {
        health = r_s16(first_object + 78);
        if ((sint32)health < 0)
        {
            w_u8(state + 99, 50);
            w_u8(state + 97, 9);
            return 0;
        }
    }
    count = r_u32(state + 56);
    flags = r_u32(state);
    command = base + (count << 5);
    advance = flags & 0x400000;
    if (advance == 0)
    {
        condition = r_u8(command + 3);
        if (condition == 1)
        {
            value = r_u32(state + 136);
            limit = r_u32(command + 4);
            if ((sint32)value < (sint32)limit)
                w_u32(state, flags | 0x400000);
        }
        else if (condition == 2)
        {
            value = r_s16(state + 102);
            limit = r_u32(command + 4);
            if ((sint32)value >= (sint32)limit)
                w_u32(state, flags | 0x400000);
        }
        flags = r_u32(state);
        advance = flags & 0x400000;
    }
    if (advance != 0)
    {
        count = r_u32(state + 56) + 1;
        w_u32(state + 56, count);
        sub_80050fa8(state, base + (count << 5));
        kind = r_u8(state + 99);
        if (kind == 255)
        {
            w_u8(state + 28, kind);
            return 0;
        }
        flags = r_u32(state);
        w_u32(state, flags & 0xFFBFFFFF);
    }
    w_u8(state + 61, 13);
    return 1;
}

uint32 sub_80051010(uint32 index)
{
    uint32 controls, state, table, game, mission, object, flags, count, descriptors, descriptor, type, base, current, next, fraction, end, start, product, position, first, second, third, fourth, pitch, yaw, roll, next_pitch, next_yaw, next_roll, start_pitch, start_yaw, start_roll, value;
    sint32 segment;
    FUNCTION_MARKER(0x80051010, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    table = r_u32(0x800BCE5C);
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    object = r_u32(table + (index << 2));
    if (mission == 3)
    {
        flags = r_u8(object + 5);
        w_u8(object + 5, flags | 8);
    }
    count = r_u32(state + 56);
    descriptors = r_u32(0x800BCEF8);
    descriptor = r_u32(descriptors + (index << 6) + 48);
    type = r_u8(descriptor + 2);
    base = r_u32(0x800C6E90 + (type << 2));
    segment = (sint32)count / 8;
    fraction = count - ((uint32)segment << 3);
    current = base + (uint32)segment * 20;
    next = current + 20;
    end = r_u32(next);
    if (end == 0xFFFFFFFF)
    {
        w_u8(state + 28, 255);
        return 0;
    }
    first = r_u32(object + 8);
    second = r_u32(object + 12);
    third = r_u32(object + 16);
    fourth = r_u32(object + 20);
    (void)first;
    (void)second;
    (void)third;
    (void)fourth;
    end = r_u32(next);
    start = r_u32(current);
    product = (end - start) * fraction;
    position = r_u32(object + 8);
    w_u32(object + 56, (uint32)((sint32)product / 8) + start - position);
    end = r_u32(next + 4);
    start = r_u32(current + 4);
    product = (end - start) * fraction;
    position = r_u32(object + 12);
    w_u32(object + 60, (uint32)((sint32)product / 8) + start - position);
    end = r_u32(next + 8);
    start = r_u32(current + 8);
    product = (end - start) * fraction;
    position = r_u32(object + 16);
    w_u32(object + 64, (uint32)((sint32)product / 8) + start - position);
    next_pitch = r_s16(next + 12);
    start_pitch = r_s16(current + 12);
    next_yaw = r_s16(next + 14);
    next_roll = r_s16(next + 16);
    pitch = next_pitch - start_pitch;
    start_yaw = r_s16(current + 14);
    start_roll = r_s16(current + 16);
    yaw = next_yaw - start_yaw;
    roll = next_roll - start_roll;
    if (((yaw & 0x3FFF) - 0x1C01) < 0x7FF)
    {
        if ((sint32)yaw > 0)
            yaw -= 0x2000;
        else
            yaw += 0x2000;
        start_pitch = r_s16(current + 12);
        next_pitch = r_s16(next + 12);
        start_roll = r_s16(current + 16);
        next_roll = r_s16(next + 16);
        pitch = 0x2000 - start_pitch - next_pitch;
        roll = next_roll + start_roll - 0x2000;
    }
    if ((sint32)pitch >= 0x2000)
        pitch -= 0x4000;
    if ((sint32)pitch < -8192)
        pitch += 0x4000;
    if ((sint32)yaw >= 0x2000)
        yaw -= 0x4000;
    if ((sint32)yaw < -8192)
        yaw += 0x4000;
    if ((sint32)roll >= 0x2000)
        roll -= 0x4000;
    if ((sint32)roll < -8192)
        roll += 0x4000;
    product = pitch * fraction;
    start = r_u16(current + 12);
    value = start + (uint32)((sint32)product / 8);
    product = yaw * fraction;
    w_u16(object + 24, value);
    start = r_u16(current + 14);
    value = start + (uint32)((sint32)product / 8);
    product = roll * fraction;
    w_u16(object + 26, value);
    start = r_u16(current + 16);
    w_u16(object + 28, start + (uint32)((sint32)product / 8));
    w_u8(state + 61, 13);
    w_u8(state + 99, 8);
    count = r_u32(state + 56);
    w_u8(state + 97, 9);
    w_u32(state + 56, count + 1);
    return 1;
}

uint32 sub_80051304(uint32 index)
{
    uint32 descriptors, descriptor, controls, state, kind, game, mission, selected, mode;
    FUNCTION_MARKER(0x80051304, "ACE3.BIN");
    descriptors = r_u32(0x800BCEF8);
    controls = r_u32(0x800BCEC0);
    descriptor = r_u32(descriptors + (index << 6) + 20);
    state = controls + index * 184;
    kind = r_u32(descriptor);
    if (kind == 7)
    {
        w_u8(state + 61, 14);
        return 14;
    }
    if (kind == 8)
    {
        w_u8(state + 61, 15);
        return 15;
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (kind == 6)
    {
        if (mission == 28)
            mode = 16;
        else if (mission == 36)
            mode = 17;
        else
            return 17;
    }
    else
    {
        if (mission != 6 && mission != 55)
            return 55;
        mode = 18;
    }
    selected = r_u8(state + 31);
    w_u8(state + 61, mode);
    w_u8(state + 63, selected);
    return mode;
}

uint32 sub_800513d8(uint32 index)
{
    uint32 controls, state, mode;
    FUNCTION_MARKER(0x800513D8, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    mode = r_u8(state + 61);
    if (mode == 255)
    {
        w_u8(state + 28, mode);
        return 0;
    }
    return 255;
}

uint32 sub_80051ba0(uint32 index)
{
    uint32 table, descriptors, data, object, member, flags, frame, elapsed, divisor, numerator, quotient, increment, next_frame, limit, count, kind, slot, value;
    FUNCTION_MARKER(0x80051BA0, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    descriptors = r_u32(0x800BCEFC);
    data = r_u32(descriptors + index * 40 + 24);
    object = r_u32(table + (index << 2));
    member = index + 32;
    if (data == 0)
    {
        value = r_u8(data + 2);
        if (value == 0)
            return 0;
    }
    flags = r_u8(object + 5);
    frame = r_s8(object + 89);
    elapsed = r_u32(object + 96);
    w_u8(object + 5, flags | 4);
    divisor = r_s16(data + (frame << 1) + 4);
    numerator = elapsed << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    elapsed = r_u32(object + 96);
    increment = r_u32(object + 104);
    elapsed += increment;
    next_frame = r_s8(object + 89);
    w_u32(object + 96, elapsed);
    w_u16(object + 6, quotient + (frame << 12));
    limit = r_s16(data + (next_frame << 1) + 4);
    frame = r_u8(object + 89);
    if ((sint32)limit < (sint32)elapsed)
    {
        w_u32(object + 96, elapsed - limit);
        w_u8(object + 89, frame + 1);
    }
    else if ((sint32)elapsed < 0)
    {
        next_frame = frame - 1;
        w_u8(object + 89, next_frame);
        next_frame = (sint32)(next_frame << 24) >> 24;
        if ((sint32)next_frame >= 0)
        {
            value = r_s16(data + (next_frame << 1) + 4);
            w_u32(object + 96, value);
        }
    }
    count = r_u8(data + 3);
    frame = r_s8(object + 89);
    if (frame == count - 1)
    {
        kind = r_u8(data + 2);
        if (kind == 1 || kind == 2)
        {
            slot = 0x800C6FD0 + (member << 2);
            flags = r_u32(slot);
            w_u32(slot, flags & 0xFFFDFFFF);
            if (kind == 2)
                sub_80051f88(index);
        }
        else if (kind == 3)
        {
            w_u8(object + 89, 0);
            w_u32(object + 96, 0);
        }
        else
        {
            next_frame = count - 2;
            w_u8(object + 89, next_frame);
            next_frame = (sint32)(next_frame << 24) >> 24;
            value = r_s16(data + (next_frame << 1) + 4);
            w_u32(object + 104, 0xFFFFFFFF);
            w_u32(object + 96, value);
        }
    }
    frame = r_s8(object + 89);
    if ((sint32)frame >= 0)
        return frame;
    kind = r_u8(data + 2);
    if (kind == 4)
    {
        slot = 0x800C6FD0 + (member << 2);
        flags = r_u32(slot);
        w_u32(slot, flags & 0xFFFDFFFF);
        kind = r_u8(data + 2);
    }
    if (kind == 5)
    {
        slot = 0x800C6FD0 + (member << 2);
        flags = r_u32(slot);
        w_u32(slot, flags & 0xFFFDFFFF);
        return sub_80051f88(index);
    }
    w_u8(object + 89, 0);
    w_u32(object + 96, 0);
    w_u32(object + 104, 1);
    return 1;
}

uint32 sub_80053b24(uint32 first_index, uint32 second_index)
{
    uint32 descriptors, first, second, first_value, second_value;
    FUNCTION_MARKER(0x80053B24, "ACE3.BIN");
    descriptors = r_u32(0x800BCEFC);
    first = descriptors + first_index * 40;
    second = descriptors + second_index * 40;
    second_value = r_u32(second + 24);
    first_value = r_u32(first + 24);
    w_u32(first + 24, second_value);
    w_u32(second + 24, first_value);
    return second;
}

uint32 sub_800539d0(void)
{
    uint32 i, index, random, remainder, target;
    FUNCTION_MARKER(0x800539D0, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
    {
        index = r_u8(0x800B20C8 + i);
        random = game_random();
        if ((random & 1) != 0)
            sub_80053b24(index, index + 1);
    }
    for (i = 0; i < 6; ++i)
    {
        index = r_u8(0x800B20CC + i);
        random = game_random();
        remainder = (sint32)random % 6;
        if (remainder < 6)
        {
            target = r_u32(0x80024224 + (remainder << 2));
            switch (target)
            {
                case 0x80053AF0:
                    break;
                case 0x80053AA0:
                    sub_80053b24(index + 1, index + 2);
                    break;
                case 0x80053AA8:
                    sub_80053b24(index, index + 1);
                    break;
                case 0x80053AB4:
                    sub_80053b24(index, index + 1);
                    sub_80053b24(index + 1, index + 2);
                    break;
                case 0x80053AC8:
                    sub_80053b24(index + 1, index + 2);
                    sub_80053b24(index, index + 1);
                    break;
                case 0x80053AE0:
                    sub_80053b24(index, index + 2);
                    break;
            }
        }
    }
    return 0;
}

uint32 sub_80053b5c(void)
{
    FUNCTION_MARKER(0x80053B5C, "ACE3.BIN");
    w_u16(0x800BD000, 0x4000);
    w_u16(0x800BCFFE, 0x4000);
    w_u16(0x800BCFFC, 0x4000);
    w_u16(0x800BD008, 0);
    return 0x4000;
}

uint32 sub_80053b78(void)
{
    uint32 scripts, script;
    FUNCTION_MARKER(0x80053B78, "ACE3.BIN");
    scripts = r_u32(0x800BCEAC);
    script = r_u32(scripts + 328);
    return sub_80054ebc(script);
}

void sub_80053ba0(void)
{
    FUNCTION_MARKER(0x80053BA0, "ACE3.BIN");
    w_u32(0x800BD004, 0);
}

uint32 sub_80052d70(uint32 from, uint32 to, uint32 out, const uint32 *native_from, const uint32 *native_to, uint32 *native_out)
{
    uint32 end, start, x, y, z, ax, ay, az, shift, length, numerator, value;
    FUNCTION_MARKER(0x80052D70, "ACE3.BIN");
    end = (native_to != 0 ? native_to[0] : r_u32(to));
    start = (native_from != 0 ? native_from[0] : r_u32(from));
    x = end - start;
    end = (native_to != 0 ? native_to[1] : r_u32(to + 4));
    start = (native_from != 0 ? native_from[1] : r_u32(from + 4));
    ax = (sint32)x < 0 ? 0 - x : x;
    y = end - start;
    end = (native_to != 0 ? native_to[2] : r_u32(to + 8));
    start = (native_from != 0 ? native_from[2] : r_u32(from + 8));
    ay = (sint32)y < 0 ? 0 - y : y;
    z = end - start;
    az = (sint32)z < 0 ? 0 - z : z;
    if ((sint32)ax < 4096 && (sint32)ay < 4096 && (sint32)az < 4096)
    {
        if (native_out != 0)
            native_out[0] = 0;
        else
            w_u32(out, 0);
        if (native_out != 0)
            native_out[1] = 0;
        else
            w_u32(out + 4, 0);
        if (native_out != 0)
            native_out[2] = 0;
        else
            w_u32(out + 8, 0);
        return 0;
    }
    shift = 0;
    while ((sint32)ax >= 0x4000 || (sint32)ay >= 0x4000 || (sint32)az >= 0x4000)
    {
        ++shift;
        ax = (sint32)ax >> 1;
        az = (sint32)az >> 1;
        ay = (sint32)ay >> 1;
    }
    length = SquareRoot0PSX(ax * ax + ay * ay + az * az, 0x800B6DD4);
    x = (sint32)x >> (shift & 31);
    numerator = x << 14;
    if (length == 0)
        xport_mips_break(7);
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (sint32)numerator / (sint32)length;
    y = (sint32)y >> (shift & 31);
    z = (sint32)z >> (shift & 31);
    if (native_out != 0)
        native_out[0] = value;
    else
        w_u32(out, value);
    numerator = y << 14;
    if (length == 0)
        xport_mips_break(7);
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (sint32)numerator / (sint32)length;
    if (native_out != 0)
        native_out[1] = value;
    else
        w_u32(out + 4, value);
    numerator = z << 14;
    if (length == 0)
        xport_mips_break(7);
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (sint32)numerator / (sint32)length;
    if (native_out != 0)
        native_out[2] = value;
    else
        w_u32(out + 8, value);
    return length << (shift & 31);
}

uint32 sub_80052fa8(uint32 index)
{
    uint32 descriptors, table, data, enabled, object, game, mission, kind, x, selected, member, result, flags, pointer, target, target_x, object_x, object_y, target_y, target_z, object_z, dx, dy, dz, distance, limit;
    FUNCTION_MARKER(0x80052FA8, "ACE3.BIN");
    descriptors = r_u32(0x800BCEFC);
    table = r_u32(0x800BCE4C);
    data = r_u32(descriptors + index * 40 + 16);
    enabled = r_u8(data);
    object = r_u32(table + (index << 2));
    if (enabled == 0)
        return 0;
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 12)
    {
        kind = r_s16(object + 72);
        if (kind == 54)
        {
            x = r_u32(object + 8);
            if (x + 0x1B000000 <= 0x02000000)
                return 0;
            if (x + 0x14000000 <= 0x04000000)
                return 0;
            if (x + 0x07000000 <= 0x02000000)
                return 0;
            if (x + 0xFF800000 <= 0x03000000)
                return 0;
        }
    }
    selected = r_u8(object + 91);
    member = index + 32;
    if (selected == 255)
    {
        result = sub_80046124(member);
        selected = result & 255;
        w_u8(object + 91, result);
        if (selected == 255)
        {
            flags = r_u32(object);
            if ((flags & 0x80) != 0)
                w_u8(object + 91, 0);
            result = r_u8(object + 91);
            if (result == selected)
                return 0;
        }
    }
    selected = r_u8(object + 91);
    if (selected == 0)
    {
        pointer = r_u32(0x800C6ED0 + (member << 2));
        distance = r_u32(pointer + 4);
    }
    else
    {
        selected = r_u8(object + 91);
        table = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + (selected << 2));
        target = r_u32(table + (selected << 2));
        if ((flags & 1) == 0)
            w_u8(object + 91, 255);
        object_x = r_u32(object + 8);
        target_x = r_u32(target + 8);
        object_y = r_u32(object + 12);
        dx = (sint32)(object_x - target_x) >> 12;
        if ((sint32)dx < 0)
            dx = 0 - dx;
        target_y = r_u32(target + 12);
        target_z = r_u32(target + 16);
        dy = (sint32)(object_y - target_y) >> 12;
        if ((sint32)dy < 0)
            dy = 0 - dy;
        object_z = r_u32(object + 16);
        dz = (sint32)(object_z - target_z) >> 12;
        if ((sint32)dz < 0)
            dz = 0 - dz;
        distance = dx + dy + dz;
    }
    limit = r_u32(data + 4);
    if (limit < distance)
    {
        result = sub_800464b0(member);
        w_u8(object + 91, result);
        return 0;
    }
    flags = r_u8(data);
    if ((flags & 0x10) != 0)
        result = sub_800535d4(index);
    else
        result = sub_80053214(index, distance);
    if (result != 0)
    {
        selected = sub_800464b0(member);
        w_u8(object + 91, selected);
    }
    return result;
}

uint32 sub_80053214(uint32 index, uint32 distance)
{
    uint32 table, descriptors, object, selected, data, target, first, second, third, fourth, ticks, velocity, value, gravity, old_y, old_x, target_x, target_y, old_z, target_z, game, difficulty, flags, spread, mode, random, angle, current, delta, lower, upper, phase, divisor, counter, kind;
    uint32 direction[3];
    FUNCTION_MARKER(0x80053214, "ACE3.BIN");
    ticks = (sint32)distance / 144;
    table = r_u32(0x800BCE4C);
    descriptors = r_u32(0x800BCEFC);
    object = r_u32(table + (index << 2));
    selected = r_u8(object + 91);
    table = r_u32(0x800BCE5C);
    data = r_u32(descriptors + index * 40 + 16);
    target = r_u32(table + (selected << 2));
    first = r_u32(object + 8);
    second = r_u32(object + 12);
    third = r_u32(object + 16);
    fourth = r_u32(object + 20);
    w_u32(object + 108, first);
    w_u32(object + 112, second);
    w_u32(object + 116, third);
    w_u32(object + 120, fourth);
    first = r_u32(target + 8);
    second = r_u32(target + 12);
    third = r_u32(target + 16);
    fourth = r_u32(target + 20);
    w_u32(object + 132, first);
    w_u32(object + 136, second);
    w_u32(object + 140, third);
    w_u32(object + 144, fourth);
    velocity = r_u32(target + 56);
    value = r_u32(object + 132);
    w_u32(object + 132, value + velocity * ticks);
    velocity = r_u32(target + 60);
    value = r_u32(object + 136);
    w_u32(object + 136, value + velocity * ticks);
    gravity = ticks * (ticks * 669);
    velocity = r_u32(target + 64);
    gravity = (sint32)gravity >> 1;
    value = r_u32(object + 136);
    old_x = r_u32(object + 8);
    target_x = r_u32(object + 132);
    w_u32(object + 136, value - gravity);
    value = r_u32(object + 140);
    w_u32(object + 140, value + velocity * ticks);
    direction[0] = (sint32)(target_x - old_x) >> 12;
    target_y = r_u32(object + 136);
    old_y = r_u32(object + 12);
    direction[1] = (sint32)(target_y - old_y) >> 12;
    target_z = r_u32(object + 140);
    old_z = r_u32(object + 16);
    direction[2] = (sint32)(target_z - old_z) >> 12;
    sub_80031bb4(0, object + 124, direction);
    game = r_u32(0x800BCE60);
    difficulty = r_u32(game + 916);
    spread = 600;
    if (difficulty == 0)
    {
        flags = r_u32(object);
        if ((flags & 0x80) != 0)
            goto spread_ready;
    }
    mode = r_u8(data + 1);
    if (mode == 0)
        spread = 0;
    else if (mode == 1)
        spread = 200;
spread_ready:
    if (spread != 0)
    {
        random = game_random();
        angle = r_u16(object + 124);
        w_u16(object + 124, angle + (uint32)((sint32)random % (sint32)(spread << 1)) - spread);
        random = game_random();
        angle = r_u16(object + 126);
        w_u16(object + 126, angle + (uint32)((sint32)random % (sint32)(spread << 1)) - spread);
    }
    angle = r_s16(object + 124);
    current = r_s16(object + 24);
    delta = angle - current;
    if ((sint32)delta > 4096)
        delta = 0x2000 - delta;
    else if ((sint32)delta < -4096)
        delta = 0 - 8192 - delta;
    lower = r_s16(data + 8);
    if ((sint32)delta < (sint32)lower)
        return 1;
    upper = r_s16(data + 10);
    if ((sint32)upper < (sint32)delta)
        return 1;
    angle = r_s16(object + 126);
    current = r_s16(object + 26);
    lower = r_s16(data + 12);
    delta = (angle - current) & 0x3FFF;
    if ((sint32)delta < (sint32)lower)
        delta += 0x4000;
    upper = r_s16(data + 14);
    if ((sint32)upper < (sint32)delta)
        return 1;
    phase = r_s8(object + 90);
    if (phase == 0)
    {
        divisor = r_s16(data + 16);
        counter = r_u32(object + 100);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && counter == 0x80000000)
            xport_mips_break(6);
        if ((sint32)counter % (sint32)divisor == 0)
        {
            kind = r_u8(data);
            flags = r_u32(object);
            sub_8006b034(flags, object + 108, 0x800B20A8, kind - 1);
        }
        upper = r_s16(data + 18);
        counter = r_u32(object + 100);
        if ((sint32)counter >= (sint32)upper)
        {
            w_u8(object + 90, 1);
            w_u32(object + 100, 0);
        }
    }
    else
    {
        upper = r_s16(data + 20);
        counter = r_u32(object + 100);
        if ((sint32)counter >= (sint32)upper)
        {
            w_u8(object + 90, 0);
            w_u32(object + 100, 0);
        }
    }
    counter = r_u32(object + 100);
    w_u32(object + 100, counter + 1);
    return 0;
}

uint32 sub_800535d4(uint32 index)
{
    uint32 table, descriptors, object, selected, data, target_object, mask_root, mask, saved_phase, saved_counter, first, second, third, fourth, end, start, angle, current, delta, lower, upper, kind, mode, game, difficulty, flags, phase, delay, counter, target;
    uint32 direction[3];
    FUNCTION_MARKER(0x800535D4, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    descriptors = r_u32(0x800BCEFC);
    object = r_u32(table + (index << 2));
    selected = r_u8(object + 91);
    table = r_u32(0x800BCE5C);
    data = r_u32(descriptors + index * 40 + 16);
    target_object = r_u32(table + (selected << 2));
    mask_root = r_u32(0x800BD080);
    if (selected < 32)
    {
        mask = r_u32(mask_root);
        if ((mask & ((uint32)1 << selected)) != 0)
            return 1;
    }
    else
    {
        mask = r_u32(mask_root + 4);
        if ((mask & ((uint32)1 << ((selected - 32) & 31))) != 0)
            return 1;
    }
    saved_phase = r_s8(object + 90);
    saved_counter = r_u32(object + 100);
    w_u8(object + 90, 0);
    w_u32(object + 100, 0);
    first = r_u32(object + 8);
    second = r_u32(object + 12);
    third = r_u32(object + 16);
    fourth = r_u32(object + 20);
    w_u32(object + 108, first);
    w_u32(object + 112, second);
    w_u32(object + 116, third);
    w_u32(object + 120, fourth);
    first = r_u32(target_object + 8);
    second = r_u32(target_object + 12);
    third = r_u32(target_object + 16);
    fourth = r_u32(target_object + 20);
    w_u32(object + 132, first);
    w_u32(object + 136, second);
    w_u32(object + 140, third);
    w_u32(object + 144, fourth);
    end = r_u32(object + 132);
    start = r_u32(object + 8);
    direction[0] = (sint32)(end - start) >> 12;
    end = r_u32(object + 136);
    start = r_u32(object + 12);
    direction[1] = (sint32)(end - start) >> 12;
    end = r_u32(object + 140);
    start = r_u32(object + 16);
    direction[2] = (sint32)(end - start) >> 12;
    sub_80031bb4(0, object + 124, direction);
    angle = r_s16(object + 124);
    current = r_s16(object + 24);
    delta = angle - current;
    if ((sint32)delta > 4096)
        delta = 0x2000 - delta;
    else if ((sint32)delta < -4096)
        delta = 0 - 8192 - delta;
    lower = r_s16(data + 8);
    if ((sint32)delta < (sint32)lower)
        return 1;
    upper = r_s16(data + 10);
    if ((sint32)upper < (sint32)delta)
        return 1;
    angle = r_s16(object + 126);
    current = r_s16(object + 26);
    lower = r_s16(data + 12);
    delta = (angle - current) & 0x3FFF;
    if ((sint32)delta < (sint32)lower)
        delta += 0x4000;
    upper = r_s16(data + 14);
    if ((sint32)upper < (sint32)delta)
        return 1;
    w_u8(object + 90, saved_phase);
    w_u32(object + 100, saved_counter);
    kind = r_u8(data);
    mode = 0;
    if (kind == 17)
    {
        game = r_u32(0x800BCE60);
        difficulty = r_u32(game + 916);
        mode = 0x100;
        if (difficulty == 0)
        {
            flags = r_u32(object);
            if ((flags & 0x80) != 0)
                goto mode_ready;
        }
        kind = r_u8(data + 1);
        if (kind == 0)
            mode = 0x20100;
        else if (kind == 1)
            mode = 0x10100;
    }
mode_ready:
    phase = r_s8(object + 90);
    if (phase == 0)
    {
        flags = r_u32(object);
        delay = r_s16(data + 18);
        if ((flags & 0x80) != 0)
        {
            game = r_u32(0x800BCE60);
            difficulty = r_u32(game + 916);
            if (difficulty == 0)
                delay <<= 1;
            if ((sint32)difficulty >= 2)
                delay = (sint32)delay >> 1;
        }
        counter = r_u32(object + 100);
        if (counter == (uint32)((sint32)delay >> 1))
        {
            flags = r_u32(0x800C6ECC);
            w_u32(0x800C6ECC, flags | 1);
        }
        counter = r_u32(object + 100);
        if ((sint32)counter < (sint32)delay)
            goto advance_counter;
        kind = r_u8(data);
        kind -= 17;
        if (kind < 9)
        {
            target = r_u32(0x800241FC + (kind << 2));
            switch (target)
            {
                case 0x800538D8:
                    flags = r_u32(object);
                    selected = r_u32(target_object);
                    sub_80068800(flags, object + 108, 0x800B20B8, selected, mode, 0);
                    break;
                case 0x800538FC:
                    flags = r_u32(object);
                    selected = r_u32(target_object);
                    sub_80068c0c(flags, object + 108, 0x800B20B8, selected, 0);
                    break;
                case 0x8005391C:
                    flags = r_u32(object);
                    selected = r_u32(target_object);
                    sub_80068c80(flags, object + 108, 0x800B20B8, selected, 0);
                    break;
                case 0x8005393C:
                    flags = r_u32(object);
                    selected = r_u32(target_object);
                    sub_80068ba8(flags, object + 108, 0x800B20B8, selected, 0);
                    break;
                case 0x8005395C:
                    flags = r_u32(object);
                    selected = r_u32(target_object);
                    sub_800689e8(flags, object + 108, 0x800B20B8, selected, 0);
                    break;
                case 0x80053974:
                    break;
            }
        }
        w_u8(object + 90, 1);
        w_u32(object + 100, 0);
    }
    else
    {
        upper = r_s16(data + 20);
        counter = r_u32(object + 100);
        if ((sint32)counter < (sint32)upper)
            goto advance_counter;
        w_u8(object + 90, 0);
        w_u32(object + 100, 0);
    }
advance_counter:
    counter = r_u32(object + 100);
    w_u32(object + 100, counter + 1);
    return 0;
}

uint32 sub_8005b308(void)
{
    uint32 game, buffer, flags, control, other, state, result, selection, table, value, first, second, pixels, enabled, setting;
    PSX_RECT capture, clear;
    FUNCTION_MARKER(0x8005B308, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    if (buffer == 0)
        return 0;
    flags = r_u32(game);
    control = r_u32(0x800BCE48);
    w_u32(game, flags | 0x10000000);
    w_u8(control + 16, 0);
    other = r_u32(0x800BCE48);
    w_u32(control + 20, 0);
    w_u32(control + 4, 1);
    w_u8(other + 17, 0);
    control = r_u32(0x800BCE48);
    w_u8(control + 18, 0);
    control = r_u32(0x800BCE48);
    flags = r_u32(control);
    w_u32(control, flags | 9);
    result = arena_alloc(100);
    w_u32(0x800BD050, result);
    result = arena_alloc(0x2000);
    state = r_u32(0x800BD050);
    w_u32(state + 4, result);
    result = arena_alloc(0x2000);
    state = r_u32(0x800BD050);
    w_u32(state + 8, result);
    game = r_u32(0x800BCE60);
    w_u32(state, 0);
    w_u16(state + 76, 320);
    w_u16(state + 78, 480);
    selection = r_u8(game + 936);
    table = r_u32(0x800BCD94);
    value = r_s16(table + (selection << 3));
    w_u32(state + 84, 0);
    w_u32(state + 80, value);
    first = r_u32(0x800BCDAC);
    first = r_u32(0x800BCDAC);
    second = r_u32(0x800BCDB0);
    second = r_u32(0x800BCDB0);
    capture.x = (sint16)first;
    capture.y = (sint16)(first >> 16);
    capture.w = (sint16)second;
    capture.h = (sint16)(second >> 16);
    pixels = arena_alloc(0x2800);
    state = r_u32(0x800BD050);
    w_u32(state + 88, pixels);
    StoreImage(&capture, (uint32 *)psx_addr(pixels, 0x2800));
    clear.x = 0;
    clear.y = 0;
    clear.w = 320;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    game = r_u32(0x800BCE60);
    enabled = r_u8(game + 924);
    flags = r_u8(game + 935);
    if ((enabled & flags) != 0)
    {
        selection = r_u8(game + 936);
        table = r_u32(0x800BCD94);
        value = r_s16(table + (selection << 3) + 2);
        setting = r_u32(0x800BCDA0);
        w_u32(setting + 4, value == 1);
    }
    else
    {
        setting = r_u32(0x800BCDA0);
        w_u32(setting + 4, 0);
    }
    setting = r_u32(0x800BCDA0);
    value = r_u32(setting + 4);
    if (value != 0)
        sub_8002cc44(10, 0, 0);
    return 0x04000000;
}

uint32 sub_8005b4e4(void)
{
    uint32 game, buffer, state, ordering, packet, result, setting;
    FUNCTION_MARKER(0x8005B4E4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x800BD050);
    ordering = state + buffer * 20 + 12;
    w_u32(state + 56, ordering);
    packet = r_u32(state + (buffer << 2) + 4);
    w_u32(state + 52, packet);
    ClearOTagR(psx_addr(ordering, 20), 5);
    result = sub_800582f8();
    if (result != 0)
    {
        sub_8005b5bc();
        game = r_u32(0x800BCE60);
        result = r_u32(game + 928);
        return result | 0x60000000;
    }
    setting = r_u32(0x800BCDA0);
    result = r_u32(setting + 4);
    if (result != 0)
    {
        state = r_u32(0x800BD050);
        ordering = r_u32(state + 56);
        packet = r_u32(state + 52);
        result = sub_8002d3f4(ordering, packet);
        state = r_u32(0x800BD050);
        w_u32(state + 52, result);
    }
    state = r_u32(0x800BD050);
    ordering = r_u32(state + 56);
    DrawOTag(psx_addr(ordering + 16, 4));
    return 0;
}

uint32 sub_8005b5bc(void)
{
    uint32 state, pixels, setting, enabled;
    PSX_RECT clear;
    FUNCTION_MARKER(0x8005B5BC, "ACE3.BIN");
    resource_init_queue(0, 0);
    clear.w = 320;
    clear.x = 0;
    clear.y = 0;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    state = r_u32(0x800BD050);
    pixels = r_u32(state + 88);
    LoadImagePSX((PSX_RECT *)psx_addr(0x800BCDC4, 8), (uint32 *)psx_addr(pixels, 0x2800));
    setting = r_u32(0x800BCDA0);
    enabled = r_u32(setting + 4);
    if (enabled != 0)
        sub_8002cc44(255, 0, 0);
    return 1;
}

uint32 sub_8005edc8(uint32 ordering, uint32 packet, uint32 records, uint32 count)
{
    uint32 scene, source, red, green, blue, game, ticks, record, i, j, value, far_red, far_green, far_blue, shift, phase, sine, factor, indices, texture, x, y, color, packed, flag, bucket, tag, link;
    FUNCTION_MARKER(0x8005EDC8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    source = r_u32(scene + 580);
    red = r_u8(source + 8) << 4;
    green = r_u8(source + 9) << 4;
    blue = r_u8(source + 10);
    game = r_u32(0x800BCE60);
    ticks = r_u32(game + 40);
    blue <<= 4;
    if ((sint32)count <= 0)
        return packet;
    record = records + 4;
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        value = r_s16(record + 2);
        xport_gte_write_data(28, value);
        far_red = r_u8(record + 4);
        far_blue = r_u8(record + 6);
        far_red <<= 4;
        far_green = r_u8(record + 5);
        far_blue <<= 4;
        far_green <<= 4;
        xport_gte_write_control(21, far_red);
        xport_gte_write_control(22, far_green);
        xport_gte_write_control(23, far_blue);
        shift = r_s16(record - 2);
        phase = r_s16(record);
        phase += ticks << (shift & 31);
        sine = r_u16(0x800104AC + ((phase & 0x3FFF) << 1));
        sine = (sint32)(sine << 16) >> 19;
        factor = sine + 2048;
        xport_gte_write_data(8, factor);
        xport_gte_execute(0x980011);
        scene = r_u32(0x800BCE54);
        indices = r_u32(scene + 648);
        texture = r_u16(records);
        flag = far_red & 0x8000;
        x = (texture & 15) + 320;
        y = ((sint32)(texture << 16) >> 20) + 256;
        color = xport_gte_read_data(22);
        xport_gte_write_control(21, red);
        xport_gte_write_control(22, green);
        xport_gte_write_control(23, blue);
        xport_gte_write_data(6, color);
        packed = (y << 16) | x;
        for (j = 0; j < 16; ++j)
        {
            xport_gte_write_data(8, j << 8);
            xport_gte_execute(0x780010);
            value = r_s16(indices);
            bucket = ordering + (value << 2);
            value = xport_gte_read_data(29) | flag;
            w_u32(packet, 0x05000000);
            w_u32(packet + 16, value);
            w_u32(packet + 4, 0xA0000000);
            w_u32(packet + 8, packed);
            w_u32(packet + 12, 0x10001);
            w_u32(packet + 20, 0x01000000);
            indices += 2;
            tag = r_u32(packet);
            link = r_u32(bucket);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            link = r_u32(bucket);
            w_u32(bucket, (link & 0xFF000000) | (packet & 0xFFFFFF));
            packet += 24;
        }
        record += 12;
        records += 12;
    }
    return packet;
}

uint32 sub_8005eff0(uint32 ordering, uint32 packet)
{
    uint32 scene, first, second, third, fourth, fifth, sixth, indices, i, index, bucket, tag, link;
    FUNCTION_MARKER(0x8005EFF0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    first = r_u32(0x80024580);
    second = r_u32(0x80024584);
    third = r_u32(0x80024588);
    fourth = r_u32(0x8002458C);
    fifth = r_u32(0x80024590);
    sixth = r_u32(0x80024594);
    indices = r_u32(scene + 648);
    for (i = 0; i < 16; ++i)
    {
        index = r_s16(indices);
        indices += 2;
        w_u32(packet, first);
        w_u32(packet + 4, second);
        bucket = ordering + (index << 2);
        w_u32(packet + 12, fourth);
        w_u32(packet + 16, fifth);
        w_u32(packet + 20, sixth);
        w_u32(packet + 8, third);
        tag = r_u32(packet);
        link = r_u32(bucket);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(bucket);
        w_u32(bucket, (link & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 24;
    }
    return packet;
}

uint32 sub_8005f0e4(uint32 enabled)
{
    uint32 scene, flags;
    FUNCTION_MARKER(0x8005F0E4, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if (enabled != 0)
    {
        flags |= 4;
        w_u32(scene, flags);
        return flags;
    }
    w_u32(scene, flags & 0xFFFFFFFB);
    return scene;
}

uint32 sub_80068f34(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 effect, value;
    FUNCTION_MARKER(0x80068F34, "ACE3.BIN");
    effect = sub_800685c0(flags, position, vector, owner, native_position);
    if (effect == 0)
        return 0;
    w_u16(effect + 26, 2048);
    w_u8(effect + 5, 4);
    w_u8(effect + 7, 2);
    w_u32(effect + 156, 16);
    value = r_u32(effect);
    w_u8(effect + 52, 0);
    w_u16(effect + 24, 0);
    w_u32(effect + 56, 0);
    w_u32(effect + 60, 0);
    w_u32(effect + 64, 0);
    w_u32(effect + 160, 1);
    w_u32(effect, value | 0x20);
    return effect;
}

uint32 sub_80068fa8(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 effect, value;
    FUNCTION_MARKER(0x80068FA8, "ACE3.BIN");
    effect = sub_800685c0(flags, position, vector, owner, native_position);
    if (effect == 0)
        return 0;
    w_u16(effect + 26, 0x4000);
    w_u8(effect + 5, 4);
    w_u8(effect + 7, 3);
    value = r_u32(effect);
    w_u8(effect + 52, 0);
    w_u16(effect + 24, 0);
    w_u32(effect + 56, 0);
    w_u32(effect + 60, 0);
    w_u32(effect + 64, 0);
    w_u32(effect + 156, 1);
    w_u32(effect + 160, 1);
    w_u32(effect, value | 0x20);
    return effect;
}

uint32 sub_8005ae8c(void)
{
    uint32 game, control, scene, flags, cursor, other, result, state, selection, table, value, first, second, pixels, enabled, setting, mode, context;
    PSX_RECT capture, clear;
    FUNCTION_MARKER(0x8005AE8C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    control = r_u32(0x800BCE48);
    flags = r_u32(game);
    scene = r_u32(0x800BCE54);
    w_u32(game, flags | 0x10000000);
    w_u32(control + 20, 0);
    cursor = r_u32(scene + 696);
    arena_set_cursor(cursor);
    control = r_u32(0x800BCE48);
    w_u8(control + 16, 0);
    other = r_u32(0x800BCE48);
    w_u32(control + 4, 1);
    w_u8(other + 17, 0);
    control = r_u32(0x800BCE48);
    w_u8(control + 18, 0);
    control = r_u32(0x800BCE48);
    flags = r_u32(control);
    w_u32(control, flags | 9);
    result = arena_alloc(100);
    w_u32(0x800BD050, result);
    result = arena_alloc(0x2000);
    state = r_u32(0x800BD050);
    w_u32(state + 4, result);
    result = arena_alloc(0x2000);
    state = r_u32(0x800BD050);
    game = r_u32(0x800BCE60);
    w_u32(state + 8, result);
    w_u16(state + 78, 480);
    w_u32(state, 0);
    w_u16(state + 76, 320);
    w_u32(state + 96, 120);
    selection = r_u8(game + 936);
    table = r_u32(0x800BCD94);
    value = r_s16(table + (selection << 3));
    w_u32(state + 84, 0);
    w_u32(state + 80, value);
    first = r_u32(0x800BCDAC);
    first = r_u32(0x800BCDAC);
    second = r_u32(0x800BCDB0);
    second = r_u32(0x800BCDB0);
    capture.x = (sint16)first;
    capture.y = (sint16)(first >> 16);
    capture.w = (sint16)second;
    capture.h = (sint16)(second >> 16);
    pixels = arena_alloc(0x2800);
    state = r_u32(0x800BD050);
    w_u32(state + 88, pixels);
    StoreImage(&capture, (uint32 *)psx_addr(pixels, 0x2800));
    capture.x = 768;
    capture.w = 256;
    capture.y = 0;
    capture.h = 284;
    pixels = arena_alloc(0x23800);
    state = r_u32(0x800BD050);
    w_u32(state + 92, pixels);
    StoreImage(&capture, (uint32 *)psx_addr(pixels, 0x23800));
    clear.x = 0;
    clear.y = 0;
    clear.w = 320;
    clear.h = 496;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    game = r_u32(0x800BCE60);
    enabled = r_u8(game + 924);
    flags = r_u8(game + 935);
    if ((enabled & flags) != 0)
    {
        selection = r_u8(game + 936);
        table = r_u32(0x800BCD94);
        value = r_s16(table + (selection << 3) + 2);
        setting = r_u32(0x800BCDA0);
        w_u32(setting + 4, value == 1);
    }
    else
    {
        setting = r_u32(0x800BCDA0);
        w_u32(setting + 4, 0);
    }
    setting = r_u32(0x800BCDA0);
    value = r_u32(setting + 4);
    if (value != 0)
        sub_8002cc44(10, 0, 0);
    game = r_u32(0x800BCE60);
    value = r_s16(game + 940);
    if (value != 0)
    {
        selection = r_u8(game + 936);
        if (selection == 12 || selection == 27)
        {
            control = r_u32(0x800BCE48);
            scene = r_u32(0x800BCE54);
            w_u32(control + 20, 2);
            flags = r_u32(control);
            w_u32(control + 4, 0);
            w_u32(control, flags | 5);
            context = r_u32(scene + 8);
            mode = 0x80000;
            goto switch_scene;
        }
    }
    scene = r_u32(0x800BCE54);
    context = r_u32(scene + 8);
    mode = 0x40000;
switch_scene:
    return sub_80032f18(mode, context);
}

uint32 sub_8005b234(void)
{
    uint32 scene, cursor, state, pixels, setting, enabled;
    PSX_RECT clear;
    FUNCTION_MARKER(0x8005B234, "ACE3.BIN");
    resource_init_queue(0, 0);
    scene = r_u32(0x800BCE54);
    cursor = r_u32(scene + 696);
    arena_set_cursor(cursor);
    scene = r_u32(0x800BCE54);
    cursor = r_u32(scene + 696);
    sub_800323f8(cursor);
    clear.w = 320;
    clear.x = 0;
    clear.y = 0;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    state = r_u32(0x800BD050);
    pixels = r_u32(state + 88);
    LoadImagePSX((PSX_RECT *)psx_addr(0x800BCDB4, 8), (uint32 *)psx_addr(pixels, 0x2800));
    state = r_u32(0x800BD050);
    pixels = r_u32(state + 92);
    LoadImagePSX((PSX_RECT *)psx_addr(0x800BCDBC, 8), (uint32 *)psx_addr(pixels, 0x23800));
    setting = r_u32(0x800BCDA0);
    enabled = r_u32(setting + 4);
    if (enabled != 0)
        sub_8002cc44(255, 0, 0);
    return sub_80037e50();
}

uint32 sub_80073a30(void)
{
    uint32 state, context, phase, mode;
    FUNCTION_MARKER(0x80073A30, "ACE3.BIN");
    state = r_u32(0x800BD094);
    context = r_u32(0x800BD098);
    w_u32(state + 16, 0);
    w_u32(context + 36, 0);
    phase = r_u32(state + 4);
    if (phase == 0x4000)
        return sub_80076dec(0, 1);
    if ((sint32)phase < 0x4001)
    {
        if (phase == 8)
        {
            mode = r_u32(0x800BCE14);
            return sub_80076b18(mode != 0, 1);
        }
        if (phase == 0x100)
        {
            mode = r_u32(0x800BCE14);
            return sub_80076c54(mode != 2, 1);
        }
        return 2;
    }
    if (phase == 0x8000)
        return sub_80076dec(1, 1);
    if (phase == 0x04000000)
        return sub_80076360(1);
    return 0x04000000;
}

uint32 sub_8005c270(void)
{
    uint32 selection, scene, record, phase, buttons, kind, eligible, ticks, game, flags, control, ordering, packet, result, value;
    FUNCTION_MARKER(0x8005C270, "ACE3.BIN");
    selection = r_u32(0x800BCEA8);
    scene = r_u32(0x800BCE54);
    record = 0x800B1FC4 + (selection << 2);
    phase = r_u32(scene + 4);
    if (phase == 0x400)
        return sub_8005bc64();
    if (phase == 0x2000)
        return sub_8005be0c();
    if (phase == 0x1000)
        return 0x1000;
    buttons = sub_8006cb30();
    kind = r_u8(record);
    if (kind != 7 && (buttons & 0x200) == 0)
    {
        eligible = kind - 2 < 2;
        if (eligible == 0 && (kind < 2 || kind == 5 || kind == 6 || kind == 4))
        {
            scene = r_u32(0x800BCE54);
            ticks = r_u32(scene + 12);
            eligible = (sint32)ticks < 300;
        }
        if (eligible != 0 && (buttons & 0x40) == 0)
        {
            game = r_u32(0x800BCE60);
            flags = r_u32(game + 300);
            if ((flags & 0x40) != 0)
            {
                sub_80073a30();
                sub_8004291c(9);
                pad_reset_slots();
                control = r_u32(0x800BCEB4);
                w_u32(control + 20, 0);
                w_u8(control + 28, 255);
            }
            scene = r_u32(0x800BCE54);
            ticks = r_u32(scene + 12);
            if ((ticks & 0x10) != 0)
            {
                ordering = r_u32(scene + 16);
                packet = r_u32(scene + 20);
                result = sub_8003563c(ordering, packet, 80, 208, 0x8300, 0x80024548, 0);
                scene = r_u32(0x800BCE54);
                w_u32(scene + 20, result);
                ordering = r_u32(scene + 16);
                packet = r_u32(scene + 20);
                result = sub_8003563c(ordering, packet, 96, 216, 0x8300, 0x8002455C, 0);
                scene = r_u32(0x800BCE54);
                w_u32(scene + 20, result);
            }
        }
    }
    scene = r_u32(0x800BCE54);
    ticks = r_u32(scene + 12);
    if (ticks == 3600)
        sub_8004ceb0(10);
    if ((buttons & 0x7000) != 0)
        return sub_80032f18(0x2000, 1);
    scene = r_u32(0x800BCE54);
    ticks = r_u32(scene + 12);
    if ((sint32)ticks >= 5400)
        return sub_80032f18(0x2000, 4);
    if ((buttons & 0x80) != 0)
        return sub_80032f18(0x2000, 2);
    if ((buttons & 0x100) != 0)
        return sub_80032f18(0x2000, 16);
    if ((buttons & 0x200) != 0)
        return sub_80032f18(0x2000, 0);
    kind = r_u8(record);
    if (kind != 7)
        return 7;
    control = r_u32(0x800BCEB4);
    value = r_s16(control + 8);
    if ((sint32)value >= 390)
    {
        sub_80032f18(0x2000, 2);
        control = r_u32(0x800BCEB4);
        w_u8(control + 47, 1);
    }
    control = r_u32(0x800BCEB4);
    value = r_s16(control + 8);
    if ((sint32)value < 0)
    {
        sub_80032f18(0x2000, 32);
        control = r_u32(0x800BCEB4);
        w_u8(control + 47, 0);
    }
    scene = r_u32(0x800BCE54);
    ticks = r_u32(scene + 12);
    if (ticks != 900)
        return 900;
    control = r_u32(0x800BCEB4);
    value = r_u8(control + 10);
    if (value == 1)
    {
        sub_80032f18(0x2000, 2);
        control = r_u32(0x800BCEB4);
        w_u8(control + 47, value);
    }
    else
    {
        sub_80032f18(0x2000, 32);
        control = r_u32(0x800BCEB4);
        w_u8(control + 47, 0);
    }
    return control;
}

uint32 sub_8006a978(uint32 state, uint32 value)
{
    uint32 bound, x, y, flags, z, mask, i, table, object, object_flags, dx, dy, dz, radius, distance, divisor, quotient, mode, parameter;
    FUNCTION_MARKER(0x8006A978, "ACE3.BIN");
    bound = value * 3;
    x = r_u32(state + 28);
    y = r_u32(state + 32);
    flags = r_u32(state + 108);
    z = r_u32(state + 36);
    mask = flags & 0x1C0;
    for (i = 0; i < 64; ++i)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (i << 2));
        object_flags = r_u32(object);
        if ((object_flags & 0x4200) != 0x4200 || (object_flags & mask) != 0)
            continue;
        dx = r_u32(object + 8);
        dy = r_u32(object + 12);
        dz = r_u32(object + 16);
        dx = (sint32)(dx - x) >> 12;
        dy = (sint32)(dy - y) >> 12;
        dz = (sint32)(dz - z) >> 12;
        if ((sint32)dx < 0)
            dx = 0 - dx;
        if ((sint32)dy < 0)
            dy = 0 - dy;
        if ((sint32)dz < 0)
            dz = 0 - dz;
        radius = r_s16(object + 74);
        if ((sint32)bound < (sint32)(dx + dy + dz + radius))
            continue;
        distance = sub_80083dfc(object + 8, state + 28, 0, 0);
        if ((sint32)value < (sint32)distance)
            continue;
        divisor = r_u32(state + 160);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && distance == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)distance / (sint32)divisor;
        mode = r_u32(state + 108);
        parameter = r_u16(state + 26);
        sub_80068454(mode, object_flags, parameter, quotient);
    }
    return 0;
}

uint32 sub_8006e7e0(uint32 value, uint32 control)
{
    uint32 factor, x, y, z;
    FUNCTION_MARKER(0x8006E7E0, "ACE3.BIN");
    if ((sint32)value > 0x14FFF)
        factor = 4096;
    else
        factor = (sint32)value / 21;
    x = r_s16(control + 4);
    y = r_s16(control + 6);
    z = r_s16(control + 8);
    x = (sint32)(x * factor) / 4096;
    y = (sint32)(y * factor) / 4096;
    z = (sint32)(z * factor) / 4096;
    w_u16(control + 4, x);
    w_u16(control + 6, y);
    w_u16(control + 8, z);
    return y;
}

uint32 sub_80071a00(uint32 slots)
{
    uint32 context, count, value, state, flags, table, object, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071A00, "ACE3.BIN");
    context = r_u32(0x800BD098);
    count = r_u32(context + 28);
    if (count == 0)
        return count;
    value = r_u32(context + 4);
    value = sub_800718a0(slots, value, 150);
    if (value == 0)
        return value;
    state = r_u32(0x800BD094);
    flags = r_u32(state + 16);
    context = r_u32(0x800BD098);
    w_u32(state + 16, flags | 0x4000);
    count = r_u32(context + 28);
    table = r_u32(0x800BCE5C);
    w_u32(context + 28, count - 1);
    object = r_u32(table);
    sub_8002db4c(26);
    sub_80029448(10);
    sub_80071770(object, 0, &pose);
    context = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(context + 24);
    return sub_80068800(flags, 0, object + 56, owner, 0x07010102, &pose);
}

uint32 sub_80071de4(uint32 slots)
{
    uint32 context, count, owner, value, state, flags, table, object;
    FUNCTION_MARKER(0x80071DE4, "ACE3.BIN");
    context = r_u32(0x800BD098);
    count = r_u32(context + 28);
    if (count == 0)
        return 0xFFFFFFFF;
    owner = r_u32(context + 24);
    if (owner == 0xFFFFFFFF)
        return 0xFFFFFFFF;
    value = r_u32(context + 4);
    value = sub_800718a0(slots, value, 180);
    if (value == 0)
        return value;
    state = r_u32(0x800BD094);
    flags = r_u32(state + 16);
    context = r_u32(0x800BD098);
    w_u32(state + 16, flags | 0x4000);
    count = r_u32(context + 28);
    table = r_u32(0x800BCE5C);
    w_u32(context + 28, count - 1);
    object = r_u32(table);
    sub_8002db4c(25);
    sub_80029448(7);
    context = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(context + 24);
    sub_80068f34(flags, object + 8, object + 56, owner, 0);
    context = r_u32(0x800BD098);
    owner = r_u32(context + 24);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + ((owner & 63) << 2));
    return sub_8008350c(object + 8, object + 56);
}

uint32 sub_80071ebc(uint32 slots)
{
    uint32 context, count, value, state, flags, table, object, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071EBC, "ACE3.BIN");
    context = r_u32(0x800BD098);
    count = r_u32(context + 28);
    if (count == 0)
        return count;
    value = r_u32(context + 4);
    value = sub_800718a0(slots, value, 90);
    if (value == 0)
        return value;
    state = r_u32(0x800BD094);
    flags = r_u32(state + 16);
    context = r_u32(0x800BD098);
    w_u32(state + 16, flags | 0x4000);
    count = r_u32(context + 28);
    table = r_u32(0x800BCE5C);
    w_u32(context + 28, count - 1);
    object = r_u32(table);
    sub_8002db4c(14);
    sub_80029448(8);
    sub_80071770(object, 0, &pose);
    context = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(context + 24);
    return sub_80068ba8(flags, 0, object + 56, owner, &pose);
}

uint32 sub_80071f60(uint32 slots)
{
    uint32 context, count, value, state, flags, table, object, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071F60, "ACE3.BIN");
    context = r_u32(0x800BD098);
    count = r_u32(context + 28);
    if (count == 0)
        return count;
    value = r_u32(context + 4);
    value = sub_800718a0(slots, value, 180);
    if (value == 0)
        return value;
    state = r_u32(0x800BD094);
    flags = r_u32(state + 16);
    context = r_u32(0x800BD098);
    w_u32(state + 16, flags | 0x4000);
    count = r_u32(context + 28);
    table = r_u32(0x800BCE5C);
    w_u32(context + 28, count - 1);
    object = r_u32(table);
    sub_8002db4c(16);
    sub_80029448(9);
    sub_80071770(object, 0, &pose);
    context = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(context + 24);
    return sub_800689e8(flags, 0, object + 56, owner, &pose);
}

uint32 sub_80072004(uint32 slots)
{
    uint32 context, count, value, state, flags, table, object, owner;
    FUNCTION_MARKER(0x80072004, "ACE3.BIN");
    context = r_u32(0x800BD098);
    count = r_u32(context + 28);
    if (count == 0)
        return count;
    value = r_u32(context + 4);
    value = sub_800718a0(slots, value, 180);
    if (value == 0)
        return value;
    state = r_u32(0x800BD094);
    flags = r_u32(state + 16);
    context = r_u32(0x800BD098);
    w_u32(state + 16, flags | 0x4000);
    count = r_u32(context + 28);
    table = r_u32(0x800BCE5C);
    w_u32(context + 28, count - 1);
    object = r_u32(table);
    sub_8002db4c(88);
    context = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(context + 24);
    return sub_80068fa8(flags, object + 8, object + 56, owner, 0);
}

uint32 sub_8008350c(uint32 position, uint32 vector)
{
    FUNCTION_MARKER(0x8008350C, "ACE3.BIN");
    sub_8007ed9c(9, position, vector, 0);
    return sub_800833c8(position, vector, 0, 0);
}

uint32 sub_80073928(uint32 flags)
{
    uint32 table, object, type, index, result, mode;
    FUNCTION_MARKER(0x80073928, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    type = r_s16(object + 72);
    index = flags & 255;
    if ((flags & 0x100) != 0)
    {
        result = 0x800B3B94 + index * 56;
        w_u32(0x800BCE14, 0);
        w_u32(0x800BCE18, result);
        return result;
    }
    if ((flags & 0x200) != 0)
        mode = 1;
    else if ((flags & 0x400) != 0 || (flags & 0x800) != 0)
    {
        mode = (flags & 0x400) != 0 ? 2 : 3;
        w_u32(0x800BCE14, mode);
        w_u32(0x800BCE1C, 0x800B3E6C + index * 20);
        w_u32(0x800BCE20, 0x800B3EBC + (type << 4));
        return 0x800B3EBC;
    }
    else if ((flags & 0x1000) != 0)
    {
        w_u32(0x800BCE14, 4);
        return 4;
    }
    else if ((flags & 0x2000) != 0)
        mode = 5;
    else if ((flags & 0x4000) != 0)
        mode = 6;
    else
        return 6;
    w_u32(0x800BCE14, mode);
    result = 0x800B3B94 + index * 56;
    w_u32(0x800BCE18, result);
    return result;
}

uint32 sub_80076290(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80076290, "ACE3.BIN");
    state = r_u32(0x800BD094);
    w_u32(state, 1);
    w_u32(state + 4, 0x01000000);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_800762b0(void)
{
    uint32 state;
    FUNCTION_MARKER(0x800762B0, "ACE3.BIN");
    state = r_u32(0x800BD094);
    w_u32(state, 1);
    w_u32(state + 4, 0x02000000);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_800762f0(uint32 mode)
{
    uint32 state, flags;
    FUNCTION_MARKER(0x800762F0, "ACE3.BIN");
    state = r_u32(0x800BD094);
    flags = r_u32(state);
    w_u32(state + 4, mode != 0 ? 2 : 0x00400000);
    w_u32(state + 8, 0);
    w_u32(state, flags | 0x40);
    return state;
}

uint32 sub_80076a38(uint32 position, uint32 target)
{
    uint32 x, lower, span, z, result, y, target_y;
    FUNCTION_MARKER(0x80076A38, "ACE3.BIN");
    x = r_u32(position);
    lower = r_u32(target + 24);
    span = r_u32(target + 32);
    result = (sint32)x < (sint32)lower;
    z = r_u32(position + 8);
    if ((sint32)(lower + span) < (sint32)x)
        result = 1;
    lower = r_u32(target + 28);
    if ((sint32)z < (sint32)lower)
        result = 1;
    span = r_u32(target + 36);
    if ((sint32)(lower + span) < (sint32)z)
        result = 1;
    y = r_u32(position + 4);
    target_y = r_u32(target + 4);
    if ((sint32)(y - target_y) < -409600)
        result = 1;
    return result;
}

uint32 sub_80076ac0(uint32 position)
{
    uint32 y, pitch, roll;
    FUNCTION_MARKER(0x80076AC0, "ACE3.BIN");
    y = r_u32(position + 4);
    if ((sint32)y < 0)
        return 0;
    pitch = r_s16(position + 16);
    roll = r_s16(position + 20);
    pitch -= 1820;
    if ((sint32)pitch < 0)
        pitch = 0 - pitch;
    if ((sint32)pitch >= 228)
        return 0;
    if ((sint32)roll < 0)
        roll = 0 - roll;
    return (sint32)roll < 456;
}

uint32 sub_80076dbc(void)
{
    uint32 state, context;
    FUNCTION_MARKER(0x80076DBC, "ACE3.BIN");
    state = r_u32(0x800BD094);
    context = r_u32(0x800BD098);
    w_u32(state + 16, 0);
    w_u32(context + 36, 0);
    w_u32(state, 0x400);
    w_u32(state + 4, 0x2000);
    w_u32(state + 8, 0);
    return 0x2000;
}

uint32 sub_80076f04(void)
{
    uint32 mode, target;
    FUNCTION_MARKER(0x80076F04, "ACE3.BIN");
    mode = r_u32(0x800BCE14);
    if (mode >= 7)
        return 0;
    target = r_u32(0x80024730 + (mode << 2));
    switch (target)
    {
        case 0x80076F38:
            return sub_80076b18(0, 0);
        case 0x80076F40:
            return sub_80076b18(1, 0);
        case 0x80076F54:
            return sub_80076c54(0, 0);
        case 0x80076F5C:
            return sub_80076c54(1, 0);
        case 0x80076F70:
            return sub_80076360(0);
        case 0x80076F80:
            return sub_80076dec(0, 0);
        case 0x80076F88:
            return sub_80076dec(1, 0);
    }
    return xport_guest_call0(target);
}

uint32 sub_8007599c(uint32 object, uint32 state, uint32 target)
{
    uint32 start[3], end[3];
    uint32 value, result, pointer, normal, height, x, z, pitch, yaw;
    FUNCTION_MARKER(0x8007599C, "ACE3.BIN");
    w_u32(state + 72, 0);
    start[0] = r_u32(target);
    value = r_u32(target + 4);
    start[1] = value + 0xFFF9C000;
    start[2] = r_u32(target + 8);
    end[0] = r_u32(target);
    value = r_u32(target + 4);
    end[1] = value + 0x64000;
    end[2] = r_u32(target + 8);
    result = sub_8009a5d0(0, 0, state + 28, 0x1F800000, start, end, 0);
    if ((sint32)result <= 0x7FFFFFFE)
    {
        pointer = r_u32(0x800BD0C0);
        normal = r_u16(pointer);
        w_u16(state + 20, normal);
        normal = r_u16(pointer + 2);
        w_u16(state + 22, normal);
        value = r_u32(0x800BD0C4);
        normal = r_u16(pointer + 4);
        w_u32(state + 44, value);
        w_u16(state + 24, normal);
    }
    else
    {
        value = r_u32(target);
        w_u32(state + 28, value);
        value = r_u32(target + 4);
        w_u32(state + 32, value);
        value = r_u32(target + 8);
        w_u32(state + 36, value);
    }
    height = r_u32(state + 32);
    w_u32(object + 12, height + 0xFFFBE000);
    x = r_u32(state + 28);
    w_u32(object + 8, x);
    z = r_u32(state + 36);
    w_u16(object + 24, 0);
    pitch = r_u16(object + 24);
    w_u32(object + 16, z);
    yaw = r_u16(target + 18);
    w_u16(object + 28, 0);
    w_u16(object + 26, yaw);
    w_u16(state + 64, pitch);
    yaw = r_u16(object + 26);
    w_u16(state + 66, yaw);
    value = r_u16(object + 28);
    w_u16(state + 68, value);
    return sub_8006ee38(object, state);
}

uint32 sub_800795c8(uint32 enabled)
{
    uint32 mode, scene, table, state, object;
    FUNCTION_MARKER(0x800795C8, "ACE3.BIN");
    if (enabled != 0)
    {
        mode = 0x100;
        sub_80035740(0);
    }
    else
    {
        mode = 0x80;
        sub_80035740(1);
    }
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    state = r_u32(scene + 40);
    object = r_u32(table);
    return sub_800793c4(state, mode, object);
}

uint32 sub_8007961c(uint32 flags)
{
    uint32 mode, game, enabled, scene, table, state, object;
    FUNCTION_MARKER(0x8007961C, "ACE3.BIN");
    if ((flags & 0x6300) != 0)
        mode = 0x400;
    else
    {
        game = r_u32(0x800BCE60);
        enabled = r_u8(game + 927);
        mode = enabled != 0 ? 2 : 1;
    }
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    state = r_u32(scene + 40);
    object = r_u32(table);
    sub_800793c4(state, mode, object);
    return sub_80035740(1);
}

uint32 sub_80079924(void)
{
    uint32 scene, table, state, object;
    FUNCTION_MARKER(0x80079924, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    state = r_u32(scene + 40);
    object = r_u32(table);
    sub_800793c4(state, 0x2000, object);
    return sub_80035740(0);
}

uint32 sub_80076428(uint32 position, uint32 target)
{
    uint32 pitch, roll, x, y, lower, z, span, result, yaw, target_yaw, delta;
    FUNCTION_MARKER(0x80076428, "ACE3.BIN");
    pitch = r_s16(position + 16);
    roll = r_s16(position + 20);
    if ((sint32)pitch >= 683 || (sint32)pitch < -364)
        return 0;
    if ((sint32)roll < 0)
        roll = 0 - roll;
    if ((sint32)roll >= 1366)
        return 0;
    x = r_u32(position);
    y = r_u32(position + 4);
    lower = r_u32(target + 40);
    z = r_u32(position + 8);
    if ((sint32)x < (sint32)lower)
        return 0;
    span = r_u32(target + 48);
    if ((sint32)(lower + span) < (sint32)x)
        return 0;
    lower = r_u32(target + 44);
    if ((sint32)z < (sint32)lower)
        return 0;
    span = r_u32(target + 52);
    if ((sint32)(lower + span) < (sint32)z)
        return 0;
    lower = r_u32(target + 4);
    if ((sint32)lower < (sint32)y)
        return 0;
    result = 1;
    lower = r_u32(target + 24);
    if ((sint32)x < (sint32)lower)
        result = 2;
    span = r_u32(target + 32);
    if ((sint32)(lower + span) < (sint32)x)
        result = 2;
    lower = r_u32(target + 28);
    if ((sint32)z < (sint32)lower)
        result = 2;
    span = r_u32(target + 36);
    if ((sint32)(lower + span) < (sint32)z)
        result = 2;
    yaw = r_s16(position + 18);
    target_yaw = r_s16(target + 18);
    delta = target_yaw - yaw;
    if ((sint32)delta < 8192)
    {
        yaw -= 0x4000;
        delta = target_yaw - yaw;
    }
    if ((sint32)delta >= 8193)
    {
        yaw += 0x4000;
        delta = target_yaw - yaw;
    }
    if ((sint32)delta < 0)
    {
        if ((sint32)(yaw - target_yaw) >= 683)
            result = 2;
    }
    else if ((sint32)delta >= 683)
        result = 2;
    return result;
}

uint32 sub_80074e64(uint32 object, uint32 state, uint32 smooth, uint32 selection)
{
    MATRIX matrix;
    uint32 index, table, target, x, y, z, a, b, c, d, e, rx, ry, rz, value, vx, vy, vz, speed;
    FUNCTION_MARKER(0x80074E64, "ACE3.BIN");
    w_u16(smooth, 0);
    w_u16(smooth + 2, 0);
    w_u16(smooth + 4, 0);
    w_u16(smooth + 6, 2048);
    w_u16(smooth + 8, 0);
    w_u16(smooth + 10, 0);
    w_u32(smooth + 12, 0);
    index = r_u32(selection);
    table = r_u32(0x800BCE5C);
    target = r_u32(table + (index << 2));
    rotation_build(target + 24, 0, &matrix, 0);
    value = r_u32(selection + 4);
    x = (sint32)value >> 6;
    value = r_u32(selection + 8);
    y = (sint32)value >> 6;
    value = r_u32(selection + 12);
    z = (sint32)(value + 0xFFF9C000) >> 6;
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    value = r_u32(target + 8);
    w_u32(object + 8, value + (rx << 6));
    value = r_u32(target + 12);
    w_u32(object + 12, value + (ry << 6));
    value = r_u32(target + 16);
    w_u32(object + 16, value + (rz << 6));
    index = r_u32(selection);
    table = r_u32(0x800BCE5C);
    value = r_u32(table + (index << 2));
    vx = r_u32(value + 56);
    vy = r_u32(value + 60);
    vz = r_u32(value + 64);
    speed = sub_80083f40((sint32)vx >> 6, (sint32)vy >> 6, (sint32)vz >> 6) << 6;
    z = (sint32)speed >> 5;
    w_u32(state + 72, speed);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    w_u32(object + 56, rx << 5);
    w_u32(object + 60, ry << 5);
    w_u32(object + 64, rz << 5);
    value = r_u16(target + 24);
    w_u16(object + 24, value);
    value = r_u16(target + 26);
    w_u16(object + 26, value);
    value = r_u16(target + 28);
    w_u16(object + 28, value);
    return value;
}

uint32 sub_80078fdc(uint32 state)
{
    MATRIX matrix;
    uint32 game, flags, count, factor, pitch, yaw, roll, speed, value, raw, pointer, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078FDC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game) & 0x20000000;
    if (flags != 0)
        return flags;
    count = r_u32(state + 208);
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = r_s16(state + 48);
        yaw = r_u16(state + 22);
        roll = r_u16(state + 24);
        w_u32(state + 228, 0x190000 - (pitch << 6));
        w_u16(state + 214, yaw);
        w_u16(state + 216, roll);
    }
    speed = r_u32(state + 228);
    if ((sint32)speed <= 0x3E7FFF)
    {
        value = factor * 10;
        value = (sint32)value / 4096;
        w_u32(state + 228, speed + (value << 12));
    }
    value = r_s16(state + 216);
    raw = r_u16(state + 216);
    if ((sint32)value >= 17)
        w_u16(state + 216, raw - 16);
    value = r_s16(state + 216);
    raw = r_u16(state + 216);
    if ((sint32)value < -16)
        w_u16(state + 216, raw + 16);
    value = r_s16(state + 216);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value < 16)
        w_u16(state + 216, 0);
    count = r_u32(state + 208);
    pointer = r_u32(state + 52);
    pitch = r_s16(state + 212);
    yaw = r_s16(state + 214);
    roll = r_s16(state + 216);
    w_u32(state + 208, count + 1);
    target_pitch = r_s16(pointer + 16);
    target_yaw = r_s16(pointer + 18);
    target_roll = r_s16(pointer + 20);
    speed = r_u32(state + 228);
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(state + 212, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    w_u32(state + 56, rx << 9);
    w_u32(state + 60, ry << 9);
    w_u16(state + 72, dx);
    w_u16(state + 74, dy);
    w_u16(state + 76, dz);
    value = rz << 9;
    w_u32(state + 64, value);
    return value;
}

uint32 sub_80078d70(uint32 state)
{
    MATRIX matrix;
    uint32 game, flags, count, factor, pitch, yaw, roll, speed, value, raw, pointer, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078D70, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game) & 0x20000000;
    if (flags != 0)
        return flags;
    count = r_u32(state + 208);
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = r_s16(state + 48);
        yaw = r_u16(state + 22);
        roll = r_u16(state + 24);
        w_u32(state + 228, 0x190000 - (pitch << 6));
        w_u16(state + 214, yaw);
        w_u16(state + 216, roll);
    }
    speed = r_u32(state + 228);
    if ((sint32)speed <= 0xAEFFFF)
    {
        value = factor * 20;
        value = (sint32)value / 4096;
        w_u32(state + 228, speed + (value << 12));
    }
    value = r_s16(state + 212);
    raw = r_u16(state + 212);
    if ((sint32)value >= -2047)
        w_u16(state + 212, raw - (uint32)((sint32)factor / 128));
    value = r_s16(state + 216);
    raw = r_u16(state + 216);
    if ((sint32)value >= 17)
        w_u16(state + 216, raw - 16);
    value = r_s16(state + 216);
    raw = r_u16(state + 216);
    if ((sint32)value < -16)
        w_u16(state + 216, raw + 16);
    value = r_s16(state + 216);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value < 16)
        w_u16(state + 216, 0);
    raw = r_u16(state + 214);
    raw += (uint32)((sint32)factor / 128);
    value = (sint32)(raw << 16) >> 16;
    w_u16(state + 214, raw);
    if ((sint32)value >= 8193)
        w_u16(state + 214, raw - 0x4000);
    count = r_u32(state + 208);
    pointer = r_u32(state + 52);
    pitch = r_s16(state + 212);
    yaw = r_s16(state + 214);
    roll = r_s16(state + 216);
    w_u32(state + 208, count + 1);
    target_pitch = r_s16(pointer + 16);
    target_yaw = r_s16(pointer + 18);
    target_roll = r_s16(pointer + 20);
    speed = r_u32(state + 228);
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(state + 212, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    w_u32(state + 56, rx << 9);
    w_u32(state + 60, ry << 9);
    w_u16(state + 72, dx);
    w_u16(state + 74, dy);
    w_u16(state + 76, dz);
    value = rz << 9;
    w_u32(state + 64, value);
    return value;
}

uint32 sub_80078b28(uint32 state)
{
    MATRIX matrix;
    uint32 game, flags, count, factor, pitch, yaw, roll, speed, value, raw, pointer, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078B28, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game) & 0x20000000;
    if (flags != 0)
        return flags;
    count = r_u32(state + 208);
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = r_s16(state + 48);
        yaw = r_u16(state + 22);
        w_u32(state + 228, (0 - pitch) << 6);
        value = yaw + 0x2000;
        w_u16(state + 214, value);
        if ((sint16)value < -8192)
            w_u16(state + 214, yaw + 0x6000);
        value = r_s16(state + 214);
        raw = r_u16(state + 214);
        if ((sint32)value >= 8193)
            w_u16(state + 214, raw - 0x4000);
    }
    speed = r_u32(state + 228);
    if ((sint32)speed <= 0xAEFFFF)
    {
        value = factor * 10;
        value = (sint32)value / 4096;
        w_u32(state + 228, speed + (value << 12));
    }
    value = r_s16(state + 212);
    raw = r_u16(state + 212);
    if ((sint32)value >= -2047)
        w_u16(state + 212, raw - (uint32)((sint32)factor / 128));
    raw = r_u16(state + 214);
    raw += (uint32)((sint32)factor / 128);
    value = (sint32)(raw << 16) >> 16;
    w_u16(state + 214, raw);
    if ((sint32)value >= 8193)
        w_u16(state + 214, raw - 0x4000);
    count = r_u32(state + 208);
    pointer = r_u32(state + 52);
    pitch = r_s16(state + 212);
    yaw = r_s16(state + 214);
    roll = r_s16(state + 216);
    w_u32(state + 208, count + 1);
    target_pitch = r_s16(pointer + 16);
    target_yaw = r_s16(pointer + 18);
    target_roll = r_s16(pointer + 20);
    speed = r_u32(state + 228);
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(state + 212, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    w_u32(state + 56, rx << 9);
    w_u32(state + 60, ry << 9);
    w_u16(state + 72, dx);
    w_u16(state + 74, dy);
    w_u16(state + 76, dz);
    value = rz << 9;
    w_u32(state + 64, value);
    return value;
}

uint32 sub_8007895c(uint32 state)
{
    MATRIX matrix = {0}, adjustment = {0};
    uint16 angles[3];
    uint32 scene, pointer, ticks, sine, value, x, y, z, a, b, c, d, e, rx, ry, rz, flags, pitch, yaw, roll, dx, dy, dz;
    FUNCTION_MARKER(0x8007895C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    pointer = r_u32(state + 52);
    ticks = r_u32(scene + 12);
    w_u16(state + 184, 0);
    sine = r_u16(0x800104AC + ((ticks << 7) & 0x1F80));
    sine = (sint32)(sine << 16) >> 18;
    value = 4096 - sine;
    w_u16(state + 180, 0 - value);
    w_u16(state + 182, value << 1);
    rotation_build(pointer + 16, 0, &matrix, 0);
    rotation_build(state + 180, 0, &adjustment, 0);
    MulMatrix0(&matrix, &adjustment, &matrix);
    x = r_s16(state + 44);
    y = r_s16(state + 46);
    z = r_s16(state + 48);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    w_u32(state + 56, rx << 6);
    flags = r_u32(state);
    w_u32(state + 60, ry << 6);
    w_u32(state + 64, rz << 6);
    if ((flags & 0x100) != 0)
    {
        pointer = r_u32(state + 52);
        sub_800313a8(pointer + 16, state + 180, 0, angles);
        pointer = r_u32(state + 52);
        pitch = r_s16(pointer + 16);
        yaw = r_s16(pointer + 18);
        roll = r_s16(pointer + 20);
    }
    else
    {
        sub_800313a8(state + 20, state + 180, 0, angles);
        pitch = r_s16(state + 20);
        yaw = r_s16(state + 22);
        roll = r_s16(state + 24);
    }
    dx = (uint32)(sint32)(sint16)angles[0] - pitch;
    dy = (uint32)(sint32)(sint16)angles[1] - yaw;
    dz = (uint32)(sint32)(sint16)angles[2] - roll;
    w_u16(state + 72, dx);
    w_u16(state + 74, dy);
    w_u16(state + 76, dz);
    return roll;
}

uint32 sub_80075e08(uint32 object, uint32 state, uint32 control, uint32 smooth, uint32 target, uint32 option)
{
    MATRIX matrix;
    uint32 a, b, c, d, e, rx, ry, rz, y, z, value;
    FUNCTION_MARKER(0x80075E08, "ACE3.BIN");
    sub_8007599c(object, state, target);
    w_u16(state + 48, 0);
    w_u16(state + 50, 0);
    w_u16(state + 52, 0);
    w_u16(state + 56, 0);
    w_u16(state + 58, 0);
    w_u16(state + 60, 0);
    w_u16(state + 76, 4096);
    w_u16(control + 4, 0);
    w_u16(control + 6, 0);
    w_u16(control + 8, 0);
    w_u16(control + 10, 0);
    w_u16(control + 12, 0);
    w_u16(control + 14, 4096);
    w_u16(smooth + 6, 4096);
    w_u16(smooth, 0);
    w_u16(smooth + 2, 0);
    w_u16(smooth + 4, 0);
    w_u16(smooth + 8, 0);
    w_u32(state + 72, 0x20000);
    w_u32(state + 12, 0);
    rotation_build(object + 24, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    rx <<= 5;
    ry <<= 5;
    rz <<= 5;
    w_u32(object + 56, rx);
    w_u32(object + 60, ry);
    w_u32(object + 64, rz);
    if (option != 0)
    {
        y = 0 - 2000;
        z = 5000;
    }
    else
    {
        y = 0 - 3500;
        z = 13000;
    }
    xport_gte_write_data(0, y << 16);
    xport_gte_write_data(1, z);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    rx <<= 12;
    ry <<= 12;
    rz <<= 12;
    value = r_u32(object + 8);
    w_u32(object + 8, value + rx);
    value = r_u32(object + 12);
    w_u32(object + 12, value + ry);
    value = r_u32(object + 16);
    w_u32(object + 16, value + rz);
    return sub_8006ee38(object, state);
}

uint32 sub_8007601c(uint32 object, uint32 state)
{
    uint32 phase, mode, ticks, numerator, increment, multiplier, y_velocity, z_velocity, y, value, first, second, third, fourth, vx, vy, vz, speed, count;
    FUNCTION_MARKER(0x8007601C, "ACE3.BIN");
    phase = r_u32(state + 8);
    if (phase == 0)
    {
        mode = r_u32(state + 4);
        if (mode != 0x04000000)
            return 1;
        w_u32(state + 8, 1);
        w_u32(state + 12, 0);
        w_u16(state + 78, 0);
        w_u16(state + 80, 0);
        w_u32(state, 16);
        return 16;
    }
    if (phase != 1)
        return 1;
    ticks = r_u32(state + 12);
    if ((sint32)ticks >= 301)
        ticks = 300;
    numerator = ticks * 5120;
    increment = (uint32)((sint32)numerator / 300) << 6;
    y_velocity = r_u32(object + 60);
    multiplier = ticks - 300;
    y_velocity *= multiplier;
    z_velocity = r_u32(object + 64);
    z_velocity *= multiplier;
    y = r_u32(object + 12);
    w_u32(object + 60, (sint32)y_velocity / 300);
    w_u32(object + 64, (sint32)z_velocity / 300);
    if ((sint32)y <= 0x0EA5FFFF)
    {
        w_u32(object + 12, y + (uint32)((sint32)increment >> 1));
        value = r_u32(object + 16);
        w_u32(object + 16, value + (increment << 1));
    }
    else
        w_u32(object + 12, 0x0EA60000);
    first = r_u32(object + 56);
    second = r_u32(object + 60);
    third = r_u32(object + 64);
    fourth = r_u32(object + 68);
    w_u32(object + 60, increment);
    sub_8006ee38(object, state);
    vx = r_u32(object + 56);
    vy = r_u32(object + 60);
    vz = r_u32(object + 64);
    speed = sub_80083f40((sint32)(0 - vx) >> 6, (sint32)(0 - vy) >> 6, (sint32)(0 - vz) >> 6) << 6;
    w_u32(state + 72, speed);
    w_u32(object + 56, first);
    w_u32(object + 60, second);
    w_u32(object + 64, third);
    w_u32(object + 68, fourth);
    ticks = r_u32(state + 12);
    w_u32(state + 12, ticks + 1);
    y = r_u32(object + 12);
    if ((sint32)y <= 0)
        return y;
    count = r_u16(state + 78) + 1;
    w_u16(state + 78, count);
    if ((sint16)count >= 301)
    {
        mode = r_u32(state + 4);
        if (mode == 0x04000000)
        {
            w_u16(object + 78, 0xFFFF);
            return 0xFFFFFFFF;
        }
    }
    count = r_s16(state + 78);
    if ((sint32)count < 31)
        return 0x08000000;
    mode = r_u32(state + 4);
    if (mode != 0x08000000)
        return 0x08000000;
    value = r_u32(state) | 0x200;
    w_u32(state, value);
    return value;
}

uint32 sub_80074ba8(uint32 object, uint32 state, uint32 smooth, uint32 target)
{
    uint32 phase, mode, height, yaw, angle, sine, cosine, x, y, z, vx, vz, value, ticks, ratio, amplitude, pitch, numerator, flags;
    FUNCTION_MARKER(0x80074BA8, "ACE3.BIN");
    w_u16(smooth, 0);
    w_u16(smooth + 2, 0);
    w_u16(smooth + 4, 0);
    w_u16(smooth + 6, 2048);
    w_u16(smooth + 8, 0);
    w_u16(smooth + 10, 0);
    w_u32(smooth + 12, 0);
    phase = r_u32(state + 8);
    if (phase == 0)
    {
        mode = r_u32(0x800BCE14);
        height = mode != 0 ? 0xFFD26000 : 0xFFCC2000;
        yaw = r_s16(target + 18);
        angle = yaw & 0x3FFF;
        sine = r_u16(0x800104AC + (angle << 1));
        sine = (sint32)(sine << 16) >> 18;
        cosine = r_u16(0x800104AC + ((angle + 0x1000) << 1));
        cosine = (sint32)(cosine << 16) >> 18;
        x = r_u32(target);
        z = r_u32(target + 8);
        y = r_u32(target + 4);
        w_u32(object + 8, x + sine * (uint32)-1920);
        vx = sine << 4;
        w_u32(object + 12, height + y);
        w_u32(object + 16, z + cosine * (uint32)-1920);
        vz = cosine << 4;
        value = rsin(-318);
        value = (sint32)((0 - value) << 16) >> 12;
        w_u32(object + 60, value);
        w_u16(object + 24, -273);
        w_u32(object + 56, vx);
        w_u32(object + 64, vz);
        w_u16(object + 26, yaw);
        w_u16(object + 28, 0);
        w_u32(state + 72, 0x10000);
        w_u32(state + 12, 0);
        w_u32(state + 8, 1);
    }
    else if (phase == 1)
    {
        ticks = r_u32(state + 12) + 1;
        w_u32(state + 12, ticks);
        if ((sint32)ticks >= 31)
        {
            w_u32(state + 12, 0);
            w_u32(state + 8, 2);
        }
    }
    else if (phase == 2)
    {
        ticks = r_u32(state + 12);
        if ((sint32)ticks < 76)
        {
            numerator = ticks << 12;
            ratio = (sint32)numerator / 75;
        }
        else
            ratio = 4096;
        value = rsin(((sint32)ratio >> 1) + 1024);
        amplitude = ((uint32)((sint32)value >> 1)) + 2048;
        numerator = 0 - amplitude * 182;
        pitch = 0 - amplitude * 409;
        value = (uint32)((sint32)numerator / 4096) - 136;
        pitch = (uint32)((sint32)pitch / 4096) + 136;
        value = rsin(value);
        value = (0 - value) * 0x10000;
        w_u16(object + 24, pitch);
        w_u32(object + 60, (sint32)value >> 12);
        ticks = r_u32(state + 12);
        if ((sint32)ticks >= 121)
            return 1;
        flags = r_u32(state + 16);
        if ((flags & 9) != 0)
            return 1;
        w_u32(state + 12, ticks + 1);
    }
    sub_80031bb4(object + 56, state + 64, 0);
    sub_8006ee38(object, state);
    return 0;
}

uint32 sub_80074878(uint32 object, uint32 state, uint32 option)
{
    uint32 vx, vy, vz, factor, product, flags, limit, roll, magnitude, damping, phase, pitch, ticks, step, raw, amplitude, sine;
    FUNCTION_MARKER(0x80074878, "ACE3.BIN");
    vx = r_u32(object + 56);
    vy = r_u32(object + 60);
    vz = r_u32(object + 64);
    factor = option != 0 ? 3936 : 4084;
    product = vx * factor;
    vx = (sint32)product / 4096;
    product = vz * factor;
    vz = (sint32)product / 4096;
    flags = r_u32(state + 16);
    if ((flags & 15) == 0)
    {
        vy += 669;
        goto apply_velocity;
    }
    limit = (sint32)vy < 1025 ? vy : 1024;
    if ((sint32)limit < 0)
        limit = 0;
    roll = r_s16(object + 28);
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    damping = (sint32)magnitude < 1536 ? (limit >> 3) + 3776 : 3840;
    product = roll * damping;
    w_u16(object + 28, (sint32)product / 4096);
    phase = r_u32(state + 8);
    pitch = r_s16(object + 24);
    if (phase == 0)
    {
        if ((sint32)pitch < 0)
            w_u32(state + 8, 1);
        else if ((sint32)pitch < 182)
        {
            w_u32(state + 8, 3);
            w_u32(state + 12, (pitch << 10) / 182 + 4096);
        }
        else
            w_u32(state + 8, 2);
    }
    phase = r_u32(state + 8);
    if (phase == 1)
    {
        step = (sint32)(182 - pitch) / 2;
        if ((sint32)step < 2)
            step = 2;
        raw = r_u16(object + 24);
        w_u16(object + 24, raw + step);
        if ((sint32)pitch >= 182)
        {
            w_u16(object + 24, 182);
            w_u32(state + 12, 5120);
            w_u32(state + 8, 3);
        }
    }
    else if (phase == 2)
    {
        product = (pitch - 182) * (limit << 1);
        step = (sint32)product / 4096;
        if ((sint32)step < 8)
            step = 8;
        raw = r_u16(object + 24);
        w_u16(object + 24, raw - step);
        if ((sint32)pitch < 183)
        {
            w_u16(object + 24, 182);
            w_u32(state + 12, 5120);
            w_u32(state + 8, 3);
        }
    }
    else if (phase == 3)
    {
        ticks = r_u32(state + 12);
        amplitude = 4096;
        if ((sint32)ticks < 4097)
            amplitude = (sint32)ticks < 0 ? 0 : ticks;
        ticks = r_u32(state + 12);
        sine = rsin(ticks);
        product = sine * 182;
        product = (sint32)product / 4096;
        product *= amplitude;
        pitch = (sint32)product / 4096;
        w_u16(object + 24, pitch);
        ticks = r_u32(state + 12);
        ticks -= option != 0 ? 128 : 64;
        w_u32(state + 12, ticks);
        ticks = r_u32(state + 12);
        if ((sint32)ticks < -512)
        {
            sub_800795c8(1);
            w_u32(state + 8, 4);
        }
    }
apply_velocity:
    w_u32(object + 56, vx);
    w_u32(object + 60, vy);
    w_u32(object + 64, vz);
    return sub_8006ee38(object, state);
}

uint32 sub_80081f94(uint32 position, uint32 vector)
{
    FUNCTION_MARKER(0x80081F94, "ACE3.BIN");
    sub_8007ed9c(3, position, vector, 0);
    return sub_80081d4c(position, vector, 0, 0);
}

uint32 sub_800821ec(uint32 position, uint32 vector, uint32 tick)
{
    uint32 scene, flags, record, value;
    FUNCTION_MARKER(0x800821EC, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene) & 0x100;
    if (flags != 0)
    {
        sub_800829bc(position, vector, 9);
        return sub_80081c30(position, vector, 0, 1);
    }
    value = tick & 1;
    if (value != 0)
        return value;
    record = sub_8007ed9c(4, position, vector, 0);
    flags = r_u32(record);
    w_u32(record, (flags & 0xFFFF01FF) | ((tick & 127) << 9));
    return sub_80081fdc(position, vector, tick, 0, 0);
}

uint32 sub_8008276c(uint32 position, uint32 tick)
{
    uint32 scene, flags, record, value;
    FUNCTION_MARKER(0x8008276C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene) & 0x100;
    if (flags != 0)
        return flags;
    record = sub_8007ece8(2, position, 0);
    value = r_u32(record + 4);
    w_u32(record + 4, (value & 0xFFFFF0FF) | ((tick & 15) << 8));
    return sub_80082594(position, tick, 0);
}

uint32 sub_80074560(uint32 object, uint32 state)
{
    uint32 phase, game, flags, value, raw, magnitude, ticks, velocity, pitch, product, step, roll, result;
    FUNCTION_MARKER(0x80074560, "ACE3.BIN");
    phase = r_u32(state + 8);
    if (phase == 0)
    {
        game = r_u32(0x800BCE60);
        w_u32(state + 12, 0);
        flags = r_u32(game + 36);
        w_u32(state + 8, (flags & 4) != 0 ? 1 : 2);
        value = r_s16(state + 52);
        raw = r_u16(state + 52);
        if (value == 0)
        {
            game = r_u32(0x800BCE60);
            flags = r_u16(game + 36);
            w_u16(state + 52, raw - 64 + (flags & 127));
        }
        else
        {
            magnitude = (sint32)value < 0 ? 0 - value : value;
            if ((sint32)magnitude < 32)
                w_u16(state + 52, raw << 2);
        }
        sub_80081f94(object + 8, object + 56);
        sub_800296b8();
        sub_8002db4c(32);
    }
    else if (phase == 1 || phase == 2)
    {
        flags = r_u32(state + 16);
        if ((flags & 15) == 0)
        {
            ticks = r_u32(state + 12);
            velocity = r_u32(object + 60);
            w_u32(object + 60, velocity + (uint32)((sint32)ticks >> 5) * 669);
        }
        pitch = r_s16(object + 24);
        raw = r_u16(object + 24);
        product = (0 - 4096 - pitch) * (phase == 1 ? 30 : 15);
        step = (sint32)product / 4096;
        w_u16(object + 24, raw + step);
        if (phase == 1)
        {
            roll = r_s16(state + 52);
            roll += (uint32)((sint32)roll / 8);
            if ((sint32)roll >= 513)
                roll = 512;
            if ((sint32)roll < -512)
                roll = 0 - 512;
            w_u16(state + 52, roll);
            raw = r_u16(object + 28);
            w_u16(object + 28, raw + roll);
        }
        else
        {
            ticks = r_u32(state + 12);
            value = r_u16(0x800104AC + ((ticks << 9) & 0x7E00));
            raw = r_u16(object + 28);
            value = (sint32)(value << 16) >> 22;
            w_u16(object + 28, raw + value);
        }
    }
    else if (phase == 16)
    {
        ticks = r_u32(state + 12);
        sub_8008276c(object + 8, ticks);
        flags = r_u32(object);
        w_u32(object, flags & 0xFFFEFFFF);
    }
    ticks = r_u32(state + 12);
    phase = r_u32(state + 8);
    ticks += 1;
    result = phase & 3;
    w_u32(state + 12, ticks);
    if (result == 0)
        return result;
    sub_800821ec(object + 8, object + 56, ticks);
    sub_80029674();
    flags = r_u32(state + 16);
    result = flags & 15;
    if (result == 0)
        return result;
    w_u32(state + 12, 0);
    w_u32(state + 8, 16);
    sub_80081f94(object + 8, object + 56);
    sub_8002963c();
    result = sub_8002db4c(34);
    w_u32(object + 56, 0);
    w_u32(object + 60, 0);
    w_u32(object + 64, 0);
    return result;
}

uint32 sub_8007dbc4(uint32 resource)
{
    uint32 state, base, offset, size, cached_base, current, result;
    FUNCTION_MARKER(0x8007DBC4, "ACE3.BIN");
    state = r_u32(resource);
    base = r_u32(resource + 4);
    offset = r_u32(state + 16);
    size = r_u32(state + 0x2050);
    w_u32(state + 4, base);
    cached_base = r_u32(state + 4);
    w_u32(0x800BD0B8, state);
    w_u32(state + 0x1018, 0);
    w_u32(state + 0x184C, 0xFFFFFFFF);
    w_u32(state + 8, base + (size & 0xFFFFFFFC) - 64);
    if ((sint32)offset >= 0)
        w_u32(state + 16, cached_base + offset);
    current = r_u32(0x800BD0B8);
    offset = r_u32(current + 12);
    result = cached_base + offset;
    if ((sint32)offset >= 0)
        w_u32(current + 12, result);
    return result;
}

uint32 sub_80082504(uint32 position, uint32 vector, uint32 tick, uint32 mode)
{
    uint32 result, record, flags;
    FUNCTION_MARKER(0x80082504, "ACE3.BIN");
    result = tick & 1;
    if (result != 0)
        return result;
    record = sub_8007ed9c(5, position, vector, 0);
    w_u8(record + 2, tick);
    flags = r_u32(record);
    w_u32(record, (flags & 0xFFFF01FF) | ((mode & 127) << 9));
    return sub_800822a4(position, vector, mode, 0, 0);
}

uint32 sub_800829bc(uint32 position, uint32 vector, uint32 kind)
{
    uint32 game, mission, record;
    FUNCTION_MARKER(0x800829BC, "ACE3.BIN");
    if (kind == 1)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission != 42)
            kind = 10;
    }
    record = sub_8007ed9c(6, position, vector, 0);
    w_u8(record + 2, kind);
    return sub_800827e0(position, vector, kind, 0, 0);
}

uint32 sub_80082d38(uint32 position, uint32 vector)
{
    FUNCTION_MARKER(0x80082D38, "ACE3.BIN");
    sub_8007ed9c(8, position, vector, 0);
    return sub_80082c1c(position, vector, 0, 0);
}

uint32 sub_80084978(uint32 ordering, uint32 count)
{
    uint32 expected, index, run, cursor, value, result;
    FUNCTION_MARKER(0x80084978, "ACE3.BIN");
    expected = ordering & 0xFFFFFF;
    index = 1;
    run = 0;
    cursor = ordering + 4;
    while ((sint32)index < (sint32)count)
    {
        value = r_u32(cursor);
        if (value == expected)
            run += 4;
        else
        {
            if ((sint32)run >= 5)
                w_u32(cursor - 4, expected - run);
            run = 0;
        }
        cursor += 4;
        index += 1;
        expected += 4;
    }
    result = index << 2;
    if (run != 0)
    {
        result += ordering;
        w_u32(result - 4, expected - run);
    }
    return result;
}

uint32 sub_800830dc(uint32 start, uint32 end, uint32 parameter, uint32 kind, const uint32 *native_start, const uint32 *native_end)
{
    uint32 ex, sx, ey, ez, sy, sz, dx, dy, dz, shift, record, flags;
    uint32 direction[3];
    FUNCTION_MARKER(0x800830DC, "ACE3.BIN");
    ex = native_end != 0 ? native_end[0] : r_u32(end);
    sx = native_start != 0 ? native_start[0] : r_u32(start);
    ey = native_end != 0 ? native_end[1] : r_u32(end + 4);
    ez = native_end != 0 ? native_end[2] : r_u32(end + 8);
    dx = ex - sx;
    sy = native_start != 0 ? native_start[1] : r_u32(start + 4);
    sz = native_start != 0 ? native_start[2] : r_u32(start + 8);
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
    record = sub_8007eeac(1, start, 0, native_start, direction);
    flags = r_u32(record + 16);
    w_u8(record + 2, kind);
    flags = ((flags & 0xFFFFF000) | shift) & 0xFF000FFF;
    w_u32(record + 16, flags | ((parameter & 0xFFF) << 12));
    return record;
}

uint32 sub_80084474(uint32 ot, uint32 packet, uint32 intensity)
{
    uint32 mode, end, value, link;
    FUNCTION_MARKER(0x80084474, "ACE3.BIN");
    if (intensity == 0)
        return packet;
    mode = 1;
    if ((sint32)intensity < 0)
    {
        mode = 2;
        intensity = 0 - intensity;
    }
    w_u32(packet, ((packet + 16) & 0xFFFFFF) | 0x03000000);
    w_u32(packet + 4, (mode << 5) | 0xE1000218);
    w_u32(packet + 8, 0xE20C4318);
    w_u32(packet + 12, 0);
    xport_gte_write_data(6, 0x66838383);
    xport_gte_write_data(8, 4096 - intensity);
    end = sub_80084318(packet + 16, 0x46D6, 0xFF88FF60, 0x00F00140, 0);
    w_u32(end + 4, 0xE1000220);
    w_u32(end, 0x03000000);
    w_u32(end + 8, 0xE2000000);
    w_u32(end + 12, 0);
    link = r_masked_u32(ot, 7);
    value = packet & 0xFFFFFF;
    w_masked_u32(end, link, 7);
    w_masked_u32(ot, value, 7);
    return end + 16;
}

uint32 sub_80084e6c(uint32 position, uint32 vector)
{
    uint32 root, y, vy, base_y, height, threshold, random, x, z, vx, vz, scene, flags;
    FUNCTION_MARKER(0x80084E6C, "ACE3.BIN");
    root = r_u32(0x800BCEB0);
    y = r_u32(position + 4);
    vy = r_u32(vector + 4);
    base_y = r_u32(root + 4);
    height = (sint32)(y - base_y) >> 12;
    threshold = height + (uint32)((sint32)vy >> 10);
    if ((sint32)threshold < 0)
        return height;
    random = game_random() & 0xFFF;
    if ((sint32)threshold < (sint32)random)
        return 1;
    x = r_u32(position);
    z = r_u32(position + 8);
    vx = r_u32(vector);
    vz = r_u32(vector + 8);
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800004, y + vy);
    w_u32(0x1F800014, vy);
    w_u32(0x1F800000, x + (vx << 1));
    w_u32(0x1F800008, z + (vz << 1));
    w_u32(0x1F800010, vx);
    w_u32(0x1F800018, vz);
    flags = r_u32(scene);
    if ((flags & 2) != 0)
        return sub_800829bc(0x1F800000, 0x1F800010, 8);
    return sub_800827e0(0x1F800000, 0x1F800010, 8, 0, 0);
}

uint32 sub_800831d0(uint32 position, uint32 angles, uint32 scale, uint32 mode)
{
    uint32 a, b, c, d, e, x, y, z, rx, ry, rz, result, scene, flags;
    uint32 start[3], end[3], hit[3];
    const uint32 *finish;
    FUNCTION_MARKER(0x800831D0, "ACE3.BIN");
    rotation_build(angles, 0x1F800000, 0, 0);
    a = r_u32(0x1F800000);
    b = r_u32(0x1F800004);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(0x1F800008);
    d = r_u32(0x1F80000C);
    e = r_u32(0x1F800010);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0xFFF80000);
    xport_gte_write_data(1, 55);
    xport_gte_execute(0x486012);
    x = r_u32(position);
    y = r_u32(position + 4);
    z = r_u32(position + 8);
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
    result = sub_8009a800(0, 0, 0, 0x1F800000, start, end, hit);
    finish = (sint32)result > 0x7FFFFFFE ? end : hit;
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if ((flags & 2) != 0)
        sub_800830dc(0, 0, scale, mode, start, finish);
    if (mode == 1)
    {
        start[0] = x;
        start[1] = y;
        start[2] = z;
    }
    return sub_80083054(0, 0, scale, mode, start, finish);
}

uint32 sub_80087168(uint32 x, uint32 y, uint32 z, uint32 parameters)
{
    uint32 root, record, random, kind, duration;
    FUNCTION_MARKER(0x80087168, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record == 0)
        return 0;
    random = game_random();
    kind = (parameters & 255) + (random & 3);
    w_u16(record + 2, kind);
    duration = (uint32)((sint32)parameters >> 16) + (random & 255);
    w_u16(record, 1);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u16(record + 20, 1);
    w_u16(record + 22, duration);
    w_u32(record + 16, 1024);
    return 1024;
}

uint32 sub_80087520(uint32 x, uint32 y, uint32 z)
{
    FUNCTION_MARKER(0x80087520, "ACE3.BIN");
    sub_8008721c(x, y, z);
    sub_800873e0(x, y, z);
    return sub_80087168(x, y, z, 0x05140010);
}

uint32 sub_80087584(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, kind, duration;
    FUNCTION_MARKER(0x80087584, "ACE3.BIN");
    sub_8008721c(x, y, z);
    sub_800873e0(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record == 0)
        return 0;
    random = game_random();
    kind = (random & 3) + 12;
    duration = (random & 255) + 1200;
    w_u16(record, 0);
    w_u16(record + 2, kind);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u16(record + 20, 0);
    w_u16(record + 22, duration);
    return duration;
}

uint32 sub_80087c18(uint32 x, uint32 y, uint32 z)
{
    FUNCTION_MARKER(0x80087C18, "ACE3.BIN");
    sub_800878b4(x, y, z);
    sub_80087ad0(x, y, z);
    return sub_80087168(x, y, z, 0x07080018);
}

uint32 sub_80093c90(uint32 ordering, uint32 packet, uint32 intensity)
{
    uint32 tag, color;
    FUNCTION_MARKER(0x80093C90, "ACE3.BIN");
    if (intensity == 0)
        return packet;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x6203135F);
    xport_gte_write_data(8, 4096 - intensity);
    xport_gte_execute(0x780010);
    tag = r_u32(ordering);
    w_u32(packet, tag | 0x03000000);
    w_u32(ordering, packet & 0xFFFFFF);
    w_u32(packet + 8, 0xFF88FF60);
    w_u32(packet + 12, 0x00F00140);
    color = xport_gte_read_data(22);
    w_u32(packet + 4, color);
    return packet + 16;
}

uint32 sub_8008812c(uint32 ticks, uint32 record)
{
    uint32 numerator, intensity, value, scene, ordering, packet, root;
    FUNCTION_MARKER(0x8008812C, "ACE3.BIN");
    numerator = (24 - ticks) * 2560;
    intensity = (sint32)numerator / 24;
    if ((sint32)intensity < 256)
    {
        value = r_u16(record + 2);
        w_u16(record, value);
        return value;
    }
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    value = sub_80093c90(ordering + 8, packet, intensity);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, value);
    root = r_u32(0x800BCF90);
    w_u32(root + 0x5C8, 1);
    return 1;
}

uint32 sub_800881bc(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record;
    FUNCTION_MARKER(0x800881BC, "ACE3.BIN");
    sub_80087c7c(x, y, z);
    sub_80087ee0(x, y, z);
    sub_80087168(x, y, z, 0x0BB8001E);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x180000;
    w_u32(record, 0x18FFFF);
    w_u32(record + 16, 0x8008812C);
    return 0x8008812C;
}

uint32 sub_8008721c(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, vertical, index, offset;
    FUNCTION_MARKER(0x8008721C, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    vertical = 0 - (uint32)100;
    if (record != 0)
    {
        random = game_random();
        w_u32(record, 0x28FFFF);
        w_u16(record + 22, 134);
        w_u16(record + 16, ((uint32)((sint32)random >> 2) & 127) - 64);
        w_u16(record + 18, -240);
        w_u16(record + 20, ((uint32)((sint32)random >> 5) & 127) - 64);
        w_u16(record + 24, 528);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 26, (random & 127) + 436);
    }
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        w_u16(record, 0xFFFF);
        w_u16(record + 2, index + (random & 1) + 18);
        w_u32(record + 4, x + (((random & 255) - 128) << 11));
        w_u32(record + 8, y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 11));
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11));
        random = game_random();
        w_u16(record + 18, vertical);
        vertical -= 80;
        offset = index << 6;
        index += 1;
        w_u16(record + 16, (random & 255) - 128);
        w_u16(record + 20, ((uint32)((sint32)random >> 7) & 255) - 128);
        w_u16(record + 22, 133);
        w_u16(record + 24, 256);
        w_u16(record + 26, offset + ((uint32)((sint32)random >> 4) & 127) + 116);
    } while (index < 5);
    return 0;
}

uint32 sub_800873e0(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, nibble, kind;
    FUNCTION_MARKER(0x800873E0, "ACE3.BIN");
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
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
        w_u32(record + 8, y + y_offset);
        w_u16(record + 16, vx);
        w_u32(record + 4, x + x_offset);
        w_u16(record + 20, vz);
        w_u32(record + 12, z + z_offset);
        nibble = random & 15;
        if (index < 8)
        {
            vy -= 384;
            w_u16(record + 2, nibble + 24);
            w_u16(record + 24, 0x1604);
            kind = 139;
            w_u16(record, 0);
            w_u16(record + 26, (nibble << 2) + 600);
        }
        else
        {
            w_u16(record + 26, nibble + 25);
            kind = 405;
            w_u16(record, 0);
            w_u16(record + 2, nibble + 30);
            w_u16(record + 24, 0);
        }
        w_u16(record + 22, kind);
        index += 1;
        w_u16(record + 18, vy);
    } while (index < 16);
    return 0;
}

uint32 sub_800878b4(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, mixed, vertical, level_y, index, kind, duration, px, py, pz, vy;
    FUNCTION_MARKER(0x800878B4, "ACE3.BIN");
    vertical = 0;
    level_y = y;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        w_u16(record + 18, vertical);
        vertical -= 220;
        w_u32(record + 8, level_y);
        level_y -= 0x100000;
        kind = (index << 1) + (random & 7) + 40;
        duration = (index << 8) + (random & 255) + 500;
        index += 1;
        w_u16(record, 0xFFFF);
        w_u16(record + 2, kind);
        w_u16(record + 16, (random & 255) - 128);
        w_u16(record + 20, ((uint32)((sint32)random >> 7) & 255) - 128);
        w_u16(record + 22, 135);
        w_u16(record + 24, 528);
        w_u32(record + 4, x);
        w_u32(record + 12, z);
        w_u16(record + 26, duration);
    } while (index < 3);
    vertical = 0 - (uint32)140;
    level_y = y;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
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
        w_u16(record, 0xFFFF);
        w_u16(record + 16, (mixed & 255) - 128);
        vy += (uint32)((sint32)mixed >> 3) & 255;
        w_u16(record + 20, ((uint32)((sint32)mixed >> 6) & 255) - 128);
        w_u16(record + 22, 133);
        w_u16(record + 24, 256);
        w_u16(record + 2, kind);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        w_u16(record + 18, vy);
        w_u16(record + 26, duration);
    } while (index < 8);
    return 0;
}

uint32 sub_80087ad0(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x80087AD0, "ACE3.BIN");
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
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
        w_u32(record + 8, y + y_offset);
        w_u16(record + 16, vx);
        w_u32(record + 4, x + x_offset);
        w_u16(record + 20, vz);
        w_u32(record + 12, z + z_offset);
        if (index < 15)
        {
            vy -= (index << 6) + 400;
            w_u16(record + 2, (random & 15) + index + 24);
            w_u16(record + 24, 0x1804);
            w_u16(record + 26, (random & 255) + 700);
            kind = 141;
            w_u16(record, 0);
        }
        else
        {
            w_u16(record + 26, (random & 15) + 25);
            kind = 405;
            w_u16(record, 0);
            w_u16(record + 2, (random & 15) + 30);
            w_u16(record + 24, 0);
        }
        w_u16(record + 22, kind);
        index += 1;
        w_u16(record + 18, vy);
    } while (index < 20);
    return 0;
}

uint32 sub_80087c7c(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, vertical, level_y, index, vy, kind, duration;
    FUNCTION_MARKER(0x80087C7C, "ACE3.BIN");
    vertical = 0 - (uint32)384;
    level_y = y;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        w_u16(record, 0xFFFF);
        w_u16(record + 2, index + 42);
        w_u32(record + 4, x + (((random & 255) - 128) << 13));
        w_u32(record + 8, level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 13));
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 13));
        random = game_random();
        vy = vertical - 128;
        vertical -= 100;
        level_y -= 0x100000;
        index += 1;
        w_u16(record + 16, (random & 255) - 128);
        vy += (uint32)((sint32)random >> 3) & 255;
        w_u16(record + 20, ((uint32)((sint32)random >> 6) & 255) - 128);
        w_u16(record + 22, 134);
        w_u16(record + 24, 528);
        w_u16(record + 18, vy);
        w_u16(record + 26, (random & 255) + 900);
    } while (index < 3);
    vertical = 0 - (uint32)340;
    level_y = y + 0x1F4000;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        kind = (index << 1) + (random & 7) + 24;
        w_u16(record, 0xFFFF);
        w_u32(record + 4, x + (((random & 255) - 128) << 14));
        w_u32(record + 8, level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 14));
        w_u16(record + 2, kind);
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 14));
        random = game_random();
        w_u16(record + 18, vertical);
        vertical -= 98 + (random & 7);
        level_y -= 0x50000;
        duration = (index << 7) + ((uint32)((sint32)random >> 2) & 127) + 496;
        index += 1;
        w_u16(record + 16, ((random & 255) - 128) << 1);
        w_u16(record + 20, (((uint32)((sint32)random >> 7) & 255) - 128) << 1);
        w_u16(record + 22, 133);
        w_u16(record + 24, 256);
        w_u16(record + 26, duration);
    } while (index < 9);
    return 0;
}

uint32 sub_80087ee0(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, mixed, vertical, index, kind, duration, px, py, pz, vy;
    FUNCTION_MARKER(0x80087EE0, "ACE3.BIN");
    vertical = 0 - (uint32)300;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
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
        w_u16(record, 10);
        w_u16(record + 16, ((mixed & 255) - 128) << 2);
        w_u16(record + 20, (((uint32)((sint32)mixed >> 6) & 255) - 128) << 2);
        w_u16(record + 22, 150);
        w_u16(record + 24, 0x1C04);
        w_u16(record + 2, kind);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        w_u16(record + 18, vy);
        w_u16(record + 26, duration);
    } while (index < 16);
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        kind = index + 50;
        duration = (index << 1) + (mixed & 31) + 40;
        index += 1;
        px = x + (((random & 255) - 128) << 12);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        w_u16(record + 2, kind);
        w_u16(record + 16, ((mixed & 255) - 128) << 1);
        w_u16(record + 18, ((((uint32)((sint32)mixed >> 3) & 255) - 128) << 1) - 256);
        w_u16(record + 20, (((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        w_u16(record + 22, 149);
        w_u16(record, 0);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        w_u16(record + 24, 0);
        w_u16(record + 26, duration);
    } while (index < 8);
    return 0;
}

uint32 sub_8008762c(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x8008762C, "ACE3.BIN");
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
        random = game_random();
        vx = (random & 255) - 128;
        y_offset = vx << 8;
        vz = ((uint32)((sint32)random >> 6) & 255) - 128;
        w_u16(record + 16, vx);
        x_offset = vz << 8;
        low = (uint32)((sint32)random >> 3) & 255;
        w_u16(record + 20, vz);
        z_offset = (low - 128) << 8;
        vy = low - 272;
        w_u32(record + 8, y + y_offset);
        w_u32(record + 4, x + x_offset);
        w_u32(record + 12, z + z_offset);
        if (index < 4)
        {
            w_u16(record + 2, (random & 15) + 26);
            w_u16(record + 24, 0x1804);
            w_u16(record + 26, (random & 127) + 280);
            kind = 141;
            w_u16(record, 0);
        }
        else
        {
            w_u16(record + 26, (random & 15) + 25);
            kind = 400;
            w_u16(record, 0);
            w_u16(record + 2, (random & 15) + 30);
            w_u16(record + 24, 0);
        }
        w_u16(record + 22, kind);
        index += 1;
        w_u16(record + 18, vy);
    } while (index < 8);
    return 0;
}

uint32 sub_8008775c(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, duration;
    FUNCTION_MARKER(0x8008775C, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record == 0)
        return 0;
    random = game_random();
    w_u16(record, 0xFFFF);
    w_u16(record + 2, (random & 7) + 20);
    duration = (random & 127) + 220;
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u16(record + 22, 0);
    w_u16(record + 24, 256);
    w_u16(record + 26, duration);
    return duration;
}

uint32 sub_800877fc(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, duration;
    FUNCTION_MARKER(0x800877FC, "ACE3.BIN");
    sub_8008775c(x, y, z);
    sub_8008762c(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record == 0)
        return 0;
    random = game_random();
    w_u16(record + 2, (random & 3) + 10);
    w_u16(record + 20, 1);
    duration = (random & 127) + 600;
    w_u16(record, 0);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u16(record + 22, duration);
    w_u16(record + 16, random);
    random = game_random();
    w_u16(record + 18, random);
    return random;
}

uint32 sub_80088254(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, mixed, index, kind, px, py, pz;
    FUNCTION_MARKER(0x80088254, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    index = 0;
    if (record != 0)
    {
        random = game_random();
        w_u16(record, ~(random & 3));
        w_u16(record + 2, (random & 31) + 32);
        w_u32(record + 4, x + (((random & 255) - 128) << 12));
        w_u32(record + 8, y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12));
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12));
        random = game_random();
        w_u16(record + 16, (random & 127) - 64);
        w_u16(record + 18, ((uint32)((sint32)random >> 3) & 127) - 320);
        w_u16(record + 20, ((uint32)((sint32)random >> 6) & 127) - 64);
        w_u16(record + 22, 132);
        w_u16(record + 24, 528);
        w_u16(record + 26, (random & 255) + 600);
    }
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
        index += 1;
        random = game_random();
        kind = (random & 31) + 48;
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        px = x + (((random & 255) - 128) << 12);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        w_u16(record, 8);
        w_u16(record + 16, (mixed & 127) - 64);
        w_u16(record + 18, ((uint32)((sint32)mixed >> 3) & 127) - 560);
        w_u16(record + 20, ((uint32)((sint32)mixed >> 6) & 127) - 64);
        w_u16(record + 22, 141);
        w_u16(record + 24, 0x1804);
        w_u16(record + 26, (mixed & 255) + 700);
        w_u16(record + 2, kind);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
    } while (index < 3);
    return 0;
}

uint32 sub_80088454(uint32 ticks, uint32 object)
{
    uint32 random, root, record, value, z, x, y, index, scene, result;
    FUNCTION_MARKER(0x80088454, "ACE3.BIN");
    if ((sint32)ticks < 38 && (ticks & 3) == 0)
    {
        random = game_random();
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record != 0)
        {
            w_u16(record, 0);
            w_u16(record + 2, (random & 31) + 30);
            value = r_u32(object + 4);
            w_u32(record + 4, value - 0x800000);
            value = r_u32(object + 8);
            w_u32(record + 8, value);
            z = r_u32(object + 12);
            w_u16(record + 16, (random & 255) - 128);
            w_u16(record + 18, ((uint32)((sint32)random >> 3) & 255) - 144);
            w_u16(record + 20, ((uint32)((sint32)random >> 6) & 255) - 128);
            w_u16(record + 22, 136);
            w_u16(record + 24, 0x1704);
            w_u16(record + 26, (random & 255) + 200);
            w_u32(record + 12, z);
        }
    }
    if (ticks != 90)
        return 90;
    z = r_u32(object + 12);
    x = r_u32(object + 4);
    y = r_u32(object + 8);
    x -= 0x200000;
    y += 0x5E6000;
    index = 0;
    do
    {
        sub_80088254(x, y, z);
        x -= 0x140000;
        index += 1;
        y -= 0x78000;
    } while (index < 5);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result != 0)
        return sub_8002f460(24, object + 4, 1, 0);
    return result;
}

uint32 sub_800885dc(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index;
    FUNCTION_MARKER(0x800885DC, "ACE3.BIN");
    sub_80087584(x + 0x1E000, y + 0x17C000, z);
    index = 0;
    do
    {
        random = game_random();
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            break;
        w_u16(record + 2, (random & 15) + 40);
        w_u16(record, 0);
        w_u32(record + 4, (index & 1) != 0 ? x + 0x400000 : x - 0x800000);
        index += 1;
        w_u16(record + 16, (random & 255) - 128);
        w_u16(record + 18, ((uint32)((sint32)random >> 3) & 255) - 176);
        w_u16(record + 20, ((uint32)((sint32)random >> 6) & 255) - 128);
        w_u16(record + 22, 136);
        w_u16(record + 24, 0x1804);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 26, (random & 255) + 300);
    } while (index < 8);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x5B0000;
    w_u32(record, 0x5BFFFF);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80088454);
    return 0x80088454;
}

uint32 sub_80088748(uint32 ticks, uint32 object)
{
    uint32 scene, flags, delta, spread, z, x, y, index, root, record, random, px, pz;
    FUNCTION_MARKER(0x80088748, "ACE3.BIN");
    if (ticks == 80)
    {
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene);
        if ((flags & 2) != 0)
            sub_8002f460(24, object + 4, 1, 0);
    }
    delta = ticks - 80;
    if (delta < 16)
    {
        spread = (((delta << 3) - delta) * 32 + delta) << 11;
        z = r_u32(object + 12);
        x = r_u32(object + 4);
        y = r_u32(object + 8);
        x -= spread;
        y += 0x3E8000;
        index = 0;
        do
        {
            root = r_u32(0x800BCF90);
            record = sub_8007fdb4(root + 0x600, 3, 7);
            if (record == 0)
                break;
            random = game_random();
            w_u16(record, 0xFFFF);
            w_u16(record + 2, (random & 15) + 20);
            px = x + (((random & 255) - 128) << 11);
            pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11);
            w_u32(record + 12, pz);
            w_u16(record + 24, 0x130);
            w_u32(record + 4, px);
            w_u32(record + 8, y);
            w_u16(record + 22, 0);
            w_u16(record + 26, (random & 255) + 320);
            if (index == 1)
                x += spread << 1;
            z += (index & 1) != 0 ? 0x5DC000 : 0xFFA24000;
            index += 1;
        } while (index < 4);
    }
    if ((sint32)ticks >= 80)
        return 0;
    random = game_random();
    x = r_u32(object + 4);
    y = r_u32(object + 8);
    z = r_u32(object + 12);
    y += ((ticks << 1) + ticks) << 14;
    if ((random & 1) != 0)
        z -= 0x5DC000;
    x += ((random & 0xFFF) - 2048) << 11;
    if ((random & 2) != 0)
        return sub_8008775c(x, y, z);
    if ((ticks & 3) != 0)
        return sub_8008762c(x, y, z);
    return sub_800873e0(x, y + 0x50000, z);
}

uint32 sub_80088968(uint32 x, uint32 y, uint32 z)
{
    uint32 level_y, level_z, index, root, record;
    FUNCTION_MARKER(0x80088968, "ACE3.BIN");
    level_y = y + 0xC8000;
    level_z = z;
    index = 0;
    do
    {
        sub_80087584(x, level_y + 0xC8000, level_z);
        sub_8008721c(x - 0x708000, level_y, level_z);
        sub_8008721c(x + 0x708000, level_y, level_z);
        index += 1;
        level_z -= 0x5DC000;
    } while (index < 2);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x610000;
    w_u32(record, 0x61FFFF);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80088748);
    return 0x80088748;
}

uint32 sub_80088a84(uint32 ticks, uint32 object)
{
    uint32 result, x, y, z, index, scene;
    FUNCTION_MARKER(0x80088A84, "ACE3.BIN");
    result = (uint32)r_s16(object + 2) - 1;
    if (ticks != result)
        return result;
    y = r_u32(object + 8);
    x = r_u32(object + 4);
    z = r_u32(object + 12);
    x -= 0x1F4000;
    index = 0;
    do
    {
        sub_80088254(x, y, z);
        index += 1;
        x += 0xFA000;
    } while (index < 4);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result != 0)
        return sub_8002f460(24, object + 4, 1, 0);
    return result;
}

uint32 sub_80088b48(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, mixed, vertical, level_y, index, kind, duration, px, py, pz;
    FUNCTION_MARKER(0x80088B48, "ACE3.BIN");
    vertical = 0 - (uint32)100;
    level_y = y;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        w_u16(record + 18, vertical);
        vertical -= 220;
        w_u32(record + 8, level_y);
        level_y -= 0x100000;
        kind = (index << 1) + (random & 7) + 40;
        duration = (index << 8) + (random & 255) + 500;
        index += 1;
        w_u16(record, 0xFFFF);
        w_u16(record + 2, kind);
        w_u16(record + 16, (random & 255) - 128);
        w_u16(record + 20, ((uint32)((sint32)random >> 7) & 255) - 128);
        w_u16(record + 22, 135);
        w_u16(record + 24, 528);
        w_u32(record + 4, x);
        w_u32(record + 12, z);
        w_u16(record + 26, duration);
    } while (index < 2);
    vertical = 0 - (uint32)100;
    level_y = y;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            return 0;
        random = game_random();
        kind = index + (random & 3) + 24;
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        w_u16(record + 18, vertical);
        vertical -= 110 + (mixed & 7);
        py = level_y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12);
        level_y -= 0x5A000;
        duration = (index << 7) + ((uint32)((sint32)mixed >> 4) & 127) + 240;
        index += 1;
        px = x + (((random & 255) - 128) << 12);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12);
        w_u16(record, 0xFFFF);
        w_u16(record + 16, (mixed & 255) - 128);
        w_u16(record + 20, ((uint32)((sint32)mixed >> 7) & 255) - 128);
        w_u16(record + 22, 133);
        w_u16(record + 24, 256);
        w_u16(record + 2, kind);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        w_u16(record + 26, duration);
    } while (index < 5);
    return 0;
}

uint32 sub_80088d60(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index, low, y_offset, x_offset, z_offset, vx, vy, vz, kind;
    FUNCTION_MARKER(0x80088D60, "ACE3.BIN");
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
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
        w_u32(record + 8, y + y_offset);
        w_u16(record + 16, vx);
        w_u32(record + 4, x + x_offset);
        w_u16(record + 20, vz);
        w_u32(record + 12, z + z_offset);
        if (index < 12)
        {
            vy -= index << 5;
            w_u16(record, 8);
            w_u16(record + 2, (random & 15) + index + 44);
            w_u16(record + 24, 0x1804);
            w_u16(record + 26, (random & 255) + 600);
            kind = 141;
        }
        else
        {
            w_u16(record + 26, (random & 15) + 25);
            kind = 405;
            w_u16(record, 0);
            w_u16(record + 2, (random & 15) + 36);
            w_u16(record + 24, 0);
        }
        w_u16(record + 22, kind);
        index += 1;
        w_u16(record + 18, vy);
    } while (index < 18);
    return 0;
}

uint32 sub_80088ea8(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, scene, result;
    FUNCTION_MARKER(0x80088EA8, "ACE3.BIN");
    sub_80088b48(x, y, z);
    sub_80088d60(x, y, z);
    sub_80087168(x, y, z, 0x07080018);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record != 0)
    {
        w_u32(record + 4, x + 0x384000);
        w_u32(record, 0x79FFFF);
        w_u32(record + 8, y + 0x3E8000);
        w_u32(record + 12, z);
        w_u32(record + 16, 0x80088A84);
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x03840000;
    w_u32(record, 0x03840001);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80088fbc(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, scene, result;
    FUNCTION_MARKER(0x80088FBC, "ACE3.BIN");
    sub_8008721c(x, y, z);
    sub_800873e0(x, y, z);
    sub_80087168(x, y, z, 0x07080018);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record != 0)
    {
        w_u32(record + 4, x - 0x258000);
        w_u32(record + 12, z + 0x190000);
        w_u32(record, 0x8DFFFF);
        w_u32(record + 8, y + 0x3E8000);
        w_u32(record + 16, 0x80088A84);
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 3600;
    w_u16(record + 2, 3600);
    w_u16(record, 0);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_800890d8(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, scene, result;
    FUNCTION_MARKER(0x800890D8, "ACE3.BIN");
    sub_8008721c(x, y, z);
    sub_800873e0(x, y, z);
    sub_80087168(x, y, z, 0x07080018);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record != 0)
    {
        w_u32(record + 8, y + 0x1EA000);
        w_u32(record, 0x97FFFF);
        w_u32(record + 4, x);
        w_u32(record + 12, z - 0x258000);
        w_u32(record + 16, 0x80088A84);
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 3600;
    w_u16(record + 2, 3600);
    w_u16(record, 0);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80089c40(uint32 ticks, uint32 object)
{
    uint32 result, scene, flags, mode, table, player;
    FUNCTION_MARKER(0x80089C40, "ACE3.BIN");
    result = ticks & 7;
    if (result != 0)
        return result;
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    mode = (uint32)((sint32)ticks >> 3) & 1;
    if ((flags & 2) == 0)
        return sub_80083c3c(object + 4, mode, 3, 0);
    table = r_u32(0x800BCE5C);
    player = r_u32(table);
    result = sub_80083dfc(object + 4, player + 8, 0, 0);
    if ((sint32)result > 0x7FFF)
        return result;
    return sub_80083ca4(object + 4, mode, 3);
}

uint32 sub_80089cd8(uint32 ticks, uint32 object)
{
    uint32 result, scene, flags, mode, table, player;
    FUNCTION_MARKER(0x80089CD8, "ACE3.BIN");
    result = ticks & 7;
    if (result != 0)
        return result;
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    mode = (uint32)((sint32)ticks >> 3) & 1;
    if ((flags & 2) == 0)
        return sub_80083c3c(object + 4, mode, 2, 0);
    table = r_u32(0x800BCE5C);
    player = r_u32(table);
    result = sub_80083dfc(object + 4, player + 8, 0, 0);
    if ((sint32)result > 0x7FFF)
        return result;
    return sub_80083ca4(object + 4, mode, 2);
}

uint32 sub_800891ec(uint32 ticks, uint32 object)
{
    uint32 random, x, y, z, scene, flags, delta, result, offset;
    FUNCTION_MARKER(0x800891EC, "ACE3.BIN");
    if ((sint32)ticks < 40 && (ticks & 3) == 0)
    {
        random = game_random();
        if ((sint32)ticks < 20 || (random & 1) != 0)
        {
            x = r_u32(object + 4);
            x += ((random & 255) - 128) << 11;
            z = r_u32(object + 12);
            y = r_u32(object + 8);
            sub_8008762c(x, y, z + ((random & 1) << 22));
        }
    }
    if (ticks == 82)
    {
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene);
        if ((flags & 2) != 0)
            sub_8002f460(24, object + 4, 1, 0);
    }
    delta = ticks - 82;
    result = ticks & 1;
    if (delta >= 11 || result == 0)
        return result;
    offset = (sint32)(delta << 22) / 10;
    y = r_u32(object + 8);
    x = r_u32(object + 4);
    y += 0x2D0000;
    z = r_u32(object + 12);
    return sub_80088254(x, y, z + offset);
}

uint32 sub_800892fc(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record;
    FUNCTION_MARKER(0x800892FC, "ACE3.BIN");
    sub_80087520(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x5D0000;
    w_u32(record, 0x5DFFFF);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x800891EC);
    return 0x800891EC;
}

uint32 sub_80089378(uint32 ticks, uint32 object)
{
    uint32 random, x, y, z, scene, flags, delta, result, offset;
    FUNCTION_MARKER(0x80089378, "ACE3.BIN");
    if ((sint32)ticks < 40 && (ticks & 3) == 0)
    {
        random = game_random();
        if ((sint32)ticks < 20 || (random & 1) != 0)
        {
            x = r_u32(object + 4);
            x += (random & 1) << 22;
            z = r_u32(object + 12);
            y = r_u32(object + 8);
            sub_8008762c(x, y, z + (((random & 255) - 128) << 11));
        }
    }
    if (ticks == 82)
    {
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene);
        if ((flags & 2) != 0)
            sub_8002f460(24, object + 4, 1, 0);
    }
    delta = ticks - 82;
    result = ticks & 1;
    if (delta >= 11 || result == 0)
        return result;
    offset = (sint32)(delta << 22) / 10;
    y = r_u32(object + 8);
    z = r_u32(object + 12);
    y += 0x2D0000;
    x = r_u32(object + 4);
    return sub_80088254(x + offset, y, z);
}

uint32 sub_80089488(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record;
    FUNCTION_MARKER(0x80089488, "ACE3.BIN");
    sub_80087520(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x5D0000;
    w_u32(record, 0x5DFFFF);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089378);
    return 0x80089378;
}

uint32 sub_80089504(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, mixed, index, header, kind, px, py, pz, duration;
    FUNCTION_MARKER(0x80089504, "ACE3.BIN");
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
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
        w_u32(record + 4, px);
        w_u16(record, header);
        w_u16(record + 16, ((mixed & 255) - 128) << 1);
        w_u16(record + 18, (((uint32)((sint32)mixed >> 3) & 255) - 128) << 1);
        w_u16(record + 20, (((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        w_u16(record + 22, 133);
        w_u16(record + 24, 256);
        w_u16(record + 2, kind);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        w_u16(record + 26, (mixed & 255) + 300);
    } while (index < 6);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    index = 0;
    if (record != 0)
    {
        random = game_random();
        w_u16(record, 0);
        w_u16(record + 2, (random & 3) + 20);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 20, 0);
        w_u16(record + 22, (random & 255) + 1600);
    }
    duration = 580;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        kind = index + 50;
        if (record == 0)
            return kind;
        w_u16(record, 0);
        w_u16(record + 2, kind);
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        px = x + (((random & 255) - 128) << 11);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 11);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 11);
        w_u16(record + 16, (mixed & 255) - 128);
        w_u16(record + 18, ((uint32)((sint32)mixed >> 3) & 255) - 128);
        w_u16(record + 20, ((uint32)((sint32)mixed >> 6) & 255) - 128);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        if ((index & 1) != 0)
        {
            w_u16(record + 22, 135);
            w_u16(record + 24, 0x1804);
            w_u16(record + 26, duration);
        }
        else
        {
            w_u16(record + 22, 384);
            w_u16(record + 24, 0);
            w_u16(record + 26, index + 30);
        }
        index += 1;
        duration += 32;
    } while (index < 24);
    return 0;
}

uint32 sub_8008978c(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, scene, result;
    FUNCTION_MARKER(0x8008978C, "ACE3.BIN");
    sub_80087520(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record != 0)
    {
        w_u32(record + 4, x + 0xC8000);
        w_u32(record + 12, z - 0x190000);
        w_u32(record, 0x5BFFFF);
        w_u32(record + 8, y + 0xB4000);
        w_u32(record + 16, 0x80088A84);
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x012C0000;
    w_u32(record, 0x012C0001);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80089880(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, scene, result;
    FUNCTION_MARKER(0x80089880, "ACE3.BIN");
    sub_80087520(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record != 0)
    {
        w_u32(record + 4, x + 0xFA000);
        w_u32(record + 12, z - 0x190000);
        w_u32(record, 0x59FFFF);
        w_u32(record + 8, y + 0x226000);
        w_u32(record + 16, 0x80088A84);
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x03840000;
    w_u32(record, 0x03840001);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80089974(uint32 x, uint32 y, uint32 z)
{
    uint32 scene, result, root, record;
    FUNCTION_MARKER(0x80089974, "ACE3.BIN");
    sub_800877fc(x, y, z);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x03840000;
    w_u32(record, 0x03840000);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80089a04(uint32 x, uint32 y, uint32 z)
{
    uint32 scene, result, root, record;
    FUNCTION_MARKER(0x80089A04, "ACE3.BIN");
    sub_80087520(x, y, z);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x03840000;
    w_u32(record, 0x03840000);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089CD8);
    return 0x80089CD8;
}

uint32 sub_80089a94(uint32 x, uint32 y, uint32 z)
{
    uint32 scene, result, root, record;
    FUNCTION_MARKER(0x80089A94, "ACE3.BIN");
    sub_80087520(x, y, z);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x23280000;
    w_u32(record, 0x23280000);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089C40);
    return 0x80089C40;
}

uint32 sub_80089b24(uint32 x, uint32 y, uint32 z)
{
    uint32 scene, result, root, record;
    FUNCTION_MARKER(0x80089B24, "ACE3.BIN");
    sub_80087c18(x, y, z);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene) & 2;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x36C4, 1, 5);
    if (record == 0)
        return 0x15180000;
    w_u32(record, 0x15180000);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    w_u32(record + 16, 0x80089CD8);
    return 0x80089CD8;
}

uint32 sub_8008cd80(uint32 object, uint32 shift, uint32 scale)
{
    uint32 x, y, z, angle, radius, packed, sine, cosine, a, b, numerator, depth, distance, size, screen, tx, ty;
    FUNCTION_MARKER(0x8008CD80, "ACE3.BIN");
    x = r_s16(object + 32);
    z = r_s16(object + 36);
    x <<= shift & 31;
    y = r_s16(object + 34);
    z <<= shift & 31;
    y <<= shift & 31;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    angle = r_u32(object + 44);
    radius = r_u32(object + 40);
    xport_gte_execute(0x180001);
    packed = r_u32(0x800B7104 + ((angle & 0xFFF) << 2));
    radius <<= shift & 31;
    cosine = (sint32)packed >> 16;
    sine = (sint32)(packed << 16) >> 16;
    a = sine * radius;
    b = cosine * radius;
    numerator = scale * radius;
    w_u16(object + 56, radius);
    sine = (sint32)a >> 12;
    cosine = (sint32)b >> 12;
    depth = xport_gte_read_data(27);
    distance = (sint32)depth >> (shift & 31);
    if (distance - 31 >= 0x77D9)
        return 0;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)depth;
    if ((sint32)size < 3)
        return 0;
    screen = xport_gte_read_data(14);
    w_u32(object + 84, screen);
    w_u32(object + 80, (sint32)distance >> 3);
    w_u16(object + 52, sine);
    w_u16(object + 60, 0 - sine);
    w_u16(object + 64, cosine);
    w_u16(object + 48, cosine);
    w_u16(object + 62, 0);
    w_u16(object + 58, 0);
    w_u16(object + 54, 0);
    w_u16(object + 50, 0);
    tx = xport_gte_read_data(25);
    ty = xport_gte_read_data(26);
    sub_80086948(object + 48);
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, depth);
    return size;
}

uint32 sub_8008cefc(uint32 state, uint32 object, uint32 count, uint32 angle, uint32 prior_result)
{
    uint32 packet, cursor, center, table, scratch, phase, index, value, step, next, ordering, tag, head;
    FUNCTION_MARKER(0x8008CEFC, "ACE3.BIN");
    if ((sint32)count <= 0)
        return prior_result;
    phase = 0;
    index = 0;
    packet = r_u32(state + 4);
    scratch = object + 88;
    cursor = packet + 8;
    w_u32(packet + 4, 0xE1000240);
    w_u32(packet, (cursor & 0xFFFFFF) | 0x01000000);
    center = r_u32(object + 84);
    w_u32(object + 88, 0);
    w_u16(scratch + 4, 4096);
    value = r_u32(0x800B7104 + ((angle << 1) & 0x3FFC));
    w_u16(scratch + 10, 64);
    w_u16(scratch + 12, (sint32)value >> 16);
    w_u16(scratch + 8, value);
    value = r_u32(0x800B7104 + ((angle & 0xFFF) << 2));
    w_u16(scratch + 18, 150);
    w_u16(scratch + 20, (sint32)value >> 16);
    step = 4096 / (sint32)count;
    w_u16(scratch + 16, value);
    do
    {
        value = r_u32(scratch);
        xport_gte_write_data(0, value);
        value = r_u32(scratch + 4);
        xport_gte_write_data(1, value);
        value = r_u32(object + 96);
        xport_gte_write_data(2, value);
        value = r_u32(object + 100);
        xport_gte_write_data(3, value);
        value = r_u32(object + 104);
        xport_gte_write_data(4, value);
        value = r_u32(object + 108);
        xport_gte_write_data(5, value);
        phase += step;
        w_u32(cursor + 24, center);
        xport_gte_execute(0x280030);
        value = r_u32(0x800B7104 + ((phase & 0xFFF) << 2));
        w_u16(scratch + 4, (sint32)value >> 16);
        w_u16(scratch, value);
        value = r_u32(0x800B7104 + (((phase + (uint32)((sint32)angle >> 1)) & 0xFFF) << 2));
        next = cursor + 36;
        w_u16(scratch + 12, (sint32)value >> 16);
        w_u16(scratch + 8, value);
        value = r_u32(0x800B7104 + (((phase + angle) & 0xFFF) << 2));
        w_u16(scratch + 20, (sint32)value >> 16);
        w_u16(scratch + 16, value);
        w_u32(cursor, (next & 0xFFFFFF) | 0x08000000);
        w_u32(cursor + 4, 0x3A242828);
        w_u32(cursor + 12, 0x001C1F1F);
        w_u32(cursor + 20, 0x00050707);
        w_u32(cursor + 28, 0x00010303);
        value = xport_gte_read_data(12);
        w_u32(cursor + 8, value);
        value = xport_gte_read_data(13);
        w_u32(cursor + 16, value);
        value = xport_gte_read_data(14);
        w_u32(cursor + 32, value);
        index += 1;
        cursor = next;
    } while ((sint32)index < (sint32)count);
    w_u32(cursor, 0x01000000);
    w_u32(cursor + 4, 0xE1000220);
    ordering = r_u32(object + 80);
    table = r_u32(state);
    ordering = table + (ordering << 2);
    head = r_u32(state + 4);
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(cursor, tag, 7);
    w_masked_u32(ordering, head & 0xFFFFFF, 7);
    next = cursor + 8;
    w_u32(state + 4, next);
    return next;
}

uint32 sub_8008d140(uint32 object)
{
    uint32 game, matrix, ticks, a, b, c, shift, scale, result;
    FUNCTION_MARKER(0x8008D140, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    matrix = object + 16;
    ticks = r_u32(game + 40);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(matrix + 20, a);
    w_u32(matrix + 24, b);
    w_u32(matrix + 28, c);
    w_u16(matrix + 34, 0xFFFF);
    w_u16(matrix + 36, 4);
    w_u16(matrix + 32, 0);
    w_u32(matrix + 40, 40);
    w_u32(matrix + 44, (ticks << 9) + ticks);
    shift = r_u32(object + 12);
    scale = xport_gte_read_control(26);
    result = sub_8008cd80(matrix, shift, scale);
    if (result == 0)
        return result;
    result = sub_8008cefc(object, matrix, 3, 341, result);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    xport_gte_write_control(5, a);
    c = r_u32(matrix + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

uint32 sub_8008d244(uint32 object)
{
    uint32 game, matrix, ticks, a, b, c, shift, scale, result;
    FUNCTION_MARKER(0x8008D244, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    matrix = object + 16;
    ticks = r_u32(game + 40);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(matrix + 20, a);
    w_u32(matrix + 24, b);
    w_u32(matrix + 28, c);
    w_u16(matrix + 34, 0xFFFF);
    w_u16(matrix + 36, 2);
    w_u16(matrix + 32, 0);
    w_u32(matrix + 40, 36);
    w_u32(matrix + 44, (ticks << 9) + ticks);
    shift = r_u32(object + 12);
    scale = xport_gte_read_control(26);
    result = sub_8008cd80(matrix, shift, scale);
    if (result == 0)
        return result;
    result = sub_8008cefc(object, matrix, 5, 318, result);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    xport_gte_write_control(5, a);
    c = r_u32(matrix + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

uint32 sub_8008d348(uint32 object)
{
    uint32 game, matrix, ticks, scale, shift, a, b, c, value, result;
    FUNCTION_MARKER(0x8008D348, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    matrix = object + 16;
    ticks = r_u32(game + 40);
    scale = xport_gte_read_control(26);
    shift = r_u32(object + 12);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(matrix + 20, a);
    w_u32(matrix + 24, b);
    w_u32(matrix + 28, c);
    w_u16(matrix + 32, 40);
    w_u16(matrix + 34, -15);
    w_u16(matrix + 36, 0);
    w_u32(matrix + 40, 37);
    w_u32(matrix + 44, (ticks << 9) - ticks);
    result = sub_8008cd80(matrix, shift, scale);
    if (result == 0)
        return result;
    result = sub_8008cefc(object, matrix, 3, 0 - (uint32)318, result);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    xport_gte_write_control(5, a);
    c = r_u32(matrix + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    value = r_u16(matrix + 32);
    c = r_u32(matrix + 44);
    w_u16(matrix + 32, 0 - value);
    w_u32(matrix + 44, 1000 - c);
    result = sub_8008cd80(matrix, shift, scale);
    if (result == 0)
        return result;
    result = sub_8008cefc(object, matrix, 3, 318, result);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    xport_gte_write_control(5, a);
    c = r_u32(matrix + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

uint32 sub_8008d678(uint32 state)
{
    uint32 scratch, packet, cursor, value, color, screen, depth, scale, shift, distance, numerator, size, ordering, table, tag, index, width, thin, half, quarter;
    FUNCTION_MARKER(0x8008D678, "ACE3.BIN");
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x00C04343);
    xport_gte_write_data(8, 1118);
    scratch = state + 16;
    packet = r_u32(state + 4);
    xport_gte_execute(0x780010);
    w_u32(packet + 68, ((packet + 136) & 0xFFFFFF) | 0x10000000);
    w_u32(packet + 4, 0x3A010101);
    w_u32(packet + 72, 0x3A010101);
    w_u32(packet + 136, ((packet + 204) & 0xFFFFFF) | 0x10000000);
    w_u32(packet + 140, 0x3A010101);
    w_u32(packet, ((packet + 68) & 0xFFFFFF) | 0x10000000);
    w_u32(scratch + 28, 0x00C04343);
    color = xport_gte_read_data(22);
    w_u32(scratch + 32, color);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, -32);
    xport_gte_execute(0x180001);
    cursor = packet;
    index = 0;
    do
    {
        w_u32(cursor + 60, 0x3A010101);
        w_u32(cursor + 52, 0x3A010101);
        w_u32(cursor + 36, 0x3A010101);
        w_u32(cursor + 28, 0x3A010101);
        w_u32(cursor + 20, 0x3A010101);
        index += 1;
        cursor += 68;
    } while (index < 3);
    screen = xport_gte_read_data(14);
    w_u32(scratch + 8, screen);
    depth = xport_gte_read_data(19);
    scale = xport_gte_read_control(26);
    shift = r_u32(state + 12);
    distance = (sint32)depth >> (shift & 31);
    numerator = scale << 4;
    if (distance - 31 >= 0x77D9)
        return numerator;
    numerator = (numerator - scale) << 1;
    if (distance == 0)
        xport_mips_break(7);
    if (distance == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)distance;
    w_u32(scratch + 24, size);
    table = r_u32(state);
    ordering = table + ((uint32)((sint32)distance >> 3) << 2);
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(packet + 136, tag, 7);
    w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    w_u32(state + 4, packet + 204);
    value = r_u32(scratch + 24);
    if ((sint32)value < 8)
        w_u32(scratch + 24, 8);
    cursor = packet;
    index = 0;
    screen = r_u32(scratch + 8);
    color = r_u32(scratch + 32);
    do
    {
        w_u32(cursor + 48, screen);
        w_u32(cursor + 16, screen);
        w_u32(cursor + 44, color);
        w_u32(cursor + 12, color);
        if (index == 1)
            color = r_u32(scratch + 28);
        index += 1;
        cursor += 68;
    } while (index < 3);
    width = r_u32(scratch + 24);
    thin = (sint32)width >> 4;
    if (thin == 0)
        thin = 1;
    value = r_u16(scratch + 8) - width;
    w_u16(packet + 40, value);
    w_u16(packet + 8, value);
    value = r_u16(scratch + 10) - width;
    w_u16(packet + 42, value);
    w_u16(packet + 10, value);
    value = r_u16(scratch + 8) + width;
    w_u16(packet + 64, value);
    w_u16(packet + 32, value);
    value = r_u16(scratch + 10) + width;
    w_u16(packet + 66, value);
    w_u16(packet + 34, value);
    value = r_u16(scratch + 8) + thin;
    w_u16(packet + 24, value);
    value = r_u16(scratch + 10) - thin;
    w_u16(packet + 26, value);
    value = r_u16(scratch + 8) - thin;
    w_u16(packet + 56, value);
    value = r_u16(scratch + 10) + thin;
    w_u16(packet + 58, value);
    packet += 68;
    width = r_u32(scratch + 24);
    half = (sint32)width >> 1;
    value = r_u16(scratch + 8) + half;
    w_u16(packet + 40, value);
    w_u16(packet + 8, value);
    value = r_u16(scratch + 10) - half;
    w_u16(packet + 42, value);
    w_u16(packet + 10, value);
    value = r_u16(scratch + 8) - half;
    w_u16(packet + 64, value);
    w_u16(packet + 32, value);
    value = r_u16(scratch + 10) + half;
    w_u16(packet + 66, value);
    w_u16(packet + 34, value);
    value = r_u16(scratch + 8) - thin;
    w_u16(packet + 24, value);
    value = r_u16(scratch + 10) - thin;
    w_u16(packet + 26, value);
    value = r_u16(scratch + 8) + thin;
    w_u16(packet + 56, value);
    value = r_u16(scratch + 10) + thin;
    w_u16(packet + 58, value);
    width = r_u32(scratch + 24);
    value = r_u16(scratch + 8);
    packet += 68;
    w_u16(packet + 56, value);
    w_u16(packet + 24, value);
    value = r_u16(scratch + 8);
    quarter = (sint32)width >> 2;
    value -= quarter;
    w_u16(packet + 40, value);
    w_u16(packet + 8, value);
    value = r_u16(scratch + 8) + quarter;
    w_u16(packet + 64, value);
    w_u16(packet + 32, value);
    value = r_u16(scratch + 10);
    w_u16(packet + 66, value);
    w_u16(packet + 42, value);
    w_u16(packet + 34, value);
    w_u16(packet + 10, value);
    value = r_u16(scratch + 10) - quarter;
    w_u16(packet + 26, value);
    value = r_u16(scratch + 10) + quarter;
    w_u16(packet + 58, value);
    return value;
}

uint32 sub_8008faa8(uint32 ordering, uint32 packet, uint32 offset, uint32 intensity)
{
    uint32 fade, game, mode, value, cursor, color, page_x, page_y, flat, textured, x, link, page, tail, tag;
    FUNCTION_MARKER(0x8008FAA8, "ACE3.BIN");
    fade = 4096 - intensity;
    if ((sint32)fade < 0)
        fade = 0;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x007F7F7F);
    xport_gte_write_data(8, fade);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 4);
    mode = value == 0 ? 256 : 0;
    xport_gte_execute(0x780010);
    cursor = packet + 8;
    w_u32(packet, (cursor & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 4, 0xE6000001);
    if (offset != 0)
    {
        cursor = packet + 20;
        w_u32(packet + 8, (cursor & 0xFFFFFF) | 0x02000000);
        w_u32(packet + 12, (((mode + 256) & 256) << 11) | 0xE5000000);
        w_u32(packet + 16, 0);
    }
    color = xport_gte_read_data(22);
    page_x = (mode & 256) >> 4;
    page_y = (mode & 512) << 2;
    flat = color | 0x62000000;
    textured = color | 0x66000000;
    x = 0;
    link = (cursor << 8) + 0x2800;
    do
    {
        page = (x & 0x3FF) >> 6;
        w_u32(cursor, (link >> 8) | 0x09000000);
        w_u32(cursor + 4, page_x | (page | 0x140) | page_y | 0xE1000200);
        w_u32(cursor + 20, page_x | (page | 0x120) | page_y | 0xE1000200);
        w_u32(cursor + 8, flat);
        w_u32(cursor + 24, textured);
        w_u32(cursor + 28, x);
        w_u32(cursor + 12, x);
        w_u16(cursor + 32, 0);
        if (x != 0)
        {
            w_u32(cursor + 36, 0x00F00040);
            w_u32(cursor + 16, 0x00F00040);
        }
        else
        {
            w_u32(cursor + 36, 0x00F00100);
            w_u32(cursor + 16, 0x00F00100);
        }
        link += 0x2800;
        x += 256;
        cursor += 40;
    } while ((sint32)x < 320);
    tail = cursor;
    if (offset != 0)
    {
        value = r_u32(offset);
        w_u32(cursor, 0x02000000);
        w_u32(cursor + 8, 0);
        w_u32(cursor + 4, (value & 0x7FF) | ((((value << 5) >> 21) << 11)) | 0xE5000000);
        cursor += 12;
    }
    else
        tail = cursor - 40;
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(tail, tag, 7);
    w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    return cursor;
}

uint32 sub_80092014(uint32 first, uint32 second)
{
    uint32 x, y, z, a, b, c, dot, sx, sy, sz, sum, leading, normalized, root, shift, divisor;
    FUNCTION_MARKER(0x80092014, "ACE3.BIN");
    x = r_u32(first);
    y = r_u32(first + 4);
    z = r_u32(first + 8);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    a = r_u32(second);
    b = r_u32(second + 4);
    xport_gte_execute(0xA00428);
    dot = x * a;
    a = b * y;
    c = r_u32(second + 8);
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
    leading = xport_gte_read_data(31) & 0xFE;
    if ((sint32)leading < 24)
        normalized = (sint32)sum >> ((24 - leading) & 31);
    else
        normalized = sum << ((leading - 24) & 31);
    root = r_s16(0x800B6DD4 + ((normalized - 64) << 1));
    shift = (sint32)(31 - leading) >> 1;
    root <<= shift & 31;
    divisor = (sint32)root >> 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && dot == 0x80000000)
        xport_mips_break(6);
    return (sint32)dot / (sint32)divisor;
}

uint32 sub_8009211c(uint32 first, uint32 second, uint32 out, uint32 weight)
{
    uint32 value, y, z, x;
    FUNCTION_MARKER(0x8009211C, "ACE3.BIN");
    xport_gte_write_data(8, weight);
    value = r_u32(first);
    xport_gte_write_data(9, value);
    value = r_u32(first + 4);
    xport_gte_write_data(10, value);
    value = r_u32(first + 8);
    xport_gte_write_data(11, value);
    y = r_u32(second + 4);
    z = r_u32(second + 8);
    x = r_u32(second);
    xport_gte_write_control(21, x);
    xport_gte_write_control(22, y);
    xport_gte_write_control(23, z);
    xport_gte_execute(0x980011);
    value = xport_gte_read_data(25);
    w_u32(out, value);
    value = xport_gte_read_data(26);
    w_u32(out + 4, value);
    value = xport_gte_read_data(27);
    w_u32(out + 8, value);
    return y;
}

uint32 sub_80092ab8(void)
{
    uint32 root, cursor, index, mask, flags;
    FUNCTION_MARKER(0x80092AB8, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    cursor = root;
    index = 0;
    do
    {
        mask = r_u32(cursor + 0x108C);
        flags = r_u32(cursor + 0x600);
        index += 1;
        w_u32(cursor + 0x108C, 0);
        w_u32(cursor + 0x600, flags & ~mask);
        cursor += 4;
    } while (index < 3);
    root = r_u32(0x800BCF90);
    w_u32(root + 0x33C0, 0);
    w_u32(root + 0x3964, 0);
    w_u32(root + 0x3B28, 0);
    return root;
}

uint32 sub_80095fa0(uint32 state, uint32 context)
{
    uint32 scene, data, value, flags, camera, far_limit, near_limit, divisor, numerator, far_width, near_width, cursor, widths, index, height;
    FUNCTION_MARKER(0x80095FA0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 28);
    w_u32(context + 248, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 8);
    w_u32(context + 256, value);
    w_u32(context + 252, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 4);
    w_u32(context + 264, value);
    w_u32(context + 260, value);
    data = r_u32(scene + 0x244);
    flags = r_u16(data + 2);
    if ((flags & 0x400) == 0)
    {
        flags = r_u32(context + 360);
        if ((flags & 1) == 0)
        {
            value = r_u32(context + 264);
            w_u32(context + 264, value | 0x01000000);
        }
    }
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    far_limit = r_s16(camera + 42);
    if ((sint32)far_limit >= 26625)
        far_limit = 26624;
    near_limit = r_s16(camera + 40);
    if ((sint32)near_limit < 1000)
        near_limit = 1000;
    far_limit = (sint32)far_limit >> 1;
    numerator = far_limit * 200;
    divisor = r_s16(state + 26);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    far_width = (sint32)numerator / (sint32)divisor;
    near_limit = (sint32)near_limit >> 1;
    numerator = near_limit * 200;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    near_width = (sint32)numerator / (sint32)divisor;
    cursor = context;
    widths = context;
    index = 0;
    value = r_u32(state + 12);
    height = (sint32)(0 - value - 3584) >> 1;
    do
    {
        if (index - 1 < 2)
        {
            w_u16(cursor + 44, 0 - far_width);
            w_u16(cursor + 48, far_width);
            w_u32(widths + 124, far_limit);
        }
        else
        {
            w_u32(widths + 124, near_limit);
            w_u16(cursor + 44, 0 - near_width);
            w_u16(cursor + 48, near_width);
        }
        w_u16(cursor + 50, height);
        w_u16(cursor + 46, height);
        if (index == 1)
        {
            value = r_u32(state + 12);
            height = (sint32)(512 - value) >> 1;
        }
        cursor += 8;
        index += 1;
        widths += 4;
    } while (index < 4);
    return 0;
}

uint32 sub_80096184(uint32 state, uint32 context)
{
    uint32 flags, mode, result, packet, next, value;
    FUNCTION_MARKER(0x80096184, "ACE3.BIN");
    flags = r_u16(context + 40);
    if (flags != 0)
    {
        mode = r_s8(context + 267);
        if (mode == 0)
        {
            if ((flags & 1) != 0)
                sub_80095298(state, context, 0);
            if ((flags & 8) != 0)
                sub_80095298(state, context, 4);
            if ((flags & 1) != 0)
                sub_8009546c(state, context, 1);
            if ((flags & 4) != 0)
                sub_8009546c(state, context, 3);
        }
        result = flags & 6;
        if (result != 0)
            return sub_80095298(state, context, 2);
        return result;
    }
    result = r_s8(context + 267);
    if (result == 0)
        return result;
    packet = r_u32(state + 4);
    next = packet + 16;
    w_u32(packet, (next & 0xFFFFFF) | 0x03000000);
    value = r_u32(context + 252);
    w_u32(packet + 4, value | 0x60000000);
    result = r_u32(context + 232);
    w_u32(packet + 12, 0x00F00140);
    w_u32(packet + 8, result);
    w_u32(state + 4, next);
    return result;
}

uint32 sub_800962b8(uint32 ordering, uint32 packet)
{
    uint32 start, flags, mode, current, tail, tag, root;
    FUNCTION_MARKER(0x800962B8, "ACE3.BIN");
    start = packet + 8;
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, start);
    sub_80094718(0x1F800000, 0x1F80001C);
    sub_80095fa0(0x1F800000, 0x1F80001C);
    sub_800948a0(0x1F80001C, 4);
    sub_800949ac(0x1F80001C);
    sub_80096184(0x1F800000, 0x1F80001C);
    flags = r_u32(0x1F800184);
    mode = (flags & 4) != 0;
    current = r_u32(0x1F800004);
    tail = packet;
    if (current == start)
    {
        w_u32(packet, 0x01000000);
        w_u32(packet + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(packet, tag, 7);
        w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    }
    else
    {
        w_u32(packet, (start & 0xFFFFFF) | 0x01000000);
        w_u32(packet + 4, 0xE6000001);
        tail = r_u32(0x1F800004);
        w_u32(tail, 0x01000000);
        w_u32(tail + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(tail, tag, 7);
        w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    }
    root = r_u32(0x800BCF90);
    w_u32(0x1F800004, tail + 8);
    w_u32(root + 0x5C8, 0);
    return r_u32(0x1F800004);
}

uint32 sub_800963f8(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    sint32 a, b, c, d, high_ab, high_cd, high, low_ab, low_cd, low;
    FUNCTION_MARKER(0x800963F8, "ACE3.BIN");
    a = (sint32)first >> 16;
    b = (sint32)second >> 16;
    c = (sint32)third >> 16;
    d = (sint32)fourth >> 16;
    high_ab = b < a ? a : b;
    high_cd = d < c ? c : d;
    high = high_cd < high_ab ? high_ab : high_cd;
    low_ab = a < b ? a : b;
    low_cd = c < d ? c : d;
    low = low_ab < low_cd ? low_ab : low_cd;
    if (high - low >= 512)
        return 1;
    a = (sint32)(first << 16) >> 16;
    b = (sint32)(second << 16) >> 16;
    c = (sint32)(third << 16) >> 16;
    d = (sint32)(fourth << 16) >> 16;
    high_ab = b < a ? a : b;
    high_cd = d < c ? c : d;
    high = high_cd < high_ab ? high_ab : high_cd;
    low_ab = a < b ? a : b;
    low_cd = c < d ? c : d;
    low = low_ab < low_cd ? low_ab : low_cd;
    return high - low >= 1024;
}

uint32 sub_80096518(uint32 out, uint32 first, uint32 second)
{
    uint32 a, b, value, low, high;
    FUNCTION_MARKER(0x80096518, "ACE3.BIN");
    a = r_s16(first);
    b = r_s16(second);
    value = a + b;
    w_u16(out, (sint32)value >> 1);
    a = r_s16(first + 2);
    b = r_s16(second + 2);
    value = a + b;
    w_u16(out + 2, (sint32)value >> 1);
    a = r_s16(first + 4);
    b = r_s16(second + 4);
    value = a + b;
    w_u16(out + 4, (sint32)value >> 1);
    a = r_u32(first + 12);
    b = r_u32(second + 12);
    low = (((a & 0x00FF00FF) + (b & 0x00FF00FF)) >> 1) & 0x00FF00FF;
    high = (((a & 0xFF00FF00) + (b & 0xFF00FF00)) >> 1) & 0xFF00FF00;
    w_u32(out + 12, low | high);
    a = r_u16(first + 18);
    b = r_u16(second + 18);
    low = ((a & 0x00FF00FF) + (b & 0x00FF00FF)) >> 1;
    value = (a & 0xFF00FF00) + (b & 0xFF00FF00);
    high = ((uint32)((sint32)value >> 1)) & 0xFF00;
    w_u16(out + 18, low | high);
    return high;
}

uint32 sub_800965d4(uint32 point)
{
    sint32 x, y;
    uint32 result;
    FUNCTION_MARKER(0x800965D4, "ACE3.BIN");
    x = r_s16(point);
    result = 0;
    if (x < -160)
        result = 1;
    else if (x >= 161)
        result = 2;
    y = r_s16(point + 2);
    if (y < -120)
        return result | 4;
    if (y >= 121)
        return result | 8;
    return result;
}

void sub_80094380(uint32 event)
{
    FUNCTION_MARKER(0x80094380, "ACE3.BIN");
    if (event == 500)
        sub_8009423c(0);
}

uint32 sub_8009757c(uint32 out, uint32 points, uint32 level)
{
    uint32 cosine, scale, negative, z, cursor, point_out, index, a, b, product, packed, flags, value, first, second, third, fourth;
    FUNCTION_MARKER(0x8009757C, "ACE3.BIN");
    if ((sint32)level < 9)
    {
        cosine = rcos(level * 100);
        scale = level * 508;
    }
    else
    {
        cosine = rcos(862);
        scale = 4096;
    }
    negative = 0 - cosine;
    z = cosine - (uint32)((sint32)cosine >> 2);
    cursor = out;
    point_out = out + 4;
    index = 0;
    do
    {
        a = r_s16(points + 2);
        product = scale * (0 - a);
        b = r_s16(points);
        b = scale * (0 - b);
        packed = ((uint32)((sint32)product >> 12) & 0xFFFF) | ((uint32)((sint32)b >> 12) << 16);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, z);
        a = (sint32)product >> 16;
        b = (sint32)packed >> 20;
        xport_gte_execute(0x180001);
        points += 4;
        w_u16(point_out + 4, a);
        w_u16(point_out + 6, b);
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            w_u32(cursor, 0xFFFFFFFF);
        else
        {
            value = xport_gte_read_data(14);
            w_u32(cursor + 4, value);
            xport_gte_write_data(1, negative);
            xport_gte_execute(0x108041B);
            value = sub_800965d4(point_out);
            w_u32(cursor, value);
            value = xport_gte_read_data(22);
            w_u32(cursor + 12, value);
        }
        index += 1;
        point_out += 16;
        cursor += 16;
    } while (index < 24);
    first = r_u32(cursor - 384);
    second = r_u32(cursor - 380);
    third = r_u32(cursor - 376);
    fourth = r_u32(cursor - 372);
    w_u32(cursor, first);
    w_u32(cursor + 4, second);
    w_u32(cursor + 8, third);
    w_u32(cursor + 12, fourth);
    return 0;
}

uint32 sub_80097eb4(uint32 ordering, uint32 packet)
{
    uint32 start, next, scene, camera, value, root, flags, mode, current, tail, tag;
    FUNCTION_MARKER(0x80097EB4, "ACE3.BIN");
    start = packet + 8;
    next = packet + 24;
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, start);
    w_u32(packet + 8, (next & 0xFFFFFF) | 0x03000000);
    w_u32(start + 8, 0xFF88FF60);
    w_u32(start + 12, 0x00F00140);
    scene = r_u32(0x800BCE54);
    w_u32(start + 4, 0x60000000);
    w_u32(0x1F800004, next);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 20);
    w_u16(0x1F800014, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 22);
    w_u16(0x1F800016, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 24);
    w_u16(0x1F800018, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    w_u16(0x1F80001A, value);
    sub_80095cb8(0x1F800000, 0x1F80001C, 256);
    sub_800976f8();
    root = r_u32(0x800BCF90);
    flags = r_u32(root + 0x5C8);
    mode = (flags & 4) != 0;
    current = r_u32(0x1F800004);
    tail = packet;
    if (current == start)
    {
        w_u32(packet, 0x01000000);
        w_u32(packet + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(packet, tag, 7);
        w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    }
    else
    {
        w_u32(packet, (start & 0xFFFFFF) | 0x01000000);
        w_u32(packet + 4, 0xE6000001);
        tail = r_u32(0x1F800004);
        w_u32(tail, 0x01000000);
        w_u32(tail + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(tail, tag, 7);
        w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    }
    root = r_u32(0x800BCF90);
    w_u32(root + 0x5C8, 0);
    return tail + 8;
}

uint32 sub_8009830c(uint32 state, uint32 out, uint32 distance)
{
    uint32 points, x, y, z, cursor, index, value, flags, first, second, third;
    FUNCTION_MARKER(0x8009830C, "ACE3.BIN");
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, distance);
    points = state + 76;
    xport_gte_execute(0x486012);
    z = xport_gte_read_data(27);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    if ((sint32)z < -2048)
    {
        index = 19;
        cursor = out + 228;
        do
        {
            w_u32(cursor, 0xFFFFFFFF);
            index -= 1;
            cursor -= 12;
        } while ((sint32)index >= 0);
        return 0xFFFFFFFF;
    }
    xport_gte_write_control(5, x);
    xport_gte_write_control(6, y);
    xport_gte_write_control(7, z);
    xport_gte_write_data(1, 0);
    index = 0;
    cursor = out;
    do
    {
        value = r_u32(points);
        xport_gte_write_data(0, value);
        points += 4;
        index += 1;
        xport_gte_execute(0x180001);
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            w_u32(cursor, 0xFFFFFFFF);
        else
        {
            value = xport_gte_read_data(14);
            w_u32(cursor + 4, value);
            xport_gte_execute(0x780010);
            value = sub_800965d4(cursor + 4);
            w_u32(cursor, value);
            value = xport_gte_read_data(22);
            w_u32(cursor + 8, value);
        }
        cursor += 12;
    } while (index < 20);
    first = r_u32(cursor - 240);
    second = r_u32(cursor - 236);
    third = r_u32(cursor - 232);
    w_u32(cursor, first);
    w_u32(cursor + 4, second);
    w_u32(cursor + 8, third);
    return 0;
}

uint32 sub_80098714(uint32 state, uint32 out, uint32 step)
{
    uint32 x, z, y, depth, magnitude, product, factor, packed_y, product_x, packed, flags, screen, value, mask, index;
    sint32 sx, sy;
    FUNCTION_MARKER(0x80098714, "ACE3.BIN");
    step <<= 10;
    x = r_u32(state + 32);
    z = r_u32(state + 40);
    y = r_u32(state + 36);
    depth = z + 8192 - step;
    xport_gte_write_data(1, depth);
    magnitude = (sint32)depth < 0 ? 0 - depth : depth;
    product = ((magnitude << 4) - magnitude) << 8;
    factor = 4096 - (uint32)((sint32)product / 8192);
    product = y * factor;
    packed_y = (uint32)((sint32)product / 4096) << 16;
    index = 0;
    product_x = (x - 8192) * factor;
    do
    {
        packed = ((uint32)((sint32)product_x / 4096) & 0xFFFF) | packed_y;
        xport_gte_write_data(0, packed);
        product_x += factor << 10;
        xport_gte_execute(0x180001);
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            w_u32(out, 0xFFFFFFFF);
        else
        {
            screen = xport_gte_read_data(14);
            mask = 0;
            sy = (sint32)screen >> 16;
            xport_gte_execute(0x780010);
            w_u32(out + 4, screen);
            sx = (sint32)(screen << 16) >> 16;
            if (sx < -160)
                mask = 1;
            else if (sx >= 161)
                mask = 2;
            if (sy < -120)
                mask |= 4;
            else if (sy >= 121)
                mask |= 8;
            w_u32(out, mask);
            value = xport_gte_read_data(22);
            w_u32(out + 8, value);
        }
        index += 1;
        out += 12;
    } while (index < 16);
    w_u32(out, 0xFFFFFFFF);
    return 0xFFFFFFFF;
}

uint32 sub_80098414(uint32 state, uint32 ticks)
{
    uint32 matrix, a, b, c, value, height, focus, table, index, cursor, distance, packet, row, old_buffer, new_buffer, old_cursor, new_cursor, old_flag, new_flag, prior_old, prior_new, next;
    FUNCTION_MARKER(0x80098414, "ACE3.BIN");
    matrix = state + 28;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    value = r_s16(0x800B7106 + ((ticks << 7) & 0x3F80));
    height = r_s16(state + 26);
    focus = (uint32)((sint32)(value + 4096) / 64) + 128;
    xport_gte_write_control(26, focus);
    w_u32(matrix + 72, focus << 11);
    sub_8005e7ec(10000, 20000, height);
    index = 0;
    table = 0x800B7104;
    cursor = matrix;
    do
    {
        index += 1;
        value = r_u32(table);
        table += 0x330;
        w_u16(cursor + 76, (sint32)value >> 17);
        w_u16(cursor + 78, (sint32)(value << 16) >> 17);
        cursor += 4;
    } while (index < 20);
    value = r_u32(matrix + 76);
    w_u32(matrix + (index << 2) + 76, value);
    xport_gte_write_data(6, 0x3C878787);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    distance = 0x3000 - ((ticks << 7) & 0x3FF);
    packet = r_u32(state + 4);
    sub_8009830c(matrix, matrix + 160, distance);
    row = 0;
    distance -= 1024;
    do
    {
        new_buffer = matrix + 160 + 252 * ((row + 1) & 1);
        old_buffer = matrix + 160 + 252 * (row & 1);
        sub_8009830c(matrix, new_buffer, distance);
        index = 0;
        new_cursor = new_buffer + 16;
        old_cursor = old_buffer + 16;
        old_flag = r_u32(old_buffer);
        new_flag = r_u32(new_buffer);
        do
        {
            prior_old = old_flag;
            prior_new = new_flag;
            old_flag = r_u32(old_cursor - 4);
            new_flag = r_u32(new_cursor - 4);
            if (prior_old != 0xFFFFFFFF && prior_new != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && (prior_old & prior_new & old_flag & new_flag) == 0)
            {
                next = packet + 52;
                w_u32(packet, (next & 0xFFFFFF) | 0x0C000000);
                a = r_u32(old_cursor - 8);
                b = r_u32(old_cursor + 4);
                w_u32(packet + 4, a);
                w_u32(packet + 16, b);
                a = r_u32(old_cursor - 12);
                b = r_u32(old_cursor);
                w_u32(packet + 8, a);
                w_u32(packet + 20, b);
                a = r_u32(new_cursor - 8);
                b = r_u32(new_cursor + 4);
                w_u32(packet + 28, a);
                w_u32(packet + 40, b);
                a = r_u32(new_cursor - 12);
                b = r_u32(new_cursor);
                w_u16(packet + 36, 0xBF00);
                w_u32(packet + 12, 0x4ED68000);
                w_u32(packet + 24, 0x0019803F);
                w_u16(packet + 48, 0xBF3F);
                w_u32(packet + 32, a);
                w_u32(packet + 44, b);
                packet = next;
            }
            index += 1;
            old_cursor += 12;
            new_cursor += 12;
        } while (index < 20);
        row += 1;
        distance -= 1024;
    } while (row < 24);
    w_u32(state + 4, packet);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    height = r_u16(state + 26);
    xport_gte_write_control(26, height);
    return 0;
}

uint32 sub_80098044(uint32 ordering, uint32 packet)
{
    uint32 game, ticks, numerator, phase, angle, cosine, intensity, cursor, start, scene, camera, value, matrix, a, b, c, root, flags, mode, tail, tag, end;
    FUNCTION_MARKER(0x80098044, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    ticks = r_u32(game + 40);
    numerator = ((ticks << 4) - ticks) << 9;
    phase = (sint32)numerator / 450;
    angle = (sint32)phase % 7680;
    if ((sint32)angle >= 2049)
        angle = 2048;
    cosine = r_s16(0x800B7106 + ((angle & 0xFFF) << 2));
    if ((sint32)cosine < 0)
        cosine = 0 - cosine;
    intensity = 4778 - cosine;
    if ((sint32)intensity < 1838)
        intensity = 1838;
    cursor = sub_8008faa8(ordering - 0x3FFC, packet, 0, intensity);
    start = cursor + 8;
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, start);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 4);
    w_u32(0x1F800008, value);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 8);
    w_u32(0x1F80000C, value);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 12);
    w_u32(0x1F800010, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    matrix = scene + 76;
    w_u16(0x1F80001A, value);
    w_u32(cursor + 8, ((cursor + 16) & 0xFFFFFF) | 0x01000000);
    w_u32(start + 4, 0xE1000200);
    w_u32(cursor + 16, ((cursor + 32) & 0xFFFFFF) | 0x04000000);
    w_u32(cursor + 20, 0x60000000);
    w_u32(cursor + 24, 0xFF88FF60);
    w_u32(cursor + 28, 0x00F00140);
    w_u32(0x1F800004, cursor + 32);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    xport_gte_write_control(5, a);
    c = r_u32(matrix + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    sub_80098414(0x1F800000, ticks);
    sub_80098850(0x1F800000);
    root = r_u32(0x800BCF90);
    flags = r_u32(root + 0x5C8);
    value = r_u32(0x1F800004);
    mode = (flags & 4) != 0;
    tail = cursor;
    if (value == start)
    {
        w_u32(cursor, 0x01000000);
        w_u32(cursor + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(tail, tag, 7);
        w_masked_u32(ordering, cursor & 0xFFFFFF, 7);
        end = tail + 8;
    }
    else
    {
        w_u32(cursor, (start & 0xFFFFFF) | 0x01000000);
        w_u32(cursor + 4, 0xE6000001);
        value = r_u32(0x1F800004);
        tail = value + 8;
        w_u32(value, (tail & 0xFFFFFF) | 0x01000000);
        w_u32(value + 4, 0xE1000220);
        w_u32(value + 8, 0x01000000);
        w_u32(tail + 4, mode | 0xE6000000);
        tag = r_masked_u32(ordering, 7);
        w_masked_u32(tail, tag, 7);
        w_masked_u32(ordering, cursor & 0xFFFFFF, 7);
        end = tail + 8;
    }
    root = r_u32(0x800BCF90);
    w_u32(root + 0x5C8, 0);
    sub_80084978(ordering - 0x3FF8, 4095);
    return end;
}

uint32 sub_80098850(uint32 state)
{
    uint32 matrix, height, a, b, c, scene, view, value, x, z, packet, group, slice, first_slice, last_slice, old_buffer, new_buffer, cursor, index, old_cursor, new_cursor, old_flag, prior_flag, new_flag, combined, next, color, point;
    FUNCTION_MARKER(0x80098850, "ACE3.BIN");
    matrix = state + 28;
    height = r_s16(0x1F80001A);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    sub_8005e7ec(2000, 20000, height);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(matrix + 20, a);
    w_u32(matrix + 24, b);
    w_u32(matrix + 28, c);
    scene = r_u32(0x800BCE54);
    view = scene + 44;
    a = r_u32(view);
    b = r_u32(view + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(view + 8);
    b = r_u32(view + 12);
    c = r_u32(view + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    x = r_u32(state + 8);
    z = r_u32(state + 16);
    xport_gte_execute(0x486012);
    w_u32(matrix + 32, (uint32)((sint32)(0 - x) >> 11) & 0x3FF);
    w_u32(matrix + 40, (uint32)((sint32)(0 - z) >> 11) & 0x3FF);
    value = r_u32(state + 12);
    value = ((uint32)((sint32)(0 - value) >> 11) & 0x7FF) - 5324;
    w_u32(matrix + 36, value);
    value = xport_gte_read_data(11);
    w_u32(matrix + 48, value);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(6, 0x00C05020);
    group = 0;
    cursor = r_u32(state + 4);
    packet = cursor + 8;
    w_u32(cursor, (packet & 0xFFFFFF) | 0x01000000);
    w_u32(cursor + 4, 0xE1000220);
    do
    {
        value = r_u32(matrix + 48);
        value = 0 - (uint32)((sint32)value >> 10);
        first_slice = value + 4;
        last_slice = value + 13;
        if ((sint32)first_slice < 0)
            first_slice = 0;
        if ((sint32)last_slice >= 17)
            last_slice = 16;
        slice = first_slice + 1;
        old_buffer = matrix + 52 + 204 * (slice & 1);
        sub_80098714(matrix, old_buffer, first_slice);
        while ((sint32)slice <= (sint32)last_slice)
        {
            new_buffer = matrix + 52 + 204 * ((slice + 1) & 1);
            old_buffer = matrix + 52 + 204 * (slice & 1);
            if (slice == last_slice)
            {
                index = 16;
                cursor = new_buffer + 192;
                do
                {
                    w_u32(cursor, 0xFFFFFFFF);
                    index -= 1;
                    cursor -= 12;
                } while ((sint32)index >= 0);
            }
            else
                sub_80098714(matrix, new_buffer, slice);
            old_flag = r_u32(old_buffer);
            index = 0;
            new_cursor = new_buffer + 4;
            old_cursor = old_buffer + 4;
            cursor = new_buffer;
            do
            {
                prior_flag = old_flag;
                old_flag = r_u32(old_cursor + 8);
                new_flag = r_u32(cursor);
                combined = old_flag & new_flag;
                if (prior_flag != 0xFFFFFFFF && combined != 0xFFFFFFFF && (prior_flag & combined) == 0)
                {
                    next = packet + 32;
                    if ((sint32)(old_flag | new_flag) < 0)
                    {
                        next = packet + 20;
                        color = r_u32(old_cursor + 4);
                        point = r_u32(old_cursor);
                        w_u32(packet, (next & 0xFFFFFF) | 0x04000000);
                        w_u32(packet + 4, color | 0x52000000);
                        w_u32(packet + 8, point);
                        if ((sint32)old_flag >= 0)
                        {
                            color = r_u32(old_cursor + 16);
                            point = r_u32(old_cursor + 12);
                        }
                        else
                        {
                            color = r_u32(new_cursor + 4);
                            point = r_u32(new_cursor);
                        }
                        w_u32(packet + 12, color);
                        w_u32(packet + 16, point);
                    }
                    else
                    {
                        color = r_u32(old_cursor + 16);
                        point = r_u32(old_cursor + 12);
                        w_u32(packet, (next & 0xFFFFFF) | 0x07000000);
                        w_u32(packet + 4, color | 0x5A000000);
                        w_u32(packet + 8, point);
                        color = r_u32(old_cursor + 4);
                        point = r_u32(old_cursor);
                        w_u32(packet + 12, color);
                        w_u32(packet + 16, point);
                        color = r_u32(new_cursor + 4);
                        point = r_u32(new_cursor);
                        w_u32(packet + 20, color);
                        w_u32(packet + 24, point);
                        game_random();
                        game_random();
                        w_u32(packet + 28, 0x55555555);
                    }
                    packet = next;
                }
                index += 1;
                old_cursor += 12;
                new_cursor += 12;
                cursor += 12;
            } while (index < 16);
            slice += 1;
        }
        value = r_u32(matrix + 36);
        group += 1;
        w_u32(matrix + 36, value + 2048);
    } while (group < 4);
    w_u32(state + 4, packet);
    return 0;
}

uint32 sub_800976f8(void)
{
    uint32 scene, a, b, c, index, value, flags, center_flag, uv, packet, fields, link, old_flag, prior_old, new_flag, prior_new, code, offset_x, offset_y, sum_x, sum_y, level, old_ring, new_ring, old_uv, new_uv, cursor, end;
    FUNCTION_MARKER(0x800976F8, "ACE3.BIN");
    w_u16(0x1F80001C, 0x3000);
    w_u16(0x1F80001E, 0);
    w_u16(0x1F800020, 0);
    rotation_build(0x1F80001C, 0x1F800024, 0, 0);
    scene = r_u32(0x800BCE54);
    MulMatrix0((MATRIX *)psx_addr(scene + 108, 20), (MATRIX *)psx_addr(0x1F800024, 20), (MATRIX *)psx_addr(0x1F800044, 20));
    a = r_u32(0x1F800044);
    b = r_u32(0x1F800048);
    xport_gte_write_control(8, a);
    xport_gte_write_control(9, b);
    a = r_u32(0x1F80004C);
    b = r_u32(0x1F800050);
    c = r_u32(0x1F800054);
    xport_gte_write_control(10, a);
    xport_gte_write_control(11, b);
    xport_gte_write_control(12, c);
    scene = r_u32(0x800BCE54);
    SetMulMatrix((MATRIX *)psx_addr(scene + 76, 20), (MATRIX *)psx_addr(0x1F800024, 20));
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_control(13, 0x300);
    xport_gte_write_control(14, 0x380);
    xport_gte_write_control(15, 0x600);
    xport_gte_write_control(16, 0x1000);
    xport_gte_write_control(17, 0x10000000);
    xport_gte_write_control(18, 0);
    xport_gte_write_control(19, 0x1000);
    xport_gte_write_control(20, 0);
    xport_gte_write_data(6, 0x34838383);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xC00);
    xport_gte_execute(0x180001);
    index = 0;
    cursor = 0x1F80001C;
    do
    {
        value = r_u32(0x800B7104 + (((index << 12) / 24) << 2));
        index += 1;
        w_u32(cursor + 12, value);
        cursor += 4;
    } while (index < 24);
    flags = xport_gte_read_control(31);
    if ((sint32)flags < 0)
        w_u32(0x1F80001C, 0xFFFFFFFF);
    else
    {
        value = xport_gte_read_data(14);
        w_u32(0x1F800020, value);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, -4096);
        xport_gte_execute(0x108041B);
        value = sub_800965d4(0x1F800020);
        w_u32(0x1F80001C, value);
        value = xport_gte_read_data(22);
        w_u32(0x1F800024, value);
    }
    xport_gte_write_data(6, 0x3C838383);
    sub_8009757c(0x1F800088, 0x1F800028, 1);
    center_flag = r_u32(0x1F80001C);
    if (center_flag != 0xFFFFFFFF)
    {
        index = 0;
        uv = 0x1F8000A2;
        packet = r_u32(0x1F800004);
        old_flag = r_u32(0x1F800088);
        fields = packet + 37;
        link = (packet << 8) + 0x2800;
        do
        {
            prior_old = old_flag;
            old_flag = r_u32(uv - 10);
            if (prior_old != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (center_flag & prior_old & old_flag) == 0)
            {
                w_u32(packet, (link >> 8) | 0x09000000);
                a = r_u32(0x1F800024);
                b = r_u32(uv - 14);
                c = r_u32(uv + 2);
                w_u32(fields - 33, a);
                w_u32(fields - 21, b);
                w_u32(fields - 9, c);
                c = r_u32(uv - 6);
                a = r_u32(0x1F800020);
                b = r_u32(uv - 22);
                w_u32(fields - 5, c);
                w_u32(fields - 29, a);
                w_u32(fields - 17, b);
                a = r_s16(uv - 16);
                b = r_s16(uv);
                c = r_s16(uv - 2);
                sum_y = a + b;
                b = r_s16(uv - 18);
                sum_x = b + c;
                code = 168;
                offset_x = 255;
                if ((sint32)sum_x >= 0)
                {
                    offset_x = 0;
                    code = 170;
                }
                offset_y = 0;
                if ((sint32)sum_y < 0)
                    offset_y = 255;
                else
                    code += 4;
                w_u16(fields - 11, code);
                w_u32(fields - 25, (offset_y << 8) | offset_x | 0x3FF00000);
                value = r_u8(uv - 18);
                w_u8(fields - 13, value + offset_x);
                value = r_u8(uv - 16);
                w_u8(fields - 12, value + offset_y);
                value = r_u8(uv - 2);
                link += 0x2800;
                w_u8(fields - 1, value + offset_x);
                value = r_u8(uv);
                packet += 40;
                w_u8(fields, value + offset_y);
                fields += 40;
            }
            index += 1;
            uv += 16;
        } while (index < 24);
        w_u32(0x1F800004, packet);
    }
    packet = r_u32(0x1F800004);
    level = 1;
    do
    {
        new_ring = 0x1F800088 + 400 * ((level + 1) & 1);
        old_ring = 0x1F800088 + 400 * (level & 1);
        if (level == 7)
        {
            xport_gte_write_control(13, 0x500);
            xport_gte_write_control(14, 0x500);
            xport_gte_write_control(15, 0x500);
            xport_gte_write_control(16, 0x924);
            xport_gte_write_control(17, 0x09240000);
            xport_gte_write_control(19, 0x924);
            xport_gte_write_data(6, 0x3AA06858);
        }
        sub_8009757c(old_ring, 0x1F800028, level + 1);
        index = 0;
        old_uv = old_ring + 26;
        new_uv = new_ring + 26;
        fields = packet + 49;
        link = (packet << 8) + 0x3400;
        new_flag = r_u32(new_ring);
        old_flag = r_u32(old_ring);
        do
        {
            prior_new = new_flag;
            prior_old = old_flag;
            new_flag = r_u32(new_uv - 10);
            old_flag = r_u32(old_uv - 10);
            if (prior_new != 0xFFFFFFFF && prior_old != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (prior_new & prior_old & new_flag & old_flag) == 0)
            {
                w_u32(packet, (link >> 8) | 0x0C000000);
                a = r_u32(new_uv - 14);
                b = r_u32(new_uv + 2);
                w_u32(fields - 45, a);
                w_u32(fields - 33, b);
                if (level < 7)
                {
                    a = r_u32(old_uv - 14);
                    b = r_u32(old_uv + 2);
                    w_u32(fields - 21, a);
                    w_u32(fields - 9, b);
                }
                else
                {
                    w_u32(fields - 9, 0);
                    w_u32(fields - 21, 0);
                }
                a = r_u32(new_uv - 22);
                b = r_u32(new_uv - 6);
                w_u32(fields - 41, a);
                w_u32(fields - 29, b);
                a = r_u32(old_uv - 22);
                b = r_u32(old_uv - 6);
                w_u32(fields - 17, a);
                w_u32(fields - 5, b);
                a = r_s16(old_uv - 16);
                b = r_s16(old_uv);
                c = r_s16(old_uv - 2);
                sum_y = a + b;
                b = r_s16(old_uv - 18);
                sum_x = b + c;
                code = 168;
                offset_x = 255;
                if ((sint32)sum_x >= 0)
                {
                    offset_x = 0;
                    code = 170;
                }
                offset_y = 0;
                if ((sint32)sum_y < 0)
                    offset_y = 255;
                else
                    code += 4;
                w_u16(fields - 35, 0x3FF0);
                w_u16(fields - 23, code);
                value = r_u8(new_uv - 18);
                w_u8(fields - 37, value + offset_x);
                value = r_u8(new_uv - 16);
                w_u8(fields - 36, value + offset_y);
                value = r_u8(new_uv - 2);
                w_u8(fields - 25, value + offset_x);
                value = r_u8(new_uv);
                w_u8(fields - 24, value + offset_y);
                value = r_u8(old_uv - 18);
                w_u8(fields - 13, value + offset_x);
                value = r_u8(old_uv - 16);
                w_u8(fields - 12, value + offset_y);
                value = r_u8(old_uv - 2);
                link += 0x3400;
                w_u8(fields - 1, value + offset_x);
                value = r_u8(old_uv);
                packet += 52;
                w_u8(fields, value + offset_y);
                fields += 52;
            }
            index += 1;
            new_uv += 16;
            old_uv += 16;
        } while (index < 24);
        level += 1;
    } while (level < 8);
    end = packet + 8;
    index = 0;
    new_uv = 0x1F80022C;
    w_u32(0x1F800004, packet);
    w_u32(packet, (end & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 4, 0xE1000220);
    old_flag = r_u32(0x1F800088);
    new_flag = r_u32(0x1F800218);
    old_uv = 0x1F80009C;
    w_u32(0x1F800004, end);
    do
    {
        prior_old = old_flag;
        prior_new = new_flag;
        old_flag = r_u32(old_uv - 4);
        new_flag = r_u32(new_uv - 4);
        if (prior_old != 0xFFFFFFFF && prior_new != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && (prior_old & prior_new & old_flag & new_flag) == 0)
        {
            cursor = end + 36;
            w_u32(end, (cursor & 0xFFFFFF) | 0x08000000);
            w_u32(end + 4, 0x3A000000);
            w_u32(end + 12, 0);
            a = r_u32(new_uv - 8);
            b = r_u32(new_uv + 8);
            w_u32(end + 20, a);
            w_u32(end + 28, b);
            a = r_u32(old_uv - 16);
            b = r_u32(old_uv);
            w_u32(end + 8, a);
            w_u32(end + 16, b);
            a = r_u32(new_uv - 16);
            b = r_u32(new_uv);
            w_u32(end + 24, a);
            w_u32(end + 32, b);
            end = cursor;
        }
        index += 1;
        old_uv += 16;
        new_uv += 16;
    } while (index < 24);
    w_u32(0x1F800004, end);
    sub_8009757c(0x1F800088, 0x1F800028, 9);
    index = 0;
    old_uv = 0x1F80009C;
    new_uv = 0x1F80022C;
    new_flag = r_u32(0x1F800218);
    old_flag = r_u32(0x1F800088);
    packet = r_u32(0x1F800004);
    do
    {
        prior_new = new_flag;
        prior_old = old_flag;
        new_flag = r_u32(new_uv - 4);
        old_flag = r_u32(old_uv - 4);
        if (prior_new != 0xFFFFFFFF && prior_old != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (prior_new & prior_old & new_flag & old_flag) == 0)
        {
            end = packet + 80;
            w_u32(packet, ((packet + 40) & 0xFFFFFF) | 0x09000000);
            w_u32(packet + 40, (end & 0xFFFFFF) | 0x09000000);
            w_u32(packet + 4, 0xE1000240);
            w_u32(packet + 44, 0xE1000220);
            w_u32(packet + 48, 0x3A010101);
            w_u32(packet + 8, 0x3A010101);
            w_u32(packet + 56, 0x00010101);
            w_u32(packet + 16, 0x00010101);
            w_u32(packet + 32, 0x00939393);
            w_u32(packet + 24, 0x00939393);
            a = r_u32(new_uv - 8);
            b = r_u32(new_uv + 8);
            w_u32(packet + 64, a);
            w_u32(packet + 72, b);
            a = r_u32(old_uv - 16);
            b = r_u32(old_uv);
            w_u32(packet + 52, a);
            w_u32(packet + 12, a);
            w_u32(packet + 60, b);
            w_u32(packet + 20, b);
            a = r_u32(new_uv - 16);
            b = r_u32(new_uv);
            w_u32(packet + 68, a);
            w_u32(packet + 28, a);
            w_u32(packet + 76, b);
            w_u32(packet + 36, b);
            packet = end;
        }
        index += 1;
        new_uv += 16;
        old_uv += 16;
    } while (index < 24);
    w_u32(0x1F800004, packet);
    return 0;
}

uint32 sub_8009a484(uint32 start, uint32 end, uint32 out, uint32 workspace, uint32 kind)
{
    uint8 record[24];
    uint32 direction[3], x, z, value, a, b, scene, models, result;
    FUNCTION_MARKER(0x8009A484, "ACE3.BIN");
    if (kind == 0)
    {
        value = r_u32(start);
        w_u32(out, value);
        value = r_u32(start + 4);
        w_u32(out + 4, value);
        value = r_u32(start + 8);
        w_u32(out + 8, value);
        return 0x7FFFFFFF;
    }
    x = r_u32(start);
    z = r_u32(start + 8);
    x = (uint32)((sint32)x >> 12) & 0x7FF;
    z = (uint32)((sint32)z >> 12) & 0x7FF;
    xport_store_le16(record, 1024 - x);
    value = r_u32(start + 4);
    xport_store_le16(record + 4, 1024 - z);
    xport_store_le32(record + 8, kind);
    xport_store_le16(record + 2, (sint32)(0 - value) >> 12);
    xport_store_le32(record + 20, 0x200000);
    a = r_u32(end);
    b = r_u32(start);
    direction[0] = (sint32)(a - b) >> 12;
    a = r_u32(end + 4);
    b = r_u32(start + 4);
    direction[1] = (sint32)(a - b) >> 12;
    a = r_u32(end + 8);
    b = r_u32(start + 8);
    direction[2] = (sint32)(a - b) >> 12;
    scene = r_u32(0x800BCE54);
    models = r_u32(scene + 588);
    result = sub_80099894(0, out, workspace, models, 0, direction, 0, record);
    a = r_u32(out);
    b = r_u32(start);
    w_u32(out, a + b);
    a = r_u32(out + 4);
    b = r_u32(start + 4);
    w_u32(out + 4, a + b);
    a = r_u32(out + 8);
    b = r_u32(start + 8);
    w_u32(out + 8, a + b);
    return result;
}

void sub_8009bd2c(void)
{
    FUNCTION_MARKER(0x8009BD2C, "ACE3.BIN");
    PadStopCom();
}

void sub_8009fd68(void)
{
    FUNCTION_MARKER(0x8009FD68, "ACE3.BIN");
    SpuQuit();
}

uint32 sub_8008abf0(uint32 object, uint32 state)
{
    uint32 a, b, x, y, z, numerator, denominator, ratio, height, scale, product, sine, flags, depth, screen, radius, value, magnitude, bucket, ordering, color, shift, count, packet, cursor, last, index, next, lookup, tag, head;
    FUNCTION_MARKER(0x8008ABF0, "ACE3.BIN");
    a = r_u32(object + 4);
    b = r_u32(0x1F800008);
    x = (sint32)(a - b) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return 1;
    a = r_u32(object + 8);
    b = r_u32(0x1F80000C);
    y = (sint32)(a - b) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return 1;
    a = r_u32(object + 12);
    b = r_u32(0x1F800010);
    z = (sint32)(a - b) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return x & 0xFFFF;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    a = r_s16(object);
    denominator = r_s16(object + 2);
    xport_gte_execute(0x180001);
    if ((sint32)denominator <= 0)
        denominator = 1;
    numerator = a << 12;
    ratio = (sint32)numerator / (sint32)denominator;
    height = r_u16(0x1F80001A);
    scale = r_s16(object + 22);
    height = (sint32)(height << 16) >> 16;
    product = scale * height;
    sine = rsin((sint32)ratio / 4);
    product *= sine;
    scale = (sint32)product / 4096;
    flags = xport_gte_read_control(31);
    if ((sint32)flags < 0)
        return flags;
    depth = xport_gte_read_data(19);
    screen = xport_gte_read_data(14);
    if (depth == 0)
        depth = 1;
    if (depth == 0xFFFFFFFF && scale == 0x80000000)
        xport_mips_break(6);
    radius = (sint32)scale / (sint32)depth;
    if ((sint32)radius <= 0)
        return (sint32)radius < 181;
    if ((sint32)radius >= 181)
        radius = 180;
    value = (sint32)screen >> 16;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)(magnitude - radius) >= 121)
        return screen << 16;
    value = (sint32)(screen << 16) >> 16;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)(magnitude - radius) >= 161)
        return 0;
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
    ordering = r_u32(0x1F800000);
    ordering += bucket << 2;
    w_u32(state + 12, ordering);
    color = xport_gte_read_data(22);
    xport_gte_write_data(8, ratio);
    xport_gte_write_data(6, color);
    w_u32(state + 24, screen);
    w_u16(state + 4, radius);
    xport_gte_execute(0x780010);
    if ((sint32)radius < 5)
        shift = 3;
    else if ((sint32)radius < 10)
        shift = 2;
    else
        shift = (sint32)radius < 44;
    count = 32 >> shift;
    color = xport_gte_read_data(22);
    packet = r_u32(0x1F800004);
    cursor = packet + 16;
    w_u32(packet, ((packet + 8) & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 8, (cursor & 0xFFFFFF) | 0x01000000);
    last = cursor + count * 28 - 28;
    w_u32(packet + 4, 0xE6000001);
    w_u32(packet + 12, 0xE1000240);
    value = r_u16(state + 24);
    w_u16(last + 24, value);
    w_u16(cursor + 16, value);
    a = r_u16(state + 26);
    b = r_u16(state + 4);
    value = a + b;
    w_u16(last + 26, value);
    index = 1;
    w_u16(cursor + 18, value);
    while ((sint32)index < (sint32)count)
    {
        next = cursor + 28;
        lookup = index << shift;
        w_u32(cursor, (next & 0xFFFFFF) | 0x06000000);
        w_u32(cursor + 4, color);
        w_u32(cursor + 20, 0x00030303);
        w_u32(cursor + 12, 0x00030303);
        w_u32(cursor + 8, screen);
        a = r_s16(state + (lookup << 1) + 36);
        b = r_s16(state + 4);
        product = a * b;
        a = r_u16(state + 24);
        value = a + (uint32)((sint32)product / 4096);
        w_u16(next + 16, value);
        w_u16(cursor + 24, value);
        a = r_s16(state + ((lookup + 8) << 1) + 36);
        b = r_s16(state + 4);
        product = a * b;
        index += 1;
        a = r_u16(state + 26);
        value = a + (uint32)((sint32)product / 4096);
        w_u16(next + 18, value);
        w_u16(cursor + 26, value);
        cursor = next;
    }
    next = cursor + 28;
    last = cursor + 36;
    w_u32(cursor, (next & 0xFFFFFF) | 0x06000000);
    w_u32(cursor + 4, color);
    w_u32(cursor + 20, 0x00030303);
    w_u32(cursor + 12, 0x00030303);
    w_u32(cursor + 8, screen);
    w_u32(cursor + 28, (last & 0xFFFFFF) | 0x01000000);
    w_u32(next + 4, 0xE1000220);
    w_u32(cursor + 36, 0x01000000);
    value = r_u32(state + 120);
    w_u32(last + 4, value);
    ordering = r_u32(state + 12);
    head = r_u32(0x1F800004);
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(last, tag, 7);
    w_masked_u32(ordering, head & 0xFFFFFF, 7);
    value = cursor + 44;
    w_u32(0x1F800004, value);
    return value;
}

uint32 sub_8008c9d4(uint32 state)
{
    uint32 root, scene, data, flags, color, matrix, a, b, c, value, index, table, point, previous, packet, preamble, start, center, clip_center, clip_previous, clip_next, screen, next, area, ordering, tag;
    FUNCTION_MARKER(0x8008C9D4, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    scene = r_u32(0x800BCE54);
    flags = r_u32(root + 0x5C8);
    data = r_u32(scene + 0x244);
    color = r_u32(data + 112);
    if ((flags & 4) == 0 || color == 0)
        return data;
    matrix = state + 16;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(matrix, a);
    w_u32(matrix + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(matrix + 8, a);
    w_u32(matrix + 12, b);
    w_u32(matrix + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(matrix + 20, a);
    w_u32(matrix + 24, b);
    w_u32(matrix + 28, c);
    a = xport_gte_read_control(8);
    b = xport_gte_read_control(9);
    w_u32(matrix + 112, a);
    w_u32(matrix + 116, b);
    value = sub_80089e48(state + 128);
    w_u32(matrix + 112, value);
    w_u16(matrix + 116, 0);
    RotMatrixYXZ((SVECTOR *)psx_addr(state + 128, 8), (MATRIX *)psx_addr(state + 48, 32));
    w_u32(matrix + 64, 4096);
    w_u32(matrix + 72, 2457);
    w_u32(matrix + 68, 0);
    w_u32(matrix + 76, 0);
    w_u32(matrix + 80, 3072);
    MulMatrix2((MATRIX *)psx_addr(state + 80, 20), (MATRIX *)psx_addr(state + 48, 20));
    w_u16(matrix + 112, 0);
    w_u16(matrix + 114, 0);
    w_u16(matrix + 116, 240);
    ApplyMatrixSV((MATRIX *)psx_addr(state + 48, 20), (SVECTOR *)psx_addr(state + 128, 8), (SVECTOR *)psx_addr(state + 128, 6));
    point = state + 136;
    index = 0;
    table = 0x800B7104;
    do
    {
        value = r_s16(table);
        w_u16(point, (sint32)value / 64);
        value = r_s16(table + 2);
        value *= 96;
        w_u16(point + 2, (sint32)value / 4096);
        w_u16(point + 4, 220);
        ApplyMatrixSV((MATRIX *)psx_addr(state + 48, 20), (SVECTOR *)psx_addr(point, 8), (SVECTOR *)psx_addr(point, 6));
        index += 1;
        point += 8;
        table += 1024;
    } while (index < 16);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 12);
    c = r_u32(matrix + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    value = r_u32(matrix + 112);
    xport_gte_write_data(0, value);
    value = r_u32(matrix + 116);
    xport_gte_write_data(1, value);
    preamble = r_u32(state + 4);
    w_u32(preamble + 4, 0xE6000000);
    xport_gte_execute(0x180001);
    value = r_u32(state + 1016);
    value = r_u32(value + 76);
    w_u32(preamble, value | 0x01000000);
    start = preamble + 8;
    packet = start + 8;
    value = r_u32(state + 1016);
    w_u32(value + 76, preamble & 0xFFFFFF);
    w_u32(start, (packet & 0xFFFFFF) | 0x01000000);
    w_u32(start + 4, 0xE6000002);
    center = xport_gte_read_data(14);
    point = matrix + 120;
    previous = matrix + 112;
    index = 0;
    w_u32(matrix + 112, center);
    do
    {
        value = r_u32(point);
        xport_gte_write_data(0, value);
        value = r_u32(point + 4);
        xport_gte_write_data(1, value);
        xport_gte_execute(0x180001);
        value = sub_800965d4(previous);
        w_u32(previous + 4, value);
        value = xport_gte_read_data(14);
        w_u32(point, value);
        index += 1;
        previous += 8;
        point += 8;
    } while (index < 16);
    value = sub_800965d4(point - 8);
    w_u32(point - 4, value);
    a = r_u32(matrix + 120);
    b = r_u32(matrix + 124);
    clip_center = r_u32(matrix + 116);
    w_u32(matrix + 248, a);
    w_u32(matrix + 252, b);
    xport_gte_write_data(12, center);
    clip_previous = r_u32(matrix + 124);
    point = matrix + 128;
    index = 0;
    screen = a;
    do
    {
        clip_next = r_u32(point + 4);
        value = clip_center & clip_previous & clip_next;
        clip_previous = clip_next;
        if (value != 0)
            screen = r_u32(point);
        else
        {
            value = r_u32(point);
            xport_gte_write_data(13, screen);
            xport_gte_write_data(14, value);
            w_u32(packet + 8, screen);
            screen = value;
            xport_gte_execute(0x1400006);
            w_u32(packet + 16, screen);
            w_u32(packet + 24, center);
            w_u32(packet + 12, 0x00030303);
            w_u32(packet + 20, color);
            area = xport_gte_read_data(24);
            next = packet + 28;
            if ((sint32)area > 0)
            {
                w_u32(packet, (next & 0xFFFFFF) | 0x06000000);
                w_u32(packet + 4, 0x32030303);
                packet = next;
            }
        }
        index += 1;
        point += 8;
    } while (index < 16);
    w_u32(packet, 0x01000000);
    w_u32(packet + 4, 0xE6000000);
    ordering = r_u32(state + 1016);
    ordering -= 76;
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(packet, tag, 7);
    w_masked_u32(ordering, start & 0xFFFFFF, 7);
    value = packet + 8;
    w_u32(state + 4, value);
    return value;
}
