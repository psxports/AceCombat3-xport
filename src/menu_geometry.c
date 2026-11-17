#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint8 bar[3];
    uint8 badge;
} WEAPON_MENU_STATS;

static const WEAPON_MENU_STATS weapon_menu_stats[] = {
    {{5, 13, 6}, 0}, {{8, 12, 4}, 0}, {{14, 9, 6}, 0}, {{6, 13, 8}, 1}, {{15, 0, 15}, 1}, {{10, 12, 10}, 1}, {{0, 0, 0}, 2}, {{8, 8, 13}, 0}, {{8, 13, 4}, 0}, {{12, 8, 2}, 0}, {{2, 6, 6}, 0}, {{8, 8, 13}, 1}, {{15, 6, 15}, 1}, {{15, 6, 0}, 0}, {{15, 6, 0}, 1}, {{7, 8, 13}, 0},
};

static MENU_POLYGON_DESCRIPTOR menu_geometry;

typedef struct
{
    uint8 high[4];
    uint8 low[4];
    uint8 lower_limit[4];
    uint8 upper_limit[4];
    uint32 phase;
} MENU_CALIBRATION;

static MENU_CALIBRATION menu_calibration;

void menu_geometry_reset(void)
{
    memset(&menu_geometry, 0, sizeof(menu_geometry));
    memset(&menu_calibration, 0, sizeof(menu_calibration));
}

