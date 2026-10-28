#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8009546c(uint32 state, uint32 context, uint32 index)
{
    uint32 current_color, previous_color, base, first, second, area, value, count, corner, low0, low1;
    uint32 numerator, ratio, packet, next, cursor;
    FUNCTION_MARKER(0x8009546C, "ACE3.BIN");
    current_color = r_u32(context + 248 + index * 4);
    previous_color = r_u32(context + 248 + (index - 1) * 4);
    xport_gte_write_control(21, (current_color & 255) << 4);
    xport_gte_write_control(22, ((uint32)((sint32)current_color >> 4)) & 0xFF0);
    xport_gte_write_control(23, ((uint32)((sint32)current_color >> 12)) & 0xFF0);
    xport_gte_write_data(6, previous_color);
    base = context + index * 8;
    value = r_u32(base + 36);
    xport_gte_write_data(12, value);
    value = r_u32(base + 40);
    xport_gte_write_data(13, value);
    value = r_u32(base + 44);
    xport_gte_write_data(14, value);
    first = r_u32(context + 124 + index * 4);
    second = r_u32(context + 124 + (index + 1) * 4);
    xport_gte_execute(0x1400006);
    area = xport_gte_read_data(24);
    area = (uint32)((sint32)area >> 4);
    count = 0;
    for (corner = 0; corner < 4; ++corner)
    {
        low0 = first & 15;
        low1 = second & 15;
        if (((first | second) & 3) == 3)
        {
            value = r_u32(context + 232 + corner * 4);
            w_u32(context + 276 + count * 4, value);
            value = 0;
            if (area)
                value = r_u32(context + 152 + (index - 1) * 16 + corner * 4);
            if (area && (sint32)value > 0)
            {
                numerator = value << 8;
                if (area == 0xFFFFFFFF && numerator == 0x80000000)
                    xport_mips_break(6);
                ratio = (uint32)((sint32)numerator / (sint32)area);
                xport_gte_write_data(8, ratio);
                xport_gte_execute(0x780010);
                value = xport_gte_read_data(22);
                w_u32(context + 308 + count * 4, value);
            }
            else
                w_u32(context + 308 + count * 4, previous_color);
            ++count;
        }
        if (low0 == 9)
        {
            value = r_u32(base + 80);
            w_u32(context + 308 + count * 4, previous_color);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low1 == 9)
        {
            value = r_u32(base + 88);
            w_u32(context + 308 + count * 4, current_color);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low1 == 6)
        {
            value = r_u32(base + 84);
            w_u32(context + 308 + count * 4, current_color);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        if (low0 == 6)
        {
            value = r_u32(base + 76);
            w_u32(context + 308 + count * 4, previous_color);
            w_u32(context + 276 + count * 4, value);
            ++count;
        }
        first = (uint32)((sint32)first >> 2);
        second = (uint32)((sint32)second >> 2);
    }
    while (count >= 4)
    {
        packet = r_u32(state + 4);
        next = packet + 36;
        w_u32(state + 4, next);
        w_u32(packet, (next & 0xFFFFFF) | 0x08000000);
        value = r_u32(context + 308);
        w_u32(packet + 4, value | 0x38000000);
        value = r_u32(context + 276);
        cursor = context + count * 4;
        w_u32(packet + 8, value);
        value = r_u32(context + 308 + (count - 3) * 4);
        w_u32(packet + 12, value);
        value = r_u32(cursor + 264);
        w_u32(packet + 16, value);
        value = r_u32(context + 308 + (count - 1) * 4);
        w_u32(packet + 20, value);
        value = r_u32(cursor + 272);
        w_u32(packet + 24, value);
        value = r_u32(context + 308 + (count - 2) * 4);
        w_u32(packet + 28, value);
        value = r_u32(cursor + 268);
        count -= 2;
        w_u32(packet + 32, value);
    }
    if (count != 3)
        return 1;
    packet = r_u32(state + 4);
    next = packet + 28;
    w_u32(state + 4, next);
    w_u32(packet, (next & 0xFFFFFF) | 0x06000000);
    value = r_u32(context + 308);
    w_u32(packet + 4, value | 0x30000000);
    value = r_u32(context + 276);
    w_u32(packet + 8, value);
    value = r_u32(context + 312);
    w_u32(packet + 12, value);
    value = r_u32(context + 280);
    w_u32(packet + 16, value);
    value = r_u32(context + 316);
    w_u32(packet + 20, value);
    value = r_u32(context + 284);
    w_u32(packet + 24, value);
    return value;
}

void sub_80095cb8(uint32 state, uint32 context, uint32 count)
{
    uint32 scene, matrix, a, b, c, d, e, start, block, data, samples, scale, x, y, z, color, channel;
    uint32 index, used, value, packed, depth, distance, point, next;
    sint32 coordinate;
    FUNCTION_MARKER(0x80095CB8, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    if ((sint32)count >= 257)
        count = 256;
    scene = r_u32(0x800BCE54);
    matrix = scene + 44;
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
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x2000);
    start = r_u32(state + 4);
    block = start + 8;
    xport_gte_execute(0x486012);
    w_u32(start, (block & 0xFFFFFF) | 0x01000000);
    w_u32(start + 4, 0xE1000220);
    data = r_u32(scene + 0x268);
    samples = r_u32(0x800BCF90);
    value = r_u8(data + 19);
    scale = (128 - value) << 5;
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_control(21, 0x60);
    xport_gte_write_control(22, 0x70);
    xport_gte_write_control(23, 0x80);
    xport_gte_write_data(6, 0x6A807060);
    xport_gte_write_data(8, scale);
    x = 0 - x;
    y = 0 - y;
    w_u32(context + 344, x);
    w_u32(context + 348, y);
    xport_gte_execute(0x780010);
    z = 0 - z;
    w_u32(context + 352, z);
    w_u16(context, (x & 0x1FFF) - 0x1000);
    w_u16(context + 2, (y & 0x1FFF) - 0x1000);
    w_u16(context + 4, (z & 0x1FFF) - 0x1000);
    color = xport_gte_read_data(22);
    xport_gte_write_data(6, color);
    channel = color >> 8;
    matrix = scene + 76;
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
    xport_gte_write_control(7, 0x2000);
    used = 0;
    for (index = 0; index < count; ++index)
    {
        value = r_u32(context);
        xport_gte_write_data(0, value);
        value = r_u32(context + 4);
        xport_gte_write_data(1, value);
        value = r_u32(samples);
        samples += 4;
        xport_gte_execute(0x180001);
        x = r_u16(context + 344);
        y = r_u16(context + 348);
        x += value;
        value = (uint32)((sint32)value >> 8);
        y += value;
        value = (uint32)((sint32)value >> 8);
        w_u16(context, (x & 0x1FFF) - 0x1000);
        z = r_u16(context + 352);
        w_u16(context + 2, (y & 0x1FFF) - 0x1000);
        z += value;
        w_u16(context + 4, (z & 0x1FFF) - 0x1000);
        packed = xport_gte_read_data(14);
        depth = xport_gte_read_data(19);
        coordinate = (sint32)packed >> 16;
        if (coordinate < 0)
            coordinate = -coordinate;
        if (coordinate >= 121)
            continue;
        coordinate = (sint16)packed;
        if (coordinate < 0)
            coordinate = -coordinate;
        if (coordinate >= 161)
            continue;
        distance = depth - 0x2000;
        if ((sint32)distance < 0)
            distance = 0x2000 - depth;
        if ((sint32)distance >= 0x1001)
            continue;
        xport_gte_write_data(8, distance);
        point = block + used * 8 + 4;
        xport_gte_execute(0x780010);
        ++used;
        w_u32(point + 4, packed);
        if (used >= 7)
        {
            next = block + 60;
            w_u32(block, (next & 0xFFFFFF) | 0x0E000000);
            block = next;
            used = 0;
        }
        value = xport_gte_read_data(22);
        w_u32(point, value);
        if ((index & 15) == 0)
            w_u8(point, channel);
    }
    if (used)
    {
        next = block + used * 8 + 4;
        w_u32(state + 4, next);
        w_u32(block, (used << 25) | (next & 0xFFFFFF));
    }
    else if (start + 8 != block)
        w_u32(state + 4, block);
}

uint32 sub_8003b758(uint32 distance)
{
    uint32 value, result;
    FUNCTION_MARKER(0x8003B758, "ACE3.BIN");
    if ((sint32)distance >= 0x1001)
        return 2;
    value = 0x1000 - distance;
    value *= value;
    value = (uint32)((sint32)value >> 12);
    result = (uint32)((sint32)((value << 1) + value) >> 9);
    if ((sint32)result < 2)
        result = 2;
    if ((sint32)result >= 25)
        result = 24;
    return result;
}

uint32 sub_8003ef40(void)
{
    uint32 data, mode, enabled;
    FUNCTION_MARKER(0x8003EF40, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    mode = r_u8(data + 938);
    enabled = (mode - 5 < 2) || mode == 9;
    if (enabled == 0 && sub_8003a5c0() == 1)
    {
        data = r_u32(0x800BCE60);
        mode = r_u8(data + 939);
        enabled = (mode - 5 < 2) || mode == 9;
    }
    w_u8(0x800C6D70, enabled);
    sub_8003f2cc();
    return sub_80040a24();
}

uint32 sub_800409d4(uint32 index)
{
    FUNCTION_MARKER(0x800409D4, "ACE3.BIN");
    if (r_u8(0x800BCEC8))
        return 0;
    w_u8(0x800BCEC8, 1);
    w_u16(0x800BCECC, 160);
    w_u16(0x800BCECE, 120);
    w_u8(0x800BCEC9, index);
    w_u16(0x800BCECA, 2);
    return 1;
}

uint32 sub_80040a18(void)
{
    FUNCTION_MARKER(0x80040A18, "ACE3.BIN");
    w_u8(0x800BCEC8, 0);
    return 255;
}

uint32 sub_8003f2cc(void)
{
    uint32 index, record, target, table, object, flags, data, value;
    FUNCTION_MARKER(0x8003F2CC, "ACE3.BIN");
    w_u8(0x800C6D72, 255);
    for (index = 2; index < 64; ++index)
    {
        record = r_u32(0x800C67A0 + index * 4);
        target = r_u32(0x800C6ED0 + index * 4);
        if (record == 0)
            continue;
        w_u8(record, 0);
        if (target == 0)
            continue;
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if ((r_u32(object) & 0x4000) == 0)
            continue;
        flags = 0x800C6FD0 + index * 4;
        if ((r_u32(flags) & 0x1000) == 0 || r_u8(record + 2) == 0)
            continue;
        if ((r_u32(target + 32) & 0x20000) == 0 || (r_u32(target) & 1) == 0)
            continue;
        w_u8(record, 1);
        value = r_u8(0x800C6D6E);
        if (value != index)
        {
            if (r_u8(record + 1) == 0 || r_u8(0x800BCEC9) != index)
                continue;
            value = r_u8(record + 1);
            value = sub_80040a18();
            w_u8(record + 1, value);
            continue;
        }
        w_u8(record, 2);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if (r_u32(object) & 0x40)
            continue;
        if ((r_u32(target) & 8) == 0 || (r_u32(flags) & 0x4000) == 0)
            continue;
        data = r_u32(0x800BCEB4);
        if ((r_u32(data) & 4) || r_u8(0x800C6D70))
            continue;
        value = r_u8(record + 1);
        if (value == 0 || r_u8(0x800BCEC9) != index)
        {
            value = sub_800409d4(index);
            w_u8(record + 1, value);
            continue;
        }
        value = r_u8(0x800BCEC8);
        if (value == 2)
        {
            w_u8(record, 3);
            w_u8(0x800C6D72, index);
        }
        else if (value == 0)
            w_u8(record + 1, 0);
    }
    return 0;
}

uint32 sub_80040a24(void)
{
    uint32 index, record, active, target, value, scene, data, mode, table, object, flags;
    uint32 a, b, c, d, dx, dy, ds, step, raw, result;
    FUNCTION_MARKER(0x80040A24, "ACE3.BIN");
    index = r_u8(0x800BCEC9);
    record = r_u32(0x800C67A0 + index * 4);
    if (record == 0)
        return 0;
    active = r_u8(0x800BCEC8);
    if (active == 0)
        return 0;
    if (r_u8(record) == 0)
    {
        sub_80040a18();
        return 0;
    }
    flags = r_u32(0x800C6FD0 + index * 4);
    if ((flags & 1) == 0)
    {
        w_u8(0x800BCEC8, 0);
        return 0;
    }
    target = r_u32(0x800C6ED0 + index * 4);
    if (target == 0)
        return 0;
    if ((r_u32(target) & 8) == 0)
    {
        w_u8(0x800BCEC8, 0);
        return 0;
    }
    if (active == 1)
    {
        scene = r_u32(0x800BCE54);
        if ((r_u32(scene + 12) & 7) == 0)
            sub_8002dbe0(45);
    }
    mode = r_u8(0x800BCEC8);
    if (mode == 2)
    {
        scene = r_u32(0x800BCE54);
        if ((r_u32(scene + 12) & 7) == 0)
            sub_8002dbe0(50);
        if (r_u8(0x800BCEC8) == mode)
        {
            value = r_u16(record + 8);
            w_u16(0x800BCECC, value);
            value = r_u16(record + 10);
            w_u16(0x800BCECE, value);
            value = r_u16(record + 6);
            w_u16(0x800BCECA, value);
            return 0;
        }
    }
    a = r_s16(record + 10);
    b = r_s16(0x800BCECE);
    c = r_s16(record + 8);
    d = r_s16(record + 6);
    dy = a - b;
    a = r_s16(0x800BCECA);
    b = r_s16(0x800BCECC);
    dx = c - b;
    ds = d - a;
    if (dx == 0 && dy == 0 && ds == 0)
        goto arrived;
    step = r_s16(0x800BCECA);
    raw = r_u16(0x800BCECA);
    value = r_u16(0x800BCECC);
    if ((sint32)step < (sint32)dx)
        value += raw;
    else if ((sint32)dx < (sint32)(0 - step))
        value -= raw;
    else
        value += dx;
    w_u16(0x800BCECC, value);
    step = r_s16(0x800BCECA);
    raw = r_u16(0x800BCECA);
    value = r_u16(0x800BCECE);
    if ((sint32)step < (sint32)dy)
        value += raw;
    else if ((sint32)dy < (sint32)(0 - step))
        value -= raw;
    else
        value += dy;
    w_u16(0x800BCECE, value);
    value = r_u16(0x800BCECA);
    if ((sint32)ds >= 5)
        value += 4;
    else if ((sint32)ds < -4)
        value -= 4;
    else
        value += ds;
    w_u16(0x800BCECA, value);
    a = r_s16(record + 10);
    b = r_s16(0x800BCECE);
    c = r_s16(record + 8);
    d = r_s16(record + 6);
    dy = a - b;
    result = r_s16(0x800BCECA);
    value = r_s16(0x800BCECC);
    ds = d - result;
    if (c != value || dy != 0 || ds != 0)
        return result;
arrived:
    if (r_u8(0x800BCEC8) != 2)
    {
        data = r_u32(0x800BCE60);
        mode = r_s16(data + 908);
        if (mode == 4 || mode == 8 || mode == 15 || mode == 30)
        {
            index = r_u8(0x800BCEC9);
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if (r_u32(object) & 0x100)
                sub_8004ceb0(14);
        }
    }
    index = r_u8(0x800BCEC9);
    flags = r_u32(0x800C6FD0 + index * 4);
    if (flags & 0x800000)
    {
        data = r_u32(0x800BCEF8);
        data = r_u32(data + (index << 6) + 16);
        value = r_u8(data + 3);
        sub_80048c0c(8, 0, value, 0);
    }
    w_u8(0x800BCEC8, 2);
    return 0;
}

uint32 sub_80041698(void)
{
    uint32 data, mode, value;
    FUNCTION_MARKER(0x80041698, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    if (r_u32(data) & 0x20000000)
        return 0;
    mode = r_u8(0x800BCFD4);
    if (mode == 1)
    {
        if (r_s16(0x800BCFD6) == 0)
            sub_8002db4c(62);
        value = r_u16(0x800BCFD6) + 16;
        w_u16(0x800BCFD6, value);
        if ((sint16)value >= 200)
        {
            w_u8(0x800BCFD4, 2);
            w_u16(0x800BCFD6, 200);
            w_u32(0x800BCFD0, 0);
        }
    }
    else if (mode == 3)
    {
        if (r_s16(0x800BCFD6) == 200)
            sub_8002db4c(54);
        value = r_u16(0x800BCFD6) - 32;
        w_u16(0x800BCFD6, value);
        if ((sint32)(value << 16) <= 0)
        {
            w_u16(0x800BCFD6, 0);
            w_u8(0x800BCFD4, 0);
        }
    }
    return 0;
}

uint32 sub_800419c0(void)
{
    uint32 data, mode, counter, value;
    FUNCTION_MARKER(0x800419C0, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    if (r_u32(data) & 0x20000000)
        return 0;
    mode = r_u8(0x800C7172);
    if (mode == 1)
    {
        counter = r_s16(0x800C7170);
        if (counter == 0)
        {
            w_u8(0x800C7176, 43);
            w_u8(0x800C7177, 6);
        }
        else if (counter == 1)
        {
            w_u8(0x800C7176, 12);
            w_u8(0x800C7177, 41);
        }
        else if (counter == 2)
        {
            w_u8(0x800C7176, 21);
            w_u8(0x800C7177, 20);
        }
        else if (counter == 3)
        {
            w_u8(0x800C7176, 43);
            w_u8(0x800C7177, 41);
            w_u8(0x800C7172, 2);
        }
        value = r_u16(0x800C7170);
        w_u16(0x800C7170, value + 1);
    }
    else if (mode == 3)
    {
        value = r_u8(0x800C7177);
        w_u8(0x800C7176, 43);
        value >>= 2;
        w_u8(0x800C7177, value);
        if (value == 0)
            w_u8(0x800C7172, 0);
    }
    return 0;
}

uint32 sub_800414a4(void)
{
    uint32 buffers, data, value;
    FUNCTION_MARKER(0x800414A4, "ACE3.BIN");
    buffers = r_u32(0x800BCFC4);
    data = r_u32(0x800BCE60);
    w_u32(buffers + 12, 0);
    w_u32(buffers + 8, 0);
    value = r_u32(data + 4);
    value = r_u32(buffers + (value << 2));
    w_u32(0x800BCFC8, value);
    return value;
}

uint32 sub_800413fc(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 scene, data, value;
    FUNCTION_MARKER(0x800413FC, "ACE3.BIN");
    sub_800414a4();
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 4);
    if (value & 4)
    {
        data = r_u32(0x800BCE60);
        if ((r_u32(data) & 0x20000000) == 0)
            sub_80042544(state, native_state);
    }
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 4);
    if (value & 0x2C00)
        sub_800428f8();
    value = r_s16(0x800BCFD6);
    sub_80041794(value);
    sub_80041adc();
    return sub_800414d8();
}

uint32 sub_800414d8(void)
{
    static const uint32 offsets[6] = {20, 40, 72, 88, 56, 96};
    uint32 scene, packet, ot, value, head, merged, low, result, i, node;
    FUNCTION_MARKER(0x800414D8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    packet = r_u32(0x800BCFC8);
    ot = r_u32(scene + 16);
    value = r_u32(packet);
    head = r_u32(ot);
    w_u32(packet, (value & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    merged = (head & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ot, merged);
    result = r_u8(0x800BCFD4);
    low = merged & 0xFFFFFF;
    if (result == 0)
        return 0;
    for (i = 0; i < 6; ++i)
    {
        node = packet + offsets[i];
        value = r_u32(node);
        w_u32(node, (value & 0xFF000000) | low);
        head = r_u32(ot);
        result = node & 0xFFFFFF;
        merged = (head & 0xFF000000) | result;
        w_u32(ot, merged);
        low = merged & 0xFFFFFF;
    }
    return result;
}

uint32 sub_80041794(uint32 size)
{
    uint32 packet, value, blue, x, width, base;
    FUNCTION_MARKER(0x80041794, "ACE3.BIN");
    packet = r_u32(0x800BCFC8);
    value = r_u8(0x800C6D6B);
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value);
    blue = r_u8(0x800C6D6D);
    w_u16(packet + 8, 271 - size);
    w_u16(packet + 10, 14);
    x = 320 - size;
    w_u8(packet + 6, blue);
    w_u8(packet + 12, size ? 120 : 170);
    packet = r_u32(0x800BCFC8) + 20;
    value = r_u8(0x800C6D6B);
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value);
    blue = r_u8(0x800C6D6D);
    w_u16(packet + 8, x);
    w_u16(packet + 10, 14);
    w_u8(packet + 6, blue);
    width = (sint32)size < 15 ? 0 : size - 14;
    x = 334 - size;
    packet = r_u32(0x800BCFC8) + 40;
    w_u16(packet + 8, x);
    w_u16(packet + 10, 14);
    w_u16(packet + 12, width);
    w_u16(packet + 14, 3);
    value = r_u8(0x800C6D68);
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6A);
    w_u8(packet + 6, value);
    packet = r_u32(0x800BCFC8) + 56;
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 0);
    w_u8(packet + 6, 0);
    base = r_u32(0x800BCFC8);
    w_u16(packet + 8, 329 - size);
    w_u16(packet + 10, 17);
    w_u16(packet + 12, width + 5);
    w_u16(packet + 14, 34);
    packet = base + 72;
    w_u16(packet + 8, x);
    w_u16(packet + 10, 51);
    w_u16(packet + 12, width);
    w_u16(packet + 14, 3);
    value = r_u8(0x800C6D68);
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6A);
    w_u8(packet + 6, value);
    return value;
}

