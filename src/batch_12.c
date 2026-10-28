#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8004ceb0(uint32 kind)
{
    uint32 root, current, record, value, flags, count, i, field, slot;
    FUNCTION_MARKER(0x8004CEB0, "ACE3.BIN");
    if (kind == 255)
        return 255;
    root = r_u32(0x800BCFEC);
    current = r_u32(root + 0x11C);
    if (current == kind)
        return 0x800B0000;
    record = 0x800B1A10 + kind * 28;
    w_u32(root + 0x11C, kind);
    value = r_u8(record + 1);
    w_u32(root + 0x114, value);
    value = r_s16(record + 2);
    w_u32(root + 0x110, 0);
    w_u32(root + 0x118, value);
    flags = r_u8(record);
    if ((flags & 1) != 0)
    {
        value = r_u8(record + 24);
        sub_8002f19c(value, 30);
    }
    else if ((flags & 2) != 0)
    {
        resource_cancel_streams();
        value = r_u32(record + 24);
        resource_queue_audio(0x800B19B0 + value * 12, 0, 0);
    }
    count = r_u32(record + 4);
    i = 0;
    field = record + 8;
    while ((sint32)i < (sint32)count)
    {
        sub_8004d038(field);
        count = r_u32(record + 4);
        ++i;
        field += 8;
    }
    root = r_u32(0x800BCFEC);
    i = r_u32(root + 0x110);
    while ((sint32)i < 32)
    {
        slot = i << 3;
        root = r_u32(0x800BCFEC);
        ++i;
        w_u8(root + slot + 8, 0);
    }
    root = r_u32(0x800BCFEC);
    count = r_u32(root + 0x110);
    i = 0;
    if ((sint32)count <= 0)
        return count;
    do
    {
        w_u8(root + (i << 3) + 8, 2);
        root = r_u32(0x800BCFEC);
        count = r_u32(root + 0x110);
        ++i;
    } while ((sint32)i < (sint32)count);
    return i << 3;
}

uint32 sub_80050720(uint32 index)
{
    uint32 root, record, selected, a, b, c, d, flags, result, mode, parameters, current, threshold;
    FUNCTION_MARKER(0x80050720, "ACE3.BIN");
    root = r_u32(0x800BCEC0);
    record = root + index * 184;
    selected = r_u8(record + 96);
    if (selected == 255)
    {
        w_u8(record + 61, 0);
        w_u8(record + 63, selected);
        a = r_u32(record + 32);
        b = r_u32(record + 36);
        c = r_u32(record + 40);
        d = r_u32(record + 44);
        w_u32(record + 68, a);
        w_u32(record + 72, b);
        w_u32(record + 76, c);
        w_u32(record + 80, d);
        flags = r_u32(record);
        if ((flags & 0x2000000) != 0)
            return 1;
        result = sub_80046124(index);
        w_u8(record + 96, result);
        if ((result & 255) == 255)
            return 1;
        selected = r_u8(record + 96);
        w_u8(record + 61, 1);
        w_u32(record + 76, 0);
        w_u32(record + 72, 0);
        w_u32(record + 68, 0);
        w_u8(record + 63, selected);
        return 1;
    }
    mode = r_u8(record + 60);
    if (mode == 0)
        return 1;
    if (mode == 1)
    {
        flags = r_u32(record);
        if ((flags & 0x1000000) == 0)
        {
            root = r_u32(0x800BCEF8);
            parameters = r_u32(root + (index << 6) + 28);
            current = r_u32(record + 160);
            threshold = r_u32(parameters + 8);
            if (current < threshold)
                w_u8(record + 61, 4);
            return 1;
        }
    }
    else if (mode == 4)
    {
        flags = r_u32(record);
        if ((flags & 0x1000000) == 0)
        {
            root = r_u32(0x800BCEF8);
            parameters = r_u32(root + (index << 6) + 28);
            current = r_u32(record + 160);
            threshold = r_u32(parameters + 8);
            if (threshold < current)
                w_u8(record + 61, 1);
            return 1;
        }
    }
    result = sub_800464b0(index);
    w_u8(record + 96, result);
    w_u8(record + 61, 0);
    w_u8(record + 63, 255);
    a = r_u32(record + 32);
    b = r_u32(record + 36);
    c = r_u32(record + 40);
    d = r_u32(record + 44);
    w_u32(record + 68, a);
    w_u32(record + 72, b);
    w_u32(record + 76, c);
    w_u32(record + 80, d);
    return 1;
}

uint32 sub_8006305c(uint32 state)
{
    uint32 model, packet, ordering, count, record, i, indices, first, second, third, value, a, b, c, shift, bias, bucket, head;
    FUNCTION_MARKER(0x8006305C, "ACE3.BIN");
    model = r_u32(state + 8);
    packet = r_u32(state + 4);
    ordering = r_u32(state + 0x3F8);
    count = r_u32(model);
    record = model + 4;
    if ((sint32)count <= 0)
    {
        w_u32(state + 4, packet);
        return model;
    }
    for (i = 0; (sint32)i < (sint32)count; ++i, record += 32)
    {
        indices = r_u32(record + 4);
        first = indices & 255;
        second = (indices >> 8) & 255;
        third = indices >> 16;
        value = r_u32(state + 16 + (first << 2));
        xport_gte_write_data(12, value);
        value = r_u32(state + 16 + (second << 2));
        xport_gte_write_data(13, value);
        value = r_u32(state + 16 + (third << 2));
        xport_gte_write_data(14, value);
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        if ((sint32)value <= 0)
            continue;
        a = r_s16(state + 0x2A4 + (first << 1));
        c = r_s16(state + 0x2A4 + (third << 1));
        b = r_s16(state + 0x2A4 + (second << 1));
        xport_gte_write_data(17, a);
        xport_gte_write_data(18, b);
        xport_gte_write_data(19, c);
        xport_gte_execute(0x158002D);
        value = xport_gte_read_data(24);
        shift = r_u32(state + 12);
        value = (sint32)value >> ((shift + 10) & 31);
        bias = r_s16(state + 0x3F6);
        value = value + 4 - bias;
        if (value + 75 >= 152)
            continue;
        bucket = ordering + ((uint32)((sint32)value >> 2) << 2);
        head = r_u32(bucket);
        w_u32(packet, head | 0x09000000);
        w_u32(bucket, packet & 0xFFFFFF);
        value = xport_gte_read_data(12);
        w_u32(packet + 8, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 20, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 32, value);
        value = r_u32(record + 8);
        xport_gte_write_data(6, value);
        xport_gte_write_data(20, value);
        value = r_u32(record + 12);
        xport_gte_write_data(21, value);
        value = r_u32(record + 16);
        xport_gte_write_data(22, value);
        xport_gte_execute(0xF8002A);
        value = r_u32(record + 20);
        w_u32(packet + 12, value);
        value = r_u32(record + 24);
        w_u32(packet + 24, value);
        value = r_u32(record + 28);
        w_u32(packet + 36, value);
        value = xport_gte_read_data(20);
        w_u32(packet + 4, value);
        value = xport_gte_read_data(21);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(22);
        w_u32(packet + 28, value);
        packet += 40;
    }
    w_u32(state + 4, packet);
    return 0;
}

uint32 sub_800631f8(uint32 state)
{
    uint32 model, packet, ordering, count, record, i, indices, first, second, third, fourth, first_xy, fourth_xy, value, a, b, c, d, bias, shift, bucket, head, visible;
    FUNCTION_MARKER(0x800631F8, "ACE3.BIN");
    model = r_u32(state + 8);
    packet = r_u32(state + 4);
    ordering = r_u32(state + 0x3F8);
    count = r_u32(model);
    record = model + 4;
    if ((sint32)count <= 0)
    {
        w_u32(state + 4, packet);
        return model;
    }
    for (i = 0; (sint32)i < (sint32)count; ++i, record += 36)
    {
        indices = r_u32(record + 4);
        first = indices & 255;
        second = (indices >> 8) & 255;
        first_xy = r_u32(state + 16 + (first << 2));
        value = r_u32(state + 16 + (second << 2));
        xport_gte_write_data(13, value);
        third = (indices >> 16) & 255;
        value = r_u32(state + 16 + (third << 2));
        xport_gte_write_data(14, value);
        fourth = indices >> 24;
        xport_gte_write_data(12, first_xy);
        fourth_xy = r_u32(state + 16 + (fourth << 2));
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        visible = (sint32)value > 0;
        if (visible == 0)
        {
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            if ((sint32)value >= 0)
                continue;
        }
        a = r_s16(state + 0x2A4 + (first << 1));
        b = r_s16(state + 0x2A4 + (second << 1));
        c = r_s16(state + 0x2A4 + (third << 1));
        d = r_s16(state + 0x2A4 + (fourth << 1));
        bias = r_s16(state + 0x3F6);
        shift = r_u32(state + 12);
        value = (sint32)(a + b + c + d) >> 2;
        value = (sint32)value >> (shift & 31);
        value = value + 4 - bias;
        if (value + 75 >= 152)
            continue;
        bucket = ordering + ((uint32)((sint32)value >> 2) << 2);
        head = r_u32(bucket);
        w_u32(packet, head | 0x0C000000);
        w_u32(bucket, packet & 0xFFFFFF);
        w_u32(packet + 8, first_xy);
        value = xport_gte_read_data(13);
        w_u32(packet + 20, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 32, value);
        value = xport_gte_read_data(12);
        w_u32(packet + 44, value);
        value = r_u32(record + 8);
        xport_gte_write_data(6, value);
        xport_gte_execute(0x780010);
        value = r_u32(record + 24);
        w_u32(packet + 12, value);
        value = r_u32(record + 28);
        w_u32(packet + 24, value);
        value = xport_gte_read_data(22);
        w_u32(packet + 4, value);
        value = r_u32(record + 12);
        xport_gte_write_data(20, value);
        value = r_u32(record + 16);
        xport_gte_write_data(21, value);
        value = r_u32(record + 20);
        xport_gte_write_data(22, value);
        xport_gte_execute(0xF8002A);
        value = r_u32(record + 32);
        w_u32(packet + 36, value);
        w_u32(packet + 48, value >> 16);
        value = xport_gte_read_data(20);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(21);
        w_u32(packet + 28, value);
        value = xport_gte_read_data(22);
        w_u32(packet + 40, value);
        packet += 52;
    }
    w_u32(state + 4, packet);
    return 0;
}

uint32 sub_800633d8(uint32 state)
{
    uint32 packet, model, ordering, normals, count, record, i, indices, first, second, third, a, b, c, value, shift, bias, bucket, head, n0, n1, n2;
    FUNCTION_MARKER(0x800633D8, "ACE3.BIN");
    packet = r_u32(state + 4);
    model = r_u32(state + 8);
    ordering = r_u32(state + 0x3F8);
    normals = r_u32(state + 0x3F0);
    count = r_u32(model);
    xport_gte_write_data(6, 0x34808080);
    record = model + 4;
    if ((sint32)count <= 0)
    {
        w_u32(state + 4, packet);
        return model;
    }
    for (i = 0; (sint32)i < (sint32)count; ++i, record += 24)
    {
        indices = r_u32(record + 4);
        first = indices & 255;
        second = (indices >> 8) & 255;
        third = indices >> 16;
        value = r_u32(state + 16 + (first << 2));
        xport_gte_write_data(12, value);
        value = r_u32(state + 16 + (second << 2));
        xport_gte_write_data(13, value);
        value = r_u32(state + 16 + (third << 2));
        xport_gte_write_data(14, value);
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        if ((sint32)value <= 0)
            continue;
        a = r_s16(state + 0x2A4 + (first << 1));
        b = r_s16(state + 0x2A4 + (second << 1));
        c = r_s16(state + 0x2A4 + (third << 1));
        xport_gte_write_data(17, a);
        xport_gte_write_data(18, b);
        xport_gte_write_data(19, c);
        xport_gte_execute(0x158002D);
        value = xport_gte_read_data(24);
        shift = r_u32(state + 12);
        bias = r_s16(state + 0x3F6);
        value = ((sint32)value >> ((shift + 10) & 31)) - bias;
        if (value + 75 >= 152)
            continue;
        n0 = r_u8(record + 8);
        n1 = r_u8(record + 9);
        n2 = r_u8(record + 10);
        n0 = normals + (n0 << 3);
        n1 = normals + (n1 << 3);
        n2 = normals + (n2 << 3);
        a = r_u32(n0);
        xport_gte_write_data(0, a);
        a = r_u32(n0 + 4);
        xport_gte_write_data(1, a);
        a = r_u32(n1);
        xport_gte_write_data(2, a);
        a = r_u32(n1 + 4);
        xport_gte_write_data(3, a);
        a = r_u32(n2);
        xport_gte_write_data(4, a);
        a = r_u32(n2 + 4);
        xport_gte_write_data(5, a);
        xport_gte_execute(0xD80420);
        bucket = ordering + ((uint32)((sint32)value >> 2) << 2);
        head = r_u32(bucket);
        w_u32(packet, head | 0x09000000);
        w_u32(bucket, packet & 0xFFFFFF);
        a = r_u32(record + 12);
        b = r_u32(record + 16);
        c = r_u32(record + 20);
        w_u32(packet + 12, a);
        w_u32(packet + 24, b);
        w_u32(packet + 36, c);
        a = xport_gte_read_data(12);
        w_u32(packet + 8, a);
        a = xport_gte_read_data(13);
        w_u32(packet + 20, a);
        a = xport_gte_read_data(14);
        w_u32(packet + 32, a);
        a = xport_gte_read_data(20);
        w_u32(packet + 4, a);
        a = xport_gte_read_data(21);
        w_u32(packet + 16, a);
        a = xport_gte_read_data(22);
        w_u32(packet + 28, a);
        packet += 40;
    }
    w_u32(state + 4, packet);
    return 0;
}

uint32 sub_8006358c(uint32 state)
{
    uint32 packet, model, ordering, normals, count, record, i, indices, first, second, third, fourth, first_xy, fourth_xy, a, b, c, d, value, shift, bias, bucket, head, n0, n1, n2, n3;
    FUNCTION_MARKER(0x8006358C, "ACE3.BIN");
    packet = r_u32(state + 4);
    model = r_u32(state + 8);
    ordering = r_u32(state + 0x3F8);
    normals = r_u32(state + 0x3F0);
    count = r_u32(model);
    xport_gte_write_data(6, 0x3C808080);
    record = model + 4;
    if ((sint32)count <= 0)
    {
        w_u32(state + 4, packet);
        return model;
    }
    for (i = 0; (sint32)i < (sint32)count; ++i, record += 24)
    {
        indices = r_u32(record + 4);
        first = indices & 255;
        second = (indices >> 8) & 255;
        first_xy = r_u32(state + 16 + (first << 2));
        value = r_u32(state + 16 + (second << 2));
        xport_gte_write_data(13, value);
        third = (indices >> 16) & 255;
        value = r_u32(state + 16 + (third << 2));
        xport_gte_write_data(14, value);
        fourth = indices >> 24;
        xport_gte_write_data(12, first_xy);
        fourth_xy = r_u32(state + 16 + (fourth << 2));
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        if ((sint32)value <= 0)
        {
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            if ((sint32)value >= 0)
                continue;
        }
        a = r_s16(state + 0x2A4 + (first << 1));
        b = r_s16(state + 0x2A4 + (second << 1));
        c = r_s16(state + 0x2A4 + (third << 1));
        d = r_s16(state + 0x2A4 + (fourth << 1));
        bias = r_s16(state + 0x3F6);
        shift = r_u32(state + 12);
        value = (sint32)(a + b + c + d) >> 2;
        value = ((sint32)value >> (shift & 31)) - bias;
        if (value + 75 >= 152)
            continue;
        n0 = normals + (r_u8(record + 8) << 3);
        a = r_u32(n0);
        xport_gte_write_data(0, a);
        a = r_u32(n0 + 4);
        xport_gte_write_data(1, a);
        xport_gte_execute(0xC8041E);
        n1 = r_u8(record + 9);
        n2 = r_u8(record + 10);
        n3 = r_u8(record + 11);
        n1 = normals + (n1 << 3);
        n2 = normals + (n2 << 3);
        n3 = normals + (n3 << 3);
        a = r_u32(n3);
        xport_gte_write_data(4, a);
        a = r_u32(n3 + 4);
        xport_gte_write_data(5, a);
        a = r_u32(n2);
        xport_gte_write_data(2, a);
        a = r_u32(n2 + 4);
        xport_gte_write_data(3, a);
        a = xport_gte_read_data(22);
        w_u32(packet + 4, a);
        a = r_u32(n1);
        xport_gte_write_data(0, a);
        a = r_u32(n1 + 4);
        xport_gte_write_data(1, a);
        xport_gte_execute(0xD80420);
        bucket = ordering + ((uint32)((sint32)value >> 2) << 2);
        head = r_u32(bucket);
        w_u32(packet, head | 0x0C000000);
        w_u32(bucket, packet & 0xFFFFFF);
        a = r_u32(record + 12);
        w_u32(packet + 8, first_xy);
        w_u32(packet + 12, a);
        c = r_u32(record + 20);
        b = r_u32(record + 16);
        w_u32(packet + 36, c);
        w_u32(packet + 24, b);
        w_u32(packet + 48, c >> 16);
        a = xport_gte_read_data(13);
        w_u32(packet + 20, a);
        a = xport_gte_read_data(14);
        w_u32(packet + 32, a);
        a = xport_gte_read_data(12);
        w_u32(packet + 44, a);
        a = xport_gte_read_data(20);
        w_u32(packet + 16, a);
        a = xport_gte_read_data(21);
        w_u32(packet + 28, a);
        a = xport_gte_read_data(22);
        w_u32(packet + 40, a);
        packet += 52;
    }
    w_u32(state + 4, packet);
    return 0;
}

