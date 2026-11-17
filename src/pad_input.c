#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint8 number;
    uint8 phase;
    uint8 status;
    uint8 alignment[6];
    uint8 recheck;
    PAD_PORT_STATE *state;
} PAD_PORT;

typedef struct
{
    uint16 header;
    uint16 buttons;
    uint16 axis_pair[2];
} PAD_INPUT_PACKET;

typedef union
{
    PAD_INPUT_PACKET single[4];
    struct
    {
        uint16 header;
        PAD_INPUT_PACKET slot[4];
    } multitap;
} PAD_INPUT_BUFFER;

typedef char PAD_INPUT_PACKET_LAYOUT_CHECK[
    sizeof(PAD_INPUT_PACKET) == 8 && offsetof(PAD_INPUT_PACKET, buttons) == 2 &&
    offsetof(PAD_INPUT_PACKET, axis_pair) == 4 && sizeof(PAD_INPUT_BUFFER) == 34 &&
    offsetof(PAD_INPUT_BUFFER, multitap.slot) == 2 ? 1 : -1];

static PAD_PORT pad_ports[8];
/* PsyQ owns publication into these persistent binary packets */
static PAD_INPUT_BUFFER pad_packets[2];
PAD_PORT_STATE pad_state[8];

static void pad_update_port(const PAD_INPUT_PACKET *packet, PAD_PORT *port);
static uint32 pad_negotiate(PAD_PORT *port);

GAME_INPUT game_input;
PAD_PROFILE pad_profiles[6];

static uint32 pad_init_profile(uint32 kind, PAD_PROFILE *profile);
static uint32 pad_copy_bindings(PAD_PROFILE *profile, const uint16 *src);
static void pad_copy_axes(PAD_PROFILE *profile, const uint32 *src);

void game_input_reset(void)
{
    memset(&game_input, 0, sizeof(game_input));
    memset(pad_profiles, 0, sizeof(pad_profiles));
    memset(pad_state, 0, sizeof(pad_state));
    memset(pad_ports, 0, sizeof(pad_ports));
    memset(pad_packets, 0, sizeof(pad_packets));
}

typedef struct
{
    uint8 mode;
    uint8 param0;
    uint8 param1;
} PAD_MODE_PARAM;

typedef struct
{
    sint32 id;
    const sint16 *modes;
} PAD_MODE_MATCH;

typedef struct
{
    uint8 mask;
    uint8 params[3];
} PAD_ACTUATOR_MATCH;

static const sint16 pad_supported_modes[2][3] = {
    {4, 7, 0},
    {4, 768, 0}
};

static const PAD_MODE_MATCH pad_mode_match[2] = {
    {8, pad_supported_modes[0]},
    {9, pad_supported_modes[1]}
};

static const PAD_ACTUATOR_MATCH pad_actuator_match[3] = {
    {1, {1, 2, 0}},
    {2, {1, 1, 1}},
    {4, {4, 3, 1}}
};

static const PAD_MODE_PARAM pad_mode_params[2] = {
    {8, 1, 2},
    {9, 0, 3}
};

static sint32 pad_mode_count;
static uint32 pad_mode_flags;
static uint32 pad_input_field;

static sint32 pad_find_mode(const sint16 *modes);
static sint32 pad_compare_modes(const sint16 *modes, const sint16 *expected);
static uint32 pad_actuator_mask(const uint8 *actuator);
static uint32 pad_compare_actuator(const uint8 *actuator, const uint8 *expected);

typedef struct
{
    uint8 ttl;
    uint8 kind;
    uint8 strength;
} PAD_RUMBLE_SLOT;

typedef struct
{
    uint8 strength;
    uint8 ttl;
} PAD_RUMBLE_PRESET;

static const PAD_RUMBLE_PRESET pad_rumble_presets[6] = {
    {64, 3},
    {96, 3},
    {255, 2},
    {56, 3},
    {255, 2},
    {48, 3}
};

static PAD_RUMBLE_SLOT pad_rumble_slots[6];
static PAD_RUMBLE_SLOT *pad_rumble_acquire(uint32 kind);

static const uint16 pad_default_bindings[4][32] = {
    {
        0x0010, 0x0040, 0x0080, 0x0020, 0x2008, 0x4000, 0x0008, 0x0001,
        0x0400, 0x0800, 0x0000, 0x4000, 0x2000, 0x0000, 0x0001, 0x8000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0040,
        0x0010, 0x0080, 0x0020, 0x0800, 0x0400, 0x0100, 0x0200, 0x0000
    },
    {
        0x0010, 0x0040, 0x0080, 0x0020, 0x2008, 0x4000, 0x0008, 0x0001,
        0x0400, 0x0800, 0x0000, 0x4000, 0x2000, 0x0000, 0x0001, 0x8000,
        0x0010, 0x0040, 0x0080, 0x0020, 0x0004, 0x1000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0800, 0x0400, 0x0100, 0x0200, 0x0000
    },
    {
        0x0010, 0x0040, 0x0080, 0x0020, 0xA008, 0x4000, 0x0008, 0x0001,
        0x0400, 0x0800, 0x0000, 0x8000, 0x4000, 0x0000, 0x0001, 0x1000,
        0x0010, 0x0040, 0x0080, 0x0020, 0x0000, 0x2000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    },
    {
        0x0010, 0x0040, 0x0080, 0x0020, 0x2008, 0x5000, 0x0008, 0x0000,
        0x0400, 0x0800, 0x0000, 0x0800, 0x2000, 0x0000, 0x0000, 0x1000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0040,
        0x0010, 0x0000, 0x0000, 0x4000, 0x8000, 0x0080, 0x0020, 0x0000
    }
};

