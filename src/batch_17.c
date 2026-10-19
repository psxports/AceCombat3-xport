#include "game_functions.h"
#include <stdlib.h>

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_80080a94(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, index, duration, random, mixed, result, px, py, pz;
    FUNCTION_MARKER(0x80080A94, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record != 0)
    {
        w_u32(record, 0x0029000A);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u32(record + 20, 0x03200002);
    }
    index = 0;
    duration = 800;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        result = index & 3;
        if (record == 0)
            return result;
        w_u16(record, 0);
        w_u16(record + 2, result + 60);
        random = game_random();
        mixed = (uint32)((sint32)random >> 8) | (random << 7);
        w_u16(record + 26, duration);
        duration += 2;
        index += 1;
        px = x + (((random & 255) - 128) << 8);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 8);
        w_u16(record + 16, ((mixed & 255) - 128) << 1);
        w_u16(record + 18, ((uint32)((sint32)mixed >> 2) & 0x1FE) - 384);
        w_u16(record + 20, (((uint32)((sint32)mixed >> 6) & 255) - 128) << 1);
        w_u16(record + 22, 398);
        w_u16(record + 24, 0x1A02);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
    } while (index < 20);
    return 0;
}

uint32 sub_80080c08(uint32 x, uint32 y, uint32 z)
{
    uint32 root, record, random, index, header, level_y;
    FUNCTION_MARKER(0x80080C08, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        random = game_random();
        index = 0;
        header = 0xFFFFFFFF;
        w_u16(record, header);
        w_u16(record + 2, (random & 7) + 40);
        w_u16(record + 22, 149);
        w_u16(record + 18, 0 - (uint32)512 - (random & 63));
        w_u16(record + 24, 528);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 16, 0);
        w_u16(record + 20, 0);
        w_u16(record + 26, (random & 127) + 720);
        do
        {
            root = r_u32(0x800BCF90);
            record = sub_8007fdb4(root + 0x600, 3, 7);
            if (record == 0)
                break;
            index += 1;
            random = game_random();
            w_u16(record, header);
            header -= 2;
            w_u16(record + 2, (random & 3) + 12);
            w_u32(record + 4, x + (((random & 255) - 128) << 14));
            w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 14));
            w_u16(record + 24, 256);
            w_u16(record + 26, ((uint32)((sint32)random >> 7) & 127) + 500);
            w_u32(record + 8, y);
            w_u16(record + 22, 0);
        } while (index < 7);
    }
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record != 0)
    {
        random = game_random();
        w_u16(record + 2, (random & 3) + 24);
        w_u16(record + 20, 1);
        w_u16(record + 22, (random & 127) + 1200);
        w_u16(record, 0);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u32(record + 16, 1024);
    }
    level_y = y - 0x50000;
    index = 0;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return 0;
        index += 1;
        random = game_random();
        w_u16(record + 2, (random & 15) + 30);
        w_u32(record + 4, x + (((random & 255) - 128) << 13));
        w_u16(record, 0);
        w_u32(record + 8, level_y);
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 4) & 255) - 128) << 13));
        random = game_random();
        w_u16(record + 16, ((random & 255) - 128) << 1);
        w_u16(record + 18, ((uint32)((sint32)random >> 2) & 0x1FE) - 576);
        w_u16(record + 20, (((uint32)((sint32)random >> 6) & 255) - 128) << 1);
        w_u16(record + 22, 141);
        w_u16(record + 24, 0x1C04);
        w_u16(record + 26, ((uint32)((sint32)random >> 8) & 127) + 800);
    } while (index < 10);
    return 0;
}

uint32 sub_80083fe0(uint32 ordering, uint32 packet)
{
    uint32 a, b, c, value, x, y, z, old_x, old_y, random, screen, depth, distance, current, count, index, output, next, end, tail, tag;
    FUNCTION_MARKER(0x80083FE0, "ACE3.BIN");
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(0x1F800008, a);
    w_u32(0x1F80000C, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(0x1F800010, a);
    w_u32(0x1F800014, b);
    w_u32(0x1F800018, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x1C00);
    xport_gte_execute(0x480012);
    w_u32(packet, ((packet + 20) & 0xFFFFFF) | 0x04000000);
    w_u32(packet + 4, 0xE1000240);
    w_u32(packet + 8, 0x620F1F1F);
    w_u32(packet + 12, 0);
    w_u32(packet + 16, 0x00F00140);
    w_u32(packet + 20, ((packet + 28) & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 24, 0xE1000220);
    random = game_random();
    w_u32(0x1F800028, random);
    w_u32(0x800CBF58, 765);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F80000C);
    w_u32(0x1F80000C, a);
    w_u32(0x1F800008, b);
    w_u16(0x1F800008, a);
    c = r_u32(0x1F800010);
    a = r_u32(0x1F800014);
    w_u32(0x1F800014, c);
    w_u32(0x1F800010, a);
    w_u16(0x1F800014, b);
    w_u16(0x1F800010, c);
    b = r_s16(0x1F800018);
    w_u16(0x1F80000C, a);
    w_u16(0x1F800018, b);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    x = 0 - x;
    y = 0 - y;
    z = 0 - z;
    w_u16(0x1F800000, (x & 0x1FFF) - 4096);
    w_u16(0x1F800002, (y & 0x1FFF) - 4096);
    w_u32(0x1F80001C, x);
    w_u32(0x1F800020, y);
    w_u32(0x1F800024, z);
    w_u16(0x1F800004, (z & 0x1FFF) - 4096);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F80000C);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(0x1F800010);
    b = r_u32(0x1F800014);
    c = r_u32(0x1F800018);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0x1C00);
    xport_gte_write_control(21, 0x360);
    xport_gte_write_control(22, 0x370);
    xport_gte_write_control(23, 0x380);
    xport_gte_write_data(6, 0x6AC0B0A0);
    count = 0;
    index = 0;
    current = packet + 28;
    do
    {
        value = r_u32(0x1F800000);
        xport_gte_write_data(0, value);
        value = r_u32(0x1F800004);
        xport_gte_write_data(1, value);
        old_x = r_u32(0x1F80001C);
        old_y = r_u32(0x1F800020);
        xport_gte_execute(0x180001);
        random = game_random();
        w_u16(0x1F800000, ((old_x + random) & 0x1FFF) - 4096);
        random = game_random();
        w_u16(0x1F800002, ((old_y + random) & 0x1FFF) - 4096);
        random = game_random();
        value = r_u16(0x1F800024);
        w_u16(0x1F800004, ((value + random) & 0x1FFF) - 4096);
        screen = xport_gte_read_data(14);
        depth = xport_gte_read_data(19);
        if ((uint32)((sint32)screen >> 16) < 241 && (uint32)((sint32)(screen << 16) >> 16) < 321)
        {
            distance = depth - 0x1C00;
            if ((sint32)distance < 0)
                distance = 0x1C00 - depth;
            if ((sint32)distance < 4096)
            {
                xport_gte_write_data(8, distance);
                output = current + 4 + (count << 3);
                xport_gte_execute(0x780010);
                count += 1;
                w_u32(output + 4, screen);
                if (count >= 7)
                {
                    next = current + 60;
                    w_u32(current, (next & 0xFFFFFF) | 0x0E000000);
                    current = next;
                    count = 0;
                }
                value = xport_gte_read_data(22);
                w_u32(output, value);
            }
        }
        index += 1;
    } while (index < 512);
    if (count != 0)
    {
        end = current + (count << 3) + 4;
        w_u32(current, count << 25);
        tail = current;
    }
    else
    {
        end = current;
        tail = packet + 20;
        if (tail + 8 != end)
            tail = end - 60;
    }
    tag = r_masked_u32(ordering, 7);
    w_masked_u32(tail, tag, 7);
    w_masked_u32(ordering, packet & 0xFFFFFF, 7);
    value = r_u32(0x1F800028);
    w_u32(0x800CBF58, value);
    return end;
}

uint32 sub_80093d1c(uint32 x, uint32 y, uint32 z, uint32 mode)
{
    uint32 root, record, random, variant, result, offset;
    FUNCTION_MARKER(0x80093D1C, "ACE3.BIN");
    if ((mode & 1) != 0)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record != 0)
        {
            random = game_random();
            variant = random & 1;
            w_u16(record, 0xFFFF);
            random = game_random();
            w_u16(record + 2, (random & 7) + (variant != 0 ? 12 : 30));
            random = game_random();
            w_u32(record + 4, x + ((random - 0x4000) << 9));
            w_u32(record + 8, y);
            random = game_random();
            w_u32(record + 12, z + ((random - 0x4000) << 9));
            w_u16(record + 22, 0);
            w_u16(record + 24, variant != 0 ? 256 : 528);
            random = game_random();
            result = variant != 0 ? (random & 0x1FF) + 1000 : (random & 255) + 1600;
            w_u16(record + 26, result);
        }
    }
    random = game_random();
    if ((random & 1) != 0)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record != 0)
        {
            w_u16(record, 0);
            random = game_random();
            w_u16(record + 2, (random & 15) + 60);
            random = game_random();
            w_u32(record + 4, x + ((random - 0x4000) << 9));
            w_u32(record + 8, y - 0x320000);
            random = game_random();
            w_u32(record + 12, z + ((random - 0x4000) << 9));
            w_u16(record + 22, 152);
            random = game_random();
            w_u16(record + 16, (random & 255) - 128);
            random = game_random();
            w_u16(record + 20, (random & 255) - 128);
            w_u16(record + 18, 0);
            w_u16(record + 24, 0);
            random = game_random();
            w_u16(record + 26, (random & 255) + 240);
        }
    }
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    if (record == 0)
        return 30;
    w_u16(record, 30);
    random = game_random();
    w_u16(record + 2, (random & 7) + 80);
    random = game_random();
    offset = (random - 0x4000) << 9;
    w_u32(record + 4, x + (uint32)((sint32)offset >> 1));
    w_u32(record + 8, y - 0xC8000);
    random = game_random();
    offset = (random - 0x4000) << 9;
    w_u32(record + 12, z + (uint32)((sint32)offset >> 1));
    w_u16(record + 22, 152);
    random = game_random();
    w_u16(record + 16, (random & 127) - 64);
    random = game_random();
    w_u16(record + 20, (random & 127) - 64);
    w_u16(record + 18, 0);
    w_u16(record + 24, 0x1E04);
    random = game_random();
    result = (random & 0x3FF) + 2000;
    w_u16(record + 26, result);
    return result;
}

uint32 sub_80093fc0(uint32 ticks)
{
    uint32 result, sine, scene, ordering, packet, root, random, value, x, y, z, parameter, product, scaled;
    FUNCTION_MARKER(0x80093FC0, "ACE3.BIN");
    if ((sint32)ticks < 11)
        return 1;
    if ((sint32)ticks >= 736)
        return 0;
    sine = rsin(ticks << 6);
    if ((sint32)sine < 0)
        sine = 0 - sine;
    if ((sint32)ticks >= 390)
        sine = (uint32)((sint32)sine >> 1) + 2048;
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    result = sub_80093c90(ordering + 8, packet, 4096 - sine);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    root = r_u32(0x800BCF90);
    w_u32(root + 0x5C8, 1);
    if ((sint32)ticks >= 391)
        return 0;
    random = game_random();
    if ((random & 1) != 0)
    {
        scene = r_u32(0x800BCE54);
        value = r_u16(scene + 656);
        w_u16(scene + 656, value + (((uint32)((sint32)random >> 1)) & 1));
        value = r_u16(scene + 658);
        w_u16(scene + 658, value + (((uint32)((sint32)random >> 3)) & 7));
    }
    if ((ticks & 1) == 0)
        return (sint32)ticks < 111;
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
    return sub_80093d1c(x, y, z, (sint32)ticks >> 1);
}

uint32 sub_80096630(uint32 vertices, uint32 workspace, uint32 context, uint32 level)
{
    uint32 a, b, current, index, flags, common, combined, maximum, minimum, depth, corner, vertex, sum, first_area, second_area, area, span, product, packet, next, color, screen;
    FUNCTION_MARKER(0x80096630, "ACE3.BIN");
    a = r_u32(vertices);
    b = r_u32(vertices + 4);
    sub_80096518(workspace, a, b);
    xport_gte_write_data(0, r_u32(workspace));
    xport_gte_write_data(1, r_u32(workspace + 4));
    a = r_u32(vertices);
    b = r_u32(vertices + 8);
    sub_80096518(workspace + 20, a, b);
    xport_gte_execute(0x180001);
    a = r_u32(vertices + 4);
    b = r_u32(vertices + 12);
    sub_80096518(workspace + 40, a, b);
    a = r_u32(vertices + 8);
    b = r_u32(vertices + 12);
    sub_80096518(workspace + 60, a, b);
    a = r_u32(vertices + 4);
    b = r_u32(vertices + 8);
    sub_80096518(workspace + 80, a, b);
    current = workspace;
    index = 0;
    do
    {
        w_u32(current + 8, xport_gte_read_data(14));
        depth = xport_gte_read_data(19);
        w_u16(current + 16, depth);
        flags = xport_gte_read_control(31);
        xport_gte_write_data(0, r_u32(current + 20));
        xport_gte_write_data(1, r_u32(current + 24));
        xport_gte_execute(0x180001);
        if ((flags & 0x7E01E000) != 0)
            w_u16(current + 6, 0xFFFF);
        else
        {
            a = sub_800965d4(current + 8);
            w_u16(current + 6, a);
        }
        index += 1;
        current += 20;
    } while (index < 4);
    w_u32(current + 8, xport_gte_read_data(14));
    depth = xport_gte_read_data(19);
    w_u16(current + 16, depth);
    flags = xport_gte_read_control(31);
    a = 0xFFFFFFFF;
    if ((flags & 0x7E01E000) == 0)
        a = sub_800965d4(current + 8);
    w_u16(current + 6, a);
    level -= 1;
    corner = 0;
    do
    {
        common = 0xFFFFFFFF;
        combined = 0;
        maximum = 0;
        minimum = 0x7FFF;
        index = 0;
        do
        {
            sum = index + corner;
            if (corner == index)
            {
                a = r_u32(vertices + corner * 4);
                w_u32(vertices + corner * 4 + 16, a);
            }
            else
            {
                if (sum == 3)
                    vertex = workspace + 80;
                else if (sum < 3)
                    vertex = workspace + sum * 20 - 20;
                else
                    vertex = workspace + sum * 20 - 40;
                w_u32(vertices + index * 4 + 16, vertex);
            }
            vertex = r_u32(vertices + index * 4 + 16);
            flags = r_s16(vertex + 6);
            depth = r_s16(vertex + 16);
            common &= flags;
            combined |= flags;
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            else if ((sint32)depth < (sint32)minimum)
                minimum = depth;
            index += 1;
        } while (index < 4);
        if (maximum - 121 >= 0x777F || common != 0)
            goto next_corner;
        if ((sint32)combined >= 0)
        {
            vertex = r_u32(vertices + 28);
            xport_gte_write_data(12, r_u32(vertex + 8));
            vertex = r_u32(vertices + 20);
            xport_gte_write_data(13, r_u32(vertex + 8));
            vertex = r_u32(vertices + 24);
            xport_gte_write_data(14, r_u32(vertex + 8));
            span = maximum - minimum;
            xport_gte_execute(0x1400006);
            first_area = xport_gte_read_data(24);
            if ((sint32)first_area >= 0)
                goto next_corner;
            vertex = r_u32(vertices + 16);
            xport_gte_write_data(12, r_u32(vertex + 8));
            first_area = 0 - first_area;
            xport_gte_execute(0x1400006);
            second_area = xport_gte_read_data(24);
            area = first_area + second_area;
            if ((sint32)second_area <= 0)
                goto next_corner;
            if ((sint32)area >= 1025)
            {
                product = area * span;
                if (((sint32)product >> 16) >= 513)
                    combined = 0xFFFFFFFF;
                else if ((sint32)combined > 0)
                {
                    a = r_u32(vertices + 16);
                    b = r_u32(vertices + 20);
                    current = r_u32(vertices + 24);
                    vertex = r_u32(vertices + 28);
                    if (sub_800963f8(r_u32(a + 8), r_u32(b + 8), r_u32(current + 8), r_u32(vertex + 8)) != 0)
                        combined = 0xFFFFFFFF;
                }
            }
        }
        if ((sint32)combined < 0 && (sint32)level >= 0)
        {
            sub_80096630(vertices + 16, workspace + 100, context, level);
            goto next_corner;
        }
        packet = r_u32(context);
        next = packet + 52;
        w_u32(packet, (next & 0xFFFFFF) | 0x0C000000);
        w_u32(context, next);
        for (index = 0; index < 4; index += 1)
        {
            vertex = r_u32(vertices + 16 + index * 4);
            color = r_u32(vertex + 12);
            screen = r_u32(vertex + 8);
            w_u32(packet + 4 + index * 12, color);
            w_u32(packet + 8 + index * 12, screen);
        }
        a = r_u16(context + 40);
        w_u16(packet + 14, a);
        a = r_u16(context + 42);
        w_u16(packet + 26, a);
        a = r_u32(vertices + 16);
        b = r_u32(vertices + 20);
        color = r_u16(a + 18);
        screen = r_u16(b + 18);
        w_u16(packet + 12, color);
        w_u16(packet + 24, screen);
        a = r_u32(vertices + 24);
        b = r_u32(vertices + 28);
        color = r_u16(a + 18);
        screen = r_u16(b + 18);
        w_u16(packet + 36, color);
        w_u16(packet + 48, screen);
    next_corner:
        corner += 1;
    } while (corner < 4);
    return 0;
}

