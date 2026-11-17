#include "game_functions.h"

const FLIGHT_TARGET_PROFILE flight_target_profiles[FLIGHT_TARGET_PROFILE_COUNT] =
{
    {{0xF0A1E000, 0xFFFB4000, 0xE4BFC000}, {0xFF78, 0x1000, 0x0000}, {{0xF0898000, 0xE4B0E000}, {0x01F0F000, 0x001E5000}}, {{0xF04A7000, 0xE4857000}, {0x02301000, 0x00738000}}},
    {{0xFB400000, 0xFFB9B000, 0xF7400000}, {0xFF78, 0x0000, 0x0000}, {{0xFB30E000, 0xF70E3000}, {0x00203000, 0x02722000}}, {{0xFB000000, 0xF7000000}, {0x00800000, 0x02800000}}},
    {{0x02A43000, 0x00000000, 0xFD1A8000}, {0xFF78, 0x0000, 0x0000}, {{0x02980000, 0xFD001000}, {0x00180000, 0x027FF000}}, {{0x02412000, 0xFD1A8000}, {0x00BE1000, 0x02658000}}},
    {{0xFFA6A000, 0xFFE67000, 0x08594000}, {0xFF78, 0x0000, 0x0000}, {{0xFF999000, 0x08403000}, {0x0019B000, 0x023FB000}}, {{0xFF800000, 0x08403000}, {0x00533000, 0x023FB000}}},
    {{0x00D8B000, 0xFFE67000, 0x0A537000}, {0xFF78, 0xE000, 0x0000}, {{0x00CCD000, 0x08402000}, {0x00198000, 0x023E1000}}, {{0x00AD0000, 0x08402000}, {0x00521000, 0x023E1000}}},
    {{0xFE236000, 0xFFB9C000, 0xF123F000}, {0xFF78, 0xF000, 0x0000}, {{0xFAAB3000, 0xF1144000}, {0x03BBE000, 0x001CA000}}, {{0xFAAB3000, 0xF0FFF000}, {0x03D4D000, 0x00800000}}},
    {{0xFB429000, 0xFFB9C000, 0xF15C9000}, {0xFF78, 0x1000, 0x0000}, {{0xFAAB3000, 0xF14F4000}, {0x03BDD000, 0x001BD000}}, {{0xFAAB3000, 0xF0FFF000}, {0x03D4D000, 0x00800000}}},
    {{0xDA72F000, 0xFFF99000, 0x24807000}, {0xFF78, 0x1000, 0x0000}, {{0xDA602000, 0x247C2000}, {0x003FD000, 0x00088000}}, {{0xDA5FA000, 0x24694000}, {0x00601000, 0x00200000}}},
    {{0x307F9000, 0xFFF99000, 0x0F6B0000}, {0xFF78, 0x0000, 0x0000}, {{0x307B5000, 0x0F600000}, {0x0008B000, 0x00400000}}, {{0x30767000, 0x0F5F9000}, {0x00204000, 0x00601000}}},
    {{0xDA72F000, 0xFFF99000, 0x24807000}, {0xFF78, 0x1000, 0x0000}, {{0xDA602000, 0x247C2000}, {0x003FD000, 0x00088000}}, {{0xDA5FA000, 0x24694000}, {0x00601000, 0x00200000}}},
    {{0x00000000, 0x00000000, 0x00000000}, {0x071C, 0x0000, 0x0000}, {{0x00000000, 0x00000000}, {0x00000000, 0x00000000}}, {{0x00000000, 0x00000000}, {0x00000000, 0x00000000}}},
    {{0x327FA000, 0xFFF4B000, 0xFB6F1000}, {0xFF78, 0x0000, 0x0000}, {{0x327B8000, 0xFB59E000}, {0x00087000, 0x004C4000}}, {{0x32733000, 0xFB59E000}, {0x00190000, 0x004C4000}}},
    {{0xDA911000, 0xFFF99000, 0x247B3000}, {0x0000, 0x11D1, 0x0000}, {{0xDA602000, 0x247C2000}, {0x003FD000, 0x00088000}}, {{0xDA5FA000, 0x24694000}, {0x00601000, 0x00200000}}},
};

