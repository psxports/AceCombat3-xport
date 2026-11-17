#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "game_functions.h"

enum
{
    TERRAIN_POINT_BASE = 0x1F8000FC,
    TERRAIN_VERTEX_BYTES = 20,
    TERRAIN_STRIP_COUNT = 4,
    TERRAIN_FRAME_POINTS = 5,
    TERRAIN_SPLIT_LEVEL = 4,
    TERRAIN_POINT_COUNT = TERRAIN_STRIP_COUNT + TERRAIN_FRAME_POINTS * (TERRAIN_SPLIT_LEVEL + 1)
};

typedef char TERRAIN_VERTEX_LAYOUT_CHECK[sizeof(TERRAIN_VERTEX) == TERRAIN_VERTEX_BYTES && offsetof(TERRAIN_VERTEX, z) == 4 && offsetof(TERRAIN_VERTEX, flags) == 6 && offsetof(TERRAIN_VERTEX, screen) == 8 && offsetof(TERRAIN_VERTEX, color) == 12 && offsetof(TERRAIN_VERTEX, depth) == 16 && offsetof(TERRAIN_VERTEX, uv) == 18 ? 1 : -1];

typedef struct
{
    uint8 point[4];
    uint8 written;
} TERRAIN_CORNERS;

enum
{
    TERRAIN_CORNER_BASE = 0x1F80009C,
    TERRAIN_CORNER_ROWS = TERRAIN_SPLIT_LEVEL + 2,
    TERRAIN_CLUT = 0x4ED6,
    TERRAIN_TPAGE = 0x39
};

typedef char terrain_strip_packet_layout_check[sizeof(POLY_GT4) == 52 && offsetof(POLY_GT4, p) == 4 && sizeof(POINT_CT) == 12 && offsetof(POINT_CT, xy) == 4 && offsetof(POINT_CT, uv) == 8 && offsetof(POINT_CT, attr) == 10 ? 1 : -1];

static const uint32 terrain_row_masks[16][2] = {
    {0x482848FFu, 0xFF884828u}, {0xD484D48Fu, 0xF884D484u}, {0x48284828u, 0x88284828u}, {0x82028202u, 0x82028202u}, {0x48284828u, 0x48284848u}, {0xD484D484u, 0xD484D488u}, {0xF8484828u, 0x4828488Fu}, {0xFF828202u, 0x820288FFu}, {0xFF884828u, 0x482848FFu}, {0xF884D484u, 0xD484D48Fu}, {0xD8284828u, 0x48284848u}, {0x82028202u, 0x82028202u}, {0x48284828u, 0x48284828u}, {0xD484D488u, 0x8484D484u}, {0x4828488Fu, 0xF8284828u}, {0x820288FFu, 0xFF828202u},
};

typedef struct
{
    uint32 origin[3];
    uint32 row_bits, row_limit, column_limit, row_index, column_index;
    uint32 palette[16];
    uint16 first_column, first_row;
    uint8 row_bits_written;
} TERRAIN_SCAN_WORK;

typedef struct
{
    uint32 row_mode, column_mode;
    uint32 masks[2][2];
    uint16 height[3];
    uint8 row_mode_written, column_mode_written, masks_written;
} TERRAIN_EDGE_WORK;

typedef struct
{
    uint16 x, y, z;
} TERRAIN_STRIP_POINT;

typedef struct
{
    uint16 x, y, z, reserved;
} TERRAIN_STRIP_PUBLICATION;

typedef struct
{
    uint32 cursor;
    uint32 origin[3];
    uint32 row_bits, column_limit, row_limit, row_index, column_index;
    uint32 first_cell;
    uint16 clut, tpage;
    TERRAIN_STRIP_PUBLICATION strip[2];
    uint32 masks[2][2];
    uint32 palette[16];
    uint32 row_mode, column_mode;
    uint16 height[3], reserved;
    uint32 corner_addresses[TERRAIN_CORNER_ROWS][4];
    TERRAIN_VERTEX points[TERRAIN_POINT_COUNT];
} TERRAIN_MODE_TWO_PUBLICATION;

typedef union
{
    TERRAIN_MODE_TWO_PUBLICATION mode_two;

    struct
    {
        uint32 prefix[5];
        uint32 vector[3];
    } ordinary;
} TERRAIN_SCRATCH_PUBLICATION;

typedef char terrain_scratch_publication_layout[sizeof(TERRAIN_STRIP_PUBLICATION) == 8 && sizeof(TERRAIN_MODE_TWO_PUBLICATION) == 832 && sizeof(TERRAIN_SCRATCH_PUBLICATION) == 832 && offsetof(TERRAIN_SCRATCH_PUBLICATION, ordinary.vector) == 20 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, origin) == 4 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, row_bits) == 16 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, column_limit) == 20 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, row_limit) == 24 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, row_index) == 28 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, column_index) == 32 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, first_cell) == 36 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, clut) == 40 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, tpage) == 42 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, strip) == 44 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, masks) == 60 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, palette) == 76 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, row_mode) == 140 &&
                                                        offsetof(TERRAIN_MODE_TWO_PUBLICATION, column_mode) == 144 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, height) == 148 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, corner_addresses) == TERRAIN_CORNER_BASE - 0x1F800000 && offsetof(TERRAIN_MODE_TWO_PUBLICATION, points) == TERRAIN_POINT_BASE - 0x1F800000
                                                    ? 1
                                                    : -1];

typedef char palette_pixel_upload_layout_check[offsetof(DR_LOAD, code) == 4 && offsetof(DR_LOAD, p) == 16 && offsetof(DR_LOAD, p) + 2 * sizeof(uint32) == 24 ? 1 : -1];

typedef struct
{
    uint32 prefix;
    uint32 projected_indices;
    uint32 color[3];
    uint32 uv[3];
} SCENE_MESH_TRIANGLE_FACE;

typedef struct
{
    uint32 prefix;
    uint32 projected_indices;
    uint32 color[4];
    uint32 uv[3];
} SCENE_MESH_QUAD_FACE;

typedef struct
{
    uint32 prefix;
    uint32 projected_indices;
    SCENE_LIT_FACE_DATA data;
} SCENE_LIT_MESH_FACE;

typedef char scene_lit_mesh_face_layout[sizeof(SCENE_LIT_MESH_FACE) == 24 && offsetof(SCENE_LIT_MESH_FACE, projected_indices) == 4 && offsetof(SCENE_LIT_MESH_FACE, data) == 8 ? 1 : -1];

SCENE_FACE_GROUP scene_geometry_face_group(uint32 address, uint32 stride)
{
    const uint8 *header = mission_packet_span(address, sizeof(uint32));
    const uint8 *data;
    uint32 count, bytes;

    memcpy(&count, header, sizeof(count));
    if ((sint32)count <= 0)
        return (SCENE_FACE_GROUP){NULL, count, stride};
    if (!stride || count > (UINT32_MAX - 4) / stride)
        abort();
    bytes = 4 + count * stride;
    data = mission_packet_span(address, bytes);
    if (data != header || ((uintptr_t)(data + 4) & 3))
        abort();
    return (SCENE_FACE_GROUP){data + 4, count, stride};
}

typedef char scene_mesh_face_layout[sizeof(SCENE_MESH_TRIANGLE_FACE) == 32 && offsetof(SCENE_MESH_TRIANGLE_FACE, projected_indices) == 4 && offsetof(SCENE_MESH_TRIANGLE_FACE, color) == 8 && offsetof(SCENE_MESH_TRIANGLE_FACE, uv) == 20 && sizeof(SCENE_MESH_QUAD_FACE) == 36 && offsetof(SCENE_MESH_QUAD_FACE, projected_indices) == 4 && offsetof(SCENE_MESH_QUAD_FACE, color) == 8 && offsetof(SCENE_MESH_QUAD_FACE, uv) == 24 ? 1 : -1];

typedef struct
{
    XY position;
    uint16 z, uv;
    uint32 color;
} SCENE_SUBDIV_VERTEX;

typedef struct
{
    SCENE_SUBDIV_VERTEX vertex[4];
    XY screen[4];
    uint16 depth[4];
} SCENE_SUBDIV_FRAME;

enum
{
    SCENE_SUBDIV_FRAME_COUNT = 5
};

typedef char SCENE_SUBDIV_LAYOUT_CHECK[sizeof(SCENE_SUBDIV_VERTEX) == 12 && offsetof(SCENE_SUBDIV_VERTEX, z) == 4 && offsetof(SCENE_SUBDIV_VERTEX, uv) == 6 && offsetof(SCENE_SUBDIV_VERTEX, color) == 8 && offsetof(SCENE_SUBDIV_FRAME, screen) == 48 && offsetof(SCENE_SUBDIV_FRAME, depth) == 64 && sizeof(SCENE_SUBDIV_FRAME) == 72 ? 1 : -1];

typedef char scene_subdivision_packet_layout_check[sizeof(POINT_CT) == 12 && offsetof(POINT_CT, xy) == 4 && offsetof(POINT_CT, uv) == 8 && offsetof(POINT_CT, attr) == 10 && sizeof(POLY_GT3) == 40 && offsetof(POLY_GT3, p) == 4 && sizeof(POLY_GT4) == 52 && offsetof(POLY_GT4, p) == 4 ? 1 : -1];

typedef char scene_subdivision_scratch_header_layout_check[sizeof(SCENE_FLAT_QUAD_FACE) == 36 && sizeof(SCENE_GOURAUD_QUAD_FACE) == 48 && offsetof(SCENE_QUAD_FACE_TEXTURE, vertex_offsets) == 8 && offsetof(SCENE_QUAD_FACE_TEXTURE, projected_indices) == 16 && offsetof(SCENE_QUAD_FACE_TEXTURE, uv) == 20 && offsetof(SCENE_FLAT_QUAD_FACE, color) == 32 && offsetof(SCENE_GOURAUD_QUAD_FACE, color) == 32 && sizeof(SCENE_SUBDIV_SCRATCH_HEADER) == 60 && offsetof(SCENE_SUBDIV_SCRATCH_HEADER, flat_polygon) == 4 && offsetof(SCENE_SUBDIV_SCRATCH_HEADER, depth) == 52 && offsetof(SCENE_SUBDIV_SCRATCH_HEADER, tpage) == 56 && offsetof(SCENE_SUBDIV_SCRATCH_HEADER, clut) == 58 ? 1 : -1];

static uint32 scene_subdiv_draw_gouraud_face(const SCENE_GOURAUD_QUAD_FACE *polygon, SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input);
static uint32 scene_subdiv_draw_frame(SCENE_SUBDIV_FRAME *frame, const SCENE_SUBDIV_SCRATCH_HEADER *scratch, uint32 count, uint32 mode, SCENE_RENDER_STREAM *stream);

static void scene_geometry_emit_prelit_triangles(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);
static void scene_geometry_emit_prelit_quads(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);
static void scene_geometry_emit_lit_triangles(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);
static void scene_geometry_emit_lit_quads(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);
#include "psx_gte.h"

SCENE_DETAIL_CALLBACK const scene_geometry_detail_callbacks[5] = {
    scene_geometry_draw_detail_0, scene_geometry_draw_detail_1, scene_geometry_draw_detail_2, scene_geometry_draw_detail_3, scene_geometry_draw_detail_4,
};

SCENE_GEOMETRY_CALLBACK const scene_geometry_mesh_callbacks[6] = {
    0, scene_geometry_draw_mesh_style_1, scene_geometry_draw_mesh_style_2, scene_geometry_draw_mesh_style_3, scene_geometry_draw_mesh_style_4, scene_geometry_draw_mesh_style_5,
};

SCENE_GEOMETRY_COMPOSITE_CALLBACK const scene_geometry_composite_callbacks[6] = {0, scene_geometry_draw_composite_variant_1, scene_geometry_draw_composite_variant_2, 0, 0, 0};

uint32 scene_geometry_collect_primary(SCENE_GEOMETRY_DESCRIPTOR *entries)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    SCENE_GEOMETRY_DESCRIPTOR *entry = entries;
    EFFECT_OBJECT *effect;
    uint32 excluded, selected, index, value, kind, mask;
    FUNCTION_MARKER(0x80067144, "ACE3.BIN");
    excluded = game_runtime.camera_control.excluded_actor;
    selected = game_runtime.camera_control.follow_actor;
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((actor_traits_object->flags & 0x14000) != 0x14000 || excluded == index)
            continue;
        value = actor_traits_object->render_flags;
        kind = 1;
        if (value & 1)
        {
            kind = 4;
            if (selected == index)
            {
                if (game_runtime.camera_control.mode & 6)
                    kind = 2;
            }
        }
        entry->kind = (uint8)kind;
        value = actor_traits_object->model;
        entry->model_index = (uint8)value;
        value = actor_traits_object->render_flags;
        entry->flags = (uint8)value;
        value = actor_traits_object->parameter;
        entry->native_pose = &pose_owner_object->pose;
        entry->parameter = (uint16)value;

        value = actor_traits_object->flags;
        entry->style = 0;
        entry->variant = 0;

        value = (sint16)actor_traits_object->kind;
        kind = value == 43 ? 3 : value == 35 ? 4 : value == 36 ? 5 : 0;
        if (kind)
        {
            entry->style = (uint8)kind;
        }
        kind = value == 25 ? 2 : 1;
        entry->variant = (uint8)kind;
        entry->selected = selected == index;

        ++entry;
    }
    mask = effect_objects.live;
    if (mask)
    {
        for (index = 0; index < 32; ++index)
        {
            if ((mask & ((uint32)1 << index)) == 0)
                continue;
            entry->kind = 1;
            effect = effect_objects.slot + index;
            value = effect->model_kind;
            if (value == 0)
                continue;
            entry->model_index = (uint8)(value + 23);
            entry->flags = 0;
            entry->parameter = 0;
            entry->native_pose = &effect->pose;
            entry->selected = 0;
            entry->style = 0;
            entry->variant = 0;

            value = effect->render_flags;
            if (value & 16)
            {
                entry->style = 1;
            }
            value = effect->render_flags;
            if (value & 32)
            {
                entry->style = 2;
            }
            ++entry;
        }
    }
    entry->model_index = 255;
    return (uint32)(entry - entries);
}

void scene_geometry_collect_secondary(SCENE_GEOMETRY_DESCRIPTOR *entries)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    SCENE_GEOMETRY_DESCRIPTOR *entry = entries;
    uint32 excluded, selected, index, value, kind;
    FUNCTION_MARKER(0x80067380, "ACE3.BIN");
    excluded = game_runtime.camera_control.excluded_actor - 32;
    selected = game_runtime.camera_control.follow_actor;
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 32;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((actor_traits_object->flags & 0x14000) != 0x14000 || excluded == index)
            continue;
        value = actor_traits_object->render_flags;
        kind = value & 2 ? 8 : 16;
        entry->kind = (uint8)kind;
        value = actor_traits_object->model;
        entry->model_index = (uint8)value;
        value = actor_traits_object->render_flags;
        entry->flags = (uint8)value;
        value = actor_traits_object->parameter;
        entry->native_pose = &pose_owner_object->pose;
        entry->parameter = (uint16)value;

        value = actor_traits_object->flags;
        entry->style = 0;
        entry->variant = 0;

        value = (sint16)actor_traits_object->kind;
        kind = value == 43 ? 3 : value == 35 ? 4 : value == 36 ? 5 : 0;
        if (kind)
        {
            entry->style = (uint8)kind;
        }
        entry->selected = selected == index;
        ++entry;
    }
    entry->model_index = 255;
    {
        (void)(uint32)((uint32)(entry - entries));
        return;
    }
}

void scene_geometry_collect_mission_primary(SCENE_GEOMETRY_DESCRIPTOR *entries)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    SCENE_GEOMETRY_DESCRIPTOR *entry = entries;
    uint32 excluded, selected, index, flags, kind, value;
    sint32 type;
    FUNCTION_MARKER(0x801073B0, "ACE3.BIN");
    excluded = game_runtime.camera_control.excluded_actor;
    selected = game_runtime.camera_control.follow_actor;
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags;
        if ((flags & 0x14000) != 0x14000 || excluded == index)
            continue;
        flags = actor_traits_object->render_flags;
        if (flags & 1)
        {
            kind = 4;
            if (selected == index)
            {
                if (game_runtime.camera_control.mode & 6)
                    kind = 2;
            }
        }
        else
            kind = 1;
        entry->kind = kind;
        entry->model_index = actor_traits_object->model;
        entry->flags = actor_traits_object->render_flags;
        value = actor_traits_object->parameter;
        entry->native_pose = &pose_owner_object->pose;
        entry->parameter = (uint16)value;
        value = actor_traits_object->flags;
        type = (sint16)actor_traits_object->kind;
        value = type == 43 ? 3 : type == 35 ? 4 : type == 36 ? 5 : 0;
        entry->style = (uint8)value;
        entry->variant = 0;
        entry->selected = selected == index;
        ++entry;
    }
    entry->model_index = 255;
    {
        (void)(uint32)((uint32)(entry - entries));
        return;
    }
}

typedef char scene_blend_wire_layout_check[sizeof(SCENE_VERTEX_TRIPLE) == 20 && offsetof(SCENE_VERTEX_TRIPLE, xy) == 0 && offsetof(SCENE_VERTEX_TRIPLE, z01) == 12 && offsetof(SCENE_VERTEX_TRIPLE, z2) == 16 ? 1 : -1];

SCENE_VERTEX_BLEND_SOURCE scene_geometry_blend_source(uint32 first, uint32 second, int native_second, const RESOURCE_ASSET_SOURCE *first_source, const RESOURCE_ASSET_SOURCE *second_source)
{
    SCENE_VERTEX_BLEND_SOURCE source;
    uint32 first_offset = first_source ? first - first_source->encoded_base : 0;
    uint32 second_offset = second_source ? second - second_source->encoded_base : 0;
    const uint8 *first_data = first_source && first_source->data && first >= first_source->encoded_base && first_offset <= first_source->bytes && sizeof(uint32) <= first_source->bytes - first_offset ? first_source->data + first_offset : NULL;
    const uint8 *header = first_data ? first_data : mission_packet_span(first, sizeof(uint32));
    const uint8 *data;
    uint32 remaining, groups = 1, bytes;

    memcpy(&source.count, header, sizeof(source.count));
    remaining = source.count - 3;
    if ((sint32)remaining > 0)
        groups += remaining / 3 + (remaining % 3 != 0);
    if (groups > (UINT32_MAX - 2) / SCENE_VERTEX_TRIPLE_BYTES)
        abort();
    bytes = 2 + groups * SCENE_VERTEX_TRIPLE_BYTES;
    data = first_data && bytes <= first_source->bytes - first_offset ? first_data : mission_packet_span(first, bytes);
    if (data != header || ((uintptr_t)(data + 4) & 3))
        abort();
    source.first = (const SCENE_VERTEX_TRIPLE *)(data + 4);
    source.second = NULL;
    if (!native_second)
    {
        const uint8 *second_data = second_source && second_source->data && second >= second_source->encoded_base && second_offset <= second_source->bytes && sizeof(uint32) <= second_source->bytes - second_offset ? second_source->data + second_offset : NULL;
        header = second_data ? second_data : mission_packet_span(second, sizeof(uint32));
        data = second_data && bytes <= second_source->bytes - second_offset ? second_data : mission_packet_span(second, bytes);
        if (data != header || ((uintptr_t)(data + 4) & 3))
            abort();
        source.second = (const SCENE_VERTEX_TRIPLE *)(data + 4);
    }
    return source;
}

uint32 scene_geometry_blend_vertices(uint32 factor, SCENE_VERTEX_BLEND_SOURCE source, uint32 dst, const SCENE_VERTEX_BLEND_WORK *native_src1, SCENE_VERTEX_BLEND_WORK *native_out, SCENE_VERTEX_BLEND_PACKED *packed_out)
{
    uint32 wire_word_1;
    uint32 mark = dst - 16;
    uint32 packed0, packed1, zpair0, zpair1, next1, third1;
    uint32 vertex = 0;
    uint32 x, y, z;
    sint32 source_z, target_z, third_z, count;
    FUNCTION_MARKER(0x80066A9C, "ACE3.BIN");
    xport_gte_write_data(8, factor);
    count = (sint32)source.count;
    if (native_out != NULL)
        native_out->count = (uint32)count;
    else if (packed_out != NULL)
        packed_out->count = (uint32)count;
    else
        (wire_word_1 = (count), (void)memcpy(mission_packet_span(dst, sizeof(uint32)), &wire_word_1, sizeof(wire_word_1)));
    do
    {
        SCENE_VERTEX_TRIPLE *wire_out = NULL;
        const SCENE_VERTEX_TRIPLE *wire0 = source.first + vertex / 3;
        packed0 = wire0->xy[0];
        source_z = wire0->z[0];
        zpair0 = wire0->z01;
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, source_z);
        const SCENE_VERTEX_TRIPLE *wire1 = native_src1 ? NULL : source.second + vertex / 3;
        packed1 = native_src1 != NULL ? ((uint32)(uint16)native_src1->vertex[vertex].vx | ((uint32)(uint16)native_src1->vertex[vertex].vy << 16)) : wire1->xy[0];
        target_z = native_src1 != NULL ? (sint16)native_src1->vertex[vertex].vz : wire1->z[0];
        zpair1 = native_src1 != NULL ? ((uint32)(uint16)native_src1->vertex[vertex].vz | ((uint32)(uint16)native_src1->vertex[vertex + 1].vz << 16)) : wire1->z01;
        xport_gte_write_control(21, (sint16)packed1);
        xport_gte_write_control(22, (sint32)packed1 >> 16);
        xport_gte_write_control(23, target_z);
        xport_gte_execute(0x980011);
        dst += 20;
        mark += 20;
        next1 = native_src1 != NULL ? ((uint32)(uint16)native_src1->vertex[vertex + 1].vx | ((uint32)(uint16)native_src1->vertex[vertex + 1].vy << 16)) : wire1->xy[1];
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        if (native_out != NULL)
        {
            native_out->vertex[vertex].vx = (sint16)(uint16)x;
            native_out->vertex[vertex].vy = (sint16)(uint16)y;
            native_out->vertex[vertex].vz = (sint16)(uint16)z;
        }
        else
        {
            wire_out = packed_out ? packed_out->group + vertex / 3 : mission_packet_span(mark, offsetof(SCENE_VERTEX_TRIPLE, z2) + sizeof(uint16));
            wire_out->point[0].x = (sint16)(uint16)(uint32)(x);
            wire_out->point[0].y = (sint16)(uint16)(uint32)(y);
            wire_out->z[0] = (sint16)(uint16)(uint32)(z);
        }
        packed0 = wire0->xy[1];
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, (sint32)zpair0 >> 16);
        xport_gte_write_control(21, (sint16)next1);
        xport_gte_write_control(22, (sint32)next1 >> 16);
        xport_gte_write_control(23, (sint32)zpair1 >> 16);
        xport_gte_execute(0x980011);
        third1 = native_src1 != NULL ? ((uint32)(uint16)native_src1->vertex[vertex + 2].vx | ((uint32)(uint16)native_src1->vertex[vertex + 2].vy << 16)) : wire1->xy[2];
        third_z = native_src1 != NULL ? (sint16)native_src1->vertex[vertex + 2].vz : wire1->z[2];
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        if (native_out != NULL)
        {
            native_out->vertex[vertex + 1].vx = (sint16)(uint16)x;
            native_out->vertex[vertex + 1].vy = (sint16)(uint16)y;
            native_out->vertex[vertex + 1].vz = (sint16)(uint16)z;
        }
        else
        {
            wire_out->point[1].x = (sint16)(uint16)(uint32)(x);
            wire_out->point[1].y = (sint16)(uint16)(uint32)(y);
            wire_out->z[1] = (sint16)(uint16)(uint32)(z);
        }
        packed0 = wire0->xy[2];
        source_z = wire0->z[2];
        xport_gte_write_data(9, (sint16)packed0);
        xport_gte_write_data(10, (sint32)packed0 >> 16);
        xport_gte_write_data(11, source_z);
        xport_gte_write_control(21, (sint16)third1);
        xport_gte_write_control(22, (sint32)third1 >> 16);
        xport_gte_write_control(23, third_z);
        xport_gte_execute(0x980011);
        count = ((uint32)count - 3);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        if (native_out != NULL)
        {
            native_out->vertex[vertex + 2].vx = (sint16)(uint16)x;
            native_out->vertex[vertex + 2].vy = (sint16)(uint16)y;
            native_out->vertex[vertex + 2].vz = (sint16)(uint16)z;
        }
        else
        {
            wire_out->point[2].x = (sint16)(uint16)(uint32)(x);
            wire_out->point[2].y = (sint16)(uint16)(uint32)(y);
            wire_out->z[2] = (sint16)(uint16)(uint32)(z);
        }
        vertex += 3;
    } while (count > 0);
    return mark + 20;
}

static uint32 scene_geometry_blend_axes_vertices(uint32 factor, uint32 weight, uint32 base, uint32 first, uint32 second, uint32 dst, SCENE_VERTEX_BLEND_WORK *workspace, const RESOURCE_ASSET_SOURCE *parent)
{
    FUNCTION_MARKER(0x80066C18, "ACE3.BIN");
    scene_geometry_blend_vertices(factor, scene_geometry_blend_source(first, second, 0, parent, parent), 0, NULL, workspace, NULL);
    return scene_geometry_blend_vertices(weight, scene_geometry_blend_source(base, 0, 1, parent, NULL), dst, workspace, NULL, NULL);
}

void scene_geometry_interpolate_sprites(SCENE_GEOMETRY_SPRITE *entries, SCENE_RENDER_STREAM *stream)
{
    const SCENE_SPRITE_MODEL_HEADER *result = entries->model_data;
    uint32 code, fraction, next, dst, first_vertices, second_vertices, output_begin;
    const SCENE_SPRITE_MODEL_HEADER *lo, *hi;
    sint32 level;
    FUNCTION_MARKER(0x80066C80, "ACE3.BIN");
    while (result != NULL)
    {
        if (entries->flags & 4)
        {
            code = entries->parameter;
            level = (sint16)code >> 12;
            fraction = code & 0xFFF;
            if (level < 0)
                level = 0;
            if (level >= 5)
                level = 4;
            if (!entries->lod_model_headers || (uint32)level >= entries->lod_header_count)
                abort();
            if (fraction != 0)
            {
                next = level < 4 ? level + 1 : 4;
                if (next >= entries->lod_header_count)
                    abort();
                dst = stream->packet;
                lo = entries->lod_model_headers[level];
                hi = entries->lod_model_headers[next];
                stream->packet = dst + 36;
                SCENE_SPRITE_MODEL_HEADER *blended;
                const SCENE_SPRITE_MODEL_HEADER *first = scene_sprite_model_header(entries->model_source, lo, 9 * sizeof(uint32));
                if (mission_native_packet_span(dst, 9 * sizeof(uint32)) || ui_native_packet_span(dst, 9 * sizeof(uint32)))
                {
                    blended = &entries->blended_model;
                    memcpy(blended, first, sizeof(*blended));
                }
                else
                {
                    blended = mission_packet_span(dst, 9 * sizeof(uint32));
                    copy_words((uint32 *)blended, (const uint32 *)first, 9);
                }
                blended->vertices = stream->packet;
                const SCENE_SPRITE_MODEL_HEADER *second = scene_sprite_model_header(entries->model_source, hi, sizeof(*second));
                first_vertices = first->vertices;
                second_vertices = second->vertices;
                output_begin = stream->packet;
                stream->packet = scene_geometry_blend_vertices(fraction, scene_geometry_blend_source(first_vertices, second_vertices, 0, &entries->model_source, &entries->model_source), output_begin, NULL, NULL, NULL);
                entries->vertex_source = (RESOURCE_ASSET_SOURCE){mission_packet_span(output_begin, stream->packet - output_begin), stream->packet - output_begin, output_begin};
                entries->model_data = dst ? blended : NULL;
            }
            else
            {
                lo = entries->lod_model_headers[level];
                entries->model_data = lo ? scene_sprite_model_header(entries->model_source, lo, sizeof(*entries->model_data)) : NULL;
                entries->vertex_source = (RESOURCE_ASSET_SOURCE){0};
            }
        }
        result = entries[1].model_data;
        ++entries;
    }
    return;
}

sint32 scene_geometry_read_vertex_count(const uint8 *data)
{
    uint32 wire_word_0;
    uint32 count = (memcpy(&wire_word_0, data, sizeof(wire_word_0)), wire_word_0);
    return count <= INT32_MAX ? (sint32)count : -1 - (sint32)~count;
}

SCENE_VERTEX_TRIPLE scene_geometry_read_vertex_triple(const uint8 *data)
{
    SCENE_VERTEX_TRIPLE triple;
    memcpy(&triple, data, sizeof(triple));
    return triple;
}

typedef char scene_model_header_layout_check[sizeof(SCENE_TERRAIN_MODEL_HEADER) == 24 && offsetof(SCENE_TERRAIN_MODEL_HEADER, vertices) == 4 && offsetof(SCENE_TERRAIN_MODEL_HEADER, flat_quads) == 12 && offsetof(SCENE_TERRAIN_MODEL_HEADER, gouraud_quads) == 16 && offsetof(SCENE_TERRAIN_MODEL_HEADER, height) == 22 && sizeof(SCENE_SPRITE_MODEL_HEADER) == 20 && offsetof(SCENE_SPRITE_MODEL_HEADER, radius) == 2 && offsetof(SCENE_SPRITE_MODEL_HEADER, vertices) == 4 && offsetof(SCENE_SPRITE_MODEL_HEADER, flat_quads) == 12 && offsetof(SCENE_SPRITE_MODEL_HEADER, gouraud_quads) == 16 && sizeof(SCENE_MESH_MODEL_HEADER) == 28 && offsetof(SCENE_MESH_MODEL_HEADER, radius) == 2 && offsetof(SCENE_MESH_MODEL_HEADER, vertices) == 4 && offsetof(SCENE_MESH_MODEL_HEADER, normals) == 16 && offsetof(SCENE_MESH_MODEL_HEADER, triangles) == 20 && offsetof(SCENE_MESH_MODEL_HEADER, quads) == 24 ? 1 : -1];

uint32 scene_mesh_model_vertices(const SCENE_MESH_MODEL_HEADER *model, uint32 level)
{
    return model->vertices[level];
}

typedef char scene_sprite_projection_layout_check[sizeof(SCENE_SPRITE_PROJECTION) == 16 && offsetof(SCENE_SPRITE_PROJECTION, depth) == 4 && offsetof(SCENE_SPRITE_PROJECTION, intensity) == 8 ? 1 : -1];

typedef struct
{
    uint32 preceding[8];
    SCENE_SPRITE_PROJECTION vertices[SCENE_SPRITE_PROJECTION_CAPACITY];
    SCENE_SUBDIV_SCRATCH_HEADER subdivision;
    uint32 untouched_620;
    SCENE_SUBDIV_FRAME frames[SCENE_SUBDIV_FRAME_COUNT];
} SCENE_SPRITE_SCRATCH;

typedef char scene_sprite_scratch_layout_check[offsetof(SCENE_SPRITE_SCRATCH, vertices) == SCENE_SPRITE_PROJECTION_OFFSET && offsetof(SCENE_SPRITE_SCRATCH, subdivision) == SCENE_SUBDIV_SCRATCH_OFFSET && offsetof(SCENE_SPRITE_SCRATCH, frames) == 0x270 && sizeof(SCENE_SPRITE_SCRATCH) == 984 ? 1 : -1];

static RESOURCE_ASSET_SOURCE scene_sprite_vertex_input(const SCENE_GEOMETRY_SPRITE *entry, uint32 address)
{
    RESOURCE_ASSET_SOURCE source = entry->vertex_source;
    uint32 offset;
    if (source.data && source.encoded_base == address && source.bytes >= sizeof(uint32))
        return source;
    source = entry->model_source;
    offset = address - source.encoded_base;
    if (!source.data || address < source.encoded_base || offset > source.bytes || sizeof(uint32) > source.bytes - offset)
        abort();
    source.data += offset;
    source.bytes -= offset;
    source.encoded_base = address;
    return source;
}

static SCENE_FACE_GROUP scene_sprite_face_input(RESOURCE_ASSET_SOURCE source, uint32 address, uint32 stride)
{
    uint32 offset = address - source.encoded_base;
    uint32 count;
    if (!source.data || address < source.encoded_base || offset > source.bytes || sizeof(count) > source.bytes - offset)
        abort();
    memcpy(&count, source.data + offset, sizeof(count));
    if ((sint32)count <= 0)
        return (SCENE_FACE_GROUP){NULL, count, stride};
    if (!stride || count > (source.bytes - offset - sizeof(count)) / stride || ((uintptr_t)(source.data + offset + sizeof(count)) & 3))
        abort();
    return (SCENE_FACE_GROUP){source.data + offset + sizeof(count), count, stride};
}