uint32 sub_80096a48(uint32 state, uint32 ordering, uint32 packet, uint8 *native_state)
{
    uint32 scene, a, b, c, index, row, column, run, value, x, y, z, point, color, flags, depth, maximum, minimum, combined, common, first_area, second_area, area, product, previous, current, output, next, screen, result;
    FUNCTION_MARKER(0x80096A48, "ACE3.BIN");
    w_u16(0x1F800028, 0x4ED6);
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800000, packet);
    w_u16(0x1F80002A, 0x39);
    scene += 76;
    a = r_u32(scene);
    b = r_u32(scene + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(scene + 8);
    b = r_u32(scene + 12);
    c = r_u32(scene + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = r_u32(scene + 20);
    b = r_u32(scene + 24);
    xport_gte_write_control(5, a);
    c = r_u32(scene + 28);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    a = (native_state ? xport_load_le32(native_state + 72) : r_u32(state + 72));
    b = (native_state ? xport_load_le32(native_state + 56) : r_u32(state + 56));
    c = (native_state ? xport_load_le32(native_state + 76) : r_u32(state + 76));
    a += b;
    b = (native_state ? xport_load_le32(native_state + 60) : r_u32(state + 60));
    w_u16(0x1F800024, a);
    w_u16(0x1F800026, c + b);
    a = (native_state ? xport_load_le32(native_state + 88) : r_u32(state + 88));
    b = (native_state ? xport_load_le32(native_state + 92) : r_u32(state + 92));
    w_u32(0x1F800014, a);
    w_u32(0x1F800018, b);
    x = (native_state ? xport_load_le32(native_state + 0) : r_u32(state + 0));
    y = (native_state ? xport_load_le32(native_state + 4) : r_u32(state + 4));
    z = (native_state ? xport_load_le32(native_state + 8) : r_u32(state + 8));
    w_u32(0x1F800088, 0x3CA6C8C5);
    w_u32(0x1F800008, y);
    w_u32(0x1F800004, x);
    w_u32(0x1F80000C, z);
    w_u16(0x1F800094, 0 - (uint32)3072 - y);
    w_u16(0x1F800096, 0 - (uint32)2816 - y);
    w_u16(0x1F800098, 0 - (uint32)2048 - y);
    xport_gte_write_control(21, 0xC50);
    xport_gte_write_control(22, 0xC80);
    xport_gte_write_control(23, 0xA60);
    xport_gte_write_data(6, 0x3C729081);
    index = 0;
    value = 0;
    do
    {
        xport_gte_write_data(8, value);
        xport_gte_execute(0x780010);
        value += 273;
        w_u32(0x1F80004C + index * 4, xport_gte_read_data(22));
        index += 1;
    } while (index < 15);
    xport_gte_write_control(21, 0x1D0);
    xport_gte_write_control(22, 0x390);
    xport_gte_write_control(23, 0x210);
    row = r_s16(0x1F800026);
    value = r_u32(0x1F800018);
    w_u32(0x1F80001C, 0);
    if ((sint32)value <= 0)
        goto terrain_end;
    do
    {
        if (row - 87 < 74)
            w_u32(0x1F80008C, 0);
        else
        {
            if (row - 85 >= 78)
                goto next_row;
            if (row == 85)
                value = 0x10002;
            else if (row == 86)
                value = 1;
            else if (row == 161)
                value = 0x10000;
            else
                value = 0x20001;
            w_u32(0x1F80008C, value);
        }
        value = (row << 11) - r_u16(0x1F80000C);
        w_u16(0x1F800030, value);
        if (((sint32)(value << 16) >> 16) >= 30720)
            goto next_row;
        w_u16(0x1F800038, value + 2048);
        a = r_s16(0x1F80008C);
        b = r_u16(0x1F800094 + a * 2);
        a = r_s16(0x1F80008E);
        w_u16(0x1F80002E, b);
        b = r_u16(0x1F800094 + a * 2);
        w_u16(0x1F800036, b);
        index = 0;
        do
        {
            if (r_s16(0x1F80008C + index * 2) == 2)
            {
                a = 0xFFFFFFFF;
                b = 0xFFFFFFFF;
            }
            else
            {
                value = 0x800B45F4 + ((row + index - 6) & 15) * 8;
                a = r_u32(value);
                b = r_u32(value + 4);
            }
            w_u32(0x1F80003C + index * 8, a);
            w_u32(0x1F800040 + index * 8, b);
            index += 1;
        } while (index < 2);
        index = r_u32(0x1F80001C);
        value = (native_state ? xport_load_le32(native_state + 96 + index * 4) : r_u32(state + 96 + index * 4));
        column = r_s16(0x1F800024);
        run = 0;
        w_u32(0x1F800020, 0);
        w_u32(0x1F800010, value);
        if (value == 0)
            goto next_row;
        do
        {
            a = r_u32(0x1F800020);
            b = r_u32(0x1F800014);
            if ((sint32)a >= (sint32)b)
                goto next_row;
            value = r_u32(0x1F800010);
            if ((value & 1) == 0)
                goto reset_run;
            if (column - 63 < 74)
                w_u32(0x1F800090, 0);
            else
            {
                if (column - 61 >= 78 || r_u32(0x1F80008C) != 0)
                    goto reset_run;
                if (column == 61)
                    value = 0x10002;
                else if (column == 62)
                    value = 1;
                else if (column == 137)
                    value = 0x10000;
                else
                    value = 0x20001;
                w_u32(0x1F800090, value);
            }
            value = (column << 11) - r_u16(0x1F800004);
            w_u16(0x1F80002C, value);
            if (((sint32)(value << 16) >> 16) >= 30720)
                goto reset_run;
            w_u16(0x1F800034, value);
            if (run == 0)
            {
                if (r_u32(0x1F800090) != 0)
                {
                    a = r_s16(0x1F800090);
                    value = r_u16(0x1F800094 + a * 2);
                    w_u16(0x1F800036, value);
                    w_u16(0x1F80002E, value);
                }
                else if (r_u32(0x1F80008C) == 0)
                {
                    value = r_u16(0x1F800094);
                    w_u16(0x1F800036, value);
                    w_u16(0x1F80002E, value);
                }
                x = r_u32(0x1F80002C);
                z = r_u32(0x1F800030);
                xport_gte_write_data(0, x);
                xport_gte_write_data(1, z);
                point = 0x1F8000FC;
                w_u16(point + 4, z);
                xport_gte_execute(0x180001);
                w_u32(point, x);
                if (r_s16(0x1F800090) == 2)
                    color = r_u32(0x1F800088);
                else
                {
                    value = column - 4;
                    a = r_u32(0x1F80003C + (((uint32)((sint32)value >> 1)) & 4));
                    color = r_u32(0x1F80004C + ((a >> ((value & 7) * 4)) & 15) * 4);
                }
                x = r_u32(0x1F800034);
                w_u32(point + 40, x);
                flags = xport_gte_read_control(31);
                w_u32(point + 8, xport_gte_read_data(14));
                maximum = xport_gte_read_data(19);
                xport_gte_write_data(6, color);
                if ((flags & 0x7E01E000) != 0)
                {
                    w_u16(point + 6, 0xFFFF);
                    w_u32(point + 12, color);
                }
                else
                {
                    xport_gte_execute(0x780010);
                    value = sub_800965d4(point + 8);
                    w_u16(point + 6, value);
                    w_u32(point + 12, xport_gte_read_data(22));
                }
                point += 40;
                z = r_u32(0x1F800038);
                xport_gte_write_data(0, x);
                xport_gte_write_data(1, z);
                minimum = maximum;
                w_u16(point + 4, z);
                xport_gte_execute(0x180001);
                w_u16(point - 24, minimum);
                if (r_s16(0x1F800090) == 2)
                    color = r_u32(0x1F800088);
                else
                {
                    value = column - 4;
                    a = r_u32(0x1F800044 + (((uint32)((sint32)value >> 1)) & 4));
                    color = r_u32(0x1F80004C + ((a >> ((value & 7) * 4)) & 15) * 4);
                }
                flags = xport_gte_read_control(31);
                depth = xport_gte_read_data(19);
                w_u32(point + 8, xport_gte_read_data(14));
                w_u16(point + 16, depth);
                if ((sint32)maximum < (sint32)depth)
                    maximum = depth;
                else
                    minimum = depth;
                xport_gte_write_data(6, color);
                if ((flags & 0x7E01E000) != 0)
                {
                    w_u16(point + 6, 0xFFFF);
                    w_u32(point + 12, color);
                }
                else
                {
                    xport_gte_execute(0x780010);
                    value = sub_800965d4(point + 8);
                    w_u16(point + 6, value);
                    w_u32(point + 12, xport_gte_read_data(22));
                }
            }
            else
            {
                point = 0x1F8000FC + (run & 1) * 20;
                a = r_s16(point + 16);
                b = r_s16(point + 56);
                if ((sint32)a < (sint32)b)
                {
                    maximum = b;
                    minimum = a;
                }
                else
                {
                    maximum = a;
                    minimum = b;
                }
            }
            if (r_u32(0x1F800090) != 0)
            {
                a = r_s16(0x1F800092);
                value = r_u16(0x1F800094 + a * 2);
                w_u16(0x1F800036, value);
                w_u16(0x1F80002E, value);
            }
            value = r_u16(0x1F80002C) + 2048;
            z = r_u32(0x1F800030);
            w_u16(0x1F80002C, value);
            x = r_u32(0x1F80002C);
            w_u16(0x1F800034, value);
            xport_gte_write_data(0, x);
            xport_gte_write_data(1, z);
            point = 0x1F8000FC + ((run + 1) & 1) * 20;
            xport_gte_execute(0x180001);
            w_u32(point, x);
            w_u16(point + 4, z);
            if (r_s16(0x1F800092) == 2)
                color = r_u32(0x1F800088);
            else
            {
                value = column - 3;
                a = r_u32(0x1F80003C + (((uint32)((sint32)value >> 1)) & 4));
                color = r_u32(0x1F80004C + ((a >> ((value & 7) * 4)) & 15) * 4);
            }
            flags = xport_gte_read_control(31);
            depth = xport_gte_read_data(19);
            w_u32(point + 8, xport_gte_read_data(14));
            w_u16(point + 16, depth);
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            else if ((sint32)depth < (sint32)minimum)
                minimum = depth;
            xport_gte_write_data(6, color);
            if ((flags & 0x7E01E000) != 0)
            {
                w_u16(point + 6, 0xFFFF);
                w_u32(point + 12, color);
            }
            else
            {
                xport_gte_execute(0x780010);
                value = sub_800965d4(point + 8);
                w_u16(point + 6, value);
                w_u32(point + 12, xport_gte_read_data(22));
            }
            x = r_u32(0x1F800034);
            z = r_u32(0x1F800038);
            xport_gte_write_data(0, x);
            xport_gte_write_data(1, z);
            point += 40;
            w_u32(point, x);
            xport_gte_execute(0x180001);
            w_u16(point + 4, z);
            if (r_s16(0x1F800092) == 2)
                color = r_u32(0x1F800088);
            else
            {
                value = column - 3;
                a = r_u32(0x1F800044 + (((uint32)((sint32)value >> 1)) & 4));
                color = r_u32(0x1F80004C + ((a >> ((value & 7) * 4)) & 15) * 4);
            }
            run += 1;
            flags = xport_gte_read_control(31);
            depth = xport_gte_read_data(19);
            w_u32(point + 8, xport_gte_read_data(14));
            w_u16(point + 16, depth);
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            xport_gte_write_data(6, color);
            if ((flags & 0x7E01E000) != 0)
            {
                w_u16(point + 6, 0xFFFF);
                w_u32(point + 12, color);
            }
            else
            {
                xport_gte_execute(0x780010);
                value = sub_800965d4(point + 8);
                w_u16(point + 6, value);
                w_u32(point + 12, xport_gte_read_data(22));
            }
            if (maximum - 121 >= 0x777F)
                goto next_column;
            a = r_u16(0x1F800102);
            b = r_u16(0x1F800116);
            c = r_u16(0x1F80012A);
            value = r_u16(0x1F80013E);
            common = a & b & c & value;
            if (common != 0)
                goto next_column;
            combined = a | b | c | value;
            combined = (sint32)(combined << 16) >> 16;
            point -= 40;
            if ((sint32)combined >= 0)
            {
                xport_gte_write_data(15, r_u32(point + 8));
                point = 0x1F8000FC + ((run + 1) & 1) * 20;
                xport_gte_write_data(15, r_u32(point + 48));
                maximum -= combined;
                xport_gte_execute(0x1400006);
                first_area = xport_gte_read_data(24);
                if ((sint32)first_area >= 0)
                    goto next_column;
                xport_gte_write_data(12, r_u32(point + 8));
                first_area = 0 - first_area;
                xport_gte_execute(0x1400006);
                second_area = xport_gte_read_data(24);
                area = first_area + second_area;
                if ((sint32)second_area <= 0)
                    goto next_column;
                if ((sint32)area >= 1025)
                {
                    product = area * maximum;
                    if (((sint32)product >> 16) >= 513)
                        combined = 0xFFFFFFFF;
                    else if ((sint32)combined > 0 && sub_800963f8(r_u32(0x1F800104), r_u32(0x1F800118), r_u32(0x1F80012C), r_u32(0x1F800140)) != 0)
                        combined = 0xFFFFFFFF;
                }
                if ((sint32)combined >= 0)
                {
                    point = 0x1F8000FC + ((run + 1) & 1) * 20;
                    output = r_u32(0x1F800000);
                    next = output + 52;
                    w_u32(output, (next & 0xFFFFFF) | 0x0C000000);
                    w_u16(output + 36, 0xBF00);
                    w_u32(output + 12, 0x4ED68000);
                    w_u32(output + 24, 0x0039803F);
                    w_u16(output + 48, 0xBF3F);
                    color = r_u32(point + 12);
                    screen = r_u32(point + 8);
                    w_u32(output + 4, color);
                    w_u32(output + 8, screen);
                    color = r_u32(point + 52);
                    screen = r_u32(point + 48);
                    point = 0x1F8000FC + (run & 1) * 20;
                    w_u32(output + 28, color);
                    w_u32(output + 32, screen);
                    color = r_u32(point + 12);
                    screen = r_u32(point + 8);
                    w_u32(output + 16, color);
                    w_u32(output + 20, screen);
                    color = r_u32(point + 52);
                    screen = r_u32(point + 48);
                    w_u32(output + 40, color);
                    w_u32(output + 44, screen);
                    w_u32(0x1F800000, next);
                    goto next_column;
                }
            }
            current = 0x1F8000FC + ((run + 1) & 1) * 20;
            previous = 0x1F8000FC + (run & 1) * 20;
            w_u32(0x1F80009C, current);
            w_u32(0x1F8000A8, previous + 40);
            w_u32(0x1F8000A4, current + 40);
            w_u32(0x1F8000A0, previous);
            value = r_u32(0x1F80009C);
            w_u16(value + 18, 0x8000);
            value = r_u32(0x1F8000A0);
            w_u16(value + 18, 0x803F);
            value = r_u32(0x1F8000A4);
            w_u16(value + 18, 0xBF00);
            value = r_u32(0x1F8000A8);
            w_u16(value + 18, 0xBF3F);
            sub_80096630(0x1F80009C, 0x1F80014C, 0x1F800000, 4);
            goto next_column;
        reset_run:
            run = 0;
        next_column:
            column += 1;
            a = r_u32(0x1F800020);
            b = r_u32(0x1F800010);
            a += 1;
            b >>= 1;
            w_u32(0x1F800020, a);
            w_u32(0x1F800010, b);
        } while (b != 0);
    next_row:
        a = r_u32(0x1F80001C);
        b = r_u32(0x1F800018);
        a += 1;
        w_u32(0x1F80001C, a);
        row += 1;
    } while ((sint32)a < (sint32)b);
terrain_end:
    result = r_u32(0x1F800000);
    if (packet != result)
    {
        value = r_masked_u32(ordering, 7);
        w_masked_u32(result - 52, value, 7);
        w_masked_u32(ordering, packet & 0xFFFFFF, 7);
        packet = r_u32(0x1F800000);
    }
    return packet;
}

uint32 sub_80079564(uint32 mode)
{
    uint32 modes[3], scene, state, descriptor, selected;
    FUNCTION_MARKER(0x80079564, "ACE3.BIN");
    modes[0] = r_u32(0x8002474C);
    modes[1] = r_u32(0x80024750);
    modes[2] = r_u32(0x80024754);
    scene = r_u32(0x800BCE54);
    state = r_u32(scene + 40);
    descriptor = r_u32(0x800BCE5C);
    selected = modes[mode];
    descriptor = r_u32(descriptor);
    sub_800793c4(state, selected, descriptor);
    return sub_80035740(0);
}

uint32 sub_80076dec(uint32 mode, uint32 kind)
{
    uint32 modes[6], index, state, value, voice;
    FUNCTION_MARKER(0x80076DEC, "ACE3.BIN");
    index = kind * 2 + mode;
    modes[0] = r_u32(0x80024718);
    modes[1] = r_u32(0x8002471C);
    modes[2] = r_u32(0x80024720);
    modes[3] = r_u32(0x80024724);
    modes[4] = r_u32(0x80024728);
    modes[5] = r_u32(0x8002472C);
    value = modes[index];
    if (value == 0x4000 || value == 0x8000)
    {
        state = r_u32(0x800BD094);
        w_u16(state + 82, 0xFFFF);
    }
    state = r_u32(0x800BD094);
    w_u32(state, 2);
    value = modes[index];
    if (value == 0x10000)
    {
        voice = r_s16(state + 82);
        w_u32(state, 0x42);
        if ((sint32)voice >= 0)
            sub_8002dda8(voice);
    }
    else if (value == 4)
    {
        sub_80076274();
        state = r_u32(0x800BD094);
        value = r_u32(state);
        voice = r_s16(state + 82);
        w_u32(state, value | 0x280);
        if ((sint32)voice >= 0)
            sub_8002dda8(voice);
    }
    state = r_u32(0x800BD094);
    value = modes[index];
    w_u32(state + 4, value);
    w_u32(state + 8, 0);
    return value;
}

uint32 sub_80091ce8(uint32 ordering, uint32 first, uint32 second, uint32 workspace)
{
    uint32 dx, dy, angle, distance, a, b, c, value, packet, magnitude, numerator, parameter, common, output, result;
    FUNCTION_MARKER(0x80091CE8, "ACE3.BIN");
    a = r_s16(first + 12);
    b = r_s16(second + 12);
    c = r_s16(first + 14);
    value = r_s16(second + 14);
    dx = a - b;
    dy = c - value;
    angle = (uint32)ratan2(0 - dx, dy);
    w_u32(workspace + 32, angle);
    distance = sub_80083f40(dx, dy, 0);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(workspace, a);
    w_u32(workspace + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(workspace + 8, a);
    w_u32(workspace + 12, b);
    w_u32(workspace + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(workspace + 20, a);
    w_u32(workspace + 24, b);
    w_u32(workspace + 28, c);
    value = r_u16(0x1F80001A);
    xport_gte_write_data(1, value);
    xport_gte_write_data(3, value);
    xport_gte_write_data(5, value);
    xport_gte_write_control(7, 0);
    a = r_u32(first);
    b = r_u32(second);
    packet = r_u32(0x1F800004);
    dx = a - b;
    magnitude = dx;
    if ((sint32)dx < 0)
        magnitude = 0 - dx;
    if ((sint32)magnitude >= (sint32)distance)
        packet = sub_80091a70(packet, workspace, (sint32)dx < 0 ? second : first);
    else
    {
        numerator = dx << 14;
        if (distance == 0)
            xport_mips_break(7);
        if (distance == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = (sint32)numerator / (sint32)distance;
        value = sub_80030eb8(value);
        parameter = (sint32)value >> 2;
        w_u32(workspace + 36, parameter + 1024);
        packet = sub_800916a8(packet, workspace, first);
        a = r_u32(workspace + 32);
        w_u32(workspace + 36, 1024 - parameter);
        w_u32(workspace + 32, a + 2048);
        packet = sub_800916a8(packet, workspace, second);
        a = r_u32(first + 8);
        b = r_u32(second + 8);
        c = r_u32(first + 16);
        common = a & b;
        b = r_u32(second + 24);
        if ((common & c & b) == 0)
        {
            output = packet;
            packet += 36;
            w_u32(output, (packet & 0xFFFFFF) | 0x08000000);
            a = r_u32(first + 12);
            b = r_u32(second + 12);
            w_u32(output + 8, a);
            w_u32(output + 16, b);
            a = r_u32(first + 20);
            b = r_u32(second + 28);
            w_u32(output + 24, a);
            w_u32(output + 32, b);
            a = r_u32(first + 4);
            b = r_u32(second + 4);
            w_u32(output + 28, 0x20202);
            w_u32(output + 20, 0x20202);
            w_u32(output + 4, (a & 0xFFFFFF) | 0x3A000000);
            w_u32(output + 12, b);
        }
        a = r_u32(first + 24);
        b = r_u32(second + 16);
        if ((common & a & b) == 0)
        {
            output = packet;
            packet += 36;
            w_u32(output, (packet & 0xFFFFFF) | 0x08000000);
            a = r_u32(first + 12);
            b = r_u32(second + 12);
            w_u32(output + 8, a);
            w_u32(output + 16, b);
            a = r_u32(first + 28);
            b = r_u32(second + 20);
            w_u32(output + 24, a);
            w_u32(output + 32, b);
            a = r_u32(first + 4);
            b = r_u32(second + 4);
            w_u32(output + 28, 0x20202);
            w_u32(output + 20, 0x20202);
            w_u32(output + 4, (a & 0xFFFFFF) | 0x3A000000);
            w_u32(output + 12, b);
        }
    }
    result = r_u32(0x1F800004);
    if (result != packet)
    {
        value = r_masked_u32(ordering, 7);
        result <<= 8;
        w_masked_u32(packet - 36, value, 7);
        w_masked_u32(ordering, result >> 8, 7);
        w_u32(0x1F800004, packet);
    }
    a = r_u32(workspace);
    b = r_u32(workspace + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(workspace + 8);
    b = r_u32(workspace + 12);
    c = r_u32(workspace + 16);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    return result;
}

uint32 sub_80091a70(uint32 packet, uint32 workspace, uint32 vertex)
{
    uint32 radius, screen, angle, trig, center_clip, x, y, a, b, c, index, offset, mask, next;
    FUNCTION_MARKER(0x80091A70, "ACE3.BIN");
    radius = r_u32(vertex);
    screen = r_u32(vertex + 12);
    if (radius == 0)
        return packet;
    xport_gte_write_control(5, (sint32)(screen << 16) >> 16);
    angle = r_u32(workspace + 32);
    xport_gte_write_control(6, (sint32)screen >> 16);
    trig = r_u32(0x800B7104 + (angle & 0xFFF) * 4);
    xport_gte_write_data(8, radius);
    xport_gte_write_data(9, (sint32)(trig << 16) >> 16);
    xport_gte_write_data(10, (sint32)trig >> 16);
    w_u32(workspace + 40, 0x10000000);
    xport_gte_execute(0x198003D);
    center_clip = sub_800965d4(vertex + 12);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26) & 0xFFFF;
    xport_gte_write_control(0, ((0 - x) << 16) | y);
    xport_gte_write_control(2, y);
    xport_gte_write_control(1, x << 16);
    xport_gte_write_control(3, 0);
    xport_gte_write_control(4, 4096);
    index = 0;
    offset = 0x400;
    w_u32(workspace + 44, 0x0EC8061F);
    w_u32(workspace + 48, 0x0B500B50);
    do
    {
        xport_gte_write_data(0, r_u32(workspace + 40));
        xport_gte_write_data(2, r_u32(workspace + 44));
        xport_gte_write_data(4, r_u32(workspace + 48));
        a = r_u32(vertex + 4);
        b = r_u32(vertex + 12);
        xport_gte_execute(0x280030);
        w_u32(packet + 20, 0x30303);
        w_u32(packet + 28, 0x30303);
        w_u32(packet + 4, 0x3A030303);
        w_u32(packet + 12, a);
        w_u32(packet + 16, b);
        if ((index & 1) != 0)
        {
            a = r_u32(0x800B7104 + offset);
            b = r_u32(0x800B7504 + offset);
            c = r_u32(0x800B7904 + offset);
            w_u32(workspace + 40, a);
            w_u32(workspace + 44, b);
            w_u32(workspace + 48, c);
        }
        else
        {
            a = r_s16(workspace + 40);
            b = r_s16(workspace + 44);
            c = r_s16(workspace + 48);
            w_u16(workspace + 40, 0 - a);
            w_u16(workspace + 44, 0 - b);
            w_u16(workspace + 48, 0 - c);
        }
        if (center_clip != 0)
        {
            w_u32(workspace + 52, xport_gte_read_data(12));
            w_u32(workspace + 56, xport_gte_read_data(13));
            w_u32(workspace + 60, xport_gte_read_data(14));
            mask = center_clip & sub_800965d4(workspace + 52);
            if (mask != 0)
                mask &= sub_800965d4(workspace + 56);
            if (mask != 0)
                mask &= sub_800965d4(workspace + 60);
            if (mask != 0)
                goto next_sector;
            a = r_u32(workspace + 52);
            b = r_u32(workspace + 56);
            c = r_u32(workspace + 60);
            w_u32(packet + 8, a);
            w_u32(packet + 24, b);
            w_u32(packet + 32, c);
        }
        else
        {
            w_u32(packet + 8, xport_gte_read_data(12));
            w_u32(packet + 24, xport_gte_read_data(13));
            w_u32(packet + 32, xport_gte_read_data(14));
        }
        next = packet + 36;
        w_u32(packet, (next & 0xFFFFFF) | 0x08000000);
        packet = next;
    next_sector:
        index += 1;
        offset += 0x400;
    } while (index < 8);
    return packet;
}

uint32 sub_800916a8(uint32 packet, uint32 workspace, uint32 vertex)
{
    uint32 radius, screen, angle, trig, center_clip, x, y, a, b, c, spread, twice, third, two_thirds, edge_value, side_value, mask, output;
    FUNCTION_MARKER(0x800916A8, "ACE3.BIN");
    radius = r_u32(vertex);
    screen = r_u32(vertex + 12);
    if (radius == 0)
    {
        a = sub_800965d4(vertex + 12);
        w_u32(vertex + 8, a);
        a = r_u32(vertex + 8);
        b = r_u32(vertex + 12);
        w_u32(vertex + 24, a);
        w_u32(vertex + 28, b);
        a = r_u32(vertex + 24);
        b = r_u32(vertex + 28);
        w_u32(vertex + 16, a);
        w_u32(vertex + 20, b);
        return packet;
    }
    angle = r_u32(workspace + 32);
    xport_gte_write_control(5, (sint32)(screen << 16) >> 16);
    xport_gte_write_control(6, (sint32)screen >> 16);
    trig = r_u32(0x800B7104 + (angle & 0xFFF) * 4);
    xport_gte_write_data(8, radius);
    xport_gte_write_data(9, (sint32)(trig << 16) >> 16);
    xport_gte_write_data(10, (sint32)trig >> 16);
    xport_gte_execute(0x198003D);
    center_clip = sub_800965d4(vertex + 12);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26) & 0xFFFF;
    xport_gte_write_control(0, ((0 - x) << 16) | y);
    xport_gte_write_control(2, y);
    xport_gte_write_control(1, x << 16);
    xport_gte_write_control(3, 0);
    xport_gte_write_control(4, 4096);
    xport_gte_write_data(0, 0x10000000);
    w_u32(vertex + 8, center_clip);
    spread = r_u32(workspace + 36);
    xport_gte_execute(0x180001);
    twice = spread << 1;
    two_thirds = (sint32)twice / 3;
    third = (sint32)spread / 3;
    b = r_u32(0x800B7104 + (two_thirds & 0xFFF) * 4);
    c = r_u32(0x800B7104 + (spread & 0xFFF) * 4);
    a = r_u32(0x800B7104 + (third & 0xFFF) * 4);
    w_u32(workspace + 44, b);
    w_u32(workspace + 48, c);
    w_u32(workspace + 40, a);
    w_u32(workspace + 52, xport_gte_read_data(14));
    xport_gte_write_data(0, r_u32(workspace + 40));
    xport_gte_write_data(2, r_u32(workspace + 44));
    xport_gte_write_data(4, r_u32(workspace + 48));
    edge_value = r_s16(workspace + 40);
    side_value = r_s16(workspace + 44);
    xport_gte_execute(0x280030);
    a = r_s16(workspace + 48);
    w_u16(workspace + 40, 0 - edge_value);
    w_u16(workspace + 44, 0 - side_value);
    w_u16(workspace + 48, 0 - a);
    if (center_clip != 0)
    {
        a = sub_800965d4(workspace + 52);
        w_u32(workspace + 64, a);
    }
    w_u32(workspace + 56, xport_gte_read_data(12));
    w_u32(workspace + 60, xport_gte_read_data(13));
    w_u32(vertex + 20, xport_gte_read_data(14));
    xport_gte_write_data(0, r_u32(workspace + 40));
    xport_gte_write_data(2, r_u32(workspace + 44));
    xport_gte_write_data(4, r_u32(workspace + 48));
    edge_value = sub_800965d4(vertex + 20);
    xport_gte_execute(0x280030);
    w_u32(vertex + 16, edge_value);
    if (center_clip != 0)
    {
        side_value = sub_800965d4(workspace + 56);
        w_u32(workspace + 68, side_value);
        side_value &= center_clip & edge_value;
        if (side_value != 0)
            side_value &= sub_800965d4(workspace + 60);
    }
    mask = center_clip & side_value;
    if (mask == 0)
    {
        output = packet;
        packet += 36;
        w_u32(output, (packet & 0xFFFFFF) | 0x08000000);
        edge_value = r_u32(vertex + 20);
        side_value = r_u32(vertex + 12);
        a = r_u32(vertex + 4);
        w_u32(output + 8, edge_value);
        w_u32(output + 16, side_value);
        w_u32(output + 12, a);
        edge_value = r_u32(workspace + 60);
        side_value = r_u32(workspace + 56);
        w_u32(output + 28, 0x3A020202);
        w_u32(output + 20, 0x3A020202);
        w_u32(output + 4, 0x3A020202);
        w_u32(output + 24, edge_value);
        w_u32(output + 32, side_value);
    }
    w_u32(workspace + 60, xport_gte_read_data(12));
    mask = center_clip & edge_value;
    if (center_clip != 0)
    {
        center_clip &= sub_800965d4(workspace + 60);
        edge_value = r_u32(workspace + 64);
        side_value = r_u32(workspace + 68);
        mask = center_clip & edge_value;
    }
    if ((mask & side_value) == 0)
    {
        output = packet;
        packet += 36;
        w_u32(output, (packet & 0xFFFFFF) | 0x08000000);
        edge_value = r_u32(workspace + 56);
        side_value = r_u32(vertex + 12);
        a = r_u32(vertex + 4);
        w_u32(output + 8, edge_value);
        w_u32(output + 16, side_value);
        w_u32(output + 12, a);
        edge_value = r_u32(workspace + 52);
        side_value = r_u32(workspace + 60);
        w_u32(output + 28, 0x3A020202);
        w_u32(output + 20, 0x3A020202);
        w_u32(output + 4, 0x3A020202);
        w_u32(output + 24, edge_value);
        w_u32(output + 32, side_value);
    }
    w_u32(workspace + 56, xport_gte_read_data(13));
    w_u32(vertex + 28, xport_gte_read_data(14));
    a = sub_800965d4(vertex + 28);
    center_clip &= a;
    w_u32(vertex + 24, a);
    if (center_clip != 0)
    {
        center_clip &= sub_800965d4(workspace + 56);
        if (center_clip != 0)
            return packet;
    }
    output = packet;
    packet += 36;
    w_u32(output, (packet & 0xFFFFFF) | 0x08000000);
    edge_value = r_u32(vertex + 28);
    side_value = r_u32(vertex + 12);
    a = r_u32(vertex + 4);
    w_u32(output + 8, edge_value);
    w_u32(output + 16, side_value);
    w_u32(output + 12, a);
    edge_value = r_u32(workspace + 56);
    side_value = r_u32(workspace + 60);
    w_u32(output + 28, 0x3A020202);
    w_u32(output + 20, 0x3A020202);
    w_u32(output + 4, 0x3A020202);
    w_u32(output + 24, edge_value);
    w_u32(output + 32, side_value);
    return packet;
}

uint32 sub_80092168(uint32 first, uint32 second, uint32 near_plane, uint32 workspace)
{
    uint32 first_depth, second_depth, numerator, divisor, weight, h, index, first_side, second_side;
    FUNCTION_MARKER(0x80092168, "ACE3.BIN");
    first_depth = r_u32(first + 8);
    second_depth = r_u32(second + 8);
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
        sub_8009211c(first, second, (sint32)first_depth < (sint32)second_depth ? first : second, weight);
    }
    h = xport_gte_read_control(26);
    w_u32(workspace + 16, 0 - (uint32)184);
    w_u32(workspace + 32, 0 - (uint32)184);
    w_u32(workspace, 184);
    w_u32(workspace + 4, 144);
    w_u32(workspace + 8, h);
    w_u32(workspace + 20, 144);
    w_u32(workspace + 24, h);
    w_u32(workspace + 36, 0 - (uint32)144);
    w_u32(workspace + 40, h);
    w_u32(workspace + 48, 184);
    w_u32(workspace + 52, 0 - (uint32)144);
    w_u32(workspace + 56, h);
    index = 0;
    do
    {
        OuterProduct0((VECTOR *)psx_addr(workspace + index * 16, 12), (VECTOR *)psx_addr(workspace + ((index + 1) & 3) * 16, 12), (VECTOR *)psx_addr(workspace + 64, 12));
        first_side = sub_80092014(workspace + 64, first);
        second_side = sub_80092014(workspace + 64, second);
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
            sub_8009211c(first, second, (sint32)first_side < (sint32)second_side ? first : second, weight);
        }
        index += 1;
    } while (index < 4);
    return 1;
}

uint32 sub_8009236c(uint32 context, uint32 matrix, uint32 record)
{
    uint32 kind, phase, shift, origin_x, origin_y, origin_z, x, y, z, value, near_plane, a, b, c, first_depth, second_depth, bucket, intensity, numerator, divisor, first_radius, second_radius, color, inner_color, ordering;
    FUNCTION_MARKER(0x8009236C, "ACE3.BIN");
    kind = r_s16(record + 2);
    phase = r_s16(record);
    shift = kind == 2 ? 15 : 12;
    value = r_u32(record + 4);
    origin_x = r_u32(context + 8);
    x = (sint32)(value - origin_x) >> shift;
    if (x + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(record + 8);
    origin_y = r_u32(context + 12);
    y = (sint32)(value - origin_y) >> shift;
    if (y + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(record + 12);
    origin_z = r_u32(context + 16);
    z = (sint32)(value - origin_z) >> shift;
    if (z + 0x8000 > 0xFFFF)
        return 1;
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    value = r_u32(record + 16);
    x = (sint32)(value - origin_x) >> shift;
    xport_gte_execute(0x49E012);
    if (x + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(record + 20);
    y = (sint32)(value - origin_y) >> shift;
    if (y + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(record + 24);
    z = (sint32)(value - origin_z) >> shift;
    if (z + 0x8000 > 0xFFFF)
        return 1;
    w_u32(matrix + 4, xport_gte_read_data(25));
    w_u32(matrix + 8, xport_gte_read_data(26));
    w_u32(matrix + 12, xport_gte_read_data(27));
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    value = r_u16(context + 26);
    near_plane = (sint32)(value << 16) >> 18;
    xport_gte_execute(0x49E012);
    if ((sint32)near_plane <= 0)
        near_plane = 1;
    w_u32(matrix + 20, xport_gte_read_data(25));
    w_u32(matrix + 24, xport_gte_read_data(26));
    w_u32(matrix + 28, xport_gte_read_data(27));
    if (sub_80092168(matrix + 4, matrix + 20, near_plane, matrix + 36) == 0)
        return 4096;
    w_u32(matrix + 284, 4096);
    w_u32(matrix + 292, 4096);
    w_u32(matrix + 300, 4096);
    w_u32(matrix + 288, 0);
    w_u32(matrix + 296, 0);
    a = r_u32(matrix + 284);
    b = r_u32(matrix + 288);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 292);
    b = r_u32(matrix + 296);
    c = r_u32(matrix + 300);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    y = r_u32(matrix + 8);
    first_depth = r_u32(matrix + 12);
    x = r_u16(matrix + 4);
    xport_gte_write_data(0, x | (y << 16));
    xport_gte_write_data(1, first_depth);
    y = r_u32(matrix + 24);
    second_depth = r_u32(matrix + 28);
    x = r_u16(matrix + 20);
    xport_gte_write_data(2, x | (y << 16));
    xport_gte_write_data(3, second_depth);
    bucket = (sint32)first_depth < (sint32)second_depth ? first_depth : second_depth;
    xport_gte_execute(0x280030);
    bucket = (sint32)bucket >> ((15 - shift) & 31);
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    value = r_u32(context);
    w_u32(matrix, value + (bucket << 2));
    if (kind == 3)
        intensity = phase;
    else
    {
        intensity = r_s16(0x800B7104 + (phase & 0xFFF) * 4);
        if (intensity == 0)
            intensity = 0 - (uint32)4096;
    }
    w_u32(matrix + 128, xport_gte_read_data(12));
    w_u32(matrix + 160, xport_gte_read_data(13));
    a = r_u32(matrix + 252);
    b = r_u32(matrix + 256);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 260);
    b = r_u32(matrix + 264);
    c = r_u32(matrix + 268);
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
        a = r_u8(record + 4);
        b = r_s16(context + 26);
        numerator = a * b;
        divisor = first_depth;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        first_radius = (sint32)numerator / (sint32)divisor;
        a = r_u8(record + 8);
        numerator = a * b;
    }
    else
    {
        a = kind == 2 ? 12 : 8;
        b = r_s16(context + 26);
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
    w_u32(matrix + 116, first_radius);
    w_u32(matrix + 148, second_radius);
    color = xport_gte_read_data(20);
    inner_color = xport_gte_read_data(21);
    ordering = r_u32(matrix);
    w_u32(matrix + 152, color);
    w_u32(matrix + 120, color);
    sub_80091ce8(ordering, matrix + 116, matrix + 148, matrix + 180);
    ordering = r_u32(matrix);
    w_u32(matrix + 116, (sint32)first_radius >> 1);
    w_u32(matrix + 152, inner_color);
    w_u32(matrix + 120, inner_color);
    w_u32(matrix + 148, (sint32)second_radius >> 1);
    return sub_80091ce8(ordering, matrix + 116, matrix + 148, matrix + 180);
}

uint32 sub_800304ec(void)
{
    FUNCTION_MARKER(0x800304EC, "ACE3.BIN");
    return 0;
}

void sub_80031f5c(void)
{
    FUNCTION_MARKER(0x80031F5C, "ACE3.BIN");
}

void sub_80031fa0(void)
{
    FUNCTION_MARKER(0x80031FA0, "ACE3.BIN");
}

void sub_80032070(void)
{
    FUNCTION_MARKER(0x80032070, "ACE3.BIN");
}

void sub_80032078(void)
{
    FUNCTION_MARKER(0x80032078, "ACE3.BIN");
}

uint32 sub_8007754c(uint32 state, uint32 input)
{
    MATRIX matrix;
    uint32 value, raw, pointer, pitch, yaw, roll, target_pitch, target_yaw, target_roll, speed, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x8007754C, "ACE3.BIN");
    if ((input & 0x40) != 0)
    {
        value = r_s16(state + 212);
        raw = r_u16(state + 212);
        if ((sint32)value < 4096)
            w_u16(state + 212, raw + 128);
    }
    if ((input & 0x10) != 0)
    {
        value = r_s16(state + 212);
        raw = r_u16(state + 212);
        if ((sint32)value >= -4095)
            w_u16(state + 212, raw - 128);
    }
    if ((input & 0x80) != 0)
    {
        raw = r_u16(state + 214);
        value = raw - 128;
        w_u16(state + 214, value);
        if (((sint32)(value << 16) >> 16) < -8192)
            w_u16(state + 214, raw + 0x3F80);
    }
    if ((input & 0x20) != 0)
    {
        raw = r_u16(state + 214);
        value = raw + 128;
        w_u16(state + 214, value);
        if (((sint32)(value << 16) >> 16) >= 8193)
            w_u16(state + 214, raw - 0x3F80);
    }
    if ((input & 0x400) != 0)
    {
        speed = r_u32(state + 228);
        if ((sint32)speed <= 0x31FFFF)
            w_u32(state + 228, speed + 0xA000);
    }
    if ((input & 0x800) != 0)
    {
        speed = r_u32(state + 228);
        if ((sint32)speed > 0)
            w_u32(state + 228, speed - 0xA000);
    }
    pointer = r_u32(state + 52);
    pitch = r_s16(state + 212);
    yaw = r_s16(state + 214);
    roll = r_s16(state + 216);
    target_pitch = r_s16(pointer + 16);
    target_yaw = r_s16(pointer + 18);
    target_roll = r_s16(pointer + 20);
    speed = r_u32(state + 228);
    z = (sint32)(0 - speed) >> 7;
    pitch -= target_pitch;
    yaw -= target_yaw;
    roll -= target_roll;
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
    w_u32(state + 56, rx << 7);
    w_u32(state + 60, ry << 7);
    w_u16(state + 72, pitch);
    w_u16(state + 74, yaw);
    w_u16(state + 76, roll);
    value = rz << 7;
    w_u32(state + 64, value);
    return value;
}

uint32 sub_80075b04(uint32 object, uint32 state, uint32 control, uint32 smooth, uint32 target, uint32 option)
{
    MATRIX matrix;
    uint32 phase, ticks, value, voice_state, voice, acceleration, a, b, c, d, e, rx, ry, rz, vx, vy, vz, x, y, z;
    FUNCTION_MARKER(0x80075B04, "ACE3.BIN");
    phase = r_u32(state + 8);
    if (phase == 0)
    {
        sub_8007599c(object, state, target);
        w_u32(state + 72, 0);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u16(state + 48, 0);
        w_u16(state + 50, 0);
        w_u16(state + 52, 0);
        w_u16(state + 56, 0);
        w_u16(state + 58, 0);
        w_u16(state + 60, 0);
        w_u16(state + 76, 0);
        w_u16(control + 4, 0);
        w_u16(control + 6, 0);
        w_u16(control + 8, 0);
        w_u16(control + 10, 0);
        w_u16(control + 12, 0);
        w_u16(control + 14, 0xF000);
        w_u16(smooth, 0);
        w_u16(smooth + 2, 0);
        w_u16(smooth + 4, 0);
        w_u16(smooth + 6, 0xF000);
        w_u16(smooth + 8, 0);
        w_u32(state + 12, 0);
        w_u32(state + 8, 1);
    }
    else if (phase == 1)
    {
        ticks = r_u32(state + 12);
        value = ticks << 13;
        value = (uint32)((sint32)value / 90) - 4096;
        w_u16(control + 14, value);
        if (((sint32)(value << 16) >> 16) >= 4097)
            w_u16(control + 14, 4096);
        value = r_u16(control + 14);
        w_u16(smooth + 6, value);
        ticks = r_u32(state + 12) + 1;
        w_u32(state + 12, ticks);
        if ((sint32)ticks >= 91)
        {
            voice_state = r_u32(0x800BD094);
            w_u32(state + 8, 2);
            w_u32(state + 12, 0);
            voice = r_s16(voice_state + 82);
            if ((sint32)voice >= 0)
                sub_8002dda8(voice);
            voice = sub_8002db4c(92);
            voice_state = r_u32(0x800BD094);
            w_u16(voice_state + 82, voice);
        }
    }
    else if (phase == 2)
    {
        acceleration = option != 0 ? 780 : 284;
        value = r_u32(state + 72);
        w_u32(state + 72, value + acceleration);
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
        xport_gte_write_data(1, acceleration & 0xFFFF);
        xport_gte_execute(0x486012);
        rx = xport_gte_read_data(25);
        ry = xport_gte_read_data(26);
        rz = xport_gte_read_data(27);
        value = r_u32(object + 56);
        w_u32(object + 56, value + rx);
        value = r_u32(object + 60);
        vx = r_u32(object + 56);
        value += ry;
        vz = r_u32(object + 64);
        w_u32(object + 60, value);
        x = r_u32(object + 8);
        x += vx;
        w_u32(object + 8, x);
        y = r_u32(object + 12);
        vy = r_u32(object + 60);
        vz += rz;
        w_u32(object + 64, vz);
        z = r_u32(object + 16);
        value = r_u32(object + 64);
        y += vy;
        z += value;
        w_u32(object + 12, y);
        w_u32(object + 16, z);
        value = r_u32(state + 72);
        if ((sint32)value > 0xF000)
        {
            voice_state = r_u32(0x800BD094);
            w_u32(state + 8, 3);
            w_u32(state + 12, 0);
            voice = r_s16(voice_state + 82);
            if ((sint32)voice >= 0)
                sub_8002dda8(voice);
        }
    }
    else if (phase == 3)
        return 1;
    sub_8006ee38(object, state);
    return 0;
}

uint32 sub_8002d71c(void)
{
    FUNCTION_MARKER(0x8002D71C, "ACE3.BIN");
    return SpuStart();
}

void sub_8002d73c(void)
{
    FUNCTION_MARKER(0x8002D73C, "ACE3.BIN");
    sub_8009fd68();
}

void sub_8002d8c8(uint32 enabled)
{
    FUNCTION_MARKER(0x8002D8C8, "ACE3.BIN");
    w_u32(0x800C5800, 256);
    w_u32(0x800C5814, enabled != 0);
    SpuSetCommonAttr(psx_addr(0x800C5800, sizeof(SpuCommonAttr)));
}

uint32 sub_8002da60(void)
{
    SpuReverbAttr reverb = {0};
    FUNCTION_MARKER(0x8002DA60, "ACE3.BIN");
    reverb.mask = 1;
    reverb.mode = 0;
    SpuSetReverbModeParam(&reverb);
    return SpuSetReverb(0);
}

uint32 sub_80077004(uint32 position, uint32 angles, uint32 control, uint32 input)
{
    uint32 pitch, yaw, roll, x, y, z, shift, value, delta;
    FUNCTION_MARKER(0x80077004, "ACE3.BIN");
    pitch = 0;
    yaw = 0;
    roll = 0;
    x = 0;
    y = 0;
    z = 0;
    shift = (input << 1) & 4;
    if ((input & 0x10) != 0)
        pitch -= 64;
    if ((input & 0x40) != 0)
        pitch += 64;
    if ((input & 0x20) != 0)
        roll += 64;
    if ((input & 0x80) != 0)
        roll -= 64;
    if ((input & 0x200) != 0)
        yaw += 64;
    if ((input & 0x100) != 0)
        yaw -= 64;
    if ((input & 0x14) == 0x14)
        pitch += 60;
    if ((input & 0x44) == 0x44)
        pitch -= 60;
    if ((input & 0x24) == 0x24)
        roll -= 60;
    if ((input & 0x84) == 0x84)
        roll += 60;
    if ((input & 0x204) == 0x204)
        yaw -= 60;
    if ((input & 0x104) == 0x104)
        yaw += 60;
    pitch = (pitch << shift) & 0xFFFF;
    roll = (roll << shift) & 0xFFFF;
    yaw = (yaw << shift) & 0xFFFF;
    if ((input & 0x1000) != 0)
        z += 0x40000;
    if ((input & 0x4000) != 0)
        z -= 0x40000;
    if ((input & 0x8000) != 0)
        x -= 0x40000;
    if ((input & 0x2000) != 0)
        x += 0x40000;
    if ((input & 0x400) != 0)
        y -= 0x40000;
    if ((input & 0x800) != 0)
        y += 0x40000;
    if ((input & 0x1004) == 0x1004)
        z -= 0x3F000;
    if ((input & 0x4004) == 0x4004)
        z += 0x3F000;
    if ((input & 0x8004) == 0x8004)
        x += 0x3F000;
    if ((input & 0x2004) == 0x2004)
        x -= 0x3F000;
    if ((input & 0x404) == 0x404)
        y += 0x3F000;
    if ((input & 0x804) == 0x804)
        y -= 0x3F000;
    x <<= shift;
    z <<= shift;
    y <<= shift;
    w_u32(angles, x);
    w_u32(angles + 4, y);
    w_u32(angles + 8, z);
    w_u32(angles + 12, 0);
    w_u32(angles + 16, pitch | (yaw << 16));
    w_u32(angles + 20, roll);
    sub_80030f64(position + 16, angles + 16, NULL);
    sub_80030ef8(angles, position + 16, control);
    if ((input & 0xC00) == 0xC00)
    {
        w_u16(position + 16, 0xF000);
        w_u16(position + 18, 0);
        w_u16(position + 20, 0);
    }
    if ((input & 6) == 6)
    {
        w_u16(position + 16, 0);
        w_u16(position + 18, 0);
        w_u16(position + 20, 0);
    }
    value = r_u32(position);
    delta = r_u32(control);
    w_u32(position, value + delta);
    value = r_u32(position + 4);
    delta = r_u32(control + 4);
    w_u32(position + 4, value + delta);
    value = r_u32(position + 8);
    delta = r_u32(control + 8);
    value += delta;
    w_u32(position + 8, value);
    return value;
}

uint32 sub_8007407c(uint32 object, uint32 output, uint32 control, uint32 smooth)
{
    uint32 pitch_target, bank_target, thrust_target, roll, absolute_roll, value, magnitude, pitch_limit, bank_limit, numerator, steps[3], targets[3], index, address, raw;
    FUNCTION_MARKER(0x8007407C, "ACE3.BIN");
    thrust_target = 0;
    pitch_target = 0;
    roll = r_s16(object + 28);
    bank_target = (0 - roll) << 2;
    absolute_roll = roll;
    if ((sint32)roll < 0)
        absolute_roll = 0 - roll;
    if ((sint32)absolute_roll < 2048)
    {
        value = r_s16(object + 24);
        if (value != 2048)
            pitch_target = (2048 - value) << 4;
        thrust_target = 4096;
    }
    value = r_s16(object + 28);
    absolute_roll = value;
    if ((sint32)value < 0)
        absolute_roll = 0 - value;
    if ((sint32)absolute_roll < 455)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)absolute_roll < 273)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)absolute_roll < 136)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)pitch_target < -4096)
        pitch_target = 0 - (uint32)4096;
    else if ((sint32)pitch_target >= 4097)
        pitch_target = 4096;
    if ((sint32)bank_target < -4096)
        bank_target = 0 - (uint32)4096;
    else if ((sint32)bank_target >= 4097)
        bank_target = 4096;
    pitch_limit = pitch_target;
    if ((sint32)pitch_target < 0)
        pitch_limit = 0 - pitch_target;
    bank_limit = bank_target;
    if ((sint32)bank_target < 0)
        bank_limit = 0 - bank_target;
    value = r_s16(output);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)pitch_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output, (sint32)numerator / 200);
    }
    value = r_s16(output + 2);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if (magnitude != 0)
    {
        numerator = value << 7;
        w_u16(output + 2, (sint32)numerator / 200);
    }
    value = r_s16(output + 4);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)bank_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output + 4, (sint32)numerator / 200);
    }
    value = r_s16(output + 6);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)thrust_target < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output + 6, (sint32)numerator / 140);
    }
    targets[0] = bank_target;
    targets[1] = pitch_target;
    targets[2] = 0;
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = smooth + index * 2;
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)targets[index] < (sint32)value)
            w_u16(address, raw - steps[index]);
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)value < (sint32)targets[index])
            w_u16(address, raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            w_u16(smooth + index * 2, targets[index]);
    }
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < 0)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value > 0)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(output + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    value = r_u16(output);
    raw = r_u16(output + 2);
    w_u16(output, value + (uint32)((sint32)pitch_target >> 3));
    value = r_u16(output + 4);
    w_u16(output + 4, value + (uint32)((sint32)bank_target >> 5));
    value = r_u16(output + 6);
    w_u16(output + 6, value + (uint32)((sint32)thrust_target >> 5));
    value = r_s16(output);
    if ((sint32)value < (sint32)(0 - pitch_limit))
        w_u16(output, 0 - pitch_limit);
    value = r_s16(output);
    if ((sint32)pitch_limit < (sint32)value)
        w_u16(output, pitch_limit);
    value = r_s16(output + 2);
    if ((sint32)value < 0)
        w_u16(output + 2, 0);
    value = r_s16(output + 2);
    if ((sint32)value > 0)
        w_u16(output + 2, 0);
    value = r_s16(output + 4);
    if ((sint32)value < (sint32)(0 - bank_limit))
        w_u16(output + 4, 0 - bank_limit);
    value = r_s16(output + 4);
    if ((sint32)bank_limit < (sint32)value)
        w_u16(output + 4, bank_limit);
    value = r_s16(output + 6);
    if ((sint32)value < -4096)
        w_u16(output + 6, 0xF000);
    value = r_s16(output + 6);
    if ((sint32)value >= 4097)
        w_u16(output + 6, 4096);
    value = r_u16(output);
    w_u16(control + 4, value);
    value = r_u16(output + 2);
    w_u16(control + 6, value);
    value = r_u16(output + 4);
    w_u16(control + 8, value);
    value = r_u16(output + 6);
    w_u16(control + 14, value);
    return value;
}

