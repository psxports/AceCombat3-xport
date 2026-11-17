#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

enum
{
    UI_MAP_LINE_COUNT = 32,
    UI_MAP_MISSION_COUNT = 64
};

typedef struct
{
    sint16 line;
    sint16 bit;
} UI_MAP_SEED;

typedef struct
{
    POLY_FT4 tiles[2][273];
    POLY_FT4 pulse[2];
} UI_MAP_GPU_PACKETS;

typedef char ui_map_gpu_payload_layout_check[sizeof(POLY_FT4) == 40 && offsetof(UI_MAP_GPU_PACKETS, pulse) == 21840 && sizeof(UI_MAP_GPU_PACKETS) == 21920 ? 1 : -1];

static struct
{
    uint8 packets_ready;
    uint32 active[UI_MAP_LINE_COUNT];
    uint32 pending[UI_MAP_LINE_COUNT];
    uint16 ticks;
    uint16 phase;
    uint16 seed_line;
    uint16 seed_bit;
} map_state;

static UI_MAP_GPU_PACKETS map_packets;

static const uint32 map_layout[UI_MAP_LINE_COUNT] = {
    0x00000000u, 0x00000000u, 0x000000C0u, 0x000001E0u, 0x000003F0u, 0x00004FE0u, 0x000047F0u, 0x00008FE0u, 0x00001FE0u, 0x00000FC0u, 0x00004FC0u, 0x0000DFC0u, 0x0000FFF0u, 0x0000FFF0u, 0x00007FF8u, 0x0000FFF0u, 0x0000FFF4u, 0x0008FFF4u, 0x000FFFFEu, 0x000FFF03u, 0x0003FF93u, 0x0003FF8Bu, 0x00007F96u, 0x00007FF2u, 0x00001FE8u, 0x00006FC0u, 0x000061C0u, 0x00044F08u, 0x0003071Cu, 0x00035038u, 0x00001810u, 0x00000020u,
};

static const UI_MAP_SEED mission_seed[UI_MAP_MISSION_COUNT] = {
    {5, 5}, {24, 11}, {18, 10}, {14, 13}, {30, 12}, {18, 10}, {11, 9}, {24, 11}, {9, 14}, {5, 5}, {14, 4}, {23, 20}, {15, 8}, {14, 13}, {24, 11}, {21, 7}, {27, 5}, {24, 11}, {24, 11}, {5, 5}, {18, 10}, {31, 10}, {24, 11}, {19, 15}, {5, 5}, {14, 4}, {23, 20}, {23, 20}, {7, 7}, {26, 6}, {30, 7}, {30, 7}, {24, 2}, {21, 7}, {21, 7}, {21, 7}, {27, 12}, {24, 11}, {27, 12}, {27, 12}, {14, 4}, {23, 20}, {23, 20}, {30, 8}, {14, 4}, {24, 11}, {30, 7}, {24, 2}, {21, 7}, {5, 5}, {21, 7}, {25, 4}, {25, 4}, {24, 2}, {5, 5}, {24, 2}, {5, 5}, {5, 5}, {5, 5}, {48, 48}, {5, 5}, {5, 5}, {5, 5}, {5, 5},
};

static const UI_MAP_SEED *ui_map_seed(uint32 mission)
{
    if (mission >= UI_MAP_MISSION_COUNT)
        abort();
    return mission_seed + mission;
}

static uint32 ui_map_layout_line(uint32 line)
{
    if (line >= UI_MAP_LINE_COUNT)
        abort();
    return map_layout[line];
}

void ui_map_reset(void)
{
    memset(&map_state, 0, sizeof(map_state));
}

typedef char ui_map_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && 273 * sizeof(POLY_FT4) == 10920 ? 1 : -1];

