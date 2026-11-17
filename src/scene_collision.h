#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include <stddef.h>

#include "psx.h"
#include "mission_actor.h"

typedef struct
{
    uint16 normal[3];
    uint32 flags;
} SCENE_COLLISION_RESULT;

typedef struct
{
    uint32 x, y, z;
    uint32 flags;
    uint32 radius_times_three;
    uint32 radius_squared;
} ACTOR_COLLISION_CANDIDATE;

typedef struct
{
    uint32 x, y, z, flags;
} ACTOR_COLLISION_PUBLICATION;

typedef char actor_collision_publication_layout[
    sizeof(ACTOR_COLLISION_PUBLICATION) == 16 &&
    offsetof(ACTOR_COLLISION_PUBLICATION, x) == 0 &&
    offsetof(ACTOR_COLLISION_PUBLICATION, y) == 4 &&
    offsetof(ACTOR_COLLISION_PUBLICATION, z) == 8 &&
    offsetof(ACTOR_COLLISION_PUBLICATION, flags) == 12 &&
    MISSION_ACTOR_COUNT * sizeof(ACTOR_COLLISION_PUBLICATION) == 1024 ? 1 : -1];

extern const uint16 scene_bounds_normals[6][3];

/* workspace_offset is a byte offset in the supplied SDK working buffer */
uint32 collision_query_kind(const uint32 *start, const uint32 *end, uint32 *out, uint32 workspace_offset, uint32 kind);
uint32 collision_query_tile(const uint32 *start, const uint32 *end, uint32 *hit);
uint32 collision_query_segment(const uint32 *start, const uint32 *end, uint32 *hit);

uint32 collision_intersect_height(uint32 plane, const uint32 *start, const sint16 *delta, uint32 *hit);

void collision_build_basis(MATRIX *matrix, const uint16 *normal);

uint32 collision_trace_actor(const MISSION_ACTOR *target, uint32 *hit, const uint32 *pos, const uint32 *vel);

uint32 scene_query_ground(const uint32 *position, uint16 *native_normal, uint32 *native_surface);

#endif
