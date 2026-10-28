#include "game_functions.h"
#include <stdlib.h>

uint32 resource002_sub_800daa6c(void)
{
    FUNCTION_MARKER(0x800DAA6C, "ACE3.BIN");
    return resource002_sub_800daa74(r_u32(0x800BCE60));
}

uint32 resource002_sub_800daa74(uint32 game)
{
    uint32 flag_address, level0, level1, enabled, context, next, state, value, scaled, packet, ordering, result;
    uint32 flags, selection, address, changed, action, index, product;
    sint32 mode = 1, high, quotient;
    FUNCTION_MARKER(0x800DAA74, "ACE3.BIN");
    flag_address = game + 922;
    level0 = game + 920;
    value = r_u8(flag_address);
    enabled = value & 1;
    level1 = game + 921;
    if (!enabled)
    {
        sub_8002bd28(0);
        sub_8002bd5c();
        value = r_u8(r_u32(0x80112DDC) + 256) != 0;
    }
    else
        value = 2;
    resource002_sub_800def94(6, value);
    if (enabled == 1)
    {
        sub_8002bd28(1);
        sub_8002bd5c();
        value = r_u8(r_u32(0x80112DDC) + 256) != 0;
    }
    else
        value = 2;
    resource002_sub_800def94(7, value);
    resource002_sub_800def94(50, 0);
    value = r_u8(level0);
    scaled = value * 221 / 255;
    context = r_u32(0x80112DDC);
    w_u16(context + 300, 194);
    w_u8(context + 308, 0);
    w_u8(context + 307, 0);
    w_u8(context + 306, 0);
    next = r_u32(0x80112DDC);
    w_u16(context + 298, scaled + 55);
    w_u16(next + 302, 2);
    w_u16(next + 304, 64);
    w_u8(next + 309, 0);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8) + 8;
    packet = r_u32(context + 8);
    result = resource002_sub_800f7180(context + 298, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 256);
    w_u32(context + 8, result);
    w_u16(context + 298, 56);
    w_u16(context + 300, 194);
    if (selection == 1)
    {
        w_u8(context + 306, 207);
        w_u8(r_u32(0x80112DDC) + 307, 139);
        w_u8(r_u32(0x80112DDC) + 308, 23);
        resource002_sub_800def94(50, 1);
    }
    else
    {
        w_u8(context + 306, 229);
        w_u8(r_u32(0x80112DDC) + 307, 229);
        w_u8(r_u32(0x80112DDC) + 308, 213);
    }
    value = r_u8(level0);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    w_u16(context + 304, 64);
    w_u16(context + 302, value * 221 / 255);
    ordering = r_u32(state + 8) + 8;
    packet = r_u32(context + 8);
    result = resource002_sub_800f7180(context + 298, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    w_u32(context + 8, result);
    value = r_u8(level1);
    scaled = value * 221 / 255;
    w_u8(context + 308, 0);
    w_u8(context + 307, 0);
    w_u8(context + 306, 0);
    next = r_u32(0x80112DDC);
    w_u16(context + 300, 268);
    state = r_u32(0x80112DC4);
    w_u16(context + 298, scaled + 55);
    w_u16(next + 302, 2);
    w_u16(next + 304, 64);
    ordering = r_u32(state + 8) + 8;
    packet = r_u32(next + 8);
    result = resource002_sub_800f7180(next + 298, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    selection = r_u8(context + 256);
    w_u32(context + 8, result);
    w_u16(context + 298, 56);
    w_u16(context + 300, 268);
    if (selection == 2)
    {
        w_u8(context + 306, 207);
        w_u8(r_u32(0x80112DDC) + 307, 139);
        w_u8(r_u32(0x80112DDC) + 308, 23);
        resource002_sub_800def94(50, 2);
    }
    else
    {
        w_u8(context + 306, 229);
        w_u8(r_u32(0x80112DDC) + 307, 229);
        w_u8(r_u32(0x80112DDC) + 308, 213);
    }
    value = r_u8(level1);
    state = r_u32(0x80112DC4);
    context = r_u32(0x80112DDC);
    w_u16(context + 304, 64);
    w_u16(context + 302, value * 221 / 255);
    ordering = r_u32(state + 8) + 8;
    packet = r_u32(context + 8);
    result = resource002_sub_800f7180(context + 298, packet, ordering, NULL);
    w_u32(r_u32(0x80112DDC) + 8, result);
    result = resource002_sub_800f77a4();
    product = result * 15;
    high = (sint32)(((sint64)(sint32)product * (sint32)0x80010003u) >> 32);
    context = r_u32(0x80112DDC);
    w_u16(context + 298, 50);
    w_u16(context + 300, 189);
    w_u16(context + 304, 12);
    w_u8(context + 306, 207);
    quotient = ((sint32)((uint32)high + product) >> 14) - ((sint32)product >> 31);
    w_u16(context + 302, (uint32)quotient * 7);
    w_u8(r_u32(0x80112DDC) + 307, 139);
    w_u8(r_u32(0x80112DDC) + 308, 23);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8) + 4;
    packet = r_u32(context + 8);
    result = resource002_sub_800f7180(context + 298, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    game = r_u32(0x800BCE60);
    w_u32(context + 8, result);
    if (r_u32(game + 304) & 2)
    {
        value = r_u8(context + 256);
        w_u8(context + 256, value < 2 ? value + 1 : 0);
        sub_8002db4c(61);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        context = r_u32(0x80112DDC);
        value = r_u8(context + 256);
        w_u8(context + 256, value ? value - 1 : 2);
        sub_8002db4c(61);
    }
    resource002_sub_800def94(5, 0);
    flags = r_u32(r_u32(0x800BCE60) + 296);
    if (flags & 1)
        resource002_sub_800def94(5, 1);
    else if (flags & 2)
        resource002_sub_800def94(5, 2);
    for (action = 0; action < 2; ++action)
    {
        if (r_u32(r_u32(0x800BCE60) + 304) & (action ? 4 : 8))
        {
            selection = r_u8(r_u32(0x80112DDC) + 256);
            if (selection == 0)
            {
                if (enabled)
                    w_u8(flag_address, 0);
                else
                {
                    value = r_u8(flag_address);
                    w_u8(flag_address, value | 1);
                }
                sub_8002db4c(61);
            }
            else if (selection == 1 || selection == 2)
            {
                address = selection == 1 ? level0 : level1;
                value = r_u8(address);
                changed = 0;
                if (action == 0 && value != 255)
                {
                    w_u8(address, value < 240 ? value + 16 : 255);
                    changed = 1;
                }
                else if (action == 1 && value != 0)
                {
                    w_u8(address, value >= 17 ? value - 16 : 0);
                    changed = 1;
                }
                if (changed)
                    sub_8002db4c(46);
            }
        }
    }
    for (action = 0; action < 2; ++action)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & (action ? 32 : 16)) && !(r_u32(game + 296) & (action ? 0x5F : 0x2F)))
        {
            if (!action)
            {
                value = r_u8(game + 922);
                context = r_u32(0x80112DDC);
                w_u8(context + 257, value & 1);
                game = r_u32(0x800BCE60);
                context = r_u32(0x80112DDC);
                value = r_u8(game + 920);
                w_u8(context + 258, value);
                game = r_u32(0x800BCE60);
                context = r_u32(0x80112DDC);
                value = r_u8(game + 921);
                w_u8(context + 259, value);
            }
            else
            {
                value = r_u8(r_u32(0x80112DDC) + 257);
                flags = r_u8(game + 922);
                w_u8(game + 922, value == 1 ? flags | 1 : flags & 0xFE);
                context = r_u32(0x80112DDC);
                game = r_u32(0x800BCE60);
                value = r_u8(context + 258);
                w_u8(game + 920, value);
                context = r_u32(0x80112DDC);
                game = r_u32(0x800BCE60);
                value = r_u8(context + 259);
                w_u8(game + 921, value);
            }
            resource002_sub_800dee08(1, 1, 0, 30);
            w_u8(r_u32(0x80112DDC) + 1, 1);
            resource002_sub_800dedf0(2, 4096);
            sub_8002db4c(action ? 59 : 60);
            value = r_u8(r_u32(0x80112DDC) + 6);
            resource002_sub_800def94(5, value);
            resource002_sub_800def94(6, 0);
            for (index = 0; index < 5; ++index)
            {
                result = sub_8002db4c(93 + index);
                w_u32(r_u32(0x80112DC4) + 304 + index * 4, result);
            }
            mode = 3;
        }
    }
    return resource002_sub_800f7524(mode, 0);
}

