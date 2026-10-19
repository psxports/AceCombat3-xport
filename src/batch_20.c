#include "game_functions.h"
#include <stdlib.h>

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 resource001_sub_800cfd28(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CFD28, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    return resource001_sub_800da9a8(1, 26);
}

uint32 resource001_sub_800cfd60(void)
{
    uint32 first, second, state;

    FUNCTION_MARKER(0x800CFD60, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if (first == 0 || first == 6 || second == 0 || second == 6)
        return second;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 10);
    w_u16(state + 78, 0);
    return resource001_sub_800da9a8(4, 21);
}

uint32 resource001_sub_800cfdcc(void)
{
    uint32 first, second, state, step;

    FUNCTION_MARKER(0x800CFDCC, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        resource001_sub_800da838(1, 3, -1, -1);
        resource001_sub_800da838(2, 1, 0, 20);
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 24);
        resource001_sub_800da49c(23);
        state = r_u32(0x800DD9D0);
        w_u8(state + 209, 1);
        return resource001_sub_800da828(0x20056);
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 11);
    w_u16(state + 78, 0);
    return 11;
}

uint32 resource001_sub_800cfea4(void)
{
    uint32 first, second, state, game, buttons, value;

    FUNCTION_MARKER(0x800CFEA4, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first != 2 && first != 6) || (second != 2 && second != 6))
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        value = resource001_sub_800d4140();
        game = r_u32(0x800BCE60);
        w_u32(game + 0x968, value);
        resource001_sub_800d4158();
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800cff90(void)
{
    uint32 first, second, state, step;

    FUNCTION_MARKER(0x800CFF90, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(18);
    resource001_sub_800da828(0x40036);
    resource001_sub_800da838(4, 1, 0, 60);
    resource001_sub_800da9a8(1, 25);
    resource001_sub_800da9a8(4, 2);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    resource001_sub_800da49c(24);
    if (first != 5 && second == 5)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 1);
    }
    state = r_u32(0x800DD9D0);
    w_u8(state + 209, 0);
    return state;
}

uint32 resource001_sub_800d0074(void)
{
    uint32 game, state, directions, previous, first, second, value, buttons;

    FUNCTION_MARKER(0x800D0074, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    state = r_u32(0x800DD9D0);
    directions = r_u32(game + 304);
    previous = r_u8(state + 90);
    if (directions & 3)
        w_u8(state + 90, previous ^ 1);
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 209, 1);
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 10);
        w_u16(state + 78, 0);
        resource001_sub_800da77c(17);
        resource001_sub_800da828(0x20056);
        resource001_sub_800da838(2, 1, 0, 20);
        return resource001_sub_800da838(1, 3, -1, -1);
    }
    if (first == 0 || first == 2 || first == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 1);
    }
    if (second == 0 || second == 2 || second == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 0);
    }
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 90);
    if (value != previous)
        sub_8002db4c(61);
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 90);
    if (value == 0)
    {
        resource001_sub_800da9a8(6, 0);
        resource001_sub_800da9a8(7, 1);
    }
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 90);
    if (value == 1)
    {
        resource001_sub_800da9a8(6, 1);
        resource001_sub_800da9a8(7, 0);
    }
    if (first == 0 || first == 2)
        resource001_sub_800da9a8(6, 2);
    if (second == 0 || second == 2)
        resource001_sub_800da9a8(7, 2);
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 12);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        return resource001_sub_800da49c(25);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d02dc(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D02DC, "ACE3.BIN");
    resource001_sub_800da9a8(1, 26);
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value != 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 13);
        return 13;
    }
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(17);
    resource001_sub_800da828(0x20056);
    resource001_sub_800da9a8(1, 28);
    resource001_sub_800da9a8(4, 21);
    return resource001_sub_800da49c(26);
}

uint32 resource001_sub_800d0384(void)
{
    uint32 state, port, value, game, buttons;

    FUNCTION_MARKER(0x800D0384, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        value = resource001_sub_800d4140();
        game = r_u32(0x800BCE60);
        w_u32(game + 0x968, value);
        resource001_sub_800d4158();
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d0460(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0460, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value != 3)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 14);
        return 14;
    }
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(19);
    resource001_sub_800da828(0x80056);
    resource001_sub_800da838(1, 3, -1, -1);
    resource001_sub_800da838(2, 3, -1, -1);
    resource001_sub_800da838(3, 3, -1, -1);
    resource001_sub_800da838(4, 1, 0, 20);
    resource001_sub_800da9a8(1, 30);
    resource001_sub_800da9a8(4, 21);
    return resource001_sub_800da49c(27);
}

uint32 resource001_sub_800d0548(void)
{
    uint32 state, port, value, game, buttons, step;

    FUNCTION_MARKER(0x800D0548, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    if (value == 0)
        return 0x800C0000;
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x40)
    {
        resource001_sub_800da9a8(1, 32);
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        sub_8002db4c(89);
        return resource001_sub_800da49c(28);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d061c(void)
{
    uint32 state, port, value;

    FUNCTION_MARKER(0x800D061C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d5e74(port);
    if (value == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 14);
    w_u16(state + 78, 3);
    return state;
}

uint32 resource001_sub_800d0668(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0668, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 4)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78) + 1;
        w_u16(state + 78, step);
        return step;
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 15);
    return resource001_sub_800da9a8(1, 26);
}

