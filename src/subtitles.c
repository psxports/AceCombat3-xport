#include <stddef.h>

#include "game_functions.h"

typedef struct
{
    uint8 width;
    uint8 height;
    uint8 columns;
    uint8 rgb[3];
    uint8 semi_transparent;
    uint8 x;
    uint16 y[3];
    uint8 duration_units;
    uint8 flags;
    uint16 texture_x;
} SUBTITLE_DESCRIPTOR;

static const SUBTITLE_DESCRIPTOR subtitle_descriptors[11] = {
    {180, 10, 25, {0, 115, 0}, 1, 132, {22, 35, 0}, 8, 16, 640}, {250, 10, 25, {99, 99, 99}, 1, 35, {201, 211, 0}, 6, 16, 640}, {250, 20, 12, {131, 131, 131}, 0, 35, {314, 342, 370}, 12, 0, 576}, {250, 20, 12, {131, 131, 131}, 0, 35, {376, 402, 0}, 120, 0, 576}, {200, 20, 12, {131, 131, 131}, 0, 38, {314, 338, 362}, 16, 0, 576}, {200, 20, 12, {131, 131, 131}, 0, 46, {303, 325, 347}, 12, 0, 576}, {135, 114, 2, {131, 131, 131}, 0, 112, {88, 208, 0}, 40, 0, 576}, {250, 10, 25, {131, 131, 131}, 0, 36, {214, 0, 0}, 255, 16, 640}, {250, 20, 12, {91, 91, 91}, 0, 38, {314, 338, 362}, 12, 16, 576}, {250, 20, 12, {131, 131, 131}, 0, 54, {214, 0, 0}, 255, 16, 640}, {250, 10, 25, {131, 131, 131}, 0, 36, {420, 0, 0}, 255, 16, 640},
};

typedef struct
{
    uint8 u, v, w, h;
} SUBTITLE_SPEAKER_SPRITE;

static const SUBTITLE_SPEAKER_SPRITE subtitle_speaker_sprites[30] = {
    {0, 128, 28, 5}, {28, 128, 30, 5}, {58, 128, 28, 5}, {86, 128, 32, 5}, {118, 128, 44, 5}, {162, 128, 32, 5}, {0, 133, 28, 5}, {28, 133, 30, 5}, {86, 133, 56, 5}, {86, 133, 56, 5}, {58, 133, 28, 5}, {142, 133, 66, 5}, {0, 138, 28, 5}, {194, 128, 20, 5}, {142, 133, 66, 5}, {28, 138, 42, 5}, {70, 138, 28, 5}, {194, 128, 20, 5}, {86, 133, 56, 5}, {0, 0, 0, 0}, {98, 138, 62, 5}, {194, 128, 20, 5}, {0, 143, 30, 5}, {160, 138, 56, 5}, {30, 143, 68, 5}, {30, 143, 68, 5}, {98, 143, 54, 5}, {0, 148, 36, 5}, {152, 143, 60, 5}, {36, 148, 60, 5},
};

typedef struct
{
    uint32 *native_ordering;
    uint8 *native_packets;
    uint32 encoded_ordering;
    uint32 encoded_packets;
    uint32 capacity;
} SUBTITLE_OUTPUT;

static uint32 *subtitles_output_ordering(SUBTITLE_OUTPUT *output)
{
    return output->native_ordering ? output->native_ordering : ui_scene_ot_span(output->encoded_ordering, sizeof(uint32));
}

static void *subtitles_output_packet(SUBTITLE_OUTPUT *output, uint32 offset, uint32 bytes)
{
    if (!output->native_packets)
        return ui_packet_span(output->encoded_packets + offset, bytes);
    if (offset > output->capacity || bytes > output->capacity - offset)
        abort();
    return output->native_packets + offset;
}

static void subtitles_update_slots(void);
static uint32 subtitles_draw_sprites(SUBTITLE_OUTPUT *output, uint32 packet);
static uint32 subtitles_draw_stretched(SUBTITLE_OUTPUT *output, uint32 packet);
static uint32 subtitles_draw_mode_9(SUBTITLE_OUTPUT *output, uint32 packet);
static uint32 subtitles_draw_speaker(SUBTITLE_OUTPUT *output, uint32 packet, uint32 mode);

