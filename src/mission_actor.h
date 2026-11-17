#ifndef MISSION_ACTOR_H
#define MISSION_ACTOR_H

#include "psx.h"
#include "mission_vm.h"
#include "game_math.h"

enum { MISSION_ACTOR_COUNT = 64, MISSION_PRIMARY_AI_COUNT = 32 };

typedef struct MISSION_ACTOR MISSION_ACTOR;

typedef struct
{
    uint32 flags;
    uint16 angular_rate[3];
    uint32 acceleration[3];
    uint8 behavior_kind;
    uint8 script_phase;
    uint8 behavior_target;
    uint32 behavior_origin[3];
    uint32 activate_radius;
    uint32 deactivate_radius;
    uint32 behavior_ticks;
    uint8 behavior_state;
    uint8 requested_behavior;
    uint8 behavior_phase;
    uint8 behavior_actor;
    uint16 behavior_timer;
    uint32 aim_position[3];
    uint8 fire_flags;
    uint16 fire_ticks;
    uint8 target_actor;
    uint8 speed_mode;
    uint8 maneuver_state;
    uint8 requested_maneuver;
    uint8 maneuver_phase;
    uint16 maneuver_ticks;
    uint32 target_position[3];
    uint16 requested_angles[3];
    uint16 steering_angles[3];
    uint32 origin_distance;
    uint32 origin_direction[3];
    uint16 origin_angle;
    uint32 target_distance;
    uint16 target_angle;
    uint16 target_refresh_ticks;
} MISSION_PRIMARY_AI;

typedef struct
{
    uint8 group;
    uint8 kind;
    uint8 model;
    uint8 attributes_index;
    NATIVE_POSE pose;
    uint8 flags;
    uint8 selectors[2];
    uint8 weapon_index;
    uint8 motion_index;
    uint8 animation_index;
    uint8 effect_index;
    uint32 behavior_words[2];
} MISSION_PRIMARY_PARAMETERS;

typedef struct
{
    uint8 kind;
    uint8 response;
    uint32 min_altitude;
    uint32 max_altitude;
    uint16 lock_ticks;
    uint16 behavior_delay_ticks;
    uint16 turn_delay_ticks;
    uint16 turn_phase_ticks;
    uint16 target_refresh_ticks;
    uint16 firing_ticks;
} MISSION_PRIMARY_MOTION_PROFILE;

typedef struct
{
    uint16 kind;
    uint16 damage;
    uint32 aim_limit;
    uint32 range;
} MISSION_PRIMARY_FIRE_PROFILE;

typedef struct
{
    uint32 kind;
    uint32 cruise_speed;
    uint32 max_speed;
    uint32 min_speed;
    uint32 accel_limit;
    uint16 bank_limit;
    uint16 pitch_limit;
    uint16 roll_rate;
    uint16 pitch_rate_max;
    uint16 pitch_rate_min;
    uint16 yaw_rate;
} MISSION_PRIMARY_CONTROL_PROFILE;

extern const MISSION_PRIMARY_CONTROL_PROFILE mission_primary_control_presets[2];

typedef struct
{
    uint16 health;
    uint8 label;
    uint8 shape;
    uint8 highlight;
    uint8 target_flags;
    uint8 radar_kind;
    uint8 lock_kind;
    uint8 render_mode;
    uint8 damage_mode;
    uint16 radius[2];
} MISSION_ACTOR_DEFINITION;

typedef struct
{
    uint16 mode;
    uint16 damage;
    uint32 range;
} MISSION_PRIMARY_EFFECT_PROFILE;

typedef struct
{
    uint8 kind;
    uint8 target;
    uint8 script_index;
    uint32 origin[3];
    uint32 activate_radius;
    uint32 deactivate_radius;
} MISSION_AI_BEHAVIOR_PROFILE;

typedef struct
{
    MISSION_PRIMARY_PARAMETERS parameters;
    MISSION_ACTOR_DEFINITION definition;
    uint8 has_controls;
    MISSION_PRIMARY_CONTROL_PROFILE controls;
    MISSION_PRIMARY_MOTION_PROFILE motion;
    uint8 has_fire_profile;
    MISSION_PRIMARY_FIRE_PROFILE fire_profile;
    uint8 has_effect_profile;
    MISSION_PRIMARY_EFFECT_PROFILE effect_profile;
    uint32 behavior_index;
    uint32 behavior_words[2];
    uint8 has_behavior_profile;
    MISSION_AI_BEHAVIOR_PROFILE behavior_profile;
    MISSION_SCRIPT_REFERENCE destroy_script;
    MISSION_SCRIPT_REFERENCE visible_script;
    MISSION_SCRIPT_REFERENCE stage_script;
} MISSION_PRIMARY_BINDING;

typedef struct
{
    uint32 velocity[3];
    uint32 local_velocity[3];
    uint16 angular_delta[3];
} MISSION_ACTOR_MOTION;


