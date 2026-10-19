#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8002d544(void)
{
    FUNCTION_MARKER(0x8002D544, "ACE3.BIN");
    return (r_u32(r_u32(0x800BCE88) + 4) & 0xFFFFFF) != 0;
}

uint32 sub_8002d560(uint32 ot, uint32 packet, uint32 mode)
{
    uint32 id;
    uint32 descriptor;
    uint32 sprite;
    uint32 value;
    uint32 tag;
    uint32 head;

    FUNCTION_MARKER(0x8002D560, "ACE3.BIN");
    id = r_u32(r_u32(0x800BCE88));
    w_u8(packet + 3, 4);
    w_u8(packet + 7, 0x64);
    descriptor = 0x800B03AC + id * 20;
    value = r_u8(descriptor + 6);
    w_u8(packet + 7, value ? 0x66 : 0x64);
    w_u8(packet + 4, r_u8(descriptor + 3));
    w_u8(packet + 5, r_u8(descriptor + 4));
    w_u8(packet + 6, r_u8(descriptor + 5));
    if (!r_u32(r_u32(0x800BCE88)))
    {
        w_u16(packet + 8, 132);
        value = 56;
    }
    else
    {
        w_u16(packet + 8, r_u8(descriptor + 7) - 4);
        value = r_u16(descriptor + 8) - 6;
    }
    w_u16(packet + 10, value);
    sprite = 0x800B0488 + (mode << 2);
    w_u8(packet + 12, r_u8(sprite));
    w_u8(packet + 13, r_u8(sprite + 1));
    w_u16(packet + 16, r_u8(sprite + 2));
    value = r_u8(sprite + 3);
    w_u16(packet + 14, 0x7FD8);
    w_u16(packet + 18, value);
    tag = r_u32(packet);
    head = r_u32(ot);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
    w_u8(packet + 23, 1);
    w_u32(packet + 24, 0xE1000236);
    tag = r_u32(packet + 20);
    head = r_u32(ot);
    w_u32(packet + 20, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    w_u32(ot, (head & 0xFF000000) | ((packet + 20) & 0xFFFFFF));
    return packet + 28;
}

uint32 sound_set_voice_note(uint32 index, uint32 note)
{
    uint32 voice;
    uint32 flags;

    FUNCTION_MARKER(0x8002DE24, "ACE3.BIN");
    if (index >= 24)
        return 255;
    voice = 0x800C5830 + index * 20;
    flags = r_u8(voice + 1);
    w_u16(voice + 12, note);
    w_u8(voice + 1, flags | 0x10);
    return index;
}

uint32 resource_cancel_streams(void)
{
    uint32 context;
    uint32 other;
    uint32 sequence;
    uint32 request;
    uint32 kind;
    uint32 end;

    FUNCTION_MARKER(0x8002C71C, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    w_u8(context + 332, 5);
    other = r_u32(0x800BCBBC);
    w_u32(context + 328, 1);
    w_u8(other + 333, 0);
    context = r_u32(0x800BCBBC);
    w_u32(other + 348, 0);
    sequence = r_u32(context);
    end = r_u32(context + 4);
    while ((sint32)end >= (sint32)sequence)
    {
        request = context + (sequence & 15) * 20 + 8;
        kind = r_u8(request);
        if (kind - 3 < 2)
            w_u8(request, 0);
        context = r_u32(0x800BCBBC);
        end = r_u32(context + 4);
        ++sequence;
    }
    return sequence & 15;
}

uint32 sub_8007786c(uint32 state)
{
    uint32 flags;

    FUNCTION_MARKER(0x8007786C, "ACE3.BIN");
    flags = r_u32(state) | 0x400;
    w_u32(state, flags);
    return flags;
}

uint32 sub_800793c4(uint32 state, uint32 mode, uint32 descriptor)
{
    uint32 previous = 0xFFFFFFFF;
    uint32 flags;
    uint32 value;
    uint32 result;

    FUNCTION_MARKER(0x800793C4, "ACE3.BIN");
    sub_8007786c(state);
    if (mode & 0x401)
        previous = r_u32(descriptor) & 63;
    if (mode & 0xDFF)
    {
        flags = r_u32(state) & 0xFFFFFFC0;
        w_u32(state, flags);
        value = r_u32(descriptor);
        w_u32(state, flags | 0x100 | (value & 63));
        value = r_u32(descriptor);
        w_u32(state + 52, descriptor + 8);
        w_u32(state + 56, 0);
        w_u32(state + 60, 0);
        w_u32(state + 64, 0);
        w_u16(state + 72, 0);
        w_u16(state + 74, 0);
        w_u16(state + 76, 0);
        w_u32(state + 200, value & 63);
        result = r_u32(descriptor);
        w_u32(state + 196, previous);
        w_u32(state + 200, result & 63);
    }
    else
    {
        flags = r_u32(state);
        w_u32(state + 196, 0xFFFFFFFF);
        result = flags & 0xFFFFFEFF;
        w_u32(state, result);
    }
    w_u32(state + 80, mode);
    w_u32(state + 208, 0);
    w_u32(state + 204, 0);
    w_u16(state + 212, 0);
    w_u16(state + 214, 0);
    w_u16(state + 216, 0);
    w_u32(state + 220, 0);
    w_u32(state + 224, 0);
    w_u32(state + 228, 0);
    return result;
}

uint32 sub_80077880(uint32 state, uint32 projection, uint32 x, uint32 y, uint32 angles, const uint8 *native_angles)
{
    uint32 value;
    uint32 game;
    uint32 mode;
    uint32 index;
    uint32 table;
    uint32 descriptor;

    FUNCTION_MARKER(0x80077880, "ACE3.BIN");
    w_u16(state + 28, -160);
    w_u16(state + 30, -120);
    w_u32(state, 0);
    w_u16(state + 36, projection);
    w_u32(state + 196, 0);
    w_u32(state + 200, 0);
    w_u16(state + 32, 320);
    w_u16(state + 34, 240);
    w_u16(state + 40, x);
    w_u16(state + 42, y);
    w_u16(state + 44, (native_angles ? xport_load_le16(native_angles) : r_u16(angles)) << 6);
    w_u16(state + 46, (native_angles ? xport_load_le16(native_angles + 2) : r_u16(angles + 2)) << 6);
    value = native_angles ? xport_load_le16(native_angles + 4) : r_u16(angles + 4);
    w_u32(state + 8, 0xFFB50000);
    w_u32(state + 4, 0);
    w_u32(state + 12, 0);
    w_u16(state + 20, 0);
    w_u16(state + 22, 0);
    w_u16(state + 24, 0);
    w_u16(state + 180, 0);
    w_u16(state + 182, 0);
    w_u16(state + 184, 0);
    w_u32(state + 188, 0xFFFFFFFF);
    w_u32(state + 192, 0xFFFFFFFF);
    w_u32(state + 236, 0);
    w_u32(state + 240, 0);
    w_u16(state + 284, 0);
    w_u16(state + 286, 0);
    w_u16(state + 288, 0);
    w_u16(state + 48, value << 6);
    w_u32(state + 268, 0);
    game = r_u32(0x800BCE60);
    w_u32(state + 272, 0);
    w_u32(state + 276, 0);
    w_u16(state + 324, 0);
    w_u16(state + 326, 0);
    w_u16(state + 328, 0);
    w_u32(state + 308, 0);
    w_u32(state + 312, 0);
    w_u32(state + 316, 0);
    w_u32(state + 292, 0);
    w_u32(state + 296, 0);
    w_u32(state + 300, 0);
    w_u32(state + 332, 0);
    w_u32(state + 336, 0);
    w_u32(state + 340, 0);
    w_u16(state + 348, 0);
    w_u16(state + 350, 0);
    mode = r_u8(game + 927) ? 2 : 1;
    w_u32(state + 80, mode);
    index = r_u32(state + 200);
    table = r_u32(0x800BCE5C);
    mode = r_u32(state + 80);
    descriptor = r_u32(table + (index << 2));
    return sub_800793c4(state, mode, descriptor);
}

uint32 relocate_pointer_array(uint32 base, uint32 pointers, sint32 count)
{
    sint32 i;
    uint32 value;

    FUNCTION_MARKER(0x80027380, "ACE3.BIN");
    for (i = 0; i < count; ++i, pointers += 4)
    {
        value = r_u32(pointers);
        if (!(value & 0x80000000))
            w_u32(pointers, value + base);
    }
    return base;
}

uint32 rotation_build(uint32 angles, uint32 matrix, MATRIX *native_out, const uint16 *native_angles)
{
    uint32 ax;
    uint32 ay;
    uint32 az;
    uint32 cx;
    uint32 cy;
    uint32 cz;
    uint32 sx;
    uint32 sy;
    uint32 sz;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 d;
    uint32 e;
    uint32 f;
    uint32 g;
    uint32 h;
    uint32 p;
    uint32 q;
    uint32 r;
    uint32 result;

    FUNCTION_MARKER(0x80030778, "ACE3.BIN");
    ax = native_angles != 0 ? native_angles[0] : r_u16(angles);
    az = native_angles != 0 ? native_angles[2] : r_u16(angles + 4);
    ay = native_angles != 0 ? native_angles[1] : r_u16(angles + 2);
    ax &= 0x3FFF;
    az &= 0x3FFF;
    ay &= 0x3FFF;
    cx = (uint32)r_s16(0x800104AC + (ax + 0x1000) * 2);
    cz = (uint32)r_s16(0x800104AC + (az + 0x1000) * 2);
    sy = (uint32)r_s16(0x800104AC + ay * 2);
    sz = (uint32)r_s16(0x800104AC + az * 2);
    xport_gte_write_data(8, cx);
    xport_gte_write_data(9, sy);
    xport_gte_write_data(10, sz);
    xport_gte_write_data(11, cz);
    sx = (uint32)r_s16(0x800104AC + ax * 2);
    xport_gte_execute(0x190003D);
    cy = (uint32)r_s16(0x800104AC + (ay + 0x1000) * 2);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, cy);
    xport_gte_write_data(9, sz);
    xport_gte_write_data(10, cz);
    xport_gte_write_data(11, cx);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 4, (sint32)a >> 16);
    else
        w_u16(matrix + 4, (sint32)a >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 6, (sint32)b >> 16);
    else
        w_u16(matrix + 6, (sint32)b >> 16);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 8, (sint32)c >> 16);
    else
        w_u16(matrix + 8, (sint32)c >> 16);
    d = xport_gte_read_data(25);
    e = xport_gte_read_data(26);
    f = xport_gte_read_data(27);
    xport_gte_write_data(8, sy);
    xport_gte_write_data(9, cz);
    xport_gte_write_data(10, sz);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 16, (sint32)f >> 16);
    else
        w_u16(matrix + 16, (sint32)f >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 10, (sint32)(0 - sx) >> 2);
    else
        w_u16(matrix + 10, (sint32)(0 - sx) >> 2);
    g = xport_gte_read_data(25);
    h = xport_gte_read_data(26);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)g >> 14);
    xport_gte_write_data(10, (sint32)d >> 14);
    xport_gte_write_data(11, (sint32)e >> 14);
    xport_gte_execute(0x190003D);
    p = xport_gte_read_data(25);
    q = xport_gte_read_data(26);
    r = xport_gte_read_data(27);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)h >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 2, (sint32)(p - d) >> 16);
    else
        w_u16(matrix + 2, (sint32)(p - d) >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 12, (sint32)(q - g) >> 16);
    else
        w_u16(matrix + 12, (sint32)(q - g) >> 16);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 14, (sint32)(h + r) >> 16);
    else
        w_u16(matrix + 14, (sint32)(h + r) >> 16);
    result = xport_gte_read_data(25);
    if (native_out)
        xport_store_le16((uint8 *)native_out, (sint32)(e + result) >> 16);
    else
        w_u16(matrix, (sint32)(e + result) >> 16);
    return result;
}

