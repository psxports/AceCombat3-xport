#include "game_functions.h"
#include <stdlib.h>

uint32 resource002_sub_800fc600(uint32 descriptor, const uint8 *native_descriptor)
{
    uint32 owner, upper, value, index, current;

    FUNCTION_MARKER(0x800FC600, "ACE3.BIN");
    owner = r_u32(0x80112E18);
    upper = owner + 0x10000;
    value = native_descriptor ? xport_load_le32(native_descriptor + 4) : r_u32(descriptor + 4);
    w_u32(upper + 0x1058, value);
    value = native_descriptor ? xport_load_le32(native_descriptor + 8) : r_u32(descriptor + 8);
    w_u32(upper + 0x105C, value);
    value = native_descriptor ? xport_load_le32(native_descriptor + 12) : r_u32(descriptor + 12);
    w_u32(upper + 0x1060, value);
    value = native_descriptor ? xport_load_le32(native_descriptor + 16) : r_u32(descriptor + 16);
    w_u32(owner + 8, value);
    value = native_descriptor ? xport_load_le32(native_descriptor + 20) : r_u32(descriptor + 20);
    w_u32(owner + 12, value);
    for (index = 0; index < 10; ++index)
    {
        value = native_descriptor ? xport_load_le32(native_descriptor + 24 + index * 4) : r_u32(descriptor + 24 + index * 4);
        w_u32(owner + 16 + index * 4, value);
    }
    owner = r_u32(0x80112E18);
    value = native_descriptor ? native_descriptor[0] : r_u8(descriptor);
    w_u8(owner + 5, value);
    owner = r_u32(0x80112E18);
    value = native_descriptor ? xport_load_le16(native_descriptor + 64) : r_u16(descriptor + 64);
    w_u16(owner + 56, value);
    value = native_descriptor ? xport_load_le16(native_descriptor + 66) : r_u16(descriptor + 66);
    w_u16(owner + 58, value);
    value = native_descriptor ? xport_load_le16(native_descriptor + 68) : r_u16(descriptor + 68);
    w_u16(owner + 64, value);
    value = native_descriptor ? xport_load_le16(native_descriptor + 70) : r_u16(descriptor + 70);
    w_u16(owner + 60, 0);
    w_u16(owner + 62, 0);
    w_u16(owner + 68, 0);
    w_u16(owner + 70, 0);
    w_u16(owner + 66, value);
    value = native_descriptor ? xport_load_le16(native_descriptor + 64) : r_u16(descriptor + 64);
    w_u16(owner + 72, value);
    value = native_descriptor ? xport_load_le16(native_descriptor + 66) : r_u16(descriptor + 66);
    w_u16(owner + 74, value);
    value = native_descriptor ? xport_load_le32(native_descriptor) : r_u32(descriptor);
    w_u16(owner + 76, value ? 24 : 16);
    current = r_u32(0x80112E18);
    w_u8(current + 1, 0);
    w_u8(current, 0);
    owner = r_u32(0x80112E18);
    w_u16(current + 78, 0);
    upper = owner + 0x10000;
    w_u16(upper + 0x1052, 0);
    w_u16(upper + 0x1050, 0);
    w_u16(upper + 0x1056, 0);
    w_u8(owner + 2, 0);
    w_u8(r_u32(0x80112E18) + 3, 0);
    w_u8(r_u32(0x80112E18) + 4, 0);
    upper = r_u32(0x80112E18) + 0x10000;
    w_u32(upper + 0x1064, 0);
    w_u32(upper + 0x1068, 300);
    return upper;
}

uint32 resource002_sub_800fc764(uint32 resource, uint32 mode, uint32 arena, uint32 bytes, uint32 buffer_bytes, uint32 slot_bytes, uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
    uint8 descriptor[72];
    uint32 cursor, index, aligned_bytes, aligned_buffer, aligned_slot;

    FUNCTION_MARKER(0x800FC764, "ACE3.BIN");
    w_u32(0x80112E18, arena);
    cursor = arena + 0x1106C;
    aligned_bytes = ((bytes + 3) >> 2) << 2;
    xport_store_le32(descriptor + 12, cursor);
    cursor += aligned_bytes;
    xport_store_le32(descriptor + 4, resource);
    xport_store_le32(descriptor + 8, aligned_bytes);
    xport_store_le32(descriptor + 16, cursor);
    aligned_buffer = ((buffer_bytes + 3) >> 2) << 2;
    aligned_slot = ((slot_bytes + 3) >> 2) << 2;
    cursor += aligned_buffer;
    xport_store_le32(descriptor + 20, cursor);
    cursor += aligned_buffer;
    for (index = 0; index < 10; ++index)
    {
        xport_store_le32(descriptor + 24 + index * 4, cursor);
        cursor += aligned_slot;
    }
    xport_store_le16(descriptor + 64, x0);
    xport_store_le16(descriptor + 66, y0);
    xport_store_le16(descriptor + 68, x1);
    xport_store_le32(descriptor, mode);
    xport_store_le16(descriptor + 70, y1);
    resource002_sub_800fc600(0, descriptor);
    resource002_sub_800fc5a8();
    resource_init_queue(0, 0);
    return cursor;
}

static void resource002_movie_output(void)
{
    resource002_sub_800fcb44();
}

uint32 resource002_sub_800fc5a8(void)
{
    uint32 owner;

    FUNCTION_MARKER(0x800FC5A8, "ACE3.BIN");
    DecDCTReset(0);
    DecDCToutCallback(resource002_movie_output);
    owner = r_u32(0x80112E18);
    DecDCTvlcBuild(psx_addr(owner + 80, sizeof(DECDCTTAB)));
    owner = r_u32(0x80112E18);
    w_u8(owner + 4, 0);
    owner = r_u32(0x80112E18);
    w_u8(owner + 3, 1);
    return 1;
}

void resource002_sub_800fcb44(void)
{
    uint32 owner, index, pixels, x, width, coordinate;
    sint32 end, words;

    FUNCTION_MARKER(0x800FCB44, "ACE3.BIN");
    owner = r_u32(0x80112E18);
    index = r_u8(owner + 1);
    pixels = r_u32(owner + index * 4 + 16);
    sub_800a8440(owner + 72, pixels, NULL, NULL, 0);
    owner = r_u32(0x80112E18);
    index = r_u8(owner + 1);
    w_u8(owner + 1, index < 9 ? index + 1 : 0);
    owner = r_u32(0x80112E18);
    x = r_u16(owner + 72);
    width = r_u16(owner + 76);
    x += width;
    index = r_u8(owner + 2);
    w_u16(owner + 72, x);
    end = r_s16(owner + index * 8 + 56);
    end += r_s16(owner + index * 8 + 60);
    if ((sint16)x < end)
    {
        index = r_u8(owner + 1);
        words = r_s16(owner + 0x11056);
        pixels = r_u32(owner + index * 4 + 16);
        DecDCTout(psx_addr(pixels, ((uint32)words & ~31) * 4), words);
        return;
    }
    w_u8(owner + 3, 1);
    owner = r_u32(0x80112E18);
    index = r_u8(owner + 2);
    w_u8(owner + 2, index ^ 1);
    owner = r_u32(0x80112E18);
    index = r_u8(owner + 2);
    coordinate = r_u16(owner + index * 8 + 56);
    index = r_u8(owner + 2);
    w_u16(owner + 72, coordinate);
    coordinate = r_u16(owner + index * 8 + 58);
    w_u16(owner + 74, coordinate);
}

