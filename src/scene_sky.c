#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    uint32 effect;
    uint8 horizon_extension, mission_six;
    uint32 colors[6];
    uint32 altitude_limit;
    sint32 camera_position[3];
    uint16 camera_angles[3];
    uint16 camera_distance;
} SCENE_LIGHT_PASS;

typedef union
{
    uint32 word;
    uint16 half[2];
} SCENE_LIGHT_XY;

typedef struct
{
    uint32 mask;
    SCENE_LIGHT_XY source[5][2], screen[5][2];
    uint32 depth_or_code[7];
    uint32 area[5][4];
    SCENE_LIGHT_XY bounds[4];
} SCENE_LIGHT_PROJECTION;

typedef char SCENE_LIGHT_PROJECTION_LAYOUT_CHECK[sizeof(SCENE_LIGHT_XY) == 4 && offsetof(SCENE_LIGHT_PROJECTION, source) == 4 && offsetof(SCENE_LIGHT_PROJECTION, screen) == 44 && offsetof(SCENE_LIGHT_PROJECTION, depth_or_code) == 84 && offsetof(SCENE_LIGHT_PROJECTION, area) == 112 && offsetof(SCENE_LIGHT_PROJECTION, bounds) == 192 && sizeof(SCENE_LIGHT_PROJECTION) == 208 ? 1 : -1];

typedef struct
{
    uint32 ordering, cursor;
} SCENE_LIGHT_FRAME_PUBLICATION;

typedef char scene_light_frame_publication_layout[sizeof(SCENE_LIGHT_FRAME_PUBLICATION) == 8 && offsetof(SCENE_LIGHT_FRAME_PUBLICATION, cursor) == 4 ? 1 : -1];

enum
{
    SKY_RING_SEGMENTS = 24,
    SKY_RING_VERTICES = SKY_RING_SEGMENTS + 1
};

typedef struct
{
    uint32 flags, screen;
    uint16 u, v;
    uint32 color;
} SKY_RING_VERTEX;

typedef char SKY_RING_LAYOUT_CHECK[sizeof(SKY_RING_VERTEX) == 16 && offsetof(SKY_RING_VERTEX, u) == 8 && offsetof(SKY_RING_VERTEX, color) == 12 ? 1 : -1];

typedef struct
{
    uint32 ordering, cursor;
    uint32 position[3];
    uint16 angles[3], distance;
} SKY_PASS_PUBLICATION;

typedef struct
{
    uint32 flags;
    XY screen;
    uint32 color;
} SKY_CENTER_PUBLICATION;

typedef char sky_publication_layout[sizeof(SKY_PASS_PUBLICATION) == 28 && offsetof(SKY_PASS_PUBLICATION, cursor) == 4 && offsetof(SKY_PASS_PUBLICATION, angles) == 20 && offsetof(SKY_PASS_PUBLICATION, distance) == 26 && sizeof(SKY_CENTER_PUBLICATION) == 12 && offsetof(SKY_CENTER_PUBLICATION, screen) == 4 && offsetof(SKY_CENTER_PUBLICATION, color) == 8 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(TILE) == 16 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 ? 1 : -1];

enum
{
    MODE_FOUR_RING_POINTS = 20,
    MODE_FOUR_SLICE_POINTS = 16
};

typedef struct
{
    uint32 flags, screen, color;
} MODE_FOUR_VERTEX;

typedef char MODE_FOUR_VERTEX_LAYOUT_CHECK[sizeof(MODE_FOUR_VERTEX) == 12 && offsetof(MODE_FOUR_VERTEX, screen) == 4 && offsetof(MODE_FOUR_VERTEX, color) == 8 ? 1 : -1];

typedef char mode_four_packet_layout_check[sizeof(POLY_GT4) == 52 && offsetof(POLY_GT4, p) == 4 && sizeof(POINT_CT) == 12 && offsetof(POINT_CT, xy) == 4 && offsetof(POINT_CT, uv) == 8 && offsetof(POINT_CT, attr) == 10 && sizeof(LINE_G2) == 20 && sizeof(LINE_G3) == 32 && offsetof(LINE_G2, p) == 4 && offsetof(LINE_G3, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && offsetof(LINE_G3, pad) == 28 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && offsetof(DR_STP, code) == 4 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 ? 1 : -1];

typedef struct
{
    uint32 tag;
    uint32 draw_mode;
    POINT_C p[4];
} SKY_RING_BLEND_PACKET;

typedef char sky_ring_packet_layout_check[sizeof(POLY_GT3) == 40 && sizeof(POLY_GT4) == 52 && sizeof(POLY_G4) == 36 && offsetof(POLY_GT3, p) == 4 && offsetof(POLY_GT4, p) == 4 && sizeof(POINT_CT) == 12 && offsetof(POINT_CT, xy) == 4 && offsetof(POINT_CT, uv) == 8 && offsetof(POINT_CT, attr) == 10 && sizeof(SKY_RING_BLEND_PACKET) == 40 && offsetof(SKY_RING_BLEND_PACKET, p) == 8 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

typedef char light_output_packet_layout_check[sizeof(POLY_G4) == 36 && sizeof(POLY_G3) == 28 && offsetof(POLY_G4, p) == 4 && offsetof(POLY_G3, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 ? 1 : -1];

typedef struct
{
    uint16 sample[3];
    uint8 reserved[338];
    uint32 base[3];
} SKY_NOISE_PUBLICATION;

typedef char sky_noise_publication_layout[sizeof(SKY_NOISE_PUBLICATION) == 356 && offsetof(SKY_NOISE_PUBLICATION, base) == 344 ? 1 : -1];

typedef char sky_layer_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 ? 1 : -1];

typedef union
{
    SKY_NOISE_PUBLICATION noise;

    struct
    {
        uint32 rotation_pitch;
        uint16 rotation_roll, reserved;
        uint16 matrix[3][3];
        uint8 before_projection[14];
        SCENE_LIGHT_PROJECTION projection;
        uint32 colors[6];
        uint32 altitude_limit;

        union
        {
            uint32 vertices[SCENE_LIGHT_POLYGON_CAPACITY];

            struct
            {
                uint32 prefix[SCENE_LIGHT_POLYGON_COLOR_OFFSET];
                uint32 colors[SCENE_LIGHT_POLYGON_CAPACITY];
            } gradient;
        } polygon;

        uint32 before_effect;
        uint32 effect, horizon_extension, mission_six;
    } light;
} SCENE_LIGHT_CONTEXT_PUBLICATION;

typedef char scene_light_context_publication_layout[sizeof(SCENE_LIGHT_PROJECTION) == 208 && sizeof(SCENE_LIGHT_CONTEXT_PUBLICATION) == 372 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.rotation_roll) == 4 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.matrix) == 8 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.projection) == 40 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.colors) == 248 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.altitude_limit) == 272 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.polygon.vertices) == 276 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.polygon.gradient.colors) == 308 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.effect) == 360 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.horizon_extension) == 364 && offsetof(SCENE_LIGHT_CONTEXT_PUBLICATION, light.mission_six) == 368 && offsetof(SKY_PASS_PUBLICATION, position) == 8 ? 1 : -1];

typedef struct
{
    uint32 rotation_words[5];
    uint32 axis[3];
    uint32 origin[3];
    uint32 reserved;
    uint32 tilt;

    union
    {
        MODE_FOUR_VERTEX slices[2][MODE_FOUR_SLICE_POINTS + 1];

        struct
        {
            uint8 before_focus[20];
            uint32 focus;
            uint32 radial[MODE_FOUR_RING_POINTS + 1];
            MODE_FOUR_VERTEX vertices[2][MODE_FOUR_RING_POINTS + 1];
        } ring;
    } work;
} MODE_FOUR_SCRATCH_PUBLICATION;

typedef char mode_four_scratch_publication_layout[sizeof(MODE_FOUR_SCRATCH_PUBLICATION) == 664 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, axis) == 20 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, origin) == 32 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, tilt) == 48 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, work.slices) == 52 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, work.ring.focus) == 72 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, work.ring.radial) == 76 && offsetof(MODE_FOUR_SCRATCH_PUBLICATION, work.ring.vertices) == 160 && sizeof(((MODE_FOUR_SCRATCH_PUBLICATION *)0)->work.slices) == 408 && sizeof(((MODE_FOUR_SCRATCH_PUBLICATION *)0)->work.ring.vertices) == 504 ? 1 : -1];

typedef struct
{
    SKY_CENTER_PUBLICATION center;
    uint32 radial[SKY_RING_SEGMENTS];
    SKY_RING_VERTEX vertices[2][SKY_RING_VERTICES];
} SKY_RING_PUBLICATION;

typedef struct
{
    SKY_PASS_PUBLICATION frame;

    union
    {
        SCENE_LIGHT_CONTEXT_PUBLICATION light;
        SKY_RING_PUBLICATION rings;
    } context;
} SKY_MODE_THREE_PUBLICATION;

typedef char sky_mode_three_publication_layout[sizeof(SKY_RING_PUBLICATION) == 908 && sizeof(SKY_MODE_THREE_PUBLICATION) == 936 && offsetof(SKY_RING_PUBLICATION, radial) == 12 && offsetof(SKY_RING_PUBLICATION, vertices) == 108 && offsetof(SKY_MODE_THREE_PUBLICATION, context) == 28 && offsetof(SKY_MODE_THREE_PUBLICATION, context.rings.radial) == 40 && offsetof(SKY_MODE_THREE_PUBLICATION, context.rings.vertices) == 136 && sizeof(((SKY_RING_PUBLICATION *)0)->radial) == 96 && sizeof(((SKY_RING_PUBLICATION *)0)->vertices) == 800 ? 1 : -1];

typedef char light_flat_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(POLY_F3) == 20 && offsetof(POLY_F3, rgbc) == 4 && offsetof(POLY_F3, p) == 8 && sizeof(POLY_F4) == 24 && offsetof(POLY_F4, rgbc) == 4 && offsetof(POLY_F4, p) == 8 && sizeof(XY) == 4 && sizeof(RGBC) == 4 ? 1 : -1];

static void scene_light_adjust_altitude_color(uint32 *color, uint32 altitude);
static void scene_light_blend_color(uint32 *native_color, uint32 target, uint32 scale);
static uint32 scene_light_build_horizon(SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection);
static void scene_light_build_horizon_rows(SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection);
static void scene_light_draw_horizon_bands(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass);
static void scene_light_draw_sky_layer(SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 *packet_cursor, const SCENE_LIGHT_PASS *pass);
static void scene_light_emit_clipped_band(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 index, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass);
static void scene_light_emit_layers(SCENE_LIGHT_CONTEXT_PUBLICATION *context, SCENE_LIGHT_PASS *pass, uint32 *packet_cursor, const SCENE_LIGHT_PROJECTION *projection);
static void scene_light_emit_noise(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 count);
static void scene_light_emit_visible_band(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 index, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass);
static void scene_light_initialize_projection(SKY_PASS_PUBLICATION *state, SCENE_LIGHT_CONTEXT_PUBLICATION *context, SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection);
static void scene_light_measure_projected_areas(SCENE_LIGHT_PROJECTION *projection);
static void scene_light_project_rows(SCENE_LIGHT_PROJECTION *projection, uint32 count);
static void scene_light_update_colors(SCENE_LIGHT_PASS *pass);
static void scene_sky_draw_rings(uint32 *packet_cursor, SKY_RING_PUBLICATION *context);
static void scene_sky_mode_four_draw_rings(MODE_FOUR_SCRATCH_PUBLICATION *context, uint32 ticks, uint16 camera_distance, uint32 *packet_cursor);
static void scene_sky_mode_four_draw_slices(MODE_FOUR_SCRATCH_PUBLICATION *context, const uint32 *camera_position, uint16 camera_distance, uint32 *packet_cursor);
static void scene_sky_mode_four_project_ring(const uint32 *radial, MODE_FOUR_VERTEX *out, uint32 distance);
static void scene_sky_mode_four_project_slice(const uint32 *origin, MODE_FOUR_VERTEX *out, uint32 step);
static void scene_sky_project_ring(SKY_RING_VERTEX *ring, const uint32 *radial, uint32 level);

