#include "game_functions.h"

FLIGHT_CONTROL flight_control;
FLIGHT_INPUT flight_input;
FLIGHT_COMMAND flight_command;

void flight_control_initialize(uint32 phase_origin)
{
    uint32 index;
    for (index = 0; index < 3; ++index)
        flight_control.axis[index] = 0;
    flight_control.thrust = 0;
    flight_control.brake = 0;
    flight_control.emitter_bits = 0;
    flight_control.phase_origin = phase_origin;
}

void flight_control_scale_axes(sint32 distance, FLIGHT_CONTROL *vector)
{
    uint32 scale, x, y, z;
    FUNCTION_MARKER(0x8006E874, "ACE3.BIN");
    scale = distance > 86015 ? 4096 : distance / 21;
    y = (uint32)(sint16)vector->axis[1] * scale;
    z = (sint16)vector->axis[2];
    x = (sint16)vector->axis[0];
    z *= scale;
    x *= scale;
    y = (sint32)y / 4096;
    z = (sint32)z / 4096;
    x = (sint32)x / 4096;
    vector->axis[1] = (uint16)(y);
    vector->axis[2] = (uint16)(z);
    vector->axis[0] = (uint16)(x);
    {
        (void)(uint32)(z);
        return;
    }
}

void flight_control_update_standard(const GAME_INPUT *input, FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth)
{
    uint32 axis[4], bounds[3], target[3], step[3], flags, mode, value, raw, magnitude, current, product, high, delta, i, limit;
    FUNCTION_MARKER(0x8006D0DC, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;
    axis[3] = 0;
    bounds[0] = 4096;
    bounds[1] = 4096;
    bounds[2] = 4096;
    mode = input->axis_mask;
    flags = input->held;
    if ((mode & 1) != 0)
    {
        axis[0] = (uint32)input->axis[0];
        bounds[0] = (sint32)axis[0] < 0 ? 0 - axis[0] : axis[0];
    }
    else
    {
        if ((flags & 0x800000) != 0)
            axis[0] = 4096;
        if ((flags & 0x1000000) != 0)
            axis[0] -= 4096;
    }
    mode = input->axis_mask;
    if ((mode & 4) != 0)
    {
        axis[1] = (uint32)input->axis[2];
        bounds[1] = (sint32)axis[1] < 0 ? 0 - axis[1] : axis[1];
    }
    else
    {
        if ((flags & 0x20000000) != 0)
            axis[1] -= 4096;
        if ((flags & 0x40000000) != 0)
            axis[1] += 4096;
    }
    mode = input->axis_mask;
    if ((mode & 2) != 0)
    {
        axis[2] = (uint32)input->axis[1];
        bounds[2] = (sint32)axis[2] < 0 ? 0 - axis[2] : axis[2];
    }
    else
    {
        if ((flags & 0x2000000) != 0)
            axis[2] -= 4096;
        if ((flags & 0x4000000) != 0)
            axis[2] += 4096;
    }
    mode = input->axis_mask;
    if ((mode & 8) != 0)
        axis[3] = (uint32)input->axis[3];
    else
    {
        if ((flags & 0x8000000) != 0)
            axis[3] += 4096;
        if ((flags & 0x10000000) != 0)
            axis[3] -= 4096;
    }
    for (i = 0; i < 4; ++i)
    {
        value = (sint16)control->axis[i];
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            control->axis[i] = (uint16)(value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)target[i] < (sint32)value)
            smooth->axis[i] = (uint16)(raw - step[i]);
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)value < (sint32)target[i])
            smooth->axis[i] = (uint16)(raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            smooth->axis[i] = (uint16)(target[i]);
    }
    limit = 0;
    if ((sint32)axis[3] < -2048)
        limit = (0 - (axis[3] + 2048)) << 1;
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < (sint32)limit)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)limit < (sint32)value)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)control->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    value = control->axis[0];
    control->axis[0] = (uint16)(value + (uint32)((sint32)axis[0] >> 3));
    value = control->axis[1];
    control->axis[1] = (uint16)(value + (uint32)((sint32)axis[1] >> 2));
    value = control->axis[2];
    control->axis[2] = (uint16)(value + (uint32)((sint32)axis[2] >> 5));
    value = control->axis[3];
    control->axis[3] = (uint16)(value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)control->axis[i];
        if ((sint32)value < (sint32)(0 - bounds[i]))
            control->axis[i] = (uint16)(0 - bounds[i]);
        value = (sint16)control->axis[i];
        if ((sint32)bounds[i] < (sint32)value)
            control->axis[i] = (uint16)(bounds[i]);
    }
    value = (sint16)control->axis[3];
    if ((sint32)value < -4096)
        control->axis[3] = (uint16)(-4096);
    value = (sint16)control->axis[3];
    if ((sint32)value > 4096)
        control->axis[3] = (uint16)(4096);
    value = control->axis[0];
    output->pitch = (uint16)(value);
    value = control->axis[1];
    output->yaw = (uint16)(value);
    value = control->axis[2];
    output->roll = (uint16)(value);
    value = control->axis[3];
    output->thrust = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_update_object_input(FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth, uint32 enabled, const MISSION_ACTOR *native_input)
{
    uint32 axis[4], bounds[3], target[3], step[3], value, raw, magnitude, current, product, high, delta, i, limit, roll, height, pitch;
    FUNCTION_MARKER(0x80073B18, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[3] = 0;
    roll = (sint16)native_input->pose.angles[2];
    axis[2] = (0 - roll) << 2;
    height = native_input->pose.position[1];
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    if ((sint32)height < (sint32)0xFF448000 || enabled == 0)
    {
        if ((sint32)magnitude < 2048)
        {
            pitch = (sint16)native_input->pose.angles[0];
            axis[0] = (0 - pitch) << 4;
        }
        pitch = (sint16)native_input->pose.angles[0];
        value = (sint32)pitch < 0 ? 0 - pitch : pitch;
        if ((sint32)value < 182)
            axis[0] = (sint32)axis[0] >> 1;
    }
    else if ((sint32)magnitude < 2048)
    {
        pitch = (sint16)native_input->pose.angles[0];
        if ((sint32)pitch < 2048)
            axis[0] = (2048 - pitch) << 4;
        axis[3] = 4096;
    }
    roll = (sint16)native_input->pose.angles[2];
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    if ((sint32)magnitude < 455)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)magnitude < 273)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)magnitude < 136)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)axis[0] < -4096)
        axis[0] = -4096;
    else if ((sint32)axis[0] > 4096)
        axis[0] = 4096;
    if ((sint32)axis[2] < -4096)
        axis[2] = -4096;
    else if ((sint32)axis[2] > 4096)
        axis[2] = 4096;
    bounds[0] = (sint32)axis[0] < 0 ? 0 - axis[0] : axis[0];
    bounds[1] = 0;
    bounds[2] = (sint32)axis[2] < 0 ? 0 - axis[2] : axis[2];
    for (i = 0; i < 4; ++i)
    {
        value = (sint16)control->axis[i];
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            control->axis[i] = (uint16)(value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)target[i] < (sint32)value)
            smooth->axis[i] = (uint16)(raw - step[i]);
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)value < (sint32)target[i])
            smooth->axis[i] = (uint16)(raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            smooth->axis[i] = (uint16)(target[i]);
    }
    limit = 0;
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < (sint32)limit)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)limit < (sint32)value)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)control->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    value = control->axis[0];
    raw = control->axis[1];
    control->axis[0] = (uint16)(value + (uint32)((sint32)axis[0] >> 3));
    value = control->axis[2];
    control->axis[2] = (uint16)(value + (uint32)((sint32)axis[2] >> 5));
    value = control->axis[3];
    control->axis[3] = (uint16)(value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)control->axis[i];
        if ((sint32)value < (sint32)(0 - bounds[i]))
            control->axis[i] = (uint16)(0 - bounds[i]);
        value = (sint16)control->axis[i];
        if ((sint32)bounds[i] < (sint32)value)
            control->axis[i] = (uint16)(bounds[i]);
    }
    value = (sint16)control->axis[3];
    if ((sint32)value < -4096)
        control->axis[3] = (uint16)(-4096);
    value = (sint16)control->axis[3];
    if ((sint32)value > 4096)
        control->axis[3] = (uint16)(4096);
    value = control->axis[0];
    output->pitch = (uint16)(value);
    value = control->axis[1];
    output->yaw = (uint16)(value);
    value = control->axis[2];
    output->roll = (uint16)(value);
    value = control->axis[3];
    output->thrust = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_update_physics(const GAME_INPUT *input, FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth, const FLIGHT_AIRCRAFT_PROFILE *settings, const FLIGHT_DYNAMICS *physics, MISSION_ACTOR *native_object)
{
    uint32 axis[4], bounds[3], target[3], step[3], flags, mode, value, raw, magnitude, current, product, high, delta, i, limit;
    uint32 command_roll, command_pitch, roll, pitch, abs_roll, abs_pitch, factor, correction, strength, sensitivity, term, tilt, blend;
    FUNCTION_MARKER(0x8006DD5C, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;
    axis[3] = 0;
    bounds[1] = 4096;
    command_roll = 0;
    command_pitch = 0;
    mode = input->axis_mask;
    flags = input->held;
    if ((mode & 2) != 0)
        command_roll = (uint32)input->axis[1];
    else
    {
        if ((flags & 0x2000000) != 0)
            command_roll = -4096;
        if ((flags & 0x4000000) != 0)
            command_roll += 4096;
    }
    mode = input->axis_mask;
    if ((mode & 1) != 0)
        command_pitch = (uint32)input->axis[0];
    else
    {
        if ((flags & 0x800000) != 0)
            command_pitch += 4096;
        if ((flags & 0x1000000) != 0)
            command_pitch -= 4096;
    }
    magnitude = (sint32)command_roll < 0 ? 0 - command_roll : command_roll;
    roll = (sint16)native_object->pose.angles[2];
    pitch = (sint16)native_object->pose.angles[0];
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    factor = 4096 - abs_pitch;
    correction = 0;
    if ((sint32)magnitude < 2048)
    {
        product = (0 - roll) * (2048 - magnitude);
        if ((sint32)product < 0)
            product += 2047;
        term = (sint32)product >> 11;
        product = term * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction = (sint32)product >> 12;
    }
    value = (sint16)native_object->pose.angles[2];
    if ((sint32)value > 2048)
    {
        product = (abs_roll - 2048) * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction -= (uint32)((sint32)product >> 12);
    }
    value = (sint16)native_object->pose.angles[2];
    if ((sint32)value < -2048)
    {
        product = (abs_roll - 2048) * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction += (uint32)((sint32)product >> 12);
    }
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    product = correction * (4096 - magnitude);
    if ((sint32)product < 0)
        product += 4095;
    correction = (sint32)product >> 12;
    roll = (sint16)native_object->pose.angles[2];
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    strength = 4096;
    if (((sint32)roll > 0 && (sint32)command_roll > 0) || ((sint32)roll < 0 && (sint32)command_roll < 0))
    {
        if ((sint32)abs_roll >= 1024)
        {
            product = (abs_roll - 1024) << 12;
            strength -= (uint32)((sint32)product / 1024);
        }
        if ((sint32)strength > 4096)
            strength = 4096;
        if ((sint32)strength < 0)
            strength = 0;
        sensitivity = settings->pitch_rate[0];
    }
    else
        sensitivity = settings->pitch_rate[1];
    product = strength * command_roll;
    if ((sint32)product < 0)
        product += 4095;
    correction += (uint32)((sint32)product >> 12);
    value = native_object->pose.angles[2];
    value = (uint16)game_wave_at(((value & 0x3FFF) << 1));
    term = (sint16)value >> 2;
    if ((sint32)term > 4096)
        term = 4096;
    if ((sint32)term < -4096)
        term = -4096;
    product = term * sensitivity;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    value = (sint16)physics->control_gain;
    product = term * value;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    raw = native_object->pose.angles[1];
    native_object->pose.angles[1] = (uint16)(raw + term);
    roll = (sint16)native_object->pose.angles[2];
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    strength = 4096;
    if ((sint32)abs_roll >= 2048)
    {
        product = (abs_roll - 2048) << 12;
        strength -= (uint32)((sint32)product / 2048);
    }
    product = strength * command_pitch;
    if ((sint32)product < 0)
        product += 4095;
    tilt = (sint32)product >> 12;
    if ((sint32)abs_roll >= 4096)
        tilt = 0 - tilt;
    strength = 0;
    if ((sint32)abs_roll >= 2048)
    {
        product = (abs_roll - 2048) << 12;
        strength = (sint32)product / 2048;
    }
    value = (sint16)native_object->pose.angles[2];
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    product = ((sint32)value < 0 ? strength : 0 - strength) * magnitude;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    pitch = (sint16)native_object->pose.angles[0];
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    product = term * (4096 - abs_pitch);
    if ((sint32)abs_roll < 4096)
    {
        if ((sint32)product < 0)
            product += 4095;
        correction += (uint32)((sint32)product >> 12);
    }
    pitch = (sint16)native_object->pose.angles[0];
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    product = abs_pitch * magnitude;
    raw = native_object->pose.angles[0];
    if ((sint32)product < 0)
        product += 4095;
    blend = (sint32)product >> 12;
    sensitivity = settings->pitch_rate[1];
    if ((sint32)abs_roll > 4096)
        sensitivity = 0 - sensitivity;
    value = (sint16)physics->control_gain;
    product = sensitivity * value;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    product = term * blend;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    axis[0] = tilt;
    bounds[0] = (sint32)tilt < 0 ? 0 - tilt : tilt;
    axis[2] = correction;
    native_object->pose.angles[0] = (uint16)(raw + term);
    mode = input->axis_mask;
    bounds[2] = (sint32)correction < 0 ? 0 - correction : correction;
    if ((mode & 4) != 0)
    {
        axis[1] = (uint32)input->axis[2];
        bounds[1] = (sint32)axis[1] < 0 ? 0 - axis[1] : axis[1];
    }
    else
    {
        if ((flags & 0x20000000) != 0)
            axis[1] -= bounds[1];
        if ((flags & 0x40000000) != 0)
            axis[1] += bounds[1];
    }
    if ((sint32)axis[1] > 4096)
        axis[1] = 4096;
    if ((sint32)axis[1] < -4096)
        axis[1] = -4096;
    if ((sint32)bounds[1] > 4096)
        bounds[1] = 4096;
    if ((sint32)bounds[1] < -4096)
        bounds[1] = -4096;
    mode = input->axis_mask;
    if ((mode & 8) != 0)
        axis[3] = (uint32)input->axis[3];
    else
    {
        if ((flags & 0x8000000) != 0)
            axis[3] += 4096;
        if ((flags & 0x10000000) != 0)
            axis[3] -= 4096;
    }
    for (i = 0; i < 4; ++i)
    {
        value = (sint16)control->axis[i];
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            control->axis[i] = (uint16)(value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)target[i] < (sint32)value)
            smooth->axis[i] = (uint16)(raw - step[i]);
        value = (sint16)smooth->axis[i];
        raw = smooth->axis[i];
        if ((sint32)value < (sint32)target[i])
            smooth->axis[i] = (uint16)(raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)smooth->axis[i];
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            smooth->axis[i] = (uint16)(target[i]);
    }
    limit = 0;
    if ((sint32)axis[3] < -2048)
        limit = (0 - (axis[3] + 2048)) << 1;
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < (sint32)limit)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)limit < (sint32)value)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)control->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    value = control->axis[0];
    control->axis[0] = (uint16)(value + (uint32)((sint32)axis[0] >> 3));
    value = control->axis[1];
    control->axis[1] = (uint16)(value + (uint32)((sint32)axis[1] >> 2));
    value = control->axis[2];
    control->axis[2] = (uint16)(value + (uint32)((sint32)axis[2] >> 5));
    value = control->axis[3];
    control->axis[3] = (uint16)(value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = (sint16)control->axis[i];
        if ((sint32)value < (sint32)(0 - bounds[i]))
            control->axis[i] = (uint16)(0 - bounds[i]);
        value = (sint16)control->axis[i];
        if ((sint32)bounds[i] < (sint32)value)
            control->axis[i] = (uint16)(bounds[i]);
    }
    value = (sint16)control->axis[3];
    if ((sint32)value < -4096)
        control->axis[3] = (uint16)(-4096);
    value = (sint16)control->axis[3];
    if ((sint32)value > 4096)
        control->axis[3] = (uint16)(4096);
    value = control->axis[0];
    output->pitch = (uint16)(value);
    value = control->axis[1];
    output->yaw = (uint16)(value);
    value = control->axis[2];
    output->roll = (uint16)(value);
    value = control->axis[3];
    output->thrust = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_update_selection(FLIGHT_DYNAMICS *state, FLIGHT_CONTROL *smooth, const FLIGHT_RELATIVE_PROFILE *selection, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    MISSION_ACTOR *pose_owner_target;
    MISSION_ACTOR_MOTION *actor_motion_value;

    uint32 actor_index_table;

    MATRIX matrix;
    uint32 index, x, y, z, a, b, c, d, e, rx, ry, rz, value, vx, vy, vz, speed;
    FUNCTION_MARKER(0x80074E64, "ACE3.BIN");
    smooth->axis[0] = (uint16)(0);
    smooth->axis[1] = (uint16)(0);
    smooth->axis[2] = (uint16)(0);
    smooth->thrust = (uint16)(2048);
    smooth->brake = (uint16)(0);

    smooth->emitter_bits = (uint32)(0);
    index = selection->actor_idx;
    actor_index_table = 0;
    pose_owner_target = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    rotation_build(&matrix, pose_owner_target->pose.angles);
    value = selection->offset[0];
    x = (sint32)value >> 6;
    value = selection->offset[1];
    y = (sint32)value >> 6;
    value = selection->offset[2];
    z = (sint32)(value + 0xFFF9C000) >> 6;
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    value = pose_owner_target->pose.position[0];
    native_object->pose.position[0] = (uint32)(value + (rx << 6));
    value = pose_owner_target->pose.position[1];
    native_object->pose.position[1] = (uint32)(value + (ry << 6));
    value = pose_owner_target->pose.position[2];
    native_object->pose.position[2] = (uint32)(value + (rz << 6));
    index = selection->actor_idx;
    actor_index_table = 0;
    actor_motion_value = mission_actor_motion((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    vx = actor_motion_value->velocity[0];
    vy = actor_motion_value->velocity[1];
    vz = actor_motion_value->velocity[2];
    speed = game_vector_length_gte((sint32)vx >> 6, (sint32)vy >> 6, (sint32)vz >> 6) << 6;
    z = (sint32)speed >> 5;
    state->speed = (uint32)(speed);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    actor_motion->velocity[0] = (uint32)(rx << 5);
    actor_motion->velocity[1] = (uint32)(ry << 5);
    actor_motion->velocity[2] = (uint32)(rz << 5);
    value = pose_owner_target->pose.angles[0];
    native_object->pose.angles[0] = (uint16)(value);
    value = pose_owner_target->pose.angles[1];
    native_object->pose.angles[1] = (uint16)(value);
    value = pose_owner_target->pose.angles[2];
    native_object->pose.angles[2] = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_update_target_option(FLIGHT_DYNAMICS *state, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    MATRIX matrix;
    uint32 a, b, c, d, e, rx, ry, rz, y, z, value;
    FUNCTION_MARKER(0x80075E08, "ACE3.BIN");
    flight_dynamics_apply_target_pose(state, target, native_object);
    flight_orientation.response[0] = (uint16)(0);
    flight_orientation.response[1] = (uint16)(0);
    flight_orientation.response[2] = (uint16)(0);
    flight_orientation.angle_delta.value[0] = (uint16)(0);
    flight_orientation.angle_delta.value[1] = (uint16)(0);
    flight_orientation.angle_delta.value[2] = (uint16)(0);
    state->control_gain = (uint16)(4096);
    control->pitch = (uint16)(0);
    control->yaw = (uint16)(0);
    control->roll = (uint16)(0);

    control->thrust = (uint16)(4096);
    smooth->thrust = (uint16)(4096);
    smooth->axis[0] = (uint16)(0);
    smooth->axis[1] = (uint16)(0);
    smooth->axis[2] = (uint16)(0);
    smooth->brake = (uint16)(0);
    state->speed = (uint32)(0x20000);
    state->ticks = (uint32)(0);
    rotation_build(&matrix, native_object->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    rx <<= 5;
    ry <<= 5;
    rz <<= 5;
    actor_motion->velocity[0] = (uint32)(rx);
    actor_motion->velocity[1] = (uint32)(ry);
    actor_motion->velocity[2] = (uint32)(rz);
    if (option != 0)
    {
        y = 0 - 2000;
        z = 5000;
    }
    else
    {
        y = 0 - 3500;
        z = 13000;
    }
    xport_gte_write_data(0, y << 16);
    xport_gte_write_data(1, z);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    rx <<= 12;
    ry <<= 12;
    rz <<= 12;
    value = native_object->pose.position[0];
    native_object->pose.position[0] = (uint32)(value + rx);
    value = native_object->pose.position[1];
    native_object->pose.position[1] = (uint32)(value + ry);
    value = native_object->pose.position[2];
    native_object->pose.position[2] = (uint32)(value + rz);
    {
        (void)(flight_orientation_update_delta(&flight_orientation, native_object));
        return;
    }
}

uint32 flight_control_update_target(FLIGHT_DYNAMICS *state, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    uint32 phase, mode, height, yaw, angle, sine, cosine, x, y, z, vx, vz, value, ticks, ratio, amplitude, pitch, numerator, flags;
    FUNCTION_MARKER(0x80074BA8, "ACE3.BIN");
    smooth->axis[0] = (uint16)(0);
    smooth->axis[1] = (uint16)(0);
    smooth->axis[2] = (uint16)(0);
    smooth->thrust = (uint16)(2048);
    smooth->brake = (uint16)(0);

    smooth->emitter_bits = (uint32)(0);
    phase = state->phase;
    if (phase == 0)
    {
        mode = mission_flow.flight_selection.mode;
        height = mode != 0 ? 0xFFD26000 : 0xFFCC2000;
        yaw = (sint16)target->angles[1];
        angle = yaw & 0x3FFF;
        sine = (uint16)game_wave_at((angle << 1));
        sine = (sint32)(sine << 16) >> 18;
        cosine = (uint16)game_wave_at(((angle + 0x1000) << 1));
        cosine = (sint32)(cosine << 16) >> 18;
        x = target->position[0];
        z = target->position[2];
        y = target->position[1];
        native_object->pose.position[0] = (uint32)(x + sine * (uint32)-1920);
        vx = sine << 4;
        native_object->pose.position[1] = (uint32)(height + y);
        native_object->pose.position[2] = (uint32)(z + cosine * (uint32)-1920);
        vz = cosine << 4;
        value = rsin(-318);
        value = (sint32)((0 - value) << 16) >> 12;
        actor_motion->velocity[1] = (uint32)(value);
        native_object->pose.angles[0] = (uint16)(-273);
        actor_motion->velocity[0] = (uint32)(vx);
        actor_motion->velocity[2] = (uint32)(vz);
        native_object->pose.angles[1] = (uint16)(yaw);
        native_object->pose.angles[2] = (uint16)(0);
        state->speed = (uint32)(0x10000);
        state->ticks = (uint32)(0);
        state->phase = (uint32)(1);
    }
    else if (phase == 1)
    {
        ticks = state->ticks + 1;
        state->ticks = (uint32)(ticks);
        if ((sint32)ticks >= 31)
        {
            state->ticks = (uint32)(0);
            state->phase = (uint32)(2);
        }
    }
    else if (phase == 2)
    {
        ticks = state->ticks;
        if ((sint32)ticks < 76)
        {
            numerator = ticks << 12;
            ratio = (sint32)numerator / 75;
        }
        else
            ratio = 4096;
        value = rsin(((sint32)ratio >> 1) + 1024);
        amplitude = ((uint32)((sint32)value >> 1)) + 2048;
        numerator = 0 - amplitude * 182;
        pitch = 0 - amplitude * 409;
        value = (uint32)((sint32)numerator / 4096) - 136;
        pitch = (uint32)((sint32)pitch / 4096) + 136;
        value = rsin(value);
        value = (0 - value) * 0x10000;
        native_object->pose.angles[0] = (uint16)(pitch);
        actor_motion->velocity[1] = (uint32)((sint32)value >> 12);
        ticks = state->ticks;
        if ((sint32)ticks >= 121)
            return 1;
        flags = state->events;
        if ((flags & 9) != 0)
            return 1;
        state->ticks = (uint32)(ticks + 1);
    }
    game_vector_angles(actor_motion->velocity, flight_orientation.velocity_angles.value);
    flight_orientation_update_delta(&flight_orientation, native_object);
    return 0;
}

uint32 flight_control_update_target_phase(FLIGHT_DYNAMICS *state, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object)
{
    FLIGHT_DYNAMICS *voice_state;
    MATRIX matrix;
    uint32 phase, ticks, value, voice, acceleration, a, b, c, d, e, rx, ry, rz, vx, vy, vz, x, y, z;
    FUNCTION_MARKER(0x80075B04, "ACE3.BIN");
    phase = state->phase;
    if (phase == 0)
    {
        flight_dynamics_apply_target_pose(state, target, native_object);
        state->speed = (uint32)(0);
        actor_motion->velocity[0] = (uint32)(0);
        actor_motion->velocity[1] = (uint32)(0);
        actor_motion->velocity[2] = (uint32)(0);
        flight_orientation.response[0] = (uint16)(0);
        flight_orientation.response[1] = (uint16)(0);
        flight_orientation.response[2] = (uint16)(0);
        flight_orientation.angle_delta.value[0] = (uint16)(0);
        flight_orientation.angle_delta.value[1] = (uint16)(0);
        flight_orientation.angle_delta.value[2] = (uint16)(0);
        state->control_gain = (uint16)(0);
        control->pitch = (uint16)(0);
        control->yaw = (uint16)(0);
        control->roll = (uint16)(0);

        control->thrust = (uint16)(0xF000);
        smooth->axis[0] = (uint16)(0);
        smooth->axis[1] = (uint16)(0);
        smooth->axis[2] = (uint16)(0);
        smooth->thrust = (uint16)(0xF000);
        smooth->brake = (uint16)(0);
        state->ticks = (uint32)(0);
        state->phase = (uint32)(1);
    }
    else if (phase == 1)
    {
        ticks = state->ticks;
        value = ticks << 13;
        value = (uint32)((sint32)value / 90) - 4096;
        control->thrust = (uint16)(value);
        if (((sint32)(value << 16) >> 16) >= 4097)
            control->thrust = (uint16)(4096);
        value = control->thrust;
        smooth->thrust = (uint16)(value);
        ticks = state->ticks + 1;
        state->ticks = (uint32)(ticks);
        if ((sint32)ticks >= 91)
        {
            voice_state = &flight_dynamics;
            state->phase = (uint32)(2);
            state->ticks = (uint32)(0);
            voice = (sint16)voice_state->voice;
            if ((sint32)voice >= 0)
                sound_stop_voice(voice);
            voice = sound_play(92);
            voice_state = &flight_dynamics;
            voice_state->voice = (uint16)(voice);
        }
    }
    else if (phase == 2)
    {
        acceleration = option != 0 ? 780 : 284;
        value = state->speed;
        state->speed = (uint32)(value + acceleration);
        rotation_build(&matrix, native_object->pose.angles);
        a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
        b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
        c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
        d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
        e = ((uint16)matrix.m[2][2]);
        SetRotMatrix(&matrix);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, acceleration & 0xFFFF);
        xport_gte_execute(0x486012);
        rx = xport_gte_read_data(25);
        ry = xport_gte_read_data(26);
        rz = xport_gte_read_data(27);
        value = actor_motion->velocity[0];
        actor_motion->velocity[0] = (uint32)(value + rx);
        value = actor_motion->velocity[1];
        vx = actor_motion->velocity[0];
        value += ry;
        vz = actor_motion->velocity[2];
        actor_motion->velocity[1] = (uint32)(value);
        x = native_object->pose.position[0];
        x += vx;
        native_object->pose.position[0] = (uint32)(x);
        y = native_object->pose.position[1];
        vy = actor_motion->velocity[1];
        vz += rz;
        actor_motion->velocity[2] = (uint32)(vz);
        z = native_object->pose.position[2];
        value = actor_motion->velocity[2];
        y += vy;
        z += value;
        native_object->pose.position[1] = (uint32)(y);
        native_object->pose.position[2] = (uint32)(z);
        value = state->speed;
        if ((sint32)value > 0xF000)
        {
            voice_state = &flight_dynamics;
            state->phase = (uint32)(3);
            state->ticks = (uint32)(0);
            voice = (sint16)voice_state->voice;
            if ((sint32)voice >= 0)
                sound_stop_voice(voice);
        }
    }
    else if (phase == 3)
        return 1;
    flight_orientation_update_delta(&flight_orientation, native_object);
    return 0;
}

void flight_control_update_object_targets(FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const MISSION_ACTOR *native_object)
{
    uint16 *address;
    uint32 pitch_target, bank_target, thrust_target, roll, absolute_roll, value, magnitude, pitch_limit, bank_limit, numerator, steps[3], targets[3], index, raw;
    FUNCTION_MARKER(0x8007407C, "ACE3.BIN");
    thrust_target = 0;
    pitch_target = 0;
    roll = (sint16)native_object->pose.angles[2];
    bank_target = (0 - roll) << 2;
    absolute_roll = roll;
    if ((sint32)roll < 0)
        absolute_roll = 0 - roll;
    if ((sint32)absolute_roll < 2048)
    {
        value = (sint16)native_object->pose.angles[0];
        if (value != 2048)
            pitch_target = (2048 - value) << 4;
        thrust_target = 4096;
    }
    value = (sint16)native_object->pose.angles[2];
    absolute_roll = value;
    if ((sint32)value < 0)
        absolute_roll = 0 - value;
    if ((sint32)absolute_roll < 455)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)absolute_roll < 273)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)absolute_roll < 136)
        bank_target = (sint32)bank_target >> 1;
    if ((sint32)pitch_target < -4096)
        pitch_target = 0 - (uint32)4096;
    else if ((sint32)pitch_target >= 4097)
        pitch_target = 4096;
    if ((sint32)bank_target < -4096)
        bank_target = 0 - (uint32)4096;
    else if ((sint32)bank_target >= 4097)
        bank_target = 4096;
    pitch_limit = pitch_target;
    if ((sint32)pitch_target < 0)
        pitch_limit = 0 - pitch_target;
    bank_limit = bank_target;
    if ((sint32)bank_target < 0)
        bank_limit = 0 - bank_target;
    value = (sint16)output->axis[0];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)pitch_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[0] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[1];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if (magnitude != 0)
    {
        numerator = value << 7;
        output->axis[1] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[2];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)bank_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[2] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[3];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)thrust_target < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[3] = (uint16)((sint32)numerator / 140);
    }
    targets[0] = bank_target;
    targets[1] = pitch_target;
    targets[2] = 0;
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = &smooth->axis[index];
        value = (sint16)*address;
        raw = *address;
        if ((sint32)targets[index] < (sint32)value)
            *address = (raw - steps[index]);
        value = (sint16)*address;
        raw = *address;
        if ((sint32)value < (sint32)targets[index])
            *address = (raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            smooth->axis[index] = (uint16)(targets[index]);
    }
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < 0)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value > 0)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)output->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    value = output->axis[0];
    raw = output->axis[1];
    output->axis[0] = (uint16)(value + (uint32)((sint32)pitch_target >> 3));
    value = output->axis[2];
    output->axis[2] = (uint16)(value + (uint32)((sint32)bank_target >> 5));
    value = output->axis[3];
    output->axis[3] = (uint16)(value + (uint32)((sint32)thrust_target >> 5));
    value = (sint16)output->axis[0];
    if ((sint32)value < (sint32)(0 - pitch_limit))
        output->axis[0] = (uint16)(0 - pitch_limit);
    value = (sint16)output->axis[0];
    if ((sint32)pitch_limit < (sint32)value)
        output->axis[0] = (uint16)(pitch_limit);
    value = (sint16)output->axis[1];
    if ((sint32)value < 0)
        output->axis[1] = (uint16)(0);
    value = (sint16)output->axis[1];
    if ((sint32)value > 0)
        output->axis[1] = (uint16)(0);
    value = (sint16)output->axis[2];
    if ((sint32)value < (sint32)(0 - bank_limit))
        output->axis[2] = (uint16)(0 - bank_limit);
    value = (sint16)output->axis[2];
    if ((sint32)bank_limit < (sint32)value)
        output->axis[2] = (uint16)(bank_limit);
    value = (sint16)output->axis[3];
    if ((sint32)value < -4096)
        output->axis[3] = (uint16)(0xF000);
    value = (sint16)output->axis[3];
    if ((sint32)value >= 4097)
        output->axis[3] = (uint16)(4096);
    value = output->axis[0];
    control->pitch = (uint16)(value);
    value = output->axis[1];
    control->yaw = (uint16)(value);
    value = output->axis[2];
    control->roll = (uint16)(value);
    value = output->axis[3];
    control->thrust = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_update_input(const GAME_INPUT *input, FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint16 *address;
    uint32 buttons, flags, value, raw, magnitude, fallback, pitch, yaw, roll, thrust, pitch_limit, yaw_limit, roll_limit, targets[4], limits[3], smoothing[3], steps[3], index, numerator, desired, result;
    FUNCTION_MARKER(0x8006D678, "ACE3.BIN");
    actor_index_table = 0;
    buttons = input->held;
    pose_owner_object = mission_actor(actor_index_table);
    if ((sint32)buttons < 0)
    {
        fallback = game_runtime.display_buffer != 0;
        if (fallback == 0)
        {
            value = (sint16)pose_owner_object->pose.angles[2];
            if ((sint32)value < 0)
                value = 0 - value;
            fallback = (sint32)value >= 547;
        }
        if (fallback != 0)
        {
            flags = flight_dynamics.mode;
            if (flags == 0x01000000)
            {
                (void)(flight_control_update_object_input(output, control, smooth, 0, pose_owner_object));
                return;
            }
            {
                (void)(flight_control_update_object_targets(output, control, smooth, pose_owner_object));
                return;
            }
        }
    }
    pitch_limit = 4096;
    yaw_limit = 4096;
    roll_limit = 4096;
    pitch = 0;
    yaw = 0;
    roll = 0;
    thrust = 0;
    flags = input->axis_mask;
    buttons = input->held;
    if ((flags & 1) != 0)
    {
        pitch = (uint32)input->axis[0];
        pitch_limit = pitch;
        if ((sint32)pitch < 0)
            pitch_limit = 0 - pitch;
    }
    else
    {
        if ((buttons & 0x00800000) != 0)
            pitch = 4096;
        if ((buttons & 0x01000000) != 0)
            pitch -= 4096;
    }
    flags = input->axis_mask;
    if ((flags & 4) != 0)
    {
        yaw = (uint32)input->axis[2];
        yaw_limit = yaw;
        if ((sint32)yaw < 0)
            yaw_limit = 0 - yaw;
    }
    else
    {
        if ((buttons & 0x20000000) != 0)
            yaw -= yaw_limit;
        if ((buttons & 0x40000000) != 0)
            yaw += yaw_limit;
    }
    flags = input->axis_mask;
    if ((flags & 2) != 0)
    {
        roll = (uint32)input->axis[1];
        roll_limit = roll;
        if ((sint32)roll < 0)
            roll_limit = 0 - roll;
    }
    else
    {
        if ((buttons & 0x02000000) != 0)
            roll -= roll_limit;
        if ((buttons & 0x04000000) != 0)
            roll += roll_limit;
    }
    flags = input->axis_mask;
    if ((flags & 8) != 0)
        thrust = (uint32)input->axis[3];
    else
    {
        if ((buttons & 0x08000000) != 0)
            thrust += 4096;
        if ((buttons & 0x10000000) != 0)
            thrust -= 4096;
    }
    targets[0] = pitch;
    targets[1] = yaw;
    targets[2] = roll;
    targets[3] = thrust;
    for (index = 0; index < 4; index += 1)
    {
        value = (sint16)output->axis[index];
        magnitude = targets[index];
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        raw = value;
        if ((sint32)value < 0)
            raw = 0 - value;
        if ((sint32)magnitude < (sint32)raw)
        {
            numerator = value << 7;
            output->axis[index] = (uint16)((sint32)numerator / (index == 3 ? 140 : 220));
        }
    }
    smoothing[0] = roll;
    smoothing[1] = pitch;
    smoothing[2] = yaw;
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - smoothing[index];
        if ((sint32)magnitude < 0)
            magnitude = smoothing[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = &smooth->axis[index];
        value = (sint16)*address;
        raw = *address;
        if ((sint32)smoothing[index] < (sint32)value)
            *address = (raw - steps[index]);
        value = (sint16)*address;
        raw = *address;
        if ((sint32)value < (sint32)smoothing[index])
            *address = (raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - smoothing[index];
        if ((sint32)magnitude < 0)
            magnitude = smoothing[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            smooth->axis[index] = (uint16)(smoothing[index]);
    }
    desired = 0;
    if ((sint32)thrust < -2048)
        desired = (0 - (thrust + 2048)) << 1;
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < (sint32)desired)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)desired < (sint32)value)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)output->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    for (index = 0; index < 4; index += 1)
    {
        value = output->axis[index];
        output->axis[index] = (uint16)(value + (uint32)((sint32)targets[index] >> 5));
    }
    limits[0] = pitch_limit;
    limits[1] = yaw_limit;
    limits[2] = roll_limit;
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)output->axis[index];
        if ((sint32)value < (sint32)(0 - limits[index]))
            output->axis[index] = (uint16)(0 - limits[index]);
        value = (sint16)output->axis[index];
        if ((sint32)limits[index] < (sint32)value)
            output->axis[index] = (uint16)(limits[index]);
    }
    value = (sint16)output->axis[3];
    if ((sint32)value < -4096)
        output->axis[3] = (uint16)(0xF000);
    value = (sint16)output->axis[3];
    if ((sint32)value >= 4097)
        output->axis[3] = (uint16)(4096);
    value = output->axis[3];
    control->thrust = (uint16)(value);
    value = output->axis[0];
    control->pitch = (uint16)(value);
    flags = game_input.flags;
    if ((flags & 4) != 0)
    {
        value = output->axis[2];
        control->yaw = (uint16)(value);
        value = (sint16)output->axis[2];
        raw = output->axis[1];
        value = raw + (uint32)((sint32)value >> 5);
        control->roll = (uint16)(value);
        if (((sint32)(value << 16) >> 16) >= 4097)
            control->roll = (uint16)(4096);
        value = (sint16)control->roll;
        if ((sint32)value < -4096)
            control->roll = (uint16)(0xF000);
        actor_index_table = 0;
        pose_owner_object = mission_actor(actor_index_table);
        value = (sint16)pose_owner_object->pose.angles[2];
        raw = pose_owner_object->pose.angles[2];
        if ((sint32)value >= 13)
            pose_owner_object->pose.angles[2] = (uint16)(raw - 12);
        value = (sint16)pose_owner_object->pose.angles[2];
        raw = pose_owner_object->pose.angles[2];
        result = raw + 12;
        if ((sint32)value < -12)
            pose_owner_object->pose.angles[2] = (uint16)(result);
        {
            (void)(uint32)(result);
            return;
        }
    }
    value = output->axis[1];
    control->yaw = (uint16)(value);
    result = output->axis[2];
    control->roll = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void flight_control_update_phase(FLIGHT_DYNAMICS *state, FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const MISSION_ACTOR *native_object)
{
    uint16 *address;
    uint32 pitch_target, bank_target, thrust_target, thrust_limit, phase, ticks, value, magnitude, pitch_limit, bank_limit, numerator, steps[3], targets[3], index, raw, desired;
    FUNCTION_MARKER(0x8007542C, "ACE3.BIN");
    thrust_target = 0;
    bank_target = 0;
    pitch_target = 0;
    phase = state->phase;
    if (phase == 0)
    {
        state->phase = (uint32)(1);
        state->ticks = (uint32)(0);
        output->axis[0] = (uint16)(0);
        output->axis[1] = (uint16)(0);
        output->axis[2] = (uint16)(0);
        output->axis[3] = (uint16)(0);
    }
    else if (phase == 1)
    {
        ticks = state->ticks;
        thrust_target = 0 - (uint32)2048;
        if ((sint32)ticks < 76)
            state->ticks = (uint32)(ticks + 1);
        else
        {
            state->phase = (uint32)(2);
            state->ticks = (uint32)(0);
        }
    }
    else if (phase == 2)
    {
        thrust_target = 0 - (uint32)2048;
        value = (sint16)native_object->pose.angles[2];
        ticks = state->ticks;
        bank_target = (1820 - value) << 1;
        if ((sint32)ticks < 121)
            state->ticks = (uint32)(ticks + 1);
        else
        {
            state->phase = (uint32)(3);
            state->ticks = (uint32)(0);
        }
    }
    else if (phase == 3)
    {
        value = (sint16)native_object->pose.angles[2];
        raw = (sint16)native_object->pose.angles[0];
        bank_target = (0 - value) << 1;
        pitch_target = (0 - raw) << 4;
    }
    if ((sint32)pitch_target < -4096)
        pitch_target = 0 - (uint32)4096;
    else if ((sint32)pitch_target >= 4097)
        pitch_target = 4096;
    if ((sint32)bank_target < -4096)
        bank_target = 0 - (uint32)4096;
    else if ((sint32)bank_target >= 4097)
        bank_target = 4096;
    pitch_limit = pitch_target;
    if ((sint32)pitch_target < 0)
        pitch_limit = 0 - pitch_target;
    bank_limit = bank_target;
    if ((sint32)bank_target < 0)
        bank_limit = 0 - bank_target;
    thrust_limit = thrust_target;
    if ((sint32)thrust_target < 0)
        thrust_limit = 0 - thrust_target;
    value = (sint16)output->axis[0];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)pitch_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[0] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[1];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if (magnitude != 0)
    {
        numerator = value << 7;
        output->axis[1] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[2];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)bank_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[2] = (uint16)((sint32)numerator / 200);
    }
    value = (sint16)output->axis[3];
    magnitude = value;
    if ((sint32)value < 0)
        magnitude = 0 - value;
    if ((sint32)thrust_limit < (sint32)magnitude)
    {
        numerator = value << 7;
        output->axis[3] = (uint16)((sint32)numerator / 140);
    }
    targets[0] = bank_target;
    targets[1] = pitch_target;
    targets[2] = 0;
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        steps[index] = (uint32)((sint32)magnitude >> 2) + 2;
    }
    for (index = 0; index < 3; index += 1)
    {
        address = &smooth->axis[index];
        value = (sint16)*address;
        raw = *address;
        if ((sint32)targets[index] < (sint32)value)
            *address = (raw - steps[index]);
        value = (sint16)*address;
        raw = *address;
        if ((sint32)value < (sint32)targets[index])
            *address = (raw + steps[index]);
    }
    for (index = 0; index < 3; index += 1)
    {
        value = (sint16)smooth->axis[index];
        magnitude = value - targets[index];
        if ((sint32)magnitude < 0)
            magnitude = targets[index] - value;
        if ((sint32)magnitude < (sint32)steps[index])
            smooth->axis[index] = (uint16)(targets[index]);
    }
    desired = 0;
    if ((sint32)thrust_target < -2048)
        desired = (0 - (thrust_target + 2048)) << 1;
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)value < (sint32)desired)
        smooth->brake = (uint16)(raw + 128);
    value = (sint16)smooth->brake;
    raw = smooth->brake;
    if ((sint32)desired < (sint32)value)
        smooth->brake = (uint16)(raw - 128);
    value = (sint16)output->axis[3];
    smooth->thrust = (uint16)((sint32)(value + 4096) >> 1);
    value = output->axis[0];
    raw = output->axis[1];
    output->axis[0] = (uint16)(value + (uint32)((sint32)pitch_target >> 3));
    value = output->axis[2];
    output->axis[2] = (uint16)(value + (uint32)((sint32)bank_target >> 5));
    value = output->axis[3];
    output->axis[3] = (uint16)(value + (uint32)((sint32)thrust_target >> 5));
    value = (sint16)output->axis[0];
    if ((sint32)value < (sint32)(0 - pitch_limit))
        output->axis[0] = (uint16)(0 - pitch_limit);
    value = (sint16)output->axis[0];
    if ((sint32)pitch_limit < (sint32)value)
        output->axis[0] = (uint16)(pitch_limit);
    value = (sint16)output->axis[1];
    if ((sint32)value < 0)
        output->axis[1] = (uint16)(0);
    value = (sint16)output->axis[1];
    if ((sint32)value > 0)
        output->axis[1] = (uint16)(0);
    value = (sint16)output->axis[2];
    if ((sint32)value < (sint32)(0 - bank_limit))
        output->axis[2] = (uint16)(0 - bank_limit);
    value = (sint16)output->axis[2];
    if ((sint32)bank_limit < (sint32)value)
        output->axis[2] = (uint16)(bank_limit);
    value = (sint16)output->axis[3];
    if ((sint32)value < -4096)
        output->axis[3] = (uint16)(0xF000);
    value = (sint16)output->axis[3];
    if ((sint32)value >= 4097)
        output->axis[3] = (uint16)(4096);
    value = output->axis[0];
    control->pitch = (uint16)(value);
    value = output->axis[1];
    control->yaw = (uint16)(value);
    value = output->axis[2];
    control->roll = (uint16)(value);
    value = output->axis[3];
    control->thrust = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void flight_control_scale_command(uint32 value, FLIGHT_COMMAND *control)
{
    uint32 factor, x, y, z;
    FUNCTION_MARKER(0x8006E7E0, "ACE3.BIN");
    if ((sint32)value > 0x14FFF)
        factor = 4096;
    else
        factor = (sint32)value / 21;
    x = (sint16)control->pitch;
    y = (sint16)control->yaw;
    z = (sint16)control->roll;
    x = (sint32)(x * factor) / 4096;
    y = (sint32)(y * factor) / 4096;
    z = (sint32)(z * factor) / 4096;
    control->pitch = (uint16)(x);
    control->yaw = (uint16)(y);
    control->roll = (uint16)(z);
    {
        (void)(uint32)(y);
        return;
    }
}

