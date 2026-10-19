#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_800928f4(uint32 state)
{
    uint32 value, counter, intensity, scene, ot, packet, base, effect, a;
    FUNCTION_MARKER(0x800928F4, "ACE3.BIN");
    value = r_u32(state);
    if (value == 0)
        return 0;
    counter = r_s16(state + 6);
    value = counter + 1;
    if ((sint32)counter >= 49)
    {
        w_u32(state, 0);
        return value;
    }
    w_u16(state + 6, value);
    if ((counter - 4) < 13)
    {
        if ((sint32)counter < 8)
            intensity = (8 - counter) << 8;
        else
            intensity = (counter - 8) << 6;
        sub_80083054(state + 32, state + 20, intensity, 2, 0, 0);
    }
    if ((counter - 4) < 5)
    {
        intensity = (0x400u >> ((counter - 4) & 31)) + 0x100;
        scene = r_u32(0x800BCE54);
        w_u16(0x1F800000, 0xFF60);
        w_u16(0x1F800002, 0xFF88);
        w_u16(0x1F800004, 320);
        w_u16(0x1F800006, 240);
        ot = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        packet = sub_80093b2c(ot + 8, packet, 0x1F800000, intensity);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, packet);
    }
    if (counter == 9)
    {
        base = r_u32(0x800BCF90);
        effect = sub_8007fdb4(base + 0x33C0, 1, 6);
        if (effect)
        {
            w_u16(effect, 0);
            value = game_random();
            w_u16(effect + 2, (value & 7) + 60);
            value = r_u32(state + 8);
            w_u32(effect + 4, value);
            value = r_u32(state + 12);
            w_u32(effect + 8, value);
            a = r_u32(state + 16);
            w_u16(effect + 16, 0x400);
            w_u16(effect + 18, 0);
            w_u16(effect + 20, 1);
            w_u32(effect + 12, a);
            value = game_random();
            w_u16(effect + 22, (value & 0xFF) + 2000);
        }
    }
    if ((sint32)counter < 16)
        return 48;
    intensity = (48 - counter) << 5;
    value = game_random();
    scene = r_u32(0x800BCE54);
    intensity += value & 0x1FF;
    ot = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    packet = sub_80084474(ot + 8, packet, intensity);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, packet);
    return packet;
}

void sub_80084d04(uint32 state, uint32 count)
{
    uint32 group, masks, record, mask, index, value, limit, flags, x, y, z, px, pz, py, shift, gravity;
    FUNCTION_MARKER(0x80084D04, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    masks = state;
    record = state + (count << 2);
    for (group = 0; (sint32)group < (sint32)count; ++group, masks += 4)
    {
        mask = r_u32(masks);
        index = 0;
        while (mask)
        {
            if (mask & 1)
            {
                value = r_u16(record) + 1;
                w_u16(record, value);
                limit = r_s16(record + 2);
                if ((sint32)(sint16)value >= (sint32)limit)
                {
                    value = r_u32(masks);
                    w_u32(masks, value & ~(1u << (index & 31)));
                }
                else
                {
                    flags = r_u16(record + 22);
                    if (flags & 0xFF)
                    {
                        x = (uint32)r_s16(record + 16) << 8;
                        y = (uint32)r_s16(record + 18) << 8;
                        z = (uint32)r_s16(record + 20) << 8;
                        px = r_u32(record + 4);
                        pz = r_u32(record + 12);
                        w_u32(record + 4, px + x);
                        py = r_u32(record + 8);
                        w_u32(record + 12, pz + z);
                        w_u32(record + 8, py + y);
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
                            w_u16(record + 16, (uint32)((sint32)x >> 8));
                            w_u16(record + 18, (uint32)((sint32)y >> 8));
                            w_u16(record + 20, (uint32)((sint32)z >> 8));
                        }
                    }
                }
            }
            ++index;
            record += 28;
            mask >>= 1;
        }
        record = state + (count << 2) + (group + 1) * 0x380;
    }
}

void sub_80089ed8(uint32 state)
{
    uint32 mask, record, index, value, limit, current;
    FUNCTION_MARKER(0x80089ED8, "ACE3.BIN");
    mask = r_u32(state);
    record = state + 4;
    index = 0;
    while (mask)
    {
        if (mask & 1)
        {
            value = r_u16(record);
            limit = r_s16(record + 2);
            ++value;
            w_u16(record, value);
            if ((sint32)(sint16)value >= (sint32)limit)
            {
                current = r_u32(state);
                w_u32(state, current & ~(1u << (index & 31)));
            }
        }
        ++index;
        mask >>= 1;
        record += 24;
    }
}

uint32 sub_8007ece8(uint32 kind, uint32 position, const uint32 *native_position)
{
    uint32 target, flags, a, b, x, y, z;
    FUNCTION_MARKER(0x8007ECE8, "ACE3.BIN");
    target = sub_8007df28(3);
    flags = r_u32(target);
    w_u8(target + 4, kind);
    a = r_u32(target + 4) & 0xFFF;
    b = r_u32(target + 8) & 0xFFF;
    flags = (flags & 0xFFFFFE00) | 0x33;
    w_u32(target, flags);
    x = native_position ? native_position[0] : r_u32(position);
    y = native_position ? native_position[1] : r_u32(position + 4);
    z = native_position ? native_position[2] : r_u32(position + 8);
    w_u32(target, (flags & 0xFFF) | (x & 0xFFFFF000));
    w_u32(target + 4, a | (y & 0xFFFFF000));
    w_u32(target + 8, b | (z & 0xFFFFF000));
    return target;
}

uint32 sub_80082d80(uint32 position, uint32 scale, const uint32 *native_position)
{
    uint32 value, size, base, effect, leading, mask, half, offset, coordinate;
    FUNCTION_MARKER(0x80082D80, "ACE3.BIN");
    value = game_random();
    value = scale + (value & 0x3F);
    size = value - 32;
    if ((sint32)size <= 0)
        return value;
    base = r_u32(0x800BCF90);
    effect = sub_8007fdb4(base + 0x600, 3, 7);
    if (effect == 0)
        return 0;
    xport_gte_write_data(30, size);
    value = (uint32)((sint32)size >> 4) + 4;
    w_u32(effect, (value << 16) | 1);
    w_u32(effect + 24, (size << 16) | 0x130);
    w_u16(effect + 22, 0);
    leading = xport_gte_read_data(31);
    mask = (1u << ((31 - leading) & 31)) - 1;
    half = 1u << ((30 - leading) & 31);
    value = game_random();
    offset = ((value & mask) - half) << 10;
    coordinate = native_position ? native_position[0] : r_u32(position);
    w_u32(effect + 4, coordinate + offset);
    value = game_random();
    offset = ((value & mask) - half) << 10;
    coordinate = native_position ? native_position[2] : r_u32(position + 8);
    w_u32(effect + 12, coordinate + offset);
    value = native_position ? native_position[1] : r_u32(position + 4);
    w_u32(effect + 8, value);
    return value;
}