void subtitles_initialize(void)
{
    uint32 i;
    GAME_SUBTITLE_STATE *state;

    FUNCTION_MARKER(0x8002CBCC, "ACE3.BIN");
    game_runtime.subtitles.requested[2] = (uint8)(0);
    game_runtime.subtitles.requested[1] = (uint8)(0);
    game_runtime.subtitles.requested[0] = (uint8)(0);
    game_runtime.subtitles.metadata_flags = (uint8)(0);
    game_runtime.subtitles.speaker = (uint8)(0);
    for (i = 0; i < 3; ++i)
    {
        game_runtime.subtitles.visible[i] = (uint8)(0);
        game_runtime.subtitles.latched[i] = (uint8)(0);
        state = &game_runtime.subtitles;
        state->elapsed[i] = (uint16)(0);
    }
    state->clut = (uint16)(0x4A16);
    state->tpage = (uint16)(58);
    return;
}

uint32 subtitles_configure(uint32 id, uint32 argument0, uint32 argument1)
{
    uint32 i;
    GAME_SUBTITLE_STATE *context = 0;
    const SUBTITLE_DESCRIPTOR *descriptor;
    uint32 value;

    FUNCTION_MARKER(0x8002CC44, "ACE3.BIN");
    game_runtime.subtitles.requested[2] = (uint8)(0);
    game_runtime.subtitles.requested[1] = (uint8)(0);
    game_runtime.subtitles.requested[0] = (uint8)(0);
    game_runtime.subtitles.metadata_flags = (uint8)(0);
    game_runtime.subtitles.speaker = (uint8)(0);
    for (i = 0; i < 3; ++i)
    {
        game_runtime.subtitles.visible[i] = (uint8)(0);
        game_runtime.subtitles.latched[i] = (uint8)(0);
        context = &game_runtime.subtitles;
        context->elapsed[i] = (uint16)(0);
    }
    if (id == 255)
    {
        game_runtime.subtitles.enabled = (uint8)(0);
        return 0;
    }
    descriptor = &subtitle_descriptors[id];
    game_runtime.subtitles.enabled = (uint8)(1);
    context->descriptor_id = (uint32)(id);
    context->arguments[0] = (uint32)(argument0);
    context->arguments[1] = (uint32)(argument1);
    value = descriptor->columns;
    context->extents[0] = (uint16)(value * 4);
    value = descriptor->columns;
    context->extents[1] = (uint16)(value * 6);
    value = descriptor->texture_x;
    game_runtime.subtitles.last_frame = (uint32)(0xFFFFFFFF);
    context->tpage = (uint16)(((value & 0x3FF) >> 6) | 0x30);
    return 0;
}

static void subtitles_update_slots(void)
{
    uint32 i;
    GAME_SUBTITLE_STATE *context;
    uint32 value;
    uint32 changes = 0;
    const SUBTITLE_DESCRIPTOR *descriptor;
    uint32 counter;
    sint32 threshold;

    FUNCTION_MARKER(0x8002CD24, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        context = &game_runtime.subtitles;
        value = game_runtime.subtitles.requested[i];
        if (value == context->latched[i])
        {
            descriptor = &subtitle_descriptors[context->descriptor_id];
            threshold = descriptor->duration_units * 30;
            counter = context->elapsed[i];
            if ((sint16)context->elapsed[i] >= threshold)
                ++changes;
            else if (!(game_runtime.flags & 0x20000000))
                context->elapsed[i] = (uint16)(counter + 1);
        }
        else
        {
            context->latched[i] = (uint8)(value);
            context = &game_runtime.subtitles;
            value = game_runtime.subtitles.requested[i];
            context->visible[i] = (uint8)(value);
            context = &game_runtime.subtitles;
            changes = i == 0 ? 128 : 0;
            context->elapsed[i] = (uint16)(0);
        }
    }
    if ((changes & 255) == 3)
    {
        for (i = 0; i < 3; ++i)
            game_runtime.subtitles.visible[i] = (uint8)(0);
    }
    else if (changes & 128)
    {
        for (i = 1; i < 3; ++i)
            game_runtime.subtitles.visible[i] = (uint8)(0);
    }
    return;
}

typedef struct
{
    SPRT sprite;
    DR_TPAGE page;
} SUBTITLE_SPRITE_PACKET;

typedef char subtitle_packet_layout_check[sizeof(SUBTITLE_SPRITE_PACKET) == 28 && offsetof(SUBTITLE_SPRITE_PACKET, page) == 20 && sizeof(SPRT) == 20 && offsetof(SPRT, tag) == 0 && offsetof(SPRT, rgbc) == 4 && offsetof(SPRT, p) == 8 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, tag) == 0 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, xy) == 0 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && offsetof(XY, y) == 2 && offsetof(UV, v) == 1 && offsetof(RGBC, code) == 3 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(DR_MOVE) == 24 && offsetof(DR_MOVE, code) == 4 ? 1 : -1];

