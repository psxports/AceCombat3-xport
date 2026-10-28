#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8005cb48(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 packet, x, i, color, tag, head, result, field;
    sint32 number, rolling, quotient, offset;
    PSX_RECT rect;
    FUNCTION_MARKER(0x8005CB48, "ACE3.BIN");
    number = (sint32)(0 - ((uint32)((native_state != 0 ? (sint32)xport_load_le32(native_state + 4) : r_s32(state + 4)) >> 12) << 1)) / 3;
    rolling = number / 10 % 10;
    if (number > 99999)
        number = 99999;
    else if (number < -9999)
        number = -9999;
    if (number < 0)
    {
        packet = dst + 240;
        color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
        sub_8003b7b0(11, 243, 103, 0, packet, color);
        number = -number;
        tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    packet = dst + 140;
    x = 283;
    i = 0;
    do
    {
        quotient = number / 10;
        color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
        sub_8003b7b0((uint32)number - (uint32)quotient * 10, x, 102, 1, packet, color);
        number = quotient;
        tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
        if (number == 0)
            break;
        x -= 10;
        ++i;
        packet += 20;
    } while (i < 5);
    w_u8(dst + 4, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
    w_u8(dst + 5, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
    w_u8(dst + 6, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
    w_u8(dst + 24, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
    w_u8(dst + 25, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
    w_u8(dst + 26, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
    if (rolling < 0)
        rolling += 10;
    field = dst + 50;
    offset = -10;
    for (i = 2; i < 7; ++i)
    {
        color = (native_state != 0 ? native_state[32] : r_u8(state + 32));
        w_u8(field - 6, color);
        color = (native_state != 0 ? native_state[33] : r_u8(state + 33));
        offset += 10;
        w_u8(field - 5, color);
        color = (native_state != 0 ? native_state[34] : r_u8(state + 34));
        w_u16(field, offset - 10 + rolling + 96);
        w_u8(field - 4, color);
        field += 20;
    }
    head = r_u32(0x800BCE60);
    rect.x = 302;
    rect.y = 96;
    rect.w = 6;
    rect.h = 40;
    if (r_u32(head + 4) == 1)
        rect.y = 352;
    SetDrawArea((DR_AREA *)psx_addr(dst + 260, 12), &rect);
    head = r_u32(0x800BCE60);
    rect.w = 320;
    rect.x = 0;
    rect.y = 0;
    rect.h = 240;
    if (r_u32(head + 4) == 1)
        rect.y = 256;
    SetDrawArea((DR_AREA *)psx_addr(dst + 272, 12), &rect);
    tag = r_u32(dst + 20);
    w_u32(dst + 20, (tag & 0xFF000000) | ((dst + 260) & 0xFFFFFF));
    tag = r_u32(dst + 260);
    w_u32(dst + 260, (tag & 0xFF000000) | ((dst + 40) & 0xFFFFFF));
    tag = r_u32(dst + 120);
    w_u32(dst + 120, (tag & 0xFF000000) | ((dst + 272) & 0xFFFFFF));
    tag = r_u32(dst + 272);
    head = r_u32(object);
    w_u32(dst + 272, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8005d000(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 color = 1, tag, head, result, blank;
    sint32 mode, frames, major, minor;
    FUNCTION_MARKER(0x8005D000, "ACE3.BIN");
    mode = (native_state != 0 ? (sint16)xport_load_le16(native_state + 38) : r_s16(state + 38));
    if (mode == 1)
    {
        frames = (native_state != 0 ? (sint32)xport_load_le32(native_state + 40) : r_s32(state + 40));
        if (frames < 1800)
        {
            major = frames / 30;
            minor = (frames % 30) * 100 / 30;
        }
        else
        {
            frames = (native_state != 0 ? (sint32)xport_load_le32(native_state + 40) : r_s32(state + 40));
            major = frames / 1800;
            minor = frames / 30 - major * 60;
        }
    }
    else
    {
        frames = (native_state != 0 ? (sint32)xport_load_le32(native_state + 40) : r_s32(state + 40));
        major = frames / 1800;
        minor = frames / 30 - major * 60;
    }
    if ((native_state != 0 ? (sint16)xport_load_le16(native_state + 38) : r_s16(state + 38)) != 1 && (native_state != 0 ? native_state[32] : r_u8(state + 32)) == 0)
        color = 0;
    sub_8003b7b0(major / 10, 24, 211, 1, dst + 40, color);
    sub_8003b7b0(major % 10, 34, 211, 1, dst + 60, color);
    sub_8003b7b0(10, 44, 211, 1, dst + 80, color);
    sub_8003b7b0(minor / 10, 49, 211, 1, dst + 100, color);
    sub_8003b7b0(minor % 10, 59, 211, 1, dst + 120, color);
    if ((native_state != 0 ? (sint16)xport_load_le16(native_state + 38) : r_s16(state + 38)) == 1)
    {
        w_u8(dst + 4, 131);
        w_u8(dst + 5, 0);
        w_u8(dst + 6, 0);
    }
    else
    {
        w_u8(dst + 4, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
        w_u8(dst + 5, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
        w_u8(dst + 6, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
    }
    if ((native_state != 0 ? (sint16)xport_load_le16(native_state + 38) : r_s16(state + 38)) == 1)
    {
        blank = (native_state != 0 ? xport_load_le32(native_state + 40) : r_u32(state + 40)) & 4;
        w_u8(dst + 24, blank ? 0 : 131);
        w_u8(dst + 25, 0);
        w_u8(dst + 26, 0);
    }
    else
    {
        frames = (sint32)((native_state != 0 ? xport_load_le32(native_state + 40) : r_u32(state + 40)) << 1);
        blank = (frames / 15) & 1;
        if (blank)
        {
            w_u8(dst + 24, 0);
            w_u8(dst + 25, 0);
            w_u8(dst + 26, 0);
        }
        else
        {
            w_u8(dst + 24, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
            w_u8(dst + 25, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
            w_u8(dst + 26, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
        }
    }
    tag = r_u32(dst + 120);
    head = r_u32(object);
    w_u32(dst + 120, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8005de40(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 field, i, tag, head, result;
    FUNCTION_MARKER(0x8005DE40, "ACE3.BIN");
    field = dst + 6;
    for (i = 0; i < 2; ++i)
    {
        w_u8(field - 2, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
        w_u8(field - 1, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
        w_u8(field, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
        field += 20;
    }
    field = dst + 46;
    for (i = 0; i < 3; ++i)
    {
        w_u8(field - 2, (native_state != 0 ? native_state[28] : r_u8(state + 28)) >> 1);
        w_u8(field - 1, (native_state != 0 ? native_state[29] : r_u8(state + 29)) >> 1);
        w_u8(field, (native_state != 0 ? native_state[30] : r_u8(state + 30)) >> 1);
        field += 16;
    }
    tag = r_u32(dst + 72);
    head = r_u32(object);
    w_u32(dst + 72, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8005d998(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 color, tag, head, packet, field, i = 0, result, dim;
    sint32 count, value, delta;
    FUNCTION_MARKER(0x8005D998, "ACE3.BIN");
    color = (native_state != 0 ? native_state[32] : r_u8(state + 32));
    tag = r_u32(dst);
    w_u8(dst + 4, color);
    w_u8(dst + 5, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
    w_u8(dst + 6, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
    head = r_u32(object);
    w_u32(dst, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    w_u32(object, (head & 0xFF000000) | (dst & 0xFFFFFF));
    count = (native_state != 0 ? (sint32)xport_load_le32(native_state + 20) : r_s32(state + 20));
    value = (native_state != 0 ? (sint32)xport_load_le32(native_state + 16) : r_s32(state + 16));
    packet = dst + 20;
    delta = (sint32)((uint32)count - (uint32)value);
    if (count > 0)
    {
        field = packet + 6;
        do
        {
            dim = (sint32)i < (native_state != 0 ? (sint32)xport_load_le32(native_state + 24) : r_s32(state + 24)) || (sint32)i < delta;
            if (dim)
            {
                w_u8(field - 2, (native_state != 0 ? native_state[32] : r_u8(state + 32)) >> 2);
                w_u8(field - 1, (native_state != 0 ? native_state[33] : r_u8(state + 33)) >> 2);
                color = (native_state != 0 ? native_state[34] : r_u8(state + 34)) >> 2;
            }
            else
            {
                w_u8(field - 2, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
                w_u8(field - 1, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
                color = (native_state != 0 ? native_state[34] : r_u8(state + 34));
            }
            w_u8(field, color);
            field += 20;
            ++i;
            tag = r_u32(packet);
            head = r_u32(object);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(object);
            w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = (native_state != 0 ? (sint32)xport_load_le32(native_state + 20) : r_s32(state + 20));
            packet += 20;
        } while ((sint32)i < count);
        value = (native_state != 0 ? (sint32)xport_load_le32(native_state + 16) : r_s32(state + 16));
    }
    if (value >= 100)
    {
        packet = dst + 140;
        color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
        sub_8003b7b0(1, 237, 200, 1, packet, color);
        tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    value = (native_state != 0 ? (sint32)xport_load_le32(native_state + 16) : r_s32(state + 16));
    if (value >= 10)
    {
        packet = dst + 160;
        color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
        sub_8003b7b0(value / 10 % 10, 243, 200, 1, packet, color);
        tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    value = (native_state != 0 ? (sint32)xport_load_le32(native_state + 16) : r_s32(state + 16));
    packet = dst + 180;
    color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
    sub_8003b7b0(value % 10, 251, 200, 1, packet, color);
    tag = r_u32(packet);
    head = r_u32(object);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_80035a38(void)
{
    uint32 context, header, copied, delta, a0, a1, a2, v0, v1, cursor;
    uint32 game, runtime, i, offset, table, target;
    sint32 count;
    FUNCTION_MARKER(0x80035A38, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    header = sub_80027338(r_u32(context + 96), 0);
    sub_8002d96c(r_u32(header + 64));
    for (offset = 60; offset >= 44; offset -= 4)
    {
        a0 = r_u32(header + offset);
        a1 = r_u32(header + offset + 4);
        sub_800272f0(a0, a1);
        DrawSync(0);
    }
    count = (sint32)(r_u32(header + 44) - header) >> 2;
    copied = 0x801FB700 - ((uint32)count << 2);
    delta = (uint32)((sint32)(copied - header) >> 2) << 2;
    copy_words(copied, header, count);
    cursor = header + 68;
    v0 = r_u32(copied);
    v1 = r_u32(copied + 8);
    w_u32(copied, v0 + delta);
    v0 = r_u32(copied + 4);
    w_u32(copied + 8, v1 + delta);
    v1 = r_u32(copied + 16);
    w_u32(copied + 4, v0 + delta);
    v0 = r_u32(copied + 12);
    w_u32(copied + 16, v1 + delta);
    v1 = r_u32(copied + 24);
    w_u32(copied + 12, v0 + delta);
    v0 = r_u32(copied + 20);
    w_u32(copied + 24, v1 + delta);
    v1 = r_u32(copied + 32);
    w_u32(copied + 20, v0 + delta);
    v0 = r_u32(copied + 28);
    w_u32(copied + 32, v1 + delta);
    v1 = r_u32(copied + 40);
    w_u32(copied + 28, v0 + delta);
    v0 = r_u32(copied + 36);
    w_u32(copied + 40, v1 + delta);
    w_u32(copied + 36, v0 + delta);
    w_u32(header, cursor);
    for (i = 0; i < 10; ++i)
    {
        a0 = r_u32(copied + i * 4);
        cursor = sub_800afd08(a0, cursor);
        w_u32(header + (i + 1) * 4, cursor);
    }
    a0 = r_u32(copied + 40);
    cursor = sub_800afd08(a0, cursor);
    v1 = r_u32(header);
    w_u32(header + 44, cursor);
    w_u32(context + 24, v1);
    v0 = r_u32(header + 12);
    game = r_u32(0x800BCE60);
    v0 = r_u32(v0);
    w_u32(game + 4156, v0);
    v0 = r_u32(r_u32(header + 20));
    w_u32(game + 4164, v0);
    v0 = r_u32(header + 4);
    runtime = context + 24;
    w_u32(runtime + 4, v0);
    w_u32(runtime + 32, r_u32(header + 32));
    w_u32(runtime + 24, r_u32(header + 24));
    w_u32(runtime + 28, r_u32(header + 28));
    v0 = sub_80027338(r_u32(header + 8), 0);
    w_u32(runtime + 8, v0 - 4);
    v0 = sub_80027338(r_u32(header + 12), 0);
    w_u32(runtime + 12, v0);
    v0 = sub_80027338(r_u32(header + 16), 0);
    w_u32(runtime + 16, v0);
    v0 = sub_80027338(r_u32(header + 20), 0);
    w_u32(runtime + 20, v0);
    v0 = sub_80027338(r_u32(header + 36), 0);
    w_u32(runtime + 36, v0);
    v0 = sub_80027338(r_u32(header + 40), 0);
    w_u32(runtime + 40, v0);
    count = r_s32(r_u32(header + 8));
    i = 1;
    if (count > 0)
    {
        do
        {
            table = r_u32(runtime + 8);
            target = r_u32(table + i * 4);
            relocate_pointer_array(target, target + 4, 4);
            count = r_s32(r_u32(header + 8));
            ++i;
        } while (!((sint32)i > count));
    }
    for (offset = 12; offset <= 20; offset += 4)
    {
        count = r_s32(r_u32(header + offset));
        i = 0;
        if (count > 0)
        {
            do
            {
                table = r_u32(runtime + offset);
                target = r_u32(table + i * 4);
                relocate_pointer_array(target, target + 4, offset == 20 ? 4 : 6);
                count = r_s32(r_u32(header + offset));
                ++i;
            } while ((sint32)i < count);
        }
    }
    count = r_s32(r_u32(header + 16));
    i = 0;
    if (count > 0)
    {
        do
        {
            table = r_u32(runtime + 16);
            target = r_u32(runtime + 12);
            v0 = r_u32(table + i * 4);
            w_u32(target + i * 4 + 384, v0);
            count = r_s32(r_u32(header + 16));
            ++i;
        } while ((sint32)i < count);
    }
    a1 = r_u32(runtime);
    a0 = a1 + 8;
    a2 = r_u32(a1 + 116);
    sub_8005ebb8(a0, a1 + 120, a2);
    arena_set_cursor(r_u32(header + 44));
    sub_800357c0();
    return 0x4000000;
}

uint32 sub_8005eb20(uint32 dst, uint32 src, sint32 count, uint32 colors, uint32 factor, uint8 *native_dst, const uint8 *native_src)
{
    uint32 red, green, blue, pixel, value, result;
    sint32 i;
    FUNCTION_MARKER(0x8005EB20, "ACE3.BIN");
    red = r_u8(colors);
    green = r_u8(colors + 1);
    blue = r_u8(colors + 2);
    xport_gte_write_control(21, red << 4);
    xport_gte_write_control(22, green << 4);
    xport_gte_write_control(23, blue << 4);
    xport_gte_write_data(8, factor);
    result = green << 4;
    for (i = 0; i < count; ++i)
    {
        if (native_src)
            pixel = xport_load_le16(native_src);
        else
            pixel = r_u16(src);
        src += 2;
        if (native_src)
            native_src += 2;
        if (pixel != 0)
        {
            xport_gte_write_data(28, pixel);
            xport_gte_execute(0x980011);
            value = xport_gte_read_data(29);
            if (pixel & 0x8000)
                value |= 0x8000;
        }
        else
        {
            value = 0;
        }
        if (native_dst)
            xport_store_le16(native_dst, value);
        else
            w_u16(dst, value);
        result = i + 1 < count;
        dst += 2;
        if (native_dst)
            native_dst += 2;
    }
    return result;
}

uint32 sub_80035f18(void)
{
    uint32 game, context, value, cursor, state;
    sint32 index;
    FUNCTION_MARKER(0x80035F18, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    index = r_s16(game + 910);
    context = r_u32(0x800BCE50);
    value = r_s16(0x800B0B84 + ((uint32)index << 1));
    w_u32(context + 96, value);
    value = resource_total_size(context + 96, 1);
    cursor = arena_alloc(value);
    context = r_u32(0x800BCE50);
    state = r_u32(0x800BCBE0);
    w_u32(state + 8, 1);
    resource_queue_list(context + 96, 1, cursor, 0x800357B4);
    sub_800357c0();
    return 0x4000000;
}

uint32 sub_80035f98(void)
{
    uint32 context, header, a0, a1, copied, delta, cursor, v0, v1;
    uint32 i, table, target, runtime, game, flags;
    sint32 count;
    FUNCTION_MARKER(0x80035F98, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    header = sub_80027338(r_u32(context + 96), 0);
    sub_8002d96c(r_u32(header + 12));
    a0 = r_u32(header + 8);
    a1 = r_u32(header + 12);
    sub_800272f0(a0, a1);
    DrawSync(0);
    count = (sint32)(r_u32(header + 8) - header) >> 2;
    copied = 0x801FB700 - ((uint32)count << 2);
    delta = (uint32)((sint32)(copied - header) >> 2) << 2;
    copy_words(copied, header, count);
    cursor = header + 16;
    v0 = r_u32(copied);
    v1 = r_u32(copied + 4);
    w_u32(copied, v0 + delta);
    w_u32(copied + 4, v1 + delta);
    w_u32(header, cursor);
    cursor = sub_800afd08(r_u32(copied), cursor);
    w_u32(header + 4, cursor);
    cursor = sub_800afd08(r_u32(copied + 4), cursor);
    a0 = r_u32(header);
    w_u32(header + 8, cursor);
    v0 = sub_80027338(a0, 0);
    w_u32(context + 8, v0);
    count = r_s32(r_u32(header));
    runtime = context + 8;
    i = 0;
    if (count > 0)
    {
        do
        {
            table = r_u32(runtime);
            target = r_u32(table + i * 4);
            relocate_pointer_array(target, target + 4, 6);
            count = r_s32(r_u32(header));
            ++i;
        } while ((sint32)i < count);
    }
    v0 = r_u32(header);
    game = r_u32(0x800BCE60);
    v0 = r_u32(v0);
    w_u32(game + 4160, v0);
    w_u32(runtime + 4, r_u32(header + 4));
    arena_set_cursor(r_u32(header + 8));
    v0 = arena_peek(0);
    context = r_u32(0x800BCE50);
    w_u32(context + 92, v0);
    sub_800357c0();
    context = r_u32(0x800BCE48);
    flags = r_u32(context);
    w_u32(context + 4, 0);
    w_u32(context + 20, 0);
    w_u32(context, flags | 5);
    return 0x4000000;
}

uint32 sub_8005e7ec(uint32 near_value, uint32 far_value, uint32 height)
{
    uint64 numerator, quotient, offset, height64;
    uint32 coefficient, result;
    sint64 signed_offset;
    FUNCTION_MARKER(0x8005E7EC, "ACE3.BIN");
    numerator = (uint64)((sint64)(sint32)(near_value - far_value) * 0x1000000);
    quotient = __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
    height64 = (uint64)(sint64)(sint32)height;
    numerator = quotient * height64;
    offset = __divdi3PSX(numerator, (uint64)(sint64)(sint32)far_value);
    quotient = __divdi3PSX((uint64)1 << 32, offset);
    coefficient = (uint32)quotient;
    if (coefficient + 0x8000 > 0xFFFF)
    {
        coefficient = (sint32)coefficient > 0 ? 0x7FFF : 0xFFFF8000;
        numerator = ((uint64)(sint64)(sint32)(0 - coefficient) << 16) * height64;
        __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
        numerator = ((uint64)(sint64)(sint32)coefficient << 16) * height64;
        quotient = __divdi3PSX(numerator, (uint64)(sint64)(sint32)far_value);
        offset = (uint64)0x1000000 - quotient;
    }
    else
    {
        numerator = ((uint64)(sint64)(sint32)(0 - coefficient) << 16) * height64;
        offset = __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
    }
    signed_offset = (sint64)offset;
    if (signed_offset > 2147483647)
        result = 0x7FFFFFFF;
    else if (signed_offset < -2147483647)
        result = 0x80000001;
    else
        result = (uint32)offset;
    xport_gte_write_control(27, coefficient);
    xport_gte_write_control(28, result);
    return result;
}

uint32 sub_8005eab8(uint32 near_value, uint32 far_value, uint32 dst)
{
    uint32 delta = far_value - near_value, i, product;
    sint32 value;
    FUNCTION_MARKER(0x8005EAB8, "ACE3.BIN");
    for (i = 1; i <= 16; ++i)
    {
        product = i * delta;
        value = (sint32)((uint32)((sint32)product / 16) + near_value) >> 3;
        if (value >= 0xFFE)
            value = 0xFFD;
        if (value < 0)
            value = 0;
        w_u16(dst, value);
        dst += 2;
    }
    w_u16(dst, 0xFFE);
    return 0xFFE;
}

uint32 sub_80032754(void)
{
    uint32 context, game, scene, table, object, flags, value, parameter, cursor;
    uint32 i, j, src, dst, w0, w1, w2, w3, a0, a1, a2, angles_address;
    uint32 c0, c1, index, limit, amount;
    sint32 signed_value;
    uint8 mission_types[38];
    SVECTOR angles;
    MATRIX matrix = {0};
    FUNCTION_MARKER(0x80032754, "ACE3.BIN");
    if (r_s16(r_u32(0x800BCE60) + 940) == 0)
        sub_800357c0();
    context = r_u32(0x800BCE48);
    table = r_u32(0x800BCE50);
    flags = r_u32(context);
    w_u32(context + 20, 2);
    game = r_u32(0x800BCE60);
    w_u32(context + 4, 0);
    w_u32(context, flags | 5);
    flags = r_u32(game);
    value = r_u32(game + 944);
    w_u32(game, flags | 0x10000000);
    w_u32(table + 112, value);
    w_u32(table + 116, r_u32(game + 948));
    DrawSync(0);
    arena_set_cursor(r_u32(r_u32(0x800BCE50) + 92));
    scene = arena_alloc(700);
    context = r_u32(0x800BCE50);
    w_u32(0x800BCE54, scene);
    w_u32(scene, 0);
    w_u32(scene + 4, 0);
    w_u32(scene + 8, 0);
    w_u32(scene + 12, 0);
    w_u16(scene + 668, 0);
    w_u16(scene + 670, 0);
    w_u32(scene + 672, 0x80000001);
    w_u32(scene + 676, 0x7FFFFFFF);
    w_u32(scene + 680, 0x80000001);
    w_u32(scene + 684, 0x7FFFFFFF);
    w_u32(scene + 688, 0x80000001);
    w_u32(scene + 692, 0x7FFFFFFF);
    src = context + 24;
    dst = scene + 580;
    for (i = 0; i < 4; ++i)
    {
        w0 = r_u32(src);
        w1 = r_u32(src + 4);
        w2 = r_u32(src + 8);
        w3 = r_u32(src + 12);
        w_u32(dst, w0);
        w_u32(dst + 4, w1);
        w_u32(dst + 8, w2);
        w_u32(dst + 12, w3);
        src += 16;
        dst += 16;
    }
    w_u32(dst, r_u32(src));
    scene = r_u32(0x800BCE54);
    context = r_u32(0x800BCE50);
    parameter = r_u32(scene + 592);
    w_u32(parameter + 12, r_u32(context + 8));
    table = arena_alloc(256);
    w_u32(0x800BCE5C, table);
    w_u32(0x800BCE4C, table + 128);
    for (i = 0; i < 2; ++i)
    {
        object = arena_alloc(88);
        w_u32(r_u32(0x800BCE5C) + i * 4, object);
    }
    for (i = 2; i < 32; ++i)
    {
        object = arena_alloc(92);
        w_u32(r_u32(0x800BCE5C) + i * 4, object);
    }
    for (i = 0; i < 32; ++i)
    {
        object = arena_alloc(148);
        w_u32(r_u32(0x800BCE4C) + i * 4, object);
    }
    for (i = 0; i < 64; ++i)
    {
        object = r_u32(r_u32(0x800BCE5C) + i * 4);
        w_u32(object, 0);
        w_u8(object + 4, 0);
        w_u8(object + 5, 0);
        w_u16(object + 6, 0);
        w_u32(object + 8, 0);
        w_u32(object + 12, 0);
        w_u32(object + 16, 0);
        w_u16(object + 24, 0);
        w_u16(object + 26, 0);
        w_u16(object + 28, 0);
    }
    table = r_u32(0x800BCE5C);
    for (i = 0; i < 64; ++i)
    {
        object = r_u32(table);
        w_u32(object + 32, 0);
        w_u32(object + 36, 0);
        w_u32(object + 40, 0);
        w_u16(object + 48, 0);
        w_u16(object + 50, 0);
        w_u16(object + 52, 0);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u16(object + 72, 0);
        w_u16(object + 74, 0);
        w_u16(object + 76, 0);
        w_u16(object + 78, 0);
        w_u32(object + 80, 0);
        w_u32(object + 84, 0);
        table += 4;
    }
    object = r_u32(r_u32(0x800BCE5C));
    value = r_u8(object + 5);
    w_u8(object + 5, value | 1);
    scene = r_u32(0x800BCE54);
    parameter = r_u32(scene + 580);
    w_u32(scene + 40, scene + 184);
    context = r_u32(0x800BCE50);
    a2 = r_s16(parameter + 12);
    angles_address = r_u32(context + 12) + 4;
    a1 = r_s16(parameter + 14);
    object = r_u32(scene + 40);
    sub_80077880(object, 296, a2, a1, angles_address, NULL);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 72, 0);
    w_u32(scene + 68, 0);
    w_u32(scene + 64, 0);
    w_u32(scene + 104, 0);
    w_u32(scene + 100, 0);
    w_u32(scene + 96, 0);
    xport_gte_write_control(24, 0);
    xport_gte_write_control(25, 0);
    object = r_u32(r_u32(0x800BCE54) + 40);
    xport_gte_write_control(26, r_s16(object + 36));
    object = r_u32(r_u32(0x800BCE54) + 40);
    a0 = r_s16(object + 40);
    a1 = r_s16(object + 42);
    a2 = r_s16(object + 36);
    sub_8005e7ec(a0, a1, a2);
    cursor = arena_alloc(36);
    scene = r_u32(0x800BCE54);
    parameter = r_u32(scene + 580);
    w_u32(scene + 648, cursor);
    a0 = r_s16(parameter + 12);
    a1 = r_s16(parameter + 14);
    sub_8005eab8(a0, a1, cursor);
    scene = r_u32(0x800BCE54);
    parameter = r_u32(scene + 580);
    w_u16(scene + 140, r_u16(parameter + 64));
    w_u16(scene + 146, r_u16(parameter + 66));
    w_u16(scene + 152, r_u16(parameter + 68));
    w_u16(scene + 142, r_u16(parameter + 80));
    w_u16(scene + 148, r_u16(parameter + 82));
    value = r_u16(parameter + 84);
    w_u16(scene + 144, 0);
    w_u16(scene + 150, 0);
    w_u16(scene + 156, 0);
    w_u16(scene + 154, value);
    for (i = 0; i < 9; ++i)
        w_u16(scene + 108 + i * 2, 0);
    w_u32(scene + 172, r_s16(parameter + 70));
    w_u32(scene + 176, r_s16(parameter + 72));
    w_u32(scene + 180, r_s16(parameter + 74));
    for (j = 0; j < 2; ++j)
    {
        angles.vx = r_u16(parameter + (j == 0 ? 60 : 76));
        angles.vy = r_u16(parameter + (j == 0 ? 62 : 78));
        angles.vz = 0;
        angles.pad = 0;
        RotMatrixYXZ(&angles, &matrix);
        for (i = 0; i < 5; ++i)
            xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + i * 4));
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 4096);
        xport_gte_execute(0x486012);
        w0 = xport_gte_read_data(9);
        w1 = xport_gte_read_data(10);
        w2 = xport_gte_read_data(11);
        w_u16(scene + 108 + j * 6, w0);
        w_u16(scene + 110 + j * 6, w1);
        w_u16(scene + 112 + j * 6, w2);
    }
    sub_8004c6ec();
    sub_80038b2c();
    for (i = 0; i < 32; i += 16)
    {
        w0 = r_u32(0x800234F8 + i);
        w1 = r_u32(0x800234FC + i);
        w2 = r_u32(0x80023500 + i);
        w3 = r_u32(0x80023504 + i);
        xport_store_le32(mission_types + i, w0);
        xport_store_le32(mission_types + i + 4, w1);
        xport_store_le32(mission_types + i + 8, w2);
        xport_store_le32(mission_types + i + 12, w3);
    }
    table = r_u32(0x800BCE5C);
    value = r_u32(0x80023518);
    c0 = r_s8(0x8002351C);
    c1 = r_s8(0x8002351D);
    xport_store_le32(mission_types + 32, value);
    mission_types[36] = c0;
    mission_types[37] = c1;
    object = r_u32(table);
    w_u8(object + 4, 0);
    index = r_s16(r_u32(0x800BCE60) + 910);
    w_u16(object + 72, mission_types[(sint32)index]);
    sub_8003a008();
    sub_8007da98();
    sub_80084a6c();
    sub_8006c588();
    sub_8006ce70(r_u32(r_u32(0x800BCE5C)));
    sub_8004d794();
    game = r_u32(0x800BCE60);
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C) + 8;
    parameter = scene + 580;
    for (i = 2; i < 64; ++i)
    {
        object = r_u32(table);
        flags = r_u32(object);
        if (flags & 0x4000)
        {
            index = r_u8(object + 4);
            if ((flags & 0x20) && !(r_u8(object + 5) & 2))
            {
                limit = r_u32(game + 4164);
                if ((sint32)index >= (sint32)limit)
                    goto next_object;
                dst = r_u32(parameter + 20);
                amount = r_s16(r_u32(dst + index * 4) + 2);
            }
            else
            {
                limit = r_u32(game + 4156);
                if ((sint32)(index * 4) >= (sint32)limit)
                    goto next_object;
                dst = r_u32(parameter + 12);
                value = r_u16(r_u32(dst + index * 16) + 2);
                signed_value = (sint16)value >> 2;
                if (signed_value < 15)
                    signed_value = 15;
                amount = signed_value;
            }
            if (r_u32(object) & 0x20)
            {
                signed_value = r_s16(object + 74);
                value = r_u16(object + 74);
                if (signed_value <= 0)
                    w_u16(object + 74, amount - value);
                signed_value = r_s16(object + 76);
                value = r_u16(object + 76);
                if (signed_value <= 0)
                    w_u16(object + 76, amount - value);
            }
            else
            {
                w0 = r_u16(object + 74);
                w1 = r_u16(object + 76);
                w_u16(object + 74, w0 + amount);
                w_u16(object + 76, w1 + amount);
            }
        }
    next_object:
        table += 4;
    }
    table = r_u32(0x800BCE5C);
    for (i = 0; i < 2; ++i)
    {
        object = r_u32(table);
        w_u16(object + 74, 54);
        w_u16(object + 76, 54);
        table += 4;
    }
    sub_80035740(1);
    sub_80032518(r_s16(r_u32(0x800BCE60) + 908));
    cursor = arena_peek(0);
    scene = r_u32(0x800BCE54);
    game = r_u32(0x800BCE60);
    w_u32(scene + 696, cursor);
    if (r_u8(game + 933))
    {
        sub_800324ac(cursor);
        context = r_u32(0x800BCE50);
        w_u32(context + 112, 0x4000000);
        w_u32(context + 116, 0);
    }
    else
    {
        sub_800323f8(cursor);
    }
    context = r_u32(0x800BCE50);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 4, r_u32(context + 112));
    w_u32(scene + 8, r_u32(context + 116));
    return sub_80032080(0, 0);
}

uint32 sub_8004c6ec(void)
{
    uint32 table, block, object, i, j;
    FUNCTION_MARKER(0x8004C6EC, "ACE3.BIN");
    table = arena_alloc(288);
    w_u32(0x800BCFEC, table);
    for (i = 0; i < 2; ++i)
    {
        block = arena_alloc(1928);
        table = r_u32(0x800BCFEC);
        w_u32(0x800BCFE8, block);
        w_u32(table + i * 4, block);
        for (j = 0; j < 32; ++j)
        {
            w_u8(r_u32(0x800BCFE8) + j * 40 + 643, 9);
            w_u8(r_u32(0x800BCFE8) + j * 40 + 647, 44);
        }
        for (j = 0; j < 32; ++j)
        {
            w_u8(r_u32(0x800BCFE8) + j * 20 + 3, 4);
            w_u8(r_u32(0x800BCFE8) + j * 20 + 7, 100);
        }
        w_u8(r_u32(0x800BCFE8) + 1923, 1);
        w_u32(r_u32(0x800BCFE8) + 1924, 0xE1000656);
    }
    for (i = 0; i < 32; ++i)
    {
        object = r_u32(0x800BCFEC) + i * 8;
        w_u8(object + 8, 0);
    }
    table = r_u32(0x800BCFEC);
    w_u32(table + 268, 0);
    w_u32(table + 264, 0);
    w_u32(table + 272, 0);
    w_u32(table + 276, 0);
    w_u32(table + 280, 0);
    w_u32(table + 284, 255);
    return table;
}

uint32 sub_80038b2c(void)
{
    uint32 source, seed, random, offset;
    FUNCTION_MARKER(0x80038B2C, "ACE3.BIN");
    source = r_u32(r_u32(0x800BCE54) + 608);
    w_u32(0x800BCF50, source);
    if (r_u32(source) == 3)
    {
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        source = r_u32(0x800BCF50);
        offset = r_u32(source + (random % 3) * 4 + 4);
        w_u32(0x800BCF50, source + offset);
    }
    sub_8004d1ac();
    sub_80053e08();
    sub_80053bac();
    sub_8002cbcc();
    sub_80056198();
    sub_8004ce9c();
    sub_8005b648();
    sub_8004856c();
    sub_80038a6c();
    sub_80038ad4();
    sub_80045ff0();
    sub_80038af4(r_u32(r_u32(0x800BCE54) + 40));
    sub_80048bdc();
    return sub_80051824();
}

uint32 sub_8004d1ac(void)
{
    uint32 resource, source, v0, v1, flags, table, base, i, index, count;
    uint32 out, entry, offset;
    FUNCTION_MARKER(0x8004D1AC, "ACE3.BIN");
    resource = r_u32(0x800BCF50);
    source = resource + r_u32(resource + 4);
    v0 = r_u8(source);
    v1 = r_u32(source + 16);
    w_u8(0x800BCEDE, v0);
    w_u8(0x800BCEF1, r_u8(source + 4));
    flags = r_u8(source + 8);
    v0 = r_u32(source + 20);
    w_u8(0x800BCEE8, flags);
    flags = r_u8(source + 12);
    w_u32(0x800BCEEC, source + (v1 << 2));
    v1 = r_u32(source + 24);
    w_u32(0x800BCF0C, source + (v0 << 2));
    v0 = r_u32(source + 28);
    w_u32(0x800BCF00, source + (v1 << 2));
    v1 = r_u32(source + 32);
    w_u32(0x800BCF2C, source + (v0 << 2));
    v0 = r_u32(source + 36);
    w_u32(0x800BCF48, source + (v1 << 2));
    v1 = r_u32(source + 40);
    w_u32(0x800BCF44, source + (v0 << 2));
    v0 = r_u32(source + 44);
    w_u32(0x800BCF38, source + (v1 << 2));
    v1 = r_u32(source + 48);
    w_u32(0x800BCF40, source + (v0 << 2));
    v0 = r_u32(source + 52);
    w_u32(0x800BCF04, source + (v1 << 2));
    v1 = r_u32(source + 56);
    w_u32(0x800BCF28, source + (v0 << 2));
    v0 = source + (r_u32(source + 60) << 2);
    w_u32(0x800BCF30, source + (v1 << 2));
    w_u8(0x800BCEF0, flags);
    w_u32(0x800BCF20, v0);
    table = source + (r_u32(source + 64) << 2);
    base = table + 64;
    for (i = 0; i < 16; ++i)
    {
        index = r_u32(table + i * 4);
        w_u32(0x800C7110 + i * 4, index == 0xFFFFFFFF ? 0 : base + (index << 5));
    }
    table = source + (r_u32(source + 68) << 2);
    for (i = 0; i < 4; ++i)
    {
        index = r_u32(table + i * 4);
        w_u32(0x800C6E90 + i * 4, index == 0xFFFFFFFF ? 0 : table + index);
    }
    out = arena_alloc(r_u8(0x800BCEDE) * 12);
    count = r_u8(0x800BCEDE);
    w_u32(0x800BCEF4, out);
    i = 0;
    offset = 0;
    if (count != 0)
    {
        do
        {
            out = r_u32(0x800BCEF4) + offset;
            entry = r_u32(0x800BCEEC) + offset;
            w_u32(out, 1);
            w_u16(out + 4, r_u16(entry + 2));
            w_u8(out + 6, r_u8(entry + 4));
            v0 = r_u32(0x800BCEF4) + offset;
            ++i;
            w_u8(v0 + 7, 0);
            v1 = r_u32(0x800BCEF4) + offset;
            count = r_u8(0x800BCEDE);
            offset += 12;
            w_u32(v1 + 8, 0);
        } while (i < count);
    }
    out = arena_alloc(r_u8(0x800BCEF1) << 2);
    count = r_u8(0x800BCEF1);
    w_u32(0x800BCF18, out);
    if (count != 0)
    {
        entry = r_u32(0x800BCF0C);
        for (i = 0; i < count; ++i)
        {
            if (r_u16(entry) == 4)
                w_u32(out, 0);
            else
                w_u32(out, r_u32(out) | 1);
            out += 4;
            entry += 20;
        }
    }
    w_u32(0x800BCF24, resource + r_u32(resource + 8));
    sub_8004d4d0();
    sub_8004d680();
    for (i = 0; i < 64; ++i)
    {
        w_u32(0x800C67A0 + i * 4, 0);
        w_u32(0x800C6ED0 + i * 4, 0);
    }
    v1 = r_u32(r_u32(0x800BCE54) + 612);
    w_u32(0x800BCF1C, v1);
    w_u32(0x800BCEE0, v1 + 32);
    return sub_80048b48();
}

uint32 sub_8004d4d0(void)
{
    uint32 result, count, i = 0, out_offset = 0, param_offset = 0, arena_offset = 0;
    uint32 out, table, object, arena, source, value, base, index;
    FUNCTION_MARKER(0x8004D4D0, "ACE3.BIN");
    result = arena_alloc(r_u8(0x800BCEE8) << 6);
    w_u32(0x800BCEF8, result);
    result = arena_alloc(5888);
    count = r_u8(0x800BCEE8);
    w_u32(0x800BCEC0, result);
    if (count != 0)
    {
        do
        {
            out = r_u32(0x800BCEF8) + out_offset;
            table = r_u32(0x800BCE5C);
            object = r_u32(table + i * 4);
            arena = r_u32(0x800BCEC0);
            source = r_u32(0x800BCF00) + param_offset;
            w_u32(out + 8, arena + arena_offset);
            w_u32(out + 12, source);
            w_u32(out + 4, object);
            index = r_u8(source + 32);
            w_u32(out + 16, r_u32(0x800BCF2C) + (index << 4));
            index = r_u8(source + 3);
            w_u32(out + 20, r_u32(0x800BCF24) + index * 40);
            index = r_u8(source + 33);
            w_u32(out + 24, r_u32(0x800BCF44) + index * 24);
            index = r_u8(source + 34);
            if (index == 255)
                w_u32(out + 28, 0);
            else
                w_u32(out + 28, r_u32(0x800BCF38) + index * 12);
            source = r_u32(0x800BCF00) + param_offset;
            index = r_u8(source + 35);
            if (index == 255)
            {
                out = r_u32(0x800BCEF8) + out_offset;
                w_u32(out + 32, 0);
            }
            else
            {
                out = r_u32(0x800BCEF8) + out_offset;
                index = r_u8(source + 35);
                base = r_u32(0x800BCF40);
                w_u32(out + 32, base + (index << 3));
            }
            arena_offset += 184;
            ++i;
            out = r_u32(0x800BCEF8) + out_offset;
            source = r_u32(0x800BCF00) + param_offset;
            w_u32(out + 36, 0);
            value = r_u32(source + 36);
            param_offset += 44;
            w_u32(out + 40, value);
            value = r_u32(source + 40);
            index = r_u8(out + 40);
            out_offset += 64;
            w_u32(out + 60, 0);
            w_u32(out + 56, 0);
            w_u32(out + 52, 0);
            w_u32(out + 44, value);
            result = r_u32(0x800BCF48) + (index << 5);
            count = r_u8(0x800BCEE8);
            w_u32(out + 48, result);
        } while (i < count);
    }
    return result;
}

uint32 sub_8004d680(void)
{
    uint32 result, count, i = 0, out, source, object, index;
    FUNCTION_MARKER(0x8004D680, "ACE3.BIN");
    result = arena_alloc(r_u8(0x800BCEF0) * 40);
    count = r_u8(0x800BCEF0);
    w_u32(0x800BCEFC, result);
    if (count != 0)
    {
        out = result;
        source = r_u32(0x800BCF04);
        do
        {
            object = r_u32(r_u32(0x800BCE4C) + i * 4);
            w_u32(out + 8, source);
            w_u32(out + 4, object);
            index = r_u8(source + 32);
            w_u32(out + 12, r_u32(0x800BCF2C) + (index << 4));
            index = r_u8(source + 33);
            if (index == 255)
                w_u32(out + 16, 0);
            else
                w_u32(out + 16, r_u32(0x800BCF28) + index * 24);
            index = r_u8(source + 34);
            w_u32(out + 20, r_u32(0x800BCF30) + index * 48);
            index = r_u8(source + 35);
            if (index == 255)
                w_u32(out + 24, 0);
            else
                w_u32(out + 24, r_u32(0x800BCF20) + (index << 4));
            w_u32(out + 36, 0);
            w_u32(out + 32, 0);
            w_u32(out + 28, 0);
            out += 40;
            count = r_u8(0x800BCEF0);
            ++i;
            result = i < count;
            source += 36;
        } while (result);
    }
    return result;
}

uint32 sub_80048b48(void)
{
    uint32 i, index, entry, field, kind, target;
    sint32 value;
    FUNCTION_MARKER(0x80048B48, "ACE3.BIN");
    for (i = 0; i < 32; ++i)
    {
        index = r_u8(r_u32(0x800BCF1C) + i);
        entry = r_u32(0x800BCEE0) + (index << 3);
        if (r_s16(entry) != 0)
        {
            field = entry + 4;
            do
            {
                kind = r_u8(field - 2);
                target = r_u8(field - 1);
                value = r_s16(field);
                entry += 8;
                sub_8005e460(kind, target, value);
                value = r_s16(entry);
                field += 8;
            } while (value != 0);
        }
    }
    return 0;
}

uint32 sub_80053e08(void)
{
    uint32 source, out, i, j, src, dst, value, kind, index, target, first, second, result;
    uint32 record, count_index = 0;
    sint32 count, relative;
    FUNCTION_MARKER(0x80053E08, "ACE3.BIN");
    source = r_u32(0x800BCF50);
    source += r_u32(source + 12);
    w_u32(0x800BCEB8, source);
    out = arena_alloc(600);
    source = r_u32(0x800BCEB8);
    w_u32(0x800BCEAC, out);
    w_u32(out, source + (uint32)r_s16(source + 4));
    w_u32(out + 4, source + (uint32)r_s16(source + 6));
    for (i = 0; i < 4; ++i)
    {
        src = source + i * 2;
        dst = out + i * 4;
        w_u32(dst + 8, source + (uint32)r_s16(src + 8));
        w_u32(dst + 24, source + (uint32)r_s16(src + 16));
        w_u32(dst + 40, source + (uint32)r_s16(src + 24));
        w_u32(dst + 56, source + (uint32)r_s16(src + 32));
        w_u32(dst + 328, source + (uint32)r_s16(src + 168));
        for (j = 0; j < 16; ++j)
        {
            src = source + i * 32 + j * 2;
            w_u32(out + i * 64 + j * 4 + 72, source + (uint32)r_s16(src + 40));
            w_u16(out + i * 32 + j * 2 + 344, r_u16(src + 176));
        }
    }
    src = r_u32(0x800BCEB8);
    dst = r_u32(0x800BCEAC);
    for (i = 0; i < 8; ++i)
    {
        w_u32(dst + 472, source + (uint32)r_s16(src + 304));
        value = source + (uint32)r_s16(src + 320);
        src += 2;
        w_u32(dst + 504, value);
        dst += 4;
    }
    src = r_u32(0x800BCEB8);
    dst = r_u32(0x800BCEAC);
    for (i = 0; i < 4; ++i)
    {
        w_u32(dst + 536, source + (uint32)r_s16(src + 336));
        value = source + (uint32)r_s16(src + 344);
        src += 2;
        w_u32(dst + 552, value);
        dst += 4;
    }
    src = r_u32(0x800BCEB8);
    dst = r_u32(0x800BCEAC);
    for (i = 0; i < 4; ++i)
    {
        w_u32(dst + 568, source + (uint32)r_s16(src + 352));
        value = source + (uint32)r_s16(src + 360);
        src += 2;
        w_u32(dst + 584, value);
        dst += 4;
    }
    src = r_u32(0x800BCEB8);
    count = r_s32(src + 368);
    result = src + 372;
    if (count > 0)
    {
        record = result;
        first = r_u32(0x800BCEF8);
        second = r_u32(0x800BCEFC);
        do
        {
            relative = r_s16(record + 2);
            kind = r_u8(record + 1);
            index = r_u8(record);
            target = source + (uint32)relative;
            if (kind == 1)
            {
                if (index < 32)
                    w_u32(first + (index << 6) + 52, target);
                else
                    w_u32(second + index * 40 - 1252, target);
            }
            else if (kind == 2)
            {
                if (index < 32)
                {
                    index = r_u8(record);
                    w_u32(first + (index << 6) + 56, target);
                }
                else
                {
                    index = r_u8(record);
                    w_u32(second + index * 40 - 1248, target);
                }
            }
            else if (kind == 4)
            {
                if (index < 32)
                {
                    index = r_u8(record);
                    w_u32(first + (index << 6) + 60, target);
                }
                else
                {
                    index = r_u8(record);
                    w_u32(second + index * 40 - 1244, target);
                }
            }
            count = r_s32(r_u32(0x800BCEB8) + 368);
            ++count_index;
            result = (sint32)count_index < count;
            record += 4;
        } while (result);
    }
    return result;
}

uint32 sub_80053bac(void)
{
    uint32 source, out, other, value, i;
    FUNCTION_MARKER(0x80053BAC, "ACE3.BIN");
    source = r_u32(0x800BCF50);
    w_u32(0x800BCEB0, source + r_u32(source + 16));
    out = arena_alloc(64);
    w_u32(0x800BCEB4, out);
    w_u8(out + 10, 0);
    other = r_u32(0x800BCEB4);
    w_u32(out, 0);
    w_u32(out + 4, 0);
    w_u16(out + 8, 0);
    w_u8(other + 11, 0);
    out = r_u32(0x800BCEB4);
    w_u8(out + 28, 0);
    other = r_u32(0x800BCEB4);
    w_u32(out + 12, 0);
    w_u32(out + 16, 0);
    w_u32(out + 20, 0);
    w_u8(other + 29, 0);
    out = r_u32(0x800BCEB4);
    w_u16(other + 24, 0);
    w_u16(other + 26, 0);
    w_u8(out + 30, 0);
    for (i = 31; i <= 35; ++i)
        w_u8(r_u32(0x800BCEB4) + i, 0);
    w_u8(r_u32(0x800BCEB4) + 61, 0);
    for (i = 36; i <= 42; ++i)
        w_u8(r_u32(0x800BCEB4) + i, 0);
    w_u8(r_u32(0x800BCEB4) + 43, 255);
    value = sub_80056964(r_s16(r_u32(0x800BCE60) + 908));
    w_u8(r_u32(0x800BCEB4) + 44, value);
    for (i = 45; i <= 49; ++i)
        w_u8(r_u32(0x800BCEB4) + i, 0);
    out = r_u32(0x800BCEB4);
    w_u8(out + 52, 0);
    other = r_u32(0x800BCEB4);
    w_u16(out + 50, 0);
    w_u8(other + 53, 0);
    w_u8(r_u32(0x800BCEB4) + 54, 0);
    w_u8(r_u32(0x800BCEB4) + 55, 0);
    out = r_u32(0x800BCEB4);
    w_u8(out + 60, 0);
    other = r_u32(0x800BCEB4);
    w_u16(out + 56, 0);
    w_u16(out + 58, 0);
    w_u8(other + 62, 0);
    w_u8(r_u32(0x800BCEB4) + 63, 0);
    return sub_80054ebc(r_u32(r_u32(0x800BCEAC)));
}

uint32 sub_80056964(uint32 index)
{
    FUNCTION_MARKER(0x80056964, "ACE3.BIN");
    return r_u8(0x800B23D8 + index);
}

uint32 sub_80032f18(uint32 mode, uint32 argument)
{
    uint32 context, scene, result;
    FUNCTION_MARKER(0x80032F18, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    scene = r_u32(0x800BCE54);
    w_u32(context + 112, mode);
    w_u32(context + 116, argument);
    result = r_u32(scene) | 0x20000000;
    w_u32(scene, result);
    return result;
}

uint32 sub_80056198(void)
{
    uint32 source, count, state, i, result;
    FUNCTION_MARKER(0x80056198, "ACE3.BIN");
    source = r_u32(0x800BCF50);
    source += r_u32(source + 20);
    count = r_u32(source);
    source += 4;
    w_u32(0x800BD034, source);
    w_u32(0x800BD024, count);
    w_u32(0x800BD02C, source + (count << 2));
    state = arena_alloc(24);
    w_u32(0x800BD020, state);
    w_u32(state, 255);
    for (i = 0; i < 4; ++i)
    {
        state = r_u32(0x800BD020);
        w_u8(state + i + 20, 255);
        state = r_u32(0x800BD020) + i * 2;
        w_u16(state + 12, 0);
        w_u16(state + 4, 0);
    }
    result = sub_800566f0();
    w_u32(0x800BD028, 0);
    return result;
}

uint32 sub_800566f0(void)
{
    uint32 state;
    FUNCTION_MARKER(0x800566F0, "ACE3.BIN");
    state = arena_alloc(16);
    w_u32(0x800BD030, state);
    return sub_8005671c(0);
}

uint32 sub_8005671c(uint32 value)
{
    uint32 state;
    FUNCTION_MARKER(0x8005671C, "ACE3.BIN");
    state = r_u32(0x800BD030);
    w_u8(state + 7, 255);
    w_u8(state + 6, 255);
    w_u32(state, value);
    w_u16(state + 4, 0);
    state = r_u32(0x800BD030);
    w_u8(state + 9, 0);
    w_u8(state + 8, 0);
    state = r_u32(0x800BD030);
    w_u32(0x800BD028, 2);
    w_u32(state + 12, 0);
    return sub_8002cc44(0, 0, 0);
}

uint32 sub_8004ce9c(void)
{
    uint32 state;
    FUNCTION_MARKER(0x8004CE9C, "ACE3.BIN");
    state = r_u32(0x800BCFEC);
    w_u32(state + 272, 0);
    w_u32(state + 284, 255);
    return 255;
}

void sub_8005b648(void)
{
    FUNCTION_MARKER(0x8005B648, "ACE3.BIN");
    w_u8(0x800BD05C, 0);
}

uint32 sub_8004856c(void)
{
    uint32 game, state, object, value, result;
    sint32 coordinate;
    FUNCTION_MARKER(0x8004856C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    state = game + 952;
    w_u8(game + 952, 0);
    w_u8(state + 1, 0);
    game = r_u32(0x800BCE60);
    w_u16(state + 2, 0);
    value = r_u8(game + 908);
    w_u8(state + 4, value);
    value = r_u8(r_u32(0x800BCE60) + 908);
    w_u8(state + 11, 4);
    w_u8(state + 10, 0);
    w_u8(state + 5, value);
    game = r_u32(0x800BCE60);
    w_u16(state + 8, 0);
    w_u8(game + 2390, 0);
    object = r_u32(r_u32(0x800BCE5C));
    coordinate = (r_s32(object + 8) >> 12) + 0x40000;
    coordinate >>= 11;
    if (coordinate < 0)
        coordinate = 0;
    if (coordinate >= 256)
        coordinate = 255;
    result = state + ((uint32)r_s16(state + 8) << 1);
    w_u8(result + 12, coordinate);
    coordinate = (r_s32(object + 16) >> 12) + 0x40000;
    coordinate >>= 11;
    if (coordinate < 0)
        coordinate = 0;
    if (coordinate >= 256)
        coordinate = 255;
    result = state + ((uint32)r_s16(state + 8) << 1);
    w_u8(result + 13, coordinate);
    return result;
}

uint32 sub_80038a6c(void)
{
    uint32 object, table;
    FUNCTION_MARKER(0x80038A6C, "ACE3.BIN");
    sub_8004d84c();
    object = r_u32(r_u32(0x800BCE5C));
    w_u8(object + 4, 0);
    table = r_u32(0x800BCE5C);
    w_u32(object, 0x1C240);
    w_u32(object + 8, 0);
    w_u32(object + 12, 0xFFB50000);
    w_u32(object + 16, 0);
    w_u16(object + 24, 0);
    w_u16(object + 26, 0);
    w_u16(object + 28, 0);
    w_u32(r_u32(table + 4), 1);
    return 1;
}

uint32 sub_80038ad4(void)
{
    FUNCTION_MARKER(0x80038AD4, "ACE3.BIN");
    return sub_8004e3a8();
}

uint32 sub_80038af4(uint32 state)
{
    uint32 flags, table, object, result;
    FUNCTION_MARKER(0x80038AF4, "ACE3.BIN");
    flags = r_u32(state);
    table = r_u32(0x800BCE5C);
    w_u32(state, flags | 0x100);
    object = r_u32(table);
    w_u32(state + 56, 0);
    w_u32(state + 60, 0);
    w_u32(state + 64, 0);
    w_u16(state + 72, 0);
    w_u16(state + 74, 0);
    w_u16(state + 76, 0);
    result = object + 8;
    w_u32(state + 52, result);
    return result;
}

uint32 sub_8004d800(uint32 index)
{
    uint32 state;
    FUNCTION_MARKER(0x8004D800, "ACE3.BIN");
    state = r_u32(0x800C6ED0 + (index << 2));
    w_u32(state, 0);
    w_u32(state + 4, 0x100000);
    w_u32(state + 16, 0);
    w_u32(state + 12, 0);
    w_u32(state + 8, 0);
    w_u16(state + 24, 0);
    w_u16(state + 26, 0);
    w_u16(state + 28, 0);
    w_u16(state + 30, 0);
    w_u32(state + 32, 0);
    w_u32(state + 36, 0);
    return 0;
}

uint32 sub_80083dfc(uint32 first, uint32 second, const uint32 *native_first, const uint32 *native_second)
{
    uint32 x, y, z, other_x, other_y, other_z, dx, dy, dz;
    uint32 a, b, c, sum, scale, leading, value, shift;
    sint32 sx, sy, sz;
    FUNCTION_MARKER(0x80083DFC, "ACE3.BIN");
    x = native_first ? native_first[0] : r_u32(first);
    y = native_first ? native_first[1] : r_u32(first + 4);
    z = native_first ? native_first[2] : r_u32(first + 8);
    other_x = native_second ? native_second[0] : r_u32(second);
    other_y = native_second ? native_second[1] : r_u32(second + 4);
    other_z = native_second ? native_second[2] : r_u32(second + 8);
    dx = x - other_x;
    dy = y - other_y;
    dz = z - other_z;
    sx = (sint32)dx >> 12;
    sy = (sint32)dy >> 12;
    sz = (sint32)dz >> 12;
    xport_gte_write_data(9, sx);
    xport_gte_write_data(10, sy);
    xport_gte_write_data(11, sz);
    scale = 0;
    xport_gte_execute(0xA00428);
    a = sx < 0 ? 0 - (uint32)sx : (uint32)sx;
    b = sy < 0 ? 0 - (uint32)sy : (uint32)sy;
    c = sz < 0 ? 0 - (uint32)sz : (uint32)sz;
    sum = a + b + c;
    if ((sint32)sum > 0x58000)
        return 0x100000;
    if ((sint32)sum > 0xB000)
    {
        xport_gte_write_data(9, (sint32)dx >> 15);
        xport_gte_write_data(10, (sint32)dy >> 15);
        xport_gte_write_data(11, (sint32)dz >> 15);
        scale = 3;
        xport_gte_execute(0xA00428);
    }
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    sum = a + b + c;
    xport_gte_write_data(30, sum);
    if ((sint32)sum <= 0)
        return 0;
    leading = xport_gte_read_data(31) & 0xFE;
    if ((sint32)leading < 24)
        value = (uint32)((sint32)sum >> ((24 - leading) & 31));
    else
        value = sum << ((leading - 24) & 31);
    value = r_s16(0x800B6DD4 + ((value - 64) << 1));
    shift = (uint32)((sint32)(31 - leading) >> 1);
    value <<= shift & 31;
    value = (uint32)((sint32)value >> 12);
    return value << scale;
}

uint32 sub_8004d84c(void)
{
    uint32 index, table, object, record, parameters, attributes, source, aux, kind;
    uint32 w0, w1, w2, w3, value, first_table, result = 0, count;
    FUNCTION_MARKER(0x8004D84C, "ACE3.BIN");
    table = r_u32(0x800BCE5C) + 8;
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(table);
        table += 4;
        w_u32(object, index);
        w_u32(0x800C6FD0 + index * 4, 0);
    }
    count = r_u8(0x800BCEE8);
    for (index = 2; index < count; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        record = r_u32(0x800BCEF8) + index * 64;
        parameters = r_u32(record + 12);
        attributes = r_u32(record + 16);
        value = r_u32(object);
        source = r_u32(record + 48);
        w_u32(object, value | 0x4000);
        value = arena_alloc(12);
        w_u32(0x800C67A0 + index * 4, value);
        value = arena_alloc(40);
        w_u32(0x800C6ED0 + index * 4, value);
        sub_8004d800(index);
        kind = r_u8(parameters);
        if (kind < 3)
            w_u32(object, r_u32(object) | (0x40 << kind));
        w_u8(object + 4, r_u8(parameters + 2));
        value = r_u16(attributes);
        first_table = r_u32(0x800BCE5C);
        w_u16(object + 78, value);
        w_u16(object + 72, r_u8(parameters + 1));
        w0 = r_u32(parameters + 4);
        w1 = r_u32(parameters + 8);
        w2 = r_u32(parameters + 12);
        w3 = r_u32(parameters + 16);
        w_u32(object + 8, w0);
        w_u32(object + 12, w1);
        w_u32(object + 16, w2);
        w_u32(object + 20, w3);
        w0 = r_u32(parameters + 20);
        w1 = r_u32(parameters + 24);
        w_u32(object + 24, w0);
        w_u32(object + 28, w1);
        value = sub_80083dfc(object + 8, r_u32(first_table) + 8, 0, 0);
        w_u32(r_u32(0x800C6ED0 + index * 4) + 4, value);
        record = r_u32(0x800BCEF8) + index * 64;
        w_u32(object + 32, 0);
        w_u32(object + 36, 0);
        value = r_u32(r_u32(record + 20) + 4);
        aux = r_u32(0x800BCEC0);
        w_u16(object + 48, 0);
        w_u16(object + 50, 0);
        w_u16(object + 52, 0);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u32(object + 84, 0x80200000);
        aux += index * 184;
        w_u32(object + 40, value);
        w_u32(aux, 0);
        value = r_u8(source);
        w_u8(aux + 29, 0);
        w_u8(aux + 30, 0);
        w_u32(aux + 56, 0);
        w_u8(aux + 28, value);
        w_u8(aux + 31, r_u8(source + 1));
        w0 = r_u32(source + 8);
        w1 = r_u32(source + 12);
        w2 = r_u32(source + 16);
        w3 = r_u32(source + 20);
        w_u32(aux + 32, w0);
        w_u32(aux + 36, w1);
        w_u32(aux + 40, w2);
        w_u32(aux + 44, w3);
        w_u32(aux + 48, r_u32(source + 24));
        value = r_u32(source + 28);
        w_u8(aux + 60, 255);
        w_u8(aux + 61, 255);
        w_u8(aux + 62, 0);
        w_u32(aux + 52, value);
        w_u16(aux + 64, 0);
        w_u8(aux + 98, 255);
        w_u8(aux + 99, 255);
        w_u8(aux + 100, 0);
        w_u8(aux + 63, 255);
        w_u8(aux + 96, 255);
        w_u8(aux + 97, 0);
        count = r_u8(0x800BCEE8);
        w_u16(aux + 102, 0);
        w_u16(aux + 182, 0);
        w_u16(aux + 4, 0);
        w_u16(aux + 6, 0);
        w_u16(aux + 8, 0);
        w_u32(aux + 12, 0);
        w_u32(aux + 16, 0);
        w_u32(aux + 20, 0);
        w_u16(aux + 128, 0);
        w_u16(aux + 130, 0);
        w_u16(aux + 132, 0);
        result = aux;
    }
    return result;
}

uint32 sub_8004e3a8(void)
{
    uint32 index, table, object, record, parameters, attributes, kind, value;
    uint32 w0, w1, w2, w3, first_table, count;
    FUNCTION_MARKER(0x8004E3A8, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    for (index = 0; index < 32; ++index)
    {
        object = r_u32(table);
        table += 4;
        w_u32(object, index + 32);
        w_u32(0x800C6FD0 + (index + 32) * 4, 0);
    }
    count = r_u8(0x800BCEF0);
    for (index = 0; index < count; ++index)
    {
        table = r_u32(0x800BCE4C);
        record = r_u32(0x800BCEFC);
        object = r_u32(table + index * 4);
        record += index * 40;
        parameters = r_u32(record + 8);
        value = r_u32(object);
        attributes = r_u32(record + 12);
        w_u32(object, value | 0x4000);
        value = arena_alloc(12);
        w_u32(0x800C67A0 + (index + 32) * 4, value);
        value = arena_alloc(40);
        w_u32(0x800C6ED0 + (index + 32) * 4, value);
        sub_8004d800(index + 32);
        kind = r_u8(parameters);
        if (kind < 3)
            w_u32(object, r_u32(object) | (0x40 << kind));
        w_u32(object + 84, 0);
        w_u8(object + 5, 0);
        w0 = r_u32(parameters + 4);
        w1 = r_u32(parameters + 8);
        w2 = r_u32(parameters + 12);
        w3 = r_u32(parameters + 16);
        w_u32(object + 8, w0);
        w_u32(object + 12, w1);
        w_u32(object + 16, w2);
        w_u32(object + 20, w3);
        w0 = r_u32(parameters + 20);
        w1 = r_u32(parameters + 24);
        w_u32(object + 24, w0);
        w_u32(object + 28, w1);
        value = r_u8(parameters + 2);
        w_u8(object + 4, value == 255 ? 0 : value);
        value = r_u8(parameters + 31);
        if (value == 1)
            w_u8(object + 5, r_u8(object + 5) | 2);
        w_u16(object + 6, 0);
        value = r_u8(parameters + 1);
        first_table = r_u32(0x800BCE5C);
        w_u16(object + 72, value);
        value = r_u16(attributes);
        w_u32(object + 32, 0);
        w_u32(object + 36, 0);
        w_u32(object + 40, 0);
        w_u16(object + 48, 0);
        w_u16(object + 50, 0);
        w_u16(object + 52, 0);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u32(object + 84, 0x80000000);
        w_u16(object + 78, value);
        value = sub_80083dfc(object + 8, r_u32(first_table) + 8, 0, 0);
        w_u32(r_u32(0x800C6ED0 + (index + 32) * 4) + 4, value);
        w_u8(object + 88, 0);
        w_u8(object + 89, 0);
        w_u8(object + 90, 0);
        w_u8(object + 91, 255);
        count = r_u8(0x800BCEF0);
        w_u32(object + 92, 0);
        w_u32(object + 96, 0);
        w_u32(object + 100, 0);
        w_u32(object + 104, 1);
    }
    return 0;
}

uint32 sub_80045ff0(void)
{
    uint32 index, record;
    FUNCTION_MARKER(0x80045FF0, "ACE3.BIN");
    for (index = 0; index < 64; ++index)
    {
        record = 0x800C6D80 + index * 4;
        w_u8(record, 0);
        w_u8(record + 3, 255);
        w_u8(record + 2, 255);
        w_u8(record + 1, 255);
    }
    w_u8(0x800C6D80, 255);
    return 255;
}

uint32 sub_80048bdc(void)
{
    FUNCTION_MARKER(0x80048BDC, "ACE3.BIN");
    w_u16(0x800C7194, 1);
    w_u32(0x800C7190, 0);
    w_u8(0x800C7199, 255);
    w_u16(0x800C7196, 65535);
    w_u8(0x800C7198, 255);
    w_u8(0x800C719A, 0);
    return 0xFFFFFFFF;
}

uint32 sub_80051824(void)
{
    FUNCTION_MARKER(0x80051824, "ACE3.BIN");
    w_u8(0x800BCFF0, 255);
    w_u8(0x800BCFF1, 255);
    w_u32(0x800BCFF4, 0x100000);
    return 0x100000;
}

uint32 sub_8003a008(void)
{
    FUNCTION_MARKER(0x8003A008, "ACE3.BIN");
    w_u32(0x800C6D60, 0);
    w_u8(0x800C6D6E, 255);
    w_u8(0x800C6D72, 255);
    w_u8(0x800C6D73, 255);
    w_u8(0x800C6D6F, 0);
    w_u16(0x800C6D66, 0);
    w_u32(0x800C6EC8, 3200);
    sub_80063af0();
    sub_8005e2c0();
    sub_8003b924();
    sub_8003ecbc();
    sub_80041120();
    return sub_80042938();
}

uint32 sub_8005e2c0(void)
{
    uint32 index, block;
    FUNCTION_MARKER(0x8005E2C0, "ACE3.BIN");
    block = arena_alloc(8);
    w_u32(0x800BD060, block);
    for (index = 0; index < 2; ++index)
    {
        block = arena_alloc(1060);
        w_u32(r_u32(0x800BD060) + index * 4, block);
        sub_8005e10c(block);
    }
    return 0;
}

uint32 sub_80063af0(void)
{
    uint32 group, index, block, table, packet, value;
    FUNCTION_MARKER(0x80063AF0, "ACE3.BIN");
    table = arena_alloc(40);
    w_u32(0x800BD074, table);
    for (group = 0; group < 2; ++group)
    {
        block = arena_alloc(2516);
        table = r_u32(0x800BD074);
        w_u32(0x800BD078, block);
        w_u32(table + group * 4, block);
        for (index = 0; index < 16; ++index)
        {
            w_u8(block + index * 24 + 11, 5);
            w_u8(block + index * 24 + 15, 42);
        }
        block = r_u32(0x800BD078);
        for (index = 0; index < 4; ++index)
        {
            w_u8(block + index * 28 + 395, 6);
            w_u8(block + index * 28 + 399, 50);
        }
        block = r_u32(0x800BD078);
        for (index = 0; index < 12; ++index)
        {
            w_u8(block + index * 40 + 507, 9);
            w_u8(block + index * 40 + 511, 46);
        }
        for (index = 0; index < 64; ++index)
        {
            w_u8(r_u32(0x800BD078) + index * 16 + 987, 3);
            w_u8(r_u32(0x800BD078) + index * 16 + 991, 64);
            packet = r_u32(0x800BD078) + index * 16 + 984;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 8; ++index)
        {
            w_u8(r_u32(0x800BD078) + index * 20 + 2011, 4);
            w_u8(r_u32(0x800BD078) + index * 20 + 2015, 80);
            packet = r_u32(0x800BD078) + index * 20 + 2008;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        w_u8(r_u32(0x800BD078) + 2171, 9);
        w_u8(r_u32(0x800BD078) + 2175, 92);
        packet = r_u32(0x800BD078) + 2168;
        w_u8(packet + 23, 0);
        block = r_u32(0x800BD078);
        w_u32(packet + 36, 0x55555555);
        w_u8(block + 2199, 0);
        packet = r_u32(0x800BD078) + 2168;
        w_u8(packet + 7, r_u8(packet + 7) | 2);
        for (index = 0; index < 4; ++index)
        {
            packet = r_u32(0x800BD078) + 2388 + index * 32;
            w_u8(packet + 3, 7);
            w_u8(packet + 7, 36);
            value = r_u8(packet + 7);
            w_u8(packet + 4, 67);
            w_u8(packet + 5, 19);
            w_u16(packet + 22, 54);
            w_u16(packet + 14, 32664);
            w_u8(packet + 12, 138);
            w_u8(packet + 29, 53);
            w_u8(packet + 6, 0);
            w_u8(packet + 13, 33);
            w_u8(packet + 20, 157);
            w_u8(packet + 21, 33);
            w_u8(packet + 28, 157);
            w_u8(packet + 7, value | 2);
            if (index != 3)
            {
                value = r_u32(packet);
                w_u32(packet, (value & 0xFF000000) | ((packet + 32) & 0xFFFFFF));
            }
        }
        block = r_u32(0x800BD078);
        for (index = 0; index < 8; ++index)
        {
            w_u8(block + index * 20 + 2211, 4);
            w_u8(block + index * 20 + 2215, 102);
            w_u16(block + index * 20 + 2222, 32664);
        }
        w_u8(r_u32(0x800BD078) + 2371, 4);
        w_u8(r_u32(0x800BD078) + 2375, 100);
        w_u8(r_u32(0x800BD078) + 2380, 236);
        w_u8(r_u32(0x800BD078) + 2381, 140);
        block = r_u32(0x800BD078);
        value = r_u8(block + 2375);
        w_u16(block + 2386, 20);
        w_u16(block + 2384, 20);
        w_u8(block + 2375, value | 2);
        packet = r_u32(0x800BD078);
        w_u8(packet + 3, 1);
        w_u32(packet + 4, 0xE1000236);
    }
    return 0;
}

uint32 sub_8003c250(void)
{
    uint32 group, index, block, packet, table, uv_x, uv_y, first, second, value, other;
    FUNCTION_MARKER(0x8003C250, "ACE3.BIN");
    for (group = 0; group < 2; ++group)
    {
        block = r_u32(r_u32(0x800BCFA8) + group * 4);
        packet = block + 0xAA4;
        w_u32(0x800BCFAC, block);
        w_u8(packet + 7, 46);
        w_u8(packet + 4, 99);
        w_u8(packet + 5, 99);
        w_u8(packet + 6, 99);
        w_u16(packet + 22, 5);
        w_u8(packet + 3, 9);
        w_u16(packet + 14, 0x5714);
        for (index = 0; index < 8; ++index)
        {
            table = 0x800B108C + (index & 3) * 8;
            packet = r_u32(0x800BCFAC) + 0xACC + index * 40;
            w_u8(packet + 7, 46);
            w_u16(packet + 22, 54);
            w_u8(packet + 3, 9);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            w_u16(packet + 14, 0x7F99);
            w_u16(packet + 8, r_u16(table));
            w_u16(packet + 10, r_u16(table + 4));
            w_u16(packet + 16, r_u16(table) + 32);
            w_u16(packet + 18, r_u16(table + 4));
            w_u16(packet + 24, r_u16(table));
            w_u16(packet + 26, r_u16(table + 4) + 32);
            w_u16(packet + 32, r_u16(table) + 32);
            value = r_u16(table + 4);
            uv_x = 0x800B10AC + (index & 3) * 4;
            w_u16(packet + 34, value + 32);
            value = r_u8(uv_x);
            uv_y = 0x800B10BC + (index & 3) * 4;
            w_u8(packet + 12, value);
            w_u8(packet + 13, r_u8(uv_y));
            w_u8(packet + 20, r_u8(uv_x + 1));
            w_u8(packet + 21, r_u8(uv_y + 1));
            w_u8(packet + 28, r_u8(uv_x + 2));
            w_u8(packet + 29, r_u8(uv_y + 2));
            w_u8(packet + 36, r_u8(uv_x + 3));
            w_u8(packet + 37, r_u8(uv_y + 3));
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 3; ++index)
        {
            packet = block + index * 40;
            first = (block + 0xAF4 + index * 40) & 0xFFFFFF;
            second = (block + 0xB94 + index * 40) & 0xFFFFFF;
            value = r_u32(packet + 0xACC);
            other = r_u32(packet + 0xB6C);
            w_u32(packet + 0xACC, (value & 0xFF000000) | first);
            w_u32(packet + 0xB6C, (other & 0xFF000000) | second);
        }
        packet = r_u32(0x800BCFAC) + 0xC0C;
        w_u16(packet + 14, 0x7F18);
        w_u8(packet + 13, 55);
        w_u16(packet + 16, 80);
        w_u8(packet + 7, 100);
        w_u16(packet + 18, 73);
        w_u8(packet + 3, 4);
        w_u8(packet + 12, 0);
        w_u16(packet + 8, 6);
        w_u8(packet + 7, 102);
        block = r_u32(0x800BCFAC);
        w_u16(packet + 10, 12);
        packet = block + 0xC20;
        w_u16(packet + 14, 0x7F98);
        w_u8(packet + 7, 100);
        w_u8(packet + 13, 45);
        w_u8(packet + 3, 4);
        w_u8(packet + 12, 0);
        w_u16(packet + 18, 10);
        w_u16(packet + 8, 10);
        w_u8(packet + 7, 102);
        block = r_u32(0x800BCFAC);
        w_u16(packet + 16, 12);
        w_u16(packet + 10, 12);
        w_u8(block + 0xA97, 1);
        first = r_u32(0x800BCFAC);
        w_u8(first + 0xA9F, 1);
        second = r_u32(0x800BCFAC);
        w_u32(first + 0xA98, 0xE1000236);
        w_u32(second + 0xAA0, 0xE1000236);
        for (index = 0; index < 2; ++index)
        {
            packet = r_u32(0x800BCFAC) + 0xC34 + index * 16;
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 98);
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
            w_u16(packet + 8, r_u16(0x800B108C));
            value = r_u16(0x800B1090);
            w_u16(packet + 12, 64);
            w_u16(packet + 14, 64);
            w_u16(packet + 10, value);
        }
        block = r_u32(0x800BCFAC);
        w_u32(block + 0xA5C, 0xE6000000);
        w_u32(block + 0xA60, 0);
        w_u32(block + 0xA68, 0xE6000002);
        w_u32(block + 0xA6C, 0);
        w_u32(block + 0xA74, 0xE6000000);
        w_u32(block + 0xA78, 0);
        w_u32(block + 0xA80, 0xE6000002);
        w_u32(block + 0xA84, 0);
        w_u32(block + 0xA8C, 0xE6000000);
        w_u32(block + 0xA90, 0);
    }
    return 0;
}

uint32 sub_8003b924(void)
{
    static const uint16 uv_bytes[][2] = {{0xDF8, 112}, {0xDF9, 55}, {0xE00, 128}, {0xE01, 55}, {0xE08, 112}, {0xE09, 71}, {0xE10, 128}, {0xE11, 71}, {0xE20, 127}, {0xE21, 55}, {0xE28, 111}, {0xE29, 55}, {0xE30, 127}, {0xE31, 71}, {0xE38, 111}, {0xE39, 71}, {0xE48, 112}, {0xE49, 70}, {0xE50, 128}, {0xE51, 70}, {0xE58, 112}, {0xE59, 54}, {0xE60, 128}, {0xE61, 54}, {0xE70, 127}, {0xE71, 70}, {0xE78, 111}, {0xE79, 70}, {0xE80, 127}, {0xE81, 54}, {0xE88, 111}, {0xE89, 54}};
    static const uint16 first_coordinates[][2] = {{0xED6, 88}, {0xEDA, 88}, {0xEEE, 88}, {0xEF2, 88}, {0xED8, 128}, {0xEDC, 128}, {0xF08, 128}, {0xF0C, 128}, {0xED4, 136}, {0xEDE, 96}, {0xEEC, 184}, {0xEF0, 192}, {0xEF4, 192}, {0xEF6, 96}, {0xF04, 136}, {0xF06, 152}, {0xF0A, 152}, {0xF0E, 144}, {0xF1C, 184}, {0xF1E, 152}, {0xF20, 192}, {0xF22, 152}, {0xF24, 192}, {0xF26, 144}};
    static const uint16 second_coordinates[][2] = {{0xF38, 164}, {0xF48, 164}, {0xF58, 164}, {0xF68, 132}, {0xF34, 156}, {0xF44, 156}, {0xF54, 156}, {0xF74, 188}, {0xF46, 120}, {0xF4A, 120}, {0xF66, 120}, {0xF6A, 120}, {0xF76, 120}, {0xF7A, 120}, {0xF8A, 124}, {0xF9A, 124}, {0xFAA, 124}, {0xF36, 88}, {0xF3A, 88}, {0xF56, 152}, {0xF5A, 152}, {0xF64, 128}, {0xF78, 192}, {0xF84, 128}, {0xF86, 116}, {0xF88, 128}, {0xF94, 160}, {0xF96, 116}, {0xF98, 160}, {0xFA4, 192}, {0xFA6, 116}, {0xFA8, 192}, {0xFB4, 160}, {0xFBA, 92}, {0xFB6, 88}, {0xFB8, 160}, {0xFC4, 160}, {0xFC6, 152}, {0xFC8, 160}, {0xFCA, 148}};
    uint32 group, index, block, table, packet, other, first, second, third, value;
    PSX_RECT rect;
    FUNCTION_MARKER(0x8003B924, "ACE3.BIN");
    block = arena_alloc(28);
    w_u32(0x800BCFA8, block);
    for (group = 0; group < 2; ++group)
    {
        block = arena_alloc(4224);
        table = r_u32(0x800BCFA8);
        w_u32(0x800BCFAC, block);
        w_u32(table + group * 4, block);
        for (index = 0; index < 64; ++index)
        {
            packet = block + 31 + index * 20;
            w_u8(packet, 100);
            w_u8(packet - 4, 4);
            w_u16(packet + 7, 0x7F98);
            w_u8(packet, 102);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 64; ++index)
        {
            packet = block + 0x51F + index * 16;
            w_u8(packet - 4, 3);
            w_u8(packet, 98);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 8; ++index)
        {
            packet = block + 0x91F + index * 16;
            w_u8(packet, 64);
            w_u8(packet - 4, 3);
            w_u8(packet - 3, 0);
            w_u8(packet - 2, 128);
            w_u8(packet - 1, 0);
            w_u8(packet, 66);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 8; ++index)
        {
            packet = block + 0x99F + index * 24;
            w_u8(packet, 72);
            w_u8(packet - 4, 5);
            w_u32(packet + 13, 0x55555555);
            w_u8(packet - 3, 0);
            w_u8(packet - 2, 128);
            w_u8(packet - 1, 0);
            w_u8(packet, 74);
        }
        rect.x = 12;
        rect.y = group == 1 ? 270 : 14;
        rect.w = 64;
        rect.h = 64;
        SetDrawArea((DR_AREA *)psx_addr(r_u32(0x800BCFAC), 12), &rect);
        rect.w = 320;
        rect.x = 0;
        rect.y = group == 1 ? 256 : 0;
        rect.h = 240;
        SetDrawArea((DR_AREA *)psx_addr(r_u32(0x800BCFAC) + 12, 12), &rect);
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 2; ++index)
        {
            packet = block + 0xC5B + index * 16;
            w_u8(packet - 4, 3);
            w_u8(packet, 66);
        }
        for (index = 0; index < 3; ++index)
        {
            w_u8(r_u32(0x800BCFAC) + index * 20 + 0xC77, 4);
            w_u8(r_u32(0x800BCFAC) + index * 20 + 0xC7B, 32);
            packet = r_u32(0x800BCFAC) + index * 20 + 0xC74;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 7; ++index)
        {
            packet = block + 0xCF6 + index * 40;
            w_u16(packet - 12, 5);
            w_u8(packet - 31, 9);
            w_u8(packet - 27, 46);
            w_u8(packet - 30, 131);
            w_u8(packet - 29, 131);
            w_u8(packet - 28, 131);
            w_u16(packet - 20, 0x5714);
            w_u16(packet - 26, r_u8(0x800BCC2C + index));
            w_u16(packet - 24, r_u8(0x800BCC34 + index));
            first = r_u8(0x800BCC2C + index);
            second = r_u8(0x800BCC3C + index);
            w_u16(packet - 18, first + second);
            w_u16(packet - 16, r_u8(0x800BCC34 + index));
            w_u16(packet - 10, r_u8(0x800BCC2C + index));
            first = r_u8(0x800BCC34 + index);
            second = r_u8(0x800BCC44 + index);
            w_u16(packet - 8, first + second);
            first = r_u8(0x800BCC2C + index);
            second = r_u8(0x800BCC3C + index);
            w_u16(packet - 2, first + second);
            first = r_u8(0x800BCC34 + index);
            second = r_u8(0x800BCC44 + index);
            w_u16(packet, first + second);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0xE0E + index * 40;
            other = block + 0xE9A + index * 16;
            w_u8(packet - 31, 9);
            w_u8(packet - 27, 46);
            w_u8(packet - 30, 131);
            w_u8(packet - 29, 131);
            w_u8(packet - 28, 131);
            w_u16(packet - 12, 54);
            w_u16(packet - 20, 0x7F99);
            w_u16(packet - 26, r_u8(0x800BCC4C + index));
            w_u16(packet - 24, r_u8(0x800BCC50 + index));
            w_u16(packet - 18, r_u8(0x800BCC4C + index) + 16);
            w_u16(packet - 16, r_u8(0x800BCC50 + index));
            w_u16(packet - 10, r_u8(0x800BCC4C + index));
            w_u16(packet - 8, r_u8(0x800BCC50 + index) + 16);
            w_u16(packet - 2, r_u8(0x800BCC4C + index) + 16);
            w_u16(packet, r_u8(0x800BCC50 + index) + 16);
            w_u8(other - 11, 3);
            w_u8(other - 7, 98);
            w_u8(other - 10, 0);
            w_u8(other - 9, 0);
            w_u8(other - 8, 0);
            w_u16(other - 6, r_u8(0x800BCC4C + index));
            value = r_u8(0x800BCC50 + index);
            w_u16(other - 2, 16);
            w_u16(other - 4, value);
            w_u16(other, 16);
        }
        for (index = 0; index < sizeof(uv_bytes) / sizeof(uv_bytes[0]); ++index)
            w_u8(r_u32(0x800BCFAC) + uv_bytes[index][0], uv_bytes[index][1]);
        block = r_u32(0x800BCFAC);
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0xED2 + index * 24;
            w_u8(packet + 1, 72);
            w_u8(packet - 3, 5);
            w_u32(packet + 14, 0x55555555);
            w_u8(packet - 2, 0);
            w_u8(packet - 1, 255);
            w_u8(packet, 0);
            w_u8(packet + 1, 74);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < sizeof(first_coordinates) / sizeof(first_coordinates[0]); ++index)
            w_u16(block + first_coordinates[index][0], first_coordinates[index][1]);
        for (index = 0; index < 10; ++index)
        {
            packet = block + 0xF32 + index * 16;
            w_u8(packet - 3, 3);
            w_u8(packet + 1, 66);
            w_u8(packet - 2, 0);
            w_u8(packet - 1, 255);
            w_u8(packet, 0);
        }
        block = r_u32(0x800BCFAC);
        for (index = 0; index < sizeof(second_coordinates) / sizeof(second_coordinates[0]); ++index)
            w_u16(block + second_coordinates[index][0], second_coordinates[index][1]);
        for (index = 0; index < 2; ++index)
        {
            packet = block + 0xFD2 + index * 24;
            w_u8(packet - 3, 5);
            w_u8(packet + 1, 42);
            w_u8(packet - 2, 16);
            w_u8(packet - 1, 32);
            w_u8(packet, 16);
        }
        packet = r_u32(0x800BCFAC) + 0xFFC;
        w_u8(packet + 7, 102);
        w_u8(packet + 3, 4);
        w_u8(packet + 4, 131);
        w_u8(packet + 5, 131);
        w_u8(packet + 6, 131);
        block = r_u32(0x800BCFAC);
        w_u16(packet + 14, 0x7F98);
        w_u16(packet + 8, 156);
        w_u16(packet + 10, 116);
        w_u16(packet + 16, 8);
        w_u16(packet + 18, 9);
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0x1022 + index * 20;
            w_u8(packet - 15, 4);
            w_u8(packet - 11, 102);
            w_u8(packet - 14, 0);
            w_u8(packet - 13, 131);
            w_u8(packet - 12, 0);
            w_u16(packet - 4, 0x7F98);
            w_u16(packet - 10, r_u16(0x800BCC54 + index * 2));
            w_u16(packet - 8, r_u16(0x800BCC5C + index * 2));
            value = r_u8(0x800BCC64 + index);
            w_u8(packet - 5, 103);
            w_u16(packet - 2, 12);
            w_u16(packet, 7);
            w_u8(packet - 6, value);
        }
        w_u8(r_u32(0x800BCFAC) + 0x1063, 1);
        first = r_u32(0x800BCFAC);
        w_u8(first + 0x106B, 1);
        second = r_u32(0x800BCFAC);
        w_u32(first + 0x1064, 0xE1000205);
        w_u8(second + 0x1073, 1);
        third = r_u32(0x800BCFAC);
        w_u32(second + 0x106C, 0xE1000236);
        w_u8(third + 0x107B, 1);
        block = r_u32(0x800BCFAC);
        w_u32(third + 0x1074, 0xE1000236);
        w_u32(block + 0x107C, 0xE1000236);
        w_u32(block + 0xCB4, 0xE6000000);
        w_u32(block + 0xCB8, 0);
        w_u32(block + 0xCC0, 0xE6000002);
        w_u32(block + 0xCC4, 0);
        w_u32(block + 0xCCC, 0xE6000000);
        w_u32(block + 0xCD0, 0);
    }
    return sub_8003c250();
}

uint32 sub_8003ecbc(void)
{
    uint32 group, index, block, table, packet, other;
    FUNCTION_MARKER(0x8003ECBC, "ACE3.BIN");
    block = arena_alloc(24);
    w_u32(0x800BCFB8, block);
    for (group = 0; group < 2; ++group)
    {
        block = arena_alloc(5464);
        table = r_u32(0x800BCFB8);
        w_u32(0x800BCFBC, block);
        w_u32(table + group * 4, block);
        for (index = 0; index < 64; ++index)
        {
            w_u8(r_u32(0x800BCFBC) + index * 16 + 11, 3);
            w_u8(r_u32(0x800BCFBC) + index * 16 + 15, 64);
            w_u8(r_u32(0x800BCFBC) + index * 16 + 12, 0);
            w_u8(r_u32(0x800BCFBC) + index * 16 + 13, 255);
            w_u8(r_u32(0x800BCFBC) + index * 16 + 14, 0);
            packet = r_u32(0x800BCFBC) + index * 16 + 8;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 128; ++index)
        {
            w_u8(r_u32(0x800BCFBC) + index * 24 + 0x40B, 5);
            w_u8(r_u32(0x800BCFBC) + index * 24 + 0x40F, 72);
            packet = r_u32(0x800BCFBC) + index * 24 + 0x408;
            w_u8(packet + 4, 0);
            other = r_u32(0x800BCFBC);
            w_u32(packet + 20, 0x55555555);
            w_u8(other + index * 24 + 0x40D, 255);
            w_u8(r_u32(0x800BCFBC) + index * 24 + 0x40E, 0);
            packet = r_u32(0x800BCFBC) + index * 24 + 0x408;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 64; ++index)
        {
            w_u8(r_u32(0x800BCFBC) + index * 20 + 0x100B, 4);
            w_u8(r_u32(0x800BCFBC) + index * 20 + 0x100F, 100);
            packet = r_u32(0x800BCFBC) + index * 20;
            w_u16(packet + 0x1016, 0x7F9B);
            w_u8(packet + 0x100F, r_u8(packet + 0x100F) | 2);
        }
        for (index = 0; index < 4; ++index)
        {
            w_u8(r_u32(0x800BCFBC) + index * 20 + 0x150B, 4);
            w_u8(r_u32(0x800BCFBC) + index * 20 + 0x150F, 100);
            packet = r_u32(0x800BCFBC) + index * 20;
            w_u16(packet + 0x1516, 0x7F98);
            w_u8(packet + 0x150F, r_u8(packet + 0x150F) | 2);
        }
        w_u8(r_u32(0x800BCFBC) + 3, 1);
        w_u32(r_u32(0x800BCFBC) + 4, 0xE1000636);
    }
    sub_8003f284();
    return sub_800409ac();
}
