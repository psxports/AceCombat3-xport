#ifndef MISSION_TARGET_H
#define MISSION_TARGET_H

#include "psx.h"
#include "audio_player.h"
#include "mission_actor.h"

typedef struct
{
    uint32 retarget_ticks;
    uint32 angle_limit;
    uint32 distance_limit;
    uint32 warning_flags;
    MATRIX direction_matrix;
    AUDIO_AUX_CONTROL aux;
    AUDIO_SPATIAL spatial;
} MISSION_TARGET_STATE;

typedef struct
{
    uint32 ammunition;
    uint32 angle_limit;
    uint32 distance_limit[2];
    uint32 camera_target_index, locked_target_index;
} MISSION_TARGET_FRAME;

extern MISSION_TARGET_STATE mission_target;

void mission_target_update(void);
void mission_target_update_geometry(void);
void mission_target_cycle(void);
void mission_target_acquire(void);
uint32 mission_target_score(uint32 index);
uint32 mission_target_find(uint32 current, uint32 direction, uint32 automatic);

void mission_target_prepare_frame(MISSION_TARGET_FRAME *record);
uint32 mission_target_uses_alternate_weapon(void);

#endif