uint32 sub_80041adc(void)
{
    static const uint32 offsets[7] = {0x210, 0x1E8, 0x248, 0x258, 0x268, 0x238, 0x270};
    uint32 width, height, value, x, y, inner_x, inner_y, right, bottom, packet, base, scene;
    uint32 blue, texture, divisor, numerator, remainder, i, ot, tag, head, node;
    FUNCTION_MARKER(0x80041ADC, "ACE3.BIN");
    if (r_u8(0x800C7172) == 0)
        return 0;
    width = r_u8(0x800C7176);
    height = r_u8(0x800C7177);
    value = r_u8(0x800C7176);
    width -= 11;
    height -= 5;
    value = 43 - value;
    x = r_u16(0x800C7178);
    x += (sint32)value / 2;
    value = r_u8(0x800C7177);
    inner_x = x + 6;
    value = 41 - value;
    y = r_u16(0x800C717A);
    y += (sint32)value / 2;
    inner_y = y + 3;
    if ((sint32)width <= 0)
        width = 1;
    if ((sint32)height <= 0)
        height = 1;
    packet = r_u32(0x800BCFC8) + 0x210;
    value = r_u8(0x800C6D6B);
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value);
    blue = r_u8(0x800C6D6D);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    w_u8(packet + 6, blue);
    value = r_u8(0x800C7176);
    w_u16(packet + 18, y);
    w_u16(packet + 24, x);
    w_u16(packet + 16, x + value);
    value = r_u8(0x800C7177);
    w_u16(packet + 26, y + value);
    value = r_u8(0x800C7176);
    w_u16(packet + 32, x + value);
    value = r_u8(0x800C7177);
    right = width + x + 5;
    w_u16(packet + 34, y + value);
    packet = r_u32(0x800BCFC8) + 0x1E8;
    texture = r_u8(0x800C7173);
    bottom = height + y + 2;
    texture = (texture << 6) + 0x3216;
    w_u16(packet + 8, inner_x);
    w_u16(packet + 10, inner_y);
    w_u16(packet + 16, right);
    w_u16(packet + 18, inner_y);
    w_u16(packet + 24, inner_x);
    w_u16(packet + 26, bottom);
    w_u16(packet + 32, right);
    w_u16(packet + 34, bottom);
    w_u16(packet + 14, texture);
    value = r_u8(0x800C7173);
    w_u8(packet + 12, (value & 7) << 5);
    value = r_u8(0x800C7173);
    w_u8(packet + 13, (value >> 3) * 36 - 128);
    value = r_u8(0x800C7173);
    w_u8(packet + 20, ((value & 7) << 5) + 31);
    value = r_u8(0x800C7173);
    w_u8(packet + 21, (value >> 3) * 36 - 128);
    value = r_u8(0x800C7173);
    w_u8(packet + 28, (value & 7) << 5);
    value = r_u8(0x800C7173);
    w_u8(packet + 29, (value >> 3) * 36 - 93);
    value = r_u8(0x800C7173);
    width -= 2;
    w_u8(packet + 36, ((value & 7) << 5) + 31);
    value = r_u8(0x800C7173);
    w_u8(packet + 37, (value >> 3) * 36 - 93);
    if ((sint32)(width << 16) <= 0)
        width = 1;
    height -= 3;
    if ((sint32)(height << 16) <= 0)
        height = 1;
    x += 7;
    value = y + 4;
    divisor = (sint16)height;
    base = r_u32(0x800BCFC8);
    scene = r_u32(0x800BCE54);
    packet = base + 0x238;
    w_u16(packet + 8, x);
    w_u16(packet + 10, value);
    w_u16(packet + 12, width);
    w_u16(packet + 14, height);
    packet = base + 0x248;
    w_u16(packet + 8, x);
    numerator = r_u32(scene + 12) << 2;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    remainder = (sint32)numerator % (sint32)divisor;
    w_u16(packet + 12, width);
    w_u16(packet + 14, 1);
    w_u16(packet + 10, inner_y + remainder);
    packet = base + 0x258;
    w_u16(packet + 8, x);
    value = r_u32(scene + 12);
    numerator = (value << 2) + value;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    remainder = (sint32)numerator % (sint32)divisor;
    w_u16(packet + 14, 1);
    w_u16(packet + 12, width);
    w_u16(packet + 10, inner_y + remainder);
    for (i = 0; i < 7; ++i)
    {
        node = base + offsets[i];
        ot = r_u32(scene + 16);
        tag = r_u32(node);
        head = r_u32(ot);
        w_u32(node, (tag & 0xFF000000) | (head & 0xFFFFFF));
        ot = r_u32(scene + 16);
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | (node & 0xFFFFFF));
    }
    return 0;
}

uint32 sub_8004167c(void)
{
    FUNCTION_MARKER(0x8004167C, "ACE3.BIN");
    if (r_u8(0x800BCFD4) == 2)
        w_u8(0x800BCFD4, 3);
    return 3;
}

uint32 sub_80041654(void)
{
    FUNCTION_MARKER(0x80041654, "ACE3.BIN");
    if (r_u8(0x800BCFD4) != 2)
        w_u8(0x800BCFD4, 1);
    return 1;
}

uint32 sub_80041670(void)
{
    FUNCTION_MARKER(0x80041670, "ACE3.BIN");
    return r_u8(0x800BCFD4);
}

uint32 sub_8004287c(uint32 index)
{
    uint32 current, record, value;
    FUNCTION_MARKER(0x8004287C, "ACE3.BIN");
    current = r_u32(0x800C7180);
    if (current != 0xFFFFFFFF && (sint32)index >= (sint32)current)
        return 0;
    record = 0x800B149C + (index << 2);
    w_u32(0x800C7180, index);
    value = r_s16(record);
    w_u8(0x800C7188, 1);
    w_u32(0x800C7184, value);
    value = r_u8(record + 2);
    w_u16(0x800C718A, 0);
    w_u8(0x800C7189, value);
    w_u8(0x800C718C, 1);
    value = r_u8(record + 3);
    w_u16(0x800C718E, 0);
    w_u8(0x800C718D, value);
    return value;
}

uint32 sub_80042544(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 flags, table, object, accumulating, kind, data, mission, bounds, position, limit, value;
    uint32 first, second, result;
    FUNCTION_MARKER(0x80042544, "ACE3.BIN");
    flags = r_u32(0x800C6ECC);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    accumulating = 0;
    if (flags & 2)
    {
        kind = 1;
        goto issue;
    }
    if (flags & 1)
    {
        kind = 2;
        goto issue;
    }
    if ((native_state ? native_state->words[0] : r_u32(state)) & 7)
    {
        sub_8004287c(0);
        if ((native_state ? native_state->words[0] : r_u32(state)) & 2)
            sub_80048c0c(1, 60, 0, 0);
        if ((native_state ? native_state->words[0] : r_u32(state)) & 4)
            sub_80048c0c(2, 60, 0, 0);
        goto finish;
    }
    if (sub_800476f0(0) == 1)
    {
        data = r_u32(0x800BCE60);
        mission = r_s16(data + 908);
        if (mission != 32 && mission != 47 && mission != 53)
        {
            kind = 3;
            goto issue;
        }
    }
    bounds = r_u32(0x800BCEB0);
    position = r_u32(object + 12);
    value = r_u32(bounds + 8);
    if ((sint32)position < (sint32)value)
    {
        kind = 4;
        goto issue;
    }
    if ((native_state ? native_state->words[0] : r_u32(state)) & 0x18)
    {
        kind = 5;
        goto issue;
    }
    limit = r_u32(bounds + 12);
    value = (native_state ? native_state->words[12] : r_u32(state + 48));
    if ((sint32)value < (sint32)(0 - limit))
        goto outside;
    data = r_u32(0x800BCE60);
    mission = r_s16(data + 908);
    if ((mission == 42 || mission == 58) && (sint32)limit < (sint32)position)
    {
    outside:
        value = r_u8(0x800BCFC0) + 1;
        w_u8(0x800BCFC0, value);
        accumulating = 1;
        if ((value & 255) >= 30)
        {
            kind = 6;
            goto issue;
        }
        goto finish;
    }
    if ((native_state ? native_state->words[7] : r_u32(state + 28)) == 0)
    {
        data = r_u32(0x800BCE60);
        if (r_u32(data + 300) & 0x1000)
        {
            kind = 7;
            goto issue;
        }
    }
    data = r_u32(0x800BCEB4);
    flags = r_u32(data);
    if (flags & 8)
    {
        kind = 8;
        goto issue;
    }
    data = r_u32(0x800BCE60);
    value = r_u32(data + 296);
    if ((sint32)value < 0)
    {
        if (r_u32(data + 300) & 0x80000000)
            sub_8002db4c(55);
        kind = 9;
        goto issue;
    }
    if (flags & 16)
    {
        mission = r_s16(data + 908);
        if (mission == 3)
        {
            data = r_u32(0x800BCE54);
            if (r_u32(data + 8) == 0)
            {
                kind = 10;
                goto issue;
            }
        }
    }
    goto finish;
issue:
    sub_8004287c(kind);
finish:
    if (accumulating == 0)
        w_u8(0x800BCFC0, 0);
    value = (native_state ? native_state->words[7] : r_u32(state + 28));
    data = r_u32(0x800BCEB4);
    flags = r_u32(data);
    if (value == 0)
        w_u32(data, flags | 0x20);
    else
        w_u32(data, flags & 0xFFFFFFDF);
    result = r_u32(0x800C7180);
    if (result == 0xFFFFFFFF)
        return result;
    first = sub_80041f88(0x800C7188);
    second = sub_80041f88(0x800C718C);
    data = r_u32(0x800BCE60);
    flags = r_u32(data);
    result = 0x20000000;
    if ((flags & 0x20000000) == 0)
    {
        result = r_u32(0x800C7184) - 1;
        w_u32(0x800C7184, result);
    }
    if (first == 0 && second == 0)
    {
        result = r_u32(0x800C7184);
        if (result == 0)
            w_u32(0x800C7180, 0xFFFFFFFF);
    }
    return result;
}