uint32 sub_80082e80(uint32 position, uint32 vector, uint32 height)
{
    uint32 scene, value, scale, input[3], x, y, z, speed, base, effect, flags;
    FUNCTION_MARKER(0x80082E80, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    value = r_u32(scene) & 0x100;
    if (value)
        return value;
    value = (uint32)((sint32)(r_u32(position + 4) - height) >> 12);
    scale = value + 0x400;
    if ((sint32)scale < 0)
        return value;
    base = r_u32(0x800BD098);
    value = r_u32(base + 56) & 15;
    if (value != 1)
        return value;
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    speed = sub_80083f40((uint32)((sint32)x >> 4), (uint32)((sint32)y >> 4), (uint32)((sint32)z >> 4)) << 4;
    if ((sint32)speed <= 0x88BF)
        return 0xFFFD0000;
    scale = scale - (uint32)((sint32)scale >> 2) + (uint32)((sint32)(speed + 0xFFFD543E) >> 9);
    value = (sint32)scale < 0x400;
    if ((sint32)scale < 0)
        return value;
    if ((sint32)scale < 0x400)
    {
        value = r_s16(0x800B65C4 + (0x400 - scale) * 2);
        scale = (uint32)((sint32)(0x1000 - value) >> 4);
    }
    value = (sint32)scale < 0x100;
    if ((sint32)scale < 80)
        return value;
    if ((sint32)scale >= 0x100)
        scale = 255;
    x = r_u32(vector);
    y = r_u32(position);
    input[0] = y - (x << 1);
    input[1] = height;
    x = r_u32(vector + 8);
    y = r_u32(position + 8);
    input[2] = y - (x << 1);
    effect = sub_8007ece8(3, 0, input);
    flags = r_u32(effect + 8);
    w_u32(effect + 8, (flags & 0xFFFFF00F) | ((scale & 0xFF) << 4));
    return sub_80082d80(0, scale, input);
}

uint32 sub_8007fdb4(uint32 state, uint32 kind, uint32 size)
{
    uint32 target, group, mask, bit, step, stride;
    FUNCTION_MARKER(0x8007FDB4, "ACE3.BIN");
    target = state + (kind << 2);
    if ((sint32)kind <= 0)
        return 0;
    step = size << 2;
    stride = size << 7;
    for (group = 0; (sint32)group < (sint32)kind; ++group, state += 4, target += stride)
    {
        mask = r_u32(state);
        if (mask == 0xFFFFFFFF)
            continue;
        bit = 1;
        while (mask & bit)
        {
            bit <<= 1;
            target += step;
        }
        w_u32(state, mask | bit);
        return target;
    }
    return 0;
}

uint32 sub_80082ff0(void)
{
    uint32 base, target, flags, common, bit;
    FUNCTION_MARKER(0x80082FF0, "ACE3.BIN");
    base = r_u32(0x800BCF90);
    target = base + 0x3968;
    flags = r_u32(base + 0x3964);
    if ((sint32)flags < 0)
        return 0;
    common = flags & (flags >> 16);
    bit = 1;
    while (common & bit)
    {
        bit <<= 1;
        target += 28;
    }
    w_u32(base + 0x3964, flags | bit | (bit << 16));
    return target;
}

uint32 sub_80083054(uint32 origin, uint32 vector, uint32 scale, uint32 kind, const uint32 *native_origin, const uint32 *native_vector)
{
    uint32 target, a, b, c;
    FUNCTION_MARKER(0x80083054, "ACE3.BIN");
    target = sub_80082ff0();
    if (target == 0)
        return 0;
    w_u16(target, scale);
    w_u16(target + 2, kind);
    if (native_origin)
    {
        a = native_origin[0];
        b = native_origin[1];
        c = native_origin[2];
    }
    else
    {
        a = r_u32(origin);
        b = r_u32(origin + 4);
        c = r_u32(origin + 8);
    }
    w_u32(target + 4, a);
    w_u32(target + 8, b);
    w_u32(target + 12, c);
    if (native_vector)
    {
        a = native_vector[0];
        b = native_vector[1];
        c = native_vector[2];
    }
    else
    {
        a = r_u32(vector);
        b = r_u32(vector + 4);
        c = r_u32(vector + 8);
    }
    w_u32(target + 16, a);
    w_u32(target + 20, b);
    w_u32(target + 24, c);
    return target;
}

void sub_80090148(uint32 state)
{
    uint32 count, packed, scene, scale, camera, data, value, mode, table, object;
    FUNCTION_MARKER(0x80090148, "ACE3.BIN");
    count = r_u32(state);
    if ((sint32)count <= 0)
        return;
    packed = r_u32(state + 4);
    w_u32(state, (sint32)count < ((sint32)packed >> 16) ? count + 1 : 0);
    scene = r_u32(0x800BCE54);
    if ((r_u32(scene) & 2) == 0 || (packed & 0xFFFF) != 5)
        return;
    scale = (count - 1) << 6;
    camera = r_u32(scene + 40);
    data = r_u32(0x800BCE60);
    value = r_u32(camera + 80);
    mode = value == 1;
    value = r_u32(data + 296);
    if ((value & 0x800) == 0)
    {
        scale += 256;
        if ((sint32)scale >= 1025)
            scale = 1024;
    }
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    sub_800831d0(object + 8, object + 24, scale, mode);
}

void sub_80091654(uint32 state)
{
    uint32 active, pending, cursor, bit;
    FUNCTION_MARKER(0x80091654, "ACE3.BIN");
    active = r_u16(state);
    if (active == 0)
        return;
    pending = r_u16(state + 2);
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
    w_u16(state, active);
    w_u16(state + 2, pending);
}

uint32 sub_8003aa60(void)
{
    uint32 selected, flags, target, value, scene, mode, data, variant;
    FUNCTION_MARKER(0x8003AA60, "ACE3.BIN");
    selected = r_u8(0x800C6D6E);
    if (selected != 255)
    {
        flags = r_u32(0x800C6FD0 + selected * 4);
        if ((flags & 1) == 0)
            value = 1;
        else
        {
            target = r_u32(0x800C6ED0 + selected * 4);
            value = ((r_u32(target) & 1) == 0) || ((flags & 0x40000) != 0);
        }
        if (value)
            w_u8(0x800C6D6E, sub_800464b0(0));
    }
    scene = r_u32(0x800BCE54);
    mode = r_u32(scene + 4);
    if (mode & 4)
    {
        data = r_u32(0x800BCE60);
        if (r_s16(data + 908) == 3 && r_u32(scene + 8) == 0)
        {
            value = r_u8(0x800C6D6E);
            if (value == 2)
                return 2;
            w_u8(0x800C6D6E, 2);
            sub_800464b0(0);
            return sub_800463b8(0, 2);
        }
        sub_8003abec();
        return sub_8003ac8c();
    }
    if (mode & 0x800)
    {
        variant = r_u32(0x800BCEA8);
        selected = r_u8(0x800B1054 + variant);
        value = r_u8(0x800C6D6E);
        if (selected == value)
            return value;
        w_u8(0x800C6D6E, selected);
        sub_800464b0(0);
        return sub_800463b8(0, selected);
    }
    w_u8(0x800C6D6E, 255);
    sub_800464b0(0);
    return sub_800463b8(0, 255);
}

uint32 sub_8003abec(void)
{
    uint32 data, flags, direction, selected, current;
    FUNCTION_MARKER(0x8003ABEC, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    flags = r_u32(data + 300);
    direction = 0;
    if ((flags & 0x200000) == 0)
    {
        if ((flags & 0x400000) == 0)
            return 0;
        direction = 1;
    }
    current = r_u8(0x800C6D6E);
    selected = sub_8003afc0(current, direction, 0);
    if (selected == 255)
        return 0;
    current = r_u8(0x800C6D6E);
    if (selected != current)
    {
        w_u8(0x800C6D6E, selected);
        sub_800464b0(0);
        sub_800463b8(0, selected);
    }
    return 0;
}

uint32 sub_8003ac8c(void)
{
    uint32 current, target, value, selected;
    FUNCTION_MARKER(0x8003AC8C, "ACE3.BIN");
    current = r_u8(0x800C6D6E);
    if (current != 255)
    {
        current = r_u8(0x800C6D6E);
        target = r_u32(0x800C6ED0 + current * 4);
        if ((r_u32(target) & 2) || (r_u32(target + 32) & 0x20000))
        {
            w_u32(0x800BCF98, 0);
            return 0;
        }
        value = r_u32(0x800BCF98);
        w_u32(0x800BCF98, value + 1);
    }
    selected = sub_8003afc0(255, 0, 1);
    if (selected == 255)
        return 0;
    current = r_u8(0x800C6D6E);
    if (selected == current)
        return current;
    if (current != 255)
    {
        if (r_s32(0x800BCF98) < 31)
            return 1;
        target = r_u32(0x800C6ED0 + selected * 4);
        if (r_s16(target + 30) < 31)
            return 1;
    }
    w_u8(0x800C6D6E, selected);
    sub_800464b0(0);
    value = sub_800463b8(0, selected);
    w_u32(0x800BCF98, 0);
    return value;
}

uint32 sub_8003b2e4(uint32 index)
{
    uint32 target, flags, distance;
    FUNCTION_MARKER(0x8003B2E4, "ACE3.BIN");
    target = r_u32(0x800C6ED0 + index * 4);
    if (target == 0)
        return index;
    flags = r_u32(target);
    if ((flags & 1) == 0)
        return index;
    if (flags & 2)
        index += 0x400000;
    distance = r_u32(target + 4);
    return index + ((0x8000 - distance) << 6);
}

uint32 sub_8003afc0(uint32 current, uint32 direction, uint32 automatic)
{
    uint32 scores[64], current_score, preferred, lower, upper, best, nearest, index, table, object, flags, state, target, score;
    uint32 maximum, minimum, below, above;
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
        current_score = sub_8003b2e4(current & 0xFF);
        preferred = (sint32)current_score > 0x3FFFFF;
    }
    for (index = 2; index < 64; ++index)
    {
        scores[index] = 0xFFFFFFFF;
        if (index == (current & 0xFF))
            continue;
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        flags = r_u32(object);
        if ((flags & 0x4000) == 0)
            continue;
        state = r_u32(0x800C6FD0 + index * 4);
        if ((state & 1) == 0 || (state & 0x8000) == 0 || (flags & 0x40))
            continue;
        if ((state & 0x40000) && (state & 0x100) == 0)
            continue;
        if (automatic == 1 && (flags & 0x100))
            continue;
        target = r_u32(0x800C6ED0 + index * 4);
        if ((r_u32(target) & 1) == 0)
            continue;
        score = sub_8003b2e4(index);
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

uint32 sub_8003adb4(void)
{
    uint32 data, value, flags, byte, scene, root, record, player, listener, voice, distance, vector[3], divisor;
    FUNCTION_MARKER(0x8003ADB4, "ACE3.BIN");
    data = r_u32(0x800BCEB4);
    value = r_u32(data + 20);
    w_u32(0x800C6D60, value);
    flags = r_u32(0x800C6ECC);
    byte = r_u8(data + 28);
    scene = r_u32(0x800BCE54);
    w_u32(0x800C6ECC, flags & 0xFFFFFFFD);
    w_u16(0x800C6D64, byte);
    flags = r_u32(scene + 4);
    value = (~flags) & 4;
    if (value)
        return value;
    if (r_u32(scene + 12) == 0)
        sub_8002ed08(0x800BCFA0, 3, 3, 0x400, 16);
    root = r_u32(0x800BD080);
    if ((r_u32(root) & 1) == 0)
        return 0x800C0000;
    value = r_u8(root + 8);
    record = root + value * 164 + 228;
    scene = r_u32(0x800BCE54);
    listener = r_u32(scene + 40);
    data = r_u32(0x800BCE5C);
    player = r_u32(data);
    sub_8002e6dc(0x800C7160, listener + 4, record, 0, 0, 0);
    sub_8002ed2c(0x800C7160, 0x800BCFA0);
    voice = r_u8(0x800BCFA3);
    if (voice != 255)
    {
        sub_8002e67c(voice);
        sub_8002e8dc(r_u8(0x800BCFA3), 0x800C7160, 0);
    }
    distance = sub_80083dfc(record, player + 8, 0, 0);
    if ((sint32)distance >= 0x2000)
        return 0;
    sub_80044f94(record, player + 8, 0, 0, vector);
    value = vector[2];
    if ((sint32)value < 0)
        return value;
    value = sub_80045090(0, vector);
    if ((sint32)value >= 0x600 && (sint32)distance >= 0x800)
        return 0;
    flags = r_u32(0x800C6ECC);
    divisor = (uint32)((sint32)distance >> 9);
    w_u32(0x800C6ECC, flags | 2);
    if ((sint32)divisor < 2)
        divisor = 2;
    value = r_u32(0x800C6D60);
    if ((sint32)value % (sint32)divisor)
        return value;
    return sub_8002dbe0(44);
}

uint32 sub_8007f0f8(void)
{
    uint32 root, value, index, first, second, table, object;
    FUNCTION_MARKER(0x8007F0F8, "ACE3.BIN");
    root = r_u32(0x800BD0B8);
    if (r_u32(root) == 0)
    {
        value = r_u32(root + 4);
        w_u32(root + 16, value);
        w_u32(root + 12, value);
        first = 24;
        second = 0x818;
        for (index = 0; index < 64; ++index, first += 32, second += 32)
        {
            root = r_u32(0x800BD0B8);
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            copy_words(root + first, object, 8);
            root = r_u32(0x800BD0B8);
            copy_words(root + second, object, 8);
        }
    }
    sub_8007e720();
    sub_8007dfc4(0, 0, 0, 0);
    root = r_u32(0x800BD0B8);
    value = r_u32(root) + 1;
    w_u32(root, value);
    return value;
}

uint32 sub_8007e720(void)
{
    uint32 index, offset, table, root, selector, object, snapshot, changed, forced, flags;
    FUNCTION_MARKER(0x8007E720, "ACE3.BIN");
    for (index = 0, offset = 0x818; index < 64; ++index, offset += 32)
    {
        forced = 0;
        table = r_u32(0x800BCE5C);
        root = r_u32(0x800BD0B8);
        selector = r_u32(root + 0x184C);
        object = r_u32(table + index * 4);
        snapshot = root + offset;
        if (index == (selector & 63))
            forced = (~selector) >> 31;
        flags = r_u32(snapshot);
        if ((flags & 0x14000) != 0x14000)
        {
            flags = r_u32(object);
            if ((flags & 0x14000) != 0x14000 && !forced)
            {
                sub_8007e6d8(object, snapshot);
                continue;
            }
        }
        changed = sub_8007e5bc(snapshot, object);
        flags = r_u32(object);
        if ((flags & 0x2000) == 0 && (changed & 1) == 0)
            continue;
        if (changed & 1)
            sub_8007dfc4(1, index, (flags & 0x14000) == 0x14000, 0);
        if (changed & 4)
        {
            flags = r_u8(object + 4);
            selector = r_u8(object + 5);
            sub_8007dfc4(2, index, flags, selector);
        }
        if (changed & 8)
            sub_8007dfc4(3, index, r_s16(object + 6), 0);
        if (changed & 16)
            sub_8007e1cc(0, index, object + 8);
        sub_8007e6d8(object, snapshot);
    }
    return 0;
}

uint32 sub_8007e5bc(uint32 snapshot, uint32 object)
{
    uint32 old_value, old_flags, flags, value, changed, x0, y0, z0, x1, y1, z1;
    FUNCTION_MARKER(0x8007E5BC, "ACE3.BIN");
    old_value = r_s16(snapshot + 4);
    old_flags = r_u32(snapshot);
    flags = r_u32(object);
    value = r_s16(object + 4);
    changed = ((flags ^ old_flags) & 0x14000) != 0;
    if (old_value != value)
        changed |= 4;
    value = r_u8(object + 5);
    if (value & 4)
    {
        old_value = r_s16(snapshot + 6);
        value = r_s16(object + 6);
        if (old_value != value)
            changed |= 8;
    }
    x0 = r_u32(snapshot + 8);
    y0 = r_u32(snapshot + 12);
    z0 = r_u32(snapshot + 16);
    x1 = r_u32(object + 8);
    y1 = r_u32(object + 12);
    z1 = r_u32(object + 16);
    if (((sint32)x0 >> 12) != ((sint32)x1 >> 12))
        changed |= 16;
    if (((sint32)y0 >> 12) != ((sint32)y1 >> 12))
        changed |= 16;
    if (((sint32)z0 >> 12) != ((sint32)z1 >> 12))
        changed |= 16;
    x0 = r_u16(snapshot + 24);
    y0 = r_u16(snapshot + 26);
    y1 = r_u16(object + 26);
    z0 = r_u16(snapshot + 28);
    x1 = r_u16(object + 24);
    z1 = r_u16(object + 28);
    if (((sint32)(x0 << 16) >> 18) != ((sint32)(x1 << 16) >> 18))
        changed |= 16;
    if (((sint32)(y0 << 16) >> 18) != ((sint32)(y1 << 16) >> 18))
        changed |= 16;
    if (((sint32)(z0 << 16) >> 18) != ((sint32)(z1 << 16) >> 18))
        changed |= 16;
    return changed;
}

uint32 sub_8007e6d8(uint32 object, uint32 snapshot)
{
    uint32 flags, value, x, y, z;
    FUNCTION_MARKER(0x8007E6D8, "ACE3.BIN");
    flags = r_u32(object);
    value = (~flags) & 0x4000;
    if (value)
    {
        w_u32(snapshot, flags);
        return value;
    }
    value = r_u32(object + 4);
    w_u32(snapshot, flags);
    w_u32(snapshot + 4, value);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    w_u32(snapshot + 8, x);
    w_u32(snapshot + 12, y);
    w_u32(snapshot + 16, z);
    return z;
}

uint32 sub_8007dfc4(uint32 action, uint32 index, uint32 value, uint32 extra)
{
    uint32 target, packed, result;
    FUNCTION_MARKER(0x8007DFC4, "ACE3.BIN");
    target = sub_8007df28(1);
    if (action >= 5)
        return 0x80020000;
    if (action == 0)
    {
        w_u32(target, 1);
        return 1;
    }
    packed = r_u32(target);
    packed = ((packed & 0xFFFFFFF0) | 1) & 0xFFFFFF8F;
    packed = (packed & 0xFFFFFC7F) | ((action & 7) << 7);
    if (action == 4)
    {
        w_u32(target, packed);
        if ((sint32)index < 0)
            index = 0;
        result = (index & 63) << 10;
        w_u32(target, (packed & 0xFFFF03FF) | result);
        w_u16(target + 2, 0);
        return result;
    }
    packed = (packed & 0xFFFF03FF) | ((index & 63) << 10);
    if (action == 1)
    {
        packed = (packed & 0xFFFEFFFF) | ((value & 1) << 16);
        packed &= 0x1FFFF;
    }
    else if (action == 3)
        packed = (packed & 0xFF81) | ((value & 0x7FFF) << 16);
    w_u32(target, packed);
    if (action == 2)
    {
        w_u8(target + 2, value);
        w_u8(target + 3, extra);
    }
    return packed;
}

uint32 sub_8007e1cc(uint32 action, uint32 index, uint32 object)
{
    uint32 target, value, a, b, c, x, y, z;
    FUNCTION_MARKER(0x8007E1CC, "ACE3.BIN");
    target = sub_8007df28(4);
    if (action)
        return target;
    value = r_u32(target);
    value = ((value & 0xFFFFFFF0) | 4) & 0xFFFFFF8F;
    value = (((value | 16) & 0xFFFFFE7F) & 0xFFFF81FF) | ((index & 63) << 9);
    w_u32(target, value);
    a = r_u16(object + 16);
    x = r_u32(object);
    b = r_u16(object + 18);
    c = r_u16(object + 20);
    a = ((a >> 2) & 0xFFF) | (x & 0xFFFFF000);
    y = r_u32(object + 4);
    b = ((b >> 2) & 0xFFF) | (y & 0xFFFFF000);
    z = r_u32(object + 8);
    w_u32(target + 4, a);
    w_u32(target + 8, b);
    value = z & 0xFFFFF000;
    w_u32(target + 12, ((c >> 2) & 0xFFF) | value);
    return value;
}

uint32 sub_800332e4(uint32 object, uint32 matrix, uint32 other_matrix)
{
    uint32 a, b, c, d, e;
    FUNCTION_MARKER(0x800332E4, "ACE3.BIN");
    rotation_build(object + 20, matrix, 0, 0);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    w_u32(other_matrix + 4, a);
    w_u32(other_matrix, b);
    w_u16(other_matrix, a);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    w_u32(other_matrix + 12, c);
    w_u32(other_matrix + 8, d);
    w_u16(other_matrix + 12, b);
    w_u16(other_matrix + 8, c);
    e = r_s16(matrix + 16);
    w_u16(other_matrix + 4, d);
    w_u16(other_matrix + 16, e);
    return other_matrix;
}

uint32 sub_80033324(void)
{
    uint32 scene, matrix, a, b, c, d, e, ot, packet;
    FUNCTION_MARKER(0x80033324, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    matrix = scene + 76;
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    e = r_u32(matrix + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    scene = r_u32(0x800BCE54);
    matrix = scene + 76;
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    c = r_u32(matrix + 28);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    scene = r_u32(0x800BCE54);
    a = r_u32(scene + 172);
    b = r_u32(scene + 176);
    c = r_u32(scene + 180);
    xport_gte_write_control(13, a << 4);
    xport_gte_write_control(14, b << 4);
    xport_gte_write_control(15, c << 4);
    scene = r_u32(0x800BCE54);
    matrix = scene + 140;
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    e = r_u32(matrix + 16);
    xport_gte_write_control(16, a);
    xport_gte_write_control(17, b);
    xport_gte_write_control(18, c);
    xport_gte_write_control(19, d);
    xport_gte_write_control(20, e);
    scene = r_u32(0x800BCE54);
    ot = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    packet = sub_8007db00(ot + 4, packet);
    scene = r_u32(0x800BCE54);
    matrix = scene + 76;
    w_u32(scene + 20, packet);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    e = r_u32(matrix + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    scene = r_u32(0x800BCE54);
    matrix = scene + 76;
    a = r_u32(matrix + 20);
    b = r_u32(matrix + 24);
    c = r_u32(matrix + 28);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    scene = r_u32(0x800BCE54);
    ot = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    packet = sub_80085204(ot + 4, packet);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, packet);
    return packet;
}

uint32 sub_800333d8(void)
{
    uint32 scene, matrix, a, b, c, d, e, ot, packet;
    FUNCTION_MARKER(0x800333D8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    matrix = scene + 76;
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    e = r_u32(matrix + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(0x800B09C8);
    b = r_u32(0x800B09CC);
    c = r_u32(0x800B09D0);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    scene = r_u32(0x800BCE54);
    ot = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    packet = sub_8006c5ec(ot + 4, packet);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, packet);
    return packet;
}

uint32 sub_8007db00(uint32 ot, uint32 packet)
{
    uint32 next;
    FUNCTION_MARKER(0x8007DB00, "ACE3.BIN");
    next = sub_8007c00c(ot, packet);
    return sub_8007cfa4(ot, next);
}

void sub_80092850(uint32 state)
{
    uint32 mask, record, a, b, c;
    FUNCTION_MARKER(0x80092850, "ACE3.BIN");
    mask = r_u16(state);
    record = state + 4;
    if (mask == 0)
        return;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(0x1F800118, a);
    w_u32(0x1F80011C, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(0x1F800120, a);
    w_u32(0x1F800124, b);
    w_u32(0x1F800128, c);
    do
    {
        if (mask & 1)
            sub_8009236c(0x1F800000, 0x1F80001C, record);
        mask >>= 1;
        record += 28;
    } while (mask);
}

uint32 sub_80090a74(uint32 records, uint32 ot, uint32 packet, const uint8 *native_records)
{
    uint32 cursor, count, scene, camera, angle, depth, first, second, target, value, color;
    sint32 z0, z1, x0, x1, y0, y1;
    FUNCTION_MARKER(0x80090A74, "ACE3.BIN");
    cursor = 0;
    count = native_records ? xport_load_le32(native_records + 12) : r_u32(records + 12);
    if ((sint32)count < 0)
        return packet;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x52000000);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    angle = r_u16(camera + 36);
    angle = (uint32)((sint32)(angle << 16) >> 18);
    do
    {
        if (count)
        {
            z0 = native_records ? (sint16)xport_load_le16(native_records + cursor + 8) : r_s16(records + cursor + 8);
            if ((sint32)angle < z0)
            {
                depth = (sint32)count < 28 ? (28 - count) << 7 : 0;
                xport_gte_write_data(8, depth);
                xport_gte_write_data(20, 0x6433D);
                xport_gte_write_data(21, 0x14CFBF);
                first = native_records ? xport_load_le32(native_records + cursor) : r_u32(records + cursor);
                second = native_records ? xport_load_le32(native_records + cursor + 4) : r_u32(records + cursor + 4);
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
                    w_u32(packet + 8, first);
                    w_u32(packet + 16, second);
                    z1 = native_records ? (sint16)xport_load_le16(native_records + cursor + 10) : r_s16(records + cursor + 10);
                    target = ot + ((uint32)((z0 + z1) >> 4) << 2);
                    value = r_u32(target);
                    w_u32(packet, value | 0x04000000);
                    w_u32(target, packet & 0xFFFFFF);
                    color = xport_gte_read_data(20);
                    w_u32(packet + 4, color);
                    color = xport_gte_read_data(21);
                    w_u32(packet + 12, color);
                    packet += 20;
                }
            }
        }
        cursor += 16;
        count = native_records ? xport_load_le32(native_records + cursor + 12) : r_u32(records + cursor + 12);
    } while ((sint32)count >= 0);
    return packet;
}

uint32 sub_800317f0(uint32 origin, uint32 target, uint32 output, uint16 *native_output)
{
    MATRIX matrix = {0};
    uint32 a, b, c, d, e, x, y, z, sum, distance, horizontal, shift, value, angle;
    uint32 input[3], high[3], low[3], mac[3], index, magnitude;
    FUNCTION_MARKER(0x800317F0, "ACE3.BIN");
    sub_800308f0(origin + 16, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le32((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(origin);
    b = r_u32(target);
    c = r_u32(origin + 4);
    x = (uint32)((sint32)(a - b) >> 12);
    b = r_u32(target + 4);
    d = r_u32(target + 8);
    a = r_u32(origin + 8);
    y = (uint32)((sint32)(c - b) >> 12);
    z = (uint32)((sint32)(a - d) >> 12);
    sum = ((sint32)x < 0 ? 0 - x : x) + ((sint32)y < 0 ? 0 - y : y) + ((sint32)z < 0 ? 0 - z : z);
    distance = 0x100000;
    if ((sint32)sum <= 0xAFFF)
        distance = sub_80083f40(x, y, z);
    input[0] = x;
    input[1] = y;
    input[2] = z;
    for (index = 0; index < 3; ++index)
    {
        value = input[index];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        high[index] = (uint32)((sint32)magnitude >> 15);
        low[index] = magnitude & 0x7FFF;
        if ((sint32)value < 0)
        {
            high[index] = 0 - high[index];
            low[index] = 0 - low[index];
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    mac[0] = xport_gte_read_data(25);
    mac[1] = xport_gte_read_data(26);
    mac[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    x = a + (mac[0] << 3);
    y = b + (mac[1] << 3);
    z = c + (mac[2] << 3);
    sum = ((sint32)x < 0 ? 0 - x : x) + ((sint32)z < 0 ? 0 - z : z);
    horizontal = 0x100000;
    if ((sint32)sum <= 0x57FFF)
    {
        shift = (sint32)sum > 0xAFFF ? 3 : 0;
        horizontal = sub_80083f40((uint32)((sint32)x >> shift), 0, (uint32)((sint32)z >> shift)) << shift;
    }
    value = sub_80030d54(y, horizontal);
    if (native_output)
        native_output[0] = (uint16)value;
    else
        w_u16(output, value);
    value = 0 - sub_80030d54(x, 0 - z);
    if (native_output)
        native_output[1] = (uint16)value;
    else
        w_u16(output + 2, value);
    value = sub_80030d54(x, y);
    angle = native_output ? (uint32)(sint32)(sint16)native_output[1] : (uint32)r_s16(output + 2);
    if ((sint32)angle < 0)
        angle = 0 - angle;
    if (native_output)
        native_output[2] = (uint16)(0 - value);
    else
        w_u16(output + 4, 0 - value);
    if ((sint32)angle >= 0x1001)
        distance = 0 - distance;
    return distance;
}

void sub_800779c8(uint32 origin, uint32 index, uint32 output, uint16 *native_output)
{
    uint32 table, object;
    FUNCTION_MARKER(0x800779C8, "ACE3.BIN");
    if ((sint32)index > 0)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if (r_u32(object) & 0x4000)
        {
            sub_800317f0(origin, object + 8, output, native_output);
            if (native_output)
                native_output[2] = 0;
            else
                w_u16(output + 4, 0);
            return;
        }
    }
    if (native_output)
    {
        native_output[0] = 0;
        native_output[1] = 0;
        native_output[2] = 0;
    }
    else
    {
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0);
    }
}

/* Audited native-vector form of SDK 0x800A6B18 */
void rotate_native_vector_a6b18(uint32 vector[3])
{
    uint32 high[3], low[3], mac[3], value, magnitude, index, a, b, c;
    for (index = 0; index < 3; ++index)
    {
        value = vector[index];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        high[index] = (uint32)((sint32)magnitude >> 15);
        low[index] = magnitude & 0x7FFF;
        if ((sint32)value < 0)
        {
            high[index] = 0 - high[index];
            low[index] = 0 - low[index];
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    mac[0] = xport_gte_read_data(25);
    mac[1] = xport_gte_read_data(26);
    mac[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    vector[0] = a + (mac[0] << 3);
    vector[1] = b + (mac[1] << 3);
    vector[2] = c + (mac[2] << 3);
}

uint32 sub_800313a8(uint32 angles, uint32 delta, uint32 out, uint16 *native_output)
{
    static const uint32 thresholds[6] = {0xF91, 0xF89, 0xFC9, 0xFC1, 0xFE1, 0xFDB};
    static const uint32 limits[6] = {8, 8, 12, 12, 16, 16};
    MATRIX matrix = {0};
    uint32 forward[4], side[4], a, b, c, d, e, index, axis, magnitude, value, pitch, yaw, roll, result;
    uint16 local_angles[3];
    FUNCTION_MARKER(0x800313A8, "ACE3.BIN");
    forward[0] = r_u32(0x800234D8);
    forward[1] = r_u32(0x800234DC);
    forward[2] = r_u32(0x800234E0);
    forward[3] = r_u32(0x800234E4);
    side[0] = r_u32(0x800234E8);
    side[1] = r_u32(0x800234EC);
    side[2] = r_u32(0x800234F0);
    side[3] = r_u32(0x800234F4);
    local_angles[0] = (uint16)r_u16(delta);
    local_angles[1] = (uint16)r_u16(delta + 2);
    local_angles[2] = (uint16)r_u16(delta + 4);
    for (index = 0; index < 6; ++index)
    {
        value = r_s16(angles);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)thresholds[index])
            continue;
        axis = index & 1;
        value = (uint32)(sint32)(sint16)local_angles[axis];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)limits[index])
            local_angles[axis] = (uint16)(local_angles[axis] << 1);
    }
    sub_80030a68(0, 0, &matrix, local_angles);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le32((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    rotate_native_vector_a6b18(forward);
    rotate_native_vector_a6b18(side);
    sub_80030a68(angles, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le32((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    rotate_native_vector_a6b18(forward);
    rotate_native_vector_a6b18(side);
    a = (uint32)((sint32)(0 - forward[1]) >> 4);
    side[0] = (uint32)((sint32)side[0] >> 4);
    side[2] = (uint32)((sint32)side[2] >> 4);
    side[1] = (uint32)((sint32)side[1] >> 4);
    pitch = sub_80030eb8(a);
    value = sub_80030d54(0 - forward[0], forward[2]);
    yaw = 0 - value;
    local_angles[0] = (uint16)(0 - pitch);
    local_angles[1] = (uint16)value;
    local_angles[2] = 0;
    sub_80030bdc(0, 0, &matrix, local_angles);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le32((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    rotate_native_vector_a6b18(side);
    side[0] = (uint32)((sint32)side[0] >> 2);
    side[1] = (uint32)((sint32)side[1] >> 2);
    side[2] = (uint32)((sint32)side[2] >> 2);
    roll = sub_80030d54(side[0], 0 - side[1]);
    if ((sint32)pitch >= 0x1001)
    {
        pitch = 0x2000 - pitch;
        yaw += 0x2000;
        roll += 0x2000;
    }
    if ((sint32)pitch < -0x1000)
    {
        pitch = 0xFFFFE000 - pitch;
        yaw += 0x2000;
        roll += 0x2000;
    }
    if ((sint32)yaw >= 0x2000)
        yaw -= 0x4000;
    if ((sint32)yaw < -0x2000)
        yaw += 0x4000;
    if ((sint32)roll >= 0x2000)
        roll -= 0x4000;
    result = (sint32)roll < -0x2000;
    if (result)
        roll += 0x4000;
    if (native_output)
    {
        native_output[0] = (uint16)pitch;
        native_output[1] = (uint16)yaw;
        native_output[2] = (uint16)roll;
    }
    else
    {
        w_u16(out, pitch);
        w_u16(out + 2, yaw);
        w_u16(out + 4, roll);
    }
    return result;
}

uint32 sub_80077a30(uint32 state)
{
    uint16 angles[3];
    uint32 game, flags, index, origin, target[3], steps[3], value, other, difference, axis, field, source, result;
    FUNCTION_MARKER(0x80077A30, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    index = 0xFFFFFFFF;
    if (flags & 0x2000)
        index = r_u32(state + 188);
    flags = r_u32(state);
    origin = state + 4;
    if (flags & 0x100)
        origin = r_u32(state + 52);
    sub_800779c8(origin, index, 0, angles);
    target[0] = (uint32)(sint32)(sint16)angles[0];
    game = r_u32(0x800BCE60);
    target[1] = (uint32)(sint32)(sint16)angles[1];
    flags = r_u32(game + 312);
    target[2] = (uint32)(sint32)(sint16)angles[2];
    if (flags & 16)
        target[0] -= r_u32(game + 332);
    if (flags & 32)
        target[1] += r_u32(game + 336);
    flags = r_u32(game + 296);
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) < -0x2000)
        w_u16(state + 182, other + 0x4000);
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) >= 0x2001)
        w_u16(state + 182, other - 0x4000);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300) & 0x100000;
    if (flags == 0)
        flags = r_u32(game + 308) & 0x100000;
    if (flags)
    {
        w_u16(state + 180, target[0]);
        w_u16(state + 182, target[1]);
        w_u16(state + 184, target[2]);
        sub_8007786c(state);
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = r_s16(state + 180 + axis * 2);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = (uint32)((sint32)difference >> 2) + 2;
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)value < (sint32)target[axis])
                w_u16(field, other + steps[axis]);
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)target[axis] < (sint32)value)
                w_u16(field, other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                w_u16(field, target[axis]);
        }
    }
    flags = r_u32(state);
    if (flags & 0x100)
    {
        source = r_u32(state + 52);
        sub_800313a8(source + 16, state + 180, 0, angles);
        source = r_u32(state + 52);
        value = (uint32)(sint32)(sint16)angles[0];
        other = r_s16(source + 16);
        target[0] = value - other;
        value = r_s16(source + 18);
        result = r_s16(source + 20);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        sub_800313a8(state + 20, state + 180, 0, angles);
        value = r_s16(state + 20);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = r_s16(state + 22);
        result = r_s16(state + 24);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    w_u16(state + 72, target[0]);
    w_u16(state + 74, target[1]);
    w_u16(state + 76, target[2]);
    return result;
}

uint32 sub_80077dd0(uint32 state)
{
    MATRIX first = {0}, second = {0};
    uint32 a, b, c, d, e, packed, depth, column0[3], column1[3], vector[3];
    uint16 angles[3];
    uint32 game, flags, index, origin, target[3], steps[3], value, other, difference, axis, field, source, result;
    FUNCTION_MARKER(0x80077DD0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    index = 0xFFFFFFFF;
    if (flags & 0x2000)
        index = r_u32(state + 188);
    flags = r_u32(state);
    origin = state + 4;
    if (flags & 0x100)
        origin = r_u32(state + 52);
    sub_800779c8(origin, index, 0, angles);
    target[0] = (uint32)(sint32)(sint16)angles[0];
    game = r_u32(0x800BCE60);
    target[1] = (uint32)(sint32)(sint16)angles[1];
    flags = r_u32(game + 312);
    target[2] = (uint32)(sint32)(sint16)angles[2];
    if (flags & 16)
        target[0] -= r_u32(game + 332);
    if (flags & 32)
        target[1] += r_u32(game + 336);
    flags = r_u32(game + 296);
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) < -0x2000)
        w_u16(state + 182, other + 0x4000);
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) >= 0x2001)
        w_u16(state + 182, other - 0x4000);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300) & 0x100000;
    if (flags == 0)
        flags = r_u32(game + 308) & 0x100000;
    if (flags)
    {
        w_u16(state + 180, target[0]);
        w_u16(state + 182, target[1]);
        w_u16(state + 184, target[2]);
        sub_8007786c(state);
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = r_s16(state + 180 + axis * 2);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = (uint32)((sint32)(difference + 2) >> 2);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)value < (sint32)target[axis])
                w_u16(field, other + steps[axis]);
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)target[axis] < (sint32)value)
                w_u16(field, other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                w_u16(field, target[axis]);
        }
    }
    source = r_u32(state + 52);
    rotation_build(source + 16, 0, &first, 0);
    rotation_build(state + 180, 0, &second, 0);
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    packed = xport_load_le16((uint8 *)&second) | (xport_load_le32((uint8 *)&second + 4) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 12);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 2) | (xport_load_le32((uint8 *)&second + 8) << 16);
    depth = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&second + 14);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 4) | (xport_load_le32((uint8 *)&second + 8) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 16);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    xport_store_le32((uint8 *)&first, (column0[0] & 0xFFFF) | (column1[0] << 16));
    xport_store_le32((uint8 *)&first + 12, (column0[2] & 0xFFFF) | (column1[2] << 16));
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    xport_store_le32((uint8 *)&first + 4, (a & 0xFFFF) | (column0[1] << 16));
    xport_store_le32((uint8 *)&first + 8, (column1[1] & 0xFFFF) | (b << 16));
    xport_store_le32((uint8 *)&first + 16, xport_gte_read_data(11));
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(state + 44);
    xport_gte_write_data(0, a);
    b = r_u32(state + 48);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    vector[0] = xport_gte_read_data(25);
    vector[1] = xport_gte_read_data(26);
    vector[2] = xport_gte_read_data(27);
    w_u32(state + 56, vector[0] << 6);
    flags = r_u32(state);
    w_u32(state + 60, vector[1] << 6);
    w_u32(state + 64, vector[2] << 6);
    if (flags & 0x100)
    {
        source = r_u32(state + 52);
        sub_800313a8(source + 16, state + 180, 0, angles);
        source = r_u32(state + 52);
        value = (uint32)(sint32)(sint16)angles[0];
        other = r_s16(source + 16);
        target[0] = value - other;
        value = r_s16(source + 18);
        result = r_s16(source + 20);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        sub_800313a8(state + 20, state + 180, 0, angles);
        value = r_s16(state + 20);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = r_s16(state + 22);
        result = r_s16(state + 24);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    w_u16(state + 72, target[0]);
    w_u16(state + 74, target[1]);
    w_u16(state + 76, target[2]);
    return result;
}

