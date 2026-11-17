#include "game_functions.h"
#include <string.h>

typedef struct
{
    uint8 u;
    uint8 v;
    uint16 clut;
    uint16 width;
    uint16 height;
} MISSION_MENU_SPRITE;

static const MISSION_MENU_SPRITE mission_menu_sprites[17] = {
    {128, 64, 23958, 67, 10}, {128, 77, 23830, 36, 6}, {128, 84, 23830, 29, 6}, {180, 84, 23830, 30, 6}, {158, 84, 23830, 20, 6}, {128, 91, 23830, 50, 6}, {180, 91, 23830, 63, 6}, {172, 98, 23830, 39, 6}, {128, 98, 23830, 41, 6}, {212, 84, 23830, 19, 6}, {166, 77, 23830, 50, 6}, {200, 64, 23830, 31, 6}, {200, 70, 23830, 32, 6}, {230, 98, 23830, 20, 6}, {128, 105, 23958, 70, 10}, {128, 105, 23958, 29, 10}, {198, 105, 23958, 53, 10},
};

static const PSX_RECT mission_options_rectangles[2] = {
    {84, 88, 152, 63},
    {86, 88, 148, 16},
};

static const PSX_RECT mission_confirm_rectangles[2] = {
    {84, 104, 152, 31},
    {86, 104, 148, 16},
};

static const uint32 mission_options_row_y[5] = {1, 12, 23, 34, 45};
static const uint32 mission_confirm_row_y[2] = {2, 13};

typedef char mission_menu_basic_packet_layout[sizeof(SPRT) == 20 && offsetof(SPRT, tag) == 0 && offsetof(SPRT, rgbc) == 4 && offsetof(SPRT, p) == 8 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, tag) == 0 && offsetof(DR_TPAGE, code) == 4 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv_attr) == 4 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 ? 1 : -1];

static uint32 mission_menu_emit_sprite(uint32 x, uint32 y, uint32 ordering, uint32 packet, const MISSION_MENU_SPRITE *dimensions, uint32 color)
{
    uint32 value;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x80034360, "ACE3.BIN");
    SPRT *sprite = ui_packet_span(packet, sizeof(*sprite));
    sprite->p[0].xy.x = (sint16)(uint16)x;
    sprite->p[0].xy.y = (sint16)(uint16)y;
    value = color | (color << 8) | (color << 16) | 0x66000000;
    sprite->rgbc.rgbc = value;
    value = (uint32)dimensions->u | ((uint32)dimensions->v << 8) | ((uint32)dimensions->clut << 16);
    sprite->p[0].uv_attr = value;
    value = (uint32)dimensions->width | ((uint32)dimensions->height << 16);
    memcpy((uint8 *)sprite + offsetof(SPRT, w), &value, sizeof(value));
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    value = *ordering_entry;
    sprite->tag = value | 0x4000000;
    *ordering_entry = packet & 0xFFFFFF;
    return packet + 20;
}

typedef char menu_surface_packet_layout_check[sizeof(SPRT_16) == 16 && offsetof(SPRT_16, rgbc) == 4 && offsetof(SPRT_16, p) == 8 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 ? 1 : -1];