uint32 sub_8006d678(uint32 input, uint32 output, uint32 control, uint32 smooth)
{
    uint32 table, object, buttons, flags, root, value, raw, magnitude, fallback, pitch, yaw, roll, thrust, pitch_limit, yaw_limit, roll_limit, targets[4], limits[3], smoothing[3], steps[3], index, address, numerator, desired, result;
    FUNCTION_MARKER(0x8006D678, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    buttons = r_u32(input + 8);
    object = r_u32(table);
    if ((sint32)buttons < 0)
    {
        root = r_u32(0x800BCE60);
        fallback = r_u32(root + 4) != 0;
        if (fallback == 0)
        {
            value = r_s16(object + 28);
            if ((sint32)value < 0)
                value = 0 - value;
            fallback = (sint32)value >= 547;
        }
        if (fallback != 0)
        {
            root = r_u32(0x800BD094);
            flags = r_u32(root + 4);
            if (flags == 0x01000000)
                return sub_80073b18(object, output, control, smooth, 0);
            return sub_8007407c(object, output, control, smooth);
        }
    }
    pitch_limit = 4096;
    yaw_limit = 4096;
    roll_limit = 4096;
    pitch = 0;
    yaw = 0;
    roll = 0;
    thrust = 0;
    flags = r_u32(input + 24);
    buttons = r_u32(input + 8);
    if ((flags & 1) != 0)
    {
        pitch = r_u32(input + 28);
        pitch_limit = pitch;
        if ((sint32)pitch < 0)
            pitch_limit = 0 - pitch;
    }
    else
    {
        if ((buttons & 0x00800000) != 0)
            pitch = 4096;
        if ((buttons & 0x01000000) != 0)
            pitch -= 4096;
    }
    flags = r_u32(input + 24);
    if ((flags & 4) != 0)
    {
        yaw = r_u32(input + 36);
        yaw_limit = yaw;
        if ((sint32)yaw < 0)
            yaw_limit = 0 - yaw;
    }
    else
    {
        if ((buttons & 0x20000000) != 0)
            yaw -= yaw_limit;
        if ((buttons & 0x40000000) != 0)
            yaw += yaw_limit;
    }
    flags = r_u32(input + 24);
    if ((flags & 2) != 0)
    {
        roll = r_u32(input + 32);
        roll_limit = roll;
        if ((sint32)roll < 0)
            roll_limit = 0 - roll;
    }
    else
    {
        if ((buttons & 0x02000000) != 0)
            roll -= roll_limit;
        if ((buttons & 0x04000000) != 0)
            roll += roll_limit;
    }
    flags = r_u32(input + 24);
    if ((flags & 8) != 0)
        thrust = r_u32(input + 40);
    else
    {
        if ((buttons & 0x08000000) != 0)
            thrust += 4096;
        if ((buttons & 0x10000000) != 0)
            thrust -= 4096;
    }
    targets[0] = pitch;
    targets[1] = yaw;
    targets[2] = roll;
    targets[3] = thrust;
    for (index = 0; index < 4; index += 1)
    {
        value = r_s16(output + index * 2);
        magnitude = targets[index];
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        raw = value;
        if ((sint32)value < 0)
            raw = 0 - value;
        if ((sint32)magnitude < (sint32)raw)
        {
            numerator = value << 7;
            w_u16(output + index * 2, (sint32)numerator / (index == 3 ? 140 : 220));
        }
    }
    smoothing[0] = roll;
    smoothing[1] = pitch;
    smoothing[2] = yaw;
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - smoothing[index];
        if ((sint32)magnitude < 0)
            magnitude = smoothing[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = smooth + index * 2;
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)smoothing[index] < (sint32)value)
            w_u16(address, raw - steps[index]);
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)value < (sint32)smoothing[index])
            w_u16(address, raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - smoothing[index];
        if ((sint32)magnitude < 0)
            magnitude = smoothing[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            w_u16(smooth + index * 2, smoothing[index]);
    }
    desired = 0;
    if ((sint32)thrust < -2048)
        desired = (0 - (thrust + 2048)) << 1;
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < (sint32)desired)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)desired < (sint32)value)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(output + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    for (index = 0; index < 4; index += 1)
    {
        value = r_u16(output + index * 2);
        w_u16(output + index * 2, value + (uint32)((sint32)targets[index] >> 5));
    }
    limits[0] = pitch_limit;
    limits[1] = yaw_limit;
    limits[2] = roll_limit;
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(output + index * 2);
        if ((sint32)value < (sint32)(0 - limits[index]))
            w_u16(output + index * 2, 0 - limits[index]);
        value = r_s16(output + index * 2);
        if ((sint32)limits[index] < (sint32)value)
            w_u16(output + index * 2, limits[index]);
    }
    value = r_s16(output + 6);
    if ((sint32)value < -4096)
        w_u16(output + 6, 0xF000);
    value = r_s16(output + 6);
    if ((sint32)value >= 4097)
        w_u16(output + 6, 4096);
    value = r_u16(output + 6);
    root = r_u32(0x800BCE60);
    w_u16(control + 14, value);
    value = r_u16(output);
    w_u16(control + 4, value);
    flags = r_u32(root + 288);
    if ((flags & 4) != 0)
    {
        value = r_u16(output + 4);
        w_u16(control + 6, value);
        value = r_s16(output + 4);
        raw = r_u16(output + 2);
        value = raw + (uint32)((sint32)value >> 5);
        w_u16(control + 8, value);
        if (((sint32)(value << 16) >> 16) >= 4097)
            w_u16(control + 8, 4096);
        value = r_s16(control + 8);
        if ((sint32)value < -4096)
            w_u16(control + 8, 0xF000);
        table = r_u32(0x800BCE5C);
        object = r_u32(table);
        value = r_s16(object + 28);
        raw = r_u16(object + 28);
        if ((sint32)value >= 13)
            w_u16(object + 28, raw - 12);
        value = r_s16(object + 28);
        raw = r_u16(object + 28);
        result = raw + 12;
        if ((sint32)value < -12)
            w_u16(object + 28, result);
        return result;
    }
    value = r_u16(output + 2);
    w_u16(control + 6, value);
    result = r_u16(output + 4);
    w_u16(control + 8, result);
    return result;
}

uint32 sub_800702d8(uint32 object, uint32 control, uint32 settings, uint32 state)
{
    MATRIX matrix;
    uint32 pitch_input, yaw_input, roll_input, pitch_limit, yaw_limit, roll_limit, product, pitch_step, yaw_step, roll_step, pitch, yaw, roll, saved[3], steps[3], values[3], index, magnitude, absolute_step, value, a, b, c, d, e, vx, vy, vz, speed, x, y, z, forward, thrust, ax, ay, az;
    FUNCTION_MARKER(0x800702D8, "ACE3.BIN");
    pitch_input = r_s16(control + 4);
    pitch_limit = r_u32(settings);
    product = pitch_input * pitch_limit;
    yaw_input = r_s16(control + 6);
    roll_input = r_s16(control + 8);
    pitch_step = (sint32)product / 4096;
    yaw_limit = r_u32(settings + 8);
    product = yaw_input * yaw_limit;
    yaw_step = (sint32)product / 4096;
    roll_limit = r_u32(settings + 12);
    product = roll_input * roll_limit;
    roll_step = (sint32)product / 4096;
    pitch = r_s16(state + 48);
    roll = r_s16(state + 52);
    pitch += pitch_step;
    yaw = r_s16(state + 50);
    yaw += yaw_step;
    roll += roll_step;
    if ((sint32)pitch < (sint32)(0 - pitch_limit))
        pitch = 0 - pitch_limit;
    else if ((sint32)pitch_limit < (sint32)pitch)
        pitch = pitch_limit;
    yaw_limit = r_u32(settings + 8);
    if ((sint32)yaw < (sint32)(0 - yaw_limit))
        yaw = 0 - yaw_limit;
    else if ((sint32)yaw_limit < (sint32)yaw)
        yaw = yaw_limit;
    roll_limit = r_u32(settings + 12);
    if ((sint32)roll < (sint32)(0 - roll_limit))
        roll = 0 - roll_limit;
    else if ((sint32)roll_limit < (sint32)roll)
        roll = roll_limit;
    saved[0] = pitch;
    saved[1] = yaw;
    saved[2] = roll;
    values[0] = pitch;
    values[1] = yaw;
    values[2] = roll;
    steps[0] = pitch_step;
    steps[1] = yaw_step;
    steps[2] = roll_step;
    for (index = 0; index < 3; index += 1)
    {
        absolute_step = steps[index];
        if ((sint32)absolute_step < 0)
            absolute_step = 0 - absolute_step;
        magnitude = values[index];
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)absolute_step < (sint32)magnitude)
        {
            product = values[index] * 3967;
            values[index] = (sint32)product / 4096;
            magnitude = values[index];
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude < (sint32)absolute_step)
                values[index] = steps[index];
        }
    }
    w_u16(state + 48, values[0]);
    w_u16(state + 50, values[1]);
    w_u16(state + 52, values[2]);
    w_u16(object + 48, saved[0]);
    w_u16(object + 50, saved[1]);
    w_u16(object + 52, saved[2]);
    sub_80030f64(object + 24, object + 48, NULL);
    vx = r_u32(object + 56);
    vy = r_u32(object + 60);
    vz = r_u32(object + 64);
    speed = sub_80083f40((sint32)(0 - vx) >> 6, (sint32)(0 - vy) >> 6, (sint32)(0 - vz) >> 6) << 6;
    sub_800308f0(object + 24, 0, &matrix, 0);
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
    x = r_u32(object + 56);
    x = (sint32)x >> 6;
    y = r_u32(object + 60);
    y = (sint32)y >> 6;
    z = r_u32(object + 64);
    z = (sint32)z >> 6;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    vx = xport_gte_read_data(25);
    vy = xport_gte_read_data(26);
    vz = xport_gte_read_data(27);
    value = r_u32(settings + 72);
    thrust = r_s16(control + 14);
    product = thrust * value;
    forward = vz << 6;
    thrust = (sint32)product >> 12;
    if ((sint32)forward < 0 && (sint32)thrust < 0)
        thrust = 0;
    value = r_u32(settings + 60);
    if ((sint32)value < (sint32)forward && (sint32)thrust > 0)
        thrust = 0;
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
    xport_gte_write_data(1, thrust & 0xFFFF);
    xport_gte_execute(0x486012);
    ax = xport_gte_read_data(25);
    ay = xport_gte_read_data(26);
    az = xport_gte_read_data(27);
    vx = r_u32(object + 56);
    ax += (uint32)((sint32)(0 - vx) >> 8);
    vy = r_u32(object + 60);
    ay += (uint32)((sint32)(0 - vy) >> 8);
    vz = r_u32(object + 64);
    az += (uint32)((sint32)(0 - vz) >> 8);
    value = r_u32(object + 56);
    w_u32(object + 56, value + ax);
    value = r_u32(object + 60);
    w_u32(object + 60, value + ay);
    value = r_u32(object + 64);
    vx = r_u32(object + 56);
    value += az;
    w_u32(object + 64, value);
    if ((sint32)vx > 0xB4000)
        w_u32(object + 56, 0xB4000);
    value = r_u32(object + 56);
    if ((sint32)value < -737280)
        w_u32(object + 56, 0xFFF4C000);
    value = r_u32(object + 60);
    if ((sint32)value > 737280)
        w_u32(object + 60, 0xB4000);
    value = r_u32(object + 60);
    if ((sint32)value < -737280)
        w_u32(object + 60, 0xFFF4C000);
    value = r_u32(object + 64);
    if ((sint32)value > 737280)
        w_u32(object + 64, 0xB4000);
    value = r_u32(object + 64);
    if ((sint32)value < -737280)
        w_u32(object + 64, 0xFFF4C000);
    w_u32(state + 72, speed);
    sub_80031bb4(object + 56, state + 64, 0);
    sub_8006ee38(object, state);
    w_u16(state + 76, 4096);
    return 4096;
}

uint32 sub_8007542c(uint32 object, uint32 state, uint32 output, uint32 control, uint32 smooth)
{
    uint32 pitch_target, bank_target, thrust_target, thrust_limit, phase, ticks, value, magnitude, pitch_limit, bank_limit, numerator, steps[3], targets[3], index, address, raw, desired;
    FUNCTION_MARKER(0x8007542C, "ACE3.BIN");
    thrust_target = 0;
    bank_target = 0;
    pitch_target = 0;
    phase = r_u32(state + 8);
    if (phase == 0)
    {
        w_u32(state + 8, 1);
        w_u32(state + 12, 0);
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0);
        w_u16(output + 6, 0);
    }
    else if (phase == 1)
    {
        ticks = r_u32(state + 12);
        thrust_target = 0 - (uint32)2048;
        if ((sint32)ticks < 76)
            w_u32(state + 12, ticks + 1);
        else
        {
            w_u32(state + 8, 2);
            w_u32(state + 12, 0);
        }
    }
    else if (phase == 2)
    {
        thrust_target = 0 - (uint32)2048;
        value = r_s16(object + 28);
        ticks = r_u32(state + 12);
        bank_target = (1820 - value) << 1;
        if ((sint32)ticks < 121)
            w_u32(state + 12, ticks + 1);
        else
        {
            w_u32(state + 8, 3);
            w_u32(state + 12, 0);
        }
    }
    else if (phase == 3)
    {
        value = r_s16(object + 28);
        raw = r_s16(object + 24);
        bank_target = (0 - value) << 1;
        pitch_target = (0 - raw) << 4;
    }
    if ((sint32)pitch_target < -4096)
        pitch_target = 0 - (uint32)4096;
    else if ((sint32)pitch_target >= 4097)
        pitch_target = 4096;
    if ((sint32)bank_target < -4096)
        bank_target = 0 - (uint32)4096;
    else if ((sint32)bank_target >= 4097)
        bank_target = 4096;
    pitch_limit = pitch_target;
    if ((sint32)pitch_target < 0)
        pitch_limit = 0 - pitch_target;
    bank_limit = bank_target;
    if ((sint32)bank_target < 0)
        bank_limit = 0 - bank_target;
    thrust_limit = thrust_target;
    if ((sint32)thrust_target < 0)
        thrust_limit = 0 - thrust_target;
    value = r_s16(output);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)pitch_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output, (sint32)numerator / 200);
    }
    value = r_s16(output + 2);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if (magnitude != 0)
    {
        numerator = value << 7;
        w_u16(output + 2, (sint32)numerator / 200);
    }
    value = r_s16(output + 4);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)bank_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output + 4, (sint32)numerator / 200);
    }
    value = r_s16(output + 6);
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)thrust_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        w_u16(output + 6, (sint32)numerator / 140);
    }
    targets[0] = bank_target;
    targets[1] = pitch_target;
    targets[2] = 0;
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = smooth + index * 2;
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)targets[index] < (sint32)value)
            w_u16(address, raw - steps[index]);
        value = r_s16(address);
        raw = r_u16(address);
        if ((sint32)value < (sint32)targets[index])
            w_u16(address, raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = r_s16(smooth + index * 2);
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            w_u16(smooth + index * 2, targets[index]);
    }
    desired = 0;
    if ((sint32)thrust_target < -2048)
        desired = (0 - (thrust_target + 2048)) << 1;
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < (sint32)desired)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)desired < (sint32)value)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(output + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    value = r_u16(output);
    raw = r_u16(output + 2);
    w_u16(output, value + (uint32)((sint32)pitch_target >> 3));
    value = r_u16(output + 4);
    w_u16(output + 4, value + (uint32)((sint32)bank_target >> 5));
    value = r_u16(output + 6);
    w_u16(output + 6, value + (uint32)((sint32)thrust_target >> 5));
    value = r_s16(output);
    if ((sint32)value < (sint32)(0 - pitch_limit))
        w_u16(output, 0 - pitch_limit);
    value = r_s16(output);
    if ((sint32)pitch_limit < (sint32)value)
        w_u16(output, pitch_limit);
    value = r_s16(output + 2);
    if ((sint32)value < 0)
        w_u16(output + 2, 0);
    value = r_s16(output + 2);
    if ((sint32)value > 0)
        w_u16(output + 2, 0);
    value = r_s16(output + 4);
    if ((sint32)value < (sint32)(0 - bank_limit))
        w_u16(output + 4, 0 - bank_limit);
    value = r_s16(output + 4);
    if ((sint32)bank_limit < (sint32)value)
        w_u16(output + 4, bank_limit);
    value = r_s16(output + 6);
    if ((sint32)value < -4096)
        w_u16(output + 6, 0xF000);
    value = r_s16(output + 6);
    if ((sint32)value >= 4097)
        w_u16(output + 6, 4096);
    value = r_u16(output);
    w_u16(control + 4, value);
    value = r_u16(output + 2);
    w_u16(control + 6, value);
    value = r_u16(output + 4);
    w_u16(control + 8, value);
    value = r_u16(output + 6);
    w_u16(control + 14, value);
    return value;
}

uint32 sub_8009d05c(uint32 port, uint32 group, uint32 item, uint32 descriptor_provider)
{
    uint32 descriptor, count, records, record, bytes;
    FUNCTION_MARKER(0x8009D05C, "ACE3.BIN");
    descriptor = xport_guest_call3(descriptor_provider, port, group, item);
    if ((sint32)group < 0)
        return r_u8(descriptor + 234);
    count = r_u8(descriptor + 234);
    if ((sint32)group >= (sint32)count)
        return 0;
    records = r_u32(descriptor + 8);
    record = records + (group << 3);
    if ((sint32)item < 0)
        return r_u8(record);
    count = r_u8(record);
    if ((sint32)item >= (sint32)count)
        return 0;
    bytes = r_u32(record + 4);
    return r_u8(bytes + item);
}

uint32 sub_8009d054(uint32 port, uint32 group, uint32 item)
{
    uint32 descriptor_provider;
    FUNCTION_MARKER(0x8009D054, "ACE3.BIN");
    descriptor_provider = r_u32(0x800B4698);
    return sub_8009d05c(port, group, item, descriptor_provider);
}

uint32 sub_800a85c0(uint32 ordering, uint32 count, uint32 debug_level)
{
    uint32 target, next, value;
    FUNCTION_MARKER(0x800A85C0, "ACE3.BIN");
    if (debug_level >= 2)
    {
        target = r_u32(0x800BB960);
        xport_guest_call3(target, 0x800251B4, ordering, count);
    }
    count -= 1;
    if (count != 0)
    {
        do
        {
            count -= 1;
            next = ordering + 4;
            w_u8(ordering + 3, 0);
            value = r_u32(ordering);
            w_u32(ordering, (value & 0xFF000000) | (next & 0xFFFFFF));
            ordering = next;
        } while (count != 0);
    }
    w_u32(0x800BBA24, (0x800BBA10 & 0xFFFFFF) | 0x04000000);
    w_u32(ordering, 0x800BBA24 & 0xFFFFFF);
    return ordering;
}

uint32 sub_800a85b8(uint32 ordering, uint32 count)
{
    uint32 debug_level;
    FUNCTION_MARKER(0x800A85B8, "ACE3.BIN");
    debug_level = r_u8(0x800BB966);
    return sub_800a85c0(ordering, count, debug_level);
}

uint32 sub_800aac04(uint32 ordering, uint32 debug_level)
{
    FUNCTION_MARKER(0x800AAC04, "ACE3.BIN");
    if (DrawSync(0) < 0)
        return 0xFFFFFFFF;
    DrawOTag(psx_addr(ordering, 4));
    return 0;
}

uint32 sub_800aabfc(uint32 ordering)
{
    uint32 debug_level;
    FUNCTION_MARKER(0x800AABFC, "ACE3.BIN");
    debug_level = r_u8(0x800BB966);
    return sub_800aac04(ordering, debug_level);
}

uint32 sub_8006477c(uint32 x, uint32 y)
{
    const uint8 faces[4][3] = {{0, 2, 1}, {0, 1, 3}, {0, 3, 2}, {1, 2, 3}};
    const uint8 edges[6][4] = {{0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 1, 2}, {1, 2, 0, 3}, {1, 3, 1, 3}, {2, 3, 2, 3}};
    uint32 points[4], areas[4], index, vertex, data, depth_cue, flags, depth, counters, count, packets, packet, result;
    FUNCTION_MARKER(0x8006477C, "ACE3.BIN");
    xport_gte_write_control(24, x << 16);
    xport_gte_write_control(25, y << 16);
    for (index = 0; index < 4; index += 1)
    {
        data = r_u32(0x800B2C18 + index * 8);
        xport_gte_write_data(0, data);
        data = r_u32(0x800B2C1C + index * 8);
        xport_gte_write_data(1, data);
        xport_gte_execute(0x180001);
        points[index] = xport_gte_read_data(14);
        depth_cue = xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        depth = xport_gte_read_data(19);
    }
    xport_gte_write_control(24, 160 << 16);
    xport_gte_write_control(25, 120 << 16);
    for (index = 0; index < 4; index += 1)
    {
        xport_gte_write_data(12, points[faces[index][0]]);
        xport_gte_write_data(14, points[faces[index][2]]);
        xport_gte_write_data(13, points[faces[index][1]]);
        xport_gte_execute(0x1400006);
        areas[index] = xport_gte_read_data(24);
    }
    counters = r_u32(0x800BD074);
    count = r_u32(counters + 12);
    packets = r_u32(0x800BD078);
    packet = packets + count * 28 + 392;
    for (index = 0; index < 4; index += 1)
    {
        if ((sint32)areas[index] <= 0)
            continue;
        for (vertex = 0; vertex < 3; vertex += 1)
        {
            w_u8(packet + 4 + vertex * 8, 0);
            w_u8(packet + 5 + vertex * 8, faces[index][vertex] == 1 ? 64 : 16);
            w_u8(packet + 6 + vertex * 8, 0);
        }
        w_u32(packet + 8, points[faces[index][0]]);
        w_u32(packet + 16, points[faces[index][1]]);
        w_u32(packet + 24, points[faces[index][2]]);
        counters = r_u32(0x800BD074);
        count = r_u32(counters + 12);
        packet += 28;
        w_u32(counters + 12, count + 1);
    }
    counters = r_u32(0x800BD074);
    count = r_u32(counters + 24);
    packets = r_u32(0x800BD078);
    packet = packets + count * 20 + 2008;
    result = areas[3];
    for (index = 0; index < 6; index += 1)
    {
        if ((sint32)areas[edges[index][2]] < 0 && (sint32)areas[edges[index][3]] < 0)
            continue;
        for (vertex = 0; vertex < 2; vertex += 1)
        {
            w_u8(packet + 4 + vertex * 8, 0);
            w_u8(packet + 5 + vertex * 8, edges[index][vertex] == 1 ? 64 : 24);
            w_u8(packet + 6 + vertex * 8, 0);
        }
        w_u32(packet + 8, points[edges[index][0]]);
        w_u32(packet + 16, points[edges[index][1]]);
        counters = r_u32(0x800BD074);
        count = r_u32(counters + 24);
        packet += 20;
        count += 1;
        w_u32(counters + 24, count);
        if (index == 5)
            result = count;
    }
    return result;
}

