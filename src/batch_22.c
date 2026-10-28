#include "game_functions.h"

#include <stdlib.h>

uint32 resource002_sub_800f6774(uint32 packet, uint32 mode)
{
    uint32 owner, ordering, tag, link;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x800F6774, "ACE3.BIN");
    if (mode == 0)
    {
        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = 320;
        rectangle.h = 480;
    }
    else if (mode == 1)
    {
        rectangle.x = 20;
        rectangle.y = 70;
        rectangle.w = 128;
        rectangle.h = 240;
    }
    else
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 draw-area mode has no initialized original rectangle: %u\n", mode);
#endif
        abort();
    }
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
    owner = r_u32(0x80112DC4);
    ordering = r_u32(owner + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(owner + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 12;
}

uint32 resource002_sub_800f4c04(uint32 packet)
{
    uint32 state, profile, point, x, y, left, right, delta, top, bottom;
    uint32 owner, ordering, tag, link, second;
    FUNCTION_MARKER(0x800F4C04, "ACE3.BIN");
    state = r_u32(0x80112E04);
    point = (uint32)(r_s16(state + 2) / 4) * 2;
    profile = r_u32(0x80112E08);
    point += profile;
    x = r_u8(point + 12);
    left = 84 - x;
    right = 340 - x;
    y = r_u8(point + 13);
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 44);
    w_u8(packet + 12, 0);
    w_u8(packet + 13, 0);
    w_u8(packet + 20, 128);
    w_u8(packet + 21, 0);
    w_u8(packet + 28, 0);
    w_u8(packet + 29, 64);
    w_u8(packet + 36, 128);
    w_u8(packet + 37, 64);
    w_u8(packet + 4, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 6, 128);
    owner = r_u32(0x80112DC4);
    w_u16(packet + 22, 31);
    w_u16(packet + 14, 0x603C);
    w_u16(packet + 8, left);
    w_u16(packet + 16, right);
    w_u16(packet + 24, left);
    w_u16(packet + 32, right);
    delta = (255 - y) * 2;
    top = 190 - delta;
    bottom = 446 - delta;
    w_u16(packet + 10, top);
    w_u16(packet + 18, top);
    w_u16(packet + 26, bottom);
    w_u16(packet + 34, bottom);
    ordering = r_u32(owner + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(owner + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    second = packet + 40;
    w_u8(second + 3, 9);
    w_u8(second + 7, 44);
    w_u8(second + 12, 0);
    w_u8(second + 13, 64);
    w_u8(second + 20, 128);
    w_u8(second + 21, 64);
    w_u8(second + 28, 0);
    w_u8(second + 29, 128);
    w_u8(second + 36, 128);
    w_u8(second + 37, 128);
    w_u8(second + 4, 128);
    w_u8(second + 5, 128);
    w_u8(second + 6, 128);
    owner = r_u32(0x80112DC4);
    w_u16(second + 22, 31);
    w_u16(second + 14, 0x603C);
    w_u16(second + 8, left);
    w_u16(second + 10, bottom);
    w_u16(second + 16, right);
    w_u16(second + 18, bottom);
    w_u16(second + 24, left);
    w_u16(second + 26, 702 - delta);
    w_u16(second + 32, right);
    w_u16(second + 34, 702 - delta);
    ordering = r_u32(owner + 8);
    tag = r_u32(second);
    link = r_u32(ordering);
    w_u32(second, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(owner + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (second & 0xFFFFFF));
    return second + 40;
}

uint32 resource002_sub_800f6cbc(uint32 config, uint32 packet, uint32 ordering, const uint8 *native_config)
{
    uint32 display, dither, depth, blend, x, y, page, tag, link;
    FUNCTION_MARKER(0x800F6CBC, "ACE3.BIN");
    display = native_config ? native_config[0] : r_u8(config);
    dither = native_config ? native_config[1] : r_u8(config + 1);
    depth = native_config ? native_config[2] : r_u8(config + 2);
    blend = native_config ? native_config[3] : r_u8(config + 3);
    y = native_config ? xport_load_le16(native_config + 6) : r_u16(config + 6);
    x = native_config ? xport_load_le16(native_config + 4) : r_u16(config + 4);
    page = ((depth & 3) << 7) | ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    sub_800ac7f8(packet, display, dither, page, 0);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 12;
}

uint32 resource002_sub_800f6da4(uint32 config, uint32 packet, uint32 ordering, const uint8 *native_config)
{
    uint32 value, tag, link;
    FUNCTION_MARKER(0x800F6DA4, "ACE3.BIN");
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 64);
    value = native_config ? native_config[11] : r_u8(config + 11);
    w_u8(packet + 7, value ? 66 : 64);
    value = native_config ? native_config[8] : r_u8(config + 8);
    w_u8(packet + 4, value);
    value = native_config ? native_config[9] : r_u8(config + 9);
    w_u8(packet + 5, value);
    value = native_config ? native_config[10] : r_u8(config + 10);
    w_u8(packet + 6, value);
    value = native_config ? xport_load_le16(native_config) : r_u16(config);
    w_u16(packet + 8, value);
    value = native_config ? xport_load_le16(native_config + 2) : r_u16(config + 2);
    w_u16(packet + 10, value);
    value = native_config ? xport_load_le16(native_config + 4) : r_u16(config + 4);
    w_u16(packet + 12, value);
    value = native_config ? xport_load_le16(native_config + 6) : r_u16(config + 6);
    tag = r_u32(packet);
    w_u16(packet + 14, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 16;
}

uint32 resource002_sub_800f5388(uint32 packet)
{
    uint32 state, profile, point, origin_x, origin_y, i, item, x, y, owner, ordering, type, j;
    sint32 color_initialized = 0;
    uint8 mode[8], rectangle[12];
    FUNCTION_MARKER(0x800F5388, "ACE3.BIN");
    state = r_u32(0x80112E04);
    point = (uint32)(r_s16(state + 2) / 4) * 2;
    profile = r_u32(0x80112E08);
    point += profile;
    origin_x = r_u8(point + 12);
    origin_y = r_u8(point + 13);
    for (i = 0; i < 64; ++i)
    {
        state = r_u32(0x80112E04);
        item = state + i * 4 + 16;
        if (r_u8(item + 3) == 0)
            return packet;
        x = r_u8(item);
        y = r_u8(item + 1);
        owner = r_u32(0x80112DC4);
        for (j = 0; j < sizeof(mode); ++j)
            mode[j] = 0;
        x -= origin_x;
        ordering = r_u32(owner + 8);
        y = (origin_y - y) * 2;
        packet = resource002_sub_800f6cbc(0, packet, ordering, mode);
        type = r_u8(item + 2);
        if (type == 0)
        {
            rectangle[8] = 255;
            rectangle[9] = 255;
            rectangle[10] = 255;
            color_initialized = 1;
        }
        type = r_u8(item + 2);
        if (type == 1)
        {
            rectangle[8] = 0;
            rectangle[9] = 0;
            rectangle[10] = 255;
            color_initialized = 1;
        }
        type = r_u8(item + 2);
        if (type == 2)
        {
            rectangle[8] = 0;
            rectangle[9] = 0;
            rectangle[10] = 0;
            color_initialized = 1;
            type = r_u8(item + 2);
        }
        if (type == 3)
        {
            rectangle[8] = 255;
            rectangle[9] = 255;
            rectangle[10] = 0;
            color_initialized = 1;
        }
        type = r_u8(item + 2);
        if (type == 4)
        {
            rectangle[8] = 255;
            rectangle[9] = 0;
            rectangle[10] = 0;
            color_initialized = 1;
        }
        if (!color_initialized)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource2 graph color has no initialized original RGB: %u\n", type);
#endif
            abort();
        }
        owner = r_u32(0x80112DC4);
        rectangle[11] = 1;
        xport_store_le16(rectangle, x + 80);
        xport_store_le16(rectangle + 2, y + 184);
        xport_store_le16(rectangle + 4, x + 88);
        xport_store_le16(rectangle + 6, y + 200);
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        owner = r_u32(0x80112DC4);
        xport_store_le16(rectangle, x + 78);
        xport_store_le16(rectangle + 4, x + 86);
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        xport_store_le16(rectangle, x + 79);
        xport_store_le16(rectangle + 4, x + 87);
        rectangle[8] *= 2;
        rectangle[9] *= 2;
        owner = r_u32(0x80112DC4);
        rectangle[10] *= 2;
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        rectangle[11] = 1;
        xport_store_le16(rectangle, x + 80);
        xport_store_le16(rectangle + 2, y + 200);
        xport_store_le16(rectangle + 4, x + 88);
        xport_store_le16(rectangle + 6, y + 184);
        rectangle[8] >>= 1;
        rectangle[9] >>= 1;
        owner = r_u32(0x80112DC4);
        rectangle[10] >>= 1;
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        owner = r_u32(0x80112DC4);
        xport_store_le16(rectangle, x + 78);
        xport_store_le16(rectangle + 4, x + 86);
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        rectangle[11] = 1;
        xport_store_le16(rectangle, x + 79);
        xport_store_le16(rectangle + 4, x + 87);
        rectangle[8] *= 2;
        rectangle[9] *= 2;
        owner = r_u32(0x80112DC4);
        rectangle[10] *= 2;
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6da4(0, packet, ordering, rectangle);
        owner = r_u32(0x80112DC4);
        mode[3] = 0;
        ordering = r_u32(owner + 8);
        packet = resource002_sub_800f6cbc(0, packet, ordering, mode);
    }
    return packet;
}

uint32 resource002_sub_800f4928(uint32 x, uint32 y, uint32 text, const uint8 *native_text, uint32 native_capacity)
{
    uint32 state, index, character, step, reset, current, value;
    sint32 count, scale;
    FUNCTION_MARKER(0x800F4928, "ACE3.BIN");
    (void)x;
    (void)y;
    state = r_u32(0x80112E00) + 0x8000;
    count = r_s32(state + 0x28D0);
    if (count <= 0)
        return (uint32)count;
    index = 0;
    do
    {
        if (native_text && index >= native_capacity)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource2 text length exceeds native buffer\n");
#endif
            abort();
        }
        character = native_text ? native_text[index] : r_u8(text + index);
        if (character == 10)
        {
            state = r_u32(0x80112E00) + 0x8000;
            step = r_u8(state + 0x2901);
            scale = r_s16(state + 0x28F2);
            reset = r_u16(state + 0x28E8);
            current = r_u16(state + 0x28E6);
            w_u16(state + 0x28E4, reset);
            w_u16(state + 0x28E6, current + (uint32)((sint32)step * scale));
        }
        else if (character == 9)
        {
            state = r_u32(0x80112E00) + 0x8000;
            reset = r_u16(state + 0x28E8);
            w_u16(state + 0x28E4, reset);
        }
        else if (character == 32)
        {
            state = r_u32(0x80112E00) + 0x8000;
            value = r_u8(state + 0x2903);
            if (value)
            {
                step = r_u8(state + 0x290A);
                current = r_u16(state + 0x28E4);
                w_u16(state + 0x28E4, current + step);
            }
            else
            {
                step = r_u8(state + 0x2900);
                current = r_u16(state + 0x28E4);
                w_u16(state + 0x28E4, current + step + 1);
            }
        }
        else
        {
            character = native_text ? native_text[index] : r_u8(text + index);
            resource002_sub_800f4124(character);
        }
        state = r_u32(0x80112E00) + 0x8000;
        count = r_s32(state + 0x28D0);
        ++index;
    } while ((sint32)index < count);
    return 0;
}

uint32 resource002_sub_800f4af0(uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length, state, x, y;
    FUNCTION_MARKER(0x800F4AF0, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, arguments, native_arguments, native_argument_bytes);
    state = r_u32(0x80112E00) + 0x8000;
    x = (uint32)r_s16(state + 0x28E4);
    y = (uint32)r_s16(state + 0x28E6);
    w_u32(state + 0x28D0, length);
    return resource002_sub_800f4928(x, y, 0, buffer, sizeof(buffer));
}

uint32 resource002_sub_800f4ae0(uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    FUNCTION_MARKER(0x800F4AE0, "ACE3.BIN");
    return resource002_sub_800f4af0(format, arguments, native_arguments, native_argument_bytes);
}

uint32 resource002_sub_800f4b50(uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length, state, index, x, y;
    sint32 count;
    FUNCTION_MARKER(0x800F4B50, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, arguments, native_arguments, native_argument_bytes);
    state = r_u32(0x80112E00) + 0x8000;
    w_u32(state + 0x28D0, length);
    if ((sint32)length > 0)
    {
        index = 0;
        do
        {
            if (index >= sizeof(buffer))
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource2 zero-padding length exceeds native buffer\n");
#endif
                abort();
            }
            if (buffer[index] == 32)
                buffer[index] = 48;
            state = r_u32(0x80112E00) + 0x8000;
            count = r_s32(state + 0x28D0);
            ++index;
        } while ((sint32)index < count);
    }
    state = r_u32(0x80112E00) + 0x8000;
    x = (uint32)r_s16(state + 0x28E4);
    y = (uint32)r_s16(state + 0x28E6);
    return resource002_sub_800f4928(x, y, 0, buffer, sizeof(buffer));
}

