#include <stddef.h>

#include "game_functions.h"

enum
{
    ACTOR_PHASE_PRIMARY = 0,
    ACTOR_PHASE_SECONDARY = 100
};

/* both views include the adjacent bytes read by original byte selectors */
static const uint8 actor_phase_selectors[356] = {
    0x11, 0x21, 0x22, 0x31, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x04, 0x02, 0x05, 0x00, 0x02, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x07, 0x00, 0x03, 0x04, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x04, 0x05, 0x00, 0x02, 0x00, 0x11, 0x21, 0x22, 0x31, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0x00, 0x00, 0xC0, 0x11, 0x00, 0x00, 0x40, 0x12, 0x00, 0x00, 0x40, 0xFD, 0x00, 0x00, 0xC0, 0xFD, 0x00, 0x00, 0x40, 0xFE, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x40, 0x0F, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x80, 0xF3, 0x00, 0x00, 0xC0, 0xF3, 0x00, 0x00, 0x80, 0xF3, 0x00, 0x00, 0xC0, 0x09, 0x00, 0x00, 0xB0, 0xFF, 0x00, 0x00, 0x40, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0D, 0x00, 0x00, 0xB0, 0xFF, 0x00, 0x00, 0x40, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF9, 0x00, 0x00, 0xB0, 0xFF, 0x00, 0x00, 0x40, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x06, 0x0A, 0x0D, 0x10, 0x13, 0x16, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD9, 0x01, 0x20, 0x00, 0xEB, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xD0, 0x05, 0x00, 0x00, 0x10, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0x02, 0x2D, 0x00, 0xE2, 0xFF, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static void mission_spawn_apply_primary_phase(void);
static void mission_spawn_apply_secondary_phase(void);
static void mission_spawn_start_signal_script(void);
static uint8 actor_phase_selector(uint8 phase, uint32 table_offset);

void mission_spawn_reset(void)
{
    FUNCTION_MARKER(0x8005B648, "ACE3.BIN");
    mission_flow.start_pose_count = 0;
}

void mission_player_set_pose(const ACTOR_POSE *native_record, const MISSION_ACTOR *source_actor)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;

    FLIGHT_DYNAMICS *state;
    FLIGHT_DYNAMICS *auxiliary;
    FLIGHT_STATUS *other;
    FLIGHT_COMMAND *angles;
    FLIGHT_INPUT *input;
    MATRIX matrix = {0};
    uint32 flags, a, b, c, d;
    FUNCTION_MARKER(0x80072780, "ACE3.BIN");
    pose_owner_object = mission_actor(0);
    actor_motion_object = mission_actor_motion(0);
    actor_traits_object = mission_actor_traits(0);
    state = &flight_dynamics;
    flags = actor_traits_object->flags;
    input = &flight_input;
    actor_traits_object->flags = (uint32)(flags | 0x1C000);
    angles = &flight_command;
    FLIGHT_CONTROL *smooth = &flight_control;
    flight_orientation.response[0] = (uint16)(0);
    flight_orientation.response[1] = (uint16)(0);
    flight_orientation.response[2] = (uint16)(0);
    flight_orientation.angle_delta.value[0] = (uint16)(0);
    flight_orientation.angle_delta.value[1] = (uint16)(0);
    flight_orientation.angle_delta.value[2] = (uint16)(0);
    state->control_gain = (uint16)(4096);
    angles->pitch = (uint16)(0);
    angles->yaw = (uint16)(0);
    angles->roll = (uint16)(0);

    angles->thrust = (uint16)(4096);
    smooth->axis[0] = (uint16)(0);
    smooth->axis[1] = (uint16)(0);
    smooth->axis[2] = (uint16)(0);
    smooth->thrust = (uint16)(0);
    smooth->brake = (uint16)(0);
    smooth->emitter_bits = (uint32)(0);
    input->axis[0] = (uint16)(0);
    input->axis[1] = (uint16)(0);
    input->axis[2] = (uint16)(0);
    input->axis[3] = (uint16)(0);
    if (native_record)
    {
        a = native_record->pose.position[0];
        b = native_record->pose.position[1];
        c = native_record->pose.position[2];
    }
    else
    {
        a = source_actor->pose.position[0];
        b = source_actor->pose.position[1];
        c = source_actor->pose.position[2];
    }
    pose_owner_object->pose.position[0] = (uint32)(a);
    pose_owner_object->pose.position[1] = (uint32)(b);
    pose_owner_object->pose.position[2] = (uint32)(c);
    if (native_record)
    {
        a = native_record->pose.angles[0] | ((uint32)native_record->pose.angles[1] << 16);
        b = native_record->pose.angles[2];
    }
    else
    {
        a = source_actor->pose.angles[0] | ((uint32)source_actor->pose.angles[1] << 16);
        b = source_actor->pose.angles[2];
    }
    {
        uint32 pose_word = (a);
        pose_owner_object->pose.angles[0] = (uint16)pose_word;
        pose_owner_object->pose.angles[1] = (uint16)(pose_word >> 16);
    }
    {
        uint32 pose_word = (b);
        pose_owner_object->pose.angles[2] = (uint16)pose_word;
    }
    state->speed = (uint32)(0x21000);
    rotation_build(&matrix, pose_owner_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x1080);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    other = &flight_status;
    a <<= 5;
    b <<= 5;
    c <<= 5;
    actor_motion_object->velocity[0] = (uint32)(a);
    auxiliary = &flight_dynamics;
    actor_motion_object->velocity[1] = (uint32)(b);
    actor_motion_object->velocity[2] = (uint32)(c);
    auxiliary->events = (uint32)(0);
    other->events = (uint32)(0);
    {
        (void)(flight_orientation_update_delta(&flight_orientation, pose_owner_object));
        return;
    }
}

