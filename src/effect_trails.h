#ifndef EFFECT_TRAILS_H
#define EFFECT_TRAILS_H

#include "psx.h"

void effect_trails_reset_storage(void);
void effect_trails_initialize(void);
void effect_trails_reset(void);
void effect_trails_update(void);
uint32 effect_trails_draw(uint32 ot, uint32 packet);

#endif