uint32 copy_words(uint32 dst, uint32 src, sint32 count)
{
    sint32 i;
    uint32 value;
    FUNCTION_MARKER(0x8002759C, "ACE3.BIN");
    for (i = 0; i < count; ++i)
    {
        value = r_u32(src);
        src += 4;
        w_u32(dst, value);
        dst += 4;
    }
    return dst;
}

uint32 sub_80066a9c(uint32 factor, uint32 src0, uint32 src1, uint32 dst)
{
    uint32 row0 = src0 + 4, row1 = src1 + 4;
    uint32 tail0 = src0 + 20, tail1 = src1 + 20, mark = dst - 16;
    uint32 packed0, packed1, zpair0, zpair1, next1, third1;
    uint32 x, y, z;
    sint32 source_z, target_z, third_z, count;
    FUNCTION_MARKER(0x80066A9C, "ACE3.BIN");
    xport_gte_write_data(8, factor);
    count = r_s32(src0);
    w_u32(dst, count);
    do
    {
        packed0 = r_u32(row0);
        source_z = r_s16(tail0 - 4);
        zpair0 = r_u32(tail0 - 4);
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, source_z);
        packed1 = r_u32(row1);
        target_z = r_s16(tail1 - 4);
        zpair1 = r_u32(tail1 - 4);
        xport_gte_write_control(21, (sint16)packed1);
        xport_gte_write_control(22, (sint32)packed1 >> 16);
        xport_gte_write_control(23, target_z);
        xport_gte_execute(0x980011);
        dst += 20;
        mark += 20;
        next1 = r_u32(tail1 - 12);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        w_u16(mark, x);
        w_u16(dst - 14, y);
        w_u16(dst - 4, z);
        packed0 = r_u32(tail0 - 12);
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, (sint32)zpair0 >> 16);
        xport_gte_write_control(21, (sint16)next1);
        xport_gte_write_control(22, (sint32)next1 >> 16);
        xport_gte_write_control(23, (sint32)zpair1 >> 16);
        xport_gte_execute(0x980011);
        third1 = r_u32(tail1 - 8);
        third_z = r_s16(tail1);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        w_u16(dst - 12, x);
        w_u16(dst - 10, y);
        w_u16(dst - 2, z);
        packed0 = r_u32(tail0 - 8);
        source_z = r_s16(tail0);
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, source_z);
        xport_gte_write_control(21, (sint16)third1);
        xport_gte_write_control(22, (sint32)third1 >> 16);
        xport_gte_write_control(23, third_z);
        xport_gte_execute(0x980011);
        tail0 += 20;
        row0 += 20;
        tail1 += 20;
        row1 += 20;
        count = (sint32)((uint32)count - 3);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        w_u16(dst - 8, x);
        w_u16(dst - 6, y);
        w_u16(dst, z);
    } while (count > 0);
    return mark + 20;
}

uint32 sub_80066c80(uint32 entries, uint32 arena_context, uint8 *native_entries)
{
    uint32 field = entries + 16, result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
    uint32 code, fraction, next, dst, table, lo, hi, a1, a2, a3;
    sint32 level;
    FUNCTION_MARKER(0x80066C80, "ACE3.BIN");
    while (result != 0)
    {
        if ((native_entries ? native_entries[0] : r_u8(entries)) & 4)
        {
            code = (native_entries ? xport_load_le16(native_entries + (field - 14 - entries)) : r_u16(field - 14));
            level = (sint16)code >> 12;
            fraction = code & 0xFFF;
            if (level < 0)
                level = 0;
            if (level >= 5)
                level = 4;
            if (fraction != 0)
            {
                next = level < 4 ? level + 1 : 4;
                dst = r_u32(arena_context + 4);
                table = (native_entries ? xport_load_le32(native_entries + (field - 12 - entries)) : r_u32(field - 12));
                lo = r_u32(table + level * 4);
                hi = r_u32(table + next * 4);
                w_u32(arena_context + 4, dst + 36);
                copy_words(dst, lo, 9);
                w_u32(dst + 4, r_u32(arena_context + 4));
                a1 = r_u32(lo + 4);
                a2 = r_u32(hi + 4);
                a3 = r_u32(arena_context + 4);
                result = sub_80066a9c(fraction, a1, a2, a3);
                w_u32(arena_context + 4, result);
                if (native_entries)
                    xport_store_le32(native_entries + (field - entries), dst);
                else
                    w_u32(field, dst);
            }
            else
            {
                table = (native_entries ? xport_load_le32(native_entries + (field - 12 - entries)) : r_u32(field - 12));
                if (native_entries)
                    xport_store_le32(native_entries + (field - entries), r_u32(table + level * 4));
                else
                    w_u32(field, r_u32(table + level * 4));
            }
        }
        field += 20;
        result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
        entries += 20;
        if (native_entries)
            native_entries += 20;
    }
    return result;
}

uint32 sub_80066dcc(uint32 entries, uint32 arena_context, uint8 *native_entries)
{
    uint32 field = entries + 8, result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
    uint32 code, level, fraction, table, lo, hi, kind, offset, dst;
    FUNCTION_MARKER(0x80066DCC, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        if ((native_entries ? native_entries[0] : r_u8(entries)) & 4)
        {
            code = (native_entries ? xport_load_le16(native_entries + (field - 6 - entries)) : r_u16(field - 6));
            level = code >> 12;
            fraction = code & 0xFFF;
            if (level >= 5)
                level = 4;
            table = (native_entries ? xport_load_le32(native_entries + (field - 4 - entries)) : r_u32(field - 4));
            lo = r_u32(table + (level << 4));
            if (native_entries)
                xport_store_le32(native_entries + (field + 8 - entries), lo);
            else
                w_u32(field + 8, lo);
            if (fraction != 0)
            {
                level = level < 4 ? level + 1 : 4;
                table = (native_entries ? xport_load_le32(native_entries + (field - 4 - entries)) : r_u32(field - 4));
                kind = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
                hi = r_u32(table + (level << 4));
                offset = kind == 0 ? 4 : kind == 1 ? 8 : 12;
                lo = r_u32(lo + offset);
                hi = r_u32(hi + offset);
                if (native_entries)
                    xport_store_le32(native_entries + (field + 4 - entries), r_u32(arena_context + 4));
                else
                    w_u32(field + 4, r_u32(arena_context + 4));
                dst = r_u32(arena_context + 4);
                result = sub_80066a9c(fraction, lo, hi, dst);
                w_u32(arena_context + 4, result);
            }
        }
        field += 56;
        result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
        entries += 56;
        if (native_entries)
            native_entries += 56;
    }
    return result;
}

uint32 sub_80067088(uint32 entries, uint32 arena_context, uint8 *native_entries)
{
    uint32 field = entries + 72, result = (native_entries ? xport_load_le32(native_entries) : r_u32(entries)), descriptor, dst;
    sint32 count, mode;
    FUNCTION_MARKER(0x80067088, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        dst = r_u32(arena_context + 4);
        descriptor = (native_entries ? xport_load_le32(native_entries + (field + 80 - entries)) : r_u32(field + 80));
        if (native_entries)
            xport_store_le32(native_entries + (field - 60 - entries), dst);
        else
            w_u32(field - 60, dst);
        count = r_s16(descriptor);
        mode = r_s16(descriptor + 2);
        dst = r_u32(arena_context + 4);
        result = sub_80066ef8(count, mode, entries + 44, dst, arena_context + 16, native_entries ? native_entries + 44 : 0);
        w_u32(arena_context + 4, result);
        if (native_entries)
            xport_store_le32(native_entries + (field - 56 - entries), result);
        else
            w_u32(field - 56, result);
        count = r_s16(descriptor + 8);
        dst = r_u32(arena_context + 4);
        result = sub_8006701c(count, entries + 64, dst, native_entries ? native_entries + 64 : 0);
        w_u32(arena_context + 4, result);
        if (native_entries)
            xport_store_le32(native_entries + (field - 52 - entries), result);
        else
            w_u32(field - 52, result);
        count = r_s16(descriptor + 4);
        dst = r_u32(arena_context + 4);
        result = sub_8006704c(count, field, dst, native_entries ? native_entries + (field - entries) : 0);
        entries += 160;
        if (native_entries)
            native_entries += 160;
        w_u32(arena_context + 4, result);
        result = (native_entries ? xport_load_le32(native_entries) : r_u32(entries));
        field += 160;
    }
    return result;
}

uint32 sub_80066c18(uint32 factor, uint32 weight, uint32 base, uint32 first, uint32 second, uint32 dst, uint32 workspace)
{
    FUNCTION_MARKER(0x80066C18, "ACE3.BIN");
    sub_80066a9c(factor, first, second, workspace);
    return sub_80066a9c(weight, base, workspace, dst);
}