FLIGHT_TARGET_STATE flight_target;

uint32 flight_target_set_primary_mode(uint32 mode, uint32 kind)
{
    FLIGHT_DYNAMICS *state;
    static const uint32 table[6] = {0x8, 0x8, 0x10, 0x10, 0x20, 0x40};
    FLIGHT_TARGET_STATE *output;
    const FLIGHT_TARGET_PROFILE *target;
    uint32 index, value, flags, i;
    FUNCTION_MARKER(0x80076B18, "ACE3.BIN");
    index = (kind << 1) + mode;
    value = table[index];
    if (value == 8)
    {
        state = &flight_dynamics;
        state->flags = (uint32)(4);
        value = table[index];
    }
    if (value == 16)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->flags = (uint32)(flags | 64);
    }
    else if (value == 32 || value == 64)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->flags = (uint32)((flags | 512) & 0xFFFFFFBF);
    }
    target = mission_flow.flight_selection.target;
    output = &flight_target;
    for (i = 0; i < 3; ++i)
    {
        value = target->position[i];
        output->reference_position.word[i] = (uint32)(value);
    }
    value = target->angles[0];
    output->reference_angles.value[0] = (uint16)(value);
    value = target->angles[1];
    output->reference_angles.value[1] = (uint16)(value);
    value = target->angles[2];
    state = &flight_dynamics;
    output->reference_angles.value[2] = (uint16)(value);
    value = table[index];
    state->phase = (uint32)(0);
    state->mode = (uint32)(value);
    return value;
}

uint32 flight_target_set_secondary_mode(uint32 mode, uint32 kind)
{
    FLIGHT_DYNAMICS *state;
    static const uint32 table[8] = {0x100, 0x100, 0x200, 0x200, 0x400, 0x400, 0x800, 0x1000};
    FLIGHT_TARGET_STATE *output;
    uint32 index, value, flags;
    FUNCTION_MARKER(0x80076C54, "ACE3.BIN");
    index = (kind << 1) + mode;
    value = table[index];
    if (value == 256)
    {
        state = &flight_dynamics;
        state->flags = (uint32)(8);
        value = table[index];
    }
    if (value == 512 || value == 1024 || value == 4096)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->flags = (uint32)(flags | 64);
    }
    else if (value == 2048)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->flags = (uint32)(flags & 0xFFFFFFBF);
    }
    value = table[index];
    if (value == 2048 || value == 4096)
    {
        state = &flight_dynamics;
        flags = state->flags;
        state->flags = (uint32)(flags | 512);
    }
    output = &flight_target;
    state = &flight_dynamics;
    output->reference_position.word[0] = (uint32)(0);
    output->reference_position.word[1] = (uint32)(0);
    output->reference_position.word[2] = (uint32)(0);
    output->reference_angles.value[0] = (uint16)(0);
    output->reference_angles.value[1] = (uint16)(0);
    output->reference_angles.value[2] = (uint16)(0);
    value = table[index];
    state->phase = (uint32)(0);
    state->mode = (uint32)(value);
    return value;
}