uint32 sub_800afba8(uint32 buffer0, uint32 size0, uint32 buffer1, uint32 size1)
{
    FUNCTION_MARKER(0x800AFBA8, "ACE3.BIN");
    return InitPAD(buffer0, size0, buffer1, size1);
}

uint32 sub_800afbb8(void)
{
    FUNCTION_MARKER(0x800AFBB8, "ACE3.BIN");
    return StartPAD();
}

uint32 sub_800afbd8(uint32 type, uint32 destination)
{
    FUNCTION_MARKER(0x800AFBD8, "ACE3.BIN");
    return PAD_init2(type, destination);
}

uint32 sub_80027998(uint32 x, uint32 y, uint32 format, uint32 arguments)
{
    uint8 buffer[512];
    uint32 origin, index, start, length, ch, end, delta, step;
    FUNCTION_MARKER(0x80027998, "ACE3.BIN");
    origin = x;
    length = format_to_native_buffer(buffer, sizeof(buffer), format, arguments, 0, 0);
    index = 0;
    start = 0;
    while ((sint32)index < (sint32)length)
    {
        ch = buffer[index];
        if (ch == 10)
        {
            buffer[index] = 0;
            sub_80027608(x, y, 0, buffer + start);
            start = index + 1;
            y += 6;
            x = (uint32)(sint32)(sint16)origin;
        }
        else if (ch == 9)
        {
            buffer[index] = 0;
            end = sub_80027608(x, y, 0, buffer + start);
            delta = (uint32)(sint32)(sint16)(end - x);
            step = (uint32)((sint32)delta / 6);
            step = (uint32)(sint32)(sint16)step;
            start = index + 1;
            step = (uint32)((sint32)(step + 4) >> 2);
            x += step * 24;
        }
        index += 1;
    }
    end = sub_80027608(x, y, 0, buffer + start);
    return (end & 0xFFFF) | (y << 16);
}

uint32 sub_80027af0(uint32 format, uint32 arguments)
{
    uint8 buffer[512];
    uint32 cursor, x, y, index, start, length, ch, end, delta, step;
    FUNCTION_MARKER(0x80027AF0, "ACE3.BIN");
    cursor = r_u32(0x800BCE68);
    x = cursor & 0xFFFF;
    y = cursor >> 16;
    index = 0;
    length = format_to_native_buffer(buffer, sizeof(buffer), format, arguments, 0, 0);
    start = 0;
    while ((sint32)index < (sint32)length)
    {
        ch = buffer[index];
        if (ch == 10)
        {
            buffer[index] = 0;
            sub_80027608(x, y, 0, buffer + start);
            start = index + 1;
            y += 6;
            x = 16;
        }
        else if (ch == 9)
        {
            buffer[index] = 0;
            end = sub_80027608(x, y, 0, buffer + start);
            delta = (end & 0xFFFF) - x;
            step = (uint32)((sint32)delta / 6);
            start = index + 1;
            step = (uint32)((sint32)(step + 4) >> 2);
            x += step * 24;
        }
        index += 1;
    }
    end = sub_80027608(x, y, 0, buffer + start);
    cursor = (end & 0xFFFF) | (y << 16);
    w_u32(0x800BCE68, cursor);
    return cursor;
}

uint32 sub_800af898(uint32 pattern, uint32 record)
{
    uint32 length;
    const char *native_pattern;
    void *native_record, *result;
    FUNCTION_MARKER(0x800AF898, "ACE3.BIN");
    native_pattern = 0;
    if (pattern != 0)
    {
        length = 0;
        while (r_u8(pattern + length) != 0)
            length += 1;
        native_pattern = (const char *)psx_addr(pattern, length + 1);
    }
    native_record = record != 0 ? psx_addr(record, 40) : 0;
    result = firstfile(native_pattern, native_record);
    return result != 0 ? record : 0;
}

void sub_800ad558(void)
{
    FUNCTION_MARKER(0x800AD558, "ACE3.BIN");
    xport_bios_stop_card();
}

void sub_800afbc8(void)
{
    FUNCTION_MARKER(0x800AFBC8, "ACE3.BIN");
    xport_bios_stop_pad();
}

