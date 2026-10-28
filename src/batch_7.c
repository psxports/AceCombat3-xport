#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8002f620(void)
{
    uint32 index, slot, state;
    FUNCTION_MARKER(0x8002F620, "ACE3.BIN");
    w_u32(0x800C6610, 0);
    for (index = 0; index < 4; ++index)
    {
        slot = 0x800C6610 + index;
        state = 0x800C6630 + index * 92;
        sub_8002dda8(r_u8(slot + 4));
        sub_8002dda8(r_u8(slot + 8));
        sub_8002dda8(r_u8(slot + 12));
        w_u8(slot + 4, 255);
        w_u8(slot + 8, 255);
        w_u8(slot + 12, 255);
        w_u32(state, 0);
        w_u32(state + 8, 0xFFFFFFFF);
        sub_8002e834(state + 36);
        w_u8(state + 48, 0);
    }
    return 0;
}

uint32 sub_8003a624(void)
{
    FUNCTION_MARKER(0x8003A624, "ACE3.BIN");
    return 0xFFFFFFFF;
}

uint32 sub_8003a62c(void)
{
    uint32 mode, target, flags;
    FUNCTION_MARKER(0x8003A62C, "ACE3.BIN");
    mode = r_s16(r_u32(0x800BCE60) + 908);
    target = r_u8(0x800C6D72);
    if (mode != 22 || target == 255)
        return 0xFFFFFFFF;
    flags = r_u32(0x800C6FD0 + target * 4);
    if ((flags & 0x100) != 0 && (~flags & 0x400) != 0)
        return 11;
    return 0xFFFFFFFF;
}

uint32 sub_800718e0(void)
{
    uint32 index, target, value;
    FUNCTION_MARKER(0x800718E0, "ACE3.BIN");
    for (index = 0; index < 12; ++index)
    {
        target = r_u32(0x800BD094) + index;
        value = r_u8(target + 88);
        if (value != 0)
            w_u8(target + 88, value - 1);
    }
    for (index = 0; index < 12; ++index)
    {
        target = r_u32(0x800BD094) + index;
        value = r_u8(target + 100);
        if (value != 0)
            w_u8(target + 100, value - 1);
    }
    return 0;
}

uint32 sub_800718a0(uint32 slots, uint32 count, uint32 value)
{
    uint32 index, available = 0;
    FUNCTION_MARKER(0x800718A0, "ACE3.BIN");
    for (index = 0; (sint32)index < (sint32)count; ++index, ++slots)
    {
        if (r_u8(slots) == 0)
        {
            w_u8(slots, value);
            value = 0;
            ++available;
        }
    }
    return available;
}

uint32 sub_80072178(uint32 flags)
{
    uint32 object, model, state, context, value, other, events, game, damage;
    uint32 first_kind, second_kind, limit, slots, result, x, z, speed;
    FUNCTION_MARKER(0x80072178, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C));
    model = r_u32(r_u32(0x800BD098));
    if ((flags & 1) != 0)
    {
        value = r_u32(object + 84);
        events = 0;
        if ((value & 0x3F) != 0)
        {
            w_u32(object + 84, value & 0xFFFFFFC0);
            value = r_u16(object + 78);
            other = r_u16(model + 136);
            w_u16(object + 78, value - other);
            events = 1;
            value = r_u32(object + 84);
        }
        if ((value & 0x7FC0) != 0)
        {
            w_u32(object + 84, value & 0xFFFF803F);
            value = r_u16(object + 78);
            other = r_u16(model + 138);
            w_u16(object + 78, value - other);
            events |= 2;
        }
        value = r_s16(r_u32(0x800BD094) + 76);
        if ((sint32)value < 8)
            events |= 16;
        else if ((sint32)value < 1536)
            events |= 8;
        w_u32(r_u32(0x800BD098) + 36, events);
    }
    if ((flags & 4) != 0)
        sub_8007084c(object, r_u32(0x800BD094));
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    if ((value & 0x1000) != 0)
    {
        w_u32(state + 16, value & 0xFFFFEFFF);
        value = r_u8(object + 5) | 8;
    }
    else
        value = r_u8(object + 5) & 0xF7;
    w_u8(object + 5, value);
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    w_u32(state + 16, value & 0xFFFFFFDF);
    sub_80070a80(object, state);
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    w_u32(state + 16, value & 0xFFFFFFF0);
    if ((flags & 2) != 0)
        sub_80070cf4(object, state);
    if ((flags & 4) != 0)
        sub_80070a34(object);
    if ((flags & 2) != 0)
    {
        context = r_u32(0x800BD098);
        result = sub_800711fc(object + 8, context + 48, context + 56);
        value = result - r_u32(object + 12);
        if (((sint32)value >> 12) < 66)
        {
            state = r_u32(0x800BD094);
            value = r_u32(state + 16);
            w_u32(state + 16, value | 8);
        }
        context = r_u32(0x800BD098);
        w_u32(context + 40, result);
        value = result - r_u32(object + 12);
        w_u32(context + 44, value);
    }
    state = r_u32(0x800BD094);
    if ((r_u32(state + 16) & 0x20) != 0)
    {
        w_u16(object + 78, 0xFFFF);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        value = r_u32(state + 28);
        w_u32(object + 8, value);
        value = r_u32(state + 32);
        w_u32(object + 12, value);
        value = r_u32(state + 36);
        w_u32(object + 16, value);
    }
    if ((flags & 2) != 0)
    {
        value = r_u32(r_u32(0x800BD094) + 16);
        if ((value & 15) != 0)
        {
            if ((value & 1) != 0 && (flags & 0x40) != 0)
                w_u16(object + 78, 0xFFFF);
            if ((flags & 0x10) != 0)
            {
                x = (uint32)(r_s16(object + 24) * 3840);
                if ((sint32)x < 0)
                    x += 4095;
                z = (uint32)(r_s16(object + 28) * 3840);
                w_u16(object + 24, (sint32)x >> 12);
                if ((sint32)z < 0)
                    z += 4095;
                speed = r_u32(object + 60);
                w_u16(object + 28, (sint32)z >> 12);
                if ((sint32)speed > 0)
                    w_u32(object + 60, 0);
                if ((flags & 0x20) != 0)
                {
                    game = r_u32(0x800BCE60);
                    value = r_u32(game + 916);
                    damage = (sint32)value > 0 ? 5 : 3;
                    if ((sint32)value >= 2)
                        damage = 100;
                    value = r_u16(object + 78);
                    context = r_u32(0x800BD098);
                    w_u16(object + 78, value - damage);
                    value = r_u32(context + 36);
                    w_u32(context + 36, value | 4);
                    if ((r_u32(game + 40) & 7) == 0)
                        sub_8002dbe0(39);
                    sub_80082bac(object + 8, object + 56, r_u32(0x800BD094) + 28, 0);
                }
            }
        }
    }
    if ((r_u32(r_u32(0x800BD094)) & 0x7000) == 0)
    {
        if ((r_u32(r_u32(0x800BD098) + 36) & 1) != 0)
        {
            sub_800295cc();
            w_u16(r_u32(0x800BD094) + 84, 1);
        }
        if ((r_u32(r_u32(0x800BD098) + 36) & 2) != 0)
        {
            sub_80029604();
            w_u16(r_u32(0x800BD094) + 84, 12);
        }
        if ((r_u32(r_u32(0x800BD098) + 36) & 4) != 0)
        {
            sub_80029588();
            w_u16(r_u32(0x800BD094) + 84, 1);
        }
        if (r_s16(r_u32(0x800BD094) + 76) < 1024)
            sub_8006cb44(0);
        sub_80072114();
    }
    state = r_u32(0x800BD094);
    value = r_u16(state + 84);
    if ((sint16)value != 0)
    {
        w_u16(state + 84, value - 1);
        sub_8006cb44(1);
    }
    if (r_s32(r_u32(0x800BD094) + 72) > 0x8C000)
    {
        sub_8006cb44(2);
        sub_800293f0(2048);
    }
    result = sub_8003a5c0();
    game = r_u32(0x800BCE60);
    first_kind = r_u8(game + 937);
    if (result == 0)
    {
        second_kind = r_u8(game + 938);
        state = r_u32(0x800BD094);
        limit = r_s16(model + 124);
        slots = state + 88;
    }
    else
    {
        second_kind = r_u8(game + 939);
        state = r_u32(0x800BD094);
        limit = r_s16(model + 126);
        slots = state + 100;
    }
    result = sub_8003a624();
    if ((sint32)result >= 0)
        first_kind = result;
    result = sub_8003a62c();
    if ((sint32)result >= 0)
        second_kind = result;
    value = r_s16(0x800B3B28 + second_kind * 2);
    if ((sint32)value < (sint32)limit)
        limit = value;
    w_u32(r_u32(0x800BD098) + 4, limit);
    sub_800718e0();
    if ((flags & 8) != 0)
    {
        state = r_u32(0x800BD094);
        value = r_u16(state + 86);
        if ((sint16)value != 0)
            w_u16(state + 86, value - 1);
        state = r_u32(0x800BD094);
        value = r_u32(state + 16);
        game = r_u32(0x800BCE60);
        w_u32(state + 16, value & 0xFFFFDFFF);
        if ((r_u32(game + 296) & 0x800) != 0)
            sub_80072094(first_kind);
        state = r_u32(0x800BD094);
        value = r_u32(state + 16);
        game = r_u32(0x800BCE60);
        w_u32(state + 16, value & 0xFFFFBFFF);
        if ((r_u32(game + 300) & 0x1000) != 0)
            sub_800720d4(second_kind, slots);
    }
    result = sub_800718a0(slots, r_u32(r_u32(0x800BD098) + 4), 0);
    context = r_u32(0x800BD098);
    value = r_u32(context + 4);
    w_u32(context + 32, value - result);
    return result;
}

