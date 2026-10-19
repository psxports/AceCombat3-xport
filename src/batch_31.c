#include "game_functions.h"
#include <string.h>
#include <stdlib.h>

uint32 resource002_sub_800e7b8c(void)
{
    uint32 first, second, context, value, object;
    FUNCTION_MARKER(0x800E7B8C, "ACE3.BIN");
    w_u8(r_u32(0x80112DF8) + 8, 0);
    w_u8(r_u32(0x80112DF8) + 9, 0);
    first = resource002_sub_800e267c(0);
    second = resource002_sub_800e267c(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        resource002_sub_800dedf0(1, 3073);
        resource002_sub_800deca8(1, 0);
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        w_u8(r_u32(0x80112DF8) + 8, 1);
    }
    else
    {
        resource002_sub_800dedf0(1, 3076);
        resource002_sub_800deca8(1, 2);
        resource002_sub_800dee08(1, 1, 0, 20);
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 46);
    }
    if (first != 5 && second == 5)
        w_u8(r_u32(0x80112DF8) + 4, 1);
    object = r_u32(r_u32(0x80112DC0) + 92);
    w_u8(object + 30, 0);
    return object;
}

uint32 resource002_sub_800e7cdc(void)
{
    uint32 phase, previous, first, second, context, value, object, game, result;
    FUNCTION_MARKER(0x800E7CDC, "ACE3.BIN");
    phase = resource002_sub_800dee84(1);
    previous = r_u8(r_u32(0x80112DF8) + 4);
    first = resource002_sub_800e267c(0);
    second = resource002_sub_800e267c(1);
    if (r_u32(r_u32(0x800BCE60) + 304) & 3)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 4);
        w_u8(context + 4, value ^ 1);
    }
    if (first == 2 || first == 6)
        w_u8(r_u32(0x80112DF8) + 4, 1);
    if (second == 2 || second == 6)
        w_u8(r_u32(0x80112DF8) + 4, 0);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        resource002_sub_800dee08(1, 0, 0, 0);
        resource002_sub_800dedf0(1, 3073);
        resource002_sub_800deca8(1, 0);
        w_u8(r_u32(0x80112DF8), 1);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
        return result;
    }
    if (first == 2 || first == 6)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 2, 2);
    }
    else
    {
        value = r_u8(r_u32(0x80112DF8) + 4);
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 2, value != 0);
    }
    if (second == 2 || second == 6)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 3, 2);
    }
    else
    {
        value = r_u8(r_u32(0x80112DF8) + 4);
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + 3, value != 1);
    }
    game = r_u32(0x800BCE60);
    if (((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F)) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(1, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 5);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
                return result;
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
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
                return result;
            }
        }
        else
        {
            resource002_sub_800dee08(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            sub_8002db4c(59);
        }
    }
    result = r_u32(r_u32(0x800BCE60) + 304) & 3;
    if (result)
    {
        result = r_u8(r_u32(0x80112DF8) + 4);
        if (previous != result)
            return sub_8002db4c(61);
    }
    return result;
}

uint32 resource002_sub_800e80b8(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E80B8, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 2 || status == 6)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        resource002_sub_800dee08(4, 1, 0, 20);
        resource002_sub_800dedf0(1, 3073);
        resource002_sub_800deca8(1, 0);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 44);
    }
    else
        w_u8(r_u32(0x80112DF8), 6);
    resource002_sub_800e3b5c();
    return resource002_sub_800e3e14();
}

uint32 resource002_sub_800e8174(void)
{
    uint32 phase, first, second, game, context, value, result, accepted;
    FUNCTION_MARKER(0x800E8174, "ACE3.BIN");
    phase = resource002_sub_800dee84(4);
    first = resource002_sub_800e267c(0);
    second = resource002_sub_800e267c(1);
    if (first == 0 || second == 0)
    {
        if (phase == 0 || phase == 3)
        {
            context = r_u32(0x80112DF8);
            if (r_u8(context + 3) == 3)
            {
                w_u8(context, 5);
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
                w_u8(r_u32(0x80112DF8) + 1, 0);
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
        if (phase == 0 || phase == 3)
        {
            result = 2;
            if (r_u8(r_u32(0x80112DF8) + 3) == 2)
            {
                resource002_sub_800dee08(4, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 3);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(4, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return 2;
        }
    }
    return result;
}

uint32 resource002_sub_800e3b5c(void)
{
    FUNCTION_MARKER(0x800E3B5C, "ACE3.BIN");
    return resource002_sub_800e3ae8();
}

uint32 resource002_sub_800e3e14(void)
{
    FUNCTION_MARKER(0x800E3E14, "ACE3.BIN");
    return resource002_sub_800e3b7c();
}

uint32 resource002_sub_800e83ac(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800E83AC, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    resource002_sub_800dee08(2, 1, 0, 20);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 47);
    return 47;
}

uint32 resource002_sub_800e8404(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800E8404, "ACE3.BIN");
    result = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (result)
    {
        w_u8(r_u32(0x80112DF8), 6);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
    }
    return result;
}

uint32 resource002_sub_800e8454(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E8454, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status != 1)
    {
        w_u8(r_u32(0x80112DF8), 7);
        return 7;
    }
    resource002_sub_800dedf0(1, 3073);
    resource002_sub_800deca8(1, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 49);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e84e8(void)
{
    uint32 status, result = 6, game, value;
    FUNCTION_MARKER(0x800E84E8, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
    }
    else if (status)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            value = r_u32(r_u32(0x80112DC4) + 224);
            value = resource002_sub_800e69a8(value);
            w_u8(r_u32(0x80112DF8), value);
            w_u8(r_u32(0x80112DF8) + 1, 0);
        }
        game = r_u32(0x800BCE60);
        result = r_u32(game + 300) & 32;
        if (result)
        {
            result = r_u32(game + 296) & 0x5F;
            if (result == 0)
            {
                w_u8(r_u32(0x80112DF8), 3);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
            }
        }
    }
    return result;
}

