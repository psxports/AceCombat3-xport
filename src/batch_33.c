#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

uint32 resource002_sub_800fa4e4(void)
{
    uint32 owner, state, game, value, mission, aircraft, variant, sound, scene, result;
    FUNCTION_MARKER(0x800FA4E4, "ACE3.BIN");
    resource002_sub_800def94(1, 0);
    owner = r_u32(0x80112E10);
    resource002_sub_800def94(4, r_s16(owner + 2) == 1);
    if (resource002_sub_800dee84(6))
    {
        value = resource002_sub_800deea8(6);
        resource002_sub_800f7c24(25 - value);
    }
    else
    {
        owner = r_u32(0x80112E10);
        if (r_s16(owner + 2) == 1)
        {
            resource002_sub_800dee08(8, 1, 0, 25);
            resource002_sub_800deca8(0, 11);
            resource002_sub_800dedf0(0, 0x7A01);
            sub_8002db4c(90);
            owner = r_u32(0x80112E10);
            w_u16(owner, 3);
            value = r_u32(owner + 16);
            w_u32(owner + 16, value & 0xFFFFFFFD);
            game = r_u32(0x800BCE60);
            state = r_u32(0x80112DC4);
            mission = (uint32)r_s16(game + 908);
            variant = r_u32(state + 192);
            value = resource002_sub_800fe8d8(mission, variant);
            aircraft = (uint32)(sint32)(sint16)value;
            game = r_u32(0x800BCE60);
            mission = (uint32)r_s16(game + 908);
            w_u16(game + 910, value);
            value = resource002_sub_800fead4(mission, aircraft, 0);
            owner = r_u32(0x80112E10);
            w_u8(owner + 22, value);
            w_u8(owner + 20, value);
            game = r_u32(0x800BCE60);
            mission = (uint32)r_s16(game + 908);
            aircraft = (uint32)r_s16(game + 910);
            value = resource002_sub_800fead4(mission, aircraft, 1);
            owner = r_u32(0x80112E10);
            w_u8(owner + 23, value);
            w_u8(owner + 21, value);
            game = r_u32(0x800BCE60);
            mission = (uint32)r_s16(game + 908);
            aircraft = (uint32)r_s16(game + 910);
            value = resource002_sub_800fe9c0(mission, aircraft, 0);
            owner = r_u32(0x80112E10);
            w_u8(owner + 282, value);
            game = r_u32(0x800BCE60);
            mission = (uint32)r_s16(game + 908);
            aircraft = (uint32)r_s16(game + 910);
            value = resource002_sub_800fe9c0(mission, aircraft, 1);
            owner = r_u32(0x80112E10);
            w_u8(owner + 283, value);
        }
        owner = r_u32(0x80112E10);
        if (r_s16(owner + 2) == 2)
        {
            resource_init_queue(0, 0);
            game = r_u32(0x800BCE60);
            mission = (uint32)r_s16(game + 908);
            value = (uint32)r_s16(0x801127AC + mission * 12);
            if (value == 0xFFFFFFFF || (r_u32(game + 2412) & 16))
            {
                state = r_u32(0x80112DC4);
                w_u8(state + 124, 0);
                state = r_u32(0x80112DC4);
                w_u8(state + 125, 0);
                state = r_u32(0x80112DC4);
                w_u8(state + 126, 0);
                resource002_sub_800debb8(0, 1);
                game = r_u32(0x800BCE60);
                if (r_u32(game + 2412) & 16)
                    w_u16(game + 908, 59);
            }
            else
            {
                resource002_sub_800debb8(0, 5);
                resource002_sub_800debb8(0, 6);
                resource002_sub_800deb5c(0, 7, 0, 0, 496, 0);
                resource002_sub_800dedf0(0, 225);
                sub_8002db4c(90);
                state = r_u32(0x80112DC4);
                w_u16(state + 132, 1);
            }
            for (sound = 93; sound <= 97; ++sound)
            {
                value = sub_8002db4c(sound);
                state = r_u32(0x80112DC4);
                w_u32(state + 304 + (sound - 93) * 4, value);
            }
            w_u16(state + 134, 0);
            resource002_sub_800dee08(1, 1, 0, 40);
            owner = r_u32(0x80112E10);
            w_u32(owner + 16, 0);
            arena_set_cursor(owner);
            scene = r_u32(0x800BCE54);
            w_u32(scene + 36, 0);
            w_u32(scene + 32, 0);
        }
    }
    result = r_u32(r_u32(0x800BCE60) + 2412) & 16;
    if (result)
        return resource002_sub_800f9cf0();
    return result;
}

uint32 resource002_sub_800f9ed0(uint32 game)
{
    uint32 state, owner, value, other, choice, slot, phase, result, changed = 0, ready;
    FUNCTION_MARKER(0x800F9ED0, "ACE3.BIN");
    if (r_u32(game + 2412) & 16)
    {
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) != 3)
        {
            value = r_u32(game + 304);
            other = r_u32(game + 300);
            value &= 0xFFFFFFC3;
            other &= 0xFFFFFFC3;
            w_u32(game + 304, value);
            w_u32(game + 300, other);
        }
    }
    resource002_sub_800def94(1, 0);
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
    {
        if (r_s16(game + 908) == 42)
        {
            resource002_sub_800def94(3, 2);
            resource002_sub_800def94(2, 2);
        }
        else
        {
            resource002_sub_800def94(2, (r_u32(game + 296) & 4) != 0);
            game = r_u32(0x800BCE60);
            resource002_sub_800def94(3, (r_u32(game + 296) & 8) != 0);
        }
    }
    else
    {
        state = r_u32(0x80112DC4);
        choice = r_u32(state + 192) ? (r_u32(game + 296) & 4) != 0 : 2;
        resource002_sub_800def94(2, choice);
        state = r_u32(0x80112DC4);
        value = r_u32(state + 228);
        other = r_u32(state + 192);
        if (other == value - 1)
            choice = 2;
        else
        {
            game = r_u32(0x800BCE60);
            choice = (r_u32(game + 296) & 8) != 0;
        }
        resource002_sub_800def94(3, choice);
    }
    state = r_u32(0x80112DC4);
    owner = r_u32(state + 364);
    resource002_sub_800def94(4, r_s16(owner + 106) == 2);
    game = r_u32(0x800BCE60);
    if (r_s16(game + 908) != 42)
    {
        value = r_u32(game + 304);
        if (value & 8)
        {
            if (r_u32(game + 2412) & 16)
            {
                state = r_u32(0x80112DC4);
                value = r_u32(state + 192);
                resource002_sub_800fe6e0(0, r_s16(state + value * 2 + 480));
                state = r_u32(0x80112DC4);
                value = (uint32)r_s16(state + 610);
                other = r_u32(state + 192);
                w_u32(state + 192, (sint32)other < (sint32)(value - 1) ? other + 1 : 0);
                state = r_u32(0x80112DC4);
                w_u32(state + 196, 0);
                changed = 1;
            }
            else
            {
                state = r_u32(0x80112DC4);
                value = r_u32(state + 228);
                other = r_u32(state + 192);
                if ((sint32)other < (sint32)(value - 1))
                {
                    w_u32(state + 192, other + 1);
                    changed = 1;
                }
            }
        }
        else if (value & 4)
        {
            if (r_u32(game + 2412) & 16)
            {
                state = r_u32(0x80112DC4);
                value = r_u32(state + 192);
                resource002_sub_800fe6e0(0, r_s16(state + value * 2 + 480));
                state = r_u32(0x80112DC4);
                value = r_u32(state + 192);
                if ((sint32)value > 0)
                    --value;
                else
                    value = (uint32)r_s16(state + 610) - 1;
                w_u32(state + 192, value);
                state = r_u32(0x80112DC4);
                w_u32(state + 196, 0);
                changed = 1;
            }
            else
            {
                state = r_u32(0x80112DC4);
                value = r_u32(state + 192);
                if ((sint32)value > 0)
                {
                    w_u32(state + 192, value - 1);
                    changed = 1;
                }
            }
        }
        if (changed)
        {
            sub_8002db4c(61);
            resource002_sub_800dee08(2, 3, -1, -1);
            resource002_sub_800dee08(8, 3, -1, -1);
        }
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
        resource002_sub_800f9cf0();
    else
        resource002_sub_800f7994(1);
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
    {
        if ((r_u32(game + 300) & 16) && (r_u32(game + 296) & 0x2F) == 0)
        {
            sub_8002db4c(91);
            sub_8002db4c(60);
            owner = r_u32(0x80112E10);
            w_u16(owner + 2, 1);
            w_u16(owner, 2);
        }
    }
    else
    {
        state = r_u32(0x80112DC4);
        ready = r_s16(state + 454) == 3;
        if ((r_u32(game + 300) & 16) && (r_u32(game + 296) & 0x2F) == 0)
        {
            phase = resource002_sub_800dee84(2);
            if (phase == 2 && ready)
            {
                resource002_sub_800dee08(5, 3, -1, -1);
                resource002_sub_800dee08(6, 3, -1, -1);
                resource002_sub_800dee08(3, 3, -1, -1);
                resource002_sub_800dee08(8, 3, -1, -1);
                sub_8002db4c(91);
                sub_8002db4c(60);
                owner = r_u32(0x80112E10);
                w_u16(owner + 2, 1);
                w_u16(owner, phase);
            }
        }
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            static const uint8 slots[] = {2, 1, 3, 11, 5, 6, 7, 8};
            for (slot = 0; slot < 8; ++slot)
                resource002_sub_800dee08(slots[slot], 3, -1, -1);
            sub_8002db4c(91);
            sub_8002db4c(59);
            resource002_sub_80100994();
            owner = r_u32(0x80112E10);
            w_u16(owner + 2, 2);
            w_u16(owner, 2);
            return 2;
        }
    }
    return result;
}