void ui_map_initialize_packets(void)
{
    uint32 i, group;
    POLY_FT4 *quad;
    FUNCTION_MARKER(0x800DE79C, "ACE3.BIN");
    for (i = 0; i < 32; ++i)
    {
        map_state.active[i] = 0;
        map_state.pending[i] = 0;
    }
    map_state.ticks = (uint16)(0);
    map_state.phase = (uint16)(0);
    *ui_widget_value(2) = (uint8)(0);
    if (!gpu_register_packet_range(&map_packets, sizeof(map_packets)))
        abort();
    map_state.packets_ready = 0;
    for (group = 0; group < 2; ++group)
    {
        quad = map_packets.tiles[group];
        for (i = 0; i < 273; ++i)
        {
            quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(9) << 24);
            quad->rgbc.code = (uint8)(46);
            quad->rgbc.r = (uint8)(128);
            quad->rgbc.g = (uint8)(128);
            quad->rgbc.b = (uint8)(128);
            quad->p[0].uv.u = (uint8)(240);
            quad->p[0].uv.v = (uint8)(126);
            quad->p[1].uv.u = (uint8)(248);
            quad->p[1].uv.v = (uint8)(126);
            quad->p[2].uv.u = (uint8)(240);
            quad->p[2].uv.v = (uint8)(138);
            quad->p[3].uv.u = (uint8)(248);
            quad->p[3].uv.v = (uint8)(138);
            quad->p[1].attr = (uint16)(71);
            quad->p[0].attr = (uint16)(728);
            quad += 1;
        }
        quad = &map_packets.pulse[group];
        quad->rgbc.code = (uint8)(44);
        quad->p[0].uv.v = (uint8)(27);
        quad->p[1].uv.v = (uint8)(27);
        quad->p[2].uv.v = (uint8)(47);
        quad->p[3].uv.v = (uint8)(47);
        quad->p[1].attr = (uint16)(7);
        quad->p[0].attr = (uint16)(856);
        quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(9) << 24);
        quad->rgbc.r = (uint8)(128);
        quad->rgbc.g = (uint8)(128);
        quad->rgbc.b = (uint8)(128);
        quad->p[0].uv.u = (uint8)(212);
        quad->p[1].uv.u = (uint8)(224);
        quad->p[2].uv.u = (uint8)(212);
        quad->p[3].uv.u = (uint8)(224);
    }
    map_state.packets_ready = 1;
    return;
}

void ui_map_begin(uint32 enabled, uint32 mode)
{
    const UI_MAP_SEED *seed;
    POLY_FT4 *off_grid_quad = NULL;
    uint32 slot, shift, saved_slot, saved_shift, result;

    FUNCTION_MARKER(0x800E11F4, "ACE3.BIN");
    if (!map_state.packets_ready)
        abort();
    map_state.phase = (uint16)(enabled);
    map_state.ticks = (uint16)(mode);
    seed = ui_map_seed((uint32)(sint16)game_runtime.selection.mission);
    slot = (sint32)seed->line;
    shift = (sint32)seed->bit;
    saved_slot = (uint16)seed->line;
    if (slot < UI_MAP_LINE_COUNT)
        result = map_state.active[slot];
    else
    {
        off_grid_quad = &map_packets.tiles[0][1];
        result = off_grid_quad->p[1].xy.xy;
    }
    saved_shift = (uint16)seed->bit;
    result |= (uint32)1 << (shift & 31);
    if (slot < UI_MAP_LINE_COUNT)
        map_state.active[slot] = result;
    else
    {
        /* preserve the off-grid seed's original GPU packet word update */
        off_grid_quad->p[1].xy.xy = result;
    }
    map_state.seed_line = (uint16)(saved_slot);
    map_state.seed_bit = (uint16)(saved_shift);
    {
        (void)(uint32)(result);
        return;
    }
}

