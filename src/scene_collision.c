#include <stddef.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    XY plane;
    sint16 depth;
    uint16 retained;
} COLLISION_PROJECTED_VERTEX;

typedef char COLLISION_PROJECTED_LAYOUT_CHECK[sizeof(COLLISION_PROJECTED_VERTEX) == 8 && offsetof(COLLISION_PROJECTED_VERTEX, depth) == 4 && offsetof(COLLISION_PROJECTED_VERTEX, retained) == 6 ? 1 : -1];

typedef struct
{
    COLLISION_PROJECTED_VERTEX *vertices;
    uint32 count;
    const uint8 *wire;
    uint32 bytes;
} COLLISION_PROJECTION;

static COLLISION_PROJECTION collision_projection_view(uint32 offset)
{
    COLLISION_PROJECTION projection = {0};
    if (offset < sizeof(SCRATCHPAD) && (offset & 3) == 0)
    {
        uint8 *storage = (uint8 *)getScratchAddr(0);
        projection.count = (sizeof(SCRATCHPAD) - offset) / sizeof(*projection.vertices);
        projection.vertices = (COLLISION_PROJECTED_VERTEX *)(storage + offset);
        if (projection.count != 0)
        {
            projection.wire = storage + offset;
            projection.bytes = sizeof(SCRATCHPAD) - offset;
        }
    }
    return projection;
}

static uint32 collision_projected_plane(const COLLISION_PROJECTION *projection, uint32 index)
{
    if (index < projection->count)
        return projection->vertices[index].plane.xy;
    uint32 offset = index * sizeof(COLLISION_PROJECTED_VERTEX);
    uint32 plane;
    if (projection->wire && offset <= projection->bytes && sizeof(plane) <= projection->bytes - offset)
    {
        memcpy(&plane, projection->wire + offset, sizeof(plane));
        return plane;
    }
    /* reads past the complete scratch span lie in the SDK unmapped gap */
    return 0;
}

static void collision_project_vertex(COLLISION_PROJECTION *projection, uint32 index, uint32 x, uint32 y, uint32 z)
{
    if (index < projection->count)
    {
        COLLISION_PROJECTED_VERTEX *vertex = projection->vertices + index;
        vertex->plane.x = (sint16)x;
        vertex->plane.y = (sint16)y;
        vertex->depth = (sint16)z;
    }
    else
        abort();
}

static int collision_ray_clip(const COLLISION_PROJECTION *projection, uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 depth, int quad, int reverse, uint32 *distance)
{
    uint32 x, y, z, numerator, area;
    if (first < projection->count)
    {
        const COLLISION_PROJECTED_VERTEX *vertex = projection->vertices + first;
        x = vertex->plane.x;
        y = vertex->plane.y;
        z = vertex->depth;
    }
    else
    {
        sint16 coordinates[3];
        uint32 lane, offset;
        for (lane = 0; lane < 3; ++lane)
        {
            offset = first * sizeof(COLLISION_PROJECTED_VERTEX) + lane * sizeof(sint16);
            if (projection->wire && offset <= projection->bytes && sizeof(coordinates[lane]) <= projection->bytes - offset)
                memcpy(&coordinates[lane], projection->wire + offset, sizeof(coordinates[lane]));
            else
                coordinates[lane] = 0;
        }
        x = coordinates[0];
        y = coordinates[1];
        z = coordinates[2];
    }
    xport_gte_write_control(8, ((0 - x) & 0xFFFF) | ((0 - y) << 16));
    xport_gte_write_control(9, 0 - z);
    xport_gte_execute(0x4A6412);
    xport_gte_write_data(12, collision_projected_plane(projection, first));
    xport_gte_write_data(13, collision_projected_plane(projection, second));
    numerator = 0 - (xport_gte_read_data(25) << 12);
    *distance = ((sint64)(sint32)numerator / (sint32)depth);
    xport_gte_execute(0x1400006);
    area = xport_gte_read_data(24);
    xport_gte_write_data(12, collision_projected_plane(projection, quad ? fourth : third));
    xport_gte_execute(0x1400006);
    if (reverse ? (sint32)area > 0 : (sint32)area < 0)
        return 0;
    area = xport_gte_read_data(24);
    xport_gte_write_data(13, collision_projected_plane(projection, quad ? third : first));
    xport_gte_execute(0x1400006);
    if (reverse ? (sint32)area < 0 : (sint32)area > 0)
        return 0;
    area = xport_gte_read_data(24);
    if (quad)
    {
        xport_gte_write_data(12, collision_projected_plane(projection, first));
        xport_gte_execute(0x1400006);
        if ((sint32)area < 0)
            return 0;
        return (sint32)xport_gte_read_data(24) <= 0;
    }
    return reverse ? (sint32)area <= 0 : (sint32)area >= 0;
}

enum
{
    COLLISION_TILE_CAPACITY = 18 * 18
};

typedef struct
{
    uint16 offset[3];
    uint32 flags;
} COLLISION_TILE;

static uint32 collision_collect_tiles(const uint16 *map, const uint32 *start, const uint32 *end, COLLISION_TILE *tiles);
static void collision_transform_vertices(RESOURCE_ASSET_SOURCE source, COLLISION_PROJECTION *projection, const uint16 offset[3], const SCENE_VERTEX_BLEND_PACKED *blend);

const uint16 scene_bounds_normals[6][3] = {
    {4096, 0, 0}, {0xF000, 0, 0}, {0, 4096, 0}, {0, 0xF000, 0}, {0, 0, 4096}, {0, 0, 0xF000},
};

static void collision_capture_normal(SCENE_NORMAL_REFERENCE source)
{
    uint16 wire_halfword_0;
    uint32 i;
    const uint8 *data = scene_normal_data(source, 6);
    for (i = 0; i < 3; ++i)
        game_runtime.collision_result.normal[i] = (memcpy(&wire_halfword_0, data + i * 2, sizeof(wire_halfword_0)), wire_halfword_0);
}