uint32 resource002_sub_800f7180(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, index, tag, link;
    FUNCTION_MARKER(0x800F7180, "ACE3.BIN");
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    value = native_descriptor ? native_descriptor[11] : r_u8(descriptor + 11);
    w_u8(packet + 7, value ? 0x62 : 0x60);
    value = native_descriptor ? native_descriptor[8] : r_u8(descriptor + 8);
    w_u8(packet + 4, value);
    value = native_descriptor ? native_descriptor[9] : r_u8(descriptor + 9);
    w_u8(packet + 5, value);
    value = native_descriptor ? native_descriptor[10] : r_u8(descriptor + 10);
    w_u8(packet + 6, value);
    for (index = 0; index < 6; index += 2)
    {
        value = native_descriptor ? xport_load_le16(native_descriptor + index) : r_u16(descriptor + index);
        w_u16(packet + index + 8, value);
    }
    value = native_descriptor ? xport_load_le16(native_descriptor + 6) : r_u16(descriptor + 6);
    tag = r_u32(packet);
    w_u16(packet + 14, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 16;
}

uint32 resource002_sub_800f77a4(void)
{
    uint32 left, right, index, left_sum = 0, right_sum = 0;
    sint32 first, second;
    FUNCTION_MARKER(0x800F77A4, "ACE3.BIN");
    left = resource002_sub_800f770c(0);
    right = resource002_sub_800f770c(1);
    for (index = 0; index < 100; ++index)
    {
        first = r_s16(left + index * 2);
        second = r_s16(right + index * 2);
        left_sum += (uint32)(first < 0 ? -first : first);
        right_sum += (uint32)(second < 0 ? -second : second);
    }
    return (uint32)(sint32)(sint16)((left_sum / 100 + right_sum / 100) / 2);
}

uint32 resource002_sub_800f770c(uint32 channel)
{
    uint32 bank, state, left, right;
    FUNCTION_MARKER(0x800F770C, "ACE3.BIN");
    state = r_u32(0x80112E0C);
    bank = (uint32)game_spu_read_decoded_native(state + 24, 5);
    SpuIsTransferCompleted(1);
    state = r_u32(0x80112E0C);
    left = state + (bank ? 24 : 224);
    right = state + (bank ? 1048 : 1248);
    if (channel == 0)
        return left;
    if (channel == 1)
        return right;
    return 1;
}

uint32 resource002_sub_800db3f8(void)
{
    FUNCTION_MARKER(0x800DB3F8, "ACE3.BIN");
    return resource002_sub_800db400(r_u32(0x800BCE60));
}

uint32 resource002_sub_800db400(uint32 game)
{
    static const uint8 starts[] = {72, 83, 99};
    static const uint8 colors[3][6] = {{0, 255, 0, 0, 255, 0}, {0, 0, 255, 0, 0, 255}, {255, 255, 255, 255, 255, 255}};
    uint32 context, next, state, packet, ordering, result, index, x;
    FUNCTION_MARKER(0x800DB400, "ACE3.BIN");
    if (r_u32(game + 300) & 0x30)
    {
        resource002_sub_800dee08(1, 1, 0, 30);
        w_u8(r_u32(0x80112DDC) + 1, 1);
        resource002_sub_800dedf0(2, 4096);
        sub_8002db4c(59);
    }
    w_u8(r_u32(0x80112DDC) + 285, 0);
    context = r_u32(0x80112DDC);
    w_u16(context + 266, 60);
    w_u16(context + 274, 60);
    w_u8(context + 282, 0);
    next = r_u32(0x80112DDC);
    w_u16(context + 268, 152);
    w_u16(context + 270, 65);
    w_u16(context + 272, 152);
    w_u16(context + 276, 320);
    w_u16(context + 278, 65);
    w_u16(context + 280, 320);
    w_u8(next + 283, 0);
    w_u8(r_u32(0x80112DDC) + 284, 0);
    w_u8(r_u32(0x80112DDC) + 286, 0);
    w_u8(r_u32(0x80112DDC) + 287, 0);
    w_u8(r_u32(0x80112DDC) + 288, 0);
    w_u8(r_u32(0x80112DDC) + 290, 255);
    w_u8(r_u32(0x80112DDC) + 291, 0);
    w_u8(r_u32(0x80112DDC) + 292, 0);
    w_u8(r_u32(0x80112DDC) + 294, 255);
    w_u8(r_u32(0x80112DDC) + 295, 0);
    w_u8(r_u32(0x80112DDC) + 296, 0);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8) + 4;
    packet = r_u32(context + 8);
    result = resource002_sub_800f7244(context + 266, packet, ordering, NULL);
    for (index = 0; index < 3; ++index)
    {
        context = r_u32(0x80112DDC);
        w_u8(context + 290, colors[index][0]);
        next = r_u32(0x80112DDC);
        w_u32(context + 8, result);
        x = starts[index];
        w_u16(context + 266, x);
        w_u16(context + 268, 152);
        w_u16(context + 270, x + 5);
        w_u16(context + 272, 152);
        w_u16(context + 274, x);
        w_u16(context + 276, 320);
        w_u16(context + 278, x + 5);
        w_u16(context + 280, 320);
        w_u8(next + 291, colors[index][1]);
        w_u8(r_u32(0x80112DDC) + 292, colors[index][2]);
        w_u8(r_u32(0x80112DDC) + 294, colors[index][3]);
        w_u8(r_u32(0x80112DDC) + 295, colors[index][4]);
        w_u8(r_u32(0x80112DDC) + 296, colors[index][5]);
        context = r_u32(0x80112DDC);
        state = r_u32(0x80112DC4);
        ordering = r_u32(state + 8) + 4;
        packet = r_u32(context + 8);
        result = resource002_sub_800f7244(context + 266, packet, ordering, NULL);
    }
    w_u32(r_u32(0x80112DDC) + 8, result);
    return result;
}

uint32 resource002_sub_800db714(void)
{
    FUNCTION_MARKER(0x800DB714, "ACE3.BIN");
    return resource002_sub_800db71c(r_u32(0x800BCE60));
}

uint32 resource002_sub_800db71c(uint32 game)
{
    static const uint8 bits[] = {8, 4, 2, 1};
    static const uint8 offsets[] = {8, 8, 12, 12};
    static const uint8 slots[] = {7, 9, 8, 6};
    uint32 context, config, value, index, moved = 0, changed, result;
    sint32 position;
    FUNCTION_MARKER(0x800DB71C, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        if (index)
            game = r_u32(0x800BCE60);
        changed = 0;
        if (r_u32(game + 304) & bits[index])
        {
            config = r_u32(0x800BCE48);
            position = r_s32(config + offsets[index]);
            if ((index == 0 && position < 12) || (index == 1 && position >= -11) || (index == 2 && position < 8) || (index == 3 && position > 0))
            {
                value = (uint32)position + (index & 1 ? 0xFFFFFFFFu : 1u);
                w_u32(config + offsets[index], value);
                changed = 1;
            }
        }
        resource002_sub_800def94(slots[index], changed);
        if (changed)
        {
            if (!moved)
                sub_8002db4c(46);
            moved = 1;
        }
    }
    if (sub_800a8ef0())
    {
        config = r_u32(0x800BCE48);
        value = r_u32(config);
        w_u32(config, value | 2);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        resource002_sub_800dee08(1, 1, 0, 30);
        w_u8(r_u32(0x80112DDC) + 1, 1);
        resource002_sub_800dedf0(0, 3);
        resource002_sub_800dedf0(2, 4096);
        config = r_u32(0x800BCE48);
        value = r_u32(config);
        w_u32(config, value | 2);
        return sub_8002db4c(60);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        context = r_u32(0x80112DDC);
        config = r_u32(0x800BCE48);
        value = (uint32)r_s16(context + 262);
        w_u32(config + 8, value);
        value = (uint32)r_s16(context + 264);
        w_u32(config + 12, value);
        resource002_sub_800dee08(1, 1, 0, 30);
        w_u8(r_u32(0x80112DDC) + 1, 1);
        resource002_sub_800dedf0(0, 3);
        resource002_sub_800dedf0(2, 4096);
        config = r_u32(0x800BCE48);
        value = r_u32(config);
        w_u32(config, value | 2);
        sub_8002db4c(59);
        result = r_u32(0x800BCE60);
        w_u32(result + 300, 0);
        return result;
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 292) & 16)
    {
        if (r_s16(game + 104) >= 129)
        {
            config = r_u32(0x800BCE48);
            w_u32(config + 8, 0xFFFFFFFE);
            w_u32(config + 12, 0);
        }
        result = r_u32(r_u32(0x800BCE60) + 292) & 16;
        if (result)
            return result;
    }
    result = r_u16(r_u32(0x800BCE60) + 96) & 0x1000;
    if (result)
    {
        config = r_u32(0x800BCE48);
        w_u32(config + 8, 0xFFFFFFFE);
        w_u32(config + 12, 0);
        return sub_8002db4c(48);
    }
    return result;
}

