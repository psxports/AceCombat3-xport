#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

DIALOGUE_VIDEO dialogue_video;

static DIALOGUE_VIDEO_PACKETS decoder_packets;

static uint32 capture_pixels[64 * 256 / 2];
static uint8 capture_valid;
static const PSX_RECT capture_rect = {832, 256, 64, 256};
static const PSX_RECT restore_rect = {704, 256, 64, 256};

void dialogue_video_reset(void)
{
    memset(&dialogue_video, 0, sizeof(dialogue_video));
    capture_valid = 0;
}

static const uint8 *decoder_resource_range(const DIALOGUE_RESOURCE_VIEW *resource, uint32 offset, uint32 bytes)
{
    if (!resource->data || offset > resource->bytes || bytes > resource->bytes - offset)
        abort();
    return resource->data + offset;
}

static void *decoder_packet_span(uint32 address, size_t bytes)
{
    DIALOGUE_VIDEO_PACKETS *packets = dialogue_video.decoder.packets;
    uint32 base = dialogue_video.decoder.packet_link;

    if (packets && bytes && address >= base && address - base <= sizeof(*packets) &&
        bytes <= sizeof(*packets) - (address - base))
        return (uint8 *)packets + (address - base);
    abort();
}

static void init_decoder_quads(void)
{
    uint32 bank, owner, count, index, a, b, value;
    POLY_FT4 *poly;
    const DIALOGUE_VIDEO_RECT *screen, *texture;

    FUNCTION_MARKER(0x800EC340, "ACE3.BIN");
    for (bank = 0; bank < 2; ++bank)
    {
        owner = dialogue_video.decoder.packet_link;
        count = dialogue_video.decoder.rect_count;
        if ((sint32)count <= 0)
            continue;
        poly = (POLY_FT4 *)decoder_packet_span(owner + 12 + bank * 160, 4 * sizeof(*poly));
        if (!poly)
            abort();
        index = 0;
        do
        {
            owner = dialogue_video.decoder.packet_link;
            setPolyFT4(poly);
            screen = dialogue_video.decoder.screen_rect + index;
            a = screen->x;
            poly->p[0].xy.x = (sint16)(uint16)a;
            a = screen->y;
            poly->p[0].xy.y = (sint16)(uint16)a;
            a = screen->x;
            b = screen->w;
            poly->p[1].xy.x = (sint16)(uint16)(a + b);
            a = screen->y;
            poly->p[1].xy.y = (sint16)(uint16)a;
            a = screen->x;
            poly->p[2].xy.x = (sint16)(uint16)a;
            a = screen->y;
            b = screen->h;
            poly->p[2].xy.y = (sint16)(uint16)(a + b);
            a = screen->x;
            b = screen->w;
            poly->p[3].xy.x = (sint16)(uint16)(a + b);
            a = screen->y;
            b = screen->h;
            texture = dialogue_video.decoder.source_rect + index;
            poly->p[3].xy.y = (sint16)(uint16)(a + b);
            a = (uint8)texture->x;
            poly->p[0].uv.u = (uint8)a;
            a = (uint8)texture->y;
            poly->p[0].uv.v = (uint8)a;
            a = (uint8)texture->x;
            b = (uint8)texture->w;
            poly->p[1].uv.u = (uint8)(a + b);
            a = (uint8)texture->y;
            poly->p[1].uv.v = (uint8)a;
            a = (uint8)texture->x;
            poly->p[2].uv.u = (uint8)a;
            a = (uint8)texture->y;
            b = (uint8)texture->h;
            poly->p[2].uv.v = (uint8)(a + b);
            a = (uint8)texture->x;
            b = (uint8)texture->w;
            poly->p[3].uv.u = (uint8)(a + b);
            a = (uint8)texture->y;
            b = (uint8)texture->h;
            poly->rgbc.r = 128;
            poly->rgbc.g = 128;
            poly->rgbc.b = 128;
            poly->p[3].uv.v = (uint8)(a + b);
            owner = dialogue_video.decoder.packet_link;
            poly->p[1].attr = 158;
            a = dialogue_video.decoder.palette_rect.y;
            b = dialogue_video.decoder.palette_rect.x;
            value = (a << 6) | ((b >> 4) & 63);
            poly->p[0].attr = (uint16)value;
            count = dialogue_video.decoder.rect_count;
            ++index;
            ++poly;
        } while ((sint32)index < (sint32)count);
    }
    return;
}