static const uint32 pad_default_axes[4][6] = {
    {
        0x20FF7F00, 0x20FF7F00, 0x20FF7F00, 0x20FF7F00, 0x03020000, 0x0100FFFF
    },
    {
        0x4CFF7F00, 0x4CFF7F00, 0x4CFF7F00, 0x4CFF7F00, 0x03020000, 0x0100FFFF
    },
    {
        0x20FF7F00, 0x20FF7F00, 0x20FF7F00, 0x20FF7F00, 0x01000008, 0xFFFF0203
    },
    {
        0x20FF0000, 0x20FF7F00, 0x20FF0000, 0x20FF0000, 0x01FF0000, 0xFFFFFFFF
    }
};

static const uint32 pad_rumble_pattern[16] = {
    0x00000000, 0x00010001, 0x01010101, 0x08420842,
    0x11111111, 0x24922492, 0x52525252, 0xAAAAAAAA,
    0xB555AAAD, 0xADADADAD, 0xDB6DDB6D, 0xEEEEEEEE,
    0xF7BDF7BD, 0xFEFEFEFE, 0xFFFEFFFE, 0xFFFFFFFF
};

sint32 pad_init_profiles(PAD_PROFILE *profiles)
{
    sint32 result;

    FUNCTION_MARKER(0x80028BC0, "ACE3.BIN");
    result = pad_reset_slots();
    if (profiles)
    {
        pad_init_profile(1, profiles);
        pad_init_profile(2, profiles + 1);
        pad_init_profile(4, profiles + 2);
        pad_init_profile(8, profiles + 3);
        result = pad_init_profile(16, profiles + 4);
    }
    return result;
}

sint32 pad_reset_slots(void)
{
    sint32 i;

    FUNCTION_MARKER(0x8002937C, "ACE3.BIN");
    for (i = 5; i >= 0; --i)
    {
        pad_rumble_slots[5 - i].ttl = 0;
    }
    return i;
}

static uint32 pad_init_profile(uint32 kind, PAD_PROFILE *profile)
{
    const uint32 *axes;
    uint32 result;

    FUNCTION_MARKER(0x80028848, "ACE3.BIN");
    profile->flags = 0;
    switch (kind)
    {
        case 1:
            pad_copy_bindings(profile, pad_default_bindings[0]);
            result = profile->flags;
            profile->axis_flags = 0;
            result |= 0x18;
            profile->flags = result;
            return result;
        case 2:
            pad_copy_bindings(profile, pad_default_bindings[1]);
            axes = pad_default_axes[0];
            break;
        case 4:
            pad_copy_bindings(profile, pad_default_bindings[1]);
            axes = pad_default_axes[1];
            break;
        case 8:
            pad_copy_bindings(profile, pad_default_bindings[2]);
            axes = pad_default_axes[2];
            break;
        case 16:
            pad_copy_bindings(profile, pad_default_bindings[3]);
            axes = pad_default_axes[3];
            break;
        default:
            return pad_copy_bindings(profile, pad_default_bindings[0]);
    }
    pad_copy_axes(profile, axes);
    result = profile->flags | 0x18;
    profile->flags = result;
    return result;
}

static uint32 pad_copy_bindings(PAD_PROFILE *profile, const uint16 *src)
{
    uint32 i;
    FUNCTION_MARKER(0x800287BC, "ACE3.BIN");
    for (i = 0; i < 32; ++i)
        profile->binding[i] = src[i];
    return 0;
}

