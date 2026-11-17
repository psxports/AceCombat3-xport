#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    uint32 xy;
    uint32 z;
} HUD_ORIENTATION_VERTEX;

typedef struct
{
    union
    {
        uint32 xy;
        uint16 axis[2];
    };

    uint16 z, reserved;
} HUD_POINT_PUBLICATION;

typedef union
{
    struct
    {
        HUD_POINT_PUBLICATION point;
        uint8 before_current[16];
        uint32 current_screen;
        uint8 before_previous[8];
        uint32 previous_screen;
    } compass;

    struct
    {
        HUD_POINT_PUBLICATION points[3];
        uint32 pitch;
        uint16 roll, reserved;
        uint32 screen;
    } pitch;
} HUD_INSTRUMENT_PUBLICATION;

typedef struct
{
    uint32 x, z, sine, cosine;

    union
    {
        uint32 heading_word;

        struct
        {
            uint16 heading, heading_sign;
        };
    };
} HUD_RADAR_REFERENCE;

typedef char hud_scratch_publication_layout[sizeof(HUD_POINT_PUBLICATION) == 8 && offsetof(HUD_POINT_PUBLICATION, z) == 4 && (offsetof(HUD_POINT_PUBLICATION, axis) + 0 * sizeof(((HUD_POINT_PUBLICATION *)0)->axis[0])) == 0 && (offsetof(HUD_POINT_PUBLICATION, axis) + 1 * sizeof(((HUD_POINT_PUBLICATION *)0)->axis[0])) == 2 && sizeof(HUD_INSTRUMENT_PUBLICATION) == 40 && offsetof(HUD_INSTRUMENT_PUBLICATION, compass.current_screen) == 24 && offsetof(HUD_INSTRUMENT_PUBLICATION, compass.previous_screen) == 36 && offsetof(HUD_INSTRUMENT_PUBLICATION, pitch.pitch) == 24 && offsetof(HUD_INSTRUMENT_PUBLICATION, pitch.roll) == 28 && offsetof(HUD_INSTRUMENT_PUBLICATION, pitch.screen) == 32 && sizeof(HUD_RADAR_REFERENCE) == 20 && offsetof(HUD_RADAR_REFERENCE, heading) == 16 && offsetof(HUD_RADAR_REFERENCE, heading_word) == 16 && offsetof(HUD_RADAR_REFERENCE, heading_sign) == 18 ? 1 : -1];

typedef char hud_flight_packet_bank_layout_check[sizeof(HUD_FLIGHT_PACKET_BANK) == 2516 && offsetof(HUD_FLIGHT_PACKET_BANK, mode) == 0 && offsetof(HUD_FLIGHT_PACKET_BANK, flat_quads) == 8 && offsetof(HUD_FLIGHT_PACKET_BANK, triangles) == 392 && offsetof(HUD_FLIGHT_PACKET_BANK, textured_quads) == 504 && offsetof(HUD_FLIGHT_PACKET_BANK, lines) == 984 && offsetof(HUD_FLIGHT_PACKET_BANK, gradient_lines) == 2008 && offsetof(HUD_FLIGHT_PACKET_BANK, waveform) == 2168 && offsetof(HUD_FLIGHT_PACKET_BANK, digits) == 2208 && offsetof(HUD_FLIGHT_PACKET_BANK, indicator) == 2368 && offsetof(HUD_FLIGHT_PACKET_BANK, markers) == 2388 && sizeof(LINE_G4) == 40 && offsetof(LINE_G4, pad) == 36 ? 1 : -1];

static const HUD_ORIENTATION_VERTEX hud_orientation_vertices[4] = {
    {0xFFEC0000, 0x00000000},
    {0x00140000, 0x000000B4},
    {0x0014FFA6, 0x0000FFE2},
    {0x0014005A, 0x0000FFE2},
};

static const uint8 hud_compass_segment_glyphs[8][2] = {
    {0, 255}, {0, 1}, {1, 255}, {2, 1}, {2, 255}, {2, 3}, {3, 255}, {0, 3},
};

HUD_CONTROL_STATE hud_control;

typedef struct
{
    DR_MODE masks[5];
    DR_TPAGE modes[2];
    POLY_FT4 base_quad;
    POLY_FT4 quads[8];
    SPRT sprites[2];
    TILE tiles[2];
} HUD_MAP_GPU_BANK;

typedef char hud_map_gpu_bank_layout_check[sizeof(DR_MODE) == 12 && sizeof(DR_TPAGE) == 8 && sizeof(POLY_FT4) == 40 && sizeof(SPRT) == 20 && sizeof(TILE) == 16 && sizeof(HUD_MAP_GPU_BANK) == 508 && offsetof(HUD_MAP_GPU_BANK, modes) == 60 && offsetof(HUD_MAP_GPU_BANK, base_quad) == 76 && offsetof(HUD_MAP_GPU_BANK, quads) == 116 && offsetof(HUD_MAP_GPU_BANK, sprites) == 436 && offsetof(HUD_MAP_GPU_BANK, tiles) == 476 ? 1 : -1];

struct HUD_GPU_BANK
{
    union
    {
        struct
        {
            DR_AREA areas[2];
            SPRT sprs[64];
            TILE tiles[64];
            LINE_F2 lines[8];
            LINE_F3 polylines[8];
            HUD_MAP_GPU_BANK map;
            LINE_F2 clip_lines[2];
            POLY_F3 triangles[3];
            DR_MODE masks[3];
            POLY_FT4 fixed_quads[7];
            POLY_FT4 markers[4];
            TILE marker_tiles[4];
            LINE_F3 frame_polylines[4];
            LINE_F2 frame_lines[10];
            POLY_F4 frame_polys[2];
            SPRT center;
            SPRT fixed_sprs[4];
            uint32 modes[4][2];
        };

        struct
        {
            DR_AREA submitted_area_prefix[2];
            SPRT submitted_sprites[94];
        };
    };
};

typedef char hud_gpu_bank_layout_check
    [sizeof(HUD_GPU_BANK) == 4224 && sizeof(SPRT) == 20 && offsetof(HUD_GPU_BANK, submitted_sprites) == 24 && sizeof(((HUD_GPU_BANK *)0)->submitted_sprites) == 1880 && offsetof(HUD_GPU_BANK, areas) == 0x0 && offsetof(HUD_GPU_BANK, sprs) == 0x18 && offsetof(HUD_GPU_BANK, tiles) == 0x518 && offsetof(HUD_GPU_BANK, lines) == 0x918 && offsetof(HUD_GPU_BANK, polylines) == 0x998 && offsetof(HUD_GPU_BANK, map) == 0xA58 && offsetof(HUD_GPU_BANK, clip_lines) == 0xC54 && offsetof(HUD_GPU_BANK, triangles) == 0xC74 && offsetof(HUD_GPU_BANK, masks) == 0xCB0 && offsetof(HUD_GPU_BANK, fixed_quads) == 0xCD4 && offsetof(HUD_GPU_BANK, markers) == 0xDEC && offsetof(HUD_GPU_BANK, marker_tiles) == 0xE8C && offsetof(HUD_GPU_BANK, frame_polylines) == 0xECC && offsetof(HUD_GPU_BANK, frame_lines) == 0xF2C && offsetof(HUD_GPU_BANK, frame_polys) == 0xFCC && offsetof(HUD_GPU_BANK, center) == 0xFFC && offsetof(HUD_GPU_BANK, fixed_sprs) == 0x1010 && offsetof(HUD_GPU_BANK, modes) == 0x1060 ? 1 : -1];

static void hud_radar_set_reference(HUD_RADAR_REFERENCE *reference, const HUD_CONTEXT *context);
static void hud_radar_update_distances(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_update(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_update_markers(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_update_contacts(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_draw(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_draw_map(const HUD_RADAR_REFERENCE *reference);
static void hud_radar_draw_waypoint(const HUD_RADAR_REFERENCE *reference, const uint32 *waypoint);

static void hud_marker_draw_kind(uint8 index, uint32 x, uint32 y);
static void hud_marker_draw_label(uint8 index, uint32 x, uint32 y);

static const sint16 hud_target_triangle_x[2][3] = {{-150, -140, -150}, {150, 140, 150}};
static const sint16 hud_target_triangle_y[3] = {-40, -50, -60};
static const sint16 hud_target_triangle_z[3] = {10, 0, 10};
static const sint16 hud_target_quad_x[4] = {0, -40, 40, 0};
static const sint16 hud_target_quad_y[4] = {0, -10, -10, -10};
static const uint8 hud_target_arrow_offsets[4][8] = {{10, 251, 10, 5, 10, 246, 10, 10}, {246, 251, 246, 5, 246, 246, 246, 10}, {251, 10, 5, 10, 246, 10, 10, 10}, {251, 246, 5, 246, 246, 246, 10, 246}};

typedef struct
{
    uint32 stride;
    uint16 width;
    uint8 u, v, height;
} HUD_DIGIT_FONT;

static const HUD_DIGIT_FONT hud_digit_fonts[3] = {
    {14, 14, 0, 0, 10},
    {10, 10, 4, 0, 6},
    {4, 4, 144, 53, 6},
};

void hud_digit_draw(uint32 glyph, uint32 x, uint32 y, uint32 font, SPRT *spr, uint32 color)
{
    uint32 width, height, stride, u, result;
    const HUD_DIGIT_FONT *table;
    FUNCTION_MARKER(0x8003B7B0, "ACE3.BIN");
    table = &hud_digit_fonts[font == 0 ? 0 : font == 1 ? 1 : 2];
    if (color == 0)
    {
        spr->rgbc.r = (uint8)(0);
        spr->rgbc.g = (uint8)(131);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 1)
    {
        spr->rgbc.r = (uint8)(131);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 2)
    {
        spr->rgbc.r = (uint8)(131);
        spr->rgbc.g = (uint8)(131);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 3)
    {
        spr->rgbc.r = (uint8)(131);
        spr->rgbc.g = (uint8)(131);
        spr->rgbc.b = (uint8)(131);
    }
    if (glyph == 10)
    {
        spr->p[0].xy.y = (sint16)(uint16)(y + 3);
        spr->w = (sint16)(uint16)(4);
        spr->p[0].xy.x = (sint16)(uint16)(x);
        spr->h = (sint16)(uint16)(3);
        spr->p[0].uv.u = (uint8)(0);
        spr->p[0].uv.v = (uint8)(0);
        return;
    }
    if (glyph == 11)
    {
        spr->p[0].xy.y = (sint16)(uint16)(y + 2);
        spr->w = (sint16)(uint16)(6);
        spr->h = (sint16)(uint16)(2);
        spr->p[0].xy.x = (sint16)(uint16)(x);
        spr->p[0].uv.u = (uint8)(44);
        spr->p[0].uv.v = (uint8)(10);
        return;
    }
    if (glyph == 0xFFFFFFFF)
    {
        spr->w = (sint16)(uint16)(4);
        spr->p[0].xy.x = (sint16)(uint16)(x);
        spr->p[0].xy.y = (sint16)(uint16)(y);
        spr->h = (sint16)(uint16)(6);
        spr->p[0].uv.u = (uint8)(40);
        spr->p[0].uv.v = (uint8)(6);
        return;
    }
    spr->p[0].xy.x = (sint16)(uint16)(x);
    spr->p[0].xy.y = (sint16)(uint16)(y);
    width = table->width;
    spr->w = (sint16)(uint16)(width);
    height = table->height;
    spr->h = (sint16)(uint16)(height);
    stride = table->stride;
    u = table->u;
    spr->p[0].uv.u = (uint8)(u + glyph * stride);
    result = table->v;
    spr->p[0].uv.v = (uint8)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

HUD_RADAR_MARK *hud_radar_contact(uint32 index)
{
    if (index >= HUD_RADAR_CONTACT_COUNT)
        abort();
    return &hud_control.radar_contacts[index];
}

enum
{
    HUD_NOTICE_COUNT = 29,
    HUD_NOTICE_ICON_COUNT = 30,
    HUD_NOTICE_PAIR_COUNT = 11
};

typedef struct
{
    uint8 mode, color;
    sint16 duration;
    uint16 x, y;
    uint8 icon[2];
    uint8 offset[2];
    sint16 sound, sound_tick;
} HUD_NOTICE_SPEC;

typedef struct
{
    uint8 u, v, w, h;
} HUD_NOTICE_ICON;

typedef struct
{
    sint16 ticks;
    uint8 kind[2];
} HUD_NOTICE_PAIR;

static const HUD_NOTICE_SPEC notice_specs[HUD_NOTICE_COUNT] = {
    {18, 0, 90, 94, 80, {2, 3}, {16, 64}, 66, 0},   {16, 0, 45, 94, 80, {0, 255}, {28, 0}, 64, 0},  {17, 0, 45, 94, 80, {1, 255}, {28, 0}, 65, 0},  {32, 0, 90, 94, 192, {8, 9}, {12, 58}, 69, 30}, {33, 1, 45, 94, 192, {11, 12}, {16, 50}, 71, 30}, {33, 1, 45, 94, 192, {4, 10}, {20, 68}, 70, 30}, {32, 1, 90, 94, 192, {7, 255}, {38, 0}, 68, 30}, {33, 1, 45, 94, 192, {4, 5}, {30, 78}, 67, 30}, {32, 1, 90, 94, 192, {2, 13}, {14, 62}, 72, 30}, {32, 1, 90, 94, 192, {29, 3}, {10, 67}, 100, 30}, {0, 2, 10, 94, 80, {25, 255}, {23, 0}, 0, -1}, {1, 2, 16, 94, 80, {21, 22}, {4, 73}, 0, -1}, {0, 1, 45, 94, 80, {14, 255}, {12, 0}, 111, 0}, {0, 1, 45, 94, 80, {15, 11}, {28, 74}, 0, -1}, {0, 1, 45, 94, 80, {16, 10}, {20, 70}, 113, 0},
    {0, 1, 180, 94, 80, {17, 10}, {14, 74}, 58, 0}, {0, 1, 45, 94, 80, {18, 19}, {6, 90}, 114, -1}, {0, 1, 45, 94, 80, {20, 255}, {28, 0}, 55, -1}, {1, 2, 16, 94, 80, {21, 22}, {4, 73}, 0, -1},   {1, 2, 16, 94, 80, {21, 23}, {11, 80}, 0, -1},    {1, 2, 16, 94, 80, {21, 24}, {1, 67}, 0, -1},    {0, 2, 180, 94, 80, {25, 255}, {23, 0}, 55, 0},  {0, 1, 45, 94, 80, {4, 5}, {30, 78}, 67, 0},    {0, 1, 45, 94, 80, {4, 10}, {20, 68}, 70, 0},    {0, 1, 45, 94, 80, {26, 27}, {16, 72}, 73, 0},    {0, 1, 45, 94, 80, {28, 27}, {20, 68}, 74, 0}, {0, 2, 16, 94, 80, {21, 22}, {4, 73}, 0, -1}, {0, 2, 16, 94, 80, {21, 23}, {11, 80}, 0, -1},  {0, 2, 16, 94, 80, {21, 24}, {1, 67}, 0, -1},
};

static const HUD_NOTICE_ICON notice_icons[HUD_NOTICE_ICON_COUNT] = {
    {0, 20, 75, 6}, {0, 26, 72, 6}, {0, 32, 45, 6}, {0, 38, 54, 6}, {46, 32, 43, 6}, {110, 44, 23, 6}, {72, 26, 61, 6}, {0, 68, 53, 6}, {90, 32, 43, 6}, {0, 44, 66, 6}, {66, 44, 44, 6}, {104, 56, 31, 6}, {0, 74, 64, 6}, {76, 20, 55, 6}, {0, 56, 104, 6}, {94, 62, 41, 6}, {50, 62, 43, 6}, {76, 50, 54, 6}, {54, 38, 82, 6}, {98, 68, 37, 6}, {0, 50, 75, 6}, {0, 80, 65, 6}, {66, 80, 54, 6}, {0, 86, 40, 6}, {0, 92, 63, 6}, {42, 86, 88, 6}, {0, 62, 50, 6}, {54, 68, 43, 6}, {64, 74, 43, 6}, {64, 92, 53, 6},
};

static const HUD_NOTICE_PAIR notice_pairs[HUD_NOTICE_PAIR_COUNT] = {
    {90, {2, 3}}, {90, {0, 255}}, {90, {1, 255}}, {90, {2, 4}}, {90, {2, 5}}, {90, {2, 6}}, {90, {2, 7}}, {90, {2, 8}}, {90, {2, 9}}, {10, {10, 255}}, {16, {11, 255}},
};

enum
{
    HUD_WAVEFORM_FEEDBACK = 4
};

static const uint8 hud_waveform_curves[4][38] = {
    {16, 14, 12, 10, 8, 8, 9, 10, 10, 11, 12, 12, 13, 12, 11, 10, 10, 11, 12, 13, 14, 15, 15, 16, 15, 14, 14, 13, 12, 11, 10, 9, 9, 8, 10, 12, 14, 16},
    {21, 19, 17, 15, 13, 13, 14, 15, 15, 14, 13, 13, 14, 15, 16, 16, 17, 18, 19, 19, 18, 17, 17, 16, 16, 15, 14, 13, 12, 13, 14, 14, 13, 13, 15, 17, 19, 21},
    {12, 11, 10, 10, 9, 8, 7, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 15, 14, 13, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 20, 19, 18, 17, 16, 16, 16},
    {21, 22, 22, 21, 20, 19, 19, 18, 18, 17, 16, 16, 15, 15, 16, 17, 18, 19, 20, 21, 20, 19, 18, 18, 17, 18, 18, 19, 19, 18, 17, 16, 15, 14, 14, 15, 16, 17},
};

struct HUD_INSTRUMENT_GPU_BANK
{
    uint32 mode[2];
    SPRT speed[5];
    POLY_F4 speed_polys[2];
    SPRT altitude[13];
    DR_AREA altitude_areas[2];
    SPRT timer[7];
    SPRT gauge[10];
    SPRT health[6];
    SPRT color_marks[2];
    LINE_F2 color_mark_lines[3];
    SPRT color_frames[2];
    LINE_F2 color_frame_lines[2];
};

typedef char hud_instrument_gpu_bank_layout_check[sizeof(HUD_INSTRUMENT_GPU_BANK) == HUD_INSTRUMENT_GPU_BYTES && offsetof(HUD_INSTRUMENT_GPU_BANK, mode) == 0 && offsetof(HUD_INSTRUMENT_GPU_BANK, speed) == 8 && offsetof(HUD_INSTRUMENT_GPU_BANK, speed_polys) == 108 && offsetof(HUD_INSTRUMENT_GPU_BANK, altitude) == 156 && offsetof(HUD_INSTRUMENT_GPU_BANK, altitude_areas) == 416 && offsetof(HUD_INSTRUMENT_GPU_BANK, timer) == 440 && offsetof(HUD_INSTRUMENT_GPU_BANK, gauge) == 580 && offsetof(HUD_INSTRUMENT_GPU_BANK, health) == 780 && offsetof(HUD_INSTRUMENT_GPU_BANK, color_marks) == 900 && offsetof(HUD_INSTRUMENT_GPU_BANK, color_mark_lines) == 940 && offsetof(HUD_INSTRUMENT_GPU_BANK, color_frames) == 988 && offsetof(HUD_INSTRUMENT_GPU_BANK, color_frame_lines) == 1028 ? 1 : -1];

typedef struct
{
    HUD_INSTRUMENT_GPU_BANK instrument[2];
    HUD_TARGET_PACKET_BANK target[2];
    HUD_NOTICE_PACKET_BANK notice[2];
    HUD_GPU_BANK packets[2];
    HUD_MARKER_PACKET_BANK marker[2];
    HUD_FLIGHT_PACKET_BANK flight[2];
    HUD_INSTRUMENT_GPU_BANK loading_instrument[2];
} HUD_GPU_STORAGE;

static HUD_GPU_STORAGE hud_gpu_storage;

void *hud_native_packet_span(uint32 address, size_t bytes)
{
    uint32 index, kind;

    for (index = 0; index < 2; ++index)
    {
        uint32 bases[6] = {hud_control.instrument_banks[index], hud_control.target_packets.bank[index], hud_control.notice_packets.bank[index], hud_control.packets.bank[index], hud_control.marker_packets.bank[index], hud_control.flight_packets.bank[index]};
        uint8 *data[6] = {(uint8 *)hud_control.instrument_bank_data[index], (uint8 *)hud_control.target_packets.bank_data[index], (uint8 *)hud_control.notice_packets.bank_data[index], (uint8 *)hud_control.packets.bank_data[index], (uint8 *)hud_control.marker_packets.bank_data[index], (uint8 *)hud_control.flight_packets.bank_data[index]};
        const size_t capacities[6] = {sizeof(hud_gpu_storage.instrument[index]), sizeof(hud_gpu_storage.target[index]), sizeof(hud_gpu_storage.notice[index]), sizeof(hud_gpu_storage.packets[index]), sizeof(hud_gpu_storage.marker[index]), sizeof(hud_gpu_storage.flight[index])};

        for (kind = 0; kind < 6; ++kind)
        {
            uint32 offset = address - bases[kind];

            if (data[kind] && bytes && address >= bases[kind] && offset <= capacities[kind] && bytes <= capacities[kind] - offset)
                return data[kind] + offset;
        }
    }
    return NULL;
}

void hud_speed_init(uint32 dst, SPRT *spr, POLY_F4 *poly)
{
    uint32 i, result;
    FUNCTION_MARKER(0x8005C5B0, "ACE3.BIN");
    for (i = 0; i < 2; ++i)
    {
        setPolyF4(&poly[i]);
        setSemiTrans(&poly[i], 1);
    }
    for (i = 0; i < 5; ++i)
    {
        setSprt(&spr[i]);
        spr[i].p[0].attr = getClut(384, 510);
        setSemiTrans(&spr[i], 1);
    }
    spr[0].p[0].uv.v = 25;
    spr[0].w = 68;
    spr[0].h = 20;
    spr[0].p[0].xy.x = 10;
    spr[0].p[0].xy.y = 128;
    result = (dst + 124) & 0xFFFFFF;
    spr[0].p[0].uv.u = 0;
    setaddr(&poly[0], dst + 124);
    setaddr(&poly[1], dst);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_altitude_init(uint32 dst, SPRT *spr)
{
    uint32 i;
    FUNCTION_MARKER(0x8005C990, "ACE3.BIN");
    spr[0].p[0].uv.u = 130;
    spr[0].w = 54;
    spr[0].h = 16;
    setSprt(&spr[0]);
    spr[0].p[0].xy.x = 239;
    spr[0].p[0].attr = getClut(384, 510);
    spr[0].p[0].uv.v = 15;
    spr[0].p[0].xy.y = 96;
    setSemiTrans(&spr[0], 1);
    setaddr(&spr[0], dst + 20);
    spr[1].p[0].uv.u = 184;
    spr[1].w = 19;
    setSprt(&spr[1]);
    spr[1].h = 41;
    spr[1].p[0].attr = getClut(384, 510);
    spr[1].p[0].uv.v = 15;
    spr[1].p[0].xy.x = 293;
    spr[1].p[0].xy.y = 96;
    setSemiTrans(&spr[1], 1);
    for (i = 2; i < 7; ++i)
    {
        setSprt(&spr[i]);
        setSemiTrans(&spr[i], 1);
        spr[i].p[0].attr = getClut(384, 510);
        spr[i].p[0].uv.u = 36;
        spr[i].p[0].uv.v = 45;
        spr[i].w = 6;
        spr[i].h = 10;
        spr[i].p[0].xy.x = 302;
        if (i < 6)
            setaddr(&spr[i], dst + (i + 1) * 20);
    }
    for (i = 7; i < 13; ++i)
    {
        setSprt(&spr[i]);
        spr[i].p[0].attr = getClut(384, 510);
        setSemiTrans(&spr[i], 1);
    }
    return;
}

void hud_timer_init(uint32 dst, SPRT *spr)
{
    uint32 i;
    FUNCTION_MARKER(0x8005CEDC, "ACE3.BIN");
    spr[0].p[0].uv.u = 68;
    spr[0].p[0].uv.v = 25;
    spr[0].w = 60;
    setSprt(&spr[0]);
    spr[0].p[0].xy.x = 12;
    spr[0].p[0].attr = getClut(384, 510);
    spr[0].h = 14;
    spr[0].p[0].xy.y = 206;
    setSemiTrans(&spr[0], 1);
    spr[1].p[0].uv.u = 50;
    spr[1].p[0].uv.v = 49;
    spr[1].w = 5;
    setSprt(&spr[1]);
    spr[1].h = 6;
    spr[1].p[0].attr = getClut(384, 510);
    spr[1].p[0].xy.x = 14;
    spr[1].p[0].xy.y = 210;
    setSemiTrans(&spr[1], 1);
    for (i = 2; i < 7; ++i)
    {
        setSprt(&spr[i]);
        spr[i].p[0].attr = getClut(384, 510);
        setSemiTrans(&spr[i], 1);
    }
    for (i = 0; i < 6; ++i)
        setaddr(&spr[i], dst + (i + 1) * 20);
    return;
}

void hud_health_init(uint32 dst, SPRT *spr)
{
    uint32 i;
    FUNCTION_MARKER(0x8005D344, "ACE3.BIN");
    spr[0].p[0].attr = getClut(384, 510);
    spr[0].p[0].uv.u = 220;
    spr[0].w = 36;
    spr[0].h = 17;
    spr[0].p[0].xy.x = 238;
    setSprt(&spr[0]);
    spr[0].p[0].xy.y = 210;
    setSemiTrans(&spr[0], 1);
    spr[0].p[0].uv.v = 0;
    setaddr(&spr[0], dst + 20);
    spr[1].p[0].attr = getClut(384, 504);
    spr[1].p[0].uv.v = 160;
    spr[1].w = 37;
    spr[1].h = 48;
    spr[1].p[0].xy.x = 274;
    setSprt(&spr[1]);
    setSemiTrans(&spr[1], 1);
    spr[1].p[0].uv.u = 216;
    spr[1].p[0].xy.y = 182;
    spr[2].p[0].attr = getClut(432, 509);
    spr[2].p[0].uv.v = 208;
    spr[2].w = 40;
    spr[2].h = 40;
    spr[2].p[0].xy.x = 263;
    setSprt(&spr[2]);
    setSemiTrans(&spr[2], 1);
    spr[2].p[0].uv.u = 216;
    spr[2].p[0].xy.y = 172;
    for (i = 3; i < 6; ++i)
    {
        setSprt(&spr[i]);
        setSemiTrans(&spr[i], 1);
        spr[i].p[0].attr = getClut(384, 510);
    }
    return;
}

void hud_gauge_init(SPRT *spr)
{
    uint32 i, x;
    FUNCTION_MARKER(0x8005D844, "ACE3.BIN");
    setSprt(&spr[0]);
    setSemiTrans(&spr[0], 1);
    spr[0].p[0].attr = getClut(384, 510);
    spr[0].p[0].uv.u = 158;
    spr[0].p[0].uv.v = 33;
    spr[0].w = 25;
    spr[0].h = 20;
    spr[0].p[0].xy.x = 238;
    spr[0].p[0].xy.y = 190;
    x = 242;
    for (i = 1; i < 5; ++i)
    {
        setSprt(&spr[i]);
        setSemiTrans(&spr[i], 1);
        spr[i].p[0].attr = getClut(384, 510);
        spr[i].p[0].uv.u = 56;
        spr[i].p[0].uv.v = 48;
        spr[i].w = 5;
        spr[i].h = 7;
        spr[i].p[0].xy.x = x;
        spr[i].p[0].xy.y = 182;
        x += 5;
    }
    x = 252;
    for (i = 5; i < 7; ++i)
    {
        setSprt(&spr[i]);
        setSemiTrans(&spr[i], 1);
        spr[i].p[0].attr = getClut(384, 510);
        spr[i].p[0].uv.u = 56;
        spr[i].p[0].uv.v = 48;
        spr[i].w = 5;
        spr[i].h = 7;
        spr[i].p[0].xy.x = x;
        spr[i].p[0].xy.y = 173;
        x += 5;
    }
    for (i = 7; i < 10; ++i)
    {
        setSprt(&spr[i]);
        setSemiTrans(&spr[i], 1);
        spr[i].p[0].attr = getClut(384, 510);
    }
    return;
}

void hud_color_marks_init(uint32 dst, SPRT *spr, LINE_F2 *line)
{
    FUNCTION_MARKER(0x8005DCA8, "ACE3.BIN");
    spr[0].p[0].uv.u = 218;
    spr[0].p[0].uv.v = 17;
    spr[0].w = 38;
    setSprt(&spr[0]);
    spr[0].h = 39;
    setSemiTrans(&spr[0], 1);
    spr[0].p[0].attr = getClut(384, 508);
    spr[0].p[0].xy.x = 230;
    spr[0].p[0].xy.y = 164;
    spr[1].p[0].uv.u = 42;
    spr[1].p[0].uv.v = 47;
    spr[1].w = 8;
    spr[1].h = 8;
    setSprt(&spr[1]);
    spr[1].p[0].xy.x = 223;
    spr[1].p[0].attr = getClut(384, 508);
    spr[1].p[0].xy.y = 219;
    setSemiTrans(&spr[1], 1);
    setLineF2(&line[0]);
    line[0].p[0].x = 0;
    line[0].p[0].y = 226;
    line[0].p[1].x = 222;
    line[0].p[1].y = 226;
    setSemiTrans(&line[0], 1);
    setLineF2(&line[1]);
    line[1].p[0].x = 230;
    line[1].p[0].y = 218;
    line[1].p[1].x = 230;
    line[1].p[1].y = 203;
    setSemiTrans(&line[1], 1);
    setLineF2(&line[2]);
    line[2].p[0].x = 268;
    line[2].p[0].y = 164;
    line[2].p[1].x = 320;
    line[2].p[1].y = 164;
    setSemiTrans(&line[2], 1);
    setaddr(&spr[0], dst + 20);
    setaddr(&spr[1], dst + 40);
    setaddr(&line[0], dst + 56);
    setaddr(&line[1], dst + 72);
    {
        (void)(uint32)(line[1].tag);
        return;
    }
}

void hud_color_frames_init(uint32 dst, SPRT *spr, LINE_F2 *line)
{
    FUNCTION_MARKER(0x8005DEFC, "ACE3.BIN");
    setLineF2(&line[0]);
    setSemiTrans(&line[0], 1);
    line[0].p[0].x = 0;
    line[0].p[0].y = 45;
    line[0].p[1].x = 4;
    line[0].p[1].y = 45;
    setLineF2(&line[1]);
    setSemiTrans(&line[1], 1);
    line[1].p[0].x = 85;
    line[1].p[1].x = 320;
    line[1].p[0].y = 14;
    line[1].p[1].y = 14;
    spr[0].p[0].xy.x = 5;
    spr[0].p[0].uv.v = 55;
    spr[0].w = 80;
    spr[0].h = 73;
    setSprt(&spr[0]);
    setSemiTrans(&spr[0], 1);
    spr[0].p[0].attr = getClut(384, 510);
    spr[0].p[0].xy.y = 12;
    spr[0].p[0].uv.u = 0;
    spr[1].p[0].xy.x = 9;
    spr[1].p[0].uv.u = 24;
    setSprt(&spr[1]);
    setSemiTrans(&spr[1], 1);
    spr[1].p[0].attr = getClut(384, 510);
    spr[1].p[0].xy.y = 12;
    spr[1].p[0].uv.v = 45;
    spr[1].w = 12;
    spr[1].h = 10;
    setaddr(&line[0], dst + 56);
    setaddr(&line[1], dst);
    setaddr(&spr[0], dst + 20);
    {
        (void)(uint32)(spr[0].tag);
        return;
    }
}

void hud_instruments_init_bank(uint32 dst, HUD_INSTRUMENT_GPU_BANK *bank)
{
    FUNCTION_MARKER(0x8005E10C, "ACE3.BIN");
    hud_speed_init(dst + 8, bank->speed, bank->speed_polys);
    hud_altitude_init(dst + 156, bank->altitude);
    hud_timer_init(dst + 440, bank->timer);
    hud_health_init(dst + 780, bank->health);
    hud_gauge_init(bank->gauge);
    hud_color_marks_init(dst + 900, bank->color_marks, bank->color_mark_lines);
    hud_color_frames_init(dst + 988, bank->color_frames, bank->color_frame_lines);
    SetDrawMode1(bank->mode, 0, 1, 54);
    return;
}

void hud_instruments_init_banks(void)
{
    uint32 index, block;
    FUNCTION_MARKER(0x8005E2C0, "ACE3.BIN");
    (void)arena_alloc(8);
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(HUD_INSTRUMENT_GPU_BYTES);
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.instrument[index]);
        hud_control.instrument_banks[index] = block;
        hud_control.instrument_bank_data[index] = &hud_gpu_storage.instrument[index];
        hud_instruments_init_bank(block, hud_control.instrument_bank_data[index]);
    }
    return;
}

uint32 hud_instruments_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments)
{
    HUD_INSTRUMENT_GPU_BANK *bank;
    uint32 index = game_runtime.display_buffer;
    uint32 link;
    FUNCTION_MARKER(0x8005E40C, "ACE3.BIN");
    if (index >= 2 || !gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    bank = &hud_gpu_storage.loading_instrument[index];
    link = gpu_packet_link((uintptr_t)bank);
    hud_instruments_init_bank(link, bank);
    hud_instruments_submit(object, link, instruments, bank);
    return dst + HUD_INSTRUMENT_GPU_BYTES;
}

void hud_instruments_submit(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    uint32 *header = bank->mode;
    uint32 tag, head, result;
    FUNCTION_MARKER(0x8005E178, "ACE3.BIN");
    if (((instruments->flags)) & 1)
        hud_speed_draw(object, dst + 8, instruments, bank);
    if (((instruments->flags)) & 2)
        hud_altitude_draw(object, dst + 156, instruments, bank);
    if (((instruments->flags)) & 4)
        hud_timer_draw(object, dst + 440, instruments, bank);
    if (((instruments->flags)) & 16)
        hud_health_draw(object, dst + 780, instruments, bank);
    if (((instruments->flags)) & 8)
        hud_gauge_draw(object, dst + 580, instruments, bank);
    if (((instruments->flags)) & 64)
        hud_color_marks_draw(object, dst + 900, instruments, bank);
    if (((instruments->flags)) & 128)
        hud_color_frames_draw(object, dst + 988, instruments, bank);
    tag = *header;
    head = (*ot);
    *header = ((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_color_frames_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->color_frames;
    LINE_F2 *line = bank->color_frame_lines;
    uint32 tag, head, result;
    FUNCTION_MARKER(0x8005E018, "ACE3.BIN");
    line[0].rgbc.r = (uint8)(((instruments->primary_color[0])) >> 1);
    line[0].rgbc.g = (uint8)(((instruments->primary_color[1])) >> 1);
    line[0].rgbc.b = (uint8)(((instruments->primary_color[2])) >> 1);
    line[1].rgbc.r = (uint8)(((instruments->primary_color[0])) >> 1);
    line[1].rgbc.g = (uint8)(((instruments->primary_color[1])) >> 1);
    line[1].rgbc.b = (uint8)(((instruments->primary_color[2])) >> 1);
    spr[0].rgbc.r = (uint8)(((instruments->secondary_color[0])) >> 1);
    spr[0].rgbc.g = (uint8)(((instruments->secondary_color[1])) >> 1);
    spr[0].rgbc.b = (uint8)(((instruments->secondary_color[2])) >> 1);
    spr[1].rgbc.r = (uint8)(((instruments->secondary_color[0])) >> 1);
    spr[1].rgbc.g = (uint8)(((instruments->secondary_color[1])) >> 1);
    spr[1].rgbc.b = (uint8)(((instruments->secondary_color[2])) >> 1);
    tag = spr[1].tag;
    head = (*ot);
    spr[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | ((dst + 40) & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_speed_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->speed;
    POLY_F4 *poly = bank->speed_polys;
    uint32 digit_packet = dst + 20, digit_x = 64, i = 0, tag, head, result;
    uint32 angle, previous, next, color, a, b;
    sint32 number, quotient, sx0, sy0, sx1, sy1, sx2, sy2;
    sint32 v;
    FUNCTION_MARKER(0x8005C680, "ACE3.BIN");
    number = (sint32)(((instruments->speed)) * 28) >> 12;
    do
    {
        quotient = number / 10;
        color = (((sint16)instruments->number_color));
        hud_digit_draw((uint32)number - (uint32)quotient * 10, digit_x, 137, 1, &spr[(digit_packet - dst) / sizeof(SPRT)], color);
        number = quotient;
        tag = spr[(digit_packet - dst) / sizeof(SPRT)].tag;
        head = (*ot);
        spr[(digit_packet - dst) / sizeof(SPRT)].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (digit_packet & 0xFFFFFF));
        if (number == 0)
            break;
        digit_x -= 10;
        ++i;
        digit_packet += 20;
    } while (i < 4);
    spr[0].rgbc.r = (uint8)(((instruments->secondary_color[0])));
    spr[0].rgbc.g = (uint8)(((instruments->secondary_color[1])));
    spr[0].rgbc.b = (uint8)(((instruments->secondary_color[2])));
    a = dst + 100;
    b = dst + 124;
    angle = ((instruments->speed));
    color = ((instruments->primary_color[0]));
    angle = (angle << 2) & 0x3FFF;
    previous = (angle - 0x600) & 0x3FFF;
    poly[1].rgbc.r = (uint8)(color);
    poly[0].rgbc.r = (uint8)(color);
    v = game_wave_at(previous * 2);
    sy0 = game_wave_at((previous + 0x1000) * 2);
    sx0 = (sint32)((uint32)v * 37) >> 16;
    sy0 = (sint32)(0 - (uint32)sy0 * 37) >> 16;
    sx1 = game_wave_at(angle * 2);
    sx1 = (sint32)((uint32)sx1 * 37) >> 16;
    sy1 = game_wave_at((angle + 0x1000) * 2);
    next = (angle + 0x600) & 0x3FFF;
    sy1 = (sint32)(0 - (uint32)sy1 * 37) >> 16;
    color = ((instruments->primary_color[1]));
    poly[1].rgbc.g = (uint8)(color);
    poly[0].rgbc.g = (uint8)(color);
    sx2 = game_wave_at(next * 2);
    sx2 = (sint32)((uint32)sx2 * 37) >> 16;
    color = ((instruments->primary_color[2]));
    poly[1].rgbc.b = (uint8)(color);
    poly[0].rgbc.b = (uint8)(color);
    poly[0].p[0].x = (sint16)(uint16)(sx0 + 20);
    poly[1].p[0].x = (sint16)(uint16)(20 - sx0);
    poly[0].p[0].y = (sint16)(uint16)(sy0 + 137);
    poly[1].p[0].y = (sint16)(uint16)(137 - sy0);
    poly[0].p[1].x = (sint16)(uint16)(sx1 + 20);
    poly[1].p[1].x = (sint16)(uint16)(20 - sx1);
    poly[0].p[1].y = (sint16)(uint16)(sy1 + 137);
    poly[1].p[1].y = (sint16)(uint16)(137 - sy1);
    poly[1].p[2].x = (sint16)(uint16)(20);
    poly[0].p[2].x = (sint16)(uint16)(20);
    poly[1].p[2].y = (sint16)(uint16)(137);
    poly[0].p[2].y = (sint16)(uint16)(137);
    poly[0].p[3].x = (sint16)(uint16)(sx2 + 20);
    sy2 = game_wave_at((next + 0x1000) * 2);
    poly[1].p[3].x = (sint16)(uint16)(20 - sx2);
    sy2 = (sint32)(0 - (uint32)sy2 * 37) >> 16;
    poly[0].p[3].y = (sint16)(uint16)(sy2 + 137);
    poly[1].p[3].y = (sint16)(uint16)(137 - sy2);
    tag = spr[0].tag;
    head = (*ot);
    spr[0].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (a & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_altitude_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->altitude;
    DR_AREA *area = bank->altitude_areas;
    uint32 packet, x, i, color, tag, head, result, field;
    sint32 number, rolling, quotient, offset;
    PSX_RECT rect;
    FUNCTION_MARKER(0x8005CB48, "ACE3.BIN");
    number = (sint32)(0 - ((uint32)((((sint32)instruments->altitude)) >> 12) << 1)) / 3;
    rolling = number / 10 % 10;
    if (number > 99999)
        number = 99999;
    else if (number < -9999)
        number = -9999;
    if (number < 0)
    {
        packet = dst + 240;
        color = (((sint16)instruments->number_color));
        hud_digit_draw(11, 243, 103, 0, &spr[(packet - dst) / sizeof(SPRT)], color);
        number = -number;
        tag = spr[(packet - dst) / sizeof(SPRT)].tag;
        head = (*ot);
        spr[(packet - dst) / sizeof(SPRT)].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    packet = dst + 140;
    x = 283;
    i = 0;
    do
    {
        quotient = number / 10;
        color = (((sint16)instruments->number_color));
        hud_digit_draw((uint32)number - (uint32)quotient * 10, x, 102, 1, &spr[(packet - dst) / sizeof(SPRT)], color);
        number = quotient;
        tag = spr[(packet - dst) / sizeof(SPRT)].tag;
        head = (*ot);
        spr[(packet - dst) / sizeof(SPRT)].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        if (number == 0)
            break;
        x -= 10;
        ++i;
        packet += 20;
    } while (i < 5);
    spr[0].rgbc.r = (uint8)(((instruments->secondary_color[0])));
    spr[0].rgbc.g = (uint8)(((instruments->secondary_color[1])));
    spr[0].rgbc.b = (uint8)(((instruments->secondary_color[2])));
    spr[1].rgbc.r = (uint8)(((instruments->secondary_color[0])));
    spr[1].rgbc.g = (uint8)(((instruments->secondary_color[1])));
    spr[1].rgbc.b = (uint8)(((instruments->secondary_color[2])));
    if (rolling < 0)
        rolling += 10;
    field = dst + 50;
    offset = -10;
    for (i = 2; i < 7; ++i)
    {
        color = ((instruments->secondary_color[0]));
        spr[i].rgbc.r = (uint8)(color);
        color = ((instruments->secondary_color[1]));
        offset += 10;
        spr[i].rgbc.g = (uint8)(color);
        color = ((instruments->secondary_color[2]));
        spr[i].p[0].xy.y = (sint16)(uint16)(offset - 10 + rolling + 96);
        spr[i].rgbc.b = (uint8)(color);
        field += 20;
    }
    rect.x = 302;
    rect.y = 96;
    rect.w = 6;
    rect.h = 40;
    if (game_runtime.display_buffer == 1)
        rect.y = 352;
    SetDrawArea(&area[0], &rect);
    rect.w = 320;
    rect.x = 0;
    rect.y = 0;
    rect.h = 240;
    if (game_runtime.display_buffer == 1)
        rect.y = 256;
    SetDrawArea(&area[1], &rect);
    tag = spr[1].tag;
    spr[1].tag = (uint32)((tag & 0xFF000000) | ((dst + 260) & 0xFFFFFF));
    tag = area[0].tag;
    area[0].tag = (uint32)((tag & 0xFF000000) | ((dst + 40) & 0xFFFFFF));
    tag = spr[6].tag;
    spr[6].tag = (uint32)((tag & 0xFF000000) | ((dst + 272) & 0xFFFFFF));
    tag = area[1].tag;
    head = (*ot);
    area[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_timer_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->timer;
    uint32 color = 1, tag, head, result, blank;
    sint32 mode, frames, major, minor;
    FUNCTION_MARKER(0x8005D000, "ACE3.BIN");
    mode = (((sint16)instruments->timer_mode));
    if (mode == 1)
    {
        frames = (((sint32)instruments->timer_ticks));
        if (frames < 1800)
        {
            major = frames / 30;
            minor = (frames % 30) * 100 / 30;
        }
        else
        {
            frames = (((sint32)instruments->timer_ticks));
            major = frames / 1800;
            minor = frames / 30 - major * 60;
        }
    }
    else
    {
        frames = (((sint32)instruments->timer_ticks));
        major = frames / 1800;
        minor = frames / 30 - major * 60;
    }
    if ((((sint16)instruments->timer_mode)) != 1 && ((instruments->secondary_color[0])) == 0)
        color = 0;
    hud_digit_draw(major / 10, 24, 211, 1, &spr[2], color);
    hud_digit_draw(major % 10, 34, 211, 1, &spr[3], color);
    hud_digit_draw(10, 44, 211, 1, &spr[4], color);
    hud_digit_draw(minor / 10, 49, 211, 1, &spr[5], color);
    hud_digit_draw(minor % 10, 59, 211, 1, &spr[6], color);
    if ((((sint16)instruments->timer_mode)) == 1)
    {
        spr[0].rgbc.r = (uint8)(131);
        spr[0].rgbc.g = (uint8)(0);
        spr[0].rgbc.b = (uint8)(0);
    }
    else
    {
        spr[0].rgbc.r = (uint8)(((instruments->secondary_color[0])));
        spr[0].rgbc.g = (uint8)(((instruments->secondary_color[1])));
        spr[0].rgbc.b = (uint8)(((instruments->secondary_color[2])));
    }
    if ((((sint16)instruments->timer_mode)) == 1)
    {
        blank = ((instruments->timer_ticks)) & 4;
        spr[1].rgbc.r = (uint8)(blank ? 0 : 131);
        spr[1].rgbc.g = (uint8)(0);
        spr[1].rgbc.b = (uint8)(0);
    }
    else
    {
        frames = (sint32)(((instruments->timer_ticks)) << 1);
        blank = (frames / 15) & 1;
        if (blank)
        {
            spr[1].rgbc.r = (uint8)(0);
            spr[1].rgbc.g = (uint8)(0);
            spr[1].rgbc.b = (uint8)(0);
        }
        else
        {
            spr[1].rgbc.r = (uint8)(((instruments->secondary_color[0])));
            spr[1].rgbc.g = (uint8)(((instruments->secondary_color[1])));
            spr[1].rgbc.b = (uint8)(((instruments->secondary_color[2])));
        }
    }
    tag = spr[6].tag;
    head = (*ot);
    spr[6].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_color_marks_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->color_marks;
    LINE_F2 *line = bank->color_mark_lines;
    uint32 field, i, tag, head, result;
    FUNCTION_MARKER(0x8005DE40, "ACE3.BIN");
    field = dst + 6;
    for (i = 0; i < 2; ++i)
    {
        spr[i].rgbc.r = (uint8)(((instruments->secondary_color[0])));
        spr[i].rgbc.g = (uint8)(((instruments->secondary_color[1])));
        spr[i].rgbc.b = (uint8)(((instruments->secondary_color[2])));
        field += 20;
    }
    field = dst + 46;
    for (i = 0; i < 3; ++i)
    {
        line[i].rgbc.r = (uint8)(((instruments->primary_color[0])) >> 1);
        line[i].rgbc.g = (uint8)(((instruments->primary_color[1])) >> 1);
        line[i].rgbc.b = (uint8)(((instruments->primary_color[2])) >> 1);
        field += 16;
    }
    tag = line[2].tag;
    head = (*ot);
    line[2].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (dst & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_gauge_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->gauge;
    SPRT *cur;
    uint32 color, tag, head, packet, field, i = 0, result, dim;
    sint32 count, value, delta;
    FUNCTION_MARKER(0x8005D998, "ACE3.BIN");
    color = ((instruments->secondary_color[0]));
    tag = spr->tag;
    spr->rgbc.r = (uint8)(color);
    spr->rgbc.g = (uint8)(((instruments->secondary_color[1])));
    spr->rgbc.b = (uint8)(((instruments->secondary_color[2])));
    head = (*ot);
    spr->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    *ot = ((head & 0xFF000000) | (dst & 0xFFFFFF));
    count = (((sint32)instruments->gauge_count));
    value = (((sint32)instruments->gauge_value));
    packet = dst + 20;
    delta = ((uint32)count - (uint32)value);
    if (count > 0)
    {
        field = packet + 6;
        do
        {
            cur = packet - dst < sizeof(bank->gauge) && (packet - dst) % sizeof(SPRT) == 0 ? &bank->gauge[(packet - dst) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(SPRT));
            dim = (sint32)i < (((sint32)instruments->gauge_dim_count)) || (sint32)i < delta;
            if (dim)
            {
                cur->rgbc.r = (uint8)(((instruments->secondary_color[0])) >> 2);
                cur->rgbc.g = (uint8)(((instruments->secondary_color[1])) >> 2);
                color = ((instruments->secondary_color[2])) >> 2;
            }
            else
            {
                cur->rgbc.r = (uint8)(((instruments->secondary_color[0])));
                cur->rgbc.g = (uint8)(((instruments->secondary_color[1])));
                color = ((instruments->secondary_color[2]));
            }
            cur->rgbc.b = (uint8)(color);
            field += 20;
            ++i;
            tag = cur->tag;
            head = (*ot);
            cur->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot);
            *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = (((sint32)instruments->gauge_count));
            packet += 20;
        } while ((sint32)i < count);
        value = (((sint32)instruments->gauge_value));
    }
    if (value >= 100)
    {
        packet = dst + 140;
        cur = &bank->gauge[7];
        color = (((sint16)instruments->number_color));
        hud_digit_draw(1, 237, 200, 1, cur, color);
        tag = cur->tag;
        head = (*ot);
        cur->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    value = (((sint32)instruments->gauge_value));
    if (value >= 10)
    {
        packet = dst + 160;
        cur = &bank->gauge[8];
        color = (((sint16)instruments->number_color));
        hud_digit_draw(value / 10 % 10, 243, 200, 1, cur, color);
        tag = cur->tag;
        head = (*ot);
        cur->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    value = (((sint32)instruments->gauge_value));
    packet = dst + 180;
    cur = &bank->gauge[9];
    color = (((sint16)instruments->number_color));
    hud_digit_draw(value % 10, 251, 200, 1, cur, color);
    tag = cur->tag;
    head = (*ot);
    cur->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (packet & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_health_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank)
{
    uint32 *ot = (uint32 *)mission_ot_span(object, sizeof(uint32));
    SPRT *spr = bank->health;
    uint32 health, percent, value, red, green, high, product, quotient, step, packet, tag, head, flags, color, result;
    FUNCTION_MARKER(0x8005D468, "ACE3.BIN");
    value = (instruments->health);
    health = 4096;
    if ((sint32)value < 4097)
    {
        health = 0;
        if ((sint32)value >= 0)
            health = value;
    }
    value = (instruments->secondary_color[0]);
    spr[0].rgbc.r = (uint8)(value);
    value = (instruments->secondary_color[1]);
    spr[0].rgbc.g = (uint8)(value);
    percent = ((sint32)(health * 100) >> 12);
    value = (instruments->secondary_color[2]);
    spr[0].rgbc.b = (uint8)(value);
    value = hud_control.alert_mode;
    if (value != 0 && (((instruments->flags)) & 32) != 0)
    {
        value = (instruments->secondary_color[0]);
        spr[1].rgbc.r = (uint8)(value);
        value = (instruments->secondary_color[1]);
        spr[1].rgbc.g = (uint8)(value);
        value = (instruments->secondary_color[2]);
        spr[1].rgbc.b = (uint8)(value);
    }
    else
    {
        product = percent * 131;
        high = (((sint64)(sint32)product * 0x51EB851F) >> 32);
        green = (uint32)((sint32)high >> 5) - (uint32)((sint32)product >> 31);
        red = 131 - green;
        if ((sint32)percent < 20)
        {
            flags = (instruments->timer_ticks);
            if ((flags & 1) != 0)
                red = (red & 255) >> 2;
        }
        spr[1].rgbc.r = (uint8)(red);
        spr[1].rgbc.g = (uint8)(green);
        spr[1].rgbc.b = (uint8)(0);
    }
    step = 15 - (uint32)((sint32)health >> 8);
    if ((sint32)step < 0)
        step = 0;
    spr[1].p[0].attr = (uint16)((step & 3) + ((step & 12) << 4) + 0x7E18);
    tag = spr[1].tag;
    head = (*ot);
    spr[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    *ot = ((head & 0xFF000000) | (dst & 0xFFFFFF));
    flags = (instruments->flags);
    if ((flags & 32) != 0)
    {
        value = hud_control.alert_mode;
        if (value != 0)
        {
            value = (instruments->secondary_color[0]);
            spr[2].rgbc.r = (uint8)(value);
            value = (instruments->secondary_color[1]);
            spr[2].rgbc.g = (uint8)(value);
            value = (instruments->secondary_color[2]);
            spr[2].rgbc.b = (uint8)(value);
        }
        else
        {
            spr[2].rgbc.r = (uint8)(red);
            spr[2].rgbc.g = (uint8)(green);
            spr[2].rgbc.b = (uint8)(0);
        }
        tag = spr[2].tag;
        head = (*ot);
        spr[2].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | ((dst + 40) & 0xFFFFFF));
    }
    percent = 100 - percent;
    packet = dst + 60;
    if ((sint32)percent >= 100)
    {
        color = ((sint16)instruments->number_color);
        hud_digit_draw(1, 238, 219, 1, &spr[(packet - dst) / sizeof(SPRT)], color);
        tag = spr[3].tag;
        head = (*ot);
        spr[3].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        packet = dst + 80;
        *ot = ((head & 0xFF000000) | ((dst + 60) & 0xFFFFFF));
    }
    if ((sint32)percent >= 10)
    {
        high = (((sint64)(sint32)percent * 0x66666667) >> 32);
        quotient = (uint32)((sint32)high >> 2) - (uint32)((sint32)percent >> 31);
        high = (((sint64)(sint32)quotient * 0x66666667) >> 32);
        value = (uint32)((sint32)high >> 2) - (uint32)((sint32)quotient >> 31);
        value = quotient - value * 10;
        color = ((sint16)instruments->number_color);
        hud_digit_draw(value, 244, 219, 1, &spr[(packet - dst) / sizeof(SPRT)], color);
        tag = spr[(packet - dst) / sizeof(SPRT)].tag;
        head = (*ot);
        spr[(packet - dst) / sizeof(SPRT)].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot);
        *ot = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 20;
    }
    high = (((sint64)(sint32)percent * 0x66666667) >> 32);
    quotient = (uint32)((sint32)high >> 2) - (uint32)((sint32)percent >> 31);
    value = percent - quotient * 10;
    color = ((sint16)instruments->number_color);
    hud_digit_draw(value, 252, 219, 1, &spr[(packet - dst) / sizeof(SPRT)], color);
    tag = spr[(packet - dst) / sizeof(SPRT)].tag;
    head = (*ot);
    spr[(packet - dst) / sizeof(SPRT)].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot);
    result = (head & 0xFF000000) | (packet & 0xFFFFFF);
    *ot = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_aim_update(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    MATRIX matrix;
    uint32 high[3], rotated[3], flags, index, i, position, speed, numerator, divisor, delay, texture, slot, value, base, packet, xy;
    uint32 *history;
    uint16 vector[3];
    MISSION_ACTOR_TARGET *target;
    SPRT *prim;
    FUNCTION_MARKER(0x800662B4, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    rotation_build(&matrix, pose_owner_object->pose.angles);
    i = 4;
    SetRotMatrix(&matrix);
    xport_gte_write_data(9, 0);
    xport_gte_write_data(10, 0);
    xport_gte_write_data(11, 0x1580000 >> 15);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    xport_gte_write_data(9, 0);
    xport_gte_write_data(10, 0);
    xport_gte_write_data(11, 0);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        rotated[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    flags = game_runtime.flags;
    if ((flags & 0x20000000) == 0)
    {
        index = hud_control.aim_head;
        hud_control.aim_head = index + 1;
    }
    index = hud_control.aim_head;
    if (index >= 32)
        hud_control.aim_head = 0;
    index = hud_control.aim_head;
    history = hud_control.aim_history[index];
    for (i = 0; i < 3; ++i)
    {
        position = pose_owner_object->pose.position[i];
        speed = actor_motion_object->velocity[i];
        history[i] = position + rotated[i] + (speed << 5);
    }
    value = hud_control.selected_target_idx;
    target = mission_actor_target(value);
    if (target == 0)
        return;
    numerator = (target->distance);
    divisor = hud_control.aim_distance_parameter;
    numerator <<= 5;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    delay = (sint32)numerator / (sint32)divisor;
    if ((sint32)delay >= 32)
        delay = 31;
    texture = (sint32)delay >> 1;
    texture = (texture & 3) + ((texture & 12) << 4);
    hud_control.aim_slot = (index - delay) & 31;
    game_build_rotation_via_gte(&matrix, game_runtime.camera_view.pose.angles);
    slot = hud_control.aim_slot;
    history = hud_control.aim_history[slot];
    for (i = 0; i < 3; ++i)
    {
        position = history[i];
        value = game_runtime.camera_view.pose.position[i];
        vector[i] = (sint32)(position - value) >> 12;
    }
    i = 4;
    SetRotMatrix(&matrix);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    xy = xport_gte_read_data(14);
    xport_gte_read_data(8);
    flags = xport_gte_read_control(31);
    xport_gte_read_data(19);
    if ((sint32)flags < 0)
        return;
    value = (target->flags) & 0x20;
    if (value == 0)
    {
        (void)(uint32)(value);
        return;
    }
    base = hud_control.flight_packets.cur;
    value = hud_control.dim_color[0];
    packet = base + 0x940;
    prim = &hud_control.flight_packets.native->indicator;
    prim->rgbc.r = (uint8)(value);
    value = hud_control.dim_color[1];
    prim->rgbc.g = (uint8)(value);
    value = hud_control.dim_color[2];
    prim->p[0].attr = (uint16)(texture + 0x7E18);
    prim->rgbc.b = (uint8)(value);
    prim->p[0].xy.x = (sint16)(uint16)((xy & 0xFFFF) - 10);
    prim->p[0].xy.y = (sint16)(uint16)((xy >> 16) - 10);
    value = hud_control.flight_packets.used[HUD_FLIGHT_FIXED_CHAIN] + 1;
    hud_control.flight_packets.used[HUD_FLIGHT_FIXED_CHAIN] = value;
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 hud_waveform_draw(uint32 packet, uint32 ordering)
{
    POLY_G4 *quad;
    DR_MODE *end_mode;
    uint32 *ot_word = (uint32 *)mission_ot_span(ordering, sizeof(uint32));
    uint8 waveform[4][38] = {0};
    uint32 time, side, i, value, high, selector, noise, sample, phase, first, second, x, y, next, base, curve, curve_next, segment, head, tag;
    sint64 product;
    FUNCTION_MARKER(0x8005A2C4, "ACE3.BIN");
    time = game_runtime.frame_counter;
    if ((sint32)time % 80 == 79)
    {
        for (side = 0; side < 4; ++side)
            for (i = 0; i < 38; ++i)
            {
                value = game_random() * 8;
                product = (sint64)(sint32)value * (sint32)0x80010003;
                high = (product >> 32);
                sample = (uint32)((sint32)(high + value) >> 14) - (uint32)((sint32)value >> 31);
                hud_control.waveform_noise[side][i] = sample;
                value = game_random() << 2;
                product = (sint64)(sint32)value * (sint32)0x80010003;
                high = (product >> 32);
                sample = (uint32)((sint32)(high + value) >> 14) - (uint32)((sint32)value >> 31);
                hud_control.waveform_curve[side] = (uint8)sample;
            }
    }
    time = game_runtime.frame_counter;
    phase = ((sint32)time % 80) / 8;
    for (side = 0; side < 4; ++side)
        for (i = 0; i < 38; ++i)
        {
            selector = hud_control.waveform_curve[side];
            noise = hud_control.waveform_noise[side][i];
            sample = (selector == HUD_WAVEFORM_FEEDBACK ? waveform[0][i] : hud_waveform_curves[selector][i]);
            if ((sint32)phase % 10 < 5)
                value = (uint32)((sint32)(phase * (sample + noise)) / 5) + 16;
            else
            {
                value = ((sint32)((phase - 5) * (sample + noise)) / 5);
                sample = (selector == HUD_WAVEFORM_FEEDBACK ? waveform[0][i] : hud_waveform_curves[selector][i]);
                value = sample + noise + 16 - value;
            }
            waveform[side][i] = value;
        }
    for (side = 0; side < 4; ++side)
    {
        if (side < 2)
        {
            x = side == 0 ? 32 : 288;
            for (i = 0; i < 36; ++i, packet += 36)
            {
                quad = (POLY_G4 *)mission_packet_span(packet, sizeof(POLY_G4));
                y = 100 + i * 8;
                next = y + 8;
                first = waveform[side][i];
                second = waveform[side][i + 1];
                quad->p[0].xy.xy = (uint32)((y << 16) | x);
                quad->p[2].xy.xy = (uint32)((next << 16) | x);
                quad->p[0].rgbc.rgbc = (uint32)(0x3AFFFFFF);
                quad->p[1].rgbc.rgbc = (uint32)(0x00030303);
                quad->p[2].rgbc.rgbc = (uint32)(0x00FFFFFF);
                quad->p[3].rgbc.rgbc = (uint32)(0x00030303);
                quad->p[1].xy.xy = (uint32)((y << 16) | (side == 0 ? first + x : x - first));
                quad->p[3].xy.xy = (uint32)((next << 16) | (side == 0 ? second + x : x - second));
                head = (*ot_word);
                quad->tag = (uint32)(head | 0x08000000);
                *ot_word = (packet & 0xFFFFFF);
            }
            for (segment = 0; segment < 2; ++segment)
                for (i = 0; i < 8; ++i, packet += 36)
                {
                    quad = (POLY_G4 *)mission_packet_span(packet, sizeof(POLY_G4));
                    base = segment == 0 ? 100 : 348;
                    y = base + i * 4;
                    next = base + (i + 1) * 4;
                    curve = i * i / 2;
                    curve_next = (i + 1) * (i + 1) / 2;
                    if (segment == 0)
                    {
                        curve = curve - i * 8 + 32;
                        curve_next = curve_next - (i + 1) * 8 + 32;
                    }
                    first = side == 0 ? x + (uint32)(sint32)(sint16)curve : x - (uint32)(sint32)(sint16)curve;
                    second = side == 0 ? x + (uint32)(sint32)(sint16)curve_next : x - (uint32)(sint32)(sint16)curve_next;
                    quad->p[0].rgbc.rgbc = (uint32)(0x3AFFFFFF);
                    quad->p[2].rgbc.rgbc = (uint32)(0x3AFFFFFF);
                    quad->p[1].rgbc.rgbc = (uint32)(0x3A030303);
                    quad->p[3].rgbc.rgbc = (uint32)(0x3A030303);
                    quad->p[0].xy.xy = (uint32)((y << 16) | x);
                    quad->p[1].xy.xy = (uint32)((y << 16) | first);
                    quad->p[2].xy.xy = (uint32)((next << 16) | x);
                    quad->p[3].xy.xy = (uint32)((next << 16) | second);
                    head = (*ot_word);
                    quad->tag = (uint32)(head | 0x08000000);
                    *ot_word = (packet & 0xFFFFFF);
                }
        }
        else
        {
            base = side == 2 ? 100 : 380;
            for (i = 0; i < 33; ++i, packet += 36)
            {
                quad = (POLY_G4 *)mission_packet_span(packet, sizeof(POLY_G4));
                x = 32 + i * 8;
                next = x + 8;
                second = waveform[side][i + 1];
                first = waveform[side][i];
                quad->p[0].xy.xy = (uint32)((base << 16) | x);
                quad->p[2].xy.xy = (uint32)((base << 16) | next);
                quad->p[0].rgbc.rgbc = (uint32)(0x3AFFFFFF);
                quad->p[1].rgbc.rgbc = (uint32)(0x00030303);
                quad->p[2].rgbc.rgbc = (uint32)(0x00FFFFFF);
                quad->p[3].rgbc.rgbc = (uint32)(0x00030303);
                y = side == 2 ? base + first : base - first;
                value = side == 2 ? base + second : base - second;
                quad->p[1].xy.xy = (uint32)((y << 16) | x);
                quad->p[3].xy.xy = (uint32)((value << 16) | next);
                head = (*ot_word);
                quad->tag = (uint32)(head | 0x08000000);
                *ot_word = (packet & 0xFFFFFF);
            }
        }
    }
    end_mode = (DR_MODE *)mission_packet_span(packet, 8);
    setlen(end_mode, 1);
    end_mode->code[0] = (uint32)(0xE1000240);
    tag = end_mode->tag;
    head = (*ot_word);
    end_mode->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 8;
}

void hud_notice_reset_pair(void)
{
    FUNCTION_MARKER(0x80042524, "ACE3.BIN");
    hud_control.notice_priority = 0xFFFFFFFF;
    hud_control.notice_ticks = 0;
    hud_control.notice_pair[0].active = 0;
    hud_control.notice_pair[1].active = 0;
}

void hud_notice_reset(void)
{
    FUNCTION_MARKER(0x800428EC, "ACE3.BIN");
    hud_control.notice_single.active = 0;
}

void hud_notice_show_pair(uint32 index)
{
    uint32 current, value;
    const HUD_NOTICE_PAIR *record;
    FUNCTION_MARKER(0x8004287C, "ACE3.BIN");
    current = hud_control.notice_priority;
    if (current != 0xFFFFFFFF && (sint32)index >= (sint32)current)
        return;
    if (index >= HUD_NOTICE_PAIR_COUNT)
        abort();
    record = &notice_pairs[index];
    hud_control.notice_priority = index;
    value = record->ticks;
    hud_control.notice_pair[0].active = 1;
    hud_control.notice_ticks = value;
    value = record->kind[0];
    hud_control.notice_pair[0].ticks = 0;
    hud_control.notice_pair[0].kind = value;
    hud_control.notice_pair[1].active = 1;
    value = record->kind[1];
    hud_control.notice_pair[1].ticks = 0;
    hud_control.notice_pair[1].kind = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_notice_show(uint32 kind)
{
    FUNCTION_MARKER(0x8004291C, "ACE3.BIN");
    hud_control.notice_single.active = 1;
    hud_control.notice_single.kind = kind + 12;
    hud_control.notice_single.ticks = 0;
}

uint32 hud_notice_draw(HUD_NOTICE *notice)
{
    SPRT *spr;
    DR_TPAGE *draw_mode;
    uint32 *ot_word;
    uint32 active, ot, index, value, limit, mode, extra, buffers, count;
    uint32 icon, packet, x, y, tag, head, draw, color, i;
    const HUD_NOTICE_SPEC *record;
    const HUD_NOTICE_ICON *texture;
    FUNCTION_MARKER(0x80041F88, "ACE3.BIN");
    active = ((notice->active));
    ot = game_runtime.mission_control.ot;
    if (active == 0)
        return 0;
    if (((notice->kind)) == 255)
        return 0;
    index = ((notice->kind));
    if (index >= HUD_NOTICE_COUNT)
        abort();
    record = &notice_specs[index];
    if ((game_runtime.flags & 0x20000000) == 0)
    {
        value = (((sint16)notice->ticks));
        limit = record->sound_tick;
        if (value == limit && mission_status_text_current() == 255)
        {
            value = record->sound;
            sound_play(value);
        }
        value = ((notice->ticks)) + 1;
        notice->ticks = value;
        limit = record->duration;
        if ((sint16)value >= (sint32)limit)
        {
            notice->active = 0;
        }
    }
    mode = record->mode;
    extra = mode & 0xF0;
    mode &= 15;
    if (mode == 1)
    {
        if ((((notice->ticks)) & 6) == 0)
            return ((notice->active));
    }
    else if (mode == 2)
    {
        if ((((notice->ticks)) & 2) == 0)
            return ((notice->active));
    }
    ot_word = (uint32 *)mission_ot_span(ot, sizeof(uint32));
    buffers = hud_control.notice_packets.arena_begin;
    count = hud_control.notice_packets.spr_count;
    icon = record->icon[0];
    packet = hud_control.notice_packets.cur + count * 20 + 104;
    for (i = 0; i < 2; ++i)
    {
        if (i != 0)
        {
            icon = record->icon[1];
            if (icon == 255)
                break;
            packet += 20;
        }
        spr = hud_control.notice_packets.native && (packet - hud_control.notice_packets.cur - 104) <= sizeof(hud_control.notice_packets.native->notice) - sizeof(SPRT) && (packet - hud_control.notice_packets.cur - 104) % sizeof(SPRT) == 0 ? &hud_control.notice_packets.native->notice[(packet - hud_control.notice_packets.cur - 104) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(SPRT));
        spr->rgbc.r = (uint8)(80);
        spr->rgbc.g = (uint8)(80);
        spr->rgbc.b = (uint8)(80);
        spr->p[0].attr = (uint16)(0x7F9B);
        value = record->offset[i];
        x = record->x;
        spr->p[0].xy.x = (sint16)(uint16)(x + value);
        texture = &notice_icons[icon];
        y = record->y;
        tag = spr->tag;
        spr->p[0].xy.y = (sint16)(uint16)(y + 2);
        value = texture->u;
        spr->p[0].uv.u = (uint8)(value);
        value = texture->v;
        spr->p[0].uv.v = (uint8)(value);
        value = texture->w;
        spr->w = (sint16)(uint16)(value);
        value = texture->h;
        spr->h = (sint16)(uint16)(value);
        head = (*ot_word);
        buffers = hud_control.notice_packets.arena_begin;
        spr->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        value = hud_control.notice_packets.spr_count;
        hud_control.notice_packets.spr_count = value + 1;
    }
    if (extra != 0)
    {
        color = record->color;
        packet += 20;
        spr = hud_control.notice_packets.native && (packet - hud_control.notice_packets.cur - 104) <= sizeof(hud_control.notice_packets.native->notice) - sizeof(SPRT) && (packet - hud_control.notice_packets.cur - 104) % sizeof(SPRT) == 0 ? &hud_control.notice_packets.native->notice[(packet - hud_control.notice_packets.cur - 104) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(SPRT));
        if (color == 0)
        {
            spr->rgbc.r = (uint8)(128);
            spr->rgbc.g = (uint8)(0);
            spr->rgbc.b = (uint8)(0);
        }
        else if (color == 1)
        {
            spr->rgbc.r = (uint8)(128);
            spr->rgbc.g = (uint8)(64);
            spr->rgbc.b = (uint8)(0);
        }
        else if (color == 2)
        {
            spr->rgbc.r = (uint8)(0);
            spr->rgbc.g = (uint8)(0);
            spr->rgbc.b = (uint8)(128);
        }
        else
        {
            spr->rgbc.r = (uint8)(255);
            spr->rgbc.g = (uint8)(255);
            spr->rgbc.b = (uint8)(255);
        }
        spr->p[0].attr = (uint16)(0x7F9B);
        value = record->x;
        spr->p[0].xy.x = (sint16)(uint16)(value + 28);
        value = record->y;
        value += extra == 16 ? (uint32)-10 : 10;
        spr->p[0].xy.y = (sint16)(uint16)(value);
        spr->p[0].uv.u = (uint8)(28);
        spr->w = (sint16)(uint16)(74);
        tag = spr->tag;
        spr->p[0].uv.v = (uint8)(0);
        spr->h = (sint16)(uint16)(9);
        head = (*ot_word);
        spr->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        buffers = hud_control.notice_packets.arena_begin;
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        value = hud_control.notice_packets.spr_count;
        hud_control.notice_packets.spr_count = value + 1;
    }
    buffers = hud_control.notice_packets.arena_begin;
    count = hud_control.notice_packets.mode_count;
    draw = hud_control.notice_packets.cur + (count << 3) + 0x1A8;
    draw_mode = hud_control.notice_packets.native && (draw - hud_control.notice_packets.cur - 424) <= sizeof(hud_control.notice_packets.native->notice_modes) - sizeof(DR_TPAGE) && (draw - hud_control.notice_packets.cur - 424) % sizeof(DR_TPAGE) == 0 ? &hud_control.notice_packets.native->notice_modes[(draw - hud_control.notice_packets.cur - 424) / sizeof(DR_TPAGE)] : (DR_TPAGE *)mission_packet_span(draw, 8);
    draw_mode->code[0] = (uint32)(0xE1000636);
    setlen(draw_mode, 1);
    tag = draw_mode->tag;
    head = (*ot_word);
    draw_mode->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    buffers = hud_control.notice_packets.arena_begin;
    *ot_word = ((head & 0xFF000000) | (draw & 0xFFFFFF));
    value = hud_control.notice_packets.mode_count;
    hud_control.notice_packets.mode_count = value + 1;
    color = record->color;
    packet += 20;
    spr = hud_control.notice_packets.native && (packet - hud_control.notice_packets.cur - 104) <= sizeof(hud_control.notice_packets.native->notice) - sizeof(SPRT) && (packet - hud_control.notice_packets.cur - 104) % sizeof(SPRT) == 0 ? &hud_control.notice_packets.native->notice[(packet - hud_control.notice_packets.cur - 104) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(SPRT));
    if (color == 0)
    {
        spr->rgbc.r = (uint8)(128);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 1)
    {
        spr->rgbc.r = (uint8)(128);
        spr->rgbc.g = (uint8)(64);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 2)
    {
        spr->rgbc.r = (uint8)(0);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(128);
    }
    else
    {
        spr->rgbc.r = (uint8)(255);
        spr->rgbc.g = (uint8)(255);
        spr->rgbc.b = (uint8)(255);
    }
    draw += 8;
    draw_mode = hud_control.notice_packets.native && (draw - hud_control.notice_packets.cur - 424) <= sizeof(hud_control.notice_packets.native->notice_modes) - sizeof(DR_TPAGE) && (draw - hud_control.notice_packets.cur - 424) % sizeof(DR_TPAGE) == 0 ? &hud_control.notice_packets.native->notice_modes[(draw - hud_control.notice_packets.cur - 424) / sizeof(DR_TPAGE)] : (DR_TPAGE *)mission_packet_span(draw, 8);
    spr->p[0].attr = (uint16)(0x7F9B);
    value = record->x;
    spr->p[0].xy.x = (sint16)(uint16)(value);
    y = record->y;
    spr->p[0].uv.u = (uint8)(0);
    spr->p[0].uv.v = (uint8)(10);
    buffers = hud_control.notice_packets.arena_begin;
    spr->w = (sint16)(uint16)(132);
    spr->h = (sint16)(uint16)(9);
    spr->p[0].xy.y = (sint16)(uint16)(y);
    tag = spr->tag;
    head = (*ot_word);
    spr->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    value = hud_control.notice_packets.spr_count;
    hud_control.notice_packets.spr_count = value + 1;
    setlen(draw_mode, 1);
    tag = draw_mode->tag;
    draw_mode->code[0] = (uint32)(0xE1000616);
    head = (*ot_word);
    draw_mode->tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    buffers = hud_control.notice_packets.arena_begin;
    *ot_word = ((head & 0xFF000000) | (draw & 0xFFFFFF));
    value = hud_control.notice_packets.mode_count;
    hud_control.notice_packets.mode_count = value + 1;
    return ((notice->active));
}

static void hud_radar_update_distances(const HUD_RADAR_REFERENCE *reference)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags, value, other, dx, dz, magnitude, ax, az;
    FUNCTION_MARKER(0x8003D138, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        flags = (*mission_actor_flags(index));
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((flags & 0x2000) == 0 || (actor_traits_object->flags & 0x40))
        {
            hud_radar_contact(index)->distance = (uint32)(0xFFFFFFFF);
            continue;
        }
        value = pose_owner_object->pose.position[0];
        other = reference->x;
        dx = ((sint32)value >> 12) - other;
        value = pose_owner_object->pose.position[2];
        other = reference->z;
        dz = other - ((sint32)value >> 12);
        ax = (sint32)dx < 0 ? 0 - dx : dx;
        az = (sint32)dz < 0 ? 0 - dz : dz;
        if ((sint32)ax < 0x4000 && (sint32)az < 0x4000)
            magnitude = game_square_root(dx * dx + dz * dz);
        else
        {
            dx = (sint32)dx >> 4;
            dz = (sint32)dz >> 4;
            magnitude = game_square_root(dx * dx + dz * dz) << 4;
        }
        hud_radar_contact(index)->distance = (uint32)(magnitude);
    }
    return;
}

void hud_radar_update_scale(void)
{
    uint32 flags, selected, distance, value, index, old, next;
    FUNCTION_MARKER(0x8003D280, "ACE3.BIN");
    flags = game_input.held;
    selected = hud_control.selected_target_idx;
    distance = 0x40000;
    if (flags & 0x8000)
    {
        if (selected != 255 && ((*mission_actor_flags(selected)) & 0x100))
        {
            value = hud_radar_contact(selected)->distance;
            if (value != 0xFFFFFFFF)
            {
                distance = value;
                goto scaled;
            }
        }
        for (index = 2; index < 64; ++index)
        {
            if (((*mission_actor_flags(index)) & 0x100) == 0)
                continue;
            value = hud_radar_contact(index)->distance;
            if (value != 0xFFFFFFFF && (sint32)value < (sint32)distance)
                distance = value;
        }
    scaled:
        distance += (sint32)distance >> 2;
    }
    else
    {
        if (selected != 255)
        {
            value = hud_radar_contact(selected)->distance;
            if (value != 0xFFFFFFFF)
                distance = value;
        }
        else
        {
            for (index = 2; index < 32; ++index)
            {
                value = hud_radar_contact(index)->distance;
                if (value != 0xFFFFFFFF && (sint32)value < (sint32)distance)
                    distance = value;
            }
        }
        if ((sint32)distance < 0x3000)
            distance = 0x4000;
        else if ((sint32)distance < 0x6000)
            distance = 0x8000;
        else
            distance = 0x10000;
    }
    old = hud_control.map_scale;
    next = (sint32)(old + distance) >> 1;
    if (next & 1)
    {
        if ((sint32)old < (sint32)next)
            ++next;
        else
            --next;
    }
    if ((sint32)next < 0x2000)
        next = 0x2000;
    if ((sint32)next > 0x40000)
        next = 0x40000;
    hud_control.map_scale = next;
    return;
}

static void hud_radar_update(const HUD_RADAR_REFERENCE *reference)
{
    FUNCTION_MARKER(0x8003D9C8, "ACE3.BIN");
    hud_radar_update_contacts(reference);
    if ((sint32)hud_control.map_scale > 0x8000)
        return;
    {
        (void)(hud_radar_update_markers(reference));
        return;
    }
}

static void hud_radar_update_markers(const HUD_RADAR_REFERENCE *reference)
{
    EFFECT_OBJECT *object;
    uint32 scale, index, flags, value, base, cosine, sine, dx, dz, numerator, x, y;
    HUD_RADAR_MARK *record;
    FUNCTION_MARKER(0x8003DCF0, "ACE3.BIN");
    scale = hud_control.map_scale;
    for (index = 0; index < 32; ++index)
    {
        record = &hud_control.radar_markers[index];
        record->flags = (uint8)(0);
        flags = effect_objects.live;
        if ((flags & (1u << index)) == 0)
            continue;
        object = effect_objects.slot + index;
        value = object->pose.position[0];
        base = reference->x;
        cosine = reference->cosine;
        dx = ((sint32)value >> 12) - base;
        value = object->pose.position[2];
        base = reference->z;
        sine = reference->sine;
        dz = base - ((sint32)value >> 12);
        numerator = dx * cosine + dz * sine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        x = (sint32)numerator / (sint32)scale;
        numerator = (0 - dx) * sine + dz * cosine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        y = (sint32)numerator / (sint32)scale;
        if (x + 31 >= 63 || y + 31 >= 63)
            continue;
        record->y = (uint8)(y + 46);
        value = record->flags;
        record->x = (uint8)(x + 44);
        record->shape = (uint8)(2);
        record->color = (uint8)(0);
        record->flags = (uint8)(value | 1);
    }
    return;
}

static void hud_radar_update_contacts(const HUD_RADAR_REFERENCE *reference)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_player;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 scale, index, value, flags, base, cosine, sine;
    HUD_RADAR_MARK *record;
    uint32 dx, dz, numerator, x, y, old, height, intensity;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003DA08, "ACE3.BIN");
    scale = hud_control.map_scale;
    actor_index_table = 0;
    pose_owner_player = mission_actor(actor_index_table);
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        record = hud_radar_contact(index);
        value = record->flags;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        record->flags = (uint8)(value & 0xF0);
        if ((actor_traits_object->flags & 0x4000) == 0)
            continue;
        flags = (*mission_actor_flags(index));
        if ((flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        if ((sint32)scale < (sint32)record->distance)
            continue;
        value = pose_owner_object->pose.position[0];
        base = reference->x;
        cosine = reference->cosine;
        dx = ((sint32)value >> 12) - base;
        value = pose_owner_object->pose.position[2];
        base = reference->z;
        sine = reference->sine;
        dz = base - ((sint32)value >> 12);
        numerator = dx * cosine + dz * sine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        x = (sint32)numerator / (sint32)scale;
        numerator = (0 - dx) * sine + dz * cosine;
        numerator = ((uint32)((sint32)numerator >> 12)) << 5;
        if (scale == 0)
            xport_mips_break(7);
        if (scale == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        y = (sint32)numerator / (sint32)scale;
        if (x + 31 >= 63 || y + 31 >= 63)
            continue;
        old = record->flags;
        record->x = (uint8)(x + 44);
        record->y = (uint8)(y + 46);
        record->flags = (uint8)(old | 1);
        if ((sint32)scale > 0x7FFF)
            record->flags = (uint8)(old | 9);
        if (hud_control.selected_target_idx == index)
        {
            value = record->flags;
            record->flags = (uint8)(value | 4);
        }
        value = pose_owner_object->pose.position[1];
        base = pose_owner_player->pose.position[1];
        height = (sint32)(base - value) >> 12;
        if ((sint32)height > 4096)
            record->height_band = (uint8)(2);
        else if ((sint32)height < -4096)
            record->height_band = (uint8)(0);
        else
            record->height_band = (uint8)(1);
        value = (sint16)pose_owner_object->pose.angles[1];
        base = (sint16)pose_owner_player->pose.angles[1];
        value = ((value - base + 512) & 0x3FFF) >> 10;
        flags = record->flags;
        record->heading = (uint8)(value);
        if ((flags & 0x10) == 0)
            continue;
        target = mission_actor_target(index);
        if (target == 0)
            continue;
        intensity = 131;
        if (((target->flags) & 2) == 0 && ((target->projection_flags) & 0x20000) == 0)
        {
            value = (target->distance);
            intensity = 131 - ((sint32)(value - 0x8000) >> 6);
            if ((sint32)intensity >= 132)
                intensity = 131;
            if ((sint32)intensity < 19)
                intensity = 19;
        }
        record->intensity = (uint8)(intensity);
    }
    return;
}

void hud_radar_draw_marker(const HUD_RADAR_MARK *marker, HUD_GPU_BANK *bank)
{
    uint32 buffers, count, packet, value, mode, size, flags, intensity, color, width, height, x, y;
    SPRT *spr;
    FUNCTION_MARKER(0x8003DFF8, "ACE3.BIN");
    if ((((marker->flags)) & 1) == 0)
        return;
    buffers = hud_control.packets.arena_begin;
    count = hud_control.packets.spr_count;
    packet = hud_control.packets.cur + count * 20 + 24;
    spr = &bank->submitted_sprites[count];
    spr->p[0].attr = (uint16)(0x7F98);
    if (((marker->flags)) & 8)
    {
        spr->p[0].uv.u = (uint8)(98);
        spr->p[0].uv.v = (uint8)(6);
        value = ((marker->color));
        size = value == 1 ? 3 : 2;
        spr->w = (sint16)(uint16)(size);
        spr->h = (sint16)(uint16)(size);
    }
    else
    {
        mode = ((marker->shape));
        if (mode == 0)
        {
            mode = ((marker->height_band));
            if (mode == 0)
            {
                value = ((marker->heading));
                spr->p[0].uv.v = (uint8)(51);
                spr->w = (sint16)(uint16)(4);
                spr->h = (sint16)(uint16)(4);
                spr->p[0].uv.u = (uint8)((value << 2) + 62);
            }
            else if (mode == 1)
            {
                value = ((marker->heading));
                spr->p[0].uv.v = (uint8)(11);
                spr->w = (sint16)(uint16)(6);
                spr->h = (sint16)(uint16)(6);
                spr->p[0].uv.u = (uint8)(value * 6);
            }
            else if (mode == 2)
            {
                value = ((marker->heading));
                spr->p[0].uv.v = (uint8)(17);
                spr->w = (sint16)(uint16)(8);
                spr->h = (sint16)(uint16)(8);
                spr->p[0].uv.u = (uint8)(value << 3);
            }
        }
        else if (mode == 1)
        {
            value = ((marker->height_band));
            spr->p[0].uv.u = (uint8)(98);
            spr->p[0].uv.v = (uint8)(6);
            size = value + 2;
            spr->w = (sint16)(uint16)(size);
            spr->h = (sint16)(uint16)(size);
        }
        else if (mode == 2)
        {
            spr->p[0].uv.u = (uint8)(98);
            spr->p[0].uv.v = (uint8)(6);
            spr->w = (sint16)(uint16)(1);
            spr->h = (sint16)(uint16)(1);
        }
    }
    flags = ((marker->flags));
    intensity = 131;
    if (flags & 0x10)
        intensity = ((marker->intensity));
    if (flags & 4)
    {
        if (game_runtime.mission_control.ticks & 2)
            intensity >>= 3;
    }
    color = ((marker->color));
    if (color == 0)
    {
        value = intensity >> 1;
        spr->rgbc.r = (uint8)(value);
        spr->rgbc.g = (uint8)(value);
        spr->rgbc.b = (uint8)(value);
    }
    else if (color == 1)
    {
        spr->rgbc.r = (uint8)(intensity);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(0);
    }
    else if (color == 2)
    {
        spr->rgbc.r = (uint8)(0);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(intensity);
    }
    else if (color == 3)
    {
        spr->rgbc.r = (uint8)(intensity);
        spr->rgbc.g = (uint8)(intensity);
        spr->rgbc.b = (uint8)(0);
    }
    width = (uint16)spr->w;
    x = ((marker->x));
    height = (uint16)spr->h;
    width = (sint32)(width << 16) >> 17;
    height = (sint32)(height << 16) >> 17;
    spr->p[0].xy.x = (sint16)(uint16)(x - width);
    y = ((marker->y));
    buffers = hud_control.packets.arena_begin;
    spr->p[0].xy.y = (sint16)(uint16)(y - height);
    count = hud_control.packets.spr_count;
    hud_control.packets.spr_count = count + 1;
    return;
}

static void hud_radar_draw(const HUD_RADAR_REFERENCE *reference)
{
    static const uint32 waypoint[6] = {0xFB400000, 0xFFA00000, 0xFA500000, 0, 0, 0};
    uint32 pass, index, flags, mission;
    HUD_GPU_BANK *bank;
    HUD_RADAR_MARK *record;
    FUNCTION_MARKER(0x8003DE5C, "ACE3.BIN");
    bank = hud_control.packets.current_data;
    for (pass = 0; pass < 2; ++pass)
    {
        for (index = 2; index < 64; ++index)
        {
            record = hud_radar_contact(index);
            flags = (*mission_actor_flags(index));
            if ((flags & 0x2000) && ((record->color != 0) == (pass != 0)))
                hud_radar_draw_marker(record, bank);
        }
    }
    if ((sint32)hud_control.map_scale <= 0x8000)
        for (index = 0; index < 32; ++index)
            hud_radar_draw_marker(&hud_control.radar_markers[index], bank);
    mission = (sint16)game_runtime.selection.mission;
    if (mission != 18 && mission != 37)
        return;
    if (game_runtime.mission_control.argument != 1)
        return;
    {
        (void)(hud_radar_draw_waypoint(reference, waypoint));
        return;
    }
}

void hud_radar_bind_contact(uint32 index, const MISSION_ACTOR_DEFINITION *definition)
{
    uint32 value, flags, result;
    HUD_RADAR_MARK *state;
    FUNCTION_MARKER(0x8004ECC4, "ACE3.BIN");
    state = hud_radar_contact(index);
    if (definition->highlight == 1)
        state->flags = (uint8)(state->flags | 16);
    value = definition->shape;
    if (value == 0)
        state->shape = (uint8)(0);
    else if (value == 1)
        state->shape = (uint8)(1);
    flags = (*mission_actor_flags(index));
    if ((flags & 0x100) != 0)
    {
        state->color = (uint8)(1);
        return;
    }
    flags = mission_actor_traits(((uint32)((sint32)((index << 2)) >> 2)))->flags;
    result = flags & 0x100;
    if ((flags & 0x40) != 0)
    {
        state->color = (uint8)(2);
        return;
    }
    if (result != 0)
    {
        state->color = (uint8)(3);
        return;
    }
    state->color = (uint8)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

static void hud_apply_long(const uint32 input[3], uint32 output[3])
{
    uint32 low[3], high[3], i, magnitude, value;
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (sint32)value >> 15;
            low[i] = value & 0x7FFF;
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    high[0] = xport_gte_read_data(25);
    high[1] = xport_gte_read_data(26);
    high[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    low[0] = xport_gte_read_data(25);
    low[1] = xport_gte_read_data(26);
    low[2] = xport_gte_read_data(27);
    output[0] = low[0] + (high[0] << 3);
    output[1] = low[1] + (high[1] << 3);
    output[2] = low[2] + (high[2] << 3);
}

static void hud_compose_rotation(const MATRIX *matrix)
{
    uint32 a, b, c, x, y, z, p, q, r, u, v, w;
    a = ((uint16)matrix->m[0][0]);
    b = ((uint32)(uint16)matrix->m[0][2] | ((uint32)(uint16)matrix->m[1][0] << 16));
    c = ((uint32)(uint16)matrix->m[2][0] | ((uint32)(uint16)matrix->m[2][1] << 16));
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)matrix->m[0][1]);
    b = ((uint32)(uint16)matrix->m[1][1] | ((uint32)(uint16)matrix->m[1][2] << 16));
    c = (sint16)((uint16)matrix->m[2][1]);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)matrix->m[0][2]);
    b = ((uint32)(uint16)matrix->m[1][1] | ((uint32)(uint16)matrix->m[1][2] << 16));
    c = ((uint16)matrix->m[2][2]);
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    w = xport_gte_read_data(11);
    xport_gte_write_control(0, (x & 0xFFFF) | (p << 16));
    xport_gte_write_control(1, (u & 0xFFFF) | (y << 16));
    xport_gte_write_control(2, (q & 0xFFFF) | (v << 16));
    xport_gte_write_control(3, (z & 0xFFFF) | (r << 16));
    xport_gte_write_control(4, w);
}

void hud_target_draw_relative(const HUD_CONTEXT *context)
{
    LINE_F2 *line;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    const FLIGHT_TARGET_STATE *target;
    static const sint16 diamond[8][4] = {{160, 156, 184, 180}, {184, 180, 160, 204}, {160, 204, 136, 180}, {136, 180, 160, 156}, {160, 156, 184, 180}, {184, 180, 160, 204}, {160, 204, 136, 180}, {136, 180, 160, 156}};
    static const uint8 root_after[8] = {1, 0, 2, 3, 0, 0, 0, 0};
    uint32 x, z, angle, value, root, count, base, packet, i, j, coordinates[4];
    sint32 pitch, offset;
    FUNCTION_MARKER(0x8004357C, "ACE3.BIN");
    actor_index_table = 0;
    target = context->target;
    pose_owner_object = mission_actor(actor_index_table);
    if ((flight_dynamics_flags() & 0x80) != 0)
        return;
    if ((sint32)target->distance >= 8193)
    {
        x = target->displacement.word[0];
        z = target->displacement.word[2];
        angle = game_vector_atan2((sint32)x >> 12, (sint32)(0 - z) >> 12);
        value = pose_owner_object->pose.angles[1];
        angle = (value + angle) & 0x3FFF;
        if (angle >= 0x2000)
            angle |= 0xC000;
        if ((sint16)angle > 256)
        {
            if ((sint16)angle >= 1025)
            {
                if ((game_runtime.mission_control.ticks & 127) == 0)
                    hud_notice_show(13);
            }
            hud_target_draw_arrow(0, 80, 120);
        }
        else if ((sint16)angle < -256)
        {
            if ((sint16)angle < -1024)
            {
                if ((game_runtime.mission_control.ticks & 127) == 0)
                    hud_notice_show(12);
            }
            hud_target_draw_arrow(1, 240, 120);
        }
        value = target->displacement.word[1];
        if ((sint32)value > 0)
        {
            pitch = (sint16)pose_owner_object->pose.angles[0];
            if (pitch < 0)
            {
                if (pitch < -2048)
                {
                    if ((game_runtime.mission_control.ticks & 127) == 64)
                        hud_notice_show(10);
                }
                hud_target_draw_arrow(2, 160, 60);
            }
            value = target->displacement.word[1];
        }
        if ((sint32)value < -1)
        {
            pitch = (sint16)pose_owner_object->pose.angles[0];
            if (pitch > 0)
            {
                if (pitch >= 2049)
                {
                    if ((game_runtime.mission_control.ticks & 127) == 64)
                        hud_notice_show(11);
                }
                hud_target_draw_arrow(3, 160, 180);
            }
        }
    }
    offset = ((sint32)target->displacement.word[0] >> 12) / -50;
    if (offset > 64)
        offset = 64;
    if (offset < -64)
        offset = -64;
    root = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.line_count;
    base = hud_control.target_packets.cur;
    packet = base + (count << 4) + 3072;
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(offset != 0 ? 255 : 0);
    line->rgbc.b = (uint8)(offset != 0 ? 0 : 255);
    line->p[0].x = (sint16)(uint16)(offset + 160);
    line->p[1].x = (sint16)(uint16)(offset + 160);
    root = hud_control.target_packets.arena_begin;
    line->p[0].y = (sint16)(uint16)(156);
    line->p[1].y = (sint16)(uint16)(204);
    count = hud_control.target_packets.line_count;
    hud_control.target_packets.line_count = count + 1;
    offset = ((sint32)target->displacement.word[1] >> 12) / -80;
    if (offset > 64)
        offset = 64;
    if (offset < -64)
        offset = -64;
    packet += 16;
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(offset != 0 ? 255 : 0);
    line->rgbc.b = (uint8)(offset != 0 ? 0 : 255);
    root = hud_control.target_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(136);
    line->p[0].y = (sint16)(uint16)(offset + 180);
    line->p[1].x = (sint16)(uint16)(184);
    line->p[1].y = (sint16)(uint16)(offset + 180);
    count = hud_control.target_packets.line_count;
    hud_control.target_packets.line_count = count + 1;
    offset = ((sint32)target->displacement.word[2] >> 12) / -150;
    if (offset > 24)
        offset = 24;
    if (offset < -24)
        offset = -24;
    for (i = 0; i < 8; ++i)
    {
        packet += 16;
        line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
        line->rgbc.r = (uint8)(0);
        line->rgbc.g = (uint8)(offset != 0 ? 255 : 0);
        line->rgbc.b = (uint8)(offset != 0 ? 0 : 255);
        for (j = 0; j < 4; ++j)
            coordinates[j] = diamond[i][j];
        if (i < 4)
        {
            if (i == 0)
            {
                coordinates[1] -= offset;
                coordinates[2] += offset;
            }
            else if (i == 1)
            {
                coordinates[0] += offset;
                coordinates[3] += offset;
            }
            else if (i == 2)
            {
                coordinates[1] += offset;
                coordinates[2] -= offset;
            }
            else
            {
                coordinates[0] -= offset;
                coordinates[3] -= offset;
            }
        }
        for (j = 0; j < 4; ++j)
        {
            if (j == root_after[i])
                root = hud_control.target_packets.arena_begin;
            if (j & 1)
                line->p[j >> 1].y = (sint16)(uint16)(coordinates[j]);
            else
                line->p[j >> 1].x = (sint16)(uint16)(coordinates[j]);
        }
        count = hud_control.target_packets.line_count;
        hud_control.target_packets.line_count = count + 1;
    }
    return;
}

void hud_target_draw_path(const HUD_CONTEXT *context)
{
    LINE_F2 *line;
    const sint16 *xs, *ys;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_value;
    uint32 actor_index_table;

    const FLIGHT_TARGET_STATE *target;
    MATRIX matrix;
    uint16 angles[4], vector[3];
    uint32 input[3], output[3], screen[4], a, b, value, shift, group, step, index, vertices, depth, flags, angle, distance, root, count, base, packet, result;
    sint32 pitch, dx, dy;
    FUNCTION_MARKER(0x80042D98, "ACE3.BIN");
    target = context->target;
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    a = target->reference_position.word[0];
    b = game_runtime.camera_view.pose.position[0];
    input[0] = (sint32)(a - b) >> 12;
    a = target->reference_position.word[1];
    b = game_runtime.camera_view.pose.position[1];
    input[1] = (sint32)(a - b) >> 12;
    a = target->reference_position.word[2];
    b = game_runtime.camera_view.pose.position[2];
    input[2] = (sint32)(a - b) >> 12;
    shift = (sint16)game_runtime.selection.mission == 25 ? 3 : 8;
    value = (sint16)target->reference_angles.value[1];
    input[0] += (uint32)((sint32)(uint32)rsin((sint32)value) >> shift);
    value = (sint16)target->reference_angles.value[1];
    input[2] += (uint32)((sint32)(uint32)rcos((sint32)value) >> shift);
    SetRotMatrix(&game_runtime.mission_matrices.view);
    hud_apply_long(input, output);
    matrix.t[0] = output[0];
    matrix.t[1] = output[1];
    matrix.t[2] = output[2];
    rotation_build(&matrix, target->reference_angles.value);
    SetRotMatrix(&game_runtime.mission_matrices.view);
    hud_compose_rotation(&matrix);
    SetTransMatrix(&matrix);
    for (group = 0; group < 3; ++group)
    {
        xs = group < 2 ? hud_target_triangle_x[group] : hud_target_quad_x;
        ys = group < 2 ? hud_target_triangle_y : hud_target_quad_y;
        vertices = group < 2 ? 3 : 4;
        for (step = 0; step < 16; ++step)
        {
            for (index = 0; index < vertices; ++index)
            {
                vector[0] = xs[index];
                value = (uint16)ys[index];
                if (group < 2)
                {
                    vector[1] = value - 8 * step * step;
                    value = (uint16)hud_target_triangle_z[index];
                    vector[2] = value - step * 1024;
                }
                else
                {
                    vector[2] = 0 - step * 1024;
                    vector[1] = value - 8 * step * step;
                }
                xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
                xport_gte_write_data(1, vector[2]);
                xport_gte_execute(0x180001);
                screen[index] = xport_gte_read_data(14);
                xport_gte_read_data(8);
                flags = xport_gte_read_control(31);
                depth = xport_gte_read_data(19);
                if ((sint32)flags < 0)
                    break;
            }
            if (index == vertices)
            {
                if (group < 2)
                    hud_target_emit_path3(screen[0], screen[1], screen[2], step);
                else
                    hud_target_emit_path4(screen[0], screen[1], screen[2], screen[3]);
            }
        }
    }
    a = ((uint32)target->reference_angles.value[0] | ((uint32)target->reference_angles.value[1] << 16));
    b = (uint32)target->reference_angles.value[2];
    angles[0] = a;
    angles[1] = 0x4000 - (a >> 16);
    angles[2] = b;
    angles[3] = b >> 16;
    actor_index_table = 0;
    pose_owner_value = mission_actor(actor_index_table);
    b = target->reference_position.word[0];
    a = pose_owner_value->pose.position[0];
    input[0] = (sint32)(a - b) >> 12;
    pose_owner_value = mission_actor(actor_index_table);
    b = target->reference_position.word[1];
    a = pose_owner_value->pose.position[1];
    input[1] = (sint32)(a - b) >> 12;
    pose_owner_value = mission_actor(actor_index_table);
    b = target->reference_position.word[2];
    a = pose_owner_value->pose.position[2];
    input[2] = (sint32)(a - b) >> 12;
    rotation_build(&matrix, angles);
    SetRotMatrix(&matrix);
    hud_apply_long(input, output);
    a = target->displacement.word[0];
    b = target->displacement.word[2];
    angle = game_vector_atan2((sint32)a >> 12, (sint32)(0 - b) >> 12);
    value = target->angle_delta.value[1];
    angle = ((angle << 1) + value) & 0x3FFF;
    if (angle - 513 < 0x1DFF)
    {
        if (angle >= 2049)
        {
            if ((game_runtime.mission_control.ticks & 127) == 0)
                hud_notice_show(13);
        }
        hud_target_draw_arrow(0, 80, 120);
    }
    else if (angle - 0x2000 < 0x1E00)
    {
        if (angle < 0x3800)
        {
            if ((game_runtime.mission_control.ticks & 127) == 0)
                hud_notice_show(12);
        }
        hud_target_draw_arrow(1, 240, 120);
    }
    if (target->result == 1)
    {
        if ((game_runtime.mission_control.ticks & 127) == 0)
            hud_notice_show(3);
    }
    else
    {
        a = target->displacement.word[1];
        b = target->displacement.word[2];
        value = game_vector_atan2((sint32)a >> 12, (sint32)(0 - b) >> 12);
        a = pose_owner_object->pose.angles[0];
        pitch = (sint16)(a - value);
        if (pitch > 512)
        {
            distance = target->distance;
            if ((sint32)distance >= 2049 && (sint32)distance < 16384)
            {
                value = target->displacement.word[2];
                if ((sint32)value < 0 && pitch >= 2049)
                {
                    if ((game_runtime.mission_control.ticks & 127) == 64)
                        hud_notice_show(11);
                }
                hud_target_draw_arrow(3, 160, 180);
                goto hud_crosshair_42d98;
            }
        }
        value = (context->ground_clearance);
        if ((sint32)value <= 0xFFFFF || pitch < -512)
        {
            if ((game_runtime.mission_control.ticks & 127) == 64)
                hud_notice_show(10);
            hud_target_draw_arrow(2, 160, 60);
        }
    }
hud_crosshair_42d98:
    dx = (sint32)(0 - output[0]) / 100;
    if (dx > 64)
        dx = 64;
    if (dx < -64)
        dx = -64;
    dy = (sint32)(0 - output[1] - 70) / 100;
    if (dy > 64)
        dy = 64;
    if (dy < -64)
        dy = -64;
    root = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.line_count;
    base = hud_control.target_packets.cur;
    packet = base + (count << 4) + 3072;
    for (index = 0; index < 4; ++index)
    {
        line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
        line->rgbc.r = (uint8)(0);
        line->rgbc.g = (uint8)(index < 2 ? 255 : 64);
        line->rgbc.b = (uint8)(0);
        root = hud_control.target_packets.arena_begin;
        if ((index & 1) == 0)
        {
            value = index == 0 ? dx + 160 : 160;
            line->p[1].x = (sint16)(uint16)(value);
            line->p[0].x = (sint16)(uint16)(value);
            line->p[0].y = (sint16)(uint16)(100);
            line->p[1].y = (sint16)(uint16)(140);
        }
        else
        {
            value = index == 1 ? dy + 120 : 120;
            line->p[0].x = (sint16)(uint16)(140);
            line->p[1].x = (sint16)(uint16)(180);
            line->p[1].y = (sint16)(uint16)(value);
            line->p[0].y = (sint16)(uint16)(value);
        }
        result = hud_control.target_packets.line_count + 1;
        hud_control.target_packets.line_count = result;
        packet += 16;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 hud_draw(void)
{
    const MISSION_ACTOR *hud_actor;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;
    uint32 actor_index_target;

    FLIGHT_DYNAMICS *flight;
    FLIGHT_STATUS *controller;
    FLIGHT_TARGET_STATE *output;
    HUD_CONTEXT context;
    uint16 fallback[4] = {0, 0, 0, 0};
    uint32 numerator, divisor, ratio, value, other, flags, index;
    FUNCTION_MARKER(0x80033814, "ACE3.BIN");
    actor_index_table = 0;
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    controller = &flight_status;
    numerator = (uint32)(sint16)actor_traits_object->health << 12;
    divisor = controller->max_health;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (sint32)numerator / (sint32)divisor;
    context.health = ratio;
    if ((sint32)ratio > 4096)
        context.health = 4096;
    if ((sint32)context.health < 0)
        context.health = 0;
    context.events = controller->events;
    value = controller->weapon_limit;
    context.weapon_limit = value;
    other = controller->active_weapons;
    context.active_weapons = other;
    if ((sint32)value >= 7)
        context.weapon_limit = 6;
    if ((sint32)other >= 7)
        context.active_weapons = 6;
    value = controller->ammunition;
    output = &flight_target;
    context.ammunition = value;
    value = controller->target_angle_limit;

    context.camera_steer = game_runtime.camera_follow.steer;
    context.camera_mode = &game_runtime.camera_control.mode;
    context.target = output;
    context.target_angle_limit = value;
    context.ground_height = controller->ground_height;
    context.ground_clearance = controller->ground_clearance;
    flags = game_runtime.camera_control.flags;
    index = flags & 63;
    context.delta_angles = fallback;
    if ((flags & 0x100) != 0)
    {
        actor_index_target = (actor_index_table + ((uint32)((sint32)((index << 2)) >> 2)));
        actor_motion_object = mission_actor_motion(actor_index_target);
        actor_traits_object = mission_actor_traits(actor_index_target);
        hud_actor = mission_actor(actor_index_target);
        context.position = hud_actor->pose.position;
        context.angles = hud_actor->pose.angles;

        if (index < 2)
        {
            flight = &flight_dynamics;
            value = flight->speed;
            context.speed = value;
            context.delta_angles = flight_orientation.angle_delta.value;
        }
        else
        {
            actor_motion_object = mission_actor_motion(actor_index_target);
            actor_traits_object = mission_actor_traits(actor_index_target);
            value = actor_motion_object->local_velocity[2];
            context.speed = value;
        }
    }
    else
    {
        context.position = game_runtime.camera_view.pose.position;
        context.angles = game_runtime.camera_view.pose.angles;

        context.speed = 0;
    }
    return hud_draw_context(&context);
}

uint32 hud_draw_context(const HUD_CONTEXT *context)
{
    uint32 result;
    FUNCTION_MARKER(0x8003A0B4, "ACE3.BIN");
    hud_update_alert_colors(context);
    hud_draw_mission_scores();
    hud_notice_draw_frame(context);
    hud_radar_draw_frame(context);
    hud_markers_draw();
    hud_draw_flight_instruments(context);
    hud_instruments_draw_context(context);
    result = game_runtime.mission_control.mode & 0x800;
    if (result != 0)
        return hud_target_draw(context);
    return result;
}

void hud_instruments_draw_context(const HUD_CONTEXT *context)
{
    HUD_INSTRUMENTS descriptor;
    uint32 source, a, b, value, h, timer, mode, red, green, blue, dim_red, dim_green, dim_blue, object, index, dst;
    FUNCTION_MARKER(0x8005E31C, "ACE3.BIN");
    descriptor.flags = 127;
    source = ((0));
    a = ((context->position[1]));
    b = game_runtime.world_settings.ground_offset;
    descriptor.altitude = a + b + 0x42000;
    value = ((context->speed));
    descriptor.speed = value;
    value = ((context->health));
    descriptor.health = value;
    value = ((context->ammunition));
    descriptor.gauge_value = value;
    value = ((context->weapon_limit));
    descriptor.gauge_count = value;
    h = hud_control.timer_mode;
    value = ((context->active_weapons));
    timer = hud_control.display_ticks;
    mode = hud_control.number_color;
    red = hud_control.primary_color[0];
    green = hud_control.primary_color[1];
    blue = hud_control.primary_color[2];
    dim_red = hud_control.dim_color[0];
    dim_green = hud_control.dim_color[1];
    dim_blue = hud_control.dim_color[2];
    descriptor.timer_mode = h;
    descriptor.timer_ticks = timer;
    descriptor.number_color = mode;
    descriptor.primary_color[0] = red;
    descriptor.primary_color[1] = green;
    descriptor.primary_color[2] = blue;
    descriptor.secondary_color[0] = dim_red;
    descriptor.secondary_color[1] = dim_green;
    descriptor.gauge_dim_count = value;
    descriptor.secondary_color[2] = dim_blue;
    index = game_runtime.display_buffer;
    object = game_runtime.mission_control.ot;
    dst = hud_control.instrument_banks[index];
    {
        (void)(hud_instruments_submit(object, dst, &descriptor, hud_control.instrument_bank_data[index]));
        return;
    }
}

void hud_target_init(void)
{
    HUD_TARGET_PACKET_BANK *bank;
    LINE_F3 *paths;
    LINE_F2 *lines;
    POLY_F3 *polys;
    uint32 group, index, block, table, packet, other;
    FUNCTION_MARKER(0x80042938, "ACE3.BIN");
    block = arena_alloc(20);
    hud_control.target_packets.arena_begin = block;
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(5200);
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.target[group]);
        table = hud_control.target_packets.arena_begin;
        hud_control.target_packets.cur = block;
        hud_control.target_packets.bank[group] = block;
        bank = &hud_gpu_storage.target[group];
        hud_control.target_packets.native = bank;
        hud_control.target_packets.bank_data[group] = bank;
        paths = bank->paths;
        lines = bank->lines;
        polys = bank->polys;
        for (index = 0; index < 128; ++index)
        {
            setlen(&paths[index], 5);
            paths[index].rgbc.code = (uint8)(72);
            packet = hud_control.target_packets.cur + index * 24;
            paths[index].rgbc.r = (uint8)(0);
            other = hud_control.target_packets.cur;
            paths[index].pad = (uint32)(0x55555555);
            paths[index].rgbc.g = (uint8)(255);
            paths[index].rgbc.b = (uint8)(0);
            packet = hud_control.target_packets.cur + index * 24;
            paths[index].rgbc.code = (uint8)(paths[index].rgbc.code | 2);
        }
        for (index = 0; index < 128; ++index)
        {
            setlen(&lines[index], 3);
            lines[index].rgbc.code = (uint8)(64);
            packet = hud_control.target_packets.cur + index * 16 + 0xC00;
            lines[index].rgbc.code = (uint8)(lines[index].rgbc.code | 2);
        }
        for (index = 0; index < 4; ++index)
        {
            setlen(&polys[index], 4);
            polys[index].rgbc.code = (uint8)(32);
            polys[index].rgbc.r = (uint8)(0);
            polys[index].rgbc.g = (uint8)(255);
            polys[index].rgbc.b = (uint8)(0);
            packet = hud_control.target_packets.cur + index * 20 + 0x1400;
            polys[index].rgbc.code = (uint8)(polys[index].rgbc.code | 2);
        }
    }
    return;
}

void hud_target_begin(void)
{
    uint32 state, index, result;
    FUNCTION_MARKER(0x80042BF4, "ACE3.BIN");
    state = hud_control.target_packets.arena_begin;
    hud_control.target_packets.poly_count = 0;
    hud_control.target_packets.line_count = 0;
    hud_control.target_packets.polyline_count = 0;
    index = game_runtime.display_buffer;
    result = hud_control.target_packets.bank[index];
    hud_control.target_packets.cur = result;
    hud_control.target_packets.native = hud_control.target_packets.bank_data[index];
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 hud_target_submit(void)
{
    uint32 *tag_word, *ot_word;
    uint32 state, count, ordering, packet, index, tag, head, result;
    FUNCTION_MARKER(0x80042C2C, "ACE3.BIN");
    state = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.polyline_count;
    ordering = game_runtime.mission_control.ot;
    if ((sint32)count > 0)
    {
        packet = hud_control.target_packets.cur;
        index = 0;
        do
        {
            ++index;
            tag_word = &hud_control.target_packets.native->paths[(packet - hud_control.target_packets.cur - 0) / sizeof(LINE_F3)].tag;
            ot_word = (uint32 *)mission_ot_span(ordering, sizeof(uint32));
            tag = (*tag_word);
            head = (*ot_word);
            *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.target_packets.polyline_count;
            packet += 24;
        } while ((sint32)index < (sint32)count);
    }
    state = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.line_count;
    if ((sint32)count > 0)
    {
        packet = hud_control.target_packets.cur + 0xC00;
        index = 0;
        do
        {
            ++index;
            tag_word = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)].tag;
            ot_word = (uint32 *)mission_ot_span(ordering, sizeof(uint32));
            tag = (*tag_word);
            head = (*ot_word);
            *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.target_packets.line_count;
            packet += 16;
        } while ((sint32)index < (sint32)count);
        state = hud_control.target_packets.arena_begin;
    }
    result = hud_control.target_packets.poly_count;
    if ((sint32)result > 0)
    {
        packet = hud_control.target_packets.cur + 0x1400;
        index = 0;
        do
        {
            ++index;
            tag_word = &hud_control.target_packets.native->polys[(packet - hud_control.target_packets.cur - 5120) / sizeof(POLY_F3)].tag;
            ot_word = (uint32 *)mission_ot_span(ordering, sizeof(uint32));
            tag = (*tag_word);
            head = (*ot_word);
            *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.target_packets.poly_count;
            result = (sint32)index < (sint32)count;
            packet += 20;
        } while (result != 0);
    }
    return result;
}

uint32 hud_target_draw(const HUD_CONTEXT *context)
{
    uint32 index, flags, kind;
    const MISSION_SCENARIO *descriptor;
    FUNCTION_MARKER(0x80042B20, "ACE3.BIN");
    index = mission_flow.scenario_idx;
    descriptor = &mission_scenarios[index];
    hud_target_begin();
    flags = flight_dynamics_flags() & 0x240;
    if (flags == 0)
    {
        kind = descriptor->kind;
        if (kind < 2)
            hud_target_draw_path(context);
        else if (((kind - 2) & 255) < 2)
            hud_target_draw_relative(context);
        else if (((kind - 5) & 255) < 2)
            hud_target_draw_empty();
        else if (kind == 4)
            hud_target_draw_attitude();
    }
    return hud_target_submit();
}

void hud_target_draw_empty(void)
{
    FUNCTION_MARKER(0x80043BF0, "ACE3.BIN");
}

void hud_target_draw_attitude(void)
{
    LINE_F2 *line;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 raw_pitch, value, state, count, base, packet, line_y;
    sint32 roll, pitch;
    FUNCTION_MARKER(0x80043BF8, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    roll = (sint16)pose_owner_object->pose.angles[2];
    if (roll >= 456)
    {
        hud_target_draw_arrow(2, 80, 120);
        hud_target_draw_arrow(3, 240, 120);
    }
    if (roll < -455)
    {
        hud_target_draw_arrow(3, 80, 120);
        hud_target_draw_arrow(2, 240, 120);
    }
    value = pose_owner_object->pose.angles[0];
    raw_pitch = 0x71C - value;
    pitch = (sint16)raw_pitch;
    if (pitch >= 228)
        hud_target_draw_arrow(2, 160, 60);
    if (pitch < -227)
        hud_target_draw_arrow(3, 160, 180);
    value = (uint16)game_wave_at((((0 - (uint32)pitch) & 0x3FFF) << 1));
    state = hud_control.target_packets.arena_begin;
    line_y = (sint32)(value << 16) >> 24;
    count = hud_control.target_packets.line_count;
    base = hud_control.target_packets.cur;
    packet = base + 0xC00 + (count << 4);
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    if (((raw_pitch + 227) & 0xFFFF) >= 455)
    {
        line->rgbc.r = (uint8)(128);
        line->rgbc.g = (uint8)(0);
    }
    else
    {
        line->rgbc.r = (uint8)(0);
        line->rgbc.g = (uint8)(128);
    }
    line->rgbc.b = (uint8)(0);
    line_y += 120;
    state = hud_control.target_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(140);
    line->p[0].y = (sint16)(uint16)(line_y);
    line->p[1].x = (sint16)(uint16)(180);
    line->p[1].y = (sint16)(uint16)(line_y);
    packet += 16;
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    count = hud_control.target_packets.line_count;
    hud_control.target_packets.line_count = count + 1;
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(64);
    line->rgbc.b = (uint8)(0);
    state = hud_control.target_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(140);
    line->p[0].y = (sint16)(uint16)(120);
    line->p[1].x = (sint16)(uint16)(180);
    line->p[1].y = (sint16)(uint16)(120);
    count = hud_control.target_packets.line_count;
    packet += 16;
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    hud_control.target_packets.line_count = count + 1;
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(32);
    line->rgbc.b = (uint8)(0);
    state = hud_control.target_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(140);
    line->p[0].y = (sint16)(uint16)(115);
    line->p[1].x = (sint16)(uint16)(180);
    line->p[1].y = (sint16)(uint16)(115);
    count = hud_control.target_packets.line_count;
    packet += 16;
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    hud_control.target_packets.line_count = count + 1;
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(32);
    line->rgbc.b = (uint8)(0);
    line->p[0].x = (sint16)(uint16)(140);
    line->p[0].y = (sint16)(uint16)(125);
    line->p[1].x = (sint16)(uint16)(180);
    line->p[1].y = (sint16)(uint16)(125);
    state = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.line_count + 1;
    hud_control.target_packets.line_count = count;
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_target_draw_arrow(uint32 kind, uint32 x, uint32 y)
{
    POLY_F3 *poly;
    LINE_F2 *line;
    const uint8 *offsets;
    uint32 state, count, base, packet, value, line_count;
    FUNCTION_MARKER(0x80043EFC, "ACE3.BIN");
    state = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.poly_count;
    base = hud_control.target_packets.cur;
    packet = base + 0x1400 + count * 20;
    if (kind >= 4)
        abort();
    offsets = hud_target_arrow_offsets[kind];
    poly = &hud_control.target_packets.native->polys[(packet - hud_control.target_packets.cur - 5120) / sizeof(POLY_F3)];
    poly->p[0].x = (sint16)(uint16)(x);
    poly->p[0].y = (sint16)(uint16)(y);
    value = offsets[0];
    poly->p[1].x = (sint16)(uint16)(x + (uint32)(sint32)(sint8)value);
    value = offsets[1];
    poly->p[1].y = (sint16)(uint16)(y + (uint32)(sint32)(sint8)value);
    value = offsets[2];
    poly->p[2].x = (sint16)(uint16)(x + (uint32)(sint32)(sint8)value);
    value = offsets[3];
    poly->p[2].y = (sint16)(uint16)(y + (uint32)(sint32)(sint8)value);
    count = hud_control.target_packets.poly_count;
    line_count = hud_control.target_packets.line_count;
    packet = base + 0xC00 + (line_count << 4);
    line = &hud_control.target_packets.native->lines[(packet - hud_control.target_packets.cur - 3072) / sizeof(LINE_F2)];
    hud_control.target_packets.poly_count = count + 1;
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(255);
    line->rgbc.b = (uint8)(0);
    value = offsets[4];
    line->p[0].x = (sint16)(uint16)(x + (uint32)(sint32)(sint8)value);
    value = offsets[5];
    line->p[0].y = (sint16)(uint16)(y + (uint32)(sint32)(sint8)value);
    value = offsets[6];
    line->p[1].x = (sint16)(uint16)(x + (uint32)(sint32)(sint8)value);
    value = offsets[7];
    state = hud_control.target_packets.arena_begin;
    line->p[1].y = (sint16)(uint16)(y + (uint32)(sint32)(sint8)value);
    count = hud_control.target_packets.line_count + 1;
    hud_control.target_packets.line_count = count;
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_target_emit_path3(uint32 first, uint32 second, uint32 third, uint32 phase)
{
    LINE_F3 *path;
    uint32 root, count, base, frame, packet, result;
    FUNCTION_MARKER(0x80043E0C, "ACE3.BIN");
    root = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.polyline_count;
    base = hud_control.target_packets.cur;
    frame = game_runtime.mission_control.ticks;
    packet = base + count * 24;
    path = &hud_control.target_packets.native->paths[(packet - hud_control.target_packets.cur - 0) / sizeof(LINE_F3)];
    path->rgbc.r = (uint8)(0);
    path->rgbc.b = (uint8)(0);
    path->p[0].xy = (uint32)(first);
    path->p[1].xy = (uint32)(second);
    path->p[2].xy = (uint32)(third);
    path->rgbc.g = (uint8)(((frame + phase) & 15) << 4);
    root = hud_control.target_packets.arena_begin;
    result = hud_control.target_packets.polyline_count + 1;
    hud_control.target_packets.polyline_count = result;
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_target_emit_path4(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    LINE_F3 *path;
    uint32 root, count, base, packet, result;
    FUNCTION_MARKER(0x80043E74, "ACE3.BIN");
    root = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.polyline_count;
    base = hud_control.target_packets.cur;
    packet = base + count * 24;
    path = &hud_control.target_packets.native->paths[(packet - hud_control.target_packets.cur - 0) / sizeof(LINE_F3)];
    path->rgbc.r = (uint8)(0);
    path->rgbc.g = (uint8)(255);
    path->rgbc.b = (uint8)(0);
    path->p[0].xy = (uint32)(first);
    path->p[1].xy = (uint32)(second);
    path->p[2].xy = (uint32)(fourth);
    root = hud_control.target_packets.arena_begin;
    count = hud_control.target_packets.polyline_count;
    packet += 24;
    path = &hud_control.target_packets.native->paths[(packet - hud_control.target_packets.cur - 0) / sizeof(LINE_F3)];
    hud_control.target_packets.polyline_count = count + 1;
    path->rgbc.r = (uint8)(0);
    path->rgbc.g = (uint8)(255);
    path->rgbc.b = (uint8)(0);
    path->p[0].xy = (uint32)(first);
    path->p[1].xy = (uint32)(third);
    path->p[2].xy = (uint32)(fourth);
    root = hud_control.target_packets.arena_begin;
    result = hud_control.target_packets.polyline_count + 1;
    hud_control.target_packets.polyline_count = result;
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_notice_init(void)
{
    HUD_NOTICE_PACKET_BANK *bank;
    SPRT *notice_spr, *panel_spr, *panel;
    TILE *panel_tile, *portrait_tile;
    POLY_FT4 *portraits;
    DR_TPAGE *panel_mode0, *panel_mode1, *portrait_mode0, *portrait_mode1;
    uint32 group, index, block, table, packet, first, second;
    FUNCTION_MARKER(0x80041120, "ACE3.BIN");
    block = arena_alloc(16);
    hud_control.notice_packets.arena_begin = block;
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(632);
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.notice[group]);
        table = hud_control.notice_packets.arena_begin;
        hud_control.notice_packets.cur = block;
        hud_control.notice_packets.bank[group] = block;
        bank = &hud_gpu_storage.notice[group];
        hud_control.notice_packets.native = bank;
        hud_control.notice_packets.bank_data[group] = bank;
        for (index = 0; index < 16; ++index)
        {
            notice_spr = bank->notice + index;
            setlen(notice_spr, 4);
            notice_spr->rgbc.code = (uint8)(100);
            packet = hud_control.notice_packets.cur + index * 20 + 104;
            notice_spr->rgbc.code = (uint8)(notice_spr->rgbc.code | 2);
        }
        for (index = 0; index < 3; ++index)
        {
            panel_spr = bank->prefix.panel_init + index;
            packet = hud_control.notice_packets.cur + index * 20;
            panel_spr->rgbc.code = (uint8)(100);
            panel_spr->rgbc.code = (uint8)(102);
            setlen(panel_spr, 4);
            panel_spr->rgbc.r = (uint8)(0);
            panel_spr->rgbc.g = (uint8)(131);
            panel_spr->rgbc.b = (uint8)(0);
            panel_spr->p[0].attr = (uint16)(0x7F98);
        }
        panel = bank->prefix.panel.sprites;
        first = hud_control.notice_packets.cur;
        panel[0].p[0].uv.u = (uint8)(170);
        panel[0].p[0].uv.v = (uint8)(0);
        second = hud_control.notice_packets.cur;
        panel[0].w = (sint16)(uint16)(49);
        panel[0].h = (sint16)(uint16)(15);
        packet = second + 20;
        panel[1].p[0].uv.u = (uint8)(204);
        panel[1].p[0].uv.v = (uint8)(16);
        panel[1].w = (sint16)(uint16)(14);
        panel[1].h = (sint16)(uint16)(40);
        for (index = 0; index < 3; ++index)
        {
            panel_tile = bank->prefix.panel.tiles + index;
            packet = hud_control.notice_packets.cur + 40 + index * 16;
            setlen(panel_tile, 3);
            panel_tile->rgbc.code = (uint8)(98);
        }
        panel_mode0 = bank->prefix.panel.modes;
        panel_mode1 = bank->prefix.panel.modes + 1;
        setlen(panel_mode0, 1);
        first = hud_control.notice_packets.cur;
        setlen(panel_mode1, 1);
        second = hud_control.notice_packets.cur;
        panel_mode0->code[0] = (uint32)(0xE1000636);
        packet = second + 0x1EF;
        portraits = bank->portrait;
        panel_mode1->code[0] = (uint32)(0xE1000616);
        for (index = 0; index < 2; ++index)
        {
            portraits[index].rgbc.code = (uint8)(44);
            setlen(&portraits[index], 9);
            portraits[index].rgbc.r = (uint8)(131);
            portraits[index].rgbc.g = (uint8)(131);
            portraits[index].rgbc.b = (uint8)(131);
            portraits[index].rgbc.code = (uint8)(46);
            packet += 40;
        }
        block = hud_control.notice_packets.cur;
        portraits[0].p[1].attr = (uint16)(37);
        packet = block + 528;
        portraits[1].p[1].attr = (uint16)(54);
        portraits[1].p[0].uv.v = (uint8)(87);
        portraits[1].p[1].uv.v = (uint8)(87);
        portraits[1].p[0].uv.u = (uint8)(80);
        portraits[1].p[1].uv.u = (uint8)(123);
        portraits[1].p[2].uv.u = (uint8)(80);
        portraits[1].p[2].uv.v = (uint8)(128);
        portraits[1].p[3].uv.u = (uint8)(123);
        portraits[1].p[3].uv.v = (uint8)(128);
        block = hud_control.notice_packets.cur;
        portraits[1].p[0].attr = (uint16)(0x7F98);
        for (index = 0; index < 3; ++index)
        {
            portrait_tile = bank->portrait_tiles + index;
            packet = block + 575 + index * 16;
            portrait_tile->rgbc.code = (uint8)(96);
            setlen(portrait_tile, 3);
            portrait_tile->rgbc.r = (uint8)(0);
            portrait_tile->rgbc.g = (uint8)(0);
            portrait_tile->rgbc.b = (uint8)(0);
            portrait_tile->rgbc.code = (uint8)(98);
        }
        portrait_mode0 = bank->portrait_modes;
        portrait_mode1 = bank->portrait_modes + 1;
        setlen(portrait_mode0, 1);
        first = hud_control.notice_packets.cur;
        setlen(portrait_mode1, 1);
        second = hud_control.notice_packets.cur;
        portrait_mode0->code[0] = (uint32)(0xE1000616);
        portrait_mode1->code[0] = (uint32)(0xE1000616);
    }
    hud_panel_reset();
    hud_portrait_reset();
    hud_notice_reset_pair();
    hud_notice_reset();
}

void hud_notice_begin(void)
{
    uint32 buffers, value;
    FUNCTION_MARKER(0x800414A4, "ACE3.BIN");
    buffers = hud_control.notice_packets.arena_begin;
    hud_control.notice_packets.mode_count = 0;
    hud_control.notice_packets.spr_count = 0;
    value = game_runtime.display_buffer;
    hud_control.notice_packets.native = hud_control.notice_packets.bank_data[value];
    value = hud_control.notice_packets.bank[value];
    hud_control.notice_packets.cur = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_notice_draw_frame(const HUD_CONTEXT *context)
{
    uint32 value;
    FUNCTION_MARKER(0x800413FC, "ACE3.BIN");
    hud_notice_begin();
    value = game_runtime.mission_control.mode;
    if (value & 4)
    {
        if ((game_runtime.flags & 0x20000000) == 0)
            hud_notice_update_warnings(context);
    }
    value = game_runtime.mission_control.mode;
    if (value & 0x2C00)
        hud_draw_single_notice();
    value = (sint16)hud_control.panel.width;
    hud_panel_draw(value);
    hud_portrait_draw();
    {
        (void)(hud_notice_submit_panel());
        return;
    }
}

void hud_notice_submit_panel(void)
{
    uint32 *tag_word, *node_word, *ot_word;
    static const uint32 offsets[6] = {20, 40, 72, 88, 56, 96};
    uint32 packet, ot, value, head, merged, low, result, i, node;
    FUNCTION_MARKER(0x800414D8, "ACE3.BIN");
    packet = hud_control.notice_packets.cur;
    ot = game_runtime.mission_control.ot;
    tag_word = (uint32 *)((uint8 *)hud_control.notice_packets.native + (packet - hud_control.notice_packets.cur));
    ot_word = (uint32 *)mission_ot_span(ot, sizeof(uint32));
    value = (*tag_word);
    head = (*ot_word);
    *tag_word = ((value & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    merged = (head & 0xFF000000) | (packet & 0xFFFFFF);
    *ot_word = (merged);
    result = hud_control.panel.mode;
    low = merged & 0xFFFFFF;
    if (result == 0)
        return;
    for (i = 0; i < 6; ++i)
    {
        node = packet + offsets[i];
        node_word = (uint32 *)((uint8 *)hud_control.notice_packets.native + (node - hud_control.notice_packets.cur));
        value = (*node_word);
        *node_word = ((value & 0xFF000000) | low);
        head = (*ot_word);
        result = node & 0xFFFFFF;
        merged = (head & 0xFF000000) | result;
        *ot_word = (merged);
        low = merged & 0xFFFFFF;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_panel_draw(uint32 size)
{
    SPRT *spr;
    TILE *tile;
    uint32 packet, value, blue, x, width, base;
    FUNCTION_MARKER(0x80041794, "ACE3.BIN");
    packet = hud_control.notice_packets.cur;
    spr = hud_control.notice_packets.native->prefix.panel.sprites + 0;
    value = hud_control.dim_color[0];
    spr->rgbc.r = (uint8)(value);
    value = hud_control.dim_color[1];
    spr->rgbc.g = (uint8)(value);
    blue = hud_control.dim_color[2];
    spr->p[0].xy.x = (sint16)(uint16)(271 - size);
    spr->p[0].xy.y = (sint16)(uint16)(14);
    x = 320 - size;
    spr->rgbc.b = (uint8)(blue);
    spr->p[0].uv.u = (uint8)(size ? 120 : 170);
    packet = hud_control.notice_packets.cur + 20;
    spr = hud_control.notice_packets.native->prefix.panel.sprites + 1;
    value = hud_control.dim_color[0];
    spr->rgbc.r = (uint8)(value);
    value = hud_control.dim_color[1];
    spr->rgbc.g = (uint8)(value);
    blue = hud_control.dim_color[2];
    spr->p[0].xy.x = (sint16)(uint16)(x);
    spr->p[0].xy.y = (sint16)(uint16)(14);
    spr->rgbc.b = (uint8)(blue);
    width = (sint32)size < 15 ? 0 : size - 14;
    x = 334 - size;
    packet = hud_control.notice_packets.cur + 40;
    tile = hud_control.notice_packets.native->prefix.panel.tiles + 0;
    tile->p[0].x = (sint16)(uint16)(x);
    tile->p[0].y = (sint16)(uint16)(14);
    tile->w = (sint16)(uint16)(width);
    tile->h = (sint16)(uint16)(3);
    value = hud_control.primary_color[0];
    tile->rgbc.r = (uint8)(value);
    value = hud_control.primary_color[1];
    tile->rgbc.g = (uint8)(value);
    value = hud_control.primary_color[2];
    tile->rgbc.b = (uint8)(value);
    packet = hud_control.notice_packets.cur + 56;
    tile = hud_control.notice_packets.native->prefix.panel.tiles + 1;
    tile->rgbc.r = (uint8)(0);
    tile->rgbc.g = (uint8)(0);
    tile->rgbc.b = (uint8)(0);
    base = hud_control.notice_packets.cur;
    tile->p[0].x = (sint16)(uint16)(329 - size);
    tile->p[0].y = (sint16)(uint16)(17);
    tile->w = (sint16)(uint16)(width + 5);
    tile->h = (sint16)(uint16)(34);
    packet = base + 72;
    tile = hud_control.notice_packets.native->prefix.panel.tiles + 2;
    tile->p[0].x = (sint16)(uint16)(x);
    tile->p[0].y = (sint16)(uint16)(51);
    tile->w = (sint16)(uint16)(width);
    tile->h = (sint16)(uint16)(3);
    value = hud_control.primary_color[0];
    tile->rgbc.r = (uint8)(value);
    value = hud_control.primary_color[1];
    tile->rgbc.g = (uint8)(value);
    value = hud_control.primary_color[2];
    tile->rgbc.b = (uint8)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_portrait_draw(void)
{
    POLY_FT4 *quad;
    TILE *tile;
    uint32 *node_word, *ot_word;
    static const uint32 offsets[7] = {0x210, 0x1E8, 0x248, 0x258, 0x268, 0x238, 0x270};
    uint32 width, height, value, x, y, inner_x, inner_y, right, bottom, packet, base;
    uint32 blue, texture, divisor, numerator, remainder, i, ot, tag, head, node;
    FUNCTION_MARKER(0x80041ADC, "ACE3.BIN");
    if (hud_control.portrait.mode == 0)
        return;
    width = hud_control.portrait.w;
    height = hud_control.portrait.h;
    value = hud_control.portrait.w;
    width -= 11;
    height -= 5;
    value = 43 - value;
    x = hud_control.portrait.x;
    x += (sint32)value / 2;
    value = hud_control.portrait.h;
    inner_x = x + 6;
    value = 41 - value;
    y = hud_control.portrait.y;
    y += (sint32)value / 2;
    inner_y = y + 3;
    if ((sint32)width <= 0)
        width = 1;
    if ((sint32)height <= 0)
        height = 1;
    packet = hud_control.notice_packets.cur + 0x210;
    quad = hud_control.notice_packets.native->portrait + 1;
    value = hud_control.dim_color[0];
    quad->rgbc.r = (uint8)(value);
    value = hud_control.dim_color[1];
    quad->rgbc.g = (uint8)(value);
    blue = hud_control.dim_color[2];
    quad->p[0].xy.x = (sint16)(uint16)(x);
    quad->p[0].xy.y = (sint16)(uint16)(y);
    quad->rgbc.b = (uint8)(blue);
    value = hud_control.portrait.w;
    quad->p[1].xy.y = (sint16)(uint16)(y);
    quad->p[2].xy.x = (sint16)(uint16)(x);
    quad->p[1].xy.x = (sint16)(uint16)(x + value);
    value = hud_control.portrait.h;
    quad->p[2].xy.y = (sint16)(uint16)(y + value);
    value = hud_control.portrait.w;
    quad->p[3].xy.x = (sint16)(uint16)(x + value);
    value = hud_control.portrait.h;
    right = width + x + 5;
    quad->p[3].xy.y = (sint16)(uint16)(y + value);
    packet = hud_control.notice_packets.cur + 0x1E8;
    quad = hud_control.notice_packets.native->portrait + 0;
    texture = hud_control.portrait.image;
    bottom = height + y + 2;
    texture = (texture << 6) + 0x3216;
    quad->p[0].xy.x = (sint16)(uint16)(inner_x);
    quad->p[0].xy.y = (sint16)(uint16)(inner_y);
    quad->p[1].xy.x = (sint16)(uint16)(right);
    quad->p[1].xy.y = (sint16)(uint16)(inner_y);
    quad->p[2].xy.x = (sint16)(uint16)(inner_x);
    quad->p[2].xy.y = (sint16)(uint16)(bottom);
    quad->p[3].xy.x = (sint16)(uint16)(right);
    quad->p[3].xy.y = (sint16)(uint16)(bottom);
    quad->p[0].attr = (uint16)(texture);
    value = hud_control.portrait.image;
    quad->p[0].uv.u = (uint8)((value & 7) << 5);
    value = hud_control.portrait.image;
    quad->p[0].uv.v = (uint8)((value >> 3) * 36 - 128);
    value = hud_control.portrait.image;
    quad->p[1].uv.u = (uint8)(((value & 7) << 5) + 31);
    value = hud_control.portrait.image;
    quad->p[1].uv.v = (uint8)((value >> 3) * 36 - 128);
    value = hud_control.portrait.image;
    quad->p[2].uv.u = (uint8)((value & 7) << 5);
    value = hud_control.portrait.image;
    quad->p[2].uv.v = (uint8)((value >> 3) * 36 - 93);
    value = hud_control.portrait.image;
    width -= 2;
    quad->p[3].uv.u = (uint8)(((value & 7) << 5) + 31);
    value = hud_control.portrait.image;
    quad->p[3].uv.v = (uint8)((value >> 3) * 36 - 93);
    if ((sint32)(width << 16) <= 0)
        width = 1;
    height -= 3;
    if ((sint32)(height << 16) <= 0)
        height = 1;
    x += 7;
    value = y + 4;
    divisor = (sint16)height;
    base = hud_control.notice_packets.cur;
    packet = base + 0x238;
    tile = hud_control.notice_packets.native->portrait_tiles + 0;
    tile->p[0].x = (sint16)(uint16)(x);
    tile->p[0].y = (sint16)(uint16)(value);
    tile->w = (sint16)(uint16)(width);
    tile->h = (sint16)(uint16)(height);
    packet = base + 0x248;
    tile = hud_control.notice_packets.native->portrait_tiles + 1;
    tile->p[0].x = (sint16)(uint16)(x);
    numerator = game_runtime.mission_control.ticks << 2;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    remainder = (sint32)numerator % (sint32)divisor;
    tile->w = (sint16)(uint16)(width);
    tile->h = (sint16)(uint16)(1);
    tile->p[0].y = (sint16)(uint16)(inner_y + remainder);
    packet = base + 0x258;
    tile = hud_control.notice_packets.native->portrait_tiles + 2;
    tile->p[0].x = (sint16)(uint16)(x);
    value = game_runtime.mission_control.ticks;
    numerator = (value << 2) + value;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    remainder = (sint32)numerator % (sint32)divisor;
    tile->h = (sint16)(uint16)(1);
    tile->w = (sint16)(uint16)(width);
    tile->p[0].y = (sint16)(uint16)(inner_y + remainder);
    for (i = 0; i < 7; ++i)
    {
        node = base + offsets[i];
        node_word = (uint32 *)((uint8 *)hud_control.notice_packets.native + (node - hud_control.notice_packets.cur));
        ot = game_runtime.mission_control.ot;
        ot_word = (uint32 *)mission_ot_span(ot, sizeof(uint32));
        tag = (*node_word);
        head = (*ot_word);
        *node_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
        ot = game_runtime.mission_control.ot;
        ot_word = (uint32 *)mission_ot_span(ot, sizeof(uint32));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (node & 0xFFFFFF));
    }
    return;
}

void hud_panel_reset(void)
{
    FUNCTION_MARKER(0x80041640, "ACE3.BIN");
    hud_control.panel.mode = (uint8)(0);
    hud_control.panel.reset_word = (uint32)(0);
    hud_control.panel.width = (uint16)(0);
}

void hud_portrait_reset(void)
{
    FUNCTION_MARKER(0x80041900, "ACE3.BIN");
    hud_control.portrait.image = (uint8)(255);
    hud_control.portrait.reset_byte = (uint8)(255);
    hud_control.portrait.mode = (uint8)(0);
    hud_control.portrait.ticks = (uint16)(0);
    return;
}

void hud_panel_update(void)
{
    uint32 mode, value;
    FUNCTION_MARKER(0x80041698, "ACE3.BIN");
    if (game_runtime.flags & 0x20000000)
        return;
    mode = hud_control.panel.mode;
    if (mode == 1)
    {
        if ((sint16)hud_control.panel.width == 0)
            sound_play(62);
        value = hud_control.panel.width + 16;
        hud_control.panel.width = (uint16)(value);
        if ((sint16)value >= 200)
        {
            hud_control.panel.mode = (uint8)(2);
            hud_control.panel.width = (uint16)(200);
            hud_control.panel.reset_word = (uint32)(0);
        }
    }
    else if (mode == 3)
    {
        if ((sint16)hud_control.panel.width == 200)
            sound_play(54);
        value = hud_control.panel.width - 32;
        hud_control.panel.width = (uint16)(value);
        if ((sint32)(value << 16) <= 0)
        {
            hud_control.panel.width = (uint16)(0);
            hud_control.panel.mode = (uint8)(0);
        }
    }
    return;
}

void hud_panel_close(void)
{
    FUNCTION_MARKER(0x8004167C, "ACE3.BIN");
    if (hud_control.panel.mode == 2)
        hud_control.panel.mode = (uint8)(3);
    return;
}

void hud_panel_open(void)
{
    FUNCTION_MARKER(0x80041654, "ACE3.BIN");
    if (hud_control.panel.mode != 2)
        hud_control.panel.mode = (uint8)(1);
    return;
}

uint32 hud_panel_get_mode(void)
{
    FUNCTION_MARKER(0x80041670, "ACE3.BIN");
    return hud_control.panel.mode;
}

void hud_portrait_update(void)
{
    uint32 mode, counter, value;
    FUNCTION_MARKER(0x800419C0, "ACE3.BIN");
    if (game_runtime.flags & 0x20000000)
        return;
    mode = hud_control.portrait.mode;
    if (mode == 1)
    {
        counter = (sint16)hud_control.portrait.ticks;
        if (counter == 0)
        {
            hud_control.portrait.w = (uint8)(43);
            hud_control.portrait.h = (uint8)(6);
        }
        else if (counter == 1)
        {
            hud_control.portrait.w = (uint8)(12);
            hud_control.portrait.h = (uint8)(41);
        }
        else if (counter == 2)
        {
            hud_control.portrait.w = (uint8)(21);
            hud_control.portrait.h = (uint8)(20);
        }
        else if (counter == 3)
        {
            hud_control.portrait.w = (uint8)(43);
            hud_control.portrait.h = (uint8)(41);
            hud_control.portrait.mode = (uint8)(2);
        }
        value = hud_control.portrait.ticks;
        hud_control.portrait.ticks = (uint16)(value + 1);
    }
    else if (mode == 3)
    {
        value = hud_control.portrait.h;
        hud_control.portrait.w = (uint8)(43);
        value >>= 2;
        hud_control.portrait.h = (uint8)(value);
        if (value == 0)
            hud_control.portrait.mode = (uint8)(0);
    }
    return;
}

void hud_portrait_hide(uint32 mode)
{
    uint32 value;
    FUNCTION_MARKER(0x8004197C, "ACE3.BIN");
    value = hud_control.portrait.mode;
    hud_control.portrait.ticks = (uint16)(0);
    if (value == 0)
        return;
    hud_control.portrait.mode = (uint8)(mode == 1 ? 3 : 0);
    hud_control.portrait.image = (uint8)(255);
    return;
}

void hud_portrait_show(uint32 speaker, uint32 x, uint32 y)
{
    uint32 result, id;
    FUNCTION_MARKER(0x80041920, "ACE3.BIN");
    if (speaker == 0)
    {
        (void)(hud_portrait_hide(1));
        return;
    }
    result = hud_control.portrait.image;
    id = speaker - 1;
    if (result != id)
    {
        result = 1;
        hud_control.portrait.mode = (uint8)(result);
        hud_control.portrait.ticks = (uint16)(0);
        hud_control.portrait.x = (uint16)(x);
        hud_control.portrait.y = (uint16)(y);
        hud_control.portrait.image = (uint8)(id);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_radar_draw_frame(const HUD_CONTEXT *context)
{
    HUD_RADAR_REFERENCE *reference = (HUD_RADAR_REFERENCE *)getScratchAddr(64);
    uint32 result;
    FUNCTION_MARKER(0x8003C5B0, "ACE3.BIN");
    hud_radar_begin();
    hud_radar_set_reference(reference, context);
    hud_radar_update_distances(reference);
    hud_radar_update_scale();
    hud_radar_draw_scale();
    hud_radar_draw_map(reference);
    hud_radar_update(reference);
    hud_radar_draw(reference);
    hud_radar_submit();
    result = game_runtime.account.hold_action_gate;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = game_input.held & 0x8000;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    hud_map_draw();
    {
        (void)(hud_map_submit());
        return;
    }
}

void hud_radar_begin(void)
{
    uint32 buffers, value;
    FUNCTION_MARKER(0x8003C650, "ACE3.BIN");
    buffers = hud_control.packets.arena_begin;
    hud_control.packets.polyline_count = 0;
    hud_control.packets.line_count = 0;
    hud_control.packets.tile_count = 0;
    hud_control.packets.spr_count = 0;
    hud_control.packets.flags = 0;
    value = game_runtime.display_buffer;
    hud_control.packets.current_data = hud_control.packets.bank_data[value];
    value = hud_control.packets.bank[value];
    hud_control.packets.cur = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_radar_submit(void)
{
    HUD_GPU_BANK *bank;
    uint32 *ot_word;
    uint32 state, flags, ordering, base, packet, tag, head, count, index;
    FUNCTION_MARKER(0x8003C690, "ACE3.BIN");
    state = hud_control.packets.arena_begin;
    flags = hud_control.packets.flags;
    ordering = game_runtime.mission_control.ot;
    ot_word = (uint32 *)mission_ot_span(ordering, sizeof(*ot_word));
    bank = hud_control.packets.current_data;
    if (flags & 2)
    {
        base = hud_control.packets.cur;
        packet = base + 0xC74;
        for (index = 0; index < 3; ++index, packet += 20)
        {
            tag = bank->triangles[index].tag;
            head = (*ot_word);
            bank->triangles[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    state = hud_control.packets.arena_begin;
    count = hud_control.packets.line_count;
    if ((sint32)count > 0)
    {
        base = hud_control.packets.cur;
        packet = base + 0x918;
        index = 0;
        do
        {
            ++index;
            tag = bank->lines[index - 1].tag;
            head = (*ot_word);
            bank->lines[index - 1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.packets.line_count;
            packet += 16;
        } while ((sint32)index < (sint32)count);
    }
    base = hud_control.packets.cur;
    head = (*ot_word);
    tag = bank->map.sprites[0].tag;
    bank->map.sprites[0].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xC0C) & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->map.sprites[1].tag;
    bank->map.sprites[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xC20) & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->areas[1].tag;
    bank->areas[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 12) & 0xFFFFFF);
    *ot_word = (head);
    bank->map.masks[4].tag = (uint32)(head | 0x2000000);
    *ot_word = ((base + 0xA88) & 0xFFFFFF);
    state = hud_control.packets.arena_begin;
    flags = hud_control.packets.flags;
    if (flags & 1)
    {
        base = hud_control.packets.cur;
        packet = base + 0xC54;
        for (index = 0; index < 2; ++index, packet += 16)
        {
            tag = bank->clip_lines[index].tag;
            head = (*ot_word);
            bank->clip_lines[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    state = hud_control.packets.arena_begin;
    base = hud_control.packets.cur;
    count = hud_control.packets.polyline_count;
    packet = base + 0x998;
    if ((sint32)count > 0)
    {
        index = 0;
        do
        {
            ++index;
            tag = bank->polylines[index - 1].tag;
            head = (*ot_word);
            bank->polylines[index - 1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.packets.polyline_count;
            packet += 24;
        } while ((sint32)index < (sint32)count);
    }
    state = hud_control.packets.arena_begin;
    count = hud_control.packets.spr_count;
    if ((sint32)count > 0)
    {
        base = hud_control.packets.cur;
        packet = base + 24;
        index = 0;
        do
        {
            ++index;
            tag = bank->submitted_sprites[index - 1].tag;
            head = (*ot_word);
            bank->submitted_sprites[index - 1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.packets.spr_count;
            packet += 20;
        } while ((sint32)index < (sint32)count);
    }
    head = (*ot_word);
    base = hud_control.packets.cur;
    bank->map.masks[3].tag = (uint32)(head | 0x2000000);
    head = (base + 0xA7C) & 0xFFFFFF;
    *ot_word = (head);
    base = hud_control.packets.cur;
    tag = bank->map.quads[7].tag;
    bank->map.quads[7].tag = (uint32)((tag & 0xFF000000) | head);
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xB6C) & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->map.tiles[1].tag;
    bank->map.tiles[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xC44) & 0xFFFFFF);
    *ot_word = (head);
    bank->map.masks[2].tag = (uint32)(head | 0x2000000);
    head = (base + 0xA70) & 0xFFFFFF;
    *ot_word = (head);
    base = hud_control.packets.cur;
    tag = bank->map.modes[0].tag;
    bank->map.modes[0].tag = (uint32)((tag & 0xFF000000) | head);
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xA94) & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->map.base_quad.tag;
    bank->map.base_quad.tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xAA4) & 0xFFFFFF);
    *ot_word = (head);
    bank->map.masks[1].tag = (uint32)(head | 0x2000000);
    head = (base + 0xA64) & 0xFFFFFF;
    *ot_word = (head);
    base = hud_control.packets.cur;
    tag = bank->map.quads[3].tag;
    bank->map.quads[3].tag = (uint32)((tag & 0xFF000000) | head);
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xACC) & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->map.tiles[0].tag;
    bank->map.tiles[0].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xC34) & 0xFFFFFF);
    *ot_word = (head);
    bank->map.masks[0].tag = (uint32)(head | 0x2000000);
    head = (base + 0xA58) & 0xFFFFFF;
    *ot_word = (head);
    base = hud_control.packets.cur;
    tag = bank->areas[0].tag;
    bank->areas[0].tag = (uint32)((tag & 0xFF000000) | head);
    head = (*ot_word);
    head = (head & 0xFF000000) | (base & 0xFFFFFF);
    *ot_word = (head);
    tag = bank->map.modes[1].tag;
    bank->map.modes[1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    head = (head & 0xFF000000) | ((base + 0xA9C) & 0xFFFFFF);
    *ot_word = (head);
    {
        (void)(uint32)(head);
        return;
    }
}

static void hud_radar_set_reference(HUD_RADAR_REFERENCE *reference, const HUD_CONTEXT *context)
{
    uint32 value;
    FUNCTION_MARKER(0x8003D08C, "ACE3.BIN");
    value = (((sint32)context->position[0]));
    reference->x = (sint32)value >> 12;
    value = (((sint32)context->position[2]));
    reference->z = (sint32)value >> 12;
    value = ((context->angles[1])) & 0x3FFF;
    value = (uint16)game_wave_at((value << 1));
    reference->sine = (sint32)(value << 16) >> 18;
    value = (((context->angles[1])) & 0x3FFF) + 0x1000;
    value = (uint16)game_wave_at((value << 1));
    reference->cosine = (sint32)(value << 16) >> 18;
    value = (((sint16)context->angles[1]));
    reference->heading_word = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_radar_draw_scale(void)
{
    HUD_GPU_BANK *bank;
    LINE_F2 *line;
    SPRT *spr;
    uint32 buffers, packet, count, value, blue, scale;
    FUNCTION_MARKER(0x8003D458, "ACE3.BIN");
    buffers = hud_control.packets.arena_begin;
    bank = hud_control.packets.current_data;
    packet = hud_control.packets.cur;
    count = hud_control.packets.line_count;
    packet += (count << 4) + 0x918;
    line = &bank->lines[(packet - hud_control.packets.cur - 0x918) / sizeof(LINE_F2)];
    value = hud_control.primary_color[0];
    line->rgbc.r = (uint8)(value >> 1);
    value = hud_control.primary_color[1];
    line->rgbc.g = (uint8)(value >> 1);
    blue = hud_control.primary_color[2];
    line->p[0].x = (sint16)(uint16)(0);
    line->p[0].y = (sint16)(uint16)(45);
    line->p[1].y = (sint16)(uint16)(45);
    line->rgbc.b = (uint8)(blue >> 1);
    buffers = hud_control.packets.arena_begin;
    line->p[1].x = (sint16)(uint16)(5);
    count = hud_control.packets.line_count;
    hud_control.packets.line_count = count + 1;
    value = hud_control.primary_color[0];
    packet += 16;
    line = &bank->lines[(packet - hud_control.packets.cur - 0x918) / sizeof(LINE_F2)];
    line->rgbc.r = (uint8)(value >> 1);
    value = hud_control.primary_color[1];
    line->rgbc.g = (uint8)(value >> 1);
    blue = hud_control.primary_color[2];
    line->p[0].x = (sint16)(uint16)(86);
    line->rgbc.b = (uint8)(blue >> 1);
    buffers = hud_control.packets.arena_begin;
    line->p[0].y = (sint16)(uint16)(14);
    line->p[1].x = (sint16)(uint16)(320);
    line->p[1].y = (sint16)(uint16)(14);
    count = hud_control.packets.line_count;
    hud_control.packets.line_count = count + 1;
    packet = hud_control.packets.cur;
    value = hud_control.dim_color[0];
    packet += 0xC0C;
    spr = &bank->map.sprites[0];
    spr->rgbc.r = (uint8)(value);
    value = hud_control.dim_color[1];
    spr->rgbc.g = (uint8)(value);
    value = hud_control.dim_color[2];
    spr->rgbc.b = (uint8)(value);
    packet = hud_control.packets.cur;
    value = hud_control.dim_color[0];
    packet += 0xC20;
    spr = &bank->map.sprites[1];
    spr->rgbc.r = (uint8)(value >> 1);
    value = hud_control.dim_color[1];
    spr->rgbc.g = (uint8)(value >> 1);
    value = hud_control.dim_color[2];
    spr->rgbc.b = (uint8)(value >> 1);
    scale = hud_control.map_scale;
    if ((sint32)scale <= 0x4000)
    {
        spr->p[0].uv.u = (uint8)(24);
        return;
    }
    if ((sint32)scale <= 0x8000)
    {
        spr->p[0].uv.u = (uint8)(12);
        return;
    }
    spr->p[0].uv.u = (uint8)(0);
    return;
}

static void hud_radar_draw_map(const HUD_RADAR_REFERENCE *reference)
{
    POLY_FT4 *quad;
    uint32 radius, center_x, center_y, left, right, top, bottom, cosine, sine, dx0, dx1, dy0, dy1;
    uint32 px0, px1, py0, py1, qx0, qx1, qy0, qy1, numerators[8], projected[8], index, packet, value;
    FUNCTION_MARKER(0x8003D5C0, "ACE3.BIN");
    radius = hud_control.map_scale;
    center_x = reference->x;
    center_y = reference->z;
    left = (center_x - radius) & 0xFFFFF000;
    right = ((center_x + radius) & 0xFFFFF000) + 0x1000;
    top = ((center_y + radius) & 0xFFFFF000) + 0x1000;
    bottom = (center_y - radius) & 0xFFFFF000;
    if ((sint32)left > 0x3FFFF)
        left = 0x3FFFF;
    if ((sint32)left < -262144)
        left = 0xFFFC0000;
    if ((sint32)right > 0x3FFFF)
        right = 0x3FFFF;
    if ((sint32)right < -262144)
        right = 0xFFFC0000;
    if ((sint32)top > 0x40000)
        top = 0x40000;
    if ((sint32)top <= -262144)
        top = 0xFFFC0001;
    if ((sint32)bottom > 0x40000)
        bottom = 0x40000;
    if ((sint32)bottom <= -262144)
        bottom = 0xFFFC0001;
    cosine = reference->cosine;
    dx0 = left - center_x;
    px0 = dx0 * cosine;
    sine = reference->sine;
    dy0 = center_y - top;
    py0 = dy0 * sine;
    qx0 = (0 - dx0) * sine;
    qy0 = dy0 * cosine;
    dx1 = right - center_x;
    px1 = dx1 * cosine;
    qx1 = (0 - dx1) * sine;
    dy1 = center_y - bottom;
    py1 = dy1 * sine;
    qy1 = dy1 * cosine;
    numerators[0] = px0 + py0;
    numerators[1] = qx0 + qy0;
    numerators[2] = px1 + py0;
    numerators[3] = qx1 + qy0;
    numerators[4] = px0 + py1;
    numerators[5] = qx0 + qy1;
    numerators[6] = px1 + py1;
    numerators[7] = qx1 + qy1;
    for (index = 0; index < 8; ++index)
    {
        value = ((uint32)((sint32)numerators[index] >> 12)) << 5;
        if (radius == 0)
            xport_mips_break(7);
        if (radius == 0xFFFFFFFF && value == 0x80000000)
            xport_mips_break(6);
        projected[index] = (sint32)value / (sint32)radius;
    }
    packet = hud_control.packets.cur + 0xAA4;
    quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
    quad->p[0].xy.x = (sint16)(uint16)(projected[0] + 44);
    quad->p[0].xy.y = (sint16)(uint16)(projected[1] + 46);
    quad->p[1].xy.x = (sint16)(uint16)(projected[2] + 44);
    quad->p[1].xy.y = (sint16)(uint16)(projected[3] + 46);
    quad->p[2].xy.x = (sint16)(uint16)(projected[4] + 44);
    quad->p[2].xy.y = (sint16)(uint16)(projected[5] + 46);
    quad->p[3].xy.x = (sint16)(uint16)(projected[6] + 44);
    quad->p[3].xy.y = (sint16)(uint16)(projected[7] + 46);
    value = reference->x;
    value = ((uint32)((sint32)(dx0 + value) >> 12)) + 64;
    quad->p[2].uv.u = (uint8)(value);
    quad->p[0].uv.u = (uint8)(value);
    value = reference->x;
    value = ((uint32)((sint32)(dx1 + value) >> 12)) + 64;
    quad->p[3].uv.u = (uint8)(value);
    quad->p[1].uv.u = (uint8)(value);
    value = reference->z;
    value = ((uint32)((sint32)(dy0 - value) >> 12)) + 64;
    quad->p[1].uv.v = (uint8)(value);
    quad->p[0].uv.v = (uint8)(value);
    value = reference->z;
    value = ((uint32)((sint32)(dy1 - value) >> 12)) + 64;
    quad->p[3].uv.v = (uint8)(value);
    quad->p[2].uv.v = (uint8)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

typedef struct
{
    uint16 x, y;
    uint8 u[4], v[4];
} HUD_MAP_QUAD;

typedef struct
{
    uint8 x, y, w, h;
} HUD_FIXED_QUAD;

typedef struct
{
    uint8 x, y;
} HUD_FIXED_POINT;

typedef struct
{
    uint16 x, y;
    uint8 u;
} HUD_FIXED_SPRITE;

typedef struct
{
    uint16 angle;
    uint8 shift;
} HUD_WAYPOINT_RAY;

enum
{
    HUD_MAP_QUAD_COUNT = 4,
    HUD_FIXED_QUAD_COUNT = 7,
    HUD_FIXED_POINT_COUNT = 4,
    HUD_FIXED_SPRITE_COUNT = 4,
    HUD_WAYPOINT_RAY_COUNT = 2
};

static const HUD_MAP_QUAD hud_map_quads[HUD_MAP_QUAD_COUNT] = {{12, 14, {80, 112, 80, 112}, {55, 55, 87, 87}}, {44, 14, {111, 79, 111, 79}, {55, 55, 87, 87}}, {12, 46, {80, 112, 80, 112}, {86, 86, 54, 54}}, {44, 46, {111, 79, 111, 79}, {86, 86, 54, 54}}};

static const HUD_FIXED_QUAD hud_fixed_quads[HUD_FIXED_QUAD_COUNT] = {{96, 72, 16, 96}, {112, 56, 96, 128}, {208, 72, 16, 96}, {96, 56, 16, 16}, {208, 56, 16, 16}, {96, 168, 16, 16}, {208, 168, 16, 16}};

static const HUD_FIXED_POINT hud_fixed_points[HUD_FIXED_POINT_COUNT] = {{96, 56}, {208, 56}, {96, 168}, {208, 168}};

static const HUD_FIXED_SPRITE hud_fixed_sprites[HUD_FIXED_SPRITE_COUNT] = {{154, 47, 124}, {226, 116, 136}, {82, 116, 148}, {154, 186, 160}};

static const HUD_WAYPOINT_RAY hud_waypoint_rays[HUD_WAYPOINT_RAY_COUNT] = {{64768, 10}, {768, 10}};

static void hud_radar_draw_waypoint(const HUD_RADAR_REFERENCE *reference, const uint32 *waypoint)
{
    LINE_F2 *line;
    POLY_F3 *triangle;
    uint32 value, other, dx, dz, ax, az, magnitude, a, b, scale, cosine, sine, numerator, x, y;
    uint32 i, angle, shift, packet, heading, buffers, distance, packed_xy, flag, size, phase, cursor;
    uint16 vector[3];
    FUNCTION_MARKER(0x8003E274, "ACE3.BIN");
    value = (waypoint[0]);
    other = reference->x;
    dx = ((sint32)value >> 12) - other;
    value = (waypoint[2]);
    other = reference->z;
    dz = other - ((sint32)value >> 12);
    ax = (sint32)dx < 0 ? 0 - dx : dx;
    az = (sint32)dz < 0 ? 0 - dz : dz;
    if ((sint32)ax < 0x4000 && (sint32)az < 0x4000)
        magnitude = game_square_root(dx * dx + dz * dz);
    else
    {
        a = (sint32)dx >> 4;
        b = (sint32)dz >> 4;
        magnitude = game_square_root(a * a + b * b) << 4;
    }
    scale = hud_control.map_scale;
    if ((sint32)scale < (sint32)magnitude)
        return;
    cosine = reference->cosine;
    sine = reference->sine;
    numerator = dx * cosine + dz * sine;
    numerator = ((uint32)((sint32)numerator >> 12)) << 5;
    if (scale == 0)
        xport_mips_break(7);
    if (scale == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    x = (sint32)numerator / (sint32)scale;
    numerator = (0 - dx) * sine + dz * cosine;
    numerator = ((uint32)((sint32)numerator >> 12)) << 5;
    if (scale == 0)
        xport_mips_break(7);
    if (scale == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    y = (sint32)numerator / (sint32)scale;
    if (x + 31 >= 63 || y + 31 >= 63)
        return;
    x += 44;
    y += 46;
    for (i = 0; i < 2; ++i)
    {
        angle = hud_waypoint_rays[i].angle;
        packet = hud_control.packets.cur;
        heading = (waypoint[4] >> 16);
        shift = hud_waypoint_rays[i].shift;
        packet += 0xC54 + i * 16;
        line = (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        value = reference->heading;
        line->rgbc.g = (uint8)(0);
        line->rgbc.b = (uint8)(0);
        line->p[0].x = (sint16)(uint16)(x);
        line->p[0].y = (sint16)(uint16)(y);
        angle = (angle + value - heading) & 0x3FFF;
        a = game_wave_at((angle << 1));
        line->rgbc.r = (uint8)(255);
        b = game_wave_at(((angle + 0x1000) << 1));
        a = (sint32)a >> (shift & 31);
        b = (sint32)b >> (shift & 31);
        line->p[1].y = (sint16)(uint16)(y + b);
        line->p[1].x = (sint16)(uint16)(x + a);
    }
    buffers = hud_control.packets.arena_begin;
    hud_control.packets.flags = 1;
    distance = game_measure_distance_gte(waypoint, game_runtime.camera_view.pose.position);
    if ((sint32)distance > 0x7FFF)
        return;
    value = (waypoint[0]);
    other = game_runtime.camera_view.pose.position[0];
    vector[0] = (sint32)(value - other) >> 12;
    value = (waypoint[1]);
    other = game_runtime.camera_view.pose.position[1];
    vector[1] = (sint32)(value - other) >> 12;
    value = (waypoint[2]);
    other = game_runtime.camera_view.pose.position[2];
    vector[2] = (sint32)(value - other) >> 12;
    SetRotMatrix(&game_runtime.mission_matrices.view);
    i = 5;
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    packed_xy = xport_gte_read_data(14);
    value = xport_gte_read_data(8);
    flag = xport_gte_read_control(31);
    value = xport_gte_read_data(19);
    if ((sint32)flag < 0)
        return;
    size = (sint32)(0x8000 - distance) >> 10;
    if ((sint32)size >= 9)
        size = 8;
    if ((sint32)size < 2)
        size = 2;
    packet = hud_control.packets.cur;
    cursor = packet + 0xC82;
    for (i = 0; i < 3; ++i)
    {
        triangle = (POLY_F3 *)mission_packet_span(cursor - 14, sizeof(*triangle));
        value = game_runtime.mission_control.ticks;
        triangle->rgbc.b = (uint8)(0);
        phase = (i + (((uint32)((sint32)value >> 2)) & 3)) << 6;
        triangle->rgbc.r = (uint8)(phase);
        triangle->rgbc.g = (uint8)((phase & 0xC0) >> 1);
        triangle->p[0].x = (sint16)(uint16)(packed_xy & 0xFFFF);
        value = (packed_xy >> 16) - (i * 2 + 1) * size;
        triangle->p[0].y = (sint16)(uint16)(value);
        triangle->p[1].x = (sint16)(uint16)((packed_xy & 0xFFFF) + size);
        triangle->p[2].x = (sint16)(uint16)((packed_xy & 0xFFFF) - size);
        value = (packed_xy >> 16) - (i * 2 + 2) * size;
        triangle->p[2].y = (sint16)(uint16)(value);
        triangle->p[1].y = (sint16)(uint16)(value);
        cursor += 20;
    }
    buffers = hud_control.packets.arena_begin;
    value = hud_control.packets.flags;
    hud_control.packets.flags = value | 2;
    return;
}

void hud_init_packet_banks(void)
{
    typedef struct
    {
        uint8 prim, vertex, component;
        uint16 value;
    } HUD_PACKET_COMPONENT;

    HUD_GPU_BANK *bank;
    HUD_GPU_BANK *banks[2];
    POLY_FT4 *quad;
    POLY_F3 *triangle;
    POLY_F4 *poly;
    uint32 *mode0, *mode1, *mode2, *mode3;
    DR_MODE *mask0, *mask1, *mask2;
    static const HUD_PACKET_COMPONENT uv_coordinates[] = {{0, 0, 0, 112}, {0, 0, 1, 55}, {0, 1, 0, 128}, {0, 1, 1, 55}, {0, 2, 0, 112}, {0, 2, 1, 71}, {0, 3, 0, 128}, {0, 3, 1, 71}, {1, 0, 0, 127}, {1, 0, 1, 55}, {1, 1, 0, 111}, {1, 1, 1, 55}, {1, 2, 0, 127}, {1, 2, 1, 71}, {1, 3, 0, 111}, {1, 3, 1, 71}, {2, 0, 0, 112}, {2, 0, 1, 70}, {2, 1, 0, 128}, {2, 1, 1, 70}, {2, 2, 0, 112}, {2, 2, 1, 54}, {2, 3, 0, 128}, {2, 3, 1, 54}, {3, 0, 0, 127}, {3, 0, 1, 70}, {3, 1, 0, 111}, {3, 1, 1, 70}, {3, 2, 0, 127}, {3, 2, 1, 54}, {3, 3, 0, 111}, {3, 3, 1, 54}};
    static const HUD_PACKET_COMPONENT polyline_coordinates[] = {{0, 0, 1, 88}, {0, 1, 1, 88}, {1, 0, 1, 88}, {1, 1, 1, 88}, {0, 1, 0, 128}, {0, 2, 0, 128}, {2, 1, 0, 128}, {2, 2, 0, 128}, {0, 0, 0, 136}, {0, 2, 1, 96}, {1, 0, 0, 184}, {1, 1, 0, 192}, {1, 2, 0, 192}, {1, 2, 1, 96}, {2, 0, 0, 136}, {2, 0, 1, 152}, {2, 1, 1, 152}, {2, 2, 1, 144}, {3, 0, 0, 184}, {3, 0, 1, 152}, {3, 1, 0, 192}, {3, 1, 1, 152}, {3, 2, 0, 192}, {3, 2, 1, 144}};
    static const HUD_PACKET_COMPONENT line_coordinates[] = {{0, 1, 0, 164}, {1, 1, 0, 164}, {2, 1, 0, 164}, {3, 1, 0, 132}, {0, 0, 0, 156}, {1, 0, 0, 156}, {2, 0, 0, 156}, {4, 0, 0, 188}, {1, 0, 1, 120}, {1, 1, 1, 120}, {3, 0, 1, 120}, {3, 1, 1, 120}, {4, 0, 1, 120}, {4, 1, 1, 120}, {5, 1, 1, 124}, {6, 1, 1, 124}, {7, 1, 1, 124}, {0, 0, 1, 88}, {0, 1, 1, 88}, {2, 0, 1, 152}, {2, 1, 1, 152}, {3, 0, 0, 128}, {4, 1, 0, 192}, {5, 0, 0, 128}, {5, 0, 1, 116}, {5, 1, 0, 128}, {6, 0, 0, 160}, {6, 0, 1, 116}, {6, 1, 0, 160}, {7, 0, 0, 192}, {7, 0, 1, 116}, {7, 1, 0, 192}, {8, 0, 0, 160}, {8, 1, 1, 92}, {8, 0, 1, 88}, {8, 1, 0, 160}, {9, 0, 0, 160}, {9, 0, 1, 152}, {9, 1, 0, 160}, {9, 1, 1, 148}};
    uint32 group, index, block, table, packet, other, first, second, third, value;
    PSX_RECT rect;
    SPRT *spr;
    TILE *tile;
    LINE_F2 *line;
    LINE_F3 *polyline;
    FUNCTION_MARKER(0x8003B924, "ACE3.BIN");
    block = arena_alloc(28);
    hud_control.packets.arena_begin = block;
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(4224);
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.packets[group]);
        bank = &hud_gpu_storage.packets[group];
        banks[group] = bank;
        table = hud_control.packets.arena_begin;
        hud_control.packets.cur = block;
        hud_control.packets.bank[group] = block;
        hud_control.packets.current_data = bank;
        hud_control.packets.bank_data[group] = bank;
        for (index = 0; index < 64; ++index)
        {
            packet = block + 31 + index * 20;
            spr = &bank->sprs[index];
            setSprt(spr);
            spr->p[0].attr = 0x7F98;
            setSemiTrans(spr, 1);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 64; ++index)
        {
            packet = block + 0x51F + index * 16;
            tile = &bank->tiles[index];
            setTile(tile);
            setSemiTrans(tile, 1);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 8; ++index)
        {
            packet = block + 0x91F + index * 16;
            line = &bank->lines[index];
            setLineF2(line);
            line->rgbc.r = 0;
            line->rgbc.g = 128;
            line->rgbc.b = 0;
            setSemiTrans(line, 1);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 8; ++index)
        {
            packet = block + 0x99F + index * 24;
            polyline = &bank->polylines[index];
            setLineF3(polyline);
            polyline->rgbc.r = 0;
            polyline->rgbc.g = 128;
            polyline->rgbc.b = 0;
            setSemiTrans(polyline, 1);
        }
        rect.x = 12;
        rect.y = group == 1 ? 270 : 14;
        rect.w = 64;
        rect.h = 64;
        SetDrawArea(&bank->areas[0], &rect);
        rect.w = 320;
        rect.x = 0;
        rect.y = group == 1 ? 256 : 0;
        rect.h = 240;
        SetDrawArea(&bank->areas[1], &rect);
        block = hud_control.packets.cur;
        for (index = 0; index < 2; ++index)
        {
            packet = block + 0xC5B + index * 16;
            line = &bank->clip_lines[index];
            setlen(line, 3);
            line->rgbc.code = (uint8)(66);
        }
        for (index = 0; index < 3; ++index)
        {
            triangle = &bank->triangles[index];
            setlen(triangle, 4);
            triangle->rgbc.code = (uint8)(32);
            packet = hud_control.packets.cur + index * 20 + 0xC74;
            triangle->rgbc.code = (uint8)(triangle->rgbc.code | 2);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 7; ++index)
        {
            packet = block + 0xCF6 + index * 40;
            quad = &bank->fixed_quads[index];
            quad->p[1].attr = (uint16)(5);
            setlen(quad, 9);
            quad->rgbc.code = (uint8)(46);
            quad->rgbc.r = (uint8)(131);
            quad->rgbc.g = (uint8)(131);
            quad->rgbc.b = (uint8)(131);
            quad->p[0].attr = (uint16)(0x5714);
            quad->p[0].xy.x = (sint16)(uint16)(hud_fixed_quads[index].x);
            quad->p[0].xy.y = (sint16)(uint16)(hud_fixed_quads[index].y);
            first = hud_fixed_quads[index].x;
            second = hud_fixed_quads[index].w;
            quad->p[1].xy.x = (sint16)(uint16)(first + second);
            quad->p[1].xy.y = (sint16)(uint16)(hud_fixed_quads[index].y);
            quad->p[2].xy.x = (sint16)(uint16)(hud_fixed_quads[index].x);
            first = hud_fixed_quads[index].y;
            second = hud_fixed_quads[index].h;
            quad->p[2].xy.y = (sint16)(uint16)(first + second);
            first = hud_fixed_quads[index].x;
            second = hud_fixed_quads[index].w;
            quad->p[3].xy.x = (sint16)(uint16)(first + second);
            first = hud_fixed_quads[index].y;
            second = hud_fixed_quads[index].h;
            quad->p[3].xy.y = (sint16)(uint16)(first + second);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0xE0E + index * 40;
            quad = &bank->markers[index];
            other = block + 0xE9A + index * 16;
            tile = &bank->marker_tiles[index];
            setlen(quad, 9);
            quad->rgbc.code = (uint8)(46);
            quad->rgbc.r = (uint8)(131);
            quad->rgbc.g = (uint8)(131);
            quad->rgbc.b = (uint8)(131);
            quad->p[1].attr = (uint16)(54);
            quad->p[0].attr = (uint16)(0x7F99);
            quad->p[0].xy.x = (sint16)(uint16)(hud_fixed_points[index].x);
            quad->p[0].xy.y = (sint16)(uint16)(hud_fixed_points[index].y);
            quad->p[1].xy.x = (sint16)(uint16)(hud_fixed_points[index].x + 16);
            quad->p[1].xy.y = (sint16)(uint16)(hud_fixed_points[index].y);
            quad->p[2].xy.x = (sint16)(uint16)(hud_fixed_points[index].x);
            quad->p[2].xy.y = (sint16)(uint16)(hud_fixed_points[index].y + 16);
            quad->p[3].xy.x = (sint16)(uint16)(hud_fixed_points[index].x + 16);
            quad->p[3].xy.y = (sint16)(uint16)(hud_fixed_points[index].y + 16);
            setlen(tile, 3);
            tile->rgbc.code = (uint8)(98);
            tile->rgbc.r = (uint8)(0);
            tile->rgbc.g = (uint8)(0);
            tile->rgbc.b = (uint8)(0);
            tile->p[0].x = (sint16)(uint16)(hud_fixed_points[index].x);
            value = hud_fixed_points[index].y;
            tile->w = (sint16)(uint16)(16);
            tile->p[0].y = (sint16)(uint16)(value);
            tile->h = (sint16)(uint16)(16);
        }
        for (index = 0; index < sizeof(uv_coordinates) / sizeof(uv_coordinates[0]); ++index)
        {
            quad = &bank->markers[uv_coordinates[index].prim];
            if (uv_coordinates[index].component == 0)
                quad->p[uv_coordinates[index].vertex].uv.u = (uint8)(uv_coordinates[index].value);
            else
                quad->p[uv_coordinates[index].vertex].uv.v = (uint8)(uv_coordinates[index].value);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0xED2 + index * 24;
            polyline = &bank->frame_polylines[index];
            polyline->rgbc.code = (uint8)(72);
            setlen(polyline, 5);
            polyline->pad = (uint32)(0x55555555);
            polyline->rgbc.r = (uint8)(0);
            polyline->rgbc.g = (uint8)(255);
            polyline->rgbc.b = (uint8)(0);
            polyline->rgbc.code = (uint8)(74);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < sizeof(polyline_coordinates) / sizeof(polyline_coordinates[0]); ++index)
        {
            polyline = &bank->frame_polylines[polyline_coordinates[index].prim];
            if (polyline_coordinates[index].component == 0)
                polyline->p[polyline_coordinates[index].vertex].x = (sint16)(uint16)(polyline_coordinates[index].value);
            else
                polyline->p[polyline_coordinates[index].vertex].y = (sint16)(uint16)(polyline_coordinates[index].value);
        }
        for (index = 0; index < 10; ++index)
        {
            packet = block + 0xF32 + index * 16;
            line = &bank->frame_lines[index];
            setlen(line, 3);
            line->rgbc.code = (uint8)(66);
            line->rgbc.r = (uint8)(0);
            line->rgbc.g = (uint8)(255);
            line->rgbc.b = (uint8)(0);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < sizeof(line_coordinates) / sizeof(line_coordinates[0]); ++index)
        {
            line = &bank->frame_lines[line_coordinates[index].prim];
            if (line_coordinates[index].component == 0)
                line->p[line_coordinates[index].vertex].x = (sint16)(uint16)(line_coordinates[index].value);
            else
                line->p[line_coordinates[index].vertex].y = (sint16)(uint16)(line_coordinates[index].value);
        }
        for (index = 0; index < 2; ++index)
        {
            packet = block + 0xFD2 + index * 24;
            poly = &bank->frame_polys[index];
            setlen(poly, 5);
            poly->rgbc.code = (uint8)(42);
            poly->rgbc.r = (uint8)(16);
            poly->rgbc.g = (uint8)(32);
            poly->rgbc.b = (uint8)(16);
        }
        packet = hud_control.packets.cur + 0xFFC;
        spr = &bank->center;
        spr->rgbc.code = (uint8)(102);
        setlen(spr, 4);
        spr->rgbc.r = (uint8)(131);
        spr->rgbc.g = (uint8)(131);
        spr->rgbc.b = (uint8)(131);
        block = hud_control.packets.cur;
        spr->p[0].attr = (uint16)(0x7F98);
        spr->p[0].xy.x = (sint16)(uint16)(156);
        spr->p[0].xy.y = (sint16)(uint16)(116);
        spr->w = (sint16)(uint16)(8);
        spr->h = (sint16)(uint16)(9);
        for (index = 0; index < 4; ++index)
        {
            packet = block + 0x1022 + index * 20;
            spr = &bank->fixed_sprs[index];
            setlen(spr, 4);
            spr->rgbc.code = (uint8)(102);
            spr->rgbc.r = (uint8)(0);
            spr->rgbc.g = (uint8)(131);
            spr->rgbc.b = (uint8)(0);
            spr->p[0].attr = (uint16)(0x7F98);
            spr->p[0].xy.x = (sint16)(uint16)(hud_fixed_sprites[index].x);
            spr->p[0].xy.y = (sint16)(uint16)(hud_fixed_sprites[index].y);
            value = hud_fixed_sprites[index].u;
            spr->p[0].uv.v = (uint8)(103);
            spr->w = (sint16)(uint16)(12);
            spr->h = (sint16)(uint16)(7);
            spr->p[0].uv.u = (uint8)(value);
        }
        mode0 = bank->modes[0];
        setlen(mode0, 1);
        first = hud_control.packets.cur;
        mode1 = bank->modes[1];
        setlen(mode1, 1);
        second = hud_control.packets.cur;
        mode0 = bank->modes[0];
        mode0[1] = (0xE1000205);
        mode2 = bank->modes[2];
        setlen(mode2, 1);
        third = hud_control.packets.cur;
        mode1 = bank->modes[1];
        mode1[1] = (0xE1000236);
        mode3 = bank->modes[3];
        setlen(mode3, 1);
        block = hud_control.packets.cur;
        mode2 = bank->modes[2];
        mode2[1] = (0xE1000236);
        mode3 = bank->modes[3];
        mode3[1] = (0xE1000236);
        mask0 = &bank->masks[0];
        mask1 = &bank->masks[1];
        mask2 = &bank->masks[2];
        mask0->code[0] = (uint32)(0xE6000000);
        mask0->code[1] = (uint32)(0);
        mask1->code[0] = (uint32)(0xE6000002);
        mask1->code[1] = (uint32)(0);
        mask2->code[0] = (uint32)(0xE6000000);
        mask2->code[1] = (uint32)(0);
    }
    {
        (void)(hud_radar_init_map(banks));
        return;
    }
}

void hud_radar_init_map(HUD_GPU_BANK *const banks[2])
{
    HUD_MAP_GPU_BANK *map;
    POLY_FT4 *quad, *chain0, *chain1;
    SPRT *spr;
    TILE *tile;
    DR_TPAGE *mode0, *mode1;
    DR_MODE *mask0, *mask1, *mask2, *mask3, *mask4;
    uint32 group, index, block, packet, first, second, value, other;
    const HUD_MAP_QUAD *table;
    const uint8 *uv_x, *uv_y;
    FUNCTION_MARKER(0x8003C250, "ACE3.BIN");
    for (group = 0; group < 2; ++group)
    {
        block = hud_control.packets.bank[group];
        packet = block + 0xAA4;
        map = &banks[group]->map;
        quad = &map->base_quad;
        hud_control.packets.cur = block;
        hud_control.packets.current_data = banks[group];
        quad->rgbc.code = (uint8)(46);
        quad->rgbc.r = (uint8)(99);
        quad->rgbc.g = (uint8)(99);
        quad->rgbc.b = (uint8)(99);
        quad->p[1].attr = (uint16)(5);
        setlen(quad, 9);
        quad->p[0].attr = (uint16)(0x5714);
        for (index = 0; index < 8; ++index)
        {
            table = &hud_map_quads[index & 3];
            packet = hud_control.packets.cur + 0xACC + index * 40;
            quad = &map->quads[index];
            quad->rgbc.code = (uint8)(46);
            quad->p[1].attr = (uint16)(54);
            setlen(quad, 9);
            quad->rgbc.r = (uint8)(128);
            quad->rgbc.g = (uint8)(128);
            quad->rgbc.b = (uint8)(128);
            quad->p[0].attr = (uint16)(0x7F99);
            quad->p[0].xy.x = (sint16)(uint16)(table->x);
            quad->p[0].xy.y = (sint16)(uint16)(table->y);
            quad->p[1].xy.x = (sint16)(uint16)(table->x + 32);
            quad->p[1].xy.y = (sint16)(uint16)(table->y);
            quad->p[2].xy.x = (sint16)(uint16)(table->x);
            quad->p[2].xy.y = (sint16)(uint16)(table->y + 32);
            quad->p[3].xy.x = (sint16)(uint16)(table->x + 32);
            value = table->y;
            uv_x = hud_map_quads[index & 3].u;
            quad->p[3].xy.y = (sint16)(uint16)(value + 32);
            value = uv_x[0];
            uv_y = hud_map_quads[index & 3].v;
            quad->p[0].uv.u = (uint8)(value);
            quad->p[0].uv.v = (uint8)(uv_y[0]);
            quad->p[1].uv.u = (uint8)(uv_x[1]);
            quad->p[1].uv.v = (uint8)(uv_y[1]);
            quad->p[2].uv.u = (uint8)(uv_x[2]);
            quad->p[2].uv.v = (uint8)(uv_y[2]);
            quad->p[3].uv.u = (uint8)(uv_x[3]);
            quad->p[3].uv.v = (uint8)(uv_y[3]);
        }
        block = hud_control.packets.cur;
        for (index = 0; index < 3; ++index)
        {
            packet = block + index * 40;
            chain0 = &map->quads[index];
            chain1 = &map->quads[index + 4];
            first = (block + 0xAF4 + index * 40) & 0xFFFFFF;
            second = (block + 0xB94 + index * 40) & 0xFFFFFF;
            value = chain0->tag;
            other = chain1->tag;
            chain0->tag = (uint32)((value & 0xFF000000) | first);
            chain1->tag = (uint32)((other & 0xFF000000) | second);
        }
        packet = hud_control.packets.cur + 0xC0C;
        spr = &map->sprites[0];
        spr->p[0].attr = (uint16)(0x7F18);
        spr->p[0].uv.v = (uint8)(55);
        spr->w = (sint16)(uint16)(80);
        spr->rgbc.code = (uint8)(100);
        spr->h = (sint16)(uint16)(73);
        setlen(spr, 4);
        spr->p[0].uv.u = (uint8)(0);
        spr->p[0].xy.x = (sint16)(uint16)(6);
        spr->rgbc.code = (uint8)(102);
        block = hud_control.packets.cur;
        spr->p[0].xy.y = (sint16)(uint16)(12);
        packet = block + 0xC20;
        spr = &map->sprites[1];
        spr->p[0].attr = (uint16)(0x7F98);
        spr->rgbc.code = (uint8)(100);
        spr->p[0].uv.v = (uint8)(45);
        setlen(spr, 4);
        spr->p[0].uv.u = (uint8)(0);
        spr->h = (sint16)(uint16)(10);
        spr->p[0].xy.x = (sint16)(uint16)(10);
        spr->rgbc.code = (uint8)(102);
        block = hud_control.packets.cur;
        spr->w = (sint16)(uint16)(12);
        spr->p[0].xy.y = (sint16)(uint16)(12);
        mode0 = &map->modes[0];
        setlen(mode0, 1);
        first = hud_control.packets.cur;
        mode1 = &map->modes[1];
        setlen(mode1, 1);
        second = hud_control.packets.cur;
        mode0 = &map->modes[0];
        mode0->code[0] = (uint32)(0xE1000236);
        mode1 = &map->modes[1];
        mode1->code[0] = (uint32)(0xE1000236);
        for (index = 0; index < 2; ++index)
        {
            packet = hud_control.packets.cur + 0xC34 + index * 16;
            tile = &map->tiles[index];
            setlen(tile, 3);
            tile->rgbc.code = (uint8)(98);
            tile->rgbc.r = (uint8)(0);
            tile->rgbc.g = (uint8)(0);
            tile->rgbc.b = (uint8)(0);
            tile->p[0].x = (sint16)(uint16)(hud_map_quads[0].x);
            value = hud_map_quads[0].y;
            tile->w = (sint16)(uint16)(64);
            tile->h = (sint16)(uint16)(64);
            tile->p[0].y = (sint16)(uint16)(value);
        }
        block = hud_control.packets.cur;
        mask0 = &map->masks[0];
        mask1 = &map->masks[1];
        mask2 = &map->masks[2];
        mask3 = &map->masks[3];
        mask4 = &map->masks[4];
        mask0->code[0] = (uint32)(0xE6000000);
        mask0->code[1] = (uint32)(0);
        mask1->code[0] = (uint32)(0xE6000002);
        mask1->code[1] = (uint32)(0);
        mask2->code[0] = (uint32)(0xE6000000);
        mask2->code[1] = (uint32)(0);
        mask3->code[0] = (uint32)(0xE6000002);
        mask3->code[1] = (uint32)(0);
        mask4->code[0] = (uint32)(0xE6000000);
        mask4->code[1] = (uint32)(0);
    }
    return;
}

typedef struct
{
    uint8 u, v, w, h;
} HUD_MAP_UV_RECT;

enum
{
    HUD_MAP_UV_COUNT = 7
};

static const HUD_MAP_UV_RECT hud_map_uvs[HUD_MAP_UV_COUNT] = {{0, 8, 8, 48}, {8, 0, 48, 64}, {56, 8, 8, 48}, {0, 0, 8, 8}, {56, 0, 8, 8}, {0, 56, 8, 8}, {56, 56, 8, 8}};

void hud_map_draw(void)
{
    HUD_GPU_BANK *bank;
    POLY_FT4 *quad;
    POLY_F4 *poly;
    SPRT *spr;
    TILE *tile, *native_tile;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 x, y, offset_x, offset_y, base, packet, index, a, b, c, value, angle, next_angle, last_angle;
    const HUD_MAP_UV_RECT *uv;
    uint32 center_x, center_y, state, count, end, cursor, flags, object_flags, opcode, result;
    uint16 origin[2], point[2];
    FUNCTION_MARKER(0x8003E69C, "ACE3.BIN");
    bank = hud_control.packets.current_data;
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    x = pose_owner_object->pose.position[0];
    y = pose_owner_object->pose.position[2];
    x = ((uint32)((sint32)x >> 23)) + 128;
    y = ((uint32)((sint32)(0 - y) >> 23)) + 128;
    if ((sint32)x >= 256)
        x = 255;
    if ((sint32)x < 0)
        x = 0;
    if ((sint32)y >= 256)
        y = 255;
    if ((sint32)y < 0)
        y = 0;
    origin[0] = x < 64 ? 0 : x >= 193 ? 128 : x - 64;
    origin[1] = y < 64 ? 0 : y >= 193 ? 128 : y - 64;
    offset_x = origin[0] >> 1;
    offset_y = origin[1] >> 1;
    base = hud_control.packets.cur;
    packet = base + 0xCF9;
    for (index = 0; index < 7; ++index, packet += 40)
    {
        quad = &bank->fixed_quads[index];
        uv = &hud_map_uvs[index];
        value = uv->u;
        quad->p[0].uv.u = (uint8)(value + offset_x);
        value = uv->v;
        quad->p[0].uv.v = (uint8)(value + offset_y);
        value = uv->u;
        a = uv->w;
        quad->p[1].uv.u = (uint8)(a + value + offset_x);
        value = uv->v;
        quad->p[1].uv.v = (uint8)(value + offset_y);
        value = uv->u;
        quad->p[2].uv.u = (uint8)(value + offset_x);
        value = uv->v;
        a = uv->h;
        quad->p[2].uv.v = (uint8)(a + value + offset_y);
        value = uv->u;
        a = uv->w;
        quad->p[3].uv.u = (uint8)(a + value + offset_x);
        value = uv->v;
        a = uv->h;
        quad->p[3].uv.v = (uint8)(a + value + offset_y);
    }
    packet = hud_control.packets.cur + 0xFFC;
    spr = &bank->center;
    hud_map_world_position(pose_owner_object->pose.position, origin, point);
    center_x = point[0] + 96;
    center_y = point[1] + 56;
    spr->p[0].xy.x = (sint16)(uint16)(point[0] + 92);
    spr->p[0].xy.y = (sint16)(uint16)(point[1] + 52);
    value = pose_owner_object->pose.angles[1];
    spr->p[0].uv.v = (uint8)(59);
    spr->p[0].uv.u = (uint8)((((0x4200 - (value & 0x3FFF)) >> 7) & 0x78) - 128);
    value = (sint16)pose_owner_object->pose.angles[1];
    base = hud_control.packets.cur;
    angle = (0x1AAC - value) & 0x3FFF;
    packet = base + 0xFE2;
    for (index = 0; index < 2; ++index, packet += 24)
    {
        poly = &bank->frame_polys[index];
        next_angle = (angle + 0x2AA) & 0x3FFF;
        last_angle = (next_angle + 0x2AA) & 0x3FFF;
        a = (uint16)game_wave_at((angle << 1));
        b = (uint16)game_wave_at(((angle + 0x1000) << 1));
        c = (uint16)game_wave_at(((next_angle + 0x1000) << 1));
        poly->p[1].x = (sint16)(uint16)(center_x);
        poly->p[1].y = (sint16)(uint16)(center_y);
        poly->p[0].y = (sint16)(uint16)(center_y + (uint32)((sint16)b >> 10));
        poly->p[0].x = (sint16)(uint16)(center_x + (uint32)((sint16)a >> 10));
        a = (uint16)game_wave_at((next_angle << 1));
        poly->p[2].y = (sint16)(uint16)(center_y + (uint32)((sint16)c >> 10));
        poly->p[2].x = (sint16)(uint16)(center_x + (uint32)((sint16)a >> 10));
        a = (uint16)game_wave_at((last_angle << 1));
        b = (uint16)game_wave_at(((last_angle + 0x1000) << 1));
        poly->p[3].x = (sint16)(uint16)(center_x + (uint32)((sint16)a >> 10));
        poly->p[3].y = (sint16)(uint16)(center_y + (uint32)((sint16)b >> 10));
        angle = last_angle;
    }
    state = hud_control.packets.arena_begin;
    count = hud_control.packets.tile_count;
    base = hud_control.packets.cur;
    end = base + 0x518 + (count << 4);
    cursor = end + 10;
    native_tile = &bank->tiles[count];
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        flags = (*mission_actor_flags(index));
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        if ((flags & 0x100) == 0)
            continue;
        object_flags = actor_traits_object->flags;
        if ((object_flags & 0x4000) == 0 || (flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        result = hud_map_world_position(pose_owner_object->pose.position, origin, point);
        if ((sint32)result < 0)
            continue;
        end += 16;
        tile = native_tile;
        x = point[0];
        opcode = tile->rgbc.code;
        tile->p[0].x = (sint16)(uint16)(x + 96);
        y = point[1];
        tile->rgbc.r = (uint8)(255);
        tile->rgbc.g = (uint8)(64);
        tile->rgbc.b = (uint8)(64);
        tile->w = (sint16)(uint16)(2);
        tile->h = (sint16)(uint16)(2);
        value = (uint16)tile->p[0].x;
        tile->rgbc.code = (uint8)(opcode & 0xFD);
        state = hud_control.packets.arena_begin;
        y += 56;
        tile->p[0].y = (sint16)(uint16)(y);
        tile->p[0].x = (sint16)(uint16)(value - 1);
        tile->p[0].y = (sint16)(uint16)(y - 1);
        count = hud_control.packets.tile_count;
        cursor += 16;
        ++native_tile;
        hud_control.packets.tile_count = count + 1;
    }
    cursor = end + 7;
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        flags = (*mission_actor_flags(index));
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        if (flags & 0x100)
            continue;
        object_flags = actor_traits_object->flags;
        if ((object_flags & 0x4000) == 0 || (flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        result = hud_map_world_position(pose_owner_object->pose.position, origin, point);
        if ((sint32)result < 0)
            continue;
        tile = native_tile;
        tile->p[0].x = (sint16)(uint16)(point[0] + 96);
        tile->p[0].y = (sint16)(uint16)(point[1] + 56);
        object_flags = actor_traits_object->flags;
        if (object_flags & 0x40)
        {
            tile->rgbc.r = (uint8)(0);
            tile->rgbc.g = (uint8)(0);
            tile->rgbc.b = (uint8)(255);
        }
        else if (object_flags & 0x100)
        {
            tile->rgbc.r = (uint8)(255);
            tile->rgbc.g = (uint8)(255);
            tile->rgbc.b = (uint8)(0);
        }
        else
        {
            tile->rgbc.r = (uint8)(64);
            tile->rgbc.g = (uint8)(64);
            tile->rgbc.b = (uint8)(64);
        }
        tile->w = (sint16)(uint16)(2);
        tile->h = (sint16)(uint16)(2);
        value = tile->rgbc.code;
        tile->rgbc.code = (uint8)(value | 2);
        state = hud_control.packets.arena_begin;
        count = hud_control.packets.tile_count;
        cursor += 16;
        ++native_tile;
        hud_control.packets.tile_count = count + 1;
    }
    return;
}

void hud_map_submit(void)
{
    HUD_GPU_BANK *bank;
    uint32 *ot_word;
    uint32 flags, ordering, base, state, tag, head, packet, count, index, result;
    FUNCTION_MARKER(0x8003CB90, "ACE3.BIN");
    flags = game_runtime.mission_control.ticks;
    ordering = game_runtime.mission_control.ot;
    ot_word = (uint32 *)mission_ot_span(ordering, sizeof(*ot_word));
    bank = hud_control.packets.current_data;
    if (flags & 8)
    {
        base = hud_control.packets.cur;
        head = (*ot_word);
        tag = bank->center.tag;
        bank->center.tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | ((base + 0xFFC) & 0xFFFFFF));
    }
    base = hud_control.packets.cur;
    head = (*ot_word);
    state = hud_control.packets.arena_begin;
    tag = bank->modes[3][0];
    bank->modes[3][0] = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    *ot_word = ((head & 0xFF000000) | ((base + 0x1078) & 0xFFFFFF));
    count = hud_control.packets.tile_count;
    if ((sint32)count > 0)
    {
        packet = base + 0x518;
        index = 0;
        do
        {
            ++index;
            tag = bank->tiles[index - 1].tag;
            head = (*ot_word);
            bank->tiles[index - 1].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = hud_control.packets.tile_count;
            packet += 16;
        } while ((sint32)index < (sint32)count);
    }
    head = (*ot_word);
    base = hud_control.packets.cur;
    tag = bank->modes[2][0];
    bank->modes[2][0] = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    *ot_word = ((head & 0xFF000000) | ((base + 0x1070) & 0xFFFFFF));
    packet = base + 0xFCC;
    for (index = 0; index < 2; ++index, packet += 24)
    {
        tag = bank->frame_polys[index].tag;
        head = (*ot_word);
        bank->frame_polys[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = hud_control.packets.cur;
    packet = base + 0x1010;
    for (index = 0; index < 4; ++index, packet += 20)
    {
        tag = bank->fixed_sprs[index].tag;
        head = (*ot_word);
        bank->fixed_sprs[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        *ot_word = (head);
    }
    base = hud_control.packets.cur;
    tag = bank->modes[1][0];
    bank->modes[1][0] = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    *ot_word = ((head & 0xFF000000) | ((base + 0x1068) & 0xFFFFFF));
    packet = base + 0xF2C;
    for (index = 0; index < 10; ++index, packet += 16)
    {
        tag = bank->frame_lines[index].tag;
        head = (*ot_word);
        bank->frame_lines[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = hud_control.packets.cur;
    packet = base + 0xECC;
    for (index = 0; index < 4; ++index, packet += 24)
    {
        tag = bank->frame_polylines[index].tag;
        head = (*ot_word);
        bank->frame_polylines[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = hud_control.packets.cur;
    packet = base + 0xCD4;
    for (index = 0; index < 3; ++index, packet += 40)
    {
        tag = bank->fixed_quads[index].tag;
        head = (*ot_word);
        bank->fixed_quads[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        *ot_word = (head);
    }
    base = hud_control.packets.cur;
    bank->masks[2].tag = (uint32)(head | 0x2000000);
    *ot_word = ((base + 0xCC8) & 0xFFFFFF);
    base = hud_control.packets.cur;
    packet = base + 0xD4C;
    for (index = 3; index < 7; ++index, packet += 40)
    {
        tag = bank->fixed_quads[index].tag;
        head = (*ot_word);
        bank->fixed_quads[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        *ot_word = (head);
    }
    base = hud_control.packets.cur;
    bank->masks[1].tag = (uint32)(head | 0x2000000);
    *ot_word = ((base + 0xCBC) & 0xFFFFFF);
    base = hud_control.packets.cur;
    packet = base + 0xDEC;
    for (index = 0; index < 4; ++index, packet += 40)
    {
        tag = bank->markers[index].tag;
        head = (*ot_word);
        bank->markers[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = hud_control.packets.cur;
    packet = base + 0xE8C;
    for (index = 0; index < 4; ++index, packet += 16)
    {
        tag = bank->marker_tiles[index].tag;
        head = (*ot_word);
        bank->marker_tiles[index].tag = (uint32)((tag & 0xFF000000) | (head & 0xFFFFFF));
        head = (*ot_word);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        *ot_word = (head);
    }
    base = hud_control.packets.cur;
    bank->masks[0].tag = (uint32)(head | 0x2000000);
    result = (base + 0xCB0) & 0xFFFFFF;
    *ot_word = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_markers_init(void)
{
    LINE_F2 *line;
    LINE_F3 *polyline;
    SPRT *spr;
    DR_TPAGE *mode;
    HUD_MARKER_PACKET_BANK *bank;
    uint32 group, index, block, table, packet, other;
    FUNCTION_MARKER(0x8003ECBC, "ACE3.BIN");
    block = arena_alloc(24);
    hud_control.marker_packets.arena_begin = block;
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(5464);
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.marker[group]);
        table = hud_control.marker_packets.arena_begin;
        hud_control.marker_packets.cur = block;
        hud_control.marker_packets.bank[group] = block;
        bank = &hud_gpu_storage.marker[group];
        hud_control.marker_packets.native = bank;
        hud_control.marker_packets.bank_data[group] = bank;
        for (index = 0; index < 64; ++index)
        {
            line = &bank->lines[index];
            setlen(line, 3);
            line->rgbc.code = (uint8)(64);
            line->rgbc.r = (uint8)(0);
            line->rgbc.g = (uint8)(255);
            line->rgbc.b = (uint8)(0);
            packet = hud_control.marker_packets.cur + index * 16 + 8;
            line->rgbc.code = (uint8)(line->rgbc.code | 2);
        }
        for (index = 0; index < 128; ++index)
        {
            polyline = &bank->polylines[index];
            setlen(polyline, 5);
            polyline->rgbc.code = (uint8)(72);
            packet = hud_control.marker_packets.cur + index * 24 + 0x408;
            polyline->rgbc.r = (uint8)(0);
            other = hud_control.marker_packets.cur;
            polyline->pad = (uint32)(0x55555555);
            polyline->rgbc.g = (uint8)(255);
            polyline->rgbc.b = (uint8)(0);
            packet = hud_control.marker_packets.cur + index * 24 + 0x408;
            polyline->rgbc.code = (uint8)(polyline->rgbc.code | 2);
        }
        for (index = 0; index < 64; ++index)
        {
            spr = &bank->labels[index];
            setlen(spr, 4);
            spr->rgbc.code = (uint8)(100);
            packet = hud_control.marker_packets.cur + index * 20;
            spr->p[0].attr = (uint16)(0x7F9B);
            spr->rgbc.code = (uint8)(spr->rgbc.code | 2);
        }
        for (index = 0; index < 4; ++index)
        {
            spr = &bank->digits[index];
            setlen(spr, 4);
            spr->rgbc.code = (uint8)(100);
            packet = hud_control.marker_packets.cur + index * 20;
            spr->p[0].attr = (uint16)(0x7F98);
            spr->rgbc.code = (uint8)(spr->rgbc.code | 2);
        }
        mode = &bank->mode;
        setlen(mode, 1);
        mode->code[0] = (uint32)(0xE1000636);
    }
    hud_markers_reset();
    {
        (void)(hud_lock_init());
        return;
    }
}

void hud_markers_draw(void)
{
    FUNCTION_MARKER(0x8003EFE0, "ACE3.BIN");
    hud_markers_begin();
    hud_markers_update();
    hud_lock_draw();
    {
        (void)(hud_markers_submit());
        return;
    }
}

void hud_markers_begin(void)
{
    uint32 buffers, value;
    FUNCTION_MARKER(0x8003F018, "ACE3.BIN");
    buffers = hud_control.marker_packets.arena_begin;
    hud_control.marker_packets.count[HUD_MARKER_DIGITS] = 0;
    hud_control.marker_packets.count[HUD_MARKER_LABELS] = 0;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = 0;
    hud_control.marker_packets.count[HUD_MARKER_LINES] = 0;
    value = game_runtime.display_buffer;
    hud_control.marker_packets.native = hud_control.marker_packets.bank_data[value];
    value = hud_control.marker_packets.bank[value];
    hud_control.marker_packets.cur = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_markers_submit(void)
{
    uint32 *tag_word, *ot_word;
    static const uint32 starts[4] = {8, 0x408, 0x1008, 0x1508};
    static const uint32 strides[4] = {16, 24, 20, 20};
    uint32 buffers, count, ot, group, index, packet, tag, head, value;
    FUNCTION_MARKER(0x8003F054, "ACE3.BIN");
    buffers = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    ot = game_runtime.mission_control.ot;
    ot_word = (uint32 *)mission_ot_span(ot, sizeof(*ot_word));
    for (group = 0; group < 4; ++group)
    {
        if (group == 2)
            buffers = hud_control.marker_packets.arena_begin;
        if (group != 0)
            count = hud_control.marker_packets.count[group];
        if ((sint32)count > 0)
        {
            index = 0;
            packet = hud_control.marker_packets.cur + starts[group];
            do
            {
                tag_word = (uint32 *)mission_packet_span(packet, sizeof(*tag_word));
                ++index;
                tag = (*tag_word);
                head = (*ot_word);
                *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
                head = (*ot_word);
                *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
                count = hud_control.marker_packets.count[group];
                value = (sint32)index < (sint32)count;
                packet += strides[group];
            } while (value);
            if (group == 0 || group == 2)
                buffers = hud_control.marker_packets.arena_begin;
        }
    }
    packet = hud_control.marker_packets.cur;
    tag_word = (uint32 *)mission_packet_span(packet, sizeof(*tag_word));
    head = (*ot_word);
    tag = (*tag_word);
    *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    value = (head & 0xFF000000) | (packet & 0xFFFFFF);
    *ot_word = (value);
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_markers_reset(void)
{
    uint32 index;
    MISSION_SCREEN_MARKER *state;
    FUNCTION_MARKER(0x8003F284, "ACE3.BIN");
    for (index = 0; index < 64; ++index)
    {
        state = mission_actor_marker(index);
        if (state != 0)
        {
            state->display_kind = (uint8)(0);
            state->lock_state = (uint8)(0);
            state->radius = (uint16)(0);
            state->x = (sint16)(0);
            state->y = (sint16)(0);
        }
    }
    return;
}

void hud_markers_update(void)
{
    uint16 left, right, top, bottom;
    uint32 index, flags, kind, color, a, b, c, value, shifted, sign, high, thousands, hundreds, tens, packet, buffers, count, field, glyph;
    MISSION_SCREEN_MARKER *descriptor;
    MISSION_ACTOR_TARGET *metric;
    SPRT *digits;
    FUNCTION_MARKER(0x8003F520, "ACE3.BIN");
    index = 0;
    do
    {
        flags = (*mission_actor_flags(index));
        if ((flags & 0x1000) == 0)
            goto next_descriptor;
        descriptor = mission_actor_marker(index);
        if (descriptor == 0)
            goto next_descriptor;
        kind = (descriptor->display_kind);
        if (kind == 0)
            goto next_descriptor;
        switch (kind)
        {
            case 1:
                color = 0xFF00;
                break;
            case 2:
                value = hud_control.display_ticks;
                color = (value & 2) != 0 ? 0x40FF40 : 0;
                break;
            case 3:
                color = 255;
                break;
            default:
                break;
        }
        flags = (*mission_actor_flags(index));
        if ((flags & 0x40000) != 0 && (descriptor->affiliation) != 1)
        {
            a = ((uint16)descriptor->x);
            b = (descriptor->radius);
            left = a - b;
            c = ((uint16)descriptor->y);
            right = a + b;
            top = c - 4;
            bottom = c - 4;
        }
        else
        {
            a = ((uint16)descriptor->x);
            b = (descriptor->radius);
            left = a - b;
            c = ((uint16)descriptor->y);
            right = a + b;
            top = c - b - 4;
            bottom = c + b;
        }
        kind = (descriptor->display_kind);
        if (kind != 1)
        {
            metric = mission_actor_target(index);
            if (metric == 0)
                goto next_descriptor;
            flags = (*mission_actor_flags(index));
            if ((flags & 0x400) != 0)
                hud_marker_draw_heading(index);
            value = (metric->distance);
            packet = hud_control.marker_packets.cur;
            shifted = ((sint32)value >> 2);
            sign = ((sint32)value >> 31);
            high = (((sint64)(sint32)shifted * 0x10624DD3) >> 32);
            thousands = (uint32)((sint32)high >> 6) - sign;
            buffers = hud_control.marker_packets.arena_begin;
            count = hud_control.marker_packets.count[HUD_MARKER_DIGITS];
            packet += count * 20 + 5384;
            digits = hud_control.marker_packets.native && packet - hud_control.marker_packets.cur == 5384u ? hud_control.marker_packets.native->digits : mission_packet_span(packet, 4 * sizeof(*digits));
            hud_digit_draw(thousands, (sint16)right + 2, (sint16)bottom, 2, &digits[0], 0);
            packet += 20;
            high = (((sint64)(sint32)shifted * 0x51EB851F) >> 32);
            buffers = hud_control.marker_packets.arena_begin;
            count = hud_control.marker_packets.count[HUD_MARKER_DIGITS];
            hud_control.marker_packets.count[HUD_MARKER_DIGITS] = count + 1;
            hundreds = (uint32)((sint32)high >> 5) - sign;
            glyph = hundreds - thousands * 10;
            hud_digit_draw(glyph, (sint16)right + 7, (sint16)bottom, 2, &digits[1], 0);
            packet += 20;
            high = (((sint64)(sint32)shifted * 0x66666667) >> 32);
            buffers = hud_control.marker_packets.arena_begin;
            count = hud_control.marker_packets.count[HUD_MARKER_DIGITS];
            hud_control.marker_packets.count[HUD_MARKER_DIGITS] = count + 1;
            tens = (uint32)((sint32)high >> 2) - sign;
            glyph = tens - hundreds * 10;
            hud_digit_draw(glyph, (sint16)right + 12, (sint16)bottom, 2, &digits[2], 0);
            packet += 20;
            glyph = shifted - tens * 10;
            buffers = hud_control.marker_packets.arena_begin;
            count = hud_control.marker_packets.count[HUD_MARKER_DIGITS];
            hud_control.marker_packets.count[HUD_MARKER_DIGITS] = count + 1;
            hud_digit_draw(glyph, (sint16)right + 17, (sint16)bottom, 2, &digits[3], 0);
            buffers = hud_control.marker_packets.arena_begin;
            count = hud_control.marker_packets.count[HUD_MARKER_DIGITS];
            hud_control.marker_packets.count[HUD_MARKER_DIGITS] = count + 1;
            field = (descriptor->actor_kind);
            hud_marker_draw_kind(field, (sint16)left, (sint16)top);
        }
        field = (descriptor->label);
        if (field != 255)
            hud_marker_draw_label(field, (sint16)right, (sint16)top);
        field = (descriptor->affiliation);
        if (field == 1)
            hud_marker_draw_affiliation(1, (sint16)left, (sint16)bottom);
        else if (field == 2)
        {
            field = (descriptor->affiliation);
            hud_marker_draw_affiliation(field, (sint16)left, (sint16)bottom);
        }
        else if (field == 3)
        {
            color = 0xFF4000;
            field = (descriptor->actor_kind);
            hud_marker_draw_kind(field, (sint16)left, (sint16)top);
            field = (descriptor->affiliation);
            hud_marker_draw_affiliation(field, (sint16)left, (sint16)bottom);
        }
        flags = (*mission_actor_flags(index));
        if ((flags & 0x40000) != 0 && (descriptor->display_kind) == 1)
        {
            field = (descriptor->actor_kind);
            hud_marker_draw_kind(field, (sint16)left, (sint16)top);
        }
        else
        {
            flags = game_runtime.metrics.flags;
            if ((flags & 4) != 0)
                hud_marker_draw_line_frame(index, color);
            else
            {
                field = (descriptor->shape_flags) & 15;
                if (field == 1)
                    hud_marker_draw_corners(index, color);
                else if (field == 2)
                    hud_marker_draw_outline(index, color);
                else if (field == 3)
                    hud_marker_draw_line_frame(index, color);
            }
        }
    next_descriptor:
        index += 1;
    } while (index < 64);
    return;
}

void hud_lock_init(void)
{
    FUNCTION_MARKER(0x800409AC, "ACE3.BIN");
    hud_control.lock.x = (uint16)(160);
    hud_control.lock.y = (uint16)(120);
    hud_control.lock.mode = (uint8)(0);
    hud_control.lock.target_idx = (uint8)(0);
    hud_control.lock.radius = (uint16)(64);
    return;
}

void hud_lock_draw(void)
{
    uint32 size, x, y, selected, radius, inner, buffers, count, packet, i, value;
    uint32 left, right, near_left, near_right, top, bottom, near_top, near_bottom;
    LINE_F2 *line;
    LINE_F3 *polyline;
    FUNCTION_MARKER(0x80040E70, "ACE3.BIN");
    if (hud_control.lock.mode == 0)
        return;
    size = hud_control.lock.radius;
    x = (sint16)hud_control.lock.x;
    y = (sint16)hud_control.lock.y;
    selected = hud_control.lock.target_idx;
    radius = (sint16)size;
    if (selected < 32)
    {
        radius += (sint32)(size << 16) >> 19;
        top = y - radius;
        bottom = y + radius;
        buffers = hud_control.marker_packets.arena_begin;
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
        packet = hud_control.marker_packets.cur + count * 24 + 0x408;
        for (i = 0; i < 2; ++i)
        {
            polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
            polyline->rgbc.r = (uint8)(255);
            polyline->rgbc.g = (uint8)(0);
            polyline->rgbc.b = (uint8)(0);
            buffers = hud_control.marker_packets.arena_begin;
            value = i == 0 ? x + radius : x - radius;
            polyline->p[0].x = (sint16)(uint16)(x);
            polyline->p[0].y = (sint16)(uint16)(top);
            polyline->p[1].x = (sint16)(uint16)(value);
            polyline->p[1].y = (sint16)(uint16)(y);
            polyline->p[2].x = (sint16)(uint16)(x);
            polyline->p[2].y = (sint16)(uint16)(bottom);
            count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
            packet += 24;
            hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
        }
        {
            (void)(uint32)(count);
            return;
        }
    }
    inner = (sint32)(size << 16) >> 18;
    top = y - radius;
    bottom = y + radius;
    near_top = y - inner;
    near_bottom = y + inner;
    left = x - radius;
    right = x + radius;
    near_left = x - inner;
    near_right = x + inner;
    buffers = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    packet = hud_control.marker_packets.cur + (count << 4) + 8;
    for (i = 0; i < 8; ++i)
    {
        line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        line->rgbc.r = (uint8)(255);
        line->rgbc.g = (uint8)(0);
        line->rgbc.b = (uint8)(0);
        if (i != 6)
            buffers = hud_control.marker_packets.arena_begin;
        if (i == 0 || i == 1)
        {
            line->p[1].x = (sint16)(uint16)(x);
            line->p[0].x = (sint16)(uint16)(x);
            line->p[0].y = (sint16)(uint16)(i == 0 ? top : bottom);
            line->p[1].y = (sint16)(uint16)(i == 0 ? near_top : near_bottom);
        }
        else if (i == 2 || i == 3)
        {
            line->p[0].x = (sint16)(uint16)(i == 2 ? left : right);
            line->p[1].x = (sint16)(uint16)(i == 2 ? near_left : near_right);
            line->p[1].y = (sint16)(uint16)(y);
            line->p[0].y = (sint16)(uint16)(y);
        }
        else if (i == 4 || i == 5)
        {
            line->p[0].x = (sint16)(uint16)(near_left);
            line->p[1].x = (sint16)(uint16)(near_right);
            line->p[1].y = (sint16)(uint16)(i == 4 ? top : bottom);
            line->p[0].y = (sint16)(uint16)(i == 4 ? top : bottom);
        }
        else
        {
            value = i == 6 ? left : right;
            line->p[1].x = (sint16)(uint16)(value);
            line->p[0].x = (sint16)(uint16)(value);
            if (i == 6)
                buffers = hud_control.marker_packets.arena_begin;
            line->p[0].y = (sint16)(uint16)(near_top);
            line->p[1].y = (sint16)(uint16)(near_bottom);
        }
        count = hud_control.marker_packets.count[HUD_MARKER_LINES] + 1;
        packet += 16;
        hud_control.marker_packets.count[HUD_MARKER_LINES] = count;
    }
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_lock_update(void)
{
    uint32 mode, enabled;
    FUNCTION_MARKER(0x8003EF40, "ACE3.BIN");
    mode = game_runtime.selection.weapon_code[1];
    enabled = (mode - 5 < 2) || mode == 9;
    if (enabled == 0 && mission_target_uses_alternate_weapon() == 1)
    {
        mode = game_runtime.selection.weapon_code[2];
        enabled = (mode - 5 < 2) || mode == 9;
    }
    hud_control.weapon_lock_mode = enabled;
    hud_markers_classify();
    {
        (void)(hud_lock_animate());
        return;
    }
}

void hud_markers_classify(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, value;
    uint32 *flags;
    MISSION_SCREEN_MARKER *record;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x8003F2CC, "ACE3.BIN");
    hud_control.locked_target_idx = 255;
    for (index = 2; index < 64; ++index)
    {
        record = mission_actor_marker(index);
        target = mission_actor_target(index);
        if (record == 0)
            continue;
        record->display_kind = (uint8)(0);
        if (target == 0)
            continue;
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((actor_traits_object->flags & 0x4000) == 0)
            continue;
        flags = mission_actor_flags(index);
        if (((*flags) & 0x1000) == 0 || (record->shape_flags) == 0)
            continue;
        if (((target->projection_flags) & 0x20000) == 0 || ((target->flags) & 1) == 0)
            continue;
        record->display_kind = (uint8)(1);
        value = hud_control.selected_target_idx;
        if (value != index)
        {
            if ((record->lock_state) == 0 || hud_control.lock.target_idx != index)
                continue;
            value = (record->lock_state);
            value = hud_lock_cancel();
            record->lock_state = (uint8)(value);
            continue;
        }
        record->display_kind = (uint8)(2);
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if (actor_traits_object->flags & 0x40)
            continue;
        if (((target->flags) & 8) == 0 || ((*flags) & 0x4000) == 0)
            continue;
        if ((game_runtime.metrics.flags & 4) || hud_control.weapon_lock_mode)
            continue;
        value = (record->lock_state);
        if (value == 0 || hud_control.lock.target_idx != index)
        {
            value = hud_lock_begin(index);
            record->lock_state = (uint8)(value);
            continue;
        }
        value = hud_control.lock.mode;
        if (value == 2)
        {
            record->display_kind = (uint8)(3);
            hud_control.locked_target_idx = index;
        }
        else if (value == 0)
            record->lock_state = (uint8)(0);
    }
    return;
}

uint32 hud_lock_begin(uint32 index)
{
    FUNCTION_MARKER(0x800409D4, "ACE3.BIN");
    if (hud_control.lock.mode)
        return 0;
    hud_control.lock.mode = (uint8)(1);
    hud_control.lock.x = (uint16)(160);
    hud_control.lock.y = (uint16)(120);
    hud_control.lock.target_idx = (uint8)(index);
    hud_control.lock.radius = (uint16)(2);
    return 1;
}

uint32 hud_lock_cancel(void)
{
    FUNCTION_MARKER(0x80040A18, "ACE3.BIN");
    hud_control.lock.mode = (uint8)(0);
    return 255;
}

void hud_lock_animate(void)
{
    const MISSION_ACTOR_DEFINITION *definition_data;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, active, value, mode, flags;
    uint32 a, b, c, d, dx, dy, ds, step, raw, result;
    MISSION_SCREEN_MARKER *record;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80040A24, "ACE3.BIN");
    index = hud_control.lock.target_idx;
    record = mission_actor_marker(index);
    if (record == 0)
        return;
    active = hud_control.lock.mode;
    if (active == 0)
        return;
    if ((record->display_kind) == 0)
    {
        hud_lock_cancel();
        return;
    }
    flags = (*mission_actor_flags(index));
    if ((flags & 1) == 0)
    {
        hud_control.lock.mode = (uint8)(0);
        return;
    }
    target = mission_actor_target(index);
    if (target == 0)
        return;
    if (((target->flags) & 8) == 0)
    {
        hud_control.lock.mode = (uint8)(0);
        return;
    }
    if (active == 1)
    {
        if ((game_runtime.mission_control.ticks & 7) == 0)
            sound_play_reuse(45);
    }
    mode = hud_control.lock.mode;
    if (mode == 2)
    {
        if ((game_runtime.mission_control.ticks & 7) == 0)
            sound_play_reuse(50);
        if (hud_control.lock.mode == mode)
        {
            value = ((uint16)record->x);
            hud_control.lock.x = (uint16)(value);
            value = ((uint16)record->y);
            hud_control.lock.y = (uint16)(value);
            value = (record->radius);
            hud_control.lock.radius = (uint16)(value);
            return;
        }
    }
    a = (record->y);
    b = (sint16)hud_control.lock.y;
    c = (record->x);
    d = ((sint16)record->radius);
    dy = a - b;
    a = (sint16)hud_control.lock.radius;
    b = (sint16)hud_control.lock.x;
    dx = c - b;
    ds = d - a;
    if (dx == 0 && dy == 0 && ds == 0)
        goto arrived;
    step = (sint16)hud_control.lock.radius;
    raw = hud_control.lock.radius;
    value = hud_control.lock.x;
    if ((sint32)step < (sint32)dx)
        value += raw;
    else if ((sint32)dx < (sint32)(0 - step))
        value -= raw;
    else
        value += dx;
    hud_control.lock.x = (uint16)(value);
    step = (sint16)hud_control.lock.radius;
    raw = hud_control.lock.radius;
    value = hud_control.lock.y;
    if ((sint32)step < (sint32)dy)
        value += raw;
    else if ((sint32)dy < (sint32)(0 - step))
        value -= raw;
    else
        value += dy;
    hud_control.lock.y = (uint16)(value);
    value = hud_control.lock.radius;
    if ((sint32)ds >= 5)
        value += 4;
    else if ((sint32)ds < -4)
        value -= 4;
    else
        value += ds;
    hud_control.lock.radius = (uint16)(value);
    a = (record->y);
    b = (sint16)hud_control.lock.y;
    c = (record->x);
    d = ((sint16)record->radius);
    dy = a - b;
    result = (sint16)hud_control.lock.radius;
    value = (sint16)hud_control.lock.x;
    ds = d - result;
    if (c != value || dy != 0 || ds != 0)
    {
        (void)(uint32)(result);
        return;
    }
arrived:
    if (hud_control.lock.mode != 2)
    {
        mode = (sint16)game_runtime.selection.mission;
        if (mode == 4 || mode == 8 || mode == 15 || mode == 30)
        {
            index = hud_control.lock.target_idx;
            actor_index_table = 0;
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if (actor_traits_object->flags & 0x100)
                mission_status_text_show(14);
        }
    }
    index = hud_control.lock.target_idx;
    flags = (*mission_actor_flags(index));
    if (flags & 0x800000)
    {
        definition_data = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
        value = definition_data->label;
        mission_prompt_request(8, 0, value, 0);
    }
    hud_control.lock.mode = (uint8)(2);
    return;
}

void hud_marker_draw_heading(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 width, angle, sine, cosine, half, offset_x, offset_y, value, i, x, y, a, b;
    uint32 state, count, base, packet;
    uint16 points[6];
    MISSION_SCREEN_MARKER *record;
    LINE_F2 *line;
    FUNCTION_MARKER(0x80040530, "ACE3.BIN");
    record = mission_actor_marker(index);
    if (record == 0)
        return;
    actor_index_table = 0;
    width = (record->radius);
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    a = pose_owner_object->pose.angles[1];
    b = game_runtime.camera_view.pose.angles[1];
    angle = (a - b) & 0x3FFF;
    sine = game_wave_at((angle << 1));
    cosine = game_wave_at(((angle + 0x1000) << 1));
    half = (sint16)width >> 1;
    offset_x = ((sint32)((cosine - sine) * half) >> 14);
    points[0] = 0;
    value = (record->radius);
    points[2] = 4;
    points[1] = value - 4;
    value = (record->radius);
    points[4] = 0xFFFC;
    points[3] = value + 4;
    value = (record->radius);
    points[5] = value + 4;
    offset_y = ((sint32)((sine + cosine) * half) >> 14);
    for (i = 0; i < 3; ++i)
    {
        x = (sint16)points[i * 2];
        y = (sint16)points[i * 2 + 1];
        a = cosine * x - sine * y;
        b = sine * x + cosine * y;
        value = ((uint16)record->x);
        points[i * 2] = value + (uint32)((sint32)a >> 14) + offset_x;
        value = ((uint16)record->y);
        points[i * 2 + 1] = value + (uint32)((sint32)b >> 14) + offset_y;
    }
    state = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    base = hud_control.marker_packets.cur;
    packet = base + 8 + (count << 4);
    line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(255);
    line->rgbc.b = (uint8)(0);
    state = hud_control.marker_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(points[0]);
    line->p[0].y = (sint16)(uint16)(points[1]);
    line->p[1].x = (sint16)(uint16)(points[2]);
    line->p[1].y = (sint16)(uint16)(points[3]);
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    packet += 16;
    hud_control.marker_packets.count[HUD_MARKER_LINES] = count + 1;
    line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
    line->rgbc.r = (uint8)(0);
    line->rgbc.g = (uint8)(255);
    line->rgbc.b = (uint8)(0);
    state = hud_control.marker_packets.arena_begin;
    line->p[0].x = (sint16)(uint16)(points[0]);
    line->p[0].y = (sint16)(uint16)(points[1]);
    line->p[1].x = (sint16)(uint16)(points[4]);
    line->p[1].y = (sint16)(uint16)(points[5]);
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    hud_control.marker_packets.count[HUD_MARKER_LINES] = count + 1;
    return;
}

void hud_marker_draw_corners(uint32 index, uint32 color)
{
    uint32 width, a, b, flags, quarter, state, count, base, packet, i, x, y, value;
    uint16 left, right, top, bottom;
    MISSION_SCREEN_MARKER *record;
    MISSION_ACTOR_TARGET *object;
    LINE_F3 *polyline;
    FUNCTION_MARKER(0x8003FA80, "ACE3.BIN");
    record = mission_actor_marker(index);
    color |= 0x4A000000;
    if (record == 0)
        return;
    object = mission_actor_target(index);
    if (object == 0)
        return;
    width = (record->radius);
    a = (record->x);
    left = a - (uint32)(sint32)(sint16)width;
    a = (record->x);
    b = ((sint16)record->radius);
    right = a + b;
    a = (record->y);
    b = ((sint16)record->radius);
    top = a - b;
    a = (record->y);
    b = ((sint16)record->radius);
    bottom = a + b;
    flags = (object->flags) & 0x100;
    quarter = (sint16)width >> 2;
    state = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    base = hud_control.marker_packets.cur;
    packet = base + 0x408 + count * 24;
    if (flags != 0)
    {
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[1].x = (sint16)(uint16)(left);
        polyline->p[0].x = (sint16)(uint16)(left);
        polyline->p[2].x = (sint16)(uint16)(right);
        polyline->p[0].y = (sint16)(uint16)(bottom);
        polyline->p[2].y = (sint16)(uint16)(top);
        polyline->p[1].y = (sint16)(uint16)(top);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
        packet += 24;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[1].x = (sint16)(uint16)(right);
        polyline->p[0].x = (sint16)(uint16)(right);
        polyline->p[2].x = (sint16)(uint16)(left);
        polyline->p[0].y = (sint16)(uint16)(top);
        polyline->p[2].y = (sint16)(uint16)(bottom);
        polyline->p[1].y = (sint16)(uint16)(bottom);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
        {
            (void)(uint32)(count);
            return;
        }
    }
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[2].x = (sint16)(uint16)(x);
        polyline->p[1].x = (sint16)(uint16)(x);
        value = (i & 1) ? x - quarter : x + quarter;
        polyline->p[0].x = (sint16)(uint16)(value);
        polyline->p[2].y = (sint16)(uint16)(y);
        polyline->p[1].y = (sint16)(uint16)(y);
        polyline->p[0].y = (sint16)(uint16)(y);
        value = i < 2 ? y + quarter : y - quarter;
        polyline->p[2].y = (sint16)(uint16)(value);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
        if (i != 3)
            packet += 24;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
    }
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_marker_draw_outline(uint32 index, uint32 color)
{
    uint32 width, a, b, quarter, eighth, stretch, flags, state, count, base, packet, value;
    uint16 left, middle, right, top, upper, lower, bottom;
    MISSION_SCREEN_MARKER *record;
    MISSION_ACTOR_TARGET *object;
    LINE_F3 *polyline;
    FUNCTION_MARKER(0x8003FD48, "ACE3.BIN");
    record = mission_actor_marker(index);
    color |= 0x4A000000;
    if (record == 0)
        return;
    object = mission_actor_target(index);
    if (object == 0)
        return;
    width = (record->radius);
    a = (record->x);
    left = a - (uint32)(sint32)(sint16)width;
    middle = (record->x);
    a = (record->x);
    b = ((sint16)record->radius);
    right = a + b;
    a = (record->y);
    b = ((sint16)record->radius);
    top = a - b;
    quarter = (sint16)width >> 2;
    a = (record->radius);
    b = (record->y);
    upper = b - (uint32)((sint16)a >> 1);
    a = (record->radius);
    b = (record->y);
    lower = b + (uint32)((sint16)a >> 1);
    a = (record->y);
    b = ((sint16)record->radius);
    bottom = a + b;
    eighth = (sint16)width >> 3;
    flags = (object->flags) & 0x100;
    stretch = (sint32)((eighth << 3) + eighth) >> 2;
    state = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    base = hud_control.marker_packets.cur;
    packet = base + 0x408 + count * 24;
    if (flags != 0)
    {
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[0].x = (sint16)(uint16)(left);
        polyline->p[1].x = (sint16)(uint16)(middle);
        polyline->p[2].x = (sint16)(uint16)(right);
        polyline->p[0].y = (sint16)(uint16)(upper);
        polyline->p[1].y = (sint16)(uint16)(top);
        polyline->p[2].y = (sint16)(uint16)(upper);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
        packet += 24;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[1].x = (sint16)(uint16)(left);
        polyline->p[0].x = (sint16)(uint16)(left);
        polyline->p[2].x = (sint16)(uint16)(middle);
        polyline->p[0].y = (sint16)(uint16)(upper);
        polyline->p[1].y = (sint16)(uint16)(lower);
        polyline->p[2].y = (sint16)(uint16)(bottom);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
        packet += 24;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
        polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
        polyline->rgbc.rgbc = (uint32)(color);
        polyline->p[1].x = (sint16)(uint16)(right);
        polyline->p[0].x = (sint16)(uint16)(right);
        polyline->p[2].x = (sint16)(uint16)(middle);
        polyline->p[0].y = (sint16)(uint16)(upper);
        polyline->p[1].y = (sint16)(uint16)(lower);
        polyline->p[2].y = (sint16)(uint16)(bottom);
        count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
        hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
        {
            (void)(uint32)(count);
            return;
        }
    }
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(middle);
    polyline->p[1].x = (sint16)(uint16)(middle);
    polyline->p[0].x = (sint16)(uint16)(middle - quarter);
    polyline->p[2].x = (sint16)(uint16)(middle + quarter);
    polyline->p[1].y = (sint16)(uint16)(top);
    value = top + eighth;
    polyline->p[2].y = (sint16)(uint16)(value);
    polyline->p[0].y = (sint16)(uint16)(value);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    packet += 24;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(left);
    polyline->p[1].x = (sint16)(uint16)(left);
    polyline->p[0].x = (sint16)(uint16)(left);
    polyline->p[2].x = (sint16)(uint16)(left + quarter);
    polyline->p[2].y = (sint16)(uint16)(upper);
    polyline->p[1].y = (sint16)(uint16)(upper);
    polyline->p[0].y = (sint16)(uint16)(upper + stretch);
    polyline->p[2].y = (sint16)(uint16)(upper - eighth);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    packet += 24;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(left);
    polyline->p[1].x = (sint16)(uint16)(left);
    polyline->p[0].x = (sint16)(uint16)(left);
    polyline->p[2].x = (sint16)(uint16)(left + quarter);
    polyline->p[2].y = (sint16)(uint16)(lower);
    polyline->p[1].y = (sint16)(uint16)(lower);
    polyline->p[0].y = (sint16)(uint16)(lower - stretch);
    polyline->p[2].y = (sint16)(uint16)(lower + eighth);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
    packet = base + 0x408 + count * 24;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(middle);
    polyline->p[1].x = (sint16)(uint16)(middle);
    polyline->p[0].x = (sint16)(uint16)(middle - quarter);
    polyline->p[2].x = (sint16)(uint16)(middle + quarter);
    polyline->p[1].y = (sint16)(uint16)(bottom);
    value = bottom - eighth;
    polyline->p[2].y = (sint16)(uint16)(value);
    polyline->p[0].y = (sint16)(uint16)(value);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    packet += 24;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(right);
    polyline->p[1].x = (sint16)(uint16)(right);
    polyline->p[0].x = (sint16)(uint16)(right);
    polyline->p[2].x = (sint16)(uint16)(right - quarter);
    polyline->p[2].y = (sint16)(uint16)(upper);
    polyline->p[1].y = (sint16)(uint16)(upper);
    polyline->p[0].y = (sint16)(uint16)(upper + stretch);
    polyline->p[2].y = (sint16)(uint16)(upper - eighth);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES];
    packet += 24;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count + 1;
    polyline = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 1032u) < sizeof(hud_control.marker_packets.native->polylines) && (packet - hud_control.marker_packets.cur - 1032u) % sizeof(LINE_F3) == 0 ? &hud_control.marker_packets.native->polylines[(packet - hud_control.marker_packets.cur - 1032u) / sizeof(LINE_F3)] : (LINE_F3 *)mission_packet_span(packet, sizeof(*polyline));
    polyline->rgbc.rgbc = (uint32)(color);
    polyline->p[2].x = (sint16)(uint16)(right);
    polyline->p[1].x = (sint16)(uint16)(right);
    polyline->p[0].x = (sint16)(uint16)(right);
    polyline->p[2].x = (sint16)(uint16)(right - quarter);
    polyline->p[2].y = (sint16)(uint16)(lower);
    polyline->p[1].y = (sint16)(uint16)(lower);
    polyline->p[0].y = (sint16)(uint16)(lower - stretch);
    polyline->p[2].y = (sint16)(uint16)(lower + eighth);
    count = hud_control.marker_packets.count[HUD_MARKER_POLYLINES] + 1;
    hud_control.marker_packets.count[HUD_MARKER_POLYLINES] = count;
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_marker_draw_line_frame(uint32 index, uint32 color)
{
    uint32 width, a, b, state, count, base, packet, quarter, eighth, flags, i, x, y, value;
    uint16 left, right, top, bottom;
    MISSION_SCREEN_MARKER *record;
    MISSION_ACTOR_TARGET *object;
    LINE_F2 *line;
    FUNCTION_MARKER(0x800401AC, "ACE3.BIN");
    record = mission_actor_marker(index);
    color |= 0x42000000;
    if (record == 0)
        return;
    object = mission_actor_target(index);
    if (object == 0)
        return;
    width = (record->radius);
    a = (record->x);
    left = a - (uint32)(sint32)(sint16)width;
    a = (record->x);
    b = ((sint16)record->radius);
    right = a + b;
    a = (record->y);
    b = ((sint16)record->radius);
    top = a - b;
    a = (record->y);
    b = ((sint16)record->radius);
    state = hud_control.marker_packets.arena_begin;
    bottom = a + b;
    count = hud_control.marker_packets.count[HUD_MARKER_LINES];
    base = hud_control.marker_packets.cur;
    packet = base + 8 + (count << 4);
    quarter = (sint16)width >> 2;
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        line->rgbc.rgbc = (uint32)(color);
        line->p[0].x = (sint16)(uint16)(x);
        value = (i & 1) ? x - quarter : x + quarter;
        line->p[1].x = (sint16)(uint16)(value);
        line->p[0].y = (sint16)(uint16)(y);
        value = i < 2 ? y + quarter : y - quarter;
        line->p[1].y = (sint16)(uint16)(value);
        count = hud_control.marker_packets.count[HUD_MARKER_LINES] + 1;
        if (i != 3)
            packet += 16;
        hud_control.marker_packets.count[HUD_MARKER_LINES] = count;
    }
    flags = (object->flags) & 0x20;
    packet += 16;
    if (flags == 0)
    {
        (void)(uint32)(flags);
        return;
    }
    width = (record->radius);
    eighth = (sint16)width >> 3;
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        line->rgbc.rgbc = (uint32)(color);
        line->p[0].x = (sint16)(uint16)(x - eighth);
        line->p[1].x = (sint16)(uint16)(x + eighth);
        value = i == 0 || i == 3 ? y + eighth : y - eighth;
        line->p[0].y = (sint16)(uint16)(value);
        value = i == 0 || i == 3 ? y - eighth : y + eighth;
        line->p[1].y = (sint16)(uint16)(value);
        count = hud_control.marker_packets.count[HUD_MARKER_LINES];
        packet += 16;
        hud_control.marker_packets.count[HUD_MARKER_LINES] = count + 1;
    }
    line = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 8u) < sizeof(hud_control.marker_packets.native->lines) && (packet - hud_control.marker_packets.cur - 8u) % sizeof(LINE_F2) == 0 ? &hud_control.marker_packets.native->lines[(packet - hud_control.marker_packets.cur - 8u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
    line->rgbc.rgbc = (uint32)(color);
    value = ((uint32)(uint16)record->x | ((uint32)(uint16)record->y << 16));
    line->p[0].xy = (uint32)(value);
    value = ((uint32)(uint16)record->x | ((uint32)(uint16)record->y << 16));
    line->p[1].xy = (uint32)(value);
    count = hud_control.marker_packets.count[HUD_MARKER_LINES] + 1;
    hud_control.marker_packets.count[HUD_MARKER_LINES] = count;
    {
        (void)(uint32)(count);
        return;
    }
}

typedef struct
{
    uint8 u, v, w, h;
} HUD_MARKER_ICON;

static const HUD_MARKER_ICON marker_affiliations[4] = {
    {0, 0, 0, 0},
    {194, 54, 14, 6},
    {236, 54, 20, 6},
    {136, 114, 18, 6},
};

void hud_marker_draw_affiliation(uint32 kind, uint32 x, uint32 y)
{
    uint32 state, count, base, packet, value;
    const HUD_MARKER_ICON *source;
    SPRT *spr;
    FUNCTION_MARKER(0x80040740, "ACE3.BIN");
    kind &= 255;
    if (kind == 0)
        return;
    source = &marker_affiliations[kind];
    state = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS];
    base = hud_control.marker_packets.cur;
    value = source->w;
    packet = base + 0x1008 + count * 20;
    spr = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 4104u) < sizeof(hud_control.marker_packets.native->labels) && (packet - hud_control.marker_packets.cur - 4104u) % sizeof(SPRT) == 0 ? &hud_control.marker_packets.native->labels[(packet - hud_control.marker_packets.cur - 4104u) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(*spr));
    spr->p[0].xy.y = (sint16)(uint16)(y);
    spr->p[0].xy.x = (sint16)(uint16)(x - value - 2);
    value = source->u;
    spr->p[0].uv.u = (uint8)(value);
    value = source->v;
    spr->p[0].uv.v = (uint8)(value);
    value = source->w;
    spr->w = (sint16)(uint16)(value);
    value = source->h;
    spr->h = (sint16)(uint16)(value);
    if (kind == 1)
    {
        spr->rgbc.r = (uint8)(0x83);
        spr->rgbc.g = (uint8)(0);
        spr->rgbc.b = (uint8)(0);
    }
    else if (kind == 2)
    {
        spr->rgbc.r = (uint8)(0x83);
        spr->rgbc.g = (uint8)(0x83);
        spr->rgbc.b = (uint8)(0);
    }
    else if (kind == 3)
    {
        spr->rgbc.g = (uint8)(0x23);
        spr->rgbc.r = (uint8)(0);
        spr->rgbc.b = (uint8)(0x83);
    }
    state = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS];
    hud_control.marker_packets.count[HUD_MARKER_LABELS] = count + 1;
}

enum
{
    MARKER_LABEL_START = 86
};

/* both byte-indexed views share the original consecutive records */
static const HUD_MARKER_ICON marker_icon_records[342] = {
    {0, 0, 0, 0},     {186, 84, 26, 6}, {136, 84, 24, 6}, {232, 102, 24, 6}, {136, 96, 34, 6}, {170, 102, 34, 6}, {222, 78, 18, 6},     {164, 90, 26, 6}, {170, 96, 30, 6}, {204, 102, 28, 6}, {136, 102, 34, 6},    {136, 60, 22, 6}, {190, 90, 26, 6}, {136, 108, 28, 6}, {160, 72, 24, 6},   {136, 78, 24, 6}, {204, 72, 18, 6}, {204, 78, 18, 6},     {184, 72, 20, 6},  {156, 66, 20, 6},  {184, 78, 20, 6},   {176, 66, 20, 6},    {136, 66, 20, 6}, {196, 66, 20, 6},  {216, 66, 20, 6},   {160, 84, 26, 6},    {240, 78, 16, 6}, {136, 90, 28, 6}, {170, 96, 30, 6}, {204, 78, 18, 6}, {136, 66, 20, 6}, {232, 60, 24, 6}, {204, 102, 28, 6}, {222, 72, 18, 6}, {178, 60, 14, 6},  {136, 72, 24, 6}, {216, 90, 20, 6}, {160, 78, 24, 6}, {212, 84, 24, 6}, {216, 66, 20, 6}, {236, 66, 20, 6}, {158, 60, 20, 6}, {236, 90, 20, 6}, {236, 84, 20, 6}, {200, 96, 28, 6}, {228, 96, 28, 6}, {212, 60, 20, 6}, {154, 114, 16, 6}, {242, 48, 14, 6},
    {216, 42, 20, 6}, {210, 12, 24, 6}, {136, 18, 42, 6}, {214, 24, 42, 6},  {136, 36, 32, 6}, {168, 36, 30, 6},  {196, 42, 20, 6},     {178, 12, 32, 6}, {226, 36, 30, 6}, {214, 30, 42, 6},  {136, 24, 44, 6},     {136, 30, 44, 6}, {182, 18, 38, 6}, {180, 30, 34, 6},  {236, 42, 20, 6},   {208, 54, 14, 6}, {208, 48, 34, 6}, {168, 42, 28, 6},     {136, 42, 32, 6},  {172, 48, 36, 6},  {198, 36, 24, 6},   {210, 6, 30, 6},     {136, 6, 42, 6},  {136, 48, 36, 6},  {136, 12, 42, 6},   {154, 54, 20, 6},    {136, 54, 18, 6}, {220, 18, 36, 6}, {178, 6, 32, 6},  {240, 72, 16, 6}, {180, 24, 34, 6}, {174, 54, 20, 6}, {164, 108, 38, 6}, {234, 12, 22, 6}, {202, 108, 40, 6}, {0, 0, 0, 0},     {0, 0, 0, 0},     {0, 0, 0, 0},     {162, 0, 20, 6},  {218, 0, 18, 6},  {182, 0, 18, 6},  {240, 6, 16, 6},  {200, 0, 18, 6},  {136, 0, 26, 6},  {236, 0, 20, 6},  {18, 0, 90, 0},   {94, 0, 80, 0},   {2, 16, 3, 64},    {66, 0, 0, 0},
    {16, 0, 45, 0},   {94, 0, 80, 0},   {0, 28, 255, 0},  {64, 0, 0, 0},     {17, 0, 45, 0},   {94, 0, 80, 0},    {1, 28, 255, 0},      {65, 0, 0, 0},    {32, 0, 90, 0},   {94, 0, 192, 0},   {8, 12, 9, 58},       {69, 0, 30, 0},   {33, 1, 45, 0},   {94, 0, 192, 0},   {11, 16, 12, 50},   {71, 0, 30, 0},   {33, 1, 45, 0},   {94, 0, 192, 0},      {4, 20, 10, 68},   {70, 0, 30, 0},    {32, 1, 90, 0},     {94, 0, 192, 0},     {7, 38, 255, 0},  {68, 0, 30, 0},    {33, 1, 45, 0},     {94, 0, 192, 0},     {4, 30, 5, 78},   {67, 0, 30, 0},   {32, 1, 90, 0},   {94, 0, 192, 0},  {2, 14, 13, 62},  {72, 0, 30, 0},   {32, 1, 90, 0},    {94, 0, 192, 0},  {29, 10, 3, 67},   {100, 0, 30, 0},  {0, 2, 10, 0},    {94, 0, 80, 0},   {25, 23, 255, 0}, {0, 0, 255, 255}, {1, 2, 16, 0},    {94, 0, 80, 0},   {21, 4, 22, 73},  {0, 0, 255, 255}, {0, 1, 45, 0},    {94, 0, 80, 0},   {14, 12, 255, 0}, {111, 0, 0, 0},    {0, 1, 45, 0},
    {94, 0, 80, 0},   {15, 28, 11, 74}, {0, 0, 255, 255}, {0, 1, 45, 0},     {94, 0, 80, 0},   {16, 20, 10, 70},  {113, 0, 0, 0},       {0, 1, 180, 0},   {94, 0, 80, 0},   {17, 14, 10, 74},  {58, 0, 0, 0},        {0, 1, 45, 0},    {94, 0, 80, 0},   {18, 6, 19, 90},   {114, 0, 255, 255}, {0, 1, 45, 0},    {94, 0, 80, 0},   {20, 28, 255, 0},     {55, 0, 255, 255}, {1, 2, 16, 0},     {94, 0, 80, 0},     {21, 4, 22, 73},     {0, 0, 255, 255}, {1, 2, 16, 0},     {94, 0, 80, 0},     {21, 11, 23, 80},    {0, 0, 255, 255}, {1, 2, 16, 0},    {94, 0, 80, 0},   {21, 1, 24, 67},  {0, 0, 255, 255}, {0, 2, 180, 0},   {94, 0, 80, 0},    {25, 23, 255, 0}, {55, 0, 0, 0},     {0, 1, 45, 0},    {94, 0, 80, 0},   {4, 30, 5, 78},   {67, 0, 0, 0},    {0, 1, 45, 0},    {94, 0, 80, 0},   {4, 20, 10, 68},  {70, 0, 0, 0},    {0, 1, 45, 0},    {94, 0, 80, 0},   {26, 16, 27, 72}, {73, 0, 0, 0},    {0, 1, 45, 0},     {94, 0, 80, 0},
    {28, 20, 27, 68}, {74, 0, 0, 0},    {0, 2, 16, 0},    {94, 0, 80, 0},    {21, 4, 22, 73},  {0, 0, 255, 255},  {0, 2, 16, 0},        {94, 0, 80, 0},   {21, 11, 23, 80}, {0, 0, 255, 255},  {0, 2, 16, 0},        {94, 0, 80, 0},   {21, 1, 24, 67},  {0, 0, 255, 255},  {0, 20, 75, 6},     {0, 26, 72, 6},   {0, 32, 45, 6},   {0, 38, 54, 6},       {46, 32, 43, 6},   {110, 44, 23, 6},  {72, 26, 61, 6},    {0, 68, 53, 6},      {90, 32, 43, 6},  {0, 44, 66, 6},    {66, 44, 44, 6},    {104, 56, 31, 6},    {0, 74, 64, 6},   {76, 20, 55, 6},  {0, 56, 104, 6},  {94, 62, 41, 6},  {50, 62, 43, 6},  {76, 50, 54, 6},  {54, 38, 82, 6},   {98, 68, 37, 6},  {0, 50, 75, 6},    {0, 80, 65, 6},   {66, 80, 54, 6},  {0, 86, 40, 6},   {0, 92, 63, 6},   {42, 86, 88, 6},  {0, 62, 50, 6},   {54, 68, 43, 6},  {64, 74, 43, 6},  {64, 92, 53, 6},  {90, 0, 2, 3},    {90, 0, 0, 255},  {90, 0, 1, 255},  {90, 0, 2, 4},     {90, 0, 2, 5},
    {90, 0, 2, 6},    {90, 0, 2, 7},    {90, 0, 2, 8},    {90, 0, 2, 9},     {10, 0, 10, 255}, {16, 0, 11, 255},  {106, 255, 116, 255}, {106, 255, 0, 0}, {150, 0, 140, 0}, {150, 0, 0, 0},    {216, 255, 206, 255}, {196, 255, 0, 0}, {10, 0, 0, 0},    {10, 0, 0, 0},     {0, 0, 216, 255},   {40, 0, 0, 0},    {0, 0, 246, 255}, {246, 255, 246, 255}, {10, 251, 10, 5},  {10, 246, 10, 10}, {246, 251, 246, 5}, {246, 246, 246, 10}, {251, 10, 5, 10}, {246, 10, 10, 10}, {251, 246, 5, 246}, {246, 246, 10, 246}, {255, 2, 1, 0},   {1, 3, 0, 255},   {0, 0, 0, 0},     {0, 0, 0, 0},     {0, 0, 0, 3},     {5, 4, 5, 3},     {3, 4, 0, 5},      {4, 4, 3, 5},     {3, 4, 4, 3},      {0, 0, 5, 3},     {3, 0, 1, 0},     {0, 2, 0, 0},     {0, 0, 0, 0},     {2, 5, 3, 1},     {0, 3, 2, 1},     {5, 5, 5, 3},     {5, 3, 5, 5},     {3, 0, 0, 5},     {3, 3, 0, 0},     {0, 0, 0, 0},     {0, 0, 0, 0},     {0, 4, 0, 4},      {0, 4, 0, 4},
    {0, 0, 4, 4},     {5, 3, 5, 3},     {5, 3, 0, 0},     {5, 5, 5, 0},      {3, 0, 0, 0},     {0, 0, 0, 0},      {0, 0, 4, 4},         {4, 0, 4, 0},     {4, 0, 0, 4},     {4, 4, 3, 3},      {4, 3, 3, 4},         {0, 0, 4, 4},     {4, 3, 3, 3},     {4, 3, 4, 0},      {0, 3, 3, 4},       {4, 4, 3, 3},     {3, 4, 0, 0},     {4, 4, 4, 4},         {4, 4, 4, 0},      {4, 0, 0, 4},      {4, 3, 4, 3},       {4, 3, 3, 4},        {0, 0, 3, 3},     {3, 3, 3, 3},      {3, 3, 0, 0},       {0, 3, 3, 3},        {3, 3, 3, 3},     {3, 0, 0, 0},     {3, 3, 3, 3},     {3, 3, 3, 3},     {0, 0, 0, 3},     {3, 0, 0, 0},     {0, 0, 0, 0},      {0, 0, 0, 0},     {2, 1, 2, 1},      {2, 0, 2, 1},     {0, 1, 1, 1},     {2, 2, 1, 1},     {0, 2, 1, 0},     {2, 2, 1, 1},     {1, 1, 1, 0},     {0, 1, 0, 1},     {1, 2, 2, 2},     {2, 2, 0, 2},     {0, 0, 2, 2},     {1, 2, 1, 0},     {1, 0, 2, 1},     {0, 1, 1, 1},
};

static void hud_marker_draw_kind(uint8 index, uint32 x, uint32 y)
{
    const HUD_MARKER_ICON *icon;
    uint32 value, buffers, count, packet;
    SPRT *spr;
    FUNCTION_MARKER(0x8004084C, "ACE3.BIN");
    icon = &marker_icon_records[index];
    value = icon->w;
    if (value == 0)
        return;
    buffers = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS];
    packet = hud_control.marker_packets.cur + count * 20 + 0x1008;
    spr = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 4104u) < sizeof(hud_control.marker_packets.native->labels) && (packet - hud_control.marker_packets.cur - 4104u) % sizeof(SPRT) == 0 ? &hud_control.marker_packets.native->labels[(packet - hud_control.marker_packets.cur - 4104u) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(*spr));
    spr->rgbc.r = (uint8)(0);
    spr->rgbc.g = (uint8)(131);
    spr->rgbc.b = (uint8)(0);
    value = icon->w;
    spr->p[0].xy.y = (sint16)(uint16)(y);
    spr->p[0].xy.x = (sint16)(uint16)(x - value - 2);
    value = icon->u;
    spr->p[0].uv.u = (uint8)(value);
    value = icon->v;
    spr->p[0].uv.v = (uint8)(value);
    value = icon->w;
    spr->w = (sint16)(uint16)(value);
    value = icon->h;
    buffers = hud_control.marker_packets.arena_begin;
    spr->h = (sint16)(uint16)(value);
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS] + 1;
    hud_control.marker_packets.count[HUD_MARKER_LABELS] = count;
    {
        (void)(uint32)(count);
        return;
    }
}

static void hud_marker_draw_label(uint8 index, uint32 x, uint32 y)
{
    const HUD_MARKER_ICON *icon;
    uint32 value, buffers, count, packet;
    SPRT *spr;
    FUNCTION_MARKER(0x80040900, "ACE3.BIN");
    icon = &marker_icon_records[MARKER_LABEL_START + index];
    value = icon->w;
    if (value == 0)
        return;
    buffers = hud_control.marker_packets.arena_begin;
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS];
    packet = hud_control.marker_packets.cur + count * 20 + 0x1008;
    spr = hud_control.marker_packets.native && (packet - hud_control.marker_packets.cur - 4104u) < sizeof(hud_control.marker_packets.native->labels) && (packet - hud_control.marker_packets.cur - 4104u) % sizeof(SPRT) == 0 ? &hud_control.marker_packets.native->labels[(packet - hud_control.marker_packets.cur - 4104u) / sizeof(SPRT)] : (SPRT *)mission_packet_span(packet, sizeof(*spr));
    spr->rgbc.g = (uint8)(131);
    spr->rgbc.r = (uint8)(0);
    spr->rgbc.b = (uint8)(0);
    spr->p[0].xy.x = (sint16)(uint16)(x + 2);
    spr->p[0].xy.y = (sint16)(uint16)(y);
    value = icon->u;
    spr->p[0].uv.u = (uint8)(value);
    value = icon->v;
    spr->p[0].uv.v = (uint8)(value);
    value = icon->w;
    spr->w = (sint16)(uint16)(value);
    value = icon->h;
    buffers = hud_control.marker_packets.arena_begin;
    spr->h = (sint16)(uint16)(value);
    count = hud_control.marker_packets.count[HUD_MARKER_LABELS] + 1;
    hud_control.marker_packets.count[HUD_MARKER_LABELS] = count;
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_notice_update_warnings(const HUD_CONTEXT *context)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 flags, accumulating, kind, mission, position, limit, value;
    uint32 first, second, result;
    FUNCTION_MARKER(0x80042544, "ACE3.BIN");
    flags = mission_target.warning_flags;
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    accumulating = 0;
    if (flags & 2)
    {
        kind = 1;
        goto issue;
    }
    if (flags & 1)
    {
        kind = 2;
        goto issue;
    }
    if (((context->events)) & 7)
    {
        hud_notice_show_pair(0);
        if (((context->events)) & 2)
            mission_prompt_request(1, 60, 0, 0);
        if (((context->events)) & 4)
            mission_prompt_request(2, 60, 0, 0);
        goto finish;
    }
    if (mission_actor_classify_world_boundary(0) == 1)
    {
        mission = (sint16)game_runtime.selection.mission;
        if (mission != 32 && mission != 47 && mission != 53)
        {
            kind = 3;
            goto issue;
        }
    }
    position = pose_owner_object->pose.position[1];
    value = game_runtime.world_settings.minimum_y;
    if ((sint32)position < (sint32)value)
    {
        kind = 4;
        goto issue;
    }
    if (((context->events)) & 0x18)
    {
        kind = 5;
        goto issue;
    }
    limit = game_runtime.world_settings.altitude_limit;
    value = ((context->ground_clearance));
    if ((sint32)value < (sint32)(0 - limit))
        goto outside;
    mission = (sint16)game_runtime.selection.mission;
    if ((mission == 42 || mission == 58) && (sint32)limit < (sint32)position)
    {
    outside:
        value = game_runtime.metrics.outside_warning_ticks + 1;
        game_runtime.metrics.outside_warning_ticks = (uint8)(value);
        accumulating = 1;
        if ((value & 255) >= 30)
        {
            kind = 6;
            goto issue;
        }
        goto finish;
    }
    if (((context->ammunition)) == 0)
    {
        if (game_input.pressed & 0x1000)
        {
            kind = 7;
            goto issue;
        }
    }
    flags = game_runtime.metrics.flags;
    if (flags & 8)
    {
        kind = 8;
        goto issue;
    }
    value = game_input.held;
    if ((sint32)value < 0)
    {
        if (game_input.pressed & 0x80000000)
            sound_play(55);
        kind = 9;
        goto issue;
    }
    if (flags & 16)
    {
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 3)
        {
            if (game_runtime.mission_control.argument == 0)
            {
                kind = 10;
                goto issue;
            }
        }
    }
    goto finish;
issue:
    hud_notice_show_pair(kind);
finish:
    if (accumulating == 0)
        game_runtime.metrics.outside_warning_ticks = (uint8)(0);
    value = ((context->ammunition));
    flags = game_runtime.metrics.flags;
    if (value == 0)
        game_runtime.metrics.flags = (uint32)(flags | 0x20);
    else
        game_runtime.metrics.flags = (uint32)(flags & 0xFFFFFFDF);
    result = hud_control.notice_priority;
    if (result == 0xFFFFFFFF)
    {
        (void)(uint32)(result);
        return;
    }
    first = hud_notice_draw(&hud_control.notice_pair[0]);
    second = hud_notice_draw(&hud_control.notice_pair[1]);
    flags = game_runtime.flags;
    result = 0x20000000;
    if ((flags & 0x20000000) == 0)
    {
        result = hud_control.notice_ticks - 1;
        hud_control.notice_ticks = result;
    }
    if (first == 0 && second == 0)
    {
        result = hud_control.notice_ticks;
        if (result == 0)
            hud_control.notice_priority = 0xFFFFFFFF;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_draw_compass_segment(uint32 angle, const HUD_CONTEXT *context)
{
    typedef struct
    {
        uint8 u, width;
    } HUD_COMPASS_GLYPH;

    static const HUD_COMPASS_GLYPH glyphs[4] = {{70, 15}, {86, 15}, {120, 16}, {104, 15}};
    const HUD_COMPASS_GLYPH *glyph;
    uint16 vertices[6][3];
    uint32 screen[6], masked, odd, segment, left_angle, right_angle, angles, a, b, c, d, e, f, pitch, y, top, i, j, start, count, base, packet, value, width, result;
    POLY_FT4 *prim;
    FUNCTION_MARKER(0x800657B0, "ACE3.BIN");
    masked = angle & 0x3FFF;
    if ((masked & 0x7FF) != 0)
        return;
    odd = masked & 0x800;
    segment = masked >> 11;
    if (odd != 0)
    {
        left_angle = (masked - 192) & 0x3FFF;
        right_angle = (masked + 192) & 0x3FFF;
        a = (uint16)game_wave_at((left_angle << 1));
        b = (uint16)game_wave_at((masked << 1));
        c = (uint16)game_wave_at(((left_angle + 0x1000) << 1));
        d = (uint16)game_wave_at(((masked + 0x1000) << 1));
        angles = ((0));
        e = (uint16)game_wave_at((right_angle << 1));
        pitch = (((sint16)context->angles[0]));
        f = (uint16)game_wave_at(((right_angle + 0x1000) << 1));
        vertices[0][0] = (sint16)a >> 3;
        vertices[0][2] = (sint16)c >> 3;
        vertices[1][0] = (sint16)b >> 3;
        vertices[1][2] = (sint16)d >> 3;
        vertices[3][0] = vertices[0][0];
        vertices[3][2] = vertices[0][2];
        vertices[4][0] = vertices[1][0];
        vertices[4][2] = vertices[1][2];
        value = (uint16)game_wave_at((((0 - pitch) & 0x3FFF) << 1));
        vertices[2][0] = (sint16)e >> 3;
        vertices[2][2] = (sint16)f >> 3;
        vertices[5][0] = vertices[2][0];
        vertices[5][2] = vertices[2][2];
        y = (sint16)value >> 2;
        y = (sint32)((y << 1) + y) >> 2;
        top = y - 144;
        for (i = 0; i < 3; ++i)
        {
            vertices[i][1] = top;
            vertices[i + 3][1] = y;
        }
    }
    else
    {
        left_angle = (masked - 128) & 0x3FFF;
        right_angle = (masked + 128) & 0x3FFF;
        a = (uint16)game_wave_at((left_angle << 1));
        b = (uint16)game_wave_at((right_angle << 1));
        c = (uint16)game_wave_at(((left_angle + 0x1000) << 1));
        angles = ((0));
        d = (uint16)game_wave_at(((right_angle + 0x1000) << 1));
        pitch = (((sint16)context->angles[0]));
        value = (uint16)game_wave_at((((0 - pitch) & 0x3FFF) << 1));
        vertices[0][0] = (sint16)a >> 3;
        vertices[0][2] = (sint16)c >> 3;
        vertices[1][0] = (sint16)b >> 3;
        vertices[1][2] = (sint16)d >> 3;
        vertices[2][0] = vertices[0][0];
        vertices[2][2] = vertices[0][2];
        vertices[3][0] = vertices[1][0];
        vertices[3][2] = vertices[1][2];
        y = (sint16)value >> 2;
        y = (sint32)((y << 1) + y) >> 2;
        vertices[2][1] = y;
        vertices[3][1] = y;
        vertices[0][1] = y - 196;
        vertices[1][1] = y - 196;
    }
    /* Vector Z controls ignore the unwritten stack padding halfword */
    for (j = 0; j < 2; ++j)
    {
        start = j * 3;
        if (j == 1 && odd == 0)
        {
            value = vertices[3][0] | ((uint32)vertices[3][1] << 16);
            xport_gte_write_data(0, value);
            xport_gte_write_data(1, vertices[3][2]);
            xport_gte_execute(0x180001);
            screen[3] = xport_gte_read_data(14);
        }
        else
        {
            for (i = 0; i < 3; ++i)
            {
                value = vertices[start + i][0] | ((uint32)vertices[start + i][1] << 16);
                xport_gte_write_data(i << 1, value);
                xport_gte_write_data((i << 1) + 1, vertices[start + i][2]);
            }
            xport_gte_execute(0x280030);
            screen[start] = xport_gte_read_data(12);
            screen[start + 1] = xport_gte_read_data(13);
            screen[start + 2] = xport_gte_read_data(14);
        }
        xport_gte_read_data(8);
        xport_gte_read_control(31);
        xport_gte_read_data(19);
    }
    count = hud_control.flight_packets.used[HUD_FLIGHT_TEXTURED_QUADS];
    base = hud_control.flight_packets.cur;
    packet = base + 0x1F8 + count * 40;
    for (j = 0; j < (uint32)(odd != 0 ? 2 : 1); ++j)
    {
        prim = &hud_control.flight_packets.native->textured_quads[(packet - hud_control.flight_packets.cur - 504u) / sizeof(POLY_FT4)];
        prim->p[1].attr = (uint16)(54);
        prim->p[0].attr = (uint16)(0x7F98);
        value = hud_control.dim_color[0];
        prim->rgbc.r = (uint8)(value >> 2);
        value = hud_control.dim_color[1];
        prim->rgbc.g = (uint8)(value >> 2);
        value = hud_control.dim_color[2];
        prim->rgbc.b = (uint8)(value >> 2);
        if (odd != 0)
        {
            prim->p[0].xy.xy = (uint32)(screen[j]);
            prim->p[1].xy.xy = (uint32)(screen[j + 1]);
            prim->p[2].xy.xy = (uint32)(screen[j + 3]);
            prim->p[3].xy.xy = (uint32)(screen[j + 4]);
        }
        else
        {
            prim->p[0].xy.xy = (uint32)(screen[0]);
            prim->p[1].xy.xy = (uint32)(screen[1]);
            prim->p[2].xy.xy = (uint32)(screen[2]);
            prim->p[3].xy.xy = (uint32)(screen[3]);
        }
        value = hud_compass_segment_glyphs[segment][j];
        glyph = &glyphs[value];
        value = glyph->u;
        prim->p[0].uv.v = (uint8)(40);
        prim->p[0].uv.u = (uint8)(value);
        value = glyph->u;
        width = glyph->width;
        prim->p[1].uv.v = (uint8)(40);
        prim->p[1].uv.u = (uint8)(value + width);
        value = glyph->u;
        prim->p[2].uv.v = (uint8)(50);
        prim->p[2].uv.u = (uint8)(value);
        value = glyph->u;
        width = glyph->width;
        prim->p[3].uv.u = (uint8)(value + width);
        prim->p[3].uv.v = (uint8)(50);
        result = hud_control.flight_packets.used[HUD_FLIGHT_TEXTURED_QUADS] + 1;
        packet += 40;
        hud_control.flight_packets.used[HUD_FLIGHT_TEXTURED_QUADS] = result;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_draw_orientation_tetrahedron(uint32 x, uint32 y)
{
    const uint8 faces[4][3] = {{0, 2, 1}, {0, 1, 3}, {0, 3, 2}, {1, 2, 3}};
    const uint8 edges[6][4] = {{0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 1, 2}, {1, 2, 0, 3}, {1, 3, 1, 3}, {2, 3, 2, 3}};
    uint32 points[4], areas[4], index, vertex, data, depth_cue, flags, depth, count, packets, packet, result;
    POLY_G3 *tri;
    LINE_G2 *seg;
    FUNCTION_MARKER(0x8006477C, "ACE3.BIN");
    xport_gte_write_control(24, x << 16);
    xport_gte_write_control(25, y << 16);
    for (index = 0; index < 4; index += 1)
    {
        data = hud_orientation_vertices[index].xy;
        xport_gte_write_data(0, data);
        data = hud_orientation_vertices[index].z;
        xport_gte_write_data(1, data);
        xport_gte_execute(0x180001);
        points[index] = xport_gte_read_data(14);
        depth_cue = xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        depth = xport_gte_read_data(19);
    }
    xport_gte_write_control(24, 160 << 16);
    xport_gte_write_control(25, 120 << 16);
    for (index = 0; index < 4; index += 1)
    {
        xport_gte_write_data(12, points[faces[index][0]]);
        xport_gte_write_data(14, points[faces[index][2]]);
        xport_gte_write_data(13, points[faces[index][1]]);
        xport_gte_execute(0x1400006);
        areas[index] = xport_gte_read_data(24);
    }
    count = hud_control.flight_packets.used[HUD_FLIGHT_SHADED_TRIS];
    packets = hud_control.flight_packets.cur;
    packet = packets + count * 28 + 392;
    for (index = 0; index < 4; index += 1)
    {
        if ((sint32)areas[index] <= 0)
            continue;
        tri = &hud_control.flight_packets.native->triangles[(packet - hud_control.flight_packets.cur - 392u) / sizeof(POLY_G3)];
        for (vertex = 0; vertex < 3; vertex += 1)
        {
            tri->p[vertex].rgbc.r = (uint8)(0);
            tri->p[vertex].rgbc.g = (uint8)(faces[index][vertex] == 1 ? 64 : 16);
            tri->p[vertex].rgbc.b = (uint8)(0);
        }
        tri->p[0].xy.xy = (uint32)(points[faces[index][0]]);
        tri->p[1].xy.xy = (uint32)(points[faces[index][1]]);
        tri->p[2].xy.xy = (uint32)(points[faces[index][2]]);
        count = hud_control.flight_packets.used[HUD_FLIGHT_SHADED_TRIS];
        packet += 28;
        hud_control.flight_packets.used[HUD_FLIGHT_SHADED_TRIS] = count + 1;
    }
    count = hud_control.flight_packets.used[HUD_FLIGHT_SHADED_LINES];
    packets = hud_control.flight_packets.cur;
    packet = packets + count * 20 + 2008;
    result = areas[3];
    for (index = 0; index < 6; index += 1)
    {
        if ((sint32)areas[edges[index][2]] < 0 && (sint32)areas[edges[index][3]] < 0)
            continue;
        seg = &hud_control.flight_packets.native->gradient_lines[(packet - hud_control.flight_packets.cur - 2008u) / sizeof(LINE_G2)];
        for (vertex = 0; vertex < 2; vertex += 1)
        {
            seg->p[vertex].rgbc.r = (uint8)(0);
            seg->p[vertex].rgbc.g = (uint8)(edges[index][vertex] == 1 ? 64 : 24);
            seg->p[vertex].rgbc.b = (uint8)(0);
        }
        seg->p[0].xy.xy = (uint32)(points[edges[index][0]]);
        seg->p[1].xy.xy = (uint32)(points[edges[index][1]]);
        count = hud_control.flight_packets.used[HUD_FLIGHT_SHADED_LINES];
        packet += 20;
        count += 1;
        hud_control.flight_packets.used[HUD_FLIGHT_SHADED_LINES] = count;
        if (index == 5)
            result = count;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_draw_attitude_indicator(void)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    MATRIX first, second, third;
    uint16 angles[3];
    uint32 a, b, c, d, e, x, y, z, p, q, r, u, v, w;
    FUNCTION_MARKER(0x8006466C, "ACE3.BIN");
    angles[0] = game_runtime.camera_view.pose.angles[0];
    angles[1] = game_runtime.camera_view.pose.angles[1];
    angles[2] = game_runtime.camera_view.pose.angles[2];
    game_build_rotation_via_gte(&first, angles);
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    angles[0] = pose_owner_object->pose.angles[0];
    pose_owner_object = mission_actor(actor_index_table);
    angles[1] = pose_owner_object->pose.angles[1];
    pose_owner_object = mission_actor(actor_index_table);
    angles[2] = pose_owner_object->pose.angles[2];
    rotation_build(&second, angles);
    angles[1] = 0;
    rotation_build(&third, angles);
    first.t[0] = (sint32)(0);
    third.t[0] = (sint32)(0);
    second.t[0] = (sint32)(0);
    first.t[1] = (sint32)(0);
    third.t[1] = (sint32)(0);
    second.t[1] = (sint32)(0);
    first.t[2] = (sint32)(1024);
    third.t[2] = (sint32)(1024);
    second.t[2] = (sint32)(1024);
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    e = ((uint16)first.m[2][2]);
    SetRotMatrix(&first);
    a = ((uint16)second.m[0][0]);
    b = ((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16));
    c = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)second.m[0][1]);
    b = ((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16));
    c = (sint16)((uint16)second.m[2][1]);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)second.m[0][2]);
    b = ((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16));
    c = ((uint16)second.m[2][2]);
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    w = xport_gte_read_data(11);
    xport_gte_write_control(0, (x & 0xFFFF) | (p << 16));
    xport_gte_write_control(1, (u & 0xFFFF) | (y << 16));
    xport_gte_write_control(2, (q & 0xFFFF) | (v << 16));
    xport_gte_write_control(3, (z & 0xFFFF) | (r << 16));
    xport_gte_write_control(4, w);
    a = ((uint32)second.t[0]);
    b = ((uint32)second.t[1]);
    c = ((uint32)second.t[2]);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    {
        (void)(hud_draw_orientation_tetrahedron(160, 200));
        return;
    }
}

void hud_draw_target_angle_frame(const HUD_CONTEXT *context)
{
    uint32 result, height, angle, sine, cosine, numerator, scale, base, packet, center_x, center_y, left, right, top, bottom, edge_y, ordering, tag, head;
    POLY_FT3 *prim;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800660F8, "ACE3.BIN");
    result = hud_control.projected_depth;
    if ((sint32)result <= 0)
    {
        (void)(uint32)(result);
        return;
    }
    height = geom_get_screen();
    angle = ((context->target_angle_limit)) & 0x3FFF;
    sine = game_wave_at((angle << 1));
    numerator = height * sine;
    cosine = game_wave_at(((angle + 0x1000) << 1));
    if (cosine == 0)
        xport_mips_break(7);
    if (cosine == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    scale = (sint32)numerator / (sint32)cosine;
    base = hud_control.flight_packets.cur;
    center_y = (sint16)hud_control.projected_y;
    scale = (sint32)((scale << 1) + scale) >> 2;
    center_x = (sint16)hud_control.projected_x;
    top = center_y - scale;
    left = center_x - scale;
    edge_y = top + 20;
    packet = base + 0x954;
    prim = &hud_control.flight_packets.native->markers[0];
    prim->p[0].xy.y = (sint16)(uint16)(top);
    prim->p[1].xy.y = (sint16)(uint16)(top);
    prim->p[0].xy.x = (sint16)(uint16)(left + 19);
    prim->p[1].xy.x = (sint16)(uint16)(left);
    prim->p[2].xy.x = (sint16)(uint16)(left);
    prim->p[2].xy.y = (sint16)(uint16)(edge_y);
    center_x = (sint16)hud_control.projected_x;
    packet = base + 0x974;
    prim = &hud_control.flight_packets.native->markers[1];
    prim->p[0].xy.y = (sint16)(uint16)(top);
    prim->p[1].xy.y = (sint16)(uint16)(top);
    prim->p[2].xy.y = (sint16)(uint16)(edge_y);
    right = center_x + scale;
    prim->p[0].xy.x = (sint16)(uint16)(right - 19);
    prim->p[1].xy.x = (sint16)(uint16)(right);
    prim->p[2].xy.x = (sint16)(uint16)(right);
    packet = base + 0x994;
    center_x = (sint16)hud_control.projected_x;
    center_y = (sint16)hud_control.projected_y;
    left = center_x - scale;
    bottom = center_y + scale;
    edge_y = bottom - 20;
    prim = &hud_control.flight_packets.native->markers[2];
    prim->p[0].xy.x = (sint16)(uint16)(left + 19);
    prim->p[0].xy.y = (sint16)(uint16)(bottom);
    prim->p[1].xy.x = (sint16)(uint16)(left);
    prim->p[1].xy.y = (sint16)(uint16)(bottom);
    prim->p[2].xy.x = (sint16)(uint16)(left);
    prim->p[2].xy.y = (sint16)(uint16)(edge_y);
    center_x = (sint16)hud_control.projected_x;
    packet = base + 0x9B4;
    prim = &hud_control.flight_packets.native->markers[3];
    prim->p[0].xy.y = (sint16)(uint16)(bottom);
    prim->p[1].xy.y = (sint16)(uint16)(bottom);
    right = center_x + scale;
    prim->p[0].xy.x = (sint16)(uint16)(right - 19);
    prim->p[1].xy.x = (sint16)(uint16)(right);
    prim->p[2].xy.x = (sint16)(uint16)(right);
    prim->p[2].xy.y = (sint16)(uint16)(edge_y);
    ordering = game_runtime.mission_control.ot;
    tag = prim->tag;
    ordering_entry = mission_ot_span(ordering, sizeof(*ordering_entry));
    head = *ordering_entry;
    prim->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    ordering = game_runtime.mission_control.ot;
    ordering_entry = mission_ot_span(ordering, sizeof(*ordering_entry));
    head = *ordering_entry;
    result = (head & 0xFF000000) | ((base + 0x954) & 0xFFFFFF);
    *ordering_entry = result;
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_draw_compass_ring(const HUD_CONTEXT *context)
{
    MATRIX matrix;
    uint32 angles, angle, sine, cosine, value, flags, base, count, packet, i;
    LINE_F2 *prim;
    FUNCTION_MARKER(0x80065598, "ACE3.BIN");
    HUD_INSTRUMENT_PUBLICATION *published = (HUD_INSTRUMENT_PUBLICATION *)getScratchAddr(0);
    game_build_rotation_via_gte(&matrix, game_runtime.camera_view.pose.angles);
    i = 4;
    SetRotMatrix(&matrix);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    angles = ((0));
    angle = 0 - (uint32)(((sint16)context->angles[0]));
    angle &= 0x3FFF;
    sine = (uint16)game_wave_at((angle << 1));
    value = (sint16)sine >> 2;
    value = (value << 1) + value;
    if ((sint32)value < 0)
        value += 3;
    published->compass.point.axis[1] = (uint16)((sint32)value >> 2);
    xport_gte_write_data(0, (uint32)published->compass.point.axis[1] << 16);
    xport_gte_write_data(1, 0x800);
    xport_gte_execute(0x180001);
    published->compass.previous_screen = xport_gte_read_data(14);
    xport_gte_read_data(8);
    xport_gte_read_control(31);
    xport_gte_read_data(19);
    for (angle = 512; angle < 16385; angle += 512)
    {
        sine = (uint16)game_wave_at(((angle & 0x3FFF) << 1));
        cosine = (uint16)game_wave_at((((angle & 0x3FFF) + 0x1000) << 1));
        published->compass.point.axis[0] = (uint16)((sint16)sine >> 3);
        published->compass.point.z = (uint16)((sint16)cosine >> 3);
        xport_gte_write_data(0, published->compass.point.axis[0] | ((uint32)published->compass.point.axis[1] << 16));
        xport_gte_write_data(1, published->compass.point.z);
        xport_gte_execute(0x180001);
        published->compass.current_screen = xport_gte_read_data(14);
        xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        xport_gte_read_data(19);
        if ((sint32)flags < 0)
        {
            published->compass.previous_screen = published->compass.current_screen;
            continue;
        }
        base = hud_control.flight_packets.cur;
        count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
        value = hud_control.primary_color[0];
        packet = base + 0x3D8 + (count << 4);
        prim = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*prim));
        prim->rgbc.r = (uint8)(value);
        value = hud_control.primary_color[1];
        prim->rgbc.g = (uint8)(value);
        value = hud_control.primary_color[2];
        prim->rgbc.b = (uint8)(value);
        prim->p[0].xy = published->compass.previous_screen;
        prim->p[1].xy = published->compass.current_screen;
        count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
        hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
        published->compass.previous_screen = published->compass.current_screen;
        hud_draw_compass_segment(angle, context);
    }
    return;
}

void hud_draw_target_direction(void)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint16 points[3][2];
    uint32 selected, flags, x, y, distance, vector[3], yaw, angle, trig, size, width, sine, cosine, anchor_x, anchor_y, i, px, py, tx, ty, count, base, packet, red, green, blue, value;
    MISSION_ACTOR_TARGET *target_state;
    MISSION_SCREEN_MARKER *descriptor;
    LINE_G4 *prim;
    FUNCTION_MARKER(0x80065D8C, "ACE3.BIN");
    selected = hud_control.selected_target_idx;
    if (selected == 255)
        return;
    selected = hud_control.selected_target_idx;
    target_state = mission_actor_target(selected);
    if (target_state == 0)
        return;
    flags = (target_state->flags);
    if ((flags & 1) == 0)
        return;
    value = ((sint16)target_state->angle);
    if ((sint32)value < 1024)
    {
        flags = (target_state->projection_flags);
        if ((flags & 0x20000) == 0)
            return;
    }
    flags = (target_state->projection_flags);
    if ((flags & 0x20000) != 0)
    {
        selected = hud_control.selected_target_idx;
        descriptor = mission_actor_marker(selected);
        if (descriptor == 0)
            return;
        x = (descriptor->x);
        x -= 160;
        y = (descriptor->y);
        y -= 120;
        distance = game_square_root(x * x + y * y);
        if ((sint32)distance < 64)
            return;
    }
    selected = hud_control.selected_target_idx;
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    game_pose_relative_direction(pose_owner_object->pose.position, vector, game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles);
    yaw = game_vector_atan2(vector[0], vector[1]);
    distance = game_square_root(vector[0] * vector[0] + vector[1] * vector[1]);
    angle = game_vector_atan2(distance, vector[2]);
    trig = (uint16)game_wave_at((angle & 0x7FFE));
    size = (sint16)trig >> 9;
    points[0][0] = 0;
    points[0][1] = (sint32)size < 6 ? 6 : size;
    width = 11 - (uint32)((sint32)size >> 1);
    if ((sint32)width < 3)
        width = 3;
    points[1][0] = width;
    points[2][0] = 0 - width;
    sine = game_wave_at(((yaw & 0x3FFF) << 1));
    cosine = game_wave_at((((yaw & 0x3FFF) + 0x1000) << 1));
    points[1][1] = 0;
    points[2][1] = 0;
    anchor_x = (uint32)((sint32)((sine << 1) + sine) >> 10) + 160;
    anchor_y = (uint32)((sint32)((cosine << 1) + cosine) >> 10) + 120;
    for (i = 0; i < 3; ++i)
    {
        px = (sint16)points[i][0];
        py = (sint16)points[i][1];
        tx = cosine * px + sine * py;
        ty = (0 - sine) * px + cosine * py;
        points[i][0] = (uint32)((sint32)tx >> 14) + anchor_x;
        points[i][1] = (uint32)((sint32)ty >> 14) + anchor_y;
    }
    red = hud_control.primary_color[0];
    green = hud_control.primary_color[1];
    blue = hud_control.primary_color[2];
    count = hud_control.flight_packets.used[HUD_FLIGHT_WAVEFORM];
    base = hud_control.flight_packets.cur;
    packet = base + 0x878 + count * 40;
    red >>= 2;
    green >>= 2;
    blue >>= 2;
    prim = &hud_control.flight_packets.native->waveform;
    prim->p[0].rgbc.r = (uint8)(red);
    prim->p[0].rgbc.g = (uint8)(green);
    prim->p[0].rgbc.b = (uint8)(blue);
    value = hud_control.primary_color[0];
    prim->p[1].rgbc.r = (uint8)(value);
    value = hud_control.primary_color[1];
    prim->p[1].rgbc.g = (uint8)(value);
    value = hud_control.primary_color[2];
    prim->p[2].rgbc.r = (uint8)(red);
    prim->p[2].rgbc.g = (uint8)(green);
    prim->p[2].rgbc.b = (uint8)(blue);
    prim->p[3].rgbc.r = (uint8)(red);
    prim->p[3].rgbc.g = (uint8)(green);
    prim->p[3].rgbc.b = (uint8)(blue);
    prim->p[1].rgbc.b = (uint8)(value);
    value = points[1][0] | ((uint32)points[1][1] << 16);
    prim->p[0].xy.xy = (uint32)(value);
    x = points[0][0] | ((uint32)points[0][1] << 16);
    prim->p[1].xy.xy = (uint32)(x);
    y = points[2][0] | ((uint32)points[2][1] << 16);
    prim->p[3].xy.xy = (uint32)(value);
    prim->p[2].xy.xy = (uint32)(y);
    count = hud_control.flight_packets.used[HUD_FLIGHT_WAVEFORM];
    hud_control.flight_packets.used[HUD_FLIGHT_WAVEFORM] = count + 1;
    return;
}

uint32 hud_draw_brand_footer(uint32 ordering, uint32 packet, uint32 strength)
{
    uint32 value, color, head;
    FUNCTION_MARKER(0x80035578, "ACE3.BIN");
    if ((sint32)strength >= 4097)
        strength = 4096;
    if ((sint32)strength < 0)
        strength = 0;
    value = ((strength << 5) - strength) << 2;
    if ((sint32)value < 0)
        value += 0xFFF;
    color = (uint32)((sint32)value >> 13) << 8;
    TILE *bar = mission_packet_span(packet, sizeof(*bar));
    bar->rgbc.rgbc = color | 0x62000000;
    bar->p[0].y = 216;
    bar->w = 320;
    bar->p[0].x = 0;
    bar->h = 2;
    uint32 *ordering_entry = mission_ot_span(ordering, sizeof(*ordering_entry));
    head = *ordering_entry;
    bar->tag = head | 0x03000000;
    *ordering_entry = packet & 0xFFFFFF;
    if ((sint32)strength < 0)
        strength += 63;
    color = (uint32)((sint32)strength >> 7) << 8;
    return ui_draw_compact_label(ordering, packet + 16, 224, 208, color, (const uint8 *)"ACECOMBAT3");
}

void hud_draw_steering_reference(const HUD_CONTEXT *context)
{
    MATRIX first, second;
    uint16 angles[3];
    uint32 value, a, b, c, d, e, x, y, z, p, q, r, u, v, w, projected, depth, pass, storage, packet, count;
    LINE_F2 *prim;
    FUNCTION_MARKER(0x8006447C, "ACE3.BIN");
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    game_build_rotation_via_gte(&first, (context->camera_steer));
    for (pass = 0; pass < 2; ++pass)
    {
        if (pass != 0)
        {
            game_build_rotation_via_gte(&second, angles);
            a = ((uint32)(uint16)second.m[0][0] | ((uint32)(uint16)second.m[0][1] << 16));
            b = ((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16));
            c = ((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16));
            d = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
            e = ((uint16)second.m[2][2]);
            SetRotMatrix(&second);
            a = ((uint16)first.m[0][0]);
            b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
            c = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
            xport_gte_write_data(0, a | (b & 0xFFFF0000));
            xport_gte_write_data(1, c);
            xport_gte_execute(0x486012);
            a = ((uint16)first.m[0][1]);
            b = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
            c = (sint16)((uint16)first.m[2][1]);
            x = xport_gte_read_data(9);
            y = xport_gte_read_data(10);
            z = xport_gte_read_data(11);
            xport_gte_write_data(0, a | (b << 16));
            xport_gte_write_data(1, c);
            xport_gte_execute(0x486012);
            a = ((uint16)first.m[0][2]);
            b = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
            c = ((uint16)first.m[2][2]);
            p = xport_gte_read_data(9);
            q = xport_gte_read_data(10);
            r = xport_gte_read_data(11);
            xport_gte_write_data(0, a | (b & 0xFFFF0000));
            xport_gte_write_data(1, c);
            xport_gte_execute(0x486012);
            {
                uint32 matrix_word = (x & 0xFFFF) | (p << 16);
                first.m[0][0] = (sint16)matrix_word;
                first.m[0][1] = (sint16)(matrix_word >> 16);
            }
            {
                uint32 matrix_word = (z & 0xFFFF) | (r << 16);
                first.m[2][0] = (sint16)matrix_word;
                first.m[2][1] = (sint16)(matrix_word >> 16);
            }
            u = xport_gte_read_data(9);
            v = xport_gte_read_data(10);
            {
                uint32 matrix_word = (u & 0xFFFF) | (y << 16);
                first.m[0][2] = (sint16)matrix_word;
                first.m[1][0] = (sint16)(matrix_word >> 16);
            }
            {
                uint32 matrix_word = (q & 0xFFFF) | (v << 16);
                first.m[1][1] = (sint16)matrix_word;
                first.m[1][2] = (sint16)(matrix_word >> 16);
            }
            w = xport_gte_read_data(11);
            first.m[2][2] = (sint16)(w);
        }
        a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
        b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
        c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
        d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
        e = ((uint16)first.m[2][2]);
        SetRotMatrix(&first);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 256);
        xport_gte_execute(0x180001);
        projected = xport_gte_read_data(14);
        xport_gte_read_data(8);
        xport_gte_read_control(31);
        depth = (sint32)xport_gte_read_data(19) >> 2;
        if (pass == 0)
        {
            hud_control.projected_x = projected;
            hud_control.projected_y = projected >> 16;
            hud_control.projected_depth = (uint32)(depth);
            {
                angles[0] = 0 - context->delta_angles[0];
                angles[1] = 0 - context->delta_angles[1];
            }
            angles[2] = 0;
            if (angles[0] == 0 && angles[1] == 0)
                return;
        }
    }
    if ((sint32)depth <= 0)
        return;
    storage = hud_control.flight_packets.cur;
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    value = hud_control.primary_color[0];
    packet = storage + (count << 4) + 0x3D8;
    prim = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*prim));
    prim->rgbc.r = (uint8)(value);
    value = hud_control.primary_color[1];
    prim->rgbc.g = (uint8)(value);
    value = hud_control.primary_color[2];
    prim->rgbc.b = (uint8)(value);
    prim->p[0].x = (sint16)(uint16)((projected & 0xFFFF) - 2);
    prim->p[1].x = (sint16)(uint16)((projected & 0xFFFF) + 2);
    value = projected >> 16;
    prim->p[1].y = (sint16)(uint16)(value);
    prim->p[0].y = (sint16)(uint16)(value);
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
    value = hud_control.primary_color[0];
    packet += 16;
    prim = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*prim));
    prim->rgbc.r = (uint8)(value);
    value = hud_control.primary_color[1];
    prim->rgbc.g = (uint8)(value);
    value = hud_control.primary_color[2];
    prim->rgbc.b = (uint8)(value);
    prim->p[1].x = (sint16)(uint16)(projected);
    prim->p[0].x = (sint16)(uint16)(projected);
    prim->p[0].y = (sint16)(uint16)((projected >> 16) - 3);
    prim->p[1].y = (sint16)(uint16)(projected >> 16);
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
    return;
}

uint32 hud_marker_radius_for_depth(uint32 distance)
{
    uint32 value, result;
    FUNCTION_MARKER(0x8003B758, "ACE3.BIN");
    if ((sint32)distance >= 0x1001)
        return 2;
    value = 0x1000 - distance;
    value *= value;
    value = ((sint32)value >> 12);
    result = ((sint32)((value << 1) + value) >> 9);
    if ((sint32)result < 2)
        result = 2;
    if ((sint32)result >= 25)
        result = 24;
    return result;
}

void hud_update_panels_and_fade(void)
{
    uint32 value, mode, numerator, divisor, scale, ot, packet, limit;
    FUNCTION_MARKER(0x8003A2B8, "ACE3.BIN");
    hud_project_actor_markers();
    hud_lock_update();
    hud_panel_update();
    hud_portrait_update();
    if (game_runtime.mission_control.flags & 1)
        hud_notice_reset_pair();
    value = game_runtime.metrics.fade.active;
    if (value != 1)
        return;
    value = speech_queue_mode();
    if (value != 0)
    {
        (void)(uint32)(value);
        return;
    }
    mode = game_runtime.metrics.fade.mode;
    if (mode == 1 || mode == 2)
    {
        if (mode == 1)
        {
            divisor = (sint16)game_runtime.metrics.fade.duration;
            value = (sint16)game_runtime.metrics.fade.ticks;
            numerator = (divisor - value) * 1365;
        }
        else
        {
            value = (sint16)game_runtime.metrics.fade.ticks;
            divisor = (sint16)game_runtime.metrics.fade.duration;
            numerator = value << 12;
        }
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        scale = (sint32)numerator / (sint32)divisor;
        ot = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        value = effect_draw_screen_transition(ot, packet, scale);
        game_runtime.mission_control.packet_cur = (uint32)(value);
    }
    value = game_runtime.metrics.fade.ticks;
    limit = (sint16)game_runtime.metrics.fade.duration;
    value += 1;
    game_runtime.metrics.fade.ticks = (uint16)(value);
    value = (sint16)value < (sint32)limit;
    if (value == 0)
        game_runtime.metrics.fade.active = (uint8)(0);
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_update_alert_colors(const HUD_CONTEXT *context)
{
    uint32 value, raw, mode, result;
    FUNCTION_MARKER(0x8003A12C, "ACE3.BIN");
    if (((context->events)) & 7)
        hud_control.alert_ticks = 15;
    if (mission_target.warning_flags & 2)
        hud_control.alert_mode = 1;
    else
    {
        value = (sint16)hud_control.alert_ticks;
        raw = hud_control.alert_ticks;
        if ((sint32)value > 0)
        {
            hud_control.alert_mode = 2;
            hud_control.alert_ticks = raw - 1;
        }
        else
            hud_control.alert_mode = 0;
    }
    mode = hud_control.alert_mode;
    if (mode == 1)
    {
        hud_control.dim_color[0] = (uint8)(131);
        result = 255;
        goto red;
    }
    if (mode == 2)
    {
        value = hud_control.display_ticks;
        if ((sint32)value % 6 < 4)
        {
            hud_control.dim_color[0] = (uint8)(67);
            result = 131;
            goto red;
        }
    }
    hud_control.dim_color[1] = (uint8)(131);
    hud_control.dim_color[0] = (uint8)(0);
    hud_control.dim_color[2] = (uint8)(0);
    hud_control.primary_color[0] = 0;
    hud_control.primary_color[1] = 255;
    hud_control.primary_color[2] = 0;
    hud_control.number_color = 0;
    return;
red:
    hud_control.dim_color[1] = (uint8)(0);
    hud_control.dim_color[2] = (uint8)(0);
    hud_control.primary_color[0] = result;
    hud_control.primary_color[1] = 0;
    hud_control.primary_color[2] = 0;
    hud_control.number_color = 1;
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_project_actor_markers(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, value, other, flags;
    uint32 projection, depth, size, x, y, z, limit, state_flags;
    MISSION_SCRIPT_REFERENCE trigger;
    MISSION_ACTOR_TARGET *target;
    MISSION_SCREEN_MARKER *record;
    FUNCTION_MARKER(0x8003B34C, "ACE3.BIN");
    SetRotMatrix(&game_runtime.mission_matrices.view);
    index = 5;
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        target = mission_actor_target(index);
        value = actor_traits_object->flags;
        record = mission_actor_marker(index);
        if ((value & 0x4000) == 0 || target == 0 || record == 0)
            continue;
        target->projection_flags = (uint32)(0);
        value = pose_owner_object->pose.position[0];
        other = game_runtime.camera_view.pose.position[0];
        x = (sint32)(value - other) >> 12;
        if (x + 0x8000 > 0xFFFF)
            continue;
        value = pose_owner_object->pose.position[1];
        other = game_runtime.camera_view.pose.position[1];
        y = (sint32)(value - other) >> 12;
        if (y + 0x8000 > 0xFFFF)
            continue;
        value = pose_owner_object->pose.position[2];
        other = game_runtime.camera_view.pose.position[2];
        z = (sint32)(value - other) >> 12;
        if (z + 0x8000 > 0xFFFF)
            continue;
        value = (target->projection_flags);
        target->projection_flags = (uint32)(value | 0x10000);
        xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(1, z & 0xFFFF);
        xport_gte_execute(0x180001);
        value = xport_gte_read_data(14);
        record->x = (sint16)value;
        record->y = (sint16)(value >> 16);
        projection = xport_gte_read_data(8);
        flags = xport_gte_read_control(31);
        depth = xport_gte_read_data(19);
        size = hud_marker_radius_for_depth((sint32)depth >> 2);
        record->radius = (uint16)(size);
        if ((sint32)flags < 0)
            continue;
        value = (target->projection_flags) | 0x40000;
        target->projection_flags = (uint32)(value);
        if (((uint16)record->x) >= 321)
            continue;
        y = (record->y);
        if ((sint32)y >= 241 || (sint32)y < 0)
            continue;
        target->projection_flags = (uint32)(value | 0x20000);
    }
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        value = actor_traits_object->flags;
        target = mission_actor_target(index);
        if ((value & 0x4000) == 0)
            continue;
        if (((target->projection_flags) & 0x20000) == 0)
        {
            target->visible_ticks = (uint32)(0);
            continue;
        }
        if ((game_runtime.mission_control.mode & 4) == 0)
        {
            target->visible_ticks = (uint32)(0);
            continue;
        }
        if ((game_runtime.flags & 0x20000000) == 0)
        {
            value = (target->visible_ticks);
            target->visible_ticks = (uint32)(value + 1);
        }
        limit = (sint16)game_runtime.selection.mission == 6 ? 0 : 90;
        state_flags = (*mission_actor_flags(index));
        if ((state_flags & 0x1000000) == 0 && ((sint32)target->visible_ticks) >= (sint32)limit)
        {
            *mission_actor_flags(index) = (uint32)(state_flags | 0x1000000);
            if (index < 32)
            {
                trigger = mission_primary_binding(index & 0x03FFFFFFu)->visible_script;
            }
            else
            {
                trigger = mission_secondary_binding(index - 32)->visible_script;
            }
            if (mission_script_reference_present(trigger))
                mission_script_start_legacy(trigger);
        }
        if ((sint32)game_runtime.mission_control.argument <= 0)
            continue;
        state_flags = (*mission_actor_flags(index));
        if ((state_flags & 0x2000000) == 0 && ((sint32)target->visible_ticks) >= (sint32)limit)
        {
            *mission_actor_flags(index) = (uint32)(state_flags | 0x2000000);
            if (index < 32)
            {
                trigger = mission_primary_binding(index & 0x03FFFFFFu)->stage_script;
            }
            else
            {
                trigger = mission_secondary_binding(index - 32)->stage_script;
            }
            if (mission_script_reference_present(trigger))
                mission_script_start_legacy(trigger);
        }
    }
    return;
}

void hud_begin_flight_packets(void)
{
    uint32 offset, value;
    FUNCTION_MARKER(0x80063F80, "ACE3.BIN");
    for (offset = 36; offset >= 8; offset -= 4)
        hud_control.flight_packets.used[(offset - 8) >> 2] = 0;
    value = game_runtime.display_buffer;
    hud_control.flight_packets.native = hud_control.flight_packets.bank_data[value];
    value = hud_control.flight_packets.bank[value];
    hud_control.flight_packets.cur = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void hud_draw_steering_attitude(const HUD_CONTEXT *context)
{
    uint32 angles, value;
    FUNCTION_MARKER(0x80064400, "ACE3.BIN");
    angles = ((0));
    value = (((context->camera_steer[0])) + 2) & 0xFFFF;
    if (value >= 5)
    {
        (void)(hud_draw_attitude_indicator());
        return;
    }
    value = (((context->camera_steer[1])) + 2) & 0xFFFF;
    if (value >= 5)
    {
        (void)(hud_draw_attitude_indicator());
        return;
    }
    value = (((context->camera_steer[2])) + 2) & 0xFFFF;
    if (value >= 5)
    {
        (void)(hud_draw_attitude_indicator());
        return;
    }
    return;
}

void hud_draw_center_reference(void)
{
    static const uint32 left[3] = {0, 260, 314};
    static const uint32 right[3] = {60, 290, 320};
    uint32 count, packet, red, green, blue, i;
    LINE_F2 *prim;
    FUNCTION_MARKER(0x80064CB4, "ACE3.BIN");
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    packet = hud_control.flight_packets.cur + (count << 4) + 0x3D8;
    red = hud_control.primary_color[0];
    green = hud_control.primary_color[1];
    blue = hud_control.primary_color[2];
    red >>= 1;
    green >>= 1;
    blue >>= 1;
    for (i = 0; i < 3; ++i)
    {
        prim = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*prim));
        prim->rgbc.r = (uint8)(red);
        prim->rgbc.g = (uint8)(green);
        prim->rgbc.b = (uint8)(blue);
        prim->p[0].x = (sint16)(uint16)(left[i]);
        prim->p[1].x = (sint16)(uint16)(right[i]);
        prim->p[1].y = (sint16)(uint16)(120);
        prim->p[0].y = (sint16)(uint16)(120);
        count = hud_control.flight_packets.used[HUD_FLIGHT_LINES] + 1;
        packet += 16;
        hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count;
    }
    {
        (void)(uint32)(count);
        return;
    }
}

void hud_submit_flight_packets(void)
{
    static const uint32 starts[7] = {8, 0x188, 0x1F8, 0x3D8, 0x7D8, 0x878, 0x8A0};
    static const uint32 strides[7] = {24, 28, 40, 16, 20, 40, 20};
    uint32 count, ot, group, index, packet, tag, head, value;
    uint32 *ordering_entry, *packet_tag;
    FUNCTION_MARKER(0x80063FCC, "ACE3.BIN");
    count = hud_control.flight_packets.used[HUD_FLIGHT_QUADS];
    ot = game_runtime.mission_control.ot;
    ordering_entry = mission_ot_span(ot, sizeof(*ordering_entry));
    for (group = 0; group < 7; ++group)
    {
        if (group != 0)
            count = hud_control.flight_packets.used[group];
        if (group == 0 && count >= 16)
        {
#ifndef NDEBUG
            printf("AVIPKT %d\n", (sint32)(count));
#endif
            continue;
        }
        if ((sint32)count <= 0)
            continue;
        index = 0;
        packet = hud_control.flight_packets.cur + starts[group];
        do
        {
            ++index;
            packet_tag = mission_packet_span(packet, sizeof(*packet_tag));
            tag = *packet_tag;
            head = *ordering_entry;
            *packet_tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
            head = *ordering_entry;
            *ordering_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
            count = hud_control.flight_packets.used[group];
            value = (sint32)index < (sint32)count;
            packet += strides[group];
        } while (value);
    }
    value = hud_control.flight_packets.used[HUD_FLIGHT_FIXED_CHAIN];
    if (value == 1)
    {
        packet = hud_control.flight_packets.cur;
        head = *ordering_entry;
        packet_tag = mission_packet_span(packet + 0x940, sizeof(*packet_tag));
        tag = *packet_tag;
        *packet_tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
        head = *ordering_entry;
        *ordering_entry = (head & 0xFF000000) | ((packet + 0x940) & 0xFFFFFF);
    }
    packet = hud_control.flight_packets.cur;
    head = *ordering_entry;
    packet_tag = mission_packet_span(packet, sizeof(*packet_tag));
    tag = *packet_tag;
    *packet_tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    head = *ordering_entry;
    value = (head & 0xFF000000) | (packet & 0xFFFFFF);
    *ordering_entry = value;
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 hud_map_world_position(const uint32 position[3], const uint16 origin[2], uint16 output[2])
{
    uint32 x, z, base, value;
    sint32 out_x, out_z;
    FUNCTION_MARKER(0x8003EC0C, "ACE3.BIN");
    x = (position[0]);
    z = (position[2]);
    x = ((uint32)((sint32)x >> 23)) + 128;
    z = ((uint32)((sint32)(0 - z) >> 23)) + 128;
    if ((sint32)x >= 256)
        x = 255;
    if ((sint32)x < 0)
        x = 0;
    if ((sint32)z >= 256)
        z = 255;
    if ((sint32)z < 0)
        z = 0;
    base = origin[0];
    value = x - base;
    output[0] = value;
    base = origin[1];
    out_x = (sint16)output[0];
    value = z - base;
    output[1] = value;
    if (out_x < 0)
        return 0xFFFFFFFF;
    out_z = (sint16)value;
    if (out_z < 0 || out_x >= 128 || out_z >= 128)
        return 0xFFFFFFFF;
    return 0;
}

void hud_draw_mission_scores(void)
{
    uint32 red, green, blue, color, value, first, second, index, active, flags, row, y;
    const uint8 *title;
    uint32 ordering, packet, result, x, quotient;
    uint8 digit[2];
    FUNCTION_MARKER(0x8003A68C, "ACE3.BIN");
    blue = hud_control.dim_color[2];
    green = hud_control.dim_color[1];
    red = hud_control.dim_color[0];
    color = ((blue >> 1) << 16) | ((green >> 1) << 8) | (red >> 1);
    value = (sint16)game_runtime.selection.mission;
    if (value == 30)
    {
        first = game_runtime.mission_control.argument;
        if (first == 0)
        {
            second = game_runtime.mission_control.ticks;
            if (second == 0)
            {
                for (index = 0; index < sizeof(hud_control.score_name); ++index)
                {
                    value = (uint8)game_runtime.account.name[index];
                    if (value - 97 < 26)
                        value -= 32;
                    hud_control.score_name[index] = (uint8)value;
                }
            }
        }
        value = (sint16)game_runtime.selection.mission;
        if (value == 30)
        {
            active = mission_flow.request.current_mission_arg;
            flags = 2;
            if (active != 0)
            {
                flags = game_runtime.mission_control.mode & 2;
            }
            if (active == 0 || flags != 0)
            {
                for (row = 0; row < 2; ++row)
                {
                    title = row == 0 ? hud_control.score_name : (const uint8 *)"KEITH";
                    y = row == 0 ? 80 : 88;
                    ordering = game_runtime.mission_control.ot;
                    packet = game_runtime.mission_control.packet_cur;
                    result = ui_draw_compact_label(ordering, packet, 222, y, color, title);
                    game_runtime.mission_control.packet_cur = (uint32)(result);
                    value = (row == 0 ? game_runtime.metrics.category0_4_count : game_runtime.metrics.category1_count);
                    digit[0] = value / 10 + 48;
                    digit[1] = 0;
                    ordering = game_runtime.mission_control.ot;
                    packet = game_runtime.mission_control.packet_cur;
                    result = ui_draw_compact_label(ordering, packet, 296, y, color, digit);
                    game_runtime.mission_control.packet_cur = (uint32)(result);
                    value = (row == 0 ? game_runtime.metrics.category0_4_count : game_runtime.metrics.category1_count);
                    digit[0] = value % 10 + 48;
                    ordering = game_runtime.mission_control.ot;
                    packet = game_runtime.mission_control.packet_cur;
                    result = ui_draw_compact_label(ordering, packet, 304, y, color, digit);
                    game_runtime.mission_control.packet_cur = (uint32)(result);
                }
            }
        }
    }
    value = (sint16)game_runtime.selection.mission;
    result = 3;
    if (value != 3)
    {
        (void)(uint32)(result);
        return;
    }
    active = mission_flow.request.current_mission_arg;
    if (active != 0 && active != 3)
    {
        result = game_runtime.mission_control.mode & 2;
        if (result == 0)
        {
            (void)(uint32)(result);
            return;
        }
    }
    digit[1] = 0;
    ordering = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    result = ui_draw_compact_label(ordering, packet, 272, 80, color, (const uint8 *)"POINT");
    game_runtime.mission_control.packet_cur = (uint32)(result);
    value = game_runtime.metrics.score;
    x = 304;
    while (value != 0)
    {
        quotient = (sint32)value / 10;
        digit[0] = value - quotient * 10 + 48;
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = ui_draw_compact_label(ordering, packet, x, 88, color, digit);
        x -= 8;
        value = quotient;
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_draw_flight_instruments(const HUD_CONTEXT *context)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 object, flags, saved_height, selected, mode;
    FUNCTION_MARKER(0x80063E4C, "ACE3.BIN");
    hud_begin_flight_packets();
    object = ((0));
    flags = ((*context->camera_mode)) & 0x401;
    if (flags != 0)
    {
        hud_draw_steering_attitude(context);
        hud_draw_center_reference();
        saved_height = geom_get_screen();
        hud_draw_steering_reference(context);
        hud_draw_pitch_ladder(context);
        geom_set_screen(120);
        hud_draw_compass_ring(context);
        geom_set_screen(saved_height);
        hud_draw_target_angle_frame(context);
    }
    flags = game_runtime.mission_control.mode & 4;
    if (flags != 0)
    {
        selected = hud_control.selected_target_idx;
        hud_draw_target_direction();
        if (selected != 255)
        {
            actor_index_table = 0;
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
            flags = actor_traits_object->flags;
            if ((flags & 0x40) == 0)
                hud_aim_update();
        }
        mode = hud_control.weapon_lock_mode;
        if (mode == 1)
            hud_draw_impact_reticle(context);
    }
    {
        (void)(hud_submit_flight_packets());
        return;
    }
}

uint32 hud_estimate_impact_ticks(uint32 distance, uint32 speed)
{
    uint32 total = 0, ticks = 0;
    FUNCTION_MARKER(0x80066A74, "ACE3.BIN");
    if ((sint32)distance > 0)
        do
        {
            speed += 669;
            total += speed;
            ++ticks;
        } while ((sint32)total < (sint32)distance);
    return ticks;
}

void hud_draw_impact_reticle(const HUD_CONTEXT *context)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    static const sint16 offsets[6][4] = {{-10, -10, 10, 10}, {-10, 10, 10, -10}, {0, -6, 6, 0}, {6, 0, 0, 6}, {0, 6, -6, 0}, {-6, 0, 0, -6}};
    MATRIX matrix;
    uint16 vector[3], screen[2];
    uint32 selected, mission, flags, target_y, value, other, distance, ticks, direct, dx, dz, base, count, packet, i, result;
    LINE_F2 *prim;
    FUNCTION_MARKER(0x80066590, "ACE3.BIN");
    actor_index_table = 0;
    selected = hud_control.selected_target_idx;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    if (selected == 255)
        goto fallback_height_66590;
    mission = (sint16)game_runtime.selection.mission;
    if (mission != 13)
    {
        flags = (*mission_actor_flags(selected));
        if ((~flags & 0x400) == 0)
            goto fallback_height_66590;
    }
    pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    target_y = pose_owner_target->pose.position[1];
    value = pose_owner_object->pose.position[1];
    if ((sint32)target_y >= (sint32)value)
        goto height_ready_66590;
fallback_height_66590:
    target_y = (context->ground_height);
height_ready_66590:
    value = pose_owner_object->pose.position[1];
    distance = (sint32)value < (sint32)target_y ? target_y - value : 0;
    value = actor_motion_object->velocity[1];
    ticks = hud_estimate_impact_ticks(distance, value + (uint32)((sint32)value >> 1));
    direct = game_runtime.selection.weapon_code[1] == 9;
    if (direct == 0 && mission_target_uses_alternate_weapon() == 1)
    {
        direct = game_runtime.selection.weapon_code[2] == 9;
    }
    if (direct != 0)
    {
        value = actor_motion_object->velocity[0];
        dx = value * ticks;
        value = actor_motion_object->velocity[2];
        dz = value * ticks;
    }
    else
    {
        value = actor_motion_object->velocity[0];
        dx = (value + (uint32)((sint32)value >> 1)) * ticks;
        value = actor_motion_object->velocity[2];
        dz = (value + (uint32)((sint32)value >> 1)) * ticks;
    }
    value = pose_owner_object->pose.position[0];
    other = game_runtime.camera_view.pose.position[0];
    vector[0] = (sint32)(value + dx - other) >> 12;
    value = game_runtime.camera_view.pose.position[1];
    vector[1] = (sint32)(target_y - value) >> 12;
    value = pose_owner_object->pose.position[2];
    other = game_runtime.camera_view.pose.position[2];
    vector[2] = (sint32)(value + dz - other) >> 12;
    game_build_rotation_via_gte(&matrix, game_runtime.camera_view.pose.angles);
    matrix.t[2] = 0;
    matrix.t[1] = 0;
    matrix.t[0] = 0;
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    scene_geometry_project_short_vector(vector, screen);
    base = hud_control.flight_packets.cur;
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    packet = base + (count << 4) + 984;
    for (i = 0; i < 6; ++i)
    {
        value = hud_control.primary_color[0];
        prim = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*prim));
        prim->rgbc.r = (uint8)(value);
        value = hud_control.primary_color[1];
        prim->rgbc.g = (uint8)(value);
        value = hud_control.primary_color[2];
        prim->rgbc.b = (uint8)(value);
        prim->p[0].x = (sint16)(uint16)(screen[0] + offsets[i][0]);
        prim->p[0].y = (sint16)(uint16)(screen[1] + offsets[i][1]);
        prim->p[1].x = (sint16)(uint16)(screen[0] + offsets[i][2]);
        prim->p[1].y = (sint16)(uint16)(screen[1] + offsets[i][3]);
        result = hud_control.flight_packets.used[HUD_FLIGHT_LINES] + 1;
        hud_control.flight_packets.used[HUD_FLIGHT_LINES] = result;
        packet += 16;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void hud_initialize_default_colors(void)
{
    FUNCTION_MARKER(0x8003A07C, "ACE3.BIN");
    hud_control.primary_color[1] = 255;
    hud_control.alert_mode = 0;
    hud_control.alert_ticks = 0;
    hud_control.primary_color[0] = 0;
    hud_control.primary_color[2] = 0;
    hud_control.dim_color[0] = (uint8)(0);
    hud_control.dim_color[1] = (uint8)(0x83);
    hud_control.dim_color[2] = (uint8)(0);
    hud_control.number_color = 0;
}

void hud_draw_single_notice(void)
{
    FUNCTION_MARKER(0x800428F8, "ACE3.BIN");
    {
        (void)(uint32)(hud_notice_draw(&hud_control.notice_single));
        return;
    }
}

static uint32 hud_pitch_project_line(uint32 packet, HUD_INSTRUMENT_PUBLICATION *work)
{
    uint32 value, flags;
    LINE_F2 *line;
    for (uint32 i = 0; i < 3; ++i)
    {
        const HUD_POINT_PUBLICATION *point = &work->pitch.points[i];
        xport_gte_write_data(i * 2, point->xy);
        xport_gte_write_data(i * 2 + 1, point->z);
    }
    xport_gte_execute(0x280030);
    value = xport_gte_read_data(12);
    line = (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
    line->p[0].xy = (uint32)(value);
    value = xport_gte_read_data(13);
    line->p[1].xy = (uint32)(value);
    value = xport_gte_read_data(14);
    work->pitch.screen = value;
    xport_gte_read_data(8);
    flags = xport_gte_read_control(31);
    xport_gte_read_data(19);
    return flags;
}

void hud_draw_pitch_ladder(const HUD_CONTEXT *context)
{
    MATRIX first, second;
    uint16 angles[3], pitch_step;
    uint32 pitch, value, a, b, c, d, e, x, y, z, p, q, r, u, v, w, count, storage, packet, level, red, green, blue, raw, sine, cosine, magnitude, base, first_flag, flags, xy[2], degree, tens, digit, i, side, color;
    sint32 step;
    sint64 product;
    LINE_F2 *line;
    SPRT *digits;
    FUNCTION_MARKER(0x80064D98, "ACE3.BIN");
    HUD_INSTRUMENT_PUBLICATION *published = (HUD_INSTRUMENT_PUBLICATION *)getScratchAddr(0);
    angles[0] = context->angles[0];
    angles[1] = 0;
    angles[2] = context->angles[2];
    pitch = (sint16)angles[0];
    game_build_rotation_via_gte(&first, angles);
    game_build_rotation_via_gte(&second, context->camera_steer);
    a = ((uint32)(uint16)second.m[0][0] | ((uint32)(uint16)second.m[0][1] << 16));
    b = ((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16));
    c = ((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16));
    d = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
    e = ((uint16)second.m[2][2]);
    SetRotMatrix(&second);
    a = ((uint16)first.m[0][0]);
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)first.m[0][1]);
    b = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    c = (sint16)((uint16)first.m[2][1]);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = ((uint16)first.m[0][2]);
    b = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    c = ((uint16)first.m[2][2]);
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    {
        uint32 matrix_word = (x & 0xFFFF) | (p << 16);
        first.m[0][0] = (sint16)matrix_word;
        first.m[0][1] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (z & 0xFFFF) | (r << 16);
        first.m[2][0] = (sint16)matrix_word;
        first.m[2][1] = (sint16)(matrix_word >> 16);
    }
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    {
        uint32 matrix_word = (u & 0xFFFF) | (y << 16);
        first.m[0][2] = (sint16)matrix_word;
        first.m[1][0] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (q & 0xFFFF) | (v << 16);
        first.m[1][1] = (sint16)matrix_word;
        first.m[1][2] = (sint16)(matrix_word >> 16);
    }
    w = xport_gte_read_data(11);
    first.m[2][2] = (sint16)(w);
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    product = (sint64)(sint32)pitch * 0x24168E19;
    count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
    storage = hud_control.flight_packets.cur;
    packet = storage + (count << 4) + 984;
    value = (uint32)((sint32)(product >> 32) >> 5) - (uint32)((sint32)pitch >> 31) - 4;
    pitch_step = (value * 227);
    for (step = -4; step < 5; ++step)
    {
        level = step < 0 ? step + 5 : 5 - step;
        level = (level << 1) * (level << 1) * 4;
        if ((sint32)level >= 256)
            level = 255;
        red = hud_control.primary_color[0];
        green = hud_control.primary_color[1];
        blue = hud_control.primary_color[2];
        raw = pitch_step;
        red = red * level >> 8;
        sine = game_wave_at((raw & 0x3FFF) * 2);
        raw = pitch_step;
        sine = (sint32)(0 - sine) >> 4;
        cosine = (uint16)game_wave_at(((raw & 0x3FFF) + 0x1000) * 2);
        green = green * level >> 8;
        blue = blue * level >> 8;
        cosine = (sint32)(cosine << 16) >> 20;
        published->pitch.points[2].axis[1] = (uint16)(sine);
        published->pitch.points[1].axis[1] = (uint16)(sine);
        published->pitch.points[0].axis[1] = (uint16)(sine);
        published->pitch.points[2].z = (uint16)(cosine);
        published->pitch.points[1].z = (uint16)(cosine);
        published->pitch.points[0].z = (uint16)(cosine);
        value = (sint16)pitch_step;
        if (value == 0)
        {
            magnitude = (sint32)pitch > 0 ? pitch : 0 - pitch;
            if ((sint32)magnitude < 445)
                magnitude = 445;
            base = 0xFFFFFF9C;
            published->pitch.points[0].axis[0] = (uint16)(base - (uint32)((sint32)magnitude >> 2));
            value = (sint32)magnitude >> 1;
        }
        else
        {
            magnitude = value - pitch;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            base = 0xFFFFFF38;
            published->pitch.points[0].axis[0] = (uint16)(base - (uint32)((sint32)magnitude >> 4));
            value = (sint32)magnitude >> 3;
        }
        value = base - value;
        published->pitch.points[1].axis[0] = (uint16)(value);
        value = published->pitch.points[1].axis[0];
        published->pitch.points[2].axis[0] = (uint16)(value + 20);
        flags = hud_pitch_project_line(packet, published);
        if ((sint32)flags >= 0)
        {
            line = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
            line->rgbc.r = (uint8)(red);
            line->rgbc.g = (uint8)(green);
            line->rgbc.b = (uint8)(blue);
            count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
            packet += 16;
            hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
        }
        a = published->pitch.points[0].axis[0];
        b = published->pitch.points[1].axis[0];
        b = 0 - b;
        published->pitch.points[1].axis[0] = (uint16)(b);
        published->pitch.points[0].axis[0] = (uint16)(0 - a);
        published->pitch.points[2].axis[0] = (uint16)(b - 20);
        flags = hud_pitch_project_line(packet, published);
        if ((sint32)flags >= 0)
        {
            line = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
            line->rgbc.r = (uint8)(red);
            line->rgbc.g = (uint8)(green);
            line->rgbc.b = (uint8)(blue);
            count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
            packet += 16;
            hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
        }
        value = pitch_step;
        pitch_step = (value + 227);
    }
    pitch_step = (pitch);
    published->pitch.points[0].axis[0] = (uint16)(0xFF30);
    published->pitch.points[1].axis[0] = (uint16)(0xFF40);
    published->pitch.points[2].axis[0] = (uint16)(0xFF68);
    raw = pitch_step;
    sine = game_wave_at((raw & 0x3FFF) * 2);
    raw = pitch_step;
    cosine = (uint16)game_wave_at(((raw & 0x3FFF) + 0x1000) * 2);
    sine = (sint32)(0 - sine) >> 4;
    cosine = (sint32)(cosine << 16) >> 20;
    published->pitch.points[0].axis[1] = (uint16)(sine);
    published->pitch.points[0].z = (uint16)(cosine);
    value = published->pitch.points[0].z;
    published->pitch.points[1].axis[1] = (uint16)(sine);
    published->pitch.points[2].axis[1] = (uint16)(sine);
    published->pitch.points[1].z = (uint16)(value);
    published->pitch.points[2].z = (uint16)(value);
    flags = hud_pitch_project_line(packet, published);
    if ((sint32)flags >= 0)
    {
        value = hud_control.primary_color[0];
        line = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        line->rgbc.r = (uint8)(value);
        value = hud_control.primary_color[1];
        line->rgbc.g = (uint8)(value);
        value = hud_control.primary_color[2];
        line->rgbc.b = (uint8)(value);
        count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
        packet += 16;
        hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
    }
    first_flag = flags;
    xy[0] = published->pitch.screen;
    a = published->pitch.points[0].axis[0];
    b = published->pitch.points[1].axis[0];
    published->pitch.points[0].axis[0] = (uint16)(0 - a);
    c = published->pitch.points[2].axis[0];
    published->pitch.points[1].axis[0] = (uint16)(0 - b);
    published->pitch.points[2].axis[0] = (uint16)(0 - c);
    flags = hud_pitch_project_line(packet, published);
    if ((sint32)flags >= 0)
    {
        value = hud_control.primary_color[0];
        line = hud_control.flight_packets.native && (packet - hud_control.flight_packets.cur - 984u) < sizeof(hud_control.flight_packets.native->lines) && (packet - hud_control.flight_packets.cur - 984u) % sizeof(LINE_F2) == 0 ? &hud_control.flight_packets.native->lines[(packet - hud_control.flight_packets.cur - 984u) / sizeof(LINE_F2)] : (LINE_F2 *)mission_packet_span(packet, sizeof(*line));
        line->rgbc.r = (uint8)(value);
        value = hud_control.primary_color[1];
        line->rgbc.g = (uint8)(value);
        value = hud_control.primary_color[2];
        line->rgbc.b = (uint8)(value);
        count = hud_control.flight_packets.used[HUD_FLIGHT_LINES];
        hud_control.flight_packets.used[HUD_FLIGHT_LINES] = count + 1;
    }
    xy[1] = published->pitch.screen;
    published->pitch.pitch = angles[0];
    published->pitch.roll = angles[2];
    if ((sint32)(first_flag | flags) < 0)
        return;
    count = hud_control.flight_packets.used[HUD_FLIGHT_DIGITS];
    storage = hud_control.flight_packets.cur;
    digits = hud_control.flight_packets.native->digits;
    packet = storage + count * 20 + 2208;
    degree = (sint32)(pitch * 360) / 16384;
    if ((sint32)degree < 0)
    {
        for (side = 0; side < 2; ++side)
        {
            x = (sint16)xy[side];
            y = (sint32)xy[side] >> 16;
            color = hud_control.number_color;
            hud_digit_draw(0xFFFFFFFF, x - 9, y - 3, 3, &digits[(packet - storage - 2208) / sizeof(SPRT)], color);
            count = hud_control.flight_packets.used[HUD_FLIGHT_DIGITS];
            packet += 20;
            hud_control.flight_packets.used[HUD_FLIGHT_DIGITS] = count + 1;
        }
        degree = 0 - degree;
    }
    tens = (sint32)degree / 10;
    tens %= 10;
    for (i = 0; i < 4; ++i)
    {
        side = i & 1;
        digit = i < 2 ? tens : degree % 10;
        x = (sint16)xy[side];
        y = (sint32)xy[side] >> 16;
        color = hud_control.number_color;
        hud_digit_draw(digit, x + (i < 2 ? 0xFFFFFFFC : 1), y - 3, 3, &digits[(packet - storage - 2208) / sizeof(SPRT)], color);
        count = hud_control.flight_packets.used[HUD_FLIGHT_DIGITS];
        packet += 20;
        hud_control.flight_packets.used[HUD_FLIGHT_DIGITS] = count + 1;
    }
    return;
}

void hud_initialize_mission(void)
{
    FUNCTION_MARKER(0x8003A008, "ACE3.BIN");
    hud_control.display_ticks = (uint32)(0);
    hud_control.selected_target_idx = (uint8)(255);
    hud_control.locked_target_idx = 255;
    hud_control.held_target_idx = 255;
    hud_control.alert_mode = 0;
    hud_control.alert_ticks = 0;
    hud_control.aim_distance_parameter = (uint32)(3200);
    hud_initialize_flight_packet_banks();
    hud_instruments_init_banks();
    hud_init_packet_banks();
    hud_markers_init();
    hud_notice_init();
    {
        (void)(hud_target_init());
        return;
    }
}

void hud_initialize_flight_packet_banks(void)
{
    uint32 group, index, block, packet, value;
    HUD_FLIGHT_PACKET_BANK *bank;
    FUNCTION_MARKER(0x80063AF0, "ACE3.BIN");
    (void)arena_alloc(40);
    if (!gpu_register_packet_range(&hud_gpu_storage, sizeof(hud_gpu_storage)))
        abort();
    for (group = 0; group < 2; ++group)
    {
        (void)arena_alloc(sizeof(*bank));
        block = gpu_packet_link((uintptr_t)&hud_gpu_storage.flight[group]);
        bank = &hud_gpu_storage.flight[group];
        hud_control.flight_packets.cur = block;
        hud_control.flight_packets.bank[group] = block;
        hud_control.flight_packets.bank_data[group] = bank;
        hud_control.flight_packets.native = bank;
        for (index = 0; index < 16; ++index)
        {
            bank->flat_quads[index].tag = (bank->flat_quads[index].tag & 0xFFFFFF) | ((uint32)(5) << 24);
            bank->flat_quads[index].rgbc.code = (uint8)(42);
        }
        block = hud_control.flight_packets.cur;
        for (index = 0; index < 4; ++index)
        {
            bank->triangles[index].tag = (bank->triangles[index].tag & 0xFFFFFF) | ((uint32)(6) << 24);
            bank->triangles[index].p[0].rgbc.code = (uint8)(50);
        }
        block = hud_control.flight_packets.cur;
        for (index = 0; index < 12; ++index)
        {
            bank->textured_quads[index].tag = (bank->textured_quads[index].tag & 0xFFFFFF) | ((uint32)(9) << 24);
            bank->textured_quads[index].rgbc.code = (uint8)(46);
        }
        for (index = 0; index < 64; ++index)
        {
            bank->lines[index].tag = (bank->lines[index].tag & 0xFFFFFF) | ((uint32)(3) << 24);
            bank->lines[index].rgbc.code = (uint8)(64);
            packet = hud_control.flight_packets.cur + index * 16 + 984;
            bank->lines[index].rgbc.code = (uint8)(bank->lines[index].rgbc.code | 2);
        }
        for (index = 0; index < 8; ++index)
        {
            bank->gradient_lines[index].tag = (bank->gradient_lines[index].tag & 0xFFFFFF) | ((uint32)(4) << 24);
            bank->gradient_lines[index].p[0].rgbc.code = (uint8)(80);
            packet = hud_control.flight_packets.cur + index * 20 + 2008;
            bank->gradient_lines[index].p[0].rgbc.code = (uint8)(bank->gradient_lines[index].p[0].rgbc.code | 2);
        }
        bank->waveform.tag = (bank->waveform.tag & 0xFFFFFF) | ((uint32)(9) << 24);
        bank->waveform.p[0].rgbc.code = (uint8)(92);
        packet = hud_control.flight_packets.cur + 2168;
        bank->waveform.p[2].rgbc.code = (uint8)(0);
        block = hud_control.flight_packets.cur;
        bank->waveform.pad = 0x55555555;
        bank->waveform.p[3].rgbc.code = (uint8)(0);
        packet = hud_control.flight_packets.cur + 2168;
        bank->waveform.p[0].rgbc.code = (uint8)(bank->waveform.p[0].rgbc.code | 2);
        for (index = 0; index < 4; ++index)
        {
            packet = hud_control.flight_packets.cur + 2388 + index * 32;
            bank->markers[index].tag = (bank->markers[index].tag & 0xFFFFFF) | ((uint32)(7) << 24);
            bank->markers[index].rgbc.code = (uint8)(36);
            value = bank->markers[index].rgbc.code;
            bank->markers[index].rgbc.r = (uint8)(67);
            bank->markers[index].rgbc.g = (uint8)(19);
            bank->markers[index].p[1].attr = (uint16)(54);
            bank->markers[index].p[0].attr = (uint16)(32664);
            bank->markers[index].p[0].uv.u = (uint8)(138);
            bank->markers[index].p[2].uv.v = (uint8)(53);
            bank->markers[index].rgbc.b = (uint8)(0);
            bank->markers[index].p[0].uv.v = (uint8)(33);
            bank->markers[index].p[1].uv.u = (uint8)(157);
            bank->markers[index].p[1].uv.v = (uint8)(33);
            bank->markers[index].p[2].uv.u = (uint8)(157);
            bank->markers[index].rgbc.code = (uint8)(value | 2);
            if (index != 3)
            {
                value = bank->markers[index].tag;
                bank->markers[index].tag = (value & 0xFF000000) | ((packet + 32) & 0xFFFFFF);
            }
        }
        block = hud_control.flight_packets.cur;
        for (index = 0; index < 8; ++index)
        {
            bank->digits[index].tag = (bank->digits[index].tag & 0xFFFFFF) | ((uint32)(4) << 24);
            bank->digits[index].rgbc.code = (uint8)(102);
            bank->digits[index].p[0].attr = (uint16)(32664);
        }
        bank->indicator.tag = (bank->indicator.tag & 0xFFFFFF) | ((uint32)(4) << 24);
        bank->indicator.rgbc.code = (uint8)(100);
        bank->indicator.p[0].uv.u = (uint8)(236);
        bank->indicator.p[0].uv.v = (uint8)(140);
        block = hud_control.flight_packets.cur;
        value = bank->indicator.rgbc.code;
        bank->indicator.h = (uint16)(20);
        bank->indicator.w = (uint16)(20);
        bank->indicator.rgbc.code = (uint8)(value | 2);
        packet = hud_control.flight_packets.cur;
        bank->mode.tag = (bank->mode.tag & 0xFFFFFF) | ((uint32)(1) << 24);
        bank->mode.code[0] = 0xE1000236;
    }
    return;
}