static void select_decoder_bank(void)
{
    uint32 buffer, owner, offset;

    FUNCTION_MARKER(0x800EC824, "ACE3.BIN");
    buffer = game_runtime.display_buffer;
    owner = dialogue_video.decoder.packet_link;
    dialogue_video.decoder.quad_cursor = (uint32)(owner + buffer * 160 + 12);
    buffer = game_runtime.display_buffer;
    offset = buffer * 26112 + 448;
    dialogue_video.decoder.tile_cursor = (uint32)(owner + offset);
    { (void)(uint32)(offset); return; }
}

static void link_decoder_quads(uint32 ordering)
{
    uint32 *gpu_word;
    uint32 owner, count, index = 0, packet, value, tag;

    FUNCTION_MARKER(0x800ECBEC, "ACE3.BIN");
    owner = dialogue_video.decoder.packet_link;
    count = dialogue_video.decoder.rect_count;
    if ((sint32)count <= 0)
        { (void)(uint32)(count); return; }
    do
    {
        ++index;
        packet = dialogue_video.decoder.quad_cursor;
        gpu_word = ui_scene_ot_span(ordering, sizeof(*gpu_word));
        value = *gpu_word;
        gpu_word = decoder_packet_span(packet, sizeof(*gpu_word));
        tag = *gpu_word;
        gpu_word = decoder_packet_span(packet, sizeof(*gpu_word));
        *gpu_word = (tag & 0xFF000000) | (value & 0xFFFFFF);
        gpu_word = ui_scene_ot_span(ordering, sizeof(*gpu_word));
        tag = *gpu_word;
        value = dialogue_video.decoder.quad_cursor;
        gpu_word = ui_scene_ot_span(ordering, sizeof(*gpu_word));
        *gpu_word = (tag & 0xFF000000) | (value & 0xFFFFFF);
        value = dialogue_video.decoder.quad_cursor;
        count = dialogue_video.decoder.rect_count;
        value += 40;
        dialogue_video.decoder.quad_cursor = (uint32)(value);
    } while ((sint32)index < (sint32)count);
    { (void)(uint32)(value); return; }
}

typedef char decoder_upload_packet_layout_check[
    sizeof(uint32) == 4 && offsetof(DR_LOAD, code) == 4 && offsetof(DR_LOAD, p) == 16 &&
    sizeof(XY) == 4 && offsetof(XY, x) == 0 && offsetof(XY, y) == 2 ? 1 : -1];

