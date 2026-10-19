#include "game_functions.h"
#include <stdlib.h>

uint32 resource002_sub_800ea2e8(void)
{
    uint32 index, value, context;
    FUNCTION_MARKER(0x800EA2E8, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3328);
    resource002_sub_800deca8(1, 8);
    resource002_sub_800deca8(1, 11);
    resource002_sub_800dee08(1, 1, 0, 20);
    resource002_sub_800dee08(15, 1, 0, 20);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 62);
    w_u8(r_u32(0x80112DF8) + 9, 1);
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800ea3f4(void)
{
    uint32 phase, status, index, value, context, game;
    FUNCTION_MARKER(0x800EA3F4, "ACE3.BIN");
    phase = resource002_sub_800dee84(1);
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 30, 0);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 3);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (!status)
        return 0;
    if (phase == 2)
    {
        if (r_u32(r_u32(0x800BCE60) + 304) & 8)
        {
            context = r_u32(0x80112DF8);
            value = r_u8(context + 6);
            w_u8(context + 6, value < 5 ? value + 1 : 0);
            sub_8002db4c(61);
        }
        if (r_u32(r_u32(0x800BCE60) + 304) & 4)
        {
            context = r_u32(0x80112DF8);
            value = r_u8(context + 6);
            w_u8(context + 6, value ? value - 1 : 5);
            sub_8002db4c(61);
        }
    }
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(1, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 14);
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
    if (((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 2)
            {
                resource002_sub_800dee08(1, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 0);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            sub_8002db4c(59);
        }
    }
    return 0;
}

uint32 resource002_sub_800e6c4c(uint32 slot, uint32 account, uint32 hide_data)
{
    uint32 present, value, text;
    FUNCTION_MARKER(0x800E6C4C, "ACE3.BIN");
    present = resource002_sub_800e458c();
    resource002_sub_800f35b8(4);
    resource002_sub_800f33f8(2);
    resource002_sub_800f34c8(1, 10, 20);
    value = resource002_sub_800deea8(15);
    resource002_sub_800f34b0(value);
    resource002_sub_800f33d0(128, 128, 128);
    resource002_sub_800f35b8(3);
    slot &= 255;
    if (hide_data == 1 || !(present & (1u << (slot & 31))))
    {
        resource002_sub_800f33a8(140, 331);
        return resource002_sub_800f4ae0(0x800CCE48, 0, NULL, 0);
    }
    resource002_sub_800f33a8(94, 294);
    text = resource002_sub_800e44e0(slot);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    resource002_sub_800f33a8(94, 317);
    resource002_sub_800f4ae0(0x800CCE50, 0, NULL, 0);
    resource002_sub_800f33a8(140, 317);
    text = resource002_sub_800e435c(slot);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    resource002_sub_800f33a8(94, 334);
    resource002_sub_800f4ae0(0x800CCE58, 0, NULL, 0);
    resource002_sub_800f33a8(140, 334);
    text = resource002_sub_800e43b4(slot);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    resource002_sub_800f33a8(94, 352);
    resource002_sub_800f4ae0(0x800CCE60, 0, NULL, 0);
    resource002_sub_800f33a8(140, 353);
    text = resource002_sub_800e4518(slot);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    resource002_sub_800f33a8(94, 369);
    resource002_sub_800f4ae0(0x800CCE68, 0, NULL, 0);
    resource002_sub_800f33a8(140, 369);
    text = resource002_sub_800e45ac(account & 255);
    return resource002_sub_800f4ae0(text, 0, NULL, 0);
}

uint32 resource002_sub_800e458c(void)
{
    FUNCTION_MARKER(0x800E458C, "ACE3.BIN");
    return (uint32)r_s16(r_u32(r_u32(0x80112DF0) + 760) + 22);
}

uint32 resource002_sub_800e44e0(uint32 slot)
{
    uint32 mission, index;
    FUNCTION_MARKER(0x800E44E0, "ACE3.BIN");
    mission = r_u32(r_u32(0x80112DF0) + (slot & 255) * 4 + 764);
    index = r_u8(mission + 275);
    return r_u32(0x8010B06C + index * 4);
}

uint32 resource002_sub_800e435c(uint32 slot)
{
    uint32 mission, index;
    FUNCTION_MARKER(0x800E435C, "ACE3.BIN");
    mission = r_u32(r_u32(0x80112DF0) + (slot & 255) * 4 + 764);
    index = r_u8(mission + 274);
    return r_u32(0x8010B1B8 + index * 4);
}

uint32 resource002_sub_800e43b4(uint32 slot)
{
    uint32 mission, frames, arguments[3], index;
    sint32 duration;
    FUNCTION_MARKER(0x800E43B4, "ACE3.BIN");
    mission = r_u32(r_u32(0x80112DF0) + (slot & 255) * 4 + 764);
    duration = r_s32(mission + 276);
    if (duration > 107999999)
        duration = 107999999;
    if (duration < 0)
        duration = 0;
    frames = (uint32)duration;
    arguments[0] = frames / 108000;
    arguments[1] = frames / 1800 - 60 * arguments[0];
    arguments[2] = frames / 30 - 60 * (frames / 1800);
    game_sprintf_guest_words(0x80112DE0, 0x800CCDDC, arguments, 3);
    for (index = 0; index < 12; ++index)
        if (r_u8(0x80112DE0 + index) == 32)
            w_u8(0x80112DE0 + index, 48);
    return 0x80112DE0;
}

uint32 resource002_sub_800e4518(uint32 slot)
{
    uint32 mission, index;
    FUNCTION_MARKER(0x800E4518, "ACE3.BIN");
    mission = r_u32(r_u32(0x80112DF0) + (slot & 255) * 4 + 764);
    index = r_u8(mission + 275);
    index = r_u8(0x8010B178 + index);
    return r_u32(0x8010B160 + index * 4);
}

uint32 resource002_sub_800e45ac(uint32 account)
{
    uint32 pointer, index;
    FUNCTION_MARKER(0x800E45AC, "ACE3.BIN");
    pointer = r_u32(r_u32(0x80112DF0) + account * 4 + 748);
    index = r_u8(pointer + 1124);
    return r_u32(0x8010B1C8 + index * 4);
}

uint32 resource002_sub_800ea744(void)
{
    uint32 context, mode, result = 0, value, index;
    FUNCTION_MARKER(0x800EA744, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    w_u8(r_u32(0x80112DF8) + 3, 0);
    context = r_u32(0x80112DF8);
    mode = r_u8(context + 10);
    if (mode == 0)
        result = resource002_sub_800e42ac(r_u8(context + 5), r_u8(context + 6));
    else if (mode == 1)
        result = resource002_sub_800e42ac(0xFFFFFFFFu, r_u8(context + 6));
    if (result == 0 || result == 1)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + (result == 1 ? 2 : 1));
        resource002_sub_800dee08(2, 1, 0, 20);
    }
    resource002_sub_800dee08(4, 1, 0, 20);
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    return 0;
}

uint32 resource002_sub_800e42ac(uint32 account, uint32 slot)
{
    uint32 pointer;
    FUNCTION_MARKER(0x800E42AC, "ACE3.BIN");
    if (account == 0xFFFFFFFFu)
        pointer = r_u32(r_u32(0x80112DF0) + 760);
    else
        pointer = r_u32(r_u32(0x80112DF0) + account * 4 + 748);
    return (uint32)(r_s16(pointer + 22) >> (slot & 31)) & 1;
}

uint32 resource002_sub_800ea8ac(void)
{
    uint32 status, phase, context, index, value, object, game;
    FUNCTION_MARKER(0x800EA8AC, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    phase = resource002_sub_800dee84(2);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 63);
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6);
        if (value == index)
            value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 3 : 0;
        else
            value = 1;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    context = r_u32(0x80112DF8);
    object = r_u32(0x80112DC0);
    value = r_u8(context + 6);
    object = r_u32(object + 92);
    w_u8(object + 9, value + 1);
    context = r_u32(0x80112DF8);
    object = r_u32(0x80112DC0);
    value = r_u8(context + 6);
    object = r_u32(object + 92);
    w_u8(object + 30, value + 1);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 3);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (!status)
        return 0;
    if ((r_u32(r_u32(0x800BCE60) + 300) & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(2, 0, 0, 0);
                resource002_sub_800dee08(4, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 15);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
            sub_8002db4c(60);
        }
    }
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 2)
            {
                resource002_sub_800dee08(2, 0, 0, 0);
                resource002_sub_800dee08(4, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 13);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            sub_8002db4c(59);
        }
    }
    return 0;
}

