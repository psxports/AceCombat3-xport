#include "game_functions.h"
#include <stdlib.h>

void resource002_sub_800e03cc(void)
{
    FUNCTION_MARKER(0x800E03CC, "ACE3.BIN");
}

uint32 resource002_sub_800e03d4(uint32 index)
{
    uint32 state, table, cursor, offset, result, value, velocity, ticks;
    sint32 phase;

    FUNCTION_MARKER(0x800E03D4, "ACE3.BIN");
    result = index * 3;
    state = r_u32(0x80112DC4);
    table = r_u32(state + 364);
    offset = index * 12;
    cursor = table + offset;
    phase = r_s16(cursor + 10);
    if (phase == 1)
        return result;
    result = phase < 2;
    if (phase == 0)
    {
        ticks = r_u32(state + 420);
        if ((sint32)ticks <= 0)
            return result;
        if ((sint32)ticks < 27)
        {
            value = r_u16(cursor + 4);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 4, value + velocity);
            cursor = r_u32(state + 364) + offset;
            result = (uint32)r_s16(cursor + 8);
            velocity = r_u16(cursor + 8);
            if ((sint32)result > 0)
            {
                result = velocity - 1;
                w_u16(cursor + 8, result);
            }
        }
        else if ((sint32)ticks < 29)
        {
            result = r_u16(cursor + 4) + 3;
            w_u16(cursor + 4, result);
        }
        else if ((sint32)ticks < 31)
        {
            if (ticks == 29)
                sub_8002db4c(92);
            state = r_u32(0x80112DC4);
            cursor = r_u32(state + 364) + offset;
            result = r_u16(cursor + 4) - 1;
            w_u16(cursor + 4, result);
        }
        else
        {
            w_u16(cursor + 8, 0);
            result = r_u32(state + 364) + offset;
            w_u16(result + 10, 1);
        }
    }
    else if (phase >= 2)
    {
        result = 2;
        if (phase == 2)
        {
            value = r_u16(cursor + 4);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 4, value - velocity);
            cursor = r_u32(state + 364) + offset;
            value = (uint32)r_s16(cursor + 4);
            result = 3;
            if ((sint32)value < -319)
                w_u16(cursor + 10, 3);
            else
            {
                result = r_u16(cursor + 8) + 2;
                w_u16(cursor + 8, result);
            }
        }
    }
    return result;
}

uint32 resource002_sub_800e056c(uint32 index)
{
    uint32 state, cursor, offset, ticks, result, value, velocity;
    sint32 phase;

    FUNCTION_MARKER(0x800E056C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    cursor = r_u32(state + 364);
    offset = index * 12;
    cursor += offset;
    phase = r_s16(cursor + 10);
    if (phase == 1)
    {
        ticks = r_u32(state + 420);
        result = (sint32)ticks < 400;
        if (result)
        {
            result = ticks + 1;
            w_u32(state + 420, result);
        }
        return result;
    }
    if (phase < 0)
        return 43;
    if (phase == 0)
    {
        ticks = r_u32(state + 420);
        if (ticks == 43)
            sub_8002db4c(92);
        state = r_u32(0x80112DC4);
        ticks = r_u32(state + 420);
        if ((sint32)ticks < 31)
        {
            cursor = r_u32(state + 364) + offset;
            value = r_u16(cursor + 6);
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 6, value + velocity);
            cursor = r_u32(state + 364) + offset;
            value = (uint32)r_s16(cursor + 8);
            velocity = r_u16(cursor + 8);
            if ((sint32)value > 0)
                w_u16(cursor + 8, velocity - 1);
        }
        else if ((sint32)ticks < 33)
        {
            cursor = r_u32(state + 364) + offset;
            value = r_u16(cursor + 6);
            w_u16(cursor + 6, value + 8);
        }
        else if ((sint32)ticks < 35)
        {
            cursor = r_u32(state + 364) + offset;
            value = r_u16(cursor + 6);
            w_u16(cursor + 6, value - 4);
        }
        else
        {
            cursor = r_u32(state + 364) + offset;
            w_u16(cursor + 10, 1);
            cursor = r_u32(state + 364);
            w_u16(cursor + 92, (uint32)-44);
        }
        state = r_u32(0x80112DC4);
        ticks = r_u32(state + 420);
        if (ticks == 33)
            sub_8002db4c(92);
        state = r_u32(0x80112DC4);
        result = r_u32(state + 420) + 1;
        w_u32(state + 420, result);
        return result;
    }
    result = 2;
    if (phase == 2)
    {
        value = r_u16(cursor + 6);
        velocity = r_u16(cursor + 8);
        w_u16(cursor + 6, value + velocity);
        cursor = r_u32(state + 364) + offset;
        if (r_s16(cursor + 6) < -479)
            w_u16(cursor + 10, 3);
        else
        {
            velocity = r_u16(cursor + 8);
            w_u16(cursor + 8, velocity - 2);
        }
        state = r_u32(0x80112DC4);
        result = r_u32(state + 420);
        if ((sint32)result > 0)
        {
            --result;
            w_u32(state + 420, result);
        }
    }
    return result;
}

uint32 resource002_sub_800e0798(uint32 index)
{
    uint32 state, table, cursor, offset, result, value, velocity;
    sint32 phase;

    FUNCTION_MARKER(0x800E0798, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + 364);
    offset = index * 12;
    cursor = table + offset;
    phase = r_s16(cursor + 10);
    result = phase < 2;
    if (phase == 1)
    {
        result = (uint32)r_s16(cursor + 6);
        if ((sint32)result < 0)
            w_u16(cursor + 6, 0);
    }
    else if (phase == 2)
    {
        value = r_u16(cursor + 6);
        velocity = r_u16(cursor + 8);
        w_u16(cursor + 6, value + velocity);
        cursor = r_u32(state + 364) + offset;
        value = (uint32)r_s16(cursor + 6);
        result = 3;
        if ((sint32)value >= 480)
            w_u16(cursor + 10, 3);
        else
        {
            result = r_u16(cursor + 8) + 4;
            w_u16(cursor + 8, result);
        }
    }
    else if (phase > 2)
        result = 2;
    else if (phase == 0 && r_s16(table + 70) == 1)
    {
        value = r_u16(cursor + 6);
        velocity = r_u16(cursor + 8);
        w_u16(cursor + 6, value + velocity);
        cursor = r_u32(state + 364) + offset;
        value = (uint32)r_s16(cursor + 6);
        if ((sint32)value < 0)
            w_u16(cursor + 6, 0);
        state = r_u32(0x80112DC4);
        cursor = r_u32(state + 364) + offset;
        value = (uint32)r_s16(cursor + 8);
        velocity = r_u16(cursor + 8);
        result = velocity + 2;
        if ((sint32)value >= 0)
            w_u16(cursor + 10, 1);
        else
            w_u16(cursor + 8, result);
    }
    return result;
}

static sint32 resource002_clip_divide(uint32 numerator, sint32 denominator)
{
    if (denominator == 0 || (numerator == 0x80000000 && denominator == -1))
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 clipping division BREAK %u\n", denominator == 0 ? 7 : 6);
#endif
        abort();
    }
    return (sint32)numerator / denominator;
}

uint32 resource002_sub_800defec(void)
{
    FUNCTION_MARKER(0x800DEFEC, "ACE3.BIN");
    return 1;
}

