#ifndef UI_PACKETS_H
#define UI_PACKETS_H

#include <stddef.h>

#include "psx.h"
#include "ui_items.h"
#include "menu_binding_data.h"

typedef struct
{
    uint16 x;
    uint16 y;
} TITLE_RENDER_VIEW;

typedef struct
{
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
} TITLE_RENDER_GEOMETRY;

typedef struct
{
    uint32 words[8192];
} UI_PACKET_PAGE;



void title_renderer_reset_storage(void);
uint32 title_renderer_hint_flags(void);
uint16 title_renderer_hint_x(void);
uint16 title_renderer_hint_y(void);


typedef struct
{
    RGBC color;
    XY position;
    uint32 dimensions;
} SCREEN_FILL_COMMAND;

typedef struct
{
    uint32 tag;
    uint32 mode;
    SCREEN_FILL_COMMAND fill;
} SCREEN_MODE_FILL_PACKET;

typedef char screen_fill_packet_layout_check[
    sizeof(SCREEN_FILL_COMMAND) == 12 && offsetof(SCREEN_FILL_COMMAND, position) == 4 &&
    offsetof(SCREEN_FILL_COMMAND, dimensions) == 8 && sizeof(SCREEN_MODE_FILL_PACKET) == 20 &&
    offsetof(SCREEN_MODE_FILL_PACKET, mode) == 4 && offsetof(SCREEN_MODE_FILL_PACKET, fill) == 8 &&
    sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 &&
    offsetof(TILE, wh) == 12 && sizeof(P_TAG) == 8 && offsetof(P_TAG, rgbc) == 4 ? 1 : -1];

typedef struct
{
    uint32 entries[16];
} UI_SCENE_ORDERING_TABLE;

typedef struct
{
    uint32 entries[256];
} UI_DETAIL_ORDERING_TABLE;

typedef char ui_ordering_table_layout[
    sizeof(UI_SCENE_ORDERING_TABLE) == 64 && sizeof(UI_DETAIL_ORDERING_TABLE) == 1024 ? 1 : -1];

typedef struct
{
    uint8 prefix[2956];
    POLY_FT4 objects[32];
    uint8 between[480];
    POLY_FT4 panel;
    uint8 tail[8];
} UI_SCENE_PRIMITIVES;

typedef char ui_scene_primitives_layout[
    sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 &&
    offsetof(UI_SCENE_PRIMITIVES, objects) == 2956 &&
    offsetof(UI_SCENE_PRIMITIVES, panel) == 4716 &&
    sizeof(UI_SCENE_PRIMITIVES) == 4764 ? 1 : -1];

typedef struct
{
    uint32 packets;
    UI_PACKET_PAGE *packet_data;
    uint32 ot;
    uint32 detail_ot;
    uint32 prims_address;
    UI_SCENE_PRIMITIVES *prims;
    UI_SCENE_ORDERING_TABLE *ot_data;
    UI_DETAIL_ORDERING_TABLE *detail_ot_data;
} UI_SCENE_BUFFER;

typedef struct
{
    uint32 cursor;
    UI_SCENE_BUFFER scene_bank[2];
    uint32 scene_ot;
    uint32 scene_detail_ot;
    uint32 scene_prims_address;
    UI_SCENE_PRIMITIVES *scene_prims;
    uint32 scene_packet_cursor;
    uint32 scene_packet_begin;
    UI_PACKET_PAGE *scene_packet_data;
    UI_SCENE_ORDERING_TABLE *scene_ot_data;
    UI_DETAIL_ORDERING_TABLE *scene_detail_ot_data;
} UI_PACKET_STATE;

extern UI_PACKET_STATE ui_packets;

void ui_bind_scene_packet_page(uint32 index);
void ui_bind_scene_ordering(uint32 index);
void ui_bind_scene_primitives(uint32 index);
uint32 *ui_native_ot_span(uint32 address, size_t bytes);
void *ui_native_packet_span(uint32 address, size_t bytes);
uint32 *ui_scene_ot_span(uint32 address, size_t bytes);
uint32 *ui_detail_ot_span(uint32 address, size_t bytes);
void *ui_packet_span(uint32 address, size_t bytes);

void ui_packets_reset(void);
void ui_packets_initialize(void);

uint32 ui_packets_draw_noise_rect(uint32 object, uint32 dst, uint32 factor, const PSX_RECT *native_rect);
uint32 ui_packets_emit_noise_tiles(uint32 dst, uint32 texture, uint32 xy, uint32 wh, uint32 *last_a0);