uint32 resource002_sub_800f9b20(uint32 game, uint32 owner)
{
    uint32 mission, aircraft, record, value, target, choice;
    FUNCTION_MARKER(0x800F9B20, "ACE3.BIN");
    mission = (uint32)r_s16(game + 908);
    aircraft = (uint32)r_s16(game + 910);
    record = r_u8(owner + 20);
    value = resource002_sub_800febb8(mission, aircraft, 0, record);
    if (value < 6)
    {
        target = r_u32(0x800CD3A4 + value * 4);
        switch (target)
        {
            case 0x800F9B68:
                choice = 1;
                break;
            case 0x800F9B78:
                choice = 2;
                break;
            case 0x800F9B88:
                choice = 3;
                break;
            case 0x800F9B98:
                choice = 4;
                break;
            case 0x800F9BA8:
                choice = 5;
                break;
            case 0x800F9BB8:
                choice = 6;
                break;
            default:
#ifndef NDEBUG
                fprintf(stderr, "Unbound resource2 aircraft publication branch %08X\n", target);
#endif
                abort();
        }
        game = r_u32(0x800BCE60);
        w_u8(game + 937, choice);
    }
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    mission = (uint32)r_s16(game + 908);
    aircraft = (uint32)r_s16(game + 910);
    record = r_u8(owner + 21);
    value = resource002_sub_800febb8(mission, aircraft, 1, record);
    if (value >= 9)
        return 0x800D0000;
    target = r_u32(0x800CD3BC + value * 4);
    switch (target)
    {
        case 0x800F9C18:
            game = r_u32(0x800BCE60);
            w_u8(game + 938, 0);
            game = r_u32(0x800BCE60);
            w_u8(game + 939, 0);
            return game;
        case 0x800F9C54:
        case 0x800F9CA4:
            choice = target == 0x800F9C54 ? 3 : 5;
            game = r_u32(0x800BCE60);
            w_u8(game + 938, 1);
            game = r_u32(0x800BCE60);
            w_u8(game + 939, choice);
            return choice;
        case 0x800F9C34:
            choice = 1;
            break;
        case 0x800F9C44:
            choice = 2;
            break;
        case 0x800F9C74:
            choice = 4;
            break;
        case 0x800F9C84:
            choice = 10;
            break;
        case 0x800F9C94:
            choice = 7;
            break;
        case 0x800F9CC4:
            choice = 6;
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 aircraft variant branch %08X\n", target);
#endif
            abort();
    }
    game = r_u32(0x800BCE60);
    w_u8(game + 938, choice);
    game = r_u32(0x800BCE60);
    w_u8(game + 939, choice);
    return game;
}

