#include <stddef.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint32 axis[3];
} EFFECT_TRAIL_POINT;

typedef char EFFECT_TRAIL_POINT_LAYOUT_CHECK[sizeof(EFFECT_TRAIL_POINT) == sizeof(uint32[3]) ? 1 : -1];

typedef struct
{
    EFFECT_TRAIL_POINT point;
    uint32 reserved;
} EFFECT_TRAIL_POINT_PUBLICATION;

typedef struct
{
    uint32 reserved, color, screen;
    uint32 edge[3];
    uint16 depth, scale;
} EFFECT_QUAD_TRAIL_PUBLICATION;

typedef struct
{
    uint32 reserved;
    uint32 screen[2];
    uint32 depth, color, scale;
} EFFECT_RIBBON_TRAIL_PUBLICATION;

typedef char effect_trail_publication_layout[sizeof(EFFECT_TRAIL_POINT) == 12 && sizeof(EFFECT_TRAIL_POINT_PUBLICATION) == 16 && offsetof(EFFECT_TRAIL_POINT_PUBLICATION, reserved) == 12 && sizeof(EFFECT_QUAD_TRAIL_PUBLICATION) == 28 && offsetof(EFFECT_QUAD_TRAIL_PUBLICATION, color) == 4 && offsetof(EFFECT_QUAD_TRAIL_PUBLICATION, screen) == 8 && offsetof(EFFECT_QUAD_TRAIL_PUBLICATION, edge) == 12 && offsetof(EFFECT_QUAD_TRAIL_PUBLICATION, depth) == 24 && offsetof(EFFECT_QUAD_TRAIL_PUBLICATION, scale) == 26 && sizeof(EFFECT_RIBBON_TRAIL_PUBLICATION) == 24 && offsetof(EFFECT_RIBBON_TRAIL_PUBLICATION, screen) == 4 && offsetof(EFFECT_RIBBON_TRAIL_PUBLICATION, depth) == 12 && offsetof(EFFECT_RIBBON_TRAIL_PUBLICATION, color) == 16 && offsetof(EFFECT_RIBBON_TRAIL_PUBLICATION, scale) == 20 ? 1 : -1];

#include "psx_gte.h"

typedef struct
{
    uint32 write_index;
    uint32 read_index;
    uint32 age[32];
    uint32 life[32];
    uint32 vertices[32][4][3];
} EFFECT_QUAD_TRAIL;

typedef struct
{
    uint32 write_index;
    uint32 read_index;
    uint32 age[32];
    uint32 life[32];
    uint32 vertices[32][2][3];
} EFFECT_RIBBON_TRAIL;

static struct
{
    uint32 quad_mask;
    uint32 ribbon_mask;
    EFFECT_QUAD_TRAIL quad[32];
    EFFECT_RIBBON_TRAIL ribbon[32];
    uint8 object_slot[32];
    uint8 actor_slot[32];
} effect_trails;

static void effect_quad_trails_tick(void);
static void effect_ribbon_trails_tick(void);
static void effect_actor_trails_update(void);
static void effect_ribbon_trail_replace(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices);
static uint32 effect_ribbon_trail_append(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices);
static void effect_object_trails_update(void);
static void effect_quad_trail_replace(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices);
static uint32 effect_quad_trail_append(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices);
static uint32 effect_quad_trails_draw(uint32 ot, uint32 packet);
static uint32 effect_ribbon_trails_draw(uint32 ot, uint32 packet);

typedef char effect_trail_packet_layout_check[sizeof(LINE_G2) == 20 && offsetof(LINE_G2, tag) == 0 && offsetof(LINE_G2, p) == 4 ? 1 : -1];

void effect_trails_reset_storage(void)
{
    memset(&effect_trails, 0, sizeof(effect_trails));
}

void effect_trails_initialize(void)
{
    FUNCTION_MARKER(0x8007DA98, "ACE3.BIN");
    arena_alloc(0x1C248);
    effect_trails_reset();
}

void effect_trails_reset(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8007DA1C, "ACE3.BIN");
    effect_trails.quad_mask = 0;
    effect_trails.ribbon_mask = 0;
    for (index = 0; index < 32; ++index)
        effect_trails.object_slot[index] = 255;
    for (index = 0; index < 32; ++index)
        effect_trails.actor_slot[index] = 255;
    return;
}

static void effect_quad_trails_tick(void)
{
    EFFECT_QUAD_TRAIL *queue;
    uint32 mask, index, cursor, end, record, age, life, value;
    FUNCTION_MARKER(0x8007BF30, "ACE3.BIN");
    mask = effect_trails.quad_mask;
    for (index = 0; index < 32; ++index)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        queue = &effect_trails.quad[index];
        cursor = queue->read_index;
        end = queue->write_index;
        while ((sint32)cursor < (sint32)end)
        {
            record = cursor & 31;
            age = queue->age[record];
            life = queue->life[record];
            queue->age[record] = age + 1;
            queue->life[record] = life - 1;
            end = queue->write_index;
            ++cursor;
        }
        value = queue->read_index;
        record = value & 31;
        life = queue->life[record];
        if ((sint32)life < 0)
            queue->read_index = value + 1;
        value = queue->read_index;
        end = queue->write_index;
        if ((sint32)value >= (sint32)end)
            mask &= ~(1u << index);
    }
    effect_trails.quad_mask = mask;
    return;
}