typedef struct
{
    uint32 flags;
    uint8 model;
    uint8 render_flags;
    uint16 parameter;
    uint16 kind;
    uint16 radius[2];
    uint16 health;
    uint32 damage_source;
    uint32 events;
} MISSION_ACTOR_TRAITS;

struct MISSION_ACTOR
{
    MISSION_ACTOR_TRAITS traits;
    MISSION_ACTOR_MOTION motion;
    NATIVE_POSE pose;
};

typedef struct
{
    uint32 flags;
    uint8 model;
    uint8 render_flags;
    uint16 parameter;
    NATIVE_POSE pose;
} MISSION_ACTOR_SNAPSHOT;

typedef struct
{
    uint32 flags;
    uint32 distance;
    GAME_FIXED_VECTOR direction;
    uint16 angle;
    uint16 lock_ticks;
    uint16 aim_ticks;
    uint16 cone_ticks;
    uint32 projection_flags;
    uint32 visible_ticks;
} MISSION_ACTOR_TARGET;

typedef struct
{
    uint8 display_kind;
    uint8 lock_state;
    uint8 shape_flags;
    uint8 actor_kind;
    uint8 label;
    uint8 affiliation;
    uint16 radius;
    sint16 x;
    sint16 y;
} MISSION_SCREEN_MARKER;

typedef struct
{
    GAME_FIXED_VECTOR reference_position;
    uint32 distance;
    GAME_FIXED_VECTOR displacement;
    GAME_ANGLES angle_delta;
} ACTOR_RELATIVE_POSE;

void mission_actor_reset(void);
MISSION_ACTOR *mission_actor(uint32 index);
MISSION_ACTOR_TRAITS *mission_actor_traits(uint32 index);
MISSION_ACTOR_MOTION *mission_actor_motion(uint32 index);
MISSION_PRIMARY_BINDING *mission_primary_binding(uint32 index);
MISSION_PRIMARY_AI *mission_primary_ai(uint32 index);
void mission_primary_bind_parameters(uint32 index, const uint8 *data);
const NATIVE_POSE *mission_camera_pose(void);
void mission_camera_bind_actor(uint32 actor_index);
void mission_camera_bind_local(const NATIVE_POSE *pose);
void mission_actor_snapshot_capture(MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor);
void mission_actor_snapshot_apply(const MISSION_ACTOR_SNAPSHOT *snapshot, MISSION_ACTOR *actor);
void mission_actor_pause_begin(void);
uint32 mission_actor_pause_restore(uint32 restore);
void mission_actor_stop_motion(uint32 index);
void mission_actor_show_secondary(uint32 index);
uint32 *mission_actor_flags(uint32 index);
MISSION_ACTOR_TARGET *mission_actor_target(uint32 index);
MISSION_SCREEN_MARKER *mission_actor_marker(uint32 index);
void mission_actor_bind_target(uint32 index, uint32 present);
typedef struct
{
    uint8 shape_flags, actor_kind, label, affiliation;
} MISSION_MARKER_SEED;

typedef char mission_marker_seed_layout[sizeof(MISSION_MARKER_SEED) == 4 ? 1 : -1];

void mission_actor_bind_marker(uint32 index, const MISSION_MARKER_SEED *seed);
void mission_actor_initialize_marker(uint32 index, const MISSION_ACTOR_DEFINITION *definition);
void mission_actor_unbind(uint32 index);

void mission_ai_limit_position(uint32 index, uint32 position[3]);

void mission_ai_steer_level_to_position(uint32 index, const uint32 position[3]);

void mission_ai_steer_direction(uint32 index, uint32 speed, GAME_ANGLES *output, const uint32 vector[3]);

uint32 mission_ai_turn_to_position(uint32 index, const uint32 *position);

void mission_ai_decode_maneuver_command(const uint8 *command, MISSION_PRIMARY_AI *state);

void mission_primary_initialize_actors(void);

void mission_ai_update_pitch_maneuver(uint32 index);
void mission_primary_prepare_scenario_actors(void);

void mission_ai_update_behavior_00(uint32 index);

void mission_ai_update_behavior_01(uint32 index);

void mission_ai_update_behavior_02(uint32 index);

void mission_ai_update_behavior_03(uint32 index);

void mission_ai_update_behavior_04(uint32 index);

void mission_ai_update_behavior_05(uint32 index);

void mission_ai_update_behavior_07(uint32 index);

void mission_ai_update_behavior_10(uint32 index);

void mission_ai_update_behavior_11(uint32 index);

void mission_ai_update_behavior_12(uint32 index);

void mission_ai_update_behavior_14(uint32 index);

void mission_ai_update_behavior_15(uint32 index);

void mission_ai_update_behavior_16(uint32 index);

void mission_ai_update_behavior_17(uint32 index);

void mission_ai_update_behavior_18(uint32 index);

void mission_ai_update_behavior_19(uint32 index);

void mission_ai_update_behavior_20(uint32 index);

void mission_ai_update_behavior_21(uint32 index);

void mission_ai_update_behavior_22(uint32 index);

void mission_ai_update_behaviors(void);