uint32 resource002_sub_800fcc7c(uint32 owner)
{
    uint32 state, phase, header, value, index, payload, output, bytes;
    sint32 previous, height, width, scale, size;

    FUNCTION_MARKER(0x800FCC7C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_u32(state + 280);
    if (phase == 1)
    {
        if (cd_stream_get_frame(&payload, NULL, 0, 0x80112E14))
            return 0;
        state = r_u32(0x80112DC4);
        header = r_u32(0x80112E14);
        w_u32(state + 280, 0);
        if (!r_u16(header + 6))
            w_u8(owner + 4, 1);
        header = r_u32(0x80112E14);
        previous = r_s16(owner + 0x11052);
        value = r_u16(header + 6);
        w_u16(owner + 0x11054, value);
        height = r_s16(header + 10);
        if (previous != height)
        {
            w_u16(owner + 0x11052, height);
            scale = r_s16(owner + 76);
            w_u16(owner + 0x11056, (height * scale) >> 1);
            w_u16(owner + 78, height);
            w_u16(owner + 70, height);
            w_u16(owner + 62, height);
        }
        header = r_u32(0x80112E14);
        previous = r_s16(owner + 0x11050);
        width = r_s16(header + 8);
        if (previous != width)
        {
            w_u16(owner + 0x11050, width);
            if (r_u8(owner + 5))
                width = (width * 3) >> 1;
            w_u16(owner + 68, width);
            w_u16(owner + 60, width);
        }
        index = r_u8(owner);
        w_u8(owner, index ^ 1);
        size = DecDCTBufSize(psx_addr(payload, 4));
        DecDCTvlcSize2(size / 4 + 1);
        index = r_u8(owner);
        output = r_u32(owner + index * 4 + 8);
        header = r_u32(0x80112E14);
        bytes = (uint32)r_u8(header + 2) * r_u32(r_u32(0x800BCBB4) + 4) * 4;
        if (DecDCTvlc2(psx_addr(payload, bytes), psx_addr(output, ((uint32)size + 1) * 4), psx_addr(owner + 80, sizeof(DECDCTTAB))) == 1)
            return 0;
        header = r_u32(0x80112E14);
        state = r_u32(0x80112DC4);
        w_u32(state + 280, 1);
    }
    else
    {
        if (phase)
            return 1;
        if (DecDCTvlc2(NULL, NULL, psx_addr(owner + 80, sizeof(DECDCTTAB))) == 1)
            return 0;
        header = r_u32(0x80112E14);
        state = r_u32(0x80112DC4);
        w_u32(state + 280, 1);
    }
    cd_stream_free_frame(header);
    return 1;
}

static uint32 resource002_scene_divide(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        xport_mips_break(7);
    if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    return (uint32)((sint32)numerator / (sint32)denominator);
}

static uint32 resource002_scene_rectangle(uint32 packet, uint32 fields, uint32 capture_tag)
{
    uint32 value, add, tag = 0;

    value = r_u16(fields);
    w_u16(packet + 8, value);
    value = r_u16(fields + 2);
    w_u16(packet + 10, value);
    value = r_u16(fields);
    add = r_u16(fields + 4);
    w_u16(packet + 16, value + add);
    value = r_u16(fields + 2);
    w_u16(packet + 18, value);
    value = r_u16(fields);
    w_u16(packet + 24, value);
    value = r_u16(fields + 2);
    add = r_u16(fields + 6);
    w_u16(packet + 26, value + add);
    value = r_u16(fields);
    add = r_u16(fields + 4);
    w_u16(packet + 32, value + add);
    value = r_u16(fields + 2);
    add = r_u16(fields + 6);
    if (capture_tag)
        tag = r_u32(packet);
    w_u16(packet + 34, value + add);
    return tag;
}

static void resource002_scene_link(uint32 packet, uint32 ordering, uint32 tag)
{
    uint32 value;

    value = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (value & 0xFFFFFF));
    value = r_u32(ordering);
    w_u32(ordering, (value & 0xFF000000) | (packet & 0xFFFFFF));
}