uint32 sub_80066ef8(sint32 count, sint32 mode, uint32 source, uint32 dst, uint32 workspace, const uint8 *native_source)
{
    uint32 base, first, second, total, product, factor;
    sint32 numerator, denominator, quotient, ratio, weight;
    FUNCTION_MARKER(0x80066EF8, "ACE3.BIN");
    base = (native_source ? xport_load_le32(native_source) : r_u32(source));
    if (count < 0)
    {
        first = (native_source ? xport_load_le32(native_source + 8) : r_u32(source + 8));
        count = (sint32)(0 - (uint32)count);
    }
    else
    {
        first = (native_source ? xport_load_le32(native_source + 4) : r_u32(source + 4));
    }
    if (mode < 0)
    {
        second = (native_source ? xport_load_le32(native_source + 16) : r_u32(source + 16));
        mode = (sint32)(0 - (uint32)mode);
    }
    else
    {
        second = (native_source ? xport_load_le32(native_source + 12) : r_u32(source + 12));
    }
    if (count == mode)
    {
        ratio = 0x800;
    }
    else
    {
        numerator = (sint32)((uint32)(mode < count ? mode : count) << 11);
        denominator = mode < count ? count : mode;
        if (denominator == 0)
            xport_mips_break(7);
        if (denominator == -1 && numerator == (sint32)0x80000000)
            xport_mips_break(6);
        quotient = (sint32)((sint64)numerator / denominator);
        ratio = mode < count ? quotient : (sint32)(0x1000 - (uint32)quotient);
    }
    total = (uint32)count + (uint32)mode;
    factor = ratio < 0x800 ? 0x1000 - (uint32)ratio : (uint32)ratio;
    product = total * factor;
    weight = (sint32)product >> 12;
    if (weight >= 0x1001)
        weight = 0x1000;
    return sub_80066c18(ratio, weight, base, first, second, dst, workspace);
}

uint32 sub_8006701c(sint32 count, uint32 source, uint32 dst, const uint8 *native_source)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8006701C, "ACE3.BIN");
    first = (native_source ? xport_load_le32(native_source) : r_u32(source));
    second = (native_source ? xport_load_le32(native_source + 4) : r_u32(source + 4));
    return sub_80066a9c(count, first, second, dst);
}

uint32 sub_8006704c(sint32 count, uint32 source, uint32 dst, const uint8 *native_source)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8006704C, "ACE3.BIN");
    if (count < 0)
    {
        second = (native_source ? xport_load_le32(native_source + 8) : r_u32(source + 8));
        count = (sint32)(0 - (uint32)count);
    }
    else
    {
        second = (native_source ? xport_load_le32(native_source + 4) : r_u32(source + 4));
    }
    first = (native_source ? xport_load_le32(native_source) : r_u32(source));
    return sub_80066a9c(count, first, second, dst);
}

uint32 sub_800626e0(uint32 entries, uint32 context, const uint8 *native_entries)
{
    uint32 result = entries, field = entries + 16, object;
    uint32 source, row, tail, out, xy0, xy1, xy2, z01, z2;
    sint32 x, y, z, count;
    FUNCTION_MARKER(0x800626E0, "ACE3.BIN");
    xport_gte_write_control(21, 0x830);
    xport_gte_write_control(22, 0x830);
    xport_gte_write_control(23, 0x830);
    object = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
    while (object != 0)
    {
        x = (native_entries ? (sint16)xport_load_le16(native_entries + (field - 8 - entries)) : r_s16(field - 8));
        y = (native_entries ? (sint16)xport_load_le16(native_entries + (field - 6 - entries)) : r_s16(field - 6));
        z = (native_entries ? (sint16)xport_load_le16(native_entries + (field - 4 - entries)) : r_s16(field - 4));
        xport_gte_write_control(5, x);
        xport_gte_write_control(6, y);
        xport_gte_write_control(7, z);
        source = r_u32(object + 4);
        out = context - 16;
        row = source + 4;
        tail = source + 20;
        count = r_s32(source);
        xy0 = r_u32(source + 4);
        xy1 = r_u32(row + 4);
        xy2 = r_u32(row + 8);
        z01 = r_u32(row + 12);
        z2 = r_u32(row + 16);
        do
        {
            xport_gte_write_data(0, xy0);
            xport_gte_write_data(2, xy1);
            xport_gte_write_data(4, xy2);
            xport_gte_write_data(1, z01);
            xport_gte_write_data(3, z01 >> 16);
            xport_gte_write_data(5, z2);
            tail += 20;
            row += 20;
            xport_gte_execute(0x280030);
            count = (sint32)((uint32)count - 3);
            out += 48;
            xy0 = r_u32(row);
            xy1 = r_u32(tail - 12);
            xy2 = r_u32(tail - 8);
            z01 = r_u32(tail - 4);
            z2 = r_u32(tail);
            w_u32(out, xport_gte_read_data(12));
            w_u32(out + 4, xport_gte_read_data(17));
            w_u32(out + 8, xport_gte_read_data(8));
            w_u32(out + 16, xport_gte_read_data(13));
            w_u32(out + 20, xport_gte_read_data(18));
            w_u32(out + 24, xport_gte_read_data(8));
            w_u32(out + 32, xport_gte_read_data(14));
            w_u32(out + 36, xport_gte_read_data(19));
            w_u32(out + 40, xport_gte_read_data(8));
        } while (count > 0);
        w_u32(context + 560, r_u32(object + 4) + 4);
        result = r_u32(object + 16);
        field += 20;
        w_u32(context + 8, result);
        sub_80061c3c(context);
        w_u32(context + 8, r_u32(object + 12));
        result = sub_800619e0(context);
        object = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
    }
    return result;
}

uint32 sub_80062bd4(uint32 entries, uint32 context, const uint8 *native_entries)
{
    uint32 field = entries + 8, result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
    uint32 c0, c1, c2, target, source, row, tail, out, depth;
    uint32 xy0, xy1, xy2, z01, z2;
    sint32 count;
    FUNCTION_MARKER(0x80062BD4, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        w_u32(context + 12, result);
        c0 = (native_entries ? xport_load_le32(native_entries + 20) : r_u32(entries + 20));
        c1 = (native_entries ? xport_load_le32(native_entries + 24) : r_u32(entries + 24));
        xport_gte_write_control(0, c0);
        xport_gte_write_control(1, c1);
        c0 = (native_entries ? xport_load_le32(native_entries + 28) : r_u32(entries + 28));
        c1 = (native_entries ? xport_load_le32(native_entries + 32) : r_u32(entries + 32));
        c2 = (native_entries ? xport_load_le16(native_entries + 36) : r_u32(entries + 36));
        xport_gte_write_control(2, c0);
        xport_gte_write_control(3, c1);
        xport_gte_write_control(4, c2);
        c0 = (native_entries ? xport_load_le32(native_entries + 40) : r_u32(entries + 40));
        c1 = (native_entries ? xport_load_le32(native_entries + 44) : r_u32(entries + 44));
        xport_gte_write_control(5, c0);
        c2 = (native_entries ? xport_load_le32(native_entries + 48) : r_u32(entries + 48));
        xport_gte_write_control(6, c1);
        xport_gte_write_control(7, c2);
        target = (native_entries ? xport_load_le32(native_entries + (field + 44 - entries)) : r_u32(field + 44));
        if (target != 0)
            xport_guest_call3(target, context, 0, 0);
        out = context + 4;
        source = (native_entries ? xport_load_le32(native_entries + (field + 4 - entries)) : r_u32(field + 4));
        depth = context + 670;
        row = source + 4;
        tail = source + 20;
        count = r_s32(source);
        xy0 = r_u32(source + 4);
        xy1 = r_u32(row + 4);
        xy2 = r_u32(row + 8);
        z01 = r_u32(row + 12);
        z2 = r_u32(row + 16);
        do
        {
            xport_gte_write_data(0, xy0);
            xport_gte_write_data(2, xy1);
            xport_gte_write_data(4, xy2);
            xport_gte_write_data(1, z01);
            xport_gte_write_data(3, z01 >> 16);
            xport_gte_write_data(5, z2);
            tail += 20;
            row += 20;
            xport_gte_execute(0x280030);
            count = (sint32)((uint32)count - 3);
            out += 12;
            depth += 6;
            xy0 = r_u32(row);
            xy1 = r_u32(tail - 12);
            xy2 = r_u32(tail - 8);
            z01 = r_u32(tail - 4);
            z2 = r_u32(tail);
            w_u32(out, xport_gte_read_data(12));
            w_u32(out + 4, xport_gte_read_data(13));
            w_u32(out + 8, xport_gte_read_data(14));
            w_u16(depth, xport_gte_read_data(17));
            w_u16(depth + 2, xport_gte_read_data(18));
            w_u16(depth + 4, xport_gte_read_data(19));
        } while (count > 0);
        w_u32(context + 8, r_u32((native_entries ? xport_load_le32(native_entries + (field + 8 - entries)) : r_u32(field + 8)) + 20));
        sub_80062d68(context);
        result = r_u32((native_entries ? xport_load_le32(native_entries + (field + 8 - entries)) : r_u32(field + 8)) + 24);
        field += 56;
        w_u32(context + 8, result);
        sub_80062ec8(context);
        result = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
        entries += 56;
        if (native_entries)
            native_entries += 56;
    }
    return result;
}

uint32 sub_80062d68(uint32 context)
{
    uint32 source, packet, table, data, tail, indices, i0, i1, i2;
    uint32 result, value0, value1, value2, ot, shift;
    sint32 count, i, z0, z1, z2, distance;
    FUNCTION_MARKER(0x80062D68, "ACE3.BIN");
    source = r_u32(context + 8);
    packet = r_u32(context + 4);
    count = r_s32(source);
    table = r_u32(context);
    result = source;
    data = source + 32;
    tail = packet + 36;
    for (i = 0; i < count; ++i)
    {
        indices = r_u32(data - 24);
        i0 = indices & 255;
        i1 = (indices >> 8) & 255;
        i2 = indices >> 16;
        xport_gte_write_data(12, r_u32(context + 16 + i0 * 4));
        xport_gte_write_data(13, r_u32(context + 16 + i1 * 4));
        xport_gte_write_data(14, r_u32(context + 16 + i2 * 4));
        xport_gte_execute(0x1400006);
        if ((sint32)xport_gte_read_data(24) > 0)
        {
            z0 = r_s16(context + 676 + i0 * 2);
            z1 = r_s16(context + 676 + i1 * 2);
            z2 = r_s16(context + 676 + i2 * 2);
            xport_gte_write_data(17, z0);
            xport_gte_write_data(18, z1);
            xport_gte_write_data(19, z2);
            xport_gte_execute(0x158002D);
            distance = (sint32)xport_gte_read_data(24);
            shift = r_u32(context + 12) + 10;
            distance >>= shift & 31;
            if ((uint32)distance - 31 < 0x77D9)
            {
                ot = table + (uint32)(distance >> 3) * 4;
                w_u32(packet, r_u32(ot) | 0x9000000);
                w_u32(ot, packet & 0xFFFFFF);
                value0 = r_u32(data - 20);
                value1 = r_u32(data - 8);
                value2 = r_u32(data - 16);
                w_u32(tail - 32, value0);
                w_u32(packet + 8, xport_gte_read_data(12));
                w_u32(tail - 24, value1);
                w_u32(tail - 20, value2);
                value0 = r_u32(data - 4);
                value1 = r_u32(data - 12);
                value2 = r_u32(data);
                w_u32(packet + 20, xport_gte_read_data(13));
                w_u32(tail - 12, value0);
                w_u32(tail - 8, value1);
                w_u32(packet + 32, xport_gte_read_data(14));
                w_u32(tail, value2);
                tail += 40;
                packet += 40;
            }
        }
        result = i + 1 < count;
        data += 32;
    }
    w_u32(context + 4, packet);
    return result;
}