uint32 sub_800660f8(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 result, height, angle, sine, cosine, numerator, scale, base, packet, center_x, center_y, left, right, top, bottom, edge_y, scene, ordering, tag, head;
    FUNCTION_MARKER(0x800660F8, "ACE3.BIN");
    result = r_u32(0x800C6D78);
    if ((sint32)result <= 0)
        return result;
    height = sub_800a70e8();
    angle = (native_state ? native_state->words[8] : r_u32(state + 32)) & 0x3FFF;
    sine = r_s16(0x800104AC + (angle << 1));
    numerator = height * sine;
    cosine = r_s16(0x800104AC + ((angle + 0x1000) << 1));
    if (cosine == 0)
        xport_mips_break(7);
    if (cosine == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    scale = (sint32)numerator / (sint32)cosine;
    base = r_u32(0x800BD078);
    center_y = r_s16(0x800C6D76);
    scale = (sint32)((scale << 1) + scale) >> 2;
    center_x = r_s16(0x800C6D74);
    top = center_y - scale;
    left = center_x - scale;
    edge_y = top + 20;
    packet = base + 0x954;
    w_u16(packet + 10, top);
    w_u16(packet + 18, top);
    w_u16(packet + 8, left + 19);
    w_u16(packet + 16, left);
    w_u16(packet + 24, left);
    w_u16(packet + 26, edge_y);
    center_x = r_s16(0x800C6D74);
    packet = base + 0x974;
    w_u16(packet + 10, top);
    w_u16(packet + 18, top);
    w_u16(packet + 26, edge_y);
    right = center_x + scale;
    w_u16(packet + 8, right - 19);
    w_u16(packet + 16, right);
    w_u16(packet + 24, right);
    packet = base + 0x994;
    center_x = r_s16(0x800C6D74);
    center_y = r_s16(0x800C6D76);
    left = center_x - scale;
    bottom = center_y + scale;
    edge_y = bottom - 20;
    w_u16(packet + 8, left + 19);
    w_u16(packet + 10, bottom);
    w_u16(packet + 16, left);
    w_u16(packet + 18, bottom);
    w_u16(packet + 24, left);
    w_u16(packet + 26, edge_y);
    center_x = r_s16(0x800C6D74);
    packet = base + 0x9B4;
    w_u16(packet + 10, bottom);
    w_u16(packet + 18, bottom);
    right = center_x + scale;
    w_u16(packet + 8, right - 19);
    w_u16(packet + 16, right);
    w_u16(packet + 24, right);
    w_u16(packet + 26, edge_y);
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    tag = r_u32(base + 0x9B4);
    head = r_u32(ordering);
    w_u32(base + 0x9B4, (tag & 0xFF000000) | (head & 0xFFFFFF));
    ordering = r_u32(scene + 16);
    head = r_u32(ordering);
    result = (head & 0xFF000000) | ((base + 0x954) & 0xFFFFFF);
    w_u32(ordering, result);
    return result;
}

uint32 sub_80065598(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    MATRIX matrix;
    uint32 scene, camera, angles, angle, sine, cosine, value, flags, root, base, count, packet, i;
    FUNCTION_MARKER(0x80065598, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    sub_800308f0(camera + 20, 0, &matrix, 0);
    for (i = 0; i < 4; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + (i << 2)));
    /* The final rotation control uses only its low halfword */
    xport_gte_write_control(4, xport_load_le16((uint8 *)&matrix + 16));
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    angles = (native_state ? native_state->words[2] : r_u32(state + 8));
    angle = 0 - (uint32)r_s16(angles);
    angle &= 0x3FFF;
    sine = r_u16(0x800104AC + (angle << 1));
    value = (sint16)sine >> 2;
    value = (value << 1) + value;
    if ((sint32)value < 0)
        value += 3;
    w_u16(0x1F800002, (sint32)value >> 2);
    w_u16(0x1F800000, 0);
    w_u16(0x1F800004, 0x800);
    value = r_u32(0x1F800000);
    xport_gte_write_data(0, value);
    value = r_u32(0x1F800004);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x180001);
    value = xport_gte_read_data(14);
    w_u32(0x1F800024, value);
    xport_gte_read_data(8);
    xport_gte_read_control(31);
    xport_gte_read_data(19);
    for (angle = 512; angle < 16385; angle += 512)
    {
        sine = r_u16(0x800104AC + ((angle & 0x3FFF) << 1));
        cosine = r_u16(0x800104AC + (((angle & 0x3FFF) + 0x1000) << 1));
        w_u16(0x1F800000, (sint16)sine >> 3);
        w_u16(0x1F800004, (sint16)cosine >> 3);
        value = r_u32(0x1F800000);
        xport_gte_write_data(0, value);
        value = r_u32(0x1F800004);
        xport_gte_write_data(1, value);
        xport_gte_execute(0x180001);
        value = xport_gte_read_data(14);
        w_u32(0x1F800018, value);
        xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        xport_gte_read_data(19);
        if ((sint32)flags < 0)
        {
            value = r_u32(0x1F800018);
            w_u32(0x1F800024, value);
            continue;
        }
        root = r_u32(0x800BD074);
        base = r_u32(0x800BD078);
        count = r_u32(root + 20);
        value = r_u8(0x800C6D68);
        packet = base + 0x3D8 + (count << 4);
        w_u8(packet + 4, value);
        value = r_u8(0x800C6D69);
        w_u8(packet + 5, value);
        value = r_u8(0x800C6D6A);
        w_u8(packet + 6, value);
        value = r_u32(0x1F800024);
        w_u32(packet + 8, value);
        value = r_u32(0x1F800018);
        w_u32(packet + 12, value);
        root = r_u32(0x800BD074);
        count = r_u32(root + 20);
        w_u32(root + 20, count + 1);
        value = r_u32(0x1F800018);
        w_u32(0x1F800024, value);
        sub_800657b0(state, angle, native_state);
    }
    return 0;
}

uint32 sub_800657b0(uint32 state, uint32 angle, const NATIVE_HUD_CONTEXT *native_state)
{
    uint16 vertices[6][3];
    uint32 screen[6], masked, odd, segment, left_angle, right_angle, angles, a, b, c, d, e, f, pitch, y, top, i, j, start, root, count, base, packet, glyph, value, width, result;
    FUNCTION_MARKER(0x800657B0, "ACE3.BIN");
    masked = angle & 0x3FFF;
    if ((masked & 0x7FF) != 0)
        return 0xFFFFFFFF;
    odd = masked & 0x800;
    segment = masked >> 11;
    if (odd != 0)
    {
        left_angle = (masked - 192) & 0x3FFF;
        right_angle = (masked + 192) & 0x3FFF;
        a = r_u16(0x800104AC + (left_angle << 1));
        b = r_u16(0x800104AC + (masked << 1));
        c = r_u16(0x800104AC + ((left_angle + 0x1000) << 1));
        d = r_u16(0x800104AC + ((masked + 0x1000) << 1));
        angles = (native_state ? native_state->words[2] : r_u32(state + 8));
        e = r_u16(0x800104AC + (right_angle << 1));
        pitch = r_s16(angles);
        f = r_u16(0x800104AC + ((right_angle + 0x1000) << 1));
        vertices[0][0] = (sint16)a >> 3;
        vertices[0][2] = (sint16)c >> 3;
        vertices[1][0] = (sint16)b >> 3;
        vertices[1][2] = (sint16)d >> 3;
        vertices[3][0] = vertices[0][0];
        vertices[3][2] = vertices[0][2];
        vertices[4][0] = vertices[1][0];
        vertices[4][2] = vertices[1][2];
        value = r_u16(0x800104AC + (((0 - pitch) & 0x3FFF) << 1));
        vertices[2][0] = (sint16)e >> 3;
        vertices[2][2] = (sint16)f >> 3;
        vertices[5][0] = vertices[2][0];
        vertices[5][2] = vertices[2][2];
        y = (sint16)value >> 2;
        y = (sint32)((y << 1) + y) >> 2;
        top = y - 144;
        for (i = 0; i < 3; ++i)
        {
            vertices[i][1] = top;
            vertices[i + 3][1] = y;
        }
    }
    else
    {
        left_angle = (masked - 128) & 0x3FFF;
        right_angle = (masked + 128) & 0x3FFF;
        a = r_u16(0x800104AC + (left_angle << 1));
        b = r_u16(0x800104AC + (right_angle << 1));
        c = r_u16(0x800104AC + ((left_angle + 0x1000) << 1));
        angles = (native_state ? native_state->words[2] : r_u32(state + 8));
        d = r_u16(0x800104AC + ((right_angle + 0x1000) << 1));
        pitch = r_s16(angles);
        value = r_u16(0x800104AC + (((0 - pitch) & 0x3FFF) << 1));
        vertices[0][0] = (sint16)a >> 3;
        vertices[0][2] = (sint16)c >> 3;
        vertices[1][0] = (sint16)b >> 3;
        vertices[1][2] = (sint16)d >> 3;
        vertices[2][0] = vertices[0][0];
        vertices[2][2] = vertices[0][2];
        vertices[3][0] = vertices[1][0];
        vertices[3][2] = vertices[1][2];
        y = (sint16)value >> 2;
        y = (sint32)((y << 1) + y) >> 2;
        vertices[2][1] = y;
        vertices[3][1] = y;
        vertices[0][1] = y - 196;
        vertices[1][1] = y - 196;
    }
    /* Vector Z controls ignore the unwritten stack padding halfword */
    for (j = 0; j < 2; ++j)
    {
        start = j * 3;
        if (j == 1 && odd == 0)
        {
            value = vertices[3][0] | ((uint32)vertices[3][1] << 16);
            xport_gte_write_data(0, value);
            xport_gte_write_data(1, vertices[3][2]);
            xport_gte_execute(0x180001);
            screen[3] = xport_gte_read_data(14);
        }
        else
        {
            for (i = 0; i < 3; ++i)
            {
                value = vertices[start + i][0] | ((uint32)vertices[start + i][1] << 16);
                xport_gte_write_data(i << 1, value);
                xport_gte_write_data((i << 1) + 1, vertices[start + i][2]);
            }
            xport_gte_execute(0x280030);
            screen[start] = xport_gte_read_data(12);
            screen[start + 1] = xport_gte_read_data(13);
            screen[start + 2] = xport_gte_read_data(14);
        }
        xport_gte_read_data(8);
        xport_gte_read_control(31);
        xport_gte_read_data(19);
    }
    root = r_u32(0x800BD074);
    count = r_u32(root + 16);
    base = r_u32(0x800BD078);
    packet = base + 0x1F8 + count * 40;
    for (j = 0; j < (uint32)(odd != 0 ? 2 : 1); ++j)
    {
        w_u16(packet + 22, 54);
        w_u16(packet + 14, 0x7F98);
        value = r_u8(0x800C6D6B);
        w_u8(packet + 4, value >> 2);
        value = r_u8(0x800C6D6C);
        w_u8(packet + 5, value >> 2);
        value = r_u8(0x800C6D6D);
        w_u8(packet + 6, value >> 2);
        if (odd != 0)
        {
            w_u32(packet + 8, screen[j]);
            w_u32(packet + 16, screen[j + 1]);
            w_u32(packet + 24, screen[j + 3]);
            w_u32(packet + 32, screen[j + 4]);
        }
        else
        {
            w_u32(packet + 8, screen[0]);
            w_u32(packet + 16, screen[1]);
            w_u32(packet + 24, screen[2]);
            w_u32(packet + 32, screen[3]);
        }
        value = r_u8(0x800B2C38 + (segment << 1) + j);
        glyph = 0x800BCDD4 + (value << 1);
        value = r_u8(glyph);
        w_u8(packet + 13, 40);
        w_u8(packet + 12, value);
        value = r_u8(glyph);
        width = r_u8(glyph + 1);
        w_u8(packet + 21, 40);
        w_u8(packet + 20, value + width);
        value = r_u8(glyph);
        w_u8(packet + 29, 50);
        w_u8(packet + 28, value);
        value = r_u8(glyph);
        width = r_u8(glyph + 1);
        w_u8(packet + 36, value + width);
        w_u8(packet + 37, 50);
        root = r_u32(0x800BD074);
        result = r_u32(root + 16) + 1;
        packet += 40;
        w_u32(root + 16, result);
    }
    return result;
}

uint32 sub_80065d8c(void)
{
    uint16 points[3][2];
    uint32 selected, object, flags, descriptor, x, y, distance, table, scene, camera, vector[3], yaw, angle, trig, size, width, sine, cosine, anchor_x, anchor_y, i, px, py, tx, ty, root, count, base, packet, red, green, blue, value;
    FUNCTION_MARKER(0x80065D8C, "ACE3.BIN");
    selected = r_u8(0x800C6D6E);
    if (selected == 255)
        return 0;
    selected = r_u8(0x800C6D6E);
    object = r_u32(0x800C6ED0 + (selected << 2));
    if (object == 0)
        return 0;
    flags = r_u32(object);
    if ((flags & 1) == 0)
        return 0;
    value = r_s16(object + 24);
    if ((sint32)value < 1024)
    {
        flags = r_u32(object + 32);
        if ((flags & 0x20000) == 0)
            return 0;
    }
    flags = r_u32(object + 32);
    if ((flags & 0x20000) != 0)
    {
        selected = r_u8(0x800C6D6E);
        descriptor = r_u32(0x800C67A0 + (selected << 2));
        if (descriptor == 0)
            return 0;
        x = r_s16(descriptor + 8);
        x -= 160;
        y = r_s16(descriptor + 10);
        y -= 120;
        distance = SquareRoot0PSX(x * x + y * y, 0x800B6DD4);
        if ((sint32)distance < 64)
            return 0;
    }
    selected = r_u8(0x800C6D6E);
    table = r_u32(0x800BCE5C);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    object = r_u32(table + (selected << 2));
    sub_80044f94(camera + 4, object + 8, 0, 0, vector);
    yaw = sub_80030d54(vector[0], vector[1]);
    distance = SquareRoot0PSX(vector[0] * vector[0] + vector[1] * vector[1], 0x800B6DD4);
    angle = sub_80030d54(distance, vector[2]);
    trig = r_u16(0x800104AC + (angle & 0x7FFE));
    size = (sint16)trig >> 9;
    points[0][0] = 0;
    points[0][1] = (sint32)size < 6 ? 6 : size;
    width = 11 - (uint32)((sint32)size >> 1);
    if ((sint32)width < 3)
        width = 3;
    points[1][0] = width;
    points[2][0] = 0 - width;
    sine = r_s16(0x800104AC + ((yaw & 0x3FFF) << 1));
    cosine = r_s16(0x800104AC + (((yaw & 0x3FFF) + 0x1000) << 1));
    points[1][1] = 0;
    points[2][1] = 0;
    anchor_x = (uint32)((sint32)((sine << 1) + sine) >> 10) + 160;
    anchor_y = (uint32)((sint32)((cosine << 1) + cosine) >> 10) + 120;
    for (i = 0; i < 3; ++i)
    {
        px = (sint16)points[i][0];
        py = (sint16)points[i][1];
        tx = cosine * px + sine * py;
        ty = (0 - sine) * px + cosine * py;
        points[i][0] = (uint32)((sint32)tx >> 14) + anchor_x;
        points[i][1] = (uint32)((sint32)ty >> 14) + anchor_y;
    }
    root = r_u32(0x800BD074);
    red = r_u8(0x800C6D68);
    green = r_u8(0x800C6D69);
    blue = r_u8(0x800C6D6A);
    count = r_u32(root + 28);
    base = r_u32(0x800BD078);
    packet = base + 0x878 + count * 40;
    red >>= 2;
    green >>= 2;
    blue >>= 2;
    w_u8(packet + 4, red);
    w_u8(packet + 5, green);
    w_u8(packet + 6, blue);
    value = r_u8(0x800C6D68);
    w_u8(packet + 12, value);
    value = r_u8(0x800C6D69);
    w_u8(packet + 13, value);
    value = r_u8(0x800C6D6A);
    w_u8(packet + 20, red);
    w_u8(packet + 21, green);
    w_u8(packet + 22, blue);
    w_u8(packet + 28, red);
    w_u8(packet + 29, green);
    w_u8(packet + 30, blue);
    w_u8(packet + 14, value);
    value = points[1][0] | ((uint32)points[1][1] << 16);
    w_u32(packet + 8, value);
    x = points[0][0] | ((uint32)points[0][1] << 16);
    w_u32(packet + 16, x);
    y = points[2][0] | ((uint32)points[2][1] << 16);
    w_u32(packet + 32, value);
    w_u32(packet + 24, y);
    root = r_u32(0x800BD074);
    count = r_u32(root + 28);
    w_u32(root + 28, count + 1);
    return 0;
}

uint32 sub_800662b4(void)
{
    MATRIX matrix;
    uint32 table, object, high[3], rotated[3], root, flags, index, history, i, position, speed, target, numerator, divisor, delay, texture, scene, camera, slot, value, base, packet, xy;
    uint16 vector[3];
    FUNCTION_MARKER(0x800662B4, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    rotation_build(object + 24, 0, &matrix, 0);
    for (i = 0; i < 4; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + (i << 2)));
    xport_gte_write_control(4, xport_load_le16((uint8 *)&matrix + 16));
    xport_gte_write_data(9, 0);
    xport_gte_write_data(10, 0);
    xport_gte_write_data(11, 0x1580000 >> 15);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    xport_gte_write_data(9, 0);
    xport_gte_write_data(10, 0);
    xport_gte_write_data(11, 0);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        rotated[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    root = r_u32(0x800BCE60);
    flags = r_u32(root);
    if ((flags & 0x20000000) == 0)
    {
        index = r_u8(0x800BD070);
        w_u8(0x800BD070, index + 1);
    }
    index = r_u8(0x800BD070);
    if (index >= 32)
        w_u8(0x800BD070, 0);
    index = r_u8(0x800BD070);
    history = 0x800C79C8 + (index << 4);
    for (i = 0; i < 3; ++i)
    {
        position = r_u32(object + 8 + (i << 2));
        speed = r_u32(object + 56 + (i << 2));
        w_u32(history + (i << 2), position + rotated[i] + (speed << 5));
    }
    value = r_u8(0x800C6D6E);
    target = r_u32(0x800C6ED0 + (value << 2));
    if (target == 0)
        return 1;
    numerator = r_u32(target + 4);
    divisor = r_u32(0x800C6EC8);
    numerator <<= 5;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    delay = (sint32)numerator / (sint32)divisor;
    if ((sint32)delay >= 32)
        delay = 31;
    texture = (sint32)delay >> 1;
    texture = (texture & 3) + ((texture & 12) << 4);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    w_u8(0x800BD071, (index - delay) & 31);
    sub_800308f0(camera + 20, 0, &matrix, 0);
    slot = r_u8(0x800BD071);
    scene = r_u32(0x800BCE54);
    history = 0x800C79C8 + (slot << 4);
    for (i = 0; i < 3; ++i)
    {
        camera = r_u32(scene + 40);
        position = r_u32(history + (i << 2));
        value = r_u32(camera + 4 + (i << 2));
        vector[i] = (sint32)(position - value) >> 12;
    }
    for (i = 0; i < 4; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + (i << 2)));
    xport_gte_write_control(4, xport_load_le16((uint8 *)&matrix + 16));
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    xy = xport_gte_read_data(14);
    xport_gte_read_data(8);
    flags = xport_gte_read_control(31);
    xport_gte_read_data(19);
    if ((sint32)flags < 0)
        return 0;
    value = r_u32(target) & 0x20;
    if (value == 0)
        return value;
    base = r_u32(0x800BD078);
    value = r_u8(0x800C6D6B);
    packet = base + 0x940;
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D6C);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6D);
    w_u16(packet + 14, texture + 0x7E18);
    w_u8(packet + 6, value);
    w_u16(packet + 8, (xy & 0xFFFF) - 10);
    root = r_u32(0x800BD074);
    w_u16(packet + 10, (xy >> 16) - 10);
    value = r_u32(root + 36) + 1;
    w_u32(root + 36, value);
    return value;
}