uint32 resource002_sub_800f4b40(uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    FUNCTION_MARKER(0x800F4B40, "ACE3.BIN");
    return resource002_sub_800f4b50(format, arguments, native_arguments, native_argument_bytes);
}

static sint32 resource002_glyph_divide(uint32 numerator, sint32 denominator)
{
    if (denominator == 0 || (numerator == 0x80000000 && denominator == -1))
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 glyph original division BREAK: %08X/%d\n", numerator, denominator);
#endif
        abort();
    }
    return (sint32)numerator / denominator;
}

uint32 resource002_sub_800f4124(uint32 character)
{
    uint32 base, state, code, index, packet, width, height, raw_u, raw_v;
    uint32 palette, page, right, bottom, red, green, blue, flag, product;
    uint32 numerator, fraction, other_fraction, packed, x, y, ordering, value;
    sint32 columns, quotient, u, v, scale, current, start, end, denominator, half;
    FUNCTION_MARKER(0x800F4124, "ACE3.BIN");
    base = r_u32(0x80112E00);
    state = base + 0x8000;
    code = r_u8(state + 0x2902);
    columns = r_s16(state + 0x28F0);
    index = (character - code) & 0xFF;
    quotient = resource002_glyph_divide(index, columns);
    raw_u = index - (uint32)quotient * (uint32)columns;
    width = r_u8(state + 0x2900);
    raw_u *= width;
    height = r_u8(state + 0x2901);
    product = (uint32)quotient * height;
    value = r_u16(state + 0x28F8);
    packet = r_u32(base + 24000);
    raw_v = r_u16(state + 0x28FA);
    u = (sint16)(value + raw_u);
    palette = (uint32)r_s16(state + 0x28EE);
    right = (uint32)u - 1;
    v = (sint16)(raw_v + product);
    packed = (uint32)v << 8;
    w_u32(packet + 12, (uint32)u | packed | (palette << 16));
    width = r_u8(state + 0x2900);
    page = (uint32)r_s16(state + 0x28EC);
    w_u32(packet + 20, (right + width) | packed | (page << 16));
    height = r_u8(state + 0x2901);
    bottom = (uint32)v - 1;
    w_u32(packet + 28, (uint32)u | ((bottom + height) << 8));
    width = r_u8(state + 0x2900);
    height = r_u8(state + 0x2901);
    w_u32(packet + 36, (right + width) | ((bottom + height) << 8));
    green = r_u8(state + 0x28CD);
    red = r_u8(state + 0x28CC);
    blue = r_u8(state + 0x28CE);
    w_u32(packet + 4, red | (green << 8) | (blue << 16));
    flag = r_u8(state + 0x290B);
    if (flag == 1)
    {
        current = r_s16(state + 0x2910);
        start = r_s16(state + 0x290C);
        if (current < start)
            return 0;
        end = r_s16(state + 0x290E);
        if (current < end)
        {
            height = r_u8(state + 0x2901);
            scale = r_s16(state + 0x28F2);
            product = height * (uint32)scale;
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            denominator = end - start;
            fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            y = (uint32)r_s16(state + 0x28E6) + fraction;
            x = r_u16(state + 0x28E4);
            w_u32(packet + 8, (y << 16) | x);
            height = r_u8(state + 0x2901);
            scale = r_s16(state + 0x28F2);
            product = height * (uint32)scale;
            current = r_s16(state + 0x2910);
            half = (sint32)product / 2;
            end = r_s16(state + 0x290E);
            numerator = (uint32)half * (uint32)(end - current);
            start = r_s16(state + 0x290C);
            denominator = end - start;
            fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            x = (uint32)r_s16(state + 0x28E4) - 1;
            y = (uint32)r_s16(state + 0x28E6) + fraction;
            width = r_u8(state + 0x2900);
            w_u32(packet + 16, (y << 16) | ((x + width) & 0xFFFF));
            height = r_u8(state + 0x2901);
            scale = r_s16(state + 0x28F2);
            product = height * (uint32)scale;
            current = r_s16(state + 0x2910);
            half = (sint32)product / 2;
            end = r_s16(state + 0x290E);
            numerator = (uint32)half * (uint32)(end - current);
            start = r_s16(state + 0x290C);
            denominator = end - start;
            fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            numerator = (height - 1) * (uint32)scale * (uint32)(current - start);
            other_fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            y = (uint32)r_s16(state + 0x28E6) + fraction + other_fraction;
            x = r_u16(state + 0x28E4);
            w_u32(packet + 24, (y << 16) | x);
            height = r_u8(state + 0x2901);
            scale = r_s16(state + 0x28F2);
            product = height * (uint32)scale;
            end = r_s16(state + 0x290E);
            current = r_s16(state + 0x2910);
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            start = r_s16(state + 0x290C);
            denominator = end - start;
            fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            numerator = (height - 1) * (uint32)scale * (uint32)(current - start);
            other_fraction = (uint32)resource002_glyph_divide(numerator, denominator);
            x = (uint32)r_s16(state + 0x28E4) - 1;
            y = (uint32)r_s16(state + 0x28E6) + fraction + other_fraction;
            width = r_u8(state + 0x2900);
            packed = (y << 16) | ((x + width) & 0xFFFF);
            goto write_last_vertex;
        }
    }
    y = (uint32)r_s16(state + 0x28E6);
    x = r_u16(state + 0x28E4);
    w_u32(packet + 8, (y << 16) | x);
    y = (uint32)r_s16(state + 0x28E6);
    x = (uint32)r_s16(state + 0x28E4);
    width = r_u8(state + 0x2900);
    w_u32(packet + 16, (y << 16) | ((x - 1 + width) & 0xFFFF));
    height = r_u8(state + 0x2901);
    scale = r_s16(state + 0x28F2);
    product = (height - 1) * (uint32)scale;
    y = (uint32)r_s16(state + 0x28E6);
    x = r_u16(state + 0x28E4);
    w_u32(packet + 24, ((y + product) << 16) | x);
    height = r_u8(state + 0x2901);
    scale = r_s16(state + 0x28F2);
    product = (height - 1) * (uint32)scale;
    width = r_u8(state + 0x2900);
    x = (uint32)r_s16(state + 0x28E4);
    y = (uint32)r_s16(state + 0x28E6);
    packed = ((y + product) << 16) | ((x - 1 + width) & 0xFFFF);
write_last_vertex:
    w_u32(packet + 32, packed);
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 44);
    value = r_u32(0x80112E00) + 0x8000;
    flag = r_u8(value + 0x2908);
    w_u8(packet + 7, flag ? 46 : 44);
    value = r_u32(0x80112E00) + 0x8000;
    ordering = r_u32(value + 0x28DC);
    packed = r_u32(ordering);
    w_u32(packet, packed | 0x09000000);
    ordering = r_u32(value + 0x28DC);
    w_u32(ordering, packet & 0xFFFFFF);
    value = r_u32(0x80112E00) + 0x8000;
    flag = r_u8(value + 0x2903);
    if (flag)
    {
        code = r_u32(value + 0x2904);
        width = r_u8(code + index);
        x = r_u16(value + 0x28E4);
        w_u16(value + 0x28E4, x + width);
    }
    else
    {
        width = r_u8(value + 0x2900);
        x = r_u16(value + 0x28E4);
        w_u16(value + 0x28E4, x + width + 1);
    }
    base = r_u32(0x80112E00);
    packet = r_u32(base + 24000) + 40;
    w_u32(base + 24000, packet);
    return packet;
}