static uint32 menu_geometry_draw_quad(uint32 packet, uint32 ordering)
{
    return ui_emit_colored_quad(&menu_geometry, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
}

static uint32 menu_geometry_draw_triangle(uint32 packet, uint32 ordering)
{
    return ui_emit_colored_triangle(&menu_geometry, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
}

uint32 menu_calibration_begin(void)
{
    uint32 index, result;
    FUNCTION_MARKER(0x800D911C, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        menu_calibration.high[index] = (uint8)(136);
        menu_calibration.low[index] = (uint8)(128);
        menu_calibration.lower_limit[index] = (uint8)(16);
        menu_calibration.upper_limit[index] = (uint8)(240);
    }
    menu_geometry.color[0][0] = (uint8)(240);
    menu_geometry.color[0][1] = (uint8)(110);
    menu_geometry.color[0][2] = (uint8)(0);
    menu_geometry.color[1][0] = (uint8)(240);
    menu_geometry.color[1][1] = (uint8)(110);
    menu_geometry.color[1][2] = (uint8)(0);
    menu_geometry.color[1][3] = (uint8)(0);
    menu_geometry.color[0][3] = (uint8)(0);
    result = menu_state.header.phase;
    menu_calibration.phase = (uint32)(0);
    ++result;
    menu_state.header.phase = (uint8)(result);
    return result;
}

void menu_calibration_negcon_frame(void)
{
    static const uint16 segments[10][4] = {{130, 40, 150, 40}, {130, 110, 150, 110}, {130, 180, 150, 180}, {130, 40, 130, 180}, {150, 40, 150, 180}, {170, 40, 190, 40}, {170, 110, 190, 110}, {170, 180, 190, 180}, {170, 40, 170, 180}, {190, 40, 190, 180}};
    MENU_LINE_DESCRIPTOR line = {{{0, 0}, {0, 0}}, {128, 128, 128}, 0};
    uint32 phase, value, index, coordinate, packet, ordering, result;
    sint32 axis, positive = 0, negative = 0, offset;
    FUNCTION_MARKER(0x800D91FC, "ACE3.BIN");
    phase = menu_calibration.phase;
    axis = (sint16)pad_state[0].axis[1];
    if (phase == 0)
    {
        offset = (axis - 128) * 70 / 128;
        if (axis >= 128)
            positive = offset;
        else
            negative = offset;
        if ((game_input.pressed & 64) && axis >= 145)
        {
            sound_play(60);
            menu_calibration.high[1] = (uint8)((uint32)axis);
            menu_calibration.phase = (uint32)(1);
        }
    }
    else if (phase == 1)
    {
        positive = ((sint32)menu_calibration.high[1] - 128) * 70 / 128;
        if (axis < 128)
            negative = (axis - 128) * 70 / 128;
        if ((game_input.pressed & 64) && axis < 112)
        {
            sound_play(60);
            menu_calibration.low[1] = (uint8)((uint32)axis);
            menu_calibration.phase = (uint32)(2);
        }
    }
    else if (phase == 2)
    {
        positive = ((sint32)menu_calibration.high[1] - 128) * 70 / 128;
        negative = ((sint32)menu_calibration.low[1] - 128) * 70 / 128;
        if (game_input.pressed & 64)
        {
            sound_play(60);
            value = menu_calibration.low[1];
            pad_profiles[4].cal[1].low = (uint8)value;
            value = (uint8)pad_state[0].axis[1];
            pad_profiles[4].cal[1].center = (uint8)value;
            value = menu_calibration.high[1];
            pad_profiles[4].cal[1].high = (uint8)value;
            menu_state.header.kind = (uint8)(0);
            menu_state.header.phase = (uint8)(0);
            ui_item_reset_motion(2, 0);
            mission_menu_start_item_motion(0, 1);
        }
    }
    value = (uint8)menu_calibration.phase;
    *ui_widget_value(20) = (uint8)(value + 74);
    for (index = 0; index < 10; ++index)
    {
        for (coordinate = 0; coordinate < 4; ++coordinate)
            line.point[coordinate / 2][coordinate % 2] = segments[index][coordinate];
        packet = menu_state.packet_cursor;
        ordering = ui_packets.scene_ot;
        result = ui_emit_line_pair(&line, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        menu_state.packet_cursor = (uint32)(result);
    }
    menu_geometry.points[0].x = (uint16)(130);
    menu_geometry.points[0].y = (uint16)(110);
    menu_geometry.points[1].x = (uint16)(150);
    menu_geometry.points[1].y = (uint16)(110);
    menu_geometry.points[2].x = (uint16)(130);
    menu_geometry.points[3].x = (uint16)(150);
    menu_geometry.color[2][0] = (uint8)(64);
    menu_geometry.color[2][2] = (uint8)(0);
    menu_geometry.color[3][0] = (uint8)(64);
    menu_geometry.color[3][2] = (uint8)(0);
    menu_geometry.points[2].y = (uint16)((uint32)(positive + 110));
    menu_geometry.points[3].y = (uint16)((uint32)(positive + 110));
    menu_geometry.color[2][1] = (uint8)(55);
    menu_geometry.color[3][1] = (uint8)(55);
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    menu_geometry.points[0].x = (uint16)(130);
    menu_geometry.points[0].y = (uint16)(110);
    menu_geometry.points[1].x = (uint16)(150);
    menu_geometry.points[1].y = (uint16)(110);
    menu_geometry.points[2].x = (uint16)(130);
    menu_geometry.points[3].x = (uint16)(150);
    menu_geometry.points[2].y = (uint16)((uint32)(negative + 110));
    menu_geometry.points[3].y = (uint16)((uint32)(negative + 110));
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    menu_geometry.points[0].x = (uint16)(170);
    menu_geometry.points[0].y = (uint16)(110);
    menu_geometry.points[1].x = (uint16)(190);
    menu_geometry.points[1].y = (uint16)(110);
    menu_geometry.points[2].x = (uint16)(170);
    menu_geometry.points[2].y = (uint16)((uint32)(110 - negative));
    menu_geometry.points[3].y = (uint16)((uint32)(110 - negative));
    menu_geometry.points[3].x = (uint16)(190);
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    menu_geometry.points[0].x = (uint16)(170);
    menu_geometry.points[0].y = (uint16)(110);
    menu_geometry.points[1].x = (uint16)(190);
    menu_geometry.points[1].y = (uint16)(110);
    menu_geometry.points[2].x = (uint16)(170);
    menu_geometry.points[2].y = (uint16)((uint32)(110 - positive));
    menu_geometry.points[3].y = (uint16)((uint32)(110 - positive));
    menu_geometry.points[3].x = (uint16)(190);
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    offset = ((sint32)(sint16)pad_state[0].axis[1] - 128) * 70 / 128;
    menu_geometry.points[0].y = (uint16)((uint32)(offset + 110));
    menu_geometry.points[1].y = (uint16)((uint32)(offset + 100));
    menu_geometry.points[2].y = (uint16)((uint32)(offset + 120));
    menu_geometry.points[0].x = (uint16)(130);
    menu_geometry.points[1].x = (uint16)(120);
    menu_geometry.points[2].x = (uint16)(120);
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_triangle(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    menu_geometry.points[0].y = (uint16)((uint32)(110 - offset));
    menu_geometry.points[1].y = (uint16)((uint32)(100 - offset));
    menu_geometry.points[2].y = (uint16)((uint32)(120 - offset));
    menu_geometry.points[0].x = (uint16)(190);
    menu_geometry.points[1].x = (uint16)(200);
    menu_geometry.points[2].x = (uint16)(200);
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_triangle(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_calibration_joystick_frame(void)
{
    static const uint8 *const limits[8][2] = {{&menu_calibration.low[2], &menu_calibration.high[3]}, {&menu_calibration.high[2], &menu_calibration.high[3]}, {&menu_calibration.high[2], &menu_calibration.low[3]}, {&menu_calibration.low[2], &menu_calibration.low[3]}, {&menu_calibration.low[0], &menu_calibration.high[1]}, {&menu_calibration.high[0], &menu_calibration.high[1]}, {&menu_calibration.high[0], &menu_calibration.low[1]}, {&menu_calibration.low[0], &menu_calibration.low[1]}};
    static const uint16 outlines[4][8] = {{105, 40, 140, 110, 105, 180, 70, 110}, {105, 180, 105, 40, 70, 110, 140, 110}, {215, 40, 250, 110, 215, 180, 180, 110}, {215, 180, 215, 40, 180, 110, 250, 110}};
    uint32 packet, ordering, result, index, channel, group, phase, value, raw;
    MENU_POLYLINE4_DESCRIPTOR outline = {0};
    sint32 horizontal, vertical, axis;
    FUNCTION_MARKER(0x800D9994, "ACE3.BIN");
    for (group = 0; group < 4; ++group)
        for (channel = 0; channel < 3; ++channel)
            menu_geometry.color[group][channel] = (uint8)(128);
    horizontal = ((sint32)(sint16)pad_state[0].axis[3] - 128) * 35 / 128;
    vertical = ((sint32)(sint16)pad_state[0].axis[2] - 128) * 70 / 128;
    menu_geometry.points[0].x = (uint16)((uint32)(horizontal + 105));
    menu_geometry.points[0].y = (uint16)((uint32)(vertical + 104));
    menu_geometry.points[1].x = (uint16)((uint32)(horizontal + 108));
    menu_geometry.points[2].x = (uint16)((uint32)(horizontal + 102));
    menu_geometry.points[3].y = (uint16)((uint32)(vertical + 116));
    menu_geometry.points[1].y = (uint16)((uint32)(vertical + 110));
    menu_geometry.points[2].y = (uint16)((uint32)(vertical + 110));
    menu_geometry.points[3].x = (uint16)((uint32)(horizontal + 105));
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    horizontal = ((sint32)(sint16)pad_state[0].axis[1] - 128) * 35 / 128;
    vertical = ((sint32)(sint16)pad_state[0].axis[0] - 128) * 70 / 128;
    menu_geometry.points[0].x = (uint16)((uint32)(horizontal + 215));
    menu_geometry.points[0].y = (uint16)((uint32)(vertical + 104));
    menu_geometry.points[1].x = (uint16)((uint32)(horizontal + 218));
    menu_geometry.points[2].x = (uint16)((uint32)(horizontal + 212));
    menu_geometry.points[1].y = (uint16)((uint32)(vertical + 110));
    menu_geometry.points[2].y = (uint16)((uint32)(vertical + 110));
    menu_geometry.points[3].x = (uint16)((uint32)(horizontal + 215));
    menu_geometry.points[3].y = (uint16)((uint32)(vertical + 116));
    packet = menu_state.packet_cursor;
    ordering = ui_packets.scene_ot;
    result = menu_geometry_draw_quad(packet, ordering);
    menu_state.packet_cursor = (uint32)(result);
    for (index = 0; index < 4; ++index)
    {
        axis = (sint16)pad_state[0].axis[index];
        value = menu_calibration.low[index];
        raw = pad_state[0].axis[index];
        if (axis < (sint32)value)
            menu_calibration.low[index] = (uint8)(raw);
        value = menu_calibration.high[index];
        if ((sint32)value < axis)
            menu_calibration.high[index] = (uint8)(raw);
    }
    menu_geometry.color[0][0] = (uint8)(240);
    menu_geometry.color[0][1] = (uint8)(110);
    menu_geometry.color[0][2] = (uint8)(0);
    menu_geometry.color[1][0] = (uint8)(64);
    menu_geometry.color[1][1] = (uint8)(55);
    menu_geometry.color[1][2] = (uint8)(0);
    menu_geometry.color[2][0] = (uint8)(64);
    menu_geometry.color[2][1] = (uint8)(55);
    menu_geometry.color[2][2] = (uint8)(0);
    for (index = 0; index < 8; ++index)
    {
        value = index < 4 ? 105 : 215;
        menu_geometry.points[0].x = (uint16)(value);
        menu_geometry.points[0].y = (uint16)(110);
        menu_geometry.points[1].x = (uint16)(value);
        value = *limits[index][0];
        menu_geometry.points[1].y = (uint16)(value * 140 / 256 + 40);
        value = *limits[index][1];
        menu_geometry.points[2].x = (uint16)(value * 70 / 256 + (index < 4 ? 70 : 180));
        if (index == 4)
        {
            menu_geometry.points[2].y = (uint16)(110);
        }
        else
        {
            menu_geometry.points[2].y = (uint16)(110);
        }
        packet = menu_state.packet_cursor;
        ordering = ui_packets.scene_ot;
        result = menu_geometry_draw_triangle(packet, ordering);
        menu_state.packet_cursor = (uint32)(result);
    }
    phase = 0;
    for (index = 0; index < 4; ++index)
    {
        for (channel = 0; channel < 4; ++channel)
            outline.points[channel].xy = outlines[index][channel * 2] | ((uint32)outlines[index][channel * 2 + 1] << 16);
        packet = menu_state.packet_cursor;
        ordering = ui_packets.scene_ot;
        result = ui_emit_polyline4_pair(&outline, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        if (index == 3)
            phase = menu_calibration.phase;
        menu_state.packet_cursor = (uint32)(result);
    }
    switch (phase)
    {
        case 0:
        {
        }
            if (menu_calibration.low[0] < 64 && menu_calibration.low[2] < 64)
                menu_calibration.phase = (uint32)(1);
            break;
        case 1:
        {
        }
            if (menu_calibration.high[0] >= 193 && menu_calibration.high[2] >= 193)
                menu_calibration.phase = (uint32)(2);
            break;
        case 2:
        {
        }
            if (menu_calibration.low[1] < 64 && menu_calibration.low[3] < 64)
                menu_calibration.phase = (uint32)(3);
            break;
        case 3:
        {
        }
            if (menu_calibration.high[1] >= 193 && menu_calibration.high[3] >= 193)
                menu_calibration.phase = (uint32)(4);
            break;
        case 4:
            if (game_input.pressed & 64)
            {
                sound_play(60);
                for (index = 0; index < 4; ++index)
                {
                    value = menu_calibration.low[index];
                    pad_profiles[3].cal[index].low = (uint8)value;
                    value = (uint8)pad_state[0].axis[index];
                    pad_profiles[3].cal[index].center = (uint8)value;
                    value = menu_calibration.high[index];
                    pad_profiles[3].cal[index].high = (uint8)value;
                }
                menu_state.header.kind = (uint8)(0);
                menu_state.header.phase = (uint8)(0);
                ui_item_reset_motion(2, 0);
                mission_menu_start_item_motion(0, 1);
            }
            break;
        default:
            break;
    }
    value = (uint8)menu_calibration.phase;
    result = value + 69;
    *ui_widget_value(20) = (uint8)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 menu_brightness_frame(void)
{
    static const uint8 starts[] = {72, 83, 99};
    static const uint8 colors[3][6] = {{0, 255, 0, 0, 255, 0}, {0, 0, 255, 0, 0, 255}, {255, 255, 255, 255, 255, 255}};
    uint32 packet, ordering, result, index, x;
    FUNCTION_MARKER(0x800DB400, "ACE3.BIN");
    if (game_input.pressed & 0x30)
    {
        ui_animation_set(1, 1, 0, 30);
        menu_state.header.phase = (uint8)(1);
        ui_group_set_mask(2, 4096);
        sound_play(59);
    }
    menu_geometry.color[0][3] = (uint8)(0);
    menu_geometry.points[0].x = (uint16)(60);
    menu_geometry.points[2].x = (uint16)(60);
    menu_geometry.color[0][0] = (uint8)(0);
    menu_geometry.points[0].y = (uint16)(152);
    menu_geometry.points[1].x = (uint16)(65);
    menu_geometry.points[1].y = (uint16)(152);
    menu_geometry.points[2].y = (uint16)(320);
    menu_geometry.points[3].x = (uint16)(65);
    menu_geometry.points[3].y = (uint16)(320);
    menu_geometry.color[0][1] = (uint8)(0);
    menu_geometry.color[0][2] = (uint8)(0);
    menu_geometry.color[1][0] = (uint8)(0);
    menu_geometry.color[1][1] = (uint8)(0);
    menu_geometry.color[1][2] = (uint8)(0);
    menu_geometry.color[2][0] = (uint8)(255);
    menu_geometry.color[2][1] = (uint8)(0);
    menu_geometry.color[2][2] = (uint8)(0);
    menu_geometry.color[3][0] = (uint8)(255);
    menu_geometry.color[3][1] = (uint8)(0);
    menu_geometry.color[3][2] = (uint8)(0);
    ordering = ui_packets.scene_ot + 4;
    packet = menu_state.packet_cursor;
    result = menu_geometry_draw_quad(packet, ordering);
    for (index = 0; index < 3; ++index)
    {
        menu_geometry.color[2][0] = (uint8)(colors[index][0]);
        menu_state.packet_cursor = (uint32)(result);
        x = starts[index];
        menu_geometry.points[0].x = (uint16)(x);
        menu_geometry.points[0].y = (uint16)(152);
        menu_geometry.points[1].x = (uint16)(x + 5);
        menu_geometry.points[1].y = (uint16)(152);
        menu_geometry.points[2].x = (uint16)(x);
        menu_geometry.points[2].y = (uint16)(320);
        menu_geometry.points[3].x = (uint16)(x + 5);
        menu_geometry.points[3].y = (uint16)(320);
        menu_geometry.color[2][1] = (uint8)(colors[index][1]);
        menu_geometry.color[2][2] = (uint8)(colors[index][2]);
        menu_geometry.color[3][0] = (uint8)(colors[index][3]);
        menu_geometry.color[3][1] = (uint8)(colors[index][4]);
        menu_geometry.color[3][2] = (uint8)(colors[index][5]);
        ordering = ui_packets.scene_ot + 4;
        packet = menu_state.packet_cursor;
        result = menu_geometry_draw_quad(packet, ordering);
    }
    menu_state.packet_cursor = (uint32)(result);
    return result;
}

uint32 title_geometry_emit_gouraud_quad(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, color, component, offset, index, tag, link;

    POLY_G4 *quad = ui_packet_span(packet, sizeof(*quad));
    FUNCTION_MARKER(0x800D94C4, "ACE3.BIN");
    quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(uint8)(8) << 24);
    quad->p[0].rgbc.code = (uint8)(0x38);
    value = descriptor->color[0][3];
    quad->p[0].rgbc.code = (uint8)(value ? 0x3A : 0x38);
    for (color = 0; color < 4; ++color)
        for (component = 0; component < 3; ++component)
        {
            offset = 16 + color * 4 + component;
            value = descriptor->color[color][component];
            ((uint8 *)&quad->p[color].rgbc)[component] = (uint8)value;
        }
    for (index = 0; index < 8; ++index)
    {
        offset = index * 2;
        value = (index & 1) ? descriptor->points[index / 2].y : descriptor->points[index / 2].x;
        if (index == 7)
            tag = quad->tag;
        if (index & 1)
            quad->p[index / 2].xy.y = (sint16)(uint16)value;
        else
            quad->p[index / 2].xy.x = (sint16)(uint16)value;
    }
    link = *ot_entry;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 36;
}

uint32 title_geometry_emit_gouraud_triangle(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, color, component, offset, index, tag, link;

    POLY_G3 *triangle = ui_packet_span(packet, sizeof(*triangle));
    FUNCTION_MARKER(0x800D9624, "ACE3.BIN");
    triangle->tag = (triangle->tag & 0xFFFFFF) | ((uint32)(uint8)(6) << 24);
    triangle->p[0].rgbc.code = (uint8)(0x30);
    value = descriptor->color[0][3];
    triangle->p[0].rgbc.code = (uint8)(value ? 0x32 : 0x30);
    for (color = 0; color < 3; ++color)
        for (component = 0; component < 3; ++component)
        {
            offset = 16 + color * 4 + component;
            value = descriptor->color[color][component];
            ((uint8 *)&triangle->p[color].rgbc)[component] = (uint8)value;
        }
    for (index = 0; index < 5; ++index)
    {
        offset = index * 2;
        value = (index & 1) ? descriptor->points[index / 2].y : descriptor->points[index / 2].x;
        if (index & 1)
            triangle->p[index / 2].xy.y = (sint16)(uint16)value;
        else
            triangle->p[index / 2].xy.x = (sint16)(uint16)value;
    }
    value = descriptor->points[2].y;
    tag = triangle->tag;
    triangle->p[2].xy.y = (sint16)(uint16)value;
    link = *ot_entry;
    triangle->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 28;
}

uint32 title_geometry_emit_textured_quad(const MENU_TEXTURED_QUAD_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, index, offset, width, height, flags, texture_x, texture_y, tag, link;
    sint32 coordinate, u, v;

    POLY_FT4 *quad = ui_packet_span(packet, sizeof(*quad));
    FUNCTION_MARKER(0x800D9748, "ACE3.BIN");
    quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(uint8)(9) << 24);
    quad->rgbc.code = (uint8)(0x2C);
    value = descriptor->semitransparent;
    quad->rgbc.code = (uint8)(value ? 0x2E : 0x2C);
    for (index = 0; index < 3; ++index)
    {
        value = descriptor->color[index];
        ((uint8 *)&quad->rgbc)[index] = (uint8)value;
    }
    for (index = 0; index < 8; ++index)
    {
        offset = index * 2;
        value = (index & 1) ? descriptor->points[index / 2].y : descriptor->points[index / 2].x;
        if (index & 1)
            quad->p[index / 2].xy.y = (sint16)(uint16)value;
        else
            quad->p[index / 2].xy.x = (sint16)(uint16)value;
    }
    for (index = 0; index < 4; ++index)
    {
        coordinate = (sint16)descriptor->texture_x;
        u = (sint32)((uint32)(coordinate % 64) << 16) >> 14;
        if (index & 1)
        {
            width = (uint8)descriptor->width;
            u = ((uint32)u + width);
        }
        quad->p[index].uv.u = (uint8)u;
        coordinate = (sint16)descriptor->texture_y;
        v = coordinate % 256;
        if (index >= 2)
        {
            height = (uint8)descriptor->height;
            v = ((uint32)v + height);
        }
        quad->p[index].uv.v = (uint8)v;
    }
    flags = descriptor->blend;
    texture_y = descriptor->texture_y;
    texture_x = descriptor->texture_x;
    value = ((flags & 3) << 5) | ((texture_y & 0x100) >> 4) | ((texture_x & 0x3FF) >> 6) | ((texture_y & 0x200) << 2);
    quad->p[1].attr = (uint16)value;
    texture_y = descriptor->palette_y;
    texture_x = descriptor->palette_x;
    quad->p[0].attr = (uint16)((texture_y << 6) | ((texture_x >> 4) & 0x3F));
    tag = quad->tag;
    link = *ot_entry;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 40;
}

uint32 title_geometry_draw_rotating_marker(uint32 x, uint32 y, uint32 angle, uint32 packet)
{
    uint32 ordering, layer, vertex, turn, radius, first, second, sum;
    MENU_TEXTURED_QUAD_DESCRIPTOR descriptor;
    sint32 cosine, sine;

    FUNCTION_MARKER(0x800D9A08, "ACE3.BIN");
    ordering = title_renderer_ordering();
    descriptor.color[0] = 128;
    descriptor.color[1] = 128;
    descriptor.color[2] = 128;
    descriptor.semitransparent = 0;
    descriptor.blend = 0;
    descriptor.palette_x = 384;
    descriptor.palette_y = 10;
    for (layer = 0; layer < 2; ++layer)
    {
        turn = layer == 0 ? angle : 0 - angle;
        radius = layer == 0 ? 7 : 13;
        descriptor.texture_x = layer == 0 ? 739 : 738;
        descriptor.texture_y = layer == 0 ? 148 : 164;
        descriptor.width = (uint16)radius;
        descriptor.height = layer == 0 ? 15 : 27;
        for (vertex = 0; vertex < 4; ++vertex)
        {
            cosine = rcos((sint32)turn);
            sine = rsin((sint32)turn);
            first = (uint32)cosine * radius;
            second = (uint32)sine * radius;
            if ((vertex & 1) == 0)
                first = 0 - first;
            if (vertex < 2)
                second = 0 - second;
            first = ((sint32)first / 4096);
            second = ((sint32)second / 4096);
            sum = first + second;
            descriptor.points[vertex].x = (uint16)(x + (uint32)((sint32)sum / 2));
            sine = rsin((sint32)turn);
            cosine = rcos((sint32)turn);
            first = (uint32)sine * radius;
            second = (uint32)cosine * radius;
            if (vertex & 1)
                first = 0 - first;
            if (vertex < 2)
                second = 0 - second;
            first = ((sint32)first / 4096);
            second = ((sint32)second / 4096);
            descriptor.points[vertex].y = (uint16)(y + first + second);
        }
        packet = title_geometry_emit_textured_quad(&descriptor, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    }
    return packet;
}

uint32 title_geometry_emit_fill(const MENU_FILL_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, index, offset, tag, link;

    TILE *fill = ui_packet_span(packet, sizeof(*fill));
    FUNCTION_MARKER(0x800D9400, "ACE3.BIN");
    fill->tag = (fill->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
    fill->rgbc.code = (uint8)(0x60);
    value = descriptor->semi;
    fill->rgbc.code = (uint8)(value ? 0x62 : 0x60);
    for (index = 0; index < 3; ++index)
    {
        value = (index == 0) ? descriptor->red : (index == 1) ? descriptor->green : descriptor->blue;
        ((uint8 *)&fill->rgbc)[index] = (uint8)value;
    }
    for (index = 0; index < 3; ++index)
    {
        offset = index * 2;
        value = (index == 0) ? descriptor->x : (index == 1) ? descriptor->y : descriptor->width;
        if (index == 0)
            fill->p[0].x = (sint16)(uint16)value;
        else if (index == 1)
            fill->p[0].y = (sint16)(uint16)value;
        else
            fill->w = (sint16)(uint16)value;
    }
    value = descriptor->height;
    tag = fill->tag;
    fill->h = (sint16)(uint16)value;
    link = *ot_entry;
    fill->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 16;
}

uint32 title_geometry_draw_brightness(void)
{
    static const uint16 left[4] = {90, 102, 113, 129};
    static const uint16 right[4] = {95, 107, 118, 134};
    uint32 packet, ordering, result, strip, index;
    uint16 coordinates[8];
    MENU_POLYGON_DESCRIPTOR descriptor;
    uint8 red, green, blue;

    FUNCTION_MARKER(0x800DBB30, "ACE3.BIN");
    descriptor.color[0][3] = 0;
    descriptor.color[0][0] = 0;
    descriptor.color[0][1] = 0;
    descriptor.color[0][2] = 0;
    descriptor.color[1][0] = 0;
    descriptor.color[1][1] = 0;
    descriptor.color[1][2] = 0;
    for (strip = 0; strip < 4; ++strip)
    {
        coordinates[0] = left[strip];
        coordinates[1] = 213;
        coordinates[2] = right[strip];
        coordinates[3] = 213;
        coordinates[4] = left[strip];
        coordinates[5] = 381;
        coordinates[6] = right[strip];
        coordinates[7] = 381;
        for (index = 0; index < 8; ++index)
        {
            if (index & 1)
                descriptor.points[index / 2].y = coordinates[index];
            else
                descriptor.points[index / 2].x = coordinates[index];
        }
        red = strip == 0 || strip == 3 ? 255 : 0;
        green = strip == 1 || strip == 3 ? 255 : 0;
        blue = strip >= 2 ? 255 : 0;
        descriptor.color[2][0] = red;
        descriptor.color[2][1] = green;
        descriptor.color[2][2] = blue;
        descriptor.color[3][0] = red;
        descriptor.color[3][1] = green;
        descriptor.color[3][2] = blue;
        packet = title_menu.packet_cursor;
        ordering = title_renderer_ordering();
        result = title_geometry_emit_gouraud_quad(&descriptor, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        title_menu.packet_cursor = result;
    }
    return result;
}

uint32 title_geometry_emit_line(const MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, tag, link;
    LINE_F2 *line = ui_packet_span(packet, sizeof(*line));
    FUNCTION_MARKER(0x800D9024, "ACE3.BIN");
    line->tag = (line->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
    line->rgbc.code = (uint8)(0x40);
    value = descriptor->semitransparent;
    line->rgbc.code = (uint8)(value ? 0x42 : 0x40);
    value = descriptor->color[0];
    line->rgbc.r = (uint8)value;
    value = descriptor->color[1];
    line->rgbc.g = (uint8)value;
    value = descriptor->color[2];
    line->rgbc.b = (uint8)value;
    value = descriptor->point[0][0];
    line->p[0].x = (sint16)(uint16)value;
    value = descriptor->point[0][1];
    line->p[0].y = (sint16)(uint16)value;
    value = descriptor->point[1][0];
    line->p[1].x = (sint16)(uint16)value;
    value = descriptor->point[1][1];
    tag = line->tag;
    line->p[1].y = (sint16)(uint16)value;
    link = *ot_entry;
    line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 16;
}

uint32 title_geometry_emit_double_line(MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 ordering)
{
    uint32 result, first_y, second_y;
    FUNCTION_MARKER(0x800D90E8, "ACE3.BIN");
    result = title_geometry_emit_line(descriptor, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    first_y = descriptor->point[0][1];
    second_y = descriptor->point[1][1];
    first_y += 1;
    second_y += 1;
    descriptor->point[0][1] = (uint16)first_y;
    descriptor->point[1][1] = (uint16)second_y;
    return title_geometry_emit_line(descriptor, result, ui_scene_ot_span(ordering, sizeof(uint32)));
}

typedef char overlay_polygon_packet_layout_check[sizeof(POLY_G3) == 28 && sizeof(POLY_G4) == 36 && offsetof(POLY_G3, p) == 4 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

typedef char overlay_textured_quad_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

typedef char overlay_fill_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 ? 1 : -1];

typedef char overlay_line_packet_layout_check[sizeof(LINE_F2) == 16 && offsetof(LINE_F2, rgbc) == 4 && offsetof(LINE_F2, p) == 8 && sizeof(XY) == 4 ? 1 : -1];

uint32 mission_save_draw_rotating_marker(uint32 x, uint32 y, uint32 ordering, uint32 angle, uint32 packet)
{
    uint32 pass, vertex, current, tag = 0, link, rotation;
    sint32 size, sx, sy, first, second, offset;
    FUNCTION_MARKER(0x800E6DFC, "ACE3.BIN");
    for (pass = 0; pass < 2; ++pass)
    {
        current = packet + pass * 40;
        size = pass ? 15 : 8;
        rotation = pass ? 0 - angle : angle;
        POLY_FT4 *quad = ui_packet_span(current, sizeof(*quad));
        ((uint8 *)&quad->tag)[3] = (uint8)(9);
        quad->rgbc.code = (uint8)(44);
        quad->p[1].attr = (uint16)(31);
        quad->p[0].attr = (uint16)(664);
        quad->rgbc.r = (uint8)(128);
        quad->rgbc.g = (uint8)(128);
        quad->rgbc.b = (uint8)(128);
        quad->p[0].uv.v = (uint8)(pass ? 228 : 240);
        quad->p[1].uv.v = (uint8)(pass ? 228 : 240);
        quad->p[0].uv.u = (uint8)(pass ? 0 : 16);
        quad->p[1].uv.u = (uint8)(pass ? 13 : 23);
        quad->p[2].uv.u = (uint8)(pass ? 0 : 16);
        quad->p[2].uv.v = (uint8)(255);
        quad->p[3].uv.u = (uint8)(pass ? 13 : 23);
        quad->p[3].uv.v = (uint8)(255);
        for (vertex = 0; vertex < 4; ++vertex)
        {
            sx = vertex & 1 ? size : -size;
            sy = vertex & 2 ? size : -size;
            first = rcos((sint32)rotation);
            second = rsin((sint32)rotation);
            offset = (sx * first / 4096 + sy * second / 4096) / 2;
            quad->p[vertex].xy.x = (sint16)(uint16)(x + (uint32)offset);
            first = rsin((sint32)rotation);
            second = rcos((sint32)rotation);
            offset = -sx * first / 4096 + sy * second / 4096;
            if (vertex == 3)
                tag = quad->tag;
            quad->p[vertex].xy.y = (sint16)(uint16)(y + (uint32)offset);
        }
        link = *(uint32 *)ui_scene_ot_span(ordering, 4);
        quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *(uint32 *)ui_scene_ot_span(ordering, 4);
        *(uint32 *)ui_scene_ot_span(ordering, 4) = (link & 0xFF000000) | (current & 0xFFFFFF);
    }
    return packet + 80;
}

uint32 menu_geometry_draw_weapons(uint32 packet, uint32 ordering)
{
    const WEAPON_MENU_STATS *stats;
    DR_MODE *draw;
    SPRT *spr;
    uint32 *ot_entry;
    PSX_RECT rectangle;
    MENU_FILL_DESCRIPTOR descriptor;
    uint32 old_primary, old_secondary, current_primary, current_secondary;
    uint32 primary_timer, secondary_timer, primary_phase, secondary_phase, slide_timer, slide_phase;
    uint32 cursor, next, tag, link, clut, offset, index, first, second, value, result;
    uint32 slide = 0;
    sint32 clip_x;
    FUNCTION_MARKER(0x800F8FA8, "ACE3.BIN");
    if (ui_animation_phase(8) == 0)
        return packet;
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    first = (sint16)game_runtime.selection.mission;
    second = (sint16)game_runtime.selection.aircraft;
    index = mission_aircraft.weapon_idx[0];
    old_primary = (uint8)aircraft_weapon_id(first, second, 0, index);
    first = (sint16)game_runtime.selection.mission;
    second = (sint16)game_runtime.selection.aircraft;
    index = mission_aircraft.weapon_idx[1];
    old_secondary = (uint8)aircraft_weapon_id(first, second, 2, index);
    first = (sint16)game_runtime.selection.mission;
    second = (sint16)game_runtime.selection.aircraft;
    index = mission_aircraft.pending_weapon_idx[0];
    current_primary = aircraft_weapon_id(first, second, 0, index);
    first = (sint16)game_runtime.selection.mission;
    second = (sint16)game_runtime.selection.aircraft;
    index = mission_aircraft.pending_weapon_idx[1];
    current_secondary = (uint8)aircraft_weapon_id(first, second, 2, index);
    primary_timer = ui_animation_ticks(9);
    secondary_timer = ui_animation_ticks(10);
    primary_phase = ui_animation_phase(9);
    secondary_phase = ui_animation_phase(10);
    slide_timer = ui_animation_ticks(8);
    slide_phase = ui_animation_phase(8);
    cursor = packet;
    next = packet + 12;
    draw = ui_packet_span(cursor, sizeof(*draw));
    SetDrawArea(draw, &ui_fullscreen_clip);
    tag = draw->tag;
    link = *ot_entry;
    draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    if (slide_phase == 1 || slide_phase == 3)
    {
        value = 25 - slide_timer;
        value = value * (value * 325);
        slide = ((sint32)value / 625);
    }
    // Define the original uninitialized slide for phases outside 1, 2 and 3
    clut = ((((current_primary & 255) >> 3) + 24) << 6) | 0x1B;
    cursor = next;
    next += 20;
    spr = ui_packet_span(cursor, sizeof(*spr));
    offset = ((sint32)(primary_timer * 48) / 10);
    if (primary_phase != 3)
    {
        setlen(spr, 4);
        spr->rgbc.code = 102;
        spr->rgbc.r = 128;
        spr->rgbc.g = 128;
        spr->rgbc.b = 128;
        spr->w = 48;
        spr->h = 88;
        spr->p[0].attr = clut;
        spr->p[0].xy.y = 68;
        spr->p[0].uv.u = 48 * (old_primary & 3);
        spr->p[0].uv.v = 88 * ((old_primary >> 2) & 1) + 10;
        tag = spr->tag;
        spr->p[0].xy.x = (sint16)(offset + slide + 234);
        link = *ot_entry;
        spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
        cursor = next;
        next += 20;
        spr = ui_packet_span(cursor, sizeof(*spr));
        spr->p[0].uv.u = 48 * (current_primary & 3);
        setlen(spr, 4);
        tag = spr->tag;
        spr->rgbc.code = 102;
        spr->rgbc.r = 128;
        spr->rgbc.g = 128;
        spr->rgbc.b = 128;
        spr->w = 48;
        spr->h = 88;
        spr->p[0].attr = clut;
        spr->p[0].xy.x = (sint16)(offset + slide + 186);
        spr->p[0].xy.y = 68;
        spr->p[0].uv.v = 88 * ((current_primary >> 2) & 1) + 10;
    }
    else
    {
        spr->p[0].uv.u = 48 * (current_primary & 3);
        spr->p[0].uv.v = 88 * ((current_primary >> 2) & 1) + 10;
        setlen(spr, 4);
        tag = spr->tag;
        spr->rgbc.code = 102;
        spr->rgbc.r = 128;
        spr->rgbc.g = 128;
        spr->rgbc.b = 128;
        spr->w = 48;
        spr->h = 88;
        spr->p[0].attr = clut;
        spr->p[0].xy.y = 68;
        spr->p[0].xy.x = (sint16)(offset + slide + 234);
        link = *ot_entry;
        spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
        cursor = next;
        next += 20;
        spr = ui_packet_span(cursor, sizeof(*spr));
        setlen(spr, 4);
        spr->rgbc.code = 102;
        spr->rgbc.r = 128;
        spr->rgbc.g = 128;
        spr->rgbc.b = 128;
        spr->w = 48;
        spr->h = 88;
        spr->p[0].attr = clut;
        spr->p[0].xy.x = (sint16)(offset + slide + 186);
        spr->p[0].xy.y = 68;
        spr->p[0].uv.u = 48 * (old_primary & 3);
        tag = spr->tag;
        spr->p[0].uv.v = 88 * ((old_primary >> 2) & 1) + 10;
    }
    link = *ot_entry;
    spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    cursor = next;
    next += 12;
    draw = ui_packet_span(cursor, sizeof(*draw));
    rectangle.x = (sint16)(slide + 234);
    rectangle.y = 68;
    rectangle.w = 48;
    rectangle.h = 88;
    clip_x = rectangle.x;
    if (clip_x + 48 >= 320)
        rectangle.w = (sint16)(320 - (slide + 234));
    if (clip_x < 320)
    {
        SetDrawArea(draw, &rectangle);
        tag = draw->tag;
        link = *ot_entry;
        draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    }
    cursor = next;
    next += 20;
    spr = ui_packet_span(cursor, sizeof(*spr));
    first = secondary_phase == 3 ? current_secondary : old_secondary;
    second = secondary_phase == 3 ? old_secondary : current_secondary;
    setlen(spr, 4);
    spr->rgbc.code = 102;
    spr->rgbc.r = 128;
    spr->rgbc.g = 128;
    spr->rgbc.b = 128;
    spr->w = 48;
    spr->h = 88;
    spr->p[0].xy.y = 184;
    spr->p[0].attr = (((first >> 3) + 24) << 6) | 0x1B;
    offset = ((sint32)(secondary_timer * 48) / 10);
    tag = spr->tag;
    clut = (((second >> 3) + 24) << 6) | 0x1B;
    value = 48 * (first & 3);
    if (secondary_phase == 3)
        value += slide;
    spr->p[0].uv.u = value;
    spr->p[0].uv.v = 88 * ((first >> 2) & 1) + 10;
    spr->p[0].xy.x = (sint16)(offset + slide + 234);
    link = *ot_entry;
    spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    cursor = next;
    next += 20;
    spr = ui_packet_span(cursor, sizeof(*spr));
    value = 48 * (second & 3);
    if (secondary_phase == 3)
        value += slide;
    setlen(spr, 4);
    spr->rgbc.code = 102;
    spr->rgbc.r = 128;
    spr->rgbc.g = 128;
    spr->rgbc.b = 128;
    spr->w = 48;
    spr->h = 88;
    spr->p[0].attr = clut;
    spr->p[0].xy.x = (sint16)(offset + slide + 186);
    spr->p[0].xy.y = 184;
    spr->p[0].uv.u = value;
    tag = spr->tag;
    spr->p[0].uv.v = 88 * ((second >> 2) & 1) + 10;
    link = *ot_entry;
    spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    cursor = next;
    next += 12;
    draw = ui_packet_span(cursor, sizeof(*draw));
    rectangle.y = 184;
    rectangle.w = 48;
    rectangle.h = 88;
    clip_x = (sint16)(slide + 234);
    rectangle.x = (sint16)(slide + 234);
    if (clip_x + 48 >= 320)
        rectangle.w = (sint16)(320 - (slide + 234));
    if (clip_x < 320)
    {
        SetDrawArea(draw, &rectangle);
        tag = draw->tag;
        link = *ot_entry;
        draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    }
    cursor = next;
    next += 12;
    draw = ui_packet_span(cursor, sizeof(*draw));
    SetDrawMode(draw, 0, 0, 76, NULL);
    tag = draw->tag;
    link = *ot_entry;
    draw->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    index = mission_aircraft.weapon_info;
    descriptor.red = 229;
    descriptor.green = 229;
    descriptor.blue = 213;
    descriptor.semi = 0;
    descriptor.y = (uint16)(314);
    descriptor.x = (uint16)(slide + 266);
    if (index >= sizeof(weapon_menu_stats) / sizeof(*weapon_menu_stats))
        abort();
    stats = &weapon_menu_stats[index];
    value = stats->bar[0];
    descriptor.height = (uint16)(8);
    descriptor.width = (uint16)(value * 38 / 15);
    result = ui_emit_menu_fill(&descriptor, game_runtime.mission_control.packet_cur, ui_scene_ot_span(ordering + 4, sizeof(uint32)));
    game_runtime.mission_control.packet_cur = (uint32)(result);
    descriptor.x = (uint16)(slide + 266);
    descriptor.y = (uint16)(333);
    value = stats->bar[1];
    descriptor.height = (uint16)(8);
    descriptor.width = (uint16)(value * 38 / 15);
    result = ui_emit_menu_fill(&descriptor, game_runtime.mission_control.packet_cur, ui_scene_ot_span(ordering + 4, sizeof(uint32)));
    game_runtime.mission_control.packet_cur = (uint32)(result);
    descriptor.x = (uint16)(slide + 266);
    descriptor.y = (uint16)(352);
    value = stats->bar[2];
    descriptor.height = (uint16)(8);
    descriptor.width = (uint16)(value * 38 / 15);
    result = ui_emit_menu_fill(&descriptor, game_runtime.mission_control.packet_cur, ui_scene_ot_span(ordering + 4, sizeof(uint32)));
    game_runtime.mission_control.packet_cur = (uint32)(result);
    value = stats->badge;
    *ui_widget_value(12) = (uint8)(value);
    value = mission_aircraft.weapon_info;
    *ui_widget_value(13) = (uint8)(value);
    return next;
}

uint32 menu_geometry_aircraft_stats_frame(uint32 packet)
{
    FUNCTION_MARKER(0x800F7EA4, "ACE3.BIN");
    return menu_geometry_draw_aircraft_stats(packet);
}

uint32 menu_geometry_draw_aircraft_stats(uint32 packet)
{
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint32 wire_word_17;
    uint32 wire_word_18;
    static const XY radar_points[] = {
        {0x00000000u}, {0xFFAE0000u}, {0xFFE0FFDCu}, {0x0040FFE8u}, {0x00400018u}, {0xFFE00024u},
    };
    static const uint8 fan[] = {1, 2, 5, 3, 4, 1};
    static const uint8 number_slots[] = {1, 0, 2};
    MENU_LINE_DESCRIPTOR line;
    uint16 points[6][2];
    MENU_POLYGON_DESCRIPTOR triangle;
    uint8 argument[4];
    const AIRCRAFT_PROFILE *current, *previous;
    uint32 timer, delta, slide, value, old[5], fresh[5], levels[5];
    uint32 old_number[5], fresh_number[5], number[5], index, target, ordering, text_x, text_right, mode, quotient;
    uint32 x_product[5], y_product[5], origin_x, origin_y;
    FUNCTION_MARKER(0x800F7EAC, "ACE3.BIN");
    if (mission_aircraft.flags == 0)
        return packet;
    current = mission_aircraft.profile;
    previous = mission_aircraft.prev_profile;
    timer = ui_animation_ticks(6);
    if ((sint32)timer >= 21)
        timer = 20;
    delta = 20 - timer;
    value = delta * delta * 325;
    slide = ((sint32)value / 400);
    if (!current)
        abort();
    if (ui_animation_phase(2) == 1)
    {
        (void)current->retained_tag;
        (void)current->widget5_base;
        (void)current->widget6_value;
        (void)current->retained[0];
        timer = ui_animation_ticks(2);
        if (!previous)
            abort();
        old[0] = previous->chart_levels[0];
        fresh[0] = current->chart_levels[0];
        old[1] = previous->chart_levels[1];
        fresh[1] = current->chart_levels[1];
        old[2] = previous->chart_levels[2];
        fresh[2] = current->chart_levels[2];
        old[3] = previous->chart_levels[3];
        fresh[3] = current->chart_levels[3];
        old_number[0] = (uint32)previous->hundredths[0];
        fresh_number[0] = (uint32)current->hundredths[0];
        old[4] = previous->chart_levels[4];
        fresh[4] = current->chart_levels[4];
        old_number[1] = (uint32)previous->hundredths[1];
        fresh_number[1] = (uint32)current->hundredths[1];
        old_number[2] = (uint32)previous->hundredths[2];
        fresh_number[2] = (uint32)current->hundredths[2];
        fresh_number[3] = current->integer_value;
        old_number[3] = previous->integer_value;
        old_number[4] = (uint32)previous->signed_integer_value;
        fresh_number[4] = (uint32)current->signed_integer_value;
        for (index = 0; index < 5; ++index)
        {
            levels[index] = old[index] * 25 + (fresh[index] - old[index]) * timer;
            value = (fresh_number[index] - old_number[index]) * timer;
            number[index] = old_number[index] + (uint32)((sint32)value / 25);
        }
    }
    else
    {
        number[0] = (uint32)current->hundredths[0];
        number[1] = (uint32)current->hundredths[1];
        number[2] = (uint32)current->hundredths[2];
        number[3] = current->integer_value;
        levels[0] = current->chart_levels[0] * 25;
        number[4] = (uint32)current->signed_integer_value;
        levels[1] = current->chart_levels[1] * 25;
        levels[2] = current->chart_levels[2] * 25;
        levels[3] = current->chart_levels[3] * 25;
        levels[4] = current->chart_levels[4] * 25;
    }
    x_product[0] = ((sint16)levels[0] * (sint32)radar_points[1].x);
    y_product[0] = ((sint16)levels[0] * (sint32)radar_points[1].y);
    x_product[1] = ((sint16)levels[1] * (sint32)radar_points[2].x);
    y_product[1] = ((sint16)levels[1] * (sint32)radar_points[2].y);
    x_product[2] = ((sint16)levels[2] * (sint32)radar_points[4].x);
    y_product[2] = ((sint16)levels[2] * (sint32)radar_points[4].y);
    x_product[3] = ((sint16)levels[3] * (sint32)radar_points[5].x);
    y_product[3] = ((sint16)levels[3] * (sint32)radar_points[5].y);
    origin_x = (uint16)radar_points[0].x;
    origin_y = (uint16)radar_points[0].y;
    x_product[4] = ((sint16)levels[4] * (sint32)radar_points[3].x);
    y_product[4] = ((sint16)levels[4] * (sint32)radar_points[3].y);
    points[0][0] = (origin_x + 236);
    points[0][1] = (origin_y + 308);
    for (index = 0; index < 5; ++index)
    {
        points[index + 1][0] = ((uint32)((sint32)x_product[index] / 375) + 236);
        points[index + 1][1] = ((uint32)((sint32)y_product[index] / 375) + 308);
    }
    for (index = 1; index < 6; ++index)
    {
        line.color[0] = 64;
        line.color[1] = 64;
        line.color[2] = 64;
        line.semitransparent = 0;
        line.point[0][0] = (uint16)((uint16)radar_points[0].x + slide + 236);
        line.point[0][1] = (uint16)((uint16)radar_points[0].y + 308);
        line.point[1][0] = (uint16)((uint16)radar_points[index].x + slide + 236);
        value = (uint16)radar_points[index].y;
        line.point[1][1] = (uint16)(value + 308);
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line_pair(&line, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    }
    for (index = 0; index < 5; ++index)
    {
        line.point[0][0] = (uint16)((uint16)radar_points[index + 1].x + slide + 236);
        line.point[0][1] = (uint16)((uint16)radar_points[index + 1].y + 308);
        target = (index + 1) % 5;
        line.point[1][0] = (uint16)((uint16)radar_points[target + 1].x + slide + 236);
        value = (uint16)radar_points[target + 1].y;
        line.point[1][1] = (uint16)(value + 308);
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line_pair(&line, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    }
    if (mission_aircraft.flags & 2)
    {
        for (index = 0; index < 3; ++index)
        {
            triangle.color[index][0] = 207;
            triangle.color[index][1] = 139;
            triangle.color[index][2] = 23;
        }
        triangle.color[0][3] = 0;
        for (index = 0; index < 5; ++index)
        {
            triangle.points[0].x = (uint16)(points[0][0] + slide);
            triangle.points[0].y = (uint16)(points[0][1]);
            triangle.points[1].x = (uint16)(points[fan[index]][0] + slide);
            triangle.points[1].y = (uint16)(points[fan[index]][1]);
            triangle.points[2].x = (uint16)(points[fan[index + 1]][0] + slide);
            triangle.points[2].y = (uint16)(points[fan[index + 1]][1]);
            ordering = ui_packets.scene_ot;
            packet = ui_emit_colored_triangle(&triangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        }
    }
    if (mission_aircraft.flags & 1)
    {
        delta = 25 - ui_animation_ticks(1);
        value = 0 - delta * delta * 325;
        slide = ((sint32)value / 625);
        text_select_font(7);
        text_set_blend(0xFFFFFFFF);
        text_set_reveal_range(1, 0, 1);
        text_set_reveal_step(1);
        mode = ui_widget_get(1);
        if (mode == 1)
            text_set_color(225, 95, 10);
        else if (ui_widget_get(1) == 0)
            text_set_color(229, 229, 213);
        text_x = slide + 70;
        text_right = slide + 91;
        for (index = 0; index < 3; ++index)
        {
            text_set_pos(text_x, 258 + index * 11);
            value = number[number_slots[index]];
            quotient = ((sint32)value / 100);
            (wire_word_15 = (quotient), (void)memcpy(argument, &wire_word_15, sizeof(wire_word_15)));
            text_print((const uint8 *)"%2d@", argument, sizeof(argument));
            text_set_pos(text_right, 258 + index * 11);
            (wire_word_16 = (value - quotient * 100), (void)memcpy(argument, &wire_word_16, sizeof(wire_word_16)));
            text_print_digits((const uint8 *)"%2d", argument, sizeof(argument));
        }
        text_set_pos(text_x, 291);
        (wire_word_17 = (number[3]), (void)memcpy(argument, &wire_word_17, sizeof(wire_word_17)));
        text_print((const uint8 *)"%5d\n", argument, sizeof(argument));
        text_set_pos(slide + 60, 302);
        (wire_word_18 = (number[4]), (void)memcpy(argument, &wire_word_18, sizeof(wire_word_18)));
        text_print((const uint8 *)"%5d\n", argument, sizeof(argument));
    }
    mode = ui_widget_get(1);
    value = current->widget6_value;
    ui_widget_set(6, value);
    ui_widget_set(1, 1);
    value = current->widget5_base;
    ui_widget_set(5, value + mode * 3);
    return packet;
}

void menu_geometry_initialize_weapon_packets(void)
{
    uint32 bank, index;
    SPRT *spr;
    DR_MODE *command;
    FUNCTION_MARKER(0x800F8AC4, "ACE3.BIN");
    for (bank = 0; bank < 2; ++bank)
    {
        for (index = 0; index < 2; ++index)
        {
            spr = &mission_aircraft.weapon_packets->primary[bank][index];
            setlen(spr, 4);
            spr->rgbc.code = 102;
            spr->rgbc.r = 128;
            spr->rgbc.g = 128;
            spr->rgbc.b = 128;
            spr->p[0].attr = 2584;
            spr->w = 48;
            spr->h = 88;
            spr = &mission_aircraft.weapon_packets->secondary[bank][index];
            setlen(spr, 4);
            spr->rgbc.code = 102;
            spr->rgbc.r = 128;
            spr->rgbc.g = 128;
            spr->rgbc.b = 128;
            spr->w = 48;
            spr->h = 88;
            spr->p[0].attr = 2648;
            command = &mission_aircraft.weapon_packets->modes[bank];
            SetDrawMode(command, 0, 0, 76, NULL);
        }
    }
    return;
}

uint32 ui_emit_polyline3(const MENU_POLYLINE3_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, command, tag, link, index;
    FUNCTION_MARKER(0x800F6EC0, "ACE3.BIN");
    LINE_F3 *line = ui_packet_span(packet, sizeof(*line));
    line->tag = (line->tag & 0xFFFFFF) | (uint32)5 << 24;
    line->rgbc.code = 0x48;
    line->pad = 0x55555555;
    value = descriptor->semitransparent;
    command = line->rgbc.code;
    line->rgbc.code = (uint8)(value ? command | 2 : command & 0xFD);
    value = descriptor->color[0];
    tag = line->tag;
    line->rgbc.r = (uint8)value;
    value = descriptor->color[1];
    line->rgbc.g = (uint8)value;
    value = descriptor->color[2];
    line->rgbc.b = (uint8)value;
    for (index = 0; index < 12; index += 2)
    {
        value = (index & 2) ? (uint16)descriptor->points[index / 4].y : (uint16)descriptor->points[index / 4].x;
        if (index & 2)
            line->p[index / 4].y = (sint16)(uint16)value;
        else
            line->p[index / 4].x = (sint16)(uint16)value;
    }
    link = *ot_entry;
    line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 24;
}

uint32 ui_emit_polyline3_pair(MENU_POLYLINE3_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 first, third, second;
    FUNCTION_MARKER(0x800F6FB0, "ACE3.BIN");
    packet = ui_emit_polyline3(descriptor, packet, ot_entry);
    first = (uint16)descriptor->points[0].y;
    third = (uint16)descriptor->points[2].y;
    ++first;
    descriptor->points[0].y = (sint16)(uint16)first;
    second = (uint16)descriptor->points[1].y;
    ++third;
    descriptor->points[2].y = (sint16)(uint16)third;
    ++second;
    descriptor->points[1].y = (sint16)(uint16)second;
    return ui_emit_polyline3(descriptor, packet, ot_entry);
}

uint32 ui_emit_polyline4(const MENU_POLYLINE4_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, command, tag, link, index;
    FUNCTION_MARKER(0x800F7014, "ACE3.BIN");
    LINE_F4 *line = ui_packet_span(packet, sizeof(*line));
    line->tag = (line->tag & 0xFFFFFF) | (uint32)6 << 24;
    line->rgbc.code = 0x4C;
    line->pad = 0x55555555;
    value = descriptor->semitransparent;
    command = line->rgbc.code;
    line->rgbc.code = (uint8)(value ? command | 2 : command & 0xFD);
    value = descriptor->color[0];
    tag = line->tag;
    line->rgbc.r = (uint8)value;
    value = descriptor->color[1];
    line->rgbc.g = (uint8)value;
    value = descriptor->color[2];
    line->rgbc.b = (uint8)value;
    for (index = 0; index < 16; index += 2)
    {
        value = (index & 2) ? (uint16)descriptor->points[index / 4].y : (uint16)descriptor->points[index / 4].x;
        if (index & 2)
            line->p[index / 4].y = (sint16)(uint16)value;
        else
            line->p[index / 4].x = (sint16)(uint16)value;
    }
    link = *ot_entry;
    line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 28;
}

uint32 ui_emit_polyline4_pair(MENU_POLYLINE4_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 first, third, second;
    FUNCTION_MARKER(0x800F711C, "ACE3.BIN");
    packet = ui_emit_polyline4(descriptor, packet, ot_entry);
    first = (uint16)descriptor->points[0].y;
    third = (uint16)descriptor->points[2].y;
    ++first;
    descriptor->points[0].y = (sint16)(uint16)first;
    second = (uint16)descriptor->points[1].y;
    ++third;
    descriptor->points[2].y = (sint16)(uint16)third;
    ++second;
    descriptor->points[1].y = (sint16)(uint16)second;
    return ui_emit_polyline4(descriptor, packet, ot_entry);
}

uint32 ui_emit_line_pair(MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 first, second;
    FUNCTION_MARKER(0x800F6E68, "ACE3.BIN");
    packet = ui_emit_line(descriptor, packet, ot_entry);
    first = descriptor->point[0][1];
    second = descriptor->point[1][1];
    ++first;
    ++second;
    descriptor->point[0][1] = (uint16)first;
    descriptor->point[1][1] = (uint16)second;
    return ui_emit_line(descriptor, packet, ot_entry);
}

uint32 ui_emit_colored_triangle(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, group, channel, index, tag, link;
    POLY_G3 *triangle = ui_packet_span(packet, sizeof(*triangle));
    FUNCTION_MARKER(0x800F73A4, "ACE3.BIN");
    triangle->tag = (triangle->tag & 0xFFFFFF) | ((uint32)(uint8)(6) << 24);
    triangle->p[0].rgbc.code = (uint8)(0x30);
    value = descriptor->color[0][3];
    triangle->p[0].rgbc.code = (uint8)(value ? 0x32 : 0x30);
    for (group = 0; group < 3; ++group)
    {
        for (channel = 0; channel < 3; ++channel)
        {
            value = descriptor->color[group][channel];
            ((uint8 *)&triangle->p[group].rgbc)[channel] = (uint8)value;
        }
    }
    for (index = 0; index < 5; ++index)
    {
        value = (index & 1) ? descriptor->points[index / 2].y : descriptor->points[index / 2].x;
        if (index & 1)
            triangle->p[index / 2].xy.y = (sint16)(uint16)value;
        else
            triangle->p[index / 2].xy.x = (sint16)(uint16)value;
    }
    value = descriptor->points[2].y;
    tag = triangle->tag;
    triangle->p[2].xy.y = (sint16)(uint16)value;
    link = *ot_entry;
    triangle->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 28;
}

typedef char menu_polyline_packet_layout_check[sizeof(LINE_F3) == 24 && sizeof(LINE_F4) == 28 && offsetof(LINE_F3, rgbc) == 4 && offsetof(LINE_F4, rgbc) == 4 && offsetof(LINE_F3, p) == 8 && offsetof(LINE_F4, p) == 8 && offsetof(LINE_F3, pad) == 20 && offsetof(LINE_F4, pad) == 24 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 ? 1 : -1];

typedef char menu_polygon_packet_layout_check[sizeof(POLY_G3) == 28 && sizeof(POLY_G4) == 36 && offsetof(POLY_G3, p) == 4 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

uint32 ui_emit_colored_quad(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, vertex, channel, source, destination, tag, link;

    POLY_G4 *quad = ui_packet_span(packet, sizeof(*quad));
    FUNCTION_MARKER(0x800F7244, "ACE3.BIN");
    quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(uint8)(8) << 24);
    quad->p[0].rgbc.code = (uint8)(56);
    value = descriptor->color[0][3];
    quad->p[0].rgbc.code = (uint8)(value ? 58 : 56);
    for (vertex = 0; vertex < 4; ++vertex)
    {
        for (channel = 0; channel < 3; ++channel)
        {
            source = 16 + vertex * 4 + channel;
            destination = 4 + vertex * 8 + channel;
            value = descriptor->color[vertex][channel];
            ((uint8 *)&quad->p[vertex].rgbc)[channel] = (uint8)value;
        }
    }
    for (vertex = 0; vertex < 4; ++vertex)
    {
        source = vertex * 4;
        destination = 8 + vertex * 8;
        value = descriptor->points[vertex].x;
        quad->p[vertex].xy.x = (sint16)(uint16)value;
        value = descriptor->points[vertex].y;
        if (vertex == 3)
            tag = quad->tag;
        quad->p[vertex].xy.y = (sint16)(uint16)value;
    }
    link = *ot_entry;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 36;
}

uint32 ui_emit_configured_draw_mode(const MENU_DRAW_MODE *config, uint32 packet, uint32 ordering)
{
    DR_MODE *draw_mode;
    uint32 display, dither, depth, blend, x, y, page, tag, link;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800F6CBC, "ACE3.BIN");
    display = config->display;
    dither = config->dither;
    depth = config->depth;
    blend = config->blend;
    y = config->texture_y;
    x = config->texture_x;
    page = ((depth & 3) << 7) | ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    draw_mode = (DR_MODE *)ui_packet_span(packet, sizeof(*draw_mode));
    SetDrawMode(draw_mode, display, dither, page, NULL);
    tag = draw_mode->tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    link = *ordering_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 12;
}

uint32 ui_emit_line(const MENU_LINE_DESCRIPTOR *config, uint32 packet, uint32 *ot_entry)
{
    uint32 value, tag, link;
    LINE_F2 *line;
    FUNCTION_MARKER(0x800F6DA4, "ACE3.BIN");
    line = ui_packet_span(packet, sizeof(*line));
    line->tag = (line->tag & 0xFFFFFF) | ((uint32)(3) << 24);
    line->rgbc.code = (uint8)(64);
    value = config->semitransparent;
    line->rgbc.code = (uint8)(value ? 66 : 64);
    value = config->color[0];
    line->rgbc.r = (uint8)(value);
    value = config->color[1];
    line->rgbc.g = (uint8)(value);
    value = config->color[2];
    line->rgbc.b = (uint8)(value);
    value = config->point[0][0];
    line->p[0].x = (sint16)(uint16)(value);
    value = config->point[0][1];
    line->p[0].y = (sint16)(uint16)(value);
    value = config->point[1][0];
    line->p[1].x = (sint16)(uint16)(value);
    value = config->point[1][1];
    tag = line->tag;
    line->p[1].y = (sint16)(uint16)(value);
    link = *ot_entry;
    line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 16;
}