uint32 resource002_sub_800e8600(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E8600, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status != 3)
    {
        w_u8(r_u32(0x80112DF8), 8);
        return 8;
    }
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    resource002_sub_800dedf0(1, 3074);
    resource002_sub_800deca8(1, 1);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 51);
    return 51;
}

uint32 resource002_sub_800e868c(void)
{
    uint32 phase, context, value;
    FUNCTION_MARKER(0x800E868C, "ACE3.BIN");
    phase = resource002_sub_800dee84(1);
    if (phase == 0)
    {
        resource002_sub_800dee08(2, 1, 0, 20);
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1) + 1;
        w_u8(context + 1, value);
        return value;
    }
    if (phase != 3)
        return resource002_sub_800dee08(1, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
    return 3;
}

uint32 resource002_sub_800e8708(void)
{
    uint32 phase, status, result = 6, game, context, value, accepted;
    FUNCTION_MARKER(0x800E8708, "ACE3.BIN");
    phase = resource002_sub_800dee84(2);
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
        return result;
    }
    if (status == 0)
        return result;
    if ((r_u32(r_u32(0x800BCE60) + 300) & 64) || phase == 0)
    {
        if (phase != 0 && phase != 3)
        {
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 1);
            return sub_8002db4c(60);
        }
        if (r_u8(r_u32(0x80112DF8) + 3) == 1)
        {
            resource002_sub_800dee08(2, 0, 0, 0);
            resource002_sub_800dee08(3, 1, 0, 20);
            context = r_u32(0x80112DF8);
            value = r_u8(context + 1);
            w_u8(context + 1, value + 1);
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 53);
            return 53;
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
            resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            return sub_8002db4c(59);
        }
        result = 2;
        if (r_u8(r_u32(0x80112DF8) + 3) == 2)
        {
            resource002_sub_800dee08(2, 0, 0, 0);
            w_u8(r_u32(0x80112DF8), 3);
            result = r_u32(0x80112DF8);
            w_u8(result + 1, 0);
        }
    }
    return result;
}

uint32 resource002_sub_800e8910(void)
{
    uint32 result, status, context, value;
    FUNCTION_MARKER(0x800E8910, "ACE3.BIN");
    result = resource002_sub_800dee84(3);
    if (result != 2)
        return result;
    status = resource002_sub_800e4760(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 0)
        return 0;
    if (status == 1)
    {
        resource002_sub_800dee08(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        result = r_u32(0x80112DF8);
        w_u8(result + 9, status);
        return result;
    }
    if (status == 0xFFFFFFFFu)
    {
        resource002_sub_800dee08(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 2);
        resource002_sub_800dedf0(1, 3073);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 80);
        return 80;
    }
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e89f8(void)
{
    uint32 status, result = 6, game, value;
    FUNCTION_MARKER(0x800E89F8, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 80);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
    }
    else if (status)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            value = r_u32(r_u32(0x80112DC4) + 224);
            value = resource002_sub_800e69a8(value);
            w_u8(r_u32(0x80112DF8), value);
            w_u8(r_u32(0x80112DF8) + 1, 0);
        }
        game = r_u32(0x800BCE60);
        result = r_u32(game + 300) & 32;
        if (result)
        {
            result = r_u32(game + 296) & 0x5F;
            if (result == 0)
            {
                w_u8(r_u32(0x80112DF8), 0);
                result = r_u32(0x80112DF8);
                w_u8(result + 1, 0);
            }
        }
    }
    return result;
}

uint32 resource002_sub_800e94e0(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E94E0, "ACE3.BIN");
    status = resource002_sub_800e47b0(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 1)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
    }
    if (status == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1) + 3;
        w_u8(context + 1, value);
        return value;
    }
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e955c(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E955C, "ACE3.BIN");
    status = resource002_sub_800e5664(r_u8(r_u32(0x80112DF8) + 4), 0);
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
        return resource002_sub_800e3b5c();
    }
    return 1;
}

uint32 resource002_sub_800e95dc(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E95DC, "ACE3.BIN");
    status = resource002_sub_800e57f0(r_u8(r_u32(0x80112DF8) + 4), 0xFFFFFFFFu, 1);
    if (status == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
    }
    if (status == 1)
    {
        w_u8(r_u32(0x80112DF8), 11);
        context = r_u32(0x80112DF8);
        w_u8(context + 1, 0);
        return context;
    }
    return 1;
}

uint32 resource002_sub_800e264c(void)
{
    uint32 state, value;
    FUNCTION_MARKER(0x800E264C, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    value = r_u8(state + 844);
    w_u32(state + 40, 0);
    if (value)
        return 0;
    return r_u8(state + 845) == 0;
}

uint32 resource002_sub_800e4760(uint32 port)
{
    sint32 result;
    FUNCTION_MARKER(0x800E4760, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    result = _card_format((sint32)(port << 4));
    resource002_sub_800e25d4();
    return result == 1 ? 1 : 0xFFFFFFFFu;
}

uint32 resource002_sub_800e47b0(uint32 port)
{
    uint32 state, descriptor;
    FUNCTION_MARKER(0x800E47B0, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    descriptor = game_bios_open_guest(state + port * 26 + 692, 0x20200, 26);
    w_u32(r_u32(0x80112DF0) + 60, descriptor);
    game_bios_close_guest(descriptor);
    resource002_sub_800e25d4();
    return descriptor == 0xFFFFFFFFu ? 0xFFFFFFFFu : 1;
}

uint32 resource002_sub_800e5664(uint32 port, uint32 finalize)
{
    uint32 state, descriptor, value, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5664, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x80112DF0);
    }
    if (r_u8(state + 822) == 0)
    {
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
            resource002_sub_800e25d4();
            w_u16(r_u32(0x80112DF0) + 858, 0);
            return 0xFFFFFFFFu;
        }
        value = r_u32(0x80112DEC);
        game_bios_write_guest(descriptor, value, 640);
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
        w_u16(state + 858, 0);
        resource002_sub_800e25d4();
        return 0xFFFFFFFFu;
    }
    if (finalize == 1)
        resource002_sub_800e25d4();
    return 1;
}