uint32 resource002_sub_800f56d8(void)
{
    const uint8 empty_arguments[1] = {0};
    uint8 arguments[4];
    uint32 i, state, count, label_y, number_y;
    FUNCTION_MARKER(0x800F56D8, "ACE3.BIN");
    label_y = 54;
    number_y = 59;
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x80112E04);
        if (r_u8(state + i * 44 + 313) == 0)
            break;
        resource002_sub_800f33a8(178, label_y);
        label_y += 22;
        state = r_u32(0x80112E04);
        resource002_sub_800f4ae0(state + i * 44 + 272, 0, empty_arguments, 0);
        resource002_sub_800f33a8(270, number_y);
        number_y += 22;
        state = r_u32(0x80112E04);
        count = r_u8(state + i * 44 + 313);
        xport_store_le32(arguments, count);
        resource002_sub_800f4b40(0x800CD14C, 0, arguments, sizeof(arguments));
    }
    label_y = 216;
    number_y = 223;
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x80112E04);
        if (r_u8(state + i * 44 + 665) == 0)
            break;
        resource002_sub_800f33a8(178, label_y);
        label_y += 22;
        state = r_u32(0x80112E04);
        resource002_sub_800f4ae0(state + i * 44 + 624, 0, empty_arguments, 0);
        resource002_sub_800f33a8(270, number_y);
        number_y += 22;
        state = r_u32(0x80112E04);
        count = r_u8(state + i * 44 + 665);
        xport_store_le32(arguments, count);
        resource002_sub_800f4b40(0x800CD14C, 0, arguments, sizeof(arguments));
    }
    return 0;
}

uint32 resource002_sub_800f5844(uint32 state, uint32 game)
{
    uint8 arguments[12];
    uint32 enabled, time, fraction;
    sint32 elapsed, random_value, minutes;
    FUNCTION_MARKER(0x800F5844, "ACE3.BIN");
    enabled = r_u8(state);
    time = r_u16(game + 954);
    if (enabled)
    {
        elapsed = r_s16(state + 2);
        if (elapsed < (sint32)(time / 30))
        {
            random_value = (sint32)game_random();
            fraction = (uint32)(random_value % 30 + 30);
        }
        else
            fraction = (time % 30) * 100 / 30;
    }
    else
        fraction = 0;
    resource002_sub_800f33a8(30, 339);
    state = r_u32(0x80112E04);
    elapsed = (sint16)r_u16(state + 2);
    minutes = elapsed / 60;
    xport_store_le32(arguments, (uint32)(sint32)(sint16)((sint16)minutes % 60));
    xport_store_le32(arguments + 4, (uint32)(sint32)(sint16)(elapsed - minutes * 60));
    xport_store_le32(arguments + 8, fraction & 0xFF);
    return resource002_sub_800f4b40(0x800CD154, 0, arguments, sizeof(arguments));
}

uint32 resource002_sub_800f5834(void)
{
    uint32 state, game;
    FUNCTION_MARKER(0x800F5834, "ACE3.BIN");
    state = r_u32(0x80112E04);
    game = r_u32(0x800BCE60);
    return resource002_sub_800f5844(state, game);
}