void scene_geometry_draw_sprites(const SCENE_GEOMETRY_SPRITE *entries, SCENE_RENDER_STREAM *stream)
{
    uint32 *scratch_words = getScratchAddr(0);
    SCENE_SPRITE_SCRATCH *work = (SCENE_SPRITE_SCRATCH *)scratch_words;
    SCENE_SPRITE_PROJECTION *native_projection = work->vertices;
    uint32 result = 0;
    const SCENE_SPRITE_MODEL_HEADER *object;
    uint32 source;
    sint32 x, y, z, count;
    FUNCTION_MARKER(0x800626E0, "ACE3.BIN");
    xport_gte_write_control(21, 0x830);
    xport_gte_write_control(22, 0x830);
    xport_gte_write_control(23, 0x830);
    object = entries->model_data;
    while (object != NULL)
    {
        x = entries->x;
        y = entries->y;
        z = entries->z;
        xport_gte_write_control(5, x);
        xport_gte_write_control(6, y);
        xport_gte_write_control(7, z);
        const SCENE_SPRITE_MODEL_HEADER *model = object;
        source = model->vertices;
        SCENE_SPRITE_PROJECTION_VIEW projection = {native_projection, 0, 0, NULL, 0};
        RESOURCE_ASSET_SOURCE vertices = scene_sprite_vertex_input(entries, source);
        const uint8 *vertex_data = vertices.data + sizeof(uint32);
        count = scene_geometry_read_vertex_count(vertices.data);
        uint32 next_count = (uint32)count - 3;
        uint32 triples = 1;
        if ((sint32)next_count > 0)
            triples += (next_count - 1) / 3 + 1;
        if (vertices.bytes < sizeof(uint32) || triples > SCENE_SPRITE_PROJECTION_CAPACITY / 3 || triples > (vertices.bytes - sizeof(uint32)) / SCENE_VERTEX_TRIPLE_BYTES)
            abort();
        SCENE_VERTEX_TRIPLE triple = scene_geometry_read_vertex_triple(vertex_data);
        do
        {
            xport_gte_write_data(0, triple.xy[0]);
            xport_gte_write_data(2, triple.xy[1]);
            xport_gte_write_data(4, triple.xy[2]);
            xport_gte_write_data(1, triple.z01);
            xport_gte_write_data(3, triple.z01 >> 16);
            xport_gte_write_data(5, triple.z2);
            vertex_data += SCENE_VERTEX_TRIPLE_BYTES;
            xport_gte_execute(0x280030);
            count = ((uint32)count - 3);
            if (count > 0)
                triple = scene_geometry_read_vertex_triple(vertex_data);
            if (projection.count > SCENE_SPRITE_PROJECTION_CAPACITY - 3)
                abort();
            SCENE_SPRITE_PROJECTION *projected = native_projection + projection.count;
            projected[0].screen.xy = xport_gte_read_data(12);
            projected[0].depth = xport_gte_read_data(17);
            projected[0].intensity = xport_gte_read_data(8);
            projected[1].screen.xy = xport_gte_read_data(13);
            projected[1].depth = xport_gte_read_data(18);
            projected[1].intensity = xport_gte_read_data(8);
            projected[2].screen.xy = xport_gte_read_data(14);
            projected[2].depth = xport_gte_read_data(19);
            projected[2].intensity = xport_gte_read_data(8);
            projection.count += 3;
        } while (count > 0);
        SCENE_SUBDIV_SCRATCH_HEADER *scratch = &work->subdivision;
        scratch->vertex_table = model->vertices + 4;
        SCENE_SUBDIV_INPUT subdivision = {0};
        subdivision.vertices = vertices;
        subdivision.vertices.data += sizeof(uint32);
        subdivision.vertices.bytes -= sizeof(uint32);
        subdivision.vertices.encoded_base += sizeof(uint32);
        RESOURCE_ASSET_SOURCE face_source = entries->model_source;
        result = model->gouraud_quads;
        ++entries;
        stream->primitives = result;
        subdivision.faces = scene_sprite_face_input(face_source, result, sizeof(SCENE_GOURAUD_QUAD_FACE));
        scene_subdiv_draw_gouraud_quad(scratch, stream, &subdivision, &projection);
        stream->primitives = model->flat_quads;
        subdivision.faces = scene_sprite_face_input(face_source, stream->primitives, sizeof(SCENE_FLAT_QUAD_FACE));
        result = scene_subdiv_draw_flat_quad(scratch, stream, &subdivision, &projection);
        object = entries->model_data;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_interpolate_meshes(SCENE_GEOMETRY_MESH *entries, SCENE_RENDER_STREAM *stream)
{
    uint32 result = entries->level;
    uint32 code, level, fraction, lo, hi, kind, offset, dst;
    FUNCTION_MARKER(0x80066DCC, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        if (entries->flags & 4)
        {
            code = entries->parameter;
            level = code >> 12;
            fraction = code & 0xFFF;
            if (level >= 5)
                level = 4;
            if (!entries->lod_model_headers || level * 4 >= entries->lod_header_count || !entries->lod_model_headers[level * 4])
                abort();
            entries->model_data = entries->lod_model_headers[level * 4];
            entries->model_source = (RESOURCE_ASSET_SOURCE){0};
            if (entries->lod_model_sources)
                entries->model_source = entries->lod_model_sources[level * 4];
            if (fraction != 0)
            {
                level = level < 4 ? level + 1 : 4;
                kind = entries->level;
                offset = kind == 0 ? 0 : kind == 1 ? 1 : 2;
                const SCENE_MESH_MODEL_HEADER *first = entries->model_data;
                if (!entries->lod_model_headers || level * 4 >= entries->lod_header_count || !entries->lod_model_headers[level * 4])
                    abort();
                const SCENE_MESH_MODEL_HEADER *second = entries->lod_model_headers[level * 4];
                const RESOURCE_ASSET_SOURCE *second_source = entries->lod_model_sources ? entries->lod_model_sources + level * 4 : NULL;
                lo = first->vertices[offset];
                hi = second->vertices[offset];
                entries->vertices_address = stream->packet;
                dst = stream->packet;
                result = scene_geometry_blend_vertices(fraction, scene_geometry_blend_source(lo, hi, 0, &entries->model_source, second_source), dst, NULL, NULL, NULL);
                stream->packet = result;
                entries->vertex_source = (RESOURCE_ASSET_SOURCE){mission_packet_span(dst, result - dst), result - dst, dst};
            }
        }
        result = entries[1].level;
        ++entries;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_draw_meshes(const SCENE_GEOMETRY_MESH *entries, SCENE_RENDER_STREAM *stream)
{
    SCENE_MESH_CONTEXT context;
    SCENE_MESH_PROJECTION projected;
    uint32 result = entries->level;
    uint32 c0, c1, c2, source, row, vertex;
    SCENE_GEOMETRY_CALLBACK target;
    sint32 count;
    FUNCTION_MARKER(0x80062BD4, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        context.level = result;
        c0 = entries->rotation.packed[0];
        c1 = entries->rotation.packed[1];
        xport_gte_write_control(0, c0);
        xport_gte_write_control(1, c1);
        c0 = entries->rotation.packed[2];
        c1 = entries->rotation.packed[3];
        c2 = entries->rotation.component[8];
        xport_gte_write_control(2, c0);
        xport_gte_write_control(3, c1);
        xport_gte_write_control(4, c2);
        c0 = entries->translation.word[0];
        c1 = entries->translation.word[1];
        xport_gte_write_control(5, c0);
        c2 = entries->translation.word[2];
        xport_gte_write_control(6, c1);
        xport_gte_write_control(7, c2);
        target = entries->callback;
        if (target != 0)
            target(&context, stream);
        vertex = 0;
        source = entries->vertices_address;
        row = source + 4;
        const RESOURCE_ASSET_SOURCE *vertices = &entries->vertex_source;
        const uint8 *vertex_data = vertices->data && vertices->encoded_base == source && vertices->bytes >= sizeof(uint32) ? vertices->data : NULL;
        count = scene_geometry_read_vertex_count(vertex_data ? vertex_data : mission_packet_span(source, sizeof(uint32)));
        if (vertex_data)
        {
            uint32 next_count = (uint32)count - 3;
            uint32 triples = 1;
            if ((sint32)next_count > 0)
                triples += (next_count - 1) / 3 + 1;
            if (triples > (vertices->bytes - sizeof(uint32)) / SCENE_VERTEX_TRIPLE_BYTES)
                abort();
        }
        SCENE_VERTEX_TRIPLE triple = scene_geometry_read_vertex_triple(vertex_data ? vertex_data + sizeof(uint32) : mission_packet_span(row, SCENE_VERTEX_TRIPLE_BYTES));
        do
        {
            xport_gte_write_data(0, triple.xy[0]);
            xport_gte_write_data(2, triple.xy[1]);
            xport_gte_write_data(4, triple.xy[2]);
            xport_gte_write_data(1, triple.z01);
            xport_gte_write_data(3, triple.z01 >> 16);
            xport_gte_write_data(5, triple.z2);
            row += SCENE_VERTEX_TRIPLE_BYTES;
            xport_gte_execute(0x280030);
            count = ((uint32)count - 3);
            if (count > 0)
                triple = scene_geometry_read_vertex_triple(vertex_data ? vertex_data + (uint32)(row - source) : mission_packet_span(row, SCENE_VERTEX_TRIPLE_BYTES));
            projected.xy[vertex].xy = xport_gte_read_data(12);
            projected.xy[vertex + 1].xy = xport_gte_read_data(13);
            projected.xy[vertex + 2].xy = xport_gte_read_data(14);
            projected.depth[vertex] = (uint16)xport_gte_read_data(17);
            projected.depth[vertex + 1] = (uint16)xport_gte_read_data(18);
            projected.depth[vertex + 2] = (uint16)xport_gte_read_data(19);
            vertex += 3;
        } while (count > 0);
        const SCENE_MESH_MODEL_HEADER *model = entries->model_data;
        stream->primitives = model->triangles;
        scene_geometry_emit_mesh_triangles(&context, stream, &projected, &entries->model_source);
        result = model->quads;
        stream->primitives = result;
        scene_geometry_emit_mesh_quads(&context, stream, &projected, &entries->model_source);
        result = entries[1].level;
        ++entries;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

SCENE_GEOMETRY_COMPOSITE *scene_geometry_composite_record(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index)
{
    if (index >= buffer->capacity)
        abort();
    return buffer->records + index;
}

uint32 scene_geometry_composite_level(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index)
{
    return scene_geometry_composite_record(buffer, index)->level;
}

void scene_geometry_end_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index)
{
    scene_geometry_composite_record(buffer, index)->level = 0xFFFFFFFF;
}

static RESOURCE_ASSET_SOURCE scene_mesh_vertex_input(RESOURCE_ASSET_SOURCE source, uint32 address)
{
    RESOURCE_ASSET_SOURCE missing = {0};
    uint32 offset = address - source.encoded_base, next_count, triples = 1;
    sint32 count;

    if (!source.data || address < source.encoded_base || offset > source.bytes || sizeof(uint32) > source.bytes - offset)
        return missing;
    count = scene_geometry_read_vertex_count(source.data + offset);
    next_count = (uint32)count - 3;
    if ((sint32)next_count > 0)
        triples += (next_count - 1) / 3 + 1;
    if (triples > (source.bytes - offset - sizeof(uint32)) / SCENE_VERTEX_TRIPLE_BYTES)
        return missing;
    source.data += offset;
    source.bytes -= offset;
    source.encoded_base = address;
    return source;
}

void scene_geometry_interpolate_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *entries, SCENE_RENDER_STREAM *stream)
{
    SCENE_VERTEX_BLEND_WORK workspace;
    const FLIGHT_CONTROL *descriptor;
    uint32 index = 0, result = scene_geometry_composite_level(entries, 0), dst;
    sint32 count, mode;
    FUNCTION_MARKER(0x80067088, "ACE3.BIN");
    while ((sint32)result >= 0)
    {
        SCENE_GEOMETRY_COMPOSITE *record = scene_geometry_composite_record(entries, index);
        dst = stream->packet;
        descriptor = record->effect;
        record->vertices_address[2] = dst;
        count = (sint16)descriptor->axis[0];
        mode = (sint16)descriptor->axis[1];
        dst = stream->packet;
        result = scene_geometry_blend_axes(count, mode, record->blend_vertices_address, dst, &workspace, &record->model_source);
        record->vertex_sources[2] = (RESOURCE_ASSET_SOURCE){mission_packet_span(dst, result - dst), result - dst, dst};
        stream->packet = result;
        record->vertices_address[3] = result;
        count = (sint16)descriptor->brake;
        dst = stream->packet;
        result = scene_geometry_blend_pair(count, record->blend_vertices_address + 5, dst, &record->model_source);
        record->vertex_sources[3] = (RESOURCE_ASSET_SOURCE){mission_packet_span(dst, result - dst), result - dst, dst};
        stream->packet = result;
        record->vertices_address[4] = result;
        count = (sint16)descriptor->axis[2];
        dst = stream->packet;
        result = scene_geometry_blend_signed_axis(count, record->blend_vertices_address + 7, dst, &record->model_source);
        record->vertex_sources[4] = (RESOURCE_ASSET_SOURCE){mission_packet_span(dst, result - dst), result - dst, dst};
        ++index;
        stream->packet = result;
        result = scene_geometry_composite_level(entries, index);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_draw_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *entries, uint16 ticks, SCENE_RENDER_STREAM *stream)
{
    SCENE_MESH_CONTEXT context;
    SCENE_MESH_PROJECTION projected;
    FLIGHT_CONTROL *effect;
    uint32 index = 0, result = scene_geometry_composite_level(entries, 0);
    uint32 packet, ot, old, depth, c0, c1, c2;
    SCENE_GEOMETRY_COMPOSITE_CALLBACK target;
    uint32 source, row, vertex, factor;
    sint32 i, count, light, f0, f1, f2;
    FUNCTION_MARKER(0x800637B0, "ACE3.BIN");
    context.ticks = ticks;
    while ((sint32)result >= 0)
    {
        const SCENE_GEOMETRY_COMPOSITE *record = scene_geometry_composite_record(entries, index);
        packet = stream->packet;
        stream->packet = packet + 156;
        uint32 *local_ot = mission_packet_span(packet, 39 * sizeof(*local_ot));
        ClearOTagR(local_ot, 39);
        depth = record->depth;
        context.depth_bias = (uint16)(depth);
        if (depth - 31 < 0x77D9)
        {
            ot = stream->ordering + (uint32)((sint32)depth >> 3) * 4;
            uint32 *parent_ot = mission_ot_span(ot, sizeof(*parent_ot));
            old = local_ot[0];
            old = (old & 0xFF000000) | (*parent_ot & 0xFFFFFF);
            local_ot[0] = old;
            old = *parent_ot;
            *parent_ot = (old & 0xFF000000) | ((packet + 152) & 0xFFFFFF);
        }
        result = record->level;
        context.local_ot = local_ot;
        context.level = result;
        c0 = record->rotation.packed[0];
        c1 = record->rotation.packed[1];
        xport_gte_write_control(0, c0);
        xport_gte_write_control(1, c1);
        c0 = record->rotation.packed[2];
        c1 = record->rotation.packed[3];
        c2 = record->rotation.component[8];
        xport_gte_write_control(2, c0);
        xport_gte_write_control(3, c1);
        xport_gte_write_control(4, c2);
        c0 = record->translation.word[0];
        c1 = record->translation.word[1];
        xport_gte_write_control(5, c0);
        c2 = record->translation.word[2];
        xport_gte_write_control(6, c1);
        xport_gte_write_control(7, c2);
        c0 = record->light_rotation.packed[0];
        c1 = record->light_rotation.packed[1];
        xport_gte_write_control(8, c0);
        xport_gte_write_control(9, c1);
        c0 = record->light_rotation.packed[2];
        c1 = record->light_rotation.packed[3];
        c2 = record->light_rotation.component[8];
        xport_gte_write_control(10, c0);
        xport_gte_write_control(11, c1);
        xport_gte_write_control(12, c2);
        effect = record->effect;
        if (effect != 0)
        {
            target = record->callback;
            if (target != 0)
                target(&context, effect, stream);
        }
        for (i = 0; i < 5; ++i)
        {
            vertex = 0;
            source = record->vertices_address[i];
            const SCENE_MESH_MODEL_HEADER *model = record->models_data[i];
            row = source + 4;
            RESOURCE_ASSET_SOURCE input = scene_mesh_vertex_input(record->vertex_sources[i], source);
            count = scene_geometry_read_vertex_count(input.data ? input.data : mission_packet_span(source, sizeof(uint32)));
            do
            {
                SCENE_VERTEX_TRIPLE triple = scene_geometry_read_vertex_triple(input.data ? input.data + (uint32)(row - source) : mission_packet_span(row, SCENE_VERTEX_TRIPLE_BYTES));
                xport_gte_write_data(0, triple.xy[0]);
                xport_gte_write_data(2, triple.xy[1]);
                xport_gte_write_data(4, triple.xy[2]);
                xport_gte_write_data(5, triple.z2);
                xport_gte_write_data(1, triple.z01);
                xport_gte_write_data(3, (sint32)triple.z01 >> 16);
                xport_gte_execute(0x280030);
                row += SCENE_VERTEX_TRIPLE_BYTES;
                count = ((uint32)count - 3);
                projected.xy[vertex].xy = xport_gte_read_data(12);
                projected.xy[vertex + 1].xy = xport_gte_read_data(13);
                projected.xy[vertex + 2].xy = xport_gte_read_data(14);
                projected.depth[vertex] = (uint16)xport_gte_read_data(17);
                projected.depth[vertex + 1] = (uint16)xport_gte_read_data(18);
                projected.depth[vertex + 2] = (uint16)xport_gte_read_data(19);
                vertex += 3;
            } while (count > 0);
            if (i != 0)
            {
                const RESOURCE_ASSET_SOURCE *normal_owner = &record->model_source;
                context.normal_table.view = (GAME_SCENE_NORMAL_DATA){normal_owner->data, normal_owner->bytes, normal_owner->encoded_base};
                context.normal_table.offset = model->normals - normal_owner->encoded_base;
                stream->primitives = model->triangles;
                scene_geometry_emit_lit_triangles(&context, stream, &projected, &record->model_source);
                stream->primitives = model->quads;
                scene_geometry_emit_lit_quads(&context, stream, &projected, &record->model_source);
            }
            else
            {
                f0 = (sint32)xport_gte_read_control(13);
                f1 = (sint32)xport_gte_read_control(14);
                f2 = (sint32)xport_gte_read_control(15);
                xport_gte_write_control(21, f0 >> 2);
                xport_gte_write_control(22, f1 >> 2);
                xport_gte_write_control(23, f2 >> 2);
                effect = record->effect;
                factor = 0xE00;
                if (effect != 0)
                {
                    light = (sint16)effect->thrust;
                    if (light < 0x800)
                    {
                        factor = 0x1000 - (uint32)(light >> 2);
                    }
                    else
                    {
                        light -= 0x800;
                        factor = 0xE00 - (((uint32)light << 1) - (uint32)(light >> 2));
                    }
                    if (context.ticks & 1)
                        factor = factor + 8 + (uint32)((sint32)factor >> 5);
                }
                xport_gte_write_data(8, factor);
                stream->primitives = model->triangles;
                scene_geometry_emit_prelit_triangles(&context, stream, &projected, &record->model_source);
                stream->primitives = model->quads;
                scene_geometry_emit_prelit_quads(&context, stream, &projected, &record->model_source);
            }
        }
        ++index;
        result = scene_geometry_composite_level(entries, index);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_geometry_blend_axes(sint32 count, sint32 mode, const uint32 source[5], uint32 dst, SCENE_VERTEX_BLEND_WORK *workspace, const RESOURCE_ASSET_SOURCE *parent)
{
    uint32 base, first, second, total, product, factor;
    sint32 numerator, denominator, quotient, ratio, weight;
    FUNCTION_MARKER(0x80066EF8, "ACE3.BIN");
    base = source[0];
    if (count < 0)
    {
        first = source[2];
        count = (0 - (uint32)count);
    }
    else
    {
        first = source[1];
    }
    if (mode < 0)
    {
        second = source[4];
        mode = (0 - (uint32)mode);
    }
    else
    {
        second = source[3];
    }
    if (count == mode)
    {
        ratio = 0x800;
    }
    else
    {
        numerator = ((uint32)(mode < count ? mode : count) << 11);
        denominator = mode < count ? count : mode;
        if (denominator == 0)
            xport_mips_break(7);
        if (denominator == -1 && numerator == (sint32)0x80000000)
            xport_mips_break(6);
        quotient = ((sint64)numerator / denominator);
        ratio = mode < count ? quotient : (sint32)(0x1000 - (uint32)quotient);
    }
    total = (uint32)count + (uint32)mode;
    factor = ratio < 0x800 ? 0x1000 - (uint32)ratio : (uint32)ratio;
    product = total * factor;
    weight = (sint32)product >> 12;
    if (weight >= 0x1001)
        weight = 0x1000;
    return scene_geometry_blend_axes_vertices(ratio, weight, base, first, second, dst, workspace, parent);
}

uint32 scene_geometry_blend_pair(sint32 count, const uint32 source[2], uint32 dst, const RESOURCE_ASSET_SOURCE *parent)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8006701C, "ACE3.BIN");
    first = source[0];
    second = source[1];
    return scene_geometry_blend_vertices(count, scene_geometry_blend_source(first, second, 0, parent, parent), dst, NULL, NULL, NULL);
}

uint32 scene_geometry_blend_signed_axis(sint32 count, const uint32 source[3], uint32 dst, const RESOURCE_ASSET_SOURCE *parent)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8006704C, "ACE3.BIN");
    if (count < 0)
    {
        second = source[2];
        count = (0 - (uint32)count);
    }
    else
    {
        second = source[1];
    }
    first = source[0];
    return scene_geometry_blend_vertices(count, scene_geometry_blend_source(first, second, 0, parent, parent), dst, NULL, NULL, NULL);
}

uint32 scene_geometry_draw_mesh_style_1(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream)
{
    XY center;
    POLY_FT4 *poly;
    uint32 *ot_entry;
    uint32 packet, value, depth, screen_height, shift, numerator, size, bucket_offset, ordering, head, half, result;
    FUNCTION_MARKER(0x8008D4E8, "ACE3.BIN");
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xFFFFFFE2);
    packet = stream->packet;
    xport_gte_execute(0x180001);
    poly = (POLY_FT4 *)mission_packet_span(packet, sizeof(*poly));
    poly->p[1].uv_attr = 0x0037203F;
    poly->p[2].uv.uv = (uint16)(0x3F20);
    poly->p[0].uv_attr = 0x46562020;
    poly->p[3].uv.uv = (uint16)(0x3F3F);
    poly->rgbc.rgbc = 0x2E838383;
    value = xport_gte_read_data(14);
    center.xy = value;
    depth = xport_gte_read_data(19);
    screen_height = xport_gte_read_control(26);
    shift = state->level;
    depth = (sint32)depth >> (shift & 31);
    numerator = screen_height << 3;
    if (depth - 31 >= 0x77D9)
        return numerator;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)depth;
    if ((sint32)size < 3)
        size = 3;
    bucket_offset = (uint32)((sint32)depth >> 3) << 2;
    ordering = stream->ordering;
    ot_entry = (uint32 *)mission_ot_span(ordering + bucket_offset, sizeof(*ot_entry));
    head = *ot_entry;
    poly->tag = head | 0x09000000;
    ordering = stream->ordering;
    ot_entry = (uint32 *)mission_ot_span(ordering + bucket_offset, sizeof(*ot_entry));
    *ot_entry = packet & 0xFFFFFF;
    stream->packet = packet + 40;
    value = (uint16)center.x;
    half = (sint32)size >> 1;
    center.x = (sint16)(uint16)(value - half);
    value = (uint16)center.y;
    center.y = (sint16)(uint16)(value - half);
    value = (uint16)center.x;
    poly->p[0].xy.x = (sint16)(uint16)(value);
    value = (uint16)center.y;
    poly->p[0].xy.y = (sint16)(uint16)(value);
    value = (uint16)center.x;
    poly->p[1].xy.x = (sint16)(uint16)(value + size);
    value = (uint16)center.y;
    poly->p[1].xy.y = (sint16)(uint16)(value);
    value = (uint16)center.x;
    poly->p[2].xy.x = (sint16)(uint16)(value);
    value = (uint16)center.y;
    poly->p[2].xy.y = (sint16)(uint16)(value + size);
    value = (uint16)center.x;
    poly->p[3].xy.x = (sint16)(uint16)(value + size);
    value = (uint16)center.y;
    result = value + size;
    poly->p[3].xy.y = (sint16)(uint16)(result);
    return result;
}

typedef struct
{
    POLY_G4 first;
    POINT_C second[4];
} SCENE_GLOW_PAIR_PACKET;

typedef struct
{
    SCENE_GLOW_PAIR_PACKET pair[3];
} SCENE_GLOW_PACKET_BATCH;

typedef char scene_glow_packet_batch_layout_check[sizeof(POLY_G4) == 36 && sizeof(POINT_C) == 8 && offsetof(SCENE_GLOW_PAIR_PACKET, second) == 36 && sizeof(SCENE_GLOW_PAIR_PACKET) == 68 && sizeof(SCENE_GLOW_PACKET_BATCH) == 204 ? 1 : -1];

typedef struct
{
    XY center;
    uint32 width;
    uint32 base_color;
    uint32 shaded_color;
} SCENE_MESH_GLOW_WORK;

uint32 scene_geometry_draw_mesh_style_2(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream)
{
    SCENE_MESH_GLOW_WORK work;
    SCENE_GLOW_PACKET_BATCH *batch;
    POLY_G4 *poly[3], *poly0;
    POINT_C *points1;
    uint32 *ot_entry;
    uint8 *link_bytes;
    uint32 packet, cursor, value, color, screen, depth, scale, shift, distance, numerator, size, ordering, table, tag, index, width, thin, half, quarter;
    FUNCTION_MARKER(0x8008D678, "ACE3.BIN");
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x00C04343);
    xport_gte_write_data(8, 1118);
    packet = stream->packet;
    xport_gte_execute(0x780010);
    batch = mission_packet_span(packet, sizeof(*batch));
    poly[0] = &batch->pair[0].first;
    poly[1] = &batch->pair[1].first;
    poly[2] = &batch->pair[2].first;
    poly[1]->tag = ((packet + 136) & 0xFFFFFF) | 0x10000000;
    poly[0]->p[0].rgbc.rgbc = 0x3A010101;
    poly[1]->p[0].rgbc.rgbc = 0x3A010101;
    poly[2]->tag = ((packet + 204) & 0xFFFFFF) | 0x10000000;
    poly[2]->p[0].rgbc.rgbc = 0x3A010101;
    poly[0]->tag = ((packet + 68) & 0xFFFFFF) | 0x10000000;
    work.base_color = 0x00C04343;
    color = xport_gte_read_data(22);
    work.shaded_color = color;
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, -32);
    xport_gte_execute(0x180001);
    cursor = packet;
    index = 0;
    do
    {
        poly0 = &batch->pair[index].first;
        points1 = batch->pair[index].second;
        points1[3].rgbc.rgbc = 0x3A010101;
        points1[2].rgbc.rgbc = 0x3A010101;
        points1[0].rgbc.rgbc = 0x3A010101;
        poly0->p[3].rgbc.rgbc = 0x3A010101;
        poly0->p[2].rgbc.rgbc = 0x3A010101;
        index += 1;
        cursor += 68;
    } while (index < 3);
    screen = xport_gte_read_data(14);
    work.center.xy = screen;
    depth = xport_gte_read_data(19);
    scale = xport_gte_read_control(26);
    shift = state->level;
    distance = (sint32)depth >> (shift & 31);
    numerator = scale << 4;
    if (distance - 31 >= 0x77D9)
        return numerator;
    numerator = (numerator - scale) << 1;
    if (distance == 0)
        xport_mips_break(7);
    if (distance == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)distance;
    work.width = size;
    table = stream->ordering;
    ordering = table + ((uint32)((sint32)distance >> 3) << 2);
    ot_entry = (uint32 *)mission_ot_span(ordering, sizeof(*ot_entry));
    tag = *ot_entry & 0xFFFFFF;
    link_bytes = (uint8 *)&poly[2]->tag;
    link_bytes[0] = tag;
    link_bytes[1] = (tag >> 8);
    link_bytes[2] = (tag >> 16);
    link_bytes = (uint8 *)ot_entry;
    link_bytes[0] = packet;
    link_bytes[1] = (packet >> 8);
    link_bytes[2] = (packet >> 16);
    stream->packet = packet + 204;
    value = work.width;
    if ((sint32)value < 8)
        work.width = 8;
    cursor = packet;
    index = 0;
    screen = work.center.xy;
    color = work.shaded_color;
    do
    {
        poly0 = &batch->pair[index].first;
        points1 = batch->pair[index].second;
        points1[1].xy.xy = screen;
        poly0->p[1].xy.xy = screen;
        points1[1].rgbc.rgbc = color;
        poly0->p[1].rgbc.rgbc = color;
        if (index == 1)
            color = work.base_color;
        index += 1;
        cursor += 68;
    } while (index < 3);
    width = work.width;
    thin = (sint32)width >> 4;
    if (thin == 0)
        thin = 1;
    poly0 = &batch->pair[0].first;
    points1 = batch->pair[0].second;
    value = (uint32)(uint16)work.center.x - width;
    points1[0].xy.x = (sint16)(uint16)(value);
    poly0->p[0].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y - width;
    points1[0].xy.y = (sint16)(uint16)(value);
    poly0->p[0].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x + width;
    points1[3].xy.x = (sint16)(uint16)(value);
    poly0->p[3].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y + width;
    points1[3].xy.y = (sint16)(uint16)(value);
    poly0->p[3].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x + thin;
    poly0->p[2].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y - thin;
    poly0->p[2].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x - thin;
    points1[2].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y + thin;
    points1[2].xy.y = (sint16)(uint16)(value);
    packet += 68;
    poly0 = &batch->pair[1].first;
    points1 = batch->pair[1].second;
    width = work.width;
    half = (sint32)width >> 1;
    value = (uint32)(uint16)work.center.x + half;
    points1[0].xy.x = (sint16)(uint16)(value);
    poly0->p[0].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y - half;
    points1[0].xy.y = (sint16)(uint16)(value);
    poly0->p[0].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x - half;
    points1[3].xy.x = (sint16)(uint16)(value);
    poly0->p[3].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y + half;
    points1[3].xy.y = (sint16)(uint16)(value);
    poly0->p[3].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x - thin;
    poly0->p[2].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y - thin;
    poly0->p[2].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x + thin;
    points1[2].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y + thin;
    points1[2].xy.y = (sint16)(uint16)(value);
    width = work.width;
    value = (uint16)work.center.x;
    packet += 68;
    poly0 = &batch->pair[2].first;
    points1 = batch->pair[2].second;
    points1[2].xy.x = (sint16)(uint16)(value);
    poly0->p[2].xy.x = (sint16)(uint16)(value);
    value = (uint16)work.center.x;
    quarter = (sint32)width >> 2;
    value -= quarter;
    points1[0].xy.x = (sint16)(uint16)(value);
    poly0->p[0].xy.x = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.x + quarter;
    points1[3].xy.x = (sint16)(uint16)(value);
    poly0->p[3].xy.x = (sint16)(uint16)(value);
    value = (uint16)work.center.y;
    points1[3].xy.y = (sint16)(uint16)(value);
    points1[0].xy.y = (sint16)(uint16)(value);
    poly0->p[3].xy.y = (sint16)(uint16)(value);
    poly0->p[0].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y - quarter;
    poly0->p[2].xy.y = (sint16)(uint16)(value);
    value = (uint32)(uint16)work.center.y + quarter;
    points1[2].xy.y = (sint16)(uint16)(value);
    return value;
}

typedef struct
{
    uint32 saved_control[8];
    SVECTOR origin;
    uint32 width;
    uint32 phase;
    MATRIX rotation;
    uint32 bucket;
    XY center;
    SVECTOR vertex[3];
} SCENE_RING_WORK;

static uint32 scene_geometry_prepare_ring(SCENE_RING_WORK *work, uint32 shift, uint32 scale)
{
    uint32 x, y, z, angle, radius, packed, sine, cosine, a, b, numerator, depth, distance, size, screen, tx, ty;
    FUNCTION_MARKER(0x8008CD80, "ACE3.BIN");
    x = (sint16)work->origin.vx;
    z = (sint16)work->origin.vz;
    x <<= shift & 31;
    y = (sint16)work->origin.vy;
    z <<= shift & 31;
    y <<= shift & 31;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    angle = work->phase;
    radius = work->width;
    xport_gte_execute(0x180001);
    packed = ((uint32)(uint16)rsin((((angle & 0xFFF) << 2)) >> 2) | ((uint32)(uint16)rcos((((angle & 0xFFF) << 2)) >> 2) << 16));
    radius <<= shift & 31;
    cosine = (sint32)packed >> 16;
    sine = (sint32)(packed << 16) >> 16;
    a = sine * radius;
    b = cosine * radius;
    numerator = scale * radius;
    work->rotation.m[1][1] = (sint16)(uint16)(radius);
    sine = (sint32)a >> 12;
    cosine = (sint32)b >> 12;
    depth = xport_gte_read_data(27);
    distance = (sint32)depth >> (shift & 31);
    if (distance - 31 >= 0x77D9)
        return 0;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    size = (sint32)numerator / (sint32)depth;
    if ((sint32)size < 3)
        return 0;
    screen = xport_gte_read_data(14);
    work->center.xy = screen;
    work->bucket = (sint32)distance >> 3;
    work->rotation.m[0][2] = (sint16)(uint16)(sine);
    work->rotation.m[2][0] = (sint16)(uint16)(0 - sine);
    work->rotation.m[2][2] = (sint16)(uint16)(cosine);
    work->rotation.m[0][0] = (sint16)(uint16)(cosine);
    work->rotation.m[2][1] = (sint16)(uint16)(0);
    work->rotation.m[1][2] = (sint16)(uint16)(0);
    work->rotation.m[1][0] = (sint16)(uint16)(0);
    work->rotation.m[0][1] = (sint16)(uint16)(0);
    tx = xport_gte_read_data(25);
    ty = xport_gte_read_data(26);
    game_compose_rotation(&work->rotation);
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, depth);
    return size;
}

typedef char scene_aux_packet_layout_check[sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(DR_MOVE) == 24 && offsetof(DR_MOVE, code) == 4 && sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && offsetof(POLY_FT4, p) + 3 * sizeof(POINT_T) + offsetof(POINT_T, attr) == 38 ? 1 : -1];

static uint32 scene_geometry_draw_ring(SCENE_RING_WORK *work, uint32 count, uint32 angle, uint32 prior_result, SCENE_RENDER_STREAM *stream)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    uint32 packet, cursor, center, table, phase, index, value, step, next, ordering, tag, head;
    FUNCTION_MARKER(0x8008CEFC, "ACE3.BIN");
    if ((sint32)count <= 0)
        return prior_result;
    phase = 0;
    index = 0;
    packet = stream->packet;
    cursor = packet + 8;
    DR_TPAGE *mode = mission_packet_span(packet, sizeof(*mode));
    mode->code[0] = (uint32)(0xE1000240);
    mode->tag = (uint32)((cursor & 0xFFFFFF) | 0x01000000);
    center = work->center.xy;
    work->vertex[0].vx = 0;
    work->vertex[0].vy = 0;
    work->vertex[0].vz = (sint16)(uint16)(4096);
    value = ((uint32)(uint16)rsin((((angle << 1) & 0x3FFC)) >> 2) | ((uint32)(uint16)rcos((((angle << 1) & 0x3FFC)) >> 2) << 16));
    work->vertex[1].vy = (sint16)(uint16)(64);
    work->vertex[1].vz = (sint16)(uint16)((sint32)value >> 16);
    work->vertex[1].vx = (sint16)(uint16)(value);
    value = ((uint32)(uint16)rsin((((angle & 0xFFF) << 2)) >> 2) | ((uint32)(uint16)rcos((((angle & 0xFFF) << 2)) >> 2) << 16));
    work->vertex[2].vy = (sint16)(uint16)(150);
    work->vertex[2].vz = (sint16)(uint16)((sint32)value >> 16);
    step = 4096 / (sint32)count;
    work->vertex[2].vx = (sint16)(uint16)(value);
    do
    {
        POLY_G4 *ring = mission_packet_span(cursor, sizeof(*ring));
        value = ((uint32)(uint16)work->vertex[0].vx | ((uint32)(uint16)work->vertex[0].vy << 16));
        xport_gte_write_data(0, value);
        value = (uint32)(uint16)work->vertex[0].vz;
        xport_gte_write_data(1, value);
        value = ((uint32)(uint16)work->vertex[1].vx | ((uint32)(uint16)work->vertex[1].vy << 16));
        xport_gte_write_data(2, value);
        value = (uint32)(uint16)work->vertex[1].vz;
        xport_gte_write_data(3, value);
        value = ((uint32)(uint16)work->vertex[2].vx | ((uint32)(uint16)work->vertex[2].vy << 16));
        xport_gte_write_data(4, value);
        value = (uint32)(uint16)work->vertex[2].vz;
        xport_gte_write_data(5, value);
        phase += step;
        ring->p[2].xy.xy = (uint32)(center);
        xport_gte_execute(0x280030);
        value = ((uint32)(uint16)rsin((((phase & 0xFFF) << 2)) >> 2) | ((uint32)(uint16)rcos((((phase & 0xFFF) << 2)) >> 2) << 16));
        work->vertex[0].vz = (sint16)(uint16)((sint32)value >> 16);
        work->vertex[0].vx = (sint16)(uint16)(value);
        value = ((uint32)(uint16)rsin(((((phase + (uint32)((sint32)angle >> 1)) & 0xFFF) << 2)) >> 2) | ((uint32)(uint16)rcos(((((phase + (uint32)((sint32)angle >> 1)) & 0xFFF) << 2)) >> 2) << 16));
        next = cursor + 36;
        work->vertex[1].vz = (sint16)(uint16)((sint32)value >> 16);
        work->vertex[1].vx = (sint16)(uint16)(value);
        value = ((uint32)(uint16)rsin(((((phase + angle) & 0xFFF) << 2)) >> 2) | ((uint32)(uint16)rcos(((((phase + angle) & 0xFFF) << 2)) >> 2) << 16));
        work->vertex[2].vz = (sint16)(uint16)((sint32)value >> 16);
        work->vertex[2].vx = (sint16)(uint16)(value);
        ring->tag = (uint32)((next & 0xFFFFFF) | 0x08000000);
        ring->p[0].rgbc.rgbc = (uint32)(0x3A242828);
        ring->p[1].rgbc.rgbc = (uint32)(0x001C1F1F);
        ring->p[2].rgbc.rgbc = (uint32)(0x00050707);
        ring->p[3].rgbc.rgbc = (uint32)(0x00010303);
        value = xport_gte_read_data(12);
        ring->p[0].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        ring->p[1].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        ring->p[3].xy.xy = (uint32)(value);
        index += 1;
        cursor = next;
    } while ((sint32)index < (sint32)count);
    DR_TPAGE *restore = mission_packet_span(cursor, sizeof(*restore));
    restore->tag = (uint32)(0x01000000);
    restore->code[0] = (uint32)(0xE1000220);
    ordering = work->bucket;
    table = stream->ordering;
    ordering = table + (ordering << 2);
    head = stream->packet;
    ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
    tag = *ordering_word;
    packet_link = mission_packet_span(cursor, sizeof(*packet_link));
    packet_link->addr = tag;
    ((P_TAG *)ordering_word)->addr = head & 0xFFFFFF;
    next = cursor + 8;
    stream->packet = next;
    return next;
}

