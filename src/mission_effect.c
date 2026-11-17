#include "game_functions.h"
#include "psx_gte.h"
#include <stdlib.h>
#include <string.h>

typedef char mission_effect_output_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 ? 1 : -1];

MISSION_EFFECT mission_effect;

static MISSION_EFFECT_ORDERING_TABLE effect_ordering_tables[2];

uint32 *mission_effect_native_ot_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        MISSION_EFFECT_ORDERING_TABLE *table = mission_effect.ot_data[index];
        uint32 base = mission_effect.ot[index];

        if (table && bytes && address >= base && !((address - base) & 3) && address - base <= sizeof(*table) && bytes <= sizeof(*table) - (address - base))
            return table->entries + (address - base) / sizeof(uint32);
    }
    return NULL;
}

typedef struct
{
    uint32 prefix;
    XY screen[2][MISSION_EFFECT_RING_VERTEX_COUNT];
    uint32 depth[2][MISSION_EFFECT_RING_VERTEX_COUNT];
    uint32 fog[2][MISSION_EFFECT_RING_VERTEX_COUNT];
} MISSION_RING_PUBLICATION;

typedef char mission_ring_publication_layout[MISSION_EFFECT_RING_VERTEX_COUNT == 16 && sizeof(MISSION_RING_PUBLICATION) == 388 && offsetof(MISSION_RING_PUBLICATION, screen) == 4 && offsetof(MISSION_RING_PUBLICATION, depth) == 132 && offsetof(MISSION_RING_PUBLICATION, fog) == 260 && sizeof(XY) == 4 ? 1 : -1];

typedef union
{
    MATRIX matrix;
    uint32 words[8];
} MISSION_ROTATION_PUBLICATION;

typedef char mission_rotation_publication_layout[sizeof(MISSION_ROTATION_PUBLICATION) == 32 && sizeof(MATRIX) == 32 && sizeof(((MATRIX *)0)->m) == 18 && sizeof(((MATRIX *)0)->m[0]) == 6 && offsetof(MATRIX, m) == 0 && offsetof(MATRIX, t) == 20 ? 1 : -1];

typedef union
{
    struct
    {
        MISSION_RING_PUBLICATION projected;
        uint8 before_angle[16];
        uint16 angle[3], angle_pad;
        uint8 before_offset[16];
        uint32 anchor_offset[3], offset_pad;
        MISSION_ROTATION_PUBLICATION pose;
        MATRIX view;
    } ring;

    struct
    {
        MATRIX pose;
        uint8 before_view[32];
        MATRIX view;
        XY current[MISSION_EFFECT_TRAIL_VERTEX_COUNT];
        XY previous[MISSION_EFFECT_TRAIL_VERTEX_COUNT];
    } trail;

    struct
    {
        uint32 bucket;
        MATRIX combined, view, pose, ring_pose;
        uint8 before_screen[28];
        XY screen;
    } sprite;

    struct
    {
        uint8 before_view[196];
        MATRIX view;
    } trail_view;
} MISSION_EFFECT_SCRATCH_PUBLICATION;

typedef char mission_effect_scratch_publication_layout[MISSION_EFFECT_TRAIL_VERTEX_COUNT == 5 && sizeof(MISSION_EFFECT_SCRATCH_PUBLICATION) == 508 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, ring.angle) == 404 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, ring.anchor_offset) == 428 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, ring.pose) == 444 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, ring.view) == 476 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, trail.view) == 64 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, trail.current) == 96 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, trail.previous) == 116 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, sprite.combined) == 4 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, sprite.view) == 36 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, sprite.pose) == 68 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, sprite.ring_pose) == 100 && offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, sprite.screen) == 160 &&
                                                               offsetof(MISSION_EFFECT_SCRATCH_PUBLICATION, trail_view.view.t) == 216
                                                           ? 1
                                                           : -1];

static void mission_effect_set_translation(MATRIX *matrix, VECTOR *offset);
static void mission_effect_compose_matrix(const MATRIX *left, const MATRIX *right, MATRIX *output);
static void mission_effect_apply_matrix(const MATRIX *matrix, const VECTOR *vector, VECTOR *output);

static const PSX_RECT mission_effect_pal_rect = {1008, 209, 16, 1};
static const SVECTOR mission_effect_trail_step = {0, 0, 512, 0};

static uint32 mission_effect_draw_trail_layer(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet);
static void mission_effect_update_trail_view(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch);
static void mission_effect_init_ring_data(void);
static void mission_effect_link_ot(uint32 ordering);
static uint32 mission_effect_draw_trail_strip(const XY *current, const XY *previous, uint32 item, uint32 packet);
static uint32 mission_effect_set_ring_mode(uint32 mode);
static void mission_effect_rotation_gpf(const SVECTOR *angles, MISSION_ROTATION_PUBLICATION *published);
static void mission_effect_update_ring(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch);
static uint32 mission_effect_draw_ring_dense(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet);
static uint32 mission_effect_draw_ring_sparse(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet);
static uint32 mission_effect_draw_ring(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet);
static void mission_effect_init_sprs(void);
static uint32 mission_effect_draw_sprs(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet);
static void mission_effect_init_trails(void);
static void mission_effect_reset_trail(uint32 idx);
static uint32 mission_effect_trail_visible(const VECTOR *pos);
static void mission_effect_project_trail(const SVECTOR *vertices, MATRIX *matrix, XY *screen, uint32 count);

static uint32 *mission_effect_ot_span(uint32 address, size_t bytes)
{
    uint32 *table = mission_effect_native_ot_span(address, bytes);

    if (table)
        return table;
    return mission_ot_span(address, bytes);
}

static void mission_effect_init_ring_data(void)
{
    uint32 buffer, i, distance, product;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800FF390, "ACE3.BIN");
    mission_effect.ring_phase = 0;
    mission_effect.ring_scroll = 0;
    mission_effect.ring_remainder = 0;
    /* Fix mission-effect-uninitialized-step before SPR can be enabled */
    mission_effect.ring_step = 0;
    mission_effect.frame_counter = 0;
    rectangle = mission_effect_pal_rect;
    mission_effect.tpage = 15;
    mission_effect.clut = 13439;
    StoreImage(&rectangle, (uint32 *)mission_effect.pal);
    if (!gpu_register_packet_range(effect_ordering_tables, sizeof(effect_ordering_tables)))
        abort();
    for (i = 0; i < 2; ++i)
    {
        (void)arena_alloc(1024);
        buffer = gpu_packet_link((uintptr_t)&effect_ordering_tables[i]);
        mission_effect.ot[i] = buffer;
        mission_effect.ot_data[i] = &effect_ordering_tables[i];
    }
    distance = 0;
    for (i = 0; i < MISSION_EFFECT_RING_HISTORY; ++i)
    {
        mission_effect.ring_angle[i].vx = 0;
        mission_effect.ring_angle[i].vy = 0;
        mission_effect.ring_angle[i].vz = 0;
        mission_effect.ring_angle[i].pad = 0;
        mission_effect.ring_pos[i].vx = 0;
        mission_effect.ring_pos[i].vy = 0;
        product = distance << 3;
        if ((sint32)product < 0)
            product += 63;
        mission_effect.ring_pos[i].vz = (sint32)(23976 - (uint32)((sint32)product >> 6));
        distance += 3125;
    }
    return;
}