uint32 resource002_sub_800ef4f4(uint32 fallback_result)
{
    uint32 state, ordering, game, buffer, owner, record, packet, index, texture;
    uint32 elapsed, denominator, numerator, dx, dy, dw, dh, tag, value, add, x, y, width, height;
    sint32 kind, start, middle, end, frame, first, second, delay, limit;

    FUNCTION_MARKER(0x800EF4F4, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    for (index = 0; index < 16; ++index)
    {
        game = r_u32(0x800BCE60);
        buffer = r_u32(game + 4);
        owner = r_u32(0x80112DB0);
        packet = owner + buffer * 640 + 44 + index * 40;
        kind = r_s16(owner + index * 160 + 1324);
        switch (kind)
        {
            case 1:
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                start = r_s16(record + 1326);
                frame = r_s32(owner + 16);
                if (frame < start)
                    break;
                middle = r_s16(record + 1328);
                if (frame < middle)
                {
                    first = r_s16(record + 1340);
                    second = r_s16(record + 1332);
                    elapsed = (uint32)frame - (uint32)start;
                    numerator = ((uint32)first - (uint32)second) * elapsed;
                    denominator = (uint32)middle - (uint32)start;
                    dx = resource002_scene_divide(numerator, denominator);
                    first = r_s16(record + 1344);
                    second = r_s16(record + 1336);
                    dw = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                    x = r_u16(record + 1332);
                    y = r_u16(record + 1334);
                    width = r_u16(record + 1336);
                    w_u16(packet + 10, y);
                    goto fixed_height;
                }
                end = r_s16(record + 1330);
                elapsed = (uint32)frame - (uint32)middle;
                if (frame < end)
                {
                    first = r_s16(record + 1342);
                    second = r_s16(record + 1334);
                    numerator = ((uint32)first - (uint32)second) * elapsed;
                    denominator = (uint32)end - (uint32)middle;
                    dy = resource002_scene_divide(numerator, denominator);
                    first = r_s16(record + 1346);
                    second = r_s16(record + 1338);
                    dh = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                    x = r_u16(record + 1340);
                    height = r_u16(record + 1338);
                    width = r_u16(record + 1344);
                    y = r_u16(record + 1334);
                    goto vertical;
                }
                tag = resource002_scene_rectangle(packet, record + 1340, 1);
                resource002_scene_link(packet, ordering, tag);
                w_u16(record + 1324, 2);
                break;
            case 2:
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                tag = resource002_scene_rectangle(packet, record + 1340, 1);
                resource002_scene_link(packet, ordering, tag);
                break;
            case 3:
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                start = r_s16(record + 1326);
                frame = r_s32(owner + 16);
                if (frame < start)
                    break;
                end = r_s16(record + 1330);
                elapsed = (uint32)frame - (uint32)start;
                if (frame >= end)
                {
                    tag = resource002_scene_rectangle(packet, record + 1340, 1);
                    resource002_scene_link(packet, ordering, tag);
                    w_u16(record + 1324, 2);
                    break;
                }
                first = r_s16(record + 1340);
                second = r_s16(record + 1332);
                numerator = ((uint32)first - (uint32)second) * elapsed;
                denominator = (uint32)end - (uint32)start;
                dx = resource002_scene_divide(numerator, denominator);
                first = r_s16(record + 1342);
                second = r_s16(record + 1334);
                dy = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                first = r_s16(record + 1344);
                second = r_s16(record + 1336);
                dw = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                first = r_s16(record + 1346);
                second = r_s16(record + 1338);
                dh = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                x = r_u16(record + 1332);
                y = r_u16(record + 1334);
                width = r_u16(record + 1336);
                height = r_u16(record + 1338);
                x += dx;
                w_u16(packet + 8, x);
                w_u16(packet + 24, x);
                y += dy;
                w_u16(packet + 10, y);
                w_u16(packet + 18, y);
                width += dw;
                w_u16(packet + 16, x + width);
                w_u16(packet + 32, x + width);
                tag = r_u32(packet);
                height += dh;
                w_u16(packet + 26, y + height);
                w_u16(packet + 34, y + height);
                resource002_scene_link(packet, ordering, tag);
                break;
            case 4:
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                start = r_s16(record + 1326);
                frame = r_s32(owner + 16);
                if (frame < start)
                    break;
                middle = r_s16(record + 1328);
                if (frame < middle)
                {
                    first = r_s16(record + 1342);
                    second = r_s16(record + 1334);
                    elapsed = (uint32)frame - (uint32)start;
                    numerator = ((uint32)first - (uint32)second) * elapsed;
                    denominator = (uint32)middle - (uint32)start;
                    dy = resource002_scene_divide(numerator, denominator);
                    first = r_s16(record + 1346);
                    second = r_s16(record + 1338);
                    dh = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                    x = r_u16(record + 1332);
                    height = r_u16(record + 1338);
                    width = r_u16(record + 1336);
                    y = r_u16(record + 1334);
                vertical:
                    w_u16(packet + 8, x);
                    w_u16(packet + 24, x);
                    w_u16(packet + 16, x + width);
                    w_u16(packet + 32, x + width);
                    tag = r_u32(packet);
                    y += dy;
                    w_u16(packet + 10, y);
                    w_u16(packet + 18, y);
                    height += dh;
                    w_u16(packet + 26, y + height);
                    w_u16(packet + 34, y + height);
                    resource002_scene_link(packet, ordering, tag);
                    break;
                }
                end = r_s16(record + 1330);
                elapsed = (uint32)frame - (uint32)middle;
                if (frame < end)
                {
                    first = r_s16(record + 1340);
                    second = r_s16(record + 1332);
                    numerator = ((uint32)first - (uint32)second) * elapsed;
                    denominator = (uint32)end - (uint32)middle;
                    dx = resource002_scene_divide(numerator, denominator);
                    first = r_s16(record + 1344);
                    second = r_s16(record + 1336);
                    dw = resource002_scene_divide(((uint32)first - (uint32)second) * elapsed, denominator);
                    x = r_u16(record + 1332);
                    y = r_u16(record + 1342);
                    width = r_u16(record + 1336);
                    w_u16(packet + 10, y);
                fixed_height:
                    w_u16(packet + 18, y);
                    w_u16(packet + 26, y + 2);
                    w_u16(packet + 34, y + 2);
                    x += dx;
                    w_u16(packet + 8, x);
                    w_u16(packet + 24, x);
                    width += dw;
                    w_u16(packet + 16, x + width);
                    w_u16(packet + 32, x + width);
                    tag = r_u32(packet);
                    resource002_scene_link(packet, ordering, tag);
                    break;
                }
                tag = resource002_scene_rectangle(packet, record + 1340, 1);
                resource002_scene_link(packet, ordering, tag);
                w_u16(record + 1324, 0);
                break;
            case 5:
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                delay = r_s16(record + 1354);
                limit = r_s16(record + 1350);
                value = r_u16(record + 1354);
                if (delay < limit)
                    w_u16(record + 1354, value + 1);
                else
                {
                    value = r_u16(record + 1352);
                    limit = r_s16(record + 1348);
                    w_u16(record + 1352, value + 1);
                    frame = r_s16(record + 1352);
                    w_u16(record + 1354, 0);
                    if (frame >= limit)
                        w_u16(record + 1352, 0);
                }
                owner = r_u32(0x80112DB0);
                record = owner + index * 160;
                frame = r_s16(record + 1352);
                texture = record + 1356 + ((uint32)frame << 4);
                value = r_u16(texture + 8);
                w_u16(packet + 22, value);
                value = r_u16(texture + 10);
                w_u16(packet + 14, value);
                resource002_scene_rectangle(packet, record + 1332, 0);
                value = r_u8(texture);
                w_u8(packet + 12, value);
                value = r_u8(texture + 2);
                w_u8(packet + 13, value);
                value = r_u8(texture + 4);
                add = r_u8(texture);
                w_u8(packet + 20, add + value - 1);
                value = r_u8(texture + 2);
                w_u8(packet + 21, value);
                value = r_u8(texture);
                w_u8(packet + 28, value);
                value = r_u8(texture + 6);
                add = r_u8(texture + 2);
                w_u8(packet + 29, add + value - 1);
                value = r_u8(texture + 4);
                add = r_u8(texture);
                w_u8(packet + 36, add + value - 1);
                value = r_u8(texture + 6);
                add = r_u8(texture + 2);
                w_u8(packet + 37, add + value - 1);
                value = r_u8(texture + 12);
                tag = r_u32(packet);
                w_u8(packet + 4, value);
                value = r_u8(texture + 13);
                w_u8(packet + 5, value);
                value = r_u8(texture + 14);
                w_u8(packet + 6, value);
                resource002_scene_link(packet, ordering, tag);
                break;
            default:
                break;
        }
    }
    owner = r_u32(0x80112DB0);
    value = r_u32(owner + 3896);
    if (value == 1)
    {
        state = r_u32(0x80112DC4);
        ordering = r_u32(state + 8);
        fallback_result = resource002_sub_800ecc70(ordering + 8, owner + 3904);
        owner = r_u32(0x80112DB0);
        record = r_u32(owner + 6300);
        frame = r_s16(record);
        if (!r_u8(0x8010B400 + ((uint32)frame << 3) + 6))
            resource002_sub_800edf84();
    }
    else if (value == 2)
        fallback_result = 2;
    return fallback_result;
}

uint32 resource002_sub_800ec824(void)
{
    uint32 game, buffer, owner, offset;

    FUNCTION_MARKER(0x800EC824, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    owner = r_u32(0x80112DFC);
    w_u32(owner + 332, owner + buffer * 160 + 12);
    buffer = r_u32(game + 4);
    offset = buffer * 26112 + 448;
    w_u32(owner + 52672, owner + offset);
    return offset;
}

uint32 resource002_sub_800ecbec(uint32 ordering)
{
    uint32 owner, count, index = 0, packet, value, tag;

    FUNCTION_MARKER(0x800ECBEC, "ACE3.BIN");
    owner = r_u32(0x80112DFC);
    count = r_u32(owner + 416);
    if ((sint32)count <= 0)
        return count;
    do
    {
        ++index;
        packet = r_u32(owner + 332);
        value = r_u32(ordering);
        tag = r_u32(packet);
        w_u32(packet, (tag & 0xFF000000) | (value & 0xFFFFFF));
        tag = r_u32(ordering);
        value = r_u32(owner + 332);
        w_u32(ordering, (tag & 0xFF000000) | (value & 0xFFFFFF));
        value = r_u32(owner + 332);
        count = r_u32(owner + 416);
        value += 40;
        w_u32(owner + 332, value);
    } while ((sint32)index < (sint32)count);
    return value;
}

uint32 resource002_sub_800ecc70(uint32 ordering, uint32 descriptor)
{
    FUNCTION_MARKER(0x800ECC70, "ACE3.BIN");
    return resource002_sub_800ecc78(ordering, descriptor, r_u32(0x800BCE60));
}

uint32 resource002_sub_800ecc78(uint32 ordering, uint32 descriptor, uint32 game)
{
    uint32 buffer, owner, result, value;

    FUNCTION_MARKER(0x800ECC78, "ACE3.BIN");
    buffer = r_u32(game + 4);
    owner = r_u32(0x80112DFC);
    w_u32(owner + 332, owner + buffer * 160 + 12);
    resource002_sub_800ec824();
    result = resource002_sub_800ec880(ordering);
    resource002_sub_800ecbec(ordering);
    owner = r_u32(0x80112DFC);
    value = r_u32(owner + 440);
    buffer = (uint32)r_s16(owner + 10);
    w_u32(owner + 440, value + 1);
    w_u32(descriptor, buffer);
    owner = r_u32(0x80112DFC);
    value = r_u32(owner + 52680);
    w_u32(owner + 52680, value + 1);
    return result;
}

uint32 resource002_sub_800ec880(uint32 ordering)
{
    FUNCTION_MARKER(0x800EC880, "ACE3.BIN");
    return resource002_sub_800ec888(r_u32(0x80112DFC), ordering);
}

uint32 resource002_sub_800edf84(void)
{
    uint8 descriptor[32];
    uint32 owner, numerator, denominator, quotient, state, packet, ordering, result;

    FUNCTION_MARKER(0x800EDF84, "ACE3.BIN");
    descriptor[19] = 0;
    owner = r_u32(0x80112DB0);
    xport_store_le16(descriptor, 107);
    xport_store_le16(descriptor + 2, 151);
    xport_store_le16(descriptor + 4, 109);
    xport_store_le16(descriptor + 6, 151);
    xport_store_le16(descriptor + 8, 107);
    numerator = r_u32(owner + 3904) * 106;
    denominator = r_u32(owner + 3900);
    quotient = resource002_scene_divide(numerator, denominator);
    xport_store_le16(descriptor + 12, 109);
    xport_store_le16(descriptor + 10, quotient + 151);
    numerator = r_u32(owner + 3904) * 106;
    denominator = r_u32(owner + 3900);
    quotient = resource002_scene_divide(numerator, denominator);
    descriptor[18] = 213;
    descriptor[22] = 213;
    descriptor[26] = 213;
    descriptor[30] = 213;
    descriptor[16] = 229;
    descriptor[17] = 229;
    descriptor[20] = 229;
    descriptor[21] = 229;
    descriptor[24] = 229;
    descriptor[25] = 229;
    descriptor[28] = 229;
    descriptor[29] = 229;
    state = r_u32(0x80112DC4);
    xport_store_le16(descriptor + 14, quotient + 151);
    packet = r_u32(state + 628);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(0, packet, ordering, descriptor);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, result);
    return result;
}

uint32 resource002_sub_800ec888(uint32 owner, uint32 ordering)
{
    uint32 table, pointer, descriptor, counter, tick, cursor, record, glyph_owner, glyph, link;
    uint32 value, packet, offset, index, item, position, total, previous_cursor, result = 0;
    uint32 merge_register, address, shift, merged, word, target_shift;
    sint32 frame, period, start, count, column, row, tile, stride, remainder;

    FUNCTION_MARKER(0x800EC888, "ACE3.BIN");
    table = r_u32(owner + 420);
    if (!r_u32(table))
    {
        pointer = r_u32(owner + 52676);
        if (r_s32(pointer) >= 31)
            return 1;
    }
    table = r_u32(owner + 420);
    frame = r_s16(owner + 10);
    if (frame >= r_s32(table))
        return 1;
    period = r_s16(owner + 8);
    counter = r_u32(owner + 440);
    tick = resource002_scene_divide(counter, (uint32)period);
    descriptor = table + (uint32)frame * 12 + 4;
    w_u32(owner + 424, descriptor);
    start = r_s32(descriptor + 4);
    cursor = r_u32(owner + 52672);
    merge_register = owner + 0x8000;
    while ((sint32)tick >= start)
    {
        descriptor = r_u32(owner + 424);
        column = r_s16(owner + 344);
        row = r_s16(owner + 346);
        count = r_s32(descriptor);
        record = r_u32(descriptor + 8);
        if (count > 0)
        {
            glyph_owner = r_u32(0x80112DFC);
            packet = cursor + 48;
            link = (cursor << 8) + 0x3400;
            index = 0;
            do
            {
                tile = r_s16(record + 2);
                stride = r_s16(owner + 432);
                resource002_scene_divide((uint32)tile, (uint32)stride);
                remainder = tile % stride;
                value = (uint32)column + ((uint32)remainder << 2);
                w_u16(packet + 12, value);
                w_u16(packet - 40, value);
                stride = r_s16(owner + 432);
                value = resource002_scene_divide((uint32)tile, (uint32)stride);
                value = (uint32)row + (value << 3);
                w_u16(packet - 38, value);
                w_u16(packet + 14, value + 4);
                tile = r_s16(record);
                pointer = r_u32(glyph_owner + 428);
                glyph = r_u32(pointer + ((uint32)tile << 2) + 516);
                w_u32(cursor, (link >> 8) | 0x0C000000);
                w_u32(packet - 44, 0xA0000000);
                w_u32(packet - 36, 0x40004);
                w_u32(packet, 0x1000000);
                offset = cursor;
                for (item = 0; item < 8; ++item)
                {
                    value = r_u32(glyph);
                    glyph += 4;
                    w_u32(offset + 16, value);
                    offset += 4;
                }
                packet += 52;
                link += 0x3400;
                cursor += 52;
                offset = cursor;
                w_u32(cursor, (link >> 8) | 0x0C000000);
                w_u32(packet - 44, 0xA0000000);
                w_u32(packet - 36, 0x40004);
                w_u32(packet, 0x1000000);
                for (item = 0; item < 8; ++item)
                {
                    value = r_u32(glyph);
                    glyph += 4;
                    w_u32(offset + 16, value);
                    offset += 4;
                }
                packet += 52;
                link += 0x3400;
                cursor += 52;
                ++index;
                record += 4;
            } while ((sint32)index < count);
        }
        position = r_u16(owner + 10) + 1;
        w_u16(owner + 10, position);
        frame = (sint16)position;
        table = r_u32(owner + 420);
        descriptor = table + (uint32)frame * 12 + 4;
        value = r_u32(owner);
        w_u32(owner + 424, descriptor);
        table = r_u32(owner + 420);
        w_u32(owner, 1 - value);
        frame = r_s16(owner + 10);
        total = r_u32(table);
        merge_register = total;
        if (frame >= (sint32)total)
        {
            result = 1;
            break;
        }
        descriptor = r_u32(owner + 424);
        start = r_s32(descriptor + 4);
    }
    previous_cursor = r_u32(owner + 52672);
    if (cursor != previous_cursor)
    {
        address = ordering + 2;
        shift = (3 - (address & 3)) << 3;
        word = r_masked_u32(address & 0xFFFFFFFC, 15 >> (shift >> 3));
        merged = word << shift;
        if (shift)
            merged |= merge_register & (0xFFFFFFFF >> (32 - shift));
        address = cursor - 52 + 2;
        target_shift = (3 - (address & 3)) << 3;
        w_masked_u32(address & 0xFFFFFFFC, merged >> target_shift, 15 >> (target_shift >> 3));
        address = ordering + 2;
        w_masked_u32(address & 0xFFFFFFFC, (previous_cursor << 8) >> shift, 15 >> (shift >> 3));
        w_u32(owner + 52672, cursor);
    }
    return result;
}

void resource002_sub_800d1410(void)
{
    FUNCTION_MARKER(0x800D1410, "ACE3.BIN");
}

uint32 resource002_sub_800d1418(void)
{
    uint32 context, bytes, buffer, game, state, value;

    FUNCTION_MARKER(0x800D1418, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 124);
    bytes = resource_total_size(context + 96, 1);
    buffer = arena_alloc(bytes);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    value = r_u16(state + 454) + 1;
    w_u16(state + 454, value);
    return value;
}

uint32 resource002_sub_800d14a0(void)
{
    uint32 game, value;

    FUNCTION_MARKER(0x800D14A0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game + 68);
    if (value == 1)
        return 0x80110000;
    value = r_u32(0x80112DC4);
    w_u16(value + 454, 0);
    return value;
}

uint32 resource002_sub_800d14d0(void)
{
    uint32 context, bytes, buffer, game, state, value;

    FUNCTION_MARKER(0x800D14D0, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 125);
    bytes = resource_total_size(context + 96, 1);
    buffer = arena_alloc(bytes);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    value = r_u16(state + 454) + 1;
    w_u16(state + 454, value);
    return value;
}

uint32 resource002_sub_800d1558(void)
{
    uint32 game, value;

    FUNCTION_MARKER(0x800D1558, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game + 68);
    if (value == 1)
        return 0x80110000;
    value = r_u32(0x80112DC4);
    w_u16(value + 454, 0);
    return value;
}

static void resource002_transition_call(uint32 target)
{
    switch (target)
    {
        case 0x800D1410:
            resource002_sub_800d1410();
            return;
        case 0x800D1418:
            resource002_sub_800d1418();
            return;
        case 0x800D14A0:
            resource002_sub_800d14a0();
            return;
        case 0x800D14D0:
            resource002_sub_800d14d0();
            return;
        case 0x800D1558:
            resource002_sub_800d1558();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 transition %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800de3cc(void)
{
    uint32 state, context, packet, kind, phase, table, target, result;

    FUNCTION_MARKER(0x800DE3CC, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    context = r_u32(0x80112DDC);
    packet = r_u32(state + 628);
    kind = r_u8(context);
    w_u32(context + 8, packet);
    phase = r_u8(context + 1);
    table = r_u32(0x8010AD00 + kind * 4);
    target = r_u32(table + phase * 4);
    resource002_detail_phase_call(target, context);
    context = r_u32(0x80112DDC);
    if (r_u8(context + 2) == 1)
    {
        w_u8(r_u32(0x80112DC4) + 124, 0);
        w_u8(r_u32(0x80112DC4) + 125, 0);
        state = r_u32(0x80112DC4);
        context = r_u32(0x80112DDC);
        w_u16(state + 452, 0);
        w_u16(state + 454, 0);
        arena_set_cursor(context);
    }
    state = r_u32(0x80112DC4);
    phase = (uint32)r_s16(state + 454);
    target = r_u32(0x8010AC6C + (phase << 2));
    resource002_transition_call(target);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    result = r_u32(context + 8);
    w_u32(state + 628, result);
    return result;
}

uint32 resource002_sub_800e6954(void)
{
    uint32 state, kind, phase, table, target;

    FUNCTION_MARKER(0x800E6954, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    kind = (uint32)r_s16(state + 132);
    phase = (uint32)r_s16(state + 134);
    table = r_u32(0x8010B1E8 + (kind << 2));
    target = r_u32(table + (phase << 2));
    return resource002_mode_phase_call(target, state);
}

static void resource002_root_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800CF364:
            resource002_sub_800cf364();
            return;
        case 0x800CF494:
            resource002_sub_800cf494();
            return;
        case 0x800E6658:
            resource002_sub_800e6658();
            return;
        case 0x800E6954:
            resource002_sub_800e6954();
            return;
        case 0x800D0588:
            resource002_sub_800d0588();
            return;
        case 0x800D07C0:
            resource002_sub_800d07c0();
            return;
        case 0x800D09E8:
            resource002_sub_800d09e8();
            return;
        case 0x800D0C34:
            resource002_sub_800d0c34();
            return;
        case 0x800D1214:
            resource002_sub_800d1214();
            return;
        case 0x800D12E0:
            resource002_sub_800d12e0();
            return;
        case 0x800D1258:
            resource002_sub_800d1258();
            return;
        case 0x800D129C:
            resource002_sub_800d129c();
            return;
        case 0x800DE188:
            resource002_sub_800de188();
            return;
        case 0x800DE3CC:
            resource002_sub_800de3cc();
            return;
        case 0x800DE4BC:
            resource002_sub_800de4bc();
            return;
        case 0x800DE540:
            resource002_sub_800de540();
            return;
        case 0x800DE718:
            resource002_sub_800de718();
            return;
        case 0x800DE73C:
            resource002_sub_800de73c();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 root phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800cdf40(void)
{
    uint32 game, buffer, state, value, ordering, tag, head, first, second, scene;
    uint32 owner, target, kind, phase, table, packet, result, card, flags;

    FUNCTION_MARKER(0x800CDF40, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    value = r_u32(state + buffer * 4 + 56);
    w_u32(state + 64, value);
    buffer = r_u32(game + 4);
    value = r_u32(state + buffer * 4 + 632);
    w_u32(state + 628, value);
    buffer = r_u32(game + 4);
    ordering = r_u32(state + buffer * 4);
    w_u32(state + 8, ordering);
    ClearOTagR(psx_addr(ordering, 64), 16);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    ordering = r_u32(state + 8);
    first = r_u32(game + 8);
    tag = r_u32(ordering);
    head = r_u32(first);
    w_u32(ordering, (tag & 0xFF000000) | (head & 0xFFFFFF));
    ordering = r_u32(state + 8);
    first = r_u32(game + 8);
    tag = r_u32(first);
    w_u32(first, (tag & 0xFF000000) | ((ordering + 60) & 0xFFFFFF));
    ordering = r_u32(state + 8);
    resource002_sub_800f3db4(ordering);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    resource002_sub_800ff100(ordering + 60);
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + buffer * 4 + 12);
    w_u32(state + 20, ordering);
    ClearOTagR(psx_addr(ordering, 1024), 256);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 20);
    first = r_u32(state + 8);
    tag = r_u32(ordering);
    head = r_u32(first + 4);
    w_u32(ordering, (tag & 0xFF000000) | (head & 0xFFFFFF));
    first = r_u32(state + 8);
    ordering = r_u32(state + 20);
    game = r_u32(0x800BCE60);
    tag = r_u32(first + 4);
    w_u32(first + 4, (tag & 0xFF000000) | ((ordering + 1020) & 0xFFFFFF));
    buffer = r_u32(game + 4);
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + buffer * 4 + 24);
    w_u32(scene + 16, value);
    buffer = r_u32(game + 4);
    value = r_u32(scene + buffer * 4 + 32);
    w_u32(scene + 20, value);
    xport_gte_write_control(13, 3072);
    xport_gte_write_control(14, 3072);
    xport_gte_write_control(15, 3072);
    xport_gte_write_control(21, 1536);
    xport_gte_write_control(22, 1536);
    xport_gte_write_control(23, 1536);
    xport_gte_write_control(27, 0xFFFF8800);
    xport_gte_write_control(28, 0x03000000);
    xport_gte_write_control(8, 0x08000000);
    xport_gte_write_control(9, 0x04000000);
    xport_gte_write_control(10, 1024);
    xport_gte_write_control(11, 0x0400FC00);
    xport_gte_write_control(12, 0);
    xport_gte_write_control(16, 0x02000400);
    xport_gte_write_control(17, 0x04000200);
    xport_gte_write_control(18, 0x02000200);
    xport_gte_write_control(19, 0x02000400);
    xport_gte_write_control(20, 512);
    value = r_u32(game + 292);
    resource002_sub_800def94(20, (value & 16) != 0);
    owner = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    target = r_u32(owner + 92);
    value = r_u8(game + 916);
    w_u8(target + 32, value);
    state = r_u32(0x80112DC4);
    kind = r_u8(state + 124);
    phase = r_u8(state + 125);
    table = r_u32(0x80109814 + kind * 4);
    target = r_u32(table + phase * 4);
    resource002_root_phase_call(target);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    state = r_u32(0x80112DC4);
    if (flags & 16)
    {
        if (r_s16(state + 132) == 6)
            goto after_prompt;
        kind = r_u8(state + 124);
        value = kind ? 5 : r_u8(state + 126) + 9;
    }
    else
    {
        kind = r_u8(state + 124);
        if (kind == 1 || r_s16(state + 132) == 6)
            goto after_prompt;
        value = r_u8(state + 126) + (kind ? 5 : 1);
    }
    resource002_sub_800def94(44, value);
after_prompt:
    state = r_u32(0x80112DC4);
    packet = r_u32(state + 628);
    result = resource002_sub_80101fec(packet);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, result);
    result = resource002_sub_800e2494(result);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, result);
    value = r_u32(state + 76);
    game = r_u32(0x800BCE60);
    w_u32(state + 76, value + 1);
    flags = r_u32(game + 292);
    owner = r_u32(0x80112DC0);
    target = r_u32(owner + 92);
    w_u8(target + 40, (flags & 16) != 0);
    state = r_u32(0x80112DC4);
    value = r_u32(state + 148);
    if (value)
    {
        sub_8002dda8(r_u32(state + 304));
        state = r_u32(0x80112DC4);
        sub_8002dda8(r_u32(state + 308));
        state = r_u32(0x80112DC4);
        sub_8002dda8(r_u32(state + 312));
        state = r_u32(0x80112DC4);
        sub_8002dda8(r_u32(state + 316));
        state = r_u32(0x80112DC4);
        sub_8002dda8(r_u32(state + 320));
        sound_disable_reverb();
        sound_stop_all();
        resource_init_queue(0, 0);
        card = r_u32(0x800BCE48);
        flags = r_u32(card);
        state = r_u32(0x80112DC4);
        w_u32(card, flags | 4);
        if (r_u8(state + 148) == 4)
        {
            game = r_u32(0x800BCE60);
            first = r_u16(game + 908);
            second = r_u16(game + 910);
            w_u16(game + 912, first);
            w_u16(game + 914, second);
        }
        state = r_u32(0x80112DC4);
        return r_u32(state + 148);
    }
    if (r_u8(state + 127) == 1)
    {
        sound_stop_all();
        resource_init_queue(0, 0);
        card = r_u32(0x800BCE48);
        flags = r_u32(card);
        game = r_u32(0x800BCE60);
        w_u32(card, flags | 4);
        first = r_u16(game + 908);
        second = r_u16(game + 910);
        w_u16(game + 912, first);
        w_u16(game + 914, second);
        return 0x60000004;
    }
    if (r_u32(state + 284) == 1)
    {
        sound_stop_all();
        resource_init_queue(0, 0);
        game = r_u32(0x800BCE60);
        card = r_u32(0x800BCE48);
        w_u32(game + 928, 3);
        flags = r_u32(card);
        w_u32(card, flags | 4);
        return 0x60000005;
    }
    return 0;
}

uint32 resource002_sub_800e6288(void)
{
    uint32 game, state, object, flags;

    FUNCTION_MARKER(0x800E6288, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    w_u16(game + 2388, 1);
    resource002_sub_800dedf0(0, 0x01004001);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 1);
    resource002_sub_800ff19c(3);
    resource002_sub_80100994();
    sub_8002db4c(104);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412) & 0x10;
    state = r_u32(0x80112DC0);
    object = r_u32(state + 92);
    w_u8(object + 46, flags == 0);
    return object;
}