uint32 sub_80041f88(uint32 state)
{
    uint32 scene, active, ot, index, data, record, value, limit, mode, extra, buffers, count;
    uint32 icon, packet, texture, x, y, tag, head, draw, color, i;
    FUNCTION_MARKER(0x80041F88, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    active = r_u8(state);
    ot = r_u32(scene + 16);
    if (active == 0)
        return 0;
    if (r_u8(state + 1) == 255)
        return 0;
    index = r_u8(state + 1);
    data = r_u32(0x800BCE60);
    record = 0x800B1254 + (index << 4);
    if ((r_u32(data) & 0x20000000) == 0)
    {
        value = r_s16(state + 2);
        limit = r_s16(record + 14);
        if (value == limit && sub_8004c858() == 255)
        {
            value = r_s16(record + 12);
            sub_8002db4c(value);
        }
        value = r_u16(state + 2) + 1;
        w_u16(state + 2, value);
        limit = r_s16(record + 2);
        if ((sint16)value >= (sint32)limit)
            w_u8(state, 0);
    }
    mode = r_u8(record);
    extra = mode & 0xF0;
    mode &= 15;
    if (mode == 1)
    {
        if ((r_u16(state + 2) & 6) == 0)
            return r_u8(state);
    }
    else if (mode == 2)
    {
        if ((r_u16(state + 2) & 2) == 0)
            return r_u8(state);
    }
    buffers = r_u32(0x800BCFC4);
    count = r_u32(buffers + 8);
    icon = r_u8(record + 8);
    packet = r_u32(0x800BCFC8) + count * 20 + 104;
    for (i = 0; i < 2; ++i)
    {
        if (i != 0)
        {
            icon = r_u8(record + 10);
            if (icon == 255)
                break;
            packet += 20;
        }
        w_u8(packet + 4, 80);
        w_u8(packet + 5, 80);
        w_u8(packet + 6, 80);
        w_u16(packet + 14, 0x7F9B);
        value = r_u8(record + 9 + i * 2);
        x = r_u16(record + 4);
        w_u16(packet + 8, x + value);
        texture = 0x800B1424 + (icon << 2);
        y = r_u16(record + 6);
        tag = r_u32(packet);
        w_u16(packet + 10, y + 2);
        value = r_u8(texture);
        w_u8(packet + 12, value);
        value = r_u8(texture + 1);
        w_u8(packet + 13, value);
        value = r_u8(texture + 2);
        w_u16(packet + 16, value);
        value = r_u8(texture + 3);
        w_u16(packet + 18, value);
        head = r_u32(ot);
        buffers = r_u32(0x800BCFC4);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
        value = r_u32(buffers + 8);
        w_u32(buffers + 8, value + 1);
    }
    if (extra != 0)
    {
        color = r_u8(record + 1);
        packet += 20;
        if (color == 0)
        {
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
        }
        else if (color == 1)
        {
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 64);
            w_u8(packet + 6, 0);
        }
        else if (color == 2)
        {
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 128);
        }
        else
        {
            w_u8(packet + 4, 255);
            w_u8(packet + 5, 255);
            w_u8(packet + 6, 255);
        }
        w_u16(packet + 14, 0x7F9B);
        value = r_u16(record + 4);
        w_u16(packet + 8, value + 28);
        value = r_u16(record + 6);
        value += extra == 16 ? (uint32)-10 : 10;
        w_u16(packet + 10, value);
        w_u8(packet + 12, 28);
        w_u16(packet + 16, 74);
        tag = r_u32(packet);
        w_u8(packet + 13, 0);
        w_u16(packet + 18, 9);
        head = r_u32(ot);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        buffers = r_u32(0x800BCFC4);
        w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
        value = r_u32(buffers + 8);
        w_u32(buffers + 8, value + 1);
    }
    buffers = r_u32(0x800BCFC4);
    count = r_u32(buffers + 12);
    draw = r_u32(0x800BCFC8) + (count << 3) + 0x1A8;
    w_u32(draw + 4, 0xE1000636);
    w_u8(draw + 3, 1);
    tag = r_u32(draw);
    head = r_u32(ot);
    w_u32(draw, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    buffers = r_u32(0x800BCFC4);
    w_u32(ot, (head & 0xFF000000) | (draw & 0xFFFFFF));
    value = r_u32(buffers + 12);
    w_u32(buffers + 12, value + 1);
    color = r_u8(record + 1);
    packet += 20;
    if (color == 0)
    {
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
    }
    else if (color == 1)
    {
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 64);
        w_u8(packet + 6, 0);
    }
    else if (color == 2)
    {
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 128);
    }
    else
    {
        w_u8(packet + 4, 255);
        w_u8(packet + 5, 255);
        w_u8(packet + 6, 255);
    }
    draw += 8;
    w_u16(packet + 14, 0x7F9B);
    value = r_u16(record + 4);
    w_u16(packet + 8, value);
    y = r_u16(record + 6);
    w_u8(packet + 12, 0);
    w_u8(packet + 13, 10);
    buffers = r_u32(0x800BCFC4);
    w_u16(packet + 16, 132);
    w_u16(packet + 18, 9);
    w_u16(packet + 10, y);
    tag = r_u32(packet);
    head = r_u32(ot);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
    value = r_u32(buffers + 8);
    w_u32(buffers + 8, value + 1);
    w_u8(draw + 3, 1);
    tag = r_u32(draw);
    w_u32(draw + 4, 0xE1000616);
    head = r_u32(ot);
    w_u32(draw, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    buffers = r_u32(0x800BCFC4);
    w_u32(ot, (head & 0xFF000000) | (draw & 0xFFFFFF));
    value = r_u32(buffers + 12);
    w_u32(buffers + 12, value + 1);
    return r_u8(state);
}

uint32 sub_8004c858(void)
{
    uint32 data;
    FUNCTION_MARKER(0x8004C858, "ACE3.BIN");
    data = r_u32(0x800BCFEC);
    return r_u32(data + 284);
}

uint32 sub_8003a2b8(void)
{
    uint32 scene, data, value, mode, numerator, divisor, scale, ot, packet, limit;
    FUNCTION_MARKER(0x8003A2B8, "ACE3.BIN");
    sub_8003b34c();
    sub_8003ef40();
    sub_80041698();
    sub_800419c0();
    scene = r_u32(0x800BCE54);
    if (r_u32(scene) & 1)
        sub_80042524();
    data = r_u32(0x800BCEB4);
    value = r_u8(data + 48);
    if (value != 1)
        return 1;
    value = sub_8005650c();
    if (value != 0)
        return value;
    data = r_u32(0x800BCEB4);
    mode = r_u8(data + 49);
    if (mode == 1 || mode == 2)
    {
        if (mode == 1)
        {
            divisor = r_s16(data + 50);
            value = r_s16(data + 26);
            numerator = (divisor - value) * 1365;
        }
        else
        {
            value = r_s16(data + 26);
            divisor = r_s16(data + 50);
            numerator = value << 12;
        }
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
        scene = r_u32(0x800BCE54);
        ot = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        value = sub_8008fc9c(ot, packet, scale);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, value);
    }
    data = r_u32(0x800BCEB4);
    value = r_u16(data + 26);
    limit = r_s16(data + 50);
    value += 1;
    w_u16(data + 26, value);
    value = (sint16)value < (sint32)limit;
    if (value == 0)
        w_u8(data + 48, 0);
    return value;
}

uint32 sub_800337bc(void)
{
    FUNCTION_MARKER(0x800337BC, "ACE3.BIN");
    return sub_8004c998();
}

uint32 sub_800337dc(void)
{
    uint32 scene, ot, packet, result;
    FUNCTION_MARKER(0x800337DC, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    ot = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    result = sub_8002d3f4(ot, packet);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    return result;
}

uint32 sub_8004c824(void)
{
    uint32 buffers, data, value;
    FUNCTION_MARKER(0x8004C824, "ACE3.BIN");
    buffers = r_u32(0x800BCFEC);
    data = r_u32(0x800BCE60);
    w_u32(buffers + 268, 0);
    w_u32(buffers + 264, 0);
    value = r_u32(data + 4);
    value = r_u32(buffers + (value << 2));
    w_u32(0x800BCFE8, value);
    return value;
}

uint32 sub_8004c998(void)
{
    uint32 data, flags;
    FUNCTION_MARKER(0x8004C998, "ACE3.BIN");
    data = r_u32(0x800BCE54);
    flags = r_u32(data + 4);
    if ((flags & 0x2C0E) == 0)
    {
        data = r_u32(0x800BCFEC);
        if (r_u32(data + 284) == 255)
            return 255;
        return sub_8004ce9c();
    }
    sub_8004c824();
    sub_8004ca28();
    data = r_u32(0x800BCE60);
    flags = (~r_u32(data)) & 0x20000000;
    if (flags == 0)
        return 0;
    return sub_8004c86c();
}

uint32 sub_8004c86c(void)
{
    uint32 buffers, scene, count, ot, index, packet, tag, head, value;
    FUNCTION_MARKER(0x8004C86C, "ACE3.BIN");
    buffers = r_u32(0x800BCFEC);
    scene = r_u32(0x800BCE54);
    count = r_u32(buffers + 264);
    ot = r_u32(scene + 16);
    index = 0;
    if ((sint32)count > 0)
    {
        packet = r_u32(0x800BCFE8) + 640;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ot);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ot);
            w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(buffers + 264);
            value = (sint32)index < (sint32)count;
            packet += 40;
        } while (value);
    }
    buffers = r_u32(0x800BCFEC);
    count = r_u32(buffers + 268);
    index = 0;
    if ((sint32)count > 0)
    {
        packet = r_u32(0x800BCFE8);
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ot);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ot);
            w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(buffers + 268);
            value = (sint32)index < (sint32)count;
            packet += 20;
        } while (value);
    }
    packet = r_u32(0x800BCFE8);
    head = r_u32(ot);
    tag = r_u32(packet + 1920);
    w_u32(packet + 1920, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    value = (head & 0xFF000000) | ((packet + 1920) & 0xFFFFFF);
    w_u32(ot, value);
    return value;
}

uint32 sub_8004ca28(void)
{
    uint32 buffers, clut, count, index, active, offset, record, selector, mode, texture, packet;
    uint32 value, other, opcode, x, y, width, height, right, bottom, shift, small_width, small_height, data, root;
    FUNCTION_MARKER(0x8004CA28, "ACE3.BIN");
    index = 0;
    active = 0;
    buffers = r_u32(0x800BCFEC);
    value = r_u16(buffers + 280);
    count = r_u32(buffers + 272);
    clut = value + 0x7F58;
    offset = 8;
    if ((sint32)count > 0)
        do
        {
            record = buffers + offset;
            selector = r_u8(record + 1);
            mode = r_u8(record);
            texture = 0x800B1914 + (selector << 2);
            if (mode == 1)
            {
                count = r_u32(buffers + 268);
                packet = r_u32(0x800BCFE8) + count * 20;
                opcode = r_u8(packet + 7);
                w_u8(packet + 4, 128);
                w_u8(packet + 5, 128);
                w_u8(packet + 6, 128);
                w_u8(packet + 7, opcode | 2);
                value = r_u16(record + 4);
                w_u16(packet + 8, value);
                value = r_u16(record + 6);
                w_u16(packet + 10, value);
                value = r_u8(texture);
                w_u8(packet + 12, value);
                value = r_u8(texture + 1);
                w_u16(packet + 14, clut);
                w_u8(packet + 13, value);
                value = r_u8(texture + 2);
                root = r_u32(0x800BCFEC);
                w_u16(packet + 16, value);
                value = r_u8(texture + 3);
                w_u16(packet + 18, value);
                value = r_u32(root + 268);
                data = r_u32(0x800BCE60);
                w_u32(root + 268, value + 1);
                if ((r_u32(data) & 0x20000000) == 0)
                {
                    value = r_u16(record + 2);
                    w_u16(record + 2, value + 1);
                }
                root = r_u32(0x800BCFEC);
                value = r_s16(record + 2);
                other = r_u32(root + 276);
                if ((sint32)value >= (sint32)other)
                {
                    w_u8(record, 3);
                    w_u16(record + 2, 0);
                }
            }
            else if (mode == 2 || mode == 3)
            {
                count = r_u32(buffers + 264);
                if (mode == 2)
                {
                    x = r_s16(record + 4);
                    y = r_s16(record + 6);
                    width = r_u8(texture + 2);
                    height = r_u8(texture + 3);
                    packet = r_u32(0x800BCFE8) + count * 40 + 640;
                    opcode = r_u8(packet + 7);
                    right = x + width;
                    w_u16(packet + 10, y);
                    w_u16(packet + 18, y);
                    bottom = y + height;
                }
                else
                {
                    width = r_u8(texture + 2);
                    shift = r_s16(record + 2);
                    height = r_u8(texture + 3);
                    x = r_s16(record + 4);
                    y = r_s16(record + 6);
                    small_width = (sint32)width >> (shift & 31);
                    x += (sint32)(width - small_width) >> 1;
                    small_height = (sint32)height >> (shift & 31);
                    y += (sint32)(height - small_height) >> 1;
                    packet = r_u32(0x800BCFE8) + count * 40 + 640;
                    opcode = r_u8(packet + 7);
                    right = x + small_width;
                    w_u16(packet + 10, y);
                    w_u16(packet + 18, y);
                    bottom = y + small_height;
                    w_u8(packet + 4, 128);
                    w_u8(packet + 5, 128);
                    w_u8(packet + 6, 128);
                }
                w_u16(packet + 8, x);
                w_u16(packet + 16, right);
                w_u16(packet + 24, x);
                w_u16(packet + 26, bottom);
                w_u16(packet + 32, right);
                w_u16(packet + 34, bottom);
                w_u8(packet + 7, opcode | 2);
                value = r_u8(texture);
                w_u8(packet + 12, value);
                value = r_u8(texture + 1);
                w_u8(packet + 13, value);
                value = r_u8(texture);
                other = r_u8(texture + 2);
                w_u8(packet + 20, value + other);
                value = r_u8(texture + 1);
                w_u8(packet + 21, value);
                value = r_u8(texture);
                w_u8(packet + 28, value);
                value = r_u8(texture + 1);
                other = r_u8(texture + 3);
                w_u8(packet + 29, value + other);
                value = r_u8(texture);
                other = r_u8(texture + 2);
                w_u8(packet + 36, value + other);
                value = r_u8(texture + 1);
                other = r_u8(texture + 3);
                if (mode == 3)
                    w_u16(packet + 14, clut);
                w_u8(packet + 37, value + other);
                if (mode == 2)
                {
                    w_u16(packet + 14, clut);
                    w_u16(packet + 22, 86);
                    value = (uint32)r_s16(record + 2) << 5;
                    w_u8(packet + 4, value);
                    w_u8(packet + 5, value);
                    w_u8(packet + 6, value);
                    root = r_u32(0x800BCFEC);
                }
                else
                {
                    root = r_u32(0x800BCFEC);
                    w_u16(packet + 22, 86);
                }
                value = r_u32(root + 264);
                data = r_u32(0x800BCE60);
                w_u32(root + 264, value + 1);
                if ((r_u32(data) & 0x20000000) == 0)
                {
                    value = r_u16(record + 2);
                    w_u16(record + 2, value + 1);
                }
                if (mode == 2)
                {
                    if (r_s16(record + 2) >= 8)
                    {
                        w_u8(record, 1);
                        w_u16(record + 2, 0);
                    }
                }
                else if (small_width == 0 && small_height == 0)
                {
                    w_u8(record, 0);
                    w_u16(record + 2, 0);
                }
            }
            if (mode != 0 && r_u8(record) != 0)
                ++active;
            buffers = r_u32(0x800BCFEC);
            count = r_u32(buffers + 272);
            ++index;
            offset += 8;
        } while ((sint32)index < (sint32)count);
    if (active == 0)
    {
        root = r_u32(0x800BCFEC);
        w_u32(root + 272, 0);
        w_u32(root + 284, 255);
    }
    return 255;
}