uint32 resource001_sub_800d06d4(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D06D4, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d64c4(port);
    if (value == 0xFFFFFFFF)
        return 1;
    if ((sint32)value < 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 34);
        resource001_sub_800da49c(30);
        return resource001_sub_800da828(0x20016);
    }
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 2;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d076c(void)
{
    uint32 state, port, value, game, buttons;

    FUNCTION_MARKER(0x800D076C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        value = resource001_sub_800d4140();
        game = r_u32(0x800BCE60);
        w_u32(game + 0x968, value);
        resource001_sub_800d4158();
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d5e74(uint32 port)
{
    uint32 result;

    FUNCTION_MARKER(0x800D5E74, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    result = (uint32)_card_format((sint32)(port << 4));
    resource001_sub_800d3ce8();
    return result == 1 ? 1 : 0xFFFFFFFF;
}

uint32 resource001_sub_800d64c4(uint32 port)
{
    uint8 pattern[16], directory[40];
    uint32 state, count = 0, offset, base, index, value, size, adjusted, slots;
    void *found;

    FUNCTION_MARKER(0x800D64C4, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0xFFFFFFFF;
    state = r_u32(0x800DDA48);
    w_u16(state + port * 2 + 84, 15);
    game_sprintf_native(pattern, sizeof(pattern), 0x800CC55C, &port, 1);
    found = firstfile((const char *)pattern, directory);
    if (found)
    {
        base = port * 300 + 92;
        offset = 0;
        do
        {
            state = r_u32(0x800DDA48);
            for (index = 0;; ++index)
            {
                if (index >= sizeof(directory))
                    abort();
                value = directory[index];
                w_u8(state + base + offset + index, value);
                if (value == 0)
                    break;
            }
            size = xport_load_le32(directory + 24);
            state = r_u32(0x800DDA48);
            adjusted = size + 0x1FFF;
            if ((sint32)adjusted < 0)
                adjusted = size + 0x3FFE;
            offset += 20;
            ++count;
            slots = r_u16(state + port * 2 + 84);
            w_u16(state + port * 2 + 84, slots - (uint32)((sint32)adjusted >> 13));
            found = nextfile(directory);
        } while (found);
    }
    state = r_u32(0x800DDA48);
    w_u16(state + port * 2 + 88, count);
    resource001_sub_800d3ce8();
    state = r_u32(0x800DDA48);
    return (uint32)r_s16(state + port * 2 + 84);
}

uint32 resource001_sub_800d0848(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0848, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d5ec4(port);
    if (value == 1)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78) + 1;
        w_u16(state + 78, step);
        return step;
    }
    if (value != 0xFFFFFFFF)
        return 0xFFFFFFFF;
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 3);
    resource001_sub_800da9a8(1, 35);
    resource001_sub_800da49c(32);
    return resource001_sub_800da828(0x20016);
}

uint32 resource001_sub_800d08dc(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D08DC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d6d78(port, 1);
    if (value == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 2);
        resource001_sub_800da9a8(1, 35);
        resource001_sub_800da49c(32);
        resource001_sub_800da828(0x20016);
    }
    if (value != 1)
        return 1;
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d0978(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0978, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d6f04(port, -1, 1);
    if (value == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 35);
        resource001_sub_800da49c(32);
        resource001_sub_800da828(0x20016);
    }
    if (value == 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 16);
        w_u16(state + 78, 0);
    }
    return 16;
}

uint32 resource001_sub_800d0a10(void)
{
    uint32 state, port, value, game, buttons;

    FUNCTION_MARKER(0x800D0A10, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        value = resource001_sub_800d4140();
        game = r_u32(0x800BCE60);
        w_u32(game + 0x968, value);
        resource001_sub_800d4158();
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d5ec4(uint32 port)
{
    uint32 state, descriptor;

    FUNCTION_MARKER(0x800D5EC4, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = game_bios_open_guest(state + port * 26 + 692, 0x20200, 26);
    state = r_u32(0x800DDA48);
    w_u32(state + 60, descriptor);
    game_bios_close_guest(descriptor);
    resource001_sub_800d3ce8();
    return descriptor == 0xFFFFFFFF ? 0xFFFFFFFF : 1;
}

uint32 resource001_sub_800d6d78(uint32 port, uint32 finalize)
{
    uint32 state, descriptor, buffer, outcome, retry;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6D78, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x800DDA48);
    }
    if (r_u8(state + 822) == 0)
    {
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8002, 26);
        state = r_u32(0x800DDA48);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            resource001_sub_800d3ce8();
            state = r_u32(0x800DDA48);
            w_u16(state + 858, 0);
            return 0xFFFFFFFF;
        }
        buffer = r_u32(0x800DDA44);
        game_bios_write_guest(descriptor, buffer, 640);
        state = r_u32(0x800DDA48);
        w_u8(state + 822, 1);
    }
    outcome = resource001_sub_800d3ea8(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = r_u32(state + 60);
    game_bios_close_guest(descriptor);
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    if (outcome != 0)
    {
        state = r_u32(0x800DDA48);
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        w_u16(state + 858, 0);
        resource001_sub_800d3ce8();
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        resource001_sub_800d3ce8();
    return 1;
}

uint32 resource001_sub_800d6f04(uint32 port, sint32 slot, uint32 finalize)
{
    uint32 state, header, value, checksum, index, descriptor, outcome, retry;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6F04, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x800DDA48);
    }
    if (r_u8(state + 822) == 0)
    {
        header = r_u32(state + 744);
        if (slot >= 0)
        {
            value = r_u8(header + 8);
            w_u8(header + 8, value | ((uint32)1 << ((uint32)slot & 31)));
            state = r_u32(0x800DDA48);
            header = r_u32(state + 744);
            value = r_u32(header + 12);
            w_u32(header + 12, value + 1);
        }
        else
        {
            w_u8(header + 8, 0);
            state = r_u32(0x800DDA48);
            header = r_u32(state + 744);
            w_u32(header + 12, 0);
        }
        checksum = 0x41434533;
        state = r_u32(0x800DDA48);
        header = r_u32(state + 744);
        for (index = 0; index < 31; ++index)
            checksum ^= r_u32(header + 4 + index * 4);
        state = r_u32(0x800DDA48);
        header = r_u32(state + 744);
        w_u32(header, checksum);
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8002, 26);
        state = r_u32(0x800DDA48);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            resource001_sub_800d3ce8();
            state = r_u32(0x800DDA48);
            w_u16(state + 858, 0);
            return 0xFFFFFFFF;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        state = r_u32(0x800DDA48);
        descriptor = r_u32(state + 60);
        header = r_u32(state + 744);
        game_bios_write_guest(descriptor, header, 128);
        state = r_u32(0x800DDA48);
        w_u8(state + 822, 1);
    }
    outcome = resource001_sub_800d3ea8(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = r_u32(state + 60);
    game_bios_close_guest(descriptor);
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    if (outcome != 0)
    {
        state = r_u32(0x800DDA48);
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        resource001_sub_800d3ce8();
        state = r_u32(0x800DDA48);
        w_u16(state + 858, 0);
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        resource001_sub_800d3ce8();
    return 1;
}

uint32 resource001_sub_800d0aec(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800D0AEC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d0b0c(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0B0C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d6604(port, 1);
    if (value == 0xFFFFFFFF || value == 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 2);
        resource001_sub_800da9a8(1, value == 2 ? 37 : 35);
        resource001_sub_800da828(0x20056);
        resource001_sub_800da49c(32);
        resource001_sub_800da9a8(4, 21);
    }
    if (value != 1)
        return 1;
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d0bfc(void)
{
    uint32 state, port, value, step;

    FUNCTION_MARKER(0x800D0BFC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d6814(port, 1);
    if (value == 0xFFFFFFFF || value == 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, value == 2 ? 37 : 35);
        if (value == 2)
            resource001_sub_800da77c(6);
        resource001_sub_800da828(0x20056);
        resource001_sub_800da49c(32);
        resource001_sub_800da9a8(4, 21);
    }
    if (value == 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 16);
        w_u16(state + 78, 0);
    }
    return 16;
}

uint32 resource001_sub_800d0cec(void)
{
    uint32 state, port, value, game, buttons;

    FUNCTION_MARKER(0x800D0CEC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        value = resource001_sub_800d4140();
        game = r_u32(0x800BCE60);
        w_u32(game + 0x968, value);
        resource001_sub_800d4158();
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 7);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d6604(uint32 port, uint32 finalize)
{
    uint32 state, descriptor, header, outcome, retry, checksum, index, value;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6604, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x800DDA48);
    }
    if (r_u8(state + 822) == 0)
    {
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
        state = r_u32(0x800DDA48);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            resource001_sub_800d3ce8();
            state = r_u32(0x800DDA48);
            w_u16(state + 858, 0);
            return 0xFFFFFFFF;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        state = r_u32(0x800DDA48);
        descriptor = r_u32(state + 60);
        header = r_u32(state + 744);
        game_bios_read_guest(descriptor, header, 128);
        state = r_u32(0x800DDA48);
        w_u8(state + 822, 1);
    }
    outcome = resource001_sub_800d3ea8(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = r_u32(state + 60);
    game_bios_close_guest(descriptor);
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    checksum = 0x41434533;
    state = r_u32(0x800DDA48);
    header = r_u32(state + 744);
    for (index = 0; index < 31; ++index)
        checksum ^= r_u32(header + 4 + index * 4);
    state = r_u32(0x800DDA48);
    header = r_u32(state + 744);
    value = r_u32(header);
    if (value != checksum || r_u32(header + 4) != 0x00990426)
    {
        resource001_sub_800d3ce8();
        state = r_u32(0x800DDA48);
        w_u16(state + 858, 0);
        return 2;
    }
    if (outcome != 0)
    {
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        w_u16(state + 858, 0);
        resource001_sub_800d3ce8();
        return 0xFFFFFFFF;
    }
    w_u16(state + 858, 0);
    if (finalize == 1)
        resource001_sub_800d3ce8();
    return 1;
}

uint32 resource001_sub_800d6814(uint32 port, uint32 finalize)
{
    uint32 state, header, flags, descriptor, buffer, outcome, retry, checksum, index, value, offset;
    sint32 slot, attempts;

    FUNCTION_MARKER(0x800D6814, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    header = r_u32(state + 744);
    slot = r_s16(state + 80);
    flags = r_u8(header + 8);
    if (((flags >> ((uint32)slot & 31)) & 1) == 0)
        goto advance_slot;
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x800DDA48);
    }
    if (r_u8(state + 822) == 0)
    {
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
        state = r_u32(0x800DDA48);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            goto failed;
        }
        slot = r_s16(state + 80);
        offset = (uint32)slot * 1152 + 768;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        state = r_u32(0x800DDA48);
        slot = r_s16(state + 80);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + (uint32)slot * 4 + 748);
        game_bios_read_guest(descriptor, buffer, 1152);
        state = r_u32(0x800DDA48);
        w_u8(state + 822, 1);
    }
    outcome = resource001_sub_800d3ea8(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = r_u32(state + 60);
    game_bios_close_guest(descriptor);
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    if (outcome != 0)
    {
        state = r_u32(0x800DDA48);
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        goto failed;
    }
    checksum = 0x41434533;
    state = r_u32(0x800DDA48);
    slot = r_s16(state + 80);
    buffer = r_u32(state + (uint32)slot * 4 + 748);
    for (index = 0; index < 287; ++index)
        checksum ^= r_u32(buffer + 4 + index * 4);
    state = r_u32(0x800DDA48);
    slot = r_s16(state + 80);
    buffer = r_u32(state + (uint32)slot * 4 + 748);
    value = r_u32(buffer);
    if (value != checksum)
    {
        resource001_sub_800d3ce8();
        state = r_u32(0x800DDA48);
        w_u16(state + 858, 0);
        w_u16(state + 80, 0);
        return 2;
    }
advance_slot:
    state = r_u32(0x800DDA48);
    slot = r_s16(state + 80);
    retry = r_u16(state + 80);
    if (slot < 2)
    {
        w_u16(state + 80, retry + 1);
        return 0;
    }
    w_u16(state + 80, 0);
    if (finalize == 1)
        resource001_sub_800d3ce8();
    return 1;
failed:
    resource001_sub_800d3ce8();
    state = r_u32(0x800DDA48);
    w_u16(state + 858, 0);
    w_u16(state + 80, 0);
    return 0xFFFFFFFF;
}