uint32 scene_light_draw_pass(uint32 ordering, uint32 packet)
{
    SCENE_LIGHT_PASS pass;
    SCENE_LIGHT_PROJECTION staging;
    SCENE_LIGHT_PROJECTION *projection;
    SCENE_LIGHT_PROJECTION *scratch_projection;
    uint32 selective_projection;
    uint32 packet_cursor, color_index;
    uint32 mode, count, value, tail, flag, seed;
    FUNCTION_MARKER(0x800957A4, "ACE3.BIN");
    SCENE_LIGHT_CONTEXT_PUBLICATION *context = (SCENE_LIGHT_CONTEXT_PUBLICATION *)getScratchAddr(7);
    SKY_PASS_PUBLICATION *state = (SKY_PASS_PUBLICATION *)getScratchAddr(0);
    selective_projection = (packet >= 0x80000000u && packet <= 0x801FE000u) || mission_native_packet_span(packet, sizeof(uint32)) != NULL || ui_native_packet_span(packet, sizeof(uint32)) != NULL;
    state->ordering = ordering;
    packet_cursor = packet + 8;
    scratch_projection = &context->light.projection;
    /* exceptional overlapping packet storage retains the original staging path */
    projection = selective_projection ? scratch_projection : &staging;
    if (!selective_projection)
        memcpy(projection, scratch_projection, sizeof(*projection));
    scene_light_initialize_projection(state, context, &pass, projection);
    mode = (sint16)game_runtime.selection.mission;
    pass.mission_six = mode == 6;
    context->light.mission_six = pass.mission_six;
    scene_light_update_colors(&pass);
    count = scene_light_build_horizon(&pass, projection);
    scene_light_project_rows(projection, count);
    scene_light_measure_projected_areas(projection);
    scene_light_emit_layers(context, &pass, &packet_cursor, projection);
    if (!selective_projection)
        memcpy(scratch_projection, projection, sizeof(*projection));
    for (color_index = 0; color_index < 6; ++color_index)
        context->light.colors[color_index] = pass.colors[color_index];
    context->light.altitude_limit = pass.altitude_limit;
    value = (sint16)game_runtime.render_settings.terrain_flags;
    if ((value & 0x200) != 0)
        scene_light_draw_sky_layer(context, &packet_cursor, &pass);
    value = pass.effect;
    flag = (value & 4) != 0;
    tail = packet;
    value = packet_cursor;
    if (value == packet + 8)
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = 0x01000000;
        mask_packet->code[0] = flag | 0xE6000000;
        seed = packet + 8;
    }
    else
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = ((packet + 8) & 0xFFFFFF) | 0x01000000;
        mask_packet->code[0] = 0xE6000001;
        tail = packet_cursor;
        DR_TPAGE *restore_packet = mission_packet_span(tail, sizeof(*restore_packet));
        restore_packet->tag = 0x01000000;
        restore_packet->code[0] = flag | 0xE6000000;
        seed = 0x01000000;
    }
    value = effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, tail, seed);
    state->cursor = tail + 8;
    effect_light.flags = (uint32)(0);
    return tail + 8;
}