uint32 sub_8003a12c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 value, raw, mode, result;
    FUNCTION_MARKER(0x8003A12C, "ACE3.BIN");
    if ((native_state ? native_state->words[0] : r_u32(state)) & 7)
        w_u16(0x800C6D66, 15);
    if (r_u32(0x800C6ECC) & 2)
        w_u8(0x800C6D6F, 1);
    else
    {
        value = r_s16(0x800C6D66);
        raw = r_u16(0x800C6D66);
        if ((sint32)value > 0)
        {
            w_u8(0x800C6D6F, 2);
            w_u16(0x800C6D66, raw - 1);
        }
        else
            w_u8(0x800C6D6F, 0);
    }
    mode = r_u8(0x800C6D6F);
    if (mode == 1)
    {
        w_u8(0x800C6D6B, 131);
        result = 255;
        goto red;
    }
    if (mode == 2)
    {
        value = r_u32(0x800C6D60);
        if ((sint32)value % 6 < 4)
        {
            w_u8(0x800C6D6B, 67);
            result = 131;
            goto red;
        }
    }
    w_u8(0x800C6D6C, 131);
    w_u8(0x800C6D6B, 0);
    w_u8(0x800C6D6D, 0);
    w_u8(0x800C6D68, 0);
    w_u8(0x800C6D69, 255);
    w_u8(0x800C6D6A, 0);
    w_u8(0x800C6D71, 0);
    return 255;
red:
    w_u8(0x800C6D6C, 0);
    w_u8(0x800C6D6D, 0);
    w_u8(0x800C6D68, result);
    w_u8(0x800C6D69, 0);
    w_u8(0x800C6D6A, 0);
    w_u8(0x800C6D71, 1);
    return result;
}

uint32 sub_8003c650(void)
{
    uint32 buffers, data, value;
    FUNCTION_MARKER(0x8003C650, "ACE3.BIN");
    buffers = r_u32(0x800BCFA8);
    data = r_u32(0x800BCE60);
    w_u32(buffers + 20, 0);
    w_u32(buffers + 16, 0);
    w_u32(buffers + 12, 0);
    w_u32(buffers + 8, 0);
    w_u32(buffers + 24, 0);
    value = r_u32(data + 4);
    value = r_u32(buffers + (value << 2));
    w_u32(0x800BCFAC, value);
    return value;
}

uint32 sub_8003d08c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 data, value;
    FUNCTION_MARKER(0x8003D08C, "ACE3.BIN");
    data = (native_state ? native_state->words[1] : r_u32(state + 4));
    value = r_s32(data);
    w_u32(0x1F800100, (sint32)value >> 12);
    data = (native_state ? native_state->words[1] : r_u32(state + 4));
    value = r_s32(data + 8);
    w_u32(0x1F800104, (sint32)value >> 12);
    data = (native_state ? native_state->words[2] : r_u32(state + 8));
    value = r_u16(data + 2) & 0x3FFF;
    value = r_u16(0x800104AC + (value << 1));
    w_u32(0x1F800108, (sint32)(value << 16) >> 18);
    data = (native_state ? native_state->words[2] : r_u32(state + 8));
    value = (r_u16(data + 2) & 0x3FFF) + 0x1000;
    value = r_u16(0x800104AC + (value << 1));
    w_u32(0x1F80010C, (sint32)(value << 16) >> 18);
    data = (native_state ? native_state->words[2] : r_u32(state + 8));
    value = r_s16(data + 2);
    w_u32(0x800BCFB0, 0x1F800100);
    w_u32(0x1F800110, value);
    return value;
}

uint32 sub_8003d138(void)
{
    uint32 index, table, flags, object, reference, value, other, dx, dz, magnitude, ax, az;
    FUNCTION_MARKER(0x8003D138, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + index * 4);
        object = r_u32(table + index * 4);
        if ((flags & 0x2000) == 0 || (r_u32(object) & 0x40))
        {
            w_u32(0x800C6A60 + index * 12 + 8, 0xFFFFFFFF);
            continue;
        }
        reference = r_u32(0x800BCFB0);
        value = r_u32(object + 8);
        other = r_u32(reference);
        dx = ((sint32)value >> 12) - other;
        value = r_u32(object + 16);
        other = r_u32(reference + 4);
        dz = other - ((sint32)value >> 12);
        ax = (sint32)dx < 0 ? 0 - dx : dx;
        az = (sint32)dz < 0 ? 0 - dz : dz;
        if ((sint32)ax < 0x4000 && (sint32)az < 0x4000)
            magnitude = SquareRoot0PSX(dx * dx + dz * dz, 0x800B6DD4);
        else
        {
            dx = (sint32)dx >> 4;
            dz = (sint32)dz >> 4;
            magnitude = SquareRoot0PSX(dx * dx + dz * dz, 0x800B6DD4) << 4;
        }
        w_u32(0x800C6A60 + index * 12 + 8, magnitude);
    }
    return 0;
}

uint32 sub_8003d280(void)
{
    uint32 data, flags, selected, distance, value, index, old, next;
    FUNCTION_MARKER(0x8003D280, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    flags = r_u32(data + 296);
    selected = r_u8(0x800C6D6E);
    distance = 0x40000;
    if (flags & 0x8000)
    {
        if (selected != 255 && (r_u32(0x800C6FD0 + selected * 4) & 0x100))
        {
            value = r_u32(0x800C6A60 + selected * 12 + 8);
            if (value != 0xFFFFFFFF)
            {
                distance = value;
                goto scaled;
            }
        }
        for (index = 2; index < 64; ++index)
        {
            if ((r_u32(0x800C6FD0 + index * 4) & 0x100) == 0)
                continue;
            value = r_u32(0x800C6A60 + index * 12 + 8);
            if (value != 0xFFFFFFFF && (sint32)value < (sint32)distance)
                distance = value;
        }
    scaled:
        distance += (sint32)distance >> 2;
    }
    else
    {
        if (selected != 255)
        {
            value = r_u32(0x800C6A60 + selected * 12 + 8);
            if (value != 0xFFFFFFFF)
                distance = value;
        }
        else
        {
            for (index = 2; index < 32; ++index)
            {
                value = r_u32(0x800C6A60 + index * 12 + 8);
                if (value != 0xFFFFFFFF && (sint32)value < (sint32)distance)
                    distance = value;
            }
        }
        if ((sint32)distance < 0x3000)
            distance = 0x4000;
        else if ((sint32)distance < 0x6000)
            distance = 0x8000;
        else
            distance = 0x10000;
    }
    old = r_u32(0x800C6D7C);
    next = (sint32)(old + distance) >> 1;
    if (next & 1)
    {
        if ((sint32)old < (sint32)next)
            ++next;
        else
            --next;
    }
    if ((sint32)next < 0x2000)
        next = 0x2000;
    if ((sint32)next > 0x40000)
        next = 0x40000;
    w_u32(0x800C6D7C, next);
    return 0x800C0000;
}

uint32 sub_8003d458(void)
{
    uint32 buffers, packet, count, value, blue, scale;
    FUNCTION_MARKER(0x8003D458, "ACE3.BIN");
    buffers = r_u32(0x800BCFA8);
    packet = r_u32(0x800BCFAC);
    count = r_u32(buffers + 16);
    packet += (count << 4) + 0x918;
    value = r_u8(0x800C6D68);
    w_u8(packet + 4, value >> 1);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value >> 1);
    blue = r_u8(0x800C6D6A);
    w_u16(packet + 8, 0);
    w_u16(packet + 10, 45);
    w_u16(packet + 14, 45);
    w_u8(packet + 6, blue >> 1);
    buffers = r_u32(0x800BCFA8);
    w_u16(packet + 12, 5);
    count = r_u32(buffers + 16);
    w_u32(buffers + 16, count + 1);
    value = r_u8(0x800C6D68);
    packet += 16;
    w_u8(packet + 4, value >> 1);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value >> 1);
    blue = r_u8(0x800C6D6A);
    w_u16(packet + 8, 86);
    w_u8(packet + 6, blue >> 1);
    buffers = r_u32(0x800BCFA8);
    w_u16(packet + 10, 14);
    w_u16(packet + 12, 320);
    w_u16(packet + 14, 14);
    count = r_u32(buffers + 16);
    w_u32(buffers + 16, count + 1);
    packet = r_u32(0x800BCFAC);
    value = r_u8(0x800C6D6B);
    packet += 0xC0C;
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6D);
    w_u8(packet + 6, value);
    packet = r_u32(0x800BCFAC);
    value = r_u8(0x800C6D6B);
    packet += 0xC20;
    w_u8(packet + 4, value >> 1);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value >> 1);
    value = r_u8(0x800C6D6D);
    w_u8(packet + 6, value >> 1);
    scale = r_u32(0x800C6D7C);
    if ((sint32)scale <= 0x4000)
    {
        w_u8(packet + 12, 24);
        return 24;
    }
    if ((sint32)scale <= 0x8000)
    {
        w_u8(packet + 12, 12);
        return 12;
    }
    w_u8(packet + 12, 0);
    return 1;
}

uint32 sub_8003d9c8(void)
{
    FUNCTION_MARKER(0x8003D9C8, "ACE3.BIN");
    sub_8003da08();
    if (r_s32(0x800C6D7C) > 0x8000)
        return 1;
    return sub_8003dcf0();
}

uint32 sub_8003dcf0(void)
{
    uint32 scale, index, record, root, flags, object, reference, value, base, cosine, sine, dx, dz, numerator, x, y;
    FUNCTION_MARKER(0x8003DCF0, "ACE3.BIN");
    scale = r_u32(0x800C6D7C);
    for (index = 0; index < 32; ++index)
    {
        record = 0x800C68E0 + index * 12;
        w_u8(record, 0);
        root = r_u32(0x800BD080);
        flags = r_u32(root + 0x1548);
        if ((flags & (1u << index)) == 0)
            continue;
        object = root + 200 + index * 164;
        value = r_u32(object + 28);
        reference = r_u32(0x800BCFB0);
        base = r_u32(reference);
        cosine = r_u32(reference + 12);
        dx = ((sint32)value >> 12) - base;
        value = r_u32(object + 36);
        base = r_u32(reference + 4);
        sine = r_u32(reference + 8);
        dz = base - ((sint32)value >> 12);
        numerator = dx * cosine + dz * sine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        x = (sint32)numerator / (sint32)scale;
        numerator = (0 - dx) * sine + dz * cosine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        y = (sint32)numerator / (sint32)scale;
        if (x + 31 >= 63 || y + 31 >= 63)
            continue;
        w_u8(record + 7, y + 46);
        value = r_u8(record);
        w_u8(record + 6, x + 44);
        w_u8(record + 1, 2);
        w_u8(record + 2, 0);
        w_u8(record, value | 1);
    }
    return 0;
}