uint32 sub_800afd40(uint32 source, uint32 destination, uint32 distance_bits)
{
    uint32 flags_cursor, literals, tokens, bits, mask, flags, reference, positive, token, distance, count, value;
    FUNCTION_MARKER(0x800AFD40, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = r_u32(source + 8);
    tokens = r_u32(source + 12);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    bits = distance_bits & 31;
    mask = ((uint32)1 << bits) - 1;
    for (;;)
    {
        flags = r_u32(flags_cursor);
        flags_cursor += 4;
        if (flags == 0)
            return destination;
        for (;;)
        {
            positive = (sint32)flags > 0;
            flags <<= 1;
            if (flags == 0)
                break;
            if (positive == 0)
            {
                value = r_u8(literals);
                literals += 1;
                w_u8(destination, value);
                destination += 1;
            }
            else
            {
                token = r_u16(tokens);
                tokens += 2;
                distance = (token & mask) + 1;
                count = (token >> bits) + 3;
                reference = destination - distance;
                do
                {
                    value = r_u8(reference);
                    count -= 1;
                    w_u8(destination, value);
                    reference += 1;
                    destination += 1;
                } while ((sint32)count > 0);
            }
        }
    }
}

uint32 sub_800afde4(uint32 source, uint32 destination, uint32 distance_bits)
{
    uint32 flags_cursor, literals, tokens, bits, mask, flags, reference, positive, token, distance, count, value;
    FUNCTION_MARKER(0x800AFDE4, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = r_u32(source + 8);
    tokens = r_u32(source + 12);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    bits = (distance_bits - 1) & 31;
    mask = ((uint32)1 << bits) - 1;
    for (;;)
    {
        flags = r_u32(flags_cursor);
        flags_cursor += 4;
        if (flags == 0)
            return destination;
        for (;;)
        {
            positive = (sint32)flags > 0;
            flags <<= 1;
            if (flags == 0)
                break;
            if (positive == 0)
            {
                value = r_u16(literals);
                literals += 2;
                w_u16(destination, value);
                destination += 2;
            }
            else
            {
                token = r_u16(tokens);
                tokens += 2;
                distance = ((token & mask) << 1) + 2;
                count = (token >> bits) + 2;
                reference = destination - distance;
                do
                {
                    value = r_u16(reference);
                    count -= 1;
                    w_u16(destination, value);
                    reference += 2;
                    destination += 2;
                } while ((sint32)count > 0);
            }
        }
    }
}

uint32 sub_800afe90(uint32 source, uint32 destination, uint32 distance_bits)
{
    uint32 flags_cursor, literals, tokens, remaining, bits, mask, flags, flags_left, cache_left, cache, literal, token, count, reference, value;
    FUNCTION_MARKER(0x800AFE90, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = r_u32(source + 8);
    tokens = r_u32(source + 12);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    remaining = r_u32(source + 4) & 0xFFFFFF;
    cache_left = 0xFFFFFFFF;
    bits = distance_bits & 31;
    mask = ((uint32)1 << bits) - 1;
    flags_left = 32;
    flags = r_u32(flags_cursor);
    flags_cursor += 4;
    flags_left -= 1;
    while ((sint32)remaining > 0)
    {
        if ((sint32)flags_left < 0)
        {
            flags_left = 31;
            flags = r_u32(flags_cursor);
            flags_cursor += 4;
        }
        literal = (sint32)flags < 0;
        flags <<= 1;
        if (literal != 0)
        {
            remaining -= 1;
            if ((sint32)cache_left > 0)
                cache_left -= 1;
            else
            {
                cache = r_u32(literals);
                literals += 4;
                cache_left = 3;
            }
            w_u8(destination, cache);
            cache >>= 8;
            destination += 1;
        }
        else
        {
            token = r_u16(tokens);
            tokens += 2;
            reference = destination - (token & mask) - 1;
            count = (token >> bits) + 3;
            remaining -= count;
            do
            {
                value = r_u8(reference);
                count -= 1;
                w_u8(destination, value);
                reference += 1;
                destination += 1;
            } while ((sint32)count > 0);
        }
        flags_left -= 1;
    }
    return destination;
}

uint32 sub_800afd08(uint32 source, uint32 destination)
{
    uint32 format, distance_bits, target;
    FUNCTION_MARKER(0x800AFD08, "ACE3.BIN");
    format = r_u8(source + 7);
    distance_bits = r_u8(source + 11);
    target = r_u32(0x800AFD30 + (format << 2));
    if (target == 0x800AFD40)
        return sub_800afd40(source, destination, distance_bits);
    if (target == 0x800AFDE4)
        return sub_800afde4(source, destination, distance_bits);
    if (target == 0x800AFE90)
        return sub_800afe90(source, destination, distance_bits);
    return xport_decompress_guest_call(target, source, destination, distance_bits);
}

uint32 sub_800aff74(uint32 source, uint32 destination, uint32 chunk_bytes)
{
    uint32 flags_cursor, literals, tokens, size, bits, mask, flags;
    FUNCTION_MARKER(0x800AFF74, "ACE3.BIN");
    flags_cursor = source + 16;
    w_u32(0x800BCB68, chunk_bytes);
    w_u32(0x800BCB64, destination);
    literals = r_u32(source + 8);
    tokens = r_u32(source + 12);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    w_u32(0x800BCB48, literals);
    size = r_u32(source + 4);
    w_u32(0x800BCB4C, tokens);
    size &= 0xFFFFFF;
    w_u32(0x800BCB58, size);
    bits = r_u8(source + 11);
    w_u32(0x800BCB5C, bits);
    mask = ((uint32)1 << (bits & 31)) - 1;
    w_u32(0x800BCB60, mask);
    w_u32(0x800BCB54, 32);
    flags = r_u32(flags_cursor);
    flags_cursor += 4;
    w_u32(0x800BCB50, flags);
    w_u32(0x800BCB44, flags_cursor);
    return size;
}

uint32 sub_800afff0(void)
{
    uint32 bits, destination, flags, mask, flags_left, flags_cursor, literals, chunk_bytes, remaining, tokens, literal, token, count, reference, value, complete, requested, total, consumed;
    FUNCTION_MARKER(0x800AFFF0, "ACE3.BIN");
    bits = r_u32(0x800BCB5C);
    destination = r_u32(0x800BCB64);
    if ((sint32)bits < 0)
    {
        w_u32(0x800BCB5C, 0xFFFFFFFF);
        return 0;
    }
    flags = r_u32(0x800BCB50);
    mask = r_u32(0x800BCB60);
    flags_left = r_u32(0x800BCB54);
    flags_cursor = r_u32(0x800BCB44);
    literals = r_u32(0x800BCB48);
    chunk_bytes = r_u32(0x800BCB68);
    remaining = r_u32(0x800BCB58);
    w_u32(0x800BCB6C, 1);
    tokens = r_u32(0x800BCB4C);
    if ((sint32)chunk_bytes < (sint32)remaining)
    {
        remaining = chunk_bytes;
        w_u32(0x800BCB6C, 0);
    }
    w_u32(0x800BCB70, remaining);
    flags_left -= 1;
    while ((sint32)remaining > 0)
    {
        if ((sint32)flags_left < 0)
        {
            flags_left = 31;
            flags = r_u32(flags_cursor);
            flags_cursor += 4;
        }
        literal = (sint32)flags < 0;
        flags <<= 1;
        if (literal != 0)
        {
            remaining -= 1;
            value = r_u8(literals);
            literals += 1;
            w_u8(destination, value);
            destination += 1;
        }
        else
        {
            token = r_u16(tokens);
            tokens += 2;
            reference = destination - (token & mask) - 1;
            count = (token >> (bits & 31)) + 3;
            remaining -= count;
            do
            {
                value = r_u8(reference);
                count -= 1;
                w_u8(destination, value);
                reference += 1;
                destination += 1;
            } while ((sint32)count > 0);
        }
        if ((sint32)remaining > 0)
            flags_left -= 1;
    }
    complete = r_u32(0x800BCB6C);
    w_u32(0x800BCB64, destination);
    if (complete != 0)
        return destination;
    w_u32(0x800BCB50, flags);
    requested = r_u32(0x800BCB70);
    w_u32(0x800BCB44, flags_cursor);
    total = r_u32(0x800BCB58);
    consumed = requested - remaining;
    total -= consumed;
    w_u32(0x800BCB58, total);
    w_u32(0x800BCB54, flags_left);
    w_u32(0x800BCB48, literals);
    w_u32(0x800BCB4C, tokens);
    return 0;
}

void sub_800ad788(uint32 return_continuation)
{
    uint32 table, destination, source, word;
    uint32 expected_return = return_continuation;
    FUNCTION_MARKER(0x800AD788, "ACE3.BIN");
    w_u32(0x800CBC88, return_continuation);
    bios_enter_critical();
    table = GetC0Table();
    destination = r_u32(table + 24);
    source = 0x800AD7F8;
    do
    {
        word = r_u32(source);
        w_u32(destination + 112, word);
        source += 4;
        destination += 4;
    } while (source != 0x800AD804);
    FlushCache();
    bios_exit_critical();
    return_continuation = r_u32(0x800CBC88);
    if (return_continuation != expected_return)
        xport_guest_call0(return_continuation);
}

uint32 sub_8004ba7c(uint32 group_index)
{
    uint8 members[8];
    uint32 source, runtime, scan, active, cursor, member, flags_address, flags, flag, root, count, leader, actor, value, result;
    FUNCTION_MARKER(0x8004BA7C, "ACE3.BIN");
    active = 0;
    scan = 0;
    source = r_u32(0x800BCEEC) + group_index * 12;
    runtime = r_u32(0x800BCEF4) + group_index * 12;
    count = r_u16(source + 2);
    cursor = source;
    if (count != 0)
    {
        do
        {
            member = r_u8(cursor + 4);
            flags_address = 0x800C6FD0 + member * 4;
            flags = r_u32(flags_address);
            if ((flags & 1) != 0)
            {
                if (active != 0)
                    w_u32(flags_address, flags | 0x100000);
                else
                {
                    w_u8(runtime + 6, member);
                    member = r_u8(cursor + 4);
                    flags_address = 0x800C6FD0 + member * 4;
                    flags = r_u32(flags_address);
                    w_u32(flags_address, flags & 0xFFEFFFFF);
                }
                members[active] = r_u8(cursor + 4);
                flag = r_u8(runtime + 7);
                active += 1;
                if (flag != 1)
                {
                    root = r_u32(0x800BD080);
                    member = r_u8(cursor + 4);
                    flags = r_u32(root);
                    if ((flags & ((uint32)1 << (member & 31))) != 0)
                    {
                        w_u8(runtime + 7, 1);
                        w_u32(runtime + 8, 0);
                    }
                }
            }
            count = r_u16(source + 2);
            scan += 1;
            cursor += 1;
        } while ((sint32)scan < (sint32)count);
    }
    w_u16(runtime + 4, active);
    if ((active & 0xFFFF) < 2)
        sub_8004bda8(group_index);
    member = r_u8(runtime + 6);
    leader = r_u32(0x800C6ED0 + member * 4);
    if (leader == 0)
        return 1;
    flag = r_u8(runtime + 7);
    if (flag != 1)
    {
        flags = r_u32(leader);
        if ((flags & 32) != 0)
        {
            w_u8(runtime + 7, 1);
            w_u32(runtime + 8, 0);
        }
    }
    count = r_u16(runtime + 4);
    scan = 1;
    if ((sint32)scan < (sint32)count)
    {
        do
        {
            member = members[scan];
            actor = r_u32(0x800BCEC0) + member * 184;
            flag = r_u8(runtime + 7);
            if (flag == 1)
            {
                value = r_u32(runtime + 8);
                if (value == 0)
                {
                    value = r_u32(actor + 68);
                    w_u8(actor + 99, 12);
                    w_u8(actor + 61, 23);
                    w_u8(actor + 97, 0);
                    w_u16(actor + 120, 4096);
                    w_u16(actor + 122, 0);
                    w_u16(actor + 124, (uint32)((sint32)value >> 7));
                }
                flags = r_u32(actor);
                if ((flags & 0x400000) != 0)
                {
                    sub_8004bda8(group_index);
                    w_u8(actor + 61, 255);
                    w_u8(actor + 99, 255);
                }
            }
            else
            {
                w_u8(actor + 61, 3);
                member = r_u8(runtime + 6);
                w_u8(actor + 63, member);
                count = r_u16(runtime + 4);
                sub_8004bd18(count, scan, actor + 68, 0);
            }
            count = r_u16(runtime + 4);
            scan += 1;
        } while ((sint32)scan < (sint32)count);
    }
    result = r_u32(runtime + 8) + 1;
    w_u32(runtime + 8, result);
    return result;
}

uint32 sub_8006b110(void)
{
    uint16 first[3], second[3];
    MATRIX matrix;
    NATIVE_POSE pose;
    uint32 offsets[3], delta[3], root, count, scan, queue, entry, index, source, slot, flags, kind, a, b, c, origin, x, y, z, jitter_x, jitter_y, jitter_z, random, result;
    FUNCTION_MARKER(0x8006B110, "ACE3.BIN");
    root = r_u32(0x800BD080);
    scan = 0;
    count = r_u32(root + 5452);
    queue = root + 5456;
    entry = root + 5458;
    if ((sint32)count > 0)
    {
        do
        {
            index = r_s16(queue);
            source = r_u32(entry + 6);
            slot = root + index * 60 + 5584;
            flags = r_u32(entry + 2);
            w_u32(slot + 56, flags);
            a = r_s16(source + 16);
            b = r_s16(source + 18);
            c = r_s16(source + 20);
            first[0] = a;
            first[1] = b;
            first[2] = c;
            rotation_build(0, 0, &matrix, first);
            a = xport_load_le32((const uint8 *)&matrix);
            b = xport_load_le32((const uint8 *)&matrix + 4);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            a = xport_load_le32((const uint8 *)&matrix + 8);
            b = xport_load_le32((const uint8 *)&matrix + 12);
            c = (uint16)matrix.m[2][2];
            xport_gte_write_control(2, a);
            xport_gte_write_control(3, b);
            xport_gte_write_control(4, c);
            kind = r_s16(entry);
            switch (kind)
            {
                case 0:
                    w_u32(slot, 0x20410);
                    w_u32(slot + 4, 37);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 5504;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 2048;
                    break;
                case 1:
                    w_u32(slot, 0x20820);
                    w_u32(slot + 4, 24);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 6592;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 2448;
                    break;
                case 2:
                    w_u32(slot, 0x41040);
                    w_u32(slot + 4, 62);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 4128;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 1216;
                    break;
                case 3:
                    w_u32(slot, 0x82080);
                    w_u32(slot + 4, 35);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 4384;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 0;
                    break;
                case 4:
                    w_u32(slot, 0x94103);
                    w_u32(slot + 4, 16);
                    first[0] = 0;
                    first[1] = 0;
                    first[2] = 13760;
                    second[0] = 0;
                    second[1] = 0;
                    second[2] = 0;
                    break;
                case 5:
                    w_u32(slot, 0x98202);
                    w_u32(slot + 4, 16);
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
            flags = r_u32(slot);
            if ((flags & 0x10000) == 0)
            {
                random = game_random();
                jitter_x = (uint32)((sint32)(0x4000 - random) >> 1);
                random = game_random();
                jitter_y = (uint32)((sint32)(0x4000 - random) >> 1);
                random = game_random();
                jitter_z = (uint32)((sint32)(0x4000 - random) >> 1);
            }
            flags = r_u32(slot);
            if ((flags & 2) == 0)
                jitter_y -= 13106;
            origin = r_u32(entry + 10);
            x = r_u32(origin);
            w_u32(slot + 40, (offsets[0] << 7) + x + jitter_x);
            origin = r_u32(entry + 10);
            y = r_u32(origin + 4);
            w_u32(slot + 44, (offsets[1] << 7) + y + jitter_y);
            origin = r_u32(entry + 10);
            z = r_u32(origin + 8);
            z = (offsets[2] << 7) + z + jitter_z;
            w_u32(slot + 48, z);
            a = r_u32(source);
            x = r_u32(slot + 40);
            b = r_u32(source + 4);
            x = a - x;
            y = r_u32(slot + 44);
            a = r_u32(source + 8);
            w_u32(slot + 8, x);
            y = b - y;
            z = a - z;
            w_u32(slot + 12, y);
            w_u32(slot + 16, z);
            w_u32(slot + 24, x + (delta[0] << 8));
            flags = r_u32(slot + 56);
            w_u32(slot + 28, y + (delta[1] << 8));
            w_u32(slot + 32, z + (delta[2] << 8));
            if ((flags & 0x2000) != 0)
            {
                pose.position[0] = x;
                pose.position[1] = r_u32(slot + 12);
                pose.position[2] = r_u32(slot + 16);
                pose.angles[0] = r_u16(source + 16);
                pose.angles[1] = r_u16(source + 18);
                pose.angles[2] = r_u16(source + 20);
                index = r_s16(queue);
                kind = r_s16(entry);
                sub_8007e8c4(index, 0, slot + 40, kind, &pose);
            }
            scan += 1;
            root = r_u32(0x800BD080);
            entry += 16;
            count = r_u32(root + 5452);
            queue += 16;
        } while ((sint32)scan < (sint32)count);
    }
    result = r_u32(0x800BD080);
    w_u32(result + 5452, 0);
    return result;
}

uint32 sub_8006c760(uint32 index, uint32 position, uint32 vector, uint32 mode, const NATIVE_POSE *native_position, const uint32 *native_vector)
{
    MATRIX matrix;
    uint16 direction[3];
    uint32 root, address, flags, slot, a, b, c, transformed[3], value;
    FUNCTION_MARKER(0x8006C760, "ACE3.BIN");
    root = r_u32(0x800BD080);
    address = root + 13264 + ((uint32)((sint32)index >> 5) << 2);
    flags = r_u32(address);
    w_u32(address, flags | ((uint32)1 << (index & 31)));
    rotation_build(position + 16, 0, &matrix, native_position != 0 ? native_position->angles : 0);
    a = xport_load_le32((const uint8 *)&matrix);
    b = xport_load_le32((const uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = xport_load_le32((const uint8 *)&matrix + 8);
    b = xport_load_le32((const uint8 *)&matrix + 12);
    c = (uint16)matrix.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    root = r_u32(0x800BD080);
    slot = root + index * 60 + 5584;
    switch (mode)
    {
        case 0:
            w_u32(slot, 0x20410);
            w_u32(slot + 4, 37);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 2048;
            break;
        case 1:
            w_u32(slot, 0x20820);
            w_u32(slot + 4, 24);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 2448;
            break;
        case 2:
            w_u32(slot, 0x41040);
            w_u32(slot + 4, 62);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 1216;
            break;
        case 3:
            w_u32(slot, 0x82080);
            w_u32(slot + 4, 35);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 0;
            break;
        case 4:
            w_u32(slot, 0x94103);
            w_u32(slot + 4, 16);
            direction[0] = 0;
            direction[1] = 0;
            direction[2] = 0;
            break;
        case 5:
            w_u32(slot, 0x98202);
            w_u32(slot + 4, 16);
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
    value = native_position != 0 ? native_position->position[0] : r_u32(position);
    w_u32(slot + 8, value);
    value = native_position != 0 ? native_position->position[1] : r_u32(position + 4);
    w_u32(slot + 12, value);
    value = native_position != 0 ? native_position->position[2] : r_u32(position + 8);
    w_u32(slot + 16, value);
    value = r_u32(slot + 8);
    w_u32(slot + 24, value + (transformed[0] << 8));
    value = r_u32(slot + 12);
    w_u32(slot + 28, value + (transformed[1] << 8));
    value = r_u32(slot + 16);
    w_u32(slot + 32, value + (transformed[2] << 8));
    value = native_vector != 0 ? native_vector[0] : r_u32(vector);
    w_u32(slot + 40, value);
    value = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    w_u32(slot + 44, value);
    value = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    w_u32(slot + 48, value);
    return value;
}

uint32 sub_800522b0(uint32 index)
{
    uint32 first[4], target[3], direction[3];
    uint32 descriptors, objects, flags, object, data, mode, phase, value, a, b, c, pointer, game, mission, distance, speed, result, scan, nearest, selected, timer, strength;
    FUNCTION_MARKER(0x800522B0, "ACE3.BIN");
    descriptors = r_u32(0x800BCEFC);
    objects = r_u32(0x800BCE4C);
    flags = r_u32(0x800C6FD0 + (index + 32) * 4);
    object = r_u32(objects + index * 4);
    data = r_u32(descriptors + index * 40 + 20);
    if ((flags & 0x400000) != 0)
        return 0;
    mode = r_u8(data);
    switch (mode)
    {
        case 0:
            w_u32(object + 56, 0);
            w_u32(object + 60, 0);
            w_u32(object + 64, 0);
            return 0x80052350;
        case 1:
            a = r_u16(data + 2);
            b = r_s16(0x800104AC + (a & 0x3FFF) * 2);
            c = r_u32(data + 8);
            w_u32(object + 60, 0);
            value = (uint32)((sint32)(c * b) >> 14);
            w_u32(object + 56, value);
            a = r_u16(data + 2);
            b = r_s16(0x800104AC + ((a & 0x3FFF) + 0x1000) * 2);
            c = r_u32(data + 8);
            result = (uint32)((sint32)(b * c) >> 14);
            w_u32(object + 64, result);
            return result;
        case 2:
            w_u32(object + 56, 0);
            result = r_u32(data + 4);
            w_u32(object + 64, 0);
            w_u32(object + 60, result);
            return result;
        case 3:
            phase = r_s8(object + 88);
            value = r_u32(object + 60);
            value += phase == 0 || phase == 3 ? 0xFFFFFF00 : 256;
            w_u32(object + 60, value);
            result = r_u32(object + 92) + 1;
            w_u32(object + 92, result);
            if (result == 64)
            {
                result = r_u8(object + 88) + 1;
                w_u8(object + 88, result);
                result = (uint32)(sint32)(sint8)result;
                if (result == 4)
                    w_u8(object + 88, 0);
                w_u32(object + 92, 0);
            }
            w_u32(object + 56, 0);
            w_u32(object + 64, 0);
            return result;
        case 4:
            phase = r_s8(object + 88);
            if (phase == 0 || phase == 1 || phase == 2)
            {
                first[0] = r_u32(object + 8);
                first[1] = r_u32(object + 12);
                first[2] = r_u32(object + 16);
                first[3] = r_u32(object + 20);
            }
            else
            {
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                if (mission == 43)
                {
                    first[0] = r_u32(data + 24);
                    first[1] = r_u32(data + 28);
                    first[2] = r_u32(data + 32);
                }
                else
                {
                    first[0] = r_u32(object + 8);
                    first[1] = r_u32(object + 12);
                    first[2] = r_u32(object + 16);
                    first[3] = r_u32(object + 20);
                }
            }
            pointer = data + (phase == 0 ? 12 : phase == 1 ? 24 : 36);
            target[0] = r_u32(pointer);
            target[1] = r_u32(pointer + 4);
            target[2] = r_u32(pointer + 8);
            distance = sub_80052d70(0, 0, 0, first, target, direction);
            phase = r_s8(object + 88);
            a = r_u8(object + 88);
            if ((sint32)phase < 3)
            {
                speed = r_u32(data + 8);
                if ((sint32)distance < (sint32)speed)
                    w_u8(object + 88, a + 1);
            }
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            if (mission == 43 && r_s8(object + 88) == 3 && distance == 0)
            {
                flags = r_u32(object);
                if ((flags & 0x40) != 0 && (r_u32(0x800BD004) & 0x40) == 0)
                {
                    pointer = r_u32(0x800BCEAC);
                    sub_80054ebc(r_u32(pointer + 328));
                    value = r_u32(0x800BD004);
                    w_u32(0x800BD004, value | 0x40);
                }
                else
                {
                    flags = r_u32(object);
                    if ((flags & 0x80) != 0 && (r_u32(0x800BD004) & 0x80) == 0)
                    {
                        pointer = r_u32(0x800BCEAC);
                        sub_80054ebc(r_u32(pointer + 332));
                        value = r_u32(0x800BD004);
                        w_u32(0x800BD004, value | 0x80);
                    }
                }
            }
            speed = r_u32(data + 8);
            w_u32(object + 56, (uint32)((sint32)(speed * direction[0]) >> 14));
            speed = r_u32(data + 8);
            w_u32(object + 60, (uint32)((sint32)(speed * direction[1]) >> 14));
            speed = r_u32(data + 8);
            result = (uint32)((sint32)(speed * direction[2]) >> 14);
            w_u32(object + 64, result);
            return result;
        case 5:
            phase = r_s8(object + 88);
            if (phase == 0 || phase == 1)
            {
                pointer = data + (phase == 0 ? 12 : 24);
                target[0] = r_u32(pointer);
                target[1] = r_u32(pointer + 4);
                target[2] = r_u32(pointer + 8);
            }
            distance = sub_80052d70(object + 8, 0, 0, 0, target, direction);
            speed = r_u32(data + 8);
            if ((sint32)distance < (sint32)speed)
            {
                phase = r_s8(object + 88);
                w_u8(object + 88, phase == 0 ? 1 : 0);
                speed = r_u32(data + 8);
            }
            w_u32(object + 56, (uint32)((sint32)(speed * direction[0]) >> 14));
            speed = r_u32(data + 8);
            w_u32(object + 60, (uint32)((sint32)(speed * direction[1]) >> 14));
            speed = r_u32(data + 8);
            result = (uint32)((sint32)(speed * direction[2]) >> 14);
            w_u32(object + 64, result);
            return result;
        case 6:
            flags = r_u8(object + 5);
            phase = r_s8(object + 88);
            w_u8(object + 5, flags | 4);
            if (phase == 0)
            {
                w_u32(object + 56, 0);
                value = r_u32(data + 4);
                w_u32(object + 64, 0);
                w_u32(object + 60, value);
                a = r_u32(object + 12);
                b = r_u32(data + 16);
                if ((sint32)(a - b) >= -1048576)
                    w_u16(object + 6, 12288);
                a = r_u32(object + 12);
                b = r_u32(data + 16);
                result = (sint32)a < (sint32)b;
                if (result == 0)
                {
                    w_u16(object + 6, 4096);
                    w_u32(object + 12, b);
                    w_u8(object + 88, 1);
                    w_u32(object + 96, 0);
                    return 1;
                }
                return result;
            }
            if (phase == 1)
            {
                timer = r_u32(object + 96);
                if ((sint32)timer < 32)
                    w_u16(object + 6, 8192);
                else if ((sint32)timer < 64)
                    w_u16(object + 6, 12288);
                else
                {
                    w_u8(object + 88, 2);
                    w_u32(object + 104, 512);
                }
                result = r_u32(object + 96) + 1;
                w_u32(object + 96, result);
                return result;
            }
            if (phase != 2)
                return 2;
            a = r_u32(data + 16);
            b = r_u32(object + 12);
            if ((sint32)a < (sint32)b)
                w_u32(object + 12, a);
            target[0] = r_u32(data + 12);
            target[1] = r_u32(data + 16);
            target[2] = r_u32(data + 20);
            distance = sub_80052d70(object + 8, 0, 0, 0, target, direction);
            pointer = r_u32(0x800BCEB4);
            flags = r_u32(pointer);
            if ((flags & 0x40) == 0 && distance == 0)
            {
                pointer = r_u32(0x800BCEAC);
                sub_80054ebc(r_u32(pointer + 328));
                pointer = r_u32(0x800BCEB4);
                flags = r_u32(pointer);
                w_u32(pointer, flags | 0x40);
            }
            speed = r_u32(data + 8);
            w_u32(object + 56, (uint32)((sint32)(speed * direction[0]) >> 14));
            speed = r_u32(data + 8);
            w_u32(object + 60, (uint32)((sint32)(speed * direction[1]) >> 14));
            speed = r_u32(data + 8);
            phase = r_s16(object + 6);
            w_u32(object + 64, (uint32)((sint32)(speed * direction[2]) >> 14));
            if (phase == 0x4000)
                w_u32(object + 104, 0xFFFFFE00);
            else if (phase == 0x2000)
                w_u32(object + 104, 512);
            a = r_u16(object + 6);
            b = r_u16(object + 104);
            result = a + b;
            w_u16(object + 6, result);
            return result;
        case 7:
            pointer = r_u32(0x800BCE5C);
            a = r_u32(pointer);
            a = (uint32)(r_s32(a + 8) >> 12);
            b = r_u32(pointer);
            b = (uint32)(r_s32(b + 12) >> 12);
            c = r_u32(pointer);
            c = (uint32)(r_s32(c + 16) >> 12);
            result = (sint32)a < -20479;
            if (result != 0)
                return result;
            result = (sint32)a < -18432;
            if (result == 0)
                return result;
            result = (sint32)b < -1023;
            if (result != 0)
                return result;
            result = (sint32)c < -6144;
            if (result == 0)
                return result;
            result = (sint32)c < -22527;
            if (result != 0)
                return result;
            goto mark_object;
        case 8:
            pointer = r_u32(0x800BCE5C);
            pointer = r_u32(pointer);
            a = r_u32(object + 16);
            b = r_u32(pointer + 16);
            c = r_u32(data + 12);
            result = (sint32)(a - b) < (sint32)c;
            if (result == 0)
                return result;
        mark_object:
            flags = r_u8(object + 5);
            result = ~flags & 4;
            if (result != 0)
            {
                pointer = 0x800C6FD0 + (index + 32) * 4;
                result = r_u32(pointer) | 0x20000;
                w_u32(pointer, result);
            }
            return result;
        case 9:
            pointer = r_u32(0x800BCE5C);
            pointer = r_u32(pointer);
            if (index == 0)
            {
                nearest = 0x7FFFFFFF;
                for (scan = 0; scan < 3; ++scan)
                {
                    distance = sub_80083dfc(0x800B2078 + scan * 16, pointer + 8, 0, 0);
                    if ((sint32)distance < (sint32)nearest)
                    {
                        nearest = distance;
                        w_u16(0x800BD008, scan);
                    }
                    a = r_s16(0x800BCFFC + scan * 2);
                    b = r_u16(0x800BCFFC + scan * 2);
                    if ((sint32)a > 0)
                    {
                        game = r_u32(0x800BCE54);
                        timer = r_u32(game + 12);
                        if ((timer & 1) != 0)
                            w_u16(0x800BCFFC + scan * 2, b - 1);
                    }
                }
                selected = r_s16(0x800BD008);
                a = r_s16(0x800BCFFC + selected * 2);
                b = r_u16(0x800BCFFC + selected * 2);
                if ((sint32)a > 0 && (sint32)nearest < 0x4000)
                    w_u16(0x800BCFFC + selected * 2, b - 16);
                selected = r_s16(0x800BD008);
                a = r_s16(0x800BCFFC + selected * 2);
                if ((sint32)a < 0)
                    w_u16(0x800BCFFC + selected * 2, 0);
                game = r_u32(0x800BCE54);
                value = r_u32(game + 4);
                if (value == 4)
                {
                    if ((sint32)nearest < 12288)
                    {
                        pointer = r_u32(0x800BCEB4);
                        flags = r_u32(pointer);
                        strength = r_u32(0x800BCFF8);
                        w_u32(pointer, flags | 8);
                        a = r_u32(game + 16);
                        b = r_u32(game + 20);
                        value = sub_8008fc9c(a, b, strength);
                        game = r_u32(0x800BCE54);
                        strength = r_u32(0x800BCFF8);
                        w_u32(game + 20, value);
                        if ((sint32)strength < 1024)
                            w_u32(0x800BCFF8, strength + 16);
                        timer = r_u32(game + 12);
                        if ((timer & 31) == 0)
                            sub_80076fa4();
                    }
                    else
                    {
                        pointer = r_u32(0x800BCEB4);
                        strength = r_u32(0x800BCFF8);
                        flags = r_u32(pointer);
                        strength -= 64;
                        w_u32(0x800BCFF8, strength);
                        w_u32(pointer, flags & 0xFFFFFFF7);
                        if ((sint32)strength < 0)
                            w_u32(0x800BCFF8, 0);
                        strength = r_u32(0x800BCFF8);
                        if ((sint32)strength > 0)
                        {
                            a = r_u32(game + 16);
                            b = r_u32(game + 20);
                            value = sub_8008fc9c(a, b, strength);
                            pointer = r_u32(0x800BCE54);
                            w_u32(pointer + 20, value);
                        }
                    }
                }
            }
            flags = r_u8(object + 5);
            w_u8(object + 5, flags | 4);
            selected = r_s16(0x800BD008);
            descriptors = r_u32(0x800BCEFC);
            a = r_u16(0x800BCFFC + selected * 2);
            pointer = r_u32(descriptors + index * 40 + 8);
            b = 3 - (uint32)((sint32)(a << 16) >> 28);
            a = r_u8(pointer + 2);
            w_u8(object + 4, a + (b << 1));
            selected = r_s16(0x800BD008);
            a = r_u16(0x800BCFFC + selected * 2);
            w_u16(object + 6, a & 0xFFF);
            pointer = 0x800B2078 + (selected << 4);
            a = r_u32(pointer);
            w_u32(object + 8, a + ((index & 1) << 24) - 0x800000);
            a = r_u32(pointer + 4);
            w_u32(object + 12, a);
            a = r_u32(pointer + 8);
            result = 0x800000;
            w_u32(object + 16, a - ((uint32)((sint32)index >> 1) << 24) + result);
            return result;
        default:
            return 0;
    }
}

uint32 sub_80037e50(void)
{
    uint32 state, counter, requests, mode, flags, parameter, game, mission, value;
    FUNCTION_MARKER(0x80037E50, "ACE3.BIN");
    state = r_u32(0x800BCEB4);
    counter = r_u8(state + 63);
    w_u8(state + 63, counter + 1);
    state = r_u32(0x800BCEB4);
    requests = r_u8(state + 42);
    if ((requests & 1) != 0)
    {
        w_u8(state + 42, requests & 0xFE);
        return sub_80032f18(128, 0);
    }
    if ((requests & 2) != 0)
    {
        w_u8(state + 42, requests & 0xFD);
        return sub_80032f18(0x100000, 0);
    }
    if ((requests & 4) != 0)
    {
        w_u8(state + 42, requests & 0xFB);
        sub_80079924();
        return sub_80032f18(0x200000, 0);
    }
    mode = r_u8(state + 41);
    switch (mode)
    {
        case 0:
            parameter = r_u8(state + 35);
            flags = 2;
            w_u8(state + 61, parameter);
            break;
        case 1:
            flags = 16;
            game = r_u32(0x800BCE60);
            parameter = r_u8(state + 36);
            mission = r_s16(game + 908);
            sub_80038054(mission, parameter);
            break;
        case 2:
            flags = 0x4000;
            game = r_u32(0x800BCE60);
            parameter = r_u8(state + 38);
            mission = r_s16(game + 908);
            sub_80038078(mission, parameter);
            break;
        case 3:
            parameter = r_u8(state + 37);
            flags = 1024;
            break;
        case 4:
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            flags = 0x1000000;
            w_u32(game + 928, 3);
            value = r_u8(state + 38);
            parameter = 0;
            sub_80038078(mission, value);
            sub_80048a40();
            sub_8003048c();
            break;
        case 5:
            flags = 0x2000000;
            sub_80048a40();
            game = r_u32(0x800BCE60);
            value = r_u8(game + 957);
            w_u16(game + 908, value);
            sub_8003048c();
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 41);
            parameter = 0;
            if (value == mode)
            {
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                if (mission == 54)
                {
                    w_u8(game + 937, mode);
                    value = r_u32(0x800BCE60);
                    w_u16(game + 910, 25);
                    w_u8(value + 938, 1);
                    value = r_u32(0x800BCE60);
                    w_u8(value + 939, 1);
                }
            }
            break;
        case 9:
            flags = 0x800000;
            parameter = 0;
            sub_80048a40();
            sub_8003048c();
            break;
        default:
            break;
    }
    return sub_80032f18(flags, parameter);
}

uint32 sub_8003f520(void)
{
    uint16 left, right, top, bottom;
    uint32 index, flag_address, flags, descriptor, kind, color, a, b, c, metric, value, shifted, sign, high, thousands, hundreds, tens, packet, buffers, count, field, glyph;
    FUNCTION_MARKER(0x8003F520, "ACE3.BIN");
    index = 0;
    flag_address = 0x800C6FD0;
    do
    {
        flags = r_u32(flag_address);
        if ((flags & 0x1000) == 0)
            goto next_descriptor;
        descriptor = r_u32(0x800C67A0 + index * 4);
        if (descriptor == 0)
            goto next_descriptor;
        kind = r_u8(descriptor);
        if (kind == 0)
            goto next_descriptor;
        switch (kind)
        {
            case 1:
                color = 0xFF00;
                break;
            case 2:
                value = r_u32(0x800C6D60);
                color = (value & 2) != 0 ? 0x40FF40 : 0;
                break;
            case 3:
                color = 255;
                break;
            default:
                break;
        }
        flags = r_u32(flag_address);
        if ((flags & 0x40000) != 0 && r_u8(descriptor + 5) != 1)
        {
            a = r_u16(descriptor + 8);
            b = r_u16(descriptor + 6);
            left = a - b;
            c = r_u16(descriptor + 10);
            right = a + b;
            top = c - 4;
            bottom = c - 4;
        }
        else
        {
            a = r_u16(descriptor + 8);
            b = r_u16(descriptor + 6);
            left = a - b;
            c = r_u16(descriptor + 10);
            right = a + b;
            top = c - b - 4;
            bottom = c + b;
        }
        kind = r_u8(descriptor);
        if (kind != 1)
        {
            metric = r_u32(0x800C6ED0 + index * 4);
            if (metric == 0)
                goto next_descriptor;
            flags = r_u32(flag_address);
            if ((flags & 0x400) != 0)
                sub_80040530(index);
            value = r_u32(metric + 4);
            packet = r_u32(0x800BCFBC);
            shifted = (uint32)((sint32)value >> 2);
            sign = (uint32)((sint32)value >> 31);
            high = (uint32)(((sint64)(sint32)shifted * 0x10624DD3) >> 32);
            thousands = (uint32)((sint32)high >> 6) - sign;
            buffers = r_u32(0x800BCFB8);
            count = r_u32(buffers + 20);
            packet += count * 20 + 5384;
            sub_8003b7b0(thousands, (sint16)right + 2, (sint16)bottom, 2, packet, 0);
            packet += 20;
            high = (uint32)(((sint64)(sint32)shifted * 0x51EB851F) >> 32);
            buffers = r_u32(0x800BCFB8);
            count = r_u32(buffers + 20);
            w_u32(buffers + 20, count + 1);
            hundreds = (uint32)((sint32)high >> 5) - sign;
            glyph = hundreds - thousands * 10;
            sub_8003b7b0(glyph, (sint16)right + 7, (sint16)bottom, 2, packet, 0);
            packet += 20;
            high = (uint32)(((sint64)(sint32)shifted * 0x66666667) >> 32);
            buffers = r_u32(0x800BCFB8);
            count = r_u32(buffers + 20);
            w_u32(buffers + 20, count + 1);
            tens = (uint32)((sint32)high >> 2) - sign;
            glyph = tens - hundreds * 10;
            sub_8003b7b0(glyph, (sint16)right + 12, (sint16)bottom, 2, packet, 0);
            packet += 20;
            glyph = shifted - tens * 10;
            buffers = r_u32(0x800BCFB8);
            count = r_u32(buffers + 20);
            w_u32(buffers + 20, count + 1);
            sub_8003b7b0(glyph, (sint16)right + 17, (sint16)bottom, 2, packet, 0);
            buffers = r_u32(0x800BCFB8);
            count = r_u32(buffers + 20);
            w_u32(buffers + 20, count + 1);
            field = r_u8(descriptor + 3);
            sub_8004084c(field, (sint16)left, (sint16)top);
        }
        field = r_u8(descriptor + 4);
        if (field != 255)
            sub_80040900(field, (sint16)right, (sint16)top);
        field = r_u8(descriptor + 5);
        if (field == 1)
            sub_80040740(1, (sint16)left, (sint16)bottom);
        else if (field == 2)
        {
            field = r_u8(descriptor + 5);
            sub_80040740(field, (sint16)left, (sint16)bottom);
        }
        else if (field == 3)
        {
            color = 0xFF4000;
            field = r_u8(descriptor + 3);
            sub_8004084c(field, (sint16)left, (sint16)top);
            field = r_u8(descriptor + 5);
            sub_80040740(field, (sint16)left, (sint16)bottom);
        }
        flags = r_u32(flag_address);
        if ((flags & 0x40000) != 0 && r_u8(descriptor) == 1)
        {
            field = r_u8(descriptor + 3);
            sub_8004084c(field, (sint16)left, (sint16)top);
        }
        else
        {
            value = r_u32(0x800BCEB4);
            flags = r_u32(value);
            if ((flags & 4) != 0)
                sub_800401ac(index, color);
            else
            {
                field = r_u8(descriptor + 2) & 15;
                if (field == 1)
                    sub_8003fa80(index, color);
                else if (field == 2)
                    sub_8003fd48(index, color);
                else if (field == 3)
                    sub_800401ac(index, color);
            }
        }
    next_descriptor:
        flag_address += 4;
        index += 1;
    } while (index < 64);
    return 0;
}

uint32 sub_8005d468(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 health, percent, value, red, green, high, product, quotient, step, packet, tag, head, flags, color, result;
    FUNCTION_MARKER(0x8005D468, "ACE3.BIN");
    value = native_state != 0 ? xport_load_le32(native_state + 12) : r_u32(state + 12);
    health = 4096;
    if ((sint32)value < 4097)
    {
        health = 0;
        if ((sint32)value >= 0)
            health = value;
    }
    value = native_state != 0 ? native_state[32] : r_u8(state + 32);
    w_u8(dst + 4, value);
    value = native_state != 0 ? native_state[33] : r_u8(state + 33);
    w_u8(dst + 5, value);
    percent = (uint32)((sint32)(health * 100) >> 12);
    value = native_state != 0 ? native_state[34] : r_u8(state + 34);
    w_u8(dst + 6, value);
    value = r_u8(0x800C6D6F);
    if (value != 0 && ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 32) != 0)
    {
        value = native_state != 0 ? native_state[32] : r_u8(state + 32);
        w_u8(dst + 24, value);
        value = native_state != 0 ? native_state[33] : r_u8(state + 33);
        w_u8(dst + 25, value);
        value = native_state != 0 ? native_state[34] : r_u8(state + 34);
        w_u8(dst + 26, value);
    }
    else
    {
        product = percent * 131;
        high = (uint32)(((sint64)(sint32)product * 0x51EB851F) >> 32);
        green = (uint32)((sint32)high >> 5) - (uint32)((sint32)product >> 31);
        red = 131 - green;
        if ((sint32)percent < 20)
        {
            flags = native_state != 0 ? xport_load_le32(native_state + 40) : r_u32(state + 40);
            if ((flags & 1) != 0)
                red = (red & 255) >> 2;
        }
        w_u8(dst + 24, red);
        w_u8(dst + 25, green);
        w_u8(dst + 26, 0);
    }
    step = 15 - (uint32)((sint32)health >> 8);
    if ((sint32)step < 0)
        step = 0;
    w_u16(dst + 34, (step & 3) + ((step & 12) << 4) + 0x7E18);
    tag = r_u32(dst + 20);
    head = r_u32(object);
    w_u32(dst + 20, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    w_u32(object, (head & 0xFF000000) | (dst & 0xFFFFFF));
    flags = native_state != 0 ? xport_load_le32(native_state) : r_u32(state);
    if ((flags & 32) != 0)
    {
        value = r_u8(0x800C6D6F);
        if (value != 0)
        {
            value = native_state != 0 ? native_state[32] : r_u8(state + 32);
            w_u8(dst + 44, value);
            value = native_state != 0 ? native_state[33] : r_u8(state + 33);
            w_u8(dst + 45, value);
            value = native_state != 0 ? native_state[34] : r_u8(state + 34);
            w_u8(dst + 46, value);
        }
        else
        {
            w_u8(dst + 44, red);
            w_u8(dst + 45, green);
            w_u8(dst + 46, 0);
        }
        tag = r_u32(dst + 40);
        head = r_u32(object);
        w_u32(dst + 40, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | ((dst + 40) & 0xFFFFFF));
    }
    percent = 100 - percent;
    packet = dst + 60;
    if ((sint32)percent >= 100)
    {
        color = native_state != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_state + 36) : (uint32)r_s16(state + 36);
        sub_8003b7b0(1, 238, 219, 1, packet, color);
        tag = r_u32(dst + 60);
        head = r_u32(object);
        w_u32(dst + 60, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        packet = dst + 80;
        w_u32(object, (head & 0xFF000000) | ((dst + 60) & 0xFFFFFF));
    }
    if ((sint32)percent >= 10)
    {
        high = (uint32)(((sint64)(sint32)percent * 0x66666667) >> 32);
        quotient = (uint32)((sint32)high >> 2) - (uint32)((sint32)percent >> 31);
        high = (uint32)(((sint64)(sint32)quotient * 0x66666667) >> 32);
        value = (uint32)((sint32)high >> 2) - (uint32)((sint32)quotient >> 31);
        value = quotient - value * 10;
        color = native_state != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_state + 36) : (uint32)r_s16(state + 36);
        sub_8003b7b0(value, 244, 219, 1, packet, color);
        tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 20;
    }
    high = (uint32)(((sint64)(sint32)percent * 0x66666667) >> 32);
    quotient = (uint32)((sint32)high >> 2) - (uint32)((sint32)percent >> 31);
    value = percent - quotient * 10;
    color = native_state != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_state + 36) : (uint32)r_s16(state + 36);
    sub_8003b7b0(value, 252, 219, 1, packet, color);
    tag = r_u32(packet);
    head = r_u32(object);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8005bc64(void)
{
    uint32 state, mask, scene, timer, value, first, second, scenario, result;
    FUNCTION_MARKER(0x8005BC64, "ACE3.BIN");
    state = r_u32(0x800BCEB4);
    value = r_u8(state + 63);
    mask = r_u32(0x800BD058);
    if (value == 1 && (mask & 0x6000) != 0)
    {
        scene = r_u32(0x800BCE54);
        timer = r_u32(scene + 12);
        if ((sint32)timer < 16)
        {
            first = r_u32(scene + 16);
            second = r_u32(scene + 20);
            value = sub_80084560(first + 8, second, 4096 - (timer << 8));
            scene = r_u32(0x800BCE54);
            w_u32(scene + 20, value);
        }
    }
    scene = r_u32(0x800BCE54);
    timer = r_u32(scene + 12);
    if ((sint32)timer >= 61 && sub_8005650c() == 0)
    {
        if ((mask & 0x8000) != 0)
            sub_800762d0();
        else
            sub_80076f04();
        if ((mask & 0x300) != 0)
            scenario = 11;
        else if ((mask & 0x400) != 0)
            scenario = 19;
        else if ((mask & 0x800) != 0)
            scenario = 29;
        else if ((mask & 0x2000) != 0)
            scenario = 15;
        else if ((mask & 0x4000) != 0)
            scenario = 27;
        else if ((mask & 0x1000) != 0)
            scenario = 39;
        else if ((mask & 0x8000) != 0)
            scenario = 45;
        sub_8004ceb0(scenario);
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        sub_80032f18(0x800, value);
    }
    result = sub_8006cb30() & 0x7000;
    if (result != 0)
    {
        sub_80056518();
        state = r_u32(0x800BCEB4);
        value = r_u8(state + 40);
        w_u8(state + 37, value);
        sub_80032f18(0x2000, 1);
        state = r_u32(0x800BCE60);
        result = r_u32(state) | 0x10000000;
        w_u32(state, result);
    }
    return result;
}

uint32 sub_8005be0c(void)
{
    uint32 input, scene, stage, scenario, mask, threshold, fade_time, state, value, result;
    FUNCTION_MARKER(0x8005BE0C, "ACE3.BIN");
    input = sub_8006cb30();
    scene = r_u32(0x800BCE54);
    stage = r_u32(scene + 8);
    scenario = 255;
    if ((stage & 2) != 0)
    {
        if ((input & 0x7000) != 0)
        {
            sub_80056518();
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 40);
            w_u8(state + 37, value);
            state = r_u32(0x800BCEB4);
            scene = r_u32(0x800BCE54);
            value = r_u8(state + 40);
            w_u32(scene + 8, value);
            sub_80032f18(0x2000, 1);
            state = r_u32(0x800BCE60);
            result = r_u32(state) | 0x10000000;
            w_u32(state, result);
            return result;
        }
        mask = r_u32(0x800BD058);
        if ((mask & 0x300) != 0)
        {
            scenario = 12;
            threshold = 270;
            fade_time = threshold;
            if (r_u32(scene + 12) == 0)
            {
                sound_set_voice_fade(0, 0, 150);
                sound_set_voice_fade(1, 30, 150);
                sound_set_voice_fade(2, 20, 150);
            }
        }
        else if ((mask & 0x400) != 0)
        {
            scenario = 20;
            threshold = 360;
            fade_time = threshold;
        }
        else if ((mask & 0x800) != 0)
        {
            scenario = 43;
            threshold = 360;
            fade_time = threshold;
        }
        else if ((mask & 0x6000) != 0)
        {
            scenario = 49;
            threshold = 120;
            fade_time = 0xFFFFFFFF;
        }
        else if ((mask & 0x1000) != 0)
        {
            scenario = 47;
            threshold = 180;
            fade_time = threshold;
        }
        else if ((mask & 0x8000) != 0)
        {
            threshold = 90;
            fade_time = threshold;
            input = 0x200;
        }
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) == fade_time)
            goto start_fade;
    after_fade:
        if ((input & 0x200) == 0)
            goto finish;
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 12);
        if ((sint32)threshold >= (sint32)value)
            goto finish;
    }
    else if ((stage & 0x20) != 0)
    {
        if ((input & 0x7000) != 0)
        {
            sub_80056518();
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 40);
            w_u8(state + 37, value);
            state = r_u32(0x800BCEB4);
            scene = r_u32(0x800BCE54);
            value = r_u8(state + 40);
            w_u32(scene + 8, value);
            sub_80032f18(0x2000, 1);
            state = r_u32(0x800BCE60);
            value = r_u32(state) | 0x10000000;
            w_u32(state, value);
        }
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) == 120)
            goto start_fade;
    after_stage_fade:
        scene = r_u32(0x800BCE54);
        if (r_s32(scene + 12) < 120)
            goto finish;
    }
    else if (stage != 0)
    {
        mask = r_u32(0x800BD058);
        if ((mask & 0x300) != 0)
            scenario = 41;
        else if ((mask & 0x400) != 0)
            scenario = 42;
        else if ((mask & 0x800) != 0)
            scenario = 44;
        else if ((mask & 0x1000) != 0)
            scenario = 48;
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) == 0)
            sub_80056518();
        scene = r_u32(0x800BCE54);
        if (r_s32(scene + 12) >= 120)
        {
            sub_8004e060();
            sub_8003048c();
            sub_80056518();
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 40);
            w_u8(state + 37, value);
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 40);
            sub_80032f18(0x1000, value);
            sub_80079924();
        }
        goto finish;
    }
    else
    {
        if (r_s32(scene + 12) < 150 || sub_8005650c() != 0)
            goto finish;
        goto end_scene;
    }
    if (sub_8005650c() != 0)
        goto finish;
    state = r_u32(0x800BCEB4);
    if (r_u8(state + 48) != 0)
        goto finish;
end_scene:
    sub_8004e060();
    state = r_u32(0x800BCEB4);
    if (r_u8(state + 41) == 0)
        sub_800304ec();
    else
        sub_8003048c();
    sub_80037e50();
finish:
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 12);
    result = 255;
    if (value == 0 && scenario != 255)
        result = sub_8004ceb0(scenario);
    return result;
start_fade:
    state = r_u32(0x800BCEB4);
    w_u8(state + 49, 2);
    state = r_u32(0x800BCEB4);
    w_u8(state + 48, 1);
    state = r_u32(0x800BCEB4);
    w_u16(state + 50, 30);
    w_u16(state + 26, 0);
    if ((stage & 2) != 0)
        goto after_fade;
    goto after_stage_fade;
}

