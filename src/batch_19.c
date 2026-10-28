#include "game_functions.h"
#include <stdlib.h>

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 resource001_sub_800d5550(void)
{
    uint32 state, other, base, words[3], tail, index, source, destination, previous;

    FUNCTION_MARKER(0x800D5550, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    w_u16(state + 856, 0);
    w_u16(state + 858, 0);
    w_u16(state + 860, 0);
    resource001_sub_800d51fc();
    resource001_sub_800d5290();
    resource001_sub_800d5548();
    resource001_sub_800d3ce8();
    state = r_u32(0x800DDA48);
    w_u8(state + 822, 0);
    other = r_u32(0x800DDA48);
    w_u32(state + 800, 0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    w_u16(state + 80, 0);
    w_u16(state + 82, 0);
    w_u8(other + 820, 0);
    base = r_u32(0x800DDA48);
    state = base;
    words[0] = r_u32(0x800CC4C4);
    words[1] = r_u32(0x800CC4C8);
    words[2] = r_u32(0x800CC4CC);
    w_u32(state + 824, words[0]);
    w_u32(state + 828, words[1]);
    w_u32(state + 832, words[2]);
    tail = r_u16(0x800CC4D0);
    w_u16(state + 836, tail);
    state = r_u32(0x800DDA48);
    words[0] = r_u32(0x800CC4D4);
    words[1] = r_u32(0x800CC4D8);
    words[2] = r_u32(0x800CC4DC);
    w_u32(state + 692, words[0]);
    w_u32(state + 696, words[1]);
    w_u32(state + 700, words[2]);
    words[0] = r_u32(0x800CC4E0);
    words[1] = r_u32(0x800CC4E4);
    tail = r_u16(0x800CC4E8);
    w_u32(state + 704, words[0]);
    w_u32(state + 708, words[1]);
    w_u16(state + 712, tail);
    tail = r_u8(0x800CC4EA);
    w_u8(state + 714, tail);
    state = r_u32(0x800DDA48);
    for (index = 0; index < 3; ++index)
    {
        source = 0x800CC4EC + index * 4;
        words[index] = r_u32(source);
        words[index] = r_u32(source);
    }
    for (index = 0; index < 3; ++index)
    {
        destination = state + 718 + index * 4;
        previous = r_u32(destination + 2);
        w_u32(destination + 2, (previous & 0xFFFF0000) | (words[index] >> 16));
        previous = r_u32(destination - 2);
        w_u32(destination - 2, (previous & 0xFFFF) | (words[index] << 16));
    }
    for (index = 0; index < 2; ++index)
    {
        source = 0x800CC4F8 + index * 4;
        words[index] = r_u32(source);
        words[index] = r_u32(source);
    }
    tail = r_u16(0x800CC500);
    for (index = 0; index < 2; ++index)
    {
        destination = state + 730 + index * 4;
        previous = r_u32(destination + 2);
        w_u32(destination + 2, (previous & 0xFFFF0000) | (words[index] >> 16));
        previous = r_u32(destination - 2);
        w_u32(destination - 2, (previous & 0xFFFF) | (words[index] << 16));
    }
    w_u16(state + 738, tail);
    tail = r_u8(0x800CC502);
    w_u8(state + 740, tail);
    for (index = 0; index < 2; ++index)
    {
        w_u16(base + index * 2 + 44, 2);
        w_u16(base + index * 2 + 48, 2);
    }
    return 0;
}

uint32 resource001_sub_800cc5d8(void)
{
    uint32 game, flags, first, second, state, other, buffer, index;

    FUNCTION_MARKER(0x800CC5D8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    w_u8(game + 0x95E, 0);
    game = r_u32(0x800BCE60);
    w_u8(game + 0x95F, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 0x96C);
    first = r_u16(game + 0x390);
    second = r_u16(game + 0x392);
    w_u32(game + 0x96C, flags & 0xFFFFFFF7);
    w_u16(game + 0x38C, first);
    w_u16(game + 0x38E, second);
    state = arena_alloc(228);
    w_u32(0x800DD9D0, state);
    for (index = 0; index < 2; ++index)
    {
        buffer = arena_alloc(0x2000);
        state = r_u32(0x800DD9D0);
        w_u32(state + index * 4, buffer);
    }
    resource001_sub_800da25c();
    state = r_u32(0x800DD9D0);
    w_u8(state + 88, 0);
    other = r_u32(0x800DD9D0);
    w_u16(state + 12, 1);
    w_u16(state + 14, 0);
    w_u32(state + 68, 0);
    w_u16(state + 18, 0);
    w_u32(state + 64, 0);
    w_u32(state + 120, 0);
    w_u8(other + 89, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 90, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 93, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 95, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 96, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 94, 0);
    state = r_u32(0x800DD9D0);
    w_u32(state + 100, 0x800DCB30);
    w_u8(state + 92, 0);
    other = r_u32(0x800DD9D0);
    w_u32(state + 104, 0x800DCCC8);
    w_u8(other + 91, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 208, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 209, 1);
    game = r_u32(0x800BCE60);
    w_u8(game + 0x971, 0);
    resource001_sub_800d4f0c();
    resource001_sub_800d5550();
    state = r_u32(0x800DD9D0);
    w_u32(state + 24, 0);
    resource001_sub_800d80c0();
    resource001_sub_800cd1a0();
    return 0x04000000;
}

uint32 resource001_sub_800da6fc(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA6FC, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    return r_u32(state + 16);
}

uint32 resource001_sub_800da9a8(uint32 index, uint32 value)
{
    uint32 state, output;

    FUNCTION_MARKER(0x800DA9A8, "ACE3.BIN");
    if (index >= 64)
        return 0x800E0000;
    state = r_u32(0x800DD9DC);
    output = r_u32(state + 76) + index;
    w_u8(output, value);
    return output;
}

uint32 resource001_sub_800cc938(uint32 output)
{
    uint32 counts[6] = {0};
    uint32 game, index, best, value, rank, count, cursor = 0;

    FUNCTION_MARKER(0x800CC938, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    for (index = 1; index < 56; ++index)
    {
        if (index == 19 || index == 49 || index == 54)
            continue;
        value = r_u8(game + index + 0xC1C);
        best = value ? value : 5;
        value = r_u8(game + index + 0xC58);
        if (value && value < best)
            best = value;
        value = r_u8(game + index + 0xC94);
        if (value && value < best)
            best = value;
        if (best >= 6)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 rank %u exceeds original local count array\n", best);
#endif
            abort();
        }
        ++counts[best];
    }
    for (rank = 1; rank <= 5; ++rank)
    {
        count = counts[rank];
        for (index = 0; index < count; ++index)
            w_u8(output + cursor++, rank);
    }
    while (cursor < 52)
        w_u8(output + cursor++, 5);
    return output;
}

uint32 resource001_sub_800ccc90(void)
{
    uint32 state, root, other, flags, value, index;

    FUNCTION_MARKER(0x800CCC90, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    w_u32(state + 72, 0);
    w_u16(state + 16, 0);
    w_u16(state + 14, 1);
    resource001_sub_800da828(1);
    root = r_u32(0x800BCE48);
    flags = r_u32(root);
    w_u8(root + 16, 0xE5);
    other = r_u32(0x800BCE48);
    w_u32(root, flags | 8);
    w_u8(other + 17, 0xE4);
    root = r_u32(0x800BCE48);
    w_u8(root + 18, 0xDE);
    state = r_u32(0x800DD9D0);
    resource001_sub_800cc938(state + 148);
    for (index = 0; index < 52; ++index)
    {
        state = r_u32(0x800DD9D0);
        value = r_u8(state + index + 148);
        resource001_sub_800da9a8(index + 1, value);
    }
    return 0;
}

void resource001_sub_800d58b0(void)
{
    uint32 state, handle, index;

    FUNCTION_MARKER(0x800D58B0, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 4 + index * 4);
        CloseEventPSX(handle);
    }
    bios_exit_critical();
}

sint32 resource001_sub_800d3f88(void)
{
    uint32 state, handle, index;
    sint32 result;

    FUNCTION_MARKER(0x800D3F88, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 4 + index * 4);
        result = TestEvent((sint32)handle);
    }
    return result;
}

sint32 resource001_sub_800d3ff4(void)
{
    uint32 state, handle, index;
    sint32 result;

    FUNCTION_MARKER(0x800D3FF4, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 20 + index * 4);
        result = TestEvent((sint32)handle);
    }
    return result;
}

sint32 resource001_sub_800d3ea8(uint32 port, uint32 next_step)
{
    uint32 state, handle, index;

    FUNCTION_MARKER(0x800D3EA8, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 4 + index * 4);
        if (TestEvent((sint32)handle) == 1)
        {
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, next_step);
            return index;
        }
    }
    return -1;
}

sint32 resource001_sub_800d4060(uint32 port, uint32 next_step)
{
    uint32 state, handle, index;

    FUNCTION_MARKER(0x800D4060, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 20 + index * 4);
        if (TestEvent((sint32)handle) == 1)
        {
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, next_step);
            return index;
        }
    }
    return -1;
}

