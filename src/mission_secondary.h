#ifndef MISSION_SECONDARY_H
#define MISSION_SECONDARY_H

#include "psx.h"
#include "mission_vm.h"
#include "game_math.h"
#include "mission_actor.h"

typedef struct
{
    uint8 category;
    uint8 kind;
    uint8 model;
    NATIVE_POSE pose;
    uint8 flags;
    uint8 selectors[2];
    uint8 render_mode;
    uint8 attributes_index;
    uint8 weapon_index;
    uint8 motion_index;
    uint8 animation_index;
} MISSION_SECONDARY_PARAMETERS;

typedef struct
{
    uint8 kind;
    uint16 phase;
    uint32 vertical_velocity;
    uint32 magnitude;
    GAME_FIXED_VECTOR waypoint[3];
} MISSION_SECONDARY_MOTION_PROFILE;

enum { SECONDARY_ANIMATION_DURATION_COUNT = 5 };

typedef struct
{
    uint8 has_descriptor;
    sint16 duration[SECONDARY_ANIMATION_DURATION_COUNT];
    uint8 effect, start_mode, end_mode, frame_count;
    uint8 replacement_model, entity_event;
} MISSION_SECONDARY_ANIMATION;

typedef struct
{
    uint8 kind;
    uint8 accuracy;
    uint16 damage;
    uint32 range;
    sint16 min_pitch, max_pitch;
    sint16 min_yaw, max_yaw;
    sint16 shot_interval, attack_ticks, recovery_ticks;
} MISSION_SECONDARY_WEAPON_PROFILE;

typedef struct
{
    MISSION_SECONDARY_PARAMETERS parameters;
    MISSION_ACTOR_DEFINITION definition;
    uint8 has_weapon_profile;
    MISSION_SECONDARY_WEAPON_PROFILE weapon_profile;
    MISSION_SECONDARY_MOTION_PROFILE motion;
    MISSION_SECONDARY_ANIMATION animation;
    MISSION_SCRIPT_REFERENCE destroy_script;
    MISSION_SCRIPT_REFERENCE visible_script;
    MISSION_SCRIPT_REFERENCE stage_script;
} MISSION_SECONDARY_BINDING;

MISSION_SECONDARY_BINDING *mission_secondary_binding(uint32 index);
void mission_secondary_reset(void);
void mission_secondary_bind_resources(const MISSION_ACTOR_CATALOG_INPUT *input);
void mission_secondary_bind_motion_catalog(const uint8 *records, uint32 bytes);
void mission_secondary_select_motion(uint32 actor, uint32 selector);
void mission_secondary_swap_animation(uint32 first_index, uint32 second_index);
void mission_secondary_update_health(uint32 index);
void mission_secondary_initialize(void);
void mission_secondary_animate(uint32 index);
void mission_secondary_stop_idle(uint32 index);
void mission_secondary_move(uint32 index);
void mission_secondary_update_target(uint32 index);
uint32 mission_secondary_fire_particles(uint32 index, uint32 distance);
uint32 mission_secondary_fire_weapon(uint32 index);

void mission_secondary_reset_points(void);

void mission_secondary_reset_signals(void);

void mission_secondary_shuffle_animations(void);

void mission_secondary_update_all(void);

void mission_secondary_update_actor(uint32 index);

void mission_secondary_prepare_scenario_actors(void);
void mission_secondary_activate_actor(uint32 index);

void mission_secondary_update_activity_flags(void);
void mission_secondary_advance_positions(void);
void mission_secondary_tick(void);
void mission_secondary_apply_animation_change(uint32 index);
void mission_secondary_emit_animation_effect(uint32 index);

void mission_secondary_initialize_frame(void);

#endif