uint32 sub_8003da08(void)
{
    uint32 scale, table, player, index, record, value, object, flags, reference, base, cosine, sine;
    uint32 dx, dz, numerator, x, y, old, height, target, intensity;
    FUNCTION_MARKER(0x8003DA08, "ACE3.BIN");
    scale = r_u32(0x800C6D7C);
    table = r_u32(0x800BCE5C);
    player = r_u32(table);
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        record = 0x800C6A60 + index * 12;
        value = r_u8(record);
        object = r_u32(table + index * 4);
        w_u8(record, value & 0xF0);
        if ((r_u32(object) & 0x4000) == 0)
            continue;
        flags = r_u32(0x800C6FD0 + index * 4);
        if ((flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        if ((sint32)scale < r_s32(record + 8))
            continue;
        value = r_u32(object + 8);
        reference = r_u32(0x800BCFB0);
        base = r_u32(reference);
        cosine = r_u32(reference + 12);
        dx = ((sint32)value >> 12) - base;
        value = r_u32(object + 16);
        base = r_u32(reference + 4);
        sine = r_u32(reference + 8);
        dz = base - ((sint32)value >> 12);
        numerator = dx * cosine + dz * sine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        x = (sint32)numerator / (sint32)scale;
        numerator = (0 - dx) * sine + dz * cosine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        y = (sint32)numerator / (sint32)scale;
        if (x + 31 >= 63 || y + 31 >= 63)
            continue;
        old = r_u8(record);
        w_u8(record + 6, x + 44);
        w_u8(record + 7, y + 46);
        w_u8(record, old | 1);
        if ((sint32)scale > 0x7FFF)
            w_u8(record, old | 9);
        if (r_u8(0x800C6D6E) == index)
        {
            value = r_u8(record);
            w_u8(record, value | 4);
        }
        value = r_u32(object + 12);
        base = r_u32(player + 12);
        height = (sint32)(base - value) >> 12;
        if ((sint32)height > 4096)
            w_u8(record + 5, 2);
        else if ((sint32)height < -4096)
            w_u8(record + 5, 0);
        else
            w_u8(record + 5, 1);
        value = r_s16(object + 26);
        base = r_s16(player + 26);
        value = ((value - base + 512) & 0x3FFF) >> 10;
        flags = r_u8(record);
        w_u8(record + 4, value);
        if ((flags & 0x10) == 0)
            continue;
        target = r_u32(0x800C6ED0 + index * 4);
        if (target == 0)
            continue;
        intensity = 131;
        if ((r_u32(target) & 2) == 0 && (r_u32(target + 32) & 0x20000) == 0)
        {
            value = r_u32(target + 4);
            intensity = 131 - ((sint32)(value - 0x8000) >> 6);
            if ((sint32)intensity >= 132)
                intensity = 131;
            if ((sint32)intensity < 19)
                intensity = 19;
        }
        w_u8(record + 3, intensity);
    }
    return 0;
}

uint32 sub_8003dff8(uint32 state)
{
    uint32 buffers, count, packet, value, mode, size, flags, intensity, color, width, height, x, y, scene;
    FUNCTION_MARKER(0x8003DFF8, "ACE3.BIN");
    if ((r_u8(state) & 1) == 0)
        return 0;
    buffers = r_u32(0x800BCFA8);
    count = r_u32(buffers + 8);
    packet = r_u32(0x800BCFAC) + count * 20 + 24;
    w_u16(packet + 14, 0x7F98);
    if (r_u8(state) & 8)
    {
        w_u8(packet + 12, 98);
        w_u8(packet + 13, 6);
        value = r_u8(state + 2);
        size = value == 1 ? 3 : 2;
        w_u16(packet + 16, size);
        w_u16(packet + 18, size);
    }
    else
    {
        mode = r_u8(state + 1);
        if (mode == 0)
        {
            mode = r_u8(state + 5);
            if (mode == 0)
            {
                value = r_u8(state + 4);
                w_u8(packet + 13, 51);
                w_u16(packet + 16, 4);
                w_u16(packet + 18, 4);
                w_u8(packet + 12, (value << 2) + 62);
            }
            else if (mode == 1)
            {
                value = r_u8(state + 4);
                w_u8(packet + 13, 11);
                w_u16(packet + 16, 6);
                w_u16(packet + 18, 6);
                w_u8(packet + 12, value * 6);
            }
            else if (mode == 2)
            {
                value = r_u8(state + 4);
                w_u8(packet + 13, 17);
                w_u16(packet + 16, 8);
                w_u16(packet + 18, 8);
                w_u8(packet + 12, value << 3);
            }
        }
        else if (mode == 1)
        {
            value = r_u8(state + 5);
            w_u8(packet + 12, 98);
            w_u8(packet + 13, 6);
            size = value + 2;
            w_u16(packet + 16, size);
            w_u16(packet + 18, size);
        }
        else if (mode == 2)
        {
            w_u8(packet + 12, 98);
            w_u8(packet + 13, 6);
            w_u16(packet + 16, 1);
            w_u16(packet + 18, 1);
        }
    }
    flags = r_u8(state);
    intensity = 131;
    if (flags & 0x10)
        intensity = r_u8(state + 3);
    if (flags & 4)
    {
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) & 2)
            intensity >>= 3;
    }
    color = r_u8(state + 2);
    if (color == 0)
    {
        value = intensity >> 1;
        w_u8(packet + 4, value);
        w_u8(packet + 5, value);
        w_u8(packet + 6, value);
    }
    else if (color == 1)
    {
        w_u8(packet + 4, intensity);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
    }
    else if (color == 2)
    {
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, intensity);
    }
    else if (color == 3)
    {
        w_u8(packet + 4, intensity);
        w_u8(packet + 5, intensity);
        w_u8(packet + 6, 0);
    }
    width = r_u16(packet + 16);
    x = r_u8(state + 6);
    height = r_u16(packet + 18);
    width = (sint32)(width << 16) >> 17;
    height = (sint32)(height << 16) >> 17;
    w_u16(packet + 8, x - width);
    y = r_u8(state + 7);
    buffers = r_u32(0x800BCFA8);
    w_u16(packet + 10, y - height);
    count = r_u32(buffers + 8);
    w_u32(buffers + 8, count + 1);
    return 0;
}

uint32 sub_8003f018(void)
{
    uint32 buffers, data, value;
    FUNCTION_MARKER(0x8003F018, "ACE3.BIN");
    buffers = r_u32(0x800BCFB8);
    data = r_u32(0x800BCE60);
    w_u32(buffers + 20, 0);
    w_u32(buffers + 16, 0);
    w_u32(buffers + 12, 0);
    w_u32(buffers + 8, 0);
    value = r_u32(data + 4);
    value = r_u32(buffers + (value << 2));
    w_u32(0x800BCFBC, value);
    return value;
}

uint32 sub_8003f054(void)
{
    static const uint32 starts[4] = {8, 0x408, 0x1008, 0x1508};
    static const uint32 strides[4] = {16, 24, 20, 20};
    uint32 buffers, scene, count, ot, group, index, packet, tag, head, value;
    FUNCTION_MARKER(0x8003F054, "ACE3.BIN");
    buffers = r_u32(0x800BCFB8);
    scene = r_u32(0x800BCE54);
    count = r_u32(buffers + 8);
    ot = r_u32(scene + 16);
    for (group = 0; group < 4; ++group)
    {
        if (group == 2)
            buffers = r_u32(0x800BCFB8);
        if (group != 0)
            count = r_u32(buffers + 8 + group * 4);
        if ((sint32)count > 0)
        {
            index = 0;
            packet = r_u32(0x800BCFBC) + starts[group];
            do
            {
                ++index;
                tag = r_u32(packet);
                head = r_u32(ot);
                w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
                head = r_u32(ot);
                w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
                count = r_u32(buffers + 8 + group * 4);
                value = (sint32)index < (sint32)count;
                packet += strides[group];
            } while (value);
            if (group == 0 || group == 2)
                buffers = r_u32(0x800BCFB8);
        }
    }
    packet = r_u32(0x800BCFBC);
    head = r_u32(ot);
    tag = r_u32(packet);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    value = (head & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ot, value);
    return value;
}

uint32 sub_80040e70(void)
{
    uint32 size, x, y, selected, radius, inner, buffers, count, packet, i, value;
    uint32 left, right, near_left, near_right, top, bottom, near_top, near_bottom;
    FUNCTION_MARKER(0x80040E70, "ACE3.BIN");
    if (r_u8(0x800BCEC8) == 0)
        return 0;
    size = r_u16(0x800BCECA);
    x = r_s16(0x800BCECC);
    y = r_s16(0x800BCECE);
    selected = r_u8(0x800BCEC9);
    radius = (sint16)size;
    if (selected < 32)
    {
        radius += (sint32)(size << 16) >> 19;
        top = y - radius;
        bottom = y + radius;
        buffers = r_u32(0x800BCFB8);
        count = r_u32(buffers + 12);
        packet = r_u32(0x800BCFBC) + count * 24 + 0x408;
        for (i = 0; i < 2; ++i)
        {
            w_u8(packet + 4, 255);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
            buffers = r_u32(0x800BCFB8);
            value = i == 0 ? x + radius : x - radius;
            w_u16(packet + 8, x);
            w_u16(packet + 10, top);
            w_u16(packet + 12, value);
            w_u16(packet + 14, y);
            w_u16(packet + 16, x);
            w_u16(packet + 18, bottom);
            count = r_u32(buffers + 12) + 1;
            packet += 24;
            w_u32(buffers + 12, count);
        }
        return count;
    }
    inner = (sint32)(size << 16) >> 18;
    top = y - radius;
    bottom = y + radius;
    near_top = y - inner;
    near_bottom = y + inner;
    left = x - radius;
    right = x + radius;
    near_left = x - inner;
    near_right = x + inner;
    buffers = r_u32(0x800BCFB8);
    count = r_u32(buffers + 8);
    packet = r_u32(0x800BCFBC) + (count << 4) + 8;
    for (i = 0; i < 8; ++i)
    {
        w_u8(packet + 4, 255);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
        if (i != 6)
            buffers = r_u32(0x800BCFB8);
        if (i == 0 || i == 1)
        {
            w_u16(packet + 12, x);
            w_u16(packet + 8, x);
            w_u16(packet + 10, i == 0 ? top : bottom);
            w_u16(packet + 14, i == 0 ? near_top : near_bottom);
        }
        else if (i == 2 || i == 3)
        {
            w_u16(packet + 8, i == 2 ? left : right);
            w_u16(packet + 12, i == 2 ? near_left : near_right);
            w_u16(packet + 14, y);
            w_u16(packet + 10, y);
        }
        else if (i == 4 || i == 5)
        {
            w_u16(packet + 8, near_left);
            w_u16(packet + 12, near_right);
            w_u16(packet + 14, i == 4 ? top : bottom);
            w_u16(packet + 10, i == 4 ? top : bottom);
        }
        else
        {
            value = i == 6 ? left : right;
            w_u16(packet + 12, value);
            w_u16(packet + 8, value);
            if (i == 6)
                buffers = r_u32(0x800BCFB8);
            w_u16(packet + 10, near_top);
            w_u16(packet + 14, near_bottom);
        }
        count = r_u32(buffers + 8) + 1;
        packet += 16;
        w_u32(buffers + 8, count);
    }
    return count;
}

uint32 sub_8003e274(uint32 state, const uint32 *native_state)
{
    uint32 reference, value, other, dx, dz, ax, az, magnitude, a, b, scale, cosine, sine, numerator, x, y;
    uint32 i, angle, shift, packet, heading, scene, buffers, camera, distance, matrix[5], packed_xy, flag, size, phase, cursor;
    uint16 vector[3];
    FUNCTION_MARKER(0x8003E274, "ACE3.BIN");
    reference = r_u32(0x800BCFB0);
    value = native_state ? native_state[0] : r_u32(state);
    other = r_u32(reference);
    dx = ((sint32)value >> 12) - other;
    value = native_state ? native_state[2] : r_u32(state + 8);
    other = r_u32(reference + 4);
    dz = other - ((sint32)value >> 12);
    ax = (sint32)dx < 0 ? 0 - dx : dx;
    az = (sint32)dz < 0 ? 0 - dz : dz;
    if ((sint32)ax < 0x4000 && (sint32)az < 0x4000)
        magnitude = SquareRoot0PSX(dx * dx + dz * dz, 0x800B6DD4);
    else
    {
        a = (sint32)dx >> 4;
        b = (sint32)dz >> 4;
        magnitude = SquareRoot0PSX(a * a + b * b, 0x800B6DD4) << 4;
    }
    scale = r_u32(0x800C6D7C);
    if ((sint32)scale < (sint32)magnitude)
        return 0;
    reference = r_u32(0x800BCFB0);
    cosine = r_u32(reference + 12);
    sine = r_u32(reference + 8);
    numerator = dx * cosine + dz * sine;
    numerator = ((uint32)((sint32)numerator >> 12)) << 5;
    if (scale == 0)
        xport_mips_break(7);
    if (scale == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    x = (sint32)numerator / (sint32)scale;
    numerator = (0 - dx) * sine + dz * cosine;
    numerator = ((uint32)((sint32)numerator >> 12)) << 5;
    if (scale == 0)
        xport_mips_break(7);
    if (scale == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    y = (sint32)numerator / (sint32)scale;
    if (x + 31 >= 63 || y + 31 >= 63)
        return 0;
    x += 44;
    y += 46;
    for (i = 0; i < 2; ++i)
    {
        angle = r_u16(0x800BCC68 + i * 2);
        packet = r_u32(0x800BCFAC);
        reference = r_u32(0x800BCFB0);
        heading = native_state ? native_state[4] >> 16 : r_u16(state + 18);
        shift = r_u8(0x800BCC6C + i);
        packet += 0xC54 + i * 16;
        value = r_u16(reference + 16);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
        w_u16(packet + 8, x);
        w_u16(packet + 10, y);
        angle = (angle + value - heading) & 0x3FFF;
        a = r_s16(0x800104AC + (angle << 1));
        w_u8(packet + 4, 255);
        b = r_s16(0x800104AC + ((angle + 0x1000) << 1));
        a = (sint32)a >> (shift & 31);
        b = (sint32)b >> (shift & 31);
        w_u16(packet + 14, y + b);
        w_u16(packet + 12, x + a);
    }
    buffers = r_u32(0x800BCFA8);
    scene = r_u32(0x800BCE54);
    w_u32(buffers + 24, 1);
    camera = r_u32(scene + 40);
    distance = sub_80083dfc(state, camera + 4, native_state, 0);
    if ((sint32)distance > 0x7FFF)
        return 0;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = native_state ? native_state[0] : r_u32(state);
    other = r_u32(camera + 4);
    vector[0] = (sint32)(value - other) >> 12;
    camera = r_u32(scene + 40);
    value = native_state ? native_state[1] : r_u32(state + 4);
    other = r_u32(camera + 8);
    vector[1] = (sint32)(value - other) >> 12;
    camera = r_u32(scene + 40);
    value = native_state ? native_state[2] : r_u32(state + 8);
    other = r_u32(camera + 12);
    vector[2] = (sint32)(value - other) >> 12;
    for (i = 0; i < 5; ++i)
        matrix[i] = r_u32(scene + 76 + i * 4);
    for (i = 0; i < 5; ++i)
        xport_gte_write_control(i, matrix[i]);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    packed_xy = xport_gte_read_data(14);
    value = xport_gte_read_data(8);
    flag = xport_gte_read_control(31);
    value = xport_gte_read_data(19);
    if ((sint32)flag < 0)
        return 0;
    size = (sint32)(0x8000 - distance) >> 10;
    if ((sint32)size >= 9)
        size = 8;
    if ((sint32)size < 2)
        size = 2;
    packet = r_u32(0x800BCFAC);
    cursor = packet + 0xC82;
    for (i = 0; i < 3; ++i)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 12);
        w_u8(cursor - 8, 0);
        phase = (i + (((uint32)((sint32)value >> 2)) & 3)) << 6;
        w_u8(cursor - 10, phase);
        w_u8(cursor - 9, (phase & 0xC0) >> 1);
        w_u16(cursor - 6, packed_xy & 0xFFFF);
        value = (packed_xy >> 16) - (i * 2 + 1) * size;
        w_u16(cursor - 4, value);
        w_u16(cursor - 2, (packed_xy & 0xFFFF) + size);
        w_u16(cursor + 2, (packed_xy & 0xFFFF) - size);
        value = (packed_xy >> 16) - (i * 2 + 2) * size;
        w_u16(cursor + 4, value);
        w_u16(cursor, value);
        cursor += 20;
    }
    buffers = r_u32(0x800BCFA8);
    value = r_u32(buffers + 24);
    w_u32(buffers + 24, value | 2);
    return 0;
}