uint32 sub_80078234(uint32 state, uint32 selected)
{
    MATRIX first = {0}, second = {0};
    uint32 a, b, c, d, e, packed, depth, column0[3], column1[3], vector[3];
    uint16 angles[3];
    uint32 game, flags, index, origin, target[3], steps[3], value, other, difference, axis, field, source, result;
    FUNCTION_MARKER(0x80078234, "ACE3.BIN");
    index = selected;
    flags = r_u32(state);
    origin = state + 4;
    if (flags & 0x100)
        origin = r_u32(state + 52);
    sub_800779c8(origin, index, 0, angles);
    target[0] = (uint32)(sint32)(sint16)angles[0];
    game = r_u32(0x800BCE60);
    target[1] = (uint32)(sint32)(sint16)angles[1];
    flags = r_u32(game + 312);
    target[2] = (uint32)(sint32)(sint16)angles[2];
    if (flags & 16)
        target[0] -= r_u32(game + 332);
    if (flags & 32)
        target[1] += r_u32(game + 336);
    flags = r_u32(game + 296);
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) < -0x2000)
        w_u16(state + 182, other + 0x4000);
    value = r_s16(state + 182);
    other = r_u16(state + 182);
    if ((sint32)(value - target[1]) >= 0x2001)
        w_u16(state + 182, other - 0x4000);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300) & 0x100000;
    if (flags == 0)
        flags = r_u32(game + 308) & 0x100000;
    if (flags)
    {
        w_u16(state + 180, target[0]);
        w_u16(state + 182, target[1]);
        w_u16(state + 184, target[2]);
        sub_8007786c(state);
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = r_s16(state + 180 + axis * 2);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = (uint32)((sint32)(difference + 2) >> 2);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)value < (sint32)target[axis])
                w_u16(field, other + steps[axis]);
            value = r_s16(field);
            other = r_u16(field);
            if ((sint32)target[axis] < (sint32)value)
                w_u16(field, other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = state + 180 + axis * 2;
            value = r_s16(field);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                w_u16(field, target[axis]);
        }
    }
    source = r_u32(state + 52);
    rotation_build(source + 16, 0, &first, 0);
    rotation_build(state + 180, 0, &second, 0);
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    packed = xport_load_le16((uint8 *)&second) | (xport_load_le32((uint8 *)&second + 4) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 12);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 2) | (xport_load_le32((uint8 *)&second + 8) << 16);
    depth = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&second + 14);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 4) | (xport_load_le32((uint8 *)&second + 8) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 16);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    xport_store_le32((uint8 *)&first, (column0[0] & 0xFFFF) | (column1[0] << 16));
    xport_store_le32((uint8 *)&first + 12, (column0[2] & 0xFFFF) | (column1[2] << 16));
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    xport_store_le32((uint8 *)&first + 4, (a & 0xFFFF) | (column0[1] << 16));
    xport_store_le32((uint8 *)&first + 8, (column1[1] & 0xFFFF) | (b << 16));
    xport_store_le32((uint8 *)&first + 16, xport_gte_read_data(11));
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(state + 44);
    xport_gte_write_data(0, a);
    b = r_u32(state + 48);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    vector[0] = xport_gte_read_data(25);
    vector[1] = xport_gte_read_data(26);
    vector[2] = xport_gte_read_data(27);
    w_u32(state + 56, vector[0] << 6);
    flags = r_u32(state);
    w_u32(state + 60, vector[1] << 6);
    w_u32(state + 64, vector[2] << 6);
    if (flags & 0x100)
    {
        source = r_u32(state + 52);
        sub_800313a8(source + 16, state + 180, 0, angles);
        source = r_u32(state + 52);
        value = (uint32)(sint32)(sint16)angles[0];
        other = r_s16(source + 16);
        target[0] = value - other;
        value = r_s16(source + 18);
        result = r_s16(source + 20);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        sub_800313a8(state + 20, state + 180, 0, angles);
        value = r_s16(state + 20);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = r_s16(state + 22);
        result = r_s16(state + 24);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    w_u16(state + 72, target[0]);
    w_u16(state + 74, target[1]);
    w_u16(state + 76, target[2]);
    return result;
}