uint32 sub_8003545c(uint32 ordering, uint32 packet)
{
    uint32 root, tick, value, color, scene, elapsed;
    FUNCTION_MARKER(0x8003545C, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    tick = r_u32(root + 40);
    value = r_u16(0x800104AC + ((tick << 10) & 0x7C00));
    value = (uint32)((sint16)value >> 3) + 0x800;
    color = (sint32)((value << 3) + value) >> 8;
    packet = sub_800346c0(ordering, packet, 53);
    packet = sub_80034360(16, 28, ordering, packet, 0x800B0AD0, color);
    packet = sub_80034360(45, 28, ordering, packet, 0x800B0AD8, color);
    packet = sub_800346c0(ordering, packet, 53);
    scene = r_u32(0x800BCE54);
    elapsed = r_u32(scene + 12);
    if ((sint32)elapsed >= 91)
    {
        value = (elapsed - 90) << 2;
        if ((sint32)value >= 65)
            value = 64;
        packet = sub_8003563c(ordering, packet, 48, 212, value << 8, 0x80023610, 0);
    }
    return packet;
}

uint32 sub_80035578(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 value, color, head;
    FUNCTION_MARKER(0x80035578, "ACE3.BIN");
    if ((sint32)strength >= 4097)
        strength = 4096;
    if ((sint32)strength < 0)
        strength = 0;
    value = ((strength << 5) - strength) << 2;
    if ((sint32)value < 0)
        value += 0xFFF;
    color = (uint32)((sint32)value >> 13) << 8;
    w_u32(packet + 4, color | 0x62000000);
    w_u16(packet + 10, 216);
    w_u16(packet + 12, 320);
    w_u16(packet + 8, 0);
    w_u16(packet + 14, 2);
    head = r_u32(ordering);
    w_u32(packet, head | 0x03000000);
    w_u32(ordering, packet & 0xFFFFFF);
    if ((sint32)strength < 0)
        strength += 63;
    color = (uint32)((sint32)strength >> 7) << 8;
    return sub_8003563c(ordering, packet + 16, 224, 208, color, 0x8002362C, 0);
}

uint32 sub_80034040(void)
{
    uint32 scene, flags, root, mode, ordering, packet, result;
    FUNCTION_MARKER(0x80034040, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800339e0(sub_800330cc, 0);
    root = r_u32(0x800BCE60);
    mode = r_u8(root + 0x3A5);
    if (mode != 0)
    {
        scene = r_u32(0x800BCE54);
        ordering = r_u32(root + 12);
        packet = r_u32(scene + 20);
        result = sub_80035578(ordering, packet, 4096);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
        flags = r_u32(scene);
        if ((flags & 0x10000000) == 0)
            return sub_80033dc0();
        sub_8003048c();
        resource_init_queue(0, 0);
        root = r_u32(0x800BCE60);
        mode = r_u8(root + 0x3A5);
        if (mode != 1)
        {
            flags = r_u32(root + 0x128);
            if ((flags & 16) == 0)
                return 0x50000000;
        }
        w_u8(root + 0x3A5, 0);
        root = r_u32(0x800BCE60);
        result = r_u32(root + 0x3A0);
        return result | 0x60000000;
    }
    flags = r_u8(root + 0x39E);
    if (flags == 0)
    {
        scene = r_u32(0x800BCE54);
        ordering = r_u32(root + 12);
        packet = r_u32(scene + 20);
        result = sub_8003545c(ordering, packet);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
    }
    return sub_80033dc0();
}

uint32 sub_8008d4e8(uint32 state)
{
    uint32 packet, value, depth, screen_height, shift, numerator, size, bucket_offset, ordering, head, half, result;
    FUNCTION_MARKER(0x8008D4E8, "ACE3.BIN");
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xFFFFFFE2);
    packet = r_u32(state + 4);
    xport_gte_execute(0x180001);
    w_u32(packet + 20, 0x0037203F);
    w_u16(packet + 28, 0x3F20);
    w_u32(packet + 12, 0x46562020);
    w_u16(packet + 36, 0x3F3F);
    w_u32(packet + 4, 0x2E838383);
    value = xport_gte_read_data(14);
    w_u32(state + 16, value);
    depth = xport_gte_read_data(19);
    screen_height = xport_gte_read_control(26);
    shift = r_u32(state + 12);
    depth = (sint32)depth >> (shift & 31);
    numerator = screen_height << 3;
    if (depth - 31 >= 0x77D9)
        return numerator;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)depth;
    if ((sint32)size < 3)
        size = 3;
    bucket_offset = (uint32)((sint32)depth >> 3) << 2;
    ordering = r_u32(state);
    head = r_u32(ordering + bucket_offset);
    w_u32(packet, head | 0x09000000);
    ordering = r_u32(state);
    w_u32(ordering + bucket_offset, packet & 0xFFFFFF);
    w_u32(state + 4, packet + 40);
    value = r_u16(state + 16);
    half = (sint32)size >> 1;
    w_u16(state + 16, value - half);
    value = r_u16(state + 18);
    w_u16(state + 18, value - half);
    value = r_u16(state + 16);
    w_u16(packet + 8, value);
    value = r_u16(state + 18);
    w_u16(packet + 10, value);
    value = r_u16(state + 16);
    w_u16(packet + 16, value + size);
    value = r_u16(state + 18);
    w_u16(packet + 18, value);
    value = r_u16(state + 16);
    w_u16(packet + 24, value);
    value = r_u16(state + 18);
    w_u16(packet + 26, value + size);
    value = r_u16(state + 16);
    w_u16(packet + 32, value + size);
    value = r_u16(state + 18);
    result = value + size;
    w_u16(packet + 34, result);
    return result;
}

uint32 sub_8008de18(uint32 packet, uint32 state)
{
    uint32 x1, x0, y1, y0, value, scale, endpoint, output, pair, corner, x, y, vx, vy, result;
    SVECTOR *normal;
    FUNCTION_MARKER(0x8008DE18, "ACE3.BIN");
    x1 = r_u16(state + 12);
    x0 = r_u16(state + 8);
    y1 = r_u16(state + 14);
    y0 = r_u16(state + 10);
    w_u16(state, x1 - x0);
    w_u16(state + 2, y1 - y0);
    value = r_u32(state);
    w_u16(state + 4, 0);
    if (value != 0)
    {
        normal = (SVECTOR *)psx_addr(state, 8);
        VectorNormalSS(normal, normal);
    }
    else
        w_u16(state, 0x1000);
    for (pair = 0; pair < 2; ++pair)
    {
        scale = r_u32(state + 16 + (pair << 2));
        endpoint = state + 8 + (pair << 2);
        output = packet + 8 + (pair << 4);
        if (scale == 0)
        {
            result = r_u32(endpoint);
            w_u32(output + 8, result);
            w_u32(output, result);
            continue;
        }
        xport_gte_write_data(8, scale);
        for (corner = 0; corner < 2; ++corner)
        {
            value = r_u16(endpoint);
            xport_gte_write_data(25, value);
            value = r_u16(endpoint + 2);
            xport_gte_write_data(26, value);
            x = r_s16(state);
            y = r_s16(state + 2);
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
            w_u16(output + (corner << 3), value);
            result = xport_gte_read_data(26);
            w_u16(output + (corner << 3) + 2, result);
        }
    }
    return result;
}

void sub_80090210(uint32 strength, uint32 kind, uint32 context)
{
    uint32 a, b, c, table, object, matrix, saved, scene, red, green, blue, scale, previous17, previous18;
    FUNCTION_MARKER(0x80090210, "ACE3.BIN");
    if ((sint32)strength < 0 || kind >= 6)
        return;
    saved = context + 0x50;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(saved, a);
    w_u32(saved + 4, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(saved + 8, a);
    w_u32(saved + 12, b);
    w_u32(saved + 16, c);
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    w_u32(saved + 20, a);
    w_u32(saved + 24, b);
    w_u32(saved + 28, c);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    matrix = context + 0x70;
    rotation_build(object + 24, matrix, 0, 0);
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
    xport_gte_write_data(0, 0xFF9C0000);
    xport_gte_write_data(1, 0xF38);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    scene = r_u32(0x800BCE54);
    w_u16(scene + 120, a);
    w_u16(scene + 122, b);
    w_u16(scene + 124, c);
    a = r_u32(saved);
    b = r_u32(saved + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(saved + 8);
    b = r_u32(saved + 12);
    c = r_u32(saved + 16);
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

uint32 sub_8008fc9c(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 value, root, page, first, current, tail, i, left, right, top, bottom, result, address, lane, shift;
    static const uint16 uv[16] = {0x0203, 0x03A1, 0x7905, 0x79A1, 0x7705, 0x77A1, 0xEE03, 0xEDA1, 0x031F, 0x02BD, 0x791F, 0x79BB, 0x771F, 0x77BB, 0xED1F, 0xEEBD};
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
    root = r_u32(0x800BCE60);
    page = r_u32(root + 4);
    page = (page == 0) << 8;
    xport_gte_execute(0x780010);
    first = packet + 28;
    w_u32(packet, ((packet + 12) & 0xFFFFFF) | 0x02000000);
    w_u32(packet + 12, (first & 0xFFFFFF) | 0x03000000);
    w_u32(packet + 4, 0xE6000001);
    w_u32(packet + 8, 0xE1000340);
    w_u32(packet + 20, 0);
    w_u32(packet + 24, 0x00F00140);
    if (strength < 4096)
        value = xport_gte_read_data(22);
    else
        value = 0x60000000;
    w_u32(packet + 16, value);
    tail = first;
    if ((strength & 0xFFF) != 0)
    {
        value = 0x1000 - rsin((sint32)strength >> 1);
        xport_gte_write_data(8, value);
        xport_gte_write_data(6, 0x2E7C7F7D);
        tail = first + 160;
        xport_gte_execute(0x780010);
        current = first;
        for (i = 0; i < 4; ++i, current += 40)
        {
            w_u32(current, ((current + 40) & 0xFFFFFF) | 0x09000000);
            w_u16(current + 22, (page >> 4) | (i & 2) | 0x120);
            left = (i & 2) != 0 ? 160 : 0;
            right = left + 160;
            w_u16(current + 24, left);
            w_u16(current + 8, left);
            w_u16(current + 32, right);
            w_u16(current + 16, right);
            top = (i & 1) != 0 ? 120 : 0;
            bottom = top + 120;
            w_u16(current + 18, top);
            w_u16(current + 10, top);
            w_u16(current + 34, bottom);
            w_u16(current + 26, bottom);
        }
        value = xport_gte_read_data(22);
        w_u32(first + 124, value);
        w_u32(first + 84, value);
        w_u32(first + 44, value);
        w_u32(first + 4, value);
        for (i = 0; i < 16; ++i)
            w_u16(first + (i >> 2) * 40 + 12 + (i & 3) * 8, uv[i]);
    }
    result = tail + 12;
    w_u32(tail, (result & 0xFFFFFF) | 0x02000000);
    w_u32(tail + 4, 0xE6000000);
    w_u32(tail + 8, 0xE1000220);
    /* The LWL seed is 02000000 and contributes no retained low bits */
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
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
    return result;
}

uint32 sub_8008c644(uint32 state, uint32 effect)
{
    uint32 first, packet, random, tick, phase, value, a, step, mask, root, page, distance, i, divisor, sample, remainder, next, packed, x, magnitude, bucket, start, address, lane, shift, result;
    FUNCTION_MARKER(0x8008C644, "ACE3.BIN");
    first = r_u32(state + 4);
    random = r_u32(0x800BCF90);
    packet = first + 12;
    w_u32(first, (packet & 0xFFFFFF) | 0x02000000);
    w_u32(first + 4, 0xE6000001);
    w_u32(first + 8, 0xE1000217);
    tick = r_u32(state + 0x3FC);
    value = r_u32(effect + 4);
    phase = ((uint32)((sint32)tick >> 3) - (value << 1)) & 255;
    value = r_s16(effect + 28);
    xport_gte_write_control(21, value);
    value = r_s16(effect + 30);
    xport_gte_write_control(22, value);
    value = r_s16(effect + 32);
    xport_gte_write_control(23, value);
    step = r_s16(effect + 78);
    value = r_u16(random + (phase << 1));
    mask = r_u32(effect + 16);
    root = r_u32(0x800BCE60);
    distance = r_s16(effect + 76);
    page = r_u32(root + 4);
    distance -= value & 127;
    page = (page != 0) << 8;
    for (i = 0; i < 16 && mask != 0; ++i, mask >>= 2)
    {
        distance += step;
        step -= (uint32)((sint32)step >> 6);
        phase += 2;
        if ((mask & 1) == 0)
            continue;
        xport_gte_write_data(8, distance);
        value = r_s16(effect + 20);
        xport_gte_write_data(9, value);
        value = r_s16(effect + 22);
        xport_gte_write_data(10, value);
        value = r_s16(effect + 24);
        xport_gte_write_data(11, value);
        xport_gte_execute(0x980011);
        divisor = r_s16(effect + 72);
        if (divisor != 0)
        {
            sample = r_s16(random + ((phase & 255) << 1));
            remainder = (sint32)sample % (sint32)divisor;
            value = (sint32)(remainder * (distance + 512)) >> 11;
            w_u16(effect + 84, value);
        }
        else
            w_u16(effect + 84, 0);
        divisor = r_s16(effect + 74);
        if (divisor != 0)
        {
            sample = r_s16(random + (((phase + 1) & 255) << 1));
            remainder = (sint32)sample % (sint32)divisor;
            value = remainder + (uint32)((sint32)(remainder * distance) >> 12);
            w_u16(effect + 86, value);
        }
        else
            w_u16(effect + 86, 0);
        a = xport_gte_read_data(25);
        value = r_u16(effect + 84);
        w_u16(effect + 84, value + a);
        a = xport_gte_read_data(26);
        value = r_u16(effect + 86);
        w_u16(effect + 86, value + a);
        value = r_u32(effect + 84);
        xport_gte_write_data(0, value);
        value = xport_gte_read_data(27);
        xport_gte_write_data(1, value);
        next = packet + 40;
        w_u32(packet, (next & 0xFFFFFF) | 0x09000000);
        w_u32(packet + 4, 0x01000000);
        w_u32(packet + 8, 0x80000000);
        xport_gte_execute(0x180001);
        w_u32(packet + 20, 0x00010010);
        w_u32(packet + 24, 0x67000000);
        w_u32(packet + 16, 0x012B0160);
        w_u16(packet + 34, 0x4AD6);
        w_u32(packet + 36, 0x00050010);
        value = xport_gte_read_data(14);
        w_u32(effect + 92, value);
        magnitude = r_s16(effect + 92);
        x = r_u16(effect + 92);
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)magnitude >= 153)
            continue;
        magnitude = r_s16(effect + 94);
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)magnitude >= 119)
            continue;
        w_u16(effect + 92, x - 8);
        w_u16(packet + 12, (x + 152) & 1023);
        value = r_u16(effect + 94);
        w_u16(packet + 14, (value + page + 120) & 511);
        value = r_u16(effect + 94);
        w_u16(effect + 94, value - 2);
        value = (sint32)phase >> 1;
        packed = r_u32(effect + 92);
        w_u8(packet + 32, 0);
        w_u8(packet + 33, value & 15);
        w_u32(packet + 28, packed);
        packet = next;
    }
    result = first + 12;
    if (packet == result)
        return result;
    w_u32(packet, 0x02000000);
    w_u32(packet + 4, 0xE6000000);
    w_u32(packet + 8, 0xE1000220);
    value = r_u32(effect + 68);
    value -= 2;
    if ((sint32)value < -19)
        value = 0xFFFFFFED;
    bucket = r_u32(state + 0x3F8);
    bucket += value << 2;
    start = r_u32(state + 4);
    address = bucket + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    value = r_u32(address & 0xFFFFFFFC);
    value = (value << shift) | (0x00050010 & (0x00FFFFFF >> (lane << 3)));
    address = packet + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    value = start << 8;
    address = bucket + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    result = packet + 12;
    w_u32(state + 4, result);
    return result;
}

uint32 sub_8009044c(uint32 effect)
{
    uint32 kind, strength, scene, camera, mode, root, packet, tick, random, value, a, b, c, d, target, head, color, result, size_x, size_y, i, start, next, center, address, lane, shift;
    const uint32 scratch = 0x1F80001C;
    FUNCTION_MARKER(0x8009044C, "ACE3.BIN");
    kind = r_s16(effect + 4);
    strength = r_u32(effect);
    scene = r_u32(0x800BCE54);
    strength -= 2;
    w_u32(scratch, kind);
    w_u32(scratch + 4, strength);
    camera = r_u32(scene + 40);
    mode = r_u32(camera + 80);
    if (mode != 1)
        sub_80090210(strength, kind, scratch);
    result = (sint32)kind < 5;
    if (result == 0 || (sint32)strength < 0)
        return result;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    mode = r_u32(camera + 80);
    if (mode != 1)
        return camera;
    result = r_u32(camera) & 0x200;
    if (result != 0)
        return result;
    root = r_u32(0x800BCE60);
    packet = r_u32(0x1F800004);
    tick = r_u32(root + 40);
    random = r_u32(0x800BCF90);
    value = r_u32(random + ((tick & 255) << 2));
    strength = r_u32(scratch + 4);
    w_u32(scratch + 8, tick);
    w_u32(scratch + 12, value);
    if (strength == 0)
    {
        kind = r_u32(scratch);
        if (kind != 3)
        {
            result = (sint32)kind < 5;
            if (result == 0)
            {
                w_u32(0x1F800004, packet);
                return result;
            }
            root = r_u32(0x1F800000);
            head = r_u32(root + 8);
            w_u32(packet, head | 0x09000000);
            root = r_u32(0x1F800000);
            w_u32(root + 8, packet & 0xFFFFFF);
            value = r_u16(effect + 8);
            kind = r_u32(scratch);
            w_u16(scratch + 24, value);
            value = r_u16(effect + 10);
            w_u16(scratch + 26, value);
            value = r_u16(effect + 12);
            w_u16(scratch + 28, value);
            value = r_u16(effect + 14);
            target = packet;
            packet += 40;
            w_u16(scratch + 30, value);
            switch (kind)
            {
                case 0:
                    w_u32(scratch + 32, 1);
                    w_u32(scratch + 36, 3);
                    w_u32(target + 20, 0x003860AF);
                    w_u16(target + 28, 0x9FA0);
                    w_u32(target + 4, 0x2E838383);
                    w_u32(target + 12, 0x445660A0);
                    w_u16(target + 36, 0x9FAF);
                    break;
                case 1:
                    w_u32(scratch + 32, 2);
                    w_u32(scratch + 36, 4);
                    w_u16(target + 28, 0x9FB0);
                    w_u32(target + 4, 0x2E838383);
                    w_u32(target + 12, 0x449660B0);
                    w_u32(target + 20, 0x003860BF);
                    w_u16(target + 36, 0x9FBF);
                    break;
                case 2:
                    w_u32(scratch + 36, 4);
                    w_u32(scratch + 32, 3);
                    w_u16(target + 28, 0x9FC0);
                    w_u32(target + 4, 0x2E838383);
                    w_u32(target + 12, 0x44D660C0);
                    w_u32(target + 20, 0x003860CF);
                    w_u16(target + 36, 0x9FCF);
                    break;
                case 4:
                    w_u32(scratch + 32, 6);
                    w_u32(scratch + 36, 7);
                    w_u16(target + 28, 0x9FD0);
                    w_u32(target + 4, 0x2E838383);
                    w_u32(target + 12, 0x455660D0);
                    w_u32(target + 20, 0x003860DF);
                    w_u16(target + 36, 0x9FDF);
                    break;
            }
            if (kind == 0 || kind == 1)
            {
                a = r_u16(scratch + 12);
                b = r_u16(scratch + 24);
                c = r_u32(scratch + 12);
                w_u16(scratch + 24, b + (a & 1));
                value = r_u16(scratch + 26);
                w_u16(scratch + 26, value + (((sint32)c >> 1) & 1));
            }
            sub_8008de18(target, scratch + 16);
        }
    }
    kind = r_u32(scratch);
    result = (sint32)kind < 5;
    if (result == 0)
    {
        w_u32(0x1F800004, packet);
        return result;
    }
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    if ((sint32)kind < 3)
    {
        value = r_u32(scratch + 12) & 0x7FF;
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
        w_u32(scratch + 32, a);
        w_u32(scratch + 36, b);
        target = packet;
        packet += 40;
        xport_gte_execute(0x780010);
        root = r_u32(0x1F800000);
        head = r_u32(root + 8);
        w_u32(target, head | 0x09000000);
        root = r_u32(0x1F800000);
        w_u32(root + 8, target & 0xFFFFFF);
        tick = r_u32(scratch + 8);
        value = (uint32)((sint32)tick % 5) << 5;
        w_u32(target + 12, value | 0x40566000);
        w_u32(target + 20, (value + 31) | 0x00386000);
        w_u16(target + 28, value | 0xFFFF9F00);
        w_u16(target + 36, (value + 31) | 0xFFFF9F00);
        a = r_u32(effect + 16);
        b = r_u32(effect + 20);
        w_u32(scratch + 24, a);
        w_u32(scratch + 28, b);
        value = xport_gte_read_data(22);
        w_u32(target + 4, value);
        sub_8008de18(scratch + 40, scratch + 16);
        result = r_u32(scratch + 8) & 1;
        if (result != 0)
        {
            a = r_u32(scratch + 56);
            b = r_u32(scratch + 48);
            c = r_u32(scratch + 72);
            d = r_u32(scratch + 64);
        }
        else
        {
            a = r_u32(scratch + 48);
            b = r_u32(scratch + 56);
            c = r_u32(scratch + 64);
            d = r_u32(scratch + 72);
        }
        w_u32(target + 8, a);
        w_u32(target + 16, b);
        w_u32(target + 24, c);
        w_u32(target + 32, d);
        w_u32(0x1F800004, packet);
        return result;
    }
    strength = r_u32(scratch + 4);
    value = r_u32(scratch + 12);
    xport_gte_write_data(8, (strength << 10) + (value & 0x3FF));
    color = kind == 3 ? 0x2E231333 : 0x2E332313;
    xport_gte_write_data(6, color);
    start = packet;
    center = r_u32(effect + 24);
    xport_gte_execute(0x780010);
    value = r_u32(scratch + 12);
    value = ((sint32)value >> 7) & 7;
    size_x = 100 - value;
    size_y = 90 - value;
    w_u32(scratch + 48, center);
    value = r_u16(scratch + 48);
    a = value;
    w_u16(scratch + 56, value - size_x);
    value = r_u16(scratch + 50);
    w_u16(scratch + 64, a + size_x);
    w_u16(scratch + 58, value - size_y);
    w_u16(scratch + 66, value + size_y);
    color = xport_gte_read_data(22);
    for (i = 0; i < 4; ++i, packet += 40)
    {
        w_u32(packet, ((packet + 40) & 0xFFFFFF) | 0x09000000);
        w_u32(packet + 4, color);
        w_u32(packet + 12, 0x459680E0);
        w_u32(packet + 20, 0x003880FF);
        w_u16(packet + 28, 0x9FE0);
        w_u16(packet + 36, 0x9FFF);
        w_u32(packet + 8, center);
        value = r_u16(scratch + 48);
        w_u16(packet + 24, value);
        value = r_u16(scratch + 50);
        w_u16(packet + 18, value);
        value = r_u16(scratch + ((i & 1) != 0 ? 64 : 56));
        w_u16(packet + 32, value);
        w_u16(packet + 16, value);
        value = r_u16(scratch + ((i & 2) != 0 ? 66 : 58));
        w_u16(packet + 34, value);
        w_u16(packet + 26, value);
    }
    root = r_u32(0x1F800000);
    target = root + 8;
    next = packet - 40;
    address = target + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    value = r_u32(address & 0xFFFFFFFC);
    value = (value << shift) | (0x003880FF & (0x00FFFFFF >> (lane << 3)));
    result = start << 8;
    address = next + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    address = target + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, result >> (shift + (i << 3)));
    w_u32(0x1F800004, packet);
    return result;
}

