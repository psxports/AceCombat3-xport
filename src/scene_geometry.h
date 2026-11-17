#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include "game_math.h"
#include "effect_frame.h"
#include "flight_control.h"
#include "game_runtime.h"

SCENE_FACE_GROUP scene_geometry_face_group(uint32 address, uint32 stride);

typedef struct
{
    uint32 origin[3];
    uint16 pitch;
    uint32 vertex_count;
    uint16 vertices[7][2];
    uint32 tile_base_x, tile_base_z;
    uint32 tile_offset_x, tile_offset_z;
    uint32 first_column, first_row;
    uint32 column_count, row_count;
    uint32 row_mask[32];
} SCENE_TERRAIN_VIEW;

enum { SCENE_TERRAIN_TILE_CAPACITY = 31 * 31 };

typedef struct
{
    uint16 x, y, z;
    uint32 flags;
} SCENE_TERRAIN_TILE;

typedef void (*SCENE_DETAIL_CALLBACK)(EFFECT_FRAME_CONTEXT *frame);

extern SCENE_DETAIL_CALLBACK const scene_geometry_detail_callbacks[5];

enum
{
    SCENE_BLEND_VERTEX_CAPACITY = 93
};

typedef struct
{
    uint32 count;
    SVECTOR vertex[SCENE_BLEND_VERTEX_CAPACITY];
} SCENE_VERTEX_BLEND_WORK;

enum { SCENE_VERTEX_TRIPLE_BYTES = 20 };

/* packed and component views share the same serialized bytes */
typedef struct
{
    union { uint32 xy[3]; XY point[3]; };
    union
    {
        struct { uint32 z01, z2; };
        struct { sint16 z[3]; uint16 retained; };
    };
} SCENE_VERTEX_TRIPLE;

typedef struct
{
    uint32 count;
    SCENE_VERTEX_TRIPLE group[SCENE_BLEND_VERTEX_CAPACITY / 3];
} SCENE_VERTEX_BLEND_PACKED;

typedef char scene_vertex_blend_packed_layout[
    offsetof(SCENE_VERTEX_BLEND_PACKED, group) == 4 &&
    sizeof(SCENE_VERTEX_BLEND_PACKED) == 624 ? 1 : -1];

typedef struct
{
    uint32 count;
    const SCENE_VERTEX_TRIPLE *first, *second;
} SCENE_VERTEX_BLEND_SOURCE;

SCENE_VERTEX_BLEND_SOURCE scene_geometry_blend_source(uint32 first, uint32 second, int native_second, const RESOURCE_ASSET_SOURCE *first_source, const RESOURCE_ASSET_SOURCE *second_source);
uint32 scene_geometry_blend_vertices(uint32 factor, SCENE_VERTEX_BLEND_SOURCE source, uint32 dst, const SCENE_VERTEX_BLEND_WORK *native_src1, SCENE_VERTEX_BLEND_WORK *native_out, SCENE_VERTEX_BLEND_PACKED *packed_out);

typedef char scene_vertex_component_layout[
    sizeof(SCENE_VERTEX_TRIPLE) == 20 && offsetof(SCENE_VERTEX_TRIPLE, point) == 0 &&
    offsetof(SCENE_VERTEX_TRIPLE, z) == 12 && offsetof(SCENE_VERTEX_TRIPLE, retained) == 18 ? 1 : -1];

sint32 scene_geometry_read_vertex_count(const uint8 *data);
SCENE_VERTEX_TRIPLE scene_geometry_read_vertex_triple(const uint8 *data);

typedef struct
{
    uint16 properties;
    sint16 radius;
} SCENE_MODEL_RADIUS_PREFIX;

typedef char scene_model_radius_prefix_layout[
    sizeof(SCENE_MODEL_RADIUS_PREFIX) == 4 && offsetof(SCENE_MODEL_RADIUS_PREFIX, radius) == 2 ? 1 : -1];

typedef struct SCENE_SPRITE_MODEL_HEADER
{
    uint16 properties;
    sint16 radius;
    uint32 vertices;
    uint32 normals;
    uint32 flat_quads;
    uint32 gouraud_quads;
} SCENE_SPRITE_MODEL_HEADER;