static void scene_light_draw_sky_layer(SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 *packet_cursor, const SCENE_LIGHT_PASS *pass)
{
    uint16 band_x, band_y[2];
    uint32 projected[2];
    uint32 color, height, distance, intensity, wide, page, a, b, c, d, e, angle, span, value, offset, packet, remaining, column, active, visible, x, y, u, v, next, original, result, early;
    FUNCTION_MARKER(0x80095930, "ACE3.BIN");
    color = game_runtime.scene_assets.environment.horizon_color;
    height = (uint32)pass->camera_position[1];
    wide = game_runtime.scene_assets.environment.wide_horizon;
    distance = (uint32)(sint32)game_runtime.scene_assets.environment.horizon_height - height;
    intensity = (sint32)(distance - 4000) >> 3;
    if ((sint32)intensity >= 4096)
        return;
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    e = (uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    page = (sint32)color >> 24;
    page = page == 0 ? 0x39 : 0x59;
    xport_gte_write_data(8, intensity);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    color = (color & 0xFFFFFF) | 0x2E000000;
    xport_gte_write_data(6, color);
    xport_gte_execute(0x780010);
    value = (sint16)pass->camera_distance;
    span = (uint32)ratan2(200, value);
    value = pass->camera_angles[1];
    angle = ((uint32)((sint32)(value << 16) >> 18) - span) & 0xF80;
    color = xport_gte_read_data(22);
    offset = wide != 0 ? 0xFFFFE800 : 0xFFFFF400;
    band_y[0] = ((sint32)(distance + offset) >> 2);
    band_y[1] = ((sint32)(distance - offset) >> 2);
    value = (uint16)rsin(((angle << 2)) >> 2) << 2;
    band_x = value;
    value = rcos(((angle << 2)) >> 2) << 2;
    a = (uint32)band_x | ((uint32)band_y[0] << 16);
    xport_gte_write_data(0, a);
    a = (uint32)band_x | ((uint32)band_y[1] << 16);
    xport_gte_write_data(2, a);
    xport_gte_write_data(1, value);
    xport_gte_write_data(3, value);
    xport_gte_write_data(4, 0);
    xport_gte_write_data(5, 0);
    active = 0;
    early = 0;
    packet = *packet_cursor;
    xport_gte_execute(0x280030);
    remaining = (uint32)((sint32)span >> 6) + 2;
    column = (uint32)((sint32)angle >> 7) - 1;
    value = xport_gte_read_data(12);
    projected[0] = value;
    value = xport_gte_read_data(13);
    projected[1] = value;
    while ((sint32)remaining > 0)
    {
        angle = (angle + 128) & 0xFFF;
        value = (uint16)rsin(((angle << 2)) >> 2) << 2;
        band_x = value;
        value = rcos(((angle << 2)) >> 2) << 2;
        a = (uint32)band_x | ((uint32)band_y[0] << 16);
        xport_gte_write_data(0, a);
        a = (uint32)band_x | ((uint32)band_y[1] << 16);
        xport_gte_write_data(2, a);
        xport_gte_write_data(1, value);
        xport_gte_write_data(3, value);
        next = column + 1;
        column = next & 15;
        xport_gte_execute(0x280030);
        value = projected[0];
        POLY_FT4 *quad = mission_packet_span(packet, 38);
        quad->p[0].xy.xy = value;
        b = projected[1];
        u = (next & 3) << 6;
        quad->p[2].uv.u = (uint8)(u);
        quad->p[0].uv.u = (uint8)(u);
        quad->p[3].uv.u = (uint8)(u + 63);
        quad->p[1].uv.u = (uint8)(u + 63);
        quad->p[2].xy.xy = b;
        if (wide != 0)
        {
            value = (column << 4) & 64;
            v = value - 128;
            value -= 65;
        }
        else
        {
            value = (column << 3) & 96;
            v = value - 128;
            value -= 97;
        }
        quad->p[1].uv.v = (uint8)(v);
        quad->p[0].uv.v = (uint8)(v);
        quad->p[3].uv.v = (uint8)(value);
        quad->p[2].uv.v = (uint8)(value);
        value = xport_gte_read_data(12);
        projected[0] = value;
        value = xport_gte_read_data(13);
        projected[1] = value;
        x = (sint16)projected[0];
        y = (sint16)projected[1];
        if ((sint32)x < 0)
            x = 0 - x;
        if ((sint32)y < 0)
            y = 0 - y;
        visible = (sint32)(x + y) < 361;
        if (visible != 0)
        {
            x = (sint16)(projected[0] >> 16);
            y = (sint16)(projected[1] >> 16);
            if ((sint32)x < 0)
                x = 0 - x;
            if ((sint32)y < 0)
                y = 0 - y;
            visible = (sint32)(x + y) < 281;
        }
        if (visible == 0)
        {
            if (active == 0)
            {
                original = *packet_cursor;
                if (packet != original)
                {
                    early = 1;
                    break;
                }
                --remaining;
                continue;
            }
            active = 0;
        }
        else
            active = 1;
        quad = mission_packet_span(packet, sizeof(*quad));
        next = packet + 40;
        quad->tag = (next & 0xFFFFFF) | 0x09000000;
        quad->rgbc.rgbc = color;
        quad->p[0].attr = (uint16)(0x4ED6);
        quad->p[1].attr = (uint16)(page);
        value = projected[0];
        quad->p[1].xy.xy = value;
        value = projected[1];
        quad->p[3].xy.xy = value;
        packet = next;
        --remaining;
    }
    /* publish the original packed endpoints and projected screen points */
    context->light.polygon.vertices[0] = (uint32)band_x | ((uint32)band_y[0] << 16);
    context->light.polygon.vertices[1] = (uint32)band_x | ((uint32)band_y[1] << 16);
    context->light.polygon.vertices[2] = projected[0];
    context->light.polygon.vertices[3] = projected[1];
    if (early == 0)
        original = *packet_cursor;
    result = 0x59;
    if (packet == original)
    {
        (void)(uint32)(result);
        return;
    }
    if (page == 0x59)
    {
        next = packet + 8;
        result = (next & 0xFFFFFF) | 0x01000000;
        DR_TPAGE *end_mode = mission_packet_span(packet, sizeof(*end_mode));
        end_mode->tag = result;
        end_mode->code[0] = 0xE1000220;
        packet = next;
    }
    *packet_cursor = packet;
    {
        (void)(uint32)(result);
        return;
    }
}

static void scene_light_emit_clipped_band(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 index, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass)
{
    uint32 polygon[SCENE_LIGHT_POLYGON_CAPACITY + SCENE_LIGHT_POLYGON_COLOR_OFFSET];
    uint32 *vertices = polygon;
    uint32 *colors = polygon + SCENE_LIGHT_POLYGON_COLOR_OFFSET;
    uint32 current_color, previous_color, first, second, area, value, count, corner, low0, low1;
    uint32 numerator, ratio, packet, next;
    FUNCTION_MARKER(0x8009546C, "ACE3.BIN");
    current_color = pass->colors[index];
    previous_color = pass->colors[(index - 1)];
    xport_gte_write_control(21, (current_color & 255) << 4);
    xport_gte_write_control(22, ((uint32)((sint32)current_color >> 4)) & 0xFF0);
    xport_gte_write_control(23, ((uint32)((sint32)current_color >> 12)) & 0xFF0);
    xport_gte_write_data(6, previous_color);
    value = projection->source[index - 1][0].word;
    xport_gte_write_data(12, value);
    value = projection->source[index - 1][1].word;
    xport_gte_write_data(13, value);
    value = projection->source[index][0].word;
    xport_gte_write_data(14, value);
    first = projection->depth_or_code[index];
    second = projection->depth_or_code[index + 1];
    xport_gte_execute(0x1400006);
    area = xport_gte_read_data(24);
    area = ((sint32)area >> 4);
    count = 0;
    for (corner = 0; corner < 4; ++corner)
    {
        low0 = first & 15;
        low1 = second & 15;
        if (((first | second) & 3) == 3)
        {
            value = projection->bounds[corner].word;
            vertices[count] = value;
            value = 0;
            if (area)
                value = projection->area[index - 1][corner];
            if (area && (sint32)value > 0)
            {
                numerator = value << 8;
                if (area == 0xFFFFFFFF && numerator == 0x80000000)
                    xport_mips_break(6);
                ratio = ((sint32)numerator / (sint32)area);
                xport_gte_write_data(8, ratio);
                xport_gte_execute(0x780010);
                value = xport_gte_read_data(22);
                colors[count] = value;
            }
            else
                colors[count] = previous_color;
            ++count;
        }
        if (low0 == 9)
        {
            value = projection->screen[index - 1][1].word;
            colors[count] = previous_color;
            vertices[count] = value;
            ++count;
        }
        if (low1 == 9)
        {
            value = projection->screen[index][1].word;
            colors[count] = current_color;
            vertices[count] = value;
            ++count;
        }
        if (low1 == 6)
        {
            value = projection->screen[index][0].word;
            colors[count] = current_color;
            vertices[count] = value;
            ++count;
        }
        if (low0 == 6)
        {
            value = projection->screen[index - 1][0].word;
            colors[count] = previous_color;
            vertices[count] = value;
            ++count;
        }
        first = ((sint32)first >> 2);
        second = ((sint32)second >> 2);
    }
    /* retain the final polygon words for overlapping scratchpad users */
    for (corner = 0; corner < count; ++corner)
        context->light.polygon.vertices[corner] = vertices[corner];
    for (corner = 0; corner < count; ++corner)
        context->light.polygon.gradient.colors[corner] = colors[corner];
    while (count >= 4)
    {
        packet = *packet_cursor;
        next = packet + 36;
        *packet_cursor = next;
        POLY_G4 *quad = mission_packet_span(packet, sizeof(*quad));
        quad->tag = (next & 0xFFFFFF) | 0x08000000;
        value = colors[0];
        quad->p[0].rgbc.rgbc = value | 0x38000000;
        value = vertices[0];
        quad->p[0].xy.xy = value;
        value = colors[count - 3];
        quad->p[1].rgbc.rgbc = value;
        value = vertices[count - 3];
        quad->p[1].xy.xy = value;
        value = colors[count - 1];
        quad->p[2].rgbc.rgbc = value;
        value = vertices[count - 1];
        quad->p[2].xy.xy = value;
        value = colors[count - 2];
        quad->p[3].rgbc.rgbc = value;
        value = vertices[count - 2];
        count -= 2;
        quad->p[3].xy.xy = value;
    }
    if (count != 3)
        return;
    packet = *packet_cursor;
    next = packet + 28;
    *packet_cursor = next;
    POLY_G3 *triangle = mission_packet_span(packet, sizeof(*triangle));
    triangle->tag = (next & 0xFFFFFF) | 0x06000000;
    value = colors[0];
    triangle->p[0].rgbc.rgbc = value | 0x30000000;
    value = vertices[0];
    triangle->p[0].xy.xy = value;
    value = colors[1];
    triangle->p[1].rgbc.rgbc = value;
    value = vertices[1];
    triangle->p[1].xy.xy = value;
    value = colors[2];
    triangle->p[2].rgbc.rgbc = value;
    value = vertices[2];
    triangle->p[2].xy.xy = value;
    {
        (void)(uint32)(value);
        return;
    }
}

static void scene_light_emit_noise(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 count)
{
    const uint16 *samples;
    uint32 base_x, base_y, base_z;
    uint16 sample_x, sample_y, sample_z;
    uint32 a, b, c, d, e, start, block, scale, x, y, z, color, channel;
    uint32 index, used, value, packed, depth, distance, point, next;
    sint32 coordinate;
    FUNCTION_MARKER(0x80095CB8, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    if ((sint32)count >= 257)
        count = 256;
    a = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][1] << 16));
    e = (uint32)(uint16)game_runtime.mission_matrices.camera.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x2000);
    start = *packet_cursor;
    block = start + 8;
    xport_gte_execute(0x486012);
    DR_TPAGE *mode = mission_packet_span(start, sizeof(*mode));
    mode->tag = (block & 0xFFFFFF) | 0x01000000;
    mode->code[0] = 0xE1000220;
    samples = effect_noise.sample;
    value = game_runtime.scene_assets.environment.noise_attenuation;
    scale = (128 - value) << 5;
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_control(21, 0x60);
    xport_gte_write_control(22, 0x70);
    xport_gte_write_control(23, 0x80);
    xport_gte_write_data(6, 0x6A807060);
    xport_gte_write_data(8, scale);
    x = 0 - x;
    y = 0 - y;
    base_x = x;
    base_y = y;
    xport_gte_execute(0x780010);
    z = 0 - z;
    base_z = z;
    sample_x = (uint16)((x & 0x1FFF) - 0x1000);
    sample_y = (uint16)((y & 0x1FFF) - 0x1000);
    sample_z = (uint16)((z & 0x1FFF) - 0x1000);
    color = xport_gte_read_data(22);
    xport_gte_write_data(6, color);
    channel = color >> 8;
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    e = (uint32)(uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0x2000);
    used = 0;
    for (index = 0; index < count; ++index)
    {
        value = (uint32)sample_x | ((uint32)sample_y << 16);
        xport_gte_write_data(0, value);
        value = sample_z;
        xport_gte_write_data(1, value);
        value = ((uint32)samples[0] | ((uint32)samples[1] << 16));
        samples += 2;
        xport_gte_execute(0x180001);
        x = (uint16)base_x;
        y = (uint16)base_y;
        x += value;
        value = (uint32)((sint32)value >> 8);
        y += value;
        value = (uint32)((sint32)value >> 8);
        sample_x = (uint16)((x & 0x1FFF) - 0x1000);
        z = (uint16)base_z;
        sample_y = (uint16)((y & 0x1FFF) - 0x1000);
        z += value;
        sample_z = (uint16)((z & 0x1FFF) - 0x1000);
        packed = xport_gte_read_data(14);
        depth = xport_gte_read_data(19);
        coordinate = (sint32)packed >> 16;
        if (coordinate < 0)
            coordinate = -coordinate;
        if (coordinate >= 121)
            continue;
        coordinate = (sint16)packed;
        if (coordinate < 0)
            coordinate = -coordinate;
        if (coordinate >= 161)
            continue;
        distance = depth - 0x2000;
        if ((sint32)distance < 0)
            distance = 0x2000 - depth;
        if ((sint32)distance >= 0x1001)
            continue;
        xport_gte_write_data(8, distance);
        point = block + used * 8 + 4;
        xport_gte_execute(0x780010);
        ++used;
        POINT_C *dot = mission_packet_span(point, sizeof(*dot));
        dot->xy.xy = packed;
        if (used >= 7)
        {
            next = block + 60;
            *(uint32 *)mission_packet_span(block, sizeof(uint32)) = (next & 0xFFFFFF) | 0x0E000000;
            block = next;
            used = 0;
        }
        value = xport_gte_read_data(22);
        dot->rgbc.rgbc = value;
        if ((index & 15) == 0)
            dot->rgbc.r = (uint8)channel;
    }
    /* publish the last generated point and the original base vector */
    SKY_NOISE_PUBLICATION *published = &context->noise;
    published->sample[0] = sample_x;
    published->sample[1] = sample_y;
    published->sample[2] = sample_z;
    published->base[0] = base_x;
    published->base[1] = base_y;
    published->base[2] = base_z;
    if (used)
    {
        next = block + used * 8 + 4;
        *packet_cursor = next;
        *(uint32 *)mission_packet_span(block, sizeof(uint32)) = (used << 25) | (next & 0xFFFFFF);
    }
    else if (start + 8 != block)
        *packet_cursor = block;
}

static void scene_light_blend_color(uint32 *native_color, uint32 target, uint32 scale)
{
    uint32 value, result;
    FUNCTION_MARKER(0x80095018, "ACE3.BIN");
    result = (sint32)scale < 0x1000;
    if ((sint32)scale <= 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = target & 255;
    if ((sint32)scale >= 0x1000)
    {
        *native_color = target;
        {
            (void)(uint32)(result);
            return;
        }
    }
    xport_gte_write_control(21, result << 4);
    target = ((sint32)target >> 8);
    value = (target & 255) << 4;
    xport_gte_write_control(22, value);
    result = ((uint32)((sint32)target >> 4)) & 0xFF0;
    xport_gte_write_control(23, result);
    value = *native_color;
    xport_gte_write_data(6, value);
    xport_gte_write_data(8, scale);
    xport_gte_execute(0x780010);
    value = xport_gte_read_data(22);
    *native_color = value;
    {
        (void)(uint32)(result);
        return;
    }
}

static void scene_light_initialize_projection(SKY_PASS_PUBLICATION *state, SCENE_LIGHT_CONTEXT_PUBLICATION *context, SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection)
{
    MATRIX rotation = {0};
    uint16 rotation_angles[3];
    uint32 value, flags, row, column;
    FUNCTION_MARKER(0x80094718, "ACE3.BIN");
    for (row = 0; row < 3; ++row)
        pass->camera_position[row] = (sint32)game_runtime.camera_view.pose.position[row] >> 12;
    for (row = 0; row < 3; ++row)
        pass->camera_angles[row] = game_runtime.camera_view.pose.angles[row];
    pass->camera_distance = (uint16)game_runtime.camera_proj.distance;
    rotation_angles[0] = pass->camera_angles[0];
    rotation_angles[1] = 0;
    rotation_angles[2] = pass->camera_angles[2];
    game_build_rotation_via_gte(&rotation, rotation_angles);
    SetRotMatrix(&rotation);
    /* retain the camera lanes and rotation bytes for overlapping scratchpad users */
    for (row = 0; row < 3; ++row)
    {
        state->position[row] = (uint32)pass->camera_position[row];
        state->angles[row] = pass->camera_angles[row];
        for (column = 0; column < 3; ++column)
            context->light.matrix[row][column] = (uint16)rotation.m[row][column];
    }
    state->distance = pass->camera_distance;
    context->light.rotation_pitch = rotation_angles[0];
    context->light.rotation_roll = rotation_angles[2];
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    projection->mask = 0;
    projection->bounds[3].half[0] = 0xFF60;
    projection->bounds[0].half[0] = 0xFF60;
    projection->bounds[2].half[0] = 160;
    projection->bounds[1].half[0] = 160;
    projection->bounds[1].half[1] = 0xFF88;
    projection->bounds[0].half[1] = 0xFF88;
    projection->bounds[3].half[1] = 120;
    projection->bounds[2].half[1] = 120;
    flags = effect_light.flags;
    pass->effect = flags;
    context->light.effect = pass->effect;
    if (flags & 1)
        value = 1;
    else
    {
        value = (game_runtime.render_settings.terrain_flags & 0x400) != 0;
    }
    pass->horizon_extension = (uint8)value;
    context->light.horizon_extension = pass->horizon_extension;
    return;
}

static void scene_light_project_rows(SCENE_LIGHT_PROJECTION *projection, uint32 count)
{
    uint32 index, flags, value, z;
    FUNCTION_MARKER(0x800948A0, "ACE3.BIN");
    xport_gte_write_data(4, 0);
    xport_gte_write_data(5, 0);
    flags = 1;
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        z = projection->depth_or_code[index];
        value = projection->source[index][0].word;
        xport_gte_write_data(0, value);
        value = projection->source[index][1].word;
        xport_gte_write_data(2, value);
        xport_gte_write_data(1, z);
        xport_gte_write_data(3, z);
        xport_gte_execute(0x280030);
        if (flags == 0)
            scene_geometry_clip_line(projection->screen[index - 1][0].half, projection->screen[index - 1][1].half, 160, 120);
        flags = xport_gte_read_control(31) & 0x6000;
        if (flags == 0)
        {
            value = xport_gte_read_data(12);
            projection->source[index][0].word = value;
            value = xport_gte_read_data(13);
            projection->source[index][1].word = value;
            value = xport_gte_read_data(12);
            projection->screen[index][0].word = value;
            value = xport_gte_read_data(13);
            projection->screen[index][1].word = value;
            projection->mask |= (uint32)1 << index;
        }
    }
    if (flags == 0)
        scene_geometry_clip_line(projection->screen[index - 1][0].half, projection->screen[index - 1][1].half, 160, 120);
}

