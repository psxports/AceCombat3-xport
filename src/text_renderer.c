#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint8 *data;
    uint32 capacity;
    uint32 cursor;
} FORMAT_OUTPUT;

static const uint8 title_glyph_advances[352] = {
    0x08, 0x0F, 0x10, 0x0F, 0x0F, 0x10, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x06, 0x0A, 0x06, 0x0B, 0x0F, 0x0C, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0D, 0x0F, 0x0F, 0x06, 0x06, 0x0F, 0x0F, 0x0F, 0x10, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0C, 0x0C, 0x0F, 0x0F, 0x07, 0x0B, 0x10, 0x0C, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0E, 0x0F, 0x0E, 0x0E, 0x10, 0x0F, 0x10, 0x0E, 0x0D, 0x09, 0x0D, 0x09, 0x10, 0x0C, 0x0F, 0x0E, 0x0E, 0x0F, 0x0E, 0x0F, 0x0A, 0x0F, 0x0F, 0x07, 0x09, 0x0F, 0x07, 0x10, 0x0E, 0x0E, 0x0E, 0x0E, 0x0B, 0x0E, 0x0B, 0x0F, 0x10, 0x10, 0x0E, 0x0E, 0x0C, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x05, 0x02, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x05, 0x07, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x05, 0x07, 0x06, 0x06, 0x05, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x06, 0x04, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x06, 0x05, 0x06, 0x05, 0x06, 0x06, 0x06, 0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x34, 0x8F, 0x0D, 0x80, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01, 0x03, 0x70, 0xDA, 0x0D, 0x80, 0x04, 0xA5, 0x0D, 0x80, 0x20, 0xA5, 0x0D, 0x80, 0x94, 0xA6, 0x0D, 0x80, 0xB8, 0xA6, 0x0D, 0x80, 0xD4, 0xA9, 0x0D, 0x80, 0xDC, 0xA9, 0x0D, 0x80, 0x98, 0xAC, 0x0D, 0x80, 0xD0, 0xAC, 0x0D, 0x80, 0x44, 0xAE, 0x0D, 0x80, 0x24, 0xB0, 0x0D, 0x80, 0x54, 0xB2, 0x0D, 0x80, 0x74, 0xB3, 0x0D, 0x80, 0x34, 0xB8, 0x0D, 0x80, 0x10, 0xB9, 0x0D, 0x80, 0x28, 0xB9, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x2C, 0xC4, 0x0D, 0x80, 0x34, 0xC4, 0x0D, 0x80,
};

typedef char account_glyph_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

typedef char title_text_packet_seed_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, tag) == 0 && offsetof(POLY_FT4, rgbc) + offsetof(RGBC, code) == 7 ? 1 : -1];

typedef struct
{
    uint32 begin, cur;
    uint8 *data;
} TEXT_PACKET_BUFFER;

typedef char text_packet_buffer_layout_check[sizeof(SPRT) == 20 && sizeof(POLY_FT4) == 40 && sizeof(DR_MODE) == 12 ? 1 : -1];

typedef struct
{
    POLY_FT4 quad[2][300];
    uint32 quad_padding;
    SPRT spr[2][300];
    uint32 spr_padding;
    DR_MODE mode[2][300];
    uint8 reserved[76];
} TEXT_GPU_STORAGE;

typedef char TEXT_GPU_STORAGE_LAYOUT_CHECK[sizeof(TEXT_GPU_STORAGE) == 43284 && offsetof(TEXT_GPU_STORAGE, quad) == 0 && offsetof(TEXT_GPU_STORAGE, spr) == 24004 && offsetof(TEXT_GPU_STORAGE, mode) == 36008 ? 1 : -1];

typedef struct
{
    uint32 packet_link;
    TEXT_GPU_STORAGE *packet_data;
    uint32 *ot_data[2];
    TEXT_PACKET_BUFFER quad, spr, mode;
    uint32 ot[2], ot_cur, ot_default;
    uint8 packet_banks_bound;
    RGBC color;
    XY pos, line_origin;
    uint32 char_count;
    uint16 tpage, clut;
    sint16 columns, line_scale;
    uint16 tex_x, tex_y, u, v, pal_x, pal_y;
    uint8 glyph_w, glyph_h, first_char, proportional;
    const uint8 *width_table;
    uint8 sem_trans, blend_mode, space_w, reveal_on;
    sint16 reveal_start, reveal_end, reveal_tick;
} TEXT_RENDER_STATE;

enum
{
    font_layout_4_widths = 0,
    font_layout_3_widths = 96,
    font_layout_8_widths = 192,
    font_layout_9_widths = 204,
    font_layout_5_6_widths = 216,
};

/* byte glyph indices can read beyond the nominal width rows */
static const uint8 font_width_data[] = {
    0x09, 0x0F, 0x10, 0x0F, 0x0F, 0x10, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x06, 0x09, 0x06, 0x0B, 0x0E, 0x0A, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0D, 0x0F, 0x0F, 0x06, 0x06, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0D, 0x0F, 0x0D, 0x0B, 0x0A, 0x0F, 0x0F, 0x07, 0x0A, 0x10, 0x0C, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0D, 0x0F, 0x0E, 0x0E, 0x10, 0x0F, 0x10, 0x0E, 0x0D, 0x09, 0x0B, 0x09, 0x0E, 0x0C, 0x0F, 0x0E, 0x0E, 0x0E, 0x0D, 0x0F, 0x0A, 0x0E, 0x0E, 0x07, 0x09, 0x0E, 0x07, 0x10, 0x0E, 0x0E, 0x0E, 0x0E, 0x0B, 0x0E, 0x0B, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0B, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x05, 0x02, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x05, 0x07, 0x06, 0x06, 0x05, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x06, 0x04, 0x07,
    0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x05, 0x05, 0x04, 0x05, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x06, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x04, 0x06, 0x05, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x06, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05, 0x03, 0x03, 0x06, 0x04, 0x07, 0x05, 0x06, 0x05, 0x06, 0x05, 0x05, 0x05, 0x06, 0x06, 0x09, 0x06, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x75, 0x08, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0xDC, 0x14, 0x00, 0x00, 0xC0, 0x17, 0x00, 0x00, 0x54, 0x18, 0x00, 0x00, 0x40, 0x1A, 0x00, 0x00, 0x80, 0x1B, 0x00, 0x00, 0x4C, 0x21, 0x00, 0x00, 0x58, 0x25, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x3B, 0x00, 0xBE, 0x01, 0x00, 0x00, 0x03, 0x00, 0x0A, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x11, 0x00, 0xB8, 0x01, 0x80, 0x80, 0x80, 0xFF, 0x90, 0x01, 0x38, 0x00, 0x4A, 0x03, 0x63, 0x01, 0x15, 0x00, 0x0C, 0x00, 0x54, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1C, 0x01, 0x5C, 0x01, 0x00, 0x00, 0x03, 0x00, 0x0A, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x01, 0x54, 0x01, 0x80, 0x80, 0x80, 0x02, 0x90, 0x01, 0x37, 0x00, 0x6F, 0x03, 0x63, 0x01, 0x04, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x01, 0x54, 0x01, 0x80, 0x80, 0x80, 0x02, 0x90, 0x01, 0x37, 0x00, 0x5F, 0x03, 0x63, 0x01, 0x04, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x01, 0x54, 0x01,
};

TITLE_TEXT_STATE title_text;

typedef struct
{
    TEXT_GPU_STORAGE packets;
    uint32 ordering[2];
} TEXT_RENDER_GPU_STORAGE;

typedef struct
{
    POLY_FT4 packets[2][TITLE_TEXT_PACKET_COUNT];
    uint32 ordering[2];
} TITLE_TEXT_GPU_STORAGE;

static TEXT_RENDER_GPU_STORAGE renderer_packets;
static TITLE_TEXT_GPU_STORAGE title_packets;

static TEXT_RENDER_STATE renderer;

typedef struct
{
    uint32 xy;
    uint32 uv_clut;
} DEBUG_TEXT_GLYPH;

typedef struct
{
    uint32 count;
    DEBUG_TEXT_GLYPH glyph[450];
    uint32 palette[3];
    uint32 palette_index;
    uint32 cursor;
} DEBUG_TEXT_STATE;

static DEBUG_TEXT_STATE debug_text_state;

typedef struct
{
    SPRT glyph[450];
    DR_TPAGE mode;
} DEBUG_TEXT_GPU_BANK;

typedef char debug_text_gpu_bank_layout_check[sizeof(SPRT) == 20 && sizeof(DR_TPAGE) == 8 && offsetof(DEBUG_TEXT_GPU_BANK, mode) == 9000 && sizeof(DEBUG_TEXT_GPU_BANK) == 9008 ? 1 : -1];

static DEBUG_TEXT_GPU_BANK debug_text_packets[2];