uint32 resource002_sub_800deffc(uint32 owner)
{
    uint32 control, table, index, first, middle, last, scratch, product, value;
    sint32 current, phase, begin, split, finish, a, b, quotient, denominator;

    FUNCTION_MARKER(0x800DEFFC, "ACE3.BIN");
    control = r_u32(owner + 60);
    table = r_u32(owner + 80);
    index = (uint32)r_s16(control + 12);
    middle = (uint32)r_s16(control + 6);
    first = r_u16(control + 4);
    current = r_s16(table + index * 2);
    control = r_u32(owner + 60);
    last = r_u16(control + 8);
    control = r_u32(owner + 60);
    index = (uint32)r_s16(control + 12);
    table = r_u32(owner + 88);
    phase = r_s16(table + index * 2);
    scratch = r_u32(0x80112DB4);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    finish = (sint16)last;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = r_s16(scratch);
        b = r_s16(control);
        product = (uint32)(a - b) * (uint32)(current - begin);
        quotient = resource002_clip_divide(product, split - begin);
        value = r_u16(control);
        w_u16(scratch, value + (uint32)quotient);
        control = r_u32(owner + 60);
        a = r_s16(scratch + 2);
        b = r_s16(control + 2);
        denominator = a - b;
        if (denominator < 0)
            denominator = -denominator;
        quotient = resource002_clip_divide((uint32)(a - b), denominator);
        value = r_u16(control + 2);
        w_u16(scratch + 2, value + (uint32)quotient);
        a = r_s16(scratch + 4);
        product = (uint32)a * (uint32)(current - begin);
        quotient = resource002_clip_divide(product, split - begin);
        w_u16(scratch + 6, 2);
        w_u16(scratch + 4, (uint32)quotient);
    }
    else if (current < finish)
    {
        a = r_s16(scratch + 2);
        b = r_s16(control + 2);
        product = (uint32)(a - b) * (uint32)(current - split);
        quotient = resource002_clip_divide(product, finish - split);
        value = r_u16(control + 2);
        w_u16(scratch + 2, value + (uint32)quotient);
        a = r_s16(scratch + 6);
        product = (uint32)a * (uint32)(current - split);
        quotient = resource002_clip_divide(product, finish - split);
        w_u16(scratch + 6, (uint32)quotient);
    }
    return 1;
}

uint32 resource002_sub_800deff4(void)
{
    FUNCTION_MARKER(0x800DEFF4, "ACE3.BIN");
    return resource002_sub_800deffc(r_u32(0x80112DC0));
}

uint32 resource002_sub_800df2b0(void)
{
    uint32 owner, control, index, table;

    FUNCTION_MARKER(0x800DF2B0, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    index = (uint32)r_s16(control + 12);
    table = r_u32(owner + 88);
    return r_s16(table + index * 2) == 2;
}

uint32 resource002_sub_800df2f0(uint32 owner)
{
    uint32 control, table, index, first, middle, scratch, product, value;
    sint32 current, phase, begin, split, a, b, quotient;

    FUNCTION_MARKER(0x800DF2F0, "ACE3.BIN");
    control = r_u32(owner + 60);
    table = r_u32(owner + 80);
    index = (uint32)r_s16(control + 12);
    middle = (uint32)r_s16(control + 6);
    first = r_u16(control + 4);
    current = r_s16(table + index * 2);
    control = r_u32(owner + 60);
    r_u16(control + 8);
    control = r_u32(owner + 60);
    index = (uint32)r_s16(control + 12);
    table = r_u32(owner + 88);
    phase = r_s16(table + index * 2);
    scratch = r_u32(0x80112DB4);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = r_s16(scratch);
        b = r_s16(control);
        product = (uint32)(a - b) * (uint32)(current - begin);
        quotient = resource002_clip_divide(product, split - begin);
        value = r_u16(control);
        w_u16(scratch, value + (uint32)quotient);
        a = r_s16(scratch + 4);
        product = (uint32)a * (uint32)(current - begin);
        quotient = resource002_clip_divide(product, split - begin);
        w_u16(scratch + 4, (uint32)quotient);
    }
    return 1;
}

uint32 resource002_sub_800df2e8(void)
{
    FUNCTION_MARKER(0x800DF2E8, "ACE3.BIN");
    return resource002_sub_800df2f0(r_u32(0x80112DC0));
}

uint32 resource002_sub_800df464(uint32 owner)
{
    uint32 control, table, index, first, middle, scratch, product, value;
    sint32 current, phase, begin, split, a, quotient;

    FUNCTION_MARKER(0x800DF464, "ACE3.BIN");
    control = r_u32(owner + 60);
    table = r_u32(owner + 80);
    index = (uint32)r_s16(control + 12);
    middle = (uint32)r_s16(control + 6);
    first = r_u16(control + 4);
    current = r_s16(table + index * 2);
    control = r_u32(owner + 60);
    r_u16(control + 8);
    control = r_u32(owner + 60);
    index = (uint32)r_s16(control + 12);
    table = r_u32(owner + 88);
    phase = r_s16(table + index * 2);
    scratch = r_u32(0x80112DB4);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = r_s16(control);
        product = (uint32)a * (uint32)(split - current);
        quotient = resource002_clip_divide(product, split - begin);
        value = r_u16(scratch);
        w_u16(scratch, value + (uint32)quotient);
        control = r_u32(owner + 60);
        a = r_s16(control + 2);
        product = (uint32)a * (uint32)(split - current);
        quotient = resource002_clip_divide(product, split - begin);
        value = r_u16(scratch + 2);
        w_u16(scratch + 2, value + (uint32)quotient);
    }
    return 1;
}

uint32 resource002_sub_800df45c(void)
{
    FUNCTION_MARKER(0x800DF45C, "ACE3.BIN");
    return resource002_sub_800df464(r_u32(0x80112DC0));
}

uint32 resource002_sub_800df5e0(uint32 owner)
{
    uint32 control, table, index, first, middle, scratch, product, square, denominator, value;
    sint32 current, phase, begin, split, a, quotient;

    FUNCTION_MARKER(0x800DF5E0, "ACE3.BIN");
    control = r_u32(owner + 60);
    table = r_u32(owner + 80);
    index = (uint32)r_s16(control + 12);
    middle = (uint32)r_s16(control + 6);
    first = r_u16(control + 4);
    current = r_s16(table + index * 2);
    control = r_u32(owner + 60);
    r_u16(control + 8);
    control = r_u32(owner + 60);
    index = (uint32)r_s16(control + 12);
    table = r_u32(owner + 88);
    phase = r_s16(table + index * 2);
    scratch = r_u32(0x80112DB4);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        square = (uint32)(split - current) * (uint32)(split - current);
        denominator = (uint32)(split - begin) * (uint32)(split - begin);
        a = r_s16(control);
        product = (uint32)a * square;
        quotient = resource002_clip_divide(product, (sint32)denominator);
        value = r_u16(scratch);
        w_u16(scratch, value + (uint32)quotient);
        control = r_u32(owner + 60);
        a = r_s16(control + 2);
        product = (uint32)a * square;
        quotient = resource002_clip_divide(product, (sint32)denominator);
        value = r_u16(scratch + 2);
        w_u16(scratch + 2, value + (uint32)quotient);
    }
    return 1;
}

uint32 resource002_sub_800df5d8(void)
{
    FUNCTION_MARKER(0x800DF5D8, "ACE3.BIN");
    return resource002_sub_800df5e0(r_u32(0x80112DC0));
}