uint32 resource002_sub_800e57f0(uint32 port, uint32 slot, uint32 finalize)
{
    uint32 state, buffer, value, checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E57F0, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x80112DF0);
    }
    if (r_u8(state + 822) == 0)
    {
        buffer = r_u32(state + 744);
        if ((sint32)slot >= 0)
        {
            value = r_u8(buffer + 8);
            w_u8(buffer + 8, value | (1u << (slot & 31)));
            buffer = r_u32(r_u32(0x80112DF0) + 744);
            value = r_u32(buffer + 12);
            w_u32(buffer + 12, value + 1);
        }
        else
        {
            w_u8(buffer + 8, 0);
            w_u32(r_u32(r_u32(0x80112DF0) + 744) + 12, 0);
        }
        buffer = r_u32(r_u32(0x80112DF0) + 744);
        checksum = 0x41434533;
        for (index = 0; index < 31; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x80112DF0);
        buffer = r_u32(state + 744);
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
        game_bios_seek_guest(descriptor, 640, 0);
        state = r_u32(0x80112DF0);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + 744);
        game_bios_write_guest(descriptor, buffer, 128);
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

uint32 resource002_sub_800e8b2c(void)
{
    uint32 result, context;
    FUNCTION_MARKER(0x800E8B2C, "ACE3.BIN");
    result = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (result == 4)
    {
        w_u8(r_u32(0x80112DF8), 9);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
        return result;
    }
    context = r_u32(0x80112DF8);
    result = r_u8(context + 1) + 1;
    w_u8(context + 1, result);
    return result;
}

uint32 resource002_sub_800e8b98(void)
{
    uint32 status, context, result;
    FUNCTION_MARKER(0x800E8B98, "ACE3.BIN");
    status = resource002_sub_800e4ef0(r_u8(r_u32(0x80112DF8) + 4), 0);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, status == 2 ? 81 : 80);
        w_u8(r_u32(0x80112DF8) + 1, 6);
    }
    if (status == 1)
    {
        context = r_u32(0x80112DF8);
        result = r_u8(context + 1) + 1;
        w_u8(context + 1, result);
        return result;
    }
    return 1;
}

uint32 resource002_sub_800e8c50(void)
{
    uint32 status, context, result;
    FUNCTION_MARKER(0x800E8C50, "ACE3.BIN");
    status = resource002_sub_800e5100(r_u8(r_u32(0x80112DF8) + 4), 1);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, status == 2 ? 81 : 80);
        w_u8(r_u32(0x80112DF8) + 1, 6);
    }
    if (status == 1)
    {
        context = r_u32(0x80112DF8);
        result = r_u8(context + 1) + 1;
        w_u8(context + 1, result);
        return result;
    }
    return 1;
}

uint32 resource002_sub_800e8d08(void)
{
    uint32 value, context, result;
    FUNCTION_MARKER(0x800E8D08, "ACE3.BIN");
    value = resource002_sub_800e46a0();
    if (value == 0xFFFFFFFFu)
    {
        w_u8(r_u32(0x80112DF8), 9);
        result = r_u32(0x80112DF8);
        w_u8(result + 1, 0);
        return result;
    }
    w_u8(r_u32(0x80112DF8) + 5, value);
    context = r_u32(0x80112DF8);
    result = r_u8(context + 1) + 1;
    w_u8(context + 1, result);
    return result;
}

uint32 resource002_sub_800e8d78(void)
{
    uint32 mode, context, result;
    FUNCTION_MARKER(0x800E8D78, "ACE3.BIN");
    resource002_sub_800e30a4(r_u8(r_u32(0x80112DF8) + 5));
    w_u8(r_u32(0x80112DF8) + 10, 0);
    mode = r_u32(r_u32(0x80112DC4) + 224);
    if (mode - 2 < 3)
    {
        resource002_sub_800e2ab4();
        w_u8(r_u32(0x80112DF8), 12);
        w_u8(r_u32(0x80112DF8) + 1, 6);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 59);
        return 59;
    }
    context = r_u32(0x80112DF8);
    result = r_u8(context + 1) + 1;
    w_u8(context + 1, result);
    return result;
}

uint32 resource002_sub_800e8e28(void)
{
    uint32 context, port, slot, status;
    FUNCTION_MARKER(0x800E8E28, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    port = r_u8(context + 4);
    slot = r_u8(context + 5);
    status = resource002_sub_800e53a0(port, slot, 1);
    if (status == 0xFFFFFFFFu || status == 2)
    {
        w_u8(r_u32(0x80112DF8) + 1, 6);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, status == 2 ? 81 : 80);
    }
    if (status == 1)
    {
        w_u8(r_u32(0x80112DF8), 13);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        return resource002_sub_800dee08(2, 0, 0, 0);
    }
    return 1;
}

uint32 resource002_sub_800e8ef0(void)
{
    uint32 game, value, result;
    FUNCTION_MARKER(0x800E8EF0, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3073);
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        value = r_u32(r_u32(0x80112DC4) + 224);
        value = resource002_sub_800e69a8(value);
        w_u8(r_u32(0x80112DF8), value);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        sub_8002db4c(60);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (result == 0)
        {
            w_u8(r_u32(0x80112DF8), 0);
            return sub_8002db4c(59);
        }
    }
    return result;
}