uint32 sub_80070cf4(uint32 object, uint32 state)
{
    uint32 x, y, z, a, b, bounds, side = 0, kind = 0, normal, value, result;
    uint32 first[3], second[3], map, tile, px, pz, nx, nz, from, to, model, altitude;
    FUNCTION_MARKER(0x80070CF4, "ACE3.BIN");
    a = r_u32(object + 8);
    b = r_u32(object + 12);
    x = r_u32(object + 56) + a;
    y = r_u32(object + 60) + b;
    a = r_u32(object + 16);
    z = r_u32(object + 64) + a;
    bounds = r_u32(0x800BCE54) + 672;
    a = r_u32(bounds) + 132;
    if ((sint32)x < (sint32)a)
    {
        kind = 4;
        side = 1;
        x = a;
    }
    a = r_u32(bounds + 4) - 132;
    if ((sint32)a < (sint32)x)
    {
        kind = 4;
        side = 2;
        x = a;
    }
    a = r_u32(bounds + 8) + 132;
    if ((sint32)y < (sint32)a)
    {
        kind = 3;
        side = 3;
        y = a;
    }
    a = r_u32(bounds + 12) - 132;
    if ((sint32)a < (sint32)y)
    {
        kind = 1;
        side = 4;
        y = a;
    }
    a = r_u32(bounds + 16) + 132;
    if ((sint32)z < (sint32)a)
    {
        kind = 4;
        side = 5;
        z = a;
    }
    a = r_u32(bounds + 20) - 132;
    if ((sint32)a < (sint32)z)
    {
        kind = 4;
        side = 6;
        z = a;
    }
    if (side != 0)
    {
        normal = 0x80024630 + side * 8;
        w_u32(object + 8, x);
        w_u32(object + 12, y);
        w_u32(object + 16, z);
        value = r_u16(normal);
        w_u16(state + 20, value);
        value = r_u16(normal + 2);
        w_u16(state + 22, value);
        a = r_u16(normal + 4);
        value = r_u32(state + 16);
        w_u32(0x800BD0C4, kind);
        w_u32(state + 44, kind);
        w_u32(state + 16, value | 2);
        w_u16(state + 24, a);
    }
    result = r_s32(object + 12) < -12288000;
    if (result != 0)
        return result;
    bounds = r_u32(0x800BCE54) + 580;
    a = r_u32(object + 8);
    b = r_u32(object + 56);
    map = r_u32(bounds + 4);
    first[0] = a + b;
    a = r_u32(object + 12);
    b = r_u32(object + 60);
    first[1] = a + b;
    a = r_u32(object + 16);
    b = r_u32(object + 64);
    first[2] = a + b;
    nx = (uint32)((sint32)first[0] >> 23) + 128;
    nz = (uint32)((sint32)first[2] >> 23) + 128;
    a = r_u32(object + 8);
    b = r_u32(object + 16);
    px = (uint32)((sint32)a >> 23) + 128;
    pz = (uint32)((sint32)b >> 23) + 128;
    if ((sint32)px < 0)
        px &= 7;
    if ((sint32)pz < 0)
        pz &= 7;
    if ((sint32)nx < 0)
        nx &= 7;
    if ((sint32)nz < 0)
        nz &= 7;
    if ((sint32)px >= 256)
        px = (px & 7) + 248;
    if ((sint32)pz >= 256)
        pz = (pz & 7) + 248;
    if ((sint32)nx >= 256)
        nx = (nx & 7) + 248;
    if ((sint32)nz >= 256)
        nz = (nz & 7) + 248;
    from = px + (pz << 8);
    to = nx + (nz << 8);
    tile = r_u16(map + from * 2) & 0x1FF;
    if (tile == 0)
        return tile;
    a = r_u32(bounds + 8);
    b = r_u32(object + 12);
    model = r_u32(a + tile * 4);
    a = r_s16(model + 22);
    altitude = (uint32)((sint32)first[1] >> 12);
    if ((sint32)((uint32)((sint32)b >> 12) + 132) < (sint32)a)
    {
        result = (sint32)(altitude + 132) < (sint32)a;
        if (result != 0 && from == to)
            return result;
    }
    if (from == to)
        result = sub_8009a5d0(object + 8, 0, state + 28, 0x1F800000, 0, first, 0);
    else
        result = sub_8009a800(object + 8, 0, state + 28, 0x1F800000, 0, first, 0);
    if ((sint32)result <= 0x7FFFFFFE)
    {
        normal = r_u32(0x800BD0C0);
        kind = r_u32(0x800BD0C4);
        value = r_u16(normal);
        w_u16(state + 20, value);
        value = r_u16(normal + 2);
        w_u16(state + 22, value);
        a = r_u16(normal + 4);
        value = r_u32(state + 16);
        w_u32(state + 44, kind);
        w_u32(state + 16, value | 1);
        w_u16(state + 24, a);
    }
    if (r_s16(model + 22) < (sint32)(altitude + 99))
    {
        first[0] = r_u32(object + 8);
        a = r_u32(object + 12);
        b = r_u32(object + 60);
        first[1] = a + b + 0x44000;
        first[2] = r_u32(object + 16);
        second[0] = r_u32(object + 8);
        a = r_u32(object + 12);
        b = r_u32(object + 60);
        second[1] = a + b - 0x44000;
        second[2] = r_u32(object + 16);
        if (r_s32(object + 60) > 0)
            result = sub_8009a5d0(0, 0, state + 28, 0x1F800000, second, first, 0);
        else
            result = sub_8009a5d0(0, 0, state + 28, 0x1F800000, first, second, 0);
        if ((sint32)result <= 0x7FFFFFFE)
        {
            normal = r_u32(0x800BD0C0);
            kind = r_u32(0x800BD0C4);
            value = r_u16(normal);
            w_u16(state + 20, value);
            value = r_u16(normal + 2);
            w_u16(state + 22, value);
            a = r_u16(normal + 4);
            value = r_u32(state + 16);
            w_u32(state + 44, kind);
            w_u32(state + 16, value | 2);
            w_u16(state + 24, a);
            a = r_u32(object + 60);
            value = r_u32(state + 32);
            value += (sint32)a > 0 ? 0xFFFBE000 : 0x42000;
            w_u32(object + 12, value);
        }
    }
    a = r_s16(model + 20);
    result = a - 60;
    if ((sint32)a < (sint32)(altitude - 60))
    {
        w_u32(object + 12, result << 12);
        value = r_u32(state + 16);
        w_u16(state + 20, 0);
        w_u16(state + 22, 0xF000);
        w_u16(state + 24, 0);
        w_u32(state + 16, value | 4);
        value = r_u32(object + 8);
        w_u32(state + 28, value);
        value = r_u32(object + 12);
        w_u32(state + 32, value);
        result = r_u32(object + 16);
        w_u32(state + 44, 0);
        w_u32(state + 36, result);
    }
    return result;
}

uint32 sub_800295cc(void)
{
    uint32 entry;
    FUNCTION_MARKER(0x800295CC, "ACE3.BIN");
    entry = sub_8002939c(20);
    if (entry == 0)
        return 3;
    w_u8(entry, 3);
    w_u8(entry + 2, 230);
    return 230;
}

uint32 sub_80029604(void)
{
    uint32 entry;
    FUNCTION_MARKER(0x80029604, "ACE3.BIN");
    entry = sub_8002939c(30);
    if (entry == 0)
        return 15;
    w_u8(entry, 15);
    w_u8(entry + 2, 250);
    return 250;
}

uint32 sub_80029588(void)
{
    uint32 entry, result;
    FUNCTION_MARKER(0x80029588, "ACE3.BIN");
    entry = sub_8002939c(16);
    if (entry == 0)
        return 10;
    w_u8(entry, 10);
    result = (game_random() & 0x3F) + 168;
    w_u8(entry + 2, result);
    return result;
}

