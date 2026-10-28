#include "game_functions.h"

uint32 resource002_sub_800d1f54(void)
{
    uint32 game, context, record, table, descriptor, base, choices, qualifiers;
    uint32 index, entry, mapping, value, flags, condition, result, command;
    sint32 active;
    FUNCTION_MARKER(0x800D1F54, "ACE3.BIN");
    active = (sint32)resource002_sub_800d1b04(r_u32(r_u32(0x800BCE60) + 292));
    table = 0x8010A520;
    for (record = 0; record < 5 * 92; record += 92, ++table)
    {
        entry = r_u8(table);
        game = r_u32(0x800BCE60);
        context = r_u32(0x80112DDC);
        descriptor = context + (entry << 4) + 36;
        for (index = 11; index < 32; ++index)
            w_u16(game + record + 348 + index * 2, 0);
        game = r_u32(0x800BCE60);
        for (index = 0; index < 15; ++index)
        {
            entry = r_u8(descriptor + index);
            mapping = r_u8(0x8010A528 + entry);
            if (mapping != 31)
            {
                value = r_u16(0x8010A540 + index * 2);
                w_u16(game + record + 348 + mapping * 2, value);
            }
        }
        game = r_u32(0x800BCE60);
        entry = r_u8(table);
        context = r_u32(0x80112DDC);
        base = game + record + 348 + 64;
        choices = context + (entry << 2) + 132;
        qualifiers = context + (entry << 2) + 156;
        for (index = 0; index < 6; ++index)
            w_u8(base + 23 - index, 255);
        for (index = 0; index < 4; ++index)
        {
            value = r_u8(choices + index);
            mapping = value >> 1;
            if (mapping < 6)
            {
                value = r_u8(0x8010A560 + index * 2);
                w_u8(base + mapping + 18, value);
                value = r_u8(choices + index);
                if (value == 0 || value == 6 || value == 8)
                {
                    condition = r_u8(qualifiers + index);
                    flags = r_u16(base + 16);
                    if (value == 0)
                        flags = condition ? flags & 0xFFFB : flags | 4;
                    else if (value == 6)
                        flags = condition ? flags & 0xFFF7 : flags | 8;
                    else
                        flags = condition == 1 ? flags | 16 : flags & 0xFFEF;
                    w_u16(base + 16, flags);
                }
            }
        }
        flags = r_u16(base + 16) & 0xFFFC;
        context = r_u32(0x80112DDC);
        w_u16(base + 16, flags);
        entry = r_u8(table);
        value = r_u8(context + entry + 21);
        if (value == 0)
            w_u16(base + 16, flags | 2);
        else if (value == 2)
            w_u16(base + 16, flags | 1);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 288);
    context = r_u32(0x80112DDC);
    flags &= 0xFFFFFFFB;
    w_u32(game + 288, flags);
    condition = r_u8(context + 20);
    result = flags | 4;
    if (!condition)
        w_u32(game + 288, result);
    if (active < 0)
        return result;
    game = r_u32(0x800BCE60);
    base = game + (uint32)active * 92 + 348;
    context = r_u32(0x80112DDC);
    condition = r_u8(context + 180);
    flags = r_u32(base + 88);
    flags = condition == 1 ? flags | 8 : flags & 0xFFFFFFF7;
    w_u32(base + 88, flags);
    context = r_u32(0x80112DDC);
    condition = r_u8(context + 181);
    flags = r_u32(base + 88);
    flags = condition == 1 ? flags | 16 : flags & 0xFFFFFFEF;
    w_u32(base + 88, flags);
    game = r_u32(0x800BCE60);
    context = r_u32(0x80112DDC);
    base = game + (uint32)active * 92 + 348;
    value = r_u8(context + 27);
    if (value > 2)
        return 4;
    condition = r_u32(game + 292);
    command = condition == 4 ? (value + 1) * 38 : (value + 1) * 16;
    result = base + 76;
    for (index = 0; index < 4; ++index)
    {
        w_u8(result + 3, command);
        result -= 4;
    }
    return result;
}

uint32 resource002_sub_800d7db0(void)
{
    uint32 game, context, next, state, object, value, raw, flags, phase;
    uint32 entry, index, packet, ordering, result, triangle[5] = {0}, quad[6] = {0};
    sint32 mode, selected, current_mode;
    FUNCTION_MARKER(0x800D7DB0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    context = r_u32(0x80112DDC);
    flags = r_u32(game + 300);
    mode = r_s16(context + 12);
    if ((flags & 0x40) && !(r_u32(game + 296) & 0x2F) && (r_s16(context + 14) != 2 || r_u8(context + 29) == 1) && !(r_u32(context + 32) & 0xFFFF0000))
    {
        sub_8002db4c(60);
        game = r_u32(0x800BCE60);
        context = r_u32(0x80112DDC);
        w_u32(game + 300, 0);
        w_u8(context + 30, 0);
        next = r_u32(0x80112DDC);
        w_u16(context + 14, 2);
        w_u8(next + 31, 0);
        w_u8(r_u32(0x80112DDC) + 29, 0);
        current_mode = r_s16(r_u32(0x80112DDC) + 12);
        if (current_mode == 0)
            resource002_sub_800ded68(2, 9, 0, 0);
        else if (current_mode == 3)
            resource002_sub_800ded68(2, 11, 0, 0);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 8, 0);
    }
    context = r_u32(0x80112DDC);
    if (r_s16(context + 14) >= 3)
    {
        if (!r_u8(context + 28))
        {
            value = r_u8(context + 34);
            if (value == 0)
                resource002_sub_800d7750();
            else if (value == 1)
                resource002_sub_800d7194();
            else if (value == 2)
                resource002_sub_800d6b3c();
            goto cancel;
        }
        value = r_u8(context + 35);
        if (value == 0)
            resource002_sub_800d65bc();
        else if (value == 1)
            resource002_sub_800d60fc();
        else if (value == 2)
            resource002_sub_800d5d64();
        context = r_u32(0x80112DDC);
        value = r_u8(context + 35);
        if (value == 1)
            value = r_u8(context + 33) + 48;
        else if (value == 0 || value == 2)
        {
            current_mode = r_s16(context + 12);
            index = r_u8(context + 31);
            entry = r_u32(context + (uint32)current_mode * 4 + 200);
            entry = r_u8(entry + index - 1);
            value = r_u8(context + (uint32)current_mode * 16 + entry + 36) + 48;
        }
        else
            value = 48;
        context = r_u32(0x80112DDC);
        if (r_u8(context + 35) == 2)
        {
            if (value == 62)
                value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 93 : 88;
            if (value == 63)
                value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 93 : 88;
            if (value == 67)
                value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 94 : 89;
        }
        else
        {
            if (value == 62 && r_u8(context + 181) == 1)
                value = 91;
            if (value == 63 && r_u8(r_u32(0x80112DDC) + 181) == 1)
                value = 92;
            if (value == 67 && r_u8(r_u32(0x80112DDC) + 180) == 1)
                value = 90;
        }
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 7, value);
        goto cancel;
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 2)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        raw = r_u16(context + 14) + 1;
        w_u16(context + 14, raw);
        if ((sint16)raw == 3)
        {
            if (r_u8(context + 28))
                w_u8(context + 31, 1);
            else
            {
                current_mode = r_s16(context + 12);
                w_u8(context + 30, current_mode == 1 ? 4 : 1);
            }
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        selected = r_s16(context + 14);
        raw = r_u16(context + 14);
        if (selected > 0)
            w_u16(context + 14, raw - 1);
        else
        {
            value = r_u8(context + 28);
            w_u16(context + 14, 3);
            if (!value)
                w_u8(context + 30, 4);
            else if (value == 1)
            {
                current_mode = r_s16(context + 12);
                value = r_u8(0x8010AC5C + (uint32)current_mode);
                w_u8(context + 31, value);
                if (r_s16(r_u32(0x80112DDC) + 12) == 3)
                {
                    resource002_sub_800dee08(5, 3, -1, -1);
                    resource002_sub_800dee08(7, 1, 0, 10);
                }
            }
        }
    }
    context = r_u32(0x80112DDC);
    selected = r_s16(context + 14);
    if (selected == 2 && r_u8(context + 29) == 1)
    {
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 300) & 16))
            goto cancel;
        flags = r_u32(game + 296);
        if (!(flags & 0x2F) && !(flags & 0x40))
        {
            sub_8002db4c(60);
            if (r_s16(r_u32(0x80112DDC) + 12) == 0)
                resource002_sub_800d180c();
            if (r_s16(r_u32(0x80112DDC) + 12) == 1)
                resource002_sub_800d18e4();
            if (r_s16(r_u32(0x80112DDC) + 12) == 2)
                resource002_sub_800d19a8();
            if (r_s16(r_u32(0x80112DDC) + 12) == 3)
                resource002_sub_800d1a3c();
            goto cancel;
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F) && r_s16(r_u32(0x80112DDC) + 14) == 2)
    {
        sub_8002db4c(60);
        resource002_sub_800d1f54();
        if (mode == 0 || mode == 2)
        {
            resource002_sub_800debb8(0, 1);
            w_u8(r_u32(0x80112DDC), 0);
        }
        if (mode == 1 || mode == 3)
        {
            resource002_sub_800dedf0(2, 8);
            w_u8(r_u32(0x80112DDC), 2);
        }
        w_u8(r_u32(0x80112DDC) + 1, 0);
        w_u32(r_u32(0x80112DDC) + 16, 0);
    }