static uint32 mission_effect_set_ring_mode(uint32 mode)
{
    uint32 voice, percent, offset, result;

    FUNCTION_MARKER(0x800FF1A4, "ACE3.BIN");
    mission_effect.layers[MISSION_EFFECT_RING].mode = mode;
    mission_effect.layers[MISSION_EFFECT_RING].elapsed = 0;
    if (mode == 1)
    {
        sound_play(98);
        percent = 100;
    }
    else if (mode == 3)
    {
        sound_play(99);
        percent = 0;
    }
    else
        return 3;
    result = 0;
    for (offset = 304; offset <= 320; offset += 4)
    {
        voice = *menu_voice_slot((offset - 304) / 4);
        result = sound_set_voice_fade(voice, percent, 120);
    }
    return result;
}

void mission_effect_init(void)
{
    uint32 i;
    sint32 component;

    FUNCTION_MARKER(0x80101EE4, "ACE3.BIN");
    /* preserve offsets of the following arena allocations */
    arena_alloc(272);
    mission_effect.view_angles = (SVECTOR){0};
    for (i = 0; i < MISSION_EFFECT_RING_VERTEX_COUNT; ++i)
    {
        component = rcos(i << 8);
        mission_effect.ring_vertex[i].vx = (sint16)((uint32)(component / 2));
        component = rsin(i << 8);
        mission_effect.ring_vertex[i].vy = (sint16)((uint32)(component / 2));
        mission_effect.ring_vertex[i].vz = 0;
    }
    mission_effect.frame_counter = 0;
    mission_effect.layers[MISSION_EFFECT_RING].mode = 0;
    mission_effect.layers[MISSION_EFFECT_SPR].mode = 0;
    mission_effect.layers[MISSION_EFFECT_TRAIL].mode = 0;
    mission_effect_init_ring_data();
    mission_effect_init_sprs();
    {
        (void)(mission_effect_init_trails());
        return;
    }
}

void mission_effect_open_ring(void)
{
    FUNCTION_MARKER(0x800FF2BC, "ACE3.BIN");
    sound_play(98);
    mission_effect.layers[MISSION_EFFECT_RING].mode = 1;
    mission_effect.layers[MISSION_EFFECT_RING].elapsed = 0;
    return;
}

uint32 mission_effect_set_ring(uint32 mode)
{
    FUNCTION_MARKER(0x800FF19C, "ACE3.BIN");
    return mission_effect_set_ring_mode(mode);
}

void mission_effect_open_sprs(void)
{
    FUNCTION_MARKER(0x8010097C, "ACE3.BIN");
    mission_effect.layers[MISSION_EFFECT_SPR].mode = 1;
    mission_effect.layers[MISSION_EFFECT_SPR].elapsed = 0;
    return;
}

void mission_effect_close_sprs(void)
{
    FUNCTION_MARKER(0x80100994, "ACE3.BIN");
    mission_effect.layers[MISSION_EFFECT_SPR].mode = 3;
    mission_effect.layers[MISSION_EFFECT_SPR].elapsed = 0;
    return;
}

void mission_effect_open_trails(void)
{
    FUNCTION_MARKER(0x80100FAC, "ACE3.BIN");
    mission_effect.layers[MISSION_EFFECT_TRAIL].mode = 1;
    mission_effect.layers[MISSION_EFFECT_TRAIL].elapsed = 0;
    return;
}

uint32 mission_effect_close_trails(void)
{
    FUNCTION_MARKER(0x80100FC4, "ACE3.BIN");
    mission_effect.layers[MISSION_EFFECT_TRAIL].mode = 3;
    mission_effect.layers[MISSION_EFFECT_TRAIL].elapsed = 0;
    return 3;
}

uint32 mission_effect_draw(uint32 packet)
{
    uint32 first, second, value;

    FUNCTION_MARKER(0x80101FEC, "ACE3.BIN");
    MISSION_EFFECT_SCRATCH_PUBLICATION *scratch = (MISSION_EFFECT_SCRATCH_PUBLICATION *)getScratchAddr(0);
    xport_gte_write_control(24, 0x00A00000);
    xport_gte_write_control(25, 0x00F00000);
    xport_gte_write_control(26, 250);
    xport_gte_write_control(27, 0xFFFFD450);
    xport_gte_write_control(28, 0x01555000);
    first = mission_effect.layers[MISSION_EFFECT_RING].mode;
    second = mission_effect.layers[MISSION_EFFECT_SPR].mode;
    if (first)
        packet = mission_effect_draw_ring(scratch, packet);
    if (second)
        packet = mission_effect_draw_sprs(scratch, packet);
    if (!first && !second)
    {
        value = mission_effect.layers[MISSION_EFFECT_TRAIL].mode;
        if (value)
            packet = mission_effect_draw_trail_layer(scratch, packet);
    }
    value = mission_effect.frame_counter;
    mission_effect.frame_counter = value + 1;
    return packet;
}

static uint32 mission_effect_draw_trails(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 row, item, component, pointer, value, random;
    SVECTOR *angles;
    VECTOR *pos;
    MATRIX *pose = &scratch->trail.pose, *view = &scratch->trail.view;
    XY *current = scratch->trail.current;
    XY *previous = scratch->trail.previous;

    FUNCTION_MARKER(0x80101AD4, "ACE3.BIN");
    pointer = mission_effect.cur_ot;
    packet = ui_emit_draw_mode(0, pointer, packet);
    for (row = 0; row < MISSION_EFFECT_TRAIL_COUNT; ++row)
    {
        angles = mission_effect.trails[row].angle;
        pos = mission_effect.trails[row].pos;
        for (item = MISSION_EFFECT_TRAIL_HISTORY - 1; item > 0; --item)
        {
            pos[item].vx = pos[item - 1].vx;
            pos[item].vy = pos[item - 1].vy;
            pos[item].vz = pos[item - 1].vz;
            angles[item].vx = angles[item - 1].vx;
            angles[item].vy = angles[item - 1].vy;
            angles[item].vz = angles[item - 1].vz;
        }
        random = game_random();
        value = (uint16)angles[0].vx;
        angles[0].vx = (sint16)(value + (random & 31));
        random = game_random();
        value = (uint16)angles[0].vy;
        angles[0].vy = (sint16)(value + (random & 31));
        random = game_random();
        value = (uint16)angles[0].vz;
        angles[0].vz = (sint16)(value + (random & 31));
        RotMatrix(angles, pose);
        TransMatrix(pose, pos + 1);
        SetRotMatrix(pose);
        SetTransMatrix(pose);
        gte_ldv0(&mission_effect_trail_step);
        xport_gte_execute(0x480012);
        pos[0].vx = (sint32)xport_gte_read_data(25);
        pos[0].vy = (sint32)xport_gte_read_data(26);
        pos[0].vz = (sint32)xport_gte_read_data(27);
        if (!mission_effect_trail_visible(pos))
        {
            mission_effect_reset_trail(row);
            continue;
        }
        for (item = 0; item < MISSION_EFFECT_TRAIL_HISTORY; ++item)
        {
            RotMatrix(angles + item, pose);
            TransMatrix(pose, pos + item);
            mission_effect_compose_matrix(&mission_effect.view_matrix, pose, view);
            mission_effect_project_trail(mission_effect.trail_vertex, view, current, MISSION_EFFECT_TRAIL_VERTEX_COUNT);
            if (item)
                packet = mission_effect_draw_trail_strip(current, previous, item - 1, packet);
            for (component = 0; component < MISSION_EFFECT_TRAIL_VERTEX_COUNT; ++component)
            {
                previous[component] = current[component];
            }
        }
    }
    pointer = mission_effect.cur_ot;
    return ui_emit_draw_mode(1, pointer + 1020, packet);
}