static void pad_copy_axes(PAD_PROFILE *profile, const uint32 *src)
{
    uint32 i, word;
    FUNCTION_MARKER(0x800287E4, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
    {
        word = src[i];
        profile->cal[i].low = (uint8)word;
        profile->cal[i].center = (uint8)(word >> 8);
        profile->cal[i].high = (uint8)(word >> 16);
        profile->cal[i].dead_zone = (uint8)(word >> 24);
    }
    word = src[4];
    profile->axis_flags = (uint16)word;
    profile->axis_map[0] = (uint8)(word >> 16);
    profile->axis_map[1] = (uint8)(word >> 24);
    for (i = 0; i < 4; ++i)
        profile->axis_map[i + 2] = (uint8)(src[5] >> (i * 8));
    { (void)(uint32)(word); return; }
}

static void pad_update_port(const PAD_INPUT_PACKET *packet, PAD_PORT *port)
{
    PAD_PORT_STATE *state;
    uint32 header;
    uint32 old_type;
    uint32 type;
    uint32 first;
    uint32 second;
    uint32 buttons;
    uint32 prev_buttons;
    uint32 result;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x80026BA4, "ACE3.BIN");
    state = port->state;
    header = packet->header;
    old_type = state->type;
    state->status = (uint8)(0);
    if (!old_type && !(header & 255))
    {
        if (!pad_negotiate(port))
        {
            header = 0;
            state->status = (uint8)(2);
        }
    }
    else if (port->recheck)
    {
        if (pad_negotiate(port))
            port->recheck = (uint8)(0);
        else
            state->status = (uint8)(2);
    }
    if (header & 255)
    {
        old_type = state->type;
        result = 1;
        state->status = (uint8)(1);
        if (old_type)
        {
            result = 8;
            state->status = (uint8)(8);
        }
        port->phase = (uint8)(0);
        state->mode_id = (uint8)(0);
        state->motor_mask = (uint8)(0);
        state->motor[5] = (uint8)(0);
        state->motor[4] = (uint8)(0);
        state->motor[3] = (uint8)(0);
        state->motor[2] = (uint8)(0);
        state->motor[1] = (uint8)(0);
        state->motor[0] = (uint8)(0);
        state->version = (uint16)(0);
        state->type = (uint8)(0);
        state->pressed = (uint16)(0);
        state->buttons = (uint16)(0);
    }
    else
    {
        type = header >> 8;
        if (state->type && state->version)
        {
            PadGetState(port->number);
            result = PadInfoMode(port->number, 2, 0);
            if (result != state->version)
            {
                type = 0;
                state->status = (uint8)(4);
                port->phase = (uint8)(1);
                port->recheck = (uint8)(1);
            }
        }
        state->type = (uint8)(type);
        buttons = packet->buttons;
        prev_buttons = state->buttons;
        buttons = ~buttons;
        prev_buttons = ~prev_buttons;
        state->buttons = (uint16)(buttons);
        state->pressed = (uint16)(buttons & prev_buttons);
        if (type == 0x23 || type == 0x53 || type == 0x73)
        {
            first = packet->axis_pair[0];
            second = packet->axis_pair[1];
            state->axis[0] = (uint16)(first >> 8);
            state->axis[1] = (uint16)(first & 255);
            state->axis[2] = (uint16)(second >> 8);
            result = second & 255;
            state->axis[3] = (uint16)(result);
            { (void)(uint32)(result); return; }
        }
        if (type == 0x12)
        {
            first = packet->axis_pair[0];
            state->axis[0] = (uint16)((sint8)(first >> 8));
            x = state->axis[0];
            y = state->axis[3];
            state->axis[1] = (uint16)((sint8)first);
            result = state->axis[2];
            second = state->axis[1];
            result += x;
            state->axis[2] = (uint16)(result);
            state->axis[3] = (uint16)(y + second);
            { (void)(uint32)(result); return; }
        }
        if (type == 0x63)
        {
            first = packet->axis_pair[0];
            second = packet->axis_pair[1];
            state->axis[3] = (uint16)(0);
            state->axis[2] = (uint16)(0);
            state->axis[0] = (uint16)(first);
            state->axis[1] = (uint16)(second);
            return;
        }
        if (type == 0xE3)
        {
            first = packet->axis_pair[0];
            second = packet->axis_pair[1];
            x = state->axis[0];
            state->axis[3] = (uint16)(0);
            result = first - x;
            state->axis[2] = (uint16)(result);
            state->axis[0] = (uint16)(first);
            state->axis[1] = (uint16)(second);
            { (void)(uint32)(result); return; }
        }
        if (type == 0x43)
            result = 0x43;
        else if (type < 0x44)
            result = 0x12;
        else if (type < 0x64)
            result = 0x53;
        else
            result = 0xE3;
    }
    state->axis[3] = (uint16)(0);
    state->axis[2] = (uint16)(0);
    state->axis[1] = (uint16)(0);
    state->axis[0] = (uint16)(0);
    { (void)(uint32)(result); return; }
}

static uint32 pad_negotiate(PAD_PORT *port)
{
    sint16 modes[8];
    uint8 actuator[3];
    uint32 number;
    uint32 status;
    uint32 mode;
    uint32 version;
    uint32 phase;
    PAD_PORT_STATE *state;
    uint32 flags;
    uint32 count;
    uint32 selected;
    uint32 mask;
    uint8 *dst;
    uint32 param0;
    uint32 param1;
    sint32 mode_count;
    sint32 id;
    sint32 i;

    FUNCTION_MARKER(0x8002685C, "ACE3.BIN");
    number = port->number;
    status = PadGetState(number);
    mode = PadInfoMode(number, 1, 0);
    version = PadInfoMode(number, 2, 0);
    port->status = (uint8)(status);
    if (!status)
    {
        port->state->mode_id = (uint8)(0);
        port->state->motor_mask = (uint8)(0);
        port->state->version = (uint16)(0);
        port->phase = (uint8)(0);
        return 0;
    }
    phase = port->phase;
    if (!phase)
    {
        if (status == 2)
        {
            id = pad_map_mode(mode);
            port->state->mode_id = (uint8)(id);
            flags = (uint8)pad_mode_flags;
            port->state->motor_mask = (uint8)(flags & 1);
            dst = port->state->motor;
            PadSetAct(number, dst, 2);
            port->state->version = (uint16)(version);
            return mode;
        }
        if (status == 6)
        {
            mode_count = (sint32)PadInfoMode(number, 4, -1);
            for (i = 7; i >= 0; --i)
                modes[i] = 0;
            for (i = 0; i < mode_count && i < 7; ++i)
                modes[i] = (sint16)PadInfoMode(number, 4, i);
            id = pad_find_mode(modes);
            param0 = pad_get_mode_param0((uint32)id);
            param1 = pad_get_mode_param1((uint32)id);
            port->state->mode_id = (uint8)(id);
            PadSetMainMode(number, param0, param1);
            port->state->motor_mask = (uint8)(0);
            port->phase = (uint8)(1);
        }
        return 0;
    }
    if (phase == 1)
    {
        if (status != 6)
            return 0;
        port->alignment[5] = 255;
        port->alignment[4] = 255;
        port->alignment[3] = 255;
        port->alignment[2] = 255;
        port->alignment[1] = 255;
        port->alignment[0] = 255;
        count = PadInfoAct(number, -1, 0) & 255;
        selected = 0;
        for (i = 0; (uint32)i < count && i < 6; ++i)
        {
            actuator[0] = (uint8)PadInfoAct(number, i, 1);
            actuator[1] = (uint8)PadInfoAct(number, i, 2);
            actuator[2] = (uint8)PadInfoAct(number, i, 3);
            mask = pad_actuator_mask(actuator);
            if (mask)
                port->alignment[selected++ & 255] = (uint8)i;
            state = port->state;
            state->motor_mask = (uint8)(state->motor_mask | mask);
        }
        PadSetActAlign(number, port->alignment);
        port->phase = (uint8)(2);
        return 0;
    }
    if (phase == 2)
    {
        if (status != 6)
            return 0;
        count = 0;
        flags = port->state->motor_mask;
        for (i = 0; i < 6; ++i)
            if ((flags >> i) & 1)
                ++count;
        dst = port->state->motor;
        PadSetAct(number, dst, count);
        port->state->version = (uint16)(version);
        return mode;
    }
    port->phase = (uint8)(0);
    return 0;
}

