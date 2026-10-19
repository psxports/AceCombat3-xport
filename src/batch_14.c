#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_800791e8(uint32 state)
{
    MATRIX matrix;
    uint32 root, flags, phase, product, pitch, source, radius, value, raw, a, b, c, d, e, x, y, z, dx, dy, dz, vector_z, count, result;
    FUNCTION_MARKER(0x800791E8, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    flags = r_u32(root) & 0x20000000;
    if (flags != 0)
        return flags;
    phase = r_u32(state + 208) << 5;
    if ((sint32)phase > 4096)
        phase = 4096;
    value = rsin((sint32)phase >> 1);
    product = 0 - (value << 9);
    if ((sint32)product < 0)
        product += 4095;
    pitch = (uint32)((sint32)product >> 12) - 3584;
    value = rsin((sint32)phase >> 2);
    source = r_u32(state + 52);
    w_u16(state + 212, pitch);
    raw = r_u16(source + 18);
    radius = 8192 - (value << 1);
    raw += radius;
    w_u16(state + 214, raw);
    if ((sint16)raw < -8192)
        w_u16(state + 214, raw + 16384);
    value = r_s16(state + 214);
    raw = r_u16(state + 214);
    if ((sint32)value >= 8193)
        w_u16(state + 214, raw - 16384);
    a = r_s16(state + 48);
    dx = r_s16(state + 212);
    dy = r_s16(state + 214);
    dz = r_s16(state + 216);
    value = ((0 - a) << 10) - (radius << 9);
    w_u32(state + 228, value);
    count = r_u32(state + 208);
    source = r_u32(state + 52);
    w_u32(state + 208, count + 1);
    a = r_s16(source + 16);
    b = r_s16(source + 18);
    c = r_s16(source + 20);
    value = r_u32(state + 228);
    vector_z = (sint32)(0 - value) >> 9;
    dx -= a;
    dy -= b;
    dz -= c;
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
    xport_gte_write_data(1, vector_z & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    w_u32(state + 56, x << 9);
    w_u32(state + 60, y << 9);
    w_u16(state + 72, dx);
    w_u16(state + 74, dy);
    w_u16(state + 76, dz);
    result = z << 9;
    w_u32(state + 64, result);
    return result;
}

uint32 sub_80027608(uint32 x, uint32 y, uint32 text, const uint8 *native_text)
{
    uint32 cursor, ch, root, count, index, packet, style, glyph, uv;
    FUNCTION_MARKER(0x80027608, "ACE3.BIN");
    cursor = 0;
    ch = native_text != 0 ? native_text[cursor] : r_u8(text + cursor);
    ++cursor;
    root = r_u32(0x800BCB98);
    count = r_u32(root);
    index = r_u32(root + 0xE20);
    packet = root + count * 8 + 4;
    style = r_u32(root + index * 4 + 0xE14);
    y <<= 16;
    while (ch != 0)
    {
        ++count;
        glyph = ch - 32;
        if ((sint32)count >= 451)
        {
            --count;
            break;
        }
        w_u32(packet, (x & 0xFFFF) | y);
        x += 6;
        uv = ((glyph & 15) * 6) | ((((uint32)((sint32)glyph >> 4) * 6) + 64) << 8);
        w_u32(packet + 4, uv | style);
        packet += 8;
        ch = native_text != 0 ? native_text[cursor] : r_u8(text + cursor);
        ++cursor;
    }
    root = r_u32(0x800BCB98);
    w_u32(root, count);
    return x;
}

uint32 sub_800277f0(uint32 x, uint32 y, uint32 value, uint32 width)
{
    uint8 digits[16], buffer[10];
    uint32 a, b, c, d, i, cursor;
    FUNCTION_MARKER(0x800277F0, "ACE3.BIN");
    a = r_u32(0x80010124);
    b = r_u32(0x80010128);
    c = r_u32(0x8001012C);
    xport_store_le32(digits, a);
    xport_store_le32(digits + 4, b);
    xport_store_le32(digits + 8, c);
    d = r_u32(0x80010130);
    xport_store_le32(digits + 12, d);
    if ((sint32)width >= 10)
        return 0xFFFFFFFF;
    cursor = 9;
    buffer[cursor] = 0;
    if ((sint32)width > 0)
    {
        for (i = 0; i < width; ++i)
        {
            --cursor;
            buffer[cursor] = digits[value & 15];
            value = (sint32)value >> 4;
        }
    }
    return sub_80027608(x, y, 0, buffer + cursor);
}

uint32 sub_800276c8(uint32 x, uint32 y, uint32 value, uint32 format)
{
    uint8 buffer[11];
    uint32 width, flags, magnitude, cursor, i, quotient, remainder;
    FUNCTION_MARKER(0x800276C8, "ACE3.BIN");
    width = format & 255;
    flags = format & 0xF00;
    if (width >= 10)
        return 0xFFFFFFFF;
    cursor = 10;
    magnitude = (sint32)value < 0 ? 0 - value : value;
    buffer[10] = 0;
    buffer[9] = 48;
    for (i = 0; i < width; ++i)
    {
        --cursor;
        if (magnitude != 0)
        {
            quotient = (sint32)magnitude / 10;
            remainder = magnitude - quotient * 10;
            buffer[cursor] = remainder + 48;
            magnitude = quotient;
        }
        else if (i != 0)
        {
            if ((flags & 0x400) != 0)
            {
                ++cursor;
                break;
            }
            buffer[cursor] = (flags & 0x100) != 0 ? 48 : 32;
        }
    }
    if ((flags & 0x200) != 0)
    {
        --cursor;
        buffer[cursor] = (sint32)value < 0 ? 45 : 43;
    }
    return sub_80027608(x, y, 0, buffer + cursor);
}

uint32 sub_8007c00c(uint32 ot, uint32 packet)
{
    uint32 root, mask, records, scene, camera, limit_x, limit_y, cx, cy, cz, h, close_limit, scale_limit, group, data, a, b, c, x, y, z, ax, ay, az, distance, scale, index, slot, source, count, close_bits, visible, clipped, last_bit, bit, scratch, value, color, rate, depth, xy, px, py, magnitude, expanded_close, expanded_visible, shift, neighbor, other, numerator, divisor, ratio, j, offset, sx, sy, sz, nx, ny, nz, old_xy, old_edges[3], old_depth, old_color, new_xy, new_edges[3], new_depth, new_color, pair, ordering, q;
    FUNCTION_MARKER(0x8007C00C, "ACE3.BIN");
    root = r_u32(0x800BD0B0);
    mask = r_u32(root);
    records = root + 4;
    if (mask == 0)
        return packet;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    a = r_u16(camera + 32);
    b = r_u16(camera + 34);
    limit_x = (uint32)((sint32)(a << 16) >> 17) + 56;
    limit_y = (uint32)((sint32)(b << 16) >> 17) + 56;
    cx = r_u32(camera + 4);
    cy = r_u32(camera + 8);
    cz = r_u32(camera + 12);
    h = xport_gte_read_control(26) << 2;
    close_limit = h << 2;
    scale_limit = h * 3;
    xport_gte_write_data(6, 0);
    for (group = 0; group < 32; ++group, records += 0x908)
    {
        scene = r_u32(0x800BCE54);
        data = r_u32(scene + 0x268);
        a = r_u8(data + 32);
        b = r_u8(data + 33);
        c = r_u8(data + 34);
        xport_gte_write_control(21, a << 4);
        xport_gte_write_control(22, b << 4);
        xport_gte_write_control(23, c << 4);
        if ((mask & (1u << group)) == 0)
            continue;
        count = 0;
        clipped = 0;
        close_bits = 0;
        last_bit = 0;
        visible = 0;
        index = r_u32(records + 4);
        value = r_u32(records);
        scratch = 0x1F800000;
        while ((sint32)index < (sint32)value)
        {
            slot = index & 31;
            source = records + 0x108 + slot * 64;
            a = r_u32(source);
            b = r_u32(source + 4);
            x = (sint32)(a - cx) >> 10;
            y = (sint32)(b - cy) >> 10;
            c = r_u32(source + 8);
            z = (sint32)(c - cz) >> 10;
            ax = (sint32)x < 0 ? 0 - x : x;
            ay = (sint32)y < 0 ? 0 - y : y;
            az = (sint32)z < 0 ? 0 - z : z;
            distance = (sint32)(ax + ay + az) >> 2;
            if ((sint32)distance <= 0xB000)
            {
                scale = 2;
                if ((sint32)scale_limit >= (sint32)distance)
                    scale = ((sint32)scale_limit >> 1) < (sint32)distance;
                x = (sint32)x >> scale;
                y = (sint32)y >> scale;
                z = (sint32)z >> scale;
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                w_u16(scratch + 26, scale);
                w_u32(scratch, source);
                bit = 1u << (count & 31);
                if ((sint32)distance < (sint32)close_limit)
                    close_bits |= bit;
                value = r_u32(records);
                if (index == value - 1)
                    last_bit = bit;
                a = r_u32(records + slot * 4 + 8);
                b = r_u32(records + slot * 4 + 0x88);
                rate = b << 5;
                if ((sint32)(a << 4) < (sint32)b)
                    rate = a << 7;
                depth = xport_gte_read_data(11);
                xport_gte_write_data(8, rate);
                value = xport_gte_read_control(31);
                xport_gte_execute(0x780010);
                if ((value & 0x20000) != 0)
                {
                    clipped |= bit;
                    bit = 0;
                }
                xy = xport_gte_read_data(14);
                px = (uint32)(sint32)(sint16)xy;
                py = (sint32)xy >> 16;
                depth = (sint32)depth >> (2 - scale);
                magnitude = (sint32)px < 0 ? 0 - px : px;
                if ((sint32)limit_x < (sint32)magnitude)
                    bit = 0;
                else
                {
                    magnitude = (sint32)py < 0 ? 0 - py : py;
                    if ((sint32)limit_y < (sint32)magnitude)
                        bit = 0;
                }
                visible |= bit;
                value = xport_gte_read_data(14);
                w_u32(scratch + 8, value);
                w_u16(scratch + 24, depth);
                color = xport_gte_read_data(22);
                ++count;
                w_u32(scratch + 4, color);
                scratch += 28;
            }
            value = r_u32(records);
            ++index;
        }
        expanded_close = close_bits | ((((uint32)((sint32)close_bits >> 1)) | (close_bits << 1)) & ~visible);
        expanded_visible = visible | ((((uint32)((sint32)visible >> 1)) | (visible << 1)) & ~visible);
        for (index = 0, scratch = 0x1F800000; (sint32)index < (sint32)count; ++index, scratch += 28)
        {
            bit = 1u << (index & 31);
            depth = r_s16(scratch + 24);
            if ((expanded_visible & bit) == 0)
                continue;
            if ((expanded_close & bit) == 0)
            {
                if ((sint32)depth < 0)
                    w_u16(scratch + 24, 0);
                continue;
            }
            shift = r_s16(scratch + 26);
            source = r_u32(scratch);
            shift = (shift + 10) & 31;
            if ((clipped & bit) != 0)
            {
                neighbor = (sint32)index < (sint32)(count - 1) ? scratch + 28 : scratch - 28;
                value = xport_gte_read_control(26);
                numerator = (depth - (uint32)((sint32)value >> 3)) << 12;
                other = r_s16(neighbor + 24);
                divisor = depth - other;
                if (divisor == 0)
                    xport_mips_break(7);
                if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                    xport_mips_break(6);
                ratio = (sint32)numerator / (sint32)divisor;
                xport_gte_write_data(8, ratio);
                a = r_u32(source);
                b = r_u32(source + 4);
                neighbor = r_u32(neighbor);
                sx = (sint32)(a - cx) >> shift;
                sy = (sint32)(b - cy) >> shift;
                c = r_u32(source + 8);
                a = r_u32(neighbor);
                b = r_u32(neighbor + 4);
                sz = (sint32)(c - cz) >> shift;
                nx = (sint32)(a - cx) >> shift;
                ny = (sint32)(b - cy) >> shift;
                c = r_u32(neighbor + 8);
                nz = (sint32)(c - cz) >> shift;
                xport_gte_write_data(9, sx);
                xport_gte_write_data(10, sy);
                xport_gte_write_data(11, sz);
                xport_gte_write_control(21, nx);
                xport_gte_write_control(22, ny);
                xport_gte_write_control(23, nz);
                xport_gte_execute(0x980011);
                x = xport_gte_read_data(9);
                y = xport_gte_read_data(10);
                z = xport_gte_read_data(11);
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                value = xport_gte_read_data(14);
                w_u32(scratch + 8, value);
                for (j = 1; j < 4; ++j)
                {
                    offset = j * 16;
                    a = r_u32(neighbor + offset);
                    b = r_u32(neighbor + offset + 4);
                    sy = r_u32(source + offset + 4);
                    c = r_u32(neighbor + offset + 8);
                    sx = r_u32(source + offset);
                    sz = r_u32(source + offset + 8);
                    nx = (sint32)(a - cx) >> shift;
                    ny = (sint32)(b - cy) >> shift;
                    nz = (sint32)(c - cz) >> shift;
                    sx = (sint32)(sx - cx) >> shift;
                    sy = (sint32)(sy - cy) >> shift;
                    sz = (sint32)(sz - cz) >> shift;
                    if (j == 1)
                        xport_gte_write_data(8, ratio);
                    xport_gte_write_data(9, sx);
                    xport_gte_write_data(10, sy);
                    xport_gte_write_data(11, sz);
                    xport_gte_write_control(21, nx);
                    xport_gte_write_control(22, ny);
                    xport_gte_write_control(23, nz);
                    xport_gte_execute(0x980011);
                    x = xport_gte_read_data(9);
                    y = xport_gte_read_data(10);
                    z = xport_gte_read_data(11);
                    if (j < 3)
                    {
                        xport_gte_write_data((j - 1) * 2, (x & 0xFFFF) | (y << 16));
                        xport_gte_write_data((j - 1) * 2 + 1, z);
                    }
                }
                w_u32(scratch + 4, 0);
            }
            else
            {
                for (j = 1; j < 4; ++j)
                {
                    offset = j * 16;
                    a = r_u32(source + offset);
                    b = r_u32(source + offset + 4);
                    x = (sint32)(a - cx) >> shift;
                    y = (sint32)(b - cy) >> shift;
                    c = r_u32(source + offset + 8);
                    z = (sint32)(c - cz) >> shift;
                    if (j < 3)
                    {
                        xport_gte_write_data((j - 1) * 2, (x & 0xFFFF) | (y << 16));
                        xport_gte_write_data((j - 1) * 2 + 1, z);
                    }
                }
            }
            xport_gte_write_data(4, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(5, z);
            xport_gte_execute(0x280030);
            if ((sint32)depth < 0)
                w_u16(scratch + 24, 0);
            if ((last_bit & bit) != 0)
            {
                value = r_u32(scratch + 8);
                w_u32(scratch + 12, value);
                w_u32(scratch + 16, value);
                w_u32(scratch + 20, value);
                xport_gte_read_data(27);
            }
            else
            {
                value = xport_gte_read_data(12);
                w_u32(scratch + 12, value);
                value = xport_gte_read_data(13);
                w_u32(scratch + 16, value);
                value = xport_gte_read_data(14);
                w_u32(scratch + 20, value);
            }
        }
        old_xy = r_u32(0x1F800008);
        old_edges[0] = r_u32(0x1F80000C);
        old_edges[1] = r_u32(0x1F800010);
        old_edges[2] = r_u32(0x1F800014);
        old_color = r_u32(0x1F800004);
        old_depth = r_s16(0x1F800018);
        for (index = 1, scratch = 0x1F80001C; (sint32)index < (sint32)count; ++index, scratch += 28)
        {
            pair = 3u << ((index - 1) & 31);
            new_xy = r_u32(scratch + 8);
            new_edges[0] = r_u32(scratch + 12);
            new_edges[1] = r_u32(scratch + 16);
            new_edges[2] = r_u32(scratch + 20);
            new_depth = r_s16(scratch + 24);
            new_color = r_u32(scratch + 4);
            if (((~expanded_visible) & pair) == 0 && (sint32)old_depth < 0x5800)
            {
                ordering = ot + ((uint32)((sint32)old_depth >> 3) << 2);
                if (((~expanded_close) & pair) != 0)
                {
                    w_u32(packet + 4, old_color | 0x52000000);
                    w_u32(packet + 8, old_xy);
                    w_u32(packet + 12, new_color);
                    w_u32(packet + 16, new_xy);
                    value = r_u32(ordering);
                    w_u32(packet, value | 0x04000000);
                    w_u32(ordering, packet & 0xFFFFFF);
                    packet += 20;
                }
                else
                {
                    color = old_color | 0x3A000000;
                    for (q = 0; q < 3; ++q)
                    {
                        w_u32(packet + 4, color);
                        w_u32(packet + 8, old_xy);
                        w_u32(packet + 12, 0);
                        w_u32(packet + 16, old_edges[q]);
                        w_u32(packet + 20, new_color);
                        w_u32(packet + 24, new_xy);
                        w_u32(packet + 28, 0);
                        w_u32(packet + 32, new_edges[q]);
                        value = r_u32(ordering);
                        w_u32(packet, value | 0x08000000);
                        w_u32(ordering, packet & 0xFFFFFF);
                        packet += 36;
                    }
                }
            }
            old_xy = new_xy;
            old_edges[0] = new_edges[0];
            old_edges[1] = new_edges[1];
            old_edges[2] = new_edges[2];
            old_depth = new_depth;
            old_color = new_color;
        }
    }
    return packet;
}

uint32 sub_8007cfa4(uint32 ot, uint32 packet)
{
    uint32 root, records, mask, scene, camera, cx, cy, cz, limit_x, limit_y, h, near_limit, range, group, index, slot, count, scratch, source, a, b, c, d, e, f, x, y, z, ax, ay, az, distance, scale, close_bits, visible, bit, brightness, depth, xy, px, py, value, expanded_close, expanded_visible, shift, j, old_x, old_y, old_x2, old_y2, old_depth, old_color, new_x, new_y, new_x2, new_y2, new_depth, new_color, pair, ordering, base, anchor, tag, color, side, step, source_x, source_y, dest_x, dest_y, offset;
    FUNCTION_MARKER(0x8007CFA4, "ACE3.BIN");
    root = r_u32(0x800BD0B0);
    mask = r_u32(root + 0x12124);
    records = root + 0x12128;
    if (mask == 0)
        return packet;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    cx = r_u32(camera + 4);
    cy = r_u32(camera + 8);
    a = r_u16(camera + 32);
    cz = r_u32(camera + 12);
    b = r_u16(camera + 34);
    limit_x = (sint32)(a << 16) >> 17;
    h = r_s16(camera + 36);
    limit_y = (sint32)(b << 16) >> 17;
    near_limit = h << 3;
    range = (h << 2) + near_limit;
    for (group = 0; group < 32; ++group, records += 0x508)
    {
        if ((mask & (1u << group)) == 0)
            continue;
        count = 0;
        close_bits = 0xFFFFFFFF;
        visible = 0xFFFFFFFF;
        index = r_u32(records + 4);
        value = r_u32(records);
        scratch = 0x1F800000;
        while ((sint32)index < (sint32)value)
        {
            slot = index & 31;
            source = records + 0x108 + slot * 32;
            a = r_u32(source);
            d = r_u32(source + 16);
            e = r_u32(source + 20);
            b = r_u32(source + 4);
            c = r_u32(source + 8);
            f = r_u32(source + 24);
            x = (sint32)((uint32)((sint32)(a + d) >> 1) - cx) >> 10;
            y = (sint32)((uint32)((sint32)(b + e) >> 1) - cy) >> 10;
            z = (sint32)((uint32)((sint32)(c + f) >> 1) - cz) >> 10;
            ax = (sint32)x < 0 ? 0 - x : x;
            ay = (sint32)y < 0 ? 0 - y : y;
            az = (sint32)z < 0 ? 0 - z : z;
            distance = (sint32)(ax + ay + az) >> 2;
            if ((sint32)distance <= 0xB000)
            {
                scale = 2;
                if ((sint32)range >= (sint32)distance)
                    scale = ((sint32)range >> 1) < (sint32)distance;
                x = (sint32)x >> scale;
                y = (sint32)y >> scale;
                z = (sint32)z >> scale;
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                w_u32(scratch + 20, scale);
                w_u32(scratch, source);
                bit = 1u << (count & 31);
                brightness = r_u32(records + 0x88 + slot * 4);
                if ((sint32)near_limit < (sint32)distance)
                {
                    close_bits &= ~bit;
                    brightness = (sint32)brightness >> 1;
                }
                if ((sint32)brightness >= 33)
                    brightness = 32;
                depth = xport_gte_read_data(19);
                xy = xport_gte_read_data(14);
                px = (uint32)(sint32)(sint16)xy;
                py = (sint32)xy >> 16;
                depth = (sint32)depth >> (2 - scale);
                if ((sint32)depth < ((sint32)range >> 3))
                    brightness = (sint32)brightness >> 1;
                if ((sint32)depth < ((sint32)range >> 4))
                    brightness = (sint32)brightness >> 1;
                if ((sint32)depth < ((sint32)range >> 5))
                    brightness = 0;
                w_u32(scratch + 16, brightness | (brightness << 8) | (brightness << 16));
                ax = (sint32)px < 0 ? 0 - px : px;
                if ((sint32)limit_x < (sint32)ax)
                    visible &= ~bit;
                ay = (sint32)py < 0 ? 0 - py : py;
                if ((sint32)limit_y < (sint32)ay)
                    visible &= ~bit;
                az = (sint32)depth < 0 ? 0 - depth : depth;
                if ((sint32)az <= 0)
                    visible &= ~bit;
                w_u32(scratch + 12, depth);
                ++count;
                scratch += 24;
            }
            value = r_u32(records);
            ++index;
        }
        expanded_close = close_bits | (uint32)((sint32)close_bits >> 1) | (close_bits << 1);
        expanded_visible = visible | (uint32)((sint32)visible >> 1) | (visible << 1);
        for (index = 0, scratch = 0x1F800000; (sint32)index < (sint32)count; ++index, scratch += 24)
        {
            if ((expanded_visible & (1u << (index & 31))) == 0)
                continue;
            source = r_u32(scratch);
            shift = (r_u32(scratch + 20) + 10) & 31;
            for (j = 0; j < 2; ++j)
            {
                offset = j * 16;
                a = r_u32(source + offset);
                b = r_u32(source + offset + 4);
                x = (sint32)(a - cx) >> shift;
                y = (sint32)(b - cy) >> shift;
                c = r_u32(source + offset + 8);
                z = (sint32)(c - cz) >> shift;
                xport_gte_write_data(j * 2, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(j * 2 + 1, z);
            }
            xport_gte_execute(0x280030);
            value = xport_gte_read_data(12);
            w_u32(scratch + 4, value);
            value = xport_gte_read_data(13);
            w_u32(scratch + 8, value);
        }
        old_depth = r_u32(0x1F80000C);
        old_color = r_u32(0x1F800010);
        a = r_u16(0x1F800004);
        b = r_u16(0x1F800006);
        c = r_u16(0x1F800008);
        d = r_u16(0x1F80000A);
        old_x = (sint16)a;
        old_y = (sint16)b;
        old_x2 = (sint16)c;
        old_y2 = (sint16)d;
        for (index = 1, scratch = 0x1F800018; (sint32)index < (sint32)count; ++index, scratch += 24)
        {
            pair = 3u << ((index - 1) & 31);
            new_x = r_s16(scratch + 4);
            new_y = r_s16(scratch + 6);
            new_x2 = r_s16(scratch + 8);
            new_depth = r_u32(scratch + 12);
            new_y2 = r_s16(scratch + 10);
            new_color = r_u32(scratch + 16);
            if (((~expanded_visible) & pair) == 0 && (sint32)old_depth < 0x5800)
            {
                ordering = ot + ((uint32)((sint32)old_depth >> 3) << 2);
                base = packet;
                if (((~expanded_close) & pair) != 0)
                {
                    anchor = base + 4;
                    w_u16(anchor + 4, old_x);
                    w_u16(anchor + 6, old_y);
                    w_u16(anchor + 12, new_x);
                    w_u16(anchor + 14, new_y);
                    w_u16(anchor + 24, old_x2);
                    w_u16(anchor + 26, old_y2);
                    w_u16(anchor + 34, new_y2);
                    w_u16(anchor + 32, new_x2);
                    packet += 40;
                    color = old_color | 0x52000000;
                    w_u32(anchor, color);
                    w_u32(anchor + 8, new_color);
                    w_u32(anchor + 20, color);
                    w_u32(anchor + 28, new_color);
                    value = r_u32(ordering);
                    w_u32(anchor - 4, value | 0x04000000);
                    tag = base & 0xFFFFFF;
                    w_u32(ordering, tag);
                    w_u32(anchor + 16, tag | 0x04000000);
                    w_u32(ordering, (base + 20) & 0xFFFFFF);
                }
                else
                {
                    anchor = base + 108;
                    packet += 144;
                    for (side = 0; side < 2; ++side)
                    {
                        source_x = side == 0 ? old_x : new_x;
                        source_y = side == 0 ? old_y : new_y;
                        dest_x = side == 0 ? old_x2 : new_x2;
                        dest_y = side == 0 ? old_y2 : new_y2;
                        xport_gte_write_control(21, dest_x);
                        xport_gte_write_control(22, dest_y);
                        for (j = 0; j < 4; ++j)
                        {
                            step = j == 0 ? (uint32)-192 : j == 1 ? 192 : j == 2 ? 3904 : 4288;
                            xport_gte_write_data(8, step);
                            xport_gte_write_data(9, source_x);
                            xport_gte_write_data(10, source_y);
                            xport_gte_execute(0x980011);
                            a = xport_gte_read_data(9);
                            b = xport_gte_read_data(10);
                            if (side == 0)
                                offset = j == 0 ? -100 : j == 1 ? -56 : j == 2 ? -28 : 16;
                            else
                                offset = j == 0 ? -84 : j == 1 ? -40 : j == 2 ? -12 : 32;
                            w_u16(anchor + offset, a);
                            w_u16(anchor + offset + 2, b);
                            if (j == 0)
                            {
                                offset = side == 0 ? -92 : -76;
                                w_u16(anchor + offset, source_x);
                                w_u16(anchor + offset + 2, source_y);
                                offset = side == 0 ? -64 : -48;
                                w_u16(anchor + offset, source_x);
                                w_u16(anchor + offset + 2, source_y);
                            }
                            else if (j == 2)
                            {
                                if (side == 0)
                                {
                                    w_u16(anchor - 20, dest_x);
                                    w_u16(anchor - 18, dest_y);
                                    w_u16(anchor + 8, dest_x);
                                    w_u16(anchor + 10, dest_y);
                                }
                                else
                                {
                                    w_u16(anchor - 2, dest_y);
                                    w_u16(anchor - 4, dest_x);
                                    w_u16(anchor + 26, dest_y);
                                    w_u16(anchor + 24, dest_x);
                                }
                            }
                        }
                    }
                    w_u32(anchor - 104, 0x3A000000);
                    w_u32(anchor - 88, 0);
                    w_u32(anchor - 80, new_color);
                    w_u32(anchor - 60, 0);
                    w_u32(anchor - 52, new_color);
                    w_u32(anchor - 44, 0);
                    w_u32(anchor - 32, 0x3A000000);
                    color = old_color | 0x3A000000;
                    w_u32(anchor - 96, old_color);
                    w_u32(anchor - 68, color);
                    w_u32(anchor - 24, old_color);
                    w_u32(anchor - 16, 0);
                    w_u32(anchor - 8, new_color);
                    w_u32(anchor + 4, color);
                    w_u32(anchor + 12, 0);
                    w_u32(anchor + 20, new_color);
                    w_u32(anchor + 28, 0);
                    value = r_u32(ordering);
                    w_u32(anchor - 108, value | 0x08000000);
                    tag = base & 0xFFFFFF;
                    w_u32(ordering, tag);
                    w_u32(anchor - 72, tag | 0x08000000);
                    tag = (base + 36) & 0xFFFFFF;
                    w_u32(ordering, tag);
                    w_u32(anchor - 36, tag | 0x08000000);
                    tag = (base + 72) & 0xFFFFFF;
                    w_u32(ordering, tag);
                    w_u32(anchor, tag | 0x08000000);
                    w_u32(ordering, anchor & 0xFFFFFF);
                }
            }
            old_x = new_x;
            old_y = new_y;
            old_y2 = new_y2;
            old_x2 = new_x2;
            old_depth = new_depth;
            old_color = new_color;
        }
    }
    return packet;
}

uint32 sub_80030f64(uint32 angles, uint32 delta, const uint16 *native_delta)
{
    static const uint32 thresholds[6] = {0xF91, 0xF89, 0xFC9, 0xFC1, 0xFE1, 0xFDB};
    static const uint32 limits[6] = {8, 8, 12, 12, 16, 16};
    MATRIX matrix;
    uint32 forward[4], side[4], a, b, c, d, e, index, axis, magnitude, value, pitch, yaw, roll, result;
    uint16 local_angles[3];
    FUNCTION_MARKER(0x80030F64, "ACE3.BIN");
    forward[0] = r_u32(0x800234D8);
    forward[1] = r_u32(0x800234DC);
    forward[2] = r_u32(0x800234E0);
    forward[3] = r_u32(0x800234E4);
    side[0] = r_u32(0x800234E8);
    side[1] = r_u32(0x800234EC);
    side[2] = r_u32(0x800234F0);
    side[3] = r_u32(0x800234F4);
    local_angles[0] = native_delta ? native_delta[0] : r_u16(delta);
    local_angles[1] = native_delta ? native_delta[1] : r_u16(delta + 2);
    local_angles[2] = native_delta ? native_delta[2] : r_u16(delta + 4);
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
    e = xport_load_le16((uint8 *)&matrix + 16);
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
    e = xport_load_le16((uint8 *)&matrix + 16);
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
    e = xport_load_le16((uint8 *)&matrix + 16);
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
    w_u16(angles, pitch);
    w_u16(angles + 2, yaw);
    w_u16(angles + 4, roll);
    return result;
}

uint32 sub_8005e31c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint8 descriptor[44];
    uint32 source, world, a, b, value, h, timer, mode, red, green, blue, dim_red, dim_green, dim_blue, game, table, scene, object, index, dst;
    FUNCTION_MARKER(0x8005E31C, "ACE3.BIN");
    xport_store_le32(descriptor, 127);
    source = (native_state ? native_state->words[1] : r_u32(state + 4));
    world = r_u32(0x800BCEB0);
    a = r_u32(source + 4);
    b = r_u32(world + 4);
    xport_store_le32(descriptor + 4, a + b + 0x42000);
    value = (native_state ? native_state->words[5] : r_u32(state + 20));
    xport_store_le32(descriptor + 8, value);
    value = (native_state ? native_state->words[6] : r_u32(state + 24));
    xport_store_le32(descriptor + 12, value);
    value = (native_state ? native_state->words[7] : r_u32(state + 28));
    xport_store_le32(descriptor + 16, value);
    value = (native_state ? native_state->words[9] : r_u32(state + 36));
    xport_store_le32(descriptor + 20, value);
    h = r_u16(0x800C6D64);
    value = (native_state ? native_state->words[10] : r_u32(state + 40));
    timer = r_u32(0x800C6D60);
    mode = r_u8(0x800C6D71);
    red = r_u8(0x800C6D68);
    green = r_u8(0x800C6D69);
    blue = r_u8(0x800C6D6A);
    dim_red = r_u8(0x800C6D6B);
    dim_green = r_u8(0x800C6D6C);
    dim_blue = r_u8(0x800C6D6D);
    xport_store_le16(descriptor + 38, h);
    xport_store_le32(descriptor + 40, timer);
    xport_store_le16(descriptor + 36, mode);
    descriptor[28] = red;
    descriptor[29] = green;
    descriptor[30] = blue;
    descriptor[32] = dim_red;
    descriptor[33] = dim_green;
    xport_store_le32(descriptor + 24, value);
    descriptor[34] = dim_blue;
    game = r_u32(0x800BCE60);
    table = r_u32(0x800BD060);
    scene = r_u32(0x800BCE54);
    index = r_u32(game + 4);
    object = r_u32(scene + 16);
    dst = r_u32(table + index * 4);
    return sub_8005e178(0, object, dst, descriptor);
}

uint32 sub_8003342c(void)
{
    uint8 scratch[520];
    uint32 scene, ordering, packet, result;
    FUNCTION_MARKER(0x8003342C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    result = sub_80068334(scene + 580, ordering + 4, packet, 0, scratch);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    return result;
}

uint32 sub_8009754c(uint32 state, uint32 ordering, uint32 packet, uint8 *native_state)
{
    uint32 result;
    FUNCTION_MARKER(0x8009754C, "ACE3.BIN");
    result = sub_80096a48(state, ordering, packet, native_state);
    return sub_800962b8(ordering, result);
}

uint32 sub_80034930(uint32 context, uint32 packet)
{
    uint8 rectangles[16];
    uint32 scene, selection, a, b, x, y, color, mode, offset, root, index, dimensions, row, value;
    FUNCTION_MARKER(0x80034930, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    a = r_u32(0x800BCBC0);
    b = r_u32(0x800BCBC4);
    xport_store_le32(rectangles, a);
    xport_store_le32(rectangles + 4, b);
    a = r_u32(0x800BCBC8);
    b = r_u32(0x800BCBCC);
    xport_store_le32(rectangles + 8, a);
    xport_store_le32(rectangles + 12, b);
    value = r_s16(scene + 668);
    selection = scene + 668;
    color = value == 0 ? 96 : 64;
    mode = value == 0 ? 53 : 85;
    x = (sint16)xport_load_le16(rectangles + 8);
    y = (sint16)xport_load_le16(rectangles + 10);
    offset = r_u32(0x800B0AE0);
    packet = sub_80034360(x + 6, y + offset + 5, context, packet, 0x800B0A60, color);
    packet = sub_800346c0(context, packet, mode);
    value = r_s16(selection);
    color = value == 1 ? 96 : 64;
    mode = value == 1 ? 53 : 85;
    offset = r_u32(0x800B0AE4);
    packet = sub_80034360(x + 6, y + offset + 5, context, packet, 0x800B0A98, color);
    offset = r_u32(0x800B0AE4);
    packet = sub_80034360(x + 51, y + offset + 5, context, packet, 0x800B0A90, color);
    offset = r_u32(0x800B0AE4);
    root = r_u32(0x800BCE60);
    index = r_u8(root + 925);
    dimensions = 0x800B0A70 + index * 8;
    packet = sub_80034360(x + 113, y + offset + 5, context, packet, dimensions, color);
    packet = sub_800346c0(context, packet, mode);
    value = r_s16(selection);
    color = value == 2 ? 96 : 64;
    mode = value == 2 ? 53 : 85;
    offset = r_u32(0x800B0AE8);
    packet = sub_80034360(x + 6, y + offset + 5, context, packet, 0x800B0A68, color);
    offset = r_u32(0x800B0AE8);
    root = r_u32(0x800BCE60);
    index = r_u8(root + 926);
    dimensions = index != 0 ? 0x800B0A70 : 0x800B0A78;
    packet = sub_80034360(x + 114, y + offset + 5, context, packet, dimensions, color);
    packet = sub_800346c0(context, packet, mode);
    for (row = 3; row < 5; ++row)
    {
        value = r_s16(selection);
        color = value == row ? 96 : 64;
        mode = value == row ? 53 : 85;
        offset = r_u32(0x800B0AE0 + row * 4);
        dimensions = row == 3 ? 0x800B0AA0 : 0x800B0A80;
        packet = sub_80034360(x + 6, y + offset + 5, context, packet, dimensions, color);
        if (row == 3)
        {
            offset = r_u32(0x800B0AEC);
            packet = sub_80034360(x + 25, y + offset + 5, context, packet, 0x800B0AA8, color);
        }
        offset = r_u32(0x800B0AE0 + row * 4);
        packet = sub_80034360(x + 80, y + offset + 5, context, packet, 0x800B0A88, color);
        packet = sub_800346c0(context, packet, mode);
    }
    index = r_s16(selection);
    value = xport_load_le16(rectangles + 10);
    offset = r_u16(0x800B0AE0 + (index << 2));
    xport_store_le16(rectangles + 10, value + offset);
    packet = sub_800343c8(context, packet, 0, 1, rectangles + 8);
    packet = sub_800346c0(context, packet, 85);
    packet = sub_800343c8(context, packet, 0, 2, rectangles);
    packet = sub_80034360(125, 74, context, packet, 0x800B0A58, 192);
    return sub_800346c0(context, packet, 21);
}

uint32 sub_80069d18(uint32 state)
{
    uint32 phase, value, angle;
    FUNCTION_MARKER(0x80069D18, "ACE3.BIN");
    phase = r_s16(state + 8);
    value = r_u16(state + 8) + 1;
    if (phase != 0)
    {
        if (phase != 1)
            return 1;
    }
    else
        w_u16(state + 8, value);
    value = r_u32(state + 60);
    angle = r_s16(state + 44);
    w_u32(state + 60, value + 669);
    value = r_u16(state + 44) - 32;
    if ((sint32)angle >= -2047)
        w_u16(state + 44, value);
    return value;
}

uint32 sub_80069d6c(uint32 state)
{
    uint32 phase, result;
    FUNCTION_MARKER(0x80069D6C, "ACE3.BIN");
    phase = r_s16(state + 8);
    result = r_u16(state + 8) + 1;
    if (phase == 0)
        w_u16(state + 8, result);
    return result;
}

uint32 sub_80069d88(uint32 state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069D88, "ACE3.BIN");
    flags = r_u32(state) & 4;
    sub_80080f10(state + 28, 0);
    if (flags != 0)
        return sub_8002f3c0(37, state + 28, 0, 0);
    result = (r_u32(state + 112) & 63) < 2;
    if (result == 0)
        return result;
    return sub_8002f3c0(28, state + 28, 0, 0);
}

uint32 sub_80069e10(uint32 state)
{
    uint32 result, value, table, source;
    FUNCTION_MARKER(0x80069E10, "ACE3.BIN");
    result = r_u32(state) & 1;
    if (result != 0)
        return result;
    value = r_u32(state + 112);
    table = r_u32(0x800BCE5C);
    source = r_u32(table + (value & 63) * 4);
    value = r_u32(source + 8);
    w_u32(state + 28, value);
    value = r_u32(source + 12);
    w_u32(state + 32, value);
    value = r_u32(source + 16);
    w_u32(state + 36, value);
    value = r_u16(source + 24);
    w_u16(state + 44, value);
    value = r_u16(source + 26);
    w_u16(state + 46, value);
    value = r_u16(source + 28);
    w_u16(state + 48, value);
    sub_800809f0(state + 28, state + 140, state + 16, 0);
    return sub_8002f3c0(29, state + 28, 0, 0);
}

uint32 sub_80069ec0(uint32 state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069EC0, "ACE3.BIN");
    flags = r_u32(state) & 4;
    sub_80080f10(state + 28, 1);
    if (flags != 0)
        return sub_8002f3c0(29, state + 28, 0, 0);
    result = (r_u32(state + 112) & 63) < 2;
    if (result == 0)
        return result;
    return sub_8002f3c0(28, state + 28, 0, 0);
}

uint32 sub_80069f48(uint32 state)
{
    uint32 phase, value, a, b, flags, result;
    FUNCTION_MARKER(0x80069F48, "ACE3.BIN");
    phase = r_s16(state + 8);
    value = r_u16(state + 8) + 1;
    if (phase != 0)
    {
        if (phase != 1)
            return 1;
    }
    else
        w_u16(state + 8, value);
    a = r_u32(state + 80);
    b = r_u32(state + 128);
    if ((sint32)a < (sint32)b)
    {
        a = r_u32(state + 96);
        b = r_u32(state + 132);
        w_u32(state + 96, a + b);
    }
    a = r_u32(state + 96);
    b = r_u32(state + 100);
    flags = r_u32(state);
    result = a + b;
    w_u32(state + 80, result);
    w_u32(state, flags | 0x4000);
    return result;
}

void sub_80069fc0(void)
{
    FUNCTION_MARKER(0x80069FC0, "ACE3.BIN");
}

uint32 sub_80080f10(uint32 position, uint32 kind)
{
    uint32 effect, value;
    FUNCTION_MARKER(0x80080F10, "ACE3.BIN");
    effect = sub_8007ece8(5, position, 0);
    value = r_u32(effect + 8);
    value = (value & 0xFFFFF00F) | ((kind & 255) << 4);
    w_u32(effect + 8, value);
    return sub_80080eb4(position, kind, 0);
}

uint32 sub_800692c4(uint32 state)
{
    uint32 divisor, scale, numerator, flags, vector, source, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x800692C4, "ACE3.BIN");
    divisor = r_u32(state + 0x50);
    scale = 1;
    if (divisor != 0)
    {
        numerator = r_u32(state + 0x68) << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
    }
    flags = r_u32(state);
    if ((flags & 0x100) != 0)
        scale = 0;
    vector = r_u32(state + 0x78);
    x = r_u32(vector) * scale;
    y = r_u32(vector + 4) * scale;
    source = r_u32(state + 0x74);
    value = r_u32(source);
    z = r_u32(vector + 8) * scale;
    target[0] = value + x;
    source = r_u32(state + 0x74);
    value = r_u32(source + 4);
    target[1] = value + y;
    source = r_u32(state + 0x74);
    value = r_u32(source + 8);
    target[2] = value + z;
    distance = sub_800319c8(state + 0x1C, 0, state + 0x58, 0, target);
    limit = r_s16(state + 0x7C);
    angle = r_s16(state + 0x58);
    raw = r_u16(state + 0x58);
    w_u32(state + 0x68, distance);
    w_u16(state + 0x5C, 0);
    clamped = 0 - limit;
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    w_u16(state + 0x58, clamped);
    clamped = 0 - limit;
    angle = r_s16(state + 0x5A);
    raw = r_u16(state + 0x5A);
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    speed = r_u32(state + 0x50);
    maximum = r_u32(state + 0x80);
    w_u16(state + 0x5A, clamped);
    if ((sint32)speed < (sint32)maximum)
    {
        acceleration = r_u32(state + 0x60);
        value = r_u32(state + 0x88);
        w_u32(state + 0x60, acceleration + value);
    }
    acceleration = r_u32(state + 0x60);
    value = r_u32(state + 0x64);
    flags = r_u32(state);
    result = acceleration + value;
    flags |= 0x5000;
    w_u32(state, flags);
    w_u32(state + 0x50, result);
    if ((flags & 8) == 0)
        return result;
    limit = r_s16(state + 0x7C);
    angle = r_s16(state + 0x30);
    raw = r_u16(state + 0x30);
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        w_u16(state + 0x5C, maximum);
        if ((sint32)angle >= 0)
            w_u16(state + 0x5C, 0 - maximum);
    }
    else
    {
        w_u16(state + 0x5C, 0 - raw);
        value = r_u32(state);
        w_u32(state, value & 0xFFFFFFF7);
    }
    result = r_u32(state) | 0x2000;
    w_u32(state, result);
    return result;
}

uint32 sub_80069a9c(uint32 state)
{
    uint32 divisor, scale, numerator, flags, vector, source, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x80069A9C, "ACE3.BIN");
    divisor = r_u32(state + 0x50);
    scale = 1;
    if (divisor != 0)
    {
        numerator = r_u32(state + 0x68) << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
    }
    vector = r_u32(state + 0x78);
    x = r_u32(vector) * scale;
    y = r_u32(vector + 4) * scale;
    source = r_u32(state + 0x74);
    value = r_u32(source);
    z = r_u32(vector + 8) * scale;
    target[0] = value + x;
    source = r_u32(state + 0x74);
    value = r_u32(source + 4);
    target[1] = value + y;
    source = r_u32(state + 0x74);
    value = r_u32(source + 8);
    target[2] = value + z;
    distance = sub_800319c8(state + 0x1C, 0, state + 0x58, 0, target);
    limit = r_s16(state + 0x7C);
    angle = r_s16(state + 0x58);
    raw = r_u16(state + 0x58);
    w_u32(state + 0x68, distance);
    w_u16(state + 0x5C, 0);
    clamped = 0 - limit;
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    w_u16(state + 0x58, clamped);
    clamped = 0 - limit;
    angle = r_s16(state + 0x5A);
    raw = r_u16(state + 0x5A);
    if ((sint32)angle >= (sint32)clamped)
    {
        clamped = limit;
        if ((sint32)limit >= (sint32)angle)
            clamped = raw;
    }
    speed = r_u32(state + 0x50);
    maximum = r_u32(state + 0x80);
    w_u16(state + 0x5A, clamped);
    if ((sint32)speed < (sint32)maximum)
    {
        acceleration = r_u32(state + 0x60);
        value = r_u32(state + 0x88);
        w_u32(state + 0x60, acceleration + value);
    }
    acceleration = r_u32(state + 0x60);
    value = r_u32(state + 0x64);
    flags = r_u32(state);
    result = acceleration + value;
    flags |= 0x5000;
    w_u32(state, flags);
    w_u32(state + 0x50, result);
    if ((flags & 8) == 0)
        return result;
    limit = r_s16(state + 0x7C);
    angle = r_s16(state + 0x30);
    raw = r_u16(state + 0x30);
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        w_u16(state + 0x5C, maximum);
        if ((sint32)angle >= 0)
            w_u16(state + 0x5C, 0 - maximum);
    }
    else
    {
        w_u16(state + 0x5C, 0 - raw);
        value = r_u32(state);
        w_u32(state, value & 0xFFFFFFF7);
    }
    result = r_u32(state) | 0x2000;
    w_u32(state, result);
    return result;
}