static uint32 resource002_clip_call(uint32 target)
{
    switch (target)
    {
        case 0x800DEFEC:
            return resource002_sub_800defec();
        case 0x800DEFF4:
            return resource002_sub_800deff4();
        case 0x800DF2B0:
            return resource002_sub_800df2b0();
        case 0x800DF2E8:
            return resource002_sub_800df2e8();
        case 0x800DF45C:
            return resource002_sub_800df45c();
        case 0x800DF5D8:
            return resource002_sub_800df5d8();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 clipping target %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800df760(uint32 command, uint32 ordering, uint32 context)
{
    uint32 owner, table, packet, scratch, control, target, value, x, y, width, height;
    uint32 red, green, blue, layer;
    sint32 condition, expected, mode, offset_x, offset_y;

    FUNCTION_MARKER(0x800DF760, "ACE3.BIN");
    condition = r_s16(command + 12);
    if (condition)
    {
        owner = r_u32(0x80112DC0);
        table = r_u32(owner + 92);
        expected = r_s16(command + 14);
        value = r_u8(table + (uint32)condition);
        if ((uint32)expected != value)
            return command + 20;
    }
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    scratch = r_u32(0x80112DB4);
    w_u32(owner + 4, packet + 16);
    y = (uint32)r_s16(command + 2);
    x = r_u16(command);
    w_u32(scratch, x | (y << 16));
    height = (uint32)r_s16(command + 6);
    width = r_u16(command + 4);
    w_u32(scratch + 4, width | (height << 16));
    control = r_u32(owner + 60);
    mode = r_s16(control + 10);
    target = r_u32(0x8010AEAC + (uint32)mode * 4);
    if (!resource002_clip_call(target))
        return command + 20;
    scratch = r_u32(0x80112DB4);
    offset_x = r_s16(context + 4);
    x = (uint32)r_s16(scratch);
    offset_y = r_s16(context + 6);
    y = (uint32)r_s16(scratch + 2);
    w_u32(packet + 8, ((x + (uint32)offset_x) & 0xFFFF) | ((y + (uint32)offset_y) << 16));
    height = (uint32)r_s16(scratch + 6);
    width = r_u16(scratch + 4);
    w_u32(packet + 12, width | (height << 16));
    red = r_u8(command + 8);
    green = r_u8(command + 9);
    blue = r_u8(command + 10);
    w_u8(packet + 3, 3);
    w_u32(packet + 4, red | (green << 8) | (blue << 16));
    w_u8(packet + 7, 96);
    value = r_u8(command + 11);
    w_u8(packet + 7, value ? 98 : 96);
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    value = r_u32(ordering + layer * 4);
    w_u32(packet, value | 0x03000000);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    w_u32(ordering + layer * 4, packet & 0xFFFFFF);
    return command + 20;
}

uint32 resource002_sub_800df940(uint32 command, uint32 ordering, uint32 context)
{
    uint32 owner, table, packet, scratch, control, target, value, x, y, width, height;
    uint32 red, green, blue, layer, u, v, page;
    sint32 condition, expected, mode, offset_x, offset_y;

    FUNCTION_MARKER(0x800DF940, "ACE3.BIN");
    condition = r_s16(command + 20);
    if (condition)
    {
        owner = r_u32(0x80112DC0);
        table = r_u32(owner + 92);
        expected = r_s16(command + 22);
        value = r_u8(table + (uint32)condition);
        if ((uint32)expected != value)
            return command + 28;
    }
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    scratch = r_u32(0x80112DB4);
    w_u32(owner + 4, packet + 20);
    y = (uint32)r_s16(command + 2);
    x = r_u16(command);
    w_u32(scratch, x | (y << 16));
    height = (uint32)r_s16(command + 18);
    width = r_u16(command + 16);
    w_u32(scratch + 4, width | (height << 16));
    control = r_u32(owner + 60);
    mode = r_s16(control + 10);
    target = r_u32(0x8010AEAC + (uint32)mode * 4);
    if (!resource002_clip_call(target))
        return command + 28;
    scratch = r_u32(0x80112DB4);
    offset_x = r_s16(context + 4);
    x = (uint32)r_s16(scratch);
    y = (uint32)r_s16(scratch + 2);
    offset_y = r_s16(context + 6);
    w_u32(packet + 8, ((x + (uint32)offset_x) & 0xFFFF) | ((y + (uint32)offset_y) << 16));
    width = (uint32)r_s16(command + 16);
    height = (uint32)r_s16(command + 18);
    w_u32(packet + 16, ((width << 2) & 0xFFFF) | (height << 16));
    u = (uint32)(r_s16(command + 12) % 64) << 2;
    v = (uint32)(r_s16(command + 14) % 256) << 8;
    page = (uint32)r_s16(command + 10) << 6;
    value = r_u16(command + 8);
    page |= (value >> 4) & 63;
    w_u32(packet + 12, u | v | (page << 16));
    red = r_u8(command + 4);
    green = r_u8(command + 5);
    blue = r_u8(command + 6);
    w_u8(packet + 3, 4);
    w_u32(packet + 4, red | (green << 8) | (blue << 16));
    w_u8(packet + 7, 100);
    value = r_u8(command + 7);
    w_u8(packet + 7, value ? 102 : 100);
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    value = r_u32(ordering + layer * 4);
    w_u32(packet, value | 0x04000000);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    w_u32(ordering + layer * 4, packet & 0xFFFFFF);
    return command + 28;
}

uint32 resource002_sub_800e01c4(uint32 command, uint32 ordering, uint32 context)
{
    uint32 owner, table, packet, scratch, control, target, value, x, y, width, height;
    uint32 red, green, blue, layer, tag, link;
    sint32 condition, expected, mode;

    FUNCTION_MARKER(0x800E01C4, "ACE3.BIN");
    condition = r_s16(command + 12);
    if (condition)
    {
        owner = r_u32(0x80112DC0);
        table = r_u32(owner + 92);
        expected = r_s16(command + 14);
        value = r_u8(table + (uint32)condition);
        if ((uint32)expected != value)
            return command + 20;
    }
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    scratch = r_u32(0x80112DB4);
    w_u32(owner + 4, packet + 16);
    y = (uint32)r_s16(command + 2);
    x = r_u16(command);
    w_u32(scratch, x | (y << 16));
    height = (uint32)r_s16(command + 6);
    width = r_u16(command + 4);
    w_u32(scratch + 4, width | (height << 16));
    control = r_u32(owner + 60);
    mode = r_s16(control + 10);
    target = r_u32(0x8010AEAC + (uint32)mode * 4);
    if (!resource002_clip_call(target))
        return command + 20;
    scratch = r_u32(0x80112DB4);
    value = r_u16(context + 4);
    x = r_u16(scratch);
    w_u16(packet + 8, x + value);
    y = r_u16(scratch + 2);
    value = r_u16(context + 6);
    w_u16(packet + 10, y + value);
    width = r_u16(scratch + 4);
    value = r_u16(context + 4);
    w_u16(packet + 12, width + value);
    height = r_u16(scratch + 6);
    value = r_u16(context + 6);
    w_u16(packet + 14, height + value);
    red = r_u8(command + 8);
    green = r_u8(command + 9);
    blue = r_u8(command + 10);
    w_u8(packet + 3, 3);
    w_u32(packet + 4, red | (green << 8) | (blue << 16));
    w_u8(packet + 7, 64);
    value = r_u8(command + 11);
    w_u8(packet + 7, value ? 66 : 64);
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + layer * 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    tag = r_u32(ordering + layer * 4);
    w_u32(ordering + layer * 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return command + 20;
}

uint32 resource002_sub_800dfcc8(uint32 command, uint32 ordering, uint32 context)
{
    uint32 owner, table, packet, scratch, control, target, value, x, y, width, height;
    uint32 blend, semitransparent, scale, format, red, green, blue, layer, u, v, du, dv;
    uint32 product, clut, bottom, right, page, a, b, c;
    sint32 condition, expected, mode;
    int negative;

    FUNCTION_MARKER(0x800DFCC8, "ACE3.BIN");
    condition = r_s16(command + 24);
    if (condition)
    {
        owner = r_u32(0x80112DC0);
        table = r_u32(owner + 92);
        expected = r_s16(command + 26);
        value = r_u8(table + (uint32)condition);
        if ((uint32)expected != value)
            return command + 32;
    }
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 4);
    blend = 0;
    w_u32(owner + 4, packet + 40);
    value = r_u8(command + 7);
    semitransparent = value != 255;
    if (value != 255)
        blend = r_u8(command + 7);
    y = (uint32)r_s16(command + 2);
    x = r_u16(command);
    scratch = r_u32(0x80112DB4);
    w_u32(scratch, x | (y << 16));
    height = (uint32)r_s16(command + 22);
    width = r_u16(command + 20);
    w_u32(scratch + 4, width | (height << 16));
    control = r_u32(owner + 60);
    mode = r_s16(control + 10);
    target = r_u32(0x8010AEAC + (uint32)mode * 4);
    if (!resource002_clip_call(target))
        return command + 32;
    format = r_u8(command + 30);
    if (format == 0)
        scale = 4;
    else if (format == 1)
        scale = 2;
    else
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 textured command has undefined scale for format %u\n", format);
#endif
        abort();
    }
    negative = r_s16(command + 20) < 0;
    value = (uint32)r_s16(command + 12);
    u = (uint32)((sint32)value % 64) * scale;
    if (negative)
        --u;
    negative = r_s16(command + 22) < 0;
    value = (uint32)r_s16(command + 14);
    v = (uint32)((sint32)value % 256);
    if (negative)
        --v;
    value = (uint32)r_s16(command + 16);
    product = value * scale;
    width = (uint32)r_s16(command + 20);
    du = width == product ? width : product - 1;
    height = (uint32)r_s16(command + 22);
    value = (uint32)r_s16(command + 18);
    a = r_u16(command + 18);
    dv = height == value ? a : a - 1;
    if ((sint16)u + (sint16)du == 256)
        --du;
    if ((sint16)v + (sint16)dv == 256)
        --dv;
    red = r_u8(command + 4);
    green = r_u8(command + 5);
    blue = r_u8(command + 6);
    w_u8(packet + 3, 9);
    w_u32(packet + 4, red | (green << 8) | (blue << 16));
    w_u8(packet + 7, 44);
    w_u8(packet + 7, semitransparent ? 46 : 44);
    u = (uint32)(sint32)(sint16)u;
    v = (uint32)(sint32)(sint16)v;
    clut = (uint32)r_s16(command + 10) << 6;
    value = r_u16(command + 8);
    clut |= (value >> 4) & 63;
    bottom = (v + (uint32)(sint32)(sint16)dv) << 8;
    w_u32(packet + 12, u | (v << 8) | (clut << 16));
    format = r_u8(command + 30);
    a = r_u16(command + 14);
    b = r_u16(command + 12);
    w_u32(packet + 28, u | bottom);
    right = u + (uint32)(sint32)(sint16)du;
    w_u32(packet + 36, right | bottom);
    page = ((format & 3) << 7) | ((blend & 3) << 5) | ((a & 0x100) >> 4) | ((b & 0x3FF) >> 6) | ((a & 0x200) << 2);
    w_u32(packet + 20, right | (v << 8) | (page << 16));
    scratch = r_u32(0x80112DB4);
    a = (uint32)r_s16(context + 6);
    y = (uint32)r_s16(scratch + 2);
    b = (uint32)r_s16(context + 4);
    x = (uint32)r_s16(scratch);
    w_u32(packet + 8, ((y + a) << 16) | ((x + b) & 0xFFFF));
    y = (uint32)r_s16(scratch + 2);
    a = (uint32)r_s16(context + 6);
    b = (uint32)r_s16(context + 4);
    width = (uint32)r_s16(scratch + 4);
    x = (uint32)r_s16(scratch);
    w_u32(packet + 16, ((y + a) << 16) | ((x + b + width) & 0xFFFF));
    y = (uint32)r_s16(scratch + 2);
    a = (uint32)r_s16(context + 6);
    height = (uint32)r_s16(scratch + 6);
    x = (uint32)r_s16(scratch);
    b = (uint32)r_s16(context + 4);
    w_u32(packet + 24, ((y + a + height) << 16) | ((x + b) & 0xFFFF));
    y = (uint32)r_s16(scratch + 2);
    a = (uint32)r_s16(context + 6);
    height = (uint32)r_s16(scratch + 6);
    width = (uint32)r_s16(scratch + 4);
    x = (uint32)r_s16(scratch);
    b = (uint32)r_s16(context + 4);
    c = ((y + a + height) << 16) | ((x + b + width) & 0xFFFF);
    w_u32(packet + 32, c);
    owner = r_u32(0x80112DC0);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    value = r_u32(ordering + layer * 4);
    w_u32(packet, value | 0x09000000);
    control = r_u32(owner + 60);
    layer = (uint32)r_s16(control + 14);
    w_u32(ordering + layer * 4, packet & 0xFFFFFF);
    return command + 32;
}

uint32 resource002_sub_800fbc48(void)
{
    uint32 state, game, bank, value, resource, record, delta;
    sint32 mission;

    FUNCTION_MARKER(0x800FBC48, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u8(state + 124, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 628, value);
    resource = sub_80027338(0x8010ED18, 0);
    resource002_sub_800de908(resource, 8, 2);
    resource002_sub_800dedf0(2, 0);
    resource002_sub_800dee08(1, 1, 0, 120);
    game = r_u32(0x800BCE60);
    if (r_u8(game + 2391) == 1)
    {
        resource002_sub_800dedf0(2, 64);
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        w_u32(state + 80, 120);
        mission = r_s16(game + 908);
        delta = (uint32)r_s16(game + 2392);
        record = game + (uint32)mission * 4;
        value = r_u32(record + 3900);
        w_u32(record + 3900, value + delta);
        delta = (uint32)r_s16(game + 2392);
        value = r_u32(game + 4148);
        w_u32(game + 4148, value + delta);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 1)
    {
        resource002_sub_800dedf0(2, 128);
        state = r_u32(0x80112DC4);
        w_u32(state + 80, 120);
    }
    return 0x04000000;
}

uint32 resource002_sub_800fbd7c(uint32 game)
{
    static const uint8 empty_arguments[1] = {0};
    uint32 flags, state, bank, packet, ordering, tag, link, value;

    FUNCTION_MARKER(0x800FBD7C, "ACE3.BIN");
    flags = r_u32(game + 2412);
    if (flags & 8)
        return 0x04000000;
    value = r_u8(game + 2391);
    if (value == 0 && !(flags & 1))
        return 0x04000000;
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    packet = r_u32(state + bank * 4);
    w_u32(state + 8, packet);
    ClearOTagR(psx_addr(packet, 64), 16);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    packet = r_u32(state + 8);
    ordering = r_u32(game + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    packet = r_u32(state + 8) + 60;
    ordering = r_u32(game + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    bank = r_u32(game + 4);
    ordering = r_u32(state + 8);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 628, value);
    resource002_sub_800f3db4(ordering);
    resource002_sub_800f35b8(7);
    resource002_sub_800f33f8(0xFFFFFFFF);
    resource002_sub_800f34c8(1, 1, 1);
    resource002_sub_800f34b0(1);
    resource002_sub_800f33d0(24, 184, 208);
    game = r_u32(0x800BCE60);
    if (r_u8(game + 2391) == 1)
    {
        resource002_sub_800f33a8(202, 155);
        resource002_sub_800f4ae0(0x800CD678, 0, empty_arguments, 0);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 1)
    {
        resource002_sub_800f33a8(233, 145);
        resource002_sub_800f4ae0(0x800CD688, 0, empty_arguments, 0);
    }
    game = r_u32(0x800BCE60);
    value = r_u32(game + 300);
    if ((value & 16) && (r_u32(game + 296) & 47) == 0)
        return 0x04000000;
    state = r_u32(0x80112DC4);
    value = r_u32(state + 80);
    if ((sint32)value <= 0)
        return 0x04000000;
    packet = r_u32(state + 628);
    w_u32(state + 80, value - 1);
    value = resource002_sub_800e2494(packet);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    return 0;
}

uint32 resource002_sub_800fbd74(void)
{
    FUNCTION_MARKER(0x800FBD74, "ACE3.BIN");
    return resource002_sub_800fbd7c(r_u32(0x800BCE60));
}

uint32 resource002_sub_800fb4f0(void)
{
    uint32 context, bytes, buffer, game, state;

    FUNCTION_MARKER(0x800FB4F0, "ACE3.BIN");
    context = r_u32(0x800BCE50);
    w_u32(context + 96, 113);
    bytes = resource_total_size(context + 96, 1);
    buffer = arena_alloc(bytes);
    context = r_u32(0x800BCE50);
    game = r_u32(0x800BCE60);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    state = r_u32(0x80112DC4);
    w_u32(state + 456, 1);
    w_u32(state + 460, 0);
    return 1;
}

uint32 resource002_sub_800fb574(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800FB574, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return r_u32(game + 68) == 0;
}

uint32 resource002_sub_800fb594(uint32 context)
{
    uint32 entries, state, value, resource;

    FUNCTION_MARKER(0x800FB594, "ACE3.BIN");
    entries = sub_80027338(r_u32(context + 96), 0);
    resource002_sub_800ecda8(r_u32(entries));
    state = r_u32(0x80112DC4);
    if (r_u32(state + 128) == 0)
        resource002_sub_800ecda8(r_u32(entries + 8));
    state = r_u32(0x80112DC4);
    if (r_u32(state + 128) == 1)
        resource002_sub_800ecda8(r_u32(entries + 12));
    state = r_u32(0x80112DC4);
    if (r_u32(state + 128) == 2)
        resource002_sub_800ecda8(r_u32(entries + 16));
    state = r_u32(0x80112DC4);
    if (r_u32(state + 128) == 3)
        resource002_sub_800ecda8(r_u32(entries + 20));
    sub_8002d96c(r_u32(entries + 4));
    arena_set_cursor(r_u32(entries + 4));
    state = r_u32(0x80112DC4);
    if (r_s16(state + 454) == 0)
    {
        value = r_u32(state + 128);
        resource002_sub_800dedf0(2, ((uint32)1 << ((value + 2) & 31)) | 2);
    }
    state = r_u32(0x80112DC4);
    value = r_u32(state + 128);
    resource = r_u32(0x80111700 + value * 4);
    value = sub_8002db4c(resource);
    state = r_u32(0x80112DC4);
    w_u32(state + 304, value);
    return 1;
}

uint32 resource002_sub_800fb58c(void)
{
    FUNCTION_MARKER(0x800FB58C, "ACE3.BIN");
    return resource002_sub_800fb594(r_u32(0x800BCE50));
}

uint32 resource002_sub_800fbb74(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800FBB74, "ACE3.BIN");
    if (resource002_sub_800dee84(1) != 2)
    {
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 300) & 16))
            return 0;
        if (r_u32(game + 296) & 47)
            return 0;
    }
    resource002_sub_800dedf0(2, 0);
    return 2;
}