uint32 scene_geometry_draw_mesh_style_3(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream)
{
    SCENE_RING_WORK work;
    uint32 ticks, a, b, c, shift, scale, result;
    FUNCTION_MARKER(0x8008D140, "ACE3.BIN");
    ticks = game_runtime.gameplay_counter;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    work.saved_control[0] = a;
    work.saved_control[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    work.saved_control[2] = a;
    work.saved_control[3] = b;
    work.saved_control[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    work.saved_control[5] = a;
    work.saved_control[6] = b;
    work.saved_control[7] = c;
    work.origin.vy = (sint16)(uint16)(0xFFFF);
    work.origin.vz = (sint16)(uint16)(4);
    work.origin.vx = (sint16)(uint16)(0);
    work.width = 40;
    work.phase = (ticks << 9) + ticks;
    shift = object->level;
    scale = xport_gte_read_control(26);
    result = scene_geometry_prepare_ring(&work, shift, scale);
    if (result == 0)
        return result;
    result = scene_geometry_draw_ring(&work, 3, 341, result, stream);
    a = work.saved_control[0];
    b = work.saved_control[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = work.saved_control[2];
    b = work.saved_control[3];
    c = work.saved_control[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = work.saved_control[5];
    b = work.saved_control[6];
    xport_gte_write_control(5, a);
    c = work.saved_control[7];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

uint32 scene_geometry_draw_mesh_style_4(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream)
{
    SCENE_RING_WORK work;
    uint32 ticks, a, b, c, shift, scale, result;
    FUNCTION_MARKER(0x8008D244, "ACE3.BIN");
    ticks = game_runtime.gameplay_counter;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    work.saved_control[0] = a;
    work.saved_control[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    work.saved_control[2] = a;
    work.saved_control[3] = b;
    work.saved_control[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    work.saved_control[5] = a;
    work.saved_control[6] = b;
    work.saved_control[7] = c;
    work.origin.vy = (sint16)(uint16)(0xFFFF);
    work.origin.vz = (sint16)(uint16)(2);
    work.origin.vx = (sint16)(uint16)(0);
    work.width = 36;
    work.phase = (ticks << 9) + ticks;
    shift = object->level;
    scale = xport_gte_read_control(26);
    result = scene_geometry_prepare_ring(&work, shift, scale);
    if (result == 0)
        return result;
    result = scene_geometry_draw_ring(&work, 5, 318, result, stream);
    a = work.saved_control[0];
    b = work.saved_control[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = work.saved_control[2];
    b = work.saved_control[3];
    c = work.saved_control[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = work.saved_control[5];
    b = work.saved_control[6];
    xport_gte_write_control(5, a);
    c = work.saved_control[7];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

uint32 scene_geometry_draw_mesh_style_5(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream)
{
    SCENE_RING_WORK work;
    uint32 ticks, scale, shift, a, b, c, value, result;
    FUNCTION_MARKER(0x8008D348, "ACE3.BIN");
    ticks = game_runtime.gameplay_counter;
    scale = xport_gte_read_control(26);
    shift = object->level;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    work.saved_control[0] = a;
    work.saved_control[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    work.saved_control[2] = a;
    work.saved_control[3] = b;
    work.saved_control[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    work.saved_control[5] = a;
    work.saved_control[6] = b;
    work.saved_control[7] = c;
    work.origin.vx = (sint16)(uint16)(40);
    work.origin.vy = (sint16)(uint16)(-15);
    work.origin.vz = (sint16)(uint16)(0);
    work.width = 37;
    work.phase = (ticks << 9) - ticks;
    result = scene_geometry_prepare_ring(&work, shift, scale);
    if (result == 0)
        return result;
    result = scene_geometry_draw_ring(&work, 3, 0 - (uint32)318, result, stream);
    a = work.saved_control[0];
    b = work.saved_control[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = work.saved_control[2];
    b = work.saved_control[3];
    c = work.saved_control[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = work.saved_control[5];
    b = work.saved_control[6];
    xport_gte_write_control(5, a);
    c = work.saved_control[7];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    value = (uint16)work.origin.vx;
    c = work.phase;
    work.origin.vx = (sint16)(uint16)(0 - value);
    work.phase = 1000 - c;
    result = scene_geometry_prepare_ring(&work, shift, scale);
    if (result == 0)
        return result;
    result = scene_geometry_draw_ring(&work, 3, 318, result, stream);
    a = work.saved_control[0];
    b = work.saved_control[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = work.saved_control[2];
    b = work.saved_control[3];
    c = work.saved_control[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = work.saved_control[5];
    b = work.saved_control[6];
    xport_gte_write_control(5, a);
    c = work.saved_control[7];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return result;
}

typedef struct
{
    XY xy;
    uint16 z;
} SCENE_COMPOSITE_VECTOR;

typedef struct
{
    uint32 count;
    uint32 tick;
    uint32 axes;
    uint32 emitter_bits;
    uint32 active_mask;
    SCENE_COMPOSITE_VECTOR direction;
    SCENE_COMPOSITE_VECTOR color;
    SCENE_COMPOSITE_VECTOR blended;
    SCENE_COMPOSITE_VECTOR edge[2];
    uint32 bucket;
    XY spread;
    XY extent;
    uint32 flags;
    XY jitter;
    XY screen;
} SCENE_COMPOSITE_THRUST_WORK;

typedef struct
{
    DR_MOVE copy;
    RGBC rgbc;
    POINT_T sprite;
    XY size;
} SCENE_COMPOSITE_SEGMENT_PACKET;

typedef char scene_composite_packet_layout_check[sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POLY_G3) == 28 && offsetof(POLY_G3, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(DR_MODE) == 12 && offsetof(DR_MODE, code) == 4 && sizeof(SCENE_COMPOSITE_SEGMENT_PACKET) == 40 && offsetof(SCENE_COMPOSITE_SEGMENT_PACKET, rgbc) == 24 && offsetof(SCENE_COMPOSITE_SEGMENT_PACKET, sprite) == 28 && offsetof(SCENE_COMPOSITE_SEGMENT_PACKET, size) == 36 ? 1 : -1];

static void scene_geometry_draw_composite_segments(const SCENE_MESH_CONTEXT *state, SCENE_COMPOSITE_THRUST_WORK *effect, uint32 phase_origin, SCENE_RENDER_STREAM *stream);

void scene_geometry_draw_composite_variant_1(const SCENE_MESH_CONTEXT *state, FLIGHT_CONTROL *record, SCENE_RENDER_STREAM *stream)
{
    const AIRCRAFT_THRUST_METADATA *metadata = &game_runtime.aircraft_thrust_metadata;
    SCENE_COMPOSITE_THRUST_WORK work;
    uint32 packed, count, result, strength, value, color, i, a, b, c, input, component, product, packet, seed, depth, bucket, start, shift;
    uint32 *ordering;
    FUNCTION_MARKER(0x8008C0A0, "ACE3.BIN");
    count = metadata->count;
    if (count == 0)
        return;
    if (count >= 3)
        count = 2;
    work.count = count;
    strength = (sint16)record->thrust;
    if ((sint32)strength <= 0)
        return;
    if ((sint32)strength > 4096)
        strength = 4096;
    input = 0;
    if (metadata->uses_axes != 0)
        input = ((uint32)record->axis[0] | ((uint32)record->axis[1] << 16));
    work.axes = input;
    value = game_runtime.gameplay_counter;
    work.tick = value;
    if ((value & 1) != 0)
        color = 0x333333;
    else
    {
        xport_gte_write_data(8, strength);
        value = metadata->base_rgbc;
        xport_gte_write_data(6, value);
        packed = metadata->far_color;
        xport_gte_write_control(21, (packed & 255) << 4);
        packed = (sint32)packed >> 8;
        xport_gte_write_control(22, (packed & 255) << 4);
        xport_gte_write_control(23, ((uint32)((sint32)packed >> 4)) & 0xFF0);
        xport_gte_execute(0x780010);
        color = xport_gte_read_data(22) & 0xFFFFFF;
    }
    value = game_runtime.flags;
    work.flags = value & 0x20000000;
    packed = record->emitter_bits;
    value = work.flags;
    work.emitter_bits = packed;
    if (value == 0)
        work.emitter_bits = packed << 2;
    value = metadata->extent_xy;
    work.extent.xy = value;
    for (i = 0; i < count; ++i)
    {
        const AIRCRAFT_THRUST_EMITTER *emitter = &metadata->emitters[i];
        work.direction.xy.xy = emitter->direction_xy;
        work.direction.z = emitter->direction_z;
        work.color.xy.xy = emitter->color_xy;
        work.color.z = emitter->color_z;
        input = work.axes;
        if (input != 0)
        {
            component = (sint16)input;
            if (component != 0)
            {
                value = (sint32)component < 0 ? emitter->axis_low[0] : emitter->axis_high[0];
                product = value * ((sint32)component < 0 ? 0 - component : component);
                value = (uint32)(uint16)work.color.xy.y;
                work.color.xy.y = (sint16)(uint16)(value + (uint32)((sint32)product >> 12));
            }
            component = (sint32)input >> 16;
            if (component != 0)
            {
                value = (sint32)component < 0 ? emitter->axis_low[1] : emitter->axis_high[1];
                product = value * ((sint32)component < 0 ? 0 - component : component);
                value = (uint32)(uint16)work.color.xy.y;
                work.color.xy.y = (sint16)(uint16)(value + (uint32)((sint32)product >> 12));
            }
        }
        value = (sint32)(sint16)work.direction.xy.x;
        xport_gte_write_data(9, value);
        value = (sint32)(sint16)work.direction.xy.y;
        xport_gte_write_data(10, value);
        value = (sint32)(sint16)work.direction.z;
        xport_gte_write_data(11, value);
        value = (sint32)(sint16)work.color.xy.x;
        xport_gte_write_control(21, value);
        value = (sint32)(sint16)work.color.xy.y;
        xport_gte_write_control(22, value);
        value = (sint32)(sint16)work.color.z;
        xport_gte_write_control(23, value);
        xport_gte_write_data(8, strength);
        xport_gte_execute(0x980011);
        work.edge[0].xy.xy = emitter->edge_xy[0];
        work.edge[0].z = emitter->edge_z[0];
        work.edge[1].xy.xy = emitter->edge_xy[1];
        work.edge[1].z = emitter->edge_z[1];
        value = xport_gte_read_data(25);
        work.blended.xy.x = (sint16)(uint16)(value);
        value = xport_gte_read_data(26);
        work.blended.xy.y = (sint16)(uint16)(value);
        value = xport_gte_read_data(27);
        work.blended.z = (uint16)(value);
        value = work.edge[0].xy.xy;
        xport_gte_write_data(0, value);
        value = (uint32)(uint16)work.edge[0].z;
        xport_gte_write_data(1, value);
        value = work.edge[1].xy.xy;
        xport_gte_write_data(2, value);
        value = (uint32)(uint16)work.edge[1].z;
        xport_gte_write_data(3, value);
        value = emitter->tip_xy[0];
        xport_gte_write_data(4, value);
        value = emitter->tip_z_word[0];
        xport_gte_write_data(5, value);
        packet = stream->packet;
        POLY_G4 *quads = mission_packet_span(packet, 2 * sizeof(*quads));
        quads[0].p[2].rgbc.rgbc = color;
        xport_gte_execute(0x280030);
        quads[0].tag = ((packet + 36) & 0xFFFFFF) | 0x08000000;
        quads[1].p[2].rgbc.rgbc = color;
        quads[0].p[0].rgbc.rgbc = 0x3A030303;
        quads[1].tag = ((packet + 72) & 0xFFFFFF) | 0x08000000;
        quads[1].p[0].rgbc.rgbc = 0x3A030303;
        value = work.flags;
        if (value == 0)
        {
            seed = random_seed;
            value = psyq_rand_next(&seed);
            random_seed = seed;
            if ((sint32)(value & 4095) >= (sint32)((0xF7E - strength) << 1))
            {
                value = work.emitter_bits;
                work.emitter_bits = value | (1u << (i & 31));
            }
        }
        value = work.emitter_bits;
        work.active_mask = (value >> (i & 31)) & 0x55555555;
        value = xport_gte_read_data(12);
        quads[1].p[0].xy.xy = value;
        value = xport_gte_read_data(13);
        quads[1].p[3].xy.xy = value;
        value = xport_gte_read_data(14);
        quads[0].p[0].xy.xy = value;
        quads[0].p[1].rgbc.rgbc = 0x10101;
        quads[1].p[1].rgbc.rgbc = 0x10101;
        xport_gte_execute(0x158002D);
        a = (uint32)(uint16)work.direction.xy.x;
        seed = (uint32)(uint16)work.edge[1].xy.x;
        b = (uint32)(uint16)work.direction.xy.y;
        c = (uint32)(uint16)work.edge[0].xy.y;
        work.spread.x = (sint16)(uint16)(a - seed);
        work.spread.y = (sint16)(uint16)(b - c);
        depth = xport_gte_read_data(24);
        value = work.direction.xy.xy;
        xport_gte_write_data(0, value);
        value = (uint32)(uint16)work.direction.z;
        xport_gte_write_data(1, value);
        value = work.blended.xy.xy;
        xport_gte_write_data(2, value);
        value = (uint32)(uint16)work.blended.z;
        xport_gte_write_data(3, value);
        value = emitter->tip_xy[1];
        xport_gte_write_data(4, value);
        value = emitter->tip_z_word[1];
        xport_gte_write_data(5, value);
        quads[0].p[3].rgbc.rgbc = 0x30303;
        quads[1].p[3].rgbc.rgbc = 0x30303;
        xport_gte_execute(0x280030);
        shift = state->level;
        value = (sint32)(sint16)state->depth_bias;
        bucket = (uint32)((sint32)depth >> ((shift + 10) & 31)) + 4 - value;
        if ((sint32)bucket < -75)
            bucket = -75;
        else if ((sint32)bucket >= 77)
            bucket = 76;
        bucket = (sint32)bucket >> 2;
        work.bucket = bucket;
        value = xport_gte_read_data(12);
        quads[0].p[2].xy.xy = value;
        value = xport_gte_read_data(12);
        quads[1].p[2].xy.xy = value;
        value = xport_gte_read_data(13);
        quads[0].p[1].xy.xy = value;
        value = xport_gte_read_data(13);
        quads[1].p[1].xy.xy = value;
        value = xport_gte_read_data(14);
        quads[0].p[3].xy.xy = value;
        ordering = state->local_ot + 19 + (sint32)bucket;
        start = stream->packet;
        effect_light_prepend_chain(ordering, 2, start, packet + 36, seed);
        stream->packet = packet + 72;
        value = work.active_mask;
        if (value != 0)
            scene_geometry_draw_composite_segments(state, &work, record->phase_origin, stream);
        value = work.tick;
        work.tick = value + 32;
    }
    result = work.flags;
    if (result == 0)
    {
        result = work.emitter_bits;
        record->emitter_bits = (uint32)(result);
    }
}

typedef struct
{
    XY xy;
    uint32 clip;
} SCENE_COMPOSITE_LIGHT_SCREEN;

typedef struct
{
    uint32 saved_control[8];
    uint16 normal[3];
    SVECTOR angles;
    MATRIX rotation;
    MATRIX scale;
    SVECTOR center;
    SVECTOR vertex[16];
    SCENE_COMPOSITE_LIGHT_SCREEN projected_center;
    SCENE_COMPOSITE_LIGHT_SCREEN projected[17];
} SCENE_COMPOSITE_LIGHT_WORK;

void scene_geometry_draw_composite_variant_2(const SCENE_MESH_CONTEXT *state, FLIGHT_CONTROL *effect, SCENE_RENDER_STREAM *stream)
{
    SCENE_COMPOSITE_LIGHT_WORK work;
    uint32 flags, color, a, b, c, value, index, trig_phase, packet, preamble, start, center, clip_center, clip_previous, clip_next, screen, next, area;
    uint32 *ordering;
    FUNCTION_MARKER(0x8008C9D4, "ACE3.BIN");
    flags = effect_light.flags;
    color = game_runtime.render_settings.halo_color;
    if ((flags & 4) == 0 || color == 0)
        return;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    work.saved_control[0] = a;
    work.saved_control[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    work.saved_control[2] = a;
    work.saved_control[3] = b;
    work.saved_control[4] = c;
    a = xport_gte_read_control(5);
    b = xport_gte_read_control(6);
    c = xport_gte_read_control(7);
    work.saved_control[5] = a;
    work.saved_control[6] = b;
    work.saved_control[7] = c;
    a = xport_gte_read_control(8);
    b = xport_gte_read_control(9);
    work.normal[0] = (uint16)a;
    work.normal[1] = (uint16)(a >> 16);
    work.normal[2] = (uint16)b;
    value = game_normal_angles(work.normal);
    work.angles.vx = (sint16)(uint16)value;
    work.angles.vy = (sint16)(uint16)(value >> 16);
    work.angles.vz = 0;
    work.angles.pad = 0;
    memset(&work.rotation, 0, sizeof(work.rotation));
    RotMatrixYXZ(&work.angles, &work.rotation);
    memset(&work.scale, 0, sizeof(work.scale));
    work.scale.m[0][0] = 4096;
    work.scale.m[0][1] = 0;
    work.scale.m[1][1] = 2457;
    work.scale.m[1][2] = 0;
    work.scale.m[0][2] = 0;
    work.scale.m[1][0] = 0;
    work.scale.m[2][0] = 0;
    work.scale.m[2][1] = 0;
    work.scale.m[2][2] = 3072;
    MulMatrix2(&work.scale, &work.rotation);
    work.center.vx = 0;
    work.center.vy = 0;
    work.center.vz = 240;
    work.center.pad = 0;
    ApplyMatrixSV(&work.rotation, &work.center, &work.center);
    index = 0;
    trig_phase = 0;
    do
    {
        value = rsin(trig_phase);
        work.vertex[index].vx = (sint16)(uint16)((sint32)value / 64);
        value = rcos(trig_phase);
        value *= 96;
        work.vertex[index].vy = (sint16)(uint16)((sint32)value / 4096);
        work.vertex[index].vz = 220;
        work.vertex[index].pad = 0;
        ApplyMatrixSV(&work.rotation, work.vertex + index, work.vertex + index);
        index += 1;
        trig_phase += 256;
    } while (index < 16);
    a = work.saved_control[0];
    b = work.saved_control[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = work.saved_control[2];
    b = work.saved_control[3];
    c = work.saved_control[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    value = ((uint32)(uint16)work.center.vx | ((uint32)(uint16)work.center.vy << 16));
    xport_gte_write_data(0, value);
    value = (uint16)work.center.vz;
    xport_gte_write_data(1, value);
    preamble = stream->packet;
    DR_TPAGE *preamble_packet = mission_packet_span(preamble, sizeof(*preamble_packet));
    preamble_packet->code[0] = 0xE6000000;
    xport_gte_execute(0x180001);
    uint32 *back_ot = state->local_ot + 38;
    value = *back_ot;
    preamble_packet->tag = value | 0x01000000;
    start = preamble + 8;
    packet = start + 8;
    *back_ot = preamble & 0xFFFFFF;
    DR_TPAGE *mask_packet = mission_packet_span(start, sizeof(*mask_packet));
    mask_packet->tag = (packet & 0xFFFFFF) | 0x01000000;
    mask_packet->code[0] = 0xE6000002;
    center = xport_gte_read_data(14);
    index = 0;
    work.projected_center.xy.xy = center;
    do
    {
        value = ((uint32)(uint16)work.vertex[index].vx | ((uint32)(uint16)work.vertex[index].vy << 16));
        xport_gte_write_data(0, value);
        value = (uint16)work.vertex[index].vz;
        xport_gte_write_data(1, value);
        xport_gte_execute(0x180001);
        value = game_screen_clip(index == 0 ? work.projected_center.xy.xy : work.projected[index - 1].xy.xy);
        if (index == 0)
            work.projected_center.clip = value;
        else
            work.projected[index - 1].clip = value;
        value = xport_gte_read_data(14);
        work.projected[index].xy.xy = value;
        index += 1;
    } while (index < 16);
    value = game_screen_clip(work.projected[15].xy.xy);
    work.projected[15].clip = value;
    a = work.projected[0].xy.xy;
    b = work.projected[0].clip;
    clip_center = work.projected_center.clip;
    work.projected[16].xy.xy = a;
    work.projected[16].clip = b;
    xport_gte_write_data(12, center);
    clip_previous = work.projected[0].clip;
    index = 0;
    screen = a;
    do
    {
        clip_next = work.projected[index + 1].clip;
        value = clip_center & clip_previous & clip_next;
        clip_previous = clip_next;
        if (value != 0)
            screen = work.projected[index + 1].xy.xy;
        else
        {
            value = work.projected[index + 1].xy.xy;
            xport_gte_write_data(13, screen);
            xport_gte_write_data(14, value);
            POLY_G3 *triangle = mission_packet_span(packet, sizeof(*triangle));
            triangle->p[0].xy.xy = screen;
            screen = value;
            xport_gte_execute(0x1400006);
            triangle->p[1].xy.xy = screen;
            triangle->p[2].xy.xy = center;
            triangle->p[1].rgbc.rgbc = 0x00030303;
            triangle->p[2].rgbc.rgbc = color;
            area = xport_gte_read_data(24);
            next = packet + 28;
            if ((sint32)area > 0)
            {
                triangle->tag = (next & 0xFFFFFF) | 0x06000000;
                triangle->p[0].rgbc.rgbc = 0x32030303;
                packet = next;
            }
        }
        index += 1;
    } while (index < 16);
    mask_packet = mission_packet_span(packet, sizeof(*mask_packet));
    mask_packet->tag = 0x01000000;
    mask_packet->code[0] = 0xE6000000;
    ordering = state->local_ot;
    effect_light_prepend_chain(ordering, 2, start, packet, 0);
    value = packet + 8;
    stream->packet = value;
}

static void scene_geometry_draw_composite_segments(const SCENE_MESH_CONTEXT *state, SCENE_COMPOSITE_THRUST_WORK *effect, uint32 phase_origin, SCENE_RENDER_STREAM *stream)
{
    const uint16 *random;
    uint32 first, packet, tick, phase, value, a, step, mask, page, distance, i, divisor, sample, remainder, next, packed, x, magnitude, start, result;
    uint32 *ordering;
    FUNCTION_MARKER(0x8008C644, "ACE3.BIN");
    first = stream->packet;
    random = effect_noise.sample;
    packet = first + 12;
    DR_MODE *mode = mission_packet_span(first, sizeof(*mode));
    mode->tag = (packet & 0xFFFFFF) | 0x02000000;
    mode->code[0] = 0xE6000001;
    mode->code[1] = 0xE1000217;
    tick = phase_origin;
    value = effect->tick;
    phase = ((uint32)((sint32)tick >> 3) - (value << 1)) & 255;
    value = (sint32)(sint16)effect->color.xy.x;
    xport_gte_write_control(21, value);
    value = (sint32)(sint16)effect->color.xy.y;
    xport_gte_write_control(22, value);
    value = (sint32)(sint16)effect->color.z;
    xport_gte_write_control(23, value);
    step = (sint32)(sint16)effect->extent.y;
    value = random[phase];
    mask = effect->active_mask;
    distance = (sint32)(sint16)effect->extent.x;
    page = game_runtime.display_buffer;
    distance -= value & 127;
    page = (page != 0) << 8;
    for (i = 0; i < 16 && mask != 0; ++i, mask >>= 2)
    {
        distance += step;
        step -= (uint32)((sint32)step >> 6);
        phase += 2;
        if ((mask & 1) == 0)
            continue;
        xport_gte_write_data(8, distance);
        value = (sint32)(sint16)effect->direction.xy.x;
        xport_gte_write_data(9, value);
        value = (sint32)(sint16)effect->direction.xy.y;
        xport_gte_write_data(10, value);
        value = (sint32)(sint16)effect->direction.z;
        xport_gte_write_data(11, value);
        xport_gte_execute(0x980011);
        divisor = (sint32)(sint16)effect->spread.x;
        if (divisor != 0)
        {
            sample = (sint16)random[phase & 255];
            remainder = (sint32)sample % (sint32)divisor;
            value = (sint32)(remainder * (distance + 512)) >> 11;
            effect->jitter.x = (sint16)(uint16)(value);
        }
        else
            effect->jitter.x = (sint16)(uint16)(0);
        divisor = (sint32)(sint16)effect->spread.y;
        if (divisor != 0)
        {
            sample = (sint16)random[(phase + 1) & 255];
            remainder = (sint32)sample % (sint32)divisor;
            value = remainder + (uint32)((sint32)(remainder * distance) >> 12);
            effect->jitter.y = (sint16)(uint16)(value);
        }
        else
            effect->jitter.y = (sint16)(uint16)(0);
        a = xport_gte_read_data(25);
        value = (uint32)(uint16)effect->jitter.x;
        effect->jitter.x = (sint16)(uint16)(value + a);
        a = xport_gte_read_data(26);
        value = (uint32)(uint16)effect->jitter.y;
        effect->jitter.y = (sint16)(uint16)(value + a);
        value = effect->jitter.xy;
        xport_gte_write_data(0, value);
        value = xport_gte_read_data(27);
        xport_gte_write_data(1, value);
        next = packet + 40;
        SCENE_COMPOSITE_SEGMENT_PACKET *segment = mission_packet_span(packet, sizeof(*segment));
        segment->copy.tag = (next & 0xFFFFFF) | 0x09000000;
        segment->copy.code[0] = 0x01000000;
        segment->copy.code[1] = 0x80000000;
        xport_gte_execute(0x180001);
        segment->copy.code[4] = 0x00010010;
        segment->rgbc.rgbc = 0x67000000;
        segment->copy.code[3] = 0x012B0160;
        segment->sprite.attr = 0x4AD6;
        segment->size.xy = 0x00050010;
        value = xport_gte_read_data(14);
        effect->screen.xy = value;
        magnitude = (sint32)(sint16)effect->screen.x;
        x = (uint32)(uint16)effect->screen.x;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)magnitude >= 153)
            continue;
        magnitude = (sint32)(sint16)effect->screen.y;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)magnitude >= 119)
            continue;
        effect->screen.x = (sint16)(uint16)(x - 8);
        value = (uint32)(uint16)effect->screen.y;
        segment->copy.code[2] = ((x + 152) & 1023) | (((value + page + 120) & 511) << 16);
        value = (uint32)(uint16)effect->screen.y;
        effect->screen.y = (sint16)(uint16)(value - 2);
        value = (sint32)phase >> 1;
        packed = effect->screen.xy;
        segment->sprite.uv.u = 0;
        segment->sprite.uv.v = (uint8)(value & 15);
        segment->sprite.xy.xy = packed;
        packet = next;
    }
    result = first + 12;
    if (packet == result)
    {
        (void)(uint32)(result);
        return;
    }
    mode = mission_packet_span(packet, sizeof(*mode));
    mode->tag = 0x02000000;
    mode->code[0] = 0xE6000000;
    mode->code[1] = 0xE1000220;
    value = effect->bucket;
    value -= 2;
    if ((sint32)value < -19)
        value = 0xFFFFFFED;
    ordering = state->local_ot + 19 + (sint32)value;
    start = stream->packet;
    effect_light_prepend_chain(ordering, 2, start, packet, 0x00050010);
    result = packet + 12;
    stream->packet = result;
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_draw_clip_mask(uint32 ordering, uint32 packet, const PSX_RECT *rectangle, const XY *offset)
{
    uint32 *cmd, *finish, *ot_tag;
    const uint16 *indices;
    uint32 value, mask_command, index, bucket, prior_bucket, current, work, next, packed_x, packed_y, merge, base, color, origin, size, x, y, right, bottom, draw, tag;
    FUNCTION_MARKER(0x800943A8, "ACE3.BIN");
    value = effect_light.flags;
    indices = game_runtime.mission_depth_buckets;
    mask_command = (value & 4) != 0 ? 0xE6000001 : 0xE6000000;
    for (index = 0; index < 16; ++index, ++indices)
    {
        value = ((sint16)*indices);
        current = packet;
        work = packet - 8;
        next = packet + 52;
        if ((index & 1) != 0)
        {
            tag = (next & 0xFFFFFF) | 0x0C000000;
            bucket = ordering + (value << 2);
            cmd = (uint32 *)mission_packet_span(current, 104);
            cmd[0] = tag;
            cmd[13] = 0x0C000000;
            merge = effect_light_prepend_chain(mission_ot_span((bucket + 2) & 0xFFFFFFFC, sizeof(uint32)), (bucket + 2) & 3, packet, next, prior_bucket);
            work += 104;
            packet += 104;
            packed_x = (index & 6) << 5;
            packed_y = (((index & 8) << 5) + 240) << 16;
            cmd[4] = packed_x | packed_y;
            cmd[9] = (packed_x + 16) | packed_y;
            cmd[17] = (packed_x + 32) | packed_y;
            value = (packed_x + 48) | packed_y;
            cmd[14] = 0xE6000000;
            cmd[1] = 0xE6000000;
            cmd[25] = mask_command;
            cmd[12] = mask_command;
            cmd[20] = 0x01000000;
            cmd[15] = 0x01000000;
            cmd[7] = 0x01000000;
            cmd[2] = 0x01000000;
            cmd[21] = 0x80000000;
            cmd[16] = 0x80000000;
            cmd[8] = 0x80000000;
            cmd[3] = 0x80000000;
            cmd[22] = value;
            cmd[5] = 0x01000140;
            cmd[10] = 0x01100140;
            cmd[18] = 0x01200140;
            cmd[23] = 0x01300140;
            cmd[24] = 0x100010;
            cmd[19] = 0x100010;
            cmd[11] = 0x100010;
            cmd[6] = 0x100010;
        }
        else
        {
            work += 32;
            bucket = ordering + (value << 2);
            prior_bucket = bucket;
            packed_x = (index & 6) << 3;
            ot_tag = (uint32 *)mission_ot_span(bucket, sizeof(*ot_tag));
            value = *ot_tag;
            cmd = (uint32 *)mission_packet_span(current, 32);
            cmd[0] = value | 0x07000000;
            packet += 32;
            *ot_tag = current & 0xFFFFFF;
            packed_y = (((index & 8) << 3) + 384) << 16;
            value = (packed_x + 320) | packed_y;
            cmd[1] = 0xE6000000;
            cmd[7] = mask_command;
            cmd[2] = 0x01000000;
            cmd[3] = 0x80000000;
            cmd[4] = value;
            cmd[5] = 0x01000140;
            cmd[6] = 0x400010;
        }
    }
    base = packet;
    next = base + 20;
    draw = base + 44;
    finish = (uint32 *)mission_packet_span(base, 64);
    finish[3] = 0xE5080140;
    finish[2] = 0xE404FD4F;
    finish[0] = (next & 0xFFFFFF) | 0x04000000;
    finish[1] = 0xE3040140;
    finish[4] = 0;
    color = game_runtime.render_settings.palette_primary;
    finish[5] = (draw & 0xFFFFFF) | 0x05000000;
    finish[6] = 0xE6000001;
    finish[8] = 0;
    finish[9] = 0x400010;
    finish[10] = mask_command;
    finish[7] = (color & 0xFFFFFF) | 0x60000000;
    origin = (uint32)(uint16)rectangle->x | ((uint32)(uint16)rectangle->y << 16);
    finish[11] = 0x04000000;
    x = (sint16)origin;
    y = (sint32)origin >> 16;
    value = (x & 0x3FF) | ((y & 0x3FF) << 10) | 0xE3000000;
    size = (uint32)(uint16)rectangle->w | ((uint32)(uint16)rectangle->h << 16);
    finish[12] = value;
    right = x + (uint32)(sint32)(sint16)size - 1;
    bottom = y + (uint32)((sint32)size >> 16) - 1;
    packed_x = ((uint32)(uint16)offset->x | ((uint32)(uint16)offset->y << 16));
    finish[13] = (right & 0x3FF) | ((bottom & 0x3FF) << 10) | 0xE4000000;
    value = ((sint16)*indices);
    finish[15] = 0;
    finish[14] = (packed_x & 0x7FF) | ((uint32)((sint32)packed_x >> 5) & 0x3FF800) | 0xE5000000;
    bucket = ordering + (value << 2);
    tag = finish[11];
    ot_tag = (uint32 *)mission_ot_span(bucket, sizeof(*ot_tag));
    value = *ot_tag;
    finish[11] = (tag & 0xFF000000) | (value & 0xFFFFFF);
    value = *ot_tag;
    *ot_tag = (value & 0xFF000000) | (base & 0xFFFFFF);
    return base + 64;
}

typedef struct
{
    uint32 rotation[5];
    uint16 delta[3];
    SVECTOR vertex[3];
    GAME_ANGLES angles;
    MATRIX matrix;
    XY screen;
    uint32 count;
} SCENE_DETAIL_CONTEXT;

void scene_geometry_draw_detail_0(EFFECT_FRAME_CONTEXT *frame)
{
    SCENE_DETAIL_CONTEXT context;
    EFFECT_QUAD quad;
    const uint16 *table;
    uint32 x, y, z, distance, color, value, a, b, c, d, e, packet, uv, size, index, output, depth, result, head, tail;
    FUNCTION_MARKER(0x8008E038, "ACE3.BIN");
    x = game_runtime.camera_delta.position[0];
    y = game_runtime.camera_delta.position[1];
    z = game_runtime.camera_delta.position[2];
    x = (sint32)x >> 7;
    y = (sint32)y >> 7;
    z = (sint32)z >> 7;
    a = (sint32)x < 0 ? 0 - x : x;
    b = (sint32)y < 0 ? 0 - y : y;
    c = (sint32)z < 0 ? 0 - z : z;
    distance = a + b + c;
    result = distance - 512 < 0x2E01;
    context.delta[0] = (uint16)(x);
    context.delta[1] = (uint16)(y);
    context.delta[2] = (uint16)(z);
    if (result == 0)
        return;
    color = 0x2E0F0F0F;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, color);
    value = (2048 - distance) << 2;
    if ((sint32)value > 0)
    {
        xport_gte_write_data(8, value);
        xport_gte_execute(0x780010);
        color = xport_gte_read_data(22);
        xport_gte_write_data(6, color);
    }
    a = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.camera.m[2][1] << 16));
    e = (uint16)game_runtime.mission_matrices.camera.m[2][2];
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    packet = frame->packet;
    table = effect_noise.sample;
    xport_gte_execute(0x486012);
    a = frame->camera.word[0];
    b = frame->camera.word[1];
    x = 0 - (uint32)((sint32)a >> 9);
    y = 0 - (uint32)((sint32)b >> 9);
    a = frame->camera.word[2];
    b = frame->projection;
    z = 0 - (uint32)((sint32)a >> 9);
    size = (uint32)(sint32)(sint16)b * 140;
    x -= xport_gte_read_data(25);
    y -= xport_gte_read_data(26);
    z -= xport_gte_read_data(27);
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
    xport_gte_write_control(7, 4096);
    value = (sint32)(distance - 2048) >> 8;
    if ((sint32)value < 0)
        value = 0;
    else if ((sint32)value > 10)
        value = 10;
    xport_gte_write_control(26, 160 - value);
    output = packet + 4;
    for (index = 0; index < 16; ++index)
    {
        /* only the first 38 bytes are touched before culling */
        POLY_FT4 *poly = mission_packet_span(packet, offsetof(POLY_FT4, p) + 3 * sizeof(POINT_T) + offsetof(POINT_T, attr));
        value = table[0];
        context.vertex[0].vx = (sint16)(uint16)(((value + x) & 0x3FFF) - 8192);
        value = table[1];
        context.vertex[0].vy = (sint16)(uint16)(((value + y) & 0x3FFF) - 8192);
        value = table[2];
        context.vertex[0].vz = (sint16)(uint16)(((value + z) & 0x3FFF) - 8192);
        a = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
        xport_gte_write_data(0, a);
        b = (uint16)context.vertex[0].vz;
        xport_gte_write_data(1, b);
        a = (uint16)context.vertex[0].vx;
        b = context.delta[0];
        c = (uint16)context.vertex[0].vy;
        d = context.delta[1];
        context.vertex[1].vx = (sint16)(uint16)(a - b);
        a = (uint16)context.vertex[0].vz;
        b = context.delta[2];
        context.vertex[1].vy = (sint16)(uint16)(c - d);
        c = (uint16)context.vertex[0].vx;
        d = context.delta[0];
        context.vertex[1].vz = (sint16)(uint16)(a - b);
        a = (uint16)context.vertex[0].vy;
        b = context.delta[1];
        context.vertex[2].vx = (sint16)(uint16)(c + d);
        c = (uint16)context.vertex[0].vz;
        d = context.delta[2];
        context.vertex[2].vy = (sint16)(uint16)(a + b);
        context.vertex[2].vz = (sint16)(uint16)(c + d);
        a = ((uint32)(uint16)context.vertex[1].vx | ((uint32)(uint16)context.vertex[1].vy << 16));
        xport_gte_write_data(2, a);
        b = (uint16)context.vertex[1].vz;
        xport_gte_write_data(3, b);
        a = ((uint32)(uint16)context.vertex[2].vx | ((uint32)(uint16)context.vertex[2].vy << 16));
        xport_gte_write_data(4, a);
        b = (uint16)context.vertex[2].vz;
        xport_gte_write_data(5, b);
        poly->tag = (uint32)(((packet + 40) & 0xFFFFFF) | 0x09000000);
        xport_gte_execute(0x280030);
        uv = (index << 1) & 0xE0;
        poly->p[1].uv_attr = (uint32)((uv + 63) | 0x0037C000);
        poly->p[0].uv_attr = (uint32)(uv | 0x40D6C000);
        poly->p[2].uv.uv = (uint16)(uv | 0xFF00);
        poly->p[3].uv.uv = (uint16)((uv + 63) | 0xFF00);
        value = xport_gte_read_control(31);
        if ((sint32)value < 0)
            goto next_particle_8e038;
        value = xport_gte_read_data(13);
        quad.endpoint[0].xy = value;
        value = quad.endpoint[0].x;
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 241)
            goto next_particle_8e038;
        value = quad.endpoint[0].y;
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 201)
            goto next_particle_8e038;
        poly->rgbc.rgbc = (uint32)(color);
        value = xport_gte_read_data(14);
        quad.endpoint[1].xy = value;
        depth = xport_gte_read_data(18);
        if (depth != 0)
        {
            if (depth == 0xFFFFFFFF && size == 0x80000000)
                xport_mips_break(6);
            value = (sint32)size / (sint32)depth;
            quad.scale[0] = value;
        }
        else
            quad.scale[0] = 0;
        depth = xport_gte_read_data(19);
        if (depth != 0)
        {
            if (depth == 0xFFFFFFFF && size == 0x80000000)
                xport_mips_break(6);
            value = (sint32)size / (sint32)depth;
            quad.scale[1] = value;
        }
        else
            quad.scale[1] = 0;
        poly = mission_packet_span(packet, sizeof(*poly));
        effect_quad_build(poly->p, &quad);
        output += 40;
        packet += 40;
    next_particle_8e038:
        table += 4;
    }
    result = 0;
    head = frame->packet;
    tail = packet - 40;
    if (head != packet)
    {
        result = frame->ot + 112;
        /* DMA packets and ordering heads are word aligned; LWL low byte is discarded by SWL */
        value = effect_light_prepend_chain(mission_ot_span((result + 2) & 0xFFFFFFFC, sizeof(uint32)), (result + 2) & 3, head, tail, 0);
        frame->packet = packet;
    }
    value = frame->projection;
    xport_gte_write_control(26, value);
    xport_gte_write_control(7, 0);
    return;
}

typedef struct
{
    uint32 tag;
    POINT_C points[3][2];
} SCENE_DETAIL_LINES;

typedef char scene_detail_packet_layout_check[sizeof(SCENE_DETAIL_LINES) == 52 && offsetof(SCENE_DETAIL_LINES, points) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && offsetof(POINT_C, rgbc) == 0 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && offsetof(POLY_FT4, rgbc) == 4 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(TILE) == 16 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 ? 1 : -1];

void scene_geometry_draw_detail_1(EFFECT_FRAME_CONTEXT *frame)
{
    SCENE_DETAIL_LINES *lines;
    SCENE_DETAIL_CONTEXT context;
    SVECTOR *point;
    const uint16 *origin;
    uint32 count, density, camera_y, height, value, result, a, b, c, x, y, z, packet, random, depth, fade, group, vertex, ordering, start, tail;
    FUNCTION_MARKER(0x8008EB58, "ACE3.BIN");
    count = game_runtime.scene_assets.environment.detail_count;
    if (count == 0)
        return;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, 0x5225211D);
    density = (sint32)game_runtime.scene_assets.environment.detail_density;
    xport_gte_write_data(8, density);
    result = (sint32)density < 4096;
    if (result == 0)
        return;
    xport_gte_execute(0x780010);
    camera_y = frame->camera.word[1];
    value = game_runtime.world_settings.ground_offset;
    height = camera_y + value;
    if (count >= 86)
        count = 85;
    if ((sint32)height < 0)
        height = (uint32)(sint32)game_runtime.scene_assets.environment.layer[1].height - ((game_runtime.scene_assets.environment.layer[1].count >> 1) * game_runtime.scene_assets.environment.layer[1].slope) - ((uint32)((sint32)camera_y >> 12) + 2048);
    else
        height = (sint32)height >> 8;
    density = xport_gte_read_data(22);
    result = (sint32)height < 4096;
    if (result == 0)
        return;
    xport_gte_write_data(8, height);
    xport_gte_write_data(6, density);
    if ((sint32)height > 0)
    {
        xport_gte_execute(0x780010);
        value = count * height;
        if ((sint32)value < 0)
            value += 4095;
        result = (sint32)value >> 12;
        count -= result;
        height = xport_gte_read_data(22);
        if ((sint32)count <= 0)
            return;
        xport_gte_write_data(6, height);
    }
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
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
    xport_gte_write_data(1, 0x2000);
    context.count = count;
    xport_gte_execute(0x486012);
    a = frame->camera.word[0];
    b = frame->camera.word[1];
    x = (sint32)(0 - a) >> 8;
    y = (sint32)(0 - b) >> 8;
    b = frame->camera.word[2];
    b = 0 - b;
    a = game_runtime.gameplay_counter;
    z = (sint32)b >> 8;
    y += a << 7;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 0x2000);
    packet = frame->packet;
    context.vertex[0].vx = (sint16)(uint16)((x & 0x1FFF) - 4096);
    context.vertex[0].vy = (sint16)(uint16)((y & 0x1FFF) - 4096);
    origin = effect_noise.sample;
    context.vertex[0].vz = (sint16)(uint16)((z & 0x1FFF) - 4096);
    for (group = 0; group < count; ++group)
    {
        lines = mission_packet_span(packet, sizeof(*lines));
        for (vertex = 0; vertex < 3; ++vertex)
        {
            value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
            xport_gte_write_data(0, value);
            value = (uint16)context.vertex[0].vz;
            xport_gte_write_data(1, value);
            random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
            origin += 2;
            xport_gte_execute(0x180001);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            context.vertex[0].vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
            value = z + random;
            context.vertex[0].vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
            context.vertex[0].vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
            depth = xport_gte_read_data(19);
            value = xport_gte_read_data(14);
            fade = depth - 8192;
            lines->points[vertex][0].xy.xy = value;
            if ((sint32)fade < 0)
                fade = 8192 - depth;
            if ((sint32)fade >= 4097)
                fade = 4096;
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            lines->points[vertex][1].rgbc.rgbc = 0x30303;
            value = xport_gte_read_data(22);
            lines->points[vertex][0].rgbc.rgbc = value;
        }
        packet += 52;
    }
    a = frame->angles.value[0];
    b = game_runtime.camera_delta.angles[0];
    context.angles.value[0] = (uint16)(a - b);
    a = frame->angles.value[1];
    b = game_runtime.camera_delta.angles[1];
    context.angles.value[1] = (uint16)(a - b);
    a = frame->angles.value[2];
    b = game_runtime.camera_delta.angles[2];
    context.angles.value[2] = (uint16)(a - b);
    game_build_rotation_via_gte(&context.matrix, context.angles.value);
    a = ((uint32)(uint16)context.matrix.m[0][0] | ((uint32)(uint16)context.matrix.m[0][1] << 16));
    b = ((uint32)(uint16)context.matrix.m[0][2] | ((uint32)(uint16)context.matrix.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)context.matrix.m[1][1] | ((uint32)(uint16)context.matrix.m[1][2] << 16));
    b = ((uint32)(uint16)context.matrix.m[2][0] | ((uint32)(uint16)context.matrix.m[2][1] << 16));
    c = (uint16)context.matrix.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    value = game_runtime.camera_delta.position[0];
    context.vertex[0].vx = (sint16)(uint16)((sint32)value >> 8);
    value = game_runtime.camera_delta.position[1];
    value = (sint32)value >> 8;
    context.vertex[0].vy = (sint16)(uint16)(value);
    b = game_runtime.camera_delta.position[2];
    context.vertex[0].vy = (sint16)(uint16)(value - 128);
    context.vertex[0].vz = (sint16)(uint16)((sint32)b >> 8);
    value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
    xport_gte_write_data(0, value);
    value = (uint16)context.vertex[0].vz;
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    value = xport_gte_read_data(25);
    xport_gte_write_control(5, value);
    value = xport_gte_read_data(26);
    xport_gte_write_control(6, value);
    value = xport_gte_read_data(27);
    xport_gte_write_control(7, value + 8192);
    packet = frame->packet;
    context.vertex[0].vx = (sint16)(uint16)((x & 0x1FFF) - 4096);
    context.vertex[0].vy = (sint16)(uint16)((y & 0x1FFF) - 4096);
    origin = effect_noise.sample;
    count = context.count;
    context.vertex[0].vz = (sint16)(uint16)((z & 0x1FFF) - 4096);
    for (vertex = 0; vertex < 2; ++vertex)
    {
        random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
        origin += 2;
        a = x + random;
        random = (sint32)random >> 8;
        b = y + random;
        random = (sint32)random >> 8;
        point = context.vertex + vertex + 1;
        point->vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
        value = z + random;
        point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
        point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
    }
    for (group = 0; (sint32)group < (sint32)count; ++group)
    {
        gte_ldv3(context.vertex, context.vertex + 1, context.vertex + 2);
        for (vertex = 0; vertex < 3; ++vertex)
        {
            random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
            origin += 2;
            if (vertex == 0)
                xport_gte_execute(0x280030);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            point = context.vertex + vertex;
            point->vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
            value = z + random;
            if (vertex == 2)
            {
                point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
                point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
            }
            else
            {
                point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
                point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
            }
        }
        lines = (SCENE_DETAIL_LINES *)mission_packet_span(packet, sizeof(*lines));
        lines->tag = ((packet + 52) & 0xFFFFFF) | 0x0C000000;
        value = xport_gte_read_data(12);
        lines->points[0][1].xy.xy = value;
        value = xport_gte_read_data(13);
        lines->points[1][1].xy.xy = value;
        value = xport_gte_read_data(14);
        lines->points[2][1].xy.xy = value;
        packet += 52;
    }
    tail = packet - 52;
    ordering = frame->ot;
    start = frame->packet;
    ordering += 112;
    effect_light_prepend_chain(mission_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, start, tail, 0);
    frame->packet = packet;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    return;
}

void scene_geometry_draw_detail_2(EFFECT_FRAME_CONTEXT *frame)
{
    POLY_FT4 *quad;
    SCENE_DETAIL_CONTEXT context;
    const uint16 *origin;
    uint32 count, density, value, result, a, b, c, x, y, z, packet, random, depth, fade, size, start, tail, ordering;
    sint32 signed_value;
    FUNCTION_MARKER(0x8008F1B8, "ACE3.BIN");
    count = game_runtime.scene_assets.environment.detail_count;
    if (count == 0)
        return;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x2E838383);
    density = (sint32)game_runtime.scene_assets.environment.detail_density;
    xport_gte_write_data(8, density);
    result = (sint32)density < 4096;
    if (result == 0)
        return;
    xport_gte_execute(0x780010);
    value = frame->camera.word[1];
    y = game_runtime.scene_assets.environment.layer[0].height;
    y -= (uint32)((sint32)value >> 12);
    density = xport_gte_read_data(22);
    result = (sint32)y < 4096;
    if (result == 0)
        return;
    xport_gte_write_data(6, density);
    xport_gte_write_data(8, y);
    if ((sint32)y > 0)
    {
        xport_gte_execute(0x780010);
        value = count * y;
        if ((sint32)value < 0)
            value += 4095;
        result = (sint32)value >> 12;
        count -= result;
        y = xport_gte_read_data(22);
        if ((sint32)count <= 0)
            return;
        xport_gte_write_data(6, y);
    }
    xport_gte_write_control(21, 256);
    xport_gte_write_control(22, 256);
    xport_gte_write_control(23, 256);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
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
    xport_gte_write_data(1, 0x3800);
    xport_gte_execute(0x486012);
    a = frame->camera.word[0];
    b = frame->camera.word[1];
    x = (sint32)(0 - a) >> 8;
    y = (sint32)(0 - b) >> 8;
    b = frame->camera.word[2];
    b = 0 - b;
    a = game_runtime.gameplay_counter;
    z = (sint32)b >> 8;
    y += a << 7;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 0x3800);
    packet = frame->packet;
    context.vertex[0].vx = (sint16)(uint16)((x & 0x3FFF) - 0x2000);
    context.vertex[0].vy = (sint16)(uint16)((y & 0x3FFF) - 0x2000);
    origin = effect_noise.sample;
    result = (z & 0x3FFF) - 0x2000;
    context.vertex[0].vz = (sint16)(uint16)(result);
    while ((sint32)count > 0)
    {
        value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
        xport_gte_write_data(0, value);
        value = (uint16)context.vertex[0].vz;
        xport_gte_write_data(1, value);
        random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
        origin += 2;
        xport_gte_execute(0x180001);
        a = x + random;
        random = (sint32)random >> 9;
        b = y + random;
        random = (sint32)random >> 9;
        context.vertex[0].vx = (sint16)(uint16)((a & 0x3FFF) - 0x2000);
        value = z + random;
        context.vertex[0].vz = (sint16)(uint16)((value & 0x3FFF) - 0x2000);
        context.vertex[0].vy = (sint16)(uint16)((b & 0x3FFF) - 0x2000);
        quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
        quad->p[2].uv.uv = (uint16)(0x1FF0);
        quad->p[0].uv_attr = (uint32)(0x461610F0);
        quad->p[1].uv_attr = (uint32)(0x003810FF);
        quad->p[3].uv.uv = (uint16)(0x1FFF);
        depth = xport_gte_read_data(19);
        value = xport_gte_read_data(14);
        context.screen.xy = value;
        value = depth - 7168;
        fade = (sint32)((sint32)value < 0 ? 7168 - depth : value) >> 2;
        if ((sint32)fade >= 4097)
            fade = 4096;
        xport_gte_write_data(8, fade);
        size = (sint32)(23686 - depth) >> 12;
        xport_gte_execute(0x780010);
        if ((sint32)size < 2)
            size = 2;
        signed_value = context.screen.x;
        if (signed_value < 0)
            signed_value = -signed_value;
        result = (sint32)((uint32)signed_value - size) < 161;
        if (result != 0)
        {
            signed_value = context.screen.y;
            if (signed_value < 0)
                signed_value = -signed_value;
            result = (sint32)((uint32)signed_value - size) < 121;
            if (result != 0)
            {
                quad->tag = (uint32)(((packet + 40) & 0xFFFFFF) | 0x09000000);
                value = context.screen.x;
                a = value - (uint32)((sint32)size >> 1);
                value = a + size;
                quad->p[2].xy.x = (sint16)(uint16)(a);
                quad->p[0].xy.x = (sint16)(uint16)(a);
                quad->p[3].xy.x = (sint16)(uint16)(value);
                quad->p[1].xy.x = (sint16)(uint16)(value);
                value = context.screen.y;
                a = value - (uint32)((sint32)size >> 1);
                result = a + size;
                quad->p[1].xy.y = (sint16)(uint16)(a);
                quad->p[0].xy.y = (sint16)(uint16)(a);
                quad->p[3].xy.y = (sint16)(uint16)(result);
                quad->p[2].xy.y = (sint16)(uint16)(result);
                value = xport_gte_read_data(22);
                quad->rgbc.rgbc = (uint32)(value);
                packet += 40;
            }
        }
        --count;
    }
    start = frame->packet;
    tail = packet - 40;
    if (start != packet)
    {
        ordering = frame->ot;
        result = ordering + 112;
        effect_light_prepend_chain(mission_ot_span((result + 2) & 0xFFFFFFFC, sizeof(uint32)), (result + 2) & 3, start, tail, 0);
        frame->packet = packet;
    }
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 0);
    return;
}

void scene_geometry_draw_detail_3(EFFECT_FRAME_CONTEXT *frame)
{
    TILE *tile;
    SCENE_DETAIL_LINES *lines;
    SCENE_DETAIL_CONTEXT context;
    SVECTOR *point;
    const uint16 *origin;
    uint32 height, camera_y, value, result, a, b, c, x, y, z, packet, random, depth, fade, group, vertex, ordering, start, tail;
    FUNCTION_MARKER(0x8008E4F8, "ACE3.BIN");
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
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
    xport_gte_write_data(1, 8192);
    value = frame->camera.word[0];
    x = (sint32)(0 - value) >> 10;
    xport_gte_execute(0x486012);
    camera_y = frame->camera.word[1];
    y = (sint32)(0 - camera_y) >> 10;
    a = frame->camera.word[2];
    value = game_runtime.world_settings.ground_offset;
    a = 0 - a;
    height = (sint32)(camera_y + value) >> 14;
    z = (sint32)a >> 10;
    if ((sint32)height < 0)
        height = 0;
    else if ((sint32)height >= 4097)
        height = 4096;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 8192);
    if ((sint32)height > 0)
    {
        xport_gte_write_control(21, 48);
        xport_gte_write_control(22, 0);
        xport_gte_write_control(23, 0);
        xport_gte_write_data(6, 0x6203135F);
        fade = 4224 - (height << 1);
        if ((sint32)fade < 0)
            fade = 0;
        xport_gte_write_data(8, fade);
        packet = frame->packet;
        frame->packet = packet + 16;
        xport_gte_execute(0x780010);
        ordering = frame->ot;
        value = *(uint32 *)mission_ot_span(ordering + 4, sizeof(uint32));
        tile = (TILE *)mission_packet_span(packet, sizeof(*tile));
        tile->tag = (uint32)(value | 0x03000000);
        value = packet & 0xFFFFFF;
        ordering = frame->ot;
        *(uint32 *)mission_ot_span(ordering + 4, sizeof(uint32)) = value;
        tile->p[0].xy = 0xFF88FF60;
        tile->wh = (uint32)(0x00F00140);
        value = xport_gte_read_data(22);
        tile->rgbc.rgbc = (uint32)(value);
    }
    xport_gte_write_data(6, 0x52534333);
    xport_gte_write_control(21, 0xC30);
    xport_gte_write_control(22, 0xB30);
    xport_gte_write_control(23, 0x1F0);
    xport_gte_write_data(8, height);
    packet = frame->packet;
    origin = effect_noise.sample;
    xport_gte_execute(0x780010);
    context.vertex[0].vx = (sint16)(uint16)((x & 0x1FFF) - 4096);
    context.vertex[0].vy = (sint16)(uint16)((y & 0x1FFF) - 4096);
    context.vertex[0].vz = (sint16)(uint16)((z & 0x1FFF) - 4096);
    value = xport_gte_read_data(22);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, value);
    for (group = 0; group < 64; ++group)
    {
        lines = mission_packet_span(packet, sizeof(*lines));
        for (vertex = 0; vertex < 3; ++vertex)
        {
            value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
            xport_gte_write_data(0, value);
            value = (uint16)context.vertex[0].vz;
            xport_gte_write_data(1, value);
            random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
            origin += 2;
            xport_gte_execute(0x180001);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            context.vertex[0].vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
            value = z + random;
            context.vertex[0].vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
            context.vertex[0].vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
            depth = xport_gte_read_data(19);
            value = xport_gte_read_data(14);
            fade = depth - 8192;
            lines->points[vertex][0].xy.xy = value;
            if ((sint32)fade < 0)
                fade = 8192 - depth;
            if ((sint32)fade >= 4097)
                fade = 4096;
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            lines->points[vertex][1].rgbc.rgbc = 0x30303;
            value = xport_gte_read_data(22);
            lines->points[vertex][0].rgbc.rgbc = value;
        }
        packet += 52;
    }
    a = frame->angles.value[0];
    b = game_runtime.camera_delta.angles[0];
    context.angles.value[0] = (uint16)(a - b);
    a = frame->angles.value[1];
    b = game_runtime.camera_delta.angles[1];
    context.angles.value[1] = (uint16)(a - b);
    a = frame->angles.value[2];
    b = game_runtime.camera_delta.angles[2];
    context.angles.value[2] = (uint16)(a - b);
    game_build_rotation_via_gte(&context.matrix, context.angles.value);
    a = ((uint32)(uint16)context.matrix.m[0][0] | ((uint32)(uint16)context.matrix.m[0][1] << 16));
    b = ((uint32)(uint16)context.matrix.m[0][2] | ((uint32)(uint16)context.matrix.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)context.matrix.m[1][1] | ((uint32)(uint16)context.matrix.m[1][2] << 16));
    b = ((uint32)(uint16)context.matrix.m[2][0] | ((uint32)(uint16)context.matrix.m[2][1] << 16));
    c = (uint16)context.matrix.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    value = game_runtime.camera_delta.position[0];
    context.vertex[0].vx = (sint16)(uint16)((sint32)value >> 9);
    value = game_runtime.camera_delta.position[1];
    value = (sint32)value >> 9;
    context.vertex[0].vy = (sint16)(uint16)(value);
    b = game_runtime.camera_delta.position[2];
    context.vertex[0].vz = (sint16)(uint16)((sint32)b >> 9);
    value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
    xport_gte_write_data(0, value);
    value = (uint16)context.vertex[0].vz;
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    value = xport_gte_read_data(25);
    xport_gte_write_control(5, value);
    value = xport_gte_read_data(26);
    xport_gte_write_control(6, value);
    value = xport_gte_read_data(27);
    xport_gte_write_control(7, value + 8192);
    packet = frame->packet;
    context.vertex[0].vx = (sint16)(uint16)((x & 0x1FFF) - 4096);
    context.vertex[0].vy = (sint16)(uint16)((y & 0x1FFF) - 4096);
    origin = effect_noise.sample;
    context.vertex[0].vz = (sint16)(uint16)((z & 0x1FFF) - 4096);
    for (vertex = 0; vertex < 2; ++vertex)
    {
        random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
        origin += 2;
        a = x + random;
        random = (sint32)random >> 8;
        b = y + random;
        random = (sint32)random >> 8;
        point = context.vertex + vertex + 1;
        point->vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
        value = z + random;
        point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
        point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
    }
    for (group = 0; group < 64; ++group)
    {
        gte_ldv3(context.vertex, context.vertex + 1, context.vertex + 2);
        for (vertex = 0; vertex < 3; ++vertex)
        {
            random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
            origin += 2;
            if (vertex == 0)
                xport_gte_execute(0x280030);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            point = context.vertex + vertex;
            point->vx = (sint16)(uint16)((a & 0x1FFF) - 4096);
            value = z + random;
            if (vertex == 2)
            {
                point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
                point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
            }
            else
            {
                point->vy = (sint16)(uint16)((b & 0x1FFF) - 4096);
                point->vz = (sint16)(uint16)((value & 0x1FFF) - 4096);
            }
        }
        lines = (SCENE_DETAIL_LINES *)mission_packet_span(packet, sizeof(*lines));
        lines->tag = ((packet + 52) & 0xFFFFFF) | 0x0C000000;
        value = xport_gte_read_data(12);
        lines->points[0][1].xy.xy = value;
        value = xport_gte_read_data(13);
        lines->points[1][1].xy.xy = value;
        value = xport_gte_read_data(14);
        lines->points[2][1].xy.xy = value;
        packet += 52;
    }
    start = frame->packet;
    tail = packet - 52;
    result = 0;
    if (start != packet)
    {
        ordering = frame->ot;
        result = ordering + 16376;
        effect_light_prepend_chain(mission_ot_span((result + 2) & 0xFFFFFFFC, sizeof(uint32)), (result + 2) & 3, start, tail, 0);
        frame->packet = packet;
    }
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    return;
}

void scene_geometry_draw_detail_4(EFFECT_FRAME_CONTEXT *frame)
{
    POLY_FT4 *quad;
    SCENE_DETAIL_CONTEXT context;
    const uint16 *origin;
    uint32 a, b, c, packet, value, x, y, z, count, result, random, old_x, old_z, extent_x, extent_z, depth, fade, ordering, slot;
    sint32 absolute;
    FUNCTION_MARKER(0x8008F580, "ACE3.BIN");
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    context.rotation[0] = a;
    context.rotation[1] = b;
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    context.rotation[2] = a;
    context.rotation[3] = b;
    context.rotation[4] = c;
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
    xport_gte_write_data(1, 0x2400);
    origin = effect_noise.sample;
    packet = frame->packet;
    xport_gte_execute(0x486012);
    a = frame->camera.word[0];
    b = frame->camera.word[2];
    x = (sint32)(0 - a) >> 12;
    z = (sint32)(0 - b) >> 12;
    a = frame->camera.word[1];
    a = 0 - a;
    count = game_runtime.scene_assets.environment.detail_repeat;
    y = (sint32)a >> 12;
    if ((sint32)count <= 0)
        count = 1;
    value = xport_gte_read_data(25);
    x -= value;
    a = xport_gte_read_data(26);
    value = xport_gte_read_data(27);
    z -= value;
    y -= a;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 0x2400);
    context.vertex[0].vx = (sint16)(uint16)((x & 0x3FFF) - 0x2000);
    context.vertex[2].vy = (sint16)(uint16)(y);
    context.vertex[1].vy = (sint16)(uint16)(y);
    context.vertex[0].vy = (sint16)(uint16)(y);
    result = (z & 0x3FFF) - 0x2000;
    context.vertex[0].vz = (sint16)(uint16)(result);
    do
    {
        value = ((uint32)(uint16)context.vertex[0].vx | ((uint32)(uint16)context.vertex[0].vy << 16));
        xport_gte_write_data(0, value);
        value = (uint16)context.vertex[0].vz;
        xport_gte_write_data(1, value);
        random = ((uint32)origin[0] | ((uint32)origin[1] << 16));
        origin += 2;
        xport_gte_execute(0x180001);
        value = ((x + random) & 0x3FFF) - 0x2000;
        extent_z = ((uint32)((sint32)random >> 16) & 255) + 800;
        old_x = (uint16)context.vertex[0].vx;
        old_z = (uint16)context.vertex[0].vz;
        context.vertex[0].vx = (sint16)(uint16)(value);
        extent_x = old_x + (random & 255) + 800;
        random = (sint32)random >> 9;
        context.vertex[2].vz = (sint16)(uint16)(old_z + extent_z);
        result = ((z + random) & 0x3FFF) - 0x2000;
        context.vertex[1].vx = (sint16)(uint16)(old_x);
        context.vertex[1].vz = (sint16)(uint16)(old_z);
        context.vertex[2].vx = (sint16)(uint16)(extent_x);
        context.vertex[0].vz = (sint16)(uint16)(result);
        depth = xport_gte_read_data(19);
        value = xport_gte_read_data(14);
        context.screen.xy = value;
        if ((sint32)depth <= 0)
            goto next_effect_8f580;
        absolute = context.screen.y;
        if (absolute < 0)
            absolute = -absolute;
        result = absolute < 161;
        if (result == 0)
            goto next_effect_8f580;
        absolute = context.screen.x;
        if (absolute < 0)
            absolute = -absolute;
        result = (sint32)depth < 2192;
        if (absolute >= 251)
            goto next_effect_8f580;
        fade = (sint32)depth < 2192 ? (2192 - depth) << 1 : (uint32)((sint32)(depth - 4415) >> 2);
        result = (sint32)fade < 4097;
        if (result == 0)
            goto next_effect_8f580;
        value = (uint16)context.vertex[2].vz;
        xport_gte_write_data(1, value);
        value = ((uint32)(uint16)context.vertex[2].vx | ((uint32)(uint16)context.vertex[2].vy << 16));
        xport_gte_write_data(2, value);
        value = (uint16)context.vertex[1].vz;
        xport_gte_write_data(3, value);
        value = ((uint32)(uint16)context.vertex[2].vx | ((uint32)(uint16)context.vertex[2].vy << 16));
        xport_gte_write_data(4, value);
        value = (uint16)context.vertex[2].vz;
        xport_gte_write_data(5, value);
        value = context.screen.xy;
        quad = (POLY_FT4 *)mission_packet_span(packet, sizeof(*quad));
        quad->p[0].xy.xy = (uint32)(value);
        xport_gte_execute(0x280030);
        if ((sint32)fade <= 0)
            quad->rgbc.rgbc = (uint32)(0x2E838383);
        if ((random & 1) != 0)
        {
            quad->p[2].uv.u = (uint8)(0);
            quad->p[0].uv.u = (uint8)(0);
            quad->p[3].uv.u = (uint8)(63);
            quad->p[1].uv.u = (uint8)(63);
        }
        else
        {
            quad->p[2].uv.u = (uint8)(63);
            quad->p[0].uv.u = (uint8)(63);
            quad->p[3].uv.u = (uint8)(0);
            quad->p[1].uv.u = (uint8)(0);
        }
        if ((random & 2) != 0)
        {
            quad->p[1].uv.v = (uint8)(128);
            quad->p[0].uv.v = (uint8)(128);
            quad->p[3].uv.v = (uint8)(191);
            quad->p[2].uv.v = (uint8)(191);
        }
        else
        {
            quad->p[1].uv.v = (uint8)(191);
            quad->p[0].uv.v = (uint8)(191);
            quad->p[3].uv.v = (uint8)(128);
            quad->p[2].uv.v = (uint8)(128);
        }
        slot = (sint32)depth >> 2;
        quad->p[0].attr = (uint16)(0x4ED6);
        quad->p[1].attr = (uint16)(57);
        if ((sint32)slot >= 4095)
            slot = 4094;
        result = xport_gte_read_data(19);
        slot <<= 2;
        if ((sint32)result <= 0)
            goto next_effect_8f580;
        ordering = frame->ot;
        value = *(uint32 *)mission_ot_span(ordering + slot, sizeof(uint32));
        quad->tag = (uint32)(value | 0x09000000);
        ordering = frame->ot;
        result = packet & 0xFFFFFF;
        *(uint32 *)mission_ot_span(ordering + slot, sizeof(uint32)) = result;
        value = xport_gte_read_data(12);
        quad->p[2].xy.xy = (uint32)(value);
        value = xport_gte_read_data(13);
        quad->p[1].xy.xy = (uint32)(value);
        value = xport_gte_read_data(14);
        quad->p[3].xy.xy = (uint32)(value);
        if ((sint32)fade > 0)
        {
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            value = xport_gte_read_data(22);
            quad->rgbc.rgbc = (uint32)(value);
        }
        packet += 40;
    next_effect_8f580:
        --count;
    } while ((sint32)count > 0);
    frame->packet = packet;
    a = context.rotation[0];
    b = context.rotation[1];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = context.rotation[2];
    b = context.rotation[3];
    c = context.rotation[4];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(7, 0);
    return;
}

uint32 scene_geometry_emit_bucket_copies(uint32 ordering, uint32 packet)
{
    uint32 *gpu_word;
    static const uint32 packet_template[6] = {0x05000000, 0x01000000, 0x80000000, 0x01180140, 0x01000140, 0x00180010};
    const uint16 *indices;
    uint32 first, second, third, fourth, fifth, sixth, i, index, bucket, tag, link;
    FUNCTION_MARKER(0x8005EFF0, "ACE3.BIN");
    first = packet_template[0];
    second = packet_template[1];
    third = packet_template[2];
    fourth = packet_template[3];
    fifth = packet_template[4];
    sixth = packet_template[5];
    indices = game_runtime.mission_depth_buckets;
    for (i = 0; i < 16; ++i)
    {
        DR_MOVE *copy = mission_packet_span(packet, sizeof(*copy));
        index = ((sint16)*indices);
        ++indices;
        copy->tag = (uint32)(first);
        copy->code[0] = (uint32)(second);
        bucket = ordering + (index << 2);
        copy->code[2] = (uint32)(fourth);
        copy->code[3] = (uint32)(fifth);
        copy->code[4] = (uint32)(sixth);
        copy->code[1] = (uint32)(third);
        tag = copy->tag;
        gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
        link = *gpu_word;
        copy->tag = (uint32)((tag & 0xFF000000) | (link & 0xFFFFFF));
        link = *gpu_word;
        *gpu_word = (link & 0xFF000000) | (packet & 0xFFFFFF);
        packet += 24;
    }
    return packet;
}

void scene_geometry_build_plane_view(const uint32 position[3], const uint16 angles[3], const uint16 viewport_size[2], SCENE_TERRAIN_VIEW *view)
{
    FUNCTION_MARKER(0x80061614, "ACE3.BIN");
    scene_geometry_intersect_plane_view(viewport_size, position, angles, view);
    scene_geometry_place_tile_view(view);
    {
        (void)(scene_geometry_build_view_row_mask(view));
        return;
    }
}

static void scene_geometry_emit_prelit_triangles(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    POLY_GT3 *poly;
    uint32 *ot_entry, *ordering;
    uint32 model, packet, count, i, indices, first, second, third, value, a, b, c, shift, bias, head;
    FUNCTION_MARKER(0x8006305C, "ACE3.BIN");
    model = stream->primitives;
    packet = stream->packet;
    ordering = state->local_ot;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, model, sizeof(SCENE_MESH_TRIANGLE_FACE));
    count = group.count;
    if ((sint32)count <= 0)
    {
        stream->packet = packet;
        {
            (void)(uint32)(model);
            return;
        }
    }
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        const SCENE_MESH_TRIANGLE_FACE *face = (const SCENE_MESH_TRIANGLE_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        first = indices & 255;
        second = (indices >> 8) & 255;
        third = indices >> 16;
        value = projected->xy[first].xy;
        xport_gte_write_data(12, value);
        value = projected->xy[second].xy;
        xport_gte_write_data(13, value);
        value = projected->xy[third].xy;
        xport_gte_write_data(14, value);
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        if ((sint32)value <= 0)
            continue;
        a = (sint16)projected->depth[first];
        c = (sint16)projected->depth[third];
        b = (sint16)projected->depth[second];
        xport_gte_write_data(17, a);
        xport_gte_write_data(18, b);
        xport_gte_write_data(19, c);
        xport_gte_execute(0x158002D);
        value = xport_gte_read_data(24);
        shift = state->level;
        value = (sint32)value >> ((shift + 10) & 31);
        bias = (sint16)state->depth_bias;
        value = value + 4 - bias;
        if (value + 75 >= 152)
            continue;
        ot_entry = ordering + 19 + ((sint32)value >> 2);
        head = *ot_entry;
        poly = (POLY_GT3 *)mission_packet_span(packet, sizeof(*poly));
        poly->tag = head | 0x09000000;
        *ot_entry = packet & 0xFFFFFF;
        value = xport_gte_read_data(12);
        poly->p[0].xy.xy = value;
        value = xport_gte_read_data(13);
        poly->p[1].xy.xy = value;
        value = xport_gte_read_data(14);
        poly->p[2].xy.xy = value;
        value = face->color[0];
        xport_gte_write_data(6, value);
        xport_gte_write_data(20, value);
        value = face->color[1];
        xport_gte_write_data(21, value);
        value = face->color[2];
        xport_gte_write_data(22, value);
        xport_gte_execute(0xF8002A);
        value = face->uv[0];
        poly->p[0].uv_attr = value;
        value = face->uv[1];
        poly->p[1].uv_attr = value;
        value = face->uv[2];
        poly->p[2].uv_attr = value;
        value = xport_gte_read_data(20);
        poly->p[0].rgbc.rgbc = value;
        value = xport_gte_read_data(21);
        poly->p[1].rgbc.rgbc = value;
        value = xport_gte_read_data(22);
        poly->p[2].rgbc.rgbc = value;
        packet += 40;
    }
    stream->packet = packet;
    return;
}

static void scene_geometry_emit_prelit_quads(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    POLY_GT4 *poly;
    uint32 *ot_entry, *ordering;
    uint32 model, packet, count, i, indices, first, second, third, fourth, first_xy, fourth_xy, value, a, b, c, d, bias, shift, head, visible;
    FUNCTION_MARKER(0x800631F8, "ACE3.BIN");
    model = stream->primitives;
    packet = stream->packet;
    ordering = state->local_ot;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, model, sizeof(SCENE_MESH_QUAD_FACE));
    count = group.count;
    if ((sint32)count <= 0)
    {
        stream->packet = packet;
        {
            (void)(uint32)(model);
            return;
        }
    }
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        const SCENE_MESH_QUAD_FACE *face = (const SCENE_MESH_QUAD_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        first = indices & 255;
        second = (indices >> 8) & 255;
        first_xy = projected->xy[first].xy;
        value = projected->xy[second].xy;
        xport_gte_write_data(13, value);
        third = (indices >> 16) & 255;
        value = projected->xy[third].xy;
        xport_gte_write_data(14, value);
        fourth = indices >> 24;
        xport_gte_write_data(12, first_xy);
        fourth_xy = projected->xy[fourth].xy;
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        visible = (sint32)value > 0;
        if (visible == 0)
        {
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            if ((sint32)value >= 0)
                continue;
        }
        a = (sint16)projected->depth[first];
        b = (sint16)projected->depth[second];
        c = (sint16)projected->depth[third];
        d = (sint16)projected->depth[fourth];
        bias = (sint16)state->depth_bias;
        shift = state->level;
        value = (sint32)(a + b + c + d) >> 2;
        value = (sint32)value >> (shift & 31);
        value = value + 4 - bias;
        if (value + 75 >= 152)
            continue;
        ot_entry = ordering + 19 + ((sint32)value >> 2);
        head = *ot_entry;
        poly = (POLY_GT4 *)mission_packet_span(packet, sizeof(*poly));
        poly->tag = head | 0x0C000000;
        *ot_entry = packet & 0xFFFFFF;
        poly->p[0].xy.xy = first_xy;
        value = xport_gte_read_data(13);
        poly->p[1].xy.xy = value;
        value = xport_gte_read_data(14);
        poly->p[2].xy.xy = value;
        value = xport_gte_read_data(12);
        poly->p[3].xy.xy = value;
        value = face->color[0];
        xport_gte_write_data(6, value);
        xport_gte_execute(0x780010);
        value = face->uv[0];
        poly->p[0].uv_attr = value;
        value = face->uv[1];
        poly->p[1].uv_attr = value;
        value = xport_gte_read_data(22);
        poly->p[0].rgbc.rgbc = value;
        value = face->color[1];
        xport_gte_write_data(20, value);
        value = face->color[2];
        xport_gte_write_data(21, value);
        value = face->color[3];
        xport_gte_write_data(22, value);
        xport_gte_execute(0xF8002A);
        value = face->uv[2];
        poly->p[2].uv_attr = value;
        poly->p[3].uv_attr = value >> 16;
        value = xport_gte_read_data(20);
        poly->p[1].rgbc.rgbc = value;
        value = xport_gte_read_data(21);
        poly->p[2].rgbc.rgbc = value;
        value = xport_gte_read_data(22);
        poly->p[3].rgbc.rgbc = value;
        packet += 52;
    }
    stream->packet = packet;
    return;
}

static void scene_geometry_emit_lit_triangles(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    SCENE_NORMAL_REFERENCE normals;
    POLY_GT3 *poly;
    uint32 *ot_entry, *ordering;
    uint32 packet, model, count, i, indices, first, second, third, a, b, c, value, shift, bias, head, n0, n1, n2;
    FUNCTION_MARKER(0x800633D8, "ACE3.BIN");
    packet = stream->packet;
    model = stream->primitives;
    ordering = state->local_ot;
    normals = state->normal_table;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, model, sizeof(SCENE_LIT_MESH_FACE));
    count = group.count;
    xport_gte_write_data(6, 0x34808080);
    if ((sint32)count <= 0)
    {
        stream->packet = packet;
        {
            (void)(uint32)(model);
            return;
        }
    }
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        const SCENE_LIT_MESH_FACE *face = (const SCENE_LIT_MESH_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        first = indices & 255;
        second = (indices >> 8) & 255;
        third = indices >> 16;
        value = projected->xy[first].xy;
        xport_gte_write_data(12, value);
        value = projected->xy[second].xy;
        xport_gte_write_data(13, value);
        value = projected->xy[third].xy;
        xport_gte_write_data(14, value);
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        if ((sint32)value <= 0)
            continue;
        a = (sint16)projected->depth[first];
        b = (sint16)projected->depth[second];
        c = (sint16)projected->depth[third];
        xport_gte_write_data(17, a);
        xport_gte_write_data(18, b);
        xport_gte_write_data(19, c);
        xport_gte_execute(0x158002D);
        value = xport_gte_read_data(24);
        shift = state->level;
        bias = (sint16)state->depth_bias;
        value = ((sint32)value >> ((shift + 10) & 31)) - bias;
        if (value + 75 >= 152)
            continue;
        const SCENE_LIT_FACE_DATA *face_data = &face->data;
        n0 = face_data->normal_indices[0];
        n1 = face_data->normal_indices[1];
        n2 = face_data->normal_indices[2];
        n0 = normals.offset + (n0 << 3);
        n1 = normals.offset + (n1 << 3);
        n2 = normals.offset + (n2 << 3);
        const uint8 *n0_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n0}, 8);
        gte_ldv0(n0_data);
        const uint8 *n1_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n1}, 8);
        gte_ldv1(n1_data);
        const uint8 *n2_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n2}, 8);
        gte_ldv2(n2_data);
        xport_gte_execute(0xD80420);
        ot_entry = ordering + 19 + ((sint32)value >> 2);
        head = *ot_entry;
        poly = (POLY_GT3 *)mission_packet_span(packet, sizeof(*poly));
        poly->tag = head | 0x09000000;
        *ot_entry = packet & 0xFFFFFF;
        a = face_data->uv[0];
        b = face_data->uv[1];
        c = face_data->uv[2];
        poly->p[0].uv_attr = a;
        poly->p[1].uv_attr = b;
        poly->p[2].uv_attr = c;
        a = xport_gte_read_data(12);
        poly->p[0].xy.xy = a;
        a = xport_gte_read_data(13);
        poly->p[1].xy.xy = a;
        a = xport_gte_read_data(14);
        poly->p[2].xy.xy = a;
        a = xport_gte_read_data(20);
        poly->p[0].rgbc.rgbc = a;
        a = xport_gte_read_data(21);
        poly->p[1].rgbc.rgbc = a;
        a = xport_gte_read_data(22);
        poly->p[2].rgbc.rgbc = a;
        packet += 40;
    }
    stream->packet = packet;
    return;
}