static sint32 pad_find_mode(const sint16 *modes)
{
    uint32 i;

    FUNCTION_MARKER(0x80026628, "ACE3.BIN");
    for (i = 0; i < 2; ++i)
    {
        if (!pad_compare_modes(modes, pad_mode_match[i].modes))
            return pad_mode_match[i].id;
    }
    return -2;
}

static sint32 pad_compare_modes(const sint16 *modes, const sint16 *expected)
{
    sint32 actual;
    sint32 wanted;
    uint32 terminator;

    FUNCTION_MARKER(0x800265F0, "ACE3.BIN");
    for (;;)
    {
        actual = *modes;
        wanted = *expected;
        terminator = (uint16)*modes;
        if (actual != wanted)
            return actual - wanted;
        ++modes;
        if (!terminator)
            return 0;
        ++expected;
    }
}

sint32 pad_map_mode(uint32 mode)
{
    FUNCTION_MARKER(0x80026694, "ACE3.BIN");
    switch (mode)
    {
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 5;
        case 4:
            return 1;
        case 5:
            return 4;
        case 6:
            return 7;
        case 7:
            return 6;
        default:
            return -1;
    }
}

uint32 pad_get_mode_param0(uint32 mode)
{
    const PAD_MODE_PARAM *ptr;
    sint32 count;
    sint32 i;

    FUNCTION_MARKER(0x80026700, "ACE3.BIN");
    count = pad_mode_count;
    if (count <= 0)
        return 0;
    ptr = pad_mode_params;
    for (i = 0; i < count; ++i)
    {
        if (ptr->mode == mode)
            return ptr->param0;
        ++ptr;
    }
    return 0;
}

uint32 pad_get_mode_param1(uint32 mode)
{
    const PAD_MODE_PARAM *ptr;
    sint32 count;
    sint32 i;

    FUNCTION_MARKER(0x80026754, "ACE3.BIN");
    count = pad_mode_count;
    if (count <= 0)
        return 0;
    ptr = pad_mode_params;
    for (i = 0; i < count; ++i)
    {
        if (ptr->mode == mode)
            return ptr->param1;
        ++ptr;
    }
    return 0;
}

static uint32 pad_actuator_mask(const uint8 *actuator)
{
    uint32 i;

    FUNCTION_MARKER(0x800267F0, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        if (!pad_compare_actuator(actuator, pad_actuator_match[i].params))
            return pad_actuator_match[i].mask;
    }
    return 0;
}

static uint32 pad_compare_actuator(const uint8 *actuator, const uint8 *expected)
{
    FUNCTION_MARKER(0x800267A8, "ACE3.BIN");
    if (actuator[0] != expected[0])
        return 1;
    if (actuator[1] != expected[1])
        return 1;
    return actuator[2] != expected[2];
}

static void pad_rumble_mix(uint8 *output)
{
    uint32 ttl;
    uint32 kind;
    uint32 strength;
    uint32 max0 = 0;
    uint32 max1 = 0;
    uint32 i;
    uint32 decay;
    uint32 result;

    FUNCTION_MARKER(0x80029734, "ACE3.BIN");
    for (i = 0; i < 6; ++i)
    {
        ttl = pad_rumble_slots[i].ttl;
        if (ttl)
        {
            kind = pad_rumble_slots[i].kind;
            strength = pad_rumble_slots[i].strength;
            switch (kind)
            {
                case 1:
                case 2:
                case 3:
                case 9:
                    if ((pad_rumble_slots[i].kind & 1) && max0 < strength)
                        max0 = strength;
                    if (pad_rumble_slots[i].kind & 2)
                    {
                        if (max1 < strength)
                            max1 = strength;
                        pad_rumble_slots[i].strength = (uint8)(strength - (strength >> 2));
                    }
                    break;
                case 5:
                case 0xA0:
                    if (max1 < strength)
                        max1 = strength;
                    break;
                case 0x10:
                    if (max1 < strength)
                        max1 = strength;
                    pad_rumble_slots[i].strength = (uint8)(strength - (strength >> 3));
                    break;
                case 0x14:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < (strength >> 1))
                        max1 = strength >> 1;
                    pad_rumble_slots[i].strength = (uint8)(strength - (strength >> 4));
                    break;
                case 0x1E:
                    if (max0 < (strength >> 1))
                        max0 = strength >> 1;
                    if (max1 < strength)
                        max1 = strength;
                    pad_rumble_slots[i].strength = (uint8)(strength - (strength >> 5));
                    break;
                case 0xA1:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < strength)
                        max1 = strength;
                    break;
                case 0xB4:
                case 0xFA:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < strength)
                        max1 = strength;
                    if (ttl < (uint32)(kind == 0xB4 ? 6 : 57))
                    {
                        decay = strength >> (kind == 0xB4 ? 4 : 5);
                        pad_rumble_slots[i].strength = (uint8)(strength - decay);
                    }
                    break;
                default:
                    break;
            }
            --ttl;
            pad_rumble_slots[i].ttl = (uint8)ttl;
            if (!ttl)
                pad_rumble_slots[i].kind = 0;
        }
    }
    result = max1 < 256;
    if (max0 >= 256)
        max0 = 255;
    if (max1 >= 256)
        max1 = 255;
    output[0] = max0;
    output[1] = max1;
    { (void)(uint32)(result); return; }
}