static void mission_effect_init_trails(void)
{
    uint32 i;
    sint32 angle, cosine, sine;

    FUNCTION_MARKER(0x80101570, "ACE3.BIN");
    for (i = 0; i < MISSION_EFFECT_TRAIL_COUNT; ++i)
        mission_effect_reset_trail(i);
    mission_effect.trail_vertex[0].vx = 0;
    mission_effect.trail_vertex[0].vy = 0;
    mission_effect.trail_vertex[0].vz = 0;
    mission_effect.trail_vertex[0].pad = 0;
    angle = -512;
    for (i = 1; i < MISSION_EFFECT_TRAIL_VERTEX_COUNT; ++i)
    {
        cosine = rcos(angle);
        sine = rsin(angle);
        mission_effect.trail_vertex[i].vx = (sint16)(cosine / 32);
        mission_effect.trail_vertex[i].vy = (sint16)(sine / 32);
        mission_effect.trail_vertex[i].vz = 0;
        mission_effect.trail_vertex[i].pad = 0;
        angle += 1024;
    }
    return;
}

static void mission_effect_reset_trail(uint32 idx)
{
    uint32 depth, direction, value, i, position;
    sint32 width, height, lateral;
    SVECTOR *angles;
    VECTOR *pos;

    FUNCTION_MARKER(0x80100FDC, "ACE3.BIN");
    angles = mission_effect.trails[idx].angle;
    pos = mission_effect.trails[idx].pos;
    depth = game_random() & 0x3FFF;
    width = (sint32)(depth * 160) / 250;
    height = (sint32)(depth * 240) / 250;
    direction = game_random() & 3;
    if (direction == 0 || direction == 1)
    {
        position = direction == 0 ? (uint32)(sint32)(sint16)width : 0 - (uint32)(sint32)(sint16)width;
        lateral = (sint32)game_random() % 480 - 240;
        pos[0].vx = (sint32)position;
        value = (uint32)lateral * depth;
        pos[0].vy = (sint32)((uint32)((sint32)value / 250));
        pos[0].vz = (sint32)depth;
        value = game_random();
        angles[0].vx = (sint16)(value & 0x1FF);
        value = game_random();
        angles[0].vy = (sint16)(value & (direction == 0 ? 0xDFF : 0x5FF));
    }
    else
    {
        lateral = (sint32)game_random() % 320 - 160;
        value = (uint32)lateral * depth;
        position = ((sint32)value / 250);
        pos[0].vx = (sint32)position;
        position = direction == 2 ? (uint32)(sint32)(sint16)height : 0 - (uint32)(sint32)(sint16)height;
        pos[0].vy = (sint32)position;
        pos[0].vz = (sint32)depth;
        value = game_random();
        angles[0].vx = (sint16)(value & (direction == 2 ? 0xDFF : 0x5FF));
        value = game_random();
        angles[0].vy = (sint16)(value & 0xFFF);
    }
    angles[0].vz = 0;
    for (i = 1; i < MISSION_EFFECT_TRAIL_HISTORY; ++i)
    {
        angles[i].vx = angles[i - 1].vx;
        angles[i].vy = angles[i - 1].vy;
        angles[i].vz = angles[i - 1].vz;
        pos[i].vx = pos[i - 1].vx;
        pos[i].vy = pos[i - 1].vy;
        pos[i].vz = pos[i - 1].vz;
    }
}

static uint32 mission_effect_trail_visible(const VECTOR *pos)
{
    uint32 index, visible;
    sint32 depth, x, y, width, height;

    FUNCTION_MARKER(0x80101A0C, "ACE3.BIN");
    visible = 0;
    for (index = 0; index < MISSION_EFFECT_TRAIL_HISTORY; ++index)
    {
        depth = pos[index].vz;
        y = pos[index].vy;
        x = pos[index].vx;
        if (depth > 32768 || depth < 4096)
            continue;
        width = depth * 160 / 250;
        height = depth * 240 / 250;
        if (x >= -width && x <= width && y >= -height && y <= height)
            visible = 1;
    }
    return visible;
}

static void mission_effect_project_trail(const SVECTOR *vertices, MATRIX *matrix, XY *screen, uint32 count)
{
    uint32 index;

    FUNCTION_MARKER(0x801017A8, "ACE3.BIN");
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    index = 0;
    for (;;)
    {
        gte_ldv0(vertices);
        ++vertices;
        ++index;
        if ((sint32)index >= (sint32)count)
        {
            xport_gte_execute(0x280030);
            screen[index - 1].xy = xport_gte_read_data(12);
            return;
        }
        gte_ldv1(vertices);
        ++vertices;
        ++index;
        if ((sint32)index >= (sint32)count)
        {
            xport_gte_execute(0x280030);
            screen[index - 2].xy = xport_gte_read_data(12);
            screen[index - 1].xy = xport_gte_read_data(13);
            return;
        }
        gte_ldv2(vertices);
        ++vertices;
        xport_gte_execute(0x280030);
        ++index;
        screen[index - 3].xy = xport_gte_read_data(12);
        screen[index - 2].xy = xport_gte_read_data(13);
        screen[index - 1].xy = xport_gte_read_data(14);
        if ((sint32)index >= (sint32)count)
            return;
    }
}

static void mission_effect_init_sprs(void)
{
    uint32 i, value, product;
    sint32 cosine, sine, random;

    FUNCTION_MARKER(0x801009AC, "ACE3.BIN");
    for (i = 0; i < MISSION_EFFECT_SPRITE_COUNT; ++i)
    {
        random = game_random();
        mission_effect.spr[i].phase = (sint16)((uint32)(random % 64));
        value = game_random();
        cosine = rcos(value & 0xFFF);
        random = game_random();
        product = (uint32)cosine * (uint32)(random % 1536);
        mission_effect.spr[i].pos.vx = (sint32)((uint32)((sint32)product / 4096));
        value = game_random();
        sine = rsin(value & 0xFFF);
        random = game_random();
        product = (uint32)sine * (uint32)(random % 1536);
        mission_effect.spr[i].pos.vy = (sint32)((uint32)((sint32)product / 4096));
        random = game_random();
        mission_effect.spr[i].pos.vz = (sint32)((uint32)(random % 1562));
        value = mission_effect.spr[i].pos.vx;
        value = (sint32)value < 0 ? value - 384 : value + 384;
        mission_effect.spr[i].pos.vx = (sint32)value;
        value = mission_effect.spr[i].pos.vy;
        value = (sint32)value < 0 ? value - 384 : value + 384;
        mission_effect.spr[i].pos.vy = (sint32)value;
    }
}