uint32 resource002_debrief_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800FB6E8:
            return resource002_sub_800fb6e8();
        case 0x800FB7A4:
            return resource002_sub_800fb7a4();
        case 0x800FB7F4:
            return resource002_sub_800fb7f4();
        case 0x800FB948:
            return resource002_sub_800fb948();
        case 0x800FB9C4:
            return resource002_sub_800fb9c4();
        case 0x800FBA30:
            return resource002_sub_800fba30();
        case 0x800FBBE0:
            return resource002_sub_800fbbe0();
        case 0x800FBC0C:
            return resource002_sub_800fbc0c();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 debrief phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800f6848(void)
{
    const uint8 empty_arguments[1] = {0};
    uint32 game, state, bank, packet, ordering, tag, link, value, flags;
    uint32 target, result, record, available, phase, table, kind;
    sint32 mode, mission;
    FUNCTION_MARKER(0x800F6848, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    packet = r_u32(state + bank * 4);
    w_u32(state + 8, packet);
    ClearOTagR(psx_addr(packet, 64), 16);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    packet = r_u32(state + 8);
    ordering = r_u32(game + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    packet = r_u32(state + 8) + 60;
    ordering = r_u32(game + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    bank = r_u32(game + 4);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 628, value);
    flags = r_u32(game + 2412);
    if (flags & 1)
        return 0x04000000;
    if (flags & 8)
        return 0x04000000;
    if (r_u8(game + 2399) == 1)
        return 0x04000000;
    mode = r_s16(game + 2388);
    if (mode != 1 && mode != 3)
        return 0x04000000;
    game = r_u32(0x800BCE60);
    if (r_u8(game + 2391) == 1)
        return 0x04000000;
    resource002_sub_800f33d0(5, 50, 53);
    state = r_u32(0x80112DC4);
    resource002_sub_800f3db4(r_u32(state + 8));
    state = r_u32(0x80112DC4);
    mode = r_s16(state + 454);
    target = r_u32(0x80111710 + (uint32)mode * 4);
    result = resource002_debrief_phase_call(target);
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    kind = r_u8(game + 2390);
    table = 0x8010E674 + ((uint32)mission * 4 + kind) * 2;
    available = r_u8(table);
    if (result - 1 < 2)
    {
        state = r_u32(0x80112DC4);
        phase = r_u16(state + 454) + 1;
        w_u16(state + 454, phase);
        if ((sint16)phase == 7 && available)
        {
            mission = r_s16(game + 908);
            kind = r_u8(game + 2390);
            table = 0x8010E674 + ((uint32)mission * 4 + kind) * 2;
            available = r_u8(table);
            resource002_sub_800f7524(1, available);
            state = r_u32(0x80112DC4);
            game = r_u32(0x800BCE60);
            w_u32(state + 616, 1);
            mission = r_s16(game + 908);
            kind = r_u8(game + 2390);
            table = 0x8010E674 + ((uint32)mission * 4 + kind) * 2;
            if (r_u8(table + 1) == 0)
            {
                record = r_u32(state + 32);
                resource002_sub_800ecec8(r_u32(record + 16));
            }
            game = r_u32(0x800BCE60);
            mission = r_s16(game + 908);
            kind = r_u8(game + 2390);
            table = 0x8010E674 + ((uint32)mission * 4 + kind) * 2;
            if (r_u8(table + 1) == 1)
            {
                state = r_u32(0x80112DC4);
                record = r_u32(state + 32);
                resource002_sub_800ecec8(r_u32(record + 20));
            }
        }
    }
    state = r_u32(0x80112DC4);
    value = r_u32(state + 616);
    if (value == 1)
    {
        if (sub_8002b59c())
        {
            sub_8002cc44(3, 0, 0);
            state = r_u32(0x80112DC4);
            w_u32(state + 616, 2);
        }
    }
    else if (value == 2)
    {
        ordering = r_u32(state + 8);
        packet = r_u32(state + 628);
        value = sub_8002d3f4(ordering, packet);
        state = r_u32(0x80112DC4);
        w_u32(state + 628, value);
    }
    state = r_u32(0x80112DC4);
    value = resource002_sub_800e2494(r_u32(state + 628));
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    resource002_sub_800f5018();
    resource002_sub_800f56d8();
    resource002_sub_800f5834();
    state = r_u32(0x80112DC4);
    value = resource002_sub_800f6774(r_u32(state + 628), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800f5388(value);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800f4df0(value);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800f4c04(value);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800f6774(value, 1);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    resource002_sub_800f33a8(20, 41);
    resource002_sub_800f4ae0(0x800CD224, 0, empty_arguments, 0);
    resource002_sub_800f33a8(139, 343);
    resource002_sub_800f4ae0(0x800CD230, 0, empty_arguments, 0);
    resource002_sub_800f59b4();
    game = r_u32(0x800BCE60);
    if (r_u32(game + 300) && (r_u32(game + 296) & 15) == 0)
    {
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) == 7)
        {
            resource002_sub_800dedf0(2, 0);
            resource002_sub_800dee08(1, 0, 0, 0);
            resource_init_queue(0, 0);
            return 0x04000000;
        }
    }
    resource002_sub_800f5a24();
    return 0;
}

uint32 resource002_sub_800fb6e8(void)
{
    uint32 game, context, state, resource, bytes, total, buffer;
    sint32 mission;

    FUNCTION_MARKER(0x800FB6E8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    context = r_u32(0x800BCE50);
    resource = (uint32)r_s16(0x80111688 + (uint32)mission * 2);
    w_u32(context + 96, resource);
    bytes = resource_total_size(context + 96, 1);
    state = r_u32(0x80112DC4);
    total = r_u32(state + 456);
    w_u32(state + 456, total + bytes);
    buffer = arena_alloc(bytes);
    context = r_u32(0x800BCE50);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    w_u32(state + 464, buffer);
    w_u32(state + 460, 0);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    return 1;
}

uint32 resource002_sub_800fb7a4(void)
{
    uint32 current, state, previous, total, game;

    FUNCTION_MARKER(0x800FB7A4, "ACE3.BIN");
    resource_read_progress(0, &current);
    state = r_u32(0x80112DC4);
    previous = r_u32(state + 464);
    w_u32(state + 464, current);
    total = r_u32(state + 460);
    game = r_u32(0x800BCE60);
    w_u32(state + 460, total + current - previous);
    return r_u32(game + 68) == 0;
}

uint32 resource002_sub_800fb7fc(uint32 context)
{
    uint32 entries, state, value, game, kind, type, resource;
    sint32 mission;

    FUNCTION_MARKER(0x800FB7FC, "ACE3.BIN");
    resource = r_u32(context + 96);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 32, entries);
    value = r_u32(entries + 8);
    w_u32(state + 448, value);
    entries = r_u32(state + 32);
    resource = r_u32(entries + 28);
    value = r_u32(entries);
    w_u32(0x80112DC8, value);
    resource002_sub_800ecda8(resource);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 32);
    resource002_sub_800ecda8(r_u32(entries + 32));
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    kind = r_u8(game + 2390);
    type = r_u8(0x8010E674 + ((uint32)mission * 4 + kind) * 2 + 1);
    if (type == 0 || type == 1)
    {
        state = r_u32(0x80112DC4);
        entries = r_u32(state + 32);
        resource = r_u32(entries + (type == 0 ? 16 : 20));
        entries = sub_80027338(resource, 0);
        resource002_sub_800ecf88(r_u32(entries));
    }
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 32);
    arena_set_cursor(r_u32(entries + 28));
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 32);
    value = sub_80027338(r_u32(entries + 4), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 288, value);
    return 1;
}

uint32 resource002_sub_800fb7f4(void)
{
    FUNCTION_MARKER(0x800FB7F4, "ACE3.BIN");
    return resource002_sub_800fb7fc(r_u32(0x800BCE50));
}

uint32 resource002_sub_800fb948(void)
{
    uint32 context, bytes, buffer, state, game;

    FUNCTION_MARKER(0x800FB948, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 0x72);
    bytes = resource_total_size(context + 96, 1);
    buffer = arena_alloc(bytes);
    context = r_u32(0x800BCE50);
    state = r_u32(0x80112DC4);
    w_u32(state + 464, buffer);
    game = r_u32(0x800BCE60);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    return 1;
}

uint32 resource002_sub_800fb9c4(void)
{
    uint32 current, state, previous, total, game;

    FUNCTION_MARKER(0x800FB9C4, "ACE3.BIN");
    resource_read_progress(0, &current);
    state = r_u32(0x80112DC4);
    previous = r_u32(state + 464);
    w_u32(state + 464, current);
    total = r_u32(state + 460);
    game = r_u32(0x800BCE60);
    w_u32(state + 460, total + current - previous);
    if (r_u32(game + 68))
        return 0;
    sub_8002dda8(r_u32(state + 304));
    return 1;
}

uint32 resource002_sub_800fba38(uint32 context)
{
    uint32 entries, state, value, buffer;

    FUNCTION_MARKER(0x800FBA38, "ACE3.BIN");
    entries = sub_80027338(r_u32(context + 96), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 24, entries);
    sub_8002d96c(r_u32(entries + 32));
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    resource002_sub_800ecda8(r_u32(entries + 28));
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    arena_set_cursor(r_u32(entries + 28));
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    value = r_u32(entries + 20);
    w_u32(state + 172, value);
    buffer = arena_alloc(128);
    state = r_u32(0x80112DC4);
    w_u32(state + 340, buffer);
    buffer = arena_alloc(16);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    w_u32(state + 444, buffer);
    resource002_sub_800ecd4c(entries + 16, buffer, 0, 0);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    value = sub_80027338(r_u32(entries + 12), 0);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    w_u32(state + 344, value);
    value = sub_80027338(r_u32(entries + 8), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 348, value);
    resource002_sub_800de908(value, 27, 0);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 24);
    resource002_sub_800e38dc(entries, r_u32(entries + 4));
    resource002_sub_80101ee4();
    return 1;
}

uint32 resource002_sub_800fba30(void)
{
    FUNCTION_MARKER(0x800FBA30, "ACE3.BIN");
    return resource002_sub_800fba38(r_u32(0x800BCE50));
}

uint32 resource002_sub_800fbbe0(void)
{
    FUNCTION_MARKER(0x800FBBE0, "ACE3.BIN");
    resource002_sub_800dee08(1, 1, 0, 10);
    return 2;
}

uint32 resource002_sub_800dee84(uint32 slot)
{
    uint32 state, table;

    FUNCTION_MARKER(0x800DEE84, "ACE3.BIN");
    state = r_u32(0x80112DC0);
    table = r_u32(state + 88);
    return (uint32)r_s16(table + slot * 2);
}

uint32 resource002_sub_800fbc0c(void)
{
    FUNCTION_MARKER(0x800FBC0C, "ACE3.BIN");
    return resource002_sub_800dee84(1) == 2 ? 3 : 0;
}

uint32 resource002_sub_800ecd4c(uint32 source, uint32 destination, uint32 *native_destination, uint32 native_capacity)
{
    uint32 base, cursor, offset, index;
    sint32 count;

    FUNCTION_MARKER(0x800ECD4C, "ACE3.BIN");
    base = r_u32(source);
    count = r_s32(base);
    if (native_destination && count > 0 && (uint32)count > native_capacity)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 relocation output exceeds native capacity\n");
#endif
        abort();
    }
    cursor = base;
    for (index = 0; (sint32)index < count; ++index)
    {
        offset = r_u32(cursor + 4);
        if ((sint32)offset < 0)
            offset += 3;
        offset = (uint32)((sint32)offset >> 2) << 2;
        if (native_destination)
            native_destination[index] = base + offset;
        else
            w_u32(destination, base + offset);
        destination += 4;
        cursor += 4;
    }
    return (uint32)count;
}