void mission_spawn_apply(void)
{
    MISSION_ACTOR *pose_owner_table;
    uint32 actor_index_state;
    uint32 actor_index_table;

    const FLIGHT_TARGET_PROFILE *profile;
    uint32 stage, mask, special, scenario, value, radius, difficulty, random, phase, product, other, a, b, c;
    sint32 mission;
    /* Defines pose padding and the angle omitted for negative difficulty */
    ACTOR_POSE pose = {0};
    uint32 angle = 0;
    FUNCTION_MARKER(0x8005B654, "ACE3.BIN");
    mission = (sint16)game_runtime.selection.mission;
    stage = game_runtime.mission_control.argument;
    value = mission_flow.request.scene_arg;
    mission_flow.request.saved_scene_arg = (uint8)(value);
    mask = 0;
    special = 0;
    scenario = 0xFFFFFFFF;
    switch (mission)
    {
        case 4:
            if (stage == 0)
            {
                scenario = 0;
                mask = 0x207;
            }
            else if (stage == 1)
            {
                scenario = 1;
                mask = 0x100;
            }
            else if (stage == 2)
            {
                scenario = 2;
                mask = 0x8000;
            }
            break;
        case 7:
            if (stage == 0)
            {
                scenario = 3;
                mask = 0x2001;
                special = 1;
            }
            else if (stage == 1)
            {
                scenario = 4;
                mask = 0x8000;
            }
            else if (stage == 2)
            {
                scenario = 5;
                mask = 0x101;
            }
            break;
        case 9:
            if (stage == 0)
            {
                scenario = 6;
                mask = 0x400;
            }
            else if (stage == 1)
            {
                scenario = 7;
                mask = 0x401;
            }
            break;
        case 14:
            scenario = 8;
            mask = 0x101;
            break;
        case 24:
            scenario = 9;
            mask = 0x402;
            break;
        case 25:
            scenario = 10;
            mask = 0x102;
            break;
        case 29:
            if (stage == 0)
            {
                scenario = 11;
                mask = 0x208;
            }
            else if (stage == 1)
            {
                scenario = 12;
                mask = 0x803;
            }
            else if (stage == 2)
            {
                scenario = 13;
                mask = 0x8000;
            }
            break;
        case 31:
            scenario = 14;
            mask = 0x4009;
            special = 1;
            break;
        case 33:
            scenario = 15;
            mask = 0x104;
            break;
        case 34:
            scenario = 16;
            mask = 0x2003;
            special = 1;
            break;
        case 41:
            if (stage == 0)
            {
                scenario = 17;
                mask = 0x2006;
                special = 1;
            }
            else if (stage == 1)
            {
                scenario = 18;
                mask = 0x105;
            }
            break;
        case 42:
            scenario = 19;
            mask = 0x1000;
            break;
        case 44:
            if (stage == 0)
            {
                scenario = 20;
                mask = 0x803;
            }
            else if (stage == 1)
            {
                scenario = 21;
                mask = 0x20B;
            }
            else if (stage == 2)
            {
                scenario = 22;
                mask = 0x8000;
            }
            break;
        case 50:
            scenario = 23;
            mask = 0x2003;
            special = 1;
            break;
    }
    if (scenario != 0xFFFFFFFF)
        mission_flow.scenario_idx = scenario;
    mission_flow.scenario_mask = mask;
    hud_notice_reset();
    if ((mask & 0x6000) != 0)
    {
        game_runtime.metrics.timer_mode = (uint8)(255);
        game_runtime.metrics.display_ticks = (uint32)(0);
    }
    else if ((mask & 0x8000) != 0)
    {
        game_runtime.metrics.timer_mode = (uint8)(0);
    }
    else
    {
        game_runtime.metrics.timer_mode = (uint8)(1);
        game_runtime.metrics.display_ticks = (uint32)(5400);
    }
    effect_pools_reset();
    mission_status_text_reset();
    if ((mask & 0x8000) == 0)
        flight_dynamics_select_sequence(mask);
    camera_restore_player_mode(mask);
    if (special != 0)
        flight_dynamics_reset_event_mode();
    else if ((mask & 0x8000) != 0)
    {
        flight_dynamics_enter_transition_control();
        game_runtime.metrics.proximity_count = (uint16)(0);
    }
    else if ((mask & 0x1000) != 0)
        flight_dynamics_enter_alternate_control();
    else
        flight_dynamics_enter_transition_control();
    value = game_runtime.flags;
    game_runtime.flags = value | 0x10000000;
    mission_audio_initialize_groups();
    subtitles_configure(0, 0, 0);
    if ((mask & 0x300) != 0)
    {
        profile = &flight_target_profiles[mask & 255];
        value = profile->position[0];
        pose.pose.position[0] = value;
        value = profile->position[1];
        pose.pose.position[1] = value + 0xFF000000;
        value = profile->position[2];
        pose.pose.angles[0] = 0;
        pose.pose.position[2] = value;
        value = profile->angles[1];
        pose.pose.angles[2] = 0;
        pose.pose.angles[1] = value;
        mission = (sint16)game_runtime.selection.mission;
        radius = mission == 33 ? 0x9000 : 0x6000;
        difficulty = game_runtime.selection.difficulty;
        if (difficulty == 0)
            angle = profile->angles[1] + 0x2000;
        else if (difficulty == 1)
        {
            random = game_random();
            angle = profile->angles[1] + (random & 1023) + 7680;
        }
        else if ((sint32)difficulty >= 2)
        {
            random = game_random();
            angle = profile->angles[1] + (random & 4095) + 6144;
            pose.pose.angles[1] = angle - 0x2000;
        }
        phase = angle & 0x3FFF;
        value = game_wave_at(phase * 2);
        product = radius * value;
        value = game_wave_at((phase + 0x1000) * 2);
        other = radius * value;
        value = pose.pose.position[0];
        pose.pose.position[0] = value + (uint32)((sint32)product >> 2);
        value = pose.pose.position[2];
        pose.pose.position[2] = value + (uint32)((sint32)other >> 2);
    }
    else if ((mask & 0x1C00) != 0)
    {
        pose.pose.position[0] = 0;
        pose.pose.position[1] = 0xFD8F0000;
        pose.pose.position[2] = 0;
        pose.pose.angles[0] = 0;
        pose.pose.angles[1] = 0;
        pose.pose.angles[2] = 0;
    }
    else
    {
        value = mission_flow.start_pose_count;
        if (value == 0)
        {
            actor_index_table = 0;
            actor_index_state = 0;
            pose_owner_table = mission_actor(actor_index_table);
            a = pose_owner_table->pose.position[0];
            b = pose_owner_table->pose.position[1];
            c = pose_owner_table->pose.position[2];
            pose.pose.position[0] = a;
            pose.pose.position[1] = b;
            pose.pose.position[2] = c;
            pose.pose.position[1] = 0xFD8F0000;
            pose.pose.angles[0] = 0;
            pose_owner_table = mission_actor(actor_index_state);
            value = pose_owner_table->pose.angles[1];
            pose.pose.angles[2] = 0;
            pose.pose.angles[1] = value;
            mission_flow.start_pose = pose;
        }
        else
        {
            pose = mission_flow.start_pose;
        }
        value = mission_flow.start_pose_count;
        mission_flow.start_pose_count = value + 1;
    }
    if ((mask & 0x6000) == 0)
        mission_player_set_pose(&pose, NULL);
    mission_primary_prepare_scenario_actors();
    mission_secondary_prepare_scenario_actors();
    hud_initialize_default_colors();
}