static uint32 emit_decoder_tiles(uint32 ordering)
{
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_14;
    uint32 counter, tick, cursor, link, record_offset, glyph_offset;
    const uint8 *table, *descriptor, *glyph_table, *record, *glyph;
    const DIALOGUE_RESOURCE_VIEW *resource = &dialogue_video.decoder.resource;
    const DIALOGUE_TABLE_VIEW *frames = &dialogue_video.decoder.frame_table;
    const DIALOGUE_TABLE_VIEW *glyphs = &dialogue_video.decoder.glyph_table;
    uint32 value, packet, index, item, position, total, previous_cursor, result = 0;
    uint32 merge_register, address, shift, merged, word, target_shift;
    uint32 *link_word;
    uint32 link_mask;
    sint32 frame, period, start, count, column, row, tile, stride, remainder;
    DR_LOAD load_packet = {0};
    const PSX_RECT load_rect = {0, 0, 4, 4};
    uint32 packet_words, packet_bytes, buffer_begin, used;

    FUNCTION_MARKER(0x800EC888, "ACE3.BIN");
    table = frames->data;
    if (!table || frames->bytes < 4 || dialogue_video.frames > (frames->bytes - 4) / 12)
        abort();
    table = frames->data;
    frame = (sint16)dialogue_video.decoder.frame_idx;
    if (frame >= (sint32)dialogue_video.frames)
        return 1;
    period = (sint16)dialogue_video.decoder.frame_period;
    counter = dialogue_video.decoder.elapsed_ticks;
    tick = game_divide_signed(counter, (uint32)period);
    if (frame < 0)
        abort();
    SetDrawLoad(&load_packet, &load_rect);
    packet_words = getlen(&load_packet);
    packet_bytes = (packet_words + 1) * 4;
    descriptor = table + (uint32)frame * 12 + 4;
    dialogue_video.decoder.frame_cursor = descriptor;
    start = (memcpy(&wire_word_3, descriptor + 4, sizeof(wire_word_3)), wire_word_3);
    cursor = dialogue_video.decoder.tile_cursor;
    buffer_begin = cursor;
    merge_register = 0;
    while ((sint32)tick >= start)
    {
        descriptor = dialogue_video.decoder.frame_cursor;
        column = (sint16)dialogue_video.decoder.image_rect.x;
        row = (sint16)dialogue_video.decoder.image_rect.y;
        count = (memcpy(&wire_word_4, descriptor, sizeof(wire_word_4)), wire_word_4);
        record_offset = (memcpy(&wire_word_5, descriptor + 8, sizeof(wire_word_5)), wire_word_5) + dialogue_video.decoder.records_offset;
        if (count > 0)
        {
            used = cursor - buffer_begin;
            if (used > 26112 || (uint32)count > (26112 - used) / (2 * packet_bytes))
                abort();
            if ((uint32)count > resource->bytes / 4)
                abort();
            record = decoder_resource_range(resource, record_offset, (uint32)count * 4);
            packet = cursor + packet_bytes - 4;
            link = (cursor << 8) + (packet_bytes << 8);
            index = 0;
            do
            {
                tile = (sint16)(memcpy(&wire_halfword_6, record + 2, sizeof(wire_halfword_6)), wire_halfword_6);
                stride = (sint16)dialogue_video.decoder.tile_columns;
                game_divide_signed((uint32)tile, (uint32)stride);
                remainder = tile % stride;
                value = (uint32)column + ((uint32)remainder << 2);
                XY *lower_xy = decoder_packet_span(packet + 12, sizeof(*lower_xy));
                lower_xy->x = (sint16)(uint16)value;
                XY *upper_xy = decoder_packet_span(packet - 40, sizeof(*upper_xy));
                upper_xy->x = (sint16)(uint16)value;
                stride = (sint16)dialogue_video.decoder.tile_columns;
                value = game_divide_signed((uint32)tile, (uint32)stride);
                value = (uint32)row + (value << 3);
                upper_xy->y = (sint16)(uint16)value;
                lower_xy->y = (sint16)(uint16)(value + 4);
                tile = (sint16)(memcpy(&wire_halfword_7, record, sizeof(wire_halfword_7)), wire_halfword_7);
                glyph_table = glyphs->data;
                if (!glyph_table || glyphs->bytes < 516 ||
                    ((glyphs->bytes - 516) / 4) > (glyphs->bytes - 516) / 4 ||
                    tile < 0 || (uint32)tile >= ((glyphs->bytes - 516) / 4))
                    abort();
                glyph_offset = (memcpy(&wire_word_10, glyph_table + ((uint32)tile << 2) + 516, sizeof(wire_word_10)), wire_word_10) + dialogue_video.decoder.pixels_offset;
                glyph = decoder_resource_range(resource, glyph_offset, 64);
                DR_LOAD *upload = decoder_packet_span(cursor, packet_bytes);
                upload->tag = (link >> 8) | (packet_words << 24);
                upload->code[0] = load_packet.code[0];
                upload->code[2] = load_packet.code[2];
                upload->p[8] = load_packet.p[8];
                for (item = 0; item < 8; ++item)
                {
                    value = (memcpy(&wire_word_11, glyph, sizeof(wire_word_11)), wire_word_11);
                    glyph += 4;
                    upload->p[item] = value;
                }
                packet += packet_bytes;
                link += packet_bytes << 8;
                cursor += packet_bytes;
                upload = decoder_packet_span(cursor, packet_bytes);
                upload->tag = (link >> 8) | (packet_words << 24);
                upload->code[0] = load_packet.code[0];
                upload->code[2] = load_packet.code[2];
                upload->p[8] = load_packet.p[8];
                for (item = 0; item < 8; ++item)
                {
                    value = (memcpy(&wire_word_12, glyph, sizeof(wire_word_12)), wire_word_12);
                    glyph += 4;
                    upload->p[item] = value;
                }
                packet += packet_bytes;
                link += packet_bytes << 8;
                cursor += packet_bytes;
                ++index;
                record += 4;
            } while ((sint32)index < count);
        }
        position = dialogue_video.decoder.frame_idx + 1;
        dialogue_video.decoder.frame_idx = (uint16)(position);
        frame = (sint16)position;
        table = frames->data;
        if (frame < 0 || (uint32)frame > (frames->bytes - 4) / 12)
            abort();
        descriptor = table + (uint32)frame * 12 + 4;
        value = dialogue_video.decoder.bank;
        dialogue_video.decoder.frame_cursor = descriptor;
        table = frames->data;
        dialogue_video.decoder.bank = (uint32)(1 - value);
        frame = (sint16)dialogue_video.decoder.frame_idx;
        total = dialogue_video.frames;
        if (total > (frames->bytes - 4) / 12)
            abort();
        merge_register = total;
        if (frame >= (sint32)total)
        {
            result = 1;
            break;
        }
        descriptor = dialogue_video.decoder.frame_cursor;
        start = (memcpy(&wire_word_14, descriptor + 4, sizeof(wire_word_14)), wire_word_14);
    }
    previous_cursor = dialogue_video.decoder.tile_cursor;
    if (cursor != previous_cursor)
    {
        address = ordering + 2;
        shift = (3 - (address & 3)) << 3;
        link_word = ui_scene_ot_span(address & 0xFFFFFFFC, sizeof(*link_word));
        word = *link_word;
        merged = word << shift;
        if (shift)
            merged |= merge_register & (0xFFFFFFFF >> (32 - shift));
        address = cursor - packet_bytes + 2;
        target_shift = (3 - (address & 3)) << 3;
        link_word = decoder_packet_span(address & 0xFFFFFFFC, sizeof(*link_word));
        link_mask = 0xFFFFFFFFu >> target_shift;
        *link_word = (*link_word & ~link_mask) | ((merged >> target_shift) & link_mask);
        address = ordering + 2;
        link_word = ui_scene_ot_span(address & 0xFFFFFFFC, sizeof(*link_word));
        link_mask = 0xFFFFFFFFu >> shift;
        *link_word = (*link_word & ~link_mask) | (((previous_cursor << 8) >> shift) & link_mask);
        dialogue_video.decoder.tile_cursor = (uint32)(cursor);
    }
    return result;
}