cancel:
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        sub_8002db4c(59);
        resource002_sub_800debb8(0, 1);
        w_u8(r_u32(0x80112DDC), 0);
        w_u8(r_u32(0x80112DDC) + 1, 0);
        context = r_u32(0x80112DDC);
        game = r_u32(0x800BCE60);
        w_u32(context + 16, 0);
        w_u32(game + 300, 0);
    }
    context = r_u32(0x80112DDC);
    if (r_u8(context + 28) == 1)
    {
        current_mode = r_s16(context + 12);
        if (r_u8(0x8010AC5C + (uint32)current_mode))
        {
            index = 0;
            do
            {
                context = r_u32(0x80112DDC);
                current_mode = r_s16(context + 12);
                entry = r_u32(context + (uint32)current_mode * 4 + 184) + index * 16;
                phase = r_u32(entry);
                if (!phase || resource002_sub_800dee84(phase) == 2)
                {
                    packet = r_u32(r_u32(0x80112DDC) + 8);
                    triangle[0] = r_u32(entry + 4);
                    value = r_u32(entry + 8);
                    state = r_u32(0x80112DC4);
                    triangle[1] = value;
                    ordering = r_u32(state + 8) + 4;
                    triangle[2] = r_u32(entry + 12);
                    result = resource002_sub_800f6fb0(0, packet, ordering, triangle);
                    w_u32(r_u32(0x80112DDC) + 8, result);
                }
                context = r_u32(0x80112DDC);
                current_mode = r_s16(context + 12);
                value = r_u8(0x8010AC5C + (uint32)current_mode);
                ++index;
            } while (index < value);
        }
        if (r_s16(r_u32(0x80112DDC) + 12) == 3)
        {
            for (index = 0, entry = 0x8010AC20; index < 3; ++index, entry += 20)
            {
                phase = r_u32(entry);
                if (!phase || resource002_sub_800dee84(phase) == 2)
                {
                    packet = r_u32(r_u32(0x80112DDC) + 8);
                    quad[0] = r_u32(entry + 4);
                    quad[1] = r_u32(entry + 8);
                    value = r_u32(entry + 12);
                    state = r_u32(0x80112DC4);
                    quad[2] = value;
                    ordering = r_u32(state + 8) + 4;
                    quad[3] = r_u32(entry + 16);
                    result = resource002_sub_800f711c(0, packet, ordering, quad);
                    w_u32(r_u32(0x80112DDC) + 8, result);
                }
            }
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 8)
    {
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) < 3 && r_u32(context + 12) != 0x10002)
            sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 0)
        {
            w_u32(context + 16, 0);
            resource002_sub_800deca8(2, 2);
            resource002_sub_800deca8(2, 3);
            resource002_sub_800dedf0(2, 12);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 7, 0);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 20, 0);
        }
        if (r_s16(r_u32(0x80112DDC) + 14) == 1)
            resource002_sub_800d5bd4();
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 2)
        {
            value = r_u8(context + 29);
            w_u8(context + 29, 1u - value);
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 4)
    {
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) < 3 && r_u32(context + 12) != 0x10002)
            sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 0)
        {
            w_u32(context + 16, 0);
            resource002_sub_800deca8(2, 2);
            resource002_sub_800deca8(2, 3);
            resource002_sub_800dedf0(2, 12);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 7, 0);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 20, 0);
        }
        if (r_s16(r_u32(0x80112DDC) + 14) == 1)
            resource002_sub_800d5bd4();
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 2)
        {
            value = r_u8(context + 29);
            w_u8(context + 29, 1u - value);
        }
        else
            goto clear_restore;
    }
    if (r_s16(r_u32(0x80112DDC) + 14) == 2)
        goto publish;
clear_restore:
    w_u8(r_u32(0x80112DDC) + 29, 0);
publish:
    state = r_u32(0x80112DC0);
    context = r_u32(0x80112DDC);
    object = r_u32(state + 92);
    value = r_u8(context + 14);
    w_u8(object + 1, value);
    value = r_s16(r_u32(0x80112DDC) + 12) == 2;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 59, value);
    value = r_s16(r_u32(0x80112DDC) + 14) != 0;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 2, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 28) ? 1 : (r_s16(context + 14) == 1 ? 2 : 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 3, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 28) == 1 ? (r_s16(context + 14) == 1 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 4, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 29) ? 1 : (r_s16(context + 14) == 2 ? 2 : 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 5, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 29) == 1 ? (r_s16(context + 14) == 2 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 6, value);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 21, 0);
    if (r_s16(r_u32(0x80112DDC) + 14) == 0)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 21, 11);
    context = r_u32(0x80112DDC);
    if (r_s16(context + 14) == 1)
    {
        value = r_u8(context + 28) ? 13 : 12;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 21, value);
    }
    context = r_u32(0x80112DDC);
    if (r_s16(context + 14) == 2)
    {
        if (r_u8(context + 29))
        {
            object = r_u32(r_u32(0x80112DC0) + 92);
            value = 16;
        }
        else
        {
            value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 10 : 9;
            object = r_u32(r_u32(0x80112DC0) + 92);
        }
        w_u8(object + 21, value);
    }
    result = r_s16(r_u32(0x80112DDC) + 14) < 3;
    if (result)
    {
        result = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(result + 7, 0);
    }
    return result;
}

