#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include "psx.h"

enum
{
    UI_ANIMATION_CHANNEL_COUNT = 16
};

typedef struct
{
    uint16 ticks;
    uint16 limit;
    uint16 phase;
} UI_ANIMATION_CHANNEL;

void ui_animation_reset(void);
UI_ANIMATION_CHANNEL *ui_animation_channel(uint32 index);
void ui_animation_set(uint32 slot, uint32 phase, sint32 ticks, sint32 limit);
uint32 ui_animation_phase(uint32 slot);
uint32 ui_animation_ticks(uint32 slot);
void ui_animation_tick(void);

#endif
