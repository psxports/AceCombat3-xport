#include "game_functions.h"

const FLIGHT_MODEL_OFFSET flight_model_offsets[FLIGHT_MODEL_OFFSET_COUNT] =
{
    {{0x00000000, 0xFFFFD000, 0x0000A000}},
    {{0x00000000, 0xFFFFAEE2, 0x0000E0CC}},
    {{0x00000000, 0xFFFFC19A, 0xFFFFEF86}},
    {{0x00000000, 0xFFFFC19A, 0xFFFFEF86}},
    {{0xFFFF7290, 0xFFFFC9EC, 0x0000ACCC}},
    {{0x00000000, 0xFFFF7F0B, 0x0001C147}},
    {{0x00000000, 0xFFFFD23E, 0xFFFFDAB9}},
    {{0x00000000, 0xFFFFED48, 0x0001EF33}},
    {{0x000097D7, 0xFFFFF7AF, 0x00003A8F}},
    {{0x00000000, 0xFFFFB734, 0x000180CC}},
    {{0x00000000, 0xFFFFBB5D, 0x00006D47}},
    {{0x00000000, 0xFFFFBF86, 0x0000F599}},
    {{0x00000000, 0xFFFFAEE2, 0x0000E0CC}},
    {{0x00000000, 0xFFFFC19A, 0xFFFFEF86}},
    {{0x00000000, 0xFFFFF171, 0x00027A8F}},
    {{0xFFFF3000, 0xFFFFBD71, 0x0002E8A3}},
    {{0x00000000, 0xFFFFC7D8, 0x00004199}},
    {{0x00000000, 0xFFFFBD71, 0x000144A3}},
    {{0x00000000, 0xFFFFF171, 0x00027A8F}},
    {{0xFFFF3000, 0xFFFFBD71, 0x0002E8A3}},
    {{0x00000000, 0xFFFFD87B, 0x00024999}},
    {{0x00000000, 0xFFFF7CF6, 0x00011C28}},
    {{0x00000000, 0xFFFFE2E2, 0x000248A3}},
    {{0x00000000, 0xFFFFEF5D, 0x00020614}},
    {{0x00000000, 0x00000000, 0x00000000}},
    {{0x000061C2, 0xFFFF2BD8, 0x0001A000}},
    {{0x00000000, 0xFFFF6E67, 0x0002AC51}},
    {{0x0000C385, 0xFFFFE4F6, 0x0000C9C2}},
    {{0x000097D7, 0xFFFFF7AF, 0x00003A8F}},
    {{0x00000000, 0xFFFFBD71, 0x000144A3}},
    {{0x00000000, 0xFFFFE2E2, 0x000248A3}},
};

const FLIGHT_RELATIVE_PROFILE flight_relative_profiles[FLIGHT_RELATIVE_PROFILE_COUNT] =
{
    {2, {0x00000000, 0x0005651E, 0xFFF8651F}},
    {3, {0x00000000, 0x00045F0A, 0xFFF2F171}},
    {2, {0x00000000, 0x0005207A, 0xFFEFB386}},
    {2, {0x00000000, 0x000AA000, 0xFFEE8000}},
};

FLIGHT_DYNAMICS flight_dynamics;

FLIGHT_DYNAMICS *flight_dynamics_enter_standard_control(void)
{
    FLIGHT_DYNAMICS *state;
    FUNCTION_MARKER(0x80076274, "ACE3.BIN");
    state = &flight_dynamics;
    state->flags = (uint32)(1);
    state->mode = (uint32)(1);
    state->phase = (uint32)(0);
    return state;
}

FLIGHT_DYNAMICS *flight_dynamics_enter_direct_control(void)
{
    FLIGHT_DYNAMICS *state;
    FUNCTION_MARKER(0x80076290, "ACE3.BIN");
    state = &flight_dynamics;
    state->flags = (uint32)(1);
    state->mode = (uint32)(0x01000000);
    state->phase = (uint32)(0);
    return state;
}

FLIGHT_DYNAMICS *flight_dynamics_enter_alternate_control(void)
{
    FLIGHT_DYNAMICS *state;
    FUNCTION_MARKER(0x800762B0, "ACE3.BIN");
    state = &flight_dynamics;
    state->flags = (uint32)(1);
    state->mode = (uint32)(0x02000000);
    state->phase = (uint32)(0);
    return state;
}

FLIGHT_DYNAMICS *flight_dynamics_enter_transition_control(void)
{
    FLIGHT_DYNAMICS *state;
    FUNCTION_MARKER(0x800762D0, "ACE3.BIN");
    state = &flight_dynamics;
    state->flags = (uint32)(2048);
    state->mode = (uint32)(4);
    state->phase = (uint32)(0);
    return state;
}

FLIGHT_DYNAMICS *flight_dynamics_select_idle_control(uint32 mode)
{
    FLIGHT_DYNAMICS *state;
    uint32 flags;
    FUNCTION_MARKER(0x800762F0, "ACE3.BIN");
    state = &flight_dynamics;
    flags = state->flags;
    state->mode = (uint32)(mode != 0 ? 2 : 0x00400000);
    state->phase = (uint32)(0);
    state->flags = (uint32)(flags | 0x40);
    return state;
}

intptr flight_dynamics_set_sequence_mode(uint32 mode)
{
    FLIGHT_DYNAMICS *state;
    uint32 flags;
    FUNCTION_MARKER(0x80076360, "ACE3.BIN");
    if (mode == 0)
    {
        state = &flight_dynamics;
        state->flags = (uint32)(16);
        state->mode = (uint32)(0x4000000);
        state->phase = (uint32)(0);
        state->mode_timer = (uint16)(0);
        state->target_ticks = (uint16)(0);
        return (intptr)state;
    }
    if (mode == 1)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->mode = (uint32)(0x8000000);
        state->flags = (uint32)(flags | 64);
        return (intptr)state;
    }
    state = &flight_dynamics;
    state->mode = (uint32)(0x10000000);
    return 0x10000000;
}

intptr flight_dynamics_advance_mode(void)
{
    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 phase, mode;
    FUNCTION_MARKER(0x80073A30, "ACE3.BIN");
    state = &flight_dynamics;
    context = &flight_status;
    state->events = (uint32)(0);
    context->events = (uint32)(0);
    phase = state->mode;
    if (phase == 0x4000)
        return flight_target_set_tertiary_mode(0, 1);
    if ((sint32)phase < 0x4001)
    {
        if (phase == 8)
        {
            mode = mission_flow.flight_selection.mode;
            return flight_target_set_primary_mode(mode != 0, 1);
        }
        if (phase == 0x100)
        {
            mode = mission_flow.flight_selection.mode;
            return flight_target_set_secondary_mode(mode != 2, 1);
        }
        return 2;
    }
    if (phase == 0x8000)
        return flight_target_set_tertiary_mode(1, 1);
    if (phase == 0x04000000)
        return flight_dynamics_set_sequence_mode(1);
    return 0x04000000;
}

intptr flight_dynamics_enter_selected_mode(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x80076F04, "ACE3.BIN");
    mode = mission_flow.flight_selection.mode;
    if (mode >= 7)
        return 0;
    switch (mode)
    {
        case 0:
            return flight_target_set_primary_mode(0, 0);
        case 1:
            return flight_target_set_primary_mode(1, 0);
        case 2:
            return flight_target_set_secondary_mode(0, 0);
        case 3:
            return flight_target_set_secondary_mode(1, 0);
        case 4:
            return flight_dynamics_set_sequence_mode(0);
        case 5:
            return flight_target_set_tertiary_mode(0, 0);
        case 6:
            return flight_target_set_tertiary_mode(1, 0);
    }
    return 0;
}