static void *text_packet_span(uint32 address, size_t bytes)
{
    uint32 index;
    uint32 base = renderer.packet_link;

    if (renderer.packet_data && bytes && address >= base && address - base <= sizeof(*renderer.packet_data) && bytes <= sizeof(*renderer.packet_data) - (address - base))
        return (uint8 *)renderer.packet_data + (address - base);
    for (index = 0; index < 2; ++index)
    {
        base = title_text.packet_pages[index];
        if (title_text.packet_data[index] && bytes && address >= base && address - base <= TITLE_TEXT_PACKET_COUNT * sizeof(POLY_FT4) && bytes <= TITLE_TEXT_PACKET_COUNT * sizeof(POLY_FT4) - (address - base))
            return (uint8 *)title_text.packet_data[index] + (address - base);
    }
    return ui_packet_span(address, bytes);
}

static uint32 *text_ot_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        if (renderer.ot_data[index] && address == renderer.ot[index] && bytes && bytes <= sizeof(uint32))
            return renderer.ot_data[index];
        if (title_text.ot_data[index] && address == title_text.ordering_pages[index] && bytes && bytes <= sizeof(uint32))
            return title_text.ot_data[index];
    }
    return ui_scene_ot_span(address, bytes);
}

void debug_text_reset_state(void)
{
    memset(&debug_text_state, 0, sizeof(debug_text_state));
}

void text_renderer_reset(void)
{
    renderer.packet_link = 0;
    renderer.packet_data = NULL;
    renderer.ot_data[0] = NULL;
    renderer.ot_data[1] = NULL;
    renderer.quad.begin = 0;
    renderer.spr.begin = 0;
    renderer.mode.begin = 0;
    renderer.packet_banks_bound = 0;
    renderer.quad.data = NULL;
    renderer.spr.data = NULL;
    renderer.mode.data = NULL;
}

static sint32 text_glyph_divide(uint32 numerator, sint32 denominator);
static void text_bind_frame(uint32 ordering);
static void text_emit_spr(uint32 character);
static void text_emit_quad(uint32 character);
static uint32 text_draw_spr_string(const char *text);
static uint32 text_draw_chars(uint32 x, uint32 y, const uint8 *native_text, uint32 native_capacity);
static uint32 text_format_draw(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);
static void text_format_digits_draw(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);

static sint32 text_glyph_divide(uint32 numerator, sint32 denominator)
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