uint32 resource001_sub_800dc028(uint32 mode)
{
    uint32 state, flags;

    FUNCTION_MARKER(0x800DC028, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    flags = r_u32(state + 84);
    if (mode == 2)
        flags |= 10;
    else if (mode == 1)
        flags = (flags | 2) & 0xFFFFFFF7;
    else
        flags &= 0xFFFFFFF5;
    w_u32(state + 84, flags);
    return flags;
}

uint32 resource001_sub_800d5a38(uint32 slot)
{
    uint32 state, header, flags;

    FUNCTION_MARKER(0x800D5A38, "ACE3.BIN");
    if (slot >= 3)
        return 0xFFFFFFFF;
    state = r_u32(0x800DDA48);
    header = r_u32(state + 744);
    flags = r_u8(header + 8);
    return (flags >> slot) & 1;
}

uint32 resource001_sub_800d0dc8(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800D0DC8, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 72, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(20);
    resource001_sub_800da828(0x100056);
    resource001_sub_800da9a8(1, 36);
    resource001_sub_800da9a8(4, 22);
    resource001_sub_800dc028(1);
    state = r_u32(0x800DD9D0);
    w_u8(state + 210, 0);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    return resource001_sub_800da49c(31);
}

uint32 resource001_sub_800d0e54(void)
{
    uint32 state, port, value, game, directions, selection, buttons;

    FUNCTION_MARKER(0x800D0E54, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return resource001_sub_800dc028(0);
    }
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    if (directions & 2)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u8(state + 93, selection < 2 ? selection + 1 : 0);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    if (directions & 1)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u8(state + 93, selection != 0 ? selection - 1 : 2);
        sub_8002db4c(61);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(6, selection != 0);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(7, selection != 1);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(8, selection != 2);
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x40)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        value = resource001_sub_800d5a38(selection);
        state = r_u32(0x800DD9D0);
        if (value != 0)
            w_u16(state + 76, 17);
        else
        {
            w_u16(state + 76, 18);
            w_u8(state + 210, 1);
        }
        state = r_u32(0x800DD9D0);
        w_u16(state + 78, 0);
        return sub_8002db4c(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 11);
    w_u16(state + 78, 0);
    resource001_sub_800dc028(0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d1080(void)
{
    uint32 state, step, selection;

    FUNCTION_MARKER(0x800D1080, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(21);
    resource001_sub_800da828(0x200056);
    resource001_sub_800da9a8(1, 38);
    resource001_sub_800da9a8(4, 24);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da838(selection + 6, 1, 0, 10);
    resource001_sub_800dc028(2);
    return resource001_sub_800da49c(33);
}

uint32 resource001_sub_800d1108(void)
{
    uint32 state, port, value, selection, game, buttons;

    FUNCTION_MARKER(0x800D1108, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return resource001_sub_800dc028(0);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(6, selection != 0);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(7, selection != 1);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(8, selection != 2);
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x40)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u16(state + 76, 18);
        w_u16(state + 78, 0);
        resource001_sub_800da838(selection + 6, 0, 0, 0);
        sub_8002db4c(89);
    }
    else if (buttons & 0x20)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u16(state + 76, 16);
        w_u16(state + 78, 0);
        resource001_sub_800da838(selection + 6, 0, 0, 0);
        sub_8002db4c(59);
    }
    state = r_u32(0x800DD9D0);
    value = r_u32(state + 72) + 1;
    w_u32(state + 72, value);
    return value;
}

uint32 resource001_sub_800d12a4(void)
{
    uint32 state, step, value, game;

    FUNCTION_MARKER(0x800D12A4, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da9a8(1, 39);
    resource001_sub_800da9a8(4, 24);
    value = resource001_sub_800d4140();
    game = r_u32(0x800BCE60);
    w_u32(game + 0x968, value);
    return resource001_sub_800d4158();
}

uint32 resource001_sub_800d1300(void)
{
    uint32 state, port, selection, value, step;

    FUNCTION_MARKER(0x800D1300, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    selection = r_u8(state + 93);
    value = resource001_sub_800d6f04(port, (sint32)selection, 1);
    if (value == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 2);
        resource001_sub_800da9a8(1, 40);
        resource001_sub_800da828(0x20016);
        resource001_sub_800da49c(35);
        resource001_sub_800dc028(0);
    }
    if (value != 1)
        return 1;
    resource001_sub_800d4158();
    resource001_sub_800d41c8();
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d13b8(void)
{
    uint32 state, port, selection, value, step, game;

    FUNCTION_MARKER(0x800D13B8, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    selection = r_u8(state + 93);
    value = resource001_sub_800d713c(port, selection, 6, 1);
    if (value == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 40);
        resource001_sub_800da828(0x20056);
        resource001_sub_800da49c(35);
        resource001_sub_800dc028(0);
    }
    if (value != 1)
        return 1;
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 136, 120);
    w_u16(state + 78, step + 2);
    resource001_sub_800da1b8();
    game = r_u32(0x800BCE60);
    state = r_u32(0x800DD9D0);
    w_u16(game + 0x954, 2);
    w_u16(state + 16, 0);
    return resource001_sub_800da49c(36);
}

uint32 resource001_sub_800d1488(void)
{
    uint32 state, port, value, game, buttons;

    FUNCTION_MARKER(0x800D1488, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    value = resource001_sub_800d3d90(port);
    if (value == 2 || value == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return 9;
    }
    resource001_sub_800da9a8(1, 40);
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        state = r_u32(0x800DD9D0);
        w_u16(game + 0x954, 2);
        w_u16(state + 16, 0);
        return game;
    }
    if (buttons & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 11);
        w_u16(state + 78, 0);
    }
    return 11;
}

uint32 resource001_sub_800d1554(void)
{
    uint32 state, timer, game, buttons;

    FUNCTION_MARKER(0x800D1554, "ACE3.BIN");
    resource001_sub_800da9a8(1, 42);
    state = r_u32(0x800DD9D0);
    timer = r_u32(state + 136);
    if ((sint32)timer < 0)
    {
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        return 19;
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x30)
    {
        w_u16(state + 76, 19);
        w_u16(state + 78, 0);
        return 19;
    }
    --timer;
    w_u32(state + 136, timer);
    return timer;
}

uint32 resource001_sub_800d15c4(void)
{
    uint32 state, step, game;

    FUNCTION_MARKER(0x800D15C4, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 212, 0);
    w_u16(state + 78, step + 1);
    game = r_u32(0x800BCE60);
    w_u16(game + 908, 1);
    return resource001_sub_800dbf6c(111, 150, 0, 1);
}

uint32 resource001_sub_800d1618(void)
{
    uint32 status, state, value, step;

    FUNCTION_MARKER(0x800D1618, "ACE3.BIN");
    status = (uint32)cd_resource_state();
    if (status == 0)
        resource001_sub_800da9a8(1, 73);
    if (status == 3 || status == 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 6);
        w_u16(state + 14, 0);
        return 6;
    }
    if (status == 4 || status == 0xFFFFFFFF || status == 2)
    {
        state = r_u32(0x800DD9D0);
        value = r_u32(state + 212);
        resource001_sub_800da9a8(1, value + 70);
        sub_8002b428();
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        return resource001_sub_800da49c(37);
    }
    return 2;
}

