#include "game_functions.h"
#include <stdlib.h>

uint32 sub_800a68a8(uint32 matrix, uint32 vector, uint32 output)
{
    uint32 words[5], input[3], high[3], low[3], mac[3], result[3];
    uint32 index, magnitude;

    FUNCTION_MARKER(0x800A68A8, "ACE3.BIN");
    for (index = 0; index < 5; ++index)
        words[index] = r_u32(matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (index = 0; index < 3; ++index)
        input[index] = r_u32(vector + index * 4);
    for (index = 0; index < 3; ++index)
    {
        magnitude = (sint32)input[index] < 0 ? 0 - input[index] : input[index];
        high[index] = (uint32)((sint32)magnitude >> 15);
        low[index] = magnitude & 0x7FFF;
        if ((sint32)input[index] < 0)
        {
            high[index] = 0 - high[index];
            low[index] = 0 - low[index];
        }
    }
    for (index = 0; index < 3; ++index)
        xport_gte_write_data(9 + index, high[index]);
    xport_gte_execute(0x41E012);
    for (index = 0; index < 3; ++index)
        mac[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        xport_gte_write_data(9 + index, low[index]);
    xport_gte_execute(0x49E012);
    for (index = 0; index < 3; ++index)
        result[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        result[index] += mac[index] << 3;
    for (index = 0; index < 3; ++index)
        w_u32(output + index * 4, result[index]);
    return output;
}

uint32 resource002_sub_801016a4(uint32 effect, uint32 scratch)
{
    uint32 value, words[4], index;

    FUNCTION_MARKER(0x801016A4, "ACE3.BIN");
    w_u32(effect + 136, 0);
    w_u32(effect + 140, 0);
    w_u32(effect + 144, 0);
    w_u32(effect + 148, 0);
    w_u32(effect + 152, 0);
    RotMatrix((SVECTOR *)psx_addr(effect + 136, 6), (MATRIX *)psx_addr(effect + 76, 32));
    value = r_u32(effect + 144);
    w_u32(scratch + 216, 0 - value);
    value = r_u32(effect + 148);
    w_u32(scratch + 220, 0 - value);
    value = r_u32(effect + 152);
    w_u32(scratch + 224, 0 - value);
    sub_800a6ed8(effect + 76, scratch + 216);
    for (index = 0; index < 4; ++index)
        words[index] = r_u32(effect + 76 + index * 4);
    for (index = 0; index < 4; ++index)
        w_u32(effect + 44 + index * 4, words[index]);
    for (index = 0; index < 4; ++index)
        words[index] = r_u32(effect + 92 + index * 4);
    for (index = 0; index < 4; ++index)
        w_u32(effect + 60 + index * 4, words[index]);
    TransposeMatrix((MATRIX *)psx_addr(effect + 76, 18), (MATRIX *)psx_addr(effect + 44, 18));
    sub_800a68a8(effect + 44, scratch + 216, effect + 160);
    return sub_800a6ed8(effect + 44, effect + 160);
}

uint32 resource002_sub_80101ad4(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 row, item, component, offset, pointer, vector, value, random;
    uint32 a, b, c;

    FUNCTION_MARKER(0x80101AD4, "ACE3.BIN");
    pointer = r_u32(effect + 8);
    packet = resource002_sub_80101514(0, pointer, packet);
    for (row = 0; row < 3; ++row)
    {
        offset = row * 4;
        for (item = 14; item > 0; --item)
        {
            for (component = 0; component < 3; ++component)
            {
                pointer = r_u32(effect + 244);
                vector = r_u32(pointer + offset) + item * 16;
                value = r_u32(vector - 16 + component * 4);
                w_u32(vector + component * 4, value);
            }
            for (component = 0; component < 3; ++component)
            {
                pointer = r_u32(effect + 240);
                vector = r_u32(pointer + offset) + item * 8;
                value = r_u16(vector - 8 + component * 2);
                w_u16(vector + component * 2, value);
            }
        }
        for (component = 0; component < 3; ++component)
        {
            random = game_random();
            pointer = r_u32(effect + 240);
            vector = r_u32(pointer + offset);
            value = r_u16(vector + component * 2);
            w_u16(vector + component * 2, value + (random & 31));
        }
        pointer = r_u32(effect + 240);
        vector = r_u32(pointer + offset);
        RotMatrix((SVECTOR *)psx_addr(vector, 6), (MATRIX *)psx_addr(scratch, 32));
        pointer = r_u32(effect + 244);
        vector = r_u32(pointer + offset);
        sub_800a6ed8(scratch, vector + 16);
        a = r_u32(scratch);
        b = r_u32(scratch + 4);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(scratch + 8);
        b = r_u32(scratch + 12);
        c = r_u32(scratch + 16);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        a = r_u32(scratch + 20);
        b = r_u32(scratch + 24);
        xport_gte_write_control(5, a);
        c = r_u32(scratch + 28);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        xport_gte_write_data(0, r_u32(effect + 252));
        xport_gte_write_data(1, r_u32(effect + 256));
        xport_gte_execute(0x480012);
        pointer = r_u32(effect + 244);
        vector = r_u32(pointer + offset);
        w_u32(vector, xport_gte_read_data(25));
        w_u32(vector + 4, xport_gte_read_data(26));
        w_u32(vector + 8, xport_gte_read_data(27));
        pointer = r_u32(effect + 244);
        vector = r_u32(pointer + offset);
        if (!resource002_sub_80101a0c(vector))
        {
            resource002_sub_80100fdc(row, effect);
            continue;
        }
        for (item = 0; item < 15; ++item)
        {
            pointer = r_u32(effect + 240);
            vector = r_u32(pointer + offset);
            RotMatrix((SVECTOR *)psx_addr(vector + item * 8, 6), (MATRIX *)psx_addr(scratch, 32));
            pointer = r_u32(effect + 244);
            vector = r_u32(pointer + offset);
            sub_800a6ed8(scratch, vector + item * 16);
            sub_800a6748(effect + 44, scratch, scratch + 64);
            pointer = r_u32(effect + 248);
            resource002_sub_801017a8(pointer, scratch, 5);
            if (item)
                packet = resource002_sub_801018d8(effect, scratch + 96, item - 1, packet);
            for (component = 0; component < 5; ++component)
            {
                value = r_u32(scratch + 96 + component * 4);
                w_u32(scratch + 116 + component * 4, value);
            }
        }
    }
    pointer = r_u32(effect + 8);
    return resource002_sub_80101514(1, pointer + 1020, packet);
}

uint32 resource002_sub_80101514(uint32 mode, uint32 ordering, uint32 packet)
{
    uint32 link;

    FUNCTION_MARKER(0x80101514, "ACE3.BIN");
    sub_800ac7f8(packet, 0, 0, (mode & 3) << 5, 0);
    /* The original GPU packet ABI requires word-aligned packet and OT pointers */
    link = r_masked_u32(ordering, 7);
    w_masked_u32(packet, link, 7);
    w_masked_u32(ordering, packet, 7);
    return packet + 12;
}

uint32 resource002_sub_80101a0c(uint32 vector)
{
    uint32 index, visible;
    sint32 depth, x, y, width, height;

    FUNCTION_MARKER(0x80101A0C, "ACE3.BIN");
    visible = 0;
    for (index = 0; index < 15; ++index)
    {
        depth = r_s32(vector + index * 16 + 8);
        y = r_s32(vector + index * 16 + 4);
        x = r_s32(vector + index * 16);
        if (depth > 32768 || depth < 4096)
            continue;
        width = depth * 160 / 250;
        height = depth * 240 / 250;
        if (x >= -width && x <= width && y >= -height && y <= height)
            visible = 1;
    }
    return visible;
}

uint32 resource002_sub_801017a8(uint32 vertices, uint32 scratch, uint32 count)
{
    uint32 matrix, output, a, b, c, index;

    FUNCTION_MARKER(0x801017A8, "ACE3.BIN");
    matrix = scratch + 64;
    output = scratch + 96;
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
    index = 0;
    for (;;)
    {
        xport_gte_write_data(0, r_u32(vertices));
        xport_gte_write_data(1, r_u32(vertices + 4));
        vertices += 8;
        ++index;
        if ((sint32)index >= (sint32)count)
        {
            xport_gte_execute(0x280030);
            output += (index - 1) * 4;
            w_u32(output, xport_gte_read_data(12));
            return output;
        }
        xport_gte_write_data(2, r_u32(vertices));
        xport_gte_write_data(3, r_u32(vertices + 4));
        vertices += 8;
        ++index;
        if ((sint32)index >= (sint32)count)
        {
            xport_gte_execute(0x280030);
            w_u32(output + (index - 2) * 4, xport_gte_read_data(12));
            output += (index - 1) * 4;
            w_u32(output, xport_gte_read_data(13));
            return output;
        }
        xport_gte_write_data(4, r_u32(vertices));
        xport_gte_write_data(5, r_u32(vertices + 4));
        vertices += 8;
        xport_gte_execute(0x280030);
        ++index;
        w_u32(output + (index - 3) * 4, xport_gte_read_data(12));
        w_u32(output + (index - 2) * 4, xport_gte_read_data(13));
        w_u32(output + (index - 1) * 4, xport_gte_read_data(14));
        if ((sint32)index >= (sint32)count)
            return 0;
    }
}

uint32 resource002_sub_801018d8(uint32 effect, uint32 vertices, uint32 item, uint32 packet)
{
    uint32 ordering, shade[2], index, input, high, quotient, color, next_color;
    uint32 other, x, y, value, link;
    sint64 product;

    FUNCTION_MARKER(0x801018D8, "ACE3.BIN");
    ordering = r_u32(effect + 8);
    for (index = 0; index < 2; ++index)
    {
        input = (item + index) << 6;
        product = (sint64)(sint32)input * (sint32)0x88888889;
        high = (uint32)(product >> 32);
        quotient = (uint32)((sint32)(high + input) >> 3) - (uint32)((sint32)input >> 31);
        shade[index] = (uint32)(sint32)(sint16)(64 - quotient);
    }
    color = shade[0] | (shade[0] << 8) | (shade[0] << 16);
    next_color = shade[1] | (shade[1] << 8) | (shade[1] << 16);
    other = vertices + 4;
    for (index = 0; index < 4; ++index)
    {
        w_u32(packet + 4, color);
        w_u32(packet + 12, color);
        w_u32(packet + 20, next_color);
        w_u32(packet + 28, next_color);
        y = r_s16(vertices + 2);
        x = r_u16(vertices);
        w_u32(packet + 8, (y << 16) | x);
        y = r_s16(other + 2);
        x = r_u16(other);
        w_u32(packet + 16, (y << 16) | x);
        y = r_s16(vertices + 22);
        x = r_u16(vertices + 20);
        w_u32(packet + 24, (y << 16) | x);
        y = r_s16(other + 22);
        x = r_u16(other + 20);
        other += 4;
        w_u8(packet + 7, 0x3A);
        value = (y << 16) | x;
        w_u32(packet + 32, value);
        link = r_u32(ordering);
        w_u32(packet, link | 0x08000000);
        w_u32(ordering, packet & 0xFFFFFF);
        packet += 36;
    }
    return packet;
}

uint32 resource002_sub_80100304(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 clut, page, ordering, scroll, ring, sample, table, matrix;
    uint32 a, b, c, index, vertex, offset, color, first, second, third, fourth;
    uint32 uv, bottom, mode, depth, link;

    FUNCTION_MARKER(0x80100304, "ACE3.BIN");
    clut = r_u16(effect + 176);
    page = r_u16(effect + 178);
    scroll = r_u32(effect + 124);
    ordering = r_u32(effect + 8);
    scroll = (scroll & 7) * 4;
    for (ring = 0; ring < 16; ++ring)
    {
        sample = ring + 48;
        table = r_u32(effect + 128);
        sub_800a79a8(table + sample * 8, scratch + 444);
        table = r_u32(effect + 132);
        sub_800a6ed8(scratch + 444, table + sample * 16);
        sub_800a6748(effect + 44, scratch + 444, scratch + 476);
        matrix = scratch + 476;
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
        /* Sixteen vertices form five triples followed by one RTPS */
        for (index = 0; index < 16; index += 3)
        {
            table = r_u32(effect + 260);
            vertex = table + index * 8;
            xport_gte_write_data(0, r_u32(vertex));
            xport_gte_write_data(1, r_u32(vertex + 4));
            if (index == 15)
            {
                xport_gte_execute(0x180001);
                w_u32(scratch + 64, xport_gte_read_data(14));
                w_u32(scratch + 192, xport_gte_read_data(19));
                w_u32(scratch + 320, xport_gte_read_data(24));
                break;
            }
            xport_gte_write_data(2, r_u32(vertex + 8));
            xport_gte_write_data(3, r_u32(vertex + 12));
            xport_gte_write_data(4, r_u32(vertex + 16));
            xport_gte_write_data(5, r_u32(vertex + 20));
            xport_gte_execute(0x280030);
            offset = index * 4;
            w_u32(scratch + 4 + offset, xport_gte_read_data(12));
            w_u32(scratch + 8 + offset, xport_gte_read_data(13));
            w_u32(scratch + 12 + offset, xport_gte_read_data(14));
            color = xport_gte_read_data(8);
            w_u32(scratch + 132 + offset, xport_gte_read_data(17));
            w_u32(scratch + 136 + offset, xport_gte_read_data(18));
            w_u32(scratch + 140 + offset, xport_gte_read_data(19));
            w_u32(scratch + 268 + offset, color);
            w_u32(scratch + 264 + offset, color);
            w_u32(scratch + 260 + offset, color);
        }
        if (ring)
        {
            xport_gte_write_control(21, 2048);
            xport_gte_write_control(22, 2048);
            xport_gte_write_control(23, 2048);
            uv = ((((ring - 1) & 7) * 4 - scroll) & 31);
            bottom = (uv + 199) << 8;
            uv = (uv + 192) << 8;
            for (index = 0; index < 16; ++index)
            {
                mode = r_u32(effect + 20);
                if (mode == 1 && (sint32)(r_u32(effect + 32) - 48) < (sint32)ring)
                    break;
                if (mode == 3 && (sint32)ring < (sint32)(r_u32(effect + 32) - 48))
                    break;
                first = scratch + 68 + index * 4;
                second = scratch + 68 + ((index + 1) & 15) * 4;
                third = scratch + 4 + index * 4;
                fourth = scratch + 4 + ((index + 1) & 15) * 4;
                if (r_s16(first) < 0 && r_s16(second) < 0 && r_s16(third) < 0 && r_s16(fourth) < 0)
                    continue;
                if (r_s16(first + 2) < 0 && r_s16(second + 2) < 0 && r_s16(third + 2) < 0 && r_s16(fourth + 2) < 0)
                    continue;
                if (r_s16(first) >= 321 && r_s16(second) >= 321 && r_s16(third) >= 321 && r_s16(fourth) >= 321)
                    continue;
                if (r_s16(first + 2) >= 481 && r_s16(second + 2) >= 481 && r_s16(third + 2) >= 481 && r_s16(fourth + 2) >= 481)
                    continue;
                a = r_u32(first);
                b = r_u32(second);
                w_u32(packet + 8, a);
                w_u32(packet + 16, b);
                a = r_u32(third);
                b = r_u32(fourth);
                w_u32(packet + 4, 0x2C838383);
                w_u32(packet + 12, uv | (page << 16));
                w_u32(packet + 28, bottom);
                w_u32(packet + 36, bottom | 31);
                w_u32(packet + 20, uv | (clut << 16) | 31);
                w_u32(packet + 24, a);
                w_u32(packet + 32, b);
                depth = (uint32)(r_s32(scratch + 196 + index * 4) >> 8);
                if ((sint32)depth >= 256)
                    depth = 255;
                table = ordering + depth * 4;
                link = r_u32(table);
                w_u32(packet, link | 0x09000000);
                w_u32(table, packet & 0xFFFFFF);
                packet += 40;
            }
        }
        for (index = 0; index < 16; ++index)
        {
            offset = index * 4;
            a = r_u32(scratch + 4 + offset);
            b = r_u32(scratch + 132 + offset);
            c = r_u32(scratch + 260 + offset);
            w_u32(scratch + 68 + offset, a);
            w_u32(scratch + 324 + offset, c);
            w_u32(scratch + 196 + offset, b);
        }
    }
    return packet;
}

uint32 sub_800a79a8(uint32 angles, uint32 output)
{
    uint32 packed, z, y, x, value, sine[3], cosine[3], index, magnitude;
    uint32 first[3], second[3], third[3], fourth[3], product;

    FUNCTION_MARKER(0x800A79A8, "ACE3.BIN");
    z = r_s16(angles + 4);
    packed = r_u32(angles);
    y = (uint32)((sint32)packed >> 16);
    x = (uint32)(sint32)(sint16)packed;
    for (index = 0; index < 3; ++index)
    {
        value = index == 0 ? z : index == 1 ? y : x;
        magnitude = (sint32)value < 0 ? 0 - value : value;
        packed = r_u32(0x800B7104 + ((magnitude << 2) & 0x3FFC));
        sine[index] = (uint32)(sint32)(sint16)packed;
        if ((sint32)value < 0)
            sine[index] = (uint32)(sint32)(sint16)(0 - sine[index]);
        cosine[index] = (uint32)((sint32)packed >> 16);
    }
    xport_gte_write_data(8, cosine[2]);
    xport_gte_write_data(9, sine[1]);
    xport_gte_write_data(10, sine[0]);
    xport_gte_write_data(11, cosine[0]);
    xport_gte_execute(0x198003D);
    product = cosine[1] * cosine[2];
    for (index = 0; index < 3; ++index)
        first[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, sine[2]);
    xport_gte_write_data(9, sine[1]);
    xport_gte_write_data(10, sine[0]);
    xport_gte_write_data(11, cosine[0]);
    xport_gte_execute(0x198003D);
    w_u16(output + 16, (uint32)((sint32)product >> 12));
    for (index = 0; index < 3; ++index)
        second[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, cosine[0]);
    xport_gte_write_data(9, cosine[1]);
    product = cosine[1] * sine[2];
    xport_gte_write_data(10, second[0]);
    xport_gte_write_data(11, first[0]);
    xport_gte_execute(0x198003D);
    for (index = 0; index < 3; ++index)
        third[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, sine[0]);
    xport_gte_write_data(9, cosine[1]);
    xport_gte_write_data(10, second[0]);
    xport_gte_write_data(11, first[0]);
    xport_gte_execute(0x198003D);
    w_u32(output + 4, ((third[1] + first[1]) << 16) | (sine[1] & 0xFFFF));
    fourth[0] = xport_gte_read_data(9);
    w_u32(output, ((0 - fourth[0]) << 16) | (third[0] & 0xFFFF));
    fourth[1] = xport_gte_read_data(10);
    fourth[2] = xport_gte_read_data(11);
    value = 0 - (uint32)((sint32)product >> 12);
    w_u32(output + 8, (value << 16) | ((first[2] - fourth[1]) & 0xFFFF));
    w_u32(output + 12, ((second[2] + fourth[2]) << 16) | ((second[1] - third[2]) & 0xFFFF));
    return output;
}

uint32 resource002_sub_800ff4ec(uint32 effect, uint32 scratch)
{
    uint32 value, step, total, quotient, count, mode, angle, position;
    uint32 anchor[3], a, b, c, index, counter, matrix, x, y, z;
    uint32 dx, dy, dz, result;

    FUNCTION_MARKER(0x800FF4EC, "ACE3.BIN");
    value = r_u32(effect + 12);
    total = r_u16(effect + 114);
    step = (value & 511) - 256;
    if ((sint32)step < 0)
        step = 0 - step;
    step += 128;
    total += step;
    total = (uint32)(sint32)(sint16)total;
    w_u16(effect + 114, total);
    w_u16(effect + 112, step);
    quotient = (uint32)((sint32)total / 390);
    w_u16(effect + 116, quotient);
    count = r_s16(effect + 116);
    w_u16(effect + 114, total - quotient * 390);
    if (count)
    {
        value = r_u32(effect + 124);
        mode = r_u32(effect + 20);
        w_u32(effect + 124, (value + count) & 7);
        if (mode == 1 || mode == 3)
        {
            a = r_s16(effect + 116);
            b = r_u32(effect + 32);
            w_u32(effect + 32, b + a);
        }
        mode = r_u32(effect + 24);
        if (mode == 1 || mode == 3)
        {
            a = r_s16(effect + 116);
            b = r_u32(effect + 36);
            w_u32(effect + 36, b + a);
        }
        count = r_s16(effect + 116);
        angle = r_u32(effect + 128);
        position = r_u32(effect + 132);
        anchor[0] = r_u32(position + 992);
        anchor[1] = r_u32(position + 996);
        anchor[2] = r_u32(position + 1000);
        angle += 496;
        position += 992;
        index = 64;
        if ((sint32)count < 64)
        {
            do
            {
                --index;
                a = r_u32(angle);
                b = r_u32(angle + 4);
                angle -= 8;
                w_u32(angle + 16, a);
                w_u32(angle + 20, b);
                a = r_u32(position);
                position -= 16;
                b = r_u32(position + 20);
                c = r_u32(position + 24);
                w_u32(position + 32, a - anchor[0]);
                w_u32(position + 36, b - anchor[1]);
                w_u32(position + 40, c - anchor[2]);
            } while ((sint32)count < (sint32)index);
        }
        count = r_s16(effect + 116);
        counter = r_s16(effect + 108);
        angle = r_u32(effect + 128) + count * 8;
        position = r_u32(effect + 132) + count * 16;
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 390);
        matrix = scratch + 444;
        ++counter;
        do
        {
            value = (uint32)rsin((sint32)(counter << 4));
            w_u16(angle - 8, (uint32)((sint32)value >> 6));
            w_u16(angle - 6, (uint32)((sint32)value >> 5));
            w_u16(angle - 4, 0);
            sub_800a79a8(angle, matrix);
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
            b = r_u32(position + 4);
            c = r_u32(position + 8);
            a = r_u32(position);
            xport_gte_write_control(5, a);
            xport_gte_write_control(6, b);
            xport_gte_write_control(7, c);
            --count;
            angle -= 8;
            xport_gte_execute(0x480012);
            position -= 16;
            w_u32(position, xport_gte_read_data(25));
            w_u32(position + 4, xport_gte_read_data(26));
            w_u32(position + 8, xport_gte_read_data(27));
            ++counter;
        } while ((sint32)count > 0);
        w_u16(effect + 108, counter - 1);
    }
    value = (uint32)r_s16(effect + 114) << 12;
    position = r_u32(effect + 132) + 992;
    x = r_u32(position);
    a = r_u32(position + 8);
    b = r_u32(position + 24);
    c = r_u32(position + 16);
    dz = a - b;
    a = r_u32(position + 4);
    b = r_u32(position + 20);
    dx = x - c;
    dy = a - b;
    value = (uint32)((sint32)value / 390);
    xport_gte_write_data(8, value);
    xport_gte_write_data(9, dx);
    xport_gte_write_data(10, dy);
    xport_gte_write_data(11, dz);
    w_u16(scratch + 408, 0);
    w_u16(effect + 140, 0);
    xport_gte_execute(0x198003D);
    position = r_u32(effect + 132);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_data(9, r_u32(position + 672));
    xport_gte_write_data(10, r_u32(position + 676));
    xport_gte_write_data(11, r_u32(position + 680));
    xport_gte_write_control(21, r_u32(position + 688));
    xport_gte_write_control(22, r_u32(position + 692));
    xport_gte_write_control(23, r_u32(position + 696));
    w_u32(effect + 144, x);
    w_u32(effect + 148, y);
    xport_gte_execute(0x980011);
    w_u32(effect + 152, z);
    w_u32(effect + 96, 0 - x);
    w_u32(scratch + 428, 0 - x);
    w_u32(effect + 100, 0 - y);
    w_u32(scratch + 432, 0 - y);
    w_u32(effect + 104, 0 - z);
    w_u32(scratch + 436, 0 - z);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    dx = a - x;
    dz = c - z;
    dy = b - y;
    value = (uint32)ratan2((sint32)dy, (sint32)dz);
    w_u16(scratch + 404, value);
    w_u16(effect + 136, value);
    value = (uint32)ratan2((sint32)dx, (sint32)dz);
    w_u16(scratch + 406, value);
    w_u16(effect + 138, value);
    RotMatrix((SVECTOR *)psx_addr(scratch + 404, 6), (MATRIX *)psx_addr(effect + 76, 32));
    TransposeMatrix((MATRIX *)psx_addr(effect + 76, 18), (MATRIX *)psx_addr(effect + 44, 18));
    sub_800a68a8(effect + 44, scratch + 428, scratch + 428);
    a = r_u32(scratch + 428);
    b = r_u32(scratch + 432);
    result = r_u32(scratch + 436);
    w_u32(effect + 64, a);
    w_u32(effect + 160, a);
    w_u32(effect + 68, b);
    w_u32(effect + 164, b);
    w_u32(effect + 72, result);
    w_u32(effect + 168, result);
    return result;
}

uint32 resource002_sub_800ff95c(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 clut, page, ordering, scroll, ring, table, matrix, vertex;
    uint32 index, lane, a, b, c, value, phase, quotient, mode, offset, color;
    uint32 first, second, third, fourth, uv, bottom, depth, link, upload;

    FUNCTION_MARKER(0x800FF95C, "ACE3.BIN");
    clut = r_u16(effect + 176);
    page = r_u16(effect + 178);
    scroll = r_u32(effect + 124);
    ordering = r_u32(effect + 8);
    scroll = 224 - ((scroll * 4) & 31);
    for (ring = 0; ring < 7; ++ring)
    {
        table = r_u32(0x80112DA8);
        table = r_u32(table + 128);
        sub_800a79a8(table + ring * 64, scratch + 444);
        table = r_u32(effect + 132);
        sub_800a6ed8(scratch + 444, table + ring * 128);
        sub_800a6748(effect + 44, scratch + 444, scratch + 476);
        matrix = scratch + 476;
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
        for (index = 0; index < 16; index += 3)
        {
            for (lane = 0; lane < 3 && index + lane < 16; ++lane)
            {
                phase = r_u32(effect + 32);
                quotient = (uint32)((sint32)phase / 8);
                if (quotient == ring - 1)
                {
                    mode = r_u32(effect + 20);
                    if (lane != 1)
                        table = r_u32(effect + 260);
                    value = mode == 1 ? (7 - (phase - ((ring - 1) << 3))) * 25000 : 0;
                    value = (uint32)((sint32)value / 64);
                    w_u16(table + (index + lane) * 8 + 4, value);
                }
                else if (quotient == ring)
                {
                    mode = r_u32(effect + 20);
                    if (lane != 1)
                        table = r_u32(effect + 260);
                    value = mode == 3 ? (uint32)((sint32)(0 - phase) % 8) * 25000 : 0;
                    value = (uint32)((sint32)value / 64);
                    w_u16(table + (index + lane) * 8 + 4, value);
                }
                table = r_u32(effect + 260);
                vertex = table + (index + lane) * 8;
                xport_gte_write_data(lane * 2, r_u32(vertex));
                xport_gte_write_data(lane * 2 + 1, r_u32(vertex + 4));
            }
            if (index == 15)
            {
                xport_gte_execute(0x180001);
                w_u32(scratch + 64, xport_gte_read_data(14));
                w_u32(scratch + 192, xport_gte_read_data(19));
                w_u32(scratch + 320, xport_gte_read_data(24));
                break;
            }
            xport_gte_execute(0x280030);
            offset = index * 4;
            w_u32(scratch + 4 + offset, xport_gte_read_data(12));
            w_u32(scratch + 8 + offset, xport_gte_read_data(13));
            w_u32(scratch + 12 + offset, xport_gte_read_data(14));
            color = xport_gte_read_data(8);
            w_u32(scratch + 132 + offset, xport_gte_read_data(17));
            w_u32(scratch + 136 + offset, xport_gte_read_data(18));
            w_u32(scratch + 140 + offset, xport_gte_read_data(19));
            w_u32(scratch + 268 + offset, color);
            w_u32(scratch + 264 + offset, color);
            w_u32(scratch + 260 + offset, color);
        }
        if (ring)
        {
            xport_gte_write_control(21, 2048);
            xport_gte_write_control(22, 2048);
            xport_gte_write_control(23, 2048);
            uv = scroll << 8;
            bottom = (scroll + 31) << 8;
            for (index = 0; index < 16; ++index)
            {
                mode = r_u32(effect + 20);
                if (mode == 1)
                {
                    phase = r_u32(effect + 32);
                    if ((sint32)phase / 8 < (sint32)(ring - 1))
                        break;
                    mode = r_u32(effect + 20);
                }
                if (mode == 3)
                {
                    phase = r_u32(effect + 32);
                    if ((sint32)(ring - 1) < (sint32)phase / 8)
                        break;
                }
                first = scratch + 68 + index * 4;
                second = scratch + 68 + ((index + 1) & 15) * 4;
                third = scratch + 4 + index * 4;
                fourth = scratch + 4 + ((index + 1) & 15) * 4;
                if (r_s16(first) < 0 && r_s16(second) < 0 && r_s16(third) < 0 && r_s16(fourth) < 0)
                    continue;
                if (r_s16(first + 2) < 0 && r_s16(second + 2) < 0 && r_s16(third + 2) < 0 && r_s16(fourth + 2) < 0)
                    continue;
                if (r_s16(first) >= 321 && r_s16(second) >= 321 && r_s16(third) >= 321 && r_s16(fourth) >= 321)
                    continue;
                if (r_s16(first + 2) >= 481 && r_s16(second + 2) >= 481 && r_s16(third + 2) >= 481 && r_s16(fourth + 2) >= 481)
                    continue;
                a = r_u32(first);
                b = r_u32(third);
                c = r_u32(fourth);
                xport_gte_write_data(12, a);
                xport_gte_write_data(13, b);
                xport_gte_write_data(14, c);
                w_u32(packet + 8, a);
                w_u32(packet + 24, b);
                xport_gte_execute(0x1400006);
                w_u32(packet + 4, 0x2C838383);
                value = r_u32(second);
                w_u32(packet + 32, c);
                w_u32(packet + 16, value);
                if ((sint32)xport_gte_read_data(24) < 0)
                    continue;
                w_u32(packet + 12, uv | (page << 16));
                w_u32(packet + 20, uv | (clut << 16) | 31);
                w_u32(packet + 28, bottom);
                w_u32(packet + 36, bottom | 31);
                depth = (uint32)(r_s32(scratch + 196 + index * 4) >> 8);
                if ((sint32)depth >= 256)
                    depth = 255;
                table = ordering + depth * 4;
                link = r_u32(table);
                w_u32(packet, link | 0x09000000);
                w_u32(table, packet & 0xFFFFFF);
                packet += 40;
            }
            w_u32(packet, 0x0C000000);
            w_u32(packet + 4, 0xA0000000);
            w_u32(packet + 48, 0x01000000);
            w_u32(packet + 12, 0x00010010);
            table = r_u32(0x800BCE48);
            upload = packet;
            w_u32(packet + 8, 0x00D103F0);
            packet += 52;
            value = r_u8(table + 16);
            xport_gte_write_control(21, value << 4);
            value = r_u8(table + 17);
            xport_gte_write_control(22, value << 4);
            value = r_u8(table + 18);
            xport_gte_write_control(23, value << 4);
            for (index = 0; index < 16; ++index)
            {
                a = r_s16(effect + 188 + index * 2);
                b = r_u32(scratch + 260);
                xport_gte_write_data(28, a);
                xport_gte_write_data(8, b);
                xport_gte_execute(0x980011);
                value = xport_gte_read_data(29);
                w_u16(upload + 16 + index * 2, value);
            }
            depth = (uint32)(r_s32(scratch + 196) >> 8) + 5;
            if ((sint32)depth >= 256)
                depth = 255;
            table = ordering + depth * 4;
            link = r_masked_u32(table, 7);
            w_masked_u32(upload, link, 7);
            w_masked_u32(table, upload, 7);
        }
        for (index = 0; index < 16; ++index)
        {
            offset = index * 4;
            a = r_u32(scratch + 4 + offset);
            b = r_u32(scratch + 132 + offset);
            c = r_u32(scratch + 260 + offset);
            w_u32(scratch + 68 + offset, a);
            w_u32(scratch + 196 + offset, b);
            w_u32(scratch + 324 + offset, c);
        }
    }
    return packet;
}

uint32 resource002_sub_800ed000(uint32 resource, uint32 x, uint32 y, uint32 clut_x, uint32 clut_y)
{
    uint32 temporary, data;

    FUNCTION_MARKER(0x800ED000, "ACE3.BIN");
    temporary = arena_alloc(0x9000);
    if (r_u32(resource) == 0x1A7A6C55)
    {
        sub_800afd08(resource, temporary);
        data = temporary;
    }
    else
        data = resource;
    sub_80027084(data, (uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y, (uint32)(sint32)(sint16)clut_x, (uint32)(sint32)(sint16)clut_y);
    DrawSync(0);
    return arena_set_cursor(temporary);
}

uint32 resource002_sub_800cf56c(uint32 page, uint32 owner)
{
    uint32 count, quotient, remaining, index, position, state, table, id, data;

    FUNCTION_MARKER(0x800CF56C, "ACE3.BIN");
    count = r_u32(owner + 596);
    quotient = (uint32)((sint32)count / 4);
    remaining = (sint32)page < (sint32)quotient ? 4 : count - quotient * 4;
    if ((sint32)remaining <= 0)
        return quotient * 4;
    for (index = 0; (sint32)index < (sint32)remaining; ++index)
    {
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        position = page * 4 + index;
        if ((sint32)position >= (sint32)count)
            continue;
        state = r_u32(0x80112DC4);
        id = r_u8(owner + 516 + position);
        table = r_u32(state + 468);
        data = r_u32(table + id * 4);
        resource002_sub_800ed000(data, 960, 340 + index * 18, 384, 6);
    }
    return 0;
}

uint32 resource002_sub_800cf564(uint32 page)
{
    FUNCTION_MARKER(0x800CF564, "ACE3.BIN");
    return resource002_sub_800cf56c(page, r_u32(0x80112DD0));
}

uint32 resource002_sub_800cf648(uint32 item, uint32 owner)
{
    uint32 page, count, index, state, id, table, data;

    FUNCTION_MARKER(0x800CF648, "ACE3.BIN");
    page = r_u8(owner + 2);
    count = r_u32(owner + 596);
    index = item + page * 4;
    if ((sint32)index >= (sint32)count)
        return page * 4;
    state = r_u32(0x80112DC4);
    id = r_u8(owner + 516 + index);
    table = r_u32(state + 476);
    data = r_u32(table + id * 4);
    return resource002_sub_800ed000(data, 960, 256, 768, 1);
}

uint32 resource002_sub_800cf640(uint32 item)
{
    FUNCTION_MARKER(0x800CF640, "ACE3.BIN");
    return resource002_sub_800cf648(item, r_u32(0x80112DD0));
}

uint32 resource002_sub_800d0838(uint32 page, uint32 globals, uint32 owner)
{
    uint32 count, quotient, remaining, index, position, state, table, id, data;

    FUNCTION_MARKER(0x800D0838, "ACE3.BIN");
    count = r_u32(owner + 184);
    quotient = (uint32)((sint32)count / 4);
    remaining = (sint32)page < (sint32)quotient ? 4 : count - quotient * 4;
    if ((sint32)remaining <= 0)
        return quotient * 4;
    for (index = 0; (sint32)index < (sint32)remaining; ++index)
    {
        position = page * 4 + index;
        owner = r_u32(globals + 0x2DB8);
        state = r_u32(0x80112DC4);
        id = r_u8(owner + position + 4);
        table = r_u32(state + 468);
        data = r_u32(table + id * 4 - 104);
        resource002_sub_800ed000(data, 960, 340 + index * 18, 384, 6);
        owner = r_u32(globals + 0x2DB8);
        state = r_u32(0x80112DC4);
        id = r_u8(owner + position + 4);
        table = r_u32(state + 472);
        id = r_u8(0x80109E28 + id * 8 + 5);
        data = r_u32(table + id * 4);
        resource002_sub_800ed000(data, 992, 340 + index * 18, 384, 6);
    }
    return 0;
}

uint32 resource002_sub_800d0830(uint32 page)
{
    FUNCTION_MARKER(0x800D0830, "ACE3.BIN");
    return resource002_sub_800d0838(page, 0x80110000, r_u32(0x80112DB8));
}

uint32 resource002_sub_800d0970(uint32 item)
{
    uint32 owner, page, id, state, table, data;

    FUNCTION_MARKER(0x800D0970, "ACE3.BIN");
    owner = r_u32(0x80112DB8);
    page = r_u8(owner + 2);
    id = r_u8(owner + item + page * 4 + 4);
    state = r_u32(0x80112DC4);
    id = r_u8(0x80109E28 + id * 8 + 5);
    table = r_u32(state + 476);
    data = r_u32(table + id * 4);
    return resource002_sub_800ed000(data, 960, 256, 768, 1);
}

uint32 resource002_sub_800d0588(void)
{
    uint32 state, game, count, owner, source, index, pointer, value, menu;

    FUNCTION_MARKER(0x800D0588, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    count = r_u32(state + 624) + 1;
    w_u32(state + 624, count);
    source = game + 3400;
    if ((sint32)count < 2)
        return game;
    w_u32(state + 624, 0);
    owner = arena_alloc(608);
    w_u32(0x80112DD0, owner);
    w_u8(owner, 0);
    owner = r_u32(0x80112DD0);
    w_u8(owner + 1, 0);
    owner = r_u32(0x80112DD0);
    w_u8(owner + 3, 0);
    owner = r_u32(0x80112DD0);
    w_u8(owner + 2, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 1);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 5, 0);
    resource002_sub_800deca8(0, 3);
    resource002_sub_800dedf0(0, 11);
    owner = r_u32(0x80112DD0);
    w_u32(owner + 596, 0);
    for (index = 0; index < 79; ++index)
    {
        owner = r_u32(0x80112DD0);
        w_u8(owner + 516 + index, 0);
        if (r_u8(source + index) == 0)
            continue;
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        w_u8(owner + count + 516, index);
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        w_u32(owner + 596, count + 1);
    }
    owner = r_u32(0x80112DD0);
    resource002_sub_800cf564(r_u8(owner + 2));
    owner = r_u32(0x80112DD0);
    resource002_sub_800cf640(r_u8(owner + 3));
    for (index = 0; index < 3; ++index)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50 + index, 0);
    }
    for (index = 0; index < 4; ++index)
    {
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        if ((sint32)index < (sint32)count)
        {
            value = r_u8(owner + index + 516);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            value = r_u8(source + value);
            w_u8(menu + index + 53, value);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 1);
        }
        else
        {
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 53, 0);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 0);
        }
    }
    return 0;
}

