#ifndef MISSION_RESULT_H
#define MISSION_RESULT_H

#include <stddef.h>

#include "psx.h"
#include "mission_vm.h"

#pragma pack(push, 1)
typedef struct
{
    uint32 reserved;
    uint32 ground_offset;
    uint32 minimum_y;
    uint32 altitude_limit;
    uint32 minimum_x;
    uint32 maximum_x;
    uint32 minimum_z;
    uint32 maximum_z;
    uint16 warning_limit;
    uint16 alternate_warning_limit;
    uint32 phase_timeout[4];
    uint8 phase_timer_mode[4];
    uint32 phase_initial_ticks[4];
    uint8 phase_message[4];
} MISSION_WORLD_RECORD;
#pragma pack(pop)

typedef char mission_world_record_layout_check[
    sizeof(MISSION_WORLD_RECORD) == 76 &&
    offsetof(MISSION_WORLD_RECORD, warning_limit) == 32 &&
    offsetof(MISSION_WORLD_RECORD, phase_timeout) == 36 &&
    offsetof(MISSION_WORLD_RECORD, phase_timer_mode) == 52 &&
    offsetof(MISSION_WORLD_RECORD, phase_initial_ticks) == 56 &&
    offsetof(MISSION_WORLD_RECORD, phase_message) == 72 ? 1 : -1];

enum { GAME_MISSION_EVENT_SLOTS = 256 };

typedef struct
{
    uint8 kind;
    uint8 category;
    uint16 seconds;
    uint8 x;
    uint8 y;
    uint8 object_index;
} GAME_MISSION_EVENT;

typedef struct
{
    uint8 x;
    uint8 y;
} GAME_TRACE_POINT;

typedef struct
{
    uint16 ticks;
    uint16 duration;
    uint8 active;
    uint8 mode;
} MISSION_FADE;

typedef struct
{
    uint32 flags;
    uint32 score;
    uint32 empty_ammo_ticks;
    uint32 event_fade_strength;
    uint8 outside_warning_ticks;
    uint32 elapsed_ticks;
    uint32 phase_ticks;
    uint32 display_ticks;
    uint16 proximity_count;
    uint16 warning_ticks;
    uint8 proximity_active;
    uint8 category3_count;
    uint8 timer_mode;
    uint8 timer_reserved;
    /* groups correspond to original object flags 0x100 and 0x200 */
    uint8 flagged_target[2];
    uint8 flagged_count[2];
    uint8 script_count;
    uint8 category1_count;
    uint8 category0_4_count;
    uint8 special_result;
    uint8 kind45_count;
    MISSION_FADE fade;
} MISSION_METRICS;

typedef struct
{
    uint8 mode;
    uint8 count_threshold[3];
    uint16 value_threshold[2];
} MISSION_RATING_RULES;

typedef struct
{
    /* original write-only header bytes are separate from unobserved padding */
    uint8 reserved[2];
    uint16 elapsed_ticks;
    uint8 mission;
    uint8 route_mission;
    uint16 sample_count;
    uint8 event_count;
    uint8 rating;
    GAME_TRACE_POINT trace[450];
    GAME_MISSION_EVENT events[GAME_MISSION_EVENT_SLOTS];
    MISSION_RATING_RULES rating_rules;
} GAME_MISSION_RESULT;

void mission_result_init(void);
void mission_result_record_trace(void);
void mission_result_record_event(uint32 index);
void mission_result_finish(void);
uint32 mission_result_last_object(void);
uint32 mission_result_compute_rating(void);

void mission_world_settings_reset(void);
uint32 mission_phase_timeout(uint32 phase);
uint8 mission_phase_message(uint32 phase);
void mission_metrics_initialize(MISSION_VARIANT_INPUT native_variant);
void mission_metrics_begin_phase(void);

void mission_scenario_intro_update(void);
void mission_objectives_update(void);

void mission_objectives_update_scenario(void);

void mission_events_update(void);

void mission_result_scene_stop_hook(void);

#endif