static uint32 draw_decoder_tiles(uint32 ordering)
{
    FUNCTION_MARKER(0x800EC880, "ACE3.BIN");
    return emit_decoder_tiles(ordering);
}

static void setup_decoder(const PSX_RECT *rectangle, const DIALOGUE_VIDEO_RECT *source_rectangles, const PSX_RECT *palette, const DIALOGUE_VIDEO_RECT *destination_rectangles, uint32 count)
{
    uint32 stage, index, value, palette_offset, palette_bytes;
    const uint8 *table;
    const PSX_RECT *source_rect;
    PSX_RECT bounds;
    const DIALOGUE_VIDEO_RECT *rect;
    DIALOGUE_VIDEO_RECT *destination_rect;

    FUNCTION_MARKER(0x800EC4EC, "ACE3.BIN");
    for (stage = 0; stage < 2; ++stage)
    {
        source_rect = stage == 0 ? palette : rectangle;
        destination_rect = stage == 0 ? &dialogue_video.decoder.palette_rect : &dialogue_video.decoder.image_rect;
        destination_rect->x = (uint16)source_rect->x;
        destination_rect->y = (uint16)source_rect->y;
        destination_rect->w = (uint16)source_rect->w;
        destination_rect->h = (uint16)source_rect->h;
    }
    dialogue_video.decoder.rect_count = (uint32)(count);
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        for (stage = 0; stage < 2; ++stage)
        {
            rect = (stage == 0 ? source_rectangles : destination_rectangles) + index;
            destination_rect = (stage == 0 ? dialogue_video.decoder.source_rect : dialogue_video.decoder.screen_rect) + index;
            destination_rect->x = rect->x;
            destination_rect->y = rect->y;
            destination_rect->w = rect->w;
            destination_rect->h = rect->h;
        }
    }
    value = game_runtime.display_buffer;
    table = dialogue_video.decoder.frame_table.data;
    dialogue_video.decoder.bank = (uint32)(value);
    dialogue_video.decoder.frame_cursor = table + 4;
    bounds = *rectangle;
    (void)ClearImage(&bounds, 0, 0, 0);
    for (index = 254; (sint32)index >= 0; --index)
    {
    }
    table = dialogue_video.decoder.glyph_table.data;
    bounds = *palette;
    palette_offset = (uint32)(table - dialogue_video.decoder.resource.data) + 4;
    palette_bytes = (uint32)bounds.w * bounds.h * 2;
    if (palette_offset > dialogue_video.decoder.resource.bytes ||
        palette_bytes > dialogue_video.decoder.resource.bytes - palette_offset)
        abort();
    (void)LoadImagePSX(&bounds, (uint32 *)(dialogue_video.decoder.resource.data + palette_offset));
    value = (sint32)rectangle->w;
    if ((sint32)value < 0)
        value += 7;
    dialogue_video.decoder.tile_columns = (uint16)((uint32)((sint32)value >> 3));
    value = (sint32)rectangle->h;
    if ((sint32)value < 0)
        value += 7;
    dialogue_video.decoder.tile_rows = (uint16)((uint32)((sint32)value >> 3));
    dialogue_video.decoder.tile_word_width = (uint16)(4);
    dialogue_video.decoder.tile_half_rows = (uint16)(4);
    dialogue_video.decoder.elapsed_ticks = (uint32)(0);
    dialogue_video.decoder.frame_period = (uint16)(4);
    dialogue_video.decoder.frame_idx = (uint16)(0);
    init_decoder_quads();
    dialogue_video.decoder.draw_calls = (uint32)(0);
    return;
}