uint32 resource002_sub_800def94(uint32 index, uint32 value)
{
    uint32 owner, result;

    FUNCTION_MARKER(0x800DEF94, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    result = r_u32(owner + 92) + index;
    w_u8(result, value);
    return result;
}

uint32 resource002_sub_800f7244(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, vertex, channel, source, destination, tag, link;

    FUNCTION_MARKER(0x800F7244, "ACE3.BIN");
    w_u8(packet + 3, 8);
    w_u8(packet + 7, 56);
    value = native_descriptor ? native_descriptor[19] : r_u8(descriptor + 19);
    w_u8(packet + 7, value ? 58 : 56);
    for (vertex = 0; vertex < 4; ++vertex)
    {
        for (channel = 0; channel < 3; ++channel)
        {
            source = 16 + vertex * 4 + channel;
            destination = 4 + vertex * 8 + channel;
            value = native_descriptor ? native_descriptor[source] : r_u8(descriptor + source);
            w_u8(packet + destination, value);
        }
    }
    for (vertex = 0; vertex < 4; ++vertex)
    {
        source = vertex * 4;
        destination = 8 + vertex * 8;
        value = native_descriptor ? xport_load_le16(native_descriptor + source) : r_u16(descriptor + source);
        w_u16(packet + destination, value);
        value = native_descriptor ? xport_load_le16(native_descriptor + source + 2) : r_u16(descriptor + source + 2);
        if (vertex == 3)
            tag = r_u32(packet);
        w_u16(packet + destination + 2, value);
    }
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 36;
}

uint32 resource002_sub_800fb330(uint32 packet)
{
    uint8 descriptor[32];
    uint32 state, completed, total, product, ordering, result, i;
    sint32 amount, enabled;

    FUNCTION_MARKER(0x800FB330, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
    {
        descriptor[16 + i * 4] = 229;
        descriptor[17 + i * 4] = 229;
        descriptor[18 + i * 4] = 213;
    }
    descriptor[19] = 0;
    state = r_u32(0x80112DC4);
    completed = r_u32(state + 460);
    total = r_u32(state + 456);
    product = completed * 115;
    amount = resource002_clip_divide(product, (sint32)total);
    if (amount >= 115)
        amount = 114;
    else if (amount < 0)
        amount = 0;
    xport_store_le16(descriptor, 192);
    xport_store_le16(descriptor + 8, 192);
    state = r_u32(0x80112DC4);
    xport_store_le16(descriptor + 2, 420);
    xport_store_le16(descriptor + 6, 420);
    xport_store_le16(descriptor + 4, (uint32)amount + 192);
    xport_store_le16(descriptor + 10, 426);
    xport_store_le16(descriptor + 12, (uint32)amount + 192);
    xport_store_le16(descriptor + 14, 426);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(0, packet, ordering, descriptor);
    for (i = 0; i < 6; ++i)
    {
        state = r_u32(0x80112DC4);
        completed = r_u32(state + 460);
        total = r_u32(state + 456);
        product = completed * 7;
        amount = resource002_clip_divide(product, (sint32)total);
        enabled = (sint32)i < amount;
        resource002_sub_800def94(i + 1, (uint32)enabled);
    }
    return result;
}

static uint32 resource002_loading_screen_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800FB4F0:
            return resource002_sub_800fb4f0();
        case 0x800FB574:
            return resource002_sub_800fb574();
        case 0x800FB58C:
            return resource002_sub_800fb58c();
        case 0x800FB6E8:
            return resource002_sub_800fb6e8();
        case 0x800FB7A4:
            return resource002_sub_800fb7a4();
        case 0x800FB7F4:
            return resource002_sub_800fb7f4();
        case 0x800FB948:
            return resource002_sub_800fb948();
        case 0x800FB9C4:
            return resource002_sub_800fb9c4();
        case 0x800FBA30:
            return resource002_sub_800fba30();
        case 0x800FBB74:
            return resource002_sub_800fbb74();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 loading phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800fbf70(void)
{
    uint32 game, state, bank, value, flags, phase, target, result, packet, ordering, tag, link;

    FUNCTION_MARKER(0x800FBF70, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 628, value);
    w_u8(game + 2391, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    state = r_u32(0x80112DC4);
    w_u32(game + 2412, flags & 0xFFFFFFFE);
    if (r_s16(state + 454))
    {
        w_u16(state + 136, 0);
        w_u16(state + 138, 0);
        return 0x04000000;
    }
    if (flags & 8)
        return 0x04000000;
    if (r_u8(game + 2399) == 1)
        return 0x04000000;
    if (r_u8(game + 2398))
        return 0x04000000;
    if (r_s16(game + 908) == 59)
        return 0x04000000;
    phase = r_u8(state + 125);
    target = r_u32(0x80111730 + phase * 4);
    result = resource002_loading_screen_phase_call(target);
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    packet = r_u32(state + bank * 4);
    w_u32(state + 8, packet);
    ClearOTagR(psx_addr(packet, 64), 16);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    packet = r_u32(state + 8);
    ordering = r_u32(game + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    packet = r_u32(state + 8) + 60;
    ordering = r_u32(game + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    value = resource002_sub_800fb330(r_u32(state + 628));
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800e2494(value);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    if (result == 1)
    {
        phase = r_u8(state + 125);
        w_u8(state + 125, phase + 1);
        return 0;
    }
    if (result == 2)
    {
        w_u16(state + 136, 0);
        w_u16(state + 138, 0);
        w_u16(state + 454, 1);
        return 0x04000000;
    }
    return 0;
}

uint32 resource002_sub_800fd634(void)
{
    uint32 state, game, flags;

    FUNCTION_MARKER(0x800FD634, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u8(state + 124, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    if (flags & 32)
        w_u16(game + 908, 59);
    else if (flags & 8)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 454, 3);
    }
    return 0x04000000;
}

uint32 resource002_sub_800fd6a0(uint32 state)
{
    uint32 game, phase, target, result;

    FUNCTION_MARKER(0x800FD6A0, "ACE3.BIN");
    if (r_s16(state + 454))
    {
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 2412) & 8))
            return 0x04000000;
    }
    state = r_u32(0x80112DC4);
    phase = r_u8(state + 125);
    target = r_u32(0x80111710 + phase * 4);
    result = resource002_debrief_phase_call(target);
    if (result == 0)
        return 0;
    if (result == 1)
    {
        state = r_u32(0x80112DC4);
        phase = r_u8(state + 125);
        w_u8(state + 125, phase + 1);
        return 0;
    }
    if (result == 2)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 136, 0);
        w_u16(state + 138, 0);
        w_u16(state + 454, 1);
        return 0x04000000;
    }
    return 2;
}