uint32 sub_8003de5c(void)
{
    uint32 pass, index, record, flags, data, mission, scene, local[6];
    FUNCTION_MARKER(0x8003DE5C, "ACE3.BIN");
    for (pass = 0; pass < 2; ++pass)
    {
        for (index = 2; index < 64; ++index)
        {
            record = 0x800C6A60 + index * 12;
            flags = r_u32(0x800C6FD0 + index * 4);
            if ((flags & 0x2000) && ((r_u8(record + 2) != 0) == (pass != 0)))
                sub_8003dff8(record);
        }
    }
    if (r_s32(0x800C6D7C) <= 0x8000)
        for (index = 0; index < 32; ++index)
            sub_8003dff8(0x800C68E0 + index * 12);
    data = r_u32(0x800BCE60);
    mission = r_s16(data + 908);
    if (mission != 18 && mission != 37)
        return 37;
    scene = r_u32(0x800BCE54);
    if (r_u32(scene + 8) != 1)
        return 1;
    local[0] = r_u32(0x80023DEC);
    local[1] = r_u32(0x80023DF0);
    local[2] = r_u32(0x80023DF4);
    local[3] = r_u32(0x80023DF8);
    local[4] = r_u32(0x80023DFC);
    local[5] = r_u32(0x80023E00);
    return sub_8003e274(0, local);
}

uint32 sub_8003b34c(void)
{
    uint32 scene, matrix[5], index, table, object, target, record, camera, value, other, flags;
    uint32 projection, depth, size, x, y, z, limit, state_flags, trigger, root;
    FUNCTION_MARKER(0x8003B34C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    for (index = 0; index < 5; ++index)
        matrix[index] = r_u32(scene + 76 + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, matrix[index]);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        target = r_u32(0x800C6ED0 + index * 4);
        value = r_u32(object);
        record = r_u32(0x800C67A0 + index * 4);
        if ((value & 0x4000) == 0 || target == 0 || record == 0)
            continue;
        scene = r_u32(0x800BCE54);
        w_u32(target + 32, 0);
        camera = r_u32(scene + 40);
        value = r_u32(object + 8);
        other = r_u32(camera + 4);
        x = (sint32)(value - other) >> 12;
        if (x + 0x8000 > 0xFFFF)
            continue;
        camera = r_u32(scene + 40);
        value = r_u32(object + 12);
        other = r_u32(camera + 8);
        y = (sint32)(value - other) >> 12;
        if (y + 0x8000 > 0xFFFF)
            continue;
        camera = r_u32(scene + 40);
        value = r_u32(object + 16);
        other = r_u32(camera + 12);
        z = (sint32)(value - other) >> 12;
        if (z + 0x8000 > 0xFFFF)
            continue;
        value = r_u32(target + 32);
        w_u32(target + 32, value | 0x10000);
        xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(1, z & 0xFFFF);
        xport_gte_execute(0x180001);
        value = xport_gte_read_data(14);
        w_u32(record + 8, value);
        projection = xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        depth = xport_gte_read_data(19);
        size = sub_8003b758((sint32)depth >> 2);
        w_u16(record + 6, size);
        if ((sint32)flags < 0)
            continue;
        value = r_u32(target + 32) | 0x40000;
        w_u32(target + 32, value);
        if (r_u16(record + 8) >= 321)
            continue;
        y = r_s16(record + 10);
        if ((sint32)y >= 241 || (sint32)y < 0)
            continue;
        w_u32(target + 32, value | 0x20000);
    }
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        value = r_u32(object);
        target = r_u32(0x800C6ED0 + index * 4);
        if ((value & 0x4000) == 0)
            continue;
        if ((r_u32(target + 32) & 0x20000) == 0)
        {
            w_u32(target + 36, 0);
            continue;
        }
        scene = r_u32(0x800BCE54);
        if ((r_u32(scene + 4) & 4) == 0)
        {
            w_u32(target + 36, 0);
            continue;
        }
        root = r_u32(0x800BCE60);
        if ((r_u32(root) & 0x20000000) == 0)
        {
            value = r_u32(target + 36);
            w_u32(target + 36, value + 1);
        }
        root = r_u32(0x800BCE60);
        limit = r_s16(root + 908) == 6 ? 0 : 90;
        state_flags = r_u32(0x800C6FD0 + index * 4);
        if ((state_flags & 0x1000000) == 0 && r_s32(target + 36) >= (sint32)limit)
        {
            w_u32(0x800C6FD0 + index * 4, state_flags | 0x1000000);
            if (index < 32)
            {
                root = r_u32(0x800BCEF8);
                trigger = r_u32(root + index * 64 + 56);
            }
            else
            {
                root = r_u32(0x800BCEFC);
                trigger = r_u32(root + 0xFFFFFB00 + index * 40 + 32);
            }
            if (trigger)
                sub_80054ebc(trigger);
        }
        scene = r_u32(0x800BCE54);
        if (r_s32(scene + 8) <= 0)
            continue;
        state_flags = r_u32(0x800C6FD0 + index * 4);
        if ((state_flags & 0x2000000) == 0 && r_s32(target + 36) >= (sint32)limit)
        {
            w_u32(0x800C6FD0 + index * 4, state_flags | 0x2000000);
            if (index < 32)
            {
                root = r_u32(0x800BCEF8);
                trigger = r_u32(root + index * 64 + 60);
            }
            else
            {
                root = r_u32(0x800BCEFC);
                trigger = r_u32(root + 0xFFFFFB00 + index * 40 + 36);
            }
            if (trigger)
                sub_80054ebc(trigger);
        }
    }
    return 0;
}

uint32 sub_80063f80(void)
{
    uint32 buffers, data, offset, value;
    FUNCTION_MARKER(0x80063F80, "ACE3.BIN");
    buffers = r_u32(0x800BD074);
    data = r_u32(0x800BCE60);
    for (offset = 36; offset >= 8; offset -= 4)
        w_u32(buffers + offset, 0);
    value = r_u32(data + 4);
    value = r_u32(buffers + (value << 2));
    w_u32(0x800BD078, value);
    return value;
}

uint32 sub_80064400(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 angles, value;
    FUNCTION_MARKER(0x80064400, "ACE3.BIN");
    angles = (native_state ? native_state->words[4] : r_u32(state + 16));
    value = (r_u16(angles) + 2) & 0xFFFF;
    if (value >= 5)
        return sub_8006466c();
    value = (r_u16(angles + 2) + 2) & 0xFFFF;
    if (value >= 5)
        return sub_8006466c();
    value = (r_u16(angles + 4) + 2) & 0xFFFF;
    if (value >= 5)
        return sub_8006466c();
    return 1;
}

uint32 sub_80064cb4(void)
{
    static const uint32 left[3] = {0, 260, 314};
    static const uint32 right[3] = {60, 290, 320};
    uint32 buffers, count, packet, red, green, blue, i;
    FUNCTION_MARKER(0x80064CB4, "ACE3.BIN");
    buffers = r_u32(0x800BD074);
    count = r_u32(buffers + 20);
    packet = r_u32(0x800BD078) + (count << 4) + 0x3D8;
    red = r_u8(0x800C6D68);
    green = r_u8(0x800C6D69);
    blue = r_u8(0x800C6D6A);
    red >>= 1;
    green >>= 1;
    blue >>= 1;
    for (i = 0; i < 3; ++i)
    {
        w_u8(packet + 4, red);
        w_u8(packet + 5, green);
        w_u8(packet + 6, blue);
        buffers = r_u32(0x800BD074);
        w_u16(packet + 8, left[i]);
        w_u16(packet + 12, right[i]);
        w_u16(packet + 14, 120);
        w_u16(packet + 10, 120);
        count = r_u32(buffers + 20) + 1;
        packet += 16;
        w_u32(buffers + 20, count);
    }
    return count;
}

uint32 sub_80063fcc(void)
{
    static const uint32 starts[7] = {8, 0x188, 0x1F8, 0x3D8, 0x7D8, 0x878, 0x8A0};
    static const uint32 strides[7] = {24, 28, 40, 16, 20, 40, 20};
    uint32 buffers, scene, count, ot, group, index, packet, tag, head, value;
    FUNCTION_MARKER(0x80063FCC, "ACE3.BIN");
    buffers = r_u32(0x800BD074);
    scene = r_u32(0x800BCE54);
    count = r_u32(buffers + 8);
    ot = r_u32(scene + 16);
    for (group = 0; group < 7; ++group)
    {
        if (group == 1 || group == 3 || group == 5)
            buffers = r_u32(0x800BD074);
        if (group != 0)
            count = r_u32(buffers + 8 + group * 4);
        if (group == 0 && count >= 16)
        {
#ifndef NDEBUG
            printf("AVIPKT %d\n", (sint32)(count));
#endif
            continue;
        }
        if ((sint32)count <= 0)
            continue;
        index = 0;
        packet = r_u32(0x800BD078) + starts[group];
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ot);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ot);
            w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(buffers + 8 + group * 4);
            value = (sint32)index < (sint32)count;
            packet += strides[group];
        } while (value);
        if (group == 1 || group == 3 || group == 5)
            buffers = r_u32(0x800BD074);
    }
    buffers = r_u32(0x800BD074);
    value = r_u32(buffers + 36);
    if (value == 1)
    {
        packet = r_u32(0x800BD078);
        head = r_u32(ot);
        tag = r_u32(packet + 0x940);
        w_u32(packet + 0x940, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | ((packet + 0x940) & 0xFFFFFF));
    }
    packet = r_u32(0x800BD078);
    head = r_u32(ot);
    tag = r_u32(packet);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    value = (head & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ot, value);
    return value;
}

uint32 sub_80056634(void)
{
    uint32 mode, data, value, selector;
    FUNCTION_MARKER(0x80056634, "ACE3.BIN");
    mode = r_u32(0x800BD01C);
    if (mode == 2)
    {
        data = r_u32(0x800BCE60);
        value = r_u8(data + 924);
        selector = value == 1 ? 82 : 268;
        value = r_u8(0x800BCE8E);
        sub_80041920(value, selector, 30);
        return 1;
    }
    if (mode == 1)
    {
        w_u32(0x800BD01C, 0);
        sub_8002cc44(255, 0, 0);
        data = r_u32(0x800BCE54);
        if (r_u32(data + 4) == 4)
        {
            data = r_u32(0x800BCEB4);
            value = r_u8(data + 44);
            sub_8005697c(value);
        }
        sub_8004197c(1);
    }
    return 0;
}