uint32 resource002_sub_800ecda8(uint32 resource)
{
    uint32 temporary, entries, count;

    FUNCTION_MARKER(0x800ECDA8, "ACE3.BIN");
    temporary = arena_alloc(0x30000);
    if (r_u32(resource) == 0x1A7A6C55)
    {
        sub_800afd08(resource, temporary);
        entries = sub_80027338(temporary, 0);
        count = r_u32(temporary);
    }
    else
    {
        entries = sub_80027338(resource, 0);
        count = r_u32(resource);
    }
    sub_8002722c(entries, count);
    DrawSync(0);
    arena_set_cursor(temporary);
    return 1;
}

uint32 resource002_sub_800ecf88(uint32 resource)
{
    uint32 temporary, data;

    FUNCTION_MARKER(0x800ECF88, "ACE3.BIN");
    temporary = arena_alloc(0x9000);
    if (r_u32(resource) == 0x1A7A6C55)
    {
        sub_800afd08(resource, temporary);
        data = temporary;
    }
    else
        data = resource;
    sub_800271a0(data);
    DrawSync(0);
    return arena_set_cursor(temporary);
}

uint32 resource002_sub_800f7524(sint32 mode, uint32 resource)
{
    uint32 owner, flags, current, desired, result, game, descriptor;
    sint32 status;

    FUNCTION_MARKER(0x800F7524, "ACE3.BIN");
    status = (sint32)resource_wait(1);
    owner = r_u32(0x80112E0C);
    w_u32(owner + 8, resource);
    if (mode == 1 && status == 0)
    {
        descriptor = r_u32(owner + 4120);
        resource_queue_audio(descriptor + resource * 12, 1, 0);
        owner = r_u32(0x80112E0C);
        flags = r_u32(owner);
        desired = r_u32(owner + 8);
        w_u32(owner + 4, desired);
        w_u32(owner, flags | 1);
    }
    else if (mode == 2)
    {
        flags = r_u32(owner);
        sub_8002c6f4((flags & 2) ? 0 : 1);
    }
    else if (mode == 3)
    {
        resource_cancel_streams();
        owner = r_u32(0x80112E0C);
        flags = r_u32(owner);
        w_u32(owner, flags & 0xFFFFFFFC);
    }
    owner = r_u32(0x80112E0C);
    current = r_u32(owner + 4);
    desired = r_u32(owner + 8);
    result = status < 2;
    if (current == desired)
    {
        if (status != 0)
            return result;
        flags = r_u32(owner);
        if (!(flags & 1))
            return status < 2;
    }
    if (!result)
        return 0x800C0000;
    game = r_u32(0x800BCE60);
    result = r_u32(game + 296) & 12;
    if (result || mode != 0)
        return result;
    resource_cancel_streams();
    owner = r_u32(0x80112E0C);
    desired = r_u32(owner + 8);
    descriptor = r_u32(owner + 4120);
    resource_queue_audio(descriptor + desired * 12, 1, 0);
    owner = r_u32(0x80112E0C);
    flags = r_u32(owner);
    desired = r_u32(owner + 8);
    w_u32(owner + 4, desired);
    result = (flags | 1) & 0xFFFFFFFD;
    w_u32(owner, result);
    return result;
}

uint32 resource002_sub_800f5028(uint32 state, uint32 scene)
{
    uint32 index, count, record, destination, kind, category, value;
    uint32 owner, cursor, limit, slot, offset, source, game_scene;
    sint32 time, event_time;

    FUNCTION_MARKER(0x800F5028, "ACE3.BIN");
    index = r_u8(state + 1);
    count = r_u8(scene + 10);
    if (index >= count)
        return 0;
    index = r_u8(state + 1);
    record = scene + index * 8 + 912;
    kind = r_u8(record);
    destination = state + index * 4 + 16;
    category = kind < 48 ? 1 : ((kind - 112 < 5) ? 2 : 0);
    owner = r_u32(0x80112E04);
    event_time = r_s16(record + 2);
    time = r_s16(owner + 2);
    if (time < event_time)
        return 1;
    for (;;)
    {
        owner = r_u32(0x80112E04);
        game_scene = r_u32(0x80112E08);
        index = r_u8(owner + 1);
        count = r_u8(game_scene + 10);
        if (index >= count)
            return 1;
        w_u8(destination + 3, 1);
        value = r_u8(record + 4);
        w_u8(destination, value);
        value = r_u8(record + 5);
        w_u8(destination + 1, value);
        value = r_u8(record + 1);
        w_u8(destination + 2, value);
        owner = r_u32(0x80112E04);
        value = r_u8(owner + 1);
        w_u8(owner + 1, value + 1);
        if (category == 1)
        {
            owner = r_u32(0x80112E04);
            limit = r_u8(owner + 976);
            slot = 0;
            cursor = owner;
            if (limit)
            {
                kind = r_u8(record);
                while (slot < limit)
                {
                    value = r_u8(cursor + 312);
                    if (value == kind)
                        break;
                    ++slot;
                    cursor += 44;
                }
            }
            if (slot < limit)
            {
                value = r_u8(cursor + 313);
                w_u8(cursor + 313, value + 1);
            }
            else
            {
                offset = slot * 44;
                owner = r_u32(0x80112E04);
                value = r_u8(record);
                w_u8(owner + offset + 312, value);
                kind = r_u8(record);
                source = r_u32(0x8010E560 + kind * 4);
                owner = r_u32(0x80112E04);
                sub_800adea8(owner + offset + 272, source);
                owner = r_u32(0x80112E04);
                w_u8(owner + offset + 313, 1);
                owner = r_u32(0x80112E04);
                value = r_u8(owner + 976);
                w_u8(owner + 976, value + 1);
            }
            sub_8002db4c(46);
        }
        else if (category == 2)
        {
            owner = r_u32(0x80112E04);
            limit = r_u8(owner + 977);
            slot = 0;
            cursor = owner;
            if (limit)
            {
                kind = r_u8(record);
                while (slot < limit)
                {
                    value = r_u8(cursor + 664);
                    if (value == kind)
                        break;
                    ++slot;
                    cursor += 44;
                }
            }
            if (slot < limit)
            {
                value = r_u8(cursor + 665);
                w_u8(cursor + 665, value + 1);
            }
            else
            {
                offset = slot * 44;
                owner = r_u32(0x80112E04);
                value = r_u8(record);
                w_u8(owner + offset + 664, value);
                kind = r_u8(record);
                source = r_u32(0x8010E620 + (kind - 112) * 4);
                owner = r_u32(0x80112E04);
                sub_800adea8(owner + offset + 624, source);
                owner = r_u32(0x80112E04);
                w_u8(owner + offset + 665, 1);
                owner = r_u32(0x80112E04);
                value = r_u8(owner + 977);
                w_u8(owner + 977, value + 1);
            }
            sub_8002db4c(47);
        }
        state = r_u32(0x80112E04);
        scene = r_u32(0x80112E08);
        index = r_u8(state + 1);
        count = r_u8(scene + 10);
        if (index == count)
            return 1;
        record = scene + index * 8 + 912;
        kind = r_u8(record);
        destination = state + index * 4 + 16;
        category = kind < 48 ? 1 : ((kind - 112 < 5) ? 2 : 0);
        owner = r_u32(0x80112E04);
        event_time = r_s16(record + 2);
        time = r_s16(owner + 2);
        if (time < event_time)
            return 1;
    }
}

uint32 resource002_sub_800f5018(void)
{
    uint32 state, scene;

    FUNCTION_MARKER(0x800F5018, "ACE3.BIN");
    state = r_u32(0x80112E04);
    scene = r_u32(0x80112E08);
    return resource002_sub_800f5028(state, scene);
}

uint32 resource002_sub_800e2494(uint32 packet)
{
    uint32 owner, state, ordering, tag, link;

    FUNCTION_MARKER(0x800E2494, "ACE3.BIN");
    w_u32(0x80112DB4, 0x1F800000);
    VSync(1);
    owner = r_u32(0x80112DC0);
    w_u32(owner + 4, packet + 12);
    sub_800ac7f8(packet, 0, 0, 0, 0);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    resource002_sub_800e23fc();
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    w_u32(owner + 4, packet + 12);
    sub_800ac7f8(packet, 0, 0, 0, 0);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    resource002_sub_800deecc();
    owner = r_u32(0x80112DC0);
    return r_u32(owner + 4);
}