static void scene_geometry_emit_lit_quads(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    SCENE_NORMAL_REFERENCE normals;
    POLY_GT4 *poly;
    uint32 *ot_entry, *ordering;
    uint32 packet, model, count, i, indices, first, second, third, fourth, first_xy, fourth_xy, a, b, c, d, value, shift, bias, head, n0, n1, n2, n3;
    FUNCTION_MARKER(0x8006358C, "ACE3.BIN");
    packet = stream->packet;
    model = stream->primitives;
    ordering = state->local_ot;
    normals = state->normal_table;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, model, sizeof(SCENE_LIT_MESH_FACE));
    count = group.count;
    xport_gte_write_data(6, 0x3C808080);
    if ((sint32)count <= 0)
    {
        stream->packet = packet;
        {
            (void)(uint32)(model);
            return;
        }
    }
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        const SCENE_LIT_MESH_FACE *face = (const SCENE_LIT_MESH_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        first = indices & 255;
        second = (indices >> 8) & 255;
        first_xy = projected->xy[first].xy;
        value = projected->xy[second].xy;
        xport_gte_write_data(13, value);
        third = (indices >> 16) & 255;
        value = projected->xy[third].xy;
        xport_gte_write_data(14, value);
        fourth = indices >> 24;
        xport_gte_write_data(12, first_xy);
        fourth_xy = projected->xy[fourth].xy;
        xport_gte_execute(0x1400006);
        value = xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        if ((sint32)value <= 0)
        {
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            if ((sint32)value >= 0)
                continue;
        }
        a = (sint16)projected->depth[first];
        b = (sint16)projected->depth[second];
        c = (sint16)projected->depth[third];
        d = (sint16)projected->depth[fourth];
        bias = (sint16)state->depth_bias;
        shift = state->level;
        value = (sint32)(a + b + c + d) >> 2;
        value = ((sint32)value >> (shift & 31)) - bias;
        if (value + 75 >= 152)
            continue;
        const SCENE_LIT_FACE_DATA *face_data = &face->data;
        n0 = normals.offset + (face_data->normal_indices[0] << 3);
        const uint8 *n0_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n0}, 8);
        gte_ldv0(n0_data);
        xport_gte_execute(0xC8041E);
        n1 = face_data->normal_indices[1];
        n2 = face_data->normal_indices[2];
        n3 = face_data->normal_indices[3];
        n1 = normals.offset + (n1 << 3);
        n2 = normals.offset + (n2 << 3);
        n3 = normals.offset + (n3 << 3);
        const uint8 *n3_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n3}, 8);
        gte_ldv2(n3_data);
        const uint8 *n2_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n2}, 8);
        gte_ldv1(n2_data);
        a = xport_gte_read_data(22);
        poly = (POLY_GT4 *)mission_packet_span(packet, sizeof(*poly));
        poly->p[0].rgbc.rgbc = a;
        const uint8 *n1_data = scene_normal_data((SCENE_NORMAL_REFERENCE){normals.view, n1}, 8);
        gte_ldv0(n1_data);
        xport_gte_execute(0xD80420);
        ot_entry = ordering + 19 + ((sint32)value >> 2);
        head = *ot_entry;
        poly->tag = head | 0x0C000000;
        *ot_entry = packet & 0xFFFFFF;
        a = face_data->uv[0];
        poly->p[0].xy.xy = first_xy;
        poly->p[0].uv_attr = a;
        c = face_data->uv[2];
        b = face_data->uv[1];
        poly->p[2].uv_attr = c;
        poly->p[1].uv_attr = b;
        poly->p[3].uv_attr = c >> 16;
        a = xport_gte_read_data(13);
        poly->p[1].xy.xy = a;
        a = xport_gte_read_data(14);
        poly->p[2].xy.xy = a;
        a = xport_gte_read_data(12);
        poly->p[3].xy.xy = a;
        a = xport_gte_read_data(20);
        poly->p[1].rgbc.rgbc = a;
        a = xport_gte_read_data(21);
        poly->p[2].rgbc.rgbc = a;
        a = xport_gte_read_data(22);
        poly->p[3].rgbc.rgbc = a;
        packet += 52;
    }
    stream->packet = packet;
    return;
}