uint32 sub_8006cb44(uint32 mode)
{
    uint32 scene, index, table, value, delta, result;
    FUNCTION_MARKER(0x8006CB44, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    result = r_u32(r_u32(scene + 40) + 80) & 3;
    if (result == 0)
        return result;
    index = r_u32(r_u32(0x800BCE60) + 40) & 7;
    if (mode == 1)
    {
        table = 0x800B2CBC + index * 4;
        value = r_u16(scene + 652);
        delta = r_u16(table);
        w_u16(scene + 652, value + delta);
        value = r_u16(scene + 654);
        delta = r_u16(table + 2);
        w_u16(scene + 654, value + delta);
        value = r_u16(scene + 656);
        delta = r_u16(table);
        w_u16(scene + 656, value + delta);
        value = r_u16(scene + 658);
        delta = r_u16(table + 2);
        w_u16(scene + 658, value + delta);
    }
    table = (mode == 0 ? 0x800B2CDC : 0x800B2CFC) + index * 4;
    scene = r_u32(0x800BCE54);
    delta = r_u16(table);
    value = r_u16(scene + 652);
    w_u16(scene + 652, value + delta);
    value = r_u16(scene + 654);
    delta = r_u16(table + 2);
    w_u16(scene + 654, value + delta);
    value = r_u16(scene + 656);
    delta = r_u16(table);
    w_u16(scene + 656, value + delta);
    value = r_u16(scene + 658);
    delta = r_u16(table + 2);
    w_u16(scene + 658, value + delta);
    scene = r_u32(0x800BCE54);
    if (r_s16(scene + 652) >= 7)
    {
        w_u16(scene + 652, 6);
        scene = r_u32(0x800BCE54);
    }
    if (r_s16(scene + 654) >= 7)
        w_u16(scene + 654, 6);
    scene = r_u32(0x800BCE54);
    if (r_s16(scene + 656) >= 7)
    {
        w_u16(scene + 656, 6);
        scene = r_u32(0x800BCE54);
    }
    if (r_s16(scene + 658) >= 7)
        w_u16(scene + 658, 6);
    return 6;
}

uint32 sub_80099750(uint32 model, uint32 offset, uint32 out, const uint8 *native_offset)
{
    uint32 dx, dy, dz, count, x, y, z, a, b, c, cursor, depth;
    FUNCTION_MARKER(0x80099750, "ACE3.BIN");
    cursor = model + 4;
    dx = native_offset ? (sint16)xport_load_le16(native_offset) : r_s16(offset);
    x = r_s16(cursor);
    count = r_u32(model);
    dy = native_offset ? (sint16)xport_load_le16(native_offset + 2) : r_s16(offset + 2);
    y = r_s16(cursor + 2);
    dz = native_offset ? (sint16)xport_load_le16(native_offset + 4) : r_s16(offset + 4);
    z = r_s16(cursor + 12);
    xport_gte_write_data(0, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
    depth = z + dz;
    xport_gte_write_data(1, depth);
    do
    {
        xport_gte_execute(0x486012);
        x = r_s16(cursor + 4);
        y = r_s16(cursor + 6);
        z = r_s16(cursor + 14);
        xport_gte_write_data(2, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(3, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        w_u16(out, a);
        w_u16(out + 2, b);
        w_u16(out + 4, c);
        out += 8;
        xport_gte_execute(0x48E012);
        x = r_s16(cursor + 8);
        y = r_s16(cursor + 10);
        z = r_s16(cursor + 16);
        xport_gte_write_data(4, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(5, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        w_u16(out, a);
        w_u16(out + 2, b);
        w_u16(out + 4, c);
        out += 8;
        xport_gte_execute(0x496012);
        cursor += 20;
        count -= 3;
        x = r_s16(cursor);
        y = r_s16(cursor + 2);
        z = r_s16(cursor + 12);
        xport_gte_write_data(0, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(1, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        w_u16(out, a);
        w_u16(out + 2, b);
        w_u16(out + 4, c);
        out += 8;
    } while ((sint32)count > 0);
    return depth;
}

uint32 sub_80099428(uint32 matrix, uint32 normal, uint8 *native_matrix, const uint16 *native_normal)
{
    uint16 local[9];
    uint32 x, y, z, length, result, numerator, first, second, words[5], a, b, c, column, index;
    FUNCTION_MARKER(0x80099428, "ACE3.BIN");
    for (index = 0; index < 9; ++index)
    {
        a = index % 4 == 0 ? 4096 : 0;
        if (native_matrix)
            xport_store_le16(native_matrix + index * 2, a);
        else
            w_u16(matrix + index * 2, a);
    }
    x = native_normal ? (sint16)native_normal[0] : r_s16(normal);
    z = native_normal ? (sint16)native_normal[2] : r_s16(normal + 4);
    length = sub_80083f40(x, 0, z);
    y = native_normal ? (sint16)native_normal[1] : r_s16(normal + 2);
    local[0] = 4096;
    local[1] = 0;
    local[2] = 0;
    local[3] = 0;
    local[4] = (uint16)length;
    local[5] = (uint16)(0 - y);
    local[6] = 0;
    local[7] = (uint16)y;
    local[8] = (uint16)length;
    result = 0 - y;
    for (index = 0; index < 5; ++index)
        words[index] = native_matrix ? xport_load_le32(native_matrix + index * 4) : r_u32(matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (column = 0; column < 3; ++column)
    {
        xport_gte_write_data(9, local[column]);
        xport_gte_write_data(10, local[column + 3]);
        xport_gte_write_data(11, local[column + 6]);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        if (native_matrix)
        {
            xport_store_le16(native_matrix + column * 2, a);
            xport_store_le16(native_matrix + column * 2 + 6, b);
            xport_store_le16(native_matrix + column * 2 + 12, c);
        }
        else
        {
            w_u16(matrix + column * 2, a);
            w_u16(matrix + column * 2 + 6, b);
            w_u16(matrix + column * 2 + 12, c);
        }
    }
    if (length == 0)
        return result;
    numerator = (uint32)(native_normal ? (sint16)native_normal[0] : r_s16(normal)) << 12;
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    first = (uint32)((sint64)(sint32)numerator / (sint32)length);
    numerator = (uint32)(native_normal ? (sint16)native_normal[2] : r_s16(normal + 4)) << 12;
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    result = (uint32)((sint64)(sint32)numerator / (sint32)length);
    second = result;
    local[0] = (uint16)second;
    local[1] = 0;
    local[2] = (uint16)(0 - first);
    local[3] = 0;
    local[4] = 4096;
    local[5] = 0;
    local[6] = (uint16)first;
    local[7] = 0;
    local[8] = (uint16)second;
    for (index = 0; index < 5; ++index)
        words[index] = native_matrix ? xport_load_le32(native_matrix + index * 4) : r_u32(matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (column = 0; column < 3; ++column)
    {
        xport_gte_write_data(9, local[column]);
        xport_gte_write_data(10, local[column + 3]);
        xport_gte_write_data(11, local[column + 6]);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        if (native_matrix)
        {
            xport_store_le16(native_matrix + column * 2, a);
            xport_store_le16(native_matrix + column * 2 + 6, b);
            xport_store_le16(native_matrix + column * 2 + 12, c);
        }
        else
        {
            w_u16(matrix + column * 2, a);
            w_u16(matrix + column * 2 + 6, b);
            w_u16(matrix + column * 2 + 12, c);
        }
    }
    return result;
}

uint32 sub_8009a5d0(uint32 start, uint32 end, uint32 out, uint32 workspace, const uint32 *native_start, const uint32 *native_end, uint32 *native_out)
{
    uint8 record[24] = {0};
    uint32 direction[3], scene, map, x, z, gx, gz, sx, sz, tile, value, a, b, result;
    FUNCTION_MARKER(0x8009A5D0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    x = native_start ? native_start[0] : r_u32(start);
    map = r_u32(scene + 584);
    sx = (uint32)((sint32)x >> 12);
    value = native_start ? native_start[1] : r_u32(start + 4);
    direction[1] = (uint32)((sint32)value >> 12);
    z = native_start ? native_start[2] : r_u32(start + 8);
    sz = (uint32)((sint32)z >> 12);
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    value = native_start ? native_start[0] : r_u32(start);
    direction[0] = (uint32)((sint32)value >> 12);
    value = native_start ? native_start[1] : r_u32(start + 4);
    direction[1] = (uint32)((sint32)value >> 12);
    value = native_start ? native_start[2] : r_u32(start + 8);
    direction[2] = (uint32)((sint32)value >> 12);
    value = native_end ? native_end[0] : r_u32(end);
    value = native_end ? native_end[1] : r_u32(end + 4);
    value = native_end ? native_end[2] : r_u32(end + 8);
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint16)gx >= 256)
        gx = (gx & 7) | 248;
    if ((sint16)gz >= 256)
        gz = (gz & 7) | 248;
    tile = r_s16(map + (uint32)((sint32)(gx << 16) >> 15) + (uint32)((sint32)(gz << 16) >> 7)) & 0x1FF;
    if (tile == 0)
    {
        value = native_start ? native_start[0] : r_u32(start);
        if (native_out)
            native_out[0] = value;
        else
            w_u32(out, value);
        value = native_start ? native_start[1] : r_u32(start + 4);
        if (native_out)
            native_out[1] = value;
        else
            w_u32(out + 4, value);
        value = native_start ? native_start[2] : r_u32(start + 8);
        if (native_out)
            native_out[2] = value;
        else
            w_u32(out + 8, value);
        return 0x7FFFFFFF;
    }
    xport_store_le16(record, 1024 - (sx & 0x7FF));
    value = native_start ? native_start[1] : r_u32(start + 4);
    xport_store_le16(record + 4, 1024 - (sz & 0x7FF));
    xport_store_le32(record + 8, tile);
    xport_store_le16(record + 2, (sint32)(0 - value) >> 12);
    xport_store_le32(record + 20, 0x200000);
    a = native_end ? native_end[0] : r_u32(end);
    b = native_start ? native_start[0] : r_u32(start);
    direction[0] = (uint32)((sint32)(a - b) >> 12);
    a = native_end ? native_end[1] : r_u32(end + 4);
    b = native_start ? native_start[1] : r_u32(start + 4);
    direction[1] = (uint32)((sint32)(a - b) >> 12);
    a = native_end ? native_end[2] : r_u32(end + 8);
    b = native_start ? native_start[2] : r_u32(start + 8);
    direction[2] = (uint32)((sint32)(a - b) >> 12);
    result = sub_80099894(0, out, workspace, r_u32(scene + 588), 0, direction, native_out, record);
    a = native_out ? native_out[0] : r_u32(out);
    b = native_start ? native_start[0] : r_u32(start);
    if (native_out)
        native_out[0] = a + b;
    else
        w_u32(out, a + b);
    a = native_out ? native_out[1] : r_u32(out + 4);
    b = native_start ? native_start[1] : r_u32(start + 4);
    if (native_out)
        native_out[1] = a + b;
    else
        w_u32(out + 4, a + b);
    a = native_out ? native_out[2] : r_u32(out + 8);
    b = native_start ? native_start[2] : r_u32(start + 8);
    if (native_out)
        native_out[2] = a + b;
    else
        w_u32(out + 8, a + b);
    return result;
}

uint32 sub_800711fc(uint32 position, uint32 normal, uint32 surface)
{
    uint32 first[3], second[3], hit[3], x, z, gx, gz, map, value, source, result;
    FUNCTION_MARKER(0x800711FC, "ACE3.BIN");
    x = r_u32(position);
    z = r_u32(position + 8);
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    map = r_u32(r_u32(0x800BCE54) + 584);
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint32)gx >= 256)
        gx = (gx & 7) + 248;
    if ((sint32)gz >= 256)
        gz = (gz & 7) + 248;
    if ((r_u16(map + (gx + (gz << 8)) * 2) & 0x1FF) == 0)
    {
        if (normal != 0)
        {
            w_u16(normal, 0);
            w_u16(normal + 2, 0xF000);
            w_u16(normal + 4, 0);
        }
        if (surface != 0)
            w_u32(surface, 1);
        return 0;
    }
    value = r_u32(position + 4);
    if ((sint32)value < -12288000)
        value = 0xFF448000;
    first[0] = x;
    first[1] = value;
    first[2] = r_u32(position + 8);
    second[0] = r_u32(position);
    second[1] = 0;
    second[2] = r_u32(position + 8);
    result = sub_8009a5d0(0, 0, 0, 0x1F800000, first, second, hit);
    if ((sint32)result > 0x7FFFFFFE)
    {
        if (normal != 0)
        {
            w_u16(normal, 0);
            w_u16(normal + 2, 0xF000);
            w_u16(normal + 4, 0);
        }
        if (surface != 0)
            w_u32(surface, 0);
        return 0;
    }
    if (normal != 0)
    {
        source = r_u32(0x800BD0C0);
        value = r_u16(source);
        w_u16(normal, value);
        value = r_u16(source + 2);
        w_u16(normal + 2, value);
        value = r_u16(source + 4);
        w_u16(normal + 4, value);
    }
    if (surface != 0)
        w_u32(surface, r_u32(0x800BD0C4));
    return hit[1];
}

uint32 sub_8009a800(uint32 start, uint32 end, uint32 out, uint32 workspace, const uint32 *native_start, const uint32 *native_end, uint32 *native_out)
{
    uint32 first[3], second[3], scene, map, value, a, b, result;
    uint8 records[576] = {0};
    FUNCTION_MARKER(0x8009A800, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    value = native_start ? native_start[0] : r_u32(start);
    map = r_u32(scene + 584);
    first[0] = (uint32)((sint32)value >> 12);
    value = native_start ? native_start[1] : r_u32(start + 4);
    first[1] = (uint32)((sint32)value >> 12);
    value = native_start ? native_start[2] : r_u32(start + 8);
    first[2] = (uint32)((sint32)value >> 12);
    value = native_end ? native_end[0] : r_u32(end);
    second[0] = (uint32)((sint32)value >> 12);
    value = native_end ? native_end[1] : r_u32(end + 4);
    second[1] = (uint32)((sint32)value >> 12);
    value = native_end ? native_end[2] : r_u32(end + 8);
    second[2] = (uint32)((sint32)value >> 12);
    sub_80098dbc(0, 0, workspace, 0, map, first, second, records);
    second[0] -= first[0];
    second[1] -= first[1];
    second[2] -= first[2];
    result = sub_80099894(0, out, workspace, r_u32(scene + 588), 0, second, native_out, records);
    a = native_out ? native_out[0] : r_u32(out);
    b = native_start ? native_start[0] : r_u32(start);
    if (native_out)
        native_out[0] = a + b;
    else
        w_u32(out, a + b);
    a = native_out ? native_out[1] : r_u32(out + 4);
    b = native_start ? native_start[1] : r_u32(start + 4);
    if (native_out)
        native_out[1] = a + b;
    else
        w_u32(out + 4, a + b);
    a = native_out ? native_out[2] : r_u32(out + 8);
    b = native_start ? native_start[2] : r_u32(start + 8);
    if (native_out)
        native_out[2] = a + b;
    else
        w_u32(out + 8, a + b);
    return result;
}

uint32 sub_80098c68(uint32 start, uint32 delta, uint32 plane, uint32 out, const uint32 *native_start, const sint16 *native_delta, uint32 *native_out)
{
    uint32 height, dx, dy, dz, difference, numerator, x, z, value;
    FUNCTION_MARKER(0x80098C68, "ACE3.BIN");
    height = native_start ? native_start[1] : r_u32(start + 4);
    dx = native_delta ? (uint32)(sint32)native_delta[0] : (uint32)r_s16(delta);
    dy = native_delta ? (uint32)(sint32)native_delta[1] : (uint32)r_s16(delta + 2);
    dz = native_delta ? (uint32)(sint32)native_delta[2] : (uint32)r_s16(delta + 4);
    difference = plane - height;
    if ((sint32)dy > 0)
    {
        if ((sint32)plane < (sint32)height)
            return 0;
    }
    else if ((sint32)dy < 0)
    {
        if ((sint32)height < (sint32)plane)
            return 0;
    }
    else
    {
        if (plane != height)
            return 0;
        if (native_out)
            native_out[0] = dx;
        else
            w_u32(out, dx);
        if (native_out)
            native_out[2] = dz;
        else
            w_u32(out + 8, dz);
        if (native_out)
            native_out[1] = difference;
        else
            w_u32(out + 4, difference);
        goto clamp_output;
    }
    numerator = dx * difference;
    if (dy == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    x = (uint32)((sint64)(sint32)numerator / (sint32)dy);
    numerator = dz * difference;
    if (dy == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    z = (uint32)((sint64)(sint32)numerator / (sint32)dy);
    if (native_out)
        native_out[1] = difference;
    else
        w_u32(out + 4, difference);
    if (native_out)
        native_out[0] = x;
    else
        w_u32(out, x);
    if (native_out)
        native_out[2] = z;
    else
        w_u32(out + 8, z);
clamp_output:
    value = native_out ? native_out[0] : r_u32(out);
    if (((sint32)dx < 0 && (sint32)value < (sint32)dx) || ((sint32)dx >= 0 && (sint32)dx < (sint32)value))
    {
        if (native_out)
            native_out[0] = dx;
        else
            w_u32(out, dx);
        if (native_out)
            native_out[1] = dy;
        else
            w_u32(out + 4, dy);
    }
    value = native_out ? native_out[2] : r_u32(out + 8);
    if (((sint32)dz < 0 && (sint32)value < (sint32)dz) || ((sint32)dz >= 0 && (sint32)dz < (sint32)value))
    {
        if (native_out)
            native_out[2] = dz;
        else
            w_u32(out + 8, dz);
        if (native_out)
            native_out[1] = dy;
        else
            w_u32(out + 4, dy);
    }
    value = native_out ? native_out[1] : r_u32(out + 4);
    return value != dy;
}

uint32 sub_80098dbc(uint32 start, uint32 end, uint32 workspace, uint32 records, uint32 map, const uint32 *native_start, const uint32 *native_end, uint8 *native_records)
{
    sint16 delta[3];
    uint32 low[3], high[3], selected[32] = {0};
    uint32 a, b, value, index, result, row, column, dx, dz, ax, az, nx, nz, base_x, base_z;
    uint32 rem_x, rem_z, packed, x, z, cx, cz, current, neighbor, next, corner, different;
    uint32 grid_x, grid_z, gx, gz, tile, record_offset = 0, height;
    FUNCTION_MARKER(0x80098DBC, "ACE3.BIN");
    a = native_start ? native_start[1] : r_u32(start + 4);
    if ((sint32)a < -3000)
    {
        b = native_end ? native_end[1] : r_u32(end + 4);
        if ((sint32)b < -3000)
            goto end_records;
    }
    a = native_end ? native_end[0] & 0xFFFF : r_u16(end);
    b = native_start ? native_start[0] & 0xFFFF : r_u16(start);
    delta[0] = (sint16)(a - b);
    a = native_end ? native_end[1] & 0xFFFF : r_u16(end + 4);
    b = native_start ? native_start[1] & 0xFFFF : r_u16(start + 4);
    delta[1] = (sint16)(a - b);
    a = native_end ? native_end[2] & 0xFFFF : r_u16(end + 8);
    b = native_start ? native_start[2] & 0xFFFF : r_u16(start + 8);
    delta[2] = (sint16)(a - b);
    a = native_start ? native_start[1] : r_u32(start + 4);
    if ((sint32)a >= -2999 && delta[1] < 0)
    {
        for (index = 0; index < 3; ++index)
            low[index] = native_start ? native_start[index] : r_u32(start + index * 4);
        result = sub_80098c68(start, 0, (uint32)-3000, 0, native_start, delta, high);
    }
    else
    {
        result = sub_80098c68(start, 0, (uint32)-3000, 0, native_start, delta, low);
        if (result != 0)
        {
            for (index = 0; index < 3; ++index)
            {
                value = native_start ? native_start[index] : r_u32(start + index * 4);
                low[index] += value;
            }
        }
        else
            for (index = 0; index < 3; ++index)
                low[index] = native_start ? native_start[index] : r_u32(start + index * 4);
        result = sub_80098c68(start, 0, 0, 0, native_start, delta, high);
    }
    if (result != 0)
    {
        for (index = 0; index < 3; ++index)
        {
            value = native_start ? native_start[index] : r_u32(start + index * 4);
            high[index] += value;
        }
    }
    else
        for (index = 0; index < 3; ++index)
            high[index] = native_end ? native_end[index] : r_u32(end + index * 4);
    dx = (uint32)(sint32)(sint16)(high[0] - low[0]);
    dz = (uint32)(sint32)(sint16)(high[2] - low[2]);
    delta[0] = (sint16)dx;
    delta[1] = (sint16)(high[1] - low[1]);
    delta[2] = (sint16)dz;
    ax = (sint32)dx < 0 ? 0 - dx : dx;
    az = (sint32)dz < 0 ? 0 - dz : dz;
    rem_x = low[0] & 0x7FF;
    rem_z = low[2] & 0x7FF;
    nx = ax / 2048 + 2;
    nz = az / 2048 + 2;
    if ((sint32)dx < 0)
    {
        base_x = (uint32)(sint32)(sint16)(dx & 0xF800);
        --nx;
    }
    else if (rem_x == 0)
        base_x = (uint32)-2048;
    else
    {
        base_x = 0;
        --nx;
    }
    if ((sint32)dz < 0)
    {
        base_z = (uint32)(sint32)(sint16)(dz & 0xF800);
        --nz;
    }
    else if (rem_z == 0)
        base_z = (uint32)-2048;
    else
    {
        base_z = 0;
        --nz;
    }
    xport_gte_write_data(12, rem_z | (rem_x << 16));
    packed = ((rem_z + dz) & 0xFFFF) | ((rem_x + dx) << 16);
    xport_gte_write_data(13, packed);
    z = base_z;
    for (row = 0; (sint32)row <= (sint32)(nz + 1); ++row, z += 2048)
    {
        cz = (sint32)z > 32767 ? 0x7FFF : ((sint32)z < -32768 ? 0x8000 : z & 0xFFFF);
        x = base_x;
        for (column = 0; (sint32)column <= (sint32)(nx + 1); ++column, x += 2048)
        {
            cx = (sint32)x > 32767 ? 0x7FFF0000 : ((sint32)x < -32768 ? 0x80000000 : x << 16);
            xport_gte_write_data(14, cz | cx);
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            value = (sint32)value > 0 ? 1 : ((sint32)value < 0 ? 255 : 0);
            w_u8(workspace + row + column * 32, value);
        }
    }
    for (row = 0; (sint32)row <= (sint32)nz; ++row)
    {
        for (column = 0; (sint32)column <= (sint32)nx; ++column)
        {
            current = r_u8(workspace + row + column * 32);
            neighbor = r_u8(workspace + row + 1 + column * 32);
            different = current != neighbor;
            if (different == 0)
            {
                next = r_u8(workspace + row + (column + 1) * 32);
                different = current != next;
                if (different == 0)
                {
                    corner = r_u8(workspace + row + 1 + (column + 1) * 32);
                    different = current != corner || (current & neighbor & next & corner) == 0;
                }
            }
            if (different != 0)
                selected[row] |= 1u << (column & 31);
        }
    }
    grid_x = (uint32)((sint32)low[0] >> 11) + 128;
    grid_z = (uint32)((sint32)low[2] >> 11) + 128;
    value = native_start ? native_start[1] : r_u32(start + 4);
    height = 0 - value;
    z = base_z;
    for (row = 0; (sint32)row <= (sint32)nz; ++row, z += 2048)
    {
        x = base_x;
        for (column = 0; (sint32)column <= (sint32)nx; ++column, x += 2048)
        {
            if ((selected[row] & (1u << (column & 31))) == 0)
                continue;
            a = (sint32)x < 0 ? x + 2047 : x;
            gx = grid_x + (uint32)((sint32)a >> 11);
            a = (sint32)z < 0 ? z + 2047 : z;
            gz = grid_z + (uint32)((sint32)a >> 11);
            if ((sint32)(gx << 16) < 0)
                gx &= 7;
            if ((sint32)(gz << 16) < 0)
                gz &= 7;
            if ((sint16)gx >= 256)
                gx = (gx & 7) | 248;
            if ((sint16)gz >= 256)
                gz = (gz & 7) | 248;
            tile = r_s16(map + (uint32)((sint32)(gx << 16) >> 15) + (uint32)((sint32)(gz << 16) >> 7)) & 0x1FF;
            if (tile == 0)
                continue;
            a = native_start ? native_start[0] & 0xFFFF : r_u16(start);
            value = x - rem_x + (low[0] & 0xFFFF) + 1024 - a;
            if (native_records)
                xport_store_le16(native_records + record_offset, value);
            else
                w_u16(records + record_offset, value);
            b = native_start ? native_start[2] & 0xFFFF : r_u16(start + 8);
            if (native_records)
                xport_store_le16(native_records + record_offset + 2, height);
            else
                w_u16(records + record_offset + 2, height);
            if (native_records)
                xport_store_le32(native_records + record_offset + 8, tile);
            else
                w_u32(records + record_offset + 8, tile);
            value = z - rem_z + (low[2] & 0xFFFF) + 1024 - b;
            if (native_records)
                xport_store_le16(native_records + record_offset + 4, value);
            else
                w_u16(records + record_offset + 4, value);
            record_offset += 12;
        }
    }
end_records:
    if (native_records)
        xport_store_le32(native_records + record_offset + 8, 0x200000);
    else
        w_u32(records + record_offset + 8, 0x200000);
    return 0x200000;
}

uint32 sub_80045898(void)
{
    FUNCTION_MARKER(0x80045898, "ACE3.BIN");
    sub_800458d0();
    sub_80045a40();
    sub_8004c3c4();
    return sub_80046030();
}

uint32 sub_800458d0(void)
{
    uint32 index, table, flags, object, timer, value;
    FUNCTION_MARKER(0x800458D0, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + index * 4);
        object = r_u32(table + index * 4);
        if ((~flags & 4) != 0)
            continue;
        timer = 0x800C70D0 + index;
        value = r_u8(timer);
        if (value != 0)
        {
            --value;
            w_u8(timer, value);
            if ((value & 0xFF) != 0)
                continue;
        }
        value = r_u32(0x800C6FD0 + index * 4);
        w_u32(0x800C6FD0 + index * 4, value & 0xFFFFFFFB);
        value = r_u32(object);
        w_u32(object, value & 0xFFFFBFFF);
    }
    return 0;
}

uint32 sub_80045a40(void)
{
    uint32 index, object, owner, flags, health, kind, damage, descriptor, value, mode, skip;
    FUNCTION_MARKER(0x80045A40, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        if ((~r_u32(object) & 0x200) != 0)
            continue;
        if ((r_u32(0x800C6FD0 + index * 4) & 1) == 0)
            continue;
        health = r_u16(object + 78);
        if ((sint16)health >= 10000)
            continue;
        value = r_u32(object + 80);
        flags = r_u32(object + 84);
        owner = value & 0x3F;
        if ((flags & 0x3F) != 0)
        {
            if ((r_u32(r_u32(0x800BCE54) + 4) & 4) != 0)
            {
                mode = r_s16(r_u32(0x800BCE60) + 908);
                if (mode != 13 && (mode != 22 || r_s16(object + 72) != 70))
                {
                    if (owner == 0)
                    {
                        damage = 0;
                        if ((flags & 1) != 0)
                            damage = 10;
                        else if ((flags & 2) != 0)
                            damage = 16;
                        else if ((flags & 4) != 0)
                            damage = 50;
                        else if ((flags & 8) != 0)
                            damage = 18;
                        else if ((flags & 16) != 0)
                            damage = 32;
                        else if ((flags & 32) != 0)
                            damage = 130;
                        if (r_u32(r_u32(0x800BCE60) + 916) == 0)
                            damage <<= 1;
                        w_u16(object + 78, r_u16(object + 78) - damage);
                    }
                    else
                    {
                        if (owner < 32)
                            descriptor = r_u32(r_u32(0x800BCEF8) + owner * 64 + 32);
                        else
                            descriptor = r_u32(r_u32(0x800BCEFC) + owner * 40 - 1264);
                        if (descriptor != 0)
                            w_u16(object + 78, health - r_u16(descriptor + 2));
                    }
                }
            }
            if (r_s16(object + 78) <= 0)
            {
                mode = r_s16(r_u32(0x800BCE60) + 908);
                kind = mode == 42 ? 88 : (index < 32 ? 27 : 29);
                sub_8002f3c0(kind, object + 8, 0, 0);
            }
            flags = r_u32(object + 84);
        }
        if ((flags & 0x7FC0) != 0)
        {
            if ((r_u32(r_u32(0x800BCE54) + 4) & 4) != 0)
            {
                mode = r_s16(r_u32(0x800BCE60) + 908);
                skip = mode == 22 && r_s16(object + 72) == 70 && (~flags & 0x4000) != 0;
                if (skip == 0)
                {
                    if (owner == 0)
                    {
                        value = r_u32(object + 84);
                        damage = 0;
                        if ((value & 0x40) != 0)
                            damage = 70;
                        else if ((value & 0x80) != 0)
                            damage = 90;
                        else if ((value & 0x100) != 0)
                            damage = 40;
                        else if ((value & 0x200) != 0)
                            damage = 210;
                        else if ((value & 0xC00) != 0)
                            damage = r_s16(object + 78);
                        else if ((value & 0x3000) != 0)
                            damage = 70;
                        else if ((value & 0x4000) != 0)
                            damage = r_s16(object + 78);
                        if (r_u32(r_u32(0x800BCE60) + 916) == 0)
                            damage <<= 1;
                        w_u16(object + 78, r_u16(object + 78) - damage);
                    }
                    else
                    {
                        if (owner < 32)
                            descriptor = r_u32(r_u32(0x800BCEF8) + owner * 64 + 28);
                        else
                            descriptor = r_u32(r_u32(0x800BCEFC) + owner * 40 - 1264);
                        if (descriptor != 0)
                        {
                            health = r_u16(object + 78);
                            damage = r_u16(descriptor + 2);
                            w_u16(object + 78, health - damage);
                        }
                    }
                }
            }
            if ((r_u32(object + 84) & 0x4000) == 0)
            {
                mode = r_s16(r_u32(0x800BCE60) + 908);
                kind = mode == 42 ? 88 : (index < 32 ? 27 : 29);
                sub_8002f3c0(kind, object + 8, 0, 0);
            }
        }
        skip = 0;
        if ((r_u32(object + 84) & 0x8000) != 0)
        {
            if ((r_u32(0x800C6FD0 + index * 4) & 0x800000) == 0)
                skip = 1;
            else
            {
                descriptor = r_u32(r_u32(0x800BCEF8) + index * 64 + 16);
                sub_80048c0c(5, 0, r_u8(descriptor + 3), 0);
            }
        }
        if (skip == 0 && (r_u32(0x800C6FD0 + index * 4) & 0x800000) != 0 && r_s16(object + 78) > 0 && (r_u32(object + 84) & 0x7FC0) != 0 && (r_u32(object) & 0x80) != 0)
        {
            descriptor = r_u32(r_u32(0x800BCEF8) + owner * 64 + 16);
            sub_80048c0c(9, 0, r_u8(descriptor + 3), 0);
        }
        if ((r_u32(object + 84) & 0x7FFF) != 0)
        {
            if (owner < 32 && (r_u32(0x800C6FD0 + owner * 4) & 0x800000) != 0 && r_s16(object + 78) <= 0)
            {
                descriptor = r_u32(r_u32(0x800BCEF8) + owner * 64 + 16);
                sub_80048c0c(4, 0, r_u8(descriptor + 3), 0);
            }
            mode = r_s16(r_u32(0x800BCE60) + 908);
            if ((mode == 9 || mode == 41 || mode == 43) && (r_u32(object) & 0x40) != 0)
            {
                health = r_s16(object + 78);
                if ((sint32)health < 21)
                    sub_8004ceb0(8);
                else if ((sint32)health < 60)
                    sub_8004ceb0(36);
            }
        }
        value = r_u32(object + 84);
        w_u32(object + 84, value & 0xFFFF8000);
    }
    return 0;
}

uint32 sub_8004c3c4(void)
{
    uint32 count, index = 0, entry, table, first_index, second_index, first, second, value, flags;
    FUNCTION_MARKER(0x8004C3C4, "ACE3.BIN");
    count = r_u8(0x800BCEF1);
    if (count == 0)
        return count;
    do
    {
        flags = r_u32(r_u32(0x800BCF18) + index * 4);
        if ((flags & 1) != 0)
        {
            entry = r_u32(0x800BCF0C) + index * 20;
            table = r_u32(0x800BCE5C);
            first_index = r_u8(entry + 2);
            second_index = r_u8(entry + 3);
            first = r_u32(table + first_index * 4);
            value = r_s16(first + 78);
            second = r_u32(table + second_index * 4);
            if ((sint32)value <= 0 && r_s16(second + 78) > 0)
            {
                if ((~r_u32(0x800C6FD0 + second_index * 4) & 1) != 0 && (r_u32(second) & 0x10000) != 0)
                {
                    if (second_index < 32)
                        sub_80038320(second_index);
                    else
                        sub_8004e94c(second_index - 32);
                }
                entry = r_u32(0x800BCF0C) + index * 20;
                w_u16(second + 78, 0);
                if (r_u16(entry) == 5)
                    sub_8004c338(index);
            }
            if ((r_u32(r_u32(0x800BCE54) + 4) & 14) != 0 && second_index < 32)
            {
                if ((r_u32(0x800C6FD0 + first_index * 4) & 4) != 0 && (~r_u32(0x800C6FD0 + second_index * 4) & 4) != 0)
                    sub_80038320(second_index);
                if ((~r_u32(first) & 0x4000) != 0)
                {
                    value = r_u32(second);
                    if ((value & 0x4000) != 0)
                        w_u32(second, value & 0xFFFFBFFF);
                }
            }
        }
        count = r_u8(0x800BCEF1);
        ++index;
    } while (index < count);
    return 0;
}

uint32 sub_80046030(void)
{
    uint32 index, target, value, other;
    FUNCTION_MARKER(0x80046030, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        target = 0x800C6D80 + index * 4;
        value = r_u8(target);
        if (value != 0)
        {
            if ((~r_u32(0x800C6FD0 + index * 4) & 1) != 0)
                sub_8004651c(index);
        }
        else if ((r_u32(0x800C6FD0 + index * 4) & 1) != 0)
            w_u8(target, 255);
    }
    value = r_u8(0x800C6D81);
    if (value == 255)
        return 255;
    other = r_u8(0x800C6D82);
    if (value == other)
    {
        sub_800464b0(other);
        value = r_u8(0x800C6D81);
    }
    other = r_u8(0x800C6D83);
    if (value == other)
        return sub_800464b0(other);
    return other;
}

uint32 sub_80038a3c(void)
{
    FUNCTION_MARKER(0x80038A3C, "ACE3.BIN");
    sub_80038100();
    sub_8003809c();
    return sub_800383ac();
}

uint32 sub_8003809c(void)
{
    uint32 value;
    FUNCTION_MARKER(0x8003809C, "ACE3.BIN");
    value = r_u32(0x800C6ECC);
    w_u32(0x800C6ECC, value & 0xFFFFFFFE);
    sub_80046600();
    sub_80046854();
    sub_800469ec();
    sub_80046dfc();
    sub_8004701c();
    sub_8004833c();
    return sub_80049438();
}

uint32 sub_80038100(void)
{
    uint32 table, state, index, object, flags, value, link, mark;
    FUNCTION_MARKER(0x80038100, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + 368;
    table = r_u32(0x800BCE5C);
    for (index = 2; index < 32; ++index, state += 184)
    {
        object = r_u32(table + index * 4);
        value = r_u32(object) & 0xFFFF7FFF;
        w_u32(object, value);
        if ((~value & 0x4000) != 0)
            continue;
        flags = r_u32(0x800C6FD0 + index * 4) & 0xFFFBFFFF;
        w_u32(0x800C6FD0 + index * 4, flags);
        if (r_s16(object + 78) >= 10000 && (flags & 0x800000) != 0 && (r_u32(object) & 0x80) != 0)
            w_u32(0x800C6FD0 + index * 4, flags | 0x40000);
        flags = r_u32(0x800C6FD0 + index * 4);
        if ((flags & 0x400000) != 0)
            continue;
        if ((flags & 1) == 0)
        {
            if ((flags & 2) != 0)
            {
                value = r_u32(object);
                w_u32(object, value | 0x8000);
            }
            continue;
        }
        mark = (flags & 16) != 0;
        if (mark == 0)
            mark = (r_u32(r_u32(0x800C6ED0 + index * 4)) & 0x80) != 0;
        if (mark == 0)
            mark = r_s16(r_u32(table + index * 4) + 78) <= 0;
        if (mark == 0)
            mark = r_u8(state + 96) == 0;
        if (mark != 0)
        {
            value = r_u32(object);
            w_u32(object, value | 0x8000);
        }
        link = r_u8(0x800C6D80 + index * 4 + 2);
        if (link != 255 && (r_u32(r_u32(table + link * 4)) & 0x40) != 0)
        {
            value = r_u32(object);
            w_u32(object, value | 0x8000);
        }
        link = r_u8(0x800C6D80 + index * 4 + 3);
        if (link != 255 && (r_u32(r_u32(table + link * 4)) & 0x40) != 0)
        {
            value = r_u32(object);
            w_u32(object, value | 0x8000);
        }
    }
    return 0;
}

uint32 sub_80046600(void)
{
    uint32 index, object, flags, target, value, mode, descriptor;
    FUNCTION_MARKER(0x80046600, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        target = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + index * 4);
        object = r_u32(target + index * 4);
        if ((~flags & 1) != 0 || r_s16(object + 78) > 0)
            continue;
        if ((flags & 0x100) != 0)
        {
            target = r_u32(0x800BCEB4);
            value = r_u8(target + 32);
            w_u8(target + 32, value + 1);
        }
        if ((r_u32(0x800C6FD0 + index * 4) & 0x200) != 0)
        {
            target = r_u32(0x800BCEB4);
            value = r_u8(target + 33);
            w_u8(target + 33, value + 1);
        }
        flags = r_u32(0x800C6FD0 + index * 4);
        if ((flags & 0x80000) != 0)
        {
            sub_80054ebc(r_u32(r_u32(0x800BCEAC) + 340));
            descriptor = r_u32(r_u32(0x800BCEF8) + index * 64 + 16);
            value = r_u16(descriptor);
            w_u16(object + 78, value);
            flags = r_u32(0x800C6FD0 + index * 4) & 0xFFF7FFFF;
            w_u32(0x800C6FD0 + index * 4, flags);
            if (r_s16(object + 72) == 27)
            {
                mode = r_s16(r_u32(0x800BCE60) + 908);
                if (mode == 33 || mode == 58)
                {
                    w_u32(0x800C6FD0 + index * 4, flags | 0x800);
                    sub_80048758(index);
                }
            }
            continue;
        }
        if ((flags & 0x800) != 0 && (r_u32(object + 80) & 0x3F) == 0)
        {
            value = r_u32(object);
            if ((~value & 0x40) != 0 && r_s16(object + 72) != 44)
            {
                if ((r_u8(r_u32(0x800BCE60) + 925) & 1) != 0)
                    sub_800797fc(value);
                if ((r_u32(object) & 0x80) != 0)
                {
                    mode = (r_u32(r_u32(0x800BCE54) + 12) & 7) != 0 ? 7 : 6;
                    sub_8004ceb0(mode);
                }
            }
        }
        sub_80048758(index);
        sub_800382d4(index);
        value = r_u32(r_u32(0x800BCEF8) + index * 64 + 52);
        if (value != 0)
            sub_80054ebc(value);
    }
    return 0;
}

void sub_80046854(void)
{
    uint32 index, object, state, value;
    FUNCTION_MARKER(0x80046854, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        state = r_u32(0x800BCEC0) + index * 184;
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        value = r_u8(state + 28);
        if (value != 255)
            continue;
        if (sub_80046928(index) != 0)
            continue;
        w_u8(state + 61, value);
        w_u8(state + 60, value);
        w_u8(state + 99, value);
        w_u8(state + 98, value);
    }
    if ((r_u32(r_u32(0x800BCE54) + 4) & 14) != 0)
        sub_8004ba0c();
    sub_8004be34();
}

uint32 sub_800797fc(uint32 flags)
{
    uint32 result, table, index, object, state;
    FUNCTION_MARKER(0x800797FC, "ACE3.BIN");
    result = ~r_u32(r_u32(0x800BD094)) & 1;
    if (result != 0 || (sint32)flags < 0)
        return result;
    result = r_u8(r_u32(0x800BCE60) + 925);
    if (result == 0)
        return result;
    index = flags & 0x3F;
    table = r_u32(0x800BCE5C);
    object = r_u32(table + index * 4);
    result = ~r_u32(object) & 0x4000;
    if (result != 0)
        return result;
    state = r_u32(r_u32(0x800BCE54) + 40);
    w_u32(state + 192, index);
    object = r_u32(table);
    sub_800793c4(state, 2048, object);
    return sub_80035740(0);
}

uint32 sub_8004ba0c(void)
{
    uint32 count, index = 0, value;
    FUNCTION_MARKER(0x8004BA0C, "ACE3.BIN");
    count = r_u8(0x800BCEDE);
    if (count == 0)
        return count;
    do
    {
        value = r_u32(r_u32(0x800BCEF4) + index * 12);
        if ((value & 1) != 0)
            sub_8004ba7c(index);
        count = r_u8(0x800BCEDE);
        ++index;
    } while (index < count);
    return 0;
}

void sub_8004be34(void)
{
    uint32 count, index, entry, flags, target, value;
    FUNCTION_MARKER(0x8004BE34, "ACE3.BIN");
    count = r_u8(0x800BCEF1);
    if (count == 0)
        return;
    entry = r_u32(0x800BCF0C);
    flags = r_u32(0x800BCF18);
    for (index = 0; index < count; ++index, entry += 20, flags += 4)
    {
        if ((r_u32(flags) & 1) == 0)
            continue;
        target = 0x800C6FD0 + r_u8(entry + 2) * 4;
        value = r_u32(target);
        w_u32(target, value | 0x200000);
        target = 0x800C6FD0 + r_u8(entry + 3) * 4;
        value = r_u32(target);
        w_u32(target, value | 0x400000);
    }
}

uint32 sub_80038320(uint32 index)
{
    uint32 object, flags, value;
    FUNCTION_MARKER(0x80038320, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + index * 4);
    value = r_u32(object);
    w_u32(object, value & 0xFFFF79FF);
    flags = 0x800C6FD0 + index * 4;
    value = r_u32(flags);
    w_u32(flags, value & 0xFF000000);
    value = r_u32(object);
    w_u32(object, value & 0xFFFEFFFF);
    value = r_u32(flags);
    w_u32(flags, value | 4);
    w_u8(0x800C70D0 + index, 150);
    return 150;
}

uint32 sub_800382d4(uint32 index)
{
    uint32 target, value;
    FUNCTION_MARKER(0x800382D4, "ACE3.BIN");
    target = 0x800C6FD0 + index * 4;
    value = (r_u32(target) & 0xFFEF8FFE) | 2;
    w_u32(target, value);
    return value;
}

uint32 sub_8004651c(uint32 index)
{
    uint32 target, linked;
    FUNCTION_MARKER(0x8004651C, "ACE3.BIN");
    target = 0x800C6D80 + index * 4;
    linked = r_u8(target + 2);
    w_u8(target, 0);
    w_u8(target + 2, 255);
    if (linked != 255)
        w_u8(0x800C6D80 + linked * 4 + 1, 255);
    linked = r_u8(target + 3);
    w_u8(target + 3, 255);
    if (linked != 255)
        w_u8(0x800C6D80 + linked * 4 + 1, 255);
    sub_800464b0(index);
    return 255;
}

uint32 sub_800464b0(uint32 index)
{
    uint32 target, linked;
    FUNCTION_MARKER(0x800464B0, "ACE3.BIN");
    if (index == 255)
        return 255;
    target = 0x800C6D80 + index * 4;
    linked = r_u8(target + 1);
    w_u8(target + 1, 255);
    if (linked == 255)
        return 255;
    target = 0x800C6D80 + linked * 4;
    if (r_u8(target + 2) == index)
        w_u8(target + 2, 255);
    else if (r_u8(target + 3) == index)
        w_u8(target + 3, 255);
    return 255;
}

uint32 sub_80046928(uint32 index)
{
    uint32 entry, state, value, next, source, a, b, c, d;
    FUNCTION_MARKER(0x80046928, "ACE3.BIN");
    entry = r_u32(0x800BCEF8) + index * 64;
    state = r_u32(0x800BCEC0) + index * 184;
    value = r_u32(entry + 36);
    next = value + 1;
    if ((sint32)value >= 7)
        return 1;
    value = r_u8(entry + next + 40);
    if (value == 255)
        return 1;
    source = r_u32(0x800BCF48);
    w_u32(entry + 36, next);
    source += value * 32;
    w_u32(entry + 48, source);
    value = r_u8(source);
    w_u8(state + 29, 0);
    w_u8(state + 30, 0);
    w_u8(state + 28, value);
    value = r_u8(source + 1);
    w_u8(state + 31, value);
    a = r_u32(source + 8);
    b = r_u32(source + 12);
    c = r_u32(source + 16);
    d = r_u32(source + 20);
    w_u32(state + 32, a);
    w_u32(state + 36, b);
    w_u32(state + 40, c);
    w_u32(state + 44, d);
    value = r_u32(source + 24);
    w_u32(state + 48, value);
    value = r_u32(source + 28);
    w_u32(state + 52, value);
    return 0;
}

uint32 sub_800469ec(void)
{
    uint32 index, object, state, flags, mode, current, result, value, link, a, b, c, d, descriptor;
    FUNCTION_MARKER(0x800469EC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        state = r_u32(0x800BCEC0) + index * 184;
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        flags = r_u32(0x800C6FD0 + index * 4);
        if ((flags & 2) != 0)
        {
            w_u32(0x800C6FD0 + index * 4, flags & 0xFFEFFFFF);
            mode = r_u8(state + 28);
            w_u8(state + 61, 12);
            if (mode == 6)
                w_u8(state + 28, 0);
            continue;
        }
        mode = r_u8(state + 28);
        if (mode == 5)
        {
            w_u32(0x800C6FD0 + index * 4, flags & 0xFFEFFFFF);
            sub_80050de8(index);
            continue;
        }
        if (mode == 6)
        {
            w_u32(0x800C6FD0 + index * 4, flags & 0xFFEFFFFF);
            sub_80051010(index);
            continue;
        }
        if (mode == 7)
        {
            sub_80051304(index);
            continue;
        }
        if (mode == 8)
        {
            sub_800513d8(index);
            continue;
        }
        if ((flags & 0x100000) != 0)
            continue;
        sub_8004781c(index);
        if (r_u8(state + 60) == 11)
            continue;
        if (sub_800476f0(index) != 0)
        {
            w_u8(state + 61, 11);
            result = sub_800464b0(index);
            value = r_u8(state + 31);
            w_u8(state + 96, result);
            w_u8(state + 63, value);
            a = r_u32(state + 32);
            b = r_u32(state + 36);
            c = r_u32(state + 40);
            d = r_u32(state + 44);
            w_u32(state + 68, a);
            w_u32(state + 72, b);
            w_u32(state + 76, c);
            w_u32(state + 80, d);
            continue;
        }
        if (r_u8(state + 60) == 10)
            continue;
        result = sub_800475b0(index);
        if (result == 0)
            result = sub_80047470(index);
        if (result != 0)
        {
            w_u8(state + 61, 10);
            result = sub_800464b0(index);
            value = r_u8(state + 31);
            w_u8(state + 96, result);
            w_u8(state + 63, value);
            continue;
        }
        current = r_u8(state + 60);
        if (current == 7)
            continue;
        if ((r_u32(r_u32(0x800BD080)) & (1u << (index & 31))) != 0)
        {
            w_u8(state + 61, 7);
            result = sub_800464b0(index);
            w_u8(state + 96, result);
            continue;
        }
        if (current == 5)
            continue;
        if ((r_u32(object) & 0x1C0) != 64 && (r_u32(r_u32(0x800C6ED0 + index * 4)) & 0x2000000) != 0 && r_u32(r_u32(0x800BCE60) + 916) != 3)
        {
            w_u8(state + 61, 5);
            result = sub_800464b0(index);
            w_u8(state + 96, result);
            continue;
        }
        mode = r_u8(state + 28);
        switch (mode)
        {
            case 2:
            case 4:
                sub_800508d0(index);
                break;
            case 9:
            case 10:
                sub_80050bb4(index);
                break;
            default:
                sub_80050720(index);
                break;
        }
        link = r_u8(state + 96);
        if (link == 255)
            continue;
        mode = r_u8(state + 28);
        if (mode - 9 < 2)
            continue;
        descriptor = r_u32(r_u32(0x800BCEF8) + index * 64 + 24);
        a = r_s16(state + 182);
        b = r_s16(descriptor + 20);
        if ((sint32)b < (sint32)a)
        {
            result = sub_800464b0(index);
            w_u8(state + 96, result);
            continue;
        }
        if (link != 0)
        {
            value = r_u8(state + 96);
            if ((~r_u32(0x800C6FD0 + value * 4) & 1) != 0)
            {
                w_u8(state + 96, 255);
                continue;
            }
        }
        value = r_u8(0x800C6D80 + index * 4 + 1);
        if (value == 255)
            w_u8(state + 96, value);
        else
        {
            value = r_u32(0x800C6FD0 + index * 4);
            w_u32(0x800C6FD0 + index * 4, value | 0x10000);
        }
    }
    return 0;
}

uint32 sub_8004781c(uint32 index)
{
    uint32 position[4], state, object, target, table, flags, mode, radius, x, z, value;
    FUNCTION_MARKER(0x8004781C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    state = r_u32(0x800BCEC0) + index * 184;
    flags = r_u32(state);
    object = r_u32(table + index * 4);
    mode = r_u8(state + 28);
    w_u32(state, flags | 0x3000000);
    if (mode == 2 || mode == 4 || mode == 9 || mode == 10)
    {
        value = r_u8(state + 31);
        table = r_u32(0x800BCE5C);
        target = r_u32(table + value * 4) + 8;
    }
    else
        target = state + 32;
    position[0] = r_u32(target);
    position[1] = r_u32(target + 4);
    position[2] = r_u32(target + 8);
    position[3] = r_u32(target + 12);
    radius = r_u32(state + 48);
    x = r_u32(object + 8);
    if ((sint32)(position[0] + radius) >= (sint32)x && (sint32)x >= (sint32)(position[0] - radius))
    {
        z = r_u32(object + 16);
        if ((sint32)(position[2] + radius) >= (sint32)z && (sint32)z >= (sint32)(position[2] - radius))
        {
            value = r_u32(state);
            w_u32(state, value & 0xFEFFFFFF);
        }
    }
    mode = r_u8(state + 28);
    radius = r_u32(state + 52);
    x = r_u32(object + 8);
    if ((sint32)(position[0] + radius) < (sint32)x || (sint32)x < (sint32)(position[0] - radius))
        return 0;
    z = r_u32(object + 16);
    if ((sint32)(position[2] + radius) < (sint32)z || (sint32)z < (sint32)(position[2] - radius))
        return 0;
    if (mode == 2 || mode == 4 || mode == 9 || mode == 10)
    {
        value = r_u32(object + 12);
        if ((sint32)(position[1] + 0x800000) < (sint32)value || (sint32)value < (sint32)(position[1] - 0x800000))
            return 0;
    }
    value = r_u32(state);
    w_u32(state, value & 0xFDFFFFFF);
    return 0;
}

uint32 sub_800476f0(uint32 index)
{
    uint32 object, bounds, x, z, minimum_x, maximum_x, minimum_z, maximum_z, vx, vz, y, value, mode;
    FUNCTION_MARKER(0x800476F0, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + index * 4);
    bounds = r_u32(0x800BCEB0);
    minimum_x = r_u32(bounds + 16);
    x = r_u32(object + 8);
    if ((sint32)x < (sint32)minimum_x)
        return 2;
    maximum_x = r_u32(bounds + 20);
    if ((sint32)maximum_x < (sint32)x)
        return 2;
    z = r_u32(object + 16);
    minimum_z = r_u32(bounds + 24);
    if ((sint32)z < (sint32)minimum_z)
        return 2;
    maximum_z = r_u32(bounds + 28);
    if ((sint32)maximum_z < (sint32)z)
        return 2;
    vx = r_u32(object + 56);
    vz = r_u32(object + 64);
    x += vx << 8;
    z += vz << 8;
    if ((sint32)x < (sint32)minimum_x || (sint32)maximum_x < (sint32)x || (sint32)z < (sint32)minimum_z || (sint32)maximum_z < (sint32)z)
        return 1;
    value = r_u32(object + 60);
    y = r_u32(object + 12);
    bounds = r_u32(0x800BCE60);
    y += value << 8;
    mode = r_s16(bounds + 908);
    if (mode == 42 && ((sint32)y > 0x3A98000 || (sint32)y < (sint32)0xF15A0000))
        return 1;
    mode = r_s16(r_u32(0x800BCE60) + 908);
    if (mode == 58)
        return (sint32)y > (sint32)0xFFD00000;
    return 0;
}

uint32 sub_800475b0(uint32 index)
{
    uint32 object, entry, descriptor, position, threshold, parameters, speed, numerator, time, divisor, value, a, b;
    FUNCTION_MARKER(0x800475B0, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + index * 4);
    entry = r_u32(0x800BCEF8) + index * 64;
    descriptor = r_u32(entry + 24);
    position = r_u32(object + 12);
    threshold = r_u32(descriptor + 4);
    parameters = r_u32(entry + 20);
    if ((sint32)position < (sint32)threshold)
        return 2;
    speed = r_u32(object + 60);
    numerator = threshold - position;
    if ((sint32)speed >= 0)
        return 0;
    if (speed == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    time = (uint32)((sint64)(sint32)numerator / (sint32)speed);
    if ((sint32)time < 0)
        return 2;
    divisor = r_u16(parameters + 24);
    numerator = r_s16(object + 28);
    divisor = (uint32)((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    a = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)a < 0)
        a = 0 - a;
    numerator = 0 - (uint32)r_s16(object + 24);
    divisor = r_u16(parameters + 28);
    divisor = (uint32)((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    b = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)b < 0)
        b = 0;
    value = a + 2 + b;
    return (sint32)value >= (sint32)time;
}

uint32 sub_80047470(uint32 index)
{
    uint32 object, entry, descriptor, position, threshold, parameters, speed, numerator, time, divisor, value, a, b;
    FUNCTION_MARKER(0x80047470, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + index * 4);
    entry = r_u32(0x800BCEF8) + index * 64;
    descriptor = r_u32(entry + 24);
    position = r_u32(object + 12);
    threshold = r_u32(descriptor + 8);
    parameters = r_u32(entry + 20);
    if ((sint32)threshold < (sint32)position)
        return 2;
    speed = r_u32(object + 60);
    numerator = threshold - position;
    if ((sint32)speed <= 0)
        return 0;
    if (speed == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    time = (uint32)((sint64)(sint32)numerator / (sint32)speed);
    if ((sint32)time < 0)
        return 2;
    divisor = r_u16(parameters + 24);
    numerator = r_s16(object + 28);
    divisor = (uint32)((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    a = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)a < 0)
        a = 0 - a;
    numerator = 0 - (uint32)r_s16(object + 24);
    divisor = r_u16(parameters + 26);
    divisor = (uint32)((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    b = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)b < 0)
        b = 0;
    value = a + 2 + b;
    return (sint32)value >= (sint32)time;
}

uint32 sub_800463b8(uint32 index, uint32 target)
{
    uint32 record, linked;
    FUNCTION_MARKER(0x800463B8, "ACE3.BIN");
    if (target == 255)
        return target;
    if (target == 0 && r_u8(0x800C6D81) == index)
        return 255;
    record = 0x800C6D80 + target * 4;
    linked = r_u8(record + 2);
    if (linked == 255)
        w_u8(record + 2, index);
    else
    {
        linked = r_u8(record + 3);
        if (linked != 255)
        {
            if (index != 0)
                return 255;
            sub_800464b0(r_u8(record + 3));
            w_u8(record + 3, 0);
        }
        else
        {
            if (r_u32(r_u32(0x800BCE60) + 916) == 0 && target == 0)
                return 255;
            w_u8(0x800C6D80 + target * 4 + 3, index);
        }
    }
    w_u8(0x800C6D80 + index * 4 + 1, target);
    return target;
}

uint32 sub_80050ac0(uint32 index)
{
    uint32 state, result, mode, angle, value, cosine;
    FUNCTION_MARKER(0x80050AC0, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    value = r_u8(state + 31);
    w_u8(state + 63, value);
    result = r_u8(state + 61);
    if (result == 2)
        return result;
    mode = r_u8(state + 28);
    w_u8(state + 61, 2);
    if (mode == 2)
    {
        w_u32(state + 68, 0);
        result = ((game_random() & 0x1FF) - 256) << 7;
        w_u32(state + 72, result);
        result = 0x1000000;
        w_u32(state + 76, result);
    }
    else
    {
        result = 4;
        if (mode == 4)
        {
            angle = (index & 15) << 10;
            value = (uint32)r_s16(0x800104AC + angle * 2) << 7;
            w_u32(state + 68, value);
            result = game_random();
            cosine = r_s16(0x800104AC + (angle + 4096) * 2);
            result = ((result & 0x1FF) - 256) << 7;
            w_u32(state + 72, result);
            w_u32(state + 76, cosine << 7);
        }
    }
    return result;
}

uint32 sub_800508d0(uint32 index)
{
    uint32 state, target, object, mode, current, link, result, value, descriptor, distance;
    FUNCTION_MARKER(0x800508D0, "ACE3.BIN");
    state = r_u32(0x800BCEC0) + index * 184;
    target = r_u8(state + 31);
    object = r_u32(r_u32(0x800BCE5C) + target * 4);
    if (r_s16(object + 78) <= 0)
    {
        w_u8(state + 28, 255);
        return 0;
    }
    mode = r_u8(state + 28);
    if (mode == 4 && r_u8(state + 60) == 2 && (r_u32(state) & 0x2000000) != 0 && r_u8(state + 63) == target)
        return 1;
    if ((r_u32(state) & 0x1000000) != 0)
        goto release_target;
    link = r_u8(state + 96);
    if (link == 255)
    {
        if (r_u8(state + 28) == 2)
        {
            result = sub_800463b8(index, r_u8(state + 31));
            w_u8(state + 96, result);
        }
        value = r_u8(state + 96);
        if (value == 255)
        {
            result = sub_80046124(index);
            w_u8(state + 96, result);
            if ((result & 0xFF) == value)
            {
                sub_80050ac0(index);
                return 1;
            }
        }
        target = r_u8(state + 96);
        w_u8(state + 61, 1);
        w_u32(state + 68, 0);
        w_u32(state + 72, 0);
        w_u32(state + 76, 0);
        w_u8(state + 63, target);
        return 1;
    }
    current = r_u8(state + 60);
    if (current == 1)
    {
        descriptor = r_u32(r_u32(0x800BCEF8) + index * 64 + 28);
        distance = r_u32(state + 160);
        value = r_u32(descriptor + 8);
        if (distance < value)
            w_u8(state + 61, 4);
        return 1;
    }
    if (current == 4)
    {
        descriptor = r_u32(r_u32(0x800BCEF8) + index * 64 + 28);
        distance = r_u32(state + 160);
        value = r_u32(descriptor + 8);
        if (value < distance)
            w_u8(state + 61, 1);
        return 1;
    }
    if (current == 2)
        return 1;
release_target:
    result = sub_800464b0(index);
    w_u8(state + 96, result);
    sub_80050ac0(index);
    return 1;
}

uint32 sub_80046124(uint32 index)
{
    uint32 scene, table, object, flags, team, selected = 255, best = 0x100000, available, mode;
    uint32 candidate, target, target_flags, other_team, value, a, b, distance;
    FUNCTION_MARKER(0x80046124, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    value = r_u32(scene + 12);
    object = r_u32(table + index * 4);
    if (index != (value & 0x3F))
        return 255;
    flags = r_u32(object);
    team = flags & 0x1C0;
    if ((flags & 0x80) != 0)
    {
        mode = r_u32(r_u32(0x800BCE60) + 916);
        value = r_u8(0x800C6D82);
        available = value == 255;
        if (mode != 0 && available == 0)
            available = r_u8(0x800C6D83) == 255;
        if (available != 0 && r_u8(0x800C6D81) != index)
            selected = 0;
    }
    if (selected == 255)
    {
        for (candidate = 2; candidate < 64; ++candidate)
        {
            if (candidate == index)
                continue;
            target = r_u32(r_u32(0x800BCE5C) + candidate * 4);
            target_flags = r_u32(target);
            if ((~target_flags & 0x4000) != 0)
                continue;
            flags = r_u32(0x800C6FD0 + candidate * 4);
            if ((flags & 1) == 0 || (flags & 0x800000) != 0)
                continue;
            other_team = target_flags & 0x1C0;
            if (other_team == team || other_team == 256)
                continue;
            if (r_u8(0x800C6D80 + candidate * 4 + 2) != 255 && r_u8(0x800C6D80 + candidate * 4 + 3) != 255)
                continue;
            if (r_u8(0x800C6D80 + index * 4 + 2) == candidate || r_u8(0x800C6D80 + index * 4 + 3) == candidate)
                continue;
            if (team == 64 && (flags & 0x100) != 0 && r_s16(r_u32(0x800BCE60) + 908) != 30)
                continue;
            a = r_u32(object + 8);
            b = r_u32(target + 8);
            distance = (uint32)((sint32)(a - b) >> 12);
            if ((sint32)distance < 0)
                distance = 0 - distance;
            if ((sint32)best < (sint32)distance)
                continue;
            a = r_u32(object + 16);
            b = r_u32(target + 16);
            distance = (uint32)((sint32)(a - b) >> 12);
            if ((sint32)distance < 0)
                distance = 0 - distance;
            if ((sint32)best < (sint32)distance)
                continue;
            distance = sub_80083dfc(object + 8, target + 8, 0, 0);
            if ((sint32)distance < (sint32)best)
            {
                selected = candidate;
                best = distance;
            }
        }
    }
    return sub_800463b8(index, selected);
}

uint32 sub_80046dfc(void)
{
    uint32 index, object, state, value, requested;
    FUNCTION_MARKER(0x80046DFC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        state = r_u32(0x800BCEC0) + index * 184;
        if ((~r_u32(object) & 0xC000) != 0 || r_u8(state + 28) == 6)
            continue;
        requested = r_u8(state + 61);
        value = r_u8(state + 60);
        if (value != requested)
        {
            w_u8(state + 60, requested);
            w_u8(state + 62, 0);
        }
        value = r_u8(state + 60);
        switch (value)
        {
            case 0:
                sub_8004f4d8(index);
                break;
            case 1:
                sub_8004ed9c(index);
                break;
            case 2:
                sub_8004ee68(index);
                break;
            case 3:
                sub_8004ef80(index);
                break;
            case 4:
                sub_8004f034(index);
                break;
            case 5:
                sub_8004f0c8(index);
                break;
            case 7:
                sub_8004f570(index);
                break;
            case 10:
                sub_8004fb28(index);
                break;
            case 11:
                sub_8004fba4(index);
                break;
            case 12:
                sub_8004fc04(index);
                break;
            case 14:
                sub_8004fcf0(index);
                break;
            case 15:
                sub_8004fd38(index);
                break;
            case 16:
                sub_8004fd9c(index);
                break;
            case 17:
                sub_8004ff88(index);
                break;
            case 18:
                sub_800500ec(index);
                break;
            case 19:
                sub_80050134(index);
                break;
            case 20:
                sub_80050184(index);
                break;
            case 21:
                sub_800502d4(index);
                break;
            case 22:
                sub_80050534(index);
                break;
            default:
                break;
        }
        requested = r_u8(state + 61);
        if (requested == 255)
        {
            value = r_u32(state);
            w_u8(state + 60, requested);
            w_u32(state, value | 0x200000);
        }
    }
    return 0;
}

uint32 sub_8004701c(void)
{
    uint32 index, object, state, mode, requested, value, flags, entry, kind, special;
    FUNCTION_MARKER(0x8004701C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        state = r_u32(0x800BCEC0) + index * 184;
        if ((~r_u32(object) & 0xC000) != 0 || r_u8(state + 28) == 6)
            continue;
        mode = r_u8(state + 98);
        requested = r_u8(state + 99);
        if (mode != requested)
        {
            flags = r_u32(state);
            requested = r_u8(state + 99);
            w_u8(state + 100, 0);
            w_u16(state + 102, 0);
            w_u32(state, flags & 0xFFBFFFFF);
            w_u8(state + 98, requested);
            w_u16(object + 48, 0);
            w_u16(object + 50, 0);
            w_u16(object + 52, 0);
            mode = r_u8(state + 98);
        }
        switch (mode)
        {
            case 0:
                sub_80049e28(index);
                break;
            case 4:
                entry = r_u32(0x800BCEF8) + index * 64;
                kind = r_u32(r_u32(entry + 20));
                if (kind == 1)
                {
                    sub_80049af8(index);
                    break;
                }
                special = r_u8(state + 60) != 3 && r_u8(r_u32(entry + 24)) == 0;
                if (special == 0)
                {
                    kind = r_s16(r_u32(0x800BCE60) + 908);
                    special = kind == 32 || kind == 47 || kind == 53;
                }
                if (special != 0)
                    sub_8004966c(index);
                else
                    sub_8004960c(index);
                break;
            case 5:
                kind = r_u32(r_u32(r_u32(0x800BCEF8) + index * 64 + 20));
                if (kind == 1)
                    sub_80049af8(index);
                else if (kind == 3)
                    sub_80049c4c(index);
                else
                    sub_80049a1c(index);
                break;
            case 6:
                sub_80049af8(index);
                break;
            case 7:
                sub_8004986c(index);
                break;
            case 8:
                sub_80049ec4(index);
                break;
            case 9:
                sub_80049f38(index);
                break;
            case 10:
                sub_8004a2d8(index);
                break;
            case 11:
                sub_8004a394(index);
                break;
            case 12:
                sub_8004a678(index);
                break;
            case 13:
                sub_8004a474(index);
                break;
            case 14:
                sub_8004a56c(index);
                break;
            case 15:
            case 16:
            case 17:
                sub_8004a7e8(index);
                break;
            case 25:
                sub_8004aa3c(index);
                break;
            case 26:
                sub_8004ac90(index);
                break;
            case 27:
                sub_8004ad98(index);
                break;
            case 30:
                sub_8004afac(index);
                break;
            case 31:
                sub_8004b108(index);
                break;
            case 40:
                sub_8004b308(index);
                break;
            case 41:
                sub_8004b3c8(index);
                break;
            case 42:
                sub_8004b4d8(index);
                break;
            case 43:
                sub_8004b6e4(index);
                break;
            case 50:
                sub_8004a018(index);
                break;
            case 51:
                sub_8004a0c0(index);
                break;
            case 52:
                sub_8004b88c(index);
                break;
            default:
                break;
        }
        value = r_u16(state + 102);
        requested = r_u8(state + 99);
        w_u16(state + 102, value + 1);
        if (requested == 255)
        {
            flags = r_u32(state);
            requested = r_u8(state + 99);
            w_u32(state, flags | 0x400000);
            w_u8(state + 98, requested);
            w_u16(object + 48, 0);
            w_u16(object + 50, 0);
            w_u16(object + 52, 0);
        }
    }
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        if ((~r_u32(object) & 0xC000) == 0 && r_u8(r_u32(0x800BCEC0) + index * 184 + 28) != 6)
            sub_80044a6c(index);
    }
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        if ((~r_u32(object) & 0xC000) == 0 && r_u8(r_u32(0x800BCEC0) + index * 184 + 28) != 6)
            sub_80044c28(index);
    }
    return 0;
}

uint32 sub_8002f70c(uint32 slot, uint32 object_index)
{
    uint32 object, kind, result;
    FUNCTION_MARKER(0x8002F70C, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + object_index * 4);
    kind = r_u16(object + 72);
    if (kind >= 48)
        return 0;
    result = r_u8(0x800B0964 + (kind & 0x3F));
    w_u32(0x800C6630 + slot * 92 + 4, result);
    return result;
}

uint32 sub_8002f770(uint32 selected, uint32 mapping, const uint32 *native_selected, uint32 *native_mapping)
{
    uint32 assigned_slots = 0, assigned_inputs = 0, slot, input, state, value, other, result;
    FUNCTION_MARKER(0x8002F770, "ACE3.BIN");
    result = r_u32(0x800C6610);
    slot = 0;
    if ((sint32)result > 0)
    {
        do
        {
            state = 0x800C6630 + slot * 92;
            if ((((assigned_slots & 0xFF) >> (slot & 31)) & 1) == 0 && r_s32(0x800C6610) > 0)
            {
                input = 0;
                do
                {
                    if ((((assigned_inputs & 0xFF) >> (input & 31)) & 1) == 0)
                    {
                        value = r_u32(state + 8);
                        other = native_selected ? native_selected[input] : r_u32(selected + input * 4);
                        if (value == other)
                        {
                            if (native_mapping)
                                native_mapping[input] = slot;
                            else
                                w_u32(mapping + input * 4, slot);
                            assigned_slots |= 1u << (slot & 31);
                            assigned_inputs |= 1u << (input & 31);
                            break;
                        }
                    }
                    ++input;
                } while ((sint32)input < r_s32(0x800C6610));
            }
            ++slot;
        } while ((sint32)slot < r_s32(0x800C6610));
    }
    result = r_u32(0x800C6610);
    slot = 0;
    if ((sint32)result <= 0)
        return result;
    do
    {
        state = 0x800C6630 + slot * 92;
        if ((((assigned_slots & 0xFF) >> (slot & 31)) & 1) == 0 && r_s32(0x800C6610) > 0)
        {
            input = 0;
            do
            {
                if ((((assigned_inputs & 0xFF) >> (input & 31)) & 1) == 0)
                {
                    value = native_selected ? native_selected[input] : r_u32(selected + input * 4);
                    w_u32(state + 8, value);
                    value = native_selected ? native_selected[input] : r_u32(selected + input * 4);
                    if (value == 0xFFFFFFFF)
                    {
                        value = r_u32(state);
                        w_u32(state, value | 2);
                        sub_8002e834(state + 36);
                    }
                    else
                    {
                        value = r_u32(state);
                        w_u32(state, value | 1);
                        value = native_selected ? native_selected[input] : r_u32(selected + input * 4);
                        sub_8002f70c(slot, value);
                    }
                    if (native_mapping)
                        native_mapping[input] = slot;
                    else
                        w_u32(mapping + input * 4, slot);
                    assigned_slots |= 1u << (slot & 31);
                    assigned_inputs |= 1u << (input & 31);
                    break;
                }
                ++input;
            } while ((sint32)input < r_s32(0x800C6610));
        }
        ++slot;
    } while ((sint32)slot < r_s32(0x800C6610));
    return 0;
}

uint32 sub_8004833c(void)
{
    uint32 nearest[2] = {0xFFFFFFFF, 0xFFFFFFFF}, mapping[2];
    uint32 distances[2] = {0x8000, 0x8000};
    uint32 index, flags, distance, table, scene, object, target, a, b, c, d;
    FUNCTION_MARKER(0x8004833C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        flags = r_u32(0x800C6FD0 + index * 4);
        if ((~flags & 1) != 0 || (flags & 0x400000) != 0)
            continue;
        if ((r_u32(r_u32(0x800C6ED0 + index * 4) + 32) & 0x10000) != 0)
        {
            table = r_u32(0x800BCE5C);
            scene = r_u32(0x800BCE54);
            target = r_u32(scene + 40);
            object = r_u32(table + index * 4);
            distance = sub_80083dfc(target + 4, object + 8, 0, 0);
        }
        else
            distance = 0x8000;
        if ((sint32)distance < (sint32)distances[0])
        {
            distances[1] = distances[0];
            distances[0] = distance;
            nearest[1] = nearest[0];
            nearest[0] = index;
        }
        else if ((sint32)distance < (sint32)distances[1])
        {
            distances[1] = distance;
            nearest[1] = index;
        }
        if (distances[0] == 0x8000)
            nearest[0] = 0xFFFFFFFF;
        if (distances[1] == 0x8000)
            nearest[1] = 0xFFFFFFFF;
    }
    sub_8002f770(0, 0, nearest, mapping);
    index = r_u32(0x800C6638);
    if (index != 0xFFFFFFFF)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        a = r_u32(object + 8);
        b = r_u32(object + 12);
        c = r_u32(object + 16);
        d = r_u32(object + 20);
        w_u32(0x800C663C, a);
        w_u32(0x800C6640, b);
        w_u32(0x800C6644, c);
        w_u32(0x800C6648, d);
        a = r_u32(object + 24);
        b = r_u32(object + 28);
        w_u32(0x800C664C, a);
        w_u32(0x800C6650, b);
    }
    index = r_u32(0x800C6694);
    if (index != 0xFFFFFFFF)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        a = r_u32(object + 8);
        b = r_u32(object + 12);
        c = r_u32(object + 16);
        d = r_u32(object + 20);
        w_u32(0x800C6698, a);
        w_u32(0x800C669C, b);
        w_u32(0x800C66A0, c);
        w_u32(0x800C66A4, d);
        a = r_u32(object + 24);
        b = r_u32(object + 28);
        w_u32(0x800C66A8, a);
        w_u32(0x800C66AC, b);
    }
    return sub_8002f9ac(r_u32(r_u32(0x800BCE54) + 40) + 4, 0);
}

uint32 sub_800304f4(void)
{
    FUNCTION_MARKER(0x800304F4, "ACE3.BIN");
    sub_8002f500(2);
    w_u32(0x800C6628, 0);
    w_u32(0x800C6624, 0);
    w_u32(0x800C6620, 0);
    return 0;
}

uint32 sub_80049438(void)
{
    uint32 result, mode, value;
    FUNCTION_MARKER(0x80049438, "ACE3.BIN");
    if (r_s16(0x800C7194) == 1 && r_s16(0x800C7196) >= 0)
    {
        result = sub_8005650c();
        if (result != 0 || (sub_8006cb30() & 0x7000) != 0)
            w_u16(0x800C7196, 0xFFFF);
    }
    if (r_s16(0x800C7194) == 2)
    {
        result = sub_8005650c();
        mode = 2;
        if (result != 0)
        {
            sub_8002dda8(r_u8(0x800C7198));
            mode = 0;
        }
        else
        {
            result = sub_8002dd7c(r_u8(0x800C7198));
            if (result == 0 || r_u8(0x800C7198) == 255)
                mode = 1;
        }
        if (mode != 2)
        {
            sub_8004197c(mode);
            w_u16(0x800C7194, 1);
            w_u16(0x800C7196, 0xFFFF);
            w_u8(0x800C7198, 255);
        }
    }
    if (r_s16(0x800C7196) == 0 && (r_u32(r_u32(0x800BCE54) + 4) & 4) != 0 && r_s16(0x800C7194) == 1)
    {
        result = sub_8002db4c(r_u8(0x800C7199));
        w_u8(0x800C7198, result);
        value = r_u8(0x800C719A);
        value = r_u8(0x800B17E8 + value);
        sub_80041920(value, 268, 30);
        w_u16(0x800C7194, 2);
        w_u16(0x800C7196, 0xFFFF);
    }
    if (r_s16(0x800C7196) == 12 && r_s16(0x800C7194) == 1 && sub_8005650c() == 0)
        sub_8002db4c(53);
    result = r_s16(0x800C7196);
    value = r_u16(0x800C7196);
    if ((sint32)result >= 0)
    {
        result = value - 1;
        w_u16(0x800C7196, result);
    }
    return result;
}

uint32 sub_8005650c(void)
{
    FUNCTION_MARKER(0x8005650C, "ACE3.BIN");
    return r_u32(0x800BD028);
}

uint32 sub_8006cb30(void)
{
    FUNCTION_MARKER(0x8006CB30, "ACE3.BIN");
    return r_u32(r_u32(0x800BD094));
}

uint32 sub_8002dd7c(uint32 voice)
{
    FUNCTION_MARKER(0x8002DD7C, "ACE3.BIN");
    if (voice == 255)
        return 255;
    return r_u8(0x800C5830 + voice * 20 + 3);
}

uint32 sub_8004197c(uint32 mode)
{
    uint32 value;
    FUNCTION_MARKER(0x8004197C, "ACE3.BIN");
    value = r_u8(0x800C7172);
    w_u16(0x800C7170, 0);
    if (value == 0)
        return 0x800C0000;
    w_u8(0x800C7172, mode == 1 ? 3 : 0);
    w_u8(0x800C7173, 255);
    return 255;
}

uint32 sub_80041920(uint32 speaker, uint32 value, uint32 ticks)
{
    uint32 result, id;
    FUNCTION_MARKER(0x80041920, "ACE3.BIN");
    if (speaker == 0)
        return sub_8004197c(1);
    result = r_u8(0x800C7173);
    id = speaker - 1;
    if (result != id)
    {
        result = 1;
        w_u8(0x800C7172, result);
        w_u16(0x800C7170, 0);
        w_u16(0x800C7178, value);
        w_u16(0x800C717A, ticks);
        w_u8(0x800C7173, id);
    }
    return result;
}

uint32 sub_8002f9ac(uint32 position, const uint32 *native_position)
{
    uint32 voices[3], index = 0, state, descriptor, movement, control, modulation0, modulation1, source;
    uint32 result, value, flags, i, voice, a, b, c, d, volume, left, right, step, bypass;
    FUNCTION_MARKER(0x8002F9AC, "ACE3.BIN");
    result = r_u32(0x800C6610);
    if ((sint32)result <= 0)
        return result;
    do
    {
        state = 0x800C6630 + index * 92;
        descriptor = 0x800B0900 + r_u32(state + 4) * 20;
        movement = state + 36;
        control = state + 48;
        modulation0 = state + 56;
        modulation1 = state + 68;
        source = state + 12;
        voices[0] = r_u8(0x800C6614 + index);
        voices[1] = r_u8(0x800C6618 + index);
        voices[2] = r_u8(0x800C661C + index);
        if (voices[0] == 255 || voices[1] == 255 || voices[2] == 255)
            goto next_slot;
        flags = r_u32(state);
        if ((flags & 1) != 0)
        {
            w_u32(state, flags & 0xFFFFFFFE);
            sub_8002dc7c(r_u8(descriptor), voices[0]);
            sub_8002dc7c(r_u8(descriptor + 1), voices[1]);
            sub_8002dc7c(r_u8(descriptor + 2), voices[2]);
            for (i = 0; i < 3; ++i)
            {
                volume = 0x800C5830 + voices[i] * 20;
                a = r_s16(volume + 14);
                value = sub_8002e64c(a, voices[i] << 4);
                w_u16(volume + 14, value);
            }
            a = r_u8(descriptor + 3);
            b = r_u8(descriptor + 4);
            c = r_s16(descriptor + 6);
            d = r_u8(descriptor + 5);
            sub_8002ed08(control, a, b, c, d);
            sub_8002ee7c(modulation0, voices[0]);
            a = r_u8(descriptor + 8);
            b = r_u8(descriptor + 12);
            sub_8002ee88(modulation0, a, b);
            a = r_u8(descriptor + 9);
            b = r_u8(descriptor + 14);
            sub_8002eea8(modulation0, a, b);
            sub_8002ee7c(modulation1, voices[2]);
            a = r_u8(descriptor + 10);
            b = r_u8(descriptor + 16);
            sub_8002ee88(modulation1, a, b);
            a = r_u8(descriptor + 11);
            b = r_u8(descriptor + 18);
            sub_8002eea8(modulation1, a, b);
        }
        else if ((flags & 2) != 0)
        {
            w_u32(state, flags & 0xFFFFFFFD);
            sub_8002dec8(voices[0], 0, 0);
            sub_8002dec8(voices[1], 0, 0);
            sub_8002dec8(voices[2], 0, 0);
            goto next_slot;
        }
        if (r_u32(state + 8) == 0xFFFFFFFF)
        {
            sub_8002dec8(voices[0], 0, 0);
            sub_8002dec8(voices[1], 0, 0);
            sub_8002dec8(voices[2], 0, 0);
            goto next_slot;
        }
        sub_8002e6dc(movement, position, source, 0, 0, native_position);
        sub_8002dec8(voices[2], 4096, 4096);
        sub_8002eec8(modulation1);
        sub_8002e84c(voices[2], movement, 4, 0);
        sub_8002e8dc(voices[2], movement, 0);
        sub_8002e67c(voices[0]);
        sub_8002eec8(modulation0);
        sub_8002e84c(voices[0], movement, 2, 0);
        sub_8002e8dc(voices[0], movement, 0);
        sub_8002ec48(voices[0], movement);
        sub_8002ea74(voices[0], voices[1], movement);
        bypass = 0;
        step = 512;
        if (r_s32(source + 4) < -12582912)
        {
            sub_8002ed2c(movement, control);
            if (r_u8(control) == 1)
            {
                sub_8002e67c(r_u8(control + 3));
                sub_8002e8dc(r_u8(control + 3), movement, 0);
                sub_8002ec48(r_u8(control + 3), movement);
                if ((r_u32(r_u32(0x800BCE54) + 4) & 0x20) == 0)
                    bypass = 1;
                else
                {
                    voice = r_u8(control + 3);
                    volume = 0x800C5830 + voice * 20;
                    value = (uint32)(r_s16(volume + 4) / 2);
                    w_u16(volume + 4, value);
                    voice = r_u8(control + 3);
                    volume = 0x800C5830 + voice * 20;
                    value = (uint32)(r_s16(volume + 6) / 2);
                    w_u16(volume + 6, value);
                }
            }
        }
        if (bypass == 0 && (r_u32(r_u32(0x800BCE54) + 4) & 0x20) != 0)
        {
            for (i = 0; i < 3; ++i)
            {
                volume = 0x800C5830 + voices[i] * 20;
                left = (uint32)r_s16(volume + 4) * 30;
                right = (uint32)r_s16(volume + 6) * 30;
                w_u16(volume + 4, (sint32)left / 100);
                w_u16(volume + 6, (sint32)right / 100);
            }
            step = 2048;
        }
        for (i = 0; i < 3; ++i)
            sub_800300e0(voices[i], 0x800C5834 + voices[i] * 20, state + 80 + i * 4, step);
        value = r_u32(0x800C5834 + voices[0] * 20);
        w_u32(state + 80, value);
        value = r_u32(0x800C5834 + voices[1] * 20);
        w_u32(state + 84, value);
        value = r_u32(0x800C5834 + voices[2] * 20);
        w_u32(state + 88, value);
    next_slot:
        result = r_u32(0x800C6610);
        ++index;
    } while ((sint32)index < (sint32)result);
    return 0;
}

uint32 sub_8002ed08(uint32 state, uint32 a, uint32 b, uint32 c, uint32 d)
{
    FUNCTION_MARKER(0x8002ED08, "ACE3.BIN");
    w_u8(state, 0);
    w_u8(state + 1, a);
    w_u8(state + 2, b);
    w_u16(state + 4, c);
    w_u8(state + 3, 255);
    w_u8(state + 6, d);
    return 255;
}