uint32 sub_800565e8(uint32 index)
{
    uint32 data, mission;
    FUNCTION_MARKER(0x800565E8, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    w_u32(0x800BD028, 1);
    mission = r_s16(data + 908);
    w_u32(0x800BD01C, 2);
    sub_80056a54(mission, index);
    return sub_8002cc44(0, 0, 0);
}

uint32 sub_8005697c(uint32 index)
{
    uint32 slot, record, id, current;
    FUNCTION_MARKER(0x8005697C, "ACE3.BIN");
    if (index == 255)
    {
        resource_cancel_streams();
        return 255;
    }
    slot = 0x800B2334 + (index << 2);
    record = r_u32(slot);
    id = r_u16(record) & 0x7FF;
    current = sound_get_cur_resource();
    if (id != (current & 0x7FF))
    {
        resource_cancel_streams();
        record = r_u32(slot);
        resource_queue_audio(record, 0, 0);
    }
    return index;
}

uint32 sub_80056a54(uint32 mission, uint32 index)
{
    uint32 table;
    FUNCTION_MARKER(0x80056A54, "ACE3.BIN");
    resource_cancel_streams();
    if (index == 255)
        return 255;
    table = r_u32(0x800B29F4 + (mission << 2));
    resource_queue_audio(table + index * 12, 1, 0x80056A44);
    return index;
}

uint32 sub_80056a44(void)
{
    FUNCTION_MARKER(0x80056A44, "ACE3.BIN");
    w_u32(0x800BD01C, 1);
    return 1;
}

uint32 sub_80056164(uint32 bytes)
{
    uint32 a, b, c, d;
    FUNCTION_MARKER(0x80056164, "ACE3.BIN");
    a = r_u8(bytes);
    b = r_u8(bytes + 1);
    c = r_u8(bytes + 2);
    d = r_u8(bytes + 3);
    return a | (b << 8) | (c << 16) | (d << 24);
}

uint32 sub_80056518(void)
{
    uint32 mode, data, value, index, offset;
    FUNCTION_MARKER(0x80056518, "ACE3.BIN");
    mode = r_u32(0x800BD028);
    if (mode == 1)
        resource_cancel_streams();
    else if (mode == 2)
    {
        data = r_u32(0x800BD030);
        value = r_u8(data + 7);
        if (value != 255)
            sub_8002dda8(value);
        sub_8005671c(0);
    }
    else if (mode == 3)
    {
        w_u32(0x800BD028, 0);
        sub_8004167c();
    }
    sub_8004197c(0);
    data = r_u32(0x800BD020);
    w_u32(data, 255);
    for (index = 0; index < 4; ++index)
    {
        data = r_u32(0x800BD020);
        offset = index << 1;
        w_u8(data + index + 20, 255);
        data = r_u32(0x800BD020);
        w_u16(data + offset + 12, 0);
        w_u16(data + offset + 4, 0);
    }
    return 0;
}

uint32 sub_80056778(void)
{
    uint32 data, stream, value, stamp, timer, cursor, type, payload, game, selector, voice;
    FUNCTION_MARKER(0x80056778, "ACE3.BIN");
    data = r_u32(0x800BD030);
    stream = r_u32(data);
    if (stream == 0)
    {
        voice = r_u8(data + 7);
        if (voice != 255 && sub_8002dd7c(voice) == 0)
        {
            sub_8002cc44(255, 0, 0);
            data = r_u32(0x800BD030);
            w_u8(data + 7, 255);
            sub_8004197c(1);
        }
        return 0;
    }
    stamp = r_s16(stream);
    timer = r_s16(data + 4);
    if (stamp == timer)
    {
        cursor = stream + 3;
        for (;;)
        {
            type = r_u8(cursor - 1);
            if (type == 0)
            {
                data = r_u32(0x800BD030);
                payload = r_u8(cursor);
                w_u8(data + 6, payload);
                payload = r_u8(cursor);
                cursor += 4;
                value = sub_8002db4c(payload);
                data = r_u32(0x800BD030);
                w_u8(data + 7, value);
            }
            else
            {
                if (((type - 1) & 255) < 2)
                {
                    data = r_u32(0x800BD030);
                    value = r_u8(cursor - 1);
                    payload = r_u8(cursor);
                    w_u8(data + value + 7, payload);
                }
                else if (type == 4)
                {
                    value = r_u8(cursor);
                    game = r_u32(0x800BCE60);
                    w_u8(0x800BCE8E, value);
                    value = r_u8(game + 924);
                    selector = value == 1 ? 82 : 268;
                    payload = r_u8(0x800BCE8E);
                    sub_80041920(payload, selector, 30);
                    data = r_u32(0x800BD030);
                    value = r_u8(cursor);
                    w_u32(data + 12, value);
                }
                cursor += 4;
            }
            data = r_u32(0x800BD030);
            stream += 4;
            w_u32(data, stream);
            stamp = r_s16(stream);
            if (stamp == 0xFFFFFFFF)
            {
                w_u32(data, 0);
                break;
            }
            timer = r_s16(data + 4);
            if (stamp != timer)
                break;
        }
    }
    data = r_u32(0x800BD030);
    value = r_u8(data + 8);
    w_u8(0x800BCE94, value);
    value = r_u8(data + 9);
    w_u8(0x800BCE96, 0);
    w_u8(0x800BCE95, value);
    if (r_u32(data) != 0)
    {
        value = r_u16(data + 4) + 1;
        w_u16(data + 4, value);
        stamp = r_s16(stream);
        if ((sint32)stamp < (sint16)value)
            w_u32(data, 0);
    }
    return 1;
}

uint32 sub_8005628c(void)
{
    uint32 game, flags, index, panel, item, slot, elapsed, duration, raw, entry, value, first, second, mode;
    FUNCTION_MARKER(0x8005628C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    if ((flags & 0x20000000) == 0)
    {
        for (index = 0; index < 4; ++index)
        {
            panel = r_u32(0x800BD020);
            item = r_u8(panel + index + 20);
            if (item == 255)
                continue;
            slot = panel + (index << 1);
            elapsed = r_s16(slot + 4);
            duration = r_s16(slot + 12);
            raw = r_u16(slot + 4);
            if (elapsed != duration)
            {
                game = r_u32(0x800BCE60);
                if ((r_u32(game) & 0x20000000) == 0)
                    w_u16(slot + 4, raw + 1);
                continue;
            }
            if (r_u32(panel) != 255)
                continue;
            game = r_u32(0x800BCE60);
            if (r_u8(game + 924) == 1)
            {
                sub_80041654();
                if (sub_80041670() != 2)
                {
                    w_u32(0x800BD028, 3);
                    continue;
                }
            }
            entry = r_u32(0x800BD034) + (item << 2);
            value = r_u8(entry);
            if (value == 0)
            {
                value = r_u8(entry + 1);
                sub_800565e8(value);
            }
            else
            {
                value = r_s16(entry + 2);
                entry = r_u32(0x800BD02C);
                sub_8005671c(entry + (value << 2));
            }
            game = r_u32(0x800BCE60);
            flags = r_u32(game);
            panel = r_u32(0x800BD020);
            w_u32(game, flags | 0x10000000);
            w_u32(panel, item);
            w_u8(panel + index + 20, 255);
        }
    }
    first = sub_80056634();
    second = sub_80056778();
    if (first == 0 && second == 0 && sub_8002d544() == 0)
    {
        mode = r_u32(0x800BD028);
        if (mode != 3)
        {
            if (mode != 0)
            {
                panel = r_u32(0x800BCE54);
                if (r_u32(panel + 4) & 4)
                {
                    game = r_u32(0x800BCE60);
                    flags = r_u32(game);
                    w_u32(game, flags & 0xEFFFFFFF);
                }
            }
            panel = r_u32(0x800BD020);
            w_u32(0x800BD028, 0);
            w_u32(panel, 255);
        }
    }
    panel = r_u32(0x800BD020);
    if (r_u32(panel) == 255 && r_u8(0x800BCE8E) != 0)
    {
        w_u8(0x800BCE8E, 0);
        sub_8004197c(1);
    }
    game = r_u32(0x800BCE60);
    if (r_u8(game + 924) == 1)
    {
        panel = r_u32(0x800BD020);
        if (r_u32(panel) == 255)
            return sub_8004167c();
        return sub_80041654();
    }
    return 1;
}

uint32 sub_80033dc0(void)
{
    uint32 game, scene, flags, value, root, mode, argument;
    FUNCTION_MARKER(0x80033DC0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u8(game + 933) == 0)
        sub_800371c4();
    game = r_u32(0x800BCE60);
    if ((r_u32(game) & 0x20000000) == 0)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 12);
        w_u32(scene + 12, value + 1);
    }
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if (flags & 0x40000000)
    {
        w_u32(scene, flags & 0xBFFFFFFF);
        sub_80032f18(0x10000000, 0);
    }
    else if ((sint32)flags < 0)
    {
        game = r_u32(0x800BCE60);
        w_u8(game + 2391, 1);
        scene = r_u32(0x800BCE54);
        value = r_u32(scene);
        w_u32(scene, value & 0x7FFFFFFF);
        sub_80032f18(0x800000, 0);
    }
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if ((flags & 0x20000000) == 0)
        return 0;
    root = r_u32(0x800BCE50);
    mode = r_u32(scene + 4);
    argument = r_u32(scene + 8);
    w_u32(scene, flags & 0xDFFFFFFF);
    value = r_u32(root + 112);
    w_u32(scene + 4, value);
    value = r_u32(root + 116);
    w_u32(scene + 12, 0);
    w_u32(scene + 8, value);
    return sub_80032080(mode, argument);
}

uint32 sub_80040900(uint32 index, uint32 x, uint32 y)
{
    uint32 record, value, buffers, count, packet;
    FUNCTION_MARKER(0x80040900, "ACE3.BIN");
    record = 0x800B1234 + (index << 2);
    value = r_u8(record + 2);
    if (value == 0)
        return 0;
    buffers = r_u32(0x800BCFB8);
    count = r_u32(buffers + 16);
    packet = r_u32(0x800BCFBC) + count * 20 + 0x1008;
    w_u8(packet + 5, 131);
    w_u8(packet + 4, 0);
    w_u8(packet + 6, 0);
    w_u16(packet + 8, x + 2);
    w_u16(packet + 10, y);
    value = r_u8(record);
    w_u8(packet + 12, value);
    value = r_u8(record + 1);
    w_u8(packet + 13, value);
    value = r_u8(record + 2);
    w_u16(packet + 16, value);
    value = r_u8(record + 3);
    buffers = r_u32(0x800BCFB8);
    w_u16(packet + 18, value);
    count = r_u32(buffers + 16) + 1;
    w_u32(buffers + 16, count);
    return count;
}

uint32 sub_8004084c(uint32 index, uint32 x, uint32 y)
{
    uint32 record, value, buffers, count, packet;
    FUNCTION_MARKER(0x8004084C, "ACE3.BIN");
    record = 0x800B10DC + (index << 2);
    value = r_u8(record + 2);
    if (value == 0)
        return 0;
    buffers = r_u32(0x800BCFB8);
    count = r_u32(buffers + 16);
    packet = r_u32(0x800BCFBC) + count * 20 + 0x1008;
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 131);
    w_u8(packet + 6, 0);
    value = r_u8(record + 2);
    w_u16(packet + 10, y);
    w_u16(packet + 8, x - value - 2);
    value = r_u8(record);
    w_u8(packet + 12, value);
    value = r_u8(record + 1);
    w_u8(packet + 13, value);
    value = r_u8(record + 2);
    w_u16(packet + 16, value);
    value = r_u8(record + 3);
    buffers = r_u32(0x800BCFB8);
    w_u16(packet + 18, value);
    count = r_u32(buffers + 16) + 1;
    w_u32(buffers + 16, count);
    return count;
}

uint32 sub_80044034(uint32 index)
{
    uint32 table, object, state, value;
    FUNCTION_MARKER(0x80044034, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table + (index << 2));
    state = r_u32(0x800BCEC0);
    value = r_u16(object + 28);
    state += index * 184;
    value = 0 - value;
    w_u16(state + 132, value);
    if ((value << 16) != 0)
        return 1;
    value = r_u16(object + 24);
    value = 0 - value;
    w_u16(state + 128, value);
    return (value << 16) != 0;
}

uint32 sub_8004409c(uint32 index)
{
    uint32 table, root, object, state, limits, value, raw, delta, low, high;
    FUNCTION_MARKER(0x8004409C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BCEF8);
    root += index << 6;
    object = r_u32(table + (index << 2));
    state = r_u32(0x800BCEC0);
    limits = r_u32(root + 20);
    state += index * 184;
    w_u16(state + 132, 0);
    w_u16(state + 130, 0);
    w_u16(state + 128, 0);
    value = r_s16(object + 28);
    raw = r_u16(object + 28);
    delta = 0 - value;
    if (value != 0)
    {
        high = r_s16(limits + 24);
        if ((sint32)high < (sint32)delta)
            delta = high;
        low = 0 - high;
        if ((sint32)delta < (sint32)low)
            delta = low;
        w_u16(object + 28, raw + delta);
        return 1;
    }
    value = r_s16(object + 24);
    raw = r_u16(object + 24);
    delta = 0 - value;
    if (value == 0)
        return 0;
    high = r_s16(limits + 26);
    if ((sint32)high < (sint32)delta)
        delta = high;
    low = r_s16(limits + 28);
    if ((sint32)delta < (sint32)low)
        delta = low;
    w_u16(object + 24, raw + delta);
    return 1;
}

uint32 sub_8004d124(uint32 character)
{
    uint32 value;
    FUNCTION_MARKER(0x8004D124, "ACE3.BIN");
    if (((character - 65) & 255) < 26)
        return (character & 255) - 65;
    if (((character - 48) & 255) < 10)
        return (character & 255) - 19;
    value = character & 255;
    if (value == 46)
        return 26;
    if (value == 33)
        return 27;
    if (value == 63)
        return 28;
    if (value == 32)
        return 80;
    return 0xFFFFFFFF;
}

