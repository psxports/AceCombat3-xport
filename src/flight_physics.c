#include "game_functions.h"

static const SVECTOR flight_orientation_axis = {0, 0, 16384, 0};

FLIGHT_ORIENTATION flight_orientation;

void flight_physics_integrate_primary(const FLIGHT_COMMAND *control, const FLIGHT_AIRCRAFT_PROFILE *settings, FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    MATRIX matrix;
    uint16 local_angles[3], turn[3];
    uint32 vector[3], acceleration[3], limits[3], command[3], desired[3], current[3];
    uint32 a, b, c, d, e, value, other, numerator, divisor, ratio, speed, bound0, bound1, bound2;
    uint32 factor, bias, lift, strength, stall, capped_strength, vertical, lateral, magnitude, gravity, pull;
    uint32 coefficient_x, coefficient_y, coefficient_z, flags, gain, weight_x, weight_z, damping, throttle, thrust, i;
    FUNCTION_MARKER(0x8006EF30, "ACE3.BIN");
    acceleration[0] = 0;
    acceleration[1] = 0;
    acceleration[2] = 0;
    game_build_rotation_via_gte(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    value = actor_motion->velocity[0];
    vector[0] = (uint16)((sint32)(0 - value) >> 5);
    value = actor_motion->velocity[1];
    vector[1] = (uint16)((sint32)(0 - value) >> 5);
    value = actor_motion->velocity[2];
    vector[2] = (uint16)((sint32)(0 - value) >> 5);
    xport_gte_write_data(0, vector[0] | (vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    value = settings->altitude.maximum_low;
    speed = ((sint32)c < 0 ? 0 - c : c) << 5;
    if ((sint32)value < (sint32)speed)
    {
        numerator = value << 6;
        if (speed == 0)
            xport_mips_break(7);
        if (speed == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        ratio = (sint32)numerator / (sint32)speed;
        a = actor_motion->velocity[0] * ratio;
        b = actor_motion->velocity[1] * ratio;
        actor_motion->velocity[0] = (uint32)((sint32)a / 64);
        c = actor_motion->velocity[2] * ratio;
        actor_motion->velocity[1] = (uint32)((sint32)b / 64);
        actor_motion->velocity[2] = (uint32)((sint32)c / 64);
        speed = settings->altitude.maximum_low;
    }
    stall = 0;
    factor = flight_model_sample_altitude(native_object->pose.position[1], settings, &bound0, &bound1, &bound2);
    divisor = (sint32)bound0 < 118784 ? 118784 : bound0;
    numerator = speed << 11;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (uint32)((sint32)numerator / (sint32)divisor) << 1;
    value = (sint16)flight_orientation.angle_delta.value[0];
    if ((sint32)value < 0)
    {
        divisor = settings->lift_pitch_limit[0];
        if ((sint32)value < (sint32)divisor)
            value = divisor;
        numerator = (settings->lift_pitch_factor[0] - 4096) * value;
    }
    else
    {
        divisor = settings->lift_pitch_limit[1];
        if ((sint32)divisor < (sint32)value)
            value = divisor;
        numerator = (settings->lift_pitch_factor[1] - 4096) * value;
    }
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    bias = (uint32)((sint32)numerator / (sint32)divisor) + 4096;
    lift = (sint32)(ratio * bias) / 4096;
    if ((sint32)(lift + bias) >= 8993 && (sint32)lift >= 6145 && (sint32)bias >= 5121)
    {
        flags = state->events;
        state->events = (uint32)(flags | 0x1000);
    }
    value = settings->lift_min;
    if ((sint32)lift < (sint32)value)
    {
        stall = 4096;
        strength = 0;
    }
    else
    {
        strength = (sint32)((lift - value) * settings->lift_gain) / 4096;
        if ((sint32)strength > 4096)
            strength = (uint32)((sint32)((strength - 4096) * settings->lift_gain_high) / 4096) + 4096;
        value = settings->lift_pitch_factor[0];
        if ((sint32)value < (sint32)strength)
            strength = value;
        value = lift - settings->lift_min;
        if ((sint32)value < 256)
            stall = 4096 - (value << 4);
    }
    capped_strength = (sint32)strength > 4096 ? 4096 : strength;
    state->control_gain = (uint16)(capped_strength);
    value = (sint32)lift > 4096 ? 4096 : lift;
    other = (sint16)native_object->pose.angles[0];
    if ((sint32)other < 0)
        other = 0 - other;
    vertical = (sint32)((4096 - other) * value) / 4096;
    value = native_object->pose.angles[2] & 0x3FFF;
    a = game_wave_at((value + 0x1000) * 2);
    b = game_wave_at(value * 2);
    other = (sint32)a < 0 ? 0 - a : a;
    if ((sint32)other < 1025)
        a = (sint32)a < 0 ? (uint32)-1024 : 1024;
    numerator = b << 12;
    if (a == 0)
        xport_mips_break(7);
    if (a == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    value = (sint32)numerator / (sint32)a;
    if ((sint32)value < 0)
        value = 0 - value;
    lateral = (sint32)(vertical * value) / 4096;
    value = (sint32)lift > 6021 ? 6021 : lift;
    if ((sint32)value < (sint32)lateral)
        magnitude = value;
    else
    {
        magnitude = game_vector_length_gte(lateral, vertical, 0);
        if ((sint32)value < (sint32)magnitude)
            magnitude = value;
    }
    rotation_build(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    gravity = -669;
    if ((sint32)factor < 4096)
        gravity = (sint32)(gravity * factor) / 4096;
    pull = (sint32)(gravity * magnitude) / 4096;
    xport_gte_write_data(0, (pull & 0xFFFF) << 16);
    xport_gte_write_data(1, 0);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    value = (uint32)((sint32)(669 * (4096 - factor)) / 4096) + 669;
    acceleration[0] += a;
    acceleration[1] += b + value;
    acceleration[2] += c;
    coefficient_y = settings->rotation_damping[1];
    coefficient_x = settings->rotation_damping[0];
    coefficient_z = settings->rotation_damping[2];
    limits[0] = settings->rotation_damping_limit[0];
    limits[1] = settings->rotation_damping_limit[1];
    limits[2] = settings->rotation_damping_limit[2];
    if (stall != 0)
    {
        coefficient_x += stall;
        coefficient_z += stall;
        if ((sint32)coefficient_x > 4096)
            coefficient_x = 4096;
        if ((sint32)coefficient_z > 4096)
            coefficient_z = 4096;
    }
    local_angles[0] = 0;
    local_angles[1] = 0;
    local_angles[2] = native_object->pose.angles[2];
    rotation_build(&matrix, local_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    a = ((uint32)flight_orientation.angle_delta.value[0] | ((uint32)flight_orientation.angle_delta.value[1] << 16));
    b = (uint32)flight_orientation.angle_delta.value[2];
    xport_gte_write_data(0, a);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    flags = state->mode;
    if ((flags & 0xC108) != 0)
        coefficient_x = (sint32)coefficient_x >> 2;
    vector[0] = (sint32)(a * coefficient_x) / 4096;
    vector[1] = (sint32)(b * coefficient_y) / 4096;
    vector[2] = (sint32)(c * coefficient_z) / 4096;
    for (i = 0; i < 3; ++i)
    {
        value = 0 - limits[i];
        if ((sint32)vector[i] < (sint32)value)
            vector[i] = value;
        if ((sint32)limits[i] < (sint32)vector[i])
            vector[i] = limits[i];
    }
    flags = state->events;
    if ((flags & 2) == 0)
    {
        a = native_object->pose.angles[0];
        c = native_object->pose.angles[2];
        native_object->pose.angles[0] = (uint16)(a + vector[0]);
        b = native_object->pose.angles[1];
        native_object->pose.angles[2] = (uint16)(c + vector[2]);
        native_object->pose.angles[1] = (uint16)(b + vector[1]);
    }
    if (stall != 0)
    {
        flags = state->events;
        if ((flags & 1) == 0 && (~flags & 10) == 10)
        {
            value = (sint32)((0 - 37) * stall) / 4096;
            a = (sint16)native_object->pose.angles[0];
            b = native_object->pose.angles[0];
            if ((sint32)((uint32)-4096 - value) < (sint32)a)
                native_object->pose.angles[0] = (uint16)(b + value);
        }
    }
    command[0] = (sint16)control->pitch;
    command[1] = (sint16)control->yaw;
    command[2] = (sint16)control->roll;
    gain = settings->pitch_rate[(sint32)command[0] < 0 ? 1 : 0];
    value = (sint16)flight_orientation.angle_delta.value[0];
    value = (sint32)(value * (uint32)((sint32)speed >> 1)) / 8192;
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value > 12800)
        value = 12800;
    value = (sint32)(value * 3192) / 12800;
    a = (sint16)control->pitch;
    if ((sint32)a < 0)
        a = 0 - a;
    value = (sint32)((3192 - value) * a) / 4096;
    value = (sint32)(gain * value) / 4096;
    b = (sint16)control->roll;
    if ((sint32)b < 0)
        b = 0 - b;
    divisor = a + b;
    gain += value;
    if (divisor == 0)
    {
        weight_x = 0;
        weight_z = 0;
    }
    else
    {
        numerator = a << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        weight_x = (sint32)numerator / (sint32)divisor;
        numerator = b << 12;
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        weight_z = (sint32)numerator / (sint32)divisor;
    }
    a = (sint32)(command[0] * weight_x) / 4096;
    b = (sint32)(command[2] * weight_z) / 4096;
    a = (sint32)(a * strength) / 4096;
    b = (sint32)(b * strength) / 4096;
    c = (sint32)(command[1] * (4096 - stall)) / 4096;
    desired[0] = (sint32)(a * gain) / 4096;
    value = settings->yaw_rate;
    desired[1] = (sint32)(c * value) / 4096;
    value = settings->roll_rate;
    desired[2] = (sint32)(b * value) / 4096;
    value = settings->angular_response;
    a = (sint16)flight_orientation.response[0];
    current[0] = a + (uint32)((sint32)(desired[0] * value) / 4096);
    a = (sint16)flight_orientation.response[1];
    current[1] = a + (uint32)((sint32)(desired[1] * value) / 4096);
    a = (sint16)flight_orientation.response[2];
    current[2] = a + (uint32)((sint32)(desired[2] * value) / 4096);
    if ((sint32)current[0] < (sint32)(0 - gain))
        current[0] = 0 - gain;
    else if ((sint32)gain < (sint32)current[0])
        current[0] = gain;
    value = settings->yaw_rate;
    if ((sint32)current[1] < (sint32)(0 - value))
        current[1] = 0 - value;
    else if ((sint32)value < (sint32)current[1])
        current[1] = value;
    value = settings->roll_rate;
    if ((sint32)current[2] < (sint32)(0 - value))
        current[2] = 0 - value;
    else if ((sint32)value < (sint32)current[2])
        current[2] = value;
    turn[0] = current[0];
    turn[1] = current[1];
    turn[2] = current[2];
    value = settings->angular_response;
    if ((sint32)strength > 4096)
        damping = 4096 - value;
    else
    {
        damping = 4096 - (uint32)((sint32)(value * strength) / 4096);
        if ((sint32)damping > 4092)
            damping = 4092;
    }
    for (i = 0; i < 3; ++i)
    {
        a = (sint32)desired[i] < 0 ? 0 - desired[i] : desired[i];
        b = (sint32)current[i] < 0 ? 0 - current[i] : current[i];
        if ((sint32)a < (sint32)b)
        {
            current[i] = (sint32)(current[i] * damping) / 4096;
            b = (sint32)current[i] < 0 ? 0 - current[i] : current[i];
            if ((sint32)b < (sint32)a)
                current[i] = desired[i];
        }
    }
    if ((sint32)factor < 4096)
    {
        value = (sint32)factor < 128 ? 128 : factor;
        for (i = 0; i < 3; ++i)
            current[i] = (sint32)(current[i] * value) / 4096;
    }
    flags = state->mode;
    flight_orientation.response[0] = (uint16)(current[0]);
    flight_orientation.response[1] = (uint16)(current[1]);
    flight_orientation.response[2] = (uint16)(current[2]);
    gain = settings->thrust_gain;
    if ((flags & 8) != 0)
    {
        gain = (sint32)gain >> 1;
        bound0 = 40960;
        bound1 = 65536;
        bound2 = 106496;
    }
    else if ((flags & 0x100) != 0)
    {
        bound0 = 32768;
        bound1 = 104960;
        bound2 = 225280;
    }
    else if ((flags & 0xC000) != 0)
    {
        gain = (sint32)gain >> 1;
        bound0 = 4096;
        bound1 = 61440;
        bound2 = 245760;
    }
    if ((sint32)bound0 < 0)
        bound0 = 0;
    throttle = (sint16)control->thrust;
    if ((sint32)throttle < 0 || (sint32)capped_strength < 4096)
        throttle = (sint16)control->thrust;
    else
    {
        value = (sint16)flight_orientation.angle_delta.value[0];
        if ((sint32)value < 0)
            value = 0 - value;
        throttle += value << 2;
        if ((sint32)throttle < -4096)
            throttle = -4096;
        if ((sint32)throttle > 4096)
            throttle = 4096;
    }
    thrust = flight_thrust_response(speed, bound2, bound1, throttle, native_object->pose.position[1], gain, gain);
    if ((sint32)thrust < -8192)
        thrust = -8192;
    else if ((sint32)thrust > 8192)
        thrust = 8192;
    a = native_object->pose.angles[0];
    b = settings->thrust_pitch_limit;
    c = (uint16)game_wave_at((a & 0x3FFF) * 2);
    c = 4096 - (uint32)((sint32)(sint16)c >> 2);
    d = settings->thrust_limit;
    value = (sint16)a > 0 ? (uint32)((sint32)((d - b) * c) / 4096) + b : d;
    if ((sint32)value < (sint32)thrust)
        thrust = value;
    if ((sint32)thrust < (sint32)(0 - value))
        thrust = 0 - value;
    thrust = (sint32)(thrust * capped_strength) / 4096;
    rotation_build(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, thrust & 0xFFFF);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    if ((sint32)b < 0 && (sint32)factor < 4096)
        b = (sint32)(b * factor) / 4096;
    acceleration[0] += a;
    acceleration[1] += b;
    acceleration[2] += c;
    state->speed = (uint32)(speed);
    value = settings->linear_drag << 12;
    value = (sint32)value / 4096;
    a = actor_motion->velocity[0];
    vector[0] = (sint32)((0 - a) * value) / 4096;
    a = actor_motion->velocity[1];
    vector[1] = (sint32)((0 - a) * value) / 4096;
    a = actor_motion->velocity[2];
    vector[2] = (sint32)((0 - a) * value) / 4096;
    rotation_build(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    value = game_vector_length_gte((sint32)vector[0] >> 1, (sint32)vector[1] >> 1, (sint32)vector[2] >> 1);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, value & 0xFFFF);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    a <<= 1;
    b <<= 1;
    c <<= 1;
    if ((sint32)factor < 4096 && (sint32)b < 0)
        b = (sint32)(b * factor) / 4096;
    acceleration[0] += a;
    acceleration[1] += b;
    acceleration[2] += c;
    if ((sint32)factor < 4096 && (sint32)vector[1] < 0)
        vector[1] = (sint32)(vector[1] * factor) / 4096;
    acceleration[0] += vector[0];
    acceleration[2] += vector[2];
    acceleration[1] += vector[1];
    value = actor_motion->velocity[0];
    actor_motion->velocity[0] = (uint32)(value + acceleration[0]);
    value = actor_motion->velocity[1];
    actor_motion->velocity[1] = (uint32)(value + acceleration[1]);
    value = actor_motion->velocity[2];
    actor_motion->velocity[2] = (uint32)(value + acceleration[2]);
    game_vector_angles(actor_motion->velocity, flight_orientation.velocity_angles.value);
    actor_motion->angular_delta[0] = (uint16)(turn[0]);
    actor_motion->angular_delta[1] = (uint16)(turn[1]);
    actor_motion->angular_delta[2] = (uint16)(turn[2]);
    game_angles_apply_delta(actor_motion->angular_delta, native_object->pose.angles);
    flight_orientation_update_delta(&flight_orientation, native_object);
    {
        (void)(pad_rumble_add_scaled(4096 - capped_strength));
        return;
    }
}

void flight_physics_integrate_secondary(const FLIGHT_COMMAND *control, const FLIGHT_AIRCRAFT_PROFILE *settings, FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    MATRIX matrix;
    uint32 pitch_input, yaw_input, roll_input, pitch_limit, yaw_limit, roll_limit, product, pitch_step, yaw_step, roll_step, pitch, yaw, roll, saved[3], steps[3], values[3], index, magnitude, absolute_step, value, a, b, c, d, e, vx, vy, vz, speed, x, y, z, forward, thrust, ax, ay, az;
    FUNCTION_MARKER(0x800702D8, "ACE3.BIN");
    pitch_input = (sint16)control->pitch;
    pitch_limit = settings->pitch_rate[0];
    product = pitch_input * pitch_limit;
    yaw_input = (sint16)control->yaw;
    roll_input = (sint16)control->roll;
    pitch_step = (sint32)product / 4096;
    yaw_limit = settings->yaw_rate;
    product = yaw_input * yaw_limit;
    yaw_step = (sint32)product / 4096;
    roll_limit = settings->roll_rate;
    product = roll_input * roll_limit;
    roll_step = (sint32)product / 4096;
    pitch = (sint16)flight_orientation.response[0];
    roll = (sint16)flight_orientation.response[2];
    pitch += pitch_step;
    yaw = (sint16)flight_orientation.response[1];
    yaw += yaw_step;
    roll += roll_step;
    if ((sint32)pitch < (sint32)(0 - pitch_limit))
        pitch = 0 - pitch_limit;
    else if ((sint32)pitch_limit < (sint32)pitch)
        pitch = pitch_limit;
    yaw_limit = settings->yaw_rate;
    if ((sint32)yaw < (sint32)(0 - yaw_limit))
        yaw = 0 - yaw_limit;
    else if ((sint32)yaw_limit < (sint32)yaw)
        yaw = yaw_limit;
    roll_limit = settings->roll_rate;
    if ((sint32)roll < (sint32)(0 - roll_limit))
        roll = 0 - roll_limit;
    else if ((sint32)roll_limit < (sint32)roll)
        roll = roll_limit;
    saved[0] = pitch;
    saved[1] = yaw;
    saved[2] = roll;
    values[0] = pitch;
    values[1] = yaw;
    values[2] = roll;
    steps[0] = pitch_step;
    steps[1] = yaw_step;
    steps[2] = roll_step;
    for (index = 0; index < 3; index += 1)
    {
        absolute_step = steps[index];
        if ((sint32)absolute_step < 0)
            absolute_step = 0 - absolute_step;
        magnitude = values[index];
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)absolute_step < (sint32)magnitude)
        {
            product = values[index] * 3967;
            values[index] = (sint32)product / 4096;
            magnitude = values[index];
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude < (sint32)absolute_step)
                values[index] = steps[index];
        }
    }
    flight_orientation.response[0] = (uint16)(values[0]);
    flight_orientation.response[1] = (uint16)(values[1]);
    flight_orientation.response[2] = (uint16)(values[2]);
    actor_motion->angular_delta[0] = (uint16)(saved[0]);
    actor_motion->angular_delta[1] = (uint16)(saved[1]);
    actor_motion->angular_delta[2] = (uint16)(saved[2]);
    game_angles_apply_delta(actor_motion->angular_delta, native_object->pose.angles);
    vx = actor_motion->velocity[0];
    vy = actor_motion->velocity[1];
    vz = actor_motion->velocity[2];
    speed = game_vector_length_gte((sint32)(0 - vx) >> 6, (sint32)(0 - vy) >> 6, (sint32)(0 - vz) >> 6) << 6;
    game_build_rotation_via_gte(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    x = actor_motion->velocity[0];
    x = (sint32)x >> 6;
    y = actor_motion->velocity[1];
    y = (sint32)y >> 6;
    z = actor_motion->velocity[2];
    z = (sint32)z >> 6;
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    vx = xport_gte_read_data(25);
    vy = xport_gte_read_data(26);
    vz = xport_gte_read_data(27);
    value = settings->thrust_gain;
    thrust = (sint16)control->thrust;
    product = thrust * value;
    forward = vz << 6;
    thrust = (sint32)product >> 12;
    if ((sint32)forward < 0 && (sint32)thrust < 0)
        thrust = 0;
    value = settings->altitude.maximum_low;
    if ((sint32)value < (sint32)forward && (sint32)thrust > 0)
        thrust = 0;
    rotation_build(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, thrust & 0xFFFF);
    xport_gte_execute(0x486012);
    ax = xport_gte_read_data(25);
    ay = xport_gte_read_data(26);
    az = xport_gte_read_data(27);
    vx = actor_motion->velocity[0];
    ax += (uint32)((sint32)(0 - vx) >> 8);
    vy = actor_motion->velocity[1];
    ay += (uint32)((sint32)(0 - vy) >> 8);
    vz = actor_motion->velocity[2];
    az += (uint32)((sint32)(0 - vz) >> 8);
    value = actor_motion->velocity[0];
    actor_motion->velocity[0] = (uint32)(value + ax);
    value = actor_motion->velocity[1];
    actor_motion->velocity[1] = (uint32)(value + ay);
    value = actor_motion->velocity[2];
    vx = actor_motion->velocity[0];
    value += az;
    actor_motion->velocity[2] = (uint32)(value);
    if ((sint32)vx > 0xB4000)
        actor_motion->velocity[0] = (uint32)(0xB4000);
    value = actor_motion->velocity[0];
    if ((sint32)value < -737280)
        actor_motion->velocity[0] = (uint32)(0xFFF4C000);
    value = actor_motion->velocity[1];
    if ((sint32)value > 737280)
        actor_motion->velocity[1] = (uint32)(0xB4000);
    value = actor_motion->velocity[1];
    if ((sint32)value < -737280)
        actor_motion->velocity[1] = (uint32)(0xFFF4C000);
    value = actor_motion->velocity[2];
    if ((sint32)value > 737280)
        actor_motion->velocity[2] = (uint32)(0xB4000);
    value = actor_motion->velocity[2];
    if ((sint32)value < -737280)
        actor_motion->velocity[2] = (uint32)(0xFFF4C000);
    state->speed = (uint32)(speed);
    game_vector_angles(actor_motion->velocity, flight_orientation.velocity_angles.value);
    flight_orientation_update_delta(&flight_orientation, native_object);
    state->control_gain = (uint16)(4096);
    return;
}

void flight_orientation_update_delta(FLIGHT_ORIENTATION *orientation, const MISSION_ACTOR *actor)
{
    MATRIX first = {0}, second = {0};
    uint32 vector0, vector1, a, b, c, d, packed, depth, x, y, z, result;
    uint32 column0[3], column1[3];
    FUNCTION_MARKER(0x8006EE38, "ACE3.BIN");
    vector0 = ((uint32)(uint16)flight_orientation_axis.vx | ((uint32)(uint16)flight_orientation_axis.vy << 16));
    vector1 = ((uint32)(uint16)flight_orientation_axis.vz | ((uint32)(uint16)flight_orientation_axis.pad << 16));
    game_build_rotation_via_gte(&first, actor->pose.angles);
    rotation_build(&second, orientation->velocity_angles.value);
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    packed = ((uint16)second.m[0][0]) | (((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16)) & 0xFFFF0000);
    depth = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][1]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) << 16);
    depth = (sint16)((uint16)second.m[2][1]);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][2]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) & 0xFFFF0000);
    depth = ((uint16)second.m[2][2]);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    {
        uint32 matrix_word = (column0[0] & 0xFFFF) | (column1[0] << 16);
        first.m[0][0] = (sint16)matrix_word;
        first.m[0][1] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column0[2] & 0xFFFF) | (column1[2] << 16);
        first.m[2][0] = (sint16)matrix_word;
        first.m[2][1] = (sint16)(matrix_word >> 16);
    }
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    {
        uint32 matrix_word = (a & 0xFFFF) | (column0[1] << 16);
        first.m[0][2] = (sint16)matrix_word;
        first.m[1][0] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column1[1] & 0xFFFF) | (b << 16);
        first.m[1][1] = (sint16)matrix_word;
        first.m[1][2] = (sint16)(matrix_word >> 16);
    }
    first.m[2][2] = (sint16)(xport_gte_read_data(11));
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    xport_gte_write_data(0, vector0);
    xport_gte_write_data(1, vector1);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    result = game_vector_atan2(0 - y, z);
    orientation->angle_delta.value[0] = (uint16)(result);
    result = game_vector_atan2(x, z);
    orientation->angle_delta.value[1] = (uint16)(result);
    orientation->angle_delta.value[2] = (uint16)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