uint32 resource002_sub_800dbaec(uint32 context)
{
    FUNCTION_MARKER(0x800DBAEC, "ACE3.BIN");
    return resource002_sub_800dbaf4(r_u32(0x800BCE60), context);
}

uint32 resource002_sub_800dbaf4(uint32 game, uint32 context)
{
    uint32 flags, value, phase, state, object, index;
    FUNCTION_MARKER(0x800DBAF4, "ACE3.BIN");
    value = r_u8(game + 924);
    w_u8(context, value != 1);
    flags = r_u32(r_u32(0x800BCE60) + 288);
    w_u8(context + 1, flags & 1 ? (flags & 2 ? 0 : 1) : 2);
    value = r_u8(r_u32(0x800BCE60) + 2394);
    if (value == 0 || value == 1)
        w_u8(context + 2, value);
    w_u8(context + 3, 0);
    resource002_sub_800deca8(2, 16);
    resource002_sub_800dedf0(2, 65536);
    phase = r_u8(context + 3);
    if (phase == 0)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        value = r_u8(context);
        w_u8(object + 50, value);
    }
    else if (phase == 1 || phase == 2)
    {
        state = r_u32(0x80112DC0);
        value = r_u8(context + phase);
        object = r_u32(state + 92);
        w_u8(object + 50, value + (phase == 1 ? 2 : 5));
    }
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 51, 0);
    for (index = 0; index < 3; ++index)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        value = r_u8(context + index);
        w_u8(object + 52 + index, value);
    }
    state = r_u32(0x80112DC0);
    value = r_u8(context + 3);
    object = r_u32(state + 92);
    w_u8(object + 55, value);
    return 1;
}

uint32 resource002_sub_800dbc94(uint32 context)
{
    FUNCTION_MARKER(0x800DBC94, "ACE3.BIN");
    return resource002_sub_800dbc9c(r_u32(0x800BCE60), context);
}

uint32 resource002_sub_800dbc9c(uint32 game, uint32 context)
{
    uint32 flags, phase, value, state, object, index;
    FUNCTION_MARKER(0x800DBC9C, "ACE3.BIN");
    flags = r_u32(game + 304);
    if (flags & 15)
    {
        sub_8002db4c(61);
        if (flags & 2)
            w_u8(context + 3, (r_u8(context + 3) + 1) % 3);
        if (flags & 1)
            w_u8(context + 3, (r_u8(context + 3) + 2) % 3);
        if (flags & 12)
        {
            phase = r_u8(context + 3);
            if (phase == 0)
                w_u8(context, r_u8(context) ^ 1);
            else if (phase == 1)
            {
                value = r_u8(context + 1);
                w_u8(context + 1, (value + (flags & 8 ? 1 : 2)) % 3);
            }
            else if (phase == 2)
                w_u8(context + 2, r_u8(context + 2) == 0);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        sub_8002db4c(60);
        return 1;
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        sub_8002db4c(59);
        w_u32(r_u32(0x800BCE60) + 300, 0);
        return 2;
    }
    phase = r_u8(context + 3);
    if (phase == 0)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        value = r_u8(context);
        w_u8(object + 50, value);
    }
    else if (phase == 1 || phase == 2)
    {
        state = r_u32(0x80112DC0);
        value = r_u8(context + phase);
        object = r_u32(state + 92);
        w_u8(object + 50, value + (phase == 1 ? 2 : 5));
    }
    flags = r_u32(r_u32(0x800BCE60) + 296);
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 51, flags & 1 ? 1 : (flags & 2 ? 2 : 0));
    for (index = 0; index < 3; ++index)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        value = r_u8(context + index);
        w_u8(object + 52 + index, value);
    }
    state = r_u32(0x80112DC0);
    value = r_u8(context + 3);
    object = r_u32(state + 92);
    w_u8(object + 55, value);
    return 0;
}

uint32 resource002_sub_800dbfc4(uint32 context)
{
    uint32 value, game, flags;
    FUNCTION_MARKER(0x800DBFC4, "ACE3.BIN");
    value = r_u8(context);
    game = r_u32(0x800BCE60);
    w_u8(game + 924, value == 0);
    value = r_u8(context + 1);
    if (value == 0 || value == 1 || value == 2)
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 288);
        if (value == 2)
            flags &= 0xFFFFFFFCu;
        else if (value == 1)
            flags = (flags | 1) & 0xFFFFFFFDu;
        else
            flags |= 3;
        w_u32(game + 288, flags);
    }
    value = r_u8(context + 2);
    if (value == 0 || value == 1)
        w_u8(r_u32(0x800BCE60) + 2394, value);
    return 2;
}