uint32 sub_8008d9f4(void)
{
    uint32 packet, value, scene, data, height_offset, i, params, word, count, color, opcode, high_opcode, layer, px, pz, x_shift, z_shift, height, brightness, random, remaining, position, zword, delta, xyword, flags, xy, magnitude, texture, u, right, far_u, depth, length, ordering, bucket, head;
    const uint32 scratch = 0x1F80001C;
    FUNCTION_MARKER(0x8008D9F4, "ACE3.BIN");
    packet = r_u32(0x1F800004);
    value = r_u32(0x1F80000C);
    scene = r_u32(0x800BCE54);
    height_offset = 0 - (uint32)((sint32)value >> 12);
    data = r_u32(scene + 0x268) + 8;
    for (i = 0; i < 2; ++i)
    {
        word = r_u32(data + (i << 2));
        count = (word >> 16) & 255;
        value = r_s16(data + (i << 2));
        params = scratch + (i << 2);
        w_u32(params + 0x38, (sint32)(height_offset + value) >> (i + 1));
        if (count >= 129)
            count = 128;
        w_u32(params + 0x40, 0 - (word >> 25));
        w_u32(params + 0x30, count);
    }
    word = r_u32(data + 12);
    w_u32(scratch + 0x2C, (word & 0xFFFFFF) | 0x2E000000);
    opcode = (sint32)word >> 24;
    opcode = opcode != 0 ? 89 : 57;
    high_opcode = opcode << 16;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    for (layer = 0; layer < 2; ++layer)
    {
        params = scratch + (layer << 2);
        px = r_u32(0x1F800008);
        pz = r_u32(0x1F800010);
        x_shift = (sint32)px >> 13;
        z_shift = (sint32)pz >> 13;
        if (layer == 0)
        {
            x_shift += (uint32)((sint32)px >> 15);
            z_shift += (uint32)((sint32)pz >> 15);
        }
        height = r_u32(params + 0x38);
        color = r_u32(scratch + 0x2C);
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
        w_u32(scratch + 0x28, color);
        remaining = r_u32(params + 0x30);
        random = r_u32(0x800BCF90);
        while ((sint32)remaining > 0)
        {
            do
            {
                word = r_u32(random);
                w_u16(scratch + 2, height);
                position = (sint32)word >> 16;
                w_u16(scratch, ((word - x_shift) & 0x7FFF) - 0x4000);
                w_u16(scratch + 4, ((position - z_shift) & 0x7FFF) - 0x4000);
                value = r_u32(scratch);
                xport_gte_write_data(0, value);
                value = r_u32(scratch + 4);
                xport_gte_write_data(1, value);
                delta = ((remaining & 7) << 4) + 0x26A;
                xport_gte_execute(0x180001);
                zword = r_u32(scratch + 4);
                if ((remaining & 1) != 0)
                    delta = 0 - delta;
                delta += zword;
                w_u32(scratch + 0x1C, delta);
                w_u32(scratch + 0x14, delta);
                xyword = r_u32(scratch);
                w_u32(scratch + 0x0C, zword);
                delta = xyword + ((remaining & 15) << 4) + 0x555;
                w_u32(scratch + 0x10, xyword);
                w_u32(scratch + 0x18, delta);
                w_u32(scratch + 8, delta);
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
                for (i = 0; i < 6; ++i)
                {
                    value = r_u32(scratch + 8 + (i << 2));
                    xport_gte_write_data(i, value);
                }
                w_u32(packet + 8, xy);
                xport_gte_execute(0x280030);
                texture = (layer << 6) + 0x4DD6;
                if ((sint32)height > 0)
                    texture += 128;
                u = (remaining & 3) << 5;
                right = u + 31;
                far_u = (remaining & 3) << 6;
                w_u32(packet + 12, (texture << 16) | u | 0x2000);
                w_u32(packet + 20, high_opcode | right | 0x2000);
                w_u16(packet + 28, u | 0x2F00);
                w_u16(packet + 36, right | 0x2F00);
                w_u32(scratch + 0x64, (texture << 16) | far_u);
                w_u32(scratch + 0x6C, high_opcode | (far_u + 63));
                w_u16(scratch + 0x74, far_u | 0x1F00);
                w_u16(scratch + 0x7C, (far_u + 63) | 0x1F00);
                flags = xport_gte_read_control(31);
                if ((sint32)flags < 0)
                    break;
                xport_gte_execute(0x168002E);
                length = 0x09000000;
                if (opcode == 89)
                {
                    length = 0x0A000000;
                    w_u32(packet + 40, 0xE1000220);
                }
                depth = xport_gte_read_data(7);
                if ((sint32)depth >= 557)
                {
                    value = r_u32(scratch + 0x28);
                    w_u32(packet + 4, value);
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
                    w_u32(packet + 4, value);
                }
                ordering = r_u32(0x1F800000);
                bucket = ordering + (depth << 2);
                head = r_u32(bucket);
                w_u32(packet, head | length);
                ordering = r_u32(0x1F800000);
                w_u32(ordering + (depth << 2), packet & 0xFFFFFF);
                if ((sint32)depth < 1536)
                {
                    value = r_u32(scratch + 0x64);
                    word = r_u32(scratch + 0x6C);
                    w_u32(packet + 12, value);
                    w_u32(packet + 20, word);
                    value = r_u32(scratch + 0x74);
                    word = r_u32(scratch + 0x7C);
                    w_u32(packet + 28, value);
                    w_u32(packet + 36, word);
                }
                value = xport_gte_read_data(12);
                w_u32(packet + 16, value);
                value = xport_gte_read_data(13);
                w_u32(packet + 24, value);
                value = xport_gte_read_data(14);
                w_u32(packet + 32, value);
                packet += 40;
                if (opcode == 89)
                    packet += 4;
            } while (0);
            --remaining;
            value = r_u32(params + 0x40);
            random += 4;
            height += value;
        }
    }
    w_u32(0x1F800004, packet);
    return 0;
}

uint32 sub_800319c8(uint32 origin, uint32 target, uint32 output, uint16 *native_output, const uint32 *native_target)
{
    MATRIX matrix;
    uint16 angles[3];
    uint32 a, b, c, d, e, x, y, z, sum, distance, horizontal, shift, value, angle, input[3], high[3], low[3], mac[3], i, magnitude;
    FUNCTION_MARKER(0x800319C8, "ACE3.BIN");
    angles[0] = r_u16(origin + 16);
    angles[1] = r_u16(origin + 18);
    angles[2] = 0;
    sub_800308f0(0, 0, &matrix, angles);
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
    a = r_u32(origin);
    b = native_target != 0 ? native_target[0] : r_u32(target);
    c = r_u32(origin + 4);
    x = (sint32)(a - b) >> 12;
    b = native_target != 0 ? native_target[1] : r_u32(target + 4);
    d = native_target != 0 ? native_target[2] : r_u32(target + 8);
    a = r_u32(origin + 8);
    y = (sint32)(c - b) >> 12;
    z = (sint32)(a - d) >> 12;
    sum = ((sint32)x < 0 ? 0 - x : x) + ((sint32)y < 0 ? 0 - y : y) + ((sint32)z < 0 ? 0 - z : z);
    distance = 0x100000;
    if ((sint32)sum <= 0xAFFF)
        distance = sub_80083f40(x, y, z);
    input[0] = x;
    input[1] = y;
    input[2] = z;
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        high[i] = (sint32)magnitude >> 15;
        low[i] = magnitude & 0x7FFF;
        if ((sint32)value < 0)
        {
            high[i] = 0 - high[i];
            low[i] = 0 - low[i];
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        mac[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
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
        horizontal = sub_80083f40((sint32)x >> shift, 0, (sint32)z >> shift) << shift;
    }
    value = sub_80030d54(y, horizontal);
    if (native_output != 0)
        native_output[0] = value;
    else
        w_u16(output, value);
    value = 0 - sub_80030d54(x, 0 - z);
    if (native_output != 0)
        native_output[1] = value;
    else
        w_u16(output + 2, value);
    value = 0 - sub_80030d54(x, y);
    angle = native_output != 0 ? (uint32)(sint32)(sint16)native_output[1] : (uint32)r_s16(output + 2);
    if ((sint32)angle < 0)
        angle = 0 - angle;
    if (native_output != 0)
        native_output[2] = value;
    else
        w_u16(output + 4, value);
    if ((sint32)angle >= 0x1001)
        distance = 0 - distance;
    return distance;
}

uint32 sub_800799d4(uint32 state, uint32 distance, uint32 scale, uint32 output, uint32 *native_output, const uint32 *native_context)
{
    MATRIX matrix;
    uint32 object, a, b, c, d, e, x, y, z, base_x, base_y, base_z;
    FUNCTION_MARKER(0x800799D4, "ACE3.BIN");
    object = (native_context != 0 ? native_context[0] : r_u32(state));
    rotation_build(object + 16, 0, &matrix, 0);
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
    distance = (sint32)distance >> 5;
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, distance & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x190003D);
    base_x = r_u32(object);
    base_y = r_u32(object + 4);
    base_z = r_u32(object + 8);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    x += base_x;
    y += base_y;
    z += base_z;
    if (native_output != 0)
    {
        native_output[0] = x;
        native_output[1] = y;
        native_output[2] = z;
    }
    else
    {
        w_u32(output, x);
        w_u32(output + 4, y);
        w_u32(output + 8, z);
    }
    return x;
}

uint32 sub_80079b1c(uint32 position, uint32 normal, uint32 surface, uint16 *native_normal, uint32 *native_surface)
{
    uint32 x, z, gx, gz, scene, map, tile, first[3], second[3], hit[3], result, source, value, i;
    FUNCTION_MARKER(0x80079B1C, "ACE3.BIN");
    x = r_u32(position);
    z = r_u32(position + 8);
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    scene = r_u32(0x800BCE54);
    map = r_u32(scene + 0x248);
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint32)gx >= 256)
        gx = (gx & 7) + 248;
    if ((sint32)gz >= 256)
        gz = (gz & 7) + 248;
    tile = r_u16(map + ((gx + (gz << 8)) << 1)) & 0x1FF;
    if (tile != 0)
    {
        first[0] = x;
        first[1] = 0xFF448000;
        first[2] = r_u32(position + 8);
        second[0] = r_u32(position);
        second[1] = 0;
        second[2] = r_u32(position + 8);
        result = sub_8009a5d0(0, 0, 0, 0x1F800000, first, second, hit);
        if ((sint32)result <= 0x7FFFFFFE)
        {
            if (normal != 0 || native_normal != 0)
            {
                source = r_u32(0x800BD0C0);
                for (i = 0; i < 3; ++i)
                {
                    value = r_u16(source + (i << 1));
                    if (native_normal != 0)
                        native_normal[i] = value;
                    else
                        w_u16(normal + (i << 1), value);
                }
            }
            if (surface != 0 || native_surface != 0)
            {
                value = r_u32(0x800BD0C4);
                if (native_surface != 0)
                    *native_surface = value;
                else
                    w_u32(surface, value);
            }
            return hit[1];
        }
    }
    if (normal != 0 || native_normal != 0)
    {
        if (native_normal != 0)
        {
            native_normal[0] = 0;
            native_normal[1] = 0xF000;
            native_normal[2] = 0;
        }
        else
        {
            w_u16(normal, 0);
            w_u16(normal + 2, 0xF000);
            w_u16(normal + 4, 0);
        }
    }
    if (tile != 0 && (surface != 0 || native_surface != 0))
    {
        if (native_surface != 0)
            *native_surface = 0;
        else
            w_u32(surface, 0);
    }
    return 0;
}

uint32 sub_80079cb0(uint32 position)
{
    uint16 normal[3];
    uint32 surface, result, altitude;
    FUNCTION_MARKER(0x80079CB0, "ACE3.BIN");
    result = sub_80079b1c(position, 0, 0, normal, &surface);
    altitude = r_u32(position + 4);
    result = result + 0xFFF7C000 - altitude;
    return (sint32)result < 0 ? result : 0;
}

uint32 sub_8007ae40(uint32 source, uint32 destination)
{
    uint32 current[3], target[3], step[3], value, magnitude, i;
    FUNCTION_MARKER(0x8007AE40, "ACE3.BIN");
    current[0] = r_s16(destination + 16);
    current[1] = r_s16(destination + 18);
    target[1] = r_s16(source + 18);
    current[2] = r_s16(destination + 20);
    target[0] = r_s16(source + 16);
    target[2] = r_s16(source + 20);
    for (i = 1; i < 3; ++i)
    {
        value = current[i] - target[i];
        if ((sint32)value < -8192)
        {
            current[i] += 0x4000;
            value = current[i] - target[i];
        }
        if ((sint32)value >= 8193)
            current[i] -= 0x4000;
    }
    for (i = 0; i < 3; ++i)
    {
        value = current[i] - target[i];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        step[i] = (sint32)(magnitude + 1) >> (i == 1 ? 3 : 4);
    }
    for (i = 0; i < 3; ++i)
    {
        if ((sint32)current[i] < (sint32)target[i])
            current[i] += step[i];
        if ((sint32)target[i] < (sint32)current[i])
            current[i] -= step[i];
    }
    for (i = 0; i < 3; ++i)
    {
        value = current[i] - target[i];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)step[i])
            current[i] = step[i];
    }
    w_u16(destination + 16, current[0]);
    w_u16(destination + 18, current[1]);
    w_u16(destination + 20, current[2]);
    value = r_u32(source);
    w_u32(destination, value);
    value = r_u32(source + 4);
    w_u32(destination + 4, value);
    value = r_u32(source + 8);
    w_u32(destination + 8, value);
    return value;
}

uint32 sub_80079dac(uint32 state, uint32 offset, uint32 output, const uint32 *native_offset, const uint32 *native_context)
{
    MATRIX matrix;
    uint32 direction[3], a, b, c, d, e, first, second, base, x, y, z, ax, ay, az, i;
    uint16 vector[3];
    FUNCTION_MARKER(0x80079DAC, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        second = (native_context != 0 ? native_context[1] : r_u32(state + 4));
        first = (native_context != 0 ? native_context[0] : r_u32(state));
        a = r_u32(second + (i << 2));
        b = r_u32(first + (i << 2));
        direction[i] = a - b;
    }
    sub_80031cd8(0, output + 16, direction);
    rotation_build(output + 16, 0, &matrix, 0);
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
    x = native_offset != 0 ? native_offset[0] : r_u32(offset);
    y = native_offset != 0 ? native_offset[1] : r_u32(offset + 4);
    z = native_offset != 0 ? native_offset[2] : r_u32(offset + 8);
    base = (native_context != 0 ? native_context[3] : r_u32(state + 12));
    ax = r_s16(base);
    ay = r_s16(base + 2);
    az = r_s16(base + 4);
    vector[0] = ax + (uint32)((sint32)x >> 6);
    vector[1] = ay + (uint32)((sint32)y >> 6);
    vector[2] = az + (uint32)((sint32)z >> 6);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    base = (native_context != 0 ? native_context[0] : r_u32(state));
    ax = r_u32(base);
    ay = r_u32(base + 4);
    az = r_u32(base + 8);
    x = (x << 6) + ax;
    y = (y << 6) + ay;
    z = (z << 6) + az;
    w_u32(output, x);
    w_u32(output + 4, y);
    w_u32(output + 8, z);
    return z;
}