uint32 resource002_sub_800eac04(void)
{
    uint32 phase, status, context, object, value, index, game, result, pressed;
    FUNCTION_MARKER(0x800EAC04, "ACE3.BIN");
    phase = resource002_sub_800dee84(2);
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 64);
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6);
        if (value == index)
            value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 3 : 0;
        else
            value = 1;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    context = r_u32(0x80112DF8);
    object = r_u32(0x80112DC0);
    value = r_u8(context + 6);
    object = r_u32(object + 92);
    w_u8(object + 9, value + 1);
    context = r_u32(0x80112DF8);
    object = r_u32(0x80112DC0);
    value = r_u8(context + 6);
    object = r_u32(object + 92);
    w_u8(object + 30, value + 1);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 3);
        context = r_u32(0x80112DF8);
        w_u8(context + 1, 0);
        return context;
    }
    if (!status)
        return 6;
    if ((r_u32(r_u32(0x800BCE60) + 300) & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(2, 0, 0, 0);
                resource002_sub_800dee08(4, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 15);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
            sub_8002db4c(60);
        }
    }
    game = r_u32(0x800BCE60);
    pressed = r_u32(game + 300) & 32;
    result = pressed;
    if (result)
        result = r_u32(game + 296) & 0x5F;
    if ((pressed && result == 0) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return sub_8002db4c(59);
        }
        if (r_u8(r_u32(0x80112DF8) + 3) != 2)
            return 2;
        resource002_sub_800dee08(2, 0, 0, 0);
        resource002_sub_800dee08(4, 0, 0, 0);
        w_u8(r_u32(0x80112DF8), 13);
        context = r_u32(0x80112DF8);
        w_u8(context + 1, 0);
        return context;
    }
    return result;
}

uint32 resource002_sub_800eaf44(void)
{
    uint32 present, context, value, index;
    FUNCTION_MARKER(0x800EAF44, "ACE3.BIN");
    present = resource002_sub_800e458c();
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 65);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 30, 0);
    resource002_sub_800e2ab4();
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    context = r_u32(0x80112DF8);
    value = r_u8(context + 6);
    w_u8(context + 10, (((sint32)present >> (value & 31)) & 1) == 0);
    return 1;
}

uint32 resource002_sub_800eb064(void)
{
    uint32 context, status, value, index;
    FUNCTION_MARKER(0x800EB064, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    context = r_u32(0x80112DF8);
    status = resource002_sub_800e5a28(r_u8(context + 4), r_u8(context + 5), r_u8(context + 6), 1);
    if (status == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 2);
    }
    if (status == 1)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        resource002_sub_800e2f44();
    }
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    return 0;
}

uint32 resource002_sub_800eb174(void)
{
    uint32 context, status, value, index;
    FUNCTION_MARKER(0x800EB174, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6c4c(r_u8(context + 6), r_u8(context + 5), r_u8(context + 10));
    context = r_u32(0x80112DF8);
    status = resource002_sub_800e5ca4(r_u8(context + 4), r_u8(context + 5), r_u8(context + 6), 1);
    if (status == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
    }
    if (status == 1)
    {
        w_u8(r_u32(0x80112DF8), 16);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    for (index = 0; index < 6; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 6) != index;
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 2, value);
    }
    return 0;
}

uint32 resource002_sub_800eb280(void)
{
    uint32 status;
    FUNCTION_MARKER(0x800EB280, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    resource002_sub_800dee08(1, 2, 20, 20);
    resource002_sub_800dedf0(1, 3200);
    resource002_sub_800deca8(1, 7);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (status)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 67);
        if (r_u32(r_u32(0x800BCE60) + 300))
        {
            resource002_sub_800dee08(1, 0, 0, 0);
            w_u8(r_u32(0x80112DF8), 0);
            w_u8(r_u32(0x80112DF8) + 1, 0);
            sub_8002db4c(59);
        }
    }
    return 1;
}

uint32 resource002_sub_800eb388(void)
{
    uint32 context, game, value;
    FUNCTION_MARKER(0x800EB388, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3584);
    resource002_sub_800deca8(1, 9);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    w_u8(r_u32(0x80112DF8) + 7, 0);
    w_u8(r_u32(0x80112DF8) + 9, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 68);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 30, 0);
    context = r_u32(0x80112DF8);
    game = r_u32(0x800BCE60);
    value = r_u8(context + 6);
    w_u8(game + 2397, value);
    return value;
}

uint32 resource002_sub_800eb42c(void)
{
    uint32 text, context, value;
    FUNCTION_MARKER(0x800EB42C, "ACE3.BIN");
    resource002_sub_800f33a8(74, 214);
    text = resource002_sub_800e425c(0);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    if (r_u8(r_u32(0x80112DF8) + 7) >= 121 || r_u32(r_u32(0x800BCE60) + 300))
    {
        value = resource002_sub_800e69a8(r_u32(r_u32(0x80112DC4) + 224));
        w_u8(r_u32(0x80112DF8), value);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        w_u8(r_u32(0x80112DF8) + 7, 0);
    }
    context = r_u32(0x80112DF8);
    value = r_u8(context + 7) + 1;
    w_u8(context + 7, value);
    return value;
}

uint32 resource002_sub_800eb4f4(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800EB4F4, "ACE3.BIN");
    w_u8(r_u32(0x80112DF8) + 8, 1);
    resource002_sub_800dedf0(1, 3073);
    resource002_sub_800deca8(1, 0);
    resource002_sub_800dee08(2, 1, 0, 20);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 69);
    return 69;
}

uint32 resource002_sub_800eb57c(void)
{
    uint32 phase, game, context, result, pressed;
    FUNCTION_MARKER(0x800EB57C, "ACE3.BIN");
    phase = resource002_sub_800dee84(2);
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(2, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 18);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
            sub_8002db4c(60);
        }
    }
    game = r_u32(0x800BCE60);
    pressed = r_u32(game + 300) & 32;
    result = pressed;
    if (pressed)
        result = r_u32(game + 296) & 0x5F;
    if ((pressed && result == 0) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return sub_8002db4c(59);
        }
        if (r_u8(r_u32(0x80112DF8) + 3) != 2)
            return 2;
        resource002_sub_800dee08(2, 0, 0, 0);
        context = r_u32(0x80112DF8);
        w_u8(context + 2, 2);
        game = r_u32(0x800BCE60);
        result = r_u32(game + 2412) | 2;
        w_u32(game + 2412, result);
    }
    return result;
}