static void scene_light_measure_projected_areas(SCENE_LIGHT_PROJECTION *projection)
{
    uint32 index, mask, corner, value, packed, code;
    FUNCTION_MARKER(0x800949AC, "ACE3.BIN");
    index = 0;
    mask = (sint16)projection->mask;
    projection->depth_or_code[0] = 0xAAA;
    while (mask && index < 5)
    {
        projection->depth_or_code[index + 1] = 0;
        if (mask & 1)
        {
            value = projection->source[index][0].word;
            xport_gte_write_data(12, value);
            value = projection->source[index][1].word;
            xport_gte_write_data(13, value);
            for (corner = 0; corner < 4; ++corner)
            {
                value = projection->bounds[corner].word;
                xport_gte_write_data(14, value);
                xport_gte_execute(0x1400006);
                value = xport_gte_read_data(24);
                projection->area[index][corner] = value;
                code = value == 0 ? 3 : (sint32)value > 0 ? 2 : 1;
                packed = projection->depth_or_code[index + 1];
                projection->depth_or_code[index + 1] = packed | (code << (corner * 2));
            }
            packed = projection->depth_or_code[index + 1];
            projection->depth_or_code[index + 1] = packed | (packed << 8);
        }
        mask = ((sint32)mask >> 1);
        ++index;
    }
    ++index;
    while (index < 7)
    {
        projection->depth_or_code[index] = 0x555;
        ++index;
    }
    return;
}

static uint32 scene_light_build_horizon(SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection)
{
    uint32 altitude, shift, value, divisor, width, index, a, b, limit, result;
    FUNCTION_MARKER(0x80094AC8, "ACE3.BIN");
    altitude = 0 - (uint32)pass->camera_position[1];
    shift = (sint32)altitude > 0x8FFE ? 3 : 2;
    value = game_runtime.render_settings.layer_height[1];
    projection->source[0][0].half[1] = (uint16)((sint32)(altitude + (value << 1)) >> shift);
    value = game_runtime.render_settings.layer_height[0];
    projection->source[1][0].half[1] = (uint16)((sint32)(altitude + (value << 1)) >> shift);
    value = game_runtime.render_settings.horizon_height;
    projection->source[2][0].half[1] = (uint16)((sint32)(altitude + (value << 1)) >> shift);
    value = (uint16)game_runtime.render_settings.horizon_height;
    projection->source[4][0].half[1] = (uint16)((sint32)altitude >> shift);
    value = ((sint32)(value << 16) >> 17);
    projection->source[3][0].half[1] = (uint16)((sint32)(altitude + value) >> shift);
    divisor = (sint16)pass->camera_distance;
    if (divisor == 0)
        xport_mips_break(7);
    width = ((sint32)0x3BFF88 / (sint32)divisor);
    for (index = 0; index < 5; ++index)
    {
        value = projection->source[index][0].half[1];
        projection->source[index][0].half[0] = (uint16)(0 - width);
        projection->source[index][1].half[0] = (uint16)(width);
        projection->source[index][1].half[1] = (uint16)(value);
        if (index == 2)
            width = ((sint32)width >> 1);
    }
    value = 0x7FFF >> (shift - 1);
    projection->depth_or_code[1] = value;
    projection->depth_or_code[0] = value;
    value = 0x7800 >> (shift - 1);
    projection->depth_or_code[2] = value;
    a = pass->mission_six;
    projection->depth_or_code[3] = 0x7800 >> shift;
    if (a)
    {
        value = projection->source[4][0].half[1];
        pass->altitude_limit = 0x6800;
        value += 0xC00;
        projection->source[4][1].half[1] = (uint16)(value);
        projection->source[4][0].half[1] = (uint16)(value);
    }
    else if (pass->horizon_extension)
    {
        a = ((sint16)game_runtime.camera_proj.near_plane);
        b = ((sint16)game_runtime.camera_proj.far_plane);
        value = ((sint32)(a + b) >> 1);
        pass->altitude_limit = value;
        if ((sint32)value < 0x3400)
        {
            a = projection->source[4][0].half[0];
            b = projection->source[4][1].half[0];
            a = ((sint32)(a << 16) >> 17);
            b = ((sint32)(b << 16) >> 17);
            projection->source[4][0].half[0] = (uint16)(a);
            projection->source[4][1].half[0] = (uint16)(b);
        }
    }
    else
        pass->altitude_limit = 0x6800;
    limit = pass->altitude_limit;
    result = 4;
    if ((sint32)altitude < (sint32)limit)
    {
        limit = pass->altitude_limit;
        xport_gte_write_data(9, limit);
        xport_gte_write_data(10, altitude);
        result = 5;
        xport_gte_execute(0xA00428);
        a = xport_gte_read_data(25);
        b = xport_gte_read_data(26);
        value = game_square_root_with_lzcs(a - b);
        projection->depth_or_code[4] = (sint32)value >> shift;
    }
    return result;
}

static void scene_light_adjust_altitude_color(uint32 *color, uint32 altitude)
{
    uint32 value, scale, result;
    FUNCTION_MARKER(0x80094F50, "ACE3.BIN");
    if ((sint32)altitude > -41192)
    {
        value = *color;
        xport_gte_write_data(6, value);
        scale = ((sint32)(0xFFFF7F18 - altitude) >> 1);
    }
    else
    {
        xport_gte_write_data(6, 0);
        scale = 0x1000 - (uint32)((sint32)(0xFFFF5F18 - altitude) >> 3);
    }
    xport_gte_write_data(8, scale);
    value = (uint8)*color - 96;
    result = value << 4;
    xport_gte_write_control(21, (sint32)value < 0 ? 0 : result);
    value = (uint8)(*color >> 8) - 96;
    result = value << 4;
    xport_gte_write_control(22, (sint32)value < 0 ? 0 : result);
    value = (uint8)(*color >> 16) - 32;
    result = value << 4;
    xport_gte_write_control(23, (sint32)value < 0 ? 0 : result);
    xport_gte_execute(0x780010);
    value = xport_gte_read_data(22);
    *color = value;
    {
        (void)(uint32)(result);
        return;
    }
}

static void scene_light_update_colors(SCENE_LIGHT_PASS *pass)
{
    uint32 value, altitude, angle, a, b, index, intensity, result, shifted;
    FUNCTION_MARKER(0x80094D10, "ACE3.BIN");
    value = game_runtime.render_settings.horizon_color;
    pass->colors[0] = value | 0x30303;
    altitude = (uint32)pass->camera_position[1];
    if ((sint32)altitude < -33000)
        scene_light_adjust_altitude_color(&pass->colors[0], altitude);
    value = pass->camera_angles[1];
    a = game_runtime.render_settings.light_angles[0][1];
    value = ((sint32)(value << 16) >> 17);
    angle = ((a << 1) - value) & 0x1FFF;
    if (angle >= 0x1001)
        angle = 0x2000 - angle;
    value = game_runtime.render_settings.blend_start[0];
    pass->colors[1] = value;
    value = game_runtime.render_settings.blend_start[1];
    pass->colors[2] = value;
    value = game_runtime.render_settings.blend_end[0];
    pass->colors[3] = value;
    value = game_runtime.render_settings.blend_end[1];
    pass->colors[4] = value;
    if (angle >= 0x1000)
    {
        a = pass->colors[3];
        b = pass->colors[4];
        pass->colors[1] = a;
        pass->colors[2] = b;
    }
    else if (angle)
    {
        for (index = 0; index < 2; ++index)
        {
            a = pass->colors[1 + index];
            b = pass->colors[3 + index];
            if (a != b)
                scene_light_blend_color(&pass->colors[1 + index], b, angle);
        }
    }
    value = game_runtime.render_settings.palette_primary;
    pass->colors[4] = value;
    pass->colors[3] = value;
    value = game_runtime.render_settings.palette_secondary;
    result = pass->effect & 2;
    pass->colors[5] = value;
    if (result == 0)
        return;
    intensity = effect_light.intensity;
    if (intensity == 0)
        return;
    xport_gte_write_data(6, 0);
    xport_gte_write_data(8, intensity);
    for (index = 0; index < 3; ++index)
    {
        value = pass->colors[index];
        xport_gte_write_data(9, 0x5B0);
        xport_gte_write_data(10, 0x5B0);
        xport_gte_write_data(11, 0x5B0);
        xport_gte_write_data(25, (value & 255) << 4);
        shifted = ((sint32)value >> 8);
        xport_gte_write_data(26, (shifted & 255) << 4);
        xport_gte_write_data(27, ((uint32)((sint32)shifted >> 4)) & 0xFF0);
        xport_gte_execute(0x1A8003E);
        value = xport_gte_read_data(22);
        pass->colors[index] = value;
    }
    return;
}