const SCENE_SPRITE_MODEL_HEADER *scene_sprite_model_header(RESOURCE_ASSET_SOURCE source, const SCENE_SPRITE_MODEL_HEADER *header, uint32 bytes);

typedef struct SCENE_MESH_MODEL_HEADER
{
    uint16 properties;
    sint16 radius;
    uint32 vertices[3];
    uint32 normals;
    uint32 triangles;
    uint32 quads;
} SCENE_MESH_MODEL_HEADER;

typedef struct SCENE_TERRAIN_MODEL_HEADER
{
    uint32 properties;
    uint32 vertices;
    uint32 auxiliary;
    uint32 flat_quads;
    uint32 gouraud_quads;
    uint16 reserved;
    sint16 height;
} SCENE_TERRAIN_MODEL_HEADER;

typedef struct
{
    uint32 flags;
    uint16 normal_offsets[2];
    uint32 vertex_offsets[2];
    uint32 projected_indices;
    uint32 uv[3];
} SCENE_QUAD_FACE_TEXTURE;

typedef struct
{
    SCENE_QUAD_FACE_TEXTURE texture;
    uint32 color;
} SCENE_FLAT_QUAD_FACE;

typedef struct
{
    SCENE_QUAD_FACE_TEXTURE texture;
    uint32 color[4];
} SCENE_GOURAUD_QUAD_FACE;

typedef struct
{
    XY screen;
    uint32 depth;
    uint32 intensity;
    uint32 reserved;
} SCENE_SPRITE_PROJECTION;

enum { SCENE_SPRITE_PROJECTION_CAPACITY = 33 };

typedef struct
{
    const SCENE_SPRITE_PROJECTION *vertices;
    uint32 count;
    uint8 legacy_outside;
    const uint8 *wire_data;
    uint32 wire_bytes;
} SCENE_SPRITE_PROJECTION_VIEW;

enum { SCENE_SPRITE_PROJECTION_OFFSET = 32 };
enum { SCENE_SUBDIV_SCRATCH_OFFSET = 0x230 };

typedef struct
{
    uint32 vertex_table;
    SCENE_GOURAUD_QUAD_FACE flat_polygon;
    uint32 depth;
    uint16 tpage, clut;
} SCENE_SUBDIV_SCRATCH_HEADER;

uint32 scene_mesh_model_vertices(const SCENE_MESH_MODEL_HEADER *model, uint32 level);

enum
{
    SCENE_MESH_VERTEX_CAPACITY = 165
};

typedef struct
{
    XY xy[SCENE_MESH_VERTEX_CAPACITY];
    uint16 depth[SCENE_MESH_VERTEX_CAPACITY];
} SCENE_MESH_PROJECTION;

#pragma pack(push, 1)
typedef struct
{
    uint32 colors[3];
    uint32 uv[3];
} SCENE_PRELIT_TRIANGLE_DATA;

typedef struct
{
    uint32 colors[4];
    uint32 uv[3];
} SCENE_PRELIT_QUAD_DATA;

typedef struct
{
    uint8 normal_indices[4];
    uint32 uv[3];
} SCENE_LIT_FACE_DATA;
#pragma pack(pop)

typedef char scene_mesh_face_data_layout_check[
    sizeof(SCENE_PRELIT_TRIANGLE_DATA) == 24 && offsetof(SCENE_PRELIT_TRIANGLE_DATA, uv) == 12 &&
    sizeof(SCENE_PRELIT_QUAD_DATA) == 28 && offsetof(SCENE_PRELIT_QUAD_DATA, uv) == 16 &&
    sizeof(SCENE_LIT_FACE_DATA) == 16 && offsetof(SCENE_LIT_FACE_DATA, uv) == 4 ? 1 : -1];

typedef struct
{
    uint32 level;
    SCENE_NORMAL_REFERENCE normal_table;
    uint32 *local_ot;
    uint16 ticks;
    uint16 depth_bias;
} SCENE_MESH_CONTEXT;

typedef struct
{
    uint32 ordering;
    uint32 packet;
    uint32 primitives;
} SCENE_RENDER_STREAM;

typedef struct
{
    SCENE_FACE_GROUP faces;
    RESOURCE_ASSET_SOURCE vertices;
} SCENE_SUBDIV_INPUT;