uint32 resource002_sub_800fd698(void)
{
    FUNCTION_MARKER(0x800FD698, "ACE3.BIN");
    return resource002_sub_800fd6a0(r_u32(0x80112DC4));
}

uint32 resource002_sub_800cd7c0(void)
{
    uint32 game, bank, state, value;

    FUNCTION_MARKER(0x800CD7C0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 628, value);
    return 1;
}

uint32 resource002_sub_800fedec(sint32 mission)
{
    uint32 row, column, value, game;

    FUNCTION_MARKER(0x800FEDEC, "ACE3.BIN");
    for (row = 0; row < 62; ++row)
    {
        for (column = 0; column < 4; ++column)
        {
            value = r_u8(0x80112228 + row * 4 + column);
            if (value == (uint32)mission)
            {
                game = r_u32(0x800BCE60);
                value = r_u8(game + row + 3400);
                if (value == 0)
                    w_u8(game + row + 3400, 1);
            }
        }
    }
    return 1;
}

uint32 resource002_sub_800ff2bc(void)
{
    uint32 effect;

    FUNCTION_MARKER(0x800FF2BC, "ACE3.BIN");
    sub_8002db4c(98);
    effect = r_u32(0x80112DA8);
    w_u32(effect + 20, 1);
    w_u32(effect + 32, 0);
    return 1;
}