static void scene_light_emit_layers(SCENE_LIGHT_CONTEXT_PUBLICATION *context, SCENE_LIGHT_PASS *pass, uint32 *packet_cursor, const SCENE_LIGHT_PROJECTION *projection)
{
    uint32 extra, mask, value, angle, color, packet, next, altitude, limit;
    FUNCTION_MARKER(0x80095074, "ACE3.BIN");
    value = game_runtime.render_settings.terrain_flags;
    extra = 0;
    if (value & 0x100)
    {
        extra = game_runtime.scene_assets.environment.detail_repeat + 1;
    }
    mask = (sint16)projection->mask;
    if (mask)
    {
        if (mask & 1)
            scene_light_emit_visible_band(packet_cursor, context, 0, projection, pass);
        if (mask & 3)
            scene_light_emit_clipped_band(packet_cursor, context, 1, projection, pass);
        if (mask & 6)
            scene_light_emit_clipped_band(packet_cursor, context, 2, projection, pass);
        if (extra)
            scene_light_emit_noise(packet_cursor, context, extra);
        if (mask & 12)
            scene_light_emit_clipped_band(packet_cursor, context, 3, projection, pass);
        if (mask & 8)
        {
            if (pass->mission_six)
            {
                value = pass->colors[5];
                pass->colors[4] = value;
                scene_light_emit_clipped_band(packet_cursor, context, 4, projection, pass);
            }
            else
                scene_light_emit_visible_band(packet_cursor, context, 4, projection, pass);
        }
        if ((mask & 16) && pass->horizon_extension)
            scene_light_emit_visible_band(packet_cursor, context, 5, projection, pass);
        return;
    }
    value = (uint32)pass->camera_position[1];
    angle = (uint32)ratan2(value, 0x7FFF) << 2;
    value = (sint16)pass->camera_angles[0];
    if ((sint32)angle < (sint32)value)
        color = pass->colors[0];
    else
    {
        altitude = (uint32)pass->camera_position[1];
        limit = pass->altitude_limit;
        altitude = 0 - altitude;
        extra = 0;
        if ((sint32)altitude >= (sint32)limit)
            color = pass->colors[4];
        else
        {
            value = pass->horizon_extension;
            color = 0xFFFFFFFF;
            if (value)
                color = pass->colors[5];
        }
    }
    if ((sint32)color >= 0)
    {
        packet = *packet_cursor;
        next = packet + 16;
        TILE *fill = mission_packet_span(packet, sizeof(*fill));
        fill->tag = (next & 0xFFFFFF) | 0x03000000;
        fill->rgbc.rgbc = color | 0x60000000;
        value = projection->bounds[0].word;
        fill->wh = 0xF00140;
        fill->p[0].xy = value;
        *packet_cursor = next;
    }
    if (extra)
        scene_light_emit_noise(packet_cursor, context, extra);
}

static void scene_light_emit_visible_band(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, uint32 index, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass)
{
    uint32 vertices[SCENE_LIGHT_POLYGON_CAPACITY];
    uint32 count, corner, first, second, low0, low1, value, packet, next, color, quad_color, triangle_color;
    FUNCTION_MARKER(0x80095298, "ACE3.BIN");
    count = 0;
    first = projection->depth_or_code[index];
    second = projection->depth_or_code[index + 1];
    for (corner = 0; corner < 4; ++corner)
    {
        low0 = first & 15;
        low1 = second & 15;
        if (((first | second) & 3) == 3)
        {
            value = projection->bounds[corner].word;
            vertices[count] = value;
            ++count;
        }
        if (low0 == 9)
        {
            value = projection->screen[index - 1][1].word;
            vertices[count] = value;
            ++count;
        }
        if (low1 == 9)
        {
            value = projection->screen[index][1].word;
            vertices[count] = value;
            ++count;
        }
        if (low1 == 6)
        {
            value = projection->screen[index][0].word;
            vertices[count] = value;
            ++count;
        }
        if (low0 == 6)
        {
            value = projection->screen[index - 1][0].word;
            vertices[count] = value;
            ++count;
        }
        first = ((sint32)first >> 2);
        second = ((sint32)second >> 2);
    }
    /* retain the final polygon words for overlapping scratchpad users */
    for (corner = 0; corner < count; ++corner)
        context->light.polygon.vertices[corner] = vertices[corner];
    if (count < 3)
    {
        (void)(uint32)(index << 2);
        return;
    }
    color = pass->colors[index];
    quad_color = color | 0x28000000;
    triangle_color = color | 0x20000000;
    while (count >= 4)
    {
        packet = *packet_cursor;
        next = packet + 24;
        *packet_cursor = next;
        POLY_F4 *quad = mission_packet_span(packet, sizeof(*quad));
        quad->tag = (next & 0xFFFFFF) | 0x05000000;
        quad->rgbc.rgbc = quad_color;
        value = vertices[0];
        quad->p[0].xy = value;
        value = vertices[count - 3];
        quad->p[1].xy = value;
        value = vertices[count - 1];
        count -= 2;
        quad->p[2].xy = value;
        value = vertices[count];
        quad->p[3].xy = value;
    }
    if (count != 3)
        return;
    packet = *packet_cursor;
    next = packet + 20;
    *packet_cursor = next;
    POLY_F3 *triangle = mission_packet_span(packet, sizeof(*triangle));
    triangle->tag = (next & 0xFFFFFF) | 0x04000000;
    triangle->rgbc.rgbc = triangle_color;
    value = vertices[0];
    triangle->p[0].xy = value;
    value = vertices[1];
    triangle->p[1].xy = value;
    value = vertices[2];
    triangle->p[2].xy = value;
    {
        (void)(uint32)(value);
        return;
    }
}

static void scene_light_build_horizon_rows(SCENE_LIGHT_PASS *pass, SCENE_LIGHT_PROJECTION *projection)
{
    uint32 value, flags, far_limit, near_limit, divisor, numerator, far_width, near_width, index, height;
    FUNCTION_MARKER(0x80095FA0, "ACE3.BIN");
    value = game_runtime.render_settings.horizon_color;
    pass->colors[0] = value;
    value = game_runtime.render_settings.palette_primary;
    pass->colors[2] = value;
    pass->colors[1] = value;
    value = game_runtime.render_settings.palette_secondary;
    pass->colors[4] = value;
    pass->colors[3] = value;
    flags = game_runtime.render_settings.terrain_flags;
    if ((flags & 0x400) == 0)
    {
        flags = pass->effect;
        if ((flags & 1) == 0)
        {
            value = pass->colors[4];
            pass->colors[4] = value | 0x01000000;
        }
    }
    far_limit = ((sint16)game_runtime.camera_proj.far_plane);
    if ((sint32)far_limit >= 26625)
        far_limit = 26624;
    near_limit = ((sint16)game_runtime.camera_proj.near_plane);
    if ((sint32)near_limit < 1000)
        near_limit = 1000;
    far_limit = (sint32)far_limit >> 1;
    numerator = far_limit * 200;
    divisor = (sint16)pass->camera_distance;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    far_width = (sint32)numerator / (sint32)divisor;
    near_limit = (sint32)near_limit >> 1;
    numerator = near_limit * 200;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    near_width = (sint32)numerator / (sint32)divisor;
    index = 0;
    value = (uint32)pass->camera_position[1];
    height = (sint32)(0 - value - 3584) >> 1;
    do
    {
        if (index - 1 < 2)
        {
            projection->source[index][0].half[0] = (uint16)(0 - far_width);
            projection->source[index][1].half[0] = (uint16)(far_width);
            projection->depth_or_code[index] = far_limit;
        }
        else
        {
            projection->depth_or_code[index] = near_limit;
            projection->source[index][0].half[0] = (uint16)(0 - near_width);
            projection->source[index][1].half[0] = (uint16)(near_width);
        }
        projection->source[index][1].half[1] = (uint16)(height);
        projection->source[index][0].half[1] = (uint16)(height);
        if (index == 1)
        {
            value = (uint32)pass->camera_position[1];
            height = (sint32)(512 - value) >> 1;
        }
        index += 1;
    } while (index < 4);
    return;
}