uint32 sub_800696d4(uint32 state)
{
    uint32 result, value, magnitude, dx, dy, dz, px, py, pz, x, y, z, scene, map, limit, ix, iz, tile, table, object, height, product, lower, upper, angle;
    FUNCTION_MARKER(0x800696D4, "ACE3.BIN");
    sub_800694cc(state);
    value = r_u32(state + 104);
    result = (sint32)value < 4097;
    if (result != 0)
        return result;
    value = r_s16(state + 48);
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = (sint32)magnitude < 227;
    if (result == 0)
        return result;
    dx = r_u32(state + 56);
    dy = r_u32(state + 60);
    px = r_u32(state + 28);
    x = dx * 20 + px;
    py = r_u32(state + 32);
    scene = r_u32(0x800BCE54);
    y = dy * 20 + py;
    dz = r_u32(state + 64);
    map = r_u32(scene + 584);
    w_u16(state + 88, 0);
    pz = r_u32(state + 36);
    z = dz * 20 + pz;
    limit = r_u32(scene + 672);
    if ((sint32)x < (sint32)limit)
        x = limit;
    limit = r_u32(scene + 676);
    if ((sint32)limit < (sint32)x)
        x = limit;
    limit = r_u32(scene + 680);
    if ((sint32)y < (sint32)limit)
        y = limit;
    limit = r_u32(scene + 684);
    if ((sint32)limit < (sint32)y)
        y = limit;
    limit = r_u32(scene + 688);
    if ((sint32)z < (sint32)limit)
        z = limit;
    limit = r_u32(scene + 692);
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
    tile = r_u16(map + ((ix + (iz << 8)) << 1)) & 511;
    result = tile << 2;
    if (tile == 0)
        return result;
    table = r_u32(scene + 588);
    limit = r_s16(state + 124);
    object = r_u32(table + result);
    height = r_s16(object + 22);
    product = ((uint32)((sint32)y >> 12) + 400 - height) * (limit << 2);
    value = (sint32)product / 200;
    w_u16(state + 88, value);
    lower = (0 - (limit << 2)) << 2;
    if ((sint32)(sint16)value < (sint32)lower)
        w_u16(state + 88, lower);
    angle = r_s16(state + 88);
    upper = limit << 4;
    result = (sint32)upper < (sint32)angle;
    if (result != 0)
        w_u16(state + 88, upper);
    return result;
}

uint32 sub_80069900(uint32 state)
{
    NATIVE_POSE pose;
    uint32 distance, threshold, result, flags, kind, owner;
    FUNCTION_MARKER(0x80069900, "ACE3.BIN");
    sub_800694cc(state);
    distance = r_u32(state + 104);
    threshold = r_u32(state + 152);
    result = (sint32)distance < (sint32)threshold;
    if (result == 0)
        return result;
    pose.position[0] = r_u32(state + 28);
    pose.position[1] = r_u32(state + 32);
    pose.position[2] = r_u32(state + 36);
    pose.angles[0] = r_u16(state + 44);
    pose.angles[1] = r_u16(state + 46);
    pose.angles[2] = r_u16(state + 48);
    flags = r_u32(state + 112) & 32;
    if (flags != 0)
    {
        pose.angles[0] = (uint16)(pose.angles[0] + 682);
        pose.angles[1] = (uint16)(pose.angles[1] + 682);
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[1] = (uint16)(pose.angles[1] - 1365);
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[0] = (uint16)(pose.angles[0] - 682);
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[1] = (uint16)(pose.angles[1] + 1365);
    }
    else
    {
        pose.angles[2] = 0;
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[0] = (uint16)(pose.angles[0] + 1137);
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[0] = (uint16)(pose.angles[0] - 2275);
        pose.angles[1] = (uint16)(pose.angles[1] + 2048);
        kind = r_u32(state + 108);
        owner = r_u32(state + 112);
        sub_80068e28(kind, 0, state + 56, owner, &pose);
        pose.angles[1] = (uint16)(pose.angles[1] - 4096);
    }
    kind = r_u32(state + 108);
    owner = r_u32(state + 112);
    result = sub_80068e28(kind, 0, state + 56, owner, &pose);
    w_u16(state + 24, 0);
    return result;
}

uint32 sub_80068e28(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 effect, value, seed, random;
    FUNCTION_MARKER(0x80068E28, "ACE3.BIN");
    effect = sub_800685c0(flags, position, vector, owner, native_position);
    if (effect == 0)
        return 0;
    value = r_u8(effect + 53);
    w_u16(effect + 26, 256);
    w_u8(effect + 52, 2);
    w_u8(effect + 6, 4);
    w_u8(effect + 7, 0);
    w_u8(effect + 5, 0);
    w_u8(effect + 53, value | 24);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    w_u16(effect + 126, random & 3);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = r_u16(effect + 48);
    w_u16(effect + 48, value - 512 + (random & 1023));
    if ((owner & 32) != 0)
    {
        w_u16(effect + 24, 150);
        w_u16(effect + 124, 24);
        w_u32(effect + 132, 4096);
        w_u32(effect + 136, 4096);
        w_u32(effect + 156, 640);
        value = r_u32(effect);
        w_u32(effect + 128, 0x3C000);
        w_u32(effect + 160, 192);
        w_u32(effect, value | 16);
    }
    else
    {
        w_u16(effect + 24, 240);
        w_u16(effect + 124, 48);
        w_u32(effect + 132, 512);
        w_u32(effect + 128, 0x6E000);
        w_u32(effect + 136, 3072);
    }
    return effect;
}

uint32 sub_8006903c(uint32 state)
{
    uint32 phase, raw, counter, delay, flags, value, a, b, result, limit, angle, maximum, magnitude;
    FUNCTION_MARKER(0x8006903C, "ACE3.BIN");
    phase = r_s16(state + 8);
    raw = r_u16(state + 8);
    if (phase == 0 || phase == 1)
    {
        if (phase == 0)
            w_u16(state + 8, raw + 1);
        counter = r_s16(state + 10);
        delay = r_s16(state + 126);
        if ((sint32)delay < (sint32)counter)
        {
            flags = r_u32(state) & 1;
            if (flags != 0)
            {
                value = r_u16(state + 8);
                w_u16(state + 8, value + 1);
            }
            else
            {
                w_u8(state + 4, 1);
                w_u16(state + 8, 0);
                w_u16(state + 10, 0xFFFF);
            }
        }
        a = r_u32(state + 96);
        b = r_u32(state + 132);
        value = r_u32(state + 100);
        a += b;
        flags = r_u32(state);
        w_u32(state + 96, a);
        w_u32(state + 80, a + value);
        w_u32(state, flags | 0x4000);
    }
    result = r_u32(state) & 8;
    if (result == 0)
        return result;
    limit = r_s16(state + 124);
    angle = r_s16(state + 48);
    raw = r_u16(state + 48);
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        w_u16(state + 92, maximum);
        if ((sint32)angle >= 0)
            w_u16(state + 92, 0 - maximum);
    }
    else
    {
        w_u16(state + 92, 0 - raw);
        flags = r_u32(state);
        w_u32(state, flags & 0xFFFFFFF7);
    }
    result = r_u32(state) | 0x2000;
    w_u32(state, result);
    return result;
}

uint32 sub_80069158(uint32 state)
{
    uint32 phase, raw, counter, delay, flags, value, a, b, result, limit, angle, maximum, magnitude;
    FUNCTION_MARKER(0x80069158, "ACE3.BIN");
    phase = r_s16(state + 8);
    raw = r_u16(state + 8);
    if (phase == 0 || phase == 1)
    {
        if (phase == 0)
            w_u16(state + 8, raw + 1);
        counter = r_s16(state + 10);
        delay = r_s16(state + 126);
        if ((sint32)delay < (sint32)counter)
        {
            flags = r_u32(state) & 1;
            if (flags != 0)
            {
                value = r_u16(state + 8);
                w_u16(state + 8, value + 1);
            }
            else
            {
                w_u8(state + 4, 1);
                w_u16(state + 8, 0);
                w_u16(state + 10, 0xFFFF);
            }
            w_u32(state + 76, 0);
        }
        counter = r_s16(state + 10);
        if ((sint32)counter < 7)
        {
            value = r_u32(state + 76);
            w_u32(state + 76, value + 669);
        }
        else
        {
            a = r_u32(state + 80);
            b = r_u32(state + 128);
            if ((sint32)a < (sint32)b)
            {
                a = r_u32(state + 96);
                b = r_u32(state + 132);
                w_u32(state + 96, a + b);
            }
            a = r_u32(state + 96);
            b = r_u32(state + 100);
            w_u32(state + 80, a + b);
        }
        flags = r_u32(state);
        w_u32(state, flags | 0x4000);
    }
    result = r_u32(state) & 8;
    if (result == 0)
        return result;
    limit = r_s16(state + 124);
    angle = r_s16(state + 48);
    raw = r_u16(state + 48);
    maximum = limit << 2;
    magnitude = (sint32)angle < 0 ? 0 - angle : angle;
    if ((sint32)maximum < (sint32)magnitude)
    {
        w_u16(state + 92, maximum);
        if ((sint32)angle >= 0)
            w_u16(state + 92, 0 - maximum);
    }
    else
    {
        w_u16(state + 92, 0 - raw);
        flags = r_u32(state);
        w_u32(state, flags & 0xFFFFFFF7);
    }
    result = r_u32(state) | 0x2000;
    w_u32(state, result);
    return result;
}