static void effect_ribbon_trails_tick(void)
{
    EFFECT_RIBBON_TRAIL *queue;
    uint32 mask, index, cursor, end, record, age, life, value;
    FUNCTION_MARKER(0x8007CEB0, "ACE3.BIN");
    mask = effect_trails.ribbon_mask;
    for (index = 0; index < 32; ++index)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        queue = &effect_trails.ribbon[index];
        cursor = queue->read_index;
        end = queue->write_index;
        while ((sint32)cursor < (sint32)end)
        {
            record = cursor & 31;
            age = queue->age[record];
            life = queue->life[record];
            queue->age[record] = age + 1;
            queue->life[record] = life - 1;
            end = queue->write_index;
            ++cursor;
        }
        value = queue->read_index;
        record = value & 31;
        life = queue->life[record];
        if ((sint32)life < 0)
            queue->read_index = value + 1;
        value = queue->read_index;
        end = queue->write_index;
        if ((sint32)value >= (sint32)end)
            mask &= ~(1u << index);
    }
    effect_trails.ribbon_mask = mask;
    return;
}

static void effect_actor_trails_update(void)
{
    static const uint32 trail_anchors[31][4] = {
        {0x02004000, 0x0000F000, 0x0200C000, 0x0000F000}, {0x00804240, 0x0000E540, 0x0080BDC0, 0x0000E540}, {0x00002F40, 0x0000E580, 0x0000D0C0, 0x0000E580}, {0x00002F40, 0x0000CD40, 0x0000D0C0, 0x0000CD40}, {0xFDC04180, 0x0000E240, 0xFDC0BE80, 0x0000E240}, {0x00004700, 0x0000D500, 0x0000B900, 0x0000D500}, {0x00004040, 0x0000EC00, 0x0000BFC0, 0x0000EC00}, {0x03403500, 0x0000CD00, 0x0340CB00, 0x0000CD00}, {0x00003400, 0x0000D200, 0x0000CC00, 0x0000D200}, {0x00004CC0, 0x000094C0, 0x0000B340, 0x000094C0}, {0x054035C0, 0x0000E3C0, 0x0540CA40, 0x0000E3C0}, {0xFFC037C0, 0x0000DB00, 0xFFC0C840, 0x0000DB00}, {0x00804240, 0x0000E540, 0x0080BDC0, 0x0000E540}, {0x00002F40, 0x0000E580, 0x0000D0C0, 0x0000E580}, {0x07404040, 0x0000DC00, 0x0740BFC0, 0x0000DC00}, {0x02C06980, 0x0000F980, 0x02C09680, 0x0000F980},
        {0x00004A00, 0x0000DDC0, 0x0000B600, 0x0000DDC0}, {0x00004C40, 0x00000C00, 0x0000B3C0, 0x00000C00}, {0x07404040, 0x0000DC00, 0x0740BFC0, 0x0000DC00}, {0x02C06980, 0x0000F980, 0x02C09680, 0x0000F980}, {0x05404180, 0x0000D740, 0x0540BE80, 0x0000D740}, {0x00003740, 0x0000D680, 0x0000C8C0, 0x0000D680}, {0x07404680, 0x0000B5C0, 0x0740B980, 0x0000B5C0}, {0x04001E00, 0x0000C240, 0x0400E200, 0x0000C240}, {0xE6C03680, 0x0000F940, 0xE6C0C980, 0x0000F940}, {0x0E407FFF, 0x0000AE40, 0x0E408000, 0x0000AE40}, {0x0B807FFF, 0x0000CD40, 0x0B808000, 0x0000CD40}, {0x00004700, 0x0000BA40, 0x0000B900, 0x0000BA40}, {0x00003400, 0x0000D200, 0x0000CC00, 0x0000D200}, {0x00004C40, 0x0000F400, 0x0000B3C0, 0x0000F400}, {0x07404680, 0x0000B5C0, 0x0740B980, 0x0000B5C0},
    };
    const uint32 *vertex;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, kind, a, b, c, x, y, z, vx, vy, vz, slot, value;
    MATRIX *rotation = (MATRIX *)getScratchAddr(0);
    FUNCTION_MARKER(0x8007D7E0, "ACE3.BIN");
    EFFECT_TRAIL_POINT_PUBLICATION *published = (EFFECT_TRAIL_POINT_PUBLICATION *)getScratchAddr(8);
    for (index = 0; index < 32; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((actor_traits_object->flags & 0x14000) != 0x14000 || (actor_traits_object->render_flags & 8) == 0)
        {
            effect_trails.actor_slot[index] = 255;
            continue;
        }
        kind = (sint16)actor_traits_object->kind;
        if ((sint32)(kind << 1) >= 62 || (sint32)kind < 0)
            continue;
        vertex = trail_anchors[kind];
        rotation_build(rotation, pose_owner_object->pose.angles);
        SetRotMatrix(rotation);
        x = pose_owner_object->pose.position[0];
        y = pose_owner_object->pose.position[1];
        z = pose_owner_object->pose.position[2];
        xport_gte_write_data(0, vertex[0]);
        xport_gte_write_data(1, vertex[1]);
        xport_gte_execute(0x486012);
        a = xport_gte_read_data(25);
        published[2].point.axis[0] = a;
        b = xport_gte_read_data(26);
        published[2].point.axis[1] = b;
        c = xport_gte_read_data(27);
        published[2].point.axis[2] = c;
        vx = a << 3;
        vy = b << 3;
        vz = c << 3;
        published[0].point.axis[0] = vx + x;
        published[0].point.axis[1] = vy + y;
        published[0].point.axis[2] = vz + z;
        xport_gte_write_data(0, vertex[2]);
        xport_gte_write_data(1, vertex[3]);
        xport_gte_execute(0x486012);
        a = xport_gte_read_data(25);
        published[2].point.axis[0] = a;
        b = xport_gte_read_data(26);
        published[2].point.axis[1] = b;
        c = xport_gte_read_data(27);
        published[2].point.axis[2] = c;
        x += a << 3;
        z += c << 3;
        published[1].point.axis[0] = x;
        published[1].point.axis[2] = z;
        y += b << 3;
        published[1].point.axis[1] = y;
        value = game_runtime.mission_control.ticks;
        slot = (sint8)effect_trails.actor_slot[index];
        if ((value & 1) == (index & 1))
        {
            value = effect_ribbon_trail_append(slot, published);
            effect_trails.actor_slot[index] = (uint8)value;
        }
        else if ((sint32)slot >= 0)
            effect_ribbon_trail_replace(slot, published);
    }
    return;
}