uint32 resource001_sub_800d16d4(void)
{
    uint32 status, state, step;

    FUNCTION_MARKER(0x800D16D4, "ACE3.BIN");
    resource001_sub_800da828(0x400036);
    resource001_sub_800da9a8(4, 4);
    resource001_sub_800dc028(0);
    status = sub_8002b528();
    if (status != 3)
        return 0x800E0000;
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d1738(void)
{
    uint32 status, state, step;

    FUNCTION_MARKER(0x800D1738, "ACE3.BIN");
    status = sub_8002b528();
    if (status != 1)
        return status;
    resource001_sub_800da9a8(1, 73);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step - 2);
    resource001_sub_800da49c(38);
    state = r_u32(0x800DD9D0);
    w_u32(state + 212, 31);
    return 31;
}

uint32 resource001_sub_800d713c(uint32 port, uint32 slot, uint32 kind, uint32 finalize)
{
    uint32 state, buffer, source, value, checksum, index, descriptor, outcome, retry, offset;
    sint32 attempts;

    FUNCTION_MARKER(0x800D713C, "ACE3.BIN");
    if (resource001_sub_800d3d60() == 0)
        return 0;
    state = r_u32(0x800DDA48);
    if (r_s16(state + 858) == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
    }
    if (kind < 6)
    {
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 760);
        w_u16(buffer + 20, kind);
        source = r_u32(state + slot * 4 + 748);
        buffer = r_u32(state + 760);
        value = r_u16(source + 22);
        w_u16(buffer + 22, value);
        buffer = r_u32(state + 760);
        value = r_u16(buffer + 22);
        w_u16(buffer + 22, value | ((uint32)1 << kind));
    }
    else if (kind == 6)
    {
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 760);
        w_u16(buffer + 20, 0);
        buffer = r_u32(state + 760);
        w_u16(buffer + 22, 0);
    }
    state = r_u32(0x800DDA48);
    if (r_u8(state + 822) == 0)
    {
        checksum = 0x41434533;
        buffer = r_u32(state + 760);
        for (index = 0; index < 287; ++index)
            checksum ^= r_u32(buffer + 4 + index * 4);
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 760);
        w_u32(buffer, checksum);
        descriptor = game_bios_open_guest(state + port * 26 + 692, 0x8002, 26);
        state = r_u32(0x800DDA48);
        w_u32(state + 60, descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = r_s16(state + 856);
            retry = r_u16(state + 856);
            if (attempts < 3)
            {
                w_u16(state + 856, retry + 1);
                return 0;
            }
            resource001_sub_800d3ce8();
            state = r_u32(0x800DDA48);
            w_u16(state + 858, 0);
            return 0xFFFFFFFF;
        }
        offset = slot * 1152 + 768;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        state = r_u32(0x800DDA48);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + 760);
        game_bios_write_guest(descriptor, buffer, 1152);
        state = r_u32(0x800DDA48);
        w_u8(state + 822, 1);
    }
    outcome = resource001_sub_800d3ea8(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    state = r_u32(0x800DDA48);
    descriptor = r_u32(state + 60);
    game_bios_close_guest(descriptor);
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    if (outcome != 0)
    {
        state = r_u32(0x800DDA48);
        attempts = r_s16(state + 856);
        retry = r_u16(state + 856);
        if (attempts < 3)
        {
            w_u16(state + 856, retry + 1);
            return 0;
        }
        w_u16(state + 858, 0);
        resource001_sub_800d3ce8();
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        resource001_sub_800d3ce8();
    return 1;
}

uint32 resource001_sub_800d41c8(void)
{
    static const uint32 half_source[4] = {0x68, 0xE0, 0x158, 0x1D0};
    static const uint32 half_destination[4] = {0x22, 0x9A, 0x112, 0x18A};
    static const uint32 nibble_source[4] = {0x2C0, 0x2FC, 0x338, 0x374};
    static const uint32 nibble_destination[4] = {0x202, 0x220, 0x23E, 0x25C};
    uint32 state, game, output, data, value, previous, block, i, j, row, column;
    uint32 record, offset, group, count, source, destination, lane, partial, second, words[4], display, result;
    FUNCTION_MARKER(0x800D41C8, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    game = r_u32(0x800BCE60);
    output = r_u32(state + 760);
    value = r_u32(game + 0x968);
    data = game + 0x95C;
    w_u32(output + 4, value);
    for (block = 0; block < 6; ++block)
    {
        offset = block * 0x5C;
        record = output + 0x2C4 + block * 0x44;
        game = r_u32(0x800BCE60);
        for (i = 0; i < 20; ++i)
        {
            value = r_u16(game + offset + 0x172 + i * 2);
            w_u16(record + i * 2, value);
        }
        game = r_u32(0x800BCE60);
        /* Preserve paired masked accesses and read each group before writing */
        for (group = 0; group < 2; ++group)
        {
            count = group == 0 ? 4 : 2;
            source = game + offset + (group == 0 ? 0x19C : 0x1AC);
            destination = record + (group == 0 ? 0x28 : 0x38);
            for (i = 0; i < count; ++i)
            {
                lane = (source + i * 4 + 3) & 3;
                partial = r_masked_u32((source + i * 4 + 3) & ~3, (1 << (lane + 1)) - 1);
                partial <<= (3 - lane) * 8;
                lane = (source + i * 4) & 3;
                second = r_masked_u32((source + i * 4) & ~3, (15 << lane) & 15);
                words[i] = lane == 0 ? second : (partial & (0xFFFFFFFF << (32 - lane * 8))) | (second >> (lane * 8));
            }
            for (i = 0; i < count; ++i)
            {
                lane = (destination + i * 4 + 3) & 3;
                w_masked_u32((destination + i * 4 + 3) & ~3, words[i] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
                lane = (destination + i * 4) & 3;
                w_masked_u32((destination + i * 4) & ~3, words[i] << (lane * 8), (15 << lane) & 15);
            }
        }
        value = r_u32(game + offset + 0x1B4);
        w_u32(record + 0x40, value);
        value = r_u32(game + 0x120);
        w_u8(output + 0x464, (value & 4) == 0);
    }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 8 : 2;
        source = data + (group == 0 ? 24 : 88);
        destination = output + (group == 0 ? 26 : 24);
        for (row = 0; row < count; ++row)
        {
            w_u8(destination + row, 0);
            for (column = 0; column < 8; ++column)
            {
                value = r_u8(source + row * 8 + column);
                previous = r_u8(destination + row);
                w_u8(destination + row, previous | ((value & 1) << column));
            }
        }
    }
    for (i = 0; i < 60; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = r_u16(data + half_source[j] + i * 2);
            w_u16(output + half_destination[j] + i * 2, value);
        }
    for (i = 0; i < 30; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = r_u8(data + nibble_source[j] + i * 2);
            second = r_u8(data + nibble_source[j] + i * 2 + 1);
            w_u8(output + nibble_destination[j] + i, (value & 15) | ((second & 15) << 4));
        }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 20 : 45;
        source = data + (group == 0 ? 0x3EC : 0x43C);
        destination = output + (group == 0 ? 0x27A : 0x28E);
        for (row = 0; row < count; ++row)
        {
            w_u8(destination + row, 0);
            for (column = 0; column < 4; ++column)
            {
                value = r_u8(source + row * 4 + column);
                previous = r_u8(destination + row);
                w_u8(destination + row, previous | ((value & 3) << (column * 2)));
            }
        }
    }
    display = r_u32(0x800BCE48);
    value = r_u16(display + 8);
    w_u16(output + 0x460, value);
    value = r_u16(display + 12);
    game = r_u32(0x800BCE60);
    w_u16(output + 0x462, value);
    value = r_u8(game + 920);
    w_u8(output + 0x45D, value);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 921);
    w_u8(output + 0x45E, value);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 922);
    w_u8(output + 0x45C, value);
    value = r_u16(data + 0x6D4);
    game = r_u32(0x800BCE60);
    w_u16(output + 0x2C0, value);
    value = r_u16(data + 0x6D8);
    w_u16(output + 0x2C2, value);
    value = r_u8(data + 1);
    w_u32(output + 0x468, 0);
    w_u16(output + 0x14, value);
    value = r_u8(game + 924);
    if (value & 1)
        w_u32(output + 0x468, 1);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 288);
    if (value & 1)
    {
        value = r_u32(output + 0x468);
        w_u32(output + 0x468, value | 2);
    }
    game = r_u32(0x800BCE60);
    value = r_u32(game + 288);
    if (value & 2)
    {
        value = r_u32(output + 0x468);
        w_u32(output + 0x468, value | 4);
    }
    game = r_u32(0x800BCE60);
    value = r_u8(game + 2394);
    result = 1;
    if (value == 1)
    {
        result = r_u32(output + 0x468) | 8;
        w_u32(output + 0x468, result);
    }
    for (i = 17; i > 0; --i)
        w_u8(output + 0x46C + i - 1, 0);
    return result;
}