void text_set_pos(uint32 x, uint32 y)
{
    uint32 state, value;
    FUNCTION_MARKER(0x800F33A8, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    value = (x & 0xFFFF) | (y << 16);
    renderer.pos.xy = value;
    renderer.line_origin.xy = value;
}

void text_set_color(uint32 red, uint32 green, uint32 blue)
{
    uint32 state;
    FUNCTION_MARKER(0x800F33D0, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    renderer.color.rgbc = red | (green << 8) | (blue << 16);
}

void text_set_blend(uint32 mode)
{
    uint32 state, y, x, texture, result;
    FUNCTION_MARKER(0x800F33F8, "ACE3.BIN");
    if ((sint16)mode == -1)
    {
        state = renderer.packet_link + 0x8000;
        renderer.sem_trans = (uint8)(0);
        state = renderer.packet_link + 0x8000;
        renderer.blend_mode = (uint8)(0);
    }
    else
    {
        state = renderer.packet_link + 0x8000;
        renderer.sem_trans = (uint8)(1);
        state = renderer.packet_link + 0x8000;
        renderer.blend_mode = (uint8)(mode);
    }
    state = renderer.packet_link + 0x8000;
    texture = renderer.blend_mode;
    y = renderer.tex_y;
    x = renderer.tex_x;
    result = (y & 0x100) >> 4;
    renderer.tpage = (uint16)(((texture & 3) << 5) | result | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2));
    {
        (void)(uint32)(result);
        return;
    }
}

void text_set_reveal_step(uint32 enabled)
{
    uint32 state;
    FUNCTION_MARKER(0x800F34B0, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    renderer.reveal_tick = (sint16)(uint16)(enabled);
    {
        (void)(uint32)(state);
        return;
    }
}

void text_set_reveal_range(uint32 enabled, uint32 first, uint32 second)
{
    uint32 state;
    FUNCTION_MARKER(0x800F34C8, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    renderer.reveal_on = (uint8)(enabled);
    state = renderer.packet_link + 0x8000;
    renderer.reveal_start = (sint16)(uint16)(first);
    renderer.reveal_end = (sint16)(uint16)(second);
    {
        (void)(uint32)(state);
        return;
    }
}

void text_select_font(uint32 layout)
{
    uint32 state, owner, texture, y, x, palette_y, palette_x, mode;
    sint32 signed_x, signed_y, remainder;
    FUNCTION_MARKER(0x800F35B8, "ACE3.BIN");
    switch (layout)
    {
        case 1:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(16);
            renderer.line_scale = (sint16)(uint16)(2);
            renderer.tex_x = (uint16)(320);
            renderer.tex_y = (uint16)(320);
            renderer.pal_x = (uint16)(320);
            renderer.first_char = (uint8)(32);
            owner = renderer.packet_link + 0x8000;
            renderer.pal_y = (uint16)(344);
            renderer.proportional = (uint8)(0);
            goto clear_modes;
        case 2:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.line_scale = (sint16)(uint16)(2);
            renderer.tex_x = (uint16)(392);
            renderer.tex_y = (uint16)(280);
            renderer.pal_x = (uint16)(384);
            renderer.first_char = (uint8)(32);
            owner = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(32);
            renderer.pal_y = (uint16)(510);
            renderer.proportional = (uint8)(0);
            goto clear_modes;
        case 3:
        case 4:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(16);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(21);
            state = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(16);
            renderer.tex_x = (uint16)(768);
            renderer.tex_y = (uint16)(layout == 3 ? 382 : 256);
            renderer.pal_x = (uint16)(400);
            renderer.first_char = (uint8)(33);
            owner = renderer.packet_link + 0x8000;
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.pal_y = (uint16)(45);
            renderer.proportional = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(4);
            owner = renderer.packet_link + 0x8000;
            renderer.width_table = font_width_data + (layout == 3 ? font_layout_3_widths : font_layout_4_widths);
            renderer.sem_trans = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.blend_mode = (uint8)(2);
            break;
        case 5:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(12);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(16);
            state = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(21);
            renderer.tex_x = (uint16)(832);
            renderer.pal_x = (uint16)(416);
            renderer.first_char = (uint8)(33);
            owner = renderer.packet_link + 0x8000;
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.tex_y = (uint16)(0);
            renderer.pal_y = (uint16)(19);
            renderer.proportional = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(4);
            owner = renderer.packet_link + 0x8000;
            renderer.width_table = font_width_data + font_layout_5_6_widths;
            renderer.sem_trans = (uint8)(0);
            goto clear_mode_index;
        case 6:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(12);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(16);
            state = renderer.packet_link + 0x8000;
            renderer.tex_x = (uint16)(832);
            renderer.first_char = (uint8)(33);
            owner = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(21);
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.tex_y = (uint16)(0);
            renderer.pal_x = (uint16)(416);
            renderer.pal_y = (uint16)(21);
            renderer.proportional = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(4);
            owner = renderer.packet_link + 0x8000;
            renderer.width_table = font_width_data + font_layout_5_6_widths;
            renderer.sem_trans = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.blend_mode = (uint8)(2);
            break;
        case 7:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(10);
            state = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(25);
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.tex_x = (uint16)(384);
            renderer.tex_y = (uint16)(384);
            renderer.pal_x = (uint16)(400);
            renderer.first_char = (uint8)(48);
            owner = renderer.packet_link + 0x8000;
            renderer.pal_y = (uint16)(511);
            renderer.proportional = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(6);
        clear_modes:
            state = renderer.packet_link + 0x8000;
            renderer.sem_trans = (uint8)(0);
        clear_mode_index:
            state = renderer.packet_link + 0x8000;
            renderer.blend_mode = (uint8)(0);
            break;
        case 8:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(8);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(29);
            state = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(12);
            renderer.tex_x = (uint16)(918);
            renderer.tex_y = (uint16)(384);
            renderer.pal_x = (uint16)(432);
            renderer.first_char = (uint8)(48);
            owner = renderer.packet_link + 0x8000;
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.pal_y = (uint16)(47);
            renderer.proportional = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.sem_trans = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.blend_mode = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.proportional = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.width_table = font_width_data + font_layout_8_widths;
            break;
        case 9:
            state = renderer.packet_link + 0x8000;
            renderer.glyph_w = (uint8)(8);
            state = renderer.packet_link + 0x8000;
            renderer.glyph_h = (uint8)(29);
            state = renderer.packet_link + 0x8000;
            renderer.first_char = (uint8)(48);
            owner = renderer.packet_link + 0x8000;
            renderer.columns = (sint16)(uint16)(12);
            renderer.tex_x = (uint16)(896);
            renderer.tex_y = (uint16)(384);
            renderer.line_scale = (sint16)(uint16)(1);
            renderer.pal_x = (uint16)(432);
            renderer.pal_y = (uint16)(48);
            renderer.proportional = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.space_w = (uint8)(6);
            state = renderer.packet_link + 0x8000;
            renderer.sem_trans = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.blend_mode = (uint8)(0);
            state = renderer.packet_link + 0x8000;
            renderer.proportional = (uint8)(1);
            state = renderer.packet_link + 0x8000;
            renderer.width_table = font_width_data + font_layout_9_widths;
            break;
        default:
            break;
    }
    state = renderer.packet_link + 0x8000;
    signed_x = (sint16)renderer.tex_x;
    mode = renderer.blend_mode;
    y = renderer.tex_y;
    x = renderer.tex_x;
    texture = ((mode & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    palette_y = renderer.pal_y;
    palette_x = renderer.pal_x;
    renderer.tpage = (uint16)(texture);
    renderer.clut = (uint16)((palette_y << 6) | ((palette_x >> 4) & 0x3F));
    remainder = signed_x % 64;
    signed_y = (sint16)renderer.tex_y;
    renderer.u = (uint16)((uint32)(remainder * 4));
    remainder = signed_y % 256;
    renderer.v = (uint16)((uint32)remainder);
    {
        (void)(uint32)((uint32)remainder);
        return;
    }
}

void text_renderer_init(void)
{
    uint32 state, owner, packet, value, group, i;
    SPRT *spr, *sprites;
    FUNCTION_MARKER(0x800F3C14, "ACE3.BIN");
    renderer.packet_banks_bound = 0;
    renderer.quad.data = NULL;
    renderer.spr.data = NULL;
    renderer.mode.data = NULL;
    (void)arena_alloc(43284);
    if (!gpu_register_packet_range(&renderer_packets, sizeof(renderer_packets)))
        abort();
    renderer.packet_link = gpu_packet_link((uintptr_t)&renderer_packets.packets);
    renderer.packet_data = &renderer_packets.packets;
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(4);
        owner = renderer.packet_link;
        state = owner + 0x8000;
        renderer.ot[group] = gpu_packet_link((uintptr_t)&renderer_packets.ordering[group]);
        renderer.ot_data[group] = &renderer_packets.ordering[group];
        renderer.sem_trans = (uint8)(0);
        value = renderer.packet_link;
        renderer.spr.cur = owner + 24004 + group * 6000;
        renderer.blend_mode = (uint8)(0);
        text_select_font(4);
        sprites = renderer.packet_data->spr[group];
        for (i = 0; i < 300; ++i)
        {
            state = renderer.packet_link + 0x8000;
            packet = renderer.spr.cur;
            spr = sprites + i;
            setSprt(spr);
            state = renderer.packet_link + 0x8000;
            packet = renderer.spr.cur;
            spr->rgbc.r = 128;
            state = renderer.packet_link + 0x8000;
            packet = renderer.spr.cur;
            spr->rgbc.g = 128;
            state = renderer.packet_link + 0x8000;
            packet = renderer.spr.cur;
            spr->rgbc.b = 128;
            state = renderer.packet_link + 0x8000;
            packet = renderer.spr.cur;
            renderer.spr.cur = packet + 20;
        }
    }
    state = renderer.packet_link + 0x8000;
    renderer.char_count = 0;
    renderer.reveal_on = (uint8)(0);
    text_set_pos(10, 200);
    text_set_color(128, 128, 128);
}

void text_begin_frame(uint32 ordering)
{
    FUNCTION_MARKER(0x800F3DB4, "ACE3.BIN");
    {
        (void)(text_bind_frame(ordering));
        return;
    }
}

static void text_bind_frame(uint32 ordering)
{
    uint32 state, bank, value, owner, packet, link, tag;
    uint32 *text_ot, *ot_entry;
    FUNCTION_MARKER(0x800F3DBC, "ACE3.BIN");
    renderer.packet_banks_bound = 0;
    bank = game_runtime.display_buffer;
    if (bank >= 2)
        abort();
    state = renderer.packet_link;
    renderer.quad.cur = state + bank * 12000;
    renderer.quad.begin = state + bank * 12000;
    bank = game_runtime.display_buffer;
    if (bank >= 2)
        abort();
    owner = state + 0x8000;
    renderer.spr.cur = state + bank * 6000 + 24004;
    renderer.spr.begin = state + bank * 6000 + 24004;
    bank = game_runtime.display_buffer;
    if (bank >= 2)
        abort();
    renderer.mode.cur = state + bank * 3600 + 36008;
    renderer.mode.begin = state + bank * 3600 + 36008;
    bank = game_runtime.display_buffer;
    if (bank >= 2)
        abort();
    value = renderer.ot[bank];
    renderer.ot_cur = value;
    renderer.ot_default = value;
    ClearOTagR(text_ot_span(value, 4), 1);
    owner = renderer.packet_link + 0x8000;
    packet = renderer.ot_cur;
    text_ot = text_ot_span(packet, sizeof(*text_ot));
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    link = *ot_entry;
    tag = *text_ot;
    *text_ot = (tag & 0xFF000000) | (link & 0xFFFFFF);
    tag = *ot_entry;
    value = renderer.ot_cur & 0xFFFFFF;
    *ot_entry = (tag & 0xFF000000) | value;
    renderer.quad.data = text_packet_span(renderer.quad.begin, 12000);
    renderer.spr.data = text_packet_span(renderer.spr.begin, 6000);
    renderer.mode.data = text_packet_span(renderer.mode.begin, 3600);
    renderer.packet_banks_bound = 1;
    {
        (void)(uint32)(value);
        return;
    }
}

void text_set_ot(uint32 ordering)
{
    uint32 owner, value;

    FUNCTION_MARKER(0x800F3EF4, "ACE3.BIN");
    owner = renderer.packet_link + 0x8000;
    if (ordering == 0xFFFFFFFF)
    {
        value = renderer.ot_default;
        renderer.ot_cur = value;
    }
    else
        renderer.ot_cur = ordering;
    {
        (void)(uint32)(owner);
        return;
    }
}

static void text_emit_spr(uint32 character)
{
    uint32 state, glyph, divisor, quotient, remainder, width, height, u, v, packet, value, tag, ordering, x, y;
    SPRT *prim;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x800F3F2C, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    glyph = (character - renderer.first_char) & 255;
    divisor = (uint32)renderer.columns;
    if (divisor == 0)
        xport_mips_break(7);
    quotient = ((sint32)glyph / (sint32)divisor);
    remainder = ((sint32)glyph % (sint32)divisor);
    width = renderer.glyph_w;
    height = renderer.glyph_h;
    u = renderer.u;
    v = renderer.v;
    packet = renderer.spr.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.spr.begin > 6000 - sizeof(*prim))
        abort();
    prim = (SPRT *)(renderer.spr.data + (packet - renderer.spr.begin));
    prim->p[0].uv.u = (uint8)(u + remainder * width);
    prim->p[0].uv.v = (uint8)(v + quotient * height);
    state = renderer.packet_link + 0x8000;
    height = renderer.glyph_h;
    width = renderer.glyph_w;
    setWH(prim, width, height);
    y = (uint32)renderer.pos.y;
    x = (uint16)renderer.pos.x;
    prim->p[0].xy.xy = x | (y << 16);
    prim->p[0].attr = (uint16)(renderer.clut);
    prim->rgbc.r = (uint8)(renderer.color.r);
    state = renderer.packet_link + 0x8000;
    prim->rgbc.g = (uint8)(renderer.color.g);
    state = renderer.packet_link + 0x8000;
    prim->rgbc.b = (uint8)(renderer.color.b);
    state = renderer.packet_link + 0x8000;
    value = renderer.sem_trans;
    tag = prim->rgbc.code;
    prim->rgbc.code = (uint8)(value ? tag | 2 : tag & 0xFD);
    state = renderer.packet_link + 0x8000;
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    value = *ot_entry;
    prim->tag = value | 0x04000000;
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    *ot_entry = packet & 0xFFFFFF;
    state = renderer.packet_link + 0x8000;
    if (renderer.proportional)
    {
        value = renderer.width_table[glyph];
        x = (uint16)renderer.pos.x;
        renderer.pos.x = (sint16)(uint16)(x + value);
    }
    else
    {
        value = renderer.glyph_w;
        x = (uint16)renderer.pos.x;
        renderer.pos.x = (sint16)(uint16)(x + value + 1);
    }
    state = renderer.packet_link + 0x8000;
    value = renderer.spr.cur + 20;
    renderer.spr.cur = value;
    {
        (void)(uint32)(value);
        return;
    }
}

static void text_emit_quad(uint32 character)
{
    uint32 base, state, code, index, packet, width, height, raw_u, raw_v;
    uint32 palette, page, right, bottom, red, green, blue, flag, product;
    uint32 numerator, fraction, other_fraction, packed, x, y, ordering, value;
    sint32 columns, quotient, u, v, scale, current, start, end, denominator, half;
    POLY_FT4 *prim;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x800F4124, "ACE3.BIN");
    base = renderer.packet_link;
    state = base + 0x8000;
    code = renderer.first_char;
    columns = renderer.columns;
    index = (character - code) & 0xFF;
    quotient = text_glyph_divide(index, columns);
    raw_u = index - (uint32)quotient * (uint32)columns;
    width = renderer.glyph_w;
    raw_u *= width;
    height = renderer.glyph_h;
    product = (uint32)quotient * height;
    value = renderer.u;
    packet = renderer.quad.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.quad.begin > 12000 - sizeof(*prim))
        abort();
    prim = (POLY_FT4 *)(renderer.quad.data + (packet - renderer.quad.begin));
    raw_v = renderer.v;
    u = (sint16)(value + raw_u);
    palette = (sint16)renderer.clut;
    right = (uint32)u - 1;
    v = (sint16)(raw_v + product);
    packed = (uint32)v << 8;
    prim->p[0].uv_attr = (uint32)u | packed | (palette << 16);
    width = renderer.glyph_w;
    page = (sint16)renderer.tpage;
    prim->p[1].uv_attr = (right + width) | packed | (page << 16);
    height = renderer.glyph_h;
    bottom = (uint32)v - 1;
    prim->p[2].uv_attr = (uint32)u | ((bottom + height) << 8);
    width = renderer.glyph_w;
    height = renderer.glyph_h;
    prim->p[3].uv_attr = (right + width) | ((bottom + height) << 8);
    green = renderer.color.g;
    red = renderer.color.r;
    blue = renderer.color.b;
    prim->rgbc.rgbc = red | (green << 8) | (blue << 16);
    flag = renderer.reveal_on;
    if (flag == 1)
    {
        current = renderer.reveal_tick;
        start = renderer.reveal_start;
        if (current < start)
            return;
        end = renderer.reveal_end;
        if (current < end)
        {
            height = renderer.glyph_h;
            scale = renderer.line_scale;
            product = height * (uint32)scale;
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            denominator = end - start;
            fraction = text_glyph_divide(numerator, denominator);
            y = (uint32)renderer.pos.y + fraction;
            x = (uint16)renderer.pos.x;
            prim->p[0].xy.xy = (y << 16) | x;
            height = renderer.glyph_h;
            scale = renderer.line_scale;
            product = height * (uint32)scale;
            current = renderer.reveal_tick;
            half = (sint32)product / 2;
            end = renderer.reveal_end;
            numerator = (uint32)half * (uint32)(end - current);
            start = renderer.reveal_start;
            denominator = end - start;
            fraction = text_glyph_divide(numerator, denominator);
            x = (uint32)renderer.pos.x - 1;
            y = (uint32)renderer.pos.y + fraction;
            width = renderer.glyph_w;
            prim->p[1].xy.xy = (y << 16) | ((x + width) & 0xFFFF);
            height = renderer.glyph_h;
            scale = renderer.line_scale;
            product = height * (uint32)scale;
            current = renderer.reveal_tick;
            half = (sint32)product / 2;
            end = renderer.reveal_end;
            numerator = (uint32)half * (uint32)(end - current);
            start = renderer.reveal_start;
            denominator = end - start;
            fraction = text_glyph_divide(numerator, denominator);
            numerator = (height - 1) * (uint32)scale * (uint32)(current - start);
            other_fraction = text_glyph_divide(numerator, denominator);
            y = (uint32)renderer.pos.y + fraction + other_fraction;
            x = (uint16)renderer.pos.x;
            prim->p[2].xy.xy = (y << 16) | x;
            height = renderer.glyph_h;
            scale = renderer.line_scale;
            product = height * (uint32)scale;
            end = renderer.reveal_end;
            current = renderer.reveal_tick;
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            start = renderer.reveal_start;
            denominator = end - start;
            fraction = text_glyph_divide(numerator, denominator);
            numerator = (height - 1) * (uint32)scale * (uint32)(current - start);
            other_fraction = text_glyph_divide(numerator, denominator);
            x = (uint32)renderer.pos.x - 1;
            y = (uint32)renderer.pos.y + fraction + other_fraction;
            width = renderer.glyph_w;
            packed = (y << 16) | ((x + width) & 0xFFFF);
            goto write_last_vertex;
        }
    }
    y = (uint32)renderer.pos.y;
    x = (uint16)renderer.pos.x;
    prim->p[0].xy.xy = (y << 16) | x;
    y = (uint32)renderer.pos.y;
    x = (uint32)renderer.pos.x;
    width = renderer.glyph_w;
    prim->p[1].xy.xy = (y << 16) | ((x - 1 + width) & 0xFFFF);
    height = renderer.glyph_h;
    scale = renderer.line_scale;
    product = (height - 1) * (uint32)scale;
    y = (uint32)renderer.pos.y;
    x = (uint16)renderer.pos.x;
    prim->p[2].xy.xy = ((y + product) << 16) | x;
    height = renderer.glyph_h;
    scale = renderer.line_scale;
    product = (height - 1) * (uint32)scale;
    width = renderer.glyph_w;
    x = (uint32)renderer.pos.x;
    y = (uint32)renderer.pos.y;
    packed = ((y + product) << 16) | ((x - 1 + width) & 0xFFFF);
write_last_vertex:
    prim->p[3].xy.xy = packed;
    setlen(prim, 9);
    prim->rgbc.code = (uint8)(44);
    value = renderer.packet_link + 0x8000;
    flag = renderer.sem_trans;
    prim->rgbc.code = (uint8)(flag ? 46 : 44);
    value = renderer.packet_link + 0x8000;
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    packed = *ot_entry;
    prim->tag = packed | 0x09000000;
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    *ot_entry = packet & 0xFFFFFF;
    value = renderer.packet_link + 0x8000;
    flag = renderer.proportional;
    if (flag)
    {
        width = renderer.width_table[index];
        x = (uint16)renderer.pos.x;
        renderer.pos.x = (sint16)(uint16)(x + width);
    }
    else
    {
        width = renderer.glyph_w;
        x = (uint16)renderer.pos.x;
        renderer.pos.x = (sint16)(uint16)(x + width + 1);
    }
    base = renderer.packet_link;
    packet = renderer.quad.cur + 40;
    renderer.quad.cur = packet;
    {
        (void)(uint32)(packet);
        return;
    }
}

static uint32 text_draw_spr_string(const char *text)
{
    uint32 state, packet, ordering, tag, link, value, index = 0, character, step, reset, current;
    sint32 count, scale;
    DR_MODE *draw;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x800F46E4, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    packet = renderer.mode.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.mode.begin > 3600 - sizeof(*draw))
        abort();
    draw = (DR_MODE *)(renderer.mode.data + (packet - renderer.mode.begin));
    SetDrawMode(draw, 0, 0, 0, NULL);
    state = renderer.packet_link + 0x8000;
    packet = renderer.mode.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.mode.begin > 3600 - sizeof(*draw))
        abort();
    draw = (DR_MODE *)(renderer.mode.data + (packet - renderer.mode.begin));
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    tag = draw->tag;
    link = *ot_entry;
    draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    packet = renderer.mode.cur;
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    packet = renderer.mode.cur;
    count = (sint32)renderer.char_count;
    renderer.mode.cur = packet + 12;
    if (count > 0)
        do
        {
            character = (uint8)text[index];
            if (character == 10)
            {
                state = renderer.packet_link + 0x8000;
                step = renderer.glyph_h;
                scale = renderer.line_scale;
                reset = (uint16)renderer.line_origin.x;
                current = (uint16)renderer.pos.y;
                renderer.pos.x = (sint16)(uint16)(reset);
                renderer.pos.y = (sint16)(uint16)(current + step * (uint32)scale);
            }
            else if (character == 9)
            {
                state = renderer.packet_link + 0x8000;
                renderer.pos.x = (sint16)(uint16)((uint16)renderer.line_origin.x);
            }
            else if (character == 32)
            {
                state = renderer.packet_link + 0x8000;
                if (renderer.proportional)
                {
                    step = renderer.space_w;
                    current = (uint16)renderer.pos.x;
                    renderer.pos.x = (sint16)(uint16)(current + step);
                }
                else
                {
                    step = renderer.glyph_w;
                    current = (uint16)renderer.pos.x;
                    renderer.pos.x = (sint16)(uint16)(current + step + 1);
                }
            }
            else
                text_emit_spr((uint8)text[index]);
            state = renderer.packet_link + 0x8000;
            count = (sint32)renderer.char_count;
            ++index;
        } while ((sint32)index < count);
    state = renderer.packet_link + 0x8000;
    value = (sint16)renderer.tpage;
    packet = renderer.mode.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.mode.begin > 3600 - sizeof(*draw))
        abort();
    draw = (DR_MODE *)(renderer.mode.data + (packet - renderer.mode.begin));
    SetDrawMode(draw, 0, 0, value, NULL);
    state = renderer.packet_link + 0x8000;
    packet = renderer.mode.cur;
    if (!renderer.packet_banks_bound || (packet & 3) || packet - renderer.mode.begin > 3600 - sizeof(*draw))
        abort();
    draw = (DR_MODE *)(renderer.mode.data + (packet - renderer.mode.begin));
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    tag = draw->tag;
    link = *ot_entry;
    draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = renderer.ot_cur;
    ot_entry = text_ot_span(ordering, sizeof(*ot_entry));
    packet = renderer.mode.cur;
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    packet = renderer.mode.cur + 12;
    renderer.mode.cur = packet;
    return packet;
}

