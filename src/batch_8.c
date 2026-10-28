#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
void sub_8002ee7c(uint32 state, uint32 voice)
{
    FUNCTION_MARKER(0x8002EE7C, "ACE3.BIN");
    w_u8(state, 0);
    w_u8(state + 1, voice);
}

uint32 sub_8002ee88(uint32 state, uint32 a, uint32 b)
{
    uint32 value;
    FUNCTION_MARKER(0x8002EE88, "ACE3.BIN");
    value = r_u8(state);
    w_u8(state + 2, a);
    w_u16(state + 4, b & 0xFF);
    w_u16(state + 8, 0);
    value |= 1;
    w_u8(state, value);
    return value;
}

uint32 sub_8002eea8(uint32 state, uint32 a, uint32 b)
{
    uint32 value;
    FUNCTION_MARKER(0x8002EEA8, "ACE3.BIN");
    value = r_u8(state);
    w_u8(state + 3, a);
    w_u16(state + 6, b & 0xFF);
    w_u16(state + 10, 0);
    value |= 2;
    w_u8(state, value);
    return value;
}

uint32 sub_800300e0(uint32 voice, uint32 volume, uint32 previous, uint32 step)
{
    uint32 left, old_left, right, old_right, delta_left, delta_right, minimum;
    FUNCTION_MARKER(0x800300E0, "ACE3.BIN");
    left = r_s16(volume);
    old_left = r_s16(previous);
    minimum = 0 - step;
    delta_left = left - old_left;
    if ((sint32)delta_left < (sint32)minimum)
        delta_left = minimum;
    if ((sint32)step < (sint32)delta_left)
        delta_left = step;
    right = r_s16(volume + 2);
    old_right = r_s16(previous + 2);
    delta_right = right - old_right;
    left = delta_left + old_left;
    if ((sint32)delta_right < (sint32)minimum)
        delta_right = minimum;
    if ((sint32)step < (sint32)delta_right)
        delta_right = step;
    right = delta_right + old_right;
    return sub_8002dec8(voice & 0xFF, (uint32)(sint32)(sint16)left, (uint32)(sint32)(sint16)right);
}