uint32 resource002_sub_800e6334(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800E6334, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return resource002_sub_800e633c(game);
}

uint32 resource002_sub_800e633c(uint32 game)
{
    uint32 state, result;

    FUNCTION_MARKER(0x800E633C, "ACE3.BIN");
    if ((r_u32(game + 300) & 0x10) && !(r_u32(game + 296) & 0x2F))
        resource002_sub_800ff19c(0);
    state = r_u32(0x80112DA8);
    result = r_u32(state + 20);
    if (!result)
    {
        resource002_sub_800dee08(1, 3, 30, 0);
        resource002_sub_800dee08(15, 3, -1, -1);
        resource002_sub_800dee08(11, 3, -1, -1);
        state = r_u32(0x80112DC4);
        result = r_u16(state + 134) + 1;
        w_u16(state + 134, result);
    }
    return result;
}

uint32 resource002_sub_800e63f4(void)
{
    uint32 progress, game, state, result, packet, numerator, high, sum;
    sint32 kind, quotient;
    sint64 product;

    FUNCTION_MARKER(0x800E63F4, "ACE3.BIN");
    progress = resource002_sub_800deea8(1);
    result = resource002_sub_800dee84(1);
    if (!result)
    {
        game = r_u32(0x800BCE60);
        kind = r_s16(game + 908);
        if (kind == 16)
        {
            w_u8(game + 936, 8);
            game = r_u32(0x800BCE60);
            state = r_u32(0x80112DC4);
            result = 2;
            w_u32(game + 928, result);
            w_u32(state + 148, 0x60000005);
        }
        else if (kind == 37)
        {
            w_u8(game + 936, 26);
            game = r_u32(0x800BCE60);
            w_u16(game + 908, 57);
            state = r_u32(0x80112DC4);
            result = 2;
            w_u32(game + 928, result);
            w_u32(state + 148, 0x60000005);
        }
        else if (kind == 42)
        {
            state = r_u32(0x80112DC4);
            result = 56;
            w_u16(game + 908, result);
            w_u32(state + 148, 0x60000002);
        }
        else
        {
            state = r_u32(0x80112DC4);
            result = 0x60000002;
            w_u32(state + 148, result);
        }
        return result;
    }
    numerator = (((progress << 3) + progress) << 4) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (uint32)(product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    packet = r_u32(0x800BCE48);
    w_u8(packet + 16, (uint32)quotient);
    numerator = (((progress << 4) + progress) << 3) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (uint32)(product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    packet = r_u32(0x800BCE48);
    w_u8(packet + 17, (uint32)quotient);
    numerator = (((((progress << 2) + progress) << 2) + progress) << 2) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (uint32)(product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    packet = r_u32(0x800BCE48);
    w_u8(packet + 18, (uint32)quotient);
    packet = r_u32(0x800BCE48);
    result = r_u32(packet) | 8;
    w_u32(packet, result);
    return result;
}

uint32 resource002_sub_800e65a4(void)
{
    uint32 game, state, result;

    FUNCTION_MARKER(0x800E65A4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 0x10;
    if (result)
    {
        result = r_u32(game + 296) & 0x2F;
        if (!result)
        {
            state = r_u32(0x80112DC4);
            result = 2;
            w_u32(game + 944, result);
            w_u32(state + 148, 0x60000002);
        }
    }
    return result;
}

uint32 resource002_sub_800e65f8(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800E65F8, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 1);
    return 1;
}

uint32 resource002_sub_800e660c(void)
{
    uint32 game, state, result, flags;

    FUNCTION_MARKER(0x800E660C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 0x10;
    if (result)
    {
        flags = r_u32(game + 296) & 0x2F;
        result = 0x80110000;
        if (!flags)
        {
            state = r_u32(0x80112DC4);
            result = 5;
            w_u16(state + 132, result);
            w_u16(state + 134, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800d240c(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800D240C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    return resource002_sub_800d2414(state);
}

uint32 resource002_sub_800d2414(uint32 state)
{
    uint32 context, phase;

    FUNCTION_MARKER(0x800D2414, "ACE3.BIN");
    resource002_sub_800ecda8(r_u32(state + 152));
    resource002_sub_800dedf0(0, 3);
    context = r_u32(0x80112DDC);
    if (!r_u8(context + 5))
    {
        resource002_sub_800dedf0(2, 1);
        phase = 1;
    }
    else
    {
        resource002_sub_800dedf0(2, 2);
        phase = 4;
    }
    context = r_u32(0x80112DDC);
    w_u8(context + 1, phase);
    resource002_sub_800dee08(1, 1, 0, 40);
    resource002_sub_800dee08(2, 1, 0, 40);
    resource002_sub_800def94(1, 4);
    context = r_u32(0x80112DDC);
    resource002_sub_800def94(5, r_u8(context + 4));
    return resource002_sub_800def94(62, 1);
}

uint32 resource002_sub_800d24e8(void)
{
    uint32 state, value, context;

    FUNCTION_MARKER(0x800D24E8, "ACE3.BIN");
    if (!resource002_sub_800ded38(0, 1))
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 420) + 1;
        w_u32(state + 420, value);
    }
    value = resource002_sub_800dee84(1);
    if (value == 2)
        w_u8(r_u32(0x80112DDC) + 1, value);
    context = r_u32(0x80112DDC);
    return resource002_sub_800def94(5, r_u8(context + 4));
}

uint32 resource002_sub_800d2568(void)
{
    uint32 game, context, restricted, changed, selection, result;

    FUNCTION_MARKER(0x800D2568, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    restricted = r_u32(game + 2412) & 0x10;
    resource002_sub_800def94(62, 1);
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 4);
    changed = 0;
    switch (selection)
    {
        case 0:
            game = r_u32(0x800BCE60);
            if ((r_u32(game + 304) & 3) && !restricted)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 2);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 1);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 4);
            }
            break;
        case 1:
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 3)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 3);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 4);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 0);
            }
            break;
        case 2:
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 3)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 0);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 3);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 5);
            }
            break;
        case 3:
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 3)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 1);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 5);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, restricted ? 5 : 2);
            }
            break;
        case 4:
            resource002_sub_800def94(62, 0);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 0);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 1);
            }
            break;
        case 5:
            resource002_sub_800def94(62, 0);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 8)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, restricted ? 3 : 2);
            }
            game = r_u32(0x800BCE60);
            if (r_u32(game + 304) & 4)
            {
                changed = 1;
                w_u8(r_u32(0x80112DDC) + 4, 3);
            }
            break;
    }
    context = r_u32(0x80112DDC);
    if ((uint32)(r_u8(context + 4) - 4) >= 2)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 304) & 3) && changed)
            sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 0xC) && changed)
        sub_8002db4c(61);
    context = r_u32(0x80112DDC);
    resource002_sub_800def94(5, r_u8(context + 4));
    context = r_u32(0x80112DDC);
    if ((uint32)(r_u8(context + 4) - 4) < 2)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 0x40) && !(r_u32(game + 296) & 0x2F) && r_u32(r_u32(0x80112DC4) + 144) != 5)
        {
            w_u8(context + 1, 3);
            w_u8(r_u32(0x80112DDC) + 3, 0);
            resource002_sub_800dee08(1, 3, -1, -1);
            resource002_sub_800decd4(0, 1);
            resource002_sub_800dee08(2, 3, -1, -1);
            resource002_sub_800dee08(15, 3, -1, -1);
            sub_8002db4c(60);
            sub_8002db4c(91);
        }
    }
    else
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 0x10) && !(r_u32(game + 296) & 0x2F) && r_u32(r_u32(0x80112DC4) + 144) != 5)
        {
            w_u8(context + 1, 3);
            w_u8(r_u32(0x80112DDC) + 3, 0);
            resource002_sub_800dee08(1, 3, -1, -1);
            context = r_u32(0x80112DDC);
            if (r_u8(context + 4) == 2)
                w_u16(r_u32(0x80112DC4) + 454, 1);
            context = r_u32(0x80112DDC);
            if (r_u8(context + 4) == 3)
                resource002_sub_800dee08(15, 3, -1, -1);
            resource002_sub_800decd4(0, 1);
            resource002_sub_800dee08(2, 3, -1, -1);
            sub_8002db4c(60);
            sub_8002db4c(91);
        }
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 0x20;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            context = r_u32(0x80112DDC);
            w_u8(context + 1, 3);
            context = r_u32(0x80112DDC);
            w_u8(context + 3, 1);
            resource002_sub_800dee08(1, 3, -1, -1);
            resource002_sub_800dee08(2, 3, -1, -1);
            sub_8002db4c(59);
            sub_8002db4c(91);
            result = r_u32(0x800BCE60);
            w_u32(result + 300, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800d2bcc(void)
{
    uint32 state, context, selection, result;

    FUNCTION_MARKER(0x800D2BCC, "ACE3.BIN");
    result = resource002_sub_800dee84(1);
    if (result)
        return result;
    state = r_u32(0x80112DC4);
    result = (uint32)r_s16(state + 454);
    if (result)
        return result;
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 5);
    result = 4;
    if (selection == 1)
    {
        w_u8(context + 1, result);
        return result;
    }
    result = r_u8(context + 3);
    if (result)
    {
        if (result == 1)
            w_u8(context + 2, result);
        return result;
    }
    if (!r_u8(context + 4))
        w_u8(context, 1);
    context = r_u32(0x80112DDC);
    if (r_u8(context + 4) == 1)
        w_u8(context, 3);
    context = r_u32(0x80112DDC);
    if (r_u8(context + 4) == 2)
        w_u8(context, 5);
    context = r_u32(0x80112DDC);
    if (r_u8(context + 4) == 3)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 0);
        w_u8(context, 6);
        resource002_sub_800dedf0(2, 0);
        resource002_sub_800dedf0(0, 0x8000);
        resource002_sub_800debb8(0, 15);
    }
    context = r_u32(0x80112DDC);
    if ((uint32)(r_u8(context + 4) - 4) < 2)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 0);
        w_u8(context, 10);
        resource002_sub_800dedf0(2, 0);
        resource002_sub_800dedf0(0, 0);
    }
    resource002_sub_800dee08(1, 1, 0, 40);
    context = r_u32(0x80112DDC);
    w_u8(context + 1, 0);
    resource002_sub_800dee08(2, 1, 0, 40);
    return sub_8002db4c(90);
}