static void flight_relative_pose_calculate(const uint32 first_vector[3], const uint32 second_vector[3], ACTOR_RELATIVE_POSE *native_output, const NATIVE_POSE *native_first, const NATIVE_POSE *native_second)
{
    MATRIX first_matrix, second_matrix;
    uint16 first_local[3], second_local[3];
    uint32 first_rotated[3], second_rotated[3], delta[3], reduced[3];
    uint32 ax, ay, az, reference, cached, difference, value, a, b, c, d, e, i, pass, total, result;
    FUNCTION_MARKER(0x800765B0, "ACE3.BIN");
    ax = ((sint16)native_second->angles[0]);
    ay = ((sint16)native_second->angles[1]);
    reference = ((sint16)native_first->angles[0]);
    az = ((sint16)native_second->angles[2]);
    cached = ((uint16)native_first->angles[0]);
    difference = reference - ax;
    if ((sint32)difference < 8192)
    {
        ax -= 16384;
        difference = reference - ax;
    }
    if ((sint32)difference > 8192)
        ax += 16384;
    reference = ((sint16)native_first->angles[1]);
    difference = reference - ay;
    if ((sint32)difference < 8192)
    {
        ay -= 16384;
        difference = reference - ay;
    }
    if ((sint32)difference > 8192)
        ay += 16384;
    reference = ((sint16)native_first->angles[2]);
    difference = reference - az;
    if ((sint32)difference < 8192)
    {
        az -= 16384;
        difference = reference - az;
    }
    if ((sint32)difference > 8192)
        az += 16384;
    value = ax - cached;
    native_output->angle_delta.pitch = (sint16)(value);
    reference = ((uint16)native_first->angles[1]);
    value = ay - reference;
    native_output->angle_delta.yaw = (sint16)(value);
    reference = ((uint16)native_first->angles[2]);
    value = az - reference;
    native_output->angle_delta.roll = (sint16)(value);
    rotation_build(&first_matrix, native_first->angles);
    rotation_build(&second_matrix, native_second->angles);
    for (i = 0; i < 3; ++i)
    {
        value = first_vector[i];
        first_local[i] = ((sint32)value >> 6);
    }
    for (i = 0; i < 3; ++i)
    {
        value = second_vector[i];
        second_local[i] = ((sint32)value >> 6);
    }
    a = ((uint32)(uint16)first_matrix.m[0][0] | ((uint32)(uint16)first_matrix.m[0][1] << 16));
    b = ((uint32)(uint16)first_matrix.m[0][2] | ((uint32)(uint16)first_matrix.m[1][0] << 16));
    c = ((uint32)(uint16)first_matrix.m[1][1] | ((uint32)(uint16)first_matrix.m[1][2] << 16));
    d = ((uint32)(uint16)first_matrix.m[2][0] | ((uint32)(uint16)first_matrix.m[2][1] << 16));
    e = ((uint16)first_matrix.m[2][2]);
    SetRotMatrix(&first_matrix);
    xport_gte_write_data(0, first_local[0] | ((uint32)first_local[1] << 16));
    xport_gte_write_data(1, first_local[2]);
    xport_gte_execute(0x486012);
    first_rotated[0] = xport_gte_read_data(25);
    first_rotated[1] = xport_gte_read_data(26);
    first_rotated[2] = xport_gte_read_data(27);
    a = ((uint32)(uint16)second_matrix.m[0][0] | ((uint32)(uint16)second_matrix.m[0][1] << 16));
    b = ((uint32)(uint16)second_matrix.m[0][2] | ((uint32)(uint16)second_matrix.m[1][0] << 16));
    c = ((uint32)(uint16)second_matrix.m[1][1] | ((uint32)(uint16)second_matrix.m[1][2] << 16));
    d = ((uint32)(uint16)second_matrix.m[2][0] | ((uint32)(uint16)second_matrix.m[2][1] << 16));
    e = ((uint16)second_matrix.m[2][2]);
    SetRotMatrix(&second_matrix);
    xport_gte_write_data(0, second_local[0] | ((uint32)second_local[1] << 16));
    xport_gte_write_data(1, second_local[2]);
    xport_gte_execute(0x486012);
    second_rotated[0] = xport_gte_read_data(25);
    second_rotated[1] = xport_gte_read_data(26);
    second_rotated[2] = xport_gte_read_data(27);
    a = ((uint32)native_second->position[0]);
    b = ((uint32)native_first->position[0]);
    c = ((uint32)native_first->position[1]);
    delta[0] = ((sint32)(a - b) >> 6);
    a = ((uint32)native_second->position[1]);
    b = ((uint32)native_second->position[2]);
    delta[1] = ((sint32)(a - c) >> 6);
    c = ((uint32)native_first->position[2]);
    delta[2] = ((sint32)(b - c) >> 6);
    delta[0] += second_rotated[0] - first_rotated[0];
    delta[1] += second_rotated[1] - first_rotated[1];
    value = delta[1] << 6;
    reduced[1] = ((sint32)delta[1] >> 6);
    native_output->displacement.word[1] = (uint32)(value);
    delta[2] += second_rotated[2] - first_rotated[2];
    reduced[0] = ((sint32)delta[0] >> 6);
    reduced[2] = ((sint32)delta[2] >> 6);
    value = delta[0] << 6;
    native_output->displacement.word[0] = (uint32)(value);
    value = delta[2] << 6;
    native_output->displacement.word[2] = (uint32)(value);
    for (pass = 0; pass < 2; ++pass)
        for (i = 0; i < 3; ++i)
        {
            reference = ((uint32)native_first->position[i]);
            value = (first_rotated[i] << 6) + reference;
            native_output->reference_position.word[i] = (uint32)(value);
        }
    total = (sint32)reduced[0] < 0 ? 0 - reduced[0] : reduced[0];
    value = (sint32)reduced[1] < 0 ? 0 - reduced[1] : reduced[1];
    total += value;
    value = (sint32)reduced[2] < 0 ? 0 - reduced[2] : reduced[2];
    total += value;
    result = 65536;
    if ((sint32)total <= 45056)
        result = game_vector_length_gte(reduced[0], reduced[1], reduced[2]);
    native_output->distance = (uint32)(result);
    { (void)(uint32)(result); return; }
}

uint32 flight_dynamics_relative_pose(const FLIGHT_RELATIVE_PROFILE *selection, const FLIGHT_MODEL_OFFSET *offsets, ACTOR_RELATIVE_POSE *native_output, const MISSION_ACTOR *native_object)
{
    const MISSION_ACTOR *native_target_actor;
    uint32 actor_index_objects;

    uint32 index, value, other, result;
    FUNCTION_MARKER(0x80076924, "ACE3.BIN");
    index = selection->actor_idx;
    actor_index_objects = 0;
    native_target_actor = mission_actor((actor_index_objects + ((uint32)((sint32)(index * 4) >> 2))));
    flight_relative_pose_calculate(selection->offset, offsets->position, native_output, &native_target_actor->pose, &native_object->pose);
    value = (sint16)((uint16)native_output->angle_delta.pitch);
    other = (sint16)((uint16)native_output->angle_delta.yaw);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)other < 0)
        other = 0 - other;
    result = (sint32)value < 365;
    if ((sint32)other >= 365)
        result = 0;
    value = (sint16)((uint16)native_output->angle_delta.roll);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value >= 365)
        result = 0;
    value = (native_output->displacement.word[0]);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x32000)
        result = 0;
    value = (native_output->displacement.word[1]);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x50000)
        result = 0;
    value = (native_output->displacement.word[2]);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 0x96000)
        result = 0;
    return result;
}