uint32 resource002_sub_800f9b10(void)
{
    uint32 game, owner;
    FUNCTION_MARKER(0x800F9B10, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    return resource002_sub_800f9b20(game, owner);
}

uint32 resource002_sub_800f3f2c(uint32 character)
{
    uint32 state, glyph, divisor, quotient, remainder, width, height, u, v, packet, value, tag, ordering, x, y, table;
    FUNCTION_MARKER(0x800F3F2C, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    glyph = (character - r_u8(state + 0x2902)) & 255;
    divisor = (uint32)r_s16(state + 0x28F0);
    if (divisor == 0)
        xport_mips_break(7);
    quotient = (uint32)((sint32)glyph / (sint32)divisor);
    remainder = (uint32)((sint32)glyph % (sint32)divisor);
    width = r_u8(state + 0x2900);
    height = r_u8(state + 0x2901);
    u = r_u16(state + 0x28F8);
    v = r_u16(state + 0x28FA);
    packet = r_u32(state + 0xCA4);
    w_u8(packet + 12, u + remainder * width);
    w_u8(packet + 13, v + quotient * height);
    state = r_u32(0x80112E00) + 0x8000;
    height = r_u8(state + 0x2901);
    width = r_u8(state + 0x2900);
    w_u32(packet + 16, width | (height << 16));
    y = (uint32)r_s16(state + 0x28E6);
    x = r_u16(state + 0x28E4);
    w_u32(packet + 8, x | (y << 16));
    w_u16(packet + 14, r_u16(state + 0x28EE));
    w_u8(packet + 4, r_u8(state + 0x28CC));
    state = r_u32(0x80112E00) + 0x8000;
    w_u8(packet + 5, r_u8(state + 0x28CD));
    state = r_u32(0x80112E00) + 0x8000;
    w_u8(packet + 6, r_u8(state + 0x28CE));
    state = r_u32(0x80112E00) + 0x8000;
    value = r_u8(state + 0x2908);
    tag = r_u8(packet + 7);
    w_u8(packet + 7, value ? tag | 2 : tag & 0xFD);
    state = r_u32(0x80112E00) + 0x8000;
    ordering = r_u32(state + 0x28DC);
    value = r_u32(ordering);
    w_u32(packet, value | 0x04000000);
    ordering = r_u32(state + 0x28DC);
    w_u32(ordering, packet & 0xFFFFFF);
    state = r_u32(0x80112E00) + 0x8000;
    if (r_u8(state + 0x2903))
    {
        table = r_u32(state + 0x2904);
        value = r_u8(table + glyph);
        x = r_u16(state + 0x28E4);
        w_u16(state + 0x28E4, x + value);
    }
    else
    {
        value = r_u8(state + 0x2900);
        x = r_u16(state + 0x28E4);
        w_u16(state + 0x28E4, x + value + 1);
    }
    state = r_u32(0x80112E00) + 0x8000;
    value = r_u32(state + 0xCA4) + 20;
    w_u32(state + 0xCA4, value);
    return value;
}

uint32 resource002_sub_800f46e4(uint32 text)
{
    uint32 state, packet, ordering, tag, link, value, index = 0, character, step, reset, current;
    sint32 count, scale;
    FUNCTION_MARKER(0x800F46E4, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    packet = r_u32(state + 0x28C8);
    SetDrawMode(psx_addr(packet, 12), 0, 0, 0, NULL);
    state = r_u32(0x80112E00) + 0x8000;
    packet = r_u32(state + 0x28C8);
    ordering = r_u32(state + 0x28DC);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 0x28DC);
    packet = r_u32(state + 0x28C8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    packet = r_u32(state + 0x28C8);
    count = r_s32(state + 0x28D0);
    w_u32(state + 0x28C8, packet + 12);
    if (count > 0)
        do
        {
            character = r_u8(text + index);
            if (character == 10)
            {
                state = r_u32(0x80112E00) + 0x8000;
                step = r_u8(state + 0x2901);
                scale = r_s16(state + 0x28F2);
                reset = r_u16(state + 0x28E8);
                current = r_u16(state + 0x28E6);
                w_u16(state + 0x28E4, reset);
                w_u16(state + 0x28E6, current + step * (uint32)scale);
            }
            else if (character == 9)
            {
                state = r_u32(0x80112E00) + 0x8000;
                w_u16(state + 0x28E4, r_u16(state + 0x28E8));
            }
            else if (character == 32)
            {
                state = r_u32(0x80112E00) + 0x8000;
                if (r_u8(state + 0x2903))
                {
                    step = r_u8(state + 0x290A);
                    current = r_u16(state + 0x28E4);
                    w_u16(state + 0x28E4, current + step);
                }
                else
                {
                    step = r_u8(state + 0x2900);
                    current = r_u16(state + 0x28E4);
                    w_u16(state + 0x28E4, current + step + 1);
                }
            }
            else
                resource002_sub_800f3f2c(r_u8(text + index));
            state = r_u32(0x80112E00) + 0x8000;
            count = r_s32(state + 0x28D0);
            ++index;
        } while ((sint32)index < count);
    state = r_u32(0x80112E00) + 0x8000;
    value = (uint32)r_s16(state + 0x28EC);
    packet = r_u32(state + 0x28C8);
    SetDrawMode(psx_addr(packet, 12), 0, 0, value, NULL);
    state = r_u32(0x80112E00) + 0x8000;
    packet = r_u32(state + 0x28C8);
    ordering = r_u32(state + 0x28DC);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 0x28DC);
    packet = r_u32(state + 0x28C8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    packet = r_u32(state + 0x28C8) + 12;
    w_u32(state + 0x28C8, packet);
    return packet;
}

uint32 resource002_sub_800f4a78(uint32 text)
{
    uint32 owner, count, state;
    FUNCTION_MARKER(0x800F4A78, "ACE3.BIN");
    owner = r_u32(0x80112E00) + 0x8000;
    count = text ? (uint32)strlen(game_native_string(text)) : 0;
    state = r_u32(0x80112E00) + 0x8000;
    w_u32(owner + 0x28D0, count);
    (void)r_s16(state + 0x28E4);
    (void)r_s16(state + 0x28E6);
    return resource002_sub_800f46e4(text);
}

uint32 resource002_sub_800f9cf0(void)
{
    uint32 state, packet, ordering, tag, link, index, table, text;
    sint32 aircraft;
    FUNCTION_MARKER(0x800F9CF0, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    packet = r_u32(state + 628);
    w_u16(packet + 12, 132);
    w_u16(packet + 14, 318);
    w_u16(packet + 10, 29);
    w_u8(packet + 3, 3);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 0);
    w_u8(packet + 6, 0);
    w_u8(packet + 7, 96);
    state = r_u32(0x80112DC4);
    w_u16(packet + 8, 0);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 8);
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    index = r_u32(state + 192);
    w_u32(state + 628, packet + 16);
    aircraft = r_s16(state + index * 2 + 480);
    if (aircraft <= 0)
        aircraft = 1;
    if (aircraft >= 37)
        aircraft = 36;
    resource002_sub_800f35b8(7);
    resource002_sub_800f33d0(0, 128, 0);
    resource002_sub_800f33a8(12, 64);
    resource002_sub_800f4a78(0x800CD66C);
    resource002_sub_800f33a8(12, 96);
    table = 0x8010EB3C + (uint32)aircraft * 12;
    text = r_u32(table);
    resource002_sub_800f4a78(text);
    resource002_sub_800f33a8(12, 112);
    text = r_u32(table + 4);
    resource002_sub_800f4a78(text);
    resource002_sub_800f33a8(12, 144);
    return resource002_sub_800f4a78(r_u32(table + 8));
}

uint32 resource002_sub_800f9e68(void)
{
    uint32 owner, result;
    FUNCTION_MARKER(0x800F9E68, "ACE3.BIN");
    owner = r_u32(0x80112E10);
    w_u16(owner + 2, 0);
    w_u16(owner, 1);
    resource002_sub_800def94(4, 0);
    result = r_u32(r_u32(0x800BCE60) + 2412) & 16;
    if (result)
        return resource002_sub_800f9cf0();
    return result;
}

uint32 resource002_sub_800fa860(void)
{
    uint32 owner, result;
    FUNCTION_MARKER(0x800FA860, "ACE3.BIN");
    resource002_sub_800def94(1, 1);
    resource002_sub_800def94(4, 0);
    if (resource002_sub_800dee84(8) == 2)
    {
        owner = r_u32(0x80112E10);
        w_u16(owner, 4);
        w_u16(owner + 2, 0);
    }
    result = r_u32(r_u32(0x800BCE60) + 2412) & 16;
    if (result)
        return resource002_sub_800f9cf0();
    return result;
}

uint32 resource002_sub_800fa8e0(void)
{
    uint32 game, owner, result;
    FUNCTION_MARKER(0x800FA8E0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
        resource002_sub_800f9cf0();
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && (r_u32(game + 296) & 0x2F) == 0)
    {
        resource002_sub_800dee08(8, 3, -1, -1);
        resource002_sub_800dee08(2, 3, -1, -1);
        resource002_sub_800dee08(8, 3, -1, -1);
        resource002_sub_800dee08(1, 3, -1, -1);
        resource002_sub_800dee08(7, 3, -1, -1);
        sub_8002db4c(91);
        sub_8002db4c(60);
        owner = r_u32(0x80112E10);
        w_u16(owner, 5);
        w_u16(owner + 2, 1);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            resource002_sub_800dee08(8, 3, -1, -1);
            sub_8002db4c(91);
            sub_8002db4c(59);
            owner = r_u32(0x80112E10);
            w_u16(owner, 5);
            w_u16(owner + 2, 2);
            return 2;
        }
    }
    return result;
}

uint32 resource002_sub_800faa50(uint32 incoming_owner)
{
    uint32 owner, state, game, value, result, slot;
    FUNCTION_MARKER(0x800FAA50, "ACE3.BIN");
    resource002_sub_800def94(4, r_s16(incoming_owner + 2) != 2);
    if (resource002_sub_800dee84(8))
    {
        value = resource002_sub_800deea8(8);
        resource002_sub_800f7c24(value);
    }
    else
    {
        owner = r_u32(0x80112E10);
        if (r_s16(owner + 2) == 1)
        {
            state = r_u32(0x80112DC4);
            w_u32(owner + 16, 0);
            w_u16(state + 132, 3);
            w_u16(state + 134, 0);
            resource002_sub_800f9b10();
            owner = r_u32(0x80112E10);
            arena_set_cursor(owner);
            resource002_sub_800f7524(3, 0);
            sub_8002db4c(90);
        }
        owner = r_u32(0x80112E10);
        if (r_s16(owner + 2) == 2)
        {
            game = r_u32(0x800BCE60);
            slot = 3;
            if ((r_u32(game + 2412) & 16) == 0)
            {
                resource002_sub_800dee08(3, 1, 0, 25);
                resource002_sub_800dee08(5, 1, 0, 25);
                resource002_sub_800dee08(6, 1, 0, 25);
                slot = 8;
            }
            resource002_sub_800dee08(slot, 1, 0, 25);
            resource002_sub_800dedf0(0, 0x7701);
            sub_8002db4c(90);
            owner = r_u32(0x80112E10);
            value = r_u32(owner + 16);
            w_u16(owner, 0);
            w_u32(owner + 16, value | 2);
        }
    }
    result = r_u32(r_u32(0x800BCE60) + 2412) & 16;
    if (result)
        return resource002_sub_800f9cf0();
    return result;
}

uint32 resource002_sub_800faa48(void)
{
    FUNCTION_MARKER(0x800FAA48, "ACE3.BIN");
    return resource002_sub_800faa50(r_u32(0x80112E10));
}

uint32 resource002_sub_800f9ec8(void)
{
    FUNCTION_MARKER(0x800F9EC8, "ACE3.BIN");
    return resource002_sub_800f9ed0(r_u32(0x800BCE60));
}

void resource002_sub_80107518(uint32 object, const uint8 *native_entries, uint8 *native_meshes, uint8 *native_animated, uint8 *native_fixed, uint8 *native_sprites, uint32 *native_output)
{
    MATRIX matrix;
    uint16 angles[3];
    uint8 *output;
    uint32 scene, camera, focal, range, half_width, half_height, position, x, y, z, distance, selection, smooth, level, model, vertices, table, source;
    uint32 a, b, c, d, value, radius, depth, screen, screen_x, screen_y, margin, count, offset, index, column, kind;
    FUNCTION_MARKER(0x80107518, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    focal = (uint32)r_s16(camera + 36);
    range = focal * 12;
    a = r_u16(camera + 32);
    b = r_u16(camera + 34);
    half_width = (uint32)((sint32)(a << 16) >> 17);
    half_height = (uint32)((sint32)(b << 16) >> 17);
    while (native_entries[0] != 255)
    {
        position = xport_load_le32(native_entries + 8);
        a = r_u32(position);
        b = r_u32(camera + 4);
        c = r_u32(camera + 8);
        x = (uint32)((sint32)(a - b) >> 10);
        a = r_u32(position + 4);
        b = r_u32(position + 8);
        y = (uint32)((sint32)(a - c) >> 10);
        c = r_u32(camera + 12);
        z = (uint32)((sint32)(b - c) >> 10);
        a = (sint32)x < 0 ? 0 - x : x;
        b = (sint32)y < 0 ? 0 - y : y;
        c = (sint32)z < 0 ? 0 - z : z;
        distance = a + b + c;
        if ((sint32)distance > 0x2C000 || (native_entries[4] & 15) == 0)
            goto next_aircraft_geometry;
        selection = (uint32)native_entries[0] * 4;
        smooth = r_u32(0x800BD090);
        if ((sint32)(range << 2) < (sint32)distance)
            selection += 2;
        else if ((sint32)(range << 1) < (sint32)distance)
            ++selection;
        table = r_u32(object + 12);
        model = r_u32(table + selection * 4);
        if ((sint32)range < (sint32)distance)
        {
            level = 0;
            vertices = r_u32(model + 4);
        }
        else if (((sint32)range >> 1) < (sint32)distance)
        {
            level = 1;
            vertices = r_u32(model + 8);
        }
        else
        {
            level = 2;
            vertices = r_u32(model + 12);
        }
        x = (uint32)((sint32)x >> (2 - level));
        y = (uint32)((sint32)y >> (2 - level));
        z = (uint32)((sint32)z >> (2 - level));
        SetRotMatrix((MATRIX *)psx_addr(scene + 76, 20));
        xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(1, z);
        xport_gte_execute(0x180001);
        kind = native_entries[4];
        if (kind & 9)
        {
            native_meshes[0] = native_entries[1];
            value = xport_load_le16(native_entries + 2);
            xport_store_le16(native_meshes + 2, value);
            table = r_u32(object + 12);
            xport_store_le32(native_meshes + 8, level);
            xport_store_le32(native_meshes + 16, model);
            xport_store_le32(native_meshes + 12, vertices);
            xport_store_le32(native_meshes + 52, 0);
            xport_store_le32(native_meshes + 4, table + selection * 4);
            a = xport_gte_read_data(25);
            xport_store_le32(native_meshes + 40, a);
            b = xport_gte_read_data(26);
            xport_store_le32(native_meshes + 44, b);
            c = xport_gte_read_data(27);
            xport_store_le32(native_meshes + 48, c);
        }
        else
        {
            xport_store_le32(native_fixed + 152, 0);
            output = (kind & 2) ? native_animated : native_fixed;
            table = r_u32(object + 12);
            source = r_u32(table + 12);
            for (index = 0; index < 2; ++index)
            {
                a = r_u32(source + index * 4);
                value = r_u32(a + (level + 1) * 4);
                xport_store_le32(output + 4 + index * 4, value);
                xport_store_le32(output + 24 + index * 4, a);
            }
            value = r_u32(source + 8);
            xport_store_le32(output + 32, value);
            value = r_u32(source + 28);
            xport_store_le32(output + 36, value);
            value = r_u32(source + 36);
            xport_store_le32(output + 40, value);
            if (kind & 2)
            {
                for (index = 0; index < 10; ++index)
                {
                    a = r_u32(source + 8 + index * 4);
                    value = r_u32(a + (level + 1) * 4);
                    xport_store_le32(output + 44 + index * 4, value);
                    if (index == 0)
                        xport_store_le32(output + 12, value);
                    if (index == 5)
                        xport_store_le32(output + 16, value);
                    if (index == 7)
                        xport_store_le32(output + 20, value);
                }
            }
            else
            {
                a = r_u32(source + 8);
                b = r_u32(source + 28);
                c = r_u32(source + 36);
                a = r_u32(a + (level + 1) * 4);
                b = r_u32(b + (level + 1) * 4);
                c = r_u32(c + (level + 1) * 4);
                xport_store_le32(output + 20, c);
                xport_store_le32(output + 12, a);
                xport_store_le32(output + 16, b);
            }
            xport_store_le32(output, level);
            a = xport_gte_read_data(25);
            xport_store_le32(output + 108, a);
            b = xport_gte_read_data(26);
            xport_store_le32(output + 112, b);
            c = xport_gte_read_data(27);
            xport_store_le32(output + 116, c);
        }
        depth = (uint32)((sint32)xport_gte_read_data(27) >> level);
        value = (uint32)r_s16(model + 2);
        radius = (uint32)((sint32)value >> (2 - level));
        if ((sint32)depth < (sint32)(0 - radius))
            goto next_aircraft_geometry;
        screen = xport_gte_read_data(14);
        kind = native_entries[4];
        if (kind & 2)
            xport_store_le32(native_animated + 84, depth);
        else if (kind & 4)
            xport_store_le32(native_fixed + 84, depth);
        if ((sint32)depth <= 0)
            depth = 1;
        margin = (uint32)((sint32)(radius * focal) / (sint32)depth);
        screen_x = (uint32)(sint32)(sint16)screen;
        screen_y = (uint32)((sint32)screen >> 16);
        if ((sint32)(radius << 4) < (sint32)margin)
            margin = radius << 2;
        value = (sint32)screen_x < 0 ? 0 - screen_x : screen_x;
        if ((sint32)(half_width + margin) < (sint32)value)
            goto next_aircraft_geometry;
        value = (sint32)screen_y < 0 ? 0 - screen_y : screen_y;
        if ((sint32)(half_height + margin) < (sint32)value)
            goto next_aircraft_geometry;
        position = xport_load_le32(native_entries + 8);
        rotation_build(position + 16, 0, &matrix, NULL);
        value = xport_load_le32(native_entries + 12);
        if (value)
        {
            count = xport_load_le32(native_output);
            offset = count * 8;
            xport_store_le32((uint8 *)native_output + offset + 4, value);
            xport_store_le16((uint8 *)native_output + offset + 8, screen_x);
            xport_store_le16((uint8 *)native_output + offset + 10, screen_y);
            xport_store_le32(native_output, count + 1);
        }
        kind = native_entries[4];
        if (kind & 9)
            output = native_meshes + 20;
        else if (kind & 2)
        {
            if (native_entries[5])
            {
                a = (uint32)r_s16(camera + 180);
                b = (uint32)r_s16(camera + 182);
                value = r_u16(camera + 180);
                if ((sint32)a < 0)
                    a = 0 - a;
                if ((sint32)b < 0)
                    b = 0 - b;
                c = (a + b) << 2;
                if ((sint32)c >= 4097)
                    c = 4096;
                c = 4096 - c;
                a = r_u16(smooth + 2);
                a = (uint32)((sint32)(a << 16) >> 20);
                a = (uint32)((sint32)(a * c) / 4096);
                b = r_u16(smooth + 4);
                b = (uint32)((sint32)(b << 16) >> 21);
                b = (uint32)((sint32)(b * c) / 4096);
                d = r_u16(smooth);
                d = (uint32)((sint32)(d << 16) >> 19);
                d = (uint32)((sint32)(d * c) / 4096);
                angles[0] = value - a;
                value = r_u16(camera + 182);
                angles[1] = value - b;
                value = r_u16(camera + 184);
                angles[2] = value - d;
                sub_800308f0(0, 0, (MATRIX *)(native_animated + 88), angles);
                value = (uint32)r_s16(camera + 44);
                xport_store_le32(native_animated + 108, (sint32)(0 - value) >> (6 - level));
                value = (uint32)r_s16(camera + 46);
                xport_store_le32(native_animated + 112, (sint32)(0 - value) >> (6 - level));
                value = (uint32)r_s16(camera + 48);
                xport_store_le32(native_animated + 116, (sint32)(0 - value) >> (6 - level));
                goto aircraft_light_columns;
            }
            output = native_animated + 88;
        }
        else
            output = native_fixed + 88;
        for (column = 0; column < 3; ++column)
        {
            a = xport_load_le16((uint8 *)&matrix + column * 2);
            b = xport_load_le16((uint8 *)&matrix + column * 2 + 6);
            c = xport_load_le16((uint8 *)&matrix + column * 2 + 12);
            xport_gte_write_data(9, a);
            xport_gte_write_data(10, b);
            xport_gte_write_data(11, c);
            xport_gte_execute(0x49E012);
            a = xport_gte_read_data(9);
            b = xport_gte_read_data(10);
            c = xport_gte_read_data(11);
            xport_store_le16(output + column * 2, a);
            xport_store_le16(output + column * 2 + 6, b);
            xport_store_le16(output + column * 2 + 12, c);
        }
        if (kind & 9)
        {
            native_meshes += 56;
            goto next_aircraft_geometry;
        }
    aircraft_light_columns:
        SetRotMatrix((MATRIX *)psx_addr(scene + 108, 20));
        output = (kind & 2) ? native_animated + 120 : native_fixed + 120;
        for (column = 0; column < 3; ++column)
        {
            a = xport_load_le16((uint8 *)&matrix + column * 2);
            b = xport_load_le16((uint8 *)&matrix + column * 2 + 6);
            c = xport_load_le16((uint8 *)&matrix + column * 2 + 12);
            xport_gte_write_data(9, a);
            xport_gte_write_data(10, b);
            xport_gte_write_data(11, c);
            xport_gte_execute(0x49E012);
            a = xport_gte_read_data(9);
            b = xport_gte_read_data(10);
            c = xport_gte_read_data(11);
            xport_store_le16(output + column * 2, a);
            xport_store_le16(output + column * 2 + 6, b);
            xport_store_le16(output + column * 2 + 12, c);
        }
        if (kind & 2)
            native_animated += 160;
        else
            native_fixed += 160;
    next_aircraft_geometry:
        native_entries += 16;
    }
    xport_store_le32(native_meshes + 8, 0xFFFFFFFF);
    xport_store_le32(native_animated, 0xFFFFFFFF);
    xport_store_le32(native_fixed, 0xFFFFFFFF);
    xport_store_le32(native_sprites + 16, 0);
}

uint32 resource002_sub_801073b0(uint8 *native_entries)
{
    uint32 scene, camera, excluded, selected, index, table, object, flags, kind, value, offset = 0;
    sint32 type;
    FUNCTION_MARKER(0x801073B0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    excluded = r_u32(camera + 196);
    selected = r_u32(camera + 200);
    for (index = 0; index < 32; ++index)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        flags = r_u32(object);
        if ((flags & 0x14000) != 0x14000 || excluded == index)
            continue;
        flags = r_u8(object + 5);
        if (flags & 1)
        {
            kind = 4;
            if (selected == index)
            {
                scene = r_u32(0x800BCE54);
                camera = r_u32(scene + 40);
                if (r_u32(camera + 80) & 6)
                    kind = 2;
            }
        }
        else
            kind = 1;
        native_entries[offset + 4] = kind;
        native_entries[offset] = r_u8(object + 4);
        native_entries[offset + 1] = r_u8(object + 5);
        value = r_u16(object + 6);
        xport_store_le32(native_entries + offset + 8, object + 8);
        xport_store_le16(native_entries + offset + 2, value);
        xport_store_le32(native_entries + offset + 12, r_u32(object));
        type = r_s16(object + 72);
        value = type == 43 ? 3 : type == 35 ? 4 : type == 36 ? 5 : 0;
        xport_store_le16(native_entries + offset + 6, value);
        native_entries[offset + 5] = selected == index;
        offset += 16;
    }
    native_entries[offset] = 255;
    return offset;
}

uint32 resource002_sub_8010817c(uint32 object, uint32 ordering, uint32 packet, uint32 *native_output)
{
    uint8 geometry[5888];
    uint8 *meshes = geometry + 1024;
    uint8 *animated = geometry + 4608;
    uint8 *fixed = geometry + 4928;
    uint8 *sprites = geometry + 5248;
    uint32 scene, value;
    FUNCTION_MARKER(0x8010817C, "ACE3.BIN");
    xport_store_le32(native_output, 0);
    resource002_sub_801073b0(geometry);
    resource002_sub_80107518(object, geometry, meshes, animated, fixed, sprites, native_output);
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, packet);
    sub_80066c80(0, 0x1F800000, sprites);
    sub_80066dcc(0, 0x1F800000, meshes);
    sub_80067088(0, 0x1F800000, animated);
    scene = r_u32(0x800BCE54);
    SetRotMatrix((MATRIX *)psx_addr(scene + 76, 20));
    sub_800626e0(0, 0x1F800000, sprites);
    sub_80062bd4(0, 0x1F800000, meshes);
    scene = r_u32(0x800BCE54);
    value = r_u16(scene + 12);
    w_u16(0x1F8003F4, value);
    sub_800637b0(0, 0x1F800000, animated);
    sub_800637b0(0, 0x1F800000, fixed);
    return r_u32(0x1F800004);
}

uint32 resource002_sub_800fe668(uint32 mission, uint32 aircraft)
{
    uint32 group, target;
    FUNCTION_MARKER(0x800FE668, "ACE3.BIN");
    group = resource002_sub_800fe6e0(mission, (sint32)aircraft);
    if (group >= 5)
        return aircraft;
    target = r_u32(0x800CD6BC + group * 4);
    switch (target)
    {
        case 0x800FE6B0:
            return aircraft - 10;
        case 0x800FE6A8:
            return aircraft - 1;
        case 0x800FE6B8:
            return aircraft - 18;
        case 0x800FE6C0:
            return aircraft - 26;
        case 0x800FE6C8:
            return aircraft - 31;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 aircraft group branch %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800fe86c(uint32 game, uint32 mission, uint32 variant)
{
    uint32 aircraft;
    FUNCTION_MARKER(0x800FE86C, "ACE3.BIN");
    if (r_u32(game + 2412) & 16)
        return 0;
    if (mission >= 60)
        mission = 0;
    if (variant >= 7)
        variant = 0;
    aircraft = r_u8(0x80111784 + mission * 12 + variant + 5);
    return resource002_sub_800fe668(mission, aircraft);
}

uint32 resource002_sub_800fe864(uint32 mission, uint32 variant)
{
    FUNCTION_MARKER(0x800FE864, "ACE3.BIN");
    return resource002_sub_800fe86c(r_u32(0x800BCE60), mission, variant);
}

uint32 resource002_sub_800ed0d4(uint32 resource)
{
    uint32 state, pixels, index;
    FUNCTION_MARKER(0x800ED0D4, "ACE3.BIN");
    resource002_sub_800ece38(resource, 0x10000);
    state = r_u32(0x80112DC4);
    pixels = r_u32(state + 208);
    sub_800a84a0(state + 216, pixels, NULL, NULL, 0);
    DrawSync(0);
    for (index = 0; index < 64; ++index)
    {
        state = r_u32(0x80112DC4);
        pixels = r_u32(state + 212);
        w_u32(pixels + index * 4, 0xCE73CE73);
    }
    state = r_u32(0x80112DC4);
    pixels = r_u32(state + 212);
    return sub_800a8440(state + 216, pixels, NULL, NULL, 0);
}

uint32 resource002_sub_800f7994(uint32 mode)
{
    uint32 state, owner, source, a, b, c, d, phase, target, game, index, resource, entries, scene, model, table, object, value;
    FUNCTION_MARKER(0x800F7994, "ACE3.BIN");
    if (resource002_sub_800dee84(2) && mode == 1)
        return 0;
    state = r_u32(0x80112DC4);
    phase = (uint32)r_s16(state + 454);
    if (phase != 3)
    {
        if (phase == 0)
        {
            owner = r_u32(0x80112E10);
            source = r_u32(owner + 8);
            a = r_u32(source);
            b = r_u32(source + 4);
            c = r_u32(source + 8);
            d = r_u32(source + 12);
            w_u32(owner + 292, a);
            w_u32(owner + 296, b);
            w_u32(owner + 300, c);
            w_u32(owner + 304, d);
            a = r_u32(source + 16);
            b = r_u32(source + 20);
            w_u32(owner + 308, a);
            w_u32(owner + 312, b);
            owner = r_u32(0x80112E10);
            w_u32(owner + 288, 1);
        }
        owner = r_u32(0x80112E10);
        w_u32(owner + 8, owner + 292);
        state = r_u32(0x80112DC4);
        phase = (uint32)r_s16(state + 454);
        target = r_u32(0x8010984C + phase * 4);
        resource002_loading_phase_call(target, 1, 0);
        return 0;
    }
    a = r_u32(state + 192);
    game = r_u32(0x800BCE60);
    b = r_u32(state + 128);
    c = (uint32)r_s16(game + 908);
    index = resource002_sub_800fe864(c, a);
    state = r_u32(0x80112DC4);
    table = r_u32(state + 244);
    resource = r_u32(table + index * 4);
    entries = sub_80027338(resource, NULL);
    state = r_u32(0x80112DC4);
    resource = r_u32(entries);
    a = r_u32(state + 208);
    b = r_u32(state + 212);
    resource002_sub_800ed0d4(resource);
    owner = r_u32(0x80112E10);
    resource = r_u32(entries + 4);
    source = r_u32(owner + 4);
    sub_800afd08(resource, source);
    scene = r_u32(0x800BCE54);
    owner = r_u32(0x80112E10);
    model = r_u32(scene + 592);
    source = r_u32(owner + 4);
    w_u32(model + 32, source);
    model = r_u32(scene + 592);
    source = r_u32(model + 32);
    relocate_pointer_array(source, source + 4, 6);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    w_u32(object, 0);
    object = r_u32(table + 8);
    value = r_u32(object);
    w_u32(object, value | 0x1C000);
    resource002_sub_800dee08(2, 1, 0, 25);
    resource002_sub_800dee08(8, 1, 0, 25);
    owner = r_u32(0x80112E10);
    value = r_u32(owner + 288);
    if (value)
    {
        w_u32(owner + 12, owner + 292);
        w_u32(owner + 8, owner + 292);
        w_u32(owner + 288, 0);
    }
    else
    {
        value = r_u32(owner + 8);
        w_u32(owner + 12, value);
    }
    state = r_u32(0x80112DC4);
    source = r_u32(state + 184) + index * 24;
    owner = r_u32(0x80112E10);
    w_u32(owner + 8, source);
    return 1;
}

uint32 resource002_sub_801084d0(void)
{
    uint16 delta[4] = {0, 0, 0, 0};
    uint32 game, state, value;
    FUNCTION_MARKER(0x801084D0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u32(game + 296) & 0x200)
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 252);
        if ((sint32)value > -2097152)
            w_u32(state + 252, value - 4096);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 296) & 0x100)
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 252);
        if ((sint32)value < -524288)
            w_u32(state + 252, value + 4096);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 296) & 8)
        delta[1] = 128;
    if (r_u32(game + 296) & 4)
        delta[1] = (uint16)-128;
    if (r_u32(game + 296) & 1)
        delta[0] = (uint16)-128;
    if (r_u32(game + 296) & 2)
        delta[0] = 128;
    state = r_u32(0x80112DC4);
    return sub_80030f64(state + 256, 0, delta);
}