uint32 resource002_sub_800d2d74(void)
{
    uint32 state, value, context;

    FUNCTION_MARKER(0x800D2D74, "ACE3.BIN");
    if (!resource002_sub_800ded38(0, 1))
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 420) + 1;
        w_u32(state + 420, value);
    }
    if (resource002_sub_800dee84(1) == 2)
        w_u8(r_u32(0x80112DDC) + 1, 5);
    context = r_u32(0x80112DDC);
    return resource002_sub_800def94(5, r_u8(context + 5));
}

uint32 resource002_sub_800d2df0(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800D2DF0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return resource002_sub_800d2df8(game);
}

uint32 resource002_sub_800d2df8(uint32 game)
{
    uint32 context, state, selection, result;

    FUNCTION_MARKER(0x800D2DF8, "ACE3.BIN");
    if (r_u32(game + 304) & 8)
    {
        context = r_u32(0x80112DDC);
        selection = r_u8(context + 5);
        w_u8(context + 5, selection < 3 ? selection + 1 : 1);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 304) & 4)
    {
        context = r_u32(0x80112DDC);
        selection = r_u8(context + 5);
        w_u8(context + 5, selection < 2 ? 3 : selection - 1);
    }
    context = r_u32(0x80112DDC);
    resource002_sub_800def94(5, r_u8(context + 5));
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 0x10) && !(r_u32(game + 296) & 0x2F))
    {
        context = r_u32(0x80112DDC);
        selection = r_u8(context + 5);
        if (selection == 3)
        {
            state = r_u32(0x80112DC4);
            w_u16(state + 454, selection);
        }
        context = r_u32(0x80112DDC);
        w_u8(context + 1, 6);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        resource002_sub_800decd4(0, 1);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 0x20;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            context = r_u32(0x80112DDC);
            w_u8(context + 5, 0);
            context = r_u32(0x80112DDC);
            w_u8(context + 1, 6);
            resource002_sub_800dee08(1, 3, -1, -1);
            result = r_u32(0x800BCE60);
            w_u32(result + 300, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800d2fa8(void)
{
    uint32 state, context, selection, result;

    FUNCTION_MARKER(0x800D2FA8, "ACE3.BIN");
    result = resource002_sub_800dee84(1);
    if (result)
        return result;
    state = r_u32(0x80112DC4);
    result = (uint32)r_s16(state + 454);
    if (result)
        return result;
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 5);
    if (!selection)
    {
        w_u8(context + 1, 1);
        resource002_sub_800dee08(1, 1, 0, 40);
        return resource002_sub_800dedf0(2, 1);
    }
    if (selection == 1)
        w_u8(context, 7);
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 5);
    if (selection == 2)
    {
        w_u8(context, 8);
        context = r_u32(0x80112DDC);
        selection = r_u8(context + 5);
    }
    if (selection == 3)
        w_u8(context, 9);
    result = r_u32(0x80112DDC);
    w_u8(result + 1, 0);
    return result;
}