uint32 sub_80069fc8(void)
{
    uint32 root, mask, state, index, kind, selector, target, counter;
    FUNCTION_MARKER(0x80069FC8, "ACE3.BIN");
    root = r_u32(0x800BD080);
    mask = r_u32(root + 5448);
    if (mask == 0)
        return 0;
    mask = ~mask;
    state = root + 200;
    for (index = 0; index < 32; ++index)
    {
        if ((mask & ((uint32)1 << index)) == 0)
        {
            kind = r_u8(state + 4);
            if (kind == 0)
            {
                selector = r_u8(state + 5);
                target = r_u32(0x800B2C80 + selector * 4);
            }
            else if (kind == 1)
            {
                selector = r_u8(state + 6);
                target = r_u32(0x800B2C94 + selector * 4);
            }
            else if (kind == 2)
            {
                selector = r_u8(state + 7);
                target = r_u32(0x800B2CA8 + selector * 4);
            }
            else
                target = 0x80069FC0;
            xport_guest_call1(target, state);
            counter = r_u16(state + 10);
            w_u16(state + 10, counter + 1);
        }
        state += 164;
    }
    return 0;
}

uint32 sub_8006a11c(void)
{
    uint32 local[154];
    uint32 root, mask, count, queued, objects, scratch, radius, object, flags, x, y, z, size, value, index, state, dx, dy, dz, ax, ay, az, range, radius_squared, filter, j, sx, sy, sz, sum, target, old_x, old_y, old_z, a, b, c, result, scene, bounds, descriptor, map, collision, material, normal, nx, ny, nz, cells[4], k, cell, table, height;
    FUNCTION_MARKER(0x8006A11C, "ACE3.BIN");
    root = r_u32(0x800BD080);
    mask = r_u32(root + 5448);
    queued = 0;
    local[152] = mask;
    if (mask == 0)
        return 0;
    count = 0;
    objects = r_u32(0x800BCE5C);
    scratch = 0x1F800000;
    for (index = 0; index < 64; ++index)
    {
        object = r_u32(objects + index * 4);
        flags = r_u32(object);
        if ((flags & 0x4200) != 0x4200)
            continue;
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        size = r_s16(object + 74);
        w_u32(scratch, (sint32)x >> 12);
        w_u32(scratch + 4, (sint32)y >> 12);
        w_u32(scratch + 8, (sint32)z >> 12);
        w_u32(scratch + 12, flags);
        scratch += 16;
        local[count * 2] = size * 3;
        local[count * 2 + 1] = size * size;
        ++count;
    }
    mask = local[152];
    root = r_u32(0x800BD080);
    mask = ~mask;
    state = root + 200;
    for (index = 0; index < 32; ++index, state += 164)
    {
        if ((mask & ((uint32)1 << index)) != 0)
            continue;
        dx = r_u32(state + 56);
        dy = r_u32(state + 60);
        dz = r_u32(state + 64);
        ax = (sint32)dx < 0 ? 0 - dx : dx;
        ay = (sint32)dy < 0 ? 0 - dy : dy;
        az = (sint32)dz < 0 ? 0 - dz : dz;
        range = (sint32)(ax + ay + az) >> 13;
        value = range * range;
        radius_squared = (sint32)value / 9;
        value = r_u32(state + 32);
        y = (sint32)(value + dy) >> 12;
        filter = r_u32(state + 108) & 0x1C0;
        value = r_u32(state + 28);
        x = (sint32)(value + dx) >> 12;
        value = r_u32(state + 36);
        z = (sint32)(value + dz) >> 12;
        if ((sint32)count <= 0)
            continue;
        filter = ~filter & 0x1C0;
        scratch = 0x1F800000;
        for (j = 0; j < count; ++j, scratch += 16)
        {
            flags = r_u32(scratch + 12);
            if ((flags & filter) == 0)
                continue;
            sx = r_u32(scratch);
            sz = r_u32(scratch + 8);
            dx = sx - x;
            dz = sz - z;
            ax = (sint32)dx < 0 ? 0 - dx : dx;
            sy = r_u32(scratch + 4);
            az = (sint32)dz < 0 ? 0 - dz : dz;
            dy = sy - y;
            ay = (sint32)dy < 0 ? 0 - dy : dy;
            sum = ax + ay + az;
            radius = local[j * 2] + range;
            if ((sint32)radius < (sint32)sum)
                continue;
            local[153] = dy * dy;
            sum = dx * dx + local[153] + dz * dz;
            radius = local[j * 2 + 1] + radius_squared;
            if ((sint32)radius < (sint32)sum)
                continue;
            objects = r_u32(0x800BCE5C);
            target = r_u32(objects + (flags & 63) * 4);
            if ((flags & 0x1800) != 0)
            {
                local[128 + queued * 2] = state;
                local[129 + queued * 2] = target;
                if (queued < 8)
                    ++queued;
            }
            else
            {
                value = r_u16(state + 26);
                a = r_u32(target + 84);
                w_u32(target + 84, a | value);
                value = r_u32(state + 108);
                w_u32(target + 80, value);
                a = r_u32(state + 56);
                b = r_u32(state + 60);
                c = r_u32(state + 64);
                w_u32(state + 28, x << 12);
                w_u32(state + 32, y << 12);
                w_u32(state + 36, z << 12);
                w_u32(state + 56, 0);
                w_u32(state + 60, 0);
                w_u32(state + 64, 0);
                w_u32(state + 140, a);
                w_u32(state + 144, b);
                w_u32(state + 148, c);
                value = r_u32(state);
                w_u32(state, value | 2);
                w_u16(state + 24, 0);
                w_u8(state + 4, 2);
                w_u16(state + 8, 0);
                w_u16(state + 10, 0);
            }
        }
    }
    for (index = 0; index < queued; ++index)
    {
        target = local[129 + index * 2];
        flags = r_u32(target);
        state = local[128 + index * 2];
        if ((flags & 0x1020) == 0x1020 && (r_u8(target + 5) & 2) == 0)
            result = sub_8009b4ec(state + 28, state + 56, target, 0, 0x1F800000, local + 144);
        else
        {
            local[144] = r_u32(state + 28);
            local[145] = r_u32(state + 32);
            local[146] = r_u32(state + 36);
            result = 0;
        }
        if (result == 0x7FFFFFFF)
            continue;
        value = r_u16(state + 26);
        a = r_u32(target + 84);
        w_u32(target + 84, a | value);
        value = r_u32(state + 108);
        w_u32(target + 80, value);
        x = local[144];
        a = r_u32(state + 56);
        b = r_u32(state + 60);
        c = r_u32(state + 64);
        w_u32(state + 28, x);
        w_u32(state + 32, local[145]);
        z = local[146];
        flags = r_u32(state);
        w_u32(state + 140, a);
        w_u32(state + 144, b);
        w_u32(state + 148, c);
        w_u32(state + 56, 0);
        w_u32(state + 60, 0);
        w_u32(state + 64, 0);
        w_u16(state + 24, 0);
        w_u16(state + 8, 0);
        w_u16(state + 10, 0);
        w_u32(state, flags | 2);
        w_u8(state + 4, 2);
        w_u32(state + 36, z);
    }
    scene = r_u32(0x800BCE54);
    root = r_u32(0x800BD080);
    bounds = scene + 672;
    state = root + 200;
    descriptor = scene + 580;
    map = r_u32(descriptor + 4);
    for (index = 0; index < 32; ++index, state += 164)
    {
        if ((~local[152] & ((uint32)1 << index)) != 0)
            continue;
        collision = 0;
        material = 0;
        old_x = r_u32(state + 28);
        old_y = r_u32(state + 32);
        dx = r_u32(state + 56);
        dy = r_u32(state + 60);
        old_z = r_u32(state + 36);
        x = dx + old_x;
        y = dy + old_y;
        dz = r_u32(state + 64);
        value = r_u32(bounds);
        z = dz + old_z;
        if ((sint32)x < (sint32)value)
        {
            collision = 1;
            material = 4;
            x = value;
        }
        value = r_u32(bounds + 4);
        if ((sint32)value < (sint32)x)
        {
            collision = 2;
            material = 4;
            x = value;
        }
        value = r_u32(bounds + 8);
        if ((sint32)y < (sint32)value)
        {
            collision = 3;
            material = 3;
            y = value;
        }
        value = r_u32(bounds + 12);
        if ((sint32)value < (sint32)y)
        {
            collision = 4;
            material = 1;
            y = value;
        }
        value = r_u32(bounds + 16);
        if ((sint32)z < (sint32)value)
        {
            collision = 5;
            material = 4;
            z = value;
        }
        value = r_u32(bounds + 20);
        if ((sint32)value < (sint32)z)
        {
            collision = 6;
            material = 4;
            z = value;
        }
        if (collision != 0)
        {
            flags = r_u32(state);
            w_u32(state, flags | 4);
            normal = 0x800245C8 + collision * 8;
            w_u16(state + 24, 0);
            w_u32(state + 28, x);
            w_u32(state + 32, y);
            w_u32(state + 36, z);
            nx = r_u16(normal);
            c = r_u32(state + 64);
            w_u16(state + 16, nx);
            ny = r_u16(normal + 2);
            w_u16(state + 18, ny);
            nz = r_u16(normal + 4);
            a = r_u32(state + 56);
            b = r_u32(state + 60);
            w_u32(0x800BD0C4, material);
            w_u32(state + 12, material);
            w_u32(state + 148, c);
            w_u32(state + 56, 0);
            w_u32(state + 60, 0);
            w_u32(state + 64, 0);
            w_u16(state + 8, 0);
            w_u16(state + 10, 0);
            w_u32(state + 140, a);
            w_u32(state + 144, b);
            w_u8(state + 4, 2);
            w_u16(state + 20, nz);
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
        local[144] = x;
        local[145] = y;
        local[146] = z;
        if (cell == cells[2] + (cells[3] << 8))
        {
            value = r_u16(map + (cell << 1)) & 511;
            if (value == 0)
                continue;
            table = r_u32(descriptor + 8);
            object = r_u32(table + value * 4);
            height = r_s16(object + 22);
            if (((sint32)old_y >> 12) < (sint32)height && ((sint32)y >> 12) < (sint32)height)
                continue;
            result = sub_8009a5d0(state + 28, 0, 0, 0x1F800000, 0, local + 144, local + 148);
        }
        else
            result = sub_8009a800(state + 28, 0, 0, 0x1F800000, 0, local + 144, local + 148);
        if (result == 0x7FFFFFFF)
            continue;
        flags = r_u32(state);
        normal = r_u32(0x800BD0C0);
        w_u32(state, flags | 4);
        w_u16(state + 24, 0);
        x = local[148];
        a = r_u32(state + 56);
        c = r_u32(state + 64);
        w_u32(state + 28, x);
        w_u32(state + 32, local[149]);
        z = local[150];
        b = r_u32(state + 60);
        w_u32(state + 140, a);
        w_u32(state + 148, c);
        w_u32(state + 56, 0);
        w_u32(state + 60, 0);
        w_u32(state + 64, 0);
        w_u32(state + 144, b);
        w_u32(state + 36, z);
        nx = r_u16(normal);
        w_u16(state + 16, nx);
        ny = r_u16(normal + 2);
        w_u16(state + 18, ny);
        material = r_u32(0x800BD0C4);
        nz = r_u16(normal + 4);
        w_u16(state + 8, 0);
        w_u16(state + 10, 0);
        w_u32(state + 12, material);
        w_u8(state + 4, 2);
        w_u16(state + 20, nz);
    }
    return 0;
}

uint32 sub_8009b4ec(uint32 position, uint32 velocity, uint32 target, uint32 out, uint32 workspace, uint32 *native_out)
{
    uint32 scaled[3];
    uint16 relative[3];
    uint32 value, origin, selector, scene, table, flags, factor, index, next, model, other, vertices, first, second, cursor, result;
    FUNCTION_MARKER(0x8009B4EC, "ACE3.BIN");
    value = r_u32(velocity);
    cursor = workspace;
    scaled[0] = (sint32)value >> 12;
    value = r_u32(velocity + 4);
    scaled[1] = (sint32)value >> 12;
    value = r_u32(velocity + 8);
    scaled[2] = (sint32)value >> 12;
    value = r_u32(target + 8);
    origin = r_u32(position);
    relative[0] = (uint16)((sint32)(value - origin) >> 12);
    value = r_u32(target + 12);
    origin = r_u32(position + 4);
    relative[1] = (uint16)((sint32)(value - origin) >> 12);
    value = r_u32(target + 16);
    origin = r_u32(position + 8);
    relative[2] = (uint16)((sint32)(value - origin) >> 12);
    selector = r_u8(target + 4);
    scene = r_u32(0x800BCE54);
    table = r_u32(scene + 600);
    flags = r_u8(target + 5);
    table += selector * 4;
    if ((flags & 4) != 0)
    {
        value = r_u16(target + 6);
        index = (sint32)(value << 16) >> 28;
        factor = value & 4095;
        if ((sint32)index < 0)
            index = 0;
        if ((sint32)index >= 5)
            index = 4;
        if (factor != 0)
        {
            next = index < 4 ? index + 1 : 4;
            vertices = workspace;
            model = r_u32(table + index * 4);
            other = r_u32(table + next * 4);
            first = r_u32(model + 4);
            second = r_u32(other + 4);
            cursor = sub_80066a9c(factor, first, second, workspace);
        }
        else
        {
            model = r_u32(table + index * 4);
            vertices = r_u32(model + 4);
        }
    }
    else
    {
        model = r_u32(table);
        vertices = r_u32(model + 4);
    }
    result = sub_8009a944(0, out, cursor, model, vertices, 0, scaled, native_out, relative);
    value = native_out != 0 ? native_out[0] : r_u32(out);
    origin = r_u32(position);
    value += origin;
    if (native_out != 0)
        native_out[0] = value;
    else
        w_u32(out, value);
    value = native_out != 0 ? native_out[1] : r_u32(out + 4);
    origin = r_u32(position + 4);
    value += origin;
    if (native_out != 0)
        native_out[1] = value;
    else
        w_u32(out + 4, value);
    value = native_out != 0 ? native_out[2] : r_u32(out + 8);
    origin = r_u32(position + 8);
    value += origin;
    if (native_out != 0)
        native_out[2] = value;
    else
        w_u32(out + 8, value);
    return result;
}

uint32 sub_800804b8(uint32 position, uint32 material, uint32 mode, const uint32 *native_position)
{
    uint32 effect, value, kind;
    FUNCTION_MARKER(0x800804B8, "ACE3.BIN");
    kind = material & 15;
    effect = sub_8007ece8(1, position, native_position);
    value = r_u32(effect + 8);
    value = (value & 0xFFFFF00F) | ((mode & 255) << 4);
    value = (value & 0xFFFFFFF0) | kind;
    w_u32(effect + 8, value);
    return sub_80080140(position, kind, mode, native_position);
}

void sub_8006b504(void)
{
    uint32 local[164], cells[4];
    uint32 objects, scratch, count, queued, index, object, flags, x, y, z, size, base, bit, mask, state, dx, dy, dz, ax, ay, az, range, filter, j, sx, sy, sz, sum, radius, target, value, sound, mode, effect, a, b, c, result, scene, bounds, descriptor, map, material, old_x, old_y, old_z, cell, table, height, k, expired;
    FUNCTION_MARKER(0x8006B504, "ACE3.BIN");
    count = 0;
    queued = 0;
    objects = r_u32(0x800BCE5C);
    scratch = 0x1F800000;
    for (index = 0; index < 64; ++index)
    {
        object = r_u32(objects + index * 4);
        flags = r_u32(object);
        if ((flags & 0x4200) != 0x4200)
            continue;
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        size = r_s16(object + 74);
        w_u32(scratch, (sint32)x >> 12);
        w_u32(scratch + 4, (sint32)y >> 12);
        w_u32(scratch + 8, (sint32)z >> 12);
        w_u32(scratch + 12, flags);
        scratch += 16;
        local[count * 2] = size * 3;
        local[count * 2 + 1] = size * size;
        ++count;
    }
    local[154] = r_u32(0x800BD080) + 13264;
    for (base = 0; base < 128; base += 32)
    {
        local[155] = r_u32(local[154]);
        if (local[155] != 0)
            for (bit = 0; bit < 32; ++bit)
            {
                if ((local[155] & ((uint32)1 << bit)) == 0)
                    continue;
                state = r_u32(0x800BD080) + (base + bit) * 60 + 5584;
                dx = r_u32(state + 40);
                dy = r_u32(state + 44);
                dz = r_u32(state + 48);
                ax = (sint32)dx < 0 ? 0 - dx : dx;
                ay = (sint32)dy < 0 ? 0 - dy : dy;
                az = (sint32)dz < 0 ? 0 - dz : dz;
                range = (sint32)(ax + ay + az) >> 13;
                local[156] = range;
                value = range * range;
                a = r_u32(state + 12);
                y = (sint32)(a + dy) >> 12;
                filter = r_u32(state + 56) & 0x1C0;
                a = r_u32(state + 8);
                x = (sint32)(a + dx) >> 12;
                a = r_u32(state + 16);
                z = (sint32)(a + dz) >> 12;
                local[157] = (sint32)value / 9;
                if ((sint32)count <= 0)
                    continue;
                local[158] = ~filter & 0x1C0;
                scratch = 0x1F800000;
                for (j = 0; j < count; ++j, scratch += 16)
                {
                    flags = r_u32(scratch + 12);
                    if ((flags & local[158]) == 0)
                        continue;
                    sx = r_u32(scratch);
                    sz = r_u32(scratch + 8);
                    range = local[156];
                    dx = sx - x;
                    dz = sz - z;
                    ax = (sint32)dx < 0 ? 0 - dx : dx;
                    sy = r_u32(scratch + 4);
                    az = (sint32)dz < 0 ? 0 - dz : dz;
                    dy = sy - y;
                    ay = (sint32)dy < 0 ? 0 - dy : dy;
                    sum = ax + ay + az;
                    radius = local[j * 2] + range;
                    if ((sint32)radius < (sint32)sum)
                        continue;
                    sum = dx * dx + dy * dy + dz * dz;
                    radius = local[j * 2 + 1] + local[157];
                    if ((sint32)radius < (sint32)sum)
                        continue;
                    objects = r_u32(0x800BCE5C);
                    target = r_u32(objects + (flags & 63) * 4);
                    if ((flags & 0x1800) != 0)
                    {
                        local[130 + queued * 2] = state;
                        local[131 + queued * 2] = target;
                        if (queued < 8)
                            ++queued;
                        continue;
                    }
                    flags = r_u32(state);
                    for (k = 0; k < 6; ++k)
                        if ((flags & (0x400 << k)) != 0)
                        {
                            value = r_u32(target + 84);
                            w_u32(target + 84, value | ((uint32)1 << k));
                            break;
                        }
                    value = r_u32(state + 56);
                    w_u32(target + 80, value);
                    w_u32(state + 8, x << 12);
                    w_u32(state + 12, y << 12);
                    w_u32(state + 16, z << 12);
                    w_u32(state + 4, 0);
                    sound = 30;
                    mode = 2;
                    flags = r_u32(state);
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
                    local[160] = y;
                    local[161] = x;
                    local[162] = count;
                    sub_8002f460(sound, state + 8, mode, 0);
                    flags = r_u32(state);
                    y = local[160];
                    x = local[161];
                    count = local[162];
                    effect = 0;
                    for (k = 0; k < 5; ++k)
                        if ((flags & (32 << k)) != 0)
                        {
                            effect = k + 1;
                            break;
                        }
                    local[160] = y;
                    local[161] = x;
                    local[162] = count;
                    sub_800800e0(state + 8, effect);
                    y = local[160];
                    x = local[161];
                    count = local[162];
                }
            }
        local[154] += 4;
    }
    for (index = 0; index < queued; ++index)
    {
        target = local[131 + index * 2];
        flags = r_u32(target);
        state = local[130 + index * 2];
        if ((flags & 0x1020) == 0x1020 && (r_u8(target + 5) & 2) == 0)
            result = sub_8009b4ec(state + 8, state + 40, target, 0, 0x1F800000, local + 146);
        else
        {
            local[146] = r_u32(state + 8);
            local[147] = r_u32(state + 12);
            local[148] = r_u32(state + 16);
            result = 0;
        }
        if (result == 0x7FFFFFFF)
            continue;
        flags = r_u32(state);
        for (k = 0; k < 3; ++k)
            if ((flags & (0x400 << k)) != 0)
            {
                value = r_u32(target + 84);
                w_u32(target + 84, value | ((uint32)1 << k));
                break;
            }
        sound = 30;
        value = r_u32(state + 56);
        mode = 2;
        w_u32(target + 80, value);
        flags = r_u32(state);
        w_u32(state + 4, 0);
        if ((flags & 0x20000) != 0)
        {
            value = r_u32(0x800BD0C4);
            if ((value & 15) == 1)
                sound = 35;
        }
        else if ((flags & 0x40000) != 0)
        {
            sound = 40;
            value = r_u32(0x800BD0C4);
            if ((value & 15) == 1)
                sound = 35;
        }
        else if ((flags & 0x80000) != 0)
        {
            sound = 41;
            mode = 1;
        }
        sub_8002f460(sound, 0, mode, local + 146);
        flags = r_u32(state);
        effect = 0;
        for (k = 0; k < 5; ++k)
            if ((flags & (32 << k)) != 0)
            {
                effect = k + 1;
                break;
            }
        material = r_u32(0x800BD0C4);
        sub_800804b8(0, material, effect, local + 146);
    }
    scene = r_u32(0x800BCE54);
    objects = r_u32(0x800BD080);
    bounds = scene + 672;
    local[154] = objects + 13264;
    descriptor = scene + 580;
    map = r_u32(descriptor + 4);
    for (base = 0; base < 128; base += 32)
    {
        mask = r_u32(local[154]);
        if (mask != 0)
            for (bit = 0; bit < 32; ++bit)
            {
                if ((mask & ((uint32)1 << bit)) == 0)
                    continue;
                state = r_u32(0x800BD080) + (base + bit) * 60 + 5584;
                old_x = r_u32(state + 8);
                old_y = r_u32(state + 12);
                dx = r_u32(state + 40);
                dy = r_u32(state + 44);
                old_z = r_u32(state + 16);
                x = dx + old_x;
                y = dy + old_y;
                dz = r_u32(state + 48);
                value = r_u32(bounds);
                z = dz + old_z;
                material = 0;
                if ((sint32)x < (sint32)value)
                {
                    material = 4;
                    x = value;
                }
                value = r_u32(bounds + 4);
                if ((sint32)value < (sint32)x)
                {
                    material = 4;
                    x = value;
                }
                value = r_u32(bounds + 8);
                if ((sint32)y < (sint32)value)
                {
                    material = 3;
                    y = value;
                }
                value = r_u32(bounds + 12);
                if ((sint32)value < (sint32)y)
                {
                    material = 1;
                    y = value;
                }
                value = r_u32(bounds + 16);
                if ((sint32)z < (sint32)value)
                {
                    material = 4;
                    z = value;
                }
                value = r_u32(bounds + 20);
                if ((sint32)value < (sint32)z)
                {
                    material = 4;
                    z = value;
                }
                if (material != 0)
                {
                    sound = 30;
                    w_u32(state + 4, 0);
                    local[150] = x;
                    local[151] = y;
                    local[152] = z;
                    flags = r_u32(state);
                    w_u32(0x800BD0C4, material);
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
                    sub_8002f460(sound, 0, mode, local + 150);
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
                    local[146] = x;
                    local[147] = y;
                    local[148] = z;
                    if (cell == cells[2] + (cells[3] << 8))
                    {
                        value = r_u16(map + (cell << 1)) & 511;
                        if (value == 0)
                            continue;
                        table = r_u32(descriptor + 8);
                        object = r_u32(table + value * 4);
                        height = r_s16(object + 22);
                        if (((sint32)old_y >> 12) < (sint32)height && ((sint32)y >> 12) < (sint32)height)
                            continue;
                        result = sub_8009a5d0(state + 8, 0, 0, 0x1F800000, 0, local + 146, local + 150);
                    }
                    else
                        result = sub_8009a800(state + 8, 0, 0, 0x1F800000, 0, local + 146, local + 150);
                    if (result == 0x7FFFFFFF)
                        continue;
                    sound = 30;
                    flags = r_u32(state);
                    mode = 2;
                    w_u32(state + 4, 0);
                    if ((flags & 0x20000) != 0)
                    {
                        value = r_u32(0x800BD0C4);
                        if ((value & 15) == 1)
                            sound = 35;
                    }
                    else if ((flags & 0x40000) != 0)
                    {
                        sound = 40;
                        value = r_u32(0x800BD0C4);
                        if ((value & 15) == 1)
                            sound = 35;
                    }
                    else if ((flags & 0x80000) != 0)
                    {
                        sound = 41;
                        mode = 1;
                    }
                    sub_8002f460(sound, 0, mode, local + 150);
                }
                flags = r_u32(state);
                effect = 0;
                for (k = 0; k < 5; ++k)
                    if ((flags & (32 << k)) != 0)
                    {
                        effect = k + 1;
                        break;
                    }
                material = r_u32(0x800BD0C4);
                sub_800804b8(0, material, effect, local + 150);
            }
        local[154] += 4;
    }
    local[154] = r_u32(0x800BD080) + 13264;
    xport_gte_write_data(8, 4000);
    expired = 0;
    for (base = 0; base < 128; base += 32)
    {
        mask = r_u32(local[154]);
        if (mask != 0)
        {
            for (bit = 0; bit < 32; ++bit)
            {
                if ((mask & ((uint32)1 << bit)) == 0)
                    continue;
                index = base + bit;
                state = r_u32(0x800BD080) + index * 60 + 5584;
                value = r_u32(state + 4) - 1;
                w_u32(state + 4, value);
                if ((sint32)value < 0)
                {
                    mask &= ~((uint32)1 << bit);
                    ((uint8 *)local)[512 + expired] = (uint8)index;
                    ++expired;
                    continue;
                }
                dx = r_u32(state + 40);
                dy = r_u32(state + 44);
                a = r_u32(state + 8);
                dz = r_u32(state + 48);
                c = r_u32(state + 16);
                w_u32(state + 8, a + dx);
                b = r_u32(state + 12);
                w_u32(state + 16, c + dz);
                c = r_u32(state + 28);
                w_u32(state + 12, b + dy);
                a = r_u32(state + 24);
                w_u32(state + 28, c + dy);
                flags = ~r_u32(state);
                w_u32(state + 24, a + dx);
                c = r_u32(state + 32);
                w_u32(state + 32, c + dz);
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
                w_u32(state + 40, dx << 6);
                w_u32(state + 44, dy << 6);
                w_u32(state + 48, dz << 6);
            }
            w_u32(local[154], mask);
        }
        local[154] += 4;
    }
    for (index = 0; index < expired; ++index)
    {
        value = ((uint8 *)local)[512 + index];
        objects = r_u32(0x800BD080);
        flags = r_u32(objects + value * 60 + 5640);
        if ((flags & 0x2000) != 0)
            sub_8007e900(value);
    }
}

/* Native call boundary for the audited eight-argument SDK projection */
static uint32 project_hud_a7388(uint32 packet)
{
    uint32 value, flags;
    value = r_u32(0x1F800000);
    xport_gte_write_data(0, value);
    value = r_u32(0x1F800004);
    xport_gte_write_data(1, value);
    value = r_u32(0x1F800008);
    xport_gte_write_data(2, value);
    value = r_u32(0x1F80000C);
    xport_gte_write_data(3, value);
    value = r_u32(0x1F800010);
    xport_gte_write_data(4, value);
    value = r_u32(0x1F800014);
    xport_gte_write_data(5, value);
    xport_gte_execute(0x280030);
    value = xport_gte_read_data(12);
    w_u32(packet + 8, value);
    value = xport_gte_read_data(13);
    w_u32(packet + 12, value);
    value = xport_gte_read_data(14);
    w_u32(0x1F800020, value);
    xport_gte_read_data(8);
    flags = xport_gte_read_control(31);
    xport_gte_read_data(19);
    return flags;
}

uint32 sub_80064d98(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    MATRIX first, second;
    uint32 source, pitch, value, a, b, c, d, e, x, y, z, p, q, r, u, v, w, root, count, storage, packet, level, red, green, blue, raw, sine, cosine, magnitude, base, first_flag, flags, xy[2], degree, tens, digit, i, side, color;
    sint32 step;
    sint64 product;
    FUNCTION_MARKER(0x80064D98, "ACE3.BIN");
    source = (native_state ? native_state->words[2] : r_u32(state + 8));
    raw = r_u16(source);
    w_u16(0x1F80001A, 0);
    pitch = (sint16)raw;
    w_u16(0x1F800018, raw);
    source = (native_state ? native_state->words[2] : r_u32(state + 8));
    value = r_u16(source + 4);
    w_u16(0x1F80001C, value);
    sub_800308f0(0x1F800018, 0, &first, 0);
    source = (native_state ? native_state->words[4] : r_u32(state + 16));
    sub_800308f0(source, 0, &second, 0);
    a = xport_load_le32((uint8 *)&second);
    b = xport_load_le32((uint8 *)&second + 4);
    c = xport_load_le32((uint8 *)&second + 8);
    d = xport_load_le32((uint8 *)&second + 12);
    e = xport_load_le16((uint8 *)&second + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = xport_load_le16((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    c = xport_load_le32((uint8 *)&first + 12);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16((uint8 *)&first + 2);
    b = xport_load_le32((uint8 *)&first + 8);
    c = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&first + 14);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16((uint8 *)&first + 4);
    b = xport_load_le32((uint8 *)&first + 8);
    c = xport_load_le16((uint8 *)&first + 16);
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    xport_store_le32((uint8 *)&first, (x & 0xFFFF) | (p << 16));
    xport_store_le32((uint8 *)&first + 12, (z & 0xFFFF) | (r << 16));
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    xport_store_le32((uint8 *)&first + 4, (u & 0xFFFF) | (y << 16));
    xport_store_le32((uint8 *)&first + 8, (q & 0xFFFF) | (v << 16));
    w = xport_gte_read_data(11);
    xport_store_le32((uint8 *)&first + 16, w);
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
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    product = (sint64)(sint32)pitch * 0x24168E19;
    root = r_u32(0x800BD074);
    count = r_u32(root + 20);
    storage = r_u32(0x800BD078);
    packet = storage + (count << 4) + 984;
    value = (uint32)((sint32)(product >> 32) >> 5) - (uint32)((sint32)pitch >> 31) - 4;
    w_u16(0x1F800018, value * 227);
    for (step = -4; step < 5; ++step)
    {
        level = step < 0 ? step + 5 : 5 - step;
        level = (level << 1) * (level << 1) * 4;
        if ((sint32)level >= 256)
            level = 255;
        red = r_u8(0x800C6D68);
        green = r_u8(0x800C6D69);
        blue = r_u8(0x800C6D6A);
        raw = r_u16(0x1F800018);
        red = red * level >> 8;
        sine = r_s16(0x800104AC + (raw & 0x3FFF) * 2);
        raw = r_u16(0x1F800018);
        sine = (sint32)(0 - sine) >> 4;
        cosine = r_u16(0x800104AC + ((raw & 0x3FFF) + 0x1000) * 2);
        green = green * level >> 8;
        blue = blue * level >> 8;
        cosine = (sint32)(cosine << 16) >> 20;
        w_u16(0x1F800012, sine);
        w_u16(0x1F80000A, sine);
        w_u16(0x1F800002, sine);
        w_u16(0x1F800014, cosine);
        w_u16(0x1F80000C, cosine);
        w_u16(0x1F800004, cosine);
        value = r_s16(0x1F800018);
        if (value == 0)
        {
            magnitude = (sint32)pitch > 0 ? pitch : 0 - pitch;
            if ((sint32)magnitude < 445)
                magnitude = 445;
            base = 0xFFFFFF9C;
            w_u16(0x1F800000, base - (uint32)((sint32)magnitude >> 2));
            value = (sint32)magnitude >> 1;
        }
        else
        {
            magnitude = value - pitch;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            base = 0xFFFFFF38;
            w_u16(0x1F800000, base - (uint32)((sint32)magnitude >> 4));
            value = (sint32)magnitude >> 3;
        }
        value = base - value;
        w_u16(0x1F800008, value);
        value = r_u16(0x1F800008);
        w_u16(0x1F800010, value + 20);
        flags = project_hud_a7388(packet);
        if ((sint32)flags >= 0)
        {
            w_u8(packet + 4, red);
            w_u8(packet + 5, green);
            w_u8(packet + 6, blue);
            root = r_u32(0x800BD074);
            count = r_u32(root + 20);
            packet += 16;
            w_u32(root + 20, count + 1);
        }
        a = r_u16(0x1F800000);
        b = r_u16(0x1F800008);
        b = 0 - b;
        w_u16(0x1F800008, b);
        w_u16(0x1F800000, 0 - a);
        w_u16(0x1F800010, b - 20);
        flags = project_hud_a7388(packet);
        if ((sint32)flags >= 0)
        {
            w_u8(packet + 4, red);
            w_u8(packet + 5, green);
            w_u8(packet + 6, blue);
            root = r_u32(0x800BD074);
            count = r_u32(root + 20);
            packet += 16;
            w_u32(root + 20, count + 1);
        }
        value = r_u16(0x1F800018);
        w_u16(0x1F800018, value + 227);
    }
    w_u16(0x1F800018, pitch);
    w_u16(0x1F800000, 0xFF30);
    w_u16(0x1F800008, 0xFF40);
    w_u16(0x1F800010, 0xFF68);
    raw = r_u16(0x1F800018);
    sine = r_s16(0x800104AC + (raw & 0x3FFF) * 2);
    raw = r_u16(0x1F800018);
    cosine = r_u16(0x800104AC + ((raw & 0x3FFF) + 0x1000) * 2);
    sine = (sint32)(0 - sine) >> 4;
    cosine = (sint32)(cosine << 16) >> 20;
    w_u16(0x1F800002, sine);
    w_u16(0x1F800004, cosine);
    value = r_u16(0x1F800004);
    w_u16(0x1F80000A, sine);
    w_u16(0x1F800012, sine);
    w_u16(0x1F80000C, value);
    w_u16(0x1F800014, value);
    flags = project_hud_a7388(packet);
    if ((sint32)flags >= 0)
    {
        value = r_u8(0x800C6D68);
        w_u8(packet + 4, value);
        value = r_u8(0x800C6D69);
        w_u8(packet + 5, value);
        value = r_u8(0x800C6D6A);
        w_u8(packet + 6, value);
        root = r_u32(0x800BD074);
        count = r_u32(root + 20);
        packet += 16;
        w_u32(root + 20, count + 1);
    }
    first_flag = flags;
    xy[0] = r_u32(0x1F800020);
    a = r_u16(0x1F800000);
    b = r_u16(0x1F800008);
    w_u16(0x1F800000, 0 - a);
    c = r_u16(0x1F800010);
    w_u16(0x1F800008, 0 - b);
    w_u16(0x1F800010, 0 - c);
    flags = project_hud_a7388(packet);
    if ((sint32)flags >= 0)
    {
        value = r_u8(0x800C6D68);
        w_u8(packet + 4, value);
        value = r_u8(0x800C6D69);
        w_u8(packet + 5, value);
        value = r_u8(0x800C6D6A);
        w_u8(packet + 6, value);
        root = r_u32(0x800BD074);
        count = r_u32(root + 20);
        w_u32(root + 20, count + 1);
    }
    xy[1] = r_u32(0x1F800020);
    if ((sint32)(first_flag | flags) < 0)
        return 0;
    root = r_u32(0x800BD074);
    count = r_u32(root + 32);
    storage = r_u32(0x800BD078);
    packet = storage + count * 20 + 2208;
    degree = (sint32)(pitch * 360) / 16384;
    if ((sint32)degree < 0)
    {
        for (side = 0; side < 2; ++side)
        {
            x = (sint16)xy[side];
            y = (sint32)xy[side] >> 16;
            color = r_u8(0x800C6D71);
            sub_8003b7b0(0xFFFFFFFF, x - 9, y - 3, 3, packet, color);
            root = r_u32(0x800BD074);
            count = r_u32(root + 32);
            packet += 20;
            w_u32(root + 32, count + 1);
        }
        degree = 0 - degree;
    }
    tens = (sint32)degree / 10;
    tens %= 10;
    for (i = 0; i < 4; ++i)
    {
        side = i & 1;
        digit = i < 2 ? tens : degree % 10;
        x = (sint16)xy[side];
        y = (sint32)xy[side] >> 16;
        color = r_u8(0x800C6D71);
        sub_8003b7b0(digit, x + (i < 2 ? 0xFFFFFFFC : 1), y - 3, 3, packet, color);
        root = r_u32(0x800BD074);
        count = r_u32(root + 32);
        packet += 20;
        w_u32(root + 32, count + 1);
    }
    return 0;
}

uint32 sub_80055cbc(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055CBC, "ACE3.BIN");
    threshold = sub_80056164(script);
    script += 4;
    value = r_u8(script);
    root = r_u32(0x800BCEB4);
    end = script + value;
    value = r_u32(root + 4);
    cursor = script + 1;
    if ((sint32)value < (sint32)threshold)
    {
        value = r_u8(cursor);
        cursor += value;
    }
    else
        ++cursor;
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055d2c(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055D2C, "ACE3.BIN");
    threshold = r_u8(script);
    ++script;
    value = r_u8(script);
    root = r_u32(0x800BCEB4);
    end = script + value;
    value = r_u8(root + 34);
    cursor = script + 1;
    if (value < threshold)
    {
        value = r_u8(cursor);
        cursor += value;
    }
    else
        ++cursor;
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055d94(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055D94, "ACE3.BIN");
    threshold = r_u8(script);
    ++script;
    value = r_u8(script);
    root = r_u32(0x800BCEB4);
    end = script + value;
    value = r_u8(root + 33);
    cursor = script + 1;
    if (threshold < value)
    {
        value = r_u8(cursor);
        cursor += value;
    }
    else
        ++cursor;
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055dfc(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055DFC, "ACE3.BIN");
    threshold = r_u8(script);
    ++script;
    value = r_u8(script);
    root = r_u32(0x800BCEB4);
    end = script + value;
    value = r_u8(root + 32);
    cursor = script + 1;
    if (value < threshold)
    {
        value = r_u8(cursor);
        cursor += value;
    }
    else
        ++cursor;
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055e64(uint32 script)
{
    uint32 value, root, end, cursor;
    FUNCTION_MARKER(0x80055E64, "ACE3.BIN");
    sub_80056164(script);
    script += 4;
    value = r_u8(script);
    end = script + value;
    root = r_u32(0x800BCEB4);
    value = r_u8(root + 47);
    cursor = script + 1;
    if (value == 1)
        ++cursor;
    else
    {
        value = r_u8(cursor);
        cursor += value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055ed8(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055ED8, "ACE3.BIN");
    value = r_u8(script);
    root = r_u32(0x800BCEB4);
    end = script + value;
    threshold = r_u8(root + 46);
    value = r_u8(root + 45);
    cursor = script + 1;
    if (value < threshold)
        ++cursor;
    else
    {
        value = r_u8(cursor);
        cursor += value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055f3c(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80055F3C, "ACE3.BIN");
    threshold = r_u8(script);
    ++script;
    value = r_u8(script);
    root = r_u32(0x800BCE60);
    end = script + value;
    value = r_u8(root + threshold + 3340);
    cursor = script + 1;
    if (value != 0)
        ++cursor;
    else
    {
        value = r_u8(cursor);
        cursor += value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80055fa4(uint32 script)
{
    uint32 value, end, cursor;
    FUNCTION_MARKER(0x80055FA4, "ACE3.BIN");
    value = r_u8(script);
    end = script + value;
    ++script;
    value = game_random();
    cursor = script + 1;
    if ((value & 1) == 0)
    {
        value = r_u8(script);
        cursor = script + value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80056004(uint32 script)
{
    uint32 value, root, end, cursor, threshold;
    FUNCTION_MARKER(0x80056004, "ACE3.BIN");
    value = r_u8(script);
    ++script;
    threshold = r_u8(script);
    ++script;
    root = r_u32(0x800BCE5C);
    cursor = r_u8(script);
    root = r_u32(root + value * 4);
    end = script + cursor;
    value = r_s16(root + 78);
    cursor = script + 1;
    if ((sint32)value < (sint32)threshold)
    {
        value = r_u8(cursor);
        cursor += value;
    }
    else
        ++cursor;
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_80056080(uint32 script)
{
    uint32 operand, length, root, end, value, cursor;
    FUNCTION_MARKER(0x80056080, "ACE3.BIN");
    operand = r_u8(script);
    ++script;
    length = r_u8(script);
    root = r_u32(0x800BCE54);
    end = script + length;
    value = r_u8(root + 8);
    cursor = script + 1;
    if (operand == value)
        ++cursor;
    else
    {
        value = r_u8(cursor);
        cursor += value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_800560e4(uint32 script)
{
    uint32 operand, length, root, end, value, cursor;
    FUNCTION_MARKER(0x800560E4, "ACE3.BIN");
    operand = r_u8(script);
    ++script;
    length = r_u8(script);
    root = r_u32(0x800BCE60);
    end = script + length;
    value = r_u8(root + 916);
    cursor = script + 1;
    if (operand == value)
        ++cursor;
    else
    {
        value = r_u8(cursor);
        cursor += value;
    }
    sub_80054ebc(cursor);
    return end;
}

uint32 sub_800540e0(void)
{
    uint32 scene, game, state, descriptor, world, objects, object, mode, submode, mission, value, other, flags, offset, i, phase, limit, timer, script, result, draw, coordinate, random, update, packet, ordering;
    FUNCTION_MARKER(0x800540E0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    mode = r_u32(scene + 4);
    submode = r_u32(scene + 8);
    if (mode == 2)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 15 && submode == 0)
        {
            value = r_u32(scene + 12);
            if (value == 0)
                sub_8009423c(3);
        }
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 35)
        {
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 8);
            if (value == 0)
            {
                value = r_u32(scene + 12);
                if (value == 0)
                    sub_8009423c(4);
            }
        }
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        if (mission == 46)
        {
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 12);
            if (value == 0)
            {
                value = r_u32(scene + 8);
                if (value == 0)
                    sub_8009423c(1);
                scene = r_u32(0x800BCE54);
                value = r_u32(scene + 8);
                if (value == 1)
                    sub_8009423c(2);
            }
        }
    }
    if (mode != 4)
        goto final_timer;
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    if ((flags & 0x20000000) == 0)
    {
        for (i = 0; i < 16; ++i)
        {
            descriptor = r_u32(0x800BCEAC);
            state = r_u32(0x800BCEB4);
            value = r_u16(descriptor + (submode << 5) + i * 2 + 344);
            other = r_u32(state + 16);
            if (other == value)
            {
                script = r_u32(descriptor + (submode << 6) + i * 4 + 72);
                sub_80054ebc(script);
            }
        }
        state = r_u32(0x800BCEB4);
        value = r_u8(state + 30);
        if (value != 0)
        {
            other = r_u8(state + 32);
            if (value == other)
            {
                descriptor = r_u32(0x800BCEAC);
                script = r_u32(descriptor + submode * 4 + 40);
                sub_80054ebc(script);
            }
            state = r_u32(0x800BCEB4);
        }
        value = r_u8(state + 31);
        if (value != 0)
        {
            other = r_u8(state + 33);
            if (value == other)
            {
                descriptor = r_u32(0x800BCEAC);
                script = r_u32(descriptor + submode * 4 + 56);
                sub_80054ebc(script);
            }
        }
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        value = 0;
        if (mission == 9)
        {
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 8);
        }
        if (mission != 9 || value == 0)
        {
            state = r_u32(0x800BCEB4);
            value = r_u32(state + 12);
            w_u32(state + 12, value + 1);
        }
        state = r_u32(0x800BCEB4);
        value = r_u32(state + 16);
        other = r_u8(state + 28);
        w_u32(state + 16, value + 1);
        if (other == 0)
        {
            value = r_u32(state + 12);
            w_u32(state + 20, value);
        }
        else if (other == 1)
        {
            value = r_u32(state + 20);
            w_u32(state + 20, (sint32)value > 0 ? value - 1 : 0);
        }
    }
    state = r_u32(0x800BCEB4);
    flags = r_u32(state);
    game = r_u32(0x800BCE60);
    w_u32(state, flags & 0xFFFFFFEF);
    mission = r_s16(game + 908);
    if (mission == 3)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            flags = r_u32(game);
            object = r_u32(0x800C6ED8);
            if ((flags & 0x20000000) != 0)
                goto final_timer;
            value = r_u32(object + 4);
            if ((sint32)value < 4096)
            {
                other = r_u32(state + 4);
                offset = (sint32)value < 256 ? 3 : (sint32)value < 1024 ? 2 : 1;
                w_u32(state + 4, other + offset);
                state = r_u32(0x800BCEB4);
                value = r_u32(state);
                w_u32(state, value | 16);
            }
        }
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    if ((flags & 0x20000000) != 0)
        goto final_timer;
    mission = r_s16(game + 908);
    if (mission == 14 || mission == 53)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            phase = r_u32(scene + 12);
            if (phase == 60)
            {
                if (mission == 14)
                {
                    world = r_u32(0x800BCEB0);
                    value = r_u16(world + 32);
                    state = r_u32(0x800BCEB4);
                    w_u16(state + 24, value - 151);
                }
                game = r_u32(0x800BCE60);
                mission = r_s16(game + 908);
                if (mission == 53)
                {
                    world = r_u32(0x800BCEB0);
                    value = r_u16(world + 32);
                    state = r_u32(0x800BCEB4);
                    w_u16(state + 24, value - 121);
                }
                state = r_u32(0x800BCEB4);
                flags = r_u32(state);
                w_u32(state, flags & 0xFFFFFFFD);
            }
            state = r_u32(0x800BCEB4);
            value = r_u16(state + 24);
            flags = r_u32(state) & 2;
            w_u16(state + 24, value + 1);
            if (flags != 0)
            {
                w_u8(state + 28, 1);
                world = r_u32(0x800BCEB0);
                state = r_u32(0x800BCEB4);
                limit = r_s16(world + 34);
                timer = r_s16(state + 24);
                game = r_u32(0x800BCE60);
                w_u32(state + 20, limit - timer);
                mission = r_s16(game + 908);
                if (mission == 14)
                {
                    limit = r_s16(world + 34);
                    timer = r_s16(state + 24);
                    if (timer == limit - 600)
                    {
                        descriptor = r_u32(0x800BCEAC);
                        script = r_u32(descriptor + 336);
                        sub_80054ebc(script);
                    }
                    world = r_u32(0x800BCEB0);
                    state = r_u32(0x800BCEB4);
                    limit = r_s16(world + 34);
                    timer = r_s16(state + 24);
                    if ((sint32)timer >= (sint32)(limit - 600))
                    {
                        objects = r_u32(0x800BCE5C);
                        object = r_u32(objects);
                        value = r_u32(object + 12);
                        if ((sint32)value > -81920000)
                            sub_8004287c(6);
                    }
                }
                state = r_u32(0x800BCEB4);
                world = r_u32(0x800BCEB0);
                timer = r_s16(state + 24);
                limit = r_s16(world + 34);
                if (timer == limit)
                {
                    game = r_u32(0x800BCE60);
                    mission = r_s16(game + 908);
                    if (mission == 53)
                    {
                        descriptor = r_u32(0x800BCEAC);
                        script = r_u32(descriptor + 336);
                        sub_80054ebc(script);
                    }
                    state = r_u32(0x800BCEB4);
                    flags = r_u32(state);
                    w_u16(state + 24, 0);
                    w_u32(state, flags & 0xFFFFFFFD);
                }
            }
            else
            {
                w_u8(state + 28, 0);
                world = r_u32(0x800BCEB0);
                state = r_u32(0x800BCEB4);
                limit = r_s16(world + 32);
                timer = r_s16(state + 24);
                if (timer == limit - 150)
                {
                    descriptor = r_u32(0x800BCEAC);
                    script = r_u32(descriptor + 328);
                    sub_80054ebc(script);
                }
                state = r_u32(0x800BCEB4);
                world = r_u32(0x800BCEB0);
                timer = r_s16(state + 24);
                limit = r_s16(world + 32);
                if (timer == limit)
                {
                    descriptor = r_u32(0x800BCEAC);
                    script = r_u32(descriptor + 332);
                    sub_80054ebc(script);
                    state = r_u32(0x800BCEB4);
                    game = r_u32(0x800BCE60);
                    flags = r_u32(state);
                    w_u16(state + 24, 0);
                    w_u32(state, flags | 2);
                    mission = r_s16(game + 908);
                    sub_8004ceb0(mission == 53 ? 32 : 17);
                }
            }
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 14)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            state = r_u32(0x800BCEB4);
            flags = r_u32(state);
            if ((flags & 1) == 0)
            {
                draw = flags & 2;
                if (draw == 0)
                {
                    objects = r_u32(0x800BCE5C);
                    object = r_u32(objects);
                    value = r_u32(object + 12);
                    if ((sint32)value > -81920000)
                    {
                        descriptor = r_u32(0x800BCEAC);
                        w_u32(state, flags | 1);
                        script = r_u32(descriptor + 340);
                        sub_80054ebc(script);
                    }
                    state = r_u32(0x800BCEB4);
                    draw = r_u32(state) & 2;
                }
                if (draw != 0)
                {
                    objects = r_u32(0x800BCE5C);
                    object = r_u32(objects);
                    value = r_u32(object + 12);
                    if ((sint32)value > -81920000)
                    {
                        scene = r_u32(0x800BCE54);
                        ordering = r_u32(scene + 16);
                        packet = r_u32(scene + 20);
                        result = sub_80084474(ordering + 4, packet, 765);
                        scene = r_u32(0x800BCE54);
                        w_u32(scene + 20, result);
                    }
                }
            }
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 6)
    {
        objects = r_u32(0x800BCE5C);
        object = r_u32(objects);
        value = r_u32(object + 12);
        if ((sint32)value < -10485760)
        {
            sub_80056518();
            descriptor = r_u32(0x800BCEAC);
            script = r_u32(descriptor + 328);
            sub_80054ebc(script);
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 13)
    {
        state = r_u32(0x800BCEB4);
        flags = r_u32(state);
        if ((flags & 32) != 0)
        {
            value = r_u32(0x800BD010);
            w_u32(0x800BD010, value + 1);
        }
        else
            w_u32(0x800BD010, 0);
        value = r_u32(0x800BD010);
        if (value == 18000)
        {
            descriptor = r_u32(0x800BCEAC);
            script = r_u32(descriptor + 328);
            sub_80054ebc(script);
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 44)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            state = r_u32(0x800BCEB4);
            flags = r_u32(state);
            if ((flags & 1) == 0)
            {
                objects = r_u32(0x800BCE5C);
                object = r_u32(objects);
                value = r_u32(object + 12);
                if ((sint32)value < -6291456)
                    sub_8004287c(4);
                value = r_u32(object + 12);
                if ((sint32)value < -8388608)
                {
                    state = r_u32(0x800BCEB4);
                    flags = r_u32(state);
                    descriptor = r_u32(0x800BCEAC);
                    w_u32(state, flags | 1);
                    script = r_u32(descriptor + 328);
                    sub_80054ebc(script);
                }
                value = r_u32(object + 8);
                if (value + 32767999 <= 0x7147FFE)
                {
                    coordinate = r_u32(object + 16);
                    if ((sint32)coordinate > -73728000 && (sint32)coordinate <= 20479999)
                    {
                        state = r_u32(0x800BCEB4);
                        flags = r_u32(state);
                        descriptor = r_u32(0x800BCEAC);
                        w_u32(state, flags | 1);
                        script = r_u32(descriptor + 332);
                        sub_80054ebc(script);
                    }
                }
            }
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 30)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            phase = r_u32(scene + 12);
            if (phase - 1200 < 0xA8D)
            {
                random = game_random();
                game = r_u32(0x800BCE60);
                flags = r_u32(game);
                w_u32(game, flags | 0x10000000);
                scene = r_u32(0x800BCE54);
                phase = r_u32(scene + 12);
                if (phase == 1200)
                {
                    sub_80056a10();
                    w_u32(0x800BD014, 0);
                }
                else
                {
                    update = 0;
                    if ((sint32)phase < 1320)
                    {
                        value = r_u32(0x800BD014);
                        if ((sint32)value < 2048)
                        {
                            value += 18;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 1440)
                    {
                        value = 2048;
                        update = 1;
                    }
                    else if ((sint32)phase < 3150)
                    {
                        value = (random & 255) + 768;
                        update = 1;
                    }
                    else if ((sint32)phase < 3450)
                    {
                        value = r_u32(0x800BD014);
                        if ((sint32)value < 2048)
                        {
                            value += 8;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 3540)
                    {
                        value = r_u32(0x800BD014);
                        if ((sint32)value < 2048)
                        {
                            value += 16;
                            update = 1;
                        }
                    }
                    else if ((sint32)phase < 3720)
                    {
                        value = 0xFFFFF800;
                        update = 1;
                    }
                    else if ((sint32)phase < 3900)
                    {
                        value = r_u32(0x800BD014);
                        if ((sint32)value < 0)
                        {
                            value += 16;
                            update = 1;
                        }
                    }
                    if (update != 0)
                        w_u32(0x800BD014, value);
                }
                scene = r_u32(0x800BCE54);
                value = r_u32(0x800BD014);
                ordering = r_u32(scene + 16);
                packet = r_u32(scene + 20);
                result = sub_80084474(ordering + 4, packet, value);
                scene = r_u32(0x800BCE54);
                w_u32(scene + 20, result);
            }
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 42)
    {
        objects = r_u32(0x800BCE5C);
        scene = r_u32(0x800BCE54);
        object = r_u32(objects);
        phase = r_u32(scene + 12);
        coordinate = r_u32(object + 12);
        phase &= 31;
        if ((sint32)coordinate >= 0 && phase == 0)
            sub_80076fa4();
        if ((sint32)coordinate > 20479999 && phase == 16)
            sub_80076fa4();
        if ((sint32)coordinate > 32767999 && (phase == 8 || phase == 24))
            sub_80076fa4();
    }
    state = r_u32(0x800BCEB4);
    flags = r_u32(state);
    game = r_u32(0x800BCE60);
    flags &= 0xFFFFFFFB;
    w_u32(state, flags);
    mission = r_s16(game + 908);
    if (mission == 23)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            objects = r_u32(0x800BCE5C);
            object = r_u32(objects);
            value = r_u32(object + 8);
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 155647999)
            {
                value = r_u32(object + 16);
                if ((sint32)value < 0)
                    value = 0 - value;
                if ((sint32)value <= 155647999)
                {
                    value = r_u32(object + 12);
                    if ((sint32)value > -62914560)
                        w_u32(state, flags | 4);
                }
            }
        }
    }
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    if (mission == 55)
    {
        value = r_u32(game + 916);
        objects = r_u32(0x800BCE5C);
        object = r_u32(objects);
        coordinate = r_u32(object + 16);
        if (value == 0)
        {
            if ((sint32)coordinate > 299008000)
            {
                descriptor = r_u32(0x800BCEAC);
                script = r_u32(descriptor + 332);
                sub_80054ebc(script);
            }
        }
        else if ((sint32)coordinate > 876544000)
        {
            descriptor = r_u32(0x800BCEAC);
            script = r_u32(descriptor + 328);
            sub_80054ebc(script);
        }
    }
final_timer:
    if (mode == 2048)
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game);
        if ((flags & 0x20000000) == 0)
        {
            state = r_u32(0x800BCEB4);
            value = r_u8(state + 28);
            if (value == 0)
            {
                other = r_u32(state + 12);
                w_u32(state + 20, other);
            }
            else if (value == 1)
            {
                other = r_u32(state + 20);
                w_u32(state + 20, (sint32)other > 0 ? other - 1 : 0);
            }
            else
                w_u32(state + 20, 0);
        }
    }
    scene = r_u32(0x800BCE54);
    result = r_u32(scene + 4) & 0x102C0E;
    if (result != 0)
        return sub_8005628c();
    return result;
}

uint32 sub_80056a10(void)
{
    FUNCTION_MARKER(0x80056A10, "ACE3.BIN");
    resource_cancel_streams();
    return resource_queue_audio(0x800B2328, 0, 0);
}

uint32 sub_80076fa4(void)
{
    uint32 root, result, objects, object, value;
    FUNCTION_MARKER(0x80076FA4, "ACE3.BIN");
    root = r_u32(0x800BD094);
    result = r_u32(root) & 0x7000;
    if (result != 0)
        return result;
    objects = r_u32(0x800BCE5C);
    object = r_u32(objects);
    value = r_u16(object + 78);
    w_u16(object + 78, value - 1);
    sub_80029588();
    return sub_8006cb44(2);
}

void sub_8009423c(uint32 kind)
{
    uint32 table, count, i, root, object, value;
    FUNCTION_MARKER(0x8009423C, "ACE3.BIN");
    switch (kind)
    {
        case 0:
            table = 0x800B445C;
            count = 7;
            break;
        case 1:
            table = 0x800B445C;
            count = 5;
            break;
        case 2:
            table = 0x800B44B0;
            count = 5;
            break;
        case 3:
            table = 0x800B44EC;
            count = 12;
            break;
        case 4:
            table = 0x800B457C;
            count = 10;
            break;
        default:
            return;
    }
    for (i = 0; i < count; ++i)
    {
        root = r_u32(0x800BCF90);
        object = sub_8007fdb4(root + 14020, 1, 5);
        if (object == 0)
            return;
        value = game_random();
        w_u16(object, i * 2 + (value & 1));
        w_u16(object + 2, 0x7FFF);
        value = r_u32(table);
        w_u32(object + 4, value);
        value = r_u32(table + 4);
        w_u32(object + 8, value);
        value = r_u32(table + 8);
        w_u32(object + 16, 0x80089CD8);
        w_u32(object + 12, value);
        table += 12;
    }
}

uint32 sub_80056fd8(uint32 mode)
{
    uint32 state, value, format, input, output, words;
    FUNCTION_MARKER(0x80056FD8, "ACE3.BIN");
    state = r_u32(0x800BD048);
    value = r_u8(state + 4);
    if (value != 0)
        return 1;
    if (sub_800572c8(state) == 0)
        return 0;
    state = r_u32(0x800BD048);
    w_u8(state + 3, 0);
    state = r_u32(0x800BD048);
    value = r_u8(state);
    format = r_u8(state + 5);
    input = r_u32(state + value * 4 + 8);
    DecDCTin(psx_addr(input, 4 + ((uint32)r_u16(input) << 2)), format | 2);
    state = r_u32(0x800BD048);
    value = r_u8(state + 1);
    words = r_s16(state + 69686);
    output = r_u32(state + value * 4 + 16);
    DecDCTout(psx_addr(output, (words & ~31u) * 4), (sint32)words);
    state = r_u32(0x800BD048);
    sub_80057084(state, mode);
    return 0;
}

uint32 sub_80057640(void)
{
    uint32 game, value, state, settings, scene;
    FUNCTION_MARKER(0x80057640, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game);
    state = r_u32(0x800BCE48);
    w_u32(game, value & 0xEFFFFFFF);
    settings = r_u32(0x800BCD8C);
    w_u32(state + 20, 2);
    value = r_u32(settings + 4);
    if (value != 0)
        sub_8002cc44(255, 0, 0);
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 696);
    arena_set_cursor(value);
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 696);
    sub_800323f8(value);
    w_u16(0x800BD03C, 320);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 0);
    w_u16(0x800BD03E, 240);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 256);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    return sub_80037e50();
}