uint32 sub_80067380(uint32 output, uint8 *native_output)
{
    uint32 scene, camera, excluded, selected, index, table, object, value, kind, offset;
    FUNCTION_MARKER(0x80067380, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    excluded = r_u32(camera + 196) - 32;
    selected = r_u32(camera + 200);
    offset = 0;
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE4C);
        object = r_u32(table + index * 4);
        if ((r_u32(object) & 0x14000) != 0x14000 || excluded == index)
            continue;
        value = r_u8(object + 5);
        kind = value & 2 ? 8 : 16;
        if (native_output)
            native_output[offset + 4] = (uint8)kind;
        else
            w_u8(output + offset + 4, kind);
        value = r_u8(object + 4);
        if (native_output)
            native_output[offset] = (uint8)value;
        else
            w_u8(output + offset, value);
        value = r_u8(object + 5);
        if (native_output)
            native_output[offset + 1] = (uint8)value;
        else
            w_u8(output + offset + 1, value);
        value = r_u16(object + 6);
        if (native_output)
        {
            xport_store_le32(native_output + offset + 8, object + 8);
            xport_store_le16(native_output + offset + 2, value);
        }
        else
        {
            w_u32(output + offset + 8, object + 8);
            w_u16(output + offset + 2, value);
        }
        value = r_u32(object);
        if (native_output)
        {
            native_output[offset + 6] = 0;
            native_output[offset + 7] = 0;
            xport_store_le32(native_output + offset + 12, value);
        }
        else
        {
            w_u8(output + offset + 6, 0);
            w_u8(output + offset + 7, 0);
            w_u32(output + offset + 12, value);
        }
        value = r_s16(object + 72);
        kind = value == 43 ? 3 : value == 35 ? 4 : value == 36 ? 5 : 0;
        if (kind)
        {
            if (native_output)
                native_output[offset + 6] = (uint8)kind;
            else
                w_u8(output + offset + 6, kind);
        }
        if (native_output)
            native_output[offset + 5] = selected == index;
        else
            w_u8(output + offset + 5, selected == index);
        offset += 16;
    }
    if (native_output)
        native_output[offset] = 255;
    else
        w_u8(output + offset, 255);
    return native_output ? offset : output + offset;
}

uint32 sub_80067144(uint32 output, uint8 *native_output)
{
    uint32 scene, camera, excluded, selected, index, table, object, value, kind, offset, root, mask;
    FUNCTION_MARKER(0x80067144, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    excluded = r_u32(camera + 196);
    selected = r_u32(camera + 200);
    offset = 0;
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if ((r_u32(object) & 0x14000) != 0x14000 || excluded == index)
            continue;
        value = r_u8(object + 5);
        kind = 1;
        if (value & 1)
        {
            kind = 4;
            if (selected == index)
            {
                scene = r_u32(0x800BCE54);
                camera = r_u32(scene + 40);
                if (r_u32(camera + 80) & 6)
                    kind = 2;
            }
        }
        if (native_output)
            native_output[offset + 4] = (uint8)kind;
        else
            w_u8(output + offset + 4, kind);
        value = r_u8(object + 4);
        if (native_output)
            native_output[offset] = (uint8)value;
        else
            w_u8(output + offset, value);
        value = r_u8(object + 5);
        if (native_output)
            native_output[offset + 1] = (uint8)value;
        else
            w_u8(output + offset + 1, value);
        value = r_u16(object + 6);
        if (native_output)
        {
            xport_store_le32(native_output + offset + 8, object + 8);
            xport_store_le16(native_output + offset + 2, value);
        }
        else
        {
            w_u32(output + offset + 8, object + 8);
            w_u16(output + offset + 2, value);
        }
        value = r_u32(object);
        if (native_output)
        {
            native_output[offset + 6] = 0;
            native_output[offset + 7] = 0;
            xport_store_le32(native_output + offset + 12, value);
        }
        else
        {
            w_u8(output + offset + 6, 0);
            w_u8(output + offset + 7, 0);
            w_u32(output + offset + 12, value);
        }
        value = r_s16(object + 72);
        kind = value == 43 ? 3 : value == 35 ? 4 : value == 36 ? 5 : 0;
        if (kind)
        {
            if (native_output)
                native_output[offset + 6] = (uint8)kind;
            else
                w_u8(output + offset + 6, kind);
        }
        kind = value == 25 ? 2 : 1;
        if (native_output)
        {
            native_output[offset + 7] = (uint8)kind;
            native_output[offset + 5] = selected == index;
        }
        else
        {
            w_u8(output + offset + 7, kind);
            w_u8(output + offset + 5, selected == index);
        }
        offset += 16;
    }
    root = r_u32(0x800BD080);
    mask = r_u32(root + 0x1548);
    if (mask)
    {
        for (index = 0; index < 32; ++index)
        {
            if ((mask & ((uint32)1 << index)) == 0)
                continue;
            root = r_u32(0x800BD080);
            if (native_output)
                native_output[offset + 4] = 1;
            else
                w_u8(output + offset + 4, 1);
            object = root + 200 + index * 164;
            value = r_u8(object + 52);
            if (value == 0)
                continue;
            if (native_output)
            {
                native_output[offset] = (uint8)(value + 23);
                native_output[offset + 1] = 0;
                xport_store_le32(native_output + offset + 8, object + 28);
                xport_store_le32(native_output + offset + 12, 0);
                native_output[offset + 5] = 0;
                native_output[offset + 6] = 0;
                native_output[offset + 7] = 0;
            }
            else
            {
                w_u8(output + offset, value + 23);
                w_u8(output + offset + 1, 0);
                w_u32(output + offset + 8, object + 28);
                w_u32(output + offset + 12, 0);
                w_u8(output + offset + 5, 0);
                w_u8(output + offset + 6, 0);
                w_u8(output + offset + 7, 0);
            }
            value = r_u8(object + 53);
            if (value & 16)
            {
                if (native_output)
                    native_output[offset + 6] = 1;
                else
                    w_u8(output + offset + 6, 1);
            }
            value = r_u8(object + 53);
            if (value & 32)
            {
                if (native_output)
                    native_output[offset + 6] = 2;
                else
                    w_u8(output + offset + 6, 2);
            }
            offset += 16;
        }
    }
    if (native_output)
        native_output[offset] = 255;
    else
        w_u8(output + offset, 255);
    return native_output ? offset : output + offset;
}

uint32 sub_8005f174(uint32 records, uint32 count, const uint8 *native_records)
{
    uint32 index, best, current, candidate;
    FUNCTION_MARKER(0x8005F174, "ACE3.BIN");
    best = 0;
    for (index = 1; (sint32)index < (sint32)count; ++index)
    {
        current = native_records ? xport_load_le32(native_records + best * 16) : r_u32(records + best * 16);
        candidate = native_records ? xport_load_le32(native_records + index * 16) : r_u32(records + index * 16);
        if ((sint32)candidate < (sint32)current)
            best = index;
    }
    return best;
}

uint32 sub_8005f124(uint32 records, uint32 count, const uint8 *native_records)
{
    uint32 index, best, current, candidate;
    FUNCTION_MARKER(0x8005F124, "ACE3.BIN");
    best = 0;
    for (index = 1; (sint32)index < (sint32)count; ++index)
    {
        current = native_records ? xport_load_le32(native_records + best * 16) : r_u32(records + best * 16);
        candidate = native_records ? xport_load_le32(native_records + index * 16) : r_u32(records + index * 16);
        if ((sint32)current < (sint32)candidate)
            best = index;
    }
    return best;
}