uint32 resource002_sub_800d5bd4(void)
{
    uint32 context, value, result;
    sint32 mode;
    FUNCTION_MARKER(0x800D5BD4, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    if (mode == 0)
    {
        if (!r_u8(context + 28))
        {
            resource002_sub_800deca8(2, 5);
            resource002_sub_800dedf0(2, 48);
        }
        if (r_u8(r_u32(0x80112DDC) + 28) == 1)
        {
            resource002_sub_800deca8(2, 9);
            resource002_sub_800dedf0(2, 528);
        }
    }
    else if (mode == 1)
    {
        if (!r_u8(context + 28))
        {
            resource002_sub_800deca8(2, 6);
            resource002_sub_800dedf0(2, 80);
        }
        if (r_u8(r_u32(0x80112DDC) + 28) == 1)
        {
            resource002_sub_800deca8(2, 10);
            resource002_sub_800dedf0(2, 1040);
        }
    }
    else if (mode == 2)
        w_u8(context + 28, 0);
    else if (mode == 3)
    {
        if (!r_u8(context + 28))
        {
            resource002_sub_800deca8(2, 8);
            resource002_sub_800dedf0(2, 272);
        }
        if (r_u8(r_u32(0x80112DDC) + 28) == 1)
        {
            resource002_sub_800deca8(2, 11);
            resource002_sub_800dedf0(2, 2064);
        }
    }
    context = r_u32(0x80112DDC);
    value = r_u8(context + 28);
    result = 1u - value;
    w_u8(context + 28, result);
    return result;
}

uint32 resource002_sub_800ded68(uint32 group, uint32 item, sint32 x, sint32 y)
{
    FUNCTION_MARKER(0x800DED68, "ACE3.BIN");
    return resource002_sub_800deb5c(group, item, 1, (uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y, 0);
}

uint32 resource002_sub_800f6ec0(uint32 descriptor, uint32 packet, uint32 ordering, uint32 *native_descriptor)
{
    uint8 *bytes = (uint8 *)native_descriptor;
    uint32 value, command, tag, link, index;
    FUNCTION_MARKER(0x800F6EC0, "ACE3.BIN");
    w_u8(packet + 3, 5);
    w_u8(packet + 7, 0x48);
    w_u32(packet + 20, 0x55555555);
    value = bytes ? bytes[15] : r_u8(descriptor + 15);
    command = r_u8(packet + 7);
    w_u8(packet + 7, value ? command | 2 : command & 0xFD);
    value = bytes ? bytes[12] : r_u8(descriptor + 12);
    tag = r_u32(packet);
    w_u8(packet + 4, value);
    value = bytes ? bytes[13] : r_u8(descriptor + 13);
    w_u8(packet + 5, value);
    value = bytes ? bytes[14] : r_u8(descriptor + 14);
    w_u8(packet + 6, value);
    for (index = 0; index < 12; index += 2)
    {
        value = bytes ? (uint32)bytes[index] | ((uint32)bytes[index + 1] << 8) : r_u16(descriptor + index);
        w_u16(packet + index + 8, value);
    }
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 24;
}

uint32 resource002_sub_800f6fb0(uint32 descriptor, uint32 packet, uint32 ordering, uint32 *native_descriptor)
{
    uint8 *bytes = (uint8 *)native_descriptor;
    uint32 first, third, second;
    FUNCTION_MARKER(0x800F6FB0, "ACE3.BIN");
    packet = resource002_sub_800f6ec0(descriptor, packet, ordering, native_descriptor);
    first = bytes ? (uint32)bytes[2] | ((uint32)bytes[3] << 8) : r_u16(descriptor + 2);
    third = bytes ? (uint32)bytes[10] | ((uint32)bytes[11] << 8) : r_u16(descriptor + 10);
    ++first;
    if (bytes)
    {
        bytes[2] = (uint8)first;
        bytes[3] = (uint8)(first >> 8);
    }
    else
        w_u16(descriptor + 2, first);
    second = bytes ? (uint32)bytes[6] | ((uint32)bytes[7] << 8) : r_u16(descriptor + 6);
    ++third;
    if (bytes)
    {
        bytes[10] = (uint8)third;
        bytes[11] = (uint8)(third >> 8);
    }
    else
        w_u16(descriptor + 10, third);
    ++second;
    if (bytes)
    {
        bytes[6] = (uint8)second;
        bytes[7] = (uint8)(second >> 8);
    }
    else
        w_u16(descriptor + 6, second);
    return resource002_sub_800f6ec0(descriptor, packet, ordering, native_descriptor);
}

uint32 resource002_sub_800f7014(uint32 descriptor, uint32 packet, uint32 ordering, uint32 *native_descriptor)
{
    uint8 *bytes = (uint8 *)native_descriptor;
    uint32 value, command, tag, link, index;
    FUNCTION_MARKER(0x800F7014, "ACE3.BIN");
    w_u8(packet + 3, 6);
    w_u8(packet + 7, 0x4C);
    w_u32(packet + 24, 0x55555555);
    value = bytes ? bytes[19] : r_u8(descriptor + 19);
    command = r_u8(packet + 7);
    w_u8(packet + 7, value ? command | 2 : command & 0xFD);
    value = bytes ? bytes[16] : r_u8(descriptor + 16);
    tag = r_u32(packet);
    w_u8(packet + 4, value);
    value = bytes ? bytes[17] : r_u8(descriptor + 17);
    w_u8(packet + 5, value);
    value = bytes ? bytes[18] : r_u8(descriptor + 18);
    w_u8(packet + 6, value);
    for (index = 0; index < 16; index += 2)
    {
        value = bytes ? (uint32)bytes[index] | ((uint32)bytes[index + 1] << 8) : r_u16(descriptor + index);
        w_u16(packet + index + 8, value);
    }
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 28;
}

uint32 resource002_sub_800f711c(uint32 descriptor, uint32 packet, uint32 ordering, uint32 *native_descriptor)
{
    uint8 *bytes = (uint8 *)native_descriptor;
    uint32 first, third, second;
    FUNCTION_MARKER(0x800F711C, "ACE3.BIN");
    packet = resource002_sub_800f7014(descriptor, packet, ordering, native_descriptor);
    first = bytes ? (uint32)bytes[2] | ((uint32)bytes[3] << 8) : r_u16(descriptor + 2);
    third = bytes ? (uint32)bytes[10] | ((uint32)bytes[11] << 8) : r_u16(descriptor + 10);
    ++first;
    if (bytes)
    {
        bytes[2] = (uint8)first;
        bytes[3] = (uint8)(first >> 8);
    }
    else
        w_u16(descriptor + 2, first);
    second = bytes ? (uint32)bytes[6] | ((uint32)bytes[7] << 8) : r_u16(descriptor + 6);
    ++third;
    if (bytes)
    {
        bytes[10] = (uint8)third;
        bytes[11] = (uint8)(third >> 8);
    }
    else
        w_u16(descriptor + 10, third);
    ++second;
    if (bytes)
    {
        bytes[6] = (uint8)second;
        bytes[7] = (uint8)(second >> 8);
    }
    else
        w_u16(descriptor + 6, second);
    return resource002_sub_800f7014(descriptor, packet, ordering, native_descriptor);
}

uint32 resource002_sub_800d5d64(void)
{
    FUNCTION_MARKER(0x800D5D64, "ACE3.BIN");
    return resource002_sub_800d5d6c(r_u32(0x80112DDC));
}

uint32 resource002_sub_800d5d6c(uint32 context)
{
    uint32 game, value, offset, flags, index, entry, object;
    sint32 mode;
    FUNCTION_MARKER(0x800D5D6C, "ACE3.BIN");
    value = r_u8(context + 33);
    if ((value >= 14 && value < 16) || value == 19)
    {
        offset = value == 19 ? 180 : 181;
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x6F))
        {
            w_u8(context + offset, 1);
            sub_8002db4c(60);
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
        {
            sub_8002db4c(60);
            w_u8(r_u32(0x80112DDC) + offset, 0);
        }
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (((flags & 16) && !(r_u32(game + 296) & 0x6F)) || ((flags & 32) && !(r_u32(game + 296) & 0x5F)))
    {
        w_u8(r_u32(0x80112DDC) + 35, 0);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    for (index = 10; index < 31; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 33, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 35, 0);
    for (index = 0; index < 15; ++index)
    {
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        entry = r_u32(context + (uint32)mode * 4 + 200);
        entry = r_u8(entry + index);
        value = r_u8(context + (uint32)mode * 16 + entry + 36) + 10;
        if (value == 21)
            value = 33;
        if (value == 11)
            value = 35;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
    }
    for (index = 0; index < 4; ++index)
    {
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        entry = context + (uint32)mode * 4 + index + 132;
        if (r_u8(entry) < 8)
        {
            value = r_u8(entry) + 10;
            object = r_u32(r_u32(0x80112DC0) + 92);
            if (value == 10)
            {
                w_u8(object + 10, 1);
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 35, 1);
            }
            else
            {
                w_u8(object + value, 1);
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + value + 1, 1);
            }
        }
    }
    return 0;
}

uint32 resource002_sub_800d60fc(void)
{
    uint32 context, game, mappings, choices, value, old, index, entry, state, object;
    sint32 mode;
    FUNCTION_MARKER(0x800D60FC, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    game = r_u32(0x800BCE60);
    mappings = context + (uint32)mode * 16 + 36;
    choices = context + (uint32)mode * 4 + 132;
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        sub_8002db4c(59);
        w_u8(r_u32(0x80112DDC) + 35, 0);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 2)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 33);
        w_u8(context + 33, value < 20 ? value + 1 : 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 33);
        w_u8(context + 33, value ? value - 1 : 20);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x6F))
    {
        sub_8002db4c(60);
        for (index = 0; index < 15; ++index)
        {
            context = r_u32(0x80112DDC);
            old = r_u8(mappings + index);
            value = r_u8(context + 33);
            if (old == value)
                w_u8(mappings + index, 20);
        }
        for (index = 0; index < 4; ++index)
        {
            context = r_u32(0x80112DDC);
            value = r_u8(context + 33);
            old = r_u8(choices + index);
            if (old == (value & 0xFE) && old < 8)
                w_u8(choices + index, 12);
        }
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        index = r_u8(context + 31);
        entry = r_u32(context + (uint32)mode * 4 + 200);
        entry = r_u8(entry + index - 1);
        value = r_u8(context + 33);
        w_u8(mappings + entry, value);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 33);
        if (value == 14 || value == 19 || value == 15)
            w_u8(r_u32(0x80112DDC) + 35, 2);
        else
        {
            w_u8(context + 35, 0);
            resource002_sub_800dee08(1, 3, -1, -1);
            resource002_sub_800dee08(2, 3, -1, -1);
        }
    }
    context = r_u32(0x80112DDC);
    if (r_u8(context + 35))
    {
        state = r_u32(0x80112DC0);
        value = r_u8(context + 33);
        object = r_u32(state + 92);
        w_u8(object + 9, value + 1);
    }
    else
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 9, 0);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC0);
    value = r_u8(context + 33);
    object = r_u32(state + 92);
    w_u8(object + 7, value + 48);
    for (index = 10; index < 31; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 33, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 35, 0);
    for (index = 0; index < 15; ++index)
    {
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        entry = r_u32(context + (uint32)mode * 4 + 200);
        entry = r_u8(entry + index);
        value = r_u8(context + (uint32)mode * 16 + entry + 36) + 10;
        if (value == 21)
            value = 33;
        if (value == 11)
            value = 35;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
    }
    for (index = 0; index < 4; ++index)
    {
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        entry = context + (uint32)mode * 4 + index + 132;
        if (r_u8(entry) < 8)
        {
            value = r_u8(entry) + 10;
            object = r_u32(r_u32(0x80112DC0) + 92);
            if (value == 10)
            {
                w_u8(object + 10, 1);
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 35, 1);
            }
            else
            {
                w_u8(object + value, 1);
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + value + 1, 1);
            }
        }
    }
    return 0;
}

