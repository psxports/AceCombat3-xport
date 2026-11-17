#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "game_functions.h"

#pragma pack(push, 1)

typedef struct
{
    uint8 kind;
    uint8 target;
    uint8 script_index;
    uint8 unknown3[5];
    uint32 origin[3];
    uint8 unknown20[4];
    uint32 activate_radius;
    uint32 deactivate_radius;
} MISSION_AI_BEHAVIOR_RECORD;

typedef struct
{
    uint8 unknown0;
    uint8 kind;
    uint8 unknown2[6];
    uint32 target_position[3];
    uint8 unknown20[4];
    uint32 packed_angles01;
    uint16 angle2;
    uint8 unknown30[2];
} MISSION_AI_MANEUVER_RECORD;

#pragma pack(pop)

typedef char mission_ai_command_wire_layout_check[sizeof(MISSION_AI_BEHAVIOR_RECORD) == 32 && (offsetof(MISSION_AI_BEHAVIOR_RECORD, origin) + 0 * sizeof(((MISSION_AI_BEHAVIOR_RECORD *)0)->origin[0])) == 8 && (offsetof(MISSION_AI_BEHAVIOR_RECORD, origin) + 1 * sizeof(((MISSION_AI_BEHAVIOR_RECORD *)0)->origin[0])) == 12 && (offsetof(MISSION_AI_BEHAVIOR_RECORD, origin) + 2 * sizeof(((MISSION_AI_BEHAVIOR_RECORD *)0)->origin[0])) == 16 && offsetof(MISSION_AI_BEHAVIOR_RECORD, activate_radius) == 24 && offsetof(MISSION_AI_BEHAVIOR_RECORD, deactivate_radius) == 28 && offsetof(MISSION_AI_BEHAVIOR_RECORD, kind) == 0 && offsetof(MISSION_AI_BEHAVIOR_RECORD, target) == 1 && offsetof(MISSION_AI_BEHAVIOR_RECORD, script_index) == 2 && sizeof(MISSION_AI_MANEUVER_RECORD) == 32 && (offsetof(MISSION_AI_MANEUVER_RECORD, target_position) + 0 * sizeof(((MISSION_AI_MANEUVER_RECORD *)0)->target_position[0])) == 8 &&
                                                          (offsetof(MISSION_AI_MANEUVER_RECORD, target_position) + 1 * sizeof(((MISSION_AI_MANEUVER_RECORD *)0)->target_position[0])) == 12 && (offsetof(MISSION_AI_MANEUVER_RECORD, target_position) + 2 * sizeof(((MISSION_AI_MANEUVER_RECORD *)0)->target_position[0])) == 16 && offsetof(MISSION_AI_MANEUVER_RECORD, packed_angles01) == 24 && offsetof(MISSION_AI_MANEUVER_RECORD, angle2) == 28 && offsetof(MISSION_AI_MANEUVER_RECORD, kind) == 1
                                                      ? 1
                                                      : -1];

typedef char mission_native_value_codec_layout_check[sizeof(MISSION_PRIMARY_CONTROL_PROFILE) == 32 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, kind) == 0 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, cruise_speed) == 4 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, max_speed) == 8 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, min_speed) == 12 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, accel_limit) == 16 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, bank_limit) == 20 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, pitch_limit) == 22 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, roll_rate) == 24 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, pitch_rate_max) == 26 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, pitch_rate_min) == 28 && offsetof(MISSION_PRIMARY_CONTROL_PROFILE, yaw_rate) == 30 && sizeof(NATIVE_POSE) == 20 && offsetof(NATIVE_POSE, position) == 0 && offsetof(NATIVE_POSE, angles) == 12 && sizeof(((NATIVE_POSE *)0)->position) == 12 && sizeof(((NATIVE_POSE *)0)->angles) == 6 ? 1 : -1];

static void mission_actor_mark_primary_destroyed(uint32 index);
static uint32 mission_ai_classify_max_altitude(uint32 index);
static uint32 mission_ai_classify_min_altitude(uint32 index);
static void mission_ai_update_behavior_radius_flags(uint32 index);
static void mission_ai_request_scenario_behavior(uint32 index);
static void mission_ai_apply_disabled_behavior(uint32 index);

static void mission_ai_tick_primary(void);
static void mission_actor_finish_primary_tick(void);
static void mission_actor_process_primary_deaths(void);
static void mission_ai_advance_primary_profiles(void);
static void mission_ai_process_fire_requests(void);
static void mission_ai_acquire_target(uint32 index);
static void mission_ai_follow_behavior_target(uint32 index);
static void mission_ai_request_fallback_aim(uint32 index);
static void mission_ai_update_timed_target(uint32 index);

enum
{
    AI_MANEUVER_CATALOG_COUNT = 16,
    AI_PATH_CATALOG_COUNT = 4,
    AI_PATH_MANEUVER_ALIAS = 160
};

enum
{
    ENTITY_SELECTOR_COUNT = 32,
    ENTITY_EVENT_BYTES = 8,
    ENTITY_EVENT_CAPACITY = 153
};

typedef struct
{
    uint16 marker;
    uint8 kind, tile;
    sint16 initial_value, trigger_value;
} MISSION_ENTITY_EVENT;

typedef enum
{
    ENTITY_EVENT_INITIAL = 4,
    ENTITY_EVENT_TRIGGER = 6
} MISSION_ENTITY_EVENT_MODE;

typedef char mission_entity_event_value_layout_check[sizeof(MISSION_ENTITY_EVENT) == ENTITY_EVENT_BYTES && ENTITY_EVENT_BYTES == 8 && offsetof(MISSION_ENTITY_EVENT, marker) == 0 && offsetof(MISSION_ENTITY_EVENT, kind) == 2 && offsetof(MISSION_ENTITY_EVENT, tile) == 3 && offsetof(MISSION_ENTITY_EVENT, initial_value) == ENTITY_EVENT_INITIAL && ENTITY_EVENT_INITIAL == 4 && offsetof(MISSION_ENTITY_EVENT, trigger_value) == ENTITY_EVENT_TRIGGER && ENTITY_EVENT_TRIGGER == 6 ? 1 : -1];

typedef struct
{
    uint16 pitch_bias;
    uint32 response_shift;
} MISSION_AI_TURN_PROFILE;

static const MISSION_AI_TURN_PROFILE mission_ai_turn_profiles[4] = {{512, 6}, {1024, 7}, {2048, 8}, {4096, 10}};

typedef struct
{
    uint8 active, target;
    uint8 attacker[2];
} MISSION_TARGET_LINKS;

typedef struct
{
    uint32 x, z;
} MISSION_AI_ENTRY_POSITION;

static const MISSION_AI_ENTRY_POSITION mission_ai_descent_entries[6] = {
    {0x11400000, 0x0F000000}, {0x11C00000, 0x0F400000}, {0x12400000, 0x0F800000}, {0xFD400000, 0xF3800000}, {0xFDC00000, 0xF3C00000}, {0xFE400000, 0xF3800000},
};

typedef struct
{
    GAME_CAMERA_PROJ camera_proj;
    GAME_CAMERA_CONTROL camera_control;
    GAME_CAMERA_FOLLOW camera_follow;
    GAME_CAMERA_VIEW camera_view;
    GAME_CAMERA_SHOTS camera_shots;
    GAME_CAMERA_DELTA camera_delta;
    const NATIVE_POSE *camera_pose;
    uint32 arena_cursor;
    MISSION_ACTOR_SNAPSHOT actors[MISSION_ACTOR_COUNT];
} MISSION_ACTOR_PAUSE;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 guest_base;
} AI_SCRIPT_VIEW;

enum
{
    AI_BEHAVIOR_RECORD_BYTES = 32,
    AI_BEHAVIOR_SELECTOR_COUNT = 256
};

typedef struct
{
    MISSION_ACTOR_PAUSE pause;
    const NATIVE_POSE *camera_pose;
    MISSION_ACTOR actors[MISSION_ACTOR_COUNT];
    MISSION_PRIMARY_BINDING primary[32];
    MISSION_PRIMARY_AI primary_ai[MISSION_PRIMARY_AI_COUNT - 2];
    MISSION_AI_BEHAVIOR_PROFILE behavior_catalog[AI_BEHAVIOR_SELECTOR_COUNT];
    uint32 behavior_catalog_count;
    MISSION_PRIMARY_CONTROL_PROFILE control_catalog[256];
    uint32 control_catalog_count;
    AI_SCRIPT_VIEW maneuver_scripts[AI_MANEUVER_CATALOG_COUNT];
    AI_SCRIPT_VIEW path_scripts[AI_PATH_CATALOG_COUNT];
    ACTOR_POSE maneuver_history[64];
    uint8 maneuver_history_alive[64];
    uint8 entity_selectors[ENTITY_SELECTOR_COUNT];
    uint8 entity_events_ready;
    MISSION_ENTITY_EVENT entity_events[ENTITY_EVENT_CAPACITY];
    MISSION_TARGET_LINKS target_links[MISSION_ACTOR_COUNT];
    uint8 deactivation_ticks[MISSION_ACTOR_COUNT];
    uint32 flags[MISSION_ACTOR_COUNT];
    MISSION_ACTOR_TARGET targets[MISSION_ACTOR_COUNT];
    MISSION_SCREEN_MARKER markers[MISSION_ACTOR_COUNT];
    MISSION_ACTOR_TARGET *target[MISSION_ACTOR_COUNT];
    MISSION_SCREEN_MARKER *marker[MISSION_ACTOR_COUNT];
} MISSION_ACTOR_STATE;

static MISSION_ACTOR_STATE mission_actor_state;

static MISSION_AI_BEHAVIOR_PROFILE mission_ai_decode_behavior_profile(const uint8 *record)
{
    const MISSION_AI_BEHAVIOR_RECORD *wire = (const MISSION_AI_BEHAVIOR_RECORD *)record;
    MISSION_AI_BEHAVIOR_PROFILE profile;

    profile.kind = wire->kind;
    profile.target = wire->target;
    profile.script_index = wire->script_index;
    profile.origin[0] = wire->origin[0];
    profile.origin[1] = wire->origin[1];
    profile.origin[2] = wire->origin[2];
    profile.activate_radius = wire->activate_radius;
    profile.deactivate_radius = wire->deactivate_radius;
    return profile;
}

static void mission_ai_select_behavior_profile(MISSION_PRIMARY_BINDING *binding, uint32 selector)
{
    binding->has_behavior_profile = selector < mission_actor_state.behavior_catalog_count;
    if (binding->has_behavior_profile)
    {
        binding->behavior_profile = mission_actor_state.behavior_catalog[selector];
    }
}

static MISSION_AI_BEHAVIOR_PROFILE mission_ai_selected_behavior_profile(const MISSION_PRIMARY_BINDING *binding)
{
    if (!binding->has_behavior_profile)
        abort();
    return binding->behavior_profile;
}

void mission_actor_reset(void)
{
    memset(&mission_actor_state, 0, sizeof(mission_actor_state));
}

uint32 *mission_actor_flags(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return &mission_actor_state.flags[index];
}

MISSION_ACTOR_TARGET *mission_actor_target(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return mission_actor_state.target[index];
}

MISSION_SCREEN_MARKER *mission_actor_marker(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return mission_actor_state.marker[index];
}

void mission_actor_bind_target(uint32 index, uint32 present)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    mission_actor_state.target[index] = present ? &mission_actor_state.targets[index] : NULL;
}

void mission_actor_bind_marker(uint32 index, const MISSION_MARKER_SEED *seed)
{
    MISSION_SCREEN_MARKER *marker;
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    if (seed == NULL)
    {
        mission_actor_state.marker[index] = NULL;
        return;
    }
    marker = &mission_actor_state.markers[index];
    marker->shape_flags = seed->shape_flags;
    marker->actor_kind = seed->actor_kind;
    marker->label = seed->label;
    marker->affiliation = seed->affiliation;
    mission_actor_state.marker[index] = marker;
}

void mission_actor_unbind(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    mission_actor_state.marker[index] = NULL;
    mission_actor_state.target[index] = NULL;
}