uint32 sub_8003346c(void)
{
    uint8 geometry[280];
    uint16 angles[3];
    uint32 position[3], scene, camera, value, a, b, c, ordering, packet, terrain, records, count, game, table, index, rectangle, kind, result;
    FUNCTION_MARKER(0x8003346C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 20);
    angles[0] = (uint16)((sint32)(value << 16) >> 18);
    value = r_u16(camera + 22);
    angles[1] = (uint16)((sint32)(value << 16) >> 18);
    value = r_u16(camera + 24);
    angles[2] = (uint16)((sint32)(value << 16) >> 18);
    if ((r_u32(scene) & 0x100) == 0)
    {
        sub_80061614(camera + 4, 0, camera + 28, 0, 0, angles, geometry);
        scene = r_u32(0x800BCE54);
        a = r_u32(scene + 76);
        b = r_u32(scene + 80);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(scene + 84);
        b = r_u32(scene + 88);
        c = r_u32(scene + 92);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        a = r_u32(scene + 96);
        b = r_u32(scene + 100);
        xport_gte_write_control(5, a);
        c = r_u32(scene + 104);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8005e6dc(scene + 580, 0, ordering + 4, packet, geometry);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    scene = r_u32(0x800BCE54);
    if ((r_u32(scene) & 8) != 0)
    {
        position[0] = r_u32(camera + 4) + 0x40000000;
        position[1] = r_u32(camera + 8) + 0xFF400000;
        position[2] = r_u32(camera + 12);
        sub_80061614(0, 0, camera + 28, 0, position, angles, geometry);
        scene = r_u32(0x800BCE54);
        a = r_u32(scene + 76);
        b = r_u32(scene + 80);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(scene + 84);
        b = r_u32(scene + 88);
        c = r_u32(scene + 92);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        a = r_u32(scene + 96);
        b = r_u32(scene + 100);
        xport_gte_write_control(5, a);
        c = r_u32(scene + 104);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8005e6dc(scene + 580, 0, ordering + 4, packet, geometry);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    scene = r_u32(0x800BCE54);
    if ((r_u32(scene) & 4) != 0)
    {
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8005eff0(ordering + 4, packet);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    scene = r_u32(0x800BCE54);
    terrain = r_u32(scene + 580);
    count = r_u32(terrain + 116);
    records = terrain + 120;
    if (count != 0)
    {
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8005edc8(ordering + 4, packet, records, count);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    scene = r_u32(0x800BCE54);
    if ((r_u32(scene) & 0x200) == 0)
    {
        game = r_u32(0x800BCE60);
        packet = r_u32(scene + 20);
        index = r_u32(game + 4);
        ordering = r_u32(scene + 16);
        table = r_u32(0x800BCE58);
        rectangle = r_u32(table + index * 4116);
        result = sub_800943a8(ordering + 4, packet, rectangle, scene + 664);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    scene = r_u32(0x800BCE54);
    terrain = r_u32(scene + 580);
    ordering = r_u32(scene + 16);
    kind = r_u16(terrain + 2) & 7;
    ordering += 16380;
    if (kind == 2)
    {
        packet = r_u32(scene + 20);
        result = sub_8009754c(0, ordering, packet, geometry);
    }
    else if (kind < 3)
    {
        if (kind != 1)
            return 1;
        packet = r_u32(scene + 20);
        result = sub_800957a4(ordering, packet);
    }
    else if (kind == 3)
    {
        packet = r_u32(scene + 20);
        result = sub_80097eb4(ordering, packet);
    }
    else if (kind == 4)
    {
        packet = r_u32(scene + 20);
        result = sub_80098044(ordering, packet);
    }
    else
        return 4;
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    return result;
}

uint32 sub_800271a0(uint32 resource)
{
    uint32 result;
    /* Defines the descriptor when the SDK rejects the input */
    uint32 image[5] = {0};
    FUNCTION_MARKER(0x800271A0, "ACE3.BIN");
    result = read_tim_native(resource, image);
    if (result == 0)
    {
#ifndef NDEBUG
        result = printf("Can't read TIM [%08x]\n", (uint32)(resource));
#else
        result = 0;
#endif
    }
    if (image[4] != 0)
        result = sub_800a8440(image[3], image[4], 0, 0, 0);
    if (image[2] != 0)
        return sub_800a8440(image[1], image[2], 0, 0, 0);
    return result;
}

uint32 read_tim_native(uint32 resource, uint32 image[5])
{
    GpuTimImage decoded;
    const uint8 *source, *body;
    uintptr_t offset;
    size_t size;
    source = psx_addr(resource, 8);
    if (!source)
        return 0;
    offset = (uintptr_t)source - (uintptr_t)DRAM;
    if (offset > sizeof(DRAM) - 8 || r_u32(resource) != 16)
        return 0;
    size = sizeof(DRAM) - (size_t)offset - 4;
    body = psx_addr(resource + 4, size);
    if (!body || !gpu_decode_tim_body(body, size, &decoded))
        return 0;
    image[0] = decoded.mode;
    image[2] = decoded.clut_data ? resource + 4 + (uint32)(decoded.clut_data - body) : 0;
    image[1] = image[2] ? image[2] - 8 : 0;
    image[4] = resource + 4 + (uint32)(decoded.pixel_data - body);
    image[3] = image[4] - 8;
    return 1;
}

void sub_8002722c(uint32 data, uint32 count)
{
    uint32 index, resource;
    /* Retains the previous descriptor after a later rejected TIM */
    uint32 image[5] = {0};
    FUNCTION_MARKER(0x8002722C, "ACE3.BIN");
    index = 0;
    if ((sint32)count <= 0)
        return;
    do
    {
        resource = r_u32(data);
        if (read_tim_native(resource, image) == 0)
        {
            resource = r_u32(data);
#ifndef NDEBUG
            printf("Can't read TIM [%08x]\n", (uint32)(resource));
#endif
        }
        if (image[4] != 0)
            sub_800a8440(image[3], image[4], 0, 0, 0);
        if (image[2] != 0)
            sub_800a8440(image[1], image[2], 0, 0, 0);
        index += 1;
        data += 4;
    } while ((sint32)index < (sint32)count);
}

uint32 sub_8004966c(uint32 index)
{
    uint32 target[3], direction[3], table, flag_address, flags, object, state, phase, value, result, first, second, angle;
    /* Defines the output when the original direction helper returns without writing */
    uint8 angles[6] = {0};
    FUNCTION_MARKER(0x8004966C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    flag_address = 0x800C6FD0 + index * 4;
    flags = r_u32(flag_address);
    object = r_u32(table + index * 4);
    w_u32(flag_address, flags | 0x20);
    state = r_u32(0x800BCEC0) + index * 184;
    phase = r_u8(state + 100);
    switch (phase)
    {
        case 0:
            first = r_u32(state + 104);
            second = r_u32(state + 112);
            angle = sub_80030d54((uint32)((sint32)first >> 12), (uint32)((sint32)second >> 12));
            value = r_u32(object + 12) - angle;
            if ((sint32)value < 0)
                value = 0 - value;
            result = 2;
            if ((sint32)value < 1025)
                w_u8(state + 100, result);
            else
            {
                result = 1;
                w_u8(state + 100, result);
            }
            break;
        case 1:
            target[0] = r_u32(state + 104);
            target[1] = r_u32(object + 12);
            target[2] = r_u32(state + 112);
            sub_80044f94(object + 8, 0, 0, target, direction);
            angle = sub_80045090(0, direction);
            sub_80044188(index, 0, (uint32)(sint32)(sint16)angle, 0, angles, direction);
            value = (uint32)(sint32)(sint16)xport_load_le16(angles);
            if ((sint32)value < 0)
                value = 0 - value;
            result = (sint32)value < 16;
            if (result != 0)
            {
                value = (uint32)(sint32)(sint16)xport_load_le16(angles + 2);
                if ((sint32)value < 0)
                    value = 0 - value;
                result = (sint32)value < 16;
                if (result != 0)
                {
                    result = 2;
                    w_u8(state + 100, result);
                }
            }
            break;
        case 2:
            result = sub_80044034(index);
            if (result == 0)
            {
                result = 3;
                w_u8(state + 100, result);
            }
            break;
        case 3:
            sub_80044360(index, state + 104, 0);
            first = r_u32(object + 12);
            second = r_u32(state + 108);
            result = first - second;
            if ((sint32)result < 0)
                result = 0 - result;
            if ((sint32)result <= 0x7FFFF)
            {
                result = 4;
                w_u8(state + 100, result);
            }
            break;
        case 4:
            result = 255;
            w_u8(state + 99, result);
            break;
        default:
            result = 0x80020000;
            w_u8(state + 100, 0);
            break;
    }
    return result;
}

uint32 sub_8002f3c0(uint32 sound, uint32 position, uint32 mode, const uint32 *native_position)
{
    uint32 voice, scene, listener, random;
    /* Defines the previous distance and direction before their first read */
    uint8 movement[12] = {0};
    FUNCTION_MARKER(0x8002F3C0, "ACE3.BIN");
    voice = sub_8002db4c(sound);
    if (voice == 255)
        return 255;
    scene = r_u32(0x800BCE54);
    listener = r_u32(scene + 40);
    sub_8002e6dc(0, listener + 4, position, movement, native_position, 0);
    sub_8002e84c(voice, 0, mode & 255, movement);
    sub_8002e8dc(voice, 0, movement);
    sub_8002ea24(voice, (uint32)(sint32)(sint16)xport_load_le16(movement + 10));
    random = game_random();
    return sub_8002f0d4(voice, (random & 1023) - 512);
}

uint32 sub_8002f460(uint32 sound, uint32 position, uint32 mode, const uint32 *native_position)
{
    uint32 voice, scene, listener, random;
    /* Defines the previous distance and direction before their first read */
    uint8 movement[12] = {0};
    FUNCTION_MARKER(0x8002F460, "ACE3.BIN");
    voice = sub_8002dbe0(sound);
    if (voice == 255)
        return 255;
    scene = r_u32(0x800BCE54);
    listener = r_u32(scene + 40);
    sub_8002e6dc(0, listener + 4, position, movement, native_position, 0);
    sub_8002e84c(voice, 0, mode & 255, movement);
    sub_8002e8dc(voice, 0, movement);
    sub_8002ea24(voice, (uint32)(sint32)(sint16)xport_load_le16(movement + 10));
    random = game_random();
    return sub_8002f0d4(voice, (random & 1023) - 512);
}

uint32 sub_80033104(void)
{
    uint32 result, root, scene, object, state;
    /* Defines the three input words the original caller leaves uninitialized */
    uint32 record[6] = {0};
    FUNCTION_MARKER(0x80033104, "ACE3.BIN");
    sub_8005c270();
    root = r_u32(0x800BCE60);
    result = ~r_u32(root) & 0x20000000;
    if (result == 0)
        return result;
    scene = r_u32(0x800BCE54);
    result = ~r_u32(scene + 4) & 0x1000;
    if (result == 0)
        return result;
    object = r_u32(scene + 40);
    sub_800729dc();
    sub_80045898();
    sub_80038a3c();
    sub_800517f4();
    sub_8004658c();
    sub_8006c5b4();
    sub_8007dac8();
    sub_800850d8();
    state = r_u32(0x800BD098);
    record[0] = r_u32(state + 28);
    sub_8003a23c(0, record);
    state = r_u32(0x800BD098);
    w_u32(object + 188, record[4]);
    result = record[5];
    w_u32(state + 24, result);
    return result;
}

uint32 sub_800331e0(void)
{
    uint32 result, root, scene, object, state;
    /* Defines the three input words the original caller leaves uninitialized */
    uint32 record[6] = {0};
    FUNCTION_MARKER(0x800331E0, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    result = ~r_u32(root) & 0x20000000;
    if (result == 0)
        return result;
    scene = r_u32(0x800BCE54);
    object = r_u32(scene + 40);
    sub_800729dc();
    sub_80045898();
    sub_80038a3c();
    sub_800517f4();
    sub_8004658c();
    sub_8006c5b4();
    sub_8007dac8();
    sub_800850d8();
    state = r_u32(0x800BD098);
    record[0] = r_u32(state + 28);
    sub_8003a23c(0, record);
    state = r_u32(0x800BD098);
    w_u32(object + 188, record[4]);
    result = record[5];
    w_u32(state + 24, result);
    return result;
}

uint32 sub_80027084(uint32 resource, uint32 pixel_x, uint32 pixel_y, uint32 clut_x, uint32 clut_y)
{
    uint32 source, first, second, result;
    uint8 rectangle[8];
    /* Defines both image pointers when ReadTIM rejects the input */
    uint32 image[5] = {0};
    FUNCTION_MARKER(0x80027084, "ACE3.BIN");
    if (read_tim_native(resource, image) == 0)
    {
#ifndef NDEBUG
        printf("Can't read TIM [%08x]\n", (uint32)(r_u32(resource)));
#endif
    }
    if (image[2] != 0)
    {
        source = image[1];
        first = r_u32(source);
        second = r_u32(source + 4);
        xport_store_le32(rectangle, first);
        xport_store_le32(rectangle + 4, second);
        xport_store_le16(rectangle, clut_x);
        xport_store_le16(rectangle + 2, clut_y);
        sub_800a8440(0, image[2], rectangle, 0, 0);
    }
    result = image[4];
    if (result != 0)
    {
        source = image[3];
        first = r_u32(source);
        second = r_u32(source + 4);
        xport_store_le32(rectangle, first);
        xport_store_le32(rectangle + 4, second);
        xport_store_le16(rectangle, pixel_x);
        xport_store_le16(rectangle + 2, pixel_y);
        result = sub_800a8440(0, image[4], rectangle, 0, 0);
    }
    return result;
}

uint32 sub_8005ebb8(uint32 colors, uint32 records, sint32 count)
{
    uint8 rectangle[8], capture_rectangle[8], output[2048];
    /* Defines pixels not written by a failed or incomplete capture */
    uint8 capture[2048] = {0};
    uint32 first, second, result, i, x, y, odd_x, odd_y, factor, value, part;
    sint32 index;
    FUNCTION_MARKER(0x8005EBB8, "ACE3.BIN");
    first = r_u32(0x800BCDCC);
    second = r_u32(0x800BCDD0);
    xport_store_le32(capture_rectangle, first);
    xport_store_le32(capture_rectangle + 4, second);
    DrawSync(0);
    result = sub_800a84a0(0, 0, capture_rectangle, capture, sizeof(capture));
    if (result == 0xFFFFFFFF)
    {
#ifndef NDEBUG
        printf("StoreImage is -1\n");
#endif
    }
    DrawSync(0);
    x = 320;
    y = 384;
    odd_x = 0;
    odd_y = 240;
    i = 0;
    do
    {
        factor = rsin(i << 6);
        sub_8005eb20(0, 0, 1024, colors, factor, output, capture);
        if ((i & 1) != 0)
        {
            xport_store_le16(rectangle, odd_x);
            odd_x += 64;
            xport_store_le16(rectangle + 2, odd_y);
            xport_store_le16(rectangle + 4, 16);
            xport_store_le16(rectangle + 6, 16);
            if ((sint32)odd_x >= 256)
            {
                odd_x = 0;
                odd_y = 496;
            }
            sub_800a8440(0, 0, rectangle, output, sizeof(output));
            for (part = 1; part < 4; ++part)
            {
                value = xport_load_le16(rectangle);
                xport_store_le16(rectangle, value + 16);
                if (part < 3)
                    sub_800a8440(0, 0, rectangle, output + part * 512, sizeof(output) - part * 512);
            }
            i += 1;
            sub_800a8440(0, 0, rectangle, output + 1536, sizeof(output) - 1536);
        }
        else
        {
            xport_store_le16(rectangle, x);
            x += 16;
            xport_store_le16(rectangle + 6, 64);
            xport_store_le16(rectangle + 2, y);
            xport_store_le16(rectangle + 4, 16);
            if ((sint32)x >= 384)
            {
                x = 320;
                y += 64;
            }
            i += 1;
            sub_800a8440(0, 0, rectangle, output, sizeof(output));
        }
        DrawSync(0);
        result = (sint32)i < 16;
    } while (result != 0);
    i = 0;
    if (count <= 0)
        return result;
    do
    {
        index = r_s16(records);
        /* Reject indices outside the original 1024-pixel local array */
        value = index >= 0 && index < 1024 ? xport_load_le16(capture + index * 2) : 0;
        i += 1;
        w_u16(records + 6, value);
        result = (sint32)i < count;
        records += 12;
    } while (result != 0);
    return result;
}

uint32 sub_8005b654(void)
{
    uint32 game, scene, state, stage, mask, special, scenario, value, table, radius, difficulty, random, phase, product, other, a, b, c, e, i;
    sint32 mission;
    /* Defines pose padding and the angle omitted for negative difficulty */
    uint8 pose[24] = {0};
    uint32 angle = 0;
    FUNCTION_MARKER(0x8005B654, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    scene = r_u32(0x800BCE54);
    state = r_u32(0x800BCEB4);
    stage = r_u32(scene + 8);
    value = r_u8(state + 37);
    w_u8(state + 40, value);
    mask = 0;
    special = 0;
    scenario = 0xFFFFFFFF;
    switch (mission)
    {
        case 4:
            if (stage == 0)
            {
                scenario = 0;
                mask = 0x207;
            }
            else if (stage == 1)
            {
                scenario = 1;
                mask = 0x100;
            }
            else if (stage == 2)
            {
                scenario = 2;
                mask = 0x8000;
            }
            break;
        case 7:
            if (stage == 0)
            {
                scenario = 3;
                mask = 0x2001;
                special = 1;
            }
            else if (stage == 1)
            {
                scenario = 4;
                mask = 0x8000;
            }
            else if (stage == 2)
            {
                scenario = 5;
                mask = 0x101;
            }
            break;
        case 9:
            if (stage == 0)
            {
                scenario = 6;
                mask = 0x400;
            }
            else if (stage == 1)
            {
                scenario = 7;
                mask = 0x401;
            }
            break;
        case 14:
            scenario = 8;
            mask = 0x101;
            break;
        case 24:
            scenario = 9;
            mask = 0x402;
            break;
        case 25:
            scenario = 10;
            mask = 0x102;
            break;
        case 29:
            if (stage == 0)
            {
                scenario = 11;
                mask = 0x208;
            }
            else if (stage == 1)
            {
                scenario = 12;
                mask = 0x803;
            }
            else if (stage == 2)
            {
                scenario = 13;
                mask = 0x8000;
            }
            break;
        case 31:
            scenario = 14;
            mask = 0x4009;
            special = 1;
            break;
        case 33:
            scenario = 15;
            mask = 0x104;
            break;
        case 34:
            scenario = 16;
            mask = 0x2003;
            special = 1;
            break;
        case 41:
            if (stage == 0)
            {
                scenario = 17;
                mask = 0x2006;
                special = 1;
            }
            else if (stage == 1)
            {
                scenario = 18;
                mask = 0x105;
            }
            break;
        case 42:
            scenario = 19;
            mask = 0x1000;
            break;
        case 44:
            if (stage == 0)
            {
                scenario = 20;
                mask = 0x803;
            }
            else if (stage == 1)
            {
                scenario = 21;
                mask = 0x20B;
            }
            else if (stage == 2)
            {
                scenario = 22;
                mask = 0x8000;
            }
            break;
        case 50:
            scenario = 23;
            mask = 0x2003;
            special = 1;
            break;
    }
    if (scenario != 0xFFFFFFFF)
        w_u32(0x800BCEA8, scenario);
    w_u32(0x800BD058, mask);
    sub_800428ec();
    if ((mask & 0x6000) != 0)
    {
        state = r_u32(0x800BCEB4);
        w_u8(state + 28, 255);
        state = r_u32(0x800BCEB4);
        w_u32(state + 20, 0);
    }
    else if ((mask & 0x8000) != 0)
    {
        state = r_u32(0x800BCEB4);
        w_u8(state + 28, 0);
    }
    else
    {
        state = r_u32(0x800BCEB4);
        w_u8(state + 28, 1);
        state = r_u32(0x800BCEB4);
        w_u32(state + 20, 5400);
    }
    sub_800849ec();
    sub_8004ce9c();
    if ((mask & 0x8000) == 0)
        sub_80073928(mask);
    sub_8007961c(mask);
    if (special != 0)
        sub_80076dbc();
    else if ((mask & 0x8000) != 0)
    {
        sub_800762d0();
        state = r_u32(0x800BCEB4);
        w_u16(state + 8, 0);
    }
    else if ((mask & 0x1000) != 0)
        sub_800762b0();
    else
        sub_800762d0();
    game = r_u32(0x800BCE60);
    value = r_u32(game);
    w_u32(game, value | 0x10000000);
    sub_80030464();
    sub_8002cc44(0, 0, 0);
    if ((mask & 0x300) != 0)
    {
        table = 0x800B3B94 + (mask & 255) * 56;
        value = r_u32(table);
        xport_store_le32(pose, value);
        value = r_u32(table + 4);
        game = r_u32(0x800BCE60);
        xport_store_le32(pose + 4, value + 0xFF000000);
        value = r_u32(table + 8);
        xport_store_le16(pose + 16, 0);
        xport_store_le32(pose + 8, value);
        value = r_u16(table + 18);
        xport_store_le16(pose + 20, 0);
        xport_store_le16(pose + 18, value);
        mission = r_s16(game + 908);
        radius = mission == 33 ? 0x9000 : 0x6000;
        difficulty = r_u32(game + 916);
        if (difficulty == 0)
            angle = r_u16(table + 18) + 0x2000;
        else if (difficulty == 1)
        {
            random = game_random();
            angle = r_u16(table + 18) + (random & 1023) + 7680;
        }
        else if ((sint32)difficulty >= 2)
        {
            random = game_random();
            angle = r_u16(table + 18) + (random & 4095) + 6144;
            xport_store_le16(pose + 18, angle - 0x2000);
        }
        phase = angle & 0x3FFF;
        value = r_s16(0x800104AC + phase * 2);
        product = radius * value;
        value = r_s16(0x800104AC + (phase + 0x1000) * 2);
        other = radius * value;
        value = xport_load_le32(pose);
        xport_store_le32(pose, value + (uint32)((sint32)product >> 2));
        value = xport_load_le32(pose + 8);
        xport_store_le32(pose + 8, value + (uint32)((sint32)other >> 2));
    }
    else if ((mask & 0x1C00) != 0)
    {
        xport_store_le32(pose, 0);
        xport_store_le32(pose + 4, 0xFD8F0000);
        xport_store_le32(pose + 8, 0);
        xport_store_le16(pose + 16, 0);
        xport_store_le16(pose + 18, 0);
        xport_store_le16(pose + 20, 0);
    }
    else
    {
        value = r_u8(0x800BD05C);
        if (value == 0)
        {
            table = r_u32(0x800BCE5C);
            state = r_u32(0x800BCE5C);
            table = r_u32(table);
            a = r_u32(table + 8);
            b = r_u32(table + 12);
            c = r_u32(table + 16);
            e = r_u32(table + 20);
            xport_store_le32(pose, a);
            xport_store_le32(pose + 4, b);
            xport_store_le32(pose + 8, c);
            xport_store_le32(pose + 12, e);
            xport_store_le32(pose + 4, 0xFD8F0000);
            xport_store_le16(pose + 16, 0);
            table = r_u32(state);
            value = r_u16(table + 26);
            xport_store_le16(pose + 20, 0);
            xport_store_le16(pose + 18, value);
            for (i = 0; i < 6; ++i)
                w_u32(0x800C79B0 + i * 4, xport_load_le32(pose + i * 4));
        }
        else
        {
            a = r_u32(0x800C79B0);
            b = r_u32(0x800C79B4);
            c = r_u32(0x800C79B8);
            xport_store_le32(pose, a);
            xport_store_le32(pose + 4, b);
            xport_store_le32(pose + 8, c);
            a = r_u32(0x800C79BC);
            b = r_u32(0x800C79C0);
            c = r_u32(0x800C79C4);
            xport_store_le32(pose + 12, a);
            xport_store_le32(pose + 16, b);
            xport_store_le32(pose + 20, c);
        }
        value = r_u8(0x800BD05C);
        w_u8(0x800BD05C, value + 1);
    }
    if ((mask & 0x6000) == 0)
        sub_80072780(0, pose);
    sub_8004dc38();
    sub_8004e80c();
    return sub_8003a07c();
}

uint32 sub_8005f90c(uint32 geometry, uint32 position, uint32 angles, uint32 state, const uint32 *native_position, const uint16 *native_angles, uint8 *native_state)
{
    /* Defines failed plane intersections and unused vector padding */
    uint32 points[28][4] = {0}, origin[3], chosen[4], ordered[4], indices[7], final_x[7], final_z[7];
    uint16 corners[4][3], projected[4][3], xy[2], local_angles[3];
    MATRIX matrix = {0}, transpose = {0}, identity = {0};
    uint32 projection, value, a, b, width, height, numerator, flags, selected, below, full, count, minimum, maximum, opposite, plane_kind, i, j, candidate, index, result, minimum_x, minimum_z;
    FUNCTION_MARKER(0x8005F90C, "ACE3.BIN");
    {
        if (native_state != 0)
            xport_store_le32(native_state + 24, 0);
        else
            w_u32(state + 24, 0);
    }
    projection = sub_800a70e8();
    value = r_u16(geometry + 4);
    width = (sint32)(value << 16) >> 17;
    numerator = (width << 15) - width;
    if (projection == 0)
        xport_mips_break(7);
    if (projection == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    width = (sint32)numerator / (sint32)projection;
    a = (native_position != 0 ? native_position[0] : r_u32(position));
    value = r_u16(geometry + 6);
    origin[0] = (sint32)a >> 12;
    a = (native_position != 0 ? native_position[1] : r_u32(position + 4));
    origin[1] = (sint32)a >> 12;
    a = (native_position != 0 ? native_position[2] : r_u32(position + 8));
    origin[2] = (sint32)a >> 12;
    {
        if (native_state != 0)
            xport_store_le32(native_state, origin[0]);
        else
            w_u32(state, origin[0]);
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 4, origin[1]);
        else
            w_u32(state + 4, origin[1]);
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 8, origin[2]);
        else
            w_u32(state + 8, origin[2]);
    }
    height = (sint32)(value << 16) >> 17;
    numerator = (height << 15) - height;
    value = (native_angles != 0 ? native_angles[0] : r_u16(angles));
    {
        if (native_state != 0)
            xport_store_le16(native_state + 16, value);
        else
            w_u16(state + 16, value);
    }
    value = (native_angles != 0 ? native_angles[1] : r_u16(angles + 2));
    {
        if (native_state != 0)
            xport_store_le16(native_state + 18, value);
        else
            w_u16(state + 18, value);
    }
    if (projection == 0)
        xport_mips_break(7);
    if (projection == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    height = (sint32)numerator / (sint32)projection;
    value = (native_angles != 0 ? native_angles[2] : r_u16(angles + 4));
    {
        if (native_state != 0)
            xport_store_le16(native_state + 20, value);
        else
            w_u16(state + 20, value);
    }
    local_angles[0] = (native_angles != 0 ? native_angles[0] : r_u16(angles));
    local_angles[1] = 0;
    local_angles[2] = (native_angles != 0 ? native_angles[2] : r_u16(angles + 4));
    below = (sint32)origin[1] < -3000;
    RotMatrixYXZ((SVECTOR *)local_angles, &matrix);
    TransposeMatrix(&matrix, &transpose);
    matrix.t[0] = 0;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    transpose.t[0] = 0;
    transpose.t[1] = 0;
    transpose.t[2] = 0;
    for (i = 0; i < 4; ++i)
    {
        corners[i][0] = i == 0 || i == 3 ? 0 - width : width;
        corners[i][1] = i < 2 ? 0 - height : height;
        corners[i][2] = 32767;
    }
    sky_load_rotation(&matrix);
    sky_load_translation(&matrix);
    for (i = 0; i < 4; ++i)
        sky_transform(corners[i], points[i]);
    for (i = 0; i < 8; ++i)
        xport_store_le32((uint8 *)&identity + i * 4, r_u32(0x800B2BF8 + i * 4));
    indices[0] = 0xFFFFFFFF;
    indices[2] = sub_8005f174(0, 4, (const uint8 *)points);
    indices[5] = sub_8005f124(0, 4, (const uint8 *)points);
    flags = 15;
    selected = 37;
    for (i = 0; i < 4; ++i)
    {
        if (sub_8005f510(0, 0, 0, 0, origin, points[i], points[i + 8]) != 0)
            flags |= 256 << i;
        if (sub_8005f510(0, 0, (uint32)-3000, 0, origin, points[i], points[i + 12]) != 0)
            flags |= 4096 << i;
    }
    if ((flags & 0xF00) != 0)
    {
        sky_load_rotation(&identity);
        sky_load_translation(&identity);
        for (i = 0; i < 4; ++i)
        {
            sub_8005f7fc(0, 0, points[i + 8], xy);
            projected[i][0] = xy[0];
            projected[i][1] = xy[1];
        }
        for (i = 0; i < 4; ++i)
        {
            points[i + 16][1] = 0;
            points[i + 16][0] = (sint32)(sint16)projected[i][0];
            points[i + 16][2] = (sint32)(sint16)projected[i][1];
        }
    }
    if ((flags & 0xF000) != 0)
    {
        sky_load_rotation(&identity);
        sky_load_translation(&identity);
        for (i = 0; i < 4; ++i)
        {
            sub_8005f7fc(0, 0, points[i + 12], xy);
            projected[i][0] = xy[0];
            projected[i][1] = xy[1];
        }
        for (i = 0; i < 4; ++i)
        {
            points[i + 20][1] = 0;
            points[i + 20][0] = (sint32)(sint16)projected[i][0];
            points[i + 20][2] = (sint32)(sint16)projected[i][1];
        }
    }
    result = flags & 0xFF00;
    if (below != 0 && result == 0)
        return result;
    corners[0][0] = 0x8000;
    corners[0][2] = 32767;
    corners[1][0] = 32767;
    corners[1][2] = 32767;
    corners[2][0] = 0x8000;
    corners[2][2] = 32767;
    corners[3][0] = 32767;
    corners[3][2] = 32767;
    corners[0][1] = 0 - origin[1];
    corners[1][1] = 0 - origin[1];
    corners[2][1] = 0 - origin[1] - 3000;
    corners[3][1] = 0 - origin[1] - 3000;
    sky_load_rotation(&transpose);
    for (i = 0; i < 4; ++i)
        sky_project(corners[i], projected[i]);
    sky_load_rotation(&matrix);
    a = r_u16(geometry + 4);
    b = r_u16(geometry + 6);
    if (sub_8005f2b4(0, 0, (sint32)(a << 16) >> 17, (sint32)(b << 16) >> 17, projected[0], projected[1]) != 0)
    {
        projected[0][2] = 32767;
        projected[1][2] = 32767;
        sub_8005f734(0, 0, 0, projection, projected[0], points[4]);
        sub_8005f734(0, 0, 0, projection, projected[1], points[5]);
        flags |= 0x30;
        selected |= 0x18;
        indices[3] = 4;
        indices[4] = 5;
    }
    a = r_u16(geometry + 4);
    b = r_u16(geometry + 6);
    if (sub_8005f2b4(0, 0, (sint32)(a << 16) >> 17, (sint32)(b << 16) >> 17, projected[2], projected[3]) != 0)
    {
        projected[2][2] = 32767;
        projected[3][2] = 32767;
        sub_8005f734(0, 0, 0, projection, projected[2], points[6]);
        sub_8005f734(0, 0, 0, projection, projected[3], points[7]);
        flags |= 0xC0;
        if ((selected & 8) == 0 || (sint32)points[6][0] < (sint32)points[4][0])
        {
            indices[3] = 6;
            selected |= 8;
        }
        if ((selected & 16) == 0 || (sint32)points[5][0] < (sint32)points[7][0])
        {
            indices[4] = 7;
            selected |= 16;
        }
    }
    full = 0;
    if (below != 0)
    {
        if ((flags & 0xF00) == 0xF00 || ((flags & 0xF000) == 0xF000 && origin[1] != 0))
            full = 1;
    }
    else if ((flags & 0xF00) == 0xF00 || ((flags & 0xF000) == 0xF000 && origin[1] != (uint32)-3000))
        full = 1;
    if (full != 0)
    {
        minimum = 0;
        maximum = 0;
        plane_kind = 0;
        if ((flags & 0xF000) == 0xF000)
        {
            minimum = sub_8005f174(0, 4, (const uint8 *)points[12]);
            maximum = sub_8005f124(0, 4, (const uint8 *)points[12]);
            plane_kind = 2;
        }
        if ((flags & 0xF00) == 0xF00)
        {
            minimum = sub_8005f174(0, 4, (const uint8 *)points[8]);
            maximum = sub_8005f124(0, 4, (const uint8 *)points[8]);
            plane_kind |= 1;
        }
        opposite = sky_order_boundary(points, minimum, maximum, ordered);
        if (sub_8005f1c4(0, 4, &points[0][0]) != 0)
        {
            if (below != 0)
                for (i = 0; i < 4; ++i)
                    indices[i] = (flags & (256 << ordered[i])) != 0 ? ordered[i] + 8 : ordered[i];
            else
            {
                value = (native_state != 0 ? (sint16)xport_load_le16(native_state + 16) : r_s16(state + 16));
                for (i = 0; i < 4; ++i)
                    indices[i] = ordered[i] + ((sint32)value > 0 ? 12 : 8);
            }
            selected = 15;
        }
        else if (below != 0 || origin[1] == (uint32)-3000)
        {
            selected = 15;
            indices[0] = ordered[0] + 12;
            if (opposite != 0)
            {
                indices[1] = ordered[1] + 8;
                indices[2] = ordered[2] + 8;
                indices[3] = ordered[3] + 8;
            }
            else
            {
                indices[1] = ordered[0] + 8;
                indices[2] = ordered[1] + 8;
                indices[3] = ordered[2] + 8;
                indices[4] = ordered[3] + 8;
                indices[5] = ordered[3] + 12;
                selected = 63;
            }
        }
        else
        {
            indices[0] = 0xFFFFFFFF;
            value = (plane_kind & 2) != 0 ? 12 : 8;
            for (i = 0; i < 4; ++i)
                indices[i + 1] = ordered[i] + value;
            selected = 31;
        }
    }
    else if (below != 0)
    {
        selected &= 0xFFFFFFFE;
        count = 0;
        candidate = 0;
        for (i = 0; i < 4; ++i)
            if ((flags & (4096 << i)) != 0)
            {
                candidate = i + 12;
                chosen[count] = candidate;
                for (j = 0; j < 3; ++j)
                    points[24 + count][j] = points[candidate][j];
                ++count;
            }
        if (count == 1)
        {
            if ((selected & 8) != 0)
                selected &= 0xFFFFFFFB;
            if ((selected & 16) != 0)
                selected &= 0xFFFFFFDF;
            selected |= 1;
            indices[0] = chosen[0];
        }
        else if (count == 2)
        {
            maximum = sub_8005f124(0, 2, (const uint8 *)points[24]);
            minimum = sub_8005f174(0, 2, (const uint8 *)points[24]);
            indices[1] = chosen[minimum];
            selected |= 0x42;
            indices[6] = chosen[maximum];
        }
        else if (count == 3)
        {
            maximum = sub_8005f124(0, 3, (const uint8 *)points[24]);
            minimum = sub_8005f174(0, 3, (const uint8 *)points[24]);
            selected |= 0x42;
            indices[1] = chosen[minimum];
            indices[6] = chosen[maximum];
            for (i = 0; i < 3; ++i)
                if ((~(1 << maximum) & 7 & ~(1 << minimum) & (1 << i)) != 0)
                {
                    candidate = i;
                    break;
                }
            indices[0] = chosen[candidate];
            value = chosen[minimum];
            selected |= 1;
            if ((flags & (256 << ((value - 12) & 31))) != 0)
            {
                indices[2] = value - 4;
                selected |= 4;
            }
            value = chosen[maximum];
            if ((flags & (256 << ((value - 12) & 31))) != 0)
            {
                indices[5] = value - 4;
                selected |= 0x20;
            }
        }
        /* Four lower intersections imply full above because below guarantees a nonzero height */
    }
    else
    {
        count = 0;
        for (i = 0; i < 4; ++i)
            if ((flags & (256 << i)) != 0)
            {
                for (j = 0; j < 3; ++j)
                    points[24 + count][j] = points[8 + i][j];
                chosen[count++] = i + 8;
            }
        if ((sint32)count >= 3)
        {
            minimum = sub_8005f174(0, count, (const uint8 *)points[24]);
            maximum = sub_8005f124(0, count, (const uint8 *)points[24]);
            indices[2] = chosen[minimum];
            indices[5] = chosen[maximum];
        }
    }
    minimum_x = 0x7FFFFFFF;
    minimum_z = 0x7FFFFFFF;
    local_angles[0] = 0;
    local_angles[1] = (native_angles != 0 ? native_angles[1] : r_u16(angles + 2));
    local_angles[2] = 0;
    RotMatrixYXZ((SVECTOR *)local_angles, &matrix);
    sky_load_rotation(&matrix);
    for (i = 0; i < 7; ++i)
        if ((selected & (1 << i)) != 0)
        {
            index = indices[i];
            count = (native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24));
            if ((sint32)index < 0)
            {
                final_z[count] = 0;
                final_x[count] = 0;
            }
            else
            {
                for (j = 0; j < 3; ++j)
                    points[24][j] = points[index][j];
                rotate_native_vector_a6b18(points[24]);
                final_x[count] = points[24][0];
                final_z[count] = points[24][2];
            }
            if ((sint32)final_x[count] < (sint32)minimum_x)
                minimum_x = final_x[count];
            if ((sint32)final_z[count] < (sint32)minimum_z)
                minimum_z = final_z[count];
            {
                if (native_state != 0)
                    xport_store_le32(native_state + 24, count + 1);
                else
                    w_u32(state + 24, count + 1);
            }
        }
    result = (native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24));
    index = 0;
    if ((sint32)result > 0)
        do
        {
            final_x[index] -= 32768 + minimum_x;
            final_z[index] -= 32768 + minimum_z;
            if ((sint32)final_x[index] < -32768)
                final_x[index] = (uint32)-32768;
            else if ((sint32)final_x[index] > 32767)
                final_x[index] = 32767;
            if ((sint32)final_z[index] < -32768)
                final_z[index] = (uint32)-32768;
            else if ((sint32)final_z[index] > 32767)
                final_z[index] = 32767;
            {
                if (native_state != 0)
                    xport_store_le16(native_state + 28 + index * 4, final_x[index]);
                else
                    w_u16(state + 28 + index * 4, final_x[index]);
            }
            ++index;
            {
                if (native_state != 0)
                    xport_store_le16(native_state + 26 + index * 4, final_z[index - 1]);
                else
                    w_u16(state + 26 + index * 4, final_z[index - 1]);
            }
            count = (native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24));
            result = (sint32)index < (sint32)count;
        } while (result != 0);
    w_u32(0x800BD068, minimum_x);
    w_u32(0x800BD06C, minimum_z);
    return result;
}

uint32 sub_80060cb8(uint32 state, uint8 *native_state)
{
    /* Keeps neighboring local arrays contiguous and defines unwritten rows */
    uint32 grid[96] = {0}, end_x[7], end_y[7], start_x[7], start_y[7], edge_start[7];
    uint16 bounds[4];
    sint32 vertices, min_x, min_y, max_x, max_y, nx, ny, x0, y0, x1, y1, low_x, low_y, high_x, high_y, sx, sy, ex, ey, row, column, edge, previous_column, bit, i;
    uint32 value, raw, first, second, packed, bits, mask, prior, result;
    FUNCTION_MARKER(0x80060CB8, "ACE3.BIN");
    vertices = (native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24));
    if (vertices == 0)
    {
        for (i = 72; i <= 92; i += 4)
        {
            if (native_state != 0)
                xport_store_le32(native_state + i, 0);
            else
                w_u32(state + i, 0);
        }
        {
            if (native_state != 0)
                xport_store_le32(native_state + 96, 0);
            else
                w_u32(state + 96, 0);
        }
        return 0;
    }
    for (i = 0; i < 32; ++i)
    {
        grid[i] = 0xFFFFFFFF;
        grid[32 + i] = 0;
    }
    min_x = 32767;
    min_y = 32767;
    max_x = -32768;
    max_y = -32768;
    for (i = 0; i < vertices; ++i)
    {
        value = (native_state != 0 ? (sint16)xport_load_le16(native_state + 28 + i * 4) : r_s16(state + 28 + i * 4));
        raw = (native_state != 0 ? xport_load_le16(native_state + 28 + i * 4) : r_u16(state + 28 + i * 4));
        if (max_x < (sint32)value)
            max_x = (sint16)raw;
        value = (native_state != 0 ? (sint16)xport_load_le16(native_state + 30 + i * 4) : r_s16(state + 30 + i * 4));
        raw = (native_state != 0 ? xport_load_le16(native_state + 30 + i * 4) : r_u16(state + 30 + i * 4));
        if (max_y < (sint32)value)
            max_y = (sint16)raw;
        value = (native_state != 0 ? (sint16)xport_load_le16(native_state + 28 + i * 4) : r_s16(state + 28 + i * 4));
        raw = (native_state != 0 ? xport_load_le16(native_state + 28 + i * 4) : r_u16(state + 28 + i * 4));
        if ((sint32)value < min_x)
            min_x = (sint16)raw;
        value = (native_state != 0 ? (sint16)xport_load_le16(native_state + 30 + i * 4) : r_s16(state + 30 + i * 4));
        raw = (native_state != 0 ? xport_load_le16(native_state + 30 + i * 4) : r_u16(state + 30 + i * 4));
        if ((sint32)value < min_y)
            min_y = (sint16)raw;
    }
    bounds[0] = min_x & 0xF800;
    bounds[2] = (max_x + 2047) & 0xF800;
    nx = ((sint32)(sint16)bounds[2] - (sint32)(sint16)bounds[0]) >> 11;
    bounds[1] = min_y & 0xF800;
    bounds[3] = (max_y + 2047) & 0xF800;
    ny = ((sint32)(sint16)bounds[3] - (sint32)(sint16)bounds[1]) >> 11;
    if (nx < 0 || ny < 0 || nx >= 32 || ny >= 32)
    {
#ifndef NDEBUG
        result = printf("Error test_makegrid: nx(%d),ny(%d)\n", (sint32)(nx), (sint32)(ny));
#else
        result = 0;
#endif
        for (i = 72; i <= 92; i += 4)
        {
            if (native_state != 0)
                xport_store_le32(native_state + i, 0);
            else
                w_u32(state + i, 0);
        }
        return result;
    }
    for (edge = 0; edge < vertices; ++edge)
    {
        i = edge % vertices;
        value = (edge + 1) % vertices;
        x0 = (native_state != 0 ? (sint16)xport_load_le16(native_state + 28 + i * 4) : r_s16(state + 28 + i * 4));
        x1 = (native_state != 0 ? (sint16)xport_load_le16(native_state + 28 + value * 4) : r_s16(state + 28 + value * 4));
        y0 = (native_state != 0 ? (sint16)xport_load_le16(native_state + 30 + i * 4) : r_s16(state + 30 + i * 4));
        y1 = (native_state != 0 ? (sint16)xport_load_le16(native_state + 30 + value * 4) : r_s16(state + 30 + value * 4));
        low_x = x1 < x0 ? x1 : x0;
        high_x = x1 < x0 ? x0 : x1;
        low_y = y1 < y0 ? y1 : y0;
        high_y = y1 < y0 ? y0 : y1;
        value = (uint32)low_x & 0xFFFFF800;
        raw = (uint32)low_y & 0xFFFFF800;
        edge_start[edge] = (value & 0xFFFF) | (raw << 16);
        start_x[edge] = (uint32)((sint32)(value - (uint32)(sint32)(sint16)bounds[0]) >> 11);
        start_y[edge] = (uint32)((sint32)(raw - (uint32)(sint32)(sint16)bounds[1]) >> 11);
        end_x[edge] = start_x[edge] + (uint32)((sint32)(((uint32)(high_x + 2047) & 0xFFFFF800) - value) >> 11);
        end_y[edge] = start_y[edge] + (uint32)((sint32)(((uint32)(high_y + 2047) & 0xFFFFF800) - raw) >> 11);
    }
    for (edge = 0; edge < vertices; ++edge)
    {
        first = (native_state != 0 ? xport_load_le32(native_state + 28 + (edge % vertices) * 4) : r_u32(state + 28 + (edge % vertices) * 4));
        xport_gte_write_data(12, first);
        second = (native_state != 0 ? xport_load_le32(native_state + 28 + ((edge + 1) % vertices) * 4) : r_u32(state + 28 + ((edge + 1) % vertices) * 4));
        xport_gte_write_data(13, second);
        sx = start_x[edge];
        ex = end_x[edge];
        sy = start_y[edge];
        ey = end_y[edge];
        packed = edge_start[edge] & 0xFFFF0000;
        for (row = sy; row <= ey; ++row)
        {
            bits = 0;
            packed = (packed & 0xFFFF0000) | (edge_start[edge] & 0xFFFF);
            xport_gte_write_data(14, packed);
            for (column = sx; column <= ex; ++column)
            {
                xport_gte_execute(0x1400006);
                packed = (packed & 0xFFFF0000) | ((packed + 2048) & 0xFFFF);
                value = xport_gte_read_data(24);
                if ((sint32)value > 0)
                    bits |= (uint32)1 << (column & 31);
                xport_gte_write_data(14, packed);
            }
            grid[64 + row] = ~bits;
            packed += 0x08000000;
        }
        for (row = sy + 1; row < ey; ++row)
        {
            previous_column = sx;
            for (column = sx + 1; column < ex; ++column)
            {
                value = grid[64 + row];
                if ((((uint32)((sint32)value >> (previous_column & 31)) ^ (uint32)((sint32)value >> (column & 31))) & 1) != 0)
                {
                    mask = (uint32)1 << (previous_column & 31);
                    grid[32 + row - 1] |= mask;
                    grid[32 + row] |= mask;
                    break;
                }
                previous_column = column;
            }
        }
        for (column = sx + 1; column < ex; ++column)
        {
            prior = grid[64 + sy];
            for (row = sy + 1; row < ey; ++row)
            {
                value = grid[64 + row];
                raw = prior;
                prior = value;
                if (((raw ^ value) & ((uint32)1 << (column & 31))) != 0)
                {
                    grid[32 + row - 1] |= (uint32)3 << ((column - 1) & 31);
                    break;
                }
            }
        }
        if (ex - sx < 2)
            grid[32 + ey - 1] |= (uint32)1 << (sx & 31);
        else
            for (column = sx + 1; column < ex; ++column)
            {
                if (((grid[64 + ey - 1] ^ grid[64 + ey]) & ((uint32)1 << (column & 31))) != 0)
                    grid[32 + ey - 1] |= (uint32)3 << ((column - 1) & 31);
            }
        if (ey - sy < 2)
            grid[32 + sy] |= (uint32)1 << ((ex - 1) & 31);
        else
            for (row = sy + 1; row < ey; ++row)
            {
                value = grid[64 + row];
                if ((((uint32)((sint32)value >> ((ex - 1) & 31)) ^ (uint32)((sint32)value >> (ex & 31))) & 1) != 0)
                {
                    mask = (uint32)1 << ((ex - 1) & 31);
                    grid[32 + row - 1] |= mask;
                    grid[32 + row] |= mask;
                }
            }
    }
    for (row = 0; row < ny; ++row)
    {
        mask = 1;
        for (column = 0; column < nx; ++column)
        {
            if ((mask & grid[32 + row]) != 0)
            {
                grid[64 + row] = (uint32)((sint32)mask >> 1);
                break;
            }
            mask |= mask << 1;
        }
    }
    for (row = 0; row < ny; ++row)
    {
        mask = 0x80000000;
        for (bit = 31; bit >= 0; --bit)
        {
            if ((mask & grid[32 + row]) != 0)
            {
                grid[32 + row] = ~((mask << 1) | grid[64 + row]);
                break;
            }
            mask |= (uint32)((sint32)mask >> 1);
        }
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 72, (uint32)((sint32)((uint32)bounds[0] << 16) >> 27));
        else
            w_u32(state + 72, (uint32)((sint32)((uint32)bounds[0] << 16) >> 27));
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 76, (uint32)((sint32)((uint32)bounds[1] << 16) >> 27));
        else
            w_u32(state + 76, (uint32)((sint32)((uint32)bounds[1] << 16) >> 27));
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 80, (uint32)((sint32)((uint32)bounds[2] << 16) >> 27));
        else
            w_u32(state + 80, (uint32)((sint32)((uint32)bounds[2] << 16) >> 27));
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 88, nx);
        else
            w_u32(state + 88, nx);
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state + 92, ny);
        else
            w_u32(state + 92, ny);
    }
    result = (uint32)((sint32)((uint32)bounds[3] << 16) >> 27);
    {
        if (native_state != 0)
            xport_store_le32(native_state + 84, result);
        else
            w_u32(state + 84, result);
    }
    for (row = 0; row < ny; ++row)
    {
        value = grid[32 + row];
        {
            if (native_state != 0)
                xport_store_le32(native_state + 96 + row * 4, value);
            else
                w_u32(state + 96 + row * 4, value);
        }
        result = row + 1 < ny;
    }
    return result;
}