static uint32 text_draw_chars(uint32 x, uint32 y, const uint8 *native_text, uint32 native_capacity)
{
    uint32 state, index, character, step, reset, current, value;
    sint32 count, scale;
    FUNCTION_MARKER(0x800F4928, "ACE3.BIN");
    state = renderer.packet_link + 0x8000;
    count = (sint32)renderer.char_count;
    if (count <= 0)
        return (uint32)count;
    index = 0;
    do
    {
        if (index >= native_capacity)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource2 text length exceeds native buffer\n");
#endif
            abort();
        }
        character = native_text[index];
        if (character == 10)
        {
            state = renderer.packet_link + 0x8000;
            step = renderer.glyph_h;
            scale = renderer.line_scale;
            reset = (uint16)renderer.line_origin.x;
            current = (uint16)renderer.pos.y;
            renderer.pos.x = (sint16)(uint16)(reset);
            renderer.pos.y = (sint16)(uint16)(current + (uint32)((sint32)step * scale));
        }
        else if (character == 9)
        {
            state = renderer.packet_link + 0x8000;
            reset = (uint16)renderer.line_origin.x;
            renderer.pos.x = (sint16)(uint16)(reset);
        }
        else if (character == 32)
        {
            state = renderer.packet_link + 0x8000;
            value = renderer.proportional;
            if (value)
            {
                step = renderer.space_w;
                current = (uint16)renderer.pos.x;
                renderer.pos.x = (sint16)(uint16)(current + step);
            }
            else
            {
                step = renderer.glyph_w;
                current = (uint16)renderer.pos.x;
                renderer.pos.x = (sint16)(uint16)(current + step + 1);
            }
        }
        else
        {
            character = native_text[index];
            text_emit_quad(character);
        }
        state = renderer.packet_link + 0x8000;
        count = (sint32)renderer.char_count;
        ++index;
    } while ((sint32)index < count);
    return 0;
}

