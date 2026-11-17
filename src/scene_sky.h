#ifndef SCENE_SKY_H
#define SCENE_SKY_H

#include "psx.h"

uint32 scene_geometry_draw_sky_mode_four(uint32 ordering, uint32 packet);
uint32 scene_geometry_draw_sky_mode_three(uint32 ordering, uint32 packet);
uint32 scene_light_draw_horizon_pass(uint32 ordering, uint32 packet);
uint32 scene_light_draw_pass(uint32 ordering, uint32 packet);

#endif