uint32 sub_80058800(void)
{
    uint32 settings, value, flags, state, id, output, bytes;
    FUNCTION_MARKER(0x80058800, "ACE3.BIN");
    settings = r_u32(0x800BCDA0);
    value = r_u32(settings + 4);
    flags = value != 0 ? 0x3000 : 0x2000;
    state = r_u32(0x800BD054);
    id = r_u32(state + 204);
    output = r_u32(state + 212);
    bytes = r_u32(state + 208);
    return sub_8002c928(id, output, bytes, flags, 0x80058854);
}

uint32 sub_80058854(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80058854, "ACE3.BIN");
    state = r_u32(0x800BD054);
    w_u8(state + 160, 1);
    return 1;
}

uint32 sub_80058864(void)
{
    FUNCTION_MARKER(0x80058864, "ACE3.BIN");
    resource_init_queue(0, 0);
    return 1;
}

uint32 sub_80058888(void)
{
    uint32 state, value;
    FUNCTION_MARKER(0x80058888, "ACE3.BIN");
    state = r_u32(0x800BD054);
    value = r_u8(state + 158);
    return value == 0;
}

uint32 sub_8005889c(uint32 nonblocking)
{
    uint32 state, value, remaining;
    FUNCTION_MARKER(0x8005889C, "ACE3.BIN");
    if (nonblocking != 0)
    {
        state = r_u32(0x800BD054);
        return sub_80058938(state) != 0;
    }
    remaining = 120;
    for (;;)
    {
        state = r_u32(0x800BD054);
        value = sub_80058938(state);
        if (value != 0)
            return 1;
        sub_8002c4dc();
        VSync(0);
        --remaining;
        if ((sint32)remaining < 0)
        {
            state = r_u32(0x800BD054);
            w_u8(state + 160, 1);
            return 1;
        }
    }
}