uint32 sub_80062ec8(uint32 context)
{
    uint32 source, packet, table, data, tail, indices, i0, i1, i2, i3;
    uint32 result, first_xy, fourth_xy, value0, value1, value2, ot, shift;
    sint32 count, i, z0, z1, z2, z3, distance, area;
    FUNCTION_MARKER(0x80062EC8, "ACE3.BIN");
    source = r_u32(context + 8);
    packet = r_u32(context + 4);
    count = r_s32(source);
    table = r_u32(context);
    result = source;
    data = source + 24;
    tail = packet + 48;
    for (i = 0; i < count; ++i)
    {
        indices = r_u32(data - 16);
        i0 = indices & 255;
        i1 = (indices >> 8) & 255;
        i2 = (indices >> 16) & 255;
        i3 = indices >> 24;
        first_xy = r_u32(context + 16 + i0 * 4);
        xport_gte_write_data(13, r_u32(context + 16 + i1 * 4));
        xport_gte_write_data(14, r_u32(context + 16 + i2 * 4));
        xport_gte_write_data(12, first_xy);
        fourth_xy = r_u32(context + 16 + i3 * 4);
        xport_gte_execute(0x1400006);
        area = (sint32)xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        if (area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = (sint32)xport_gte_read_data(24);
            if (area >= 0)
                goto next_quad;
        }
        z0 = r_s16(context + 676 + i0 * 2);
        z1 = r_s16(context + 676 + i1 * 2);
        z2 = r_s16(context + 676 + i2 * 2);
        z3 = r_s16(context + 676 + i3 * 2);
        shift = r_u32(context + 12);
        distance = (z0 + z1 + z2 + z3) >> 2;
        distance >>= shift & 31;
        if ((uint32)distance - 31 < 0x77D9)
        {
            ot = table + (uint32)(distance >> 3) * 4;
            w_u32(packet, r_u32(ot) | 0xC000000);
            w_u32(ot, packet & 0xFFFFFF);
            value0 = r_u32(data - 12);
            value1 = r_u32(data + 4);
            w_u32(tail - 40, first_xy);
            w_u32(tail - 44, value0);
            w_u32(tail - 36, value1);
            value0 = r_u32(data - 8);
            value1 = r_u32(data + 8);
            value2 = r_u32(data - 4);
            w_u32(tail - 32, value0);
            w_u32(packet + 20, xport_gte_read_data(13));
            w_u32(tail - 24, value1);
            w_u32(tail - 20, value2);
            w_u32(packet + 32, xport_gte_read_data(14));
            value0 = r_u32(data + 12);
            value1 = r_u32(data);
            w_u32(tail - 12, value0);
            w_u32(tail - 8, value1);
            w_u32(packet + 44, xport_gte_read_data(12));
            w_u32(tail, value0 >> 16);
            tail += 52;
            packet += 52;
        }
    next_quad:
        result = i + 1 < count;
        data += 36;
    }
    w_u32(context + 4, packet);
    return result;
}

uint32 sub_800637b0(uint32 entries, uint32 context, const uint8 *native_entries)
{
    uint32 result = (native_entries ? xport_load_le32(native_entries) : r_u32(entries)), field = entries + 156;
    uint32 packet, ot, old, depth, c0, c1, c2, target, part;
    uint32 source, object, row, out, z_out, z01, effect, factor;
    sint32 i, count, light, f0, f1, f2;
    FUNCTION_MARKER(0x800637B0, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        packet = r_u32(context + 4);
        w_u32(context + 4, packet + 156);
        ClearOTagR(psx_addr(packet, 156), 39);
        depth = (native_entries ? xport_load_le32(native_entries + (field - 72 - entries)) : r_u32(field - 72));
        w_u16(context + 1014, depth);
        if (depth - 31 < 0x77D9)
        {
            ot = r_u32(context) + (uint32)((sint32)depth >> 3) * 4;
            old = r_u32(packet);
            old = (old & 0xFF000000) | (r_u32(ot) & 0xFFFFFF);
            w_u32(packet, old);
            old = r_u32(ot);
            w_u32(ot, (old & 0xFF000000) | ((packet + 152) & 0xFFFFFF));
        }
        result = (native_entries ? xport_load_le32(native_entries) : r_u32(entries));
        w_u32(context + 1016, packet + 76);
        w_u32(context + 12, result);
        c0 = (native_entries ? xport_load_le32(native_entries + 88) : r_u32(entries + 88));
        c1 = (native_entries ? xport_load_le32(native_entries + 92) : r_u32(entries + 92));
        xport_gte_write_control(0, c0);
        xport_gte_write_control(1, c1);
        c0 = (native_entries ? xport_load_le32(native_entries + 96) : r_u32(entries + 96));
        c1 = (native_entries ? xport_load_le32(native_entries + 100) : r_u32(entries + 100));
        c2 = (native_entries ? xport_load_le16(native_entries + 104) : r_u32(entries + 104));
        xport_gte_write_control(2, c0);
        xport_gte_write_control(3, c1);
        xport_gte_write_control(4, c2);
        c0 = (native_entries ? xport_load_le32(native_entries + 108) : r_u32(entries + 108));
        c1 = (native_entries ? xport_load_le32(native_entries + 112) : r_u32(entries + 112));
        xport_gte_write_control(5, c0);
        c2 = (native_entries ? xport_load_le32(native_entries + 116) : r_u32(entries + 116));
        xport_gte_write_control(6, c1);
        xport_gte_write_control(7, c2);
        c0 = (native_entries ? xport_load_le32(native_entries + 120) : r_u32(entries + 120));
        c1 = (native_entries ? xport_load_le32(native_entries + 124) : r_u32(entries + 124));
        xport_gte_write_control(8, c0);
        xport_gte_write_control(9, c1);
        c0 = (native_entries ? xport_load_le32(native_entries + 128) : r_u32(entries + 128));
        c1 = (native_entries ? xport_load_le32(native_entries + 132) : r_u32(entries + 132));
        c2 = (native_entries ? xport_load_le16(native_entries + 136) : r_u32(entries + 136));
        xport_gte_write_control(10, c0);
        xport_gte_write_control(11, c1);
        xport_gte_write_control(12, c2);
        effect = (native_entries ? xport_load_le32(native_entries + (field - 4 - entries)) : r_u32(field - 4));
        w_u32(context + 1020, effect);
        if (effect != 0)
        {
            target = (native_entries ? xport_load_le32(native_entries + (field - entries)) : r_u32(field));
            if (target != 0)
                xport_guest_call3(target, context, 0, 0);
        }
        part = entries;
        for (i = 0; i < 5; ++i)
        {
            out = context + 4;
            z_out = context + 670;
            source = (native_entries ? xport_load_le32(native_entries + (part + 4 - entries)) : r_u32(part + 4));
            object = (native_entries ? xport_load_le32(native_entries + (part + 24 - entries)) : r_u32(part + 24));
            row = source + 4;
            count = r_s32(source);
            do
            {
                xport_gte_write_data(0, r_u32(row));
                xport_gte_write_data(2, r_u32(row + 4));
                xport_gte_write_data(4, r_u32(row + 8));
                z01 = r_u32(row + 12);
                xport_gte_write_data(5, r_u32(row + 16));
                xport_gte_write_data(1, z01);
                xport_gte_write_data(3, (sint32)z01 >> 16);
                xport_gte_execute(0x280030);
                row += 20;
                count = (sint32)((uint32)count - 3);
                out += 12;
                z_out += 6;
                w_u32(out, xport_gte_read_data(12));
                w_u32(out + 4, xport_gte_read_data(13));
                w_u32(out + 8, xport_gte_read_data(14));
                w_u16(z_out, xport_gte_read_data(17));
                w_u16(z_out + 2, xport_gte_read_data(18));
                w_u16(z_out + 4, xport_gte_read_data(19));
            } while (count > 0);
            if (i != 0)
            {
                w_u32(context + 1008, r_u32(object + 16));
                w_u32(context + 8, r_u32(object + 20));
                sub_800633d8(context);
                w_u32(context + 8, r_u32(object + 24));
                sub_8006358c(context);
            }
            else
            {
                f0 = (sint32)xport_gte_read_control(13);
                f1 = (sint32)xport_gte_read_control(14);
                f2 = (sint32)xport_gte_read_control(15);
                xport_gte_write_control(21, f0 >> 2);
                xport_gte_write_control(22, f1 >> 2);
                xport_gte_write_control(23, f2 >> 2);
                effect = r_u32(context + 1020);
                factor = 0xE00;
                if (effect != 0)
                {
                    light = (sint16)r_u16(effect + 6);
                    if (light < 0x800)
                    {
                        factor = 0x1000 - (uint32)(light >> 2);
                    }
                    else
                    {
                        light -= 0x800;
                        factor = 0xE00 - (((uint32)light << 1) - (uint32)(light >> 2));
                    }
                    if (r_u16(context + 1012) & 1)
                        factor = factor + 8 + (uint32)((sint32)factor >> 5);
                }
                xport_gte_write_data(8, factor);
                w_u32(context + 8, r_u32(object + 20));
                sub_8006305c(context);
                w_u32(context + 8, r_u32(object + 24));
                sub_800631f8(context);
            }
            part += 4;
        }
        entries += 160;
        if (native_entries)
            native_entries += 160;
        result = (native_entries ? xport_load_le32(native_entries) : r_u32(entries));
        field += 160;
    }
    return result;
}