uint32 resource001_sub_800d1a5c(void)
{
    uint32 state, step;
    FUNCTION_MARKER(0x800D1A5C, "ACE3.BIN");
    resource001_sub_800da838(2, 1, 0, 60);
    resource001_sub_800da838(4, 1, 0, 60);
    resource001_sub_800da838(9, 1, 0, 60);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da9a8(1, 7);
    return resource001_sub_800da9a8(4, 5);
}

uint32 resource001_sub_800d1adc(void)
{
    uint32 first, second, state;
    FUNCTION_MARKER(0x800D1ADC, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if (first == 0)
        return second;
    if (second == 0)
        return 0x800E0000;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 1);
    w_u16(state + 78, 0);
    return 1;
}

uint32 resource001_sub_800d1b2c(void)
{
    uint32 first, second, state, step;
    FUNCTION_MARKER(0x800D1B2C, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        resource001_sub_800da77c(8);
        resource001_sub_800da828(318);
        resource001_sub_800da838(4, 1, 0, 60);
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 2);
        resource001_sub_800da9a8(6, 2);
        resource001_sub_800da9a8(7, 2);
        resource001_sub_800da49c(3);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
        return resource001_sub_800da9a8(4, 5);
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 2);
    w_u16(state + 78, 0);
    return 2;
}