uint32 sub_800371c4(void)
{
    uint32 object, scene, game, control, descriptor, flags, stage, ticks, value, other, packet, ordering, reason, remaining, high, quotient;
    sint32 mission;
    /* Defines the padding copied into the transition object */
    uint8 pose[24] = {0};
    FUNCTION_MARKER(0x800371C4, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C));
    sub_800540e0();
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 4);
    if ((flags & 2) != 0)
    {
        game = r_u32(0x800BCE60);
        value = r_u32(game);
        control = r_u32(0x800BCEB4);
        w_u32(game, value | 0x10000000);
        if (r_u8(control + 63) == 1 && r_u32(scene + 8) == 0)
        {
            ticks = r_u32(scene + 12);
            if ((sint32)ticks < 16)
            {
                ordering = r_u32(scene + 16);
                packet = r_u32(scene + 20);
                value = sub_80084560(ordering + 4, packet, 4096 - (ticks << 8));
                scene = r_u32(0x800BCE54);
                w_u32(scene + 20, value);
            }
        }
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) == 0)
        {
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            if (mission == 42)
                sub_800762b0();
            else if (r_u32(scene + 8) == 0 && (mission == 6 || mission == 14 || mission == 44))
                sub_800762f0(0);
            else
                sub_800762d0();
            sub_8007953c();
        }
        if ((sub_8006cb30() & 0x7000) != 0)
        {
            control = r_u32(0x800BCEB4);
            w_u8(control + 41, 9);
            control = r_u32(0x800BCEB4);
            w_u8(control + 42, 5);
            sub_80032f18(8, 1);
            game = r_u32(0x800BCE60);
            value = r_u32(game);
            w_u32(game, value | 0x10000000);
            sub_80056518();
            resource_cancel_streams();
            return 0;
        }
        scene = r_u32(0x800BCE54);
        if (r_s32(scene + 12) < 61 || sub_8005650c() != 0)
            return 0;
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 42)
            sub_80076290();
        else
            sub_80076274();
        game = r_u32(0x800BCE60);
        sub_800794d8(r_u8(game + 927));
        scene = r_u32(0x800BCE54);
        sub_80032f18(4, r_u32(scene + 8));
        game = r_u32(0x800BCE60);
        value = r_u32(game);
        scene = r_u32(0x800BCE54);
        w_u32(game, value & 0xEFFFFFFF);
        descriptor = r_u32(0x800BCEB0);
        stage = r_u32(scene + 8);
        sub_8004ceb0(r_u8(descriptor + stage + 72));
        return 0;
    }
    if ((flags & 4) != 0)
    {
        if ((sub_8006cb30() & 0x7000) != 0)
        {
            control = r_u32(0x800BCEB4);
            w_u8(control + 42, 5);
            control = r_u32(0x800BCEB4);
            w_u8(control + 41, 9);
            sub_80032f18(8, 1);
            game = r_u32(0x800BCE60);
            value = r_u32(game);
            w_u32(game, value | 0x10000000);
            goto abort_streams;
        }
        scene = r_u32(0x800BCE54);
        descriptor = r_u32(0x800BCEB0);
        control = r_u32(0x800BCEB4);
        stage = r_u32(scene + 8);
        value = r_u32(control + 16);
        other = r_u32(descriptor + stage * 4 + 36);
        if (value == other)
        {
            w_u8(control + 42, 5);
            control = r_u32(0x800BCEB4);
            w_u8(control + 41, 9);
            sub_80032f18(8, 4);
            game = r_u32(0x800BCE60);
            value = r_u32(game);
            w_u32(game, value | 0x10000000);
            goto abort_streams;
        }
        value = r_u32(object + 8);
        other = r_u32(descriptor + 16);
        if ((sint32)value < (sint32)other)
            goto outside_area;
        other = r_u32(descriptor + 20);
        if ((sint32)other < (sint32)value)
            goto outside_area;
        value = r_u32(object + 16);
        other = r_u32(descriptor + 24);
        if ((sint32)value < (sint32)other)
            goto outside_area;
        other = r_u32(descriptor + 28);
        if ((sint32)other < (sint32)value)
            goto outside_area;
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 58 && r_s32(object + 12) > (sint32)0xFFD00000)
            goto outside_area;
        if (mission == 42 && r_u32(object + 12) + 0x0EA60000 > 0x124F8000)
            goto outside_area;
        control = r_u32(0x800BCEB4);
        if (r_u8(control + 41) != 9)
            goto countdown;
        sub_80032f18(8, r_u8(control + 39));
        game = r_u32(0x800BCE60);
        value = r_u32(game);
        mission = r_s16(game + 908);
        w_u32(game, value | 0x10000000);
        if (mission == 6)
            sub_80056518();
        goto cancel_streams;
    outside_area:
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 32 || mission == 47 || mission == 53)
            goto countdown;
        control = r_u32(0x800BCEB4);
        w_u8(control + 42, 5);
        control = r_u32(0x800BCEB4);
        w_u8(control + 41, 9);
        sub_80032f18(8, 8);
        game = r_u32(0x800BCE60);
        value = r_u32(game);
        w_u32(game, value | 0x10000000);
    abort_streams:
        sub_80056518();
    cancel_streams:
        resource_cancel_streams();
    countdown:
        scene = r_u32(0x800BCE54);
        stage = r_u32(scene + 8);
        descriptor = r_u32(0x800BCEB0);
        control = r_u32(0x800BCEB4);
        value = r_u32(descriptor + stage * 4 + 36);
        other = r_u32(control + 16);
        remaining = value - other;
        if (remaining == 1800)
        {
            sub_8004ceb0(10);
            control = r_u32(0x800BCEB4);
            w_u8(control + 28, 1);
            control = r_u32(0x800BCEB4);
            w_u32(control + 20, remaining);
        }
        if ((sint32)remaining >= 1800)
        {
            control = r_u32(0x800BCEB4);
            if (r_u8(control + 28) == 1)
            {
                value = r_u32(control + 20);
                if ((sint32)value < 1800)
                    remaining = value;
            }
            if ((sint32)remaining >= 1800)
                return 0;
        }
        high = (uint32)(((sint64)(sint32)remaining * (sint32)0x88888889) >> 32);
        quotient = (uint32)((sint32)(high + remaining) >> 4) - (uint32)((sint32)remaining >> 31);
        value = ((quotient << 4) - quotient) << 1;
        if (remaining == value)
            sub_8002db4c((sint32)remaining < 301 ? 57 : 56);
        return 0;
    }
    if ((flags & 8) != 0)
    {
        game = r_u32(0x800BCE60);
        value = r_u32(game);
        w_u32(game, value | 0x10000000);
        stage = r_u32(scene + 8);
        if ((stage & 0x62) == 0 && stage != 0)
            goto staged_transition;
        if ((sub_8006cb30() & 0x7000) != 0)
        {
            control = r_u32(0x800BCEB4);
            w_u8(control + 41, 9);
            sub_80032f18(8, 1);
            game = r_u32(0x800BCE60);
            value = r_u32(game);
            control = r_u32(0x800BCEB4);
            w_u32(game, value | 0x10000000);
            w_u8(control + 48, 0);
            sub_80056518();
            control = r_u32(0x800BCEB4);
            w_u8(control + 42, 5);
            return 0;
        }
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 8) == 0)
            goto unstaged_transition;
    staged_transition:
        scene = r_u32(0x800BCE54);
        stage = r_u32(scene + 8);
        reason = 0;
        if ((stage & 2) == 0)
        {
            reason = 1;
            if ((stage & 0x20) == 0)
            {
                reason = 2;
                if ((stage & 0x40) != 0)
                    reason = 50;
            }
        }
        scene = r_u32(0x800BCE54);
        if (r_u32(scene + 12) == 0)
        {
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            if (mission == 58 && (r_u32(scene + 8) & 8) != 0)
                sub_800762f0(0);
            else
            {
                scene = r_u32(0x800BCE54);
                if ((r_u32(scene + 8) & 1) == 0)
                {
                    game = r_u32(0x800BCE60);
                    mission = r_s16(game + 908);
                    if (mission == 42)
                        sub_800762b0();
                    else
                        sub_800762d0();
                }
            }
            w_u8(0x800BCEDC, 0);
        }
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 6)
        {
            w_u32(0x800BCED4, 60);
            w_u32(0x800BCED8, 0);
            value = 90;
        }
        else
        {
            w_u32(0x800BCED8, 60);
            w_u32(0x800BCED4, 180);
            value = 210;
        }
        w_u32(0x800BCED0, value);
        scene = r_u32(0x800BCE54);
        ticks = r_u32(scene + 12);
        value = r_u32(0x800BCED8);
        if (ticks == value)
            sub_8004ceb0(reason);
        scene = r_u32(0x800BCE54);
        value = r_u32(0x800BCED4);
        ticks = r_u32(scene + 12);
        if ((sint32)ticks >= (sint32)value && r_u8(0x800BCEDC) == 0 && sub_8005650c() == 0)
        {
            control = r_u32(0x800BCEB4);
            w_u8(control + 49, 2);
            control = r_u32(0x800BCEB4);
            w_u8(control + 48, 1);
            control = r_u32(0x800BCEB4);
            w_u8(0x800BCEDC, 1);
            w_u16(control + 50, 30);
            w_u16(control + 26, 0);
        }
        scene = r_u32(0x800BCE54);
        value = r_u32(0x800BCED0);
        ticks = r_u32(scene + 12);
        if ((sint32)ticks < (sint32)value || r_u8(0x800BCEDC) != 1 || sub_8005650c() != 0)
            return 0;
        control = r_u32(0x800BCEB4);
        if (r_u8(control + 48) != 0)
            return 0;
        if (r_u8(control + 41) != 0)
            sub_8003048c();
        else
            sub_800304cc();
        sub_80056518();
        sub_80042524();
        sub_80037e50();
        return 0;
    unstaged_transition:
        if (r_u32(scene + 12) == 0)
        {
            control = r_u32(0x800BCEB4);
            value = r_u8(control + 41);
            other = 0xFFFFFFFF;
            reason = 90;
            if (value != 0)
            {
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                sub_800762d0();
                if (mission == 6)
                {
                    control = r_u32(0x800BCEB4);
                    if ((r_u8(control + 42) & 2) != 0)
                    {
                        other = 0;
                        reason = 30;
                    }
                    else
                        other = 60;
                }
                else if (mission == 23)
                {
                    other = 120;
                    reason = 150;
                }
                else
                    other = 60;
            }
            w_u32(0x800BCED4, other);
            w_u32(0x800BCED0, reason);
        }
        scene = r_u32(0x800BCE54);
        ticks = r_u32(scene + 12);
        value = r_u32(0x800BCED4);
        if (ticks == value)
        {
            control = r_u32(0x800BCEB4);
            w_u8(control + 49, 2);
            control = r_u32(0x800BCEB4);
            w_u8(control + 48, 1);
            control = r_u32(0x800BCEB4);
            w_u16(control + 50, 30);
            w_u16(control + 26, 0);
        }
        scene = r_u32(0x800BCE54);
        value = r_u32(0x800BCED0);
        ticks = r_u32(scene + 12);
        if ((sint32)ticks < (sint32)value || sub_8005650c() != 0)
            return 0;
        control = r_u32(0x800BCEB4);
        if (r_u8(control + 48) != 0)
            return 0;
        if (r_u8(control + 41) != 0)
        {
            sub_8003048c();
            sub_800849ec();
            sub_8004596c();
        }
        else
            sub_800304cc();
        sub_80042524();
        sub_80056518();
        sub_80037e50();
        return 0;
    }
    if ((flags & 0x10) != 0)
    {
        if (r_u32(scene + 12) == 0)
        {
            game = r_u32(0x800BCE60);
            if (r_s16(game + 908) == 46 && r_u32(scene + 8) == 2)
                sub_80048ad4(0);
        }
        return 0;
    }
    if ((flags & 0x21FFE0) != 0 || (flags & 0x100000) == 0)
        return 0;
    if (r_u32(scene + 12) == 0)
    {
        game = r_u32(0x800BCE60);
        w_u8(0x800BCEDD, 0);
        mission = r_s16(game + 908);
        if (mission == 6)
        {
            xport_store_le32(pose, 0xC7C00000);
            xport_store_le32(pose + 8, 0xD7C00000);
            xport_store_le32(pose + 4, 0xFF448000);
            xport_store_le16(pose + 16, 0);
            xport_store_le16(pose + 18, 0xE000);
            xport_store_le16(pose + 20, 0);
            sub_80072780(0, pose);
        }
        else if (mission == 9)
        {
            xport_store_le32(pose, 0);
            xport_store_le32(pose + 4, 0xFE2B4000);
            xport_store_le32(pose + 8, 0);
            xport_store_le16(pose + 16, 0);
            xport_store_le16(pose + 18, 0);
            xport_store_le16(pose + 20, 0);
            sub_80072780(0, pose);
        }
        sub_800762f0(0);
        game = r_u32(0x800BCE60);
        sub_800794d8(r_u8(game + 927));
        sub_80030464();
    }
    scene = r_u32(0x800BCE54);
    if (r_u32(scene + 12) == 30)
    {
        control = r_u32(0x800BCEB4);
        sub_80056234(r_u8(control + 62), 0);
    }
    if (r_u8(0x800BCEDD) == 0)
    {
        scene = r_u32(0x800BCE54);
        if (r_s32(scene + 12) >= 151 && sub_8005650c() == 0)
            w_u8(0x800BCEDD, 1);
    }
    if (r_u8(0x800BCEDD) == 1)
    {
        control = r_u32(0x800BCEB4);
        w_u8(control + 49, 2);
        control = r_u32(0x800BCEB4);
        w_u8(control + 48, 1);
        w_u8(0x800BCEDD, 2);
        control = r_u32(0x800BCEB4);
        w_u16(control + 50, 30);
        w_u16(control + 26, 0);
    }
    if (r_u8(0x800BCEDD) == 2)
    {
        control = r_u32(0x800BCEB4);
        if (r_u8(control + 48) == 0)
        {
            sub_80037e50();
            sub_8003048c();
        }
    }
    return 0;
}