uint32 resource002_sub_800d65bc(void)
{
    static const uint8 down_choices[] = {2, 4, 11, 13};
    static const uint8 down_out[] = {4, 6, 5, 7};
    static const uint8 down_in[] = {6, 4, 7, 5};
    static const uint8 up_choices[] = {5, 3, 12};
    static const uint8 up_out[] = {4, 6, 7};
    static const uint8 up_in[] = {6, 4, 5};
    uint32 context, game, value, count, index, entry, state, object;
    sint32 mode;
    FUNCTION_MARKER(0x800D65BC, "ACE3.BIN");
    mode = r_s16(r_u32(0x80112DDC) + 12);
    if (mode == 0 || mode == 1 || mode == 2)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            if (mode == 1)
            {
                resource002_sub_800dee08(1, 1, 0, 10);
                sub_8002db4c(60);
            }
            else
            {
                sub_8002db4c(60);
                resource002_sub_800dee08(1, 1, 0, 10);
            }
        }
    }
    else if (mode == 3)
    {
        if (r_u32(r_u32(0x800BCE60) + 304) & 2)
        {
            sub_8002db4c(61);
            for (index = 0; index < sizeof(down_choices); ++index)
            {
                if (r_u8(r_u32(0x80112DDC) + 31) == down_choices[index])
                {
                    resource002_sub_800dee08(down_out[index], 3, -1, -1);
                    resource002_sub_800dee08(down_in[index], 1, 0, 10);
                }
            }
        }
        if (r_u32(r_u32(0x800BCE60) + 304) & 1)
        {
            sub_8002db4c(61);
            for (index = 0; index < sizeof(up_choices); ++index)
            {
                if (r_u8(r_u32(0x80112DDC) + 31) == up_choices[index])
                {
                    resource002_sub_800dee08(up_out[index], 3, -1, -1);
                    resource002_sub_800dee08(up_in[index], 1, 0, 10);
                }
            }
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            sub_8002db4c(60);
            value = r_u8(r_u32(0x80112DDC) + 31);
            if ((uint32)(value - 6u) < 4)
                resource002_sub_800dee08(2, 1, 0, 20);
            else
            {
                resource002_sub_800dee08(1, 1, 0, 20);
                value = r_u8(r_u32(0x80112DDC) + 31);
                if ((uint32)(value - 6u) < 4)
                    resource002_sub_800dee08(2, 1, 0, 20);
            }
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 2)
    {
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        value = r_u8(context + 31);
        count = r_u8(0x8010AC5C + (uint32)mode);
        if (value < count)
            w_u8(context + 31, value + 1);
        else
        {
            w_u8(context + 31, 0);
            w_u16(r_u32(0x80112DDC) + 14, 0);
        }
        sub_8002db4c(61);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 31);
        if (value >= 2)
            w_u8(context + 31, value - 1);
        else
        {
            w_u8(context + 31, 0);
            w_u16(r_u32(0x80112DDC) + 14, 2);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        sub_8002db4c(60);
        w_u8(r_u32(0x80112DDC) + 35, 1);
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        index = r_u8(context + 31);
        entry = r_u32(context + (uint32)mode * 4 + 200);
        entry = r_u8(entry + index - 1);
        value = r_u8(context + (uint32)mode * 16 + entry + 36);
        w_u8(context + 33, value);
    }
    state = r_u32(0x80112DC0);
    context = r_u32(0x80112DDC);
    object = r_u32(state + 92);
    value = r_u8(context + 31);
    w_u8(object + 8, value);
    return value;
}

uint32 resource002_sub_800d6b3c(void)
{
    uint32 context, game, qualifier, selection, flags, accepted, value, state, object, index, entry, count;
    uint32 first_offset, second_offset;
    sint32 mode, remainder;
    FUNCTION_MARKER(0x800D6B3C, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    game = r_u32(0x800BCE60);
    mode = r_s16(context + 12);
    selection = r_u8(context + 30);
    flags = r_u32(game + 300);
    qualifier = context + (uint32)mode * 4 + 156 + selection - 1;
    if ((flags & 32) && !(r_u32(game + 296) & 0x5F))
    {
        w_u8(context + 34, 1);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 62, 0);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 63, 0);
        sub_8002db4c(60);
        w_u32(r_u32(0x800BCE60) + 300, 0);
        return 0;
    }
    if ((r_u32(r_u32(0x800BCE60) + 304) & 3) && !((r_u8(r_u32(0x80112DDC) + 30) - 1u) & 1))
    {
        sub_8002db4c(61);
        value = r_u8(qualifier);
        w_u8(qualifier, 1u - value);
        w_u32(r_u32(0x800BCE60) + 304, 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 12)
    {
        remainder = ((sint32)r_u8(r_u32(0x80112DDC) + 30) - 1) % 2;
        if (remainder == 1)
        {
            sub_8002db4c(61);
            value = r_u8(qualifier);
            w_u8(qualifier, 1u - value);
            w_u32(r_u32(0x800BCE60) + 304, 0);
        }
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    accepted = 0;
    if (flags & 16)
        accepted = !(r_u32(game + 296) & 0x2F);
    flags = r_u32(game + 296);
    if (accepted & (((flags >> 6) ^ 1) & 1))
    {
        w_u8(r_u32(0x80112DDC) + 34, 0);
        value = r_u8(r_u32(0x80112DDC) + 30);
        if ((uint32)(value - 1u) < 2)
            resource002_sub_800dee08(1, 3, -1, -1);
        value = r_u8(r_u32(0x80112DDC) + 30);
        if ((uint32)(value - 3u) < 2)
            resource002_sub_800dee08(2, 3, -1, -1);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 62, 0);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 63, 0);
        sub_8002db4c(60);
        return 0;
    }
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC0);
    value = r_u8(context + 32);
    object = r_u32(state + 92);
    w_u8(object + 7, value * 5 + 18);
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    value = r_u8(qualifier);
    first_offset = value ? 63 : 62;
    second_offset = value ? 62 : 63;
    if (mode == 1 || mode == 2)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + first_offset, 15);
        context = r_u32(0x80112DDC);
        state = r_u32(0x80112DC0);
        value = r_u8(context + 32);
        object = r_u32(state + 92);
        w_u8(object + second_offset, value + 16);
    }
    else
    {
        remainder = (sint32)r_u8(context + 30) - 1;
        state = r_u32(0x80112DC0);
        remainder %= 2;
        object = r_u32(state + 92);
        w_u8(object + first_offset, (uint32)(remainder * 7 + 1));
        context = r_u32(0x80112DDC);
        remainder = ((sint32)r_u8(context + 30) - 1) % 2;
        state = r_u32(0x80112DC0);
        value = r_u8(context + 32);
        object = r_u32(state + 92);
        w_u8(object + second_offset, value + (uint32)(remainder * 7 + 2));
    }
    for (index = 10; index < 18; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    if (mode == 1)
    {
        value = (r_u8(context + 139) >> 1) + 10;
        if (value == 11)
            value = 17;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
    }
    else
    {
        for (index = 0; index < 4; ++index)
        {
            context = r_u32(0x80112DDC);
            mode = r_s16(context + 12);
            value = (r_u8(context + (uint32)mode * 4 + index + 132) >> 1) + 10;
            if (value == 11)
                value = 17;
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
        }
    }
    mode = r_s16(r_u32(0x80112DDC) + 12);
    count = r_u8(0x8010AC5C + (uint32)mode);
    if (count)
    {
        index = 0;
        do
        {
            context = r_u32(0x80112DDC);
            mode = r_s16(context + 12);
            entry = r_u32(context + (uint32)mode * 4 + 200);
            entry = r_u8(entry + index);
            value = r_u8(context + (uint32)mode * 16 + entry + 36);
            if (value < 8)
            {
                value = (value >> 1) + 10;
                if (value == 11)
                    value = 35;
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
            }
            mode = r_s16(r_u32(0x80112DDC) + 12);
            count = r_u8(0x8010AC5C + (uint32)mode);
            ++index;
        } while (index < count);
    }
    return 1;
}

