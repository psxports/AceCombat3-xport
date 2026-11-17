#ifndef MENU_BINDING_DATA_H
#define MENU_BINDING_DATA_H

#include "psx.h"

typedef struct
{
    uint16 vram_x, vram_y;
    uint16 width, height;
} MENU_BINDING_IMAGE;

typedef struct
{
    uint32 phase;
    uint32 packed_xy[3];
} MENU_BINDING_LABEL;

typedef struct
{
    uint32 phase;
    uint32 packed_xy[4];
} MENU_BINDING_QUAD_LABEL;

uint32 menu_binding_count(uint32 profile);
uint32 menu_binding_key(uint32 profile, sint32 index);
uint32 menu_binding_action_key(uint32 action);
const MENU_BINDING_LABEL *menu_binding_label(uint32 profile, uint32 index);
const MENU_BINDING_QUAD_LABEL *menu_binding_extra_label(uint32 index);

const MENU_BINDING_IMAGE *menu_binding_button_image(uint32 index);
const MENU_BINDING_IMAGE *menu_binding_axis_image(uint32 index);

#endif