typedef struct
{
    uint32 flags;
    uint16 normal_offset[2];
    uint32 vertex_offsets[2];
    uint32 projected_indices;
} COLLISION_FACE_HEADER;

typedef char collision_face_header_layout_check[sizeof(COLLISION_FACE_HEADER) == 20 && offsetof(COLLISION_FACE_HEADER, normal_offset) == 4 && offsetof(COLLISION_FACE_HEADER, projected_indices) == 16 ? 1 : -1];

static uint32 collision_trace_models(uint32 projection_offset, const TERRAIN_MODEL_TABLE *models, const uint32 *dir, uint32 *hit, const COLLISION_TILE *tiles, uint32 tile_count)
{
    GAME_SCENE_NORMAL_DATA normals;
    SCENE_NORMAL_REFERENCE source_normal, initial_normal, chosen_normal;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    COLLISION_PROJECTION projection;
    SVECTOR vector = {0};
    uint16 normal[3];
    MATRIX matrix = {0}, basis = {0};
    uint32 index, column, a, b, c, maximum, best = 0x7FFFFFFF;
    uint32 descriptor, flags, kind, count, polygon;
    uint32 face, face_count, row_flags, hit_flags, packed, depth, distance;
    uint32 first, second, third, fourth, chosen_flags;
    int quad, clipped;
    FUNCTION_MARKER(0x80099894, "ACE3.BIN");
    projection = collision_projection_view(projection_offset);
    vector.vx = ((sint16)dir[0]);
    vector.vy = ((sint16)dir[1]);
    vector.vz = ((sint16)dir[2]);
    a = VectorNormalSS(&vector, &vector);
    maximum = game_square_root_with_lzcs(a);
    normal[0] = (uint16)vector.vx;
    normal[1] = (uint16)vector.vy;
    normal[2] = (uint16)vector.vz;
    collision_build_basis(&matrix, normal);
    matrix.t[2] = (sint32)(0);
    matrix.t[1] = (sint32)(0);
    matrix.t[0] = (sint32)(0);
    for (index = 0; index < 9; ++index)
        basis.m[index / 3][index % 3] = (sint16)(index % 4 == 0 ? 4096 : 0);
    index = 5;
    SetRotMatrix(&matrix);
    for (column = 0; column < 3; ++column)
    {
        a = ((uint16)basis.m[0][column]);
        b = ((uint16)basis.m[1][column]);
        c = ((uint16)basis.m[2][column]);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        basis.m[0][column] = (sint16)(a);
        basis.m[1][column] = (sint16)(b);
        basis.m[2][column] = (sint16)(c);
    }
    index = 3;
    SetTransMatrix(&matrix);
    b = ((uint16)basis.t[1]);
    a = ((uint16)basis.t[0]);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, ((uint32)basis.t[2]));
    xport_gte_execute(0x480012);
    basis.t[0] = (sint32)(xport_gte_read_data(25));
    basis.t[1] = (sint32)(xport_gte_read_data(26));
    basis.t[2] = (sint32)(xport_gte_read_data(27));
    index = 5;
    SetRotMatrix(&basis);
    index = 3;
    SetTransMatrix(&basis);
    xport_gte_write_data(14, 0);
    for (descriptor = 0; descriptor < tile_count; ++descriptor)
    {
        flags = tiles[descriptor].flags;
        if (flags & 0x200000)
            break;
        const SCENE_TERRAIN_MODEL_HEADER *header = terrain_model_header(models, flags & 0x1FF);
        RESOURCE_ASSET_SOURCE vertex_source = terrain_model_vertices(models, header);
        collision_transform_vertices(vertex_source, &projection, tiles[descriptor].offset, NULL);
        normals = game_runtime.scene_assets.normals;
        for (kind = 0; kind < 2; ++kind)
        {
            SCENE_FACE_GROUP faces = terrain_model_faces(models, header, kind);
            const uint8 *row = faces.records;
            count = faces.count;
            for (polygon = 0; (sint32)polygon < (sint32)count; ++polygon, row += faces.stride)
            {
                const COLLISION_FACE_HEADER *polygon_header = (const COLLISION_FACE_HEADER *)row;
                initial_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[0]};
                const uint8 *initial_data = scene_normal_data(initial_normal, 8);
                xport_gte_write_data(0, (memcpy(&wire_word_1, initial_data, sizeof(wire_word_1)), wire_word_1));
                xport_gte_write_data(1, (memcpy(&wire_word_2, initial_data + 4, sizeof(wire_word_2)), wire_word_2));
                row_flags = polygon_header->flags;
                face_count = (row_flags & 0x200) ? 2 : 1;
                for (face = 0; face < face_count; ++face)
                {
                    source_normal = initial_normal;
                    if (face != 0)
                    {
                        source_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[1]};
                        const uint8 *second_data = scene_normal_data(source_normal, 8);
                        xport_gte_write_data(0, (memcpy(&wire_word_3, second_data, sizeof(wire_word_3)), wire_word_3));
                        xport_gte_write_data(1, (memcpy(&wire_word_4, second_data + 4, sizeof(wire_word_4)), wire_word_4));
                    }
                    xport_gte_execute(0x486012);
                    packed = polygon_header->projected_indices;
                    a = xport_gte_read_data(9);
                    b = xport_gte_read_data(10);
                    depth = xport_gte_read_data(11);
                    xport_gte_write_data(1, depth);
                    xport_gte_write_data(0, (a & 0xFFFF) | (b << 16));
                    if ((sint32)depth >= 0)
                        continue;
                    first = (packed & 255);
                    second = ((packed >> 8) & 255);
                    third = ((packed >> 16) & 255);
                    fourth = (packed >> 24);
                    quad = (row_flags & 0x300) == 0;
                    /* Fix014 defines the second triangle base even when its first normal was rejected */
                    clipped = face != 0 ? collision_ray_clip(&projection, second, third, fourth, 0, depth, 0, 1, &distance) : collision_ray_clip(&projection, first, second, third, fourth, depth, quad, 0, &distance);
                    if (!clipped)
                        continue;
                    if (face == 0 && (row_flags & 0x200))
                        hit_flags = row_flags;
                    else if (face != 0)
                        hit_flags = polygon_header->flags;
                    else
                    {
                        if (kind == 0)
                        {
                            source_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[0]};
                            hit_flags = polygon_header->flags;
                        }
                        else
                        {
                            a = polygon_header->normal_offset[0];
                            hit_flags = polygon_header->flags;
                            source_normal = (SCENE_NORMAL_REFERENCE){normals, a};
                        }
                    }
                    if ((sint32)distance >= 0 && (sint32)maximum >= (sint32)distance && (sint32)distance < (sint32)best)
                    {
                        best = distance;
                        chosen_normal = source_normal;
                        chosen_flags = hit_flags;
                    }
                }
            }
        }
    }
    if ((sint32)best > 0x7FFF)
    {
        {
            hit[2] = 0;
            hit[1] = 0;
            hit[0] = 0;
        }
        game_runtime.collision_result.normal[0] = 0;
        game_runtime.collision_result.normal[1] = 0;
        game_runtime.collision_result.normal[2] = 0;
    }
    else
    {
        a = (uint32)(sint32)vector.vx * best;
        hit[0] = a;
        a = (uint32)(sint32)vector.vy * best;
        hit[1] = a;
        a = (uint32)(sint32)vector.vz * best;
        collision_capture_normal(chosen_normal);
        game_runtime.collision_result.flags = (uint32)(chosen_flags);
        hit[2] = a;
    }
    return best;
}