void pad_normalize_axes(sint16 *axes, const PAD_AXIS_CALIBRATION *calibration)
{
    uint32 i;
    sint32 low;
    sint32 high;
    sint32 center;
    sint32 dead_zone;
    sint32 delta;
    sint32 numerator;
    sint32 denominator;

    FUNCTION_MARKER(0x8002895C, "ACE3.BIN");
    for (i = 0; i < 4; ++i, ++calibration)
    {
        low = calibration->low;
        high = calibration->high;
        if (axes[i] < low)
            axes[i] = low;
        if (high < axes[i])
            axes[i] = high;
        center = calibration->center;
        dead_zone = calibration->dead_zone;
        delta = (sint16)((uint16)axes[i] - center);
        if (delta > dead_zone)
        {
            numerator = (delta - dead_zone) * 8192;
            denominator = 2 * (high - center - dead_zone);
            if (!denominator)
                trap(7);
            axes[i] = (numerator / denominator - 1);
        }
        else if (delta < -dead_zone)
        {
            numerator = (delta + dead_zone) * 8192;
            denominator = -2 * (low - center + dead_zone);
            if (!denominator)
                trap(7);
            axes[i] = (numerator / denominator);
        }
        else
            axes[i] = 0;
    }
    return;
}

sint32 pad_curve_expand(sint32 value)
{
    sint32 half;
    sint32 sine;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x80028A7C, "ACE3.BIN");
    half = (sint16)value / 2;
    sine = rsin((uint32)half & 0xFFF);
    scaled = ((uint32)sine * 7);
    result = (sint16)((uint32)value + (uint32)(scaled / 24));
    if (result < -4096)
        result = -4096;
    if (result >= 4096)
        result = 4095;
    return result;
}

sint32 pad_curve_contract(sint32 value)
{
    sint32 half;
    sint32 sine;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x80028B1C, "ACE3.BIN");
    half = (sint16)value / 2;
    sine = rsin((uint32)(-half) & 0xFFF);
    scaled = ((uint32)sine * 7);
    result = (sint16)((uint32)value + (uint32)(scaled / 24));
    if (result < -4096)
        result = -4096;
    if (result >= 4096)
        result = 4095;
    return result;
}