typedef struct
{
    uint8 actor_index, kind;
    sint16 script_offset;
} MISSION_EVENT_BINDING_RECORD;

typedef char mission_event_binding_record_layout[sizeof(MISSION_EVENT_BINDING_RECORD) == 4 && offsetof(MISSION_EVENT_BINDING_RECORD, actor_index) == 0 && offsetof(MISSION_EVENT_BINDING_RECORD, kind) == 1 && offsetof(MISSION_EVENT_BINDING_RECORD, script_offset) == 2 ? 1 : -1];

typedef struct
{
    uint32 initial[1], reserved_script;
    uint32 enter_mode_2[4], enter_mode_8[4];
    uint32 first_target_count[4], second_target_count[4];
    uint32 timed[4][16];
    uint32 signal[4];
    uint16 ticks[4][16];
    uint32 enter_mode_16[8], enter_mode_64[8];
    uint32 enter_mode_16384[4], shared_mode[4];
    uint32 enter_mode_1024[4], enter_mode_8192[4];
} MISSION_EVENT_PUBLICATION;

typedef char mission_event_publication_layout[sizeof(MISSION_EVENT_PUBLICATION) == 600 && offsetof(MISSION_EVENT_PUBLICATION, reserved_script) == 4 && offsetof(MISSION_EVENT_PUBLICATION, timed) == 72 && offsetof(MISSION_EVENT_PUBLICATION, ticks) == 344 && offsetof(MISSION_EVENT_PUBLICATION, initial) == 0 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_2) == 8 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_8) == 24 && offsetof(MISSION_EVENT_PUBLICATION, first_target_count) == 40 && offsetof(MISSION_EVENT_PUBLICATION, second_target_count) == 56 && offsetof(MISSION_EVENT_PUBLICATION, signal) == 328 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_16) == 472 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_64) == 504 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_16384) == 536 && offsetof(MISSION_EVENT_PUBLICATION, shared_mode) == 552 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_1024) == 568 && offsetof(MISSION_EVENT_PUBLICATION, enter_mode_8192) == 584 ? 1 : -1];