uint32 resource002_sub_800febb8(uint32 mission, uint32 aircraft, uint32 kind, uint32 record)
{
    uint32 entry, value;
    FUNCTION_MARKER(0x800FEBB8, "ACE3.BIN");
    if (aircraft >= 37)
        aircraft = 0;
    if (kind == 0)
    {
        if (record >= 4)
            record = 0;
        return r_u8(0x80111A54 + aircraft * 12 + record + 4);
    }
    if (mission == 13)
        value = 8;
    else
    {
        entry = 0x80111A54 + aircraft * 12;
        value = r_u8(entry + 1);
        if (record == value + 1)
            value = 6;
        else if (record == value)
            value = 7;
        else
        {
            if (record >= 4)
                record = 0;
            value = r_u8(entry + record + 8);
        }
    }
    if (kind == 2)
        value += 6;
    return value;
}

uint32 resource002_sub_801085f8(uint32 mode)
{
    uint32 fade, owner, index, selected, table, slot, object, state, scene, camera, delta, product;
    FUNCTION_MARKER(0x801085F8, "ACE3.BIN");
    fade = resource002_sub_800deea8(2);
    owner = r_u32(0x80112E30);
    index = r_u8(owner + 0x4000);
    selected = r_s16(0x80112C08 + index * 8 + 4) == 0 ? 2 : 0;
    if (mode == 0)
    {
        delta = 25 - fade;
        product = delta * (0u - delta * 325u);
        SetGeomOffset((uint32)((sint32)product / 625) + 80, 132);
        owner = r_u32(0x80112E30);
        index = (uint32)r_s16(0x80112C08 + r_u8(owner + 0x4000) * 8 + 2);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + index * 4);
        w_u16(object + 24, 0);
        index = (uint32)r_s16(0x80112C08 + r_u8(owner + 0x4000) * 8 + 2);
        object = r_u32(table + index * 4);
        w_u16(object + 26, 4096);
        index = (uint32)r_s16(0x80112C08 + r_u8(owner + 0x4000) * 8 + 2);
        scene = r_u32(0x800BCE54);
        object = r_u32(table + index * 4);
        w_u16(object + 28, 0);
    }
    else if (mode == 1)
    {
        resource002_sub_801084d0();
        SetGeomOffset(160, 240);
        table = r_u32(0x800BCE5C);
        slot = table + selected * 4;
        state = r_u32(0x80112DC4);
        object = r_u32(slot);
        w_u16(object + 24, r_u16(state + 256));
        object = r_u32(slot);
        w_u16(object + 26, r_u16(state + 258));
        object = r_u32(slot);
        product = r_u16(state + 260);
        scene = r_u32(0x800BCE54);
        w_u16(object + 28, product);
        camera = r_u32(scene + 40);
        w_u32(camera + 4, 0);
        camera = r_u32(scene + 40);
        w_u32(camera + 8, r_u32(state + 252));
        camera = r_u32(scene + 40);
        w_u32(camera + 12, 0);
        return camera;
    }
    else if (mode == 2)
    {
        delta = 25 - fade;
        product = delta * (0u - delta * 325u);
        SetGeomOffset((uint32)((sint32)product / 625) + 80, 132);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + 8);
        w_u16(object + 24, 0);
        object = r_u32(table + 8);
        w_u16(object + 26, 4096);
        object = r_u32(table + 8);
        scene = r_u32(0x800BCE54);
        w_u16(object + 28, 0);
    }
    else
        return 2;
    camera = r_u32(scene + 40);
    w_u32(camera + 4, 0);
    camera = r_u32(scene + 40);
    w_u32(camera + 8, 0xFFF00000);
    camera = r_u32(scene + 40);
    w_u32(camera + 12, 0);
    return camera;
}

