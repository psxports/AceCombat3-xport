#ifndef FLIGHT_DYNAMICS_H
#define FLIGHT_DYNAMICS_H

#include "game_math.h"
#include "mission_actor.h"

enum { FLIGHT_MODEL_OFFSET_COUNT = 31 };

typedef struct
{
    uint32 position[3];
} FLIGHT_MODEL_OFFSET;

extern const FLIGHT_MODEL_OFFSET flight_model_offsets[FLIGHT_MODEL_OFFSET_COUNT];

enum { FLIGHT_TARGET_PROFILE_COUNT = 13 };

typedef struct
{
    uint32 lower[2];
    uint32 extent[2];
} FLIGHT_TARGET_RECT;

typedef struct
{
    uint32 position[3];
    uint16 angles[3];
    FLIGHT_TARGET_RECT inner;
    FLIGHT_TARGET_RECT outer;
} FLIGHT_TARGET_PROFILE;

extern const FLIGHT_TARGET_PROFILE flight_target_profiles[FLIGHT_TARGET_PROFILE_COUNT];

enum { FLIGHT_RELATIVE_PROFILE_COUNT = 4 };

typedef struct
{
    uint32 actor_idx;
    uint32 offset[3];
} FLIGHT_RELATIVE_PROFILE;

extern const FLIGHT_RELATIVE_PROFILE flight_relative_profiles[FLIGHT_RELATIVE_PROFILE_COUNT];

typedef struct
{
    uint32 flags;
    uint32 mode;
    uint32 phase;
    uint32 ticks;
    uint32 events;
    uint16 contact_normal[3];
    GAME_FIXED_VECTOR contact_point;
    uint32 contact_material;
    uint32 speed;
    uint16 control_gain;
    uint16 mode_timer;
    uint16 target_ticks;
    uint16 voice;
    uint16 rumble_ticks;
    uint16 primary_cooldown;
} FLIGHT_DYNAMICS;

extern FLIGHT_DYNAMICS flight_dynamics;

FLIGHT_DYNAMICS *flight_dynamics_enter_standard_control(void);
FLIGHT_DYNAMICS *flight_dynamics_enter_direct_control(void);
FLIGHT_DYNAMICS *flight_dynamics_enter_alternate_control(void);
FLIGHT_DYNAMICS *flight_dynamics_enter_transition_control(void);
FLIGHT_DYNAMICS *flight_dynamics_select_idle_control(uint32 mode);
intptr flight_dynamics_set_sequence_mode(uint32 mode);
intptr flight_dynamics_advance_mode(void);
intptr flight_dynamics_enter_selected_mode(void);
void flight_dynamics_update(void);
uint32 flight_dynamics_flags(void);
void flight_dynamics_emit_contact(const uint32 second[3], uint32 kind, const uint32 *native_vector, const uint32 *native_first);
void flight_dynamics_probe_collision(FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, const MISSION_ACTOR *native_object);
void flight_dynamics_resolve_collision(FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
void flight_dynamics_apply_target_pose(FLIGHT_DYNAMICS *state, const FLIGHT_TARGET_PROFILE *target, MISSION_ACTOR *native_object);
void flight_dynamics_update_object_motion(FLIGHT_DYNAMICS *state, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
void flight_dynamics_update_object_option(FLIGHT_DYNAMICS *state, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
uint32 flight_dynamics_apply_relative_pose(FLIGHT_DYNAMICS *state, const ACTOR_RELATIVE_POSE *relative, const uint32 *target_velocity, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *actor);
void flight_dynamics_update_object_sequence(FLIGHT_DYNAMICS *state, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);

void flight_dynamics_select_sequence(uint32 flags);

uint32 flight_dynamics_relative_pose(const FLIGHT_RELATIVE_PROFILE *selection, const FLIGHT_MODEL_OFFSET *offsets, ACTOR_RELATIVE_POSE *native_output, const MISSION_ACTOR *native_object);

uint32 flight_pose_is_level_above_ground(const NATIVE_POSE *native_position);
void flight_dynamics_reset_event_mode(void);

void flight_enter_engine_off_mode(void);
void flight_apply_player_damage_tick(void);

#endif