uint32 sub_8007aab4(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, root, value, first, second, angle, sine, offset[3], flags, count, limit;
    FUNCTION_MARKER(0x8007AAB4, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        root = r_u32(0x800BCE60);
        w_u32(control + 28, 0);
        w_u32(control, 1);
        value = r_u32(root + 36);
        w_u32(control + 32, duration);
        w_u32(control + 40, 0);
        w_u32(control + 36, value & 15);
    }
    else if (mode != 1)
        return 0;
    first = (native_context != 0 ? native_context[0] : r_u32(state));
    second = (native_context != 0 ? native_context[1] : r_u32(state + 4));
    sub_80083dfc(first, second, 0, 0);
    value = r_u32(control + 40);
    if (value != 0)
        w_u32(control + 40, value - 1);
    first = (native_context != 0 ? native_context[0] : r_u32(state));
    angle = r_u16(first + 20);
    sine = r_s16(0x800104AC + ((angle & 0x3FFF) << 1));
    angle = r_u16(first + 16);
    offset[2] = 0xFFFB0000;
    offset[0] = ((sine << 2) + sine) << 1;
    sine = r_s16(0x800104AC + ((angle & 0x3FFF) << 1));
    offset[1] = (uint32)((sint32)((sine << 4) - sine) >> 1) + 0xFFFEC000;
    sub_80079dac(state, 0, control + 4, offset, native_context);
    flags = r_u32(control + 36);
    if ((flags & 8) != 0)
    {
        first = (native_context != 0 ? native_context[0] : r_u32(state));
        value = r_u16(first + 20);
        if ((flags & 4) == 0)
            value = 0 - value;
        w_u16(control + 24, value);
    }
    count = r_u32(control + 28) + 1;
    w_u32(control + 28, count);
    if (duration == 0)
        return 0;
    limit = r_u32(control + 32);
    return (sint32)count >= (sint32)limit;
}

uint32 sub_8007a8c4(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, value, root, tick, flags, scale, distance, i, source, offset, current, first, count, limit;
    FUNCTION_MARKER(0x8007A8C4, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        w_u32(control, 1);
        w_u32(control + 32, (sint32)duration > 0 ? duration : 240);
        value = sub_80079ae8(state, native_context);
        root = r_u32(0x800BCE60);
        w_u32(control + 40, value);
        tick = r_u32(root + 36);
        w_u32(control + 36, tick & 15);
        distance = r_u32(control + 40);
        if ((tick & 1) != 0)
        {
            if ((sint32)distance < 0x4000)
                w_u32(control + 40, 0x4000);
        }
        else if ((sint32)distance <= 0x30FFF)
            w_u32(control + 40, 0x31000);
    }
    else if (mode != 1)
        return 0;
    flags = r_u32(control + 36);
    scale = (flags & 1) != 0 ? 128 : 0xFFFFFF80;
    distance = r_u32(control + 40);
    sub_800799d4(state, distance, scale, control + 4, 0, native_context);
    for (i = 0; i < 3; ++i)
    {
        source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
        offset = r_s16(source + (i << 1));
        current = r_u32(control + 4 + (i << 2));
        w_u32(control + 4 + (i << 2), current - (offset << 6));
    }
    sub_8007a060(state, control + 4, control + 4, native_context);
    flags = r_u32(control + 36);
    if ((flags & 8) != 0)
    {
        first = (native_context != 0 ? native_context[0] : r_u32(state));
        value = r_u16(first + 20);
        if ((flags & 4) == 0)
            value = 0 - value;
        w_u16(control + 24, value);
    }
    count = r_u32(control + 28) + 1;
    w_u32(control + 28, count);
    if ((sint32)duration <= 0)
        return 0;
    limit = r_u32(control + 32);
    return (sint32)count >= (sint32)limit;
}

uint32 sub_80079f2c(uint32 state, uint32 scale, uint32 angles, uint32 output, const uint16 *native_angles, const uint32 *native_context)
{
    MATRIX matrix;
    uint32 source, value, length, a, b, c, d, e, x, y, z, first_angle, base_x, base_y, base_z;
    FUNCTION_MARKER(0x80079F2C, "ACE3.BIN");
    source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
    value = r_s16(source + 4);
    length = (sint32)(value * scale) >> 18;
    if ((sint32)length > 32767)
        length = 32767;
    if ((sint32)length < -32768)
        length = 0xFFFF8000;
    rotation_build(angles, 0, &matrix, native_angles);
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
    xport_gte_write_data(1, length & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    source = (native_context != 0 ? native_context[0] : r_u32(state));
    first_angle = native_angles != 0 ? native_angles[0] : r_u16(angles);
    base_x = r_u32(source);
    base_y = r_u32(source + 4);
    base_z = r_u32(source + 8);
    w_u16(output + 16, first_angle);
    value = native_angles != 0 ? native_angles[1] : r_u16(angles + 2);
    w_u16(output + 18, value);
    value = native_angles != 0 ? native_angles[2] : r_u16(angles + 4);
    w_u32(output, base_x + (x << 12));
    w_u32(output + 4, base_y + (y << 12));
    w_u32(output + 8, base_z + (z << 12));
    w_u16(output + 20, value);
    return value;
}

uint32 sub_8007a0f0(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, source, value, delay, limit, root, offset, current, i, count;
    FUNCTION_MARKER(0x8007A0F0, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        for (i = 0; i < 3; ++i)
        {
            source = (native_context != 0 ? native_context[0] : r_u32(state));
            value = r_u16(source + 16 + (i << 1));
            w_u16(control + 20 + (i << 1), value);
        }
        if ((sint32)duration > 0)
        {
            delay = (sint32)duration < 13 ? (uint32)((sint32)duration >> 1) : 12;
            limit = duration - delay;
        }
        else
        {
            root = r_u32(0x800BCE60);
            delay = r_u32(root + 36) & 15;
            limit = delay + 50;
        }
        w_u32(control + 32, limit);
        sub_8007995c(state, delay << 5, control + 4, 0, native_context);
        for (i = 0; i < 2; ++i)
        {
            source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
            offset = r_s16(source + (i << 1));
            current = r_u32(control + 4 + (i << 2));
            w_u32(control + 4 + (i << 2), current + (offset << 6));
        }
        source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
        offset = r_s16(source + 4);
        w_u32(control, 1);
        current = r_u32(control + 12);
        w_u32(control + 12, current + (offset << 6));
        return 0;
    }
    if (mode != 1)
        return 0;
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    ++count;
    w_u32(control + 28, count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    w_u32(control, 0);
    return 0;
}

uint32 sub_8007a6cc(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint16 angles[3];
    uint32 mode, root, divisor, numerator, quotient, magnitude, flags, value, scale, first, count, limit;
    FUNCTION_MARKER(0x8007A6CC, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        w_u32(control, 1);
        w_u32(control + 32, (sint32)duration > 0 ? duration : 320);
        root = r_u32(0x800BCE60);
        value = r_u8(root + 36);
        w_u32(control + 36, value);
    }
    else if (mode != 1)
        return 0;
    limit = r_u32(control + 32);
    count = r_u32(control + 28);
    divisor = (sint32)limit >> 1;
    numerator = (count - divisor) << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    flags = r_u32(control + 36);
    magnitude = (sint32)quotient < 0 ? 0 - quotient : quotient;
    value = r_s16(0x800104AC + (((magnitude + 0x1000) & 0x3FFF) << 1));
    scale = (0x4000 - value) * ((flags & 3) + 1) + 0x1000;
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
    if ((flags & 0x80) != 0)
    {
        angles[0] = quotient;
        if ((flags & 0x40) == 0)
        {
            angles[0] = 0 - quotient;
            flags = r_u32(control + 36);
        }
    }
    if ((flags & 0x20) != 0)
        angles[1] = (flags & 0x10) != 0 ? quotient : 0 - quotient;
    sub_80079f2c(state, scale, 0, control + 4, angles, native_context);
    flags = r_u32(control + 36);
    if ((flags & 8) != 0)
    {
        first = (native_context != 0 ? native_context[0] : r_u32(state));
        value = r_u16(first + 20);
        if ((flags & 4) == 0)
            value = 0 - value;
        w_u16(control + 24, value);
    }
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    ++count;
    w_u32(control + 28, count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    w_u32(control, 0);
    return 0;
}

uint32 sub_8007ac50(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, root, count, divisor, numerator, quotient, flags, angle, value, offset[3], first, limit;
    FUNCTION_MARKER(0x8007AC50, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        root = r_u32(0x800BCE60);
        w_u32(control + 28, 0);
        w_u32(control, 1);
        value = r_u32(root + 36);
        w_u32(control + 36, value & 15);
        w_u32(control + 32, (sint32)duration > 0 ? duration : 60);
    }
    else if (mode != 1)
        return 0;
    count = r_u32(control + 28);
    divisor = r_u32(control + 32);
    numerator = count << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    flags = r_u32(control + 36);
    angle = (((0x1000 - quotient) << 1) - 0x1000) & 0x3FFF;
    value = r_u16(0x800104AC + (angle << 1));
    value = (uint32)((sint32)(value << 16) >> 19) + 0x800;
    offset[0] = 0;
    offset[1] = 0xFFFEC000;
    offset[2] = 0xFFFB0000;
    if ((flags & 2) != 0)
    {
        value = ((value << 4) - value) << 15;
        if ((flags & 1) == 0)
            value = 0 - value;
        offset[0] = (sint32)value >> 12;
    }
    sub_80079dac(state, 0, control + 4, offset, native_context);
    flags = r_u32(control + 36);
    if ((flags & 8) != 0)
    {
        first = (native_context != 0 ? native_context[0] : r_u32(state));
        value = r_u16(first + 20);
        if ((flags & 4) == 0)
            value = 0 - value;
        w_u16(control + 24, value);
    }
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    if ((sint32)count < (sint32)limit)
        w_u32(control + 28, count + 1);
    if ((sint32)duration <= 0)
        return 0;
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    return (sint32)count >= (sint32)limit;
}

uint32 sub_80071770(uint32 object, uint32 output, NATIVE_POSE *native_output)
{
    MATRIX matrix;
    uint32 root, data, count, source, a, b, c, d, e, x, y, z, value, base, i;
    FUNCTION_MARKER(0x80071770, "ACE3.BIN");
    root = r_u32(0x800BD098);
    data = r_u32(0x800BCE50);
    count = r_u32(root + 28);
    source = r_u32(data + 12) + ((count & 1) << 3) + 0x98;
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
    value = r_u32(source);
    xport_gte_write_data(0, value);
    value = r_u32(source + 4);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    for (i = 0; i < 3; ++i)
    {
        value = xport_gte_read_data(25 + i);
        if (native_output != 0)
            native_output->position[i] = value;
        else
            w_u32(output + (i << 2), value);
    }
    x = native_output != 0 ? native_output->position[0] : r_u32(output);
    z = native_output != 0 ? native_output->position[2] : r_u32(output + 8);
    if (native_output != 0)
        native_output->position[0] = x << 4;
    else
        w_u32(output, x << 4);
    y = native_output != 0 ? native_output->position[1] : r_u32(output + 4);
    if (native_output != 0)
    {
        native_output->position[2] = z << 4;
        native_output->position[1] = y << 4;
    }
    else
    {
        w_u32(output + 8, z << 4);
        w_u32(output + 4, y << 4);
    }
    for (i = 0; i < 3; ++i)
    {
        value = native_output != 0 ? native_output->position[i] : r_u32(output + (i << 2));
        base = r_u32(object + 8 + (i << 2));
        if (native_output != 0)
            native_output->position[i] = value + base;
        else
            w_u32(output + (i << 2), value + base);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_u16(object + 24 + (i << 1));
        if (native_output != 0)
            native_output->angles[i] = value;
        else
            w_u16(output + 16 + (i << 1), value);
    }
    return value;
}

uint32 sub_800685c0(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 root, used, index, slot, value, x, y, z, speed, table, object, first, second, owner_index, i;
    FUNCTION_MARKER(0x800685C0, "ACE3.BIN");
    root = r_u32(0x800BD080);
    used = r_u32(root + 0x1548);
    if (used == 0xFFFFFFFF)
        return 0;
    index = 0;
    while ((used & (1u << (index & 31))) != 0)
        ++index;
    used |= 1u << (index & 31);
    root = r_u32(0x800BD080);
    w_u32(root + 0x1548, used);
    slot = root + (flags & 63);
    value = r_u8(slot + 0x48);
    w_u8(slot + 0x48, value + 1);
    root = r_u32(0x800BD080);
    slot = root + 0xC8 + index * 164;
    w_u8(slot + 4, 0);
    w_u16(slot + 8, 0);
    w_u16(slot + 10, 0);
    w_u32(slot, 0);
    w_u8(slot + 0x34, 1);
    w_u8(slot + 0x35, 0);
    w_u16(slot + 0x1A, 64);
    w_u32(slot + 0x6C, flags);
    for (i = 0; i < 3; ++i)
    {
        value = native_position != 0 ? native_position->position[i] : r_u32(position + (i << 2));
        w_u32(slot + 0x1C + (i << 2), value);
    }
    for (i = 0; i < 3; ++i)
    {
        value = native_position != 0 ? native_position->angles[i] : r_u16(position + 16 + (i << 1));
        w_u16(slot + 0x2C + (i << 1), value);
    }
    value = r_u32(vector);
    w_u32(slot + 0x38, value);
    y = r_u32(vector + 4);
    x = r_u32(slot + 0x38);
    w_u32(slot + 0x3C, y);
    z = r_u32(vector + 8);
    w_u32(slot + 0x40, z);
    speed = sub_80083f40((sint32)x >> 12, (sint32)y >> 12, (sint32)z >> 12) << 12;
    w_u32(slot + 0x64, speed);
    w_u32(slot + 0x50, speed);
    w_u32(slot + 0x60, 0);
    w_u32(slot + 0x48, 0);
    w_u32(slot + 0x4C, 0);
    w_u16(slot + 0x58, 0);
    w_u16(slot + 0x5A, 0);
    w_u16(slot + 0x5C, 0);
    w_u32(slot + 0x68, 0x1D4C0);
    w_u32(slot + 0x70, owner);
    if (owner != 0xFFFFFFFF)
    {
        owner_index = owner & 63;
        table = r_u32(0x800BCE5C);
        root = r_u32(0x800BD080);
        object = r_u32(table + (owner_index << 2));
        value = r_u8(root + owner_index + 0x88);
        first = object + 8;
        w_u8(root + owner_index + 0x88, value + 1);
        root = r_u32(0x800BD080);
        w_u8(root + owner_index + 8, index);
        second = object + 0x38;
        root = r_u32(0x800BD080);
        if ((owner & 0x20) != 0)
        {
            value = r_u32(root + 4);
            w_u32(root + 4, value | (1u << (owner & 31)));
        }
        else
        {
            value = r_u32(root);
            w_u32(root, value | (1u << (owner_index & 31)));
        }
    }
    else
    {
        second = slot + 0x1C;
        value = r_u32(slot);
        first = second;
        w_u32(slot, value | 1);
    }
    w_u32(slot + 0x74, first);
    w_u32(slot + 0x78, second);
    return slot;
}

uint32 sub_80068800(uint32 flags, uint32 position, uint32 vector, uint32 owner, uint32 mode, const NATIVE_POSE *native_position)
{
    uint32 slot, value, kind, first_index, second_index, target, a, b, c, speed, limit, threshold;
    FUNCTION_MARKER(0x80068800, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    if ((mode & 0x1000000) == 0)
    {
        value = r_u8(slot + 0x35);
        w_u8(slot + 0x35, value | 8);
    }
    value = r_u8(slot + 0x35);
    w_u8(slot + 0x35, value | ((mode & 0x2000000) != 0 ? 0x20 : 0x10));
    if ((mode & 0x4000000) != 0)
    {
        value = r_u32(slot);
        w_u32(slot, value | 0x80);
    }
    if ((mode & 0x8000000) != 0)
    {
        value = r_u32(slot);
        w_u32(slot, value | 0x100);
    }
    value = r_u32(slot);
    kind = mode & 255;
    w_u8(slot + 6, 0);
    w_u8(slot + 7, 0);
    w_u32(slot, value | 8);
    first_index = (mode >> 8) & 255;
    second_index = (mode >> 16) & 255;
    if (kind == 2)
    {
        target = r_u32(slot + 0x74);
        value = sub_80083dfc(position, target, native_position != 0 ? native_position->position : 0, 0);
        kind = (sint32)value >= 5000;
    }
    if (kind == 0)
    {
        w_u8(slot + 5, 0);
        a = r_u16(0x800BCDEC + (first_index << 1));
        w_u16(slot + 0x18, a);
        b = r_u16(0x800BCDDC + (second_index << 1));
        w_u16(slot + 0x7C, b);
        c = r_u16(0x800BCDE4 + (second_index << 1));
        speed = 0x50000;
        limit = 0x1E00;
        threshold = 0xE00;
    }
    else if (kind == 1)
    {
        w_u8(slot + 5, 1);
        a = r_u16(0x800BCE04 + (first_index << 1));
        w_u16(slot + 0x18, a);
        b = r_u16(0x800BCDF4 + (second_index << 1));
        w_u16(slot + 0x7C, b);
        c = r_u16(0x800BCDFC + (second_index << 1));
        speed = 0x78000;
        limit = 0x1400;
        threshold = 0x1400;
    }
    else
        return slot;
    w_u32(slot + 0x80, speed);
    w_u32(slot + 0x84, limit);
    w_u32(slot + 0x88, threshold);
    w_u16(slot + 0x7E, c);
    return slot;
}

uint32 sub_80071950(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071950, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 150);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(14);
    sub_80029448(1);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068800(flags, 0, object + 0x38, owner, 0x10102, &pose);
}

void sub_80031e9c(void)
{
    FUNCTION_MARKER(0x80031E9C, "ACE3.BIN");
}

void sub_80032008(void)
{
    FUNCTION_MARKER(0x80032008, "ACE3.BIN");
}

void sub_80031f1c(void)
{
    FUNCTION_MARKER(0x80031F1C, "ACE3.BIN");
}

uint32 sub_80068c0c(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 slot, value;
    FUNCTION_MARKER(0x80068C0C, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    w_u16(slot + 0x18, 0x53);
    w_u16(slot + 0x7C, 20);
    w_u16(slot + 0x7E, 1);
    w_u32(slot + 0x80, 0xA0000);
    value = r_u8(slot + 0x35);
    w_u16(slot + 0x30, 0);
    w_u8(slot + 6, 0);
    w_u8(slot + 7, 0);
    w_u8(slot + 5, 0);
    w_u32(slot + 0x84, 0x5A00);
    w_u32(slot + 0x88, 0x5A00);
    w_u8(slot + 0x35, value | 0x18);
    return slot;
}

uint32 sub_80068c80(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 slot, value;
    FUNCTION_MARKER(0x80068C80, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    w_u16(slot + 0x1A, 128);
    w_u8(slot + 0x34, 3);
    w_u8(slot + 6, 2);
    w_u8(slot + 5, 1);
    w_u16(slot + 0x18, 150);
    w_u16(slot + 0x7C, 18);
    w_u16(slot + 0x7E, 7);
    value = r_u8(slot + 0x35);
    w_u32(slot + 0x80, 0x3E000);
    w_u8(slot + 7, 0);
    w_u32(slot + 0x84, 0x5A00);
    w_u32(slot + 0x88, 0x5A00);
    w_u8(slot + 0x35, value | 0x18);
    return slot;
}

uint32 sub_80068d0c(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 slot, value, table, target, distance;
    FUNCTION_MARKER(0x80068D0C, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    w_u16(slot + 0x1A, 256);
    w_u8(slot + 0x34, 2);
    w_u8(slot + 6, 3);
    w_u8(slot + 5, 1);
    w_u16(slot + 0x18, 150);
    w_u16(slot + 0x7C, 30);
    w_u16(slot + 0x7E, 7);
    w_u32(slot + 0x84, 0x1200);
    value = r_u8(slot + 0x35);
    w_u32(slot + 0x80, 0x48000);
    w_u8(slot + 7, 0);
    w_u32(slot + 0x88, 0xA00);
    w_u8(slot + 0x35, value | 0x18);
    if (owner != 0xFFFFFFFF)
    {
        table = r_u32(0x800BCE5C);
        target = r_u32(table + ((owner & 63) << 2));
        distance = sub_80083dfc(position, target + 8, native_position != 0 ? native_position->position : 0, 0);
        if ((owner & 0x20) != 0)
            value = (uint32)((sint32)distance >> 2) + (uint32)((sint32)distance >> 1);
        else
        {
            value = (distance << 4) - distance;
            value = (value << 2) + distance;
            value = ((value << 2) - distance) << 2;
            value = (sint32)value >> 10;
        }
        w_u32(slot + 0x98, value);
        w_u32(slot + 0x68, distance);
    }
    return slot;
}

uint32 sub_80068a58(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 slot, x, y, z, value;
    FUNCTION_MARKER(0x80068A58, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    x = r_u32(slot + 0x38);
    w_u16(slot + 0x1A, 512);
    w_u8(slot + 0x34, 4);
    w_u8(slot + 5, 2);
    w_u8(slot + 7, 3);
    w_u16(slot + 0x18, 240);
    w_u32(slot + 0x9C, 1024);
    w_u32(slot + 0xA0, 192);
    y = r_u32(slot + 0x3C);
    w_u8(slot + 6, 0);
    w_u32(slot + 0x38, x + (uint32)((sint32)x >> 1));
    z = r_u32(slot + 0x40);
    w_u32(slot + 0x3C, y + (uint32)((sint32)y >> 1));
    value = r_u32(slot);
    w_u32(slot + 0x40, z + (uint32)((sint32)z >> 1));
    w_u32(slot, value | 0x10);
    return slot;
}

uint32 sub_80068afc(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 slot, x, y, z, value;
    FUNCTION_MARKER(0x80068AFC, "ACE3.BIN");
    slot = sub_800685c0(flags, position, vector, owner, native_position);
    if (slot == 0)
        return 0;
    x = r_u32(slot + 0x38);
    w_u16(slot + 0x1A, 1024);
    w_u8(slot + 0x34, 4);
    w_u8(slot + 5, 2);
    w_u8(slot + 7, 1);
    w_u16(slot + 0x18, 240);
    w_u32(slot + 0x9C, 1260);
    w_u32(slot + 0xA0, 192);
    y = r_u32(slot + 0x3C);
    w_u8(slot + 6, 0);
    w_u32(slot + 0x38, x + (uint32)((sint32)x >> 1));
    w_u32(slot + 0x3C, y + (uint32)((sint32)y >> 1));
    value = r_u32(slot);
    z = r_u32(slot + 0x40);
    value |= 0x10;
    w_u32(slot, value);
    w_u32(slot + 0x40, z + (uint32)((sint32)z >> 1));
    w_u32(slot, value | 0x40);
    return slot;
}

uint32 sub_80071ab0(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071AB0, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 75);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(14);
    sub_80029448(2);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068c0c(flags, 0, object + 0x38, owner, &pose);
}

uint32 sub_80071b54(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071B54, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 150);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(16);
    sub_80029448(3);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068c80(flags, 0, object + 0x38, owner, &pose);
}

uint32 sub_80071bf8(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071BF8, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 250);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(14);
    sub_80029448(4);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068d0c(flags, 0, object + 0x38, owner, &pose);
}

uint32 sub_80071c9c(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071C9C, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 180);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(16);
    sub_80029448(5);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068a58(flags, 0, object + 0x38, owner, &pose);
}

uint32 sub_80071d40(uint32 slots)
{
    NATIVE_POSE pose;
    uint32 root, result, count, state, value, table, object, flags, owner;
    FUNCTION_MARKER(0x80071D40, "ACE3.BIN");
    root = r_u32(0x800BD098);
    result = r_u32(root + 28);
    if (result == 0)
        return result;
    count = r_u32(root + 4);
    result = sub_800718a0(slots, count, 180);
    if (result == 0)
        return result;
    state = r_u32(0x800BD094);
    value = r_u32(state + 16);
    root = r_u32(0x800BD098);
    w_u32(state + 16, value | 0x4000);
    value = r_u32(root + 28);
    table = r_u32(0x800BCE5C);
    w_u32(root + 28, value - 1);
    object = r_u32(table);
    sub_8002db4c(15);
    sub_80029448(6);
    sub_80071770(object, 0, &pose);
    root = r_u32(0x800BD098);
    flags = r_u32(object);
    owner = r_u32(root + 24);
    return sub_80068afc(flags, 0, object + 0x38, owner, &pose);
}

uint32 sub_80071544(void)
{
    uint32 state, result, value;
    FUNCTION_MARKER(0x80071544, "ACE3.BIN");
    state = r_u32(0x800BD094);
    result = r_s16(state + 86);
    if (result != 0)
        return result;
    value = r_u32(state + 16);
    w_u32(state + 16, value | 0x2000);
    sub_800713b0(1);
    sub_8002db4c(18);
    sub_800294c4(2);
    sub_80090110(0x30001);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 3);
    return 3;
}

uint32 sub_800715ac(void)
{
    uint32 state, result, value;
    FUNCTION_MARKER(0x800715AC, "ACE3.BIN");
    state = r_u32(0x800BD094);
    result = r_s16(state + 86);
    if (result != 0)
        return result;
    value = r_u32(state + 16);
    w_u32(state + 16, value | 0x2000);
    sub_800713b0(2);
    sub_8002db4c(19);
    sub_800294c4(3);
    sub_80090110(0x30002);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 6);
    return 6;
}

uint32 sub_80071614(void)
{
    uint32 state, result, value;
    FUNCTION_MARKER(0x80071614, "ACE3.BIN");
    state = r_u32(0x800BD094);
    result = r_s16(state + 86);
    if (result != 0)
        return result;
    value = r_u32(state + 16);
    w_u32(state + 16, value | 0x2000);
    sub_800713b0(3);
    sub_8002db4c(20);
    sub_800294c4(4);
    sub_80090110(0x40003);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 2);
    return 2;
}