uint32 resource002_sub_800e46a0(void)
{
    uint32 index, state, header, account, game, identity, expected;
    const char *name, *current_name;
    FUNCTION_MARKER(0x800E46A0, "ACE3.BIN");
    for (index = 0; index < 3; ++index)
    {
        state = r_u32(0x80112DF0);
        header = r_u32(state + 744);
        if ((r_u8(header + 8) >> index) & 1)
        {
            account = r_u32(state + 748 + index * 4);
            game = r_u32(0x800BCE60);
            name = game_native_string(account + 8);
            current_name = game_native_string(game + 2376);
            if (strcmp(name, current_name) == 0)
            {
                account = r_u32(r_u32(0x80112DF0) + 748 + index * 4);
                game = r_u32(0x800BCE60);
                identity = r_u32(account + 4);
                expected = r_u32(game + 2408);
                if (identity == expected)
                    return index;
            }
        }
    }
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e4ef0(uint32 port, uint32 finalize)
{
    uint32 state, descriptor, event, retry, buffer, checksum, index, value;
    sint32 attempts;
    FUNCTION_MARKER(0x800E4EF0, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x80112DF0);
    }
    if (r_u8(state + 822) == 0)
    {
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
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
            resource002_sub_800e25d4();
            w_u16(r_u32(0x80112DF0) + 858, 0);
            return 0xFFFFFFFFu;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        state = r_u32(0x80112DF0);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + 744);
        game_bios_read_guest(descriptor, buffer, 128);
        w_u8(r_u32(0x80112DF0) + 822, 1);
    }
    event = resource002_sub_800e2794(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(r_u32(r_u32(0x80112DF0) + 60));
    w_u8(r_u32(0x80112DF0) + 822, 0);
    buffer = r_u32(r_u32(0x80112DF0) + 744);
    checksum = 0x41434533;
    for (index = 0; index < 31; ++index)
        checksum ^= r_u32(buffer + 4 + index * 4);
    state = r_u32(0x80112DF0);
    buffer = r_u32(state + 744);
    value = r_u32(buffer);
    if (value != checksum || r_u32(buffer + 4) != 0x990426)
    {
        resource002_sub_800e25d4();
        w_u16(r_u32(0x80112DF0) + 858, 0);
        return 2;
    }
    if (event)
    {
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        w_u16(state + 858, 0);
        resource002_sub_800e25d4();
        return 0xFFFFFFFFu;
    }
    w_u16(state + 858, 0);
    if (finalize == 1)
        resource002_sub_800e25d4();
    return 1;
}