static struct
{
    MISSION_SCRIPT_REFERENCE initial[1];
    MISSION_SCRIPT_REFERENCE reserved_script;
    MISSION_SCRIPT_REFERENCE enter_mode_2[4];
    MISSION_SCRIPT_REFERENCE enter_mode_8[4];
    MISSION_SCRIPT_REFERENCE first_target_count[4];
    MISSION_SCRIPT_REFERENCE second_target_count[4];
    MISSION_SCRIPT_REFERENCE signal[4];
    MISSION_SCRIPT_REFERENCE enter_mode_16[8];
    MISSION_SCRIPT_REFERENCE enter_mode_64[8];
    MISSION_SCRIPT_REFERENCE enter_mode_16384[4];
    MISSION_SCRIPT_REFERENCE shared_mode[4];
    MISSION_SCRIPT_REFERENCE enter_mode_1024[4];
    MISSION_SCRIPT_REFERENCE enter_mode_8192[4];
    MISSION_SCRIPT_REFERENCE timed[4][16];
    uint16 ticks[4][16];
    const MISSION_EVENT_PUBLICATION *publication;
    uint8 ready;
} mission_events;

static const struct
{
    const MISSION_SCRIPT_REFERENCE *scripts;
    uint32 count;
    uint32 legacy_offset;
} mission_event_lanes[] = {
    {mission_events.initial, 1, 0}, {mission_events.enter_mode_2, 4, 8}, {mission_events.enter_mode_8, 4, 24}, {mission_events.first_target_count, 4, 40}, {mission_events.second_target_count, 4, 56}, {mission_events.signal, 4, 328}, {mission_events.enter_mode_16, 8, 472}, {mission_events.enter_mode_64, 8, 504}, {mission_events.enter_mode_16384, 4, 536}, {mission_events.shared_mode, 4, 552}, {mission_events.enter_mode_1024, 4, 568}, {mission_events.enter_mode_8192, 4, 584},
};