uint32 sub_800a70e8(void)
{
    FUNCTION_MARKER(0x800A70E8, "ACE3.BIN");
    return xport_gte_read_control(26);
}

uint32 sub_8005f734(uint32 input, uint32 output, uint32 unused, uint32 divisor, const uint16 *native_input, uint32 *native_output)
{
    uint32 a, b, numerator, vector[3];
    FUNCTION_MARKER(0x8005F734, "ACE3.BIN");
    a = native_input ? (uint32)(sint32)(sint16)native_input[0] : (uint32)r_s16(input);
    b = native_input ? (uint32)(sint32)(sint16)native_input[2] : (uint32)r_s16(input + 4);
    numerator = a * b;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    vector[0] = (uint32)((sint32)numerator / (sint32)divisor);
    a = native_input ? (uint32)(sint32)(sint16)native_input[1] : (uint32)r_s16(input + 2);
    b = native_input ? (uint32)(sint32)(sint16)native_input[2] : (uint32)r_s16(input + 4);
    numerator = a * b;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    vector[1] = (uint32)((sint32)numerator / (sint32)divisor);
    vector[2] = native_input ? (uint32)(sint32)(sint16)native_input[2] : (uint32)r_s16(input + 4);
    rotate_native_vector_a6b18(vector);
    if (native_output)
    {
        native_output[0] = vector[0];
        native_output[1] = vector[1];
        native_output[2] = vector[2];
    }
    else
    {
        w_u32(output, vector[0]);
        w_u32(output + 4, vector[1]);
        w_u32(output + 8, vector[2]);
    }
    return output;
}