uint32 resource002_sub_800eb73c(void)
{
    uint32 result, media, kind, context, game, value, object;
    FUNCTION_MARKER(0x800EB73C, "ACE3.BIN");
    w_u8(r_u32(0x800BCE60) + 2398, 0);
    result = resource002_sub_800e264c();
    if (!result)
        return result;
    media = (uint32)cd_resource_state();
    if (!media)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 73);
        return 73;
    }
    game = r_u32(0x800BCE60);
    kind = r_u8(0x80112750 + (uint32)r_s16(game + 908));
    if (kind == 3 || media == 3)
    {
        w_u8(r_u32(0x80112DF8) + 2, 1);
        return 1;
    }
    if (media == 4 || media == 0xFFFFFFFFu)
    {
        if (kind == 1)
        {
            context = r_u32(0x80112DF8);
            object = r_u32(0x80112DC0);
            value = r_u8(context + 24);
            object = r_u32(object + 92);
            w_u8(object + 1, value + 70);
        }
        game = r_u32(0x800BCE60);
        kind = r_u8(0x80112750 + (uint32)r_s16(game + 908));
        if (kind == 2)
        {
            context = r_u32(0x80112DF8);
            object = r_u32(0x80112DC0);
            value = r_u8(context + 24);
            object = r_u32(object + 92);
            w_u8(object + 1, value + 71);
        }
        sub_8002b428();
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u16(context + 24, 31);
        result = value + 1;
        w_u8(context + 1, result);
        return result;
    }
    if (media == 1)
    {
        if (kind == 1)
        {
            context = r_u32(0x80112DF8);
            w_u8(context + 2, 1);
            return context;
        }
        context = r_u32(0x80112DF8);
        object = r_u32(0x80112DC0);
        value = r_u8(context + 24);
        object = r_u32(object + 92);
        w_u8(object + 1, value + 71);
        sub_8002b428();
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u16(context + 24, 31);
        w_u8(context + 1, value + 1);
        return 2;
    }
    if (media == 2)
    {
        game = r_u32(0x800BCE60);
        kind = r_u8(0x80112750 + (uint32)r_s16(game + 908));
        if (kind == 2)
        {
            w_u8(r_u32(0x80112DF8) + 2, 1);
            return 1;
        }
        context = r_u32(0x80112DF8);
        object = r_u32(0x80112DC0);
        value = r_u8(context + 24);
        object = r_u32(object + 92);
        w_u8(object + 1, value + 70);
        sub_8002b428();
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u16(context + 24, 31);
        result = value + 1;
        w_u8(context + 1, result);
        return result;
    }
    return 2;
}

uint32 resource002_sub_800eb944(void)
{
    uint32 result, context;
    FUNCTION_MARKER(0x800EB944, "ACE3.BIN");
    result = sub_8002b528();
    if (result == 3)
    {
        context = r_u32(0x80112DF8);
        result = r_u8(context + 1) + 1;
        w_u8(context + 1, result);
    }
    return result;
}

uint32 resource002_sub_800eb98c(void)
{
    uint32 result, context;
    FUNCTION_MARKER(0x800EB98C, "ACE3.BIN");
    result = sub_8002b528();
    if (result == 1)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 73);
        context = r_u32(0x80112DF8);
        result = r_u8(context + 1) - 2;
        w_u8(context + 1, result);
    }
    return result;
}