uint32 resource002_sub_800e38dc(uint32 entries, uint32 resource)
{
    uint32 owner, source, destination, a, b, c, d, i, j, value;
    uint32 pointers[4];
    sint32 count;

    FUNCTION_MARKER(0x800E38DC, "ACE3.BIN");
    owner = arena_alloc(640);
    w_u32(0x80112DEC, owner);
    a = (uint32)r_s16(0x800CCD38);
    b = (uint32)r_s8(0x800CCD3A);
    w_u16(owner, a);
    w_u8(owner + 2, b);
    owner = r_u32(0x80112DEC);
    w_u8(owner + 2, 19);
    owner = r_u32(0x80112DEC);
    w_u8(owner + 3, 2);
    owner = r_u32(0x80112DEC);
    source = 0x800CCD3C;
    destination = owner + 4;
    do
    {
        a = r_u32(source);
        b = r_u32(source + 4);
        c = r_u32(source + 8);
        d = r_u32(source + 12);
        w_u32(destination, a);
        w_u32(destination + 4, b);
        w_u32(destination + 8, c);
        w_u32(destination + 12, d);
        source += 16;
        destination += 16;
    } while (source != 0x800CCD5C);
    owner = r_u32(0x80112DEC);
    a = r_u32(source);
    b = (uint32)r_s16(source + 4);
    c = (uint32)r_s8(source + 6);
    w_u32(destination, a);
    w_u16(destination + 4, b);
    w_u8(destination + 6, c);
    w_u32(owner + 76, 0);
    w_u32(owner + 72, 0);
    w_u32(owner + 68, 0);
    owner = r_u32(0x80112DEC);
    w_u16(owner + 80, 1);
    a = r_u32(0x800CCD64);
    b = (uint32)r_s8(0x800CCD68);
    w_u32(owner + 82, a);
    w_u8(owner + 86, b);
    w_u8(owner + 86, 0);
    owner = r_u32(0x80112DEC);
    w_u8(owner + 87, 0);
    owner = r_u32(0x80112DEC);
    w_u32(owner + 88, 0);
    w_u32(owner + 92, 0);
    count = (sint32)resource002_sub_800ecd4c(entries, 0, pointers, 4);
    if (count <= 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 save header requires a defined first relocation\n");
#endif
        abort();
    }
    owner = r_u32(0x80112DEC);
    w_u8(owner + 2, (uint32)count + 16);
    for (i = 0; i < 8; ++i)
    {
        owner = r_u32(0x80112DEC);
        value = r_u32(pointers[0] + i * 4 + 20);
        w_u32(owner + i * 4 + 96, value);
    }
    for (i = 0; (sint32)i < count; ++i)
    {
        for (j = 0; j < 32; ++j)
        {
            owner = r_u32(0x80112DEC);
            value = r_u32(pointers[i] + j * 4 + 64);
            w_u32(owner + i * 128 + j * 4 + 128, value);
        }
    }
    owner = r_u32(0x80112DEC);
    for (i = 0; i < 32; ++i)
    {
        value = r_u32(resource + i * 4);
        w_u32(owner + i * 4 + 512, value);
    }
    return 0;
}

uint32 resource002_sub_80101ee4(void)
{
    uint32 owner, value, table, i;
    sint32 component;

    FUNCTION_MARKER(0x80101EE4, "ACE3.BIN");
    owner = arena_alloc(272);
    w_u32(0x80112DA8, owner);
    value = arena_alloc(512);
    w_u32(owner + 128, value);
    value = arena_alloc(1024);
    w_u32(owner + 132, value);
    value = arena_alloc(96);
    w_u32(owner + 220, value);
    value = arena_alloc(768);
    w_u32(owner + 224, value);
    value = arena_alloc(128);
    w_u32(owner + 260, value);
    for (i = 0; i < 16; ++i)
    {
        component = rcos(i << 8);
        table = r_u32(owner + 260);
        w_u16(table + i * 8, (uint32)(component / 2));
        component = rsin(i << 8);
        table = r_u32(owner + 260);
        w_u16(table + i * 8 + 2, (uint32)(component / 2));
        table = r_u32(owner + 260);
        w_u16(table + i * 8 + 4, 0);
    }
    w_u32(owner + 12, 0);
    w_u32(owner + 20, 0);
    w_u32(owner + 24, 0);
    w_u32(owner + 28, 0);
    resource002_sub_800ff390(owner);
    resource002_sub_801009ac(owner);
    return resource002_sub_80101570(owner);
}

uint32 resource002_sub_800e23fc(void)
{
    uint32 group, index, state, flags;

    FUNCTION_MARKER(0x800E23FC, "ACE3.BIN");
    for (group = 0; group < 3; ++group)
    {
        for (index = 0; index < 32; ++index)
        {
            state = r_u32(0x80112DC4);
            flags = r_u32(state + group * 4 + 376);
            if (flags & ((uint32)1 << index))
                resource002_sub_800e22e4(index, group);
        }
    }
    return 0;
}

uint32 resource002_sub_800deecc(void)
{
    uint32 owner, index, offset, state, current, maximum, value;
    sint32 first, second;

    FUNCTION_MARKER(0x800DEECC, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    for (index = 0; index < 16; ++index)
    {
        offset = index * 2;
        state = r_u32(owner + 88) + offset;
        if (r_s16(state) == 1)
        {
            current = r_u32(owner + 80) + offset;
            maximum = r_u32(owner + 84) + offset;
            first = r_s16(current);
            second = r_s16(maximum);
            value = r_u16(current);
            if (first < second)
                w_u16(current, value + 1);
            else
                w_u16(state, 2);
        }
        state = r_u32(owner + 88) + offset;
        if (r_s16(state) == 3)
        {
            current = r_u32(owner + 80) + offset;
            first = r_s16(current);
            value = r_u16(current);
            if (first > 0)
                w_u16(current, value - 1);
            else
                w_u16(state, 0);
        }
    }
    return 0;
}

uint32 resource002_sub_800ff390(uint32 owner)
{
    uint32 first, second, buffer, i, distance, product;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800FF390, "ACE3.BIN");
    w_u32(owner + 120, 200);
    w_u16(owner + 112, 256);
    w_u16(owner + 264, 512);
    w_u16(owner + 108, 0);
    w_u32(owner + 124, 0);
    w_u16(owner + 114, 0);
    w_u16(owner + 266, 0);
    w_u32(owner + 12, 0);
    first = r_u32(0x800CD76C);
    second = r_u32(0x800CD770);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    w_u16(owner + 176, 15);
    w_u16(owner + 178, 13439);
    w_u16(owner + 182, 192);
    w_u16(owner + 180, 0);
    w_u16(owner + 184, 31);
    w_u16(owner + 186, 31);
    StoreImage(&rectangle, (uint32 *)psx_addr(owner + 188, (uint32)(uint16)rectangle.w * (uint16)rectangle.h * 2));
    for (i = 0; i < 2; ++i)
    {
        buffer = arena_alloc(1024);
        w_u32(owner + i * 4, buffer);
    }
    distance = 0;
    for (i = 0; i < 64; ++i)
    {
        buffer = r_u32(owner + 128);
        w_u32(buffer + i * 8, 0);
        buffer = r_u32(owner + 128);
        w_u32(buffer + i * 8 + 4, 0);
        buffer = r_u32(owner + 132);
        w_u32(buffer + i * 16, 0);
        buffer = r_u32(owner + 132);
        w_u32(buffer + i * 16 + 4, 0);
        buffer = r_u32(owner + 132);
        product = distance << 3;
        if ((sint32)product < 0)
            product += 63;
        w_u32(buffer + i * 16 + 8, 23976 - (uint32)((sint32)product >> 6));
        distance += 3125;
    }
    return 0;
}

uint32 resource002_sub_801009ac(uint32 owner)
{
    uint32 i, value, table, product, cursor;
    sint32 cosine, sine, random;

    FUNCTION_MARKER(0x801009AC, "ACE3.BIN");
    for (i = 0; i < 48; ++i)
    {
        random = (sint32)game_random();
        table = r_u32(owner + 220);
        w_u16(table + i * 2, (uint32)(random % 64));
        value = game_random();
        cosine = rcos(value & 0xFFF);
        random = (sint32)game_random();
        product = (uint32)cosine * (uint32)(random % 1536);
        table = r_u32(owner + 224);
        w_u32(table + i * 16, (uint32)((sint32)product / 4096));
        value = game_random();
        sine = rsin(value & 0xFFF);
        random = (sint32)game_random();
        product = (uint32)sine * (uint32)(random % 1536);
        table = r_u32(owner + 224);
        w_u32(table + i * 16 + 4, (uint32)((sint32)product / 4096));
        random = (sint32)game_random();
        table = r_u32(owner + 224);
        w_u32(table + i * 16 + 8, (uint32)(random % 1562));
        table = r_u32(owner + 224);
        cursor = table + i * 16;
        value = r_u32(cursor);
        value = (sint32)value < 0 ? value - 384 : value + 384;
        w_u32(cursor, value);
        table = r_u32(owner + 224);
        cursor = table + i * 16;
        value = r_u32(cursor + 4);
        value = (sint32)value < 0 ? value - 384 : value + 384;
        w_u32(cursor + 4, value);
    }
    return 0;
}