uint32 sub_800358f8(void)
{
    uint32 game, slot, cursor, context, state, value;
    sint32 index;
    FUNCTION_MARKER(0x800358F8, "ACE3.BIN");
    resource_init_queue(0, 0);
    sound_disable_reverb();
    if (r_u8(r_u32(0x800BCE60) + 933) != 0)
        sub_80036130();
    w_u8(r_u32(0x800BCE60) + 2391, 0);
    game = r_u32(0x800BCE60);
    index = r_s16(game + 908);
    w_u16(game + 2392, 0);
    w_u16(game + 940, 0);
    context = r_u32(0x800BCE50);
    value = r_s16(0x800B0B08 + ((uint32)index << 1));
    w_u32(context + 96, value);
    value = resource_total_size(context + 96, 1);
    cursor = arena_alloc(value);
    slot = r_u32(0x800BCE50) + 96;
    state = r_u32(0x800BCBE0);
    w_u32(state + 8, 1);
    resource_queue_list(slot, 1, cursor, 0x800357B4);
    context = r_u32(0x800BCE48);
    state = r_u32(0x800BCBE0);
    value = r_u32(context);
    w_u32(context + 4, 0);
    w_u32(context + 20, 0);
    w_u32(context, value | 1);
    w_u16(state, 0);
    w_u8(state + 4, r_u8(0x800B0C64 + (uint32)index));
    if (r_u8(r_u32(0x800BCE60) + 933) != 0)
        w_u8(r_u32(0x800BCBE0) + 4, 1);
    w_u8(r_u32(0x800BCBE0) + 6, 0);
    w_u16(r_u32(0x800BCBE0) + 2, 0);
    sub_800357c0();
    return 0x4000000;
}

uint32 sub_800357c0(void)
{
    uint32 current, game, state, value, mode = 0;
    sint32 index;
    FUNCTION_MARKER(0x800357C0, "ACE3.BIN");
    current = sound_get_cur_resource();
    game = r_u32(0x800BCE60);
    index = r_s16(game + 908);
    value = r_s16(0x800B0B08 + ((uint32)index << 1));
    if (current == value)
    {
        mode = 1;
    }
    else
    {
        index = r_s16(game + 910);
        value = r_s16(0x800B0B84 + ((uint32)index << 1));
        if (current == value)
            mode = 2;
    }
    if (mode != 0)
    {
        w_u8(r_u32(0x800BCBE0) + 6, mode);
        value = resource_read_progress(0, 0);
        w_u16(r_u32(0x800BCBE0) + 2, value);
    }
    if (r_s16(r_u32(0x800BCBE0)) <= 0)
        sub_80037070();
    value = r_u16(r_u32(0x800BCBE0));
    if (value - 1 < 2)
    {
        game = r_u32(0x800BCE60);
        current = r_u32(game + 12);
        value = r_u32(game + 20);
        value = sub_800362a0(current, value, 0);
        w_u32(r_u32(0x800BCE60) + 20, value);
    }
    if (r_s16(r_u32(0x800BCBE0)) >= 3)
        sub_80037108();
    state = r_u32(0x800BCBE0);
    value = r_u32(state + 8);
    current = r_u16(state);
    w_u16(state, current + 1);
    return (value == 0) << 26;
}

uint32 sub_80037070(void)
{
    uint32 mode, game, value, arg0, arg1;
    FUNCTION_MARKER(0x80037070, "ACE3.BIN");
    w_u8(r_u32(0x800BCBE0) + 5, 0);
    mode = r_u8(r_u32(0x800BCBE0) + 4);
    if (mode < 2)
        return sub_800369c8();
    if (mode != 2)
        return 2;
    game = r_u32(0x800BCE60);
    arg0 = r_u32(game + 12);
    arg1 = r_u32(game + 20);
    value = sub_80036440(arg0, arg1);
    w_u32(r_u32(0x800BCE60) + 20, value);
    return value;
}

uint32 sub_80037108(void)
{
    uint32 mode, game, value, arg0, arg1;
    FUNCTION_MARKER(0x80037108, "ACE3.BIN");
    mode = r_u8(r_u32(0x800BCBE0) + 4);
    if (mode > 2)
        return 2;
    game = r_u32(0x800BCE60);
    arg0 = r_u32(game + 8);
    arg1 = r_u32(game + 20);
    if (mode == 0)
        value = sub_80036a84(arg0, arg1);
    else if (mode == 1)
        value = sub_80036e84(arg0, arg1);
    else
        value = sub_800364c4(arg0, arg1);
    w_u32(r_u32(0x800BCE60) + 20, value);
    return value;
}

uint32 sub_800369c8(void)
{
    uint32 context, value, state, other;
    FUNCTION_MARKER(0x800369C8, "ACE3.BIN");
    context = r_u32(0x800BCE48);
    value = r_u32(context);
    w_u8(context + 16, 0);
    w_u32(context, value | 8);
    w_u8(r_u32(0x800BCE48) + 17, 12);
    w_u8(r_u32(0x800BCE48) + 18, 0);
    state = r_u32(0x800BCBE0);
    w_u32(state + 16, 0xF8000000);
    w_u32(state + 20, 0xA00000);
    w_u32(state + 28, 199);
    w_u32(state + 32, 4);
    w_u8(state + 40, 0);
    other = r_u32(0x800BCBE0);
    w_u32(state + 56, 0);
    w_u32(state + 12, 0);
    w_u32(state + 24, 0);
    w_u32(state + 36, 0);
    w_u32(state + 52, 0x2C3B2);
    w_u16(state + 50, 0);
    w_u8(other + 41, 255);
    w_u8(r_u32(0x800BCBE0) + 42, 0);
    w_u8(r_u32(0x800BCBE0) + 44, 0);
    w_u8(r_u32(0x800BCBE0) + 45, 0x83);
    w_u8(r_u32(0x800BCBE0) + 46, 0);
    state = r_u32(0x800BCBE0);
    w_u16(state + 48, 0);
    return state;
}

uint32 sub_800362a0(uint32 object, uint32 dst, uint32 mode)
{
    uint32 a = dst + 12, b = dst + 20, color = mode;
    uint32 command, last_tag, old, address, lane, i, value;
    FUNCTION_MARKER(0x800362A0, "ACE3.BIN");
    w_u32(dst, (a & 0xFFFFFF) | 0x2000000);
    w_u32(dst + 4, 0xE6000001);
    w_u32(dst + 8, 0);
    if ((sint32)mode < 0)
    {
        color = 0 - mode;
        command = 0xE1000240;
    }
    else
    {
        command = 0xE1000220;
    }
    w_u32(dst + 12, (b & 0xFFFFFF) | 0x1000000);
    w_u32(a + 4, command);
    w_u32(b, 0x3000000);
    w_u32(b + 4, color | (color << 8) | (color << 16) | 0x62000000);
    w_u16(b + 12, 320);
    w_u16(b + 14, 240);
    w_u32(b + 16, 0x2000000);
    w_u16(b + 8, 0);
    w_u16(b + 10, 0);
    w_u32(b + 20, 0xE6000000);
    w_u32(b + 24, 0);
    last_tag = r_u32(b + 16);
    old = r_u32(object);
    last_tag = (last_tag & 0xFF000000) | (old & 0xFFFFFF);
    w_u32(b + 16, last_tag);
    old = r_u32(object);
    w_u32(object, (old & 0xFF000000) | ((b + 16) & 0xFFFFFF));
    /* Preserve the LWL and SWL lane masks */
    address = object + 2;
    lane = address & 3;
    value = (last_tag & (0xFFFFFF >> (lane * 8))) | (r_u32(address & ~3) << ((3 - lane) * 8));
    address = b + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    value = dst << 8;
    address = object + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    return b + 28;
}

uint32 sub_80036a84(uint32 object, uint32 dst)
{
    uint32 state, mode, inverse, value, flags, other, scale, color, result;
    sint32 progress, angle, sine, phase;
    PSX_RECT rect;
    FUNCTION_MARKER(0x80036A84, "ACE3.BIN");
    state = r_u32(0x800BCBE0);
    mode = r_u8(state + 6);
    progress = mode == 1 ? r_s16(state + 2) : (mode == 2) << 12;
    dst = sub_80036824(object, dst);
    if (progress >= 0x1001)
        progress = 0x1000;
    inverse = 0x1000 - (uint32)progress;
    state = r_u32(0x800BCBE0);
    value = (uint32)progress * 255;
    w_u8(state + 41, (sint32)value / 4096);
    state = r_u32(0x800BCBE0);
    value = (uint32)progress * 131;
    w_u8(state + 45, (sint32)value / 4096);
    state = r_u32(0x800BCBE0);
    if (r_s32(state + 56) < 0xA00)
        w_u32(state + 12, 0);
    if (progress >= 0x401)
    {
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        w_u32(state + 12, flags | 0x80);
    }
    if (progress >= 0x501)
    {
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        w_u32(state + 20, inverse * 2560);
        w_u32(state + 12, flags | 1);
    }
    if (progress >= 0x601)
    {
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        value = 0 - (inverse << 15);
        w_u32(state + 16, value);
        w_u32(state + 12, flags | 2);
        if ((sint32)value > 0)
            w_u32(state + 16, 0);
    }
    if (progress >= 0x701)
    {
        value = inverse * 181170;
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        w_u32(state + 12, flags | 4);
        w_u32(state + 52, (sint32)value / 4096);
    }
    if (progress >= 0x801)
    {
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        w_u32(state + 12, flags | 0x40);
    }
    if (progress >= 0x901)
    {
        state = r_u32(0x800BCBE0);
        flags = r_u32(state + 12);
        w_u32(state + 24, progress);
        w_u32(state + 12, flags | 0x18);
        w_u32(state + 28, (sint32)(inverse * 199) / 4096);
        w_u32(state + 32, (sint32)inverse / 1024);
        other = r_u32(state + 32);
        value = progress % 5;
        w_u32(state + 36, value);
        if ((sint32)other < (sint32)value)
            w_u32(state + 36, other);
    }
    if (r_u8(r_u32(0x800BCE60) + 926) == 0)
        dst = sub_8005e40c(r_u32(0x800BCBE0) + 12, object, dst);
    angle = (sint32)((uint32)progress << 12) / 768;
    if (angle >= 0x1001)
        angle = 0x1000;
    sine = (sint16)r_u16(0x800104AC + ((uint32)angle & 0x3FFF) * 2) >> 2;
    rect.w = (sint16)((sint32)((uint32)sine * 1280) / 4096);
    if (rect.w >= 321)
        rect.w = 320;
    rect.h = (sint16)((sint32)((uint32)sine * 240) / 4096);
    rect.y = (240 - rect.h) >> 1;
    rect.x = (320 - rect.w) >> 1;
    dst = sub_800847a8(object, dst, 0, inverse << 1, &rect);
    phase = (sint32)((uint32)r_s16(r_u32(0x800BCBE0)) << 6);
    if (phase >= 0x1001)
        phase = 0x1000;
    scale = 0x1000 - (uint32)phase;
    color = ((scale * 131) >> 4) & 0xFF00;
    result = sub_800365fc(object, dst, phase, color, (color >> 1) & 0xFF00);
    state = r_u32(0x800BCBE0);
    value = r_u32(state + 56);
    w_u32(state + 56, value + 1);
    return result;
}