void scene_terrain_change_tile(uint32 kind, uint32 index, sint32 value)
{
    uint16 *map;
    uint32 cell, old;
    FUNCTION_MARKER(0x8005E460, "ACE3.BIN");
    map = game_runtime.scene_assets.tile_map;
    cell = (index << 8) + kind;
    old = map[cell];
    if (value != 0)
    {
        map[cell] = ((old & 0xFE00) | (uint32)value);
        if (game_runtime.mission_control.flags & 2)
            mission_replay_record_tile_change(kind, index, value, old & 0x1FF);
    }
    {
        (void)(uint32)(old & 0x1FF);
        return;
    }
}

uint32 scene_terrain_polygon_has_consistent_winding(const uint32 points[][4], uint32 count)
{
    uint32 current, previous, x, y, positives, i;
    FUNCTION_MARKER(0x8005F1C4, "ACE3.BIN");
    x = (sint32)points[count - 1][0] >> 1;
    y = (sint32)points[count - 1][2] >> 1;
    current = (x & 0xFFFF) | (y << 16);
    positives = 0;
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        previous = current;
        x = (sint32)points[0][0] >> 1;
        y = (sint32)points[0][2] >> 1;
        current = (x & 0xFFFF) | (y << 16);
        if ((sint32)game_triangle_signed_area_gte(0, previous, current) > 0)
            ++positives;
        points += 1;
    }
    return positives == 0 || positives == count;
}

typedef struct
{
    uint32 screen[4];
    uint32 depth[4];
    const uint8 *intensity_data;
} SCENE_QUAD_PROJECTION_INPUT;

static SCENE_QUAD_PROJECTION_INPUT scene_subdiv_read_quad_projection(const SCENE_SPRITE_PROJECTION_VIEW *projection, uint32 indices)
{
    static const uint8 order[2][4] = {{0, 1, 2, 3}, {1, 0, 3, 2}};
    SCENE_QUAD_PROJECTION_INPUT quad = {0};
    uint32 component, step, lane, vertex, offset, value;
    int wire = !projection;

    if (projection)
    {
        if (!projection->vertices)
            abort();
        wire = (indices & 255) >= projection->count || ((indices >> 8) & 255) >= projection->count || ((indices >> 16) & 255) >= projection->count || (indices >> 24) >= projection->count;
        if (wire && !projection->legacy_outside)
            abort();
    }
    for (component = 0; component < 2; ++component)
    {
        for (step = 0; step < 4; ++step)
        {
            lane = order[component][step];
            vertex = (indices >> (lane * 8)) & 255;
            offset = vertex * sizeof(SCENE_SPRITE_PROJECTION) + component * sizeof(uint32);
            if (!wire)
                value = component ? projection->vertices[vertex].depth : projection->vertices[vertex].screen.xy;
            else if (projection && projection->wire_data && offset <= projection->wire_bytes && sizeof(value) <= projection->wire_bytes - offset)
                memcpy(&value, projection->wire_data + offset, sizeof(value));
            else
                value = 0;
            if (component)
                quad.depth[lane] = value;
            else
                quad.screen[lane] = value;
        }
    }
    vertex = indices & 255;
    offset = vertex * sizeof(SCENE_SPRITE_PROJECTION) + offsetof(SCENE_SPRITE_PROJECTION, intensity);
    if (!wire)
        quad.intensity_data = (const uint8 *)&projection->vertices[vertex].intensity;
    else if (projection && projection->wire_data && offset <= projection->wire_bytes && sizeof(uint32) <= projection->wire_bytes - offset)
        quad.intensity_data = projection->wire_data + offset;
    return quad;
}

static uint32 scene_subdiv_read_quad_intensity(const SCENE_QUAD_PROJECTION_INPUT *quad)
{
    uint32 value;
    if (!quad->intensity_data)
        return 0;
    memcpy(&value, quad->intensity_data, sizeof(value));
    return value;
}

uint32 scene_subdiv_draw_flat_quad(SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input, const SCENE_SPRITE_PROJECTION_VIEW *projection)
{
    POLY_FT4 *poly;
    uint32 *ot_entry;
    uint32 model, packet, ot, count, index, indices, xy0, xy3;
    uint32 depth0, depth1, depth2, depth3, depth, area, target, value, a, b, c, result;
    FUNCTION_MARKER(0x800619E0, "ACE3.BIN");
    model = stream->primitives;
    packet = stream->packet;
    ot = stream->ordering;
    SCENE_FACE_GROUP faces = input ? input->faces : scene_geometry_face_group(model, sizeof(SCENE_FLAT_QUAD_FACE));
    count = faces.count;
    result = model;
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        const SCENE_FLAT_QUAD_FACE *face = (const SCENE_FLAT_QUAD_FACE *)(faces.records + index * sizeof(*face));
        indices = face->texture.projected_indices;
        SCENE_QUAD_PROJECTION_INPUT quad_projection = scene_subdiv_read_quad_projection(projection, indices);
        xy0 = quad_projection.screen[0];
        value = quad_projection.screen[1];
        xport_gte_write_data(13, value);
        value = quad_projection.screen[2];
        xport_gte_write_data(14, value);
        xport_gte_write_data(12, xy0);
        xy3 = quad_projection.screen[3];
        xport_gte_execute(0x1400006);
        depth1 = quad_projection.depth[1];
        depth0 = quad_projection.depth[0];
        depth = (sint32)depth1 < (sint32)depth0 ? depth0 : depth1;
        depth3 = quad_projection.depth[3];
        depth2 = quad_projection.depth[2];
        if ((sint32)depth3 < (sint32)depth2)
            depth3 = depth2;
        if ((sint32)depth3 < (sint32)depth)
            depth3 = depth;
        depth = depth3;
        area = xport_gte_read_data(24);
        xport_gte_write_data(12, xy3);
        if ((sint32)area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = xport_gte_read_data(24);
            if ((sint32)area >= 0)
            {
                result = index + 1 < count;
                continue;
            }
        }
        if (depth - 121 >= 0x777F)
        {
            result = index + 1 < count;
            continue;
        }
        depth = ((sint32)depth >> 3);
        if ((sint32)depth >= 0x38F)
        {
            value = scene_subdiv_read_quad_intensity(&quad_projection);
            xport_gte_write_data(8, value);
            value = face->color;
            xport_gte_write_data(6, value);
            target = ot + (depth << 2);
            poly = (POLY_FT4 *)mission_packet_span(packet, sizeof(*poly));
            poly->p[0].xy.xy = xy0;
            ot_entry = (uint32 *)mission_ot_span(target, sizeof(*ot_entry));
            value = *ot_entry;
            poly->tag = value | 0x09000000;
            *ot_entry = packet & 0xFFFFFF;
            xport_gte_execute(0x780010);
            a = face->texture.uv[2];
            b = face->texture.uv[0];
            c = face->texture.uv[1];
            poly->p[2].uv_attr = a;
            poly->p[0].uv_attr = b;
            poly->p[1].uv_attr = c;
            poly->p[3].uv_attr = a >> 16;
            value = xport_gte_read_data(22);
            poly->rgbc.rgbc = value;
            value = xport_gte_read_data(13);
            poly->p[1].xy.xy = value;
            value = xport_gte_read_data(14);
            poly->p[2].xy.xy = value;
            value = xport_gte_read_data(12);
            poly->p[3].xy.xy = value;
            packet += 40;
        }
        else
        {
            scratch->depth = depth;
            value = face->texture.flags;
            scratch->flat_polygon.texture.flags = value;
            a = face->texture.vertex_offsets[0];
            b = face->texture.vertex_offsets[1];
            scratch->flat_polygon.texture.vertex_offsets[0] = a;
            scratch->flat_polygon.texture.vertex_offsets[1] = b;
            a = face->texture.uv[0];
            b = face->texture.uv[1];
            c = face->texture.uv[2];
            scratch->flat_polygon.texture.uv[2] = c;
            scratch->flat_polygon.texture.uv[0] = a;
            scratch->flat_polygon.texture.uv[1] = b;
            value = (face->color & 0xFFFFFF) | 0x3C000000;
            scratch->flat_polygon.color[0] = value;
            scratch->flat_polygon.color[1] = value;
            scratch->flat_polygon.color[2] = value;
            scratch->flat_polygon.color[3] = value;
            stream->packet = packet;
            packet = scene_subdiv_draw_gouraud_face(&scratch->flat_polygon, scratch, stream, input);
        }
        result = (sint32)(index + 1) < (sint32)count;
    }
    stream->packet = packet;
    return result;
}

void scene_subdiv_draw_gouraud_quad(SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input, const SCENE_SPRITE_PROJECTION_VIEW *projection)
{
    POLY_GT4 *poly;
    uint32 *ot_entry;
    uint32 model, packet, ot, count, index, indices, xy0, xy3;
    uint32 depth0, depth1, depth2, depth3, depth, area, target, value, a, b, c, result;
    FUNCTION_MARKER(0x80061C3C, "ACE3.BIN");
    model = stream->primitives;
    packet = stream->packet;
    ot = stream->ordering;
    SCENE_FACE_GROUP faces = input ? input->faces : scene_geometry_face_group(model, sizeof(SCENE_GOURAUD_QUAD_FACE));
    count = faces.count;
    result = model;
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        const SCENE_GOURAUD_QUAD_FACE *face = (const SCENE_GOURAUD_QUAD_FACE *)(faces.records + index * sizeof(*face));
        indices = face->texture.projected_indices;
        SCENE_QUAD_PROJECTION_INPUT quad_projection = scene_subdiv_read_quad_projection(projection, indices);
        xy0 = quad_projection.screen[0];
        value = quad_projection.screen[1];
        xport_gte_write_data(13, value);
        value = quad_projection.screen[2];
        xport_gte_write_data(14, value);
        xport_gte_write_data(12, xy0);
        xy3 = quad_projection.screen[3];
        xport_gte_execute(0x1400006);
        depth1 = quad_projection.depth[1];
        depth0 = quad_projection.depth[0];
        depth = (sint32)depth1 < (sint32)depth0 ? depth0 : depth1;
        depth3 = quad_projection.depth[3];
        depth2 = quad_projection.depth[2];
        if ((sint32)depth3 < (sint32)depth2)
            depth3 = depth2;
        if ((sint32)depth3 < (sint32)depth)
            depth3 = depth;
        depth = depth3;
        area = xport_gte_read_data(24);
        xport_gte_write_data(12, xy3);
        if ((sint32)area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = xport_gte_read_data(24);
            if ((sint32)area >= 0)
            {
                result = index + 1 < count;
                continue;
            }
        }
        if (depth - 121 >= 0x777F)
        {
            result = index + 1 < count;
            continue;
        }
        depth = ((sint32)depth >> 3);
        if ((sint32)depth >= 0x38F)
        {
            value = scene_subdiv_read_quad_intensity(&quad_projection);
            xport_gte_write_data(8, value);
            value = face->color[0];
            xport_gte_write_data(6, value);
            value = face->color[1];
            xport_gte_write_data(20, value);
            value = face->color[2];
            xport_gte_write_data(21, value);
            value = face->color[3];
            xport_gte_write_data(22, value);
            poly = (POLY_GT4 *)mission_packet_span(packet, sizeof(*poly));
            poly->p[0].xy.xy = xy0;
            xport_gte_execute(0xF8002A);
            target = ot + (depth << 2);
            ot_entry = (uint32 *)mission_ot_span(target, sizeof(*ot_entry));
            value = *ot_entry;
            poly->tag = value | 0x0C000000;
            *ot_entry = packet & 0xFFFFFF;
            a = face->texture.uv[2];
            b = face->texture.uv[0];
            c = face->texture.uv[1];
            value = xport_gte_read_data(20);
            poly->p[1].rgbc.rgbc = value;
            value = xport_gte_read_data(21);
            poly->p[2].rgbc.rgbc = value;
            value = xport_gte_read_data(22);
            poly->p[3].rgbc.rgbc = value;
            poly->p[0].uv_attr = b;
            poly->p[1].uv_attr = c;
            xport_gte_execute(0x780010);
            poly->p[2].uv_attr = a;
            poly->p[3].uv_attr = a >> 16;
            value = xport_gte_read_data(22);
            poly->p[0].rgbc.rgbc = value;
            value = xport_gte_read_data(13);
            poly->p[1].xy.xy = value;
            value = xport_gte_read_data(14);
            poly->p[2].xy.xy = value;
            value = xport_gte_read_data(12);
            poly->p[3].xy.xy = value;
            packet += 52;
        }
        else
        {
            scratch->depth = depth;
            stream->packet = packet;
            packet = scene_subdiv_draw_gouraud_face(face, scratch, stream, input);
        }
        result = (sint32)(index + 1) < (sint32)count;
    }
    stream->packet = packet;
    {
        (void)(uint32)(result);
        return;
    }
}

static uint32 scene_subdiv_draw_gouraud_face(const SCENE_GOURAUD_QUAD_FACE *polygon, SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input)
{
    SCENE_SPRITE_SCRATCH *work = (SCENE_SPRITE_SCRATCH *)getScratchAddr(0);
    SCENE_SUBDIV_FRAME *frames;
    uint32 value, indices, table, a, b, c;
    uint16 wire_z;
    FUNCTION_MARKER(0x8006258C, "ACE3.BIN");
    value = (uint16)(polygon->texture.uv[1] >> 16);
    scratch->tpage = (uint16)value;
    value = (uint16)(polygon->texture.uv[0] >> 16);
    frames = work->frames;
    scratch->clut = (uint16)value;
    value = polygon->color[0];
    xport_gte_write_data(6, value);
    value = polygon->color[1];
    xport_gte_write_data(20, value);
    value = polygon->color[2];
    xport_gte_write_data(21, value);
    value = polygon->color[3];
    xport_gte_write_data(22, value);
    indices = polygon->texture.vertex_offsets[1];
    table = scratch->vertex_table;
    if (input)
    {
        uint32 xy_offset = (indices & 255);
        uint32 z_offset = ((indices >> 8) & 255);
        uint16 native_z;
        if (input->vertices.bytes < 4 || xy_offset > input->vertices.bytes - 4 || z_offset > input->vertices.bytes - 2)
            abort();
        memcpy(&a, input->vertices.data + xy_offset, sizeof(a));
        memcpy(&native_z, input->vertices.data + z_offset, sizeof(native_z));
        b = native_z;
    }
    else
    {
        memcpy(&a, mission_packet_span(table + (indices & 255), sizeof(a)), sizeof(a));
        memcpy(&wire_z, mission_packet_span(table + ((indices >> 8) & 255), sizeof(wire_z)), sizeof(wire_z));
        b = wire_z;
    }
    frames->vertex[2].z = (uint16)(b);
    frames->vertex[2].position.xy = a;
    if (input)
    {
        uint32 xy_offset = ((indices >> 16) & 255);
        uint32 z_offset = (indices >> 24);
        uint16 native_z;
        if (input->vertices.bytes < 4 || xy_offset > input->vertices.bytes - 4 || z_offset > input->vertices.bytes - 2)
            abort();
        memcpy(&a, input->vertices.data + xy_offset, sizeof(a));
        memcpy(&native_z, input->vertices.data + z_offset, sizeof(native_z));
        b = native_z;
    }
    else
    {
        memcpy(&a, mission_packet_span(table + ((indices >> 16) & 255), sizeof(a)), sizeof(a));
        memcpy(&wire_z, mission_packet_span(table + (indices >> 24), sizeof(wire_z)), sizeof(wire_z));
        b = wire_z;
    }
    indices = polygon->texture.vertex_offsets[0];
    xport_gte_write_data(0, a);
    xport_gte_write_data(1, b);
    frames->vertex[3].position.xy = a;
    frames->vertex[3].z = (uint16)(b);
    xport_gte_execute(0x180001);
    if (input)
    {
        uint32 xy_offset = (indices & 255);
        uint32 z_offset = ((indices >> 8) & 255);
        uint16 native_z;
        if (input->vertices.bytes < 4 || xy_offset > input->vertices.bytes - 4 || z_offset > input->vertices.bytes - 2)
            abort();
        memcpy(&a, input->vertices.data + xy_offset, sizeof(a));
        memcpy(&native_z, input->vertices.data + z_offset, sizeof(native_z));
        b = native_z;
    }
    else
    {
        memcpy(&a, mission_packet_span(table + (indices & 255), sizeof(a)), sizeof(a));
        memcpy(&wire_z, mission_packet_span(table + ((indices >> 8) & 255), sizeof(wire_z)), sizeof(wire_z));
        b = wire_z;
    }
    frames->vertex[0].position.xy = a;
    frames->vertex[0].z = (uint16)(b);
    xport_gte_execute(0xF8002A);
    if (input)
    {
        uint32 xy_offset = ((indices >> 16) & 255);
        uint32 z_offset = (indices >> 24);
        uint16 native_z;
        if (input->vertices.bytes < 4 || xy_offset > input->vertices.bytes - 4 || z_offset > input->vertices.bytes - 2)
            abort();
        memcpy(&a, input->vertices.data + xy_offset, sizeof(a));
        memcpy(&native_z, input->vertices.data + z_offset, sizeof(native_z));
        b = native_z;
    }
    else
    {
        memcpy(&a, mission_packet_span(table + ((indices >> 16) & 255), sizeof(a)), sizeof(a));
        memcpy(&wire_z, mission_packet_span(table + (indices >> 24), sizeof(wire_z)), sizeof(wire_z));
        b = wire_z;
    }
    value = xport_gte_read_data(20);
    frames->vertex[1].color = value;
    value = xport_gte_read_data(21);
    frames->vertex[2].color = value;
    value = xport_gte_read_data(22);
    frames->vertex[3].color = value;
    frames->vertex[1].position.xy = a;
    frames->vertex[1].z = (uint16)(b);
    xport_gte_execute(0x780010);
    a = polygon->texture.uv[2];
    b = (uint16)polygon->texture.uv[0];
    c = (uint16)polygon->texture.uv[1];
    frames->vertex[2].uv = (uint16)(a);
    frames->vertex[0].uv = (uint16)(b);
    frames->vertex[1].uv = (uint16)(c);
    frames->vertex[3].uv = (uint16)(a >> 16);
    value = xport_gte_read_data(22);
    frames->vertex[0].color = value;
    value = xport_gte_read_data(14);
    frames->screen[3].xy = value;
    value = xport_gte_read_data(19);
    frames->depth[3] = (uint16)value;
    scene_subdiv_draw_frame(frames, scratch, 4, 0, stream);
    return stream->packet;
}

static void scene_subdiv_midpoint(SCENE_SUBDIV_VERTEX *output, const SCENE_SUBDIV_VERTEX *first, const SCENE_SUBDIV_VERTEX *second)
{
    uint32 a, b, value;
    FUNCTION_MARKER(0x80061E50, "ACE3.BIN");
    output->position.x = (sint16)(((sint32)first->position.x + second->position.x) >> 1);
    output->position.y = (sint16)(((sint32)first->position.y + second->position.y) >> 1);
    output->z = (uint16)(((sint32)(sint16)first->z + (sint16)second->z) >> 1);
    a = first->color;
    b = second->color;
    value = ((sint32)((a & 0xFEFEFEFF) + (b & 0xFEFEFEFF)) >> 1);
    output->color = value;
    a = (sint16)first->uv;
    b = (sint16)second->uv;
    value = ((sint32)((a & 0xFEFEFEFF) + (b & 0xFEFEFEFF)) >> 1);
    output->uv = (uint16)value;
    {
        (void)(uint32)(value);
        return;
    }
}

static void scene_subdiv_emit_triangle(const SCENE_SUBDIV_VERTEX *first, const SCENE_SUBDIV_VERTEX *second, const SCENE_SUBDIV_VERTEX *third, const SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream)
{
    uint32 value, a, b, packet, depth, ot, result;
    POLY_GT3 *triangle;
    uint32 *ot_entry;
    FUNCTION_MARKER(0x80061ED4, "ACE3.BIN");
    value = first->position.xy;
    xport_gte_write_data(0, value);
    value = first->z;
    xport_gte_write_data(1, value);
    value = second->position.xy;
    xport_gte_write_data(2, value);
    value = second->z;
    xport_gte_write_data(3, value);
    value = third->position.xy;
    xport_gte_write_data(4, value);
    value = third->z;
    xport_gte_write_data(5, value);
    a = first->color;
    packet = stream->packet;
    xport_gte_execute(0x280030);
    b = second->color;
    triangle = mission_packet_span(packet, sizeof(*triangle));
    triangle->p[0].rgbc.rgbc = a;
    triangle->p[1].rgbc.rgbc = b;
    a = third->uv;
    b = third->color;
    triangle->p[2].uv.uv = (uint16)a;
    triangle->p[2].rgbc.rgbc = b;
    a = first->uv;
    b = second->uv;
    triangle->p[0].uv.uv = (uint16)a;
    triangle->p[1].uv.uv = (uint16)b;
    a = (sint16)scratch->clut;
    b = (sint16)scratch->tpage;
    triangle->p[0].rgbc.code = 0x34;
    triangle->p[0].attr = (uint16)a;
    triangle->p[1].attr = (uint16)b;
    depth = scratch->depth;
    result = xport_gte_read_control(31);
    depth <<= 2;
    if ((sint32)result < 0)
    {
        (void)(uint32)(result);
        return;
    }
    value = stream->packet;
    stream->packet = value + 40;
    ot = stream->ordering;
    ot_entry = mission_ot_span(ot + depth, sizeof(*ot_entry));
    value = *ot_entry;
    triangle->tag = value | 0x09000000;
    ot = stream->ordering;
    result = packet & 0xFFFFFF;
    *(uint32 *)mission_ot_span(ot + depth, sizeof(uint32)) = result;
    value = xport_gte_read_data(12);
    triangle->p[0].xy.xy = value;
    value = xport_gte_read_data(13);
    triangle->p[1].xy.xy = value;
    value = xport_gte_read_data(14);
    triangle->p[2].xy.xy = value;
    {
        (void)(uint32)(result);
        return;
    }
}

static uint32 scene_subdiv_draw_frame(SCENE_SUBDIV_FRAME *frame, const SCENE_SUBDIV_SCRATCH_HEADER *scratch, uint32 count, uint32 mode, SCENE_RENDER_STREAM *stream)
{
    static const uint8 endpoint0[3] = {1, 2, 0}, endpoint1[3] = {3, 3, 2};
    static const uint8 copied[3] = {1, 3, 2};
    static const uint8 mode_delta[3] = {3, 0, 1}, bridge0[3] = {1, 3, 2}, bridge1[3] = {3, 2, 0};
    SCENE_SUBDIV_FRAME *next;
    POLY_GT4 *quad;
    uint32 *ot_entry;
    uint32 value, packet, depth, index, flags, inside, all_outside, a, b, c, d, area0, area1, sum, ot, level;
    uint32 colors[4], uv[4], xy[4], order[4] = {0, 1, 2, 3}, stage;
    sint32 x, y;
    FUNCTION_MARKER(0x80061FA8, "ACE3.BIN");
    for (index = 0; index < 3; ++index)
    {
        value = frame->vertex[index].position.xy;
        xport_gte_write_data(index * 2, value);
        value = frame->vertex[index].z;
        xport_gte_write_data(index * 2 + 1, value);
    }
    next = frame + 1;
    xport_gte_execute(0x280030);
    packet = stream->packet;
    quad = mission_packet_span(packet, sizeof(*quad));
    value = scratch->tpage;
    depth = scratch->depth;
    quad->p[1].attr = (uint16)value;
    value = scratch->clut;
    quad->p[0].attr = (uint16)value;
    value = xport_gte_read_data(12);
    frame->screen[0].xy = value;
    value = xport_gte_read_data(13);
    frame->screen[1].xy = value;
    value = xport_gte_read_data(14);
    frame->screen[2].xy = value;
    flags = 0;
    all_outside = 1;
    for (index = 0; index < 4; ++index)
    {
        x = frame->screen[index].x;
        inside = 0;
        if (x < -160)
            flags |= (uint32)1 << index;
        else if (x >= 161)
            flags |= (uint32)16 << index;
        else
            inside = 1;
        y = frame->screen[index].y;
        if (y < -120)
            flags |= (uint32)256 << index;
        else if (y >= 121)
            flags |= (uint32)4096 << index;
        else
            ++inside;
        if (inside >= 2)
        {
            all_outside = 0;
            break;
        }
    }
    if (all_outside && ((flags & 15) == 15 || ((flags >> 4) & 15) == 15 || ((flags >> 8) & 15) == 15 || ((flags >> 12) & 15) == 15))
        return 0;
    value = xport_gte_read_data(17);
    frame->depth[0] = (uint16)value;
    value = xport_gte_read_data(18);
    frame->depth[1] = (uint16)value;
    value = xport_gte_read_data(19);
    frame->depth[2] = (uint16)value;
    xport_gte_execute(0x1400006);
    a = (sint16)frame->depth[1];
    b = (sint16)frame->depth[0];
    c = frame->depth[1];
    d = frame->depth[0];
    if ((sint32)a < (sint32)b)
        c = d;
    a = (sint16)frame->depth[3];
    b = (sint16)frame->depth[2];
    d = frame->depth[2];
    if ((sint32)a < (sint32)b)
        a = d;
    if ((sint32)(a << 16) < (sint32)(c << 16))
        a = c;
    if (((sint32)(a << 16) >> 16) < 31)
        return 0;
    area0 = xport_gte_read_data(24);
    value = frame->screen[3].xy;
    xport_gte_write_data(15, value);
    if ((sint32)area0 < 0)
        area0 = 0 - area0;
    xport_gte_execute(0x1400006);
    area1 = xport_gte_read_data(24);
    if ((sint32)area1 < 0)
        area1 = 0 - area1;
    sum = area0 + area1;
    if ((sint32)sum <= 0x13FFE && (sint32)count >= 4)
        count = 3;
    if ((sint32)sum < 0x703F && (sint32)count >= 3)
        count = 2;
    if ((sint32)sum < 0x307F && (sint32)count >= 2)
        count = 1;
    if ((sint32)sum < 0x14FF && (sint32)count > 0)
        count = 0;
    level = count - 1;
    if ((sint32)level < 0)
    {
        if (mode & 1)
        {
            order[0] = 1;
            order[1] = 3;
            order[2] = 0;
            order[3] = 2;
        }
        for (index = 0; index < 4; ++index)
            colors[index] = frame->vertex[order[index]].color;
        for (index = 0; index < 4; ++index)
            uv[index] = frame->vertex[order[index]].uv;
        for (index = 0; index < 4; ++index)
            xy[index] = frame->screen[order[index]].xy;
        for (index = 0; index < 4; ++index)
            quad->p[index].rgbc.rgbc = colors[index];
        for (index = 0; index < 4; ++index)
            quad->p[index].uv.uv = (uint16)uv[index];
        for (index = 0; index < 4; ++index)
            quad->p[index].xy.xy = xy[index];
        ot = stream->ordering;
        depth <<= 2;
        ot_entry = mission_ot_span(ot + depth, sizeof(*ot_entry));
        value = *ot_entry;
        quad->tag = value | 0x0C000000;
        ot = stream->ordering;
        *(uint32 *)mission_ot_span(ot + depth, sizeof(uint32)) = packet & 0xFFFFFF;
        value = stream->packet;
        stream->packet = value + 52;
        return 1;
    }
    scene_subdiv_midpoint(&next->vertex[0], &frame->vertex[mode & 1 ? 0 : 1], &frame->vertex[mode & 1 ? 3 : 2]);
    scene_subdiv_midpoint(&next->vertex[1], &frame->vertex[0], &frame->vertex[2]);
    scene_subdiv_midpoint(&next->vertex[2], &frame->vertex[0], &frame->vertex[1]);
    next->vertex[3] = frame->vertex[0];
    next->screen[3] = frame->screen[0];
    next->depth[3] = frame->depth[0];
    if (scene_subdiv_draw_frame(next, scratch, level, mode + 2, stream))
        scene_subdiv_emit_triangle(&frame->vertex[0], &frame->vertex[1], &next->vertex[2], scratch, stream);
    for (stage = 0; stage < 3; ++stage)
    {
        next->vertex[1] = next->vertex[2];
        scene_subdiv_midpoint(&next->vertex[2], &frame->vertex[endpoint0[stage]], &frame->vertex[endpoint1[stage]]);
        next->vertex[3] = frame->vertex[copied[stage]];
        next->screen[3] = frame->screen[copied[stage]];
        next->depth[3] = frame->depth[copied[stage]];
        if (scene_subdiv_draw_frame(next, scratch, level, mode + mode_delta[stage], stream))
            scene_subdiv_emit_triangle(&frame->vertex[bridge0[stage]], &frame->vertex[bridge1[stage]], &next->vertex[2], scratch, stream);
    }
    return 1;
}

void scene_terrain_draw_tiles(const SCENE_TERRAIN_TILE *records, uint32 tile_count, const TERRAIN_MODEL_TABLE *models, const uint32 origin[3], uint32 *previous, uint32 *current, SCENE_RENDER_STREAM *stream)
{
    uint32 *scratch_words = getScratchAddr(0);
    SCENE_SPRITE_SCRATCH *work = (SCENE_SPRITE_SCRATCH *)scratch_words;
    SCENE_SPRITE_PROJECTION *native_projection = work->vertices;
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 tile, flags, a, b, c, count, slot, saved, depth;
    uint32 value, index, reg;
    uint32 *swap;
    FUNCTION_MARKER(0x80061768, "ACE3.BIN");
    xport_gte_write_control(21, 0x830);
    xport_gte_write_control(22, 0x830);
    xport_gte_write_control(23, 0x830);
    for (tile = 0; tile < tile_count; ++tile)
    {
        flags = records[tile].flags;
        if (flags & 0x200000)
            return;
        const SCENE_TERRAIN_MODEL_HEADER *header = terrain_model_header(models, flags & 0x1FF);
        a = origin[0];
        b = origin[1];
        c = origin[2];
        xport_gte_write_control(5, a);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        value = records[tile].x | ((uint32)records[tile].y << 16);
        xport_gte_write_data(0, value);
        xport_gte_write_data(1, records[tile].z);
        if (flags & 0x400000)
        {
            swap = previous;
            previous = current;
            current = swap;
        }
        xport_gte_execute(0x480012);
        slot = (flags >> 16) & 31;
        RESOURCE_ASSET_SOURCE vertices = terrain_model_vertices(models, header);
        SCENE_SPRITE_PROJECTION *nearby = work->vertices;
        saved = nearby[3].screen.xy;
        const uint8 *input = vertices.data + 4;
        count = scene_geometry_read_vertex_count(vertices.data);
        a = xport_gte_read_data(25);
        b = xport_gte_read_data(26);
        xport_gte_write_control(5, a);
        depth = xport_gte_read_data(27);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, depth);
        if ((sint32)depth >= 0x7001)
        {
            if (vertices.bytes < 40)
                abort();
            const uint8 *far_vertex = input + 20;
            value = (memcpy(&wire_word_0, far_vertex, sizeof(wire_word_0)), wire_word_0);
            xport_gte_write_data(0, value);
            value = (memcpy(&wire_word_1, far_vertex + 12, sizeof(wire_word_1)), wire_word_1);
            xport_gte_write_data(1, value);
            xport_gte_execute(0x180001);
            value = xport_gte_read_data(14);
            nearby[3].screen.xy = value;
            current[slot] = value;
        }
        else
        {
            SCENE_SPRITE_PROJECTION_VIEW projection = {native_projection, 0, 1, (const uint8 *)scratch_words + SCENE_SPRITE_PROJECTION_OFFSET, sizeof(SCRATCHPAD) - SCENE_SPRITE_PROJECTION_OFFSET};
            uint32 next_count = count - 3u;
            uint32 triples = 1;
            if (next_count && next_count <= INT32_MAX)
                triples += (next_count - 1) / 3 + 1;
            if (triples > (vertices.bytes - 4) / SCENE_VERTEX_TRIPLE_BYTES || triples > SCENE_SPRITE_PROJECTION_CAPACITY / 3)
                abort();
            SCENE_VERTEX_TRIPLE triple = scene_geometry_read_vertex_triple(input);
            do
            {
                xport_gte_write_data(0, triple.xy[0]);
                xport_gte_write_data(2, triple.xy[1]);
                xport_gte_write_data(4, triple.xy[2]);
                xport_gte_write_data(1, triple.z01);
                xport_gte_write_data(3, triple.z01 >> 16);
                xport_gte_write_data(5, triple.z2);
                input += SCENE_VERTEX_TRIPLE_BYTES;
                xport_gte_execute(0x280030);
                count -= 3;
                if ((sint32)count > 0)
                    triple = scene_geometry_read_vertex_triple(input);
                SCENE_SPRITE_PROJECTION *projected = native_projection + projection.count;
                for (index = 0; index < 3; ++index)
                {
                    reg = 12 + index;
                    value = xport_gte_read_data(reg);
                    projected[index].screen.xy = value;
                    reg = 17 + index;
                    value = xport_gte_read_data(reg);
                    projected[index].depth = value;
                    value = xport_gte_read_data(8);
                    projected[index].intensity = value;
                }
                projection.count += 3;
            } while ((sint32)count > 0);
            flags ^= flags & (flags << 16);
            if (flags & 0x8000000)
                nearby[2].screen.xy = saved;
            if (flags & 0x2000000)
            {
                saved = previous[slot - 1];
                nearby[0].screen.xy = saved;
            }
            if (flags & 0x4000000)
            {
                saved = previous[slot];
                nearby[1].screen.xy = saved;
            }
            current[slot] = nearby[3].screen.xy;
            value = header->vertices;
            SCENE_SUBDIV_SCRATCH_HEADER *scratch = &work->subdivision;
            scratch->vertex_table = value + 4;
            SCENE_SUBDIV_INPUT subdivision = {0};
            subdivision.vertices = vertices;
            subdivision.vertices.data += 4;
            subdivision.vertices.bytes -= 4;
            subdivision.vertices.encoded_base += 4;
            value = header->gouraud_quads;
            stream->primitives = value;
            subdivision.faces = terrain_model_faces(models, header, 0);
            scene_subdiv_draw_gouraud_quad(scratch, stream, &subdivision, &projection);
            value = header->flat_quads;
            stream->primitives = value;
            subdivision.faces = terrain_model_faces(models, header, 1);
            scene_subdiv_draw_flat_quad(scratch, stream, &subdivision, &projection);
        }
    }
    return;
}