static uint32 load_decoder(const PSX_RECT *rectangle, const DIALOGUE_VIDEO_RECT *source_rectangles, const PSX_RECT *palette, const DIALOGUE_VIDEO_RECT *destination_rectangles, const DIALOGUE_RESOURCE_VIEW *resource, uint32 count)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_8;
    uint32 wire_word_10;
    uint32 wire_word_13;
    uint32 owner, value, bound;
    uint32 frame_offset, glyph_offset;
    const uint8 *entries, *table;
    uint32 directory_count, resource_bytes;
    const uint8 *payload;

    FUNCTION_MARKER(0x800EC6B4, "ACE3.BIN");
    (void)arena_alloc(0xCDCC);
    if (!gpu_register_packet_range(&decoder_packets, sizeof(decoder_packets)))
        abort();
    dialogue_video.decoder.packet_link = gpu_packet_link((uintptr_t)&decoder_packets);
    dialogue_video.decoder.packets = &decoder_packets;
    resource_bytes = resource->bytes;
    if (resource_bytes < 20)
        abort();
    payload = resource->data;
    if (!payload)
        abort();
    directory_count = (memcpy(&wire_halfword_0, payload, sizeof(wire_halfword_0)), wire_halfword_0);
    if (directory_count < 4 || directory_count > (resource_bytes - 4) / 4)
        abort();
    entries = resource->data + 4;
    owner = dialogue_video.decoder.packet_link;
    value = (memcpy(&wire_word_1, entries, sizeof(wire_word_1)), wire_word_1);
    dialogue_video.decoder.resource = *resource;
    frame_offset = value;
    dialogue_video.decoder.frame_table.data = decoder_resource_range(resource, frame_offset, 4);
    dialogue_video.decoder.frame_table.bytes = 4;
    value = (memcpy(&wire_word_2, entries + 8, sizeof(wire_word_2)), wire_word_2);
    table = dialogue_video.decoder.frame_table.data;
    glyph_offset = value;
    dialogue_video.decoder.glyph_table.data = decoder_resource_range(resource, glyph_offset, 516);
    dialogue_video.decoder.glyph_table.bytes = 516;
    bound = (memcpy(&wire_word_3, table, sizeof(wire_word_3)), wire_word_3);
    if (bound > (resource_bytes - frame_offset - 4) / 12)
        abort();
    dialogue_video.decoder.frame_table.bytes = 4 + bound * 12;
    (void)decoder_resource_range(resource, frame_offset, dialogue_video.decoder.frame_table.bytes);
    dialogue_video.decoder.records_offset =
        (memcpy(&wire_word_4, entries + 4, sizeof(wire_word_4)), wire_word_4);
    owner = dialogue_video.decoder.packet_link;
    table = dialogue_video.decoder.glyph_table.data;
    bound = (memcpy(&wire_word_8, table, sizeof(wire_word_8)), wire_word_8);
    if (bound > (resource_bytes - glyph_offset - 516) / 4)
        abort();
    dialogue_video.decoder.glyph_table.bytes = 516 + bound * 4;
    (void)decoder_resource_range(resource, glyph_offset, dialogue_video.decoder.glyph_table.bytes);
    dialogue_video.decoder.pixels_offset =
        (memcpy(&wire_word_10, entries + 12, sizeof(wire_word_10)), wire_word_10);
    setup_decoder(rectangle, source_rectangles, palette, destination_rectangles, count);
    owner = dialogue_video.decoder.packet_link;
    table = dialogue_video.decoder.frame_table.data;
    return (memcpy(&wire_word_13, table, sizeof(wire_word_13)), wire_word_13);
}