uint32 resource001_sub_800d5f6c(void)
{
    uint32 state, port, step, target, field, value, result, flags, status_base;
    sint32 status;

    FUNCTION_MARKER(0x800D5F6C, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    port = r_u32(state + 852);
    step = r_u8(state + port + 844);
    if (step >= 9)
        return 0x800D0000;
    target = r_u32(0x800CC538 + step * 4);
    switch (target)
    {
        case 0x800D5FC4:
            state = r_u32(0x800DDA48);
            goto toggle_port;
        case 0x800D5FD4:
            resource001_sub_800d3f88();
            _card_info((sint32)(port << 4));
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, 2);
            state = r_u32(0x800DDA48);
            w_u32(state + 792, 300);
            return 300;
        case 0x800D6008:
            result = resource001_sub_800d3ea8(port, 3);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 846, result);
            state = r_u32(0x800DDA48);
            value = r_u32(state + 792);
            if ((sint32)value < 0)
            {
                w_u8(state + port + 844, 1);
                return 1;
            }
            result = value - 1;
            w_u32(state + 792, result);
            return result;
        case 0x800D6058:
            state = r_u32(0x800DDA48);
            field = state + port;
            value = r_u8(field + 846);
            status_base = state + port * 2;
            w_u16(status_base + 44, value);
            if (value == 0)
            {
                if (r_u8(field + 840) == 1)
                {
                    flags = r_u32(state + 40);
                    w_u8(field + 844, flags != 0);
                    state = r_u32(0x800DDA48);
                    goto toggle_port;
                }
                resource001_sub_800d3ff4();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                state = r_u32(0x800DDA48);
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
                    field = r_u32(0x800DDA48) + port;
                    flags = r_u8(field + 848);
                    w_u8(field + 848, flags + 1);
                }
                state = r_u32(0x800DDA48);
                flags = r_u32(state + 40);
                w_u8(state + port + 844, value == 1 ? flags == 1 : flags != 0);
                state = r_u32(0x800DDA48);
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
                resource001_sub_800d3ff4();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                state = r_u32(0x800DDA48);
                w_u8(state + port + 844, 4);
                state = r_u32(0x800DDA48);
                w_u8(state + port + 840, 0);
                state = r_u32(0x800DDA48);
                result = state + port;
                w_u8(result + 848, 0);
                return result;
            }
            state = r_u32(0x800DDA48);
            w_u8(state + port + 840, 1);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, 1);
            state = r_u32(0x800DDA48);
            goto toggle_port;
        case 0x800D6284:
            do
            {
                result = resource001_sub_800d4060(port, 5);
                state = r_u32(0x800DDA48);
                w_u8(state + port + 846, result);
                state = r_u32(0x800DDA48);
                result = r_u8(state + port + 846);
            } while (result == 255);
            return result;
        case 0x800D62CC:
            state = r_u32(0x800DDA48);
            value = r_u8(state + port + 846);
            w_u16(state + port * 2 + 44, value);
            resource001_sub_800d3f88();
            psx_bios_card_load_guest(port << 4);
            state = r_u32(0x800DDA48);
            result = state + port;
            w_u8(result + 844, 6);
            return result;
        case 0x800D6300:
            result = resource001_sub_800d3ea8(port, 7);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 846, result);
            return result;
        case 0x800D6324:
            state = r_u32(0x800DDA48);
            field = state + port;
            value = r_u8(field + 846);
            w_u16(state + port * 2 + 48, value);
            w_u8(field + 840, 1);
            state = r_u32(0x800DDA48);
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
                    field = r_u32(0x800DDA48) + port;
                    value = r_u8(field + 848);
                    w_u8(field + 848, value + 1);
                }
                state = r_u32(0x800DDA48);
                result = state + port;
                w_u8(result + 844, 1);
                return result;
            }
            if (status != 3)
                return state + port;
            w_u8(state + port + 840, 1);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, 1);
            state = r_u32(0x800DDA48);
            goto toggle_port;
        case 0x800D6418:
            state = r_u32(0x800DDA48);
            result = game_bios_open_guest(state + port * 26 + 692, 0x8001, 26);
            state = r_u32(0x800DDA48);
            w_u32(state + 60, result);
            if (result != 0xFFFFFFFF)
                w_u8(state + port + 842, 1);
            state = r_u32(0x800DDA48);
            value = r_u32(state + 60);
            game_bios_close_guest(value);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 844, 1);
            state = r_u32(0x800DDA48);
            w_u8(state + port + 840, 1);
            state = r_u32(0x800DDA48);
            goto toggle_port;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound card-state branch %08X\n", target);
#endif
            abort();
    }
toggle_port:
    flags = r_u32(state + 852);
    result = 1 - flags;
    w_u32(state + 852, result);
    return result;
}