uint32 resource002_sub_800d3088(void)
{
    uint32 context, value, state, object, index;

    FUNCTION_MARKER(0x800D3088, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    context = r_u32(0x80112DDC);
    w_u8(context + 2, 0);
    resource002_sub_800dee08(1, 0, 0, 0);
    resource002_sub_800dee08(2, 0, 0, 0);
    resource002_sub_800dedf0(2, 12);
    resource002_sub_800dedf0(0, 1);
    resource002_sub_800deca8(2, 2);
    resource002_sub_800deca8(2, 3);
    state = r_u32(0x80112DC4);
    object = r_u32(state + 28);
    resource002_sub_800ecda8(r_u32(object + 12));
    context = r_u32(0x80112DDC);
    w_u16(context + 14, 0);
    w_u8(context + 20, 0);
    for (index = 0; index < 4; ++index)
    {
        context = r_u32(0x80112DDC);
        w_u8(context + index + 21, 0);
    }
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC0);
    w_u32(context + 224, 1);
    object = r_u32(state + 92);
    w_u8(object + 11, 4);
    state = r_u32(0x80112DC0);
    object = r_u32(state + 92);
    w_u8(object + 10, 0);
    resource002_sub_800def94(62, 0);
    return resource002_sub_800d1b6c();
}

uint32 resource002_sub_800d8ca8(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800D8CA8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return resource002_sub_800d8cb0(game);
}