void pad_update_input(PAD_PORT_STATE *pad, GAME_INPUT *state, const PAD_PROFILE *profiles)
{
    sint16 axes[4];
    const PAD_PROFILE *profile = profiles;
    uint32 pressed;
    uint32 buttons = 0;
    uint32 type;
    uint32 kind;
    uint32 i;
    uint32 selector;
    uint32 flags;
    uint32 mask;
    uint32 count;
    uint32 prev;
    uint32 edges;
    uint32 directions;
    uint32 result;
    uint32 level0;
    uint32 level1;
    uint8 *output;
    uint32 phase;
    sint32 value;
    sint32 limit;
    sint32 horizontal;
    sint32 vertical;
    sint32 dither;

    FUNCTION_MARKER(0x80028C24, "ACE3.BIN");
    pressed = pad->buttons;
    state->axis_mask = (uint32)(0);
    state->status = (uint16)(0);
    type = pad->type;
    if (type == 0x41 || type == 0x43)
        state->kind = (uint32)(1);
    else if (type == 0x23 || type == 0x53 || type == 0x73)
    {
        if (type == 0x23)
        {
            kind = 16;
            profile += 4;
        }
        else
        {
            type = pad->type;
            if (type == 0x73)
            {
                if (pad->version == 7)
                {
                    kind = 4;
                    profile += 2;
                }
                else
                {
                    kind = 2;
                    profile += 1;
                }
            }
            else
            {
                kind = 8;
                profile += 3;
            }
        }
        state->kind = (uint32)(kind);
        axes[0] = (sint16)pad->axis[0];
        axes[1] = (sint16)pad->axis[1];
        axes[2] = (sint16)pad->axis[2];
        axes[3] = (sint16)pad->axis[3];
        pad_normalize_axes(axes, profile->cal);
        for (i = 0; i < 6; ++i)
        {
            selector = profile->axis_map[i];
            if (selector == 255)
                state->axis[i] = (sint32)(uint32)(0);
            else
            {
                mask = state->axis_mask;
                value = axes[selector];
                state->axis_mask = (uint32)(mask | ((uint32)1 << i));
                flags = profile->axis_flags;
                if (flags & 1)
                    value = (sint16)pad_curve_expand(value);
                else if (flags & 2)
                    value = (sint16)pad_curve_contract(value);
                state->axis[i] = (sint32)(uint32)((uint32)value);
            }
        }
        if (kind == 16)
        {
            if (axes[0] >= 1537)
                pressed |= 0x4000;
            if (axes[3] >= 1537)
                pressed |= 0x8000;
            if (axes[2] >= 1537)
                pressed |= 0x400;
            if (pressed & 0x400)
                pressed |= 0x100;
            if (pressed & 0x800)
                pressed |= 0x200;
        }
        else
        {
            if (pad->type == 0x53)
            {
                horizontal = axes[0];
                vertical = axes[1];
                limit = 1536;
            }
            else
            {
                horizontal = axes[2];
                vertical = axes[3];
                limit = 3754;
            }
            if (horizontal < -limit)
                buttons |= 1;
            if (horizontal > limit)
                buttons |= 2;
            if (vertical < -limit)
                buttons |= 4;
            if (vertical > limit)
                buttons |= 8;
        }
    }
    else
    {
        pressed = 0;
        state->kind = (uint32)(type ? 32 : 0);
        state->repeat = (uint32)(0);
        state->pressed = (uint32)(0);
        state->held = (uint32)(0);
        state->axis_mask = (uint32)(0);
        state->axis[2] = (sint32)(uint32)(0);
        state->axis[1] = (sint32)(uint32)(0);
        state->axis[0] = (sint32)(uint32)(0);
        state->axis[5] = (sint32)(uint32)(0);
        state->axis[4] = (sint32)(uint32)(0);
        state->axis[3] = (sint32)(uint32)(0);
        state->repeat_tick = (uint8)(0);
        state->delay_tick[1] = (uint8)(0);
        state->delay_tick[0] = (uint8)(0);
    }
    flags = pad->status;
    if (flags & 1)
        state->status = (uint16)(state->status | 2);
    else if (flags & 2)
        state->status = (uint16)(state->status | 8);
    else if (flags & 8)
        state->status = (uint16)(state->status | 1);
    for (i = 0; i < 32; ++i)
    {
        mask = (sint16)profile->binding[i];
        if (mask && (pressed & mask))
            buttons |= (uint32)1 << i;
    }
    if (profile->flags & 0x10)
    {
        if (buttons & 0x200000)
        {
            count = state->delay_tick[0];
            buttons &= 0xFFDFFFFF;
            if (count < 7)
            {
                ++count;
                state->delay_tick[0] = (uint8)(count);
            }
            if ((count & 255) >= 7)
                buttons |= 0x2000;
        }
        else
        {
            if ((uint32)(state->delay_tick[0] - 1) < 6)
                buttons |= 0x200000;
            state->delay_tick[0] = (uint8)(0);
        }
    }
    if (profile->flags & 8)
    {
        if (buttons & 0x4000)
        {
            count = state->delay_tick[1];
            buttons &= 0xFFFFBFFF;
            if (count < 10)
            {
                ++count;
                state->delay_tick[1] = (uint8)(count);
            }
            if ((count & 255) >= 10)
                buttons |= 0x80000000;
        }
        else
        {
            if ((uint32)(state->delay_tick[1] - 1) < 9)
                buttons |= 0x4000;
            state->delay_tick[1] = (uint8)(0);
        }
    }
    prev = state->held;
    directions = buttons & 15;
    state->held = (uint32)(buttons);
    edges = ~prev & buttons;
    state->pressed = (uint32)(edges);
    state->repeat = (uint32)(edges);
    state->released = (uint32)(prev & ~buttons);
    if (directions && directions == (prev & 15))
        state->repeat_tick = (uint8)(state->repeat_tick + 1);
    else
        state->repeat_tick = (uint8)(0);
    count = state->repeat_tick;
    if (count >= 23 && !(count & 3))
        state->repeat = (uint32)(state->repeat | directions);
    if (profile->axis_flags & 4)
        state->axis[0] = (sint32)(uint32)(0 - (uint32)state->axis[0]);
    if (profile->axis_flags & 8)
        state->axis[3] = (sint32)(uint32)(0 - (uint32)state->axis[3]);
    if (profile->axis_flags & 0x10)
        state->axis[4] = (sint32)(uint32)(0 - (uint32)state->axis[4]);
    pad_rumble_mix(state->motor);
    result = state->flags & 1;
    if (result)
    {
        result = ~game_runtime.flags & 0x20000000;
    }
    if (result)
    {
        level0 = state->motor[0];
        flags = pad->motor_mask;
        level1 = state->motor[1];
        if (!(flags & 1))
            level1 = (level1 + level0) >> 1;
        if (!(flags & 2))
            level0 = (level0 + level1) >> 1;
        output = pad->motor;
        phase = game_runtime.frame_counter & 31;
        dither = (sint32)pad_rumble_pattern[((level0 >> 2) & 0x3C) >> 2];
        mask = (uint32)(dither >> phase) & 1;
        if (!pad->version && pad->type)
        {
            pad->motor[0] = (uint8)(64);
            output = pad->motor + 1;
        }
        if (pad->motor_mask & 1)
        {
            *output = (mask);
            ++output;
        }
        result = pad->motor_mask & 2;
        if (result)
            *output = (level1);
    }
    else
    {
        pad->motor[0] = (uint8)(0);
        pad->motor[1] = (uint8)(0);
        pad->motor[2] = (uint8)(0);
        pad->motor[3] = (uint8)(0);
        pad->motor[4] = (uint8)(0);
        pad->motor[5] = (uint8)(0);
    }
    { (void)(uint32)(result); return; }
}