uint32 resource002_sub_800d7194(void)
{
    FUNCTION_MARKER(0x800D7194, "ACE3.BIN");
    return resource002_sub_800d719c(r_u32(0x80112DDC));
}

uint32 resource002_sub_800d719c(uint32 context)
{
    uint32 game, choices, mappings, value, old, index, count, entry, address, state, object;
    sint32 mode;
    FUNCTION_MARKER(0x800D719C, "ACE3.BIN");
    mode = r_s16(context + 12);
    game = r_u32(0x800BCE60);
    choices = context + (uint32)mode * 4 + 132;
    mappings = context + (uint32)mode * 16 + 36;
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        w_u8(context + 34, 0);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        sub_8002db4c(59);
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 2)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 32);
        w_u8(context + 32, value < 6 ? value + 1 : 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        sub_8002db4c(61);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 32);
        w_u8(context + 32, value ? value - 1 : 6);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x6F))
    {
        sub_8002db4c(60);
        for (index = 0; index < 15; ++index)
        {
            context = r_u32(0x80112DDC);
            value = r_u8(context + 32);
            old = r_u8(choices + index);
            if (old == value * 2)
                w_u8(choices + index, 12);
        }
        mode = r_s16(r_u32(0x80112DDC) + 12);
        count = r_u8(0x8010AC5C + (uint32)mode);
        if (count)
        {
            index = 0;
            do
            {
                context = r_u32(0x80112DDC);
                value = r_u8(context + 32) * 2;
                if (value < 8)
                {
                    mode = r_s16(context + 12);
                    entry = r_u32(context + (uint32)mode * 4 + 200);
                    entry = r_u8(entry + index);
                    address = mappings + entry;
                    old = r_u8(address);
                    if (old == value)
                        w_u8(address, 20);
                    context = r_u32(0x80112DDC);
                    mode = r_s16(context + 12);
                    entry = r_u32(context + (uint32)mode * 4 + 200);
                    entry = r_u8(entry + index);
                    value = r_u8(context + 32);
                    address = mappings + entry;
                    old = r_u8(address);
                    if (old == value * 2 + 1)
                        w_u8(address, 20);
                }
                mode = r_s16(r_u32(0x80112DDC) + 12);
                count = r_u8(0x8010AC5C + (uint32)mode);
                ++index;
            } while (index < count);
        }
        context = r_u32(0x80112DDC);
        index = r_u8(context + 30);
        value = r_u8(context + 32);
        w_u8(choices + index - 1, value * 2);
        context = r_u32(0x80112DDC);
        value = r_u8(context + 32);
        if (value == 0 || value == 3 || value == 4)
            w_u8(r_u32(0x80112DDC) + 34, 2);
        else
        {
            w_u8(context + 34, 0);
            resource002_sub_800dee08(1, 3, -1, -1);
            resource002_sub_800dee08(2, 3, -1, -1);
        }
    }
    context = r_u32(0x80112DDC);
    if (r_u8(context + 34))
    {
        state = r_u32(0x80112DC0);
        value = r_u8(context + 32);
        object = r_u32(state + 92);
        w_u8(object + 9, value + 1);
    }
    else
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 9, 0);
    value = r_u8(r_u32(0x80112DDC) + 32);
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 7, value == 6 ? 68 : value * 5 + 17);
    for (index = 10; index < 18; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    if (mode == 1)
    {
        value = (r_u8(context + 139) >> 1) + 10;
        if (value == 11)
            value = 17;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
    }
    else
    {
        for (index = 0; index < 4; ++index)
        {
            context = r_u32(0x80112DDC);
            mode = r_s16(context + 12);
            value = (r_u8(context + (uint32)mode * 4 + index + 132) >> 1) + 10;
            if (value == 11)
                value = 17;
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
        }
    }
    mode = r_s16(r_u32(0x80112DDC) + 12);
    count = r_u8(0x8010AC5C + (uint32)mode);
    if (count)
    {
        index = 0;
        do
        {
            context = r_u32(0x80112DDC);
            mode = r_s16(context + 12);
            entry = r_u32(context + (uint32)mode * 4 + 200);
            entry = r_u8(entry + index);
            value = r_u8(context + (uint32)mode * 16 + entry + 36);
            if (value < 8)
            {
                value = (value >> 1) + 10;
                if (value == 11)
                    value = 17;
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + value, 1);
            }
            mode = r_s16(r_u32(0x80112DDC) + 12);
            count = r_u8(0x8010AC5C + (uint32)mode);
            ++index;
        } while (index < count);
    }
    return 0;
}

uint32 resource002_sub_800d7750(void)
{
    uint32 context, game, value, item, entry, state, object, result, edit = 0;
    sint32 mode;
    FUNCTION_MARKER(0x800D7750, "ACE3.BIN");
    mode = r_s16(r_u32(0x80112DDC) + 12);
    if (mode == 0 || mode == 3)
    {
        item = mode == 0 ? 9 : 11;
        if (r_u32(r_u32(0x800BCE60) + 304) & 2)
        {
            sub_8002db4c(61);
            context = r_u32(0x80112DDC);
            value = r_u8(context + 30);
            if (value < 4)
                w_u8(context + 30, value + 1);
            else
            {
                w_u8(context + 30, 0);
                w_u16(r_u32(0x80112DDC) + 14, 0);
            }
        }
        if (r_u8(r_u32(0x80112DDC) + 30) == 0)
            resource002_sub_800ded68(2, item, 0, 0);
        if (r_u8(r_u32(0x80112DDC) + 30) == 1)
            resource002_sub_800ded68(2, item, 40, 0);
        if (r_u8(r_u32(0x80112DDC) + 30) == 3)
            resource002_sub_800ded68(2, item, -40, 0);
        if (r_u32(r_u32(0x800BCE60) + 304) & 1)
        {
            sub_8002db4c(61);
            context = r_u32(0x80112DDC);
            value = r_u8(context + 30);
            if (value >= 2)
                w_u8(context + 30, value - 1);
            else
            {
                w_u8(context + 30, 0);
                w_u16(r_u32(0x80112DDC) + 14, 2);
            }
        }
        if (r_u8(r_u32(0x80112DDC) + 30) == 0)
            resource002_sub_800ded68(2, item, 0, 0);
        if (r_u8(r_u32(0x80112DDC) + 30) == 2)
            resource002_sub_800ded68(2, item, 40, 0);
        if (r_u8(r_u32(0x80112DDC) + 30) == 4)
            resource002_sub_800ded68(2, item, -40, 0);
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            sub_8002db4c(60);
            value = r_u8(r_u32(0x80112DDC) + 30);
            if ((uint32)(value - 1u) < 2)
                resource002_sub_800dee08(1, 1, 0, 10);
            value = r_u8(r_u32(0x80112DDC) + 30);
            if ((uint32)(value - 3u) < 2)
                resource002_sub_800dee08(2, 1, 0, 10);
            edit = 1;
        }
    }
    else if (mode == 1)
    {
        if (r_u32(r_u32(0x800BCE60) + 304) & 2)
        {
            sub_8002db4c(61);
            w_u8(r_u32(0x80112DDC) + 30, 0);
            w_u16(r_u32(0x80112DDC) + 14, 0);
        }
        if (r_u32(r_u32(0x800BCE60) + 304) & 1)
        {
            sub_8002db4c(61);
            w_u8(r_u32(0x80112DDC) + 30, 0);
            w_u16(r_u32(0x80112DDC) + 14, 2);
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            sub_8002db4c(60);
            resource002_sub_800dee08(1, 1, 0, 10);
            edit = 1;
        }
    }
    if (edit)
    {
        w_u8(r_u32(0x80112DDC) + 34, 1);
        context = r_u32(0x80112DDC);
        mode = r_s16(context + 12);
        value = r_u8(context + 30);
        entry = r_u8(context + (uint32)mode * 4 + value - 1 + 132);
        w_u8(context + 32, entry >> 1);
    }
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    value = r_u8(context + 30);
    entry = r_u8(context + (uint32)mode * 4 + value - 1 + 132);
    value = entry == 12 ? 68 : (entry >> 1) * 5 + 17;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 7, value);
    context = r_u32(0x80112DDC);
    mode = r_s16(context + 12);
    if (mode == 1 && r_u8(context + 30) == 4)
    {
        result = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(result + 8, 1);
    }
    else
    {
        state = r_u32(0x80112DC0);
        context = r_u32(0x80112DDC);
        object = r_u32(state + 92);
        result = r_u8(context + 30);
        w_u8(object + 8, result);
    }
    return result;
}