uint32 resource001_sub_800ccb38(uint32 ordering, uint32 packet)
{
    uint32 game, tick, amplitude, red, green, blue, tag, link;
    sint32 high, quotient, scaled;

    FUNCTION_MARKER(0x800CCB38, "ACE3.BIN");
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    w_u16(packet + 8, 110);
    w_u16(packet + 10, 229);
    w_u16(packet + 12, 8);
    game = r_u32(0x800BCE60);
    w_u16(packet + 14, 16);
    tick = r_u32(game + 32);
    high = (sint32)(((sint64)(sint32)tick * (sint32)0x88888889) >> 32);
    quotient = (sint32)((uint32)high + tick) >> 6;
    quotient = (sint32)((uint32)quotient - (uint32)((sint32)tick >> 31));
    amplitude = tick - (uint32)quotient * 120 - 60;
    if ((sint32)amplitude < 0)
        amplitude = 0 - amplitude;
    red = 0 - amplitude * 29;
    green = 0 - amplitude * 132;
    blue = 0 - amplitude * 198;
    high = (sint32)(((sint64)(sint32)red * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + red) >> 5;
    red = (uint32)scaled - (uint32)((sint32)red >> 31) + 229;
    w_u8(packet + 4, red);
    high = (sint32)(((sint64)(sint32)green * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + green) >> 5;
    green = (uint32)scaled - (uint32)((sint32)green >> 31) + 228;
    w_u8(packet + 5, green);
    tag = r_u32(packet);
    high = (sint32)(((sint64)(sint32)blue * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + blue) >> 5;
    blue = (uint32)scaled - (uint32)((sint32)blue >> 31) + 222;
    w_u8(packet + 6, blue);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 16;
}

uint32 resource001_sub_800ccd48(void)
{
    uint32 game, state, packet, ordering, flags, root, other, index, result;

    FUNCTION_MARKER(0x800CCD48, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    state = r_u32(0x800DD9D0);
    ordering = r_u32(game + 8);
    packet = r_u32(state + 8);
    resource001_sub_800ccb38(ordering, packet);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x40)
    {
        state = r_u32(0x800DD9D0);
        root = r_u32(0x800BCE48);
#if 0
        /* Original link-mode selection */
        w_u16(state + 12, 2);
#else
        w_u16(state + 12, game_account_load() ? 6 : 3);
        /* Keep the menu XA descriptor previously initialized by link mode */
        w_u16(state + 124, 0x83D);
        w_u16(state + 126, 0);
        w_u16(state + 128, 0);
        w_u16(state + 130, 0);
        w_u32(state + 132, 0);
#endif
        w_u16(state + 14, 0);
        w_u16(state + 16, 0);
        flags = r_u32(root);
        w_u8(root + 16, 0);
        other = r_u32(0x800BCE48);
        w_u32(root, flags | 8);
        w_u8(other + 17, 112);
        root = r_u32(0x800BCE48);
        w_u8(root + 18, 120);
        sub_8002db4c(88);
        for (index = 1; index < 56; ++index)
            resource001_sub_800da9a8(index, 0);
        result = r_u32(0x800DD9D0);
        w_u32(result + 120, 0);
        return result;
    }
    state = r_u32(0x800DD9D0);
    result = r_u32(state + 120) + 1;
    w_u32(state + 120, result);
    return result;
}

uint32 resource001_sub_800d3d60(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800D3D60, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    step = r_u8(state + 844);
    w_u32(state + 40, 0);
    if (step != 0)
        return 0;
    return r_u8(state + 845) == 0;
}

uint32 resource001_sub_800d75e4(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800D75E4, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    w_u32(state + 72, 0);
    w_u16(state + 16, 0);
    w_u16(state + 14, 1);
    return state;
}

uint32 resource001_sub_800d7600(void)
{
#if 0
    /* Original memory-card completion and event teardown */
    uint32 result, state;

    FUNCTION_MARKER(0x800D7600, "ACE3.BIN");
    result = resource001_sub_800d3d60();
    if (result != 0)
    {
        resource001_sub_800d58b0();
        state = r_u32(0x800DD9D0);
        w_u32(state + 24, 0);
        w_u8(state + 88, 1);
        result = 1;
    }
    return result;
#else
    uint32 state = r_u32(0x800DD9D0);
    w_u32(state + 24, 0);
    w_u8(state + 88, 1);
    return 1;
#endif
}

void resource001_sub_800d1798(void)
{
    uint32 state, buffer;

    FUNCTION_MARKER(0x800D1798, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    w_u32(state + 72, 0);
    w_u16(state + 16, 0);
    w_u16(state + 14, 1);
    buffer = arena_alloc(4);
    w_u32(0x800DDA30, buffer);
    w_u8(buffer, 0);
    buffer = r_u32(0x800DDA30);
    w_u8(buffer + 1, 0);
    state = r_u32(0x800DD9D0);
    w_u8(state + 210, 0);
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    resource001_sub_800d51fc();
    resource001_sub_800d5290();
    resource001_sub_800d5548();
}

uint32 resource001_sub_800da49c(uint32 selection)
{
    uint32 state, value;
    sint32 index;

    FUNCTION_MARKER(0x800DA49C, "ACE3.BIN");
    state = r_u32(0x800DDA68);
    for (index = 7; index >= 0; --index)
    {
        value = r_u8(state + index);
        w_u8(state + index + 1, value);
    }
    w_u8(state, selection);
    return resource001_sub_800da484();
}

uint32 resource001_sub_800da838(uint32 index, uint32 enabled, sint32 value, sint32 duration)
{
    uint32 state, result, offset, entries;

    FUNCTION_MARKER(0x800DA838, "ACE3.BIN");
    if (index >= 16)
        return 0;
    state = r_u32(0x800DD9DC);
    entries = r_u32(state + 72);
    offset = index * 2;
    result = entries + offset;
    w_u16(result, enabled);
    if (duration >= 0)
    {
        entries = r_u32(state + 68);
        result = entries + offset;
        w_u16(result, duration);
    }
    if (value >= 0)
    {
        state = r_u32(0x800DD9DC);
        entries = r_u32(state + 64);
        result = entries + offset;
        w_u16(result, value);
    }
    return result;
}

uint32 resource001_sub_800dbf6c(uint32 x, uint32 y, uint32 first_flag, uint32 second_flag)
{
    uint32 state, flags;

    FUNCTION_MARKER(0x800DBF6C, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    w_u16(state + 80, x);
    w_u16(state + 82, y);
    flags = r_u32(state + 84);
    flags = first_flag == 1 ? flags | 1 : flags & 0xFFFFFFFE;
    w_u32(state + 84, flags);
    state = r_u32(0x800DD9DC);
    flags = r_u32(state + 84);
    flags = second_flag == 1 ? flags | 4 : flags & 0xFFFFFFFB;
    w_u32(state + 84, flags);
    return flags;
}

uint32 resource001_sub_800d3ae8(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800D3AE8, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    w_u32(state + 72, 0);
    w_u16(state + 14, 1);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    w_u8(state + 211, 0);
    resource001_sub_800dbf6c(111, 148, 0, 0);
    return resource001_sub_800da49c(3);
}

void resource001_sub_800d56e0(void)
{
    static const uint32 specifications[7] = {4, 0x8000, 0x100, 0x2000, 4, 0x8000, 0x100};
    uint32 index, handle, state, first_handle;

    FUNCTION_MARKER(0x800D56E0, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 6; ++index)
    {
        handle = OpenEventPSX(index < 4 ? 0xF4000001 : 0xF0000011, specifications[index], 0x2000, 0);
        state = r_u32(0x800DDA48);
        w_u32(state + 4 + index * 4, handle);
    }
    handle = OpenEventPSX(0xF0000011, specifications[6], 0x2000, 0);
    state = r_u32(0x800DDA48);
    first_handle = r_u32(state + 4);
    w_u32(state + 28, handle);
    EnableEventPSX(first_handle);
    for (index = 1; index < 7; ++index)
    {
        state = r_u32(0x800DDA48);
        handle = r_u32(state + 4 + index * 4);
        EnableEventPSX(handle);
    }
    bios_exit_critical();
}

uint32 resource001_sub_800d94c4(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, color, component, offset, index, tag, link;

    FUNCTION_MARKER(0x800D94C4, "ACE3.BIN");
    w_u8(packet + 3, 8);
    w_u8(packet + 7, 0x38);
    value = native_descriptor ? native_descriptor[19] : r_u8(descriptor + 19);
    w_u8(packet + 7, value ? 0x3A : 0x38);
    for (color = 0; color < 4; ++color)
        for (component = 0; component < 3; ++component)
        {
            offset = 16 + color * 4 + component;
            value = native_descriptor ? native_descriptor[offset] : r_u8(descriptor + offset);
            w_u8(packet + 4 + color * 8 + component, value);
        }
    for (index = 0; index < 8; ++index)
    {
        offset = index * 2;
        value = native_descriptor ? native_descriptor[offset] | (uint32)native_descriptor[offset + 1] << 8 : r_u16(descriptor + offset);
        if (index == 7)
            tag = r_u32(packet);
        w_u16(packet + 8 + (index / 2) * 8 + (index % 2) * 2, value);
    }
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 36;
}

uint32 resource001_sub_800cd9a8(void)
{
    uint32 state, other, root, mode, ordering, result, flags, color;
    uint8 descriptor[32];
    static const uint16 coordinates[8] = {266, 28, 311, 28, 266, 148, 311, 148};

    FUNCTION_MARKER(0x800CD9A8, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    w_u8(state + 94, 0);
    other = r_u32(0x800DD9D0);
    w_u16(state + 14, 1);
    w_u32(state + 72, 0);
    flags = r_u32(other + 24);
    if (flags == 0)
    {
        resource001_sub_800d56e0();
        resource001_sub_800d3ce8();
    }
    state = r_u32(0x800DD9D0);
    w_u32(state + 24, 1);
    resource001_sub_800da838(1, 1, 0, 60);
    resource001_sub_800da838(3, 1, 0, 60);
    resource001_sub_800da838(15, 1, 0, 60);
    mode = resource001_sub_800da6fc();
    for (color = 0; color < 8; ++color)
    {
        descriptor[color * 2] = (uint8)coordinates[color];
        descriptor[color * 2 + 1] = (uint8)(coordinates[color] >> 8);
    }
    state = r_u32(0x800DD9D0);
    /* Only the first flag and RGB bytes are consumed by the packet builder */
    for (color = 0; color < 4; ++color)
    {
        descriptor[16 + color * 4] = 0;
        descriptor[17 + color * 4] = 112;
        descriptor[18 + color * 4] = 120;
    }
    descriptor[19] = 0;
    ordering = r_u32(state + 8);
    result = resource001_sub_800d94c4(0, ordering, mode + 56, descriptor);
    state = r_u32(0x800DD9D0);
    root = r_u32(0x800BCE60);
    w_u32(state + 8, result);
    flags = r_u32(root + 292);
    resource001_sub_800da9a8(5, (flags & 0x10) != 0);
    resource001_sub_800da828(190);
    resource001_sub_800da9a8(4, 1);
    state = r_u32(0x800DD9D0);
    w_u16(state + 124, 0x83D);
    w_u16(state + 126, 0);
    w_u16(state + 128, 0);
    w_u16(state + 130, 0);
    w_u32(state + 132, 0);
    resource001_sub_800da49c(2);
    state = r_u32(0x800DD9D0);
    w_u8(state + 209, 1);
    return resource001_sub_800da9a8(16, 2);
}

sint32 resource001_sub_800da898(uint32 index)
{
    uint32 state, entries;

    FUNCTION_MARKER(0x800DA898, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    entries = r_u32(state + 72);
    return r_s16(entries + index * 2);
}

uint32 resource001_sub_800d9624(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, color, component, offset, index, tag, link;

    FUNCTION_MARKER(0x800D9624, "ACE3.BIN");
    w_u8(packet + 3, 6);
    w_u8(packet + 7, 0x30);
    value = native_descriptor ? native_descriptor[19] : r_u8(descriptor + 19);
    w_u8(packet + 7, value ? 0x32 : 0x30);
    for (color = 0; color < 3; ++color)
        for (component = 0; component < 3; ++component)
        {
            offset = 16 + color * 4 + component;
            value = native_descriptor ? native_descriptor[offset] : r_u8(descriptor + offset);
            w_u8(packet + 4 + color * 8 + component, value);
        }
    for (index = 0; index < 5; ++index)
    {
        offset = index * 2;
        value = native_descriptor ? native_descriptor[offset] | (uint32)native_descriptor[offset + 1] << 8 : r_u16(descriptor + offset);
        w_u16(packet + 8 + (index / 2) * 8 + (index % 2) * 2, value);
    }
    value = native_descriptor ? native_descriptor[10] | (uint32)native_descriptor[11] << 8 : r_u16(descriptor + 10);
    tag = r_u32(packet);
    w_u16(packet + 26, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 28;
}

uint32 resource001_sub_800cdb64(void)
{
    uint32 first_card, second_card, mode, game, available, state, flags, packet, result;
    uint32 row, column, color, index, old_selection, selection, value, counter;
    sint32 delay, tick, phase, enabled;
    uint16 coordinates[6];
    uint8 descriptor[28];

    FUNCTION_MARKER(0x800CDB64, "ACE3.BIN");
    first_card = resource001_sub_800d3d90(0);
    second_card = resource001_sub_800d3d90(1);
    mode = resource001_sub_800da6fc();
    game = r_u32(0x800BCE60);
    available = r_u32(game + 2404);
    for (color = 0; color < 3; ++color)
    {
        descriptor[16 + color * 4] = 0;
        descriptor[17 + color * 4] = 112;
        descriptor[18 + color * 4] = 120;
    }
    descriptor[19] = 0;
    descriptor[23] = 0;
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 94);
    available &= 1;
    if (value < 30)
    {
        for (row = 0; row < 5; ++row)
            for (column = 0; column < 5; ++column)
            {
                state = r_u32(0x800DD9D0);
                value = r_u8(state + 94);
                delay = (sint32)(2 * (column + row) + 30 - value);
                if (delay >= 0)
                {
                    coordinates[0] = (uint16)(column * 9 + 275 - (uint32)delay);
                    coordinates[1] = (uint16)(100 - row * 18);
                    coordinates[2] = (uint16)(275 + column * 9);
                    coordinates[3] = coordinates[1];
                    coordinates[4] = coordinates[2];
                    coordinates[5] = (uint16)(2 * delay + 100 - row * 18);
                    for (index = 0; index < 6; ++index)
                    {
                        descriptor[index * 2] = (uint8)coordinates[index];
                        descriptor[index * 2 + 1] = (uint8)(coordinates[index] >> 8);
                    }
                    packet = r_u32(state + 8);
                    result = resource001_sub_800d9624(0, packet, mode + 56, descriptor);
                    state = r_u32(0x800DD9D0);
                    w_u32(state + 8, result);
                }
            }
        state = r_u32(0x800DD9D0);
        value = r_u8(state + 94);
        w_u8(state + 94, value + 1);
    }
    game = r_u32(0x800BCE60);
    tick = r_s32(game + 32);
    phase = tick / 64;
    state = r_u32(0x800DD9D0);
    packet = r_u32(state + 8);
    result = resource001_sub_800d9a08(70, 410, ((uint32)tick - (uint32)phase * 64) << 6, packet);
    state = r_u32(0x800DD9D0);
    w_u32(state + 8, result);
    enabled = resource001_sub_800da898(1);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if ((flags & 0x10) || enabled == 0)
    {
        if ((uint32)enabled - 1 < 2)
        {
            resource001_sub_800da838(1, 3, -1, -1);
            resource001_sub_800da838(3, 3, -1, -1);
            resource001_sub_800da838(8, 3, -1, -1);
            sub_8002db4c(89);
        }
        else
        {
            resource001_sub_800da838(1, 0, 0, 0);
            resource001_sub_800da838(3, 0, 0, 0);
            resource001_sub_800da838(8, 0, 0, 0);
            state = r_u32(0x800DD9D0);
            selection = r_u8(state + 89);
            if (selection == 2)
            {
                value = r_u16(state + 14);
                w_u16(state + 14, value + 1);
            }
            else
            {
                selection = r_u8(state + 89);
                w_u16(state + 14, 0);
                w_u16(state + 16, 0);
                w_u16(state + 12, selection + 3);
            }
        }
    }
    else if (flags & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 1);
        w_u16(state + 14, 0);
        w_u32(state + 120, 0);
        sub_8002db4c(59);
        resource_init_queue(0, 0);
        state = r_u32(0x800DD9D0);
        w_u32(state + 24, 0);
        resource001_sub_800d58b0();
        return resource001_sub_800d3ce8();
    }
    state = r_u32(0x800DD9D0);
    old_selection = r_u8(state + 89);
    enabled = resource001_sub_800da898(1);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if ((flags & 2) && enabled == 2)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        w_u8(state + 89, selection < 2 ? selection + 1 : 0);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        if (selection == 1 && first_card != 5 && second_card != 5)
            w_u8(state + 89, 2);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        if (selection == 2 && !available)
            w_u8(state + 89, 0);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if ((flags & 1) && enabled == 2)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        w_u8(state + 89, selection == 0 ? 2 : selection - 1);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        if (selection == 2 && !available)
            w_u8(state + 89, selection - 1);
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        if (selection == 1 && first_card != 5 && second_card != 5)
            w_u8(state + 89, 0);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 89);
    if (selection != old_selection)
        sub_8002db4c(61);
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 89);
    if (selection == 1 && first_card != 5 && second_card != 5 && resource001_sub_800da898(1) == 2)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 89, 0);
    }
    state = r_u32(0x800DD9D0);
    selection = r_u8(state + 89);
    resource001_sub_800da9a8(6, selection != 0);
    value = 2;
    if (first_card == 5 || second_card == 5)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        value = selection != 1;
    }
    resource001_sub_800da9a8(7, value);
    value = 2;
    if (available)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 89);
        value = selection != 2;
    }
    resource001_sub_800da9a8(8, value);
    resource001_sub_800da9a8(1, 1);
    state = r_u32(0x800DD9D0);
    counter = r_u32(state + 72);
    game = r_u32(0x800BCE60);
    w_u32(state + 72, counter + 1);
    flags = r_u32(game + 292);
    resource001_sub_800da9a8(5, (flags & 0x10) != 0);
    state = r_u32(0x800DD9D0);
    value = (uint32)(sint32)r_s16(state + 124);
    result = resource_is_cd_audio(value);
    if (result == 1)
    {
        state = r_u32(0x800DD9D0);
        value = (uint32)(sint32)r_s16(state + 12);
        result = 2;
        if (value == 2)
        {
            result = resource_wait(1);
            if (result == 0)
            {
                state = r_u32(0x800DD9D0);
                result = resource_queue_audio(state + 124, 1, 0);
            }
        }
    }
    return result;
}