void mission_events_reset(void)
{
    mission_events.ready = 0;
    mission_events.publication = NULL;
}

MISSION_SCRIPT_REFERENCE mission_event_script(MISSION_EVENT_LANE lane, uint32 index)
{
    if (mission_events.ready && index < mission_event_lanes[lane].count)
        return mission_event_lanes[lane].scripts[index];
    uint32 wire_offset = mission_event_lanes[lane].legacy_offset + index * 4;
    uint32 encoded;
    if (mission_events.publication && wire_offset <= sizeof(*mission_events.publication) - sizeof(encoded))
    {
        memcpy(&encoded, (const uint8 *)mission_events.publication + wire_offset, sizeof(encoded));
        return mission_script_reference(encoded);
    }
    abort();
}

MISSION_SCRIPT_REFERENCE mission_event_timed_script(uint32 phase, uint32 slot)
{
    if (mission_events.ready && phase < 4 && slot < 16)
        return mission_events.timed[phase][slot];
    uint32 wire_offset = (phase << 6) + slot * 4 + 72;
    uint32 encoded;
    if (mission_events.publication && wire_offset <= sizeof(*mission_events.publication) - sizeof(encoded))
    {
        memcpy(&encoded, (const uint8 *)mission_events.publication + wire_offset, sizeof(encoded));
        return mission_script_reference(encoded);
    }
    abort();
}

uint16 mission_event_tick(uint32 phase, uint32 slot)
{
    if (mission_events.ready && phase < 4 && slot < 16)
        return mission_events.ticks[phase][slot];
    uint32 wire_offset = (phase << 5) + slot * 2 + 344;
    uint16 encoded;
    if (mission_events.publication && wire_offset <= sizeof(*mission_events.publication) - sizeof(encoded))
    {
        memcpy(&encoded, (const uint8 *)mission_events.publication + wire_offset, sizeof(encoded));
        return encoded;
    }
    abort();
}

/* retain the guest table for legacy indexed reads and script spans */
#pragma pack(push, 1)

typedef struct
{
    uint32 reserved;
    sint16 initial;
    sint16 reserved_script;
    sint16 enter_mode_2[4];
    sint16 enter_mode_8[4];
    sint16 first_target_count[4];
    sint16 second_target_count[4];
    sint16 timed[4][16];
    sint16 signal[4];
    uint16 ticks[4][16];
    sint16 enter_mode_16[8];
    sint16 enter_mode_64[8];
    sint16 enter_mode_16384[4];
    sint16 shared_mode[4];
    sint16 enter_mode_1024[4];
    sint16 enter_mode_8192[4];
    sint32 binding_count;
} MISSION_EVENT_HEADER;

#pragma pack(pop)