uint32 sub_800573e8(void)
{
    uint32 game, index, first, second, table, settings, value, state, scene, address, allocation, mode, argument;
    FUNCTION_MARKER(0x800573E8, "ACE3.BIN");
    DrawSync(0);
    game = r_u32(0x800BCE60);
    index = r_u8(game + 936);
    if (index == 7 || index == 16 || index == 12)
    {
        scene = r_u32(0x800BCE54);
        argument = r_u32(scene + 8);
        return sub_80032f18(0x20000, argument);
    }
    first = r_u8(game + 924);
    second = r_u8(game + 935);
    if ((first & second) != 0)
    {
        index = r_u8(game + 936);
        table = r_u32(0x800BCD94);
        value = r_s16(table + index * 8 + 2);
        settings = r_u32(0x800BCD8C);
        w_u32(settings + 4, value == 1);
    }
    else
    {
        settings = r_u32(0x800BCD8C);
        w_u32(settings + 4, 0);
    }
    game = r_u32(0x800BCE60);
    state = r_u32(0x800BCE48);
    value = r_u32(game);
    scene = r_u32(0x800BCE54);
    w_u32(game, value | 0x10000000);
    w_u32(state + 20, 0);
    address = r_u32(scene + 696);
    arena_set_cursor(address);
    allocation = arena_alloc(68);
    w_u32(0x800BD044, allocation);
    allocation = arena_alloc(1024);
    state = r_u32(0x800BD044);
    w_u32(state + 4, allocation);
    allocation = arena_alloc(1024);
    state = r_u32(0x800BD044);
    game = r_u32(0x800BCE60);
    w_u32(state + 8, allocation);
    w_u32(state, 0);
    w_u16(state + 36, 0);
    w_u16(state + 38, 0);
    w_u16(state + 40, 0);
    w_u16(state + 42, 256);
    w_u16(state + 44, 320);
    w_u16(state + 46, 240);
    w_u16(state + 48, 0);
    w_u16(state + 50, 32);
    index = r_u8(game + 936);
    table = r_u32(0x800BCD94);
    value = r_s16(table + index * 8);
    w_u32(state + 56, 0);
    w_u32(state + 60, 0);
    w_u32(state + 64, 120);
    w_u32(state + 52, value);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 0);
    w_u16(0x800BD03C, 320);
    w_u16(0x800BD03E, 240);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 256);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    settings = r_u32(0x800BCD8C);
    value = r_u32(settings + 4);
    if (value != 0)
        sub_8002cc44(7, 0, 0);
    game = r_u32(0x800BCE60);
    value = r_s16(game + 940);
    mode = 0x8000;
    if (value != 0)
    {
        index = r_u8(game + 936);
        if (index == 12 || index == 27)
            mode = 0x10000;
    }
    else
    {
        index = r_u8(game + 936);
        if (index - 29 < 2)
            mode = 0x10000;
    }
    scene = r_u32(0x800BCE54);
    argument = r_u32(scene + 8);
    return sub_80032f18(mode, argument);
}

uint32 sub_80058938(uint32 state)
{
    uint32 payload, header, value, other, format, phase, kind, variant, last, i, j, cursor, offset, bank, destination, rectangle, base, row, column;
    FUNCTION_MARKER(0x80058938, "ACE3.BIN");
    if (cd_stream_get_frame(&payload, &header, 0, 0) != 0)
        return 0;
    value = r_u16(header + 6);
    if (value == 0)
        w_u8(state + 160, 1);
    value = r_u8(header + 12);
    kind = r_u8(header + 15);
    format = value & 15;
    phase = value >> 4;
    value = r_u8(header + 19);
    variant = r_u8(header + 13);
    last = value & 15;
    if (kind < 2)
    {
        cursor = payload + 20;
        for (i = 0; i < 256; ++i, cursor += 2)
        {
            value = r_s16(cursor);
            other = r_u16(cursor);
            if (value != 0)
                w_u16(cursor, other | 0x8000);
        }
    }
    else
    {
        cursor = payload + 2292;
        for (i = 573; i < 1125; ++i, cursor += 4)
        {
            value = r_u32(cursor + 4);
            w_u32(cursor, value);
        }
    }
    if (kind == last + 15)
    {
        value = r_u8(state + 158);
        w_u8(state + 158, value == 0);
        if (variant == 1)
            w_u8(state + 158, 0);
        for (i = 0; i < 5; ++i)
        {
            offset = i * 14;
            bank = r_u8(state + 158);
            w_u8(state + bank * 70 + offset + 18, 0);
            bank = r_u8(state + 158);
            w_u16(state + bank * 70 + offset + 20, 0xFFFF);
            bank = r_u8(state + 158);
            w_u16(state + bank * 70 + offset + 22, 0xFFFF);
            for (j = 0; j < 4; ++j)
            {
                bank = r_u8(state + 158);
                w_u16(state + bank * 70 + offset + j * 2 + 24, 0);
            }
        }
    }
    if (kind - 2 < 16)
    {
        bank = r_u8(state + 158);
        value = r_u8(header + 19);
        w_u8(state + bank * 20 + 179 - kind, value >> 4);
        if (kind < 6)
        {
            bank = r_u8(state + 158);
            value = r_u8(header + 19);
            w_u8(state + bank * 20 + 183 - kind, value >> 4);
        }
    }
    value = r_u8(header + 16);
    if ((value & 15) == 1)
    {
        offset = (value >> 4) * 14;
        bank = r_u8(state + 158);
        value = r_u8(header + 17);
        w_u8(state + bank * 70 + offset + 18, value & 15);
        bank = r_u8(state + 158);
        value = r_u16(header + 20);
        w_u16(state + bank * 70 + offset + 20, value);
        bank = r_u8(state + 158);
        value = r_u16(header + 22);
        w_u16(state + bank * 70 + offset + 22, value);
        for (i = 0; i < 4; ++i)
        {
            value = r_u16(header + 24 + i * 2);
            bank = r_u8(state + 158);
            w_u16(state + bank * 70 + offset + i * 2 + 24, value);
        }
    }
    if (format == 0)
    {
        rectangle = 0x800C7800 + kind * 8;
        if (kind >= 2)
        {
            bank = r_u8(state + 158);
            base = bank != 0 ? 896 : 768;
            column = kind - 2;
            row = (sint32)column < 0 ? kind + 1 : column;
            row = (sint32)row >> 2;
            w_u16(rectangle, base - ((column - row * 4 - 3) << 5));
            w_u16(rectangle + 2, 0 - (row - 3) * 70);
            w_u16(rectangle + 4, 32);
            w_u16(rectangle + 6, 70);
        }
        else
        {
            w_u16(rectangle, 768);
            w_u16(rectangle + 4, 256);
            w_u16(rectangle + 6, 1);
            bank = r_u8(state + 158);
            if (kind == 0)
                w_u16(0x800C7802, bank != 0 ? 283 : 281);
            else
                w_u16(0x800C780A, bank != 0 ? 282 : 280);
        }
        destination = payload + 20;
        w_u32(0x800C78A0 + kind * 4, destination);
        LoadImagePSX((PSX_RECT *)psx_addr(rectangle, 8), (uint32 *)psx_addr(destination, kind < 2 ? 512 : 4480));
    }
    if (phase == 1)
        w_u8(state, variant == 1 ? 2 : 1);
    cd_stream_free_frame(header);
    return 1;
}

uint32 sub_800586ac(uint32 descriptor, const uint32 *native_descriptor)
{
    uint32 state, other, value, i, j, bank, offset;
    FUNCTION_MARKER(0x800586AC, "ACE3.BIN");
    state = r_u32(0x800BD054);
    value = native_descriptor ? native_descriptor[1] : r_u32(descriptor + 4);
    w_u32(state + 204, value);
    value = native_descriptor ? native_descriptor[2] : r_u32(descriptor + 8);
    w_u32(state + 208, value);
    value = native_descriptor ? native_descriptor[3] : r_u32(descriptor + 12);
    w_u32(state + 212, value);
    value = native_descriptor ? native_descriptor[0] & 255 : r_u8(descriptor);
    w_u8(state + 161, value);
    state = r_u32(0x800BD054);
    w_u8(state + 158, 0);
    state = r_u32(0x800BD054);
    w_u8(state + 159, 0);
    state = r_u32(0x800BD054);
    w_u8(state + 160, 0);
    state = r_u32(0x800BD054);
    w_u8(state + 10, 0);
    state = r_u32(0x800BD054);
    w_u8(state + 1, 0);
    other = r_u32(0x800BD054);
    w_u32(state + 12, 0);
    w_u32(state + 4, 0);
    w_u8(other, 0);
    state = r_u32(0x800BD054);
    w_u8(state + 16, 0);
    state = r_u32(0x800BD054);
    w_u16(state + 8, 0);
    for (bank = 0; bank < 2; ++bank)
    {
        for (i = 0; i < 5; ++i)
        {
            offset = bank * 70 + i * 14;
            state = r_u32(0x800BD054);
            w_u8(state + offset + 18, 0);
            state = r_u32(0x800BD054);
            w_u16(state + offset + 20, 0);
            w_u16(state + offset + 22, 0);
            for (j = 0; j < 4; ++j)
                w_u16(state + offset + j * 2 + 24, 0xFFFF);
        }
    }
    for (bank = 0; bank < 2; ++bank)
        for (i = 0; i < 20; ++i)
        {
            state = r_u32(0x800BD054);
            w_u8(state + bank * 20 + i + 162, 0);
        }
    return 0;
}

uint32 sub_8005860c(uint32 resource, uint32 kind, uint32 buffer, uint32 bytes)
{
    uint32 descriptor[4], rounded, end;
    FUNCTION_MARKER(0x8005860C, "ACE3.BIN");
    w_u32(0x800BD054, buffer);
    rounded = ((bytes + 3) >> 2) << 2;
    descriptor[3] = buffer + 216;
    end = descriptor[3] + rounded;
    descriptor[1] = resource;
    descriptor[2] = rounded;
    descriptor[0] = kind;
    sub_800586ac(0, descriptor);
    resource_init_queue(0, 0);
    return end;
}

uint32 sub_800582f8(void)
{
    PSX_RECT rectangle;
    uint32 state, mode, game, value, stream, settings, buffer, resource, kind, i;
    FUNCTION_MARKER(0x800582F8, "ACE3.BIN");
    state = r_u32(0x800BD050);
    mode = r_u32(state);
    switch (mode)
    {
        case 0:
            buffer = arena_peek(0);
            state = r_u32(0x800BD050);
            for (i = 0; i < 8; ++i)
                r_s16(state + 60 + i * 2);
            resource = r_u32(state + 80);
            kind = r_u32(state + 84);
            sub_8005860c(resource, kind, buffer, 18368);
            sub_80058800();
            state = r_u32(0x800BD050);
            w_u32(state, 1);
            break;
        case 1:
            game = r_u32(0x800BCE60);
            value = r_u32(game + 4);
            if (value == 0)
            {
                w_u32(game + 4, 0);
                if (sub_8005889c(1) != 0)
                {
                    state = r_u32(0x800BD050);
                    w_u32(state, 2);
                }
                else
                {
                    state = r_u32(0x800BD050);
                    value = r_u32(state + 96) - 1;
                    w_u32(state + 96, value);
                    if ((sint32)value <= 0)
                    {
                        stream = r_u32(0x800BD054);
                        w_u8(stream + 160, 1);
                    }
                    stream = r_u32(0x800BD054);
                    value = r_u8(stream + 160);
                    if (value != 0)
                    {
                        settings = r_u32(0x800BCDA0);
                        value = r_u32(settings + 4);
                        if (value != 0)
                            sub_8002cc44(255, 0, 0);
                        state = r_u32(0x800BD050);
                        w_u32(state, 3);
                    }
                }
            }
            stream = r_u32(0x800BD054);
            value = r_u32(stream + 4);
            w_u32(stream + 4, value + 1);
            break;
        case 2:
            game = r_u32(0x800BCE60);
            value = r_u32(game + 300);
            if ((value & 64) != 0)
            {
                stream = r_u32(0x800BD054);
                w_u8(stream + 160, 1);
            }
            if (sub_8005911c() != 0)
            {
                state = r_u32(0x800BD050);
                stream = r_u32(0x800BD054);
                w_u32(state, 3);
                w_u32(stream + 4, 0);
            }
            break;
        case 3:
            sub_80058864();
            sub_80058888();
            rectangle.w = 320;
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.h = 480;
            ClearImage(&rectangle, 0, 0, 0);
            game = r_u32(0x800BCE60);
            value = r_u32(game + 4);
            if (value == 0)
            {
                state = r_u32(0x800BD050);
                w_u32(state, 4);
            }
            break;
        case 4:
            rectangle.w = 320;
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.h = 480;
            ClearImage(&rectangle, 0, 0, 0);
            state = r_u32(0x800BD050);
            w_u32(state, 5);
            break;
        case 5:
            state = r_u32(0x800BD050);
            w_u32(state, 6);
            break;
        case 6:
            game = r_u32(0x800BCE60);
            value = r_u32(game);
            state = r_u32(0x800BCE48);
            w_u32(game, value & 0xEFFFFFFF);
            value = r_u32(state);
            w_u32(state + 20, 2);
            stream = r_u32(0x800BD050);
            w_u32(state + 4, 0);
            w_u32(state, value | 5);
            w_u32(stream, 7);
            break;
        case 7:
            state = r_u32(0x800BD050);
            w_u32(state, 8);
            break;
        case 8:
            state = r_u32(0x800BD050);
            w_u32(state, 9);
            break;
        case 9:
            return 1;
        default:
            break;
    }
    return 0;
}

uint32 sub_8005911c(void)
{
    uint32 rectangle[2], stream, other, state, value, packet, ordering, time, phase, intensity, result;
    FUNCTION_MARKER(0x8005911C, "ACE3.BIN");
    stream = r_u32(0x800BD054);
    value = r_u8(stream);
    if (value != 0)
    {
        if (value == 1)
        {
            value = r_u8(stream + 159);
            w_u8(stream + 159, value ^ 1);
            stream = r_u32(0x800BD054);
            value = r_u16(stream + 8);
            w_u16(stream + 8, value + 1);
        }
        else
        {
            w_u8(stream + 159, 0);
            stream = r_u32(0x800BD054);
            w_u16(stream + 8, 0);
            w_u8(stream + 1, 1);
        }
        stream = r_u32(0x800BD054);
        w_u8(stream, 0);
        stream = r_u32(0x800BD054);
        w_u8(stream + 16, 0);
        other = r_u32(0x800BD054);
        w_u32(stream + 4, 0);
        w_u8(other + 10, 0);
    }
    stream = r_u32(0x800BD054);
    value = r_u8(stream + 1);
    if (value == 1)
    {
        state = r_u32(0x800BD050);
        packet = r_u32(state + 52);
        ordering = r_u32(state + 56);
        result = sub_800592e0(stream, packet, ordering);
    }
    else
    {
        stream = r_u32(0x800BD054);
        r_masked_u32(0x800BCDA4, 15);
        rectangle[0] = r_masked_u32(0x800BCDA4, 15);
        r_masked_u32(0x800BCDA8, 15);
        rectangle[1] = r_masked_u32(0x800BCDA8, 15);
        time = r_u32(stream + 4);
        phase = time - 30;
        if (phase < 181)
        {
            if (phase < 64)
                intensity = 4096 - (phase << 6);
            else if (time - 94 < 52)
                intensity = 0;
            else if (time - 146 < 64)
                intensity = (time - 146) << 6;
            else
                intensity = 4096;
            if ((sint32)intensity < 0)
                intensity = 0;
            state = r_u32(0x800BD050);
            ordering = r_u32(state + 56);
            packet = r_u32(state + 52);
            result = sub_800846cc(ordering + 12, packet, 0, intensity, rectangle);
            state = r_u32(0x800BD050);
            w_u32(state + 52, result);
        }
        state = r_u32(0x800BD050);
        packet = r_u32(state + 52);
        ordering = r_u32(state + 56);
        result = sub_8005a2c4(packet, ordering);
    }
    state = r_u32(0x800BD050);
    w_u32(state + 52, result);
    stream = r_u32(0x800BD054);
    sub_80058938(stream);
    stream = r_u32(0x800BD054);
    result = r_u8(stream + 160);
    value = r_u32(stream + 4);
    w_u32(stream + 4, value + 1);
    return (result ^ 1) == 0;
}

uint32 sub_800592e0(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 index, bank, record, first, second, time, mode, value;
    FUNCTION_MARKER(0x800592E0, "ACE3.BIN");
    index = r_u8(state + 16);
    bank = r_u8(state + 159);
    record = state + bank * 70 + index * 14;
    first = r_s16(record + 22);
    second = r_s16(record + 30);
    time = r_u32(state + 4);
    if (time == first + second)
    {
        index = r_u8(state + 16);
        w_u8(state + 10, 0);
        w_u32(state + 12, 0);
        w_u8(state + 16, index + 1);
    }
    index = r_u8(state + 16);
    bank = r_u8(state + 159);
    record = state + bank * 70 + index * 14;
    value = r_s16(record + 20);
    time = r_u32(state + 4);
    if (time == value)
    {
        value = r_u8(record + 18);
        w_u32(state + 12, 0);
        w_u8(state + 10, value);
    }
    mode = r_u8(state + 10);
    switch (mode)
    {
        case 0:
            packet = sub_80059488(state, packet, ordering);
            break;
        case 1:
            packet = sub_80059488(state, packet, ordering);
            packet = sub_800598bc(state, packet, ordering);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            packet = sub_80059488(state, packet, ordering);
            packet = sub_80059bd8(state, packet, ordering);
            break;
        case 6:
            packet = sub_80059e1c(state, packet, ordering);
            break;
        case 7:
            packet = sub_8005a034(state, packet, ordering);
            break;
        case 8:
        case 9:
            packet = sub_80059488(state, packet, ordering);
            break;
        default:
            break;
    }
    if (mode - 1 < 9)
    {
        value = r_u32(state + 12);
        w_u32(state + 12, value + 1);
    }
    return sub_8005a2c4(packet, ordering);
}

uint32 sub_800846cc(uint32 ordering, uint32 packet, uint32 rectangle, uint32 intensity, const uint32 *native_rectangle)
{
    uint32 width, height, xy, wh, tail, carrier, value, address, lane, shift, i;
    FUNCTION_MARKER(0x800846CC, "ACE3.BIN");
    width = native_rectangle ? (uint32)(sint32)(sint16)native_rectangle[1] : r_s16(rectangle + 4);
    if ((sint32)width <= 0)
        return packet;
    height = native_rectangle ? (uint32)((sint32)native_rectangle[1] >> 16) : r_s16(rectangle + 6);
    if ((sint32)height <= 0)
        return packet;
    w_u32(packet, ((packet + 16) & 0xFFFFFF) | 0x03000000);
    w_u32(packet + 4, 0xE1000218);
    w_u32(packet + 8, 0xE20C6318);
    w_u32(packet + 12, 0);
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, intensity);
    xy = native_rectangle ? native_rectangle[0] : r_u32(rectangle);
    wh = native_rectangle ? native_rectangle[1] : r_u32(rectangle + 4);
    tail = sub_80084318(packet + 16, 0x4696, xy, wh, &carrier);
    w_u32(tail + 4, 0xE1000220);
    w_u32(tail, 0x03000000);
    w_u32(tail + 8, 0xE2000000);
    w_u32(tail + 12, 0);
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    value = (carrier & ((1 << shift) - 1)) | (r_u32(address & 0xFFFFFFFC) << shift);
    address = tail + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    value = packet << 8;
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    return tail + 16;
}