uint32 resource002_sub_800cd7f0(void)
{
    uint32 game, bank, state, value, scene, buffer, index, effect, voice;
    sint32 mission, mode;

    FUNCTION_MARKER(0x800CD7F0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    state = r_u32(0x80112DC4);
    value = r_u32(state + bank * 4 + 632);
    w_u32(state + 76, 0);
    w_u32(state + 624, 0);
    w_u32(state + 628, value);
    buffer = arena_alloc(256);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 592, buffer);
    for (index = 0; index < 64; ++index)
    {
        buffer = arena_alloc(28);
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 592);
        w_u32(value + index * 4, buffer);
    }
    game = r_u32(0x800BCE60);
    value = r_u32(game + 2404);
    mission = r_s16(game + 908);
    w_u32(game + 2404, value | 1);
    resource002_sub_800fedec(mission);
    state = r_u32(0x80112DC4);
    effect = r_u32(0x80112DA8);
    w_u32(state + 148, 0);
    w_u16(effect + 110, 0);
    SetGeomOffset(160, 240);
    SetGeomScreen(250);
    state = r_u32(0x80112DC4);
    w_u16(state + 48, 0);
    w_u16(state + 50, 0);
    w_u32(state + 324, 0);
    w_u32(state + 328, 1);
    value = sub_8002db4c(93);
    state = r_u32(0x80112DC4);
    w_u32(state + 304, value);
    value = sub_8002db4c(94);
    state = r_u32(0x80112DC4);
    w_u32(state + 308, value);
    value = sub_8002db4c(95);
    state = r_u32(0x80112DC4);
    w_u32(state + 312, value);
    value = sub_8002db4c(96);
    state = r_u32(0x80112DC4);
    w_u32(state + 316, value);
    value = sub_8002db4c(97);
    state = r_u32(0x80112DC4);
    voice = r_u32(state + 304);
    w_u32(state + 320, value);
    sound_set_voice_fade(voice, 0, 1);
    for (index = 308; index <= 320; index += 4)
    {
        state = r_u32(0x80112DC4);
        voice = r_u32(state + index);
        sound_set_voice_fade(voice, 0, 1);
    }
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 2399);
    if (value == 1)
    {
        state = r_u32(0x80112DC4);
        w_u8(state + 124, 4);
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 2);
    }
    else if (r_u32(game + 2412) & 8)
    {
        state = r_u32(0x80112DC4);
        w_u8(state + 124, 1);
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 0);
    }
    else
    {
        mode = r_s16(game + 2388);
        state = r_u32(0x80112DC4);
        w_u8(state + 124, mode != 0 && mode != 3);
    }
    state = r_u32(0x80112DC4);
    w_u8(state + 126, 4);
    state = r_u32(0x80112DC4);
    w_u8(state + 127, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 432, 0);
    effect = r_u32(0x80112DC4);
    w_u32(state + 284, 0);
    w_u16(state + 52, 2);
    w_u8(effect + 433, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 434, 0);
    game = r_u32(0x800BCE60);
    w_u32(game + 4156, 12);
    resource002_sub_800f35b8(1);
    resource002_sub_800f33f8(-1);
    resource002_sub_800f34c8(1, 1, 1);
    resource002_sub_800f34b0(1);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    w_u32(state + 80, 0);
    if (!(r_u32(game + 2412) & 8))
    {
        mode = r_s16(game + 2388);
        if (mode == 0 || mode == 3)
            return 1;
    }
    return 2;
}

uint32 resource002_sub_800cdaf4(void)
{
    uint32 state, display, value, flags;

    FUNCTION_MARKER(0x800CDAF4, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    value = r_u32(state + 80) * (uint32)143;
    display = r_u32(0x800BCE48);
    w_u8(display + 16, (sint32)value / 32);
    state = r_u32(0x80112DC4);
    value = r_u32(state + 80) * (uint32)135;
    display = r_u32(0x800BCE48);
    w_u8(display + 17, (sint32)value / 32);
    state = r_u32(0x80112DC4);
    value = r_u32(state + 80) * (uint32)83;
    display = r_u32(0x800BCE48);
    w_u8(display + 18, (sint32)value / 32);
    display = r_u32(0x800BCE48);
    flags = r_u32(display);
    state = r_u32(0x80112DC4);
    w_u32(display, flags | 8);
    value = r_u32(state + 80) + 1;
    w_u32(state + 80, value);
    if ((sint32)value < 33)
        return 0;
    w_u32(state + 80, 0);
    return 1;
}

uint32 resource002_sub_800cdbf8(uint32 game)
{
    sint32 mode;

    FUNCTION_MARKER(0x800CDBF8, "ACE3.BIN");
    mode = r_s16(game + 2388);
    if (mode == 0 || mode == 3)
        resource002_sub_800ff2bc();
    game = r_u32(0x800BCE60);
    mode = r_s16(game + 2388);
    if (mode == 2 || (r_u32(game + 2412) & 32))
        resource002_sub_800ff2bc();
    return 1;
}

uint32 resource002_sub_800cdbf0(void)
{
    FUNCTION_MARKER(0x800CDBF0, "ACE3.BIN");
    return resource002_sub_800cdbf8(r_u32(0x800BCE60));
}

uint32 resource002_sub_800cdc68(void)
{
    uint32 game, effect;
    sint32 mode;

    FUNCTION_MARKER(0x800CDC68, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    mode = r_s16(game + 2388);
    if (mode != 0 && mode != 3 && mode != 2 && !(r_u32(game + 2412) & 32))
        return 0;
    effect = r_u32(0x80112DA8);
    return r_u32(effect + 20) == 1 ? 2 : 0;
}

static uint32 resource002_mission_init_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800CD7C0:
            return resource002_sub_800cd7c0();
        case 0x800CD7F0:
            return resource002_sub_800cd7f0();
        case 0x800CDAF4:
            return resource002_sub_800cdaf4();
        case 0x800CDBF0:
            return resource002_sub_800cdbf0();
        case 0x800CDC68:
            return resource002_sub_800cdc68();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 mission initializer phase 0x%08X\n", target);
#endif
            abort();
            return 0;
    }
}