void mission_ai_update_maneuver_00(uint32 index);

void mission_ai_maneuver_05_standard(uint32 index);

void mission_ai_update_maneuver_40(uint32 index);

void mission_ai_update_maneuver_41(uint32 index);

void mission_ai_maneuver_06(uint32 index);

void mission_ai_update_maneuver_12(uint32 index);

void mission_ai_update_maneuver_30(uint32 index);

void mission_ai_update_maneuver_42(uint32 index);

void mission_ai_update_maneuver_43(uint32 index);

void mission_ai_update_maneuver_07(uint32 index);

void mission_ai_maneuver_05_kind3(uint32 index);

void mission_ai_update_maneuver_08(uint32 index);

void mission_ai_update_maneuver_09(uint32 index);

void mission_ai_update_maneuver_51(uint32 index);

void mission_ai_update_maneuver_10(uint32 index);

void mission_ai_update_maneuver_11(uint32 index);

void mission_ai_update_maneuver_13(uint32 index);

void mission_ai_update_maneuver_14(uint32 index);

void mission_ai_update_maneuver_25(uint32 index);

void mission_ai_update_maneuver_26(uint32 index);

void mission_ai_update_maneuver_27(uint32 index);

void mission_ai_update_maneuver_31(uint32 index);

void mission_ai_update_maneuver_52(uint32 index);

void mission_ai_maneuver_04_alternate(uint32 index);

void mission_ai_update_maneuvers(void);

void mission_ai_maneuver_follow_direction(uint32 index);

void mission_ai_apply_steering_delta(uint32 index);

void mission_ai_update_speed(uint32 index);

void mission_target_links_initialize(void);

void mission_target_links_update(void);

void mission_actor_update_threat_flags(void);

void mission_target_links_remove_actor(uint32 index);

uint32 mission_target_links_release(uint32 index);

void mission_ai_update_target_selection(void);

uint32 mission_target_links_assign(uint32 index, uint32 target);

uint32 mission_ai_find_target(uint32 index);

void mission_actor_finish_deactivation(void);

void mission_actor_update_deactivation(void);

void mission_actor_begin_deactivation(uint32 index);

void mission_secondary_begin_deactivation(uint32 index);

void mission_ai_update_maneuver_script(uint32 index);

void mission_primary_restart_scenario_ai(void);

void mission_ai_update_path_script(uint32 index);

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} MISSION_ACTOR_CATALOG_VIEW;

typedef struct
{
    MISSION_ACTOR_CATALOG_VIEW primary_parameters;
    MISSION_ACTOR_CATALOG_VIEW secondary_parameters;
    MISSION_ACTOR_CATALOG_VIEW definitions;
    MISSION_ACTOR_CATALOG_VIEW primary_motion;
    MISSION_ACTOR_CATALOG_VIEW primary_fire;
    MISSION_ACTOR_CATALOG_VIEW secondary_animation;
    MISSION_ACTOR_CATALOG_VIEW primary_effect;
    MISSION_ACTOR_CATALOG_VIEW secondary_weapon;
} MISSION_ACTOR_CATALOG_INPUT;

const uint8 *mission_actor_catalog_record(const MISSION_ACTOR_CATALOG_VIEW *view, uint32 offset, uint32 bytes);

void mission_primary_bind_resources(const MISSION_ACTOR_CATALOG_INPUT *input);
void mission_primary_select_controls(uint32 actor, uint32 mode);
MISSION_PRIMARY_CONTROL_PROFILE mission_primary_controls(uint32 actor);

uint32 mission_ai_advance_behavior_profile(uint32 index);

uint32 mission_ai_update_effect_fire(uint32 index);

void mission_actor_update_damage(void);

void mission_actor_bind_scene_resources(MISSION_VARIANT_INPUT native_variant);
void mission_entity_events_apply_initial(void);
void mission_entity_events_trigger(uint32 id);

void mission_ai_update_weapon_fire(uint32 index);

void mission_actor_tick_primary(void);

uint32 mission_actor_classify_world_boundary(uint32 index);

void mission_actor_tick_damage(void);

void mission_primary_show_actor(uint32 index);
void mission_primary_activate_actor(uint32 index);

void mission_actor_advance_position(const MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);

uint32 mission_ai_request_level_angles(uint32 index);
uint32 mission_ai_step_level_attitude(uint32 index);

uint32 mission_ai_update_roll_rate(uint32 index, uint32 value);
uint32 mission_ai_update_pitch_rate(uint32 index, uint32 value);
uint32 mission_ai_limit_yaw_rate(uint32 index, uint32 value);
void mission_actor_update_target_geometry(void);
void mission_ai_update_target_geometry(void);
void mission_actor_update_target_timers(void);
void mission_actor_update_links_and_trace(void);

void mission_actor_camera_target_angles(uint32 index, uint16 angles[3], const NATIVE_POSE *origin);

void mission_actor_clear_script_flags(void);

void mission_primary_initialize_menu_actor(void);
void mission_actor_initialize_target_state(uint32 index);

#endif
