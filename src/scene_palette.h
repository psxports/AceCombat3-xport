#ifndef SCENE_PALETTE_H
#define SCENE_PALETTE_H

#include "psx.h"
#include "resource_queue.h"

enum
{
    SCENE_PALETTE_COLORS = 128,
    SCENE_PALETTE_WORDS = 64
};

typedef struct
{
    PSX_RECT rectangle;
    uint32 saved[SCENE_PALETTE_WORDS];
    uint32 neutral[SCENE_PALETTE_WORDS];
} SCENE_PALETTE;

extern SCENE_PALETTE scene_palette;

void scene_palette_reset(void);
void scene_palette_begin(void);
void scene_palette_capture(uint32 resource, RESOURCE_ASSET_SOURCE parent);
uint32 scene_palette_restore(uint32 mode);
void scene_palette_blend(uint32 amount);

void scene_palette_blend_pixels(uint8 *dst, const uint8 *src, sint32 count, uint32 colors, uint32 factor);

#endif