uint32 resource001_sub_800d1c18(void)
{
    uint32 first, second, state, game, flags;
    FUNCTION_MARKER(0x800D1C18, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 300) & 0x20;
        if (flags == 0)
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 2);
        w_u16(state + 14, 0);
        resource001_sub_800da828(158);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 0);
        return state;
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    w_u8(state + 208, 0);
    resource001_sub_800da9a8(1, 7);
    return resource001_sub_800da9a8(4, 5);
}

uint32 resource001_sub_800d1cf0(void)
{
    uint32 first, second, state, step;
    FUNCTION_MARKER(0x800D1CF0, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(8);
    resource001_sub_800da828(318);
    resource001_sub_800da838(4, 1, 0, 60);
    resource001_sub_800da9a8(1, 3);
    resource001_sub_800da9a8(4, 2);
    resource001_sub_800da49c(4);
    state = r_u32(0x800DD9D0);
    w_u8(state + 209, 0);
    if (first == 5)
    {
        if (second == 5)
            return 5;
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 0);
        return state;
    }
    if (second == 5)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 1);
    }
    return 1;
}

uint32 resource001_sub_800d1dcc(void)
{
    uint32 first, second, state, game, flags, old_selection, selection;
    FUNCTION_MARKER(0x800D1DCC, "ACE3.BIN");
    first = resource001_sub_800d3d90(0);
    second = resource001_sub_800d3d90(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 1);
        w_u16(state + 78, 0);
        resource001_sub_800da838(2, 1, 0, 20);
        resource001_sub_800da838(1, 3, -1, -1);
        state = r_u32(0x800DD9D0);
        w_u8(state + 209, 1);
        return state;
    }
    game = r_u32(0x800BCE60);
    state = r_u32(0x800DD9D0);
    flags = r_u32(game + 304);
    old_selection = r_u8(state + 90);
    if (flags & 3)
        w_u8(state + 90, old_selection ^ 1);
    if (first == 0 || first == 2 || first == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 1);
    }
    if (second == 0 || second == 2 || second == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 90, 0);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 90);
    if (selection != old_selection)
        sub_8002db4c(61);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 90);
    if (selection == 0)
    {
        resource001_sub_800da9a8(6, 0);
        resource001_sub_800da9a8(7, 1);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 90);
    if (selection == 1)
    {
        resource001_sub_800da9a8(6, 1);
        resource001_sub_800da9a8(7, 0);
    }
    if (first == 0 || first == 2 || first == 6)
        resource001_sub_800da9a8(6, 2);
    if (second == 0 || second == 2 || second == 6)
        resource001_sub_800da9a8(7, 2);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 3);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        return resource001_sub_800da9a8(4, 5);
    }
    if (flags & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 2);
        w_u16(state + 14, 0);
        resource001_sub_800da828(190);
        return sub_8002db4c(59);
    }
    return 0x800E0000;
}

uint32 resource001_sub_800d202c(void)
{
    uint32 state, port, status, step;
    FUNCTION_MARKER(0x800D202C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    resource001_sub_800da9a8(1, 7);
    if (status == 1)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 4);
        resource001_sub_800da49c(6);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
        return state;
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 4);
    return 4;
}

uint32 resource001_sub_800d20bc(void)
{
    uint32 state, port, status, game, flags;
    FUNCTION_MARKER(0x800D20BC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
    }
    else
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 300) & 0x20;
        if (flags == 0)
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
    }
    w_u8(state + 208, 0);
    return state;
}

uint32 resource001_sub_800d21d0(void)
{
    uint32 state, port, status, game, flags;
    FUNCTION_MARKER(0x800D21D0, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
    }
    else
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 300) & 0x20;
        if (flags == 0)
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
    }
    w_u8(state + 208, 0);
    return state;
}

uint32 resource001_sub_800d22e4(void)
{
    uint32 state, port, status, game, flags;
    FUNCTION_MARKER(0x800D22E4, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
    }
    else
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 300) & 0x20;
        if (flags == 0)
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 2);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
    }
    w_u8(state + 208, 0);
    return state;
}

uint32 resource001_sub_800d2154(void)
{
    uint32 state, port, status, step;
    FUNCTION_MARKER(0x800D2154, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 3)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 5);
        resource001_sub_800da49c(7);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
        return 1;
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 5);
    return 5;
}

uint32 resource001_sub_800d2268(void)
{
    uint32 state, port, status, step;
    FUNCTION_MARKER(0x800D2268, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 4)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 6);
        resource001_sub_800da49c(8);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
        return 1;
    }
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 6);
    return 6;
}

uint32 resource001_sub_800d237c(void)
{
    uint32 state, step, result;
    FUNCTION_MARKER(0x800D237C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da9a8(1, 7);
    resource001_sub_800d51fc();
    result = resource001_sub_800d5290();
    resource001_sub_800d5548();
    return result;
}

uint32 resource001_sub_800d23cc(void)
{
    uint32 state, port, status, step;
    FUNCTION_MARKER(0x800D23CC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d6604(port, 1);
    if (status == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 2);
        resource001_sub_800da9a8(1, 14);
        resource001_sub_800da49c(6);
    }
    if (status == 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 2);
        resource001_sub_800da9a8(1, 8);
        resource001_sub_800da49c(6);
    }
    if (status == 1)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78) + 1;
        w_u16(state + 78, step);
        return step;
    }
    return 1;
}

uint32 resource001_sub_800d248c(void)
{
    uint32 state, port, status, step;
    FUNCTION_MARKER(0x800D248C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d6814(port, 1);
    if (status == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 14);
        resource001_sub_800da49c(6);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
    }
    if (status == 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 8);
        resource001_sub_800da49c(6);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
    }
    if (status == 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 7);
        w_u16(state + 78, 0);
    }
    return 7;
}