uint32 resource002_sub_800dc0a0(void)
{
    uint32 context, target, value, result;
    FUNCTION_MARKER(0x800DC0A0, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    value = r_u32(context + 320);
    target = r_u32(0x8010ACAC + value * 4);
    result = resource002_detail_phase_call(target, context + 314);
    if (result == 1)
    {
        context = r_u32(0x80112DDC);
        value = r_u32(context + 320);
        w_u32(context + 320, value + 1);
    }
    if (result == 2)
    {
        context = r_u32(0x80112DDC);
        w_u8(context + 1, 1);
        w_u32(context + 320, 0);
        resource002_sub_800dee08(1, 1, 0, 30);
        return resource002_sub_800dedf0(2, 4096);
    }
    return 2;
}

uint32 resource002_sub_800dc144(void)
{
    uint32 game, state, resource, directory, statistics, list, count, value, index, context, object, selected;
    FUNCTION_MARKER(0x800DC144, "ACE3.BIN");
    directory = r_u32(0x800BCE50);
    game = r_u32(0x800BCE60);
    resource = r_u32(directory + 96);
    state = r_u32(0x80112DC4);
    statistics = game + 2396;
    w_u32(state + 40, resource);
    directory = sub_80027338(resource, NULL);
    resource002_sub_800ecda8(r_u32(directory + 4));
    state = r_u32(0x80112DC4);
    resource002_sub_800ecda8(r_u32(r_u32(state + 24) + 24));
    arena_set_cursor(r_u32(directory + 4));
    resource = sub_80027338(r_u32(directory), NULL);
    w_u32(r_u32(0x80112DC4) + 476, resource);
    list = arena_alloc(64);
    w_u32(0x80112DBC, list);
    w_u8(list + 60, 0);
    for (index = 0; index < 59; ++index)
    {
        game = r_u32(0x800BCE60);
        if (r_u8(game + 3340 + index))
        {
            list = r_u32(0x80112DBC);
            count = r_u8(list + 60);
            w_u8(list + count, index);
            list = r_u32(0x80112DBC);
            count = r_u8(list + 60);
            w_u8(list + 60, count + 1);
        }
    }
    list = r_u32(0x80112DBC);
    game = r_u32(0x800BCE60);
    count = r_u8(list + 60);
    value = r_u8(game + 908);
    w_u8(list + count, value);
    list = r_u32(0x80112DBC);
    count = r_u8(list + 60);
    w_u8(list + 60, count + 1);
    w_u8(r_u32(0x80112DBC) + 61, 0);
    for (index = 0; index < 64; ++index)
    {
        if (r_u8(statistics + 24 + index) == 1)
        {
            list = r_u32(0x80112DBC);
            count = r_u8(list + 61);
            w_u8(list + 61, count + 1);
        }
    }
    w_u8(r_u32(0x80112DBC) + 62, 0);
    for (index = 0; index < 16; ++index)
    {
        if (r_u8(statistics + 88 + index) == 1)
        {
            list = r_u32(0x80112DBC);
            count = r_u8(list + 62);
            w_u8(list + 62, count + 1);
        }
    }
    list = r_u32(0x80112DBC);
    count = r_u8(list + 60);
    w_u8(list + 63, count - 1);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    value = resource002_sub_800fb2a0(r_u8(list + selected));
    state = r_u32(0x80112DC4);
    directory = r_u32(state + 476);
    resource002_sub_800ed000(r_u32(directory + value * 4), 960, 256, 768, 1);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    resource002_sub_800deca8(2, 17);
    resource002_sub_800deca8(2, 18);
    resource002_sub_800dedf0(2, 393216);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    state = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    selected = r_u8(list + selected);
    object = r_u32(state + 92);
    value = r_u8(game + 3340 + selected);
    w_u8(object + 50, value);
    state = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    object = r_u32(state + 92);
    value = r_u8(game + 916);
    w_u8(object + 51, value);
    state = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    object = r_u32(state + 92);
    value = r_u8(game + 2397);
    w_u8(object + 53, value);
    return value;
}

uint32 resource002_sub_800dc43c(void)
{
    uint32 list, selected, game, statistics, faction, mode, value, state, object, count, flags, changed = 0, directory;
    uint32 arguments[3];
    sint32 frames = 0;
    FUNCTION_MARKER(0x800DC43C, "ACE3.BIN");
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    game = r_u32(0x800BCE60);
    value = r_u8(list + selected);
    statistics = game + 2396;
    faction = resource002_sub_800fb2a0(value);
    resource002_sub_800f35b8(4);
    resource002_sub_800f33f8(2);
    resource002_sub_800f33d0(128, 128, 128);
    resource002_sub_800f33a8(29, 65);
    resource002_sub_800f4ae0(r_u32(0x800BCE60) + 2376, 0, NULL, 0);
    resource002_sub_800f33f8(0xFFFFFFFFu);
    resource002_sub_800f35b8(8);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    game = r_u32(0x800BCE60);
    mode = r_u32(game + 916);
    selected = r_u8(list + selected);
    if (mode == 0)
        frames = r_u16(statistics + selected * 2 + 104);
    else if (mode == 1)
        frames = r_u16(statistics + selected * 2 + 224);
    else if (mode == 2)
        frames = r_u16(statistics + selected * 2 + 344);
    // Initialize the original undefined value for an invalid mode
    if (frames < 0)
        frames = 0;
    if (frames > 107999999)
        frames = 107999999;
    resource002_sub_800f33a8(234, 152);
    arguments[0] = frames / 1800;
    arguments[1] = frames / 30 - 60 * (frames / 1800);
    arguments[2] = 100 * (frames % 30) / 30;
    resource002_sub_800f4b40(0x800CC800, 0, (const uint8 *)arguments, 12);
    resource002_sub_800f35b8(3);
    resource002_sub_800f33f8(2);
    resource002_sub_800f33a8(180, 110);
    arguments[0] = r_u32(statistics + 1748);
    resource002_sub_800f4ae0(0x800CC80C, 0, (const uint8 *)arguments, 4);
    resource002_sub_800f33a8(180, 128);
    arguments[0] = r_u32(statistics + 1752);
    resource002_sub_800f4ae0(0x800CC80C, 0, (const uint8 *)arguments, 4);
    resource002_sub_800f33a8(180, 147);
    arguments[0] = r_u8(r_u32(0x80112DBC) + 61);
    arguments[1] = 20;
    resource002_sub_800f4ae0(0x800CC810, 0, (const uint8 *)arguments, 8);
    resource002_sub_800f33a8(180, 167);
    arguments[0] = r_u8(r_u32(0x80112DBC) + 62);
    arguments[1] = 13;
    resource002_sub_800f4ae0(0x800CC810, 0, (const uint8 *)arguments, 8);
    resource002_sub_800f33a8(145, 215);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    selected = r_u8(list + selected);
    resource002_sub_800f4ae0(r_u32(0x8010A3BC + selected * 4), 0, NULL, 0);
    resource002_sub_800f33a8(94, 253);
    resource002_sub_800f4ae0(r_u32(0x8010A4B0 + faction * 4), 0, NULL, 0);
    resource002_sub_800f33f8(1);
    resource002_sub_800f33a8(253, 251);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    selected = r_u8(list + selected);
    frames = r_u16(statistics + selected * 2 + 584);
    arguments[0] = frames / 1800;
    arguments[1] = frames / 30 - 60 * (frames / 1800);
    arguments[2] = 100 * (frames % 30) / 30;
    resource002_sub_800f4b40(0x800CC818, 0, (const uint8 *)arguments, 12);
    resource002_sub_800f33a8(175, 282);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    selected = r_u8(list + selected);
    arguments[0] = r_u32(statistics + selected * 4 + 1264);
    resource002_sub_800f4ae0(0x800CC80C, 0, (const uint8 *)arguments, 4);
    resource002_sub_800f33a8(175, 303);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    selected = r_u8(list + selected);
    arguments[0] = r_u32(statistics + selected * 4 + 1504);
    resource002_sub_800f4ae0(0x800CC80C, 0, (const uint8 *)arguments, 4);
    resource002_sub_800f35b8(8);
    resource002_sub_800f33f8(2);
    resource002_sub_800f33a8(145, 359);
    frames = r_s32(statistics + 1744);
    if (frames < 0)
        frames = 0;
    if (frames > 107999999)
        frames = 107999999;
    arguments[0] = frames / 108000;
    arguments[1] = frames / 1800 - 60 * (frames / 108000);
    arguments[2] = frames / 30 - 60 * (frames / 1800);
    resource002_sub_800f4b40(0x800CC828, 0, (const uint8 *)arguments, 12);
    list = r_u32(0x80112DBC);
    selected = r_u8(list + 63);
    selected = r_u8(list + selected);
    state = r_u32(0x80112DC0);
    object = r_u32(state + 92);
    value = r_u8(statistics + selected + 944);
    w_u8(object + 50, value);
    state = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    object = r_u32(state + 92);
    value = r_u8(game + 916);
    w_u8(object + 51, value);
    object = r_u32(r_u32(0x80112DC0) + 92);
    value = r_u8(statistics + 1);
    w_u8(object + 53, value);
    list = r_u32(0x80112DBC);
    count = r_u8(list + 60);
    selected = r_u8(list + 63);
    value = 2 * ((sint32)selected < (sint32)count - 1);
    if (selected)
        value |= 1;
    resource002_sub_800def94(49, value);
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 52, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if (flags & 8)
    {
        list = r_u32(0x80112DBC);
        count = r_u8(list + 60);
        selected = r_u8(list + 63);
        if ((sint32)selected < (sint32)count - 1)
        {
            w_u8(list + 63, selected + 1);
            sub_8002db4c(61);
            changed = 1;
        }
    }
    else if (flags & 4)
    {
        list = r_u32(0x80112DBC);
        selected = r_u8(list + 63);
        if (selected)
        {
            w_u8(list + 63, selected - 1);
            sub_8002db4c(61);
            changed = 2;
        }
    }
    else if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        resource002_sub_800debb8(0, 1);
        w_u8(r_u32(0x80112DDC), 0);
        w_u8(r_u32(0x80112DDC) + 1, 0);
        arena_set_cursor(r_u32(r_u32(0x80112DC4) + 40));
        sub_8002db4c(59);
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    if (changed)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 52, changed);
        list = r_u32(0x80112DBC);
        selected = r_u8(list + 63);
        value = resource002_sub_800fb2a0(r_u8(list + selected));
        state = r_u32(0x80112DC4);
        directory = r_u32(state + 476);
        resource002_sub_800ed000(r_u32(directory + value * 4), 960, 256, 768, 1);
    }
    state = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    object = r_u32(state + 92);
    value = r_u8(game + 2397);
    w_u8(object + 53, value);
    return value;
}

uint32 resource002_sub_800dcb5c(void)
{
    FUNCTION_MARKER(0x800DCB5C, "ACE3.BIN");
    return resource002_sub_800dcb64(r_u32(0x80112DDC));
}

uint32 resource002_sub_800dcb64(uint32 context)
{
    uint32 value, state, object, game;
    FUNCTION_MARKER(0x800DCB64, "ACE3.BIN");
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    state = r_u32(0x80112DC4);
    object = r_u32(state + 364);
    w_u16(object + 190, 1);
    game = r_u32(0x800BCE60);
    w_u16(state + 132, 6);
    w_u16(state + 134, 0);
    value = r_u32(game + 2412);
    w_u32(state + 224, value & 16 ? 3 : 1);
    return resource002_sub_800dedf0(0, 32768);
}