void dialogue_video_init(uint32 kind)
{
    PSX_RECT rectangle, palette;
    DIALOGUE_RESOURCE_VIEW resource;
    uint32 width, height, count, result;

    FUNCTION_MARKER(0x800EDCC0, "ACE3.BIN");
    /* Initializes dimensions omitted by the original default branch */
    width = 0;
    height = 0;
    if (kind < 10)
    {
        switch (kind)
        {
            case 0:
            case 1:
            case 6:
                width = 80;
                height = 80;
                break;
            case 3:
                width = 144;
                height = 64;
                break;
            case 4:
            case 9:
                width = 128;
                height = 96;
                break;
            case 5:
                width = 160;
                height = 64;
                break;
            case 2:
                width = 160;
                height = 80;
                break;
            case 7:
            case 8:
                break;
            default:
#ifndef NDEBUG
                fprintf(stderr, "Unbound resource002 upload dimension kind %u\n", kind);
#endif
                abort();
        }
    }
    rectangle.x = 896;
    rectangle.y = 256;
    rectangle.w = (sint16)width;
    rectangle.h = (sint16)height;
    palette.x = 768;
    palette.y = 1;
    palette.w = 256;
    palette.h = 1;
    resource = dialogue_video.resource;
    count = dialogue_video.rect_count;
    result = load_decoder(&rectangle, dialogue_video.tex, &palette, dialogue_video.scr, &resource, count);
    dialogue_video.frames = (uint32)(result);
    { (void)(uint32)(result); return; }
}

static uint32 draw_decoder_frame(uint32 ordering, uint32 *frame_ret)
{
    uint32 buffer, owner, result, value;

    FUNCTION_MARKER(0x800ECC78, "ACE3.BIN");
    buffer = game_runtime.display_buffer;
    owner = dialogue_video.decoder.packet_link;
    dialogue_video.decoder.quad_cursor = (uint32)(owner + buffer * 160 + 12);
    select_decoder_bank();
    result = draw_decoder_tiles(ordering);
    link_decoder_quads(ordering);
    owner = dialogue_video.decoder.packet_link;
    value = dialogue_video.decoder.elapsed_ticks;
    buffer = (sint16)dialogue_video.decoder.frame_idx;
    dialogue_video.decoder.elapsed_ticks = (uint32)(value + 1);
    *frame_ret = buffer;
    owner = dialogue_video.decoder.packet_link;
    value = dialogue_video.decoder.draw_calls;
    dialogue_video.decoder.draw_calls = (uint32)(value + 1);
    return result;
}