uint32 resource002_sub_800e5100(uint32 port, uint32 finalize)
{
    uint32 state, header, descriptor, event, buffer, index, checksum, retry, value;
    sint32 slot, attempts;
    FUNCTION_MARKER(0x800E5100, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    header = r_u32(state + 744);
    slot = r_s16(state + 80);
    value = r_u8(header + 8);
    if ((value >> ((uint32)slot & 31)) & 1)
    {
        if (!resource002_sub_800e264c())
            return 0;
        state = r_u32(0x80112DF0);
        if (r_s16(state + 858) == 0)
        {
            w_u16(state + 858, 1);
            w_u16(state + 856, 0);
            state = r_u32(0x80112DF0);
        }
        if (r_u8(state + 822) == 0)
        {
            descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
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
            slot = r_s16(state + 80);
            game_bios_seek_guest(descriptor, (sint32)((uint32)slot * 1152 + 768), 0);
            state = r_u32(0x80112DF0);
            slot = r_s16(state + 80);
            descriptor = r_u32(state + 60);
            buffer = r_u32(state + (uint32)slot * 4 + 748);
            game_bios_read_guest(descriptor, buffer, 1152);
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
        state = r_u32(0x80112DF0);
        slot = r_s16(state + 80);
        buffer = r_u32(state + (uint32)slot * 4 + 748);
        checksum = 0x41434533;
        for (index = 0; index < 287; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x80112DF0);
        slot = r_s16(state + 80);
        buffer = r_u32(state + (uint32)slot * 4 + 748);
        if (r_u32(buffer) != checksum)
        {
            resource002_sub_800e25d4();
            state = r_u32(0x80112DF0);
            w_u16(state + 858, 0);
            w_u16(state + 80, 0);
            return 2;
        }
    }
    state = r_u32(0x80112DF0);
    slot = r_s16(state + 80);
    value = r_u16(state + 80);
    if (slot < 2)
    {
        w_u16(state + 80, value + 1);
        return 0;
    }
    w_u16(state + 80, 0);
    if (finalize == 1)
        resource002_sub_800e25d4();
    return 1;
failure:
    resource002_sub_800e25d4();
    state = r_u32(0x80112DF0);
    w_u16(state + 858, 0);
    w_u16(state + 80, 0);
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e53a0(uint32 port, uint32 account, uint32 finalize)
{
    uint32 state, buffer, mask, descriptor, event, index, checksum, retry, value;
    sint32 slot, attempts;
    FUNCTION_MARKER(0x800E53A0, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    buffer = r_u32(state + account * 4 + 748);
    slot = r_s16(state + 80);
    mask = (uint32)r_s16(buffer + 22);
    if ((mask >> ((uint32)slot & 31)) & 1)
    {
        if (!resource002_sub_800e264c())
            return 0;
        state = r_u32(0x80112DF0);
        if (r_s16(state + 858) == 0)
        {
            w_u16(state + 858, 1);
            w_u16(state + 856, 0);
            state = r_u32(0x80112DF0);
        }
        if (r_u8(state + 822) == 0)
        {
            descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
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
            slot = r_s16(state + 80);
            game_bios_seek_guest(descriptor, (sint32)(account * 2304 + 4224 + (uint32)slot * 384), 0);
            state = r_u32(0x80112DF0);
            slot = r_s16(state + 80);
            descriptor = r_u32(state + 60);
            buffer = r_u32(state + (uint32)slot * 4 + 764);
            game_bios_read_guest(descriptor, buffer, 384);
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
        state = r_u32(0x80112DF0);
        slot = r_s16(state + 80);
        buffer = r_u32(state + (uint32)slot * 4 + 764);
        checksum = 0x41434533;
        for (index = 0; index < 95; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x80112DF0);
        slot = r_s16(state + 80);
        buffer = r_u32(state + (uint32)slot * 4 + 764);
        if (r_u32(buffer) != checksum)
        {
            resource002_sub_800e25d4();
            state = r_u32(0x80112DF0);
            w_u16(state + 858, 0);
            w_u16(state + 80, 0);
            return 2;
        }
    }
    state = r_u32(0x80112DF0);
    slot = r_s16(state + 80);
    value = r_u16(state + 80);
    if (slot < 6)
    {
        w_u16(state + 80, value + 1);
        return 0;
    }
    if (finalize == 1)
        resource002_sub_800e25d4();
    state = r_u32(0x80112DF0);
    w_u16(state + 858, 0);
    w_u16(state + 80, 0);
    return 1;
failure:
    w_u16(state + 858, 0);
    resource002_sub_800e25d4();
    w_u16(r_u32(0x80112DF0) + 80, 0);
    return 0xFFFFFFFFu;
}

uint32 resource002_sub_800e30a4(uint32 account)
{
    uint32 state, source, destination, offset, first, second, third, fourth;
    FUNCTION_MARKER(0x800E30A4, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    source = r_u32(state + account * 4 + 748);
    destination = r_u32(state + 760);
    for (offset = 0; offset < 1152; offset += 16)
    {
        first = r_u32(source + offset);
        second = r_u32(source + offset + 4);
        third = r_u32(source + offset + 8);
        fourth = r_u32(source + offset + 12);
        w_u32(destination + offset, first);
        w_u32(destination + offset + 4, second);
        w_u32(destination + offset + 8, third);
        w_u32(destination + offset + 12, fourth);
    }
    return destination + 1152;
}

uint32 resource002_sub_800e2ab4(void)
{
    uint32 state, game, statistics, account, identity, group, index, source, target, first, second, third, fourth, value;
    uint32 row, bit, count, source_offset, target_offset, display, result;
    FUNCTION_MARKER(0x800E2AB4, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    game = r_u32(0x800BCE60);
    account = r_u32(state + 760);
    identity = r_u32(game + 2408);
    statistics = game + 2396;
    w_u32(account + 4, identity);
    for (group = 0; group < 6; ++group)
    {
        target = account + 708 + group * 68;
        game = r_u32(0x800BCE60);
        for (index = 0; index < 20; ++index)
        {
            value = r_u16(game + group * 92 + 370 + index * 2);
            w_u16(target + index * 2, value);
        }
        game = r_u32(0x800BCE60);
        source = game + group * 92;
        first = r_u32(source + 412);
        second = r_u32(source + 416);
        third = r_u32(source + 420);
        fourth = r_u32(source + 424);
        w_u32(target + 40, first);
        w_u32(target + 44, second);
        w_u32(target + 48, third);
        w_u32(target + 52, fourth);
        first = r_u32(source + 428);
        second = r_u32(source + 432);
        w_u32(target + 56, first);
        w_u32(target + 60, second);
        value = r_u32(source + 436);
        w_u32(target + 64, value);
        value = r_u32(game + 288);
        w_u8(account + 1124, (value & 4) == 0);
    }
    for (group = 0; group < 2; ++group)
    {
        count = group ? 2 : 8;
        source_offset = group ? 88 : 24;
        target_offset = group ? 24 : 26;
        for (row = 0; row < count; ++row)
        {
            target = account + target_offset + row;
            w_u8(target, 0);
            for (bit = 0; bit < 8; ++bit)
            {
                first = r_u8(statistics + source_offset + row * 8 + bit);
                second = r_u8(target);
                w_u8(target, second | ((first & 1) << bit));
            }
        }
    }
    for (index = 0; index < 60; ++index)
    {
        value = r_u16(statistics + 104 + index * 2);
        w_u16(account + 34 + index * 2, value);
        value = r_u16(statistics + 224 + index * 2);
        w_u16(account + 154 + index * 2, value);
        value = r_u16(statistics + 344 + index * 2);
        w_u16(account + 274 + index * 2, value);
        value = r_u16(statistics + 464 + index * 2);
        w_u16(account + 394 + index * 2, value);
    }
    for (index = 0; index < 30; ++index)
    {
        for (group = 0; group < 4; ++group)
        {
            first = r_u8(statistics + 704 + index * 2 + group * 60);
            second = r_u8(statistics + 705 + index * 2 + group * 60);
            w_u8(account + 514 + index + group * 30, (first & 15) | ((second & 15) << 4));
        }
    }
    for (group = 0; group < 2; ++group)
    {
        count = group ? 45 : 20;
        source_offset = group ? 1084 : 1004;
        target_offset = group ? 654 : 634;
        for (row = 0; row < count; ++row)
        {
            target = account + target_offset + row;
            w_u8(target, 0);
            for (bit = 0; bit < 4; ++bit)
            {
                first = r_u8(statistics + source_offset + row * 4 + bit);
                second = r_u8(target);
                w_u8(target, second | ((first & 3) << (bit * 2)));
            }
        }
    }
    display = r_u32(0x800BCE48);
    value = r_u16(display + 8);
    w_u16(account + 1120, value);
    value = r_u16(display + 12);
    game = r_u32(0x800BCE60);
    w_u16(account + 1122, value);
    value = r_u8(game + 920);
    w_u8(account + 1117, value);
    value = r_u8(r_u32(0x800BCE60) + 921);
    w_u8(account + 1118, value);
    value = r_u8(r_u32(0x800BCE60) + 922);
    w_u8(account + 1116, value);
    value = r_u16(statistics + 1748);
    game = r_u32(0x800BCE60);
    w_u16(account + 704, value);
    value = r_u16(statistics + 1752);
    w_u16(account + 706, value);
    value = r_u8(statistics + 1);
    w_u32(account + 1128, 0);
    w_u16(account + 20, value);
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
    value = r_u8(r_u32(0x800BCE60) + 2394);
    result = 1;
    if (value == 1)
    {
        result = r_u32(account + 1128) | 8;
        w_u32(account + 1128, result);
    }
    for (index = 17; index > 0; --index)
        w_u8(account + 1131 + index, 0);
    return result;
}

uint32 resource002_sub_800e8fc4(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800E8FC4, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3074);
    resource002_sub_800deca8(1, 1);
    resource002_sub_800dee08(2, 0, 0, 0);
    resource002_sub_800dee08(3, 1, 0, 20);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 55);
    w_u8(r_u32(0x80112DF8) + 9, 1);
    w_u8(r_u32(0x80112DF8) + 3, 0);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e9068(void)
{
    uint32 status, phase, game;
    FUNCTION_MARKER(0x800E9068, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    phase = resource002_sub_800dee84(3);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        return 0;
    }
    if (status == 0)
        return 0;
    if ((r_u32(r_u32(0x800BCE60) + 300) & 64) || phase == 0)
    {
        if (phase == 0 || phase == 3)
        {
            if (r_u8(r_u32(0x80112DF8) + 3) == 1)
            {
                resource002_sub_800dee08(3, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 10);
                w_u8(r_u32(0x80112DF8) + 1, 0);
                w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 47);
            }
        }
        else
        {
            resource002_sub_800dee08(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
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
                resource002_sub_800dee08(3, 0, 0, 0);
                w_u8(r_u32(0x80112DF8), 3);
                w_u8(r_u32(0x80112DF8) + 1, 0);
            }
        }
        else
        {
            resource002_sub_800dee08(3, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
            w_u8(r_u32(0x80112DF8) + 3, 2);
            sub_8002db4c(59);
        }
    }
    return 0;
}

uint32 resource002_sub_800e927c(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E927C, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 5 || status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), status == 5 ? 11 : 1);
        context = r_u32(0x80112DF8);
        w_u8(context + 1, 0);
        return context;
    }
    if (status)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1) + 1;
        w_u8(context + 1, value);
        return value;
    }
    return 6;
}

uint32 resource002_sub_800e9310(void)
{
    uint32 result, context, value;
    FUNCTION_MARKER(0x800E9310, "ACE3.BIN");
    result = resource002_sub_800e4db0(r_u8(r_u32(0x80112DF8) + 4));
    if (result == 0xFFFFFFFFu)
        return result;
    if ((sint32)result < 2)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 56);
        resource002_sub_800dedf0(1, 3073);
        return resource002_sub_800e25d4();
    }
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 2;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e93b8(void)
{
    uint32 status, game, value;
    FUNCTION_MARKER(0x800E93B8, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (status)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            value = r_u32(r_u32(0x80112DC4) + 224);
            value = resource002_sub_800e69a8(value);
            w_u8(r_u32(0x80112DF8), value);
            w_u8(r_u32(0x80112DF8) + 1, 0);
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
        {
            w_u8(r_u32(0x80112DF8), 1);
            w_u8(r_u32(0x80112DF8) + 1, 0);
        }
    }
    return 0;
}