uint32 text_print_spr(const char *text)
{
    uint32 owner, count, state;
    FUNCTION_MARKER(0x800F4A78, "ACE3.BIN");
    owner = renderer.packet_link + 0x8000;
    count = text ? (uint32)strlen(text) : 0;
    state = renderer.packet_link + 0x8000;
    renderer.char_count = count;
    (void)renderer.pos.x;
    (void)renderer.pos.y;
    return text_draw_spr_string(text);
}

uint32 text_print(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    FUNCTION_MARKER(0x800F4AE0, "ACE3.BIN");
    return text_format_draw(format, native_arguments, native_argument_bytes);
}

static uint32 text_format_draw(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length, state, x, y;
    FUNCTION_MARKER(0x800F4AF0, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, native_arguments, native_argument_bytes);
    state = renderer.packet_link + 0x8000;
    x = (uint32)renderer.pos.x;
    y = (uint32)renderer.pos.y;
    renderer.char_count = length;
    return text_draw_chars(x, y, buffer, sizeof(buffer));
}

void text_print_digits(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    FUNCTION_MARKER(0x800F4B40, "ACE3.BIN");
    {
        (void)(text_format_digits_draw(format, native_arguments, native_argument_bytes));
        return;
    }
}

static void text_format_digits_draw(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length, state, index, x, y;
    sint32 count;
    FUNCTION_MARKER(0x800F4B50, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, native_arguments, native_argument_bytes);
    state = renderer.packet_link + 0x8000;
    renderer.char_count = length;
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
            state = renderer.packet_link + 0x8000;
            count = (sint32)renderer.char_count;
            ++index;
        } while ((sint32)index < count);
    }
    state = renderer.packet_link + 0x8000;
    x = (uint32)renderer.pos.x;
    y = (uint32)renderer.pos.y;
    {
        (void)(uint32)(text_draw_chars(x, y, buffer, sizeof(buffer)));
        return;
    }
}

void debug_text_initialize(void)
{
    FUNCTION_MARKER(0x800275C8, "ACE3.BIN");
    debug_text_state.cursor = (uint32)(0x100010);
    debug_text_state.palette[0] = (uint32)(0x56140000);
    debug_text_state.palette[1] = (uint32)(0x56540000);
    debug_text_state.count = 0;
    debug_text_state.palette[2] = (uint32)(0x56940000);
    debug_text_state.palette_index = (uint32)(0);
    return;
}

uint32 debug_text_print(uint32 x, uint32 y, const uint8 *text)
{
    DEBUG_TEXT_GLYPH *output;
    uint32 cursor, ch, count, index, style, glyph, uv;
    FUNCTION_MARKER(0x80027608, "ACE3.BIN");
    cursor = 0;
    ch = text[cursor];
    ++cursor;
    count = debug_text_state.count;
    index = debug_text_state.palette_index;
    output = debug_text_state.glyph + count;
    style = debug_text_state.palette[index];
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
        output->xy = (x & 0xFFFF) | y;
        x += 6;
        uv = ((glyph & 15) * 6) | ((((uint32)((sint32)glyph >> 4) * 6) + 64) << 8);
        output->uv_clut = uv | style;
        ++output;
        ch = text[cursor];
        ++cursor;
    }
    debug_text_state.count = count;
    return x;
}

void debug_text_hex(uint32 x, uint32 y, uint32 value, uint32 width)
{
    static const uint8 digits[16] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70};
    uint8 buffer[10];
    uint32 i, cursor;
    FUNCTION_MARKER(0x800277F0, "ACE3.BIN");
    if ((sint32)width >= 10)
        return;
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
    {
        (void)(uint32)(debug_text_print(x, y, buffer + cursor));
        return;
    }
}

uint32 debug_text_decimal(uint32 x, uint32 y, uint32 value, uint32 format)
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
    return debug_text_print(x, y, buffer + cursor);
}

typedef char debug_text_packet_layout_check[sizeof(SPRT) == 20 && offsetof(SPRT, rgbc) == 4 && offsetof(SPRT, p) == 8 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && sizeof(DR_TPAGE) == 8 ? 1 : -1];

uint32 debug_text_draw(uint32 ot, uint32 packet)
{
    uint32 *gpu_word;
    DEBUG_TEXT_GPU_BANK *bank;
    uint32 bank_index = game_runtime.display_buffer;
    const DEBUG_TEXT_GLYPH *entry;
    uint32 i;
    uint32 first;
    uint32 second;
    uint32 tag;
    uint32 head;
    sint32 count;

    FUNCTION_MARKER(0x800278A8, "ACE3.BIN");
    if (bank_index >= 2 || !gpu_register_packet_range(debug_text_packets, sizeof(debug_text_packets)))
        abort();
    bank = &debug_text_packets[bank_index];
    gpu_word = text_ot_span(ot, sizeof(*gpu_word));
    count = (sint32)debug_text_state.count;
    entry = debug_text_state.glyph;
    for (i = 0; (sint32)i < count; ++i, packet += 20)
    {
        SPRT *sprite = &bank->glyph[i];
        sprite->rgbc.rgbc = 0x64808080;
        first = entry->xy;
        sprite->p[0].xy.xy = first;
        second = entry->uv_clut;
        ++entry;
        {
            uint32 dimensions = 0x00060006;
            memcpy((uint8 *)sprite + offsetof(SPRT, w), &dimensions, sizeof(dimensions));
        }
        sprite->p[0].uv_attr = second;
        head = *gpu_word;
        sprite->tag = head | 0x04000000;
        *gpu_word = gpu_packet_link((uintptr_t)sprite);
    }
    DR_TPAGE *mode = &bank->mode;
    mode->tag = (mode->tag & 0xFFFFFF) | 0x01000000;
    mode->code[0] = 0xE1000015;
    tag = mode->tag;
    head = *gpu_word;
    debug_text_state.cursor = (uint32)(0x00100010);
    mode->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    head = *gpu_word;
    *gpu_word = (head & 0xFF000000) | gpu_packet_link((uintptr_t)mode);
    debug_text_state.count = 0;
    return packet + 8;
}

void format_mission_time(uint8 text[MISSION_TIME_TEXT_SIZE], sint32 duration)
{
    uint32 frames, hours, minutes, seconds;

    if (duration > 107999999)
        duration = 107999999;
    if (duration < 0)
        duration = 0;
    frames = duration;
    hours = frames / 108000;
    minutes = frames / 1800 - 60 * hours;
    seconds = frames / 30 - 60 * (frames / 1800);
    text[0] = ('0' + hours / 100);
    text[1] = ('0' + hours / 10 % 10);
    text[2] = ('0' + hours % 10);
    text[3] = ':';
    text[4] = ('0' + minutes / 10);
    text[5] = ('0' + minutes % 10);
    text[6] = ':';
    text[7] = ('0' + seconds / 10);
    text[8] = ('0' + seconds % 10);
    text[9] = 0;
}