static uint32 subtitles_draw_speaker(SUBTITLE_OUTPUT *output, uint32 packet, uint32 mode)
{
    SUBTITLE_SPRITE_PACKET *primitive;
    uint32 id;
    const SUBTITLE_DESCRIPTOR *descriptor;
    const SUBTITLE_SPEAKER_SPRITE *sprite;
    uint32 value;

    FUNCTION_MARKER(0x8002D560, "ACE3.BIN");
    id = game_runtime.subtitles.descriptor_id;
    primitive = (SUBTITLE_SPRITE_PACKET *)subtitles_output_packet(output, packet, sizeof(*primitive));
    ((uint8 *)&primitive->sprite.tag)[3] = (uint8)(4);
    primitive->sprite.rgbc.code = (uint8)(0x64);
    descriptor = &subtitle_descriptors[id];
    value = descriptor->semi_transparent;
    primitive->sprite.rgbc.code = (uint8)(value ? 0x66 : 0x64);
    primitive->sprite.rgbc.r = (uint8)(descriptor->rgb[0]);
    primitive->sprite.rgbc.g = (uint8)(descriptor->rgb[1]);
    primitive->sprite.rgbc.b = (uint8)(descriptor->rgb[2]);
    if (!game_runtime.subtitles.descriptor_id)
    {
        primitive->sprite.p[0].xy.x = (sint16)(uint16)(132);
        value = 56;
    }
    else
    {
        primitive->sprite.p[0].xy.x = (sint16)(uint16)(descriptor->x - 4);
        value = descriptor->y[0] - 6;
    }
    primitive->sprite.p[0].xy.y = (sint16)(uint16)(value);
    sprite = &subtitle_speaker_sprites[mode];
    primitive->sprite.p[0].uv.u = (uint8)(sprite->u);
    primitive->sprite.p[0].uv.v = (uint8)(sprite->v);
    primitive->sprite.w = (sint16)(uint16)(sprite->w);
    value = sprite->h;
    primitive->sprite.p[0].attr = (uint16)(0x7FD8);
    primitive->sprite.h = (sint16)(uint16)(value);
    AddPrim(subtitles_output_ordering(output), primitive);
    ((uint8 *)&primitive->page.tag)[3] = (uint8)(1);
    primitive->page.code[0] = (uint32)(0xE1000236);
    AddPrim(subtitles_output_ordering(output), &primitive->page);
    return packet + sizeof(*primitive);
}

static uint32 subtitles_draw_sprites(SUBTITLE_OUTPUT *output, uint32 packet)
{
    SUBTITLE_SPRITE_PACKET *primitive;
    GAME_SUBTITLE_STATE *context;
    uint32 id;
    const SUBTITLE_DESCRIPTOR *descriptor;
    uint32 i;
    uint32 count = 0;
    uint32 glyph;
    uint32 columns;
    uint32 row;
    uint32 column;
    uint32 v;
    uint32 clut;
    uint32 page;
    uint32 value;

    FUNCTION_MARKER(0x8002CE70, "ACE3.BIN");
    context = &game_runtime.subtitles;
    id = context->descriptor_id;
    descriptor = &subtitle_descriptors[id];
    if (!id && game_runtime.scene.mode == 2 && (game_runtime.mission_control.flags & 1))
        return packet;
    for (i = 0; i < 3; ++i)
    {
        context = &game_runtime.subtitles;
        glyph = context->visible[i];
        if (!glyph)
            continue;
        columns = descriptor->columns;
        --glyph;
        if (!columns)
            trap(7);
        row = glyph / columns;
        column = glyph % columns;
        v = column * descriptor->height;
        ++count;
        primitive = (SUBTITLE_SPRITE_PACKET *)subtitles_output_packet(output, packet, sizeof(*primitive));
        ((uint8 *)&primitive->sprite.tag)[3] = (uint8)(4);
        primitive->sprite.rgbc.code = (uint8)(0x64);
        clut = (row & 7) << 6;
        page = row >> 1;
        value = descriptor->semi_transparent;
        primitive->sprite.rgbc.code = (uint8)(value ? 0x66 : 0x64);
        primitive->sprite.rgbc.r = (uint8)(descriptor->rgb[0]);
        primitive->sprite.rgbc.g = (uint8)(descriptor->rgb[1]);
        primitive->sprite.rgbc.b = (uint8)(descriptor->rgb[2]);
        primitive->sprite.p[0].xy.x = (sint16)(uint16)(descriptor->x);
        value = descriptor->y[i];
        primitive->sprite.p[0].uv.u = (uint8)(0);
        context = &game_runtime.subtitles;
        primitive->sprite.p[0].xy.y = (sint16)(uint16)(value);
        primitive->sprite.w = (sint16)(uint16)(descriptor->width);
        primitive->sprite.h = (sint16)(uint16)(descriptor->height);
        if (context->descriptor_id == 6 && i == 1)
            primitive->sprite.h = (sint16)(uint16)(138);
        game_runtime.subtitles.clut = (uint16)(0x4A16);
        primitive->sprite.p[0].attr = (uint16)(clut + 0x4A16);
        primitive->sprite.p[0].uv.v = (uint8)(v);
        AddPrim(subtitles_output_ordering(output), primitive);
        ((uint8 *)&primitive->page.tag)[3] = (uint8)(1);
        value = game_runtime.subtitles.tpage;
        primitive->page.code[0] = (uint32)(((value + page) & 0x9FF) | 0xE1000200);
        AddPrim(subtitles_output_ordering(output), &primitive->page);
        packet += sizeof(*primitive);
    }
    if ((descriptor->flags & 0x10) && count)
    {
        value = game_runtime.subtitles.speaker;
        if (value)
            return subtitles_draw_speaker(output, packet, value - 1);
    }
    return packet;
}