static PAD_RUMBLE_SLOT *pad_rumble_acquire(uint32 kind)
{
    PAD_RUMBLE_SLOT *slot;
    uint32 index, occupied, priority;
    FUNCTION_MARKER(0x8002939C, "ACE3.BIN");
    for (index = 0; index < 6; ++index)
    {
        slot = &pad_rumble_slots[index];
        occupied = slot->ttl;
        if (occupied != 0)
        {
            priority = slot->kind;
            if ((sint32)kind < (sint32)priority)
                continue;
        }
        slot->kind = (uint8)kind;
        return slot;
    }
    return 0;
}

void pad_rumble_add_sustained(void)
{
    PAD_RUMBLE_SLOT *record;
    FUNCTION_MARKER(0x8002963C, "ACE3.BIN");
    record = pad_rumble_acquire(250);
    if (record == 0)
        return;
    record->ttl = (uint8)(60);
    record->strength = (uint8)(255);
    return;
}

void pad_rumble_add_random_large(void)
{
    PAD_RUMBLE_SLOT *record;
    uint32 seed, result;
    FUNCTION_MARKER(0x80029674, "ACE3.BIN");
    record = pad_rumble_acquire(160);
    if (record == 0)
        return;
    record->ttl = (uint8)(1);
    seed = random_seed;
    seed = seed * 0x41C64E6D + 12345;
    random_seed = seed;
    result = ((seed >> 16) & 63) + 65;
    record->strength = (uint8)(result);
    { (void)(uint32)(result); return; }
}

void pad_rumble_add_burst(void)
{
    PAD_RUMBLE_SLOT *record;
    FUNCTION_MARKER(0x800296B8, "ACE3.BIN");
    record = pad_rumble_acquire(180);
    if (record == 0)
        return;
    record->ttl = (uint8)(9);
    record->strength = (uint8)(255);
    return;
}

uint32 pad_rumble_add_random_both(void)
{
    PAD_RUMBLE_SLOT *record;
    uint32 seed, result;
    FUNCTION_MARKER(0x800296F0, "ACE3.BIN");
    record = pad_rumble_acquire(161);
    if (record == 0)
        return 1;
    record->ttl = (uint8)(1);
    seed = random_seed;
    seed = seed * 0x41C64E6D + 12345;
    random_seed = seed;
    result = ((seed >> 16) & 63) + 97;
    record->strength = (uint8)(result);
    return result;
}

void pad_rumble_add_scaled(sint32 value)
{
    PAD_RUMBLE_SLOT *state;
    uint32 result;
    sint32 scaled;
    FUNCTION_MARKER(0x800293F0, "ACE3.BIN");
    if (value < 32)
        return;
    state = pad_rumble_acquire(5);
    if (state == 0)
        return;
    state->ttl = (uint8)(1);
    scaled = value >> 5;
    result = scaled < 109;
    if (scaled >= 109)
        scaled = 105;
    state->strength = (uint8)(scaled);
    { (void)(uint32)(result); return; }
}

void pad_rumble_add_pulse(uint32 mode)
{
    PAD_RUMBLE_SLOT *slot;
    uint32 result;
    FUNCTION_MARKER(0x80029448, "ACE3.BIN");
    result = game_input.flags & 3;
    if (result != 3)
        { (void)(uint32)(result); return; }
    if (mode == 7 || mode == 11)
        return;
    slot = pad_rumble_acquire(9);
    if (slot == 0)
        return;
    slot->strength = (uint8)(255);
    result = mode == 6 || mode == 10 ? 2 : 1;
    slot->ttl = (uint8)(result);
    { (void)(uint32)(result); return; }
}

uint32 pad_rumble_add_preset(uint32 mode)
{
    PAD_RUMBLE_SLOT *slot;
    uint32 result, index;
    FUNCTION_MARKER(0x800294C4, "ACE3.BIN");
    result = game_input.flags & 3;
    if (result != 3)
        return result;
    slot = pad_rumble_acquire(3);
    if (slot == 0)
        return 0x80010000;
    index = mode - 1;
    if (index >= 6)
    {
        slot->ttl = (uint8)(0);
        return 0;
    }
    slot->strength = pad_rumble_presets[index].strength;
    slot->ttl = pad_rumble_presets[index].ttl;
    if (index != 2)
        slot->kind = (uint8)(1);
    return 1;
}

void pad_rumble_add_decay_both(void)
{
    PAD_RUMBLE_SLOT *entry;
    FUNCTION_MARKER(0x800295CC, "ACE3.BIN");
    entry = pad_rumble_acquire(20);
    if (entry == 0)
        return;
    entry->ttl = (uint8)(3);
    entry->strength = (uint8)(230);
    return;
}

void pad_rumble_add_decay_strong(void)
{
    PAD_RUMBLE_SLOT *entry;
    FUNCTION_MARKER(0x80029604, "ACE3.BIN");
    entry = pad_rumble_acquire(30);
    if (entry == 0)
        return;
    entry->ttl = (uint8)(15);
    entry->strength = (uint8)(250);
    return;
}

void pad_rumble_add_decay_slow(void)
{
    PAD_RUMBLE_SLOT *entry;
    uint32 result;
    FUNCTION_MARKER(0x80029588, "ACE3.BIN");
    entry = pad_rumble_acquire(16);
    if (entry == 0)
        return;
    entry->ttl = (uint8)(10);
    result = (game_random() & 0x3F) + 168;
    entry->strength = (uint8)(result);
    { (void)(uint32)(result); return; }
}