static void scene_light_draw_horizon_bands(uint32 *packet_cursor, SCENE_LIGHT_CONTEXT_PUBLICATION *context, const SCENE_LIGHT_PROJECTION *projection, const SCENE_LIGHT_PASS *pass)
{
    uint32 flags, mode, result, packet, next, value;
    FUNCTION_MARKER(0x80096184, "ACE3.BIN");
    flags = (uint16)projection->mask;
    if (flags != 0)
    {
        mode = (sint8)(pass->colors[4] >> 24);
        if (mode == 0)
        {
            if ((flags & 1) != 0)
                scene_light_emit_visible_band(packet_cursor, context, 0, projection, pass);
            if ((flags & 8) != 0)
                scene_light_emit_visible_band(packet_cursor, context, 4, projection, pass);
            if ((flags & 1) != 0)
                scene_light_emit_clipped_band(packet_cursor, context, 1, projection, pass);
            if ((flags & 4) != 0)
                scene_light_emit_clipped_band(packet_cursor, context, 3, projection, pass);
        }
        result = flags & 6;
        if (result != 0)
        {
            (void)(scene_light_emit_visible_band(packet_cursor, context, 2, projection, pass));
            return;
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    result = (sint8)(pass->colors[4] >> 24);
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    packet = *packet_cursor;
    next = packet + 16;
    TILE *fill = mission_packet_span(packet, sizeof(*fill));
    fill->tag = (next & 0xFFFFFF) | 0x03000000;
    value = pass->colors[1];
    fill->rgbc.rgbc = value | 0x60000000;
    result = projection->bounds[0].word;
    fill->wh = 0x00F00140;
    fill->p[0].xy = result;
    *packet_cursor = next;
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_light_draw_horizon_pass(uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    SCENE_LIGHT_PASS pass;
    SCENE_LIGHT_PROJECTION staging;
    SCENE_LIGHT_PROJECTION *projection;
    SCENE_LIGHT_PROJECTION *scratch_projection;
    uint32 selective_projection;
    uint32 packet_cursor, color_index;
    uint32 start, flags, mode, current, tail, tag;
    FUNCTION_MARKER(0x800962B8, "ACE3.BIN");
    SCENE_LIGHT_CONTEXT_PUBLICATION *context = (SCENE_LIGHT_CONTEXT_PUBLICATION *)getScratchAddr(7);
    selective_projection = (packet >= 0x80000000u && packet <= 0x801FF000u) || mission_native_packet_span(packet, sizeof(uint32)) != NULL || ui_native_packet_span(packet, sizeof(uint32)) != NULL;
    start = packet + 8;
    SKY_PASS_PUBLICATION *published = (SKY_PASS_PUBLICATION *)getScratchAddr(0);
    published->ordering = ordering;
    packet_cursor = start;
    scratch_projection = &context->light.projection;
    /* exceptional overlapping packet storage retains the original staging path */
    projection = selective_projection ? scratch_projection : &staging;
    if (!selective_projection)
        memcpy(projection, scratch_projection, sizeof(*projection));
    scene_light_initialize_projection(published, context, &pass, projection);
    scene_light_build_horizon_rows(&pass, projection);
    scene_light_project_rows(projection, 4);
    scene_light_measure_projected_areas(projection);
    scene_light_draw_horizon_bands(&packet_cursor, context, projection, &pass);
    if (!selective_projection)
        memcpy(scratch_projection, projection, sizeof(*projection));
    uint32 *published_colors = context->light.colors;
    for (color_index = 0; color_index < 5; ++color_index)
        published_colors[color_index] = pass.colors[color_index];
    flags = pass.effect;
    mode = (flags & 4) != 0;
    current = packet_cursor;
    tail = packet;
    if (current == start)
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = 0x01000000;
        mask_packet->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(packet, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
    }
    else
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = (start & 0xFFFFFF) | 0x01000000;
        mask_packet->code[0] = 0xE6000001;
        tail = packet_cursor;
        DR_TPAGE *restore_packet = mission_packet_span(tail, sizeof(*restore_packet));
        restore_packet->tag = 0x01000000;
        restore_packet->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(tail, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
    }
    published->cursor = tail + 8;
    effect_light.flags = (uint32)(0);
    return tail + 8;
}

static void scene_sky_project_ring(SKY_RING_VERTEX *ring, const uint32 *radial, uint32 level)
{
    uint32 cosine, scale, negative, z, index, a, b, product, packed, flags, value;
    FUNCTION_MARKER(0x8009757C, "ACE3.BIN");
    if ((sint32)level < 9)
    {
        cosine = rcos(level * 100);
        scale = level * 508;
    }
    else
    {
        cosine = rcos(862);
        scale = 4096;
    }
    negative = 0 - cosine;
    z = cosine - (uint32)((sint32)cosine >> 2);
    index = 0;
    do
    {
        a = (sint16)(radial[index] >> 16);
        product = scale * (0 - a);
        b = (sint16)radial[index];
        b = scale * (0 - b);
        packed = ((uint32)((sint32)product >> 12) & 0xFFFF) | ((uint32)((sint32)b >> 12) << 16);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, z);
        a = (sint32)product >> 16;
        b = (sint32)packed >> 20;
        xport_gte_execute(0x180001);
        ring[index].u = (uint16)a;
        ring[index].v = (uint16)b;
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            ring[index].flags = 0xFFFFFFFF;
        else
        {
            value = xport_gte_read_data(14);
            ring[index].screen = value;
            xport_gte_write_data(1, negative);
            xport_gte_execute(0x108041B);
            value = game_screen_clip(ring[index].screen);
            ring[index].flags = value;
            value = xport_gte_read_data(22);
            ring[index].color = value;
        }
        index += 1;
    } while (index < SKY_RING_SEGMENTS);
    ring[SKY_RING_SEGMENTS] = ring[0];
    return;
}

static void scene_sky_draw_rings(uint32 *packet_cursor, SKY_RING_PUBLICATION *context)
{
    static const uint16 angles[3] = {0x3000, 0, 0};
    MATRIX rotation = {0}, light;
    uint32 center_screen = 0, center_color;
    uint32 *radial = context->radial;
    SKY_RING_VERTEX(*rings)[SKY_RING_VERTICES] = context->vertices;
    SKY_RING_VERTEX *old_ring, *new_ring;
    uint32 a, b, c, index, value, flags, center_flag, packet, fields, link, old_flag, prior_old, new_flag, prior_new, code, offset_x, offset_y, sum_x, sum_y, level, cursor, end;
    FUNCTION_MARKER(0x800976F8, "ACE3.BIN");
    rotation_build(&rotation, angles);
    MulMatrix0(&game_runtime.mission_lighting.directions, &rotation, &light);
    SetLightMatrix(&light);
    SetMulMatrix(&game_runtime.mission_matrices.view, &rotation);
    packet = *packet_cursor;
    center_color = (uint32)(uint16)rotation.m[0][0] | ((uint32)(uint16)rotation.m[0][1] << 16);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_control(13, 0x300);
    xport_gte_write_control(14, 0x380);
    xport_gte_write_control(15, 0x600);
    xport_gte_write_control(16, 0x1000);
    xport_gte_write_control(17, 0x10000000);
    xport_gte_write_control(18, 0);
    xport_gte_write_control(19, 0x1000);
    xport_gte_write_control(20, 0);
    xport_gte_write_data(6, 0x34838383);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xC00);
    xport_gte_execute(0x180001);
    index = 0;
    do
    {
        value = ((uint32)(uint16)rsin(((((index << 12) / 24) << 2)) >> 2) | ((uint32)(uint16)rcos(((((index << 12) / 24) << 2)) >> 2) << 16));
        radial[index] = value;
        index += 1;
    } while (index < SKY_RING_SEGMENTS);
    flags = xport_gte_read_control(31);
    if ((sint32)flags < 0)
        center_flag = 0xFFFFFFFF;
    else
    {
        center_screen = xport_gte_read_data(14);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, -4096);
        xport_gte_execute(0x108041B);
        center_flag = game_screen_clip(center_screen);
        center_color = xport_gte_read_data(22);
    }
    xport_gte_write_data(6, 0x3C838383);
    scene_sky_project_ring(rings[0], radial, 1);
    if (center_flag != 0xFFFFFFFF)
    {
        index = 0;
        old_flag = rings[0][0].flags;
        fields = packet + 37;
        link = (packet << 8) + 0x2800;
        do
        {
            prior_old = old_flag;
            old_flag = rings[0][index + 1].flags;
            if (prior_old != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (center_flag & prior_old & old_flag) == 0)
            {
                POLY_GT3 *triangle = mission_packet_span(packet, sizeof(*triangle));
                triangle->tag = (link >> 8) | 0x09000000;
                a = center_color;
                b = rings[0][index].color;
                c = rings[0][index + 1].color;
                triangle->p[0].rgbc.rgbc = a;
                triangle->p[1].rgbc.rgbc = b;
                triangle->p[2].rgbc.rgbc = c;
                c = rings[0][index + 1].screen;
                a = center_screen;
                b = rings[0][index].screen;
                triangle->p[2].xy.xy = c;
                triangle->p[0].xy.xy = a;
                triangle->p[1].xy.xy = b;
                a = (sint16)rings[0][index].v;
                b = (sint16)rings[0][index + 1].v;
                c = (sint16)rings[0][index + 1].u;
                sum_y = a + b;
                b = (sint16)rings[0][index].u;
                sum_x = b + c;
                code = 168;
                offset_x = 255;
                if ((sint32)sum_x >= 0)
                {
                    offset_x = 0;
                    code = 170;
                }
                offset_y = 0;
                if ((sint32)sum_y < 0)
                    offset_y = 255;
                else
                    code += 4;
                triangle->p[1].attr = (uint16)(code);
                triangle->p[0].uv_attr = (offset_y << 8) | offset_x | 0x3FF00000;
                value = (uint8)rings[0][index].u;
                triangle->p[1].uv.u = (uint8)(value + offset_x);
                value = (uint8)rings[0][index].v;
                triangle->p[1].uv.v = (uint8)(value + offset_y);
                value = (uint8)rings[0][index + 1].u;
                link += 0x2800;
                triangle->p[2].uv.u = (uint8)(value + offset_x);
                value = (uint8)rings[0][index + 1].v;
                packet += 40;
                triangle->p[2].uv.v = (uint8)(value + offset_y);
                fields += 40;
            }
            index += 1;
        } while (index < SKY_RING_SEGMENTS);
    }
    level = 1;
    do
    {
        new_ring = rings[(level + 1) & 1];
        old_ring = rings[level & 1];
        if (level == 7)
        {
            xport_gte_write_control(13, 0x500);
            xport_gte_write_control(14, 0x500);
            xport_gte_write_control(15, 0x500);
            xport_gte_write_control(16, 0x924);
            xport_gte_write_control(17, 0x09240000);
            xport_gte_write_control(19, 0x924);
            xport_gte_write_data(6, 0x3AA06858);
        }
        scene_sky_project_ring(old_ring, radial, level + 1);
        index = 0;
        fields = packet + 49;
        link = (packet << 8) + 0x3400;
        new_flag = new_ring[0].flags;
        old_flag = old_ring[0].flags;
        do
        {
            prior_new = new_flag;
            prior_old = old_flag;
            new_flag = new_ring[index + 1].flags;
            old_flag = old_ring[index + 1].flags;
            if (prior_new != 0xFFFFFFFF && prior_old != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (prior_new & prior_old & new_flag & old_flag) == 0)
            {
                POLY_GT4 *quad = mission_packet_span(packet, sizeof(*quad));
                quad->tag = (link >> 8) | 0x0C000000;
                a = new_ring[index].color;
                b = new_ring[index + 1].color;
                quad->p[0].rgbc.rgbc = a;
                quad->p[1].rgbc.rgbc = b;
                if (level < 7)
                {
                    a = old_ring[index].color;
                    b = old_ring[index + 1].color;
                    quad->p[2].rgbc.rgbc = a;
                    quad->p[3].rgbc.rgbc = b;
                }
                else
                {
                    quad->p[3].rgbc.rgbc = 0;
                    quad->p[2].rgbc.rgbc = 0;
                }
                a = new_ring[index].screen;
                b = new_ring[index + 1].screen;
                quad->p[0].xy.xy = a;
                quad->p[1].xy.xy = b;
                a = old_ring[index].screen;
                b = old_ring[index + 1].screen;
                quad->p[2].xy.xy = a;
                quad->p[3].xy.xy = b;
                a = (sint16)old_ring[index].v;
                b = (sint16)old_ring[index + 1].v;
                c = (sint16)old_ring[index + 1].u;
                sum_y = a + b;
                b = (sint16)old_ring[index].u;
                sum_x = b + c;
                code = 168;
                offset_x = 255;
                if ((sint32)sum_x >= 0)
                {
                    offset_x = 0;
                    code = 170;
                }
                offset_y = 0;
                if ((sint32)sum_y < 0)
                    offset_y = 255;
                else
                    code += 4;
                quad->p[0].attr = (uint16)(0x3FF0);
                quad->p[1].attr = (uint16)(code);
                value = (uint8)new_ring[index].u;
                quad->p[0].uv.u = (uint8)(value + offset_x);
                value = (uint8)new_ring[index].v;
                quad->p[0].uv.v = (uint8)(value + offset_y);
                value = (uint8)new_ring[index + 1].u;
                quad->p[1].uv.u = (uint8)(value + offset_x);
                value = (uint8)new_ring[index + 1].v;
                quad->p[1].uv.v = (uint8)(value + offset_y);
                value = (uint8)old_ring[index].u;
                quad->p[2].uv.u = (uint8)(value + offset_x);
                value = (uint8)old_ring[index].v;
                quad->p[2].uv.v = (uint8)(value + offset_y);
                value = (uint8)old_ring[index + 1].u;
                link += 0x3400;
                quad->p[3].uv.u = (uint8)(value + offset_x);
                value = (uint8)old_ring[index + 1].v;
                packet += 52;
                quad->p[3].uv.v = (uint8)(value + offset_y);
                fields += 52;
            }
            index += 1;
        } while (index < SKY_RING_SEGMENTS);
        level += 1;
    } while (level < 8);
    end = packet + 8;
    old_ring = rings[0];
    new_ring = rings[1];
    index = 0;
    DR_MODE *mode = mission_packet_span(packet, 8);
    mode->tag = (end & 0xFFFFFF) | 0x01000000;
    mode->code[0] = 0xE1000220;
    old_flag = old_ring[0].flags;
    new_flag = new_ring[0].flags;
    do
    {
        prior_old = old_flag;
        prior_new = new_flag;
        old_flag = old_ring[index + 1].flags;
        new_flag = new_ring[index + 1].flags;
        if (prior_old != 0xFFFFFFFF && prior_new != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && (prior_old & prior_new & old_flag & new_flag) == 0)
        {
            POLY_G4 *quad = mission_packet_span(end, sizeof(*quad));
            cursor = end + 36;
            quad->tag = (cursor & 0xFFFFFF) | 0x08000000;
            quad->p[0].rgbc.rgbc = 0x3A000000;
            quad->p[1].rgbc.rgbc = 0;
            a = new_ring[index].color;
            b = new_ring[index + 1].color;
            quad->p[2].rgbc.rgbc = a;
            quad->p[3].rgbc.rgbc = b;
            a = old_ring[index].screen;
            b = old_ring[index + 1].screen;
            quad->p[0].xy.xy = a;
            quad->p[1].xy.xy = b;
            a = new_ring[index].screen;
            b = new_ring[index + 1].screen;
            quad->p[2].xy.xy = a;
            quad->p[3].xy.xy = b;
            end = cursor;
        }
        index += 1;
    } while (index < SKY_RING_SEGMENTS);
    packet = end;
    scene_sky_project_ring(old_ring, radial, 9);
    index = 0;
    new_flag = new_ring[0].flags;
    old_flag = old_ring[0].flags;
    do
    {
        prior_new = new_flag;
        prior_old = old_flag;
        new_flag = new_ring[index + 1].flags;
        old_flag = old_ring[index + 1].flags;
        if (prior_new != 0xFFFFFFFF && prior_old != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && (prior_new & prior_old & new_flag & old_flag) == 0)
        {
            SKY_RING_BLEND_PACKET *pair = mission_packet_span(packet, 2 * sizeof(*pair));
            end = packet + 80;
            pair[0].tag = ((packet + 40) & 0xFFFFFF) | 0x09000000;
            pair[1].tag = (end & 0xFFFFFF) | 0x09000000;
            pair[0].draw_mode = 0xE1000240;
            pair[1].draw_mode = 0xE1000220;
            pair[1].p[0].rgbc.rgbc = 0x3A010101;
            pair[0].p[0].rgbc.rgbc = 0x3A010101;
            pair[1].p[1].rgbc.rgbc = 0x00010101;
            pair[0].p[1].rgbc.rgbc = 0x00010101;
            pair[0].p[3].rgbc.rgbc = 0x00939393;
            pair[0].p[2].rgbc.rgbc = 0x00939393;
            a = new_ring[index].color;
            b = new_ring[index + 1].color;
            pair[1].p[2].rgbc.rgbc = a;
            pair[1].p[3].rgbc.rgbc = b;
            a = old_ring[index].screen;
            b = old_ring[index + 1].screen;
            pair[1].p[0].xy.xy = a;
            pair[0].p[0].xy.xy = a;
            pair[1].p[1].xy.xy = b;
            pair[0].p[1].xy.xy = b;
            a = new_ring[index].screen;
            b = new_ring[index + 1].screen;
            pair[1].p[2].xy.xy = a;
            pair[0].p[2].xy.xy = a;
            pair[1].p[3].xy.xy = b;
            pair[0].p[3].xy.xy = b;
            packet = end;
        }
        index += 1;
    } while (index < SKY_RING_SEGMENTS);
    *packet_cursor = packet;
    SKY_CENTER_PUBLICATION *published = &context->center;
    published->flags = center_flag;
    if (center_flag == 0xFFFFFFFF)
        published->screen.x = 0;
    else
        published->screen.xy = center_screen;
    published->color = center_color;
    return;
}