static void effect_ribbon_trail_replace(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices)
{
    EFFECT_RIBBON_TRAIL *queue;
    uint32(*target)[3];
    uint32 mask, cursor;
    FUNCTION_MARKER(0x8007CDF8, "ACE3.BIN");
    mask = effect_trails.ribbon_mask;
    if ((sint32)slot < 0 || ((~mask) & (1u << (slot & 31))))
        return;
    queue = &effect_trails.ribbon[slot];
    cursor = queue->write_index;
    target = queue->vertices[(cursor - 1) & 31];
    for (uint32 point = 0; point < 2; ++point)
        memcpy(target[point], &vertices[point].point, sizeof(vertices[point].point));
    {
        (void)(slot);
        return;
    }
}

static uint32 effect_ribbon_trail_append(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices)
{
    EFFECT_RIBBON_TRAIL *queue;
    uint32(*target)[3];
    uint32 mask, write, read;
    FUNCTION_MARKER(0x8007CC6C, "ACE3.BIN");
    mask = effect_trails.ribbon_mask;
    if (slot == 0xFFFFFFFF)
    {
        if (mask == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        slot = 0;
        while (mask & (1u << (slot & 31)))
            ++slot;
        mask |= 1u << (slot & 31);
        queue = &effect_trails.ribbon[slot];
        queue->read_index = 0;
        queue->write_index = 0;
        effect_trails.ribbon_mask = mask;
    }
    else
    {
        if ((~mask) & (1u << (slot & 31)))
            return 0xFFFFFFFF;
        queue = &effect_trails.ribbon[slot];
    }
    write = queue->write_index;
    read = queue->read_index;
    if ((sint32)(write - read) >= 32)
        return slot;
    queue->age[write & 31] = 0;
    write = queue->write_index;
    queue->life[write & 31] = 63;
    write = queue->write_index;
    target = queue->vertices[write & 31];
    queue->write_index = write + 1;
    for (uint32 point = 0; point < 2; ++point)
        memcpy(target[point], &vertices[point].point, sizeof(vertices[point].point));
    return slot;
}

static void effect_object_trails_update(void)
{
    static const uint32 trail_cross_section[4] = {0x00000000, 0xF6000000, 0x0A00F600, 0x0A000A00};
    EFFECT_OBJECT *record;
    EFFECT_OBJECT *position;
    uint32 mask, index, a, b, c, x, y, z, point, tick, slot, value;
    MATRIX *rotation = (MATRIX *)getScratchAddr(0);
    FUNCTION_MARKER(0x8007C9E8, "ACE3.BIN");
    EFFECT_TRAIL_POINT_PUBLICATION *published = (EFFECT_TRAIL_POINT_PUBLICATION *)getScratchAddr(8);
    record = effect_objects.slot;
    position = effect_objects.slot;
    mask = effect_objects.live;
    for (index = 0; index < 32; ++index, ++record, ++position)
    {
        if ((mask & (1u << index)) == 0 || (position->render_flags & 8) == 0)
        {
            effect_trails.object_slot[index] = 255;
            continue;
        }
        rotation_build(rotation, record->pose.angles);
        SetRotMatrix(rotation);
        x = position->pose.position[0];
        y = position->pose.position[1];
        z = position->pose.position[2];
        for (point = 0; point < 4; ++point)
        {
            xport_gte_write_data(0, trail_cross_section[point]);
            xport_gte_execute(0x486012);
            a = xport_gte_read_data(25);
            b = xport_gte_read_data(26);
            c = xport_gte_read_data(27);
            a = (a << 3) + x;
            b = (b << 3) + y;
            c = (c << 3) + z;
            if (point == 3)
            {
            }
            published[point].point.axis[0] = a;
            published[point].point.axis[1] = b;
            published[point].point.axis[2] = c;
        }
        tick = game_runtime.mission_control.ticks;
        if ((tick & 3) != (index & 3))
        {
            slot = (sint8)effect_trails.object_slot[index];
            if ((sint32)slot >= 0)
            {
                effect_quad_trail_replace(slot, published);
                continue;
            }
        }
        slot = (sint8)effect_trails.object_slot[index];
        value = effect_quad_trail_append(slot, published);
        effect_trails.object_slot[index] = (uint8)value;
    }
    return;
}

static void effect_quad_trail_replace(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices)
{
    EFFECT_QUAD_TRAIL *queue;
    uint32(*target)[3];
    uint32 mask, cursor;
    FUNCTION_MARKER(0x8007BE3C, "ACE3.BIN");
    mask = effect_trails.quad_mask;
    if ((sint32)slot < 0 || ((~mask) & (1u << (slot & 31))))
        return;
    queue = &effect_trails.quad[slot];
    cursor = queue->write_index;
    target = queue->vertices[(cursor - 1) & 31];
    for (uint32 point = 0; point < 4; ++point)
        memcpy(target[point], &vertices[point].point, sizeof(vertices[point].point));
    {
        (void)(slot);
        return;
    }
}

static uint32 effect_quad_trail_append(uint32 slot, const EFFECT_TRAIL_POINT_PUBLICATION *vertices)
{
    EFFECT_QUAD_TRAIL *queue;
    uint32(*target)[3];
    uint32 mask, write, read;
    FUNCTION_MARKER(0x8007BC88, "ACE3.BIN");
    mask = effect_trails.quad_mask;
    if (slot == 0xFFFFFFFF)
    {
        if (mask == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        slot = 0;
        while (mask & (1u << (slot & 31)))
            ++slot;
        mask |= 1u << (slot & 31);
        queue = &effect_trails.quad[slot];
        queue->read_index = 0;
        queue->write_index = 0;
        effect_trails.quad_mask = mask;
    }
    else
    {
        if ((~mask) & (1u << (slot & 31)))
            return 0xFFFFFFFF;
        queue = &effect_trails.quad[slot];
    }
    write = queue->write_index;
    read = queue->read_index;
    if ((sint32)(write - read) >= 32)
        return slot;
    queue->age[write & 31] = 0;
    write = queue->write_index;
    queue->life[write & 31] = 90;
    write = queue->write_index;
    target = queue->vertices[write & 31];
    queue->write_index = write + 1;
    for (uint32 point = 0; point < 4; ++point)
        memcpy(target[point], &vertices[point].point, sizeof(vertices[point].point));
    return slot;
}

static uint32 effect_quad_trails_draw(uint32 ot, uint32 packet)
{
    EFFECT_QUAD_TRAIL *records;
    uint32(*source)[3];
    uint32(*source_cache[32])[3];
    EFFECT_QUAD_TRAIL_PUBLICATION *projected = (EFFECT_QUAD_TRAIL_PUBLICATION *)getScratchAddr(0);
    uint32(*neighbor_source)[3];
    uint32 mask, limit_x, limit_y, cx, cy, cz, h, close_limit, scale_limit, group, a, b, c, x, y, z, ax, ay, az, distance, scale, index, slot, count, close_bits, visible, clipped, last_bit, bit, value, color, rate, depth, xy, px, py, magnitude, expanded_close, expanded_visible, shift, neighbor, other, numerator, divisor, ratio, j, offset, sx, sy, sz, nx, ny, nz, old_xy, old_edges[3], old_depth, old_color, new_xy, new_edges[3], new_depth, new_color, pair, q;
    FUNCTION_MARKER(0x8007C00C, "ACE3.BIN");
    mask = effect_trails.quad_mask;
    if (mask == 0)
        return packet;
    a = game_runtime.camera_proj.size[0];
    b = game_runtime.camera_proj.size[1];
    limit_x = (uint32)((sint32)(a << 16) >> 17) + 56;
    limit_y = (uint32)((sint32)(b << 16) >> 17) + 56;
    cx = game_runtime.camera_view.pose.position[0];
    cy = game_runtime.camera_view.pose.position[1];
    cz = game_runtime.camera_view.pose.position[2];
    h = xport_gte_read_control(26) << 2;
    close_limit = h << 2;
    scale_limit = h * 3;
    xport_gte_write_data(6, 0);
    for (group = 0; group < 32; ++group)
    {
        records = &effect_trails.quad[group];
        a = game_runtime.scene_assets.environment.trail_back_color[0];
        b = game_runtime.scene_assets.environment.trail_back_color[1];
        c = game_runtime.scene_assets.environment.trail_back_color[2];
        xport_gte_write_control(21, a << 4);
        xport_gte_write_control(22, b << 4);
        xport_gte_write_control(23, c << 4);
        if ((mask & (1u << group)) == 0)
            continue;
        count = 0;
        clipped = 0;
        close_bits = 0;
        last_bit = 0;
        visible = 0;
        index = records->read_index;
        value = records->write_index;
        while ((sint32)index < (sint32)value)
        {
            slot = index & 31;
            source = records->vertices[slot];
            a = source[0][0];
            b = source[0][1];
            x = (sint32)(a - cx) >> 10;
            y = (sint32)(b - cy) >> 10;
            c = source[0][2];
            z = (sint32)(c - cz) >> 10;
            ax = (sint32)x < 0 ? 0 - x : x;
            ay = (sint32)y < 0 ? 0 - y : y;
            az = (sint32)z < 0 ? 0 - z : z;
            distance = (sint32)(ax + ay + az) >> 2;
            if ((sint32)distance <= 0xB000)
            {
                scale = 2;
                if ((sint32)scale_limit >= (sint32)distance)
                    scale = ((sint32)scale_limit >> 1) < (sint32)distance;
                x = (sint32)x >> scale;
                y = (sint32)y >> scale;
                z = (sint32)z >> scale;
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                projected[count].scale = (uint16)scale;
                source_cache[count] = source;
                bit = 1u << (count & 31);
                if ((sint32)distance < (sint32)close_limit)
                    close_bits |= bit;
                value = records->write_index;
                if (index == value - 1)
                    last_bit = bit;
                a = records->age[slot];
                b = records->life[slot];
                rate = b << 5;
                if ((sint32)(a << 4) < (sint32)b)
                    rate = a << 7;
                depth = xport_gte_read_data(11);
                xport_gte_write_data(8, rate);
                value = xport_gte_read_control(31);
                xport_gte_execute(0x780010);
                if ((value & 0x20000) != 0)
                {
                    clipped |= bit;
                    bit = 0;
                }
                xy = xport_gte_read_data(14);
                px = (sint16)xy;
                py = (sint32)xy >> 16;
                depth = (sint32)depth >> (2 - scale);
                magnitude = (sint32)px < 0 ? 0 - px : px;
                if ((sint32)limit_x < (sint32)magnitude)
                    bit = 0;
                else
                {
                    magnitude = (sint32)py < 0 ? 0 - py : py;
                    if ((sint32)limit_y < (sint32)magnitude)
                        bit = 0;
                }
                visible |= bit;
                value = xport_gte_read_data(14);
                projected[count].screen = value;
                projected[count].depth = (uint16)depth;
                color = xport_gte_read_data(22);
                projected[count].color = color;
                ++count;
            }
            value = records->write_index;
            ++index;
        }
        expanded_close = close_bits | ((((uint32)((sint32)close_bits >> 1)) | (close_bits << 1)) & ~visible);
        expanded_visible = visible | ((((uint32)((sint32)visible >> 1)) | (visible << 1)) & ~visible);
        for (index = 0; (sint32)index < (sint32)count; ++index)
        {
            bit = 1u << (index & 31);
            depth = (sint16)projected[index].depth;
            if ((expanded_visible & bit) == 0)
                continue;
            if ((expanded_close & bit) == 0)
            {
                if ((sint32)depth < 0)
                    projected[index].depth = (uint16)0;
                continue;
            }
            shift = (sint16)projected[index].scale;
            source = source_cache[index];
            shift = (shift + 10) & 31;
            if ((clipped & bit) != 0)
            {
                neighbor = (sint32)index < (sint32)(count - 1) ? index + 1 : index - 1;
                value = xport_gte_read_control(26);
                numerator = (depth - (uint32)((sint32)value >> 3)) << 12;
                other = (sint16)projected[neighbor].depth;
                divisor = depth - other;
                if (divisor == 0)
                    xport_mips_break(7);
                if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
                    xport_mips_break(6);
                ratio = (sint32)numerator / (sint32)divisor;
                xport_gte_write_data(8, ratio);
                a = source[0][0];
                b = source[0][1];
                neighbor_source = source_cache[neighbor];
                sx = (sint32)(a - cx) >> shift;
                sy = (sint32)(b - cy) >> shift;
                c = source[0][2];
                a = neighbor_source[0][0];
                b = neighbor_source[0][1];
                sz = (sint32)(c - cz) >> shift;
                nx = (sint32)(a - cx) >> shift;
                ny = (sint32)(b - cy) >> shift;
                c = neighbor_source[0][2];
                nz = (sint32)(c - cz) >> shift;
                xport_gte_write_data(9, sx);
                xport_gte_write_data(10, sy);
                xport_gte_write_data(11, sz);
                xport_gte_write_control(21, nx);
                xport_gte_write_control(22, ny);
                xport_gte_write_control(23, nz);
                xport_gte_execute(0x980011);
                x = xport_gte_read_data(9);
                y = xport_gte_read_data(10);
                z = xport_gte_read_data(11);
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                value = xport_gte_read_data(14);
                projected[index].screen = value;
                for (j = 1; j < 4; ++j)
                {
                    offset = j * 16;
                    a = neighbor_source[j][0];
                    b = neighbor_source[j][1];
                    sy = source[j][1];
                    c = neighbor_source[j][2];
                    sx = source[j][0];
                    sz = source[j][2];
                    nx = (sint32)(a - cx) >> shift;
                    ny = (sint32)(b - cy) >> shift;
                    nz = (sint32)(c - cz) >> shift;
                    sx = (sint32)(sx - cx) >> shift;
                    sy = (sint32)(sy - cy) >> shift;
                    sz = (sint32)(sz - cz) >> shift;
                    if (j == 1)
                        xport_gte_write_data(8, ratio);
                    xport_gte_write_data(9, sx);
                    xport_gte_write_data(10, sy);
                    xport_gte_write_data(11, sz);
                    xport_gte_write_control(21, nx);
                    xport_gte_write_control(22, ny);
                    xport_gte_write_control(23, nz);
                    xport_gte_execute(0x980011);
                    x = xport_gte_read_data(9);
                    y = xport_gte_read_data(10);
                    z = xport_gte_read_data(11);
                    if (j < 3)
                    {
                        xport_gte_write_data((j - 1) * 2, (x & 0xFFFF) | (y << 16));
                        xport_gte_write_data((j - 1) * 2 + 1, z);
                    }
                }
                projected[index].color = 0;
            }
            else
            {
                for (j = 1; j < 4; ++j)
                {
                    offset = j * 16;
                    a = source[j][0];
                    b = source[j][1];
                    x = (sint32)(a - cx) >> shift;
                    y = (sint32)(b - cy) >> shift;
                    c = source[j][2];
                    z = (sint32)(c - cz) >> shift;
                    if (j < 3)
                    {
                        xport_gte_write_data((j - 1) * 2, (x & 0xFFFF) | (y << 16));
                        xport_gte_write_data((j - 1) * 2 + 1, z);
                    }
                }
            }
            xport_gte_write_data(4, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(5, z);
            xport_gte_execute(0x280030);
            if ((sint32)depth < 0)
                projected[index].depth = (uint16)0;
            if ((last_bit & bit) != 0)
            {
                value = projected[index].screen;
                projected[index].edge[0] = value;
                projected[index].edge[1] = value;
                projected[index].edge[2] = value;
                xport_gte_read_data(27);
            }
            else
            {
                value = xport_gte_read_data(12);
                projected[index].edge[0] = value;
                value = xport_gte_read_data(13);
                projected[index].edge[1] = value;
                value = xport_gte_read_data(14);
                projected[index].edge[2] = value;
            }
        }
        old_xy = projected[0].screen;
        old_edges[0] = projected[0].edge[0];
        old_edges[1] = projected[0].edge[1];
        old_edges[2] = projected[0].edge[2];
        old_color = projected[0].color;
        old_depth = (sint16)projected[0].depth;
        for (index = 1; (sint32)index < (sint32)count; ++index)
        {
            pair = 3u << ((index - 1) & 31);
            new_xy = projected[index].screen;
            new_edges[0] = projected[index].edge[0];
            new_edges[1] = projected[index].edge[1];
            new_edges[2] = projected[index].edge[2];
            new_depth = (sint16)projected[index].depth;
            new_color = projected[index].color;
            if (((~expanded_visible) & pair) == 0 && (sint32)old_depth < 0x5800)
            {
                uint32 ordering = ot + ((uint32)((sint32)old_depth >> 3) << 2);
                uint32 *ot_tag = mission_ot_span(ordering, sizeof(*ot_tag));
                if (((~expanded_close) & pair) != 0)
                {
                    LINE_G2 *line = mission_packet_span(packet, sizeof(*line));
                    line->p[0].rgbc.rgbc = (uint32)(old_color | 0x52000000);
                    line->p[0].xy.xy = (uint32)(old_xy);
                    line->p[1].rgbc.rgbc = (uint32)(new_color);
                    line->p[1].xy.xy = (uint32)(new_xy);
                    value = *ot_tag;
                    line->tag = (uint32)(value | 0x04000000);
                    *ot_tag = packet & 0xFFFFFF;
                    packet += 20;
                }
                else
                {
                    color = old_color | 0x3A000000;
                    for (q = 0; q < 3; ++q)
                    {
                        POLY_G4 *quad = mission_packet_span(packet, sizeof(*quad));
                        quad->p[0].rgbc.rgbc = (uint32)(color);
                        quad->p[0].xy.xy = (uint32)(old_xy);
                        quad->p[1].rgbc.rgbc = (uint32)(0);
                        quad->p[1].xy.xy = (uint32)(old_edges[q]);
                        quad->p[2].rgbc.rgbc = (uint32)(new_color);
                        quad->p[2].xy.xy = (uint32)(new_xy);
                        quad->p[3].rgbc.rgbc = (uint32)(0);
                        quad->p[3].xy.xy = (uint32)(new_edges[q]);
                        value = *ot_tag;
                        quad->tag = (uint32)(value | 0x08000000);
                        *ot_tag = packet & 0xFFFFFF;
                        packet += 36;
                    }
                }
            }
            old_xy = new_xy;
            old_edges[0] = new_edges[0];
            old_edges[1] = new_edges[1];
            old_edges[2] = new_edges[2];
            old_depth = new_depth;
            old_color = new_color;
        }
    }
    return packet;
}

static uint32 effect_ribbon_trails_draw(uint32 ot, uint32 packet)
{
    EFFECT_RIBBON_TRAIL *records;
    uint32(*source)[3];
    uint32(*source_cache[32])[3];
    EFFECT_RIBBON_TRAIL_PUBLICATION *projected = (EFFECT_RIBBON_TRAIL_PUBLICATION *)getScratchAddr(0);
    uint32 mask, cx, cy, cz, limit_x, limit_y, h, near_limit, range, group, index, slot, count, a, b, c, d, e, f, x, y, z, ax, ay, az, distance, scale, close_bits, visible, bit, brightness, depth, xy, px, py, value, expanded_close, expanded_visible, shift, j, old_x, old_y, old_x2, old_y2, old_depth, old_color, new_x, new_y, new_x2, new_y2, new_depth, new_color, pair, base, tag, color, side, step, source_x, source_y, dest_x, dest_y, offset;
    FUNCTION_MARKER(0x8007CFA4, "ACE3.BIN");
    mask = effect_trails.ribbon_mask;
    if (mask == 0)
        return packet;
    cx = game_runtime.camera_view.pose.position[0];
    cy = game_runtime.camera_view.pose.position[1];
    a = game_runtime.camera_proj.size[0];
    cz = game_runtime.camera_view.pose.position[2];
    b = game_runtime.camera_proj.size[1];
    limit_x = (sint32)(a << 16) >> 17;
    h = ((sint16)game_runtime.camera_proj.distance);
    limit_y = (sint32)(b << 16) >> 17;
    near_limit = h << 3;
    range = (h << 2) + near_limit;
    for (group = 0; group < 32; ++group)
    {
        records = &effect_trails.ribbon[group];
        if ((mask & (1u << group)) == 0)
            continue;
        count = 0;
        close_bits = 0xFFFFFFFF;
        visible = 0xFFFFFFFF;
        index = records->read_index;
        value = records->write_index;
        while ((sint32)index < (sint32)value)
        {
            slot = index & 31;
            source = records->vertices[slot];
            a = source[0][0];
            d = source[1][0];
            e = source[1][1];
            b = source[0][1];
            c = source[0][2];
            f = source[1][2];
            x = (sint32)((uint32)((sint32)(a + d) >> 1) - cx) >> 10;
            y = (sint32)((uint32)((sint32)(b + e) >> 1) - cy) >> 10;
            z = (sint32)((uint32)((sint32)(c + f) >> 1) - cz) >> 10;
            ax = (sint32)x < 0 ? 0 - x : x;
            ay = (sint32)y < 0 ? 0 - y : y;
            az = (sint32)z < 0 ? 0 - z : z;
            distance = (sint32)(ax + ay + az) >> 2;
            if ((sint32)distance <= 0xB000)
            {
                scale = 2;
                if ((sint32)range >= (sint32)distance)
                    scale = ((sint32)range >> 1) < (sint32)distance;
                x = (sint32)x >> scale;
                y = (sint32)y >> scale;
                z = (sint32)z >> scale;
                xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(1, z);
                xport_gte_execute(0x180001);
                projected[count].scale = scale;
                source_cache[count] = source;
                bit = 1u << (count & 31);
                brightness = records->life[slot];
                if ((sint32)near_limit < (sint32)distance)
                {
                    close_bits &= ~bit;
                    brightness = (sint32)brightness >> 1;
                }
                if ((sint32)brightness >= 33)
                    brightness = 32;
                depth = xport_gte_read_data(19);
                xy = xport_gte_read_data(14);
                px = (uint32)(sint32)(sint16)xy;
                py = (sint32)xy >> 16;
                depth = (sint32)depth >> (2 - scale);
                if ((sint32)depth < ((sint32)range >> 3))
                    brightness = (sint32)brightness >> 1;
                if ((sint32)depth < ((sint32)range >> 4))
                    brightness = (sint32)brightness >> 1;
                if ((sint32)depth < ((sint32)range >> 5))
                    brightness = 0;
                projected[count].color = brightness | (brightness << 8) | (brightness << 16);
                ax = (sint32)px < 0 ? 0 - px : px;
                if ((sint32)limit_x < (sint32)ax)
                    visible &= ~bit;
                ay = (sint32)py < 0 ? 0 - py : py;
                if ((sint32)limit_y < (sint32)ay)
                    visible &= ~bit;
                az = (sint32)depth < 0 ? 0 - depth : depth;
                if ((sint32)az <= 0)
                    visible &= ~bit;
                projected[count].depth = depth;
                ++count;
            }
            value = records->write_index;
            ++index;
        }
        expanded_close = close_bits | (uint32)((sint32)close_bits >> 1) | (close_bits << 1);
        expanded_visible = visible | (uint32)((sint32)visible >> 1) | (visible << 1);
        for (index = 0; (sint32)index < (sint32)count; ++index)
        {
            if ((expanded_visible & (1u << (index & 31))) == 0)
                continue;
            source = source_cache[index];
            shift = (projected[index].scale + 10) & 31;
            for (j = 0; j < 2; ++j)
            {
                offset = j * 16;
                a = source[j][0];
                b = source[j][1];
                x = (sint32)(a - cx) >> shift;
                y = (sint32)(b - cy) >> shift;
                c = source[j][2];
                z = (sint32)(c - cz) >> shift;
                xport_gte_write_data(j * 2, (x & 0xFFFF) | (y << 16));
                xport_gte_write_data(j * 2 + 1, z);
            }
            xport_gte_execute(0x280030);
            value = xport_gte_read_data(12);
            projected[index].screen[0] = value;
            value = xport_gte_read_data(13);
            projected[index].screen[1] = value;
        }
        old_depth = projected[0].depth;
        old_color = projected[0].color;
        a = (uint16)projected[0].screen[0];
        b = (uint16)(projected[0].screen[0] >> 16);
        c = (uint16)projected[0].screen[1];
        d = (uint16)(projected[0].screen[1] >> 16);
        old_x = (sint16)a;
        old_y = (sint16)b;
        old_x2 = (sint16)c;
        old_y2 = (sint16)d;
        for (index = 1; (sint32)index < (sint32)count; ++index)
        {
            pair = 3u << ((index - 1) & 31);
            new_x = (sint16)projected[index].screen[0];
            new_y = (sint16)(projected[index].screen[0] >> 16);
            new_x2 = (sint16)projected[index].screen[1];
            new_depth = projected[index].depth;
            new_y2 = (sint16)(projected[index].screen[1] >> 16);
            new_color = projected[index].color;
            if (((~expanded_visible) & pair) == 0 && (sint32)old_depth < 0x5800)
            {
                uint32 ordering = ot + ((uint32)((sint32)old_depth >> 3) << 2);
                uint32 *ot_tag = mission_ot_span(ordering, sizeof(*ot_tag));
                base = packet;
                if (((~expanded_close) & pair) != 0)
                {
                    LINE_G2 *lines = mission_packet_span(base, 2 * sizeof(*lines));
                    lines[0].p[0].xy.x = (sint16)(uint16)(old_x);
                    lines[0].p[0].xy.y = (sint16)(uint16)(old_y);
                    lines[0].p[1].xy.x = (sint16)(uint16)(new_x);
                    lines[0].p[1].xy.y = (sint16)(uint16)(new_y);
                    lines[1].p[0].xy.x = (sint16)(uint16)(old_x2);
                    lines[1].p[0].xy.y = (sint16)(uint16)(old_y2);
                    lines[1].p[1].xy.y = (sint16)(uint16)(new_y2);
                    lines[1].p[1].xy.x = (sint16)(uint16)(new_x2);
                    packet += 40;
                    color = old_color | 0x52000000;
                    lines[0].p[0].rgbc.rgbc = (uint32)(color);
                    lines[0].p[1].rgbc.rgbc = (uint32)(new_color);
                    lines[1].p[0].rgbc.rgbc = (uint32)(color);
                    lines[1].p[1].rgbc.rgbc = (uint32)(new_color);
                    value = *ot_tag;
                    lines[0].tag = (uint32)(value | 0x04000000);
                    tag = base & 0xFFFFFF;
                    *ot_tag = tag;
                    lines[1].tag = (uint32)(tag | 0x04000000);
                    *ot_tag = (base + 20) & 0xFFFFFF;
                }
                else
                {
                    POLY_G4 *quads = mission_packet_span(base, 4 * sizeof(*quads));
                    packet += 144;
                    for (side = 0; side < 2; ++side)
                    {
                        source_x = side == 0 ? old_x : new_x;
                        source_y = side == 0 ? old_y : new_y;
                        dest_x = side == 0 ? old_x2 : new_x2;
                        dest_y = side == 0 ? old_y2 : new_y2;
                        xport_gte_write_control(21, dest_x);
                        xport_gte_write_control(22, dest_y);
                        for (j = 0; j < 4; ++j)
                        {
                            step = j == 0 ? (uint32)-192 : j == 1 ? 192 : j == 2 ? 3904 : 4288;
                            xport_gte_write_data(8, step);
                            xport_gte_write_data(9, source_x);
                            xport_gte_write_data(10, source_y);
                            xport_gte_execute(0x980011);
                            a = xport_gte_read_data(9);
                            b = xport_gte_read_data(10);
                            quads[j].p[side * 2 + (j & 1)].xy.x = (sint16)(uint16)a;
                            quads[j].p[side * 2 + (j & 1)].xy.y = (sint16)(uint16)b;
                            if (j == 0)
                            {
                                quads[0].p[side * 2 + 1].xy.x = (sint16)(uint16)source_x;
                                quads[0].p[side * 2 + 1].xy.y = (sint16)(uint16)source_y;
                                quads[1].p[side * 2].xy.x = (sint16)(uint16)source_x;
                                quads[1].p[side * 2].xy.y = (sint16)(uint16)source_y;
                            }
                            else if (j == 2)
                            {
                                if (side == 0)
                                {
                                    quads[2].p[1].xy.x = (sint16)(uint16)(dest_x);
                                    quads[2].p[1].xy.y = (sint16)(uint16)(dest_y);
                                    quads[3].p[0].xy.x = (sint16)(uint16)(dest_x);
                                    quads[3].p[0].xy.y = (sint16)(uint16)(dest_y);
                                }
                                else
                                {
                                    quads[2].p[3].xy.y = (sint16)(uint16)(dest_y);
                                    quads[2].p[3].xy.x = (sint16)(uint16)(dest_x);
                                    quads[3].p[2].xy.y = (sint16)(uint16)(dest_y);
                                    quads[3].p[2].xy.x = (sint16)(uint16)(dest_x);
                                }
                            }
                        }
                    }
                    quads[0].p[0].rgbc.rgbc = (uint32)(0x3A000000);
                    quads[0].p[2].rgbc.rgbc = (uint32)(0);
                    quads[0].p[3].rgbc.rgbc = (uint32)(new_color);
                    quads[1].p[1].rgbc.rgbc = (uint32)(0);
                    quads[1].p[2].rgbc.rgbc = (uint32)(new_color);
                    quads[1].p[3].rgbc.rgbc = (uint32)(0);
                    quads[2].p[0].rgbc.rgbc = (uint32)(0x3A000000);
                    color = old_color | 0x3A000000;
                    quads[0].p[1].rgbc.rgbc = (uint32)(old_color);
                    quads[1].p[0].rgbc.rgbc = (uint32)(color);
                    quads[2].p[1].rgbc.rgbc = (uint32)(old_color);
                    quads[2].p[2].rgbc.rgbc = (uint32)(0);
                    quads[2].p[3].rgbc.rgbc = (uint32)(new_color);
                    quads[3].p[0].rgbc.rgbc = (uint32)(color);
                    quads[3].p[1].rgbc.rgbc = (uint32)(0);
                    quads[3].p[2].rgbc.rgbc = (uint32)(new_color);
                    quads[3].p[3].rgbc.rgbc = (uint32)(0);
                    value = *ot_tag;
                    quads[0].tag = (uint32)(value | 0x08000000);
                    tag = base & 0xFFFFFF;
                    *ot_tag = tag;
                    quads[1].tag = (uint32)(tag | 0x08000000);
                    tag = (base + 36) & 0xFFFFFF;
                    *ot_tag = tag;
                    quads[2].tag = (uint32)(tag | 0x08000000);
                    tag = (base + 72) & 0xFFFFFF;
                    *ot_tag = tag;
                    quads[3].tag = (uint32)(tag | 0x08000000);
                    *ot_tag = (base + 3 * sizeof(*quads)) & 0xFFFFFF;
                }
            }
            old_x = new_x;
            old_y = new_y;
            old_y2 = new_y2;
            old_x2 = new_x2;
            old_depth = new_depth;
            old_color = new_color;
        }
    }
    return packet;
}

void effect_trails_update(void)
{
    FUNCTION_MARKER(0x8007DAC8, "ACE3.BIN");
    effect_object_trails_update();
    effect_quad_trails_tick();
    effect_actor_trails_update();
    effect_ribbon_trails_tick();
}

uint32 effect_trails_draw(uint32 ot, uint32 packet)
{
    uint32 next;
    FUNCTION_MARKER(0x8007DB00, "ACE3.BIN");
    next = effect_quad_trails_draw(ot, packet);
    return effect_ribbon_trails_draw(ot, next);
}