uint32 resource001_sub_800d9748(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, index, offset, width, height, flags, texture_x, texture_y, tag, link;
    sint32 coordinate, u, v;

    FUNCTION_MARKER(0x800D9748, "ACE3.BIN");
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 0x2C);
    value = native_descriptor ? native_descriptor[19] : r_u8(descriptor + 19);
    w_u8(packet + 7, value ? 0x2E : 0x2C);
    for (index = 0; index < 3; ++index)
    {
        value = native_descriptor ? native_descriptor[16 + index] : r_u8(descriptor + 16 + index);
        w_u8(packet + 4 + index, value);
    }
    for (index = 0; index < 8; ++index)
    {
        offset = index * 2;
        value = native_descriptor ? native_descriptor[offset] | (uint32)native_descriptor[offset + 1] << 8 : r_u16(descriptor + offset);
        w_u16(packet + 8 + (index / 2) * 8 + (index % 2) * 2, value);
    }
    for (index = 0; index < 4; ++index)
    {
        coordinate = native_descriptor ? (sint16)(native_descriptor[32] | (uint32)native_descriptor[33] << 8) : r_s16(descriptor + 32);
        u = (sint32)((uint32)(coordinate % 64) << 16) >> 14;
        if (index & 1)
        {
            width = native_descriptor ? native_descriptor[36] : r_u8(descriptor + 36);
            u = (sint32)((uint32)u + width);
        }
        w_u8(packet + 12 + index * 8, u);
        coordinate = native_descriptor ? (sint16)(native_descriptor[34] | (uint32)native_descriptor[35] << 8) : r_s16(descriptor + 34);
        v = coordinate % 256;
        if (index >= 2)
        {
            height = native_descriptor ? native_descriptor[38] : r_u8(descriptor + 38);
            v = (sint32)((uint32)v + height);
        }
        w_u8(packet + 13 + index * 8, v);
    }
    flags = native_descriptor ? native_descriptor[23] : r_u8(descriptor + 23);
    texture_y = native_descriptor ? native_descriptor[34] | (uint32)native_descriptor[35] << 8 : r_u16(descriptor + 34);
    texture_x = native_descriptor ? native_descriptor[32] | (uint32)native_descriptor[33] << 8 : r_u16(descriptor + 32);
    value = ((flags & 3) << 5) | ((texture_y & 0x100) >> 4) | ((texture_x & 0x3FF) >> 6) | ((texture_y & 0x200) << 2);
    w_u16(packet + 22, value);
    texture_y = native_descriptor ? native_descriptor[42] | (uint32)native_descriptor[43] << 8 : r_u16(descriptor + 42);
    texture_x = native_descriptor ? native_descriptor[40] | (uint32)native_descriptor[41] << 8 : r_u16(descriptor + 40);
    w_u16(packet + 14, (texture_y << 6) | ((texture_x >> 4) & 0x3F));
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 40;
}

uint32 resource001_sub_800d9a08(uint32 x, uint32 y, uint32 angle, uint32 packet)
{
    uint32 ordering, layer, vertex, index, turn, radius, first, second, sum;
    uint16 coordinates[8], texture[6];
    uint8 descriptor[44];
    sint32 cosine, sine;

    FUNCTION_MARKER(0x800D9A08, "ACE3.BIN");
    ordering = resource001_sub_800da6fc();
    descriptor[16] = 128;
    descriptor[17] = 128;
    descriptor[18] = 128;
    descriptor[19] = 0;
    descriptor[23] = 0;
    texture[4] = 384;
    texture[5] = 10;
    for (layer = 0; layer < 2; ++layer)
    {
        turn = layer == 0 ? angle : 0 - angle;
        radius = layer == 0 ? 7 : 13;
        texture[0] = layer == 0 ? 739 : 738;
        texture[1] = layer == 0 ? 148 : 164;
        texture[2] = (uint16)radius;
        texture[3] = layer == 0 ? 15 : 27;
        for (index = 0; index < 6; ++index)
        {
            descriptor[32 + index * 2] = (uint8)texture[index];
            descriptor[33 + index * 2] = (uint8)(texture[index] >> 8);
        }
        for (vertex = 0; vertex < 4; ++vertex)
        {
            cosine = rcos((sint32)turn);
            sine = rsin((sint32)turn);
            first = (uint32)cosine * radius;
            second = (uint32)sine * radius;
            if ((vertex & 1) == 0)
                first = 0 - first;
            if (vertex < 2)
                second = 0 - second;
            first = (uint32)((sint32)first / 4096);
            second = (uint32)((sint32)second / 4096);
            sum = first + second;
            coordinates[vertex * 2] = (uint16)(x + (uint32)((sint32)sum / 2));
            sine = rsin((sint32)turn);
            cosine = rcos((sint32)turn);
            first = (uint32)sine * radius;
            second = (uint32)cosine * radius;
            if (vertex & 1)
                first = 0 - first;
            if (vertex < 2)
                second = 0 - second;
            first = (uint32)((sint32)first / 4096);
            second = (uint32)((sint32)second / 4096);
            coordinates[vertex * 2 + 1] = (uint16)(y + first + second);
        }
        for (index = 0; index < 8; ++index)
        {
            descriptor[index * 2] = (uint8)coordinates[index];
            descriptor[index * 2 + 1] = (uint8)(coordinates[index] >> 8);
        }
        packet = resource001_sub_800d9748(0, packet, ordering, descriptor);
    }
    return packet;
}