typedef char
    mission_event_header_layout[sizeof(MISSION_EVENT_HEADER) == 372 && offsetof(MISSION_EVENT_HEADER, reserved) == 0 && offsetof(MISSION_EVENT_HEADER, initial) == 4 && offsetof(MISSION_EVENT_HEADER, reserved_script) == 6 && offsetof(MISSION_EVENT_HEADER, enter_mode_2) == 8 && offsetof(MISSION_EVENT_HEADER, enter_mode_8) == 16 && offsetof(MISSION_EVENT_HEADER, first_target_count) == 24 && offsetof(MISSION_EVENT_HEADER, second_target_count) == 32 && offsetof(MISSION_EVENT_HEADER, timed) == 40 && offsetof(MISSION_EVENT_HEADER, signal) == 168 && offsetof(MISSION_EVENT_HEADER, ticks) == 176 && offsetof(MISSION_EVENT_HEADER, enter_mode_16) == 304 && offsetof(MISSION_EVENT_HEADER, enter_mode_64) == 320 && offsetof(MISSION_EVENT_HEADER, enter_mode_16384) == 336 && offsetof(MISSION_EVENT_HEADER, shared_mode) == 344 && offsetof(MISSION_EVENT_HEADER, enter_mode_1024) == 352 && offsetof(MISSION_EVENT_HEADER, enter_mode_8192) == 360 && offsetof(MISSION_EVENT_HEADER, binding_count) == 368 ? 1 : -1];

static MISSION_SCRIPT_REFERENCE mission_event_reference(const MISSION_SCRIPT_REFERENCE *root, uint32 address)
{
    uint32 offset = address - root->legacy_address;
    if (root->bytes && offset < root->size)
        return (MISSION_SCRIPT_REFERENCE){root->bytes + offset, root->size - offset, address};
    return mission_script_reference(address);
}

