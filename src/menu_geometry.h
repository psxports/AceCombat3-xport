#ifndef MENU_GEOMETRY_H
#define MENU_GEOMETRY_H

#include "psx.h"

typedef struct
{
    XY points[3];
    uint8 color[3];
    uint8 semitransparent;
} MENU_POLYLINE3_DESCRIPTOR;

typedef struct
{
    XY points[4];
    uint8 color[3];
    uint8 semitransparent;
} MENU_POLYLINE4_DESCRIPTOR;

typedef struct
{
    uint16 point[2][2];
    uint8 color[3];
    uint8 semitransparent;
} MENU_LINE_DESCRIPTOR;

typedef struct
{
    uint8 display, dither, depth, blend;
    uint16 texture_x, texture_y;
} MENU_DRAW_MODE;

typedef struct
{
    uint16 x;
    uint16 y;
} MENU_GEOMETRY_POINT;

typedef struct
{
    MENU_GEOMETRY_POINT points[4];
    uint8 color[4][4];
} MENU_POLYGON_DESCRIPTOR;

typedef struct
{
    uint16 x, y, width, height;
    uint8 red, green, blue, semi;
} MENU_FILL_DESCRIPTOR;

typedef struct
{
    MENU_GEOMETRY_POINT points[4];
    uint8 color[3];
    uint8 semitransparent, blend;
    uint16 texture_x, texture_y, width, height;
    uint16 palette_x, palette_y;
} MENU_TEXTURED_QUAD_DESCRIPTOR;

void menu_geometry_reset(void);
uint32 menu_calibration_begin(void);
void menu_calibration_negcon_frame(void);
void menu_calibration_joystick_frame(void);
uint32 menu_brightness_frame(void);

uint32 ui_emit_polyline3(const MENU_POLYLINE3_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 ui_emit_polyline3_pair(MENU_POLYLINE3_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 ui_emit_polyline4(const MENU_POLYLINE4_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 ui_emit_polyline4_pair(MENU_POLYLINE4_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);

uint32 title_geometry_emit_line(const MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 title_geometry_emit_double_line(MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 ordering);
uint32 title_geometry_emit_fill(const MENU_FILL_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 title_geometry_emit_gouraud_quad(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 title_geometry_emit_gouraud_triangle(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 title_geometry_emit_textured_quad(const MENU_TEXTURED_QUAD_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 title_geometry_draw_rotating_marker(uint32 x, uint32 y, uint32 angle, uint32 packet);
uint32 title_geometry_draw_brightness(void);

uint32 mission_save_draw_rotating_marker(uint32 x, uint32 y, uint32 ordering, uint32 angle, uint32 packet);

uint32 menu_geometry_aircraft_stats_frame(uint32 packet);
uint32 menu_geometry_draw_aircraft_stats(uint32 packet);
uint32 menu_geometry_draw_weapons(uint32 packet, uint32 ordering);

void menu_geometry_initialize_weapon_packets(void);

uint32 ui_emit_line_pair(MENU_LINE_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);
uint32 ui_emit_colored_triangle(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);

uint32 ui_emit_colored_quad(const MENU_POLYGON_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry);

uint32 ui_emit_configured_draw_mode(const MENU_DRAW_MODE *config, uint32 packet, uint32 ordering);
uint32 ui_emit_line(const MENU_LINE_DESCRIPTOR *config, uint32 packet, uint32 *ot_entry);

#endif