uint32 resource002_sub_800d8cb0(uint32 game)
{
    uint32 context, state, object, phase, value, flags, next_context, result;
    sint32 input_state, selection;

    FUNCTION_MARKER(0x800D8CB0, "ACE3.BIN");
    if (r_u16(game + 342) & 0xB)
    {
        context = r_u32(0x80112DDC);
        w_u32(context + 224, 1);
        w_u32(context + 16, 0);
        resource002_sub_800deca8(2, 2);
        resource002_sub_800deca8(2, 3);
        resource002_sub_800dedf0(2, 12);
    }
    context = r_u32(0x80112DDC);
    if (!r_u32(context + 16))
        resource002_sub_800d3240();
    context = r_u32(0x80112DDC);
    phase = r_u32(context + 16);
    if (phase == 1)
    {
        if (!r_u8(context + 28))
            resource002_sub_800d4ca4();
        context = r_u32(0x80112DDC);
        if (r_u8(context + 28) == phase)
            resource002_sub_800d4394();
    }
    context = r_u32(0x80112DDC);
    phase = r_u32(context + 16);
    if (!phase)
        resource002_sub_800d4f78();
    else if (phase == 1)
        resource002_sub_800d7db0();
    context = r_u32(0x80112DDC);
    if (r_u32(context + 224))
    {
        game = r_u32(0x800BCE60);
        input_state = r_s16(game + 342);
        flags = r_u16(game + 342);
        if (!input_state)
        {
            if (r_u32(game + 292) & 4)
                w_u16(context + 12, 0);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 292) & 2)
                w_u16(r_u32(0x80112DDC) + 12, 0);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 292) & 0x10)
                w_u16(r_u32(0x80112DDC) + 12, 1);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 292) & 1)
                w_u16(r_u32(0x80112DDC) + 12, 2);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 292) & 8)
                w_u16(r_u32(0x80112DDC) + 12, 3);
            game = r_u32(0x800BCE60);
            if (r_u32(game + 292) & 0x20)
                w_u16(r_u32(0x80112DDC) + 12, 5);
            context = r_u32(0x80112DDC);
            if (r_s16(context + 12) < 4)
            {
                state = r_u32(0x80112DC0);
                w_u32(context + 228, 1);
                object = r_u32(state + 92);
                w_u8(object + 11, 4);
                w_u32(r_u32(0x80112DDC) + 224, 0);
            }
            game = r_u32(0x800BCE60);
            context = r_u32(0x80112DDC);
            value = r_u32(game + 292);
            w_u32(context + 232, value);
            if (!r_u32(game + 292))
                w_u16(context + 12, 4);
            return 0;
        }
        if (flags & 8)
        {
            state = r_u32(0x80112DC0);
            object = r_u32(state + 92);
            w_u8(object + 11, 5);
        }
    }
    else
    {
        game = r_u32(0x800BCE60);
        if (r_s16(game + 342))
            w_u32(context + 224, 1);
        else
        {
            value = r_u32(game + 292);
            flags = r_u32(context + 232);
            if (value != flags)
                w_u32(context + 224, 1);
        }
    }
    game = r_u32(0x800BCE60);
    context = r_u32(0x80112DDC);
    value = r_u32(game + 292);
    w_u32(context + 232, value);
    if (!r_u32(game + 292))
        w_u16(context + 12, 4);
    context = r_u32(0x80112DDC);
    if (r_u32(context + 228))
    {
        w_u32(context + 16, 0);
        resource002_sub_800deca8(2, 2);
        resource002_sub_800deca8(2, 3);
        resource002_sub_800dedf0(2, 12);
        context = r_u32(0x80112DDC);
        selection = r_s16(context + 12);
        if (selection < 4)
        {
            state = r_u32(0x80112DC4);
            object = r_u32(state + 444);
            value = r_u32(object + ((uint32)selection << 2));
            resource002_sub_800ecf88(value);
        }
        state = r_u32(0x80112DC0);
        object = r_u32(state + 92);
        w_u8(object + 11, 4);
        context = r_u32(0x80112DDC);
        w_u8(context + 31, 0);
        next_context = r_u32(0x80112DDC);
        w_u32(context + 228, 0);
        w_u8(next_context + 30, 0);
        context = r_u32(0x80112DDC);
        w_u8(context + 34, 0);
        context = r_u32(0x80112DDC);
        w_u8(context + 35, 0);
        context = r_u32(0x80112DDC);
        w_u8(context + 32, 0);
        context = r_u32(0x80112DDC);
        w_u8(context + 33, 0);
        context = r_u32(0x80112DDC);
        w_u16(context + 14, 0);
        resource002_sub_800d1b6c();
        state = r_u32(0x80112DC0);
        object = r_u32(state + 92);
        w_u8(object + 62, 0);
        state = r_u32(0x80112DC0);
        object = r_u32(state + 92);
        w_u8(object + 63, 0);
        resource002_sub_800dee08(1, 3, -1, -1);
        return resource002_sub_800dee08(2, 3, -1, -1);
    }
    state = r_u32(0x80112DC0);
    object = r_u32(state + 92);
    result = r_u8(context + 12);
    w_u8(object + 11, result);
    return result;
}