uint32 sub_8005f7fc(uint32 input, uint32 output, const uint32 *native_input, uint16 *native_output)
{
    uint32 scale, x, y, vector[3], numerator, value;
    FUNCTION_MARKER(0x8005F7FC, "ACE3.BIN");
    scale = sub_800a70e8();
    x = xport_gte_read_control(24);
    y = xport_gte_read_control(25);
    x = (uint32)((sint32)x >> 16);
    y = (uint32)((sint32)y >> 16);
    vector[0] = native_input ? native_input[0] : r_u32(input);
    vector[1] = native_input ? native_input[1] : r_u32(input + 4);
    vector[2] = native_input ? native_input[2] : r_u32(input + 8);
    rotate_native_vector_a6b18(vector);
    if (vector[2] == 0)
        vector[2] = 1;
    numerator = vector[0] * scale;
    if (vector[2] == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (x & 0xFFFF) + (uint32)((sint32)numerator / (sint32)vector[2]);
    if (native_output)
        native_output[0] = (uint16)value;
    else
        w_u16(output, value);
    numerator = vector[1] * scale;
    if (vector[2] == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (y & 0xFFFF) + (uint32)((sint32)numerator / (sint32)vector[2]);
    if (native_output)
        native_output[1] = (uint16)value;
    else
        w_u16(output + 2, value);
    return value;
}

uint32 sub_8005f510(uint32 origin, uint32 vector, uint32 plane, uint32 output, const uint32 *native_origin, const uint32 *native_vector, uint32 *native_output)
{
    uint32 height, divisor, delta, value, numerator, other;
    FUNCTION_MARKER(0x8005F510, "ACE3.BIN");
    height = native_origin ? native_origin[1] : r_u32(origin + 4);
    divisor = native_vector ? native_vector[1] : r_u32(vector + 4);
    delta = plane - height;
    if ((sint32)divisor > 0)
    {
        if ((sint32)plane < (sint32)height)
            return 0;
    }
    else if ((sint32)divisor < 0)
    {
        if ((sint32)height < (sint32)plane)
            return 0;
    }
    else if (plane != height)
        return 0;
    if (divisor)
    {
        value = native_vector ? native_vector[0] : r_u32(vector);
        numerator = value * delta;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = (uint32)((sint32)numerator / (sint32)divisor);
        if (native_output)
            native_output[0] = value;
        else
            w_u32(output, value);
        value = native_vector ? native_vector[2] : r_u32(vector + 8);
        numerator = value * delta;
        divisor = native_vector ? native_vector[1] : r_u32(vector + 4);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = (uint32)((sint32)numerator / (sint32)divisor);
        if (native_output)
            native_output[1] = delta;
        else
            w_u32(output + 4, delta);
    }
    else
    {
        value = native_vector ? native_vector[0] : r_u32(vector);
        if (native_output)
            native_output[0] = value;
        else
            w_u32(output, value);
        value = native_vector ? native_vector[2] : r_u32(vector + 8);
        if (native_output)
            native_output[1] = delta;
        else
            w_u32(output + 4, delta);
    }
    if (native_output)
        native_output[2] = value;
    else
        w_u32(output + 8, value);
    value = native_vector ? native_vector[0] : r_u32(vector);
    other = native_output ? native_output[0] : r_u32(output);
    if ((sint32)value < 0 ? (sint32)other < (sint32)value : (sint32)value < (sint32)other)
    {
        if (native_output)
            native_output[0] = value;
        else
            w_u32(output, value);
        value = native_vector ? native_vector[1] : r_u32(vector + 4);
        if (native_output)
            native_output[1] = value;
        else
            w_u32(output + 4, value);
    }
    value = native_vector ? native_vector[2] : r_u32(vector + 8);
    other = native_output ? native_output[2] : r_u32(output + 8);
    if ((sint32)value < 0 ? (sint32)other < (sint32)value : (sint32)value < (sint32)other)
    {
        if (native_output)
            native_output[2] = value;
        else
            w_u32(output + 8, value);
        value = native_vector ? native_vector[1] : r_u32(vector + 4);
        if (native_output)
            native_output[1] = value;
        else
            w_u32(output + 4, value);
    }
    value = native_output ? native_output[1] : r_u32(output + 4);
    other = native_vector ? native_vector[1] : r_u32(vector + 4);
    return value != other;
}

uint32 sub_80061534(uint32 state, uint8 *native_state)
{
    uint32 value, base, x, z, count, index, cursor, result;
    sint32 first, second;
    FUNCTION_MARKER(0x80061534, "ACE3.BIN");
    value = native_state ? xport_load_le32(native_state) : r_u32(state);
    base = r_u32(0x800BD068);
    x = value + 0x8000 + base;
    value = native_state ? xport_load_le32(native_state + 8) : r_u32(state + 8);
    base = r_u32(0x800BD06C);
    z = value + 0x8000 + base;
    value = (uint32)((sint32)z >> 11) + 128;
    if (native_state)
        xport_store_le32(native_state + 60, value);
    else
        w_u32(state + 60, value);
    count = native_state ? xport_load_le32(native_state + 24) : r_u32(state + 24);
    value = (uint32)((sint32)x >> 11) + 128;
    if (native_state)
        xport_store_le32(native_state + 56, value);
    else
        w_u32(state + 56, value);
    x &= 0x7FF;
    z &= 0x7FF;
    if (native_state)
        xport_store_le32(native_state + 64, x - 0x400);
    else
        w_u32(state + 64, x - 0x400);
    result = z - 0x400;
    if (native_state)
        xport_store_le32(native_state + 68, result);
    else
        w_u32(state + 68, result);
    if ((sint32)count <= 0)
        return result;
    index = 0;
    cursor = 0;
    do
    {
        first = native_state ? (sint16)xport_load_le16(native_state + cursor + 28) : r_s16(state + cursor + 28);
        second = native_state ? (sint16)xport_load_le16(native_state + cursor + 30) : r_s16(state + cursor + 30);
        first += (sint32)x;
        second += (sint32)z;
        if (first < -0x8000)
            first = -0x8000;
        else if (first >= 0x7800)
            first = 0x77FF;
        if (second < -0x8000)
            second = -0x8000;
        else if (second >= 0x7800)
            second = 0x77FF;
        if (native_state)
        {
            xport_store_le16(native_state + cursor + 28, first);
            xport_store_le16(native_state + cursor + 30, second);
        }
        else
        {
            w_u16(state + cursor + 28, first);
            w_u16(state + cursor + 30, second);
        }
        count = native_state ? xport_load_le32(native_state + 24) : r_u32(state + 24);
        ++index;
        result = (sint32)index < (sint32)count;
        cursor += 4;
    } while (result);
    return result;
}

uint32 sub_8005e4cc(uint32 tiles, uint32 state, uint32 packet, const uint8 *native_state)
{
    uint32 x_offset, z_offset, row_coordinate, height, x_base, z_base, count, row, column;
    uint32 previous, current, first, column_coordinate, table_x, table_z, value, bit, flags, depth;
    FUNCTION_MARKER(0x8005E4CC, "ACE3.BIN");
    x_offset = native_state ? xport_load_le32(native_state + 64) : r_u32(state + 64);
    z_offset = native_state ? xport_load_le32(native_state + 68) : r_u32(state + 68);
    row_coordinate = native_state ? xport_load_le32(native_state + 76) : r_u32(state + 76);
    height = native_state ? xport_load_le32(native_state + 4) : r_u32(state + 4);
    x_base = native_state ? xport_load_le32(native_state + 56) : r_u32(state + 56);
    x_offset -= 0x400;
    count = native_state ? xport_load_le32(native_state + 92) : r_u32(state + 92);
    z_base = native_state ? xport_load_le32(native_state + 60) : r_u32(state + 60);
    z_offset -= 0x400;
    previous = 0;
    current = 0;
    row = 0;
    if ((sint32)count > 0)
        do
        {
            depth = (row_coordinate << 11) - z_offset;
            first = 0xFFFFFFFF;
            count = native_state ? xport_load_le32(native_state + 88) : r_u32(state + 88);
            column_coordinate = native_state ? xport_load_le32(native_state + 72) : r_u32(state + 72);
            column = 0;
            if ((sint32)count > 0)
                do
                {
                    value = native_state ? xport_load_le32(native_state + 96 + row * 4) : r_u32(state + 96 + row * 4);
                    bit = (uint32)1 << (column & 31);
                    if (value & bit)
                    {
                        table_x = x_base + column_coordinate;
                        table_z = z_base + row_coordinate;
                        if ((sint32)(table_x << 16) < 0)
                            table_x &= 7;
                        if ((sint16)(z_base + row_coordinate) < 0)
                            table_z &= 7;
                        if ((sint16)table_x >= 256)
                            table_x = (table_x & 7) | 0xF8;
                        if ((sint16)table_z >= 256)
                            table_z = (table_z & 7) | 0xF8;
                        value = r_s16(tiles + (uint32)((sint32)(table_z << 16) >> 7) + (uint32)((sint32)(table_x << 16) >> 15));
                        if (value & 0x1FF)
                        {
                            flags = value | (column << 16);
                            current |= bit;
                            if ((sint32)first < 0)
                            {
                                first = column;
                                flags |= 0x400000;
                            }
                            else
                                flags |= ((previous >> ((column - 1) & 31)) & 3) == 3 ? 0xE000000 : 0x8000000;
                            w_u16(packet, (column_coordinate << 11) - x_offset);
                            w_u16(packet + 2, 0 - height);
                            w_u16(packet + 4, depth);
                            w_u32(packet + 8, flags);
                            packet += 12;
                        }
                    }
                    count = native_state ? xport_load_le32(native_state + 88) : r_u32(state + 88);
                    ++column;
                    ++column_coordinate;
                } while ((sint32)column < (sint32)count);
            previous = current;
            current = 0;
            ++row_coordinate;
            count = native_state ? xport_load_le32(native_state + 92) : r_u32(state + 92);
            ++row;
        } while ((sint32)row < (sint32)count);
    w_u32(packet + 8, 0x200000);
    return packet + 12;
}

uint32 sub_800619e0(uint32 state)
{
    uint32 model, packet, ot, count, record, index, indices, p0, p1, p2, p3, xy0, xy3;
    uint32 depth0, depth1, depth2, depth3, depth, area, target, value, a, b, c, result;
    FUNCTION_MARKER(0x800619E0, "ACE3.BIN");
    model = r_u32(state + 8);
    packet = r_u32(state + 4);
    ot = r_u32(state);
    count = r_u32(model);
    record = model + 4;
    result = model;
    for (index = 0; (sint32)index < (sint32)count; ++index, record += 36)
    {
        indices = r_u32(record + 16);
        p0 = state + 32 + ((indices & 255) << 4);
        p1 = state + 32 + (((indices >> 8) & 255) << 4);
        p2 = state + 32 + (((indices >> 16) & 255) << 4);
        p3 = state + 32 + ((indices >> 24) << 4);
        xy0 = r_u32(p0);
        value = r_u32(p1);
        xport_gte_write_data(13, value);
        value = r_u32(p2);
        xport_gte_write_data(14, value);
        xport_gte_write_data(12, xy0);
        xy3 = r_u32(p3);
        xport_gte_execute(0x1400006);
        depth1 = r_u32(p1 + 4);
        depth0 = r_u32(p0 + 4);
        depth = (sint32)depth1 < (sint32)depth0 ? depth0 : depth1;
        depth3 = r_u32(p3 + 4);
        depth2 = r_u32(p2 + 4);
        if ((sint32)depth3 < (sint32)depth2)
            depth3 = depth2;
        if ((sint32)depth3 < (sint32)depth)
            depth3 = depth;
        depth = depth3;
        area = xport_gte_read_data(24);
        xport_gte_write_data(12, xy3);
        if ((sint32)area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = xport_gte_read_data(24);
            if ((sint32)area >= 0)
            {
                result = index + 1 < count;
                continue;
            }
        }
        if (depth - 121 >= 0x777F)
        {
            result = index + 1 < count;
            continue;
        }
        depth = (uint32)((sint32)depth >> 3);
        if ((sint32)depth >= 0x38F)
        {
            value = r_u32(p0 + 8);
            xport_gte_write_data(8, value);
            value = r_u32(record + 32);
            xport_gte_write_data(6, value);
            target = ot + (depth << 2);
            w_u32(packet + 8, xy0);
            value = r_u32(target);
            w_u32(packet, value | 0x09000000);
            w_u32(target, packet & 0xFFFFFF);
            xport_gte_execute(0x780010);
            a = r_u32(record + 28);
            b = r_u32(record + 20);
            c = r_u32(record + 24);
            w_u32(packet + 28, a);
            w_u32(packet + 12, b);
            w_u32(packet + 20, c);
            w_u32(packet + 36, a >> 16);
            value = xport_gte_read_data(22);
            w_u32(packet + 4, value);
            value = xport_gte_read_data(13);
            w_u32(packet + 16, value);
            value = xport_gte_read_data(14);
            w_u32(packet + 24, value);
            value = xport_gte_read_data(12);
            w_u32(packet + 32, value);
            packet += 40;
        }
        else
        {
            w_u32(state + 0x264, depth);
            value = r_u32(record);
            w_u32(state + 0x234, value);
            a = r_u32(record + 8);
            b = r_u32(record + 12);
            w_u32(state + 0x23C, a);
            w_u32(state + 0x240, b);
            a = r_u32(record + 20);
            b = r_u32(record + 24);
            c = r_u32(record + 28);
            w_u32(state + 0x250, c);
            w_u32(state + 0x248, a);
            w_u32(state + 0x24C, b);
            value = (r_u32(record + 32) & 0xFFFFFF) | 0x3C000000;
            w_u32(state + 0x254, value);
            w_u32(state + 0x258, value);
            w_u32(state + 0x25C, value);
            w_u32(state + 0x260, value);
            w_u32(state + 4, packet);
            packet = sub_8006258c(state + 0x234, state);
        }
        result = (sint32)(index + 1) < (sint32)count;
    }
    w_u32(state + 4, packet);
    return result;
}

uint32 sub_80061c3c(uint32 state)
{
    uint32 model, packet, ot, count, record, index, indices, p0, p1, p2, p3, xy0, xy3;
    uint32 depth0, depth1, depth2, depth3, depth, area, target, value, a, b, c, result;
    FUNCTION_MARKER(0x80061C3C, "ACE3.BIN");
    model = r_u32(state + 8);
    packet = r_u32(state + 4);
    ot = r_u32(state);
    count = r_u32(model);
    record = model + 4;
    result = model;
    for (index = 0; (sint32)index < (sint32)count; ++index, record += 48)
    {
        indices = r_u32(record + 16);
        p0 = state + 32 + ((indices & 255) << 4);
        p1 = state + 32 + (((indices >> 8) & 255) << 4);
        p2 = state + 32 + (((indices >> 16) & 255) << 4);
        p3 = state + 32 + ((indices >> 24) << 4);
        xy0 = r_u32(p0);
        value = r_u32(p1);
        xport_gte_write_data(13, value);
        value = r_u32(p2);
        xport_gte_write_data(14, value);
        xport_gte_write_data(12, xy0);
        xy3 = r_u32(p3);
        xport_gte_execute(0x1400006);
        depth1 = r_u32(p1 + 4);
        depth0 = r_u32(p0 + 4);
        depth = (sint32)depth1 < (sint32)depth0 ? depth0 : depth1;
        depth3 = r_u32(p3 + 4);
        depth2 = r_u32(p2 + 4);
        if ((sint32)depth3 < (sint32)depth2)
            depth3 = depth2;
        if ((sint32)depth3 < (sint32)depth)
            depth3 = depth;
        depth = depth3;
        area = xport_gte_read_data(24);
        xport_gte_write_data(12, xy3);
        if ((sint32)area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = xport_gte_read_data(24);
            if ((sint32)area >= 0)
            {
                result = index + 1 < count;
                continue;
            }
        }
        if (depth - 121 >= 0x777F)
        {
            result = index + 1 < count;
            continue;
        }
        depth = (uint32)((sint32)depth >> 3);
        if ((sint32)depth >= 0x38F)
        {
            value = r_u32(p0 + 8);
            xport_gte_write_data(8, value);
            value = r_u32(record + 32);
            xport_gte_write_data(6, value);
            value = r_u32(record + 36);
            xport_gte_write_data(20, value);
            value = r_u32(record + 40);
            xport_gte_write_data(21, value);
            value = r_u32(record + 44);
            xport_gte_write_data(22, value);
            w_u32(packet + 8, xy0);
            xport_gte_execute(0xF8002A);
            target = ot + (depth << 2);
            value = r_u32(target);
            w_u32(packet, value | 0x0C000000);
            w_u32(target, packet & 0xFFFFFF);
            a = r_u32(record + 28);
            b = r_u32(record + 20);
            c = r_u32(record + 24);
            value = xport_gte_read_data(20);
            w_u32(packet + 16, value);
            value = xport_gte_read_data(21);
            w_u32(packet + 28, value);
            value = xport_gte_read_data(22);
            w_u32(packet + 40, value);
            w_u32(packet + 12, b);
            w_u32(packet + 24, c);
            xport_gte_execute(0x780010);
            w_u32(packet + 36, a);
            w_u32(packet + 48, a >> 16);
            value = xport_gte_read_data(22);
            w_u32(packet + 4, value);
            value = xport_gte_read_data(13);
            w_u32(packet + 20, value);
            value = xport_gte_read_data(14);
            w_u32(packet + 32, value);
            value = xport_gte_read_data(12);
            w_u32(packet + 44, value);
            packet += 52;
        }
        else
        {
            w_u32(state + 0x264, depth);
            w_u32(state + 4, packet);
            packet = sub_8006258c(record, state);
        }
        result = (sint32)(index + 1) < (sint32)count;
    }
    w_u32(state + 4, packet);
    return result;
}

uint32 sub_8006258c(uint32 polygon, uint32 state)
{
    uint32 value, output, indices, table, a, b, c;
    FUNCTION_MARKER(0x8006258C, "ACE3.BIN");
    value = r_u16(polygon + 26);
    w_u16(state + 0x268, value);
    value = r_u16(polygon + 22);
    output = state + 0x270;
    w_u16(state + 0x26A, value);
    value = r_u32(polygon + 32);
    xport_gte_write_data(6, value);
    value = r_u32(polygon + 36);
    xport_gte_write_data(20, value);
    value = r_u32(polygon + 40);
    xport_gte_write_data(21, value);
    value = r_u32(polygon + 44);
    xport_gte_write_data(22, value);
    indices = r_u32(polygon + 12);
    table = r_u32(state + 0x230);
    a = r_u32(table + (indices & 255));
    b = r_u16(table + ((indices >> 8) & 255));
    w_u16(output + 28, b);
    w_u32(output + 24, a);
    a = r_u32(table + ((indices >> 16) & 255));
    b = r_u16(table + (indices >> 24));
    indices = r_u32(polygon + 8);
    xport_gte_write_data(0, a);
    xport_gte_write_data(1, b);
    w_u32(output + 36, a);
    w_u16(output + 40, b);
    xport_gte_execute(0x180001);
    a = r_u32(table + (indices & 255));
    b = r_u16(table + ((indices >> 8) & 255));
    w_u32(output, a);
    w_u16(output + 4, b);
    xport_gte_execute(0xF8002A);
    a = r_u32(table + ((indices >> 16) & 255));
    b = r_u16(table + (indices >> 24));
    value = xport_gte_read_data(20);
    w_u32(output + 20, value);
    value = xport_gte_read_data(21);
    w_u32(output + 32, value);
    value = xport_gte_read_data(22);
    w_u32(output + 44, value);
    w_u32(output + 12, a);
    w_u16(output + 16, b);
    xport_gte_execute(0x780010);
    a = r_u32(polygon + 28);
    b = r_u16(polygon + 20);
    c = r_u16(polygon + 24);
    w_u16(output + 30, a);
    w_u16(output + 6, b);
    w_u16(output + 18, c);
    w_u16(output + 42, a >> 16);
    value = xport_gte_read_data(22);
    w_u32(output + 8, value);
    value = xport_gte_read_data(14);
    w_u32(state + 0x2AC, value);
    value = xport_gte_read_data(19);
    w_u16(state + 0x2B6, value);
    sub_80061fa8(output, state, 4, 0);
    return r_u32(state + 4);
}

uint32 sub_80061e50(uint32 output, uint32 first, uint32 second)
{
    uint32 a, b, value, offset;
    FUNCTION_MARKER(0x80061E50, "ACE3.BIN");
    for (offset = 0; offset < 6; offset += 2)
    {
        a = r_s16(first + offset);
        b = r_s16(second + offset);
        value = (uint32)((sint32)(a + b) >> 1);
        w_u16(output + offset, value);
    }
    a = r_u32(first + 8);
    b = r_u32(second + 8);
    value = (uint32)((sint32)((a & 0xFEFEFEFF) + (b & 0xFEFEFEFF)) >> 1);
    w_u32(output + 8, value);
    a = r_s16(first + 6);
    b = r_s16(second + 6);
    value = (uint32)((sint32)((a & 0xFEFEFEFF) + (b & 0xFEFEFEFF)) >> 1);
    w_u16(output + 6, value);
    return value;
}

uint32 sub_80061ed4(uint32 first, uint32 second, uint32 third, uint32 state)
{
    uint32 value, a, b, packet, depth, ot, result;
    FUNCTION_MARKER(0x80061ED4, "ACE3.BIN");
    value = r_u32(first);
    xport_gte_write_data(0, value);
    value = r_u32(first + 4);
    xport_gte_write_data(1, value);
    value = r_u32(second);
    xport_gte_write_data(2, value);
    value = r_u32(second + 4);
    xport_gte_write_data(3, value);
    value = r_u32(third);
    xport_gte_write_data(4, value);
    value = r_u32(third + 4);
    xport_gte_write_data(5, value);
    a = r_u32(first + 8);
    packet = r_u32(state + 4);
    xport_gte_execute(0x280030);
    b = r_u32(second + 8);
    w_u32(packet + 4, a);
    w_u32(packet + 16, b);
    a = r_u16(third + 6);
    b = r_u32(third + 8);
    w_u16(packet + 36, a);
    w_u32(packet + 28, b);
    a = r_u16(first + 6);
    b = r_u16(second + 6);
    w_u16(packet + 12, a);
    w_u16(packet + 24, b);
    a = r_s16(state + 0x26A);
    b = r_s16(state + 0x268);
    w_u8(packet + 7, 0x34);
    w_u16(packet + 14, a);
    w_u16(packet + 26, b);
    depth = r_u32(state + 0x264);
    result = xport_gte_read_control(31);
    depth <<= 2;
    if ((sint32)result < 0)
        return result;
    value = r_u32(state + 4);
    w_u32(state + 4, value + 40);
    ot = r_u32(state);
    value = r_u32(ot + depth);
    w_u32(packet, value | 0x09000000);
    ot = r_u32(state);
    result = packet & 0xFFFFFF;
    w_u32(ot + depth, result);
    value = xport_gte_read_data(12);
    w_u32(packet + 8, value);
    value = xport_gte_read_data(13);
    w_u32(packet + 20, value);
    value = xport_gte_read_data(14);
    w_u32(packet + 32, value);
    return result;
}

uint32 sub_80061fa8(uint32 vertices, uint32 state, uint32 count, uint32 mode)
{
    static const uint32 endpoint0[3] = {12, 24, 0}, endpoint1[3] = {36, 36, 24};
    static const uint32 copied[3] = {12, 36, 24}, screen[3] = {52, 60, 56}, depths[3] = {66, 70, 68};
    static const uint32 mode_delta[3] = {3, 0, 1}, bridge0[3] = {12, 36, 24}, bridge1[3] = {36, 24, 0};
    uint32 value, packet, depth, next, index, flags, inside, all_outside, a, b, c, d, area0, area1, sum, ot, level;
    uint32 colors[4], uv[4], xy[4], color_offsets[4], uv_offsets[4], xy_offsets[4], stage, offset;
    sint32 x, y;
    FUNCTION_MARKER(0x80061FA8, "ACE3.BIN");
    for (index = 0; index < 3; ++index)
    {
        value = r_u32(vertices + index * 12);
        xport_gte_write_data(index * 2, value);
        value = r_u32(vertices + index * 12 + 4);
        xport_gte_write_data(index * 2 + 1, value);
    }
    next = vertices + 72;
    xport_gte_execute(0x280030);
    packet = r_u32(state + 4);
    value = r_u16(state + 0x268);
    depth = r_u32(state + 0x264);
    w_u16(packet + 26, value);
    value = r_u16(state + 0x26A);
    w_u16(packet + 14, value);
    value = xport_gte_read_data(12);
    w_u32(vertices + 48, value);
    value = xport_gte_read_data(13);
    w_u32(vertices + 52, value);
    value = xport_gte_read_data(14);
    w_u32(vertices + 56, value);
    flags = 0;
    all_outside = 1;
    for (index = 0; index < 4; ++index)
    {
        x = r_s16(vertices + 48 + index * 4);
        inside = 0;
        if (x < -160)
            flags |= (uint32)1 << index;
        else if (x >= 161)
            flags |= (uint32)16 << index;
        else
            inside = 1;
        y = r_s16(vertices + 50 + index * 4);
        if (y < -120)
            flags |= (uint32)256 << index;
        else if (y >= 121)
            flags |= (uint32)4096 << index;
        else
            ++inside;
        if (inside >= 2)
        {
            all_outside = 0;
            break;
        }
    }
    if (all_outside && ((flags & 15) == 15 || ((flags >> 4) & 15) == 15 || ((flags >> 8) & 15) == 15 || ((flags >> 12) & 15) == 15))
        return 0;
    value = xport_gte_read_data(17);
    w_u16(vertices + 64, value);
    value = xport_gte_read_data(18);
    w_u16(vertices + 66, value);
    value = xport_gte_read_data(19);
    w_u16(vertices + 68, value);
    xport_gte_execute(0x1400006);
    a = r_s16(vertices + 66);
    b = r_s16(vertices + 64);
    c = r_u16(vertices + 66);
    d = r_u16(vertices + 64);
    if ((sint32)a < (sint32)b)
        c = d;
    a = r_s16(vertices + 70);
    b = r_s16(vertices + 68);
    d = r_u16(vertices + 68);
    if ((sint32)a < (sint32)b)
        a = d;
    if ((sint32)(a << 16) < (sint32)(c << 16))
        a = c;
    if (((sint32)(a << 16) >> 16) < 31)
        return 0;
    area0 = xport_gte_read_data(24);
    value = r_u32(vertices + 60);
    xport_gte_write_data(15, value);
    if ((sint32)area0 < 0)
        area0 = 0 - area0;
    xport_gte_execute(0x1400006);
    area1 = xport_gte_read_data(24);
    if ((sint32)area1 < 0)
        area1 = 0 - area1;
    sum = area0 + area1;
    if ((sint32)sum <= 0x13FFE && (sint32)count >= 4)
        count = 3;
    if ((sint32)sum < 0x703F && (sint32)count >= 3)
        count = 2;
    if ((sint32)sum < 0x307F && (sint32)count >= 2)
        count = 1;
    if ((sint32)sum < 0x14FF && (sint32)count > 0)
        count = 0;
    level = count - 1;
    if ((sint32)level < 0)
    {
        for (index = 0; index < 4; ++index)
        {
            color_offsets[index] = 8 + index * 12;
            uv_offsets[index] = 6 + index * 12;
            xy_offsets[index] = 48 + index * 4;
        }
        if (mode & 1)
        {
            color_offsets[0] = 20;
            color_offsets[1] = 44;
            color_offsets[2] = 8;
            color_offsets[3] = 32;
            uv_offsets[0] = 18;
            uv_offsets[1] = 42;
            uv_offsets[2] = 6;
            uv_offsets[3] = 30;
            xy_offsets[0] = 52;
            xy_offsets[1] = 60;
            xy_offsets[2] = 48;
            xy_offsets[3] = 56;
        }
        for (index = 0; index < 4; ++index)
            colors[index] = r_u32(vertices + color_offsets[index]);
        for (index = 0; index < 4; ++index)
            uv[index] = r_u16(vertices + uv_offsets[index]);
        for (index = 0; index < 4; ++index)
            xy[index] = r_u32(vertices + xy_offsets[index]);
        for (index = 0; index < 4; ++index)
            w_u32(packet + 4 + index * 12, colors[index]);
        for (index = 0; index < 4; ++index)
            w_u16(packet + 12 + index * 12, uv[index]);
        for (index = 0; index < 4; ++index)
            w_u32(packet + 8 + index * 12, xy[index]);
        ot = r_u32(state);
        depth <<= 2;
        value = r_u32(ot + depth);
        w_u32(packet, value | 0x0C000000);
        ot = r_u32(state);
        w_u32(ot + depth, packet & 0xFFFFFF);
        value = r_u32(state + 4);
        w_u32(state + 4, value + 52);
        return 1;
    }
    sub_80061e50(next, vertices + (mode & 1 ? 0 : 12), vertices + (mode & 1 ? 36 : 24));
    sub_80061e50(next + 12, vertices, vertices + 24);
    sub_80061e50(next + 24, vertices, vertices + 12);
    for (offset = 0; offset < 12; offset += 4)
    {
        value = r_u32(vertices + offset);
        w_u32(next + 36 + offset, value);
    }
    value = r_u32(vertices + 48);
    w_u32(next + 60, value);
    value = r_u16(vertices + 64);
    w_u16(next + 70, value);
    if (sub_80061fa8(next, state, level, mode + 2))
        sub_80061ed4(vertices, vertices + 12, next + 24, state);
    for (stage = 0; stage < 3; ++stage)
    {
        a = r_u32(next + 24);
        b = r_u32(next + 28);
        c = r_u32(next + 32);
        w_u32(next + 12, a);
        w_u32(next + 16, b);
        w_u32(next + 20, c);
        sub_80061e50(next + 24, vertices + endpoint0[stage], vertices + endpoint1[stage]);
        for (offset = 0; offset < 12; offset += 4)
        {
            value = r_u32(vertices + copied[stage] + offset);
            w_u32(next + 36 + offset, value);
        }
        value = r_u32(vertices + screen[stage]);
        w_u32(next + 60, value);
        value = r_u16(vertices + depths[stage]);
        w_u16(next + 70, value);
        if (sub_80061fa8(next, state, level, mode + mode_delta[stage]))
            sub_80061ed4(vertices + bridge0[stage], vertices + bridge1[stage], next + 24, state);
    }
    return 1;
}

uint32 sub_80061768(uint32 records, uint32 models, uint32 state, uint32 *native_previous, uint32 *native_current)
{
    uint32 flags, model, a, b, c, count, input, following, output, slot, saved, depth, table;
    uint32 words[5], next_words[5], value, index, base, reg;
    uint32 *native_swap;
    FUNCTION_MARKER(0x80061768, "ACE3.BIN");
    xport_gte_write_control(21, 0x830);
    xport_gte_write_control(22, 0x830);
    xport_gte_write_control(23, 0x830);
    for (;;)
    {
        flags = r_u32(records + 8);
        if (flags & 0x200000)
            return 0;
        model = r_u32(models + (flags & 0x1FF) * 4);
        a = r_u32(state + 20);
        b = r_u32(state + 24);
        c = r_u32(state + 28);
        xport_gte_write_control(5, a);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        value = r_u32(records);
        xport_gte_write_data(0, value);
        value = r_u32(records + 4);
        xport_gte_write_data(1, value);
        if (flags & 0x400000)
        {
            if (native_previous != 0)
            {
                native_swap = native_previous;
                native_previous = native_current;
                native_current = native_swap;
            }
            else
            {
                a = r_u32(state + 12);
                b = r_u32(state + 16);
                w_u32(state + 16, a);
                w_u32(state + 12, b);
            }
        }
        xport_gte_execute(0x480012);
        output = state - 16;
        slot = (flags >> 16) & 31;
        base = r_u32(model + 4);
        saved = r_u32(state + 80);
        input = base + 4;
        count = r_u32(base);
        a = xport_gte_read_data(25);
        b = xport_gte_read_data(26);
        xport_gte_write_control(5, a);
        depth = xport_gte_read_data(27);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, depth);
        following = base + 20;
        if ((sint32)depth >= 0x7001)
        {
            value = r_u32(input + 20);
            xport_gte_write_data(0, value);
            value = r_u32(input + 32);
            xport_gte_write_data(1, value);
            xport_gte_execute(0x180001);
            value = xport_gte_read_data(14);
            w_u32(state + 80, value);
            if (native_current != 0)
            {
                value = r_u32(state + 80);
                native_current[slot] = value;
            }
            else
            {
                table = r_u32(state + 16);
                value = r_u32(state + 80);
                w_u32(table + slot * 4, value);
            }
        }
        else
        {
            words[0] = r_u32(base + 4);
            words[1] = r_u32(input + 4);
            words[2] = r_u32(input + 8);
            words[3] = r_u32(input + 12);
            words[4] = r_u32(input + 16);
            do
            {
                xport_gte_write_data(0, words[0]);
                xport_gte_write_data(2, words[1]);
                xport_gte_write_data(4, words[2]);
                xport_gte_write_data(1, words[3]);
                xport_gte_write_data(3, words[3] >> 16);
                xport_gte_write_data(5, words[4]);
                following += 20;
                input += 20;
                xport_gte_execute(0x280030);
                count -= 3;
                output += 48;
                next_words[0] = r_u32(input);
                next_words[1] = r_u32(following - 12);
                next_words[2] = r_u32(following - 8);
                next_words[3] = r_u32(following - 4);
                next_words[4] = r_u32(following);
                for (index = 0; index < 3; ++index)
                {
                    reg = 12 + index;
                    value = xport_gte_read_data(reg);
                    w_u32(output + index * 16, value);
                    reg = 17 + index;
                    value = xport_gte_read_data(reg);
                    w_u32(output + index * 16 + 4, value);
                    value = xport_gte_read_data(8);
                    w_u32(output + index * 16 + 8, value);
                }
                for (index = 0; index < 5; ++index)
                    words[index] = next_words[index];
            } while ((sint32)count > 0);
            flags ^= flags & (flags << 16);
            if (flags & 0x8000000)
                w_u32(state + 64, saved);
            if (flags & 0x2000000)
            {
                if (native_previous != 0)
                    saved = native_previous[slot - 1];
                else
                {
                    table = r_u32(state + 12);
                    saved = r_u32(table + slot * 4 - 4);
                }
                w_u32(state + 32, saved);
            }
            if (flags & 0x4000000)
            {
                if (native_previous != 0)
                    saved = native_previous[slot];
                else
                {
                    table = r_u32(state + 12);
                    saved = r_u32(table + slot * 4);
                }
                w_u32(state + 48, saved);
            }
            if (native_current != 0)
            {
                value = r_u32(state + 80);
                native_current[slot] = value;
            }
            else
            {
                table = r_u32(state + 16);
                value = r_u32(state + 80);
                w_u32(table + slot * 4, value);
            }
            value = r_u32(model + 4);
            w_u32(state + 0x230, value + 4);
            value = r_u32(model + 16);
            w_u32(state + 8, value);
            sub_80061c3c(state);
            value = r_u32(model + 12);
            w_u32(state + 8, value);
            sub_800619e0(state);
        }
        records += 12;
    }
}

uint32 sub_80095018(uint32 color, uint32 target, uint32 scale, uint32 *native_color)
{
    uint32 value, result;
    FUNCTION_MARKER(0x80095018, "ACE3.BIN");
    result = (sint32)scale < 0x1000;
    if ((sint32)scale <= 0)
        return result;
    result = target & 255;
    if ((sint32)scale >= 0x1000)
    {
        if (native_color)
            *native_color = target;
        else
            w_u32(color, target);
        return result;
    }
    xport_gte_write_control(21, result << 4);
    target = (uint32)((sint32)target >> 8);
    value = (target & 255) << 4;
    xport_gte_write_control(22, value);
    result = ((uint32)((sint32)target >> 4)) & 0xFF0;
    xport_gte_write_control(23, result);
    value = native_color ? *native_color : r_u32(color);
    xport_gte_write_data(6, value);
    xport_gte_write_data(8, scale);
    xport_gte_execute(0x780010);
    value = xport_gte_read_data(22);
    if (native_color)
        *native_color = value;
    else
        w_u32(color, value);
    return result;
}

uint32 sub_80094718(uint32 state, uint32 context)
{
    uint32 scene, camera, value, pitch, matrix, a, b, c, d, e, effects, flags;
    FUNCTION_MARKER(0x80094718, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 4);
    w_u32(state + 8, (sint32)value >> 12);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 8);
    w_u32(state + 12, (sint32)value >> 12);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 12);
    w_u32(state + 16, (sint32)value >> 12);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 20);
    w_u16(state + 20, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 22);
    w_u16(state + 22, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 24);
    matrix = context + 8;
    w_u16(state + 24, value);
    camera = r_u32(scene + 40);
    pitch = r_u16(state + 20);
    value = r_u16(camera + 36);
    w_u16(state + 26, value);
    w_u32(context, pitch);
    value = r_u16(state + 24);
    w_u16(context + 4, value);
    sub_800308f0(context, matrix, 0, 0);
    a = r_u32(matrix);
    b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(matrix + 8);
    d = r_u32(matrix + 12);
    e = r_u32(matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    w_u32(context + 40, 0);
    effects = r_u32(0x800BCF90);
    w_u16(context + 244, 0xFF60);
    w_u16(context + 232, 0xFF60);
    w_u16(context + 240, 160);
    w_u16(context + 236, 160);
    w_u16(context + 238, 0xFF88);
    w_u16(context + 234, 0xFF88);
    w_u16(context + 246, 120);
    w_u16(context + 242, 120);
    flags = r_u32(effects + 0x5C8);
    w_u32(context + 360, flags);
    if (flags & 1)
        value = 1;
    else
    {
        scene = r_u32(0x800BCE54);
        camera = r_u32(scene + 0x244);
        value = (r_u16(camera + 2) & 0x400) != 0;
    }
    w_u32(context + 364, value);
    return 1;
}