void flight_dynamics_update(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_MOTION *actor_motion_target;

    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_objects;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *settings_root;
    FLIGHT_TARGET_STATE *output;
    FLIGHT_COMMAND *control;
    FLIGHT_INPUT *input;
    FLIGHT_CONTROL *smooth;
    MATRIX matrix;
    ACTOR_RELATIVE_POSE relative, final_relative;
    uint16 local[3];
    const FLIGHT_RELATIVE_PROFILE *selection;
    const FLIGHT_TARGET_PROFILE *profile;
    const FLIGHT_MODEL_OFFSET *offsets;
    const FLIGHT_AIRCRAFT_PROFILE *settings;
    uint32 rotated[3], mode, value, other, flags, result, saved, option, ax, ay, az, reference, difference, cached, a, b, c, d, e, i;
    FUNCTION_MARKER(0x800729DC, "ACE3.BIN");
    actor_index_objects = 0;
    state = &flight_dynamics;
    settings_root = &flight_status;
    pose_owner_object = mission_actor(actor_index_objects);
    actor_motion_object = mission_actor_motion(actor_index_objects);
    actor_traits_object = mission_actor_traits(actor_index_objects);
    mode = state->mode;
    settings = settings_root->model;
    switch (mode)
    {
        case 1:
        case 4:
            flight_control_update_for_model(settings, pose_owner_object);
            state = &flight_dynamics;
            smooth = &flight_control;
            value = state->speed;
            flight_control_scale_axes((sint32)value, smooth);
            state = &flight_dynamics;
            flags = state->events;
            if ((flags & 15) != 0)
            {
                control = &flight_command;
                value = (sint16)control->roll;
                if ((sint32)value >= 257)
                {
                    control->roll = (uint16)(256);
                    control = &flight_command;
                }
                value = (sint16)control->roll;
                if ((sint32)value < -256)
                    control->roll = (uint16)(0xFF00);
            }
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(mode == 1 ? 0xFFFFFFFF : 0xFFFFFFF7);
            break;
        case 2:
            control = &flight_command;
            control->roll = (uint16)(0);
            control->yaw = (uint16)(0);
            control->pitch = (uint16)(0);
            control->thrust = (uint16)(0);


            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(71);
            break;
        case 8:
            flight_control_update_for_model(settings, pose_owner_object);
            profile = mission_flow.flight_selection.target;
            saved = flight_target_classify_landing(profile, &pose_owner_object->pose);
            state = &flight_dynamics;
            flags = state->events;
            if ((flags & 9) != 0)
            {
                if (saved == 0)
                    actor_traits_object->health = (uint16)(0xFFFF);
                else if (saved == 1)
                {
                    value = state->flags;
                    state->flags = (uint32)(value | 128);
                    sound_play(90);
                    option = mission_flow.flight_selection.mode;
                    if (option != 1)
                        goto collision_transition;
                    sound_play(91);
                }
                else
                {
                    value = state->flags;
                    state->flags = (uint32)(value | 256);
                }
                option = mission_flow.flight_selection.mode;
            collision_transition:
                flight_target_set_primary_mode(option != 0, 2);
                state = &flight_dynamics;
                state->mode_timer = (uint16)(30);
            }
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(saved != 0 ? 0xFFFFFFE7 : 0xFFFFFFF7);
            output = &flight_target;
            output->result = (uint32)(saved);
            break;
        case 16:
            smooth = &flight_control;
            profile = mission_flow.flight_selection.target;
            result = flight_control_update_target(state, smooth, profile, actor_motion_object, pose_owner_object);
            if (result != 0)
            {
                option = mission_flow.flight_selection.mode;
                flight_target_set_primary_mode(option != 0, 2);
            }
            flight_status_apply_actor_events(6);
            output = &flight_target;
            output->result = (uint32)(1);
            break;
        case 32:
            flight_dynamics_update_object_option(state, 0, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(70);
            profile = mission_flow.flight_selection.target;
            result = flight_target_classify_landing(profile, &pose_owner_object->pose);
            state = &flight_dynamics;
            flags = state->events;
            if ((flags & 9) != 0 && result == 0)
            {
                settings_root = &flight_status;
                value = settings_root->events;
                settings_root->events = (uint32)(value | 4);
                value = game_runtime.gameplay_counter;
                if ((value & 7) == 0)
                    sound_play_reuse(39);
                state = &flight_dynamics;
                flight_dynamics_emit_contact(state->contact_point.word, 0, actor_motion_object->velocity, pose_owner_object->pose.position);
                state = &flight_dynamics;
                value = state->mode_timer - 1;
                state->mode_timer = (uint16)(value);
                if ((sint32)(value << 16) <= 0)
                    actor_traits_object->health = (uint16)(0xFFFF);
            }
            break;
        case 64:
            flight_dynamics_update_object_option(state, 1, actor_motion_object, pose_owner_object);
            profile = mission_flow.flight_selection.target;
            result = flight_target_classify_landing(profile, &pose_owner_object->pose);
            state = &flight_dynamics;
            flags = state->events;
            if ((flags & 9) != 0 && result == 0)
                actor_traits_object->health = (uint16)(0xFFFF);
            flight_status_apply_actor_events(6);
            break;
        case 256:
            flight_control_update_for_model(settings, pose_owner_object);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            selection = mission_flow.flight_selection.relative;
            offsets = mission_flow.flight_selection.offsets;
            saved = flight_dynamics_relative_pose(selection, offsets, &relative, pose_owner_object);
            flight_status_apply_actor_events(0xFFFFFFF7);
            state = &flight_dynamics;
            value = state->phase;
            if (value == 0)
            {
                state->phase = (uint32)(1);
                state->ticks = (uint32)(0);
            }
            if (saved != 0)
            {
                state = &flight_dynamics;
                value = state->phase;
                if (value == 1)
                {
                    value = state->ticks;
                    state->ticks = (uint32)(value + 1);
                }
                else
                {
                    state = &flight_dynamics;
                    state->ticks = (uint32)(0);
                }
            }
            else
            {
                state = &flight_dynamics;
                state->ticks = (uint32)(0);
            }
            state = &flight_dynamics;
            value = state->ticks;
            if ((sint32)value > 45)
            {
                value = state->flags;
                option = mission_flow.flight_selection.mode;
                state->flags = (uint32)(value | 128);
                flight_target_set_secondary_mode(option != 2, 2);
            }
            output = &flight_target;
            output->result = (uint32)(saved);
            break;
        case 512:
            smooth = &flight_control;
            selection = mission_flow.flight_selection.relative;
            flight_control_update_selection(state, smooth, selection, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(4);
            option = mission_flow.flight_selection.mode;
            flight_target_set_secondary_mode(option != 2, 2);
            break;
        case 1024:
            selection = mission_flow.flight_selection.relative;
            value = selection->actor_idx;
            offsets = mission_flow.flight_selection.offsets;
            actor_motion_target = mission_actor_motion((actor_index_objects + ((uint32)((sint32)(value * 4) >> 2))));
            flight_dynamics_relative_pose(selection, offsets, &relative, pose_owner_object);
            state = &flight_dynamics;
            saved = flight_dynamics_apply_relative_pose(state, &relative, actor_motion_target->velocity, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(4);
            if (saved != 0)
            {
                option = mission_flow.flight_selection.mode;
                flight_target_set_secondary_mode(option != 2, 3);
            }
            break;
        case 2048:
            input = &flight_input;
            control = &flight_command;
            smooth = &flight_control;
            flight_control_update_phase(state, input, control, smooth, pose_owner_object);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(4);
            output = &flight_target;
            output->result = (uint32)(0);
            break;
        case 4096:
            flight_status_apply_actor_events(4);
            break;
        case 8192:
            profile = mission_flow.flight_selection.target;
            flight_dynamics_apply_target_pose(state, profile, pose_owner_object);
            state = &flight_dynamics;
            smooth = &flight_control;
            value = state->speed;
            flight_control_scale_axes((sint32)value, smooth);
            settings_root = &flight_status;
            state = &flight_dynamics;
            settings_root->ground_clearance = (uint32)(0);
            state->speed = (uint32)(0);
            control = &flight_command;
            control->thrust = (uint16)(0xF000);
            engine_audio_apply();
            break;
        case 0x4000:
        case 0x8000:
            state = &flight_dynamics;
            control = &flight_command;
            input = &flight_input;
            smooth = &flight_control;
            input->axis[3] = (uint16)(4096);
            value = state->mode;
            profile = mission_flow.flight_selection.target;
            saved = value == 0x8000;
            if (flight_control_update_target_phase(state, control, smooth, profile, saved, actor_motion_object, pose_owner_object) == 0)
            {
                engine_audio_apply();
                break;
            }
            flight_control_update_for_model(settings, pose_owner_object);
            state = &flight_dynamics;
            control = &flight_command;
            value = state->speed;
            flight_control_scale_command(value, control);
            state = &flight_dynamics;
            smooth = &flight_control;
            value = state->speed;
            flight_control_scale_axes((sint32)value, smooth);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(0xFFFFFFD7);
            profile = mission_flow.flight_selection.target;
            if (flight_target_outside_bounds(profile, pose_owner_object->pose.position) != 0)
                flight_target_set_tertiary_mode(saved, 2);
            break;
        case 0x10000:
            control = &flight_command;
            smooth = &flight_control;
            profile = mission_flow.flight_selection.target;
            flight_control_update_target_option(state, control, smooth, profile, 0, actor_motion_object, pose_owner_object);
            flight_target_set_tertiary_mode(0, 2);
            break;
        case 0x100000:
            flight_dynamics_update_object_motion(state, actor_traits_object, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(6);
            break;
        case 0x200000:
        case 0x400000:
            input = &flight_input;
            control = &flight_command;
            smooth = &flight_control;
            flight_control_update_object_input(input, control, smooth, mode == 0x200000, pose_owner_object);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_primary(control, settings, state, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(71);
            break;
        case 0x1000000:
        case 0x2000000:
        case 0x4000000:
            input = &flight_input;
            control = &flight_command;
            smooth = &flight_control;
            flight_control_update_input(&game_input, input, control, smooth);
            state = &flight_dynamics;
            smooth = &flight_control;
            value = state->speed;
            flight_control_scale_axes((sint32)value, smooth);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_secondary(control, settings, state, actor_motion_object, pose_owner_object);
            if (mode == 0x4000000)
            {
                state = &flight_dynamics;
                flight_dynamics_update_object_sequence(state, actor_traits_object, actor_motion_object, pose_owner_object);
            }
            flight_status_apply_actor_events(mode == 0x1000000 ? 0xFFFFFFAD : 0xFFFFFFA5);
            if (mode == 0x4000000)
            {
                profile = mission_flow.flight_selection.target;
                result = flight_pose_is_level_above_ground(&pose_owner_object->pose);
                state = &flight_dynamics;
                value = state->flags;
                if (value == 16 && result != 0)
                {
                    value = state->target_ticks + 2;
                    state->target_ticks = (uint16)(value);
                    if ((sint16)value >= 61)
                    {
                        flight_dynamics_set_sequence_mode(1);
                        state = &flight_dynamics;
                        value = state->flags;
                        state->flags = (uint32)(value | 0x280);
                    }
                }
            }
            break;
        case 0x8000000:
            value = (sint16)pose_owner_object->pose.angles[2];
            other = pose_owner_object->pose.angles[2];
            pose_owner_object->pose.angles[1] = (uint16)(0);
            if ((sint32)value < 4)
                pose_owner_object->pose.angles[2] = (uint16)(other + 4);
            value = (sint16)pose_owner_object->pose.angles[2];
            other = pose_owner_object->pose.angles[2];
            if ((sint32)value >= -3)
                pose_owner_object->pose.angles[2] = (uint16)(other - 4);
            value = game_runtime.display_buffer;
            option = value != 0;
            if (!option)
            {
                value = (sint16)pose_owner_object->pose.angles[2];
                if ((sint32)value < 0)
                    value = 0 - value;
                option = (sint32)value >= 547;
            }
            if (option)
            {
                input = &flight_input;
                control = &flight_command;
                smooth = &flight_control;
                flight_control_update_object_targets(input, control, smooth, pose_owner_object);
            }
            else
            {
                control = &flight_command;
                input = &flight_input;
                control->roll = (uint16)(0);
                control->yaw = (uint16)(0);
                control->pitch = (uint16)(0);
                control->thrust = (uint16)(0);


                input->axis[0] = (uint16)(0);
                input->axis[1] = (uint16)(0);
                input->axis[2] = (uint16)(0);
                input->axis[3] = (uint16)(0);
            }
            state = &flight_dynamics;
            smooth = &flight_control;
            value = state->speed;
            flight_control_scale_axes((sint32)value, smooth);
            control = &flight_command;
            state = &flight_dynamics;
            flight_physics_integrate_secondary(control, settings, state, actor_motion_object, pose_owner_object);
            state = &flight_dynamics;
            flight_dynamics_update_object_sequence(state, actor_traits_object, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(0xFFFFFFA5);
            break;
        case 0x10000000:
            control = &flight_command;
            control->roll = (uint16)(0);
            control->yaw = (uint16)(0);
            control->pitch = (uint16)(0);
            control->thrust = (uint16)(0);


            flight_physics_integrate_secondary(control, settings, state, actor_motion_object, pose_owner_object);
            state = &flight_dynamics;
            flight_dynamics_update_object_sequence(state, actor_traits_object, actor_motion_object, pose_owner_object);
            flight_status_apply_actor_events(0xFFFFFFA5);
            break;
        case 0x20000000:
            engine_audio_apply();
            break;
        default:
            break;
    }
    state = &flight_dynamics;
    flags = state->mode;
    if ((flags & 24) != 0)
    {
        profile = mission_flow.flight_selection.target;
        rotation_build(&matrix, profile->angles);
        for (i = 0; i < 3; ++i)
        {
            value = pose_owner_object->pose.position[i];
            other = profile->position[i];
            local[i] = ((sint32)(value - other) >> 12);
        }
        a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
        b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
        c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
        d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
        e = ((uint16)matrix.m[2][2]);
        SetRotMatrix(&matrix);
        xport_gte_write_data(0, local[0] | ((uint32)local[1] << 16));
        xport_gte_write_data(1, local[2]);
        xport_gte_execute(0x486012);
        rotated[0] = xport_gte_read_data(25);
        rotated[1] = xport_gte_read_data(26);
        rotated[2] = xport_gte_read_data(27);
        output = &flight_target;
        output->displacement.word[0] = (uint32)(rotated[0] << 12);
        output->displacement.word[1] = (uint32)(rotated[1] << 12);
        state = &flight_dynamics;
        output->displacement.word[2] = (uint32)(rotated[2] << 12);
        ax = (sint16)flight_orientation.velocity_angles.value[0];
        ay = (sint16)flight_orientation.velocity_angles.value[1];
        reference = (sint16)profile->angles[0];
        az = (sint16)flight_orientation.velocity_angles.value[2];
        cached = profile->angles[0];
        difference = reference - ax;
        if ((sint32)difference < 8192)
        {
            ax -= 16384;
            difference = reference - ax;
        }
        if ((sint32)difference > 8192)
            ax += 16384;
        reference = (sint16)profile->angles[1];
        difference = reference - ay;
        if ((sint32)difference < 8192)
        {
            ay -= 16384;
            difference = reference - ay;
        }
        if ((sint32)difference > 8192)
            ay += 16384;
        reference = (sint16)profile->angles[2];
        difference = reference - az;
        if ((sint32)difference < 8192)
        {
            az -= 16384;
            difference = reference - az;
        }
        if ((sint32)difference > 8192)
            az += 16384;
        output->angle_delta.value[0] = (uint16)(ax - cached);
        value = profile->angles[1];
        output->angle_delta.value[1] = (uint16)(ay - value);
        value = profile->angles[2];
        profile = mission_flow.flight_selection.target;
        output->angle_delta.value[2] = (uint16)(az - value);
        result = game_measure_distance_gte(profile->position, pose_owner_object->pose.position);
        output = &flight_target;
        output->distance = (uint32)(result);
    }
    state = &flight_dynamics;
    flags = state->mode;
    if ((flags & 0x1F00) != 0)
    {
        selection = mission_flow.flight_selection.relative;
        offsets = mission_flow.flight_selection.offsets;
        value = selection->actor_idx;
        actor_index_objects = 0;
        pose_owner_target = mission_actor((actor_index_objects + ((uint32)((sint32)(value * 4) >> 2))));
        actor_motion_target = mission_actor_motion((actor_index_objects + ((uint32)((sint32)(value * 4) >> 2))));
        result = flight_dynamics_relative_pose(selection, offsets, &final_relative, pose_owner_object);
        output = &flight_target;
        value = pose_owner_target->pose.angles[0];
        output->reference_angles.value[0] = (uint16)(value);
        value = pose_owner_target->pose.angles[1];
        output->reference_angles.value[1] = (uint16)(value);
        value = pose_owner_target->pose.angles[2];
        output->reference_angles.value[2] = (uint16)(value);
        output->reference_position.word[0] = (uint32)((final_relative.reference_position.word[0]));
        output->reference_position.word[1] = (uint32)((final_relative.reference_position.word[1]));
        output->reference_position.word[2] = (uint32)((final_relative.reference_position.word[2]));
        output->displacement.word[0] = (uint32)((final_relative.displacement.word[0]));
        output->displacement.word[1] = (uint32)((final_relative.displacement.word[1]));
        output->displacement.word[2] = (uint32)((final_relative.displacement.word[2]));
        output->angle_delta.value[0] = (uint16)(((uint16)final_relative.angle_delta.pitch));
        output->angle_delta.value[1] = (uint16)(((uint16)final_relative.angle_delta.yaw));
        output->angle_delta.value[2] = (uint16)(((uint16)final_relative.angle_delta.roll));
        value = (final_relative.distance);
        output->result = (uint32)(result);
        output->distance = (uint32)(value);
    }
    result = (sint16)actor_traits_object->health;
    if ((sint32)result > 0)
        { (void)(uint32)(result); return; }
    state = &flight_dynamics;
    result = state->flags & 0x7000;
    if (result != 0)
        { (void)(uint32)(result); return; }
    value = game_runtime.account.loss_count;
    game_runtime.account.loss_count = (uint16)(value + 1);
    flight_enter_engine_off_mode();
    flags = game_runtime.mission_control.flags;
    option = 2;
    if ((flags & 16) == 0)
    {
        state = &flight_dynamics;
        flags = state->events;
        option = (flags & 0x2F) != 0;
    }
    { (void)(camera_begin_player_loss_view(option)); return; }
}

uint32 flight_dynamics_flags(void)
{
    FUNCTION_MARKER(0x8006CB30, "ACE3.BIN");
    return flight_dynamics.flags;
}

void flight_dynamics_emit_contact(const uint32 second[3], uint32 kind, const uint32 *native_vector, const uint32 *native_first)
{
    uint32 x, y, z, a, b, c, position[3];
    FUNCTION_MARKER(0x80082BAC, "ACE3.BIN");
    x = (((native_first[0])));
    a = second[0];
    y = (((native_first[1])));
    z = (((native_first[2])));
    position[0] = x + (uint32)((sint32)(a - x) >> 2);
    b = second[1];
    position[1] = y + (uint32)((sint32)(b - y) >> 2);
    c = second[2];
    position[2] = z + (uint32)((sint32)(c - z) >> 2);
    { (void)(effect_emit_contact(kind, position, native_vector)); return; }
}

void flight_dynamics_probe_collision(FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, const MISSION_ACTOR *native_object)
{
    MISSION_ACTOR *target;
    MISSION_ACTOR_TRAITS *actor_traits_target;

    uint32 vx, vy, vz, x, y, z, a, b, c, speed, square, threshold;
    uint32 index, flags, radius, dx, dy, dz, sum, result;
    FUNCTION_MARKER(0x80070A80, "ACE3.BIN");
    vx = actor_motion->velocity[0];
    vy = actor_motion->velocity[1];
    vz = actor_motion->velocity[2];
    a = (sint32)vx < 0 ? 0 - vx : vx;
    b = (sint32)vy < 0 ? 0 - vy : vy;
    c = (sint32)vz < 0 ? 0 - vz : vz;
    speed = ((sint32)(a + b + c) >> 12);
    square = speed * speed;
    threshold = ((sint64)(sint32)square * 0x38E38E39LL >> 32);
    threshold = (uint32)((sint32)threshold >> 1) - (uint32)((sint32)square >> 31);
    y = ((sint32)(native_object->pose.position[1] + vy) >> 12);
    x = ((sint32)(native_object->pose.position[0] + vx) >> 12);
    z = ((sint32)(native_object->pose.position[2] + vz) >> 12);
    for (index = 0; index < 64; ++index)
    {
        target = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
        actor_traits_target = &target->traits;
        flags = actor_traits_target->flags;
        if ((flags & 0x4400) != 0x4400)
            continue;
        radius = (sint16)actor_traits_target->radius[1];
        a = target->pose.position[0];
        b = target->pose.position[1];
        dx = (uint32)((sint32)a >> 12) - x;
        dy = (uint32)((sint32)b >> 12) - y;
        a = (sint32)dx < 0 ? 0 - dx : dx;
        dz = (uint32)((sint32)target->pose.position[2] >> 12) - z;
        b = (sint32)dy < 0 ? 0 - dy : dy;
        c = (sint32)dz < 0 ? 0 - dz : dz;
        if ((sint32)(radius * 3 + speed) < (sint32)(a + b + c))
            continue;
        sum = dx * dx + dy * dy + dz * dz;
        if ((sint32)(radius * radius + threshold) < (sint32)sum)
            continue;
        if ((flags & 0x1020) == 0x1020 && (~actor_traits_target->render_flags & 2) != 0)
        {
            result = collision_trace_actor(target, state->contact_point.word, native_object->pose.position, actor_motion->velocity);
            if ((sint32)result > 0x7FFFFFFE)
                continue;
            a = state->events;
            b = game_runtime.collision_result.flags;
            state->contact_material = (uint32)(b);
            state->events = (uint32)(a | 0x20);
        }
        else
        {
            a = state->events;
            state->events = (uint32)(a | 0x20);
            a = native_object->pose.position[0];
            state->contact_point.word[0] = (uint32)(a);
            a = native_object->pose.position[1];
            state->contact_point.word[1] = (uint32)(a);
            a = native_object->pose.position[2];
            state->contact_material = (uint32)(0);
            state->contact_point.word[2] = (uint32)(a);
        }
    }
    return;
}

void flight_dynamics_resolve_collision(FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 x, y, z, a, b, side = 0, kind = 0, value, result;
    const uint16 *map;
    uint32 first[3], second[3], tile, px, pz, nx, nz, from, to, altitude;
    FUNCTION_MARKER(0x80070CF4, "ACE3.BIN");
    a = native_object->pose.position[0];
    b = native_object->pose.position[1];
    x = actor_motion->velocity[0] + a;
    y = actor_motion->velocity[1] + b;
    a = native_object->pose.position[2];
    z = actor_motion->velocity[2] + a;
    a = game_runtime.mission_bounds.min[0] + 132;
    if ((sint32)x < (sint32)a)
    {
        kind = 4;
        side = 1;
        x = a;
    }
    a = game_runtime.mission_bounds.max[0] - 132;
    if ((sint32)a < (sint32)x)
    {
        kind = 4;
        side = 2;
        x = a;
    }
    a = game_runtime.mission_bounds.min[1] + 132;
    if ((sint32)y < (sint32)a)
    {
        kind = 3;
        side = 3;
        y = a;
    }
    a = game_runtime.mission_bounds.max[1] - 132;
    if ((sint32)a < (sint32)y)
    {
        kind = 1;
        side = 4;
        y = a;
    }
    a = game_runtime.mission_bounds.min[2] + 132;
    if ((sint32)z < (sint32)a)
    {
        kind = 4;
        side = 5;
        z = a;
    }
    a = game_runtime.mission_bounds.max[2] - 132;
    if ((sint32)a < (sint32)z)
    {
        kind = 4;
        side = 6;
        z = a;
    }
    if (side != 0)
    {
        native_object->pose.position[0] = (uint32)(x);
        native_object->pose.position[1] = (uint32)(y);
        native_object->pose.position[2] = (uint32)(z);
        value = scene_bounds_normals[side - 1][0];
        state->contact_normal[0] = (uint16)(value);
        value = scene_bounds_normals[side - 1][1];
        state->contact_normal[1] = (uint16)(value);
        a = scene_bounds_normals[side - 1][2];
        value = state->events;
        game_runtime.collision_result.flags = (uint32)(kind);
        state->contact_material = (uint32)(kind);
        state->events = (uint32)(value | 2);
        state->contact_normal[2] = (uint16)(a);
    }
    result = (sint32)native_object->pose.position[1] < -12288000;
    if (result != 0)
        { (void)(uint32)(result); return; }
    a = native_object->pose.position[0];
    b = actor_motion->velocity[0];
    map = game_runtime.scene_assets.tile_map;
    first[0] = a + b;
    a = native_object->pose.position[1];
    b = actor_motion->velocity[1];
    first[1] = a + b;
    a = native_object->pose.position[2];
    b = actor_motion->velocity[2];
    first[2] = a + b;
    nx = (uint32)((sint32)first[0] >> 23) + 128;
    nz = (uint32)((sint32)first[2] >> 23) + 128;
    a = native_object->pose.position[0];
    b = native_object->pose.position[2];
    px = (uint32)((sint32)a >> 23) + 128;
    pz = (uint32)((sint32)b >> 23) + 128;
    if ((sint32)px < 0)
        px &= 7;
    if ((sint32)pz < 0)
        pz &= 7;
    if ((sint32)nx < 0)
        nx &= 7;
    if ((sint32)nz < 0)
        nz &= 7;
    if ((sint32)px >= 256)
        px = (px & 7) + 248;
    if ((sint32)pz >= 256)
        pz = (pz & 7) + 248;
    if ((sint32)nx >= 256)
        nx = (nx & 7) + 248;
    if ((sint32)nz >= 256)
        nz = (nz & 7) + 248;
    from = px + (pz << 8);
    to = nx + (nz << 8);
    tile = map[from] & 0x1FF;
    if (tile == 0)
        { (void)(uint32)(tile); return; }
    b = native_object->pose.position[1];
    const SCENE_TERRAIN_MODEL_HEADER *header = terrain_model_header(&game_runtime.scene_assets.terrain_models, tile);
    a = header->height;
    altitude = ((sint32)first[1] >> 12);
    if ((sint32)((uint32)((sint32)b >> 12) + 132) < (sint32)a)
    {
        result = (sint32)(altitude + 132) < (sint32)a;
        if (result != 0 && from == to)
            { (void)(uint32)(result); return; }
    }
    if (from == to)
        result = collision_query_tile(native_object->pose.position, first, state->contact_point.word);
    else
        result = collision_query_segment(native_object->pose.position, first, state->contact_point.word);
    if ((sint32)result <= 0x7FFFFFFE)
    {
        kind = game_runtime.collision_result.flags;
        value = game_runtime.collision_result.normal[0];
        state->contact_normal[0] = (uint16)(value);
        value = game_runtime.collision_result.normal[1];
        state->contact_normal[1] = (uint16)(value);
        a = game_runtime.collision_result.normal[2];
        value = state->events;
        state->contact_material = (uint32)(kind);
        state->events = (uint32)(value | 1);
        state->contact_normal[2] = (uint16)(a);
    }
    if (header->height < (sint32)(altitude + 99))
    {
        first[0] = native_object->pose.position[0];
        a = native_object->pose.position[1];
        b = actor_motion->velocity[1];
        first[1] = a + b + 0x44000;
        first[2] = native_object->pose.position[2];
        second[0] = native_object->pose.position[0];
        a = native_object->pose.position[1];
        b = actor_motion->velocity[1];
        second[1] = a + b - 0x44000;
        second[2] = native_object->pose.position[2];
        if ((sint32)actor_motion->velocity[1] > 0)
            result = collision_query_tile(second, first, state->contact_point.word);
        else
            result = collision_query_tile(first, second, state->contact_point.word);
        if ((sint32)result <= 0x7FFFFFFE)
        {
            kind = game_runtime.collision_result.flags;
            value = game_runtime.collision_result.normal[0];
            state->contact_normal[0] = (uint16)(value);
            value = game_runtime.collision_result.normal[1];
            state->contact_normal[1] = (uint16)(value);
            a = game_runtime.collision_result.normal[2];
            value = state->events;
            state->contact_material = (uint32)(kind);
            state->events = (uint32)(value | 2);
            state->contact_normal[2] = (uint16)(a);
            a = actor_motion->velocity[1];
            value = state->contact_point.word[1];
            value += (sint32)a > 0 ? 0xFFFBE000 : 0x42000;
            native_object->pose.position[1] = (uint32)(value);
        }
    }
    a = (sint16)header->reserved;
    result = a - 60;
    if ((sint32)a < (sint32)(altitude - 60))
    {
        native_object->pose.position[1] = (uint32)(result << 12);
        value = state->events;
        state->contact_normal[0] = (uint16)(0);
        state->contact_normal[1] = (uint16)(0xF000);
        state->contact_normal[2] = (uint16)(0);
        state->events = (uint32)(value | 4);
        value = native_object->pose.position[0];
        state->contact_point.word[0] = (uint32)(value);
        value = native_object->pose.position[1];
        state->contact_point.word[1] = (uint32)(value);
        result = native_object->pose.position[2];
        state->contact_material = (uint32)(0);
        state->contact_point.word[2] = (uint32)(result);
    }
    { (void)(uint32)(result); return; }
}

void flight_dynamics_apply_target_pose(FLIGHT_DYNAMICS *state, const FLIGHT_TARGET_PROFILE *target, MISSION_ACTOR *native_object)
{
    uint32 start[3], end[3];
    uint32 value, result, normal, height, x, z, pitch, yaw;
    FUNCTION_MARKER(0x8007599C, "ACE3.BIN");
    state->speed = (uint32)(0);
    start[0] = target->position[0];
    value = target->position[1];
    start[1] = value + 0xFFF9C000;
    start[2] = target->position[2];
    end[0] = target->position[0];
    value = target->position[1];
    end[1] = value + 0x64000;
    end[2] = target->position[2];
    result = collision_query_tile(start, end, state->contact_point.word);
    if ((sint32)result <= 0x7FFFFFFE)
    {
        normal = game_runtime.collision_result.normal[0];
        state->contact_normal[0] = (uint16)(normal);
        normal = game_runtime.collision_result.normal[1];
        state->contact_normal[1] = (uint16)(normal);
        value = game_runtime.collision_result.flags;
        normal = game_runtime.collision_result.normal[2];
        state->contact_material = (uint32)(value);
        state->contact_normal[2] = (uint16)(normal);
    }
    else
    {
        value = target->position[0];
        state->contact_point.word[0] = (uint32)(value);
        value = target->position[1];
        state->contact_point.word[1] = (uint32)(value);
        value = target->position[2];
        state->contact_point.word[2] = (uint32)(value);
    }
    height = state->contact_point.word[1];
    native_object->pose.position[1] = (uint32)(height + 0xFFFBE000);
    x = state->contact_point.word[0];
    native_object->pose.position[0] = (uint32)(x);
    z = state->contact_point.word[2];
    native_object->pose.angles[0] = (uint16)(0);
    pitch = native_object->pose.angles[0];
    native_object->pose.position[2] = (uint32)(z);
    yaw = target->angles[1];
    native_object->pose.angles[2] = (uint16)(0);
    native_object->pose.angles[1] = (uint16)(yaw);
    flight_orientation.velocity_angles.value[0] = (uint16)(pitch);
    yaw = native_object->pose.angles[1];
    flight_orientation.velocity_angles.value[1] = (uint16)(yaw);
    value = native_object->pose.angles[2];
    flight_orientation.velocity_angles.value[2] = (uint16)(value);
    { (void)(flight_orientation_update_delta(&flight_orientation, native_object)); return; }
}

void flight_dynamics_update_object_motion(FLIGHT_DYNAMICS *state, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 phase, flags, value, raw, magnitude, ticks, velocity, pitch, product, step, roll, result;
    FUNCTION_MARKER(0x80074560, "ACE3.BIN");
    phase = state->phase;
    if (phase == 0)
    {
        state->ticks = (uint32)(0);
        flags = game_runtime.frame_counter;
        state->phase = (uint32)((flags & 4) != 0 ? 1 : 2);
        value = (sint16)flight_orientation.response[2];
        raw = flight_orientation.response[2];
        if (value == 0)
        {
            flags = (uint16)game_runtime.frame_counter;
            flight_orientation.response[2] = (uint16)(raw - 64 + (flags & 127));
        }
        else
        {
            magnitude = (sint32)value < 0 ? 0 - value : value;
            if ((sint32)magnitude < 32)
                flight_orientation.response[2] = (uint16)(raw << 2);
        }
        effect_emit_vector_type3(actor_motion->velocity, native_object->pose.position);
        pad_rumble_add_burst();
        sound_play(32);
    }
    else if (phase == 1 || phase == 2)
    {
        flags = state->events;
        if ((flags & 15) == 0)
        {
            ticks = state->ticks;
            velocity = actor_motion->velocity[1];
            actor_motion->velocity[1] = (uint32)(velocity + (uint32)((sint32)ticks >> 5) * 669);
        }
        pitch = (sint16)native_object->pose.angles[0];
        raw = native_object->pose.angles[0];
        product = (0 - 4096 - pitch) * (phase == 1 ? 30 : 15);
        step = (sint32)product / 4096;
        native_object->pose.angles[0] = (uint16)(raw + step);
        if (phase == 1)
        {
            roll = (sint16)flight_orientation.response[2];
            roll += (uint32)((sint32)roll / 8);
            if ((sint32)roll >= 513)
                roll = 512;
            if ((sint32)roll < -512)
                roll = 0 - 512;
            flight_orientation.response[2] = (uint16)(roll);
            raw = native_object->pose.angles[2];
            native_object->pose.angles[2] = (uint16)(raw + roll);
        }
        else
        {
            ticks = state->ticks;
            value = (uint16)game_wave_at(((ticks << 9) & 0x7E00));
            raw = native_object->pose.angles[2];
            value = (sint32)(value << 16) >> 22;
            native_object->pose.angles[2] = (uint16)(raw + value);
        }
    }
    else if (phase == 16)
    {
        ticks = state->ticks;
        effect_emit_position_type2(ticks, native_object->pose.position);
        flags = actor_traits->flags;
        actor_traits->flags = (uint32)(flags & 0xFFFEFFFF);
    }
    ticks = state->ticks;
    phase = state->phase;
    ticks += 1;
    result = phase & 3;
    state->ticks = (uint32)(ticks);
    if (result == 0)
        { (void)(uint32)(result); return; }
    effect_emit_trail_periodic(ticks, actor_motion->velocity, native_object->pose.position);
    pad_rumble_add_random_large();
    flags = state->events;
    result = flags & 15;
    if (result == 0)
        { (void)(uint32)(result); return; }
    state->ticks = (uint32)(0);
    state->phase = (uint32)(16);
    effect_emit_vector_type3(actor_motion->velocity, native_object->pose.position);
    pad_rumble_add_sustained();
    result = sound_play(34);
    actor_motion->velocity[0] = (uint32)(0);
    actor_motion->velocity[1] = (uint32)(0);
    actor_motion->velocity[2] = (uint32)(0);
    { (void)(uint32)(result); return; }
}

void flight_dynamics_update_object_option(FLIGHT_DYNAMICS *state, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 vx, vy, vz, factor, product, flags, limit, roll, magnitude, damping, phase, pitch, ticks, step, raw, amplitude, sine;
    FUNCTION_MARKER(0x80074878, "ACE3.BIN");
    vx = actor_motion->velocity[0];
    vy = actor_motion->velocity[1];
    vz = actor_motion->velocity[2];
    factor = option != 0 ? 3936 : 4084;
    product = vx * factor;
    vx = (sint32)product / 4096;
    product = vz * factor;
    vz = (sint32)product / 4096;
    flags = state->events;
    if ((flags & 15) == 0)
    {
        vy += 669;
        goto apply_velocity;
    }
    limit = (sint32)vy < 1025 ? vy : 1024;
    if ((sint32)limit < 0)
        limit = 0;
    roll = (sint16)native_object->pose.angles[2];
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    damping = (sint32)magnitude < 1536 ? (limit >> 3) + 3776 : 3840;
    product = roll * damping;
    native_object->pose.angles[2] = (uint16)((sint32)product / 4096);
    phase = state->phase;
    pitch = (sint16)native_object->pose.angles[0];
    if (phase == 0)
    {
        if ((sint32)pitch < 0)
            state->phase = (uint32)(1);
        else if ((sint32)pitch < 182)
        {
            state->phase = (uint32)(3);
            state->ticks = (uint32)((pitch << 10) / 182 + 4096);
        }
        else
            state->phase = (uint32)(2);
    }
    phase = state->phase;
    if (phase == 1)
    {
        step = (sint32)(182 - pitch) / 2;
        if ((sint32)step < 2)
            step = 2;
        raw = native_object->pose.angles[0];
        native_object->pose.angles[0] = (uint16)(raw + step);
        if ((sint32)pitch >= 182)
        {
            native_object->pose.angles[0] = (uint16)(182);
            state->ticks = (uint32)(5120);
            state->phase = (uint32)(3);
        }
    }
    else if (phase == 2)
    {
        product = (pitch - 182) * (limit << 1);
        step = (sint32)product / 4096;
        if ((sint32)step < 8)
            step = 8;
        raw = native_object->pose.angles[0];
        native_object->pose.angles[0] = (uint16)(raw - step);
        if ((sint32)pitch < 183)
        {
            native_object->pose.angles[0] = (uint16)(182);
            state->ticks = (uint32)(5120);
            state->phase = (uint32)(3);
        }
    }
    else if (phase == 3)
    {
        ticks = state->ticks;
        amplitude = 4096;
        if ((sint32)ticks < 4097)
            amplitude = (sint32)ticks < 0 ? 0 : ticks;
        ticks = state->ticks;
        sine = rsin(ticks);
        product = sine * 182;
        product = (sint32)product / 4096;
        product *= amplitude;
        pitch = (sint32)product / 4096;
        native_object->pose.angles[0] = (uint16)(pitch);
        ticks = state->ticks;
        ticks -= option != 0 ? 128 : 64;
        state->ticks = (uint32)(ticks);
        ticks = state->ticks;
        if ((sint32)ticks < -512)
        {
            camera_set_cinematic_mode(1);
            state->phase = (uint32)(4);
        }
    }
apply_velocity:
    actor_motion->velocity[0] = (uint32)(vx);
    actor_motion->velocity[1] = (uint32)(vy);
    actor_motion->velocity[2] = (uint32)(vz);
    { (void)(flight_orientation_update_delta(&flight_orientation, native_object)); return; }
}

uint32 flight_dynamics_apply_relative_pose(FLIGHT_DYNAMICS *state, const ACTOR_RELATIVE_POSE *relative, const uint32 *target_velocity, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *actor)
{
    uint32 direction[3], angles[3], step[3], value, other, mode, i, difference, magnitude, total;
    FUNCTION_MARKER(0x80075084, "ACE3.BIN");
    value = (relative->displacement.word[0]);
    other = (relative->displacement.word[2]);
    direction[0] = ((sint32)value >> 5);
    value = (relative->displacement.word[1]);
    direction[1] = ((sint32)value >> 5);
    direction[2] = ((sint32)other >> 5);
    for (i = 0; i < 3; ++i)
    {
        if ((sint32)direction[i] >= 4097)
            direction[i] = 4096;
        if ((sint32)direction[i] < -4096)
            direction[i] = 0xFFFFF000;
    }
    angles[0] = (sint16)((uint16)relative->angle_delta.pitch);
    angles[1] = (sint16)((uint16)relative->angle_delta.yaw);
    angles[2] = (sint16)((uint16)relative->angle_delta.roll);
    for (i = 0; i < 3; ++i)
        step[i] = ((sint32)(0 - angles[i]) >> 6);
    if (step[0] == 0)
    {
        if ((sint32)angles[0] > 0)
            step[0] = 0xFFFFFFFF;
        if (step[0] == 0 && (sint32)angles[0] < 0)
            step[0] = 1;
    }
    for (i = 1; i < 3; ++i)
        if (step[i] == 0)
        {
            value = (sint16)(relative->angle_delta.value[i]);
            if ((sint32)value > 0)
                step[i] = 0xFFFFFFFF;
            if (step[i] == 0 && (sint32)value < 0)
                step[i] = 1;
        }
    value = actor->pose.angles[0];
    other = actor->pose.angles[2];
    actor->pose.angles[0] = (uint16)(value + step[0]);
    value = actor->pose.angles[1];
    actor->pose.angles[2] = (uint16)(other + step[2]);
    actor->pose.angles[1] = (uint16)(value + step[1]);
    mode = state->phase;
    if (mode == 0)
    {
        for (i = 0; i < 3; ++i)
        {
            value = actor_motion->velocity[i];
            other = target_velocity[i];
            if ((sint32)value < (sint32)other)
            {
                actor_motion->velocity[i] = (uint32)(value + 256);
                value = actor_motion->velocity[i];
            }
            other = target_velocity[i];
            if ((sint32)other < (sint32)value)
                actor_motion->velocity[i] = (uint32)(value - 256);
        }
        for (i = 0; i < 3; ++i)
        {
            value = actor_motion->velocity[i];
            other = target_velocity[i];
            difference = value - other;
            if ((sint32)difference < 0)
                difference = other - value;
            if ((sint32)difference >= 256)
                goto finish;
        }
        state->phase = (uint32)(1);
        state->ticks = (uint32)(0);
    }
    else if (mode == 1)
    {
        value = target_velocity[0];
        actor_motion->velocity[0] = (uint32)(value - direction[0]);
        value = target_velocity[1];
        actor_motion->velocity[1] = (uint32)(value - direction[1]);
        value = target_velocity[2];
        actor_motion->velocity[2] = (uint32)(value - direction[2]);
        total = (sint32)direction[1] < 0 ? 0 - direction[1] : direction[1];
        magnitude = (sint32)direction[0] < 0 ? 0 - direction[0] : direction[0];
        total += magnitude;
        magnitude = (sint32)direction[2] < 0 ? 0 - direction[2] : direction[2];
        total += magnitude;
        if ((sint32)total < 12288)
        {
            value = state->ticks;
            if (value == 150)
            {
                other = mission_flow.flight_selection.mode;
                sound_play(other == 2 ? 94 : 95);
            }
            value = state->ticks + 1;
            state->ticks = (uint32)(value);
            if ((sint32)value >= 301)
                return 1;
        }
    }
finish:
    flight_orientation_update_delta(&flight_orientation, actor);
    return 0;
}

void flight_dynamics_update_object_sequence(FLIGHT_DYNAMICS *state, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 phase, mode, ticks, numerator, increment, multiplier, y_velocity, z_velocity, y, value, first, second, third, vx, vy, vz, speed, count;
    FUNCTION_MARKER(0x8007601C, "ACE3.BIN");
    phase = state->phase;
    if (phase == 0)
    {
        mode = state->mode;
        if (mode != 0x04000000)
            return;
        state->phase = (uint32)(1);
        state->ticks = (uint32)(0);
        state->mode_timer = (uint16)(0);
        state->target_ticks = (uint16)(0);
        state->flags = (uint32)(16);
        return;
    }
    if (phase != 1)
        return;
    ticks = state->ticks;
    if ((sint32)ticks >= 301)
        ticks = 300;
    numerator = ticks * 5120;
    increment = (uint32)((sint32)numerator / 300) << 6;
    y_velocity = actor_motion->velocity[1];
    multiplier = ticks - 300;
    y_velocity *= multiplier;
    z_velocity = actor_motion->velocity[2];
    z_velocity *= multiplier;
    y = native_object->pose.position[1];
    actor_motion->velocity[1] = (uint32)((sint32)y_velocity / 300);
    actor_motion->velocity[2] = (uint32)((sint32)z_velocity / 300);
    if ((sint32)y <= 0x0EA5FFFF)
    {
        native_object->pose.position[1] = (uint32)(y + (uint32)((sint32)increment >> 1));
        value = native_object->pose.position[2];
        native_object->pose.position[2] = (uint32)(value + (increment << 1));
    }
    else
        native_object->pose.position[1] = (uint32)(0x0EA60000);
    first = actor_motion->velocity[0];
    second = actor_motion->velocity[1];
    third = actor_motion->velocity[2];
    actor_motion->velocity[1] = (uint32)(increment);
    flight_orientation_update_delta(&flight_orientation, native_object);
    vx = actor_motion->velocity[0];
    vy = actor_motion->velocity[1];
    vz = actor_motion->velocity[2];
    speed = game_vector_length_gte((sint32)(0 - vx) >> 6, (sint32)(0 - vy) >> 6, (sint32)(0 - vz) >> 6) << 6;
    state->speed = (uint32)(speed);
    actor_motion->velocity[0] = (uint32)(first);
    actor_motion->velocity[1] = (uint32)(second);
    actor_motion->velocity[2] = (uint32)(third);
    ticks = state->ticks;
    state->ticks = (uint32)(ticks + 1);
    y = native_object->pose.position[1];
    if ((sint32)y <= 0)
        { (void)(uint32)(y); return; }
    count = state->mode_timer + 1;
    state->mode_timer = (uint16)(count);
    if ((sint16)count >= 301)
    {
        mode = state->mode;
        if (mode == 0x04000000)
        {
            actor_traits->health = (uint16)(0xFFFF);
            return;
        }
    }
    count = (sint16)state->mode_timer;
    if ((sint32)count < 31)
        return;
    mode = state->mode;
    if (mode != 0x08000000)
        return;
    value = state->flags | 0x200;
    state->flags = (uint32)(value);
    { (void)(uint32)(value); return; }
}

void flight_dynamics_select_sequence(uint32 flags)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 type, index, mode;
    FUNCTION_MARKER(0x80073928, "ACE3.BIN");
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits(actor_index_table);
    type = (sint16)actor_traits_object->kind;
    index = flags & 255;
    if ((flags & 0x100) != 0)
    {
        mission_flow.flight_selection.mode = (uint32)(0);
        mission_flow.flight_selection.target = &flight_target_profiles[index];
        return;
    }
    if ((flags & 0x200) != 0)
        mode = 1;
    else if ((flags & 0x400) != 0 || (flags & 0x800) != 0)
    {
        mode = (flags & 0x400) != 0 ? 2 : 3;
        mission_flow.flight_selection.mode = (uint32)(mode);
        mission_flow.flight_selection.relative = &flight_relative_profiles[index];
        mission_flow.flight_selection.offsets = &flight_model_offsets[type];
        return;
    }
    else if ((flags & 0x1000) != 0)
    {
        mission_flow.flight_selection.mode = (uint32)(4);
        return;
    }
    else if ((flags & 0x2000) != 0)
        mode = 5;
    else if ((flags & 0x4000) != 0)
        mode = 6;
    else
        return;
    mission_flow.flight_selection.mode = (uint32)(mode);
    mission_flow.flight_selection.target = &flight_target_profiles[index];
    return;
}


uint32 flight_pose_is_level_above_ground(const NATIVE_POSE *native_position)
{
    uint32 y, pitch, roll;
    FUNCTION_MARKER(0x80076AC0, "ACE3.BIN");
    y = (((uint32)native_position->position[1]));
    if ((sint32)y < 0)
        return 0;
    pitch = (((sint16)native_position->angles[0]));
    roll = (((sint16)native_position->angles[2]));
    pitch -= 1820;
    if ((sint32)pitch < 0)
        pitch = 0 - pitch;
    if ((sint32)pitch >= 228)
        return 0;
    if ((sint32)roll < 0)
        roll = 0 - roll;
    return (sint32)roll < 456;
}

void flight_dynamics_reset_event_mode(void)
{
    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    FUNCTION_MARKER(0x80076DBC, "ACE3.BIN");
    state = &flight_dynamics;
    context = &flight_status;
    state->events = (uint32)(0);
    context->events = (uint32)(0);
    state->flags = (uint32)(0x400);
    state->mode = (uint32)(0x2000);
    state->phase = (uint32)(0);
    return;
}


void flight_apply_player_damage_tick(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_objects;

    FLIGHT_DYNAMICS *root;
    uint32 result, value;
    FUNCTION_MARKER(0x80076FA4, "ACE3.BIN");
    root = &flight_dynamics;
    result = root->flags & 0x7000;
    if (result != 0)
        { (void)(uint32)(result); return; }
    actor_index_objects = 0;
    actor_traits_object = mission_actor_traits(actor_index_objects);
    value = actor_traits_object->health;
    actor_traits_object->health = (uint16)(value - 1);
    pad_rumble_add_decay_slow();
    { (void)(camera_apply_shake(2)); return; }
}

void flight_enter_engine_off_mode(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 flags;
    FUNCTION_MARKER(0x800763CC, "ACE3.BIN");
    state = &flight_dynamics;
    flags = state->flags;
    state->mode = (uint32)(0x100000);
    state->phase = (uint32)(0);
    state->flags = (uint32)(flags | 0x1000);
    { (void)(engine_audio_stop()); return; }
}