static uint32 mission_effect_draw_sprs(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 ordering, index, value, mode, depth, size;
    uint32 bucket, target, tag;
    MATRIX *sprite_pose = &scratch->sprite.pose, *ring_pose = &scratch->sprite.ring_pose;
    MATRIX *combined = &scratch->sprite.combined, *view = &scratch->sprite.view;
    XY *screen = &scratch->sprite.screen;
    sint32 step, number, counter, x, y, right, bottom;

    FUNCTION_MARKER(0x80100B94, "ACE3.BIN");
    step = (sint16)mission_effect.ring_step;
    ordering = mission_effect.cur_ot;
    if (step)
    {
        for (index = 0; index < MISSION_EFFECT_SPRITE_COUNT; ++index)
        {
            number = mission_effect.spr[index].phase + step;
            if (number >= 62)
                number %= 62;
            mission_effect.spr[index].phase = (sint16)number;
        }
    }
    xport_gte_write_control(21, game_runtime.display.background[0] << 4);
    xport_gte_write_control(22, game_runtime.display.background[1] << 4);
    xport_gte_write_control(23, game_runtime.display.background[2] << 4);
    xport_gte_write_data(6, 0x2CB4B4B4);
    for (index = 0; index < MISSION_EFFECT_SPRITE_COUNT; ++index)
    {
        mode = mission_effect.layers[MISSION_EFFECT_SPR].mode;
        if (mode == 1)
        {
            number = mission_effect.spr[index].phase;
            counter = (sint32)mission_effect.layers[MISSION_EFFECT_SPR].elapsed;
            if (counter < number)
                continue;
        }
        if (mode == 3)
        {
            counter = (sint32)mission_effect.layers[MISSION_EFFECT_SPR].elapsed;
            number = mission_effect.spr[index].phase;
            if (number < counter)
                continue;
        }
        RotMatrix(&mission_effect.view_angles, sprite_pose);
        TransMatrix(sprite_pose, &mission_effect.spr[index].pos);
        number = mission_effect.spr[index].phase;
        RotMatrix(&mission_effect.ring_angle[number], ring_pose);
        TransMatrix(ring_pose, &mission_effect.ring_pos[number]);
        mission_effect_compose_matrix(ring_pose, sprite_pose, combined);
        mission_effect_compose_matrix(&mission_effect.view_matrix, combined, view);
        SetRotMatrix(view);
        SetTransMatrix(view);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 0);
        xport_gte_execute(0x180001);
        POLY_FT4 *quad = mission_packet_span(packet, 38);
        quad->p[1].uv_attr = 0x000FE02F;
        quad->p[2].uv.uv = (uint16)(0xEF20);
        quad->p[0].uv_attr = 0x343FE020;
        quad->p[3].uv.uv = (uint16)(0xEF2F);
        depth = xport_gte_read_data(19);
        screen->xy = xport_gte_read_data(14);
        if ((sint32)depth <= 0)
            continue;
        xport_gte_execute(0x780010);
        size = 64000 / (sint32)depth;
        if ((sint32)size < 2)
            size = 2;
        number = screen->x;
        bucket = (sint32)depth >> 8;
        scratch->sprite.bucket = bucket;
        x = ((uint32)number - (uint32)((sint32)size >> 2));
        number = screen->y;
        right = ((uint32)x + (uint32)((sint32)size >> 1));
        y = ((uint32)number - (uint32)((sint32)size >> 1));
        if (right < 0 || x >= 321)
            continue;
        bottom = ((uint32)y + size);
        if (bottom < 0 || y >= 481)
            continue;
        value = (uint32)y << 16;
        quad = mission_packet_span(packet, sizeof(*quad));
        quad->p[1].xy.xy = value | (uint32)right;
        quad->p[0].xy.xy = value | (uint32)x;
        value = (uint32)bottom << 16;
        quad->p[2].xy.xy = value | (uint32)x;
        quad->p[3].xy.xy = value | (uint32)right;
        quad->rgbc.rgbc = xport_gte_read_data(22);
        value = bucket;
        if ((sint32)value >= 256)
            value = 255;
        target = ordering + value * 4;
        uint32 *ot_entry = mission_effect_ot_span(target, sizeof(*ot_entry));
        tag = *ot_entry;
        quad->tag = tag | 0x09000000;
        *ot_entry = packet & 0xFFFFFF;
        packet += 40;
    }
    mode = mission_effect.layers[MISSION_EFFECT_SPR].mode;
    if (mode == 1)
    {
        counter = (sint32)mission_effect.layers[MISSION_EFFECT_SPR].elapsed;
        if (counter >= 64)
        {
            mission_effect.layers[MISSION_EFFECT_SPR].elapsed = 0;
            mission_effect.layers[MISSION_EFFECT_SPR].mode = 2;
            mode = mission_effect.layers[MISSION_EFFECT_SPR].mode;
        }
    }
    if (mode == 3)
    {
        counter = (sint32)mission_effect.layers[MISSION_EFFECT_SPR].elapsed;
        if (counter >= 64)
        {
            mission_effect.layers[MISSION_EFFECT_SPR].elapsed = 0;
            mission_effect.layers[MISSION_EFFECT_SPR].mode = 0;
        }
    }
    return packet;
}

static void mission_effect_set_ring_transform(uint32 sample, MISSION_EFFECT_SCRATCH_PUBLICATION *scratch)
{
    MATRIX *pose = &scratch->ring.pose.matrix, *view = &scratch->ring.view;

    mission_effect_rotation_gpf(&mission_effect.ring_angle[sample], &scratch->ring.pose);
    TransMatrix(pose, &mission_effect.ring_pos[sample]);
    mission_effect_compose_matrix(&mission_effect.view_matrix, pose, view);
    SetRotMatrix(view);
    SetTransMatrix(view);
}

static void mission_effect_project_ring_group(MISSION_RING_PUBLICATION *published, uint32 index)
{
    uint32 fog;

    if (index == MISSION_EFFECT_RING_VERTEX_COUNT - 1)
    {
        xport_gte_execute(0x180001);
        published->screen[0][index].xy = xport_gte_read_data(14);
        published->depth[0][index] = xport_gte_read_data(19);
        /* the final vertex takes MAC0 instead of IR0 */
        published->fog[0][index] = xport_gte_read_data(24);
        return;
    }
    xport_gte_execute(0x280030);
    published->screen[0][index].xy = xport_gte_read_data(12);
    published->screen[0][index + 1].xy = xport_gte_read_data(13);
    published->screen[0][index + 2].xy = xport_gte_read_data(14);
    fog = xport_gte_read_data(8);
    published->depth[0][index] = xport_gte_read_data(17);
    published->depth[0][index + 1] = xport_gte_read_data(18);
    published->depth[0][index + 2] = xport_gte_read_data(19);
    published->fog[0][index + 2] = fog;
    published->fog[0][index + 1] = fog;
    published->fog[0][index] = fog;
}

typedef struct
{
    uint32 tag;
    uint32 command;
    XY destination;
    uint32 dimensions;
    uint16 colors[16];
    uint32 trailer;
} MISSION_RING_PALETTE_PACKET;

typedef char mission_ring_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(MISSION_RING_PALETTE_PACKET) == 52 && offsetof(MISSION_RING_PALETTE_PACKET, command) == 4 && offsetof(MISSION_RING_PALETTE_PACKET, destination) == 8 && offsetof(MISSION_RING_PALETTE_PACKET, dimensions) == 12 && offsetof(MISSION_RING_PALETTE_PACKET, colors) == 16 && offsetof(MISSION_RING_PALETTE_PACKET, trailer) == 48 ? 1 : -1];

