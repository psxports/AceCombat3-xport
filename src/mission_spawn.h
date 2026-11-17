#ifndef MISSION_SPAWN_H
#define MISSION_SPAWN_H

#include "psx.h"
#include "mission_vm.h"
#include "mission_actor.h"

typedef struct
{
    NATIVE_POSE pose;
} ACTOR_POSE;

void mission_spawn_reset(void);
void mission_player_set_pose(const ACTOR_POSE *native_record, const MISSION_ACTOR *source_actor);
void mission_spawn_apply(void);

typedef enum
{
    MISSION_EVENT_INITIAL,
    MISSION_EVENT_ENTER_MODE_2,
    MISSION_EVENT_ENTER_MODE_8,
    MISSION_EVENT_FIRST_TARGET_COUNT,
    MISSION_EVENT_SECOND_TARGET_COUNT,
    MISSION_EVENT_SIGNAL,
    MISSION_EVENT_ENTER_MODE_16,
    MISSION_EVENT_ENTER_MODE_64,
    MISSION_EVENT_ENTER_MODE_16384,
    MISSION_EVENT_SHARED_MODE,
    MISSION_EVENT_ENTER_MODE_1024,
    MISSION_EVENT_ENTER_MODE_8192,
} MISSION_EVENT_LANE;

void mission_events_reset(void);
MISSION_SCRIPT_REFERENCE mission_event_script(MISSION_EVENT_LANE lane, uint32 index);
MISSION_SCRIPT_REFERENCE mission_event_timed_script(uint32 phase, uint32 slot);
uint16 mission_event_tick(uint32 phase, uint32 slot);
void mission_event_scripts_initialize(MISSION_VARIANT_INPUT native_variant);

void mission_spawn_begin_phase(void);
void mission_player_apply_initial_pose(void);

#endif