uint32 resource002_sub_800d911c(void)
{
    uint32 index, context, result;
    FUNCTION_MARKER(0x800D911C, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        w_u8(r_u32(0x80112DDC) + index + 236, 136);
        w_u8(r_u32(0x80112DDC) + index + 240, 128);
        w_u8(r_u32(0x80112DDC) + index + 244, 16);
        w_u8(r_u32(0x80112DDC) + index + 248, 240);
    }
    w_u8(r_u32(0x80112DDC) + 282, 240);
    w_u8(r_u32(0x80112DDC) + 283, 110);
    w_u8(r_u32(0x80112DDC) + 284, 0);
    w_u8(r_u32(0x80112DDC) + 286, 240);
    w_u8(r_u32(0x80112DDC) + 287, 110);
    w_u8(r_u32(0x80112DDC) + 288, 0);
    context = r_u32(0x80112DDC);
    w_u8(context + 289, 0);
    w_u8(context + 285, 0);
    context = r_u32(0x80112DDC);
    result = r_u8(context + 1);
    w_u32(context + 252, 0);
    ++result;
    w_u8(context + 1, result);
    return result;
}

uint32 resource002_sub_800da444(void)
{
    uint32 game, context, value, result;
    FUNCTION_MARKER(0x800DA444, "ACE3.BIN");
    if (r_s16(r_u32(0x80112DDC) + 12) == 1)
        resource002_sub_800d91fc();
    if (r_s16(r_u32(0x80112DDC) + 12) == 3)
        resource002_sub_800d9994();
    game = r_u32(0x800BCE60);
    if (r_s16(game + 342) != 2)
    {
        context = r_u32(0x80112DDC);
        value = r_u32(game + 292);
        result = r_u32(context + 232);
        if (value == result)
            return result;
    }
    w_u8(r_u32(0x80112DDC), 1);
    result = r_u32(0x80112DDC);
    w_u8(result + 1, 0);
    return result;
}

uint32 resource002_sub_800da4f4(void)
{
    FUNCTION_MARKER(0x800DA4F4, "ACE3.BIN");
    return resource002_sub_800da4fc(r_u32(0x80112DC4));
}

uint32 resource002_sub_800da4fc(uint32 state)
{
    uint32 game, context, value;
    FUNCTION_MARKER(0x800DA4FC, "ACE3.BIN");
    resource002_sub_800ecda8(r_u32(state + 160));
    w_u8(r_u32(0x80112DDC) + 1, 1);
    resource002_sub_800deca8(2, 12);
    resource002_sub_800dedf0(0, 1);
    resource002_sub_800dedf0(2, 4096);
    w_u8(r_u32(0x80112DDC) + 256, 0);
    game = r_u32(0x800BCE60);
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
    w_u8(r_u32(0x80112DDC) + 3, 0);
    value = r_u8(r_u32(0x80112DDC) + 6);
    resource002_sub_800def94(5, value);
    resource002_sub_800def94(6, 0);
    resource002_sub_800dee08(1, 1, 0, 30);
    return resource002_sub_800f7514(0x8010AC60);
}

uint32 resource002_sub_800da5ec(void)
{
    FUNCTION_MARKER(0x800DA5EC, "ACE3.BIN");
    return resource002_sub_800da5f4(r_u32(0x800BCE60));
}

uint32 resource002_sub_800da5f4(uint32 game)
{
    uint32 phase, result = 2;
    FUNCTION_MARKER(0x800DA5F4, "ACE3.BIN");
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        resource002_sub_800dee08(1, 2, 30, 30);
    phase = resource002_sub_800dee84(1);
    if (phase == 2)
    {
        result = r_u32(0x80112DDC);
        w_u8(result + 1, phase);
    }
    return result;
}

