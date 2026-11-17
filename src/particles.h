#ifndef PARTICLES_H
#define PARTICLES_H

#include "psx.h"

enum { PARTICLE_CAPACITY = 128, PARTICLE_MASK_WORDS = 4 };

typedef struct
{
    uint32 flags;
    uint32 lifetime;
    uint32 position[3];
    uint32 endpoint[3];
    uint32 velocity[3];
    uint32 actor_flags;
} PARTICLE;

typedef struct
{
    PARTICLE slot[PARTICLE_CAPACITY];
    uint32 live[PARTICLE_MASK_WORDS];
} PARTICLE_POOL;

extern PARTICLE_POOL particles;

void particle_tick_simple(void);
void particle_expire(uint32 index);
uint32 particle_draw_segments(uint32 ordering, uint32 packet);
void particle_update(void);

void particle_draw_frame(void);

void particle_update_simple(void);

#endif