uint32 sub_80054ebc(uint32 script)
{
    uint32 cursor, opcode, control, game, scene, index, mode, value, other, object, table, slot, flag_slot, target, mask, count, i;
    sint32 parameter;
    /* Defines the pose bytes omitted by the original script command */
    uint8 pose[24] = {0};
    FUNCTION_MARKER(0x80054EBC, "ACE3.BIN");
    cursor = script;
    opcode = r_u8(cursor);
    while (opcode != 0)
    {
        cursor += 1;
        switch (opcode)
        {
            case 1:
                control = r_u32(0x800BCEB4);
                value = r_u8(cursor);
                w_u8(control + 41, value);
                scene = r_u32(0x800BCE54);
                mode = r_u32(scene + 4);
                cursor += 1;
                if (mode == 4)
                {
                    control = r_u32(0x800BCEB4);
                    sub_80032f18(8, r_u8(control + 39));
                    game = r_u32(0x800BCE60);
                    value = r_u32(game);
                    control = r_u32(0x800BCEB4);
                    w_u32(game, value | 0x10000000);
                    if (r_u8(control + 41) != 0)
                        resource_cancel_streams();
                }
                else if (mode == 0)
                {
                    sub_80037e50();
                    scene = r_u32(0x800BCE54);
                    value = r_u32(scene);
                    w_u32(scene, value & 0xDFFFFFFF);
                }
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                control = r_u32(0x800BCEB4);
                value = r_u8(cursor);
                cursor += 1;
                w_u8(control + (opcode == 2 ? 36 : opcode == 3 ? 35 : opcode == 4 ? 37 : 38), value);
                break;
            case 6:
                value = r_u8(cursor);
                if (value <= 7)
                {
                    control = r_u32(0x800BCEB4);
                    if (value == 3)
                        value = 8;
                    else if (value >= 5)
                        value = (uint32)1 << (value - 1);
                    w_u8(control + 39, value);
                }
                cursor += 1;
                break;
            case 7:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 42);
                w_u8(control + 42, value | 4);
                control = r_u32(0x800BCEB4);
                w_u8(control + 41, 9);
                break;
            case 8:
                control = r_u32(0x800BCEB4);
                w_u8(control + 41, 9);
                game = r_u32(0x800BCE60);
                value = r_u8(cursor);
                cursor += 1;
                w_u8(game + 957, value);
                break;
            case 9:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 42);
                w_u8(control + 42, value | 1);
                break;
            case 10:
                game = r_u32(0x800BCE60);
                value = r_u8(cursor);
                cursor += 1;
                w_u8(game + 2390, value);
                break;
            case 11:
                control = r_u32(0x800BCEB4);
                value = r_u32(control + 16);
                cursor = sub_80055c4c(cursor, value);
                break;
            case 12:
                cursor = sub_80055cbc(cursor);
                break;
            case 13:
                cursor = sub_80055d2c(cursor);
                break;
            case 14:
                cursor = sub_80055d94(cursor);
                break;
            case 15:
                cursor = sub_80055dfc(cursor);
                break;
            case 16:
                cursor = sub_80055e64(cursor);
                break;
            case 17:
                cursor = sub_80055ed8(cursor);
                break;
            case 18:
                cursor = sub_80055f3c(cursor);
                break;
            case 19:
                cursor = sub_80055fa4(cursor);
                break;
            case 20:
                cursor = sub_80056004(cursor);
                break;
            case 21:
                index = r_u8(cursor);
                cursor += 1;
                if (index < 32)
                {
                    sub_8004e0fc(index);
                    sub_8004e124(index);
                }
                else
                {
                    index -= 32;
                    sub_8004e900(index);
                    sub_8004e94c(index);
                }
                break;
            case 22:
                index = r_u8(cursor);
                cursor += 1;
                table = r_u32(0x800BCE5C);
                slot = table + index * 4;
                object = r_u32(slot);
                value = r_u32(object);
                w_u32(object, value & 0xFFFFBFFF);
                if (index < 32)
                {
                    object = r_u32(slot);
                    value = r_u32(object);
                    w_u32(object, value & 0xFFFEFFFF);
                }
                break;
            case 23:
            case 25:
                cursor += 1;
                break;
            case 24:
                index = r_u8(cursor);
                cursor += 1;
                table = r_u32(0x800BCE5C);
                object = r_u32(table + index * 4);
                value = sub_80056148(cursor);
                parameter = (sint16)value;
                if (parameter == -1)
                {
                    if (index < 32)
                    {
                        table = r_u32(0x800BCEF8);
                        target = r_u32(table + index * 64 + 16);
                    }
                    else
                    {
                        table = r_u32(0x800BCEFC);
                        target = r_u32(table + index * 40 - 1268);
                    }
                    value = r_u16(target);
                    w_u16(object + 78, value);
                }
                else if (parameter == -2)
                {
                    value = r_u16(0x800BD018);
                    w_u16(object + 78, value);
                }
                else if (parameter == -3)
                {
                    value = r_u16(object + 78);
                    w_u16(0x800BD018, value);
                }
                else
                    w_u16(object + 78, value);
                cursor += 2;
                break;
            case 26:
                index = r_u8(cursor);
                cursor += 1;
                flag_slot = 0x800C6FD0 + index * 4;
                value = r_u32(flag_slot);
                w_u32(flag_slot, value | 0x20000);
                break;
            case 27:
            case 28:
                index = r_u8(cursor);
                cursor += 1;
                mode = r_u8(cursor);
                if (mode == 1)
                {
                    flag_slot = 0x800C6FD0 + index * 4;
                    value = r_u32(flag_slot);
                    table = r_u32(0x800BCE5C);
                    slot = table + index * 4;
                    w_u32(flag_slot, value | (opcode == 27 ? 0x100 : 0x200));
                    object = r_u32(slot);
                    value = r_u32(object);
                    w_u32(object, value & 0xFFFFFE3F);
                    object = r_u32(slot);
                    value = r_u32(object);
                    w_u32(object, value | (opcode == 27 ? 0x80 : 0x40));
                    target = r_u32(0x800C67A0 + index * 4);
                    w_u8(target + 5, opcode == 27 ? 1 : 3);
                    w_u8(0x800C6A60 + index * 12 + 2, opcode == 27 ? 1 : 2);
                    value = r_u32(flag_slot);
                    w_u32(flag_slot, value & (opcode == 27 ? 0xFFFFFDFF : 0xFFFFFEFF));
                    if (opcode == 28 && index == r_u8(0x800C6D6E))
                    {
                        value = sub_800464b0(0);
                        w_u8(0x800C6D6E, value);
                    }
                }
                else if (mode == 0)
                {
                    flag_slot = 0x800C6FD0 + index * 4;
                    value = r_u32(flag_slot);
                    w_u32(flag_slot, value & (opcode == 27 ? 0xFFFFFEFF : 0xFFFFFDFF));
                    target = r_u32(0x800C67A0 + index * 4);
                    w_u8(target + 5, 0);
                    w_u8(0x800C6A60 + index * 12 + 2, 0);
                }
                cursor += 1;
                break;
            case 29:
                index = r_u8(cursor);
                cursor += 1;
                value = r_u8(cursor);
                flag_slot = 0x800C6FD0 + index * 4;
                other = r_u32(flag_slot);
                w_u32(flag_slot, (value & 2) != 0 ? other | 0x4000 : other & 0xFFFFBFFF);
                value = r_u8(cursor);
                other = r_u32(flag_slot);
                w_u32(flag_slot, (value & 1) != 0 ? other | 0x8000 : other & 0xFFFF7FFF);
                value = r_u8(0x800C6D6E);
                cursor += 1;
                if (value == index)
                {
                    w_u8(0x800C6D6E, 255);
                    sub_800464b0(0);
                    sub_800463b8(0, 255);
                }
                break;
            case 30:
                cursor += 2;
                break;
            case 31:
                index = r_u8(cursor);
                cursor += 1;
                table = r_u32(0x800BCEC0);
                w_u8(table + index * 184 + 28, 255);
                value = r_u8(cursor);
                cursor += 1;
                table = r_u32(0x800BCEF8);
                w_u32(table + index * 64 + 36, value - 1);
                break;
            case 32:
                index = r_u8(cursor);
                cursor += 1;
                value = r_u8(cursor);
                cursor += 1;
                table = r_u32(0x800BCEFC);
                target = r_u32(0x800BCF30);
                w_u32(table + index * 40 + 20, target + value * 48);
                break;
            case 33:
                index = r_u8(cursor);
                cursor += 1;
                mode = r_u8(cursor);
                if (mode == 1 || mode == 2)
                {
                    table = r_u32(0x800BCEF8);
                    w_u32(table + index * 64 + 20, mode == 1 ? 0x800B20D4 : 0x800B20FC);
                }
                else
                {
                    target = r_u32(0x800BCF00);
                    table = r_u32(0x800BCEF8);
                    value = r_u8(target + index * 44 + 3);
                    target = r_u32(0x800BCF24);
                    w_u32(table + index * 64 + 20, target + value * 40);
                }
                cursor += 1;
                break;
            case 34:
            case 35:
                index = r_u8(cursor);
                cursor += 1;
                mode = r_u8(cursor);
                flag_slot = 0x800C6FD0 + index * 4;
                value = r_u32(flag_slot);
                mask = opcode == 34 ? 0x1000 : 0x2000;
                w_u32(flag_slot, mode == 1 ? value | mask : value & ~mask);
                cursor += 1;
                break;
            case 36:
                index = r_u8(cursor);
                cursor += 1;
                mode = r_u8(cursor);
                cursor += 1;
                if (mode == 1)
                    sub_8004c2c8(index);
                else
                    sub_8004c338(index);
                break;
            case 37:
                index = r_u8(cursor);
                cursor += 1;
                value = sub_80056164(cursor);
                cursor += 4;
                xport_store_le32(pose, value << 12);
                value = sub_80056164(cursor);
                cursor += 4;
                xport_store_le32(pose + 4, value << 12);
                value = sub_80056164(cursor);
                cursor += 4;
                xport_store_le32(pose + 8, value << 12);
                value = sub_80056148(cursor);
                cursor += 2;
                xport_store_le16(pose + 16, value);
                value = sub_80056148(cursor);
                cursor += 2;
                xport_store_le16(pose + 18, value);
                value = sub_80056148(cursor);
                xport_store_le16(pose + 20, value);
                cursor += 2;
                if (index == 0)
                    sub_80072780(0, pose);
                else
                {
                    table = r_u32(0x800BCE5C);
                    object = r_u32(table + index * 4);
                    for (i = 0; i < 6; ++i)
                        w_u32(object + 8 + i * 4, xport_load_le32(pose + i * 4));
                }
                break;
            case 41:
                index = r_u8(cursor);
                cursor += 1;
                value = sub_80056148(cursor);
                cursor += 2;
                sub_80056234(index, (sint16)value);
                break;
            case 42:
                index = r_u8(cursor);
                cursor += 1;
                value = r_u8(cursor);
                cursor += 1;
                sub_8002f294(index, value);
                break;
            case 43:
                count = r_u8(cursor);
                cursor += 1;
                i = 0;
                while (i < count)
                {
                    index = r_u8(cursor);
                    cursor += 1;
                    value = r_u8(cursor);
                    cursor += 1;
                    i += 1;
                    sub_8002f294(index, value);
                }
                break;
            case 45:
            case 47:
            case 52:
            case 54:
            case 57:
                control = r_u32(0x800BCEB4);
                value = r_u8(cursor);
                cursor += 1;
                target = opcode == 45 ? 30 : opcode == 47 ? 31 : opcode == 52 ? 43 : opcode == 54 ? 44 : 28;
                w_u8(control + target, value);
                break;
            case 46:
            case 48:
            case 50:
                control = r_u32(0x800BCEB4);
                w_u8(control + (opcode == 46 ? 32 : opcode == 48 ? 33 : 34), 0);
                break;
            case 49:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 34);
                w_u8(control + 34, value + 1);
                break;
            case 51:
                control = r_u32(0x800BCEB4);
                sub_8004ceb0(r_u8(control + 43));
                break;
            case 53:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 32);
                w_u8(control + 32, value << 1);
                break;
            case 55:
                control = r_u32(0x800BCEB4);
                sub_8005697c(r_u8(control + 44));
                break;
            case 56:
                sub_8005697c(r_u8(cursor));
                cursor += 1;
                break;
            case 58:
                value = sub_80056148(cursor);
                cursor += 2;
                control = r_u32(0x800BCEB4);
                w_u32(control + 20, (uint32)(sint32)(sint16)value);
                break;
            case 59:
                sub_8005f0e4(1);
                break;
            case 60:
                control = r_u32(0x800BCEB4);
                value = r_u32(control + 12);
                cursor = sub_80055c4c(cursor, value);
                break;
            case 61:
                cursor = sub_80056080(cursor);
                break;
            case 62:
                cursor = sub_800560e4(cursor);
                break;
            case 70:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 42);
                w_u8(control + 42, value | 2);
                control = r_u32(0x800BCEB4);
                value = r_u8(cursor);
                cursor += 1;
                w_u8(control + 62, value);
                break;
            case 71:
                control = r_u32(0x800BCEB4);
                value = r_u8(control + 42);
                cursor += 1;
                w_u8(control + 42, value | 4);
                break;
            case 72:
                value = r_u8(cursor);
                cursor += 1;
                control = r_u32(0x800BCEB4);
                w_u8(control + 49, value);
                value = sub_80056148(cursor);
                control = r_u32(0x800BCEB4);
                cursor += 2;
                w_u16(control + 50, value);
                break;
            case 73:
                control = r_u32(0x800BCEB4);
                w_u8(control + 48, 1);
                control = r_u32(0x800BCEB4);
                w_u16(control + 26, 0);
                break;
            case 74:
                for (i = 52; i <= 55; ++i)
                {
                    control = r_u32(0x800BCEB4);
                    value = r_u8(cursor);
                    cursor += 1;
                    w_u8(control + i, value);
                }
                break;
            case 75:
                value = sub_80056148(cursor);
                cursor += 2;
                control = r_u32(0x800BCEB4);
                other = r_u32(control + 4);
                w_u32(control + 4, other + (uint32)(sint32)(sint16)value);
                break;
            case 76:
                value = r_u8(cursor);
                cursor += 1;
                control = r_u32(0x800BCEB4);
                w_u8(control + 52, value);
                value = sub_80056148(cursor);
                cursor += 2;
                control = r_u32(0x800BCEB4);
                w_u16(control + 56, value);
                value = sub_80056148(cursor);
                control = r_u32(0x800BCEB4);
                cursor += 2;
                w_u16(control + 58, value);
                break;
            default:
                break;
        }
        opcode = r_u8(cursor);
    }
    return cursor;
}

/* Bounded native views for the recovered formatter */
static uint32 game_format_native_argument(const uint32 *arguments, uint32 count, uint32 index)
{
    if (!arguments || index >= count)
        abort();
    return arguments[index];
}

static void game_format_native_byte(uint8 *destination, uint32 capacity, uint32 offset, uint32 value)
{
    if (offset >= capacity)
        abort();
    destination[offset] = (uint8)value;
}

static uint32 game_format_text(uint32 destination, uint32 format, uint32 arguments, uint8 *native_destination, uint32 native_capacity, const uint32 *native_arguments, uint32 native_argument_count)
{
    uint8 digits[32], prefix[2];
    uint32 cursor, written, character, flags, width, precision, value, source, length, digit_count, zero_count, prefix_count, radix, digit_table, index, offset, target, searched;
    sint32 code;
    cursor = format;
    written = 0;
    character = (uint32)r_s8(cursor);
    while (character != 0)
    {
        if (character != 37)
        {
            if (native_destination)
                game_format_native_byte(native_destination, native_capacity, (destination + written) - destination, character);
            else
                w_u8(destination + written, character);
            written += 1;
            cursor += 1;
            character = (uint32)r_s8(cursor);
            continue;
        }
        flags = r_u32(0x800BCAF4);
        width = r_u32(0x800BCAF8);
        precision = r_u32(0x800BCAFC);
        for (;;)
        {
            cursor += 1;
            character = (uint32)r_s8(cursor);
            if (character == 45)
                flags |= 1;
            else if (character == 43)
                flags |= 2;
            else if (character == 32)
                flags = (flags & 0xFFFF00FF) | 0x2000;
            else if (character == 35)
                flags |= 4;
            else if (character == 48)
                flags |= 8;
            else
                break;
        }
        if (character == 42)
        {
            width = ((native_arguments || native_destination) ? game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u32(arguments));
            arguments += 4;
            if ((sint32)width < 0)
            {
                width = 0 - width;
                flags |= 1;
            }
            cursor += 1;
            character = (uint32)r_s8(cursor);
        }
        else
            while (character - 48 < 10)
            {
                width = width * 10 + character - 48;
                cursor += 1;
                character = (uint32)r_s8(cursor);
            }
        if (character == 46)
        {
            cursor += 1;
            character = (uint32)r_s8(cursor);
            if (character == 42)
            {
                precision = ((native_arguments || native_destination) ? game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u32(arguments));
                arguments += 4;
                cursor += 1;
                character = (uint32)r_s8(cursor);
            }
            else
                while (character - 48 < 10)
                {
                    precision = precision * 10 + character - 48;
                    cursor += 1;
                    character = (uint32)r_s8(cursor);
                }
            if ((sint32)precision >= 0)
                flags |= 16;
        }
        if ((flags & 1) != 0)
            flags &= 0xFFFFFFF7;
        for (;;)
        {
            if (character == 104)
                flags |= 0x20;
            else if (character == 108)
                flags |= 0x40;
            else if (character == 76)
                flags |= 0x80;
            else
                break;
            cursor += 1;
            character = (uint32)r_s8(cursor);
        }
        code = character;
        digit_count = 0;
        zero_count = 0;
        prefix_count = 0;
        source = 0;
        if (code == 110)
        {
            target = ((native_arguments || native_destination) ? game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u32(arguments));
            arguments += 4;
            if ((flags & 0x20) != 0)
                w_u16(target, written);
            else
                w_u32(target, written);
            cursor += 1;
            character = (uint32)r_s8(cursor);
            continue;
        }
        if (code == 99)
        {
            digits[31] = ((native_arguments || native_destination) ? (uint8)game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u8(arguments));
            arguments += 4;
            digit_count = 1;
            length = 1;
        }
        else if (code == 115)
        {
            source = ((native_arguments || native_destination) ? game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u32(arguments));
            arguments += 4;
            if ((flags & 4) != 0)
            {
                length = r_u8(source);
                source += 1;
                if ((flags & 16) != 0 && (sint32)precision < (sint32)length)
                    length = precision;
            }
            else if ((flags & 16) != 0)
            {
                searched = 0;
                if (source != 0 && (sint32)precision > 0)
                    for (index = 0; index < precision; ++index)
                    {
                        value = r_u8(source + index);
                        if (value == 0)
                        {
                            searched = source + index;
                            break;
                        }
                    }
                length = searched != 0 ? searched - source : precision;
            }
            else
                length = guest_strlen(source);
        }
        else if (code == 100 || code == 105 || code == 117 || code == 111 || code == 88 || code == 120 || code == 112)
        {
            if (code == 112)
            {
                precision = 8;
                flags |= 0x50;
            }
            value = ((native_arguments || native_destination) ? game_format_native_argument(native_arguments, native_argument_count, arguments / 4) : r_u32(arguments));
            arguments += 4;
            if (code == 100 || code == 105)
            {
                if ((flags & 0x20) != 0)
                    value = (uint32)(sint32)(sint16)value;
                if ((sint32)value < 0)
                {
                    value = 0 - value;
                    flags = (flags & 0xFFFF00FF) | 0x2D00;
                }
                else if ((flags & 2) != 0)
                    flags = (flags & 0xFFFF00FF) | 0x2B00;
            }
            else
            {
                if ((flags & 0x20) != 0)
                    value &= 0xFFFF;
                if (code == 117)
                    flags &= 0xFFFF00FF;
            }
            radix = code == 111 ? 8 : code == 100 || code == 105 || code == 117 ? 10 : 16;
            if ((flags & 16) == 0)
            {
                if ((flags & 8) != 0)
                {
                    precision = width;
                    if (radix == 10 && ((flags >> 8) & 255) != 0)
                        precision -= 1;
                    else if (radix == 16 && (flags & 4) != 0)
                        precision -= 2;
                }
                if ((sint32)precision <= 0)
                    precision = 1;
            }
            digit_table = code == 120 ? 0x80025648 : 0x80025634;
            while (value != 0)
            {
                if (radix == 10)
                {
                    digits[31 - digit_count] = value % 10 + 48;
                    value /= 10;
                }
                else if (radix == 8)
                {
                    digits[31 - digit_count] = (value & 7) + 48;
                    value >>= 3;
                }
                else
                {
                    index = value & 15;
                    value >>= 4;
                    digits[31 - digit_count] = r_u8(digit_table + index);
                }
                digit_count += 1;
            }
            if (radix == 8 && (flags & 4) != 0 && digit_count != 0 && digits[32 - digit_count] != 48)
            {
                digits[31 - digit_count] = 48;
                digit_count += 1;
            }
            /* Emits precision zeroes without extending the local digit buffer */
            if ((sint32)digit_count < (sint32)precision)
                zero_count = precision - digit_count;
            if (radix == 10 && ((flags >> 8) & 255) != 0)
            {
                prefix[0] = flags >> 8;
                prefix_count = 1;
            }
            else if (radix == 16 && (flags & 4) != 0)
            {
                prefix[0] = 48;
                prefix[1] = character;
                prefix_count = 2;
            }
            length = prefix_count + zero_count + digit_count;
        }
        else
        {
            if (code != 37)
                break;
            if (native_destination)
                game_format_native_byte(native_destination, native_capacity, (destination + written) - destination, character);
            else
                w_u8(destination + written, character);
            written += 1;
            cursor += 1;
            character = (uint32)r_s8(cursor);
            continue;
        }
        if ((sint32)length < (sint32)width && (flags & 1) == 0)
        {
            do
            {
                if (native_destination)
                    game_format_native_byte(native_destination, native_capacity, (destination + written) - destination, 32);
                else
                    w_u8(destination + written, 32);
                width -= 1;
                written += 1;
            } while ((sint32)length < (sint32)width);
        }
        if (code == 115)
        {
            target = destination + written;
            if ((sint32)length > 0)
            {
                if (target < source)
                {
                    for (index = 0; index < length; ++index)
                    {
                        value = r_u8(source + index);
                        if (native_destination)
                            game_format_native_byte(native_destination, native_capacity, (target + index) - destination, value);
                        else
                            w_u8(target + index, value);
                    }
                }
                else
                {
                    index = length;
                    do
                    {
                        index -= 1;
                        value = r_u8(source + index);
                        if (native_destination)
                            game_format_native_byte(native_destination, native_capacity, (target + index) - destination, value);
                        else
                            w_u8(target + index, value);
                    } while (index != 0);
                }
            }
        }
        else
        {
            offset = 0;
            for (index = 0; index < prefix_count; ++index)
            {
                if (native_destination)
                    game_format_native_byte(native_destination, native_capacity, (destination + written + offset) - destination, prefix[index]);
                else
                    w_u8(destination + written + offset, prefix[index]);
                offset += 1;
            }
            for (index = 0; index < zero_count; ++index)
            {
                if (native_destination)
                    game_format_native_byte(native_destination, native_capacity, (destination + written + offset) - destination, 48);
                else
                    w_u8(destination + written + offset, 48);
                offset += 1;
            }
            for (index = 0; index < digit_count; ++index)
            {
                if (native_destination)
                    game_format_native_byte(native_destination, native_capacity, (destination + written + offset) - destination, digits[32 - digit_count + index]);
                else
                    w_u8(destination + written + offset, digits[32 - digit_count + index]);
                offset += 1;
            }
        }
        written += length;
        while ((sint32)length < (sint32)width)
        {
            if (native_destination)
                game_format_native_byte(native_destination, native_capacity, (destination + written) - destination, 32);
            else
                w_u8(destination + written, 32);
            length += 1;
            written += 1;
        }
        cursor += 1;
        character = (uint32)r_s8(cursor);
    }
    if (native_destination)
        game_format_native_byte(native_destination, native_capacity, (destination + written) - destination, 0);
    else
        w_u8(destination + written, 0);
    return written;
}

uint32 sub_800ae904(uint32 destination, uint32 format, uint32 arguments)
{
    FUNCTION_MARKER(0x800AE904, "ACE3.BIN");
    return game_format_text(destination, format, arguments, 0, 0, 0, 0);
}

uint32 game_sprintf_native(uint8 *destination, uint32 capacity, uint32 format, const uint32 *arguments, uint32 argument_count)
{
    if (!destination || capacity == 0)
        abort();
    return game_format_text(0, format, 0, destination, capacity, arguments, argument_count);
}

uint32 game_sprintf_guest_words(uint32 destination, uint32 format, const uint32 *arguments, uint32 argument_count)
{
    return game_format_text(destination, format, 0, 0, 0, arguments, argument_count);
}

/* Native helper for the original polygon NCLIP sequences */
int collision_ray_clip(uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 depth, int quad, int reverse, uint32 *distance)
{
    uint32 x, y, z, numerator, area;
    x = r_s16(first);
    y = r_s16(first + 2);
    z = r_s16(first + 4);
    xport_gte_write_control(8, ((0 - x) & 0xFFFF) | ((0 - y) << 16));
    xport_gte_write_control(9, 0 - z);
    xport_gte_execute(0x4A6412);
    xport_gte_write_data(12, r_u32(first));
    xport_gte_write_data(13, r_u32(second));
    numerator = 0 - (xport_gte_read_data(25) << 12);
    *distance = (uint32)((sint64)(sint32)numerator / (sint32)depth);
    xport_gte_execute(0x1400006);
    area = xport_gte_read_data(24);
    xport_gte_write_data(12, r_u32(quad ? fourth : third));
    xport_gte_execute(0x1400006);
    if (reverse ? (sint32)area > 0 : (sint32)area < 0)
        return 0;
    area = xport_gte_read_data(24);
    xport_gte_write_data(13, r_u32(quad ? third : first));
    xport_gte_execute(0x1400006);
    if (reverse ? (sint32)area < 0 : (sint32)area > 0)
        return 0;
    area = xport_gte_read_data(24);
    if (quad)
    {
        xport_gte_write_data(12, r_u32(first));
        xport_gte_execute(0x1400006);
        if ((sint32)area < 0)
            return 0;
        return (sint32)xport_gte_read_data(24) <= 0;
    }
    return reverse ? (sint32)area <= 0 : (sint32)area >= 0;
}

uint32 sub_80099894(uint32 direction, uint32 out, uint32 workspace, uint32 models, uint32 records, const uint32 *native_direction, uint32 *native_out, const uint8 *native_records)
{
    SVECTOR vector = {0};
    uint16 normal[3];
    uint8 matrix[32] = {0}, basis[32] = {0};
    uint32 words[5], index, column, a, b, c, maximum, best = 0x7FFFFFFF;
    uint32 descriptor = 0, flags, model, scene, normals, list, kind, count, row, polygon;
    uint32 face, face_count, source_normal, initial_normal, row_flags, hit_flags, packed, depth, distance;
    uint32 first, second, third, fourth, chosen_normal, chosen_flags;
    int quad, clipped;
    FUNCTION_MARKER(0x80099894, "ACE3.BIN");
    vector.vx = native_direction ? (sint16)native_direction[0] : (sint16)r_u16(direction);
    vector.vy = native_direction ? (sint16)native_direction[1] : (sint16)r_u16(direction + 4);
    vector.vz = native_direction ? (sint16)native_direction[2] : (sint16)r_u16(direction + 8);
    a = VectorNormalSS(&vector, &vector);
    maximum = sub_80083d88(a);
    normal[0] = (uint16)vector.vx;
    normal[1] = (uint16)vector.vy;
    normal[2] = (uint16)vector.vz;
    sub_80099428(0, 0, matrix, normal);
    xport_store_le32(matrix + 28, 0);
    xport_store_le32(matrix + 24, 0);
    xport_store_le32(matrix + 20, 0);
    for (index = 0; index < 9; ++index)
        xport_store_le16(basis + index * 2, index % 4 == 0 ? 4096 : 0);
    for (index = 0; index < 5; ++index)
        words[index] = xport_load_le32(matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (column = 0; column < 3; ++column)
    {
        a = xport_load_le16(basis + column * 2);
        b = xport_load_le16(basis + column * 2 + 6);
        c = xport_load_le16(basis + column * 2 + 12);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        xport_store_le16(basis + column * 2, a);
        xport_store_le16(basis + column * 2 + 6, b);
        xport_store_le16(basis + column * 2 + 12, c);
    }
    for (index = 0; index < 3; ++index)
        xport_gte_write_control(index + 5, xport_load_le32(matrix + 20 + index * 4));
    b = xport_load_le16(basis + 24);
    a = xport_load_le16(basis + 20);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, xport_load_le32(basis + 28));
    xport_gte_execute(0x480012);
    xport_store_le32(basis + 20, xport_gte_read_data(25));
    xport_store_le32(basis + 24, xport_gte_read_data(26));
    xport_store_le32(basis + 28, xport_gte_read_data(27));
    for (index = 0; index < 5; ++index)
        words[index] = xport_load_le32(basis + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (index = 0; index < 3; ++index)
        xport_gte_write_control(index + 5, xport_load_le32(basis + 20 + index * 4));
    xport_gte_write_data(14, 0);
    flags = native_records ? xport_load_le32(native_records + 8) : r_u32(records + 8);
    while ((flags & 0x200000) == 0)
    {
        model = r_u32(models + (flags & 0x1FF) * 4);
        sub_80099750(r_u32(model + 4), records + descriptor, workspace, native_records ? native_records + descriptor : 0);
        scene = r_u32(0x800BCE54);
        list = r_u32(model + 16);
        normals = r_u32(scene + 604);
        for (kind = 0; kind < 2; ++kind)
        {
            if (kind != 0)
                list = r_u32(model + 12);
            count = r_u32(list);
            row = list + 4;
            for (polygon = 0; (sint32)polygon < (sint32)count; ++polygon, row += kind == 0 ? 48 : 36)
            {
                initial_normal = normals + r_u16(row + 4);
                xport_gte_write_data(0, r_u32(initial_normal));
                xport_gte_write_data(1, r_u32(initial_normal + 4));
                row_flags = r_u32(row);
                face_count = (row_flags & 0x200) ? 2 : 1;
                for (face = 0; face < face_count; ++face)
                {
                    source_normal = initial_normal;
                    if (face != 0)
                    {
                        source_normal = normals + r_u16(row + 6);
                        xport_gte_write_data(0, r_u32(source_normal));
                        xport_gte_write_data(1, r_u32(source_normal + 4));
                    }
                    xport_gte_execute(0x486012);
                    packed = r_u32(row + 16);
                    a = xport_gte_read_data(9);
                    b = xport_gte_read_data(10);
                    depth = xport_gte_read_data(11);
                    xport_gte_write_data(1, depth);
                    xport_gte_write_data(0, (a & 0xFFFF) | (b << 16));
                    if ((sint32)depth >= 0)
                        continue;
                    first = workspace + (packed & 255) * 8;
                    second = workspace + ((packed >> 8) & 255) * 8;
                    third = workspace + ((packed >> 16) & 255) * 8;
                    fourth = workspace + (packed >> 24) * 8;
                    quad = (row_flags & 0x300) == 0;
                    /* Fix014 defines the second triangle base even when its first normal was rejected */
                    clipped = face != 0 ? collision_ray_clip(second, third, fourth, 0, depth, 0, 1, &distance) : collision_ray_clip(first, second, third, fourth, depth, quad, 0, &distance);
                    if (!clipped)
                        continue;
                    if (face == 0 && (row_flags & 0x200))
                        hit_flags = row_flags;
                    else if (face != 0)
                        hit_flags = r_u32(row);
                    else
                    {
                        if (kind == 0)
                        {
                            source_normal = normals + r_u16(row + 4);
                            hit_flags = r_u32(row);
                        }
                        else
                        {
                            a = r_u16(row + 4);
                            hit_flags = r_u32(row);
                            source_normal = normals + a;
                        }
                    }
                    if ((sint32)distance >= 0 && (sint32)maximum >= (sint32)distance && (sint32)distance < (sint32)best)
                    {
                        best = distance;
                        chosen_normal = source_normal;
                        chosen_flags = hit_flags;
                    }
                }
            }
        }
        descriptor += 12;
        flags = native_records ? xport_load_le32(native_records + descriptor + 8) : r_u32(records + descriptor + 8);
    }
    if ((sint32)best > 0x7FFF)
    {
        if (native_out)
        {
            native_out[2] = 0;
            native_out[1] = 0;
            native_out[0] = 0;
        }
        else
        {
            w_u32(out + 8, 0);
            w_u32(out + 4, 0);
            w_u32(out, 0);
        }
        w_u32(0x800BD0C0, 0);
    }
    else
    {
        a = (uint32)(sint32)vector.vx * best;
        if (native_out)
            native_out[0] = a;
        else
            w_u32(out, a);
        a = (uint32)(sint32)vector.vy * best;
        if (native_out)
            native_out[1] = a;
        else
            w_u32(out + 4, a);
        a = (uint32)(sint32)vector.vz * best;
        w_u32(0x800BD0C0, chosen_normal);
        w_u32(0x800BD0C4, chosen_flags);
        if (native_out)
            native_out[2] = a;
        else
            w_u32(out + 8, a);
    }
    return best;
}