uint32 resource002_sub_800d1b04(uint32 flags)
{
    FUNCTION_MARKER(0x800D1B04, "ACE3.BIN");
    if (flags & 1)
        return 0;
    if (flags & 2)
        return 1;
    if (flags & 4)
        return 2;
    if (flags & 8)
        return 3;
    if (flags & 0x10)
        return 4;
    if (flags & 0x20)
        return 5;
    return 0xFFFFFFFF;
}

uint32 resource002_sub_800d1b6c(void)
{
    uint32 game, input, map, slot, source_offset, destination, index, key;
    uint32 mask_address, source, binding, choices, enabled, flags, context;
    uint32 type, value, result, quotient, record_offset;
    sint32 input_index, reverse;
    sint32 mask;
    uint64 product;

    FUNCTION_MARKER(0x800D1B6C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    input = r_u32(game + 292);
    input_index = (sint32)resource002_sub_800d1b04(input);
    map = 0x8010A520;
    source_offset = 0;
    for (slot = 0; slot < 5; ++slot)
    {
        type = r_u8(map);
        context = r_u32(0x80112DDC);
        destination = context + (type << 4) + 36;
        for (reverse = 14; reverse >= 0; --reverse)
            w_u8(destination + (uint32)reverse, 20);
        for (key = 11; key < 31; ++key)
        {
            source = key * 2 + source_offset;
            mask_address = 0x8010A540;
            for (index = 0; index < 15; ++index)
            {
                game = r_u32(0x800BCE60);
                mask = r_s16(mask_address);
                value = r_u16(game + source + 348);
                if ((uint32)mask & value)
                    w_u8(destination + index, r_u8(0x8010A508 + key - 11));
                mask_address += 2;
            }
        }
        game = r_u32(0x800BCE60);
        type = r_u8(map);
        context = r_u32(0x80112DDC);
        source = game + source_offset + 412;
        choices = context + (type << 2) + 132;
        enabled = context + (type << 2) + 156;
        for (reverse = 3; reverse >= 0; --reverse)
            w_u8(choices + (uint32)reverse, 12);
        for (key = 0; key < 6; ++key)
        {
            mask_address = 0x8010A560;
            for (binding = 0; binding < 4; ++binding)
            {
                mask = r_s16(mask_address);
                value = r_u8(source + key + 18);
                if (mask == (sint32)value)
                {
                    w_u8(choices + binding, key * 2);
                    if (!key)
                        w_u8(enabled + binding, !(r_u16(source + 16) & 4));
                    else if (key == 3)
                        w_u8(enabled + binding, !(r_u16(source + 16) & 8));
                    else if (key == 4)
                        w_u8(enabled + binding, (r_u16(source + 16) & 0x10) != 0);
                }
                mask_address += 2;
            }
        }
        flags = r_u16(source + 16);
        context = r_u32(0x80112DDC);
        type = r_u8(map);
        value = flags & 2 ? 0 : (flags & 1 ? 2 : 1);
        w_u8(context + type + 21, value);
        ++map;
        source_offset += 92;
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 288) & 4;
    if (flags)
    {
        result = r_u32(0x80112DDC);
        w_u8(result + 20, 0);
    }
    else
    {
        context = r_u32(0x80112DDC);
        result = 1;
        w_u8(context + 20, result);
    }
    if (input_index < 0)
        return result;
    record_offset = (uint32)input_index * 92;
    game = r_u32(0x800BCE60);
    source = game + record_offset + 412;
    flags = r_u32(game + 292);
    value = r_u8(source + 3);
    if (flags == 4)
    {
        product = (uint64)(value >> 1) * 0x6BCA1AF3;
        context = r_u32(0x80112DDC);
        quotient = (uint32)(product >> 32) >> 3;
    }
    else
    {
        context = r_u32(0x80112DDC);
        quotient = value >> 4;
    }
    w_u8(context + 27, quotient - 1);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + record_offset + 436);
    if (flags & 8)
    {
        context = r_u32(0x80112DDC);
        result = 1;
        w_u8(context + 180, result);
    }
    else
    {
        result = r_u32(0x80112DDC);
        w_u8(result + 180, 0);
    }
    if (flags & 0x10)
    {
        context = r_u32(0x80112DDC);
        result = 1;
        w_u8(context + 181, result);
    }
    else
    {
        result = r_u32(0x80112DDC);
        w_u8(result + 181, 0);
    }
    return result;
}