uint32 sub_80059488(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 mode, root, index, bank, record, first, second, numerator, divisor, quotient, intensity, period, amplitude, time, sample, square, color, texture, clut, i, row, height, x, y, uv_y, value;
    FUNCTION_MARKER(0x80059488, "ACE3.BIN");
    texture = 0;
    clut = 0;
    intensity = 128;
    mode = r_u8(state + 10);
    if (mode == 1)
    {
        root = r_u32(0x800BD054);
        index = r_u8(root + 16);
        bank = r_u8(root + 159);
        record = state + bank * 70 + index * 14;
        first = r_s16(record + 22);
        second = r_s16(record + 20);
        time = r_u32(state + 12);
        divisor = first - second - 1;
        numerator = 0 - (time << 7);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = (uint32)((sint32)numerator / (sint32)divisor) + 128;
        if ((sint16)intensity >= 129)
            intensity = 128;
        if ((sint16)intensity < 0)
            intensity = 0;
        mode = r_u8(state + 10);
    }
    if (mode == 8)
    {
        index = r_u8(state + 16);
        bank = r_u8(state + 159);
        record = state + bank * 70 + index * 14;
        amplitude = r_u16(record + 26);
        first = r_s16(record + 24);
        period = r_u16(record + 24);
        if (first == 0)
        {
            period = 16;
            amplitude = 8;
        }
        period = (uint32)(sint32)(sint16)period;
        time = r_u32(state + 12);
        sample = 0;
        if ((sint32)time < (sint32)(period << 1))
            sample = r_u16(state + 12);
        sample = (uint32)(sint32)(sint16)sample;
        amplitude = (uint32)(sint32)(sint16)amplitude;
        square = sample * sample;
        numerator = square * (0 - amplitude);
        divisor = period * period;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)numerator / (sint32)divisor;
        numerator = sample * (amplitude << 1);
        divisor = period;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = quotient + (uint32)((sint32)numerator / (sint32)divisor) + 128;
        if ((sint16)intensity >= 256)
            intensity = 255;
        if ((sint16)intensity < 0)
            intensity = 0;
    }
    intensity = (uint32)(sint32)(sint16)intensity;
    color = (intensity << 16) | (intensity << 8) | intensity;
    for (i = 0; i < 20; ++i, packet += 24)
    {
        row = i >> 2;
        height = row == 3 ? 46 : row == 4 ? 24 : 70;
        bank = r_u8(state + 159);
        if (bank == 0)
        {
            clut = i < 16 ? 140 : 156;
            value = r_u8(state + i + 162);
            texture = value != 0 ? 0x4670 : 0x4630;
        }
        else if (bank == 1)
        {
            clut = i < 16 ? 142 : 158;
            value = r_u8(state + i + 182);
            texture = value != 0 ? 0x46F0 : 0x46B0;
        }
        x = ((i - row * 4) << 6) | 32;
        if (row == 3)
        {
            y = 310;
            uv_y = 210;
        }
        else if (row == 4)
        {
            y = 356;
            uv_y = 0;
        }
        else
        {
            uv_y = height * row;
            y = uv_y + 100;
        }
        w_u32(packet + 12, (y << 16) | (uint32)(sint32)(sint16)x);
        w_u32(packet + 20, (height << 16) | 64);
        w_u32(packet + 8, color);
        w_u8(packet + 11, 100);
        w_u32(packet + 4, clut);
        w_u32(packet + 16, (texture << 16) | ((uv_y & 255) << 8) | (((i - row * 4) << 6) & 255));
        w_u8(packet + 7, 0xE1);
        w_u8(packet + 6, 0);
        value = r_u32(ordering + 12);
        w_u32(packet, value | 0x05000000);
        w_u32(ordering + 12, packet & 0xFFFFFF);
    }
    return packet;
}

uint32 sub_800598bc(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 root, index, bank, record, first, second, time, divisor, numerator, intensity, color, texture, clut, i, row, height, x, y, uv_y, value, head, tag;
    FUNCTION_MARKER(0x800598BC, "ACE3.BIN");
    root = r_u32(0x800BD054);
    index = r_u8(root + 16);
    bank = r_u8(root + 159);
    record = state + bank * 70 + index * 14;
    first = r_s16(record + 22);
    second = r_s16(record + 20);
    time = r_u32(state + 12);
    divisor = first - second - 1;
    numerator = time << 7;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    intensity = (sint32)numerator / (sint32)divisor;
    texture = 0;
    clut = 0;
    bank = r_u8(state + 159) ^ 1;
    if ((sint16)intensity >= 129)
        intensity = 128;
    if ((sint16)intensity < 0)
        intensity = 0;
    intensity = (uint32)(sint32)(sint16)intensity;
    color = (intensity << 16) | (intensity << 8) | intensity | 0x66000000;
    for (i = 0; i < 20; ++i, packet += 28)
    {
        row = i >> 2;
        height = row == 3 ? 46 : row == 4 ? 24 : 70;
        if (bank == 0)
        {
            clut = i < 16 ? 172 : 188;
            value = r_u8(state + i + 162);
            texture = value != 0 ? 0x4670 : 0x4630;
        }
        else if (bank == 1)
        {
            clut = i < 16 ? 174 : 190;
            value = r_u8(state + i + 182);
            texture = value != 0 ? 0x46F0 : 0x46B0;
        }
        x = ((i - row * 4) << 6) | 32;
        if (row == 3)
        {
            y = 310;
            uv_y = 210;
        }
        else if (row == 4)
        {
            y = 356;
            uv_y = 0;
        }
        else
        {
            uv_y = height * row;
            y = uv_y + 100;
        }
        w_u32(packet + 8, (y << 16) | (uint32)(sint32)(sint16)x);
        w_u32(packet + 4, color);
        w_u32(packet + 12, (texture << 16) | ((uv_y & 255) << 8) | (((i - row * 4) << 6) & 255));
        value = r_u8(packet + 7);
        w_u32(packet + 16, (height << 16) | 64);
        w_u8(packet + 7, value | 2);
        head = r_u32(ordering + 8);
        w_u32(packet, head | 0x04000000);
        w_u32(ordering + 8, packet & 0xFFFFFF);
        w_u8(packet + 23, 1);
        w_u32(packet + 24, clut | 0xE1000200);
        tag = r_u32(packet + 20);
        head = r_u32(ordering + 8);
        w_u32(packet + 20, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering + 8);
        w_u32(ordering + 8, (head & 0xFF000000) | ((packet + 20) & 0xFFFFFF));
    }
    return packet;
}

uint32 sub_80059bd8(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 index, bank, record, start, offset, end, value, divisor, mode, rate, initial, time, numerator, intensity, head, tag, blend;
    FUNCTION_MARKER(0x80059BD8, "ACE3.BIN");
    index = r_u8(state + 16);
    bank = r_u8(state + 159);
    record = state + bank * 70 + index * 14;
    start = r_u16(record + 20);
    offset = r_u16(record + 24);
    end = r_u16(record + 26);
    start += offset;
    value = r_s16(record + 26);
    if (value == 0)
        end = r_u16(record + 22);
    divisor = (uint32)(sint32)(sint16)end - (uint32)(sint32)(sint16)start;
    mode = r_u8(state + 10);
    rate = 0;
    initial = 0;
    if (mode == 2 || mode == 4)
        rate = 255;
    else if (mode == 3 || mode == 5)
    {
        rate = 0xFFFFFF01;
        initial = 255;
    }
    time = r_u32(state + 4);
    if ((sint32)time < (sint16)start)
        intensity = initial;
    else
    {
        time = r_u32(state + 12);
        numerator = time * (uint32)(sint32)(sint16)rate;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = initial + (uint32)((sint32)numerator / (sint32)divisor);
    }
    if ((sint16)intensity >= 256)
        intensity = 255;
    else if ((sint16)intensity < 0)
        intensity = 0;
    intensity &= 255;
    w_u32(packet + 4, (intensity << 16) | (intensity << 8) | intensity | 0x62000000);
    w_u32(packet + 8, 0x00640020);
    w_u32(packet + 12, 0x01180100);
    head = r_u32(ordering + 4);
    w_u32(packet, head | 0x03000000);
    w_u32(ordering + 4, packet & 0xFFFFFF);
    mode = r_u8(state + 10);
    packet += 16;
    blend = mode >= 2 && mode < 4 ? 32 : mode >= 4 && mode < 6 ? 64 : 0;
    w_u8(packet + 3, 1);
    w_u32(packet + 4, blend | 0xE1000200);
    tag = r_u32(packet);
    head = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering + 4);
    w_u32(ordering + 4, (head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 8;
}

uint32 sub_80059e1c(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 index, bank, record, divisor, width, height, x, y, texture, clut, right, bottom, tag, head;
    FUNCTION_MARKER(0x80059E1C, "ACE3.BIN");
    index = r_u8(state + 16);
    bank = r_u8(state + 159);
    record = state + bank * 70 + index * 14;
    divisor = r_s16(record + 28);
    if (divisor == 0)
        xport_mips_break(7);
    width = 256 / (sint32)divisor;
    if (divisor == 0)
        xport_mips_break(7);
    height = 280 / (sint32)divisor;
    x = r_u8(record + 24) - (uint32)((sint16)width / 2);
    y = r_u8(record + 26) - (uint32)((sint16)height / 2);
    texture = 0;
    clut = 0;
    if (bank == 0)
    {
        clut = 140;
        texture = 0x4630;
    }
    else if (bank == 1)
    {
        clut = 142;
        texture = 0x46B0;
    }
    w_u32(packet + 4, 0x00808080);
    w_u32(packet + 8, 0x00640020);
    y &= 255;
    x &= 255;
    w_u32(packet + 12, (texture << 16) | (y << 8) | x);
    right = x + (uint32)(sint32)(sint16)width - 1;
    w_u32(packet + 16, 0x00640120);
    w_u32(packet + 20, (clut << 16) | (y << 8) | right);
    bottom = (y + (uint32)(sint32)(sint16)height - 1) << 8;
    w_u8(packet + 3, 9);
    w_u32(packet + 32, 0x017C0120);
    tag = r_u32(packet);
    w_u32(packet + 24, 0x017C0020);
    w_u32(packet + 28, bottom | x);
    w_u32(packet + 36, bottom | right);
    w_u8(packet + 7, 44);
    head = r_u32(ordering + 12);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering + 12);
    w_u32(ordering + 12, (head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 40;
}

uint32 sub_8005a034(uint32 state, uint32 packet, uint32 ordering)
{
    uint32 time, texture, clut, uv_y, screen_y, height, uv_height, count, left, right, i, bank, y, bottom, uv, tag, head;
    FUNCTION_MARKER(0x8005A034, "ACE3.BIN");
    time = r_u32(state + 12);
    texture = 0;
    clut = 0;
    screen_y = 100;
    uv_y = time << 2;
    if ((sint32)(uv_y & 255) + (sint16)(280 - (time << 3)) < 256)
    {
        count = 1;
        height = 280;
        uv_height = 24 - (time << 3);
    }
    else
    {
        count = 2;
        height = uv_y + 256;
        uv_height = ~uv_y;
    }
    left = (time * 3) & 255;
    right = left + ((~(time * 6)) & 255);
    for (i = 0; i < count; ++i, packet += 40)
    {
        if (i == 1)
        {
            time = r_u32(state + 12) * 4;
            uv_y = 0;
            screen_y = time + 356;
            height = 24 - time;
            uv_height = height;
        }
        bank = r_u8(state + 159);
        if (bank == 0)
        {
            clut = i == 0 ? 140 : 156;
            texture = 0x4630;
        }
        else if (bank == 1)
        {
            clut = i == 0 ? 142 : 158;
            texture = 0x46B0;
        }
        y = (uint32)(sint32)(sint16)screen_y;
        w_u32(packet + 8, (y << 16) | 32);
        uv = (uv_y & 255) << 8;
        w_u32(packet + 12, (texture << 16) | uv | left);
        w_u32(packet + 20, (clut << 16) | uv | right);
        bottom = (y + (uint32)(sint32)(sint16)height) << 16;
        w_u32(packet + 24, bottom | 32);
        uv = ((uv_y & 255) + (uv_height & 255)) << 8;
        w_u32(packet + 28, uv | left);
        w_u8(packet + 3, 9);
        w_u32(packet + 4, 0x00808080);
        w_u32(packet + 16, (y << 16) | 288);
        w_u32(packet + 32, bottom | 288);
        w_u32(packet + 36, uv | right);
        w_u8(packet + 7, 44);
        tag = r_u32(packet);
        head = r_u32(ordering + 12);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering + 12);
        w_u32(ordering + 12, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    return packet;
}

uint32 sub_8005a2c4(uint32 packet, uint32 ordering)
{
    uint8 data[304];
    uint32 root, time, side, i, a, b, c, d, value, high, selector, noise, sample, phase, offset, first, second, x, y, next, base, curve, curve_next, segment, head, tag;
    sint64 product;
    FUNCTION_MARKER(0x8005A2C4, "ACE3.BIN");
    for (i = 0; i < 144; i += 16)
    {
        a = r_u32(0x80024420 + i);
        b = r_u32(0x80024424 + i);
        c = r_u32(0x80024428 + i);
        d = r_u32(0x8002442C + i);
        xport_store_le32(data + i, a);
        xport_store_le32(data + i + 4, b);
        xport_store_le32(data + i + 8, c);
        xport_store_le32(data + i + 12, d);
    }
    root = r_u32(0x800BCE60);
    r_masked_u32(0x800244B0, 15);
    a = r_masked_u32(0x800244B0, 15);
    r_masked_u32(0x800244B4, 15);
    b = r_masked_u32(0x800244B4, 15);
    xport_store_le32(data + 144, a);
    xport_store_le32(data + 148, b);
    time = r_u32(root + 36);
    if ((sint32)time % 80 == 79)
    {
        for (side = 0; side < 4; ++side)
            for (i = 0; i < 38; ++i)
            {
                value = game_random() * 8;
                product = (sint64)(sint32)value * (sint32)0x80010003;
                high = (uint32)(product >> 32);
                sample = (uint32)((sint32)(high + value) >> 14) - (uint32)((sint32)value >> 31);
                w_u8(0x800C78F0 + side * 38 + i, sample);
                value = game_random() << 2;
                product = (sint64)(sint32)value * (sint32)0x80010003;
                high = (uint32)(product >> 32);
                sample = (uint32)((sint32)(high + value) >> 14) - (uint32)((sint32)value >> 31);
                w_u32(0x800C7990 + side * 4, sample);
            }
    }
    root = r_u32(0x800BCE60);
    time = r_u32(root + 36);
    phase = ((sint32)time % 80) / 8;
    for (side = 0; side < 4; ++side)
        for (i = 0; i < 38; ++i)
        {
            selector = r_u32(0x800C7990 + side * 4);
            noise = r_u8(0x800C78F0 + side * 38 + i);
            offset = selector * 38 + i;
            sample = data[offset];
            if ((sint32)phase % 10 < 5)
                value = (uint32)((sint32)(phase * (sample + noise)) / 5) + 16;
            else
            {
                value = (uint32)((sint32)((phase - 5) * (sample + noise)) / 5);
                sample = data[offset];
                value = sample + noise + 16 - value;
            }
            data[152 + side * 38 + i] = (uint8)value;
        }
    for (side = 0; side < 4; ++side)
    {
        if (side < 2)
        {
            x = side == 0 ? 32 : 288;
            for (i = 0; i < 36; ++i, packet += 36)
            {
                y = 100 + i * 8;
                next = y + 8;
                first = data[152 + side * 38 + i];
                second = data[153 + side * 38 + i];
                w_u32(packet + 8, (y << 16) | x);
                w_u32(packet + 24, (next << 16) | x);
                w_u32(packet + 4, 0x3AFFFFFF);
                w_u32(packet + 12, 0x00030303);
                w_u32(packet + 20, 0x00FFFFFF);
                w_u32(packet + 28, 0x00030303);
                w_u32(packet + 16, (y << 16) | (side == 0 ? first + x : x - first));
                w_u32(packet + 32, (next << 16) | (side == 0 ? second + x : x - second));
                head = r_u32(ordering);
                w_u32(packet, head | 0x08000000);
                w_u32(ordering, packet & 0xFFFFFF);
            }
            for (segment = 0; segment < 2; ++segment)
                for (i = 0; i < 8; ++i, packet += 36)
                {
                    base = segment == 0 ? 100 : 348;
                    y = base + i * 4;
                    next = base + (i + 1) * 4;
                    curve = i * i / 2;
                    curve_next = (i + 1) * (i + 1) / 2;
                    if (segment == 0)
                    {
                        curve = curve - i * 8 + 32;
                        curve_next = curve_next - (i + 1) * 8 + 32;
                    }
                    first = side == 0 ? x + (uint32)(sint32)(sint16)curve : x - (uint32)(sint32)(sint16)curve;
                    second = side == 0 ? x + (uint32)(sint32)(sint16)curve_next : x - (uint32)(sint32)(sint16)curve_next;
                    w_u32(packet + 4, 0x3AFFFFFF);
                    w_u32(packet + 20, 0x3AFFFFFF);
                    w_u32(packet + 12, 0x3A030303);
                    w_u32(packet + 28, 0x3A030303);
                    w_u32(packet + 8, (y << 16) | x);
                    w_u32(packet + 16, (y << 16) | first);
                    w_u32(packet + 24, (next << 16) | x);
                    w_u32(packet + 32, (next << 16) | second);
                    head = r_u32(ordering);
                    w_u32(packet, head | 0x08000000);
                    w_u32(ordering, packet & 0xFFFFFF);
                }
        }
        else
        {
            base = side == 2 ? 100 : 380;
            for (i = 0; i < 33; ++i, packet += 36)
            {
                x = 32 + i * 8;
                next = x + 8;
                second = data[153 + side * 38 + i];
                first = data[152 + side * 38 + i];
                w_u32(packet + 8, (base << 16) | x);
                w_u32(packet + 24, (base << 16) | next);
                w_u32(packet + 4, 0x3AFFFFFF);
                w_u32(packet + 12, 0x00030303);
                w_u32(packet + 20, 0x00FFFFFF);
                w_u32(packet + 28, 0x00030303);
                y = side == 2 ? base + first : base - first;
                value = side == 2 ? base + second : base - second;
                w_u32(packet + 16, (y << 16) | x);
                w_u32(packet + 32, (value << 16) | next);
                head = r_u32(ordering);
                w_u32(packet, head | 0x08000000);
                w_u32(ordering, packet & 0xFFFFFF);
            }
        }
    }
    w_u8(packet + 3, 1);
    w_u32(packet + 4, 0xE1000240);
    tag = r_u32(packet);
    head = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 8;
}

uint32 sub_800765b0(uint32 first, uint32 first_vector, uint32 second, uint32 second_vector, uint32 output, uint8 *native_output)
{
    MATRIX first_matrix, second_matrix;
    uint16 first_local[3], second_local[3];
    uint32 first_rotated[3], second_rotated[3], delta[3], reduced[3];
    uint32 ax, ay, az, reference, cached, difference, value, a, b, c, d, e, i, pass, total, result;
    FUNCTION_MARKER(0x800765B0, "ACE3.BIN");
    ax = r_s16(second + 16);
    ay = r_s16(second + 18);
    reference = r_s16(first + 16);
    az = r_s16(second + 20);
    cached = r_u16(first + 16);
    difference = reference - ax;
    if ((sint32)difference < 8192)
    {
        ax -= 16384;
        difference = reference - ax;
    }
    if ((sint32)difference > 8192)
        ax += 16384;
    reference = r_s16(first + 18);
    difference = reference - ay;
    if ((sint32)difference < 8192)
    {
        ay -= 16384;
        difference = reference - ay;
    }
    if ((sint32)difference > 8192)
        ay += 16384;
    reference = r_s16(first + 20);
    difference = reference - az;
    if ((sint32)difference < 8192)
    {
        az -= 16384;
        difference = reference - az;
    }
    if ((sint32)difference > 8192)
        az += 16384;
    value = ax - cached;
    if (native_output)
        xport_store_le16(native_output + 52, value);
    else
        w_u16(output + 52, value);
    reference = r_u16(first + 18);
    value = ay - reference;
    if (native_output)
        xport_store_le16(native_output + 54, value);
    else
        w_u16(output + 54, value);
    reference = r_u16(first + 20);
    value = az - reference;
    if (native_output)
        xport_store_le16(native_output + 56, value);
    else
        w_u16(output + 56, value);
    rotation_build(first + 16, 0, &first_matrix, 0);
    rotation_build(second + 16, 0, &second_matrix, 0);
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(first_vector + i * 4);
        first_local[i] = (uint16)((sint32)value >> 6);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(second_vector + i * 4);
        second_local[i] = (uint16)((sint32)value >> 6);
    }
    a = xport_load_le32((uint8 *)&first_matrix);
    b = xport_load_le32((uint8 *)&first_matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&first_matrix + 8);
    d = xport_load_le32((uint8 *)&first_matrix + 12);
    e = xport_load_le16((uint8 *)&first_matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, first_local[0] | ((uint32)first_local[1] << 16));
    xport_gte_write_data(1, first_local[2]);
    xport_gte_execute(0x486012);
    first_rotated[0] = xport_gte_read_data(25);
    first_rotated[1] = xport_gte_read_data(26);
    first_rotated[2] = xport_gte_read_data(27);
    a = xport_load_le32((uint8 *)&second_matrix);
    b = xport_load_le32((uint8 *)&second_matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&second_matrix + 8);
    d = xport_load_le32((uint8 *)&second_matrix + 12);
    e = xport_load_le16((uint8 *)&second_matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, second_local[0] | ((uint32)second_local[1] << 16));
    xport_gte_write_data(1, second_local[2]);
    xport_gte_execute(0x486012);
    second_rotated[0] = xport_gte_read_data(25);
    second_rotated[1] = xport_gte_read_data(26);
    second_rotated[2] = xport_gte_read_data(27);
    a = r_u32(second);
    b = r_u32(first);
    c = r_u32(first + 4);
    delta[0] = (uint32)((sint32)(a - b) >> 6);
    a = r_u32(second + 4);
    b = r_u32(second + 8);
    delta[1] = (uint32)((sint32)(a - c) >> 6);
    c = r_u32(first + 8);
    delta[2] = (uint32)((sint32)(b - c) >> 6);
    delta[0] += second_rotated[0] - first_rotated[0];
    delta[1] += second_rotated[1] - first_rotated[1];
    value = delta[1] << 6;
    reduced[1] = (uint32)((sint32)delta[1] >> 6);
    if (native_output)
        xport_store_le32(native_output + 40, value);
    else
        w_u32(output + 40, value);
    delta[2] += second_rotated[2] - first_rotated[2];
    reduced[0] = (uint32)((sint32)delta[0] >> 6);
    reduced[2] = (uint32)((sint32)delta[2] >> 6);
    value = delta[0] << 6;
    if (native_output)
        xport_store_le32(native_output + 36, value);
    else
        w_u32(output + 36, value);
    value = delta[2] << 6;
    if (native_output)
        xport_store_le32(native_output + 44, value);
    else
        w_u32(output + 44, value);
    for (pass = 0; pass < 2; ++pass)
        for (i = 0; i < 3; ++i)
        {
            reference = r_u32(first + i * 4);
            value = (first_rotated[i] << 6) + reference;
            if (native_output)
                xport_store_le32(native_output + i * 4, value);
            else
                w_u32(output + i * 4, value);
        }
    total = (sint32)reduced[0] < 0 ? 0 - reduced[0] : reduced[0];
    value = (sint32)reduced[1] < 0 ? 0 - reduced[1] : reduced[1];
    total += value;
    value = (sint32)reduced[2] < 0 ? 0 - reduced[2] : reduced[2];
    total += value;
    result = 65536;
    if ((sint32)total <= 45056)
        result = sub_80083f40(reduced[0], reduced[1], reduced[2]);
    if (native_output)
        xport_store_le32(native_output + 32, result);
    else
        w_u32(output + 32, result);
    return result;
}

uint32 sub_80076924(uint32 object, uint32 selection, uint32 vector, uint32 output, uint8 *native_output)
{
    uint32 index, objects, target, value, other, result;
    FUNCTION_MARKER(0x80076924, "ACE3.BIN");
    index = r_u32(selection);
    objects = r_u32(0x800BCE5C);
    target = r_u32(objects + index * 4);
    sub_800765b0(target + 8, selection + 4, object + 8, vector, output, native_output);
    value = native_output ? (uint32)(sint32)(sint16)xport_load_le16(native_output + 52) : r_s16(output + 52);
    other = native_output ? (uint32)(sint32)(sint16)xport_load_le16(native_output + 54) : r_s16(output + 54);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)other < 0)
        other = 0 - other;
    result = (sint32)value < 365;
    if ((sint32)other >= 365)
        result = 0;
    value = native_output ? (uint32)(sint32)(sint16)xport_load_le16(native_output + 56) : r_s16(output + 56);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value >= 365)
        result = 0;
    value = native_output ? xport_load_le32(native_output + 36) : r_u32(output + 36);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x32000)
        result = 0;
    value = native_output ? xport_load_le32(native_output + 40) : r_u32(output + 40);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x50000)
        result = 0;
    value = native_output ? xport_load_le32(native_output + 44) : r_u32(output + 44);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x96000)
        result = 0;
    return result;
}