uint32 sub_8002b528(void)
{
    uint32 context, state, phase;

    FUNCTION_MARKER(0x8002B528, "ACE3.BIN");
    context = r_u32(0x800BCBA8);
    state = r_u32(context);
    if (state == 2)
    {
        phase = r_u32(context + 4);
        return phase == 2 ? 2 : 1;
    }
    if (state == 3)
    {
        phase = r_u32(context + 4);
        return phase == 0 ? 3 : 1;
    }
    return state == 4;
}

uint32 resource001_sub_800da4dc(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA4DC, "ACE3.BIN");
    state = r_u32(0x800DD910);
    return r_u32(state);
}

uint32 resource001_sub_800da4f4(uint32 resource)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA4F4, "ACE3.BIN");
    state = r_u32(0x800DD910);
    w_u32(state + 8, resource);
    return state;
}

uint32 resource001_sub_800da504(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA504, "ACE3.BIN");
    state = r_u32(0x800DD910);
    w_u32(state + 4, 0);
    w_u32(state, 0);
    return 1;
}

uint32 resource001_sub_800da520(void)
{
    uint32 current, state, index, type, previous;

    FUNCTION_MARKER(0x800DA520, "ACE3.BIN");
    current = (uint32)cd_resource_state();
    if (current == 0)
    {
        state = r_u32(0x800DD910);
        w_u32(state, 1);
        return 0;
    }
    state = r_u32(0x800DD910);
    index = r_u32(state + 8);
    type = r_u8(0x800DD8D4 + index);
    if (type == 3 || current == 3)
    {
        w_u32(state, 0);
        return 2;
    }
    if (current == 4 || current == 0xFFFFFFFF)
    {
        if (type == 1)
        {
            previous = r_u32(state + 4);
            w_u32(state, previous ? 4 : 2);
        }
        state = r_u32(0x800DD910);
        index = r_u32(state + 8);
        type = r_u8(0x800DD8D4 + index);
        if (type == 2)
        {
            previous = r_u32(state + 4);
            w_u32(state, previous ? 5 : 3);
        }
    }
    else if (current == 1)
    {
        if (type == 1)
        {
            w_u32(state, 6);
            return 2;
        }
        previous = r_u32(state + 4);
        w_u32(state, previous ? 5 : 3);
    }
    else if (current == 2)
    {
        if (type == 2)
        {
            w_u32(state, 6);
            return 2;
        }
        previous = r_u32(state + 4);
        w_u32(state, previous ? 4 : 2);
    }
    else
        return 0;
    sub_8002b428();
    return 1;
}

uint32 resource001_sub_800da694(void)
{
    FUNCTION_MARKER(0x800DA694, "ACE3.BIN");
    return sub_8002b528() == 3;
}

uint32 resource001_sub_800da6b8(void)
{
    uint32 state, value;

    FUNCTION_MARKER(0x800DA6B8, "ACE3.BIN");
    value = sub_8002b528();
    if (value != 1)
        return 0;
    state = r_u32(0x800DD910);
    w_u32(state, value);
    w_u32(state + 4, value);
    return 4;
}

uint32 resource001_sub_800ce24c(void)
{
    uint32 state, game, step, resource;

    FUNCTION_MARKER(0x800CE24C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    game = r_u32(0x800BCE60);
    step = r_u16(state + 14);
    w_u32(state + 224, 0);
    w_u16(state + 14, step + 1);
    resource = (uint32)(sint32)r_s16(game + 908);
    return resource001_sub_800da4f4(resource);
}

uint32 resource001_sub_800ce290(void)
{
    uint32 state, step, target, value, status;

    FUNCTION_MARKER(0x800CE290, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u32(state + 224);
    target = r_u32(0x800DD914 + step * 4);
    switch (target)
    {
        case 0x800DA504:
            value = resource001_sub_800da504();
            break;
        case 0x800DA520:
            value = resource001_sub_800da520();
            break;
        case 0x800DA694:
            value = resource001_sub_800da694();
            break;
        case 0x800DA6B8:
            value = resource001_sub_800da6b8();
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound menu transfer phase %08X\n", target);
#endif
            abort();
    }
    if (value & 1)
    {
        state = r_u32(0x800DD9D0);
        step = r_u32(state + 224);
        w_u32(state + 224, step + 1);
    }
    if (value & 2)
    {
        state = r_u32(0x800DD9D0);
        w_u8(state + 88, 1);
        return 1;
    }
    if (value & 4)
    {
        state = r_u32(0x800DD9D0);
        w_u32(state + 224, 1);
    }
    status = resource001_sub_800da4dc();
    state = r_u32(0x800DD9D0);
    step = r_u32(state + 224);
    if ((sint32)step >= 2)
    {
        resource001_sub_800da828(0x400036);
        resource001_sub_800da9a8(4, 4);
    }
    switch (status)
    {
        case 1:
            resource001_sub_800da9a8(1, 73);
            break;
        case 2:
            resource001_sub_800da9a8(1, 70);
            break;
        case 3:
            resource001_sub_800da9a8(1, 71);
            break;
        case 4:
            resource001_sub_800da9a8(1, 101);
            break;
        case 5:
            resource001_sub_800da9a8(1, 102);
            break;
        case 6:
            return resource001_sub_800da9a8(1, 0);
    }
    return 6;
}

uint32 resource001_sub_800ce3f4(void)
{
    uint32 game, base, index, flags;
    sint32 remaining;

    FUNCTION_MARKER(0x800CE3F4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    base = game + 2396;
    w_u8(base, 0);
    w_u8(base + 1, 6);
    w_u8(base + 2, 0);
    for (index = 0; index < 60; ++index)
    {
        w_u16(base + index * 2 + 104, 0x8CA0);
        w_u16(base + index * 2 + 224, 0x8CA0);
        w_u16(base + index * 2 + 344, 0x8CA0);
        w_u16(base + index * 2 + 464, 0x8CA0);
        w_u16(base + index * 2 + 584, 0x8CA0);
    }
    for (index = 0; index < 60; ++index)
    {
        w_u8(base + index + 704, 0);
        w_u8(base + index + 764, 0);
        w_u8(base + index + 824, 0);
        w_u8(base + index + 884, 0);
        w_u8(base + index + 944, 0);
    }
    for (remaining = 59; remaining >= 0; --remaining)
        w_u8(base + remaining + 1004, 0);
    for (remaining = 179; remaining >= 0; --remaining)
        w_u8(base + remaining + 1084, 0);
    for (index = 0; index < 60; ++index)
    {
        w_u32(base + index * 4 + 1264, 0);
        w_u32(base + index * 4 + 1504, 0);
    }
    w_u32(base + 1744, 0);
    w_u32(base + 1748, 0);
    w_u32(base + 1752, 0);
    for (remaining = 63; remaining >= 0; --remaining)
        w_u8(base + remaining + 24, 0);
    for (remaining = 15; remaining >= 0; --remaining)
        w_u8(base + remaining + 88, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 2412) & 0xFFFFFFEF;
    w_u32(game + 2412, flags);
    return flags;
}

uint32 resource001_sub_800ce518(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CE518, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 16, 0);
    w_u32(state + 72, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(11);
    resource001_sub_800da77c(6);
    resource001_sub_800da828(0x85E);
    resource001_sub_800da838(15, 1, 0, 60);
    resource001_sub_800da9a8(1, 15);
    resource001_sub_800da9a8(4, 22);
    resource001_sub_800dbf6c(111, 150, 1, 1);
    return resource001_sub_800da49c(14);
}

uint32 resource001_sub_800d5970(sint32 index)
{
    uint32 state, buffer;

    FUNCTION_MARKER(0x800D5970, "ACE3.BIN");
    if (index == 0)
    {
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 760);
        return buffer + 8;
    }
    if (index < 4)
    {
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 748 + ((uint32)index - 1) * 4);
        return buffer + 8;
    }
    return 0x800CC504;
}

uint32 resource001_sub_800ce5ac(void)
{
    uint32 state, key, value, game, directions, buttons, buffer, bank, table, character, sound, result;
    sint32 length;

    FUNCTION_MARKER(0x800CE5AC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    key = r_u8(state + 91);
    resource001_sub_800da9a8(2, key);
    buffer = resource001_sub_800d5970(0);
    length = (sint32)guest_strlen(buffer);
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    if (directions & 8)
    {
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        value = key + 1;
        if (key < 14)
        {
            if (key == 13)
                value = 0;
        }
        else if (key < 27)
        {
            if (key == 26)
                value = 14;
        }
        else if (key < 39)
        {
            if (key == 38)
                value = 26;
        }
        else if (key < 50)
        {
            if (key == 49)
                value = 39;
        }
        else if (key < 52)
        {
            if (key == 51)
                value = 50;
        }
        else
            goto moved;
        w_u8(state + 91, value);
        goto moved;
    }
    if (directions & 4)
    {
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        value = key - 1;
        if (key < 14)
        {
            if (key == 0)
                value = 13;
        }
        else if (key < 27)
        {
            if (key == 14)
                value = 26;
        }
        else if (key < 39)
        {
            if (key == 27)
                value = 26;
        }
        else if (key < 50)
        {
            if (key == 39)
                value = 49;
        }
        else if (key < 52)
        {
            if (key == 50)
                value = 51;
        }
        else
            goto moved;
        w_u8(state + 91, value);
        goto moved;
    }
    if (directions & 2)
    {
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        if (key < 14)
            value = key == 13 ? key + 13 : key + 14;
        else if (key < 27)
            value = key == 26 ? key + 23 : key + 13;
        else if (key < 39)
            value = key == 38 ? key + 11 : key + 12;
        else if (key < 48)
            value = 50;
        else if (key < 50)
            value = 51;
        else if (key == 50)
            value = 2;
        else if (key == 51)
            value = 9;
        else
            goto moved;
        w_u8(state + 91, value);
        goto moved;
    }
    if (directions & 1)
    {
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        if (key < 9)
            value = 50;
        else if (key < 14)
            value = 51;
        else if (key < 27)
            value = key - 14;
        else if (key < 39)
            value = key - 13;
        else if (key < 50)
            value = key - 12;
        else if (key == 50)
            value = 41;
        else
        {
            state = r_u32(0x800DD9D0);
            key = r_u8(state + 91);
            if (key >= 52)
                goto moved;
            value = 48;
        }
        w_u8(state + 91, value);
        goto moved;
    }
    buttons = r_u32(game + 300);
    if (buttons & 0x40)
    {
        sound = 48;
        if (length > 0)
        {
            state = r_u32(0x800DD9D0);
            w_u16(state + 76, 1);
            w_u16(state + 78, 0);
            sound = 89;
        }
        goto play_sound;
    }
    if (buttons & 0x300)
        goto toggle_bank;
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        if (key == 26)
        {
            sound = 48;
            if (length > 0)
            {
                w_u16(state + 76, 1);
                w_u16(state + 78, 0);
                sound = 89;
            }
            goto play_sound;
        }
        if (key < 27)
        {
            if (key == 13)
            {
                if (length <= 0)
                {
                    w_u8(buffer + (uint32)length, 0);
                    goto finish;
                }
                --length;
                goto erase_character;
            }
        }
        else
        {
            if (key == 49)
                goto toggle_bank;
            if (key == 51)
            {
                state = r_u32(0x800DD9D0);
                w_u8(state + 91, 0);
                state = r_u32(0x800DD9D0);
#if 0
                /* Original return to link-mode selection */
                w_u16(state + 12, 2);
#else
                w_u16(state + 12, 1);
#endif
                w_u16(state + 14, 0);
                w_u16(state + 16, 0);
                resource001_sub_800da77c(7);
                resource001_sub_800da77c(9);
                resource001_sub_800da828(190);
                sub_8002db4c(59);
                value = r_u32(0x800DDA30);
                arena_set_cursor(value);
                goto finish;
            }
        }
        if (length >= 8)
            goto finish;
        state = r_u32(0x800DD9D0);
        bank = r_u8(state + 92);
        key = r_u8(state + 91);
        table = r_u32(state + bank * 4 + 100);
        character = r_u8(table + key * 8 + 4);
        if (character == 32 && length == 0)
        {
            sound = 48;
            goto play_sound;
        }
        state = r_u32(0x800DD9D0);
        bank = r_u8(state + 92);
        key = r_u8(state + 91);
        table = r_u32(state + bank * 4 + 100);
        character = r_u8(table + key * 8 + 4);
        w_u8(buffer + (uint32)length, character);
        w_u8(buffer + (uint32)length + 1, 0);
        sound = 47;
        goto play_sound;
    }
    if (buttons & 0x20)
    {
        if (length > 0)
        {
            --length;
            goto erase_character;
        }
        state = r_u32(0x800DD9D0);
        key = r_u8(state + 91);
        if (key == 51)
            goto finish;
        w_u8(state + 91, 51);
        goto moved;
    }
    goto finish;
toggle_bank:
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 92);
    w_u8(state + 92, value ^ 1);
    sound = 47;
    goto play_sound;
erase_character:
    sub_8002db4c(46);
    w_u8(buffer + (uint32)length, 0);
    goto finish;
moved:
    sound = 61;
play_sound:
    sub_8002db4c(sound);
finish:
    state = r_u32(0x800DD9D0);
    result = r_u32(state + 72) + 1;
    w_u32(state + 72, result);
    return result;
}