uint32 sub_80071708(void)
{
    uint32 state, result, value;
    FUNCTION_MARKER(0x80071708, "ACE3.BIN");
    state = r_u32(0x800BD094);
    result = r_s16(state + 86);
    if (result != 0)
        return result;
    value = r_u32(state + 16);
    w_u32(state + 16, value | 0x2000);
    sub_800713b0(5);
    sub_8002db4c(21);
    sub_800294c4(6);
    sub_80090110(0x40004);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 3);
    return 3;
}

uint32 sub_8007167c(void)
{
    uint32 state, count, result, value;
    FUNCTION_MARKER(0x8007167C, "ACE3.BIN");
    state = r_u32(0x800BD094);
    count = r_s16(state + 86);
    if (count != 0)
    {
        result = ~count;
        if ((sint32)count < 17)
            return result;
        result &= 1;
        if (result == 0)
            return 0;
        return sub_800713b0(4);
    }
    value = r_u32(state + 16);
    w_u32(state + 16, value | 0x2000);
    sub_80090110(0x100005);
    sub_800713b0(4);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 32);
    sub_8002db4c(21);
    return sub_800294c4(5);
}

uint32 sub_800720d4(uint32 kind, uint32 slots)
{
    uint32 target;
    FUNCTION_MARKER(0x800720D4, "ACE3.BIN");
    target = r_u32(0x800B3B60 + (kind << 2));
    return target != 0 ? xport_guest_call3(target, slots, 0, 0) : 0;
}

uint32 sub_80072094(uint32 kind)
{
    uint32 target;
    FUNCTION_MARKER(0x80072094, "ACE3.BIN");
    target = r_u32(0x800B3B44 + (kind << 2));
    return target != 0 ? xport_guest_call3(target, 0, 0, 0) : 0x800B3B44;
}

uint32 sub_80084560(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 count, remaining, current, next, tail, value, i, address, lane, shift;
    FUNCTION_MARKER(0x80084560, "ACE3.BIN");
    count = (sint32)strength >> 8;
    if ((sint32)count <= 0)
        return packet;
    next = packet + 60;
    remaining = 16 - count;
    current = packet + 8;
    w_u32(packet, (current & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 4, 0xE6000000);
    w_u32(packet + 8, (next & 0xFFFFFF) | 0x0C000000);
    w_u32(current + 4, 0xA0000000);
    w_u32(current + 8, 0x010001D0);
    w_u32(current + 12, 0x10010);
    w_u32(current + 48, 0x01000000);
    for (i = 0; i < 16; ++i, current += 2)
        w_u16(current + 16, (sint32)i < (sint32)remaining ? 0 : 0x8000);
    current = next + 16;
    w_u32(next, (current & 0xFFFFFF) | 0x03000000);
    w_u32(next + 4, 0xE1000238);
    w_u32(next + 8, 0xE20C4318);
    w_u32(next + 12, 0);
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, 0);
    tail = sub_80084318(current, 0x401D, 0xFF88FF60, 0xF00140, 0);
    w_u32(tail + 4, 0xE1000220);
    w_u32(tail, 0x03000000);
    w_u32(tail + 8, 0xE2000000);
    w_u32(tail + 12, 0);
    /* The E2000000 LWL seed has no retained low bits */
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
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

uint32 sub_8008591c(uint32 state, uint32 packet)
{
    uint32 center_x, center_y, flags, angle, previous, offset, trig, cosine, width, height, packed, ax, ay, bx, by, value;
    FUNCTION_MARKER(0x8008591C, "ACE3.BIN");
    center_x = r_s16(state + 28);
    flags = r_u32(state + 16);
    center_y = r_s16(state + 30);
    angle = ((uint32)((sint32)flags >> 2)) & 0xFFF;
    if (angle != 0)
    {
        previous = r_u32(state + 88);
        offset = r_u32(state + 24);
        if (angle != previous)
        {
            trig = r_u32(0x800B7104 + (angle << 2));
            w_u32(state + 88, angle);
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
        height = r_u16(state + 22);
        width = r_u16(state + 20);
        height = (uint32)(sint32)(sint16)height;
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
        value = r_s16(state + 24);
        offset = r_s16(state + 26);
        center_x += value;
        width = r_u16(state + 20);
        center_y += offset;
        ax = (sint32)(width << 16) >> 17;
        bx = ax;
        height = r_u16(state + 22);
        ay = (sint32)(height << 16) >> 17;
        height = (uint32)(sint32)(sint16)height;
        by = (sint32)(1 - height) >> 1;
    }
    w_u16(packet + 8, center_x - ax);
    w_u16(packet + 10, center_y - ay);
    w_u16(packet + 16, center_x + bx);
    w_u16(packet + 18, center_y + by);
    w_u16(packet + 24, center_x - bx);
    w_u16(packet + 26, center_y - by);
    w_u16(packet + 32, center_x + ax);
    value = center_y + ay;
    w_u16(packet + 34, value);
    return value;
}

uint32 sub_80085a94(uint32 state, uint32 geometry, uint32 frame)
{
    uint32 packed, packet, divisor, numerator, index, value, size, coordinate, shift, u, right, v, bottom, mode, table, bucket, result;
    FUNCTION_MARKER(0x80085A94, "ACE3.BIN");
    packed = r_u32(frame);
    packet = r_u32(state + 4);
    divisor = (sint32)packed >> 16;
    if (divisor == 0)
        divisor = 1;
    value = (uint32)(sint32)(sint16)packed;
    numerator = ((value << 2) + value) << 1;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    index = (sint32)numerator / (sint32)divisor;
    coordinate = r_u32(geometry);
    size = r_s16(geometry + 6);
    w_u32(state + 24, 0);
    w_u32(state + 28, coordinate);
    w_u16(state + 22, size);
    index += 6;
    w_u16(state + 20, size);
    if ((sint32)size < 8)
        w_u32(state + 16, 0);
    sub_8008591c(state, packet);
    shift = (sint32)size < 17;
    value = (index & 3) << 6;
    u = (sint32)value >> shift;
    right = (uint32)((sint32)(value + 64) >> shift) - 1;
    value = (uint32)((sint32)index >> 2) << 6;
    v = (sint32)value >> shift;
    w_u8(packet + 21, v);
    w_u8(packet + 13, v);
    bottom = (uint32)((sint32)(value + 64) >> shift) - 1;
    w_u8(packet + 28, u);
    w_u8(packet + 12, u);
    w_u8(packet + 36, right);
    w_u8(packet + 20, right);
    w_u8(packet + 37, bottom);
    w_u8(packet + 29, bottom);
    mode = r_u32(state + 8);
    w_u32(packet + 4, 0x2E838383);
    if (mode != 0)
    {
        w_u16(packet + 14, 0x4016);
        value = ((mode & 3) << 5) | 0x17;
    }
    else
    {
        w_u16(packet + 14, 0x4056);
        value = 0x17;
    }
    w_u16(packet + 22, value);
    if ((mode & 1) == 0)
        w_u32(packet + 40, 0xE1000220);
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    value = r_u32(table + (bucket << 2));
    w_u32(packet, value | ((mode & 1) != 0 ? 0x09000000 : 0x0A000000));
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    w_u32(table + (bucket << 2), packet & 0xFFFFFF);
    result = packet + ((mode & 1) != 0 ? 40 : 44);
    w_u32(state + 4, result);
    return result;
}

uint32 sub_80085c8c(uint32 state, uint32 geometry, uint32 frame)
{
    uint32 packed, packet, divisor, low, numerator, quotient, size, mode, flags, value, index, bucket, table, result, shift, u, right, v, bottom, coordinate;
    FUNCTION_MARKER(0x80085C8C, "ACE3.BIN");
    packed = r_u32(frame);
    packet = r_u32(state + 4);
    divisor = (sint32)packed >> 16;
    if (divisor == 0)
        divisor = 1;
    low = (uint32)(sint32)(sint16)packed;
    numerator = low << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    size = r_s16(geometry + 6);
    xport_gte_write_data(25, 6);
    xport_gte_write_data(9, 10);
    xport_gte_write_data(8, quotient);
    mode = r_u32(state + 12);
    xport_gte_write_data(26, size);
    if (mode != 0)
        size <<= 1;
    xport_gte_write_data(10, size);
    value = r_u32(state + 16);
    w_u32(state + 16, value + (low << ((mode + 5) & 31)));
    xport_gte_execute(0x1A8003E);
    flags = r_u32(state + 8);
    xport_gte_write_control(21, 60);
    xport_gte_write_control(22, 60);
    xport_gte_write_control(23, 60);
    xport_gte_write_data(6, 0x2E535353);
    size = xport_gte_read_data(26);
    index = xport_gte_read_data(25);
    w_u16(state + 22, size);
    w_u16(state + 20, size);
    xport_gte_execute(0x780010);
    if ((flags & 1) == 0)
    {
        w_u16(packet + 22, ((flags & 3) << 5) | 0x17);
        w_u32(packet + 40, 0xE1000220);
    }
    else
        w_u16(packet + 22, 0x37);
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    value = r_u32(table + (bucket << 2));
    w_u32(packet, value | ((flags & 1) != 0 ? 0x09000000 : 0x0A000000));
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    w_u32(table + (bucket << 2), packet & 0xFFFFFF);
    result = packet + ((flags & 1) != 0 ? 40 : 44);
    w_u32(state + 4, result);
    if ((sint32)size < 8)
        w_u32(state + 16, 0);
    shift = (sint32)size < 17;
    value = (index & 3) << 6;
    u = (sint32)value >> shift;
    right = (uint32)((sint32)(value + 64) >> shift) - 1;
    value = (uint32)((sint32)index >> 2) << 6;
    v = (sint32)value >> shift;
    w_u8(packet + 21, v);
    w_u8(packet + 13, v);
    bottom = (uint32)((sint32)(value + 64) >> shift) - 1;
    w_u8(packet + 37, bottom);
    w_u8(packet + 29, bottom);
    w_u8(packet + 28, u);
    w_u8(packet + 12, u);
    w_u8(packet + 36, right);
    w_u8(packet + 20, right);
    w_u16(packet + 14, 0x40D6);
    value = xport_gte_read_data(22);
    w_u32(packet + 4, value);
    coordinate = r_u32(geometry);
    w_u32(state + 24, 0);
    w_u32(state + 28, coordinate);
    return sub_8008591c(state, packet);
}

uint32 sub_800867e4(uint32 angles, uint32 scale, uint32 output, MATRIX *native_output)
{
    uint32 ax, ay, az, cx, sx, cy, sy, cz, sz, a, b, c, d, e, f, g, h, i, j, k, p, q, r, product, result;
    FUNCTION_MARKER(0x800867E4, "ACE3.BIN");
    ax = r_u16(angles);
    ay = r_u16(angles + 2);
    ax = (ax & 0xFFF) << 2;
    ay = (ay & 0xFFF) << 2;
    cx = r_s16(0x800B7106 + ax);
    sx = r_s16(0x800B7104 + ax);
    cy = r_s16(0x800B7106 + ay);
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, sx);
    xport_gte_write_data(10, cx);
    xport_gte_write_data(11, cy);
    sy = r_s16(0x800B7104 + ay);
    xport_gte_execute(0x198003D);
    az = r_u16(angles + 4);
    az = (az & 0xFFF) << 2;
    cz = r_s16(0x800B7106 + az);
    sz = r_s16(0x800B7104 + az);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, b);
    xport_gte_write_data(9, sy);
    xport_gte_write_data(10, sz);
    xport_gte_write_data(11, cz);
    if (native_output != 0)
        xport_store_le16((uint8 *)native_output + 10, 0 - a);
    else
        w_u16(output + 10, 0 - a);
    xport_gte_execute(0x198003D);
    product = (sint32)(scale * sy) >> 12;
    d = xport_gte_read_data(25);
    e = xport_gte_read_data(26);
    f = xport_gte_read_data(27);
    xport_gte_write_data(8, c);
    xport_gte_write_data(9, sz);
    xport_gte_write_data(10, cz);
    xport_gte_write_data(11, cx);
    if (native_output != 0)
    {
        xport_store_le16((uint8 *)native_output + 4, d);
        xport_store_le16((uint8 *)native_output + 6, e);
    }
    else
    {
        w_u16(output + 4, d);
        w_u16(output + 6, e);
    }
    xport_gte_execute(0x198003D);
    if (native_output != 0)
        xport_store_le16((uint8 *)native_output + 8, f);
    else
        w_u16(output + 8, f);
    g = xport_gte_read_data(25);
    h = xport_gte_read_data(26);
    i = xport_gte_read_data(27);
    xport_gte_write_data(8, product);
    xport_gte_write_data(9, cz);
    xport_gte_write_data(10, sz);
    if (native_output != 0)
        xport_store_le16((uint8 *)native_output + 16, i);
    else
        w_u16(output + 16, i);
    xport_gte_execute(0x198003D);
    j = xport_gte_read_data(25);
    k = xport_gte_read_data(26);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, j);
    xport_gte_write_data(10, g);
    xport_gte_write_data(11, h);
    xport_gte_execute(0x198003D);
    p = xport_gte_read_data(25);
    q = xport_gte_read_data(26);
    r = xport_gte_read_data(27);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, k);
    if (native_output != 0)
        xport_store_le16((uint8 *)native_output + 2, p - g);
    else
        w_u16(output + 2, p - g);
    xport_gte_execute(0x198003D);
    if (native_output != 0)
    {
        xport_store_le16((uint8 *)native_output + 12, q - j);
        xport_store_le16((uint8 *)native_output + 14, k + r);
    }
    else
    {
        w_u16(output + 12, q - j);
        w_u16(output + 14, k + r);
    }
    result = xport_gte_read_data(25);
    if (native_output != 0)
        xport_store_le16((uint8 *)native_output, h + result);
    else
        w_u16(output, h + result);
    return result;
}