uint32 resource002_sub_800cdcd4(void)
{
    uint32 state, phase, target;
    sint32 result;

    FUNCTION_MARKER(0x800CDCD4, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = (uint32)r_s16(state + 138);
    target = r_u32(0x8010982C + phase * 4);
    result = (sint32)resource002_mission_init_phase_call(target);
    if (result == 1)
    {
        state = r_u32(0x80112DC4);
        phase = r_u16(state + 138);
        w_u16(state + 138, phase + 1);
        return 0;
    }
    if (result < 2)
        return 0;
    if (result == 2)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 454, 0);
    }
    return 0x04000000;
}

uint32 resource002_sub_800ff108(uint32 ordering, uint32 game)
{
    uint32 bank, effect, packet, value, tag, link;

    FUNCTION_MARKER(0x800FF108, "ACE3.BIN");
    bank = r_u32(game + 4);
    effect = r_u32(0x80112DA8);
    packet = r_u32(effect + bank * 4);
    w_u32(effect + 8, packet);
    ClearOTagR(psx_addr(packet, 1024), 256);
    effect = r_u32(0x80112DA8);
    packet = r_u32(effect + 8);
    link = r_u32(ordering);
    tag = r_u32(packet);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    tag = r_u32(ordering);
    packet = r_u32(effect + 8);
    value = (packet + 1020) & 0xFFFFFF;
    w_u32(ordering, (tag & 0xFF000000) | value);
    return value;
}

uint32 resource002_sub_800ff100(uint32 ordering)
{
    FUNCTION_MARKER(0x800FF100, "ACE3.BIN");
    return resource002_sub_800ff108(ordering, r_u32(0x800BCE60));
}

uint32 resource002_sub_8010084c(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 value, state;

    FUNCTION_MARKER(0x8010084C, "ACE3.BIN");
    resource002_sub_800ff4ec(effect, scratch);
    packet = resource002_sub_800ff95c(effect, scratch, packet);
    packet = resource002_sub_80100304(effect, scratch, packet);
    value = r_u32(effect + 20);
    if (value == 1)
    {
        value = r_u32(effect + 32);
        if ((sint32)value / 8 < 12)
            return packet;
        w_u32(effect + 32, 0);
        w_u32(effect + 20, 2);
        value = sub_8002db4c(93);
        state = r_u32(0x80112DC4);
        w_u32(state + 304, value);
        value = sub_8002db4c(94);
        state = r_u32(0x80112DC4);
        w_u32(state + 308, value);
        value = sub_8002db4c(95);
        state = r_u32(0x80112DC4);
        w_u32(state + 312, value);
        value = sub_8002db4c(96);
        state = r_u32(0x80112DC4);
        w_u32(state + 316, value);
        value = sub_8002db4c(97);
        state = r_u32(0x80112DC4);
        w_u32(state + 320, value);
    }
    else if (value == 3)
    {
        value = r_u32(effect + 32);
        if ((sint32)value / 8 >= 12)
        {
            w_u32(effect + 32, 0);
            w_u32(effect + 20, 0);
        }
    }
    return packet;
}

uint32 resource002_sub_80101e4c(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 value;

    FUNCTION_MARKER(0x80101E4C, "ACE3.BIN");
    value = r_u32(effect + 28);
    if (value == 1)
    {
        w_u32(effect + 28, 2);
        value = r_u32(effect + 28);
    }
    if (value == 3)
    {
        value = r_u32(effect + 40);
        if ((sint32)value < 60)
            w_u32(effect + 40, value + 1);
        else
            w_u32(effect + 28, 0);
    }
    resource002_sub_801016a4(effect, scratch);
    return resource002_sub_80101ad4(effect, scratch, packet);
}

uint32 resource002_sub_80101fec(uint32 packet)
{
    uint32 effect, first, second, value;

    FUNCTION_MARKER(0x80101FEC, "ACE3.BIN");
    effect = r_u32(0x80112DA8);
    xport_gte_write_control(24, 0x00A00000);
    xport_gte_write_control(25, 0x00F00000);
    xport_gte_write_control(26, 250);
    xport_gte_write_control(27, 0xFFFFD450);
    xport_gte_write_control(28, 0x01555000);
    first = r_u32(effect + 20);
    second = r_u32(effect + 24);
    if (first)
        packet = resource002_sub_8010084c(effect, 0x1F800000, packet);
    if (second)
        packet = resource002_sub_80100b94(effect, 0x1F800000, packet);
    if (!first && !second)
    {
        value = r_u32(effect + 28);
        if (value)
            packet = resource002_sub_80101e4c(effect, 0x1F800000, packet);
    }
    value = r_u32(effect + 12);
    w_u32(effect + 12, value + 1);
    return packet;
}

uint32 resource002_sub_800de718(void)
{
    uint32 state, other;

    FUNCTION_MARKER(0x800DE718, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u8(state + 126, 0);
    other = r_u32(0x80112DC4);
    w_u32(state + 80, 0);
    w_u8(other + 125, 1);
    return 1;
}

uint32 resource002_sub_800de73c(void)
{
    uint32 game, state, value;

    FUNCTION_MARKER(0x800DE73C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game + 300) & 32;
    if (value == 0)
        return value;
    value = r_u32(game + 296) & 0x5F;
    if (value)
        return value;
    state = r_u32(0x80112DC4);
    w_u8(state + 124, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 126, 4);
    return 4;
}

uint32 resource002_sub_800ff1a4(uint32 mode, uint32 effect)
{
    uint32 state, voice, percent, offset, result;

    FUNCTION_MARKER(0x800FF1A4, "ACE3.BIN");
    w_u32(effect + 20, mode);
    w_u32(effect + 32, 0);
    if (mode == 1)
    {
        sub_8002db4c(98);
        percent = 100;
    }
    else if (mode == 3)
    {
        sub_8002db4c(99);
        percent = 0;
    }
    else
        return 3;
    result = 0;
    for (offset = 304; offset <= 320; offset += 4)
    {
        state = r_u32(0x80112DC4);
        voice = r_u32(state + offset);
        result = sound_set_voice_fade(voice, percent, 120);
    }
    return result;
}

uint32 resource002_sub_800ff19c(uint32 mode)
{
    FUNCTION_MARKER(0x800FF19C, "ACE3.BIN");
    return resource002_sub_800ff1a4(mode, r_u32(0x80112DA8));
}

uint32 resource002_sub_800de4bc(void)
{
    uint32 state, value, display;

    FUNCTION_MARKER(0x800DE4BC, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    value = r_u8(state + 125);
    w_u8(state + 125, value + 1);
    display = r_u32(0x800BCE48);
    w_u8(display + 16, 143);
    display = r_u32(0x800BCE48);
    w_u8(display + 17, 135);
    display = r_u32(0x800BCE48);
    w_u8(display + 18, 83);
    display = r_u32(0x800BCE48);
    value = r_u32(display);
    w_u32(display, value | 8);
    resource002_sub_800ff19c(2);
    state = r_u32(0x80112DC4);
    w_u16(state + 454, 10);
    return 10;
}

uint32 resource002_sub_800e6658(void)
{
    uint32 state, game, flags, phase;
    sint32 mode;

    FUNCTION_MARKER(0x800E6658, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u8(state + 126, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 1);
    game = r_u32(0x800BCE60);
    mode = r_s16(game + 2388);
    if (mode == 0)
        phase = 1;
    else if (mode == 1)
        phase = 5;
    else if (mode == 2)
    {
        flags = r_u32(game + 2412);
        phase = (flags & 128) ? 7 : ((flags & 64) ? 5 : 7);
    }
    else if (mode == 3)
        phase = 8;
    else
        phase = 7;
    state = r_u32(0x80112DC4);
    w_u16(state + 132, phase);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412);
    if (flags & 32)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 132, 5);
        w_u16(state + 134, 0);
    }
    else if (flags & 8)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 132, 6);
    }
    else if (r_s16(game + 908) == 59)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 132, 8);
    }
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 0);
    return state;
}