uint32 sub_8005f2b4(uint32 first, uint32 second, uint32 width, uint32 height, uint16 *native_first, uint16 *native_second)
{
    uint32 codes[2], x, y, left, top, index, iteration, edge, a, b, c, numerator, divisor, value, pointer;
    uint16 *native_pointer;
    FUNCTION_MARKER(0x8005F2B4, "ACE3.BIN");
    left = 0 - width;
    top = 0 - height;
    for (index = 0; index < 2; ++index)
    {
        pointer = index ? second : first;
        native_pointer = index ? native_second : native_first;
        x = native_pointer ? (uint32)(sint32)(sint16)native_pointer[0] : (uint32)r_s16(pointer);
        codes[index] = (sint32)x < (sint32)left ? 1 : (sint32)width < (sint32)x ? 2 : 0;
        y = native_pointer ? (uint32)(sint32)(sint16)native_pointer[1] : (uint32)r_s16(pointer + 2);
        if ((sint32)y < (sint32)top)
            codes[index] |= 4;
        else if ((sint32)height < (sint32)y)
            codes[index] |= 8;
    }
    for (iteration = 0; iteration < 8; ++iteration)
    {
        if (codes[0] & codes[1])
            return 0;
        if ((codes[0] | codes[1]) == 0)
            return 1;
        if (codes[0] == 0)
        {
            pointer = first;
            first = second;
            second = pointer;
            native_pointer = native_first;
            native_first = native_second;
            native_second = native_pointer;
            codes[1] = 0;
        }
        else if (codes[0] & 3)
        {
            edge = codes[0] & 1 ? left : width;
            a = native_second ? (uint32)(sint32)(sint16)native_second[1] : (uint32)r_s16(second + 2);
            b = native_first ? (uint32)(sint32)(sint16)native_first[1] : (uint32)r_s16(first + 2);
            c = native_first ? (uint32)(sint32)(sint16)native_first[0] : (uint32)r_s16(first);
            numerator = (a - b) * (edge - c);
            a = native_second ? (uint32)(sint32)(sint16)native_second[0] : (uint32)r_s16(second);
            divisor = a - c;
            if (divisor == 0)
                xport_mips_break(7);
            if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                xport_mips_break(6);
            value = (uint32)((sint32)numerator / (sint32)divisor);
            a = native_first ? native_first[1] : r_u16(first + 2);
            if (native_first)
            {
                native_first[0] = (uint16)edge;
                native_first[1] = (uint16)(a + value);
            }
            else
            {
                w_u16(first, edge);
                w_u16(first + 2, a + value);
            }
        }
        else if (codes[0] & 12)
        {
            edge = codes[0] & 4 ? top : height;
            a = native_second ? (uint32)(sint32)(sint16)native_second[0] : (uint32)r_s16(second);
            b = native_first ? (uint32)(sint32)(sint16)native_first[0] : (uint32)r_s16(first);
            c = native_first ? (uint32)(sint32)(sint16)native_first[1] : (uint32)r_s16(first + 2);
            numerator = (a - b) * (edge - c);
            a = native_second ? (uint32)(sint32)(sint16)native_second[1] : (uint32)r_s16(second + 2);
            divisor = a - c;
            if (divisor == 0)
                xport_mips_break(7);
            if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                xport_mips_break(6);
            value = (uint32)((sint32)numerator / (sint32)divisor);
            a = native_first ? native_first[0] : r_u16(first);
            if (native_first)
            {
                native_first[1] = (uint16)edge;
                native_first[0] = (uint16)(a + value);
            }
            else
            {
                w_u16(first + 2, edge);
                w_u16(first, a + value);
            }
        }
        x = native_first ? (uint32)(sint32)(sint16)native_first[0] : (uint32)r_s16(first);
        codes[0] = (sint32)x < (sint32)left ? 1 : (sint32)width < (sint32)x ? 2 : 0;
        y = native_first ? (uint32)(sint32)(sint16)native_first[1] : (uint32)r_s16(first + 2);
        if ((sint32)y < (sint32)top)
            codes[0] |= 4;
        else if ((sint32)height < (sint32)y)
            codes[0] |= 8;
    }
    return 1;
}