uint32 dialogue_video_draw(uint32 ordering, uint32 *frame_ret)
{
    FUNCTION_MARKER(0x800ECC70, "ACE3.BIN");
    return draw_decoder_frame(ordering, frame_ret);
}

typedef char dialogue_video_rectangle_layout_check[
    sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 &&
    sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

uint32 dialogue_video_draw_rect(uint32 packet, uint32 ordering, const DIALOGUE_VIDEO_RECT *screen, const DIALOGUE_VIDEO_RECT *texture)
{
    uint32 a, b, offset, c, tag, link;

    POLY_FT4 *quad = ui_packet_span(packet, sizeof(*quad));
    uint32 *entry = ui_scene_ot_span(ordering, sizeof(*entry));
    FUNCTION_MARKER(0x800FD48C, "ACE3.BIN");
    quad->tag = (quad->tag & 0xFFFFFF) | 0x09000000;
    quad->rgbc.code = (uint8)(44);
    quad->rgbc.r = (uint8)(128);
    quad->rgbc.g = (uint8)(128);
    quad->rgbc.b = (uint8)(128);
    a = screen->x;
    quad->p[0].xy.x = (sint16)(uint16)(a);
    a = screen->y;
    quad->p[0].xy.y = (sint16)(uint16)(a);
    a = screen->x;
    b = screen->w;
    quad->p[1].xy.x = (sint16)(uint16)(a + b);
    a = screen->y;
    quad->p[1].xy.y = (sint16)(uint16)(a);
    a = screen->x;
    quad->p[2].xy.x = (sint16)(uint16)(a);
    a = screen->y;
    b = screen->h;
    quad->p[2].xy.y = (sint16)(uint16)(a + b);
    a = screen->x;
    b = screen->w;
    quad->p[3].xy.x = (sint16)(uint16)(a + b);
    a = screen->y;
    b = screen->h;
    quad->p[3].xy.y = (sint16)(uint16)(a + b);
    a = (uint8)texture->x;
    quad->p[0].uv.u = (uint8)(a);
    a = overlay_movie_display_page();
    b = (uint8)texture->y;
    offset = (1 - a) << 7;
    quad->p[0].uv.v = (uint8)(b + offset);
    a = (uint8)texture->x;
    b = (uint8)texture->w;
    quad->p[1].uv.u = (uint8)(a + b);
    a = overlay_movie_display_page();
    b = (uint8)texture->y;
    offset = (1 - a) << 7;
    quad->p[1].uv.v = (uint8)(b + offset);
    a = (uint8)texture->x;
    quad->p[2].uv.u = (uint8)(a);
    a = (uint8)texture->h;
    b = overlay_movie_display_page();
    c = (uint8)texture->y;
    offset = (1 - b) << 7;
    quad->p[2].uv.v = (uint8)(a + c + offset);
    a = (uint8)texture->x;
    b = (uint8)texture->w;
    quad->p[3].uv.u = (uint8)(a + b);
    a = (uint8)texture->y;
    b = (uint8)texture->h;
    c = overlay_movie_display_page();
    quad->p[1].attr = (uint16)(285);
    offset = (1 - c) << 7;
    quad->p[3].uv.v = (uint8)(a + b + offset);
    tag = quad->tag;
    link = *entry;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *entry;
    *entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 40;
}

void dialogue_video_move_capture(void)
{
    PSX_RECT rect = restore_rect;

    (void)MoveImage(&rect, capture_rect.x, capture_rect.y);
}

void dialogue_video_clear_capture(void)
{
    capture_valid = 0;
}

void dialogue_video_store_capture(uint32 available)
{
    PSX_RECT rect = capture_rect;

    capture_valid = (uint8)(available != 0);
    (void)StoreImage(&rect, capture_pixels);
}

void dialogue_video_restore_capture(void)
{
    PSX_RECT rect = restore_rect;

    if (capture_valid)
    {
        (void)LoadImagePSX(&rect, capture_pixels);
        DrawSync(0);
    }
}