uint32 resource002_sub_800d09e8(void)
{
    uint32 state, game, source, count, owner, index, pointer, menu, value;

    FUNCTION_MARKER(0x800D09E8, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    count = r_u32(state + 624) + 1;
    w_u32(state + 624, count);
    source = game + 3480;
    if ((sint32)count < 2)
        return 0;
    owner = r_u32(0x80112DB8);
    w_u32(state + 624, 0);
    w_u16(state + 454, 0);
    w_u8(owner, 0);
    owner = r_u32(0x80112DB8);
    w_u8(owner + 1, 0);
    owner = r_u32(0x80112DB8);
    w_u8(owner + 3, 0);
    owner = r_u32(0x80112DB8);
    w_u8(owner + 2, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 1);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 5, 1);
    resource002_sub_800deca8(0, 3);
    resource002_sub_800dedf0(0, 11);
    owner = r_u32(0x80112DB8);
    w_u32(owner + 184, 0);
    for (index = 26; index < 175; ++index)
    {
        game = r_u32(0x800BCE60);
        if (r_u8(game + index + 3480) == 0)
            continue;
        owner = r_u32(0x80112DB8);
        count = r_u32(owner + 184);
        w_u8(owner + count + 4, index);
        owner = r_u32(0x80112DB8);
        count = r_u32(owner + 184);
        w_u32(owner + 184, count + 1);
    }
    owner = r_u32(0x80112DB8);
    resource002_sub_800d0830(r_u8(owner + 2));
    owner = r_u32(0x80112DB8);
    resource002_sub_800d0970(r_u8(owner + 3));
    for (index = 0; index < 3; ++index)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50 + index, index == 0 ? 1 : 0);
    }
    for (index = 0; index < 4; ++index)
    {
        owner = r_u32(0x80112DB8);
        count = r_u32(owner + 184);
        if ((sint32)index < (sint32)count)
        {
            value = r_u8(owner + index + 4);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            value = r_u8(source + value);
            w_u8(menu + index + 53, value);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 2);
        }
        else
        {
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 53, 0);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 0);
        }
    }
    resource002_sub_800def94(49, 0);
    w_u32(0x80112DD4, 0);
    return 0x80110000;
}