uint32 resource002_sub_800dcbe0(void)
{
    uint32 state, target, context, value;
    FUNCTION_MARKER(0x800DCBE0, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    target = r_u32(0x8010B358 + (uint32)r_s16(state + 134) * 4);
    resource002_mode_phase_call(target, state);
    state = r_u32(0x80112DC4);
    if (r_s16(state + 132) == 7)
    {
        resource002_sub_800dedf0(1, 0);
        state = r_u32(0x80112DC4);
        context = r_u32(0x80112DDC);
        w_u16(state + 132, 0);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        resource002_sub_800dee08(1, 1, 0, 30);
        return resource002_sub_80100fc4();
    }
    return 7;
}

uint32 resource002_sub_800dcc80(void)
{
    static const uint8 factors[] = {143, 135, 83};
    uint32 progress, result, product, index, display, flags;
    sint32 high, quotient;
    FUNCTION_MARKER(0x800DCC80, "ACE3.BIN");
    progress = resource002_sub_800deea8(1);
    result = resource002_sub_800dee84(1);
    if (result == 2)
    {
        resource002_sub_800debb8(0, 1);
        resource002_sub_800decd4(0, 15);
        resource002_sub_800ff2bc();
        display = r_u32(0x80112DDC);
        result = r_u8(display + 1);
        w_u8(display + 1, result + 1);
        return resource002_sub_800dee08(1, 1, 0, 25);
    }
    for (index = 0; index < 3; ++index)
    {
        product = progress * factors[index];
        high = (sint32)(((sint64)(sint32)product * (sint32)0x8D3DCB09u) >> 32);
        quotient = ((sint32)((uint32)high + product) >> 4) - ((sint32)product >> 31);
        display = r_u32(0x800BCE48);
        w_u8(display + 16 + index, (uint32)quotient);
    }
    display = r_u32(0x800BCE48);
    flags = r_u32(display);
    result = flags | 8;
    w_u32(display, result);
    return result;
}

uint32 resource002_sub_800dcdc4(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800DCDC4, "ACE3.BIN");
    result = resource002_sub_800dee84(1);
    if (result == 2)
    {
        w_u8(r_u32(0x80112DDC), 0);
        result = r_u32(0x80112DDC);
        w_u8(result + 1, 0);
    }
    return result;
}

uint32 resource002_sub_80100fc4(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80100FC4, "ACE3.BIN");
    state = r_u32(0x80112DA8);
    w_u32(state + 28, 3);
    w_u32(state + 40, 0);
    return 3;
}

uint32 resource002_sub_80100fac(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80100FAC, "ACE3.BIN");
    state = r_u32(0x80112DA8);
    w_u32(state + 28, 1);
    w_u32(state + 40, 0);
    return 1;
}

uint32 resource002_sub_800eb9ec(void)
{
#if 0
    /* Original save-account and memory-card screen sequence */
    uint32 state, value, index, buffer, game, next;
    FUNCTION_MARKER(0x800EB9EC, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134);
    w_u16(state + 134, value + 1);
    buffer = arena_alloc(28);
    w_u32(0x80112DF8, buffer);
    for (index = 0; index < 2; ++index)
    {
        buffer = arena_alloc(8192);
        state = r_u32(0x80112DF8);
        w_u32(state + index * 4 + 16, buffer);
    }
    game = r_u32(0x800BCE60);
    index = r_u32(game + 4);
    state = r_u32(0x80112DF8);
    buffer = r_u32(state + index * 4 + 16);
    w_u8(state, 0);
    next = r_u32(0x80112DF8);
    w_u32(state + 12, buffer);
    w_u8(next + 1, 0);
    for (index = 2; index < 11; ++index)
        w_u8(r_u32(0x80112DF8) + index, index == 8);
    state = r_u32(0x80112DF8);
    w_u16(state + 24, 0);
    w_u16(state + 26, 0);
    resource002_sub_80100fac();
    resource002_sub_800de908(r_u32(r_u32(0x80112DC4) + 344), 12, 1);
    resource002_sub_800dedf0(0, 0);
    resource002_sub_800dedf0(1, 0);
    resource002_sub_800dedf0(2, 0);
    resource002_sub_800ecda8(r_u32(r_u32(0x80112DC4) + 172));
    state = r_u32(0x80112DC4);
    resource002_sub_800ecda8(r_u32(r_u32(state + 24) + 24));
    resource002_sub_800e3e3c();
    resource002_sub_800e3fcc();
    resource002_sub_800e25d4();
    resource002_sub_800dee08(15, 1, 0, 20);
    value = r_u32(r_u32(0x80112DA8) + 20);
    return resource002_sub_800ff19c(value ? 3 : 0);
#else
    FUNCTION_MARKER(0x800EB9EC, "ACE3.BIN");
    return game_account_save_requested(r_u32(0x80112DC4));
#endif
}

uint32 resource002_sub_800ebbd0(void)
{
    uint32 result, state, display, value;
    FUNCTION_MARKER(0x800EBBD0, "ACE3.BIN");
    result = r_u32(r_u32(0x80112DA8) + 20);
    if (result == 0)
    {
        state = r_u32(0x80112DC4);
        value = r_u16(state + 134);
        display = r_u32(0x800BCE48);
        w_u16(state + 134, value + 1);
        result = r_u8(display + 16) == 143 ? 32 : 1;
        w_u8(r_u32(0x80112DF8) + 7, result);
    }
    return result;
}

uint32 resource002_sub_800ebc3c(void)
{
    static const uint8 factors[] = {143, 135, 83};
    uint32 state, value, display, index, result;
    FUNCTION_MARKER(0x800EBC3C, "ACE3.BIN");
    state = r_u32(0x80112DF8);
    value = r_u8(state + 7) - 1;
    w_u8(state + 7, value);
    if ((value & 255) == 0)
    {
        state = r_u32(0x80112DC4);
        value = r_u16(state + 134);
        w_u16(state + 134, value + 1);
    }
    for (index = 0; index < 3; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 7);
        value *= factors[index];
        display = r_u32(0x800BCE48);
        w_u8(display + 16 + index, value / 32);
    }
    display = r_u32(0x800BCE48);
    result = r_u32(display) | 8;
    w_u32(display, result);
    return result;
}