uint32 resource002_sub_800e965c(void)
{
    uint32 status, game, value;
    FUNCTION_MARKER(0x800E965C, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    w_u8(r_u32(0x80112DF8) + 9, 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 80);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (status)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
        {
            value = r_u32(r_u32(0x80112DC4) + 224);
            value = resource002_sub_800e69a8(value);
            w_u8(r_u32(0x80112DF8), value);
            w_u8(r_u32(0x80112DF8) + 1, 0);
            sub_8002db4c(60);
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
        {
            w_u8(r_u32(0x80112DF8), 0);
            sub_8002db4c(59);
        }
    }
    return 1;
}

uint32 resource002_sub_800e4db0(uint32 port)
{
    uint8 pattern[16], record[40];
    uint32 state, count = 0, offset = 0, target, bytes, adjusted, blocks, remaining;
    const uint8 *terminator;
    void *found, *destination;
    FUNCTION_MARKER(0x800E4DB0, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0xFFFFFFFFu;
    state = r_u32(0x80112DF0);
    w_u16(state + port * 2 + 84, 15);
    game_sprintf_native(pattern, sizeof(pattern), 0x800CCE10, &port, 1);
    found = firstfile((const char *)pattern, record);
    while (found)
    {
        state = r_u32(0x80112DF0);
        target = state + port * 300 + 92 + offset;
        terminator = memchr(record, 0, sizeof(record));
        if (!terminator)
            abort();
        bytes = (uint32)(terminator - record) + 1;
        destination = psx_addr(target, bytes);
        if (!destination)
            abort();
        strcpy((char *)destination, (const char *)record);
        bytes = xport_load_le32(record + 24);
        state = r_u32(0x80112DF0);
        adjusted = bytes + 8191;
        if ((sint32)adjusted < 0)
            adjusted = bytes + 16382;
        offset += 20;
        ++count;
        remaining = r_u16(state + port * 2 + 84);
        blocks = (uint32)((sint32)adjusted >> 13);
        w_u16(state + port * 2 + 84, remaining - blocks);
        found = nextfile(record);
    }
    w_u16(r_u32(0x80112DF0) + port * 2 + 88, count);
    resource002_sub_800e25d4();
    return (uint32)r_s16(r_u32(0x80112DF0) + port * 2 + 84);
}

uint32 resource002_sub_800e97a4(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800E97A4, "ACE3.BIN");
    resource002_sub_800dedf0(1, 3088);
    resource002_sub_800deca8(1, 4);
    resource002_sub_800dee08(1, 1, 0, 60);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 57);
    w_u8(r_u32(0x80112DF8) + 9, 1);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e9824(void)
{
    FUNCTION_MARKER(0x800E9824, "ACE3.BIN");
    return resource002_sub_800e982c(r_u32(0x80112DF8));
}

uint32 resource002_sub_800e982c(uint32 context)
{
    uint32 status, index, value, object, game, input;
    FUNCTION_MARKER(0x800E982C, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(context + 4));
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, status ? 57 : 47);
    w_u8(r_u32(0x80112DF8) + 10, 0);
    for (index = 0; index < 3; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 5);
        if (value == index)
            value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 3 : 0;
        else
            value = 1;
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + index + 2, value);
    }
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    resource002_sub_800e6a6c(r_u8(r_u32(0x80112DF8) + 10), 0);
    if (!status)
        return 0;
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        w_u8(r_u32(0x80112DF8), 12);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        sub_8002db4c(60);
        return 0;
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        w_u8(r_u32(0x80112DF8), 0);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        sub_8002db4c(59);
        return 0;
    }
    input = r_u32(r_u32(0x800BCE60) + 304);
    if (input & 2)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 5);
        w_u8(context + 5, value < 2 ? value + 1 : 0);
    }
    else if (input & 1)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 5);
        w_u8(context + 5, value ? value - 1 : 2);
    }
    else
        return 0;
    sub_8002db4c(61);
    return 0;
}