static uint32 subtitles_draw_stretched(SUBTITLE_OUTPUT *output, uint32 packet)
{
    POLY_FT4 *primitive;
    GAME_SUBTITLE_STATE *state;
    const SUBTITLE_DESCRIPTOR *table;
    const uint16 *row;
    uint32 index, frame, divisor, quotient, remainder, height, v, a, b, y;
    FUNCTION_MARKER(0x8002D10C, "ACE3.BIN");
    state = &game_runtime.subtitles;
    index = state->descriptor_id;
    table = &subtitle_descriptors[index];
    row = table->y;
    for (index = 0; index < 3; ++index, ++row)
    {
        state = &game_runtime.subtitles;
        frame = state->visible[index];
        if (frame == 0)
            continue;
        divisor = table->columns;
        --frame;
        if (divisor == 0)
            xport_mips_break(7);
        quotient = frame / divisor;
        remainder = frame % divisor;
        height = table->height;
        v = remainder * height;
        primitive = (POLY_FT4 *)subtitles_output_packet(output, packet, sizeof(*primitive));
        ((uint8 *)&primitive->tag)[3] = (uint8)(9);
        primitive->rgbc.code = (uint8)(44);
        a = table->semi_transparent;
        primitive->rgbc.code = (uint8)(a != 0 ? 46 : 44);
        a = table->rgb[0];
        primitive->rgbc.r = (uint8)(a);
        a = table->rgb[1];
        primitive->rgbc.g = (uint8)(a);
        a = table->rgb[2];
        primitive->rgbc.b = (uint8)(a);
        a = table->x;
        primitive->p[0].xy.x = (sint16)(uint16)(a);
        a = *row;
        primitive->p[0].xy.y = (sint16)(uint16)(a);
        a = table->x;
        b = table->width;
        primitive->p[1].xy.x = (sint16)(uint16)(a + b);
        a = *row;
        primitive->p[1].xy.y = (sint16)(uint16)(a);
        a = table->x;
        primitive->p[2].xy.x = (sint16)(uint16)(a);
        y = *row;
        height = table->height;
        primitive->p[2].xy.y = (sint16)(uint16)(y - 1 + (height << 1));
        a = table->x;
        b = table->width;
        primitive->p[3].xy.x = (sint16)(uint16)(a + b);
        y = *row;
        height = table->height;
        primitive->p[0].uv.u = (uint8)(0);
        primitive->p[0].uv.v = (uint8)(v);
        primitive->p[3].xy.y = (sint16)(uint16)(y - 1 + (height << 1));
        a = table->width;
        primitive->p[1].uv.v = (uint8)(v);
        primitive->p[2].uv.u = (uint8)(0);
        primitive->p[1].uv.u = (uint8)(a);
        height = table->height;
        primitive->p[2].uv.v = (uint8)(height + v);
        a = table->width;
        primitive->p[3].uv.u = (uint8)(a);
        height = table->height;
        primitive->p[3].uv.v = (uint8)(height + v);
        state = &game_runtime.subtitles;
        primitive->p[0].attr = (uint16)(((quotient & 7) << 6) + 0x4A16);
        a = state->tpage;
        primitive->p[1].attr = (uint16)(a + (quotient >> 1));
        AddPrim(subtitles_output_ordering(output), primitive);
        packet += sizeof(*primitive);
    }
    return packet;
}