uint32 sub_800729dc(void)
{
    MATRIX matrix;
    uint8 relative[64], final_relative[58];
    uint16 local[3];
    uint32 rotated[3], objects, state, settings_root, object, settings, mode, control, smooth, input, game, target, selection, vector, output, value, other, flags, result, saved, option, ax, ay, az, reference, difference, cached, a, b, c, d, e, i;
    FUNCTION_MARKER(0x800729DC, "ACE3.BIN");
    objects = r_u32(0x800BCE5C);
    state = r_u32(0x800BD094);
    settings_root = r_u32(0x800BD098);
    object = r_u32(objects);
    mode = r_u32(state + 4);
    settings = r_u32(settings_root);
    switch (mode)
    {
        case 1:
        case 4:
            sub_80072938(object, settings);
            state = r_u32(0x800BD094);
            smooth = r_u32(0x800BD090);
            value = r_u32(state + 72);
            sub_8006e874((sint32)value, smooth);
            state = r_u32(0x800BD094);
            flags = r_u32(state + 16);
            if ((flags & 15) != 0)
            {
                control = r_u32(0x800BD08C);
                value = r_s16(control + 8);
                if ((sint32)value >= 257)
                {
                    w_u16(control + 8, 256);
                    control = r_u32(0x800BD08C);
                }
                value = r_s16(control + 8);
                if ((sint32)value < -256)
                    w_u16(control + 8, 0xFF00);
            }
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(mode == 1 ? 0xFFFFFFFF : 0xFFFFFFF7);
            break;
        case 2:
            control = r_u32(0x800BD08C);
            w_u16(control + 8, 0);
            w_u16(control + 6, 0);
            w_u16(control + 4, 0);
            w_u16(control + 14, 0);
            w_u16(control + 12, 0);
            w_u16(control + 10, 0);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(71);
            break;
        case 8:
            sub_80072938(object, settings);
            target = r_u32(0x800BCE18);
            saved = sub_80076428(object + 8, target);
            state = r_u32(0x800BD094);
            flags = r_u32(state + 16);
            if ((flags & 9) != 0)
            {
                if (saved == 0)
                    w_u16(object + 78, 0xFFFF);
                else if (saved == 1)
                {
                    value = r_u32(state);
                    w_u32(state, value | 128);
                    sub_8002db4c(90);
                    option = r_u32(0x800BCE14);
                    if (option != 1)
                        goto collision_transition;
                    sub_8002db4c(91);
                }
                else
                {
                    value = r_u32(state);
                    w_u32(state, value | 256);
                }
                option = r_u32(0x800BCE14);
            collision_transition:
                sub_80076b18(option != 0, 2);
                state = r_u32(0x800BD094);
                w_u16(state + 78, 30);
            }
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(saved != 0 ? 0xFFFFFFE7 : 0xFFFFFFF7);
            output = r_u32(0x800BD0A0);
            w_u32(output + 52, saved);
            break;
        case 16:
            smooth = r_u32(0x800BD090);
            target = r_u32(0x800BCE18);
            result = sub_80074ba8(object, state, smooth, target);
            if (result != 0)
            {
                option = r_u32(0x800BCE14);
                sub_80076b18(option != 0, 2);
            }
            sub_80072178(6);
            output = r_u32(0x800BD0A0);
            w_u32(output + 52, 1);
            break;
        case 32:
            sub_80074878(object, state, 0);
            sub_80072178(70);
            target = r_u32(0x800BCE18);
            result = sub_80076428(object + 8, target);
            state = r_u32(0x800BD094);
            flags = r_u32(state + 16);
            if ((flags & 9) != 0 && result == 0)
            {
                settings_root = r_u32(0x800BD098);
                value = r_u32(settings_root + 36);
                game = r_u32(0x800BCE60);
                w_u32(settings_root + 36, value | 4);
                value = r_u32(game + 40);
                if ((value & 7) == 0)
                    sub_8002dbe0(39);
                state = r_u32(0x800BD094);
                sub_80082bac(object + 8, object + 56, state + 28, 0);
                state = r_u32(0x800BD094);
                value = r_u16(state + 78) - 1;
                w_u16(state + 78, value);
                if ((sint32)(value << 16) <= 0)
                    w_u16(object + 78, 0xFFFF);
            }
            break;
        case 64:
            sub_80074878(object, state, 1);
            target = r_u32(0x800BCE18);
            result = sub_80076428(object + 8, target);
            state = r_u32(0x800BD094);
            flags = r_u32(state + 16);
            if ((flags & 9) != 0 && result == 0)
                w_u16(object + 78, 0xFFFF);
            sub_80072178(6);
            break;
        case 256:
            sub_80072938(object, settings);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            selection = r_u32(0x800BCE1C);
            vector = r_u32(0x800BCE20);
            saved = sub_80076924(object, selection, vector, 0, relative);
            sub_80072178(0xFFFFFFF7);
            state = r_u32(0x800BD094);
            value = r_u32(state + 8);
            if (value == 0)
            {
                w_u32(state + 8, 1);
                w_u32(state + 12, 0);
            }
            if (saved != 0)
            {
                state = r_u32(0x800BD094);
                value = r_u32(state + 8);
                if (value == 1)
                {
                    value = r_u32(state + 12);
                    w_u32(state + 12, value + 1);
                }
                else
                {
                    state = r_u32(0x800BD094);
                    w_u32(state + 12, 0);
                }
            }
            else
            {
                state = r_u32(0x800BD094);
                w_u32(state + 12, 0);
            }
            state = r_u32(0x800BD094);
            value = r_u32(state + 12);
            if ((sint32)value > 45)
            {
                value = r_u32(state);
                option = r_u32(0x800BCE14);
                w_u32(state, value | 128);
                sub_80076c54(option != 2, 2);
            }
            output = r_u32(0x800BD0A0);
            w_u32(output + 52, saved);
            break;
        case 512:
            smooth = r_u32(0x800BD090);
            selection = r_u32(0x800BCE1C);
            sub_80074e64(object, state, smooth, selection);
            sub_80072178(4);
            option = r_u32(0x800BCE14);
            sub_80076c54(option != 2, 2);
            break;
        case 1024:
            selection = r_u32(0x800BCE1C);
            value = r_u32(selection);
            vector = r_u32(0x800BCE20);
            target = r_u32(objects + value * 4);
            sub_80076924(object, selection, vector, 0, relative);
            state = r_u32(0x800BD094);
            saved = sub_80075084(object, state, 0, target + 56, relative + 36);
            sub_80072178(4);
            if (saved != 0)
            {
                option = r_u32(0x800BCE14);
                sub_80076c54(option != 2, 3);
            }
            break;
        case 2048:
            input = r_u32(0x800BD088);
            control = r_u32(0x800BD08C);
            smooth = r_u32(0x800BD090);
            sub_8007542c(object, state, input, control, smooth);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(4);
            output = r_u32(0x800BD0A0);
            w_u32(output + 52, 0);
            break;
        case 4096:
            sub_80072178(4);
            break;
        case 8192:
            target = r_u32(0x800BCE18);
            sub_8007599c(object, state, target);
            state = r_u32(0x800BD094);
            smooth = r_u32(0x800BD090);
            value = r_u32(state + 72);
            sub_8006e874((sint32)value, smooth);
            settings_root = r_u32(0x800BD098);
            state = r_u32(0x800BD094);
            w_u32(settings_root + 44, 0);
            w_u32(state + 72, 0);
            control = r_u32(0x800BD08C);
            w_u16(control + 14, 0xF000);
            sub_80072114();
            break;
        case 0x4000:
        case 0x8000:
            state = r_u32(0x800BD094);
            control = r_u32(0x800BD08C);
            input = r_u32(0x800BD088);
            smooth = r_u32(0x800BD090);
            w_u16(input + 6, 4096);
            value = r_u32(state + 4);
            target = r_u32(0x800BCE18);
            saved = value == 0x8000;
            if (sub_80075b04(object, state, control, smooth, target, saved) == 0)
            {
                sub_80072114();
                break;
            }
            sub_80072938(object, settings);
            state = r_u32(0x800BD094);
            control = r_u32(0x800BD08C);
            value = r_u32(state + 72);
            sub_8006e7e0(value, control);
            state = r_u32(0x800BD094);
            smooth = r_u32(0x800BD090);
            value = r_u32(state + 72);
            sub_8006e874((sint32)value, smooth);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(0xFFFFFFD7);
            target = r_u32(0x800BCE18);
            if (sub_80076a38(object + 8, target) != 0)
                sub_80076dec(saved, 2);
            break;
        case 0x10000:
            control = r_u32(0x800BD08C);
            smooth = r_u32(0x800BD090);
            target = r_u32(0x800BCE18);
            sub_80075e08(object, state, control, smooth, target, 0);
            sub_80076dec(0, 2);
            break;
        case 0x100000:
            sub_80074560(object, state);
            sub_80072178(6);
            break;
        case 0x200000:
        case 0x400000:
            input = r_u32(0x800BD088);
            control = r_u32(0x800BD08C);
            smooth = r_u32(0x800BD090);
            sub_80073b18(object, input, control, smooth, mode == 0x200000);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_8006ef30(object, control, settings, state);
            sub_80072178(71);
            break;
        case 0x1000000:
        case 0x2000000:
        case 0x4000000:
            input = r_u32(0x800BD088);
            control = r_u32(0x800BD08C);
            game = r_u32(0x800BCE60);
            smooth = r_u32(0x800BD090);
            sub_8006d678(game + 288, input, control, smooth);
            state = r_u32(0x800BD094);
            smooth = r_u32(0x800BD090);
            value = r_u32(state + 72);
            sub_8006e874((sint32)value, smooth);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_800702d8(object, control, settings, state);
            if (mode == 0x4000000)
            {
                state = r_u32(0x800BD094);
                sub_8007601c(object, state);
            }
            sub_80072178(mode == 0x1000000 ? 0xFFFFFFAD : 0xFFFFFFA5);
            if (mode == 0x4000000)
            {
                target = r_u32(0x800BCE18);
                result = sub_80076ac0(object + 8);
                state = r_u32(0x800BD094);
                value = r_u32(state);
                if (value == 16 && result != 0)
                {
                    value = r_u16(state + 80) + 2;
                    w_u16(state + 80, value);
                    if ((sint16)value >= 61)
                    {
                        sub_80076360(1);
                        state = r_u32(0x800BD094);
                        value = r_u32(state);
                        w_u32(state, value | 0x280);
                    }
                }
            }
            break;
        case 0x8000000:
            value = r_s16(object + 28);
            other = r_u16(object + 28);
            w_u16(object + 26, 0);
            if ((sint32)value < 4)
                w_u16(object + 28, other + 4);
            value = r_s16(object + 28);
            other = r_u16(object + 28);
            if ((sint32)value >= -3)
                w_u16(object + 28, other - 4);
            game = r_u32(0x800BCE60);
            value = r_u32(game + 4);
            option = value != 0;
            if (!option)
            {
                value = r_s16(object + 28);
                if ((sint32)value < 0)
                    value = 0 - value;
                option = (sint32)value >= 547;
            }
            if (option)
            {
                input = r_u32(0x800BD088);
                control = r_u32(0x800BD08C);
                smooth = r_u32(0x800BD090);
                sub_8007407c(object, input, control, smooth);
            }
            else
            {
                control = r_u32(0x800BD08C);
                input = r_u32(0x800BD088);
                w_u16(control + 8, 0);
                w_u16(control + 6, 0);
                w_u16(control + 4, 0);
                w_u16(control + 14, 0);
                w_u16(control + 12, 0);
                w_u16(control + 10, 0);
                w_u16(input, 0);
                w_u16(input + 2, 0);
                w_u16(input + 4, 0);
                w_u16(input + 6, 0);
            }
            state = r_u32(0x800BD094);
            smooth = r_u32(0x800BD090);
            value = r_u32(state + 72);
            sub_8006e874((sint32)value, smooth);
            control = r_u32(0x800BD08C);
            state = r_u32(0x800BD094);
            sub_800702d8(object, control, settings, state);
            state = r_u32(0x800BD094);
            sub_8007601c(object, state);
            sub_80072178(0xFFFFFFA5);
            break;
        case 0x10000000:
            control = r_u32(0x800BD08C);
            w_u16(control + 8, 0);
            w_u16(control + 6, 0);
            w_u16(control + 4, 0);
            w_u16(control + 14, 0);
            w_u16(control + 12, 0);
            w_u16(control + 10, 0);
            sub_800702d8(object, control, settings, state);
            state = r_u32(0x800BD094);
            sub_8007601c(object, state);
            sub_80072178(0xFFFFFFA5);
            break;
        case 0x20000000:
            sub_80072114();
            break;
        default:
            break;
    }
    state = r_u32(0x800BD094);
    flags = r_u32(state + 4);
    if ((flags & 24) != 0)
    {
        target = r_u32(0x800BCE18);
        rotation_build(target + 16, 0, &matrix, 0);
        for (i = 0; i < 3; ++i)
        {
            value = r_u32(object + 8 + i * 4);
            other = r_u32(target + i * 4);
            local[i] = (uint16)((sint32)(value - other) >> 12);
        }
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
        xport_gte_write_data(0, local[0] | ((uint32)local[1] << 16));
        xport_gte_write_data(1, local[2]);
        xport_gte_execute(0x486012);
        rotated[0] = xport_gte_read_data(25);
        rotated[1] = xport_gte_read_data(26);
        rotated[2] = xport_gte_read_data(27);
        output = r_u32(0x800BD0A0);
        w_u32(output + 28, rotated[0] << 12);
        w_u32(output + 32, rotated[1] << 12);
        state = r_u32(0x800BD094);
        w_u32(output + 36, rotated[2] << 12);
        ax = r_s16(state + 64);
        ay = r_s16(state + 66);
        reference = r_s16(target + 16);
        az = r_s16(state + 68);
        cached = r_u16(target + 16);
        difference = reference - ax;
        if ((sint32)difference < 8192)
        {
            ax -= 16384;
            difference = reference - ax;
        }
        if ((sint32)difference > 8192)
            ax += 16384;
        reference = r_s16(target + 18);
        difference = reference - ay;
        if ((sint32)difference < 8192)
        {
            ay -= 16384;
            difference = reference - ay;
        }
        if ((sint32)difference > 8192)
            ay += 16384;
        reference = r_s16(target + 20);
        difference = reference - az;
        if ((sint32)difference < 8192)
        {
            az -= 16384;
            difference = reference - az;
        }
        if ((sint32)difference > 8192)
            az += 16384;
        w_u16(output + 44, ax - cached);
        value = r_u16(target + 18);
        w_u16(output + 46, ay - value);
        value = r_u16(target + 20);
        target = r_u32(0x800BCE18);
        w_u16(output + 48, az - value);
        result = sub_80083dfc(target, object + 8, 0, 0);
        output = r_u32(0x800BD0A0);
        w_u32(output + 24, result);
    }
    state = r_u32(0x800BD094);
    flags = r_u32(state + 4);
    if ((flags & 0x1F00) != 0)
    {
        selection = r_u32(0x800BCE1C);
        vector = r_u32(0x800BCE20);
        value = r_u32(selection);
        objects = r_u32(0x800BCE5C);
        target = r_u32(objects + value * 4);
        result = sub_80076924(object, selection, vector, 0, final_relative);
        output = r_u32(0x800BD0A0);
        value = r_u16(target + 24);
        w_u16(output + 16, value);
        value = r_u16(target + 26);
        w_u16(output + 18, value);
        value = r_u16(target + 28);
        w_u16(output + 20, value);
        w_u32(output, xport_load_le32(final_relative));
        w_u32(output + 4, xport_load_le32(final_relative + 4));
        w_u32(output + 8, xport_load_le32(final_relative + 8));
        w_u32(output + 28, xport_load_le32(final_relative + 36));
        w_u32(output + 32, xport_load_le32(final_relative + 40));
        w_u32(output + 36, xport_load_le32(final_relative + 44));
        w_u16(output + 44, xport_load_le16(final_relative + 52));
        w_u16(output + 46, xport_load_le16(final_relative + 54));
        w_u16(output + 48, xport_load_le16(final_relative + 56));
        value = xport_load_le32(final_relative + 32);
        w_u32(output + 52, result);
        w_u32(output + 24, value);
    }
    result = r_s16(object + 78);
    if ((sint32)result > 0)
        return result;
    state = r_u32(0x800BD094);
    result = r_u32(state) & 0x7000;
    if (result != 0)
        return result;
    game = r_u32(0x800BCE60);
    value = r_u16(game + 2392);
    w_u16(game + 2392, value + 1);
    sub_800763cc();
    target = r_u32(0x800BCE54);
    flags = r_u32(target);
    option = 2;
    if ((flags & 16) == 0)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state + 16);
        option = (flags & 0x2F) != 0;
    }
    return sub_80079564(option);
}

uint32 sub_80075084(uint32 object, uint32 state, uint32 relative, uint32 velocity, const uint8 *native_relative)
{
    uint32 direction[3], angles[3], step[3], value, other, mode, i, difference, magnitude, total;
    FUNCTION_MARKER(0x80075084, "ACE3.BIN");
    value = native_relative ? xport_load_le32(native_relative) : r_u32(relative);
    other = native_relative ? xport_load_le32(native_relative + 8) : r_u32(relative + 8);
    direction[0] = (uint32)((sint32)value >> 5);
    value = native_relative ? xport_load_le32(native_relative + 4) : r_u32(relative + 4);
    direction[1] = (uint32)((sint32)value >> 5);
    direction[2] = (uint32)((sint32)other >> 5);
    for (i = 0; i < 3; ++i)
    {
        if ((sint32)direction[i] >= 4097)
            direction[i] = 4096;
        if ((sint32)direction[i] < -4096)
            direction[i] = 0xFFFFF000;
    }
    angles[0] = native_relative ? (uint32)(sint32)(sint16)xport_load_le16(native_relative + 16) : r_s16(relative + 16);
    angles[1] = native_relative ? (uint32)(sint32)(sint16)xport_load_le16(native_relative + 18) : r_s16(relative + 18);
    angles[2] = native_relative ? (uint32)(sint32)(sint16)xport_load_le16(native_relative + 20) : r_s16(relative + 20);
    for (i = 0; i < 3; ++i)
        step[i] = (uint32)((sint32)(0 - angles[i]) >> 6);
    if (step[0] == 0)
    {
        if ((sint32)angles[0] > 0)
            step[0] = 0xFFFFFFFF;
        if (step[0] == 0 && (sint32)angles[0] < 0)
            step[0] = 1;
    }
    for (i = 1; i < 3; ++i)
        if (step[i] == 0)
        {
            value = native_relative ? (uint32)(sint32)(sint16)xport_load_le16(native_relative + 16 + i * 2) : r_s16(relative + 16 + i * 2);
            if ((sint32)value > 0)
                step[i] = 0xFFFFFFFF;
            if (step[i] == 0 && (sint32)value < 0)
                step[i] = 1;
        }
    value = r_u16(object + 24);
    other = r_u16(object + 28);
    w_u16(object + 24, value + step[0]);
    value = r_u16(object + 26);
    w_u16(object + 28, other + step[2]);
    w_u16(object + 26, value + step[1]);
    mode = r_u32(state + 8);
    if (mode == 0)
    {
        for (i = 0; i < 3; ++i)
        {
            value = r_u32(object + 56 + i * 4);
            other = r_u32(velocity + i * 4);
            if ((sint32)value < (sint32)other)
            {
                w_u32(object + 56 + i * 4, value + 256);
                value = r_u32(object + 56 + i * 4);
            }
            other = r_u32(velocity + i * 4);
            if ((sint32)other < (sint32)value)
                w_u32(object + 56 + i * 4, value - 256);
        }
        for (i = 0; i < 3; ++i)
        {
            value = r_u32(object + 56 + i * 4);
            other = r_u32(velocity + i * 4);
            difference = value - other;
            if ((sint32)difference < 0)
                difference = other - value;
            if ((sint32)difference >= 256)
                goto finish;
        }
        w_u32(state + 8, 1);
        w_u32(state + 12, 0);
    }
    else if (mode == 1)
    {
        value = r_u32(velocity);
        w_u32(object + 56, value - direction[0]);
        value = r_u32(velocity + 4);
        w_u32(object + 60, value - direction[1]);
        value = r_u32(velocity + 8);
        w_u32(object + 64, value - direction[2]);
        total = (sint32)direction[1] < 0 ? 0 - direction[1] : direction[1];
        magnitude = (sint32)direction[0] < 0 ? 0 - direction[0] : direction[0];
        total += magnitude;
        magnitude = (sint32)direction[2] < 0 ? 0 - direction[2] : direction[2];
        total += magnitude;
        if ((sint32)total < 12288)
        {
            value = r_u32(state + 12);
            if (value == 150)
            {
                other = r_u32(0x800BCE14);
                sub_8002db4c(other == 2 ? 94 : 95);
            }
            value = r_u32(state + 12) + 1;
            w_u32(state + 12, value);
            if ((sint32)value >= 301)
                return 1;
        }
    }
finish:
    sub_8006ee38(object, state);
    return 0;
}

uint32 sub_80076360(uint32 mode)
{
    uint32 state, flags;
    FUNCTION_MARKER(0x80076360, "ACE3.BIN");
    if (mode == 0)
    {
        state = r_u32(0x800BD094);
        w_u32(state, 16);
        w_u32(state + 4, 0x4000000);
        w_u32(state + 8, 0);
        w_u16(state + 78, 0);
        w_u16(state + 80, 0);
        return state;
    }
    if (mode == 1)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state + 4, 0x8000000);
        w_u32(state, flags | 64);
        return state;
    }
    state = r_u32(0x800BD094);
    w_u32(state + 4, 0x10000000);
    return 0x10000000;
}

uint32 sub_800763cc(void)
{
    uint32 state, flags;
    FUNCTION_MARKER(0x800763CC, "ACE3.BIN");
    state = r_u32(0x800BD094);
    flags = r_u32(state);
    w_u32(state + 4, 0x100000);
    w_u32(state + 8, 0);
    w_u32(state, flags | 0x1000);
    return sub_800301a8();
}

uint32 sub_80076b18(uint32 mode, uint32 kind)
{
    uint32 table[6], index, state, value, flags, target, output, i;
    FUNCTION_MARKER(0x80076B18, "ACE3.BIN");
    index = (kind << 1) + mode;
    table[0] = r_u32(0x800246E0);
    table[1] = r_u32(0x800246E4);
    table[2] = r_u32(0x800246E8);
    table[3] = r_u32(0x800246EC);
    table[4] = r_u32(0x800246F0);
    table[5] = r_u32(0x800246F4);
    value = table[index];
    if (value == 8)
    {
        state = r_u32(0x800BD094);
        w_u32(state, 4);
        value = table[index];
    }
    if (value == 16)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state, flags | 64);
    }
    else if (value == 32 || value == 64)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state, (flags | 512) & 0xFFFFFFBF);
    }
    target = r_u32(0x800BCE18);
    output = r_u32(0x800BD0A0);
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(target + i * 4);
        w_u32(output + i * 4, value);
    }
    value = r_u16(target + 16);
    w_u16(output + 16, value);
    value = r_u16(target + 18);
    w_u16(output + 18, value);
    value = r_u16(target + 20);
    state = r_u32(0x800BD094);
    w_u16(output + 20, value);
    value = table[index];
    w_u32(state + 8, 0);
    w_u32(state + 4, value);
    return value;
}

uint32 sub_80076c54(uint32 mode, uint32 kind)
{
    uint32 table[8], index, state, value, flags, output;
    FUNCTION_MARKER(0x80076C54, "ACE3.BIN");
    index = (kind << 1) + mode;
    table[0] = r_u32(0x800246F8);
    table[1] = r_u32(0x800246FC);
    table[2] = r_u32(0x80024700);
    table[3] = r_u32(0x80024704);
    table[4] = r_u32(0x80024708);
    table[5] = r_u32(0x8002470C);
    table[6] = r_u32(0x80024710);
    table[7] = r_u32(0x80024714);
    value = table[index];
    if (value == 256)
    {
        state = r_u32(0x800BD094);
        w_u32(state, 8);
        value = table[index];
    }
    if (value == 512 || value == 1024 || value == 4096)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state, flags | 64);
    }
    else if (value == 2048)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state, flags & 0xFFFFFFBF);
    }
    value = table[index];
    if (value == 2048 || value == 4096)
    {
        state = r_u32(0x800BD094);
        flags = r_u32(state);
        w_u32(state, flags | 512);
    }
    output = r_u32(0x800BD0A0);
    state = r_u32(0x800BD094);
    w_u32(output, 0);
    w_u32(output + 4, 0);
    w_u32(output + 8, 0);
    w_u16(output + 16, 0);
    w_u16(output + 18, 0);
    w_u16(output + 20, 0);
    value = table[index];
    w_u32(state + 8, 0);
    w_u32(state + 4, value);
    return value;
}