uint32 resource002_sub_800e9ab4(void)
{
    uint32 context, value, index, object;
    FUNCTION_MARKER(0x800E9AB4, "ACE3.BIN");
    value = resource002_sub_800e4324(r_u8(r_u32(0x80112DF8) + 5));
    if (value)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        w_u16(r_u32(0x80112DF8) + 26, 1);
        for (index = 0; index < 3; ++index)
        {
            value = r_u8(r_u32(0x80112DF8) + 5);
            if (value == index)
                value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 3 : 0;
            else
                value = 1;
            object = r_u32(r_u32(0x80112DC0) + 92);
            w_u8(object + index + 2, value);
        }
        resource002_sub_800dedf0(1, 3104);
        resource002_sub_800deca8(1, 5);
        resource002_sub_800dee08(1, 1, 0, 60);
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 58);
    }
    else
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 2);
        context = r_u32(0x80112DF8);
        value = r_u8(context + 5);
        w_u16(context + 26, 0);
        resource002_sub_800e3658(value);
    }
    context = r_u32(0x80112DF8);
    return resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
}

uint32 resource002_sub_800e4324(uint32 account)
{
    FUNCTION_MARKER(0x800E4324, "ACE3.BIN");
    if (account >= 3)
        return 0xFFFFFFFFu;
    return (r_u8(r_u32(r_u32(0x80112DF0) + 744) + 8) >> account) & 1;
}

uint32 resource002_sub_800e3658(uint32 account)
{
    uint32 state, pointer_field, source, destination, bytes;
    const char *name;
    char *target;
    FUNCTION_MARKER(0x800E3658, "ACE3.BIN");
    if (account == 0xFFFFFFFFu)
        pointer_field = r_u32(0x80112DF0) + 760;
    else
    {
        if ((sint32)account >= 3)
            return 0x80110000;
        state = r_u32(0x80112DF0);
        pointer_field = state + account * 4 + 748;
    }
    w_u16(r_u32(pointer_field) + 22, 0);
    source = r_u32(0x800BCE60) + 2376;
    destination = r_u32(pointer_field) + 8;
    name = game_native_string(source);
    bytes = (uint32)strlen(name) + 1;
    target = psx_addr(destination, bytes);
    if (!target)
        abort();
    strcpy(target, name);
    return destination;
}

uint32 resource002_sub_800e456c(void)
{
    FUNCTION_MARKER(0x800E456C, "ACE3.BIN");
    return r_u8(r_u32(r_u32(0x80112DF0) + 744) + 8);
}

uint32 resource002_sub_800e425c(uint32 account)
{
    FUNCTION_MARKER(0x800E425C, "ACE3.BIN");
    if (!account)
        return r_u32(r_u32(0x80112DF0) + 760) + 8;
    if ((sint32)account >= 4)
        return 0x800CCDB4;
    return r_u32(r_u32(0x80112DF0) + (account - 1) * 4 + 748) + 8;
}

uint32 resource002_sub_800e6a6c(uint32 hide_selected, uint32 layout)
{
    uint32 present, value, index, name, color, result = 0;
    FUNCTION_MARKER(0x800E6A6C, "ACE3.BIN");
    present = resource002_sub_800e456c();
    if (hide_selected == 1)
        present &= ~(1u << (r_u8(r_u32(0x80112DF8) + 5) & 31));
    resource002_sub_800f35b8(4);
    resource002_sub_800f33f8(2);
    resource002_sub_800f34c8(1, 10, 20);
    value = resource002_sub_800deea8(15);
    resource002_sub_800f34b0(value);
    for (index = 0; index < 3; ++index)
    {
        if (layout == 0 || layout == 1)
            resource002_sub_800f33a8(layout ? 56 : 81, 214 + index * 53);
        color = (present & (1u << index)) ? 128 : 64;
        resource002_sub_800f33d0(color, color, color);
        name = (present & (1u << index)) ? resource002_sub_800e425c(index + 1) : 0x800CCE40;
        result = resource002_sub_800f4ae0(name, 0, NULL, 0);
    }
    return result;
}

uint32 resource002_sub_800e9c40(void)
{
    uint32 status, index, value, object, context, game;
    FUNCTION_MARKER(0x800E9C40, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    resource002_sub_800e6a6c(r_u8(r_u32(0x80112DF8) + 10), 1);
    for (index = 0; index < 3; ++index)
    {
        value = r_u8(r_u32(0x80112DF8) + 5);
        if (value == index)
            value = (r_u32(r_u32(0x800BCE60) + 292) & 16) ? 3 : 0;
        else
            value = 1;
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + index + 2, value);
    }
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 3);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        w_u16(r_u32(0x80112DF8) + 26, 0);
    }
    if (!status)
        return 0;
    if (r_u32(r_u32(0x800BCE60) + 300) & 64)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
        sub_8002db4c(60);
        resource002_sub_800e3658(r_u8(r_u32(0x80112DF8) + 5));
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) && !(r_u32(game + 296) & 0x5F))
    {
        w_u8(r_u32(0x80112DF8), 11);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        w_u16(r_u32(0x80112DF8) + 26, 0);
        sub_8002db4c(59);
    }
    return 0;
}