uint32 resource001_sub_800d255c(void)
{
    uint32 state, port, status, game, flags;
    FUNCTION_MARKER(0x800D255C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    state = r_u32(0x800DD9D0);
    w_u8(state + 208, 1);
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
    }
    else
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 300) & 0x20;
        if (flags == 0)
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 2);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
    }
    w_u8(state + 208, 0);
    return state;
}

uint32 resource001_sub_800d2600(void)
{
    uint32 state, step;
    FUNCTION_MARKER(0x800D2600, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 72, 0);
    w_u8(state + 211, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da828(190);
    resource001_sub_800da838(1, 1, 0, 20);
    resource001_sub_800da838(2, 3, -1, -1);
    resource001_sub_800da838(4, 3, -1, -1);
    resource001_sub_800da838(9, 3, -1, -1);
    resource001_sub_800da838(1, 1, 0, 60);
    resource001_sub_800da838(5, 1, 0, 60);
    resource001_sub_800da838(10, 1, 0, 60);
    resource001_sub_800dc028(1);
    resource001_sub_800da9a8(1, 9);
    resource001_sub_800da9a8(4, 3);
    return resource001_sub_800da49c(9);
}

uint32 resource001_sub_800d26f0(void)
{
    uint32 state, port, status, occupied, game, flags, selection;
    FUNCTION_MARKER(0x800D26F0, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    occupied = resource001_sub_800d5c80();
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
        return resource001_sub_800dc028(0);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if (flags & 2)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u8(state + 93, selection < 2 ? selection + 1 : 0);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if (flags & 1)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        w_u8(state + 93, selection == 0 ? 2 : selection - 1);
        sub_8002db4c(61);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(6, selection != 0);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(7, selection != 1);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800da9a8(8, selection != 2);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x10)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        if ((occupied >> (selection & 31)) & 1)
        {
            w_u16(state + 76, 8);
            w_u16(state + 78, 0);
            sub_8002db4c(89);
            return resource001_sub_800da9a8(4, 5);
        }
        return sub_8002db4c(48);
    }
    if ((flags & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 2);
    w_u16(state + 78, 0);
    resource001_sub_800da838(2, 1, 0, 20);
    resource001_sub_800da838(1, 3, -1, -1);
    resource001_sub_800dc028(0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d2954(void)
{
    uint32 state, step;
    FUNCTION_MARKER(0x800D2954, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da9a8(1, 12);
    return resource001_sub_800da49c(12);
}

uint32 resource001_sub_800d2994(void)
{
    uint32 state, selection, game, flags, step;
    FUNCTION_MARKER(0x800D2994, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 93);
    resource001_sub_800d47b8(selection);
    state = r_u32(0x800DD9D0);
    resource001_sub_800d4808(state + 95);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 0x964);
    state = r_u32(0x800DD9D0);
    w_u32(game + 0x964, flags & ~1);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}

uint32 resource001_sub_800d29fc(void)
{
    uint32 state, port, slot, status, step;
    FUNCTION_MARKER(0x800D29FC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    slot = r_u8(state + 93);
    status = resource001_sub_800d6ab4(port, slot, 1);
    if (status == 0xFFFFFFFF)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 14);
        resource001_sub_800da49c(13);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
        resource001_sub_800dc028(0);
    }
    if (status == 2)
    {
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78);
        w_u16(state + 78, step + 1);
        resource001_sub_800da9a8(1, 8);
        resource001_sub_800da49c(6);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 1);
    }
    if (status == 1)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        resource001_sub_800da49c(10);
        return resource001_sub_800dc028(0);
    }
    return 9;
}

uint32 resource001_sub_800d2ae8(void)
{
    uint32 state, port, status, game, flags;
    FUNCTION_MARKER(0x800D2AE8, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    state = r_u32(0x800DD9D0);
    w_u8(state + 208, 1);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 2);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        state = r_u32(0x800DD9D0);
        w_u8(state + 208, 0);
        resource001_sub_800dc028(0);
    }
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 2);
        w_u16(state + 14, 0);
        w_u8(state + 208, 0);
        return resource001_sub_800dc028(0);
    }
    return 6;
}

uint32 resource001_sub_800d2e74(void)
{
    uint32 state, step, i, selection, game, flags, available, index;
    FUNCTION_MARKER(0x800D2E74, "ACE3.BIN");
    resource001_sub_800da828(542);
    resource001_sub_800da77c(9);
    resource001_sub_800da838(1, 1, 0, 20);
    resource001_sub_800da838(15, 1, 0, 20);
    resource001_sub_800da9a8(1, 10);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u8(state + 211, 1);
    w_u16(state + 78, step + 1);
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        resource001_sub_800da9a8(i + 6, selection == i);
    }
    available = resource001_sub_800d2ba0();
    if (available)
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 0x96C);
        state = r_u32(0x800DD9D0);
        w_u32(game + 0x96C, flags | 0x10);
        selection = r_u8(state + 95);
        index = selection == 6 ? 13 : 12;
    }
    else
    {
        game = r_u32(0x800BCE60);
        flags = r_u32(game + 0x96C);
        w_u32(game + 0x96C, flags & ~0x10);
        index = 13;
    }
    return resource001_sub_800da9a8(index, 2);
}