uint32 resource002_sub_80101570(uint32 owner)
{
    uint32 buffer, table, i, packed;
    sint32 angle, cosine, sine;

    FUNCTION_MARKER(0x80101570, "ACE3.BIN");
    buffer = arena_alloc(12);
    w_u32(owner + 240, buffer);
    buffer = arena_alloc(12);
    w_u32(owner + 244, buffer);
    buffer = arena_alloc(40);
    w_u32(owner + 248, buffer);
    for (i = 0; i < 3; ++i)
    {
        buffer = arena_alloc(120);
        table = r_u32(owner + 240);
        w_u32(table + i * 4, buffer);
        buffer = arena_alloc(240);
        table = r_u32(owner + 244);
        w_u32(table + i * 4, buffer);
        resource002_sub_80100fdc(i, owner);
    }
    table = r_u32(owner + 248);
    w_u32(table, 0);
    table = r_u32(owner + 248);
    w_u32(table + 4, 0);
    angle = -512;
    for (i = 1; i < 5; ++i)
    {
        cosine = rcos(angle);
        sine = rsin(angle);
        table = r_u32(owner + 248);
        packed = ((uint32)(cosine / 32) & 0xFFFF) | ((uint32)(sine / 32) << 16);
        w_u32(table + i * 8, packed);
        table = r_u32(owner + 248);
        w_u32(table + i * 8 + 4, 0);
        angle += 1024;
    }
    w_u32(owner + 252, 0);
    w_u32(owner + 256, 512);
    return 512;
}

uint32 resource002_sub_80100fdc(uint32 index, uint32 owner)
{
    uint32 depth, direction, table, cursor, value, i, position;
    sint32 width, height, lateral;

    FUNCTION_MARKER(0x80100FDC, "ACE3.BIN");
    depth = game_random() & 0x3FFF;
    width = (sint32)(depth * 160) / 250;
    height = (sint32)(depth * 240) / 250;
    direction = game_random() & 3;
    if (direction == 0 || direction == 1)
    {
        position = direction == 0 ? (uint32)(sint32)(sint16)width : 0 - (uint32)(sint32)(sint16)width;
        lateral = (sint32)game_random() % 480 - 240;
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        w_u32(cursor, position);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        value = (uint32)lateral * depth;
        w_u32(cursor + 4, (uint32)((sint32)value / 250));
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        w_u32(cursor + 8, depth);
        value = game_random();
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4);
        w_u16(cursor, value & 0x1FF);
        value = game_random();
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4);
        w_u16(cursor + 2, value & (direction == 0 ? 0xDFF : 0x5FF));
    }
    else
    {
        lateral = (sint32)game_random() % 320 - 160;
        value = (uint32)lateral * depth;
        position = (uint32)((sint32)value / 250);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        w_u32(cursor, position);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        position = direction == 2 ? (uint32)(sint32)(sint16)height : 0 - (uint32)(sint32)(sint16)height;
        w_u32(cursor + 4, position);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4);
        w_u32(cursor + 8, depth);
        value = game_random();
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4);
        w_u16(cursor, value & (direction == 2 ? 0xDFF : 0x5FF));
        value = game_random();
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4);
        w_u16(cursor + 2, value & 0xFFF);
    }
    table = r_u32(owner + 240);
    cursor = r_u32(table + index * 4);
    w_u16(cursor + 4, 0);
    for (i = 1; i < 15; ++i)
    {
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4) + i * 8;
        value = r_u16(cursor - 8);
        w_u16(cursor, value);
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4) + i * 8;
        value = r_u16(cursor - 6);
        w_u16(cursor + 2, value);
        table = r_u32(owner + 240);
        cursor = r_u32(table + index * 4) + i * 8;
        value = r_u16(cursor - 4);
        w_u16(cursor + 4, value);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4) + i * 16;
        value = r_u32(cursor - 16);
        w_u32(cursor, value);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4) + i * 16;
        value = r_u32(cursor - 12);
        w_u32(cursor + 4, value);
        table = r_u32(owner + 244);
        cursor = r_u32(table + index * 4) + i * 16;
        value = r_u32(cursor - 8);
        w_u32(cursor + 8, value);
    }
    return 0;
}

uint32 resource002_sub_800e019c(uint32 command)
{
    FUNCTION_MARKER(0x800E019C, "ACE3.BIN");
    return command + 16;
}