uint32 resource002_sub_80108874(uint32 mode)
{
    uint32 output[130];
    PSX_RECT rectangle;
    uint32 scene, camera, packet, ordering, link, tag, result, delta, product;
    FUNCTION_MARKER(0x80108874, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    SetRotMatrix((MATRIX *)psx_addr(scene + 76, 20));
    scene = r_u32(0x800BCE54);
    SetTransMatrix((MATRIX *)psx_addr(scene + 76, 32));
    scene = r_u32(0x800BCE54);
    SetBackColor(r_s32(scene + 172), r_s32(scene + 176), r_s32(scene + 180));
    scene = r_u32(0x800BCE54);
    SetColorMatrix((MATRIX *)psx_addr(scene + 140, 20));
    scene = r_u32(0x800BCE54);
    SetRotMatrix((MATRIX *)psx_addr(scene + 76, 20));
    scene = r_u32(0x800BCE54);
    SetTransMatrix((MATRIX *)psx_addr(scene + 76, 32));
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    w_u16(camera + 32, 320);
    camera = r_u32(scene + 40);
    w_u16(camera + 34, 480);
    camera = r_u32(scene + 40);
    w_u16(camera + 36, 250);
    if (mode == 1)
    {
        delta = 25 - resource002_sub_800deea8(1);
        product = delta * (0u - delta * 325u);
        scene = r_u32(0x800BCE54);
        packet = r_u32(scene + 20);
        w_u32(scene + 20, packet + 12);
        rectangle.x = (uint16)((uint32)((sint32)product / 625) + 23);
        rectangle.y = 58;
        rectangle.w = 155;
        rectangle.h = 147;
        SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
        scene = r_u32(0x800BCE54);
        ordering = r_u32(scene + 16);
        tag = r_u32(packet);
        link = r_u32(ordering + 252);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        ordering = r_u32(scene + 16);
        link = r_u32(ordering + 252);
        w_u32(ordering + 252, (link & 0xFF000000) | (packet & 0xFFFFFF));
    }
    scene = r_u32(0x800BCE54);
    packet = r_u32(scene + 20);
    w_u32(scene + 20, packet + 12);
    SetDrawMode(psx_addr(packet, 12), 0, 0, 0, NULL);
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    tag = r_u32(packet);
    link = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(scene + 16);
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (packet & 0xFFFFFF));
    ordering = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    packet = resource002_sub_8010817c(scene + 580, ordering + 4, packet, output);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, packet + 12);
    SetDrawMode(psx_addr(packet, 12), 0, 1, 0, NULL);
    scene = r_u32(0x800BCE54);
    ordering = r_u32(scene + 16);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(scene + 16);
    link = r_u32(ordering);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(ordering, result);
    if (mode == 1)
    {
        packet = r_u32(scene + 20);
        w_u32(scene + 20, packet + 12);
        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = 320;
        rectangle.h = 480;
        SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
        scene = r_u32(0x800BCE54);
        ordering = r_u32(scene + 16);
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        ordering = r_u32(scene + 16);
        link = r_u32(ordering);
        result = (link & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, result);
    }
    return result;
}