void mission_actor_stop_motion(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 mode;
    FUNCTION_MARKER(0x8004A018, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    mode = ai_owner_state->maneuver_phase;
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (mode == 1)
        return;
    if (mode >= 2)
    {
        if (mode == 2)
            ai_owner_state->requested_maneuver = (uint8)(255);
        else
            ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    ai_owner_state->maneuver_phase = (uint8)(1);
    actor_motion_object->velocity[0] = (uint32)(0);
    actor_motion_object->velocity[1] = (uint32)(0);
    actor_motion_object->velocity[2] = (uint32)(0);
    actor_motion_object->local_velocity[0] = (uint32)(0);
    actor_motion_object->local_velocity[1] = (uint32)(0);
    actor_motion_object->local_velocity[2] = (uint32)(0);
    ai_owner_state->steering_angles[0] = (uint16)(0);
    ai_owner_state->steering_angles[1] = (uint16)(0);
    ai_owner_state->steering_angles[2] = (uint16)(0);
    return;
}

void mission_actor_show_secondary(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_SECONDARY_BINDING *record;
    uint32 actor_index_slot;
    uint32 actor_index_table;

    uint32 value;
    FUNCTION_MARKER(0x8004E900, "ACE3.BIN");

    actor_index_table = 0;
    record = mission_secondary_binding(index);
    actor_index_slot = (actor_index_table + ((uint32)((sint32)((index << 2)) >> 2)));
    value = record->parameters.model;
    actor_traits_object = mission_actor_traits((actor_index_slot + 32));
    if (value == 255)
        return;
    value = actor_traits_object->flags | 0x10000;
    actor_traits_object->flags = (uint32)(value);
    return;
}

void mission_actor_snapshot_capture(MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor)
{
    const MISSION_ACTOR_TRAITS *actor_traits = &actor->traits;
    snapshot->flags = actor_traits->flags;
    snapshot->model = actor_traits->model;
    snapshot->render_flags = actor_traits->render_flags;
    snapshot->parameter = actor_traits->parameter;
    snapshot->pose.position[0] = actor->pose.position[0];
    snapshot->pose.position[1] = actor->pose.position[1];
    snapshot->pose.position[2] = actor->pose.position[2];
    snapshot->pose.angles[0] = actor->pose.angles[0];
    snapshot->pose.angles[1] = actor->pose.angles[1];
    snapshot->pose.angles[2] = actor->pose.angles[2];
}

void mission_actor_snapshot_apply(const MISSION_ACTOR_SNAPSHOT *snapshot, MISSION_ACTOR *actor)
{
    MISSION_ACTOR_TRAITS *actor_traits = &actor->traits;
    uint32 angles;
    actor_traits->flags = snapshot->flags;
    actor_traits->model = snapshot->model;
    actor_traits->render_flags = snapshot->render_flags;
    actor_traits->parameter = snapshot->parameter;
    actor->pose.position[0] = (uint32)(snapshot->pose.position[0]);
    actor->pose.position[1] = (uint32)(snapshot->pose.position[1]);
    actor->pose.position[2] = (uint32)(snapshot->pose.position[2]);
    angles = snapshot->pose.angles[0] | ((uint32)snapshot->pose.angles[1] << 16);
    {
        uint32 pose_word = (angles);
        actor->pose.angles[0] = (uint16)pose_word;
        actor->pose.angles[1] = (uint16)(pose_word >> 16);
    }
    angles = snapshot->pose.angles[2];
    {
        uint32 pose_word = (angles);
        actor->pose.angles[2] = (uint16)pose_word;
    }
}

void mission_actor_pause_begin(void)
{
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_ACTOR_PAUSE *pause = &mission_actor_state.pause;
    uint32 flags, pointer, index;
    FUNCTION_MARKER(0x80038C78, "ACE3.BIN");
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x10000000;
    effect_objects_reset();
    pointer = arena_peek(0);
    pause->arena_cursor = pointer;
    (void)arena_alloc(2444);
    pause->camera_proj = game_runtime.camera_proj;
    pause->camera_control = game_runtime.camera_control;
    pause->camera_follow = game_runtime.camera_follow;
    pause->camera_view = game_runtime.camera_view;
    pause->camera_shots = game_runtime.camera_shots;
    pause->camera_delta = game_runtime.camera_delta;
    pause->camera_pose = mission_camera_pose();
    for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
    {
        actor_object = mission_actor(index);
        actor_traits_object = &actor_object->traits;
        mission_actor_snapshot_capture(&pause->actors[index], actor_object);
    }
    return;
}

uint32 mission_actor_pause_restore(uint32 restore)
{
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    MISSION_ACTOR_PAUSE *pause = &mission_actor_state.pause;
    uint32 near_value, far_value, height, index, cursor, flags;
    FUNCTION_MARKER(0x80038D1C, "ACE3.BIN");
    game_runtime.camera_proj = pause->camera_proj;
    game_runtime.camera_control = pause->camera_control;
    game_runtime.camera_follow = pause->camera_follow;
    game_runtime.camera_view = pause->camera_view;
    game_runtime.camera_shots = pause->camera_shots;
    game_runtime.camera_delta = pause->camera_delta;
    mission_actor_state.camera_pose = pause->camera_pose;
    near_value = ((sint16)game_runtime.camera_proj.near_plane);
    far_value = ((sint16)game_runtime.camera_proj.far_plane);
    height = ((sint16)game_runtime.camera_proj.distance);
    camera_set_clip(&game_runtime.camera_proj, near_value, far_value, height, game_runtime.mission_depth_buckets);
    if (restore != 0)
    {
        for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
        {
            actor_object = mission_actor(index);
            actor_traits_object = &actor_object->traits;
            actor_traits_object->flags = (uint32)(pause->actors[index].flags);
            if (index == 0)
                mission_player_set_pose(NULL, actor_object);
        }
    }
    else
    {
        for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
        {
            actor_object = mission_actor(index);
            actor_traits_object = &actor_object->traits;
            mission_actor_snapshot_apply(&pause->actors[index], actor_object);
        }
    }
    cursor = pause->arena_cursor;
    arena_set_cursor(cursor);
    effect_pools_reset();
    flags = game_runtime.flags;
    game_runtime.flags = flags & 0xEFFFFFFF;
    effect_objects_reset();
    return mission_dispatch_scene_request();
}

void mission_primary_bind_parameters(uint32 index, const uint8 *data)
{
    uint32 wire_word_0;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    MISSION_PRIMARY_PARAMETERS *parameters = &mission_primary_binding(index)->parameters;
    uint32 angles, i;
    parameters->group = data[0];
    parameters->kind = data[1];
    parameters->model = data[2];
    parameters->attributes_index = data[3];
    for (i = 0; i < 3; ++i)
        parameters->pose.position[i] = (memcpy(&wire_word_0, data + 4 + i * 4, sizeof(wire_word_0)), wire_word_0);
    angles = (memcpy(&wire_word_2, data + 20, sizeof(wire_word_2)), wire_word_2);
    parameters->pose.angles[0] = (uint16)angles;
    parameters->pose.angles[1] = (uint16)(angles >> 16);
    angles = (memcpy(&wire_word_3, data + 24, sizeof(wire_word_3)), wire_word_3);
    parameters->pose.angles[2] = (uint16)angles;
    parameters->flags = data[28];
    parameters->selectors[0] = data[29];
    parameters->selectors[1] = data[30];
    parameters->weapon_index = data[32];
    parameters->motion_index = data[33];
    parameters->animation_index = data[34];
    parameters->effect_index = data[35];
    parameters->behavior_words[0] = (memcpy(&wire_word_4, data + 36, sizeof(wire_word_4)), wire_word_4);
    parameters->behavior_words[1] = (memcpy(&wire_word_5, data + 40, sizeof(wire_word_5)), wire_word_5);
}

MISSION_ACTOR *mission_actor(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return &mission_actor_state.actors[index];
}

MISSION_ACTOR_TRAITS *mission_actor_traits(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return &mission_actor_state.actors[index].traits;
}

MISSION_ACTOR_MOTION *mission_actor_motion(uint32 index)
{
    if (index >= MISSION_ACTOR_COUNT)
        abort();
    return &mission_actor_state.actors[index].motion;
}

static uint32 mission_primary_engagement_range(const MISSION_PRIMARY_BINDING *binding)
{
    return binding->has_fire_profile ? binding->fire_profile.range : 0;
}

MISSION_PRIMARY_BINDING *mission_primary_binding(uint32 index)
{
    if (index >= 32)
        abort();
    return &mission_actor_state.primary[index];
}

const NATIVE_POSE *mission_camera_pose(void)
{
    return mission_actor_state.camera_pose;
}

void mission_camera_bind_actor(uint32 actor_index)
{
    if (actor_index >= MISSION_ACTOR_COUNT)
        abort();
    mission_actor_state.camera_pose = &mission_actor(actor_index)->pose;
}

void mission_camera_bind_local(const NATIVE_POSE *pose)
{
    mission_actor_state.camera_pose = pose;
}

MISSION_PRIMARY_AI *mission_primary_ai(uint32 index)
{
    if (index < 2 || index >= MISSION_PRIMARY_AI_COUNT)
        abort();
    return &mission_actor_state.primary_ai[index - 2];
}

static void mission_primary_ai_import_initial(const uint8 *data, uint32 active_count, const MISSION_ACTOR_CATALOG_VIEW *parameters)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint16 wire_halfword_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint16 wire_halfword_12;
    uint32 wire_word_13;
    uint16 wire_halfword_14;
    uint16 wire_halfword_15;
    uint16 wire_halfword_16;
    uint16 wire_halfword_17;
    uint32 wire_word_18;
    uint16 wire_halfword_19;
    uint16 wire_halfword_20;
    uint16 wire_halfword_21;
    uint32 wire_word_22;
    uint32 wire_word_23;
    uint32 wire_word_24;
    uint32 wire_word_25;
    uint32 wire_word_26;
    uint32 wire_word_27;
    uint32 wire_word_28;
    uint32 wire_word_29;
    uint32 wire_word_30;
    uint16 wire_halfword_31;
    uint16 wire_halfword_32;
    uint16 wire_halfword_33;
    uint16 wire_halfword_34;
    uint16 wire_halfword_35;
    uint16 wire_halfword_36;
    uint32 index;
    const uint8 *source;
    MISSION_PRIMARY_AI *state;
    for (index = 2; index < MISSION_PRIMARY_AI_COUNT; ++index)
    {
        state = mission_primary_ai(index);
        source = data + index * 184u;
        state->aim_position[0] = (memcpy(&wire_word_0, source + 68, sizeof(wire_word_0)), wire_word_0);
        state->aim_position[1] = (memcpy(&wire_word_1, source + 72, sizeof(wire_word_1)), wire_word_1);
        state->aim_position[2] = (memcpy(&wire_word_2, source + 76, sizeof(wire_word_2)), wire_word_2);
        if (index >= active_count || mission_actor_catalog_record(parameters, index * 44u, 44)[35] != 255)
            state->fire_flags = source[92];
        if (index >= active_count)
            state->fire_ticks = (memcpy(&wire_halfword_4, source + 94, sizeof(wire_halfword_4)), wire_halfword_4);
        else
            state->fire_ticks = 0;
        state->target_position[0] = (memcpy(&wire_word_5, source + 104, sizeof(wire_word_5)), wire_word_5);
        state->target_position[1] = (memcpy(&wire_word_6, source + 108, sizeof(wire_word_6)), wire_word_6);
        state->target_position[2] = (memcpy(&wire_word_7, source + 112, sizeof(wire_word_7)), wire_word_7);
        state->origin_distance = (memcpy(&wire_word_8, source + 136, sizeof(wire_word_8)), wire_word_8);
        state->origin_direction[0] = (memcpy(&wire_word_9, source + 140, sizeof(wire_word_9)), wire_word_9);
        state->origin_direction[1] = (memcpy(&wire_word_10, source + 144, sizeof(wire_word_10)), wire_word_10);
        state->origin_direction[2] = (memcpy(&wire_word_11, source + 148, sizeof(wire_word_11)), wire_word_11);
        state->origin_angle = (memcpy(&wire_halfword_12, source + 156, sizeof(wire_halfword_12)), wire_halfword_12);
        state->target_distance = (memcpy(&wire_word_13, source + 160, sizeof(wire_word_13)), wire_word_13);
        state->target_angle = (memcpy(&wire_halfword_14, source + 180, sizeof(wire_halfword_14)), wire_halfword_14);
        /* active NPCs receive these fields before their first read */
        if (index < 2 || index >= active_count)
        {
            state->requested_angles[0] = (memcpy(&wire_halfword_15, source + 120, sizeof(wire_halfword_15)), wire_halfword_15);
            state->requested_angles[1] = (memcpy(&wire_halfword_16, source + 122, sizeof(wire_halfword_16)), wire_halfword_16);
            state->requested_angles[2] = (memcpy(&wire_halfword_17, source + 124, sizeof(wire_halfword_17)), wire_halfword_17);
            state->flags = (memcpy(&wire_word_18, source, sizeof(wire_word_18)), wire_word_18);
            state->angular_rate[0] = (memcpy(&wire_halfword_19, source + 4, sizeof(wire_halfword_19)), wire_halfword_19);
            state->angular_rate[1] = (memcpy(&wire_halfword_20, source + 6, sizeof(wire_halfword_20)), wire_halfword_20);
            state->angular_rate[2] = (memcpy(&wire_halfword_21, source + 8, sizeof(wire_halfword_21)), wire_halfword_21);
            state->acceleration[0] = (memcpy(&wire_word_22, source + 12, sizeof(wire_word_22)), wire_word_22);
            state->acceleration[1] = (memcpy(&wire_word_23, source + 16, sizeof(wire_word_23)), wire_word_23);
            state->acceleration[2] = (memcpy(&wire_word_24, source + 20, sizeof(wire_word_24)), wire_word_24);
            state->behavior_kind = source[28];
            state->script_phase = source[29];
            state->behavior_target = source[31];
            state->behavior_origin[0] = (memcpy(&wire_word_25, source + 32, sizeof(wire_word_25)), wire_word_25);
            state->behavior_origin[1] = (memcpy(&wire_word_26, source + 36, sizeof(wire_word_26)), wire_word_26);
            state->behavior_origin[2] = (memcpy(&wire_word_27, source + 40, sizeof(wire_word_27)), wire_word_27);
            state->activate_radius = (memcpy(&wire_word_28, source + 48, sizeof(wire_word_28)), wire_word_28);
            state->deactivate_radius = (memcpy(&wire_word_29, source + 52, sizeof(wire_word_29)), wire_word_29);
            state->behavior_ticks = (memcpy(&wire_word_30, source + 56, sizeof(wire_word_30)), wire_word_30);
            state->behavior_state = source[60];
            state->requested_behavior = source[61];
            state->behavior_phase = source[62];
            state->behavior_actor = source[63];
            state->behavior_timer = (memcpy(&wire_halfword_31, source + 64, sizeof(wire_halfword_31)), wire_halfword_31);
            state->target_actor = source[96];
            state->speed_mode = source[97];
            state->maneuver_state = source[98];
            state->requested_maneuver = source[99];
            state->maneuver_phase = source[100];
            state->maneuver_ticks = (memcpy(&wire_halfword_32, source + 102, sizeof(wire_halfword_32)), wire_halfword_32);
            state->steering_angles[0] = (memcpy(&wire_halfword_33, source + 128, sizeof(wire_halfword_33)), wire_halfword_33);
            state->steering_angles[1] = (memcpy(&wire_halfword_34, source + 130, sizeof(wire_halfword_34)), wire_halfword_34);
            state->steering_angles[2] = (memcpy(&wire_halfword_35, source + 132, sizeof(wire_halfword_35)), wire_halfword_35);
            state->target_refresh_ticks = (memcpy(&wire_halfword_36, source + 182, sizeof(wire_halfword_36)), wire_halfword_36);
        }
    }
}

void mission_actor_initialize_marker(uint32 index, const MISSION_ACTOR_DEFINITION *definition)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 kind, value, flags, result;
    MISSION_SCREEN_MARKER *state;
    FUNCTION_MARKER(0x8004EBA4, "ACE3.BIN");
    state = mission_actor_marker(index);
    if (state == 0)
        return;
    actor_index_table = 0;
    kind = definition->radar_kind;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (kind == 0)
        state->shape_flags = (uint8)(0);
    else if ((definition->target_flags & 2) == 0)
        state->shape_flags = (uint8)(3);
    else
    {
        value = definition->shape;
        if (value == 0)
            state->shape_flags = (uint8)(1);
        else if (value == 1)
            state->shape_flags = (uint8)(2);
    }
    if (definition->highlight == 1)
        state->shape_flags = (uint8)(state->shape_flags | 0x80);
    state->label = (uint8)(definition->label);
    state->actor_kind = (uint8)((uint8)actor_traits_object->kind);
    result = (*mission_actor_flags(index)) & 0x100;
    if (result != 0)
    {
        state->affiliation = (uint8)(1);
        {
            (void)(uint32)(result);
            return;
        }
    }
    flags = actor_traits_object->flags;
    result = flags & 0x100;
    if ((flags & 0x40) != 0)
    {
        state->affiliation = (uint8)(3);
        return;
    }
    if (result != 0)
    {
        state->affiliation = (uint8)(2);
        return;
    }
    state->affiliation = (uint8)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

static void mission_ai_limit_altitude(uint32 index, uint32 position[3])
{
    const MISSION_PRIMARY_MOTION_PROFILE *motion_base;
    uint32 limit, value, result;
    FUNCTION_MARKER(0x80050690, "ACE3.BIN");
    motion_base = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    limit = motion_base->max_altitude;
    value = position[1];
    if ((sint32)limit < (sint32)value)
    {
        position[1] = (limit);
    }
    motion_base = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    limit = motion_base->min_altitude;
    value = position[1];
    result = (sint32)value < (sint32)limit;
    if (result)
    {
        position[1] = (limit);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_limit_position(uint32 index, uint32 position[3])
{
    uint32 limit, value, result;
    FUNCTION_MARKER(0x800505DC, "ACE3.BIN");
    mission_ai_limit_altitude(index, position);
    limit = game_runtime.world_settings.minimum_x;
    value = position[0];
    limit += 0x4000000;
    if ((sint32)value < (sint32)limit)
    {
        position[0] = (limit);
    }
    limit = game_runtime.world_settings.maximum_x;
    value = position[0];
    limit -= 0x4000000;
    if ((sint32)limit < (sint32)value)
    {
        position[0] = (limit);
    }
    limit = game_runtime.world_settings.minimum_z;
    value = position[2];
    limit += 0x4000000;
    if ((sint32)value < (sint32)limit)
    {
        position[2] = (limit);
    }
    limit = game_runtime.world_settings.maximum_z;
    value = position[2];
    limit -= 0x4000000;
    result = (sint32)limit < (sint32)value;
    if (result)
    {
        position[2] = (limit);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_steer_level_to_position(uint32 index, const uint32 position[3])
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_root;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_root;
    uint32 actor_index_table;

    uint32 value, target, altitude, limit, low, angle;
    FUNCTION_MARKER(0x80044360, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    value = pose_owner_object->pose.angles[2];
    ai_owner_state = mission_primary_ai((0) + (index));
    value = 0 - value;
    ai_owner_state->steering_angles[2] = (uint16)(value);
    if ((value << 16) != 0)
        return;
    target = (position[1]);
    altitude = pose_owner_object->pose.position[1];
    value = (sint32)(target - altitude) >> 12;
    angle = game_vector_atan2(0 - value, 2048);
    selected_controls_root = mission_primary_controls(index & 0x03FFFFFFu);
    controls_root = &selected_controls_root;
    limit = (sint16)controls_root->pitch_limit;
    if (limit == 0)
        limit = 3640;
    if ((sint32)limit < (sint32)angle)
        angle = limit;
    low = 0 - limit;
    if ((sint32)angle < (sint32)low)
        angle = low;
    if (angle - 1 < 7)
        angle = 8;
    if ((sint32)angle < 0 && (sint32)angle >= -7)
        angle = 0xFFFFFFF8;
    value = pose_owner_object->pose.angles[0];
    ai_owner_state->steering_angles[0] = (uint16)(angle - value);
    return;
}

void mission_ai_steer_direction(uint32 index, uint32 speed, GAME_ANGLES *output, const uint32 vector[3])
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_ACTOR *pose_owner_descriptor;
    uint32 actor_index_table;

    uint32 value, a, b;
    sint32 pitch, yaw, roll;
    FUNCTION_MARKER(0x80044188, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_object = mission_primary_ai((0) + (index));
    speed = (sint16)speed;
    pose_owner_descriptor = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    if ((sint32)speed < 128)
        return;
    a = ((vector[1]));
    b = ((vector[2]));
    value = game_vector_atan2(0 - a, b);
    output->pitch = (sint16)(value);
    a = ((vector[0]));
    b = ((vector[2]));
    value = game_vector_atan2(a, b);
    output->yaw = (sint16)(value);
    if ((sint32)speed < 768)
    {
        value = 0 - pose_owner_descriptor->pose.angles[2];
        output->roll = (sint16)(value);
    }
    else if ((sint32)speed >= 0x1FFF)
    {
        output->roll = (sint16)(0);
    }
    else
    {
        a = ((vector[1]));
        b = ((vector[0]));
        value = game_vector_atan2(b, 0 - a);
        pitch = ((sint16)((uint16)output->pitch));
        output->roll = (sint16)(value);
        if (pitch < 0 && pitch >= -1023)
        {
            yaw = ((sint16)((uint16)output->yaw));
            if (yaw < 512 && yaw >= -511)
            {
                output->roll = (sint16)(value + 0x2000);
            }
        }
        value = (((uint16)output->roll));
        output->roll = (sint16)(value & 0x3FFF);
        value = (((uint16)output->pitch));
        roll = ((sint16)((uint16)output->roll));
        output->pitch = (sint16)(value & 0x3FFF);
        value = (((uint16)output->roll));
        if (roll >= 0x2000)
        {
            output->roll = (sint16)(value - 0x4000);
        }
        pitch = ((sint16)((uint16)output->pitch));
        value = (((uint16)output->pitch));
        if (pitch >= 0x2000)
        {
            output->pitch = (sint16)(value - 0x4000);
        }
    }
    value = (((uint16)output->roll));
    ai_owner_object->steering_angles[2] = (uint16)(value);
    value = (((uint16)output->roll));
    if (((value + 127) & 0xFFFF) >= 255)
        return;
    value = (((uint16)output->pitch));
    ai_owner_object->steering_angles[0] = (uint16)(value);
    value = (((uint16)output->pitch));
    if (((value + 127) & 0xFFFF) >= 255)
        return;
    value = (((uint16)output->yaw));
    ai_owner_object->steering_angles[1] = (uint16)(value);
    return;
}

uint32 mission_ai_turn_to_position(uint32 index, const uint32 *position)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_parameters;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_parameters;
    uint32 actor_index_objects;

    uint32 speed, value, x, z, heading, difference, turn, old_turn, step, minimum;
    sint32 signed_speed, signed_turn, magnitude, turn_magnitude, step_magnitude;
    FUNCTION_MARKER(0x80044474, "ACE3.BIN");
    selected_controls_parameters = mission_primary_controls(index & 0x03FFFFFFu);
    controls_parameters = &selected_controls_parameters;
    actor_index_objects = 0;
    speed = controls_parameters->bank_limit;
    value = (sint16)controls_parameters->bank_limit;
    pose_owner_object = mission_actor((actor_index_objects + ((uint32)((sint32)((index << 2)) >> 2))));
    ai_owner_record = mission_primary_ai((0) + (index));
    if ((sint32)value >= 0x1001)
        speed = 0x1000;
    if (speed == 0)
        speed = 0x1000;
    x = (position[0]);
    value = pose_owner_object->pose.position[0];
    x -= value;
    z = (position[2]);
    value = pose_owner_object->pose.position[2];
    z -= value;
    heading = game_vector_atan2((sint32)x >> 12, (sint32)z >> 12);
    value = pose_owner_object->pose.angles[1];
    difference = (heading - value) & 0x3FFF;
    if (difference == 0)
        return 0;
    turn = difference;
    if (difference >= 0x2000)
        turn |= 0xC000;
    signed_turn = (sint16)turn;
    signed_speed = (sint16)speed;
    if (signed_turn < -signed_speed)
        turn = 0 - speed;
    else if (signed_speed < signed_turn)
        turn = speed;
    old_turn = pose_owner_object->pose.angles[2];
    ai_owner_record->steering_angles[2] = (uint16)(turn - old_turn);
    value = (sint16)controls_parameters->pitch_rate_max;
    old_turn = (sint16)pose_owner_object->pose.angles[2];
    step = (sint32)(value * old_turn) >> 12;
    minimum = (sint16)controls_parameters->yaw_rate;
    magnitude = (sint16)step;
    if (magnitude < 0)
        magnitude = -magnitude;
    value = controls_parameters->yaw_rate;
    if (magnitude < (sint32)minimum)
    {
        if ((sint16)step < 0)
            step = 0 - value;
        else
            step = value;
    }
    turn_magnitude = (sint16)turn;
    if (turn_magnitude < 0)
        turn_magnitude = -turn_magnitude;
    step_magnitude = (sint16)step;
    if (step_magnitude < 0)
        step_magnitude = -step_magnitude;
    if (turn_magnitude < step_magnitude)
        step = turn;
    heading = pose_owner_object->pose.angles[1];
    pose_owner_object->pose.angles[1] = (uint16)(heading + step);
    return 1;
}

void mission_ai_decode_maneuver_command(const uint8 *command, MISSION_PRIMARY_AI *state)
{
    const MISSION_AI_MANEUVER_RECORD *wire = (const MISSION_AI_MANEUVER_RECORD *)command;
    uint32 kind, first, second, third;
    FUNCTION_MARKER(0x80050FA8, "ACE3.BIN");
    kind = wire->kind;
    {
        state->requested_maneuver = (uint8)(kind);
    }
    first = wire->target_position[0];
    second = wire->target_position[1];
    third = wire->target_position[2];
    {
        state->target_position[0] = (uint32)(first);
    }
    {
        state->target_position[1] = (uint32)(second);
    }
    {
        state->target_position[2] = (uint32)(third);
    }
    first = wire->packed_angles01;
    second = wire->angle2;
    {
        {
            uint32 word = (first);
            state->requested_angles[0] = (uint16)word;
            state->requested_angles[1] = (uint16)(word >> 16);
        }
    }
    state->requested_angles[2] = (uint16)second;
    kind = (state->requested_maneuver);
    if (kind - 50 < 2)
    {
        {
            state->speed_mode = (uint8)(9);
        }
        return;
    }
    {
        state->speed_mode = (uint8)(0);
    }
    return;
}

void mission_primary_initialize_actors(void)
{
    MISSION_PRIMARY_AI *ai_owner_aux;
    MISSION_ACTOR *pose_owner_object;
    const MISSION_ACTOR_DEFINITION *definition_attributes;
    const MISSION_PRIMARY_PARAMETERS *parameters;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_first_table;
    uint32 actor_index_table;

    MISSION_AI_BEHAVIOR_PROFILE profile;
    uint32 index, kind;
    uint32 w0, w1, w2, value, count;
    FUNCTION_MARKER(0x8004D84C, "ACE3.BIN");
    actor_index_table = 2;
    for (index = 2; index < 32; ++index)
    {
        actor_motion_object = mission_actor_motion(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        actor_index_table += 1;
        actor_traits_object->flags = (uint32)(index);
        *mission_actor_flags(index) = (uint32)(0);
    }
    count = game_runtime.actor_resources.primary_count;
    for (index = 2; index < count; ++index)
    {
        pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
        actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        parameters = &mission_primary_binding(index & 0x03FFFFFFu)->parameters;
        definition_attributes = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
        value = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(value | 0x4000);
        value = arena_alloc(12);
        const RESOURCE_ASSET_SOURCE *marker_owner = &game_runtime.resource_context.aircraft_backing;
        const MISSION_MARKER_SEED *marker_seed = NULL;
        if (value)
        {
            uint32 marker_offset = value - marker_owner->encoded_base;
            if (!marker_owner->data || value < marker_owner->encoded_base || marker_offset > marker_owner->bytes || 6 > marker_owner->bytes - marker_offset)
                abort();
            marker_seed = (const MISSION_MARKER_SEED *)(marker_owner->data + marker_offset + 2);
        }
        mission_actor_bind_marker(index, marker_seed);
        value = arena_alloc(40);
        mission_actor_bind_target(index, value != 0);
        mission_actor_initialize_target_state(index);
        kind = parameters->group;
        if (kind < 3)
            actor_traits_object->flags = (uint32)(actor_traits_object->flags | (0x40 << kind));
        actor_traits_object->model = (uint8)(parameters->model);
        value = definition_attributes->health;
        actor_index_first_table = 0;
        actor_traits_object->health = (uint16)(value);
        actor_traits_object->kind = (uint16)(parameters->kind);
        w0 = parameters->pose.position[0];
        w1 = parameters->pose.position[1];
        w2 = parameters->pose.position[2];
        pose_owner_object->pose.position[0] = (uint32)(w0);
        pose_owner_object->pose.position[1] = (uint32)(w1);
        pose_owner_object->pose.position[2] = (uint32)(w2);
        w0 = (parameters->pose.angles[0] | ((uint32)parameters->pose.angles[1] << 16));
        w1 = (parameters->pose.angles[2]);
        {
            uint32 pose_word = (w0);
            pose_owner_object->pose.angles[0] = (uint16)pose_word;
            pose_owner_object->pose.angles[1] = (uint16)(pose_word >> 16);
        }
        {
            uint32 pose_word = (w1);
            pose_owner_object->pose.angles[2] = (uint16)pose_word;
        }
        value = game_measure_distance_gte(pose_owner_object->pose.position, mission_actor(actor_index_first_table)->pose.position);
        mission_actor_target(index)->distance = value;
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        value = mission_primary_controls(index & 0x03FFFFFFu).cruise_speed;
        actor_motion_object->angular_delta[0] = (uint16)(0);
        actor_motion_object->angular_delta[1] = (uint16)(0);
        actor_motion_object->angular_delta[2] = (uint16)(0);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_traits_object->events = (uint32)(0x80200000);
        ai_owner_aux = mission_primary_ai((0) + (index));
        actor_motion_object->local_velocity[2] = (uint32)(value);
        ai_owner_aux->flags = (uint32)(0);
        profile = mission_ai_selected_behavior_profile(mission_primary_binding(index));
        value = profile.kind;
        ai_owner_aux->script_phase = (uint8)(0);
        ai_owner_aux->behavior_ticks = (uint32)(0);
        ai_owner_aux->behavior_kind = (uint8)(value);
        ai_owner_aux->behavior_target = (uint8)(profile.target);
        w0 = profile.origin[0];
        w1 = profile.origin[1];
        w2 = profile.origin[2];
        ai_owner_aux->behavior_origin[0] = (uint32)(w0);
        ai_owner_aux->behavior_origin[1] = (uint32)(w1);
        ai_owner_aux->behavior_origin[2] = (uint32)(w2);
        ai_owner_aux->activate_radius = (uint32)(profile.activate_radius);
        value = profile.deactivate_radius;
        ai_owner_aux->behavior_state = (uint8)(255);
        ai_owner_aux->requested_behavior = (uint8)(255);
        ai_owner_aux->behavior_phase = (uint8)(0);
        ai_owner_aux->deactivate_radius = (uint32)(value);
        ai_owner_aux->behavior_timer = (uint16)(0);
        ai_owner_aux->maneuver_state = (uint8)(255);
        ai_owner_aux->requested_maneuver = (uint8)(255);
        ai_owner_aux->maneuver_phase = (uint8)(0);
        ai_owner_aux->behavior_actor = (uint8)(255);
        ai_owner_aux->target_actor = (uint8)(255);
        ai_owner_aux->speed_mode = (uint8)(0);
        count = game_runtime.actor_resources.primary_count;
        ai_owner_aux->maneuver_ticks = (uint16)(0);
        ai_owner_aux->target_refresh_ticks = (uint16)(0);
        ai_owner_aux->angular_rate[0] = (uint16)(0);
        ai_owner_aux->angular_rate[1] = (uint16)(0);
        ai_owner_aux->angular_rate[2] = (uint16)(0);
        ai_owner_aux->acceleration[0] = (uint32)(0);
        ai_owner_aux->acceleration[1] = (uint32)(0);
        ai_owner_aux->acceleration[2] = (uint32)(0);
        ai_owner_aux->steering_angles[0] = (uint16)(0);
        ai_owner_aux->steering_angles[1] = (uint16)(0);
        ai_owner_aux->steering_angles[2] = (uint16)(0);
    }
    return;
}

void mission_ai_update_pitch_maneuver(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_data;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_data;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 phase, value, magnitude, step, remaining, angle, current, roll, signed_angle, velocity, limit, result;
    FUNCTION_MARKER(0x8004A7E8, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    selected_controls_data = mission_primary_controls(index & 0x03FFFFFFu);
    controls_data = &selected_controls_data;
    if (phase >= 5)
    {
        ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    switch (phase)
    {
        case 0:
            ai_owner_state->speed_mode = (uint8)(0);
            result = mission_ai_request_level_angles(index);
            if (result != 0)
            {
                (void)(uint32)(result);
                return;
            }
            ai_owner_state->speed_mode = (uint8)(9);
            actor_motion_object->local_velocity[2] = (uint32)(0);
            actor_motion_object->local_velocity[1] = (uint32)(0);
            actor_motion_object->local_velocity[0] = (uint32)(0);
            value = ai_owner_state->requested_angles[2];
            ai_owner_state->maneuver_phase = (uint8)(1);
            ai_owner_state->steering_angles[2] = (uint16)(value);
            return;
        case 1:
            result = (sint16)ai_owner_state->steering_angles[2];
            if (result == 0)
            {
                result = 2;
                ai_owner_state->maneuver_phase = (uint8)(result);
            }
            {
                (void)(uint32)(result);
                return;
            }
        case 2:
            value = actor_motion_object->velocity[0];
            magnitude = value;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude > 0xA000)
                actor_motion_object->velocity[0] = (uint32)((sint32)value >> 1);
            value = actor_motion_object->velocity[2];
            magnitude = value;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude > 0xA000)
                actor_motion_object->velocity[2] = (uint32)((sint32)value >> 1);
            step = (sint16)controls_data->pitch_rate_max;
            step <<= 1;
            remaining = (sint16)ai_owner_state->requested_angles[0];
            if ((sint32)remaining < (sint32)step)
                step = remaining;
            angle = pose_owner_object->pose.angles[0];
            pose_owner_object->pose.angles[0] = (uint16)(angle + step);
            remaining = ai_owner_state->requested_angles[0];
            value = remaining - step;
            ai_owner_state->requested_angles[0] = (uint16)(value);
            result = value << 16;
            if (result == 0)
            {
                result = 3;
                ai_owner_state->maneuver_phase = (uint8)(result);
            }
            {
                (void)(uint32)(result);
                return;
            }
        case 3:
            ai_owner_state->speed_mode = (uint8)(10);
            signed_angle = (sint16)pose_owner_object->pose.angles[0];
            current = pose_owner_object->pose.angles[0];
            if ((sint32)signed_angle > 0)
            {
                roll = (sint16)pose_owner_object->pose.angles[2];
                if ((sint32)roll < 0)
                    roll = 0 - roll;
            }
            else
                roll = 4096;
            if ((sint32)signed_angle > 0 && (sint32)roll < 4096)
            {
                step = (sint16)controls_data->pitch_rate_max;
                value = controls_data->pitch_rate_max;
                if ((sint32)step < (sint32)signed_angle)
                    pose_owner_object->pose.angles[0] = (uint16)(current - value);
            }
            else
            {
                signed_angle = (sint16)pose_owner_object->pose.angles[0];
                current = pose_owner_object->pose.angles[0];
                if ((sint32)signed_angle >= -4095)
                {
                    step = controls_data->pitch_rate_max;
                    pose_owner_object->pose.angles[0] = (uint16)(current - step);
                }
            }
            velocity = actor_motion_object->local_velocity[2];
            limit = controls_data->min_speed;
            result = (sint32)velocity < (sint32)limit;
            if (result != 0)
            {
                (void)(uint32)(result);
                return;
            }
            value = (sint16)ai_owner_state->requested_angles[1];
            if (value == 0)
            {
                result = mission_ai_request_level_angles(index);
                if (result != 0)
                {
                    (void)(uint32)(result);
                    return;
                }
            }
            ai_owner_state->maneuver_phase = (uint8)(4);
            return;
    }
    ai_owner_state->requested_maneuver = (uint8)(255);
    return;
}

void mission_primary_prepare_scenario_actors(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_destination;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_source;
    const MISSION_PRIMARY_PARAMETERS *parameters;
    MISSION_ACTOR_MOTION *actor_motion_source;

    MISSION_ACTOR_MOTION *actor_motion_destination;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_cursor;
    uint32 actor_index_table;

    uint32 selection, kind, mission, clear_objects, index, flags, member, first, second, third, value, mode;
    uint32 *slot;
    const MISSION_SCENARIO *record;
    FUNCTION_MARKER(0x8004DC38, "ACE3.BIN");
    selection = mission_flow.scenario_idx;
    record = &mission_scenarios[selection];
    kind = record->kind;
    clear_objects = kind == 0 || kind == 1 || kind == 2 || kind == 3;
    if (kind == 7)
    {
        mission = (sint16)game_runtime.selection.mission;
        clear_objects = mission != 7;
    }
    if (clear_objects != 0)
    {
        actor_index_table = 0;
        actor_index_cursor = (actor_index_table + 2);
        for (index = 2; index < 32; ++index)
        {
            actor_traits_object = mission_actor_traits(actor_index_cursor);
            flags = actor_traits_object->flags;
            if ((flags & 64) != 0 && ((*mission_actor_flags(index)) & 0x400000) != 0)
                value = flags & 0xFFFFFBFF;
            else
                value = actor_traits_object->flags & 0xFFFFBFFF;
            actor_traits_object->flags = (uint32)(value);
            actor_index_cursor += 1;
        }
    }
    for (index = 0; index < 2; ++index)
    {
        member = record->members[index];
        if (member == 0)
            continue;
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
        flags = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(flags | 0x4000);
        ai_owner_state = mission_primary_ai((0) + (member));
        mission_primary_show_actor(member);
        mission_primary_activate_actor(member);
        ai_owner_state->behavior_kind = (uint8)(8);
        ai_owner_state->script_phase = (uint8)(0);
        ai_owner_state->behavior_ticks = (uint32)(0);
        ai_owner_state->behavior_phase = (uint8)(0);
        ai_owner_state->maneuver_phase = (uint8)(0);
        slot = mission_actor_flags(member);
        flags = (*slot);
        *slot = (flags & 0xFFEFFFFF);
        ai_owner_state->angular_rate[0] = (uint16)(0);
        ai_owner_state->angular_rate[1] = (uint16)(0);
        ai_owner_state->angular_rate[2] = (uint16)(0);
        ai_owner_state->acceleration[0] = (uint32)(0);
        ai_owner_state->acceleration[1] = (uint32)(0);
        ai_owner_state->acceleration[2] = (uint32)(0);
        ai_owner_state->steering_angles[0] = (uint16)(0);
        ai_owner_state->steering_angles[1] = (uint16)(0);
        ai_owner_state->steering_angles[2] = (uint16)(0);
        kind = record->kind;
        if (kind - 2 >= 6)
            continue;
        if (kind == 2 || kind == 3)
        {
            actor_index_table = 0;
            pose_owner_destination = mission_actor((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
            actor_motion_destination = mission_actor_motion((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
            pose_owner_source = mission_actor(actor_index_table);
            actor_motion_source = mission_actor_motion(actor_index_table);
            first = pose_owner_source->pose.position[0];
            second = pose_owner_source->pose.position[1];
            third = pose_owner_source->pose.position[2];
            pose_owner_destination->pose.position[0] = (uint32)(first);
            pose_owner_destination->pose.position[1] = (uint32)(second);
            pose_owner_destination->pose.position[2] = (uint32)(third);
            value = pose_owner_destination->pose.position[2];
            pose_owner_destination->pose.position[2] = (uint32)(value + 0x01000000);
            value = pose_owner_destination->pose.position[1];
            pose_owner_destination->pose.position[1] = (uint32)(value - 0x100000);
            mode = game_runtime.selection.difficulty;
            if ((sint32)mode >= 2)
            {
                value = game_random() & 1;
                first = pose_owner_destination->pose.position[0];
                pose_owner_destination->pose.position[0] = (uint32)(value != 0 ? first + 0x400000 : first - 0x400000);
            }
            first = (pose_owner_source->pose.angles[0] | ((uint32)pose_owner_source->pose.angles[1] << 16));
            second = (pose_owner_source->pose.angles[2]);
            {
                uint32 pose_word = (first);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (first);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (second);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            {
                uint32 pose_word = (second);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            first = actor_motion_source->velocity[0];
            second = actor_motion_source->velocity[1];
            third = actor_motion_source->velocity[2];
            actor_motion_destination->velocity[0] = (uint32)(first);
            actor_motion_destination->velocity[1] = (uint32)(second);
            actor_motion_destination->velocity[2] = (uint32)(third);
            value = actor_motion_source->velocity[2];
            actor_motion_destination->local_velocity[2] = (uint32)(value);
            ai_owner_state->requested_behavior = (uint8)(19);
        }
        else if (kind == 5 || kind == 6)
        {
            parameters = &mission_primary_binding(member)->parameters;
            first = parameters->pose.position[0];
            second = parameters->pose.position[1];
            third = parameters->pose.position[2];
            pose_owner_object->pose.position[0] = (uint32)(first);
            pose_owner_object->pose.position[1] = (uint32)(second);
            pose_owner_object->pose.position[2] = (uint32)(third);
            first = (parameters->pose.angles[0] | ((uint32)parameters->pose.angles[1] << 16));
            second = (parameters->pose.angles[2]);
            {
                uint32 pose_word = (first);
                pose_owner_object->pose.angles[0] = (uint16)pose_word;
                pose_owner_object->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (second);
                pose_owner_object->pose.angles[2] = (uint16)pose_word;
            }
            ai_owner_state->requested_behavior = (uint8)(20);
        }
        else if (kind == 7)
        {
            actor_index_table = 0;
            pose_owner_destination = mission_actor((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
            actor_motion_destination = mission_actor_motion((actor_index_table + ((uint32)((sint32)((member << 2)) >> 2))));
            pose_owner_source = mission_actor(actor_index_table);
            actor_motion_source = mission_actor_motion(actor_index_table);
            first = pose_owner_source->pose.position[0];
            second = pose_owner_source->pose.position[1];
            third = pose_owner_source->pose.position[2];
            pose_owner_destination->pose.position[0] = (uint32)(first);
            pose_owner_destination->pose.position[1] = (uint32)(second);
            pose_owner_destination->pose.position[2] = (uint32)(third);
            value = actor_motion_source->velocity[0];
            first = pose_owner_destination->pose.position[0];
            pose_owner_destination->pose.position[0] = (uint32)(first + value * 150);
            value = actor_motion_source->velocity[2];
            first = pose_owner_destination->pose.position[2];
            pose_owner_destination->pose.position[2] = (uint32)(first + value * 150);
            first = (pose_owner_source->pose.angles[0] | ((uint32)pose_owner_source->pose.angles[1] << 16));
            second = (pose_owner_source->pose.angles[2]);
            {
                uint32 pose_word = (first);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (first);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (second);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            {
                uint32 pose_word = (second);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            ai_owner_state->requested_behavior = (uint8)(index == 0 ? 21 : 22);
        }
    }
    return;
}

void mission_ai_update_behavior_04(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 selected, mode, a, b, c;
    FUNCTION_MARKER(0x8004F034, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    selected = ai_owner_state->behavior_actor;
    actor_index_table = 0;
    mode = ai_owner_state->behavior_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    if (mode == 0)
        ai_owner_state->behavior_phase = (uint8)(1);
    a = pose_owner_object->pose.position[0];
    b = pose_owner_object->pose.position[1];
    c = pose_owner_object->pose.position[2];
    ai_owner_state->target_position[0] = (uint32)(a);
    ai_owner_state->target_position[1] = (uint32)(b);
    ai_owner_state->target_position[2] = (uint32)(c);
    mission_ai_limit_position(index, ai_owner_state->target_position);
    ai_owner_state->requested_maneuver = (uint8)(4);
    ai_owner_state->speed_mode = (uint8)(6);
    return;
}

void mission_ai_update_behavior_00(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 a, b, c, flags;
    FUNCTION_MARKER(0x8004F4D8, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    if (ai_owner_state->behavior_phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        a = ai_owner_state->aim_position[0];
        b = ai_owner_state->aim_position[1];
        c = ai_owner_state->aim_position[2];
        ai_owner_state->target_position[0] = (uint32)(a);
        ai_owner_state->target_position[1] = (uint32)(b);
        ai_owner_state->target_position[2] = (uint32)(c);
        mission_ai_limit_position(index, ai_owner_state->target_position);
        ai_owner_state->speed_mode = (uint8)(0);
    }
    flags = ai_owner_state->flags;
    ai_owner_state->requested_maneuver = (uint8)((flags & 0x2000000) ? 5 : 0);
    return;
}

void mission_ai_update_behavior_10(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 value;
    FUNCTION_MARKER(0x8004FB28, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    if (ai_owner_state->behavior_phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        ai_owner_state->requested_maneuver = (uint8)(30);
    }
    else if (ai_owner_state->flags & 0x400000)
        ai_owner_state->requested_behavior = (uint8)(255);
    value = ai_owner_state->maneuver_phase;
    if (value == 0)
    {
        ai_owner_state->speed_mode = (uint8)(2);
        return;
    }
    ai_owner_state->speed_mode = (uint8)(0);
    {
        (void)(uint32)(value);
        return;
    }
}

void mission_ai_update_behavior_12(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_root;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_root;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 mode, type, value;
    FUNCTION_MARKER(0x8004FC04, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    mode = ai_owner_state->behavior_phase;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (mode == 0)
    {
        selected_controls_root = mission_primary_controls(index & 0x03FFFFFFu);
        controls_root = &selected_controls_root;
        type = controls_root->kind;
        if (type == 1 || type == 3)
            value = 43;
        else
        {
            type = (sint16)actor_traits_object->kind;
            if (type == 26)
                value = 42;
            else if (type == 44)
                value = 40;
            else
            {
                value = game_random();
                value = (sint32)value % 3 + 40;
            }
        }
        ai_owner_state->requested_maneuver = (uint8)(value);
        ai_owner_state->behavior_phase = (uint8)(1);
    }
    ai_owner_state->speed_mode = (uint8)(9);
    return;
}

void mission_ai_update_behavior_03(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 selected, mode, value;
    FUNCTION_MARKER(0x8004EF80, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    selected = ai_owner_state->behavior_actor;
    actor_index_table = 0;
    mode = ai_owner_state->behavior_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    if (mode == 0)
        ai_owner_state->behavior_phase = (uint8)(1);
    game_pose_transform_offset(&pose_owner_object->pose, ai_owner_state->target_position, ai_owner_state->aim_position);
    mission_ai_limit_position(index, ai_owner_state->target_position);
    value = ai_owner_state->origin_distance;
    ai_owner_state->requested_maneuver = (uint8)((sint32)value < 0x2000 ? 7 : 4);
    ai_owner_state->speed_mode = (uint8)(5);
    return;
}

void mission_ai_update_behavior_05(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_player;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_behavior;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_behavior;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_settings;
    MISSION_ACTOR_MOTION *actor_motion_player;

    uint32 actor_index_table;

    uint32 phase, value, a, b, tick, angle, sine, cosine, product, result, mode, direction, seed, random;
    FUNCTION_MARKER(0x8004F0C8, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    pose_owner_player = mission_actor(actor_index_table);
    actor_motion_player = mission_actor_motion(actor_index_table);
    motion_settings = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    phase = ai_owner_state->behavior_phase;
    selected_controls_behavior = mission_primary_controls(index & 0x03FFFFFFu);
    controls_behavior = &selected_controls_behavior;
    if (phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        value = motion_settings->behavior_delay_ticks;
        ai_owner_state->behavior_timer = (uint16)(value);
        mode = game_runtime.selection.difficulty;
        if ((sint32)mode >= 2)
            ai_owner_state->behavior_timer = (uint16)((sint16)value >> 1);
        result = pose_owner_player->pose.position[1];
        ai_owner_state->target_position[1] = (uint32)(result);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 1)
    {
        ai_owner_state->behavior_actor = (uint8)(0);
        a = actor_motion_player->velocity[0];
        b = pose_owner_player->pose.position[0];
        tick = (uint8)game_runtime.mission_control.ticks;
        ai_owner_state->target_position[0] = (uint32)(b + (a << 12));
        a = actor_motion_player->velocity[2];
        b = pose_owner_player->pose.position[2];
        ai_owner_state->target_position[2] = (uint32)(b + (a << 12));
        mode = game_runtime.selection.difficulty;
        if ((sint32)mode >= 2)
        {
            angle = (tick << 6) & 0x3FFF;
            sine = game_wave_at(angle * 2);
            value = motion_settings->response;
            product = sine * value;
            cosine = game_wave_at((angle + 0x1000) * 2);
            value = ai_owner_state->target_position[0];
            ai_owner_state->target_position[0] = (uint32)(value + (product << 12));
            a = motion_settings->response;
            product = cosine * a;
            value = ai_owner_state->target_position[2];
            ai_owner_state->target_position[2] = (uint32)(value + (product << 12));
        }
        mission_ai_limit_position(index, ai_owner_state->target_position);
        ai_owner_state->requested_maneuver = (uint8)(4);
        value = ai_owner_state->behavior_timer;
        ai_owner_state->speed_mode = (uint8)(3);
        --value;
        ai_owner_state->behavior_timer = (uint16)(value);
        result = value << 16;
        if (result == 0)
        {
            ai_owner_state->behavior_phase = (uint8)(2);
            result = 2;
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        ai_owner_state->behavior_phase = (uint8)(3);
        mode = controls_behavior->kind;
        if (mode == 3)
            return;
        if (mode == 1)
        {
            a = (sint16)controls_behavior->bank_limit;
            if (a == 0)
                a = 4096;
            value = game_runtime.mission_control.ticks;
            if ((value & 1) == 0)
                a = 0 - a;
            ai_owner_state->requested_angles[0] = (uint16)(2048);
            ai_owner_state->requested_angles[1] = (uint16)(0);
            ai_owner_state->requested_angles[2] = (uint16)(a);
            result = 12;
            goto select_behavior;
        }
        mode = (sint16)game_runtime.selection.mission;
        if (mode == 32 || mode == 47 || mode == 53)
        {
            ai_owner_state->requested_angles[0] = (uint16)(8192);
            ai_owner_state->requested_angles[1] = (uint16)(0);
            value = game_runtime.mission_control.ticks;
            a = pose_owner_object->pose.angles[2];
            ai_owner_state->requested_angles[2] = (uint16)(((value & 1) != 0 ? 0xFFFFF000 : 0x1000) - a);
            result = 12;
            goto select_behavior;
        }
        value = motion_settings->kind;
        if (value >= 2)
        {
            tick = game_runtime.mission_control.ticks;
            if ((tick & 7) == 0)
            {
                direction = pose_owner_object->pose.position[0] & 3;
                if (direction < 2)
                {
                    ai_owner_state->requested_angles[0] = (uint16)(direction == 1 ? 8192 : 4096);
                    ai_owner_state->requested_angles[1] = (uint16)(0);
                    ai_owner_state->requested_angles[2] = (uint16)(0);
                }
                else if (direction == 2)
                {
                    ai_owner_state->requested_angles[0] = (uint16)(4096);
                    ai_owner_state->requested_angles[1] = (uint16)(0);
                    ai_owner_state->requested_angles[2] = (uint16)(4096);
                }
                else
                {
                    a = (uint16)(pose_owner_object->pose.position[1]);
                    ai_owner_state->requested_angles[1] = (uint16)(0);
                    ai_owner_state->requested_angles[0] = (uint16)((a & 0x1FFF) + 8192);
                    a = (uint16)(pose_owner_object->pose.position[2]);
                    ai_owner_state->requested_angles[2] = (uint16)(a & 0x1FFF);
                }
                result = 15;
                goto select_behavior;
            }
        }
        value = motion_settings->kind;
        if (value != 0)
        {
            tick = game_runtime.mission_control.ticks;
            if ((tick & 7) == 1)
            {
                direction = pose_owner_object->pose.position[0] & 3;
                result = direction == 0 ? 10 : direction == 1 ? 11 : direction == 2 ? 13 : 14;
                goto select_behavior;
            }
        }
        ai_owner_state->requested_angles[0] = (uint16)(8192);
        ai_owner_state->requested_angles[1] = (uint16)(0);
        seed = random_seed;
        random = psyq_rand_next(&seed);
        random_seed = seed;
        ai_owner_state->requested_angles[2] = (uint16)((random & 0x3FFF) - 8192);
        result = 12;
    select_behavior:
        ai_owner_state->requested_maneuver = (uint8)(result);
        ai_owner_state->speed_mode = (uint8)(0);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 3)
    {
        result = ai_owner_state->flags & 0x400000;
        if (result != 0)
        {
            ai_owner_state->requested_behavior = (uint8)(255);
            result = 255;
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    return;
}

void mission_ai_update_behavior_07(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_value;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_behavior;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_behavior;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_settings;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    EFFECT_OBJECT *effect;
    uint32 slot, phase, value, mode, a, b, c, tick, seed, random, difficulty, angle, yaw, raw, result, shift;
    FUNCTION_MARKER(0x8004F570, "ACE3.BIN");
    actor_index_table = 0;
    slot = effect_actor_tracking.last_effect[index];
    motion_settings = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    selected_controls_behavior = mission_primary_controls(index & 0x03FFFFFFu);
    controls_behavior = &selected_controls_behavior;
    ai_owner_state = mission_primary_ai((0) + (index));
    effect = slot < EFFECT_OBJECT_CAPACITY ? effect_objects.slot + slot : 0;
    phase = ai_owner_state->behavior_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        mode = controls_behavior->kind;
        if (mode != 0 && mode != 5 && mode != 2 && mode != 1)
            ai_owner_state->requested_behavior = (uint8)(255);
        ai_owner_state->behavior_phase = (uint8)(1);
        a = motion_settings->response;
        b = motion_settings->turn_delay_ticks;
        c = motion_settings->turn_phase_ticks;
        ai_owner_state->behavior_timer = (uint16)(c + b - (a - 22));
        value = (*mission_actor_flags(index));
        if ((value & 0x800000) == 0)
        {
            slot = effect_actor_tracking.last_effect[index];
            if (slot >= EFFECT_OBJECT_CAPACITY)
                abort();
            value = effect_objects.slot[slot].source_flags;
            if ((value & 63) != 0 && (value & 0x40) != 0)
                ai_owner_state->behavior_phase = (uint8)(16);
        }
        value = ai_owner_state->behavior_kind;
        if (value == 10)
        {
            tick = game_runtime.mission_control.ticks;
            if ((tick & 3) != 0)
                ai_owner_state->behavior_phase = (uint8)(7);
        }
    }
    value = ai_owner_state->behavior_timer - 1;
    ai_owner_state->behavior_timer = (uint16)(value);
    if ((sint32)(value << 16) <= 0)
        ai_owner_state->requested_behavior = (uint8)(255);
    phase = ai_owner_state->behavior_phase;
    if (phase == 16)
    {
        value = actor_traits_object->render_flags;
        actor_traits_object->render_flags = (uint8)(value | 8);
        ai_owner_state->requested_maneuver = (uint8)(0);
        ai_owner_state->speed_mode = (uint8)(0);
        return;
    }
    a = (sint16)ai_owner_state->behavior_timer;
    b = (sint16)motion_settings->turn_phase_ticks;
    if ((sint32)b < (sint32)a)
        return;
    value = actor_traits_object->render_flags;
    actor_traits_object->render_flags = (uint8)(value | 8);
    phase = ai_owner_state->behavior_phase;
    if (phase == 7)
    {
        actor_index_table = 0;
        pose_owner_value = mission_actor(actor_index_table);
        a = pose_owner_value->pose.position[0];
        b = pose_owner_value->pose.position[1];
        c = pose_owner_value->pose.position[2];
        ai_owner_state->target_position[0] = (uint32)(a);
        ai_owner_state->target_position[1] = (uint32)(b);
        ai_owner_state->target_position[2] = (uint32)(c);
        a = (pose_owner_value->pose.angles[0] | ((uint32)pose_owner_value->pose.angles[1] << 16));
        {
            uint32 ai_packed_word = (a);
            ai_owner_state->requested_angles[0] = (uint16)ai_packed_word;
            ai_owner_state->requested_angles[1] = (uint16)(ai_packed_word >> 16);
        }
        ai_owner_state->requested_angles[2] = pose_owner_value->pose.angles[2];
        ai_owner_state->requested_maneuver = (uint8)(4);
        ai_owner_state->speed_mode = (uint8)(6);
        return;
    }
    if (phase == 3)
    {
        difficulty = game_runtime.selection.difficulty;
        if (difficulty == 3)
            ai_owner_state->behavior_phase = (uint8)(1);
        else if (difficulty == 2)
        {
            value = (sint16)ai_owner_state->maneuver_ticks;
            ai_owner_state->requested_maneuver = (uint8)(0);
            if (value == 60)
                ai_owner_state->behavior_phase = (uint8)(1);
        }
        else if (difficulty == 1)
        {
            value = (sint16)ai_owner_state->maneuver_ticks;
            ai_owner_state->requested_maneuver = (uint8)(0);
            if (value == 90)
                ai_owner_state->behavior_phase = (uint8)(1);
        }
        else if (difficulty == 0)
            ai_owner_state->requested_maneuver = (uint8)(0);
    }
    phase = ai_owner_state->behavior_phase;
    if (phase == 2)
    {
        value = ai_owner_state->requested_maneuver;
        if (value == 255)
        {
            value = ai_owner_state->behavior_kind;
            ai_owner_state->requested_maneuver = (uint8)(50);
            ai_owner_state->speed_mode = (uint8)(value == 10);
            ai_owner_state->behavior_phase = (uint8)(3);
        }
    }
    phase = ai_owner_state->behavior_phase;
    if (phase != 1)
        return;
    a = (sint16)ai_owner_state->behavior_timer;
    b = (sint16)motion_settings->turn_phase_ticks;
    result = (sint32)b < (sint32)a;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    ai_owner_state->behavior_phase = (uint8)(2);
    difficulty = game_runtime.selection.difficulty & 3;
    seed = random_seed;
    random = psyq_rand_next(&seed);
    random_seed = seed;
    angle = (random & 0x3FFF) - 8192;
    mode = controls_behavior->kind;
    if (mode == 1)
    {
        a = (sint16)controls_behavior->bank_limit;
        if (a == 0)
            a = 4096;
        tick = game_runtime.mission_control.ticks;
        if ((tick & 1) == 0)
            a = 0 - a;
        ai_owner_state->requested_angles[2] = (uint16)(a);
        value = ai_owner_state->behavior_kind;
        ai_owner_state->requested_angles[0] = (uint16)(2048);
        ai_owner_state->requested_maneuver = (uint8)(12);
        ai_owner_state->requested_angles[1] = (uint16)(0);
        ai_owner_state->speed_mode = (uint8)(value == 10 ? mode : 0);
        return;
    }
    mode = (sint16)game_runtime.selection.mission;
    if (mode == 32 || mode == 47 || mode == 53)
    {
        tick = game_runtime.mission_control.ticks;
        raw = pose_owner_object->pose.angles[2];
        angle = ((tick & 1) != 0 ? 0xFFFFF000 : 0x1000) - raw;
    }
    else if (difficulty == 3)
    {
        if (effect == 0)
            abort();
        value = effect->distance;
        if ((sint32)value < 1024)
            angle = (sint32)(angle << 16) >> 19;
        else if ((sint32)value < 2048)
            angle = (sint32)(angle << 16) >> 18;
        else if ((sint32)value < 4096)
            angle = (sint32)(angle << 16) >> 17;
    }
    else
    {
        if (difficulty == 0)
        {
            value = motion_settings->kind;
            if (value != 3)
                goto align_yaw;
        }
        value = motion_settings->kind;
        if (value != 0)
            goto create_turn;
    align_yaw:
        yaw = (sint16)pose_owner_object->pose.angles[2];
        raw = pose_owner_object->pose.angles[2];
        value = (sint32)yaw < 0 ? 0 - yaw : yaw;
        if ((sint32)value > 4096)
        {
            ai_owner_state->requested_angles[0] = (uint16)(0);
            ai_owner_state->requested_angles[1] = (uint16)(0);
            a = pose_owner_object->pose.angles[2];
            ai_owner_state->requested_maneuver = (uint8)(12);
            value = ai_owner_state->behavior_kind;
            ai_owner_state->requested_angles[2] = (uint16)(0 - a);
            ai_owner_state->speed_mode = (uint8)(value == 10);
            return;
        }
        angle = ((sint32)yaw > 0 ? 0xFFFFF000 : 0x1000) - raw;
    }
create_turn:
    a = motion_settings->response;
    shift = mission_ai_turn_profiles[difficulty].response_shift;
    b = mission_ai_turn_profiles[difficulty].pitch_bias;
    ai_owner_state->requested_maneuver = (uint8)(12);
    ai_owner_state->requested_angles[0] = (uint16)(b + (a << (shift & 31)));
    value = ai_owner_state->behavior_kind;
    ai_owner_state->requested_angles[1] = (uint16)(0);
    ai_owner_state->requested_angles[2] = (uint16)(angle);
    result = value == 10 ? 1 : 10;
    ai_owner_state->speed_mode = (uint8)(value == 10);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_behavior_11(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, flags;
    FUNCTION_MARKER(0x8004FBA4, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        ai_owner_state->requested_maneuver = (uint8)(31);
        return;
    }
    flags = ai_owner_state->flags;
    if ((flags & 0x400000) != 0)
        ai_owner_state->requested_behavior = (uint8)(255);
    return;
}

void mission_ai_update_behavior_14(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase;
    FUNCTION_MARKER(0x8004FCF0, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        ai_owner_state->requested_maneuver = (uint8)(25);
    }
    ai_owner_state->speed_mode = (uint8)(9);
    return;
}

void mission_ai_update_behavior_15(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 phase, value;
    FUNCTION_MARKER(0x8004FD38, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    phase = ai_owner_state->behavior_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        value = pose_owner_object->pose.position[1];
        pose_owner_object->pose.position[1] = (uint32)(value - 0x01000000);
        ai_owner_state->requested_maneuver = (uint8)(26);
    }
    ai_owner_state->speed_mode = (uint8)(9);
    return;
}

void mission_ai_update_behavior_16(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_other;
    uint32 actor_index_table;

    uint32 phase, selected, base, value, random, high, quotient, entry, result;
    MISSION_SCRIPT_REFERENCE script;
    sint64 product;
    FUNCTION_MARKER(0x8004FD9C, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    phase = ai_owner_state->behavior_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        base = 0;
        selected = mission_result_last_object();
        if (selected != 0xFFFFFFFF)
        {
            actor_index_table = 0;
            pose_owner_other = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
            value = pose_owner_other->pose.position[0];
            if ((sint32)value < 0)
                base = 3;
        }
        random = game_random();
        product = (sint64)(sint32)random * 0x55555556;
        high = (product >> 32);
        quotient = high - (uint32)((sint32)random >> 31);
        entry = base + random - quotient * 3;
        value = mission_ai_descent_entries[entry].x;
        pose_owner_object->pose.position[0] = (uint32)(value);
        value = mission_ai_descent_entries[entry].z;
        pose_owner_object->pose.angles[0] = (uint16)(4096);
        pose_owner_object->pose.angles[1] = (uint16)(0x2000);
        pose_owner_object->pose.angles[2] = (uint16)(0);
        pose_owner_object->pose.position[2] = (uint32)(value);
        ai_owner_state->behavior_phase = (uint8)(1);
    }
    phase = ai_owner_state->behavior_phase;
    if (phase == 1)
    {
        ai_owner_state->speed_mode = (uint8)(9);
        ai_owner_state->requested_maneuver = (uint8)(26);
        value = pose_owner_object->pose.position[1];
        if ((sint32)value < (sint32)0xFEC00000)
            ai_owner_state->behavior_phase = (uint8)(2);
        phase = ai_owner_state->behavior_phase;
    }
    if (phase == 2)
    {
        ai_owner_state->speed_mode = (uint8)(0);
        ai_owner_state->requested_maneuver = (uint8)(0);
        value = pose_owner_object->pose.position[2];
        if ((sint32)value < (sint32)0xE0000000)
        {
            ai_owner_state->behavior_phase = (uint8)(3);
            value = pose_owner_object->pose.position[0];
            ai_owner_state->target_position[1] = (uint32)(0xE7000000);
            ai_owner_state->target_position[2] = (uint32)(0xC7000000);
            ai_owner_state->target_position[0] = (uint32)(value);
        }
    }
    phase = ai_owner_state->behavior_phase;
    if (phase != 3)
        return;
    ai_owner_state->speed_mode = (uint8)(0);
    ai_owner_state->requested_maneuver = (uint8)(5);
    value = pose_owner_object->pose.position[1];
    result = (sint32)value < (sint32)0xE7000000;
    if (result != 0)
    {
        ai_owner_state->behavior_phase = (uint8)(4);
        script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
        mission_script_start_legacy(script);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_behavior_17(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    uint32 actor_index_table;

    uint32 selected, phase, first, second, third, distance, ticks, angle, oscillation, sine, cosine, x_product, z_product, value, result;
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x8004FF88, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    selected = ai_owner_state->behavior_actor;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    phase = ai_owner_state->behavior_phase;
    pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    if (phase == 0)
    {
        ai_owner_state->behavior_phase = (uint8)(1);
        ai_owner_state->requested_maneuver = (uint8)(4);
        ai_owner_state->speed_mode = (uint8)(0);
        phase = ai_owner_state->behavior_phase;
    }
    if (phase != 1)
        return;
    first = pose_owner_target->pose.position[0];
    second = pose_owner_target->pose.position[1];
    third = pose_owner_target->pose.position[2];
    ai_owner_state->target_position[0] = (uint32)(first);
    ai_owner_state->target_position[1] = (uint32)(second);
    ai_owner_state->target_position[2] = (uint32)(third);
    distance = ai_owner_state->origin_distance;
    if ((sint32)distance >= 8193)
    {
        ticks = game_runtime.mission_control.ticks;
        angle = pose_owner_object->pose.angles[1];
        oscillation = game_wave_at(((ticks << 8) & 0x7F00));
        sine = game_wave_at(((angle & 0x3FFF) << 1));
        x_product = sine * oscillation;
        angle = pose_owner_object->pose.angles[1];
        cosine = game_wave_at((((angle & 0x3FFF) + 4096) << 1));
        z_product = cosine * oscillation;
        value = pose_owner_object->pose.position[0];
        pose_owner_object->pose.position[0] = (uint32)(value + (uint32)((sint32)x_product >> 14));
        value = pose_owner_object->pose.position[2];
        pose_owner_object->pose.position[2] = (uint32)(value + (uint32)((sint32)z_product >> 14));
    }
    distance = ai_owner_state->origin_distance;
    result = (sint32)distance < 512;
    if (result != 0)
    {
        script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
        mission_script_start_legacy(script);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_behavior_18(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase;
    FUNCTION_MARKER(0x800500EC, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase != 0)
        return;
    ai_owner_state->requested_maneuver = (uint8)(27);
    ai_owner_state->speed_mode = (uint8)(9);
    ai_owner_state->behavior_phase = (uint8)(1);
    return;
}

void mission_ai_update_behavior_19(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase;
    FUNCTION_MARKER(0x80050134, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        ai_owner_state->steering_angles[0] = (uint16)(0);
        ai_owner_state->steering_angles[1] = (uint16)(0);
        ai_owner_state->steering_angles[2] = (uint16)(0);
        ai_owner_state->behavior_phase = (uint8)(1);
    }
    ai_owner_state->speed_mode = (uint8)(8);
    ai_owner_state->requested_maneuver = (uint8)(0);
    return;
}

void mission_ai_update_behavior_20(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, mission, ticks, flags, signed_ticks, result;
    FUNCTION_MARKER(0x80050184, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        mission = (sint16)game_runtime.selection.mission;
        ai_owner_state->requested_maneuver = (uint8)(50);
        if (mission == 7)
            ticks = 150;
        else if (mission == 31)
            ticks = 360;
        else if (mission == 41)
            ticks = index == 2 ? 240 : 420;
        else
            ticks = mission == 50 ? 210 : 40;
        ai_owner_state->behavior_timer = (uint16)(ticks);
        ai_owner_state->behavior_phase = (uint8)(1);
    }
    else if (phase == 1)
    {
        ticks = (sint16)ai_owner_state->behavior_timer;
        if (ticks == 0)
            ai_owner_state->behavior_phase = (uint8)(2);
    }
    else if (phase == 2)
    {
        ai_owner_state->requested_maneuver = (uint8)(52);
        ai_owner_state->target_position[1] = (uint32)(0);
        ai_owner_state->target_position[0] = (uint32)(0);
        mission = (sint16)game_runtime.selection.mission;
        ai_owner_state->target_position[2] = (uint32)(mission == 31 ? 0x200000 : 0x800000);
        ai_owner_state->behavior_phase = (uint8)(3);
    }
    else
    {
        flags = ai_owner_state->flags;
        if ((flags & 0x400000) != 0)
        {
            ai_owner_state->requested_behavior = (uint8)(255);
            mission_primary_binding(index & 0x03FFFFFFu)->behavior_index = (uint32)(0xFFFFFFFF);
        }
    }
    ai_owner_state->speed_mode = (uint8)(9);
    signed_ticks = (sint16)ai_owner_state->behavior_timer;
    ticks = ai_owner_state->behavior_timer;
    result = ticks - 1;
    if ((sint32)signed_ticks > 0)
        ai_owner_state->behavior_timer = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_behavior_21(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, flags, value, distance, counter, mission, kind;
    MISSION_ACTOR_TARGET *object;
    FUNCTION_MARKER(0x800502D4, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        ai_owner_state->requested_maneuver = (uint8)(0);
        flags = game_runtime.mission_control.mode;
        if ((flags & 0x800) != 0)
        {
            value = ai_owner_state->behavior_timer;
            ai_owner_state->behavior_timer = (uint16)(value + 1);
        }
        value = (sint16)ai_owner_state->behavior_timer;
        if ((sint32)value >= 151)
            ai_owner_state->behavior_phase = (uint8)(1);
    }
    else if (phase == 1)
    {
        ai_owner_state->requested_maneuver = (uint8)(12);
        ai_owner_state->requested_angles[2] = (uint16)(2048);
        ai_owner_state->requested_angles[0] = (uint16)(1024);
        ai_owner_state->requested_angles[1] = (uint16)(0);
        ai_owner_state->behavior_phase = (uint8)(2);
    }
    else if (phase == 2)
    {
        flags = ai_owner_state->flags;
        if ((flags & 0x400000) != 0)
            ai_owner_state->behavior_phase = (uint8)(3);
    }
    else if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(0);
    flags = game_runtime.mission_control.mode;
    if ((flags & 0x800) != 0)
    {
        object = mission_actor_target(index);
        distance = (object->distance);
        value = (sint32)distance < 4097;
        if (value == 0)
        {
            flags = (object->flags);
            value = flags & 2;
        }
        if (value != 0)
        {
            counter = game_runtime.metrics.proximity_count;
            if ((counter & 15) == 0)
            {
                mission = (sint16)game_runtime.selection.mission;
                kind = mission == 7 ? 7 : mission == 44 ? 8 : 6;
                hud_notice_show(kind);
            }
            game_runtime.metrics.proximity_active = (uint8)(1);
            counter = game_runtime.metrics.proximity_count;
            game_runtime.metrics.proximity_count = (uint16)(counter + 1);
        }
        else
        {
            game_runtime.metrics.proximity_active = (uint8)(0);
            counter = game_runtime.metrics.proximity_count;
            game_runtime.metrics.proximity_count = (uint16)(counter - 1);
        }
    }
    flags = game_runtime.mission_control.mode;
    if ((flags & 0x2000) != 0)
    {
        value = game_runtime.metrics.special_result;
        if (value == 1)
        {
            value = game_runtime.mission_control.ticks;
            if ((value & 15) == 0)
            {
                mission = (sint16)game_runtime.selection.mission;
                kind = mission == 7 ? 15 : mission == 44 ? 16 : 14;
                hud_notice_show(kind);
            }
        }
    }
    ai_owner_state->speed_mode = (uint8)(8);
    return;
}

void mission_ai_update_behavior_22(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, flags;
    FUNCTION_MARKER(0x80050534, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->behavior_phase;
    if (phase == 0)
    {
        ai_owner_state->requested_maneuver = (uint8)(0);
        ai_owner_state->behavior_phase = (uint8)(1);
    }
    else if (phase == 1)
    {
        ai_owner_state->requested_maneuver = (uint8)(12);
        ai_owner_state->requested_angles[2] = (uint16)(0xF800);
        ai_owner_state->requested_angles[0] = (uint16)(2048);
        ai_owner_state->requested_angles[1] = (uint16)(0);
        ai_owner_state->behavior_phase = (uint8)(2);
    }
    else if (phase == 2)
    {
        flags = ai_owner_state->flags;
        if ((flags & 0x400000) != 0)
            ai_owner_state->behavior_phase = (uint8)(3);
    }
    else if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(0);
    ai_owner_state->speed_mode = (uint8)(8);
    return;
}

void mission_ai_update_behaviors(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 index, value, requested;
    FUNCTION_MARKER(0x80046DFC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((~actor_traits_object->flags & 0xC000) != 0 || ai_owner_state->behavior_kind == 6)
            continue;
        requested = ai_owner_state->requested_behavior;
        value = ai_owner_state->behavior_state;
        if (value != requested)
        {
            ai_owner_state->behavior_state = (uint8)(requested);
            ai_owner_state->behavior_phase = (uint8)(0);
        }
        value = ai_owner_state->behavior_state;
        switch (value)
        {
            case 0:
                mission_ai_update_behavior_00(index);
                break;
            case 1:
                mission_ai_update_behavior_01(index);
                break;
            case 2:
                mission_ai_update_behavior_02(index);
                break;
            case 3:
                mission_ai_update_behavior_03(index);
                break;
            case 4:
                mission_ai_update_behavior_04(index);
                break;
            case 5:
                mission_ai_update_behavior_05(index);
                break;
            case 7:
                mission_ai_update_behavior_07(index);
                break;
            case 10:
                mission_ai_update_behavior_10(index);
                break;
            case 11:
                mission_ai_update_behavior_11(index);
                break;
            case 12:
                mission_ai_update_behavior_12(index);
                break;
            case 14:
                mission_ai_update_behavior_14(index);
                break;
            case 15:
                mission_ai_update_behavior_15(index);
                break;
            case 16:
                mission_ai_update_behavior_16(index);
                break;
            case 17:
                mission_ai_update_behavior_17(index);
                break;
            case 18:
                mission_ai_update_behavior_18(index);
                break;
            case 19:
                mission_ai_update_behavior_19(index);
                break;
            case 20:
                mission_ai_update_behavior_20(index);
                break;
            case 21:
                mission_ai_update_behavior_21(index);
                break;
            case 22:
                mission_ai_update_behavior_22(index);
                break;
            default:
                break;
        }
        requested = ai_owner_state->requested_behavior;
        if (requested == 255)
        {
            value = ai_owner_state->flags;
            ai_owner_state->behavior_state = (uint8)(requested);
            ai_owner_state->flags = (uint32)(value | 0x200000);
        }
    }
    return;
}

void mission_ai_update_behavior_01(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_ACTOR *pose_owner_descriptor;
    MISSION_ACTOR_MOTION *actor_motion_descriptor;

    uint32 actor_index_table;

    uint32 slot, flag, a, b, i;
    FUNCTION_MARKER(0x8004ED9C, "ACE3.BIN");
    ai_owner_object = mission_primary_ai((0) + (index));
    slot = ai_owner_object->behavior_actor;
    actor_index_table = 0;
    flag = ai_owner_object->behavior_phase;
    pose_owner_descriptor = mission_actor((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
    actor_motion_descriptor = mission_actor_motion((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
    if (flag == 0)
        ai_owner_object->behavior_phase = (uint8)(1);
    for (i = 0; i < 3; ++i)
    {
        a = pose_owner_descriptor->pose.position[i];
        b = actor_motion_descriptor->velocity[i];
        ai_owner_object->target_position[i] = (uint32)(a + b);
    }
    a = (pose_owner_descriptor->pose.angles[0] | ((uint32)pose_owner_descriptor->pose.angles[1] << 16));
    {
        uint32 ai_packed_word = (a);
        ai_owner_object->requested_angles[0] = (uint16)ai_packed_word;
        ai_owner_object->requested_angles[1] = (uint16)(ai_packed_word >> 16);
    }
    ai_owner_object->requested_angles[2] = pose_owner_descriptor->pose.angles[2];
    mission_ai_limit_position(index, ai_owner_object->target_position);
    ai_owner_object->requested_maneuver = (uint8)(4);
    ai_owner_object->speed_mode = (uint8)(4);
    return;
}

void mission_ai_update_behavior_02(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_ACTOR *pose_owner_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_own;
    uint32 actor_index_table;

    uint32 slot, flag, value, result;
    FUNCTION_MARKER(0x8004EE68, "ACE3.BIN");
    ai_owner_object = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    slot = ai_owner_object->behavior_actor;
    actor_traits_own = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    flag = ai_owner_object->behavior_phase;
    pose_owner_descriptor = mission_actor((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
    if (flag == 0)
        ai_owner_object->behavior_phase = (uint8)(1);
    game_pose_transform_offset(&pose_owner_descriptor->pose, ai_owner_object->target_position, ai_owner_object->aim_position);
    mission_ai_limit_position(index, ai_owner_object->target_position);
    if ((sint32)ai_owner_object->origin_distance < 0x2000)
    {
        value = (*mission_actor_flags(index));
        *mission_actor_flags(index) = (uint32)(value | 0x20);
        ai_owner_object->requested_maneuver = (uint8)(0);
    }
    else
        ai_owner_object->requested_maneuver = (uint8)(4);
    value = actor_traits_own->flags;
    result = 7;
    if ((value & 0x40) == 0 && ai_owner_object->behavior_actor == 0)
        result = 4;
    ai_owner_object->speed_mode = (uint8)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_00(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 offset, mode, value;
    FUNCTION_MARKER(0x80049E28, "ACE3.BIN");
    offset = index * 184;
    ai_owner_state = mission_primary_ai((0) + (index));
    mode = ai_owner_state->maneuver_phase;
    if (mode == 1)
    {
        (void)(uint32)(offset);
        return;
    }
    if (mode == 0)
    {
        value = mission_ai_request_level_angles(index);
        if (value == 0)
            ai_owner_state->maneuver_phase = (uint8)(1);
        {
            (void)(uint32)(value);
            return;
        }
    }
    if (mode == 2)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_maneuver_05_standard(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 value, mode, speed;
    GAME_ANGLES output;
    FUNCTION_MARKER(0x80049A1C, "ACE3.BIN");
    value = (*mission_actor_flags(index));
    *mission_actor_flags(index) = (uint32)(value | 0x20);
    ai_owner_state = mission_primary_ai((0) + (index));
    mode = ai_owner_state->maneuver_phase;
    if (mode == 0)
    {
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (mode == 1)
    {
        speed = (sint16)ai_owner_state->origin_angle;
        {
            (void)(mission_ai_steer_direction(index, speed, &output, ai_owner_state->origin_direction));
            return;
        }
    }
    if (mode == 2)
    {
        value = mission_ai_request_level_angles(index);
        if (value == 0)
        {
            ai_owner_state->maneuver_phase = (uint8)(3);
            return;
        }
        {
            (void)(uint32)(value);
            return;
        }
    }
    if (mode == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_40(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 mode, variant;
    FUNCTION_MARKER(0x8004B308, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    mode = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (mode == 1)
    {
        ai_owner_state->maneuver_phase = (uint8)(2);
        return;
    }
    if (mode >= 2)
    {
        if (mode == 2)
        {
            (void)(mission_actor_begin_deactivation(index));
            return;
        }
        ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    ai_owner_state->maneuver_phase = (uint8)(1);
    variant = (sint16)actor_traits_object->kind == 44;
    {
        (void)(effect_emit_drift_group(variant, actor_motion_object->velocity, pose_owner_object->pose.position));
        return;
    }
}

void mission_ai_update_maneuver_41(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 kind, result, limit;
    sint32 tick;
    FUNCTION_MARKER(0x8004B3C8, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_record = mission_primary_ai((0) + (index));
    kind = ai_owner_record->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (kind == 1)
    {
        tick = (sint16)ai_owner_record->maneuver_ticks;
        effect_emit_vector_periodic(tick, 1, actor_motion_object->velocity, pose_owner_object->pose.position);
        limit = pose_owner_object->pose.position[0];
        tick = (sint16)ai_owner_record->maneuver_ticks;
        result = (limit & 31) + 16;
        if (tick >= (sint32)result)
        {
            effect_emit_drift_group(0, actor_motion_object->velocity, pose_owner_object->pose.position);
            sound_play_spatial(27, 0, pose_owner_object->pose.position);
            result = 2;
            ai_owner_record->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (kind == 0)
    {
        ai_owner_record->maneuver_phase = (uint8)(1);
        return;
    }
    if (kind == 2)
    {
        (void)(mission_actor_begin_deactivation(index));
        return;
    }
    ai_owner_record->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_maneuver_06(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 flags, kind, result, first, second, distance;
    FUNCTION_MARKER(0x80049AF8, "ACE3.BIN");
    actor_index_table = 0;
    flags = (*mission_actor_flags(index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    *mission_actor_flags(index) = (uint32)(flags | 0x20);
    ai_owner_record = mission_primary_ai((0) + (index));
    kind = ai_owner_record->maneuver_phase;
    switch (kind)
    {
        case 0:
            result = mission_ai_request_level_angles(index);
            if (result == 0)
            {
                result = 1;
                ai_owner_record->maneuver_phase = (uint8)(result);
            }
            break;
        case 1:
            result = mission_ai_turn_to_position(index, ai_owner_record->target_position);
            if (result == 0)
            {
                result = 2;
                ai_owner_record->maneuver_phase = (uint8)(result);
            }
            break;
        case 2:
            result = mission_ai_request_level_angles(index);
            if (result == 0)
            {
                result = 3;
                ai_owner_record->maneuver_phase = (uint8)(result);
            }
            break;
        case 3:
            mission_ai_steer_level_to_position(index, ai_owner_record->target_position);
            first = pose_owner_object->pose.position[1];
            second = ai_owner_record->target_position[1];
            distance = first - second;
            if ((sint32)distance < 0)
                distance = 0 - distance;
            result = (sint32)distance > 0x3FFFF;
            if (result == 0)
                ai_owner_record->maneuver_phase = (uint8)(0);
            break;
        case 4:
            result = mission_ai_request_level_angles(index);
            if (result == 0)
            {
                result = 255;
                ai_owner_record->requested_maneuver = (uint8)(result);
            }
            break;
        default:
            result = 0x80020000;
            ai_owner_record->maneuver_phase = (uint8)(0);
            break;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_12(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_parameters;
    uint32 type, mission, kind, result, value, first;
    FUNCTION_MARKER(0x8004A678, "ACE3.BIN");
    motion_parameters = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    type = motion_parameters->kind;
    ai_owner_record = mission_primary_ai((0) + (index));
    if (type >= 2)
    {
        mission = (sint16)game_runtime.selection.mission;
    }
    if (type < 2 || mission == 32 || mission == 47 || mission == 53)
    {
        kind = ai_owner_record->maneuver_phase;
        switch (kind)
        {
            case 0:
                value = ai_owner_record->requested_angles[2];
                ai_owner_record->steering_angles[1] = (uint16)(0);
                ai_owner_record->steering_angles[0] = (uint16)(0);
                ai_owner_record->maneuver_phase = (uint8)(2);
                ai_owner_record->steering_angles[2] = (uint16)(value);
                /* Fall through to the live countdown read */
            case 2:
                result = (sint16)ai_owner_record->steering_angles[2];
                if (result == 0)
                {
                    value = ai_owner_record->requested_angles[0];
                    result = 3;
                    ai_owner_record->maneuver_phase = (uint8)(result);
                    ai_owner_record->steering_angles[0] = (uint16)(value);
                }
                {
                    (void)(uint32)(result);
                    return;
                }
            case 3:
                result = (sint16)ai_owner_record->steering_angles[0];
                if (result != 0)
                {
                    (void)(uint32)(result);
                    return;
                }
                ai_owner_record->maneuver_phase = (uint8)(4);
                return;
            case 4:
                ai_owner_record->requested_maneuver = (uint8)(255);
                return;
            default:
                result = kind < 3 ? 1 : 255;
                ai_owner_record->maneuver_phase = (uint8)(0);
                {
                    (void)(uint32)(result);
                    return;
                }
        }
    }
    kind = ai_owner_record->maneuver_phase;
    if (kind >= 2)
    {
        if (kind == 4)
            ai_owner_record->requested_maneuver = (uint8)(255);
        else
            ai_owner_record->maneuver_phase = (uint8)(0);
        return;
    }
    if (kind == 0)
    {
        first = (ai_owner_record->requested_angles[0] | ((uint32)ai_owner_record->requested_angles[1] << 16));
        {
            uint32 ai_packed_word = (first);
            ai_owner_record->steering_angles[0] = (uint16)ai_packed_word;
            ai_owner_record->steering_angles[1] = (uint16)(ai_packed_word >> 16);
        }
        ai_owner_record->steering_angles[2] = ai_owner_record->requested_angles[2];
        ai_owner_record->maneuver_phase = (uint8)(1);
    }
    result = (sint16)ai_owner_record->steering_angles[0];
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = (sint16)ai_owner_record->steering_angles[2];
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    ai_owner_record->maneuver_phase = (uint8)(4);
    return;
}

void mission_ai_update_maneuver_30(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_parameters;
    uint32 actor_index_table;

    uint32 kind, upper, lower, result, altitude, position[3];
    FUNCTION_MARKER(0x8004AFAC, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_record = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    motion_parameters = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    kind = ai_owner_record->maneuver_phase;
    upper = motion_parameters->max_altitude;
    lower = motion_parameters->min_altitude;
    if (kind == 0)
    {
        result = mission_ai_step_level_attitude(index);
        if (result != 0)
        {
            (void)(uint32)(result);
            return;
        }
        altitude = pose_owner_object->pose.position[1];
        result = (sint32)altitude < (sint32)lower;
        if ((sint32)upper < (sint32)altitude)
        {
            ai_owner_record->maneuver_phase = (uint8)(1);
            {
                (void)(uint32)(result);
                return;
            }
        }
        if (result != 0)
        {
            ai_owner_record->maneuver_phase = (uint8)(1);
            return;
        }
        ai_owner_record->maneuver_phase = (uint8)(2);
        return;
    }
    if (kind == 1)
    {
        altitude = pose_owner_object->pose.position[1];
        if ((sint32)upper < (sint32)altitude)
        {
            position[2] = 0;
            position[0] = 0;
            position[1] = lower;
            {
                (void)(mission_ai_steer_level_to_position(index, position));
                return;
            }
        }
        if ((sint32)altitude < (sint32)lower)
        {
            position[2] = 0;
            position[0] = 0;
            position[1] = upper;
            {
                (void)(mission_ai_steer_level_to_position(index, position));
                return;
            }
        }
        ai_owner_record->maneuver_phase = (uint8)(2);
        return;
    }
    if (kind == 2)
        ai_owner_record->requested_maneuver = (uint8)(255);
    else
        ai_owner_record->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_42(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 kind, flags, value, random, mode, mission, threshold, altitude, tick, result;
    FUNCTION_MARKER(0x8004B4D8, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_record = mission_primary_ai((0) + (index));
    kind = ai_owner_record->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (kind == 0)
    {
        ai_owner_record->steering_angles[2] = (uint16)(0);
        ai_owner_record->steering_angles[1] = (uint16)(0);
        ai_owner_record->steering_angles[0] = (uint16)(0);
        ai_owner_record->angular_rate[2] = (uint16)(0);
        ai_owner_record->angular_rate[1] = (uint16)(0);
        ai_owner_record->angular_rate[0] = (uint16)(0);
        flags = actor_traits_object->events;
        actor_motion_object->local_velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->angular_delta[0] = (uint16)(0);
        actor_motion_object->angular_delta[1] = (uint16)(0);
        actor_motion_object->angular_delta[2] = (uint16)(0);
        actor_traits_object->events = (uint32)(flags | 0x800000);
        random = game_random();
        mode = game_runtime.mission_control.ticks & 3;
        switch (mode)
        {
            case 0:
                actor_motion_object->angular_delta[2] = (uint16)((random & 1023) - 512);
                break;
            case 1:
                actor_motion_object->angular_delta[1] = (uint16)((random & 255) - 128);
                break;
            case 2:
                actor_motion_object->angular_delta[0] = (uint16)((random & 255) - 128);
                break;
            case 3:
                actor_motion_object->angular_delta[0] = (uint16)(0xFFF0);
                break;
        }
        ai_owner_record->maneuver_phase = (uint8)(1);
        return;
    }
    if (kind == 2)
    {
        (void)(mission_actor_begin_deactivation(index));
        return;
    }
    if (kind != 1)
    {
        ai_owner_record->maneuver_phase = (uint8)(0);
        return;
    }
    flags = actor_traits_object->events;
    value = actor_motion_object->velocity[1];
    actor_traits_object->events = (uint32)(flags | 0x800000);
    actor_motion_object->velocity[1] = (uint32)(value + 1024);
    tick = (sint16)ai_owner_record->maneuver_ticks;
    effect_emit_vector_periodic(tick, 0, actor_motion_object->velocity, pose_owner_object->pose.position);
    mission = (sint16)game_runtime.selection.mission;
    threshold = mission == 32 || mission == 47 || mission == 53 ? 0 : 0xFF800000;
    altitude = pose_owner_object->pose.position[1];
    result = (sint32)threshold < (sint32)altitude;
    if (result != 0)
    {
        effect_emit_drift_group(0, actor_motion_object->velocity, pose_owner_object->pose.position);
        sound_play_spatial(27, 0, pose_owner_object->pose.position);
        ai_owner_record->maneuver_phase = (uint8)(2);
        return;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_43(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 kind, value, flags, speed, tick, altitude, result;
    sint32 angle;
    FUNCTION_MARKER(0x8004B6E4, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_record = mission_primary_ai((0) + (index));
    kind = ai_owner_record->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (kind == 0)
    {
        ai_owner_record->steering_angles[2] = (uint16)(0);
        ai_owner_record->steering_angles[1] = (uint16)(0);
        ai_owner_record->steering_angles[0] = (uint16)(0);
        ai_owner_record->angular_rate[2] = (uint16)(0);
        ai_owner_record->angular_rate[1] = (uint16)(0);
        ai_owner_record->angular_rate[0] = (uint16)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        ai_owner_record->maneuver_phase = (uint8)(1);
        return;
    }
    if (kind == 2)
    {
        (void)(mission_actor_begin_deactivation(index));
        return;
    }
    if (kind != 1)
    {
        ai_owner_record->maneuver_phase = (uint8)(0);
        return;
    }
    angle = (sint16)pose_owner_object->pose.angles[2];
    value = pose_owner_object->pose.angles[2];
    if (angle < 0 && angle >= -1023)
        pose_owner_object->pose.angles[2] = (uint16)(value - 4);
    else if (value < 1024)
        pose_owner_object->pose.angles[2] = (uint16)(value + 4);
    else
    {
        angle = (sint16)pose_owner_object->pose.angles[0];
        value = pose_owner_object->pose.angles[0];
        if ((angle < 0 && angle >= -2047) || value < 2048)
            pose_owner_object->pose.angles[0] = (uint16)(value - 4);
    }
    flags = actor_traits_object->events;
    speed = actor_motion_object->velocity[1];
    actor_traits_object->events = (uint32)(flags | 0x800000);
    actor_motion_object->velocity[1] = (uint32)(speed + 669);
    tick = (sint16)ai_owner_record->maneuver_ticks;
    effect_emit_vector_periodic(tick, 0, actor_motion_object->velocity, pose_owner_object->pose.position);
    altitude = pose_owner_object->pose.position[1];
    result = (sint32)altitude > (sint32)0xFF800000;
    if (result != 0)
    {
        effect_emit_drift_group(0, actor_motion_object->velocity, pose_owner_object->pose.position);
        sound_play_spatial(27, 0, pose_owner_object->pose.position);
        ai_owner_record->maneuver_phase = (uint8)(2);
        return;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_07(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_other;
    uint32 actor_index_table;

    uint32 other_index, mode, value, current, raw, difference, offset, i, result;
    uint32 *flags;
    FUNCTION_MARKER(0x8004986C, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    other_index = ai_owner_state->behavior_actor;
    flags = mission_actor_flags(index);
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    value = (*flags);
    pose_owner_other = mission_actor((actor_index_table + ((uint32)((sint32)((other_index << 2)) >> 2))));
    *flags = (value | 0x20);
    mode = ai_owner_state->maneuver_phase;
    if (mode == 0)
    {
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (mode != 1)
    {
        if (mode == 2)
            ai_owner_state->requested_maneuver = (uint8)(255);
        else
            ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    for (i = 0; i < 3; ++i)
    {
        offset = i == 0 ? 24 : i == 1 ? 28 : 26;
        value = (sint16)pose_owner_other->pose.angles[(offset - 24u) >> 1];
        current = (sint16)pose_owner_object->pose.angles[(offset - 24u) >> 1];
        raw = pose_owner_object->pose.angles[(offset - 24u) >> 1];
        difference = (value - current) & 0x3FFF;
        if (difference >= 0x2001)
            difference -= 0x4000;
        value = difference;
        if ((sint32)value < 0)
            value += 3;
        value = (sint32)value >> 2;
        pose_owner_object->pose.angles[(offset - 24u) >> 1] = (uint16)(raw + value);
    }
    for (i = 0; i < 3; ++i)
    {
        value = ai_owner_state->target_position[i];
        current = pose_owner_object->pose.position[i];
        result = (sint32)(value - current) >> 8;
        pose_owner_object->pose.position[i] = (uint32)(current + result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_maneuver_05_kind3(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_data;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_data;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 flags, phase, target, current, x, z, angle, difference, signed_difference, magnitude, limit, step, result;
    uint32 *slot;
    FUNCTION_MARKER(0x80049C4C, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    slot = mission_actor_flags(index);
    flags = (*slot);
    selected_controls_data = mission_primary_controls(index & 0x03FFFFFFu);
    controls_data = &selected_controls_data;
    *slot = (flags | 32);
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    if (phase == 0)
    {
        actor_motion_object->angular_delta[2] = (uint16)(0);
        actor_motion_object->angular_delta[1] = (uint16)(0);
        actor_motion_object->angular_delta[0] = (uint16)(0);
        ai_owner_state->steering_angles[2] = (uint16)(0);
        ai_owner_state->steering_angles[1] = (uint16)(0);
        ai_owner_state->steering_angles[0] = (uint16)(0);
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 1)
    {
        target = ai_owner_state->target_position[0];
        current = pose_owner_object->pose.position[0];
        z = ai_owner_state->target_position[2];
        x = (sint32)(target - current) >> 12;
        current = pose_owner_object->pose.position[2];
        z = (sint32)(z - current) >> 12;
        angle = game_vector_atan2(x, z);
        current = pose_owner_object->pose.angles[1];
        difference = (angle - current) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        limit = (sint16)controls_data->yaw_rate;
        magnitude = signed_difference;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        step = controls_data->yaw_rate;
        if ((sint32)magnitude < (sint32)limit)
            pose_owner_object->pose.angles[1] = (uint16)(current + difference);
        else if ((sint32)signed_difference > 0)
            pose_owner_object->pose.angles[1] = (uint16)(current + step);
        else if ((sint32)signed_difference < 0)
            pose_owner_object->pose.angles[1] = (uint16)(current - step);
        target = ai_owner_state->target_position[1];
        current = pose_owner_object->pose.position[1];
        difference = target - current;
        if ((sint32)difference > 0x5000)
            result = current + 0x5000;
        else if ((sint32)difference < -0x5000)
            result = current - 0x5000;
        else
            result = current + difference;
        pose_owner_object->pose.position[1] = (uint32)(result);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
            ai_owner_state->maneuver_phase = (uint8)(0);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 3)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
            ai_owner_state->requested_maneuver = (uint8)(255);
        ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_08(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 offset, phase;
    FUNCTION_MARKER(0x80049EC4, "ACE3.BIN");
    offset = index * 184;
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    if (phase == 1)
    {
        (void)(uint32)(offset);
        return;
    }
    if (phase == 0)
    {
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 2)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_09(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    GAME_ANGLES output;
    uint32 flags, phase, speed, result;
    uint32 *slot;
    FUNCTION_MARKER(0x80049F38, "ACE3.BIN");
    slot = mission_actor_flags(index);
    flags = (*slot);
    *slot = (flags | 32);
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    ai_owner_state->speed_mode = (uint8)(1);
    if (phase == 0)
    {
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 1)
    {
        speed = (sint16)ai_owner_state->origin_angle;
        {
            (void)(mission_ai_steer_direction(index, speed, &output, ai_owner_state->origin_direction));
            return;
        }
    }
    if (phase == 2)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
        {
            ai_owner_state->maneuver_phase = (uint8)(3);
            result = 3;
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_51(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 phase, x, z, target_x, target_z, angle, current, difference, signed_difference, value, first, second, result;
    FUNCTION_MARKER(0x8004A0C0, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    phase = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        ai_owner_state->maneuver_phase = (uint8)(1);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        ai_owner_state->steering_angles[0] = (uint16)(0);
        ai_owner_state->steering_angles[1] = (uint16)(0);
        ai_owner_state->steering_angles[2] = (uint16)(0);
        return;
    }
    if (phase == 1)
    {
        target_x = ai_owner_state->target_position[0];
        x = pose_owner_object->pose.position[0];
        target_z = ai_owner_state->target_position[2];
        z = pose_owner_object->pose.position[2];
        x = (sint32)(target_x - x) >> 12;
        z = (sint32)(target_z - z) >> 12;
        angle = game_vector_atan2(x, z);
        current = pose_owner_object->pose.angles[1];
        difference = (angle - current) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        if ((sint32)signed_difference > 64)
            value = current + 64;
        else if ((sint32)signed_difference < -64)
            value = current - 64;
        else
            value = current + difference;
        pose_owner_object->pose.angles[1] = (uint16)(value);
        result = difference << 16;
        if (result == 0)
        {
            result = 2;
            ai_owner_state->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        angle = pose_owner_object->pose.angles[1];
        value = game_wave_at(((angle & 0x3FFF) << 1));
        first = value * 5;
        angle = pose_owner_object->pose.angles[1];
        first = (sint32)first >> 2;
        x = pose_owner_object->pose.position[0];
        value = game_wave_at((((angle & 0x3FFF) + 4096) << 1));
        x += first;
        second = value * 5;
        z = pose_owner_object->pose.position[2];
        second = (sint32)second >> 2;
        pose_owner_object->pose.position[0] = (uint32)(x);
        z += second;
        pose_owner_object->pose.position[2] = (uint32)(z);
        target_x = ai_owner_state->target_position[0];
        target_z = ai_owner_state->target_position[2];
        difference = (sint32)(target_x - x) >> 12;
        second = (sint32)(target_z - z) >> 12;
        if ((sint32)difference < 0)
            difference = 0 - difference;
        result = (sint32)difference < 5;
        if (result == 0)
        {
            (void)(uint32)(result);
            return;
        }
        if ((sint32)second < 0)
            second = 0 - second;
        result = (sint32)second < 5;
        if (result == 0)
        {
            (void)(uint32)(result);
            return;
        }
        pose_owner_object->pose.position[0] = (uint32)(target_x);
        target_z = ai_owner_state->target_position[2];
        pose_owner_object->pose.position[2] = (uint32)(target_z);
        ai_owner_state->maneuver_phase = (uint8)(3);
        return;
    }
    if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_10(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, result;
    FUNCTION_MARKER(0x8004A2D8, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    if (phase == 0)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
        {
            result = 0x4000;
            ai_owner_state->maneuver_phase = (uint8)(1);
            ai_owner_state->steering_angles[0] = (uint16)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 1)
    {
        result = (sint16)ai_owner_state->steering_angles[0];
        if (result == 0)
        {
            result = 2;
            ai_owner_state->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_11(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 phase, result;
    FUNCTION_MARKER(0x8004A394, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    if (phase == 0)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
        {
            result = 0x2000;
            ai_owner_state->maneuver_phase = (uint8)(1);
            ai_owner_state->steering_angles[0] = (uint16)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 1)
    {
        result = (sint16)ai_owner_state->steering_angles[0];
        if (result == 0)
        {
            result = 2;
            ai_owner_state->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        result = mission_ai_request_level_angles(index);
        if (result == 0)
        {
            result = 3;
            ai_owner_state->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_13(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 phase, position, angle, value, result;
    FUNCTION_MARKER(0x8004A474, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        position = pose_owner_object->pose.position[2];
        if ((sint32)position > 0)
        {
            angle = pose_owner_object->pose.angles[2];
            value = 0xFFFFC000 - angle;
        }
        else if ((sint32)position < 0)
        {
            angle = pose_owner_object->pose.angles[2];
            value = 0x4000 - angle;
        }
        else
        {
            position = pose_owner_object->pose.position[0];
            value = (position & 1) != 0 ? 0xFFFFC000 : 0x4000;
        }
        ai_owner_state->steering_angles[2] = (uint16)(value);
        ai_owner_state->maneuver_phase = (uint8)(1);
    }
    else if (phase != 1)
    {
        if (phase == 2)
            ai_owner_state->requested_maneuver = (uint8)(255);
        else
            ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    result = (sint16)ai_owner_state->steering_angles[2];
    if (result == 0)
    {
        result = 2;
        ai_owner_state->maneuver_phase = (uint8)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_14(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 phase, position, angle, value, flags, result;
    FUNCTION_MARKER(0x8004A56C, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        position = pose_owner_object->pose.position[2];
        if ((sint32)position > 0)
        {
            angle = pose_owner_object->pose.angles[2];
            value = 0xFFFFC000 - angle;
        }
        else if ((sint32)position < 0)
        {
            angle = pose_owner_object->pose.angles[2];
            value = 0x4000 - angle;
        }
        else
        {
            position = pose_owner_object->pose.position[0];
            value = (position & 1) != 0 ? 0xFFFFC000 : 0x4000;
        }
        ai_owner_state->steering_angles[2] = (uint16)(value);
        ai_owner_state->maneuver_phase = (uint8)(1);
    }
    else if (phase != 1)
    {
        if (phase == 2)
            ai_owner_state->requested_maneuver = (uint8)(255);
        else
            ai_owner_state->maneuver_phase = (uint8)(0);
        return;
    }
    actor_motion_object->local_velocity[1] = (uint32)(0xFFFEB000);
    flags = actor_traits_object->events;
    actor_traits_object->events = (uint32)(flags | 0x200000);
    result = (sint16)ai_owner_state->steering_angles[2];
    if (result == 0)
    {
        result = 2;
        actor_motion_object->local_velocity[1] = (uint32)(0);
        ai_owner_state->maneuver_phase = (uint8)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuver_25(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 mission, duration, speed, phase, counter, numerator, angle, sine, cosine, x, z, remaining, roll, result, height;
    FUNCTION_MARKER(0x8004AA3C, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    mission = (sint16)game_runtime.selection.mission;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    duration = mission == 26 ? 2700 : 7200;
    speed = mission == 26 ? 0x5000 : 0x4000;
    phase = ai_owner_state->maneuver_phase;
    ai_owner_state->speed_mode = (uint8)(9);
    if (phase == 0)
    {
        pose_owner_object->pose.position[0] = (uint32)(0xFDBE0000);
        pose_owner_object->pose.position[2] = (uint32)(0xF15CC000);
        pose_owner_object->pose.angles[0] = (uint16)(0);
        pose_owner_object->pose.angles[1] = (uint16)(0);
        pose_owner_object->pose.angles[2] = (uint16)(1024);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        pose_owner_object->pose.position[1] = (uint32)(0xFF59A000 - speed * duration);
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 1)
    {
        counter = (sint16)ai_owner_state->maneuver_ticks;
        numerator = counter << 15;
        angle = ((uint32)((sint32)numerator / (sint32)duration)) & 0x3FFF;
        sine = game_wave_at((angle << 1));
        cosine = game_wave_at(((angle + 4096) << 1));
        x = 0xFDBE0000 - (sine << 11);
        z = 0xF35CC000 - (cosine << 11);
        pose_owner_object->pose.position[0] = (uint32)(x);
        pose_owner_object->pose.position[2] = (uint32)(z);
        counter = (sint16)ai_owner_state->maneuver_ticks;
        remaining = duration - counter;
        if (counter == duration)
        {
            pose_owner_object->pose.position[1] = (uint32)(0xFF59A000);
            ai_owner_state->maneuver_phase = (uint8)(2);
        }
        else
            pose_owner_object->pose.position[1] = (uint32)(0xFF59A000 - speed * remaining);
        pose_owner_object->pose.angles[1] = (uint16)(angle - 4096);
        counter = (sint16)ai_owner_state->maneuver_ticks;
        roll = (sint16)pose_owner_object->pose.angles[2];
        remaining = duration - counter;
        result = (sint32)remaining < (sint32)roll;
        if (result != 0)
            pose_owner_object->pose.angles[2] = (uint16)(remaining);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        counter = (sint16)ai_owner_state->maneuver_ticks;
        height = pose_owner_object->pose.position[1];
        remaining = 304 - (counter - duration);
        x = pose_owner_object->pose.position[0];
        pose_owner_object->pose.position[0] = (uint32)(x - (remaining << 9));
        if ((sint32)height > (sint32)0xFFB9A000)
            pose_owner_object->pose.position[1] = (uint32)(0xFFB9A000);
        else if ((sint32)height < (sint32)0xFFB9A000)
            pose_owner_object->pose.position[1] = (uint32)(height + 0x8000);
        pose_owner_object->pose.position[2] = (uint32)(0xF15CC000);
        if (remaining == 0)
        {
            ai_owner_state->maneuver_phase = (uint8)(3);
            return;
        }
        return;
    }
    if (phase != 3)
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_26(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_data;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_data;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 actor_index_table;

    uint32 phase, value, velocity, decrement, position, result;
    FUNCTION_MARKER(0x8004AC90, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    phase = ai_owner_state->maneuver_phase;
    selected_controls_data = mission_primary_controls(index & 0x03FFFFFFu);
    controls_data = &selected_controls_data;
    ai_owner_state->speed_mode = (uint8)(9);
    if (phase == 0)
    {
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 1)
    {
        value = controls_data->cruise_speed;
        velocity = actor_motion_object->velocity[1];
        value = 0 - value;
        if ((sint32)value < (sint32)velocity)
        {
            decrement = controls_data->accel_limit;
            actor_motion_object->velocity[1] = (uint32)(velocity - decrement);
        }
        position = pose_owner_object->pose.position[1];
        result = (sint32)position < (sint32)0xE7000000;
        if (result != 0)
        {
            result = 2;
            ai_owner_state->maneuver_phase = (uint8)(result);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
    {
        actor_motion_object->velocity[1] = (uint32)(0);
        return;
    }
    if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_27(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_source;
    MISSION_ACTOR_MOTION *actor_motion_object;
    MISSION_ACTOR_MOTION *actor_motion_source;

    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_source;
    uint32 actor_index_table;

    ACTOR_POSE *sample;
    uint32 phase, i, factor, velocity, position, first, second, third, result;
    FUNCTION_MARKER(0x8004AD98, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    pose_owner_source = mission_actor(actor_index_table);
    actor_motion_source = mission_actor_motion(actor_index_table);
    actor_traits_source = mission_actor_traits(actor_index_table);
    phase = ai_owner_state->maneuver_phase;
    ai_owner_state->speed_mode = (uint8)(9);
    if (phase == 0)
    {
        for (i = 0; i < 64; ++i)
        {
            sample = &mission_actor_state.maneuver_history[i];
            factor = 64 - i;
            velocity = actor_motion_source->velocity[0];
            position = pose_owner_source->pose.position[0];
            sample->pose.position[0] = position - velocity * factor;
            velocity = actor_motion_source->velocity[1];
            position = pose_owner_source->pose.position[1];
            sample->pose.position[1] = position - velocity * factor;
            velocity = actor_motion_source->velocity[2];
            position = pose_owner_source->pose.position[2];
            sample->pose.position[2] = position - velocity * factor;
            memcpy(sample->pose.angles, pose_owner_source->pose.angles, sizeof(sample->pose.angles));
            mission_actor_state.maneuver_history_alive[i] = 1;
        }
        ai_owner_state->maneuver_phase = (uint8)(1);
        return;
    }
    if (phase == 1)
    {
        i = game_runtime.frame_counter & 63;
        sample = &mission_actor_state.maneuver_history[i];
        memcpy(pose_owner_object->pose.position, sample->pose.position, sizeof(sample->pose.position));
        memcpy(pose_owner_object->pose.angles, sample->pose.angles, sizeof(sample->pose.angles));
        if (!mission_actor_state.maneuver_history_alive[i])
            actor_traits_object->health = (uint16)(0);
        memcpy(sample->pose.position, pose_owner_source->pose.position, sizeof(sample->pose.position));
        memcpy(sample->pose.angles, pose_owner_source->pose.angles, sizeof(sample->pose.angles));
        result = (sint16)actor_traits_source->health;
        mission_actor_state.maneuver_history_alive[i] = (sint32)result > 0;
        first = actor_motion_source->velocity[0];
        second = actor_motion_source->velocity[1];
        third = actor_motion_source->velocity[2];
        actor_motion_object->velocity[0] = (uint32)(first);
        actor_motion_object->velocity[1] = (uint32)(second);
        actor_motion_object->velocity[2] = (uint32)(third);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 2)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_31(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 phase, mission, x, z, angle, current, difference, signed_difference, roll, value, result;
    FUNCTION_MARKER(0x8004B108, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (phase == 0)
    {
        mission = (sint16)game_runtime.selection.mission;
        x = pose_owner_object->pose.position[0];
        x = (sint32)x >> 12;
        if (mission == 32 || mission == 47 || mission == 53)
            x += 0xE000;
        z = pose_owner_object->pose.position[2];
        z = (sint32)z >> 12;
        if (mission == 32 || mission == 47 || mission == 53)
            z += 0x2000;
        angle = game_vector_atan2(x, z);
        current = pose_owner_object->pose.angles[1];
        difference = (angle - current - 0x2000) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        ai_owner_state->requested_angles[0] = (uint16)(difference);
        ai_owner_state->steering_angles[1] = (uint16)(0);
        ai_owner_state->steering_angles[0] = (uint16)(0);
        if ((sint32)signed_difference > 0)
        {
            roll = pose_owner_object->pose.angles[2];
            ai_owner_state->steering_angles[2] = (uint16)(0x1000 - roll);
        }
        else if ((sint32)signed_difference < 0)
        {
            roll = pose_owner_object->pose.angles[2];
            ai_owner_state->steering_angles[2] = (uint16)(0xFFFFF000 - roll);
        }
        else
            ai_owner_state->steering_angles[2] = (uint16)(0);
        value = ai_owner_state->steering_angles[2];
        value &= 0x3FFF;
        ai_owner_state->steering_angles[2] = (uint16)(value);
        if (value >= 0x2000)
            ai_owner_state->steering_angles[2] = (uint16)(value - 0x4000);
        ai_owner_state->maneuver_phase = (uint8)(1);
        ai_owner_state->speed_mode = (uint8)(2);
        return;
    }
    if (phase == 1)
    {
        value = (sint16)ai_owner_state->steering_angles[2];
        if (value == 0)
        {
            value = ai_owner_state->requested_angles[0];
            ai_owner_state->maneuver_phase = (uint8)(2);
            ai_owner_state->steering_angles[0] = (uint16)(value);
        }
        ai_owner_state->speed_mode = (uint8)(2);
        return;
    }
    if (phase == 2)
    {
        result = (sint16)ai_owner_state->steering_angles[0];
        ai_owner_state->speed_mode = (uint8)(2);
        if (result == 0)
        {
            result = 3;
            ai_owner_state->maneuver_phase = (uint8)(result);
            ai_owner_state->speed_mode = (uint8)(0);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 3)
    {
        ai_owner_state->speed_mode = (uint8)(0);
        ai_owner_state->requested_maneuver = (uint8)(255);
    }
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    return;
}

void mission_ai_update_maneuver_52(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_data;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_data;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_other;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 phase, value, numerator, divisor, quotient, increment, position, result;
    FUNCTION_MARKER(0x8004B88C, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    phase = ai_owner_state->maneuver_phase;
    selected_controls_data = mission_primary_controls(index & 0x03FFFFFFu);
    controls_data = &selected_controls_data;
    if (phase == 0)
    {
        actor_motion_object->velocity[2] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->local_velocity[2] = (uint32)(0);
        actor_motion_object->local_velocity[1] = (uint32)(0);
        actor_motion_object->local_velocity[0] = (uint32)(0);
        numerator = ai_owner_state->target_position[2];
        divisor = controls_data->min_speed;
        numerator <<= 1;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)numerator / (sint32)divisor;
        if (quotient == 0)
            xport_mips_break(7);
        if (quotient == 0xFFFFFFFF && divisor == 0x80000000)
            xport_mips_break(6);
        increment = (sint32)divisor / (sint32)quotient;
        ai_owner_state->maneuver_phase = (uint8)(1);
        ai_owner_state->acceleration[2] = (uint32)(increment);
    }
    else if (phase == 1)
    {
        value = actor_motion_object->local_velocity[2];
        increment = ai_owner_state->acceleration[2];
        value += increment;
        actor_motion_object->local_velocity[2] = (uint32)(value);
        divisor = controls_data->min_speed;
        if ((sint32)value >= (sint32)divisor)
        {
            ai_owner_state->maneuver_phase = (uint8)(2);
            ai_owner_state->steering_angles[0] = (uint16)(1024);
        }
    }
    else if (phase == 2)
    {
        motion_other = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
        position = pose_owner_object->pose.position[1];
        value = motion_other->max_altitude;
        if ((sint32)position < (sint32)value)
            ai_owner_state->maneuver_phase = (uint8)(3);
    }
    else if (phase == 3)
        ai_owner_state->requested_maneuver = (uint8)(255);
    else
        ai_owner_state->maneuver_phase = (uint8)(0);
    result = actor_traits_object->events | 0x200000;
    actor_traits_object->events = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_maneuver_04_alternate(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 target[3], direction[3], flags, phase, value, result, first, second, angle;
    uint32 *flag_address;
    /* Defines the output when the original direction helper returns without writing */
    GAME_ANGLES angles = {0};
    FUNCTION_MARKER(0x8004966C, "ACE3.BIN");
    actor_index_table = 0;
    flag_address = mission_actor_flags(index);
    flags = (*flag_address);
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    *flag_address = (flags | 0x20);
    ai_owner_state = mission_primary_ai((0) + (index));
    phase = ai_owner_state->maneuver_phase;
    switch (phase)
    {
        case 0:
            first = ai_owner_state->target_position[0];
            second = ai_owner_state->target_position[2];
            angle = game_vector_atan2((uint32)((sint32)first >> 12), (uint32)((sint32)second >> 12));
            value = pose_owner_object->pose.position[1] - angle;
            if ((sint32)value < 0)
                value = 0 - value;
            result = 2;
            if ((sint32)value < 1025)
                ai_owner_state->maneuver_phase = (uint8)(result);
            else
            {
                result = 1;
                ai_owner_state->maneuver_phase = (uint8)(result);
            }
            break;
        case 1:
            target[0] = ai_owner_state->target_position[0];
            target[1] = pose_owner_object->pose.position[1];
            target[2] = ai_owner_state->target_position[2];
            game_pose_relative_direction(target, direction, pose_owner_object->pose.position, pose_owner_object->pose.angles);
            angle = game_vector_axis_angle(direction);
            mission_ai_steer_direction(index, (uint32)(sint32)(sint16)angle, &angles, direction);
            value = (sint16)((uint16)angles.pitch);
            if ((sint32)value < 0)
                value = 0 - value;
            result = (sint32)value < 16;
            if (result != 0)
            {
                value = (sint16)((uint16)angles.yaw);
                if ((sint32)value < 0)
                    value = 0 - value;
                result = (sint32)value < 16;
                if (result != 0)
                {
                    result = 2;
                    ai_owner_state->maneuver_phase = (uint8)(result);
                }
            }
            break;
        case 2:
            result = mission_ai_request_level_angles(index);
            if (result == 0)
            {
                result = 3;
                ai_owner_state->maneuver_phase = (uint8)(result);
            }
            break;
        case 3:
            mission_ai_steer_level_to_position(index, ai_owner_state->target_position);
            first = pose_owner_object->pose.position[1];
            second = ai_owner_state->target_position[1];
            result = first - second;
            if ((sint32)result < 0)
                result = 0 - result;
            if ((sint32)result <= 0x7FFFF)
            {
                result = 4;
                ai_owner_state->maneuver_phase = (uint8)(result);
            }
            break;
        case 4:
            result = 255;
            ai_owner_state->requested_maneuver = (uint8)(result);
            break;
        default:
            result = 0x80020000;
            ai_owner_state->maneuver_phase = (uint8)(0);
            break;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_ai_update_maneuvers(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 index, mode, requested, value, flags, kind, special;
    FUNCTION_MARKER(0x8004701C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((~actor_traits_object->flags & 0xC000) != 0 || ai_owner_state->behavior_kind == 6)
            continue;
        mode = ai_owner_state->maneuver_state;
        requested = ai_owner_state->requested_maneuver;
        if (mode != requested)
        {
            flags = ai_owner_state->flags;
            requested = ai_owner_state->requested_maneuver;
            ai_owner_state->maneuver_phase = (uint8)(0);
            ai_owner_state->maneuver_ticks = (uint16)(0);
            ai_owner_state->flags = (uint32)(flags & 0xFFBFFFFF);
            ai_owner_state->maneuver_state = (uint8)(requested);
            actor_motion_object->angular_delta[0] = (uint16)(0);
            actor_motion_object->angular_delta[1] = (uint16)(0);
            actor_motion_object->angular_delta[2] = (uint16)(0);
            mode = ai_owner_state->maneuver_state;
        }
        switch (mode)
        {
            case 0:
                mission_ai_update_maneuver_00(index);
                break;
            case 4:
                kind = mission_primary_controls(index & 0x03FFFFFFu).kind;
                if (kind == 1)
                {
                    mission_ai_maneuver_06(index);
                    break;
                }
                special = ai_owner_state->behavior_state != 3 && mission_primary_binding(index & 0x03FFFFFFu)->motion.kind == 0;
                if (special == 0)
                {
                    kind = (sint16)game_runtime.selection.mission;
                    special = kind == 32 || kind == 47 || kind == 53;
                }
                if (special != 0)
                    mission_ai_maneuver_04_alternate(index);
                else
                    mission_ai_maneuver_follow_direction(index);
                break;
            case 5:
                kind = mission_primary_controls(index & 0x03FFFFFFu).kind;
                if (kind == 1)
                    mission_ai_maneuver_06(index);
                else if (kind == 3)
                    mission_ai_maneuver_05_kind3(index);
                else
                    mission_ai_maneuver_05_standard(index);
                break;
            case 6:
                mission_ai_maneuver_06(index);
                break;
            case 7:
                mission_ai_update_maneuver_07(index);
                break;
            case 8:
                mission_ai_update_maneuver_08(index);
                break;
            case 9:
                mission_ai_update_maneuver_09(index);
                break;
            case 10:
                mission_ai_update_maneuver_10(index);
                break;
            case 11:
                mission_ai_update_maneuver_11(index);
                break;
            case 12:
                mission_ai_update_maneuver_12(index);
                break;
            case 13:
                mission_ai_update_maneuver_13(index);
                break;
            case 14:
                mission_ai_update_maneuver_14(index);
                break;
            case 15:
            case 16:
            case 17:
                mission_ai_update_pitch_maneuver(index);
                break;
            case 25:
                mission_ai_update_maneuver_25(index);
                break;
            case 26:
                mission_ai_update_maneuver_26(index);
                break;
            case 27:
                mission_ai_update_maneuver_27(index);
                break;
            case 30:
                mission_ai_update_maneuver_30(index);
                break;
            case 31:
                mission_ai_update_maneuver_31(index);
                break;
            case 40:
                mission_ai_update_maneuver_40(index);
                break;
            case 41:
                mission_ai_update_maneuver_41(index);
                break;
            case 42:
                mission_ai_update_maneuver_42(index);
                break;
            case 43:
                mission_ai_update_maneuver_43(index);
                break;
            case 50:
                mission_actor_stop_motion(index);
                break;
            case 51:
                mission_ai_update_maneuver_51(index);
                break;
            case 52:
                mission_ai_update_maneuver_52(index);
                break;
            default:
                break;
        }
        value = ai_owner_state->maneuver_ticks;
        requested = ai_owner_state->requested_maneuver;
        ai_owner_state->maneuver_ticks = (uint16)(value + 1);
        if (requested == 255)
        {
            flags = ai_owner_state->flags;
            requested = ai_owner_state->requested_maneuver;
            ai_owner_state->flags = (uint32)(flags | 0x400000);
            ai_owner_state->maneuver_state = (uint8)(requested);
            actor_motion_object->angular_delta[0] = (uint16)(0);
            actor_motion_object->angular_delta[1] = (uint16)(0);
            actor_motion_object->angular_delta[2] = (uint16)(0);
        }
    }
    for (index = 2; index < 32; ++index)
    {
        actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        if ((~actor_traits_object->flags & 0xC000) == 0 && mission_primary_ai((0) + (index))->behavior_kind != 6)
            mission_ai_apply_steering_delta(index);
    }
    for (index = 2; index < 32; ++index)
    {
        actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        if ((~actor_traits_object->flags & 0xC000) == 0 && mission_primary_ai((0) + (index))->behavior_kind != 6)
            mission_ai_update_speed(index);
    }
    return;
}

void mission_ai_maneuver_follow_direction(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    GAME_ANGLES angles;
    uint32 value, speed;
    FUNCTION_MARKER(0x8004960C, "ACE3.BIN");
    value = (*mission_actor_flags(index));
    *mission_actor_flags(index) = (uint32)(value | 0x20);
    ai_owner_object = mission_primary_ai((0) + (index));
    speed = (sint16)ai_owner_object->origin_angle;
    {
        (void)(mission_ai_steer_direction(index, speed, &angles, ai_owner_object->origin_direction));
        return;
    }
}

void mission_ai_apply_steering_delta(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_ACTOR *pose_owner_descriptor;
    MISSION_ACTOR_MOTION *actor_motion_descriptor;

    MISSION_ACTOR_TRAITS *actor_traits_descriptor;
    uint32 actor_index_table;

    uint32 value, delta, flag;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80044A6C, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_object = mission_primary_ai((0) + (index));
    target = mission_actor_target(index);
    pose_owner_descriptor = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    actor_motion_descriptor = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    actor_traits_descriptor = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    if (target && ((sint32)target->distance) < 0x4000)
    {
        if ((sint16)ai_owner_object->steering_angles[0] >= 0x800)
        {
            flag = actor_traits_descriptor->render_flags;
            actor_traits_descriptor->render_flags = (uint8)(flag | 8);
        }
        value = (sint16)ai_owner_object->steering_angles[2];
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 0x1000)
        {
            flag = actor_traits_descriptor->render_flags;
            actor_traits_descriptor->render_flags = (uint8)(flag | 8);
        }
        if ((sint16)ai_owner_object->steering_angles[0] == 0 && (sint16)ai_owner_object->steering_angles[2] == 0)
        {
            flag = actor_traits_descriptor->render_flags;
            actor_traits_descriptor->render_flags = (uint8)(flag & 0xF7);
        }
    }
    else
    {
        flag = actor_traits_descriptor->render_flags;
        actor_traits_descriptor->render_flags = (uint8)(flag & 0xF7);
    }
    delta = mission_ai_update_pitch_rate(index, (sint16)ai_owner_object->steering_angles[0]);
    if (delta)
    {
        value = actor_traits_descriptor->events;
        actor_motion_descriptor->angular_delta[0] = (uint16)(delta);
        actor_traits_descriptor->events = (uint32)(value | 0x800000);
        value = ai_owner_object->steering_angles[0];
        ai_owner_object->steering_angles[0] = (uint16)(value - delta);
    }
    delta = mission_ai_update_roll_rate(index, (sint16)ai_owner_object->steering_angles[2]);
    if (delta)
    {
        value = pose_owner_descriptor->pose.angles[2];
        pose_owner_descriptor->pose.angles[2] = (uint16)(value + delta);
        value = ai_owner_object->steering_angles[2];
        ai_owner_object->steering_angles[2] = (uint16)(value - delta);
    }
    delta = mission_ai_limit_yaw_rate(index, (sint16)ai_owner_object->steering_angles[1]);
    if (delta)
    {
        value = actor_traits_descriptor->events;
        actor_motion_descriptor->angular_delta[1] = (uint16)(delta);
        actor_traits_descriptor->events = (uint32)(value | 0x800000);
        value = ai_owner_object->steering_angles[1];
        ai_owner_object->steering_angles[1] = (uint16)(value - delta);
    }
    {
        (void)(game_angles_normalize(pose_owner_descriptor->pose.angles));
        return;
    }
}

void mission_ai_update_speed(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_profile;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_profile;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    MISSION_ACTOR_MOTION *actor_motion_object;
    MISSION_ACTOR_MOTION *actor_motion_other;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 limit, step, altitude;
    uint32 first, middle, third, minimum, maximum, desired, mode, slot, value, delta, current, flags, next;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80044C28, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    target = mission_actor_target(index);
    selected_controls_profile = mission_primary_controls(index & 0x03FFFFFFu);
    controls_profile = &selected_controls_profile;
    if (target == 0)
        return;
    motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    limit = controls_profile->accel_limit;
    step = motion_descriptor->response + 1;
    altitude = pose_owner_object->pose.position[1];
    flight_sample_ai_altitude_speeds(altitude, &first, &middle, &third);
    minimum = controls_profile->min_speed;
    mode = ai_owner_state->speed_mode;
    desired = controls_profile->cruise_speed;
    maximum = controls_profile->max_speed;
    switch (mode)
    {
        case 0:
            desired = controls_profile->cruise_speed;
            break;
        case 1:
            desired = controls_profile->max_speed;
            break;
        case 2:
            desired = controls_profile->min_speed;
            break;
        case 3:
            desired = middle;
            break;
        case 4:
            slot = ai_owner_state->behavior_actor;
            actor_index_table = 0;
            actor_motion_other = mission_actor_motion((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
            if (slot == 0 || ai_owner_state->target_actor == 0)
            {
                if ((sint32)ai_owner_state->origin_direction[2] < 0)
                    desired = middle;
                else
                {
                    value = ai_owner_state->origin_distance;
                    desired = middle + (value << 3);
                }
            }
            else if ((actor_traits_object->flags & 0x40) == 0 || (sint32)ai_owner_state->origin_direction[2] < 0)
                desired = controls_profile->cruise_speed;
            else
            {
                value = ai_owner_state->origin_distance;
                desired = actor_motion_other->local_velocity[2] + (value << 3);
            }
            break;
        case 5:
        case 7:
            if (mode == 5)
                limit = 256;
            slot = ai_owner_state->behavior_actor;
            actor_index_table = 0;
            actor_motion_other = mission_actor_motion((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
            if (mode == 5)
                step = 8;
            desired = slot ? actor_motion_other->local_velocity[2] : middle;
            if ((sint32)ai_owner_state->origin_direction[2] < 0)
            {
                value = ai_owner_state->origin_distance;
                delta = 0 - value;
            }
            else
            {
                delta = ai_owner_state->origin_distance;
                value = ai_owner_state->origin_distance;
            }
            if ((sint32)value >= 0x4001)
                delta <<= 3;
            desired += delta;
            break;
        case 6:
            slot = ai_owner_state->behavior_actor;
            actor_index_table = 0;
            value = ai_owner_state->origin_direction[2];
            actor_motion_other = mission_actor_motion((actor_index_table + ((uint32)((sint32)(slot * 4) >> 2))));
            if ((sint32)value < 0)
                desired = controls_profile->cruise_speed;
            else
            {
                value = ai_owner_state->origin_distance;
                desired = actor_motion_other->local_velocity[2] + value;
            }
            break;
        case 8:
            limit = 256;
            step = 8;
            desired = middle - (uint32)((sint32)middle >> 3);
            break;
        case 9:
            return;
        case 10:
            desired = minimum;
            minimum = 0;
            limit = 256;
            step = 8;
            break;
        default:
            break;
    }
    if ((sint32)maximum < (sint32)desired)
        desired = maximum;
    else if ((sint32)desired < (sint32)minimum)
        desired = minimum;
    value = actor_motion_object->local_velocity[2];
    current = ai_owner_state->acceleration[2];
    delta = desired - value;
    if ((sint32)current < (sint32)delta)
    {
        ai_owner_state->acceleration[2] = (uint32)(current + step);
        current = ai_owner_state->acceleration[2];
    }
    else if ((sint32)delta < (sint32)current)
    {
        ai_owner_state->acceleration[2] = (uint32)(current - step);
        current = ai_owner_state->acceleration[2];
    }
    if ((sint32)limit < (sint32)current)
        ai_owner_state->acceleration[2] = (uint32)(limit);
    else if ((sint32)current < (sint32)(0 - limit))
        ai_owner_state->acceleration[2] = (uint32)(0 - limit);
    flags = actor_traits_object->events;
    value = actor_motion_object->local_velocity[2];
    actor_traits_object->events = (uint32)(flags | 0x200000);
    delta = ai_owner_state->acceleration[2];
    next = value + delta;
    actor_motion_object->local_velocity[2] = (uint32)(next);
    if ((sint32)maximum < (sint32)next)
        actor_motion_object->local_velocity[2] = (uint32)(maximum);
    else if ((sint32)next < (sint32)minimum)
        actor_motion_object->local_velocity[2] = (uint32)(minimum);
    return;
}

void mission_target_links_initialize(void)
{
    MISSION_TARGET_LINKS *record;
    uint32 index;
    FUNCTION_MARKER(0x80045FF0, "ACE3.BIN");
    for (index = 0; index < 64; ++index)
    {
        record = &mission_actor_state.target_links[index];
        record->active = (uint8)(0);
        record->attacker[1] = (uint8)(255);
        record->attacker[0] = (uint8)(255);
        record->target = (uint8)(255);
    }
    mission_actor_state.target_links[0].active = (uint8)(255);
    return;
}

void mission_target_links_update(void)
{
    MISSION_TARGET_LINKS *target;
    uint32 index, value, other;
    FUNCTION_MARKER(0x80046030, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        target = &mission_actor_state.target_links[index];
        value = target->active;
        if (value != 0)
        {
            if ((~(*mission_actor_flags(index)) & 1) != 0)
                mission_target_links_remove_actor(index);
        }
        else if (((*mission_actor_flags(index)) & 1) != 0)
            target->active = (uint8)(255);
    }
    value = mission_actor_state.target_links[0].target;
    if (value == 255)
        return;
    other = mission_actor_state.target_links[0].attacker[0];
    if (value == other)
    {
        mission_target_links_release(other);
        value = mission_actor_state.target_links[0].target;
    }
    other = mission_actor_state.target_links[0].attacker[1];
    if (value == other)
    {
        (void)(uint32)(mission_target_links_release(other));
        return;
    }
    {
        (void)(uint32)(other);
        return;
    }
}

void mission_actor_update_threat_flags(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags, value, link, mark;
    FUNCTION_MARKER(0x80038100, "ACE3.BIN");
    actor_index_table = 0;
    for (index = 2; index < 32; ++index)
    {
        ai_owner_state = mission_primary_ai(index);
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        value = actor_traits_object->flags & 0xFFFF7FFF;
        actor_traits_object->flags = (uint32)(value);
        if ((~value & 0x4000) != 0)
            continue;
        flags = (*mission_actor_flags(index)) & 0xFFFBFFFF;
        *mission_actor_flags(index) = (uint32)(flags);
        if ((sint16)actor_traits_object->health >= 10000 && (flags & 0x800000) != 0 && (actor_traits_object->flags & 0x80) != 0)
            *mission_actor_flags(index) = (uint32)(flags | 0x40000);
        flags = (*mission_actor_flags(index));
        if ((flags & 0x400000) != 0)
            continue;
        if ((flags & 1) == 0)
        {
            if ((flags & 2) != 0)
            {
                value = actor_traits_object->flags;
                actor_traits_object->flags = (uint32)(value | 0x8000);
            }
            continue;
        }
        mark = (flags & 16) != 0;
        if (mark == 0)
            mark = ((mission_actor_target(index)->flags) & 0x80) != 0;
        if (mark == 0)
            mark = (sint16)mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))))->health <= 0;
        if (mark == 0)
            mark = ai_owner_state->target_actor == 0;
        if (mark != 0)
        {
            value = actor_traits_object->flags;
            actor_traits_object->flags = (uint32)(value | 0x8000);
        }
        link = mission_actor_state.target_links[index].attacker[0];
        if (link != 255 && (mission_actor_traits((actor_index_table + ((uint32)((sint32)(link * 4) >> 2))))->flags & 0x40) != 0)
        {
            value = actor_traits_object->flags;
            actor_traits_object->flags = (uint32)(value | 0x8000);
        }
        link = mission_actor_state.target_links[index].attacker[1];
        if (link != 255 && (mission_actor_traits((actor_index_table + ((uint32)((sint32)(link * 4) >> 2))))->flags & 0x40) != 0)
        {
            value = actor_traits_object->flags;
            actor_traits_object->flags = (uint32)(value | 0x8000);
        }
    }
    return;
}

void mission_target_links_remove_actor(uint32 index)
{
    MISSION_TARGET_LINKS *target;
    uint32 linked;
    FUNCTION_MARKER(0x8004651C, "ACE3.BIN");
    target = &mission_actor_state.target_links[index];
    linked = target->attacker[0];
    target->active = (uint8)(0);
    target->attacker[0] = (uint8)(255);
    if (linked != 255)
        mission_actor_state.target_links[linked].target = (uint8)(255);
    linked = target->attacker[1];
    target->attacker[1] = (uint8)(255);
    if (linked != 255)
        mission_actor_state.target_links[linked].target = (uint8)(255);
    mission_target_links_release(index);
    return;
}

uint32 mission_target_links_release(uint32 index)
{
    MISSION_TARGET_LINKS *target;
    uint32 linked;
    FUNCTION_MARKER(0x800464B0, "ACE3.BIN");
    if (index == 255)
        return 255;
    target = &mission_actor_state.target_links[index];
    linked = target->target;
    target->target = (uint8)(255);
    if (linked == 255)
        return 255;
    target = &mission_actor_state.target_links[linked];
    if (target->attacker[0] == index)
        target->attacker[0] = (uint8)(255);
    else if (target->attacker[1] == index)
        target->attacker[1] = (uint8)(255);
    return 255;
}

void mission_ai_update_target_selection(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 index, flags, mode, current, result, value, link, a, b, c;
    FUNCTION_MARKER(0x800469EC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        flags = (*mission_actor_flags(index));
        if ((flags & 2) != 0)
        {
            *mission_actor_flags(index) = (uint32)(flags & 0xFFEFFFFF);
            mode = ai_owner_state->behavior_kind;
            ai_owner_state->requested_behavior = (uint8)(12);
            if (mode == 6)
                ai_owner_state->behavior_kind = (uint8)(0);
            continue;
        }
        mode = ai_owner_state->behavior_kind;
        if (mode == 5)
        {
            *mission_actor_flags(index) = (uint32)(flags & 0xFFEFFFFF);
            mission_ai_update_maneuver_script(index);
            continue;
        }
        if (mode == 6)
        {
            *mission_actor_flags(index) = (uint32)(flags & 0xFFEFFFFF);
            mission_ai_update_path_script(index);
            continue;
        }
        if (mode == 7)
        {
            mission_ai_request_scenario_behavior(index);
            continue;
        }
        if (mode == 8)
        {
            mission_ai_apply_disabled_behavior(index);
            continue;
        }
        if ((flags & 0x100000) != 0)
            continue;
        mission_ai_update_behavior_radius_flags(index);
        if (ai_owner_state->behavior_state == 11)
            continue;
        if (mission_actor_classify_world_boundary(index) != 0)
        {
            ai_owner_state->requested_behavior = (uint8)(11);
            result = mission_target_links_release(index);
            value = ai_owner_state->behavior_target;
            ai_owner_state->target_actor = (uint8)(result);
            ai_owner_state->behavior_actor = (uint8)(value);
            a = ai_owner_state->behavior_origin[0];
            b = ai_owner_state->behavior_origin[1];
            c = ai_owner_state->behavior_origin[2];
            ai_owner_state->aim_position[0] = (uint32)(a);
            ai_owner_state->aim_position[1] = (uint32)(b);
            ai_owner_state->aim_position[2] = (uint32)(c);
            continue;
        }
        if (ai_owner_state->behavior_state == 10)
            continue;
        result = mission_ai_classify_min_altitude(index);
        if (result == 0)
            result = mission_ai_classify_max_altitude(index);
        if (result != 0)
        {
            ai_owner_state->requested_behavior = (uint8)(10);
            result = mission_target_links_release(index);
            value = ai_owner_state->behavior_target;
            ai_owner_state->target_actor = (uint8)(result);
            ai_owner_state->behavior_actor = (uint8)(value);
            continue;
        }
        current = ai_owner_state->behavior_state;
        if (current == 7)
            continue;
        if ((effect_actor_tracking.target_mask[0] & (1u << (index & 31))) != 0)
        {
            ai_owner_state->requested_behavior = (uint8)(7);
            result = mission_target_links_release(index);
            ai_owner_state->target_actor = (uint8)(result);
            continue;
        }
        if (current == 5)
            continue;
        if ((actor_traits_object->flags & 0x1C0) != 64 && ((mission_actor_target(index)->flags) & 0x2000000) != 0 && game_runtime.selection.difficulty != 3)
        {
            ai_owner_state->requested_behavior = (uint8)(5);
            result = mission_target_links_release(index);
            ai_owner_state->target_actor = (uint8)(result);
            continue;
        }
        mode = ai_owner_state->behavior_kind;
        switch (mode)
        {
            case 2:
            case 4:
                mission_ai_follow_behavior_target(index);
                break;
            case 9:
            case 10:
                mission_ai_update_timed_target(index);
                break;
            default:
                mission_ai_acquire_target(index);
                break;
        }
        link = ai_owner_state->target_actor;
        if (link == 255)
            continue;
        mode = ai_owner_state->behavior_kind;
        if (mode - 9 < 2)
            continue;
        motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
        a = (sint16)ai_owner_state->target_refresh_ticks;
        b = (sint16)motion_descriptor->target_refresh_ticks;
        if ((sint32)b < (sint32)a)
        {
            result = mission_target_links_release(index);
            ai_owner_state->target_actor = (uint8)(result);
            continue;
        }
        if (link != 0)
        {
            value = ai_owner_state->target_actor;
            if ((~(*mission_actor_flags(value)) & 1) != 0)
            {
                ai_owner_state->target_actor = (uint8)(255);
                continue;
            }
        }
        value = mission_actor_state.target_links[index].target;
        if (value == 255)
            ai_owner_state->target_actor = (uint8)(value);
        else
        {
            value = (*mission_actor_flags(index));
            *mission_actor_flags(index) = (uint32)(value | 0x10000);
        }
    }
    return;
}

uint32 mission_target_links_assign(uint32 index, uint32 target)
{
    MISSION_TARGET_LINKS *record;
    uint32 linked;
    FUNCTION_MARKER(0x800463B8, "ACE3.BIN");
    if (target == 255)
        return target;
    if (target == 0 && mission_actor_state.target_links[0].target == index)
        return 255;
    record = &mission_actor_state.target_links[target];
    linked = record->attacker[0];
    if (linked == 255)
        record->attacker[0] = (uint8)(index);
    else
    {
        linked = record->attacker[1];
        if (linked != 255)
        {
            if (index != 0)
                return 255;
            mission_target_links_release(record->attacker[1]);
            record->attacker[1] = (uint8)(0);
        }
        else
        {
            if (game_runtime.selection.difficulty == 0 && target == 0)
                return 255;
            mission_actor_state.target_links[target].attacker[1] = (uint8)(index);
        }
    }
    mission_actor_state.target_links[index].target = (uint8)(target);
    return target;
}

uint32 mission_ai_find_target(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_target;
    uint32 actor_index_table;

    uint32 flags, team, selected = 255, best = 0x100000, available, mode;
    uint32 candidate, target_flags, other_team, value, a, b, distance;
    FUNCTION_MARKER(0x80046124, "ACE3.BIN");
    actor_index_table = 0;
    value = game_runtime.mission_control.ticks;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    if (index != (value & 0x3F))
        return 255;
    flags = actor_traits_object->flags;
    team = flags & 0x1C0;
    if ((flags & 0x80) != 0)
    {
        mode = game_runtime.selection.difficulty;
        value = mission_actor_state.target_links[0].attacker[0];
        available = value == 255;
        if (mode != 0 && available == 0)
            available = mission_actor_state.target_links[0].attacker[1] == 255;
        if (available != 0 && mission_actor_state.target_links[0].target != index)
            selected = 0;
    }
    if (selected == 255)
    {
        for (candidate = 2; candidate < 64; ++candidate)
        {
            if (candidate == index)
                continue;
            pose_owner_target = mission_actor(((uint32)((sint32)(candidate * 4) >> 2)));
            actor_traits_target = mission_actor_traits(((uint32)((sint32)(candidate * 4) >> 2)));
            target_flags = actor_traits_target->flags;
            if ((~target_flags & 0x4000) != 0)
                continue;
            flags = (*mission_actor_flags(candidate));
            if ((flags & 1) == 0 || (flags & 0x800000) != 0)
                continue;
            other_team = target_flags & 0x1C0;
            if (other_team == team || other_team == 256)
                continue;
            if (mission_actor_state.target_links[candidate].attacker[0] != 255 && mission_actor_state.target_links[candidate].attacker[1] != 255)
                continue;
            if (mission_actor_state.target_links[index].attacker[0] == candidate || mission_actor_state.target_links[index].attacker[1] == candidate)
                continue;
            if (team == 64 && (flags & 0x100) != 0 && (sint16)game_runtime.selection.mission != 30)
                continue;
            a = pose_owner_object->pose.position[0];
            b = pose_owner_target->pose.position[0];
            distance = ((sint32)(a - b) >> 12);
            if ((sint32)distance < 0)
                distance = 0 - distance;
            if ((sint32)best < (sint32)distance)
                continue;
            a = pose_owner_object->pose.position[2];
            b = pose_owner_target->pose.position[2];
            distance = ((sint32)(a - b) >> 12);
            if ((sint32)distance < 0)
                distance = 0 - distance;
            if ((sint32)best < (sint32)distance)
                continue;
            distance = game_measure_distance_gte(pose_owner_object->pose.position, pose_owner_target->pose.position);
            if ((sint32)distance < (sint32)best)
            {
                selected = candidate;
                best = distance;
            }
        }
    }
    return mission_target_links_assign(index, selected);
}

void mission_actor_finish_deactivation(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags;
    uint32 *slot;
    FUNCTION_MARKER(0x8004596C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        flags = (*mission_actor_flags(index));
        if ((flags & 2) != 0)
            mission_actor_begin_deactivation(index);
    }
    for (index = 2; index < 64; ++index)
    {
        slot = mission_actor_flags(index);
        actor_index_table = 0;
        flags = (*slot);
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        if ((flags & 4) != 0)
        {
            mission_actor_state.deactivation_ticks[index] = (uint8)(0);
            flags = (*slot);
            *slot = (flags & 0xFFFFFFFB);
            flags = actor_traits_object->flags;
            actor_traits_object->flags = (uint32)(flags & 0xFFFFBFFF);
        }
    }
    return;
}

void mission_actor_update_deactivation(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint8 *timer;
    uint32 index, flags, value;
    FUNCTION_MARKER(0x800458D0, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        actor_index_table = 0;
        flags = (*mission_actor_flags(index));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~flags & 4) != 0)
            continue;
        timer = &mission_actor_state.deactivation_ticks[index];
        value = *timer;
        if (value != 0)
        {
            --value;
            *timer = (value);
            if ((value & 0xFF) != 0)
                continue;
        }
        value = (*mission_actor_flags(index));
        *mission_actor_flags(index) = (uint32)(value & 0xFFFFFFFB);
        value = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(value & 0xFFFFBFFF);
    }
    return;
}

void mission_actor_begin_deactivation(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 value;
    uint32 *flags;
    FUNCTION_MARKER(0x80038320, "ACE3.BIN");
    actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value & 0xFFFF79FF);
    flags = mission_actor_flags(index);
    value = (*flags);
    *flags = (value & 0xFF000000);
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value & 0xFFFEFFFF);
    value = (*flags);
    *flags = (value | 4);
    mission_actor_state.deactivation_ticks[index] = (uint8)(150);
    return;
}

void mission_secondary_begin_deactivation(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 value, slot;
    FUNCTION_MARKER(0x80051F88, "ACE3.BIN");
    actor_index_table = 32;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value & 0xFFFE7BFF);
    slot = index + 32;
    value = (*mission_actor_flags(slot));
    *mission_actor_flags(slot) = (uint32)((value & 0xFF000000) | 4);
    mission_actor_state.deactivation_ticks[slot] = (uint8)(150);
    return;
}

static AI_SCRIPT_VIEW mission_ai_bind_script(const AI_SCRIPT_VIEW *section, uint32 address)
{
    AI_SCRIPT_VIEW view = {NULL, 0, address};
    uint32 offset = address - section->guest_base;

    if (section->data && offset < section->bytes)
    {
        view.data = section->data + offset;
        view.bytes = section->bytes - offset;
    }
    return view;
}

static const uint8 *mission_ai_script_span(const AI_SCRIPT_VIEW *view, uint32 offset, uint32 bytes)
{
    if (view && view->data && offset <= view->bytes && bytes <= view->bytes - offset)
        return view->data + offset;
    return NULL;
}

static const uint8 *mission_ai_script_record(const AI_SCRIPT_VIEW *view, uint32 offset, uint32 bytes)
{
    const uint8 *record = mission_ai_script_span(view, offset, bytes);
    if (!record)
        abort();
    return record;
}

void mission_ai_update_maneuver_script(uint32 index)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_TRAITS *actor_traits_first_object, *actor_traits_object;
    uint32 actor_index_table;

    const uint8 *fields;
    const AI_SCRIPT_VIEW *view = NULL;
    uint32 type, phase, count, kind, health, flags, command, condition, value, limit, advance;
    FUNCTION_MARKER(0x80050DE8, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    type = mission_ai_selected_behavior_profile(mission_primary_binding(index)).script_index;
    phase = ai_owner_state->script_phase;
    if (type < AI_MANEUVER_CATALOG_COUNT)
        view = &mission_actor_state.maneuver_scripts[type];
    else
        abort();
    if (phase == 0)
    {
        ai_owner_state->behavior_ticks = (uint32)(0);
        count = ai_owner_state->behavior_ticks;
        ai_owner_state->script_phase = (uint8)(1);
        mission_ai_decode_maneuver_command(mission_ai_script_record(view, count << 5, 32), ai_owner_state);
        return;
    }
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    kind = (sint16)actor_traits_object->kind;
    actor_traits_first_object = mission_actor_traits(actor_index_table);
    if (kind == 44)
    {
        health = (sint16)actor_traits_first_object->health;
        if ((sint32)health < 0)
        {
            ai_owner_state->requested_maneuver = (uint8)(50);
            ai_owner_state->speed_mode = (uint8)(9);
            return;
        }
    }
    count = ai_owner_state->behavior_ticks;
    flags = ai_owner_state->flags;
    command = count << 5;
    advance = flags & 0x400000;
    if (advance == 0)
    {
        fields = mission_ai_script_record(view, command, 8);
        condition = fields[3];
        if (condition == 1)
        {
            value = ai_owner_state->origin_distance;
            limit = (memcpy(&wire_word_0, fields + 4, sizeof(wire_word_0)), wire_word_0);
            if ((sint32)value < (sint32)limit)
                ai_owner_state->flags = (uint32)(flags | 0x400000);
        }
        else if (condition == 2)
        {
            value = (sint16)ai_owner_state->maneuver_ticks;
            limit = (memcpy(&wire_word_1, fields + 4, sizeof(wire_word_1)), wire_word_1);
            if ((sint32)value >= (sint32)limit)
                ai_owner_state->flags = (uint32)(flags | 0x400000);
        }
        flags = ai_owner_state->flags;
        advance = flags & 0x400000;
    }
    if (advance != 0)
    {
        count = ai_owner_state->behavior_ticks + 1;
        ai_owner_state->behavior_ticks = (uint32)(count);
        mission_ai_decode_maneuver_command(mission_ai_script_record(view, count << 5, 32), ai_owner_state);
        kind = ai_owner_state->requested_maneuver;
        if (kind == 255)
        {
            ai_owner_state->behavior_kind = (uint8)(kind);
            return;
        }
        flags = ai_owner_state->flags;
        ai_owner_state->flags = (uint32)(flags & 0xFFBFFFFF);
    }
    ai_owner_state->requested_behavior = (uint8)(13);
    return;
}

void mission_primary_restart_scenario_ai(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    const MISSION_PRIMARY_BINDING *binding;
    uint32 selection, index, member, kind, flags;
    const MISSION_SCENARIO *record;
    FUNCTION_MARKER(0x8004E060, "ACE3.BIN");
    selection = mission_flow.scenario_idx;
    record = &mission_scenarios[selection];
    for (index = 0; index < 2; ++index)
    {
        member = record->members[index];
        if (member == 0)
            continue;
        binding = mission_primary_binding(member);
        ai_owner_state = mission_primary_ai((0) + (member));
        kind = mission_ai_selected_behavior_profile(binding).kind;
        flags = ai_owner_state->flags;
        ai_owner_state->script_phase = (uint8)(0);
        ai_owner_state->behavior_ticks = (uint32)(0);
        ai_owner_state->requested_behavior = (uint8)(255);
        ai_owner_state->behavior_state = (uint8)(255);
        ai_owner_state->flags = (uint32)(flags | 0x200000);
        ai_owner_state->behavior_kind = (uint8)(kind);
    }
    return;
}

enum
{
    AI_PATH_POINT_BYTES = 18,
    AI_PATH_POINT_STRIDE = 20
};

static NATIVE_POSE mission_ai_read_path_point(const uint8 *point)
{
    NATIVE_POSE pose;
    memcpy(&pose, point, AI_PATH_POINT_BYTES);
    return pose;
}

void mission_ai_update_path_script(uint32 index)
{
    uint32 wire_word_0;
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const uint8 *current_point, *next_point;
    const AI_SCRIPT_VIEW *view = NULL;
    NATIVE_POSE current_pose, next_pose;
    uint32 mission, flags, count, type, current, next, fraction, end, start, product, position, pitch, yaw, roll, next_pitch, next_yaw, next_roll, start_pitch, start_yaw, start_roll, value;
    sint32 segment;
    FUNCTION_MARKER(0x80051010, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    mission = (sint16)game_runtime.selection.mission;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (mission == 3)
    {
        flags = actor_traits_object->render_flags;
        actor_traits_object->render_flags = (uint8)(flags | 8);
    }
    count = ai_owner_state->behavior_ticks;
    type = mission_ai_selected_behavior_profile(mission_primary_binding(index)).script_index;
    if (type < AI_PATH_CATALOG_COUNT)
        view = &mission_actor_state.path_scripts[type];
    else if (type >= AI_PATH_MANEUVER_ALIAS && type - AI_PATH_MANEUVER_ALIAS < AI_MANEUVER_CATALOG_COUNT)
        view = &mission_actor_state.maneuver_scripts[type - AI_PATH_MANEUVER_ALIAS];
    else
        abort();
    segment = (sint32)count / 8;
    fraction = count - ((uint32)segment << 3);
    current = (uint32)segment * AI_PATH_POINT_STRIDE;
    next = current + AI_PATH_POINT_STRIDE;
    end = (memcpy(&wire_word_0, mission_ai_script_record(view, next, 4), sizeof(wire_word_0)), wire_word_0);
    if (end == 0xFFFFFFFF)
    {
        ai_owner_state->behavior_kind = (uint8)(255);
        return;
    }
    next_point = mission_ai_script_record(view, next, AI_PATH_POINT_BYTES);
    current_point = mission_ai_script_record(view, current, AI_PATH_POINT_BYTES);
    next_pose = mission_ai_read_path_point(next_point);
    current_pose = mission_ai_read_path_point(current_point);
    end = next_pose.position[0];
    start = current_pose.position[0];
    product = (end - start) * fraction;
    position = pose_owner_object->pose.position[0];
    actor_motion_object->velocity[0] = (uint32)((uint32)((sint32)product / 8) + start - position);
    end = next_pose.position[1];
    start = current_pose.position[1];
    product = (end - start) * fraction;
    position = pose_owner_object->pose.position[1];
    actor_motion_object->velocity[1] = (uint32)((uint32)((sint32)product / 8) + start - position);
    end = next_pose.position[2];
    start = current_pose.position[2];
    product = (end - start) * fraction;
    position = pose_owner_object->pose.position[2];
    actor_motion_object->velocity[2] = (uint32)((uint32)((sint32)product / 8) + start - position);
    next_pitch = (sint16)next_pose.angles[0];
    start_pitch = (sint16)current_pose.angles[0];
    next_yaw = (sint16)next_pose.angles[1];
    next_roll = (sint16)next_pose.angles[2];
    pitch = next_pitch - start_pitch;
    start_yaw = (sint16)current_pose.angles[1];
    start_roll = (sint16)current_pose.angles[2];
    yaw = next_yaw - start_yaw;
    roll = next_roll - start_roll;
    if (((yaw & 0x3FFF) - 0x1C01) < 0x7FF)
    {
        if ((sint32)yaw > 0)
            yaw -= 0x2000;
        else
            yaw += 0x2000;
        pitch = 0x2000 - start_pitch - next_pitch;
        roll = next_roll + start_roll - 0x2000;
    }
    if ((sint32)pitch >= 0x2000)
        pitch -= 0x4000;
    if ((sint32)pitch < -8192)
        pitch += 0x4000;
    if ((sint32)yaw >= 0x2000)
        yaw -= 0x4000;
    if ((sint32)yaw < -8192)
        yaw += 0x4000;
    if ((sint32)roll >= 0x2000)
        roll -= 0x4000;
    if ((sint32)roll < -8192)
        roll += 0x4000;
    product = pitch * fraction;
    start = current_pose.angles[0];
    value = start + (uint32)((sint32)product / 8);
    product = yaw * fraction;
    pose_owner_object->pose.angles[0] = (uint16)(value);
    start = current_pose.angles[1];
    value = start + (uint32)((sint32)product / 8);
    product = roll * fraction;
    pose_owner_object->pose.angles[1] = (uint16)(value);
    start = current_pose.angles[2];
    pose_owner_object->pose.angles[2] = (uint16)(start + (uint32)((sint32)product / 8));
    ai_owner_state->requested_behavior = (uint8)(13);
    ai_owner_state->requested_maneuver = (uint8)(8);
    count = ai_owner_state->behavior_ticks;
    ai_owner_state->speed_mode = (uint8)(9);
    ai_owner_state->behavior_ticks = (uint32)(count + 1);
    return;
}

enum
{
    PRIMARY_CONTROL_RECORD_STRIDE = 40,
    PRIMARY_CONTROL_DATA_BYTES = 32
};

static MISSION_PRIMARY_CONTROL_PROFILE mission_primary_decode_controls(const uint8 *record)
{
    MISSION_PRIMARY_CONTROL_PROFILE profile;
    memcpy(&profile, record, sizeof(profile));
    return profile;
}

MISSION_PRIMARY_CONTROL_PROFILE mission_primary_controls(uint32 actor)
{
    const MISSION_PRIMARY_BINDING *binding = mission_primary_binding(actor);

    if (!binding->has_controls)
        abort();
    return binding->controls;
}

static MISSION_PRIMARY_CONTROL_PROFILE mission_primary_default_controls(uint32 selector)
{
    if (selector < mission_actor_state.control_catalog_count)
        return mission_actor_state.control_catalog[selector];
    abort();
}

void mission_primary_select_controls(uint32 actor, uint32 mode)
{
    MISSION_PRIMARY_BINDING *binding = mission_primary_binding(actor);

    if (mode == 1 || mode == 2)
    {
        binding->controls = mission_primary_control_presets[mode - 1];
        binding->has_controls = 1;
    }
    else
    {
        uint32 selector = binding->parameters.attributes_index;
        binding->has_controls = selector < mission_actor_state.control_catalog_count;
        if (binding->has_controls)
            binding->controls = mission_primary_default_controls(selector);
    }
}

const uint8 *mission_actor_catalog_record(const MISSION_ACTOR_CATALOG_VIEW *view, uint32 offset, uint32 bytes)
{
    if (!view->data || offset > view->bytes || bytes > view->bytes - offset)
        abort();
    return view->data + offset;
}

void mission_primary_bind_resources(const MISSION_ACTOR_CATALOG_INPUT *input)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint16 wire_halfword_10;
    uint16 wire_halfword_11;
    uint16 wire_halfword_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    uint16 wire_halfword_15;
    uint16 wire_halfword_16;
    uint32 wire_word_17;
    MISSION_PRIMARY_BINDING *binding;
    MISSION_ACTOR_DEFINITION *definition;
    const uint8 *definition_source;
    const MISSION_PRIMARY_PARAMETERS *parameters;
    MISSION_PRIMARY_MOTION_PROFILE *motion;
    MISSION_PRIMARY_BINDING *fire_binding;
    const uint8 *fire_source, *effect_input;
    const uint8 *motion_source;
    uint32 actor_index_table;

    uint32 result, count, i = 0, param_offset = 0;
    uint32 value, index;
    FUNCTION_MARKER(0x8004D4D0, "ACE3.BIN");
    result = arena_alloc(game_runtime.actor_resources.primary_count << 6);
    (void)(result);
    result = arena_alloc(5888);
    count = game_runtime.actor_resources.primary_count;
    const RESOURCE_ASSET_SOURCE *seed_owner = &game_runtime.resource_context.aircraft_backing;
    uint32 seed_offset = result - seed_owner->encoded_base;
    if (!seed_owner->data || result < seed_owner->encoded_base || seed_offset > seed_owner->bytes || 5888 > seed_owner->bytes - seed_offset)
        abort();
    mission_primary_ai_import_initial(seed_owner->data + seed_offset, count, &input->primary_parameters);
    if (count != 0)
    {
        do
        {
            actor_index_table = 0;
            (void)mission_actor((actor_index_table + ((uint32)((sint32)(i * 4) >> 2))));
            mission_primary_bind_parameters(i, mission_actor_catalog_record(&input->primary_parameters, param_offset, 44));
            binding = mission_primary_binding(i);
            parameters = &binding->parameters;
            index = parameters->weapon_index;
            definition_source = mission_actor_catalog_record(&input->definitions, (index << 4), 16);
            definition = &binding->definition;
            definition->health = (memcpy(&wire_halfword_0, definition_source, sizeof(wire_halfword_0)), wire_halfword_0);
            definition->label = definition_source[3];
            definition->shape = definition_source[4];
            definition->highlight = definition_source[5];
            definition->target_flags = definition_source[6];
            definition->radar_kind = definition_source[7];
            definition->lock_kind = definition_source[8];
            definition->render_mode = definition_source[9];
            definition->damage_mode = definition_source[10];
            definition->radius[0] = (memcpy(&wire_halfword_1, definition_source + 12, sizeof(wire_halfword_1)), wire_halfword_1);
            definition->radius[1] = (memcpy(&wire_halfword_2, definition_source + 14, sizeof(wire_halfword_2)), wire_halfword_2);
            index = parameters->attributes_index;
            binding->has_controls = index < mission_actor_state.control_catalog_count;
            if (binding->has_controls)
                binding->controls = mission_primary_default_controls(index);
            index = parameters->motion_index;
            motion_source = mission_actor_catalog_record(&input->primary_motion, index * 24, 24);
            motion = &binding->motion;
            motion->kind = motion_source[0];
            motion->response = motion_source[1];
            motion->min_altitude = (memcpy(&wire_word_3, motion_source + 4, sizeof(wire_word_3)), wire_word_3);
            motion->max_altitude = (memcpy(&wire_word_4, motion_source + 8, sizeof(wire_word_4)), wire_word_4);
            motion->lock_ticks = (memcpy(&wire_halfword_5, motion_source + 12, sizeof(wire_halfword_5)), wire_halfword_5);
            motion->behavior_delay_ticks = (memcpy(&wire_halfword_6, motion_source + 14, sizeof(wire_halfword_6)), wire_halfword_6);
            motion->turn_delay_ticks = (memcpy(&wire_halfword_7, motion_source + 16, sizeof(wire_halfword_7)), wire_halfword_7);
            motion->turn_phase_ticks = (memcpy(&wire_halfword_8, motion_source + 18, sizeof(wire_halfword_8)), wire_halfword_8);
            motion->target_refresh_ticks = (memcpy(&wire_halfword_9, motion_source + 20, sizeof(wire_halfword_9)), wire_halfword_9);
            motion->firing_ticks = (memcpy(&wire_halfword_10, motion_source + 22, sizeof(wire_halfword_10)), wire_halfword_10);
            index = parameters->animation_index;
            fire_binding = binding;
            if (index == 255)
                fire_binding->has_fire_profile = 0;
            else
            {
                fire_source = mission_actor_catalog_record(&input->primary_fire, index * 12, 12);
                fire_binding->has_fire_profile = 1;
                fire_binding->fire_profile.kind = (memcpy(&wire_halfword_11, fire_source, sizeof(wire_halfword_11)), wire_halfword_11);
                fire_binding->fire_profile.damage = (memcpy(&wire_halfword_12, fire_source + 2, sizeof(wire_halfword_12)), wire_halfword_12);
                fire_binding->fire_profile.aim_limit = (memcpy(&wire_word_13, fire_source + 4, sizeof(wire_word_13)), wire_word_13);
                fire_binding->fire_profile.range = (memcpy(&wire_word_14, fire_source + 8, sizeof(wire_word_14)), wire_word_14);
            }
            index = parameters->effect_index;
            effect_input = index == 255 ? NULL : mission_actor_catalog_record(&input->primary_effect, index << 3, 8);
            binding->has_effect_profile = effect_input != NULL;
            if (effect_input)
            {
                binding->effect_profile.mode = (memcpy(&wire_halfword_15, effect_input, sizeof(wire_halfword_15)), wire_halfword_15);
                binding->effect_profile.damage = (memcpy(&wire_halfword_16, effect_input + 2, sizeof(wire_halfword_16)), wire_halfword_16);
                binding->effect_profile.range = (memcpy(&wire_word_17, effect_input + 4, sizeof(wire_word_17)), wire_word_17);
            }
            ++i;
            binding->behavior_index = (uint32)(0);
            value = parameters->behavior_words[0];
            param_offset += 44;
            binding->behavior_words[0] = (uint32)(value);
            value = parameters->behavior_words[1];
            index = (uint8)binding->behavior_words[0];
            binding->stage_script = mission_script_reference(0);
            binding->visible_script = mission_script_reference(0);
            binding->destroy_script = mission_script_reference(0);
            binding->behavior_words[1] = (uint32)(value);
            count = game_runtime.actor_resources.primary_count;
            mission_ai_select_behavior_profile(binding, index);
        } while (i < count);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 mission_ai_advance_behavior_profile(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_PRIMARY_BINDING *binding;
    MISSION_AI_BEHAVIOR_PROFILE profile;
    uint32 value, next, a, b, c;
    FUNCTION_MARKER(0x80046928, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    binding = mission_primary_binding(index);
    value = binding->behavior_index;
    next = value + 1;
    if ((sint32)value >= 7)
        return 1;
    value = (uint8)(binding->behavior_words[next >> 2] >> ((next & 3) * 8));
    if (value == 255)
        return 1;
    binding->behavior_index = next;
    mission_ai_select_behavior_profile(binding, value);
    profile = mission_ai_selected_behavior_profile(binding);
    value = profile.kind;
    ai_owner_state->script_phase = (uint8)(0);
    ai_owner_state->behavior_kind = (uint8)(value);
    value = profile.target;
    ai_owner_state->behavior_target = (uint8)(value);
    a = profile.origin[0];
    b = profile.origin[1];
    c = profile.origin[2];
    ai_owner_state->behavior_origin[0] = (uint32)(a);
    ai_owner_state->behavior_origin[1] = (uint32)(b);
    ai_owner_state->behavior_origin[2] = (uint32)(c);
    value = profile.activate_radius;
    ai_owner_state->activate_radius = (uint32)(value);
    value = profile.deactivate_radius;
    ai_owner_state->deactivate_radius = (uint32)(value);
    return 0;
}

uint32 mission_ai_update_effect_fire(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_data;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const MISSION_PRIMARY_BINDING *binding;
    const MISSION_PRIMARY_EFFECT_PROFILE *settings;
    uint32 limit, count, type, allowed, value, mode, mask, expected;
    FUNCTION_MARKER(0x80047B00, "ACE3.BIN");
    allowed = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    binding = mission_primary_binding(index & 0x03FFFFFFu);
    settings = &binding->effect_profile;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    if (!binding->has_effect_profile)
        return 1;
    count = ai_owner_state->target_distance;
    limit = settings->range;
    if (count >= limit)
    {
        ai_owner_state->fire_flags = (uint16)(0);
        return 1;
    }
    motion_data = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    type = motion_data->kind;
    if (type == 0)
    {
        value = ai_owner_state->target_angle;
        allowed = value - 0x801 < 0x17FF;
    }
    else if (type == 1)
    {
        value = ai_owner_state->target_angle;
        allowed = value - 0x101 < 0xEFF;
    }
    else if (type == 2 || type == 3)
    {
        value = (sint16)ai_owner_state->target_angle;
        allowed = (sint32)value < 0x1000;
    }
    mode = settings->mode;
    switch (mode)
    {
        case 1:
        case 4:
            mask = 0x41;
            expected = 0x40;
            break;
        case 2:
            mask = 0x43;
            expected = 0x40;
            break;
        case 3:
            mask = 0x27;
            expected = 0x20;
            break;
        case 6:
            mask = 0x13;
            expected = 0x10;
            break;
        case 5:
            value = ai_owner_state->fire_flags;
            if ((value & 0x50) == 0x50)
            {
                value = ai_owner_state->fire_flags;
                if ((value & 1) != 0)
                    ++allowed;
                effect_segment_trace_emit((value & 15) << 6, 0, &pose_owner_object->pose);
                value = ai_owner_state->fire_flags;
                if ((value & 15) == 0)
                    sound_play_spatial(13, 0, pose_owner_object->pose.position);
            }
            break;
        default:
            break;
    }
    if (mode >= 1 && mode <= 6 && mode != 5)
    {
        value = ai_owner_state->fire_flags;
        if ((value & mask) == expected)
            ++allowed;
    }
    if (allowed == 2)
    {
        mode = settings->mode;
        value = actor_traits_object->flags;
        effect_spawn_enqueue(value, actor_motion_object->velocity, mode - 1, &pose_owner_object->pose);
    }
    value = ai_owner_state->fire_flags;
    ai_owner_state->fire_flags = (uint16)(value + 1);
    return 0;
}

void mission_actor_update_damage(void)
{
    MISSION_ACTOR *pose_owner_object;
    const MISSION_ACTOR_DEFINITION *definition_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    const MISSION_PRIMARY_BINDING *fire_binding;
    const MISSION_PRIMARY_FIRE_PROFILE *fire_profile;

    const MISSION_SECONDARY_BINDING *weapon_binding;
    const MISSION_SECONDARY_WEAPON_PROFILE *weapon_profile;
    uint32 index, owner, flags, health, kind, damage, value, mode, skip;
    FUNCTION_MARKER(0x80045A40, "ACE3.BIN");
    for (index = 2; index < 64; ++index)
    {
        pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        if ((~actor_traits_object->flags & 0x200) != 0)
            continue;
        if (((*mission_actor_flags(index)) & 1) == 0)
            continue;
        health = actor_traits_object->health;
        if ((sint16)health >= 10000)
            continue;
        value = actor_traits_object->damage_source;
        flags = actor_traits_object->events;
        owner = value & 0x3F;
        if ((flags & 0x3F) != 0)
        {
            if ((game_runtime.mission_control.mode & 4) != 0)
            {
                mode = (sint16)game_runtime.selection.mission;
                if (mode != 13 && (mode != 22 || (sint16)actor_traits_object->kind != 70))
                {
                    if (owner == 0)
                    {
                        damage = 0;
                        if ((flags & 1) != 0)
                            damage = 10;
                        else if ((flags & 2) != 0)
                            damage = 16;
                        else if ((flags & 4) != 0)
                            damage = 50;
                        else if ((flags & 8) != 0)
                            damage = 18;
                        else if ((flags & 16) != 0)
                            damage = 32;
                        else if ((flags & 32) != 0)
                            damage = 130;
                        if (game_runtime.selection.difficulty == 0)
                            damage <<= 1;
                        actor_traits_object->health = (uint16)(actor_traits_object->health - damage);
                    }
                    else
                    {
                        if (owner < 32)
                        {
                            fire_binding = mission_primary_binding(owner & 0x03FFFFFFu);
                            if (fire_binding->has_effect_profile)
                                actor_traits_object->health = (uint16)(health - fire_binding->effect_profile.damage);
                        }
                        else
                        {
                            weapon_binding = mission_secondary_binding(owner - 32);
                            if (weapon_binding->has_weapon_profile)
                                actor_traits_object->health = (uint16)(health - weapon_binding->weapon_profile.damage);
                        }
                    }
                }
            }
            if ((sint16)actor_traits_object->health <= 0)
            {
                mode = (sint16)game_runtime.selection.mission;
                kind = mode == 42 ? 88 : (index < 32 ? 27 : 29);
                sound_play_spatial(kind, 0, pose_owner_object->pose.position);
            }
            flags = actor_traits_object->events;
        }
        if ((flags & 0x7FC0) != 0)
        {
            if ((game_runtime.mission_control.mode & 4) != 0)
            {
                mode = (sint16)game_runtime.selection.mission;
                skip = mode == 22 && (sint16)actor_traits_object->kind == 70 && (~flags & 0x4000) != 0;
                if (skip == 0)
                {
                    if (owner == 0)
                    {
                        value = actor_traits_object->events;
                        damage = 0;
                        if ((value & 0x40) != 0)
                            damage = 70;
                        else if ((value & 0x80) != 0)
                            damage = 90;
                        else if ((value & 0x100) != 0)
                            damage = 40;
                        else if ((value & 0x200) != 0)
                            damage = 210;
                        else if ((value & 0xC00) != 0)
                            damage = (sint16)actor_traits_object->health;
                        else if ((value & 0x3000) != 0)
                            damage = 70;
                        else if ((value & 0x4000) != 0)
                            damage = (sint16)actor_traits_object->health;
                        if (game_runtime.selection.difficulty == 0)
                            damage <<= 1;
                        actor_traits_object->health = (uint16)(actor_traits_object->health - damage);
                    }
                    else
                    {
                        if (owner < 32)
                        {
                            fire_binding = mission_primary_binding(owner & 0x03FFFFFFu);
                            fire_profile = fire_binding->has_fire_profile ? &fire_binding->fire_profile : NULL;
                            weapon_profile = NULL;
                        }
                        else
                        {
                            fire_profile = NULL;
                            weapon_binding = mission_secondary_binding(owner - 32);
                            weapon_profile = weapon_binding->has_weapon_profile ? &weapon_binding->weapon_profile : NULL;
                        }
                        if (fire_profile != NULL || weapon_profile != NULL)
                        {
                            health = actor_traits_object->health;
                            damage = fire_profile != NULL ? fire_profile->damage : weapon_profile->damage;
                            actor_traits_object->health = (uint16)(health - damage);
                        }
                    }
                }
            }
            if ((actor_traits_object->events & 0x4000) == 0)
            {
                mode = (sint16)game_runtime.selection.mission;
                kind = mode == 42 ? 88 : (index < 32 ? 27 : 29);
                sound_play_spatial(kind, 0, pose_owner_object->pose.position);
            }
        }
        skip = 0;
        if ((actor_traits_object->events & 0x8000) != 0)
        {
            if (((*mission_actor_flags(index)) & 0x800000) == 0)
                skip = 1;
            else
            {
                definition_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
                mission_prompt_request(5, 0, definition_descriptor->label, 0);
            }
        }
        if (skip == 0 && ((*mission_actor_flags(index)) & 0x800000) != 0 && (sint16)actor_traits_object->health > 0 && (actor_traits_object->events & 0x7FC0) != 0 && (actor_traits_object->flags & 0x80) != 0)
        {
            definition_descriptor = &mission_primary_binding(owner & 0x03FFFFFFu)->definition;
            mission_prompt_request(9, 0, definition_descriptor->label, 0);
        }
        if ((actor_traits_object->events & 0x7FFF) != 0)
        {
            if (owner < 32 && ((*mission_actor_flags(owner)) & 0x800000) != 0 && (sint16)actor_traits_object->health <= 0)
            {
                definition_descriptor = &mission_primary_binding(owner & 0x03FFFFFFu)->definition;
                mission_prompt_request(4, 0, definition_descriptor->label, 0);
            }
            mode = (sint16)game_runtime.selection.mission;
            if ((mode == 9 || mode == 41 || mode == 43) && (actor_traits_object->flags & 0x40) != 0)
            {
                health = (sint16)actor_traits_object->health;
                if ((sint32)health < 21)
                    mission_status_text_show(8);
                else if ((sint32)health < 60)
                    mission_status_text_show(36);
            }
        }
        value = actor_traits_object->events;
        actor_traits_object->events = (uint32)(value & 0xFFFF8000);
    }
    return;
}

static void mission_entity_events_bind(GAME_SCENE_ENTITY_DATA input)
{
    uint32 bytes = input.bytes;
    const uint8 *wire;
    uint32 index, count, terminated_limit = 0;

    mission_actor_state.entity_events_ready = 0;
    if (!input.data || bytes < ENTITY_SELECTOR_COUNT + ENTITY_EVENT_BYTES || bytes > ENTITY_SELECTOR_COUNT + ENTITY_EVENT_CAPACITY * ENTITY_EVENT_BYTES || (bytes - ENTITY_SELECTOR_COUNT) % ENTITY_EVENT_BYTES)
        abort();

    wire = input.data;
    memcpy(mission_actor_state.entity_selectors, wire, ENTITY_SELECTOR_COUNT);
    wire += ENTITY_SELECTOR_COUNT;
    count = (bytes - ENTITY_SELECTOR_COUNT) / ENTITY_EVENT_BYTES;
    for (index = 0; index < count; ++index, wire += ENTITY_EVENT_BYTES)
    {
        MISSION_ENTITY_EVENT *event = &mission_actor_state.entity_events[index];
        memcpy(event, wire, sizeof(*event));
        if (!event->marker)
            terminated_limit = index + 1;
    }
    for (index = 0; index < ENTITY_SELECTOR_COUNT; ++index)
        if (mission_actor_state.entity_selectors[index] >= terminated_limit)
            abort();
    mission_actor_state.entity_events_ready = 1;
}

static void mission_entity_events_apply(uint32 id, MISSION_ENTITY_EVENT_MODE mode)
{
    const MISSION_ENTITY_EVENT *event;

    if (!mission_actor_state.entity_events_ready || id >= ENTITY_SELECTOR_COUNT)
        abort();
    event = mission_actor_state.entity_events + mission_actor_state.entity_selectors[id];
    for (; event->marker; ++event)
    {
        sint32 value = mode == ENTITY_EVENT_INITIAL ? event->initial_value : event->trigger_value;
        scene_terrain_change_tile(event->kind, event->tile, value);
    }
}

static uint32 mission_ai_catalog_word(const AI_SCRIPT_VIEW *section, uint32 address)
{
    uint32 wire_word_0;
    return (memcpy(&wire_word_0, mission_ai_script_record(section, address - section->guest_base, 4), sizeof(wire_word_0)), wire_word_0);
}

static void mission_ai_bind_behavior_catalog(const AI_SCRIPT_VIEW *section, uint32 base, uint32 end)
{
    uint32 bytes, index;
    const uint8 *records;

    mission_actor_state.behavior_catalog_count = 0;
    if (base >= end)
        return;
    bytes = end - base;
    if (bytes % AI_BEHAVIOR_RECORD_BYTES || bytes / AI_BEHAVIOR_RECORD_BYTES > AI_BEHAVIOR_SELECTOR_COUNT)
        return;
    records = mission_ai_script_span(section, base - section->guest_base, bytes);
    if (!records)
        return;
    mission_actor_state.behavior_catalog_count = bytes / AI_BEHAVIOR_RECORD_BYTES;
    for (index = 0; index < mission_actor_state.behavior_catalog_count; ++index)
        mission_actor_state.behavior_catalog[index] = mission_ai_decode_behavior_profile(records + index * AI_BEHAVIOR_RECORD_BYTES);
}

static void mission_primary_bind_control_catalog(const AI_SCRIPT_VIEW *section, uint32 base, uint32 end)
{
    const uint8 *data;
    uint32 bytes, count, index;

    mission_actor_state.control_catalog_count = 0;
    if (end <= base || end - base < PRIMARY_CONTROL_DATA_BYTES)
        return;
    bytes = end - base;
    data = mission_ai_script_span(section, base - section->guest_base, bytes);
    if (!data)
        return;
    count = 1 + (bytes - PRIMARY_CONTROL_DATA_BYTES) / PRIMARY_CONTROL_RECORD_STRIDE;
    if (count > sizeof(mission_actor_state.control_catalog) / sizeof(mission_actor_state.control_catalog[0]))
        return;
    mission_actor_state.control_catalog_count = count;
    for (index = 0; index < count; ++index)
        mission_actor_state.control_catalog[index] = mission_primary_decode_controls(data + index * PRIMARY_CONTROL_RECORD_STRIDE);
}

static MISSION_ACTOR_CATALOG_VIEW mission_actor_catalog_view(const AI_SCRIPT_VIEW *section, uint32 address)
{
    AI_SCRIPT_VIEW source = mission_ai_bind_script(section, address);
    MISSION_ACTOR_CATALOG_VIEW view = {source.data, source.bytes};

    return view;
}

void mission_actor_bind_scene_resources(MISSION_VARIANT_INPUT native_variant)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint32 resource = native_variant.selected.legacy_address;
    uint32 source, v0, v1, flags, table, base, i, index;
    const uint8 *header;
    AI_SCRIPT_VIEW section = {0};
    uint32 link_source, link_count, group_source, group_count;
    MISSION_ACTOR_CATALOG_INPUT input;
    uint32 secondary_motion_base, controls_base;
    const uint8 *directory, *path_table;
    FUNCTION_MARKER(0x8004D1AC, "ACE3.BIN");
    section.guest_base = game_runtime.scene_assets.scripts.legacy_base;
    if (section.guest_base >= 0x80000000u && game_runtime.scene_assets.entities.legacy_base <= 0x80000000u + sizeof(DRAM) && game_runtime.scene_assets.entities.legacy_base > section.guest_base)
    {
        section.bytes = game_runtime.scene_assets.entities.legacy_base - section.guest_base;
        if (!native_variant.root.bytes || native_variant.root.legacy_address != section.guest_base || section.bytes > native_variant.root.size)
            abort();
        section.data = native_variant.root.bytes;
    }
    source = resource + mission_ai_catalog_word(&section, resource + 4);
    header = mission_ai_script_record(&section, source - section.guest_base, 72);
    v0 = header[0];
    v1 = (memcpy(&wire_word_0, header + 16, sizeof(wire_word_0)), wire_word_0);
    group_count = v0;
    link_count = header[4];
    flags = header[8];
    v0 = (memcpy(&wire_word_1, header + 20, sizeof(wire_word_1)), wire_word_1);
    game_runtime.actor_resources.primary_count = (uint8)(flags);
    flags = header[12];
    group_source = source + (v1 << 2);
    v1 = (memcpy(&wire_word_2, header + 24, sizeof(wire_word_2)), wire_word_2);
    link_source = source + (v0 << 2);
    v0 = (memcpy(&wire_word_3, header + 28, sizeof(wire_word_3)), wire_word_3);
    input.primary_parameters = mission_actor_catalog_view(&section, (uint32)(source + (v1 << 2)));
    v1 = (memcpy(&wire_word_4, header + 32, sizeof(wire_word_4)), wire_word_4);
    input.definitions = mission_actor_catalog_view(&section, (uint32)(source + (v0 << 2)));
    v0 = (memcpy(&wire_word_5, header + 36, sizeof(wire_word_5)), wire_word_5);
    mission_ai_bind_behavior_catalog(&section, source + (v1 << 2), source + (v0 << 2));
    v1 = (memcpy(&wire_word_6, header + 40, sizeof(wire_word_6)), wire_word_6);
    input.primary_motion = mission_actor_catalog_view(&section, (uint32)(source + (v0 << 2)));
    v0 = (memcpy(&wire_word_7, header + 44, sizeof(wire_word_7)), wire_word_7);
    input.primary_fire = mission_actor_catalog_view(&section, (uint32)(source + (v1 << 2)));
    v1 = (memcpy(&wire_word_8, header + 48, sizeof(wire_word_8)), wire_word_8);
    input.primary_effect = mission_actor_catalog_view(&section, source + (v0 << 2));
    v0 = (memcpy(&wire_word_9, header + 52, sizeof(wire_word_9)), wire_word_9);
    input.secondary_parameters = mission_actor_catalog_view(&section, (uint32)(source + (v1 << 2)));
    v1 = (memcpy(&wire_word_10, header + 56, sizeof(wire_word_10)), wire_word_10);
    input.secondary_weapon = mission_actor_catalog_view(&section, source + (v0 << 2));
    v0 = source + ((memcpy(&wire_word_11, header + 60, sizeof(wire_word_11)), wire_word_11) << 2);
    secondary_motion_base = (source + (v1 << 2));
    game_runtime.actor_resources.secondary_count = (uint8)(flags);
    input.secondary_animation = mission_actor_catalog_view(&section, v0);
    if (v0 > secondary_motion_base)
        mission_secondary_bind_motion_catalog(mission_ai_script_span(&section, secondary_motion_base - section.guest_base, v0 - secondary_motion_base), v0 - secondary_motion_base);
    else
        mission_secondary_bind_motion_catalog(NULL, 0);
    table = source + ((memcpy(&wire_word_12, header + 64, sizeof(wire_word_12)), wire_word_12) << 2);
    base = table + 64;
    for (i = 0; i < AI_MANEUVER_CATALOG_COUNT; ++i)
    {
        index = mission_ai_catalog_word(&section, table + i * 4);
        mission_actor_state.maneuver_scripts[i] = mission_ai_bind_script(&section, index == 0xFFFFFFFF ? 0 : base + (index << 5));
    }
    table = source + ((memcpy(&wire_word_13, header + 68, sizeof(wire_word_13)), wire_word_13) << 2);
    path_table = mission_ai_script_span(&section, table - section.guest_base, AI_PATH_CATALOG_COUNT * 4);
    for (i = 0; i < AI_PATH_CATALOG_COUNT; ++i)
    {
        index = path_table ? (memcpy(&wire_word_14, path_table + i * 4, sizeof(wire_word_14)), wire_word_14) : 0xFFFFFFFF;
        mission_actor_state.path_scripts[i] = mission_ai_bind_script(&section, index == 0xFFFFFFFF ? 0 : table + index);
    }
    /* retain arena layout until remaining initial-state inputs migrate */
    arena_alloc(group_count * 12);
    mission_groups_bind(group_count ? mission_ai_script_record(&section, group_source - section.guest_base, group_count * 12) : NULL, group_count);
    (void)arena_alloc(link_count << 2);
    mission_links_bind(link_count ? mission_ai_script_record(&section, link_source - section.guest_base, link_count * 20) : NULL, link_count);
    controls_base = resource + mission_ai_catalog_word(&section, resource + 8);
    directory = mission_ai_script_span(&section, resource - section.guest_base, 16);
    mission_primary_bind_control_catalog(&section, controls_base, directory && (memcpy(&wire_word_15, directory, sizeof(wire_word_15)), wire_word_15) >= 3 ? resource + (memcpy(&wire_word_16, directory + 12, sizeof(wire_word_16)), wire_word_16) : controls_base);
    mission_primary_bind_resources(&input);
    mission_secondary_bind_resources(&input);
    for (i = 0; i < 64; ++i)
    {
        mission_actor_unbind(i);
    }
    mission_entity_events_bind(game_runtime.scene_assets.entities);
    {
        (void)(mission_entity_events_apply_initial());
        return;
    }
}

const MISSION_PRIMARY_CONTROL_PROFILE mission_primary_control_presets[2] = {{0u, 98304u, 167936u, 57344u, 32u, 0u, 0u, 473u, 32u, 65515u, 15u}, {0u, 131072u, 380928u, 69632u, 32u, 0u, 0u, 717u, 45u, 65506u, 11u}};

void mission_entity_events_apply_initial(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80048B48, "ACE3.BIN");
    for (index = 0; index < ENTITY_SELECTOR_COUNT; ++index)
        mission_entity_events_apply(index, ENTITY_EVENT_INITIAL);
    return;
}

void mission_entity_events_trigger(uint32 id)
{
    FUNCTION_MARKER(0x80048AD4, "ACE3.BIN");
    mission_entity_events_apply(id, ENTITY_EVENT_TRIGGER);
    return;
}

void mission_ai_update_weapon_fire(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    const MISSION_ACTOR_DEFINITION *definition_value;
    const MISSION_PRIMARY_BINDING *fire_binding_trigger;
    const MISSION_PRIMARY_FIRE_PROFILE *fire_trigger;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_settings;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_value;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object, *actor_traits_target, *actor_traits_value;
    uint32 actor_index_table;

    uint32 target_index, mode, value, flags, duration, count, kind, type, parameter, owner, sound;
    FUNCTION_MARKER(0x80047D68, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    actor_index_table = 0;
    target_index = ai_owner_state->target_actor;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    actor_traits_target = mission_actor_traits((actor_index_table + ((uint32)((sint32)(target_index * 4) >> 2))));
    fire_binding_trigger = mission_primary_binding(index & 0x03FFFFFFu);
    fire_trigger = fire_binding_trigger->has_fire_profile ? &fire_binding_trigger->fire_profile : NULL;
    motion_settings = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    if (fire_trigger == NULL)
        return;
    if (fire_trigger->kind == 0)
        return;
    mode = (sint16)game_runtime.selection.mission;
    if (mode == 23)
    {
        value = game_runtime.mission_control.argument;
        if (value == 0)
        {
            value = pose_owner_object->pose.position[1];
            if ((sint32)0xFC400000 < (sint32)value)
                return;
            value = pose_owner_object->pose.position[0];
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 0x0946FFFF)
                return;
            value = pose_owner_object->pose.position[2];
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 0x0946FFFF)
                return;
        }
    }
    target_index = ai_owner_state->target_actor;
    if (target_index < 32)
    {
        target_index = ai_owner_state->target_actor;
        value = effect_actor_tracking.target_mask[0];
        if ((value & (1u << (target_index & 31))) != 0)
            goto reset_charge;
    }
    else
    {
        target_index = ai_owner_state->target_actor;
        value = effect_actor_tracking.target_mask[1];
        if ((value & (1u << ((target_index - 32) & 31))) != 0)
            goto reset_charge;
    }
    value = (sint16)ai_owner_state->target_angle;
    mode = fire_trigger->aim_limit;
    if ((sint32)value >= (sint32)mode)
        goto reset_charge;
    count = ai_owner_state->target_distance;
    value = fire_trigger->range;
    if (value < count)
        goto reset_charge;
    value = ai_owner_state->fire_ticks;
    ai_owner_state->fire_ticks = (uint16)(value + 1);
    flags = actor_traits_object->flags;
    duration = (sint16)motion_settings->firing_ticks;
    if ((flags & 0x80) != 0)
    {
        mode = game_runtime.selection.difficulty;
        if (mode == 0)
            duration <<= 1;
        if ((sint32)mode >= 2)
        {
            duration = (sint32)duration >> 1;
            if ((sint32)duration < 60)
                duration = 60;
        }
    }
    count = (sint16)ai_owner_state->fire_ticks;
    if ((sint32)count < (sint32)duration)
    {
        target_index = ai_owner_state->target_actor;
        if (target_index == 0)
            goto charge_feedback;
        value = (*mission_actor_flags(target_index));
        if ((value & 0x400) != 0)
            goto finish_fire;
    }
    kind = fire_trigger->kind;
    if (kind == 3)
    {
        target_index = ai_owner_state->target_actor;
        flags = (*mission_actor_flags(target_index));
        if ((flags & 0x400) != 0)
            kind = 1;
    }
    if (kind == 1 || kind == 4)
    {
        mode = game_runtime.selection.difficulty;
        type = 0;
        if (mode != 0 || (actor_traits_object->flags & 0x80) == 0)
        {
            motion_value = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
            type = motion_value->kind;
        }
        if (type == 0)
            parameter = 0x08000002;
        else if (type == 1)
            parameter = 0x102;
        else if (type == 2)
            parameter = 0x10102;
        else if (type == 3)
        {
            value = actor_traits_object->flags;
            if ((value & 0x40) != 0)
                parameter = 0x20202;
            else
            {
                value = game_runtime.selection.difficulty;
                parameter = value == 3 ? 0x20202 : 0x10202;
            }
        }
        else
            parameter = 0x10202;
        flags = actor_traits_object->flags;
        owner = actor_traits_target->flags;
        effect_object_create_variant_1(flags, owner, parameter, &pose_owner_object->pose, actor_motion_object->velocity);
    }
    else if (kind == 2 || kind == 3 || kind == 8 || kind == 9)
    {
        flags = actor_traits_object->flags;
        owner = actor_traits_target->flags;
        if (kind == 2)
            effect_object_create_variant_2(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
        else if (kind == 3)
            effect_object_create_variant_3(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
        else if (kind == 8)
            effect_object_create_variant_7(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
        else
            effect_object_create_variant_8(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
    }
    ai_owner_state->fire_ticks = (uint16)(0);
    flags = (*mission_actor_flags(index));
    if ((flags & 0x800000) != 0)
    {
        actor_index_table = 0;
        actor_traits_value = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        flags = actor_traits_value->flags;
        if ((flags & 0x40) != 0)
        {
            target_index = ai_owner_state->target_actor;
            if (target_index < 32)
            {
                definition_value = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
                sound = definition_value->label;
                mission_prompt_request(3, 0, sound, 0);
            }
        }
        actor_index_table = 0;
        actor_traits_value = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        flags = actor_traits_value->flags;
        if ((flags & 0x80) != 0)
        {
            definition_value = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
            sound = definition_value->label;
            mission_prompt_request(7, 0, sound, 0);
        }
    }
finish_fire:
    target_index = ai_owner_state->target_actor;
    if (target_index != 0)
        return;
charge_feedback:
    count = (sint16)ai_owner_state->fire_ticks;
    if (((sint32)duration >> 1) < (sint32)count)
    {
        value = (sint16)motion_settings->firing_ticks;
        if ((sint32)count < (sint32)value)
        {
            value = mission_target.warning_flags;
            mission_target.warning_flags = value | 1;
        }
        count = (sint16)ai_owner_state->fire_ticks;
    }
    value = (sint32)duration >> 2;
    if (count != value)
    {
        (void)(uint32)(value);
        return;
    }
    flags = (*mission_actor_flags(index));
    if ((flags & 0x800000) != 0 && (actor_traits_object->flags & 0x80) != 0)
    {
        definition_value = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
        sound = definition_value->label;
        {
            (void)(mission_prompt_request(6, 0, sound, 0));
            return;
        }
    }
    {
        (void)(mission_prompt_request(0, 0, 0, 0));
        return;
    }
reset_charge:
    ai_owner_state->fire_ticks = (uint16)(0);
    return;
}

static void mission_ai_process_fire_requests(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 index, flags;
    FUNCTION_MARKER(0x80047A54, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        flags = (*mission_actor_flags(index));
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((flags & 0x10000) == 0)
            continue;
        *mission_actor_flags(index) = (uint32)(flags & 0xFFFEFFFF);
        if (ai_owner_state->target_actor == 255)
            return;
        if (mission_ai_update_effect_fire(index) == 1)
            mission_ai_update_weapon_fire(index);
    }
    return;
}

static void mission_ai_acquire_target(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_record;
    const MISSION_PRIMARY_BINDING *fire_binding_parameters;
    uint32 selected, a, b, c, flags, result, mode, current, threshold;
    FUNCTION_MARKER(0x80050720, "ACE3.BIN");
    ai_owner_record = mission_primary_ai((0) + (index));
    selected = ai_owner_record->target_actor;
    if (selected == 255)
    {
        ai_owner_record->requested_behavior = (uint8)(0);
        ai_owner_record->behavior_actor = (uint8)(selected);
        a = ai_owner_record->behavior_origin[0];
        b = ai_owner_record->behavior_origin[1];
        c = ai_owner_record->behavior_origin[2];
        ai_owner_record->aim_position[0] = (uint32)(a);
        ai_owner_record->aim_position[1] = (uint32)(b);
        ai_owner_record->aim_position[2] = (uint32)(c);
        flags = ai_owner_record->flags;
        if ((flags & 0x2000000) != 0)
            return;
        result = mission_ai_find_target(index);
        ai_owner_record->target_actor = (uint8)(result);
        if ((result & 255) == 255)
            return;
        selected = ai_owner_record->target_actor;
        ai_owner_record->requested_behavior = (uint8)(1);
        ai_owner_record->aim_position[2] = (uint32)(0);
        ai_owner_record->aim_position[1] = (uint32)(0);
        ai_owner_record->aim_position[0] = (uint32)(0);
        ai_owner_record->behavior_actor = (uint8)(selected);
        return;
    }
    mode = ai_owner_record->behavior_state;
    if (mode == 0)
        return;
    if (mode == 1)
    {
        flags = ai_owner_record->flags;
        if ((flags & 0x1000000) == 0)
        {
            fire_binding_parameters = mission_primary_binding(index & 0x03FFFFFFu);
            current = ai_owner_record->target_distance;
            threshold = mission_primary_engagement_range(fire_binding_parameters);
            if (current < threshold)
                ai_owner_record->requested_behavior = (uint8)(4);
            return;
        }
    }
    else if (mode == 4)
    {
        flags = ai_owner_record->flags;
        if ((flags & 0x1000000) == 0)
        {
            fire_binding_parameters = mission_primary_binding(index & 0x03FFFFFFu);
            current = ai_owner_record->target_distance;
            threshold = mission_primary_engagement_range(fire_binding_parameters);
            if (threshold < current)
                ai_owner_record->requested_behavior = (uint8)(1);
            return;
        }
    }
    result = mission_target_links_release(index);
    ai_owner_record->target_actor = (uint8)(result);
    ai_owner_record->requested_behavior = (uint8)(0);
    ai_owner_record->behavior_actor = (uint8)(255);
    a = ai_owner_record->behavior_origin[0];
    b = ai_owner_record->behavior_origin[1];
    c = ai_owner_record->behavior_origin[2];
    ai_owner_record->aim_position[0] = (uint32)(a);
    ai_owner_record->aim_position[1] = (uint32)(b);
    ai_owner_record->aim_position[2] = (uint32)(c);
    return;
}

static void mission_ai_update_timed_target(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    const MISSION_PRIMARY_BINDING *fire_binding_pointer;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_pointer;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 selected, health, other, flags, count, limit, mode, progress, first, second, third, toggle, ticks, reset_count;
    uint32 *slot;
    FUNCTION_MARKER(0x80050BB4, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    selected = ai_owner_state->behavior_target;
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
    health = (sint16)actor_traits_object->health;
    if ((sint32)health <= 0)
    {
        ai_owner_state->behavior_kind = (uint8)(255);
        return;
    }
    other = ai_owner_state->behavior_target;
    ai_owner_state->behavior_actor = (uint8)(selected);
    ai_owner_state->target_actor = (uint8)(other);
    slot = mission_actor_flags(index);
    flags = (*slot);
    *slot = (flags | 0x10000);
    count = ai_owner_state->behavior_ticks;
    if (count == 0)
    {
        reset_count = 0;
        goto initialize_target;
    }
    motion_pointer = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    limit = (sint16)motion_pointer->target_refresh_ticks;
    if (count == limit)
    {
        ai_owner_state->requested_behavior = (uint8)(0);
        actor_index_table = 0;
        pose_owner_object = mission_actor(actor_index_table);
        actor_traits_object = mission_actor_traits(actor_index_table);
        first = pose_owner_object->pose.position[0];
        second = pose_owner_object->pose.position[1];
        third = pose_owner_object->pose.position[2];
        ai_owner_state->aim_position[0] = (uint32)(first);
        ai_owner_state->aim_position[1] = (uint32)(second);
        ai_owner_state->aim_position[2] = (uint32)(third);
        goto advance_count;
    }
    mode = ai_owner_state->behavior_state;
    if (mode == 1)
    {
        fire_binding_pointer = mission_primary_binding(index & 0x03FFFFFFu);
        progress = ai_owner_state->target_distance;
        limit = mission_primary_engagement_range(fire_binding_pointer);
        if (progress < limit)
            ai_owner_state->requested_behavior = (uint8)(4);
        goto advance_count;
    }
    if (mode == 4)
    {
        fire_binding_pointer = mission_primary_binding(index & 0x03FFFFFFu);
        progress = ai_owner_state->target_distance;
        limit = mission_primary_engagement_range(fire_binding_pointer);
        if (limit < progress)
            ai_owner_state->requested_behavior = (uint8)(1);
        goto advance_count;
    }
    if (mode == 0)
        return;
    reset_count = 1;
initialize_target:
    selected = ai_owner_state->behavior_target;
    toggle = ai_owner_state->script_phase;
    ai_owner_state->requested_behavior = (uint8)(1);
    ai_owner_state->aim_position[0] = (uint32)(0);
    ai_owner_state->aim_position[1] = (uint32)(0);
    ai_owner_state->behavior_actor = (uint8)(selected);
    if (toggle == 0)
    {
        ai_owner_state->script_phase = (uint8)(1);
        ticks = game_runtime.mission_control.ticks;
        ticks <<= 13;
    }
    else
    {
        ai_owner_state->script_phase = (uint8)(0);
        ticks = game_runtime.mission_control.ticks;
        ticks = ((ticks << 1) - 20000) << 12;
    }
    ai_owner_state->aim_position[2] = (uint32)(ticks);
    if (reset_count != 0)
    {
        ai_owner_state->behavior_ticks = (uint32)(0);
        return;
    }
advance_count:
    count = ai_owner_state->behavior_ticks;
    ai_owner_state->behavior_ticks = (uint32)(count + 1);
    return;
}

void mission_actor_tick_primary(void)
{
    FUNCTION_MARKER(0x80038A3C, "ACE3.BIN");
    mission_actor_update_threat_flags();
    mission_ai_tick_primary();
    {
        (void)(mission_actor_finish_primary_tick());
        return;
    }
}

static void mission_ai_tick_primary(void)
{
    uint32 value;
    FUNCTION_MARKER(0x8003809C, "ACE3.BIN");
    value = mission_target.warning_flags;
    mission_target.warning_flags = value & 0xFFFFFFFE;
    mission_actor_process_primary_deaths();
    mission_ai_advance_primary_profiles();
    mission_ai_update_target_selection();
    mission_ai_update_behaviors();
    mission_ai_update_maneuvers();
    scene_audio_update_actors();
    {
        (void)(mission_prompt_update());
        return;
    }
}

static void mission_actor_process_primary_deaths(void)
{
    MISSION_SCRIPT_REFERENCE destroy_script;
    const MISSION_ACTOR_DEFINITION *definition_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_target;

    uint32 index, flags, value, mode;
    FUNCTION_MARKER(0x80046600, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_index_target = 0;
        flags = (*mission_actor_flags(index));
        actor_traits_object = mission_actor_traits((actor_index_target + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~flags & 1) != 0 || (sint16)actor_traits_object->health > 0)
            continue;
        if ((flags & 0x100) != 0)
        {
            value = game_runtime.metrics.flagged_count[0];
            game_runtime.metrics.flagged_count[0] = (uint8)(value + 1);
        }
        if (((*mission_actor_flags(index)) & 0x200) != 0)
        {
            value = game_runtime.metrics.flagged_count[1];
            game_runtime.metrics.flagged_count[1] = (uint8)(value + 1);
        }
        flags = (*mission_actor_flags(index));
        if ((flags & 0x80000) != 0)
        {
            mission_script_start_legacy(mission_event_script(MISSION_EVENT_SIGNAL, 3));
            definition_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
            value = definition_descriptor->health;
            actor_traits_object->health = (uint16)(value);
            flags = (*mission_actor_flags(index)) & 0xFFF7FFFF;
            *mission_actor_flags(index) = (uint32)(flags);
            if ((sint16)actor_traits_object->kind == 27)
            {
                mode = (sint16)game_runtime.selection.mission;
                if (mode == 33 || mode == 58)
                {
                    *mission_actor_flags(index) = (uint32)(flags | 0x800);
                    mission_result_record_event(index);
                }
            }
            continue;
        }
        if ((flags & 0x800) != 0 && (actor_traits_object->damage_source & 0x3F) == 0)
        {
            value = actor_traits_object->flags;
            if ((~value & 0x40) != 0 && (sint16)actor_traits_object->kind != 44)
            {
                if ((game_runtime.options.kill_camera & 1) != 0)
                    camera_try_kill_view(value);
                if ((actor_traits_object->flags & 0x80) != 0)
                {
                    mode = (game_runtime.mission_control.ticks & 7) != 0 ? 7 : 6;
                    mission_status_text_show(mode);
                }
            }
        }
        mission_result_record_event(index);
        mission_actor_mark_primary_destroyed(index);
        destroy_script = mission_primary_binding(index & 0x03FFFFFFu)->destroy_script;
        if (mission_script_reference_present(destroy_script))
            mission_script_start_legacy(destroy_script);
    }
    return;
}

static void mission_ai_advance_primary_profiles(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 index, value;
    FUNCTION_MARKER(0x80046854, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        value = ai_owner_state->behavior_kind;
        if (value != 255)
            continue;
        if (mission_ai_advance_behavior_profile(index) != 0)
            continue;
        ai_owner_state->requested_behavior = (uint8)(value);
        ai_owner_state->behavior_state = (uint8)(value);
        ai_owner_state->requested_maneuver = (uint8)(value);
        ai_owner_state->maneuver_state = (uint8)(value);
    }
    if ((game_runtime.mission_control.mode & 14) != 0)
        mission_groups_update();
    mission_links_mark_actors();
}

static void mission_ai_request_fallback_aim(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 result, mode, angle, value, cosine;
    FUNCTION_MARKER(0x80050AC0, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    value = ai_owner_state->behavior_target;
    ai_owner_state->behavior_actor = (uint8)(value);
    result = ai_owner_state->requested_behavior;
    if (result == 2)
    {
        (void)(uint32)(result);
        return;
    }
    mode = ai_owner_state->behavior_kind;
    ai_owner_state->requested_behavior = (uint8)(2);
    if (mode == 2)
    {
        ai_owner_state->aim_position[0] = (uint32)(0);
        result = ((game_random() & 0x1FF) - 256) << 7;
        ai_owner_state->aim_position[1] = (uint32)(result);
        result = 0x1000000;
        ai_owner_state->aim_position[2] = (uint32)(result);
    }
    else
    {
        result = 4;
        if (mode == 4)
        {
            angle = (index & 15) << 10;
            value = (uint32)game_wave_at(angle * 2) << 7;
            ai_owner_state->aim_position[0] = (uint32)(value);
            result = game_random();
            cosine = game_wave_at((angle + 4096) * 2);
            result = ((result & 0x1FF) - 256) << 7;
            ai_owner_state->aim_position[1] = (uint32)(result);
            ai_owner_state->aim_position[2] = (uint32)(cosine << 7);
        }
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void mission_ai_follow_behavior_target(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    const MISSION_PRIMARY_BINDING *fire_binding_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 target, mode, current, link, result, value, distance;
    FUNCTION_MARKER(0x800508D0, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    target = ai_owner_state->behavior_target;
    actor_traits_object = mission_actor_traits(((uint32)((sint32)(target * 4) >> 2)));
    if ((sint16)actor_traits_object->health <= 0)
    {
        ai_owner_state->behavior_kind = (uint8)(255);
        return;
    }
    mode = ai_owner_state->behavior_kind;
    if (mode == 4 && ai_owner_state->behavior_state == 2 && (ai_owner_state->flags & 0x2000000) != 0 && ai_owner_state->behavior_actor == target)
        return;
    if ((ai_owner_state->flags & 0x1000000) != 0)
        goto release_target;
    link = ai_owner_state->target_actor;
    if (link == 255)
    {
        if (ai_owner_state->behavior_kind == 2)
        {
            result = mission_target_links_assign(index, ai_owner_state->behavior_target);
            ai_owner_state->target_actor = (uint8)(result);
        }
        value = ai_owner_state->target_actor;
        if (value == 255)
        {
            result = mission_ai_find_target(index);
            ai_owner_state->target_actor = (uint8)(result);
            if ((result & 0xFF) == value)
            {
                mission_ai_request_fallback_aim(index);
                return;
            }
        }
        target = ai_owner_state->target_actor;
        ai_owner_state->requested_behavior = (uint8)(1);
        ai_owner_state->aim_position[0] = (uint32)(0);
        ai_owner_state->aim_position[1] = (uint32)(0);
        ai_owner_state->aim_position[2] = (uint32)(0);
        ai_owner_state->behavior_actor = (uint8)(target);
        return;
    }
    current = ai_owner_state->behavior_state;
    if (current == 1)
    {
        fire_binding_descriptor = mission_primary_binding(index & 0x03FFFFFFu);
        distance = ai_owner_state->target_distance;
        value = mission_primary_engagement_range(fire_binding_descriptor);
        if (distance < value)
            ai_owner_state->requested_behavior = (uint8)(4);
        return;
    }
    if (current == 4)
    {
        fire_binding_descriptor = mission_primary_binding(index & 0x03FFFFFFu);
        distance = ai_owner_state->target_distance;
        value = mission_primary_engagement_range(fire_binding_descriptor);
        if (value < distance)
            ai_owner_state->requested_behavior = (uint8)(1);
        return;
    }
    if (current == 2)
        return;
release_target:
    result = mission_target_links_release(index);
    ai_owner_state->target_actor = (uint8)(result);
    mission_ai_request_fallback_aim(index);
    return;
}

static void mission_actor_finish_primary_tick(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags, a, b, c, d, e, f, scene, value, kind, health, emit, blocked;
    FUNCTION_MARKER(0x800383AC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        if (actor_traits_object->events & 0x800000)
            game_angles_apply_delta(actor_motion_object->angular_delta, pose_owner_object->pose.angles);
        if (actor_traits_object->events & 0x400000)
            game_rotate_vector(actor_motion_object->angular_delta, actor_motion_object->local_velocity, actor_motion_object->velocity);
        if (actor_traits_object->events & 0x200000)
            game_rotate_vector(pose_owner_object->pose.angles, actor_motion_object->local_velocity, actor_motion_object->velocity);
        flags = actor_traits_object->events;
        if ((sint32)flags < 0)
        {
            a = pose_owner_object->pose.position[0];
            b = actor_motion_object->velocity[0];
            c = actor_motion_object->velocity[1];
            d = actor_motion_object->velocity[2];
            pose_owner_object->pose.position[0] = (uint32)(a + b);
            e = pose_owner_object->pose.position[1];
            f = pose_owner_object->pose.position[2];
            pose_owner_object->pose.position[1] = (uint32)(e + c);
            pose_owner_object->pose.position[2] = (uint32)(f + d);
            flags = actor_traits_object->events;
        }
        actor_traits_object->events = (uint32)(flags & 0xFF000000);
    }
    if (game_runtime.mission_control.mode == 4)
        mission_ai_process_fire_requests();
    scene = (sint16)game_runtime.selection.mission;
    if (scene == 32 || scene == 47 || scene == 53)
    {
        actor_index_table = 2;
        for (index = 2; index < 32; ++index, actor_index_table += 1)
        {
            pose_owner_object = mission_actor(actor_index_table);
            actor_motion_object = mission_actor_motion(actor_index_table);
            actor_traits_object = mission_actor_traits(actor_index_table);
            if ((~actor_traits_object->flags & 0xC000) != 0)
                continue;
            if ((sint32)pose_owner_object->pose.position[1] < (sint32)0xFF4AC000)
                pose_owner_object->pose.position[1] = (uint32)(0xFF4AC000);
            if ((sint32)pose_owner_object->pose.position[1] > (sint32)0xFFF9C000)
                pose_owner_object->pose.position[1] = (uint32)(0xFFF9C000);
        }
    }
    if (scene == 6)
    {
        for (index = 2; index < 32; ++index)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((~actor_traits_object->flags & 0xC000) != 0)
                continue;
            value = game_runtime.world_settings.ground_offset;
            effect_emit_height_scaled(0 - value, actor_motion_object->velocity, pose_owner_object->pose.position);
        }
    }
    if (scene == 17 || scene == 31 || scene == 38 || scene == 46)
    {
        if (game_runtime.metrics.kind45_count >= 3)
        {
            for (index = 2; index < 32; ++index)
            {
                actor_index_table = 0;
                pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                if ((~actor_traits_object->flags & 0xC000) != 0 || (sint16)actor_traits_object->kind != 45)
                    continue;
                if (((*mission_actor_flags(index)) & 0x200000) == 0)
                    continue;
                value = game_runtime.mission_control.ticks;
                effect_emit_vector_periodic(value + index, 3, actor_motion_object->velocity, pose_owner_object->pose.position);
                break;
            }
        }
    }
    if ((scene - 31) < 3 || scene == 38 || scene == 46 || scene == 47 || scene == 48 || scene == 53)
    {
        for (index = 2; index < 32; ++index)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((~actor_traits_object->flags & 0xC000) != 0)
                continue;
            kind = (sint16)actor_traits_object->kind;
            if (kind == 26)
            {
                emit = 0;
                if (scene == 33 || scene == 48 || scene == 53)
                {
                    health = (sint16)actor_traits_object->health;
                    if ((sint32)health < 201 && (sint32)health > 0)
                        emit = 1;
                }
                if (!emit && scene == 38)
                {
                    if (game_runtime.mission_control.argument == 1)
                        emit = 1;
                }
                if (emit)
                {
                    value = game_runtime.mission_control.ticks;
                    effect_emit_vector_periodic(value + index, 2, actor_motion_object->velocity, pose_owner_object->pose.position);
                    value = actor_traits_object->render_flags;
                    actor_traits_object->render_flags = (uint8)(value & 0xF7);
                }
                else
                {
                    blocked = game_runtime.metrics.flags & 2;
                    if (!blocked && scene == 38)
                    {
                        if ((sint32)game_runtime.mission_control.argument > 0)
                            blocked = 1;
                    }
                    if (!blocked && scene == 48)
                    {
                        if ((sint32)game_runtime.mission_control.argument >= 2)
                            blocked = 1;
                    }
                    if (!blocked && (actor_traits_object->render_flags & 8))
                    {
                        effect_emit_trail(2, pose_owner_object->pose.position, actor_motion_object->velocity);
                        value = actor_traits_object->render_flags;
                        actor_traits_object->render_flags = (uint8)(value & 0xF7);
                    }
                }
                kind = (sint16)actor_traits_object->kind;
            }
            if (kind == 27 && (scene == 33 || scene == 46 || scene == 53))
            {
                health = (sint16)actor_traits_object->health;
                if ((sint32)health < 201 && (sint32)health > 0)
                {
                    value = game_runtime.mission_control.ticks;
                    effect_emit_vector_periodic(value + index, 2, actor_motion_object->velocity, pose_owner_object->pose.position);
                    value = actor_traits_object->render_flags;
                    actor_traits_object->render_flags = (uint8)(value & 0xF7);
                }
            }
        }
    }
    if (scene == 34 || scene == 54)
    {
        for (index = 2; index < 32; ++index)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((~actor_traits_object->flags & 0xC000) != 0 || (sint16)actor_traits_object->kind != 25 || (actor_traits_object->render_flags & 8) == 0)
                continue;
            effect_emit_trail(3, pose_owner_object->pose.position, actor_motion_object->velocity);
            value = actor_traits_object->render_flags;
            actor_traits_object->render_flags = (uint8)(value & 0xF7);
        }
    }
    if (scene == 27)
    {
        for (index = 2; index < 32; ++index)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((~actor_traits_object->flags & 0xC000) != 0)
                continue;
            flags = (*mission_actor_flags(index));
            if ((flags & 1) && (sint16)actor_traits_object->kind == 39 && (flags & 0x200000))
                effect_emit_vector_type8(actor_motion_object->velocity, pose_owner_object->pose.position);
        }
    }
    if (scene == 58)
    {
        for (index = 2; index < 32; ++index)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            if ((~actor_traits_object->flags & 0xC000) == 0)
                effect_emit_trail(4, pose_owner_object->pose.position, actor_motion_object->velocity);
        }
        return;
    }
    return;
}

static void mission_ai_request_scenario_behavior(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_descriptor;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_descriptor;
    uint32 kind, mission, selected, mode;
    FUNCTION_MARKER(0x80051304, "ACE3.BIN");
    selected_controls_descriptor = mission_primary_controls(index & 0x03FFFFFFu);
    controls_descriptor = &selected_controls_descriptor;
    ai_owner_state = mission_primary_ai((0) + (index));
    kind = controls_descriptor->kind;
    if (kind == 7)
    {
        ai_owner_state->requested_behavior = (uint8)(14);
        return;
    }
    if (kind == 8)
    {
        ai_owner_state->requested_behavior = (uint8)(15);
        return;
    }
    mission = (sint16)game_runtime.selection.mission;
    if (kind == 6)
    {
        if (mission == 28)
            mode = 16;
        else if (mission == 36)
            mode = 17;
        else
            return;
    }
    else
    {
        if (mission != 6 && mission != 55)
            return;
        mode = 18;
    }
    selected = ai_owner_state->behavior_target;
    ai_owner_state->requested_behavior = (uint8)(mode);
    ai_owner_state->behavior_actor = (uint8)(selected);
    {
        (void)(uint32)(mode);
        return;
    }
}

static void mission_ai_apply_disabled_behavior(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    uint32 mode;
    FUNCTION_MARKER(0x800513D8, "ACE3.BIN");
    ai_owner_state = mission_primary_ai((0) + (index));
    mode = ai_owner_state->requested_behavior;
    if (mode == 255)
    {
        ai_owner_state->behavior_kind = (uint8)(mode);
        return;
    }
    return;
}

static void mission_actor_mark_primary_destroyed(uint32 index)
{
    uint32 value;
    uint32 *target;
    FUNCTION_MARKER(0x800382D4, "ACE3.BIN");
    target = mission_actor_flags(index);
    value = ((*target) & 0xFFEF8FFE) | 2;
    *target = (value);
    {
        (void)(uint32)(value);
        return;
    }
}

static void mission_ai_update_behavior_radius_flags(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    const MISSION_ACTOR *pose_target_actor;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 position[3], flags, mode, radius, x, z, value;
    FUNCTION_MARKER(0x8004781C, "ACE3.BIN");
    actor_index_table = 0;
    ai_owner_state = mission_primary_ai((0) + (index));
    flags = ai_owner_state->flags;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    mode = ai_owner_state->behavior_kind;
    ai_owner_state->flags = (uint32)(flags | 0x3000000);
    if (mode == 2 || mode == 4 || mode == 9 || mode == 10)
    {
        value = ai_owner_state->behavior_target;
        actor_index_table = 0;
        pose_target_actor = mission_actor((actor_index_table + ((uint32)((sint32)(value * 4) >> 2))));
    }
    else
    {
        pose_target_actor = NULL;
    }
    position[0] = (pose_target_actor != NULL ? pose_target_actor->pose.position[0] : ai_owner_state->behavior_origin[0]);
    position[1] = (pose_target_actor != NULL ? pose_target_actor->pose.position[1] : ai_owner_state->behavior_origin[1]);
    position[2] = (pose_target_actor != NULL ? pose_target_actor->pose.position[2] : ai_owner_state->behavior_origin[2]);
    radius = ai_owner_state->activate_radius;
    x = pose_owner_object->pose.position[0];
    if ((sint32)(position[0] + radius) >= (sint32)x && (sint32)x >= (sint32)(position[0] - radius))
    {
        z = pose_owner_object->pose.position[2];
        if ((sint32)(position[2] + radius) >= (sint32)z && (sint32)z >= (sint32)(position[2] - radius))
        {
            value = ai_owner_state->flags;
            ai_owner_state->flags = (uint32)(value & 0xFEFFFFFF);
        }
    }
    mode = ai_owner_state->behavior_kind;
    radius = ai_owner_state->deactivate_radius;
    x = pose_owner_object->pose.position[0];
    if ((sint32)(position[0] + radius) < (sint32)x || (sint32)x < (sint32)(position[0] - radius))
        return;
    z = pose_owner_object->pose.position[2];
    if ((sint32)(position[2] + radius) < (sint32)z || (sint32)z < (sint32)(position[2] - radius))
        return;
    if (mode == 2 || mode == 4 || mode == 9 || mode == 10)
    {
        value = pose_owner_object->pose.position[1];
        if ((sint32)(position[1] + 0x800000) < (sint32)value || (sint32)value < (sint32)(position[1] - 0x800000))
            return;
    }
    value = ai_owner_state->flags;
    ai_owner_state->flags = (uint32)(value & 0xFDFFFFFF);
    return;
}

uint32 mission_actor_classify_world_boundary(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 x, z, minimum_x, maximum_x, minimum_z, maximum_z, vx, vz, y, value, mode;
    FUNCTION_MARKER(0x800476F0, "ACE3.BIN");
    pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
    actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
    minimum_x = game_runtime.world_settings.minimum_x;
    x = pose_owner_object->pose.position[0];
    if ((sint32)x < (sint32)minimum_x)
        return 2;
    maximum_x = game_runtime.world_settings.maximum_x;
    if ((sint32)maximum_x < (sint32)x)
        return 2;
    z = pose_owner_object->pose.position[2];
    minimum_z = game_runtime.world_settings.minimum_z;
    if ((sint32)z < (sint32)minimum_z)
        return 2;
    maximum_z = game_runtime.world_settings.maximum_z;
    if ((sint32)maximum_z < (sint32)z)
        return 2;
    vx = actor_motion_object->velocity[0];
    vz = actor_motion_object->velocity[2];
    x += vx << 8;
    z += vz << 8;
    if ((sint32)x < (sint32)minimum_x || (sint32)maximum_x < (sint32)x || (sint32)z < (sint32)minimum_z || (sint32)maximum_z < (sint32)z)
        return 1;
    value = actor_motion_object->velocity[1];
    y = pose_owner_object->pose.position[1];
    y += value << 8;
    mode = (sint16)game_runtime.selection.mission;
    if (mode == 42 && ((sint32)y > 0x3A98000 || (sint32)y < (sint32)0xF15A0000))
        return 1;
    mode = (sint16)game_runtime.selection.mission;
    if (mode == 58)
        return (sint32)y > (sint32)0xFFD00000;
    return 0;
}

static uint32 mission_ai_classify_min_altitude(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_parameters;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_parameters;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 position, threshold, speed, numerator, time, divisor, value, a, b;
    FUNCTION_MARKER(0x800475B0, "ACE3.BIN");
    pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
    actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
    motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    position = pose_owner_object->pose.position[1];
    threshold = motion_descriptor->min_altitude;
    selected_controls_parameters = mission_primary_controls(index & 0x03FFFFFFu);
    controls_parameters = &selected_controls_parameters;
    if ((sint32)position < (sint32)threshold)
        return 2;
    speed = actor_motion_object->velocity[1];
    numerator = threshold - position;
    if ((sint32)speed >= 0)
        return 0;
    if (speed == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    time = ((sint64)(sint32)numerator / (sint32)speed);
    if ((sint32)time < 0)
        return 2;
    divisor = controls_parameters->roll_rate;
    numerator = (sint16)pose_owner_object->pose.angles[2];
    divisor = ((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    a = ((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)a < 0)
        a = 0 - a;
    numerator = 0 - (uint32)(sint16)pose_owner_object->pose.angles[0];
    divisor = controls_parameters->pitch_rate_min;
    divisor = ((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    b = ((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)b < 0)
        b = 0;
    value = a + 2 + b;
    return (sint32)value >= (sint32)time;
}

static uint32 mission_ai_classify_max_altitude(uint32 index)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_parameters;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_parameters;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    MISSION_ACTOR_MOTION *actor_motion_object;

    uint32 position, threshold, speed, numerator, time, divisor, value, a, b;
    FUNCTION_MARKER(0x80047470, "ACE3.BIN");
    pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
    actor_motion_object = mission_actor_motion(((uint32)((sint32)(index * 4) >> 2)));
    motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    position = pose_owner_object->pose.position[1];
    threshold = motion_descriptor->max_altitude;
    selected_controls_parameters = mission_primary_controls(index & 0x03FFFFFFu);
    controls_parameters = &selected_controls_parameters;
    if ((sint32)threshold < (sint32)position)
        return 2;
    speed = actor_motion_object->velocity[1];
    numerator = threshold - position;
    if ((sint32)speed <= 0)
        return 0;
    if (speed == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    time = ((sint64)(sint32)numerator / (sint32)speed);
    if ((sint32)time < 0)
        return 2;
    divisor = controls_parameters->roll_rate;
    numerator = (sint16)pose_owner_object->pose.angles[2];
    divisor = ((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    a = ((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)a < 0)
        a = 0 - a;
    numerator = 0 - (uint32)(sint16)pose_owner_object->pose.angles[0];
    divisor = controls_parameters->pitch_rate_max;
    divisor = ((sint32)(divisor << 16) >> 17);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    b = ((sint64)(sint32)numerator / (sint32)divisor);
    if ((sint32)b < 0)
        b = 0;
    value = a + 2 + b;
    return (sint32)value >= (sint32)time;
}

void mission_actor_tick_damage(void)
{
    FUNCTION_MARKER(0x80045898, "ACE3.BIN");
    mission_actor_update_deactivation();
    mission_actor_update_damage();
    mission_links_update_health();
    {
        (void)(mission_target_links_update());
        return;
    }
}

void mission_primary_show_actor(uint32 index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 result;
    FUNCTION_MARKER(0x8004E0FC, "ACE3.BIN");
    actor_traits_object = mission_actor_traits(((uint32)((sint32)((index << 2)) >> 2)));
    result = actor_traits_object->flags | 0x10000;
    actor_traits_object->flags = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_primary_activate_actor(uint32 index)
{
    const MISSION_ACTOR_DEFINITION *definition_attributes;
    const MISSION_PRIMARY_PARAMETERS *parameters;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 original, value, result;
    uint32 *flags;
    sint32 amount;
    FUNCTION_MARKER(0x8004E124, "ACE3.BIN");
    actor_index_table = 0;
    flags = mission_actor_flags(index);
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    parameters = &mission_primary_binding(index & 0x03FFFFFFu)->parameters;
    original = (*flags);
    definition_attributes = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
    *flags = (original | 1);
    if ((parameters->flags & 1) != 0)
        *flags = (original | 0x101);
    if ((parameters->flags & 2) != 0)
        *flags = ((*flags) | 0x200);
    if ((parameters->flags & 4) != 0)
        *flags = ((*flags) | 0x80000);
    if (definition_attributes->radar_kind != 0)
        *flags = ((*flags) | 0x1000);
    if (definition_attributes->lock_kind != 0)
        *flags = ((*flags) | 0x2000);
    if ((definition_attributes->target_flags & 2) != 0)
        *flags = ((*flags) | 0x4000);
    if ((definition_attributes->target_flags & 1) != 0)
        *flags = ((*flags) | 0x8000);
    if (definition_attributes->damage_mode == 0)
        *flags = ((*flags) | 16);
    if (definition_attributes->label != 255)
        *flags = ((*flags) | 0x800000);
    if (definition_attributes->shape == 0)
        *flags = ((*flags) | 0x400);
    value = (*flags);
    if ((value & 0x1000) != 0 && (sint16)actor_traits_object->kind != 0)
        *flags = (value | 0x800);
    mission_actor_initialize_marker(index, definition_attributes);
    hud_radar_bind_contact(index, definition_attributes);
    value = definition_attributes->render_mode;
    if (value == 1)
        actor_traits_object->flags = (uint32)(actor_traits_object->flags | 0x800);
    else if (value == 2)
        actor_traits_object->flags = (uint32)(actor_traits_object->flags | 0x1000);
    amount = (sint16)definition_attributes->radius[0];
    value = definition_attributes->radius[0];
    if (amount != -1)
    {
        result = actor_traits_object->flags;
        actor_traits_object->radius[0] = (uint16)(value);
        actor_traits_object->flags = (uint32)(result | 0x200);
    }
    else
        actor_traits_object->radius[0] = (uint16)(0);
    amount = (sint16)definition_attributes->radius[1];
    value = definition_attributes->radius[1];
    result = amount;
    if (amount != -1)
    {
        result = actor_traits_object->flags;
        actor_traits_object->radius[1] = (uint16)(value);
        result |= 0x400;
        actor_traits_object->flags = (uint32)(result);
    }
    else
        actor_traits_object->radius[1] = (uint16)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_actor_advance_position(const MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 value, x, vx, vy, vz, z;
    FUNCTION_MARKER(0x80070A34, "ACE3.BIN");
    value = actor_traits->events;
    if ((sint32)value < 0)
    {
        x = native_object->pose.position[0];
        vx = actor_motion->velocity[0];
        vy = actor_motion->velocity[1];
        vz = actor_motion->velocity[2];
        native_object->pose.position[0] = (uint32)(x + vx);
        value = native_object->pose.position[1];
        z = native_object->pose.position[2];
        value += vy;
        z += vz;
        native_object->pose.position[1] = (uint32)(value);
        native_object->pose.position[2] = (uint32)(z);
    }
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 mission_ai_request_level_angles(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 value;
    FUNCTION_MARKER(0x80044034, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    value = pose_owner_object->pose.angles[2];
    ai_owner_state = mission_primary_ai((0) + (index));
    value = 0 - value;
    ai_owner_state->steering_angles[2] = (uint16)(value);
    if ((value << 16) != 0)
        return 1;
    value = pose_owner_object->pose.angles[0];
    value = 0 - value;
    ai_owner_state->steering_angles[0] = (uint16)(value);
    return (value << 16) != 0;
}

uint32 mission_ai_step_level_attitude(uint32 index)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    MISSION_ACTOR *pose_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_limits;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_limits;
    uint32 actor_index_table;

    uint32 value, raw, delta, low, high;
    FUNCTION_MARKER(0x8004409C, "ACE3.BIN");
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    selected_controls_limits = mission_primary_controls(index & 0x03FFFFFFu);
    controls_limits = &selected_controls_limits;
    ai_owner_state = mission_primary_ai((0) + (index));
    ai_owner_state->steering_angles[2] = (uint16)(0);
    ai_owner_state->steering_angles[1] = (uint16)(0);
    ai_owner_state->steering_angles[0] = (uint16)(0);
    value = (sint16)pose_owner_object->pose.angles[2];
    raw = pose_owner_object->pose.angles[2];
    delta = 0 - value;
    if (value != 0)
    {
        high = (sint16)controls_limits->roll_rate;
        if ((sint32)high < (sint32)delta)
            delta = high;
        low = 0 - high;
        if ((sint32)delta < (sint32)low)
            delta = low;
        pose_owner_object->pose.angles[2] = (uint16)(raw + delta);
        return 1;
    }
    value = (sint16)pose_owner_object->pose.angles[0];
    raw = pose_owner_object->pose.angles[0];
    delta = 0 - value;
    if (value == 0)
        return 0;
    high = (sint16)controls_limits->pitch_rate_max;
    if ((sint32)high < (sint32)delta)
        delta = high;
    low = (sint16)controls_limits->pitch_rate_min;
    if ((sint32)delta < (sint32)low)
        delta = low;
    pose_owner_object->pose.angles[0] = (uint16)(raw + delta);
    return 1;
}

uint32 mission_ai_limit_yaw_rate(uint32 index, uint32 value)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_descriptor;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_descriptor;
    uint32 limit, raw;
    FUNCTION_MARKER(0x800449DC, "ACE3.BIN");
    ai_owner_object = mission_primary_ai((0) + (index));
    selected_controls_descriptor = mission_primary_controls(index & 0x03FFFFFFu);
    controls_descriptor = &selected_controls_descriptor;
    if (value == 0)
        ai_owner_object->angular_rate[1] = (uint16)(0);
    else if ((sint32)value > 0)
    {
        limit = (sint16)controls_descriptor->yaw_rate;
        raw = controls_descriptor->yaw_rate;
        ai_owner_object->angular_rate[1] = (uint16)((sint32)value < (sint32)limit ? value : raw);
    }
    else
    {
        limit = (sint16)controls_descriptor->yaw_rate;
        raw = controls_descriptor->yaw_rate;
        limit = 0 - limit;
        ai_owner_object->angular_rate[1] = (uint16)((sint32)limit < (sint32)value ? value : 0 - raw);
    }
    return (sint16)ai_owner_object->angular_rate[1];
}

uint32 mission_ai_update_roll_rate(uint32 index, uint32 value)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_profile;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_profile;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    uint32 factor, speed, product, limit, raw, current, next, delta;
    FUNCTION_MARKER(0x8004470C, "ACE3.BIN");
    motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
    selected_controls_profile = mission_primary_controls(index & 0x03FFFFFFu);
    controls_profile = &selected_controls_profile;
    factor = motion_descriptor->response;
    speed = (sint16)controls_profile->roll_rate;
    ++factor;
    product = speed * factor;
    ai_owner_object = mission_primary_ai((0) + (index));
    limit = (speed << 1) + speed + (uint32)((sint32)product >> 3);
    limit = ((sint32)limit >> 2);
    raw = ai_owner_object->angular_rate[2];
    current = (sint16)raw;
    if ((sint32)value >= 0)
    {
        if ((sint32)current < -1)
            next = ((sint32)current >> 1);
        else if ((sint32)(current << 1) >= (sint32)value)
            next = ((sint32)value >> 1);
        else
        {
            delta = (sint16)limit;
            if ((sint32)value < (sint32)delta)
                delta = value;
            next = raw + 1 + (uint32)((sint32)(delta * factor) >> 8);
        }
        ai_owner_object->angular_rate[2] = (uint16)(next);
        current = (sint16)ai_owner_object->angular_rate[2];
        if ((sint32)(sint16)limit < (sint32)current)
            ai_owner_object->angular_rate[2] = (uint16)(limit);
    }
    else
    {
        if ((sint32)current > 0)
            next = ((sint32)current >> 1);
        else if ((sint32)value >= (sint32)(current << 1))
            next = ((sint32)value >> 1);
        else
        {
            delta = 0 - (uint32)(sint32)(sint16)limit;
            if ((sint32)delta < (sint32)value)
                delta = value;
            next = raw + (uint32)((sint32)(delta * factor) >> 8);
        }
        ai_owner_object->angular_rate[2] = (uint16)(next);
        current = (sint16)ai_owner_object->angular_rate[2];
        if ((sint32)current < (sint32)(0 - (uint32)(sint32)(sint16)limit))
            ai_owner_object->angular_rate[2] = (uint16)(0 - limit);
    }
    return (sint16)ai_owner_object->angular_rate[2];
}

uint32 mission_ai_update_pitch_rate(uint32 index, uint32 value)
{
    MISSION_PRIMARY_AI *ai_owner_object;
    MISSION_PRIMARY_CONTROL_PROFILE selected_controls_profile;
    const MISSION_PRIMARY_CONTROL_PROFILE *controls_profile;
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    uint32 raw, current, next, limit, tick, rate, bound;
    FUNCTION_MARKER(0x80044884, "ACE3.BIN");
    ai_owner_object = mission_primary_ai((0) + (index));
    selected_controls_profile = mission_primary_controls(index & 0x03FFFFFFu);
    controls_profile = &selected_controls_profile;
    raw = ai_owner_object->angular_rate[0];
    current = (sint16)raw;
    if ((sint32)value >= 0)
    {
        if ((sint32)current < -1)
            next = ((sint32)current >> 1);
        else if ((sint32)(current << 1) >= (sint32)value)
            next = ((sint32)value >> 1);
        else
        {
            limit = (sint16)controls_profile->pitch_rate_max;
            if ((sint32)value >= (sint32)limit)
                value = limit;
            motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
            tick = game_runtime.mission_control.ticks;
            rate = motion_descriptor->response;
            if (rate < (tick & 3))
                goto positive_clamp;
            next = raw + 1 + (uint32)((sint32)value >> 4);
        }
        ai_owner_object->angular_rate[0] = (uint16)(next);
    positive_clamp:
        current = (sint16)ai_owner_object->angular_rate[0];
        limit = (sint16)controls_profile->pitch_rate_max;
        bound = controls_profile->pitch_rate_max;
        if ((sint32)limit < (sint32)current)
            ai_owner_object->angular_rate[0] = (uint16)(bound);
    }
    else
    {
        if ((sint32)current > 0)
            next = ((sint32)current >> 1);
        else if ((sint32)value >= (sint32)(current << 1))
            next = ((sint32)value >> 1);
        else
        {
            limit = (sint16)controls_profile->pitch_rate_min;
            if ((sint32)limit >= (sint32)value)
                value = limit;
            motion_descriptor = &mission_primary_binding(index & 0x03FFFFFFu)->motion;
            tick = game_runtime.mission_control.ticks;
            rate = motion_descriptor->response;
            if (rate < (tick & 3))
                goto negative_clamp;
            next = raw + (uint32)((sint32)value >> 4);
        }
        ai_owner_object->angular_rate[0] = (uint16)(next);
    negative_clamp:
        current = (sint16)ai_owner_object->angular_rate[0];
        limit = (sint16)controls_profile->pitch_rate_min;
        bound = controls_profile->pitch_rate_min;
        if ((sint32)current < (sint32)limit)
            ai_owner_object->angular_rate[0] = (uint16)(bound);
    }
    return (sint16)ai_owner_object->angular_rate[0];
}

void mission_actor_update_target_timers(void)
{
    const MISSION_PRIMARY_MOTION_PROFILE *motion_descriptor;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_objects;

    uint32 index, profiles, flags, value, raw, count;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80045768, "ACE3.BIN");
    profiles = 0 + 128;
    actor_index_objects = 2;
    for (index = 2; index < 64; ++index, profiles += 64, actor_index_objects += 1)
    {
        actor_traits_object = mission_actor_traits(actor_index_objects);
        flags = actor_traits_object->flags;
        target = mission_actor_target(index);
        if ((~flags & 0xC000) != 0 || target == 0)
            continue;
        flags = (target->flags);
        if (flags & 0x40)
        {
            value = ((sint16)target->cone_ticks);
            raw = (target->cone_ticks);
            if ((sint32)value < 0x7FFF)
                target->cone_ticks = (uint16)(raw + 1);
        }
        else
            target->cone_ticks = (uint16)(0);
        flags = (target->flags);
        if (flags & 8)
        {
            value = ((sint16)target->aim_ticks);
            raw = (target->aim_ticks);
            if ((sint32)value < 0x7FFF)
                target->aim_ticks = (uint16)(raw + 1);
        }
        else
            target->aim_ticks = (uint16)(0);
        if (index >= 32)
            continue;
        flags = (target->flags);
        if (flags & 4)
        {
            value = (target->lock_ticks);
            motion_descriptor = &mission_primary_binding(2)->motion;
            ++value;
            target->lock_ticks = (uint16)(value);
            count = (sint16)motion_descriptor->lock_ticks;
            if ((sint32)(sint16)value >= (sint32)count)
            {
                flags = (target->flags);
                target->flags = (uint32)(flags | 0x2000000);
            }
        }
        else
            target->lock_ticks = (uint16)(0);
    }
    return;
}

void mission_ai_update_target_geometry(void)
{
    MISSION_PRIMARY_AI *ai_owner_state;
    const MISSION_ACTOR *pose_owner_other;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 target_direction[3];
    uint32 index, flags, value;
    FUNCTION_MARKER(0x800455B8, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        flags = actor_traits_object->flags;
        ai_owner_state = mission_primary_ai((0) + (index));
        if ((~flags & 0xC000) != 0)
            continue;
        flags = (*mission_actor_flags(index));
        if (flags & 0x20)
        {
            value = game_measure_distance_gte(pose_owner_object->pose.position, ai_owner_state->target_position);
            ai_owner_state->origin_distance = (uint32)(value);
            game_pose_relative_direction(ai_owner_state->target_position, ai_owner_state->origin_direction, pose_owner_object->pose.position, pose_owner_object->pose.angles);
            value = game_vector_axis_angle(ai_owner_state->origin_direction);
            ai_owner_state->origin_angle = (uint16)(value);
            flags = (*mission_actor_flags(index));
            *mission_actor_flags(index) = (uint32)(flags & 0xFFFFFFDF);
        }
        value = ai_owner_state->target_actor;
        if (value != 255)
        {
            actor_index_table = 0;
            pose_owner_other = mission_actor((actor_index_table + ((uint32)((sint32)(value * 4) >> 2))));
            value = game_measure_distance_gte(pose_owner_object->pose.position, pose_owner_other->pose.position);
            ai_owner_state->target_distance = (uint32)(value);
            game_pose_relative_direction(pose_owner_other->pose.position, target_direction, pose_owner_object->pose.position, pose_owner_object->pose.angles);
            value = game_vector_axis_angle(target_direction);
            flags = ai_owner_state->target_distance;
            ai_owner_state->target_angle = (uint16)(value);
            if ((sint32)flags < 0x2000)
            {
                value = ai_owner_state->target_refresh_ticks;
                ai_owner_state->target_refresh_ticks = (uint16)(value + 16);
            }
        }
        else
        {
            value = ai_owner_state->origin_distance;
            ai_owner_state->target_distance = (uint32)(value);
            value = ai_owner_state->origin_angle;
            ai_owner_state->target_refresh_ticks = (uint16)(0);
            ai_owner_state->target_angle = (uint16)(value);
        }
    }
    return;
}

void mission_actor_update_target_geometry(void)
{
    FUNCTION_MARKER(0x80045240, "ACE3.BIN");
    mission_target_update_geometry();
    mission_links_update_targets();
    mission_ai_update_target_geometry();
    {
        (void)(mission_actor_update_target_timers());
        return;
    }
}

void mission_actor_update_links_and_trace(void)
{
    uint32 value;
    FUNCTION_MARKER(0x8004658C, "ACE3.BIN");
    mission_links_update_poses();
    mission_actor_update_target_geometry();
    value = game_runtime.mission_control.mode & 4;
    if (value == 0)
        return;
    if ((sint16)game_runtime.selection.mission != 9)
    {
        (void)(mission_result_record_trace());
        return;
    }
    value = game_runtime.mission_control.argument;
    if ((sint32)value > 0)
    {
        (void)(uint32)(value);
        return;
    }
    {
        (void)(mission_result_record_trace());
        return;
    }
}

static void mission_actor_relative_target_angles(uint16 angles[3], const uint32 target[3], const NATIVE_POSE *origin)
{
    MATRIX matrix = {0};
    uint32 a, b, c, d, x, y, z, sum, distance, horizontal, shift, value, angle;
    uint32 input[3], high[3], low[3], mac[3], index, magnitude;
    FUNCTION_MARKER(0x800317F0, "ACE3.BIN");
    game_build_rotation_via_gte(&matrix, origin->angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    SetRotMatrix(&matrix);
    a = origin->position[0];
    b = target[0];
    c = origin->position[1];
    x = ((sint32)(a - b) >> 12);
    b = target[1];
    d = target[2];
    a = origin->position[2];
    y = ((sint32)(c - b) >> 12);
    z = ((sint32)(a - d) >> 12);
    sum = ((sint32)x < 0 ? 0 - x : x) + ((sint32)y < 0 ? 0 - y : y) + ((sint32)z < 0 ? 0 - z : z);
    distance = 0x100000;
    if ((sint32)sum <= 0xAFFF)
        distance = game_vector_length_gte(x, y, z);
    input[0] = x;
    input[1] = y;
    input[2] = z;
    for (index = 0; index < 3; ++index)
    {
        value = input[index];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        high[index] = ((sint32)magnitude >> 15);
        low[index] = magnitude & 0x7FFF;
        if ((sint32)value < 0)
        {
            high[index] = 0 - high[index];
            low[index] = 0 - low[index];
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    mac[0] = xport_gte_read_data(25);
    mac[1] = xport_gte_read_data(26);
    mac[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    x = a + (mac[0] << 3);
    y = b + (mac[1] << 3);
    z = c + (mac[2] << 3);
    sum = ((sint32)x < 0 ? 0 - x : x) + ((sint32)z < 0 ? 0 - z : z);
    horizontal = 0x100000;
    if ((sint32)sum <= 0x57FFF)
    {
        shift = (sint32)sum > 0xAFFF ? 3 : 0;
        horizontal = game_vector_length_gte((uint32)((sint32)x >> shift), 0, (uint32)((sint32)z >> shift)) << shift;
    }
    value = game_vector_atan2(y, horizontal);
    angles[0] = value;
    value = 0 - game_vector_atan2(x, 0 - z);
    angles[1] = value;
    value = game_vector_atan2(x, y);
    angle = (sint16)angles[1];
    if ((sint32)angle < 0)
        angle = 0 - angle;
    angles[2] = (0 - value);
    if ((sint32)angle >= 0x1001)
        distance = 0 - distance;
    {
        (void)(uint32)(distance);
        return;
    }
}

void mission_actor_camera_target_angles(uint32 index, uint16 angles[3], const NATIVE_POSE *origin)
{
    FUNCTION_MARKER(0x800779C8, "ACE3.BIN");
    if ((sint32)index > 0)
    {
        const MISSION_ACTOR *actor = mission_actor((uint32)((sint32)(index * 4) >> 2));
        if (actor->traits.flags & 0x4000)
        {
            mission_actor_relative_target_angles(angles, actor->pose.position, origin);
            angles[2] = 0;
            return;
        }
    }
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
}

void mission_actor_clear_script_flags(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, flags;
    FUNCTION_MARKER(0x80038E58, "ACE3.BIN");
    actor_index_table = 0;
    for (index = 0; index < 64; ++index)
    {
        actor_traits_object = mission_actor_traits(actor_index_table);
        flags = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(flags & 0xFFFEFFFF);
        actor_index_table += 1;
    }
    return;
}

void mission_primary_initialize_menu_actor(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FUNCTION_MARKER(0x80038A6C, "ACE3.BIN");
    mission_primary_initialize_actors();
    pose_owner_object = mission_actor(0);
    actor_traits_object = mission_actor_traits(0);
    actor_traits_object->model = (uint8)(0);
    actor_index_table = 0;
    actor_traits_object->flags = (uint32)(0x1C240);
    pose_owner_object->pose.position[0] = (uint32)(0);
    pose_owner_object->pose.position[1] = (uint32)(0xFFB50000);
    pose_owner_object->pose.position[2] = (uint32)(0);
    pose_owner_object->pose.angles[0] = (uint16)(0);
    pose_owner_object->pose.angles[1] = (uint16)(0);
    pose_owner_object->pose.angles[2] = (uint16)(0);
    mission_actor_traits((actor_index_table + 1))->flags = (uint32)(1);
    return;
}

void mission_actor_initialize_target_state(uint32 index)
{
    MISSION_ACTOR_TARGET *state;
    FUNCTION_MARKER(0x8004D800, "ACE3.BIN");
    state = mission_actor_target(index);
    state->flags = (uint32)(0);
    state->distance = (uint32)(0x100000);
    state->direction.word[2] = (uint32)(0);
    state->direction.word[1] = (uint32)(0);
    state->direction.word[0] = (uint32)(0);
    state->angle = (uint16)(0);
    state->lock_ticks = (uint16)(0);
    state->aim_ticks = (uint16)(0);
    state->cone_ticks = (uint16)(0);
    state->projection_flags = (uint32)(0);
    state->visible_ticks = (uint32)(0);
    return;
}