void pad_init_ports(void)
{
    uint32 i;
    FUNCTION_MARKER(0x8002654C, "ACE3.BIN");
    memset(pad_packets, 0, sizeof(pad_packets));
    memset(pad_ports, 0, sizeof(pad_ports));
    pad_mode_count = 0;
    pad_mode_flags = 0;
    pad_mode_count = 2;
    pad_mode_flags = 3;
    for (i = 0; i < 8; ++i)
    {
        pad_ports[i].state = &pad_state[i];
        pad_ports[i].number = (uint8)(i + (i >= 4 ? 12 : 0));
    }
    PadInitDirect((uint8 *)&pad_packets[0], (uint8 *)&pad_packets[1]);
    PadStartCom();
    pad_input_field = VSync(-1);
}

static void pad_refresh_connection(void)
{
    static uint8 available[2] = {255, 255};
    uint8 axes[4];
    uint32 controller, analog;
    PAD_PORT *port;
    for (controller = 0; controller < 2; ++controller)
    {
        xport_input_read((sint32)controller);
        analog = xport_input_analog((sint32)controller, axes) != 0;
        if (available[controller] != analog)
        {
            port = &pad_ports[controller * 4];
            if (available[controller] != 255 && port->state->type)
            {
                /* Reuse original negotiation when the host input device changes */
                port->phase = 0;
                port->recheck = 1;
            }
            available[controller] = analog;
        }
    }
}

void pad_poll(void)
{
    PAD_INPUT_BUFFER *state;
    PAD_INPUT_PACKET *packet;
    uint32 i;
    uint32 result;

    FUNCTION_MARKER(0x80026E84, "ACE3.BIN");
    result = VSync(-1);
    if (result == pad_input_field)
        return;
    pad_input_field = result;
    state = &pad_packets[0];
#if !defined(LOCKSTEP_DEBUG)
    pad_refresh_connection();
#endif
    if (state->multitap.header == 0x8000)
    {
        packet = &state->multitap.slot[0];
        for (i = 0; i < 4; ++i)
        {
            pad_update_port(packet, &pad_ports[i]);
            ++packet;
        }
    }
    else
    {
        pad_update_port(&state->single[0], &pad_ports[0]);
        packet = &state->single[1];
        for (i = 1; i < 4; ++i)
        {
            packet->header = 0xFFFF;
            pad_update_port(packet, &pad_ports[i]);
            ++packet;
        }
    }
    state = &pad_packets[1];
    if (state->multitap.header == 0x8000)
    {
        packet = &state->multitap.slot[0];
        for (i = 4; i < 8; ++i)
        {
            pad_update_port(packet, &pad_ports[i]);
            ++packet;
        }
    }
    else
    {
        pad_update_port(&state->single[0], &pad_ports[4]);
        packet = &state->single[1];
        for (i = 5; i < 8; ++i)
        {
            packet->header = 0xFFFF;
            pad_update_port(packet, &pad_ports[i]);
            ++packet;
        }
    }
    return;
}

uint32 pad_profile_index(uint32 kind)
{
    FUNCTION_MARKER(0x800D1B04, "ACE3.BIN");
    if (kind & 1)
        return 0;
    if (kind & 2)
        return 1;
    if (kind & 4)
        return 2;
    if (kind & 8)
        return 3;
    if (kind & 0x10)
        return 4;
    if (kind & 0x20)
        return 5;
    return 0xFFFFFFFF;
}

typedef struct
{
    uint16 binding[20];
    PAD_CALIBRATION_RECORD cal[4];
    uint16 axis_flags;
    uint8 axis_map[6];
    uint32 flags;
} PAD_CARD_PROFILE;

typedef char PAD_CARD_PROFILE_LAYOUT_CHECK[
    sizeof(PAD_CARD_PROFILE) == 68 && offsetof(PAD_CARD_PROFILE, cal) == 40 &&
    offsetof(PAD_CARD_PROFILE, axis_flags) == 56 && offsetof(PAD_CARD_PROFILE, axis_map) == 58 &&
    offsetof(PAD_CARD_PROFILE, flags) == 64 ? 1 : -1];

void pad_profile_encode(uint8 *dst, const PAD_PROFILE *profile)
{
    PAD_CARD_PROFILE wire;
    uint32 i;
    for (i = 0; i < 20; ++i)
        wire.binding[i] = profile->binding[i + 11];
    for (i = 0; i < 4; ++i)
    {
        wire.cal[i].low = profile->cal[i].low;
        wire.cal[i].center = profile->cal[i].center;
        wire.cal[i].high = profile->cal[i].high;
        wire.cal[i].dead_zone = profile->cal[i].dead_zone;
    }
    wire.axis_flags = profile->axis_flags;
    for (i = 0; i < 6; ++i)
        wire.axis_map[i] = profile->axis_map[i];
    wire.flags = profile->flags;
    memcpy(dst, &wire, sizeof(wire));
}

void pad_profile_decode(PAD_PROFILE *profile, const uint8 *src)
{
    PAD_CARD_PROFILE wire;
    uint32 i;
    memcpy(&wire, src, sizeof(wire));
    for (i = 0; i < 20; ++i)
        profile->binding[i + 11] = wire.binding[i];
    for (i = 0; i < 4; ++i)
    {
        profile->cal[i].low = wire.cal[i].low;
        profile->cal[i].center = wire.cal[i].center;
        profile->cal[i].high = wire.cal[i].high;
        profile->cal[i].dead_zone = wire.cal[i].dead_zone;
    }
    profile->axis_flags = wire.axis_flags;
    for (i = 0; i < 6; ++i)
        profile->axis_map[i] = wire.axis_map[i];
    profile->flags = wire.flags;
}