uint32 sub_8002e67c(uint32 voice)
{
    uint32 target, value;
    FUNCTION_MARKER(0x8002E67C, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    value = r_u8(target + 1);
    w_u8(target + 1, value | 0x10);
    value = r_u32(target + 8);
    w_u32(target + 4, value);
    value = r_u16(target + 14);
    w_u16(target + 12, value);
    return voice;
}

uint32 sub_8002eec8(uint32 state)
{
    uint32 voice, target, flags, numerator, divisor, phase, product, delta, left, right, count, note;
    sint32 left_signed;
    FUNCTION_MARKER(0x8002EEC8, "ACE3.BIN");
    voice = r_u8(state + 1);
    if (voice == 255)
        return 255;
    voice = r_u8(state + 1);
    flags = r_u8(state);
    target = 0x800C5830 + voice * 20;
    if (flags & 1)
    {
        numerator = (uint32)r_s16(state + 8) << 14;
        divisor = r_u8(state + 2);
        if (divisor == 0)
            xport_mips_break(7);
        phase = (uint32)((sint32)numerator / (sint32)divisor) & 0x3FFF;
        product = (uint32)r_s16(0x800104AC + phase * 2) * (uint32)r_s16(state + 4);
        delta = (uint32)((sint32)product >> 14);
        left = r_u16(target + 4);
        w_u16(target + 4, left + delta);
        right = r_u16(target + 6);
        left_signed = r_s16(target + 4);
        w_u16(target + 6, right + delta);
        if (left_signed >= 0x4000)
            w_u16(target + 4, 0x3FFF);
        if (r_s16(target + 6) >= 0x4000)
            w_u16(target + 6, 0x3FFF);
        if (r_s16(target + 4) < 0)
            w_u16(target + 4, 0);
        if (r_s16(target + 6) < 0)
            w_u16(target + 6, 0);
        count = r_u16(state + 8);
        divisor = r_u8(state + 2);
        ++count;
        w_u16(state + 8, count);
        if ((sint16)count >= (sint32)divisor)
            w_u16(state + 8, 0);
    }
    flags = r_u8(state);
    if (flags & 2)
    {
        numerator = (uint32)r_s16(state + 10) << 14;
        divisor = r_u8(state + 3);
        if (divisor == 0)
            xport_mips_break(7);
        phase = (uint32)((sint32)numerator / (sint32)divisor) & 0x3FFF;
        product = (uint32)r_s16(0x800104AC + phase * 2) * (uint32)r_s16(state + 6);
        note = r_s16(target + 12);
        delta = (uint32)((sint32)(product << 2) >> 16);
        w_u16(target + 12, sub_8002e64c(note, delta));
        count = r_u16(state + 10);
        divisor = r_u8(state + 3);
        ++count;
        w_u16(state + 10, count);
        if ((sint16)count >= (sint32)divisor)
            w_u16(state + 10, 0);
    }
    return r_u8(state + 1);
}

uint32 sub_8002ed2c(uint32 state, uint32 control)
{
    uint32 flag, threshold, distance, value, voice;
    FUNCTION_MARKER(0x8002ED2C, "ACE3.BIN");
    flag = r_u8(control);
    if (flag == 0)
    {
        threshold = r_s16(control + 4);
        distance = r_u32(state);
        if (distance < threshold)
        {
            threshold = r_u8(control + 6);
            value = r_u32(state + 4);
            if ((sint32)threshold < (sint32)value)
            {
                w_u8(control, 1);
                if (r_s16(state + 10) >= 0)
                    voice = sub_8002dcb0(r_u8(control + 1));
                else
                    voice = sub_8002dcb0(r_u8(control + 2));
                w_u8(control + 3, voice);
            }
            else if ((sint32)(threshold >> 1) < (sint32)value)
            {
                w_u8(control, 1);
                value = game_random();
                voice = sub_8002db4c((uint32)((sint32)value % 3) + 10);
                w_u8(control + 3, voice);
            }
            value = game_random();
            voice = r_u8(control + 3);
            sub_8002f0d4(voice, 0 - (value & 0x3FF));
        }
    }
    flag = r_u8(control);
    if (flag == 1)
    {
        voice = r_u8(control + 3);
        if (voice == 255)
            return 255;
        if (r_u8(0x800C5830 + voice * 20 + 3) == 0)
        {
            w_u8(control + 3, 255);
            w_u8(control, 0);
        }
    }
    return r_u8(control + 3);
}

uint32 sub_8002dcb0(uint32 sound)
{
    uint32 voice;
    FUNCTION_MARKER(0x8002DCB0, "ACE3.BIN");
    voice = sound_reserve_voice();
    if (voice == 255)
        return 255;
    sound_start_voice(sound, voice);
    return voice;
}

uint32 sub_8002f0d4(uint32 voice, uint32 offset)
{
    uint32 target, flags, note;
    FUNCTION_MARKER(0x8002F0D4, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    offset = (uint32)(sint32)(sint16)offset;
    flags = r_u8(target + 1);
    note = r_s16(target + 12);
    w_u8(target + 1, flags | 0x10);
    w_u16(target + 12, sub_8002e64c(note, offset));
    return voice;
}

uint32 sub_8002e84c(uint32 voice, uint32 state, uint32 mode, const uint8 *native_state)
{
    uint32 target, value, gain, product;
    FUNCTION_MARKER(0x8002E84C, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    value = r_u8(target + 1);
    w_u8(target + 1, value | 0x10);
    gain = 0x3FFF - ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) << (mode & 31));
    if ((sint32)gain < 0)
        gain = 0;
    if ((sint32)gain >= 0x4001)
        gain = 0x4000;
    product = gain * gain;
    gain = (uint32)((sint32)product >> 14);
    product = (uint32)r_s16(target + 4) * gain;
    value = (uint32)((sint32)product >> 14);
    w_u16(target + 6, value);
    w_u16(target + 4, value);
    return voice;
}

uint32 sub_8002e8dc(uint32 voice, uint32 state, const uint8 *native_state)
{
    uint32 target, value, phase, amplitude, sine_product, cosine_product, gain, minimum, raw;
    sint32 signed_value;
    FUNCTION_MARKER(0x8002E8DC, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    value = r_u8(target + 1);
    w_u8(target + 1, value | 0x10);
    phase = (native_state != 0 ? xport_load_le16(native_state + 8) : r_u16(state + 8));
    amplitude = r_s16(target + 4);
    phase = (phase >> 1) & 0x3FFF;
    sine_product = amplitude * (uint32)r_s16(0x800104AC + phase * 2);
    cosine_product = amplitude * (uint32)r_s16(0x800104AC + (phase + 0x1000) * 2);
    w_u16(target + 4, (uint32)((sint32)sine_product >> 14));
    w_u16(target + 6, (uint32)((sint32)cosine_product >> 14));
    signed_value = r_s16(target + 4);
    raw = r_u16(target + 4);
    if (signed_value < 0)
        w_u16(target + 4, 0 - raw);
    signed_value = r_s16(target + 6);
    raw = r_u16(target + 6);
    if (signed_value < 0)
        w_u16(target + 6, 0 - raw);
    gain = 0x4000 - ((native_state != 0 ? xport_load_le32(native_state) : r_u32(state)) << 6);
    if ((sint32)gain < 0)
        gain = 0;
    value = r_s16(target + 4);
    minimum = (uint32)((sint32)(amplitude * gain) >> 14);
    if ((sint32)value < (sint32)minimum)
        w_u16(target + 4, minimum);
    if (r_s16(target + 6) < (sint32)minimum)
        w_u16(target + 6, minimum);
    if (r_s16(target + 4) >= 0x4000)
        w_u16(target + 4, 0x3FFF);
    if (r_s16(target + 6) >= 0x4000)
        w_u16(target + 6, 0x3FFF);
    return voice;
}

uint32 sub_8002ec48(uint32 voice, uint32 state)
{
    uint32 target, value, numerator, high, index, note, offset;
    FUNCTION_MARKER(0x8002EC48, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    value = r_u8(target + 1);
    w_u8(target + 1, value | 0x10);
    numerator = (r_u32(state + 4) + 0x470) << 9;
    high = (uint32)(((sint64)(sint32)numerator * (sint32)0xE6C2B449) >> 32);
    index = (uint32)((sint32)(high + numerator) >> 11) - (uint32)((sint32)numerator >> 31);
    if ((sint32)index < 0)
        index = 0;
    if ((sint32)index >= 512)
        index = 511;
    note = r_s16(target + 14);
    offset = r_s16(0x800B0500 + index * 2);
    w_u16(target + 12, sub_8002e64c(note, offset));
    return voice;
}

uint32 sub_8002ea74(uint32 voice, uint32 other_voice, uint32 state)
{
    uint32 target, other, value, sign, phase, cosine, sine, product, left, right, raw_left, raw_right;
    sint32 old_left, angle;
    FUNCTION_MARKER(0x8002EA74, "ACE3.BIN");
    if (voice == 255 || other_voice == 255)
        return 255;
    target = 0x800C5830 + voice * 20;
    value = r_u8(target + 1);
    w_u8(target + 1, value | 0x10);
    other = 0x800C5830 + other_voice * 20;
    value = r_u8(other + 1);
    old_left = r_s16(other + 4);
    w_u8(other + 1, value | 0x10);
    value = r_u16(target + 12);
    w_u16(other + 12, value);
    if (old_left < 0)
        sign = 1;
    else
        sign = (r_u16(other + 6) >> 14) & 2;
    angle = r_s16(state + 10);
    if (angle > 1024)
    {
        w_u16(other + 4, 0);
        w_u16(other + 6, 0);
    }
    else if (angle < -1024)
    {
        value = r_u16(target + 4);
        w_u16(other + 4, value);
        value = r_u16(target + 6);
        w_u16(other + 6, value);
        w_u16(target + 4, 0);
        w_u16(target + 6, 0);
    }
    else
    {
        phase = (((uint32)angle + 1024) << 1) & 0x3FFF;
        left = r_s16(target + 4);
        cosine = r_s16(0x800104AC + (phase + 0x1000) * 2);
        product = left * cosine;
        w_u16(other + 4, (uint32)((sint32)product >> 14));
        product = (uint32)r_s16(target + 6) * cosine;
        sine = r_s16(0x800104AC + phase * 2);
        w_u16(other + 6, (uint32)((sint32)product >> 14));
        product = (uint32)r_s16(target + 4) * sine;
        value = r_s16(target + 6);
        right = value * sine;
        w_u16(target + 4, (uint32)((sint32)product >> 14));
        w_u16(target + 6, (uint32)((sint32)right >> 14));
    }
    if (sign == 1)
    {
        value = r_u16(other + 4);
        w_u16(other + 4, 0 - value);
    }
    else if (sign == 2)
    {
        value = r_u16(other + 6);
        w_u16(other + 6, 0 - value);
    }
    else
    {
        left = r_s16(other + 4);
        right = r_s16(other + 6);
        raw_left = r_u16(other + 4);
        raw_right = r_u16(other + 6);
        if ((sint32)right < (sint32)left)
            w_u16(other + 6, 0 - raw_right);
        else
            w_u16(other + 4, 0 - raw_left);
    }
    return voice;
}

uint32 sub_8002dc7c(uint32 sound, uint32 voice)
{
    FUNCTION_MARKER(0x8002DC7C, "ACE3.BIN");
    if (voice == 255)
        return 255;
    sound_start_voice(sound, voice);
    return voice;
}

uint32 sub_8004ed9c(uint32 index)
{
    uint32 object, slot, table, descriptor, flag, a, b, i;
    FUNCTION_MARKER(0x8004ED9C, "ACE3.BIN");
    object = r_u32(0x800BCEC0) + index * 184;
    slot = r_u8(object + 63);
    table = r_u32(0x800BCE5C);
    flag = r_u8(object + 62);
    descriptor = r_u32(table + slot * 4);
    if (flag == 0)
        w_u8(object + 62, 1);
    for (i = 0; i < 3; ++i)
    {
        a = r_u32(descriptor + 8 + i * 4);
        b = r_u32(descriptor + 56 + i * 4);
        w_u32(object + 104 + i * 4, a + b);
    }
    a = r_u32(descriptor + 24);
    b = r_u32(descriptor + 28);
    w_u32(object + 120, a);
    w_u32(object + 124, b);
    sub_800505dc(index, object + 104);
    w_u8(object + 99, 4);
    w_u8(object + 97, 4);
    return 4;
}

uint32 sub_80050690(uint32 index, uint32 position)
{
    uint32 base, limit, value, result;
    FUNCTION_MARKER(0x80050690, "ACE3.BIN");
    base = r_u32(0x800BCEF8);
    index <<= 6;
    base = r_u32(base + index + 24);
    limit = r_u32(base + 8);
    value = r_u32(position + 4);
    if ((sint32)limit < (sint32)value)
        w_u32(position + 4, limit);
    base = r_u32(0x800BCEF8);
    base = r_u32(base + index + 24);
    limit = r_u32(base + 4);
    value = r_u32(position + 4);
    result = (sint32)value < (sint32)limit;
    if (result)
        w_u32(position + 4, limit);
    return result;
}

uint32 sub_800505dc(uint32 index, uint32 position)
{
    uint32 base, limit, value, result;
    FUNCTION_MARKER(0x800505DC, "ACE3.BIN");
    sub_80050690(index, position);
    base = r_u32(0x800BCEB0);
    limit = r_u32(base + 16);
    value = r_u32(position);
    limit += 0x4000000;
    if ((sint32)value < (sint32)limit)
        w_u32(position, limit);
    base = r_u32(0x800BCEB0);
    limit = r_u32(base + 20);
    value = r_u32(position);
    limit -= 0x4000000;
    if ((sint32)limit < (sint32)value)
        w_u32(position, limit);
    base = r_u32(0x800BCEB0);
    limit = r_u32(base + 24);
    value = r_u32(position + 8);
    limit += 0x4000000;
    if ((sint32)value < (sint32)limit)
        w_u32(position + 8, limit);
    base = r_u32(0x800BCEB0);
    limit = r_u32(base + 28);
    value = r_u32(position + 8);
    limit -= 0x4000000;
    result = (sint32)limit < (sint32)value;
    if (result)
        w_u32(position + 8, limit);
    return result;
}

uint32 sub_8002e6dc(uint32 state, uint32 listener, uint32 source, uint8 *native_state, const uint32 *native_source, const uint32 *native_listener)
{
    MATRIX matrix = {0};
    uint16 angles[3];
    uint32 distance, previous, value, absolute, i, input[3], high[3], low[3], result[3], magnitude;
    sint32 angle;
    FUNCTION_MARKER(0x8002E6DC, "ACE3.BIN");
    distance = sub_80083dfc(listener, source, native_listener, native_source);
    previous = (native_state != 0 ? xport_load_le32(native_state) : r_u32(state));
    if (previous == 0 || previous == 0x100000)
    {
        if (native_state != 0)
            xport_store_le32(native_state + 4, 0);
        else
            w_u32(state + 4, 0);
    }
    else
    {
        if (native_state != 0)
            xport_store_le32(native_state + 4, previous - distance);
        else
            w_u32(state + 4, previous - distance);
    }
    absolute = (native_state != 0 ? xport_load_le32(native_state + 4) : r_u32(state + 4));
    if ((sint32)absolute < 0)
        absolute = 0 - absolute;
    if ((sint32)absolute >= 301)
    {
        if (native_state != 0)
            xport_store_le32(native_state + 4, 0);
        else
            w_u32(state + 4, 0);
    }
    {
        if (native_state != 0)
            xport_store_le32(native_state, distance);
        else
            w_u32(state, distance);
    }
    if (native_listener)
    {
        angles[0] = (uint16)native_listener[4];
        angles[1] = (uint16)(native_listener[4] >> 16);
        angles[2] = (uint16)native_listener[5];
    }
    sub_800308f0(listener + 16, 0, &matrix, native_listener ? angles : 0);
    for (i = 0; i < 3; ++i)
    {
        value = native_source != 0 ? native_source[i] : r_u32(source + i * 4);
        previous = native_listener ? native_listener[i] : r_u32(listener + i * 4);
        input[i] = (uint32)((sint32)(value - previous) >> 12);
    }
    /* Preserve the SDK long-vector transform and its GTE register effects */
    for (i = 0; i < 5; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + i * 4));
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (uint32)((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    value = sub_80030d54(result[2], result[0]);
    {
        if (native_state != 0)
            xport_store_le16(native_state + 8, value & 0x3FFF);
        else
            w_u16(state + 8, value & 0x3FFF);
    }
    value = (native_state != 0 ? xport_load_le16(native_state + 10) : r_u16(state + 10));
    if ((sint32)result[2] < 0)
        value -= 256;
    else
        value += 256;
    {
        if (native_state != 0)
            xport_store_le16(native_state + 10, value);
        else
            w_u16(state + 10, value);
    }
    angle = (native_state != 0 ? (sint16)xport_load_le16(native_state + 10) : r_s16(state + 10));
    if (angle > 1024)
    {
        {
            if (native_state != 0)
                xport_store_le16(native_state + 10, 1024);
            else
                w_u16(state + 10, 1024);
        }
        return 1024;
    }
    if (angle < -1024)
    {
        {
            if (native_state != 0)
                xport_store_le16(native_state + 10, 0xFC00);
            else
                w_u16(state + 10, 0xFC00);
        }
        return 0xFFFFFC00;
    }
    return 0;
}

uint32 sub_800451ac(uint32 origin, uint32 vector, uint32 output)
{
    MATRIX matrix = {0};
    uint32 i, input[3], high[3], low[3], result[3], magnitude, value, a, b, c;
    FUNCTION_MARKER(0x800451AC, "ACE3.BIN");
    rotation_build(origin + 16, 0, &matrix, 0);
    for (i = 0; i < 5; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + i * 4));
    for (i = 0; i < 3; ++i)
        input[i] = r_u32(vector + i * 4);
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (uint32)((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    for (i = 0; i < 3; ++i)
        w_u32(output + i * 4, result[i]);
    a = r_u32(output);
    b = r_u32(origin);
    w_u32(output, a + b);
    a = r_u32(output + 4);
    b = r_u32(origin + 4);
    c = r_u32(output + 8);
    w_u32(output + 4, a + b);
    a = r_u32(origin + 8);
    w_u32(output + 8, c + a);
    return 0;
}

uint32 sub_8004ee68(uint32 index)
{
    uint32 object, table, slot, own, flag, descriptor, value, result;
    FUNCTION_MARKER(0x8004EE68, "ACE3.BIN");
    object = r_u32(0x800BCEC0) + index * 184;
    table = r_u32(0x800BCE5C);
    slot = r_u8(object + 63);
    own = r_u32(table + index * 4);
    flag = r_u8(object + 62);
    descriptor = r_u32(table + slot * 4);
    if (flag == 0)
        w_u8(object + 62, 1);
    sub_800451ac(descriptor + 8, object + 68, object + 104);
    sub_800505dc(index, object + 104);
    if (r_s32(object + 136) < 0x2000)
    {
        value = r_u32(0x800C6FD0 + index * 4);
        w_u32(0x800C6FD0 + index * 4, value | 0x20);
        w_u8(object + 99, 0);
    }
    else
        w_u8(object + 99, 4);
    value = r_u32(own);
    result = 7;
    if ((value & 0x40) == 0 && r_u8(object + 63) == 0)
        result = 4;
    w_u8(object + 97, result);
    return result;
}

uint32 sub_80044188(uint32 index, uint32 vector, uint32 speed, uint32 output, uint8 *native_output, const uint32 *native_vector)
{
    uint32 table, object, descriptor, value, a, b;
    sint32 pitch, yaw, roll;
    FUNCTION_MARKER(0x80044188, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(0x800BCEC0) + index * 184;
    speed = (uint32)(sint32)(sint16)speed;
    descriptor = r_u32(table + index * 4);
    if ((sint32)speed < 128)
        return 0;
    a = (native_vector != 0 ? native_vector[1] : r_u32(vector + 4));
    b = (native_vector != 0 ? native_vector[2] : r_u32(vector + 8));
    value = sub_80030d54(0 - a, b);
    if (native_output)
        xport_store_le16(native_output, value);
    else
        w_u16(output, value);
    a = (native_vector != 0 ? native_vector[0] : r_u32(vector));
    b = (native_vector != 0 ? native_vector[2] : r_u32(vector + 8));
    value = sub_80030d54(a, b);
    if (native_output)
        xport_store_le16(native_output + 2, value);
    else
        w_u16(output + 2, value);
    if ((sint32)speed < 768)
    {
        value = 0 - r_u16(descriptor + 28);
        if (native_output)
            xport_store_le16(native_output + 4, value);
        else
            w_u16(output + 4, value);
    }
    else if ((sint32)speed >= 0x1FFF)
    {
        if (native_output)
            xport_store_le16(native_output + 4, 0);
        else
            w_u16(output + 4, 0);
    }
    else
    {
        a = (native_vector != 0 ? native_vector[1] : r_u32(vector + 4));
        b = (native_vector != 0 ? native_vector[0] : r_u32(vector));
        value = sub_80030d54(b, 0 - a);
        pitch = native_output ? (sint16)xport_load_le16(native_output) : r_s16(output);
        if (native_output)
            xport_store_le16(native_output + 4, value);
        else
            w_u16(output + 4, value);
        if (pitch < 0 && pitch >= -1023)
        {
            yaw = native_output ? (sint16)xport_load_le16(native_output + 2) : r_s16(output + 2);
            if (yaw < 512 && yaw >= -511)
            {
                if (native_output)
                    xport_store_le16(native_output + 4, value + 0x2000);
                else
                    w_u16(output + 4, value + 0x2000);
            }
        }
        value = native_output ? xport_load_le16(native_output + 4) : r_u16(output + 4);
        if (native_output)
            xport_store_le16(native_output + 4, value & 0x3FFF);
        else
            w_u16(output + 4, value & 0x3FFF);
        value = native_output ? xport_load_le16(native_output) : r_u16(output);
        roll = native_output ? (sint16)xport_load_le16(native_output + 4) : r_s16(output + 4);
        if (native_output)
            xport_store_le16(native_output, value & 0x3FFF);
        else
            w_u16(output, value & 0x3FFF);
        value = native_output ? xport_load_le16(native_output + 4) : r_u16(output + 4);
        if (roll >= 0x2000)
        {
            if (native_output)
                xport_store_le16(native_output + 4, value - 0x4000);
            else
                w_u16(output + 4, value - 0x4000);
        }
        pitch = native_output ? (sint16)xport_load_le16(native_output) : r_s16(output);
        value = native_output ? xport_load_le16(native_output) : r_u16(output);
        if (pitch >= 0x2000)
        {
            if (native_output)
                xport_store_le16(native_output, value - 0x4000);
            else
                w_u16(output, value - 0x4000);
        }
    }
    value = native_output ? xport_load_le16(native_output + 4) : r_u16(output + 4);
    w_u16(object + 132, value);
    value = native_output ? xport_load_le16(native_output + 4) : r_u16(output + 4);
    if (((value + 127) & 0xFFFF) >= 255)
        return 1;
    value = native_output ? xport_load_le16(native_output) : r_u16(output);
    w_u16(object + 128, value);
    value = native_output ? xport_load_le16(native_output) : r_u16(output);
    if (((value + 127) & 0xFFFF) >= 255)
        return 1;
    value = native_output ? xport_load_le16(native_output + 2) : r_u16(output + 2);
    w_u16(object + 130, value);
    return 1;
}

uint32 sub_8004960c(uint32 index)
{
    uint8 angles[8];
    uint32 value, object, speed;
    FUNCTION_MARKER(0x8004960C, "ACE3.BIN");
    value = r_u32(0x800C6FD0 + index * 4);
    w_u32(0x800C6FD0 + index * 4, value | 0x20);
    object = r_u32(0x800BCEC0) + index * 184;
    speed = r_s16(object + 156);
    return sub_80044188(index, object + 140, speed, 0, angles, 0);
}

uint32 sub_80045090(uint32 vector, const uint32 *native_vector)
{
    uint32 x, y, ax, ay, total, shift, a, b, magnitude, z, result;
    FUNCTION_MARKER(0x80045090, "ACE3.BIN");
    x = native_vector ? native_vector[0] : r_u32(vector);
    y = native_vector ? native_vector[1] : r_u32(vector + 4);
    ax = x;
    ay = y;
    if ((sint32)ax < 0)
        ax = 0 - ax;
    if ((sint32)ay < 0)
        ay = 0 - ay;
    total = ax + ay;
    if ((sint32)total > 0x57FFF)
        magnitude = 0x100000;
    else
    {
        shift = (sint32)total > 0xAFFF ? 3 : 0;
        a = (uint32)((sint32)x >> shift);
        b = (uint32)((sint32)y >> shift);
        magnitude = SquareRoot0PSX(a * a + b * b, 0x800B6DD4) << shift;
    }
    z = native_vector ? native_vector[2] : r_u32(vector + 8);
    result = sub_80030d54(magnitude, z);
    if ((sint32)result < 0)
        result = 0 - result;
    return result;
}

uint32 sub_800449dc(uint32 index, uint32 value)
{
    uint32 object, descriptor, limit, raw;
    FUNCTION_MARKER(0x800449DC, "ACE3.BIN");
    object = r_u32(0x800BCEC0) + index * 184;
    descriptor = r_u32(0x800BCEF8);
    descriptor = r_u32(descriptor + (index << 6) + 20);
    if (value == 0)
        w_u16(object + 6, 0);
    else if ((sint32)value > 0)
    {
        limit = r_s16(descriptor + 30);
        raw = r_u16(descriptor + 30);
        w_u16(object + 6, (sint32)value < (sint32)limit ? value : raw);
    }
    else
    {
        limit = r_s16(descriptor + 30);
        raw = r_u16(descriptor + 30);
        limit = 0 - limit;
        w_u16(object + 6, (sint32)limit < (sint32)value ? value : 0 - raw);
    }
    return r_s16(object + 6);
}

uint32 sub_8004463c(uint32 angles)
{
    uint32 value, raw, yaw, roll, result;
    FUNCTION_MARKER(0x8004463C, "ACE3.BIN");
    value = r_s16(angles);
    raw = r_u16(angles);
    if ((sint32)value >= 0x1001)
    {
        w_u16(angles, 0x2000 - raw);
        yaw = r_u16(angles + 2);
        roll = r_u16(angles + 4);
        w_u16(angles + 2, yaw + 0x2000);
        w_u16(angles + 4, roll + 0x2000);
    }
    value = r_s16(angles);
    raw = r_u16(angles);
    if ((sint32)value < -0x1000)
    {
        w_u16(angles, 0xFFFFE000 - raw);
        yaw = r_u16(angles + 2);
        roll = r_u16(angles + 4);
        w_u16(angles + 2, yaw + 0x2000);
        w_u16(angles + 4, roll + 0x2000);
    }
    value = r_s16(angles + 2);
    raw = r_u16(angles + 2);
    if ((sint32)value >= 0x2000)
        w_u16(angles + 2, raw - 0x4000);
    value = r_s16(angles + 2);
    raw = r_u16(angles + 2);
    if ((sint32)value < -0x2000)
        w_u16(angles + 2, raw + 0x4000);
    value = r_s16(angles + 4);
    raw = r_u16(angles + 4);
    if ((sint32)value >= 0x2000)
        w_u16(angles + 4, raw - 0x4000);
    value = r_s16(angles + 4);
    raw = r_u16(angles + 4);
    result = raw + 0x4000;
    if ((sint32)value < -0x2000)
        w_u16(angles + 4, result);
    return result;
}

uint32 sub_8004470c(uint32 index, uint32 value)
{
    uint32 base, descriptor, profile, factor, speed, product, object, limit, raw, current, next, delta;
    FUNCTION_MARKER(0x8004470C, "ACE3.BIN");
    base = r_u32(0x800BCEF8) + (index << 6);
    descriptor = r_u32(base + 24);
    profile = r_u32(base + 20);
    factor = r_u8(descriptor + 1);
    speed = r_s16(profile + 24);
    ++factor;
    product = speed * factor;
    object = r_u32(0x800BCEC0) + index * 184;
    limit = (speed << 1) + speed + (uint32)((sint32)product >> 3);
    limit = (uint32)((sint32)limit >> 2);
    raw = r_u16(object + 8);
    current = (uint32)(sint32)(sint16)raw;
    if ((sint32)value >= 0)
    {
        if ((sint32)current < -1)
            next = (uint32)((sint32)current >> 1);
        else if ((sint32)(current << 1) >= (sint32)value)
            next = (uint32)((sint32)value >> 1);
        else
        {
            delta = (uint32)(sint32)(sint16)limit;
            if ((sint32)value < (sint32)delta)
                delta = value;
            next = raw + 1 + (uint32)((sint32)(delta * factor) >> 8);
        }
        w_u16(object + 8, next);
        current = r_s16(object + 8);
        if ((sint32)(sint16)limit < (sint32)current)
            w_u16(object + 8, limit);
    }
    else
    {
        if ((sint32)current > 0)
            next = (uint32)((sint32)current >> 1);
        else if ((sint32)value >= (sint32)(current << 1))
            next = (uint32)((sint32)value >> 1);
        else
        {
            delta = 0 - (uint32)(sint32)(sint16)limit;
            if ((sint32)delta < (sint32)value)
                delta = value;
            next = raw + (uint32)((sint32)(delta * factor) >> 8);
        }
        w_u16(object + 8, next);
        current = r_s16(object + 8);
        if ((sint32)current < (sint32)(0 - (uint32)(sint32)(sint16)limit))
            w_u16(object + 8, 0 - limit);
    }
    return r_s16(object + 8);
}

uint32 sub_80044884(uint32 index, uint32 value)
{
    uint32 object, base, profile, raw, current, next, limit, scene, descriptor, tick, rate, bound;
    FUNCTION_MARKER(0x80044884, "ACE3.BIN");
    object = r_u32(0x800BCEC0) + index * 184;
    base = r_u32(0x800BCEF8) + (index << 6);
    profile = r_u32(base + 20);
    raw = r_u16(object + 4);
    current = (uint32)(sint32)(sint16)raw;
    if ((sint32)value >= 0)
    {
        if ((sint32)current < -1)
            next = (uint32)((sint32)current >> 1);
        else if ((sint32)(current << 1) >= (sint32)value)
            next = (uint32)((sint32)value >> 1);
        else
        {
            limit = r_s16(profile + 26);
            if ((sint32)value >= (sint32)limit)
                value = limit;
            scene = r_u32(0x800BCE54);
            descriptor = r_u32(base + 24);
            tick = r_u32(scene + 12);
            rate = r_u8(descriptor + 1);
            if (rate < (tick & 3))
                goto positive_clamp;
            next = raw + 1 + (uint32)((sint32)value >> 4);
        }
        w_u16(object + 4, next);
    positive_clamp:
        current = r_s16(object + 4);
        limit = r_s16(profile + 26);
        bound = r_u16(profile + 26);
        if ((sint32)limit < (sint32)current)
            w_u16(object + 4, bound);
    }
    else
    {
        if ((sint32)current > 0)
            next = (uint32)((sint32)current >> 1);
        else if ((sint32)value >= (sint32)(current << 1))
            next = (uint32)((sint32)value >> 1);
        else
        {
            limit = r_s16(profile + 28);
            if ((sint32)limit >= (sint32)value)
                value = limit;
            scene = r_u32(0x800BCE54);
            descriptor = r_u32(base + 24);
            tick = r_u32(scene + 12);
            rate = r_u8(descriptor + 1);
            if (rate < (tick & 3))
                goto negative_clamp;
            next = raw + (uint32)((sint32)value >> 4);
        }
        w_u16(object + 4, next);
    negative_clamp:
        current = r_s16(object + 4);
        limit = r_s16(profile + 28);
        bound = r_u16(profile + 28);
        if ((sint32)current < (sint32)limit)
            w_u16(object + 4, bound);
    }
    return r_s16(object + 4);
}

uint32 sub_80044a6c(uint32 index)
{
    uint32 table, object, target, descriptor, value, delta, flag;
    FUNCTION_MARKER(0x80044A6C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(0x800BCEC0) + index * 184;
    target = r_u32(0x800C6ED0 + index * 4);
    descriptor = r_u32(table + index * 4);
    if (target && r_s32(target + 4) < 0x4000)
    {
        if (r_s16(object + 128) >= 0x800)
        {
            flag = r_u8(descriptor + 5);
            w_u8(descriptor + 5, flag | 8);
        }
        value = r_s16(object + 132);
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value >= 0x1000)
        {
            flag = r_u8(descriptor + 5);
            w_u8(descriptor + 5, flag | 8);
        }
        if (r_s16(object + 128) == 0 && r_s16(object + 132) == 0)
        {
            flag = r_u8(descriptor + 5);
            w_u8(descriptor + 5, flag & 0xF7);
        }
    }
    else
    {
        flag = r_u8(descriptor + 5);
        w_u8(descriptor + 5, flag & 0xF7);
    }
    delta = sub_80044884(index, r_s16(object + 128));
    if (delta)
    {
        value = r_u32(descriptor + 84);
        w_u16(descriptor + 48, delta);
        w_u32(descriptor + 84, value | 0x800000);
        value = r_u16(object + 128);
        w_u16(object + 128, value - delta);
    }
    delta = sub_8004470c(index, r_s16(object + 132));
    if (delta)
    {
        value = r_u16(descriptor + 28);
        w_u16(descriptor + 28, value + delta);
        value = r_u16(object + 132);
        w_u16(object + 132, value - delta);
    }
    delta = sub_800449dc(index, r_s16(object + 130));
    if (delta)
    {
        value = r_u32(descriptor + 84);
        w_u16(descriptor + 50, delta);
        w_u32(descriptor + 84, value | 0x800000);
        value = r_u16(object + 130);
        w_u16(object + 130, value - delta);
    }
    return sub_8004463c(descriptor + 24);
}

uint32 sub_80051498(void)
{
    uint32 index, slots, object, flags, state, target, descriptor;
    FUNCTION_MARKER(0x80051498, "ACE3.BIN");
    slots = r_u32(0x800BCE4C);
    for (index = 0; index < 32; ++index)
    {
        object = r_u32(slots + index * 4);
        flags = r_u32(object) & 0xFFFF7FFF;
        w_u32(object, flags);
        if ((flags & 0x4000) == 0)
            continue;
        state = r_u32(0x800C6FD0 + (index + 32) * 4);
        if (state & 1)
        {
            if (state & 0x10)
            {
                w_u32(object, flags | 0x8000);
                continue;
            }
            target = r_u32(0x800C6ED0 + (index + 32) * 4);
            if (r_u32(target) & 0x80)
            {
                w_u32(object, flags | 0x8000);
                continue;
            }
            descriptor = r_u32(0x800BCE5C);
            descriptor = r_u32(descriptor + (index + 32) * 4);
            if (r_s16(descriptor + 78) <= 0)
                w_u32(object, flags | 0x8000);
        }
        else if (state & 2)
            w_u32(object, flags | 0x8000);
    }
    return 0;
}

uint32 sub_80051414(void)
{
    uint32 index, table, object;
    FUNCTION_MARKER(0x80051414, "ACE3.BIN");
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE4C);
        object = r_u32(table + index * 4);
        if ((~r_u32(object) & 0xC000) == 0)
            sub_80051a54(index);
    }
    table = r_u32(0x800BCE60);
    if (r_s16(table + 908) == 41)
        return 41;
    return sub_80051840();
}

uint32 sub_8005157c(void)
{
    uint32 index, table, object, flags, a, b, c, d, e, f, scene, target, mode;
    FUNCTION_MARKER(0x8005157C, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    for (index = 0; index < 32; ++index)
    {
        object = r_u32(table + index * 4);
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        if (r_u32(0x800C6FD0 + (index + 32) * 4) & 0x400000)
            continue;
        flags = r_u32(object + 84);
        if (flags & 0x80000000)
        {
            a = r_u32(object + 8);
            b = r_u32(object + 56);
            c = r_u32(object + 60);
            d = r_u32(object + 64);
            w_u32(object + 8, a + b);
            e = r_u32(object + 12);
            f = r_u32(object + 16);
            w_u32(object + 12, e + c);
            w_u32(object + 16, f + d);
        }
        flags = r_u32(object + 84);
        w_u32(object + 84, flags & 0xFF000000);
    }
    table = r_u32(0x800BCE60);
    scene = r_s16(table + 908);
    if (scene == 8 || scene == 48)
    {
        mode = scene == 8 ? 0 : 1;
        for (index = 0; index < 32; ++index)
        {
            table = r_u32(0x800BCE4C);
            object = r_u32(table + index * 4);
            if ((r_u32(object) & 0x4000) == 0)
                continue;
            if ((r_u32(0x800C6FD0 + (index + 32) * 4) & 1) == 0)
                continue;
            target = r_u32(0x800C6ED0 + (index + 32) * 4);
            if (r_s32(target + 4) >= 0x3001)
                continue;
            if (r_s16(object + 72) != (mode == 0 ? 65 : 62))
                continue;
            table = r_u32(0x800BCE60);
            table = r_u32(table + 40);
            sub_80083ca4(object + 8, table + index * 8, mode);
        }
    }
    return scene == 48 ? 0 : 48;
}

uint32 sub_800517f4(void)
{
    FUNCTION_MARKER(0x800517F4, "ACE3.BIN");
    sub_80051498();
    sub_80051414();
    return sub_8005157c();
}

uint32 sub_80051840(void)
{
    uint32 voice, scene, slots, index, object, kind, target, distance, value, selected, table, listener, source;
    FUNCTION_MARKER(0x80051840, "ACE3.BIN");
    voice = r_u8(0x800BCFF0);
    if (voice != 255)
    {
        scene = r_u32(0x800BCE54);
        if ((r_u32(scene + 4) & 4) == 0)
        {
            sub_8002dda8(r_u8(0x800BCFF0));
            w_u8(0x800BCFF0, 255);
        }
    }
    slots = r_u32(0x800BCE4C);
    w_u8(0x800BCFF1, 255);
    w_u32(0x800BCFF4, 0x100000);
    for (index = 0; index < 32; ++index)
    {
        object = r_u32(slots + index * 4);
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        kind = r_u16(object + 72);
        if ((kind - 35) >= 2 && (sint16)kind != 43)
            continue;
        target = r_u32(0x800C6ED0 + (index + 32) * 4);
        if (target == 0)
            continue;
        distance = r_u32(0x800BCFF4);
        value = r_u32(target + 4);
        if ((sint32)value < (sint32)distance)
        {
            value = r_u32(target + 4);
            w_u8(0x800BCFF1, index + 32);
            w_u32(0x800BCFF4, value);
        }
    }
    voice = r_u8(0x800BCFF0);
    if (voice != 255)
    {
        selected = r_u8(0x800BCFF1);
        if (selected == 255 || r_s32(0x800BCFF4) >= 0x2000)
        {
            value = sub_8002dda8(r_u8(0x800BCFF0));
            w_u8(0x800BCFF0, 255);
            return value;
        }
    }
    selected = r_u8(0x800BCFF1);
    if (selected == 255)
        return 255;
    voice = r_u8(0x800BCFF0);
    if (voice == 255)
    {
        if (r_s32(0x800BCFF4) < 0x2000)
            w_u8(0x800BCFF0, sub_8002dcb0(92));
        voice = r_u8(0x800BCFF0);
        if (voice == 255)
            return 255;
    }
    selected = r_u8(0x800BCFF1);
    table = r_u32(0x800BCE5C);
    scene = r_u32(0x800BCE54);
    listener = r_u32(scene + 40);
    source = r_u32(table + selected * 4);
    sub_8002e6dc(0x800C77E0, listener + 4, source + 8, 0, 0, 0);
    sub_8002e67c(r_u8(0x800BCFF0));
    sub_8002e84c(r_u8(0x800BCFF0), 0x800C77E0, 2, 0);
    sub_8002e8dc(r_u8(0x800BCFF0), 0x800C77E0, 0);
    return sub_8002ec48(r_u8(0x800BCFF0), 0x800C77E0);
}

uint32 sub_80083ca4(uint32 position, uint32 value, uint32 mode)
{
    uint32 variant, result, effect, flags, parameters;
    FUNCTION_MARKER(0x80083CA4, "ACE3.BIN");
    variant = value;
    if (mode == 0)
    {
        result = (uint32)((sint32)value >> 4);
        if (value & 15)
            return result;
        variant = 0;
        if ((result & 7) == 0)
            variant = (game_random() & 7) != 0;
    }
    else if (mode == 1)
    {
        result = value & 3;
        if (result)
            return result;
    }
    effect = sub_8007ece8(6, position, 0);
    parameters = r_u32(effect + 8);
    flags = r_u32(effect + 4);
    parameters = (parameters & 0xFFFFF00F) | ((variant & 0xFF) << 4);
    flags = (flags & 0xFFFFF0FF) | ((mode & 15) << 8);
    w_u32(effect + 4, flags);
    w_u32(effect + 8, parameters);
    return sub_80083c3c(position, variant, mode, 0);
}

uint32 sub_80051a54(uint32 index)
{
    uint32 state, slot, flags, record, scene, value;
    FUNCTION_MARKER(0x80051A54, "ACE3.BIN");
    state = r_u32(0x800BCEFC) + index * 40;
    slot = 0x800C6FD0 + (index + 32) * 4;
    flags = r_u32(slot);
    record = r_u32(state + 24);
    if (flags & 0x20000)
        sub_80051ba0(index);
    flags = r_u32(slot);
    if ((flags & 1) == 0)
    {
        sub_80052060(index);
        return 0;
    }
    if (record && r_u8(record + 1) == 1 && r_u8(record + 3) != 0)
        w_u32(slot, flags | 0x20000);
    sub_800520c8(index);
    if ((r_u32(slot) & 1) == 0)
        return 0;
    sub_800522b0(index);
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 4);
    if (value != 4)
        return 4;
    value = r_u32(state + 16);
    if (value == 0)
        return 0;
    return sub_80052fa8(index);
}

uint32 sub_80052060(uint32 index)
{
    uint32 state, table, descriptor, value, object;
    FUNCTION_MARKER(0x80052060, "ACE3.BIN");
    state = r_u32(0x800BCEFC);
    table = r_u32(0x800BCE4C);
    descriptor = r_u32(state + index * 40 + 20);
    value = r_u8(descriptor);
    object = r_u32(table + index * 4);
    if (value != 6)
        return value;
    value = r_s8(object + 88);
    if (value != 0)
        return value;
    return sub_80051f88(index);
}

uint32 sub_80051f88(uint32 index)
{
    uint32 table, object, value, slot;
    FUNCTION_MARKER(0x80051F88, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    object = r_u32(table + index * 4);
    value = r_u32(object);
    w_u32(object, value & 0xFFFE7BFF);
    slot = index + 32;
    value = r_u32(0x800C6FD0 + slot * 4);
    w_u32(0x800C6FD0 + slot * 4, (value & 0xFF000000) | 4);
    w_u8(0x800C70D0 + slot, 150);
    return 150;
}

uint32 sub_80052000(uint32 index)
{
    uint32 table, state, record, object, value;
    FUNCTION_MARKER(0x80052000, "ACE3.BIN");
    table = r_u32(0x800BCE4C);
    state = r_u32(0x800BCEFC);
    record = r_u32(state + index * 40 + 24);
    object = r_u32(table + index * 4);
    if (record == 0)
        return 0;
    value = r_u8(record);
    if (value == 0)
        return 0;
    return sub_8008130c(object + 8, (value & 0x7F) - 1);
}

uint32 sub_80051e34(uint32 index)
{
    uint32 table, slot, flags, object, byte, status, state, record, value;
    FUNCTION_MARKER(0x80051E34, "ACE3.BIN");
    slot = 0x800C6FD0 + (index + 32) * 4;
    table = r_u32(0x800BCE4C);
    flags = r_u32(slot);
    object = r_u32(table + index * 4);
    w_u32(slot, flags & 0xFFFDFFFF);
    byte = r_u8(object + 5);
    status = r_u32(object + 84);
    w_u16(object + 6, 0);
    w_u8(object + 5, byte & 0xFB);
    state = r_u32(0x800BCEFC);
    w_u32(object + 84, status & 0x7FFFFFFF);
    record = r_u32(state + index * 40 + 24);
    if (record == 0)
        return 0;
    value = r_u8(record + 14);
    if (value == 254)
        sub_80051f88(index);
    else if (value != 255)
    {
        flags = r_u32(object);
        w_u32(object, flags | 0x10000);
        value = r_u8(record + 14);
        w_u8(object + 4, value);
    }
    value = r_u8(record + 15);
    if (value != 255)
        sub_80048ad4(value);
    if (r_u8(record + 1) == 3 && r_u8(record + 3) != 0)
    {
        flags = r_u32(slot);
        w_u32(slot, flags | 0x20000);
    }
    return sub_80052000(index);
}

uint32 sub_800520c8(uint32 index)
{
    uint32 slot, table, flags, object, health, base, value, scene, state, script;
    FUNCTION_MARKER(0x800520C8, "ACE3.BIN");
    slot = 0x800C6FD0 + (index + 32) * 4;
    table = r_u32(0x800BCE4C);
    flags = r_u32(slot);
    object = r_u32(table + index * 4);
    if ((flags & 1) == 0)
        return 0;
    health = r_s16(object + 78);
    if ((sint32)health > 0)
        return health;
    flags = r_u32(object);
    w_u32(object, flags & 0xFFFFFDFF);
    if (r_u32(slot) & 0x100)
    {
        base = r_u32(0x800BCEB4);
        value = r_u8(base + 32);
        w_u8(base + 32, value + 1);
    }
    if (r_u32(slot) & 0x200)
    {
        base = r_u32(0x800BCEB4);
        value = r_u8(base + 33);
        w_u8(base + 33, value + 1);
    }
    if ((r_u32(slot) & 0x800) && (r_u32(object + 80) & 0x3F) == 0)
    {
        flags = r_u32(object);
        if ((flags & 0x40) == 0)
        {
            scene = r_u32(0x800BCE60);
            if (r_u8(scene + 925) & 1)
                sub_800797fc(flags);
            scene = r_u32(0x800BCE60);
            if (r_s16(scene + 908) == 22 && r_s16(object + 72) == 70)
                sub_8004ceb0(46);
            else if (r_u32(object) & 0x80)
                sub_8004ceb0(7);
        }
    }
    sub_80048758(index + 32);
    flags = r_u32(slot);
    w_u32(slot, (flags & 0xFF000000) | 2);
    sub_80051e34(index);
    state = r_u32(0x800BCEFC) + index * 40;
    script = r_u32(state + 28);
    if (script)
        return sub_80054ebc(script);
    return state;
}

uint32 sub_80048ad4(uint32 id)
{
    uint32 table, record, value, kind, index, parameter;
    FUNCTION_MARKER(0x80048AD4, "ACE3.BIN");
    table = r_u32(0x800BCF1C);
    value = r_u8(table + id);
    table = r_u32(0x800BCEE0);
    record = table + value * 8;
    while (r_s16(record) != 0)
    {
        kind = r_u8(record + 2);
        index = r_u8(record + 3);
        parameter = r_s16(record + 6);
        sub_8005e460(kind, index, (sint32)parameter);
        record += 8;
    }
    return 0;
}

uint32 sub_8008130c(uint32 position, uint32 kind)
{
    uint32 effect, parameters;
    FUNCTION_MARKER(0x8008130C, "ACE3.BIN");
    effect = sub_8007ece8(4, position, 0);
    parameters = r_u32(effect + 8);
    parameters = (parameters & 0xFFFFF00F) | ((kind & 0xFF) << 4);
    w_u32(effect + 8, parameters);
    return sub_80089bb4(position, kind, 0);
}

uint32 sub_8006ec70(uint32 altitude, uint32 first, uint32 second, uint32 third, uint32 *native_first, uint32 *native_second, uint32 *native_third)
{
    uint32 height, numerator, ratio0, ratio1, product0, product1, phase, sine0, sine1, a, b, c, cap, blend;
    FUNCTION_MARKER(0x8006EC70, "ACE3.BIN");
    height = (uint32)((sint32)altitude >> 12);
    if ((sint32)height > 0)
        height = 0;
    if ((sint32)height < -50000)
        height = (uint32)-50000;
    numerator = 0 - ((height + 50000) << 14);
    ratio0 = (uint32)((sint32)numerator / -50000);
    product0 = ratio0 * 0x7000;
    numerator = height << 14;
    ratio1 = (uint32)((sint32)numerator / -50000);
    phase = (uint32)((sint32)((ratio0 << 2) + ratio0) >> 2) & 0x7FFE;
    sine0 = r_s16(0x800104AC + phase);
    product1 = sine0 * 0x1C00;
    sine1 = r_s16(0x800104AC + (ratio1 & 0x7FFE));
    a = (uint32)((sint32)(ratio1 * 0xB800) >> 12) + 0xD000 - (uint32)((sint32)(sine1 * 0xB800) >> 14);
    b = (uint32)((sint32)product0 >> 14) + 0x52000 + (uint32)((sint32)product1 >> 13);
    cap = a + 0x4A000;
    if ((sint32)cap < (sint32)b)
        b = cap;
    numerator = (height + 37500) << 12;
    blend = (uint32)((sint32)numerator / -12500);
    if (native_first)
        *native_first = a;
    else
        w_u32(first, a);
    if (native_third)
        *native_third = b;
    else
        w_u32(third, b);
    c = (uint32)((sint32)(ratio1 * 0xA200) >> 12) + 0x1E000;
    if (native_second)
        *native_second = c;
    else
        w_u32(second, c);
    blend = 0x1000 - blend;
    if ((sint32)blend >= 0x1001)
        blend = 0x1000;
    return blend;
}

uint32 sub_80045768(void)
{
    uint32 index, profiles, objects, object, flags, target, value, raw, descriptor, count;
    FUNCTION_MARKER(0x80045768, "ACE3.BIN");
    profiles = r_u32(0x800BCEF8) + 128;
    objects = r_u32(0x800BCE5C) + 8;
    for (index = 2; index < 64; ++index, profiles += 64, objects += 4)
    {
        object = r_u32(objects);
        flags = r_u32(object);
        target = r_u32(0x800C6ED0 + index * 4);
        if ((~flags & 0xC000) != 0 || target == 0)
            continue;
        flags = r_u32(target);
        if (flags & 0x40)
        {
            value = r_s16(target + 30);
            raw = r_u16(target + 30);
            if ((sint32)value < 0x7FFF)
                w_u16(target + 30, raw + 1);
        }
        else
            w_u16(target + 30, 0);
        flags = r_u32(target);
        if (flags & 8)
        {
            value = r_s16(target + 28);
            raw = r_u16(target + 28);
            if ((sint32)value < 0x7FFF)
                w_u16(target + 28, raw + 1);
        }
        else
            w_u16(target + 28, 0);
        if (index >= 32)
            continue;
        flags = r_u32(target);
        if (flags & 4)
        {
            value = r_u16(target + 26);
            descriptor = r_u32(profiles + 24);
            ++value;
            w_u16(target + 26, value);
            count = r_s16(descriptor + 12);
            if ((sint32)(sint16)value >= (sint32)count)
            {
                flags = r_u32(target);
                w_u32(target, flags | 0x2000000);
            }
        }
        else
            w_u16(target + 26, 0);
    }
    return 0;
}

uint32 sub_80044c28(uint32 index)
{
    uint32 table, state, profile_base, object, profile, target, descriptor, limit, step, altitude;
    uint32 first, middle, third, minimum, maximum, desired, mode, slot, other, value, delta, current, flags, next;
    FUNCTION_MARKER(0x80044C28, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    state = r_u32(0x800BCEC0) + index * 184;
    profile_base = r_u32(0x800BCEF8);
    object = r_u32(table + index * 4);
    profile_base += index << 6;
    target = r_u32(0x800C6ED0 + index * 4);
    profile = r_u32(profile_base + 20);
    if (target == 0)
        return 1;
    descriptor = r_u32(profile_base + 24);
    limit = r_u32(profile + 16);
    step = r_u8(descriptor + 1) + 1;
    altitude = r_u32(object + 12);
    sub_8006ec70(altitude, 0, 0, 0, &first, &middle, &third);
    minimum = r_u32(profile + 12);
    mode = r_u8(state + 97);
    desired = r_u32(profile + 4);
    maximum = r_u32(profile + 8);
    switch (mode)
    {
        case 0:
            desired = r_u32(profile + 4);
            break;
        case 1:
            desired = r_u32(profile + 8);
            break;
        case 2:
            desired = r_u32(profile + 12);
            break;
        case 3:
            desired = middle;
            break;
        case 4:
            slot = r_u8(state + 63);
            table = r_u32(0x800BCE5C);
            other = r_u32(table + slot * 4);
            if (slot == 0 || r_u8(state + 96) == 0)
            {
                if (r_s32(state + 148) < 0)
                    desired = middle;
                else
                {
                    value = r_u32(state + 136);
                    desired = middle + (value << 3);
                }
            }
            else if ((r_u32(object) & 0x40) == 0 || r_s32(state + 148) < 0)
                desired = r_u32(profile + 4);
            else
            {
                value = r_u32(state + 136);
                desired = r_u32(other + 40) + (value << 3);
            }
            break;
        case 5:
        case 7:
            if (mode == 5)
                limit = 256;
            slot = r_u8(state + 63);
            table = r_u32(0x800BCE5C);
            other = r_u32(table + slot * 4);
            if (mode == 5)
                step = 8;
            desired = slot ? r_u32(other + 40) : middle;
            if (r_s32(state + 148) < 0)
            {
                value = r_u32(state + 136);
                delta = 0 - value;
            }
            else
            {
                delta = r_u32(state + 136);
                value = r_u32(state + 136);
            }
            if ((sint32)value >= 0x4001)
                delta <<= 3;
            desired += delta;
            break;
        case 6:
            slot = r_u8(state + 63);
            table = r_u32(0x800BCE5C);
            value = r_u32(state + 148);
            other = r_u32(table + slot * 4);
            if ((sint32)value < 0)
                desired = r_u32(profile + 4);
            else
            {
                value = r_u32(state + 136);
                desired = r_u32(other + 40) + value;
            }
            break;
        case 8:
            limit = 256;
            step = 8;
            desired = middle - (uint32)((sint32)middle >> 3);
            break;
        case 9:
            return 0;
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
    value = r_u32(object + 40);
    current = r_u32(state + 20);
    delta = desired - value;
    if ((sint32)current < (sint32)delta)
    {
        w_u32(state + 20, current + step);
        current = r_u32(state + 20);
    }
    else if ((sint32)delta < (sint32)current)
    {
        w_u32(state + 20, current - step);
        current = r_u32(state + 20);
    }
    if ((sint32)limit < (sint32)current)
        w_u32(state + 20, limit);
    else if ((sint32)current < (sint32)(0 - limit))
        w_u32(state + 20, 0 - limit);
    flags = r_u32(object + 84);
    value = r_u32(object + 40);
    w_u32(object + 84, flags | 0x200000);
    delta = r_u32(state + 20);
    next = value + delta;
    w_u32(object + 40, next);
    if ((sint32)maximum < (sint32)next)
        w_u32(object + 40, maximum);
    else if ((sint32)next < (sint32)minimum)
        w_u32(object + 40, minimum);
    return 0;
}

uint32 sub_8004c5b8(void)
{
    uint32 count, index, table, record, from, to, destination, source, a, b, c, d, offset, flags, object, value;
    FUNCTION_MARKER(0x8004C5B8, "ACE3.BIN");
    count = r_u8(0x800BCEF1);
    if (count == 0)
        return 0;
    index = 0;
    do
    {
        table = r_u32(0x800BCF18);
        if (r_u32(table + index * 4) & 1)
        {
            table = r_u32(0x800BCF0C);
            record = table + index * 20;
            to = r_u8(record + 3);
            from = r_u8(record + 2);
            destination = r_u32(0x800C6ED0 + to * 4);
            if (from == 0)
                w_u32(destination + 4, 1);
            else
            {
                source = r_u32(0x800C6ED0 + from * 4);
                for (offset = 0; offset < 32; offset += 16)
                {
                    a = r_u32(source + offset);
                    b = r_u32(source + offset + 4);
                    c = r_u32(source + offset + 8);
                    d = r_u32(source + offset + 12);
                    w_u32(destination + offset, a);
                    w_u32(destination + offset + 4, b);
                    w_u32(destination + offset + 8, c);
                    w_u32(destination + offset + 12, d);
                }
                a = r_u32(source + 32);
                b = r_u32(source + 36);
                w_u32(destination + 32, a);
                w_u32(destination + 36, b);
            }
            table = r_u32(0x800BCE5C);
            object = r_u32(table + from * 4);
            flags = r_u32(object);
            if (flags & 0x4000)
            {
                object = r_u32(table + to * 4);
                value = r_u32(object);
                if ((value & 0x4000) && (flags & 0x2000))
                    w_u32(object, value | 0x2000);
            }
        }
        count = r_u8(0x800BCEF1);
        ++index;
    } while ((sint32)index < (sint32)count);
    return 0;
}

uint32 sub_80044f94(uint32 origin, uint32 vector, uint32 output, const uint32 *native_vector, uint32 *native_output)
{
    MATRIX matrix = {0};
    uint32 i, value, base, input[3], high[3], low[3], result[3], magnitude;
    FUNCTION_MARKER(0x80044F94, "ACE3.BIN");
    sub_800308f0(origin + 16, 0, &matrix, 0);
    for (i = 0; i < 3; ++i)
    {
        value = native_vector ? native_vector[i] : r_u32(vector + i * 4);
        base = r_u32(origin + i * 4);
        input[i] = (uint32)((sint32)(value - base) >> 12);
    }
    for (i = 0; i < 5; ++i)
        xport_gte_write_control(i, xport_load_le32((uint8 *)&matrix + i * 4));
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (uint32)((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    for (i = 0; i < 3; ++i)
    {
        if (native_output)
            native_output[i] = result[i];
        else
            w_u32(output + i * 4, result[i]);
    }
    return output;
}

uint32 sub_800455b8(void)
{
    uint32 index, table, object, state, flags, value, other, a, b, c, d;
    FUNCTION_MARKER(0x800455B8, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        state = r_u32(0x800BCEC0);
        flags = r_u32(object);
        state += index * 184;
        if ((~flags & 0xC000) != 0)
            continue;
        flags = r_u32(0x800C6FD0 + index * 4);
        if (flags & 0x20)
        {
            value = sub_80083dfc(object + 8, state + 104, 0, 0);
            w_u32(state + 136, value);
            sub_80044f94(object + 8, state + 104, state + 140, 0, 0);
            value = sub_80045090(state + 140, 0);
            w_u16(state + 156, value);
            flags = r_u32(0x800C6FD0 + index * 4);
            w_u32(0x800C6FD0 + index * 4, flags & 0xFFFFFFDF);
        }
        value = r_u8(state + 96);
        if (value != 255)
        {
            table = r_u32(0x800BCE5C);
            other = r_u32(table + value * 4) + 8;
            value = sub_80083dfc(object + 8, other, 0, 0);
            w_u32(state + 160, value);
            sub_80044f94(object + 8, other, state + 164, 0, 0);
            value = sub_80045090(state + 164, 0);
            flags = r_u32(state + 160);
            w_u16(state + 180, value);
            if ((sint32)flags < 0x2000)
            {
                value = r_u16(state + 182);
                w_u16(state + 182, value + 16);
            }
        }
        else
        {
            value = r_u32(state + 136);
            w_u32(state + 160, value);
            a = r_u32(state + 140);
            b = r_u32(state + 144);
            c = r_u32(state + 148);
            d = r_u32(state + 152);
            w_u32(state + 164, a);
            w_u32(state + 168, b);
            w_u32(state + 172, c);
            w_u32(state + 176, d);
            value = r_u16(state + 156);
            w_u16(state + 182, 0);
            w_u16(state + 180, value);
        }
    }
    return 0;
}

uint32 sub_80045024(uint32 origin, uint32 matrix, uint32 vector, uint32 output)
{
    uint32 input[3], controls[5], high[3], low[3], result[3], i, value, base, magnitude;
    FUNCTION_MARKER(0x80045024, "ACE3.BIN");
    value = r_u32(vector);
    base = r_u32(origin);
    input[0] = (uint32)((sint32)(value - base) >> 12);
    value = r_u32(vector + 4);
    base = r_u32(origin + 4);
    input[1] = (uint32)((sint32)(value - base) >> 12);
    base = r_u32(origin + 8);
    value = r_u32(vector + 8);
    input[2] = (uint32)((sint32)(value - base) >> 12);
    for (i = 0; i < 5; ++i)
        controls[i] = r_u32(matrix + i * 4);
    for (i = 0; i < 5; ++i)
        xport_gte_write_control(i, controls[i]);
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (uint32)((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    for (i = 0; i < 3; ++i)
        w_u32(output + i * 4, result[i]);
    return output;
}

uint32 sub_80045278(void)
{
    uint32 table, player, index, object, target, flags, distance, scene, value, angle, threshold, cached;
    FUNCTION_MARKER(0x80045278, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    player = r_u32(table);
    sub_800308f0(player + 24, 0x800C6EA0, 0, 0);
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        flags = r_u32(object);
        target = r_u32(0x800C6ED0 + index * 4);
        flags &= 0xFFFFDFFF;
        w_u32(object, flags);
        if ((flags & 0x4000) == 0)
            continue;
        if (r_u32(0x800C6FD0 + index * 4) & 0x400000)
            continue;
        distance = r_u32(target + 4);
        w_u32(target, 0);
        if (distance == 0x100000)
        {
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 12);
            if (index != (value & 0x3F))
                continue;
        }
        else if ((sint32)distance > 34000)
        {
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 12);
            if ((index & 15) != (value & 15))
                continue;
        }
        distance = sub_80083dfc(object + 8, player + 8, 0, 0);
        w_u32(target + 4, distance);
        if ((index < 32 && (sint32)distance <= 33999) || (index >= 32 && (sint32)distance < 0x3000))
        {
            flags = r_u32(target);
            w_u32(target, flags | 0x80);
        }
        if (r_s32(target + 4) > 0x7FFF)
            continue;
        flags = r_u32(target);
        w_u32(target, flags | 1);
        flags = r_u32(object);
        w_u32(object, flags | 0x2000);
        if (r_s32(target + 4) > 0x7FFF)
            continue;
        flags = r_u32(target);
        w_u32(target, flags | 0x1000000);
        sub_80045024(player + 8, 0x800C6EA0, object + 8, target + 8);
        angle = sub_80045090(target + 8, 0);
        w_u16(target + 24, angle);
        if ((sint16)angle < 0x600)
        {
            flags = r_u32(target);
            w_u32(target, flags | 2);
        }
        if (r_s32(target + 4) < 0x3000 && r_s16(target + 24) < 0x600)
        {
            flags = r_u32(target);
            w_u32(target, flags | 0x40);
        }
        if (r_s32(target + 4) < 0x2000 && r_s16(target + 24) < 0x600)
        {
            flags = r_u32(target);
            w_u32(target, flags | 4);
        }
        distance = r_u32(target + 4);
        threshold = r_u32(0x800C6EC4);
        if ((sint32)distance < (sint32)threshold)
        {
            cached = r_u32(target);
            w_u32(target, cached | 0x100);
            angle = r_s16(target + 24);
            threshold = r_u32(0x800C6EC0);
            if ((sint32)angle < (sint32)threshold)
                w_u32(target, cached | 0x108);
            if (r_s16(target + 24) < 0x600)
            {
                flags = r_u32(target);
                w_u32(target, flags | 0x10);
            }
        }
        distance = r_u32(target + 4);
        threshold = r_u32(0x800C6EC8);
        if ((sint32)distance < (sint32)threshold)
        {
            flags = r_u32(target);
            if (flags & 2)
                w_u32(target, flags | 0x20);
        }
    }
    return 0;
}

uint32 sub_80045240(void)
{
    FUNCTION_MARKER(0x80045240, "ACE3.BIN");
    sub_80045278();
    sub_8004c5b8();
    sub_800455b8();
    return sub_80045768();
}

uint32 sub_8004bed4(void)
{
    uint32 index, count, table, slot, flags, record, from, to, source, destination, kind, value, a, b, c, d;
    uint32 phase, pitch, sine, cosine, product0, product1, scene, owner;
    FUNCTION_MARKER(0x8004BED4, "ACE3.BIN");
    count = r_u8(0x800BCEF1);
    if (count == 0)
        return 0;
    index = 0;
    do
    {
        table = r_u32(0x800BCF18);
        slot = table + index * 4;
        flags = r_u32(slot);
        if ((flags & 1) == 0)
            goto next_record;
        table = r_u32(0x800BCF0C);
        scene = r_u32(0x800BCE5C);
        record = table + index * 20;
        from = r_u8(record + 2);
        to = r_u8(record + 3);
        source = r_u32(scene + from * 4);
        kind = r_u16(record);
        destination = r_u32(scene + to * 4);
        if (kind == 3)
        {
            if (from == 0)
            {
                value = r_u8(destination + 5);
                w_u8(destination + 4, 0);
                w_u8(destination + 5, value | 1);
            }
            value = r_u32(source + 8);
            w_u32(destination + 8, value);
            value = r_u32(source + 12);
            w_u32(destination + 12, 0xFFFBE000 - value);
            value = r_u32(source + 16);
            w_u32(destination + 16, value);
            value = r_u16(source + 24);
            w_u16(destination + 24, 0 - value);
            value = r_u16(source + 26);
            w_u16(destination + 26, value);
            value = r_u16(source + 28);
            w_u16(destination + 28, 0x2000 - value);
            value = r_u32(source);
            if ((value & 0x10000) == 0)
            {
                value = r_u32(destination);
                w_u32(destination, value & 0xFFFEFFFF);
            }
        }
        else if (kind == 4)
        {
            sub_800451ac(source + 8, record + 4, destination + 8);
            value = r_u16(source + 24);
            w_u16(destination + 24, value);
            value = r_u16(source + 26);
            w_u16(destination + 26, value);
            value = r_u16(source + 28);
            w_u16(destination + 28, value + 0x1FFF);
            a = r_u32(source + 56);
            b = r_u32(source + 60);
            c = r_u32(source + 64);
            d = r_u32(source + 68);
            w_u32(destination + 56, a);
            w_u32(destination + 60, b);
            w_u32(destination + 64, c);
            w_u32(destination + 68, d);
            value = r_u32(destination + 84);
            w_u32(destination + 84, value & 0x7FFFFFFF);
        }
        else if (kind == 5)
        {
            a = r_s16(destination + 26);
            b = r_s16(source + 26);
            phase = (a - b) & 0x3FFF;
            if ((phase - 0x1C01) < 0x7FF)
                w_u32(slot, flags ^ 0x10);
            phase = r_s16(source + 26);
            table = r_u32(0x800BCF18);
            flags = r_u32(table + index * 4);
            phase = (phase + 0x1000) & 0x3FFF;
            if (flags & 0x10)
                phase = (phase + 0x2000) & 0x3FFF;
            pitch = r_s16(source + 24);
            sine = r_s16(0x800104AC + phase * 2);
            pitch &= 0x3FFF;
            cosine = r_s16(0x800104AC + (pitch + 0x1000) * 2);
            product0 = sine * cosine;
            value = r_s16(0x800104AC + (phase + 0x1000) * 2);
            product1 = value * cosine;
            value = r_u32(source + 8);
            w_u32(destination + 8, value + (uint32)((sint32)product0 >> 9));
            a = r_s16(0x800104AC + pitch * 2);
            value = r_u32(source + 12);
            w_u32(destination + 12, value + (a << 5));
            value = r_u32(source + 16);
            w_u32(destination + 16, value + (uint32)((sint32)product1 >> 9));
            a = r_u32(source + 24);
            b = r_u32(source + 28);
            w_u32(destination + 24, a);
            w_u32(destination + 28, b);
            value = r_u8(source + 5);
            flags = r_u8(destination + 5);
            w_u8(destination + 5, (value & 8) ? flags | 8 : flags & 0xF7);
            table = r_u32(0x800BD094);
            if (r_u32(table + 16) & 0x4000)
            {
                value = r_u8(0x800C6D72);
                owner = 0xFFFFFFFF;
                if (value != 255)
                {
                    value = r_u8(0x800C6D6E);
                    table = r_u32(0x800BCE5C);
                    table = r_u32(table + value * 4);
                    owner = r_u32(table);
                }
                flags = r_u32(source);
                sub_80068800(flags, destination + 8, source + 56, owner, 0x102, 0);
            }
        }
        else
        {
            sub_800451ac(source + 8, record + 4, destination + 8);
            if (kind == 6)
                scene = r_u32(0x800BCE54);
            a = r_u32(source + 24);
            b = r_u32(source + 28);
            w_u32(destination + 24, a);
            w_u32(destination + 28, b);
            if (kind == 6)
            {
                value = r_u16(scene + 12);
                w_u16(destination + 28, (value & 0x3F) << 8);
            }
        }
    next_record:
        count = r_u8(0x800BCEF1);
        ++index;
    } while ((sint32)index < (sint32)count);
    return 0;
}

uint32 sub_80048658(void)
{
    uint32 scene, tick, table, data, player, high, quotient, count, value, coordinate, offset;
    FUNCTION_MARKER(0x80048658, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    tick = r_u32(scene + 12);
    table = r_u32(0x800BCE5C);
    data = r_u32(0x800BCE60);
    player = r_u32(table);
    data += 952;
    high = (uint32)(((sint64)(sint32)tick * (sint32)0x88888889) >> 32);
    quotient = (uint32)((sint32)(high + tick) >> 6) - (uint32)((sint32)tick >> 31);
    if (tick == (((quotient << 4) - quotient) << 3))
    {
        count = r_s16(data + 8);
        if ((sint32)count < 450)
        {
            coordinate = r_u32(player + 8);
            value = (uint32)((sint32)coordinate >> 12) + 0x40000;
            value = (uint32)((sint32)value >> 11);
            if ((sint32)value < 0)
                value = 0;
            if ((sint32)value >= 256)
                value = 255;
            offset = data + (count << 1);
            w_u8(offset + 12, value);
            coordinate = r_u32(player + 16);
            value = (uint32)((sint32)coordinate >> 12) + 0x40000;
            value = (uint32)((sint32)value >> 11);
            if ((sint32)value < 0)
                value = 0;
            if ((sint32)value >= 256)
                value = 255;
            count = r_s16(data + 8);
            w_u8(data + (count << 1) + 13, value);
            count = r_u16(data + 8);
            w_u16(data + 8, count + 1);
        }
    }
    value = r_u16(data + 2) + 1;
    w_u16(data + 2, value);
    return value;
}

uint32 sub_8004658c(void)
{
    uint32 scene, value, table;
    FUNCTION_MARKER(0x8004658C, "ACE3.BIN");
    sub_8004bed4();
    sub_80045240();
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 4) & 4;
    if (value == 0)
        return 0;
    table = r_u32(0x800BCE60);
    if (r_s16(table + 908) != 9)
        return sub_80048658();
    value = r_u32(scene + 8);
    if ((sint32)value > 0)
        return value;
    return sub_80048658();
}

uint32 sub_800383ac(void)
{
    uint32 index, table, object, flags, a, b, c, d, e, f, scene, base, value, kind, health, emit, blocked;
    FUNCTION_MARKER(0x800383AC, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        if (r_u32(object + 84) & 0x800000)
            sub_80030f64(object + 24, object + 48, NULL);
        if (r_u32(object + 84) & 0x400000)
            sub_80030ef8(object + 32, object + 48, object + 56);
        if (r_u32(object + 84) & 0x200000)
            sub_80030ef8(object + 32, object + 24, object + 56);
        flags = r_u32(object + 84);
        if ((sint32)flags < 0)
        {
            a = r_u32(object + 8);
            b = r_u32(object + 56);
            c = r_u32(object + 60);
            d = r_u32(object + 64);
            w_u32(object + 8, a + b);
            e = r_u32(object + 12);
            f = r_u32(object + 16);
            w_u32(object + 12, e + c);
            w_u32(object + 16, f + d);
            flags = r_u32(object + 84);
        }
        w_u32(object + 84, flags & 0xFF000000);
    }
    base = r_u32(0x800BCE54);
    if (r_u32(base + 4) == 4)
        sub_80047a54();
    base = r_u32(0x800BCE60);
    scene = r_s16(base + 908);
    if (scene == 32 || scene == 47 || scene == 53)
    {
        table = r_u32(0x800BCE5C) + 8;
        for (index = 2; index < 32; ++index, table += 4)
        {
            object = r_u32(table);
            if ((~r_u32(object) & 0xC000) != 0)
                continue;
            if (r_s32(object + 12) < (sint32)0xFF4AC000)
                w_u32(object + 12, 0xFF4AC000);
            if (r_s32(object + 12) > (sint32)0xFFF9C000)
                w_u32(object + 12, 0xFFF9C000);
        }
    }
    if (scene == 6)
    {
        for (index = 2; index < 32; ++index)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if ((~r_u32(object) & 0xC000) != 0)
                continue;
            base = r_u32(0x800BCEB0);
            value = r_u32(base + 4);
            sub_80082e80(object + 8, object + 56, 0 - value);
        }
    }
    if (scene == 17 || scene == 31 || scene == 38 || scene == 46)
    {
        base = r_u32(0x800BCEB4);
        if (r_u8(base + 60) >= 3)
        {
            for (index = 2; index < 32; ++index)
            {
                table = r_u32(0x800BCE5C);
                object = r_u32(table + index * 4);
                if ((~r_u32(object) & 0xC000) != 0 || r_s16(object + 72) != 45)
                    continue;
                if ((r_u32(0x800C6FD0 + index * 4) & 0x200000) == 0)
                    continue;
                base = r_u32(0x800BCE54);
                value = r_u32(base + 12);
                sub_80081c30(object + 8, object + 56, value + index, 3);
                break;
            }
        }
    }
    if ((scene - 31) < 3 || scene == 38 || scene == 46 || scene == 47 || scene == 48 || scene == 53)
    {
        for (index = 2; index < 32; ++index)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if ((~r_u32(object) & 0xC000) != 0)
                continue;
            kind = r_s16(object + 72);
            if (kind == 26)
            {
                emit = 0;
                if (scene == 33 || scene == 48 || scene == 53)
                {
                    health = r_s16(object + 78);
                    if ((sint32)health < 201 && (sint32)health > 0)
                        emit = 1;
                }
                if (!emit && scene == 38)
                {
                    base = r_u32(0x800BCE54);
                    if (r_u32(base + 8) == 1)
                        emit = 1;
                }
                if (emit)
                {
                    base = r_u32(0x800BCE54);
                    value = r_u32(base + 12);
                    sub_80081c30(object + 8, object + 56, value + index, 2);
                    value = r_u8(object + 5);
                    w_u8(object + 5, value & 0xF7);
                }
                else
                {
                    base = r_u32(0x800BCEB4);
                    blocked = r_u32(base) & 2;
                    if (!blocked && scene == 38)
                    {
                        base = r_u32(0x800BCE54);
                        if (r_s32(base + 8) > 0)
                            blocked = 1;
                    }
                    if (!blocked && scene == 48)
                    {
                        base = r_u32(0x800BCE54);
                        if (r_s32(base + 8) >= 2)
                            blocked = 1;
                    }
                    if (!blocked && (r_u8(object + 5) & 8))
                    {
                        sub_800829bc(object + 8, object + 56, 2);
                        value = r_u8(object + 5);
                        w_u8(object + 5, value & 0xF7);
                    }
                }
                kind = r_s16(object + 72);
            }
            if (kind == 27 && (scene == 33 || scene == 46 || scene == 53))
            {
                health = r_s16(object + 78);
                if ((sint32)health < 201 && (sint32)health > 0)
                {
                    base = r_u32(0x800BCE54);
                    value = r_u32(base + 12);
                    sub_80081c30(object + 8, object + 56, value + index, 2);
                    value = r_u8(object + 5);
                    w_u8(object + 5, value & 0xF7);
                }
            }
        }
    }
    if (scene == 34 || scene == 54)
    {
        for (index = 2; index < 32; ++index)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if ((~r_u32(object) & 0xC000) != 0 || r_s16(object + 72) != 25 || (r_u8(object + 5) & 8) == 0)
                continue;
            sub_800829bc(object + 8, object + 56, 3);
            value = r_u8(object + 5);
            w_u8(object + 5, value & 0xF7);
        }
    }
    if (scene == 27)
    {
        for (index = 2; index < 32; ++index)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if ((~r_u32(object) & 0xC000) != 0)
                continue;
            flags = r_u32(0x800C6FD0 + index * 4);
            if ((flags & 1) && r_s16(object + 72) == 39 && (flags & 0x200000))
                sub_80082d38(object + 8, object + 56);
        }
    }
    if (scene == 58)
    {
        for (index = 2; index < 32; ++index)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + index * 4);
            if ((~r_u32(object) & 0xC000) == 0)
                sub_800829bc(object + 8, object + 56, 4);
        }
        return 0;
    }
    return 58;
}

uint32 sub_8006abb8(void)
{
    uint32 base, mask, index, record, a, b, c, d, e, f;
    FUNCTION_MARKER(0x8006ABB8, "ACE3.BIN");
    base = r_u32(0x800BD080);
    mask = r_u32(base + 0x1548);
    if (mask == 0)
        return 0;
    record = base + 0xEC;
    for (index = 0; index < 32; ++index, record += 0xA4)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        a = r_u32(record - 8);
        b = r_u32(record + 20);
        c = r_u32(record + 24);
        d = r_u32(record + 28);
        w_u32(record - 8, a + b);
        e = r_u32(record - 4);
        f = r_u32(record);
        w_u32(record - 4, e + c);
        w_u32(record, f + d);
    }
    return 0;
}

uint32 sub_800684d4(void)
{
    uint32 base, mask, table, record, index, bit, value, object, flags, condition, mode;
    FUNCTION_MARKER(0x800684D4, "ACE3.BIN");
    base = r_u32(0x800BD080);
    mask = r_u32(base + 0x33E0);
    if (mask == 0)
        return base;
    table = r_u32(0x800BCE5C);
    record = base + 0x33EC;
    for (index = 0; index < 32; ++index, record += 12)
    {
        bit = 1u << index;
        if ((mask & bit) == 0)
            continue;
        value = r_u16(record - 8) - 1;
        w_u16(record - 8, value);
        if ((sint32)(value << 16) >= 0)
            continue;
        value = r_u32(record - 4);
        object = r_u32(table + (value & 0x3F) * 4);
        flags = r_u32(object);
        mask &= ~bit;
        if ((flags & 0x4200) != 0x4200)
            continue;
        condition = r_u32(record);
        if (flags & (condition & 0x1C0))
            continue;
        mode = r_u16(record - 6);
        value = r_u32(object + 84);
        w_u32(object + 84, value | mode);
        value = r_u32(record);
        w_u32(object + 80, value);
    }
    base = r_u32(0x800BD080);
    w_u32(base + 0x33E0, mask);
    return base;
}

uint32 sub_8006af20(void)
{
    FUNCTION_MARKER(0x8006AF20, "ACE3.BIN");
    sub_8006a11c();
    sub_8006abb8();
    sub_80069fc8();
    return sub_8006ac34();
}

uint32 sub_8006c5b4(void)
{
    FUNCTION_MARKER(0x8006C5B4, "ACE3.BIN");
    sub_8006b110();
    sub_8006b504();
    sub_8006af20();
    return sub_800684d4();
}

uint32 sub_8006ac34(void)
{
    uint32 base, mask, index, record, link, bit, flags, a, b, value, owner, object, state;
    FUNCTION_MARKER(0x8006AC34, "ACE3.BIN");
    base = r_u32(0x800BD080);
    mask = r_u32(base + 0x1548);
    if (mask == 0)
        return 0;
    record = base + 0xC8;
    link = base + 0x134;
    for (index = 0; index < 32; ++index, record += 0xA4, link += 0xA4)
    {
        bit = 1u << index;
        if ((mask & bit) == 0)
            continue;
        flags = r_u32(record);
        if (flags & 0x2000)
        {
            a = r_u16(link - 60);
            b = r_u16(link - 16);
            flags &= 0xFFFFDFFF;
            w_u16(link - 60, a + b);
        }
        if (flags & 0x1000)
        {
            sub_80030f64(record + 44, record + 88, NULL);
            flags &= 0xFFFFEFFF;
        }
        if (flags & 0x4000)
        {
            sub_80030ef8(record + 72, record + 44, record + 56);
            flags &= 0xFFFFBFFF;
        }
        if (flags & 0x40)
            sub_800829bc(record + 28, record + 56, 0);
        if (flags & 0x80)
            sub_800829bc(record + 28, record + 56, 1);
        w_u32(record, flags);
        value = r_u16(link - 84) - 1;
        w_u16(link - 84, value);
        if ((sint32)(value << 16) >= 0)
        {
            if (r_u32(link) & 0x2000)
                sub_8007e930(index, r_s16(link - 56), record + 28);
            continue;
        }
        if (r_u32(record) & 0x10)
            sub_8006a978(record, r_u32(link + 48));
        if (r_u32(record) & 0x20)
            sub_8006aafc(record);
        if (r_u32(link) & 0x2000)
            sub_8007e968(index);
        value = r_u32(link);
        base = r_u32(0x800BD080);
        state = base + (value & 0x3F);
        value = r_u8(state + 72);
        w_u8(state + 72, value - 1);
        owner = r_u32(link + 4);
        mask &= ~bit;
        if (owner == 0xFFFFFFFF)
            continue;
        base = r_u32(0x800BD080);
        state = base + (owner & 0x3F);
        value = r_u8(state + 136) - 1;
        w_u8(state + 136, value);
        if (value & 0xFF)
            continue;
        base = r_u32(0x800BD080);
        state = base + ((owner & 0x20) ? 4 : 0);
        value = r_u32(state);
        w_u32(state, value & ~(1u << (owner & 31)));
        if (r_u32(record) & 2)
            continue;
        base = r_u32(0x800BCE5C);
        object = r_u32(base + (owner & 0x3F) * 4);
        flags = r_u32(object + 84);
        if ((flags & 0x7FC0) == 0)
        {
            w_u32(object + 84, flags | 0x8000);
            value = r_u32(link);
            w_u32(object + 80, value);
        }
    }
    base = r_u32(0x800BD080);
    w_u32(base + 0x1548, mask);
    return base;
}

uint32 sub_8007e930(uint32 index, uint32 value, uint32 position)
{
    FUNCTION_MARKER(0x8007E930, "ACE3.BIN");
    return sub_8007e2b0(2, index, value, position, 0, 0);
}

uint32 sub_8007e968(uint32 index)
{
    FUNCTION_MARKER(0x8007E968, "ACE3.BIN");
    return sub_8007e2b0(3, index, 0, 0, 0, 0);
}

uint32 sub_8006aafc(uint32 state)
{
    uint32 value, table, object, flags, mode, numerator, divisor, parameter, delay;
    FUNCTION_MARKER(0x8006AAFC, "ACE3.BIN");
    value = r_u32(state) & 1;
    if (value)
        return value;
    value = r_u32(state + 112);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + (value & 0x3F) * 4);
    flags = r_u32(object);
    if ((flags & 0x4200) != 0x4200)
        return 0x4200;
    mode = r_u32(state + 108);
    value = flags & (mode & 0x1C0);
    if (value)
        return value;
    numerator = r_u32(state + 156);
    divisor = r_u32(state + 160);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    delay = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    parameter = r_u16(state + 26);
    return sub_80068454(mode, flags, parameter, delay);
}

uint32 sub_80068454(uint32 mode, uint32 flags, uint32 parameter, uint32 delay)
{
    uint32 base, mask, index, target;
    FUNCTION_MARKER(0x80068454, "ACE3.BIN");
    base = r_u32(0x800BD080);
    mask = r_u32(base + 0x33E0);
    if (mask == 0xFFFFFFFF)
        return 0;
    index = 0;
    while (mask & (1u << (index & 31)))
        ++index;
    mask |= 1u << (index & 31);
    base = r_u32(0x800BD080);
    target = base + 0x33E4 + index * 12;
    w_u32(base + 0x33E0, mask);
    w_u16(target, delay);
    w_u16(target + 2, parameter);
    w_u32(target + 4, flags);
    w_u32(target + 8, mode);
    return target;
}

uint32 sub_8007e2b0(uint32 action, uint32 index, uint32 value, uint32 position, uint32 extra, const NATIVE_POSE *native_value)
{
    uint32 target, flags, x, y, z, ax, ay, az, packed_x, packed_y, packed_z;
    FUNCTION_MARKER(0x8007E2B0, "ACE3.BIN");
    if (action == 1 || action == 3)
    {
        target = sub_8007df28(1);
        flags = r_u32(target);
        flags = (flags & 0xFFFF0000) | (action == 1 ? 0xA1 : 0x1A1) | ((index & 0x7F) << 9);
        w_u32(target, flags);
        return target;
    }
    if (action == 0)
    {
        target = sub_8007df28(6);
        w_u32(target + 20, extra);
        flags = r_u32(target);
        w_u32(target, (flags & 0xFFFF0000) | 0x26 | ((index & 0x7F) << 9));
        x = r_u32(position);
        z = r_u32(position + 8);
        y = r_u32(position + 4);
        w_u16(target + 2, (uint32)((sint32)x >> 6));
        w_u16(target + 6, (uint32)((sint32)z >> 6));
        w_u16(target + 4, (uint32)((sint32)y >> 6));
        x = native_value ? native_value->position[0] : r_u32(value);
        y = native_value ? native_value->position[1] : r_u32(value + 4);
        ax = native_value ? native_value->angles[0] : r_u16(value + 16);
        z = native_value ? native_value->position[2] : r_u32(value + 8);
        ay = native_value ? native_value->angles[1] : r_u16(value + 18);
        az = native_value ? native_value->angles[2] : r_u16(value + 20);
        packed_x = ((ax >> 2) & 0xFFF) | (x & 0xFFFFF000);
        packed_y = ((ay >> 2) & 0xFFF) | (y & 0xFFFFF000);
        w_u32(target + 8, packed_x);
        w_u32(target + 12, packed_y);
        packed_z = ((az >> 2) & 0xFFF) | (z & 0xFFFFF000);
        w_u32(target + 16, packed_z);
        return target;
    }
    if (action == 2)
    {
        target = sub_8007df28(4);
        flags = r_u32(target);
        w_u32(target, (flags & 0xFFFF0000) | 0x124 | ((index & 0x7F) << 9));
        w_u16(target + 2, value);
        ax = r_u16(position + 16);
        x = r_u32(position);
        ay = r_u16(position + 18);
        az = r_u16(position + 20);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
        packed_x = ((ax >> 2) & 0xFFF) | (x & 0xFFFFF000);
        packed_y = ((ay >> 2) & 0xFFF) | (y & 0xFFFFF000);
        w_u32(target + 4, packed_x);
        w_u32(target + 8, packed_y);
        packed_z = ((az >> 2) & 0xFFF) | (z & 0xFFFFF000);
        w_u32(target + 12, packed_z);
        return target;
    }
    return (sint32)action < 2 ? 1 : 3;
}

uint32 sub_8007df28(uint32 words)
{
    uint32 base, start, end, next, read, release;
    FUNCTION_MARKER(0x8007DF28, "ACE3.BIN");
    base = r_u32(0x800BD0B8);
    start = r_u32(base + 12);
    end = r_u32(base + 8);
    next = start + (words << 2);
    read = r_u32(base + 16);
    release = 0;
    if ((sint32)next >= (sint32)end)
    {
        next = r_u32(base + 4);
        if (!((sint32)read < (sint32)start && (sint32)next < (sint32)read))
            release = 1;
    }
    else if ((sint32)read >= (sint32)start && (sint32)next >= (sint32)read && r_u32(base) != 0)
        release = 1;
    if (release)
        sub_8007de64(words);
    base = r_u32(0x800BD0B8);
    w_u32(base + 12, next);
    return start;
}

uint32 sub_8007de64(uint32 words)
{
    uint32 base, record, end, consumed, value;
    FUNCTION_MARKER(0x8007DE64, "ACE3.BIN");
    base = r_u32(0x800BD0B8);
    record = r_u32(base + 16);
    end = r_u32(base + 8);
    consumed = 0;
    while ((sint32)consumed < (sint32)words)
    {
        if (r_u32(record) == 1)
        {
            base = r_u32(0x800BD0B8);
            value = r_u32(base);
            w_u32(base, value - 1);
        }
        value = sub_8007ddf4(record);
        record += value << 2;
        consumed += value;
        if ((sint32)record >= (sint32)end)
        {
            base = r_u32(0x800BD0B8);
            record = r_u32(base + 4);
        }
    }
    base = r_u32(0x800BD0B8);
    w_u32(base + 16, record);
    return base;
}

uint32 sub_8007ddf4(uint32 record)
{
    uint32 base, kind;
    FUNCTION_MARKER(0x8007DDF4, "ACE3.BIN");
    base = r_u32(0x800BD0B8);
    kind = (r_u32(record) >> 4) & 7;
    if (kind == 0)
        sub_8007dc3c(record, base + 24);
    else if (kind == 1)
        sub_8007dd58(record, base + 24);
    return r_u32(record) & 15;
}

uint32 sub_8007bf30(void)
{
    uint32 base, write, read, mask, index, cursor, end, record, age, life, value;
    FUNCTION_MARKER(0x8007BF30, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    write = base + 4;
    read = base + 8;
    mask = r_u32(base);
    for (index = 0; index < 32; ++index, write += 0x908, read += 0x908)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        cursor = r_u32(read);
        end = r_u32(write);
        while ((sint32)cursor < (sint32)end)
        {
            record = write + (cursor & 31) * 4;
            age = r_u32(record + 8);
            life = r_u32(record + 136);
            w_u32(record + 8, age + 1);
            w_u32(record + 136, life - 1);
            end = r_u32(write);
            ++cursor;
        }
        value = r_u32(read);
        record = write + (value & 31) * 4;
        life = r_u32(record + 136);
        if ((sint32)life < 0)
            w_u32(read, value + 1);
        value = r_u32(read);
        end = r_u32(write);
        if ((sint32)value >= (sint32)end)
            mask &= ~(1u << index);
    }
    base = r_u32(0x800BD0B0);
    w_u32(base, mask);
    return base;
}

uint32 sub_8007ceb0(void)
{
    uint32 base, write, read, mask, index, cursor, end, record, age, life, value;
    FUNCTION_MARKER(0x8007CEB0, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    write = base + 0x12128;
    read = write + 4;
    mask = r_u32(base + 0x12124);
    for (index = 0; index < 32; ++index, write += 0x508, read += 0x508)
    {
        if ((mask & (1u << index)) == 0)
            continue;
        cursor = r_u32(read);
        end = r_u32(write);
        while ((sint32)cursor < (sint32)end)
        {
            record = write + (cursor & 31) * 4;
            age = r_u32(record + 8);
            life = r_u32(record + 136);
            w_u32(record + 8, age + 1);
            w_u32(record + 136, life - 1);
            end = r_u32(write);
            ++cursor;
        }
        value = r_u32(read);
        record = write + (value & 31) * 4;
        life = r_u32(record + 136);
        if ((sint32)life < 0)
            w_u32(read, value + 1);
        value = r_u32(read);
        end = r_u32(write);
        if ((sint32)value >= (sint32)end)
            mask &= ~(1u << index);
    }
    base = r_u32(0x800BD0B0) + 0x10000;
    w_u32(base + 0x2124, mask);
    return base;
}

uint32 sub_8007dd58(uint32 record, uint32 context)
{
    uint32 header, mode, target, value, result;
    FUNCTION_MARKER(0x8007DD58, "ACE3.BIN");
    header = r_u32(record);
    mode = (header >> 7) & 3;
    if (mode)
        return mode;
    target = context + ((header >> 4) & 0x7E0);
    value = r_u32(record + 4);
    w_u32(target + 8, value & 0xFFFFF000);
    value = r_u32(record + 8);
    w_u32(target + 12, value & 0xFFFFF000);
    value = r_u32(record + 12);
    w_u32(target + 16, value & 0xFFFFF000);
    value = r_u32(record + 4);
    w_u16(target + 24, (uint32)((sint32)(value << 20) >> 18));
    value = r_u32(record + 8);
    w_u16(target + 26, (uint32)((sint32)(value << 20) >> 18));
    value = r_u32(record + 12);
    result = (uint32)((sint32)(value << 20) >> 18);
    w_u16(target + 28, result);
    return result;
}

uint32 sub_8007dc3c(uint32 record, uint32 context)
{
    uint32 header, mode, base, target, value, offset;
    FUNCTION_MARKER(0x8007DC3C, "ACE3.BIN");
    mode = (r_u32(record) >> 7) & 7;
    switch (mode)
    {
        case 0:
            base = r_u32(0x800BD0B8);
            w_u32(base + 20, 1);
            return 1;
        case 1:
            header = r_u32(record);
            target = context + ((header >> 5) & 0x7E0);
            value = r_u32(target);
            if (header & 0x10000)
                value |= 0x14000;
            else
                value &= 0xFFFEBFFF;
            w_u32(target, value);
            return value;
        case 2:
            header = r_u32(record);
            value = r_u8(record + 2);
            target = context + ((header >> 5) & 0x7E0);
            w_u8(target + 4, value);
            value = r_u8(record + 3);
            w_u8(target + 5, value);
            return value;
        case 3:
            header = r_u32(record);
            offset = (header >> 5) & 0x7E0;
            w_u16(context + offset + 6, (header >> 16) & 0x7FFF);
            return offset;
        case 4:
            header = r_u32(record);
            value = (header >> 10) & 0x3F;
            if (value == 0)
                value = 0xFFFFFFFF;
            base = r_u32(0x800BD0B8);
            w_u32(base + 0x1840, value);
            w_u32(base + 0x1830, 0);
            w_u32(base + 0x1834, 0);
            w_u32(base + 0x1838, 0);
            return base;
        default:
            return 0x80020000;
    }
}

uint32 sub_8007d7e0(void)
{
    uint32 index, table, object, kind, vertex, a, b, c, x, y, z, vx, vy, vz, scene, slot, value;
    FUNCTION_MARKER(0x8007D7E0, "ACE3.BIN");
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        if ((r_u32(object) & 0x14000) != 0x14000 || (r_u8(object + 5) & 8) == 0)
        {
            table = r_u32(0x800BD0B0);
            w_u8(table + index + 0x1C228, 255);
            continue;
        }
        kind = r_s16(object + 72);
        if ((sint32)(kind << 1) >= 62 || (sint32)kind < 0)
            continue;
        vertex = 0x800B4144 + (kind << 4);
        rotation_build(object + 24, 0x1F800000, 0, 0);
        a = r_u32(0x1F800000);
        b = r_u32(0x1F800004);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(0x1F800008);
        b = r_u32(0x1F80000C);
        c = r_u32(0x1F800010);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        xport_gte_write_data(0, r_u32(vertex));
        xport_gte_write_data(1, r_u32(vertex + 4));
        xport_gte_execute(0x486012);
        w_u32(0x1F800040, xport_gte_read_data(25));
        w_u32(0x1F800044, xport_gte_read_data(26));
        w_u32(0x1F800048, xport_gte_read_data(27));
        a = r_u32(0x1F800040);
        c = r_u32(0x1F800048);
        b = r_u32(0x1F800044);
        vx = a << 3;
        vy = b << 3;
        vz = c << 3;
        w_u32(0x1F800020, vx + x);
        w_u32(0x1F800024, vy + y);
        w_u32(0x1F800028, vz + z);
        xport_gte_write_data(0, r_u32(vertex + 8));
        xport_gte_write_data(1, r_u32(vertex + 12));
        xport_gte_execute(0x486012);
        w_u32(0x1F800040, xport_gte_read_data(25));
        w_u32(0x1F800044, xport_gte_read_data(26));
        w_u32(0x1F800048, xport_gte_read_data(27));
        a = r_u32(0x1F800040);
        c = r_u32(0x1F800048);
        b = r_u32(0x1F800044);
        x += a << 3;
        z += c << 3;
        w_u32(0x1F800030, x);
        w_u32(0x1F800038, z);
        scene = r_u32(0x800BCE54);
        y += b << 3;
        w_u32(0x1F800034, y);
        value = r_u32(scene + 12);
        table = r_u32(0x800BD0B0);
        slot = r_s8(table + index + 0x1C228);
        if ((value & 1) == (index & 1))
        {
            value = sub_8007cc6c(slot, 0x1F800020);
            table = r_u32(0x800BD0B0);
            w_u8(table + index + 0x1C228, value);
        }
        else if ((sint32)slot >= 0)
            sub_8007cdf8(slot, 0x1F800020);
    }
    return 0;
}

uint32 sub_8007dac8(void)
{
    FUNCTION_MARKER(0x8007DAC8, "ACE3.BIN");
    sub_8007c9e8();
    sub_8007bf30();
    sub_8007d7e0();
    return sub_8007ceb0();
}

uint32 sub_8007cdf8(uint32 slot, uint32 vertices)
{
    uint32 base, mask, queue, cursor, target, value;
    FUNCTION_MARKER(0x8007CDF8, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    mask = r_u32(base + 0x12124);
    if ((sint32)slot < 0 || ((~mask) & (1u << (slot & 31))))
        return 0xFFFFFFFF;
    queue = base + 0x12128 + slot * 0x508;
    cursor = r_u32(queue);
    value = r_u32(vertices);
    target = queue + (((cursor - 1) & 31) << 5) + 0x108;
    w_u32(target, value);
    value = r_u32(vertices + 4);
    w_u32(target + 4, value);
    value = r_u32(vertices + 8);
    w_u32(target + 8, value);
    value = r_u32(vertices + 16);
    w_u32(target + 16, value);
    value = r_u32(vertices + 20);
    w_u32(target + 20, value);
    value = r_u32(vertices + 24);
    w_u32(target + 24, value);
    return slot;
}

uint32 sub_8007cc6c(uint32 slot, uint32 vertices)
{
    uint32 base, mask, queue, write, read, value, target;
    FUNCTION_MARKER(0x8007CC6C, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    mask = r_u32(base + 0x12124);
    if (slot == 0xFFFFFFFF)
    {
        if (mask == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        slot = 0;
        while (mask & (1u << (slot & 31)))
            ++slot;
        mask |= 1u << (slot & 31);
        base = r_u32(0x800BD0B0);
        queue = base + 0x12128 + slot * 0x508;
        w_u32(queue + 4, 0);
        w_u32(queue, 0);
        w_u32(base + 0x12124, mask);
    }
    else
    {
        if ((~mask) & (1u << (slot & 31)))
            return 0xFFFFFFFF;
        queue = base + 0x12128 + slot * 0x508;
    }
    write = r_u32(queue);
    read = r_u32(queue + 4);
    if ((sint32)(write - read) >= 32)
        return slot;
    w_u32(queue + (write & 31) * 4 + 8, 0);
    write = r_u32(queue);
    w_u32(queue + (write & 31) * 4 + 136, 63);
    write = r_u32(queue);
    target = queue + ((write & 31) << 5) + 0x108;
    w_u32(queue, write + 1);
    value = r_u32(vertices);
    w_u32(target, value);
    value = r_u32(vertices + 4);
    w_u32(target + 4, value);
    value = r_u32(vertices + 8);
    w_u32(target + 8, value);
    value = r_u32(vertices + 16);
    w_u32(target + 16, value);
    value = r_u32(vertices + 20);
    w_u32(target + 20, value);
    value = r_u32(vertices + 24);
    w_u32(target + 24, value);
    return slot;
}

uint32 sub_8007c9e8(void)
{
    uint32 base, mask, index, record, position, a, b, c, x, y, z, point, out, scene, tick, slot, value;
    FUNCTION_MARKER(0x8007C9E8, "ACE3.BIN");
    base = r_u32(0x800BD080);
    record = base + 0xC8;
    position = base + 0xEC;
    mask = r_u32(base + 0x1548);
    for (index = 0; index < 32; ++index, record += 0xA4, position += 0xA4)
    {
        if ((mask & (1u << index)) == 0 || (r_u8(position + 17) & 8) == 0)
        {
            base = r_u32(0x800BD0B0);
            w_u8(base + index + 0x12104, 255);
            continue;
        }
        rotation_build(record + 44, 0x1F800000, 0, 0);
        a = r_u32(0x1F800000);
        b = r_u32(0x1F800004);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(0x1F800008);
        b = r_u32(0x1F80000C);
        c = r_u32(0x1F800010);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        x = r_u32(position - 8);
        y = r_u32(position - 4);
        z = r_u32(position);
        for (point = 0; point < 4; ++point)
        {
            xport_gte_write_data(0, r_u32(0x800B4124 + point * 8));
            xport_gte_execute(0x486012);
            a = xport_gte_read_data(25);
            b = xport_gte_read_data(26);
            c = xport_gte_read_data(27);
            a = (a << 3) + x;
            b = (b << 3) + y;
            c = (c << 3) + z;
            if (point == 3)
                scene = r_u32(0x800BCE54);
            out = 0x1F800020 + point * 16;
            w_u32(out, a);
            w_u32(out + 4, b);
            w_u32(out + 8, c);
        }
        tick = r_u32(scene + 12);
        if ((tick & 3) != (index & 3))
        {
            base = r_u32(0x800BD0B0);
            slot = r_s8(base + index + 0x12104);
            if ((sint32)slot >= 0)
            {
                sub_8007be3c(slot, 0x1F800020);
                continue;
            }
        }
        base = r_u32(0x800BD0B0);
        slot = r_s8(base + index + 0x12104);
        value = sub_8007bc88(slot, 0x1F800020);
        base = r_u32(0x800BD0B0);
        w_u8(base + index + 0x12104, value);
    }
    return 0;
}

uint32 sub_8007be3c(uint32 slot, uint32 vertices)
{
    uint32 base, mask, queue, cursor, target, value, point, axis, offset;
    FUNCTION_MARKER(0x8007BE3C, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    mask = r_u32(base);
    if ((sint32)slot < 0 || ((~mask) & (1u << (slot & 31))))
        return 0xFFFFFFFF;
    queue = base + slot * 0x908 + 4;
    cursor = r_u32(queue);
    value = r_u32(vertices);
    target = queue + (((cursor - 1) & 31) << 6) + 0x108;
    w_u32(target, value);
    for (point = 0; point < 4; ++point)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            offset = point * 16 + axis * 4;
            if (offset == 0)
                continue;
            value = r_u32(vertices + offset);
            w_u32(target + offset, value);
        }
    }
    return slot;
}

uint32 sub_8007bc88(uint32 slot, uint32 vertices)
{
    uint32 base, mask, queue, write, read, value, target, point, axis, offset;
    FUNCTION_MARKER(0x8007BC88, "ACE3.BIN");
    base = r_u32(0x800BD0B0);
    mask = r_u32(base);
    if (slot == 0xFFFFFFFF)
    {
        if (mask == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        slot = 0;
        while (mask & (1u << (slot & 31)))
            ++slot;
        mask |= 1u << (slot & 31);
        base = r_u32(0x800BD0B0);
        queue = base + slot * 0x908 + 4;
        w_u32(queue + 4, 0);
        w_u32(queue, 0);
        w_u32(base, mask);
    }
    else
    {
        if ((~mask) & (1u << (slot & 31)))
            return 0xFFFFFFFF;
        queue = base + slot * 0x908 + 4;
    }
    write = r_u32(queue);
    read = r_u32(queue + 4);
    if ((sint32)(write - read) >= 32)
        return slot;
    w_u32(queue + (write & 31) * 4 + 8, 0);
    write = r_u32(queue);
    w_u32(queue + (write & 31) * 4 + 136, 90);
    write = r_u32(queue);
    target = queue + ((write & 31) << 6) + 0x108;
    w_u32(queue, write + 1);
    for (point = 0; point < 4; ++point)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            offset = point * 16 + axis * 4;
            value = r_u32(vertices + offset);
            w_u32(target + offset, value);
        }
    }
    return slot;
}

uint32 sub_800850d8(void)
{
    uint32 scene, table, player, value, mission, variant, base;
    FUNCTION_MARKER(0x800850D8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    if (r_u32(scene) & 2)
    {
        table = r_u32(0x800BCE5C);
        player = r_u32(table);
        base = r_u32(0x800BCEB0);
        value = r_u32(base + 4);
        sub_80082e80(player + 8, player + 56, 0 - value);
        table = r_u32(0x800BCE5C);
        player = r_u32(table);
        if (r_s16(player + 78) > 0)
        {
            scene = r_u32(0x800BCE60);
            mission = r_s16(scene + 908);
            variant = r_s16(scene + 910);
            sub_80084f6c(mission, variant);
        }
    }
    scene = r_u32(0x800BCE60);
    if (r_s16(scene + 908) == 42)
    {
        table = r_u32(0x800BCE5C);
        player = r_u32(table);
        if (r_s16(player + 78) > 0)
            sub_80084e6c(player + 8, player + 56);
    }
    base = r_u32(0x800BCF90);
    sub_80089d70(base + 0x36C4);
    base = r_u32(0x800BCF90);
    sub_80084d04(base + 0x600, 3);
    base = r_u32(0x800BCF90);
    sub_80084d04(base + 0x1098, 10);
    base = r_u32(0x800BCF90);
    sub_80089ed8(base + 0x33C0);
    base = r_u32(0x800BCF90);
    sub_80090148(base + 0x3948);
    base = r_u32(0x800BCF90);
    sub_80091654(base + 0x3964);
    base = r_u32(0x800BCF90);
    return sub_800928f4(base + 0x3B28);
}

uint32 sub_80084f6c(uint32 mission, uint32 variant)
{
    uint32 minimum, speed_minimum, kind, base, table, object, vector, x, y, z, value, px, py, pz;
    FUNCTION_MARKER(0x80084F6C, "ACE3.BIN");
    if (mission == 58)
    {
        minimum = 0x780;
        speed_minimum = 30;
        kind = 5;
    }
    else
    {
        if ((variant - 25) >= 2)
            return 0;
        minimum = 0xC00;
        speed_minimum = 60;
        kind = variant - 19;
    }
    base = r_u32(0x800BD090);
    if (r_s16(base + 6) < (sint32)minimum)
        return 1;
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    vector = object + 56;
    x = r_u32(object + 56);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    value = sub_80083f40((uint32)((sint32)x >> 12), (uint32)((sint32)y >> 12), (uint32)((sint32)z >> 12));
    if ((sint32)value < (sint32)speed_minimum)
        return 1;
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    px = r_u32(object + 8);
    w_u32(0x1F800000, px);
    object = r_u32(table);
    py = r_u32(object + 12);
    w_u32(0x1F800004, py);
    object = r_u32(table);
    pz = r_u32(object + 16);
    w_u32(0x1F800008, pz);
    if ((variant - 25) >= 3)
    {
        w_u32(0x1F800000, px - (x - (uint32)((sint32)x >> 2)));
        w_u32(0x1F800004, py - (y - (uint32)((sint32)y >> 2)));
        w_u32(0x1F800008, pz - (z - (uint32)((sint32)z >> 2)));
    }
    return sub_800829bc(0x1F800000, vector, kind);
}