static uint32 mission_effect_draw_ring_sparse(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    uint32 clut, page, ordering, scroll, ring, table;
    uint32 index, lane, a, b, c, value, phase, quotient, mode, offset;
    uint32 uv, bottom, depth, link, upload;
    XY first, second, third, fourth;

    FUNCTION_MARKER(0x800FF95C, "ACE3.BIN");
    clut = mission_effect.tpage;
    page = mission_effect.clut;
    scroll = mission_effect.ring_scroll;
    ordering = mission_effect.cur_ot;
    MISSION_RING_PUBLICATION *published = &scratch->ring.projected;
    scroll = 224 - ((scroll * 4) & 31);
    for (ring = 0; ring < 7; ++ring)
    {
        mission_effect_set_ring_transform(ring * 8, scratch);
        for (index = 0; index < MISSION_EFFECT_RING_VERTEX_COUNT; index += 3)
        {
            for (lane = 0; lane < 3 && index + lane < MISSION_EFFECT_RING_VERTEX_COUNT; ++lane)
            {
                phase = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
                quotient = ((sint32)phase / 8);
                if (quotient == ring - 1)
                {
                    mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
                    value = mode == 1 ? (7 - (phase - ((ring - 1) << 3))) * 25000 : 0;
                    value = ((sint32)value / 64);
                    mission_effect.ring_vertex[index + lane].vz = (sint16)value;
                }
                else if (quotient == ring)
                {
                    mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
                    value = mode == 3 ? (uint32)((sint32)(0 - phase) % 8) * 25000 : 0;
                    value = ((sint32)value / 64);
                    mission_effect.ring_vertex[index + lane].vz = (sint16)value;
                }
                xport_gte_load_vertex(lane, &mission_effect.ring_vertex[index + lane]);
            }
            mission_effect_project_ring_group(published, index);
        }
        if (ring)
        {
            xport_gte_write_control(21, 2048);
            xport_gte_write_control(22, 2048);
            xport_gte_write_control(23, 2048);
            uv = scroll << 8;
            bottom = (scroll + 31) << 8;
            for (index = 0; index < 16; ++index)
            {
                mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
                if (mode == 1)
                {
                    phase = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
                    if ((sint32)phase / 8 < (sint32)(ring - 1))
                        break;
                    mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
                }
                if (mode == 3)
                {
                    phase = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
                    if ((sint32)(ring - 1) < (sint32)phase / 8)
                        break;
                }
                first = published->screen[1][index];
                second = published->screen[1][((index + 1) & 15)];
                third = published->screen[0][index];
                fourth = published->screen[0][((index + 1) & 15)];
                if (first.x < 0 && second.x < 0 && third.x < 0 && fourth.x < 0)
                    continue;
                if (first.y < 0 && second.y < 0 && third.y < 0 && fourth.y < 0)
                    continue;
                if (first.x >= 321 && second.x >= 321 && third.x >= 321 && fourth.x >= 321)
                    continue;
                if (first.y >= 481 && second.y >= 481 && third.y >= 481 && fourth.y >= 481)
                    continue;
                POLY_FT4 *quad = mission_packet_span(packet, sizeof(*quad));
                a = first.xy;
                b = third.xy;
                c = fourth.xy;
                xport_gte_write_data(12, a);
                xport_gte_write_data(13, b);
                xport_gte_write_data(14, c);
                quad->p[0].xy.xy = a;
                quad->p[2].xy.xy = b;
                xport_gte_execute(0x1400006);
                quad->rgbc.rgbc = 0x2C838383;
                value = second.xy;
                quad->p[3].xy.xy = c;
                quad->p[1].xy.xy = value;
                if ((sint32)xport_gte_read_data(24) < 0)
                    continue;
                quad->p[0].uv_attr = uv | (page << 16);
                quad->p[1].uv_attr = uv | (clut << 16) | 31;
                quad->p[2].uv_attr = bottom;
                quad->p[3].uv_attr = bottom | 31;
                depth = ((sint32)published->depth[1][index] >> 8);
                if ((sint32)depth >= 256)
                    depth = 255;
                table = ordering + depth * 4;
                uint32 *ordering_entry = mission_effect_ot_span(table, sizeof(*ordering_entry));
                link = *ordering_entry;
                quad->tag = link | 0x09000000;
                *ordering_entry = packet & 0xFFFFFF;
                packet += 40;
            }
            MISSION_RING_PALETTE_PACKET *palette = mission_packet_span(packet, sizeof(*palette));
            palette->tag = 0x0C000000;
            palette->command = 0xA0000000;
            palette->trailer = 0x01000000;
            palette->dimensions = 0x00010010;
            upload = packet;
            palette->destination.xy = 0x00D103F0;
            packet += 52;
            value = game_runtime.display.background[0];
            xport_gte_write_control(21, value << 4);
            value = game_runtime.display.background[1];
            xport_gte_write_control(22, value << 4);
            value = game_runtime.display.background[2];
            xport_gte_write_control(23, value << 4);
            for (index = 0; index < 16; ++index)
            {
                a = (sint16)mission_effect.pal[index];
                b = published->fog[0][0];
                xport_gte_write_data(28, a);
                xport_gte_write_data(8, b);
                xport_gte_execute(0x980011);
                value = xport_gte_read_data(29);
                palette->colors[index] = (uint16)(value);
            }
            depth = (uint32)((sint32)published->depth[1][0] >> 8) + 5;
            if ((sint32)depth >= 256)
                depth = 255;
            table = ordering + depth * 4;
            ordering_word = mission_effect_ot_span(table, sizeof(*ordering_word));
            link = *ordering_word;
            packet_link = mission_packet_span(upload, sizeof(*packet_link));
            packet_link->addr = link;
            ((P_TAG *)ordering_word)->addr = upload;
        }
        for (index = 0; index < 16; ++index)
        {
            offset = index * 4;
            published->screen[1][index].xy = published->screen[0][index].xy;
            published->depth[1][index] = published->depth[0][index];
            published->fog[1][index] = published->fog[0][index];
        }
    }
    return packet;
}

static uint32 mission_effect_draw_ring(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 value;

    FUNCTION_MARKER(0x8010084C, "ACE3.BIN");
    mission_effect_update_ring(scratch);
    packet = mission_effect_draw_ring_sparse(scratch, packet);
    packet = mission_effect_draw_ring_dense(scratch, packet);
    value = mission_effect.layers[MISSION_EFFECT_RING].mode;
    if (value == 1)
    {
        value = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
        if ((sint32)value / 8 < 12)
            return packet;
        mission_effect.layers[MISSION_EFFECT_RING].elapsed = 0;
        mission_effect.layers[MISSION_EFFECT_RING].mode = 2;
        value = sound_play(93);
        *menu_voice_slot(0) = (uint32)(value);
        value = sound_play(94);
        *menu_voice_slot(1) = (uint32)(value);
        value = sound_play(95);
        *menu_voice_slot(2) = (uint32)(value);
        value = sound_play(96);
        *menu_voice_slot(3) = (uint32)(value);
        value = sound_play(97);
        *menu_voice_slot(4) = (uint32)(value);
    }
    else if (value == 3)
    {
        value = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
        if ((sint32)value / 8 >= 12)
        {
            mission_effect.layers[MISSION_EFFECT_RING].elapsed = 0;
            mission_effect.layers[MISSION_EFFECT_RING].mode = 0;
        }
    }
    return packet;
}