void flight_control_update_for_model(const FLIGHT_AIRCRAFT_PROFILE *settings, MISSION_ACTOR *native_object)
{
    FLIGHT_DYNAMICS *physics;
    FLIGHT_COMMAND *output;
    FLIGHT_INPUT *control;
    FLIGHT_CONTROL *smooth;
    uint32 flags;
    FUNCTION_MARKER(0x80072938, "ACE3.BIN");
    flags = game_input.held;
    if ((sint32)flags < 0)
    {
        control = &flight_input;
        output = &flight_command;
        smooth = &flight_control;
        {
            (void)(flight_control_update_object_input(control, output, smooth, 0, native_object));
            return;
        }
    }
    flags = game_input.flags;
    if ((flags & 4) != 0)
    {
        control = &flight_input;
        output = &flight_command;
        smooth = &flight_control;
        physics = &flight_dynamics;
        {
            (void)(flight_control_update_physics(&game_input, control, output, smooth, settings, physics, native_object));
            return;
        }
    }
    control = &flight_input;
    output = &flight_command;
    smooth = &flight_control;
    {
        (void)(flight_control_update_standard(&game_input, control, output, smooth));
        return;
    }
}

void flight_initialize_control_storage(void)
{
    uint32 state, index;
    FUNCTION_MARKER(0x8006CCE0, "ACE3.BIN");
    state = arena_alloc(8);
    for (index = 0; index < 4; ++index)
        flight_input.axis[index] = 0;
    state = arena_alloc(16);
    /* preserve the numeric effect phase and arena positions until allocator migration */
    flight_control_initialize(state);
    state = arena_alloc(16);
    state = arena_alloc(112);
    state = arena_alloc(60);
    state = arena_alloc(56);
    {
        (void)(uint32)(state);
        return;
    }
}