uint32 collision_query_tile(const uint32 *start, const uint32 *end, uint32 *hit)
{
    COLLISION_TILE tile_record;
    const uint16 *map;
    uint32 direction[3], x, z, gx, gz, sx, sz, tile, value, a, b, result;
    FUNCTION_MARKER(0x8009A5D0, "ACE3.BIN");
    x = (start[0]);
    map = game_runtime.scene_assets.tile_map;
    sx = ((sint32)x >> 12);
    value = (start[1]);
    direction[1] = ((sint32)value >> 12);
    z = (start[2]);
    sz = ((sint32)z >> 12);
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    value = (start[0]);
    direction[0] = ((sint32)value >> 12);
    value = (start[1]);
    direction[1] = ((sint32)value >> 12);
    value = (start[2]);
    direction[2] = ((sint32)value >> 12);
    value = (end[0]);
    value = (end[1]);
    value = (end[2]);
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint16)gx >= 256)
        gx = (gx & 7) | 248;
    if ((sint16)gz >= 256)
        gz = (gz & 7) | 248;
    tile = map[(uint16)gx + ((uint32)(uint16)gz << 8)] & 0x1FF;
    if (tile == 0)
    {
        value = (start[0]);
        hit[0] = value;
        value = (start[1]);
        hit[1] = value;
        value = (start[2]);
        hit[2] = value;
        return 0x7FFFFFFF;
    }
    tile_record.offset[0] = (uint16)(1024 - (sx & 0x7FF));
    value = (start[1]);
    tile_record.offset[2] = (uint16)(1024 - (sz & 0x7FF));
    tile_record.flags = tile;
    tile_record.offset[1] = (uint16)((sint32)(0 - value) >> 12);
    a = (end[0]);
    b = (start[0]);
    direction[0] = ((sint32)(a - b) >> 12);
    a = (end[1]);
    b = (start[1]);
    direction[1] = ((sint32)(a - b) >> 12);
    a = (end[2]);
    b = (start[2]);
    direction[2] = ((sint32)(a - b) >> 12);
    result = collision_trace_models(0, &game_runtime.scene_assets.terrain_models, direction, hit, &tile_record, 1);
    a = (hit[0]);
    b = (start[0]);
    hit[0] = a + b;
    a = (hit[1]);
    b = (start[1]);
    hit[1] = a + b;
    a = (hit[2]);
    b = (start[2]);
    hit[2] = a + b;
    return result;
}

uint32 collision_query_segment(const uint32 *start, const uint32 *end, uint32 *hit)
{
    const uint16 *map;
    uint32 first[3], second[3], value, a, b, result;
    COLLISION_TILE tiles[COLLISION_TILE_CAPACITY];
    uint32 tile_count;
    FUNCTION_MARKER(0x8009A800, "ACE3.BIN");
    value = (start[0]);
    map = game_runtime.scene_assets.tile_map;
    first[0] = ((sint32)value >> 12);
    value = (start[1]);
    first[1] = ((sint32)value >> 12);
    value = (start[2]);
    first[2] = ((sint32)value >> 12);
    value = (end[0]);
    second[0] = ((sint32)value >> 12);
    value = (end[1]);
    second[1] = ((sint32)value >> 12);
    value = (end[2]);
    second[2] = ((sint32)value >> 12);
    tile_count = collision_collect_tiles(map, first, second, tiles);
    second[0] -= first[0];
    second[1] -= first[1];
    second[2] -= first[2];
    result = collision_trace_models(0, &game_runtime.scene_assets.terrain_models, second, hit, tiles, tile_count);
    a = (hit[0]);
    b = (start[0]);
    hit[0] = a + b;
    a = (hit[1]);
    b = (start[1]);
    hit[1] = a + b;
    a = (hit[2]);
    b = (start[2]);
    hit[2] = a + b;
    return result;
}