uint32 sub_800a6ed8(uint32 matrix, uint32 vector)
{
    uint32 x, y, z;

    FUNCTION_MARKER(0x800A6ED8, "ACE3.BIN");
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    w_u32(matrix + 20, x);
    w_u32(matrix + 24, y);
    w_u32(matrix + 28, z);
    return matrix;
}

uint32 sub_800a6748(uint32 left, uint32 right, uint32 output)
{
    uint32 matrix[5], first_column[3], second_column[3];
    uint32 third_x, third_y, first, second, third, index;
    uint32 translation[3], origin[3], result[3];

    FUNCTION_MARKER(0x800A6748, "ACE3.BIN");
    for (index = 0; index < 5; ++index)
        matrix[index] = r_u32(left + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, matrix[index]);
    first = r_u16(right);
    second = r_u32(right + 4);
    third = r_u32(right + 12);
    xport_gte_write_data(0, first | (second & 0xFFFF0000));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    first = r_u16(right + 2);
    second = r_u32(right + 8);
    third = (uint32)r_s16(right + 14);
    for (index = 0; index < 3; ++index)
        first_column[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(0, first | (second << 16));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    first = r_u16(right + 4);
    second = r_u32(right + 8);
    third = r_u32(right + 16);
    for (index = 0; index < 3; ++index)
        second_column[index] = xport_gte_read_data(9 + index);
    xport_gte_write_data(0, first | (second & 0xFFFF0000));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    w_u32(output, (first_column[0] & 0xFFFF) | (second_column[0] << 16));
    w_u32(output + 12, (first_column[2] & 0xFFFF) | (second_column[2] << 16));
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    w_u32(output + 16, xport_gte_read_data(11));
    first = r_u16(right + 20);
    second = r_u32(right + 24);
    third = r_u32(right + 28);
    xport_gte_write_data(0, first | (second << 16));
    xport_gte_write_data(1, third);
    xport_gte_execute(0x486012);
    w_u32(output + 4, (third_x & 0xFFFF) | (first_column[1] << 16));
    w_u32(output + 8, (second_column[1] & 0xFFFF) | (third_y << 16));
    for (index = 0; index < 3; ++index)
        translation[index] = xport_gte_read_data(25 + index);
    for (index = 0; index < 3; ++index)
        origin[index] = r_u32(left + 20 + index * 4);
    for (index = 0; index < 3; ++index)
    {
        result[index] = translation[index] + origin[index];
        if ((translation[index] ^ result[index]) & (origin[index] ^ result[index]) & 0x80000000)
        {
#ifndef NDEBUG
            fprintf(stderr, "CompMatrix original ADD overflow at axis %u\n", index);
#endif
            abort();
        }
    }
    for (index = 0; index < 3; ++index)
        w_u32(output + 20 + index * 4, result[index]);
    return output;
}

uint32 resource002_sub_80100b94(uint32 effect, uint32 scratch, uint32 packet)
{
    uint32 ordering, index, pointer, value, mode, display, depth, size;
    uint32 a, b, c, matrix, target, tag;
    sint32 step, number, counter, x, y, right, bottom;

    FUNCTION_MARKER(0x80100B94, "ACE3.BIN");
    step = r_s16(effect + 116);
    ordering = r_u32(effect + 8);
    if (step)
    {
        for (index = 0; index < 48; ++index)
        {
            pointer = r_u32(effect + 220) + index * 2;
            number = r_s16(pointer) + step;
            if (number >= 62)
                number %= 62;
            w_u16(pointer, number);
        }
    }
    display = r_u32(0x800BCE48);
    xport_gte_write_control(21, r_u8(display + 16) << 4);
    xport_gte_write_control(22, r_u8(display + 17) << 4);
    xport_gte_write_control(23, r_u8(display + 18) << 4);
    xport_gte_write_data(6, 0x2CB4B4B4);
    for (index = 0; index < 48; ++index)
    {
        mode = r_u32(effect + 24);
        if (mode == 1)
        {
            pointer = r_u32(effect + 220);
            number = r_s16(pointer + index * 2);
            counter = r_s32(effect + 36);
            if (counter < number)
                continue;
        }
        if (mode == 3)
        {
            pointer = r_u32(effect + 220);
            counter = r_s32(effect + 36);
            number = r_s16(pointer + index * 2);
            if (number < counter)
                continue;
        }
        value = r_u32(0x80112DA8);
        RotMatrix((SVECTOR *)psx_addr(value + 136, 6), (MATRIX *)psx_addr(scratch + 68, 32));
        value = r_u32(0x80112DA8);
        pointer = r_u32(value + 224);
        sub_800a6ed8(scratch + 68, pointer + index * 16);
        pointer = r_u32(effect + 220);
        number = r_s16(pointer + index * 2);
        pointer = r_u32(effect + 128);
        RotMatrix((SVECTOR *)psx_addr(pointer + (uint32)number * 8, 6), (MATRIX *)psx_addr(scratch + 100, 32));
        pointer = r_u32(effect + 132);
        sub_800a6ed8(scratch + 100, pointer + (uint32)number * 16);
        sub_800a6748(scratch + 100, scratch + 68, scratch + 4);
        sub_800a6748(effect + 44, scratch + 4, scratch + 36);
        matrix = scratch + 36;
        a = r_u32(matrix);
        b = r_u32(matrix + 4);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        a = r_u32(matrix + 8);
        b = r_u32(matrix + 12);
        c = r_u32(matrix + 16);
        xport_gte_write_control(2, a);
        xport_gte_write_control(3, b);
        xport_gte_write_control(4, c);
        a = r_u32(matrix + 20);
        b = r_u32(matrix + 24);
        xport_gte_write_control(5, a);
        c = r_u32(matrix + 28);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
        xport_gte_write_data(0, 0);
        xport_gte_write_data(1, 0);
        xport_gte_execute(0x180001);
        w_u32(packet + 20, 0x000FE02F);
        w_u16(packet + 28, 0xEF20);
        w_u32(packet + 12, 0x343FE020);
        w_u16(packet + 36, 0xEF2F);
        depth = xport_gte_read_data(19);
        w_u32(scratch + 160, xport_gte_read_data(14));
        if ((sint32)depth <= 0)
            continue;
        xport_gte_execute(0x780010);
        size = 64000 / (sint32)depth;
        if ((sint32)size < 2)
            size = 2;
        number = r_s16(scratch + 160);
        w_u32(scratch, (sint32)depth >> 8);
        x = (sint32)((uint32)number - (uint32)((sint32)size >> 2));
        number = r_s16(scratch + 162);
        right = (sint32)((uint32)x + (uint32)((sint32)size >> 1));
        y = (sint32)((uint32)number - (uint32)((sint32)size >> 1));
        if (right < 0 || x >= 321)
            continue;
        bottom = (sint32)((uint32)y + size);
        if (bottom < 0 || y >= 481)
            continue;
        value = (uint32)y << 16;
        w_u32(packet + 16, value | (uint32)right);
        w_u32(packet + 8, value | (uint32)x);
        value = (uint32)bottom << 16;
        w_u32(packet + 24, value | (uint32)x);
        w_u32(packet + 32, value | (uint32)right);
        w_u32(packet + 4, xport_gte_read_data(22));
        value = r_u32(scratch);
        if ((sint32)value >= 256)
            value = 255;
        target = ordering + value * 4;
        tag = r_u32(target);
        w_u32(packet, tag | 0x09000000);
        w_u32(target, packet & 0xFFFFFF);
        packet += 40;
    }
    mode = r_u32(effect + 24);
    if (mode == 1)
    {
        counter = r_s32(effect + 36);
        if (counter >= 64)
        {
            w_u32(effect + 36, 0);
            w_u32(effect + 24, 2);
            mode = r_u32(effect + 24);
        }
    }
    if (mode == 3)
    {
        counter = r_s32(effect + 36);
        if (counter >= 64)
        {
            w_u32(effect + 36, 0);
            w_u32(effect + 24, 0);
        }
    }
    return packet;
}