uint32 resource002_sub_800fc37c(void)
{
    SVECTOR angles = {0, 0, 0, 0};
    SVECTOR vector = {0, 0, 4096, 0};
    MATRIX matrix = {0};
    uint32 scene;
    FUNCTION_MARKER(0x800FC37C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    w_u16(scene + 140, 2048);
    w_u16(scene + 146, 2048);
    w_u16(scene + 152, 2048);
    w_u16(scene + 142, 0);
    w_u16(scene + 148, 0);
    w_u16(scene + 154, 0);
    w_u16(scene + 144, 0);
    w_u16(scene + 150, 0);
    w_u16(scene + 156, 0);
    w_u16(scene + 108, 0);
    w_u16(scene + 110, 0);
    w_u16(scene + 112, 0);
    w_u16(scene + 114, 0);
    w_u16(scene + 116, 0);
    w_u16(scene + 118, 0);
    w_u16(scene + 120, 0);
    w_u16(scene + 122, 0);
    w_u16(scene + 124, 0);
    w_u32(scene + 172, 84);
    w_u32(scene + 176, 84);
    w_u32(scene + 180, 84);
    RotMatrixYXZ(&angles, &matrix);
    ApplyMatrixSV(&matrix, &vector, (SVECTOR *)psx_addr(scene + 108, 6));
    angles.vx = 0;
    angles.vy = 0;
    angles.vz = 0;
    RotMatrixYXZ(&angles, &matrix);
    ApplyMatrixSV(&matrix, &vector, (SVECTOR *)psx_addr(scene + 114, 6));
    return scene + 114;
}

uint32 resource002_sub_800fc488(void)
{
    const uint8 angles[8] = {0};
    uint32 scene, camera;
    FUNCTION_MARKER(0x800FC488, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    w_u32(scene + 40, scene + 184);
    camera = r_u32(scene + 40);
    sub_80077880(camera, 200, 8000, 28000, 0, angles);
    resource002_sub_800fc37c();
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    w_u32(camera + 4, 0);
    camera = r_u32(scene + 40);
    w_u32(camera + 8, 0xFFE00000);
    camera = r_u32(scene + 40);
    w_u32(camera + 12, 0);
    camera = r_u32(scene + 40);
    w_u16(camera + 20, 0xF000);
    camera = r_u32(scene + 40);
    w_u16(camera + 22, 0);
    camera = r_u32(scene + 40);
    w_u16(camera + 24, 0);
    camera = r_u32(scene + 40);
    w_u32(scene + 72, 0);
    w_u32(scene + 68, 0);
    w_u32(scene + 64, 0);
    w_u32(scene + 104, 0);
    w_u32(scene + 100, 0);
    w_u32(scene + 96, 0);
    rotation_build(camera + 20, scene + 44, NULL, NULL);
    scene = r_u32(0x800BCE54);
    TransposeMatrix((MATRIX *)psx_addr(scene + 44, 18), (MATRIX *)psx_addr(scene + 76, 18));
    scene = r_u32(0x800BCE54);
    resource002_sub_80102120(scene + 76);
    SetGeomOffset(0, 0);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    SetGeomScreen(r_s16(camera + 36));
    return camera;
}

uint32 resource002_sub_800fd770(uint32 row, uint32 column)
{
    uint32 owner, state, index, table, resource;
    FUNCTION_MARKER(0x800FD770, "ACE3.BIN");
    owner = r_u32(0x80112E1C);
    state = r_u32(0x80112DC4);
    index = r_u8(owner + column + row * 4 + 6);
    table = r_u32(state + 468);
    resource = r_u32(table + index * 4);
    return resource002_sub_800ed000(resource, 960, 340, 384, 6);
}

uint32 resource002_sub_800fd7d0(uint32 row)
{
    uint32 index, owner, state, entry, table, resource, object, result;
    sint32 count, items = 4;
    FUNCTION_MARKER(0x800FD7D0, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        w_u8(r_u32(r_u32(0x80112DC0) + 92) + index + 53, 0);
    count = r_s16(r_u32(0x80112E1C) + 66);
    result = (sint32)row < count / 4;
    if (!result)
    {
        result = (uint32)(count % 4) << 16;
        items = (sint16)(uint16)(count % 4);
    }
    for (index = 0; (sint32)index < items; ++index)
    {
        owner = r_u32(0x80112E1C);
        state = r_u32(0x80112DC4);
        entry = r_u8(owner + row * 4 + index + 6);
        table = r_u32(state + 468);
        resource = r_u32(table + entry * 4);
        resource002_sub_800ed000(resource, 960, 340 + index * 18, 384, 6);
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + index + 53, 2);
        result = (sint32)(index + 1) < items;
    }
    return result;
}

uint32 resource002_sub_800fd8f4(uint32 column)
{
    uint32 owner, row, state, index, table, resource;
    FUNCTION_MARKER(0x800FD8F4, "ACE3.BIN");
    owner = r_u32(0x80112E1C);
    row = r_u8(owner + 5);
    state = r_u32(0x80112DC4);
    index = r_u8(owner + column + row * 4 + 6);
    table = r_u32(state + 476);
    resource = r_u32(table + index * 4);
    return resource002_sub_800ed000(resource, 960, 256, 768, 1);
}

uint32 resource002_sub_800feef8(uint32 mission)
{
    uint32 context;
    FUNCTION_MARKER(0x800FEEF8, "ACE3.BIN");
    context = r_u32(0x8011278C);
    w_u32(context + 8, mission);
    return context;
}

uint32 resource002_sub_800fe1e8(void)
{
    uint32 media, target, flags, state, phase, value, game;
    FUNCTION_MARKER(0x800FE1E8, "ACE3.BIN");
    media = (uint32)cd_resource_state();
    state = r_u32(0x80112DC4);
    target = r_u32(0x80112790 + r_u32(state + 612) * 4);
    flags = resource002_load_phase_call(target);
    if (flags & 1)
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 612);
        w_u32(state + 612, value + 1);
    }
    if (flags & 2)
    {
        w_u8(r_u32(0x80112E1C) + 1, 1);
        return 1;
    }
    if (flags & 4)
        w_u32(r_u32(0x80112DC4) + 612, 1);
    phase = resource002_sub_800feee0();
    if (phase)
        resource002_sub_800dedf0(0, 0x0A000003);
    switch (phase)
    {
        case 1:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 4);
            return 4;
        case 2:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 0);
            break;
        case 3:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 1);
            break;
        case 4:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 2);
            break;
        case 5:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 3);
            break;
        case 6:
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 61, 5);
            return 5;
        default:
            return 0x800D0000;
    }
    game = r_u32(0x800BCE60);
    value = r_u32(game + 300) & 32;
    if (!value)
        return value;
    if (r_u32(game + 296) & 0x5F)
        return 4;
    if (media == 4 || media == 0xFFFFFFFFu)
        return 0xFFFFFFFFu;
    if (sub_8002b46c())
        w_u8(r_u32(0x80112E1C) + 1, 2);
    return sub_8002db4c(59);
}