void title_text_set_pos(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800D7C40, "ACE3.BIN");
    title_text.x = (uint16)(first);
    title_text.line_x = (uint16)(first);
    title_text.y = (uint16)(second);
    return;
}

void title_text_set_color(uint32 red, uint32 green, uint32 blue)
{
    FUNCTION_MARKER(0x800D7C60, "ACE3.BIN");
    title_text.red = (uint8)(red);
    title_text.green = (uint8)(green);
    title_text.blue = (uint8)(blue);
    return;
}

void title_text_initialize(void)
{
    uint32 buffer, index, packet;

    FUNCTION_MARKER(0x800D80C0, "ACE3.BIN");
    arena_alloc(76);
    title_text.frame_packets = NULL;
    if (!gpu_register_packet_range(&title_packets, sizeof(title_packets)))
        abort();
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(TITLE_TEXT_PACKET_COUNT * sizeof(POLY_FT4));
        title_text.packet_pages[index] = gpu_packet_link((uintptr_t)&title_packets.packets[index][0]);
        (void)arena_alloc(4);
        buffer = title_text.packet_pages[index];
        title_text.ordering_pages[index] = gpu_packet_link((uintptr_t)&title_packets.ordering[index]);
        title_text.ot_data[index] = &title_packets.ordering[index];
        title_text.blend_enabled = (uint8)(0);
        title_text.packet_cursor = buffer;
        title_text.blend_mode = (uint8)(0);
        title_text_select_font(4);
        POLY_FT4 *quads = title_packets.packets[index];
        title_text.packet_data[index] = quads;
        for (packet = 0; packet < TITLE_TEXT_PACKET_COUNT; ++packet)
        {
            buffer = title_text.packet_cursor;
            ((uint8 *)&quads[packet].tag)[3] = 9;
            buffer = title_text.packet_cursor;
            quads[packet].rgbc.code = 44;
            buffer = title_text.packet_cursor;
            title_text.packet_cursor = buffer + 40;
        }
    }
    title_text.length = (uint32)(0);
    title_text.interpolate = (uint8)(0);
    title_text_set_pos(10, 200);
    {
        (void)(title_text_set_color(128, 128, 128));
        return;
    }
}

void title_text_select_font(uint32 mode)
{
    uint32 x, y, tail;

    FUNCTION_MARKER(0x800D7D08, "ACE3.BIN");
    tail = 0;
    switch (mode)
    {
        case 1:
            title_text.glyph_width = (uint8)(6);
            title_text.glyph_height = (uint8)(6);
            title_text.columns = (uint16)(16);
            title_text.vertical_scale = (uint16)(2);
            title_text.texture_y = (uint16)(320);
            title_text.texture_x = (uint16)(320);
            title_text.palette_x = (uint16)(320);
            title_text.palette_y = (uint16)(344);
            title_text.first_character = (uint8)(32);
            break;
        case 2:
            title_text.glyph_width = (uint8)(6);
            title_text.glyph_height = (uint8)(6);
            title_text.vertical_scale = (uint16)(2);
            title_text.texture_x = (uint16)(392);
            title_text.texture_y = (uint16)(281);
            title_text.columns = (uint16)(32);
            title_text.first_character = (uint8)(32);
            title_text.palette_x = (uint16)(384);
            title_text.palette_y = (uint16)(510);
            break;
        case 3:
        case 4:
            tail = mode;
            title_text.glyph_width = (uint8)(16);
            title_text.glyph_height = (uint8)(21);
            title_text.columns = (uint16)(16);
            title_text.texture_x = (uint16)(768);
            title_text.texture_y = (uint16)(tail == 3 ? 382 : 256);
            title_text.palette_x = (uint16)(400);
            title_text.palette_y = (uint16)(45);
            title_text.vertical_scale = (uint16)(1);
            title_text.first_character = (uint8)(33);
            break;
        case 7:
            tail = 7;
            title_text.glyph_width = (uint8)(6);
            title_text.glyph_height = (uint8)(10);
            title_text.columns = (uint16)(25);
            title_text.vertical_scale = (uint16)(1);
            title_text.texture_y = (uint16)(384);
            title_text.texture_x = (uint16)(384);
            title_text.palette_x = (uint16)(400);
            title_text.palette_y = (uint16)(511);
            title_text.first_character = (uint8)(48);
            break;
        default:
            return;
    }
    x = title_text.palette_y;
    y = title_text.palette_x;
    title_text.clut = (uint16)((x << 6) | ((y >> 4) & 63));
    title_text.proportional = (uint8)(tail == 3 || tail == 4);
    if (tail == 3 || tail == 4)
    {
        title_text.advance_widths = title_glyph_advances + (tail == 3 ? 96 : 0);
        title_text.space_width = (uint8)(4);
    }
    else if (tail == 7)
    {
        title_text.space_width = (uint8)(6);
        title_text.blend_enabled = (uint8)(0);
        title_text.blend_mode = (uint8)(0);
    }
}

void title_text_set_blend(uint32 selection)
{
    FUNCTION_MARKER(0x800D7C88, "ACE3.BIN");
    if ((sint16)selection == -1)
    {
        title_text.blend_enabled = (uint8)(0);
        title_text.blend_mode = (uint8)(0);
    }
    else
    {
        title_text.blend_enabled = (uint8)(1);
        title_text.blend_mode = (uint8)(selection);
    }
    return;
}

void title_text_set_reveal_tick(uint32 value)
{
    FUNCTION_MARKER(0x800D7CD4, "ACE3.BIN");
    title_text.interpolation_tick = (uint16)(value);
    return;
}

void title_text_set_reveal_range(uint32 enabled, uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800D7CE4, "ACE3.BIN");
    title_text.interpolate = (uint8)(enabled);
    title_text.interpolation_start = (uint16)(first);
    title_text.interpolation_end = (uint16)(second);
    return;
}