static uint32 collision_collect_tiles(const uint16 *map, const uint32 *start, const uint32 *end, COLLISION_TILE *tiles)
{
    uint8(*side)[32] = (uint8(*)[32])getScratchAddr(0);
    sint16 delta[3];
    uint32 low[3], high[3], selected[32] = {0};
    uint32 a, b, value, index, result, row, column, dx, dz, ax, az, nx, nz, base_x, base_z;
    uint32 rem_x, rem_z, packed, x, z, cx, cz, current, neighbor, next, corner, different;
    uint32 grid_x, grid_z, gx, gz, tile, tile_count = 0, height;
    FUNCTION_MARKER(0x80098DBC, "ACE3.BIN");
    a = (start[1]);
    if ((sint32)a < -3000)
    {
        b = (end[1]);
        if ((sint32)b < -3000)
            return 0;
    }
    a = (end[0] & 0xFFFF);
    b = (start[0] & 0xFFFF);
    delta[0] = (a - b);
    a = (end[1] & 0xFFFF);
    b = (start[1] & 0xFFFF);
    delta[1] = (a - b);
    a = (end[2] & 0xFFFF);
    b = (start[2] & 0xFFFF);
    delta[2] = (a - b);
    a = (start[1]);
    if ((sint32)a >= -2999 && delta[1] < 0)
    {
        for (index = 0; index < 3; ++index)
            low[index] = (start[index]);
        result = collision_intersect_height((uint32)-3000, start, delta, high);
    }
    else
    {
        result = collision_intersect_height((uint32)-3000, start, delta, low);
        if (result != 0)
        {
            for (index = 0; index < 3; ++index)
            {
                value = (start[index]);
                low[index] += value;
            }
        }
        else
            for (index = 0; index < 3; ++index)
                low[index] = (start[index]);
        result = collision_intersect_height(0, start, delta, high);
    }
    if (result != 0)
    {
        for (index = 0; index < 3; ++index)
        {
            value = (start[index]);
            high[index] += value;
        }
    }
    else
        for (index = 0; index < 3; ++index)
            high[index] = (end[index]);
    dx = (sint16)(high[0] - low[0]);
    dz = (sint16)(high[2] - low[2]);
    delta[0] = dx;
    delta[1] = (high[1] - low[1]);
    delta[2] = dz;
    ax = (sint32)dx < 0 ? 0 - dx : dx;
    az = (sint32)dz < 0 ? 0 - dz : dz;
    rem_x = low[0] & 0x7FF;
    rem_z = low[2] & 0x7FF;
    nx = ax / 2048 + 2;
    nz = az / 2048 + 2;
    if ((sint32)dx < 0)
    {
        base_x = (sint16)(dx & 0xF800);
        --nx;
    }
    else if (rem_x == 0)
        base_x = -2048;
    else
    {
        base_x = 0;
        --nx;
    }
    if ((sint32)dz < 0)
    {
        base_z = (sint16)(dz & 0xF800);
        --nz;
    }
    else if (rem_z == 0)
        base_z = -2048;
    else
    {
        base_z = 0;
        --nz;
    }
    xport_gte_write_data(12, rem_z | (rem_x << 16));
    packed = ((rem_z + dz) & 0xFFFF) | ((rem_x + dx) << 16);
    xport_gte_write_data(13, packed);
    z = base_z;
    for (row = 0; (sint32)row <= (sint32)(nz + 1); ++row, z += 2048)
    {
        cz = (sint32)z > 32767 ? 0x7FFF : ((sint32)z < -32768 ? 0x8000 : z & 0xFFFF);
        x = base_x;
        for (column = 0; (sint32)column <= (sint32)(nx + 1); ++column, x += 2048)
        {
            cx = (sint32)x > 32767 ? 0x7FFF0000 : ((sint32)x < -32768 ? 0x80000000 : x << 16);
            xport_gte_write_data(14, cz | cx);
            xport_gte_execute(0x1400006);
            value = xport_gte_read_data(24);
            value = (sint32)value > 0 ? 1 : ((sint32)value < 0 ? 255 : 0);
            side[column][row] = value;
        }
    }
    for (row = 0; (sint32)row <= (sint32)nz; ++row)
    {
        for (column = 0; (sint32)column <= (sint32)nx; ++column)
        {
            current = side[column][row];
            neighbor = side[column][row + 1];
            different = current != neighbor;
            if (different == 0)
            {
                next = side[column + 1][row];
                different = current != next;
                if (different == 0)
                {
                    corner = side[column + 1][row + 1];
                    different = current != corner || (current & neighbor & next & corner) == 0;
                }
            }
            if (different != 0)
                selected[row] |= 1u << (column & 31);
        }
    }
    grid_x = (uint32)((sint32)low[0] >> 11) + 128;
    grid_z = (uint32)((sint32)low[2] >> 11) + 128;
    value = (start[1]);
    height = 0 - value;
    z = base_z;
    for (row = 0; (sint32)row <= (sint32)nz; ++row, z += 2048)
    {
        x = base_x;
        for (column = 0; (sint32)column <= (sint32)nx; ++column, x += 2048)
        {
            if ((selected[row] & (1u << (column & 31))) == 0)
                continue;
            a = (sint32)x < 0 ? x + 2047 : x;
            gx = grid_x + (uint32)((sint32)a >> 11);
            a = (sint32)z < 0 ? z + 2047 : z;
            gz = grid_z + (uint32)((sint32)a >> 11);
            if ((sint32)(gx << 16) < 0)
                gx &= 7;
            if ((sint32)(gz << 16) < 0)
                gz &= 7;
            if ((sint16)gx >= 256)
                gx = (gx & 7) | 248;
            if ((sint16)gz >= 256)
                gz = (gz & 7) | 248;
            tile = map[(uint16)gx + ((uint32)(uint16)gz << 8)] & 0x1FF;
            if (tile == 0)
                continue;
            a = (start[0] & 0xFFFF);
            value = x - rem_x + (low[0] & 0xFFFF) + 1024 - a;
            tiles[tile_count].offset[0] = (uint16)value;
            b = (start[2] & 0xFFFF);
            tiles[tile_count].offset[1] = (uint16)height;
            tiles[tile_count].flags = tile;
            value = z - rem_z + (low[2] & 0xFFFF) + 1024 - b;
            tiles[tile_count].offset[2] = (uint16)value;
            ++tile_count;
        }
    }
    return tile_count;
}