void flight_orientation_apply_object(FLIGHT_ORIENTATION *orientation, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    MATRIX first = {0}, second = {0};
    uint32 vector0, vector1, a, b, c, d, packed, depth, x, y, z, result;
    uint32 column0[3], column1[3];
    FUNCTION_MARKER(0x8007084C, "ACE3.BIN");
    result = ~actor_traits->flags & 0xC000;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    if ((actor_traits->events & 0x400000) != 0)
        game_angles_apply_delta(actor_motion->angular_delta, native_object->pose.angles);
    if ((actor_traits->events & 0x100000) != 0)
    {
        game_angles_apply_delta(actor_motion->angular_delta, orientation->velocity_angles.value);
        if ((actor_traits->events & 0x100000) != 0)
            game_angles_compose_delta(orientation->velocity_angles.value, actor_motion->angular_delta, native_object->pose.angles);
    }
    if ((actor_traits->events & 0x20000) != 0)
        game_rotate_vector(native_object->pose.angles, actor_motion->local_velocity, actor_motion->velocity);
    if ((actor_traits->events & 0x10000) != 0)
        game_rotate_vector(orientation->velocity_angles.value, actor_motion->local_velocity, actor_motion->velocity);
    if ((actor_traits->events & 0xF80000) != 0)
    {
        vector0 = ((uint32)(uint16)flight_orientation_axis.vx | ((uint32)(uint16)flight_orientation_axis.vy << 16));
        vector1 = ((uint32)(uint16)flight_orientation_axis.vz | ((uint32)(uint16)flight_orientation_axis.pad << 16));
        game_build_rotation_via_gte(&first, native_object->pose.angles);
        rotation_build(&second, orientation->velocity_angles.value);
        a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
        b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
        c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
        d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
        SetRotMatrix(&first);
        packed = ((uint16)second.m[0][0]) | (((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16)) & 0xFFFF0000);
        depth = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        packed = ((uint16)second.m[0][1]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) << 16);
        depth = (sint16)((uint16)second.m[2][1]);
        column0[0] = xport_gte_read_data(9);
        column0[1] = xport_gte_read_data(10);
        column0[2] = xport_gte_read_data(11);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        packed = ((uint16)second.m[0][2]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) & 0xFFFF0000);
        depth = ((uint16)second.m[2][2]);
        column1[0] = xport_gte_read_data(9);
        column1[1] = xport_gte_read_data(10);
        column1[2] = xport_gte_read_data(11);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        {
            uint32 matrix_word = (column0[0] & 0xFFFF) | (column1[0] << 16);
            first.m[0][0] = (sint16)matrix_word;
            first.m[0][1] = (sint16)(matrix_word >> 16);
        }
        {
            uint32 matrix_word = (column0[2] & 0xFFFF) | (column1[2] << 16);
            first.m[2][0] = (sint16)matrix_word;
            first.m[2][1] = (sint16)(matrix_word >> 16);
        }
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        {
            uint32 matrix_word = (a & 0xFFFF) | (column0[1] << 16);
            first.m[0][2] = (sint16)matrix_word;
            first.m[1][0] = (sint16)(matrix_word >> 16);
        }
        {
            uint32 matrix_word = (column1[1] & 0xFFFF) | (b << 16);
            first.m[1][1] = (sint16)matrix_word;
            first.m[1][2] = (sint16)(matrix_word >> 16);
        }
        first.m[2][2] = (sint16)(xport_gte_read_data(11));
        a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
        b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
        c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
        d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
        SetRotMatrix(&first);
        xport_gte_write_data(0, vector0);
        xport_gte_write_data(1, vector1);
        xport_gte_execute(0x486012);
        x = xport_gte_read_data(25);
        y = xport_gte_read_data(26);
        z = xport_gte_read_data(27);
        result = game_vector_atan2(0 - y, z);
        orientation->angle_delta.value[0] = (uint16)(result);
        result = game_vector_atan2(x, z);
        orientation->angle_delta.value[1] = (uint16)(result);
        orientation->angle_delta.value[2] = (uint16)(0);
    }
    result = actor_traits->events & 0xFF000000;
    actor_traits->events = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 flight_model_sample_altitude(uint32 value, const FLIGHT_AIRCRAFT_PROFILE *settings, uint32 *out0, uint32 *out1, uint32 *out2)
{
    uint32 position, bound, start, base, other, end, center, delta, numerator, q0, q1, q2;
    uint32 product0, product1, product2, fraction, wave, result0, result2, cap, quarter, result1, result;
    FUNCTION_MARKER(0x8006EA88, "ACE3.BIN");
    position = ((sint32)value >> 12);
    bound = settings->altitude.height_bound;
    start = settings->altitude.reference_high;
    base = settings->altitude.maximum_high;
    other = settings->altitude.reference_low;
    end = settings->altitude.maximum_low;
    center = settings->altitude.cap_low;
    value = settings->altitude.cruise_low;
    if ((sint32)position > 0)
        position = 0;
    if ((sint32)position < (sint32)bound)
        position = bound;
    numerator = 0 - ((position - bound) << 14);
    if (bound == 0)
        xport_mips_break(7);
    if (bound == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q0 = ((sint64)(sint32)numerator / (sint32)bound);
    delta = end - base;
    product0 = delta * q0;
    wave = (q0 << 2) + q0;
    numerator = position << 14;
    if (bound == 0)
        xport_mips_break(7);
    if (bound == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q1 = ((sint64)(sint32)numerator / (sint32)bound);
    wave = (uint32)((sint32)wave >> 2) & 0x7FFE;
    product1 = (uint32)game_wave_at(wave) * (uint32)((sint32)delta >> 2);
    fraction = ((sint32)(start - other) >> 2);
    product2 = (uint32)game_wave_at((q1 & 0x7FFE)) * fraction;
    fraction *= q1;
    result2 = (uint32)((sint32)product0 >> 14) + base + (uint32)((sint32)product1 >> 13);
    result0 = (uint32)((sint32)fraction >> 12) + other - (uint32)((sint32)product2 >> 14);
    cap = result0 + (center - other);
    if ((sint32)cap < (sint32)result2)
        result2 = cap;
    quarter = ((sint32)bound >> 2);
    numerator = (position - (bound - quarter)) << 12;
    if (quarter == 0)
        xport_mips_break(7);
    if (quarter == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q2 = ((sint64)(sint32)numerator / (sint32)quarter);
    delta = (uint32)((sint32)(start + base) >> 1) - value;
    product0 = (uint32)((sint32)delta >> 2) * q1;
    *out0 = result0;
    *out2 = result2;
    result1 = (uint32)((sint32)product0 >> 12) + value;
    *out1 = result1;
    result = 4096 - q2;
    if ((sint32)result > 4096)
        result = 4096;
    if ((sint32)result < 0)
        result = 0;
    return result;
}

void flight_sample_ai_altitude_speeds(uint32 altitude, uint32 *first, uint32 *second, uint32 *third)
{
    uint32 height, numerator, ratio0, ratio1, product0, product1, phase, sine0, sine1, a, b, c, cap, blend;
    FUNCTION_MARKER(0x8006EC70, "ACE3.BIN");
    height = ((sint32)altitude >> 12);
    if ((sint32)height > 0)
        height = 0;
    if ((sint32)height < -50000)
        height = -50000;
    numerator = 0 - ((height + 50000) << 14);
    ratio0 = ((sint32)numerator / -50000);
    product0 = ratio0 * 0x7000;
    numerator = height << 14;
    ratio1 = ((sint32)numerator / -50000);
    phase = (uint32)((sint32)((ratio0 << 2) + ratio0) >> 2) & 0x7FFE;
    sine0 = game_wave_at(phase);
    product1 = sine0 * 0x1C00;
    sine1 = game_wave_at((ratio1 & 0x7FFE));
    a = (uint32)((sint32)(ratio1 * 0xB800) >> 12) + 0xD000 - (uint32)((sint32)(sine1 * 0xB800) >> 14);
    b = (uint32)((sint32)product0 >> 14) + 0x52000 + (uint32)((sint32)product1 >> 13);
    cap = a + 0x4A000;
    if ((sint32)cap < (sint32)b)
        b = cap;
    numerator = (height + 37500) << 12;
    blend = ((sint32)numerator / -12500);
    *first = a;
    *third = b;
    c = (uint32)((sint32)(ratio1 * 0xA200) >> 12) + 0x1E000;
    *second = c;
    blend = 0x1000 - blend;
    if ((sint32)blend >= 0x1001)
        blend = 0x1000;
    {
        (void)(uint32)(blend);
        return;
    }
}

uint32 flight_thrust_response(uint32 value, uint32 maximum, uint32 middle, uint32 offset, uint32 speed, uint32 positive, uint32 negative)
{
    uint32 numerator, denominator, factor, product, coefficient, intermediate, high;
    sint64 wide;
    FUNCTION_MARKER(0x8006E908, "ACE3.BIN");
    if ((sint32)value < 0)
        value = 0;
    if ((sint32)maximum < (sint32)value)
        value = maximum;
    if ((sint32)middle < (sint32)value)
    {
        numerator = (value - middle) << 12;
        denominator = maximum - middle;
        if (denominator == 0)
            xport_mips_break(7);
        if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        factor = ((sint64)(sint32)numerator / (sint32)denominator);
        factor = (uint32)((sint32)factor >> 1) + 2048;
    }
    else
    {
        numerator = value << 12;
        if (middle == 0)
            xport_mips_break(7);
        if (middle == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        factor = ((sint64)(sint32)numerator / (sint32)middle);
        factor = ((sint32)factor >> 1);
    }
    if ((sint32)factor > 4096)
        factor = 4096;
    if ((sint32)factor < -4096)
        factor = -4096;
    intermediate = offset + 4096;
    factor = (uint32)((sint32)intermediate >> 1) - factor;
    product = ((sint32)factor > 0 ? positive : negative) * factor;
    if ((sint32)product < 0)
        product += 4095;
    factor = ((sint32)product >> 12);
    speed = ((sint32)speed >> 12);
    if ((sint32)speed < -54822)
        speed = -54822;
    intermediate = (speed << 4) - speed;
    intermediate = (intermediate << 4) - speed;
    coefficient = (intermediate << 2) - intermediate;
    coefficient = (coefficient << 2) - speed;
    wide = (sint64)(sint32)coefficient * 0x4C81F747;
    high = ((uint64)wide >> 32);
    intermediate = (uint32)((sint32)coefficient >> 31) - (uint32)((sint32)high >> 14);
    product = factor * (4096 - intermediate);
    if ((sint32)product < 0)
        product += 4095;
    return ((sint32)product >> 12);
}