uint32 resource002_sub_800e5ca4(uint32 port, uint32 account, uint32 slot, uint32 finalize)
{
    uint32 state, buffer, checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5CA4, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x80112DF0);
    }
    if (!r_u8(state + 822))
    {
        checksum = 0x41434533;
        buffer = r_u32(state + 788);
        for (index = 0; index < 95; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x80112DF0);
        buffer = r_u32(state + 788);
        w_u32(buffer, checksum);
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8002, 26);
        state = r_u32(0x80112DF0);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            goto failure;
        }
        game_bios_seek_guest(descriptor, slot * 384 + account * 2304 + 4224, 0);
        state = r_u32(0x80112DF0);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + 788);
        game_bios_write_guest(descriptor, buffer, 384);
        w_u8(r_u32(0x80112DF0) + 822, 1);
    }
    event = resource002_sub_800e2794(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(r_u32(r_u32(0x80112DF0) + 60));
    w_u8(r_u32(0x80112DF0) + 822, 0);
    if (event)
    {
        state = r_u32(0x80112DF0);
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        goto failure;
    }
    if (finalize == 1)
        resource002_sub_800e25d4();
    return 1;
failure:
    resource002_sub_800e25d4();
    w_u16(r_u32(0x80112DF0) + 858, 0);
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e2f44(void)
{
    uint32 statistics, mission, game, value, index, low, high;
    FUNCTION_MARKER(0x800E2F44, "ACE3.BIN");
    statistics = r_u32(0x800BCE60) + 2396;
    mission = r_u32(r_u32(0x80112DF0) + 788);
#ifndef NDEBUG
    printf(game_native_string(0x800CCD1C), mission);
#endif
    value = r_u32(statistics + 1744);
    game = r_u32(0x800BCE60);
    w_u32(mission + 276, value);
    value = r_u8(game + 908);
    w_u8(mission + 275, value);
    value = r_u8(r_u32(0x800BCE60) + 916);
    w_u8(mission + 274, value);
    for (index = 0; index < 60; ++index)
    {
        value = r_u16(statistics + 584 + index * 2);
        w_u16(mission + 4 + index * 2, value);
        value = r_u8(statistics + 1264 + index * 4);
        w_u8(mission + 154 + index, value);
        value = r_u8(statistics + 1504 + index * 4);
        w_u8(mission + 214 + index, value);
        value = r_u8(r_u32(0x800BCE60) + 910);
        w_u8(mission + 280, value);
        value = r_u8(r_u32(0x800BCE60) + 937);
        w_u8(mission + 281, value);
        value = r_u8(r_u32(0x800BCE60) + 938);
        w_u8(mission + 282, value);
        value = r_u8(r_u32(0x800BCE60) + 939);
        w_u8(mission + 283, value);
    }
    for (index = 0; index < 30; ++index)
    {
        low = r_u8(statistics + 944 + index * 2);
        high = r_u8(statistics + 945 + index * 2);
        w_u8(mission + 124 + index, (low & 15) | ((high & 15) << 4));
    }
    for (index = 100; index > 0; --index)
        w_u8(mission + 283 + index, 0);
    return 0;
}

uint32 resource002_sub_800dce0c(void)
{
    uint32 owner, bank, column, row, first, second, x, y, index, context, value;
    void *command;
    FUNCTION_MARKER(0x800DCE0C, "ACE3.BIN");
    owner = arena_alloc(9148);
    w_u32(0x80112DD8, owner);
    w_u8(owner + 9144, 2);
    context = r_u32(0x80112DD8);
    w_u16(owner + 9142, 0);
    w_u8(context + 9145, 0);
    resource002_sub_800f7514(0x8010A5EC);
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DD8);
        first = owner + bank * 2160;
        second = owner + bank * 2160 + 4320;
        for (column = 0; column < 18; ++column)
        {
            x = 156 + column * 8;
            for (row = 0; row < 6; ++row)
            {
                y = 350 - row * 16;
                w_u8(first + 3, 4);
                w_u8(first + 7, 102);
                w_u8(first + 4, 128);
                w_u8(first + 5, 128);
                w_u8(first + 6, 128);
                w_u16(first + 8, x);
                w_u16(first + 10, y);
                w_u16(first + 16, 8);
                w_u16(first + 18, 16);
                w_u8(first + 12, row < 3 ? 220 : 228);
                w_u8(first + 13, row < 3 ? 36 : 20);
                w_u16(first + 14, row < 3 ? 3803 : 3867);
                first += 20;
                w_u8(second + 12, 236);
                w_u8(second + 13, 24);
                w_u8(second + 3, 4);
                w_u8(second + 7, 102);
                w_u8(second + 4, 128);
                w_u8(second + 5, 128);
                w_u8(second + 6, 128);
                w_u16(second + 8, x);
                w_u16(second + 10, y);
                w_u16(second + 16, 8);
                w_u16(second + 18, 16);
                w_u16(second + 14, 3739);
                second += 20;
            }
        }
        command = psx_addr(r_u32(0x80112DD8) + 8640 + bank * 12, 12);
        SetDrawMode(command, 0, 0, 29, NULL);
        command = psx_addr(r_u32(0x80112DD8) + 8664 + bank * 12, 12);
        SetDrawMode(command, 0, 0, 93, NULL);
    }
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DD8);
        first = owner + 8688 + bank * 100;
        for (index = 0; index < 5; ++index)
        {
            w_u8(first + 3, 4);
            w_u8(first + 7, 102);
            w_u8(first + 4, 128);
            w_u8(first + 5, 128);
            w_u8(first + 6, 128);
            w_u16(first + 16, 76);
            w_u16(first + 18, 84);
            w_u8(first + 12, 0);
            w_u8(first + 13, 0);
            w_u16(first + 14, 3419);
            first += 20;
        }
        command = psx_addr(r_u32(0x80112DD8) + 9088 + bank * 12, 12);
        SetDrawMode(command, 0, 0, 93, NULL);
    }
    owner = r_u32(0x80112DD8);
    w_u16(owner + 9132, 32);
    w_u16(owner + 9122, 4);
    w_u16(owner + 9112, 4);
    w_u16(owner + 9124, 4);
    w_u16(owner + 9114, 4);
    w_u16(owner + 9134, 0xFFD0);
    w_u16(owner + 9136, 56);
    w_u16(owner + 9138, 0xFFE8);
    w_u16(owner + 9130, 8);
    w_u16(owner + 9120, 8);
    w_u16(owner + 9126, 12);
    w_u16(owner + 9116, 12);
    w_u16(owner + 9128, 12);
    w_u16(owner + 9118, 12);
    w_u16(owner + 9140, 64);
    resource002_sub_800deca8(2, 19);
    resource002_sub_800dedf0(2, 0x80000);
    resource002_sub_800deca8(0, 14);
    resource002_sub_800dedf0(0, 0x4001);
    resource002_sub_800ecda8(r_u32(r_u32(0x80112DC4) + 168));
    for (index = 0; index < 5; ++index)
        sub_8002dda8(r_u32(r_u32(0x80112DC4) + 304 + index * 4));
    context = r_u32(0x80112DDC);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800dd1d0(void)
{
    uint32 levels[19] = {0};
    uint32 left, right, index, group = 0, column, row, first, second, owner, game, state;
    uint32 value, packet, ordering, tag, link, frame, mode = 0, text, context, result;
    uint8 arguments[12];
    sint32 sample, divisor_x, divisor_y, frequency, numerator, quotient, elapsed, time;
    FUNCTION_MARKER(0x800DD1D0, "ACE3.BIN");
    left = resource002_sub_800f770c(0);
    right = resource002_sub_800f770c(1);
    frame = r_u32(r_u32(0x800BCE60) + 4);
    owner = r_u32(0x80112DD8);
    first = owner + frame * 2160;
    second = owner + frame * 2160 + 4320;
    for (index = 0; index < 100; ++index)
    {
        sample = r_s16(left + index * 2);
        if (sample < 0)
            sample = -sample;
        levels[group] += (uint32)sample;
        sample = r_s16(right + index * 2);
        if (sample < 0)
            sample = -sample;
        levels[group] += (uint32)sample;
        if (index % 5 == 4)
        {
            levels[group] = (uint32)((sint32)levels[group] / 10);
            value = levels[group] * 7;
            levels[group] = (uint32)((sint32)value / 32768);
            if (index / 5 < 18)
                ++group;
        }
    }
    state = r_u32(0x80112DC4);
    for (column = 0; column < 18; ++column)
    {
        for (row = 0; (sint32)row < (sint32)levels[column]; ++row)
        {
            packet = first + (column * 6 + row) * 20;
            ordering = r_u32(state + 8);
            tag = r_u32(packet);
            link = r_u32(ordering + 4);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            ordering = r_u32(state + 8);
            tag = r_u32(ordering + 4);
            w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    game = r_u32(0x800BCE60);
    frame = r_u32(game + 4);
    owner = r_u32(0x80112DD8);
    state = r_u32(0x80112DC4);
    packet = owner + frame * 12 + 8640;
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    frame = r_u32(game + 4);
    ordering = r_u32(state + 8);
    packet = owner + frame * 12 + 8640;
    tag = r_u32(ordering + 4);
    w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    for (column = 0; column < 18; ++column)
    {
        for (row = 0; (sint32)row < (sint32)levels[column]; ++row)
        {
            packet = second + (column * 6 + row) * 20;
            ordering = r_u32(state + 8);
            tag = r_u32(packet);
            link = r_u32(ordering + 4);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            ordering = r_u32(state + 8);
            tag = r_u32(ordering + 4);
            w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    game = r_u32(0x800BCE60);
    frame = r_u32(game + 4);
    owner = r_u32(0x80112DD8);
    state = r_u32(0x80112DC4);
    packet = owner + frame * 12 + 8664;
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    frame = r_u32(game + 4);
    ordering = r_u32(state + 8);
    packet = owner + frame * 12 + 8664;
    tag = r_u32(ordering + 4);
    w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    frame = r_u32(game + 4);
    packet = owner + frame * 100 + 8688;
    resource002_sub_800f77a4();
    for (index = 0; index < 5; ++index)
    {
        owner = r_u32(0x80112DD8);
        value = (uint32)r_s16(owner + 9142);
        frequency = r_s16(owner + 9132 + index * 2);
        divisor_x = r_s16(owner + 9112 + index * 2);
        divisor_y = r_s16(owner + 9122 + index * 2);
        numerator = (sint32)((uint32)rcos((sint32)(value * (uint32)frequency)) * 76);
        if (!divisor_x || (numerator == (sint32)0x80000000u && divisor_x == -1))
            abort();
        quotient = numerator / divisor_x;
        owner = r_u32(0x80112DD8);
        w_u16(packet + 8, (uint32)(quotient / 4096) + 37);
        value = (uint32)r_s16(owner + 9142);
        numerator = (sint32)((uint32)rsin((sint32)(value * (uint32)frequency)) * 84);
        if (!divisor_y || (numerator == (sint32)0x80000000u && divisor_y == -1))
            abort();
        quotient = numerator / divisor_y;
        w_u16(packet + 10, (uint32)(quotient / 4096) + 302);
        state = r_u32(0x80112DC4);
        ordering = r_u32(state + 8);
        tag = r_u32(packet);
        link = r_u32(ordering + 4);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        ordering = r_u32(state + 8);
        tag = r_u32(ordering + 4);
        w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 20;
    }
    game = r_u32(0x800BCE60);
    frame = r_u32(game + 4);
    owner = r_u32(0x80112DD8);
    packet = owner + frame * 12 + 9088;
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    frame = r_u32(game + 4);
    ordering = r_u32(state + 8);
    packet = owner + frame * 12 + 9088;
    tag = r_u32(ordering + 4);
    w_u32(ordering + 4, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    if (r_u32(game + 304) & 8)
    {
        value = r_u8(owner + 9144);
        if (value == 1)
            w_u8(owner + 9144, 0);
        else if (value == 7)
            w_u8(owner + 9144, 2);
        else
        {
            owner = r_u32(0x80112DD8);
            value = r_u8(owner + 9144);
            w_u8(owner + 9144, value + 1);
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 4)
    {
        owner = r_u32(0x80112DD8);
        value = r_u8(owner + 9144);
        if (value == 0)
            value = 1;
        else if (value == 2)
            value = 7;
        else
        {
            owner = r_u32(0x80112DD8);
            value = r_u8(owner + 9144) - 1;
        }
        w_u8(owner + 9144, value);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 3)
    {
        value = r_u8(r_u32(0x80112DD8) + 9144);
        switch (value)
        {
            case 0:
                w_u8(r_u32(0x80112DD8) + 9144, 2);
                break;
            case 1:
                w_u8(r_u32(0x80112DD8) + 9144, 4);
                break;
            case 2:
            case 3:
                w_u8(r_u32(0x80112DD8) + 9144, 0);
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                w_u8(r_u32(0x80112DD8) + 9144, 1);
                break;
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 16) && !(r_u32(game + 296) & 32))
    {
        value = r_u8(r_u32(0x80112DD8) + 9144);
        switch (value)
        {
            case 0:
            case 1:
                if (!r_u8(r_u32(0x80112DD8) + 9145))
                {
                    context = r_u32(0x80112E0C);
                    frame = r_u32(context + 8);
                    w_u32(context + 8, value ? frame + 1 : frame - 1);
                }
                if (r_u8(r_u32(0x80112DD8) + 9145) == 1)
                {
                    frame = r_u32(r_u32(0x800BCE60) + 32);
                    context = r_u32(0x80112E0C);
                    w_u32(context + 8, (uint32)((sint32)frame % 32));
                }
                break;
            case 2:
                w_u8(r_u32(0x80112DD8) + 9145, 0);
                break;
            case 3:
                w_u8(r_u32(0x80112DD8) + 9145, 1);
                break;
            case 4:
                mode = 1;
                break;
            case 5:
                mode = 3;
                break;
            case 6:
                mode = 2;
                break;
            case 7:
                resource002_sub_800debb8(0, 1);
                w_u8(r_u32(0x80112DDC), 0);
                w_u8(r_u32(0x80112DDC) + 1, 0);
                resource_init_queue(0, 0);
                value = sub_8002db4c(93);
                for (index = 0; index < 4; ++index)
                {
                    context = r_u32(0x80112DC4);
                    w_u32(context + 304 + index * 4, value);
                    value = sub_8002db4c(94 + index);
                }
                w_u32(r_u32(0x80112DC4) + 320, value);
                break;
        }
    }
    context = r_u32(0x80112DC0);
    owner = r_u32(0x80112DD8);
    context = r_u32(context + 92);
    value = r_u8(owner + 9144);
    w_u8(context + 1, value);
    if (!resource_wait(1))
    {
        context = r_u32(0x80112E0C);
        if (r_u32(context) & 1)
        {
            if (!r_u8(r_u32(0x80112DD8) + 9145))
            {
                value = r_u32(context + 8);
                w_u32(context + 8, value + 1);
            }
            if (r_u8(r_u32(0x80112DD8) + 9145) == 1)
            {
                frame = r_u32(r_u32(0x800BCE60) + 32);
                context = r_u32(0x80112E0C);
                w_u32(context + 8, (uint32)((sint32)frame % 32));
            }
        }
    }
    context = r_u32(0x80112E0C);
    time = r_s32(context + 8);
    if (time < 0)
    {
        w_u32(context + 8, 32);
        context = r_u32(0x80112E0C);
        time = r_s32(context + 8);
    }
    if (time >= 33)
        w_u32(context + 8, 0);
    value = sub_8002b588();
    context = r_u32(0x80112E0C);
    result = r_u32(context);
    w_u32(context, value & 0x40000000 ? result | 2 : result & 0xFFFFFFFD);
    resource002_sub_800f7524((sint32)mode, r_u32(r_u32(0x80112E0C) + 8));
    elapsed = (sint32)sub_8002b59c();
    resource002_sub_800f35b8(3);
    resource002_sub_800f33f8(2);
    resource002_sub_800f33d0(128, 128, 128);
    resource002_sub_800f33a8(102, 144);
    value = r_u32(r_u32(0x80112E0C) + 8);
    xport_store_le32(arguments, value);
    resource002_sub_800f4b40(0x800CC834, 0, arguments, 4);
    resource002_sub_800f33a8(124, 144);
    value = r_u32(r_u32(0x80112E0C) + 8);
    text = r_u32(0x8010A568 + value * 4);
    resource002_sub_800f4ae0(text, 0, NULL, 0);
    resource002_sub_800f35b8(4);
    resource002_sub_800f33f8(2);
    resource002_sub_800f33a8(200, 394);
    value = sub_8002b588();
    if (!(value & 0x40000000) || (r_s32(r_u32(0x800BCE60) + 32) % 60) / 30 != 0)
    {
        xport_store_le32(arguments, (uint32)(elapsed / 3600));
        xport_store_le32(arguments + 4, (uint32)(elapsed / 60 - 60 * (elapsed / 3600)));
        xport_store_le32(arguments + 8, (uint32)(100 * (elapsed % 60) / 60));
        resource002_sub_800f4b40(0x800CC838, 0, arguments, 12);
    }
    value = sub_8002b588();
    if (!(value & 0x02000000) || (sub_8002b588() & 0x40000000))
    {
        owner = r_u32(0x80112DD8);
        value = r_u16(owner + 9142);
        w_u16(owner + 9142, value + 1);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            resource002_sub_800debb8(0, 1);
            w_u8(r_u32(0x80112DDC), 0);
            w_u8(r_u32(0x80112DDC) + 1, 0);
            resource_init_queue(0, 0);
            value = sub_8002db4c(93);
            context = r_u32(0x80112DC4);
            w_u32(context + 304, value);
            value = sub_8002db4c(94);
            context = r_u32(0x80112DC4);
            w_u32(context + 308, value);
            result = sub_8002db4c(95);
            context = r_u32(0x80112DC4);
            game = r_u32(0x800BCE60);
            w_u32(context + 312, result);
            w_u32(game + 300, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800ddfa4(void)
{
    uint32 resource, table, value, state, context;
    FUNCTION_MARKER(0x800DDFA4, "ACE3.BIN");
    resource002_sub_800deca8(2, 22);
    resource002_sub_800dedf0(2, 0x400000);
    if (r_u32(r_u32(0x800BCE60) + 68) == 1)
        return 1;
    resource = r_u32(r_u32(0x800BCE50) + 96);
    w_u32(r_u32(0x80112DC4) + 40, resource);
    table = sub_80027338(resource, NULL);
    resource002_sub_800ecda8(r_u32(table + 8));
    arena_set_cursor(r_u32(table + 8));
    value = sub_80027338(r_u32(table), NULL);
    w_u32(r_u32(0x80112DC4) + 468, value);
    value = sub_80027338(r_u32(table + 4), NULL);
    state = r_u32(0x80112DC4);
    context = r_u32(0x80112DDC);
    w_u32(state + 476, value);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800de078(void)
{
    FUNCTION_MARKER(0x800DE078, "ACE3.BIN");
    return resource002_sub_800de080(r_u32(0x800BCE60), 0x800C0000);
}

uint32 resource002_sub_800de080(uint32 game, uint32 globals)
{
    uint32 state, fresh_game, value;
    FUNCTION_MARKER(0x800DE080, "ACE3.BIN");
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        w_u32(r_u32(0x80112DC4) + 284, 1);
        w_u8(game + 2399, 1);
        fresh_game = r_u32(globals - 12704);
        value = r_u8(0x8010A3A8);
        w_u8(fresh_game + 936, value);
    }
    fresh_game = r_u32(0x800BCE60);
    if ((r_u32(fresh_game + 300) & 32) && !(r_u32(fresh_game + 296) & 0x5F))
    {
        resource002_sub_800debb8(0, 1);
        w_u8(r_u32(0x80112DDC), 0);
        w_u8(r_u32(0x80112DDC) + 1, 0);
        state = r_u32(0x80112DC4);
        fresh_game = r_u32(0x800BCE60);
        w_u16(state + 132, 0);
        w_u16(state + 134, 0);
        w_u8(fresh_game + 2399, 0);
        arena_set_cursor(r_u32(r_u32(0x80112DC4) + 40));
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    state = r_u32(0x800BCE54);
    w_u32(state + 4, 0);
    return state;
}

void resource002_sub_800f6cb4(void)
{
    FUNCTION_MARKER(0x800F6CB4, "ACE3.BIN");
}

uint32 resource002_sub_800f0898(void)
{
    uint32 owner, game, state, value, pointer, count, index, descriptor, context;
    FUNCTION_MARKER(0x800F0898, "ACE3.BIN");
    owner = arena_alloc(6304);
    game = r_u32(0x800BCE60);
    w_u16(owner, 0);
    w_u16(owner + 2, 1);
    w_u32(owner + 16, 0);
    w_u32(0x80112DB0, owner);
    value = r_u32(game + 2412);
    w_u32(game + 2412, (value | 64) & 0xFFFFFF7F);
    sub_8002cbcc();
    game = r_u32(0x800BCE60);
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DB0);
    value = r_u8(game + 2390);
    pointer = r_u32(state + 288);
    pointer = r_u32(pointer + value * 4 + 4);
    w_u32(owner + 8, pointer);
    count = r_u32(pointer);
    w_u32(owner + 24, pointer + 4);
    w_u32(owner + 12, count);
    value = r_u8(game + 2398);
    pointer = r_u32(owner + 8);
    w_u32(owner + 6300, pointer + value * 4 + 4);
    value = r_u8(game + 2398);
    count = r_u32(owner + 12);
    w_u32(owner + 12, count - value);
    resource002_sub_800f07f4();
    w_u8(r_u32(0x80112DB0) + 3886, 0);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3896, 0);
    for (index = 16; index > 0; --index)
        w_u16(owner + 1324 + (index - 1) * 160, 0);
    owner = r_u32(0x80112DB0);
    for (index = 16; index > 0; --index)
        w_u16(owner + 5200 + (index - 1) * 32, 0);
    value = r_u32(r_u32(0x80112DB0) + 12);
    if (value)
    {
        w_u16(r_u32(0x80112DC4) + 134, 1);
        value = 0;
    }
    else
    {
        w_u16(r_u32(0x80112DC4) + 134, 3);
        resource002_sub_800debb8(0, 15);
        value = 0x8000;
    }
    resource002_sub_800dedf0(0, value);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3888, 0x8000);
    pointer = r_u32(owner + 6300);
    descriptor = 0x8010B400 + (uint32)r_s16(pointer) * 8;
    value = resource002_sub_800f0144(r_u8(descriptor + 6), descriptor);
    owner = r_u32(0x80112DB0);
    pointer = r_u32(owner + 6300);
    w_u32(owner + 40, value);
    descriptor = 0x8010B400 + (uint32)r_s16(pointer) * 8;
    value = resource002_sub_800f00a8(r_u8(descriptor + 6));
    context = r_u32(0x80112DB0);
    w_u32(context + 28, value);
    return sub_8002db4c(90);
}

uint32 resource002_sub_800f0a7c(void)
{
    FUNCTION_MARKER(0x800F0A7C, "ACE3.BIN");
    return resource002_sub_800f0a84(r_u32(0x800BCE60), 0x800C0000);
}

uint32 resource002_sub_800f0a84(uint32 game, uint32 globals)
{
    uint32 flags, kind, owner, state, pointer, value, count, index, descriptor;
    sint32 mission;
    FUNCTION_MARKER(0x800F0A84, "ACE3.BIN");
    flags = r_u32(game + 2412);
    w_u32(game + 2412, (flags | 128) & 0xFFFFFFBF);
    mission = r_s16(game + 908);
    kind = r_u8(0x8010B9B4 + (uint32)mission);
    if (kind == 0)
    {
        w_u32(r_u32(0x80112DC4) + 148, 0x60000002);
        w_u16(game + 2388, 1);
    }
    else if (kind == 2)
    {
        if (mission == 16)
            w_u8(game + 936, 8);
        game = r_u32(globals - 12704);
        if (r_s16(game + 908) == 37)
        {
            w_u8(game + 936, 26);
            w_u16(r_u32(globals - 12704) + 908, 57);
        }
        game = r_u32(globals - 12704);
        state = r_u32(0x80112DC4);
        w_u32(game + 928, 2);
        w_u16(game + 2388, 1);
        w_u32(state + 148, 0x60000005);
    }
    owner = arena_alloc(6304);
    w_u32(0x80112DB0, owner);
    sub_8002cbcc();
    owner = r_u32(0x80112DB0);
    state = r_u32(0x80112DC4);
    w_u16(owner, 0);
    w_u16(owner + 2, 1);
    w_u32(owner + 16, 0);
    pointer = r_u32(r_u32(state + 288));
    w_u32(owner + 8, pointer);
    count = r_u32(pointer);
    w_u32(owner + 12, count);
    pointer = r_u32(owner + 8);
    game = r_u32(0x800BCE60);
    w_u32(owner + 24, pointer + 4);
    value = r_u8(game + 2398);
    pointer = r_u32(owner + 8);
    w_u32(owner + 6300, pointer + value * 4 + 4);
    value = r_u8(game + 2398);
    count = r_u32(owner + 12);
    w_u32(owner + 12, count - value);
    resource002_sub_800f07f4();
    w_u8(r_u32(0x80112DB0) + 3886, 0);
    w_u32(r_u32(0x80112DB0) + 3896, 0);
    resource002_sub_800dedf0(1, 0);
    owner = r_u32(0x80112DB0);
    state = r_u32(0x80112DC4);
    w_u32(owner + 3888, 0x8000);
    resource002_sub_800de908(r_u32(state + 348), 27, 0);
    w_u8(r_u32(0x80112DB0) + 3912, 0);
    w_u8(r_u32(0x80112DB0) + 3913, 0);
    owner = r_u32(0x80112DB0);
    for (index = 16; index > 0; --index)
        w_u16(owner + 1324 + (index - 1) * 160, 0);
    owner = r_u32(0x80112DB0);
    for (index = 16; index > 0; --index)
        w_u16(owner + 5200 + (index - 1) * 32, 0);
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DB0);
    w_u16(state + 134, 1);
    pointer = r_u32(owner + 6300);
    descriptor = 0x8010B400 + (uint32)r_s16(pointer) * 8;
    value = resource002_sub_800f0144(r_u8(descriptor + 6), descriptor);
    owner = r_u32(0x80112DB0);
    pointer = r_u32(owner + 6300);
    w_u32(owner + 40, value);
    descriptor = 0x8010B400 + (uint32)r_s16(pointer) * 8;
    value = resource002_sub_800f00a8(r_u8(descriptor + 6));
    owner = r_u32(0x80112DB0);
    pointer = r_u32(owner + 6300);
    w_u32(owner + 28, value);
    descriptor = 0x8010B400 + (uint32)r_s16(pointer) * 8;
    if (r_u8(descriptor + 6) != 10)
        return 10;
    count = r_u32(owner + 12);
    if (!count)
        return count;
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    w_u32(state + 148, 0x60000005);
    value = r_u8(game + 2398);
    w_u32(game + 928, 3);
    w_u8(game + 2398, value + 1);
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112DB0);
    w_u16(game + 2388, 2);
    pointer = r_u32(owner + 6300);
    value = r_u8(0x8010B360 + (uint32)r_s16(pointer));
    w_u8(game + 936, value);
    pointer = r_u32(r_u32(0x80112DB0) + 6300);
    return resource002_sub_800fee68((uint32)r_s16(pointer));
}

uint32 resource002_sub_800f0f5c(void)
{
    uint32 owner, old, value, display, result;
    FUNCTION_MARKER(0x800F0F5C, "ACE3.BIN");
    owner = r_u32(0x80112DB0);
    old = r_u32(owner + 16);
    w_u32(owner + 16, old + 1);
    if ((sint32)old >= 32)
    {
        resource002_sub_800ff2bc();
        w_u32(r_u32(0x80112DB0) + 16, 0);
        w_u16(r_u32(0x80112DC4) + 134, 2);
        return 2;
    }
    value = (old + 2) * 143;
    display = r_u32(0x800BCE48);
    w_u8(display + 16, (uint32)((sint32)value / 32));
    value = (r_u32(r_u32(0x80112DB0) + 16) + 1) * 135;
    display = r_u32(0x800BCE48);
    w_u8(display + 17, (uint32)((sint32)value / 32));
    value = (r_u32(r_u32(0x80112DB0) + 16) + 1) * 83;
    display = r_u32(0x800BCE48);
    w_u8(display + 18, (uint32)((sint32)value / 32));
    display = r_u32(0x800BCE48);
    result = r_u32(display) | 8;
    w_u32(display, result);
    return result;
}

uint32 resource002_sub_800f108c(void)
{
    uint32 owner, old, value, display, result;
    FUNCTION_MARKER(0x800F108C, "ACE3.BIN");
    owner = r_u32(0x80112DB0);
    old = r_u32(owner + 16);
    w_u32(owner + 16, old + 1);
    if ((sint32)old >= 32)
    {
        resource002_sub_800ff2bc();
        w_u32(r_u32(0x80112DB0) + 16, 0);
        w_u16(r_u32(0x80112DC4) + 134, 2);
        return 2;
    }
    value = (old + 2) * 143;
    display = r_u32(0x800BCE48);
    w_u8(display + 16, (uint32)((sint32)value / 32));
    value = (r_u32(r_u32(0x80112DB0) + 16) + 1) * 135;
    display = r_u32(0x800BCE48);
    w_u8(display + 17, (uint32)((sint32)value / 32));
    value = (r_u32(r_u32(0x80112DB0) + 16) + 1) * 83;
    display = r_u32(0x800BCE48);
    w_u8(display + 18, (uint32)((sint32)value / 32));
    display = r_u32(0x800BCE48);
    result = r_u32(display) | 8;
    w_u32(display, result);
    return result;
}

uint32 resource002_sub_800f11bc(void)
{
    FUNCTION_MARKER(0x800F11BC, "ACE3.BIN");
    return resource002_sub_800f11c4(r_u32(0x80112DA8));
}

uint32 resource002_sub_800f11c4(uint32 transition)
{
    uint32 game, raw, value;
    sint32 mode;
    FUNCTION_MARKER(0x800F11C4, "ACE3.BIN");
    if (r_u32(transition + 20) != 2)
        return 2;
    game = r_u32(0x800BCE60);
    mode = r_s16(game + 2388);
    raw = r_u16(game + 2388);
    if (mode == 1)
    {
        if (r_u32(r_u32(0x80112DB0) + 12))
        {
            resource002_sub_800debb8(0, 15);
            resource002_sub_800dedf0(0, 0x8000);
            w_u16(r_u32(0x80112DC4) + 134, 3);
            return 3;
        }
        resource002_sub_800decd4(0, 15);
        w_u16(r_u32(0x80112DC4) + 134, 15);
        return 15;
    }
    if (((raw - 2) & 0xFFFF) < 2 && !r_u32(r_u32(0x80112DB0) + 12))
    {
        w_u8(r_u32(0x80112DC4) + 124, 0);
        w_u8(r_u32(0x80112DC4) + 125, 0);
        w_u8(r_u32(0x80112DC4) + 126, 4);
        arena_set_cursor(r_u32(0x80112DB0));
        resource002_sub_800debb8(0, 0);
        resource002_sub_800debb8(0, 1);
        resource002_sub_800dedf0(0, 3);
        resource002_sub_8010097c();
        w_u8(r_u32(0x800BCE60) + 2398, 0);
        value = r_u32(0x800BCE60);
        w_u16(value + 2388, 0);
        return value;
    }
    w_u16(r_u32(0x80112DC4) + 134, 3);
    resource002_sub_800debb8(0, 15);
    return resource002_sub_800dedf0(0, 0x8000);
}

uint32 resource002_sub_800f132c(void)
{
    uint32 state, owner;
    FUNCTION_MARKER(0x800F132C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    if (r_s16(r_u32(state + 364) + 190) != 1)
        return 0x80110000;
    owner = r_u32(0x80112DB0);
    if (!r_u32(owner + 12))
    {
        w_u16(state + 134, 15);
        return 15;
    }
    w_u16(state + 134, 4);
    w_u32(owner + 16, 0);
    return 4;
}

uint32 resource002_sub_800f26ec(void)
{
    uint32 state, owner, pointer, kind, game, value;
    FUNCTION_MARKER(0x800F26EC, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u16(r_u32(state + 364) + 190, 1);
    w_u8(state + 126, 0);
    owner = r_u32(0x80112DB0);
    w_u16(state + 132, 6);
    w_u16(state + 134, 0);
    w_u32(state + 224, 0);
    kind = 0;
    if (r_u32(owner + 12))
    {
        pointer = r_u32(owner + 6300);
        kind = r_u8(0x8010B400 + (uint32)r_s16(pointer) * 8 + 6);
    }
    if (kind == 10)
    {
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        w_u32(state + 284, 1);
        value = r_u8(game + 2398);
        w_u8(game + 2398, value + 1);
        owner = r_u32(0x80112DB0);
        w_u16(game + 2388, 2);
        pointer = r_u32(owner + 6300);
        value = (uint32)r_s16(pointer);
        game = r_u32(0x800BCE60);
        value = r_u8(0x8010B360 + value);
        w_u8(game + 936, value);
        pointer = r_u32(r_u32(0x80112DB0) + 6300);
        resource002_sub_800fee68((uint32)r_s16(pointer));
        return 0;
    }
    w_u8(r_u32(0x800BCE60) + 2398, 0);
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112DB0);
    value = r_u32(game + 2412);
    w_u32(game + 2412, value & 0xFFFFFFBF);
    arena_set_cursor(owner);
    return resource002_sub_800dedf0(0, 0x8000);
}

uint32 resource002_sub_800f2820(void)
{
    uint32 result, owner, pointer, kind = 0, state, game, value, map;
    FUNCTION_MARKER(0x800F2820, "ACE3.BIN");
    result = resource002_sub_800dee84(1);
    if (result)
        return result;
    w_u8(r_u32(0x80112DC4) + 124, 0);
    w_u8(r_u32(0x80112DC4) + 125, 0);
    w_u8(r_u32(0x80112DC4) + 126, 0);
    arena_set_cursor(r_u32(0x80112DB0));
    resource002_sub_800debb8(0, 1);
    resource002_sub_800dedf0(0, 3);
    resource002_sub_800dee08(15, 1, 0, 25);
    resource002_sub_8010097c();
    owner = r_u32(0x80112DB0);
    if (r_u32(owner + 12))
    {
        pointer = r_u32(owner + 6300);
        kind = r_u8(0x8010B400 + (uint32)r_s16(pointer) * 8 + 6);
    }
    if (kind == 10)
    {
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        w_u32(state + 284, 1);
        value = r_u8(game + 2398);
        w_u8(game + 2398, value + 1);
        owner = r_u32(0x80112DB0);
        w_u16(game + 2388, 2);
        pointer = r_u32(owner + 6300);
        value = (uint32)r_s16(pointer);
        map = r_u8(0x8010B360 + value);
        resource002_sub_800fee68(value);
        w_u8(r_u32(0x800BCE60) + 936, map);
        return 0;
    }
    w_u8(r_u32(0x800BCE60) + 2398, 0);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 2412);
    w_u16(game + 2388, 0);
    result = value & 0xFFFFFF7F;
    w_u32(game + 2412, result);
    return result;
}

uint32 resource002_sub_800fe444(void)
{
    uint32 statistics, owner, held, value, index, state;
    FUNCTION_MARKER(0x800FE444, "ACE3.BIN");
    statistics = r_u32(0x800BCE60) + 2396;
    owner = arena_alloc(68);
    w_u32(0x80112E1C, owner);
    w_u8(owner, 0);
    w_u8(r_u32(0x80112E1C) + 1, 0);
    held = r_u32(0x80112E1C);
    w_u8(held + 4, 0);
    owner = r_u32(0x80112E1C);
    w_u16(held + 2, 0);
    w_u8(owner + 5, 0);
    w_u16(r_u32(0x80112E1C) + 66, 0);
    for (index = 0; index < 60; ++index)
    {
        if (r_u8(statistics + index + 704) || r_u8(statistics + index + 764) || r_u8(statistics + index + 824) || r_u8(statistics + index + 884))
        {
            owner = r_u32(0x80112E1C);
            value = (uint32)r_s16(owner + 66);
            w_u8(owner + value + 6, index);
            owner = r_u32(0x80112E1C);
            value = r_u16(owner + 66);
            w_u16(owner + 66, value + 1);
        }
    }
    resource002_sub_800dedf0(0, 0x04000003);
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800fe564(void)
{
    uint32 state, value;
    FUNCTION_MARKER(0x800FE564, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800fe584(void)
{
    uint32 owner, target, value, state, game, column, row;
    FUNCTION_MARKER(0x800FE584, "ACE3.BIN");
    owner = r_u32(0x80112E1C);
    target = r_u32(0x80111758 + r_u8(owner) * 4);
    resource002_mission_select_phase_call(target);
    owner = r_u32(0x80112E1C);
    value = r_u8(owner + 1);
    if (value == 1)
    {
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        w_u16(state + 132, 2);
        w_u16(state + 134, 0);
        row = r_u8(owner + 5);
        column = r_u8(owner + 4);
        value = r_u8(owner + row * 4 + column + 6);
        w_u16(game + 908, value);
        return arena_set_cursor(r_u32(state + 40));
    }
    if (value == 2)
    {
        w_u8(r_u32(0x80112DC4) + 124, 0);
        w_u8(r_u32(0x80112DC4) + 125, 0);
        return arena_set_cursor(r_u32(r_u32(0x80112DC4) + 40));
    }
    return 2;
}

uint32 resource002_sub_800fd958(void)
{
    uint32 object;
    FUNCTION_MARKER(0x800FD958, "ACE3.BIN");
    w_u8(r_u32(0x80112E1C), 1);
    resource002_sub_800fd7d0(r_u8(r_u32(0x80112E1C) + 5));
    resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 47, 1);
    return object;
}

uint32 resource002_sub_800fd9cc(void)
{
    FUNCTION_MARKER(0x800FD9CC, "ACE3.BIN");
    return resource002_sub_800fd9d4(r_u32(0x80112DD4));
}

uint32 resource002_sub_800fde78(void)
{
    uint32 owner, object, index, row, column;
    FUNCTION_MARKER(0x800FDE78, "ACE3.BIN");
    owner = r_u32(0x80112E1C);
    if (r_s16(owner + 2) == 1)
    {
        w_u8(owner, 3);
        resource002_sub_800dedf0(0, 0x08000003);
        owner = r_u32(0x80112E1C);
        row = r_u8(owner + 5);
        column = r_u8(owner + 4);
        resource002_sub_800fd770(row, column);
    }
    owner = r_u32(0x80112E1C);
    if (r_s16(owner + 2) == 2)
        w_u8(owner + 1, 2);
    for (index = 49; index < 57; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 47, 0);
    return object;
}

uint32 resource002_sub_800fdf38(void)
{
    FUNCTION_MARKER(0x800FDF38, "ACE3.BIN");
    w_u8(r_u32(0x80112E1C), 4);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 47, 1);
    return 1;
}

uint32 resource002_sub_800fdf64(void)
{
    FUNCTION_MARKER(0x800FDF64, "ACE3.BIN");
    return resource002_sub_800fdf6c(r_u32(0x80112DC0));
}

uint32 resource002_sub_800fdf6c(uint32 scene)
{
    uint32 game, value, owner, object;
    FUNCTION_MARKER(0x800FDF6C, "ACE3.BIN");
    w_u8(r_u32(scene + 92) + 51, 0);
    game = r_u32(0x800BCE60);
    if (r_u32(game + 304) & 2)
    {
        value = r_u32(game + 916);
        if ((sint32)value < 2)
        {
            w_u32(game + 916, value + 1);
            sub_8002db4c(61);
        }
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 304) & 1)
    {
        value = r_u32(game + 916);
        if ((sint32)value > 0)
        {
            w_u32(game + 916, value - 1);
            sub_8002db4c(61);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 16) && !(r_u32(game + 296) & 32))
    {
        owner = r_u32(0x80112E1C);
        w_u16(owner + 2, 1);
        w_u8(owner, 5);
        sub_8002db4c(60);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 32) && !(r_u32(game + 296) & 16))
    {
        owner = r_u32(0x80112E1C);
        w_u16(owner + 2, 2);
        w_u8(owner, 5);
        sub_8002db4c(59);
    }
    scene = r_u32(0x80112DC0);
    game = r_u32(0x800BCE60);
    object = r_u32(scene + 92);
    value = r_u8(game + 916);
    w_u8(object + 52, value);
    return value;
}

uint32 resource002_sub_800fe0d4(void)
{
    uint32 owner, value, object;
    sint32 phase;
    FUNCTION_MARKER(0x800FE0D4, "ACE3.BIN");
    owner = r_u32(0x80112E1C);
    phase = r_s16(owner + 2);
    if (phase == 1)
    {
        value = r_u8(owner) + 1;
        w_u8(owner, value);
    }
    else if (phase == 2)
    {
        resource002_sub_800fd7d0(r_u8(owner + 5));
        resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
        resource002_sub_800dedf0(0, 0x04000003);
        w_u8(r_u32(0x80112E1C), 0);
    }
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 47, 0);
    return object;
}

uint32 resource002_sub_800fe180(void)
{
    uint32 state, owner, row, column, value;
    FUNCTION_MARKER(0x800FE180, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112E1C);
    w_u32(state + 612, 0);
    row = r_u8(owner + 5);
    column = r_u8(owner + 4);
    value = r_u8(owner + row * 4 + column + 6);
    resource002_sub_800feef8(value);
    owner = r_u32(0x80112E1C);
    value = r_u8(owner) + 1;
    w_u8(owner, value);
    return value;
}

uint32 resource002_sub_800fd9d4(uint32 cooldown)
{
    uint32 owner, game, row, column, value, flags, scene, object;
    sint32 count, last_page;
    FUNCTION_MARKER(0x800FD9D4, "ACE3.BIN");
    if (cooldown)
        w_u32(0x80112DD4, cooldown - 1);
    else
    {
        owner = r_u32(0x80112E1C);
        count = r_s16(owner + 66);
        row = r_u8(owner + 5);
        last_page = (count - 1) / 4;
        flags = (sint32)row < last_page ? 2 : 0;
        if (row)
            flags |= 1;
        resource002_sub_800def94(49, flags);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 51, 0);
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 2)
    {
        owner = r_u32(0x80112E1C);
        if (r_u8(owner + 4) < 3)
        {
            row = r_u8(owner + 5);
            column = r_u8(owner + 4);
            count = r_s16(owner + 66);
            if ((sint32)(column + row * 4) < count - 1)
            {
                w_u8(owner + 4, column + 1);
                sub_8002db4c(61);
            }
        }
        else
        {
            count = r_s16(owner + 66);
            row = r_u8(owner + 5);
            if ((sint32)row < (count - 1) / 4)
            {
                w_u8(owner + 4, 0);
                owner = r_u32(0x80112E1C);
                value = r_u8(owner + 5);
                w_u8(owner + 5, value + 1);
                resource002_sub_800fd7d0(r_u8(r_u32(0x80112E1C) + 5));
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 51, 2);
                w_u32(0x80112DD4, 12);
                sub_8002db4c(61);
            }
        }
        resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 1)
    {
        owner = r_u32(0x80112E1C);
        value = r_u8(owner + 4);
        if (value)
        {
            w_u8(owner + 4, value - 1);
            sub_8002db4c(61);
        }
        else if (r_u8(owner + 5))
        {
            w_u8(owner + 4, 3);
            owner = r_u32(0x80112E1C);
            value = r_u8(owner + 5);
            w_u8(owner + 5, value - 1);
            resource002_sub_800fd7d0(r_u8(r_u32(0x80112E1C) + 5));
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 51, 1);
            w_u32(0x80112DD4, 12);
            sub_8002db4c(61);
        }
        resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 0x200)
    {
        owner = r_u32(0x80112E1C);
        count = r_s16(owner + 66);
        row = r_u8(owner + 5);
        if ((sint32)row < (count - 1) / 4)
        {
            w_u8(owner + 5, row + 1);
            owner = r_u32(0x80112E1C);
            row = r_u8(owner + 5);
            column = r_u8(owner + 4);
            count = r_s16(owner + 66);
            if ((sint32)(column + row * 4) >= count - 1)
            {
                value = (uint32)(count - 1 - ((count - 1) / 4) * 4);
                w_u8(owner + 4, value);
            }
            sub_8002db4c(61);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 51, 2);
            w_u32(0x80112DD4, 12);
        }
        resource002_sub_800fd7d0(r_u8(r_u32(0x80112E1C) + 5));
        resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 0x100)
    {
        owner = r_u32(0x80112E1C);
        value = r_u8(owner + 5);
        if (value)
        {
            w_u8(owner + 5, value - 1);
            sub_8002db4c(61);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 51, 1);
            w_u32(0x80112DD4, 12);
        }
        resource002_sub_800fd7d0(r_u8(r_u32(0x80112E1C) + 5));
        resource002_sub_800fd8f4(r_u8(r_u32(0x80112E1C) + 4));
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        owner = r_u32(0x80112E1C);
        w_u16(owner + 2, 1);
        w_u8(owner, 2);
        sub_8002db4c(60);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        owner = r_u32(0x80112E1C);
        w_u16(owner + 2, 2);
        w_u8(owner, 2);
        sub_8002db4c(59);
    }
    scene = r_u32(0x80112DC0);
    owner = r_u32(0x80112E1C);
    object = r_u32(scene + 92);
    value = r_u8(owner + 4);
    w_u8(object + 52, value);
    return value;
}