uint32 collision_intersect_height(uint32 plane, const uint32 *start, const sint16 *delta, uint32 *hit)
{
    uint32 height, dx, dy, dz, difference, numerator, x, z, value;
    FUNCTION_MARKER(0x80098C68, "ACE3.BIN");
    height = (start[1]);
    dx = (delta[0]);
    dy = (delta[1]);
    dz = (delta[2]);
    difference = plane - height;
    if ((sint32)dy > 0)
    {
        if ((sint32)plane < (sint32)height)
            return 0;
    }
    else if ((sint32)dy < 0)
    {
        if ((sint32)height < (sint32)plane)
            return 0;
    }
    else
    {
        if (plane != height)
            return 0;
        hit[0] = dx;
        hit[2] = dz;
        hit[1] = difference;
        goto clamp_output;
    }
    numerator = dx * difference;
    if (dy == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    x = ((sint64)(sint32)numerator / (sint32)dy);
    numerator = dz * difference;
    if (dy == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    z = ((sint64)(sint32)numerator / (sint32)dy);
    hit[1] = difference;
    hit[0] = x;
    hit[2] = z;
clamp_output:
    value = (hit[0]);
    if (((sint32)dx < 0 && (sint32)value < (sint32)dx) || ((sint32)dx >= 0 && (sint32)dx < (sint32)value))
    {
        hit[0] = dx;
        hit[1] = dy;
    }
    value = (hit[2]);
    if (((sint32)dz < 0 && (sint32)value < (sint32)dz) || ((sint32)dz >= 0 && (sint32)dz < (sint32)value))
    {
        hit[2] = dz;
        hit[1] = dy;
    }
    value = (hit[1]);
    return value != dy;
}

typedef char collision_vertex_wire_layout_check[sizeof(SCENE_VERTEX_TRIPLE) == 20 && offsetof(SCENE_VERTEX_TRIPLE, xy) == 0 && offsetof(SCENE_VERTEX_TRIPLE, z01) == 12 && offsetof(SCENE_VERTEX_TRIPLE, z2) == 16 ? 1 : -1];

static void collision_transform_vertices(RESOURCE_ASSET_SOURCE source, COLLISION_PROJECTION *projection, const uint16 offset[3], const SCENE_VERTEX_BLEND_PACKED *blend)
{
    uint32 wire_word_1;
    uint32 vertex = 0, dx, dy, dz, count, x, y, z, a, b, c, cursor, depth;
    FUNCTION_MARKER(0x80099750, "ACE3.BIN");
    if (!blend && (!source.data || source.bytes < 22))
        abort();
    cursor = 4;
    dx = (sint16)offset[0];
    const SCENE_VERTEX_TRIPLE *wire = blend ? NULL : (const SCENE_VERTEX_TRIPLE *)(source.data + cursor);
    x = blend ? blend->group[(0) / 3].point[(0) % 3].x : wire->point[0].x;
    count = blend ? blend->count : (memcpy(&wire_word_1, source.data, sizeof(wire_word_1)), wire_word_1);
    dy = (sint16)offset[1];
    y = blend ? blend->group[(0) / 3].point[(0) % 3].y : wire->point[0].y;
    dz = (sint16)offset[2];
    z = blend ? blend->group[(0) / 3].z[(0) % 3] : wire->z[0];
    xport_gte_write_data(0, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
    depth = z + dz;
    xport_gte_write_data(1, depth);
    do
    {
        xport_gte_execute(0x486012);
        x = blend ? blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].x : wire->point[1].x;
        y = blend ? blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].y : wire->point[1].y;
        z = blend ? blend->group[(vertex + 1) / 3].z[(vertex + 1) % 3] : wire->z[1];
        xport_gte_write_data(2, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(3, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        collision_project_vertex(projection, vertex++, a, b, c);
        xport_gte_execute(0x48E012);
        x = blend ? blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].x : wire->point[2].x;
        y = blend ? blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].y : wire->point[2].y;
        z = blend ? blend->group[(vertex + 1) / 3].z[(vertex + 1) % 3] : wire->z[2];
        xport_gte_write_data(4, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(5, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        collision_project_vertex(projection, vertex++, a, b, c);
        xport_gte_execute(0x496012);
        cursor += 20;
        count -= 3;
        if (blend && (sint32)count <= 0)
        {
            /* the original terminal load reads the first two projected records */
            x = projection->vertices[0].plane.x;
            y = projection->vertices[0].plane.y;
            z = projection->vertices[1].depth;
        }
        else if (blend)
        {
            x = blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].x;
            y = blend->group[(vertex + 1) / 3].point[(vertex + 1) % 3].y;
            z = blend->group[(vertex + 1) / 3].z[(vertex + 1) % 3];
        }
        else
        {
            uint32 read_bytes = (sint32)count > 0 ? 18 : 14;
            if (cursor > source.bytes || read_bytes > source.bytes - cursor)
                abort();
            wire = (const SCENE_VERTEX_TRIPLE *)(source.data + cursor);
            x = wire->point[0].x;
            y = wire->point[0].y;
            z = wire->z[0];
        }
        xport_gte_write_data(0, ((x + dx) & 0xFFFF) | ((y + dy) << 16));
        depth = z + dz;
        xport_gte_write_data(1, depth);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        collision_project_vertex(projection, vertex++, a, b, c);
    } while ((sint32)count > 0);
    {
        (void)(uint32)(depth);
        return;
    }
}

static uint32 collision_trace_model(uint32 projection_offset, const SCENE_SPRITE_MODEL_HEADER *header, uint32 vertices, const uint32 *velocity, uint32 *hit, const uint16 *relative, const SCENE_VERTEX_BLEND_PACKED *blend, const RESOURCE_ASSET_SOURCE *model_source)
{
    GAME_SCENE_NORMAL_DATA normals;
    SCENE_NORMAL_REFERENCE source_normal, initial_normal, chosen_normal;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    COLLISION_PROJECTION projection;
    SVECTOR vector = {0};
    uint16 normal[3];
    MATRIX matrix = {0}, basis = {0};
    uint16 offset[3];
    uint32 index, column, a, b, c, maximum, best = 0x7FFFFFFF;
    uint32 list, kind, count, polygon;
    uint32 face, face_count, row_flags, hit_flags, packed, depth, distance;
    uint32 first, second, third, fourth, chosen_flags;
    int quad, clipped;
    FUNCTION_MARKER(0x8009A944, "ACE3.BIN");
    projection = collision_projection_view(projection_offset);
    vector.vx = ((sint16)velocity[0]);
    vector.vy = ((sint16)velocity[1]);
    vector.vz = ((sint16)velocity[2]);
    a = VectorNormalSS(&vector, &vector);
    maximum = game_square_root_with_lzcs(a);
    normal[0] = (uint16)vector.vx;
    normal[1] = (uint16)vector.vy;
    normal[2] = (uint16)vector.vz;
    collision_build_basis(&matrix, normal);
    matrix.t[2] = (sint32)(0);
    matrix.t[1] = (sint32)(0);
    matrix.t[0] = (sint32)(0);
    for (index = 0; index < 9; ++index)
        basis.m[index / 3][index % 3] = (sint16)(index % 4 == 0 ? 4096 : 0);
    index = 5;
    SetRotMatrix(&matrix);
    for (column = 0; column < 3; ++column)
    {
        a = ((uint16)basis.m[0][column]);
        b = ((uint16)basis.m[1][column]);
        c = ((uint16)basis.m[2][column]);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        basis.m[0][column] = (sint16)(a);
        basis.m[1][column] = (sint16)(b);
        basis.m[2][column] = (sint16)(c);
    }
    index = 3;
    SetTransMatrix(&matrix);
    b = ((uint16)basis.t[1]);
    a = ((uint16)basis.t[0]);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, ((uint32)basis.t[2]));
    xport_gte_execute(0x480012);
    basis.t[0] = (sint32)(xport_gte_read_data(25));
    basis.t[1] = (sint32)(xport_gte_read_data(26));
    basis.t[2] = (sint32)(xport_gte_read_data(27));
    index = 5;
    SetRotMatrix(&basis);
    index = 3;
    SetTransMatrix(&basis);
    xport_gte_write_data(14, 0);
    offset[0] = relative[0];
    offset[1] = relative[1];
    offset[2] = relative[2];
    RESOURCE_ASSET_SOURCE vertex_source = {0};
    if (!blend)
    {
        uint32 input_offset = model_source ? vertices - model_source->encoded_base : 0;
        const uint8 *input = model_source && model_source->data && vertices >= model_source->encoded_base && input_offset <= model_source->bytes && sizeof(uint32) <= model_source->bytes - input_offset ? model_source->data + input_offset : NULL;
        const uint8 *first = input ? input : mission_packet_span(vertices, sizeof(uint32));
        uint32 count, remaining, groups = 1, bytes;
        memcpy(&count, first, sizeof(count));
        remaining = count - 3;
        if ((sint32)remaining > 0)
            groups += remaining / 3 + (remaining % 3 != 0);
        if (groups > projection.count / 3 || groups > (UINT32_MAX - 18) / SCENE_VERTEX_TRIPLE_BYTES)
            abort();
        bytes = 18 + groups * SCENE_VERTEX_TRIPLE_BYTES;
        vertex_source = (RESOURCE_ASSET_SOURCE){input && bytes <= model_source->bytes - input_offset ? input : mission_packet_span(vertices, bytes), bytes, vertices};
        if (vertex_source.data != first || ((uintptr_t)(first + 4) & 3))
            abort();
    }
    collision_transform_vertices(vertex_source, &projection, offset, blend);
    list = header->gouraud_quads;
    normals = game_runtime.scene_assets.normals;
    for (kind = 0; kind < 2; ++kind)
    {
        if (kind != 0)
            list = header->flat_quads;
        SCENE_FACE_GROUP group = scene_model_face_group(model_source, list, kind == 0 ? 48 : 36);
        count = group.count;
        for (polygon = 0; (sint32)polygon < (sint32)count; ++polygon)
        {
            const COLLISION_FACE_HEADER *polygon_header = (const COLLISION_FACE_HEADER *)(group.records + polygon * group.stride);
            initial_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[0]};
            const uint8 *initial_data = scene_normal_data(initial_normal, 8);
            xport_gte_write_data(0, (memcpy(&wire_word_1, initial_data, sizeof(wire_word_1)), wire_word_1));
            xport_gte_write_data(1, (memcpy(&wire_word_2, initial_data + 4, sizeof(wire_word_2)), wire_word_2));
            row_flags = polygon_header->flags;
            face_count = (row_flags & 0x200) ? 2 : 1;
            for (face = 0; face < face_count; ++face)
            {
                source_normal = initial_normal;
                if (face != 0)
                {
                    source_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[1]};
                    const uint8 *second_data = scene_normal_data(source_normal, 8);
                    xport_gte_write_data(0, (memcpy(&wire_word_3, second_data, sizeof(wire_word_3)), wire_word_3));
                    xport_gte_write_data(1, (memcpy(&wire_word_4, second_data + 4, sizeof(wire_word_4)), wire_word_4));
                }
                xport_gte_execute(0x486012);
                packed = polygon_header->projected_indices;
                a = xport_gte_read_data(9);
                b = xport_gte_read_data(10);
                depth = xport_gte_read_data(11);
                xport_gte_write_data(1, depth);
                xport_gte_write_data(0, (a & 0xFFFF) | (b << 16));
                if ((sint32)depth >= 0)
                    continue;
                first = (packed & 255);
                second = ((packed >> 8) & 255);
                third = ((packed >> 16) & 255);
                fourth = (packed >> 24);
                quad = (row_flags & 0x300) == 0;
                /* Fix015 defines the second triangle base even when its first normal was rejected */
                clipped = face != 0 ? collision_ray_clip(&projection, second, third, fourth, 0, depth, 0, 1, &distance) : collision_ray_clip(&projection, first, second, third, fourth, depth, quad, 0, &distance);
                if (!clipped)
                    continue;
                if (face == 0 && (row_flags & 0x200))
                    hit_flags = row_flags;
                else if (face != 0)
                    hit_flags = polygon_header->flags;
                else
                {
                    if (kind == 0)
                    {
                        source_normal = (SCENE_NORMAL_REFERENCE){normals, polygon_header->normal_offset[0]};
                        hit_flags = polygon_header->flags;
                    }
                    else
                    {
                        a = polygon_header->normal_offset[0];
                        hit_flags = polygon_header->flags;
                        source_normal = (SCENE_NORMAL_REFERENCE){normals, a};
                    }
                }
                if ((sint32)distance >= 0 && (sint32)maximum >= (sint32)distance && (sint32)distance < (sint32)best)
                {
                    best = distance;
                    chosen_normal = source_normal;
                    chosen_flags = hit_flags;
                }
            }
        }
    }
    if ((sint32)best > 0x7FFF)
    {
        game_runtime.collision_result.normal[0] = 0;
        game_runtime.collision_result.normal[1] = 0;
        game_runtime.collision_result.normal[2] = 0;
        {
            hit[2] = 0;
            hit[1] = 0;
            hit[0] = 0;
        }
    }
    else
    {
        a = (uint32)(sint32)vector.vx * best;
        hit[0] = a;
        a = (uint32)(sint32)vector.vy * best;
        hit[1] = a;
        a = (uint32)(sint32)vector.vz * best;
        collision_capture_normal(chosen_normal);
        game_runtime.collision_result.flags = (uint32)(chosen_flags);
        hit[2] = a;
    }
    return best;
}