uint32 sub_800adea8(uint32 destination, uint32 source)
{
    uint32 result = destination, value;

    FUNCTION_MARKER(0x800ADEA8, "ACE3.BIN");
    if (destination == 0 || source == 0)
        return 0;
    do
    {
        value = r_u8(source++);
        w_u8(destination++, value);
    } while (value != 0);
    return result;
}

uint32 resource001_sub_800ceb20(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CEB20, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(12);
    resource001_sub_800da828(0x105E);
    resource001_sub_800da9a8(1, 16);
    resource001_sub_800da9a8(4, 22);
    resource001_sub_800dbf6c(111, 150, 1, 0);
    return resource001_sub_800da49c(14);
}

uint32 resource001_sub_800ceb90(void)
{
    uint32 game, state, flags, name;

    FUNCTION_MARKER(0x800CEB90, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    if (flags & 0x10)
    {
        flags = r_u32(game + 300);
        if (flags & 0x10)
        {
            state = r_u32(0x800DD9D0);
            w_u16(state + 76, 2);
            w_u16(state + 78, 0);
            name = resource001_sub_800d5970(0);
            game = r_u32(0x800BCE60);
            sub_800adea8(game + 2376, name);
            resource001_sub_800ce3f4();
            game = r_u32(0x800BCE60);
            flags = r_u32(game + 2404);
            w_u32(game + 2404, flags & 0xFFFFFFFE);
            sub_8002db4c(89);
        }
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    if ((flags & 0x20) == 0)
        return 0;
    flags = r_u32(game + 300);
    if ((flags & 0x20) == 0)
        return 0x800E0000;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800cec70(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CEC70, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 72, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(10);
    resource001_sub_800da828(0x43E);
    resource001_sub_800da838(15, 1, 0, 60);
    resource001_sub_800da9a8(1, 11);
    resource001_sub_800da9a8(4, 6);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    return resource001_sub_800da49c(16);
}

uint32 resource001_sub_800cecf8(void)
{
    uint32 game, state, directions, buttons, index, result;
    sint32 selection;

    FUNCTION_MARKER(0x800CECF8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    selection = r_s32(game + 916);
    directions = r_u32(game + 304);
    buttons = r_u32(game + 300);
    if (selection < 0)
        selection = 0;
    else if (selection >= 3)
        selection = 2;
    resource001_sub_800d7d08(4);
    resource001_sub_800d7c60(128, 128, 128);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7c40(106, 166);
    resource001_sub_800d8aa0(0x800CBFF4, 0, 0, 0);
    resource001_sub_800d7c40(106, 218);
    resource001_sub_800d8aa0(0x800CBFFC, 0, 0, 0);
    resource001_sub_800d7c40(106, 270);
    resource001_sub_800d8aa0(0x800CC004, 0, 0, 0);
    if (buttons & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 0);
        w_u16(state + 78, 0);
        sub_8002db4c(59);
    }
    else if (buttons & 0x10)
    {
#if 0
        /* Original configuration and memory-card setup sequence */
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 3);
        w_u16(state + 78, 0);
#else
        resource001_sub_800da1b8();
        game = r_u32(0x800BCE60);
        w_u16(game + 908, 1);
        w_u16(game + 0x954, 2);
        w_u32(game + 0x968, resource001_sub_800d4140());
        if (!game_account_save())
            return 0;
        state = r_u32(0x800DD9D0);
        w_u16(state + 12, 6);
        w_u16(state + 14, 0);
#endif
        sub_8002db4c(89);
    }
    else if (directions & 3)
    {
        if (directions & 2)
            selection = (selection + 1) % 3;
        if (directions & 1)
            selection = (selection + 2) % 3;
        game = r_u32(0x800BCE60);
        w_u32(game + 916, selection);
        sub_8002db4c(61);
    }
    for (index = 0; index < 3; ++index)
        resource001_sub_800da9a8(index + 6, index != (uint32)selection);
    state = r_u32(0x800DD9D0);
    result = r_u32(state + 72) + 1;
    w_u32(state + 72, result);
    return result;
}

uint32 resource001_sub_800ceecc(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CEECC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 80, 0);
    w_u32(state + 72, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(13);
    resource001_sub_800da828(0x2056);
    resource001_sub_800da9a8(1, 17);
    resource001_sub_800da9a8(4, 23);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    return resource001_sub_800da49c(17);
}

uint32 resource001_sub_800cef44(void)
{
    uint32 game, state, flags, result;

    FUNCTION_MARKER(0x800CEF44, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    if (flags & 0x10)
    {
        flags = r_u32(game + 300);
        if (flags & 0x10)
        {
            state = r_u32(0x800DD9D0);
            w_u16(state + 76, 4);
            w_u16(state + 78, 0);
            sub_8002db4c(89);
            goto finish;
        }
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 296);
    if (flags & 0x20)
    {
        flags = r_u32(game + 300);
        if (flags & 0x20)
        {
            state = r_u32(0x800DD9D0);
            w_u16(state + 76, 7);
            w_u16(state + 78, 0);
            sub_8002db4c(59);
        }
    }
finish:
    state = r_u32(0x800DD9D0);
    result = r_u32(state + 72) + 1;
    w_u32(state + 72, result);
    return result;
}

uint32 resource001_sub_800cf004(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CF004, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u32(state + 72, 0);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(14);
    resource001_sub_800da828(0x4016);
    resource001_sub_800da9a8(1, 18);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    return resource001_sub_800da49c(18);
}

uint32 resource001_sub_800d9400(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, index, offset, tag, link;

    FUNCTION_MARKER(0x800D9400, "ACE3.BIN");
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    value = native_descriptor ? native_descriptor[11] : r_u8(descriptor + 11);
    w_u8(packet + 7, value ? 0x62 : 0x60);
    for (index = 0; index < 3; ++index)
    {
        value = native_descriptor ? native_descriptor[8 + index] : r_u8(descriptor + 8 + index);
        w_u8(packet + 4 + index, value);
    }
    for (index = 0; index < 3; ++index)
    {
        offset = index * 2;
        value = native_descriptor ? native_descriptor[offset] | (uint32)native_descriptor[offset + 1] << 8 : r_u16(descriptor + offset);
        w_u16(packet + 8 + offset, value);
    }
    value = native_descriptor ? native_descriptor[6] | (uint32)native_descriptor[7] << 8 : r_u16(descriptor + 6);
    tag = r_u32(packet);
    w_u16(packet + 14, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 16;
}

uint32 resource001_sub_800da2a8(uint32 channel)
{
    uint32 state, first, second;
    sint32 pending;

    FUNCTION_MARKER(0x800DA2A8, "ACE3.BIN");
    state = r_u32(0x800DDA60);
    pending = game_spu_read_decoded_native(state + 24, 5);
    SpuIsTransferCompleted(1);
    state = r_u32(0x800DDA60);
    if (pending != 0)
    {
        first = state + 24;
        second = state + 1048;
    }
    else
    {
        first = state + 224;
        second = state + 1248;
    }
    if (channel == 0)
        return first;
    if (channel == 1)
        return second;
    return 1;
}

sint32 resource001_sub_800da340(void)
{
    uint32 first, second, first_total = 0, second_total = 0, index;
    sint32 first_sample, second_sample;

    FUNCTION_MARKER(0x800DA340, "ACE3.BIN");
    first = resource001_sub_800da2a8(0);
    second = resource001_sub_800da2a8(1);
    for (index = 0; index < 100; ++index)
    {
        first_sample = r_s16(first);
        first += 2;
        second_sample = r_s16(second);
        second += 2;
        if (first_sample < 0)
            first_sample = -first_sample;
        first_total += first_sample;
        if (second_sample < 0)
            second_sample = -second_sample;
        second_total += second_sample;
    }
    return (sint16)((first_total / 100 + second_total / 100) / 2);
}

uint32 sub_8002bd28(uint32 mode)
{
    uint32 state;

    FUNCTION_MARKER(0x8002BD28, "ACE3.BIN");
    state = r_u32(0x800BCBBC);
    w_u32(state + 352, mode);
    return state;
}

/* Native arithmetic helper for the original byte fader scale */
static uint32 resource001_settings_scale_byte(uint32 value)
{
    uint32 product;
    sint32 high, quotient;

    product = (value << 3) - value;
    product = (product << 3) - value;
    product = (product << 2) + value;
    high = (sint32)(((sint64)(sint32)product * (sint32)0x80808081) >> 32);
    quotient = (sint32)((uint32)high + product) >> 7;
    return (uint32)quotient - (uint32)((sint32)product >> 31);
}

uint32 resource001_sub_800cf06c(void)
{
    struct
    {
        uint16 x, y, width, height;
        uint8 red, green, blue, semi;
    } meter, bar;

    uint32 ordering, meter_ordering, state, game, packet, result, value, flags, selection, channel, product;
    uint32 directions, buttons, offset;
    sint32 high, quotient, volume;

    FUNCTION_MARKER(0x800CF06C, "ACE3.BIN");
    ordering = resource001_sub_800da6fc();
    value = (uint32)resource001_sub_800da340();
    product = (value << 4) - value;
    high = (sint32)(((sint64)(sint32)product * (sint32)0x80010003) >> 32);
    quotient = (sint32)((uint32)high + product) >> 14;
    value = (uint32)quotient - (uint32)((sint32)product >> 31);
    meter.x = 68;
    meter.y = 224;
    meter.width = (uint16)(value * 7);
    meter.height = 12;
    meter.red = 200;
    meter.green = 200;
    meter.blue = 200;
    /* User-authorized fix for the original uninitialized blend flags */
    meter.semi = 0;
    bar.semi = 0;
    meter_ordering = resource001_sub_800da6fc();
    state = r_u32(0x800DD9D0);
    packet = r_u32(state + 8);
    result = resource001_sub_800d9400(0, packet, meter_ordering, (const uint8 *)&meter);
    state = r_u32(0x800DD9D0);
    game = r_u32(0x800BCE60);
    w_u32(state + 8, result);
    flags = r_u8(game + 922);
    if (flags & 1)
        value = 2;
    else
        value = r_u32(0x800DD4C0) != 0;
    resource001_sub_800da9a8(6, value);
    game = r_u32(0x800BCE60);
    flags = r_u8(game + 922);
    value = (flags & 1) ? r_u32(0x800DD4C0) != 0 : 2;
    resource001_sub_800da9a8(7, value);
    game = r_u32(0x800BCE60);
    flags = r_u8(game + 922);
    sub_8002bd28((flags & 1) != 0);
    sub_8002bd5c();
    game = r_u32(0x800BCE60);
    value = r_u8(game + 920);
    for (channel = 0; channel < 2; ++channel)
    {
        bar.x = (uint16)(resource001_settings_scale_byte(value) + 69);
        bar.y = (uint16)(229 + channel * 78);
        bar.width = 4;
        bar.height = 64;
        bar.red = 0;
        bar.green = 0;
        bar.blue = 0;
        if (channel == 0)
            state = r_u32(0x800DD9D0);
        packet = r_u32(state + 8);
        result = resource001_sub_800d9400(0, packet, ordering + 4, (const uint8 *)&bar);
        state = r_u32(0x800DD9D0);
        w_u32(state + 8, result);
        bar.x = 70;
        selection = r_u32(0x800DD4C0);
        if (selection == channel + 1)
        {
            bar.red = 200;
            bar.green = 200;
            bar.blue = 200;
        }
        else
        {
            bar.red = 8;
            bar.green = 200;
            bar.blue = 192;
        }
        game = r_u32(0x800BCE60);
        value = r_u8(game + 920 + channel);
        state = r_u32(0x800DD9D0);
        bar.width = (uint16)resource001_settings_scale_byte(value);
        packet = r_u32(state + 8);
        result = resource001_sub_800d9400(0, packet, ordering + 4, (const uint8 *)&bar);
        state = r_u32(0x800DD9D0);
        game = r_u32(0x800BCE60);
        w_u32(state + 8, result);
        if (channel == 0)
            value = r_u8(game + 921);
    }
    directions = r_u32(game + 304);
    if (directions & 2)
    {
        selection = r_u32(0x800DD4C0);
        w_u32(0x800DD4C0, (sint32)selection < 2 ? selection + 1 : 0);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    if (directions & 1)
    {
        selection = r_u32(0x800DD4C0);
        w_u32(0x800DD4C0, (sint32)selection > 0 ? selection - 1 : 2);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    if (directions & 12)
    {
        selection = r_u32(0x800DD4C0);
        if (selection == 0)
        {
            if (directions & 8)
            {
                flags = r_u8(game + 922);
                w_u8(game + 922, (flags & 1) ? flags & 0xFE : flags | 1);
            }
            if (directions & 4)
            {
                game = r_u32(0x800BCE60);
                flags = r_u8(game + 922);
                w_u8(game + 922, (flags & 1) ? flags & 0xFE : flags | 1);
            }
            sub_8002db4c(61);
        }
        else if (selection == 1 || selection == 2)
        {
            offset = 919 + selection;
            volume = r_u8(game + offset);
            if (directions & 8)
                volume += 16;
            if (directions & 4)
                volume -= 16;
            if (volume >= 256)
                volume = 255;
            else if (volume < 0)
                volume = 0;
            game = r_u32(0x800BCE60);
            value = r_u8(game + offset);
            if (value != (uint32)volume)
            {
                sub_8002db4c(46);
                game = r_u32(0x800BCE60);
                w_u8(game + offset, volume);
            }
        }
    }
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 6);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
    }
    else if (buttons & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 3);
        w_u16(state + 78, 0);
        sub_8002db4c(59);
    }
    resource001_sub_800da9a8(8, 0);
    resource001_sub_800da9a8(9, 0);
    resource001_sub_800da9a8(10, 0);
    selection = r_u32(0x800DD4C0);
    resource001_sub_800da9a8(selection + 8, 1);
    state = r_u32(0x800DD9D0);
    result = r_u32(state + 72) + 1;
    w_u32(state + 72, result);
    return result;
}

uint32 resource001_sub_800cf750(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CF750, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(15);
    resource001_sub_800da828(0x8010);
    resource001_sub_800da9a8(1, 20);
    resource001_sub_800dbf6c(64, 50, 0, 0);
    return resource001_sub_800da49c(19);
}

uint32 resource001_sub_800cf7b4(void)
{
    uint32 game, root, x, y, directions, flags, state, value;
    sint32 dx = 0, dy = 0;

    FUNCTION_MARKER(0x800CF7B4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    directions = r_u32(game + 304);
    root = r_u32(0x800BCE48);
    x = r_u32(root + 8);
    y = r_u32(root + 12);
    if (directions & 15)
    {
        if (directions & 8)
            dx = (sint32)x < 12;
        if ((directions & 4) && (sint32)x >= -11)
            dx = -1;
        if ((directions & 2) && (sint32)y < 8)
            dy = 1;
        if ((directions & 1) && (sint32)y > 0)
            dy = -1;
        if (dx != 0 || dy != 0)
        {
            sub_8002db4c(46);
            root = r_u32(0x800BCE48);
            w_u32(root + 8, x + (uint32)dx);
            w_u32(root + 12, y + (uint32)dy);
        }
    }
    resource001_sub_800da9a8(6, dy == -1);
    resource001_sub_800da9a8(7, dx == 1);
    resource001_sub_800da9a8(8, dy == 1);
    resource001_sub_800da9a8(9, dx == -1);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 292);
    if (flags & 0x10)
    {
        if (r_s16(game + 104) >= 129)
        {
            root = r_u32(0x800BCE48);
            w_u32(root + 8, 0xFFFFFFFE);
            w_u32(root + 12, 0);
        }
    }
    else if (r_u16(game + 96) & 0x1000)
    {
        root = r_u32(0x800BCE48);
        w_u32(root + 8, 0xFFFFFFFE);
        w_u32(root + 12, 0);
        sub_8002db4c(48);
    }
    if (sub_800a8ef0() == 1)
    {
        root = r_u32(0x800BCE48);
        value = r_u32(root);
        w_u32(root, value | 2);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if ((flags & 0x30) == 0)
        return 0x800C0000;
    root = r_u32(0x800BCE48);
    value = r_u32(root);
    w_u32(root, value | 2);
    if (flags & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 7);
        w_u16(state + 78, 0);
        return sub_8002db4c(89);
    }
    if ((flags & 0x20) == 0)
        return 0;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 6);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800cfa34(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CFA34, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da828(0x10016);
    resource001_sub_800da9a8(1, 19);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    return resource001_sub_800da49c(20);
}

uint32 resource001_sub_800cfa94(void)
{
    uint32 game, flags, state, root, value;

    FUNCTION_MARKER(0x800CFA94, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x10)
    {
        state = r_u32(0x800DD9D0);
        root = r_u32(0x800BCE48);
        w_u16(state + 76, 5);
        w_u16(state + 78, 0);
        value = r_u16(root + 8);
        w_u16(state + 220, value);
        value = r_u16(root + 12);
        w_u16(state + 222, value);
        sub_8002db4c(89);
    }
    else if (flags & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 4);
        w_u16(state + 78, 0);
        sub_8002db4c(59);
    }
    return resource001_sub_800dbb30();
}

uint32 resource001_sub_800dbb30(void)
{
    static const uint16 left[4] = {90, 102, 113, 129};
    static const uint16 right[4] = {95, 107, 118, 134};
    uint32 state, mode, packet, ordering, result, strip, index;
    uint16 coordinates[8];
    uint8 descriptor[32], red, green, blue;

    FUNCTION_MARKER(0x800DBB30, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    mode = r_u32(0x800DD9DC);
    descriptor[19] = 0;
    descriptor[16] = 0;
    descriptor[17] = 0;
    descriptor[18] = 0;
    descriptor[20] = 0;
    descriptor[21] = 0;
    descriptor[22] = 0;
    for (strip = 0; strip < 4; ++strip)
    {
        coordinates[0] = left[strip];
        coordinates[1] = 213;
        coordinates[2] = right[strip];
        coordinates[3] = 213;
        coordinates[4] = left[strip];
        coordinates[5] = 381;
        coordinates[6] = right[strip];
        coordinates[7] = 381;
        for (index = 0; index < 8; ++index)
        {
            descriptor[index * 2] = (uint8)coordinates[index];
            descriptor[index * 2 + 1] = (uint8)(coordinates[index] >> 8);
        }
        red = strip == 0 || strip == 3 ? 255 : 0;
        green = strip == 1 || strip == 3 ? 255 : 0;
        blue = strip >= 2 ? 255 : 0;
        descriptor[24] = red;
        descriptor[25] = green;
        descriptor[26] = blue;
        descriptor[28] = red;
        descriptor[29] = green;
        descriptor[30] = blue;
        packet = r_u32(state + 8);
        ordering = r_u32(mode + 16);
        result = resource001_sub_800d94c4(0, packet, ordering, descriptor);
        state = r_u32(0x800DD9D0);
        w_u32(state + 8, result);
        if (strip != 3)
            mode = r_u32(0x800DD9DC);
    }
    return result;
}

uint32 resource001_sub_800cfb2c(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CFB2C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da77c(17);
    resource001_sub_800da828(0x20056);
    resource001_sub_800da838(2, 3, -1, -1);
    resource001_sub_800da9a8(2, 1);
    resource001_sub_800da9a8(1, 22);
    resource001_sub_800da9a8(4, 22);
    resource001_sub_800dbf6c(64, 50, 1, 0);
    resource001_sub_800da49c(21);
    state = r_u32(0x800DD9D0);
    w_u8(state + 209, 1);
    return 1;
}

uint32 resource001_sub_800cfbd0(void)
{
    uint32 game, buttons, state;

    FUNCTION_MARKER(0x800CFBD0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    if (buttons & 0x10)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        return sub_8002db4c(89);
    }
    if (buttons & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 8);
        w_u16(state + 78, 0);
        return sub_8002db4c(59);
    }
    return 0;
}

uint32 resource001_sub_800cfc38(void)
{
    uint32 state, step;

    FUNCTION_MARKER(0x800CFC38, "ACE3.BIN");
    resource001_sub_800da9a8(2, 2);
    resource001_sub_800da9a8(1, 23);
    state = r_u32(0x800DD9D0);
    step = r_u16(state + 78);
    w_u16(state + 78, step + 1);
    resource001_sub_800da49c(22);
    return resource001_sub_800da9a8(4, 22);
}

uint32 resource001_sub_800cfc94(void)
{
    uint32 game, buttons, state, value;

    FUNCTION_MARKER(0x800CFC94, "ACE3.BIN");
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
        return 0x800E0000;
    state = r_u32(0x800DD9D0);
    w_u16(state + 76, 0);
    w_u16(state + 78, 0);
    return sub_8002db4c(59);
}

uint32 resource001_sub_800d4140(void)
{
    uint32 game;

    FUNCTION_MARKER(0x800D4140, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return r_u32(game + 32);
}

uint32 resource001_sub_800d4158(void)
{
    uint32 card, game, source;

    FUNCTION_MARKER(0x800D4158, "ACE3.BIN");
    card = r_u32(0x800DDA48);
    game = r_u32(0x800BCE60);
    source = r_u32(card + 0x2F8);
    return sub_800adea8(game + 0x948, source + 8);
}

uint32 resource001_sub_800da1b8(void)
{
    uint32 game, record, cursor, index;

    FUNCTION_MARKER(0x800DA1B8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    record = game + 0x95C;
    cursor = game + 0xA0F;
    for (index = 0; index < 180; ++index)
    {
        w_u8(cursor + 0x43C, 0);
        --cursor;
    }
    cursor = record + 79;
    for (index = 0; index < 80; ++index)
    {
        w_u8(cursor + 0x3EC, 0);
        --cursor;
    }
    for (index = 0; index < 60; ++index)
    {
        w_u8(record + index + 0x374, 0);
        w_u8(record + index + 0x338, 0);
        w_u8(record + index + 0x2FC, 0);
        w_u8(record + index + 0x2C0, 0);
        w_u8(record + index + 0x3B0, 0);
        w_u16(record + index * 2 + 0x1D0, 0xD2F0);
        w_u16(record + index * 2 + 0x158, 0xD2F0);
        w_u16(record + index * 2 + 0xE0, 0xD2F0);
        w_u16(record + index * 2 + 0x68, 0xD2F0);
        w_u16(record + index * 2 + 0x248, 0xD2F0);
        w_u32(record + index * 4 + 0x4F0, 0);
        w_u32(record + index * 4 + 0x5E0, 0);
    }
    w_u32(record + 0x6D0, 0);
    w_u32(record + 0x6D4, 0);
    w_u32(record + 0x6D8, 0);
    return 0;
}