static uint32 mission_menu_emit_window(uint32 ordering, uint32 packet, const PSX_RECT *rectangle, uint32 mode)
{
    uint32 x, y, width, height, right, bottom, corner_color, edge_color, fill_color, palette, i, current, value, uv;
    uint32 fields[7][4];
    FUNCTION_MARKER(0x800343C8, "ACE3.BIN");
    x = rectangle->x;
    y = rectangle->y;
    width = rectangle->w;
    height = rectangle->h;
    if (mode == 1)
    {
        palette = 0x5DD6;
        corner_color = 0x76808080;
        edge_color = 0x62000000;
        fill_color = 0x62484848;
    }
    else if (mode == 2)
    {
        palette = 0x5D56;
        corner_color = 0x76808080;
        edge_color = 0x62000098;
        fill_color = 0x620030F8;
    }
    else
    {
        palette = 0x5D56;
        corner_color = 0x74808080;
        edge_color = 0x60000098;
        fill_color = 0x600030F8;
    }
    right = x + width;
    bottom = y + height;
    for (i = 0; i < 4; ++i)
    {
        current = packet + i * 16;
        SPRT_16 *corner = ui_packet_span(current, sizeof(*corner));
        uv = (palette << 16) | ((i & 1) != 0 ? 0x40F8 : 0x40E8) | ((i & 2) != 0 ? 0x1000 : 0);
        corner->rgbc.rgbc = corner_color;
        corner->p[0].xy.x = (sint16)(uint16)((i & 1) != 0 ? right - 8 : x);
        corner->p[0].xy.y = (sint16)(uint16)((i & 2) != 0 ? bottom - 8 : y);
        corner->p[0].uv_attr = uv;
        value = *(uint32 *)ui_scene_ot_span(ordering, 4);
        corner->tag = value | 0x03000000;
        *(uint32 *)ui_scene_ot_span(ordering, 4) = current & 0xFFFFFF;
    }
    fields[0][0] = x;
    fields[0][1] = y + 8;
    fields[0][2] = 2;
    fields[0][3] = height - 16;
    fields[1][0] = right - 2;
    fields[1][1] = y + 8;
    fields[1][2] = 2;
    fields[1][3] = height - 16;
    fields[2][0] = x + 8;
    fields[2][1] = y;
    fields[2][2] = width - 16;
    fields[2][3] = 2;
    fields[3][0] = x + 8;
    fields[3][1] = bottom - 2;
    fields[3][2] = width - 16;
    fields[3][3] = 2;
    fields[4][0] = x + 8;
    fields[4][1] = y + 2;
    fields[4][2] = width - 16;
    fields[4][3] = 6;
    fields[5][0] = x + 2;
    fields[5][1] = y + 8;
    fields[5][2] = width - 4;
    fields[5][3] = height - 16;
    fields[6][0] = x + 8;
    fields[6][1] = bottom - 8;
    fields[6][2] = width - 16;
    fields[6][3] = 6;
    for (i = 0; i < 7; ++i)
    {
        current = packet + 64 + i * 16;
        TILE *tile = ui_packet_span(current, sizeof(*tile));
        tile->rgbc.rgbc = i < 4 ? edge_color : fill_color;
        tile->p[0].x = (sint16)(uint16)(fields[i][0]);
        tile->p[0].y = (sint16)(uint16)(fields[i][1]);
        tile->w = (sint16)(uint16)(fields[i][2]);
        tile->h = (sint16)(uint16)(fields[i][3]);
        value = *(uint32 *)ui_scene_ot_span(ordering, 4);
        tile->tag = value | 0x03000000;
        *(uint32 *)ui_scene_ot_span(ordering, 4) = current & 0xFFFFFF;
    }
    return packet + 176;
}

uint32 mission_menu_draw_mode(uint32 ordering, uint32 packet, uint32 mode)
{
    uint32 packet_tag, head;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800346C0, "ACE3.BIN");
    DR_TPAGE *draw_mode = ui_packet_span(packet, sizeof(*draw_mode));
    ((uint8 *)&draw_mode->tag)[3] = 1;
    draw_mode->code[0] = (mode & 0x9FF) | 0xE1000000;
    packet_tag = draw_mode->tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    head = *ordering_entry;
    draw_mode->tag = (packet_tag & 0xFF000000) | (head & 0xFFFFFF);
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    head = *ordering_entry;
    *ordering_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 8;
}