void ui_map_tick(void)
{
    uint32 mode, time, value;

    FUNCTION_MARKER(0x800E1260, "ACE3.BIN");
    mode = (sint16)map_state.phase;
    if (mode == 0 || mode == 2)
        return;
    if (mode == 1)
    {
        time = (sint16)map_state.ticks;
        value = map_state.ticks;
        if ((sint32)time < 120)
            map_state.ticks = (uint16)(value + 1);
        else
        {
            map_state.phase = (uint16)(2);
            mission_effect_open_sprs();
        }
    }
    mode = (sint16)map_state.phase;
    if (mode == 3)
    {
        time = (sint16)map_state.ticks;
        value = map_state.ticks;
        if ((sint32)time > 0)
            map_state.ticks = (uint16)(value - 1);
        else
            map_state.phase = (uint16)(0);
    }
    time = (sint16)map_state.ticks;
    if ((sint32)time >= 31)
    {
        *ui_widget_value(2) = (uint8)(1);
        return;
    }
    *ui_widget_value(2) = (uint8)(0);
    return;
}

void ui_map_spread(void)
{
    uint32 row, column, active, previous, current, next, bit, adjacent, value;

    FUNCTION_MARKER(0x800E133C, "ACE3.BIN");
    if ((sint16)map_state.phase != 1)
        return;
    for (row = 0; row < 32; ++row)
    {
        active = map_state.active[row];
        previous = row ? map_state.pending[row - 1] : 0;
        current = map_state.pending[row];
        next = row < 31 ? map_state.pending[row + 1] : 0;
        for (column = 0; column < 32; ++column)
        {
            bit = (uint32)1 << column;
            if (!(active & bit))
                continue;
            if (column > 0)
                current |= bit >> 1;
            if (column < 31)
                current |= bit << 1;
            adjacent = column == 0 ? 3 : column == 31 ? 0xC0000000 : (bit >> 1) | bit | (bit << 1);
            if (row & 1)
            {
                if (row < 31)
                    next |= bit;
                if (row > 0)
                    previous |= adjacent;
            }
            else
            {
                if (row < 31)
                    next |= adjacent;
                if (row > 0)
                    previous |= bit;
            }
        }
        if (row)
            map_state.pending[row - 1] = previous;
        map_state.pending[row] = current;
        if (row < 31)
            map_state.pending[row + 1] = next;
    }
    for (row = 0; row < 32; ++row)
    {
        active = map_state.active[row];
        value = map_state.pending[row];
        map_state.active[row] = active | value;
        map_state.pending[row] = 0;
    }
    return;
}