uint32 resource002_sub_800e25d4(void)
{
    uint32 state, next;
    FUNCTION_MARKER(0x800E25D4, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    w_u8(state + 844, 1);
    next = r_u32(0x80112DF0);
    w_u32(state + 40, 1);
    w_u8(next + 840, 0);
    w_u8(r_u32(0x80112DF0) + 842, 0);
    state = r_u32(0x80112DF0);
    w_u8(state + 848, 0);
    next = r_u32(0x80112DF0);
    w_u16(state + 48, 0);
    w_u8(next + 845, 1);
    w_u8(r_u32(0x80112DF0) + 841, 0);
    w_u8(r_u32(0x80112DF0) + 843, 0);
    state = r_u32(0x80112DF0);
    w_u8(state + 849, 0);
    next = r_u32(0x80112DF0);
    w_u16(state + 50, 0);
    w_u32(next + 852, 0);
    return 1;
}

uint32 resource002_sub_800e3ae8(void)
{
    uint32 state, buffer, index;
    FUNCTION_MARKER(0x800E3AE8, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    buffer = r_u32(state + 744);
    w_u32(buffer + 4, 0x990426);
    w_u8(r_u32(state + 744) + 8, 0);
    w_u8(r_u32(r_u32(0x80112DF0) + 744) + 9, 0);
    for (index = 0; index < 112; ++index)
        w_u8(r_u32(r_u32(0x80112DF0) + 744) + index + 16, 0);
    buffer = r_u32(r_u32(0x80112DF0) + 744);
    w_u32(buffer + 12, 0);
    return buffer;
}

uint32 resource002_sub_800e3b7c(void)
{
    uint32 state, account, slot, name, terminator, index, group, game, target, value, first, second, third, fourth;
    FUNCTION_MARKER(0x800E3B7C, "ACE3.BIN");
    for (slot = 0; slot < 4; ++slot)
    {
        state = r_u32(0x80112DF0);
        account = r_u32(state + (slot < 3 ? 748 + slot * 4 : 760));
        name = r_u32(0x800CCD6C);
        terminator = (uint32)r_s8(0x800CCD70);
        w_u32(account + 8, name);
        w_u8(account + 12, terminator);
        w_u16(account + 20, 0);
        w_u16(account + 22, 0);
        for (index = 0; index < 60; ++index)
        {
            w_u16(account + 394 + index * 2, 0);
            w_u16(account + 274 + index * 2, 0);
            w_u16(account + 154 + index * 2, 0);
            w_u16(account + 34 + index * 2, 0);
            w_u8(account + 514 + index, 0);
            w_u8(account + 544 + index, 0);
            w_u8(account + 574 + index, 0);
            w_u8(account + 604 + index, 0);
        }
        w_u16(account + 704, 0);
        w_u16(account + 706, 0);
        for (group = 0; group < 6; ++group)
        {
            game = r_u32(0x800BCE60);
            target = account + 708 + group * 68;
            for (index = 0; index < 20; ++index)
            {
                value = r_u16(game + 370 + group * 92 + index * 2);
                w_u16(target + index * 2, value);
            }
            game = r_u32(0x800BCE60) + group * 92;
            first = r_u32(game + 412);
            second = r_u32(game + 416);
            third = r_u32(game + 420);
            fourth = r_u32(game + 424);
            w_u32(target + 40, first);
            w_u32(target + 44, second);
            w_u32(target + 48, third);
            w_u32(target + 52, fourth);
            first = r_u32(game + 428);
            second = r_u32(game + 432);
            w_u32(target + 56, first);
            w_u32(target + 60, second);
            value = r_u32(game + 436);
            w_u32(target + 64, value);
        }
        w_u8(account + 1116, 0);
        w_u8(account + 1117, 204);
        w_u8(account + 1118, 204);
        game = r_u32(0x800BCE60);
        w_u16(account + 1122, 0);
        w_u16(account + 1120, 0);
        w_u16(account + 22, 0);
        value = r_u32(game + 288);
        w_u8(account + 1124, (value & 4) == 0);
        game = r_u32(0x800BCE60);
        w_u32(account + 1128, 0);
        if (r_u8(game + 924) & 1)
            w_u32(account + 1128, 1);
        if (r_u32(r_u32(0x800BCE60) + 288) & 1)
        {
            value = r_u32(account + 1128);
            w_u32(account + 1128, value | 2);
        }
        if (r_u32(r_u32(0x800BCE60) + 288) & 2)
        {
            value = r_u32(account + 1128);
            w_u32(account + 1128, value | 4);
        }
        if (r_u8(r_u32(0x800BCE60) + 2394) == 1)
        {
            value = r_u32(account + 1128);
            w_u32(account + 1128, value | 8);
        }
        for (index = 17; index > 0; --index)
            w_u8(account + 1131 + index, 0);
    }
    return 0;
}

void resource002_sub_800e3e34(void)
{
    FUNCTION_MARKER(0x800E3E34, "ACE3.BIN");
}

uint32 resource002_sub_800e3e3c(void)
{
    uint32 state, next, owner, first, second, third, half, tail, index;
    FUNCTION_MARKER(0x800E3E3C, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    w_u16(state + 856, 0);
    w_u16(state + 858, 0);
    w_u16(state + 860, 0);
    resource002_sub_800e3ae8();
    resource002_sub_800e3b7c();
    resource002_sub_800e3e34();
    resource002_sub_800e25d4();
    state = r_u32(0x80112DF0);
    w_u8(state + 822, 0);
    next = r_u32(0x80112DF0);
    w_u32(state + 800, 0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    w_u16(state + 80, 0);
    w_u16(state + 82, 0);
    w_u8(next + 820, 0);
    owner = r_u32(0x80112DF0);
    first = r_u32(0x800CCD74);
    second = r_u32(0x800CCD78);
    third = r_u32(0x800CCD7C);
    w_u32(owner + 824, first);
    w_u32(owner + 828, second);
    w_u32(owner + 832, third);
    half = r_u16(0x800CCD80);
    w_u16(owner + 836, half);
    state = r_u32(0x80112DF0);
    first = r_u32(0x800CCD84);
    second = r_u32(0x800CCD88);
    third = r_u32(0x800CCD8C);
    w_u32(state + 692, first);
    w_u32(state + 696, second);
    w_u32(state + 700, third);
    first = r_u32(0x800CCD90);
    second = r_u32(0x800CCD94);
    half = r_u16(0x800CCD98);
    w_u32(state + 704, first);
    w_u32(state + 708, second);
    w_u16(state + 712, half);
    tail = r_u8(0x800CCD9A);
    w_u8(state + 714, tail);
    state = r_u32(0x80112DF0);
    first = r_u32(0x800CCD9C);
    second = r_u32(0x800CCDA0);
    third = r_u32(0x800CCDA4);
    w_u32(state + 718, first);
    w_u32(state + 722, second);
    w_u32(state + 726, third);
    first = r_u32(0x800CCDA8);
    second = r_u32(0x800CCDAC);
    half = r_u16(0x800CCDB0);
    w_u32(state + 730, first);
    w_u32(state + 734, second);
    w_u16(state + 738, half);
    tail = r_u8(0x800CCDB2);
    w_u8(state + 740, tail);
    for (index = 0; index < 2; ++index)
    {
        w_u16(owner + 44 + index * 2, 2);
        w_u16(owner + 48 + index * 2, 2);
    }
    return 0;
}

void resource002_sub_800e3fcc(void)
{
    static const uint32 classes[] = {0xF4000001u, 0xF4000001u, 0xF4000001u, 0xF4000001u, 0xF0000011u, 0xF0000011u, 0xF0000011u};
    static const uint16 specifications[] = {4, 32768, 256, 8192, 4, 32768, 256};
    uint32 handle, index;
    FUNCTION_MARKER(0x800E3FCC, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
    {
        handle = (uint32)OpenEvent(classes[index], specifications[index], 8192, NULL);
        w_u32(r_u32(0x80112DF0) + 4 + index * 4, handle);
    }
    for (index = 0; index < 7; ++index)
        EnableEvent((sint32)r_u32(r_u32(0x80112DF0) + 4 + index * 4));
    bios_exit_critical();
}

uint32 resource002_sub_800ebd48(void)
{
    FUNCTION_MARKER(0x800EBD48, "ACE3.BIN");
    return resource002_sub_800ebd50(r_u32(0x80112DC4));
}

uint32 resource002_sub_800ebd50(uint32 state)
{
    uint32 mode, context, value, object, table, kind, phase, target, packet, ordering, game, result;
    sint32 tick, mission;
    FUNCTION_MARKER(0x800EBD50, "ACE3.BIN");
    mode = r_u32(state + 224);
    if (mode == 0 || mode == 2 || mode == 3)
    {
        state = r_u32(0x80112DC4);
        context = r_u32(0x80112DF8);
        value = r_u32(state + 628);
        w_u32(context + 12, value);
    }
    else if (mode == 1 || mode == 4)
    {
        state = r_u32(0x80112DDC);
        context = r_u32(0x80112DF8);
        value = r_u32(state + 8);
        w_u32(context + 12, value);
    }
    resource002_sub_800e4858();
    value = r_u32(r_u32(0x800BCE60) + 292);
    resource002_sub_800def94(40, (value & 16) != 0);
    context = r_u32(0x80112DF8);
    if (r_u8(context + 8))
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 16, 2);
    }
    else
        resource002_sub_800def94(16, r_u8(context + 4));
    resource002_sub_800def94(17, 0);
    resource002_sub_800f35b8(4);
    resource002_sub_800f33f8(2);
    resource002_sub_800f34c8(1, 10, 20);
    value = resource002_sub_800deea8(15);
    resource002_sub_800f34b0(value);
    resource002_sub_800f33d0(128, 128, 128);
    resource002_sub_800f33a8(50, 73);
    resource002_sub_800f4ae0(r_u32(0x800BCE60) + 2376, 0, NULL, 0);
    context = r_u32(0x80112DF8);
    kind = r_u8(context);
    phase = r_u8(context + 1);
    table = r_u32(0x8010B30C + kind * 4);
    target = r_u32(table + phase * 4);
    resource002_selection_phase_call(target, context);
    context = r_u32(0x80112DF8);
    if (r_u8(context + 9) != 1)
    {
        tick = r_s32(r_u32(0x800BCE60) + 32);
        state = r_u32(0x80112DC4);
        packet = r_u32(context + 12);
        ordering = r_u32(state + 8);
        result = resource002_sub_800e6dfc(227, 343, ordering, (uint32)(tick % 64) << 6, packet);
        w_u32(r_u32(0x80112DF8) + 12, result);
    }
    state = r_u32(0x80112DC4);
    mode = r_u32(state + 224);
    if (mode == 0 || mode == 2 || mode == 3)
    {
        context = r_u32(0x80112DF8);
        state = r_u32(0x80112DC4);
        value = r_u32(context + 12);
        w_u32(state + 628, value);
    }
    else if (mode == 1 || mode == 4)
    {
        context = r_u32(0x80112DF8);
        state = r_u32(0x80112DDC);
        value = r_u32(context + 12);
        w_u32(state + 8, value);
    }
    context = r_u32(0x80112DF8);
    kind = r_u8(context + 2);
    if (kind == 1)
    {
        resource002_sub_800e419c();
        value = r_u32(r_u32(0x80112DC4) + 348);
        resource002_sub_800de908(value, 27, 0);
        context = r_u32(0x80112DF8);
        state = r_u32(0x80112DC4);
        w_u16(state + 132, 7);
        w_u16(state + 134, 0);
        arena_set_cursor(context);
        mode = r_u32(r_u32(0x80112DC4) + 224);
        if (mode == 0)
        {
            mission = r_s16(r_u32(0x800BCE60) + 908);
            if (mission == 20 || mission == 35 || mission == 40 || mission == 45)
            {
                game = r_u32(0x800BCE60);
                w_u16(game + 2388, 2);
                value = r_u32(game + 2412);
                state = r_u32(0x80112DC4);
                w_u32(game + 2412, value | 1);
                w_u32(state + 148, 0x60000003);
            }
            else
            {
                game = r_u32(0x800BCE60);
                value = r_u32(game + 2412);
                w_u32(game + 2412, value & 0xFFFFFFFEu);
            }
        }
        game = r_u32(0x800BCE60);
        result = r_u32(game + 2412) & 0xFFFFFFF7u;
        w_u32(game + 2412, result);
        return result;
    }
    if (kind == 2)
    {
        resource002_sub_800e419c();
        w_u32(r_u32(0x80112DC4) + 148, 0x60000004);
        return 0x60000004;
    }
    return 2;
}

void resource002_sub_800e419c(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800E419C, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
        CloseEvent((sint32)r_u32(r_u32(0x80112DF0) + 4 + index * 4));
    bios_exit_critical();
}

uint32 resource002_sub_800e4858(void)
{
    FUNCTION_MARKER(0x800E4858, "ACE3.BIN");
    return resource002_sub_800e4860(r_u32(0x80112DF0));
}

uint32 resource002_sub_800e4860(uint32 state)
{
    uint32 port, step, target, field, status_base, value, flags, result;
    sint32 status;
    FUNCTION_MARKER(0x800E4860, "ACE3.BIN");
    port = r_u32(state + 852);
    step = r_u8(state + port + 844);
    if (step >= 9)
        return 0x800D0000;
    target = r_u32(0x800CCDEC + step * 4);
    switch (target)
    {
        case 0x800E48B0:
            state = r_u32(0x80112DF0);
            goto toggle_port;
        case 0x800E48C0:
            resource002_sub_800e2874();
            _card_info((sint32)(port << 4));
            w_u8(r_u32(0x80112DF0) + port + 844, 2);
            w_u32(r_u32(0x80112DF0) + 792, 300);
            return 300;
        case 0x800E48F4:
            result = resource002_sub_800e2794(port, 3);
            w_u8(r_u32(0x80112DF0) + port + 846, result);
            state = r_u32(0x80112DF0);
            value = r_u32(state + 792);
            if ((sint32)value < 0)
            {
                w_u8(state + port + 844, 1);
                return 1;
            }
            result = value - 1;
            w_u32(state + 792, result);
            return result;
        case 0x800E4944:
            state = r_u32(0x80112DF0);
            field = state + port;
            status_base = state + port * 2;
            value = r_u8(field + 846);
            w_u16(status_base + 44, value);
            if (value == 0)
            {
                if (r_u8(field + 840) == 1)
                {
                    flags = r_u32(state + 40);
                    w_u8(field + 844, flags != 0);
                    state = r_u32(0x80112DF0);
                    goto toggle_port;
                }
                resource002_sub_800e28e0();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                state = r_u32(0x80112DF0);
                result = state + port;
                w_u8(result + 844, 4);
                return result;
            }
            if (value == 1 || value == 2)
            {
                flags = r_u8(field + 848);
                if (flags >= 17)
                    w_u8(field + 840, 1);
                else
                {
                    w_u8(field + 840, 0);
                    field = r_u32(0x80112DF0) + port;
                    flags = r_u8(field + 848);
                    w_u8(field + 848, flags + 1);
                }
                state = r_u32(0x80112DF0);
                flags = r_u32(state + 40);
                w_u8(state + port + 844, value == 1 ? flags == 1 : flags != 0);
                state = r_u32(0x80112DF0);
                if (value == 1)
                    goto toggle_port;
                flags = r_u32(state + 852);
                result = 1 - flags;
                w_u32(state + 852, result);
                w_u8(state + port + 842, 0);
                return result;
            }
            if (value == 3)
            {
                w_u16(status_base + 44, 0);
                resource002_sub_800e28e0();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                w_u8(r_u32(0x80112DF0) + port + 844, 4);
                w_u8(r_u32(0x80112DF0) + port + 840, 0);
                result = r_u32(0x80112DF0) + port;
                w_u8(result + 848, 0);
                return result;
            }
            w_u8(r_u32(0x80112DF0) + port + 840, 1);
            w_u8(r_u32(0x80112DF0) + port + 844, 1);
            state = r_u32(0x80112DF0);
            goto toggle_port;
        case 0x800E4B70:
            do
            {
                result = resource002_sub_800e294c(port, 5);
                w_u8(r_u32(0x80112DF0) + port + 846, result);
                result = r_u8(r_u32(0x80112DF0) + port + 846);
            } while (result == 255);
            return result;
        case 0x800E4BB8:
            state = r_u32(0x80112DF0);
            value = r_u8(state + port + 846);
            w_u16(state + port * 2 + 44, value);
            resource002_sub_800e2874();
            psx_bios_card_load_guest(port << 4);
            result = r_u32(0x80112DF0) + port;
            w_u8(result + 844, 6);
            return result;
        case 0x800E4BEC:
            result = resource002_sub_800e2794(port, 7);
            w_u8(r_u32(0x80112DF0) + port + 846, result);
            return result;
        case 0x800E4C10:
            state = r_u32(0x80112DF0);
            field = state + port;
            value = r_u8(field + 846);
            w_u16(state + port * 2 + 48, value);
            w_u8(field + 840, 1);
            state = r_u32(0x80112DF0);
            status = r_s16(state + port * 2 + 48);
            if (status < 0)
                return 1;
            if (status == 0)
            {
                w_u8(state + port + 844, 8);
                return 8;
            }
            if (status <= 2)
            {
                field = state + port;
                value = r_u8(field + 848);
                if (value >= 17)
                    w_u8(field + 840, 1);
                else
                {
                    w_u8(field + 840, 0);
                    field = r_u32(0x80112DF0) + port;
                    value = r_u8(field + 848);
                    w_u8(field + 848, value + 1);
                }
                result = r_u32(0x80112DF0) + port;
                w_u8(result + 844, 1);
                return result;
            }
            if (status != 3)
                return state + port;
            w_u8(state + port + 840, 1);
            w_u8(r_u32(0x80112DF0) + port + 844, 1);
            state = r_u32(0x80112DF0);
            goto toggle_port;
        case 0x800E4D04:
            state = r_u32(0x80112DF0);
            result = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
            state = r_u32(0x80112DF0);
            w_u32(state + 60, result);
            if (result != 0xFFFFFFFFu)
                w_u8(state + port + 842, 1);
            game_bios_close_guest(r_u32(r_u32(0x80112DF0) + 60));
            w_u8(r_u32(0x80112DF0) + port + 844, 1);
            w_u8(r_u32(0x80112DF0) + port + 840, 1);
            state = r_u32(0x80112DF0);
            goto toggle_port;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 card phase %08X\n", target);
#endif
            abort();
    }
toggle_port:
    value = r_u32(state + 852);
    result = 1 - value;
    w_u32(state + 852, result);
    return result;
}

uint32 resource002_sub_800e2794(uint32 port, uint32 phase)
{
    uint32 index, handle;
    FUNCTION_MARKER(0x800E2794, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        handle = r_u32(r_u32(0x80112DF0) + 4 + index * 4);
        if (TestEvent((sint32)handle) == 1)
        {
            w_u8(r_u32(0x80112DF0) + port + 844, phase);
            return index;
        }
    }
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e2874(void)
{
    uint32 index, result = 0;
    FUNCTION_MARKER(0x800E2874, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        result = (uint32)TestEvent((sint32)r_u32(r_u32(0x80112DF0) + 4 + index * 4));
    return result;
}

uint32 resource002_sub_800e28e0(void)
{
    uint32 index, result = 0;
    FUNCTION_MARKER(0x800E28E0, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        result = (uint32)TestEvent((sint32)r_u32(r_u32(0x80112DF0) + 20 + index * 4));
    return result;
}

uint32 resource002_sub_800e294c(uint32 port, uint32 phase)
{
    uint32 index, handle;
    FUNCTION_MARKER(0x800E294C, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        handle = r_u32(r_u32(0x80112DF0) + 20 + index * 4);
        if (TestEvent((sint32)handle) == 1)
        {
            w_u8(r_u32(0x80112DF0) + port + 844, phase);
            return index;
        }
    }
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e6dfc(uint32 x, uint32 y, uint32 ordering, uint32 angle, uint32 packet)
{
    uint32 pass, vertex, current, tag = 0, link, rotation;
    sint32 size, sx, sy, first, second, offset;
    FUNCTION_MARKER(0x800E6DFC, "ACE3.BIN");
    for (pass = 0; pass < 2; ++pass)
    {
        current = packet + pass * 40;
        size = pass ? 15 : 8;
        rotation = pass ? 0 - angle : angle;
        w_u8(current + 3, 9);
        w_u8(current + 7, 44);
        w_u16(current + 22, 31);
        w_u16(current + 14, 664);
        w_u8(current + 4, 128);
        w_u8(current + 5, 128);
        w_u8(current + 6, 128);
        w_u8(current + 13, pass ? 228 : 240);
        w_u8(current + 21, pass ? 228 : 240);
        w_u8(current + 12, pass ? 0 : 16);
        w_u8(current + 20, pass ? 13 : 23);
        w_u8(current + 28, pass ? 0 : 16);
        w_u8(current + 29, 255);
        w_u8(current + 36, pass ? 13 : 23);
        w_u8(current + 37, 255);
        for (vertex = 0; vertex < 4; ++vertex)
        {
            sx = vertex & 1 ? size : -size;
            sy = vertex & 2 ? size : -size;
            first = rcos((sint32)rotation);
            second = rsin((sint32)rotation);
            offset = (sx * first / 4096 + sy * second / 4096) / 2;
            w_u16(current + 8 + vertex * 8, x + (uint32)offset);
            first = rsin((sint32)rotation);
            second = rcos((sint32)rotation);
            offset = -sx * first / 4096 + sy * second / 4096;
            if (vertex == 3)
                tag = r_u32(current);
            w_u16(current + 10 + vertex * 8, y + (uint32)offset);
        }
        link = r_u32(ordering);
        w_u32(current, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | (current & 0xFFFFFF));
    }
    return packet + 80;
}

uint32 resource002_sub_800e7498(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800E7498, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3073);
    resource002_sub_800deca8(1, 11);
    resource002_sub_800deca8(1, 0);
    resource002_sub_800deca8(1, 10);
    resource002_sub_800dee08(1, 1, 0, 20);
    w_u8(r_u32(0x80112DF8) + 3, 0);
    w_u8(r_u32(0x80112DF8) + 8, 1);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 8, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 30, 0);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e7554(void)
{
    uint32 phase, game, value, result, accepted;
    FUNCTION_MARKER(0x800E7554, "ACE3.BIN");
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 43);
    phase = resource002_sub_800dee84(1);
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            value = r_u8(r_u32(0x80112DF8) + 3);
            if (value == 1)
            {
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 47);
                resource002_sub_800dee08(1, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), value);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
            sub_8002db4c(60);
        }
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    accepted = 0;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            result = 2;
            if (r_u8(r_u32(0x80112DF8) + 3) == 2)
            {
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 0);
                resource002_sub_800dee08(1, 0, 0, 0);
                value = r_u32(r_u32(0x80112DC4) + 224);
                value = resource002_sub_800e69a8(value);
                w_u8(r_u32(0x80112DF8), value);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return sub_8002db4c(59);
        }
    }
    return result;
}

uint32 resource002_sub_800e7758(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800E7758, "ACE3.BIN");
    w_u8(r_u32(0x80112DF8) + 8, 1);
    resource002_sub_800dedf0(1, 3073);
    if (resource002_sub_800e267c(0) && resource002_sub_800e267c(1))
    {
        w_u8(r_u32(0x80112DF8), 2);
        return 2;
    }
    resource002_sub_800dee08(5, 1, 0, 20);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e77ec(void)
{
    uint32 phase, result;
    FUNCTION_MARKER(0x800E77EC, "ACE3.BIN");
    phase = resource002_sub_800dee84(5);
    if (resource002_sub_800e267c(0) && resource002_sub_800e267c(1))
    {
        if (phase == 0)
        {
            resource002_sub_800dee08(5, 0, 0, 0);
            w_u8(r_u32(0x80112DF8), 2);
            result = r_u32(0x80112DF8);
            w_u8(result + 1, 0);
            return result;
        }
        if (phase != 3)
            return resource002_sub_800dee08(5, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        return 3;
    }
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 47);
    return 47;
}

uint32 resource002_sub_800e69a8(uint32 mode)
{
    uint32 game, state, value, context;
    FUNCTION_MARKER(0x800E69A8, "ACE3.BIN");
    if (mode == 0)
        return 18;
    if (mode == 1)
        return 17;
    if (mode == 2)
    {
        game = r_u32(0x800BCE60);
        state = r_u32(0x80112DC4);
        value = r_u32(game + 2404);
        context = r_u32(0x80112DF8);
        w_u32(game + 2404, value & 0xFFFFFFFEu);
        w_u32(state + 148, 0x60000004);
        w_u8(context + 2, 1);
        return 0;
    }
    if (mode == 3)
    {
        game = r_u32(0x800BCE60);
        w_u16(game + 2388, mode);
        value = r_u32(game + 2412);
        context = r_u32(0x80112DF8);
        w_u32(game + 2412, value | 16);
        w_u8(context + 2, 1);
        return 18;
    }
    return 0;
}

uint32 resource002_sub_800e267c(uint32 port)
{
    uint32 state, value;
    sint32 status;
    FUNCTION_MARKER(0x800E267C, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    value = r_u8(state + port + 840);
    status = r_s16(state + port * 2 + 44);
    if (value == 0)
        return status == 2 ? 6 : 0;
    if (status == 1 || status == 2)
        return (uint32)status;
    if (status != 0 && status != 3)
        return 0xFFFFFFFFu;
    state = r_u32(0x80112DF0);
    status = r_s16(state + port * 2 + 48);
    if (status >= 1 && status <= 3)
        return (uint32)status;
    if (status != 0)
        return 0xFFFFFFFFu;
    value = r_u8(state + port + 842);
    return value == 1 ? 5 : (value == 0 ? 4 : 1);
}

uint32 resource002_sub_800e78a0(void)
{
    uint32 first, second, context, value;
    FUNCTION_MARKER(0x800E78A0, "ACE3.BIN");
    first = resource002_sub_800e267c(0);
    second = resource002_sub_800e267c(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        resource002_sub_800dee08(4, 1, 0, 20);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 44);
        return 44;
    }
    w_u8(r_u32(0x80112DF8), 3);
    return 3;
}

uint32 resource002_sub_800e7948(void)
{
    uint32 phase, first, second, game, context, value, result, accepted;
    FUNCTION_MARKER(0x800E7948, "ACE3.BIN");
    phase = resource002_sub_800dee84(4);
    first = resource002_sub_800e267c(0);
    second = resource002_sub_800e267c(1);
    if ((first != 2 && first != 6) || (second != 2 && second != 6))
    {
        if (phase == 0 || phase == 3)
        {
            context = r_u32(0x80112DF8);
            if (r_u8(context + 3) == 3)
            {
                w_u8(context, 1);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
                return result;
            }
        }
        else
        {
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 3);
        }
    }
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(4, 0, 0, 0);
                value = r_u32(r_u32(0x80112DC4) + 224);
                value = resource002_sub_800e69a8(value);
                w_u8(r_u32(0x80112DF8), value);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
                return result;
            }
        }
        else
        {
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
        }
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    accepted = 0;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        accepted = result == 0;
    }
    if (accepted || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return 2;
        }
        result = 2;
        if (r_u8(r_u32(0x80112DF8) + 3) == 2)
        {
            resource002_sub_800dee08(4, 0, 0, 0);
            w_u8(r_u32(0x80112DF8), 0);
            result = r_u32(0x80112DF8);
            w_u8(result + 1, 0);
        }
    }
    return result;
}