uint32 mission_menu_draw_options(uint32 ordering, uint32 packet)
{
    PSX_RECT rectangles[2];
    const MISSION_MENU_SPRITE *dimensions;
    uint32 x, y, color, mode, offset, index, row, value;
    FUNCTION_MARKER(0x80034930, "ACE3.BIN");
    rectangles[0] = mission_options_rectangles[0];
    rectangles[1] = mission_options_rectangles[1];
    value = (sint16)game_runtime.mission_menu.options_sel;
    color = value == 0 ? 96 : 64;
    mode = value == 0 ? 53 : 85;
    x = rectangles[1].x;
    y = rectangles[1].y;
    offset = mission_options_row_y[0];
    packet = mission_menu_emit_sprite(x + 6, y + offset + 5, ordering, packet, &mission_menu_sprites[1], color);
    packet = mission_menu_draw_mode(ordering, packet, mode);
    value = (sint16)game_runtime.mission_menu.options_sel;
    color = value == 1 ? 96 : 64;
    mode = value == 1 ? 53 : 85;
    offset = mission_options_row_y[1];
    packet = mission_menu_emit_sprite(x + 6, y + offset + 5, ordering, packet, &mission_menu_sprites[8], color);
    offset = mission_options_row_y[1];
    packet = mission_menu_emit_sprite(x + 51, y + offset + 5, ordering, packet, &mission_menu_sprites[7], color);
    offset = mission_options_row_y[1];
    index = game_runtime.options.kill_camera;
    dimensions = &mission_menu_sprites[3 + index];
    packet = mission_menu_emit_sprite(x + 113, y + offset + 5, ordering, packet, dimensions, color);
    packet = mission_menu_draw_mode(ordering, packet, mode);
    value = (sint16)game_runtime.mission_menu.options_sel;
    color = value == 2 ? 96 : 64;
    mode = value == 2 ? 53 : 85;
    offset = mission_options_row_y[2];
    packet = mission_menu_emit_sprite(x + 6, y + offset + 5, ordering, packet, &mission_menu_sprites[2], color);
    offset = mission_options_row_y[2];
    index = game_runtime.options.hud_hidden;
    dimensions = index != 0 ? &mission_menu_sprites[3] : &mission_menu_sprites[4];
    packet = mission_menu_emit_sprite(x + 114, y + offset + 5, ordering, packet, dimensions, color);
    packet = mission_menu_draw_mode(ordering, packet, mode);
    for (row = 3; row < 5; ++row)
    {
        value = (sint16)game_runtime.mission_menu.options_sel;
        color = value == row ? 96 : 64;
        mode = value == row ? 53 : 85;
        offset = mission_options_row_y[row];
        dimensions = row == 3 ? &mission_menu_sprites[9] : &mission_menu_sprites[5];
        packet = mission_menu_emit_sprite(x + 6, y + offset + 5, ordering, packet, dimensions, color);
        if (row == 3)
        {
            offset = mission_options_row_y[3];
            packet = mission_menu_emit_sprite(x + 25, y + offset + 5, ordering, packet, &mission_menu_sprites[10], color);
        }
        offset = mission_options_row_y[row];
        packet = mission_menu_emit_sprite(x + 80, y + offset + 5, ordering, packet, &mission_menu_sprites[6], color);
        packet = mission_menu_draw_mode(ordering, packet, mode);
    }
    index = (sint16)game_runtime.mission_menu.options_sel;
    value = (uint16)rectangles[1].y;
    offset = (uint16)mission_options_row_y[index];
    rectangles[1].y = (sint16)(uint16)(value + offset);
    packet = mission_menu_emit_window(ordering, packet, &rectangles[1], 1);
    packet = mission_menu_draw_mode(ordering, packet, 85);
    packet = mission_menu_emit_window(ordering, packet, &rectangles[0], 2);
    packet = mission_menu_emit_sprite(125, 74, ordering, packet, &mission_menu_sprites[0], 192);
    return mission_menu_draw_mode(ordering, packet, 21);
}

uint32 mission_menu_draw_confirm(uint32 ordering, uint32 packet)
{
    PSX_RECT rectangles[2];
    uint32 value, x, y, offset, color, mode, index;
    FUNCTION_MARKER(0x80035184, "ACE3.BIN");
    rectangles[0] = mission_confirm_rectangles[0];
    rectangles[1] = mission_confirm_rectangles[1];
    value = (sint16)game_runtime.mission_menu.confirm_sel;
    color = value == 0 ? 96 : 64;
    mode = value == 0 ? 53 : 85;
    x = rectangles[1].x;
    y = rectangles[1].y;
    offset = mission_confirm_row_y[0];
    packet = mission_menu_emit_sprite(x + 58, y + offset + 5, ordering, packet, &mission_menu_sprites[12], color);
    packet = mission_menu_draw_mode(ordering, packet, mode);
    value = (sint16)game_runtime.mission_menu.confirm_sel;
    color = value == 1 ? 96 : 64;
    mode = value == 1 ? 53 : 85;
    x = rectangles[1].x;
    y = rectangles[1].y;
    offset = mission_confirm_row_y[1];
    packet = mission_menu_emit_sprite(x + 64, y + offset + 5, ordering, packet, &mission_menu_sprites[13], color);
    packet = mission_menu_draw_mode(ordering, packet, mode);
    index = (sint16)game_runtime.mission_menu.confirm_sel;
    value = (uint16)rectangles[1].y;
    offset = (uint16)mission_confirm_row_y[index];
    rectangles[1].y = (sint16)(uint16)(value + offset);
    packet = mission_menu_emit_window(ordering, packet, &rectangles[1], 1);
    packet = mission_menu_draw_mode(ordering, packet, 85);
    packet = mission_menu_emit_window(ordering, packet, &rectangles[0], 2);
    packet = mission_menu_emit_sprite(125, 90, ordering, packet, &mission_menu_sprites[14], 192);
    return mission_menu_draw_mode(ordering, packet, 21);
}