uint32 resource002_sub_800feee0(void)
{
    FUNCTION_MARKER(0x800FEEE0, "ACE3.BIN");
    return r_u32(r_u32(0x8011278C));
}

uint32 resource002_sub_800fef08(void)
{
    uint32 context;
    FUNCTION_MARKER(0x800FEF08, "ACE3.BIN");
    context = r_u32(0x8011278C);
    w_u32(context + 4, 0);
    w_u32(context, 0);
    return 1;
}

uint32 resource002_sub_800fef24(void)
{
    uint32 media, context, kind, value;
    FUNCTION_MARKER(0x800FEF24, "ACE3.BIN");
    media = (uint32)cd_resource_state();
    if (!media)
    {
        w_u32(r_u32(0x8011278C), 1);
        return 0;
    }
    context = r_u32(0x8011278C);
    kind = r_u8(0x80112750 + r_u32(context + 8));
    if (kind == 3 || media == 3)
    {
        w_u32(context, 0);
        return 2;
    }
    if (media == 4 || media == 0xFFFFFFFFu)
    {
        if (kind == 1)
        {
            value = r_u32(context + 4);
            w_u32(context, value ? 4 : 2);
        }
        context = r_u32(0x8011278C);
        kind = r_u8(0x80112750 + r_u32(context + 8));
        if (kind == 2)
        {
            value = r_u32(context + 4);
            w_u32(context, value ? 5 : 3);
        }
        sub_8002b428();
        return 1;
    }
    if (media == 1)
    {
        if (kind == 1)
        {
            w_u32(context, 6);
            return 2;
        }
        value = r_u32(context + 4);
        w_u32(context, value ? 5 : 3);
        sub_8002b428();
        return 1;
    }
    if (media == 2)
    {
        if (kind == 2)
        {
            w_u32(context, 6);
            return 2;
        }
        value = r_u32(context + 4);
        w_u32(context, value ? 4 : 2);
        sub_8002b428();
        return 1;
    }
    return 0;
}

uint32 resource002_sub_800ff098(void)
{
    FUNCTION_MARKER(0x800FF098, "ACE3.BIN");
    return sub_8002b528() == 3;
}

uint32 resource002_sub_800ff0bc(void)
{
    uint32 status, context;
    FUNCTION_MARKER(0x800FF0BC, "ACE3.BIN");
    status = sub_8002b528();
    if (status != 1)
        return 0;
    context = r_u32(0x8011278C);
    w_u32(context, status);
    w_u32(context + 4, status);
    return 4;
}