typedef uint32 (*SCENE_GEOMETRY_CALLBACK)(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream);
typedef void (*SCENE_GEOMETRY_COMPOSITE_CALLBACK)(const SCENE_MESH_CONTEXT *context, FLIGHT_CONTROL *effect, SCENE_RENDER_STREAM *stream);

extern SCENE_GEOMETRY_CALLBACK const scene_geometry_mesh_callbacks[6];
extern SCENE_GEOMETRY_COMPOSITE_CALLBACK const scene_geometry_composite_callbacks[6];

enum
{
    SCENE_GEOMETRY_ACTOR_LIMIT = 32,
    SCENE_GEOMETRY_EFFECT_LIMIT = 32,
    SCENE_GEOMETRY_DESCRIPTOR_CAPACITY = 2 * SCENE_GEOMETRY_ACTOR_LIMIT + SCENE_GEOMETRY_EFFECT_LIMIT + 1,
    SCENE_GEOMETRY_MESH_CAPACITY = SCENE_GEOMETRY_DESCRIPTOR_CAPACITY,
    SCENE_GEOMETRY_SPRITE_CAPACITY = SCENE_GEOMETRY_ACTOR_LIMIT + 1,
    SCENE_GEOMETRY_ANIMATED_CAPACITY = 2,
    SCENE_GEOMETRY_FIXED_CAPACITY = SCENE_GEOMETRY_ACTOR_LIMIT + 1,
    SCENE_GEOMETRY_MISSION_CAPACITY = SCENE_GEOMETRY_ACTOR_LIMIT + 1
};


typedef struct
{
    uint8 model_index;
    uint8 flags;
    uint16 parameter;
    uint8 kind;
    uint8 selected;
    uint8 style;
    uint8 variant;
    const NATIVE_POSE *native_pose;
} SCENE_GEOMETRY_DESCRIPTOR;

typedef struct
{
    uint8 flags;
    uint16 parameter;
    const SCENE_SPRITE_MODEL_HEADER *const *lod_model_headers;
    uint32 lod_header_count;
    RESOURCE_ASSET_SOURCE model_source;
    RESOURCE_ASSET_SOURCE vertex_source;
    SCENE_SPRITE_MODEL_HEADER blended_model;
    sint16 x;
    sint16 y;
    sint16 z;
    const SCENE_SPRITE_MODEL_HEADER *model_data;
} SCENE_GEOMETRY_SPRITE;

typedef union
{
    uint16 component[9];
    uint32 packed[4];
} SCENE_GEOMETRY_ROTATION;

typedef char SCENE_GEOMETRY_ROTATION_LAYOUT_CHECK[
    sizeof(SCENE_GEOMETRY_ROTATION) == 20 &&
    sizeof(((SCENE_GEOMETRY_ROTATION *)0)->component) == 18 &&
    sizeof(((SCENE_GEOMETRY_ROTATION *)0)->packed) == 16 ? 1 : -1];

typedef struct
{
    uint8 flags;
    uint16 parameter;
    const SCENE_MESH_MODEL_HEADER *const *lod_model_headers;
    const RESOURCE_ASSET_SOURCE *lod_model_sources;
    uint32 lod_header_count;
    uint32 level;
    uint32 vertices_address;
    RESOURCE_ASSET_SOURCE vertex_source;
    RESOURCE_ASSET_SOURCE model_source;
    const SCENE_MESH_MODEL_HEADER *model_data;
    SCENE_GEOMETRY_ROTATION rotation;
    GAME_FIXED_VECTOR translation;
    SCENE_GEOMETRY_CALLBACK callback;
} SCENE_GEOMETRY_MESH;

typedef struct
{
    uint32 level;
    uint32 vertices_address[5];
    RESOURCE_ASSET_SOURCE vertex_sources[5];
    RESOURCE_ASSET_SOURCE model_source;
    const SCENE_MESH_MODEL_HEADER *models_data[5];
    uint32 blend_vertices_address[10];
    uint32 depth;
    SCENE_GEOMETRY_ROTATION rotation;
    GAME_FIXED_VECTOR translation;
    SCENE_GEOMETRY_ROTATION light_rotation;
    FLIGHT_CONTROL *effect;
    SCENE_GEOMETRY_COMPOSITE_CALLBACK callback;
} SCENE_GEOMETRY_COMPOSITE;