uint32 sub_80036824(uint32 object, uint32 dst)
{
    uint32 game, state, mode, text, length, cursor, ch, color;
    sint32 index, x, value;
    FUNCTION_MARKER(0x80036824, "ACE3.BIN");
    if (r_s16(r_u32(0x800BCBE0)) < 40)
    {
        game = r_u32(0x800BCE60);
        value = r_u32(game + 916) & 3;
        text = r_u32(0x800B0D90 + value * 4);
        dst = sub_8003563c(object, dst, 190, 18, 0x4000, text, 0);
    }
    state = r_u32(0x800BCBE0);
    mode = r_u8(state + 6);
    if ((mode == 1 && r_s16(state + 2) > 0) || mode == 2)
    {
        game = r_u32(0x800BCE60);
        index = r_s16(game + 908);
        text = r_u32(0x800B0CA0 + ((uint32)index << 2));
        length = 0;
        ch = r_u8(text);
        cursor = text + 1;
        while (ch != 0)
        {
            ++length;
            ch = r_u8(cursor);
            ++cursor;
        }
        x = (sint32)(320 - (length << 3)) >> 1;
        state = r_u32(0x800BCBE0);
        mode = r_u8(state + 6);
        value = r_s16(state + 2);
        if (mode != 1)
            value = (sint32)(0x1000 - (uint32)value);
        color = ((uint32)(value / 32) & 255) << 8;
        dst = sub_8003563c(object, dst, x, 118, color, text, 0);
        if (r_u32(r_u32(0x800BCE60) + 32) & 0x10)
            dst = sub_8003563c(object, dst, (uint32)x - 12, 118, color, 0x800BCC0C, 0);
    }
    return dst;
}

uint32 sub_8003563c(uint32 object, uint32 dst, uint32 x, uint32 y, uint32 color, uint32 text, const uint8 *native_text)
{
    uint32 packet = dst, field, ch, code, uv, tag;
    FUNCTION_MARKER(0x8003563C, "ACE3.BIN");
    ch = native_text ? *native_text++ : r_u8(text);
    ++text;
    if (ch != 0)
    {
        color |= 0x76000000;
        field = dst + 10;
        do
        {
            code = ch - 32;
            if (code != 0)
            {
                uv = ((code & 15) * 8 + 128) | (((uint32)((sint32)code >> 4) << 11) + 0x4700) | 0x7F980000;
                w_u32(field - 6, color);
                w_u32(field + 2, uv);
                w_u16(field - 2, x);
                w_u16(field, y);
                tag = r_u32(object);
                field += 16;
                w_u32(packet, tag | 0x3000000);
                tag = packet & 0xFFFFFF;
                packet += 16;
                w_u32(object, tag);
                if (code == 45 || code == 55)
                    ++x;
                if (code == 59 || code == 61)
                    x -= 4;
                if (code - 26 < 2)
                    x -= 5;
            }
            x += 8;
            ch = native_text ? *native_text++ : r_u8(text);
            ++text;
        } while (ch != 0);
    }
    return sub_800346c0(object, packet, 54);
}