void scene_geometry_place_tile_view(SCENE_TERRAIN_VIEW *view)
{
    uint32 value, base, x, z, count, index, result;
    sint32 first, second;
    FUNCTION_MARKER(0x80061534, "ACE3.BIN");
    value = view->origin[0];
    base = (uint32)game_runtime.terrain_origin.x;
    x = value + 0x8000 + base;
    value = view->origin[2];
    base = (uint32)game_runtime.terrain_origin.z;
    z = value + 0x8000 + base;
    value = (uint32)((sint32)z >> 11) + 128;
    view->tile_base_z = value;
    count = view->vertex_count;
    value = (uint32)((sint32)x >> 11) + 128;
    view->tile_base_x = value;
    x &= 0x7FF;
    z &= 0x7FF;
    view->tile_offset_x = x - 0x400;
    result = z - 0x400;
    view->tile_offset_z = result;
    if ((sint32)count <= 0)
    {
        (void)(uint32)(result);
        return;
    }
    index = 0;
    do
    {
        first = (sint16)view->vertices[index][0];
        second = (sint16)view->vertices[index][1];
        first += (sint32)x;
        second += (sint32)z;
        if (first < -0x8000)
            first = -0x8000;
        else if (first >= 0x7800)
            first = 0x77FF;
        if (second < -0x8000)
            second = -0x8000;
        else if (second >= 0x7800)
            second = 0x77FF;
        view->vertices[index][0] = (uint16)first;
        view->vertices[index][1] = (uint16)second;
        count = view->vertex_count;
        ++index;
        result = (sint32)index < (sint32)count;
    } while (result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_terrain_collect_tiles(const uint16 *tiles, SCENE_TERRAIN_TILE *records, const SCENE_TERRAIN_VIEW *view)
{
    uint32 tile_count = 0, x_offset, z_offset, row_coordinate, height, x_base, z_base, count, row, column;
    uint32 previous, current, first, column_coordinate, table_x, table_z, value, bit, flags, depth;
    FUNCTION_MARKER(0x8005E4CC, "ACE3.BIN");
    x_offset = view->tile_offset_x;
    z_offset = view->tile_offset_z;
    row_coordinate = view->first_row;
    height = view->origin[1];
    x_base = view->tile_base_x;
    x_offset -= 0x400;
    count = view->row_count;
    z_base = view->tile_base_z;
    z_offset -= 0x400;
    previous = 0;
    current = 0;
    row = 0;
    if ((sint32)count > 0)
        do
        {
            depth = (row_coordinate << 11) - z_offset;
            first = 0xFFFFFFFF;
            count = view->column_count;
            column_coordinate = view->first_column;
            column = 0;
            if ((sint32)count > 0)
                do
                {
                    value = view->row_mask[row];
                    bit = (uint32)1 << (column & 31);
                    if (value & bit)
                    {
                        table_x = x_base + column_coordinate;
                        table_z = z_base + row_coordinate;
                        if ((sint32)(table_x << 16) < 0)
                            table_x &= 7;
                        if ((sint16)(z_base + row_coordinate) < 0)
                            table_z &= 7;
                        if ((sint16)table_x >= 256)
                            table_x = (table_x & 7) | 0xF8;
                        if ((sint16)table_z >= 256)
                            table_z = (table_z & 7) | 0xF8;
                        value = (sint16)tiles[(uint16)table_x + ((uint32)(uint16)table_z << 8)];
                        if (value & 0x1FF)
                        {
                            flags = value | (column << 16);
                            current |= bit;
                            if ((sint32)first < 0)
                            {
                                first = column;
                                flags |= 0x400000;
                            }
                            else
                                flags |= ((previous >> ((column - 1) & 31)) & 3) == 3 ? 0xE000000 : 0x8000000;
                            SCENE_TERRAIN_TILE *tile = &records[tile_count++];
                            tile->x = (uint16)((column_coordinate << 11) - x_offset);
                            tile->y = (uint16)(0 - height);
                            tile->z = (uint16)depth;
                            tile->flags = flags;
                        }
                    }
                    count = view->column_count;
                    ++column;
                    ++column_coordinate;
                } while ((sint32)column < (sint32)count);
            previous = current;
            current = 0;
            ++row_coordinate;
            count = view->row_count;
            ++row;
        } while ((sint32)row < (sint32)count);
    return tile_count;
}

uint32 scene_geometry_clip_line(uint16 *first, uint16 *second, uint32 width, uint32 height)
{
    uint32 codes[2], x, y, left, top, index, iteration, edge, a, b, c, numerator, divisor, value;
    uint16 *pointer;
    FUNCTION_MARKER(0x8005F2B4, "ACE3.BIN");
    left = 0 - width;
    top = 0 - height;
    for (index = 0; index < 2; ++index)
    {
        pointer = index ? second : first;
        x = (sint16)pointer[0];
        codes[index] = (sint32)x < (sint32)left ? 1 : (sint32)width < (sint32)x ? 2 : 0;
        y = (sint16)pointer[1];
        if ((sint32)y < (sint32)top)
            codes[index] |= 4;
        else if ((sint32)height < (sint32)y)
            codes[index] |= 8;
    }
    for (iteration = 0; iteration < 8; ++iteration)
    {
        if (codes[0] & codes[1])
            return 0;
        if ((codes[0] | codes[1]) == 0)
            return 1;
        if (codes[0] == 0)
        {
            pointer = first;
            first = second;
            second = pointer;
            codes[1] = 0;
        }
        else if (codes[0] & 3)
        {
            edge = codes[0] & 1 ? left : width;
            a = (sint16)second[1];
            b = (sint16)first[1];
            c = (sint16)first[0];
            numerator = (a - b) * (edge - c);
            a = (sint16)second[0];
            divisor = a - c;
            if (divisor == 0)
                xport_mips_break(7);
            if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                xport_mips_break(6);
            value = ((sint32)numerator / (sint32)divisor);
            a = first[1];
            first[0] = edge;
            first[1] = (a + value);
        }
        else if (codes[0] & 12)
        {
            edge = codes[0] & 4 ? top : height;
            a = (sint16)second[0];
            b = (sint16)first[0];
            c = (sint16)first[1];
            numerator = (a - b) * (edge - c);
            a = (sint16)second[1];
            divisor = a - c;
            if (divisor == 0)
                xport_mips_break(7);
            if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                xport_mips_break(6);
            value = ((sint32)numerator / (sint32)divisor);
            a = first[0];
            first[1] = edge;
            first[0] = (a + value);
        }
        x = (sint16)first[0];
        codes[0] = (sint32)x < (sint32)left ? 1 : (sint32)width < (sint32)x ? 2 : 0;
        y = (sint16)first[1];
        if ((sint32)y < (sint32)top)
            codes[0] |= 4;
        else if ((sint32)height < (sint32)y)
            codes[0] |= 8;
    }
    return 1;
}

uint32 scene_geometry_draw_terrain(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet, const SCENE_TERRAIN_VIEW *view)
{
    uint32 *gpu_word;
    DR_TPAGE *draw_mode;
    SCENE_RENDER_STREAM stream;
    SCENE_TERRAIN_TILE records[SCENE_TERRAIN_TILE_CAPACITY];
    const uint16 *tiles;
    uint32 tile_count;
    uint32 previous[32], current[32], output, tag, result;
    FUNCTION_MARKER(0x8005E6DC, "ACE3.BIN");
    TERRAIN_SCRATCH_PUBLICATION *published = (TERRAIN_SCRATCH_PUBLICATION *)getScratchAddr(0);
    uint32 *vector = published->ordinary.vector;
    tiles = assets->tile_map;
    tile_count = scene_terrain_collect_tiles(tiles, records, view);
    /* preserve the original packet cursor including the terminal record */
    stream.packet = packet + (tile_count + 1) * 12;
    vector[0] = (uint32)game_runtime.terrain_origin.x + 31744;
    vector[2] = (uint32)game_runtime.terrain_origin.z + 31744;
    stream.ordering = ordering;
    vector[1] = (uint32)game_runtime.mission_matrices.view.t[1];
    game_rotate_vector_current_gte(vector);
    scene_terrain_draw_tiles(records, tile_count, &assets->terrain_models, vector, previous, current, &stream);
    output = stream.packet;
    draw_mode = mission_packet_span(output, sizeof(*draw_mode));
    draw_mode->tag = (draw_mode->tag & 0xFFFFFFu) | 0x01000000u;
    output = stream.packet;
    draw_mode->code[0] = 0xE1000228;
    gpu_word = mission_ot_span(ordering + 16376, sizeof(*gpu_word));
    tag = *gpu_word;
    output = stream.packet;
    draw_mode->tag = tag | 0x01000000;
    output = stream.packet;
    *gpu_word = output & 0xFFFFFF;
    output = stream.packet;
    result = output + 8;
    stream.packet = result;
    return result;
}

static RESOURCE_ASSET_SOURCE scene_mesh_catalog_vertex_input(const GAME_SCENE_ASSETS *assets, uint32 selection, const SCENE_MESH_MODEL_HEADER *header, uint32 address, RESOURCE_ASSET_SOURCE *parent)
{
    RESOURCE_ASSET_SOURCE missing = {0};
    RESOURCE_ASSET_SOURCE source;
    *parent = missing;

    if (!assets->model_header_sources || !assets->model_headers || selection >= assets->model_header_count || assets->model_headers[selection] != header)
        return missing;
    source = assets->model_header_sources[selection];
    if (source.data)
        *parent = source;
    return scene_mesh_vertex_input(source, address);
}

void scene_geometry_prepare_aircraft(const GAME_SCENE_ASSETS *assets, const SCENE_GEOMETRY_DESCRIPTOR *native_entries, SCENE_GEOMETRY_MESH *native_meshes, const SCENE_GEOMETRY_COMPOSITE_BUFFER *native_animated, const SCENE_GEOMETRY_COMPOSITE_BUFFER *native_fixed, SCENE_GEOMETRY_SPRITE *native_sprites)
{
    FLIGHT_CONTROL *smooth;
    MATRIX matrix;
    uint16 angles[3];
    SCENE_GEOMETRY_COMPOSITE *output, *animated_record = native_animated->records, *fixed_record = native_fixed->records;
    uint32 animated_index = 0, fixed_index = 0;
    SCENE_GEOMETRY_ROTATION *rotation_output;
    uint32 focal, range, half_width, half_height, x, y, z, distance, selection, level, vertices;
    uint32 a, b, c, d, value, radius, depth, screen, screen_x, screen_y, margin, index, column, kind;
    FUNCTION_MARKER(0x80107518, "ACE3.BIN");
    focal = ((sint16)game_runtime.camera_proj.distance);
    range = focal * 12;
    a = game_runtime.camera_proj.size[0];
    b = game_runtime.camera_proj.size[1];
    half_width = ((sint32)(a << 16) >> 17);
    half_height = ((sint32)(b << 16) >> 17);
    while (native_entries->model_index != 255)
    {
        a = native_entries->native_pose->position[0];
        b = game_runtime.camera_view.pose.position[0];
        c = game_runtime.camera_view.pose.position[1];
        x = ((sint32)(a - b) >> 10);
        a = native_entries->native_pose->position[1];
        b = native_entries->native_pose->position[2];
        y = ((sint32)(a - c) >> 10);
        c = game_runtime.camera_view.pose.position[2];
        z = ((sint32)(b - c) >> 10);
        a = (sint32)x < 0 ? 0 - x : x;
        b = (sint32)y < 0 ? 0 - y : y;
        c = (sint32)z < 0 ? 0 - z : z;
        distance = a + b + c;
        if ((sint32)distance > 0x2C000 || (native_entries->kind & 15) == 0)
            goto next_aircraft_geometry;
        selection = (uint32)native_entries->model_index * 4;
        smooth = &flight_control;
        if ((sint32)(range << 2) < (sint32)distance)
            selection += 2;
        else if ((sint32)(range << 1) < (sint32)distance)
            ++selection;
        if (!assets->model_headers || selection >= assets->model_header_count || !assets->model_headers[selection])
            abort();
        const SCENE_MESH_MODEL_HEADER *mesh_model = assets->model_headers[selection];
        if ((sint32)range < (sint32)distance)
        {
            level = 0;
            vertices = mesh_model->vertices[0];
        }
        else if (((sint32)range >> 1) < (sint32)distance)
        {
            level = 1;
            vertices = mesh_model->vertices[1];
        }
        else
        {
            level = 2;
            vertices = mesh_model->vertices[2];
        }
        x = ((sint32)x >> (2 - level));
        y = ((sint32)y >> (2 - level));
        z = ((sint32)z >> (2 - level));
        SetRotMatrix(&game_runtime.mission_matrices.view);
        xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(1, z);
        xport_gte_execute(0x180001);
        kind = native_entries->kind;
        if (kind & 9)
        {
            native_meshes->flags = native_entries->flags;
            value = native_entries->parameter;
            native_meshes->parameter = (uint16)value;
            native_meshes->level = level;
            native_meshes->model_data = mesh_model;
            native_meshes->vertices_address = vertices;
            native_meshes->vertex_source = scene_mesh_catalog_vertex_input(assets, selection, mesh_model, vertices, &native_meshes->model_source);
            native_meshes->callback = 0;
            native_meshes->lod_model_headers = assets->model_headers && selection < assets->model_header_count ? assets->model_headers + selection : NULL;
            native_meshes->lod_model_sources = assets->model_header_sources && selection < assets->model_header_count ? assets->model_header_sources + selection : NULL;
            native_meshes->lod_header_count = native_meshes->lod_model_headers ? assets->model_header_count - selection : 0;
            a = xport_gte_read_data(25);
            native_meshes->translation.word[0] = a;
            b = xport_gte_read_data(26);
            native_meshes->translation.word[1] = b;
            c = xport_gte_read_data(27);
            native_meshes->translation.word[2] = c;
        }
        else
        {
            fixed_record = scene_geometry_composite_record(native_fixed, fixed_index);
            fixed_record->effect = 0;
            if (kind & 2)
            {
                animated_record = output = scene_geometry_composite_record(native_animated, animated_index);
                output->effect = smooth;
                output->callback = 0;
            }
            else
                output = fixed_record;
            output->model_source = assets->aircraft_model_source;
            const SCENE_MESH_MODEL_HEADER *const *composite_headers = scene_aircraft_composite_headers(assets);
            for (index = 0; index < 2; ++index)
            {
                const SCENE_MESH_MODEL_HEADER *component_model = composite_headers[index];
                value = scene_mesh_model_vertices(component_model, level);
                output->vertices_address[index] = value;
                output->models_data[index] = component_model;
            }
            output->models_data[2] = composite_headers[2];
            output->models_data[3] = composite_headers[7];
            output->models_data[4] = composite_headers[9];
            if (kind & 2)
            {
                for (index = 0; index < 10; ++index)
                {
                    const SCENE_MESH_MODEL_HEADER *component_model = index == 0 ? output->models_data[2] : index == 5 ? output->models_data[3] : index == 7 ? output->models_data[4] : composite_headers[2 + index];
                    value = scene_mesh_model_vertices(component_model, level);
                    output->blend_vertices_address[index] = value;
                    if (index == 0)
                        output->vertices_address[2] = value;
                    if (index == 5)
                        output->vertices_address[3] = value;
                    if (index == 7)
                        output->vertices_address[4] = value;
                }
            }
            else
            {
                a = scene_mesh_model_vertices(output->models_data[2], level);
                b = scene_mesh_model_vertices(output->models_data[3], level);
                c = scene_mesh_model_vertices(output->models_data[4], level);
                output->vertices_address[4] = c;
                output->vertices_address[2] = a;
                output->vertices_address[3] = b;
            }
            for (uint32 mesh_index = 0; mesh_index < 5; ++mesh_index)
                output->vertex_sources[mesh_index] = scene_mesh_vertex_input(output->model_source, output->vertices_address[mesh_index]);
            output->level = level;
            a = xport_gte_read_data(25);
            output->translation.word[0] = a;
            b = xport_gte_read_data(26);
            output->translation.word[1] = b;
            c = xport_gte_read_data(27);
            output->translation.word[2] = c;
        }
        depth = ((sint32)xport_gte_read_data(27) >> level);
        value = (uint32)mesh_model->radius;
        radius = ((sint32)value >> (2 - level));
        if ((sint32)depth < (sint32)(0 - radius))
            goto next_aircraft_geometry;
        screen = xport_gte_read_data(14);
        kind = native_entries->kind;
        if (kind & 2)
            animated_record->depth = depth;
        else if (kind & 4)
            fixed_record->depth = depth;
        if ((sint32)depth <= 0)
            depth = 1;
        margin = ((sint32)(radius * focal) / (sint32)depth);
        screen_x = (sint16)screen;
        screen_y = ((sint32)screen >> 16);
        if ((sint32)(radius << 4) < (sint32)margin)
            margin = radius << 2;
        value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
        if ((sint32)(half_width + margin) < (sint32)value)
            goto next_aircraft_geometry;
        value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
        if ((sint32)(half_height + margin) < (sint32)value)
            goto next_aircraft_geometry;
        rotation_build(&matrix, native_entries->native_pose->angles);

        kind = native_entries->kind;
        if (kind & 9)
            rotation_output = &native_meshes->rotation;
        else if (kind & 2)
        {
            if (native_entries->selected)
            {
                a = ((sint16)game_runtime.camera_follow.steer[0]);
                b = ((sint16)game_runtime.camera_follow.steer[1]);
                value = game_runtime.camera_follow.steer[0];
                if ((sint32)a < 0)
                    a = 0 - a;
                if ((sint32)b < 0)
                    b = 0 - b;
                c = (a + b) << 2;
                if ((sint32)c >= 4097)
                    c = 4096;
                c = 4096 - c;
                a = smooth->axis[1];
                a = ((sint32)(a << 16) >> 20);
                a = ((sint32)(a * c) / 4096);
                b = smooth->axis[2];
                b = ((sint32)(b << 16) >> 21);
                b = ((sint32)(b * c) / 4096);
                d = smooth->axis[0];
                d = ((sint32)(d << 16) >> 19);
                d = ((sint32)(d * c) / 4096);
                angles[0] = value - a;
                value = game_runtime.camera_follow.steer[1];
                angles[1] = value - b;
                value = game_runtime.camera_follow.steer[2];
                angles[2] = value - d;
                {
                    MATRIX rotation;
                    game_build_rotation_via_gte(&rotation, angles);
                    memcpy(animated_record->rotation.component, rotation.m, sizeof(rotation.m));
                }
                value = ((sint16)game_runtime.camera_shots.offset[0]);
                animated_record->translation.word[0] = (sint32)(0 - value) >> (6 - level);
                value = ((sint16)game_runtime.camera_shots.offset[1]);
                animated_record->translation.word[1] = (sint32)(0 - value) >> (6 - level);
                value = ((sint16)game_runtime.camera_shots.offset[2]);
                animated_record->translation.word[2] = (sint32)(0 - value) >> (6 - level);
                goto aircraft_light_columns;
            }
            rotation_output = &animated_record->rotation;
        }
        else
            rotation_output = &fixed_record->rotation;
        for (column = 0; column < 3; ++column)
        {
            a = ((uint16)matrix.m[0][column]);
            b = ((uint16)matrix.m[1][column]);
            c = ((uint16)matrix.m[2][column]);
            xport_gte_write_data(9, a);
            xport_gte_write_data(10, b);
            xport_gte_write_data(11, c);
            xport_gte_execute(0x49E012);
            a = xport_gte_read_data(9);
            b = xport_gte_read_data(10);
            c = xport_gte_read_data(11);
            rotation_output->component[column] = (uint16)a;
            rotation_output->component[column + 3] = (uint16)b;
            rotation_output->component[column + 6] = (uint16)c;
        }
        if (kind & 9)
        {
            ++native_meshes;
            goto next_aircraft_geometry;
        }
    aircraft_light_columns:
        SetRotMatrix(&game_runtime.mission_lighting.directions);
        rotation_output = (kind & 2) ? &animated_record->light_rotation : &fixed_record->light_rotation;
        for (column = 0; column < 3; ++column)
        {
            a = ((uint16)matrix.m[0][column]);
            b = ((uint16)matrix.m[1][column]);
            c = ((uint16)matrix.m[2][column]);
            xport_gte_write_data(9, a);
            xport_gte_write_data(10, b);
            xport_gte_write_data(11, c);
            xport_gte_execute(0x49E012);
            a = xport_gte_read_data(9);
            b = xport_gte_read_data(10);
            c = xport_gte_read_data(11);
            rotation_output->component[column] = (uint16)a;
            rotation_output->component[column + 3] = (uint16)b;
            rotation_output->component[column + 6] = (uint16)c;
        }
        if (kind & 2)
        {
            ++animated_index;
            animated_record = native_animated->records + animated_index;
        }
        else
        {
            ++fixed_index;
            fixed_record = native_fixed->records + fixed_index;
        }
    next_aircraft_geometry:
        ++native_entries;
    }
    native_meshes->level = 0xFFFFFFFF;
    scene_geometry_end_composites(native_animated, animated_index);
    scene_geometry_end_composites(native_fixed, fixed_index);
    native_sprites->model_data = NULL;
}

uint32 scene_geometry_draw_aircraft(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet)
{
    SCENE_RENDER_STREAM stream;
    SCENE_GEOMETRY_MISSION_WORKSPACE geometry;
    SCENE_GEOMETRY_COMPOSITE_BUFFER animated = {geometry.animated, SCENE_GEOMETRY_ANIMATED_CAPACITY};
    SCENE_GEOMETRY_COMPOSITE_BUFFER fixed = {geometry.fixed, SCENE_GEOMETRY_FIXED_CAPACITY};
    SCENE_GEOMETRY_MESH *meshes = geometry.meshes;
    SCENE_GEOMETRY_SPRITE *sprites = geometry.sprites;
    uint32 value;
    FUNCTION_MARKER(0x8010817C, "ACE3.BIN");
    scene_geometry_collect_mission_primary(geometry.descriptors);
    scene_geometry_prepare_aircraft(assets, geometry.descriptors, meshes, &animated, &fixed, sprites);
    stream.ordering = ordering;
    stream.packet = packet;
    scene_geometry_interpolate_sprites(sprites, &stream);
    scene_geometry_interpolate_meshes(meshes, &stream);
    scene_geometry_interpolate_composites(&animated, &stream);
    SetRotMatrix(&game_runtime.mission_matrices.view);
    scene_geometry_draw_sprites(sprites, &stream);
    scene_geometry_draw_meshes(meshes, &stream);
    value = (uint16)game_runtime.mission_control.ticks;
    scene_geometry_draw_composites(&animated, (uint16)value, &stream);
    scene_geometry_draw_composites(&fixed, (uint16)value, &stream);
    return stream.packet;
}

static void scene_terrain_split_quad(TERRAIN_CORNERS *corners, TERRAIN_VERTEX *points, uint32 frame, uint32 *cursor, uint32 level)
{
    TERRAIN_VERTEX *midpoints = points + frame;
    const uint8 *parent = corners[0].point;
    uint8 *child = corners[1].point;
    uint32 a, b, current, index, flags, common, combined, maximum, minimum, depth, corner, vertex, sum, first_area, second_area, area, span, product, packet, next, color, screen;
    FUNCTION_MARKER(0x80096630, "ACE3.BIN");
    a = parent[0];
    b = parent[1];
    scene_terrain_average_vertex(midpoints + 0, points + a, points + b);
    xport_gte_write_data(0, midpoints[0].xy);
    xport_gte_write_data(1, midpoints[0].z);
    a = parent[0];
    b = parent[2];
    scene_terrain_average_vertex(midpoints + 1, points + a, points + b);
    xport_gte_execute(0x180001);
    a = parent[1];
    b = parent[3];
    scene_terrain_average_vertex(midpoints + 2, points + a, points + b);
    a = parent[2];
    b = parent[3];
    scene_terrain_average_vertex(midpoints + 3, points + a, points + b);
    a = parent[1];
    b = parent[2];
    scene_terrain_average_vertex(midpoints + 4, points + a, points + b);
    current = frame;
    index = 0;
    do
    {
        points[current].screen = xport_gte_read_data(14);
        depth = xport_gte_read_data(19);
        points[current].depth = (uint16)(depth);
        flags = xport_gte_read_control(31);
        xport_gte_write_data(0, points[current + 1].xy);
        xport_gte_write_data(1, points[current + 1].z);
        xport_gte_execute(0x180001);
        if ((flags & 0x7E01E000) != 0)
            points[current].flags = (uint16)(0xFFFF);
        else
        {
            a = game_screen_clip(points[current].screen);
            points[current].flags = (uint16)(a);
        }
        index += 1;
        current += 1;
    } while (index < 4);
    points[current].screen = xport_gte_read_data(14);
    depth = xport_gte_read_data(19);
    points[current].depth = (uint16)(depth);
    flags = xport_gte_read_control(31);
    a = 0xFFFFFFFF;
    if ((flags & 0x7E01E000) == 0)
        a = game_screen_clip(points[current].screen);
    points[current].flags = (uint16)(a);
    corners[1].written = 1;
    level -= 1;
    corner = 0;
    do
    {
        common = 0xFFFFFFFF;
        combined = 0;
        maximum = 0;
        minimum = 0x7FFF;
        index = 0;
        do
        {
            sum = index + corner;
            if (corner == index)
            {
                a = parent[corner];
                child[corner] = a;
            }
            else
            {
                if (sum == 3)
                    vertex = frame + 4;
                else if (sum < 3)
                    vertex = frame + sum - 1;
                else
                    vertex = frame + sum - 2;
                child[index] = vertex;
            }
            vertex = child[index];
            flags = (sint16)points[vertex].flags;
            depth = (sint16)points[vertex].depth;
            common &= flags;
            combined |= flags;
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            else if ((sint32)depth < (sint32)minimum)
                minimum = depth;
            index += 1;
        } while (index < 4);
        if (maximum - 121 >= 0x777F || common != 0)
            goto next_corner;
        if ((sint32)combined >= 0)
        {
            vertex = child[3];
            xport_gte_write_data(12, points[vertex].screen);
            vertex = child[1];
            xport_gte_write_data(13, points[vertex].screen);
            vertex = child[2];
            xport_gte_write_data(14, points[vertex].screen);
            span = maximum - minimum;
            xport_gte_execute(0x1400006);
            first_area = xport_gte_read_data(24);
            if ((sint32)first_area >= 0)
                goto next_corner;
            vertex = child[0];
            xport_gte_write_data(12, points[vertex].screen);
            first_area = 0 - first_area;
            xport_gte_execute(0x1400006);
            second_area = xport_gte_read_data(24);
            area = first_area + second_area;
            if ((sint32)second_area <= 0)
                goto next_corner;
            if ((sint32)area >= 1025)
            {
                product = area * span;
                if (((sint32)product >> 16) >= 513)
                    combined = 0xFFFFFFFF;
                else if ((sint32)combined > 0)
                {
                    a = child[0];
                    b = child[1];
                    current = child[2];
                    vertex = child[3];
                    if (scene_terrain_quad_exceeds_span(points[a].screen, points[b].screen, points[current].screen, points[vertex].screen) != 0)
                        combined = 0xFFFFFFFF;
                }
            }
        }
        if ((sint32)combined < 0 && (sint32)level >= 0)
        {
            scene_terrain_split_quad(corners + 1, points, frame + TERRAIN_FRAME_POINTS, cursor, level);
            goto next_corner;
        }
        packet = *cursor;
        POLY_GT4 *quad = mission_packet_span(packet, sizeof(*quad));
        next = packet + 52;
        quad->tag = (next & 0xFFFFFF) | 0x0C000000;
        *cursor = next;
        for (index = 0; index < 4; index += 1)
        {
            vertex = child[index];
            color = points[vertex].color;
            screen = points[vertex].screen;
            quad->p[index].rgbc.rgbc = color;
            quad->p[index].xy.xy = screen;
        }
        quad->p[0].attr = (uint16)(TERRAIN_CLUT);
        quad->p[1].attr = (uint16)(TERRAIN_TPAGE);
        a = child[0];
        b = child[1];
        color = points[a].uv;
        screen = points[b].uv;
        quad->p[0].uv.uv = (uint16)(color);
        quad->p[1].uv.uv = (uint16)(screen);
        a = child[2];
        b = child[3];
        color = points[a].uv;
        screen = points[b].uv;
        quad->p[2].uv.uv = (uint16)(color);
        quad->p[3].uv.uv = (uint16)(screen);
    next_corner:
        corner += 1;
    } while (corner < 4);
    return;
}

uint32 scene_geometry_draw_terrain_mode_two(const SCENE_TERRAIN_VIEW *view, uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    TERRAIN_SCAN_WORK scan;
    TERRAIN_EDGE_WORK edge;
    TERRAIN_STRIP_POINT strip[2];
    TERRAIN_CORNERS corners[TERRAIN_CORNER_ROWS] = {0};
    uint32 cursor = packet;
    uint8 strip_projected = 0;
    uint32 a, b, c, index, row, column, run, value, x, y, z, point, color, flags, depth, maximum, minimum, combined, common, first_area, second_area, area, product, previous, current, output, next, screen, result;
    FUNCTION_MARKER(0x80096A48, "ACE3.BIN");
    TERRAIN_SCRATCH_PUBLICATION *published = (TERRAIN_SCRATCH_PUBLICATION *)getScratchAddr(0);
    TERRAIN_VERTEX *points = published->mode_two.points;
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    c = (uint32)(uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    a = (uint32)game_runtime.mission_matrices.view.t[0];
    b = (uint32)game_runtime.mission_matrices.view.t[1];
    xport_gte_write_control(5, a);
    c = (uint32)game_runtime.mission_matrices.view.t[2];
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    a = view->first_column;
    b = view->tile_base_x;
    c = view->first_row;
    a += b;
    b = view->tile_base_z;
    scan.first_column = (uint16)(a);
    scan.first_row = (uint16)(c + b);
    a = view->column_count;
    b = view->row_count;
    scan.column_limit = a;
    scan.row_limit = b;
    x = view->origin[0];
    y = view->origin[1];
    z = view->origin[2];
    scan.palette[15] = 0x3CA6C8C5;
    scan.origin[1] = y;
    scan.origin[0] = x;
    scan.origin[2] = z;
    edge.height[0] = (uint16)(0 - (uint32)3072 - y);
    edge.height[1] = (uint16)(0 - (uint32)2816 - y);
    edge.height[2] = (uint16)(0 - (uint32)2048 - y);
    xport_gte_write_control(21, 0xC50);
    xport_gte_write_control(22, 0xC80);
    xport_gte_write_control(23, 0xA60);
    xport_gte_write_data(6, 0x3C729081);
    index = 0;
    value = 0;
    do
    {
        xport_gte_write_data(8, value);
        xport_gte_execute(0x780010);
        value += 273;
        scan.palette[index] = xport_gte_read_data(22);
        index += 1;
    } while (index < 15);
    xport_gte_write_control(21, 0x1D0);
    xport_gte_write_control(22, 0x390);
    xport_gte_write_control(23, 0x210);
    row = (sint16)scan.first_row;
    value = scan.row_limit;
    scan.row_index = 0;
    scan.row_bits_written = 0;
    edge.row_mode_written = 0;
    edge.column_mode_written = 0;
    edge.masks_written = 0;
    if ((sint32)value <= 0)
        goto terrain_end;
    do
    {
        if (row - 87 < 74)
            edge.row_mode = 0;
        else
        {
            if (row - 85 >= 78)
                goto next_row;
            if (row == 85)
                value = 0x10002;
            else if (row == 86)
                value = 1;
            else if (row == 161)
                value = 0x10000;
            else
                value = 0x20001;
            edge.row_mode = value;
        }
        edge.row_mode_written = 1;
        value = (row << 11) - (uint16)scan.origin[2];
        strip[0].z = (uint16)(value);
        if (((sint32)(value << 16) >> 16) >= 30720)
            goto next_row;
        strip[1].z = (uint16)(value + 2048);
        a = (sint16)edge.row_mode;
        b = edge.height[a];
        a = (sint16)(edge.row_mode >> 16);
        strip[0].y = (uint16)(b);
        b = edge.height[a];
        strip[1].y = (uint16)(b);
        index = 0;
        do
        {
            if ((sint16)(edge.row_mode >> (index * 16)) == 2)
            {
                a = 0xFFFFFFFF;
                b = 0xFFFFFFFF;
            }
            else
            {
                const uint32 *mask = terrain_row_masks[(row + index - 6) & 15];
                a = mask[0];
                b = mask[1];
            }
            edge.masks[index][0] = a;
            edge.masks[index][1] = b;
            index += 1;
        } while (index < 2);
        edge.masks_written = 1;
        index = scan.row_index;
        value = view->row_mask[index];
        column = (sint16)scan.first_column;
        run = 0;
        scan.column_index = 0;
        scan.row_bits_written = 1;
        scan.row_bits = value;
        if (value == 0)
            goto next_row;
        do
        {
            a = scan.column_index;
            b = scan.column_limit;
            if ((sint32)a >= (sint32)b)
                goto next_row;
            value = scan.row_bits;
            if ((value & 1) == 0)
                goto reset_run;
            if (column - 63 < 74)
                edge.column_mode = 0;
            else
            {
                if (column - 61 >= 78 || edge.row_mode != 0)
                    goto reset_run;
                if (column == 61)
                    value = 0x10002;
                else if (column == 62)
                    value = 1;
                else if (column == 137)
                    value = 0x10000;
                else
                    value = 0x20001;
                edge.column_mode = value;
            }
            edge.column_mode_written = 1;
            value = (column << 11) - (uint16)scan.origin[0];
            strip[0].x = (uint16)(value);
            if (((sint32)(value << 16) >> 16) >= 30720)
                goto reset_run;
            strip[1].x = (uint16)value;
            strip_projected = 1;
            if (run == 0)
            {
                if (edge.column_mode != 0)
                {
                    a = (sint16)edge.column_mode;
                    value = edge.height[a];
                    strip[1].y = (uint16)(value);
                    strip[0].y = (uint16)(value);
                }
                else if (edge.row_mode == 0)
                {
                    value = edge.height[0];
                    strip[1].y = (uint16)(value);
                    strip[0].y = (uint16)(value);
                }
                x = (uint32)strip[0].x | ((uint32)strip[0].y << 16);
                z = strip[0].z;
                xport_gte_write_data(0, x);
                xport_gte_write_data(1, z);
                point = 0;
                points[point].z = (uint16)(z);
                xport_gte_execute(0x180001);
                points[point].xy = x;
                if ((sint16)edge.column_mode == 2)
                    color = scan.palette[15];
                else
                {
                    value = column - 4;
                    a = edge.masks[0][(value >> 3) & 1];
                    color = scan.palette[(a >> ((value & 7) * 4)) & 15];
                }
                x = (uint32)strip[1].x | ((uint32)strip[1].y << 16);
                points[point + 2].xy = x;
                flags = xport_gte_read_control(31);
                points[point].screen = xport_gte_read_data(14);
                maximum = xport_gte_read_data(19);
                xport_gte_write_data(6, color);
                if ((flags & 0x7E01E000) != 0)
                {
                    points[point].flags = (uint16)(0xFFFF);
                    points[point].color = color;
                }
                else
                {
                    xport_gte_execute(0x780010);
                    value = game_screen_clip(points[point].screen);
                    points[point].flags = (uint16)(value);
                    points[point].color = xport_gte_read_data(22);
                }
                point += 2;
                z = strip[1].z;
                xport_gte_write_data(0, x);
                xport_gte_write_data(1, z);
                minimum = maximum;
                points[point].z = (uint16)(z);
                xport_gte_execute(0x180001);
                points[point - 2].depth = (uint16)(minimum);
                if ((sint16)edge.column_mode == 2)
                    color = scan.palette[15];
                else
                {
                    value = column - 4;
                    a = edge.masks[1][(value >> 3) & 1];
                    color = scan.palette[(a >> ((value & 7) * 4)) & 15];
                }
                flags = xport_gte_read_control(31);
                depth = xport_gte_read_data(19);
                points[point].screen = xport_gte_read_data(14);
                points[point].depth = (uint16)(depth);
                if ((sint32)maximum < (sint32)depth)
                    maximum = depth;
                else
                    minimum = depth;
                xport_gte_write_data(6, color);
                if ((flags & 0x7E01E000) != 0)
                {
                    points[point].flags = (uint16)(0xFFFF);
                    points[point].color = color;
                }
                else
                {
                    xport_gte_execute(0x780010);
                    value = game_screen_clip(points[point].screen);
                    points[point].flags = (uint16)(value);
                    points[point].color = xport_gte_read_data(22);
                }
            }
            else
            {
                point = (run & 1);
                a = (sint16)points[point].depth;
                b = (sint16)points[point + 2].depth;
                if ((sint32)a < (sint32)b)
                {
                    maximum = b;
                    minimum = a;
                }
                else
                {
                    maximum = a;
                    minimum = b;
                }
            }
            if (edge.column_mode != 0)
            {
                a = (sint16)(edge.column_mode >> 16);
                value = edge.height[a];
                strip[1].y = (uint16)(value);
                strip[0].y = (uint16)(value);
            }
            value = strip[0].x + 2048;
            z = strip[0].z;
            strip[0].x = (uint16)(value);
            x = (uint32)strip[0].x | ((uint32)strip[0].y << 16);
            strip[1].x = (uint16)(value);
            xport_gte_write_data(0, x);
            xport_gte_write_data(1, z);
            point = ((run + 1) & 1);
            xport_gte_execute(0x180001);
            points[point].xy = x;
            points[point].z = (uint16)(z);
            if ((sint16)(edge.column_mode >> 16) == 2)
                color = scan.palette[15];
            else
            {
                value = column - 3;
                a = edge.masks[0][(value >> 3) & 1];
                color = scan.palette[(a >> ((value & 7) * 4)) & 15];
            }
            flags = xport_gte_read_control(31);
            depth = xport_gte_read_data(19);
            points[point].screen = xport_gte_read_data(14);
            points[point].depth = (uint16)(depth);
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            else if ((sint32)depth < (sint32)minimum)
                minimum = depth;
            xport_gte_write_data(6, color);
            if ((flags & 0x7E01E000) != 0)
            {
                points[point].flags = (uint16)(0xFFFF);
                points[point].color = color;
            }
            else
            {
                xport_gte_execute(0x780010);
                value = game_screen_clip(points[point].screen);
                points[point].flags = (uint16)(value);
                points[point].color = xport_gte_read_data(22);
            }
            x = (uint32)strip[1].x | ((uint32)strip[1].y << 16);
            z = strip[1].z;
            xport_gte_write_data(0, x);
            xport_gte_write_data(1, z);
            point += 2;
            points[point].xy = x;
            xport_gte_execute(0x180001);
            points[point].z = (uint16)(z);
            if ((sint16)(edge.column_mode >> 16) == 2)
                color = scan.palette[15];
            else
            {
                value = column - 3;
                a = edge.masks[1][(value >> 3) & 1];
                color = scan.palette[(a >> ((value & 7) * 4)) & 15];
            }
            run += 1;
            flags = xport_gte_read_control(31);
            depth = xport_gte_read_data(19);
            points[point].screen = xport_gte_read_data(14);
            points[point].depth = (uint16)(depth);
            if ((sint32)maximum < (sint32)depth)
                maximum = depth;
            xport_gte_write_data(6, color);
            if ((flags & 0x7E01E000) != 0)
            {
                points[point].flags = (uint16)(0xFFFF);
                points[point].color = color;
            }
            else
            {
                xport_gte_execute(0x780010);
                value = game_screen_clip(points[point].screen);
                points[point].flags = (uint16)(value);
                points[point].color = xport_gte_read_data(22);
            }
            if (maximum - 121 >= 0x777F)
                goto next_column;
            a = points[0].flags;
            b = points[1].flags;
            c = points[2].flags;
            value = points[3].flags;
            common = a & b & c & value;
            if (common != 0)
                goto next_column;
            combined = a | b | c | value;
            combined = (sint32)(combined << 16) >> 16;
            point -= 2;
            if ((sint32)combined >= 0)
            {
                xport_gte_write_data(15, points[point].screen);
                point = ((run + 1) & 1);
                xport_gte_write_data(15, points[point + 2].screen);
                maximum -= combined;
                xport_gte_execute(0x1400006);
                first_area = xport_gte_read_data(24);
                if ((sint32)first_area >= 0)
                    goto next_column;
                xport_gte_write_data(12, points[point].screen);
                first_area = 0 - first_area;
                xport_gte_execute(0x1400006);
                second_area = xport_gte_read_data(24);
                area = first_area + second_area;
                if ((sint32)second_area <= 0)
                    goto next_column;
                if ((sint32)area >= 1025)
                {
                    product = area * maximum;
                    if (((sint32)product >> 16) >= 513)
                        combined = 0xFFFFFFFF;
                    else if ((sint32)combined > 0 && scene_terrain_quad_exceeds_span(points[0].screen, points[1].screen, points[2].screen, points[3].screen) != 0)
                        combined = 0xFFFFFFFF;
                }
                if ((sint32)combined >= 0)
                {
                    point = ((run + 1) & 1);
                    output = cursor;
                    POLY_GT4 *quad = mission_packet_span(output, sizeof(*quad));
                    next = output + 52;
                    quad->tag = (next & 0xFFFFFF) | 0x0C000000;
                    quad->p[2].uv.uv = (uint16)(0xBF00);
                    quad->p[0].uv_attr = 0x4ED68000;
                    quad->p[1].uv_attr = 0x0039803F;
                    quad->p[3].uv.uv = (uint16)(0xBF3F);
                    color = points[point].color;
                    screen = points[point].screen;
                    quad->p[0].rgbc.rgbc = color;
                    quad->p[0].xy.xy = screen;
                    color = points[point + 2].color;
                    screen = points[point + 2].screen;
                    point = (run & 1);
                    quad->p[2].rgbc.rgbc = color;
                    quad->p[2].xy.xy = screen;
                    color = points[point].color;
                    screen = points[point].screen;
                    quad->p[1].rgbc.rgbc = color;
                    quad->p[1].xy.xy = screen;
                    color = points[point + 2].color;
                    screen = points[point + 2].screen;
                    quad->p[3].rgbc.rgbc = color;
                    quad->p[3].xy.xy = screen;
                    cursor = next;
                    goto next_column;
                }
            }
            current = ((run + 1) & 1);
            previous = (run & 1);
            corners[0].point[0] = (uint8)(current);
            corners[0].point[3] = (uint8)(previous + 2);
            corners[0].point[2] = (uint8)(current + 2);
            corners[0].point[1] = (uint8)(previous);
            points[current].uv = 0x8000;
            points[previous].uv = 0x803F;
            points[current + 2].uv = 0xBF00;
            points[previous + 2].uv = 0xBF3F;
            corners[0].written = 1;
            scene_terrain_split_quad(corners, points, TERRAIN_STRIP_COUNT, &cursor, TERRAIN_SPLIT_LEVEL);
            goto next_column;
        reset_run:
            run = 0;
        next_column:
            column += 1;
            a = scan.column_index;
            b = scan.row_bits;
            a += 1;
            b >>= 1;
            scan.column_index = a;
            scan.row_bits = b;
        } while (b != 0);
    next_row:
        a = scan.row_index;
        b = scan.row_limit;
        a += 1;
        scan.row_index = a;
        row += 1;
    } while ((sint32)a < (sint32)b);
terrain_end:
    /* publish the original exit lanes for the remaining scratchpad renderers */
    for (index = 0; index < 3; ++index)
        published->mode_two.origin[index] = scan.origin[index];
    if (scan.row_bits_written)
    {
        published->mode_two.row_bits = scan.row_bits;
        published->mode_two.column_index = scan.column_index;
    }
    published->mode_two.column_limit = scan.column_limit;
    published->mode_two.row_limit = scan.row_limit;
    published->mode_two.row_index = scan.row_index;
    published->mode_two.first_cell = scan.first_column | ((uint32)scan.first_row << 16);
    for (index = 0; index < 16; ++index)
        published->mode_two.palette[index] = scan.palette[index];
    if (edge.masks_written)
    {
        for (index = 0; index < 2; ++index)
        {
            published->mode_two.masks[index][0] = edge.masks[index][0];
            published->mode_two.masks[index][1] = edge.masks[index][1];
        }
    }
    if (edge.row_mode_written)
        published->mode_two.row_mode = edge.row_mode;
    if (edge.column_mode_written)
        published->mode_two.column_mode = edge.column_mode;
    published->mode_two.height[0] = edge.height[0];
    published->mode_two.height[1] = edge.height[1];
    published->mode_two.height[2] = edge.height[2];
    if (edge.column_mode_written)
        published->mode_two.strip[0].x = strip[0].x;
    if (edge.row_mode_written)
        published->mode_two.strip[0].z = strip[0].z;
    if (strip_projected)
        published->mode_two.strip[1].x = strip[1].x;
    if (edge.masks_written)
    {
        published->mode_two.strip[0].y = strip[0].y;
        published->mode_two.strip[1].y = strip[1].y;
        published->mode_two.strip[1].z = strip[1].z;
    }
    /* preserve untouched corner rows and encode only the lists that were written */
    for (row = 0; row < TERRAIN_CORNER_ROWS; ++row)
    {
        if (!corners[row].written)
            break;
        for (index = 0; index < 4; ++index)
            published->mode_two.corner_addresses[row][index] = TERRAIN_POINT_BASE + (uint32)corners[row].point[index] * TERRAIN_VERTEX_BYTES;
    }
    published->mode_two.cursor = cursor;
    published->mode_two.clut = TERRAIN_CLUT;
    published->mode_two.tpage = TERRAIN_TPAGE;
    result = cursor;
    if (packet != result)
    {
        ordering_word = mission_ot_span(ordering, sizeof(*ordering_word));
        value = *ordering_word;
        packet_link = mission_packet_span(result - 52, sizeof(*packet_link));
        packet_link->addr = value;
        ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
        packet = cursor;
    }
    return packet;
}

void scene_geometry_render_mission_terrain(void)
{
    SCENE_TERRAIN_VIEW geometry;
    uint16 angles[3];
    uint32 position[3], value, a, b, c, ordering, packet, count, index, kind, result;
    const PSX_RECT *rectangle;
    FUNCTION_MARKER(0x8003346C, "ACE3.BIN");
    value = game_runtime.camera_view.pose.angles[0];
    angles[0] = ((sint32)(value << 16) >> 18);
    value = game_runtime.camera_view.pose.angles[1];
    angles[1] = ((sint32)(value << 16) >> 18);
    value = game_runtime.camera_view.pose.angles[2];
    angles[2] = ((sint32)(value << 16) >> 18);
    if ((game_runtime.mission_control.flags & 0x100) == 0)
    {
        scene_geometry_build_plane_view(game_runtime.camera_view.pose.position, angles, game_runtime.camera_proj.size, &geometry);
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
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_terrain(&game_runtime.scene_assets, ordering + 4, packet, &geometry);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    if ((game_runtime.mission_control.flags & 8) != 0)
    {
        position[0] = game_runtime.camera_view.pose.position[0] + 0x40000000;
        position[1] = game_runtime.camera_view.pose.position[1] + 0xFF400000;
        position[2] = game_runtime.camera_view.pose.position[2];
        scene_geometry_build_plane_view(position, angles, game_runtime.camera_proj.size, &geometry);
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
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_terrain(&game_runtime.scene_assets, ordering + 4, packet, &geometry);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    if ((game_runtime.mission_control.flags & 4) != 0)
    {
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_emit_bucket_copies(ordering + 4, packet);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    count = game_runtime.terrain_capture.count;
    if (count != 0)
    {
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_capture_records(ordering + 4, packet, game_runtime.terrain_capture.records, count);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    if ((game_runtime.mission_control.flags & 0x200) == 0)
    {
        packet = game_runtime.mission_control.packet_cur;
        index = game_runtime.display_buffer;
        ordering = game_runtime.mission_control.ot;
        rectangle = &game_runtime.surfaces.bank[index].draw->clip;
        result = scene_draw_clip_mask(ordering + 4, packet, rectangle, &game_runtime.mission_view.center);
        game_runtime.mission_control.packet_cur = (uint32)(result);
    }
    ordering = game_runtime.mission_control.ot;
    kind = game_runtime.render_settings.terrain_flags & 7;
    ordering += 16380;
    if (kind == 2)
    {
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_mode_two_pass(&geometry, ordering, packet);
    }
    else if (kind < 3)
    {
        if (kind != 1)
            return;
        packet = game_runtime.mission_control.packet_cur;
        result = scene_light_draw_pass(ordering, packet);
    }
    else if (kind == 3)
    {
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_sky_mode_three(ordering, packet);
    }
    else if (kind == 4)
    {
        packet = game_runtime.mission_control.packet_cur;
        result = scene_geometry_draw_sky_mode_four(ordering, packet);
    }
    else
        return;
    game_runtime.mission_control.packet_cur = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_prepare_capture_textures(uint32 colors, MISSION_TERRAIN_CAPTURE_RECORD *records, uint32 count)
{
    uint16 wire_halfword_0;
    PSX_RECT capture_rectangle = {320, 256, 16, 64};
    PSX_RECT rectangle;
    uint8 output[2048];
    /* Defines pixels not written by a failed or incomplete capture */
    uint8 capture[2048] = {0};
    uint32 result, i, x, y, odd_x, odd_y, factor, value, part;
    sint32 index;
    FUNCTION_MARKER(0x8005EBB8, "ACE3.BIN");
    DrawSync(0);
    result = game_capture_image(capture_rectangle, capture, sizeof(capture));
    if (result == 0xFFFFFFFF)
    {
#ifndef NDEBUG
        printf("StoreImage is -1\n");
#endif
    }
    DrawSync(0);
    x = 320;
    y = 384;
    odd_x = 0;
    odd_y = 240;
    i = 0;
    do
    {
        factor = rsin(i << 6);
        scene_palette_blend_pixels(output, capture, 1024, colors, factor);
        if ((i & 1) != 0)
        {
            rectangle.x = (sint16)odd_x;
            odd_x += 64;
            rectangle.y = (sint16)odd_y;
            rectangle.w = 16;
            rectangle.h = 16;
            if ((sint32)odd_x >= 256)
            {
                odd_x = 0;
                odd_y = 496;
            }
            game_upload_image(rectangle, output, sizeof(output));
            for (part = 1; part < 4; ++part)
            {
                rectangle.x = (sint16)((uint16)rectangle.x + 16);
                if (part < 3)
                    game_upload_image(rectangle, output + part * 512, sizeof(output) - part * 512);
            }
            i += 1;
            game_upload_image(rectangle, output + 1536, sizeof(output) - 1536);
        }
        else
        {
            rectangle.x = (sint16)x;
            x += 16;
            rectangle.h = 64;
            rectangle.y = (sint16)y;
            rectangle.w = 16;
            if ((sint32)x >= 384)
            {
                x = 320;
                y += 64;
            }
            i += 1;
            game_upload_image(rectangle, output, sizeof(output));
        }
        DrawSync(0);
        result = (sint32)i < 16;
    } while (result != 0);
    i = 0;
    if (count == 0)
    {
        (void)(uint32)(result);
        return;
    }
    do
    {
        index = (sint16)records[i].texture;
        /* Reject indices outside the original 1024-pixel local array */
        value = index >= 0 && index < 1024 ? (memcpy(&wire_halfword_0, capture + index * 2, sizeof(wire_halfword_0)), wire_halfword_0) : 0;
        records[i].sample = (uint16)value;
        i += 1;
        result = i < count;
    } while (result != 0);
    {
        (void)(uint32)(result);
        return;
    }
}

static uint32 scene_plane_minimum_x_index(const uint32 points[][4], uint32 count)
{
    uint32 index, best, current, candidate;
    FUNCTION_MARKER(0x8005F174, "ACE3.BIN");
    best = 0;
    for (index = 1; (sint32)index < (sint32)count; ++index)
    {
        current = points[best][0];
        candidate = points[index][0];
        if ((sint32)candidate < (sint32)current)
            best = index;
    }
    return best;
}

static uint32 scene_plane_maximum_x_index(const uint32 points[][4], uint32 count)
{
    uint32 index, best, current, candidate;
    FUNCTION_MARKER(0x8005F124, "ACE3.BIN");
    best = 0;
    for (index = 1; (sint32)index < (sint32)count; ++index)
    {
        current = points[best][0];
        candidate = points[index][0];
        if ((sint32)current < (sint32)candidate)
            best = index;
    }
    return best;
}

static void scene_plane_unproject_point(uint32 divisor, const uint16 input[3], uint32 output[3])
{
    uint32 a, b, numerator, vector[3];
    FUNCTION_MARKER(0x8005F734, "ACE3.BIN");
    a = (sint16)input[0];
    b = (sint16)input[2];
    numerator = a * b;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    vector[0] = ((sint32)numerator / (sint32)divisor);
    a = (sint16)input[1];
    b = (sint16)input[2];
    numerator = a * b;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    vector[1] = ((sint32)numerator / (sint32)divisor);
    vector[2] = (sint16)input[2];
    game_rotate_vector_current_gte(vector);
    output[0] = vector[0];
    output[1] = vector[1];
    output[2] = vector[2];
}

static void scene_plane_project_point(const uint32 input[3], uint16 output[2])
{
    uint32 scale, x, y, vector[3], numerator, value;
    FUNCTION_MARKER(0x8005F7FC, "ACE3.BIN");
    scale = geom_get_screen();
    x = xport_gte_read_control(24);
    y = xport_gte_read_control(25);
    x = ((sint32)x >> 16);
    y = ((sint32)y >> 16);
    vector[0] = input[0];
    vector[1] = input[1];
    vector[2] = input[2];
    game_rotate_vector_current_gte(vector);
    if (vector[2] == 0)
        vector[2] = 1;
    numerator = vector[0] * scale;
    if (vector[2] == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (x & 0xFFFF) + (uint32)((sint32)numerator / (sint32)vector[2]);
    output[0] = value;
    numerator = vector[1] * scale;
    if (vector[2] == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (y & 0xFFFF) + (uint32)((sint32)numerator / (sint32)vector[2]);
    output[1] = value;
}

static uint32 scene_plane_intersect_ray(const uint32 origin[3], const uint32 vector[3], uint32 plane, uint32 output[3])
{
    uint32 height, divisor, delta, value, numerator, other;
    FUNCTION_MARKER(0x8005F510, "ACE3.BIN");
    height = origin[1];
    divisor = vector[1];
    delta = plane - height;
    if ((sint32)divisor > 0)
    {
        if ((sint32)plane < (sint32)height)
            return 0;
    }
    else if ((sint32)divisor < 0)
    {
        if ((sint32)height < (sint32)plane)
            return 0;
    }
    else if (plane != height)
        return 0;
    if (divisor)
    {
        value = vector[0];
        numerator = value * delta;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = ((sint32)numerator / (sint32)divisor);
        output[0] = value;
        value = vector[2];
        numerator = value * delta;
        divisor = vector[1];
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        value = ((sint32)numerator / (sint32)divisor);
        output[1] = delta;
    }
    else
    {
        value = vector[0];
        output[0] = value;
        value = vector[2];
        output[1] = delta;
    }
    output[2] = value;
    value = vector[0];
    other = output[0];
    if ((sint32)value < 0 ? (sint32)other < (sint32)value : (sint32)value < (sint32)other)
    {
        output[0] = value;
        value = vector[1];
        output[1] = value;
    }
    value = vector[2];
    other = output[2];
    if ((sint32)value < 0 ? (sint32)other < (sint32)value : (sint32)value < (sint32)other)
    {
        output[2] = value;
        value = vector[1];
        output[1] = value;
    }
    value = output[1];
    other = vector[1];
    return value != other;
}

void scene_geometry_intersect_plane_view(const uint16 viewport_size[2], const uint32 position[3], const uint16 angles[3], SCENE_TERRAIN_VIEW *view)
{
    /* Defines failed plane intersections and unused vector padding */
    uint32 points[28][4] = {0}, origin[3], chosen[4], ordered[4], indices[7], final_x[7], final_z[7];
    uint16 corners[4][3], projected[4][3], xy[2], local_angles[3];
    MATRIX matrix = {0}, transpose = {0}, identity = {0};
    uint32 projection, value, a, b, width, height, numerator, flags, selected, below, full, count, minimum, maximum, opposite, plane_kind, i, j, candidate, index, result, minimum_x, minimum_z;
    FUNCTION_MARKER(0x8005F90C, "ACE3.BIN");
    view->vertex_count = 0;
    projection = geom_get_screen();
    value = viewport_size[0];
    width = (sint32)(value << 16) >> 17;
    numerator = (width << 15) - width;
    if (projection == 0)
        xport_mips_break(7);
    if (projection == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    width = (sint32)numerator / (sint32)projection;
    a = position[0];
    value = viewport_size[1];
    origin[0] = (sint32)a >> 12;
    a = position[1];
    origin[1] = (sint32)a >> 12;
    a = position[2];
    origin[2] = (sint32)a >> 12;
    view->origin[0] = origin[0];
    view->origin[1] = origin[1];
    view->origin[2] = origin[2];
    height = (sint32)(value << 16) >> 17;
    numerator = (height << 15) - height;
    view->pitch = angles[0];
    if (projection == 0)
        xport_mips_break(7);
    if (projection == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    height = (sint32)numerator / (sint32)projection;
    local_angles[0] = angles[0];
    local_angles[1] = 0;
    local_angles[2] = angles[2];
    below = (sint32)origin[1] < -3000;
    RotMatrixYXZ((SVECTOR *)local_angles, &matrix);
    TransposeMatrix(&matrix, &transpose);
    matrix.t[0] = 0;
    matrix.t[1] = 0;
    matrix.t[2] = 0;
    transpose.t[0] = 0;
    transpose.t[1] = 0;
    transpose.t[2] = 0;
    for (i = 0; i < 4; ++i)
    {
        corners[i][0] = i == 0 || i == 3 ? 0 - width : width;
        corners[i][1] = i < 2 ? 0 - height : height;
        corners[i][2] = 32767;
    }
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    for (i = 0; i < 4; ++i)
        scene_geometry_transform_short_vector(corners[i], points[i]);
    i = 8;
    identity = psx_identity_matrix;
    indices[0] = 0xFFFFFFFF;
    indices[2] = scene_plane_minimum_x_index(points, 4);
    indices[5] = scene_plane_maximum_x_index(points, 4);
    flags = 15;
    selected = 37;
    for (i = 0; i < 4; ++i)
    {
        if (scene_plane_intersect_ray(origin, points[i], 0, points[i + 8]) != 0)
            flags |= 256 << i;
        if (scene_plane_intersect_ray(origin, points[i], (uint32)-3000, points[i + 12]) != 0)
            flags |= 4096 << i;
    }
    if ((flags & 0xF00) != 0)
    {
        SetRotMatrix(&identity);
        SetTransMatrix(&identity);
        for (i = 0; i < 4; ++i)
        {
            scene_plane_project_point(points[i + 8], xy);
            projected[i][0] = xy[0];
            projected[i][1] = xy[1];
        }
        for (i = 0; i < 4; ++i)
        {
            points[i + 16][1] = 0;
            points[i + 16][0] = (sint16)projected[i][0];
            points[i + 16][2] = (sint16)projected[i][1];
        }
    }
    if ((flags & 0xF000) != 0)
    {
        SetRotMatrix(&identity);
        SetTransMatrix(&identity);
        for (i = 0; i < 4; ++i)
        {
            scene_plane_project_point(points[i + 12], xy);
            projected[i][0] = xy[0];
            projected[i][1] = xy[1];
        }
        for (i = 0; i < 4; ++i)
        {
            points[i + 20][1] = 0;
            points[i + 20][0] = (sint16)projected[i][0];
            points[i + 20][2] = (sint16)projected[i][1];
        }
    }
    result = flags & 0xFF00;
    if (below != 0 && result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    corners[0][0] = 0x8000;
    corners[0][2] = 32767;
    corners[1][0] = 32767;
    corners[1][2] = 32767;
    corners[2][0] = 0x8000;
    corners[2][2] = 32767;
    corners[3][0] = 32767;
    corners[3][2] = 32767;
    corners[0][1] = 0 - origin[1];
    corners[1][1] = 0 - origin[1];
    corners[2][1] = 0 - origin[1] - 3000;
    corners[3][1] = 0 - origin[1] - 3000;
    SetRotMatrix(&transpose);
    for (i = 0; i < 4; ++i)
        scene_geometry_project_short_vector(corners[i], projected[i]);
    SetRotMatrix(&matrix);
    a = viewport_size[0];
    b = viewport_size[1];
    if (scene_geometry_clip_line(projected[0], projected[1], (sint32)(a << 16) >> 17, (sint32)(b << 16) >> 17) != 0)
    {
        projected[0][2] = 32767;
        projected[1][2] = 32767;
        scene_plane_unproject_point(projection, projected[0], points[4]);
        scene_plane_unproject_point(projection, projected[1], points[5]);
        flags |= 0x30;
        selected |= 0x18;
        indices[3] = 4;
        indices[4] = 5;
    }
    a = viewport_size[0];
    b = viewport_size[1];
    if (scene_geometry_clip_line(projected[2], projected[3], (sint32)(a << 16) >> 17, (sint32)(b << 16) >> 17) != 0)
    {
        projected[2][2] = 32767;
        projected[3][2] = 32767;
        scene_plane_unproject_point(projection, projected[2], points[6]);
        scene_plane_unproject_point(projection, projected[3], points[7]);
        flags |= 0xC0;
        if ((selected & 8) == 0 || (sint32)points[6][0] < (sint32)points[4][0])
        {
            indices[3] = 6;
            selected |= 8;
        }
        if ((selected & 16) == 0 || (sint32)points[5][0] < (sint32)points[7][0])
        {
            indices[4] = 7;
            selected |= 16;
        }
    }
    full = 0;
    if (below != 0)
    {
        if ((flags & 0xF00) == 0xF00 || ((flags & 0xF000) == 0xF000 && origin[1] != 0))
            full = 1;
    }
    else if ((flags & 0xF00) == 0xF00 || ((flags & 0xF000) == 0xF000 && origin[1] != (uint32)-3000))
        full = 1;
    if (full != 0)
    {
        minimum = 0;
        maximum = 0;
        plane_kind = 0;
        if ((flags & 0xF000) == 0xF000)
        {
            minimum = scene_plane_minimum_x_index(&points[12], 4);
            maximum = scene_plane_maximum_x_index(&points[12], 4);
            plane_kind = 2;
        }
        if ((flags & 0xF00) == 0xF00)
        {
            minimum = scene_plane_minimum_x_index(&points[8], 4);
            maximum = scene_plane_maximum_x_index(&points[8], 4);
            plane_kind |= 1;
        }
        opposite = scene_geometry_order_plane_boundary(points, minimum, maximum, ordered);
        if (scene_terrain_polygon_has_consistent_winding(points, 4) != 0)
        {
            if (below != 0)
                for (i = 0; i < 4; ++i)
                    indices[i] = (flags & (256 << ordered[i])) != 0 ? ordered[i] + 8 : ordered[i];
            else
            {
                value = (sint16)view->pitch;
                for (i = 0; i < 4; ++i)
                    indices[i] = ordered[i] + ((sint32)value > 0 ? 12 : 8);
            }
            selected = 15;
        }
        else if (below != 0 || origin[1] == (uint32)-3000)
        {
            selected = 15;
            indices[0] = ordered[0] + 12;
            if (opposite != 0)
            {
                indices[1] = ordered[1] + 8;
                indices[2] = ordered[2] + 8;
                indices[3] = ordered[3] + 8;
            }
            else
            {
                indices[1] = ordered[0] + 8;
                indices[2] = ordered[1] + 8;
                indices[3] = ordered[2] + 8;
                indices[4] = ordered[3] + 8;
                indices[5] = ordered[3] + 12;
                selected = 63;
            }
        }
        else
        {
            indices[0] = 0xFFFFFFFF;
            value = (plane_kind & 2) != 0 ? 12 : 8;
            for (i = 0; i < 4; ++i)
                indices[i + 1] = ordered[i] + value;
            selected = 31;
        }
    }
    else if (below != 0)
    {
        selected &= 0xFFFFFFFE;
        count = 0;
        candidate = 0;
        for (i = 0; i < 4; ++i)
            if ((flags & (4096 << i)) != 0)
            {
                candidate = i + 12;
                chosen[count] = candidate;
                for (j = 0; j < 3; ++j)
                    points[24 + count][j] = points[candidate][j];
                ++count;
            }
        if (count == 1)
        {
            if ((selected & 8) != 0)
                selected &= 0xFFFFFFFB;
            if ((selected & 16) != 0)
                selected &= 0xFFFFFFDF;
            selected |= 1;
            indices[0] = chosen[0];
        }
        else if (count == 2)
        {
            maximum = scene_plane_maximum_x_index(&points[24], 2);
            minimum = scene_plane_minimum_x_index(&points[24], 2);
            indices[1] = chosen[minimum];
            selected |= 0x42;
            indices[6] = chosen[maximum];
        }
        else if (count == 3)
        {
            maximum = scene_plane_maximum_x_index(&points[24], 3);
            minimum = scene_plane_minimum_x_index(&points[24], 3);
            selected |= 0x42;
            indices[1] = chosen[minimum];
            indices[6] = chosen[maximum];
            for (i = 0; i < 3; ++i)
                if ((~(1 << maximum) & 7 & ~(1 << minimum) & (1 << i)) != 0)
                {
                    candidate = i;
                    break;
                }
            indices[0] = chosen[candidate];
            value = chosen[minimum];
            selected |= 1;
            if ((flags & (256 << ((value - 12) & 31))) != 0)
            {
                indices[2] = value - 4;
                selected |= 4;
            }
            value = chosen[maximum];
            if ((flags & (256 << ((value - 12) & 31))) != 0)
            {
                indices[5] = value - 4;
                selected |= 0x20;
            }
        }
        /* Four lower intersections imply full above because below guarantees a nonzero height */
    }
    else
    {
        count = 0;
        for (i = 0; i < 4; ++i)
            if ((flags & (256 << i)) != 0)
            {
                for (j = 0; j < 3; ++j)
                    points[24 + count][j] = points[8 + i][j];
                chosen[count++] = i + 8;
            }
        if ((sint32)count >= 3)
        {
            minimum = scene_plane_minimum_x_index(&points[24], count);
            maximum = scene_plane_maximum_x_index(&points[24], count);
            indices[2] = chosen[minimum];
            indices[5] = chosen[maximum];
        }
    }
    minimum_x = 0x7FFFFFFF;
    minimum_z = 0x7FFFFFFF;
    local_angles[0] = 0;
    local_angles[1] = angles[1];
    local_angles[2] = 0;
    RotMatrixYXZ((SVECTOR *)local_angles, &matrix);
    SetRotMatrix(&matrix);
    for (i = 0; i < 7; ++i)
        if ((selected & (1 << i)) != 0)
        {
            index = indices[i];
            count = view->vertex_count;
            if ((sint32)index < 0)
            {
                final_z[count] = 0;
                final_x[count] = 0;
            }
            else
            {
                for (j = 0; j < 3; ++j)
                    points[24][j] = points[index][j];
                game_rotate_vector_current_gte(points[24]);
                final_x[count] = points[24][0];
                final_z[count] = points[24][2];
            }
            if ((sint32)final_x[count] < (sint32)minimum_x)
                minimum_x = final_x[count];
            if ((sint32)final_z[count] < (sint32)minimum_z)
                minimum_z = final_z[count];
            view->vertex_count = count + 1;
        }
    result = view->vertex_count;
    index = 0;
    if ((sint32)result > 0)
        do
        {
            final_x[index] -= 32768 + minimum_x;
            final_z[index] -= 32768 + minimum_z;
            if ((sint32)final_x[index] < -32768)
                final_x[index] = -32768;
            else if ((sint32)final_x[index] > 32767)
                final_x[index] = 32767;
            if ((sint32)final_z[index] < -32768)
                final_z[index] = -32768;
            else if ((sint32)final_z[index] > 32767)
                final_z[index] = 32767;
            view->vertices[index][0] = (uint16)(final_x[index]);
            ++index;
            view->vertices[index - 1][1] = (uint16)(final_z[index - 1]);
            count = view->vertex_count;
            result = (sint32)index < (sint32)count;
        } while (result != 0);
    game_runtime.terrain_origin.x = (sint32)(minimum_x);
    game_runtime.terrain_origin.z = (sint32)(minimum_z);
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_build_view_row_mask(SCENE_TERRAIN_VIEW *view)
{
    /* Keeps neighboring local arrays contiguous and defines unwritten rows */
    uint32 grid[96] = {0}, end_x[7], end_y[7], start_x[7], start_y[7], edge_start[7];
    uint16 bounds[4];
    sint32 vertices, min_x, min_y, max_x, max_y, nx, ny, x0, y0, x1, y1, low_x, low_y, high_x, high_y, sx, sy, ex, ey, row, column, edge, previous_column, bit, i;
    uint32 value, raw, first, second, packed, bits, mask, prior, result;
    FUNCTION_MARKER(0x80060CB8, "ACE3.BIN");
    vertices = view->vertex_count;
    if (vertices == 0)
    {
        view->first_column = 0;
        view->first_row = 0;
        view->column_count = 0;
        view->row_count = 0;
        {
            view->row_mask[0] = 0;
        }
        return;
    }
    for (i = 0; i < 32; ++i)
    {
        grid[i] = 0xFFFFFFFF;
        grid[32 + i] = 0;
    }
    min_x = 32767;
    min_y = 32767;
    max_x = -32768;
    max_y = -32768;
    for (i = 0; i < vertices; ++i)
    {
        value = (sint16)view->vertices[i][0];
        raw = view->vertices[i][0];
        if (max_x < (sint32)value)
            max_x = (sint16)raw;
        value = (sint16)view->vertices[i][1];
        raw = view->vertices[i][1];
        if (max_y < (sint32)value)
            max_y = (sint16)raw;
        value = (sint16)view->vertices[i][0];
        raw = view->vertices[i][0];
        if ((sint32)value < min_x)
            min_x = (sint16)raw;
        value = (sint16)view->vertices[i][1];
        raw = view->vertices[i][1];
        if ((sint32)value < min_y)
            min_y = (sint16)raw;
    }
    bounds[0] = min_x & 0xF800;
    bounds[2] = (max_x + 2047) & 0xF800;
    nx = ((sint32)(sint16)bounds[2] - (sint32)(sint16)bounds[0]) >> 11;
    bounds[1] = min_y & 0xF800;
    bounds[3] = (max_y + 2047) & 0xF800;
    ny = ((sint32)(sint16)bounds[3] - (sint32)(sint16)bounds[1]) >> 11;
    if (nx < 0 || ny < 0 || nx >= 32 || ny >= 32)
    {
#ifndef NDEBUG
        result = printf("Error test_makegrid: nx(%d),ny(%d)\n", (sint32)(nx), (sint32)(ny));
#else
        result = 0;
#endif
        view->first_column = 0;
        view->first_row = 0;
        view->column_count = 0;
        view->row_count = 0;
        {
            (void)(uint32)(result);
            return;
        }
    }
    for (edge = 0; edge < vertices; ++edge)
    {
        i = edge % vertices;
        value = (edge + 1) % vertices;
        x0 = (sint16)view->vertices[i][0];
        x1 = (sint16)view->vertices[value][0];
        y0 = (sint16)view->vertices[i][1];
        y1 = (sint16)view->vertices[value][1];
        low_x = x1 < x0 ? x1 : x0;
        high_x = x1 < x0 ? x0 : x1;
        low_y = y1 < y0 ? y1 : y0;
        high_y = y1 < y0 ? y0 : y1;
        value = (uint32)low_x & 0xFFFFF800;
        raw = (uint32)low_y & 0xFFFFF800;
        edge_start[edge] = (value & 0xFFFF) | (raw << 16);
        start_x[edge] = (uint32)((sint32)(value - (uint32)(sint32)(sint16)bounds[0]) >> 11);
        start_y[edge] = (uint32)((sint32)(raw - (uint32)(sint32)(sint16)bounds[1]) >> 11);
        end_x[edge] = start_x[edge] + (uint32)((sint32)(((uint32)(high_x + 2047) & 0xFFFFF800) - value) >> 11);
        end_y[edge] = start_y[edge] + (uint32)((sint32)(((uint32)(high_y + 2047) & 0xFFFFF800) - raw) >> 11);
    }
    for (edge = 0; edge < vertices; ++edge)
    {
        first = ((uint32)view->vertices[edge % vertices][0] | ((uint32)view->vertices[edge % vertices][1] << 16));
        xport_gte_write_data(12, first);
        second = ((uint32)view->vertices[(edge + 1) % vertices][0] | ((uint32)view->vertices[(edge + 1) % vertices][1] << 16));
        xport_gte_write_data(13, second);
        sx = start_x[edge];
        ex = end_x[edge];
        sy = start_y[edge];
        ey = end_y[edge];
        packed = edge_start[edge] & 0xFFFF0000;
        for (row = sy; row <= ey; ++row)
        {
            bits = 0;
            packed = (packed & 0xFFFF0000) | (edge_start[edge] & 0xFFFF);
            xport_gte_write_data(14, packed);
            for (column = sx; column <= ex; ++column)
            {
                xport_gte_execute(0x1400006);
                packed = (packed & 0xFFFF0000) | ((packed + 2048) & 0xFFFF);
                value = xport_gte_read_data(24);
                if ((sint32)value > 0)
                    bits |= (uint32)1 << (column & 31);
                xport_gte_write_data(14, packed);
            }
            grid[64 + row] = ~bits;
            packed += 0x08000000;
        }
        for (row = sy + 1; row < ey; ++row)
        {
            previous_column = sx;
            for (column = sx + 1; column < ex; ++column)
            {
                value = grid[64 + row];
                if ((((uint32)((sint32)value >> (previous_column & 31)) ^ (uint32)((sint32)value >> (column & 31))) & 1) != 0)
                {
                    mask = (uint32)1 << (previous_column & 31);
                    grid[32 + row - 1] |= mask;
                    grid[32 + row] |= mask;
                    break;
                }
                previous_column = column;
            }
        }
        for (column = sx + 1; column < ex; ++column)
        {
            prior = grid[64 + sy];
            for (row = sy + 1; row < ey; ++row)
            {
                value = grid[64 + row];
                raw = prior;
                prior = value;
                if (((raw ^ value) & ((uint32)1 << (column & 31))) != 0)
                {
                    grid[32 + row - 1] |= (uint32)3 << ((column - 1) & 31);
                    break;
                }
            }
        }
        if (ex - sx < 2)
            grid[32 + ey - 1] |= (uint32)1 << (sx & 31);
        else
            for (column = sx + 1; column < ex; ++column)
            {
                if (((grid[64 + ey - 1] ^ grid[64 + ey]) & ((uint32)1 << (column & 31))) != 0)
                    grid[32 + ey - 1] |= (uint32)3 << ((column - 1) & 31);
            }
        if (ey - sy < 2)
            grid[32 + sy] |= (uint32)1 << ((ex - 1) & 31);
        else
            for (row = sy + 1; row < ey; ++row)
            {
                value = grid[64 + row];
                if ((((uint32)((sint32)value >> ((ex - 1) & 31)) ^ (uint32)((sint32)value >> (ex & 31))) & 1) != 0)
                {
                    mask = (uint32)1 << ((ex - 1) & 31);
                    grid[32 + row - 1] |= mask;
                    grid[32 + row] |= mask;
                }
            }
    }
    for (row = 0; row < ny; ++row)
    {
        mask = 1;
        for (column = 0; column < nx; ++column)
        {
            if ((mask & grid[32 + row]) != 0)
            {
                grid[64 + row] = (uint32)((sint32)mask >> 1);
                break;
            }
            mask |= mask << 1;
        }
    }
    for (row = 0; row < ny; ++row)
    {
        mask = 0x80000000;
        for (bit = 31; bit >= 0; --bit)
        {
            if ((mask & grid[32 + row]) != 0)
            {
                grid[32 + row] = ~((mask << 1) | grid[64 + row]);
                break;
            }
            mask |= (uint32)((sint32)mask >> 1);
        }
    }
    {
        view->first_column = (uint32)((sint32)((uint32)bounds[0] << 16) >> 27);
    }
    {
        view->first_row = (uint32)((sint32)((uint32)bounds[1] << 16) >> 27);
    }
    {
        view->column_count = nx;
    }
    {
        view->row_count = ny;
    }
    result = (uint32)((sint32)((uint32)bounds[3] << 16) >> 27);
    for (row = 0; row < ny; ++row)
    {
        value = grid[32 + row];
        {
            view->row_mask[row] = value;
        }
        result = row + 1 < ny;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_geometry_draw_capture_records(uint32 ordering, uint32 packet, const MISSION_TERRAIN_CAPTURE_RECORD *records, uint32 count)
{
    uint32 *gpu_word;
    const uint16 *indices;
    uint32 red, green, blue, ticks, i, j, value, far_red, far_green, far_blue, shift, phase, sine, factor, texture, x, y, color, packed, flag, bucket, tag, link;
    FUNCTION_MARKER(0x8005EDC8, "ACE3.BIN");
    red = (game_runtime.render_settings.palette_primary & 255) << 4;
    green = ((game_runtime.render_settings.palette_primary >> 8) & 255) << 4;
    blue = ((game_runtime.render_settings.palette_primary >> 16) & 255);
    ticks = game_runtime.gameplay_counter;
    blue <<= 4;
    if (count == 0)
        return packet;
    for (i = 0; i < count; ++i)
    {
        value = (sint16)records[i].sample;
        xport_gte_write_data(28, value);
        far_red = records[i].red;
        far_blue = records[i].blue;
        far_red <<= 4;
        far_green = records[i].green;
        far_blue <<= 4;
        far_green <<= 4;
        xport_gte_write_control(21, far_red);
        xport_gte_write_control(22, far_green);
        xport_gte_write_control(23, far_blue);
        shift = records[i].shift;
        phase = records[i].phase;
        phase += ticks << (shift & 31);
        sine = (uint16)game_wave_at(((phase & 0x3FFF) << 1));
        sine = (sint32)(sine << 16) >> 19;
        factor = sine + 2048;
        xport_gte_write_data(8, factor);
        xport_gte_execute(0x980011);
        indices = game_runtime.mission_depth_buckets;
        texture = records[i].texture;
        flag = far_red & 0x8000;
        x = (texture & 15) + 320;
        y = ((sint32)(texture << 16) >> 20) + 256;
        color = xport_gte_read_data(22);
        xport_gte_write_control(21, red);
        xport_gte_write_control(22, green);
        xport_gte_write_control(23, blue);
        xport_gte_write_data(6, color);
        packed = (y << 16) | x;
        for (j = 0; j < 16; ++j)
        {
            xport_gte_write_data(8, j << 8);
            xport_gte_execute(0x780010);
            value = ((sint16)*indices);
            bucket = ordering + (value << 2);
            value = xport_gte_read_data(29) | flag;
            DR_LOAD *upload = mission_packet_span(packet, offsetof(DR_LOAD, p) + 2 * sizeof(uint32));
            upload->tag = 0x05000000;
            upload->p[0] = value;
            upload->code[0] = 0xA0000000;
            upload->code[1] = packed;
            upload->code[2] = 0x10001;
            upload->p[1] = 0x01000000;
            ++indices;
            tag = upload->tag;
            gpu_word = mission_ot_span(bucket, sizeof(*gpu_word));
            link = *gpu_word;
            upload->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            link = *gpu_word;
            *gpu_word = (link & 0xFF000000) | (packet & 0xFFFFFF);
            packet += 24;
        }
    }
    return packet;
}

void scene_geometry_enable_bucket_copies(uint32 enabled)
{
    uint32 flags;
    FUNCTION_MARKER(0x8005F0E4, "ACE3.BIN");
    flags = game_runtime.mission_control.flags;
    if (enabled != 0)
    {
        flags |= 4;
        game_runtime.mission_control.flags = (uint32)(flags);
        return;
    }
    game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFFB);
    return;
}

uint32 scene_terrain_quad_exceeds_span(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    sint32 a, b, c, d, high_ab, high_cd, high, low_ab, low_cd, low;
    FUNCTION_MARKER(0x800963F8, "ACE3.BIN");
    a = (sint32)first >> 16;
    b = (sint32)second >> 16;
    c = (sint32)third >> 16;
    d = (sint32)fourth >> 16;
    high_ab = b < a ? a : b;
    high_cd = d < c ? c : d;
    high = high_cd < high_ab ? high_ab : high_cd;
    low_ab = a < b ? a : b;
    low_cd = c < d ? c : d;
    low = low_ab < low_cd ? low_ab : low_cd;
    if (high - low >= 512)
        return 1;
    a = (sint32)(first << 16) >> 16;
    b = (sint32)(second << 16) >> 16;
    c = (sint32)(third << 16) >> 16;
    d = (sint32)(fourth << 16) >> 16;
    high_ab = b < a ? a : b;
    high_cd = d < c ? c : d;
    high = high_cd < high_ab ? high_ab : high_cd;
    low_ab = a < b ? a : b;
    low_cd = c < d ? c : d;
    low = low_ab < low_cd ? low_ab : low_cd;
    return high - low >= 1024;
}

void scene_terrain_average_vertex(TERRAIN_VERTEX *out, const TERRAIN_VERTEX *first, const TERRAIN_VERTEX *second)
{
    uint32 a, b, value, low, high;
    FUNCTION_MARKER(0x80096518, "ACE3.BIN");
    a = (sint16)first->xy;
    b = (sint16)second->xy;
    value = a + b;
    out->xy = (uint16)((sint32)value >> 1);
    a = (sint16)(first->xy >> 16);
    b = (sint16)(second->xy >> 16);
    value = a + b;
    out->xy |= (uint32)(uint16)((sint32)value >> 1) << 16;
    a = (sint16)first->z;
    b = (sint16)second->z;
    value = a + b;
    out->z = (uint16)((sint32)value >> 1);
    a = first->color;
    b = second->color;
    low = (((a & 0x00FF00FF) + (b & 0x00FF00FF)) >> 1) & 0x00FF00FF;
    high = (((a & 0xFF00FF00) + (b & 0xFF00FF00)) >> 1) & 0xFF00FF00;
    out->color = low | high;
    a = first->uv;
    b = second->uv;
    low = ((a & 0x00FF00FF) + (b & 0x00FF00FF)) >> 1;
    value = (a & 0xFF00FF00) + (b & 0xFF00FF00);
    high = ((uint32)((sint32)value >> 1)) & 0xFF00;
    out->uv = (uint16)(low | high);
    {
        (void)(uint32)(high);
        return;
    }
}

uint32 scene_geometry_draw_models(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet)
{
    SCENE_RENDER_STREAM stream;
    SCENE_GEOMETRY_WORKSPACE geometry;
    SCENE_GEOMETRY_COMPOSITE_BUFFER animated = {geometry.animated, SCENE_GEOMETRY_ANIMATED_CAPACITY};
    SCENE_GEOMETRY_COMPOSITE_BUFFER fixed = {geometry.fixed, SCENE_GEOMETRY_FIXED_CAPACITY};
    SCENE_GEOMETRY_MESH *meshes = geometry.meshes;
    SCENE_GEOMETRY_SPRITE *sprites = geometry.sprites;
    uint32 count, a, b, c, d, e, value;
    FUNCTION_MARKER(0x80068334, "ACE3.BIN");
    count = scene_geometry_collect_primary(geometry.descriptors);
    scene_geometry_collect_secondary(geometry.descriptors + count);
    scene_geometry_prepare_models(assets, geometry.descriptors, meshes, &animated, &fixed, sprites);
    stream.ordering = ordering;
    stream.packet = packet;
    scene_geometry_interpolate_sprites(sprites, &stream);
    scene_geometry_interpolate_meshes(meshes, &stream);
    scene_geometry_interpolate_composites(&animated, &stream);
    a = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][1] << 16));
    b = ((uint32)(uint16)game_runtime.mission_matrices.view.m[0][2] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][0] << 16));
    c = ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][1] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[1][2] << 16));
    d = ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][0] | ((uint32)(uint16)game_runtime.mission_matrices.view.m[2][1] << 16));
    e = (uint16)game_runtime.mission_matrices.view.m[2][2];
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    scene_geometry_draw_sprites(sprites, &stream);
    scene_geometry_draw_meshes(meshes, &stream);
    value = (uint16)game_runtime.mission_control.ticks;
    scene_geometry_draw_composites(&animated, (uint16)value, &stream);
    scene_geometry_draw_composites(&fixed, (uint16)value, &stream);
    return stream.packet;
}

void scene_geometry_transform_short_vector(const uint16 vector[3], uint32 output[3])
{
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x480012);
    output[0] = xport_gte_read_data(25);
    output[1] = xport_gte_read_data(26);
    output[2] = xport_gte_read_data(27);
    xport_gte_read_control(31);
}

void scene_geometry_project_short_vector(const uint16 vector[3], uint16 output[2])
{
    uint32 screen;
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    screen = xport_gte_read_data(14);
    output[0] = screen;
    output[1] = screen >> 16;
    xport_gte_read_data(8);
    xport_gte_read_control(31);
    xport_gte_read_data(19);
}

uint32 scene_geometry_order_plane_boundary(const uint32 points[4][4], uint32 minimum, uint32 maximum, uint32 ordered[4])
{
    uint32 remaining[4], count = 0, i, choice, first, second, difference = minimum - maximum;
    for (i = 0; i < 4; ++i)
        if ((~(1 << minimum) & 15 & ~(1 << maximum) & (1 << i)) != 0)
            remaining[count++] = i;
    if ((sint32)difference < 0)
        difference = 0 - difference;
    if (difference == 2)
    {
        choice = (sint32)points[remaining[1]][2] < (sint32)points[remaining[0]][2];
        ordered[0] = remaining[choice];
        ordered[1] = minimum;
        ordered[2] = remaining[choice ^ 1];
        ordered[3] = maximum;
        return 1;
    }
    choice = (sint32)points[remaining[1]][0] < (sint32)points[remaining[0]][0];
    first = remaining[choice ^ 1];
    second = remaining[choice];
    if ((sint32)points[first][2] < (sint32)points[maximum][2])
    {
        ordered[0] = second;
        ordered[1] = minimum;
        ordered[2] = maximum;
        ordered[3] = first;
    }
    else
    {
        ordered[0] = minimum;
        ordered[1] = second;
        ordered[2] = first;
        ordered[3] = maximum;
    }
    return 0;
}

void scene_geometry_draw_mission_models(void)
{
    uint32 ordering, packet, result;
    FUNCTION_MARKER(0x8003342C, "ACE3.BIN");
    ordering = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    result = scene_geometry_draw_models(&game_runtime.scene_assets, ordering + 4, packet);
    game_runtime.mission_control.packet_cur = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_geometry_draw_mode_two_pass(const SCENE_TERRAIN_VIEW *view, uint32 ordering, uint32 packet)
{
    uint32 result;
    FUNCTION_MARKER(0x8009754C, "ACE3.BIN");
    result = scene_geometry_draw_terrain_mode_two(view, ordering, packet);
    return scene_light_draw_horizon_pass(ordering, result);
}

static void scene_geometry_rotate_columns(const MATRIX *matrix, MATRIX *output)
{
    uint32 column, a, b, c;
    for (column = 0; column < 3; ++column)
    {
        a = ((uint16)matrix->m[0][column]);
        b = ((uint16)matrix->m[1][column]);
        c = ((uint16)matrix->m[2][column]);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        output->m[0][column] = (sint16)a;
        output->m[1][column] = (sint16)b;
        output->m[2][column] = (sint16)c;
    }
}

void scene_geometry_prepare_models(const GAME_SCENE_ASSETS *assets, const SCENE_GEOMETRY_DESCRIPTOR *descriptors, SCENE_GEOMETRY_MESH *meshes, const SCENE_GEOMETRY_COMPOSITE_BUFFER *animated, const SCENE_GEOMETRY_COMPOSITE_BUFFER *fixed, SCENE_GEOMETRY_SPRITE *sprites)
{
    FLIGHT_CONTROL *smooth;
    MATRIX matrix, inverse;
    uint16 angles[3];
    SCENE_GEOMETRY_COMPOSITE *output, *animated_record = animated->records, *fixed_record = fixed->records;
    uint32 animated_index = 0, fixed_index = 0;
    uint32 focal, range, half_width, half_height, dx, dy, dz, x, y, z, distance, selection, level, vertices, a, b, c, d, e, value, radius, depth, screen, screen_x, screen_y, margin, i, kind;
    FUNCTION_MARKER(0x80067494, "ACE3.BIN");
    focal = ((sint16)game_runtime.camera_proj.distance);
    range = focal * 12;
    a = game_runtime.camera_proj.size[0];
    b = game_runtime.camera_proj.size[1];
    half_width = (sint32)(a << 16) >> 17;
    half_height = (sint32)(b << 16) >> 17;
    while (descriptors->model_index != 255)
    {
        a = descriptors->native_pose->position[0];
        b = game_runtime.camera_view.pose.position[0];
        dx = a - b;
        a = descriptors->native_pose->position[1];
        b = game_runtime.camera_view.pose.position[1];
        dy = a - b;
        a = descriptors->native_pose->position[2];
        b = game_runtime.camera_view.pose.position[2];
        dz = a - b;
        x = (sint32)dx >> 10;
        y = (sint32)dy >> 10;
        z = (sint32)dz >> 10;
        a = (sint32)x < 0 ? 0 - x : x;
        b = (sint32)y < 0 ? 0 - y : y;
        c = (sint32)z < 0 ? 0 - z : z;
        distance = a + b + c;
        if ((sint32)distance > 139288)
            goto next_geometry_67494;
        if ((descriptors->kind & 15) != 0)
        {
            selection = descriptors->model_index << 2;
            smooth = &flight_control;
            if ((sint32)(range << 2) < (sint32)distance)
                selection += 2;
            else if ((sint32)(range << 1) < (sint32)distance)
                ++selection;
            if (!assets->model_headers || selection >= assets->model_header_count || !assets->model_headers[selection])
                abort();
            const SCENE_MESH_MODEL_HEADER *mesh_model = assets->model_headers[selection];
            if ((sint32)range < (sint32)distance)
            {
                level = 0;
                vertices = mesh_model->vertices[0];
            }
            else if (((sint32)range >> 1) < (sint32)distance)
            {
                level = 1;
                vertices = mesh_model->vertices[1];
            }
            else
            {
                level = 2;
                vertices = mesh_model->vertices[2];
            }
            x = (sint32)x >> (2 - level);
            y = (sint32)y >> (2 - level);
            z = (sint32)z >> (2 - level);
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
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            xport_gte_execute(0x180001);
            if ((descriptors->kind & 9) != 0)
            {
                meshes->flags = descriptors->flags;
                value = descriptors->parameter;
                meshes->parameter = (uint16)value;
                meshes->level = level;
                meshes->model_data = mesh_model;
                meshes->vertices_address = vertices;
                meshes->vertex_source = scene_mesh_catalog_vertex_input(assets, selection, mesh_model, vertices, &meshes->model_source);
                meshes->lod_model_headers = assets->model_headers && selection < assets->model_header_count ? assets->model_headers + selection : NULL;
                meshes->lod_model_sources = assets->model_header_sources && selection < assets->model_header_count ? assets->model_header_sources + selection : NULL;
                meshes->lod_header_count = meshes->lod_model_headers ? assets->model_header_count - selection : 0;
                meshes->callback = scene_geometry_mesh_callbacks[descriptors->style];
                a = xport_gte_read_data(25);
                meshes->translation.word[0] = a;
                b = xport_gte_read_data(26);
                meshes->translation.word[1] = b;
                c = xport_gte_read_data(27);
                meshes->translation.word[2] = c;
            }
            if ((descriptors->kind & 2) != 0)
            {
                animated_record = output = scene_geometry_composite_record(animated, animated_index);
                output->effect = smooth;
                output->callback = scene_geometry_composite_callbacks[descriptors->variant];
            }
            else
            {
                fixed_record = output = scene_geometry_composite_record(fixed, fixed_index);
                output->effect = 0;
                output->callback = 0;
            }
            if ((descriptors->kind & 9) == 0)
            {
                output->model_source = assets->aircraft_model_source;
                const SCENE_MESH_MODEL_HEADER *const *composite_headers = scene_aircraft_composite_headers(assets);
                for (i = 0; i < 2; ++i)
                {
                    const SCENE_MESH_MODEL_HEADER *component_model = composite_headers[i];
                    value = scene_mesh_model_vertices(component_model, level);
                    output->vertices_address[i] = value;
                    output->models_data[i] = component_model;
                }
                output->models_data[2] = composite_headers[2];
                output->models_data[3] = composite_headers[7];
                output->models_data[4] = composite_headers[9];
                if ((descriptors->kind & 2) != 0)
                {
                    for (i = 0; i < 10; ++i)
                    {
                        const SCENE_MESH_MODEL_HEADER *component_model = i == 0 ? output->models_data[2] : i == 5 ? output->models_data[3] : i == 7 ? output->models_data[4] : composite_headers[2 + i];
                        value = scene_mesh_model_vertices(component_model, level);
                        output->blend_vertices_address[i] = value;
                        if (i == 0)
                            output->vertices_address[2] = value;
                        if (i == 5)
                            output->vertices_address[3] = value;
                        if (i == 7)
                            output->vertices_address[4] = value;
                    }
                }
                else
                {
                    a = scene_mesh_model_vertices(output->models_data[2], level);
                    b = scene_mesh_model_vertices(output->models_data[3], level);
                    c = scene_mesh_model_vertices(output->models_data[4], level);
                    output->vertices_address[4] = c;
                    output->vertices_address[2] = a;
                    output->vertices_address[3] = b;
                }
                for (uint32 mesh_index = 0; mesh_index < 5; ++mesh_index)
                    output->vertex_sources[mesh_index] = scene_mesh_vertex_input(output->model_source, output->vertices_address[mesh_index]);
                output->level = level;
                a = xport_gte_read_data(25);
                output->translation.word[0] = a;
                b = xport_gte_read_data(26);
                output->translation.word[1] = b;
                c = xport_gte_read_data(27);
                output->translation.word[2] = c;
            }
            depth = (sint32)xport_gte_read_data(27) >> level;
            value = (uint32)mesh_model->radius;
            radius = (sint32)value >> (2 - level);
            if ((sint32)depth < (sint32)(0 - radius))
                goto next_geometry_67494;
            screen = xport_gte_read_data(14);
            kind = descriptors->kind;
            if ((kind & 2) != 0)
                animated_record->depth = depth;
            else if ((kind & 4) != 0)
                fixed_record->depth = depth;
            if ((sint32)depth <= 0)
                depth = 1;
            margin = (sint32)(radius * focal) / (sint32)depth;
            screen_x = (sint16)screen;
            screen_y = (sint32)screen >> 16;
            if ((sint32)(radius << 4) < (sint32)margin)
                margin = radius << 2;
            value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
            if ((sint32)(half_width + margin) < (sint32)value)
                goto next_geometry_67494;
            value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
            if ((sint32)(half_height + margin) < (sint32)value)
                goto next_geometry_67494;
            rotation_build(&matrix, descriptors->native_pose->angles);

            kind = descriptors->kind;
            if ((kind & 9) != 0)
            {
                {
                    MATRIX rotated;
                    scene_geometry_rotate_columns(&matrix, &rotated);
                    memcpy(meshes->rotation.component, rotated.m, sizeof(rotated.m));
                }
                ++meshes;
            }
            else
            {
                if ((kind & 2) != 0)
                {
                    if (descriptors->selected != 0)
                    {
                        a = ((sint16)game_runtime.camera_follow.steer[0]);
                        b = ((sint16)game_runtime.camera_follow.steer[1]);
                        value = game_runtime.camera_follow.steer[0];
                        if ((sint32)a < 0)
                            a = 0 - a;
                        if ((sint32)b < 0)
                            b = 0 - b;
                        c = (a + b) << 2;
                        if ((sint32)c > 4096)
                            c = 4096;
                        c = 4096 - c;
                        a = smooth->axis[1];
                        a = ((sint32)(a << 16) >> 20);
                        a = (sint32)(a * c) / 4096;
                        b = smooth->axis[2];
                        b = ((sint32)(b << 16) >> 22);
                        b = (sint32)(b * c) / 4096;
                        d = smooth->axis[0];
                        d = ((sint32)(d << 16) >> 19);
                        d = (sint32)(d * c) / 4096;
                        angles[0] = value - a;
                        value = game_runtime.camera_follow.steer[1];
                        angles[1] = value - b;
                        value = game_runtime.camera_follow.steer[2];
                        angles[2] = value - d;
                        game_build_rotation_via_gte(&inverse, angles);
                        for (i = 0; i < 9; ++i)
                            animated_record->rotation.component[i] = (uint16)inverse.m[i / 3][i % 3];
                        value = ((sint16)game_runtime.camera_shots.offset[0]);
                        animated_record->translation.word[0] = (sint32)(0 - value) >> (6 - level);
                        value = ((sint16)game_runtime.camera_shots.offset[1]);
                        animated_record->translation.word[1] = (sint32)(0 - value) >> (6 - level);
                        value = ((sint16)game_runtime.camera_shots.offset[2]);
                        animated_record->translation.word[2] = (sint32)(0 - value) >> (6 - level);
                    }
                    else
                    {
                        MATRIX rotated;
                        scene_geometry_rotate_columns(&matrix, &rotated);
                        memcpy(animated_record->rotation.component, rotated.m, sizeof(rotated.m));
                    }
                    output = animated_record;
                }
                else
                {
                    {
                        MATRIX rotated;
                        scene_geometry_rotate_columns(&matrix, &rotated);
                        memcpy(fixed_record->rotation.component, rotated.m, sizeof(rotated.m));
                    }
                    output = fixed_record;
                }
                a = ((uint32)(uint16)game_runtime.mission_lighting.directions.m[0][0] | ((uint32)(uint16)game_runtime.mission_lighting.directions.m[0][1] << 16));
                b = ((uint32)(uint16)game_runtime.mission_lighting.directions.m[0][2] | ((uint32)(uint16)game_runtime.mission_lighting.directions.m[1][0] << 16));
                xport_gte_write_control(0, a);
                xport_gte_write_control(1, b);
                c = ((uint32)(uint16)game_runtime.mission_lighting.directions.m[1][1] | ((uint32)(uint16)game_runtime.mission_lighting.directions.m[1][2] << 16));
                d = ((uint32)(uint16)game_runtime.mission_lighting.directions.m[2][0] | ((uint32)(uint16)game_runtime.mission_lighting.directions.m[2][1] << 16));
                e = (uint16)game_runtime.mission_lighting.directions.m[2][2];
                xport_gte_write_control(2, c);
                xport_gte_write_control(3, d);
                xport_gte_write_control(4, e);
                {
                    MATRIX rotated;
                    scene_geometry_rotate_columns(&matrix, &rotated);
                    memcpy(output->light_rotation.component, rotated.m, sizeof(rotated.m));
                }
                if ((kind & 2) != 0)
                {
                    ++animated_index;
                    animated_record = animated->records + animated_index;
                }
                else
                {
                    ++fixed_index;
                    fixed_record = fixed->records + fixed_index;
                }
            }
        }
        else
        {
            x = (sint32)dx >> 12;
            y = (sint32)dy >> 12;
            z = (sint32)dz >> 12;
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
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            xport_gte_execute(0x180001);
            sprites->flags = descriptors->flags;
            value = descriptors->parameter;
            sprites->parameter = (uint16)value;
            kind = descriptors->model_index;
            if (!assets->animated_headers || kind >= assets->animated_header_count)
                abort();
            sprites->lod_model_headers = assets->animated_headers + kind;
            sprites->lod_header_count = assets->animated_header_count - kind;
            sprites->model_source = assets->animated_model_source;
            sprites->vertex_source = (RESOURCE_ASSET_SOURCE){0};
            const SCENE_SPRITE_MODEL_HEADER *sprite_header = sprites->lod_model_headers[0];
            x = xport_gte_read_data(25);
            y = xport_gte_read_data(26);
            z = xport_gte_read_data(27);
            sprites->x = (sint16)x;
            sprites->y = (sint16)y;
            sprites->z = (sint16)z;
            depth = xport_gte_read_data(27);
            const SCENE_SPRITE_MODEL_HEADER *sprite_model = scene_sprite_model_header(sprites->model_source, sprite_header, sizeof(*sprite_model));
            sprites->model_data = sprite_header ? sprite_model : NULL;
            radius = sprite_model->radius;
            if ((sint32)depth < (sint32)(0 - radius))
                goto next_geometry_67494;
            screen = xport_gte_read_data(14);
            if ((sint32)depth <= 0)
                depth = 1;
            margin = (sint32)(radius * focal) / (sint32)depth;
            screen_x = (sint16)screen;
            screen_y = (sint32)screen >> 16;
            if ((sint32)(radius << 4) < (sint32)margin)
                margin = radius << 2;
            value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
            if ((sint32)(half_width + margin) < (sint32)value)
                goto next_geometry_67494;
            value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
            if ((sint32)(half_height + margin) < (sint32)value)
                goto next_geometry_67494;
            ++sprites;
        }
    next_geometry_67494:
        ++descriptors;
    }
    meshes->level = 0xFFFFFFFF;
    scene_geometry_end_composites(animated, animated_index);
    scene_geometry_end_composites(fixed, fixed_index);
    sprites->model_data = NULL;
    return;
}

SCENE_FACE_GROUP scene_model_face_group(const RESOURCE_ASSET_SOURCE *source, uint32 address, uint32 stride)
{
    if (source && source->data && address >= source->encoded_base)
    {
        uint32 offset = address - source->encoded_base;
        uint32 count;
        if (offset <= source->bytes && sizeof(count) <= source->bytes - offset)
        {
            memcpy(&count, source->data + offset, sizeof(count));
            if ((sint32)count <= 0)
                return (SCENE_FACE_GROUP){NULL, count, stride};
            if (stride && count <= (source->bytes - offset - sizeof(count)) / stride && !((uintptr_t)(source->data + offset + sizeof(count)) & 3))
                return (SCENE_FACE_GROUP){source->data + offset + sizeof(count), count, stride};
        }
    }
    return scene_geometry_face_group(address, stride);
}

void scene_geometry_emit_mesh_triangles(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    POLY_GT3 *poly;
    uint32 *ot_entry;
    uint32 source, packet, table, tail, indices, i0, i1, i2;
    uint32 result, value0, value1, value2, ot, shift;
    sint32 count, i, z0, z1, z2, distance;
    FUNCTION_MARKER(0x80062D68, "ACE3.BIN");
    source = stream->primitives;
    packet = stream->packet;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, source, sizeof(SCENE_MESH_TRIANGLE_FACE));
    count = (sint32)group.count;
    table = stream->ordering;
    result = source;
    tail = packet + 36;
    for (i = 0; i < count; ++i)
    {
        const SCENE_MESH_TRIANGLE_FACE *face = (const SCENE_MESH_TRIANGLE_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        i0 = indices & 255;
        i1 = (indices >> 8) & 255;
        i2 = indices >> 16;
        xport_gte_write_data(12, projected->xy[i0].xy);
        xport_gte_write_data(13, projected->xy[i1].xy);
        xport_gte_write_data(14, projected->xy[i2].xy);
        xport_gte_execute(0x1400006);
        if ((sint32)xport_gte_read_data(24) > 0)
        {
            z0 = (sint16)projected->depth[i0];
            z1 = (sint16)projected->depth[i1];
            z2 = (sint16)projected->depth[i2];
            xport_gte_write_data(17, z0);
            xport_gte_write_data(18, z1);
            xport_gte_write_data(19, z2);
            xport_gte_execute(0x158002D);
            distance = (sint32)xport_gte_read_data(24);
            shift = context->level + 10;
            distance >>= shift & 31;
            if ((uint32)distance - 31 < 0x77D9)
            {
                ot = table + (uint32)(distance >> 3) * 4;
                poly = (POLY_GT3 *)mission_packet_span(packet, sizeof(*poly));
                ot_entry = (uint32 *)mission_ot_span(ot, sizeof(*ot_entry));
                poly->tag = *ot_entry | 0x9000000;
                *ot_entry = packet & 0xFFFFFF;
                value0 = face->color[0];
                value1 = face->uv[0];
                value2 = face->color[1];
                poly->p[0].rgbc.rgbc = value0;
                poly->p[0].xy.xy = xport_gte_read_data(12);
                poly->p[0].uv_attr = value1;
                poly->p[1].rgbc.rgbc = value2;
                value0 = face->uv[1];
                value1 = face->color[2];
                value2 = face->uv[2];
                poly->p[1].xy.xy = xport_gte_read_data(13);
                poly->p[1].uv_attr = value0;
                poly->p[2].rgbc.rgbc = value1;
                poly->p[2].xy.xy = xport_gte_read_data(14);
                poly->p[2].uv_attr = value2;
                tail += 40;
                packet += 40;
            }
        }
        result = i + 1 < count;
    }
    stream->packet = packet;
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_emit_mesh_quads(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source)
{
    POLY_GT4 *poly;
    uint32 *ot_entry;
    uint32 source, packet, table, tail, indices, i0, i1, i2, i3;
    uint32 result, first_xy, fourth_xy, value0, value1, value2, ot, shift;
    sint32 count, i, z0, z1, z2, z3, distance, area;
    FUNCTION_MARKER(0x80062EC8, "ACE3.BIN");
    source = stream->primitives;
    packet = stream->packet;
    SCENE_FACE_GROUP group = scene_model_face_group(face_source, source, sizeof(SCENE_MESH_QUAD_FACE));
    count = (sint32)group.count;
    table = stream->ordering;
    result = source;
    tail = packet + 48;
    for (i = 0; i < count; ++i)
    {
        const SCENE_MESH_QUAD_FACE *face = (const SCENE_MESH_QUAD_FACE *)(group.records + i * sizeof(*face));
        indices = face->projected_indices;
        i0 = indices & 255;
        i1 = (indices >> 8) & 255;
        i2 = (indices >> 16) & 255;
        i3 = indices >> 24;
        first_xy = projected->xy[i0].xy;
        xport_gte_write_data(13, projected->xy[i1].xy);
        xport_gte_write_data(14, projected->xy[i2].xy);
        xport_gte_write_data(12, first_xy);
        fourth_xy = projected->xy[i3].xy;
        xport_gte_execute(0x1400006);
        area = (sint32)xport_gte_read_data(24);
        xport_gte_write_data(12, fourth_xy);
        if (area <= 0)
        {
            xport_gte_execute(0x1400006);
            area = (sint32)xport_gte_read_data(24);
            if (area >= 0)
                goto next_quad;
        }
        z0 = (sint16)projected->depth[i0];
        z1 = (sint16)projected->depth[i1];
        z2 = (sint16)projected->depth[i2];
        z3 = (sint16)projected->depth[i3];
        shift = context->level;
        distance = (z0 + z1 + z2 + z3) >> 2;
        distance >>= shift & 31;
        if ((uint32)distance - 31 < 0x77D9)
        {
            ot = table + (uint32)(distance >> 3) * 4;
            poly = (POLY_GT4 *)mission_packet_span(packet, sizeof(*poly));
            ot_entry = (uint32 *)mission_ot_span(ot, sizeof(*ot_entry));
            poly->tag = *ot_entry | 0xC000000;
            *ot_entry = packet & 0xFFFFFF;
            value0 = face->color[0];
            value1 = face->uv[0];
            poly->p[0].xy.xy = first_xy;
            poly->p[0].rgbc.rgbc = value0;
            poly->p[0].uv_attr = value1;
            value0 = face->color[1];
            value1 = face->uv[1];
            value2 = face->color[2];
            poly->p[1].rgbc.rgbc = value0;
            poly->p[1].xy.xy = xport_gte_read_data(13);
            poly->p[1].uv_attr = value1;
            poly->p[2].rgbc.rgbc = value2;
            poly->p[2].xy.xy = xport_gte_read_data(14);
            value0 = face->uv[2];
            value1 = face->color[3];
            poly->p[2].uv_attr = value0;
            poly->p[3].rgbc.rgbc = value1;
            poly->p[3].xy.xy = xport_gte_read_data(12);
            poly->p[3].uv_attr = value0 >> 16;
            tail += 52;
            packet += 52;
        }
    next_quad:
        result = i + 1 < count;
    }
    stream->packet = packet;
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_geometry_set_depth_cue(uint32 near_value, uint32 far_value, uint32 height)
{
    uint64 numerator, quotient, offset, height64;
    uint32 coefficient, result;
    sint64 signed_offset;
    FUNCTION_MARKER(0x8005E7EC, "ACE3.BIN");
    numerator = ((sint64)(sint32)(near_value - far_value) * 0x1000000);
    quotient = __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
    height64 = (sint32)height;
    numerator = quotient * height64;
    offset = __divdi3PSX(numerator, (uint64)(sint64)(sint32)far_value);
    quotient = __divdi3PSX((uint64)1 << 32, offset);
    coefficient = quotient;
    if (coefficient + 0x8000 > 0xFFFF)
    {
        coefficient = (sint32)coefficient > 0 ? 0x7FFF : 0xFFFF8000;
        numerator = ((uint64)(sint64)(sint32)(0 - coefficient) << 16) * height64;
        __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
        numerator = ((uint64)(sint64)(sint32)coefficient << 16) * height64;
        quotient = __divdi3PSX(numerator, (uint64)(sint64)(sint32)far_value);
        offset = (uint64)0x1000000 - quotient;
    }
    else
    {
        numerator = ((uint64)(sint64)(sint32)(0 - coefficient) << 16) * height64;
        offset = __divdi3PSX(numerator, (uint64)(sint64)(sint32)near_value);
    }
    signed_offset = offset;
    if (signed_offset > 2147483647)
        result = 0x7FFFFFFF;
    else if (signed_offset < -2147483647)
        result = 0x80000001;
    else
        result = offset;
    xport_gte_write_control(27, coefficient);
    xport_gte_write_control(28, result);
    {
        (void)(uint32)(result);
        return;
    }
}

void scene_build_depth_buckets(uint32 near_value, uint32 far_value, uint16 *buckets)
{
    uint32 delta = far_value - near_value, i, product;
    sint32 value;
    FUNCTION_MARKER(0x8005EAB8, "ACE3.BIN");
    for (i = 1; i <= 16; ++i)
    {
        product = i * delta;
        value = (sint32)((uint32)((sint32)product / 16) + near_value) >> 3;
        if (value >= 0xFFE)
            value = 0xFFD;
        if (value < 0)
            value = 0;
        buckets[i - 1] = value;
    }
    buckets[16] = 0xFFE;
    return;
}