uint32 sub_8008158c(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 dx, dy, dz, x, y, z, a, b, c, seed, random, root, record, value, result;
    FUNCTION_MARKER(0x8008158C, "ACE3.BIN");
    dx = (native_vector ? native_vector[0] : r_u32(vector));
    a = (native_position ? native_position[0] : r_u32(position));
    dy = (native_vector ? native_vector[1] : r_u32(vector + 4));
    dz = (native_vector ? native_vector[2] : r_u32(vector + 8));
    c = (native_position ? native_position[2] : r_u32(position + 8));
    x = a + dx;
    b = (native_position ? native_position[1] : r_u32(position + 4));
    z = c + dz;
    y = b + dy;
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        w_u16(record, 0xFFFF);
        w_u16(record + 2, (random & 7) + 9);
        w_u32(record + 4, x + (((random & 255) - 128) << 9));
        value = ((uint32)((sint32)random >> 3) & 255) - 128;
        w_u32(record + 8, y + (value << 9));
        value = ((uint32)((sint32)random >> 6) & 255) - 128;
        w_u32(record + 12, z + (value << 9));
        w_u16(record + 24, 256);
        w_u16(record + 22, 0);
        w_u16(record + 26, (random & 63) + 90);
    }
    result = random & 1;
    if (result == 0)
        return 0;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    result = random & 7;
    if (record == 0)
        return result;
    w_u16(record, 0);
    w_u16(record + 2, result + 16);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = (uint32)((sint32)dx >> 8) - 64 + (random & 127);
    w_u16(record + 16, value);
    value = (uint32)((sint32)dy >> 8) - 64 + ((uint32)((sint32)random >> 3) & 127);
    w_u16(record + 18, value);
    value = (uint32)((sint32)dz >> 8) - 64 + ((uint32)((sint32)random >> 6) & 127);
    w_u16(record + 20, value);
    if ((random & 1) != 0)
    {
        w_u16(record + 22, 0x8D);
        w_u16(record + 24, 0x1504);
        result = (random & 63) + 150;
    }
    else
    {
        w_u16(record + 22, 0x18C);
        result = (random & 7) + 20;
        w_u16(record + 24, 0);
    }
    w_u16(record + 26, result);
    return result;
}

uint32 sub_800694cc(uint32 state)
{
    uint32 divisor, scale, numerator, flags, vector, source, x, y, z, value, target[3], distance, limit, angle, raw, clamped, speed, acceleration, result, magnitude, maximum;
    FUNCTION_MARKER(0x800694CC, "ACE3.BIN");
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

uint32 sub_8004986c(uint32 index)
{
    uint32 root, state, table, other_index, object, other, flags, mode, value, current, raw, difference, offset, i, result;
    FUNCTION_MARKER(0x8004986C, "ACE3.BIN");
    root = r_u32(0x800BCEC0);
    state = root + index * 184;
    table = r_u32(0x800BCE5C);
    other_index = r_u8(state + 63);
    flags = 0x800C6FD0 + (index << 2);
    object = r_u32(table + (index << 2));
    value = r_u32(flags);
    other = r_u32(table + (other_index << 2));
    w_u32(flags, value | 0x20);
    mode = r_u8(state + 100);
    if (mode == 0)
    {
        w_u8(state + 100, 1);
        return 1;
    }
    if (mode != 1)
    {
        if (mode == 2)
            w_u8(state + 99, 255);
        else
            w_u8(state + 100, 0);
        return 255;
    }
    for (i = 0; i < 3; ++i)
    {
        offset = i == 0 ? 24 : i == 1 ? 28 : 26;
        value = r_s16(other + offset);
        current = r_s16(object + offset);
        raw = r_u16(object + offset);
        difference = (value - current) & 0x3FFF;
        if (difference >= 0x2001)
            difference -= 0x4000;
        value = difference;
        if ((sint32)value < 0)
            value += 3;
        value = (sint32)value >> 2;
        w_u16(object + offset, raw + value);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(state + 104 + (i << 2));
        current = r_u32(object + 8 + (i << 2));
        result = (sint32)(value - current) >> 8;
        w_u32(object + 8 + (i << 2), current + result);
    }
    return result;
}

uint32 sub_8004e94c(uint32 index)
{
    uint32 status, table, root, record, object, data, attributes, initial, value, mode, raw, result;
    FUNCTION_MARKER(0x8004E94C, "ACE3.BIN");
    status = 0x800C7050 + (index << 2);
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BCEFC);
    record = root + index * 40;
    object = r_u32(table + (index << 2) + 128);
    data = r_u32(record + 8);
    initial = r_u32(status);
    attributes = r_u32(record + 12);
    w_u32(status, initial | 1);
    value = r_u8(data + 28);
    if ((value & 1) != 0)
        w_u32(status, initial | 0x101);
    value = r_u8(data + 28);
    if ((value & 2) != 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x200);
    }
    value = r_u8(attributes + 7);
    if (value != 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x1000);
    }
    value = r_u8(attributes + 8);
    if (value != 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x2000);
    }
    value = r_u8(attributes + 6);
    if ((value & 2) != 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x4000);
    }
    value = r_u8(attributes + 6);
    if ((value & 1) != 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x8000);
    }
    value = r_u8(attributes + 10);
    if (value == 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x10);
    }
    value = r_u8(attributes + 3);
    if (value != 255)
    {
        value = r_u32(status);
        w_u32(status, value | 0x800000);
    }
    value = r_u8(attributes + 4);
    if (value == 0)
    {
        value = r_u32(status);
        w_u32(status, value | 0x400);
    }
    value = r_u32(status);
    if ((value & 0x1000) != 0)
        w_u32(status, value | 0x800);
    sub_8004eba4(index + 32, attributes);
    sub_8004ecc4(index + 32, attributes);
    mode = r_u8(attributes + 9);
    if (mode == 1)
    {
        value = r_u32(object);
        w_u32(object, value | 0x800);
    }
    else if (mode == 2)
    {
        value = r_u32(object);
        w_u32(object, value | 0x1000);
    }
    value = r_s16(attributes + 12);
    raw = r_u16(attributes + 12);
    if (value != 0xFFFFFFFF)
    {
        value = r_u32(object);
        w_u16(object + 0x4A, raw);
        w_u32(object, value | 0x200);
    }
    else
        w_u16(object + 0x4A, 0);
    result = r_s16(attributes + 14);
    raw = r_u16(attributes + 14);
    if (result != 0xFFFFFFFF)
    {
        result = r_u32(object);
        w_u16(object + 0x4C, raw);
        result |= 0x400;
        w_u32(object, result);
    }
    else
        w_u16(object + 0x4C, 0);
    return result;
}

uint32 sub_8006447c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    MATRIX first, second;
    uint16 angles[3];
    uint32 source, value, a, b, c, d, e, x, y, z, p, q, r, u, v, w, projected, depth, pass, root, storage, packet, count;
    FUNCTION_MARKER(0x8006447C, "ACE3.BIN");
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    source = (native_state ? native_state->words[4] : r_u32(state + 16));
    sub_800308f0(source, 0, &first, 0);
    for (pass = 0; pass < 2; ++pass)
    {
        if (pass != 0)
        {
            sub_800308f0(0, 0, &second, angles);
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
        }
        a = xport_load_le32((uint8 *)&first);
        b = xport_load_le32((uint8 *)&first + 4);
        c = xport_load_le32((uint8 *)&first + 8);
        d = xport_load_le32((uint8 *)&first + 12);
        e = xport_load_le16((uint8 *)&first + 16);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        xport_gte_write_control(2, c);
        xport_gte_write_control(3, d);
        xport_gte_write_control(4, e);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 256);
        xport_gte_execute(0x180001);
        projected = xport_gte_read_data(14);
        xport_gte_read_data(8);
        xport_gte_read_control(31);
        depth = (sint32)xport_gte_read_data(19) >> 2;
        if (pass == 0)
        {
            w_u32(0x800C6D74, projected);
            w_u32(0x800C6D78, depth);
            if (native_state && native_state->delta_angles)
            {
                angles[0] = 0 - native_state->delta_angles[0];
                angles[1] = 0 - native_state->delta_angles[1];
            }
            else
            {
                source = native_state ? native_state->words[3] : r_u32(state + 12);
                angles[0] = 0 - r_u16(source);
                source = native_state ? native_state->words[3] : r_u32(state + 12);
                angles[1] = 0 - r_u16(source + 2);
            }
            angles[2] = 0;
            if (angles[0] == 0 && angles[1] == 0)
                return 0;
        }
    }
    if ((sint32)depth <= 0)
        return 0;
    root = r_u32(0x800BD074);
    storage = r_u32(0x800BD078);
    count = r_u32(root + 20);
    value = r_u8(0x800C6D68);
    packet = storage + (count << 4) + 0x3D8;
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6A);
    w_u8(packet + 6, value);
    w_u16(packet + 8, (projected & 0xFFFF) - 2);
    root = r_u32(0x800BD074);
    w_u16(packet + 12, (projected & 0xFFFF) + 2);
    value = projected >> 16;
    w_u16(packet + 14, value);
    w_u16(packet + 10, value);
    count = r_u32(root + 20);
    w_u32(root + 20, count + 1);
    value = r_u8(0x800C6D68);
    packet += 16;
    w_u8(packet + 4, value);
    value = r_u8(0x800C6D69);
    w_u8(packet + 5, value);
    value = r_u8(0x800C6D6A);
    w_u8(packet + 6, value);
    w_u16(packet + 12, projected);
    w_u16(packet + 8, projected);
    root = r_u32(0x800BD074);
    w_u16(packet + 10, (projected >> 16) - 3);
    w_u16(packet + 14, projected >> 16);
    count = r_u32(root + 20);
    w_u32(root + 20, count + 1);
    return 0;
}

uint32 sub_800957a4(uint32 ordering, uint32 packet)
{
    uint32 root, mode, count, scene, value, tail, flag, seed, address, lane, shift, i;
    FUNCTION_MARKER(0x800957A4, "ACE3.BIN");
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, packet + 8);
    sub_80094718(0x1F800000, 0x1F80001C);
    root = r_u32(0x800BCE60);
    mode = r_s16(root + 0x38C);
    w_u32(0x1F80018C, mode == 6);
    sub_80094d10(0x1F800000, 0x1F80001C);
    count = sub_80094ac8(0x1F800000, 0x1F80001C);
    sub_800948a0(0x1F80001C, count);
    sub_800949ac(0x1F80001C);
    sub_80095074(0x1F800000, 0x1F80001C);
    scene = r_u32(0x800BCE54);
    root = r_u32(scene + 0x244);
    value = r_s16(root + 2);
    if ((value & 0x200) != 0)
        sub_80095930(0x1F800000, 0x1F80001C);
    value = r_u32(0x1F800184);
    flag = (value & 4) != 0;
    tail = packet;
    value = r_u32(0x1F800004);
    if (value == packet + 8)
    {
        w_u32(packet, 0x01000000);
        w_u32(packet + 4, flag | 0xE6000000);
        seed = packet + 8;
    }
    else
    {
        w_u32(packet, ((packet + 8) & 0xFFFFFF) | 0x01000000);
        w_u32(packet + 4, 0xE6000001);
        tail = r_u32(0x1F800004);
        w_u32(tail, 0x01000000);
        w_u32(tail + 4, flag | 0xE6000000);
        seed = 0x01000000;
    }
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
    if (lane != 3)
        value |= seed & (0xFFFFFF >> (lane << 3));
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
    root = r_u32(0x800BCF90);
    w_u32(0x1F800004, tail + 8);
    w_u32(root + 0x5C8, 0);
    return r_u32(0x1F800004);
}

uint32 sub_80095930(uint32 state, uint32 context)
{
    uint32 scene, settings, packed, color, height, distance, intensity, wide, page, a, b, c, d, e, angle, span, value, offset, packet, remaining, column, active, visible, x, y, u, v, next, original, result, early;
    FUNCTION_MARKER(0x80095930, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    settings = r_u32(scene + 0x268);
    packed = r_u32(settings + 4);
    color = r_u32(settings + 24);
    height = r_u32(state + 12);
    wide = packed & 1;
    distance = (uint32)((sint32)packed >> 16) - height;
    intensity = (sint32)(distance - 4000) >> 3;
    if ((sint32)intensity >= 4096)
        return 0;
    a = r_u32(scene + 0x4C);
    b = r_u32(scene + 0x50);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(scene + 0x54);
    d = r_u32(scene + 0x58);
    e = r_u32(scene + 0x5C);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    page = (sint32)color >> 24;
    page = page == 0 ? 0x39 : 0x59;
    xport_gte_write_data(8, intensity);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    color = (color & 0xFFFFFF) | 0x2E000000;
    xport_gte_write_data(6, color);
    xport_gte_execute(0x780010);
    value = r_s16(state + 26);
    span = (uint32)ratan2(200, value);
    value = r_u16(state + 22);
    angle = ((uint32)((sint32)(value << 16) >> 18) - span) & 0xF80;
    color = xport_gte_read_data(22);
    offset = wide != 0 ? 0xFFFFE800 : 0xFFFFF400;
    w_u16(context + 0x116, (sint32)(distance + offset) >> 2);
    w_u16(context + 0x11A, (sint32)(distance - offset) >> 2);
    value = r_u16(0x800B7104 + (angle << 2)) << 2;
    w_u16(context + 0x118, value);
    w_u16(context + 0x114, value);
    value = r_s16(0x800B7106 + (angle << 2)) << 2;
    a = r_u32(context + 0x114);
    xport_gte_write_data(0, a);
    a = r_u32(context + 0x118);
    xport_gte_write_data(2, a);
    xport_gte_write_data(1, value);
    xport_gte_write_data(3, value);
    xport_gte_write_data(4, 0);
    xport_gte_write_data(5, 0);
    active = 0;
    early = 0;
    packet = r_u32(state + 4);
    xport_gte_execute(0x280030);
    remaining = (uint32)((sint32)span >> 6) + 2;
    column = (uint32)((sint32)angle >> 7) - 1;
    value = xport_gte_read_data(12);
    w_u32(context + 0x11C, value);
    value = xport_gte_read_data(13);
    w_u32(context + 0x120, value);
    while ((sint32)remaining > 0)
    {
        angle = (angle + 128) & 0xFFF;
        value = r_u16(0x800B7104 + (angle << 2)) << 2;
        w_u16(context + 0x118, value);
        w_u16(context + 0x114, value);
        value = r_s16(0x800B7106 + (angle << 2)) << 2;
        a = r_u32(context + 0x114);
        xport_gte_write_data(0, a);
        a = r_u32(context + 0x118);
        xport_gte_write_data(2, a);
        xport_gte_write_data(1, value);
        xport_gte_write_data(3, value);
        next = column + 1;
        column = next & 15;
        xport_gte_execute(0x280030);
        value = r_u32(context + 0x11C);
        w_u32(packet + 8, value);
        b = r_u32(context + 0x120);
        u = (next & 3) << 6;
        w_u8(packet + 28, u);
        w_u8(packet + 12, u);
        w_u8(packet + 36, u + 63);
        w_u8(packet + 20, u + 63);
        w_u32(packet + 24, b);
        if (wide != 0)
        {
            value = (column << 4) & 64;
            v = value - 128;
            value -= 65;
        }
        else
        {
            value = (column << 3) & 96;
            v = value - 128;
            value -= 97;
        }
        w_u8(packet + 21, v);
        w_u8(packet + 13, v);
        w_u8(packet + 37, value);
        w_u8(packet + 29, value);
        value = xport_gte_read_data(12);
        w_u32(context + 0x11C, value);
        value = xport_gte_read_data(13);
        w_u32(context + 0x120, value);
        x = r_s16(context + 0x11C);
        y = r_s16(context + 0x120);
        if ((sint32)x < 0)
            x = 0 - x;
        if ((sint32)y < 0)
            y = 0 - y;
        visible = (sint32)(x + y) < 361;
        if (visible != 0)
        {
            x = r_s16(context + 0x11E);
            y = r_s16(context + 0x122);
            if ((sint32)x < 0)
                x = 0 - x;
            if ((sint32)y < 0)
                y = 0 - y;
            visible = (sint32)(x + y) < 281;
        }
        if (visible == 0)
        {
            if (active == 0)
            {
                original = r_u32(state + 4);
                if (packet != original)
                {
                    early = 1;
                    break;
                }
                --remaining;
                continue;
            }
            active = 0;
        }
        else
            active = 1;
        next = packet + 40;
        w_u32(packet, (next & 0xFFFFFF) | 0x09000000);
        w_u32(packet + 4, color);
        w_u16(packet + 14, 0x4ED6);
        w_u16(packet + 22, page);
        value = r_u32(context + 0x11C);
        w_u32(packet + 16, value);
        value = r_u32(context + 0x120);
        w_u32(packet + 32, value);
        packet = next;
        --remaining;
    }
    if (early == 0)
        original = r_u32(state + 4);
    result = 0x59;
    if (packet == original)
        return result;
    if (page == 0x59)
    {
        next = packet + 8;
        result = (next & 0xFFFFFF) | 0x01000000;
        w_u32(packet, result);
        w_u32(packet + 4, 0xE1000220);
        packet = next;
    }
    w_u32(state + 4, packet);
    return result;
}

uint32 sub_800943a8(uint32 ordering, uint32 packet, uint32 rectangle, uint32 offset)
{
    uint32 root, scene, value, indices, mask_command, index, bucket, prior_bucket, current, work, next, packed_x, packed_y, address, lane, shift, merge, i, base, settings, color, origin, size, x, y, right, bottom, draw, tag;
    FUNCTION_MARKER(0x800943A8, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    scene = r_u32(0x800BCE54);
    value = r_u32(root + 0x5C8);
    indices = r_u32(scene + 0x288);
    mask_command = (value & 4) != 0 ? 0xE6000001 : 0xE6000000;
    for (index = 0; index < 16; ++index, indices += 2)
    {
        value = r_s16(indices);
        current = packet;
        work = packet - 8;
        next = packet + 52;
        if ((index & 1) != 0)
        {
            tag = (next & 0xFFFFFF) | 0x0C000000;
            bucket = ordering + (value << 2);
            w_u32(packet, tag);
            w_u32(work + 60, 0x0C000000);
            address = bucket + 2;
            lane = address & 3;
            merge = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
            if (lane != 3)
                merge |= prior_bucket & (0xFFFFFF >> (lane << 3));
            address = next + 2;
            lane = address & 3;
            shift = 24 - (lane << 3);
            address &= 0xFFFFFFFC;
            for (i = 0; i <= lane; ++i)
                w_u8(address + i, merge >> (shift + (i << 3)));
            merge = packet << 8;
            address = bucket + 2;
            lane = address & 3;
            shift = 24 - (lane << 3);
            address &= 0xFFFFFFFC;
            for (i = 0; i <= lane; ++i)
                w_u8(address + i, merge >> (shift + (i << 3)));
            work += 104;
            packet += 104;
            packed_x = (index & 6) << 5;
            packed_y = (((index & 8) << 5) + 240) << 16;
            w_u32(work - 80, packed_x | packed_y);
            w_u32(work - 60, (packed_x + 16) | packed_y);
            w_u32(work - 28, (packed_x + 32) | packed_y);
            value = (packed_x + 48) | packed_y;
            w_u32(work - 40, 0xE6000000);
            w_u32(work - 92, 0xE6000000);
            w_u32(work + 4, mask_command);
            w_u32(work - 48, mask_command);
            w_u32(work - 16, 0x01000000);
            w_u32(work - 36, 0x01000000);
            w_u32(work - 68, 0x01000000);
            w_u32(work - 88, 0x01000000);
            w_u32(work - 12, 0x80000000);
            w_u32(work - 32, 0x80000000);
            w_u32(work - 64, 0x80000000);
            w_u32(work - 84, 0x80000000);
            w_u32(work - 8, value);
            w_u32(work - 76, 0x01000140);
            w_u32(work - 56, 0x01100140);
            w_u32(work - 24, 0x01200140);
            w_u32(work - 4, 0x01300140);
            w_u32(work, 0x100010);
            w_u32(work - 20, 0x100010);
            w_u32(work - 52, 0x100010);
            w_u32(work - 72, 0x100010);
        }
        else
        {
            work += 32;
            bucket = ordering + (value << 2);
            prior_bucket = bucket;
            packed_x = (index & 6) << 3;
            value = r_u32(bucket);
            w_u32(packet, value | 0x07000000);
            packet += 32;
            w_u32(bucket, current & 0xFFFFFF);
            packed_y = (((index & 8) << 3) + 384) << 16;
            value = (packed_x + 320) | packed_y;
            w_u32(work - 20, 0xE6000000);
            w_u32(work + 4, mask_command);
            w_u32(work - 16, 0x01000000);
            w_u32(work - 12, 0x80000000);
            w_u32(work - 8, value);
            w_u32(work - 4, 0x01000140);
            w_u32(work, 0x400010);
        }
    }
    base = packet;
    next = base + 20;
    draw = base + 44;
    w_u32(base + 12, 0xE5080140);
    w_u32(base + 8, 0xE404FD4F);
    scene = r_u32(0x800BCE54);
    w_u32(base, (next & 0xFFFFFF) | 0x04000000);
    w_u32(base + 4, 0xE3040140);
    w_u32(base + 16, 0);
    settings = r_u32(scene + 0x244);
    color = r_u32(settings + 8);
    w_u32(base + 20, (draw & 0xFFFFFF) | 0x05000000);
    w_u32(next + 4, 0xE6000001);
    w_u32(next + 12, 0);
    w_u32(next + 16, 0x400010);
    w_u32(next + 20, mask_command);
    w_u32(next + 8, (color & 0xFFFFFF) | 0x60000000);
    origin = r_u32(rectangle);
    w_u32(base + 44, 0x04000000);
    x = (uint32)(sint32)(sint16)origin;
    y = (sint32)origin >> 16;
    value = (x & 0x3FF) | ((y & 0x3FF) << 10) | 0xE3000000;
    size = r_u32(rectangle + 4);
    w_u32(draw + 4, value);
    right = x + (uint32)(sint32)(sint16)size - 1;
    bottom = y + (uint32)((sint32)size >> 16) - 1;
    packed_x = r_u32(offset);
    w_u32(draw + 8, (right & 0x3FF) | ((bottom & 0x3FF) << 10) | 0xE4000000);
    value = r_s16(indices);
    w_u32(draw + 16, 0);
    w_u32(draw + 12, (packed_x & 0x7FF) | ((uint32)((sint32)packed_x >> 5) & 0x3FF800) | 0xE5000000);
    bucket = ordering + (value << 2);
    tag = r_u32(base + 44);
    value = r_u32(bucket);
    w_u32(base + 44, (tag & 0xFF000000) | (value & 0xFFFFFF));
    value = r_u32(bucket);
    w_u32(bucket, (value & 0xFF000000) | (base & 0xFFFFFF));
    return base + 64;
}

uint32 sub_8008136c(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 dx, dy, dz, x, y, z, seed, random, root, record, value, result;
    FUNCTION_MARKER(0x8008136C, "ACE3.BIN");
    dx = (native_vector ? native_vector[0] : r_u32(vector));
    dy = (native_vector ? native_vector[1] : r_u32(vector + 4));
    dz = (native_vector ? native_vector[2] : r_u32(vector + 8));
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    x = (native_position ? native_position[0] : r_u32(position)) + ((random & 127) << 11);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    y = (native_position ? native_position[1] : r_u32(position + 4)) + ((random & 127) << 11);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    z = (native_position ? native_position[2] : r_u32(position + 8)) + ((random & 127) << 11);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        w_u32(record, 0x280000);
        w_u16(record + 24, 0x210);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u16(record + 26, (random & 63) + 128);
    }
    if ((random & 1) != 0)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record != 0)
        {
            w_u32(record, 0xCFFFF);
            w_u32(record + 4, x - dx);
            w_u32(record + 8, y - dy);
            w_u32(record + 12, z - dz);
            w_u16(record + 24, 256);
            w_u16(record + 22, 0);
            w_u16(record + 26, (((uint32)((sint32)random >> 3)) & 63) + 88);
        }
    }
    result = random & 3;
    if (result == 0)
        return 0;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    if (record == 0)
        return 0;
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = (uint32)((sint32)dx >> 8) - 128 + (random & 255);
    w_u16(record + 16, value);
    value = (uint32)((sint32)dy >> 8) - 128 + (((uint32)((sint32)random >> 3)) & 255);
    w_u16(record + 18, value);
    value = (uint32)((sint32)dz >> 8) - 128 + (((uint32)((sint32)random >> 6)) & 255);
    w_u16(record + 20, value);
    if ((random & 1) != 0)
    {
        w_u32(record, 0x120000);
        w_u16(record + 22, 0x8B);
        w_u16(record + 24, 0x1504);
        result = (random & 31) + 100;
    }
    else
    {
        w_u32(record, 0x1E0000);
        w_u16(record + 22, 0x18C);
        result = (random & 7) + 20;
        w_u16(record + 24, 0);
    }
    w_u16(record + 26, result);
    return result;
}