uint32 title_renderer_ordering(void);
void title_renderer_set_view_origin(uint32 index, uint32 first, uint32 second);
void title_renderer_reset_view(uint32 index);
uint32 title_renderer_set_visible(uint32 enabled);
uint32 title_renderer_set_animation(uint32 index, uint32 enabled, sint32 value, sint32 duration);
sint32 title_renderer_animation_mode(uint32 index);
sint32 title_renderer_animation_phase(uint32 index);
void title_renderer_tick_animations(void);
void title_renderer_set_group_value(uint32 index, uint32 value);
uint32 title_renderer_transform_identity(void);
uint32 title_renderer_transform_reveal(TITLE_RENDER_GEOMETRY *geometry);
uint32 title_renderer_transform_completed(void);
uint32 title_renderer_transform_horizontal_reveal(TITLE_RENDER_GEOMETRY *geometry);
uint32 title_renderer_set_hint(uint32 x, uint32 y, uint32 first_flag, uint32 second_flag);
void title_renderer_draw_group(uint32 index);
void title_renderer_draw_visible(void);
void title_renderer_initialize(void);
void title_renderer_begin_frame(uint32 packet);
void title_renderer_draw_frame(void);
void title_renderer_bind_commands(const uint8 *data, uint32 bytes);

void title_renderer_default_group_hook(void);
void title_renderer_group10_hook(void);
void title_renderer_group12_hook(void);
void title_renderer_group13_hook(void);
void title_renderer_group14_hook(void);
void title_renderer_group17_hook(void);
void title_renderer_group18_hook(void);
void title_renderer_group19_hook(void);

uint32 title_renderer_set_name_hint_mode(uint32 mode);

uint32 ui_emit_menu_fill(const MENU_FILL_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);

extern const PSX_RECT ui_fullscreen_clip;

uint32 ui_emit_controller_label(uint32 x, uint32 y, const MENU_BINDING_IMAGE *descriptor, uint32 blend, uint32 packet);

void ui_draw_panel_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
void ui_draw_panel_tile(uint32 x, uint32 y, uint32 width, uint32 height, uint32 transparent);
uint32 ui_emit_draw_mode(uint32 mode, uint32 ordering, uint32 packet);

const uint8 *ui_command_draw_tile(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context);
const uint8 *ui_command_draw_sprite(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context);
const uint8 *ui_command_draw_textured_quad(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context);
const uint8 *ui_command_draw_line(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context);
void ui_command_idle_hook(void);
uint32 ui_draw_frontend_load_progress(uint32 packet);

const uint8 *ui_command_set_draw_mode(UI_COMMAND_STREAM input, uint32 ordering);
const uint8 *ui_command_skip(UI_COMMAND_STREAM input);
uint32 ui_render_frame(uint32 packet);
uint32 ui_draw_debrief_area(uint32 packet, uint32 mode);

uint32 ui_draw_noise_overlay(uint32 ot, uint32 packet, uint32 intensity);
void ui_compact_ordering_links(uint32 ordering, uint32 count, uint32 *entries);
uint32 ui_draw_framebuffer_feedback(uint32 ordering, uint32 packet, const XY *offset, uint32 intensity);
uint32 ui_draw_scripted_flash(uint32 ordering, uint32 packet, uint32 intensity);

uint32 ui_copy_display_framebuffer(uint32 ordering, uint32 packet);
uint32 ui_draw_letterbox(uint32 ordering, uint32 packet, uint32 progress);
uint32 ui_draw_screen_wipe(uint32 ordering, uint32 packet, uint32 direction, uint32 progress);

uint32 ui_draw_noise_rectangle(uint32 ordering, uint32 packet, PSX_RECT rectangle, uint32 intensity);

uint32 ui_draw_compact_label(uint32 object, uint32 dst, uint32 x, uint32 y, uint32 color, const uint8 *native_text);
uint32 ui_draw_loading_fade(uint32 object, uint32 dst, uint32 mode);
uint32 ui_draw_frame_border(uint32 object, uint32 dst, uint32 amount, uint32 outer_color, uint32 inner_color);

uint32 ui_draw_blend_rectangle(uint32 ot, uint32 packet, PSX_RECT rect, uint32 depth);
uint32 gpu_copy_rect(uint32 ot, uint32 packet, uint32 src_x, uint32 src_y, uint32 dst_x, uint32 dst_y, uint32 width, uint32 height);

#endif