uint32 sub_800346c0(uint32 object, uint32 dst, uint32 mode)
{
    uint32 packet_tag, head;
    FUNCTION_MARKER(0x800346C0, "ACE3.BIN");
    w_u8(dst + 3, 1);
    w_u32(dst + 4, (mode & 0x9FF) | 0xE1000000);
    packet_tag = r_u32(dst);
    head = r_u32(object);
    w_u32(dst, (packet_tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    w_u32(object, (head & 0xFF000000) | (dst & 0xFFFFFF));
    return dst + 8;
}

uint32 sub_800365fc(uint32 object, uint32 dst, uint32 amount, uint32 outer_color, uint32 inner_color)
{
    uint32 x, y, rx, ry, packet, tag, head, i, command;
    FUNCTION_MARKER(0x800365FC, "ACE3.BIN");
    x = (sint32)(amount * 160) / 4096;
    rx = 320 - x;
    y = (sint32)(amount * 120) / 4096;
    ry = 240 - y;
    command = outer_color | 0x38000000;
    for (i = 0; i < 4; ++i)
    {
        packet = dst + i * 36;
        w_u8(packet + 3, 8);
        if (i == 0)
            tag = r_u32(packet);
        w_u8(packet + 7, 56);
        w_u32(packet + 4, command);
        w_u32(packet + 12, outer_color);
        w_u32(packet + 20, inner_color);
        w_u32(packet + 28, inner_color);
        if (i == 0)
        {
            w_u16(packet + 8, 0);
            w_u16(packet + 10, 0);
            w_u16(packet + 16, 320);
            w_u16(packet + 18, 0);
            w_u16(packet + 24, x);
            w_u16(packet + 26, y);
            w_u16(packet + 32, rx);
            w_u16(packet + 34, y);
        }
        else if (i == 1)
        {
            w_u16(packet + 8, 320);
            w_u16(packet + 10, 240);
            w_u16(packet + 16, 0);
            w_u16(packet + 18, 240);
            w_u16(packet + 24, rx);
            w_u16(packet + 26, ry);
            w_u16(packet + 32, x);
            w_u16(packet + 34, ry);
        }
        else if (i == 2)
        {
            w_u16(packet + 8, 0);
            w_u16(packet + 10, 240);
            w_u16(packet + 16, 0);
            w_u16(packet + 18, 0);
            w_u16(packet + 24, x);
            w_u16(packet + 26, ry);
            w_u16(packet + 32, x);
            w_u16(packet + 34, y);
        }
        else
        {
            w_u16(packet + 8, 320);
            w_u16(packet + 10, 0);
            w_u16(packet + 16, 320);
            w_u16(packet + 18, 240);
            w_u16(packet + 24, rx);
            w_u16(packet + 26, y);
            w_u16(packet + 32, rx);
            w_u16(packet + 34, ry);
        }
        if (i != 0)
            tag = r_u32(packet);
        head = r_u32(object);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    return dst + 144;
}

uint32 sub_8005e40c(uint32 state, uint32 object, uint32 dst)
{
    FUNCTION_MARKER(0x8005E40C, "ACE3.BIN");
    sub_8005e10c(dst);
    sub_8005e178(state, object, dst, 0);
    return dst + 1060;
}

uint32 sub_8005e10c(uint32 dst)
{
    FUNCTION_MARKER(0x8005E10C, "ACE3.BIN");
    sub_8005c5b0(dst + 8);
    sub_8005c990(dst + 156);
    sub_8005cedc(dst + 440);
    sub_8005d344(dst + 780);
    sub_8005d844(dst + 580);
    sub_8005dca8(dst + 900);
    sub_8005defc(dst + 988);
    SetDrawMode1(psx_addr(dst, 8), 0, 1, 54);
    return 0xE1000236;
}

uint32 sub_8005e178(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 tag, head, result;
    FUNCTION_MARKER(0x8005E178, "ACE3.BIN");
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 1)
        sub_8005c680(state, object, dst + 8, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 2)
        sub_8005cb48(state, object, dst + 156, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 4)
        sub_8005d000(state, object, dst + 440, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 16)
        sub_8005d468(state, object, dst + 780, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 8)
        sub_8005d998(state, object, dst + 580, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 64)
        sub_8005de40(state, object, dst + 900, native_state);
    if ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) & 128)
        sub_8005e018(state, object, dst + 988, native_state);
    tag = r_u32(dst);
    head = r_u32(object);
    w_u32(dst, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8005c5b0(uint32 dst)
{
    uint32 i, field, first, second, result;
    FUNCTION_MARKER(0x8005C5B0, "ACE3.BIN");
    field = dst + 107;
    for (i = 0; i < 2; ++i)
    {
        w_u8(field - 4, 5);
        w_u8(field, 42);
        field += 24;
    }
    field = dst + 7;
    for (i = 0; i < 5; ++i)
    {
        w_u8(field, 100);
        w_u8(field - 4, 4);
        w_u16(field + 7, 0x7F98);
        w_u8(field, 102);
        field += 20;
    }
    w_u8(dst + 13, 25);
    w_u16(dst + 16, 68);
    w_u16(dst + 18, 20);
    w_u16(dst + 8, 10);
    w_u16(dst + 10, 128);
    result = (dst + 124) & 0xFFFFFF;
    first = r_u32(dst + 100);
    second = r_u32(dst + 124);
    w_u8(dst + 12, 0);
    w_u32(dst + 100, (first & 0xFF000000) | result);
    w_u32(dst + 124, (second & 0xFF000000) | (dst & 0xFFFFFF));
    return result;
}

uint32 sub_8005c990(uint32 dst)
{
    uint32 field, packet, next, i, tag;
    FUNCTION_MARKER(0x8005C990, "ACE3.BIN");
    w_u8(dst + 12, 0x82);
    w_u16(dst + 16, 54);
    w_u16(dst + 18, 16);
    w_u8(dst + 7, 100);
    w_u16(dst + 8, 239);
    w_u8(dst + 3, 4);
    w_u16(dst + 14, 0x7F98);
    w_u8(dst + 13, 15);
    w_u16(dst + 10, 96);
    w_u8(dst + 7, 102);
    tag = r_u32(dst);
    w_u32(dst, (tag & 0xFF000000) | ((dst + 20) & 0xFFFFFF));
    packet = dst + 20;
    w_u8(packet + 12, 184);
    w_u16(packet + 16, 19);
    w_u8(packet + 7, 100);
    w_u16(packet + 18, 41);
    w_u8(packet + 3, 4);
    w_u16(packet + 14, 0x7F98);
    w_u8(packet + 13, 15);
    w_u16(packet + 8, 293);
    w_u16(packet + 10, 96);
    w_u8(packet + 7, 102);
    packet += 20;
    next = packet + 20;
    for (i = 2; i < 7; ++i)
    {
        w_u8(next - 13, 100);
        w_u8(next - 13, 102);
        w_u8(next - 17, 4);
        w_u16(next - 6, 0x7F98);
        w_u8(next - 8, 36);
        w_u8(next - 7, 45);
        w_u16(next - 4, 6);
        w_u16(next - 2, 10);
        w_u16(next - 12, 302);
        if (i < 6)
        {
            tag = r_u32(packet);
            w_u32(packet, (tag & 0xFF000000) | (next & 0xFFFFFF));
        }
        next += 20;
        packet += 20;
    }
    field = packet + 7;
    for (i = 7; i < 13; ++i)
    {
        w_u8(field, 100);
        w_u8(field - 4, 4);
        w_u16(field + 7, 0x7F98);
        w_u8(field, 102);
        field += 20;
    }
    return 0;
}

uint32 sub_8005cedc(uint32 dst)
{
    uint32 field, packet, tag, i;
    FUNCTION_MARKER(0x8005CEDC, "ACE3.BIN");
    w_u8(dst + 12, 68);
    w_u8(dst + 13, 25);
    w_u16(dst + 16, 60);
    w_u8(dst + 7, 100);
    w_u16(dst + 8, 12);
    w_u8(dst + 3, 4);
    w_u16(dst + 14, 0x7F98);
    w_u16(dst + 18, 14);
    w_u16(dst + 10, 206);
    w_u8(dst + 7, 102);
    packet = dst + 20;
    w_u8(packet + 12, 50);
    w_u8(packet + 13, 49);
    w_u16(packet + 16, 5);
    w_u8(packet + 7, 100);
    w_u16(packet + 18, 6);
    w_u8(packet + 3, 4);
    w_u16(packet + 14, 0x7F98);
    w_u16(packet + 8, 14);
    w_u16(packet + 10, 210);
    w_u8(packet + 7, 102);
    field = packet + 27;
    for (i = 2; i < 7; ++i)
    {
        w_u8(field, 100);
        w_u8(field - 4, 4);
        w_u16(field + 7, 0x7F98);
        w_u8(field, 102);
        field += 20;
    }
    packet = dst;
    for (i = 0; i < 6; ++i)
    {
        tag = r_u32(packet);
        w_u32(packet, (tag & 0xFF000000) | ((packet + 20) & 0xFFFFFF));
        packet += 20;
    }
    return 0;
}

uint32 sub_8005d344(uint32 dst)
{
    uint32 tag, field, i;
    FUNCTION_MARKER(0x8005D344, "ACE3.BIN");
    w_u16(dst + 14, 0x7F98);
    w_u8(dst + 12, 220);
    w_u16(dst + 16, 36);
    w_u16(dst + 18, 17);
    w_u16(dst + 8, 238);
    w_u8(dst + 3, 4);
    w_u16(dst + 10, 210);
    tag = r_u32(dst);
    w_u8(dst + 7, 102);
    w_u8(dst + 13, 0);
    w_u32(dst, (tag & 0xFF000000) | ((dst + 20) & 0xFFFFFF));
    w_u16(dst + 34, 0x7E18);
    w_u8(dst + 33, 160);
    w_u16(dst + 36, 37);
    w_u16(dst + 38, 48);
    w_u16(dst + 28, 274);
    w_u8(dst + 23, 4);
    w_u8(dst + 27, 102);
    w_u8(dst + 32, 216);
    w_u16(dst + 30, 182);
    w_u16(dst + 54, 0x7F5B);
    w_u8(dst + 53, 208);
    w_u16(dst + 56, 40);
    w_u16(dst + 58, 40);
    w_u16(dst + 48, 263);
    w_u8(dst + 43, 4);
    w_u8(dst + 47, 102);
    w_u8(dst + 52, 216);
    w_u16(dst + 50, 172);
    field = dst + 74;
    for (i = 3; i < 6; ++i)
    {
        w_u8(field - 11, 4);
        w_u8(field - 7, 102);
        w_u16(field, 0x7F98);
        field += 20;
    }
    return 0;
}

uint32 sub_8005d844(uint32 dst)
{
    uint32 field, packet, x, i;
    FUNCTION_MARKER(0x8005D844, "ACE3.BIN");
    w_u8(dst + 3, 4);
    w_u8(dst + 7, 102);
    w_u16(dst + 14, 0x7F98);
    w_u8(dst + 12, 158);
    w_u8(dst + 13, 33);
    w_u16(dst + 16, 25);
    w_u16(dst + 18, 20);
    w_u16(dst + 8, 238);
    w_u16(dst + 10, 190);
    packet = dst + 20;
    x = 242;
    field = packet + 10;
    for (i = 1; i < 5; ++i)
    {
        w_u8(field - 7, 4);
        w_u8(field - 3, 102);
        w_u16(field + 4, 0x7F98);
        w_u8(field + 2, 56);
        w_u8(field + 3, 48);
        w_u16(field + 6, 5);
        w_u16(field + 8, 7);
        w_u16(field - 2, x);
        w_u16(field, 182);
        field += 20;
        packet += 20;
        x += 5;
    }
    x = 252;
    field = packet + 10;
    for (i = 5; i < 7; ++i)
    {
        w_u8(field - 7, 4);
        w_u8(field - 3, 102);
        w_u16(field + 4, 0x7F98);
        w_u8(field + 2, 56);
        w_u8(field + 3, 48);
        w_u16(field + 6, 5);
        w_u16(field + 8, 7);
        w_u16(field - 2, x);
        w_u16(field, 173);
        field += 20;
        packet += 20;
        x += 5;
    }
    field = packet + 14;
    for (i = 7; i < 10; ++i)
    {
        w_u8(field - 11, 4);
        w_u8(field - 7, 102);
        w_u16(field, 0x7F98);
        field += 20;
    }
    return 0;
}

uint32 sub_8005dca8(uint32 dst)
{
    uint32 tag;
    FUNCTION_MARKER(0x8005DCA8, "ACE3.BIN");
    w_u8(dst + 12, 218);
    w_u8(dst + 13, 17);
    w_u16(dst + 16, 38);
    w_u8(dst + 7, 100);
    w_u16(dst + 18, 39);
    w_u8(dst + 7, 102);
    w_u8(dst + 3, 4);
    w_u16(dst + 14, 0x7F18);
    w_u16(dst + 8, 230);
    w_u16(dst + 10, 164);
    w_u8(dst + 32, 42);
    w_u8(dst + 33, 47);
    w_u16(dst + 36, 8);
    w_u16(dst + 38, 8);
    w_u8(dst + 27, 100);
    w_u16(dst + 28, 223);
    w_u16(dst + 34, 0x7F18);
    w_u8(dst + 23, 4);
    w_u16(dst + 30, 219);
    w_u8(dst + 27, 102);
    w_u8(dst + 47, 64);
    w_u8(dst + 43, 3);
    w_u16(dst + 48, 0);
    w_u16(dst + 50, 226);
    w_u16(dst + 52, 222);
    w_u16(dst + 54, 226);
    w_u8(dst + 47, 66);
    w_u8(dst + 59, 3);
    w_u8(dst + 63, 64);
    w_u16(dst + 64, 230);
    w_u16(dst + 66, 218);
    w_u16(dst + 68, 230);
    w_u16(dst + 70, 203);
    w_u8(dst + 63, 66);
    w_u8(dst + 79, 64);
    w_u16(dst + 80, 268);
    w_u8(dst + 75, 3);
    w_u16(dst + 82, 164);
    w_u16(dst + 84, 320);
    w_u16(dst + 86, 164);
    w_u8(dst + 79, 66);
    tag = r_u32(dst);
    w_u32(dst, (tag & 0xFF000000) | ((dst + 20) & 0xFFFFFF));
    tag = r_u32(dst + 20);
    w_u32(dst + 20, (tag & 0xFF000000) | ((dst + 40) & 0xFFFFFF));
    tag = r_u32(dst + 40);
    w_u32(dst + 40, (tag & 0xFF000000) | ((dst + 56) & 0xFFFFFF));
    tag = (r_u32(dst + 56) & 0xFF000000) | ((dst + 72) & 0xFFFFFF);
    w_u32(dst + 56, tag);
    return tag;
}

uint32 sub_8005defc(uint32 dst)
{
    uint32 tag;
    FUNCTION_MARKER(0x8005DEFC, "ACE3.BIN");
    w_u8(dst + 43, 3);
    w_u8(dst + 47, 66);
    w_u16(dst + 48, 0);
    w_u16(dst + 50, 45);
    w_u16(dst + 52, 4);
    w_u16(dst + 54, 45);
    w_u8(dst + 63, 66);
    w_u8(dst + 59, 3);
    w_u16(dst + 64, 85);
    w_u16(dst + 68, 320);
    w_u16(dst + 66, 14);
    w_u16(dst + 70, 14);
    w_u16(dst + 8, 5);
    w_u8(dst + 13, 55);
    w_u16(dst + 16, 80);
    w_u16(dst + 18, 73);
    w_u8(dst + 3, 4);
    w_u8(dst + 7, 102);
    w_u16(dst + 14, 0x7F98);
    w_u16(dst + 10, 12);
    w_u8(dst + 12, 0);
    w_u16(dst + 28, 9);
    w_u8(dst + 32, 24);
    w_u8(dst + 23, 4);
    w_u8(dst + 27, 102);
    w_u16(dst + 34, 0x7F98);
    w_u16(dst + 30, 12);
    w_u8(dst + 33, 45);
    w_u16(dst + 36, 12);
    w_u16(dst + 38, 10);
    tag = r_u32(dst + 40);
    w_u32(dst + 40, (tag & 0xFF000000) | ((dst + 56) & 0xFFFFFF));
    tag = r_u32(dst + 56);
    w_u32(dst + 56, (tag & 0xFF000000) | (dst & 0xFFFFFF));
    tag = (r_u32(dst) & 0xFF000000) | ((dst + 20) & 0xFFFFFF);
    w_u32(dst, tag);
    return tag;
}

uint32 sub_800847a8(uint32 object, uint32 dst, uint32 rect, uint32 factor, const PSX_RECT *native_rect)
{
    uint32 result, xy, wh, carrier, address, lane, value, i;
    sint32 width, height;
    FUNCTION_MARKER(0x800847A8, "ACE3.BIN");
    width = native_rect ? native_rect->w : r_s16(rect + 4);
    if (width <= 0)
        return dst;
    height = native_rect ? native_rect->h : r_s16(rect + 6);
    if (height <= 0)
        return dst;
    w_u32(dst, ((dst + 16) & 0xFFFFFF) | 0x3000000);
    w_u32(dst + 4, 0xE1000218);
    w_u32(dst + 8, 0xE20C4318);
    w_u32(dst + 12, 0);
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, 0x1000 - factor);
    xy = native_rect ? (uint16)native_rect->x | ((uint32)(uint16)native_rect->y << 16) : r_u32(rect);
    wh = native_rect ? (uint16)native_rect->w | ((uint32)(uint16)native_rect->h << 16) : r_u32(rect + 4);
    result = sub_80084318(dst + 16, 0x46D6, xy, wh, &carrier);
    w_u32(result + 4, 0xE1000220);
    w_u32(result, 0x3000000);
    w_u32(result + 8, 0xE2000000);
    w_u32(result + 12, 0);
    address = object + 2;
    lane = address & 3;
    value = (carrier & (0xFFFFFF >> (lane * 8))) | (r_u32(address & ~3) << ((3 - lane) * 8));
    address = result + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    value = dst << 8;
    address = object + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    return result + 16;
}

uint32 sub_80084318(uint32 dst, uint32 texture, uint32 xy, uint32 wh, uint32 *last_a0)
{
    uint32 color, field, link, random, seed, carrier = dst;
    sint32 x, y, width, height, remaining;
    FUNCTION_MARKER(0x80084318, "ACE3.BIN");
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_execute(0x780010);
    color = xport_gte_read_data(22);
    y = (sint32)xy >> 16;
    width = (sint16)wh;
    height = (sint32)wh >> 16;
    while (height > 0)
    {
        remaining = width;
        x = (sint16)xy;
        if (remaining > 0)
        {
            field = dst + 18;
            link = (dst << 8) + 0x1400;
            do
            {
                w_u32(dst, (link >> 8) | 0x4000000);
                w_u32(field - 14, color);
                w_u16(field - 10, x);
                w_u16(field - 8, y);
                seed = r_u32(0x800CBF58);
                carrier = seed * 0x41C64E6D;
                random = psyq_rand_next(&seed);
                w_u32(0x800CBF58, seed);
                random &= texture == 0x4696 ? 0x7C7E : 0x7E7F;
                w_u16(field - 6, random);
                w_u16(field - 4, texture);
                w_u16(field - 2, remaining < 32 ? remaining : 32);
                w_u16(field, height < 48 ? height : 48);
                field += 20;
                link += 0x1400;
                dst += 20;
                remaining -= 32;
                x += 32;
            } while (remaining > 0);
        }
        height -= 48;
        y += 48;
    }
    if (last_a0)
        *last_a0 = carrier;
    return dst;
}

uint32 sub_8005e018(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 tag, head, result;
    FUNCTION_MARKER(0x8005E018, "ACE3.BIN");
    w_u8(dst + 44, (native_state != 0 ? native_state[28] : r_u8(state + 28)) >> 1);
    w_u8(dst + 45, (native_state != 0 ? native_state[29] : r_u8(state + 29)) >> 1);
    w_u8(dst + 46, (native_state != 0 ? native_state[30] : r_u8(state + 30)) >> 1);
    w_u8(dst + 60, (native_state != 0 ? native_state[28] : r_u8(state + 28)) >> 1);
    w_u8(dst + 61, (native_state != 0 ? native_state[29] : r_u8(state + 29)) >> 1);
    w_u8(dst + 62, (native_state != 0 ? native_state[30] : r_u8(state + 30)) >> 1);
    w_u8(dst + 4, (native_state != 0 ? native_state[32] : r_u8(state + 32)) >> 1);
    w_u8(dst + 5, (native_state != 0 ? native_state[33] : r_u8(state + 33)) >> 1);
    w_u8(dst + 6, (native_state != 0 ? native_state[34] : r_u8(state + 34)) >> 1);
    w_u8(dst + 24, (native_state != 0 ? native_state[32] : r_u8(state + 32)) >> 1);
    w_u8(dst + 25, (native_state != 0 ? native_state[33] : r_u8(state + 33)) >> 1);
    w_u8(dst + 26, (native_state != 0 ? native_state[34] : r_u8(state + 34)) >> 1);
    tag = r_u32(dst + 20);
    head = r_u32(object);
    w_u32(dst + 20, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | ((dst + 40) & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_800357b4(void)
{
    uint32 state;
    FUNCTION_MARKER(0x800357B4, "ACE3.BIN");
    state = r_u32(0x800BCBE0);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_800272f0(uint32 data, uint32 header)
{
    uint32 result, entries, descriptor;
    FUNCTION_MARKER(0x800272F0, "ACE3.BIN");
    result = sub_800afd08(data, header);
    entries = sub_80027338(header, 0);
    descriptor = r_u32(header);
    sub_8002722c(entries, descriptor);
    return result;
}

uint32 sub_8005c680(uint32 state, uint32 object, uint32 dst, const uint8 *native_state)
{
    uint32 digit_packet = dst + 20, digit_x = 64, i = 0, tag, head, result;
    uint32 angle, previous, next, color, a, b;
    sint32 number, quotient, sx0, sy0, sx1, sy1, sx2, sy2;
    sint32 v;
    FUNCTION_MARKER(0x8005C680, "ACE3.BIN");
    number = (sint32)((native_state != 0 ? xport_load_le32(native_state + 8) : r_u32(state + 8)) * 28) >> 12;
    do
    {
        quotient = number / 10;
        color = (native_state != 0 ? (sint16)xport_load_le16(native_state + 36) : r_s16(state + 36));
        sub_8003b7b0((uint32)number - (uint32)quotient * 10, digit_x, 137, 1, digit_packet, color);
        number = quotient;
        tag = r_u32(digit_packet);
        head = r_u32(object);
        w_u32(digit_packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(object);
        w_u32(object, (head & 0xFF000000) | (digit_packet & 0xFFFFFF));
        if (number == 0)
            break;
        digit_x -= 10;
        ++i;
        digit_packet += 20;
    } while (i < 4);
    w_u8(dst + 4, (native_state != 0 ? native_state[32] : r_u8(state + 32)));
    w_u8(dst + 5, (native_state != 0 ? native_state[33] : r_u8(state + 33)));
    w_u8(dst + 6, (native_state != 0 ? native_state[34] : r_u8(state + 34)));
    a = dst + 100;
    b = dst + 124;
    angle = (native_state != 0 ? xport_load_le32(native_state + 8) : r_u32(state + 8));
    color = (native_state != 0 ? native_state[28] : r_u8(state + 28));
    angle = (angle << 2) & 0x3FFF;
    previous = (angle - 0x600) & 0x3FFF;
    w_u8(b + 4, color);
    w_u8(a + 4, color);
    v = r_s16(0x800104AC + previous * 2);
    sy0 = r_s16(0x800104AC + (previous + 0x1000) * 2);
    sx0 = (sint32)((uint32)v * 37) >> 16;
    sy0 = (sint32)(0 - (uint32)sy0 * 37) >> 16;
    sx1 = r_s16(0x800104AC + angle * 2);
    sx1 = (sint32)((uint32)sx1 * 37) >> 16;
    sy1 = r_s16(0x800104AC + (angle + 0x1000) * 2);
    next = (angle + 0x600) & 0x3FFF;
    sy1 = (sint32)(0 - (uint32)sy1 * 37) >> 16;
    color = (native_state != 0 ? native_state[29] : r_u8(state + 29));
    w_u8(b + 5, color);
    w_u8(a + 5, color);
    sx2 = r_s16(0x800104AC + next * 2);
    sx2 = (sint32)((uint32)sx2 * 37) >> 16;
    color = (native_state != 0 ? native_state[30] : r_u8(state + 30));
    w_u8(b + 6, color);
    w_u8(a + 6, color);
    w_u16(a + 8, sx0 + 20);
    w_u16(b + 8, 20 - sx0);
    w_u16(a + 10, sy0 + 137);
    w_u16(b + 10, 137 - sy0);
    w_u16(a + 12, sx1 + 20);
    w_u16(b + 12, 20 - sx1);
    w_u16(a + 14, sy1 + 137);
    w_u16(b + 14, 137 - sy1);
    w_u16(b + 16, 20);
    w_u16(a + 16, 20);
    w_u16(b + 18, 137);
    w_u16(a + 18, 137);
    w_u16(a + 20, sx2 + 20);
    sy2 = r_s16(0x800104AC + (next + 0x1000) * 2);
    w_u16(b + 20, 20 - sx2);
    sy2 = (sint32)(0 - (uint32)sy2 * 37) >> 16;
    w_u16(a + 22, sy2 + 137);
    w_u16(b + 22, 137 - sy2);
    tag = r_u32(dst);
    head = r_u32(object);
    w_u32(dst, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(object);
    result = (head & 0xFF000000) | (a & 0xFFFFFF);
    w_u32(object, result);
    return result;
}

uint32 sub_8003b7b0(uint32 glyph, uint32 x, uint32 y, uint32 font, uint32 dst, uint32 color)
{
    uint32 table, width, height, stride, u, result;
    FUNCTION_MARKER(0x8003B7B0, "ACE3.BIN");
    table = font == 0 ? 0x800B1074 : font == 1 ? 0x800B107C : 0x800B1084;
    if (color == 0)
    {
        w_u8(dst + 4, 0);
        w_u8(dst + 5, 131);
        w_u8(dst + 6, 0);
    }
    else if (color == 1)
    {
        w_u8(dst + 4, 131);
        w_u8(dst + 5, 0);
        w_u8(dst + 6, 0);
    }
    else if (color == 2)
    {
        w_u8(dst + 4, 131);
        w_u8(dst + 5, 131);
        w_u8(dst + 6, 0);
    }
    else if (color == 3)
    {
        w_u8(dst + 4, 131);
        w_u8(dst + 5, 131);
        w_u8(dst + 6, 131);
    }
    if (glyph == 10)
    {
        w_u16(dst + 10, y + 3);
        w_u16(dst + 16, 4);
        w_u16(dst + 8, x);
        w_u16(dst + 18, 3);
        w_u8(dst + 12, 0);
        w_u8(dst + 13, 0);
        return 3;
    }
    if (glyph == 11)
    {
        w_u16(dst + 10, y + 2);
        w_u16(dst + 16, 6);
        w_u16(dst + 18, 2);
        w_u16(dst + 8, x);
        w_u8(dst + 12, 44);
        w_u8(dst + 13, 10);
        return 44;
    }
    if (glyph == 0xFFFFFFFF)
    {
        w_u16(dst + 16, 4);
        w_u16(dst + 8, x);
        w_u16(dst + 10, y);
        w_u16(dst + 18, 6);
        w_u8(dst + 12, 40);
        w_u8(dst + 13, 6);
        return 40;
    }
    w_u16(dst + 8, x);
    w_u16(dst + 10, y);
    width = r_u16(table + 4);
    w_u16(dst + 16, width);
    height = r_u8(table + 3);
    w_u16(dst + 18, height);
    stride = r_u32(table + 4);
    u = r_u8(table);
    w_u8(dst + 12, u + glyph * stride);
    result = r_u8(table + 1);
    w_u8(dst + 13, result);
    return result;
}