void title_text_begin_frame(uint32 packet)
{
    uint32 *gpu_word;
    uint32 index, buffer, ordering, link, tag, result;

    FUNCTION_MARKER(0x800D81F0, "ACE3.BIN");
    index = game_runtime.display_buffer;
    if (index >= 2)
        abort();
    buffer = title_text.packet_pages[index];
    title_text.packet_cursor = buffer;
    title_text.frame_packets = title_text.packet_data[index];
    title_text.frame_packet_link = buffer;
    index = game_runtime.display_buffer;
    if (index >= 2)
        abort();
    ordering = title_text.ordering_pages[index];
    title_text.ordering = ordering;
    ClearOTagR(text_ot_span(ordering, 4), 1);
    ordering = title_text.ordering;
    gpu_word = text_ot_span(packet, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = text_ot_span(ordering, sizeof(*gpu_word));
    tag = *gpu_word;
    gpu_word = text_ot_span(ordering, sizeof(*gpu_word));
    *gpu_word = (tag & 0xFF000000) | (link & 0xFFFFFF);
    gpu_word = text_ot_span(packet, sizeof(*gpu_word));
    link = *gpu_word;
    ordering = title_text.ordering;
    result = ordering & 0xFFFFFF;
    gpu_word = text_ot_span(packet, sizeof(*gpu_word));
    *gpu_word = (link & 0xFF000000) | result;
    {
        (void)(uint32)(result);
        return;
    }
}

void title_text_emit_glyph(uint32 character)
{
    uint32 *gpu_word;
    uint32 packet, ordering, relative, width, height, u, v, right, bottom;
    uint32 color, blend, page_y, page_x, tag, link, result, advance;
    uint32 vertex, x, y, product, numerator, extra, quotient, second_quotient;
    sint32 columns, column, row, texture_x, texture_y, scale, end, current, start, divisor, half;
    int interpolate = 0;
    POLY_FT4 *quad;

    FUNCTION_MARKER(0x800D82A4, "ACE3.BIN");
    relative = title_text.first_character;
    columns = (sint16)title_text.columns;
    relative = (character - relative) & 255;
    if (columns == 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource001 glyph division trap7\n");
#endif
        abort();
    }
    row = (sint32)relative / columns;
    column = (sint32)relative % columns;
    width = title_text.glyph_width;
    texture_x = (sint16)title_text.texture_x;
    u = (uint32)column * width + (uint32)(texture_x % 64) * 4;
    height = title_text.glyph_height;
    texture_y = (sint16)title_text.texture_y;
    v = (uint32)row * height + (uint32)(texture_y % 256);
    packet = title_text.packet_cursor;
    if (!title_text.frame_packets || (packet & 3) || packet - title_text.frame_packet_link > TITLE_TEXT_PACKET_COUNT * sizeof(*quad) - sizeof(*quad))
        abort();
    quad = (POLY_FT4 *)((uint8 *)title_text.frame_packets + (packet - title_text.frame_packet_link));
    quad->p[0].uv.u = (uint8)(u);
    packet = title_text.packet_cursor;
    quad->p[0].uv.v = (uint8)(v);
    right = u + 255;
    width = title_text.glyph_width;
    packet = title_text.packet_cursor;
    quad->p[1].uv.u = (uint8)(width + right);
    packet = title_text.packet_cursor;
    quad->p[1].uv.v = (uint8)(v);
    packet = title_text.packet_cursor;
    quad->p[2].uv.u = (uint8)(u);
    bottom = v + 255;
    height = title_text.glyph_height;
    packet = title_text.packet_cursor;
    quad->p[2].uv.v = (uint8)(height + bottom);
    width = title_text.glyph_width;
    packet = title_text.packet_cursor;
    quad->p[3].uv.u = (uint8)(width + right);
    height = title_text.glyph_height;
    packet = title_text.packet_cursor;
    quad->p[3].uv.v = (uint8)(height + bottom);
    blend = title_text.blend_enabled;
    packet = title_text.packet_cursor;
    color = quad->rgbc.code;
    quad->rgbc.code = (uint8)(blend ? color | 2 : color & 0xFD);
    packet = title_text.packet_cursor;
    color = title_text.red;
    quad->rgbc.r = (uint8)(color);
    packet = title_text.packet_cursor;
    color = title_text.green;
    quad->rgbc.g = (uint8)(color);
    packet = title_text.packet_cursor;
    color = title_text.blue;
    quad->rgbc.b = (uint8)(color);
    blend = title_text.blend_mode;
    page_y = title_text.texture_y;
    page_x = title_text.texture_x;
    packet = title_text.packet_cursor;
    quad->p[1].attr = (uint16)(((blend & 3) << 5) | ((page_y & 0x100) >> 4) | ((page_x & 0x3FF) >> 6) | ((page_y & 0x200) << 2));
    packet = title_text.packet_cursor;
    color = title_text.clut;
    quad->p[0].attr = (uint16)(color);
    if (title_text.interpolate == 1)
    {
        current = (sint16)title_text.interpolation_tick;
        start = (sint16)title_text.interpolation_start;
        if (current < start)
            return;
        end = (sint16)title_text.interpolation_end;
        interpolate = current < end;
    }
    for (vertex = 0; vertex < 4; ++vertex)
    {
        if ((vertex & 1) == 0)
        {
            packet = title_text.packet_cursor;
            x = title_text.x;
        }
        else
        {
            width = title_text.glyph_width;
            x = title_text.x;
            packet = title_text.packet_cursor;
            x += width + 0xFFFF;
        }
        quad->p[vertex].xy.x = (sint16)(uint16)(x);
        if (interpolate)
        {
            height = title_text.glyph_height;
            scale = (sint16)title_text.vertical_scale;
            end = (sint16)title_text.interpolation_end;
            current = (sint16)title_text.interpolation_tick;
            start = (sint16)title_text.interpolation_start;
            product = height * (uint32)scale;
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            divisor = end - start;
            if (divisor == 0 || (divisor == -1 && numerator == 0x80000000))
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource001 glyph interpolation division trap\n");
#endif
                abort();
            }
            quotient = (uint32)((sint32)numerator / divisor);
            second_quotient = 0;
            if (vertex >= 2)
            {
                extra = (height - 1) * (uint32)scale * (uint32)(current - start);
                if (divisor == -1 && extra == 0x80000000)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource001 glyph interpolation division trap6\n");
#endif
                    abort();
                }
                second_quotient = (uint32)((sint32)extra / divisor);
            }
            y = title_text.y;
            packet = title_text.packet_cursor;
            quad->p[vertex].xy.y = (sint16)(uint16)(y + quotient + second_quotient);
        }
        else if (vertex < 2)
        {
            packet = title_text.packet_cursor;
            y = title_text.y;
            quad->p[vertex].xy.y = (sint16)(uint16)(y);
        }
        else
        {
            height = title_text.glyph_height;
            scale = (sint16)title_text.vertical_scale;
            y = title_text.y;
            packet = title_text.packet_cursor;
            quad->p[vertex].xy.y = (sint16)(uint16)(y + (height - 1) * (uint32)scale);
        }
    }
    packet = title_text.packet_cursor;
    ordering = title_text.ordering;
    tag = quad->tag;
    gpu_word = text_ot_span(ordering, sizeof(*gpu_word));
    link = *gpu_word;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = title_text.ordering;
    packet = title_text.packet_cursor;
    tag = *gpu_word;
    *gpu_word = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    if (title_text.proportional)
    {
        advance = title_text.advance_widths[relative];
    }
    else
        advance = title_text.glyph_width;
    x = title_text.x;
    title_text.x = (uint16)(x + advance);
    packet = title_text.packet_cursor;
    result = packet + 40;
    title_text.packet_cursor = result;
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 title_text_draw_bytes(const uint8 *native_text, uint32 native_capacity)
{
    uint32 count, index, character, height, origin, y, x, advance, result;
    sint32 scale;

    FUNCTION_MARKER(0x800D8980, "ACE3.BIN");
    count = title_text.length;
    result = count;
    index = 0;
    if ((sint32)count > 0)
    {
        do
        {
            if (index >= native_capacity)
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource001 text exceeds native buffer capacity\n");
#endif
                abort();
            }
            character = native_text[index];
            switch (character)
            {
                case 10:
                    height = title_text.glyph_height;
                    scale = (sint16)title_text.vertical_scale;
                    origin = title_text.line_x;
                    y = title_text.y;
                    title_text.x = (uint16)(origin);
                    title_text.y = (uint16)(y + height * (uint32)scale);
                    break;
                case 9:
                    origin = title_text.line_x;
                    title_text.x = (uint16)(origin);
                    break;
                case 32:
                    if (title_text.proportional)
                        advance = title_text.space_width;
                    else
                        advance = title_text.glyph_width;
                    x = title_text.x;
                    title_text.x = (uint16)(x + advance);
                    break;
                default:
                    character = native_text[index];
                    title_text_emit_glyph(character);
                    break;
            }
            count = title_text.length;
            ++index;
            result = (sint32)index < (sint32)count;
        } while (result);
    }
    return result;
}

uint32 title_text_print(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length;

    FUNCTION_MARKER(0x800D8AA0, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, native_arguments, native_argument_bytes);
    title_text.length = (uint32)(length);
    return title_text_draw_bytes(buffer, sizeof(buffer));
}

void title_text_get_pos(uint16 *first, uint16 *second)
{
    uint32 value;
    FUNCTION_MARKER(0x800D8094, "ACE3.BIN");
    value = title_text.x;
    *first = value;
    value = title_text.y;
    *second = value;
    {
        (void)(uint32)(value);
        return;
    }
}

static void text_format_put_char(uint32 ch, FORMAT_OUTPUT *output)
{
    uint32 cursor;

    FUNCTION_MARKER(0x80027C24, "ACE3.BIN");
    cursor = output->cursor;
    if (cursor >= output->capacity)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native format output buffer exhausted\n");
#endif
        abort();
    }
    xport_store_u8(output->data + cursor, ch);
    output->cursor = cursor + 1;
    {
        (void)(uint32)(ch);
        return;
    }
}

static const uint8 *text_format_number(uint32 low, uint32 high, uint32 flags, sint32 precision, uint32 radix, uint32 *length, const uint8 *digits, uint8 *buffer)
{
    uint8 *cursor = buffer + 64;
    uint32 shift = 0;
    uint32 mask = 0;
    uint32 digit;
    uint32 ch;
    uint32 next;
    uint64 value;
    uint64 quotient;
    uint64 divisor;

    FUNCTION_MARKER(0x80027C3C, "ACE3.BIN");
    if (radix == 8)
    {
        shift = 3;
        mask = 7;
    }
    else if (radix == 2)
    {
        shift = 1;
        mask = 1;
    }
    else if (radix == 16)
    {
        shift = 4;
        mask = 15;
    }
    if (precision)
    {
        if (flags & 4)
        {
            value = ((uint64)high << 32) | low;
            if (shift)
            {
                do
                {
                    --cursor;
                    digit = (uint32)value & mask;
                    value >>= shift;
                    ch = digits[digit];
                    *cursor = ch;
                } while (value);
            }
            else
            {
                divisor = (sint32)radix;
                do
                {
                    if (!divisor)
                        trap(7);
                    quotient = value / divisor;
                    --cursor;
                    digit = (uint32)value - (uint32)quotient * radix;
                    ch = digits[digit];
                    value = quotient;
                    *cursor = ch;
                } while (value);
            }
        }
        else if (shift)
        {
            do
            {
                --cursor;
                ch = digits[low & mask];
                low >>= shift;
                *cursor = ch;
            } while (low);
        }
        else
        {
            do
            {
                if (!radix)
                    trap(7);
                next = low / radix;
                --cursor;
                ch = digits[low - next * radix];
                low = next;
                *cursor = ch;
            } while (low);
        }
    }
    *length = (uint32)(buffer + 64 - cursor);
    return cursor;
}

