#ifndef EFFECT_FRAME_H
#define EFFECT_FRAME_H

#include "game_math.h"

typedef struct
{
    uint32 ot;
    uint32 packet;
    GAME_FIXED_VECTOR camera;
    GAME_ANGLES angles;
    uint16 projection;
} EFFECT_FRAME_CONTEXT;

#endif