uint32 resource002_sub_800e01ac(uint32 command)
{
    uint32 owner;

    FUNCTION_MARKER(0x800E01AC, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    w_u32(owner + 60, command);
    return command + 16;
}

void resource002_sub_800e11e4(void)
{
    FUNCTION_MARKER(0x800E11E4, "ACE3.BIN");
}

void resource002_sub_800e1f54(void)
{
    FUNCTION_MARKER(0x800E1F54, "ACE3.BIN");
}

void resource002_sub_800e1f6c(void)
{
    FUNCTION_MARKER(0x800E1F6C, "ACE3.BIN");
}

void resource002_sub_800e1f84(void)
{
    FUNCTION_MARKER(0x800E1F84, "ACE3.BIN");
}

void resource002_sub_800e1f8c(void)
{
    FUNCTION_MARKER(0x800E1F8C, "ACE3.BIN");
}

uint32 resource002_sub_800e1b48(void)
{
    uint32 state, owner, ordering, group, cursor, packet, value, other;
    uint32 phase, vertex, product, tag, link, control, layer;
    sint32 current, offset_x, offset_y, origin_x, origin_y, angle;

    FUNCTION_MARKER(0x800E1B48, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    for (group = 0; group < 4; ++group)
    {
        state = r_u32(0x80112DC4);
        value = r_u8(state + 126);
        cursor = state + group * 2;
        if (group == value)
        {
            value = r_u16(cursor + 412);
            if (value < 128)
                w_u16(cursor + 412, value + 1);
        }
        else
        {
            current = r_s16(cursor + 412);
            value = r_u16(cursor + 412);
            if (current > 0)
                w_u16(cursor + 412, value - 1);
            state = r_u32(0x80112DC4);
            cursor = state + group * 2;
            current = r_s16(cursor + 412);
            if (current == 5)
            {
                value = r_u16(cursor + 404);
                w_u16(cursor + 404, value - 32);
            }
            state = r_u32(0x80112DC4);
            cursor = state + group * 2;
            if (r_s16(cursor + 412) < 5)
                w_u16(cursor + 412, 0);
        }
        state = r_u32(0x80112DC4);
        cursor = state + group * 2;
        value = r_u16(cursor + 404);
        other = r_u16(cursor + 412);
        owner = r_u32(0x80112DC0);
        w_u16(cursor + 404, value + other);
        packet = r_u32(owner + 4);
        w_u32(owner + 4, packet + 24);
        w_u8(packet + 3, 5);
        w_u8(packet + 7, 40);
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
        state = r_u32(0x80112DC4);
        phase = (uint32)r_s16(state + group * 2 + 404);
        offset_x = r_s16(state + group * 4 + 388);
        control = r_u32(state + 364);
        offset_y = r_s16(state + group * 4 + 390);
        origin_x = r_s16(control + 16);
        origin_y = r_s16(control + 18);
        for (vertex = 0; vertex < 4; ++vertex)
        {
            angle = (sint32)(phase + ((vertex & 1) ? 128 : 0 - (uint32)128));
            product = (uint32)rcos(angle) * (vertex < 2 ? 22 : 18);
            value = (uint32)((sint32)product / 4096) + (uint32)offset_x + (uint32)origin_x;
            w_u16(packet + 8 + vertex * 4, value);
            product = (uint32)rsin(angle) * (vertex < 2 ? 40 : 29);
            value = (uint32)((sint32)product / 4096) + (uint32)offset_y + (uint32)origin_y;
            if (vertex == 3)
                owner = r_u32(0x80112DC0);
            w_u16(packet + 10 + vertex * 4, value);
        }
        control = r_u32(owner + 60);
        layer = (uint32)r_s16(control + 14);
        tag = r_u32(packet);
        link = r_u32(ordering + layer * 4);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        control = r_u32(owner + 60);
        layer = (uint32)r_s16(control + 14);
        tag = r_u32(ordering + layer * 4);
        w_u32(ordering + layer * 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    }
    return 0;
}

static uint32 resource002_command_call(uint32 target, uint32 command, uint32 ordering, uint32 context)
{
    switch (target)
    {
        case 0x800DF760:
            return resource002_sub_800df760(command, ordering, context);
        case 0x800DF940:
            return resource002_sub_800df940(command, ordering, context);
        case 0x800DFBA8:
            return resource002_sub_800dfba8(command, ordering);
        case 0x800DFCC8:
            return resource002_sub_800dfcc8(command, ordering, context);
        case 0x800E019C:
            return resource002_sub_800e019c(command);
        case 0x800E01AC:
            return resource002_sub_800e01ac(command);
        case 0x800E01C4:
            return resource002_sub_800e01c4(command, ordering, context);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 command target %08X\n", target);
#endif
            abort();
    }
}

static uint32 resource002_command_final_call(uint32 target)
{
    switch (target)
    {
        case 0x800E11E4:
            resource002_sub_800e11e4();
            return target;
        case 0x800E1B48:
            return resource002_sub_800e1b48();
        case 0x800E1F54:
            resource002_sub_800e1f54();
            return target;
        case 0x800E1F6C:
            resource002_sub_800e1f6c();
            return target;
        case 0x800E1F84:
            resource002_sub_800e1f84();
            return target;
        case 0x800E1F8C:
            resource002_sub_800e1f8c();
            return target;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 final command target %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800e22e4(uint32 index, uint32 group)
{
#if 0
    uint32 state, table, ordering, stream, command, context, target;
#else
    uint32 state, table, ordering, stream, command, context, target;
    uint32 settings_panel = 0;
    sint32 saved_y;
#endif

    FUNCTION_MARKER(0x800E22E4, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + group * 4 + 352);
    ordering = r_u32(state + 8);
    stream = r_u32(table + index * 4);
    if (group == 0)
        resource002_sub_800e08d4(index);
    command = r_u32(stream);
    while (command != 0xFFFFFFFF)
    {
        state = r_u32(0x80112DC4);
        command = r_u32(stream);
        context = r_u32(state + group * 4 + 364);
        target = r_u32(0x8010AEC4 + command * 4);
#if 0
        stream = resource002_command_call(target, stream + 4, ordering, context + index * 12);
#else
        /* Settings panel order is audio, brightness, screen, game, footer */
        if (group == 2 && index == 12)
        {
            static const uint8 command_bytes[] = {24, 32, 16, 36, 20, 20, 24};
            if (command == 5)
                ++settings_panel;
            if (settings_panel == 2 || settings_panel == 3 ||
                (settings_panel == 5 && command == 0 && r_s16(stream + 6) >= 261))
            {
                if (command >= sizeof(command_bytes))
                    abort();
                stream += command_bytes[command];
                command = r_u32(stream);
                continue;
            }
        }
        saved_y = r_s16(context + index * 12 + 6);
        if (group == 2 && index == 12 && settings_panel == 4)
            w_u16(context + index * 12 + 6, (uint32)(saved_y - 136));
        stream = resource002_command_call(target, stream + 4, ordering, context + index * 12);
        if (group == 2 && index == 12 && settings_panel == 4)
        w_u16(context + index * 12 + 6, (uint32)saved_y);
#endif
        command = r_u32(stream);
    }
    table = r_u32(0x8010B060 + group * 4);
    target = r_u32(table + index * 4);
    return resource002_command_final_call(target);
}

uint32 resource002_sub_800dfba8(uint32 command, uint32 ordering)
{
    uint32 owner, packet, blend, y, x, page, control, layer, tag, link;

    FUNCTION_MARKER(0x800DFBA8, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    w_u32(owner + 4, packet + 12);
    blend = r_u8(command + 7);
    y = r_u16(command + 2);
    x = r_u16(command);
    page = ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    sub_800ac7f8(packet, 0, 0, page, 0);
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + layer * 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    tag = r_u32(ordering + layer * 4);
    w_u32(ordering + layer * 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return command + 12;
}

void resource002_sub_800e08d4(uint32 index)
{
    uint32 target, state, cursor, offset, value, velocity, game, ticks;
    sint32 phase, signed_velocity;

    FUNCTION_MARKER(0x800E08D4, "ACE3.BIN");
    if (index >= 16)
        return;
    target = r_u32(0x800CC8D4 + index * 4);
    switch (target)
    {
        case 0x800E0910:
            resource002_sub_800e03cc();
            return;
        case 0x800E0920:
            resource002_sub_800e03d4(index);
            return;
        case 0x800E0930:
            resource002_sub_800e056c(index);
            return;
        case 0x800E0DC8:
            resource002_sub_800e0798(index);
            return;
        case 0x800E0F74:
            return;
        case 0x800E0940:
        case 0x800E0BFC:
            state = r_u32(0x80112DC4);
            offset = index * 12;
            cursor = r_u32(state + 364) + offset;
            phase = r_s16(cursor + 10);
            if (phase != 0 && phase != 2)
                return;
            value = r_u16(cursor + 4);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 4, value + velocity);
            cursor = r_u32(state + 364) + offset;
            if (phase == 0)
            {
                signed_velocity = r_s16(cursor + 8);
                velocity = r_u16(cursor + 8);
                if (signed_velocity < 0)
                    w_u16(cursor + 8, velocity + 1);
                state = r_u32(0x80112DC4);
                cursor = r_u32(state + 364) + offset;
                if (r_s16(cursor + 8) == 0)
                    w_u16(cursor + 10, 1);
            }
            else
            {
                velocity = r_u16(cursor + 8);
                w_u16(cursor + 8, velocity + 1);
                if (target == 0x800E0940)
                {
                    cursor = r_u32(state + 364) + offset;
                    if (r_s16(cursor + 8) >= 26)
                        w_u16(cursor + 10, 3);
                }
            }
            return;
        case 0x800E0A14:
            state = r_u32(0x80112DC4);
            offset = index * 12;
            cursor = r_u32(state + 364) + offset;
            phase = r_s16(cursor + 10);
            if (phase != 0 && phase != 2)
                return;
            value = r_u16(cursor + 4);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 4, value + velocity);
            cursor = r_u32(state + 364) + offset;
            if (phase == 2)
            {
                velocity = r_u16(cursor + 8);
                w_u16(cursor + 8, velocity - 1);
                cursor = r_u32(state + 364) + offset;
                if (r_s16(cursor + 8) < -24)
                    w_u16(cursor + 10, 3);
                return;
            }
            signed_velocity = r_s16(cursor + 8);
            velocity = r_u16(cursor + 8);
            if (signed_velocity > 0)
                w_u16(cursor + 8, velocity - 1);
            state = r_u32(0x80112DC4);
            cursor = r_u32(state + 364) + offset;
            if (r_s16(cursor + 8) == 0)
                w_u16(cursor + 10, 1);
            game = r_u32(0x800BCE60);
            if ((r_u32(game + 296) & 8) && (r_u32(game + 300) & 8))
            {
                state = r_u32(0x80112DC4);
                cursor = r_u32(state + 364) + offset;
                velocity = r_u16(cursor + 8);
                w_u16(cursor + 8, 0 - velocity);
                cursor = r_u32(state + 364) + offset;
                w_u16(cursor + 10, 2);
            }
            game = r_u32(0x800BCE60);
            if ((r_u32(game + 296) & 4) && (r_u32(game + 300) & 4))
            {
                state = r_u32(0x80112DC4);
                cursor = r_u32(state + 364) + index * 12;
                velocity = r_u16(cursor + 8);
                w_u16(cursor + 8, 0 - velocity);
                cursor = r_u32(state + 364) + index * 12;
                w_u16(cursor + 10, 2);
            }
            return;
        case 0x800E0CEC:
            state = r_u32(0x80112DC4);
            offset = index * 12;
            cursor = r_u32(state + 364) + offset;
            phase = r_s16(cursor + 10);
            if (phase != 0 && phase != 2)
                return;
            value = r_u16(cursor + 6);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 6, value + velocity);
            cursor = r_u32(state + 364) + offset;
            signed_velocity = r_s16(cursor + 8);
            velocity = r_u16(cursor + 8);
            if (phase == 0 && signed_velocity <= 0)
                w_u16(cursor + 10, 1);
            else if (phase == 2 && signed_velocity < -24)
                w_u16(cursor + 10, 3);
            else
                w_u16(cursor + 8, velocity - 1);
            return;
        case 0x800E0DD8:
            state = r_u32(0x80112DC4);
            offset = index * 12;
            cursor = r_u32(state + 364) + offset;
            phase = r_s16(cursor + 10);
            if (phase == 0)
            {
                ticks = r_u32(state + 420);
                if ((sint32)ticks < 33)
                {
                    value = r_u16(cursor + 6);
                    velocity = r_u16(cursor + 8);
                    w_u16(cursor + 6, value + velocity);
                    cursor = r_u32(state + 364) + offset;
                    signed_velocity = r_s16(cursor + 8);
                    velocity = r_u16(cursor + 8);
                    if (signed_velocity < 0)
                        w_u16(cursor + 8, velocity + 1);
                }
                else if ((sint32)ticks < 36)
                {
                    value = r_u16(cursor + 6);
                    w_u16(cursor + 6, value - 8);
                }
                else if ((sint32)ticks < 38)
                {
                    if (ticks == 36)
                        sub_8002db4c(92);
                    state = r_u32(0x80112DC4);
                    cursor = r_u32(state + 364) + offset;
                    value = r_u16(cursor + 6);
                    w_u16(cursor + 6, value + 4);
                }
                else
                    w_u16(cursor + 10, 1);
                state = r_u32(0x80112DC4);
                ticks = r_u32(state + 420);
                w_u32(state + 420, ticks + 1);
            }
            else if (phase == 2)
            {
                value = r_u16(cursor + 6);
                velocity = r_u16(cursor + 8);
                w_u16(cursor + 6, value + velocity);
                cursor = r_u32(state + 364) + offset;
                if (r_s16(cursor + 6) >= 480)
                    w_u16(cursor + 10, 3);
                else
                {
                    velocity = r_u16(cursor + 8);
                    w_u16(cursor + 8, velocity + 2);
                }
                state = r_u32(0x80112DC4);
                ticks = r_u32(state + 420);
                if ((sint32)ticks > 0)
                    w_u32(state + 420, ticks - 1);
            }
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 motion jump target %08X\n", target);
#endif
            abort();
    }
}