uint32 resource001_sub_800d2fb8(void)
{
    uint32 mode, occupied, state, game, flags, selection, port, status, i, index, valid;
    FUNCTION_MARKER(0x800D2FB8, "ACE3.BIN");
    mode = resource001_sub_800da898(1);
    occupied = resource001_sub_800d5ca0();
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
        w_u8(state + 211, 0);
        return resource001_sub_800da828(318);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if ((flags & 8) && mode == 2)
    {
        flags = r_u32(game + 0x96C);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        if (flags & 0x10)
        {
            if (selection >= 8 || selection == 6)
                w_u8(state + 95, 0);
            else if (selection == 5)
                w_u8(state + 95, 7);
            else if (selection == 7)
                w_u8(state + 95, 6);
            else
            {
                state = r_u32(0x800DD9D0);
                w_u8(state + 95, selection + 1);
            }
        }
        else
            w_u8(state + 95, selection < 6 ? selection + 1 : 0);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if ((flags & 4) && mode == 2)
    {
        flags = r_u32(game + 0x96C);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        if (flags & 0x10)
        {
            if (selection >= 8)
                w_u8(state + 95, 0);
            else if (selection == 0 || selection == 6)
                w_u8(state + 95, 7);
            else if (selection == 7)
                w_u8(state + 95, 5);
            else
            {
                state = r_u32(0x800DD9D0);
                w_u8(state + 95, selection - 1);
            }
        }
        else
            w_u8(state + 95, selection == 0 ? 6 : selection - 1);
        sub_8002db4c(61);
    }
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        resource001_sub_800da9a8(i + 6, selection == i);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 0x96C);
    index = 13;
    if (flags & 0x10)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        if (selection != 6)
            index = 12;
    }
    resource001_sub_800da9a8(index, 2);
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 93);
    selection = r_u8(state + 95);
    resource001_sub_800d2c84(port, selection);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if ((flags & 0x30) == 0x10 || mode == 0)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 95);
        valid = selection - 6 < 2;
        if (!valid)
        {
            selection = r_u8(state + 95);
            valid = (occupied >> (selection & 31)) & 1;
        }
        if (valid)
        {
            if (mode != 0 && mode != 3)
            {
                resource001_sub_800da838(1, 3, -1, -1);
                state = r_u32(0x800DD9D0);
                w_u8(state + 96, 1);
                sub_8002db4c(89);
            }
            else
            {
                state = r_u32(0x800DD9D0);
                status = r_u8(state + 96);
                if (status == 1)
                {
                    selection = r_u8(state + 95);
                    if (selection < 6)
                    {
                        resource001_sub_800da838(1, 0, 0, 0);
                        state = r_u32(0x800DD9D0);
                        selection = r_u8(state + 95);
                        resource001_sub_800d4df4(selection);
                        state = r_u32(0x800DD9D0);
                        w_u16(state + 76, 11);
                        game = r_u32(0x800BCE60);
                        w_u16(state + 78, 0);
                        w_u16(game + 0x954, 2);
                        flags = r_u32(game + 0x96C);
                        w_u32(game + 0x96C, flags & ~0x10);
                    }
                    state = r_u32(0x800DD9D0);
                    selection = r_u8(state + 95);
                    if (selection == 6)
                    {
                        game = r_u32(0x800BCE60);
                        w_u16(state + 76, 10);
                        w_u16(state + 78, 0);
                        flags = r_u32(game + 0x96C);
                        w_u32(game + 0x96C, flags & ~0x10);
                    }
                    state = r_u32(0x800DD9D0);
                    selection = r_u8(state + 95);
                    if (selection == 7)
                    {
                        w_u16(state + 12, 6);
                        w_u16(state + 14, 0);
                    }
                    state = r_u32(0x800DD9D0);
                    game = r_u32(0x800BCE60);
                    selection = r_u8(state + 95);
                    w_u8(game + 0x95D, selection);
                }
            }
        }
        else if (flags & 0x10)
            sub_8002db4c(48);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300) & 0x20;
    if (flags || mode == 0)
    {
        if (mode != 0 && mode != 3)
        {
            resource001_sub_800da838(1, 3, -1, -1);
            state = r_u32(0x800DD9D0);
            w_u8(state + 96, 2);
            return sub_8002db4c(59);
        }
        state = r_u32(0x800DD9D0);
        status = r_u8(state + 96);
        if (status != 2)
            return 2;
        resource001_sub_800da838(1, 0, 0, 0);
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 7);
        w_u16(state + 78, 0);
        return resource001_sub_800dbf6c(64, 30, 0, 0);
    }
    return flags;
}

uint32 resource001_sub_800d34fc(void)
{
    uint32 state, step;
    FUNCTION_MARKER(0x800D34FC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da828(1086);
    resource001_sub_800da838(15, 1, 0, 60);
    resource001_sub_800da9a8(1, 11);
    resource001_sub_800da9a8(4, 6);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    resource001_sub_800da49c(11);
    return resource001_sub_800da9a8(4, 6);
}

uint32 resource001_sub_800d37fc(void)
{
    uint32 state, step;
    FUNCTION_MARKER(0x800D37FC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 212, 0);
    w_u16(state + 78, step + 1);
    return resource001_sub_800dbf6c(111, 150, 0, 0);
}

uint32 resource001_sub_800d3a24(void)
{
    uint32 state, result;
    FUNCTION_MARKER(0x800D3A24, "ACE3.BIN");
    resource001_sub_800da828(0x400036);
    resource001_sub_800da9a8(4, 4);
    state = r_u32(0x800DD9D0);
    w_u8(state + 211, 0);
    result = sub_8002b528();
    if (result == 3)
    {
        state = r_u32(0x800DD9D0);
        result = r_u16(state + 78) + 1;
        w_u16(state + 78, result);
    }
    return result;
}

uint32 resource001_sub_800d3a8c(void)
{
    uint32 state, result;
    FUNCTION_MARKER(0x800D3A8C, "ACE3.BIN");
    result = sub_8002b528();
    if (result == 1)
    {
        resource001_sub_800da9a8(1, 73);
        resource001_sub_800da49c(38);
        state = r_u32(0x800DD9D0);
        result = r_u16(state + 78);
        w_u32(state + 212, 31);
        result -= 2;
        w_u16(state + 78, result);
    }
    return result;
}

uint32 resource001_sub_800d3840(void)
{
    uint32 status, game, state, kind, value, step;
    sint32 index;
    FUNCTION_MARKER(0x800D3840, "ACE3.BIN");
    status = (uint32)cd_resource_state();
    if (status == 0)
        return resource001_sub_800da9a8(1, 73);
    game = r_u32(0x800BCE60);
    index = r_s16(game + 908);
    kind = r_u8(0x800DD8D4 + (uint32)index);
    if (kind == 3 || status == 3 || (status == 1 && kind == 1))
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 6);
        w_u16(state + 14, 0);
        return 6;
    }
    if (status == 4 || status == 0xFFFFFFFF)
    {
        if (kind == 1)
        {
            state = r_u32(0x800DD9D0);
            value = r_u32(state + 212);
            resource001_sub_800da9a8(1, value + 70);
        }
        game = r_u32(0x800BCE60);
        index = r_s16(game + 908);
        kind = r_u8(0x800DD8D4 + (uint32)index);
        if (kind == 2)
        {
            state = r_u32(0x800DD9D0);
            value = r_u32(state + 212);
            resource001_sub_800da9a8(1, value + 71);
        }
        sub_8002b428();
        resource001_sub_800da49c(37);
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78) + 1;
        w_u16(state + 78, step);
        return step;
    }
    if (status == 1)
    {
        state = r_u32(0x800DD9D0);
        value = r_u32(state + 212);
        resource001_sub_800da9a8(1, value + 71);
        sub_8002b428();
        resource001_sub_800da49c(37);
        state = r_u32(0x800DD9D0);
        step = r_u16(state + 78) + 1;
        w_u16(state + 78, step);
        return 0x800C0000;
    }
    if (status != 2)
        return 0x800C0000;
    game = r_u32(0x800BCE60);
    index = r_s16(game + 908);
    kind = r_u8(0x800DD8D4 + (uint32)index);
    if (kind == 2)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 6);
        w_u16(state + 14, 0);
        return 6;
    }
    state = r_u32(0x800DD9D0);
    value = r_u32(state + 212);
    resource001_sub_800da9a8(1, value + 70);
    sub_8002b428();
    resource001_sub_800da49c(37);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78) + 1;
    w_u16(state + 78, step);
    return step;
}
