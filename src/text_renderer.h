#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "psx.h"

enum { TITLE_TEXT_PACKET_COUNT = 1000 };

typedef struct
{
    uint8 red;
    uint8 green;
    uint8 blue;
    uint32 length;
    uint32 packet_pages[2];
    uint32 ordering_pages[2];
    POLY_FT4 *packet_data[2];
    uint32 *ot_data[2];
    POLY_FT4 *frame_packets;
    uint32 frame_packet_link;
    uint32 packet_cursor;
    uint32 ordering;
    const uint8 *advance_widths;
    uint16 x;
    uint16 y;
    uint16 line_x;
    uint16 clut;
    uint16 columns;
    uint16 vertical_scale;
    uint16 texture_x;
    uint16 texture_y;
    uint16 palette_x;
    uint16 palette_y;
    uint8 glyph_width;
    uint8 glyph_height;
    uint8 first_character;
    uint8 proportional;
    uint8 blend_enabled;
    uint8 blend_mode;
    uint8 space_width;
    uint8 interpolate;
    uint16 interpolation_start;
    uint16 interpolation_end;
    uint16 interpolation_tick;
} TITLE_TEXT_STATE;

extern TITLE_TEXT_STATE title_text;


enum { MISSION_TIME_TEXT_SIZE = 10 };

void format_mission_time(uint8 text[MISSION_TIME_TEXT_SIZE], sint32 duration);
void text_set_pos(uint32 x, uint32 y);
void text_set_color(uint32 red, uint32 green, uint32 blue);
void text_set_blend(uint32 mode);
void text_set_reveal_step(uint32 enabled);
void text_set_reveal_range(uint32 enabled, uint32 first, uint32 second);
void text_select_font(uint32 layout);
void text_renderer_reset(void);

void debug_text_reset_state(void);
void debug_text_initialize(void);
uint32 debug_text_print(uint32 x, uint32 y, const uint8 *text);
void debug_text_hex(uint32 x, uint32 y, uint32 value, uint32 width);
uint32 debug_text_decimal(uint32 x, uint32 y, uint32 value, uint32 format);
uint32 debug_text_draw(uint32 ot, uint32 packet);
void text_renderer_init(void);
void text_begin_frame(uint32 ordering);
void text_set_ot(uint32 ordering);
uint32 text_print_spr(const char *text);
uint32 text_print(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);
void text_print_digits(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);

uint32 title_text_print(const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);
void title_text_set_pos(uint32 first, uint32 second);
void title_text_set_color(uint32 red, uint32 green, uint32 blue);
void title_text_set_blend(uint32 selection);
void title_text_set_reveal_tick(uint32 value);
void title_text_set_reveal_range(uint32 enabled, uint32 first, uint32 second);
void title_text_select_font(uint32 mode);
void title_text_get_pos(uint16 *first, uint16 *second);
void title_text_initialize(void);
void title_text_begin_frame(uint32 packet);
void title_text_emit_glyph(uint32 character);
uint32 title_text_draw_bytes(const uint8 *native_text, uint32 native_capacity);

uint32 format_to_native_buffer(uint8 *destination, uint32 capacity, const uint8 *format, const uint8 *native_arguments, uint32 native_argument_bytes);

#endif