void sub_800948a0(uint32 context, uint32 count)
{
    uint32 source, output, depths, index, flags, value, z;
    FUNCTION_MARKER(0x800948A0, "ACE3.BIN");
    xport_gte_write_data(4, 0);
    xport_gte_write_data(5, 0);
    source = context + 44;
    output = context + 84;
    depths = context + 124;
    flags = 1;
    for (index = 0; (sint32)index < (sint32)count; ++index, source += 8, output += 8, depths += 4)
    {
        z = r_u32(depths);
        value = r_u32(source);
        xport_gte_write_data(0, value);
        value = r_u32(source + 4);
        xport_gte_write_data(2, value);
        xport_gte_write_data(1, z);
        xport_gte_write_data(3, z);
        xport_gte_execute(0x280030);
        if (flags == 0)
            sub_8005f2b4(output - 8, output - 4, 160, 120, 0, 0);
        flags = xport_gte_read_control(31) & 0x6000;
        if (flags == 0)
        {
            value = xport_gte_read_data(12);
            w_u32(source, value);
            value = xport_gte_read_data(13);
            w_u32(source + 4, value);
            value = xport_gte_read_data(12);
            w_u32(output, value);
            value = xport_gte_read_data(13);
            w_u32(output + 4, value);
            value = r_u16(context + 40);
            w_u16(context + 40, value | ((uint32)1 << (index & 31)));
        }
    }
    if (flags == 0)
        sub_8005f2b4(output - 8, output - 4, 160, 120, 0, 0);
}

uint32 sub_800949ac(uint32 context)
{
    uint32 source, target, index, mask, offset, corner, value, packed, code;
    FUNCTION_MARKER(0x800949AC, "ACE3.BIN");
    source = context + 44;
    target = context + 128;
    index = 0;
    mask = r_s16(context + 40);
    w_u32(context + 124, 0xAAA);
    offset = 152;
    while (mask && index < 5)
    {
        w_u32(target, 0);
        if (mask & 1)
        {
            value = r_u32(source);
            xport_gte_write_data(12, value);
            value = r_u32(source + 4);
            xport_gte_write_data(13, value);
            for (corner = 0; corner < 4; ++corner)
            {
                value = r_u32(context + 232 + corner * 4);
                xport_gte_write_data(14, value);
                xport_gte_execute(0x1400006);
                value = xport_gte_read_data(24);
                w_u32(context + offset + corner * 4, value);
                value = r_u32(context + offset + corner * 4);
                code = value == 0 ? 3 : (sint32)value > 0 ? 2 : 1;
                packed = r_u32(target);
                w_u32(target, packed | (code << (corner * 2)));
            }
            packed = r_u32(target);
            w_u32(target, packed | (packed << 8));
        }
        mask = (uint32)((sint32)mask >> 1);
        offset += 16;
        ++index;
        source += 8;
        target += 4;
    }
    ++index;
    while (index < 7)
    {
        w_u32(context + 124 + index * 4, 0x555);
        ++index;
    }
    return 0;
}

uint32 sub_80094ac8(uint32 state, uint32 context)
{
    uint32 altitude, shift, scene, data, value, divisor, width, index, field, a, b, limit, result;
    FUNCTION_MARKER(0x80094AC8, "ACE3.BIN");
    altitude = 0 - r_u32(state + 12);
    shift = (sint32)altitude > 0x8FFE ? 3 : 2;
    scene = r_u32(0x800BCE54);
    data = r_u32(scene + 0x244);
    value = r_s16(data + 34);
    w_u16(context + 46, (sint32)(altitude + (value << 1)) >> shift);
    data = r_u32(scene + 0x244);
    value = r_s16(data + 32);
    w_u16(context + 54, (sint32)(altitude + (value << 1)) >> shift);
    data = r_u32(scene + 0x244);
    value = r_s16(data + 16);
    w_u16(context + 62, (sint32)(altitude + (value << 1)) >> shift);
    data = r_u32(scene + 0x244);
    value = r_u16(data + 16);
    w_u16(context + 78, (sint32)altitude >> shift);
    value = (uint32)((sint32)(value << 16) >> 17);
    w_u16(context + 70, (sint32)(altitude + value) >> shift);
    divisor = r_s16(state + 26);
    if (divisor == 0)
        xport_mips_break(7);
    width = (uint32)((sint32)0x3BFF88 / (sint32)divisor);
    for (index = 0, field = context; index < 5; ++index, field += 8)
    {
        value = r_u16(field + 46);
        w_u16(field + 44, 0 - width);
        w_u16(field + 48, width);
        w_u16(field + 50, value);
        if (index == 2)
            width = (uint32)((sint32)width >> 1);
    }
    value = 0x7FFF >> (shift - 1);
    w_u32(context + 128, value);
    w_u32(context + 124, value);
    value = 0x7800 >> (shift - 1);
    w_u32(context + 132, value);
    a = r_u32(context + 368);
    w_u32(context + 136, 0x7800 >> shift);
    if (a)
    {
        value = r_u16(context + 78);
        w_u32(context + 272, 0x6800);
        value += 0xC00;
        w_u16(context + 82, value);
        w_u16(context + 78, value);
    }
    else if (r_u32(context + 364))
    {
        scene = r_u32(0x800BCE54);
        data = r_u32(scene + 40);
        a = r_s16(data + 40);
        b = r_s16(data + 42);
        value = (uint32)((sint32)(a + b) >> 1);
        w_u32(context + 272, value);
        if ((sint32)value < 0x3400)
        {
            a = r_u16(context + 76);
            b = r_u16(context + 80);
            a = (uint32)((sint32)(a << 16) >> 17);
            b = (uint32)((sint32)(b << 16) >> 17);
            w_u16(context + 76, a);
            w_u16(context + 80, b);
        }
    }
    else
        w_u32(context + 272, 0x6800);
    limit = r_u32(context + 272);
    result = 4;
    if ((sint32)altitude < (sint32)limit)
    {
        limit = r_u32(context + 272);
        xport_gte_write_data(9, limit);
        xport_gte_write_data(10, altitude);
        result = 5;
        xport_gte_execute(0xA00428);
        a = xport_gte_read_data(25);
        b = xport_gte_read_data(26);
        value = sub_80083d88(a - b);
        w_u32(context + 140, (sint32)value >> shift);
    }
    return result;
}

uint32 sub_80094f50(uint32 color, uint32 altitude)
{
    uint32 value, scale, result;
    FUNCTION_MARKER(0x80094F50, "ACE3.BIN");
    if ((sint32)altitude > -41192)
    {
        value = r_u32(color);
        xport_gte_write_data(6, value);
        scale = (uint32)((sint32)(0xFFFF7F18 - altitude) >> 1);
    }
    else
    {
        xport_gte_write_data(6, 0);
        scale = 0x1000 - (uint32)((sint32)(0xFFFF5F18 - altitude) >> 3);
    }
    xport_gte_write_data(8, scale);
    value = r_u8(color) - 96;
    result = value << 4;
    xport_gte_write_control(21, (sint32)value < 0 ? 0 : result);
    value = r_u8(color + 1) - 96;
    result = value << 4;
    xport_gte_write_control(22, (sint32)value < 0 ? 0 : result);
    value = r_u8(color + 2) - 32;
    result = value << 4;
    xport_gte_write_control(23, (sint32)value < 0 ? 0 : result);
    xport_gte_execute(0x780010);
    value = xport_gte_read_data(22);
    w_u32(color, value);
    return result;
}

uint32 sub_80094d10(uint32 state, uint32 context)
{
    uint32 scene, data, value, altitude, angle, a, b, index, root, intensity, result, shifted;
    FUNCTION_MARKER(0x80094D10, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 28);
    w_u32(context + 248, value | 0x30303);
    altitude = r_u32(state + 12);
    if ((sint32)altitude < -33000)
        sub_80094f50(context + 248, altitude);
    scene = r_u32(0x800BCE54);
    value = r_u16(state + 22);
    data = r_u32(scene + 0x244);
    a = r_s16(data + 62);
    value = (uint32)((sint32)(value << 16) >> 17);
    angle = ((a << 1) - value) & 0x1FFF;
    if (angle >= 0x1001)
        angle = 0x2000 - angle;
    value = r_u32(data + 20);
    w_u32(context + 252, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 36);
    w_u32(context + 256, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 24);
    w_u32(context + 260, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 40);
    w_u32(context + 264, value);
    if (angle >= 0x1000)
    {
        a = r_u32(context + 260);
        b = r_u32(context + 264);
        w_u32(context + 252, a);
        w_u32(context + 256, b);
    }
    else if (angle)
    {
        for (index = 0; index < 2; ++index)
        {
            a = r_u32(context + 252 + index * 4);
            b = r_u32(context + 260 + index * 4);
            if (a != b)
                sub_80095018(context + 252 + index * 4, b, angle, 0);
        }
    }
    scene = r_u32(0x800BCE54);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 8);
    w_u32(context + 264, value);
    w_u32(context + 260, value);
    data = r_u32(scene + 0x244);
    value = r_u32(data + 4);
    result = r_u32(context + 360) & 2;
    w_u32(context + 268, value);
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    intensity = r_u32(root + 0x5CC);
    result = root;
    if (intensity == 0)
        return result;
    xport_gte_write_data(6, 0);
    xport_gte_write_data(8, intensity);
    for (index = 0; index < 3; ++index)
    {
        value = r_u32(context + 248 + index * 4);
        xport_gte_write_data(9, 0x5B0);
        xport_gte_write_data(10, 0x5B0);
        xport_gte_write_data(11, 0x5B0);
        xport_gte_write_data(25, (value & 255) << 4);
        shifted = (uint32)((sint32)value >> 8);
        xport_gte_write_data(26, (shifted & 255) << 4);
        xport_gte_write_data(27, ((uint32)((sint32)shifted >> 4)) & 0xFF0);
        xport_gte_execute(0x1A8003E);
        value = xport_gte_read_data(22);
        w_u32(context + 248 + index * 4, value);
    }
    return 0;
}

void sub_80095074(uint32 state, uint32 context)
{
    uint32 scene, data, extra, mask, value, angle, color, packet, next, altitude, limit;
    FUNCTION_MARKER(0x80095074, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    data = r_u32(scene + 0x244);
    value = r_u16(data + 2);
    extra = 0;
    if (value & 0x100)
    {
        data = r_u32(scene + 0x268);
        extra = r_u8(data + 28) + 1;
    }
    mask = r_s16(context + 40);
    if (mask)
    {
        if (mask & 1)
            sub_80095298(state, context, 0);
        if (mask & 3)
            sub_8009546c(state, context, 1);
        if (mask & 6)
            sub_8009546c(state, context, 2);
        if (extra)
            sub_80095cb8(state, context, extra);
        if (mask & 12)
            sub_8009546c(state, context, 3);
        if (mask & 8)
        {
            if (r_u32(context + 368))
            {
                value = r_u32(context + 268);
                w_u32(context + 264, value);
                sub_8009546c(state, context, 4);
            }
            else
                sub_80095298(state, context, 4);
        }
        if ((mask & 16) && r_u32(context + 364))
            sub_80095298(state, context, 5);
        return;
    }
    value = r_u32(state + 12);
    angle = (uint32)ratan2(value, 0x7FFF) << 2;
    value = r_s16(state + 20);
    if ((sint32)angle < (sint32)value)
        color = r_u32(context + 248);
    else
    {
        altitude = r_u32(state + 12);
        limit = r_u32(context + 272);
        altitude = 0 - altitude;
        extra = 0;
        if ((sint32)altitude >= (sint32)limit)
            color = r_u32(context + 264);
        else
        {
            value = r_u32(context + 364);
            color = 0xFFFFFFFF;
            if (value)
                color = r_u32(context + 268);
        }
    }
    if ((sint32)color >= 0)
    {
        packet = r_u32(state + 4);
        next = packet + 16;
        w_u32(packet, (next & 0xFFFFFF) | 0x03000000);
        w_u32(packet + 4, color | 0x60000000);
        value = r_u32(context + 232);
        w_u32(packet + 12, 0xF00140);
        w_u32(packet + 8, value);
        w_u32(state + 4, next);
    }
    if (extra)
        sub_80095cb8(state, context, extra);
}

uint32 sub_80095298(uint32 state, uint32 context, uint32 index)
{
    uint32 count, corner, first, second, low0, low1, base, value, packet, next, color, quad_color, triangle_color, cursor;
    FUNCTION_MARKER(0x80095298, "ACE3.BIN");
    count = 0;
    base = context + (index << 3);
    first = r_u32(context + 124 + index * 4);
    second = r_u32(context + 124 + (index + 1) * 4);
    for (corner = 0; corner < 4; ++corner)
    {
        low0 = first & 15;
        low1 = second & 15;
        if (((first | second) & 3) == 3)
        {
            value = r_u32(context + 232 + corner * 4);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low0 == 9)
        {
            value = r_u32(base + 80);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low1 == 9)
        {
            value = r_u32(base + 88);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low1 == 6)
        {
            value = r_u32(base + 84);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low0 == 6)
        {
            value = r_u32(base + 76);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        first = (uint32)((sint32)first >> 2);
        second = (uint32)((sint32)second >> 2);
    }
    if (count < 3)
        return index << 2;
    color = r_u32(context + 248 + index * 4);
    quad_color = color | 0x28000000;
    triangle_color = color | 0x20000000;
    cursor = context + count * 4;
    while (count >= 4)
    {
        packet = r_u32(state + 4);
        next = packet + 24;
        w_u32(state + 4, next);
        w_u32(packet, (next & 0xFFFFFF) | 0x05000000);
        w_u32(packet + 4, quad_color);
        value = r_u32(context + 276);
        w_u32(packet + 8, value);
        value = r_u32(cursor + 264);
        w_u32(packet + 12, value);
        value = r_u32(cursor + 272);
        count -= 2;
        w_u32(packet + 16, value);
        value = r_u32(cursor + 268);
        cursor -= 8;
        w_u32(packet + 20, value);
    }
    if (count != 3)
        return 1;
    packet = r_u32(state + 4);
    next = packet + 20;
    w_u32(state + 4, next);
    w_u32(packet, (next & 0xFFFFFF) | 0x04000000);
    w_u32(packet + 4, triangle_color);
    value = r_u32(context + 276);
    w_u32(packet + 8, value);
    value = r_u32(context + 280);
    w_u32(packet + 12, value);
    value = r_u32(context + 284);
    w_u32(packet + 16, value);
    return value;
}