typedef struct
{
    SCENE_GEOMETRY_COMPOSITE *records;
    uint32 capacity;
} SCENE_GEOMETRY_COMPOSITE_BUFFER;

typedef struct
{
    SCENE_GEOMETRY_DESCRIPTOR descriptors[SCENE_GEOMETRY_DESCRIPTOR_CAPACITY];
    SCENE_GEOMETRY_MESH meshes[SCENE_GEOMETRY_MESH_CAPACITY];
    SCENE_GEOMETRY_SPRITE sprites[SCENE_GEOMETRY_SPRITE_CAPACITY];
    SCENE_GEOMETRY_COMPOSITE animated[SCENE_GEOMETRY_ANIMATED_CAPACITY];
    SCENE_GEOMETRY_COMPOSITE fixed[SCENE_GEOMETRY_FIXED_CAPACITY];
} SCENE_GEOMETRY_WORKSPACE;

typedef struct
{
    SCENE_GEOMETRY_DESCRIPTOR descriptors[SCENE_GEOMETRY_MISSION_CAPACITY];
    SCENE_GEOMETRY_MESH meshes[SCENE_GEOMETRY_MISSION_CAPACITY];
    SCENE_GEOMETRY_SPRITE sprites[1];
    SCENE_GEOMETRY_COMPOSITE animated[SCENE_GEOMETRY_ANIMATED_CAPACITY];
    SCENE_GEOMETRY_COMPOSITE fixed[SCENE_GEOMETRY_FIXED_CAPACITY];
} SCENE_GEOMETRY_MISSION_WORKSPACE;

SCENE_GEOMETRY_COMPOSITE *scene_geometry_composite_record(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index);
uint32 scene_geometry_composite_level(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index);
void scene_geometry_end_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *buffer, uint32 index);
void scene_geometry_interpolate_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *entries, SCENE_RENDER_STREAM *stream);
void scene_geometry_draw_composites(const SCENE_GEOMETRY_COMPOSITE_BUFFER *entries, uint16 ticks, SCENE_RENDER_STREAM *stream);
uint32 scene_geometry_blend_axes(sint32 count, sint32 mode, const uint32 source[5], uint32 dst, SCENE_VERTEX_BLEND_WORK *workspace, const RESOURCE_ASSET_SOURCE *parent);
uint32 scene_geometry_blend_pair(sint32 count, const uint32 source[2], uint32 dst, const RESOURCE_ASSET_SOURCE *parent);
uint32 scene_geometry_blend_signed_axis(sint32 count, const uint32 source[3], uint32 dst, const RESOURCE_ASSET_SOURCE *parent);

void scene_geometry_interpolate_meshes(SCENE_GEOMETRY_MESH *entries, SCENE_RENDER_STREAM *stream);
void scene_geometry_draw_meshes(const SCENE_GEOMETRY_MESH *entries, SCENE_RENDER_STREAM *stream);

void scene_geometry_interpolate_sprites(SCENE_GEOMETRY_SPRITE *entries, SCENE_RENDER_STREAM *stream);
void scene_geometry_draw_sprites(const SCENE_GEOMETRY_SPRITE *entries, SCENE_RENDER_STREAM *stream);

uint32 scene_geometry_collect_primary(SCENE_GEOMETRY_DESCRIPTOR *entries);
void scene_geometry_collect_secondary(SCENE_GEOMETRY_DESCRIPTOR *entries);
void scene_geometry_collect_mission_primary(SCENE_GEOMETRY_DESCRIPTOR *entries);

uint32 scene_geometry_draw_mesh_style_1(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream);
uint32 scene_geometry_draw_mesh_style_2(const SCENE_MESH_CONTEXT *state, SCENE_RENDER_STREAM *stream);
uint32 scene_geometry_draw_mesh_style_3(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream);
uint32 scene_geometry_draw_mesh_style_4(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream);
uint32 scene_geometry_draw_mesh_style_5(const SCENE_MESH_CONTEXT *object, SCENE_RENDER_STREAM *stream);
void scene_geometry_draw_composite_variant_1(const SCENE_MESH_CONTEXT *state, FLIGHT_CONTROL *record, SCENE_RENDER_STREAM *stream);
void scene_geometry_draw_composite_variant_2(const SCENE_MESH_CONTEXT *state, FLIGHT_CONTROL *effect, SCENE_RENDER_STREAM *stream);