uint32 mission_menu_draw_status_banner(uint32 ordering, uint32 packet)
{
    uint32 tick, value, color, elapsed;
    FUNCTION_MARKER(0x8003545C, "ACE3.BIN");
    tick = game_runtime.gameplay_counter;
    value = (uint16)game_wave_at(((tick << 10) & 0x7C00));
    value = (uint32)((sint16)value >> 3) + 0x800;
    color = (sint32)((value << 3) + value) >> 8;
    packet = mission_menu_draw_mode(ordering, packet, 53);
    packet = mission_menu_emit_sprite(16, 28, ordering, packet, &mission_menu_sprites[15], color);
    packet = mission_menu_emit_sprite(45, 28, ordering, packet, &mission_menu_sprites[16], color);
    packet = mission_menu_draw_mode(ordering, packet, 53);
    elapsed = game_runtime.mission_control.ticks;
    if ((sint32)elapsed >= 91)
    {
        value = (elapsed - 90) << 2;
        if ((sint32)value >= 65)
            value = 64;
        packet = ui_draw_compact_label(ordering, packet, 48, 212, value << 8, (const uint8 *)"PRESS START BUTTON TO EXIT");
    }
    return packet;
}

void mission_menu_update_game_options(void)
{
    uint32 value, current, flags, result;
    FUNCTION_MARKER(0x80034718, "ACE3.BIN");
    value = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(value | 0x40);
    flags = game_input.repeat;
    if ((flags & 1) != 0)
    {
        value = (uint16)game_runtime.mission_menu.options_sel;
        game_runtime.mission_menu.options_sel = (uint16)(value - 1);
    }
    flags = game_input.repeat;
    if ((flags & 2) != 0)
    {
        value = (uint16)game_runtime.mission_menu.options_sel;
        game_runtime.mission_menu.options_sel = (uint16)(value + 1);
    }
    value = (sint16)game_runtime.mission_menu.options_sel;
    if ((sint32)value < 0)
    {
        game_runtime.mission_menu.options_sel = (uint16)(4);
        value = (sint16)game_runtime.mission_menu.options_sel;
    }
    if ((sint32)value >= 5)
        game_runtime.mission_menu.options_sel = (uint16)(0);
    current = (sint16)game_runtime.mission_menu.options_sel;
    if (current == 1)
    {
        flags = game_input.repeat;
        if ((flags & 12) != 0)
        {
            value = game_runtime.options.kill_camera;
            game_runtime.options.kill_camera = (uint8)(value == 0);
        }
    }
    current = (sint16)game_runtime.mission_menu.options_sel;
    if (current == 2)
    {
        flags = game_input.repeat;
        if ((flags & 12) != 0)
        {
            value = game_runtime.options.hud_hidden;
            game_runtime.options.hud_hidden = (uint8)(value == 0);
            value = game_runtime.options.hud_hidden;
            mission_apply_hud_visibility(value == 0);
        }
    }
    flags = game_input.pressed;
    if ((flags & 0x40) != 0)
        game_runtime.mission_menu.options_sel = (uint16)(0);
    flags = game_input.pressed;
    if ((flags & 0x10) == 0)
        return;
    current = (sint16)game_runtime.mission_menu.options_sel;
    if (current == 3)
    {
        value = game_runtime.flags;
        game_runtime.flags = value | 0x08000000;
        result = game_runtime.mission_control.flags | 0x40000000;
        game_runtime.mission_control.flags = (uint32)(result);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if ((sint32)current < 4)
    {
        if (current == 0)
        {
            result = game_runtime.flags | 0x08000000;
            game_runtime.flags = result;
            {
                (void)(uint32)(result);
                return;
            }
        }
        return;
    }
    if (current != 4)
        return;
    value = game_runtime.flags;
    game_runtime.flags = value | 0x08000000;
    result = game_runtime.mission_control.flags | 0x80000000;
    game_runtime.mission_control.flags = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}