void mission_event_scripts_initialize(MISSION_VARIANT_INPUT native_variant)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 wire_word_0;
    MISSION_SCRIPT_REFERENCE script_root = native_variant.root;
    uint32 source = native_variant.selected.legacy_address;
    uint32 out, i, j, src, value, kind, index, target, result;
    uint32 record, count_index = 0;
    sint32 count, relative;
    const MISSION_EVENT_HEADER *header;
    FUNCTION_MARKER(0x80053E08, "ACE3.BIN");
    if (!native_variant.selected.bytes || native_variant.selected.size < 16)
        abort();
    source += (memcpy(&wire_word_0, native_variant.selected.bytes + 12, sizeof(wire_word_0)), wire_word_0);
    buffer_allocation = arena_alloc_backed(600);
    out = buffer_allocation.encoded_begin;
    mission_events.ready = 0;
    MISSION_EVENT_PUBLICATION *publication = (MISSION_EVENT_PUBLICATION *)buffer_allocation.buffer.data;
    mission_events.publication = publication;
    uint32 header_offset = source - native_variant.selected.legacy_address;
    if (source < native_variant.selected.legacy_address || header_offset > native_variant.selected.size || 372 > native_variant.selected.size - header_offset)
        abort();
    header = (const MISSION_EVENT_HEADER *)(native_variant.selected.bytes + header_offset);
    const MISSION_SCRIPT_REFERENCE primary_root = mission_script_primary_source();
    if (script_root.bytes != primary_root.bytes || script_root.size != primary_root.size || script_root.legacy_address != primary_root.legacy_address)
        script_root = (MISSION_SCRIPT_REFERENCE){0};
    mission_events.initial[0] = mission_event_reference(&script_root, source + (uint32)(sint16)header->initial);
    publication->initial[0] = mission_events.initial[0].legacy_address;
    mission_events.reserved_script = mission_event_reference(&script_root, source + (uint32)(sint16)header->reserved_script);
    publication->reserved_script = mission_events.reserved_script.legacy_address;
    for (i = 0; i < 4; ++i)
    {
        src = i * 2;
        mission_events.enter_mode_2[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->enter_mode_2[i]);
        publication->enter_mode_2[i] = mission_events.enter_mode_2[i].legacy_address;
        mission_events.enter_mode_8[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->enter_mode_8[i]);
        publication->enter_mode_8[i] = mission_events.enter_mode_8[i].legacy_address;
        mission_events.first_target_count[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->first_target_count[i]);
        publication->first_target_count[i] = mission_events.first_target_count[i].legacy_address;
        mission_events.second_target_count[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->second_target_count[i]);
        publication->second_target_count[i] = mission_events.second_target_count[i].legacy_address;
        mission_events.signal[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->signal[i]);
        publication->signal[i] = mission_events.signal[i].legacy_address;
        for (j = 0; j < 16; ++j)
        {
            src = i * 32 + j * 2;
            mission_events.timed[i][j] = mission_event_reference(&script_root, source + (uint32)(sint16)header->timed[i][j]);
            publication->timed[i][j] = mission_events.timed[i][j].legacy_address;
            mission_events.ticks[i][j] = header->ticks[i][j];
            publication->ticks[i][j] = mission_events.ticks[i][j];
        }
    }
    src = 0;
    for (i = 0; i < 8; ++i)
    {
        mission_events.enter_mode_16[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->enter_mode_16[i]);
        publication->enter_mode_16[i] = mission_events.enter_mode_16[i].legacy_address;
        value = source + (uint32)(sint16)header->enter_mode_64[i];
        src += 2;
        mission_events.enter_mode_64[i] = mission_event_reference(&script_root, value);
        publication->enter_mode_64[i] = mission_events.enter_mode_64[i].legacy_address;
    }
    src = 0;
    for (i = 0; i < 4; ++i)
    {
        mission_events.enter_mode_16384[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->enter_mode_16384[i]);
        publication->enter_mode_16384[i] = mission_events.enter_mode_16384[i].legacy_address;
        value = source + (uint32)(sint16)header->shared_mode[i];
        src += 2;
        mission_events.shared_mode[i] = mission_event_reference(&script_root, value);
        publication->shared_mode[i] = mission_events.shared_mode[i].legacy_address;
    }
    src = 0;
    for (i = 0; i < 4; ++i)
    {
        mission_events.enter_mode_1024[i] = mission_event_reference(&script_root, source + (uint32)(sint16)header->enter_mode_1024[i]);
        publication->enter_mode_1024[i] = mission_events.enter_mode_1024[i].legacy_address;
        value = source + (uint32)(sint16)header->enter_mode_8192[i];
        src += 2;
        mission_events.enter_mode_8192[i] = mission_event_reference(&script_root, value);
        publication->enter_mode_8192[i] = mission_events.enter_mode_8192[i].legacy_address;
    }
    mission_events.ready = out != 0;
    src = 0;
    count = (sint32)header->binding_count;
    result = source + 372;
    if (count > 0)
    {
        record = result;
        do
        {
            uint32 binding_offset = record - native_variant.selected.legacy_address;
            if (record < native_variant.selected.legacy_address || binding_offset > native_variant.selected.size || sizeof(MISSION_EVENT_BINDING_RECORD) > native_variant.selected.size - binding_offset)
                abort();
            const uint8 *binding_bytes = native_variant.selected.bytes + binding_offset;
            if ((uintptr_t)binding_bytes & 1)
                abort();
            const MISSION_EVENT_BINDING_RECORD *binding = (const MISSION_EVENT_BINDING_RECORD *)binding_bytes;
            relative = binding->script_offset;
            kind = binding->kind;
            index = binding->actor_index;
            target = source + (uint32)relative;
            if (kind == 1)
            {
                if (index < 32)
                    mission_primary_binding(index & 0x03FFFFFFu)->destroy_script = mission_event_reference(&script_root, target);
                else
                    mission_secondary_binding(index - 32)->destroy_script = mission_event_reference(&script_root, target);
            }
            else if (kind == 2)
            {
                if (index < 32)
                {
                    index = binding->actor_index;
                    mission_primary_binding(index & 0x03FFFFFFu)->visible_script = mission_event_reference(&script_root, target);
                }
                else
                {
                    index = binding->actor_index;
                    mission_secondary_binding(index - 32)->visible_script = mission_event_reference(&script_root, target);
                }
            }
            else if (kind == 4)
            {
                if (index < 32)
                {
                    index = binding->actor_index;
                    mission_primary_binding(index & 0x03FFFFFFu)->stage_script = mission_event_reference(&script_root, target);
                }
                else
                {
                    index = binding->actor_index;
                    mission_secondary_binding(index - 32)->stage_script = mission_event_reference(&script_root, target);
                }
            }
            count = (sint32)header->binding_count;
            ++count_index;
            result = (sint32)count_index < count;
            record += 4;
        } while (result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void mission_spawn_start_signal_script(void)
{
    MISSION_SCRIPT_REFERENCE script;
    FUNCTION_MARKER(0x80053B78, "ACE3.BIN");
    script = mission_event_script(MISSION_EVENT_SIGNAL, 0);
    {
        mission_script_start_legacy(script);
        return;
    }
}

void mission_player_apply_initial_pose(void)
{
    const MISSION_PRIMARY_PARAMETERS *source;
    ACTOR_POSE record;
    uint32 a, b, c;
    FUNCTION_MARKER(0x8004D794, "ACE3.BIN");
    source = &mission_primary_binding(0)->parameters;
    a = source->pose.position[0];
    b = source->pose.position[1];
    c = source->pose.position[2];
    record.pose.position[0] = a;
    record.pose.position[1] = b;
    record.pose.position[2] = c;
    a = (source->pose.angles[0] | ((uint32)source->pose.angles[1] << 16));
    b = (source->pose.angles[2]);
    record.pose.angles[0] = a;
    record.pose.angles[1] = a >> 16;
    record.pose.angles[2] = b;
    {
        (void)(mission_player_set_pose(&record, NULL));
        return;
    }
}

void mission_spawn_begin_phase(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x80038C28, "ACE3.BIN");
    mission_metrics_begin_phase();
    mission_spawn_apply_primary_phase();
    mission_spawn_apply_secondary_phase();
    mission_audio_initialize_groups();
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x10000000;
    return;
}

static uint8 actor_phase_selector(uint8 phase, uint32 table_offset)
{
    return actor_phase_selectors[table_offset + phase];
}

static void mission_spawn_apply_primary_phase(void)
{
    const MISSION_PRIMARY_PARAMETERS *parameters;
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 selector, mask, index, value;
    FUNCTION_MARKER(0x8004DB40, "ACE3.BIN");
    selector = actor_phase_selector((uint8)game_runtime.mission_control.argument, ACTOR_PHASE_PRIMARY);
    mask = selector & 0xF0;
    for (index = 2; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)));
        if ((actor_traits_object->flags & 0x4000) == 0)
            continue;
        if ((sint16)actor_traits_object->health <= 0)
            continue;
        parameters = &mission_primary_binding(index & 0x03FFFFFFu)->parameters;
        value = parameters->selectors[0];
        if (value == selector || value == mask)
            mission_primary_show_actor(index);
        value = parameters->selectors[1];
        if (value == selector || value == mask)
            mission_primary_activate_actor(index);
    }
    return;
}