void ui_map_render(void)
{
    UI_CONTROL clip_control;
    uint32 bank, cursor, depth, ordering, tag, link;
    uint32 corner, x, y, value, time, packet, row, column, layout, visible, bit;

    POLY_FT4 *quad, *tile;
    uint32 *entry;
    FUNCTION_MARKER(0x800E14D0, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    if (!map_state.packets_ready || bank >= 2)
        abort();
    quad = &map_packets.pulse[bank];
    cursor = gpu_packet_link((uintptr_t)quad);
    for (corner = 0; corner < 4; ++corner)
    {
        value = (sint16)map_state.seed_line;
        x = value * 5 + ((corner & 1) ? 162 : 150);
        quad->p[corner].xy.x = (sint16)(uint16)(x);
        value = (sint16)map_state.seed_bit;
        y = (sint16)map_state.seed_line;
        y = ((sint32)(y + 1) % 2);
        y = value * 10 + y * 5 + (corner < 2 ? 190 : 210);
        quad->p[corner].xy.y = (sint16)(uint16)(y);
    }
    value = ((sint32)game_runtime.field_counter % 128);
    value = 64 - value;
    if ((sint32)value < 0)
        value = 0 - value;
    value *= 2;
    quad->rgbc.r = (uint8)(value);
    quad->rgbc.g = (uint8)(value);
    quad->rgbc.b = (uint8)(value);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    ordering = ui_packets.scene_ot;
    entry = ui_scene_ot_span(ordering + depth * 4, sizeof(*entry));
    tag = quad->tag;
    link = *entry;
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    ordering = ui_packets.scene_ot;
    entry = ui_scene_ot_span(ordering + depth * 4, sizeof(*entry));
    link = *entry;
    *entry = (link & 0xFF000000) | (cursor & 0xFFFFFF);
    bank = game_runtime.display_buffer;
    time = (sint16)map_state.ticks;
    tile = map_packets.tiles[bank];
    packet = gpu_packet_link((uintptr_t)tile);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    ordering = ui_packets.scene_ot + depth * 4;
    entry = ui_scene_ot_span(ordering, sizeof(*entry));
    for (row = 0; row < 32; ++row)
    {
        layout = ui_map_layout_line(row);
        visible = map_state.active[row];
        for (column = 0; column < 32; ++column)
        {
            bit = (uint32)1 << column;
            if (!(layout & bit) || !(visible & bit))
                continue;
            y = 190 + ((row + 1) % 2) * 5 + column * 10;
            tile->p[2].xy.y = (sint16)(uint16)(y + 12);
            tile->p[3].xy.y = (sint16)(uint16)(y + 12);
            tile->p[0].xy.x = (sint16)(uint16)(150 + row * 5);
            tile->p[0].xy.y = (sint16)(uint16)(y);
            tile->p[1].xy.x = (sint16)(uint16)(158 + row * 5);
            tile->p[1].xy.y = (sint16)(uint16)(y);
            tile->p[2].xy.x = (sint16)(uint16)(150 + row * 5);
            tile->p[3].xy.x = (sint16)(uint16)(158 + row * 5);
            if ((sint32)time >= 31)
            {
                value = 128 - (uint32)((sint32)((time - 30) << 7) / 90);
                tile->rgbc.r = (uint8)(value);
                tile->rgbc.g = (uint8)(value);
                tile->rgbc.b = (uint8)(value);
            }
            tag = tile->tag;
            link = *entry;
            tile->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            link = *entry;
            *entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
            packet += 40;
            tile += 1;
        }
    }
    return;
}

void ui_map_spread_frame(void)
{
    FUNCTION_MARKER(0x800E1334, "ACE3.BIN");
    {
        (void)(ui_map_spread());
        return;
    }
}

void ui_map_draw_grid(void)
{
    uint32 count, mode, index, value;

    FUNCTION_MARKER(0x800E18B8, "ACE3.BIN");
    count = ui_animation_ticks(2);
    mode = ui_animation_phase(2);
    if (mode == 2)
    {
        for (index = 0; index < 17; ++index)
            ui_draw_panel_tile(168, 258 + index * 9, 140, 2, 0);
        ui_draw_panel_tile(232, 160, 75, 2, 0);
        for (index = 0; index < 16; ++index)
            ui_draw_panel_line(232 + index * 5, 160, 232 + index * 5, 402);
        {
            (void)(ui_draw_panel_line(168, 258, 168, 402));
            return;
        }
    }
    if (mode != 1 && mode != 3)
    {
        (void)(uint32)((sint32)mode < 2 ? 1 : 3);
        return;
    }
    for (index = 0; index < 17 && (sint32)count >= (sint32)index; ++index)
    {
        value = (count - index) * 7;
        if ((sint32)value >= 141)
            value = 140;
        ui_draw_panel_tile(168, 258 + index * 9, (uint32)(sint32)(sint16)value, 2, 0);
    }
    value = ((sint32)(count * 75) / 17);
    if ((sint32)value >= 76)
        value = 75;
    ui_draw_panel_tile(232, 160, (uint32)(sint32)(sint16)value, 2, 0);
    for (index = 0; index < 16 && (sint32)count >= (sint32)index; ++index)
    {
        value = ((sint32)((count - index) * 242) / 20);
        if ((sint32)value >= 243)
            value = 242;
        ui_draw_panel_line(232 + index * 5, 160, 232 + index * 5, (uint32)(sint32)(sint16)(value + 160));
    }
    value = ((sint32)(count * 144) / 17);
    if ((sint32)value >= 145)
        value = 144;
    {
        (void)(ui_draw_panel_line(168, 258, 168, (uint32)(sint32)(sint16)(value + 258)));
        return;
    }
}

void ui_map_tick_frame(void)
{
    FUNCTION_MARKER(0x800E1258, "ACE3.BIN");
    {
        ui_map_tick();
        return;
    }
}
