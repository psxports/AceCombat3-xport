#include "game_functions.h"

typedef struct
{
    uint16 angle;
    uint16 speed;
} MENU_ORBIT;

typedef struct
{
    sint16 x;
    sint16 y;
} MENU_ORBIT_CENTER;

static MENU_ORBIT orbits[4];

static const MENU_ORBIT_CENTER orbit_centers[4] = {{48, 131}, {86, 88}, {124, 131}, {162, 88}};

void menu_orbits_reset(void)
{
    uint32 i;
    for (i = 0; i < 4; ++i)
    {
        orbits[i].angle = 3584;
        orbits[i].speed = 0;
    }
}

typedef char menu_orbit_packet_layout[sizeof(POLY_F4) == 24 && offsetof(POLY_F4, tag) == 0 && offsetof(POLY_F4, rgbc) == 4 && offsetof(POLY_F4, p) == 8 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 ? 1 : -1];

void menu_orbits_render(void)
{
    UI_CONTROL clip_control;
    uint32 ordering, group, packet, value, other;
    uint32 phase, vertex, product, tag, link, layer;
    sint32 current, offset_x, offset_y, origin_x, origin_y, angle;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800E1B48, "ACE3.BIN");
    ordering = ui_packets.scene_ot;
    for (group = 0; group < 4; ++group)
    {
        value = mission_flow.root_variant;
        if (group == value)
        {
            value = orbits[group].speed;
            if (value < 128)
                orbits[group].speed = (uint16)(value + 1);
        }
        else
        {
            current = (sint16)orbits[group].speed;
            value = orbits[group].speed;
            if (current > 0)
                orbits[group].speed = (uint16)(value - 1);
            current = (sint16)orbits[group].speed;
            if (current == 5)
            {
                value = orbits[group].angle;
                orbits[group].angle = (uint16)(value - 32);
            }
            if ((sint16)orbits[group].speed < 5)
                orbits[group].speed = (uint16)(0);
        }
        value = orbits[group].angle;
        other = orbits[group].speed;
        orbits[group].angle = (uint16)(value + other);
        packet = ui_packets.cursor;
        ui_packets.cursor = (uint32)(packet + 24);
        if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 24)
            abort();
        POLY_F4 *poly = (void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
        ((uint8 *)&poly->tag)[3] = 5;
        poly->rgbc.code = 40;
        poly->rgbc.r = 0;
        poly->rgbc.g = 0;
        poly->rgbc.b = 0;
        phase = (sint16)orbits[group].angle;
        offset_x = orbit_centers[group].x;
        offset_y = orbit_centers[group].y;
        origin_x = (sint16)ui_item_at(0, 1)->x;
        origin_y = (sint16)ui_item_at(0, 1)->y;
        for (vertex = 0; vertex < 4; ++vertex)
        {
            angle = (phase + ((vertex & 1) ? 128 : 0 - (uint32)128));
            product = (uint32)rcos(angle) * (vertex < 2 ? 22 : 18);
            value = (uint32)((sint32)product / 4096) + (uint32)offset_x + (uint32)origin_x;
            poly->p[vertex].x = (sint16)(uint16)value;
            product = (uint32)rsin(angle) * (vertex < 2 ? 40 : 29);
            value = (uint32)((sint32)product / 4096) + (uint32)offset_y + (uint32)origin_y;
            if (vertex == 3)
            {
            }
            poly->p[vertex].y = (sint16)(uint16)value;
        }
        ui_control_read(&clip_control);
        layer = (uint32)clip_control.layer;
        tag = poly->tag;
        ordering_entry = ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
        link = *ordering_entry;
        poly->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ui_control_read(&clip_control);
        layer = (uint32)clip_control.layer;
        ordering_entry = ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
        tag = *ordering_entry;
        *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    }
    return;
}