uint32 resource002_sub_800e9e20(void)
{
    uint32 status, context, value;
    FUNCTION_MARKER(0x800E9E20, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    context = r_u32(0x80112DF8);
    resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    if (status == 4)
    {
        w_u8(context + 1, value + 1);
        context = r_u32(r_u32(0x80112DC0) + 92);
        value = 59;
    }
    else
        value += 3;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e9ec4(void)
{
    uint32 context, result, value;
    FUNCTION_MARKER(0x800E9EC4, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
    result = resource002_sub_800e47b0(r_u8(r_u32(0x80112DF8) + 4));
    if (result == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 4);
    }
    if (result != 1)
        return 1;
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e9f64(void)
{
    uint32 context, result, value;
    FUNCTION_MARKER(0x800E9F64, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
    result = resource002_sub_800e5664(r_u8(r_u32(0x80112DF8) + 4), 0);
    if (!result)
        return 0;
    if (result == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 3);
    }
    if (result != 1)
        return 1;
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1);
    w_u8(context + 1, value + 1);
    return resource002_sub_800e3b5c();
}

uint32 resource002_sub_800ea004(void)
{
    uint32 context, result, value;
    FUNCTION_MARKER(0x800EA004, "ACE3.BIN");
    context = r_u32(0x80112DF8);
    resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
    context = r_u32(0x80112DF8);
    result = resource002_sub_800e57f0(r_u8(context + 4), r_u8(context + 5), 0);
    if (result == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 2);
    }
    if (result != 1)
        return 1;
    resource002_sub_800e2ab4();
    resource002_sub_800e2a7c();
    resource002_sub_800e3658(r_u8(r_u32(0x80112DF8) + 5));
    resource002_sub_800e3658(0xFFFFFFFFu);
    context = r_u32(0x80112DF8);
    value = r_u8(context + 1) + 1;
    w_u8(context + 1, value);
    return value;
}

uint32 resource002_sub_800e2a7c(void)
{
    uint32 state, game;
    FUNCTION_MARKER(0x800E2A7C, "ACE3.BIN");
    state = r_u32(0x80112DF0);
    game = r_u32(0x800BCE60);
    return resource002_sub_800e2a8c(state, game);
}

uint32 resource002_sub_800e2a8c(uint32 state, uint32 game)
{
    uint32 destination, bytes;
    const char *name;
    char *target;
    FUNCTION_MARKER(0x800E2A8C, "ACE3.BIN");
    destination = r_u32(state + 760) + 8;
    name = game_native_string(game + 2376);
    bytes = (uint32)strlen(name) + 1;
    target = psx_addr(destination, bytes);
    if (!target)
        abort();
    strcpy(target, name);
    return destination;
}

uint32 resource002_sub_800ea0c8(void)
{
    FUNCTION_MARKER(0x800EA0C8, "ACE3.BIN");
    return resource002_sub_800ea0d0(r_u32(0x80112DC4));
}

uint32 resource002_sub_800ea0d0(uint32 state)
{
    uint32 context, result, value;
    FUNCTION_MARKER(0x800EA0D0, "ACE3.BIN");
    if (r_u32(state + 224) < 2)
    {
        context = r_u32(0x80112DF8);
        resource002_sub_800e6a6c(r_u8(context + 10), (uint32)r_s16(context + 26));
    }
    context = r_u32(0x80112DF8);
    result = resource002_sub_800e5a28(r_u8(context + 4), r_u8(context + 5), 0xFFFFFFFFu, 1);
    if (result == 0xFFFFFFFFu)
    {
        context = r_u32(0x80112DF8);
        value = r_u8(context + 1);
        w_u8(context + 1, value + 1);
    }
    if (result != 1)
        return 1;
    value = r_u32(r_u32(0x80112DC4) + 224) - 2;
    w_u8(r_u32(0x80112DF8), value < 3 ? 16 : 13);
    context = r_u32(0x80112DF8);
    w_u8(context + 1, 0);
    return context;
}

uint32 resource002_sub_800ea1c4(void)
{
    uint32 status;
    FUNCTION_MARKER(0x800EA1C4, "ACE3.BIN");
    status = resource002_sub_800e267c(r_u8(r_u32(0x80112DF8) + 4));
    w_u8(r_u32(0x80112DF8) + 9, 1);
    resource002_sub_800dee08(1, 2, 20, 20);
    resource002_sub_800dedf0(1, 3200);
    resource002_sub_800deca8(1, 7);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 1, 61);
    if (status == 2 || status == 6)
    {
        w_u8(r_u32(0x80112DF8), 1);
        w_u8(r_u32(0x80112DF8) + 1, 0);
    }
    if (status && r_u32(r_u32(0x800BCE60) + 300))
    {
        w_u8(r_u32(0x80112DF8) + 9, 0);
        resource002_sub_800dee08(1, 0, 0, 0);
        w_u8(r_u32(0x80112DF8), 0);
        w_u8(r_u32(0x80112DF8) + 1, 0);
        sub_8002db4c(59);
    }
    return 1;
}

uint32 resource002_sub_800e5a28(uint32 port, uint32 account, uint32 slot, uint32 finalize)
{
    uint32 state, buffer, source, value, checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5A28, "ACE3.BIN");
    if (!resource002_sub_800e264c())
        return 0;
    state = r_u32(0x80112DF0);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
    }
    if (slot < 6)
    {
        state = r_u32(0x80112DF0);
        w_u16(r_u32(state + 760) + 20, slot);
        source = r_u32(state + account * 4 + 748);
        buffer = r_u32(state + 760);
        value = r_u16(source + 22);
        w_u16(buffer + 22, value);
        buffer = r_u32(state + 760);
        value = r_u16(buffer + 22);
        w_u16(buffer + 22, value | (1u << slot));
    }
    else if (slot == 6)
    {
        state = r_u32(0x80112DF0);
        w_u16(r_u32(state + 760) + 20, 0);
        w_u16(r_u32(state + 760) + 22, 0);
    }
    state = r_u32(0x80112DF0);
    if (!r_u8(state + 822))
    {
        checksum = 0x41434533;
        buffer = r_u32(state + 760);
        for (index = 0; index < 287; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x80112DF0);
        buffer = r_u32(state + 760);
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
            resource002_sub_800e25d4();
            w_u16(r_u32(0x80112DF0) + 858, 0);
            return 0xFFFFFFFFu;
        }
        game_bios_seek_guest(descriptor, account * 1152 + 768, 0);
        state = r_u32(0x80112DF0);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + 760);
        game_bios_write_guest(descriptor, buffer, 1152);
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
        w_u16(state + 858, 0);
        resource002_sub_800e25d4();
        return 0xFFFFFFFFu;
    }
    if (finalize == 1)
        resource002_sub_800e25d4();
    return 1;
}