uint32 sub_80081760(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 dx, dy, dz, x, y, z, value, root, record, seed, random, rotated, lifetime, count, vx, vy, vz, i, result;
    FUNCTION_MARKER(0x80081760, "ACE3.BIN");
    dx = (native_vector ? native_vector[0] : r_u32(vector));
    dy = (native_vector ? native_vector[1] : r_u32(vector + 4));
    value = (native_position ? native_position[0] : r_u32(position));
    dz = (native_vector ? native_vector[2] : r_u32(vector + 8));
    x = value + dx;
    vx = (sint32)(0 - dx) >> 11;
    value = (native_position ? native_position[1] : r_u32(position + 4));
    y = value + dy;
    vy = (sint32)(0 - dy) >> 11;
    value = (native_position ? native_position[2] : r_u32(position + 8));
    root = r_u32(0x800BCF90);
    z = value + dz;
    vz = (sint32)(0 - dz) >> 11;
    result = 0;
    for (i = 0; i < 2; ++i)
    {
        if (i != 0)
            root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + (i == 0 ? 0x600 : 0x1098), i == 0 ? 3 : 10, 7);
        result = record;
        if (record == 0)
            continue;
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        count = (random & (i == 0 ? 31 : 15)) + (i == 0 ? 30 : 18);
        rotated = (uint32)((sint32)random >> 8) | (random << 7);
        w_u32(record + 4, x + (((random & 255) - 128) << 9));
        value = vx - (i == 0 ? 128 : 64) + (rotated & (i == 0 ? 255 : 127));
        w_u16(record + 16, value);
        w_u16(record + 18, vy - 256);
        value = vz - (i == 0 ? 128 : 64) + (((uint32)((sint32)rotated >> 7)) & (i == 0 ? 255 : 127));
        w_u16(record + 22, i == 0 ? 0x84 : 0x83);
        w_u16(record + 24, i == 0 ? 0x210 : 0x1604);
        lifetime = (rotated & (i == 0 ? 127 : 63)) + (i == 0 ? 120 : 100);
        w_u16(record, 0);
        w_u16(record + 2, count);
        w_u32(record + 8, y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 9));
        w_u32(record + 12, z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 9));
        w_u16(record + 20, value);
        w_u16(record + 26, lifetime);
        result = lifetime;
    }
    return result;
}

uint32 sub_80081994(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 dx, dy, dz, x, y, z, vx, vy, vz, value, root, record, seed, random, rotated, a, b, c, count, result;
    FUNCTION_MARKER(0x80081994, "ACE3.BIN");
    dx = (native_vector ? native_vector[0] : r_u32(vector));
    dy = (native_vector ? native_vector[1] : r_u32(vector + 4));
    value = (native_position ? native_position[0] : r_u32(position));
    dz = (native_vector ? native_vector[2] : r_u32(vector + 8));
    b = (native_position ? native_position[1] : r_u32(position + 4));
    x = value - dx;
    vx = (sint32)(0 - dx) >> 7;
    y = b - dy;
    vy = (sint32)(0 - dy) >> 7;
    value = (native_position ? native_position[2] : r_u32(position + 8));
    z = value - dz;
    vz = (sint32)(0 - dz) >> 7;
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    if (record != 0)
    {
        count = (random & 15) + 30;
        a = x + (((random & 255) - 128) << 8);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        random = (uint32)((sint32)random >> 8) | (random << 7);
        w_u32(record + 4, a);
        w_u32(record + 8, b);
        w_u16(record + 16, vx - 128 + (random & 255));
        w_u16(record + 18, vy - 192);
        value = vz - 128 + (((uint32)((sint32)random >> 7)) & 255);
        w_u16(record + 22, 0x86);
        w_u16(record, 0);
        w_u16(record + 2, count);
        w_u32(record + 12, c);
        w_u16(record + 20, value);
        w_u16(record + 24, 0x1804);
        w_u16(record + 26, (random & 255) + 200);
    }
    result = random & 1;
    if (result == 0)
        return 0;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record == 0)
        return 0;
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    count = (random & 31) + 50;
    a = x + (((random & 255) - 128) << 8);
    b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
    c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
    rotated = (uint32)((sint32)random >> 8) | (random << 7);
    w_u16(record, 0xFFFF);
    w_u32(record + 8, b);
    w_u16(record + 16, vx - 64 + (rotated & 127));
    w_u32(record + 4, a);
    w_u16(record + 18, vy - 192);
    value = vz - 64 + (((uint32)((sint32)rotated >> 7)) & 127);
    w_u16(record + 22, 0x87);
    w_u16(record + 2, count);
    w_u32(record + 12, c);
    w_u16(record + 20, value);
    w_u16(record + 24, 0x210);
    w_u16(record + 26, (rotated & 127) + 200);
    return 0x210;
}

uint32 sub_80081bc8(uint32 position, uint32 vector, uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 targets[4];
    FUNCTION_MARKER(0x80081BC8, "ACE3.BIN");
    targets[0] = r_u32(0x8002479C);
    targets[1] = r_u32(0x800247A0);
    targets[2] = r_u32(0x800247A4);
    targets[3] = r_u32(0x800247A8);
    if (kind >= 4)
        return 0;
    return xport_guest_call_vectors3(targets[kind], position, vector, kind, native_position, native_vector);
}

uint32 sub_80047b00(uint32 index)
{
    uint32 root, state, table, record, settings, object, limit, count, data, type, allowed, value, mode, mask, expected, position;
    FUNCTION_MARKER(0x80047B00, "ACE3.BIN");
    allowed = 0;
    root = r_u32(0x800BCEC0);
    state = root + index * 184;
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BCEF8);
    record = root + (index << 6);
    settings = r_u32(record + 32);
    object = r_u32(table + (index << 2));
    if (settings == 0)
        return 1;
    count = r_u32(state + 160);
    limit = r_u32(settings + 4);
    if (count >= limit)
    {
        w_u16(state + 92, 0);
        return 1;
    }
    data = r_u32(record + 24);
    type = r_u8(data);
    if (type == 0)
    {
        value = r_u16(state + 180);
        allowed = value - 0x801 < 0x17FF;
    }
    else if (type == 1)
    {
        value = r_u16(state + 180);
        allowed = value - 0x101 < 0xEFF;
    }
    else if (type == 2 || type == 3)
    {
        value = r_s16(state + 180);
        allowed = (sint32)value < 0x1000;
    }
    mode = r_u16(settings);
    switch (mode)
    {
        case 1:
        case 4:
            mask = 0x41;
            expected = 0x40;
            break;
        case 2:
            mask = 0x43;
            expected = 0x40;
            break;
        case 3:
            mask = 0x27;
            expected = 0x20;
            break;
        case 6:
            mask = 0x13;
            expected = 0x10;
            break;
        case 5:
            value = r_u32(state + 92);
            if ((value & 0x50) == 0x50)
            {
                value = r_u16(state + 92);
                if ((value & 1) != 0)
                    ++allowed;
                position = object + 8;
                sub_800831d0(position, object + 24, (value & 15) << 6, 0);
                value = r_u16(state + 92);
                if ((value & 15) == 0)
                    sub_8002f3c0(13, position, 0, 0);
            }
            break;
        default:
            break;
    }
    if (mode >= 1 && mode <= 6 && mode != 5)
    {
        value = r_u32(state + 92);
        if ((value & mask) == expected)
            ++allowed;
    }
    if (allowed == 2)
    {
        mode = r_u16(settings);
        value = r_u32(object);
        sub_8006b034(value, object + 8, object + 56, mode - 1);
    }
    value = r_u16(state + 92);
    w_u16(state + 92, value + 1);
    return 0;
}

uint32 sub_80034718(void)
{
    uint32 scene, root, value, selected, current, flags, result;
    FUNCTION_MARKER(0x80034718, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    value = r_u32(scene);
    root = r_u32(0x800BCE60);
    w_u32(scene, value | 0x40);
    flags = r_u32(root + 0x130);
    selected = scene + 0x29C;
    if ((flags & 1) != 0)
    {
        value = r_u16(selected);
        w_u16(selected, value - 1);
    }
    root = r_u32(0x800BCE60);
    flags = r_u32(root + 0x130);
    if ((flags & 2) != 0)
    {
        value = r_u16(selected);
        w_u16(selected, value + 1);
    }
    value = r_s16(selected);
    if ((sint32)value < 0)
    {
        w_u16(selected, 4);
        value = r_s16(selected);
    }
    if ((sint32)value >= 5)
        w_u16(selected, 0);
    current = r_s16(selected);
    if (current == 1)
    {
        root = r_u32(0x800BCE60);
        flags = r_u32(root + 0x130);
        if ((flags & 12) != 0)
        {
            value = r_u8(root + 0x39D);
            w_u8(root + 0x39D, value == 0);
        }
    }
    current = r_s16(selected);
    if (current == 2)
    {
        root = r_u32(0x800BCE60);
        flags = r_u32(root + 0x130);
        if ((flags & 12) != 0)
        {
            value = r_u8(root + 0x39E);
            w_u8(root + 0x39E, value == 0);
            root = r_u32(0x800BCE60);
            value = r_u8(root + 0x39E);
            sub_80035740(value == 0);
        }
    }
    root = r_u32(0x800BCE60);
    flags = r_u32(root + 0x12C);
    if ((flags & 0x40) != 0)
        w_u16(selected, 0);
    root = r_u32(0x800BCE60);
    flags = r_u32(root + 0x12C);
    if ((flags & 0x10) == 0)
        return 3;
    current = r_s16(selected);
    if (current == 3)
    {
        value = r_u32(root);
        scene = r_u32(0x800BCE54);
        w_u32(root, value | 0x08000000);
        result = r_u32(scene) | 0x40000000;
        w_u32(scene, result);
        return result;
    }
    if ((sint32)current < 4)
    {
        if (current == 0)
        {
            result = r_u32(root) | 0x08000000;
            w_u32(root, result);
            return result;
        }
        return 1;
    }
    if (current != 4)
        return 4;
    value = r_u32(root);
    scene = r_u32(0x800BCE54);
    w_u32(root, value | 0x08000000);
    result = r_u32(scene) | 0x80000000;
    w_u32(scene, result);
    return result;
}

uint32 sub_800860a4(uint32 state, uint32 geometry, uint32 frame)
{
    uint32 packed, packet, divisor, low, numerator, ratio, value, wave, color, size, width, height, product, high, shift, coordinate, flip, u, v, bucket, table, result;
    FUNCTION_MARKER(0x800860A4, "ACE3.BIN");
    packed = r_u32(frame);
    packet = r_u32(state + 4);
    divisor = (sint32)packed >> 16;
    low = (uint32)(sint32)(sint16)packed;
    w_u16(state + 32, (low + divisor) & 2);
    w_u16(state + 34, ((divisor & 1) << 6) + 128);
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
        value = r_s16(0x800B7106 + ((ratio << 1) & 0x3FFC));
        wave = 4096 - value;
    }
    else
        wave = r_s16(0x800B7106 + (((ratio - 2048) << 1) & 0x3FFC));
    value = 0x1200 - (uint32)((sint32)wave >> 1);
    color = r_u32(state + 96);
    xport_gte_write_data(8, value);
    xport_gte_write_data(6, color);
    xport_gte_write_control(21, (color & 255) << 2);
    color = (sint32)color >> 8;
    xport_gte_write_control(22, (color & 255) << 2);
    xport_gte_write_control(23, (uint32)((sint32)color >> 6) & 0x3FC);
    size = r_s16(geometry + 6);
    if ((sint32)ratio < 2048)
    {
        product = size * (ratio + 0x2AA);
        high = (uint32)(((sint64)(sint32)product * (sint64)(sint32)0xC00C00C1) >> 32);
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
    w_u16(state + 20, width);
    shift = (sint32)height < 17;
    w_u16(state + 22, height);
    coordinate = r_u32(geometry);
    value = (uint32)((sint32)product >> 15) - height;
    value = (sint32)value >> 1;
    w_u16(state + 26, value);
    w_u16(state + 24, 0);
    w_u32(state + 28, coordinate);
    w_u16(packet + 14, 0x40D6);
    w_u16(packet + 22, 0x37);
    value = (64 >> shift) - 1;
    w_u8(packet + 21, 0);
    w_u8(packet + 13, 0);
    w_u8(packet + 37, value);
    w_u8(packet + 29, value);
    flip = r_s16(state + 32);
    u = r_s16(state + 34);
    if (flip != 0)
    {
        value = (sint32)u >> shift;
        w_u8(packet + 28, value);
        w_u8(packet + 12, value);
        v = (uint32)((sint32)(u + 64) >> shift) - 1;
    }
    else
    {
        value = (uint32)((sint32)(u + 64) >> shift) - 1;
        w_u8(packet + 28, value);
        w_u8(packet + 12, value);
        v = (sint32)u >> shift;
    }
    w_u8(packet + 36, v);
    w_u8(packet + 20, v);
    value = xport_gte_read_data(22);
    w_u32(packet + 4, value);
    sub_8008591c(state, packet);
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    value = r_u32(table + (bucket << 2));
    w_u32(packet, value | 0x09000000);
    bucket = r_s16(geometry + 4);
    table = r_u32(state);
    w_u32(table + (bucket << 2), packet & 0xFFFFFF);
    result = packet + 40;
    w_u32(state + 4, result);
    return result;
}