uint32 sub_8004d038(uint32 descriptor)
{
    uint32 index, x, y, text, character, glyph, buffers, count, record, value;
    FUNCTION_MARKER(0x8004D038, "ACE3.BIN");
    index = 0;
    x = r_u16(descriptor);
    y = r_u16(descriptor + 2);
    for (;;)
    {
        text = r_u32(descriptor + 4);
        character = r_u8(text + index);
        if (character == 0)
            return 0;
        ++index;
        glyph = sub_8004d124(character);
        if (glyph == 80)
        {
            x += 10;
            continue;
        }
        if (glyph == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        buffers = r_u32(0x800BCFEC);
        count = r_u32(buffers + 272);
        w_u32(buffers + 272, count + 1);
        record = buffers + (count << 3) + 8;
        w_u8(record, 0);
        w_u8(record + 1, glyph);
        w_u16(record + 2, 0);
        w_u16(record + 4, x);
        w_u16(record + 6, y);
        value = r_u8(0x800B1914 + (glyph << 2) + 2);
        x += value - 1;
    }
}

uint32 sub_80047a54(void)
{
    uint32 index, root, flags, state;
    FUNCTION_MARKER(0x80047A54, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        root = r_u32(0x800BCEC0);
        flags = r_u32(0x800C6FD0 + index * 4);
        state = root + index * 184;
        if ((flags & 0x10000) == 0)
            continue;
        w_u32(0x800C6FD0 + index * 4, flags & 0xFFFEFFFF);
        if (r_u8(state + 96) == 255)
            return 0;
        if (sub_80047b00(index) == 1)
            sub_80047d68(index);
    }
    return 0;
}

uint32 sub_8004f034(uint32 index)
{
    uint32 state, selected, table, mode, object, a, b, c, d;
    FUNCTION_MARKER(0x8004F034, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    selected = r_u8(state + 63);
    table = r_u32(0x800BCE5C);
    mode = r_u8(state + 62);
    object = r_u32(table + (selected << 2));
    if (mode == 0)
        w_u8(state + 62, 1);
    a = r_u32(object + 8);
    b = r_u32(object + 12);
    c = r_u32(object + 16);
    d = r_u32(object + 20);
    w_u32(state + 104, a);
    w_u32(state + 108, b);
    w_u32(state + 112, c);
    w_u32(state + 116, d);
    sub_800505dc(index, state + 104);
    w_u8(state + 99, 4);
    w_u8(state + 97, 6);
    return 6;
}

uint32 sub_8004f4d8(uint32 index)
{
    uint32 state, a, b, c, d, flags;
    FUNCTION_MARKER(0x8004F4D8, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    if (r_u8(state + 62) == 0)
    {
        w_u8(state + 62, 1);
        a = r_u32(state + 68);
        b = r_u32(state + 72);
        c = r_u32(state + 76);
        d = r_u32(state + 80);
        w_u32(state + 104, a);
        w_u32(state + 108, b);
        w_u32(state + 112, c);
        w_u32(state + 116, d);
        sub_800505dc(index, state + 104);
        w_u8(state + 97, 0);
    }
    flags = r_u32(state);
    w_u8(state + 99, (flags & 0x2000000) ? 5 : 0);
    return 5;
}

uint32 sub_8004fb28(uint32 index)
{
    uint32 state, value;
    FUNCTION_MARKER(0x8004FB28, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    if (r_u8(state + 62) == 0)
    {
        w_u8(state + 62, 1);
        w_u8(state + 99, 30);
    }
    else if (r_u32(state) & 0x400000)
        w_u8(state + 61, 255);
    value = r_u8(state + 100);
    if (value == 0)
    {
        w_u8(state + 97, 2);
        return 2;
    }
    w_u8(state + 97, 0);
    return value;
}

uint32 sub_8004fc04(uint32 index)
{
    uint32 state, table, mode, object, root, type, value;
    FUNCTION_MARKER(0x8004FC04, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    table = r_u32(0x800BCE5C);
    mode = r_u8(state + 62);
    object = r_u32(table + (index << 2));
    if (mode == 0)
    {
        root = r_u32(0x800BCEF8);
        root = r_u32(root + (index << 6) + 20);
        type = r_u32(root);
        if (type == 1 || type == 3)
            value = 43;
        else
        {
            type = r_s16(object + 72);
            if (type == 26)
                value = 42;
            else if (type == 44)
                value = 40;
            else
            {
                value = game_random();
                value = (sint32)value % 3 + 40;
            }
        }
        w_u8(state + 99, value);
        w_u8(state + 62, 1);
    }
    w_u8(state + 97, 9);
    return 9;
}

uint32 sub_8004e900(uint32 index)
{
    uint32 root, table, record, value, slot, object;
    FUNCTION_MARKER(0x8004E900, "ACE3.BIN");
    root = r_u32(0x800BCEFC);
    table = r_u32(0x800BCE5C);
    record = root + index * 40;
    slot = table + (index << 2);
    value = r_u32(record + 8);
    value = r_u8(value + 2);
    object = r_u32(slot + 128);
    if (value == 255)
        return slot;
    value = r_u32(object) | 0x10000;
    w_u32(object, value);
    return value;
}

uint32 sub_8004ef80(uint32 index)
{
    uint32 state, selected, table, mode, object, value;
    FUNCTION_MARKER(0x8004EF80, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    selected = r_u8(state + 63);
    table = r_u32(0x800BCE5C);
    mode = r_u8(state + 62);
    object = r_u32(table + (selected << 2));
    if (mode == 0)
        w_u8(state + 62, 1);
    sub_800451ac(object + 8, state + 68, state + 104);
    sub_800505dc(index, state + 104);
    value = r_u32(state + 136);
    w_u8(state + 99, (sint32)value < 0x2000 ? 7 : 4);
    w_u8(state + 97, 5);
    return 5;
}

uint32 sub_80049e28(uint32 index)
{
    uint32 offset, state, mode, value;
    FUNCTION_MARKER(0x80049E28, "ACE3.BIN");
    offset = index * 184;
    state = r_u32(0x800BCEC0) + offset;
    mode = r_u8(state + 100);
    if (mode == 1)
        return offset;
    if (mode == 0)
    {
        value = sub_80044034(index);
        if (value == 0)
            w_u8(state + 100, 1);
        return value;
    }
    if (mode == 2)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_80049a1c(uint32 index)
{
    uint32 value, state, mode, speed;
    uint8 output[8];
    FUNCTION_MARKER(0x80049A1C, "ACE3.BIN");
    value = r_u32(0x800C6FD0 + index * 4);
    w_u32(0x800C6FD0 + index * 4, value | 0x20);
    state = r_u32(0x800BCEC0) + index * 184;
    mode = r_u8(state + 100);
    if (mode == 0)
    {
        w_u8(state + 100, 1);
        return 1;
    }
    if (mode == 1)
    {
        speed = r_s16(state + 156);
        return sub_80044188(index, state + 140, speed, 0, output, 0);
    }
    if (mode == 2)
    {
        value = sub_80044034(index);
        if (value == 0)
        {
            w_u8(state + 100, 3);
            return 3;
        }
        return value;
    }
    if (mode == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_80044360(uint32 index, uint32 position, const uint32 *native_position)
{
    uint32 table, root, object, state, value, target, altitude, limit, low, angle;
    FUNCTION_MARKER(0x80044360, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BCEC0);
    object = r_u32(table + (index << 2));
    value = r_u16(object + 28);
    state = root + index * 184;
    value = 0 - value;
    w_u16(state + 132, value);
    if ((value << 16) != 0)
        return 1;
    target = native_position != 0 ? native_position[1] : r_u32(position + 4);
    altitude = r_u32(object + 12);
    value = (sint32)(target - altitude) >> 12;
    angle = sub_80030d54(0 - value, 2048);
    root = r_u32(0x800BCEF8);
    root = r_u32(root + (index << 6) + 20);
    limit = r_s16(root + 22);
    if (limit == 0)
        limit = 3640;
    if ((sint32)limit < (sint32)angle)
        angle = limit;
    low = 0 - limit;
    if ((sint32)angle < (sint32)low)
        angle = low;
    if (angle - 1 < 7)
        angle = 8;
    if ((sint32)angle < 0 && (sint32)angle >= -7)
        angle = 0xFFFFFFF8;
    value = r_u16(object + 24);
    w_u16(state + 128, angle - value);
    return 1;
}

uint32 sub_8004bd18(uint32 kind, uint32 variant, uint32 output, uint32 *native_output)
{
    uint32 selector, source, a, b, c, d;
    FUNCTION_MARKER(0x8004BD18, "ACE3.BIN");
    selector = r_u8(0x800B1880 + ((kind - 2) << 3) + variant);
    source = 0x800B17F0 + (selector << 4);
    a = r_u32(source);
    b = r_u32(source + 4);
    c = r_u32(source + 8);
    d = r_u32(source + 12);
    if (native_output)
    {
        native_output[0] = a;
        native_output[1] = b;
        native_output[2] = c;
        native_output[3] = d;
    }
    else
    {
        w_u32(output, a);
        w_u32(output + 4, b);
        w_u32(output + 8, c);
        w_u32(output + 12, d);
    }
    return source;
}

uint32 sub_80048a40(void)
{
    uint32 game, value, data;
    FUNCTION_MARKER(0x80048A40, "ACE3.BIN");
    game = r_u32(0x800BCE60) + 952;
    value = sub_80054dbc();
    w_u8(game + 11, value);
    data = r_u32(0x800BCEB4);
    value = r_u16(data + 12);
    w_u16(game + 2, value);
    return value;
}

uint32 sub_80054dbc(void)
{
    uint32 data, mode, a, b, limit, value;
    FUNCTION_MARKER(0x80054DBC, "ACE3.BIN");
    data = r_u32(0x800BCEB4);
    mode = r_u8(data + 52);
    if (mode == 255)
        return 5;
    if (mode == 0)
        return 4;
    if (mode == 1)
    {
        a = r_u8(data + 11);
        b = r_u8(data + 33);
        limit = r_u8(data + 55);
        if ((sint32)limit < (sint32)(a + b))
            return 3;
        a = r_u8(data + 46);
        b = r_u8(data + 45);
        limit = r_u8(data + 54);
        value = a + b;
        if ((sint32)value < (sint32)limit)
            return 3;
        limit = r_u8(data + 53);
        if ((sint32)value < (sint32)limit)
            return 2;
        return 1;
    }
    if (mode == 2)
    {
        value = r_u32(data + 12);
        limit = r_u16(data + 56);
        if ((sint32)value < (sint32)limit)
            return 1;
        limit = r_u16(data + 58);
        if ((sint32)value < (sint32)limit)
            return 2;
        return 3;
    }
    if (mode == 4)
    {
        value = r_u32(data + 4);
        limit = r_u16(data + 56);
        if ((sint32)value >= (sint32)limit)
            return 1;
        limit = r_u16(data + 58);
        if ((sint32)value < (sint32)limit)
            return 3;
        return 2;
    }
    return 4;
}

uint32 sub_80055c4c(uint32 script, uint32 threshold)
{
    uint32 value, length, next, branch;
    FUNCTION_MARKER(0x80055C4C, "ACE3.BIN");
    value = sub_80056164(script);
    script += 4;
    length = r_u8(script);
    next = script + length;
    ++script;
    if ((sint32)value < (sint32)threshold)
    {
        length = r_u8(script);
        branch = script + length;
    }
    else
        branch = script + 1;
    sub_80054ebc(branch);
    return next;
}

uint32 sub_8007e890(uint32 index)
{
    uint32 state;
    FUNCTION_MARKER(0x8007E890, "ACE3.BIN");
    state = r_u32(0x800BD0B8);
    w_u32(state + 6220, index);
    return sub_8007dfc4(4, index, 0, 0);
}

uint32 sub_8007e8c4(uint32 index, uint32 value, uint32 position, uint32 extra, const NATIVE_POSE *native_value)
{
    FUNCTION_MARKER(0x8007E8C4, "ACE3.BIN");
    return sub_8007e2b0(0, index, value, position, extra, native_value);
}

uint32 sub_8007e900(uint32 index)
{
    FUNCTION_MARKER(0x8007E900, "ACE3.BIN");
    return sub_8007e2b0(1, index, 0, 0, 0, 0);
}

uint32 sub_8007ed9c(uint32 kind, uint32 position, uint32 vector, const uint32 *native_position)
{
    uint32 packet, tag, a, b, c, x, y, z, high;
    FUNCTION_MARKER(0x8007ED9C, "ACE3.BIN");
    packet = sub_8007df28(4);
    tag = r_u32(packet);
    a = r_u32(packet + 4);
    b = r_u32(packet + 8);
    c = r_u32(packet + 12);
    tag = (((tag & 0xFFFFFFF0) | 4) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    a &= 0xFFF00000;
    b &= 0xFFF00000;
    c &= 0xFFF00000;
    w_u32(packet, tag);
    w_u8(packet + 3, kind);
    if (native_position)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    a |= x >> 12;
    b |= y >> 12;
    c |= z >> 12;
    w_u32(packet + 4, a);
    w_u32(packet + 12, c);
    a &= 0xFFFFF;
    w_u32(packet + 8, b);
    b &= 0xFFFFF;
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    a |= ((uint32)((sint32)x >> 12)) << 20;
    b |= ((uint32)((sint32)y >> 12)) << 20;
    high = ((uint32)((sint32)z >> 12)) << 20;
    c = r_u32(packet + 12);
    w_u32(packet + 4, a);
    w_u32(packet + 8, b);
    w_u32(packet + 12, (c & 0xFFFFF) | high);
    return packet;
}

uint32 sub_8007eeac(uint32 kind, uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 packet, tag, a, b, c, x, y, z, high;
    FUNCTION_MARKER(0x8007EEAC, "ACE3.BIN");
    packet = sub_8007df28(5);
    tag = r_u32(packet);
    a = r_u32(packet + 4);
    b = r_u32(packet + 8);
    c = r_u32(packet + 12);
    tag = (((tag & 0xFFFFFFF0) | 5) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    a &= 0xFFF00000;
    b &= 0xFFF00000;
    c &= 0xFFF00000;
    w_u32(packet, tag);
    w_u8(packet + 3, kind);
    if (native_position != 0)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    a |= x >> 12;
    b |= y >> 12;
    c |= z >> 12;
    w_u32(packet + 4, a);
    w_u32(packet + 12, c);
    a &= 0xFFFFF;
    w_u32(packet + 8, b);
    b &= 0xFFFFF;
    if (native_vector != 0)
    {
        x = native_vector[0];
        y = native_vector[1];
        z = native_vector[2];
    }
    else
    {
        x = r_u32(vector);
        y = r_u32(vector + 4);
        z = r_u32(vector + 8);
    }
    a |= ((uint32)((sint32)x >> 12)) << 20;
    b |= ((uint32)((sint32)y >> 12)) << 20;
    high = ((uint32)((sint32)z >> 12)) << 20;
    c = r_u32(packet + 12);
    w_u32(packet + 4, a);
    w_u32(packet + 8, b);
    w_u32(packet + 12, (c & 0xFFFFF) | high);
    return packet;
}

uint32 sub_8007efbc(uint32 kind, uint32 value)
{
    uint32 packet, tag;
    FUNCTION_MARKER(0x8007EFBC, "ACE3.BIN");
    packet = sub_8007df28(1);
    kind = (kind & 255) << 9;
    value = (value & 0xFFF) << 17;
    tag = r_u32(packet);
    tag = (((tag & 0xFFFFFFF0) | 1) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    tag = (tag & 0xFFFE01FF) | kind;
    tag = (tag & 0xE001FFFF) | value;
    w_u32(packet, tag);
    return packet;
}

uint32 sub_8007f048(uint32 packet)
{
    uint32 tag, kind, value;
    FUNCTION_MARKER(0x8007F048, "ACE3.BIN");
    tag = r_u32(packet);
    kind = tag & 15;
    if (kind == 3)
    {
        value = r_u8(packet + 4);
        return sub_8007f9e8(value, packet);
    }
    if (kind < 4)
    {
        if (kind == 1)
            return sub_8007f9cc((tag >> 9) & 255, (tag >> 17) & 0xFFF);
        return 1;
    }
    if (kind == 4)
    {
        value = r_u8(packet + 3);
        return sub_8007fb24(value, packet);
    }
    if (kind == 5)
    {
        value = r_u8(packet + 3);
        return sub_8007fca0(value, packet);
    }
    return 5;
}

uint32 sub_8007ebb4(uint32 packet, uint32 complete, uint32 forward, uint32 *native_complete)
{
    uint32 tag, type;
    FUNCTION_MARKER(0x8007EBB4, "ACE3.BIN");
    if (native_complete)
        *native_complete = 0;
    else
        w_u32(complete, 0);
    tag = r_u32(packet);
    type = (tag >> 4) & 7;
    if (type == 0)
    {
        if (((tag >> 7) & 7) == 0)
        {
            if (native_complete)
                *native_complete = 1;
            else
                w_u32(complete, 1);
        }
    }
    else if (type == 4)
    {
        if (forward)
            sub_8007eb50(packet);
        else
            sub_8007eb80(packet);
    }
    tag = r_u32(packet);
    return packet + ((tag & 15) << 2);
}

uint32 sub_8007ec54(uint32 forward)
{
    uint32 root, count, packet, index, complete, end;
    FUNCTION_MARKER(0x8007EC54, "ACE3.BIN");
    root = r_u32(0x800BD0B8);
    count = r_u32(root);
    packet = r_u32(root + 16);
    if ((sint32)count <= 0)
        return root;
    index = 0;
    do
    {
        complete = 0;
        do
        {
            root = r_u32(0x800BD0B8);
            end = r_u32(root + 8);
            if ((sint32)packet >= (sint32)end)
                packet = r_u32(root + 4);
            packet = sub_8007ebb4(packet, 0, forward, &complete);
        } while (complete == 0);
        root = r_u32(0x800BD0B8);
        count = r_u32(root);
        ++index;
    } while ((sint32)index < (sint32)count);
    return 0;
}

uint32 sub_8007f1d4(uint32 packet)
{
    uint32 context, tag, type;
    FUNCTION_MARKER(0x8007F1D4, "ACE3.BIN");
    context = r_u32(0x800BD0B8) + 0x1020;
    tag = r_u32(packet);
    type = (tag >> 4) & 7;
    switch (type)
    {
        case 0:
            sub_8007dc3c(packet, context);
            break;
        case 1:
            sub_8007dd58(packet, context);
            break;
        case 2:
            sub_8007e998(packet);
            break;
        case 3:
            sub_8007f048(packet);
            break;
        case 4:
            sub_8007eb50(packet);
            break;
    }
    tag = r_u32(packet);
    return packet + ((tag & 15) << 2);
}

uint32 sub_8007eb50(uint32 packet)
{
    uint32 tag, kind, index;
    FUNCTION_MARKER(0x8007EB50, "ACE3.BIN");
    tag = r_u32(packet);
    kind = r_u8(packet + 4);
    index = r_u8(packet + 5);
    return sub_8005e460(kind, index, tag >> 23);
}

uint32 sub_8007eb80(uint32 packet)
{
    uint32 tag, kind, index;
    FUNCTION_MARKER(0x8007EB80, "ACE3.BIN");
    tag = r_u32(packet);
    kind = r_u8(packet + 4);
    index = r_u8(packet + 5);
    return sub_8005e460(kind, index, (tag >> 14) & 0x1FF);
}

uint32 sub_8005e460(uint32 kind, uint32 index, sint32 value)
{
    uint32 scene, table, address, old;
    FUNCTION_MARKER(0x8005E460, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    table = r_u32(scene + 584);
    address = table + (index << 9) + (kind << 1);
    old = r_u16(address);
    if (value != 0)
    {
        w_u16(address, (old & 0xFE00) | (uint32)value);
        if (r_u32(scene) & 2)
            sub_8007eb30(kind, index, value, old & 0x1FF);
    }
    return old & 0x1FF;
}

uint32 sub_8007eb30(uint32 kind, uint32 index, uint32 value, uint32 previous)
{
    FUNCTION_MARKER(0x8007EB30, "ACE3.BIN");
    return sub_8007e530(kind, index, value, previous);
}