static uint32 subtitles_draw_mode_9(SUBTITLE_OUTPUT *output, uint32 packet)
{
    DR_MOVE *primitive;
    GAME_SUBTITLE_STATE *context;
    uint32 y;
    uint32 glyph;

    FUNCTION_MARKER(0x8002D330, "ACE3.BIN");
    context = &game_runtime.subtitles;
    if (!context->visible[0])
        return packet;
    y = movie_captions.display_page == 1 ? 470 : 214;
    primitive = (DR_MOVE *)subtitles_output_packet(output, packet, sizeof(*primitive));
    primitive->tag = (uint32)(0x05000000);
    primitive->code[0] = (uint32)(0x01000000);
    primitive->code[1] = (uint32)(0x80000000);
    glyph = context->visible[0];
    primitive->code[4] = (uint32)(0x000A0180);
    primitive->code[3] = (uint32)((y << 16) | 0x36);
    glyph = (glyph - 1) * 5;
    primitive->code[2] = (uint32)((glyph << 17) | 0x240);
    AddPrim(subtitles_output_ordering(output), primitive);
    return packet + sizeof(*primitive);
}

static uint32 subtitles_draw_frame(SUBTITLE_OUTPUT *output, uint32 packet, uint32 ready, uint32 frame)
{
    GAME_SUBTITLE_STATE *context;
    uint32 id;

    if (ready)
    {
        if (frame != 0xFFFFFFFF && game_runtime.subtitles.last_frame != frame)
        {
            game_runtime.subtitles.requested[0] = (uint8)(frame >> 25);
            game_runtime.subtitles.requested[1] = (uint8)((frame >> 18) & 127);
            game_runtime.subtitles.requested[2] = (uint8)((frame >> 11) & 127);
            game_runtime.subtitles.speaker = (uint8)((frame >> 5) & 63);
            game_runtime.subtitles.metadata_flags = (uint8)((frame >> 2) & 7);
        }
        game_runtime.subtitles.last_frame = (uint32)(frame);
    }
    subtitles_update_slots();
    context = &game_runtime.subtitles;
    id = context->descriptor_id;
    if ((id < 2 || id == 7 || id == 10 || id == 9) && game_runtime.options.captions != 1)
        return packet;
    if (game_runtime.subtitles.enabled != 1)
        return packet;
    id = game_runtime.subtitles.descriptor_id;
    if (id == 9)
        return subtitles_draw_mode_9(output, packet);
    if (id == 10)
        return subtitles_draw_stretched(output, packet);
    return subtitles_draw_sprites(output, packet);
}

uint32 subtitles_draw_movie(uint32 *ordering, void *packets, uint32 capacity)
{
    SUBTITLE_OUTPUT output = {ordering, packets, 0, 0, capacity};
    const RESOURCE_FRAME *metadata = resource_get_frame_metadata();
    return subtitles_draw_frame(&output, 0, metadata && metadata->kind, metadata ? metadata->frame : 0);
}

uint32 subtitles_draw(uint32 ot, uint32 packet)
{
    SUBTITLE_OUTPUT output = {NULL, NULL, ot, packet, 0};
    uint32 frame = 0;
    uint32 ready;
    FUNCTION_MARKER(0x8002D3F4, "ACE3.BIN");
    ready = resource_poll_frame(&frame, 0);
    return packet + subtitles_draw_frame(&output, 0, ready, frame);
}

uint32 subtitles_has_visible(void)
{
    FUNCTION_MARKER(0x8002D544, "ACE3.BIN");
    return (game_runtime.subtitles.visible[0] | game_runtime.subtitles.visible[1] | game_runtime.subtitles.visible[2]) != 0;
}

void subtitles_submit_frame(void)
{
    uint32 ot, packet, result;
    FUNCTION_MARKER(0x800337DC, "ACE3.BIN");
    ot = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    result = subtitles_draw(ot, packet);
    game_runtime.mission_control.packet_cur = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}