uint32 resource002_sub_800d91fc(void)
{
    static const uint16 segments[10][4] = {{130, 40, 150, 40}, {130, 110, 150, 110}, {130, 180, 150, 180}, {130, 40, 130, 180}, {150, 40, 150, 180}, {170, 40, 190, 40}, {170, 110, 190, 110}, {170, 180, 190, 180}, {170, 40, 170, 180}, {190, 40, 190, 180}};
    uint16 line[6] = {0, 0, 0, 0, 0x8080, 0x0080};
    uint32 game, context, descriptor, phase, value, state, object, destination, index, coordinate, packet, ordering, result;
    sint32 axis, positive = 0, negative = 0, offset;
    FUNCTION_MARKER(0x800D91FC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    context = r_u32(0x80112DDC);
    phase = r_u32(context + 252);
    axis = r_s16(game + 100);
    descriptor = context + 266;
    if (phase == 0)
    {
        offset = (axis - 128) * 70 / 128;
        if (axis >= 128)
            positive = offset;
        else
            negative = offset;
        if ((r_u32(r_u32(0x800BCE60) + 300) & 64) && axis >= 145)
        {
            sub_8002db4c(60);
            w_u8(r_u32(0x80112DDC) + 237, (uint32)axis);
            w_u32(r_u32(0x80112DDC) + 252, 1);
        }
    }
    else if (phase == 1)
    {
        positive = ((sint32)r_u8(context + 237) - 128) * 70 / 128;
        if (axis < 128)
            negative = (axis - 128) * 70 / 128;
        if ((r_u32(r_u32(0x800BCE60) + 300) & 64) && axis < 112)
        {
            sub_8002db4c(60);
            w_u8(r_u32(0x80112DDC) + 241, (uint32)axis);
            w_u32(r_u32(0x80112DDC) + 252, 2);
        }
    }
    else if (phase == 2)
    {
        positive = ((sint32)r_u8(context + 237) - 128) * 70 / 128;
        negative = ((sint32)r_u8(context + 241) - 128) * 70 / 128;
        if (r_u32(game + 300) & 64)
        {
            sub_8002db4c(60);
            context = r_u32(0x80112DDC);
            game = r_u32(0x800BCE60);
            value = r_u8(context + 241);
            destination = game + 780;
            w_u8(destination + 4, value);
            value = r_u8(r_u32(0x800BCE60) + 100);
            w_u8(destination + 5, value);
            value = r_u8(r_u32(0x80112DDC) + 237);
            w_u8(destination + 6, value);
            w_u8(r_u32(0x80112DDC), 0);
            w_u8(r_u32(0x80112DDC) + 1, 0);
            resource002_sub_800deca8(2, 0);
            resource002_sub_800debb8(0, 1);
        }
    }
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC0);
    value = r_u8(context + 252);
    object = r_u32(state + 92);
    w_u8(object + 20, value + 74);
    context = r_u32(0x80112DDC);
    for (index = 0; index < 10; ++index)
    {
        state = r_u32(0x80112DC4);
        for (coordinate = 0; coordinate < 4; ++coordinate)
            line[coordinate] = segments[index][coordinate];
        packet = r_u32(context + 8);
        ordering = r_u32(state + 8);
        result = resource002_sub_800f6e68(0, packet, ordering, line);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    w_u16(descriptor, 130);
    w_u16(descriptor + 2, 110);
    w_u16(descriptor + 4, 150);
    w_u16(descriptor + 6, 110);
    w_u16(descriptor + 8, 130);
    w_u16(descriptor + 12, 150);
    w_u8(descriptor + 24, 64);
    w_u8(descriptor + 26, 0);
    w_u8(descriptor + 28, 64);
    w_u8(descriptor + 30, 0);
    w_u16(descriptor + 10, (uint32)(positive + 110));
    w_u16(descriptor + 14, (uint32)(positive + 110));
    w_u8(descriptor + 25, 55);
    w_u8(descriptor + 29, 55);
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC4);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    w_u32(context + 8, result);
    w_u16(descriptor, 130);
    w_u16(descriptor + 2, 110);
    w_u16(descriptor + 4, 150);
    w_u16(descriptor + 6, 110);
    w_u16(descriptor + 8, 130);
    w_u16(descriptor + 12, 150);
    w_u16(descriptor + 10, (uint32)(negative + 110));
    w_u16(descriptor + 14, (uint32)(negative + 110));
    state = r_u32(0x80112DC4);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    w_u32(context + 8, result);
    w_u16(descriptor, 170);
    w_u16(descriptor + 2, 110);
    w_u16(descriptor + 4, 190);
    w_u16(descriptor + 6, 110);
    w_u16(descriptor + 8, 170);
    w_u16(descriptor + 10, (uint32)(110 - negative));
    w_u16(descriptor + 14, (uint32)(110 - negative));
    state = r_u32(0x80112DC4);
    w_u16(descriptor + 12, 190);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    w_u32(context + 8, result);
    w_u16(descriptor, 170);
    w_u16(descriptor + 2, 110);
    w_u16(descriptor + 4, 190);
    w_u16(descriptor + 6, 110);
    w_u16(descriptor + 8, 170);
    w_u16(descriptor + 10, (uint32)(110 - positive));
    w_u16(descriptor + 14, (uint32)(110 - positive));
    state = r_u32(0x80112DC4);
    w_u16(descriptor + 12, 190);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    game = r_u32(0x800BCE60);
    w_u32(context + 8, result);
    offset = ((sint32)r_s16(game + 100) - 128) * 70 / 128;
    w_u16(descriptor + 2, (uint32)(offset + 110));
    w_u16(descriptor + 6, (uint32)(offset + 100));
    w_u16(descriptor + 10, (uint32)(offset + 120));
    state = r_u32(0x80112DC4);
    w_u16(descriptor, 130);
    w_u16(descriptor + 4, 120);
    w_u16(descriptor + 8, 120);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f73a4(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    w_u32(context + 8, result);
    w_u16(descriptor + 2, (uint32)(110 - offset));
    w_u16(descriptor + 6, (uint32)(100 - offset));
    w_u16(descriptor + 10, (uint32)(120 - offset));
    state = r_u32(0x80112DC4);
    w_u16(descriptor, 190);
    w_u16(descriptor + 4, 200);
    w_u16(descriptor + 8, 200);
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f73a4(descriptor, packet, ordering, NULL);
    w_u32(r_u32(0x80112DDC) + 8, result);
    return result;
}

uint32 resource002_sub_800f6e68(uint32 descriptor, uint32 packet, uint32 ordering, uint16 *native_descriptor)
{
    uint32 first, second;
    FUNCTION_MARKER(0x800F6E68, "ACE3.BIN");
    packet = resource002_sub_800f6da4(descriptor, packet, ordering, (const uint8 *)native_descriptor);
    first = native_descriptor ? native_descriptor[1] : r_u16(descriptor + 2);
    second = native_descriptor ? native_descriptor[3] : r_u16(descriptor + 6);
    ++first;
    ++second;
    if (native_descriptor)
        native_descriptor[1] = (uint16)first;
    else
        w_u16(descriptor + 2, first);
    if (native_descriptor)
        native_descriptor[3] = (uint16)second;
    else
        w_u16(descriptor + 6, second);
    return resource002_sub_800f6da4(descriptor, packet, ordering, (const uint8 *)native_descriptor);
}

uint32 resource002_sub_800f73a4(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    static const uint8 coordinates[] = {8, 10, 16, 18, 24};
    uint32 value, group, channel, index, tag, link;
    FUNCTION_MARKER(0x800F73A4, "ACE3.BIN");
    w_u8(packet + 3, 6);
    w_u8(packet + 7, 0x30);
    value = native_descriptor ? native_descriptor[19] : r_u8(descriptor + 19);
    w_u8(packet + 7, value ? 0x32 : 0x30);
    for (group = 0; group < 3; ++group)
    {
        for (channel = 0; channel < 3; ++channel)
        {
            value = native_descriptor ? native_descriptor[16 + group * 4 + channel] : r_u8(descriptor + 16 + group * 4 + channel);
            w_u8(packet + 4 + group * 8 + channel, value);
        }
    }
    for (index = 0; index < 5; ++index)
    {
        value = native_descriptor ? xport_load_le16(native_descriptor + index * 2) : r_u16(descriptor + index * 2);
        w_u16(packet + coordinates[index], value);
    }
    value = native_descriptor ? xport_load_le16(native_descriptor + 10) : r_u16(descriptor + 10);
    tag = r_u32(packet);
    w_u16(packet + 26, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 28;
}

uint32 resource002_sub_800d9994(void)
{
    static const uint8 limits[8][2] = {{242, 239}, {238, 239}, {238, 243}, {242, 243}, {240, 237}, {236, 237}, {236, 241}, {240, 241}};
    static const uint16 outlines[4][8] = {{105, 40, 140, 110, 105, 180, 70, 110}, {105, 180, 105, 40, 70, 110, 140, 110}, {215, 40, 250, 110, 215, 180, 180, 110}, {215, 180, 215, 40, 180, 110, 250, 110}};
    uint32 context, game, descriptor, state, packet, ordering, result, index, channel, group, phase, value, raw, destination, object;
    uint32 outline[6] = {0, 0, 0, 0, 0x00808080, 0};
    sint32 horizontal, vertical, axis;
    FUNCTION_MARKER(0x800D9994, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    descriptor = context + 266;
    for (group = 0; group < 4; ++group)
        for (channel = 0; channel < 3; ++channel)
            w_u8(descriptor + 16 + group * 4 + channel, 128);
    game = r_u32(0x800BCE60);
    horizontal = ((sint32)r_s16(game + 104) - 128) * 35 / 128;
    vertical = ((sint32)r_s16(game + 102) - 128) * 70 / 128;
    w_u16(descriptor, (uint32)(horizontal + 105));
    w_u16(descriptor + 2, (uint32)(vertical + 104));
    w_u16(descriptor + 4, (uint32)(horizontal + 108));
    w_u16(descriptor + 8, (uint32)(horizontal + 102));
    context = r_u32(0x80112DDC);
    w_u16(descriptor + 14, (uint32)(vertical + 116));
    state = r_u32(0x80112DC4);
    w_u16(descriptor + 6, (uint32)(vertical + 110));
    w_u16(descriptor + 10, (uint32)(vertical + 110));
    w_u16(descriptor + 12, (uint32)(horizontal + 105));
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    context = r_u32(0x80112DDC);
    game = r_u32(0x800BCE60);
    w_u32(context + 8, result);
    horizontal = ((sint32)r_s16(game + 100) - 128) * 35 / 128;
    vertical = ((sint32)r_s16(game + 98) - 128) * 70 / 128;
    w_u16(descriptor, (uint32)(horizontal + 215));
    w_u16(descriptor + 2, (uint32)(vertical + 104));
    w_u16(descriptor + 4, (uint32)(horizontal + 218));
    w_u16(descriptor + 8, (uint32)(horizontal + 212));
    state = r_u32(0x80112DC4);
    w_u16(descriptor + 6, (uint32)(vertical + 110));
    w_u16(descriptor + 10, (uint32)(vertical + 110));
    w_u16(descriptor + 12, (uint32)(horizontal + 215));
    w_u16(descriptor + 14, (uint32)(vertical + 116));
    packet = r_u32(context + 8);
    ordering = r_u32(state + 8);
    result = resource002_sub_800f7244(descriptor, packet, ordering, NULL);
    w_u32(r_u32(0x80112DDC) + 8, result);
    for (index = 0; index < 4; ++index)
    {
        game = r_u32(0x800BCE60);
        context = r_u32(0x80112DDC);
        axis = r_s16(game + index * 2 + 98);
        value = r_u8(context + index + 240);
        raw = r_u16(game + index * 2 + 98);
        if (axis < (sint32)value)
            w_u8(context + index + 240, raw);
        context = r_u32(0x80112DDC);
        value = r_u8(context + index + 236);
        if ((sint32)value < axis)
            w_u8(context + index + 236, raw);
    }
    w_u8(descriptor + 16, 240);
    w_u8(descriptor + 17, 110);
    w_u8(descriptor + 18, 0);
    w_u8(descriptor + 20, 64);
    w_u8(descriptor + 21, 55);
    w_u8(descriptor + 22, 0);
    w_u8(descriptor + 24, 64);
    w_u8(descriptor + 25, 55);
    w_u8(descriptor + 26, 0);
    context = r_u32(0x80112DDC);
    for (index = 0; index < 8; ++index)
    {
        value = index < 4 ? 105 : 215;
        w_u16(descriptor, value);
        w_u16(descriptor + 2, 110);
        w_u16(descriptor + 4, value);
        value = r_u8(context + limits[index][0]);
        w_u16(descriptor + 6, value * 140 / 256 + 40);
        value = r_u8(context + limits[index][1]);
        w_u16(descriptor + 8, value * 70 / 256 + (index < 4 ? 70 : 180));
        if (index == 4)
        {
            w_u16(descriptor + 10, 110);
            context = r_u32(0x80112DDC);
            state = r_u32(0x80112DC4);
        }
        else
        {
            state = r_u32(0x80112DC4);
            w_u16(descriptor + 10, 110);
        }
        packet = r_u32(context + 8);
        ordering = r_u32(state + 8);
        result = resource002_sub_800f73a4(descriptor, packet, ordering, NULL);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    phase = 0;
    for (index = 0; index < 4; ++index)
    {
        for (channel = 0; channel < 4; ++channel)
            outline[channel] = outlines[index][channel * 2] | ((uint32)outlines[index][channel * 2 + 1] << 16);
        state = r_u32(0x80112DC4);
        packet = r_u32(context + 8);
        ordering = r_u32(state + 8);
        result = resource002_sub_800f711c(0, packet, ordering, outline);
        context = r_u32(0x80112DDC);
        if (index == 3)
            phase = r_u32(context + 252);
        w_u32(context + 8, result);
    }
    switch (phase)
    {
        case 0:
            context = r_u32(0x80112DDC);
            if (r_u8(context + 240) < 64 && r_u8(context + 242) < 64)
                w_u32(context + 252, 1);
            break;
        case 1:
            context = r_u32(0x80112DDC);
            if (r_u8(context + 236) >= 193 && r_u8(context + 238) >= 193)
                w_u32(context + 252, 2);
            break;
        case 2:
            context = r_u32(0x80112DDC);
            if (r_u8(context + 241) < 64 && r_u8(context + 243) < 64)
                w_u32(context + 252, 3);
            break;
        case 3:
            context = r_u32(0x80112DDC);
            if (r_u8(context + 237) >= 193 && r_u8(context + 239) >= 193)
                w_u32(context + 252, 4);
            break;
        case 4:
            if (r_u32(r_u32(0x800BCE60) + 300) & 64)
            {
                sub_8002db4c(60);
                destination = r_u32(0x800BCE60) + 688;
                for (index = 0; index < 4; ++index, destination += 4)
                {
                    value = r_u8(r_u32(0x80112DDC) + index + 240);
                    w_u8(destination, value);
                    game = r_u32(0x800BCE60);
                    value = r_u8(game + index * 2 + 98);
                    w_u8(destination + 1, value);
                    value = r_u8(r_u32(0x80112DDC) + index + 236);
                    w_u8(destination + 2, value);
                }
                w_u8(r_u32(0x80112DDC), 0);
                w_u8(r_u32(0x80112DDC) + 1, 0);
                resource002_sub_800deca8(2, 0);
                resource002_sub_800debb8(0, 1);
            }
            break;
        default:
            break;
    }
    context = r_u32(0x80112DDC);
    state = r_u32(0x80112DC0);
    value = r_u8(context + 252);
    object = r_u32(state + 92);
    result = value + 69;
    w_u8(object + 20, result);
    return result;
}

uint32 resource002_sub_800da66c(void)
{
    FUNCTION_MARKER(0x800DA66C, "ACE3.BIN");
    return resource002_sub_800da674(r_u32(0x800BCE60));
}

uint32 resource002_sub_800da674(uint32 game)
{
    uint32 context, value, result;
    FUNCTION_MARKER(0x800DA674, "ACE3.BIN");
    if (r_u32(game + 304) & 2)
    {
        context = r_u32(0x80112DDC);
        value = r_u8(context + 6);
#if 0
        w_u8(context + 6, value < 3 ? value + 1 : 0);
#else
        w_u8(context + 6, value == 0 ? 3 : 0);
#endif
        sub_8002db4c(61);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        context = r_u32(0x80112DDC);
        value = r_u8(context + 6);
#if 0
        w_u8(context + 6, value ? value - 1 : 3);
#else
        w_u8(context + 6, value == 0 ? 3 : 0);
#endif
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        resource002_sub_800dee08(1, 3, -1, -1);
        w_u8(r_u32(0x80112DDC) + 1, 3);
        w_u8(r_u32(0x80112DDC) + 3, 1);
        sub_8002db4c(60);
        sub_8002db4c(91);
    }
    value = r_u8(r_u32(0x80112DDC) + 6);
    resource002_sub_800def94(5, value);
    resource002_sub_800def94(6, 0);
    if (r_u32(r_u32(0x800BCE60) + 296) & 1)
        resource002_sub_800def94(6, 1);
    if (r_u32(r_u32(0x800BCE60) + 296) & 2)
        resource002_sub_800def94(6, 2);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            resource002_sub_800dee08(1, 3, -1, -1);
            w_u8(r_u32(0x80112DDC) + 1, 3);
            w_u8(r_u32(0x80112DDC) + 3, 2);
            sub_8002db4c(59);
            result = r_u32(0x800BCE60);
            w_u32(result + 300, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800da870(void)
{
    FUNCTION_MARKER(0x800DA870, "ACE3.BIN");
    return resource002_sub_800da878(r_u32(0x800BCE60));
}

uint32 resource002_sub_800da878(uint32 game)
{
#if 0
    uint32 phase, context, action, choice, result, state, value, index;
#else
    uint32 phase, context, action, choice, result, index;
#endif
    FUNCTION_MARKER(0x800DA878, "ACE3.BIN");
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        resource002_sub_800dee08(1, 0, 0, 0);
    phase = resource002_sub_800dee84(1);
    if (phase)
        return phase;
    context = r_u32(0x80112DDC);
    action = r_u8(context + 3);
    if (action == 1)
    {
        choice = r_u8(context + 6);
        switch (choice)
        {
            case 0:
                w_u8(context + 1, 4);
                resource002_sub_800deca8(2, 13);
                resource002_sub_800dedf0(2, 8192);
                result = 0;
                for (index = 304; index <= 320; index += 4)
                    result = sub_8002dda8(r_u32(r_u32(0x80112DC4) + index));
                return result;
#if 0
            case 1:
                w_u8(context + 1, 5);
                resource002_sub_800deca8(2, 14);
                return resource002_sub_800dedf0(2, 16384);
            case 2:
                w_u8(context + 1, 6);
                resource002_sub_800deca8(2, 15);
                resource002_sub_800dedf0(0, 0);
                resource002_sub_800dedf0(2, 32768);
                state = r_u32(0x800BCE48);
                context = r_u32(0x80112DDC);
                value = r_u16(state + 8);
                w_u16(context + 262, value);
                result = r_u16(state + 12);
                w_u16(context + 264, result);
                return result;
#endif
            case 3:
                w_u8(context + 1, 7);
                return 7;
            default:
                return 7;
        }
    }
    if (action == 2)
    {
        resource002_sub_800debb8(0, 1);
        w_u8(r_u32(0x80112DDC), 0);
        result = r_u32(0x80112DDC);
        w_u8(result + 1, 0);
        return result;
    }
    return 2;
}