uint32 flight_target_set_tertiary_mode(uint32 mode, uint32 kind)
{
    FLIGHT_DYNAMICS *state;
    static const uint32 modes[6] = {0x4000, 0x8000, 0x10000, 0x10000, 0x4, 0x4};
    uint32 index, value, voice;
    FUNCTION_MARKER(0x80076DEC, "ACE3.BIN");
    index = kind * 2 + mode;
    value = modes[index];
    if (value == 0x4000 || value == 0x8000)
    {
        state = &flight_dynamics;
        state->voice = (uint16)(0xFFFF);
    }
    state = &flight_dynamics;
    state->flags = (uint32)(2);
    value = modes[index];
    if (value == 0x10000)
    {
        voice = (sint16)state->voice;
        state->flags = (uint32)(0x42);
        if ((sint32)voice >= 0)
            sound_stop_voice(voice);
    }
    else if (value == 4)
    {
        flight_dynamics_enter_standard_control();
        state = &flight_dynamics;
        value = state->flags;
        voice = (sint16)state->voice;
        state->flags = (uint32)(value | 0x280);
        if ((sint32)voice >= 0)
            sound_stop_voice(voice);
    }
    state = &flight_dynamics;
    value = modes[index];
    state->mode = (uint32)(value);
    state->phase = (uint32)(0);
    return value;
}

uint32 flight_target_classify_landing(const FLIGHT_TARGET_PROFILE *target, const NATIVE_POSE *native_position)
{
    uint32 pitch, roll, x, y, lower, z, span, result, yaw, target_yaw, delta;
    FUNCTION_MARKER(0x80076428, "ACE3.BIN");
    pitch = ((((sint16)native_position->angles[0])));
    roll = ((((sint16)native_position->angles[2])));
    if ((sint32)pitch >= 683 || (sint32)pitch < -364)
        return 0;
    if ((sint32)roll < 0)
        roll = 0 - roll;
    if ((sint32)roll >= 1366)
        return 0;
    x = ((((uint32)native_position->position[0])));
    y = ((((uint32)native_position->position[1])));
    lower = target->outer.lower[0];
    z = ((((uint32)native_position->position[2])));
    if ((sint32)x < (sint32)lower)
        return 0;
    span = target->outer.extent[0];
    if ((sint32)(lower + span) < (sint32)x)
        return 0;
    lower = target->outer.lower[1];
    if ((sint32)z < (sint32)lower)
        return 0;
    span = target->outer.extent[1];
    if ((sint32)(lower + span) < (sint32)z)
        return 0;
    lower = target->position[1];
    if ((sint32)lower < (sint32)y)
        return 0;
    result = 1;
    lower = target->inner.lower[0];
    if ((sint32)x < (sint32)lower)
        result = 2;
    span = target->inner.extent[0];
    if ((sint32)(lower + span) < (sint32)x)
        result = 2;
    lower = target->inner.lower[1];
    if ((sint32)z < (sint32)lower)
        result = 2;
    span = target->inner.extent[1];
    if ((sint32)(lower + span) < (sint32)z)
        result = 2;
    yaw = ((((sint16)native_position->angles[1])));
    target_yaw = (sint16)target->angles[1];
    delta = target_yaw - yaw;
    if ((sint32)delta < 8192)
    {
        yaw -= 0x4000;
        delta = target_yaw - yaw;
    }
    if ((sint32)delta >= 8193)
    {
        yaw += 0x4000;
        delta = target_yaw - yaw;
    }
    if ((sint32)delta < 0)
    {
        if ((sint32)(yaw - target_yaw) >= 683)
            result = 2;
    }
    else if ((sint32)delta >= 683)
        result = 2;
    return result;
}

uint32 flight_target_outside_bounds(const FLIGHT_TARGET_PROFILE *target, const uint32 *native_position)
{
    uint32 x, lower, span, z, result, y, target_y;
    FUNCTION_MARKER(0x80076A38, "ACE3.BIN");
    x = (((native_position[0])));
    lower = target->inner.lower[0];
    span = target->inner.extent[0];
    result = (sint32)x < (sint32)lower;
    z = (((native_position[2])));
    if ((sint32)(lower + span) < (sint32)x)
        result = 1;
    lower = target->inner.lower[1];
    if ((sint32)z < (sint32)lower)
        result = 1;
    span = target->inner.extent[1];
    if ((sint32)(lower + span) < (sint32)z)
        result = 1;
    y = (((native_position[1])));
    target_y = target->position[1];
    if ((sint32)(y - target_y) < -409600)
        result = 1;
    return result;
}