uint32 resource002_sub_800fabe8(void)
{
    const uint32 unlock_offsets[10] = {2451, 2452, 2453, 2454, 2455, 2456, 2421, 2422, 2438, 2439};
    uint32 unlock, owner, buffer, state, game, context, target, value, index, selected, descriptor, group;
    FUNCTION_MARKER(0x800FABE8, "ACE3.BIN");
    unlock = resource002_sub_800f78b0();
    w_u32(r_u32(0x80112DC4) + 620, unlock);
    resource_init_queue(0, 0);
    owner = arena_alloc(316);
    w_u32(0x80112E10, owner);
    buffer = arena_alloc(0x4000);
    owner = r_u32(0x80112E10);
    state = r_u32(0x80112DC4);
    game = r_u32(0x800BCE60);
    w_u32(owner + 4, buffer);
    w_u16(owner, 0);
    w_u16(owner + 2, 0);
    w_u32(owner + 288, 0);
    w_u32(state + 192, 0);
    if (r_u32(game + 2412) & 16)
    {
        if (unlock & 4)
            w_u8(game + 2445, 1);
        if (unlock & 2)
            w_u8(r_u32(0x800BCE60) + 2447, 1);
        if (unlock & 1)
            for (index = 0; index < 10; ++index)
                w_u8(r_u32(0x800BCE60) + unlock_offsets[index], 1);
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        w_u16(state + 610, 0);
        w_u16(state + 608, 0);
        if (r_s16(game + 908) == 42)
        {
            w_u16(state + 480, 24);
            w_u16(state + 610, 1);
        }
        else
        {
            for (index = 0; index < 64; ++index)
            {
                if (r_u8(game + 2420 + index) == 1 && index != 24)
                {
                    value = (uint32)r_s16(state + 610);
                    w_u16(state + 480 + value * 2, index);
                    if (r_s16(game + 910) == (sint32)index)
                        w_u16(state + 608, r_u16(state + 610));
                    value = r_u16(state + 610);
                    w_u16(state + 610, value + 1);
                }
            }
        }
        state = r_u32(0x80112DC4);
        game = r_u32(0x800BCE60);
        selected = (uint32)r_s16(state + 608);
        w_u32(state + 192, selected);
        value = (uint32)r_s16(state + selected * 2 + 480);
        if ((sint32)value != r_s16(game + 910))
        {
            w_u32(state + 192, 0);
            w_u16(state + 608, 0);
        }
        w_u32(r_u32(0x80112DC4) + 196, 0);
    }
    resource002_sub_800fc148();
    resource002_sub_800fc488();
    owner = r_u32(0x80112E10);
    state = r_u32(0x80112DC4);
    w_u32(owner + 12, 0x8010EB0C);
    selected = r_u32(state + 192);
    descriptor = r_u32(state + 184);
    w_u8(owner + 280, 0);
    game = r_u32(0x800BCE60);
    w_u32(owner + 8, descriptor + selected * 24);
    if (!(r_u32(game + 2412) & 16))
        resource002_sub_800f7514(0x8010EB00);
    for (index = 0; index < 5; ++index)
        sub_8002dda8(r_u32(r_u32(0x80112DC4) + 304 + index * 4));
    if (!(r_u32(r_u32(0x800BCE60) + 2412) & 16))
        resource002_sub_800f7524(1, 0);
    sub_8002db4c(90);
    if (r_u32(r_u32(0x800BCE60) + 2412) & 16)
    {
        resource002_sub_800dee08(3, 1, 0, 25);
        group = 11;
    }
    else
    {
        resource002_sub_800dee08(2, 1, 0, 25);
        resource002_sub_800dee08(8, 1, 0, 25);
        resource002_sub_800dee08(1, 1, 0, 25);
        resource002_sub_800dee08(3, 1, 0, 25);
        resource002_sub_800dee08(11, 1, 0, 25);
        resource002_sub_800dee08(5, 1, 0, 25);
        group = 6;
    }
    resource002_sub_800dee08(group, 1, 0, 25);
    resource002_sub_800dee08(7, 1, 0, 25);
    resource002_sub_800dedf0(0, 0x7701);
    context = r_u32(0x800BCE5C);
    target = r_u32(context + 8);
    value = r_u32(target);
    w_u32(target, value | 0x1C000);
    w_u8(r_u32(context + 8) + 4, 2);
    context = r_u32(0x800BCE5C);
    state = r_u32(0x80112DC4);
    w_u32(r_u32(context + 8) + 8, 0);
    w_u32(r_u32(context + 8) + 12, 0);
    w_u32(r_u32(context + 8) + 16, 0);
    w_u16(r_u32(context + 8) + 24, 0);
    w_u16(r_u32(context + 8) + 26, 4096);
    w_u16(r_u32(context + 8) + 28, 4096);
    sub_800a8440(state + 216, r_u32(state + 212), NULL, NULL, 0);
    DrawSync(0);
    w_u32(r_u32(0x80112E10) + 16, 3);
    resource002_sub_800f8ac4();
    resource002_sub_800f7994(0);
    if (r_u32(r_u32(0x800BCE60) + 2412) & 16)
    {
        resource002_sub_800dee08(2, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
        resource002_sub_800dee08(8, 3, 0xFFFFFFFFu, 0xFFFFFFFFu);
    }
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800fb10c(void)
{
    FUNCTION_MARKER(0x800FB10C, "ACE3.BIN");
    return resource002_sub_800fb114(r_u32(0x800BCE60));
}

uint32 resource002_sub_800fb114(uint32 game)
{
    uint32 owner, context, state, packet, ordering, tag, link, value, target;
    FUNCTION_MARKER(0x800FB114, "ACE3.BIN");
    if (!(r_u32(game + 2412) & 16) && r_s16(r_u32(0x80112E10)) != 5)
        resource002_sub_800f7524(1, 0);
    if (r_u32(r_u32(0x80112E10)) != 0x00020002)
    {
        packet = r_u32(r_u32(0x800BCE54) + 16);
        ClearOTagR(psx_addr(packet, 256), 64);
        context = r_u32(0x800BCE54);
        state = r_u32(0x80112DC4);
        packet = r_u32(context + 16);
        ordering = r_u32(state + 8);
        tag = r_u32(packet);
        link = r_u32(ordering + 12);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        packet = r_u32(context + 16);
        ordering = r_u32(state + 8);
        tag = r_u32(ordering + 12);
        w_u32(ordering + 12, (tag & 0xFF000000) | ((packet + 252) & 0xFFFFFF));
        resource002_sub_800f7be8();
    }
    value = r_u16(r_u32(0x80112E10)) - 3;
    if (value < 3)
    {
        resource002_sub_800f8c0c();
        state = r_u32(0x80112DC4);
        packet = r_u32(state + 628);
        ordering = r_u32(state + 8);
        value = resource002_sub_800f8fa8(packet, ordering);
        w_u32(r_u32(0x80112DC4) + 628, value);
    }
    value = resource002_sub_800f7ea4(r_u32(r_u32(0x80112DC4) + 628));
    w_u32(r_u32(0x80112DC4) + 628, value);
    owner = r_u32(0x80112E10);
    target = r_u32(0x8010ECF8 + (uint32)r_s16(owner) * 4);
    resource002_aircraft_phase_call(target);
    return resource002_sub_800f7524(0, 0);
}

uint32 resource002_sub_800f78b0(void)
{
    uint32 flags = 15, game, mission, best, value;
    FUNCTION_MARKER(0x800F78B0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    for (mission = 1; mission < 56; ++mission)
    {
        if (mission == 19 || mission == 49 || mission == 54)
            continue;
        value = r_u8(game + mission + 3100);
        best = value ? value : 5;
        value = r_u8(game + mission + 3160);
        if (value && value < best)
            best = value;
        value = r_u8(game + mission + 3220);
        if (value && value < best)
            best = value;
        if (best == 5)
            return 0;
        if (best >= 2)
            flags &= 0xFFFFFFF7;
        if (best >= 3)
            flags &= 0xFFFFFFFB;
        if (best >= 4)
            flags &= 0xFFFFFFFD;
    }
    return flags;
}

uint32 resource002_sub_800fc148(void)
{
    uint32 state, value, index, context, game, frame, target;
    FUNCTION_MARKER(0x800FC148, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u16(state + 216, 1008);
    w_u16(state + 218, 248);
    w_u16(state + 220, 16);
    w_u16(state + 222, 8);
    value = arena_alloc(256);
    state = r_u32(0x80112DC4);
    target = (uint32)r_s16(state + 222) << 5;
    w_u32(state + 208, value);
    value = arena_alloc(target);
    w_u32(r_u32(0x80112DC4) + 212, value);
    for (index = 0; index < 2; ++index)
    {
        value = arena_alloc(256);
        w_u32(r_u32(0x800BCE54) + index * 4 + 24, value);
        value = arena_alloc(18432);
        w_u32(r_u32(0x800BCE54) + index * 4 + 32, value);
    }
    game = r_u32(0x800BCE60);
    frame = r_u32(game + 4);
    context = r_u32(0x800BCE54);
    value = r_u32(context + frame * 4 + 24);
    w_u32(context + 16, value);
    frame = r_u32(game + 4);
    value = r_u32(context + frame * 4 + 32);
    w_u32(context + 20, value);
    value = arena_alloc(256);
    w_u32(0x800BCE5C, value);
    w_u32(0x800BCE4C, value + 128);
    for (index = 0; index < 2; ++index)
    {
        value = arena_alloc(88);
        w_u32(r_u32(0x800BCE5C) + index * 4, value);
        w_u32(value, 0);
    }
    for (index = 2; index < 32; ++index)
    {
        value = arena_alloc(92);
        w_u32(r_u32(0x800BCE5C) + index * 4, value);
        w_u32(value, 0);
    }
    for (index = 0; index < 32; ++index)
    {
        w_u8(r_u32(r_u32(0x800BCE5C) + index * 4) + 5, 0);
        w_u16(r_u32(r_u32(0x800BCE5C) + index * 4) + 6, 0);
    }
    for (index = 0; index < 32; ++index)
    {
        value = arena_alloc(148);
        w_u32(r_u32(0x800BCE4C) + index * 4, value);
        w_u32(value, 0);
    }
    value = arena_alloc(4);
    w_u32(r_u32(0x800BCE54) + 592, value);
    for (index = 0; index < 64; ++index)
    {
        value = arena_alloc(28);
        target = r_u32(r_u32(0x800BCE54) + 592);
        w_u32(target + index * 4, value);
    }
    return 0;
}

uint32 resource002_sub_800f8ac4(void)
{
    uint32 bank, index, owner, packet;
    void *command;
    FUNCTION_MARKER(0x800F8AC4, "ACE3.BIN");
    for (bank = 0; bank < 2; ++bank)
    {
        for (index = 0; index < 2; ++index)
        {
            owner = r_u32(0x80112E10);
            packet = owner + 24 + bank * 40 + index * 20;
            w_u8(packet + 3, 4);
            w_u8(packet + 7, 102);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            owner = r_u32(0x80112E10);
            w_u16(packet + 14, 2584);
            w_u16(packet + 16, 48);
            w_u16(packet + 18, 88);
            packet = owner + 104 + bank * 40 + index * 20;
            w_u8(packet + 3, 4);
            w_u8(packet + 7, 102);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            w_u16(packet + 16, 48);
            w_u16(packet + 18, 88);
            w_u16(packet + 14, 2648);
            command = psx_addr(r_u32(0x80112E10) + 184 + bank * 12, 12);
            SetDrawMode(command, 0, 0, 76, NULL);
        }
    }
    return 0;
}

uint32 resource002_sub_800f7be8(void)
{
    FUNCTION_MARKER(0x800F7BE8, "ACE3.BIN");
    if (!resource002_sub_800dee84(2))
        return 0;
    resource002_sub_801085f8(2);
    resource002_sub_80108874(1);
    return 1;
}

uint32 resource002_sub_800f8c0c(void)
{
    uint32 first, second, result = 2, owner, value, limit, input, active, game, record, context;
    FUNCTION_MARKER(0x800F8C0C, "ACE3.BIN");
    first = resource002_sub_800dee84(9);
    second = resource002_sub_800dee84(10);
    if (first == 0 || first == 2)
    {
        owner = r_u32(0x80112E10);
        value = r_u8(owner + 22);
        w_u8(owner + 20, value);
    }
    if (second == 0 || second == 2)
    {
        owner = r_u32(0x80112E10);
        result = r_u8(owner + 23);
        w_u8(owner + 21, result);
    }
    if (first == 2)
        result = resource002_sub_800dee08(9, 0, 0, 0);
    if (second == 2)
        result = resource002_sub_800dee08(10, 0, 0, 0);
    if (first || second)
        return result;
    if (r_u32(r_u32(0x800BCE60) + 300) & 3)
    {
        owner = r_u32(0x80112E10);
        value = r_u8(owner + 280);
        w_u8(owner + 280, 1 - value);
    }
    resource002_sub_800def94(9, 0);
    resource002_sub_800def94(8, 0);
    resource002_sub_800def94(11, 0);
    resource002_sub_800def94(10, 0);
    owner = r_u32(0x80112E10);
    if (r_s16(owner) != 5)
    {
        input = r_u32(r_u32(0x800BCE60) + 304);
        if (input & 8)
        {
            if (!r_u8(owner + 280))
            {
                limit = r_u8(owner + 282);
                value = r_u8(owner + 20);
                if ((sint32)value < (sint32)limit - 1)
                {
                    w_u8(owner + 22, value + 1);
                    resource002_sub_800dee08(9, 3, 10, 10);
                    resource002_sub_800def94(9, 1);
                    sub_8002db4c(61);
                }
            }
            owner = r_u32(0x80112E10);
            if (r_u8(owner + 280) == 1)
            {
                limit = r_u8(owner + 283);
                value = r_u8(owner + 21);
                if ((sint32)value < (sint32)limit - 1)
                {
                    w_u8(owner + 23, value + 1);
                    resource002_sub_800dee08(10, 3, 10, 10);
                    resource002_sub_800def94(11, 1);
                    sub_8002db4c(61);
                }
            }
        }
        else if (input & 4)
        {
            if (!r_u8(owner + 280))
            {
                value = r_u8(owner + 20);
                if (value)
                {
                    w_u8(owner + 22, value - 1);
                    resource002_sub_800dee08(9, 1, 0, 10);
                    resource002_sub_800def94(8, 1);
                    sub_8002db4c(61);
                }
            }
            owner = r_u32(0x80112E10);
            if (r_u8(owner + 280) == 1)
            {
                value = r_u8(owner + 21);
                if (value)
                {
                    w_u8(owner + 23, value - 1);
                    resource002_sub_800dee08(10, 1, 0, 10);
                    resource002_sub_800def94(10, 1);
                    sub_8002db4c(61);
                }
            }
        }
    }
    owner = r_u32(0x80112E10);
    limit = r_u8(owner + 282);
    value = r_u8(owner + 20);
    active = (sint32)value < (sint32)limit - 1 ? 2 : 0;
    if (value)
        active |= 1;
    resource002_sub_800def94(7, active);
    owner = r_u32(0x80112E10);
    limit = r_u8(owner + 283);
    value = r_u8(owner + 21);
    active = (sint32)value < (sint32)limit - 1 ? 2 : 0;
    if (value)
        active |= 1;
    resource002_sub_800def94(14, active);
    resource002_sub_800def94(15, r_u8(r_u32(0x80112E10) + 280));
    owner = r_u32(0x80112E10);
    active = r_u8(owner + 280);
    if (active != 0 && active != 1)
        return 0x800C0000;
    game = r_u32(0x800BCE60);
    record = r_u8(owner + (active ? 23 : 22));
    value = (uint32)r_s16(game + 908);
    context = (uint32)r_s16(game + 910);
    result = resource002_sub_800febb8(value, context, active ? 2 : 0, record);
    w_u8(r_u32(0x80112E10) + 281, result);
    return result;
}