static void mission_spawn_apply_secondary_phase(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    const MISSION_SECONDARY_PARAMETERS *parameters;

    uint32 selector, mask, index, value;
    FUNCTION_MARKER(0x8004E620, "ACE3.BIN");
    selector = actor_phase_selector((uint8)game_runtime.mission_control.argument, ACTOR_PHASE_SECONDARY);
    mask = selector & 0xF0;
    for (index = 0; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits((((uint32)((sint32)(index * 4) >> 2)) + 32));
        if ((actor_traits_object->flags & 0x4000) == 0)
            continue;
        if ((sint16)actor_traits_object->health <= 0)
            continue;
        parameters = &mission_secondary_binding(index)->parameters;
        value = parameters->selectors[0];
        if (value == selector || value == mask)
            mission_actor_show_secondary(index);
        value = parameters->selectors[1];
        if (value == selector || value == mask)
            mission_secondary_activate_actor(index);
    }
    if ((sint16)game_runtime.selection.mission == 13 && game_runtime.mission_control.argument == 0)
        mission_secondary_reset_points();
    if ((sint16)game_runtime.selection.mission == 21 && game_runtime.mission_control.argument == 1)
        mission_spawn_start_signal_script();
    if ((sint16)game_runtime.selection.mission == 43 && game_runtime.mission_control.argument == 0)
        mission_secondary_reset_signals();
    if ((sint16)game_runtime.selection.mission == 55 && game_runtime.mission_control.argument == 0)
        mission_secondary_shuffle_animations();
    {
        (void)(proximity_audio_reset());
        return;
    }
}