static uint32 mission_effect_draw_ring_dense(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 clut, page, ordering, scroll, ring, sample, table;
    uint32 a, b, index, offset;
    uint32 uv, bottom, mode, depth, link;
    XY first, second, third, fourth;

    FUNCTION_MARKER(0x80100304, "ACE3.BIN");
    clut = mission_effect.tpage;
    page = mission_effect.clut;
    scroll = mission_effect.ring_scroll;
    ordering = mission_effect.cur_ot;
    MISSION_RING_PUBLICATION *published = &scratch->ring.projected;
    scroll = (scroll & 7) * 4;
    for (ring = 0; ring < 16; ++ring)
    {
        sample = ring + 48;
        mission_effect_set_ring_transform(sample, scratch);
        /* Sixteen vertices form five triples followed by one RTPS */
        for (index = 0; index < MISSION_EFFECT_RING_VERTEX_COUNT; index += 3)
        {
            gte_ldv0(&mission_effect.ring_vertex[index]);
            if (index + 2 < MISSION_EFFECT_RING_VERTEX_COUNT)
            {
                gte_ldv1(&mission_effect.ring_vertex[index + 1]);
                gte_ldv2(&mission_effect.ring_vertex[index + 2]);
            }
            mission_effect_project_ring_group(published, index);
        }
        if (ring)
        {
            xport_gte_write_control(21, 2048);
            xport_gte_write_control(22, 2048);
            xport_gte_write_control(23, 2048);
            uv = ((((ring - 1) & 7) * 4 - scroll) & 31);
            bottom = (uv + 199) << 8;
            uv = (uv + 192) << 8;
            for (index = 0; index < 16; ++index)
            {
                mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
                if (mode == 1 && (sint32)(mission_effect.layers[MISSION_EFFECT_RING].elapsed - 48) < (sint32)ring)
                    break;
                if (mode == 3 && (sint32)ring < (sint32)(mission_effect.layers[MISSION_EFFECT_RING].elapsed - 48))
                    break;
                first = published->screen[1][index];
                second = published->screen[1][((index + 1) & 15)];
                third = published->screen[0][index];
                fourth = published->screen[0][((index + 1) & 15)];
                if (first.x < 0 && second.x < 0 && third.x < 0 && fourth.x < 0)
                    continue;
                if (first.y < 0 && second.y < 0 && third.y < 0 && fourth.y < 0)
                    continue;
                if (first.x >= 321 && second.x >= 321 && third.x >= 321 && fourth.x >= 321)
                    continue;
                if (first.y >= 481 && second.y >= 481 && third.y >= 481 && fourth.y >= 481)
                    continue;
                POLY_FT4 *quad = mission_packet_span(packet, sizeof(*quad));
                a = first.xy;
                b = second.xy;
                quad->p[0].xy.xy = a;
                quad->p[1].xy.xy = b;
                a = third.xy;
                b = fourth.xy;
                quad->rgbc.rgbc = 0x2C838383;
                quad->p[0].uv_attr = uv | (page << 16);
                quad->p[2].uv_attr = bottom;
                quad->p[3].uv_attr = bottom | 31;
                quad->p[1].uv_attr = uv | (clut << 16) | 31;
                quad->p[2].xy.xy = a;
                quad->p[3].xy.xy = b;
                depth = ((sint32)published->depth[1][index] >> 8);
                if ((sint32)depth >= 256)
                    depth = 255;
                table = ordering + depth * 4;
                uint32 *ordering_entry = mission_effect_ot_span(table, sizeof(*ordering_entry));
                link = *ordering_entry;
                quad->tag = link | 0x09000000;
                *ordering_entry = packet & 0xFFFFFF;
                packet += 40;
            }
        }
        for (index = 0; index < 16; ++index)
        {
            offset = index * 4;
            published->screen[1][index].xy = published->screen[0][index].xy;
            published->fog[1][index] = published->fog[0][index];
            published->depth[1][index] = published->depth[0][index];
        }
    }
    return packet;
}

static void mission_effect_update_ring(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch)
{
    uint32 value, step, total, quotient, count, mode;
    uint32 anchor[3], a, b, c, index, counter, x, y, z;
    uint32 dx, dy, dz;
    MATRIX rotation = {0};
    VECTOR offset = {0};

    FUNCTION_MARKER(0x800FF4EC, "ACE3.BIN");
    value = mission_effect.frame_counter;
    total = mission_effect.ring_remainder;
    step = (value & 511) - 256;
    if ((sint32)step < 0)
        step = 0 - step;
    step += 128;
    total += step;
    total = (sint16)total;
    mission_effect.ring_remainder = total;
    quotient = ((sint32)total / 390);
    mission_effect.ring_step = quotient;
    count = (sint16)mission_effect.ring_step;
    mission_effect.ring_remainder = total - quotient * 390;
    if (count)
    {
        value = mission_effect.ring_scroll;
        mode = mission_effect.layers[MISSION_EFFECT_RING].mode;
        mission_effect.ring_scroll = (value + count) & 7;
        if (mode == 1 || mode == 3)
        {
            a = (sint16)mission_effect.ring_step;
            b = mission_effect.layers[MISSION_EFFECT_RING].elapsed;
            mission_effect.layers[MISSION_EFFECT_RING].elapsed = b + a;
        }
        mode = mission_effect.layers[MISSION_EFFECT_SPR].mode;
        if (mode == 1 || mode == 3)
        {
            a = (sint16)mission_effect.ring_step;
            b = mission_effect.layers[MISSION_EFFECT_SPR].elapsed;
            mission_effect.layers[MISSION_EFFECT_SPR].elapsed = b + a;
        }
        count = (sint16)mission_effect.ring_step;
        anchor[0] = mission_effect.ring_pos[62].vx;
        anchor[1] = mission_effect.ring_pos[62].vy;
        anchor[2] = mission_effect.ring_pos[62].vz;
        index = 64;
        if ((sint32)count < 64)
        {
            do
            {
                --index;
                mission_effect.ring_angle[index] = mission_effect.ring_angle[index - 1];
                a = mission_effect.ring_pos[index - 1].vx;
                b = mission_effect.ring_pos[index - 1].vy;
                c = mission_effect.ring_pos[index - 1].vz;
                mission_effect.ring_pos[index].vx = (sint32)(a - anchor[0]);
                mission_effect.ring_pos[index].vy = (sint32)(b - anchor[1]);
                mission_effect.ring_pos[index].vz = (sint32)(c - anchor[2]);
            } while ((sint32)count < (sint32)index);
        }
        count = (sint16)mission_effect.ring_step;
        counter = (sint16)mission_effect.ring_phase;
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 390);
        ++counter;
        do
        {
            value = (uint32)rsin((sint32)(counter << 4));
            mission_effect.ring_angle[count - 1].vx = (sint16)((uint32)((sint32)value >> 6));
            mission_effect.ring_angle[count - 1].vy = (sint16)((uint32)((sint32)value >> 5));
            mission_effect.ring_angle[count - 1].vz = 0;
            mission_effect_rotation_gpf(&mission_effect.ring_angle[count], &scratch->ring.pose);
            SetRotMatrix(&scratch->ring.pose.matrix);
            b = mission_effect.ring_pos[count].vy;
            c = mission_effect.ring_pos[count].vz;
            a = mission_effect.ring_pos[count].vx;
            xport_gte_write_control(5, a);
            xport_gte_write_control(6, b);
            xport_gte_write_control(7, c);
            --count;
            xport_gte_execute(0x480012);
            mission_effect.ring_pos[count].vx = (sint32)xport_gte_read_data(25);
            mission_effect.ring_pos[count].vy = (sint32)xport_gte_read_data(26);
            mission_effect.ring_pos[count].vz = (sint32)xport_gte_read_data(27);
            ++counter;
        } while ((sint32)count > 0);
        mission_effect.ring_phase = counter - 1;
    }
    value = (uint32)(sint16)mission_effect.ring_remainder << 12;
    x = mission_effect.ring_pos[62].vx;
    a = mission_effect.ring_pos[62].vz;
    b = mission_effect.ring_pos[63].vz;
    c = mission_effect.ring_pos[63].vx;
    dz = a - b;
    a = mission_effect.ring_pos[62].vy;
    b = mission_effect.ring_pos[63].vy;
    dx = x - c;
    dy = a - b;
    value = ((sint32)value / 390);
    xport_gte_write_data(8, value);
    xport_gte_write_data(9, dx);
    xport_gte_write_data(10, dy);
    xport_gte_write_data(11, dz);
    scratch->ring.angle[2] = 0;
    mission_effect.view_angles.vz = 0;
    xport_gte_execute(0x198003D);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_data(9, mission_effect.ring_pos[42].vx);
    xport_gte_write_data(10, mission_effect.ring_pos[42].vy);
    xport_gte_write_data(11, mission_effect.ring_pos[42].vz);
    xport_gte_write_control(21, mission_effect.ring_pos[43].vx);
    xport_gte_write_control(22, mission_effect.ring_pos[43].vy);
    xport_gte_write_control(23, mission_effect.ring_pos[43].vz);
    xport_gte_execute(0x980011);
    scratch->ring.anchor_offset[0] = 0 - x;
    scratch->ring.anchor_offset[1] = 0 - y;
    scratch->ring.anchor_offset[2] = 0 - z;
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    dx = a - x;
    dz = c - z;
    dy = b - y;
    value = (uint32)ratan2((sint32)dy, (sint32)dz);
    scratch->ring.angle[0] = value;
    mission_effect.view_angles.vx = (sint16)value;
    value = (uint32)ratan2((sint32)dx, (sint32)dz);
    scratch->ring.angle[1] = value;
    mission_effect.view_angles.vy = (sint16)value;
    RotMatrix(&mission_effect.view_angles, &rotation);
    TransposeMatrix(&rotation, &mission_effect.view_matrix);
    offset.vx = (sint32)(0 - x);
    offset.vy = (sint32)(0 - y);
    offset.vz = (sint32)(0 - z);
    mission_effect_apply_matrix(&mission_effect.view_matrix, &offset, &offset);
    scratch->ring.anchor_offset[0] = (uint32)offset.vx;
    scratch->ring.anchor_offset[1] = (uint32)offset.vy;
    scratch->ring.anchor_offset[2] = (uint32)offset.vz;
    mission_effect.view_matrix.t[0] = offset.vx;
    mission_effect.view_matrix.t[1] = offset.vy;
    mission_effect.view_matrix.t[2] = offset.vz;
    {
        (void)(uint32)((uint32)offset.vz);
        return;
    }
}