uint32 scene_draw_clip_mask(uint32 ordering, uint32 packet, const PSX_RECT *rectangle, const XY *offset);

void scene_geometry_draw_detail_0(EFFECT_FRAME_CONTEXT *frame);
void scene_geometry_draw_detail_1(EFFECT_FRAME_CONTEXT *frame);
void scene_geometry_draw_detail_2(EFFECT_FRAME_CONTEXT *frame);
void scene_geometry_draw_detail_3(EFFECT_FRAME_CONTEXT *frame);
void scene_geometry_draw_detail_4(EFFECT_FRAME_CONTEXT *frame);

uint32 scene_geometry_emit_bucket_copies(uint32 ordering, uint32 packet);

void scene_geometry_build_plane_view(const uint32 position[3], const uint16 angles[3], const uint16 viewport_size[2], SCENE_TERRAIN_VIEW *view);


enum
{
    SCENE_LIGHT_POLYGON_CAPACITY = 12,
    SCENE_LIGHT_POLYGON_COLOR_OFFSET = 8
};

void scene_terrain_change_tile(uint32 kind, uint32 index, sint32 value);

uint32 scene_terrain_polygon_has_consistent_winding(const uint32 points[][4], uint32 count);

void scene_terrain_draw_tiles(const SCENE_TERRAIN_TILE *records, uint32 tile_count, const TERRAIN_MODEL_TABLE *models, const uint32 origin[3], uint32 *previous, uint32 *current, SCENE_RENDER_STREAM *stream);
uint32 scene_subdiv_draw_flat_quad(SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input, const SCENE_SPRITE_PROJECTION_VIEW *projection);
void scene_subdiv_draw_gouraud_quad(SCENE_SUBDIV_SCRATCH_HEADER *scratch, SCENE_RENDER_STREAM *stream, const SCENE_SUBDIV_INPUT *input, const SCENE_SPRITE_PROJECTION_VIEW *projection);


void scene_geometry_place_tile_view(SCENE_TERRAIN_VIEW *view);
uint32 scene_terrain_collect_tiles(const uint16 *tiles, SCENE_TERRAIN_TILE *records, const SCENE_TERRAIN_VIEW *view);
uint32 scene_geometry_clip_line(uint16 *first, uint16 *second, uint32 width, uint32 height);

void scene_geometry_render_mission_terrain(void);
void scene_geometry_build_view_row_mask(SCENE_TERRAIN_VIEW *view);
uint32 scene_geometry_draw_terrain_mode_two(const SCENE_TERRAIN_VIEW *view, uint32 ordering, uint32 packet);
void scene_geometry_intersect_plane_view(const uint16 viewport_size[2], const uint32 position[3], const uint16 angles[3], SCENE_TERRAIN_VIEW *view);

void scene_geometry_enable_bucket_copies(uint32 enabled);

uint32 scene_terrain_quad_exceeds_span(uint32 first, uint32 second, uint32 third, uint32 fourth);

uint32 scene_geometry_order_plane_boundary(const uint32 points[4][4], uint32 minimum, uint32 maximum, uint32 ordered[4]);
void scene_geometry_project_short_vector(const uint16 vector[3], uint16 output[2]);
void scene_geometry_transform_short_vector(const uint16 vector[3], uint32 output[3]);

void scene_geometry_draw_mission_models(void);
uint32 scene_geometry_draw_mode_two_pass(const SCENE_TERRAIN_VIEW *view, uint32 ordering, uint32 packet);

void scene_geometry_emit_mesh_triangles(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);
void scene_geometry_emit_mesh_quads(const SCENE_MESH_CONTEXT *context, SCENE_RENDER_STREAM *stream, const SCENE_MESH_PROJECTION *projected, const RESOURCE_ASSET_SOURCE *face_source);

void scene_geometry_set_depth_cue(uint32 near_value, uint32 far_value, uint32 height);
void scene_build_depth_buckets(uint32 near_value, uint32 far_value, uint16 *buckets);

#endif
