#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint8 untouched_prefix[10];
    uint16 marker;
    uint8 untouched_tail[4];
} MISSION_GRAPH_SETUP_PUBLICATION;

typedef char mission_graph_setup_publication_layout[sizeof(MISSION_GRAPH_SETUP_PUBLICATION) == 16 && offsetof(MISSION_GRAPH_SETUP_PUBLICATION, marker) == 10 ? 1 : -1];

static ARCHIVE_BROWSER_PACKETS archive_browser_packets;
static MISSION_WEAPON_PACKET_BANKS weapon_packet_storage;

static const uint8 archive_unlock_missions[62][4] = {
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {9, 0, 0, 0}, {0, 0, 0, 0}, {15, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {31, 38, 45, 0}, {38, 0, 0, 0}, {0, 0, 0, 0}, {48, 53, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {38, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {13, 0, 0, 0}, {11, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {14, 45, 0, 0}, {9, 14, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {14, 45, 0, 0}, {0, 0, 0, 0}, {14, 45, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {38, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {27, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 0, 0, 0}, {0, 0, 0, 0}, {58, 0, 0, 0}, {38, 53, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
};

typedef char debrief_map_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && offsetof(POLY_FT4, rgbc) == 4 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(LINE_F2) == 16 && offsetof(LINE_F2, p) == 8 ? 1 : -1];

typedef struct
{
    uint8 track;
    uint8 graphics;
} DEBRIEF_VARIANT;

static const DEBRIEF_VARIANT debrief_variants[] = {
    {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0},  {0, 0}, {0, 0}, {1, 0}, {1, 0}, {0, 0}, {0, 0}, {2, 0}, {0, 0}, {0, 0}, {0, 0}, {3, 0},  {3, 0}, {0, 0}, {0, 0}, {4, 0}, {4, 0}, {0, 0}, {0, 0}, {5, 0}, {5, 0}, {0, 0}, {0, 0}, {6, 0}, {0, 0}, {0, 0}, {0, 0}, {7, 0}, {0, 0}, {0, 0}, {0, 0}, {8, 0}, {9, 1}, {9, 1}, {0, 0}, {10, 0}, {0, 0}, {0, 0}, {0, 0}, {11, 0}, {11, 0}, {0, 0}, {0, 0}, {12, 0}, {12, 0}, {0, 0}, {0, 0}, {13, 0}, {0, 0},  {0, 0}, {0, 0}, {14, 0}, {0, 0},  {0, 0}, {0, 0}, {15, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {17, 0}, {17, 0}, {0, 0}, {0, 0}, {18, 0}, {0, 0},  {0, 0}, {0, 0}, {18, 0}, {18, 0}, {0, 0}, {0, 0}, {19, 0}, {19, 0}, {20, 1}, {20, 1}, {21, 0}, {0, 0}, {0, 0}, {0, 0}, {22, 0}, {0, 0}, {0, 0}, {0, 0}, {23, 0}, {0, 0}, {0, 0}, {0, 0}, {24, 0}, {24, 0}, {0, 0}, {0, 0}, {25, 0}, {26, 1}, {26, 1}, {0, 0},
    {27, 0}, {0, 0}, {0, 0}, {0, 0}, {28, 0}, {28, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {29, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {30, 0}, {0, 0},  {0, 0}, {0, 0}, {30, 0}, {0, 0},  {0, 0}, {0, 0}, {31, 0}, {31, 0}, {0, 0}, {0, 0}, {32, 0}, {33, 1}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {34, 0}, {0, 0}, {0, 0}, {0, 0}, {35, 0}, {0, 0},  {0, 0}, {0, 0}, {35, 0}, {35, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0},  {0, 0}, {0, 0}, {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0}, {0, 0}, {0, 0}, {0, 0},  {0, 0},  {0, 0}, {0, 0}, {0, 0},  {0, 0},  {0, 0},  {0, 0},
};

static const DEBRIEF_VARIANT debrief_legacy_dispatch[] = {
    {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0xB4, 0x6C}, {0x0F, 0x80}, {0x7C, 0xD3}, {0x0C, 0x80}, {0x74, 0xD3}, {0x0C, 0x80}, {0x6C, 0xD3}, {0x0C, 0x80}, {0x64, 0xD3}, {0x0C, 0x80}, {0x5C, 0xD3}, {0x0C, 0x80}, {0x54, 0xD3}, {0x0C, 0x80}, {0x4C, 0xD3}, {0x0C, 0x80}, {0x44, 0xD3}, {0x0C, 0x80}, {0x3C, 0xD3}, {0x0C, 0x80}, {0x34, 0xD3}, {0x0C, 0x80}, {0x2C, 0xD3}, {0x0C, 0x80},
};

typedef struct
{
    uint32 card_icons;
    uint32 card_palette;
    uint32 menu_commands;
    uint32 save_commands;
    uint32 pad_profile_directory;
    uint32 save_textures;
    uint32 shared_textures;
    uint32 initial_textures;
    uint32 sound_bank;
} COMMON_MENU_DIRECTORY;

typedef char common_menu_directory_layout[sizeof(COMMON_MENU_DIRECTORY) == 36 ? 1 : -1];

typedef struct
{
    uint16 reserved_0;
    uint16 terrain_flags;
    uint32 palette_secondary;
    uint32 palette_primary;
    sint16 near_plane;
    sint16 far_plane;
    sint16 horizon_height;
    uint16 reserved_18;
    uint32 first_blend_start;
    uint32 first_blend_end;
    uint32 horizon_color;
    sint16 layer_height[2];
    uint32 second_blend_start;
    uint32 second_blend_end;
    uint8 reserved_44[16];
    sint16 first_light_angles[2];
    sint16 first_light_colors[3];
    sint16 ambient_color[3];
    sint16 second_light_angles[2];
    sint16 second_light_colors[3];
    uint8 reserved_86[22];
    sint16 flare_flags;
    uint8 layer_enabled;
    uint8 detail_kind;
    uint32 halo_color;
} MISSION_RENDER_RECORD;

typedef struct
{
    uint8 reserved_0[4];
    uint16 camera_offsets[3];
    uint8 reserved_10[114];
    uint16 effect_lines[4];
    uint32 effect_trails[2];
    uint32 effect_center;
    uint32 spawn_vector[2];
    uint32 weapon_vectors[4];
} AIRCRAFT_MISSION_RECORD;

typedef struct
{
    uint32 xy;
    uint32 z_axis;
} AIRCRAFT_THRUST_VERTEX_RECORD;

typedef struct
{
    AIRCRAFT_THRUST_VERTEX_RECORD vertices[4];
} AIRCRAFT_THRUST_GEOMETRY_RECORD;

typedef struct
{
    uint32 direction_xy;
    uint16 direction_z;
    uint16 reserved_6;
    uint32 color_xy;
    uint16 color_z;
    uint16 reserved_14;
} AIRCRAFT_THRUST_STYLE_RECORD;

typedef struct
{
    uint8 reserved_0[12];
    uint8 count;
    uint8 uses_axes;
    uint16 reserved_14;
    uint32 base_rgbc;
    uint32 far_color;
    AIRCRAFT_THRUST_GEOMETRY_RECORD geometry[2];
    AIRCRAFT_THRUST_STYLE_RECORD style[2];
    uint32 extent_xy;
} AIRCRAFT_THRUST_RECORD;

typedef struct
{
    sint16 height;
    uint8 count;
    uint8 slope;
} MISSION_ENVIRONMENT_LAYER_RECORD;

typedef struct
{
    uint32 reserved_0;
    uint8 flags;
    uint8 reserved_5;
    sint16 horizon_height;
    MISSION_ENVIRONMENT_LAYER_RECORD layer[2];
    uint32 drift_noise;
    uint32 layer_color;
    uint32 horizon_color;
    uint8 detail_repeat;
    uint8 detail_count;
    sint16 detail_density;
    uint8 trail_back_color[3];
} MISSION_ENVIRONMENT_RECORD;

typedef char asset_metadata_record_layout[sizeof(MISSION_RENDER_RECORD) == 116 && sizeof(AIRCRAFT_MISSION_RECORD) == 168 && sizeof(AIRCRAFT_THRUST_RECORD) == 124 && sizeof(MISSION_ENVIRONMENT_RECORD) == 36 && offsetof(MISSION_RENDER_RECORD, reserved_0) == 0 && offsetof(MISSION_RENDER_RECORD, terrain_flags) == 2 && offsetof(MISSION_RENDER_RECORD, palette_secondary) == 4 && offsetof(MISSION_RENDER_RECORD, palette_primary) == 8 && offsetof(MISSION_RENDER_RECORD, near_plane) == 12 && offsetof(MISSION_RENDER_RECORD, far_plane) == 14 && offsetof(MISSION_RENDER_RECORD, horizon_height) == 16 && offsetof(MISSION_RENDER_RECORD, reserved_18) == 18 && offsetof(MISSION_RENDER_RECORD, first_blend_start) == 20 && offsetof(MISSION_RENDER_RECORD, first_blend_end) == 24 && offsetof(MISSION_RENDER_RECORD, horizon_color) == 28 && offsetof(MISSION_RENDER_RECORD, layer_height) == 32 && offsetof(MISSION_RENDER_RECORD, second_blend_start) == 36 && offsetof(MISSION_RENDER_RECORD, second_blend_end) == 40 &&
                                                  offsetof(MISSION_RENDER_RECORD, reserved_44) == 44 && offsetof(MISSION_RENDER_RECORD, first_light_angles) == 60 && offsetof(MISSION_RENDER_RECORD, first_light_colors) == 64 && offsetof(MISSION_RENDER_RECORD, ambient_color) == 70 && offsetof(MISSION_RENDER_RECORD, second_light_angles) == 76 && offsetof(MISSION_RENDER_RECORD, second_light_colors) == 80 && offsetof(MISSION_RENDER_RECORD, reserved_86) == 86 && offsetof(MISSION_RENDER_RECORD, flare_flags) == 108 && offsetof(MISSION_RENDER_RECORD, layer_enabled) == 110 && offsetof(MISSION_RENDER_RECORD, detail_kind) == 111 && offsetof(MISSION_RENDER_RECORD, halo_color) == 112 && offsetof(AIRCRAFT_MISSION_RECORD, reserved_0) == 0 && offsetof(AIRCRAFT_MISSION_RECORD, camera_offsets) == 4 && offsetof(AIRCRAFT_MISSION_RECORD, reserved_10) == 10 && offsetof(AIRCRAFT_MISSION_RECORD, effect_lines) == 124 && offsetof(AIRCRAFT_MISSION_RECORD, effect_trails) == 132 &&
                                                  offsetof(AIRCRAFT_MISSION_RECORD, effect_center) == 140 && offsetof(AIRCRAFT_MISSION_RECORD, spawn_vector) == 144 && offsetof(AIRCRAFT_MISSION_RECORD, weapon_vectors) == 152 && offsetof(AIRCRAFT_THRUST_VERTEX_RECORD, xy) == 0 && offsetof(AIRCRAFT_THRUST_VERTEX_RECORD, z_axis) == 4 && offsetof(AIRCRAFT_THRUST_GEOMETRY_RECORD, vertices) == 0 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, direction_xy) == 0 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, direction_z) == 4 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, reserved_6) == 6 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, color_xy) == 8 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, color_z) == 12 && offsetof(AIRCRAFT_THRUST_STYLE_RECORD, reserved_14) == 14 && offsetof(AIRCRAFT_THRUST_RECORD, reserved_0) == 0 && offsetof(AIRCRAFT_THRUST_RECORD, count) == 12 && offsetof(AIRCRAFT_THRUST_RECORD, uses_axes) == 13 && offsetof(AIRCRAFT_THRUST_RECORD, reserved_14) == 14 &&
                                                  offsetof(AIRCRAFT_THRUST_RECORD, base_rgbc) == 16 && offsetof(AIRCRAFT_THRUST_RECORD, far_color) == 20 && offsetof(AIRCRAFT_THRUST_RECORD, geometry) == 24 && offsetof(AIRCRAFT_THRUST_RECORD, style) == 88 && offsetof(AIRCRAFT_THRUST_RECORD, extent_xy) == 120 && offsetof(MISSION_ENVIRONMENT_LAYER_RECORD, height) == 0 && offsetof(MISSION_ENVIRONMENT_LAYER_RECORD, count) == 2 && offsetof(MISSION_ENVIRONMENT_LAYER_RECORD, slope) == 3 && offsetof(MISSION_ENVIRONMENT_RECORD, reserved_0) == 0 && offsetof(MISSION_ENVIRONMENT_RECORD, flags) == 4 && offsetof(MISSION_ENVIRONMENT_RECORD, reserved_5) == 5 && offsetof(MISSION_ENVIRONMENT_RECORD, horizon_height) == 6 && offsetof(MISSION_ENVIRONMENT_RECORD, layer) == 8 && offsetof(MISSION_ENVIRONMENT_RECORD, drift_noise) == 16 && offsetof(MISSION_ENVIRONMENT_RECORD, layer_color) == 20 && offsetof(MISSION_ENVIRONMENT_RECORD, horizon_color) == 24 &&
                                                  offsetof(MISSION_ENVIRONMENT_RECORD, detail_repeat) == 28 && offsetof(MISSION_ENVIRONMENT_RECORD, detail_count) == 29 && offsetof(MISSION_ENVIRONMENT_RECORD, detail_density) == 30 && offsetof(MISSION_ENVIRONMENT_RECORD, trail_back_color) == 32
                                              ? 1
                                              : -1];

typedef struct
{
    uint8 *data;
    uint32 bytes;
} ASSET_SECTION_VIEW;

typedef struct
{
    uint32 render_settings;
    uint32 tile_map;
    uint32 terrain_models;
    uint32 models;
    uint32 extended_models;
    uint32 animated_models;
    uint32 normals;
    uint32 speech;
    uint32 entities;
    uint32 environment;
    uint32 restore;
    uint32 texture_groups[5];
    uint32 sound_bank;
} MISSION_ASSET_DIRECTORY;

typedef char mission_asset_directory_layout[sizeof(MISSION_ASSET_DIRECTORY) == 68 && offsetof(MISSION_ASSET_DIRECTORY, render_settings) == 0 && offsetof(MISSION_ASSET_DIRECTORY, tile_map) == 4 && offsetof(MISSION_ASSET_DIRECTORY, terrain_models) == 8 && offsetof(MISSION_ASSET_DIRECTORY, models) == 12 && offsetof(MISSION_ASSET_DIRECTORY, extended_models) == 16 && offsetof(MISSION_ASSET_DIRECTORY, animated_models) == 20 && offsetof(MISSION_ASSET_DIRECTORY, normals) == 24 && offsetof(MISSION_ASSET_DIRECTORY, speech) == 28 && offsetof(MISSION_ASSET_DIRECTORY, entities) == 32 && offsetof(MISSION_ASSET_DIRECTORY, environment) == 36 && offsetof(MISSION_ASSET_DIRECTORY, restore) == 40 && offsetof(MISSION_ASSET_DIRECTORY, texture_groups) == 44 && offsetof(MISSION_ASSET_DIRECTORY, sound_bank) == 64 ? 1 : -1];

typedef struct
{
    uint32 models;
    uint32 metadata;
    uint32 textures;
    uint32 sound_bank;
} AIRCRAFT_ASSET_DIRECTORY;

typedef char aircraft_asset_directory_layout[sizeof(AIRCRAFT_ASSET_DIRECTORY) == 16 && offsetof(AIRCRAFT_ASSET_DIRECTORY, models) == 0 && offsetof(AIRCRAFT_ASSET_DIRECTORY, metadata) == 4 && offsetof(AIRCRAFT_ASSET_DIRECTORY, textures) == 8 && offsetof(AIRCRAFT_ASSET_DIRECTORY, sound_bank) == 12 ? 1 : -1];

static const uint8 debrief_sound_ids[4] = {86, 84, 85, 87};

typedef struct
{
    uint32 page_count;
    uint8 widget_codes[4][2];
} ARCHIVE_DETAIL_ENTRY;

static const ARCHIVE_DETAIL_ENTRY archive_details[] = {
    {0, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {3, {{1, 0}, {1, 0}, {1, 0}, {0, 0}}}, {2, {{1, 2}, {1, 2}, {0, 0}, {0, 0}}}, {3, {{1, 0}, {1, 0}, {1, 0}, {0, 0}}}, {3, {{1, 0}, {1, 0}, {1, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {0, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {3, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {0, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {0, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}}, {1, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {1, {{1, 0}, {0, 0}, {0, 0}, {0, 0}}}, {2, {{1, 0}, {1, 0}, {0, 0}, {0, 0}}},
};

static const sint16 aircraft_resource_ids[GAME_AIRCRAFT_COUNT] = {
    66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
};

static void mission_transition_wait_ring(void);
static void mission_transition_finish_ring(void);

typedef struct
{
    sint16 resource_id;
    uint8 loading_mode;
} MISSION_LOAD_SPEC;

static const MISSION_LOAD_SPEC mission_load_specs[GAME_MISSION_COUNT] = {
    {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 1}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 1}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 1}, {28, 0}, {29, 0}, {30, 1}, {31, 0}, {32, 0}, {33, 0}, {34, 0}, {35, 0}, {36, 0}, {37, 1}, {38, 1}, {39, 0}, {40, 0}, {41, 1}, {42, 0}, {43, 1}, {44, 0}, {45, 1}, {46, 0}, {47, 0}, {48, 0}, {49, 0}, {50, 0}, {51, 1}, {52, 1}, {53, 1}, {54, 0}, {55, 0}, {56, 0}, {57, 0}, {58, 1}, {59, 0}, {60, 1}, {61, 1}, {62, 1}, {63, 2}, {64, 0},
};

const MISSION_SCENARIO mission_scenarios[MISSION_SCENARIO_COUNT] = {
    {1, {0, 0}, 255}, {0, {0, 0}, 255}, {7, {4, 2}, 4}, {5, {2, 0}, 255}, {7, {2, 0}, 2}, {0, {0, 0}, 255}, {2, {2, 0}, 2}, {2, {3, 0}, 3}, {0, {0, 0}, 255}, {2, {2, 0}, 2}, {0, {0, 0}, 255}, {1, {0, 0}, 255}, {3, {2, 0}, 2}, {7, {3, 4}, 3}, {6, {2, 0}, 255}, {0, {0, 0}, 255}, {5, {0, 0}, 255}, {5, {2, 3}, 255}, {0, {0, 0}, 255}, {4, {0, 0}, 255}, {3, {2, 0}, 2}, {1, {0, 0}, 255}, {7, {3, 4}, 3}, {5, {2, 0}, 255},
};

MISSION_FLOW mission_flow;
MISSION_SAVE_MENU mission_save_menu;
MISSION_RESOURCE_LOAD mission_resource_load;
MISSION_SELECTOR mission_selector;
const char mission_debrief_labels[MISSION_DEBRIEF_LABEL_COUNT][MISSION_DEBRIEF_LABEL_BYTES] = {
    "NON", "F?A;18I", "F;16XF", "F;16XA", "F;15S?MT", "A?F;117X", "F;22C", "F?A;32C", "XFA;36A", "RF;12A2", "EF2000E", "MiG;33", "F?A;18U", "F;16XFU", "R;101U", "R;201U", "Su;37", "Su;43", "R;101", "R;201", "R;102", "R;211", "R;103", "R;311", "R;352", "XR;900", "X;49", "UI;4054", "XFA;36A", "Su;43", "R;103", "KC777", "RF;12A2", "C;17B", "B;1C", "AH;66B", "V;22B", "EC;17U", "R;505U", "R;352", "R;501", "R;531", "R;808", "R;701", "UI;4052", "UI;4053", "IRBM", "SAM", "AIR TGT", "GROUND TGT", "GUN", "MSSL", "NON TGT",
};

static const uint8 resource002_failed_mission[GAME_MISSION_COUNT] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 48, 50, 51, 52, 53, 34, 55, 42, 37, 39, 59,
};

MISSION_DEBRIEF mission_debrief;
MISSION_AIRCRAFT mission_aircraft;

const AIRCRAFT_PROFILE aircraft_fallback_profile = {0};

enum
{
    AIRCRAFT_LOADOUT_COUNT = 37,
    AIRCRAFT_WEAPON_SLOT_COUNT = 4
};

typedef struct
{
    uint8 primary_count;
    uint8 secondary_count;
    uint8 primary_default;
    uint8 secondary_default;
    uint8 primary_weapons[AIRCRAFT_WEAPON_SLOT_COUNT];
    uint8 secondary_weapons[AIRCRAFT_WEAPON_SLOT_COUNT];
} AIRCRAFT_LOADOUT;

static const AIRCRAFT_LOADOUT loadouts[AIRCRAFT_LOADOUT_COUNT] = {
    {1, 1, 0, 1, {0, 0, 0, 0}, {1, 0, 0, 0}}, {2, 2, 0, 3, {0, 1, 0, 0}, {2, 3, 0, 0}}, {2, 1, 1, 1, {0, 1, 0, 0}, {1, 0, 0, 0}}, {2, 1, 0, 3, {0, 1, 0, 0}, {3, 0, 0, 0}}, {2, 3, 0, 1, {0, 2, 0, 0}, {1, 2, 4, 0}}, {2, 1, 2, 3, {0, 2, 0, 0}, {3, 0, 0, 0}}, {2, 1, 1, 1, {0, 1, 0, 0}, {1, 0, 0, 0}}, {2, 2, 0, 3, {0, 2, 0, 0}, {2, 3, 0, 0}}, {3, 3, 2, 1, {0, 1, 2, 0}, {1, 2, 4, 0}}, {2, 3, 1, 1, {0, 1, 0, 0}, {1, 2, 4, 0}}, {1, 1, 0, 1, {0, 0, 0, 0}, {1, 0, 0, 0}}, {1, 1, 0, 1, {0, 0, 0, 0}, {1, 0, 0, 0}}, {2, 2, 0, 3, {0, 1, 0, 0}, {2, 3, 0, 0}}, {2, 1, 1, 1, {0, 1, 0, 0}, {1, 0, 0, 0}}, {1, 1, 1, 1, {1, 0, 0, 0}, {1, 0, 0, 0}}, {1, 3, 2, 3, {2, 0, 0, 0}, {2, 3, 4, 0}}, {2, 1, 0, 1, {0, 2, 0, 0}, {1, 0, 0, 0}}, {3, 2, 2, 1, {0, 1, 2, 0}, {1, 4, 0, 0}}, {1, 1, 1, 1, {1, 0, 0, 0}, {1, 0, 0, 0}},
    {1, 3, 2, 3, {2, 0, 0, 0}, {2, 3, 4, 0}}, {2, 1, 3, 1, {1, 3, 0, 0}, {1, 0, 0, 0}}, {2, 2, 3, 3, {2, 3, 0, 0}, {2, 3, 0, 0}}, {3, 2, 3, 1, {1, 2, 3, 0}, {1, 4, 0, 0}}, {2, 3, 3, 3, {1, 3, 0, 0}, {2, 3, 4, 0}}, {1, 1, 5, 5, {5, 0, 0, 0}, {5, 0, 0, 0}}, {1, 3, 4, 4, {4, 0, 0, 0}, {2, 3, 4, 0}}, {1, 3, 4, 4, {4, 0, 0, 0}, {2, 3, 4, 0}}, {3, 3, 2, 4, {0, 1, 2, 0}, {2, 3, 4, 0}}, {3, 3, 2, 1, {0, 1, 2, 0}, {1, 2, 4, 0}}, {3, 2, 2, 1, {0, 1, 2, 0}, {1, 4, 0, 0}}, {3, 2, 3, 1, {1, 2, 3, 0}, {1, 4, 0, 0}}, {2, 3, 0, 1, {0, 2, 0, 0}, {1, 2, 4, 0}}, {2, 1, 1, 1, {0, 1, 0, 0}, {1, 0, 0, 0}}, {2, 1, 0, 1, {0, 2, 0, 0}, {1, 0, 0, 0}}, {2, 1, 3, 1, {1, 3, 0, 0}, {1, 0, 0, 0}}, {3, 2, 3, 1, {1, 2, 3, 0}, {1, 4, 0, 0}}, {2, 3, 3, 3, {1, 3, 0, 0}, {2, 3, 4, 0}},
};

enum
{
    AIRCRAFT_MODEL_GROUP_COUNT = 5
};

static const uint8 model_group_offsets[AIRCRAFT_MODEL_GROUP_COUNT] = {10, 1, 18, 26, 31};

static const uint8 model_group_ends[AIRCRAFT_MODEL_GROUP_COUNT] = {18, 10, 26, 31, 37};

enum
{
    MISSION_AIRCRAFT_SET_COUNT = 60,
    MISSION_AIRCRAFT_SLOT_COUNT = 7
};

typedef struct
{
    uint32 group;
    uint8 count;
    uint8 aircraft[MISSION_AIRCRAFT_SLOT_COUNT];
} MISSION_AIRCRAFT_SET;

static const MISSION_AIRCRAFT_SET mission_aircraft_sets[MISSION_AIRCRAFT_SET_COUNT] = {
    {1, 2, {3, 4, 0, 0, 0, 0, 0}},  {0, 2, {10, 11, 0, 0, 0, 0, 0}}, {0, 2, {10, 11, 0, 0, 0, 0, 0}}, {0, 4, {10, 11, 12, 13, 0, 0, 0}}, {0, 4, {10, 11, 12, 13, 0, 0, 0}}, {0, 4, {10, 11, 12, 13, 0, 0, 0}}, {0, 6, {10, 11, 12, 13, 14, 15, 0}}, {0, 6, {10, 11, 12, 13, 14, 15, 0}}, {0, 6, {10, 11, 12, 13, 14, 15, 0}}, {0, 6, {10, 11, 12, 13, 14, 15, 0}}, {0, 7, {10, 11, 12, 13, 14, 15, 16}}, {0, 7, {10, 11, 12, 13, 14, 15, 16}}, {0, 7, {10, 11, 12, 13, 14, 15, 16}}, {0, 7, {10, 11, 12, 13, 14, 15, 16}}, {2, 1, {21, 0, 0, 0, 0, 0, 0}}, {0, 1, {17, 0, 0, 0, 0, 0, 0}}, {0, 1, {17, 0, 0, 0, 0, 0, 0}}, {0, 1, {17, 0, 0, 0, 0, 0, 0}}, {0, 1, {17, 0, 0, 0, 0, 0, 0}}, {0, 1, {17, 0, 0, 0, 0, 0, 0}},
    {1, 2, {3, 4, 0, 0, 0, 0, 0}},  {1, 2, {3, 4, 0, 0, 0, 0, 0}},   {1, 1, {5, 0, 0, 0, 0, 0, 0}},   {1, 2, {3, 4, 0, 0, 0, 0, 0}},     {1, 4, {3, 4, 6, 7, 0, 0, 0}},     {1, 1, {9, 0, 0, 0, 0, 0, 0}},     {1, 4, {3, 4, 6, 7, 0, 0, 0}},       {1, 4, {3, 4, 6, 7, 0, 0, 0}},       {1, 4, {3, 4, 6, 7, 0, 0, 0}},       {1, 4, {3, 4, 6, 7, 0, 0, 0}},       {1, 1, {8, 0, 0, 0, 0, 0, 0}},        {1, 1, {8, 0, 0, 0, 0, 0, 0}},        {1, 1, {8, 0, 0, 0, 0, 0, 0}},        {1, 1, {8, 0, 0, 0, 0, 0, 0}},        {1, 1, {8, 0, 0, 0, 0, 0, 0}},  {3, 1, {28, 0, 0, 0, 0, 0, 0}}, {3, 1, {28, 0, 0, 0, 0, 0, 0}}, {3, 1, {28, 0, 0, 0, 0, 0, 0}}, {3, 1, {28, 0, 0, 0, 0, 0, 0}}, {3, 1, {28, 0, 0, 0, 0, 0, 0}},
    {2, 1, {23, 0, 0, 0, 0, 0, 0}}, {2, 2, {20, 21, 0, 0, 0, 0, 0}}, {2, 1, {24, 0, 0, 0, 0, 0, 0}},  {2, 2, {20, 21, 0, 0, 0, 0, 0}},   {2, 1, {22, 0, 0, 0, 0, 0, 0}},    {2, 1, {22, 0, 0, 0, 0, 0, 0}},    {2, 1, {22, 0, 0, 0, 0, 0, 0}},      {2, 1, {22, 0, 0, 0, 0, 0, 0}},      {2, 1, {22, 0, 0, 0, 0, 0, 0}},      {2, 1, {22, 0, 0, 0, 0, 0, 0}},      {2, 1, {22, 0, 0, 0, 0, 0, 0}},       {2, 1, {22, 0, 0, 0, 0, 0, 0}},       {2, 1, {22, 0, 0, 0, 0, 0, 0}},       {2, 1, {22, 0, 0, 0, 0, 0, 0}},       {2, 0, {0, 0, 0, 0, 0, 0, 0}},  {2, 0, {22, 0, 0, 0, 0, 0, 0}}, {2, 0, {0, 0, 0, 0, 0, 0, 0}},  {2, 0, {0, 0, 0, 0, 0, 0, 0}},  {2, 0, {0, 0, 0, 0, 0, 0, 0}},  {2, 0, {0, 0, 0, 0, 0, 0, 0}},
};

enum
{
    MISSION_WEAPON_UNLOCK_COUNT = 32
};

static const uint8 mission_weapon_unlocks[MISSION_WEAPON_UNLOCK_COUNT] = {
    8, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7,
};

uint32 mission_aircraft_group(uint32 mission, sint32 aircraft)
{
    uint32 flags;

    FUNCTION_MARKER(0x800FE6E0, "ACE3.BIN");
    flags = game_runtime.progress.flow_flags;
    if (flags & 16)
    {
        if (aircraft < 10)
            return 1;
        if (aircraft < 18)
            return 0;
        if (aircraft < 26)
            return 2;
        if (aircraft < 31)
            return 3;
        return 4;
    }
    if (mission >= MISSION_AIRCRAFT_SET_COUNT)
        mission = 0;
    return mission_aircraft_sets[mission].group;
}

uint32 mission_aircraft_count(uint32 mission, uint32 group)
{
    uint32 flags, first, end, index, count;

    FUNCTION_MARKER(0x800FE764, "ACE3.BIN");
    flags = game_runtime.progress.flow_flags;
    if (!(flags & 16))
    {
        if (mission >= MISSION_AIRCRAFT_SET_COUNT)
            mission = 0;
        return mission_aircraft_sets[mission].count;
    }
    if (group >= AIRCRAFT_MODEL_GROUP_COUNT)
        return 0;
    first = model_group_offsets[group];
    end = model_group_ends[group];
    count = 0;
    for (index = first; index < end; ++index)
        if (game_runtime.progress.aircraft_unlocked[index] == 1)
            ++count;
    return count;
}

uint32 mission_model_variant(uint32 mission, uint32 variant)
{
    uint32 aircraft;
    FUNCTION_MARKER(0x800FE86C, "ACE3.BIN");
    if (game_runtime.progress.flow_flags & 16)
        return 0;
    if (mission >= MISSION_AIRCRAFT_SET_COUNT)
        mission = 0;
    if (variant >= MISSION_AIRCRAFT_SLOT_COUNT)
        variant = 0;
    aircraft = mission_aircraft_sets[mission].aircraft[variant];
    return aircraft_model_index(mission, aircraft);
}

uint32 mission_aircraft_id(uint32 mission, uint32 variant)
{
    FUNCTION_MARKER(0x800FE8D8, "ACE3.BIN");
    if (game_runtime.progress.flow_flags & 16)
    {
        if (variant >= 64)
            variant = 0;
        return (sint16)mission_aircraft.preview[variant];
    }
    if (mission >= MISSION_AIRCRAFT_SET_COUNT)
        mission = 0;
    if (variant >= MISSION_AIRCRAFT_SLOT_COUNT)
        variant = 0;
    return mission_aircraft_sets[mission].aircraft[variant];
}

const AIRCRAFT_PROFILE *aircraft_profile_at(uint32 index)
{
    const MISSION_SCENE_ASSETS *assets = &mission_flow.assets;

    if (index >= assets->aircraft_profile_bytes / sizeof(AIRCRAFT_PROFILE))
        abort();
    return (const AIRCRAFT_PROFILE *)(assets->aircraft_profiles + index * sizeof(AIRCRAFT_PROFILE));
}

uint32 aircraft_model_index(uint32 mission, uint32 aircraft)
{
    uint32 group;
    FUNCTION_MARKER(0x800FE668, "ACE3.BIN");
    group = mission_aircraft_group(mission, (sint32)aircraft);
    if (group >= AIRCRAFT_MODEL_GROUP_COUNT)
        return aircraft;
    return aircraft - model_group_offsets[group];
}

void mission_unlock_aircraft(uint32 mission)
{
    const MISSION_AIRCRAFT_SET *options;
    uint32 count, i, flag;
    FUNCTION_MARKER(0x800FE950, "ACE3.BIN");
    if (mission >= MISSION_AIRCRAFT_SET_COUNT)
        mission = 0;
    options = mission_aircraft_sets + mission;
    count = options->count;
    if (count == 0)
        return;
    for (i = 0; i < count; ++i)
    {
        flag = options->aircraft[i];
        game_runtime.progress.aircraft_unlocked[flag] = 1;
    }
    return;
}

void mission_unlock_weapons(uint32 mission)
{
    const AIRCRAFT_LOADOUT *loadout;
    const MISSION_AIRCRAFT_SET *options;
    uint32 count, i, item, inner_count, j, flag, weapon_id;
    FUNCTION_MARKER(0x800FEC84, "ACE3.BIN");
    if (mission >= MISSION_AIRCRAFT_SET_COUNT)
        mission = 0;
    weapon_id = 0;
    if (mission - 13 < MISSION_WEAPON_UNLOCK_COUNT)
        weapon_id = mission_weapon_unlocks[mission - 13];
    if (weapon_id)
    {
        game_runtime.progress.weapon_unlocked[6 + weapon_id] = 1;
    }
    options = mission_aircraft_sets + mission;
    count = options->count;
    if (count == 0)
        return;
    for (i = 0; i < count; ++i)
    {
        item = options->aircraft[i];
        if (item - 1 < 36)
        {
            loadout = loadouts + item;
            inner_count = loadout->primary_count;
            for (j = 0; j < inner_count; ++j)
            {
                flag = loadout->primary_weapons[j];
                game_runtime.progress.weapon_unlocked[flag] = 1;
            }
            inner_count = loadout->secondary_count;
            for (j = 0; j < inner_count; ++j)
            {
                flag = loadout->secondary_weapons[j];
                game_runtime.progress.weapon_unlocked[6 + flag] = 1;
            }
        }
    }
    return;
}

uint32 aircraft_weapon_id(uint32 mission, uint32 aircraft, uint32 kind, uint32 record)
{
    const AIRCRAFT_LOADOUT *loadout;
    uint32 value;
    FUNCTION_MARKER(0x800FEBB8, "ACE3.BIN");
    if (aircraft >= AIRCRAFT_LOADOUT_COUNT)
        aircraft = 0;
    if (kind == 0)
    {
        if (record >= AIRCRAFT_WEAPON_SLOT_COUNT)
            record = 0;
        return loadouts[aircraft].primary_weapons[record];
    }
    if (mission == 13)
        value = 8;
    else
    {
        loadout = loadouts + aircraft;
        value = loadout->secondary_count;
        if (record == value + 1)
            value = 6;
        else if (record == value)
            value = 7;
        else
        {
            if (record >= AIRCRAFT_WEAPON_SLOT_COUNT)
                record = 0;
            value = loadout->secondary_weapons[record];
        }
    }
    if (kind == 2)
        value += 6;
    return value;
}

uint32 aircraft_weapon_count(uint32 mission, uint32 aircraft, uint32 kind)
{
    const AIRCRAFT_LOADOUT *loadout;
    uint32 value;
    FUNCTION_MARKER(0x800FE9C0, "ACE3.BIN");
    if (aircraft >= AIRCRAFT_LOADOUT_COUNT)
        aircraft = 0;
    loadout = loadouts + aircraft;
    if (kind == 0)
        return loadout->primary_count;
    if (mission == 13)
        return 1;
    value = loadout->secondary_count;
    if (mission == 42)
        return value;
    if (game_runtime.progress.flow_flags & 16)
    {
        ++value;
        if (mission == 32 || mission == 47 || mission == 53 || mission == 55)
            return value;
        if (mission_aircraft.unlock_flags & 8)
            ++value;
    }
    else if (mission == 14 || mission == 28 || mission == 29 || mission == 43 || mission == 44)
        ++value;
    return value;
}

uint32 aircraft_default_weapon_slot(uint32 mission, uint32 aircraft, uint32 kind)
{
    const AIRCRAFT_LOADOUT *loadout;
    uint32 value, index;
    FUNCTION_MARKER(0x800FEAD4, "ACE3.BIN");
    if (aircraft >= AIRCRAFT_LOADOUT_COUNT)
        aircraft = 0;
    loadout = loadouts + aircraft;
    if (kind == 0)
    {
        value = loadout->primary_default;
        for (index = 0; index < AIRCRAFT_WEAPON_SLOT_COUNT; ++index)
            if (loadout->primary_weapons[index] == value)
                return index;
        return 0;
    }
    if (mission == 13)
        return 0;
    if (mission == 14 || mission == 28 || mission == 29 || mission == 43 || mission == 44)
        return loadout->secondary_count;
    value = loadout->secondary_default;
    for (index = 0; index < AIRCRAFT_WEAPON_SLOT_COUNT; ++index)
        if (loadout->secondary_weapons[index] == value)
            return index;
    return 0;
}

static const char *const aircraft_preview_labels[][3] = {
    {"---;---", "---------", "----------------"},
    {"F?A;18I", "HornetADV", "General Resource"},
    {"F;16XF", "Gyrfalcon", "General Resource"},
    {"F;16XA", "Sakerfalcon", "General Resource"},
    {"F;15S?MT", "Eagle Plus", "General Resource"},
    {"A?F;117X", "NAV Hawk", "General Resource"},
    {"F;22C", "RaptorII", "General Resource"},
    {"F?A;32C", "Erne", "General Resource"},
    {"XFA;36A", "Game", "General Resource"},
    {"RF;12A2", "Blackbird", "General Resource"},
    {"EF2000E", "TyphoonII", "UPEO"},
    {"MiG;33", "FulcrumSS", "UPEO"},
    {"F?A;18U", "HornetADV", "UPEO"},
    {"F;16XFU", "Gyrfalcon", "UPEO"},
    {"R;101U", "Delphinus;1", "UPEO"},
    {"R;201U", "Asterozoa", "UPEO"},
    {"Su;37", "Super Flanker", "UPEO"},
    {"Su;43", "Berkut", "UPEO"},
    {"R;101", "Delphinus;1", "Neucom"},
    {"R;201", "Asterozoa", "Neucom"},
    {"R;102", "Delphinus;2", "Neucom"},
    {"R;211", "Orcinus", "Neucom"},
    {"R;103", "Delphinus;3", "Neucom"},
    {"R;311", "Remora", "Neucom"},
    {"R;352", "Sepia", "Neucom"},
    {"XR;900", "Geopelia", "Neucom"},
    {"X;49", "Night Raven", "Ouroboros"},
    {"UI;4054", "Aurora", "Ouroboros"},
    {"XFA;36A", "Game", "Ouroboros"},
    {"Su;43", "Berkut", "Ouroboros"},
    {"R;103", "Delphinus;3", "Ouroboros"},
    {"F;15S?MT", "Eagle Plus D", "General Resource"},
    {"F;22C", "RaptorII D", "General Resource"},
    {"Su;37", "Super Flanker R", "UPEO"},
    {"R;102", "Delphinus;2 C", "Neucom"},
    {"R;103", "Delphinus;3 C", "Neucom"},
    {"R;311", "Remora C", "Neucom"},
};

uint32 aircraft_preview_draw_info(void)
{
    uint32 packet, ordering, tag, link, index;
    sint32 aircraft;
    FUNCTION_MARKER(0x800F9CF0, "ACE3.BIN");
    packet = ui_packets.scene_packet_cursor;
    TILE *tile = ui_packet_span(packet, sizeof(*tile));
    tile->w = (sint16)(uint16)(132);
    tile->h = (sint16)(uint16)(318);
    tile->p[0].y = (sint16)(uint16)(29);
    ((uint8 *)&tile->tag)[3] = (uint8)(3);
    tile->rgbc.r = (uint8)(0);
    tile->rgbc.g = (uint8)(0);
    tile->rgbc.b = (uint8)(0);
    tile->rgbc.code = (uint8)(96);
    tile->p[0].x = (sint16)(uint16)(0);
    ordering = ui_packets.scene_ot;
    tag = tile->tag;
    link = *(uint32 *)ui_scene_ot_span(ordering, 4);
    tile->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    link = *(uint32 *)ui_scene_ot_span(ordering, 4);
    *(uint32 *)ui_scene_ot_span(ordering, 4) = (link & 0xFF000000) | (packet & 0xFFFFFF);
    index = mission_aircraft.variant;
    ui_packets.scene_packet_cursor = (uint32)(packet + 16);
    aircraft = (sint16)mission_aircraft.preview[index];
    if (aircraft <= 0)
        aircraft = 1;
    if (aircraft >= 37)
        aircraft = 36;
    text_select_font(7);
    text_set_color(0, 128, 0);
    text_set_pos(12, 64);
    text_print_spr("MODEL ID");
    text_set_pos(12, 96);
    text_print_spr(aircraft_preview_labels[aircraft][0]);
    text_set_pos(12, 112);
    text_print_spr(aircraft_preview_labels[aircraft][1]);
    text_set_pos(12, 144);
    return text_print_spr(aircraft_preview_labels[aircraft][2]);
}

uint32 aircraft_selection_input(void)
{
    uint32 value, other, choice, slot, phase, result, changed = 0, ready;
    FUNCTION_MARKER(0x800F9ED0, "ACE3.BIN");
    if (game_runtime.progress.flow_flags & 16)
    {
        if ((sint16)mission_flow.transition_phase != 3)
        {
            value = game_input.repeat;
            other = game_input.pressed;
            value &= 0xFFFFFFC3;
            other &= 0xFFFFFFC3;
            game_input.repeat = (uint32)(value);
            game_input.pressed = (uint32)(other);
        }
    }
    ui_widget_set(1, 0);
    if (game_runtime.progress.flow_flags & 16)
    {
        if ((sint16)game_runtime.selection.mission == 42)
        {
            ui_widget_set(3, 2);
            ui_widget_set(2, 2);
        }
        else
        {
            ui_widget_set(2, (game_input.held & 4) != 0);
            ui_widget_set(3, (game_input.held & 8) != 0);
        }
    }
    else
    {
        choice = mission_aircraft.variant ? (game_input.held & 4) != 0 : 2;
        ui_widget_set(2, choice);
        value = mission_aircraft.variant_count;
        other = mission_aircraft.variant;
        if (other == value - 1)
            choice = 2;
        else
        {
            choice = (game_input.held & 8) != 0;
        }
        ui_widget_set(3, choice);
    }
    ui_widget_set(4, (sint16)ui_item_at(0, 8)->phase == 2);
    if ((sint16)game_runtime.selection.mission != 42)
    {
        value = game_input.repeat;
        if (value & 8)
        {
            if (game_runtime.progress.flow_flags & 16)
            {
                value = mission_aircraft.variant;
                mission_aircraft_group(0, (sint16)mission_aircraft.preview[value]);
                value = (sint16)mission_aircraft.preview_count;
                other = mission_aircraft.variant;
                mission_aircraft.variant = (sint32)other < (sint32)(value - 1) ? other + 1 : 0;
                mission_aircraft.group = 0;
                changed = 1;
            }
            else
            {
                value = mission_aircraft.variant_count;
                other = mission_aircraft.variant;
                if ((sint32)other < (sint32)(value - 1))
                {
                    mission_aircraft.variant = other + 1;
                    changed = 1;
                }
            }
        }
        else if (value & 4)
        {
            if (game_runtime.progress.flow_flags & 16)
            {
                value = mission_aircraft.variant;
                mission_aircraft_group(0, (sint16)mission_aircraft.preview[value]);
                value = mission_aircraft.variant;
                if ((sint32)value > 0)
                    --value;
                else
                    value = (uint32)(sint16)mission_aircraft.preview_count - 1;
                mission_aircraft.variant = value;
                mission_aircraft.group = 0;
                changed = 1;
            }
            else
            {
                value = mission_aircraft.variant;
                if ((sint32)value > 0)
                {
                    mission_aircraft.variant = value - 1;
                    changed = 1;
                }
            }
        }
        if (changed)
        {
            sound_play(61);
            ui_animation_set(2, 3, -1, -1);
            ui_animation_set(8, 3, -1, -1);
        }
    }
    if (game_runtime.progress.flow_flags & 16)
        aircraft_preview_draw_info();
    else
        menu_model_update_selection(1);
    if (game_runtime.progress.flow_flags & 16)
    {
        if ((game_input.pressed & 16) && (game_input.held & 0x2F) == 0)
        {
            sound_play(91);
            sound_play(60);
            mission_aircraft.action = 1;
            mission_aircraft.phase = 2;
        }
    }
    else
    {
        ready = (sint16)mission_flow.transition_phase == 3;
        if ((game_input.pressed & 16) && (game_input.held & 0x2F) == 0)
        {
            phase = ui_animation_phase(2);
            if (phase == 2 && ready)
            {
                ui_animation_set(5, 3, -1, -1);
                ui_animation_set(6, 3, -1, -1);
                ui_animation_set(3, 3, -1, -1);
                ui_animation_set(8, 3, -1, -1);
                sound_play(91);
                sound_play(60);
                mission_aircraft.action = 1;
                mission_aircraft.phase = phase;
            }
        }
    }
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            static const uint8 slots[] = {2, 1, 3, 11, 5, 6, 7, 8};
            for (slot = 0; slot < 8; ++slot)
                ui_animation_set(slots[slot], 3, -1, -1);
            sound_play(91);
            sound_play(59);
            mission_effect_close_sprs();
            mission_aircraft.action = 2;
            mission_aircraft.phase = 2;
            return 2;
        }
    }
    return result;
}

uint32 aircraft_selection_init(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    MISSION_ACTOR_TRAITS *actor_traits_target;
    uint32 actor_index_context;

    const uint8 unlock_indices[10] = {31, 32, 33, 34, 35, 36, 1, 2, 18, 19};
    uint32 unlock, value, index, selected, group;
    uint16 matched_idx;
    FUNCTION_MARKER(0x800FABE8, "ACE3.BIN");
    unlock = aircraft_selection_rating_unlocks();
    mission_aircraft.unlock_flags = (uint32)(unlock);
    resource_init_queue(0, 0);
    mission_aircraft.selection_bookmark = arena_alloc(316);
    mission_aircraft.weapon_packets = &weapon_packet_storage;
    mission_aircraft.weapon_info = 0;
    buffer_allocation = arena_alloc_backed(MISSION_AIRCRAFT_MODEL_BUFFER_BYTES);
    mission_aircraft.model_buffer = buffer_allocation.encoded_begin;
    mission_aircraft.model_data = buffer_allocation.buffer.data;
    mission_aircraft.phase = 0;
    mission_aircraft.action = 0;
    mission_aircraft.desc_saved = 0;
    mission_aircraft.variant = 0;
    if (game_runtime.progress.flow_flags & 16)
    {
        if (unlock & 4)
            game_runtime.progress.aircraft_unlocked[25] = 1;
        if (unlock & 2)
            game_runtime.progress.aircraft_unlocked[27] = 1;
        if (unlock & 1)
            for (index = 0; index < 10; ++index)
                game_runtime.progress.aircraft_unlocked[unlock_indices[index]] = 1;
        mission_aircraft.preview_count = 0;
        matched_idx = 0;
        if ((sint16)game_runtime.selection.mission == 42)
        {
            mission_aircraft.preview[0] = (uint16)(24);
            mission_aircraft.preview_count = 1;
        }
        else
        {
            for (index = 0; index < 64; ++index)
            {
                if (game_runtime.progress.aircraft_unlocked[index] == 1 && index != 24)
                {
                    value = (sint16)mission_aircraft.preview_count;
                    mission_aircraft.preview[value] = (uint16)(index);
                    if ((sint16)game_runtime.selection.aircraft == (sint32)index)
                        matched_idx = mission_aircraft.preview_count;
                    value = mission_aircraft.preview_count;
                    mission_aircraft.preview_count = value + 1;
                }
            }
        }
        if (!mission_aircraft.preview_count)
            abort();
        selected = (sint16)matched_idx;
        mission_aircraft.variant = selected;
        value = (sint16)mission_aircraft.preview[selected];
        if ((sint32)value != (sint16)game_runtime.selection.aircraft)
        {
            mission_aircraft.variant = 0;
            matched_idx = 0;
        }
        mission_aircraft.group = 0;
    }
    aircraft_preview_initialize();
    menu_model_initialize_camera();
    mission_aircraft.prev_profile = &aircraft_fallback_profile;
    selected = mission_aircraft.variant;
    mission_aircraft.weapon_tab = 0;
    mission_aircraft.profile = aircraft_profile_at(selected);
    if (!(game_runtime.progress.flow_flags & 16))
        mission_audio_bind_tracks(&audio_tracks_aircraft);
    for (index = 0; index < 5; ++index)
        sound_stop_voice(*menu_voice_slot(index));
    if (!(game_runtime.progress.flow_flags & 16))
        mission_audio_control_track(1, 0);
    sound_play(90);
    if (game_runtime.progress.flow_flags & 16)
    {
        ui_animation_set(3, 1, 0, 25);
        group = 11;
    }
    else
    {
        ui_animation_set(2, 1, 0, 25);
        ui_animation_set(8, 1, 0, 25);
        ui_animation_set(1, 1, 0, 25);
        ui_animation_set(3, 1, 0, 25);
        ui_animation_set(11, 1, 0, 25);
        ui_animation_set(5, 1, 0, 25);
        group = 6;
    }
    ui_animation_set(group, 1, 0, 25);
    ui_animation_set(7, 1, 0, 25);
    ui_group_set_mask(0, 0x7701);
    actor_index_context = 0;
    actor_traits_target = mission_actor_traits((actor_index_context + 2));
    value = actor_traits_target->flags;
    actor_traits_target->flags = (uint32)(value | 0x1C000);
    mission_actor_traits((actor_index_context + 2))->model = (uint8)(2);
    actor_index_context = 0;
    mission_actor((actor_index_context + 2))->pose.position[0] = (uint32)(0);
    mission_actor((actor_index_context + 2))->pose.position[1] = (uint32)(0);
    mission_actor((actor_index_context + 2))->pose.position[2] = (uint32)(0);
    mission_actor((actor_index_context + 2))->pose.angles[0] = (uint16)(0);
    mission_actor((actor_index_context + 2))->pose.angles[1] = (uint16)(4096);
    mission_actor((actor_index_context + 2))->pose.angles[2] = (uint16)(4096);
    game_upload_image(scene_palette.rectangle, scene_palette.neutral, sizeof(scene_palette.neutral));
    DrawSync(0);
    mission_aircraft.flags = 3;
    menu_geometry_initialize_weapon_packets();
    menu_model_update_selection(0);
    if (game_runtime.progress.flow_flags & 16)
    {
        ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        ui_animation_set(8, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
    }
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return value;
}

uint32 mission_menu_init(void)
{
    uint32 mode, mission, item, value;

    FUNCTION_MARKER(0x800CF364, "ACE3.BIN");
    mission_flow.menu_phase = 0;
    mission_flow.phase_timer = (uint32)(0);
    mission_flow.root_phase = 1;
    ui_animation_set(1, 1, 0, 40);
    ui_animation_set(2, 1, 0, 40);
    ui_map_initialize_packets();
    ui_map_begin(1, 0);
    ui_item_reset_motion(0, 2);
    mode = mission_flow.root_variant;
    if (mode == 4)
    {
        mission_menu_start_item_motion(0, 1);
        ui_item_reset_motion(0, 0);
        ui_animation_set(15, 1, 0, 25);
        mission_flow.root_variant = 0;
    }
    ui_group_set_mask(0, 7);
    ui_group_set_mask(2, 0);
    mode = mission_flow.root_variant;
    ui_widget_set(1, mode + 1);
    mission = (sint16)game_runtime.selection.mission;
    item = (sint16)game_runtime.selection.aircraft;
    value = mission_aircraft_group(mission, (sint32)item);
    mission_aircraft.group = value;
    sound_play(90);
    return menu_voices_restart(0);
}

void mission_flow_reset(void)
{
    memset(&mission_flow, 0, sizeof(mission_flow));
    mission_flow.flight_selection.target = &flight_target_profiles[0];
    mission_flow.flight_selection.relative = &flight_relative_profiles[0];
    mission_flow.flight_selection.offsets = &flight_model_offsets[0];
    memset(&mission_resource_load, 0, sizeof(mission_resource_load));
    memset(&mission_selector, 0, sizeof(mission_selector));
    memset(&mission_debrief, 0, sizeof(mission_debrief));
    memset(&mission_aircraft, 0, sizeof(mission_aircraft));
}

typedef uint32 (*MISSION_CALLBACK)(void);

typedef struct
{
    /* Exactly one callback is set */
    MISSION_CALLBACK plain;
    uint32 (*context)(uint32);
    void (*notify)(void);
    uint32 (*options)(MENU_OPTIONS *);
} MISSION_DETAIL_PHASE;

typedef struct
{
    const MISSION_DETAIL_PHASE *callbacks;
    uint32 count;
} MISSION_PHASE_TABLE;

typedef struct
{
    const MISSION_DETAIL_PHASE *callbacks;
    uint32 count;
} MISSION_DETAIL_TABLE;

static const MISSION_DETAIL_PHASE mission_root_callbacks[] = {
    {mission_menu_init, NULL, NULL}, {mission_frontend_root_frame, NULL, NULL}, {NULL, NULL, mission_frontend_select_mode}, {NULL, NULL, mission_dispatch_current_mode}, {NULL, NULL, archive_browser_init}, {NULL, NULL, archive_browser_dispatch}, {scene_archive_init, NULL, NULL}, {scene_archive_update, NULL, NULL}, {NULL, NULL, dialogue_ui_dispatch_phase}, {scene_archive_resume, NULL, NULL}, {NULL, NULL, mission_dialogue_dispatch_media_request}, {NULL, NULL, mission_dialogue_dispatch_media_cancel}, {NULL, NULL, menu_state_ready}, {menu_state_frame, NULL, NULL}, {mission_frontend_begin_archive_transition, NULL, NULL}, {menu_state_return_ready, NULL, NULL}, {mission_frontend_reset_root_selection, NULL, NULL}, {mission_frontend_cancel_root_selection, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group0[] = {
    {mission_briefing_intro_begin, NULL, NULL}, {mission_briefing_intro_tick, NULL, NULL}, {mission_briefing_prepare_playback, NULL, NULL}, {mission_briefing_start_playback_frame, NULL, NULL}, {mission_briefing_playback_tick, NULL, NULL}, {mission_briefing_finish_frame, NULL, NULL}, {mission_briefing_finish_tick, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group1[] = {
    {mission_briefing_begin, NULL, NULL},
    {mission_briefing_frame, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group2[] = {
    {aircraft_selection_init, NULL, NULL},
    {aircraft_selection_frame, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group3_4[] = {
    {NULL, NULL, mission_launch_begin}, {mission_launch_wait_frame, NULL, NULL}, {mission_launch_fade_tick, NULL, NULL}, {mission_resume_confirm_tick, NULL, NULL}, {mission_replay_prompt_begin, NULL, NULL}, {mission_replay_prompt_tick, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group5_7[] = {
    {dialogue_ui_begin_result, NULL, NULL},       {dialogue_ui_result_fade_tick, NULL, NULL}, {NULL, NULL, mission_transition_wait_ring},    {dialogue_ui_wait_header_tick, NULL, NULL}, {dialogue_ui_open_step, NULL, NULL},        {dialogue_ui_bind_entry_asset, NULL, NULL},  {dialogue_ui_begin_intro_audio, NULL, NULL}, {NULL, NULL, dialogue_ui_wait_intro_audio},     {dialogue_ui_open_entry_fade_tick, NULL, NULL}, {dialogue_ui_begin_entry, NULL, NULL},      {dialogue_ui_wait_entry_audio, NULL, NULL}, {dialogue_ui_entry_tick, NULL, NULL},        {dialogue_ui_begin_outro_audio, NULL, NULL}, {NULL, NULL, dialogue_ui_wait_outro_audio},   {NULL, NULL, dialogue_ui_advance_panel_tick}, {NULL, NULL, dialogue_ui_return_account}, {dialogue_ui_briefing_frame, NULL, NULL},
    {dialogue_ui_briefing_fade_tick, NULL, NULL}, {NULL, NULL, mission_transition_wait_ring}, {dialogue_ui_wait_selection_tick, NULL, NULL}, {dialogue_ui_open_step, NULL, NULL},        {dialogue_ui_bind_entry_asset, NULL, NULL}, {dialogue_ui_begin_intro_audio, NULL, NULL}, {NULL, NULL, dialogue_ui_wait_intro_audio},  {dialogue_ui_open_entry_fade_tick, NULL, NULL}, {dialogue_ui_begin_entry, NULL, NULL},          {dialogue_ui_wait_entry_audio, NULL, NULL}, {dialogue_ui_entry_tick, NULL, NULL},       {dialogue_ui_begin_outro_audio, NULL, NULL}, {NULL, NULL, dialogue_ui_wait_outro_audio},  {NULL, NULL, dialogue_ui_advance_panel_tick}, {dialogue_ui_return_root_tick, NULL, NULL},   {dialogue_ui_begin_sequence, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_mode_group6[] = {
    {mission_save_begin, NULL, NULL},
#if 0
    {mission_save_wait_ring, NULL, NULL},
    {mission_save_fade_background, NULL, NULL},
    {mission_save_frame, NULL, NULL},
#endif
};

static const MISSION_DETAIL_PHASE mission_mode_group8[] = {
    {mission_select_begin, NULL, NULL},
    {mission_select_advance_phase, NULL, NULL},
    {mission_select_tick, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_detail_callbacks[] = {
    {NULL, NULL, menu_detail_begin},         {NULL, NULL, mission_menu_main_open_tick}, {NULL, NULL, mission_menu_main_select_tick}, {mission_menu_main_close_tick, NULL, NULL}, {NULL, NULL, mission_menu_detail_open_tick}, {NULL, NULL, mission_menu_detail_frame}, {NULL, NULL, mission_menu_detail_close_tick}, {NULL, NULL, menu_bindings_screen_begin},       {NULL, NULL, menu_bindings_screen_frame}, {menu_calibration_begin, NULL, NULL},  {NULL, NULL, menu_calibration_update},  {NULL, NULL, NULL, menu_options_begin}, {NULL, NULL, NULL, menu_options_input}, {NULL, NULL, NULL, menu_options_apply}, {NULL, NULL, menu_audio_begin}, {NULL, NULL, mission_menu_options_wait_frame}, {NULL, NULL, mission_menu_options_select_frame}, {NULL, NULL, mission_menu_options_leave_frame}, {menu_audio_frame, NULL, NULL}, {mission_menu_brightness_frame, NULL, NULL},
    {NULL, NULL, mission_menu_screen_frame}, {NULL, NULL, menu_options_step},           {mission_menu_records_begin, NULL, NULL},    {mission_menu_records_tick, NULL, NULL},    {NULL, NULL, mission_menu_account_frame},    {mission_menu_account_tick, NULL, NULL}, {NULL, NULL, mission_menu_account_fade_tick}, {NULL, NULL, mission_menu_account_finish_tick}, {mission_sound_test_begin, NULL, NULL},   {mission_sound_test_tick, NULL, NULL}, {mission_menu_movie_begin, NULL, NULL}, {NULL, NULL, mission_menu_movie_frame},
};

static const MISSION_DETAIL_PHASE mission_detail_notify[] = {
    {NULL, NULL, mission_detail_notify_noop},
};

static const MISSION_DETAIL_PHASE mission_detail_view[] = {
    {menu_model_begin, NULL, NULL}, {NULL, NULL, menu_model_wait_load}, {menu_model_select_tick, NULL, NULL}, {NULL, NULL, menu_model_select_finish}, {menu_model_detail_begin, NULL, NULL}, {menu_model_detail_tick, NULL, NULL}, {menu_model_restore, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_detail_auxiliary[] = {
    {mission_menu_exit_begin, NULL, NULL},
    {mission_menu_exit_wait_ring, NULL, NULL},
    {mission_menu_exit_fade_tick, NULL, NULL},
    {mission_menu_exit_finish, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_transition_callbacks[] = {
    {NULL, NULL, mission_frontend_idle_phase}, {mission_frontend_load_resource124, NULL, NULL}, {NULL, NULL, mission_frontend_wait_resource124}, {mission_frontend_load_resource125, NULL, NULL}, {NULL, NULL, mission_frontend_wait_resource125},
};

static const MISSION_PHASE_TABLE mission_root_tables[] = {
    {mission_root_callbacks + 0, 18}, {mission_root_callbacks + 2, 16}, {mission_root_callbacks + 4, 14}, {mission_root_callbacks + 6, 12}, {mission_root_callbacks + 12, 6}, {mission_root_callbacks + 16, 2},
};

static const MISSION_PHASE_TABLE mission_mode_tables[] = {
    {mission_mode_group0 + 0, 7},     {mission_mode_group1 + 0, 2}, {mission_mode_group2 + 0, 2}, {mission_mode_group3_4 + 0, 6}, {mission_mode_group3_4 + 4, 2}, {mission_mode_group5_7 + 0, 33},
#if 0
    {mission_mode_group6 + 0, 4},
#else
    {mission_mode_group6 + 0, 1},
#endif
    {mission_mode_group5_7 + 16, 17}, {mission_mode_group8 + 0, 3},
};

static const MISSION_DETAIL_TABLE mission_detail_tables[] = {
    {mission_detail_callbacks + 0, 32}, {mission_detail_callbacks + 7, 25}, {mission_detail_callbacks + 9, 23}, {mission_detail_callbacks + 14, 18}, {mission_detail_notify + 0, 1}, {mission_detail_callbacks + 22, 10}, {mission_detail_callbacks + 24, 8}, {mission_detail_callbacks + 28, 4}, {mission_detail_view + 0, 7}, {mission_detail_callbacks + 30, 2}, {mission_detail_auxiliary + 0, 4},
};

static const MISSION_DETAIL_PHASE *mission_phase(const MISSION_PHASE_TABLE *tables, uint32 count, uint32 kind, uint32 phase)
{
    if (kind >= count || phase >= tables[kind].count)
        abort();
    return tables[kind].callbacks + phase;
}

static uint32 mission_detail_call(const MISSION_DETAIL_PHASE *entry, uint32 context, MENU_OPTIONS *options)
{
    if (entry->options)
        return entry->options(options);
    if (entry->context)
        return entry->context(context);
    if (entry->plain)
        return entry->plain();
    if (!entry->notify)
        abort();
    entry->notify();
    return 0;
}

void mission_dispatch_root(uint32 kind, uint32 phase)
{
    const MISSION_DETAIL_PHASE *callback = mission_phase(mission_root_tables, sizeof(mission_root_tables) / sizeof(mission_root_tables[0]), kind, phase);
    mission_detail_call(callback, 0, NULL);
}

void mission_dispatch_mode(uint32 kind, uint32 phase)
{
    const MISSION_DETAIL_PHASE *callback = mission_phase(mission_mode_tables, sizeof(mission_mode_tables) / sizeof(mission_mode_tables[0]), kind, phase);
    mission_detail_call(callback, 0, NULL);
}

void mission_dispatch_detail(uint32 kind, uint32 phase, MENU_HEADER *header)
{
    const MISSION_DETAIL_PHASE *entry;
    MENU_OPTIONS *options = NULL;

    if (kind >= sizeof(mission_detail_tables) / sizeof(mission_detail_tables[0]) || phase >= mission_detail_tables[kind].count)
        abort();
    entry = mission_detail_tables[kind].callbacks + phase;
    /* preserve the four header lanes for physical options entries */
    if (entry->options)
        options = &header->options;
    mission_detail_call(entry, 0, options);
}

uint32 mission_dispatch_detail_aux(uint32 phase, MENU_OPTIONS *options)
{
    if (phase >= 3)
        abort();
    return mission_detail_call(mission_detail_callbacks + 11 + phase, 0, options);
}

void mission_dispatch_transition(uint32 phase)
{
    if (phase >= sizeof(mission_transition_callbacks) / sizeof(mission_transition_callbacks[0]))
        abort();
    mission_detail_call(mission_transition_callbacks + phase, 0, NULL);
}

typedef struct
{
    MISSION_CALLBACK plain;
    uint32 (*load)(uint32, RESOURCE_STREAM_BUFFER);
    uint32 (*carry)(uint32);
    uint32 result;
} MISSION_LOADING_PHASE;

static const MISSION_LOADING_PHASE mission_loading_callbacks[] = {
    {NULL, mission_frontend_load_aircraft_assets, NULL, 0}, {mission_frontend_wait_aircraft_assets, NULL, NULL, 0}, {mission_frontend_bind_aircraft_assets, NULL, NULL, 0}, {NULL, NULL, mission_load_preserve_result, 0x800CE754}, {mission_frontend_load_resource121, NULL, NULL, 0}, {mission_frontend_wait_resource121, NULL, NULL, 0}, {mission_frontend_load_resource122, NULL, NULL, 0}, {mission_frontend_wait_resource122, NULL, NULL, 0}, {mission_frontend_load_resource123, NULL, NULL, 0}, {mission_frontend_wait_resource123, NULL, NULL, 0}, {menu_state_load_begin, NULL, NULL, 0}, {mission_frontend_wait_options_assets, NULL, NULL, 0}, {menu_state_return_load_begin, NULL, NULL, 0}, {mission_frontend_finish_asset_load, NULL, NULL, 0},
};

uint32 mission_dispatch_loading(uint32 phase, uint32 mode, RESOURCE_STREAM_BUFFER buffer)
{
    const MISSION_LOADING_PHASE *entry;

    if (phase >= sizeof(mission_loading_callbacks) / sizeof(mission_loading_callbacks[0]))
        abort();
    entry = mission_loading_callbacks + phase;
    if (entry->load)
        return entry->load(mode, buffer);
    if (entry->carry)
        return entry->carry(entry->result);
    return entry->plain();
}

static const MISSION_CALLBACK mission_debrief_callbacks[] = {
    frontend_load_mission, frontend_load_poll_mission, mission_frontend_bind_mission_assets, frontend_load_common, frontend_load_poll_common, mission_frontend_bind_common_assets, mission_frontend_begin_asset_fade, mission_frontend_wait_asset_fade, NULL, NULL, NULL, frontend_load_mission, frontend_load_poll_mission, mission_frontend_bind_mission_assets, frontend_load_common, frontend_load_poll_common, mission_frontend_bind_common_assets, NULL,
};

uint32 mission_dispatch_debrief(uint32 phase)
{
    if (phase >= sizeof(mission_debrief_callbacks) / sizeof(mission_debrief_callbacks[0]) || !mission_debrief_callbacks[phase])
        abort();
    return mission_debrief_callbacks[phase]();
}

static const MISSION_CALLBACK mission_loading_screen_callbacks[] = {
    frontend_load_begin, mission_debrief_wait_assets, mission_debrief_bind_loaded_assets, frontend_load_mission, frontend_load_poll_mission, mission_frontend_bind_mission_assets, frontend_load_common, frontend_load_poll_common, mission_frontend_bind_common_assets, mission_debrief_wait_animation,
};

uint32 mission_dispatch_loading_screen(uint32 phase)
{
    if (phase >= sizeof(mission_loading_screen_callbacks) / sizeof(mission_loading_screen_callbacks[0]) || !mission_loading_screen_callbacks[phase])
        abort();
    return mission_loading_screen_callbacks[phase]();
}

static const MISSION_CALLBACK mission_init_callbacks[] = {
    mission_frontend_initialize_packet_cursor, mission_frontend_initialize_state, mission_frontend_fade_in_tick, mission_frontend_open_ring_frame, mission_frontend_wait_open_ring,
};

uint32 mission_dispatch_init(uint32 phase)
{
    if (phase >= sizeof(mission_init_callbacks) / sizeof(mission_init_callbacks[0]) || !mission_init_callbacks[phase])
        abort();
    return mission_init_callbacks[phase]();
}

static const MISSION_DETAIL_PHASE mission_select_callbacks[] = {
    {NULL, NULL, mission_select_grid_begin}, {mission_select_grid_frame, NULL, NULL}, {NULL, NULL, mission_select_grid_finish}, {mission_select_difficulty_begin, NULL, NULL}, {mission_select_difficulty_frame, NULL, NULL}, {NULL, NULL, mission_select_difficulty_finish}, {mission_select_load_begin, NULL, NULL}, {mission_select_load_tick, NULL, NULL},
};

void mission_dispatch_select(uint32 phase)
{
    if (phase >= sizeof(mission_select_callbacks) / sizeof(mission_select_callbacks[0]))
        abort();
    mission_detail_call(mission_select_callbacks + phase, 0, NULL);
}

static const MISSION_CALLBACK mission_load_callbacks[] = {
    mission_load_begin_media_check,
    mission_load_check_media,
    mission_load_wait_recovery,
    mission_load_finish_recovery,
};

uint32 mission_dispatch_load(uint32 phase)
{
    if (phase >= sizeof(mission_load_callbacks) / sizeof(mission_load_callbacks[0]))
        abort();
    return mission_load_callbacks[phase]();
}

static const MISSION_CALLBACK mission_aircraft_callbacks[] = {
    aircraft_selection_begin_input, aircraft_selection_input_frame, aircraft_selection_finish, aircraft_selection_wait_weapons, aircraft_selection_weapons_input, aircraft_selection_finish_weapons_frame,
};

void mission_dispatch_aircraft(uint32 phase)
{
    if (phase >= sizeof(mission_aircraft_callbacks) / sizeof(mission_aircraft_callbacks[0]))
        abort();
    mission_aircraft_callbacks[phase]();
}

static const MISSION_DETAIL_PHASE mission_selection_callbacks[] = {
    {mission_save_offer_begin, NULL, NULL},
    {mission_save_offer_tick, NULL, NULL},
    {mission_save_wait_cards_begin, NULL, NULL},
    {NULL, NULL, mission_save_wait_cards_tick},
    {mission_save_unavailable_begin, NULL, NULL},
    {mission_save_unavailable_tick, NULL, NULL},
    {NULL, NULL, mission_save_select_card_begin},
    {mission_save_select_card_tick, NULL, NULL},
    {mission_save_selected_card_missing_begin, NULL, NULL},
    {mission_save_selected_card_missing_tick, NULL, NULL},
    {mission_save_poll_wait_begin, NULL, NULL},
    {mission_save_poll_wait_tick, NULL, NULL},
    {mission_save_info_notice_begin, NULL, NULL},
    {mission_save_info_notice_tick, NULL, NULL},
    {mission_save_format_begin, NULL, NULL},
    {NULL, NULL, mission_save_format_transition_tick},
    {mission_save_format_confirm_tick, NULL, NULL},
    {mission_save_format_tick, NULL, NULL},
    {mission_save_create_file_tick, NULL, NULL},
    {mission_save_write_icons_tick, NULL, NULL},
    {mission_save_initialize_directory_tick, NULL, NULL},
    {mission_save_format_notice_tick, NULL, NULL},
    {mission_save_load_begin, NULL, NULL},
    {mission_save_read_directory_tick, NULL, NULL},
    {mission_save_read_accounts_tick, NULL, NULL},
    {mission_save_find_account_tick, NULL, NULL},
    {mission_save_load_account_tick, NULL, NULL},
    {NULL, NULL, mission_save_read_missions_tick},
    {mission_save_read_notice_tick, NULL, NULL},
    {mission_save_create_offer_begin, NULL, NULL},
    {mission_save_create_offer_tick, NULL, NULL},
    {mission_save_check_file_tick, NULL, NULL},
    {mission_save_check_space_tick, NULL, NULL},
    {mission_save_space_notice_tick, NULL, NULL},
    {mission_save_create_file_tick, NULL, NULL},
    {mission_save_write_icons_tick, NULL, NULL},
    {mission_save_initialize_directory_tick, NULL, NULL},
    {mission_save_file_notice_tick, NULL, NULL},
    {mission_save_select_account_begin, NULL, NULL},
    {mission_save_select_account_frame, NULL, NULL},
    {mission_save_overwrite_account_begin, NULL, NULL},
    {mission_save_overwrite_account_tick, NULL, NULL},
    {mission_save_account_file_check_tick, NULL, NULL},
    {mission_save_create_account_file_tick, NULL, NULL},
    {mission_save_write_account_icons_tick, NULL, NULL},
    {mission_save_write_account_directory_tick, NULL, NULL},
    {mission_save_write_account_frame, NULL, NULL},
    {mission_save_account_write_notice_tick, NULL, NULL},
    {mission_save_select_mission_begin, NULL, NULL},
    {mission_save_select_mission_tick, NULL, NULL},
    {mission_save_overwrite_mission_begin, NULL, NULL},
    {mission_save_overwrite_mission_tick, NULL, NULL},
    {mission_save_confirm_mission_tick, NULL, NULL},
    {mission_save_commit_mission_begin, NULL, NULL},
    {mission_save_write_mission_account_tick, NULL, NULL},
    {mission_save_write_mission_record_tick, NULL, NULL},
    {mission_save_mission_write_notice_tick, NULL, NULL},
    {mission_save_success_begin, NULL, NULL},
    {mission_save_success_tick, NULL, NULL},
    {mission_save_leave_offer_begin, NULL, NULL},
    {mission_save_leave_offer_tick, NULL, NULL},
    {mission_save_check_disc_tick, NULL, NULL},
    {mission_save_recovery_wait_tick, NULL, NULL},
    {mission_save_recovery_finish_tick, NULL, NULL},
};

static const MISSION_PHASE_TABLE mission_selection_tables[] = {
    {mission_selection_callbacks + 0, 64}, {mission_selection_callbacks + 2, 62}, {mission_selection_callbacks + 4, 60}, {mission_selection_callbacks + 6, 58}, {mission_selection_callbacks + 8, 56}, {mission_selection_callbacks + 10, 54}, {mission_selection_callbacks + 12, 52}, {mission_selection_callbacks + 14, 50}, {mission_selection_callbacks + 22, 42}, {mission_selection_callbacks + 29, 35}, {mission_selection_callbacks + 31, 33}, {mission_selection_callbacks + 38, 26}, {mission_selection_callbacks + 40, 24}, {mission_selection_callbacks + 48, 16}, {mission_selection_callbacks + 50, 14}, {mission_selection_callbacks + 53, 11}, {mission_selection_callbacks + 57, 7}, {mission_selection_callbacks + 59, 5}, {mission_selection_callbacks + 61, 3},
};

void mission_dispatch_selection(uint32 kind, uint32 phase)
{
    const MISSION_DETAIL_PHASE *callback = mission_phase(mission_selection_tables, sizeof(mission_selection_tables) / sizeof(mission_selection_tables[0]), kind, phase);
    mission_detail_call(callback, 0, NULL);
}

static const MISSION_DETAIL_PHASE mission_event_callbacks[] = {
    {NULL, NULL, mission_briefing_object_event_noop}, {NULL, mission_briefing_spawn_object, NULL}, {mission_briefing_schedule_object_move, NULL, NULL}, {mission_briefing_add_panel, NULL, NULL}, {mission_briefing_set_panel_rows, NULL, NULL}, {mission_briefing_schedule_camera_move, NULL, NULL}, {mission_briefing_set_camera_position, NULL, NULL}, {mission_briefing_set_marker, NULL, NULL}, {NULL, NULL, mission_briefing_event_noop}, {mission_briefing_update_events, NULL, NULL},
};

static const MISSION_DETAIL_PHASE mission_map_callbacks[] = {
    {mission_briefing_intro_begin, NULL, NULL}, {mission_briefing_intro_tick, NULL, NULL}, {mission_briefing_prepare_playback, NULL, NULL}, {mission_briefing_start_playback_frame, NULL, NULL}, {mission_briefing_playback_tick, NULL, NULL}, {mission_briefing_finish_frame, NULL, NULL}, {mission_briefing_finish_tick, NULL, NULL},
};

static const MISSION_PHASE_TABLE mission_map_tables[] = {
    {mission_map_callbacks + 0, 7},
    {mission_map_callbacks + 2, 5},
    {mission_map_callbacks + 5, 2},
};

void mission_dispatch_event(uint32 type, uint32 event_time)
{
    if (type >= sizeof(mission_event_callbacks) / sizeof(mission_event_callbacks[0]))
        abort();
    mission_detail_call(mission_event_callbacks + type, event_time, NULL);
}

void mission_dispatch_map(uint32 kind, uint32 phase)
{
    const MISSION_DETAIL_PHASE *callback = mission_phase(mission_map_tables, sizeof(mission_map_tables) / sizeof(mission_map_tables[0]), kind, phase);
    mission_detail_call(callback, 0, NULL);
}

static void mission_transition_wait_ring(void)
{
    FUNCTION_MARKER(0x800F11BC, "ACE3.BIN");
    {
        mission_transition_finish_ring();
        return;
    }
}

static void mission_transition_finish_ring(void)
{
    uint32 raw;
    sint32 mode;
    FUNCTION_MARKER(0x800F11C4, "ACE3.BIN");
    if (mission_effect.layers[MISSION_EFFECT_RING].mode != 2)
        return;
    mode = (sint16)game_runtime.account.route_mode;
    raw = game_runtime.account.route_mode;
    if (mode == 1)
    {
        if (dialogue_sequence_remaining())
        {
            mission_menu_start_item_motion(0, 15);
            ui_group_set_mask(0, 0x8000);
            mission_flow.mode_phase = 3;
            return;
        }
        ui_item_reverse_motion(0, 15);
        mission_flow.mode_phase = 15;
        return;
    }
    if (((raw - 2) & 0xFFFF) < 2 && !dialogue_sequence_remaining())
    {
        mission_flow.root_kind = 0;
        mission_flow.root_phase = 0;
        mission_flow.root_variant = 4;
        arena_set_cursor(dialogue_ui.arena_begin);
        mission_menu_start_item_motion(0, 0);
        mission_menu_start_item_motion(0, 1);
        ui_group_set_mask(0, 3);
        mission_effect_open_sprs();
        game_runtime.progress.sequence_idx = (uint8)(0);
        game_runtime.account.route_mode = (uint16)(0);
        return;
    }
    mission_flow.mode_phase = 3;
    mission_menu_start_item_motion(0, 15);
    {
        ui_group_set_mask(0, 0x8000);
        return;
    }
}

void mission_asset_load_complete(void)
{
    FUNCTION_MARKER(0x800357B4, "ACE3.BIN");
    if (resource_queue_is_completing(mission_flow.loading.read_sequence))
        mission_flow.loading.pending = 0;
    return;
}

uint32 mission_asset_load_poll(void)
{
    uint32 current, value, mode = 0, mission;
    sint32 index;
    FUNCTION_MARKER(0x800357C0, "ACE3.BIN");
    current = sound_get_cur_resource();
    mission = (sint16)game_runtime.selection.mission;
    if (mission >= GAME_MISSION_COUNT)
        abort();
    value = mission_load_specs[mission].resource_id;
    if (current == value)
    {
        mode = 1;
    }
    else
    {
        index = (sint16)game_runtime.selection.aircraft;
        if ((uint32)index >= GAME_AIRCRAFT_COUNT)
            abort();
        value = aircraft_resource_ids[index];
        if (current == value)
            mode = 2;
    }
    if (mode != 0)
    {
        mission_flow.loading.resource_kind = mode;
        value = resource_read_progress(NULL);
        mission_flow.loading.progress = value;
    }
    if ((sint16)mission_flow.loading.ticks <= 0)
        mission_loading_frame_begin();
    value = mission_flow.loading.ticks;
    if (value - 1 < 2)
    {
        current = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
        value = game_frame_packet_cursor();
        value = ui_draw_loading_fade(current, value, 0);
        game_frame_set_packet_cursor(value);
    }
    if ((sint16)mission_flow.loading.ticks >= 3)
        frontend_draw_loading_screen(mission);
    value = mission_flow.loading.pending;
    current = mission_flow.loading.ticks;
    mission_flow.loading.ticks = current + 1;
    return (value == 0) << 26;
}

uint32 mission_load_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 cursor, value;
    sint32 index;
    FUNCTION_MARKER(0x800358F8, "ACE3.BIN");
    mission_script_reset_region();
    resource_init_queue(0, 0);
    sound_disable_reverb();
    if (game_runtime.playback.demo_mode != 0)
        mission_demo_select_next();
    game_runtime.account.mission_failed = (uint8)(0);
    index = (sint16)game_runtime.selection.mission;
    if ((uint32)index >= GAME_MISSION_COUNT)
        abort();
    game_runtime.account.loss_count = (uint16)(0);
    game_runtime.playback.resume_count = 0;
    value = mission_load_specs[index].resource_id;
    game_runtime.resource_slot.resource_id = (uint32)(value);
    value = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(value);
    cursor = asset_allocation.encoded_begin;
    mission_flow.loading.pending = 1;
    mission_flow.loading.read_sequence = resource_queue_next_sequence();
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, mission_asset_load_complete);
    value = game_runtime.display.flags;
    game_runtime.display.mode = (uint32)(0);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.flags = (uint32)(value | 1);
    mission_flow.loading.ticks = 0;
    mission_flow.loading.scr_type = mission_load_specs[index].loading_mode;
    if (game_runtime.playback.demo_mode != 0)
        mission_flow.loading.scr_type = 1;
    mission_flow.loading.resource_kind = 0;
    mission_flow.loading.progress = 0;
    mission_asset_load_poll();
    return 0x4000000;
}

uint32 aircraft_load_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 value, cursor;
    sint32 index;
    FUNCTION_MARKER(0x80035F18, "ACE3.BIN");
    index = (sint16)game_runtime.selection.aircraft;
    if ((uint32)index >= GAME_AIRCRAFT_COUNT)
        abort();
    value = aircraft_resource_ids[index];
    game_runtime.resource_slot.resource_id = (uint32)(value);
    value = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(value);
    cursor = asset_allocation.encoded_begin;
    mission_flow.loading.pending = 1;
    mission_flow.loading.read_sequence = resource_queue_next_sequence();
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, mission_asset_load_complete);
    mission_asset_load_poll();
    return 0x4000000;
}

typedef struct
{
    uint32 shared_textures;
    uint32 sound_bank;
    uint32 faction_textures[4];
} LOADING_SCREEN_DIRECTORY;

uint32 mission_debrief_load_assets(uint32 loaded, const uint8 *data, uint32 data_bytes)
{
    uint32 sections[6], value, resource;

    FUNCTION_MARKER(0x800FB594, "ACE3.BIN");
    mission_image_sections(loaded, data, data_bytes, sections, 6);
    const LOADING_SCREEN_DIRECTORY directory = {sections[0], sections[1], {sections[2], sections[3], sections[4], sections[5]}};
    scene_texture_load_group(directory.shared_textures, (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    if (mission_flow.faction == 0)
        scene_texture_load_group(directory.faction_textures[0], (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    if (mission_flow.faction == 1)
        scene_texture_load_group(directory.faction_textures[1], (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    if (mission_flow.faction == 2)
        scene_texture_load_group(directory.faction_textures[2], (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    if (mission_flow.faction == 3)
        scene_texture_load_group(directory.faction_textures[3], (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    sound_load_bank((SOUND_BANK_INPUT){data, data_bytes, directory.sound_bank - loaded});
    arena_set_cursor(directory.sound_bank);
    if ((sint16)mission_flow.transition_phase == 0)
    {
        value = mission_flow.faction;
        ui_group_set_mask(2, ((uint32)1 << ((value + 2) & 31)) | 2);
    }
    value = mission_flow.faction;
    if (value >= 4)
        abort();
    resource = debrief_sound_ids[value];
    value = sound_play(resource);
    *menu_voice_slot(0) = (uint32)(value);
    return 1;
}

enum
{
    SCENE_ARCHIVE_FIRST_ID = 26,
    SCENE_ARCHIVE_END_ID = 175
};

static const uint8 scene_archive_image_indices[SCENE_ARCHIVE_END_ID - SCENE_ARCHIVE_FIRST_ID] = {
    1, 1, 2, 1, 3, 1, 3, 4, 1, 1, 4, 2, 0, 4, 3, 1, 2, 3, 4, 3, 1, 0, 1, 0, 2, 0, 1, 1, 3, 1, 3, 4, 3, 0, 1, 1, 3, 2, 3, 1, 0, 2, 2, 7, 8, 11, 8, 7, 8, 11, 7, 10, 8, 11, 11, 11, 11, 10, 8, 7, 8, 10, 10, 11, 7, 7, 11, 7, 11, 11, 6, 6, 6, 9, 6, 6, 6, 9, 9, 6, 6, 6, 9, 9, 6, 9, 6, 9, 6, 9, 9, 6, 6, 9, 9, 9, 6, 6, 9, 9, 9, 6, 9, 9, 6, 9, 9, 9, 9, 11, 6, 5, 8, 5, 8, 5, 8, 5, 12, 8, 8, 5, 8, 5, 8, 5, 6, 5, 8, 8, 8, 12, 8, 8, 7, 8, 7, 8, 7, 14, 11, 7, 15, 15, 10, 11, 9, 13, 5,
};

static uint8 scene_archive_image_index(uint32 id)
{
    uint32 index = id - SCENE_ARCHIVE_FIRST_ID;
    if (index >= sizeof(scene_archive_image_indices))
        abort();
    return scene_archive_image_indices[index];
}

uint32 mission_image_address(uint32 resource, uint32 offset)
{
    return (offset & 0x80000000u) ? offset : resource + offset;
}

void mission_image_sections(uint32 resource, const uint8 *data, uint32 data_bytes, uint32 *sections, uint32 count)
{
    const uint32 *header;
    uint32 index, bytes = (count + 1) * sizeof(uint32);

    if ((resource & 3) || resource > arena_cursor() || bytes > arena_cursor() - resource)
        abort();
    if (!data || data_bytes < bytes)
        abort();
    header = (const uint32 *)data;
    if ((uint16)header[0] != count)
        abort();
    for (index = 0; index < count; ++index)
        sections[index] = mission_image_address(resource, header[index + 1]);
}

MISSION_IMAGE_DIRECTORY mission_image_directory_bind(uint32 resource, uint32 end, uint32 base, const uint8 *data, uint32 data_bytes)
{
    MISSION_IMAGE_DIRECTORY directory;
    const uint32 *header;
    const uint8 *view;
    uint32 available;

    if ((resource & 3) || end > arena_cursor() || resource > end || end - resource < 4)
        abort();
    if (!data || resource < base || resource - base > data_bytes || data_bytes - (resource - base) < sizeof(uint32))
        abort();
    available = data_bytes - (resource - base);
    view = data + (resource - base);
    header = (const uint32 *)view;
    directory.count = (uint16)header[0];
    if (directory.count > (end - resource - 4) / sizeof(uint32))
        abort();
    if (directory.count > (available - sizeof(uint32)) / sizeof(uint32))
        abort();
    directory.base = resource;
    directory.data = data;
    directory.data_bytes = data_bytes;
    directory.encoded_base = base;
    directory.entries = directory.count ? header + 1 : NULL;
    return directory;
}

uint32 mission_image_directory_at(const MISSION_IMAGE_DIRECTORY *directory, uint32 index)
{
    if (index >= directory->count)
        abort();
    return mission_image_address(directory->base, directory->entries[index]);
}

static DIALOGUE_SEQUENCE_SOURCE mission_dialogue_source(uint32 encoded, RESOURCE_ASSET_SOURCE parent)
{
    DIALOGUE_SEQUENCE_SOURCE source = {0};
    uint32 offset = encoded - parent.encoded_base;

    if ((encoded & 3) || !parent.data || encoded < parent.encoded_base || offset > parent.bytes || sizeof(uint32) > parent.bytes - offset)
        return source;
    source.data = parent.data + offset;
    source.bytes = parent.bytes - offset;
    return source;
}

DIALOGUE_SEQUENCE_SOURCE mission_dialogue_result(uint8 kind)
{
    uint32 wire_word_0;
    const MISSION_DIALOGUE_DIRECTORY *directory = &mission_flow.assets.dialogue_sequences;

    if (kind < 4)
        return directory->result[kind];

    /* original byte indices past the authored entries read live arena data */
    memcpy(&wire_word_0, directory->legacy_result_tail + (kind - 4) * sizeof(uint32), sizeof(wire_word_0));
    return mission_dialogue_source(wire_word_0, directory->parent);
}

static void scene_archive_load_page_context(uint32 page, const MISSION_SCENE_ARCHIVE *archive)
{
    uint32 count, quotient, remaining, index, position, id;

    FUNCTION_MARKER(0x800D0838, "ACE3.BIN");
    count = archive->count;
    quotient = ((sint32)count / 4);
    remaining = (sint32)page < (sint32)quotient ? 4 : count - quotient * 4;
    if ((sint32)remaining <= 0)
    {
        (void)(uint32)(quotient * 4);
        return;
    }
    for (index = 0; (sint32)index < (sint32)remaining; ++index)
    {
        position = page * 4 + index;
        if (position >= archive->count || position >= sizeof(archive->ids))
            abort();
        id = archive->ids[position];
        scene_texture_load_image_at(&mission_flow.assets.row_image_table, id - SCENE_ARCHIVE_FIRST_ID, 960, 340 + index * 18, 384, 6);
        if (position >= archive->count || position >= sizeof(archive->ids))
            abort();
        id = archive->ids[position];
        id = scene_archive_image_index(id);
        scene_texture_load_image_at(&mission_flow.assets.secondary_image_table, id, 992, 340 + index * 18, 384, 6);
    }
    return;
}

void scene_archive_load_page(uint32 page)
{
    FUNCTION_MARKER(0x800D0830, "ACE3.BIN");
    {
        (void)(scene_archive_load_page_context(page, &mission_flow.scene_archive));
        return;
    }
}

void scene_archive_load_detail(uint32 item)
{
    uint32 page, id;

    FUNCTION_MARKER(0x800D0970, "ACE3.BIN");
    page = mission_flow.scene_archive.page;
    if (!mission_flow.scene_archive.count)
        return;
    if (item + page * 4 >= mission_flow.scene_archive.count || item + page * 4 >= sizeof(mission_flow.scene_archive.ids))
        abort();
    id = mission_flow.scene_archive.ids[item + page * 4];
    id = scene_archive_image_index(id);
    {
        (void)(scene_texture_load_image_at(&mission_flow.assets.detail_image_table, id, 960, 256, 768, 1));
        return;
    }
}

uint32 scene_archive_init(void)
{
    uint32 count, index, value;
    const uint8 *source;

    FUNCTION_MARKER(0x800D09E8, "ACE3.BIN");
    count = mission_flow.menu_init_delay + 1;
    mission_flow.menu_init_delay = (uint32)(count);
    source = game_runtime.progress.scene_status;
    if ((sint32)count < 2)
        return 0;
    mission_flow.menu_init_delay = (uint32)(0);
    mission_flow.transition_phase = 0;
    mission_flow.scene_archive.row = (uint8)(0);
    mission_flow.scene_archive.page = (uint8)(0);
    mission_flow.root_phase = 1;
    *ui_widget_value(5) = (uint8)(1);
    ui_item_reset_motion(0, 3);
    ui_group_set_mask(0, 11);
    mission_flow.scene_archive.count = (uint32)(0);
    for (index = SCENE_ARCHIVE_FIRST_ID; index < SCENE_ARCHIVE_END_ID; ++index)
    {
        if (source[index] == 0)
            continue;
        count = mission_flow.scene_archive.count;
        if (count >= sizeof(mission_flow.scene_archive.ids))
            abort();
        mission_flow.scene_archive.ids[count] = (uint8)(index);
        count = mission_flow.scene_archive.count;
        mission_flow.scene_archive.count = (uint32)(count + 1);
    }
    scene_archive_load_page(mission_flow.scene_archive.page);
    scene_archive_load_detail(mission_flow.scene_archive.row);
    for (index = 0; index < 3; ++index)
    {
        *ui_widget_value(50 + index) = (uint8)(index == 0 ? 1 : 0);
    }
    for (index = 0; index < 4; ++index)
    {
        count = mission_flow.scene_archive.count;
        if ((sint32)index < (sint32)count)
        {
            value = mission_flow.scene_archive.ids[index];
            value = source[value];
            *ui_widget_value(index + 53) = (uint8)(value);
            *ui_widget_value(index + 57) = (uint8)(2);
        }
        else
        {
            *ui_widget_value(index + 53) = (uint8)(0);
            *ui_widget_value(index + 57) = (uint8)(0);
        }
    }
    ui_widget_set(49, 0);
    mission_flow.menu_arrow_delay = 0;
    return 0x80110000;
}

uint32 scene_archive_update(void)
{
    uint32 count, page, item, last, value, flags;
    uint32 index, status, countdown;
    const uint8 *source;

    FUNCTION_MARKER(0x800D0C34, "ACE3.BIN");
    status = resource_get_recovery_state();
    ui_group_set_mask(0, 11);
    countdown = mission_flow.menu_arrow_delay;
    if (countdown != 0)
        mission_flow.menu_arrow_delay = countdown - 1;
    else
    {
        count = mission_flow.scene_archive.count;
        page = mission_flow.scene_archive.page;
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        flags = (sint32)page < ((sint32)last >> 2) ? 2 : 0;
        if (page != 0)
            flags |= 1;
        ui_widget_set(49, flags);
        *ui_widget_value(51) = (uint8)(0);
    }
    if ((game_input.repeat & 2) != 0)
    {
        page = mission_flow.scene_archive.page;
        item = mission_flow.scene_archive.row;
        count = mission_flow.scene_archive.count;
        if ((sint32)(item + page * 4) < (sint32)(count - 1))
        {
            if (item < 3)
                mission_flow.scene_archive.row = (uint8)(item + 1);
            else
            {
                mission_flow.scene_archive.row = (uint8)(0);
                page = mission_flow.scene_archive.page;
                mission_flow.scene_archive.page = (uint8)(page + 1);
                scene_archive_load_page(mission_flow.scene_archive.page);
                *ui_widget_value(51) = (uint8)(2);
                mission_flow.menu_arrow_delay = 12;
            }
            sound_play(61);
            scene_archive_load_detail(mission_flow.scene_archive.row);
        }
    }
    if ((game_input.repeat & 1) != 0)
    {
        item = mission_flow.scene_archive.row;
        if (item != 0)
            mission_flow.scene_archive.row = (uint8)(item - 1);
        else
        {
            page = mission_flow.scene_archive.page;
            if (page == 0)
                goto selection_next_page;
            mission_flow.scene_archive.row = (uint8)(3);
            page = mission_flow.scene_archive.page;
            mission_flow.scene_archive.page = (uint8)(page - 1);
            scene_archive_load_page(mission_flow.scene_archive.page);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
        }
        sound_play(61);
        scene_archive_load_detail(mission_flow.scene_archive.row);
    }
selection_next_page:
    if ((game_input.repeat & 512) != 0)
    {
        count = mission_flow.scene_archive.count;
        page = mission_flow.scene_archive.page;
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        if ((sint32)page < ((sint32)last >> 2))
        {
            mission_flow.scene_archive.page = (uint8)(page + 1);
            page = mission_flow.scene_archive.page;
            item = mission_flow.scene_archive.row;
            count = mission_flow.scene_archive.count;
            if ((sint32)(item + page * 4) >= (sint32)count)
            {
                last = count - 1;
                value = (sint32)last < 0 ? count + 2 : last;
                mission_flow.scene_archive.row = (uint8)(last - ((uint32)((sint32)value >> 2) << 2));
            }
            scene_archive_load_page(mission_flow.scene_archive.page);
            scene_archive_load_detail(mission_flow.scene_archive.row);
            sound_play(61);
            *ui_widget_value(51) = (uint8)(2);
            mission_flow.menu_arrow_delay = 12;
        }
    }
    if ((game_input.repeat & 256) != 0)
    {
        page = mission_flow.scene_archive.page;
        if (page != 0)
        {
            mission_flow.scene_archive.page = (uint8)(page - 1);
            scene_archive_load_page(mission_flow.scene_archive.page);
            scene_archive_load_detail(mission_flow.scene_archive.row);
            sound_play(61);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
        }
    }
    if ((game_input.pressed & 16) != 0 && (game_input.held & 47) == 0 && status != 1 && mission_flow.scene_archive.count != 0)
    {
        ui_animation_set(1, 0, 0, 0);
        ui_animation_set(2, 0, 0, 0);
        ui_animation_set(3, 0, 0, 0);
        ui_animation_set(4, 0, 0, 0);
        ui_animation_set(5, 0, 0, 0);
        ui_widget_set(48, 1);
        mission_flow.mode_phase = 0;
        page = mission_flow.scene_archive.page;
        item = mission_flow.scene_archive.row;
        if (page * 4 + item >= mission_flow.scene_archive.count || page * 4 + item >= sizeof(mission_flow.scene_archive.ids))
            abort();
        value = mission_flow.scene_archive.ids[page * 4 + item];
        mission_flow.root_phase = 4;
        dialogue_sequence_set_sel(value);
    }
    if ((game_input.pressed & 32) != 0 && (game_input.held & 95) == 0 && status != 1)
    {
        mission_flow.root_phase = 5;
        mission_flow.mode_phase = 0;
    }
    item = mission_flow.scene_archive.row;
    source = game_runtime.progress.scene_status;
    *ui_widget_value(52) = (uint8)(item);
    for (index = 0; index < 4; ++index)
    {
        page = mission_flow.scene_archive.page;
        count = mission_flow.scene_archive.count;
        value = index + page * 4;
        if ((sint32)value < (sint32)count)
        {
            value = mission_flow.scene_archive.ids[value];
            value = source[value];
            *ui_widget_value(index + 53) = (uint8)(value);
            *ui_widget_value(index + 57) = (uint8)(2);
        }
        else
        {
            *ui_widget_value(index + 53) = (uint8)(0);
            *ui_widget_value(index + 57) = (uint8)(0);
        }
    }
    return 0;
}

uint32 scene_archive_resume(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800D12E0, "ACE3.BIN");
    if ((sint16)mission_flow.transition_phase == 3)
    {
        *ui_widget_value(50) = (uint8)(1);
        *ui_widget_value(51) = (uint8)(0);
        value = mission_flow.scene_archive.row;
        *ui_widget_value(52) = (uint8)(value);
        scene_archive_load_page(mission_flow.scene_archive.page);
        scene_archive_load_detail(mission_flow.scene_archive.row);
        mission_flow.root_phase = 1;
        ui_group_set_mask(0, 11);
        ui_animation_set(1, 1, 0, 120);
        ui_widget_set(50, 1);
        game_runtime.progress.sequence_idx = (uint8)(0);
        value = mission_flow.root_variant;
        ui_widget_set(1, value + 1);
    }
    value = (sint16)mission_flow.transition_phase;
    return mission_dispatch_loading(value, 0, (RESOURCE_STREAM_BUFFER){0});
}

static void archive_browser_load_page_context(uint32 page, const MISSION_ARCHIVE_BROWSER *browser)
{
    uint32 count, quotient, remaining, index, position, id;

    FUNCTION_MARKER(0x800CF56C, "ACE3.BIN");
    count = browser->count;
    quotient = ((sint32)count / 4);
    remaining = (sint32)page < (sint32)quotient ? 4 : count - quotient * 4;
    if ((sint32)remaining <= 0)
    {
        (void)(uint32)(quotient * 4);
        return;
    }
    for (index = 0; (sint32)index < (sint32)remaining; ++index)
    {
        count = browser->count;
        position = page * 4 + index;
        if ((sint32)position >= (sint32)count)
            continue;
        if (position >= sizeof(browser->ids))
            abort();
        id = browser->ids[position];
        scene_texture_load_image_at(&mission_flow.assets.row_image_table, id, 960, 340 + index * 18, 384, 6);
    }
    return;
}

void archive_browser_load_page(uint32 page)
{
    FUNCTION_MARKER(0x800CF564, "ACE3.BIN");
    {
        (void)(archive_browser_load_page_context(page, &mission_flow.archive_browser));
        return;
    }
}

static void archive_browser_load_detail_context(uint32 item, const MISSION_ARCHIVE_BROWSER *browser)
{
    uint32 page, count, index, id;

    FUNCTION_MARKER(0x800CF648, "ACE3.BIN");
    page = browser->page;
    count = browser->count;
    index = item + page * 4;
    if ((sint32)index >= (sint32)count)
    {
        (void)(uint32)(page * 4);
        return;
    }
    if (index >= sizeof(browser->ids))
        abort();
    id = browser->ids[index];
    {
        (void)(scene_texture_load_image_at(&mission_flow.assets.detail_image_table, id, 960, 256, 768, 1));
        return;
    }
}

void archive_browser_load_detail(uint32 item)
{
    FUNCTION_MARKER(0x800CF640, "ACE3.BIN");
    {
        (void)(archive_browser_load_detail_context(item, &mission_flow.archive_browser));
        return;
    }
}

void archive_browser_init(void)
{
    uint32 count, index, value;
    const uint8 *source;

    FUNCTION_MARKER(0x800D0588, "ACE3.BIN");
    count = mission_flow.menu_init_delay + 1;
    mission_flow.menu_init_delay = (uint32)(count);
    source = game_runtime.progress.archive_status;
    if ((sint32)count < 2)
        return;
    mission_flow.menu_init_delay = (uint32)(0);
    (void)arena_alloc(608);
    if (!gpu_register_packet_range(&archive_browser_packets, sizeof(archive_browser_packets)))
        abort();
    mission_flow.archive_browser.packet_link = gpu_packet_link((uintptr_t)&archive_browser_packets);
    mission_flow.archive_browser.packets = &archive_browser_packets;
    mission_flow.archive_browser.phase = (uint8)(0);
    mission_flow.archive_browser.exit_mode = (uint8)(0);
    mission_flow.archive_browser.row = (uint8)(0);
    mission_flow.archive_browser.page = (uint8)(0);
    mission_flow.root_phase = 1;
    *ui_widget_value(5) = (uint8)(0);
    ui_item_reset_motion(0, 3);
    ui_group_set_mask(0, 11);
    mission_flow.archive_browser.count = (uint32)(0);
    for (index = 0; index < 79; ++index)
    {
        mission_flow.archive_browser.ids[index] = (uint8)(0);
        if (source[index] == 0)
            continue;
        count = mission_flow.archive_browser.count;
        if (count >= sizeof(mission_flow.archive_browser.ids))
            abort();
        mission_flow.archive_browser.ids[count] = (uint8)(index);
        count = mission_flow.archive_browser.count;
        mission_flow.archive_browser.count = (uint32)(count + 1);
    }
    archive_browser_load_page(mission_flow.archive_browser.page);
    archive_browser_load_detail(mission_flow.archive_browser.row);
    for (index = 0; index < 3; ++index)
    {
        *ui_widget_value(50 + index) = (uint8)(0);
    }
    for (index = 0; index < 4; ++index)
    {
        count = mission_flow.archive_browser.count;
        if ((sint32)index < (sint32)count)
        {
            value = mission_flow.archive_browser.ids[index];
            value = source[value];
            *ui_widget_value(index + 53) = (uint8)(value);
            *ui_widget_value(index + 57) = (uint8)(1);
        }
        else
        {
            *ui_widget_value(index + 53) = (uint8)(0);
            *ui_widget_value(index + 57) = (uint8)(0);
        }
    }
    return;
}

void archive_browser_load_row(uint32 page, uint32 item)
{
    uint32 id;

    FUNCTION_MARKER(0x800CF504, "ACE3.BIN");
    if (page * 4 + item >= sizeof(mission_flow.archive_browser.ids))
        abort();
    id = mission_flow.archive_browser.ids[page * 4 + item];
    {
        (void)(scene_texture_load_image_at(&mission_flow.assets.row_image_table, id, 960, 340, 384, 6));
        return;
    }
}

uint32 archive_browser_enter_list(void)
{
    const uint32 fields[4] = {2, 50, 51, 52};
    uint32 index, count, value;
    const uint8 *source;

    FUNCTION_MARKER(0x800CF854, "ACE3.BIN");
    mission_flow.archive_browser.phase = (uint8)(1);
    value = mission_flow.archive_browser.page;
    source = game_runtime.progress.archive_status;
    archive_browser_load_page(value);
    for (index = 0; index < 4; ++index)
    {
        *ui_widget_value(fields[index]) = (uint8)(0);
    }
    ui_widget_set(49, 0);
    mission_flow.menu_arrow_delay = 0;
    for (index = 0; index < 4; ++index)
    {
        count = mission_flow.archive_browser.count;
        if ((sint32)index < (sint32)count)
        {
            value = mission_flow.archive_browser.ids[index];
            value = source[value];
            *ui_widget_value(index + 53) = (uint8)(value);
            *ui_widget_value(index + 57) = (uint8)(1);
        }
        else
        {
            *ui_widget_value(index + 53) = (uint8)(0);
            *ui_widget_value(index + 57) = (uint8)(0);
        }
    }
    return 0;
}

void archive_browser_transition(void)
{
    uint32 mode, page, item, buffer;

    FUNCTION_MARKER(0x800CFF5C, "ACE3.BIN");
    mode = mission_flow.archive_browser.exit_mode;
    if (mode == 0)
    {
        mission_flow.archive_browser.phase = (uint8)(3);
        ui_item_reset_motion(0, 4);
        ui_animation_set(11, 1, 0, 25);
        ui_group_set_mask(0, 0x4011);
        page = mission_flow.archive_browser.page;
        item = mission_flow.archive_browser.row;
        archive_browser_load_row(page, item);
        *ui_widget_value(3) = (uint8)(0);
        return;
    }
    if (mode != 1)
        return;
    ui_group_set_mask(0, 3);
    mission_flow.root_kind = 0;
    mission_flow.root_phase = 0;
    buffer = mission_flow.assets.selection_buffer.encoded_begin;
    mission_flow.transition_phase = 0;
    {
        arena_set_cursor(buffer);
        return;
    }
}

typedef struct
{
    uint32 textures;
} ARCHIVE_DETAIL_RESOURCES;

typedef char archive_detail_resources_layout_check[sizeof(ARCHIVE_DETAIL_RESOURCES) == 4 ? 1 : -1];

uint32 archive_browser_wait_entry(void)
{
    uint32 page, item, id;
    const ARCHIVE_DETAIL_RESOURCES *entries;
    uint8 *data;
    uint32 bytes, texture_offset;

    FUNCTION_MARKER(0x800D0034, "ACE3.BIN");
    page = mission_flow.archive_browser.page;
    item = mission_flow.archive_browser.row;
    if (page * 4 + item >= sizeof(mission_flow.archive_browser.ids))
        abort();
    id = mission_flow.archive_browser.ids[page * 4 + item];
    game_runtime.progress.archive_status[id] = 2;
    ui_widget_set(49, 0);
    mission_flow.archive_browser.detail_icons = (uint32)(0);
    if (game_runtime.resource_pending != 0)
        return 0x800C0000;
    data = game_runtime.resource_slot.data;
    bytes = game_runtime.resource_slot.read_bytes;
    if (!data || bytes < 4 + sizeof(*entries))
        abort();
    (void)resource_relocate_directory(game_runtime.resource_slot.encoded_base, 0, data, bytes);
    entries = (const ARCHIVE_DETAIL_RESOURCES *)(data + 4);
    texture_offset = entries->textures - game_runtime.resource_slot.encoded_base;
    if (entries->textures < game_runtime.resource_slot.encoded_base || texture_offset > bytes)
        abort();
    scene_texture_load_archive_group(data + texture_offset, bytes - texture_offset);
    arena_set_cursor(entries->textures);
    mission_flow.archive_browser.scroll_dir = (uint32)(0);
    *ui_widget_value(1) = (uint8)(0);
    *ui_widget_value(2) = (uint8)(0);
    ui_animation_set(3, 0, 0, 0);
    ui_animation_set(4, 0, 0, 0);
    mission_flow.archive_browser.phase = (uint8)(4);
    return 4;
}

void archive_browser_reset_detail(void)
{
    FUNCTION_MARKER(0x800CF6B0, "ACE3.BIN");
    mission_flow.archive_browser.detail_page = (uint8)(0);
}

void archive_browser_draw_detail(void)
{
    uint32 index, packet, page, head, tag, link, base, offset;
    SPRT *sprites, *spr;
    ARCHIVE_BROWSER_PACKET_BANK *native_bank;
    DR_MODE *mode;
    uint32 *ordering;

    FUNCTION_MARKER(0x800CF6C0, "ACE3.BIN");
    base = mission_flow.archive_browser.packet_cursor;
    offset = base - mission_flow.archive_browser.packet_link;
    head = ui_packets.scene_ot;
    if ((offset != 0 && offset != 252) || ((base | head) & 3) || !mission_flow.archive_browser.packets)
        abort();
    native_bank = &mission_flow.archive_browser.packets->banks[offset == 252];
    sprites = native_bank->sprites;
    ordering = (uint32 *)ui_scene_ot_span(head, sizeof(*ordering));
    for (index = 0; index < 11; ++index)
    {
        packet = base + index * (uint32)sizeof(*spr);
        spr = sprites + index;
        setSprt(spr);
        spr->p[0].xy.x = 101;
        spr->w = 144;
        spr->p[0].xy.y = (sint16)(uint16)(120 + index * 24);
        spr->rgbc.r = 128;
        spr->rgbc.g = 128;
        spr->rgbc.b = 128;
        spr->p[0].uv.u = 0;
        spr->p[0].uv.v = (uint8)(index * 18);
        spr->h = 18;
        page = mission_flow.archive_browser.detail_page;
        spr->p[0].attr = (uint16)(((page + 210) << 6) | 63);
        tag = spr->tag;
        link = *ordering;
        spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ordering;
        *ordering = (link & 0xFF000000) | (packet & 0xFFFFFF);
    }
    page = mission_flow.archive_browser.detail_page;
    packet = base + 240;
    page = ((((page >> 1) << 6) + 640) & 1023) >> 6;
    mode = &native_bank->mode;
    SetDrawMode(mode, 0, 0, page | 16, NULL);
    tag = mode->tag;
    link = *ordering;
    mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ordering;
    link = (link & 0xFF000000) | (packet & 0xFFFFFF);
    *ordering = link;
    {
        (void)(uint32)(link);
        return;
    }
}

uint32 archive_browser_update_list(uint32 countdown)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 count, page, item, last, value, flags;
    uint32 buffer, index, resource, size;
    const uint8 *source;

    FUNCTION_MARKER(0x800CF9B0, "ACE3.BIN");
    if (countdown != 0)
        mission_flow.menu_arrow_delay = countdown - 1;
    else
    {
        count = mission_flow.archive_browser.count;
        page = mission_flow.archive_browser.page;
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        flags = (sint32)page < ((sint32)last >> 2) ? 2 : 0;
        if (page != 0)
            flags |= 1;
        ui_widget_set(49, flags);
        *ui_widget_value(51) = (uint8)(0);
    }
    if ((game_input.repeat & 2) != 0)
    {
        page = mission_flow.archive_browser.page;
        item = mission_flow.archive_browser.row;
        count = mission_flow.archive_browser.count;
        if ((sint32)(item + page * 4) < (sint32)(count - 1))
        {
            if (item < 3)
                mission_flow.archive_browser.row = (uint8)(item + 1);
            else
            {
                mission_flow.archive_browser.row = (uint8)(0);
                page = mission_flow.archive_browser.page;
                mission_flow.archive_browser.page = (uint8)(page + 1);
                archive_browser_load_page(mission_flow.archive_browser.page);
                *ui_widget_value(51) = (uint8)(2);
                mission_flow.menu_arrow_delay = 12;
            }
            sound_play(61);
            archive_browser_load_detail(mission_flow.archive_browser.row);
        }
    }
    if ((game_input.repeat & 1) != 0)
    {
        item = mission_flow.archive_browser.row;
        if (item != 0)
            mission_flow.archive_browser.row = (uint8)(item - 1);
        else
        {
            page = mission_flow.archive_browser.page;
            if (page == 0)
                goto selection_next_page;
            mission_flow.archive_browser.row = (uint8)(3);
            page = mission_flow.archive_browser.page;
            mission_flow.archive_browser.page = (uint8)(page - 1);
            archive_browser_load_page(mission_flow.archive_browser.page);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
        }
        sound_play(61);
        archive_browser_load_detail(mission_flow.archive_browser.row);
    }
selection_next_page:
    if ((game_input.repeat & 512) != 0)
    {
        count = mission_flow.archive_browser.count;
        page = mission_flow.archive_browser.page;
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        if ((sint32)page < ((sint32)last >> 2))
        {
            mission_flow.archive_browser.page = (uint8)(page + 1);
            page = mission_flow.archive_browser.page;
            item = mission_flow.archive_browser.row;
            count = mission_flow.archive_browser.count;
            if ((sint32)(item + page * 4) >= (sint32)count)
            {
                last = count - 1;
                value = (sint32)last < 0 ? count + 2 : last;
                mission_flow.archive_browser.row = (uint8)(last - ((uint32)((sint32)value >> 2) << 2));
            }
            sound_play(61);
            archive_browser_load_page(mission_flow.archive_browser.page);
            archive_browser_load_detail(mission_flow.archive_browser.row);
            *ui_widget_value(51) = (uint8)(2);
            mission_flow.menu_arrow_delay = 12;
        }
    }
    if ((game_input.repeat & 256) != 0)
    {
        page = mission_flow.archive_browser.page;
        if (page != 0)
        {
            mission_flow.archive_browser.page = (uint8)(page - 1);
            sound_play(61);
            archive_browser_load_page(mission_flow.archive_browser.page);
            archive_browser_load_detail(mission_flow.archive_browser.row);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
        }
    }
    if ((game_input.pressed & 16) != 0 && (game_input.held & 47) == 0)
    {
        sound_play(60);
        archive_browser_reset_detail();
        mission_flow.archive_browser.phase = (uint8)(2);
        mission_flow.archive_browser.exit_mode = (uint8)(0);
        page = mission_flow.archive_browser.page;
        item = mission_flow.archive_browser.row;
        if (item + page * 4 >= sizeof(mission_flow.archive_browser.ids))
            abort();
        value = mission_flow.archive_browser.ids[item + page * 4];
        if (value >= sizeof(archive_details) / sizeof(archive_details[0]))
            abort();
        resource = 186 + value;
        game_runtime.resource_slot.resource_id = (uint32)(resource);
        size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
        asset_allocation = arena_alloc_backed(size);
        buffer = asset_allocation.encoded_begin;
        game_runtime.resource_pending = 1;
        resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    }
    if ((game_input.pressed & 32) != 0 && (game_input.held & 95) == 0)
    {
        sound_play(59);
        mission_flow.archive_browser.phase = (uint8)(2);
        mission_flow.archive_browser.exit_mode = (uint8)(1);
    }
    item = mission_flow.archive_browser.row;
    source = game_runtime.progress.archive_status;
    *ui_widget_value(52) = (uint8)(item);
    for (index = 0; index < 4; ++index)
    {
        page = mission_flow.archive_browser.page;
        count = mission_flow.archive_browser.count;
        value = index + page * 4;
        if ((sint32)value < (sint32)count)
        {
            value = mission_flow.archive_browser.ids[value];
            value = source[value];
            *ui_widget_value(index + 53) = (uint8)(value);
            *ui_widget_value(index + 57) = (uint8)(1);
        }
        else
        {
            *ui_widget_value(index + 53) = (uint8)(0);
            *ui_widget_value(index + 57) = (uint8)(0);
        }
    }
    return 0;
}

uint32 archive_browser_input(void)
{
    uint32 countdown;

    FUNCTION_MARKER(0x800CF9A8, "ACE3.BIN");
    countdown = mission_flow.menu_arrow_delay;
    return archive_browser_update_list(countdown);
}

uint32 archive_browser_update_detail(void)
{
    uint32 page, item, id, count, last, flags, result;
    const ARCHIVE_DETAIL_ENTRY *entry;

    FUNCTION_MARKER(0x800D013C, "ACE3.BIN");
    page = mission_flow.archive_browser.page;
    item = mission_flow.archive_browser.row;
    if (page * 4 + item >= sizeof(mission_flow.archive_browser.ids))
        abort();
    id = mission_flow.archive_browser.ids[page * 4 + item];
    if (id >= sizeof(archive_details) / sizeof(archive_details[0]))
        abort();
    entry = archive_details + id;
    count = entry->page_count;
    *ui_widget_value(51) = (uint8)(0);
    item = mission_flow.archive_browser.detail_page;
    last = count - 1;
    flags = (sint32)item < (sint32)last ? 2 : 0;
    if (item != 0)
        flags |= 1;
    ui_widget_set(49, flags);
    if (entry->page_count != 0)
    {
        archive_browser_draw_detail();
        if ((game_input.pressed & 2) != 0)
        {
            item = mission_flow.archive_browser.detail_page;
            if ((sint32)item < (sint32)last)
            {
                mission_flow.archive_browser.scroll_dir = (uint32)(1);
                *ui_widget_value(51) = (uint8)(2);
                sound_play(61);
                item = mission_flow.archive_browser.detail_page;
                mission_flow.archive_browser.detail_page = (uint8)(item + 1);
                *ui_widget_value(51) = (uint8)(2);
            }
        }
        if ((game_input.pressed & 1) != 0)
        {
            item = mission_flow.archive_browser.detail_page;
            if (item != 0)
            {
                mission_flow.archive_browser.scroll_dir = (uint32)(2);
                *ui_widget_value(51) = (uint8)(1);
                sound_play(61);
                item = mission_flow.archive_browser.detail_page;
                mission_flow.archive_browser.detail_page = (uint8)(item - 1);
                *ui_widget_value(51) = (uint8)(1);
            }
        }
        if (ui_animation_phase(5) == 0)
            ui_animation_set(5, 1, 0, 10);
        item = mission_flow.archive_browser.detail_page;
        if (item >= sizeof(entry->widget_codes) / sizeof(entry->widget_codes[0]))
            abort();
        result = entry->widget_codes[item][0];
        *ui_widget_value(1) = (uint8)(result);
        item = mission_flow.archive_browser.detail_page;
        if (item >= sizeof(entry->widget_codes) / sizeof(entry->widget_codes[0]))
            abort();
        if (entry->widget_codes[item][0] != 0 && ui_animation_phase(3) == 0)
        {
            ui_animation_set(3, 1, 0, 10);
            mission_flow.archive_browser.detail_icons = (uint32)(1);
        }
        item = mission_flow.archive_browser.detail_page;
        if (item >= sizeof(entry->widget_codes) / sizeof(entry->widget_codes[0]))
            abort();
        result = entry->widget_codes[item][1];
        *ui_widget_value(2) = (uint8)(result);
        item = mission_flow.archive_browser.detail_page;
        if (item >= sizeof(entry->widget_codes) / sizeof(entry->widget_codes[0]))
            abort();
        if (entry->widget_codes[item][1] != 0 && ui_animation_phase(4) == 0)
        {
            ui_animation_set(4, 1, 0, 10);
            mission_flow.archive_browser.detail_icons = (uint32)(2);
        }
        result = (uint8)mission_flow.archive_browser.detail_icons;
        *ui_widget_value(3) = (uint8)(result);
    }
    else
    {
        *ui_widget_value(3) = (uint8)(3);
    }
    result = game_input.pressed & 32;
    if (result != 0)
    {
        result = game_input.held & 95;
        if (result == 0)
        {
            sound_play(59);
            mission_flow.archive_browser.phase = (uint8)(5);
            result = 5;
        }
    }
    return result;
}

void archive_browser_leave_detail(void)
{
    uint32 mode;

    FUNCTION_MARKER(0x800D04F4, "ACE3.BIN");
    mission_flow.archive_browser.phase = (uint8)(0);
    ui_item_reset_motion(0, 3);
    ui_group_set_mask(0, 11);
    ui_animation_set(5, 0, 0, 0);
    ui_animation_set(3, 0, 0, 0);
    ui_animation_set(4, 0, 0, 0);
    mode = mission_flow.root_variant;
    {
        ui_widget_set(1, mode + 1);
        return;
    }
}

void archive_browser_dispatch(void)
{
    static const MISSION_DETAIL_PHASE callbacks[] = {
        {archive_browser_enter_list, NULL, NULL}, {archive_browser_input, NULL, NULL}, {NULL, NULL, archive_browser_transition}, {archive_browser_wait_entry, NULL, NULL}, {archive_browser_update_detail, NULL, NULL}, {NULL, NULL, archive_browser_leave_detail},
    };
    uint32 bank, mode;

    FUNCTION_MARKER(0x800D07C0, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    mission_flow.archive_browser.packet_cursor = mission_flow.archive_browser.packet_link + bank * 252;
    mode = mission_flow.archive_browser.phase;
    if (mode >= sizeof(callbacks) / sizeof(callbacks[0]))
        abort();
    (void)mission_detail_call(callbacks + mode, 0, NULL);
}

uint32 mission_dispatch_scene_request(void)
{
    uint32 counter, requests, mode, flags, parameter, mission, value;
    FUNCTION_MARKER(0x80037E50, "ACE3.BIN");
    counter = mission_flow.request.dispatch_count;
    mission_flow.request.dispatch_count = (uint8)(counter + 1);
    requests = mission_flow.request.flags;
    if ((requests & 1) != 0)
    {
        mission_flow.request.flags = (uint8)(requests & 0xFE);
        return mission_request_mode(128, 0);
    }
    if ((requests & 2) != 0)
    {
        mission_flow.request.flags = (uint8)(requests & 0xFD);
        return mission_request_mode(0x100000, 0);
    }
    if ((requests & 4) != 0)
    {
        mission_flow.request.flags = (uint8)(requests & 0xFB);
        camera_set_scripted_mode();
        return mission_request_mode(0x200000, 0);
    }
    mode = mission_flow.request.mode;
    switch (mode)
    {
        case 0:
            parameter = mission_flow.request.mission_arg;
            flags = 2;
            mission_flow.request.current_mission_arg = (uint8)(parameter);
            break;
        case 1:
            flags = 16;
            parameter = mission_flow.request.script_arg;
            mission = (sint16)game_runtime.selection.mission;
            scene_script_select_playback(mission, parameter);
            break;
        case 2:
            flags = 0x4000;
            parameter = mission_flow.request.movie_arg;
            mission = (sint16)game_runtime.selection.mission;
            scene_script_select_movie(mission, parameter);
            break;
        case 3:
            parameter = mission_flow.request.scene_arg;
            flags = 1024;
            break;
        case 4:
            mission = (sint16)game_runtime.selection.mission;
            flags = 0x1000000;
            game_runtime.playback.return_mode = (uint32)(3);
            value = mission_flow.request.movie_arg;
            parameter = 0;
            scene_script_select_movie(mission, value);
            mission_result_finish();
            mission_audio_stop_groups();
            break;
        case 5:
            flags = 0x2000000;
            mission_result_finish();
            value = game_runtime.mission_result.route_mission;
            game_runtime.selection.mission = (uint16)(value);
            mission_audio_stop_groups();
            value = mission_flow.request.mode;
            parameter = 0;
            if (value == mode)
            {
                mission = (sint16)game_runtime.selection.mission;
                if (mission == 54)
                {
                    game_runtime.selection.weapon_code[0] = (uint8)(mode);
                    game_runtime.selection.aircraft = (uint16)(25);
                    game_runtime.selection.weapon_code[1] = (uint8)(1);
                    game_runtime.selection.weapon_code[2] = (uint8)(1);
                }
            }
            break;
        case 9:
            flags = 0x800000;
            parameter = 0;
            mission_result_finish();
            mission_audio_stop_groups();
            break;
        default:
            return 0;
    }
    return mission_request_mode(flags, parameter);
}

uint32 mission_flow_resolve_faction(sint32 mission)
{
    FUNCTION_MARKER(0x800FB2A0, "ACE3.BIN");
    if (mission == 0)
    {
        mission_flow.faction = (uint32)(1);
        return 1;
    }
    if (mission < 20)
        return 0;
    if (mission < 35)
        return 1;
    if (mission < 40)
        return 3;
    if (mission < 45)
        return 2;
    if (mission < 49)
        return 3;
    if (mission < 54)
        return 2;
    if (mission == 54)
        return 1;
    if (mission < 57)
        return 2;
    if (mission < 59)
        return 3;
    return 0;
}

void mission_debrief_clear_markers(void)
{
    uint32 i;
    FUNCTION_MARKER(0x800F5AC4, "ACE3.BIN");
    for (i = 0; i < 64; ++i)
    {
        mission_debrief.markers[i].visible = (uint8)(0);
    }
    return;
}

void mission_debrief_clear_event_rows(void)
{
    uint32 i;
    FUNCTION_MARKER(0x800F5FF8, "ACE3.BIN");
    for (i = 0; i < 8; ++i)
    {
        mission_debrief.kind_rows[i].count = (uint8)(0);
        mission_debrief.kind_rows[i].kind = (uint8)(0);
        mission_debrief.kind_rows[i].label[0] = (uint8)(0);
    }
    for (i = 0; i < 8; ++i)
    {
        mission_debrief.special_rows[i].count = (uint8)(0);
        mission_debrief.special_rows[i].kind = (uint8)(0);
        mission_debrief.special_rows[i].label[0] = (uint8)(0);
    }
    mission_debrief.kind_count = (uint8)(0);
    mission_debrief.special_count = (uint8)(0);
    return;
}

uint32 mission_frontend_initialize(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 flags, value, i, j;
    sint32 mode;
    PSX_RECT first_rect = {320, 0, 32, 128};
    PSX_RECT second_rect = {320, 348, 16, 1};
    FUNCTION_MARKER(0x800F60F0, "ACE3.BIN");
    game_runtime.surfaces.movie_draw.isbg = (uint8)(1);
    flags = game_runtime.display.flags;
    game_runtime.display.mode = (uint32)(1);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.flags = (uint32)(flags | 9);
    resource_init_queue(0, 0);
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    subtitles_initialize();
    buffer_allocation = arena_alloc_backed(644);
    value = buffer_allocation.encoded_begin;
    memcpy(&game_runtime.menu_confirmation_gate, buffer_allocation.buffer.data + 144, sizeof(game_runtime.menu_confirmation_gate));
    menu_voices_reset();
    for (i = 0; i < 2; ++i)
    {
        (void)arena_alloc(0x8000);
        ui_bind_scene_packet_page(i);
    }
    value = game_runtime.mission_result.mission;
    if (value == 54)
    {
        game_runtime.mission_result.mission = 34;
        value = game_runtime.mission_result.mission;
    }
    if (value == 58)
        game_runtime.mission_result.mission = 39;
    value = game_runtime.mission_result.route_mission;
    if (value == 56)
    {
        game_runtime.mission_result.route_mission = 42;
        value = game_runtime.mission_result.route_mission;
    }
    if (value == 57)
        game_runtime.mission_result.route_mission = 37;
    if (game_runtime.account.mission_failed == 1)
    {
        value = game_runtime.selection.mission;
        if (value >= GAME_MISSION_COUNT)
            abort();
        value = resource002_failed_mission[value];
        game_runtime.account.route_mode = (uint16)(0);
        game_runtime.selection.mission = (uint16)(value);
    }
    value = mission_flow_resolve_faction((sint16)game_runtime.selection.mission);
    mission_flow.faction = (uint32)(value);
    for (i = 0; i < 3; ++i)
    {
        value = arena_alloc(384);
        for (j = 0; j < 32; ++j)
        {
            ui_item_reset_motion(i, j);
        }
    }
    buffer_allocation = arena_alloc_backed(16);
    value = buffer_allocation.encoded_begin;
    MISSION_GRAPH_SETUP_PUBLICATION *graph_setup = (MISSION_GRAPH_SETUP_PUBLICATION *)buffer_allocation.buffer.data;
    graph_setup->marker = 0xFFFF;
    for (i = 0; i < 2; ++i)
    {
        (void)arena_alloc(64);
        (void)arena_alloc(1024);
        ui_bind_scene_ordering(i);
    }
    for (i = 0; i < 2; ++i)
    {
        (void)arena_alloc(4764);
        ui_bind_scene_primitives(i);
    }
    ui_packets_initialize();
    ui_group_bind(ui_boot_commands, 2);
    ui_group_set_mask(0, 0);
    ui_group_set_mask(1, 0);
    ui_group_set_mask(2, 1);
    text_renderer_init();
    value = arena_alloc(700);
    game_runtime.scene_initialized = (value != 0);
    mission_card_reserve_records();
    mission_audio_reset_sequence();
    mission_audio_bind_tracks(&audio_tracks_debrief);
    mission_flow.transition_phase = 0;
    sound_enable_reverb();
    if (game_runtime.progress.flow_flags & 0x10)
    {
        mission_aircraft.group = 0;
        if ((sint16)game_runtime.account.route_mode == 1)
            game_runtime.account.route_mode = (uint16)(3);
        if (game_runtime.progress.flow_flags & 0x10)
            game_runtime.selection.mission = (uint16)(59);
    }
    if ((sint16)game_runtime.account.route_mode == 1)
    {
        value = game_runtime.mission_result.mission;
        switch (value)
        {
            case 18:
            case 34:
            case 39:
            case 48:
            case 53:
                flags = game_runtime.progress.flow_flags;
                game_runtime.progress.flow_flags = (uint32)(flags | 8);
                mission_flow.account_mode = (uint32)(2);
                value = game_runtime.mission_result.mission;
                if (mission_debrief_special_missions_complete(value))
                {
                    flags = game_runtime.progress.flow_flags;
                    if (flags & 0x20)
                    {
                        game_runtime.progress.flow_flags = (uint32)(flags & ~0x20u);
                        mission_flow.account_mode = (uint32)(3);
                    }
                    else
                        game_runtime.progress.flow_flags = (uint32)(flags | 0x20);
                }
                break;
            default:
                flags = game_runtime.progress.flow_flags;
                game_runtime.progress.flow_flags = (uint32)(flags & ~8u);
                break;
        }
        if (game_runtime.progress.flow_flags & 8)
        {
            mission_debrief_commit_result();
            return 0x04000000;
        }
    }
    if (game_runtime.progress.movie_branch == 1)
        return 0x04000000;
    if (game_runtime.progress.flow_flags & 1)
        return 0x04000000;
    if (game_runtime.account.mission_failed == 1)
        return 0x04000000;
    mode = (sint16)game_runtime.account.route_mode;
    if (mode == 1)
    {
        flags = game_runtime.progress.flow_flags;
        value = game_runtime.mission_result.route_mission;
        game_runtime.selection.mission = (uint16)(value);
        if (flags & 0x10)
            game_runtime.selection.mission = (uint16)(59);
    }
    else if (mode == 0 || mode == 2)
    {
        if (game_runtime.progress.flow_flags & 0x10)
            game_runtime.selection.mission = (uint16)(59);
        return 0x04000000;
    }
    else if (mode == 3)
        game_runtime.selection.mission = (uint16)(59);
    mission_debrief_commit_result();
    value = arena_alloc(984);
    mission_debrief.phase = 0;
    mission_debrief.event_idx = 0;
    mission_debrief.revealed_seconds = 0;
    mission_debrief.phase = 0;
    mission_debrief.delay_ticks = (uint16)(0);
    mission_flow.debrief_movie_phase = 0;
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(4096);
    }
    mission_debrief_clear_markers();
    mission_debrief_clear_event_rows();
    text_select_font(7);
    text_set_blend(1);
    text_set_reveal_range(1, 1, 1);
    text_set_reveal_step(1);
    MoveImage(&first_rect, 960, 256);
    DrawSync(0);
    MoveImage(&second_rect, 960, 384);
    DrawSync(0);
    return 0x04000000;
}

uint32 mission_debrief_special_missions_complete(uint32 mission)
{
    FUNCTION_MARKER(0x800F5B04, "ACE3.BIN");
    if ((sint16)game_runtime.account.route_mode != 1)
        return 0;
    if (!game_runtime.progress.mission_rating[0][34] && !game_runtime.progress.mission_rating[1][34] && !game_runtime.progress.mission_rating[2][34] && !game_runtime.progress.mission_rating[3][34] && mission != 34)
        return 0;
    if (!game_runtime.progress.mission_rating[0][39] && !game_runtime.progress.mission_rating[1][39] && !game_runtime.progress.mission_rating[2][39] && !game_runtime.progress.mission_rating[3][39] && mission != 39)
        return 0;
    if (!game_runtime.progress.mission_rating[0][18] && !game_runtime.progress.mission_rating[1][18] && !game_runtime.progress.mission_rating[2][18] && !game_runtime.progress.mission_rating[3][18] && mission != 18)
        return 0;
    if (!game_runtime.progress.mission_rating[0][53] && !game_runtime.progress.mission_rating[1][53] && !game_runtime.progress.mission_rating[2][53] && !game_runtime.progress.mission_rating[3][53] && mission != 53)
        return 0;
    if (game_runtime.progress.mission_rating[0][48])
        return 1;
    if (game_runtime.progress.mission_rating[1][48])
        return 1;
    if (game_runtime.progress.mission_rating[2][48])
        return 1;
    if (game_runtime.progress.mission_rating[3][48])
        return 1;
    return mission == 48;
}

void mission_debrief_commit_result(void)
{
    uint32 mission, difficulty, rating;
    uint32 value, half, flags, total, count;
    sint32 signed_half;
    FUNCTION_MARKER(0x800F5CA4, "ACE3.BIN");
    mission = game_runtime.mission_result.mission;
    difficulty = game_runtime.selection.difficulty;
    rating = game_runtime.mission_result.rating;
    signed_half = (sint16)game_runtime.mission_result.elapsed_ticks;
    half = game_runtime.mission_result.elapsed_ticks;
    if (signed_half != 0)
    {
        game_runtime.progress.mission_time[4][mission] = (uint16)(half);
        if (difficulty == 0 && signed_half < (sint32)game_runtime.progress.mission_time[0][mission])
            game_runtime.progress.mission_time[0][mission] = (uint16)(half);
        if (difficulty == 1)
        {
            if ((sint16)half < (sint32)game_runtime.progress.mission_time[1][mission])
                game_runtime.progress.mission_time[1][mission] = (uint16)(half);
        }
        if (difficulty == 2)
        {
            if ((sint16)half < (sint32)game_runtime.progress.mission_time[2][mission])
                game_runtime.progress.mission_time[2][mission] = (uint16)(half);
        }
        if (difficulty == 3)
        {
            if ((sint16)half < (sint32)game_runtime.progress.mission_time[3][mission])
                game_runtime.progress.mission_time[3][mission] = (uint16)(half);
        }
        total = game_runtime.progress.total_time;
        game_runtime.progress.total_time = (uint32)(total + (uint32)(sint32)(sint16)half);
    }
    mission_unlock_aircraft((uint32)(sint16)game_runtime.selection.mission);
    mission_unlock_weapons((uint32)(sint16)game_runtime.selection.mission);
    game_runtime.progress.mission_rating[4][mission & 0xFF] = (uint8)(rating);
    if (difficulty == 0)
    {
        value = game_runtime.progress.mission_rating[0][mission & 0xFF];
        if (value == 0 || (rating & 0xFF) < value)
            game_runtime.progress.mission_rating[0][mission & 0xFF] = (uint8)(rating);
    }
    if (difficulty == 1)
    {
        value = game_runtime.progress.mission_rating[1][mission & 0xFF];
        if (value == 0 || (rating & 0xFF) < value)
            game_runtime.progress.mission_rating[1][mission & 0xFF] = (uint8)(rating);
    }
    if (difficulty == 2)
    {
        value = game_runtime.progress.mission_rating[2][mission & 0xFF];
        if (value == 0 || (rating & 0xFF) < value)
            game_runtime.progress.mission_rating[2][mission & 0xFF] = (uint8)(rating);
    }
    if (difficulty == 3)
    {
        value = game_runtime.progress.mission_rating[3][mission & 0xFF];
        if (value == 0 || (rating & 0xFF) < value)
            game_runtime.progress.mission_rating[3][mission & 0xFF] = (uint8)(rating);
    }
    if ((rating & 0xFF) < 6)
    {
        value = rating & 0xFF;
        *ui_widget_value(1) = (uint8)(value);
    }
    *ui_widget_value(1) = (uint8)(rating);
    value = game_runtime.mission_result.event_count;
    game_runtime.progress.mission_count[mission & 0xFF] = (uint32)(value);
    count = game_runtime.mission_result.event_count;
    total = game_runtime.progress.total_count;
    flags = (sint16)game_runtime.account.loss_count;
    game_runtime.progress.total_count = (uint32)(total + count);
    total = game_runtime.progress.mission_losses[mission & 0xFF];
    game_runtime.progress.mission_losses[mission & 0xFF] = (uint32)(total + flags);
    flags = (sint16)game_runtime.account.loss_count;
    total = game_runtime.progress.total_losses + flags;
    game_runtime.progress.total_losses = (uint32)(total);
    {
        (void)(uint32)(total);
        return;
    }
}

uint32 mission_debrief_draw_recent_trace(uint32 packet)
{
    DR_MODE *draw_mode;
    uint32 x, y, i, fade, shade, previous, write_packet;
    uint32 current_x, current_y, next_x, next_y, ordering, tag, link, result;
    sint32 sample, distance;
    FUNCTION_MARKER(0x800F4DF8, "ACE3.BIN");
    sample = (sint16)mission_debrief.revealed_seconds / 4;
    if (sample < 0)
        sample = 0;
    if (sample >= game_runtime.mission_result.sample_count)
        sample = game_runtime.mission_result.sample_count ? game_runtime.mission_result.sample_count - 1 : 0;
    x = game_runtime.mission_result.trace[sample].x;
    y = game_runtime.mission_result.trace[sample].y;
    fade = 2;
    shade = 128;
    write_packet = packet - 20;
    for (i = 0; i < 40; ++i)
    {
        distance = ((uint32)sample - i);
        if (distance > 0)
        {
            if (distance < (sint16)game_runtime.mission_result.sample_count)
            {
                current_x = game_runtime.mission_result.trace[distance].x - x + 83;
                current_y = (y - game_runtime.mission_result.trace[distance].y) * 2 + 192;
                next_x = game_runtime.mission_result.trace[distance - 1].x - x + 83;
                write_packet += 20;
                next_y = game_runtime.mission_result.trace[distance - 1].y;
                LINE_G2 *line = ui_packet_span(write_packet, sizeof(*line));
                ((uint8 *)&line->tag)[3] = (uint8)(4);
                line->p[0].rgbc.code = (uint8)(82);
                line->p[0].rgbc.r = (uint8)(shade);
                line->p[0].rgbc.g = (uint8)(shade);
                line->p[0].rgbc.b = (uint8)(shade);
                line->p[1].rgbc.r = (uint8)(128 - fade);
                line->p[1].rgbc.g = (uint8)(128 - fade);
                line->p[1].rgbc.b = (uint8)(128 - fade);
                line->p[0].xy.x = (sint16)(uint16)(current_x);
                line->p[0].xy.y = (sint16)(uint16)(current_y);
                line->p[1].xy.x = (sint16)(uint16)(next_x);
                line->p[1].xy.y = (sint16)(uint16)((y - next_y) * 2 + 192);
                ordering = ui_packets.scene_ot;
                tag = line->tag;
                link = *(uint32 *)ui_scene_ot_span(ordering, 4);
                line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
                previous = packet;
                ordering = ui_packets.scene_ot;
                packet += 20;
                tag = *(uint32 *)ui_scene_ot_span(ordering, 4);
                *(uint32 *)ui_scene_ot_span(ordering, 4) = (tag & 0xFF000000) | (previous & 0xFFFFFF);
            }
        }
        fade += 2;
        shade -= 2;
    }
    previous = packet;
    result = packet + 12;
    draw_mode = (DR_MODE *)ui_packet_span(previous, sizeof(*draw_mode));
    SetDrawMode(draw_mode, 0, 0, 32, NULL);
    ordering = ui_packets.scene_ot;
    tag = draw_mode->tag;
    link = *(uint32 *)ui_scene_ot_span(ordering, 4);
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    tag = *(uint32 *)ui_scene_ot_span(ordering, 4);
    *(uint32 *)ui_scene_ot_span(ordering, 4) = (tag & 0xFF000000) | (previous & 0xFFFFFF);
    return result;
}

uint32 mission_debrief_draw_trace(uint32 packet)
{
    FUNCTION_MARKER(0x800F4DF0, "ACE3.BIN");
    return mission_debrief_draw_recent_trace(packet);
}

void mission_debrief_tick_phase(void)
{
    uint32 value, mode;
    sint32 threshold;
    FUNCTION_MARKER(0x800F59B4, "ACE3.BIN");
    value = mission_debrief.delay_ticks;
    mode = mission_debrief.phase;
    value += 1;
    mission_debrief.delay_ticks = (uint16)(value);
    if (mode > 1)
        abort();
    threshold = mode == 0 ? 15 : 3599;
    if ((sint16)value < threshold)
        return;
    if (mode == 0)
        mission_debrief.phase = mode + 1;
    else
        mission_debrief.phase_done = (uint8)(1);
    mission_debrief.delay_ticks = (uint16)(0);
    return;
}

void mission_debrief_reveal_trace(void)
{
    uint32 limit, value;
    sint32 elapsed;
    FUNCTION_MARKER(0x800F5A24, "ACE3.BIN");
    if (mission_debrief.phase == 0)
        return;
    limit = game_runtime.mission_result.elapsed_ticks;
    elapsed = (sint16)mission_debrief.revealed_seconds;
    limit = (((unsigned long long)limit * 0x88888889u) >> 36);
    value = mission_debrief.revealed_seconds + 1;
    if (elapsed >= (sint32)(limit & 0xFFFF))
        return;
    mission_debrief.revealed_seconds = value;
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 mission_save_begin(void)
{
#if 0
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    /* Original save-account and memory-card screen sequence */
    uint32 value, index, packet_banks[2];
    FUNCTION_MARKER(0x800EB9EC, "ACE3.BIN");
    value = mission_flow.mode_phase;
    mission_flow.mode_phase = value + 1;
    menu->arena_bookmark = arena_alloc(28);
    for (index = 0; index < 2; ++index)
        packet_banks[index] = arena_alloc(8192);
    menu->screen = 0;
    menu->packet_cursor = packet_banks[game_runtime.display_buffer];
    menu->phase = 0;
    menu->exit_requested = 0;
    menu->confirmation = 0;
    menu->card_port = 0;
    menu->account_slot = 0;
    menu->mission_slot = 0;
    menu->ticks = 0;
    menu->show_both_ports = 1;
    menu->hide_prompt_sprite = 0;
    menu->hide_data = 0;
    menu->recovery_indicator = 0;
    menu->account_detail_mode = 0;
    mission_effect_open_trails();
    ui_group_bind(mission_flow.assets.save_screen_ui, 1);
    ui_group_set_mask(0, 0);
    ui_group_set_mask(1, 0);
    ui_group_set_mask(2, 0);
    scene_texture_load_group(mission_flow.assets.save_screen_assets, mission_flow.assets.frontend_source);
    scene_texture_load_group(mission_flow.assets.shared_menu_textures, mission_flow.assets.frontend_source);
    mission_card_initialize();
    mission_card_open_events();
    mission_card_begin_polling();
    ui_animation_set(15, 1, 0, 20);
    value = mission_effect.layers[MISSION_EFFECT_RING].mode;
    return mission_effect_set_ring(value ? 3 : 0);
#else
    FUNCTION_MARKER(0x800EB9EC, "ACE3.BIN");
    return game_account_save_requested();
#endif
}

uint32 mission_save_wait_ring(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800EBBD0, "ACE3.BIN");
    result = mission_effect.layers[MISSION_EFFECT_RING].mode;
    if (result == 0)
    {
        value = mission_flow.mode_phase;
        mission_flow.mode_phase = value + 1;
        result = game_runtime.display.background[0] == 143 ? 32 : 1;
        menu->ticks = (uint8)(result);
    }
    return result;
}

uint32 mission_save_fade_background(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    static const uint8 factors[] = {143, 135, 83};
    uint32 value, index, result;
    FUNCTION_MARKER(0x800EBC3C, "ACE3.BIN");
    value = menu->ticks - 1;
    menu->ticks = (uint8)(value);
    if ((value & 255) == 0)
    {
        value = mission_flow.mode_phase;
        mission_flow.mode_phase = value + 1;
    }
    for (index = 0; index < 3; ++index)
    {
        value = menu->ticks;
        value *= factors[index];
        game_runtime.display.background[index] = (uint8)(value / 32);
    }
    result = game_runtime.display.flags | 8;
    game_runtime.display.flags = (uint32)(result);
    return result;
}

uint32 mission_save_frame(void)
{
    FUNCTION_MARKER(0x800EBD48, "ACE3.BIN");
    return mission_save_update();
}

uint32 mission_save_update(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 mode, value, kind, phase, packet, ordering, result;
    sint32 tick, mission;
    FUNCTION_MARKER(0x800EBD50, "ACE3.BIN");
    mode = mission_flow.account_mode;
    if (mode == 0 || mode == 2 || mode == 3)
    {
        value = ui_packets.scene_packet_cursor;
        menu->packet_cursor = (uint32)(value);
    }
    else if (mode == 1 || mode == 4)
    {
        value = menu_state.packet_cursor;
        menu->packet_cursor = (uint32)(value);
    }
    mission_card_poll_frame();
    value = game_input.kind;
    ui_widget_set(40, (value & 16) != 0);
    if (menu->show_both_ports)
    {
        *ui_widget_value(16) = (uint8)(2);
    }
    else
        ui_widget_set(16, menu->card_port);
    ui_widget_set(17, 0);
    text_select_font(4);
    text_set_blend(2);
    text_set_reveal_range(1, 10, 20);
    value = ui_animation_ticks(15);
    text_set_reveal_step(value);
    text_set_color(128, 128, 128);
    text_set_pos(50, 73);
    text_print((const uint8 *)game_runtime.account.name, NULL, 0);
    kind = menu->screen;
    phase = menu->phase;
    mission_dispatch_selection(kind, phase);
    if (menu->hide_prompt_sprite != 1)
    {
        tick = (sint32)game_runtime.field_counter;
        packet = menu->packet_cursor;
        ordering = ui_packets.scene_ot;
        result = mission_save_draw_rotating_marker(227, 343, ordering, (uint32)(tick % 64) << 6, packet);
        menu->packet_cursor = (uint32)(result);
    }
    mode = mission_flow.account_mode;
    if (mode == 0 || mode == 2 || mode == 3)
    {
        value = menu->packet_cursor;
        ui_packets.scene_packet_cursor = (uint32)(value);
    }
    else if (mode == 1 || mode == 4)
    {
        value = menu->packet_cursor;
        menu_state.packet_cursor = (uint32)(value);
    }
    kind = menu->exit_requested;
    if (kind == 1)
    {
        mission_card_close_events();
        ui_group_bind(ui_group_directory(0), 0);
        mission_flow.mode = 7;
        mission_flow.mode_phase = 0;
        arena_set_cursor(menu->arena_bookmark);
        mode = mission_flow.account_mode;
        if (mode == 0)
        {
            mission = (sint16)game_runtime.selection.mission;
            if (mission == 20 || mission == 35 || mission == 40 || mission == 45)
            {
                game_runtime.account.route_mode = (uint16)(2);
                value = game_runtime.progress.flow_flags;
                game_runtime.progress.flow_flags = (uint32)(value | 1);
                mission_flow.next_scene = 0x60000003;
            }
            else
            {
                value = game_runtime.progress.flow_flags;
                game_runtime.progress.flow_flags = (uint32)(value & 0xFFFFFFFEu);
            }
        }
        result = game_runtime.progress.flow_flags & 0xFFFFFFF7u;
        game_runtime.progress.flow_flags = (uint32)(result);
        return result;
    }
    if (kind == 2)
    {
        mission_card_close_events();
        mission_flow.next_scene = 0x60000004;
        return 0x60000004;
    }
    return 2;
}

uint32 mission_save_offer_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E7498, "ACE3.BIN");
    ui_group_set_mask(1, 3073);
    ui_item_reset_motion(1, 11);
    ui_item_reset_motion(1, 0);
    ui_item_reset_motion(1, 10);
    ui_animation_set(1, 1, 0, 20);
    menu->confirmation = (uint8)(0);
    menu->show_both_ports = (uint8)(1);
    *ui_widget_value(8) = (uint8)(0);
    *ui_widget_value(30) = (uint8)(0);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_offer_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, value, result, accepted;
    FUNCTION_MARKER(0x800E7554, "ACE3.BIN");
    *ui_widget_value(1) = (uint8)(43);
    phase = ui_animation_phase(1);
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            value = menu->confirmation;
            if (value == 1)
            {
                *ui_widget_value(1) = (uint8)(47);
                ui_animation_set(1, 0, 0, 0);
                menu->screen = (uint8)(value);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    result = game_input.pressed & 32;
    accepted = 0;
    if (result)
    {
        result = game_input.held & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            result = 2;
            if (menu->confirmation == 2)
            {
                *ui_widget_value(1) = (uint8)(0);
                ui_animation_set(1, 0, 0, 0);
                value = mission_flow.account_mode;
                value = mission_save_leave_mode(value);
                menu->screen = (uint8)(value);
                result = 0;
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return sound_play(59);
        }
    }
    return result;
}

uint32 mission_save_wait_cards_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E7758, "ACE3.BIN");
    menu->show_both_ports = (uint8)(1);
    ui_group_set_mask(1, 3073);
    if (mission_card_port_status(0) && mission_card_port_status(1))
    {
        menu->screen = (uint8)(2);
        return 2;
    }
    ui_animation_set(5, 1, 0, 20);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

void mission_save_wait_cards_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase;
    FUNCTION_MARKER(0x800E77EC, "ACE3.BIN");
    phase = ui_animation_phase(5);
    if (mission_card_port_status(0) && mission_card_port_status(1))
    {
        if (phase == 0)
        {
            ui_animation_set(5, 0, 0, 0);
            menu->screen = (uint8)(2);
            menu->phase = (uint8)(0);
            return;
        }
        if (phase != 3)
        {
            ui_animation_set(5, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            return;
        }
        return;
    }
    *ui_widget_value(1) = (uint8)(47);
    return;
}

uint32 mission_save_leave_mode(uint32 mode)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E69A8, "ACE3.BIN");
    if (mode == 0)
        return 18;
    if (mode == 1)
        return 17;
    if (mode == 2)
    {
        value = game_runtime.progress.account_flags;
        game_runtime.progress.account_flags = (uint32)(value & 0xFFFFFFFEu);
        mission_flow.next_scene = 0x60000004;
        menu->exit_requested = (uint8)(1);
        return 0;
    }
    if (mode == 3)
    {
        game_runtime.account.route_mode = (uint16)(mode);
        value = game_runtime.progress.flow_flags;
        game_runtime.progress.flow_flags = (uint32)(value | 16);
        menu->exit_requested = (uint8)(1);
        return 18;
    }
    return 0;
}

uint32 mission_save_unavailable_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 first, second, value;
    FUNCTION_MARKER(0x800E78A0, "ACE3.BIN");
    first = mission_card_port_status(0);
    second = mission_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        ui_animation_set(4, 1, 0, 20);
        *ui_widget_value(1) = (uint8)(44);
        return 44;
    }
    menu->screen = (uint8)(3);
    return 3;
}

uint32 mission_save_unavailable_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, first, second, value, result, accepted;
    FUNCTION_MARKER(0x800E7948, "ACE3.BIN");
    phase = ui_animation_phase(4);
    first = mission_card_port_status(0);
    second = mission_card_port_status(1);
    if ((first != 2 && first != 6) || (second != 2 && second != 6))
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 3)
            {
                menu->screen = (uint8)(1);
                result = 0;
                menu->phase = (uint8)(0);
                return result;
            }
        }
        else
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(3);
        }
    }
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(4, 0, 0, 0);
                value = mission_flow.account_mode;
                value = mission_save_leave_mode(value);
                menu->screen = (uint8)(value);
                result = 0;
                menu->phase = (uint8)(0);
                return result;
            }
        }
        else
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
        }
    }
    result = game_input.pressed & 32;
    accepted = 0;
    if (result)
    {
        result = game_input.held & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return 2;
        }
        result = 2;
        if (menu->confirmation == 2)
        {
            ui_animation_set(4, 0, 0, 0);
            menu->screen = (uint8)(0);
            result = 0;
            menu->phase = (uint8)(0);
        }
    }
    return result;
}

void mission_save_select_card_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 first, second, value;
    FUNCTION_MARKER(0x800E7B8C, "ACE3.BIN");
    menu->show_both_ports = (uint8)(0);
    menu->hide_prompt_sprite = (uint8)(0);
    first = mission_card_port_status(0);
    second = mission_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        ui_group_set_mask(1, 3073);
        ui_item_reset_motion(1, 0);
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
        menu->show_both_ports = (uint8)(1);
    }
    else
    {
        ui_group_set_mask(1, 3076);
        ui_item_reset_motion(1, 2);
        ui_animation_set(1, 1, 0, 20);
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        *ui_widget_value(1) = (uint8)(46);
    }
    if (first != 5 && second == 5)
        menu->card_port = (uint8)(1);
    *ui_widget_value(30) = (uint8)(0);
    return;
}

uint32 mission_save_select_card_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, previous, first, second, value, result;
    FUNCTION_MARKER(0x800E7CDC, "ACE3.BIN");
    phase = ui_animation_phase(1);
    previous = menu->card_port;
    first = mission_card_port_status(0);
    second = mission_card_port_status(1);
    if (game_input.repeat & 3)
    {
        value = menu->card_port;
        menu->card_port = (uint8)(value ^ 1);
    }
    if (first == 2 || first == 6)
        menu->card_port = (uint8)(1);
    if (second == 2 || second == 6)
        menu->card_port = (uint8)(0);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        ui_animation_set(1, 0, 0, 0);
        ui_group_set_mask(1, 3073);
        ui_item_reset_motion(1, 0);
        menu->screen = (uint8)(1);
        result = 0;
        menu->phase = (uint8)(0);
        return result;
    }
    if (first == 2 || first == 6)
    {
        *ui_widget_value(2) = (uint8)(2);
    }
    else
    {
        value = menu->card_port;
        *ui_widget_value(2) = (uint8)(value != 0);
    }
    if (second == 2 || second == 6)
    {
        *ui_widget_value(3) = (uint8)(2);
    }
    else
    {
        value = menu->card_port;
        *ui_widget_value(3) = (uint8)(value != 1);
    }
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(1, 0, 0, 0);
                menu->screen = (uint8)(5);
                result = 0;
                menu->phase = (uint8)(0);
                return result;
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    if (((game_input.pressed & 32) && !(game_input.held & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 2)
            {
                ui_animation_set(1, 0, 0, 0);
                menu->screen = (uint8)(0);
                result = 0;
                menu->phase = (uint8)(0);
                return result;
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            sound_play(59);
        }
    }
    result = game_input.repeat & 3;
    if (result)
    {
        result = menu->card_port;
        if (previous != result)
            return sound_play(61);
    }
    return result;
}

uint32 mission_save_selected_card_missing_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E80B8, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status == 2 || status == 6)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        ui_animation_set(4, 1, 0, 20);
        ui_group_set_mask(1, 3073);
        ui_item_reset_motion(1, 0);
        *ui_widget_value(1) = (uint8)(44);
    }
    else
        menu->screen = (uint8)(6);
    mission_card_reset_directory_alias();
    return mission_card_initialize_accounts_alias();
}

uint32 mission_save_selected_card_missing_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, first, second, value, result, accepted;
    FUNCTION_MARKER(0x800E8174, "ACE3.BIN");
    phase = ui_animation_phase(4);
    first = mission_card_port_status(0);
    second = mission_card_port_status(1);
    if (first == 0 || second == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 3)
            {
                menu->screen = (uint8)(5);
                result = 0;
                menu->phase = (uint8)(0);
                return result;
            }
        }
        else
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(3);
        }
    }
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(4, 0, 0, 0);
                value = mission_flow.account_mode;
                value = mission_save_leave_mode(value);
                menu->screen = (uint8)(value);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
        }
    }
    result = game_input.pressed & 32;
    accepted = 0;
    if (result)
    {
        result = game_input.held & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            result = 2;
            if (menu->confirmation == 2)
            {
                ui_animation_set(4, 0, 0, 0);
                menu->screen = (uint8)(3);
                result = 0;
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return 2;
        }
    }
    return result;
}

uint32 mission_save_poll_wait_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E83AC, "ACE3.BIN");
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    ui_animation_set(2, 1, 0, 20);
    *ui_widget_value(1) = (uint8)(47);
    return 47;
}

uint32 mission_save_poll_wait_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result;
    FUNCTION_MARKER(0x800E8404, "ACE3.BIN");
    result = mission_card_port_status(menu->card_port);
    if (result)
    {
        menu->screen = (uint8)(6);
        result = 0;
        menu->phase = (uint8)(0);
    }
    return result;
}

uint32 mission_save_info_notice_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E8454, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status != 1)
    {
        menu->screen = (uint8)(7);
        return 7;
    }
    ui_group_set_mask(1, 3073);
    ui_item_reset_motion(1, 0);
    *ui_widget_value(1) = (uint8)(49);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_info_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, result = 6, value;
    FUNCTION_MARKER(0x800E84E8, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        result = 0;
        menu->phase = (uint8)(0);
    }
    else if (status)
    {
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            value = mission_flow.account_mode;
            value = mission_save_leave_mode(value);
            menu->screen = (uint8)(value);
            menu->phase = (uint8)(0);
        }
        result = game_input.pressed & 32;
        if (result)
        {
            result = game_input.held & 0x5F;
            if (result == 0)
            {
                menu->screen = (uint8)(3);
                result = 0;
                menu->phase = (uint8)(0);
            }
        }
    }
    return result;
}

uint32 mission_save_format_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E8600, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status != 3)
    {
        menu->screen = (uint8)(8);
        return 8;
    }
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    ui_group_set_mask(1, 3074);
    ui_item_reset_motion(1, 1);
    *ui_widget_value(1) = (uint8)(51);
    return 51;
}

void mission_save_format_transition_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, value;
    FUNCTION_MARKER(0x800E868C, "ACE3.BIN");
    phase = ui_animation_phase(1);
    if (phase == 0)
    {
        ui_animation_set(2, 1, 0, 20);
        value = menu->phase + 1;
        menu->phase = (uint8)(value);
        return;
    }
    if (phase != 3)
    {
        ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        return;
    }
    return;
}

uint32 mission_save_format_confirm_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, status, result = 6, value, accepted;
    FUNCTION_MARKER(0x800E8708, "ACE3.BIN");
    phase = ui_animation_phase(2);
    status = mission_card_port_status(menu->card_port);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        result = 0;
        menu->phase = (uint8)(0);
        return result;
    }
    if (status == 0)
        return result;
    if ((game_input.pressed & 64) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            return sound_play(60);
        }
        if (menu->confirmation == 1)
        {
            ui_animation_set(2, 0, 0, 0);
            ui_animation_set(3, 1, 0, 20);
            value = menu->phase;
            menu->phase = (uint8)(value + 1);
            *ui_widget_value(1) = (uint8)(53);
            return 53;
        }
    }
    result = game_input.pressed & 32;
    accepted = 0;
    if (result)
    {
        result = game_input.held & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return sound_play(59);
        }
        result = 2;
        if (menu->confirmation == 2)
        {
            ui_animation_set(2, 0, 0, 0);
            menu->screen = (uint8)(3);
            result = 0;
            menu->phase = (uint8)(0);
        }
    }
    return result;
}

uint32 mission_save_format_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, status, value;
    FUNCTION_MARKER(0x800E8910, "ACE3.BIN");
    result = ui_animation_phase(3);
    if (result != 2)
        return result;
    status = mission_card_format_port(menu->card_port);
    if (status == 0)
        return 0;
    if (status == 1)
    {
        ui_animation_set(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        result = 0;
        menu->hide_prompt_sprite = (uint8)(status);
        return result;
    }
    if (status == 0xFFFFFFFFu)
    {
        ui_animation_set(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        value = menu->phase;
        menu->phase = (uint8)(value + 2);
        ui_group_set_mask(1, 3073);
        *ui_widget_value(1) = (uint8)(80);
        return 80;
    }
    return 0xFFFFFFFFu;
}

uint32 mission_save_format_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, result = 6, value;
    FUNCTION_MARKER(0x800E89F8, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    *ui_widget_value(1) = (uint8)(80);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        result = 0;
        menu->phase = (uint8)(0);
    }
    else if (status)
    {
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            value = mission_flow.account_mode;
            value = mission_save_leave_mode(value);
            menu->screen = (uint8)(value);
            menu->phase = (uint8)(0);
        }
        result = game_input.pressed & 32;
        if (result)
        {
            result = game_input.held & 0x5F;
            if (result == 0)
            {
                menu->screen = (uint8)(0);
                result = 0;
                menu->phase = (uint8)(0);
            }
        }
    }
    return result;
}

uint32 mission_save_create_file_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E94E0, "ACE3.BIN");
    status = mission_card_create_file(menu->card_port);
    if (status == 1)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
    }
    if (status == 0xFFFFFFFFu)
    {
        value = menu->phase + 3;
        menu->phase = (uint8)(value);
        return value;
    }
    return 0xFFFFFFFFu;
}

uint32 mission_save_write_icons_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E955C, "ACE3.BIN");
    status = mission_card_write_icons(menu->card_port, 0);
    if (status == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 2);
    }
    if (status == 1)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        mission_card_reset_directory_alias();
        return 0;
    }
    return 1;
}

uint32 mission_save_initialize_directory_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E95DC, "ACE3.BIN");
    status = mission_card_write_directory(menu->card_port, 0xFFFFFFFFu, 1);
    if (status == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
    }
    if (status == 1)
    {
        menu->screen = (uint8)(11);
        menu->phase = (uint8)(0);
        return 0;
    }
    return 1;
}

uint32 mission_save_load_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result;
    FUNCTION_MARKER(0x800E8B2C, "ACE3.BIN");
    result = mission_card_port_status(menu->card_port);
    if (result == 4)
    {
        menu->screen = (uint8)(9);
        result = 0;
        menu->phase = (uint8)(0);
        return result;
    }
    result = menu->phase + 1;
    menu->phase = (uint8)(result);
    return result;
}

uint32 mission_save_read_directory_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, result;
    FUNCTION_MARKER(0x800E8B98, "ACE3.BIN");
    status = mission_card_read_directory(menu->card_port, 0);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        *ui_widget_value(1) = (uint8)(status == 2 ? 81 : 80);
        menu->phase = (uint8)(6);
    }
    if (status == 1)
    {
        result = menu->phase + 1;
        menu->phase = (uint8)(result);
        return result;
    }
    return 1;
}

uint32 mission_save_read_accounts_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, result;
    FUNCTION_MARKER(0x800E8C50, "ACE3.BIN");
    status = mission_card_read_accounts(menu->card_port, 1);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        *ui_widget_value(1) = (uint8)(status == 2 ? 81 : 80);
        menu->phase = (uint8)(6);
    }
    if (status == 1)
    {
        result = menu->phase + 1;
        menu->phase = (uint8)(result);
        return result;
    }
    return 1;
}

uint32 mission_save_find_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value, result;
    FUNCTION_MARKER(0x800E8D08, "ACE3.BIN");
    value = mission_card_find_current_account();
    if (value == 0xFFFFFFFFu)
    {
        menu->screen = (uint8)(9);
        result = 0;
        menu->phase = (uint8)(0);
        return result;
    }
    menu->account_slot = (uint8)(value);
    result = menu->phase + 1;
    menu->phase = (uint8)(result);
    return result;
}

uint32 mission_save_load_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 mode, result;
    FUNCTION_MARKER(0x800E8D78, "ACE3.BIN");
    mission_card_select_account_copy(menu->account_slot);
    menu->hide_data = (uint8)(0);
    mode = mission_flow.account_mode;
    if (mode - 2 < 3)
    {
        mission_card_encode_current_account();
        menu->screen = (uint8)(12);
        menu->phase = (uint8)(6);
        *ui_widget_value(1) = (uint8)(59);
        return 59;
    }
    result = menu->phase + 1;
    menu->phase = (uint8)(result);
    return result;
}

void mission_save_read_missions_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 port, slot, status;
    FUNCTION_MARKER(0x800E8E28, "ACE3.BIN");
    port = menu->card_port;
    slot = menu->account_slot;
    status = mission_card_read_missions(port, slot, 1);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        menu->phase = (uint8)(6);
        *ui_widget_value(1) = (uint8)(status == 2 ? 81 : 80);
    }
    if (status == 1)
    {
        menu->screen = (uint8)(13);
        menu->phase = (uint8)(0);
        {
            ui_animation_set(2, 0, 0, 0);
            return;
        }
    }
    return;
}

uint32 mission_save_read_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value, result;
    FUNCTION_MARKER(0x800E8EF0, "ACE3.BIN");
    ui_group_set_mask(1, 3073);
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        value = mission_flow.account_mode;
        value = mission_save_leave_mode(value);
        menu->screen = (uint8)(value);
        menu->phase = (uint8)(0);
        sound_play(60);
    }
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (result == 0)
        {
            menu->screen = (uint8)(0);
            return sound_play(59);
        }
    }
    return result;
}

uint32 mission_save_create_offer_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E8FC4, "ACE3.BIN");
    ui_group_set_mask(1, 3074);
    ui_item_reset_motion(1, 1);
    ui_animation_set(2, 0, 0, 0);
    ui_animation_set(3, 1, 0, 20);
    *ui_widget_value(1) = (uint8)(55);
    menu->hide_prompt_sprite = (uint8)(1);
    menu->confirmation = (uint8)(0);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_create_offer_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, phase;
    FUNCTION_MARKER(0x800E9068, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    phase = ui_animation_phase(3);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
        return 0;
    }
    if (status == 0)
        return 0;
    if ((game_input.pressed & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(3, 0, 0, 0);
                menu->screen = (uint8)(10);
                menu->phase = (uint8)(0);
                *ui_widget_value(1) = (uint8)(47);
            }
        }
        else
        {
            ui_animation_set(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    if (((game_input.pressed & 32) && !(game_input.held & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 2)
            {
                ui_animation_set(3, 0, 0, 0);
                menu->screen = (uint8)(3);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            sound_play(59);
        }
    }
    return 0;
}

uint32 mission_save_check_file_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E927C, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status == 5 || status == 2 || status == 6)
    {
        menu->screen = (uint8)(status == 5 ? 11 : 1);
        menu->phase = (uint8)(0);
        return 0;
    }
    if (status)
    {
        value = menu->phase + 1;
        menu->phase = (uint8)(value);
        return value;
    }
    return 6;
}

uint32 mission_save_check_space_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800E9310, "ACE3.BIN");
    result = mission_card_free_blocks(menu->card_port);
    if (result == 0xFFFFFFFFu)
        return result;
    if ((sint32)result < 2)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        *ui_widget_value(1) = (uint8)(56);
        ui_group_set_mask(1, 3073);
        return mission_card_begin_polling();
    }
    value = menu->phase + 2;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_space_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E93B8, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
    }
    if (status)
    {
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            value = mission_flow.account_mode;
            value = mission_save_leave_mode(value);
            menu->screen = (uint8)(value);
            menu->phase = (uint8)(0);
        }
        if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
        {
            menu->screen = (uint8)(1);
            menu->phase = (uint8)(0);
        }
    }
    return 0;
}

uint32 mission_save_file_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E965C, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    menu->hide_prompt_sprite = (uint8)(0);
    *ui_widget_value(1) = (uint8)(80);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
    }
    if (status)
    {
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            value = mission_flow.account_mode;
            value = mission_save_leave_mode(value);
            menu->screen = (uint8)(value);
            menu->phase = (uint8)(0);
            sound_play(60);
        }
        if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
        {
            menu->screen = (uint8)(0);
            sound_play(59);
        }
    }
    return 1;
}

uint32 mission_save_select_account_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800E97A4, "ACE3.BIN");
    ui_group_set_mask(1, 3088);
    ui_item_reset_motion(1, 4);
    ui_animation_set(1, 1, 0, 60);
    *ui_widget_value(1) = (uint8)(57);
    menu->hide_prompt_sprite = (uint8)(1);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_select_account_frame(void)
{
    FUNCTION_MARKER(0x800E9824, "ACE3.BIN");
    return mission_save_select_account_tick();
}

uint32 mission_save_select_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, index, value, input;
    FUNCTION_MARKER(0x800E982C, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    *ui_widget_value(1) = (uint8)(status ? 57 : 47);
    menu->hide_data = (uint8)(0);
    for (index = 0; index < 3; ++index)
    {
        value = menu->account_slot;
        if (value == index)
            value = (game_input.kind & 16) ? 3 : 0;
        else
            value = 1;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
    }
    mission_save_draw_account_slots(menu->hide_data, 0);
    if (!status)
        return 0;
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        menu->screen = (uint8)(12);
        menu->phase = (uint8)(0);
        sound_play(60);
        return 0;
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        menu->screen = (uint8)(0);
        menu->phase = (uint8)(0);
        sound_play(59);
        return 0;
    }
    input = game_input.repeat;
    if (input & 2)
    {
        value = menu->account_slot;
        menu->account_slot = (uint8)(value < 2 ? value + 1 : 0);
    }
    else if (input & 1)
    {
        value = menu->account_slot;
        menu->account_slot = (uint8)(value ? value - 1 : 2);
    }
    else
        return 0;
    sound_play(61);
    return 0;
}

uint32 mission_save_overwrite_account_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value, index;
    FUNCTION_MARKER(0x800E9AB4, "ACE3.BIN");
    value = mission_card_account_present(menu->account_slot);
    if (value)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        menu->account_detail_mode = (uint16)(1);
        for (index = 0; index < 3; ++index)
        {
            value = menu->account_slot;
            if (value == index)
                value = (game_input.kind & 16) ? 3 : 0;
            else
                value = 1;
            *ui_widget_value(index + 2) = (uint8)(value);
        }
        ui_group_set_mask(1, 3104);
        ui_item_reset_motion(1, 5);
        ui_animation_set(1, 1, 0, 60);
        *ui_widget_value(1) = (uint8)(58);
    }
    else
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 2);
        value = menu->account_slot;
        menu->account_detail_mode = (uint16)(0);
        mission_card_prepare_account(value);
    }
    return mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
}

uint32 mission_save_draw_account_slots(uint32 hide_selected, uint32 layout)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    const char *name;
    uint32 present, value, index, color, result = 0;
    FUNCTION_MARKER(0x800E6A6C, "ACE3.BIN");
    present = mission_card_account_mask();
    if (hide_selected == 1)
        present &= ~(1u << (menu->account_slot & 31));
    text_select_font(4);
    text_set_blend(2);
    text_set_reveal_range(1, 10, 20);
    value = ui_animation_ticks(15);
    text_set_reveal_step(value);
    for (index = 0; index < 3; ++index)
    {
        if (layout == 0 || layout == 1)
            text_set_pos(layout ? 56 : 81, 214 + index * 53);
        color = (present & (1u << index)) ? 128 : 64;
        text_set_color(color, color, color);
        name = (present & (1u << index)) ? mission_card_account_name(index + 1) : "empty";
        result = text_print((const uint8 *)name, NULL, 0);
    }
    return result;
}

uint32 mission_save_overwrite_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, index, value;
    FUNCTION_MARKER(0x800E9C40, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    mission_save_draw_account_slots(menu->hide_data, 1);
    for (index = 0; index < 3; ++index)
    {
        value = menu->account_slot;
        if (value == index)
            value = (game_input.kind & 16) ? 3 : 0;
        else
            value = 1;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(3);
        menu->phase = (uint8)(0);
        menu->account_detail_mode = (uint16)(0);
    }
    if (!status)
        return 0;
    if (game_input.pressed & 64)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        sound_play(60);
        mission_card_prepare_account(menu->account_slot);
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        menu->screen = (uint8)(11);
        menu->phase = (uint8)(0);
        menu->account_detail_mode = (uint16)(0);
        sound_play(59);
    }
    return 0;
}

uint32 mission_save_account_file_check_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value;
    FUNCTION_MARKER(0x800E9E20, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
    value = menu->phase;
    if (status == 4)
    {
        menu->phase = (uint8)(value + 1);
        value = 59;
    }
    else
        value += 3;
    {
        if (status == 4)
            *ui_widget_value(1) = (uint8)(value);
        else
            menu->phase = (uint8)(value);
    };
    return value;
}

uint32 mission_save_create_account_file_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800E9EC4, "ACE3.BIN");
    mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
    result = mission_card_create_file(menu->card_port);
    if (result == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 4);
    }
    if (result != 1)
        return 1;
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_write_account_icons_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800E9F64, "ACE3.BIN");
    mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
    result = mission_card_write_icons(menu->card_port, 0);
    if (!result)
        return 0;
    if (result == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 3);
    }
    if (result != 1)
        return 1;
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    mission_card_reset_directory_alias();
    return 0;
}

uint32 mission_save_write_account_directory_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800EA004, "ACE3.BIN");
    mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
    result = mission_card_write_directory(menu->card_port, menu->account_slot, 0);
    if (result == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 2);
    }
    if (result != 1)
        return 1;
    mission_card_encode_current_account();
    mission_card_update_current_name();
    mission_card_prepare_account(menu->account_slot);
    mission_card_prepare_account(0xFFFFFFFFu);
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_write_account_frame(void)
{
    FUNCTION_MARKER(0x800EA0C8, "ACE3.BIN");
    return mission_save_write_account_tick();
}

uint32 mission_save_write_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, value;
    FUNCTION_MARKER(0x800EA0D0, "ACE3.BIN");
    if (mission_flow.account_mode < 2)
    {
        mission_save_draw_account_slots(menu->hide_data, (uint32)((sint16)menu->account_detail_mode));
    }
    result = mission_card_write_account(menu->card_port, menu->account_slot, 0xFFFFFFFFu, 1);
    if (result == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
    }
    if (result != 1)
        return 1;
    value = mission_flow.account_mode - 2;
    menu->screen = (uint8)(value < 3 ? 16 : 13);
    menu->phase = (uint8)(0);
    return 0;
}

uint32 mission_save_account_write_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status;
    FUNCTION_MARKER(0x800EA1C4, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    menu->hide_prompt_sprite = (uint8)(1);
    ui_animation_set(1, 2, 20, 20);
    ui_group_set_mask(1, 3200);
    ui_item_reset_motion(1, 7);
    *ui_widget_value(1) = (uint8)(61);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
    }
    if (status && game_input.pressed)
    {
        menu->hide_prompt_sprite = (uint8)(0);
        ui_animation_set(1, 0, 0, 0);
        menu->screen = (uint8)(0);
        menu->phase = (uint8)(0);
        sound_play(59);
    }
    return 1;
}

uint32 mission_save_select_mission_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 index, value;
    FUNCTION_MARKER(0x800EA2E8, "ACE3.BIN");
    ui_group_set_mask(1, 3328);
    ui_item_reset_motion(1, 8);
    ui_item_reset_motion(1, 11);
    ui_animation_set(1, 1, 0, 20);
    ui_animation_set(15, 1, 0, 20);
    *ui_widget_value(1) = (uint8)(62);
    menu->hide_prompt_sprite = (uint8)(1);
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    value = menu->phase + 1;
    menu->phase = (uint8)(value);
    return value;
}

uint32 mission_save_select_mission_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, status, index, value;
    FUNCTION_MARKER(0x800EA3F4, "ACE3.BIN");
    phase = ui_animation_phase(1);
    status = mission_card_port_status(menu->card_port);
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    *ui_widget_value(30) = (uint8)(0);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(3);
        menu->phase = (uint8)(0);
    }
    if (!status)
        return 0;
    if (phase == 2)
    {
        if (game_input.repeat & 8)
        {
            value = menu->mission_slot;
            menu->mission_slot = (uint8)(value < 5 ? value + 1 : 0);
            sound_play(61);
        }
        if (game_input.repeat & 4)
        {
            value = menu->mission_slot;
            menu->mission_slot = (uint8)(value ? value - 1 : 5);
            sound_play(61);
        }
    }
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(1, 0, 0, 0);
                menu->screen = (uint8)(14);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    if (((game_input.pressed & 32) && !(game_input.held & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 2)
            {
                ui_animation_set(1, 0, 0, 0);
                menu->screen = (uint8)(0);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            sound_play(59);
        }
    }
    return 0;
}

uint32 mission_save_overwrite_mission_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 mode, result = 0, value, index;
    FUNCTION_MARKER(0x800EA744, "ACE3.BIN");
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    menu->confirmation = (uint8)(0);
    mode = menu->hide_data;
    if (mode == 0)
        result = mission_card_mission_present(menu->account_slot, menu->mission_slot);
    else if (mode == 1)
        result = mission_card_mission_present(0xFFFFFFFFu, menu->mission_slot);
    if (result == 0 || result == 1)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + (result == 1 ? 2 : 1));
        ui_animation_set(2, 1, 0, 20);
    }
    ui_animation_set(4, 1, 0, 20);
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    return 0;
}

uint32 mission_save_overwrite_mission_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, phase, index, value;
    FUNCTION_MARKER(0x800EA8AC, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    phase = ui_animation_phase(2);
    *ui_widget_value(1) = (uint8)(63);
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot;
        if (value == index)
            value = (game_input.kind & 16) ? 3 : 0;
        else
            value = 1;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    value = menu->mission_slot;
    *ui_widget_value(9) = (uint8)(value + 1);
    value = menu->mission_slot;
    *ui_widget_value(30) = (uint8)(value + 1);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(3);
        menu->phase = (uint8)(0);
    }
    if (!status)
        return 0;
    if ((game_input.pressed & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(2, 0, 0, 0);
                ui_animation_set(4, 0, 0, 0);
                menu->screen = (uint8)(15);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    if (((game_input.pressed & 32) && !(game_input.held & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 2)
            {
                ui_animation_set(2, 0, 0, 0);
                ui_animation_set(4, 0, 0, 0);
                menu->screen = (uint8)(13);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            sound_play(59);
        }
    }
    return 0;
}

uint32 mission_save_confirm_mission_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, status, value, index, result, pressed;
    FUNCTION_MARKER(0x800EAC04, "ACE3.BIN");
    phase = ui_animation_phase(2);
    status = mission_card_port_status(menu->card_port);
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    *ui_widget_value(1) = (uint8)(64);
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot;
        if (value == index)
            value = (game_input.kind & 16) ? 3 : 0;
        else
            value = 1;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    value = menu->mission_slot;
    *ui_widget_value(9) = (uint8)(value + 1);
    value = menu->mission_slot;
    *ui_widget_value(30) = (uint8)(value + 1);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(3);
        menu->phase = (uint8)(0);
        return 0;
    }
    if (!status)
        return 6;
    if ((game_input.pressed & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(2, 0, 0, 0);
                ui_animation_set(4, 0, 0, 0);
                menu->screen = (uint8)(15);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    pressed = game_input.pressed & 32;
    result = pressed;
    if (result)
        result = game_input.held & 0x5F;
    if ((pressed && result == 0) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            ui_animation_set(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return sound_play(59);
        }
        if (menu->confirmation != 2)
            return 2;
        ui_animation_set(2, 0, 0, 0);
        ui_animation_set(4, 0, 0, 0);
        menu->screen = (uint8)(13);
        menu->phase = (uint8)(0);
        return 0;
    }
    return result;
}

uint32 mission_save_commit_mission_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 present, value, index;
    FUNCTION_MARKER(0x800EAF44, "ACE3.BIN");
    present = mission_card_current_record_mask();
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    *ui_widget_value(1) = (uint8)(65);
    *ui_widget_value(30) = (uint8)(0);
    mission_card_encode_current_account();
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    value = menu->mission_slot;
    menu->hide_data = (uint8)((((sint32)present >> (value & 31)) & 1) == 0);
    return 1;
}

uint32 mission_save_write_mission_account_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value, index;
    FUNCTION_MARKER(0x800EB064, "ACE3.BIN");
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    status = mission_card_write_account(menu->card_port, menu->account_slot, menu->mission_slot, 1);
    if (status == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 2);
    }
    if (status == 1)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
        mission_card_encode_current_mission();
    }
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    return 0;
}

uint32 mission_save_write_mission_record_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status, value, index;
    FUNCTION_MARKER(0x800EB174, "ACE3.BIN");
    mission_save_draw_mission_preview(menu->mission_slot, menu->account_slot, menu->hide_data);
    status = mission_card_write_mission(menu->card_port, menu->account_slot, menu->mission_slot, 1);
    if (status == 0xFFFFFFFFu)
    {
        value = menu->phase;
        menu->phase = (uint8)(value + 1);
    }
    if (status == 1)
    {
        menu->screen = (uint8)(16);
        menu->phase = (uint8)(0);
    }
    for (index = 0; index < 6; ++index)
    {
        value = menu->mission_slot != index;
        *ui_widget_value(index + 2) = (uint8)(value);
    }
    return 0;
}

uint32 mission_save_mission_write_notice_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 status;
    FUNCTION_MARKER(0x800EB280, "ACE3.BIN");
    status = mission_card_port_status(menu->card_port);
    ui_animation_set(1, 2, 20, 20);
    ui_group_set_mask(1, 3200);
    ui_item_reset_motion(1, 7);
    if (status == 2 || status == 6)
    {
        menu->screen = (uint8)(1);
        menu->phase = (uint8)(0);
    }
    if (status)
    {
        *ui_widget_value(1) = (uint8)(67);
        if (game_input.pressed)
        {
            ui_animation_set(1, 0, 0, 0);
            menu->screen = (uint8)(0);
            menu->phase = (uint8)(0);
            sound_play(59);
        }
    }
    return 1;
}

uint32 mission_save_success_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800EB388, "ACE3.BIN");
    ui_group_set_mask(1, 3584);
    ui_item_reset_motion(1, 9);
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    menu->ticks = (uint8)(0);
    menu->hide_prompt_sprite = (uint8)(0);
    *ui_widget_value(1) = (uint8)(68);
    *ui_widget_value(30) = (uint8)(0);
    value = menu->mission_slot;
    game_runtime.progress.record_slot = (uint8)(value);
    return value;
}

uint32 mission_save_success_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    const char *text;
    uint32 value;
    FUNCTION_MARKER(0x800EB42C, "ACE3.BIN");
    text_set_pos(74, 214);
    text = mission_card_account_name(0);
    text_print((const uint8 *)text, NULL, 0);
    if (menu->ticks >= 121 || game_input.pressed)
    {
        value = mission_save_leave_mode(mission_flow.account_mode);
        menu->screen = (uint8)(value);
        menu->phase = (uint8)(0);
        menu->ticks = (uint8)(0);
    }
    value = menu->ticks + 1;
    menu->ticks = (uint8)(value);
    return value;
}

uint32 mission_save_leave_offer_begin(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 value;
    FUNCTION_MARKER(0x800EB4F4, "ACE3.BIN");
    menu->show_both_ports = (uint8)(1);
    ui_group_set_mask(1, 3073);
    ui_item_reset_motion(1, 0);
    ui_animation_set(2, 1, 0, 20);
    value = menu->phase;
    menu->phase = (uint8)(value + 1);
    *ui_widget_value(1) = (uint8)(69);
    return 69;
}

uint32 mission_save_leave_offer_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 phase, result, pressed;
    FUNCTION_MARKER(0x800EB57C, "ACE3.BIN");
    phase = ui_animation_phase(2);
    if (((game_input.pressed & 16) && !(game_input.held & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (menu->confirmation == 1)
            {
                ui_animation_set(2, 0, 0, 0);
                menu->screen = (uint8)(18);
                menu->phase = (uint8)(0);
            }
        }
        else
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(1);
            sound_play(60);
        }
    }
    pressed = game_input.pressed & 32;
    result = pressed;
    if (pressed)
        result = game_input.held & 0x5F;
    if ((pressed && result == 0) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            ui_animation_set(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            menu->confirmation = (uint8)(2);
            return sound_play(59);
        }
        if (menu->confirmation != 2)
            return 2;
        ui_animation_set(2, 0, 0, 0);
        menu->exit_requested = (uint8)(2);
        result = game_runtime.progress.flow_flags | 2;
        game_runtime.progress.flow_flags = (uint32)(result);
    }
    return result;
}

uint32 mission_save_check_disc_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result, media, kind, value;
    FUNCTION_MARKER(0x800EB73C, "ACE3.BIN");
    game_runtime.progress.sequence_idx = (uint8)(0);
    result = mission_card_stop_polling();
    if (!result)
        return result;
    media = cd_resource_state();
    if (!media)
    {
        *ui_widget_value(1) = (uint8)(73);
        return 73;
    }
    kind = game_data_mission_media_kind((uint32)(sint16)game_runtime.selection.mission);
    if (kind == 3 || media == 3)
    {
        menu->exit_requested = (uint8)(1);
        return 1;
    }
    if (media == 4 || media == 0xFFFFFFFFu)
    {
        if (kind == 1)
        {
            value = ((uint8)menu->recovery_indicator);
            *ui_widget_value(1) = (uint8)(value + 70);
        }
        kind = game_data_mission_media_kind((uint32)(sint16)game_runtime.selection.mission);
        if (kind == 2)
        {
            value = ((uint8)menu->recovery_indicator);
            *ui_widget_value(1) = (uint8)(value + 71);
        }
        resource_begin_recovery();
        value = menu->phase;
        menu->recovery_indicator = (uint16)(31);
        result = value + 1;
        menu->phase = (uint8)(result);
        return result;
    }
    if (media == 1)
    {
        if (kind == 1)
        {
            menu->exit_requested = (uint8)(1);
            return 0;
        }
        value = ((uint8)menu->recovery_indicator);
        *ui_widget_value(1) = (uint8)(value + 71);
        resource_begin_recovery();
        value = menu->phase;
        menu->recovery_indicator = (uint16)(31);
        menu->phase = (uint8)(value + 1);
        return 2;
    }
    if (media == 2)
    {
        kind = game_data_mission_media_kind((uint32)(sint16)game_runtime.selection.mission);
        if (kind == 2)
        {
            menu->exit_requested = (uint8)(1);
            return 1;
        }
        value = ((uint8)menu->recovery_indicator);
        *ui_widget_value(1) = (uint8)(value + 70);
        resource_begin_recovery();
        value = menu->phase;
        menu->recovery_indicator = (uint16)(31);
        result = value + 1;
        menu->phase = (uint8)(result);
        return result;
    }
    return 2;
}

uint32 mission_save_recovery_wait_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result;
    FUNCTION_MARKER(0x800EB944, "ACE3.BIN");
    result = resource_get_recovery_state();
    if (result == 3)
    {
        result = menu->phase + 1;
        menu->phase = (uint8)(result);
    }
    return result;
}

uint32 mission_save_recovery_finish_tick(void)
{
    MISSION_SAVE_MENU *menu = &mission_save_menu;
    uint32 result;
    FUNCTION_MARKER(0x800EB98C, "ACE3.BIN");
    result = resource_get_recovery_state();
    if (result == 1)
    {
        *ui_widget_value(1) = (uint8)(73);
        result = menu->phase - 2;
        menu->phase = (uint8)(result);
    }
    return result;
}

static void mission_save_format_record_time(uint32 slot, uint8 text[MISSION_TIME_TEXT_SIZE])
{
    uint32 wire_word_0;
    const uint8 *mission;
    sint32 duration;
    FUNCTION_MARKER(0x800E43B4, "ACE3.BIN");
    mission = mission_card_mission_record(slot & 255);
    duration = (memcpy(&wire_word_0, mission + 276, sizeof(wire_word_0)), wire_word_0);
    format_mission_time(text, duration);
}

void mission_save_draw_mission_preview(uint32 slot, uint32 account, uint32 hide_data)
{
    uint32 present, value;
    const char *text;
    uint8 mission_time[MISSION_TIME_TEXT_SIZE];
    FUNCTION_MARKER(0x800E6C4C, "ACE3.BIN");
    present = mission_card_current_record_mask();
    text_select_font(4);
    text_set_blend(2);
    text_set_reveal_range(1, 10, 20);
    value = ui_animation_ticks(15);
    text_set_reveal_step(value);
    text_set_color(128, 128, 128);
    text_select_font(3);
    slot &= 255;
    if (hide_data == 1 || !(present & (1u << (slot & 31))))
    {
        text_set_pos(140, 331);
        {
            (void)(uint32)(text_print((const uint8 *)"no data", NULL, 0));
            return;
        }
    }
    text_set_pos(94, 294);
    text = mission_card_mission_label(slot);
    text_print((const uint8 *)text, NULL, 0);
    text_set_pos(94, 317);
    text_print((const uint8 *)"level", NULL, 0);
    text_set_pos(140, 317);
    text = mission_card_difficulty_label(slot);
    text_print((const uint8 *)text, NULL, 0);
    text_set_pos(94, 334);
    text_print((const uint8 *)"time", NULL, 0);
    text_set_pos(140, 334);
    mission_save_format_record_time(slot, mission_time);
    text_print(mission_time, NULL, 0);
    text_set_pos(94, 352);
    text_print((const uint8 *)"unit", NULL, 0);
    text_set_pos(140, 353);
    text = mission_card_unit_label(slot);
    text_print((const uint8 *)text, NULL, 0);
    text_set_pos(94, 369);
    text_print((const uint8 *)"control", NULL, 0);
    text_set_pos(140, 369);
    text = mission_card_control_label(account & 255);
    {
        (void)(uint32)(text_print((const uint8 *)text, NULL, 0));
        return;
    }
}

void mission_detail_notify_noop(void)
{
    FUNCTION_MARKER(0x800F6CB4, "ACE3.BIN");
}

uint32 mission_select_begin(void)
{
    uint32 value, index;
    FUNCTION_MARKER(0x800FE444, "ACE3.BIN");
    /* Keep the arena reservation until following allocations become native */
    arena_alloc(68);
    mission_selector.phase = 0;
    mission_selector.result = 0;
    mission_selector.col = 0;
    mission_selector.action = 0;
    mission_selector.row = 0;
    mission_selector.count = 0;
    for (index = 0; index < MISSION_SELECTOR_CAPACITY; ++index)
    {
        if (game_runtime.progress.mission_rating[0][index] || game_runtime.progress.mission_rating[1][index] || game_runtime.progress.mission_rating[2][index] || game_runtime.progress.mission_rating[3][index])
        {
            value = (sint16)mission_selector.count;
            mission_selector.entries[value] = index;
            value = mission_selector.count;
            mission_selector.count = value + 1;
        }
    }
    ui_group_set_mask(0, 0x04000003);
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return value;
}

uint32 mission_select_advance_phase(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800FE564, "ACE3.BIN");
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return value;
}

uint32 mission_select_tick(void)
{
    uint32 value, column, row;
    FUNCTION_MARKER(0x800FE584, "ACE3.BIN");
    if (mission_selector.count)
        mission_dispatch_select(mission_selector.phase);
    else
        mission_selector.result = 2;
    value = mission_selector.result;
    if (value == 1)
    {
        mission_flow.mode = 2;
        mission_flow.mode_phase = 0;
        row = mission_selector.row;
        column = mission_selector.col;
        value = mission_selector.entries[row * 4 + column];
        game_runtime.selection.mission = (uint16)(value);
        return arena_set_cursor(mission_flow.assets.arena_begin);
    }
    if (value == 2)
    {
        mission_flow.root_kind = 0;
        mission_flow.root_phase = 0;
        return arena_set_cursor(mission_flow.assets.arena_begin);
    }
    return 2;
}

void mission_select_grid_begin(void)
{
    FUNCTION_MARKER(0x800FD958, "ACE3.BIN");
    mission_selector.phase = 1;
    mission_select_load_row(mission_selector.row);
    mission_select_load_detail(mission_selector.col);
    *ui_widget_value(47) = (uint8)(1);
    return;
}

uint32 mission_select_grid_frame(void)
{
    FUNCTION_MARKER(0x800FD9CC, "ACE3.BIN");
    return mission_select_grid_tick(mission_flow.menu_arrow_delay);
}

void mission_select_grid_finish(void)
{
    uint32 index, row, column;
    FUNCTION_MARKER(0x800FDE78, "ACE3.BIN");
    if ((sint16)mission_selector.action == 1)
    {
        mission_selector.phase = 3;
        ui_group_set_mask(0, 0x08000003);
        row = mission_selector.row;
        column = mission_selector.col;
        mission_select_load_preview(row, column);
    }
    if ((sint16)mission_selector.action == 2)
        mission_selector.result = 2;
    for (index = 49; index < 57; ++index)
        *ui_widget_value(index) = (uint8)(0);
    *ui_widget_value(47) = (uint8)(0);
    return;
}

uint32 mission_select_difficulty_begin(void)
{
    FUNCTION_MARKER(0x800FDF38, "ACE3.BIN");
    mission_selector.phase = 4;
    *ui_widget_value(47) = (uint8)(1);
    return 1;
}

uint32 mission_select_difficulty_frame(void)
{
    FUNCTION_MARKER(0x800FDF64, "ACE3.BIN");
    return mission_select_difficulty_tick();
}

uint32 mission_select_difficulty_tick(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800FDF6C, "ACE3.BIN");
    *ui_widget_value(51) = (uint8)(0);
    if (game_input.repeat & 2)
    {
        value = game_runtime.selection.difficulty;
        if ((sint32)value < 2)
        {
            game_runtime.selection.difficulty = (uint32)(value + 1);
            sound_play(61);
        }
    }
    if (game_input.repeat & 1)
    {
        value = game_runtime.selection.difficulty;
        if ((sint32)value > 0)
        {
            game_runtime.selection.difficulty = (uint32)(value - 1);
            sound_play(61);
        }
    }
    if ((game_input.repeat & 16) && !(game_input.held & 32))
    {
        mission_selector.action = 1;
        mission_selector.phase = 5;
        sound_play(60);
    }
    if ((game_input.repeat & 32) && !(game_input.held & 16))
    {
        mission_selector.action = 2;
        mission_selector.phase = 5;
        sound_play(59);
    }
    value = (uint8)game_runtime.selection.difficulty;
    *ui_widget_value(52) = (uint8)(value);
    return value;
}

void mission_select_difficulty_finish(void)
{
    uint32 value;
    sint32 phase;
    FUNCTION_MARKER(0x800FE0D4, "ACE3.BIN");
    phase = (sint16)mission_selector.action;
    if (phase == 1)
    {
        value = mission_selector.phase + 1;
        mission_selector.phase = value;
    }
    else if (phase == 2)
    {
        mission_select_load_row(mission_selector.row);
        mission_select_load_detail(mission_selector.col);
        ui_group_set_mask(0, 0x04000003);
        mission_selector.phase = 0;
    }
    *ui_widget_value(47) = (uint8)(0);
    return;
}

uint32 mission_select_load_begin(void)
{
    uint32 row, column, value;
    FUNCTION_MARKER(0x800FE180, "ACE3.BIN");
    mission_resource_load.phase = 0;
    row = mission_selector.row;
    column = mission_selector.col;
    value = mission_selector.entries[row * 4 + column];
    mission_load_set_mission(value);
    value = mission_selector.phase + 1;
    mission_selector.phase = value;
    return value;
}

uint32 mission_select_grid_tick(uint32 cooldown)
{
    uint32 row, column, value, flags;
    sint32 count, last_page;
    FUNCTION_MARKER(0x800FD9D4, "ACE3.BIN");
    if (cooldown)
        mission_flow.menu_arrow_delay = cooldown - 1;
    else
    {
        count = (sint16)mission_selector.count;
        row = mission_selector.row;
        last_page = (count - 1) / 4;
        flags = (sint32)row < last_page ? 2 : 0;
        if (row)
            flags |= 1;
        ui_widget_set(49, flags);
        *ui_widget_value(51) = (uint8)(0);
    }
    if (game_input.repeat & 2)
    {
        if (mission_selector.col < 3)
        {
            row = mission_selector.row;
            column = mission_selector.col;
            count = (sint16)mission_selector.count;
            if ((sint32)(column + row * 4) < count - 1)
            {
                mission_selector.col = column + 1;
                sound_play(61);
            }
        }
        else
        {
            count = (sint16)mission_selector.count;
            row = mission_selector.row;
            if ((sint32)row < (count - 1) / 4)
            {
                mission_selector.col = 0;
                value = mission_selector.row;
                mission_selector.row = value + 1;
                mission_select_load_row(mission_selector.row);
                *ui_widget_value(51) = (uint8)(2);
                mission_flow.menu_arrow_delay = 12;
                sound_play(61);
            }
        }
        mission_select_load_detail(mission_selector.col);
    }
    if (game_input.repeat & 1)
    {
        value = mission_selector.col;
        if (value)
        {
            mission_selector.col = value - 1;
            sound_play(61);
        }
        else if (mission_selector.row)
        {
            mission_selector.col = 3;
            value = mission_selector.row;
            mission_selector.row = value - 1;
            mission_select_load_row(mission_selector.row);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
            sound_play(61);
        }
        mission_select_load_detail(mission_selector.col);
    }
    if (game_input.repeat & 0x200)
    {
        count = (sint16)mission_selector.count;
        row = mission_selector.row;
        if ((sint32)row < (count - 1) / 4)
        {
            mission_selector.row = row + 1;
            row = mission_selector.row;
            column = mission_selector.col;
            count = (sint16)mission_selector.count;
            if ((sint32)(column + row * 4) >= count - 1)
            {
                value = (count - 1 - ((count - 1) / 4) * 4);
                mission_selector.col = value;
            }
            sound_play(61);
            *ui_widget_value(51) = (uint8)(2);
            mission_flow.menu_arrow_delay = 12;
        }
        mission_select_load_row(mission_selector.row);
        mission_select_load_detail(mission_selector.col);
    }
    if (game_input.repeat & 0x100)
    {
        value = mission_selector.row;
        if (value)
        {
            mission_selector.row = value - 1;
            sound_play(61);
            *ui_widget_value(51) = (uint8)(1);
            mission_flow.menu_arrow_delay = 12;
        }
        mission_select_load_row(mission_selector.row);
        mission_select_load_detail(mission_selector.col);
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        mission_selector.action = 1;
        mission_selector.phase = 2;
        sound_play(60);
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        mission_selector.action = 2;
        mission_selector.phase = 2;
        sound_play(59);
    }
    value = mission_selector.col;
    *ui_widget_value(52) = (uint8)(value);
    return value;
}

uint32 aircraft_selection_finish(void)
{
    uint32 value, mission, aircraft, variant, sound, result;
    FUNCTION_MARKER(0x800FA4E4, "ACE3.BIN");
    ui_widget_set(1, 0);
    ui_widget_set(4, (sint16)mission_aircraft.action == 1);
    if (ui_animation_phase(6))
    {
        value = ui_animation_ticks(6);
        scene_palette_blend(25 - value);
    }
    else
    {
        if ((sint16)mission_aircraft.action == 1)
        {
            ui_animation_set(8, 1, 0, 25);
            ui_item_reset_motion(0, 11);
            ui_group_set_mask(0, 0x7A01);
            sound_play(90);
            mission_aircraft.phase = 3;
            value = mission_aircraft.flags;
            mission_aircraft.flags = value & 0xFFFFFFFD;
            mission = (sint16)game_runtime.selection.mission;
            variant = mission_aircraft.variant;
            value = mission_aircraft_id(mission, variant);
            aircraft = (sint16)value;
            mission = (sint16)game_runtime.selection.mission;
            game_runtime.selection.aircraft = (uint16)(value);
            value = aircraft_default_weapon_slot(mission, aircraft, 0);
            mission_aircraft.pending_weapon_idx[0] = value;
            mission_aircraft.weapon_idx[0] = value;
            mission = (sint16)game_runtime.selection.mission;
            aircraft = (sint16)game_runtime.selection.aircraft;
            value = aircraft_default_weapon_slot(mission, aircraft, 1);
            mission_aircraft.pending_weapon_idx[1] = value;
            mission_aircraft.weapon_idx[1] = value;
            mission = (sint16)game_runtime.selection.mission;
            aircraft = (sint16)game_runtime.selection.aircraft;
            value = aircraft_weapon_count(mission, aircraft, 0);
            mission_aircraft.weapon_count[0] = value;
            mission = (sint16)game_runtime.selection.mission;
            aircraft = (sint16)game_runtime.selection.aircraft;
            value = aircraft_weapon_count(mission, aircraft, 1);
            mission_aircraft.weapon_count[1] = value;
        }
        if ((sint16)mission_aircraft.action == 2)
        {
            resource_init_queue(0, 0);
            mission = (sint16)game_runtime.selection.mission;
            value = audio_mission_track_id(mission);
            if (value == 0xFFFFFFFF || (game_runtime.progress.flow_flags & 16))
            {
                mission_flow.root_kind = 0;
                mission_flow.root_phase = 0;
                mission_flow.root_variant = 0;
                mission_menu_start_item_motion(0, 1);
                if (game_runtime.progress.flow_flags & 16)
                    game_runtime.selection.mission = (uint16)(59);
            }
            else
            {
                mission_menu_start_item_motion(0, 5);
                mission_menu_start_item_motion(0, 6);
                ui_item_set_motion(0, 7, 0, 0, 496, 0);
                ui_group_set_mask(0, 225);
                sound_play(90);
                mission_flow.mode = 1;
            }
            for (sound = 93; sound <= 97; ++sound)
            {
                value = sound_play(sound);
                *menu_voice_slot(sound - 93) = (uint32)(value);
            }
            mission_flow.mode_phase = 0;
            ui_animation_set(1, 1, 0, 40);
            mission_aircraft.flags = 0;
            mission_aircraft.profile = NULL;
            mission_aircraft.prev_profile = NULL;
            arena_set_cursor(mission_aircraft.selection_bookmark);
            mission_aircraft.weapon_packets = NULL;
            game_runtime.mission_control.banks[1].packet_begin = (uint32)(0);
            game_runtime.mission_control.banks[1].packet_data = NULL;
            game_runtime.mission_control.banks[1].packet_capacity = 0;
            game_runtime.mission_control.banks[0].packet_begin = (uint32)(0);
            game_runtime.mission_control.banks[0].packet_data = NULL;
            game_runtime.mission_control.banks[0].packet_capacity = 0;
        }
    }
    result = game_runtime.progress.flow_flags & 16;
    if (result)
        return aircraft_preview_draw_info();
    return result;
}

void aircraft_selection_commit_weapons(void)
{
    static const uint8 secondary_codes[][2] = {
        {0, 0}, {1, 1}, {2, 2}, {1, 3}, {4, 4}, {10, 10}, {7, 7}, {1, 5}, {6, 6},
    };
    uint32 mission, aircraft, record, value;
    FUNCTION_MARKER(0x800F9B20, "ACE3.BIN");
    mission = (sint16)game_runtime.selection.mission;
    aircraft = (sint16)game_runtime.selection.aircraft;
    record = mission_aircraft.weapon_idx[0];
    value = aircraft_weapon_id(mission, aircraft, 0, record);
    if (value < 6)
        game_runtime.selection.weapon_code[0] = (uint8)(value + 1);
    mission = (sint16)game_runtime.selection.mission;
    aircraft = (sint16)game_runtime.selection.aircraft;
    record = mission_aircraft.weapon_idx[1];
    value = aircraft_weapon_id(mission, aircraft, 1, record);
    if (value >= sizeof(secondary_codes) / sizeof(*secondary_codes))
        return;
    game_runtime.selection.weapon_code[1] = secondary_codes[value][0];
    game_runtime.selection.weapon_code[2] = secondary_codes[value][1];
}

void aircraft_selection_commit_weapons_frame(void)
{
    FUNCTION_MARKER(0x800F9B10, "ACE3.BIN");
    {
        aircraft_selection_commit_weapons();
        return;
    }
}

uint32 aircraft_selection_begin_input(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800F9E68, "ACE3.BIN");
    mission_aircraft.action = 0;
    mission_aircraft.phase = 1;
    ui_widget_set(4, 0);
    result = game_runtime.progress.flow_flags & 16;
    if (result)
        return aircraft_preview_draw_info();
    return result;
}

uint32 aircraft_selection_wait_weapons(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800FA860, "ACE3.BIN");
    ui_widget_set(1, 1);
    ui_widget_set(4, 0);
    if (ui_animation_phase(8) == 2)
    {
        mission_aircraft.phase = 4;
        mission_aircraft.action = 0;
    }
    result = game_runtime.progress.flow_flags & 16;
    if (result)
        return aircraft_preview_draw_info();
    return result;
}

uint32 aircraft_selection_weapons_input(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800FA8E0, "ACE3.BIN");
    if (game_runtime.progress.flow_flags & 16)
        aircraft_preview_draw_info();
    if ((game_input.pressed & 16) && (game_input.held & 0x2F) == 0)
    {
        ui_animation_set(8, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        ui_animation_set(8, 3, -1, -1);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(7, 3, -1, -1);
        sound_play(91);
        sound_play(60);
        mission_aircraft.phase = 5;
        mission_aircraft.action = 1;
    }
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            ui_animation_set(8, 3, -1, -1);
            sound_play(91);
            sound_play(59);
            mission_aircraft.phase = 5;
            mission_aircraft.action = 2;
            return 2;
        }
    }
    return result;
}

uint32 aircraft_selection_finish_weapons(void)
{
    uint32 value, result, slot;
    FUNCTION_MARKER(0x800FAA50, "ACE3.BIN");
    ui_widget_set(4, (sint16)mission_aircraft.action != 2);
    if (ui_animation_phase(8))
    {
        value = ui_animation_ticks(8);
        scene_palette_blend(value);
    }
    else
    {
        if ((sint16)mission_aircraft.action == 1)
        {
            mission_aircraft.flags = 0;
            mission_flow.mode = 3;
            mission_flow.mode_phase = 0;
            aircraft_selection_commit_weapons_frame();
            mission_aircraft.profile = NULL;
            mission_aircraft.prev_profile = NULL;
            arena_set_cursor(mission_aircraft.selection_bookmark);
            mission_aircraft.weapon_packets = NULL;
            mission_audio_control_track(3, 0);
            sound_play(90);
        }
        if ((sint16)mission_aircraft.action == 2)
        {
            slot = 3;
            if ((game_runtime.progress.flow_flags & 16) == 0)
            {
                ui_animation_set(3, 1, 0, 25);
                ui_animation_set(5, 1, 0, 25);
                ui_animation_set(6, 1, 0, 25);
                slot = 8;
            }
            ui_animation_set(slot, 1, 0, 25);
            ui_group_set_mask(0, 0x7701);
            sound_play(90);
            value = mission_aircraft.flags;
            mission_aircraft.phase = 0;
            mission_aircraft.flags = value | 2;
        }
    }
    result = game_runtime.progress.flow_flags & 16;
    if (result)
        return aircraft_preview_draw_info();
    return result;
}

uint32 aircraft_selection_finish_weapons_frame(void)
{
    FUNCTION_MARKER(0x800FAA48, "ACE3.BIN");
    return aircraft_selection_finish_weapons();
}

uint32 aircraft_selection_input_frame(void)
{
    FUNCTION_MARKER(0x800F9EC8, "ACE3.BIN");
    return aircraft_selection_input();
}

void mission_select_load_preview(uint32 row, uint32 column)
{
    uint32 index;
    FUNCTION_MARKER(0x800FD770, "ACE3.BIN");
    index = mission_selector.entries[column + row * 4];
    {
        (void)(scene_texture_load_image_at(&mission_flow.assets.row_image_table, index, 960, 340, 384, 6));
        return;
    }
}

void mission_select_load_row(uint32 row)
{
    uint32 index, entry, result;
    sint32 count, items = 4;
    FUNCTION_MARKER(0x800FD7D0, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        *ui_widget_value(index + 53) = (uint8)(0);
    count = (sint16)mission_selector.count;
    result = (sint32)row < count / 4;
    if (!result)
    {
        result = (uint32)(count % 4) << 16;
        items = (sint16)(uint16)(count % 4);
    }
    for (index = 0; (sint32)index < items; ++index)
    {
        entry = mission_selector.entries[row * 4 + index];
        scene_texture_load_image_at(&mission_flow.assets.row_image_table, entry, 960, 340 + index * 18, 384, 6);
        *ui_widget_value(index + 53) = (uint8)(2);
        result = (sint32)(index + 1) < items;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_select_load_detail(uint32 column)
{
    uint32 row, index;
    FUNCTION_MARKER(0x800FD8F4, "ACE3.BIN");
    row = mission_selector.row;
    index = mission_selector.entries[column + row * 4];
    {
        (void)(scene_texture_load_image_at(&mission_flow.assets.detail_image_table, index, 960, 256, 768, 1));
        return;
    }
}

uint32 aircraft_selection_model_variant(uint32 mission, uint32 variant)
{
    FUNCTION_MARKER(0x800FE864, "ACE3.BIN");
    return mission_model_variant(mission, variant);
}

void mission_load_set_mission(uint32 mission)
{
    FUNCTION_MARKER(0x800FEEF8, "ACE3.BIN");
    mission_resource_load.mission = mission;
}

uint32 mission_select_load_tick(void)
{
    uint32 media, flags, phase, value;
    FUNCTION_MARKER(0x800FE1E8, "ACE3.BIN");
    media = cd_resource_state();
    flags = mission_dispatch_load(mission_resource_load.phase);
    if (flags & 1)
    {
        value = mission_resource_load.phase;
        mission_resource_load.phase = value + 1;
    }
    if (flags & 2)
    {
        mission_selector.result = 1;
        return 1;
    }
    if (flags & 4)
        mission_resource_load.phase = 1;
    phase = mission_load_status();
    if (phase)
        ui_group_set_mask(0, 0x0A000003);
    switch (phase)
    {
        case 1:
            *ui_widget_value(61) = (uint8)(4);
            return 4;
        case 2:
            *ui_widget_value(61) = (uint8)(0);
            break;
        case 3:
            *ui_widget_value(61) = (uint8)(1);
            break;
        case 4:
            *ui_widget_value(61) = (uint8)(2);
            break;
        case 5:
            *ui_widget_value(61) = (uint8)(3);
            break;
        case 6:
            *ui_widget_value(61) = (uint8)(5);
            return 5;
        default:
            return 0x800D0000;
    }
    value = game_input.pressed & 32;
    if (!value)
        return value;
    if (game_input.held & 0x5F)
        return 4;
    if (media == 4 || media == 0xFFFFFFFFu)
        return 0xFFFFFFFFu;
    if (resource_try_standby())
        mission_selector.result = 2;
    return sound_play(59);
}

uint32 mission_load_status(void)
{
    FUNCTION_MARKER(0x800FEEE0, "ACE3.BIN");
    return mission_resource_load.status;
}

uint32 mission_load_begin_media_check(void)
{
    FUNCTION_MARKER(0x800FEF08, "ACE3.BIN");
    mission_resource_load.recovered = 0;
    mission_resource_load.status = 0;
    return 1;
}

uint32 mission_load_check_media(void)
{
    uint32 media, kind, value;
    FUNCTION_MARKER(0x800FEF24, "ACE3.BIN");
    media = cd_resource_state();
    if (!media)
    {
        mission_resource_load.status = 1;
        return 0;
    }
    kind = game_data_mission_media_kind(mission_resource_load.mission);
    if (kind == 3 || media == 3)
    {
        mission_resource_load.status = 0;
        return 2;
    }
    if (media == 4 || media == 0xFFFFFFFFu)
    {
        if (kind == 1)
        {
            value = mission_resource_load.recovered;
            mission_resource_load.status = value ? 4 : 2;
        }
        kind = game_data_mission_media_kind(mission_resource_load.mission);
        if (kind == 2)
        {
            value = mission_resource_load.recovered;
            mission_resource_load.status = value ? 5 : 3;
        }
        resource_begin_recovery();
        return 1;
    }
    if (media == 1)
    {
        if (kind == 1)
        {
            mission_resource_load.status = 6;
            return 2;
        }
        value = mission_resource_load.recovered;
        mission_resource_load.status = value ? 5 : 3;
        resource_begin_recovery();
        return 1;
    }
    if (media == 2)
    {
        if (kind == 2)
        {
            mission_resource_load.status = 6;
            return 2;
        }
        value = mission_resource_load.recovered;
        mission_resource_load.status = value ? 4 : 2;
        resource_begin_recovery();
        return 1;
    }
    return 0;
}

uint32 mission_load_wait_recovery(void)
{
    FUNCTION_MARKER(0x800FF098, "ACE3.BIN");
    return resource_get_recovery_state() == 3;
}

uint32 mission_load_finish_recovery(void)
{
    uint32 status;
    FUNCTION_MARKER(0x800FF0BC, "ACE3.BIN");
    status = resource_get_recovery_state();
    if (status != 1)
        return 0;
    mission_resource_load.status = status;
    mission_resource_load.recovered = status;
    return 4;
}

uint32 aircraft_selection_frame(void)
{
    FUNCTION_MARKER(0x800FB10C, "ACE3.BIN");
    return aircraft_selection_tick();
}

uint32 aircraft_selection_tick(void)
{
    uint32 *ordering_words;
    uint32 packet, ordering, tag, link, value;
    FUNCTION_MARKER(0x800FB114, "ACE3.BIN");
    if (!(game_runtime.progress.flow_flags & 16) && (sint16)mission_aircraft.phase != 5)
        mission_audio_control_track(1, 0);
    if (((uint32)mission_aircraft.phase | ((uint32)mission_aircraft.action << 16)) != 0x00020002)
    {
        packet = game_runtime.mission_control.ot;
        ClearOTagR(mission_ot_span(packet, 256), 64);
        packet = game_runtime.mission_control.ot;
        ordering = ui_packets.scene_ot;
        ordering_words = ui_scene_ot_span(ordering, 16 * sizeof(*ordering_words));
        uint32 *mission_head = mission_ot_span(packet, sizeof(*mission_head));
        tag = *mission_head;
        link = ordering_words[3];
        *mission_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
        packet = game_runtime.mission_control.ot;
        ordering = ui_packets.scene_ot;
        ordering_words = ui_scene_ot_span(ordering, 16 * sizeof(*ordering_words));
        tag = ordering_words[3];
        ordering_words[3] = (tag & 0xFF000000) | ((packet + 252) & 0xFFFFFF);
        menu_model_draw_selection_preview();
    }
    value = mission_aircraft.phase - 3;
    if (value < 3)
    {
        aircraft_selection_update_weapons();
        packet = ui_packets.scene_packet_cursor;
        ordering = ui_packets.scene_ot;
        value = menu_geometry_draw_weapons(packet, ordering);
        ui_packets.scene_packet_cursor = (uint32)(value);
    }
    value = menu_geometry_aircraft_stats_frame(ui_packets.scene_packet_cursor);
    ui_packets.scene_packet_cursor = (uint32)(value);
    mission_dispatch_aircraft((uint32)(sint16)mission_aircraft.phase);
    return mission_audio_control_track(0, 0);
}

uint32 aircraft_selection_rating_unlocks(void)
{
    uint32 flags = 15, mission, best, value;
    FUNCTION_MARKER(0x800F78B0, "ACE3.BIN");
    for (mission = 1; mission < 56; ++mission)
    {
        if (mission == 19 || mission == 49 || mission == 54)
            continue;
        value = game_runtime.progress.mission_rating[0][mission];
        best = value ? value : 5;
        value = game_runtime.progress.mission_rating[1][mission];
        if (value && value < best)
            best = value;
        value = game_runtime.progress.mission_rating[2][mission];
        if (value && value < best)
            best = value;
        if (best == 5)
            return 0;
        if (best >= 2)
            flags &= 0xFFFFFFF7;
        if (best >= 3)
            flags &= 0xFFFFFFFB;
        if (best >= 4)
            flags &= 0xFFFFFFFD;
    }
    return flags;
}

void aircraft_selection_update_weapons(void)
{
    uint32 first, second, result = 2, value, limit, input, active, record, context;
    FUNCTION_MARKER(0x800F8C0C, "ACE3.BIN");
    first = ui_animation_phase(9);
    second = ui_animation_phase(10);
    if (first == 0 || first == 2)
    {
        value = mission_aircraft.pending_weapon_idx[0];
        mission_aircraft.weapon_idx[0] = value;
    }
    if (second == 0 || second == 2)
    {
        result = mission_aircraft.pending_weapon_idx[1];
        mission_aircraft.weapon_idx[1] = result;
    }
    if (first == 2)
        ui_animation_set(9, 0, 0, 0);
    if (second == 2)
        ui_animation_set(10, 0, 0, 0);
    if (first || second)
        return;
    if (game_input.pressed & 3)
    {
        value = mission_aircraft.weapon_tab;
        mission_aircraft.weapon_tab = 1 - value;
    }
    ui_widget_set(9, 0);
    ui_widget_set(8, 0);
    ui_widget_set(11, 0);
    ui_widget_set(10, 0);
    if ((sint16)mission_aircraft.phase != 5)
    {
        input = game_input.repeat;
        if (input & 8)
        {
            if (!mission_aircraft.weapon_tab)
            {
                limit = mission_aircraft.weapon_count[0];
                value = mission_aircraft.weapon_idx[0];
                if ((sint32)value < (sint32)limit - 1)
                {
                    mission_aircraft.pending_weapon_idx[0] = value + 1;
                    ui_animation_set(9, 3, 10, 10);
                    ui_widget_set(9, 1);
                    sound_play(61);
                }
            }
            if (mission_aircraft.weapon_tab == 1)
            {
                limit = mission_aircraft.weapon_count[1];
                value = mission_aircraft.weapon_idx[1];
                if ((sint32)value < (sint32)limit - 1)
                {
                    mission_aircraft.pending_weapon_idx[1] = value + 1;
                    ui_animation_set(10, 3, 10, 10);
                    ui_widget_set(11, 1);
                    sound_play(61);
                }
            }
        }
        else if (input & 4)
        {
            if (!mission_aircraft.weapon_tab)
            {
                value = mission_aircraft.weapon_idx[0];
                if (value)
                {
                    mission_aircraft.pending_weapon_idx[0] = value - 1;
                    ui_animation_set(9, 1, 0, 10);
                    ui_widget_set(8, 1);
                    sound_play(61);
                }
            }
            if (mission_aircraft.weapon_tab == 1)
            {
                value = mission_aircraft.weapon_idx[1];
                if (value)
                {
                    mission_aircraft.pending_weapon_idx[1] = value - 1;
                    ui_animation_set(10, 1, 0, 10);
                    ui_widget_set(10, 1);
                    sound_play(61);
                }
            }
        }
    }
    limit = mission_aircraft.weapon_count[0];
    value = mission_aircraft.weapon_idx[0];
    active = (sint32)value < (sint32)limit - 1 ? 2 : 0;
    if (value)
        active |= 1;
    ui_widget_set(7, active);
    limit = mission_aircraft.weapon_count[1];
    value = mission_aircraft.weapon_idx[1];
    active = (sint32)value < (sint32)limit - 1 ? 2 : 0;
    if (value)
        active |= 1;
    ui_widget_set(14, active);
    ui_widget_set(15, mission_aircraft.weapon_tab);
    active = mission_aircraft.weapon_tab;
    if (active != 0 && active != 1)
        return;
    record = mission_aircraft.pending_weapon_idx[active ? 1 : 0];
    value = (sint16)game_runtime.selection.mission;
    context = (sint16)game_runtime.selection.aircraft;
    result = aircraft_weapon_id(value, context, active ? 2 : 0, record);
    mission_aircraft.weapon_info = result;
    return;
}

void mission_frontend_idle_phase(void)
{
    FUNCTION_MARKER(0x800D1410, "ACE3.BIN");
}

uint32 mission_frontend_load_resource124(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 bytes, buffer, value;

    FUNCTION_MARKER(0x800D1418, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(124);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    value = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = value;
    return value;
}

void mission_frontend_wait_resource124(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800D14A0, "ACE3.BIN");
    value = game_runtime.resource_pending;
    if (value == 1)
        return;
    mission_flow.transition_phase = 0;
    return;
}

uint32 mission_frontend_load_resource125(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 bytes, buffer, value;

    FUNCTION_MARKER(0x800D14D0, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(125);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    value = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = value;
    return value;
}

void mission_frontend_wait_resource125(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800D1558, "ACE3.BIN");
    value = game_runtime.resource_pending;
    if (value == 1)
        return;
    mission_flow.transition_phase = 0;
    return;
}

void mission_dispatch_current_mode(void)
{
    uint32 kind, phase;

    FUNCTION_MARKER(0x800E6954, "ACE3.BIN");
    kind = (sint16)mission_flow.mode;
    phase = (sint16)mission_flow.mode_phase;
    {
        mission_dispatch_mode(kind, phase);
        return;
    }
}

uint32 mission_frontend_frame(void)
{
    uint32 *frame_middle_tag;
    uint32 *ordering_entry;
    uint32 buffer, value, ordering, tag, head, first, second;
    uint32 kind, phase, packet, result, flags;

    FUNCTION_MARKER(0x800CDF40, "ACE3.BIN");
    buffer = game_runtime.display_buffer;
    ui_packets.scene_prims = ui_packets.scene_bank[buffer].prims;
    ui_packets.scene_prims_address = ui_packets.scene_bank[buffer].prims_address;
    buffer = game_runtime.display_buffer;
    value = ui_packets.scene_bank[buffer].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[buffer].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    buffer = game_runtime.display_buffer;
    ordering = ui_packets.scene_bank[buffer].ot;
    ui_packets.scene_ot = (uint32)(ordering);
    ui_packets.scene_ot_data = ui_packets.scene_bank[buffer].ot_data;
    ClearOTagR(ui_scene_ot_span(ordering, 64), 16);
    ordering = ui_packets.scene_ot;
    first = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    tag = *ordering_entry;
    ordering_entry = frame_middle_tag;
    head = *ordering_entry;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    *ordering_entry = (tag & 0xFF000000) | (head & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    first = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    ordering_entry = frame_middle_tag;
    tag = *ordering_entry;
    ordering_entry = frame_middle_tag;
    *ordering_entry = (tag & 0xFF000000) | ((ordering + 60) & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    text_begin_frame(ordering);
    ordering = ui_packets.scene_ot;
    mission_effect_begin_frame(ordering + 60);
    buffer = game_runtime.display_buffer;
    ordering = ui_packets.scene_bank[buffer].detail_ot;
    ui_packets.scene_detail_ot = (uint32)(ordering);
    ui_packets.scene_detail_ot_data = ui_packets.scene_bank[buffer].detail_ot_data;
    ClearOTagR(ui_detail_ot_span(ordering, 1024), 256);
    ordering = ui_packets.scene_detail_ot;
    first = ui_packets.scene_ot;
    ordering_entry = ui_detail_ot_span(ordering, sizeof(*ordering_entry));
    tag = *ordering_entry;
    ordering_entry = ui_scene_ot_span(first + 4, sizeof(*ordering_entry));
    head = *ordering_entry;
    ordering_entry = ui_detail_ot_span(ordering, sizeof(*ordering_entry));
    *ordering_entry = (tag & 0xFF000000) | (head & 0xFFFFFF);
    first = ui_packets.scene_ot;
    ordering = ui_packets.scene_detail_ot;
    ordering_entry = ui_scene_ot_span(first + 4, sizeof(*ordering_entry));
    tag = *ordering_entry;
    ordering_entry = ui_scene_ot_span(first + 4, sizeof(*ordering_entry));
    *ordering_entry = (tag & 0xFF000000) | ((ordering + 1020) & 0xFFFFFF);
    buffer = game_runtime.display_buffer;
    value = game_runtime.mission_control.banks[buffer].ot;
    game_runtime.mission_control.ot = (uint32)(value);
    buffer = game_runtime.display_buffer;
    value = game_runtime.mission_control.banks[buffer].packet_begin;
    game_runtime.mission_control.packet_cur = (uint32)(value);
    xport_gte_write_control(13, 3072);
    xport_gte_write_control(14, 3072);
    xport_gte_write_control(15, 3072);
    xport_gte_write_control(21, 1536);
    xport_gte_write_control(22, 1536);
    xport_gte_write_control(23, 1536);
    xport_gte_write_control(27, 0xFFFF8800);
    xport_gte_write_control(28, 0x03000000);
    xport_gte_write_control(8, 0x08000000);
    xport_gte_write_control(9, 0x04000000);
    xport_gte_write_control(10, 1024);
    xport_gte_write_control(11, 0x0400FC00);
    xport_gte_write_control(12, 0);
    xport_gte_write_control(16, 0x02000400);
    xport_gte_write_control(17, 0x04000200);
    xport_gte_write_control(18, 0x02000200);
    xport_gte_write_control(19, 0x02000400);
    xport_gte_write_control(20, 512);
    value = game_input.kind;
    ui_widget_set(20, (value & 16) != 0);
    value = (uint8)game_runtime.selection.difficulty;
    *ui_widget_value(32) = (uint8)(value);
    kind = mission_flow.root_kind;
    phase = mission_flow.root_phase;
    mission_dispatch_root(kind, phase);
    flags = game_runtime.progress.flow_flags;
    if (flags & 16)
    {
        if ((sint16)mission_flow.mode == 6)
            goto after_prompt;
        kind = mission_flow.root_kind;
        value = kind ? 5 : mission_flow.root_variant + 9;
    }
    else
    {
        kind = mission_flow.root_kind;
        if (kind == 1 || (sint16)mission_flow.mode == 6)
            goto after_prompt;
        value = mission_flow.root_variant + (kind ? 5 : 1);
    }
    ui_widget_set(44, value);
after_prompt:
{
}
    packet = ui_packets.scene_packet_cursor;
    result = mission_effect_draw(packet);
    ui_packets.scene_packet_cursor = (uint32)(result);
    result = ui_render_frame(result);
    ui_packets.scene_packet_cursor = (uint32)(result);
    value = mission_flow.frontend_frames;
    mission_flow.frontend_frames = (uint32)(value + 1);
    flags = game_input.kind;
    *ui_widget_value(40) = (uint8)((flags & 16) != 0);
    value = mission_flow.next_scene;
    if (value)
    {
        sound_stop_voice(*menu_voice_slot(0));
        sound_stop_voice(*menu_voice_slot(1));
        sound_stop_voice(*menu_voice_slot(2));
        sound_stop_voice(*menu_voice_slot(3));
        sound_stop_voice(*menu_voice_slot(4));
        sound_disable_reverb();
        sound_stop_all();
        resource_init_queue(0, 0);
        flags = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(flags | 4);
        if ((uint8)mission_flow.next_scene == 4)
        {
            first = game_runtime.selection.mission;
            second = game_runtime.selection.aircraft;
            game_runtime.selection.saved_mission = (uint16)(first);
            game_runtime.selection.saved_aircraft = (uint16)(second);
        }
        return mission_flow.next_scene;
    }
    if (mission_flow.exit_requested == 1)
    {
        sound_stop_all();
        resource_init_queue(0, 0);
        flags = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(flags | 4);
        first = game_runtime.selection.mission;
        second = game_runtime.selection.aircraft;
        game_runtime.selection.saved_mission = (uint16)(first);
        game_runtime.selection.saved_aircraft = (uint16)(second);
        return 0x60000004;
    }
    if (mission_flow.playback_requested == 1)
    {
        sound_stop_all();
        resource_init_queue(0, 0);
        game_runtime.playback.return_mode = (uint32)(3);
        flags = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(flags | 4);
        return 0x60000005;
    }
    return 0;
}

void mission_launch_begin(void)
{
    uint32 flags;

    FUNCTION_MARKER(0x800E6288, "ACE3.BIN");
    game_runtime.account.route_mode = (uint16)(1);
    ui_group_set_mask(0, 0x01004001);
    mission_flow.mode_phase = 1;
    mission_effect_set_ring(3);
    mission_effect_close_sprs();
    sound_play(104);
    flags = game_runtime.progress.flow_flags & 0x10;
    *ui_widget_value(46) = (uint8)(flags == 0);
    return;
}

uint32 mission_launch_wait_frame(void)
{
    FUNCTION_MARKER(0x800E6334, "ACE3.BIN");
    return mission_launch_wait_ring();
}

uint32 mission_launch_wait_ring(void)
{
    uint32 result;

    FUNCTION_MARKER(0x800E633C, "ACE3.BIN");
    if ((game_input.pressed & 0x10) && !(game_input.held & 0x2F))
        mission_effect_set_ring(0);
    result = mission_effect.layers[MISSION_EFFECT_RING].mode;
    if (!result)
    {
        ui_animation_set(1, 3, 30, 0);
        ui_animation_set(15, 3, -1, -1);
        ui_animation_set(11, 3, -1, -1);
        result = mission_flow.mode_phase + 1;
        mission_flow.mode_phase = result;
    }
    return result;
}

uint32 mission_launch_fade_tick(void)
{
    uint32 progress, result, numerator, high, sum;
    sint32 kind, quotient;
    sint64 product;

    FUNCTION_MARKER(0x800E63F4, "ACE3.BIN");
    progress = ui_animation_ticks(1);
    result = ui_animation_phase(1);
    if (!result)
    {
        kind = (sint16)game_runtime.selection.mission;
        if (kind == 16)
        {
            game_runtime.playback.movie_idx = (uint8)(8);
            result = 2;
            game_runtime.playback.return_mode = (uint32)(result);
            mission_flow.next_scene = 0x60000005;
        }
        else if (kind == 37)
        {
            game_runtime.playback.movie_idx = (uint8)(26);
            game_runtime.selection.mission = (uint16)(57);
            result = 2;
            game_runtime.playback.return_mode = (uint32)(result);
            mission_flow.next_scene = 0x60000005;
        }
        else if (kind == 42)
        {
            result = 56;
            game_runtime.selection.mission = (uint16)(result);
            mission_flow.next_scene = 0x60000002;
        }
        else
        {
            result = 0x60000002;
            mission_flow.next_scene = result;
        }
        return result;
    }
    numerator = (((progress << 3) + progress) << 4) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    game_runtime.display.background[0] = (uint8)((uint32)quotient);
    numerator = (((progress << 4) + progress) << 3) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    game_runtime.display.background[1] = (uint8)((uint32)quotient);
    numerator = (((((progress << 2) + progress) << 2) + progress) << 2) - progress;
    product = (sint64)(sint32)numerator * (sint32)0x88888889;
    high = (product >> 32);
    sum = high + numerator;
    quotient = ((sint32)sum >> 4) - ((sint32)numerator >> 31);
    game_runtime.display.background[2] = (uint8)((uint32)quotient);
    result = game_runtime.display.flags | 8;
    game_runtime.display.flags = (uint32)(result);
    return result;
}

uint32 mission_resume_confirm_tick(void)
{
    uint32 result;

    FUNCTION_MARKER(0x800E65A4, "ACE3.BIN");
    result = game_input.pressed & 0x10;
    if (result)
    {
        result = game_input.held & 0x2F;
        if (!result)
        {
            result = 2;
            game_runtime.playback.resume_mode = (uint32)result;
            mission_flow.next_scene = 0x60000002;
        }
    }
    return result;
}

uint32 mission_replay_prompt_begin(void)
{
    FUNCTION_MARKER(0x800E65F8, "ACE3.BIN");
    mission_flow.mode_phase = 1;
    return 1;
}

uint32 mission_replay_prompt_tick(void)
{
    uint32 result, flags;

    FUNCTION_MARKER(0x800E660C, "ACE3.BIN");
    result = game_input.pressed & 0x10;
    if (result)
    {
        flags = game_input.held & 0x2F;
        result = 0x80110000;
        if (!flags)
        {
            result = 5;
            mission_flow.mode = result;
            mission_flow.mode_phase = 0;
        }
    }
    return result;
}

void mission_image_optional_data_hook(void)
{
    FUNCTION_MARKER(0x8002D6E0, "ACE3.BIN");
    return;
}

void mission_frontend_choose_root_tick(void)
{
    uint32 value, mode, mission;

    FUNCTION_MARKER(0x800CEDA8, "ACE3.BIN");
    ui_map_draw_grid();
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        if ((sint16)mission_flow.transition_phase == 3)
        {
            value = mission_flow.menu_phase;
            mode = mission_flow.root_variant;
            mission_flow.menu_phase = value + 1;
            if (mode == 0)
            {
                if (!(game_runtime.progress.flow_flags & 16))
                    ui_item_reverse_motion(0, 1);
                if (mission_flow.root_variant == 0)
                {
                    mission = (sint16)game_runtime.selection.mission;
                    if (mission == 59 || (sint16)game_runtime.account.route_mode == 3)
                        mission_flow.transition_phase = 8;
                }
            }
            if (mission_flow.root_variant == 1)
            {
                if (game_runtime.progress.flow_flags & 16)
                    mission_flow.transition_phase = 3;
                else
                {
                    arena_set_cursor(mission_flow.assets.selection_buffer.encoded_begin);
                    mission_flow.transition_phase = 4;
                }
            }
            if (mission_flow.root_variant == 2)
            {
                arena_set_cursor(mission_flow.assets.selection_buffer.encoded_begin);
                value = arena_alloc(188);
                mission_flow.scene_archive.arena_begin = value;
                mission_flow.transition_phase = 6;
            }
            ui_animation_set(1, 3, -1, -1);
            ui_animation_set(2, 3, -1, -1);
            if (mission_flow.root_variant == 0)
                mission_effect_close_sprs();
            if (mission_flow.root_variant == 3)
            {
                arena_set_cursor(mission_flow.assets.selection_buffer.encoded_begin);
                mission_flow.transition_phase = 10;
            }
            sound_play(60);
        }
    }
    if (game_runtime.progress.flow_flags & 16)
    {
        if (game_input.repeat & 8)
        {
            if ((sint16)mission_flow.transition_phase == 3)
            {
                if (ui_item_motion_phase(0, 1) == 2)
                    goto finish_menu_input;
                if ((sint16)mission_flow.transition_phase != 3)
                    goto finish_menu_input;
                mode = mission_flow.root_variant;
                mission_flow.root_variant = mode == 0 ? mode + 1 : 0;
                sound_play(61);
                goto finish_menu_input;
            }
        }
        if (!(game_input.repeat & 4))
            goto finish_menu_input;
        if ((sint16)mission_flow.transition_phase != 3 || ui_item_motion_phase(0, 1) == 2)
            goto finish_menu_input;
        if ((sint16)mission_flow.transition_phase != 3)
            goto finish_menu_input;
        mode = mission_flow.root_variant;
        mission_flow.root_variant = mode == 0 ? 1 : mode - 1;
        sound_play(61);
    }
    else
    {
        if (game_input.repeat & 8)
        {
            if ((sint16)mission_flow.transition_phase == 3)
            {
                if (ui_item_motion_phase(0, 1) == 2)
                    goto finish_menu_input;
                if ((sint16)mission_flow.transition_phase != 3)
                    goto finish_menu_input;
                mode = mission_flow.root_variant;
                mission_flow.root_variant = mode < 3 ? mode + 1 : 0;
                sound_play(61);
                goto finish_menu_input;
            }
        }
        if (!(game_input.repeat & 4))
            goto finish_menu_input;
        if ((sint16)mission_flow.transition_phase != 3 || ui_item_motion_phase(0, 1) == 2)
            goto finish_menu_input;
        value = (sint16)mission_flow.transition_phase;
        if (value != 3)
            goto finish_menu_input;
        mode = mission_flow.root_variant;
        mission_flow.root_variant = mode == 0 ? value : mode - 1;
        sound_play(61);
    }
finish_menu_input:
    ui_map_spread_frame();
    ui_map_render();
    {
        ui_map_tick_frame();
        return;
    }
}

uint32 mission_frontend_root_frame(void)
{
    static void (*const menu_callbacks[])(void) = {
        mission_frontend_root_open_tick,
        mission_frontend_choose_root_tick,
        mission_frontend_root_close_tick,
    };
    uint32 phase;

    FUNCTION_MARKER(0x800CF494, "ACE3.BIN");
    phase = (sint16)mission_flow.menu_phase;
    if (phase >= sizeof(menu_callbacks) / sizeof(menu_callbacks[0]))
        abort();
    menu_callbacks[phase]();
    phase = (sint16)mission_flow.transition_phase;
    return mission_dispatch_loading(phase, 0, (RESOURCE_STREAM_BUFFER){0});
}

void mission_dialogue_media_request_begin(void)
{
    uint32 id, value;

    FUNCTION_MARKER(0x800F29E8, "ACE3.BIN");
    id = dialogue_sequence_sel_id();
    value = dialogue_sequence_entry(id)->audio_resource;
    if (resource_is_cd_audio(value) == 1)
    {
        ui_group_set_mask(0, 0);
        arena_set_cursor(mission_flow.assets.arena_begin);
        sound_play(60);
        mission_flow.root_phase = 2;
        mission_flow.mode_phase = 0;
        return;
    }
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return;
}

uint32 mission_dialogue_media_request_check(void)
{
    uint32 id, value, status;

    FUNCTION_MARKER(0x800F2A8C, "ACE3.BIN");
    id = dialogue_sequence_sel_id();
    value = dialogue_sequence_entry(id)->audio_resource;
    status = resource_media_kind(value);
    if (status != 1 && status != 3 && status != 2)
        return 2;
    resource_begin_recovery();
    ui_group_set_mask(0, 0x0200000B);
    *ui_widget_value(61) = (uint8)(status == 2 ? 1 : 0);
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return value;
}

void mission_dialogue_media_request_wait(void)
{
    uint32 status, result;

    FUNCTION_MARKER(0x800F2B6C, "ACE3.BIN");
    status = resource_get_recovery_state();
    if (status == 3)
    {
        result = mission_flow.mode_phase + 1;
        mission_flow.mode_phase = result;
    }
    result = game_input.pressed & 32;
    if (result != 0 && status != 1)
    {
        status = resource_try_standby();
        mission_flow.root_phase = status == 1 ? 1 : 5;
        mission_flow.mode_phase = status == 1 ? 0 : 3;
        return;
    }
    return;
}

void mission_dialogue_media_request_finish(void)
{
    uint32 status, result;

    FUNCTION_MARKER(0x800F2C34, "ACE3.BIN");
    status = resource_get_recovery_state();
    if (status == 0 || status == 2)
    {
        result = mission_flow.mode_phase + 1;
        mission_flow.mode_phase = result;
    }
    else if (status == 1)
    {
        *ui_widget_value(61) = (uint8)(4);
    }
    result = game_input.pressed & 32;
    if (result != 0 && status != 1)
    {
        status = resource_try_standby();
        mission_flow.root_phase = status == 1 ? 1 : 5;
        mission_flow.mode_phase = status == 1 ? 0 : 3;
        return;
    }
    return;
}

void mission_dialogue_media_request_retry(void)
{
    uint32 id, value;

    FUNCTION_MARKER(0x800F2D1C, "ACE3.BIN");
    id = dialogue_sequence_sel_id();
    value = dialogue_sequence_entry(id)->audio_resource;
    if (resource_is_cd_audio(value) == 1)
    {
        ui_group_set_mask(0, 0);
        arena_set_cursor(mission_flow.assets.arena_begin);
        sound_play(60);
        mission_flow.root_phase = 2;
        mission_flow.mode_phase = 0;
        return;
    }
    value = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = value;
    return;
}

void mission_dialogue_media_request_recheck(void)
{
    uint32 id, value, status;

    FUNCTION_MARKER(0x800F2DC0, "ACE3.BIN");
    id = dialogue_sequence_sel_id();
    value = dialogue_sequence_entry(id)->audio_resource;
    status = resource_media_kind(value);
    if (status != 1 && status != 3 && status != 2)
        return;
    resource_begin_recovery();
    ui_group_set_mask(0, 0x0200000B);
    *ui_widget_value(61) = (uint8)(status == 2 ? 3 : 2);
    mission_flow.mode_phase = 2;
    return;
}

uint32 mission_dialogue_media_cancel_begin(void)
{
    uint32 status, mission, kind, owner, result;

    FUNCTION_MARKER(0x800F2EB0, "ACE3.BIN");
    status = cd_resource_state();
    if (status == 1 || status == 2)
    {
        mission = (sint16)game_runtime.selection.mission;
        kind = game_data_mission_media_kind(mission);
        if ((status == 1 && (kind == 1 || kind == 3)) || (status == 2 && kind - 2 < 2))
        {
            ui_group_set_mask(0, 3);
            mission_flow.root_kind = 0;
            mission_flow.root_phase = 0;
            owner = mission_flow.scene_archive.arena_begin;
            mission_flow.transition_phase = 0;
            arena_set_cursor(owner);
            return sound_play(59);
        }
    }
    else if (status != 4 && status != 0xFFFFFFFF)
        return 0xFFFFFFFF;
    resource_begin_recovery();
    ui_group_set_mask(0, 0x0200000B);
    *ui_widget_value(61) = (uint8)(status == 1 ? 1 : 0);
    result = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = result;
    return result;
}

uint32 mission_dialogue_media_cancel_wait(void)
{
    uint32 result;

    FUNCTION_MARKER(0x800F3020, "ACE3.BIN");
    if (resource_get_recovery_state() != 3)
        return 0x80110000;
    result = mission_flow.mode_phase + 1;
    mission_flow.mode_phase = result;
    return result;
}

uint32 mission_dialogue_media_cancel_finish(void)
{
    uint32 status, result;

    FUNCTION_MARKER(0x800F3064, "ACE3.BIN");
    status = resource_get_recovery_state();
    if (status == 0 || status == 2)
    {
        result = mission_flow.mode_phase + 1;
        mission_flow.mode_phase = result;
        return result;
    }
    if (status != 1)
        return 0x80110000;
    *ui_widget_value(61) = (uint8)(4);
    return 4;
}

void mission_dialogue_media_cancel_retry(void)
{
    uint32 status, mission, kind, owner, display;

    FUNCTION_MARKER(0x800F30D4, "ACE3.BIN");
    status = cd_resource_state();
    display = 2;
    if (status == 1 || status == 2)
    {
        mission = (sint16)game_runtime.selection.mission;
        kind = game_data_mission_media_kind(mission);
        if (status == 1)
            display = 3;
        if ((status == 1 && (kind == 1 || kind == 3)) || (status == 2 && kind - 2 < 2))
        {
            ui_group_set_mask(0, 3);
            mission_flow.root_kind = 0;
            mission_flow.root_phase = 0;
            owner = mission_flow.scene_archive.arena_begin;
            mission_flow.transition_phase = 0;
            arena_set_cursor(owner);
            {
                sound_play(59);
                return;
            }
        }
    }
    else if (status != 4 && status != 0xFFFFFFFF)
        return;
    resource_begin_recovery();
    ui_group_set_mask(0, 0x0200000B);
    *ui_widget_value(61) = (uint8)(display);
    mission_flow.mode_phase = 1;
    return;
}

void mission_dialogue_dispatch_media_request(void)
{
    sint16 phase;

    FUNCTION_MARKER(0x800D1258, "ACE3.BIN");
    phase = (sint16)mission_flow.mode_phase;
    switch (phase)
    {
        case 0:
        {
            mission_dialogue_media_request_begin();
            return;
        }
        case 1:
        {
            mission_dialogue_media_request_check();
            return;
        }
        case 2:
        {
            mission_dialogue_media_request_wait();
            return;
        }
        case 3:
        {
            mission_dialogue_media_request_finish();
            return;
        }
        case 4:
        {
            mission_dialogue_media_request_retry();
            return;
        }
        case 5:
        {
            mission_dialogue_media_request_recheck();
            return;
        }
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 request phase %d\n", phase);
#endif
            abort();
    }
}

void mission_dialogue_dispatch_media_cancel(void)
{
    sint16 phase;

    FUNCTION_MARKER(0x800D129C, "ACE3.BIN");
    phase = (sint16)mission_flow.mode_phase;
    switch (phase)
    {
        case 0:
        {
            mission_dialogue_media_cancel_begin();
            return;
        }
        case 1:
        {
            mission_dialogue_media_cancel_wait();
            return;
        }
        case 2:
        {
            mission_dialogue_media_cancel_finish();
            return;
        }
        case 3:
        {
            mission_dialogue_media_cancel_retry();
            return;
        }
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 cancel phase %d\n", phase);
#endif
            abort();
    }
}

uint32 mission_frontend_wait_aircraft_assets(void)
{
    uint32 result;

    FUNCTION_MARKER(0x800CE6B0, "ACE3.BIN");
    if (game_runtime.resource_pending != 0)
        return 0x80110000;
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 aircraft_bind_loaded_assets(uint32 loaded, uint8 *data, uint32 data_bytes)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint16 wire_halfword_3;
    MISSION_SCENE_ASSETS *assets = &mission_flow.assets;
    uint8 *bundle;
    uint32 entries, profile, model, count, end = assets->aircraft_bundle_end;

    FUNCTION_MARKER(0x800CE6F4, "ACE3.BIN");
    if (loaded > end || end - loaded < 12)
        abort();
    if (!data || data_bytes < end - loaded)
        abort();
    bundle = data;
    if ((memcpy(&wire_halfword_0, bundle, sizeof(wire_halfword_0)), wire_halfword_0) != 2)
        abort();
    resource_relocate_directory(loaded, 0, bundle, end - loaded);
    profile = (memcpy(&wire_word_1, bundle + 4, sizeof(wire_word_1)), wire_word_1);
    model = (memcpy(&wire_word_2, bundle + 8, sizeof(wire_word_2)), wire_word_2);
    if (profile < loaded || profile - loaded < 12 || model <= profile || model > end || end - model < 4 || (model & 3) || (model - profile) % sizeof(AIRCRAFT_PROFILE))
        abort();
    count = (memcpy(&wire_halfword_3, bundle + (model - loaded), sizeof(wire_halfword_3)), wire_halfword_3);
    if (count > (end - model - 4) / sizeof(uint32))
        abort();
    entries = resource_relocate_directory(model, 0, bundle + (model - loaded), end - model);
    assets->aircraft_profiles = bundle + (profile - loaded);
    assets->aircraft_profile_bytes = end - profile;
    assets->aircraft_model_table = (const uint32 *)(bundle + (entries - loaded));
    assets->aircraft_model_count = count;
    mission_flow.transition_phase = 3;
    return 3;
}

uint32 mission_frontend_bind_aircraft_assets(void)
{
    FUNCTION_MARKER(0x800CE6EC, "ACE3.BIN");
    return aircraft_bind_loaded_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
}

uint32 mission_load_preserve_result(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x800CE754, "ACE3.BIN");
    return incoming_v0;
}

uint32 mission_frontend_load_resource121(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 size, buffer, result;

    FUNCTION_MARKER(0x800CE75C, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(121);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    mission_flow.assets.selection_buffer = asset_allocation;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 mission_frontend_queue_aircraft_assets(uint32 mode, RESOURCE_STREAM_BUFFER incoming_buffer)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    static const uint16 aircraft_resources[] = {117, 116, 118, 119, 120};
    uint32 flags, group, value, buffer, mission, result;

    FUNCTION_MARKER(0x800CE55C, "ACE3.BIN");
    flags = game_runtime.progress.flow_flags;
    if (flags & 16)
    {
        group = mission_aircraft.group;
    }
    else
        group = mission_aircraft_group((uint32)(sint16)game_runtime.selection.mission, 0);
    /* Modes other than zero and one retain the original incoming S2 carrier */
    asset_allocation = incoming_buffer;
    buffer = asset_allocation.encoded_begin;
    if (mode == 0)
    {
        if (group >= sizeof(aircraft_resources) / sizeof(*aircraft_resources))
            abort();
        value = aircraft_resources[group];
        game_runtime.resource_slot.resource_id = (uint32)(value);
        value = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
        asset_allocation = arena_alloc_backed(value);
        buffer = asset_allocation.encoded_begin;
        mission_flow.assets.selection_buffer = asset_allocation;
    }
    if (mode == 1)
    {
        if (group >= sizeof(aircraft_resources) / sizeof(*aircraft_resources))
            abort();
        value = aircraft_resources[group];
        game_runtime.resource_slot.resource_id = (uint32)(value);
        asset_allocation = mission_flow.assets.selection_buffer;
        buffer = asset_allocation.encoded_begin;
    }
    mission = (sint16)game_runtime.selection.mission;
    value = mission_aircraft_count(mission, group);
    mission_aircraft.variant_count = (uint32)(value);
    game_runtime.resource_pending = 1;
    mission_flow.assets.aircraft_bundle_end = resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 mission_frontend_load_aircraft_assets(uint32 mode, RESOURCE_STREAM_BUFFER incoming_buffer)
{
    FUNCTION_MARKER(0x800CE554, "ACE3.BIN");
    return mission_frontend_queue_aircraft_assets(mode, incoming_buffer);
}

uint32 mission_frontend_bind_resource121(void)
{
    const uint8 *asset_data;
    uint32 asset_bytes;
    uint32 resource;

    uint32 sections[3];

    FUNCTION_MARKER(0x800CE7F4, "ACE3.BIN");
    if (game_runtime.resource_pending == 1)
        return 0x800C0000;
    resource = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_flow.assets.arena_begin = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 3);
    scene_texture_load_group(sections[2], (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, resource});
    arena_set_cursor(sections[2]);
    mission_flow.assets.row_image_table = mission_image_directory_bind(sections[0], sections[1], resource, asset_data, asset_bytes);
    mission_flow.assets.detail_image_table = mission_image_directory_bind(sections[1], sections[2], resource, asset_data, asset_bytes);
    mission_flow.transition_phase = 3;
    return 3;
}

uint32 mission_frontend_wait_resource121(void)
{
    FUNCTION_MARKER(0x800CE7EC, "ACE3.BIN");
    return mission_frontend_bind_resource121();
}

uint32 mission_frontend_load_resource122(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 size, buffer, result;

    FUNCTION_MARKER(0x800CE894, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(122);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    mission_flow.assets.selection_buffer = asset_allocation;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 mission_frontend_load_resource123(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 size, buffer, result;

    FUNCTION_MARKER(0x800CE9E4, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(123);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 mission_frontend_bind_resource122(void)
{
    const uint8 *asset_data;
    uint32 asset_bytes;
    uint32 resource;

    uint32 sections[4];

    FUNCTION_MARKER(0x800CE92C, "ACE3.BIN");
    if (game_runtime.resource_pending == 1)
        return 0x800C0000;
    resource = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_flow.assets.arena_begin = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 4);
    scene_texture_load_group(sections[3], (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, resource});
    arena_set_cursor(sections[3]);
    mission_flow.assets.row_image_table = mission_image_directory_bind(sections[0], sections[1], resource, asset_data, asset_bytes);
    mission_flow.assets.secondary_image_table = mission_image_directory_bind(sections[1], sections[2], resource, asset_data, asset_bytes);
    mission_flow.assets.detail_image_table = mission_image_directory_bind(sections[2], sections[3], resource, asset_data, asset_bytes);
    mission_flow.transition_phase = 3;
    return 3;
}

uint32 mission_frontend_wait_resource122(void)
{
    FUNCTION_MARKER(0x800CE924, "ACE3.BIN");
    return mission_frontend_bind_resource122();
}

uint32 mission_frontend_bind_resource123(void)
{
    const uint8 *asset_data;
    uint32 asset_bytes;
    uint32 resource;

    uint32 sections[2];

    FUNCTION_MARKER(0x800CEA74, "ACE3.BIN");
    if (game_runtime.resource_pending == 1)
        return 0x800C0000;
    resource = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_flow.assets.arena_begin = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 2);
    mission_flow.assets.row_image_table = mission_image_directory_bind(sections[0], sections[1], resource, asset_data, asset_bytes);
    mission_flow.assets.detail_image_table = mission_image_directory_bind(sections[1], arena_cursor(), resource, asset_data, asset_bytes);
    mission_flow.transition_phase = 3;
    return 3;
}

uint32 mission_frontend_wait_resource123(void)
{
    FUNCTION_MARKER(0x800CEA6C, "ACE3.BIN");
    return mission_frontend_bind_resource123();
}

uint32 mission_frontend_bind_options_assets(void)
{
    uint32 value, result;

    FUNCTION_MARKER(0x800CEB9C, "ACE3.BIN");
    if (game_runtime.resource_pending == 1)
        return 1;
    if (game_runtime.progress.movie_branch == 0)
    {
        mission_flow.transition_phase = 3;
        return 3;
    }
    value = menu_bind_options_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
    result = mission_flow.transition_phase;
    ++result;
    mission_flow.transition_phase = result;
    mission_flow.assets.sound_test_assets = (uint32)(value);
    return result;
}

uint32 mission_frontend_wait_options_assets(void)
{
    FUNCTION_MARKER(0x800CEB94, "ACE3.BIN");
    return mission_frontend_bind_options_assets();
}

uint32 mission_frontend_finish_asset_load(void)
{
    FUNCTION_MARKER(0x800CED08, "ACE3.BIN");
    if (game_runtime.resource_pending == 1)
        return 0x80110000;
    mission_flow.transition_phase = 3;
    return 3;
}

void mission_frontend_root_open_tick(void)
{
    uint32 value, result;

    FUNCTION_MARKER(0x800CED38, "ACE3.BIN");
    if (ui_item_motion_phase(0, 1) == 0)
    {
        value = mission_flow.ui_timer;
        mission_flow.ui_timer = (uint32)(value + 1);
    }
    if (ui_animation_phase(1) != 2)
        return;
    result = mission_flow.menu_phase + 1;
    mission_flow.menu_phase = result;
    return;
}

void mission_frontend_root_close_tick(void)
{
    uint32 phase, mode, mission, value;

    FUNCTION_MARKER(0x800CF1DC, "ACE3.BIN");
    if (ui_animation_phase(1) != 0)
        return;
    phase = (sint16)mission_flow.transition_phase;
    if (phase != 3)
        return;
    mode = mission_flow.root_variant;
    switch (mode)
    {
        case 0:
            mission_flow.root_kind = 1;
            mission = (sint16)game_runtime.selection.mission;
            if (audio_mission_track_id(mission) == -1)
                break;
            mission_menu_start_item_motion(0, 5);
            mission_menu_start_item_motion(0, 6);
            ui_item_set_motion(0, 7, 0, 0, 506, 0);
            ui_group_set_mask(0, 225);
            ui_animation_set(1, 1, 0, 40);
            sound_play(90);
            break;
        case 1:
            value = game_runtime.progress.flow_flags;
            if (value & 16)
            {
                mission_flow.exit_requested = 1;
                value = game_runtime.progress.flow_flags;
                game_runtime.progress.flow_flags = (uint32)(value | 2);
            }
            else
                mission_flow.root_kind = 2;
            break;
        case 2:
            mission_flow.root_kind = 3;
            break;
        case 3:
            mission_flow.root_kind = 4;
            sound_play(90);
            break;
        default:
            break;
    }
    mission_flow.root_phase = 0;
    return;
}

uint32 mission_frontend_begin_status_screen(void)
{
    uint32 bank, value, delta;
    sint32 mission;

    FUNCTION_MARKER(0x800FBC48, "ACE3.BIN");
    mission_flow.root_kind = 0;
    mission_flow.root_phase = 0;
    bank = game_runtime.display_buffer;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    ui_group_bind(ui_boot_commands, 2);
    ui_group_set_mask(2, 0);
    ui_animation_set(1, 1, 0, 120);
    if (game_runtime.account.mission_failed == 1)
    {
        ui_group_set_mask(2, 64);
        mission_flow.phase_timer = (uint32)(120);
        mission = (sint16)game_runtime.selection.mission;
        delta = (sint16)game_runtime.account.loss_count;
        value = game_runtime.progress.mission_losses[(uint32)mission];
        game_runtime.progress.mission_losses[(uint32)mission] = (uint32)(value + delta);
        delta = (sint16)game_runtime.account.loss_count;
        value = game_runtime.progress.total_losses;
        game_runtime.progress.total_losses = (uint32)(value + delta);
    }
    if (game_runtime.progress.flow_flags & 1)
    {
        ui_group_set_mask(2, 128);
        mission_flow.phase_timer = (uint32)(120);
    }
    return 0x04000000;
}

uint32 mission_frontend_status_tick(void)
{
    uint32 *frame_middle_tag;
    static const uint8 empty_arguments[1] = {0};
    uint32 flags, bank, packet, ordering, tag, link, value;

    uint32 *ordering_entry;
    uint32 *scene_head;
    FUNCTION_MARKER(0x800FBD7C, "ACE3.BIN");
    flags = game_runtime.progress.flow_flags;
    if (flags & 8)
        return 0x04000000;
    value = game_runtime.account.mission_failed;
    if (value == 0 && !(flags & 1))
        return 0x04000000;
    bank = game_runtime.display_buffer;
    packet = ui_packets.scene_bank[bank].ot;
    ui_packets.scene_ot = (uint32)(packet);
    ui_packets.scene_ot_data = ui_packets.scene_bank[bank].ot_data;
    ClearOTagR(ui_scene_ot_span(packet, 64), 16);
    packet = ui_packets.scene_ot;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    scene_head = ui_scene_ot_span(packet, sizeof(*scene_head));
    tag = *scene_head;
    ordering_entry = frame_middle_tag;
    link = *ordering_entry;
    *scene_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    packet = ui_packets.scene_ot + 60;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    ordering_entry = frame_middle_tag;
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    bank = game_runtime.display_buffer;
    ordering = ui_packets.scene_ot;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    text_begin_frame(ordering);
    text_select_font(7);
    text_set_blend(0xFFFFFFFF);
    text_set_reveal_range(1, 1, 1);
    text_set_reveal_step(1);
    text_set_color(24, 184, 208);
    if (game_runtime.account.mission_failed == 1)
    {
        text_set_pos(202, 155);
        text_print((const uint8 *)"CONNECTION LOST", empty_arguments, 0);
    }
    if (game_runtime.progress.flow_flags & 1)
    {
        text_set_pos(233, 145);
        text_print((const uint8 *)"REBOOT", empty_arguments, 0);
    }
    value = game_input.pressed;
    if ((value & 16) && (game_input.held & 47) == 0)
        return 0x04000000;
    value = mission_flow.phase_timer;
    if ((sint32)value <= 0)
        return 0x04000000;
    packet = ui_packets.scene_packet_cursor;
    mission_flow.phase_timer = (uint32)(value - 1);
    value = ui_render_frame(packet);
    ui_packets.scene_packet_cursor = (uint32)(value);
    return 0;
}

uint32 mission_frontend_status_frame(void)
{
    FUNCTION_MARKER(0x800FBD74, "ACE3.BIN");
    return mission_frontend_status_tick();
}

uint32 mission_debrief_wait_assets(void)
{
    FUNCTION_MARKER(0x800FB574, "ACE3.BIN");
    return game_runtime.resource_pending == 0;
}

uint32 mission_debrief_bind_loaded_assets(void)
{
    FUNCTION_MARKER(0x800FB58C, "ACE3.BIN");
    return mission_debrief_load_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
}

uint32 mission_debrief_wait_animation(void)
{
    FUNCTION_MARKER(0x800FBB74, "ACE3.BIN");
    if (ui_animation_phase(1) != 2)
    {
        if (!(game_input.pressed & 16))
            return 0;
        if (game_input.held & 47)
            return 0;
    }
    ui_group_set_mask(2, 0);
    return 2;
}

uint32 mission_frontend_loading_frame(void)
{
    uint32 *frame_middle_tag;
    uint32 bank, value, flags, phase, result, packet, ordering, tag, link;

    uint32 *ordering_entry;
    uint32 *scene_head;
    FUNCTION_MARKER(0x800FBF70, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    game_runtime.account.mission_failed = (uint8)(0);
    flags = game_runtime.progress.flow_flags;
    game_runtime.progress.flow_flags = (uint32)(flags & 0xFFFFFFFE);
    if ((sint16)mission_flow.transition_phase)
    {
        mission_flow.init_mode = 0;
        mission_flow.init_phase = 0;
        return 0x04000000;
    }
    if (flags & 8)
        return 0x04000000;
    if (game_runtime.progress.movie_branch == 1)
        return 0x04000000;
    if (game_runtime.progress.sequence_idx)
        return 0x04000000;
    if ((sint16)game_runtime.selection.mission == 59)
        return 0x04000000;
    phase = mission_flow.root_phase;
    result = mission_dispatch_loading_screen(phase);
    bank = game_runtime.display_buffer;
    packet = ui_packets.scene_bank[bank].ot;
    ui_packets.scene_ot = (uint32)(packet);
    ui_packets.scene_ot_data = ui_packets.scene_bank[bank].ot_data;
    ClearOTagR(ui_scene_ot_span(packet, 64), 16);
    packet = ui_packets.scene_ot;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    scene_head = ui_scene_ot_span(packet, sizeof(*scene_head));
    tag = *scene_head;
    ordering_entry = frame_middle_tag;
    link = *ordering_entry;
    *scene_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    packet = ui_packets.scene_ot + 60;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    ordering_entry = frame_middle_tag;
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    value = ui_draw_frontend_load_progress(ui_packets.scene_packet_cursor);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = ui_render_frame(value);
    ui_packets.scene_packet_cursor = (uint32)(value);
    if (result == 1)
    {
        phase = mission_flow.root_phase;
        mission_flow.root_phase = phase + 1;
        return 0;
    }
    if (result == 2)
    {
        mission_flow.init_mode = 0;
        mission_flow.init_phase = 0;
        mission_flow.transition_phase = 1;
        return 0x04000000;
    }
    return 0;
}

uint32 mission_frontend_begin_debrief(void)
{
    uint32 flags;

    FUNCTION_MARKER(0x800FD634, "ACE3.BIN");
    mission_flow.root_kind = 0;
    mission_flow.root_phase = 0;
    flags = game_runtime.progress.flow_flags;
    if (flags & 32)
        game_runtime.selection.mission = (uint16)(59);
    else if (flags & 8)
    {
        mission_flow.transition_phase = 3;
    }
    return 0x04000000;
}

uint32 mission_frontend_debrief_tick(void)
{
    uint32 phase, result;

    FUNCTION_MARKER(0x800FD6A0, "ACE3.BIN");
    if ((sint16)mission_flow.transition_phase)
    {
        if (!(game_runtime.progress.flow_flags & 8))
            return 0x04000000;
    }
    phase = mission_flow.root_phase;
    result = mission_dispatch_debrief(phase);
    if (result == 0)
        return 0;
    if (result == 1)
    {
        phase = mission_flow.root_phase;
        mission_flow.root_phase = phase + 1;
        return 0;
    }
    if (result == 2)
    {
        mission_flow.init_mode = 0;
        mission_flow.init_phase = 0;
        mission_flow.transition_phase = 1;
        return 0x04000000;
    }
    return 2;
}

uint32 mission_frontend_debrief_frame(void)
{
    FUNCTION_MARKER(0x800FD698, "ACE3.BIN");
    return mission_frontend_debrief_tick();
}

uint32 mission_frontend_initialize_packet_cursor(void)
{
    uint32 bank, value;

    FUNCTION_MARKER(0x800CD7C0, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    return 1;
}

void mission_unlock_archive_for_mission(sint32 mission)
{
    uint32 row, column, value;

    FUNCTION_MARKER(0x800FEDEC, "ACE3.BIN");
    for (row = 0; row < 62; ++row)
    {
        for (column = 0; column < 4; ++column)
        {
            value = archive_unlock_missions[row][column];
            if (value == (uint32)mission)
            {
                value = game_runtime.progress.archive_status[row];
                if (value == 0)
                    game_runtime.progress.archive_status[row] = 1;
            }
        }
    }
    return;
}

uint32 mission_frontend_initialize_state(void)
{
    uint32 bank, value, buffer, index, voice;
    sint32 mission, mode;

    FUNCTION_MARKER(0x800CD7F0, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    mission_flow.frontend_frames = (uint32)(0);
    mission_flow.menu_init_delay = (uint32)(0);
    ui_packets.scene_packet_cursor = (uint32)(value);
    buffer = arena_alloc(256);
    game_runtime.scene_assets.aircraft_headers = NULL;
    game_runtime.scene_assets.aircraft_model_source = (RESOURCE_ASSET_SOURCE){0};
    game_runtime.scene_assets.model_headers = NULL;
    game_runtime.scene_assets.model_header_sources = NULL;
    game_runtime.scene_assets.model_header_count = 0;
    for (index = 0; index < 64; ++index)
    {
        buffer = arena_alloc(28);
    }
    value = game_runtime.progress.account_flags;
    mission = (sint16)game_runtime.selection.mission;
    game_runtime.progress.account_flags = (uint32)(value | 1);
    mission_unlock_archive_for_mission(mission);
    mission_flow.next_scene = 0;
    /* Fix mission-mode-initialization clears the inactive submenu */
    mission_flow.mode = 0;
    mission_flow.mode_phase = 0;
    SetGeomOffset(160, 240);
    SetGeomScreen(250);
    value = sound_play(93);
    *menu_voice_slot(0) = (uint32)(value);
    value = sound_play(94);
    *menu_voice_slot(1) = (uint32)(value);
    value = sound_play(95);
    *menu_voice_slot(2) = (uint32)(value);
    value = sound_play(96);
    *menu_voice_slot(3) = (uint32)(value);
    value = sound_play(97);
    voice = *menu_voice_slot(0);
    *menu_voice_slot(4) = (uint32)(value);
    sound_set_voice_fade(voice, 0, 1);
    for (index = 308; index <= 320; index += 4)
    {
        voice = *menu_voice_slot((index - 304) / 4);
        sound_set_voice_fade(voice, 0, 1);
    }
    mission_flow.root_phase = 0;
    value = game_runtime.progress.movie_branch;
    if (value == 1)
    {
        mission_flow.root_kind = 4;
        mission_flow.root_phase = 2;
    }
    else if (game_runtime.progress.flow_flags & 8)
    {
        mission_flow.root_kind = 1;
        mission_flow.root_phase = 0;
    }
    else
    {
        mode = (sint16)game_runtime.account.route_mode;
        mission_flow.root_kind = mode != 0 && mode != 3;
    }
    mission_flow.root_variant = 4;
    mission_flow.exit_requested = 0;
    mission_flow.playback_requested = (uint32)(0);
    game_runtime.asset_counts.model_entries = (uint32)(12);
    text_select_font(1);
    text_set_blend(-1);
    text_set_reveal_range(1, 1, 1);
    text_set_reveal_step(1);
    mission_flow.phase_timer = (uint32)(0);
    if (!(game_runtime.progress.flow_flags & 8))
    {
        mode = (sint16)game_runtime.account.route_mode;
        if (mode == 0 || mode == 3)
            return 1;
    }
    return 2;
}

uint32 mission_frontend_fade_in_tick(void)
{
    uint32 value, flags;

    FUNCTION_MARKER(0x800CDAF4, "ACE3.BIN");
    value = mission_flow.phase_timer * (uint32)143;
    game_runtime.display.background[0] = (uint8)((sint32)value / 32);
    value = mission_flow.phase_timer * (uint32)135;
    game_runtime.display.background[1] = (uint8)((sint32)value / 32);
    value = mission_flow.phase_timer * (uint32)83;
    game_runtime.display.background[2] = (uint8)((sint32)value / 32);
    flags = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(flags | 8);
    value = mission_flow.phase_timer + 1;
    mission_flow.phase_timer = (uint32)(value);
    if ((sint32)value < 33)
        return 0;
    mission_flow.phase_timer = (uint32)(0);
    return 1;
}

uint32 mission_frontend_open_ring(void)
{
    sint32 mode;

    FUNCTION_MARKER(0x800CDBF8, "ACE3.BIN");
    mode = (sint16)game_runtime.account.route_mode;
    if (mode == 0 || mode == 3)
        mission_effect_open_ring();
    mode = (sint16)game_runtime.account.route_mode;
    if (mode == 2 || (game_runtime.progress.flow_flags & 32))
        mission_effect_open_ring();
    return 1;
}

uint32 mission_frontend_open_ring_frame(void)
{
    FUNCTION_MARKER(0x800CDBF0, "ACE3.BIN");
    return mission_frontend_open_ring();
}

uint32 mission_frontend_wait_open_ring(void)
{
    sint32 mode;

    FUNCTION_MARKER(0x800CDC68, "ACE3.BIN");
    mode = (sint16)game_runtime.account.route_mode;
    if (mode != 0 && mode != 3 && mode != 2 && !(game_runtime.progress.flow_flags & 32))
        return 0;
    return mission_effect.layers[MISSION_EFFECT_RING].mode == 1 ? 2 : 0;
}

uint32 mission_frontend_initialize_step(void)
{
    uint32 phase;
    sint32 result;

    FUNCTION_MARKER(0x800CDCD4, "ACE3.BIN");
    phase = (sint16)mission_flow.init_phase;
    result = mission_dispatch_init(phase);
    if (result == 1)
    {
        phase = mission_flow.init_phase;
        mission_flow.init_phase = phase + 1;
        return 0;
    }
    if (result < 2)
        return 0;
    if (result == 2)
    {
        mission_flow.transition_phase = 0;
    }
    return 0x04000000;
}

uint32 mission_frontend_reset_root_selection(void)
{
    FUNCTION_MARKER(0x800DE718, "ACE3.BIN");
    mission_flow.root_variant = 0;
    mission_flow.phase_timer = (uint32)(0);
    mission_flow.root_phase = 1;
    return 1;
}

uint32 mission_frontend_cancel_root_selection(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800DE73C, "ACE3.BIN");
    value = game_input.pressed & 32;
    if (value == 0)
        return value;
    value = game_input.held & 0x5F;
    if (value)
        return value;
    mission_flow.root_kind = 0;
    mission_flow.root_phase = 0;
    mission_flow.root_variant = 4;
    return 4;
}

uint32 mission_frontend_begin_archive_transition(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800DE4BC, "ACE3.BIN");
    value = mission_flow.root_phase;
    mission_flow.root_phase = value + 1;
    game_runtime.display.background[0] = (uint8)(143);
    game_runtime.display.background[1] = (uint8)(135);
    game_runtime.display.background[2] = (uint8)(83);
    value = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(value | 8);
    mission_effect_set_ring(2);
    mission_flow.transition_phase = 10;
    return 10;
}

void mission_frontend_select_mode(void)
{
    uint32 flags, phase;
    sint32 mode;

    FUNCTION_MARKER(0x800E6658, "ACE3.BIN");
    mission_flow.root_variant = 0;
    mission_flow.root_phase = 1;
    mode = (sint16)game_runtime.account.route_mode;
    if (mode == 0)
        phase = 1;
    else if (mode == 1)
        phase = 5;
    else if (mode == 2)
    {
        flags = game_runtime.progress.flow_flags;
        phase = (flags & 128) ? 7 : ((flags & 64) ? 5 : 7);
    }
    else if (mode == 3)
        phase = 8;
    else
        phase = 7;
    mission_flow.mode = phase;
    flags = game_runtime.progress.flow_flags;
    if (flags & 32)
    {
        mission_flow.mode = 5;
        mission_flow.mode_phase = 0;
    }
    else if (flags & 8)
    {
        mission_flow.mode = 6;
    }
    else if ((sint16)game_runtime.selection.mission == 59)
    {
        mission_flow.mode = 8;
    }
    mission_flow.mode_phase = 0;
    return;
}

uint32 mission_debrief_draw_map(uint32 packet)
{
    uint32 x, y, left, right, delta, top, bottom;
    sint32 sample;
    uint32 ordering, tag, link, second;
    POLY_FT4 *quads;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800F4C04, "ACE3.BIN");
    quads = ui_packet_span(packet, 2 * sizeof(*quads));
    sample = (sint16)mission_debrief.revealed_seconds / 4;
    if (sample < 0)
        sample = 0;
    if (sample >= game_runtime.mission_result.sample_count)
        sample = game_runtime.mission_result.sample_count ? game_runtime.mission_result.sample_count - 1 : 0;
    x = game_runtime.mission_result.trace[sample].x;
    left = 84 - x;
    right = 340 - x;
    y = game_runtime.mission_result.trace[sample].y;
    quads[0].tag = (quads[0].tag & 0xFFFFFF) | ((uint32)(9) << 24);
    quads[0].rgbc.code = (uint8)(44);
    quads[0].p[0].uv.u = (uint8)(0);
    quads[0].p[0].uv.v = (uint8)(0);
    quads[0].p[1].uv.u = (uint8)(128);
    quads[0].p[1].uv.v = (uint8)(0);
    quads[0].p[2].uv.u = (uint8)(0);
    quads[0].p[2].uv.v = (uint8)(64);
    quads[0].p[3].uv.u = (uint8)(128);
    quads[0].p[3].uv.v = (uint8)(64);
    quads[0].rgbc.r = (uint8)(128);
    quads[0].rgbc.g = (uint8)(128);
    quads[0].rgbc.b = (uint8)(128);
    quads[0].p[1].attr = (uint16)(31);
    quads[0].p[0].attr = (uint16)(0x603C);
    quads[0].p[0].xy.x = (sint16)(uint16)(left);
    quads[0].p[1].xy.x = (sint16)(uint16)(right);
    quads[0].p[2].xy.x = (sint16)(uint16)(left);
    quads[0].p[3].xy.x = (sint16)(uint16)(right);
    delta = (255 - y) * 2;
    top = 190 - delta;
    bottom = 446 - delta;
    quads[0].p[0].xy.y = (sint16)(uint16)(top);
    quads[0].p[1].xy.y = (sint16)(uint16)(top);
    quads[0].p[2].xy.y = (sint16)(uint16)(bottom);
    quads[0].p[3].xy.y = (sint16)(uint16)(bottom);
    ordering = ui_packets.scene_ot;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    tag = quads[0].tag;
    link = *ordering_entry;
    quads[0].tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    second = packet + 40;
    quads[1].tag = (quads[1].tag & 0xFFFFFF) | ((uint32)(9) << 24);
    quads[1].rgbc.code = (uint8)(44);
    quads[1].p[0].uv.u = (uint8)(0);
    quads[1].p[0].uv.v = (uint8)(64);
    quads[1].p[1].uv.u = (uint8)(128);
    quads[1].p[1].uv.v = (uint8)(64);
    quads[1].p[2].uv.u = (uint8)(0);
    quads[1].p[2].uv.v = (uint8)(128);
    quads[1].p[3].uv.u = (uint8)(128);
    quads[1].p[3].uv.v = (uint8)(128);
    quads[1].rgbc.r = (uint8)(128);
    quads[1].rgbc.g = (uint8)(128);
    quads[1].rgbc.b = (uint8)(128);
    quads[1].p[1].attr = (uint16)(31);
    quads[1].p[0].attr = (uint16)(0x603C);
    quads[1].p[0].xy.x = (sint16)(uint16)(left);
    quads[1].p[0].xy.y = (sint16)(uint16)(bottom);
    quads[1].p[1].xy.x = (sint16)(uint16)(right);
    quads[1].p[1].xy.y = (sint16)(uint16)(bottom);
    quads[1].p[2].xy.x = (sint16)(uint16)(left);
    quads[1].p[2].xy.y = (sint16)(uint16)(702 - delta);
    quads[1].p[3].xy.x = (sint16)(uint16)(right);
    quads[1].p[3].xy.y = (sint16)(uint16)(702 - delta);
    ordering = ui_packets.scene_ot;
    tag = quads[1].tag;
    link = *ordering_entry;
    quads[1].tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (second & 0xFFFFFF);
    return second + 40;
}

uint32 mission_debrief_draw_markers(uint32 packet)
{
    uint32 origin_x, origin_y, i, x, y, ordering, type;
    const MISSION_DEBRIEF_MARKER *item;
    sint32 sample;
    sint32 color_initialized = 0;
    MENU_DRAW_MODE mode;
    MENU_LINE_DESCRIPTOR rectangle;
    FUNCTION_MARKER(0x800F5388, "ACE3.BIN");
    sample = (sint16)mission_debrief.revealed_seconds / 4;
    if (sample < 0)
        sample = 0;
    if (sample >= game_runtime.mission_result.sample_count)
        sample = game_runtime.mission_result.sample_count ? game_runtime.mission_result.sample_count - 1 : 0;
    origin_x = game_runtime.mission_result.trace[sample].x;
    origin_y = game_runtime.mission_result.trace[sample].y;
    for (i = 0; i < 64; ++i)
    {
        item = &mission_debrief.markers[i];
        if (item->visible == 0)
            return packet;
        x = item->x;
        y = item->y;
        mode = (MENU_DRAW_MODE){0};
        x -= origin_x;
        ordering = ui_packets.scene_ot;
        y = (origin_y - y) * 2;
        packet = ui_emit_configured_draw_mode(&mode, packet, ordering);
        type = item->category;
        if (type == 0)
        {
            rectangle.color[0] = 255;
            rectangle.color[1] = 255;
            rectangle.color[2] = 255;
            color_initialized = 1;
        }
        type = item->category;
        if (type == 1)
        {
            rectangle.color[0] = 0;
            rectangle.color[1] = 0;
            rectangle.color[2] = 255;
            color_initialized = 1;
        }
        type = item->category;
        if (type == 2)
        {
            rectangle.color[0] = 0;
            rectangle.color[1] = 0;
            rectangle.color[2] = 0;
            color_initialized = 1;
            type = item->category;
        }
        if (type == 3)
        {
            rectangle.color[0] = 255;
            rectangle.color[1] = 255;
            rectangle.color[2] = 0;
            color_initialized = 1;
        }
        type = item->category;
        if (type == 4)
        {
            rectangle.color[0] = 255;
            rectangle.color[1] = 0;
            rectangle.color[2] = 0;
            color_initialized = 1;
        }
        if (!color_initialized)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource2 graph color has no initialized original RGB: %u\n", type);
#endif
            abort();
        }
        rectangle.semitransparent = 1;
        rectangle.point[0][0] = (uint16)(x + 80);
        rectangle.point[0][1] = (uint16)(y + 184);
        rectangle.point[1][0] = (uint16)(x + 88);
        rectangle.point[1][1] = (uint16)(y + 200);
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        rectangle.point[0][0] = (uint16)(x + 78);
        rectangle.point[1][0] = (uint16)(x + 86);
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        rectangle.point[0][0] = (uint16)(x + 79);
        rectangle.point[1][0] = (uint16)(x + 87);
        rectangle.color[0] *= 2;
        rectangle.color[1] *= 2;
        rectangle.color[2] *= 2;
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        rectangle.semitransparent = 1;
        rectangle.point[0][0] = (uint16)(x + 80);
        rectangle.point[0][1] = (uint16)(y + 200);
        rectangle.point[1][0] = (uint16)(x + 88);
        rectangle.point[1][1] = (uint16)(y + 184);
        rectangle.color[0] >>= 1;
        rectangle.color[1] >>= 1;
        rectangle.color[2] >>= 1;
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        rectangle.point[0][0] = (uint16)(x + 78);
        rectangle.point[1][0] = (uint16)(x + 86);
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        rectangle.semitransparent = 1;
        rectangle.point[0][0] = (uint16)(x + 79);
        rectangle.point[1][0] = (uint16)(x + 87);
        rectangle.color[0] *= 2;
        rectangle.color[1] *= 2;
        rectangle.color[2] *= 2;
        ordering = ui_packets.scene_ot;
        packet = ui_emit_line(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
        mode.blend = 0;
        ordering = ui_packets.scene_ot;
        packet = ui_emit_configured_draw_mode(&mode, packet, ordering);
    }
    return packet;
}

void mission_debrief_draw_counts(void)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    const uint8 empty_arguments[1] = {0};
    uint8 arguments[4];
    uint32 i, count, label_y, number_y;
    FUNCTION_MARKER(0x800F56D8, "ACE3.BIN");
    label_y = 54;
    number_y = 59;
    for (i = 0; i < 8; ++i)
    {
        if (mission_debrief.kind_rows[i].count == 0)
            break;
        text_set_pos(178, label_y);
        label_y += 22;
        text_print((const uint8 *)mission_debrief.kind_rows[i].label, empty_arguments, 0);
        text_set_pos(270, number_y);
        number_y += 22;
        count = mission_debrief.kind_rows[i].count;
        (wire_word_0 = (count), (void)memcpy(arguments, &wire_word_0, sizeof(wire_word_0)));
        text_print_digits((const uint8 *)"%2d\n", arguments, sizeof(arguments));
    }
    label_y = 216;
    number_y = 223;
    for (i = 0; i < 8; ++i)
    {
        if (mission_debrief.special_rows[i].count == 0)
            break;
        text_set_pos(178, label_y);
        label_y += 22;
        text_print((const uint8 *)mission_debrief.special_rows[i].label, empty_arguments, 0);
        text_set_pos(270, number_y);
        number_y += 22;
        count = mission_debrief.special_rows[i].count;
        (wire_word_1 = (count), (void)memcpy(arguments, &wire_word_1, sizeof(wire_word_1)));
        text_print_digits((const uint8 *)"%2d\n", arguments, sizeof(arguments));
    }
    return;
}

void mission_debrief_draw_time(void)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint8 arguments[12];
    uint32 enabled, time, fraction;
    sint32 elapsed, random_value, minutes;
    FUNCTION_MARKER(0x800F5844, "ACE3.BIN");
    enabled = mission_debrief.phase;
    time = game_runtime.mission_result.elapsed_ticks;
    if (enabled)
    {
        elapsed = (sint16)mission_debrief.revealed_seconds;
        if (elapsed < (sint32)(time / 30))
        {
            random_value = game_random();
            fraction = (random_value % 30 + 30);
        }
        else
            fraction = (time % 30) * 100 / 30;
    }
    else
        fraction = 0;
    text_set_pos(30, 339);
    elapsed = (sint16)mission_debrief.revealed_seconds;
    minutes = elapsed / 60;
    (wire_word_0 = ((sint16)((sint16)minutes % 60)), (void)memcpy(arguments, &wire_word_0, sizeof(wire_word_0)));
    (wire_word_1 = ((sint16)(elapsed - minutes * 60)), (void)memcpy(arguments + 4, &wire_word_1, sizeof(wire_word_1)));
    (wire_word_2 = (fraction & 0xFF), (void)memcpy(arguments + 8, &wire_word_2, sizeof(wire_word_2)));
    {
        (void)(text_print_digits((const uint8 *)"%2d:%2d@%2d", arguments, sizeof(arguments)));
        return;
    }
}

void mission_debrief_draw_time_frame(void)
{
    FUNCTION_MARKER(0x800F5834, "ACE3.BIN");
    {
        (void)(mission_debrief_draw_time());
        return;
    }
}

static DEBRIEF_VARIANT mission_debrief_variant(sint32 mission, uint32 kind)
{
    uint32 index;
    if (mission < 0 || mission >= GAME_MISSION_COUNT || kind > UINT8_MAX)
        abort();
    index = (uint32)mission * 4 + kind;
    if (index < sizeof(debrief_variants) / sizeof(*debrief_variants))
        return debrief_variants[index];

    index -= sizeof(debrief_variants) / sizeof(*debrief_variants);
    if (index < audio_tracks_debrief.count * 6u)
    {
        if (index % 6u != 0)
            return (DEBRIEF_VARIANT){0, 0};

        uint16 id = (uint16)audio_tracks_debrief.records[index / 6u].id;
        return (DEBRIEF_VARIANT){(uint8)id, (uint8)(id >> 8)};
    }

    index -= audio_tracks_debrief.count * 6u;
    if (index < sizeof(debrief_legacy_dispatch) / sizeof(*debrief_legacy_dispatch))
        return debrief_legacy_dispatch[index];

    abort();
}

uint32 mission_debrief_frame(void)
{
    uint32 *frame_middle_tag;
    const uint8 empty_arguments[1] = {0};
    const MISSION_BUNDLE_DIRECTORY *directory = &mission_flow.assets.mission_directory;
    uint32 bank, packet, ordering, tag, link, value, flags;
    uint32 result, available, phase, kind;
    sint32 mode, mission;
    uint32 *ordering_entry;
    uint32 *scene_head;
    FUNCTION_MARKER(0x800F6848, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    packet = ui_packets.scene_bank[bank].ot;
    ui_packets.scene_ot = (uint32)(packet);
    ui_packets.scene_ot_data = ui_packets.scene_bank[bank].ot_data;
    ClearOTagR(ui_scene_ot_span(packet, 64), 16);
    packet = ui_packets.scene_ot;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    scene_head = ui_scene_ot_span(packet, sizeof(*scene_head));
    tag = *scene_head;
    ordering_entry = frame_middle_tag;
    link = *ordering_entry;
    *scene_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    packet = ui_packets.scene_ot + 60;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    frame_middle_tag = game_runtime.frame_packets.middle_tag;
    ordering_entry = frame_middle_tag;
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    bank = game_runtime.display_buffer;
    value = ui_packets.scene_bank[bank].packets;
    ui_packets.scene_packet_begin = value;
    ui_packets.scene_packet_data = ui_packets.scene_bank[bank].packet_data;
    ui_packets.scene_packet_cursor = (uint32)(value);
    flags = game_runtime.progress.flow_flags;
    if (flags & 1)
        return 0x04000000;
    if (flags & 8)
        return 0x04000000;
    if (game_runtime.progress.movie_branch == 1)
        return 0x04000000;
    mode = (sint16)game_runtime.account.route_mode;
    if (mode != 1 && mode != 3)
        return 0x04000000;
    if (game_runtime.account.mission_failed == 1)
        return 0x04000000;
    text_set_color(5, 50, 53);
    text_begin_frame(ui_packets.scene_ot);
    mode = (sint16)mission_flow.transition_phase;
    result = mission_dispatch_debrief((uint32)mode);
    mission = (sint16)game_runtime.selection.mission;
    kind = game_runtime.account.result_kind;
    available = mission_debrief_variant(mission, kind).track;
    if (result - 1 < 2)
    {
        phase = mission_flow.transition_phase + 1;
        mission_flow.transition_phase = phase;
        if ((sint16)phase == 7 && available)
        {
            mission = (sint16)game_runtime.selection.mission;
            kind = game_runtime.account.result_kind;
            available = mission_debrief_variant(mission, kind).track;
            mission_audio_control_track(1, available);
            mission_flow.debrief_movie_phase = 1;
            mission = (sint16)game_runtime.selection.mission;
            kind = game_runtime.account.result_kind;
            if (mission_debrief_variant(mission, kind).graphics == 0)
            {
                scene_texture_load_mission_images(directory->result_textures[0], directory);
            }
            mission = (sint16)game_runtime.selection.mission;
            kind = game_runtime.account.result_kind;
            if (mission_debrief_variant(mission, kind).graphics == 1)
            {
                scene_texture_load_mission_images(directory->result_textures[1], directory);
            }
        }
    }
    value = mission_flow.debrief_movie_phase;
    if (value == 1)
    {
        if (resource_get_stream_elapsed())
        {
            subtitles_configure(3, 0, 0);
            mission_flow.debrief_movie_phase = 2;
        }
    }
    else if (value == 2)
    {
        ordering = ui_packets.scene_ot;
        packet = ui_packets.scene_packet_cursor;
        value = subtitles_draw(ordering, packet);
        ui_packets.scene_packet_cursor = (uint32)(value);
    }
    value = ui_render_frame(ui_packets.scene_packet_cursor);
    ui_packets.scene_packet_cursor = (uint32)(value);
    mission_debrief_reveal_events_frame();
    mission_debrief_draw_counts();
    mission_debrief_draw_time_frame();
    value = ui_draw_debrief_area(ui_packets.scene_packet_cursor, 0);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = mission_debrief_draw_markers(value);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = mission_debrief_draw_trace(value);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = mission_debrief_draw_map(value);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = ui_draw_debrief_area(value, 1);
    ui_packets.scene_packet_cursor = (uint32)(value);
    text_set_pos(20, 41);
    text_print((const uint8 *)"DEBRIEFING", empty_arguments, 0);
    text_set_pos(139, 343);
    text_print((const uint8 *)"PERFORMANCE RATING", empty_arguments, 0);
    mission_debrief_tick_phase();
    if (game_input.pressed && (game_input.held & 15) == 0)
    {
        if ((sint16)mission_flow.transition_phase == 7)
        {
            ui_group_set_mask(2, 0);
            ui_animation_set(1, 0, 0, 0);
            resource_init_queue(0, 0);
            return 0x04000000;
        }
    }
    mission_debrief_reveal_trace();
    return 0;
}

uint32 mission_bind_loaded_assets(uint32 loaded, const uint8 *data, uint32 data_bytes)
{
    MISSION_HEIGHT_MAP *map = &mission_briefing_height_map;
    MISSION_BUNDLE_DIRECTORY *directory = &mission_flow.assets.mission_directory;
    const uint8 *dialogue_data = NULL, *dialogue_tail;
    uint32 dialogue_bytes = 0;
    RESOURCE_ASSET_SOURCE dialogue_source;
    uint32 sections[9], dialogue[5];
    uint32 value, kind, type, resource, map_begin, map_end, event_bytes;
    sint32 mission;

    FUNCTION_MARKER(0x800FB7FC, "ACE3.BIN");
    mission_image_sections(loaded, data, data_bytes, sections, 9);
    *directory = (MISSION_BUNDLE_DIRECTORY){
        sections[0], sections[1], sections[2], sections[3], {sections[4], sections[5]}, sections[6], sections[7], sections[8], data, data_bytes, loaded,
    };
    value = directory->briefing_events;
    if (value < directory->dialogue || directory->briefing_textures < value || directory->briefing_textures > directory->map_textures)
        abort();
    event_bytes = directory->briefing_textures - value;
    if (event_bytes != 4 && event_bytes % sizeof(MISSION_BRIEFING_EVENT) != 0)
        abort();
    if (!data || value < loaded || value - loaded > data_bytes || event_bytes > data_bytes - (value - loaded))
        abort();
    mission_flow.assets.mission_events = (const MISSION_BRIEFING_EVENT *)(data + (value - loaded));
    mission_flow.assets.mission_events_end = mission_flow.assets.mission_events + event_bytes / sizeof(MISSION_BRIEFING_EVENT);
    resource = directory->map_textures;
    map_begin = directory->height_map;
    map_end = directory->dialogue;
    if (map_begin < loaded || map_begin - loaded < 40 || map_end <= map_begin || map_end > resource || resource > arena_cursor())
        abort();
    map->bytes = map_end - map_begin;
    if (!data || map_begin - loaded > data_bytes || map->bytes > data_bytes - (map_begin - loaded))
        abort();
    map->data = data + (map_begin - loaded);
    scene_texture_load_group(resource, (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    scene_texture_load_group(directory->interface_textures, (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    mission = (sint16)game_runtime.selection.mission;
    kind = game_runtime.account.result_kind;
    type = mission_debrief_variant(mission, kind).graphics;
    if (type == 0 || type == 1)
    {
        resource = directory->result_textures[type];
        MISSION_IMAGE_DIRECTORY textures = mission_image_directory_bind(resource, sections[5 + type], loaded, data, data_bytes);
        scene_texture_load_image(mission_image_directory_at(&textures, 0), (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded}, SCENE_TEXTURE_STANDARD);
    }
    arena_set_cursor(directory->map_textures);
    value = directory->dialogue;
    if (!data || value < loaded || value - loaded > data_bytes || 6 * sizeof(uint32) > data_bytes - (value - loaded))
        abort();
    dialogue_data = data + (value - loaded);
    dialogue_bytes = data_bytes - (value - loaded);
    mission_image_sections(directory->dialogue, dialogue_data, dialogue_bytes, dialogue, 5);
    value = directory->dialogue + 24;
    if (value < loaded || value - loaded > data_bytes || (256 - 4) * sizeof(uint32) > data_bytes - (value - loaded))
        abort();
    dialogue_tail = data + (value - loaded);
    dialogue_source = (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded};
    mission_flow.assets.dialogue_sequences = (MISSION_DIALOGUE_DIRECTORY){
        mission_dialogue_source(dialogue[0], dialogue_source),
        {mission_dialogue_source(dialogue[1], dialogue_source), mission_dialogue_source(dialogue[2], dialogue_source), mission_dialogue_source(dialogue[3], dialogue_source), mission_dialogue_source(dialogue[4], dialogue_source)},
        dialogue_tail,
        dialogue_source,
    };
    return 1;
}

uint32 mission_frontend_bind_mission_assets(void)
{
    FUNCTION_MARKER(0x800FB7F4, "ACE3.BIN");
    return mission_bind_loaded_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
}

uint32 frontend_bind_loaded_assets(uint32 loaded, const uint8 *data, uint32 data_bytes)
{
    uint32 sections[9], buffer;

    FUNCTION_MARKER(0x800FBA38, "ACE3.BIN");
    mission_image_sections(loaded, data, data_bytes, sections, 9);
    const COMMON_MENU_DIRECTORY directory = {
        sections[0], sections[1], sections[2], sections[3], sections[4], sections[5], sections[6], sections[7], sections[8],
    };
    sound_load_bank((SOUND_BANK_INPUT){data, data_bytes, directory.sound_bank - loaded});
    scene_texture_load_group(directory.initial_textures, (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded});
    arena_set_cursor(directory.initial_textures);
    mission_flow.assets.save_screen_assets = directory.save_textures;
    buffer = arena_alloc(128);
    mission_flow.assets.menu_work_buffer = (uint32)(buffer);
    (void)arena_alloc(sizeof(mission_flow.assets.pad_profile_images));
    if (directory.pad_profile_directory < loaded || directory.pad_profile_directory - loaded > data_bytes || 5 * sizeof(uint32) > data_bytes - (directory.pad_profile_directory - loaded))
        abort();
    const uint32 *image_directory = (const uint32 *)(data + (directory.pad_profile_directory - loaded));
    mission_image_relocate_entries(image_directory, directory.pad_profile_directory, mission_flow.assets.pad_profile_images, 4);
    mission_flow.assets.frontend_source = (RESOURCE_ASSET_SOURCE){data, data_bytes, loaded};
    mission_flow.assets.save_screen_ui = ui_command_directory(data, data_bytes, directory.save_commands - loaded, directory.pad_profile_directory - loaded);
    ui_group_bind(ui_command_directory(data, data_bytes, directory.menu_commands - loaded, directory.save_commands - loaded), 0);
#if 0
    menu_load_card_header_assets(data, data_bytes, directory.card_icons - loaded, directory.card_palette - loaded);
#else
    /* preserve subsequent arena offsets until their address dependencies are retired */
    (void)arena_alloc(GAME_CARD_ICON_BLOCK_BYTES);
#endif
    mission_flow.assets.shared_menu_textures = directory.shared_textures;
    mission_effect_init();
    return 1;
}

uint32 mission_frontend_bind_common_assets(void)
{
    FUNCTION_MARKER(0x800FBA30, "ACE3.BIN");
    return frontend_bind_loaded_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
}

uint32 mission_frontend_begin_asset_fade(void)
{
    FUNCTION_MARKER(0x800FBBE0, "ACE3.BIN");
    ui_animation_set(1, 1, 0, 10);
    return 2;
}

uint32 mission_frontend_wait_asset_fade(void)
{
    FUNCTION_MARKER(0x800FBC0C, "ACE3.BIN");
    return ui_animation_phase(1) == 2 ? 3 : 0;
}

uint32 mission_image_relocate_entries(const uint32 *image_directory, uint32 base, uint32 *native_destination, uint32 native_capacity)
{
    uint32 offset, index;
    sint32 count;

    FUNCTION_MARKER(0x800ECD4C, "ACE3.BIN");
    count = (sint32)image_directory[0];
    if (count > 0 && (uint32)count > native_capacity)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 relocation output exceeds native capacity\n");
#endif
        abort();
    }
    for (index = 0; (sint32)index < count; ++index)
    {
        offset = image_directory[index + 1];
        if ((sint32)offset < 0)
            offset += 3;
        offset = (uint32)((sint32)offset >> 2) << 2;
        native_destination[index] = base + offset;
    }
    return (uint32)count;
}

void mission_debrief_reveal_events(void)
{
    uint32 index, count, kind, category, value;
    MISSION_DEBRIEF_MARKER *destination;
    const GAME_MISSION_EVENT *record;
    uint32 limit, slot;
    MISSION_DEBRIEF_ROW *cursor;
    sint32 time, event_time;

    FUNCTION_MARKER(0x800F5028, "ACE3.BIN");
    index = mission_debrief.event_idx;
    count = game_runtime.mission_result.event_count;
    if (index >= count)
        return;
    index = mission_debrief.event_idx;
    record = &game_runtime.mission_result.events[index];
    kind = record->kind;
    destination = &mission_debrief.markers[index];
    category = kind < 48 ? 1 : ((kind - 112 < 5) ? 2 : 0);
    event_time = (sint16)record->seconds;
    time = (sint16)mission_debrief.revealed_seconds;
    if (time < event_time)
        return;
    for (;;)
    {
        index = mission_debrief.event_idx;
        count = game_runtime.mission_result.event_count;
        if (index >= count)
            return;
        destination->visible = (uint8)(1);
        value = record->x;
        destination->x = (uint8)(value);
        value = record->y;
        destination->y = (uint8)(value);
        value = record->category;
        destination->category = (uint8)(value);
        value = mission_debrief.event_idx;
        mission_debrief.event_idx = value + 1;
        if (category == 1)
        {
            limit = mission_debrief.kind_count;
            slot = 0;
            cursor = mission_debrief.kind_rows;
            if (limit)
            {
                kind = record->kind;
                while (slot < limit)
                {
                    value = cursor->kind;
                    if (value == kind)
                        break;
                    ++slot;
                    ++cursor;
                }
            }
            if (slot < limit)
            {
                value = cursor->count;
                cursor->count = (uint8)(value + 1);
            }
            else
            {
                value = record->kind;
                mission_debrief.kind_rows[slot].kind = (uint8)(value);
                kind = record->kind;
                strcpy(mission_debrief.kind_rows[slot].label, mission_debrief_labels[kind]);
                mission_debrief.kind_rows[slot].count = (uint8)(1);
                value = mission_debrief.kind_count;
                mission_debrief.kind_count = (uint8)(value + 1);
            }
            sound_play(46);
        }
        else if (category == 2)
        {
            limit = mission_debrief.special_count;
            slot = 0;
            cursor = mission_debrief.special_rows;
            if (limit)
            {
                kind = record->kind;
                while (slot < limit)
                {
                    value = cursor->kind;
                    if (value == kind)
                        break;
                    ++slot;
                    ++cursor;
                }
            }
            if (slot < limit)
            {
                value = cursor->count;
                cursor->count = (uint8)(value + 1);
            }
            else
            {
                value = record->kind;
                mission_debrief.special_rows[slot].kind = (uint8)(value);
                kind = record->kind;
                strcpy(mission_debrief.special_rows[slot].label, mission_debrief_labels[48 + kind - 112]);
                mission_debrief.special_rows[slot].count = (uint8)(1);
                value = mission_debrief.special_count;
                mission_debrief.special_count = (uint8)(value + 1);
            }
            sound_play(47);
        }
        index = mission_debrief.event_idx;
        count = game_runtime.mission_result.event_count;
        if (index == count)
            return;
        record = &game_runtime.mission_result.events[index];
        kind = record->kind;
        destination = &mission_debrief.markers[index];
        category = kind < 48 ? 1 : ((kind - 112 < 5) ? 2 : 0);
        event_time = (sint16)record->seconds;
        time = (sint16)mission_debrief.revealed_seconds;
        if (time < event_time)
            return;
    }
}

void mission_debrief_reveal_events_frame(void)
{
    FUNCTION_MARKER(0x800F5018, "ACE3.BIN");
    {
        (void)(mission_debrief_reveal_events());
        return;
    }
}

static uint32 mission_asset_unpack_sections(uint8 *directory, uint32 header, uint32 sections, uint32 output_start, uint32 base, const uint8 *data, uint32 data_bytes, ASSET_SECTION_VIEW *views, RESOURCE_STREAM_BUFFER backing)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 section_offsets[11];
    uint32 wire_word_3;
    uint32 bytes = (memcpy(&wire_word_0, directory + sections * 4, sizeof(wire_word_0)), wire_word_0) - header;
    uint32 cursor = header + output_start;
    uint8 *compressed, *output;
    const uint8 *input;
    uint32 index, offset, decoded_bytes;
    size_t output_offset;

    if (backing.buffer.data != data || backing.encoded_begin != base)
        abort();
    if (bytes < sections * 4)
        abort();
    compressed = malloc(bytes);
    if (!compressed)
        abort();
    if (!data || header < base || header - base > data_bytes || bytes > data_bytes - (header - base))
        abort();
    input = data + (header - base);
    memcpy(compressed, input, bytes);
    for (index = 0; index < sections; ++index)
    {
        memcpy(&section_offsets[index], compressed + index * 4, sizeof(section_offsets[index]));
        section_offsets[index] -= header;
    }
    (wire_word_1 = (cursor), (void)memcpy(directory, &wire_word_1, sizeof(wire_word_1)));
    for (index = 0; index < sections; ++index)
    {
        offset = section_offsets[index];
        if (offset > bytes || bytes - offset < 16)
            abort();
        if (cursor < 0x80000000u || cursor >= 0x80000000u + PSX_DRAM_SIZE)
            abort();
        if (cursor < base || cursor - base >= backing.buffer.bytes)
            abort();
        output_offset = cursor - base;
        output = backing.buffer.data + output_offset;
        decoded_bytes = (uint32)game_decompress_buffer(compressed + offset, bytes - offset, output, backing.buffer.bytes - output_offset);
        views[index].data = output;
        views[index].bytes = decoded_bytes;
        cursor += decoded_bytes;
        (wire_word_3 = (cursor), (void)memcpy(directory + (index + 1) * 4, &wire_word_3, sizeof(wire_word_3)));
    }
    free(compressed);
    return cursor;
}

static void mission_asset_bind_terrain_capture(const uint8 *data, uint32 bytes)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    MISSION_TERRAIN_CAPTURE *capture = &game_runtime.terrain_capture;
    uint32 count, i;

    capture->count = 0;
    if (bytes < 120)
        return;
    count = (memcpy(&wire_word_0, data + 116, sizeof(wire_word_0)), wire_word_0);
    if (count > MISSION_TERRAIN_CAPTURE_CAPACITY || count > (bytes - 120) / 12)
        return;
    capture->count = count;
    data += 120;
    for (i = 0; i < count; ++i, data += 12)
    {
        MISSION_TERRAIN_CAPTURE_RECORD *record = &capture->records[i];
        record->texture = (memcpy(&wire_halfword_1, data, sizeof(wire_halfword_1)), wire_halfword_1);
        record->shift = (sint16)(memcpy(&wire_halfword_2, data + 2, sizeof(wire_halfword_2)), wire_halfword_2);
        record->phase = (sint16)(memcpy(&wire_halfword_3, data + 4, sizeof(wire_halfword_3)), wire_halfword_3);
        record->red = data[8];
        record->green = data[9];
        record->blue = data[10];
    }
}

static void mission_asset_bind_render_settings(const uint8 *data)
{
    MISSION_RENDER_RECORD record;
    MISSION_RENDER_SETTINGS *settings = &game_runtime.render_settings;

    memcpy(&record, data, 116);

    settings->near_plane = (sint16)record.near_plane;
    settings->far_plane = (sint16)record.far_plane;
    settings->light_angles[0][0] = (sint16)record.first_light_angles[0];
    settings->light_angles[0][1] = (sint16)record.first_light_angles[1];
    settings->light_angles[1][0] = (sint16)record.second_light_angles[0];
    settings->light_angles[1][1] = (sint16)record.second_light_angles[1];
    settings->flare_flags = (sint16)record.flare_flags;
    settings->layer_enabled = record.layer_enabled;
    settings->detail_kind = record.detail_kind;
    settings->halo_color = record.halo_color;
    settings->terrain_flags = record.terrain_flags;
    settings->horizon_height = (sint16)record.horizon_height;
    settings->layer_height[0] = (sint16)record.layer_height[0];
    settings->layer_height[1] = (sint16)record.layer_height[1];
    settings->palette_secondary = record.palette_secondary;
    settings->palette_primary = record.palette_primary;
    settings->blend_start[0] = record.first_blend_start;
    settings->blend_end[0] = record.first_blend_end;
    settings->horizon_color = record.horizon_color;
    settings->blend_start[1] = record.second_blend_start;
    settings->blend_end[1] = record.second_blend_end;
    settings->light_colors[0][0] = (sint16)record.first_light_colors[0];
    settings->light_colors[0][1] = (sint16)record.first_light_colors[1];
    settings->light_colors[0][2] = (sint16)record.first_light_colors[2];
    settings->light_colors[1][0] = (sint16)record.second_light_colors[0];
    settings->light_colors[1][1] = (sint16)record.second_light_colors[1];
    settings->light_colors[1][2] = (sint16)record.second_light_colors[2];
    settings->ambient_color[0] = (sint16)record.ambient_color[0];
    settings->ambient_color[1] = (sint16)record.ambient_color[1];
    settings->ambient_color[2] = (sint16)record.ambient_color[2];
}

uint32 mission_unpack_assets(void)
{
    uint32 wire_word_4;
    uint32 wire_word_6;
    uint32 wire_word_28;
    uint32 wire_word_30;
    uint32 wire_word_32;
    uint32 wire_word_34;
    uint32 wire_word_36;
    uint32 wire_word_38;
    uint8 *asset_data;
    RESOURCE_STREAM_BUFFER backing;
    RESOURCE_ASSET_SOURCE model_parent;
    ASSET_SECTION_VIEW decoded[11];
    uint32 asset_base, asset_bytes, initialized_bytes, texture_bytes, texture_offset;
    GAME_SCENE_ASSETS *assets = &game_runtime.resource_context.mission_assets;
    const uint32 *extensions, *animated_models, *terrain_directory;
    uint32 terrain_entries;
    uint32 *models;
    uint32 header, texture_begin, texture_end, directory_value, cursor;
    uint32 render_begin;
    uint32 i, offset, target, model_base, model_header_bytes;
    sint32 count;
    uint32 restore, restore_bytes, terrain_count, render_bytes;
    const uint8 *render_data;
    TERRAIN_MODEL_TABLE *terrain = &game_runtime.resource_context.mission_assets.terrain_models;
    FUNCTION_MARKER(0x80035A38, "ACE3.BIN");
    asset_base = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    backing = game_runtime.resource_slot.backing;
    if (!asset_data || asset_bytes < 4 + sizeof(MISSION_ASSET_DIRECTORY))
        abort();
    header = resource_relocate_directory(asset_base, 0, asset_data, asset_bytes);
    MISSION_ASSET_DIRECTORY *directory = (MISSION_ASSET_DIRECTORY *)(asset_data + 4);
    sound_load_bank((SOUND_BANK_INPUT){asset_data, asset_bytes, directory->sound_bank - asset_base});
    initialized_bytes = asset_bytes;
    for (offset = 60; offset >= 44; offset -= 4)
    {
        texture_begin = directory->texture_groups[(offset - 44) / 4];
        texture_end = (offset == 60 ? directory->sound_bank : directory->texture_groups[(offset - 44) / 4 + 1]);
        texture_bytes = scene_texture_unpack_tim_group(texture_begin, texture_end, (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, asset_base}, backing);
        texture_offset = texture_end - asset_base;
        if (texture_end < asset_base || texture_offset > asset_bytes || texture_bytes > UINT32_MAX - texture_offset)
            abort();
        if (initialized_bytes < texture_offset + texture_bytes)
            initialized_bytes = texture_offset + texture_bytes;
        DrawSync(0);
    }
    cursor = mission_asset_unpack_sections((uint8 *)directory, header, 11, 68, asset_base, asset_data, asset_bytes, decoded, backing);
    if (cursor < asset_base)
        abort();
    if (initialized_bytes < cursor - asset_base)
        initialized_bytes = cursor - asset_base;
    if (initialized_bytes > backing.buffer.bytes)
        abort();
    model_parent = (RESOURCE_ASSET_SOURCE){asset_data, initialized_bytes, asset_base};
    if (decoded[3].bytes < 4 || decoded[5].bytes < 4)
        abort();
    directory_value = (memcpy(&wire_word_4, decoded[3].data, sizeof(wire_word_4)), wire_word_4);
    game_runtime.asset_counts.model_entries = (uint32)(directory_value);
    directory_value = (memcpy(&wire_word_6, decoded[5].data, sizeof(wire_word_6)), wire_word_6);
    game_runtime.asset_counts.animated_model_entries = (uint32)(directory_value);
    if (decoded[1].bytes < 256u * 256u * sizeof(uint16))
        abort();
    game_runtime.resource_context.mission_assets.tile_map = (uint16 *)decoded[1].data;
    game_runtime.resource_context.mission_assets.entities.legacy_base = (uint32)(directory->entities);
    game_runtime.resource_context.mission_assets.entities.bytes = directory->environment - game_runtime.resource_context.mission_assets.entities.legacy_base;
    game_runtime.resource_context.mission_assets.entities.data = decoded[8].data;
    game_runtime.resource_context.mission_assets.normals.legacy_base = (uint32)(directory->normals);
    game_runtime.resource_context.mission_assets.normals.data = decoded[6].data;
    if (cursor < directory->normals)
        abort();
    game_runtime.resource_context.mission_assets.normals.bytes = model_parent.bytes - (directory->normals - model_parent.encoded_base);
    game_runtime.resource_context.mission_assets.scripts.data = decoded[7].data;
    game_runtime.resource_context.mission_assets.scripts.bytes = decoded[7].bytes;
    game_runtime.resource_context.mission_assets.scripts.legacy_base = directory->speech;
    directory_value = resource_relocate_directory(directory->terrain_models, &terrain_count, decoded[2].data, decoded[2].bytes);
    terrain->guest_base = directory_value - 4;
    terrain_entries = terrain_count + 1;
    terrain_directory = (const uint32 *)decoded[2].data;
    for (i = 0; i < sizeof(game_runtime.resource_context.terrain_header_storage) / sizeof(game_runtime.resource_context.terrain_header_storage[0]); ++i)
        game_runtime.resource_context.terrain_header_storage[i] = NULL;
    terrain->headers = game_runtime.resource_context.terrain_header_storage;
    terrain->entry_count = terrain_entries < GAME_TERRAIN_MODEL_HEADER_CAPACITY ? terrain_entries : GAME_TERRAIN_MODEL_HEADER_CAPACITY;
    terrain->data = decoded[2].data;
    terrain->bytes = decoded[2].bytes;
    directory_value = resource_relocate_directory(directory->models, 0, decoded[3].data, decoded[3].bytes);
    models = (uint32 *)(decoded[3].data + 4);
    if (decoded[3].bytes < 4 + 128 * sizeof(uint32))
        abort();
    directory_value = resource_relocate_directory(directory->extended_models, 0, decoded[4].data, decoded[4].bytes);
    extensions = (const uint32 *)(decoded[4].data + 4);
    directory_value = resource_relocate_directory(directory->animated_models, 0, decoded[5].data, decoded[5].bytes);
    if (game_runtime.asset_counts.animated_model_entries > (decoded[5].bytes - 4) / sizeof(uint32))
        abort();
    animated_models = (const uint32 *)(decoded[5].data + 4);
    for (i = 0; i < sizeof(game_runtime.resource_context.animated_header_storage) / sizeof(game_runtime.resource_context.animated_header_storage[0]); ++i)
        game_runtime.resource_context.animated_header_storage[i] = NULL;
    assets->animated_headers = game_runtime.resource_context.animated_header_storage;
    assets->animated_header_count = game_runtime.asset_counts.animated_model_entries < GAME_SPRITE_MODEL_HEADER_CAPACITY ? game_runtime.asset_counts.animated_model_entries : GAME_SPRITE_MODEL_HEADER_CAPACITY;
    assets->animated_model_source = model_parent;
    directory_value = resource_relocate_directory(directory->environment, 0, decoded[9].data, decoded[9].bytes);
    {
        const uint8 *record = decoded[9].data + 4;
        if (decoded[9].bytes < 39)
            abort();
        GAME_SCENE_ENVIRONMENT *environment = &game_runtime.resource_context.mission_assets.environment;
        MISSION_ENVIRONMENT_RECORD environment_record;

        memcpy(&environment_record, record, 35);

        environment->wide_horizon = environment_record.flags & 1;
        environment->horizon_height = (sint16)environment_record.horizon_height;
        for (i = 0; i < 2; ++i)
        {
            environment->layer[i].height = (sint16)environment_record.layer[i].height;
            environment->layer[i].count = environment_record.layer[i].count;
            environment->layer[i].slope = environment_record.layer[i].slope;
        }
        environment->drift_color = environment_record.drift_noise & 0xFFFFFF;
        environment->noise_attenuation = environment_record.drift_noise >> 24;
        environment->layer_color = environment_record.layer_color;
        environment->horizon_color = environment_record.horizon_color;
        environment->detail_repeat = environment_record.detail_repeat;
        environment->detail_count = environment_record.detail_count;
        environment->detail_density = (sint16)environment_record.detail_density;
        for (i = 0; i < 3; ++i)
            environment->trail_back_color[i] = environment_record.trail_back_color[i];
    }
    restore = directory->restore;
    restore_bytes = directory->texture_groups[0] - restore;
    if (restore_bytes > decoded[10].bytes)
        abort();
    scene_script_bind_restore(decoded[10].data, restore_bytes);
    directory_value = resource_relocate_directory(directory->restore, 0, decoded[10].data, decoded[10].bytes);
    game_runtime.resource_context.restore_data = (uint32)(directory_value);
    count = (memcpy(&wire_word_28, decoded[2].data, sizeof(wire_word_28)), wire_word_28);
    i = 1;
    if (count > 0)
    {
        do
        {
            if (i >= terrain_entries)
                abort();
            target = terrain_directory[i];
            if (target < terrain->guest_base || decoded[2].bytes < 20 || target - terrain->guest_base > decoded[2].bytes - 20)
                abort();
            relocate_pointer_array(target, decoded[2].data + (target - terrain->guest_base) + 4, 4);
            uint32 header_offset = target - terrain->guest_base;
            if (i < GAME_TERRAIN_MODEL_HEADER_CAPACITY && decoded[2].bytes >= sizeof(SCENE_TERRAIN_MODEL_HEADER) && header_offset <= decoded[2].bytes - sizeof(SCENE_TERRAIN_MODEL_HEADER) && !(header_offset & 3))
                game_runtime.resource_context.terrain_header_storage[i] = (const SCENE_TERRAIN_MODEL_HEADER *)(decoded[2].data + header_offset);
            count = (memcpy(&wire_word_30, decoded[2].data, sizeof(wire_word_30)), wire_word_30);
            ++i;
        } while (!((sint32)i > count));
    }
    for (i = 0; i < sizeof(game_runtime.resource_context.model_header_storage) / sizeof(game_runtime.resource_context.model_header_storage[0]); ++i)
        game_runtime.resource_context.model_header_storage[i] = NULL;
    memset(game_runtime.resource_context.model_header_source_storage, 0, sizeof(game_runtime.resource_context.model_header_source_storage));
    assets->model_headers = game_runtime.resource_context.model_header_storage;
    assets->model_header_sources = game_runtime.resource_context.model_header_source_storage;
    assets->model_header_count = GAME_SCENE_MODEL_HEADER_CAPACITY;
    for (offset = 12; offset <= 20; offset += 4)
    {
        count = (memcpy(&wire_word_32, decoded[offset >> 2].data, sizeof(wire_word_32)), wire_word_32);
        i = 0;
        if (count > 0)
        {
            do
            {
                if (offset == 12)
                    target = models[i];
                else if (offset == 16)
                    target = extensions[i];
                else
                    target = animated_models[i];
                model_header_bytes = offset == 20 ? 20 : 28;
                model_base = offset == 12 ? directory->models : offset == 16 ? directory->extended_models : directory->animated_models;
                if (target < model_base || decoded[offset >> 2].bytes < model_header_bytes || target - model_base > decoded[offset >> 2].bytes - model_header_bytes)
                    abort();
                relocate_pointer_array(target, decoded[offset >> 2].data + (target - model_base) + 4, offset == 20 ? 4 : 6);
                if (offset == 12 && i < GAME_SCENE_MODEL_HEADER_CAPACITY)
                {
                    game_runtime.resource_context.model_header_storage[i] = (const SCENE_MESH_MODEL_HEADER *)(decoded[3].data + (target - model_base));
                    game_runtime.resource_context.model_header_source_storage[i] = model_parent;
                }
                else if (offset == 16 && i < GAME_SCENE_MODEL_HEADER_CAPACITY - 96)
                {
                    game_runtime.resource_context.model_header_storage[96 + i] = (const SCENE_MESH_MODEL_HEADER *)(decoded[4].data + (target - model_base));
                    game_runtime.resource_context.model_header_source_storage[96 + i] = model_parent;
                }
                else if (offset == 20 && i < GAME_SPRITE_MODEL_HEADER_CAPACITY)
                {
                    game_runtime.resource_context.animated_header_storage[i] = (const SCENE_SPRITE_MODEL_HEADER *)(decoded[5].data + (target - model_base));
                }
                count = (memcpy(&wire_word_34, decoded[offset >> 2].data, sizeof(wire_word_34)), wire_word_34);
                ++i;
            } while ((sint32)i < count);
        }
    }
    count = (memcpy(&wire_word_36, decoded[4].data, sizeof(wire_word_36)), wire_word_36);
    i = 0;
    if (count > 0)
    {
        do
        {
            models[96 + i] = extensions[i];
            count = (memcpy(&wire_word_38, decoded[4].data, sizeof(wire_word_38)), wire_word_38);
            ++i;
        } while ((sint32)i < count);
    }
    render_begin = directory->render_settings;
    render_bytes = directory->tile_map - render_begin;
    render_data = decoded[0].data;
    if (!render_data || render_bytes < 116 || render_bytes > decoded[0].bytes)
        abort();
    mission_asset_bind_render_settings(render_data);
    mission_asset_bind_terrain_capture(render_data, render_bytes);
    scene_geometry_prepare_capture_textures(game_runtime.render_settings.palette_primary, game_runtime.terrain_capture.records, game_runtime.terrain_capture.count);
    terrain->guest_base = model_parent.encoded_base;
    terrain->data = model_parent.data;
    terrain->bytes = model_parent.bytes;
    mission_script_bind_region(game_runtime.resource_context.mission_assets.scripts.data, game_runtime.resource_context.mission_assets.scripts.legacy_base, cursor, (MISSION_SCRIPT_REFERENCE){model_parent.data, model_parent.bytes, model_parent.encoded_base});
    arena_set_cursor(directory->texture_groups[0]);
    mission_asset_load_poll();
    return 0x4000000;
}

static void mission_asset_bind_aircraft_metadata(const uint8 *data, uint32 bytes)
{
    AIRCRAFT_MISSION_RECORD record;
    AIRCRAFT_MISSION_METADATA *metadata = &game_runtime.aircraft_mission_metadata;
    uint32 i;

    if (bytes < 168 || !data)
        abort();
    memcpy(&record, data, 168);
    for (i = 0; i < 3; ++i)
        metadata->camera_offsets[i] = record.camera_offsets[i];
    for (i = 0; i < 4; ++i)
        metadata->effect_lines[i] = record.effect_lines[i];
    metadata->effect_center = record.effect_center;
    for (i = 0; i < 2; ++i)
    {
        metadata->effect_trails[i] = record.effect_trails[i];
        metadata->spawn_vector[i] = record.spawn_vector[i];
        metadata->weapon_vectors[i][0] = record.weapon_vectors[i * 2];
        metadata->weapon_vectors[i][1] = record.weapon_vectors[i * 2 + 1];
    }
}

void aircraft_bind_thrust_metadata(const uint8 *data, uint32 bytes)
{
    AIRCRAFT_THRUST_RECORD record;
    AIRCRAFT_THRUST_METADATA *metadata = &game_runtime.aircraft_thrust_metadata;
    uint32 i;

    if (bytes < 124 || !data)
        abort();
    memcpy(&record, data, 124);
    metadata->count = record.count;
    metadata->uses_axes = record.uses_axes;
    metadata->base_rgbc = record.base_rgbc;
    metadata->far_color = record.far_color;
    metadata->extent_xy = record.extent_xy;
    for (i = 0; i < 2; ++i)
    {
        AIRCRAFT_THRUST_EMITTER *emitter = &metadata->emitters[i];
        emitter->direction_xy = record.style[i].direction_xy;
        emitter->direction_z = record.style[i].direction_z;
        emitter->color_xy = record.style[i].color_xy;
        emitter->color_z = record.style[i].color_z;
        emitter->edge_xy[0] = record.geometry[i].vertices[0].xy;
        emitter->edge_xy[1] = record.geometry[i].vertices[1].xy;
        emitter->edge_z[0] = (uint16)record.geometry[i].vertices[0].z_axis;
        emitter->edge_z[1] = (uint16)record.geometry[i].vertices[1].z_axis;
        emitter->tip_xy[0] = record.geometry[i].vertices[2].xy;
        emitter->tip_z_word[0] = record.geometry[i].vertices[2].z_axis;
        emitter->tip_xy[1] = record.geometry[i].vertices[3].xy;
        emitter->tip_z_word[1] = record.geometry[i].vertices[3].z_axis;
        emitter->axis_low[0] = (sint16)(uint16)(record.geometry[i].vertices[0].z_axis >> 16);
        emitter->axis_high[0] = (sint16)(uint16)(record.geometry[i].vertices[1].z_axis >> 16);
        emitter->axis_low[1] = (sint16)(uint16)(record.geometry[i].vertices[3].z_axis >> 16);
        emitter->axis_high[1] = (sint16)(uint16)(record.geometry[i].vertices[2].z_axis >> 16);
    }
}

uint32 aircraft_unpack_assets(void)
{
    uint8 *asset_data;
    RESOURCE_STREAM_BUFFER backing;
    ASSET_SECTION_VIEW decoded[2];
    uint32 asset_base, asset_bytes;
    uint32 texture_bytes, texture_offset, initialized_bytes;
    uint32 header, texture_begin, texture_end, cursor, arena_mark;
    uint32 models_begin, metadata_begin;
    uint32 i, target, flags;
    const uint32 *models;
    FUNCTION_MARKER(0x80035F98, "ACE3.BIN");
    asset_base = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    backing = game_runtime.resource_slot.backing;
    if (!asset_data || asset_bytes < 4 + sizeof(AIRCRAFT_ASSET_DIRECTORY))
        abort();
    header = resource_relocate_directory(asset_base, 0, asset_data, asset_bytes);
    AIRCRAFT_ASSET_DIRECTORY *directory = (AIRCRAFT_ASSET_DIRECTORY *)(asset_data + 4);
    sound_load_bank((SOUND_BANK_INPUT){asset_data, asset_bytes, directory->sound_bank - asset_base});
    texture_begin = directory->textures;
    texture_end = directory->sound_bank;
    texture_bytes = scene_texture_unpack_tim_group(texture_begin, texture_end, (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, asset_base}, backing);
    texture_offset = texture_end - asset_base;
    if (texture_end < asset_base || texture_offset > asset_bytes || texture_bytes > 0xFFFFFFFFu - texture_offset)
        abort();
    initialized_bytes = texture_offset + texture_bytes;
    if (initialized_bytes < asset_bytes)
        initialized_bytes = asset_bytes;
    DrawSync(0);
    cursor = mission_asset_unpack_sections((uint8 *)directory, header, 2, 16, asset_base, asset_data, asset_bytes, decoded, backing);
    if (cursor < asset_base)
        abort();
    if (initialized_bytes < cursor - asset_base)
        initialized_bytes = cursor - asset_base;
    /* retain the initialized raw and decoded tail as one mutable resource view */
    game_runtime.resource_context.aircraft_backing = (RESOURCE_ASSET_SOURCE){asset_data, initialized_bytes, asset_base};
    models_begin = directory->models;
    resource_relocate_directory(models_begin, 0, decoded[0].data, decoded[0].bytes);
    if (decoded[0].bytes < 4 + AIRCRAFT_COMPOSITE_MODEL_COUNT * sizeof(*models))
        abort();
    models = (const uint32 *)(decoded[0].data + 4);
    for (i = 0; i < AIRCRAFT_COMPOSITE_MODEL_COUNT; ++i)
    {
        target = models[i];
        if (target < models_begin || decoded[0].bytes < 28 || target - models_begin > decoded[0].bytes - 28)
            abort();
        relocate_pointer_array(target, decoded[0].data + (target - models_begin) + 4, 6);
        game_runtime.resource_context.aircraft_headers[i] = (const SCENE_MESH_MODEL_HEADER *)(decoded[0].data + (target - models_begin));
    }
    game_runtime.resource_context.aircraft_model_source = game_runtime.resource_context.aircraft_backing;
    game_runtime.asset_counts.aircraft_model_entries = AIRCRAFT_COMPOSITE_MODEL_COUNT;
    metadata_begin = directory->metadata;
    mission_asset_bind_aircraft_metadata(decoded[1].data, cursor - metadata_begin);
    aircraft_bind_thrust_metadata(decoded[1].data, cursor - metadata_begin);
    mission_script_bind_aircraft_region(asset_data, asset_base, asset_base + initialized_bytes);
    arena_set_cursor(directory->textures);
    arena_mark = arena_peek(0);
    game_runtime.resource_context.arena_mark = (uint32)(arena_mark);
    mission_asset_load_poll();
    flags = game_runtime.display.flags;
    game_runtime.display.mode = (uint32)(0);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.flags = (uint32)(flags | 5);
    return 0x4000000;
}