static void mission_effect_rotation_gpf(const SVECTOR *angles, MISSION_ROTATION_PUBLICATION *published)
{
    uint32 packed, z, y, x, value, sine[3], cosine[3], index, magnitude;
    uint32 first[3], second[3], third[3], fourth[3], product, lane;

    FUNCTION_MARKER(0x800A79A8, "ACE3.BIN");
    z = (sint32)angles->vz;
    y = (sint32)angles->vy;
    x = (sint32)angles->vx;
    for (index = 0; index < 3; ++index)
    {
        value = index == 0 ? z : index == 1 ? y : x;
        magnitude = (sint32)value < 0 ? 0 - value : value;
        packed = ((uint32)(uint16)rsin((((magnitude << 2) & 0x3FFC)) >> 2) | ((uint32)(uint16)rcos((((magnitude << 2) & 0x3FFC)) >> 2) << 16));
        sine[index] = (sint16)packed;
        if ((sint32)value < 0)
            sine[index] = (sint16)(0 - sine[index]);
        cosine[index] = ((sint32)packed >> 16);
    }
    xport_gte_write_data(8, cosine[2]);
    xport_gte_write_data(9, sine[1]);
    xport_gte_write_data(10, sine[0]);
    xport_gte_write_data(11, cosine[0]);
    xport_gte_execute(0x198003D);
    product = cosine[1] * cosine[2];
    for (index = 0; index < 3; ++index)
        first[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, sine[2]);
    xport_gte_write_data(9, sine[1]);
    xport_gte_write_data(10, sine[0]);
    xport_gte_write_data(11, cosine[0]);
    xport_gte_execute(0x198003D);
    published->matrix.m[2][2] = (sint16)((sint32)product >> 12);
    for (index = 0; index < 3; ++index)
        second[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, cosine[0]);
    xport_gte_write_data(9, cosine[1]);
    product = cosine[1] * sine[2];
    xport_gte_write_data(10, second[0]);
    xport_gte_write_data(11, first[0]);
    xport_gte_execute(0x198003D);
    for (index = 0; index < 3; ++index)
        third[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(8, sine[0]);
    xport_gte_write_data(9, cosine[1]);
    xport_gte_write_data(10, second[0]);
    xport_gte_write_data(11, first[0]);
    xport_gte_execute(0x198003D);
    lane = ((third[1] + first[1]) << 16) | (sine[1] & 0xFFFF);
    published->words[1] = lane;
    fourth[0] = xport_gte_read_data(9);
    lane = ((0 - fourth[0]) << 16) | (third[0] & 0xFFFF);
    published->words[0] = lane;
    fourth[1] = xport_gte_read_data(10);
    fourth[2] = xport_gte_read_data(11);
    value = 0 - (uint32)((sint32)product >> 12);
    lane = (value << 16) | ((first[2] - fourth[1]) & 0xFFFF);
    published->words[2] = lane;
    lane = ((second[2] + fourth[2]) << 16) | ((second[1] - third[2]) & 0xFFFF);
    published->words[3] = lane;
}

static uint32 mission_effect_draw_trail_layer(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch, uint32 packet)
{
    uint32 value;

    FUNCTION_MARKER(0x80101E4C, "ACE3.BIN");
    value = mission_effect.layers[MISSION_EFFECT_TRAIL].mode;
    if (value == 1)
    {
        mission_effect.layers[MISSION_EFFECT_TRAIL].mode = 2;
        value = mission_effect.layers[MISSION_EFFECT_TRAIL].mode;
    }
    if (value == 3)
    {
        value = mission_effect.layers[MISSION_EFFECT_TRAIL].elapsed;
        if ((sint32)value < 60)
            mission_effect.layers[MISSION_EFFECT_TRAIL].elapsed = value + 1;
        else
            mission_effect.layers[MISSION_EFFECT_TRAIL].mode = 0;
    }
    mission_effect_update_trail_view(scratch);
    return mission_effect_draw_trails(scratch, packet);
}

static void mission_effect_update_trail_view(MISSION_EFFECT_SCRATCH_PUBLICATION *scratch)
{
    MATRIX rotation = {0};
    VECTOR offset = {0};

    FUNCTION_MARKER(0x801016A4, "ACE3.BIN");
    mission_effect.view_angles = (SVECTOR){0};
    RotMatrix(&mission_effect.view_angles, &rotation);
    scratch->trail_view.view.t[0] = 0;
    scratch->trail_view.view.t[1] = 0;
    scratch->trail_view.view.t[2] = 0;
    mission_effect_set_translation(&rotation, &offset);
    TransposeMatrix(&rotation, &mission_effect.view_matrix);
    mission_effect_apply_matrix(&mission_effect.view_matrix, &offset, &offset);
    mission_effect_set_translation(&mission_effect.view_matrix, &offset);
}

static void mission_effect_link_ot(uint32 ordering)
{
    uint32 bank, packet, value, tag, link;

    FUNCTION_MARKER(0x800FF108, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    packet = mission_effect.ot[bank];
    mission_effect.cur_ot = packet;
    ClearOTagR(mission_effect_ot_span(packet, 1024), 256);
    packet = mission_effect.cur_ot;
    uint32 *ot_entry = mission_effect_ot_span(ordering, sizeof(*ot_entry));
    uint32 *chain_head = mission_effect_ot_span(packet, sizeof(*chain_head));
    link = *ot_entry;
    tag = *chain_head;
    *chain_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    tag = *ot_entry;
    packet = mission_effect.cur_ot;
    value = (packet + 1020) & 0xFFFFFF;
    *ot_entry = (tag & 0xFF000000) | value;
    {
        (void)(uint32)(value);
        return;
    }
}

void mission_effect_begin_frame(uint32 ordering)
{
    FUNCTION_MARKER(0x800FF100, "ACE3.BIN");
    {
        (void)(mission_effect_link_ot(ordering));
        return;
    }
}

static uint32 mission_effect_draw_trail_strip(const XY *current, const XY *previous, uint32 item, uint32 packet)
{
    uint32 ordering, shade[2], index, input, high, quotient, color, next_color;
    uint32 link;
    sint64 product;

    FUNCTION_MARKER(0x801018D8, "ACE3.BIN");
    ordering = mission_effect.cur_ot;
    for (index = 0; index < 2; ++index)
    {
        input = (item + index) << 6;
        product = (sint64)(sint32)input * (sint32)0x88888889;
        high = (product >> 32);
        quotient = (uint32)((sint32)(high + input) >> 3) - (uint32)((sint32)input >> 31);
        shade[index] = (sint16)(64 - quotient);
    }
    color = shade[0] | (shade[0] << 8) | (shade[0] << 16);
    next_color = shade[1] | (shade[1] << 8) | (shade[1] << 16);
    for (index = 0; index < 4; ++index)
    {
        POLY_G4 *quad = mission_packet_span(packet, sizeof(*quad));
        quad->p[0].rgbc.rgbc = color;
        quad->p[1].rgbc.rgbc = color;
        quad->p[2].rgbc.rgbc = next_color;
        quad->p[3].rgbc.rgbc = next_color;
        quad->p[0].xy.xy = current[0].xy;
        quad->p[1].xy.xy = current[index + 1].xy;
        quad->p[2].xy.xy = previous[0].xy;
        quad->p[0].rgbc.code = (uint8)(0x3A);
        quad->p[3].xy.xy = previous[index + 1].xy;
        uint32 *ot_entry = mission_effect_ot_span(ordering, sizeof(*ot_entry));
        link = *ot_entry;
        quad->tag = link | 0x08000000;
        *ot_entry = packet & 0xFFFFFF;
        packet += 36;
    }
    return packet;
}

static void mission_effect_set_translation(MATRIX *matrix, VECTOR *offset)
{
    FUNCTION_MARKER(0x800A6ED8, "ACE3.BIN");
    TransMatrix(matrix, offset);
}

static void mission_effect_compose_matrix(const MATRIX *left, const MATRIX *right, MATRIX *output)
{
    uint32 first_column[3], second_column[3];
    uint32 third_x, third_y, first, second, third, index;
    uint32 translation[3], origin[3], result[3];
    uint32 last_rotation_word;

    FUNCTION_MARKER(0x800A6748, "ACE3.BIN");
    gte_SetRotMatrix(left);
    first = (uint16)right->m[0][0];
    second = (uint16)right->m[0][2] | ((uint32)(uint16)right->m[1][0] << 16);
    third = (uint16)right->m[2][0] | ((uint32)(uint16)right->m[2][1] << 16);
    xport_gte_write_data(0, first | (second & 0xFFFF0000));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    first = (uint16)right->m[0][1];
    second = (uint16)right->m[1][1] | ((uint32)(uint16)right->m[1][2] << 16);
    third = right->m[2][1];
    for (index = 0; index < 3; ++index)
        first_column[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(0, first | (second << 16));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    first = (uint16)right->m[0][2];
    second = (uint16)right->m[1][1] | ((uint32)(uint16)right->m[1][2] << 16);
    /* preserve the last rotation halfword and both MATRIX padding bytes */
    memcpy(&third, (const uint8 *)right + 16, sizeof(third));
    for (index = 0; index < 3; ++index)
        second_column[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(0, first | (second & 0xFFFF0000));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    output->m[0][0] = first_column[0];
    output->m[0][1] = second_column[0];
    output->m[2][0] = first_column[2];
    output->m[2][1] = second_column[2];
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    last_rotation_word = xport_gte_read_data(11);
    memcpy((uint8 *)output + 16, &last_rotation_word, sizeof(last_rotation_word));
    first = (uint16)right->t[0];
    second = right->t[1];
    third = right->t[2];
    xport_gte_write_data(0, first | (second << 16));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    output->m[0][2] = third_x;
    output->m[1][0] = first_column[1];
    output->m[1][1] = second_column[1];
    output->m[1][2] = third_y;
    for (index = 0; index < 3; ++index)
        translation[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        origin[index] = (uint32)left->t[index];
    for (index = 0; index < 3; ++index)
    {
        result[index] = translation[index] + origin[index];
        if ((translation[index] ^ result[index]) & (origin[index] ^ result[index]) & 0x80000000)
        {
#ifndef NDEBUG
            fprintf(stderr, "CompMatrix original ADD overflow at axis %u\n", index);
#endif
            abort();
        }
    }
    for (index = 0; index < 3; ++index)
        output->t[index] = result[index];
}

static void mission_effect_apply_matrix(const MATRIX *matrix, const VECTOR *vector, VECTOR *output)
{
    uint32 input[3], high[3], low[3], mac[3], result[3];
    uint32 index, magnitude;

    FUNCTION_MARKER(0x800A68A8, "ACE3.BIN");
    gte_SetRotMatrix(matrix);
    input[0] = (uint32)vector->vx;
    input[1] = (uint32)vector->vy;
    input[2] = (uint32)vector->vz;
    for (index = 0; index < 3; ++index)
    {
        magnitude = (sint32)input[index] < 0 ? 0 - input[index] : input[index];
        high[index] = ((sint32)magnitude >> 15);
        low[index] = magnitude & 0x7FFF;
        if ((sint32)input[index] < 0)
        {
            high[index] = 0 - high[index];
            low[index] = 0 - low[index];
        }
    }
    for (index = 0; index < 3; ++index)
        xport_gte_write_data(9 + index, high[index]);
    xport_gte_execute(0x41E012);
    for (index = 0; index < 3; ++index)
        mac[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        xport_gte_write_data(9 + index, low[index]);
    xport_gte_execute(0x49E012);
    for (index = 0; index < 3; ++index)
        result[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        result[index] += mac[index] << 3;
    output->vx = (sint32)result[0];
    output->vy = (sint32)result[1];
    output->vz = (sint32)result[2];
}
