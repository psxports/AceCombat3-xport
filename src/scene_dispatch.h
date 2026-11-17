#ifndef SCENE_DISPATCH_H
#define SCENE_DISPATCH_H

#include "psx.h"

void scene_dispatch_set_arena(void);
void scene_dispatch(void);

uint32 scene_return_frontend_command(void);
void scene_overlay_activate(void);
uint32 scene_overlay_begin_load(uint32 id);
uint32 scene_overlay_poll_load(void);

#endif