uint32 scene_geometry_draw_sky_mode_three(uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    uint32 start, cursor, value, flags, mode, current, tail, tag;
    FUNCTION_MARKER(0x80097EB4, "ACE3.BIN");
    SKY_MODE_THREE_PUBLICATION *scratch = (SKY_MODE_THREE_PUBLICATION *)getScratchAddr(0);
    SCENE_LIGHT_CONTEXT_PUBLICATION *context = &scratch->context.light;
    start = packet + 8;
    cursor = packet + 24;
    SKY_PASS_PUBLICATION *published = &scratch->frame;
    published->ordering = ordering;
    TILE *fill = mission_packet_span(packet + 8, sizeof(*fill));
    fill->tag = (cursor & 0xFFFFFF) | 0x03000000;
    fill->p[0].xy = 0xFF88FF60;
    fill->wh = 0x00F00140;
    fill->rgbc.rgbc = 0x60000000;
    value = game_runtime.camera_view.pose.angles[0];
    published->angles[0] = (uint16)value;
    value = game_runtime.camera_view.pose.angles[1];
    published->angles[1] = (uint16)value;
    value = game_runtime.camera_view.pose.angles[2];
    published->angles[2] = (uint16)value;
    value = game_runtime.camera_proj.distance;
    published->distance = (uint16)value;
    scene_light_emit_noise(&cursor, context, 256);
    scene_sky_draw_rings(&cursor, &scratch->context.rings);
    published->cursor = cursor;
    flags = effect_light.flags;
    mode = (flags & 4) != 0;
    current = cursor;
    tail = packet;
    if (current == start)
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = 0x01000000;
        mask_packet->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(packet, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
    }
    else
    {
        DR_TPAGE *mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
        mask_packet->tag = (start & 0xFFFFFF) | 0x01000000;
        mask_packet->code[0] = 0xE6000001;
        tail = cursor;
        DR_TPAGE *restore_packet = mission_packet_span(tail, sizeof(*restore_packet));
        restore_packet->tag = 0x01000000;
        restore_packet->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(tail, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
    }
    effect_light.flags = (uint32)(0);
    return tail + 8;
}

static void scene_sky_mode_four_project_ring(const uint32 *radial, MODE_FOUR_VERTEX *out, uint32 distance)
{
    uint32 x, y, z, index, value, flags;
    FUNCTION_MARKER(0x8009830C, "ACE3.BIN");
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, distance);
    xport_gte_execute(0x486012);
    z = xport_gte_read_data(27);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    if ((sint32)z < -2048)
    {
        for (index = MODE_FOUR_RING_POINTS; index != 0; --index)
            out[index - 1].flags = 0xFFFFFFFF;
        return;
    }
    xport_gte_write_control(5, x);
    xport_gte_write_control(6, y);
    xport_gte_write_control(7, z);
    xport_gte_write_data(1, 0);
    index = 0;
    do
    {
        value = radial[index];
        xport_gte_write_data(0, value);
        xport_gte_execute(0x180001);
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            out[index].flags = 0xFFFFFFFF;
        else
        {
            value = xport_gte_read_data(14);
            out[index].screen = value;
            xport_gte_execute(0x780010);
            value = game_screen_clip(out[index].screen);
            out[index].flags = value;
            value = xport_gte_read_data(22);
            out[index].color = value;
        }
        index += 1;
    } while (index < MODE_FOUR_RING_POINTS);
    out[MODE_FOUR_RING_POINTS] = out[0];
    return;
}

static void scene_sky_mode_four_project_slice(const uint32 *origin, MODE_FOUR_VERTEX *out, uint32 step)
{
    uint32 x, z, y, depth, magnitude, product, factor, packed_y, product_x, packed, flags, screen, value, mask, index;
    sint32 sx, sy;
    FUNCTION_MARKER(0x80098714, "ACE3.BIN");
    step <<= 10;
    x = origin[0];
    z = origin[2];
    y = origin[1];
    depth = z + 8192 - step;
    xport_gte_write_data(1, depth);
    magnitude = (sint32)depth < 0 ? 0 - depth : depth;
    product = ((magnitude << 4) - magnitude) << 8;
    factor = 4096 - (uint32)((sint32)product / 8192);
    product = y * factor;
    packed_y = (uint32)((sint32)product / 4096) << 16;
    index = 0;
    product_x = (x - 8192) * factor;
    do
    {
        packed = ((uint32)((sint32)product_x / 4096) & 0xFFFF) | packed_y;
        xport_gte_write_data(0, packed);
        product_x += factor << 10;
        xport_gte_execute(0x180001);
        flags = xport_gte_read_control(31);
        if ((sint32)flags < 0)
            out[index].flags = 0xFFFFFFFF;
        else
        {
            screen = xport_gte_read_data(14);
            mask = 0;
            sy = (sint32)screen >> 16;
            xport_gte_execute(0x780010);
            out[index].screen = screen;
            sx = (sint32)(screen << 16) >> 16;
            if (sx < -160)
                mask = 1;
            else if (sx >= 161)
                mask = 2;
            if (sy < -120)
                mask |= 4;
            else if (sy >= 121)
                mask |= 8;
            out[index].flags = mask;
            value = xport_gte_read_data(22);
            out[index].color = value;
        }
        index += 1;
    } while (index < MODE_FOUR_SLICE_POINTS);
    out[MODE_FOUR_SLICE_POINTS].flags = 0xFFFFFFFF;
    return;
}

static void scene_sky_mode_four_draw_rings(MODE_FOUR_SCRATCH_PUBLICATION *context, uint32 ticks, uint16 camera_distance, uint32 *packet_cursor)
{
    uint32 *saved_rotation = context->rotation_words;
    uint32 *radial = context->work.ring.radial;
    MODE_FOUR_VERTEX(*rings)[MODE_FOUR_RING_POINTS + 1] = context->work.ring.vertices;
    MODE_FOUR_VERTEX *old_buffer, *new_buffer;
    uint32 a, b, c, value, height, focus, trig_phase, index, distance, packet, row, old_flag, new_flag, prior_old, prior_new, next;
    FUNCTION_MARKER(0x80098414, "ACE3.BIN");
    a = xport_gte_read_control(0);
    saved_rotation[0] = a;
    b = xport_gte_read_control(1);
    saved_rotation[1] = b;
    a = xport_gte_read_control(2);
    saved_rotation[2] = a;
    b = xport_gte_read_control(3);
    saved_rotation[3] = b;
    c = xport_gte_read_control(4);
    saved_rotation[4] = c;
    value = rcos((((ticks << 7) & 0x3F80)) >> 2);
    height = (sint16)camera_distance;
    focus = (uint32)((sint32)(value + 4096) / 64) + 128;
    xport_gte_write_control(26, focus);
    context->work.ring.focus = focus << 11;
    scene_geometry_set_depth_cue(10000, 20000, height);
    index = 0;
    trig_phase = 0;
    do
    {
        value = ((uint32)(uint16)rsin(trig_phase) | ((uint32)(uint16)rcos(trig_phase) << 16));
        trig_phase += 204;
        radial[index] = (uint16)((sint32)value >> 17) | ((uint32)(uint16)((sint32)(value << 16) >> 17) << 16);
        ++index;
    } while (index < MODE_FOUR_RING_POINTS);
    radial[MODE_FOUR_RING_POINTS] = radial[0];
    xport_gte_write_data(6, 0x3C878787);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    distance = 0x3000 - ((ticks << 7) & 0x3FF);
    packet = *packet_cursor;
    scene_sky_mode_four_project_ring(radial, rings[0], distance);
    row = 0;
    distance -= 1024;
    do
    {
        new_buffer = rings[(row + 1) & 1];
        old_buffer = rings[row & 1];
        scene_sky_mode_four_project_ring(radial, new_buffer, distance);
        index = 0;
        old_flag = old_buffer[0].flags;
        new_flag = new_buffer[0].flags;
        do
        {
            prior_old = old_flag;
            prior_new = new_flag;
            old_flag = old_buffer[index + 1].flags;
            new_flag = new_buffer[index + 1].flags;
            if (prior_old != 0xFFFFFFFF && prior_new != 0xFFFFFFFF && old_flag != 0xFFFFFFFF && new_flag != 0xFFFFFFFF && (prior_old & prior_new & old_flag & new_flag) == 0)
            {
                POLY_GT4 *quad = mission_packet_span(packet, sizeof(*quad));
                next = packet + 52;
                quad->tag = (next & 0xFFFFFF) | 0x0C000000;
                a = old_buffer[index].color;
                b = old_buffer[index + 1].color;
                quad->p[0].rgbc.rgbc = a;
                quad->p[1].rgbc.rgbc = b;
                a = old_buffer[index].screen;
                b = old_buffer[index + 1].screen;
                quad->p[0].xy.xy = a;
                quad->p[1].xy.xy = b;
                a = new_buffer[index].color;
                b = new_buffer[index + 1].color;
                quad->p[2].rgbc.rgbc = a;
                quad->p[3].rgbc.rgbc = b;
                a = new_buffer[index].screen;
                b = new_buffer[index + 1].screen;
                quad->p[2].uv.uv = (uint16)(0xBF00);
                quad->p[0].uv_attr = 0x4ED68000;
                quad->p[1].uv_attr = 0x0019803F;
                quad->p[3].uv.uv = (uint16)(0xBF3F);
                quad->p[2].xy.xy = a;
                quad->p[3].xy.xy = b;
                packet = next;
            }
            index += 1;
        } while (index < MODE_FOUR_RING_POINTS);
        row += 1;
        distance -= 1024;
    } while (row < 24);
    *packet_cursor = packet;
    a = saved_rotation[0];
    b = saved_rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = saved_rotation[2];
    b = saved_rotation[3];
    c = saved_rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    height = camera_distance;
    xport_gte_write_control(26, height);
    return;
}