uint32 resource002_sub_800ce6b0(void)
{
    uint32 game, state, result;

    FUNCTION_MARKER(0x800CE6B0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u32(game + 68) != 0)
        return 0x80110000;
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ce6f4(uint32 context)
{
    uint32 resource, entries, state, value, pointer;

    FUNCTION_MARKER(0x800CE6F4, "ACE3.BIN");
    resource = r_u32(context + 96);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 36, entries);
    value = r_u32(entries);
    pointer = r_u32(state + 36);
    w_u32(state + 184, value);
    resource = r_u32(pointer + 4);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 244, entries);
    w_u16(state + 454, 3);
    return 3;
}

uint32 resource002_sub_800ce6ec(void)
{
    FUNCTION_MARKER(0x800CE6EC, "ACE3.BIN");
    return resource002_sub_800ce6f4(r_u32(0x800BCE50));
}

uint32 resource002_sub_800ce754(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x800CE754, "ACE3.BIN");
    return incoming_v0;
}

uint32 resource002_sub_800ce75c(void)
{
    uint32 context, size, buffer, state, game, result;

    FUNCTION_MARKER(0x800CE75C, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 121);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    context = r_u32(0x800BCE50);
    state = r_u32(0x80112DC4);
    w_u32(state + 264, buffer);
    game = r_u32(0x800BCE60);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800fe6e0(uint32 mission, sint32 group)
{
    uint32 game, flags;

    FUNCTION_MARKER(0x800FE6E0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    if (flags & 16)
    {
        if (group < 10)
            return 1;
        if (group < 18)
            return 0;
        if (group < 26)
            return 2;
        if (group < 31)
            return 3;
        return 4;
    }
    if (mission >= 60)
        mission = 0;
    return r_u32(0x80111784 + mission * 12);
}

uint32 resource002_sub_800fe764(uint32 mission, uint32 group)
{
    uint32 game, flags, target, first, end, index, count;

    FUNCTION_MARKER(0x800FE764, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    if (!(flags & 16))
        return r_u8(0x80111788 + mission * 12);
    if (group >= 5)
        return 0;
    target = r_u32(0x800CD6D4 + group * 4);
    switch (target)
    {
        case 0x800FE7A8:
            first = 1;
            end = 10;
            break;
        case 0x800FE7B4:
            first = 10;
            end = 18;
            break;
        case 0x800FE7C0:
            first = 18;
            end = 26;
            break;
        case 0x800FE7CC:
            first = 26;
            end = 31;
            break;
        case 0x800FE7D8:
            first = 31;
            end = 37;
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 mission count branch %08X\n", target);
#endif
            abort();
            return 0;
    }
    count = 0;
    game = r_u32(0x800BCE60);
    for (index = first; index < end; ++index)
        if (r_u8(game + 2420 + index) == 1)
            ++count;
    return count;
}

uint32 resource002_sub_800ce55c(uint32 mode, uint32 game, uint32 incoming_buffer)
{
    uint32 flags, group, context, value, buffer, state, mission, result;

    FUNCTION_MARKER(0x800CE55C, "ACE3.BIN");
    flags = r_u32(game + 2412);
    if (flags & 16)
    {
        state = r_u32(0x80112DC4);
        group = r_u32(state + 196);
    }
    else
        group = resource002_sub_800fe6e0((uint32)r_s16(game + 908), 0);
    /* Modes other than zero and one retain the original incoming S2 carrier */
    buffer = incoming_buffer;
    if (mode == 0)
    {
        context = r_u32(0x800BCE50);
        value = r_s16(0x80109840 + group * 2);
        w_u32(context + 96, value);
        value = resource_total_size(context + 96, 1);
        buffer = arena_alloc(value);
        state = r_u32(0x80112DC4);
        w_u32(state + 264, buffer);
    }
    if (mode == 1)
    {
        context = r_u32(0x800BCE50);
        value = r_s16(0x80109840 + group * 2);
        state = r_u32(0x80112DC4);
        w_u32(context + 96, value);
        buffer = r_u32(state + 264);
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    value = resource002_sub_800fe764(mission, group);
    state = r_u32(0x80112DC4);
    context = r_u32(0x800BCE50);
    game = r_u32(0x800BCE60);
    w_u32(state + 228, value);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ce554(uint32 mode, uint32 incoming_buffer)
{
    FUNCTION_MARKER(0x800CE554, "ACE3.BIN");
    return resource002_sub_800ce55c(mode, r_u32(0x800BCE60), incoming_buffer);
}

uint32 resource002_sub_800ce7f4(uint32 game)
{
    uint32 context, resource, state, entries, value;

    FUNCTION_MARKER(0x800CE7F4, "ACE3.BIN");
    if (r_u32(game + 68) == 1)
        return 0x800C0000;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    state = r_u32(0x80112DC4);
    w_u32(state + 40, resource);
    entries = sub_80027338(resource, 0);
    resource002_sub_800ecda8(r_u32(entries + 8));
    arena_set_cursor(r_u32(entries + 8));
    value = sub_80027338(r_u32(entries), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 468, value);
    value = sub_80027338(r_u32(entries + 4), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 476, value);
    w_u16(state + 454, 3);
    return 3;
}

uint32 resource002_sub_800ce7ec(void)
{
    FUNCTION_MARKER(0x800CE7EC, "ACE3.BIN");
    return resource002_sub_800ce7f4(r_u32(0x800BCE60));
}

uint32 resource002_sub_800ce894(void)
{
    uint32 context, size, buffer, state, game, result;

    FUNCTION_MARKER(0x800CE894, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 122);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    context = r_u32(0x800BCE50);
    state = r_u32(0x80112DC4);
    w_u32(state + 264, buffer);
    game = r_u32(0x800BCE60);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ce9e4(void)
{
    uint32 context, size, buffer, game, state, result;

    FUNCTION_MARKER(0x800CE9E4, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 123);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ce92c(uint32 game)
{
    uint32 context, resource, state, entries, value;

    FUNCTION_MARKER(0x800CE92C, "ACE3.BIN");
    if (r_u32(game + 68) == 1)
        return 0x800C0000;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    state = r_u32(0x80112DC4);
    w_u32(state + 40, resource);
    entries = sub_80027338(resource, 0);
    resource002_sub_800ecda8(r_u32(entries + 12));
    arena_set_cursor(r_u32(entries + 12));
    value = sub_80027338(r_u32(entries), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 468, value);
    value = sub_80027338(r_u32(entries + 4), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 472, value);
    value = sub_80027338(r_u32(entries + 8), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 476, value);
    w_u16(state + 454, 3);
    return 3;
}

uint32 resource002_sub_800ce924(void)
{
    FUNCTION_MARKER(0x800CE924, "ACE3.BIN");
    return resource002_sub_800ce92c(r_u32(0x800BCE60));
}

uint32 resource002_sub_800cea74(uint32 game)
{
    uint32 context, resource, entries, state, value;

    FUNCTION_MARKER(0x800CEA74, "ACE3.BIN");
    if (r_u32(game + 68) == 1)
        return 0x800C0000;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    state = r_u32(0x80112DC4);
    w_u32(state + 40, resource);
    entries = sub_80027338(resource, 0);
    value = sub_80027338(r_u32(entries), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 468, value);
    value = sub_80027338(r_u32(entries + 4), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 476, value);
    w_u16(state + 454, 3);
    return 3;
}

uint32 resource002_sub_800cea6c(void)
{
    FUNCTION_MARKER(0x800CEA6C, "ACE3.BIN");
    return resource002_sub_800cea74(r_u32(0x800BCE60));
}

uint32 resource002_sub_800ceafc(void)
{
    uint32 buffer, context, size, game, state, result;

    FUNCTION_MARKER(0x800CEAFC, "ACE3.BIN");
    buffer = arena_alloc(324);
    context = r_u32(0x800BCE50);
    w_u32(0x80112DDC, buffer);
    w_u32(context + 96, 115);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ceb9c(uint32 game)
{
    uint32 context, resource, entries, state, value, pointer, result;

    FUNCTION_MARKER(0x800CEB9C, "ACE3.BIN");
    if (r_u32(game + 68) == 1)
        return 1;
    if (r_u8(game + 2399) == 0)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 454, 3);
        return 3;
    }
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 28, entries);
    resource = sub_80027338(r_u32(entries + 4), 0);
    resource002_sub_800de908(resource, 22, 2);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 28);
    pointer = r_u32(state + 28);
    value = r_u32(entries + 8);
    w_u32(state + 152, value);
    value = r_u32(pointer + 16);
    w_u32(state + 160, value);
    value = r_u32(pointer + 20);
    w_u32(state + 164, value);
    result = r_u16(state + 454);
    value = r_u32(pointer + 24);
    ++result;
    w_u16(state + 454, result);
    w_u32(state + 168, value);
    return result;
}

uint32 resource002_sub_800ceb94(void)
{
    FUNCTION_MARKER(0x800CEB94, "ACE3.BIN");
    return resource002_sub_800ceb9c(r_u32(0x800BCE60));
}

uint32 resource002_sub_800cec70(void)
{
    uint32 buffer, context, size, game, state, result;

    FUNCTION_MARKER(0x800CEC70, "ACE3.BIN");
    buffer = arena_alloc(324);
    context = r_u32(0x800BCE50);
    w_u32(0x80112DDC, buffer);
    w_u32(context + 96, 125);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 454) + 1;
    w_u16(state + 454, result);
    return result;
}

uint32 resource002_sub_800ced08(void)
{
    uint32 game, state;

    FUNCTION_MARKER(0x800CED08, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u32(game + 68) == 1)
        return 0x80110000;
    state = r_u32(0x80112DC4);
    w_u16(state + 454, 3);
    return 3;
}

uint32 resource002_sub_800ded38(uint32 group, uint32 index)
{
    uint32 state, table;

    FUNCTION_MARKER(0x800DED38, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + group * 4 + 364);
    return (uint32)r_s16(table + index * 12 + 10);
}

uint32 resource002_sub_800ced38(void)
{
    uint32 state, value, result;

    FUNCTION_MARKER(0x800CED38, "ACE3.BIN");
    if (resource002_sub_800ded38(0, 1) == 0)
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 420);
        w_u32(state + 420, value + 1);
    }
    if (resource002_sub_800dee84(1) != 2)
        return 0x80110000;
    state = r_u32(0x80112DC4);
    result = r_u16(state + 452) + 1;
    w_u16(state + 452, result);
    return result;
}

uint32 sub_8002de68(uint32 sound)
{
    FUNCTION_MARKER(0x8002DE68, "ACE3.BIN");
    return (uint32)r_s16(0x800C5A18 + sound * 20);
}

uint32 resource002_sub_800debb8(uint32 group, uint32 item)
{
    uint32 target, state, first, second, third;

    FUNCTION_MARKER(0x800DEBB8, "ACE3.BIN");
    target = item < 19 ? r_u32(0x800CC884 + item * 4) : 0x800DEC4C;
    switch (target)
    {
        case 0x800DEBE8:
            first = 0;
            second = 55;
            third = 10;
            break;
        case 0x800DEBFC:
            first = (uint32)-329;
            second = 0;
            third = 25;
            break;
        case 0x800DEC10:
            first = 0;
            second = (uint32)-504;
            third = 31;
            break;
        case 0x800DEC24:
            first = 325;
            second = 0;
            third = (uint32)-25;
            break;
        case 0x800DEC38:
            first = 0;
            second = 512;
            third = (uint32)-31;
            break;
        case 0x800DEC4C:
            first = 0;
            second = 0;
            third = 0;
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 item transition branch %08X\n", target);
#endif
            abort();
            return 0;
    }
    resource002_sub_800deb5c(group, item, 0, first, second, third);
    state = r_u32(0x80112DC4);
    w_u32(state + 420, 0);
    return state;
}

uint32 resource002_sub_800decd4(uint32 group, uint32 item)
{
    uint32 state, table, second, third, first;

    FUNCTION_MARKER(0x800DECD4, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + group * 4 + 364) + item * 12;
    second = r_s16(table + 6);
    third = r_u16(table + 8);
    first = r_s16(table + 4);
    third = (uint32)(sint32)(sint16)(0 - third);
    return resource002_sub_800deb5c(group, item, 2, first, second, third);
}

uint32 resource002_sub_800e11f4(uint32 enabled, uint32 mode)
{
    uint32 owner, game, table, slot, shift, saved_slot, saved_shift, result;

    FUNCTION_MARKER(0x800E11F4, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    w_u16(owner + 370, enabled);
    w_u16(owner + 368, mode);
    table = 0x8010ADAC + (uint32)r_s16(game + 908) * 4;
    slot = r_s16(table);
    shift = r_s16(table + 2);
    saved_slot = r_u16(table);
    result = r_u32(owner + slot * 4 + 240);
    saved_shift = r_u16(table + 2);
    result |= (uint32)1 << (shift & 31);
    w_u32(owner + slot * 4 + 240, result);
    w_u16(owner + 372, saved_slot);
    w_u16(owner + 374, saved_shift);
    return result;
}

uint32 resource002_sub_800f3218(uint32 adjustment)
{
    uint32 index, state, voice, value, result;

    FUNCTION_MARKER(0x800F3218, "ACE3.BIN");
    for (index = 0; index < 5; ++index)
    {
        state = r_u32(0x80112DC4);
        voice = r_u32(state + 304 + index * 4);
        sub_8002dda8(voice);
    }
    for (index = 0; index < 5; ++index)
    {
        voice = sub_8002db4c(93 + index);
        state = r_u32(0x80112DC4);
        w_u32(state + 304 + index * 4, voice);
    }
    result = 0;
    for (index = 0; index < 5; ++index)
    {
        value = sub_8002de68(93 + index);
        value = (uint32)(sint32)(sint16)value;
        state = r_u32(0x80112DC4);
        voice = r_u32(state + 304 + index * 4);
        result = sound_set_voice_note(voice, (adjustment + value) & 0xFFFF);
    }
    return result;
}

uint32 resource002_sub_800cf1dc(void)
{
    uint32 state, phase, mode, game, mission, value;

    FUNCTION_MARKER(0x800CF1DC, "ACE3.BIN");
    if (resource002_sub_800dee84(1) != 0)
        return 0x80110000;
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 454);
    if (phase != 3)
        return 3;
    mode = r_u8(state + 126);
    switch (mode)
    {
        case 0:
            w_u8(state + 124, 1);
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            if (r_s16(0x801127AC + mission * 12) == -1)
                break;
            resource002_sub_800debb8(0, 5);
            resource002_sub_800debb8(0, 6);
            resource002_sub_800deb5c(0, 7, 0, 0, 506, 0);
            resource002_sub_800dedf0(0, 225);
            resource002_sub_800dee08(1, 1, 0, 40);
            sub_8002db4c(90);
            break;
        case 1:
            game = r_u32(0x800BCE60);
            value = r_u32(game + 2412);
            if (value & 16)
            {
                w_u8(state + 127, 1);
                game = r_u32(0x800BCE60);
                value = r_u32(game + 2412);
                w_u32(game + 2412, value | 2);
            }
            else
                w_u8(state + 124, 2);
            break;
        case 2:
            w_u8(state + 124, 3);
            break;
        case 3:
            w_u8(state + 124, 4);
            sub_8002db4c(90);
            break;
        default:
            break;
    }
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    return state;
}

uint32 resource002_sub_800cf364(void)
{
    uint32 state, game, mode, mission, item, value;

    FUNCTION_MARKER(0x800CF364, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u16(state + 452, 0);
    w_u32(state + 80, 0);
    w_u8(state + 125, 1);
    resource002_sub_800dee08(1, 1, 0, 40);
    resource002_sub_800dee08(2, 1, 0, 40);
    resource002_sub_800de79c();
    resource002_sub_800e11f4(1, 0);
    resource002_sub_800deca8(0, 2);
    state = r_u32(0x80112DC4);
    mode = r_u8(state + 126);
    if (mode == 4)
    {
        resource002_sub_800debb8(0, 1);
        resource002_sub_800deca8(0, 0);
        resource002_sub_800dee08(15, 1, 0, 25);
        state = r_u32(0x80112DC4);
        w_u8(state + 126, 0);
    }
    resource002_sub_800dedf0(0, 7);
    resource002_sub_800dedf0(2, 0);
    state = r_u32(0x80112DC4);
    mode = r_u8(state + 126);
    resource002_sub_800def94(1, mode + 1);
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    item = r_s16(game + 910);
    value = resource002_sub_800fe6e0(mission, (sint32)item);
    state = r_u32(0x80112DC4);
    w_u32(state + 196, value);
    sub_8002db4c(90);
    return resource002_sub_800f3218(0);
}

uint32 resource002_sub_80100994(void)
{
    uint32 effect;

    FUNCTION_MARKER(0x80100994, "ACE3.BIN");
    effect = r_u32(0x80112DA8);
    w_u32(effect + 24, 3);
    w_u32(effect + 36, 0);
    return 3;
}

uint32 resource002_sub_800e0f88(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
    uint32 state, packet, ordering, owner, pointer, depth, tag, link, result;

    FUNCTION_MARKER(0x800E0F88, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    packet = r_u32(state + 628);
    ordering = r_u32(state + 8);
    w_u32(state + 628, packet + 16);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x40);
    w_u8(packet + 4, 96);
    w_u8(packet + 5, 96);
    w_u8(packet + 6, 96);
    owner = r_u32(0x80112DC0);
    ordering += 32;
    w_u16(packet + 8, x0);
    w_u16(packet + 10, y0);
    w_u16(packet + 12, x1);
    w_u16(packet + 14, y1);
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + depth * 4 + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    link = r_u32(ordering + depth * 4 + 4);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ordering + depth * 4 + 4, result);
    return result;
}

uint32 resource002_sub_800e1040(uint32 x, uint32 y, uint32 width, uint32 height, uint32 transparent)
{
    uint32 state, packet, ordering, owner, pointer, depth, tag, link, result;

    FUNCTION_MARKER(0x800E1040, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    packet = r_u32(state + 628);
    w_u32(state + 628, packet + 16);
    ordering = r_u32(state + 8);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    ordering += 32;
    w_u8(packet + 7, (transparent & 255) == 0 ? 0x60 : 0x62);
    w_u8(packet + 4, 96);
    w_u8(packet + 5, 96);
    w_u8(packet + 6, 96);
    owner = r_u32(0x80112DC0);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    w_u16(packet + 12, width);
    w_u16(packet + 14, height);
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + depth * 4 + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    link = r_u32(ordering + depth * 4 + 4);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ordering + depth * 4 + 4, result);
    return result;
}

uint32 resource002_sub_800deea8(uint32 slot)
{
    uint32 owner, table;

    FUNCTION_MARKER(0x800DEEA8, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    table = r_u32(owner + 80);
    return (uint32)r_s16(table + slot * 2);
}

uint32 resource002_sub_8010097c(void)
{
    uint32 effect;

    FUNCTION_MARKER(0x8010097C, "ACE3.BIN");
    effect = r_u32(0x80112DA8);
    w_u32(effect + 24, 1);
    w_u32(effect + 36, 0);
    return 1;
}

uint32 resource002_sub_800e18b8(void)
{
    uint32 count, mode, index, value;

    FUNCTION_MARKER(0x800E18B8, "ACE3.BIN");
    count = resource002_sub_800deea8(2);
    mode = resource002_sub_800dee84(2);
    if (mode == 2)
    {
        for (index = 0; index < 17; ++index)
            resource002_sub_800e1040(168, 258 + index * 9, 140, 2, 0);
        resource002_sub_800e1040(232, 160, 75, 2, 0);
        for (index = 0; index < 16; ++index)
            resource002_sub_800e0f88(232 + index * 5, 160, 232 + index * 5, 402);
        return resource002_sub_800e0f88(168, 258, 168, 402);
    }
    if (mode != 1 && mode != 3)
        return (sint32)mode < 2 ? 1 : 3;
    for (index = 0; index < 17 && (sint32)count >= (sint32)index; ++index)
    {
        value = (count - index) * 7;
        if ((sint32)value >= 141)
            value = 140;
        resource002_sub_800e1040(168, 258 + index * 9, (uint32)(sint32)(sint16)value, 2, 0);
    }
    value = (uint32)((sint32)(count * 75) / 17);
    if ((sint32)value >= 76)
        value = 75;
    resource002_sub_800e1040(232, 160, (uint32)(sint32)(sint16)value, 2, 0);
    for (index = 0; index < 16 && (sint32)count >= (sint32)index; ++index)
    {
        value = (uint32)((sint32)((count - index) * 242) / 20);
        if ((sint32)value >= 243)
            value = 242;
        resource002_sub_800e0f88(232 + index * 5, 160, 232 + index * 5, (uint32)(sint32)(sint16)(value + 160));
    }
    value = (uint32)((sint32)(count * 144) / 17);
    if ((sint32)value >= 145)
        value = 144;
    return resource002_sub_800e0f88(168, 258, 168, (uint32)(sint32)(sint16)(value + 258));
}

uint32 resource002_sub_800e1260(uint32 owner)
{
    uint32 mode, time, value, pointer;

    FUNCTION_MARKER(0x800E1260, "ACE3.BIN");
    mode = r_s16(owner + 370);
    if (mode == 0 || mode == 2)
        return 0;
    if (mode == 1)
    {
        time = r_s16(owner + 368);
        value = r_u16(owner + 368);
        if ((sint32)time < 120)
            w_u16(owner + 368, value + 1);
        else
        {
            w_u16(owner + 370, 2);
            resource002_sub_8010097c();
        }
    }
    owner = r_u32(0x80112DC0);
    mode = r_s16(owner + 370);
    if (mode == 3)
    {
        time = r_s16(owner + 368);
        value = r_u16(owner + 368);
        if ((sint32)time > 0)
            w_u16(owner + 368, value - 1);
        else
            w_u16(owner + 370, 0);
    }
    owner = r_u32(0x80112DC0);
    time = r_s16(owner + 368);
    pointer = r_u32(owner + 92);
    if ((sint32)time >= 31)
    {
        w_u8(pointer + 2, 1);
        return 1;
    }
    w_u8(pointer + 2, 0);
    return pointer;
}

uint32 resource002_sub_800e1258(void)
{
    FUNCTION_MARKER(0x800E1258, "ACE3.BIN");
    return resource002_sub_800e1260(r_u32(0x80112DC0));
}

uint32 resource002_sub_800e133c(uint32 owner)
{
    uint32 row, column, active, previous, current, next, bit, adjacent, value;

    FUNCTION_MARKER(0x800E133C, "ACE3.BIN");
    if (r_s16(owner + 370) != 1)
        return 0;
    for (row = 0; row < 32; ++row)
    {
        active = r_u32(owner + 240 + row * 4);
        previous = r_u32(owner + 108 + row * 4);
        current = r_u32(owner + 112 + row * 4);
        next = r_u32(owner + 116 + row * 4);
        for (column = 0; column < 32; ++column)
        {
            bit = (uint32)1 << column;
            if (!(active & bit))
                continue;
            if (column > 0)
                current |= bit >> 1;
            if (column < 31)
                current |= bit << 1;
            adjacent = column == 0 ? 3 : column == 31 ? 0xC0000000 : (bit >> 1) | bit | (bit << 1);
            if (row & 1)
            {
                if (row < 31)
                    next |= bit;
                if (row > 0)
                    previous |= adjacent;
            }
            else
            {
                if (row < 31)
                    next |= adjacent;
                if (row > 0)
                    previous |= bit;
            }
        }
        w_u32(owner + 108 + row * 4, previous);
        w_u32(owner + 112 + row * 4, current);
        w_u32(owner + 116 + row * 4, next);
    }
    owner = r_u32(0x80112DC0);
    for (row = 0; row < 32; ++row)
    {
        active = r_u32(owner + 240 + row * 4);
        value = r_u32(owner + 112 + row * 4);
        w_u32(owner + 240 + row * 4, active | value);
        w_u32(owner + 112 + row * 4, 0);
    }
    return 0;
}