uint32 sub_8006ef30(uint32 object, uint32 control, uint32 settings, uint32 state)
{
    MATRIX matrix;
    uint16 local_angles[3], turn[3];
    uint32 vector[3], acceleration[3], limits[3], command[3], desired[3], current[3];
    uint32 a, b, c, d, e, value, other, numerator, divisor, ratio, speed, bound0, bound1, bound2;
    uint32 factor, bias, lift, strength, stall, capped_strength, vertical, lateral, magnitude, gravity, pull;
    uint32 coefficient_x, coefficient_y, coefficient_z, flags, gain, weight_x, weight_z, damping, throttle, thrust, i;
    FUNCTION_MARKER(0x8006EF30, "ACE3.BIN");
    acceleration[0] = 0;
    acceleration[1] = 0;
    acceleration[2] = 0;
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
    value = r_u32(object + 56);
    vector[0] = (uint16)((sint32)(0 - value) >> 5);
    value = r_u32(object + 60);
    vector[1] = (uint16)((sint32)(0 - value) >> 5);
    value = r_u32(object + 64);
    vector[2] = (uint16)((sint32)(0 - value) >> 5);
    xport_gte_write_data(0, vector[0] | (vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    value = r_u32(settings + 60);
    speed = ((sint32)c < 0 ? 0 - c : c) << 5;
    if ((sint32)value < (sint32)speed)
    {
        numerator = value << 6;
        if (speed == 0)
            xport_mips_break(7);
        if (speed == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        ratio = (sint32)numerator / (sint32)speed;
        a = r_u32(object + 56) * ratio;
        b = r_u32(object + 60) * ratio;
        w_u32(object + 56, (sint32)a / 64);
        c = r_u32(object + 64) * ratio;
        w_u32(object + 60, (sint32)b / 64);
        w_u32(object + 64, (sint32)c / 64);
        speed = r_u32(settings + 60);
    }
    stall = 0;
    factor = sub_8006ea88(r_u32(object + 12), speed, settings, 0, 0, 0, &bound0, &bound1, &bound2);
    divisor = (sint32)bound0 < 118784 ? 118784 : bound0;
    numerator = speed << 11;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (uint32)((sint32)numerator / (sint32)divisor) << 1;
    value = r_s16(state + 56);
    if ((sint32)value < 0)
    {
        divisor = r_u32(settings + 84);
        if ((sint32)value < (sint32)divisor)
            value = divisor;
        numerator = (r_u32(settings + 92) - 4096) * value;
    }
    else
    {
        divisor = r_u32(settings + 88);
        if ((sint32)divisor < (sint32)value)
            value = divisor;
        numerator = (r_u32(settings + 96) - 4096) * value;
    }
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    bias = (uint32)((sint32)numerator / (sint32)divisor) + 4096;
    lift = (sint32)(ratio * bias) / 4096;
    if ((sint32)(lift + bias) >= 8993 && (sint32)lift >= 6145 && (sint32)bias >= 5121)
    {
        flags = r_u32(state + 16);
        w_u32(state + 16, flags | 0x1000);
    }
    value = r_u32(settings + 100);
    if ((sint32)lift < (sint32)value)
    {
        stall = 4096;
        strength = 0;
    }
    else
    {
        strength = (sint32)((lift - value) * r_u32(settings + 104)) / 4096;
        if ((sint32)strength > 4096)
            strength = (uint32)((sint32)((strength - 4096) * r_u32(settings + 108)) / 4096) + 4096;
        value = r_u32(settings + 92);
        if ((sint32)value < (sint32)strength)
            strength = value;
        value = lift - r_u32(settings + 100);
        if ((sint32)value < 256)
            stall = 4096 - (value << 4);
    }
    capped_strength = (sint32)strength > 4096 ? 4096 : strength;
    w_u16(state + 76, capped_strength);
    value = (sint32)lift > 4096 ? 4096 : lift;
    other = r_s16(object + 24);
    if ((sint32)other < 0)
        other = 0 - other;
    vertical = (sint32)((4096 - other) * value) / 4096;
    value = r_u16(object + 28) & 0x3FFF;
    a = r_s16(0x800104AC + (value + 0x1000) * 2);
    b = r_s16(0x800104AC + value * 2);
    other = (sint32)a < 0 ? 0 - a : a;
    if ((sint32)other < 1025)
        a = (sint32)a < 0 ? (uint32)-1024 : 1024;
    numerator = b << 12;
    if (a == 0)
        xport_mips_break(7);
    if (a == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (sint32)numerator / (sint32)a;
    if ((sint32)value < 0)
        value = 0 - value;
    lateral = (sint32)(vertical * value) / 4096;
    value = (sint32)lift > 6021 ? 6021 : lift;
    if ((sint32)value < (sint32)lateral)
        magnitude = value;
    else
    {
        magnitude = sub_80083f40(lateral, vertical, 0);
        if ((sint32)value < (sint32)magnitude)
            magnitude = value;
    }
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
    gravity = (uint32)-669;
    if ((sint32)factor < 4096)
        gravity = (sint32)(gravity * factor) / 4096;
    pull = (sint32)(gravity * magnitude) / 4096;
    xport_gte_write_data(0, (pull & 0xFFFF) << 16);
    xport_gte_write_data(1, 0);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    value = (uint32)((sint32)(669 * (4096 - factor)) / 4096) + 669;
    acceleration[0] += a;
    acceleration[1] += b + value;
    acceleration[2] += c;
    coefficient_y = r_u32(settings + 36);
    coefficient_x = r_u32(settings + 32);
    coefficient_z = r_u32(settings + 40);
    limits[0] = r_u32(settings + 20);
    limits[1] = r_u32(settings + 24);
    limits[2] = r_u32(settings + 28);
    if (stall != 0)
    {
        coefficient_x += stall;
        coefficient_z += stall;
        if ((sint32)coefficient_x > 4096)
            coefficient_x = 4096;
        if ((sint32)coefficient_z > 4096)
            coefficient_z = 4096;
    }
    local_angles[0] = 0;
    local_angles[1] = 0;
    local_angles[2] = r_u16(object + 28);
    rotation_build(0, 0, &matrix, local_angles);
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
    a = r_u32(state + 56);
    b = r_u32(state + 60);
    xport_gte_write_data(0, a);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    flags = r_u32(state + 4);
    if ((flags & 0xC108) != 0)
        coefficient_x = (sint32)coefficient_x >> 2;
    vector[0] = (sint32)(a * coefficient_x) / 4096;
    vector[1] = (sint32)(b * coefficient_y) / 4096;
    vector[2] = (sint32)(c * coefficient_z) / 4096;
    for (i = 0; i < 3; ++i)
    {
        value = 0 - limits[i];
        if ((sint32)vector[i] < (sint32)value)
            vector[i] = value;
        if ((sint32)limits[i] < (sint32)vector[i])
            vector[i] = limits[i];
    }
    flags = r_u32(state + 16);
    if ((flags & 2) == 0)
    {
        a = r_u16(object + 24);
        c = r_u16(object + 28);
        w_u16(object + 24, a + vector[0]);
        b = r_u16(object + 26);
        w_u16(object + 28, c + vector[2]);
        w_u16(object + 26, b + vector[1]);
    }
    if (stall != 0)
    {
        flags = r_u32(state + 16);
        if ((flags & 1) == 0 && (~flags & 10) == 10)
        {
            value = (sint32)((0 - 37) * stall) / 4096;
            a = r_s16(object + 24);
            b = r_u16(object + 24);
            if ((sint32)((uint32)-4096 - value) < (sint32)a)
                w_u16(object + 24, b + value);
        }
    }
    command[0] = r_s16(control + 4);
    command[1] = r_s16(control + 6);
    command[2] = r_s16(control + 8);
    gain = r_u32(settings + ((sint32)command[0] < 0 ? 4 : 0));
    value = r_s16(state + 56);
    value = (sint32)(value * (uint32)((sint32)speed >> 1)) / 8192;
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 12800)
        value = 12800;
    value = (sint32)(value * 3192) / 12800;
    a = r_s16(control + 4);
    if ((sint32)a < 0)
        a = 0 - a;
    value = (sint32)((3192 - value) * a) / 4096;
    value = (sint32)(gain * value) / 4096;
    b = r_s16(control + 8);
    if ((sint32)b < 0)
        b = 0 - b;
    divisor = a + b;
    gain += value;
    if (divisor == 0)
    {
        weight_x = 0;
        weight_z = 0;
    }
    else
    {
        numerator = a << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        weight_x = (sint32)numerator / (sint32)divisor;
        numerator = b << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        weight_z = (sint32)numerator / (sint32)divisor;
    }
    a = (sint32)(command[0] * weight_x) / 4096;
    b = (sint32)(command[2] * weight_z) / 4096;
    a = (sint32)(a * strength) / 4096;
    b = (sint32)(b * strength) / 4096;
    c = (sint32)(command[1] * (4096 - stall)) / 4096;
    desired[0] = (sint32)(a * gain) / 4096;
    value = r_u32(settings + 8);
    desired[1] = (sint32)(c * value) / 4096;
    value = r_u32(settings + 12);
    desired[2] = (sint32)(b * value) / 4096;
    value = r_u32(settings + 16);
    a = r_s16(state + 48);
    current[0] = a + (uint32)((sint32)(desired[0] * value) / 4096);
    a = r_s16(state + 50);
    current[1] = a + (uint32)((sint32)(desired[1] * value) / 4096);
    a = r_s16(state + 52);
    current[2] = a + (uint32)((sint32)(desired[2] * value) / 4096);
    if ((sint32)current[0] < (sint32)(0 - gain))
        current[0] = 0 - gain;
    else if ((sint32)gain < (sint32)current[0])
        current[0] = gain;
    value = r_u32(settings + 8);
    if ((sint32)current[1] < (sint32)(0 - value))
        current[1] = 0 - value;
    else if ((sint32)value < (sint32)current[1])
        current[1] = value;
    value = r_u32(settings + 12);
    if ((sint32)current[2] < (sint32)(0 - value))
        current[2] = 0 - value;
    else if ((sint32)value < (sint32)current[2])
        current[2] = value;
    turn[0] = current[0];
    turn[1] = current[1];
    turn[2] = current[2];
    value = r_u32(settings + 16);
    if ((sint32)strength > 4096)
        damping = 4096 - value;
    else
    {
        damping = 4096 - (uint32)((sint32)(value * strength) / 4096);
        if ((sint32)damping > 4092)
            damping = 4092;
    }
    for (i = 0; i < 3; ++i)
    {
        a = (sint32)desired[i] < 0 ? 0 - desired[i] : desired[i];
        b = (sint32)current[i] < 0 ? 0 - current[i] : current[i];
        if ((sint32)a < (sint32)b)
        {
            current[i] = (sint32)(current[i] * damping) / 4096;
            b = (sint32)current[i] < 0 ? 0 - current[i] : current[i];
            if ((sint32)b < (sint32)a)
                current[i] = desired[i];
        }
    }
    if ((sint32)factor < 4096)
    {
        value = (sint32)factor < 128 ? 128 : factor;
        for (i = 0; i < 3; ++i)
            current[i] = (sint32)(current[i] * value) / 4096;
    }
    flags = r_u32(state + 4);
    w_u16(state + 48, current[0]);
    w_u16(state + 50, current[1]);
    w_u16(state + 52, current[2]);
    gain = r_u32(settings + 72);
    if ((flags & 8) != 0)
    {
        gain = (sint32)gain >> 1;
        bound0 = 40960;
        bound1 = 65536;
        bound2 = 106496;
    }
    else if ((flags & 0x100) != 0)
    {
        bound0 = 32768;
        bound1 = 104960;
        bound2 = 225280;
    }
    else if ((flags & 0xC000) != 0)
    {
        gain = (sint32)gain >> 1;
        bound0 = 4096;
        bound1 = 61440;
        bound2 = 245760;
    }
    if ((sint32)bound0 < 0)
        bound0 = 0;
    throttle = r_s16(control + 14);
    if ((sint32)throttle < 0 || (sint32)capped_strength < 4096)
        throttle = r_s16(control + 14);
    else
    {
        value = r_s16(state + 56);
        if ((sint32)value < 0)
            value = 0 - value;
        throttle += value << 2;
        if ((sint32)throttle < -4096)
            throttle = (uint32)-4096;
        if ((sint32)throttle > 4096)
            throttle = 4096;
    }
    thrust = sub_8006e908(speed, bound2, bound1, throttle, r_u32(object + 12), gain, gain);
    if ((sint32)thrust < -8192)
        thrust = (uint32)-8192;
    else if ((sint32)thrust > 8192)
        thrust = 8192;
    a = r_u16(object + 24);
    b = r_u32(settings + 80);
    c = r_u16(0x800104AC + (a & 0x3FFF) * 2);
    c = 4096 - (uint32)((sint32)(sint16)c >> 2);
    d = r_u32(settings + 76);
    value = (sint16)a > 0 ? (uint32)((sint32)((d - b) * c) / 4096) + b : d;
    if ((sint32)value < (sint32)thrust)
        thrust = value;
    if ((sint32)thrust < (sint32)(0 - value))
        thrust = 0 - value;
    thrust = (sint32)(thrust * capped_strength) / 4096;
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
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    if ((sint32)b < 0 && (sint32)factor < 4096)
        b = (sint32)(b * factor) / 4096;
    acceleration[0] += a;
    acceleration[1] += b;
    acceleration[2] += c;
    w_u32(state + 72, speed);
    value = r_u32(settings + 112) << 12;
    value = (sint32)value / 4096;
    a = r_u32(object + 56);
    vector[0] = (sint32)((0 - a) * value) / 4096;
    a = r_u32(object + 60);
    vector[1] = (sint32)((0 - a) * value) / 4096;
    a = r_u32(object + 64);
    vector[2] = (sint32)((0 - a) * value) / 4096;
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
    value = sub_80083f40((sint32)vector[0] >> 1, (sint32)vector[1] >> 1, (sint32)vector[2] >> 1);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, value & 0xFFFF);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    a <<= 1;
    b <<= 1;
    c <<= 1;
    if ((sint32)factor < 4096 && (sint32)b < 0)
        b = (sint32)(b * factor) / 4096;
    acceleration[0] += a;
    acceleration[1] += b;
    acceleration[2] += c;
    if ((sint32)factor < 4096 && (sint32)vector[1] < 0)
        vector[1] = (sint32)(vector[1] * factor) / 4096;
    acceleration[0] += vector[0];
    acceleration[2] += vector[2];
    acceleration[1] += vector[1];
    value = r_u32(object + 56);
    w_u32(object + 56, value + acceleration[0]);
    value = r_u32(object + 60);
    w_u32(object + 60, value + acceleration[1]);
    value = r_u32(object + 64);
    w_u32(object + 64, value + acceleration[2]);
    sub_80031bb4(object + 56, state + 64, 0);
    w_u16(object + 48, turn[0]);
    w_u16(object + 50, turn[1]);
    w_u16(object + 52, turn[2]);
    sub_80030f64(object + 24, object + 48, NULL);
    sub_8006ee38(object, state);
    return sub_800293f0(4096 - capped_strength);
}

uint32 sub_8008e038(void)
{
    uint32 scene, camera, x, y, z, distance, color, value, a, b, c, d, e, packet, uv, table, size, index, output, depth, result, head, tail, address, lane, shift, i;
    FUNCTION_MARKER(0x8008E038, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    x = r_u32(camera + 156);
    y = r_u32(camera + 160);
    z = r_u32(camera + 164);
    x = (sint32)x >> 7;
    y = (sint32)y >> 7;
    z = (sint32)z >> 7;
    a = (sint32)x < 0 ? 0 - x : x;
    b = (sint32)y < 0 ? 0 - y : y;
    c = (sint32)z < 0 ? 0 - z : z;
    distance = a + b + c;
    result = distance - 512 < 0x2E01;
    w_u16(0x1F80001C, x);
    w_u16(0x1F80001E, y);
    w_u16(0x1F800020, z);
    if (result == 0)
        return result;
    color = 0x2E0F0F0F;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, color);
    value = (2048 - distance) << 2;
    if ((sint32)value > 0)
    {
        xport_gte_write_data(8, value);
        xport_gte_execute(0x780010);
        color = xport_gte_read_data(22);
        xport_gte_write_data(6, color);
    }
    a = r_u32(scene + 44);
    b = r_u32(scene + 48);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(scene + 52);
    d = r_u32(scene + 56);
    e = r_u32(scene + 60);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    packet = r_u32(0x1F800004);
    table = r_u32(0x800BCF90);
    xport_gte_execute(0x486012);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F80000C);
    x = 0 - (uint32)((sint32)a >> 9);
    y = 0 - (uint32)((sint32)b >> 9);
    a = r_u32(0x1F800010);
    b = r_u16(0x1F80001A);
    z = 0 - (uint32)((sint32)a >> 9);
    size = (uint32)(sint32)(sint16)b * 140;
    x -= xport_gte_read_data(25);
    y -= xport_gte_read_data(26);
    z -= xport_gte_read_data(27);
    a = r_u32(scene + 76);
    b = r_u32(scene + 80);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(scene + 84);
    d = r_u32(scene + 88);
    e = r_u32(scene + 92);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(7, 4096);
    value = (sint32)(distance - 2048) >> 8;
    if ((sint32)value < 0)
        value = 0;
    else if ((sint32)value > 10)
        value = 10;
    xport_gte_write_control(26, 160 - value);
    output = packet + 4;
    for (index = 0; index < 16; ++index)
    {
        value = r_u16(table);
        w_u16(0x1F800024, ((value + x) & 0x3FFF) - 8192);
        value = r_u16(table + 2);
        w_u16(0x1F800026, ((value + y) & 0x3FFF) - 8192);
        value = r_u16(table + 4);
        w_u16(0x1F800028, ((value + z) & 0x3FFF) - 8192);
        a = r_u32(0x1F800024);
        xport_gte_write_data(0, a);
        b = r_u32(0x1F800028);
        xport_gte_write_data(1, b);
        a = r_u16(0x1F800024);
        b = r_u16(0x1F80001C);
        c = r_u16(0x1F800026);
        d = r_u16(0x1F80001E);
        w_u16(0x1F80002C, a - b);
        a = r_u16(0x1F800028);
        b = r_u16(0x1F800020);
        w_u16(0x1F80002E, c - d);
        c = r_u16(0x1F800024);
        d = r_u16(0x1F80001C);
        w_u16(0x1F800030, a - b);
        a = r_u16(0x1F800026);
        b = r_u16(0x1F80001E);
        w_u16(0x1F800034, c + d);
        c = r_u16(0x1F800028);
        d = r_u16(0x1F800020);
        w_u16(0x1F800036, a + b);
        w_u16(0x1F800038, c + d);
        a = r_u32(0x1F80002C);
        xport_gte_write_data(2, a);
        b = r_u32(0x1F800030);
        xport_gte_write_data(3, b);
        a = r_u32(0x1F800034);
        xport_gte_write_data(4, a);
        b = r_u32(0x1F800038);
        xport_gte_write_data(5, b);
        w_u32(packet, ((packet + 40) & 0xFFFFFF) | 0x09000000);
        xport_gte_execute(0x280030);
        uv = (index << 1) & 0xE0;
        w_u32(output + 16, (uv + 63) | 0x0037C000);
        w_u32(output + 8, uv | 0x40D6C000);
        w_u16(output + 24, uv | 0xFF00);
        w_u16(output + 32, (uv + 63) | 0xFF00);
        value = xport_gte_read_control(31);
        if ((sint32)value < 0)
            goto next_particle_8e038;
        value = xport_gte_read_data(13);
        w_u32(0x1F800044, value);
        value = r_s16(0x1F800044);
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 241)
            goto next_particle_8e038;
        value = r_s16(0x1F800046);
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 201)
            goto next_particle_8e038;
        w_u32(output, color);
        value = xport_gte_read_data(14);
        w_u32(0x1F800048, value);
        depth = xport_gte_read_data(18);
        if (depth != 0)
        {
            if (depth == 0xFFFFFFFF && size == 0x80000000)
                xport_mips_break(6);
            value = (sint32)size / (sint32)depth;
            w_u32(0x1F80004C, value);
        }
        else
            w_u32(0x1F80004C, 0);
        depth = xport_gte_read_data(19);
        if (depth != 0)
        {
            if (depth == 0xFFFFFFFF && size == 0x80000000)
                xport_mips_break(6);
            value = (sint32)size / (sint32)depth;
            w_u32(0x1F800050, value);
        }
        else
            w_u32(0x1F800050, 0);
        sub_8008de18(packet, 0x1F80003C);
        output += 40;
        packet += 40;
    next_particle_8e038:
        table += 8;
    }
    result = 0;
    head = r_u32(0x1F800004);
    tail = packet - 40;
    if (head != packet)
    {
        result = r_u32(0x1F800000) + 112;
        /* DMA packets and ordering heads are word aligned; LWL low byte is discarded by SWL */
        address = result + 2;
        lane = address & 3;
        value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
        address = tail + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        value = head << 8;
        address = result + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        w_u32(0x1F800004, packet);
    }
    value = r_u16(0x1F80001A);
    xport_gte_write_control(26, value);
    xport_gte_write_control(7, 0);
    return result;
}

/* Native matrix-column sink for the original geometry preparation */
static void geometry_rotate_columns(const MATRIX *matrix, uint8 *output)
{
    uint32 column, a, b, c;
    for (column = 0; column < 3; ++column)
    {
        a = xport_load_le16((const uint8 *)matrix + column * 2);
        b = xport_load_le16((const uint8 *)matrix + column * 2 + 6);
        c = xport_load_le16((const uint8 *)matrix + column * 2 + 12);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        xport_store_le16(output + column * 2, a);
        xport_store_le16(output + column * 2 + 6, b);
        xport_store_le16(output + column * 2 + 12, c);
    }
}

uint32 sub_80067494(uint32 scene_data, const uint8 *descriptors, uint8 *meshes, uint8 *animated, uint8 *fixed, uint8 *sprites, uint32 hits, uint8 *native_hits)
{
    MATRIX matrix, inverse;
    uint16 angles[3];
    uint8 *output;
    uint32 scene, camera, focal, range, half_width, half_height, position, dx, dy, dz, x, y, z, distance, selection, smooth, level, model, vertices, table, source, a, b, c, d, e, value, radius, depth, screen, screen_x, screen_y, margin, count, offset, i, kind;
    FUNCTION_MARKER(0x80067494, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    focal = r_s16(camera + 36);
    range = focal * 12;
    a = r_u16(camera + 32);
    b = r_u16(camera + 34);
    half_width = (sint32)(a << 16) >> 17;
    half_height = (sint32)(b << 16) >> 17;
    while (descriptors[0] != 255)
    {
        position = xport_load_le32(descriptors + 8);
        a = r_u32(position);
        b = r_u32(camera + 4);
        dx = a - b;
        a = r_u32(position + 4);
        b = r_u32(camera + 8);
        dy = a - b;
        a = r_u32(position + 8);
        b = r_u32(camera + 12);
        dz = a - b;
        x = (sint32)dx >> 10;
        y = (sint32)dy >> 10;
        z = (sint32)dz >> 10;
        a = (sint32)x < 0 ? 0 - x : x;
        b = (sint32)y < 0 ? 0 - y : y;
        c = (sint32)z < 0 ? 0 - z : z;
        distance = a + b + c;
        if ((sint32)distance > 139288)
            goto next_geometry_67494;
        if ((descriptors[4] & 15) != 0)
        {
            selection = descriptors[0] << 2;
            smooth = r_u32(0x800BD090);
            if ((sint32)(range << 2) < (sint32)distance)
                selection += 2;
            else if ((sint32)(range << 1) < (sint32)distance)
                ++selection;
            table = r_u32(scene_data + 12);
            model = r_u32(table + (selection << 2));
            if ((sint32)range < (sint32)distance)
            {
                level = 0;
                vertices = r_u32(model + 4);
            }
            else if (((sint32)range >> 1) < (sint32)distance)
            {
                level = 1;
                vertices = r_u32(model + 8);
            }
            else
            {
                level = 2;
                vertices = r_u32(model + 12);
            }
            x = (sint32)x >> (2 - level);
            y = (sint32)y >> (2 - level);
            z = (sint32)z >> (2 - level);
            a = r_u32(scene + 76);
            b = r_u32(scene + 80);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = r_u32(scene + 84);
            d = r_u32(scene + 88);
            e = r_u32(scene + 92);
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            xport_gte_execute(0x180001);
            if ((descriptors[4] & 9) != 0)
            {
                meshes[0] = descriptors[1];
                value = xport_load_le16(descriptors + 2);
                xport_store_le16(meshes + 2, value);
                table = r_u32(scene_data + 12);
                xport_store_le32(meshes + 8, level);
                xport_store_le32(meshes + 16, model);
                xport_store_le32(meshes + 12, vertices);
                xport_store_le32(meshes + 4, table + (selection << 2));
                value = r_u32(0x800B2C48 + ((uint32)descriptors[6] << 2));
                xport_store_le32(meshes + 52, value);
                a = xport_gte_read_data(25);
                xport_store_le32(meshes + 40, a);
                b = xport_gte_read_data(26);
                xport_store_le32(meshes + 44, b);
                c = xport_gte_read_data(27);
                xport_store_le32(meshes + 48, c);
            }
            if ((descriptors[4] & 2) != 0)
            {
                output = animated;
                xport_store_le32(output + 152, smooth);
                value = r_u32(0x800B2C60 + ((uint32)descriptors[7] << 2));
                xport_store_le32(output + 156, value);
            }
            else
            {
                output = fixed;
                xport_store_le32(output + 152, 0);
                xport_store_le32(output + 156, 0);
            }
            table = r_u32(scene_data + 12);
            source = r_u32(table + 12);
            for (i = 0; i < 2; ++i)
            {
                a = r_u32(source + i * 4);
                value = r_u32(a + (level + 1) * 4);
                xport_store_le32(output + 4 + i * 4, value);
                xport_store_le32(output + 24 + i * 4, a);
            }
            value = r_u32(source + 8);
            xport_store_le32(output + 32, value);
            value = r_u32(source + 28);
            xport_store_le32(output + 36, value);
            value = r_u32(source + 36);
            xport_store_le32(output + 40, value);
            if ((descriptors[4] & 2) != 0)
            {
                for (i = 0; i < 10; ++i)
                {
                    a = r_u32(source + 8 + i * 4);
                    value = r_u32(a + (level + 1) * 4);
                    xport_store_le32(output + 44 + i * 4, value);
                    if (i == 0)
                        xport_store_le32(output + 12, value);
                    if (i == 5)
                        xport_store_le32(output + 16, value);
                    if (i == 7)
                        xport_store_le32(output + 20, value);
                }
            }
            else
            {
                a = r_u32(source + 8);
                b = r_u32(source + 28);
                c = r_u32(source + 36);
                a = r_u32(a + (level + 1) * 4);
                b = r_u32(b + (level + 1) * 4);
                c = r_u32(c + (level + 1) * 4);
                xport_store_le32(output + 20, c);
                xport_store_le32(output + 12, a);
                xport_store_le32(output + 16, b);
            }
            xport_store_le32(output, level);
            a = xport_gte_read_data(25);
            xport_store_le32(output + 108, a);
            b = xport_gte_read_data(26);
            xport_store_le32(output + 112, b);
            c = xport_gte_read_data(27);
            xport_store_le32(output + 116, c);
            depth = (sint32)xport_gte_read_data(27) >> level;
            value = r_s16(model + 2);
            radius = (sint32)value >> (2 - level);
            if ((sint32)depth < (sint32)(0 - radius))
                goto next_geometry_67494;
            screen = xport_gte_read_data(14);
            kind = descriptors[4];
            if ((kind & 2) != 0)
                xport_store_le32(animated + 84, depth);
            else if ((kind & 4) != 0)
                xport_store_le32(fixed + 84, depth);
            if ((sint32)depth <= 0)
                depth = 1;
            margin = (sint32)(radius * focal) / (sint32)depth;
            screen_x = (sint32)(sint16)screen;
            screen_y = (sint32)screen >> 16;
            if ((sint32)(radius << 4) < (sint32)margin)
                margin = radius << 2;
            value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
            if ((sint32)(half_width + margin) < (sint32)value)
                goto next_geometry_67494;
            value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
            if ((sint32)(half_height + margin) < (sint32)value)
                goto next_geometry_67494;
            position = xport_load_le32(descriptors + 8);
            rotation_build(position + 16, 0, &matrix, 0);
            value = xport_load_le32(descriptors + 12);
            if (value != 0)
            {
                count = native_hits ? xport_load_le32(native_hits) : r_u32(hits);
                offset = count << 3;
                if (native_hits)
                {
                    xport_store_le32(native_hits + offset + 4, value);
                    xport_store_le16(native_hits + offset + 8, screen_x);
                    xport_store_le16(native_hits + offset + 10, screen_y);
                    xport_store_le32(native_hits, count + 1);
                }
                else
                {
                    w_u32(hits + offset + 4, value);
                    w_u16(hits + offset + 8, screen_x);
                    w_u16(hits + offset + 10, screen_y);
                    w_u32(hits, count + 1);
                }
            }
            kind = descriptors[4];
            if ((kind & 9) != 0)
            {
                geometry_rotate_columns(&matrix, meshes + 20);
                meshes += 56;
            }
            else
            {
                if ((kind & 2) != 0)
                {
                    if (descriptors[5] != 0)
                    {
                        a = r_s16(camera + 180);
                        b = r_s16(camera + 182);
                        value = r_u16(camera + 180);
                        if ((sint32)a < 0)
                            a = 0 - a;
                        if ((sint32)b < 0)
                            b = 0 - b;
                        c = (a + b) << 2;
                        if ((sint32)c > 4096)
                            c = 4096;
                        c = 4096 - c;
                        a = r_u16(smooth + 2);
                        a = (uint32)((sint32)(a << 16) >> 20);
                        a = (sint32)(a * c) / 4096;
                        b = r_u16(smooth + 4);
                        b = (uint32)((sint32)(b << 16) >> 22);
                        b = (sint32)(b * c) / 4096;
                        d = r_u16(smooth);
                        d = (uint32)((sint32)(d << 16) >> 19);
                        d = (sint32)(d * c) / 4096;
                        angles[0] = value - a;
                        value = r_u16(camera + 182);
                        angles[1] = value - b;
                        value = r_u16(camera + 184);
                        angles[2] = value - d;
                        sub_800308f0(0, 0, &inverse, angles);
                        for (i = 0; i < 9; ++i)
                            xport_store_le16(animated + 88 + i * 2, xport_load_le16((uint8 *)&inverse + i * 2));
                        value = r_s16(camera + 44);
                        xport_store_le32(animated + 108, (sint32)(0 - value) >> (6 - level));
                        value = r_s16(camera + 46);
                        xport_store_le32(animated + 112, (sint32)(0 - value) >> (6 - level));
                        value = r_s16(camera + 48);
                        xport_store_le32(animated + 116, (sint32)(0 - value) >> (6 - level));
                    }
                    else
                        geometry_rotate_columns(&matrix, animated + 88);
                    output = animated;
                }
                else
                {
                    geometry_rotate_columns(&matrix, fixed + 88);
                    output = fixed;
                }
                a = r_u32(scene + 108);
                b = r_u32(scene + 112);
                xport_gte_write_control(0, a);
                xport_gte_write_control(1, b);
                c = r_u32(scene + 116);
                d = r_u32(scene + 120);
                e = r_u32(scene + 124);
                xport_gte_write_control(2, c);
                xport_gte_write_control(3, d);
                xport_gte_write_control(4, e);
                geometry_rotate_columns(&matrix, output + 120);
                if ((kind & 2) != 0)
                    animated += 160;
                else
                    fixed += 160;
            }
        }
        else
        {
            x = (sint32)dx >> 12;
            y = (sint32)dy >> 12;
            z = (sint32)dz >> 12;
            a = r_u32(scene + 76);
            b = r_u32(scene + 80);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = r_u32(scene + 84);
            d = r_u32(scene + 88);
            e = r_u32(scene + 92);
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            xport_gte_execute(0x180001);
            sprites[0] = descriptors[1];
            value = xport_load_le16(descriptors + 2);
            xport_store_le16(sprites + 2, value);
            kind = descriptors[0];
            table = r_u32(scene_data + 20);
            source = table + (kind << 2);
            model = r_u32(source);
            xport_store_le32(sprites + 4, source);
            xport_store_le32(sprites + 16, model);
            x = xport_gte_read_data(25);
            y = xport_gte_read_data(26);
            z = xport_gte_read_data(27);
            xport_store_le16(sprites + 8, x);
            xport_store_le16(sprites + 10, y);
            xport_store_le16(sprites + 12, z);
            depth = xport_gte_read_data(27);
            radius = r_s16(model + 2);
            if ((sint32)depth < (sint32)(0 - radius))
                goto next_geometry_67494;
            screen = xport_gte_read_data(14);
            if ((sint32)depth <= 0)
                depth = 1;
            margin = (sint32)(radius * focal) / (sint32)depth;
            screen_x = (sint32)(sint16)screen;
            screen_y = (sint32)screen >> 16;
            if ((sint32)(radius << 4) < (sint32)margin)
                margin = radius << 2;
            value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
            if ((sint32)(half_width + margin) < (sint32)value)
                goto next_geometry_67494;
            value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
            if ((sint32)(half_height + margin) < (sint32)value)
                goto next_geometry_67494;
            sprites += 20;
            value = xport_load_le32(descriptors + 12);
            if (value != 0)
            {
                count = native_hits ? xport_load_le32(native_hits) : r_u32(hits);
                offset = count << 3;
                if (native_hits)
                {
                    xport_store_le32(native_hits + offset + 4, value);
                    xport_store_le16(native_hits + offset + 8, screen_x);
                    xport_store_le16(native_hits + offset + 10, screen_y);
                    xport_store_le32(native_hits, count + 1);
                }
                else
                {
                    w_u32(hits + offset + 4, value);
                    w_u16(hits + offset + 8, screen_x);
                    w_u16(hits + offset + 10, screen_y);
                    w_u32(hits, count + 1);
                }
            }
        }
    next_geometry_67494:
        descriptors += 16;
    }
    xport_store_le32(meshes + 8, 0xFFFFFFFF);
    xport_store_le32(animated, 0xFFFFFFFF);
    xport_store_le32(fixed, 0xFFFFFFFF);
    xport_store_le32(sprites + 16, 0);
    return 0xFFFFFFFF;
}