void collision_build_basis(MATRIX *matrix, const uint16 *normal)
{
    uint16 local[9];
    uint32 x, y, z, length, result, numerator, first, second, a, b, c, column, index;
    FUNCTION_MARKER(0x80099428, "ACE3.BIN");
    for (index = 0; index < 9; ++index)
    {
        a = index % 4 == 0 ? 4096 : 0;
        matrix->m[index / 3][index % 3] = (sint16)(a);
    }
    x = ((sint16)normal[0]);
    z = ((sint16)normal[2]);
    length = game_vector_length_gte(x, 0, z);
    y = ((sint16)normal[1]);
    local[0] = 4096;
    local[1] = 0;
    local[2] = 0;
    local[3] = 0;
    local[4] = length;
    local[5] = (0 - y);
    local[6] = 0;
    local[7] = y;
    local[8] = length;
    result = 0 - y;
    {
        index = 5;
        SetRotMatrix(matrix);
    }
    for (column = 0; column < 3; ++column)
    {
        xport_gte_write_data(9, local[column]);
        xport_gte_write_data(10, local[column + 3]);
        xport_gte_write_data(11, local[column + 6]);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        {
            matrix->m[0][column] = (sint16)(a);
            matrix->m[1][column] = (sint16)(b);
            matrix->m[2][column] = (sint16)(c);
        }
    }
    if (length == 0)
    {
        (void)(uint32)(result);
        return;
    }
    numerator = (uint32)(((sint16)normal[0])) << 12;
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    first = ((sint64)(sint32)numerator / (sint32)length);
    numerator = (uint32)(((sint16)normal[2])) << 12;
    if (length == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    result = ((sint64)(sint32)numerator / (sint32)length);
    second = result;
    local[0] = second;
    local[1] = 0;
    local[2] = (0 - first);
    local[3] = 0;
    local[4] = 4096;
    local[5] = 0;
    local[6] = first;
    local[7] = 0;
    local[8] = second;
    {
        index = 5;
        SetRotMatrix(matrix);
    }
    for (column = 0; column < 3; ++column)
    {
        xport_gte_write_data(9, local[column]);
        xport_gte_write_data(10, local[column + 3]);
        xport_gte_write_data(11, local[column + 6]);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        {
            matrix->m[0][column] = (sint16)(a);
            matrix->m[1][column] = (sint16)(b);
            matrix->m[2][column] = (sint16)(c);
        }
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 collision_trace_actor(const MISSION_ACTOR *target, uint32 *hit, const uint32 *pos, const uint32 *vel)
{
    const MISSION_ACTOR_TRAITS *actor_traits = &target->traits;
    uint32 scaled[3];
    uint16 relative[3];
    SCENE_VERTEX_BLEND_PACKED *blend = (SCENE_VERTEX_BLEND_PACKED *)getScratchAddr(0);
    const SCENE_VERTEX_BLEND_PACKED *native_blend = NULL;
    const SCENE_SPRITE_MODEL_HEADER *const *models;
    const SCENE_SPRITE_MODEL_HEADER *model, *other;
    uint32 model_count;
    const SCENE_SPRITE_MODEL_HEADER *model_data;
    uint32 value, origin, selector, flags, factor, index, next, vertices, first, second, projection_offset, result;
    FUNCTION_MARKER(0x8009B4EC, "ACE3.BIN");
    value = (vel[0]);
    projection_offset = 0;
    scaled[0] = (sint32)value >> 12;
    value = (vel[1]);
    scaled[1] = (sint32)value >> 12;
    value = (vel[2]);
    scaled[2] = (sint32)value >> 12;
    value = target->pose.position[0];
    origin = (pos[0]);
    relative[0] = ((sint32)(value - origin) >> 12);
    value = target->pose.position[1];
    origin = (pos[1]);
    relative[1] = ((sint32)(value - origin) >> 12);
    value = target->pose.position[2];
    origin = (pos[2]);
    relative[2] = ((sint32)(value - origin) >> 12);
    selector = actor_traits->model;
    if (!game_runtime.scene_assets.animated_headers || selector >= game_runtime.scene_assets.animated_header_count)
        abort();
    models = game_runtime.scene_assets.animated_headers + selector;
    model_count = game_runtime.scene_assets.animated_header_count - selector;
    flags = actor_traits->render_flags;
    if ((flags & 4) != 0)
    {
        value = actor_traits->parameter;
        index = (sint32)(value << 16) >> 28;
        factor = value & 4095;
        if ((sint32)index < 0)
            index = 0;
        if ((sint32)index >= 5)
            index = 4;
        if (index >= model_count)
            abort();
        if (factor != 0)
        {
            next = index < 4 ? index + 1 : 4;
            if (next >= model_count)
                abort();
            vertices = 0;
            model = models[index];
            other = models[next];
            const SCENE_SPRITE_MODEL_HEADER *first_model = scene_sprite_model_header(game_runtime.scene_assets.animated_model_source, model, sizeof(*first_model));
            const SCENE_SPRITE_MODEL_HEADER *second_model = scene_sprite_model_header(game_runtime.scene_assets.animated_model_source, other, sizeof(*second_model));
            model_data = first_model;
            first = first_model->vertices;
            second = second_model->vertices;
            SCENE_VERTEX_BLEND_SOURCE blend_source = scene_geometry_blend_source(first, second, 0, &game_runtime.scene_assets.animated_model_source, &game_runtime.scene_assets.animated_model_source);
            uint32 vertex_count = blend_source.count;
            uint32 remaining = vertex_count - 3;
            uint32 groups = 1;
            if ((sint32)remaining > 0)
                groups += remaining / 3 + (remaining % 3 != 0);
            if (groups > 23 || groups > SCENE_BLEND_VERTEX_CAPACITY / 3)
                abort();
            projection_offset = scene_geometry_blend_vertices(factor, blend_source, 0, NULL, NULL, blend);
            native_blend = blend;
        }
        else
        {
            model = models[index];
            model_data = scene_sprite_model_header(game_runtime.scene_assets.animated_model_source, model, sizeof(*model_data));
            vertices = model_data->vertices;
        }
    }
    else
    {
        model = models[0];
        model_data = scene_sprite_model_header(game_runtime.scene_assets.animated_model_source, model, sizeof(*model_data));
        vertices = model_data->vertices;
    }
    result = collision_trace_model(projection_offset, model_data, vertices, scaled, hit, relative, native_blend, &game_runtime.scene_assets.animated_model_source);
    value = (hit[0]);
    origin = (pos[0]);
    value += origin;
    hit[0] = value;
    value = (hit[1]);
    origin = (pos[1]);
    value += origin;
    hit[1] = value;
    value = (hit[2]);
    origin = (pos[2]);
    value += origin;
    hit[2] = value;
    return result;
}

uint32 scene_query_ground(const uint32 *position, uint16 *native_normal, uint32 *native_surface)
{
    const uint16 *map;
    uint32 x, z, gx, gz, tile, first[3], second[3], hit[3], result, value, i;
    FUNCTION_MARKER(0x80079B1C, "ACE3.BIN");
    x = position[0];
    z = position[2];
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    map = game_runtime.scene_assets.tile_map;
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint32)gx >= 256)
        gx = (gx & 7) + 248;
    if ((sint32)gz >= 256)
        gz = (gz & 7) + 248;
    tile = map[gx + (gz << 8)] & 0x1FF;
    if (tile != 0)
    {
        first[0] = x;
        first[1] = 0xFF448000;
        first[2] = position[2];
        second[0] = position[0];
        second[1] = 0;
        second[2] = position[2];
        result = collision_query_tile(first, second, hit);
        if ((sint32)result <= 0x7FFFFFFE)
        {
            {
                for (i = 0; i < 3; ++i)
                {
                    value = game_runtime.collision_result.normal[i];
                    native_normal[i] = value;
                }
            }
            {
                value = game_runtime.collision_result.flags;
                *native_surface = value;
            }
            return hit[1];
        }
    }
    {
        {
            native_normal[0] = 0;
            native_normal[1] = 0xF000;
            native_normal[2] = 0;
        }
    }
    if (tile != 0)
    {
        *native_surface = 0;
    }
    return 0;
}

uint32 collision_query_kind(const uint32 *start, const uint32 *end, uint32 *out, uint32 workspace_offset, uint32 kind)
{
    COLLISION_TILE tile_record;
    uint32 direction[3], x, z, value, a, b, result;
    FUNCTION_MARKER(0x8009A484, "ACE3.BIN");
    if (kind == 0)
    {
        value = start[0];
        out[0] = value;
        value = start[1];
        out[1] = value;
        value = start[2];
        out[2] = value;
        return 0x7FFFFFFF;
    }
    x = start[0];
    z = start[2];
    x = (uint32)((sint32)x >> 12) & 0x7FF;
    z = (uint32)((sint32)z >> 12) & 0x7FF;
    tile_record.offset[0] = (uint16)(1024 - x);
    value = start[1];
    tile_record.offset[2] = (uint16)(1024 - z);
    tile_record.flags = kind;
    tile_record.offset[1] = (uint16)((sint32)(0 - value) >> 12);
    a = end[0];
    b = start[0];
    direction[0] = (sint32)(a - b) >> 12;
    a = end[1];
    b = start[1];
    direction[1] = (sint32)(a - b) >> 12;
    a = end[2];
    b = start[2];
    direction[2] = (sint32)(a - b) >> 12;
    result = collision_trace_models(workspace_offset, &game_runtime.scene_assets.terrain_models, direction, out, &tile_record, 1);
    a = out[0];
    b = start[0];
    out[0] = a + b;
    a = out[1];
    b = start[1];
    out[1] = a + b;
    a = out[2];
    b = start[2];
    out[2] = a + b;
    return result;
}