uint32 scene_geometry_draw_sky_mode_four(uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    uint32 packet_cursor;
    uint16 camera_distance;
    uint32 ticks, numerator, phase, angle, cosine, intensity, cursor, start, value, a, b, c, flags, mode, tail, tag, end;
    FUNCTION_MARKER(0x80098044, "ACE3.BIN");
    SKY_PASS_PUBLICATION *published = (SKY_PASS_PUBLICATION *)getScratchAddr(0);
    MODE_FOUR_SCRATCH_PUBLICATION *context = (MODE_FOUR_SCRATCH_PUBLICATION *)getScratchAddr(7);
    uint32 *camera_position = published->position;
    ticks = game_runtime.gameplay_counter;
    numerator = ((ticks << 4) - ticks) << 9;
    phase = (sint32)numerator / 450;
    angle = (sint32)phase % 7680;
    if ((sint32)angle >= 2049)
        angle = 2048;
    cosine = rcos((((angle & 0xFFF) << 2)) >> 2);
    if ((sint32)cosine < 0)
        cosine = 0 - cosine;
    intensity = 4778 - cosine;
    if ((sint32)intensity < 1838)
        intensity = 1838;
    cursor = ui_draw_framebuffer_feedback(ordering - 0x3FFC, packet, NULL, intensity);
    DR_STP *mask = mission_packet_span(cursor, 8);
    start = cursor + 8;
    DR_TPAGE *shade = mission_packet_span(start, sizeof(*shade));
    TILE *fill = mission_packet_span(cursor + 16, sizeof(*fill));
    published->ordering = ordering;
    camera_position[0] = game_runtime.camera_view.pose.position[0];
    camera_position[1] = game_runtime.camera_view.pose.position[1];
    camera_position[2] = game_runtime.camera_view.pose.position[2];
    camera_distance = (uint16)game_runtime.camera_proj.distance;
    published->distance = camera_distance;
    shade->tag = ((cursor + 16) & 0xFFFFFF) | 0x01000000;
    shade->code[0] = 0xE1000200;
    fill->tag = ((cursor + 32) & 0xFFFFFF) | 0x04000000;
    fill->rgbc.rgbc = 0x60000000;
    fill->p[0].xy = 0xFF88FF60;
    fill->wh = 0x00F00140;
    packet_cursor = cursor + 32;
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    c = (uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = (uint32)game_runtime.mission_matrices.view.t[0];
    b = (uint32)game_runtime.mission_matrices.view.t[1];
    xport_gte_write_control(5, a);
    c = (uint32)game_runtime.mission_matrices.view.t[2];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    scene_sky_mode_four_draw_rings(context, ticks, camera_distance, &packet_cursor);
    scene_sky_mode_four_draw_slices(context, camera_position, camera_distance, &packet_cursor);
    published->cursor = packet_cursor;
    flags = effect_light.flags;
    value = packet_cursor;
    mode = (flags & 4) != 0;
    tail = cursor;
    if (value == start)
    {
        mask->tag = 0x01000000;
        mask->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(tail, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = cursor & 0xFFFFFF;
        end = tail + 8;
    }
    else
    {
        mask->tag = (start & 0xFFFFFF) | 0x01000000;
        mask->code[0] = 0xE6000001;
        value = packet_cursor;
        DR_TPAGE *reset = mission_packet_span(value, sizeof(*reset));
        tail = value + 8;
        DR_STP *restore = mission_packet_span(tail, 8);
        reset->tag = (tail & 0xFFFFFF) | 0x01000000;
        reset->code[0] = 0xE1000220;
        restore->tag = 0x01000000;
        restore->code[0] = mode | 0xE6000000;
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        tag = *ordering_word;
        packet_link = mission_packet_span(tail, sizeof(*packet_link));
        packet_link->addr = tag;
        ((P_TAG *)ordering_word)->addr = cursor & 0xFFFFFF;
        end = tail + 8;
    }
    effect_light.flags = (uint32)(0);
    ui_compact_ordering_links(ordering - 0x3FF8, 4095, mission_ot_span(ordering - 0x3FF8, 4095 * sizeof(uint32)));
    return end;
}

static void scene_sky_mode_four_draw_slices(MODE_FOUR_SCRATCH_PUBLICATION *context, const uint32 *camera_position, uint16 camera_distance, uint32 *packet_cursor)
{
    uint32 *saved_rotation = context->rotation_words, *origin = context->origin;
    uint32 tilt;
    MODE_FOUR_VERTEX(*slices)[MODE_FOUR_SLICE_POINTS + 1] = context->work.slices;
    MODE_FOUR_VERTEX *old_buffer, *new_buffer;
    uint32 height, a, b, c, value, x, z, packet, group, slice, first_slice, last_slice, cursor, index, old_flag, prior_flag, new_flag, combined, next, color, point;
    FUNCTION_MARKER(0x80098850, "ACE3.BIN");
    height = (sint16)camera_distance;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    scene_geometry_set_depth_cue(2000, 20000, height);
    a = xport_gte_read_control(0);
    saved_rotation[0] = a;
    b = xport_gte_read_control(1);
    saved_rotation[1] = b;
    a = xport_gte_read_control(2);
    saved_rotation[2] = a;
    b = xport_gte_read_control(3);
    saved_rotation[3] = b;
    c = xport_gte_read_control(4);
    saved_rotation[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    context->axis[0] = a;
    context->axis[1] = b;
    context->axis[2] = c;
    a = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][2] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][1] << 16));
    c = (uint16)game_runtime.mission_matrices.camera.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    x = camera_position[0];
    z = camera_position[2];
    xport_gte_execute(0x486012);
    origin[0] = (uint32)((sint32)(0 - x) >> 11) & 0x3FF;
    origin[2] = (uint32)((sint32)(0 - z) >> 11) & 0x3FF;
    value = camera_position[1];
    value = ((uint32)((sint32)(0 - value) >> 11) & 0x7FF) - 5324;
    origin[1] = value;
    value = xport_gte_read_data(11);
    tilt = value;
    a = saved_rotation[0];
    b = saved_rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = saved_rotation[2];
    b = saved_rotation[3];
    c = saved_rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(6, 0x00C05020);
    group = 0;
    cursor = *packet_cursor;
    DR_TPAGE *draw_mode = mission_packet_span(cursor, sizeof(*draw_mode));
    packet = cursor + 8;
    draw_mode->tag = (packet & 0xFFFFFF) | 0x01000000;
    draw_mode->code[0] = 0xE1000220;
    do
    {
        value = tilt;
        value = 0 - (uint32)((sint32)value >> 10);
        first_slice = value + 4;
        last_slice = value + 13;
        if ((sint32)first_slice < 0)
            first_slice = 0;
        if ((sint32)last_slice >= 17)
            last_slice = 16;
        slice = first_slice + 1;
        old_buffer = slices[slice & 1];
        scene_sky_mode_four_project_slice(origin, old_buffer, first_slice);
        while ((sint32)slice <= (sint32)last_slice)
        {
            new_buffer = slices[(slice + 1) & 1];
            old_buffer = slices[slice & 1];
            if (slice == last_slice)
            {
                for (index = MODE_FOUR_SLICE_POINTS + 1; index != 0; --index)
                    new_buffer[index - 1].flags = 0xFFFFFFFF;
            }
            else
                scene_sky_mode_four_project_slice(origin, new_buffer, slice);
            old_flag = old_buffer[0].flags;
            index = 0;
            do
            {
                prior_flag = old_flag;
                old_flag = old_buffer[index + 1].flags;
                new_flag = new_buffer[index].flags;
                combined = old_flag & new_flag;
                if (prior_flag != 0xFFFFFFFF && combined != 0xFFFFFFFF && (prior_flag & combined) == 0)
                {
                    next = packet + 32;
                    if ((sint32)(old_flag | new_flag) < 0)
                    {
                        LINE_G2 *segment = mission_packet_span(packet, sizeof(*segment));
                        next = packet + 20;
                        color = old_buffer[index].color;
                        point = old_buffer[index].screen;
                        segment->tag = (next & 0xFFFFFF) | 0x04000000;
                        segment->p[0].rgbc.rgbc = color | 0x52000000;
                        segment->p[0].xy.xy = point;
                        if ((sint32)old_flag >= 0)
                        {
                            color = old_buffer[index + 1].color;
                            point = old_buffer[index + 1].screen;
                        }
                        else
                        {
                            color = new_buffer[index].color;
                            point = new_buffer[index].screen;
                        }
                        segment->p[1].rgbc.rgbc = color;
                        segment->p[1].xy.xy = point;
                    }
                    else
                    {
                        LINE_G3 *polyline = mission_packet_span(packet, sizeof(*polyline));
                        color = old_buffer[index + 1].color;
                        point = old_buffer[index + 1].screen;
                        polyline->tag = (next & 0xFFFFFF) | 0x07000000;
                        polyline->p[0].rgbc.rgbc = color | 0x5A000000;
                        polyline->p[0].xy.xy = point;
                        color = old_buffer[index].color;
                        point = old_buffer[index].screen;
                        polyline->p[1].rgbc.rgbc = color;
                        polyline->p[1].xy.xy = point;
                        color = new_buffer[index].color;
                        point = new_buffer[index].screen;
                        polyline->p[2].rgbc.rgbc = color;
                        polyline->p[2].xy.xy = point;
                        game_random();
                        game_random();
                        polyline->pad = 0x55555555;
                    }
                    packet = next;
                }
                index += 1;
            } while (index < MODE_FOUR_SLICE_POINTS);
            slice += 1;
        }
        group += 1;
        origin[1] += 2048;
    } while (group < 4);
    context->tilt = tilt;
    *packet_cursor = packet;
    return;
}
