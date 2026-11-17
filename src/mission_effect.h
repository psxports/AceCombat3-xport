#ifndef MISSION_EFFECT_H
#define MISSION_EFFECT_H

#include <stddef.h>

#include "psx.h"

enum
{
    MISSION_EFFECT_RING,
    MISSION_EFFECT_SPR,
    MISSION_EFFECT_TRAIL,
    MISSION_EFFECT_LAYER_COUNT
};

enum
{
    MISSION_EFFECT_PALETTE_COUNT = 16,
    MISSION_EFFECT_TRAIL_COUNT = 3,
    MISSION_EFFECT_TRAIL_HISTORY = 15,
    MISSION_EFFECT_TRAIL_VERTEX_COUNT = 5,
    MISSION_EFFECT_SPRITE_COUNT = 48,
    MISSION_EFFECT_RING_VERTEX_COUNT = 16,
    MISSION_EFFECT_RING_HISTORY = 64
};

typedef struct
{
    uint32 mode;
    uint32 elapsed;
} MISSION_EFFECT_LAYER;

typedef struct
{
    SVECTOR angle[MISSION_EFFECT_TRAIL_HISTORY];
    VECTOR pos[MISSION_EFFECT_TRAIL_HISTORY];
} MISSION_TRAIL_STATE;

typedef struct
{
    sint16 phase;
    VECTOR pos;
} MISSION_EFFECT_SPRITE;

typedef struct
{
    uint32 entries[256];
} MISSION_EFFECT_ORDERING_TABLE;

typedef struct
{
    uint32 frame_counter;
    SVECTOR view_angles;
    MATRIX view_matrix;
    MISSION_EFFECT_LAYER layers[MISSION_EFFECT_LAYER_COUNT];
    uint16 ring_phase;
    uint16 ring_step;
    uint16 ring_remainder;
    uint32 ring_scroll;
    uint16 tpage;
    uint16 clut;
    uint16 pal[MISSION_EFFECT_PALETTE_COUNT];
    MISSION_TRAIL_STATE trails[MISSION_EFFECT_TRAIL_COUNT];
    SVECTOR trail_vertex[MISSION_EFFECT_TRAIL_VERTEX_COUNT];
    MISSION_EFFECT_SPRITE spr[MISSION_EFFECT_SPRITE_COUNT];
    SVECTOR ring_vertex[MISSION_EFFECT_RING_VERTEX_COUNT];
    VECTOR ring_pos[MISSION_EFFECT_RING_HISTORY];
    SVECTOR ring_angle[MISSION_EFFECT_RING_HISTORY];
    /* encoded GPU links for native ordering tables */
    uint32 ot[2];
    MISSION_EFFECT_ORDERING_TABLE *ot_data[2];
    uint32 cur_ot;
} MISSION_EFFECT;

typedef char MISSION_EFFECT_PALETTE_ALIGNMENT_CHECK[offsetof(MISSION_EFFECT, pal) % sizeof(uint32) == 0 ? 1 : -1];

typedef char MISSION_EFFECT_MATRIX_LAYOUT_CHECK[sizeof(MATRIX) == 32 && offsetof(MATRIX, m) == 0 &&
    sizeof(((MATRIX *)0)->m) == 18 && offsetof(MATRIX, t) == 20 ? 1 : -1];

extern MISSION_EFFECT mission_effect;

uint32 *mission_effect_native_ot_span(uint32 address, size_t bytes);

void mission_effect_init(void);
void mission_effect_open_ring(void);
uint32 mission_effect_set_ring(uint32 mode);
void mission_effect_open_sprs(void);
void mission_effect_close_sprs(void);
void mission_effect_open_trails(void);
uint32 mission_effect_close_trails(void);
uint32 mission_effect_draw(uint32 packet);
void mission_effect_begin_frame(uint32 ordering);

#endif