static uint32 text_format_emit(FORMAT_OUTPUT *output, const uint8 *native_text, uint32 flags, uint32 sign, sint32 width, sint32 precision, sint32 length)
{
    uint32 total;
    uint32 padded;
    uint32 i;
    uint32 ch;

    FUNCTION_MARKER(0x80027E80, "ACE3.BIN");
    total = ((flags >> 9) & 1) + (uint32)length + (sign != 0);
    if (flags & 0x180)
        total += 2;
    padded = (sint32)total < precision ? (uint32)precision : total;
    if (!(flags & 0x60))
        for (i = padded; (sint32)i < width; ++i)
            text_format_put_char(' ', output);
    if (sign)
        text_format_put_char(sign, output);
    else if (flags & 0x380)
    {
        text_format_put_char('0', output);
        if (flags & 0x80)
            text_format_put_char('x', output);
        else if (flags & 0x100)
            text_format_put_char('X', output);
        else if (flags & 0x400)
            text_format_put_char('b', output);
    }
    if ((flags & 0x60) == 0x40)
        for (i = padded; (sint32)i < width; ++i)
            text_format_put_char('0', output);
    for (i = total; (sint32)i < precision; ++i)
        text_format_put_char('0', output);
    for (i = 0; (sint32)i < length; ++i)
    {
        ch = native_text[i];
        text_format_put_char(ch, output);
    }
    if (flags & 0x20)
        for (i = padded; (sint32)i < width; ++i)
            text_format_put_char(' ', output);
    return (sint32)padded < width ? (uint32)width : padded;
}

static uint32 text_format_decimal_character(uint8 character)
{
    if (character < 128)
        return character - '0' < 10u;

    /* retain the original bit flags beyond the ASCII table */
    switch (character)
    {
        case 0xB5:
        case 0xC2:
        case 0xC6:
        case 0xE6:
        case 0xEA:
        case 0xEF:
        case 0xF0:
        case 0xF3:
        case 0xF4:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xFB:
        case 0xFC:
        case 0xFE:
            return 4;
        default:
            return 0;
    }
}

static uint32 text_format_core(const uint8 *format, FORMAT_OUTPUT *output, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint16 wire_halfword_4;
    uint32 wire_word_5;
    uint32 count = 0;
    uint32 native_offset = 0;
    uint32 ch;
    uint32 parsed;
    uint32 flags;
    uint32 sign;
    uint32 low;
    uint32 high;
    uint32 radix;
    const uint8 *digits;
    const uint8 *number;
    uint8 number_buffer[64];
    uint32 length;
    uint32 numeric;
    uint32 negative;
    uint8 character;
    sint32 width;
    sint32 precision;

    FUNCTION_MARKER(0x80028074, "ACE3.BIN");
    for (;;)
    {
        ch = *format++;
        while (ch && ch != '%')
        {
            text_format_put_char(ch, output);
            ++count;
            ch = *format++;
        }
        --format;
        if (!ch)
            return count;
        flags = 0;
        width = 0;
        precision = -1;
        sign = 0;
        for (;;)
        {
            ch = *++format;
            if (!ch)
                return count;
            if (ch == ' ')
            {
                if (!sign)
                    sign = ' ';
                continue;
            }
            if (ch == '#')
            {
                flags |= 0x10;
                continue;
            }
            if (ch == '+')
            {
                sign = '+';
                continue;
            }
            if (ch == '-')
            {
                flags |= 0x20;
                continue;
            }
            if (ch == '0')
            {
                if (sign != '-' && precision <= 0)
                    flags |= 0x40;
                continue;
            }
            if (ch == 'h' || ch == 'l' || ch == 'L')
            {
                flags |= ch == 'h' ? 1 : (ch == 'l' ? 2 : 12);
                continue;
            }
            if (ch == '.')
            {
                ch = *++format;
                if (ch == '*')
                {
                    if ((native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                    {
#ifndef NDEBUG
                        fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                        abort();
                    }
                    precision = (sint32)(memcpy(&wire_word_0, native_arguments + native_offset, sizeof(wire_word_0)), wire_word_0);
                    native_offset += 4;
                }
                else
                {
                    parsed = 0;
                    while (text_format_decimal_character(ch))
                    {
                        parsed = parsed * 10 - '0' + ch;
                        ch = *++format;
                    }
                    --format;
                    precision = (sint32)parsed;
                }
                continue;
            }
            if (ch == '*' || (ch >= '1' && ch <= '9'))
            {
                if (ch == '*')
                {
                    if ((native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                    {
#ifndef NDEBUG
                        fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                        abort();
                    }
                    width = (sint32)(memcpy(&wire_word_1, native_arguments + native_offset, sizeof(wire_word_1)), wire_word_1);
                    native_offset += 4;
                    if (width < 0)
                    {
                        width = (sint32)(0 - (uint32)width);
                        flags |= 0x20;
                    }
                }
                else
                {
                    parsed = 0;
                    do
                    {
                        ch = *format++;
                        parsed = parsed * 10 - '0' + ch;
                    } while (text_format_decimal_character(*format));
                    --format;
                    width = (sint32)parsed;
                }
                continue;
            }
            break;
        }
        numeric = ch == 'X' || ch == 'p' || ch == 'x' || ch == 'o' || ch == 'b' || ch == 'u' || ch == 'U' || ch == 'Z' || ch == 'D' || ch == 'I' || ch == 'd' || ch == 'i';
        if (numeric)
        {
            if (ch == 'U' || ch == 'Z' || ch == 'D' || ch == 'I')
                flags |= 2;
            if (ch == 'p' && precision == -1)
                precision = 8;
            high = 0;
            negative = ch == 'D' || ch == 'I' || ch == 'd' || ch == 'i';
            if (flags & 4)
            {
                if ((native_offset > native_argument_bytes || native_argument_bytes - native_offset < 8))
                {
#ifndef NDEBUG
                    fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                    abort();
                }
                low = (memcpy(&wire_word_2, native_arguments + native_offset, sizeof(wire_word_2)), wire_word_2);
                high = (memcpy(&wire_word_3, native_arguments + native_offset + 4, sizeof(wire_word_3)), wire_word_3);
                native_offset += 8;
            }
            else
            {
                if ((native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                {
#ifndef NDEBUG
                    fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                    abort();
                }
                if (flags & 1)
                {
                    low = (memcpy(&wire_halfword_4, native_arguments + native_offset, sizeof(wire_halfword_4)), wire_halfword_4);
                    if (negative)
                        low = (uint32)(sint32)(sint16)low;
                }
                else
                    low = (memcpy(&wire_word_5, native_arguments + native_offset, sizeof(wire_word_5)), wire_word_5);
                native_offset += 4;
                if (negative)
                    high = (uint32)((sint32)low >> 31);
            }
            radix = 10;
            digits = (const uint8 *)"0123456789abcdef";
            if (ch == 'X' || ch == 'x' || ch == 'p')
            {
                radix = 16;
                if (ch == 'X')
                    digits = (const uint8 *)"0123456789ABCDEF";
                if ((flags & 0x10) && (low | high))
                    flags |= ch == 'X' ? 0x100 : 0x80;
            }
            else if (ch == 'o')
            {
                radix = 8;
                if ((flags & 0x10) && (low | high))
                    flags |= 0x200;
            }
            else if (ch == 'b')
            {
                radix = 2;
                if ((flags & 0x10) && (low | high))
                    flags |= 0x400;
            }
            if (negative && (sint32)high < 0)
            {
                low = 0 - low;
                high = 0 - high - (low != 0);
                sign = '-';
            }
            number = text_format_number(low, high, flags, precision, radix, &length, digits, number_buffer);
            count += text_format_emit(output, number, flags, negative ? sign : 0, width, precision, (sint32)length);
        }
        else if (ch == 'c')
        {
            if ((native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
            {
#ifndef NDEBUG
                fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                abort();
            }
            character = native_arguments[native_offset];
            native_offset += 4;
            count += text_format_emit(output, &character, flags, 0, width, precision, 1);
        }
        else if (ch == 'n' || ch == 's')
        {
            /* native integer argument buffers contain no encoded pointer values */
            abort();
        }
        else
        {
            ++count;
            text_format_put_char(*format, output);
        }
        ++format;
    }
}

uint32 format_to_native_buffer(uint8 *destination, uint32 capacity, const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    FORMAT_OUTPUT output = {destination, capacity, 0};
    uint32 length;
    if (!destination || capacity == 0 || !format)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native format output requires a bounded buffer\n");
#endif
        abort();
    }
    if (native_argument_bytes == 0)
    {
        for (length = 0; format[length]; ++length)
            text_format_put_char(format[length], &output);
    }
    else
    {
        if (!native_arguments)
            abort();
        length = text_format_core(format, &output, native_arguments, native_argument_bytes);
    }
    if (length >= capacity)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native format terminator exceeds output buffer\n");
#endif
        abort();
    }
    xport_store_u8(destination + length, 0);
    return length;
}
