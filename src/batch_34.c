#include "game_functions.h"
#include <stdlib.h>

static void resource002_map_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x80106000:
            resource002_sub_80106000();
            return;
        case 0x8010606C:
            resource002_sub_8010606c();
            return;
        case 0x801062B8:
            resource002_sub_801062b8();
            return;
        case 0x8010655C:
            resource002_sub_8010655c();
            return;
        case 0x801065C8:
            resource002_sub_801065c8();
            return;
        case 0x8010673C:
            resource002_sub_8010673c();
            return;
        case 0x80106780:
            resource002_sub_80106780();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 map phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_80105fe0(uint32 owner)
{
    FUNCTION_MARKER(0x80105FE0, "ACE3.BIN");
    return arena_set_cursor(owner);
}

uint32 resource002_sub_80105fd8(void)
{
    FUNCTION_MARKER(0x80105FD8, "ACE3.BIN");
    return resource002_sub_80105fe0(r_u32(0x80112DAC));
}

uint32 resource002_sub_80105c20(uint32 packet)
{
    uint8 arguments[8];
    uint32 owner, value, screen, quotient, remainder;
    FUNCTION_MARKER(0x80105C20, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    if (r_s32(owner + 2500) >= 0)
    {
        SetRotMatrix((MATRIX *)psx_addr(owner + 80, 20));
        SetTransMatrix((MATRIX *)psx_addr(owner + 80, 32));
        xport_gte_write_data(0, r_u32(owner + 2492));
        xport_gte_write_data(1, r_u32(owner + 2496));
        xport_gte_execute(0x180001);
        screen = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        resource002_sub_800f33a8((uint32)(sint32)(sint16)screen - 10, (uint32)((sint32)screen >> 16));
        resource002_sub_800f33d0(128, 128, 128);
        owner = r_u32(0x80112DAC);
        value = r_u32(owner + 2500);
        quotient = (uint32)((sint32)value / 60);
        remainder = value - quotient * 60;
        xport_store_le32(arguments, quotient);
        xport_store_le32(arguments + 4, remainder);
        resource002_sub_800f4ae0(0x800CD77C, 0, arguments, sizeof(arguments));
    }
    return packet;
}

uint32 resource002_sub_80106a34(void)
{
    PSX_RECT rectangle;
    uint32 owner, state, group, phase, table, target, packet, ordering, tag, link, value, other, index, object, type, game;
    uint32 scroll_x, scroll_y, center_x, x, y, left, right, cursor;
    FUNCTION_MARKER(0x80106A34, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    group = r_u8(owner + 2516);
    phase = r_u8(owner + 2517);
    table = r_u32(0x80112BE0 + group * 4);
    target = r_u32(table + phase * 4);
    resource002_map_phase_call(target);
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2518) == 0)
    {
        state = r_u32(0x80112DC4);
        packet = r_u32(state + 628);
        (void)r_u32(state + 20);
        packet = resource002_sub_801048f0(packet);
        state = r_u32(0x80112DC4);
        w_u32(state + 628, packet);
    }
    state = r_u32(0x80112DC4);
    packet = r_u32(state + 628);
    (void)r_u32(state + 20);
    packet = resource002_sub_80105c20(packet);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, packet);
    value = r_u32(0x800CD784);
    other = r_u32(0x800CD788);
    xport_store_le32((uint8 *)&rectangle, value);
    xport_store_le32((uint8 *)&rectangle + 4, other);
    packet = r_u32(state + 628);
    ordering = r_u32(state + 20);
    w_u32(state + 628, packet + 12);
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    owner = r_u32(0x80112DAC);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    if (r_u8(owner + 2516) && r_u8(owner + 2517))
    {
        state = r_u32(0x80112DC4);
        value = r_u32(owner + 32);
        cursor = r_u32(state + 628);
        ordering = r_u32(state + 20);
        scroll_x = (uint32)((sint32)value / 128) + 128;
        value = 0 - r_u32(owner + 40);
        scroll_y = (uint32)((sint32)value / 128) + 128;
        cursor += 16;
        center_x = (uint32)(sint32)(sint16)scroll_x - 128;
        for (index = 0; index < 32; ++index)
        {
            owner = r_u32(0x80112DAC);
            value = r_u32(owner + 2512);
            if (((value >> index) & 1) == 0)
                continue;
            object = r_u32(owner + index * 4 + 2524);
            type = (uint32)r_s16(object);
            if (type == 9 || (sint32)type >= 11)
                continue;
            x = (uint32)r_s16(object + 12);
            x = (uint32)((sint32)x / 128) - center_x;
            y = 0 - (uint32)r_s16(object + 16);
            x = (uint32)((sint32)x / 2);
            y = (uint32)((sint32)y / 128) - scroll_y;
            packet = cursor;
            cursor += 16;
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 96);
            w_u16(packet + 8, x + 275);
            w_u16(packet + 10, y + 292);
            w_u16(packet + 12, 3);
            w_u16(packet + 14, 6);
            type = (uint32)r_s16(object);
            if (type < 11)
            {
                target = r_u32(0x800CD794 + type * 4);
                switch (target)
                {
                    case 0x80106CFC:
                        w_u8(packet + 4, 56);
                        w_u8(packet + 5, 96);
                        w_u8(packet + 6, 200);
                        break;
                    case 0x80106D14:
                        w_u8(packet + 4, 255);
                        w_u8(packet + 5, 255);
                        w_u8(packet + 6, 255);
                        break;
                    case 0x80106D28:
                        w_u8(packet + 4, 232);
                        w_u8(packet + 5, 208);
                        w_u8(packet + 6, 104);
                        break;
                    case 0x80106D3C:
                        w_u8(packet + 4, 240);
                        w_u8(packet + 5, 80);
                        w_u8(packet + 6, 104);
                        break;
                    case 0x80106D54:
                        w_u8(packet + 4, 144);
                        w_u8(packet + 5, 208);
                        w_u8(packet + 6, 208);
                        break;
                    case 0x80106D64:
                        break;
                    default:
#ifndef NDEBUG
                        fprintf(stderr, "Unbound resource2 map color branch %08X\n", target);
#endif
                        abort();
                }
            }
            owner = r_u32(0x80112DAC);
            if (r_u8(owner + 2518) == 0)
            {
                tag = r_u32(packet);
                link = r_u32(ordering);
                w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
                link = r_u32(ordering);
                w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
            }
        }
        packet = cursor;
        state = r_u32(0x80112DC4);
        w_u32(state + 628, cursor);
        w_u8(packet + 3, 9);
        w_u8(packet + 7, 44);
        value = (uint32)((sint32)(sint16)scroll_x / 2);
        left = 275 - value;
        right = 403 - value;
        w_u16(packet + 10, 164 - scroll_y);
        w_u16(packet + 18, 164 - scroll_y);
        w_u16(packet + 26, 420 - scroll_y);
        w_u16(packet + 34, 420 - scroll_y);
        w_u16(packet + 22, 5);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u8(packet + 12, 0);
        w_u8(packet + 13, 0);
        w_u8(packet + 20, 128);
        w_u8(packet + 21, 0);
        w_u8(packet + 28, 0);
        w_u8(packet + 29, 128);
        w_u8(packet + 36, 128);
        w_u8(packet + 37, 128);
        owner = r_u32(0x80112DAC);
        w_u16(packet + 8, left);
        w_u16(packet + 16, right);
        w_u16(packet + 24, left);
        w_u16(packet + 32, right);
        w_u16(packet + 14, 0x5714);
        cursor += 40;
        if (r_u8(owner + 2518) == 0)
        {
            tag = r_u32(packet);
            link = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            link = r_u32(ordering);
            w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
        }
        state = r_u32(0x80112DC4);
        w_u32(state + 628, cursor);
    }
    state = r_u32(0x80112DC4);
    value = r_u32(0x800CD78C);
    other = r_u32(0x800CD790);
    xport_store_le32((uint8 *)&rectangle, value);
    xport_store_le32((uint8 *)&rectangle + 4, other);
    packet = r_u32(state + 628);
    ordering = r_u32(state + 20);
    w_u32(state + 628, packet + 12);
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    game = r_u32(0x800BCE60);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    if ((r_u32(game + 300) & 32) && (r_u32(game + 296) & 0x5F) == 0)
    {
        sub_8002cc44(255, 0, 0);
        state = r_u32(0x80112DC4);
        object = r_u32(state + 364);
        if (r_s16(object + 70) == 2)
        {
            w_u16(object + 70, 3);
            resource002_sub_800dee08(1, 0, 0, 0);
        }
        else
        {
            resource002_sub_8010097c();
            resource002_sub_800dee08(1, 3, -1, -1);
            owner = r_u32(0x80112DAC);
            if (r_u8(owner + 2516))
            {
                state = r_u32(0x80112DC4);
                w_u32(state + 420, 29);
            }
            else
            {
                state = r_u32(0x80112DC4);
                object = r_u32(state + 364);
                value = (uint32)r_s16(object + 70);
                if (value == 0)
                    w_u32(state + 420, 0);
                else if (value == 1)
                {
                    value = (r_u32(state + 420) - 258) * 29;
                    value = (uint32)((sint32)value / 64);
                    w_u32(state + 420, value);
                    if ((sint32)value < 0)
                        w_u32(state + 420, 0);
                    state = r_u32(0x80112DC4);
                    if (r_s32(state + 420) >= 30)
                        w_u32(state + 420, 29);
                }
                else
                    w_u32(state + 420, 29);
            }
            resource002_sub_800decd4(0, 5);
            resource002_sub_800decd4(0, 7);
            sub_8002db4c(91);
            sub_8002db4c(59);
        }
        owner = r_u32(0x80112DAC);
        w_u8(owner + 2518, 2);
    }
    state = r_u32(0x80112DC4);
    object = r_u32(state + 364);
    if (r_s16(object + 70) != 3)
        return 3;
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2518) == 1)
    {
        game = r_u32(0x800BCE60);
        resource002_sub_800f7524(3, (uint32)r_s16(game + 908));
        state = r_u32(0x80112DC4);
        w_u16(state + 132, 2);
        w_u16(state + 134, 0);
    }
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2518) == 2)
    {
        state = r_u32(0x80112DC4);
        w_u8(state + 124, 0);
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 0);
        state = r_u32(0x80112DC4);
        w_u8(state + 126, 0);
        resource002_sub_800debb8(0, 1);
        game = r_u32(0x800BCE60);
        resource002_sub_800f7524(3, (uint32)r_s16(game + 908));
    }
    return resource002_sub_80105fd8();
}

uint32 resource002_sub_801020c0(void)
{
    uint32 owner;
    FUNCTION_MARKER(0x801020C0, "ACE3.BIN");
    owner = arena_alloc(2684);
    w_u32(owner + 2500, 0xFFFFFFFF);
    w_u32(0x80112DAC, owner);
    return owner;
}

uint32 resource002_sub_801020f4(void)
{
    uint32 owner;
    FUNCTION_MARKER(0x801020F4, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    w_u16(owner + 144, 128);
    w_u16(owner + 146, 128);
    w_u32(owner + 116, 0);
    w_u32(owner + 120, 0);
    w_u16(owner + 124, 0);
    w_u16(owner + 126, 0);
    w_u16(owner + 128, 0);
    return owner;
}

uint32 resource002_sub_80105f04(void)
{
    uint32 index, owner, object;
    FUNCTION_MARKER(0x80105F04, "ACE3.BIN");
    for (index = 0; index < 32; ++index)
    {
        object = arena_alloc(32);
        owner = r_u32(0x80112DAC);
        w_u32(owner + index * 4 + 2524, object);
        w_u16(object + 4, 0);
        w_u16(object + 6, 0);
        w_u16(object + 2, 0xFFFF);
        w_u16(object + 28, 0xFFFF);
        w_u16(object + 30, 0xFFFF);
    }
    return 0;
}

uint32 resource002_sub_80105f80(uint32 global_base, uint32 owner)
{
    uint32 index, object;
    FUNCTION_MARKER(0x80105F80, "ACE3.BIN");
    w_u16(owner + 2508, 0);
    for (index = 0; index < 8; ++index)
    {
        object = arena_alloc(4);
        owner = r_u32(global_base + 0x2DAC);
        w_u32(owner + index * 4 + 2652, object);
    }
    return 0;
}

uint32 resource002_sub_80105f78(void)
{
    FUNCTION_MARKER(0x80105F78, "ACE3.BIN");
    return resource002_sub_80105f80(0x80110000, r_u32(0x80112DAC));
}

uint32 resource002_sub_80102564(void)
{
    static const uint32 colors[16] = {0x804040, 0x606040, 0x608040, 0x408040, 0x408060, 0x408080, 0x406080, 0x404080, 0x404060, 0x849494, 0x90A0A0, 0xACB4B4, 0xB4C4C4, 0xC8D2D2, 0xD2DCDC, 0xE6F0F0};
    uint32 owner, index;
    FUNCTION_MARKER(0x80102564, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    for (index = 0; index < 16; ++index)
        w_u32(owner + 2196 + index * 4, colors[index]);
    return owner;
}

uint32 resource002_sub_801067b0(void)
{
    uint32 game, mission, state, owner, value, record, event, type, target, argument;
    FUNCTION_MARKER(0x801067B0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    mission = (uint32)r_s16(game + 908);
    if (r_s16(0x801127AC + mission * 12) == -1)
    {
        state = r_u32(0x80112DC4);
        value = r_u16(state + 132);
        w_u16(state + 132, value + 1);
        return 0;
    }
    resource002_sub_801020c0();
    resource002_sub_80105f04();
    resource002_sub_80105f78();
    resource002_sub_801020f4();
    resource002_sub_800f7514(0x801127AC);
    game = r_u32(0x800BCE60);
    mission = (uint32)r_s16(game + 908);
    if (r_s16(0x80112AAC + mission * 4 + 2) == 1)
    {
        sub_8002cc44(2, 0, 0);
        state = r_u32(0x80112DC4);
        record = r_u32(state + 32);
        (void)r_u32(record + 24);
        sub_8002d6e0();
    }
    game = r_u32(0x800BCE60);
    mission = (uint32)r_s16(game + 908);
    if (r_s16(0x80112AAC + mission * 4 + 2) == 0)
        sub_8002cc44(2, 0, 0);
    state = r_u32(0x80112DC4);
    record = r_u32(state + 32);
    resource002_sub_800ecec8(r_u32(record + 12));
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DAC);
    event = r_u32(state + 448);
    w_u32(owner + 2504, event);
    value = (uint32)r_s16(event);
    while (value == 0xFFFFFFFF)
    {
        owner = r_u32(0x80112DAC);
        event = r_u32(owner + 2504);
        type = r_u8(event + 2);
        argument = (uint32)r_s16(event);
        target = r_u32(0x80112B9C + type * 4);
        resource002_mode_event_call(target, argument);
        owner = r_u32(0x80112DAC);
        event = r_u32(owner + 2504);
        w_u32(owner + 2504, event + 16);
        value = (uint32)r_s16(event + 16);
    }
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2510, 0);
    resource002_sub_80102564();
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134);
    owner = r_u32(0x80112DAC);
    w_u16(state + 134, value + 1);
    w_u8(owner + 2516, 0);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2517, 0);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2518, 0);
    resource002_sub_800dee08(2, 0, 0, 0);
    resource002_sub_800dee08(3, 0, 0, 0);
    resource002_sub_800dee08(4, 0, 0, 0);
    resource002_sub_800f35b8(7);
    resource002_sub_800f33f8(0xFFFFFFFF);
    resource002_sub_800f34c8(1, 1, 1);
    resource002_sub_800f34b0(1);
    return resource002_sub_800f3218(256);
}

uint32 resource002_sub_800fe8d8(uint32 mission, uint32 variant)
{
    uint32 game, state;
    FUNCTION_MARKER(0x800FE8D8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
    {
        if (variant >= 64)
            variant = 0;
        state = r_u32(0x80112DC4);
        return (uint32)r_s16(state + variant * 2 + 480);
    }
    if (variant >= 7)
        variant = 0;
    return r_u8(0x80111784 + mission * 12 + variant + 5);
}

uint32 resource002_sub_800fe9c0(uint32 mission, uint32 aircraft, uint32 kind)
{
    uint32 entry, value, game, state;
    FUNCTION_MARKER(0x800FE9C0, "ACE3.BIN");
    if (aircraft >= 37)
        aircraft = 0;
    entry = 0x80111A54 + aircraft * 12;
    if (kind == 0)
        return r_u8(entry);
    if (mission == 13)
        return 1;
    value = r_u8(entry + 1);
    if (mission == 42)
        return value;
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
    {
        ++value;
        if (mission == 32 || mission == 47 || mission == 53 || mission == 55)
            return value;
        state = r_u32(0x80112DC4);
        if (r_u32(state + 620) & 8)
            ++value;
    }
    else if (mission == 14 || mission == 28 || mission == 29 || mission == 43 || mission == 44)
        ++value;
    return value;
}

uint32 resource002_sub_800fead4(uint32 mission, uint32 aircraft, uint32 kind)
{
    uint32 entry, value, index;
    FUNCTION_MARKER(0x800FEAD4, "ACE3.BIN");
    if (aircraft >= 37)
        aircraft = 0;
    entry = 0x80111A54 + aircraft * 12;
    if (kind == 0)
    {
        value = r_u8(entry + 2);
        for (index = 0; index < 4; ++index)
            if (r_u8(entry + index + 4) == value)
                return index;
        return 0;
    }
    if (mission == 13)
        return 0;
    if (mission == 14 || mission == 28 || mission == 29 || mission == 43 || mission == 44)
        return r_u8(entry + 1);
    value = r_u8(entry + 3);
    for (index = 0; index < 4; ++index)
        if (r_u8(entry + index + 8) == value)
            return index;
    return 0;
}

uint32 resource002_sub_801071b8(void)
{
    uint32 owner, value;
    FUNCTION_MARKER(0x801071B8, "ACE3.BIN");
    owner = r_u32(0x80112DDC);
    value = r_u8(owner + 1);
    w_u8(owner + 1, value + 1);
    resource002_sub_800dee08(15, 1, 0, 20);
    return resource002_sub_800ff19c(3);
}

uint32 resource002_sub_80107200(void)
{
    uint32 owner, value, result;
    FUNCTION_MARKER(0x80107200, "ACE3.BIN");
    owner = r_u32(0x80112DA8);
    if (r_u32(owner + 20))
        return 0x80110000;
    owner = r_u32(0x80112DDC);
    value = r_u8(owner + 1);
    w_u8(owner + 1, value + 1);
    owner = r_u32(0x800BCE48);
    value = r_u8(owner + 16);
    result = value < 143 ? 1 : 32;
    w_u32(0x80112BF4, result);
    return result;
}

uint32 resource002_sub_80107270(void)
{
    uint32 counter, owner, value, product;
    FUNCTION_MARKER(0x80107270, "ACE3.BIN");
    counter = r_u32(0x80112BF4) - 1;
    w_u32(0x80112BF4, counter);
    if (counter == 0)
    {
        owner = r_u32(0x80112DDC);
        value = r_u8(owner + 1);
        w_u8(owner + 1, value + 1);
    }
    counter = r_u32(0x80112BF4);
    product = counter * 143;
    owner = r_u32(0x800BCE48);
    w_u8(owner + 16, (sint32)product / 32);
    counter = r_u32(0x80112BF4);
    product = counter * 135;
    owner = r_u32(0x800BCE48);
    w_u8(owner + 17, (sint32)product / 32);
    counter = r_u32(0x80112BF4);
    product = counter * 83;
    owner = r_u32(0x800BCE48);
    w_u8(owner + 18, (sint32)product / 32);
    owner = r_u32(0x800BCE48);
    value = r_u32(owner) | 8;
    w_u32(owner, value);
    return value;
}

uint32 resource002_sub_80107358(void)
{
    uint32 state, game, value;
    FUNCTION_MARKER(0x80107358, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    w_u8(state + 127, 1);
    resource002_sub_800dee08(2, 0, 0, 0);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 2412) | 2;
    w_u32(game + 2412, value);
    return value;
}

uint32 resource002_sub_80108298(void)
{
    uint32 owner, index, context, resource, bytes, buffer, game;
    FUNCTION_MARKER(0x80108298, "ACE3.BIN");
    owner = r_u32(0x80112E30);
    index = r_u8(owner + 0x4000);
    context = r_u32(0x800BCE50);
    resource = (uint32)r_s16(0x80112C08 + index * 8);
    w_u32(context + 96, resource);
    bytes = resource_total_size(context + 96, 1);
    buffer = arena_alloc(bytes);
    context = r_u32(0x800BCE50);
    game = r_u32(0x800BCE60);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    owner = r_u32(0x80112E30);
    w_u8(owner + 0x4001, 1);
    return owner;
}

uint32 resource002_sub_80108334(void)
{
    uint32 owner, game, context, resource, entries, scene, model, source, state;
    FUNCTION_MARKER(0x80108334, "ACE3.BIN");
    owner = r_u32(0x80112E30);
    if (r_u8(owner + 0x4001) == 0)
        return 0xFFFFFFFF;
    game = r_u32(0x800BCE60);
    if (r_u32(game + 68) == 1)
        return 0;
    if (resource002_sub_800dee84(2))
        return 0xFFFFFFFF;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    entries = sub_80027338(resource, NULL);
    entries = sub_80027338(r_u32(entries), NULL);
    resource002_sub_800ed0d4(r_u32(entries));
    source = r_u32(entries + 4);
    owner = r_u32(0x80112E30);
    sub_800afd08(source, owner);
    scene = r_u32(0x800BCE54);
    owner = r_u32(0x80112E30);
    model = r_u32(scene + 592);
    w_u32(model + 32, owner);
    model = r_u32(scene + 592);
    source = r_u32(model + 32);
    relocate_pointer_array(source, source + 4, 6);
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112E30);
    w_u32(state + 252, 0xFFF00000);
    w_u16(state + 256, 3072);
    w_u16(state + 258, 10240);
    w_u16(state + 260, 0);
    w_u8(owner + 0x4001, 0);
    resource002_sub_800dee08(2, 1, 0, 25);
    resource002_sub_800dee08(8, 1, 0, 25);
    arena_set_cursor(resource);
    return 1;
}

uint32 resource002_sub_80108478(uint32 mode)
{
    uint32 state, pixels;
    FUNCTION_MARKER(0x80108478, "ACE3.BIN");
    if (mode != 0 && mode != 1)
        return 1;
    state = r_u32(0x80112DC4);
    pixels = r_u32(state + (mode ? 212 : 208));
    return sub_800a8440(state + 216, pixels, NULL, NULL, 0);
}

uint32 resource002_sub_80108b7c(void)
{
    uint32 owner, state, table, object, value, scene, camera;
    FUNCTION_MARKER(0x80108B7C, "ACE3.BIN");
    owner = arena_alloc(0x4008);
    w_u32(0x80112E30, owner);
    w_u8(owner + 0x4000, 0);
    owner = r_u32(0x80112E30);
    w_u8(owner + 0x4001, 1);
    owner = r_u32(0x80112E30);
    w_u8(owner + 0x4002, 0);
    owner = r_u32(0x80112E30);
    w_u8(owner + 0x4003, 0);
    resource002_sub_800fc148();
    resource002_sub_800fc488();
    resource002_sub_80108298();
    state = r_u32(0x80112DC4);
    resource002_sub_800ece38(r_u32(state + 164), 0x10000);
    resource002_sub_800deca8(2, 20);
    resource002_sub_800dedf0(2, 0x100000);
    resource002_sub_800deca8(0, 9);
    resource002_sub_800deca8(0, 12);
    resource002_sub_800deca8(0, 10);
    resource002_sub_800deca8(0, 14);
    resource002_sub_800deca8(0, 8);
    resource002_sub_800dedf0(0, 0x5701);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 1, 1);
    resource002_sub_800dee08(2, 0, 0, 0);
    resource002_sub_800def94(1, 0);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    w_u32(object, 0);
    object = r_u32(table + 8);
    w_u32(object, 0);
    object = r_u32(table);
    w_u8(object + 4, 2);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + 8);
    w_u8(object + 4, 2);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    value = r_u8(object + 5);
    w_u8(object + 5, value | 1);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    w_u32(object + 8, 0);
    object = r_u32(table);
    w_u32(object + 12, 0);
    object = r_u32(table);
    w_u32(object + 16, 0);
    object = r_u32(table);
    w_u16(object + 24, 0);
    object = r_u32(table);
    w_u16(object + 26, 4096);
    object = r_u32(table);
    w_u16(object + 28, 4096);
    object = r_u32(table + 8);
    w_u32(object + 8, 0);
    object = r_u32(table + 8);
    w_u32(object + 12, 0);
    object = r_u32(table + 8);
    w_u32(object + 16, 0);
    object = r_u32(table + 8);
    w_u16(object + 24, 0);
    object = r_u32(table + 8);
    w_u16(object + 26, 4096);
    object = r_u32(table + 8);
    scene = r_u32(0x800BCE54);
    w_u16(object + 28, 4096);
    camera = r_u32(scene + 40);
    w_u32(camera + 196, 1);
    return 1;
}

uint32 resource002_sub_80108d90(void)
{
    uint32 owner, table, object, value;
    FUNCTION_MARKER(0x80108D90, "ACE3.BIN");
    if (resource002_sub_80108334() == 1)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + 1, 2);
        resource002_sub_800dee08(1, 1, 0, 25);
        resource002_sub_800dee08(3, 1, 0, 25);
        resource002_sub_800dee08(11, 1, 0, 25);
        resource002_sub_800dee08(5, 1, 0, 25);
        resource002_sub_800dee08(8, 1, 0, 25);
    }
    table = r_u32(0x800BCE5C);
    object = r_u32(table + 8);
    value = r_u32(object);
    w_u32(object, value | 0x1C000);
    object = r_u32(table);
    w_u32(object, 0);
    resource002_sub_800def94(1, 0);
    resource002_sub_800def94(3, 0);
    resource002_sub_800def94(2, 0);
    return resource002_sub_800def94(4, 0);
}

uint32 resource002_sub_80108e88(void)
{
    uint32 game, owner, value, state, object, context, bytes, buffer, scene, packet, ordering, tag, link, result, index;
    FUNCTION_MARKER(0x80108E88, "ACE3.BIN");
    resource002_sub_800def94(1, 0);
    game = r_u32(0x800BCE60);
    resource002_sub_800def94(2, (r_u32(game + 296) & 4) != 0);
    game = r_u32(0x800BCE60);
    resource002_sub_800def94(3, (r_u32(game + 296) & 8) != 0);
    state = r_u32(0x80112DC4);
    object = r_u32(state + 364);
    resource002_sub_800def94(4, r_s16(object + 106) == 2);
    game = r_u32(0x800BCE60);
    if (r_u32(game + 300) & 8)
    {
        owner = r_u32(0x80112E30);
        value = r_u8(owner + 0x4000);
        if (value < 48)
            w_u8(owner + 0x4000, value + 1);
        resource002_sub_80108298();
        resource002_sub_800dee08(2, 3, -1, -1);
        resource002_sub_800dee08(8, 3, -1, -1);
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 300) & 4)
    {
        owner = r_u32(0x80112E30);
        value = r_u8(owner + 0x4000);
        if (value)
            w_u8(owner + 0x4000, value - 1);
        resource002_sub_80108298();
        resource002_sub_800dee08(2, 3, -1, -1);
        resource002_sub_800dee08(8, 3, -1, -1);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && (r_u32(game + 296) & 0x2F) == 0)
    {
        owner = r_u32(0x80112E30);
        value = r_u8(owner + 0x4000);
        value = (uint32)r_s16(0x80112C08 + value * 8 + 4);
        if (value)
        {
            context = r_u32(0x800BCE50);
            w_u32(context + 96, value);
            bytes = resource_total_size(context + 96, 1);
            buffer = arena_alloc(bytes);
            context = r_u32(0x800BCE50);
            game = r_u32(0x800BCE60);
            w_u32(game + 68, 1);
            resource_queue_list(context + 96, 1, buffer, 0x8009B904);
        }
        owner = r_u32(0x80112DDC);
        w_u8(owner + 1, 3);
        {
            static const uint8 slots[] = {2, 8, 1, 3, 11, 5};
            for (index = 0; index < 6; ++index)
                resource002_sub_800dee08(slots[index], 3, -1, -1);
        }
        owner = r_u32(0x80112E30);
        w_u8(owner + 0x4003, 1);
    }
    resource002_sub_80108334();
    scene = r_u32(0x800BCE54);
    packet = r_u32(scene + 16);
    ClearOTagR(psx_addr(packet, 256), 64);
    scene = r_u32(0x800BCE54);
    state = r_u32(0x80112DC4);
    packet = r_u32(scene + 16);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 12);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    packet = r_u32(scene + 16);
    ordering = r_u32(state + 8);
    link = r_u32(ordering + 12);
    w_u32(ordering + 12, (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF));
    resource002_sub_801085f8(0);
    resource002_sub_80108874(1);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result)
    {
        result = r_u32(game + 296) & 0x5F;
        if (!result)
        {
            static const uint8 slots[] = {8, 2, 1, 3, 11, 5};
            owner = r_u32(0x80112DDC);
            w_u8(owner + 1, 3);
            for (index = 0; index < 6; ++index)
                resource002_sub_800dee08(slots[index], 3, -1, -1);
            owner = r_u32(0x80112E30);
            w_u8(owner + 0x4003, 2);
            return 2;
        }
    }
    return result;
}

uint32 resource002_sub_801092bc(void)
{
    uint32 owner, value, scene, packet, state, ordering, tag, link;
    FUNCTION_MARKER(0x801092BC, "ACE3.BIN");
    if (!resource002_sub_800dee84(2))
    {
        owner = r_u32(0x80112E30);
        value = r_u8(owner + 0x4003);
        if (value == 1)
        {
            owner = r_u32(0x80112DDC);
            w_u8(owner + 1, 4);
            return resource002_sub_800dedf0(2, 0x200000);
        }
        if (value == 2)
        {
            resource002_sub_800debb8(0, 1);
            owner = r_u32(0x80112DDC);
            w_u8(owner, 0);
            owner = r_u32(0x80112DDC);
            w_u8(owner + 1, 0);
            return arena_set_cursor(r_u32(0x80112E30));
        }
        return 2;
    }
    scene = r_u32(0x800BCE54);
    packet = r_u32(scene + 16);
    ClearOTagR(psx_addr(packet, 256), 64);
    scene = r_u32(0x800BCE54);
    state = r_u32(0x80112DC4);
    packet = r_u32(scene + 16);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 12);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    packet = r_u32(scene + 16);
    ordering = r_u32(state + 8);
    link = r_u32(ordering + 12);
    w_u32(ordering + 12, (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF));
    resource002_sub_801085f8(0);
    return resource002_sub_80108874(1);
}

uint32 resource002_sub_801093e8(void)
{
    uint32 owner, index, game, context, resource, entries, temporary, cached_context, table, source, object, scene, model, value;
    FUNCTION_MARKER(0x801093E8, "ACE3.BIN");
    owner = r_u32(0x80112E30);
    index = r_u8(owner + 0x4000);
    if (r_s16(0x80112C08 + index * 8 + 4) == 0)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + 1, 5);
        return resource002_sub_80108478(0);
    }
    game = r_u32(0x800BCE60);
    value = r_u32(game + 68);
    if (value)
        return value;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    w_u32(owner + 0x4004, resource);
    entries = sub_80027338(resource, NULL);
    source = r_u32(entries + 12);
    cached_context = r_u32(0x800BCE50);
    arena_set_cursor(source);
    resource002_sub_800ece38(r_u32(entries + 8), 0x10000);
    arena_set_cursor(r_u32(entries + 8));
    temporary = arena_alloc(0x10000);
    sub_800afd08(r_u32(entries), temporary);
    table = sub_80027338(temporary, NULL);
    w_u32(cached_context + 8, table);
    index = 0;
    if (r_s32(temporary) > 0)
        do
        {
            table = r_u32(cached_context + 8);
            object = r_u32(table + index * 4);
            relocate_pointer_array(object, object + 4, 6);
            value = r_u32(temporary);
            ++index;
        } while ((sint32)index < (sint32)value);
    value = r_u32(entries + 4);
    context = r_u32(0x800BCE50);
    w_u32(cached_context + 12, value);
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    model = r_u32(scene + 592);
    source = r_u32(context + 8);
    w_u32(model + 12, source);
    object = r_u32(table);
    value = r_u32(object);
    w_u32(object, value | 0x1C000);
    object = r_u32(table + 8);
    w_u32(object, 0);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 1, 5);
    return 5;
}

uint32 resource002_sub_80109588(void)
{
    uint32 scene, packet, state, ordering, tag, link, game, owner;
    FUNCTION_MARKER(0x80109588, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    packet = r_u32(scene + 16);
    ClearOTagR(psx_addr(packet, 256), 64);
    scene = r_u32(0x800BCE54);
    state = r_u32(0x80112DC4);
    packet = r_u32(scene + 16);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 12);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 8);
    packet = r_u32(scene + 16);
    game = r_u32(0x800BCE60);
    link = r_u32(ordering + 12);
    w_u32(ordering + 12, (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF));
    if ((r_u32(game + 300) & 32) && (r_u32(game + 296) & 0x5F) == 0)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + 1, 6);
        return 6;
    }
    resource002_sub_801085f8(1);
    return resource002_sub_80108874(0);
}

uint32 resource002_sub_80109660(void)
{
    uint32 owner, index, table, object, value, state, phase;
    FUNCTION_MARKER(0x80109660, "ACE3.BIN");
    resource002_sub_800dedf0(2, 0x100000);
    owner = r_u32(0x80112E30);
    index = r_u8(owner + 0x4000);
    if (r_s16(0x80112C08 + index * 8 + 4))
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table);
        w_u32(object, 0);
        object = r_u32(table + 8);
        value = r_u32(object);
        w_u32(object, value | 0x1C000);
        arena_set_cursor(r_u32(owner + 0x4004));
        state = r_u32(0x80112DC4);
        w_u32(state + 252, 0xFFF00000);
        w_u16(state + 256, 3072);
        w_u16(state + 258, 10240);
        w_u16(state + 260, 0);
        resource002_sub_80108298();
        phase = 1;
    }
    else
    {
        static const uint8 slots[] = {2, 1, 3, 11, 5, 8};
        for (index = 0; index < 6; ++index)
            resource002_sub_800dee08(slots[index], 1, 0, 25);
        phase = 2;
    }
    owner = r_u32(0x80112DDC);
    w_u8(owner + 1, phase);
    return phase;
}

uint32 resource002_sub_800f8fa8(uint32 packet, uint32 ordering)
{
    PSX_RECT rectangle;
    uint8 descriptor[12];
    uint32 game, owner, old_primary, old_secondary, current_primary, current_secondary;
    uint32 primary_timer, secondary_timer, primary_phase, secondary_phase, slide_timer, slide_phase;
    uint32 cursor, next, tag, link, clut, offset, index, first, second, value, other, scene, stats, result;
    uint32 slide = 0;
    sint32 clip_x;
    FUNCTION_MARKER(0x800F8FA8, "ACE3.BIN");
    if (resource002_sub_800dee84(8) == 0)
        return packet;
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    first = (uint32)r_s16(game + 908);
    second = (uint32)r_s16(game + 910);
    index = r_u8(owner + 20);
    old_primary = (uint8)resource002_sub_800febb8(first, second, 0, index);
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    first = (uint32)r_s16(game + 908);
    second = (uint32)r_s16(game + 910);
    index = r_u8(owner + 21);
    old_secondary = (uint8)resource002_sub_800febb8(first, second, 2, index);
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    first = (uint32)r_s16(game + 908);
    second = (uint32)r_s16(game + 910);
    index = r_u8(owner + 22);
    current_primary = resource002_sub_800febb8(first, second, 0, index);
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112E10);
    first = (uint32)r_s16(game + 908);
    second = (uint32)r_s16(game + 910);
    index = r_u8(owner + 23);
    current_secondary = (uint8)resource002_sub_800febb8(first, second, 2, index);
    primary_timer = resource002_sub_800deea8(9);
    secondary_timer = resource002_sub_800deea8(10);
    primary_phase = resource002_sub_800dee84(9);
    secondary_phase = resource002_sub_800dee84(10);
    slide_timer = resource002_sub_800deea8(8);
    slide_phase = resource002_sub_800dee84(8);
    cursor = packet;
    next = packet + 12;
    value = r_u32(0x800CD398);
    other = r_u32(0x800CD39C);
    xport_store_le32((uint8 *)&rectangle, value);
    xport_store_le32((uint8 *)&rectangle + 4, other);
    SetDrawArea((DR_AREA *)psx_addr(cursor, 12), &rectangle);
    tag = r_u32(cursor);
    link = r_u32(ordering + 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    if (slide_phase == 1 || slide_phase == 3)
    {
        value = 25 - slide_timer;
        value = value * (value * 325);
        slide = (uint32)((sint32)value / 625);
    }
    // Define the original uninitialized slide for phases outside 1, 2 and 3
    clut = ((((current_primary & 255) >> 3) + 24) << 6) | 0x1B;
    cursor = next;
    next += 20;
    offset = (uint32)((sint32)(primary_timer * 48) / 10);
    if (primary_phase != 3)
    {
        w_u8(cursor + 3, 4);
        w_u8(cursor + 7, 102);
        w_u8(cursor + 4, 128);
        w_u8(cursor + 5, 128);
        w_u8(cursor + 6, 128);
        w_u16(cursor + 16, 48);
        w_u16(cursor + 18, 88);
        w_u16(cursor + 14, clut);
        w_u16(cursor + 10, 68);
        w_u8(cursor + 12, 48 * (old_primary & 3));
        w_u8(cursor + 13, 88 * ((old_primary >> 2) & 1) + 10);
        tag = r_u32(cursor);
        w_u16(cursor + 8, offset + slide + 234);
        link = r_u32(ordering + 4);
        w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering + 4);
        w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
        cursor = next;
        next += 20;
        w_u8(cursor + 12, 48 * (current_primary & 3));
        w_u8(cursor + 3, 4);
        tag = r_u32(cursor);
        w_u8(cursor + 7, 102);
        w_u8(cursor + 4, 128);
        w_u8(cursor + 5, 128);
        w_u8(cursor + 6, 128);
        w_u16(cursor + 16, 48);
        w_u16(cursor + 18, 88);
        w_u16(cursor + 14, clut);
        w_u16(cursor + 8, offset + slide + 186);
        w_u16(cursor + 10, 68);
        w_u8(cursor + 13, 88 * ((current_primary >> 2) & 1) + 10);
    }
    else
    {
        w_u8(cursor + 12, 48 * (current_primary & 3));
        w_u8(cursor + 13, 88 * ((current_primary >> 2) & 1) + 10);
        w_u8(cursor + 3, 4);
        tag = r_u32(cursor);
        w_u8(cursor + 7, 102);
        w_u8(cursor + 4, 128);
        w_u8(cursor + 5, 128);
        w_u8(cursor + 6, 128);
        w_u16(cursor + 16, 48);
        w_u16(cursor + 18, 88);
        w_u16(cursor + 14, clut);
        w_u16(cursor + 10, 68);
        w_u16(cursor + 8, offset + slide + 234);
        link = r_u32(ordering + 4);
        w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering + 4);
        w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
        cursor = next;
        next += 20;
        w_u8(cursor + 3, 4);
        w_u8(cursor + 7, 102);
        w_u8(cursor + 4, 128);
        w_u8(cursor + 5, 128);
        w_u8(cursor + 6, 128);
        w_u16(cursor + 16, 48);
        w_u16(cursor + 18, 88);
        w_u16(cursor + 14, clut);
        w_u16(cursor + 8, offset + slide + 186);
        w_u16(cursor + 10, 68);
        w_u8(cursor + 12, 48 * (old_primary & 3));
        tag = r_u32(cursor);
        w_u8(cursor + 13, 88 * ((old_primary >> 2) & 1) + 10);
    }
    link = r_u32(ordering + 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    cursor = next;
    next += 12;
    rectangle.x = (sint16)(slide + 234);
    rectangle.y = 68;
    rectangle.w = 48;
    rectangle.h = 88;
    clip_x = rectangle.x;
    if (clip_x + 48 >= 320)
        rectangle.w = (sint16)(320 - (slide + 234));
    if (clip_x < 320)
    {
        SetDrawArea((DR_AREA *)psx_addr(cursor, 12), &rectangle);
        tag = r_u32(cursor);
        link = r_u32(ordering + 4);
        w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering + 4);
        w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    }
    cursor = next;
    next += 20;
    first = secondary_phase == 3 ? current_secondary : old_secondary;
    second = secondary_phase == 3 ? old_secondary : current_secondary;
    w_u8(cursor + 3, 4);
    w_u8(cursor + 7, 102);
    w_u8(cursor + 4, 128);
    w_u8(cursor + 5, 128);
    w_u8(cursor + 6, 128);
    w_u16(cursor + 16, 48);
    w_u16(cursor + 18, 88);
    w_u16(cursor + 10, 184);
    w_u16(cursor + 14, (((first >> 3) + 24) << 6) | 0x1B);
    offset = (uint32)((sint32)(secondary_timer * 48) / 10);
    tag = r_u32(cursor);
    clut = (((second >> 3) + 24) << 6) | 0x1B;
    value = 48 * (first & 3);
    if (secondary_phase == 3)
        value += slide;
    w_u8(cursor + 12, value);
    w_u8(cursor + 13, 88 * ((first >> 2) & 1) + 10);
    w_u16(cursor + 8, offset + slide + 234);
    link = r_u32(ordering + 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    cursor = next;
    next += 20;
    value = 48 * (second & 3);
    if (secondary_phase == 3)
        value += slide;
    w_u8(cursor + 3, 4);
    w_u8(cursor + 7, 102);
    w_u8(cursor + 4, 128);
    w_u8(cursor + 5, 128);
    w_u8(cursor + 6, 128);
    w_u16(cursor + 16, 48);
    w_u16(cursor + 18, 88);
    w_u16(cursor + 14, clut);
    w_u16(cursor + 8, offset + slide + 186);
    w_u16(cursor + 10, 184);
    w_u8(cursor + 12, value);
    tag = r_u32(cursor);
    w_u8(cursor + 13, 88 * ((second >> 2) & 1) + 10);
    link = r_u32(ordering + 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    cursor = next;
    next += 12;
    rectangle.y = 184;
    rectangle.w = 48;
    rectangle.h = 88;
    clip_x = (sint16)(slide + 234);
    rectangle.x = (sint16)(slide + 234);
    if (clip_x + 48 >= 320)
        rectangle.w = (sint16)(320 - (slide + 234));
    if (clip_x < 320)
    {
        SetDrawArea((DR_AREA *)psx_addr(cursor, 12), &rectangle);
        tag = r_u32(cursor);
        link = r_u32(ordering + 4);
        w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering + 4);
        w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    }
    cursor = next;
    next += 12;
    SetDrawMode(psx_addr(cursor, 12), 0, 0, 76, NULL);
    tag = r_u32(cursor);
    link = r_u32(ordering + 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    owner = r_u32(0x80112E10);
    w_u32(ordering + 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    index = r_u8(owner + 281);
    descriptor[8] = 229;
    descriptor[9] = 229;
    descriptor[10] = 213;
    descriptor[11] = 0;
    xport_store_le16(descriptor + 2, 314);
    xport_store_le16(descriptor, slide + 266);
    stats = 0x8010EAC0 + index * 4;
    value = r_u8(stats);
    scene = r_u32(0x800BCE54);
    xport_store_le16(descriptor + 6, 8);
    xport_store_le16(descriptor + 4, value * 38 / 15);
    result = resource002_sub_800f7180(0, r_u32(scene + 20), ordering + 4, descriptor);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    xport_store_le16(descriptor, slide + 266);
    xport_store_le16(descriptor + 2, 333);
    value = r_u8(stats + 1);
    xport_store_le16(descriptor + 6, 8);
    xport_store_le16(descriptor + 4, value * 38 / 15);
    result = resource002_sub_800f7180(0, r_u32(scene + 20), ordering + 4, descriptor);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    xport_store_le16(descriptor, slide + 266);
    xport_store_le16(descriptor + 2, 352);
    value = r_u8(stats + 2);
    xport_store_le16(descriptor + 6, 8);
    xport_store_le16(descriptor + 4, value * 38 / 15);
    result = resource002_sub_800f7180(0, r_u32(scene + 20), ordering + 4, descriptor);
    scene = r_u32(0x800BCE54);
    owner = r_u32(0x80112DC0);
    w_u32(scene + 20, result);
    cursor = r_u32(owner + 92);
    value = r_u8(stats + 3);
    w_u8(cursor + 12, value);
    owner = r_u32(0x80112DC0);
    game = r_u32(0x80112E10);
    cursor = r_u32(owner + 92);
    value = r_u8(game + 281);
    w_u8(cursor + 13, value);
    return next;
}

uint32 resource002_sub_800f7ea4(uint32 packet)
{
    FUNCTION_MARKER(0x800F7EA4, "ACE3.BIN");
    return resource002_sub_800f7eac(packet, r_u32(0x80112E10));
}

uint32 resource002_sub_800f7eac(uint32 packet, uint32 owner)
{
    static const uint8 fan[] = {1, 2, 5, 3, 4, 1};
    static const uint8 number_slots[] = {1, 0, 2};
    uint16 line[6], points[6][2];
    uint8 triangle[28], argument[4];
    uint32 current, previous, timer, delta, slide, value, old[5], fresh[5], levels[5];
    uint32 old_number[5], fresh_number[5], number[5], index, target, state, ordering, text_x, text_right, mode, quotient;
    uint32 x_product[5], y_product[5], origin_x, origin_y;
    FUNCTION_MARKER(0x800F7EAC, "ACE3.BIN");
    if (r_u32(owner + 16) == 0)
        return packet;
    current = r_u32(owner + 8);
    previous = r_u32(owner + 12);
    timer = resource002_sub_800deea8(6);
    if ((sint32)timer >= 21)
        timer = 20;
    delta = 20 - timer;
    value = delta * delta * 325;
    slide = (uint32)((sint32)value / 400);
    if (resource002_sub_800dee84(2) == 1)
    {
        (void)r_u8(current);
        (void)r_u8(current + 18);
        (void)r_u8(current + 19);
        (void)r_u8(current + 20);
        timer = resource002_sub_800deea8(2);
        old[0] = r_u8(previous + 1);
        fresh[0] = r_u8(current + 1);
        old[1] = r_u8(previous + 2);
        fresh[1] = r_u8(current + 2);
        old[2] = r_u8(previous + 3);
        fresh[2] = r_u8(current + 3);
        old[3] = r_u8(previous + 4);
        fresh[3] = r_u8(current + 4);
        old_number[0] = (uint32)r_s16(previous + 6);
        fresh_number[0] = (uint32)r_s16(current + 6);
        old[4] = r_u8(previous + 5);
        fresh[4] = r_u8(current + 5);
        old_number[1] = (uint32)r_s16(previous + 8);
        fresh_number[1] = (uint32)r_s16(current + 8);
        old_number[2] = (uint32)r_s16(previous + 10);
        fresh_number[2] = (uint32)r_s16(current + 10);
        fresh_number[3] = r_u32(current + 12);
        old_number[3] = r_u32(previous + 12);
        old_number[4] = (uint32)r_s16(previous + 16);
        fresh_number[4] = (uint32)r_s16(current + 16);
        for (index = 0; index < 5; ++index)
        {
            levels[index] = old[index] * 25 + (fresh[index] - old[index]) * timer;
            value = (fresh_number[index] - old_number[index]) * timer;
            number[index] = old_number[index] + (uint32)((sint32)value / 25);
        }
    }
    else
    {
        number[0] = (uint32)r_s16(current + 6);
        number[1] = (uint32)r_s16(current + 8);
        number[2] = (uint32)r_s16(current + 10);
        number[3] = r_u32(current + 12);
        levels[0] = r_u8(current + 1) * 25;
        number[4] = (uint32)r_s16(current + 16);
        levels[1] = r_u8(current + 2) * 25;
        levels[2] = r_u8(current + 3) * 25;
        levels[3] = r_u8(current + 4) * 25;
        levels[4] = r_u8(current + 5) * 25;
    }
    x_product[0] = (uint32)((sint16)levels[0] * (sint32)r_s16(0x8010EB28));
    y_product[0] = (uint32)((sint16)levels[0] * (sint32)r_s16(0x8010EB2A));
    x_product[1] = (uint32)((sint16)levels[1] * (sint32)r_s16(0x8010EB2C));
    y_product[1] = (uint32)((sint16)levels[1] * (sint32)r_s16(0x8010EB2E));
    x_product[2] = (uint32)((sint16)levels[2] * (sint32)r_s16(0x8010EB34));
    y_product[2] = (uint32)((sint16)levels[2] * (sint32)r_s16(0x8010EB36));
    x_product[3] = (uint32)((sint16)levels[3] * (sint32)r_s16(0x8010EB38));
    y_product[3] = (uint32)((sint16)levels[3] * (sint32)r_s16(0x8010EB3A));
    origin_x = r_u16(0x8010EB24);
    origin_y = r_u16(0x8010EB26);
    x_product[4] = (uint32)((sint16)levels[4] * (sint32)r_s16(0x8010EB30));
    y_product[4] = (uint32)((sint16)levels[4] * (sint32)r_s16(0x8010EB32));
    points[0][0] = (uint16)(origin_x + 236);
    points[0][1] = (uint16)(origin_y + 308);
    for (index = 0; index < 5; ++index)
    {
        points[index + 1][0] = (uint16)((uint32)((sint32)x_product[index] / 375) + 236);
        points[index + 1][1] = (uint16)((uint32)((sint32)y_product[index] / 375) + 308);
    }
    for (index = 1; index < 6; ++index)
    {
        line[4] = 0x4040;
        line[5] = 0x0040;
        line[0] = (uint16)(r_u16(0x8010EB24) + slide + 236);
        line[1] = (uint16)(r_u16(0x8010EB26) + 308);
        line[2] = (uint16)(r_u16(0x8010EB24 + index * 4) + slide + 236);
        value = r_u16(0x8010EB26 + index * 4);
        state = r_u32(0x80112DC4);
        line[3] = (uint16)(value + 308);
        ordering = r_u32(state + 8);
        packet = resource002_sub_800f6e68(0, packet, ordering, line);
    }
    for (index = 0; index < 5; ++index)
    {
        line[0] = (uint16)(r_u16(0x8010EB28 + index * 4) + slide + 236);
        line[1] = (uint16)(r_u16(0x8010EB2A + index * 4) + 308);
        target = (index + 1) % 5;
        line[2] = (uint16)(r_u16(0x8010EB28 + target * 4) + slide + 236);
        value = r_u16(0x8010EB2A + target * 4);
        state = r_u32(0x80112DC4);
        line[3] = (uint16)(value + 308);
        ordering = r_u32(state + 8);
        packet = resource002_sub_800f6e68(0, packet, ordering, line);
    }
    owner = r_u32(0x80112E10);
    if (r_u32(owner + 16) & 2)
    {
        for (index = 0; index < 3; ++index)
        {
            triangle[16 + index * 4] = 207;
            triangle[17 + index * 4] = 139;
            triangle[18 + index * 4] = 23;
        }
        triangle[19] = 0;
        for (index = 0; index < 5; ++index)
        {
            xport_store_le16(triangle, points[0][0] + slide);
            xport_store_le16(triangle + 2, points[0][1]);
            xport_store_le16(triangle + 4, points[fan[index]][0] + slide);
            xport_store_le16(triangle + 6, points[fan[index]][1]);
            xport_store_le16(triangle + 8, points[fan[index + 1]][0] + slide);
            state = r_u32(0x80112DC4);
            xport_store_le16(triangle + 10, points[fan[index + 1]][1]);
            ordering = r_u32(state + 8);
            packet = resource002_sub_800f73a4(0, packet, ordering, triangle);
        }
    }
    owner = r_u32(0x80112E10);
    if (r_u32(owner + 16) & 1)
    {
        delta = 25 - resource002_sub_800deea8(1);
        value = 0 - delta * delta * 325;
        slide = (uint32)((sint32)value / 625);
        resource002_sub_800f35b8(7);
        resource002_sub_800f33f8(0xFFFFFFFF);
        resource002_sub_800f34c8(1, 0, 1);
        resource002_sub_800f34b0(1);
        mode = resource002_sub_800dee60(1);
        if (mode == 1)
            resource002_sub_800f33d0(225, 95, 10);
        else if (resource002_sub_800dee60(1) == 0)
            resource002_sub_800f33d0(229, 229, 213);
        text_x = slide + 70;
        text_right = slide + 91;
        for (index = 0; index < 3; ++index)
        {
            resource002_sub_800f33a8(text_x, 258 + index * 11);
            value = number[number_slots[index]];
            quotient = (uint32)((sint32)value / 100);
            xport_store_le32(argument, quotient);
            resource002_sub_800f4ae0(0x800CD384, 0, argument, sizeof(argument));
            resource002_sub_800f33a8(text_right, 258 + index * 11);
            xport_store_le32(argument, value - quotient * 100);
            resource002_sub_800f4b40(0x800CD38C, 0, argument, sizeof(argument));
        }
        resource002_sub_800f33a8(text_x, 291);
        xport_store_le32(argument, number[3]);
        resource002_sub_800f4ae0(0x800CD390, 0, argument, sizeof(argument));
        resource002_sub_800f33a8(slide + 60, 302);
        xport_store_le32(argument, number[4]);
        resource002_sub_800f4ae0(0x800CD390, 0, argument, sizeof(argument));
    }
    mode = resource002_sub_800dee60(1);
    value = r_u8(current + 19);
    resource002_sub_800def94(6, value);
    resource002_sub_800def94(1, 1);
    value = r_u8(current + 18);
    resource002_sub_800def94(5, value + mode * 3);
    return packet;
}

static void resource002_map_color_pair(uint32 first, uint32 next_first, uint32 second, uint32 next_second, uint32 weight, uint32 adjustment)
{
    uint32 red, green, blue, alpha, other, factor, product, color, second_red, second_product;
    red = r_u8(first);
    other = r_u8(next_first);
    factor = (uint32)r_s16(weight) + adjustment;
    product = (other - red) * factor;
    color = red + (uint32)((sint32)product / 128);
    green = r_u8(first + 1);
    other = r_u8(next_first + 1);
    product = (other - green) * factor;
    color |= (green + (uint32)((sint32)product / 128)) << 8;
    blue = r_u8(first + 2);
    other = r_u8(next_first + 2);
    product = (other - blue) * factor;
    second_red = r_u8(second);
    other = r_u8(next_second);
    factor = (uint32)r_s16(weight) + adjustment;
    second_product = (other - second_red) * factor;
    color |= (blue + (uint32)((sint32)product / 128)) << 16;
    alpha = r_u8(first + 3);
    w_u32(first, color | (alpha << 24));
    color = second_red + (uint32)((sint32)second_product / 128);
    green = r_u8(second + 1);
    other = r_u8(next_second + 1);
    product = (other - green) * factor;
    color |= (green + (uint32)((sint32)product / 128)) << 8;
    blue = r_u8(second + 2);
    other = r_u8(next_second + 2);
    product = (other - blue) * factor;
    color |= (blue + (uint32)((sint32)product / 128)) << 16;
    alpha = r_u8(second + 3);
    w_u32(second, color | (alpha << 24));
}

uint32 resource002_sub_801048f0(uint32 packet)
{
    PSX_RECT rectangle;
    uint16 coordinates[8];
    const uint32 scratch = 0x1F800000;
    uint32 owner, state, ordering, tag, link, value, other, third, base, map, parity, sample, cursor;
    uint32 first, second, factor, first_product, second_product, first_height, second_height, depth;
    uint32 height, next_height, color, time;
    sint32 row, column;
    FUNCTION_MARKER(0x801048F0, "ACE3.BIN");
    value = r_u32(0x800CD774);
    other = r_u32(0x800CD778);
    xport_store_le32((uint8 *)&rectangle, value);
    xport_store_le32((uint8 *)&rectangle + 4, other);
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 20);
    tag = r_u32(packet);
    link = r_u32(ordering + 1020);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 20);
    link = r_u32(ordering + 1020);
    cursor = packet + 12;
    w_u32(ordering + 1020, (link & 0xFF000000) | (packet & 0xFFFFFF));
    VSync(1);
    owner = r_u32(0x80112DAC);
    value = r_u8(owner + 2516);
    if (!value)
        value = (sint32)r_u32(r_u32(0x80112DC4) + 80) >= 83;
    if (value)
    {
        base = owner + 80;
        value = r_u32(base);
        other = r_u32(base + 4);
        xport_gte_write_control(0, value);
        xport_gte_write_control(1, other);
        value = r_u32(base + 8);
        other = r_u32(base + 12);
        third = r_u32(base + 16);
        xport_gte_write_control(2, value);
        xport_gte_write_control(3, other);
        xport_gte_write_control(4, third);
        value = r_u32(base + 20);
        other = r_u32(base + 24);
        xport_gte_write_control(5, value);
        third = r_u32(base + 28);
        xport_gte_write_control(6, other);
        xport_gte_write_control(7, third);
        for (row = -8; row < 9; ++row)
        {
            owner = r_u32(0x80112DAC);
            value = r_u32(owner + 40);
            w_u16(scratch + 2, (uint32)row + (uint32)((sint32)value / 128));
            value = r_u32(owner + 40);
            w_u16(scratch + 6, value - (uint32)((sint32)value / 128) * 128);
            value = r_u32(owner + 40);
            if ((sint32)value < 0)
            {
                value = r_u16(scratch + 2);
                other = r_u16(scratch + 6);
                w_u16(scratch + 2, value - 1);
                w_u16(scratch + 6, other + 128);
            }
            for (column = -8; column < 9; ++column)
            {
                owner = r_u32(0x80112DAC);
                value = r_u32(owner + 32);
                other = (uint32)((sint32)value / 128);
                w_u16(scratch, (uint32)column + other);
                w_u16(scratch + 4, value - other * 128);
                if ((sint32)value < 0)
                {
                    value = r_u16(scratch);
                    other = r_u16(scratch + 4);
                    w_u16(scratch, value - 1);
                    w_u16(scratch + 4, other + 128);
                }
                packet = cursor;
                cursor += 36;
                if (column == -8)
                {
                    value = r_u16(scratch + 2);
                    base = ((uint32)(sint32)(sint16)value + 128) * 258 + 128;
                    base += (uint32)r_s16(scratch);
                    map = r_u32(0x80112DC8);
                    parity = r_u16(scratch) & 1;
                    value = (uint32)r_s16(scratch);
                    sample = r_u8(map + (uint32)((sint32)base / 2));
                    height = ((sample >> (parity * 4)) & 15) << 7;
                    w_u16(scratch + 32, value << 7);
                    w_u16(scratch + 8, height);
                    value = r_u16(scratch + 2);
                    w_u16(scratch + 34, 0 - height);
                    w_u16(scratch + 36, (uint32)(sint32)(sint16)value << 7);
                }
                else
                {
                    value = r_u32(scratch + 40);
                    other = r_u32(scratch + 44);
                    height = r_u16(scratch + 10);
                    w_u32(scratch + 32, value);
                    w_u32(scratch + 36, other);
                    w_u16(scratch + 8, height);
                }
                value = r_u16(scratch + 2);
                first = (uint32)r_s16(scratch);
                second = (uint32)r_s16(scratch);
                base = ((uint32)(sint32)(sint16)value + 128) * 258 + 129 + first;
                map = r_u32(0x80112DC8);
                value = r_u16(scratch + 2);
                sample = r_u8(map + (uint32)((sint32)base / 2));
                height = ((sample >> (((first + 1) & 1) * 4)) & 15) << 7;
                w_u16(scratch + 10, height);
                base = ((uint32)(sint32)(sint16)value + 129) * 258 + 128;
                first = (uint32)r_s16(scratch);
                value = r_u16(scratch + 2);
                base += first;
                other = r_u16(scratch);
                sample = r_u8(map + (uint32)((sint32)base / 2));
                height = ((sample >> ((other & 1) * 4)) & 15) << 7;
                base = ((uint32)(sint32)(sint16)value + 129) * 258 + 129 + second;
                w_u16(scratch + 12, height);
                sample = r_u8(map + (uint32)((sint32)base / 2));
                w_u16(scratch + 40, (second + 1) << 7);
                height = r_u16(scratch + 10);
                value = r_u16(scratch + 2);
                w_u16(scratch + 42, 0 - height);
                w_u16(scratch + 48, second << 7);
                w_u16(scratch + 44, (uint32)(sint32)(sint16)value << 7);
                value = r_u16(scratch + 2);
                w_u16(scratch + 56, (second + 1) << 7);
                height = ((sample >> (((second + 1) & 1) * 4)) & 15) << 7;
                w_u16(scratch + 14, height);
                w_u16(scratch + 52, ((uint32)(sint32)(sint16)value << 7) + 128);
                value = r_u16(scratch + 2);
                w_u16(scratch + 60, ((uint32)(sint32)(sint16)value << 7) + 128);
                other = r_u16(scratch + 12);
                w_u16(scratch + 58, 0 - height);
                w_u16(scratch + 50, 0 - other);
                if (column == -8)
                {
                    first = r_u16(scratch + 10);
                    first_height = r_u16(scratch + 8);
                    factor = r_u16(scratch + 4);
                    first_product = ((uint32)(sint32)(sint16)first - (uint32)(sint32)(sint16)first_height) * (uint32)(sint32)(sint16)factor;
                    value = r_u16(scratch + 32);
                    factor = r_u16(scratch + 4);
                    other = r_u16(scratch + 48);
                    w_u16(scratch + 32, value + factor);
                    w_u16(scratch + 48, other + factor);
                    second = r_u16(scratch + 14);
                    second_height = r_u16(scratch + 12);
                    factor = r_u16(scratch + 4);
                    second_product = ((uint32)(sint32)(sint16)second - (uint32)(sint32)(sint16)second_height) * (uint32)(sint32)(sint16)factor;
                    w_u16(scratch + 8, first_height + (uint32)((sint32)first_product / 128));
                    w_u16(scratch + 12, second_height + (uint32)((sint32)second_product / 128));
                    value = r_u16(scratch + 8);
                    other = r_u16(scratch + 12);
                    w_u16(scratch + 34, 0 - value);
                    w_u16(scratch + 50, 0 - other);
                }
                if (column == 8)
                {
                    first_height = r_u16(scratch + 10);
                    first = r_u16(scratch + 8);
                    factor = r_u16(scratch + 4);
                    first_product = ((uint32)(sint32)(sint16)first_height - (uint32)(sint32)(sint16)first) * ((uint32)(sint32)(sint16)factor - 128);
                    value = r_u16(scratch + 40);
                    factor = r_u16(scratch + 4);
                    other = r_u16(scratch + 56);
                    w_u16(scratch + 40, value - 128 + factor);
                    factor = r_u16(scratch + 4);
                    w_u16(scratch + 56, other - 128 + factor);
                    second_height = r_u16(scratch + 14);
                    second = r_u16(scratch + 12);
                    factor = r_u16(scratch + 4);
                    second_product = ((uint32)(sint32)(sint16)second_height - (uint32)(sint32)(sint16)second) * ((uint32)(sint32)(sint16)factor - 128);
                    w_u16(scratch + 10, first_height + (uint32)((sint32)first_product / 128));
                    w_u16(scratch + 14, second_height + (uint32)((sint32)second_product / 128));
                    value = r_u16(scratch + 10);
                    other = r_u16(scratch + 14);
                    w_u16(scratch + 42, 0 - value);
                    w_u16(scratch + 58, 0 - other);
                }
                if (row == -8)
                {
                    first = r_u16(scratch + 12);
                    first_height = r_u16(scratch + 8);
                    factor = r_u16(scratch + 6);
                    first_product = ((uint32)(sint32)(sint16)first - (uint32)(sint32)(sint16)first_height) * (uint32)(sint32)(sint16)factor;
                    value = r_u16(scratch + 36);
                    factor = r_u16(scratch + 6);
                    other = r_u16(scratch + 44);
                    w_u16(scratch + 36, value + factor);
                    w_u16(scratch + 44, other + factor);
                    second = r_u16(scratch + 14);
                    second_height = r_u16(scratch + 10);
                    factor = r_u16(scratch + 6);
                    second_product = ((uint32)(sint32)(sint16)second - (uint32)(sint32)(sint16)second_height) * (uint32)(sint32)(sint16)factor;
                    w_u16(scratch + 8, first_height + (uint32)((sint32)first_product / 128));
                    w_u16(scratch + 10, second_height + (uint32)((sint32)second_product / 128));
                    value = r_u16(scratch + 8);
                    other = r_u16(scratch + 10);
                    w_u16(scratch + 34, 0 - value);
                    w_u16(scratch + 42, 0 - other);
                }
                if (row == 8)
                {
                    first_height = r_u16(scratch + 12);
                    first = r_u16(scratch + 8);
                    factor = r_u16(scratch + 6);
                    first_product = ((uint32)(sint32)(sint16)first_height - (uint32)(sint32)(sint16)first) * ((uint32)(sint32)(sint16)factor - 128);
                    value = r_u16(scratch + 52);
                    factor = r_u16(scratch + 6);
                    other = r_u16(scratch + 60);
                    w_u16(scratch + 52, value - 128 + factor);
                    factor = r_u16(scratch + 6);
                    w_u16(scratch + 60, other - 128 + factor);
                    second_height = r_u16(scratch + 14);
                    second = r_u16(scratch + 10);
                    factor = r_u16(scratch + 6);
                    second_product = ((uint32)(sint32)(sint16)second_height - (uint32)(sint32)(sint16)second) * ((uint32)(sint32)(sint16)factor - 128);
                    w_u16(scratch + 12, first_height + (uint32)((sint32)first_product / 128));
                    w_u16(scratch + 14, second_height + (uint32)((sint32)second_product / 128));
                    value = r_u16(scratch + 12);
                    other = r_u16(scratch + 14);
                    w_u16(scratch + 50, 0 - value);
                    w_u16(scratch + 58, 0 - other);
                }
                if (column == -8)
                {
                    xport_gte_write_data(0, r_u32(scratch + 32));
                    xport_gte_write_data(1, r_u32(scratch + 36));
                    xport_gte_execute(0x180001);
                    w_u32(packet + 8, xport_gte_read_data(14));
                }
                else
                    w_u32(packet + 8, xport_gte_read_data(12));
                xport_gte_write_data(0, r_u32(scratch + 40));
                xport_gte_write_data(1, r_u32(scratch + 44));
                xport_gte_write_data(2, r_u32(scratch + 48));
                xport_gte_write_data(3, r_u32(scratch + 52));
                xport_gte_write_data(4, r_u32(scratch + 56));
                xport_gte_write_data(5, r_u32(scratch + 60));
                xport_gte_execute(0x280030);
                height = r_u16(scratch + 8);
                owner = r_u32(0x80112DAC);
                next_height = r_u16(scratch + 10);
                color = r_u32(owner + 2196 + (uint32)((sint16)height / 128) * 4);
                w_u32(scratch + 16, color);
                height = r_u16(scratch + 12);
                color = r_u32(owner + 2196 + (uint32)((sint16)next_height / 128) * 4);
                w_u32(scratch + 20, color);
                next_height = r_u16(scratch + 14);
                color = r_u32(owner + 2196 + (uint32)((sint16)height / 128) * 4);
                w_u32(scratch + 24, color);
                color = r_u32(owner + 2196 + (uint32)((sint16)next_height / 128) * 4);
                w_u32(scratch + 28, color);
                w_u32(packet + 16, xport_gte_read_data(12));
                w_u32(packet + 24, xport_gte_read_data(13));
                w_u32(packet + 32, xport_gte_read_data(14));
                depth = xport_gte_read_data(19);
                depth = (uint32)((sint32)depth >> 2);
                if (column == -8)
                    resource002_map_color_pair(scratch + 16, scratch + 20, scratch + 24, scratch + 28, scratch + 4, 0);
                if (row == -8)
                    resource002_map_color_pair(scratch + 16, scratch + 24, scratch + 20, scratch + 28, scratch + 6, 0);
                if (column == 8)
                    resource002_map_color_pair(scratch + 20, scratch + 16, scratch + 28, scratch + 24, scratch + 4, 0 - 128);
                if (row == 8)
                    resource002_map_color_pair(scratch + 24, scratch + 16, scratch + 28, scratch + 20, scratch + 6, 0 - 128);
                value = r_u32(scratch + 16);
                w_u32(packet + 4, value | 0x38000000);
                value = r_u32(scratch + 20);
                w_u32(packet + 12, value);
                value = r_u32(scratch + 24);
                w_u32(packet + 20, value);
                value = r_u32(scratch + 28);
                w_u32(packet + 28, value);
                state = r_u32(0x80112DC4);
                ordering = r_u32(state + 20);
                other = ((uint32)((sint32)depth >> 6)) << 2;
                value = r_u32(ordering + other);
                w_u32(packet, value | 0x08000000);
                ordering = r_u32(state + 20);
                w_u32(ordering + other, packet & 0xFFFFFF);
            }
        }
    }
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2516))
        return cursor;
    state = r_u32(0x80112DC4);
    time = r_u32(state + 80);
    if ((sint32)time < 60)
        return cursor;
    if ((sint32)time < 66)
    {
        coordinates[4] = coordinates[0] = (uint16)(154 - (time - 60) * 8);
        value = r_u32(state + 80);
        coordinates[6] = coordinates[2] = (uint16)(value * 8 - 326);
        value = r_u32(state + 364);
        value = r_u16(value + 66);
        coordinates[3] = coordinates[1] = (uint16)(value + 153);
        value = r_u32(state + 364);
        value = r_u16(value + 66);
        value += 155;
    }
    else if ((sint32)time < 82)
    {
        coordinates[4] = coordinates[0] = 96;
        coordinates[6] = coordinates[2] = 224;
        value = r_u32(state + 364);
        other = r_u16(value + 66);
        value = r_u32(state + 80);
        coordinates[3] = coordinates[1] = (uint16)(other + 154 - (value - 66) * 8);
        value = r_u32(state + 80);
        other = r_u32(state + 364);
        other = r_u16(other + 66);
        value = (value - 66) * 8 + other + 154;
    }
    else
    {
        coordinates[4] = coordinates[0] = 96;
        coordinates[6] = coordinates[2] = 224;
        value = r_u32(state + 364);
        value = r_u16(value + 66);
        coordinates[3] = coordinates[1] = (uint16)(value + 26);
        value = r_u32(state + 364);
        value = r_u16(value + 66);
        value += 282;
    }
    coordinates[7] = coordinates[5] = (uint16)value;
    state = r_u32(0x80112DC4);
    if (r_u32(state + 80) == 60)
        sub_8002db4c(100);
    state = r_u32(0x80112DC4);
    if (r_u32(state + 80) == 82)
        sub_8002db4c(102);
    state = r_u32(0x80112DC4);
    if (r_u32(state + 80) - 82 < 32)
    {
        packet = cursor;
        cursor += 16;
        w_u8(packet + 3, 3);
        w_u8(packet + 7, 98);
        owner = r_u32(0x80112DAC);
        value = r_u32(owner + 32);
        w_u16(packet + 8, (uint32)((sint32)value / 256) + 146);
        value = r_u32(owner + 40);
        value = 154 - ((uint32)((sint32)value / 256) + 8) * 2;
        state = r_u32(0x80112DC4);
        w_u16(packet + 10, value);
        w_u16(packet + 12, 16);
        w_u16(packet + 14, 32);
        value = r_u32(state + 80);
        w_u8(packet + 4, 0 - 128 - (value - 82) * 2);
        state = r_u32(0x80112DC4);
        value = r_u32(state + 80);
        w_u8(packet + 5, 0 - 128 - (value - 82) * 2);
        state = r_u32(0x80112DC4);
        value = r_u32(state + 80);
        w_u8(packet + 6, 0 - 128 - (value - 82) * 2);
        state = r_u32(0x80112DC4);
        ordering = r_u32(state + 20);
        tag = r_u32(packet);
        link = r_u32(ordering + 1020);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        ordering = r_u32(state + 20);
        link = r_u32(ordering + 1020);
        w_u32(ordering + 1020, (link & 0xFF000000) | (packet & 0xFFFFFF));
    }
    packet = cursor;
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 44);
    state = r_u32(0x80112DC4);
    w_u16(packet + 8, coordinates[0]);
    w_u16(packet + 10, coordinates[1]);
    w_u16(packet + 16, coordinates[2]);
    w_u16(packet + 18, coordinates[3]);
    w_u16(packet + 24, coordinates[4]);
    w_u16(packet + 26, coordinates[5]);
    w_u16(packet + 32, coordinates[6]);
    w_u16(packet + 34, coordinates[7]);
    time = r_u32(state + 80);
    cursor += 40;
    if ((sint32)time < 82)
    {
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
    }
    else if ((sint32)time < 114)
    {
        w_u8(packet + 4, 0 - 128 - (time - 82) * 4);
        state = r_u32(0x80112DC4);
        value = r_u32(state + 80);
        w_u8(packet + 5, 0 - 128 - (value - 82) * 4);
        state = r_u32(0x80112DC4);
        value = r_u32(state + 80);
        w_u8(packet + 6, 0 - 128 - (value - 82) * 4);
    }
    else
    {
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
    }
    w_u16(packet + 22, 37);
    w_u16(packet + 14, 0x5714);
    w_u8(packet + 20, 127);
    w_u8(packet + 29, 127);
    w_u8(packet + 36, 127);
    w_u8(packet + 37, 127);
    w_u8(packet + 12, 0);
    w_u8(packet + 13, 0);
    w_u8(packet + 21, 0);
    w_u8(packet + 28, 0);
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 20);
    tag = r_u32(packet);
    link = r_u32(ordering + 1020);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 20);
    link = r_u32(ordering + 1020);
    w_u32(ordering + 1020, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return cursor;
}

uint32 resource002_sub_800f7c24(uint32 amount)
{
    PSX_RECT rectangle;
    uint16 *colors;
    uint32 state, source, destination, first, second, red, green, blue, product, index, row, word;
    uint32 scene, packet, ordering, tag, link, value, other, context;
    sint32 rows;
    FUNCTION_MARKER(0x800F7C24, "ACE3.BIN");
    colors = calloc(16 * 32767, sizeof(*colors));
    if (!colors)
    {
#ifndef NDEBUG
        fprintf(stderr, "Cannot allocate resource2 palette buffer\n");
#endif
        abort();
    }
    state = r_u32(0x80112DC4);
    rows = r_s16(state + 222);
    index = 0;
    if (rows > 0)
    {
        do
        {
            destination = r_u32(state + 212);
            source = r_u32(state + 208);
            first = r_u16(destination + index * 2);
            second = r_u16(source + index * 2);
            product = ((second & 31) - (first & 31)) * amount;
            red = (first & 31) + (uint32)((sint32)product / 25);
            product = (((second >> 5) & 31) - ((first >> 5) & 31)) * amount;
            green = ((first >> 5) & 31) + (uint32)((sint32)product / 25);
            product = (((second >> 10) & 31) - ((first >> 10) & 31)) * amount;
            blue = ((first >> 10) & 31) + (uint32)((sint32)product / 25);
            colors[index] = (uint16)(red | (green << 5) | (blue << 10));
            rows = r_s16(state + 222);
            ++index;
        } while ((sint32)index < rows * 16);
    }
    state = r_u32(0x80112DC4);
    context = r_u32(0x80112DC4);
    value = r_u32(state + 216);
    other = r_u32(state + 220);
    xport_store_le32((uint8 *)&rectangle, value);
    xport_store_le32((uint8 *)&rectangle + 4, other);
    rectangle.h = 1;
    rows = r_s16(context + 222);
    row = 0;
    value = (uint32)rows;
    if (rows > 0)
    {
        do
        {
            scene = r_u32(0x800BCE54);
            packet = r_u32(scene + 20);
            state = r_u32(0x80112DC4);
            w_u32(scene + 20, packet + 68);
            rectangle.y = (sint16)(r_u16(state + 218) + row);
            SetDrawLoad((DR_LOAD *)psx_addr(packet, sizeof(DR_LOAD)), &rectangle);
            for (word = 0; word < 8; ++word)
            {
                other = xport_load_le32(colors + row * 16 + word * 2);
                w_u32(packet + 16 + word * 4, other);
            }
            scene = r_u32(0x800BCE54);
            ordering = r_u32(scene + 16);
            tag = r_u32(packet);
            link = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            ordering = r_u32(scene + 16);
            link = r_u32(ordering);
            state = r_u32(0x80112DC4);
            w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
            rows = r_s16(state + 222);
            ++row;
            value = (sint32)row < rows;
        } while (value);
    }
    free(colors);
    return value;
}

void resource002_mission_select_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800FD958:
            resource002_sub_800fd958();
            return;
        case 0x800FD9CC:
            resource002_sub_800fd9cc();
            return;
        case 0x800FDE78:
            resource002_sub_800fde78();
            return;
        case 0x800FDF38:
            resource002_sub_800fdf38();
            return;
        case 0x800FDF64:
            resource002_sub_800fdf64();
            return;
        case 0x800FE0D4:
            resource002_sub_800fe0d4();
            return;
        case 0x800FE180:
            resource002_sub_800fe180();
            return;
        case 0x800FE1E8:
            resource002_sub_800fe1e8();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 mission selection phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_load_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800FEF08:
            return resource002_sub_800fef08();
        case 0x800FEF24:
            return resource002_sub_800fef24();
        case 0x800FF098:
            return resource002_sub_800ff098();
        case 0x800FF0BC:
            return resource002_sub_800ff0bc();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 load phase %08X\n", target);
#endif
            abort();
    }
}

void resource002_aircraft_phase_call(uint32 target)
{
    switch (target)
    {
        case 0x800F9E68:
            resource002_sub_800f9e68();
            return;
        case 0x800F9EC8:
            resource002_sub_800f9ec8();
            return;
        case 0x800FA4E4:
            resource002_sub_800fa4e4();
            return;
        case 0x800FA860:
            resource002_sub_800fa860();
            return;
        case 0x800FA8E0:
            resource002_sub_800fa8e0();
            return;
        case 0x800FAA48:
            resource002_sub_800faa48();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 aircraft phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_detail_phase_call(uint32 target, uint32 context)
{
    switch (target)
    {
        case 0x800D240C:
            return resource002_sub_800d240c();
        case 0x800D24E8:
            return resource002_sub_800d24e8();
        case 0x800D2568:
            return resource002_sub_800d2568();
        case 0x800D2BCC:
            return resource002_sub_800d2bcc();
        case 0x800D2D74:
            return resource002_sub_800d2d74();
        case 0x800D2DF0:
            return resource002_sub_800d2df0();
        case 0x800D2FA8:
            return resource002_sub_800d2fa8();
        case 0x800D3088:
            return resource002_sub_800d3088();
        case 0x800D8CA8:
            return resource002_sub_800d8ca8();
        case 0x800D911C:
            return resource002_sub_800d911c();
        case 0x800DA444:
            return resource002_sub_800da444();
        case 0x800DA4F4:
            return resource002_sub_800da4f4();
        case 0x800DA5EC:
            return resource002_sub_800da5ec();
        case 0x800DA66C:
            return resource002_sub_800da66c();
        case 0x800DA870:
            return resource002_sub_800da870();
        case 0x800DAA6C:
            return resource002_sub_800daa6c();
        case 0x800DB3F8:
            return resource002_sub_800db3f8();
        case 0x800DB714:
            return resource002_sub_800db714();
        case 0x800DBAEC:
            return resource002_sub_800dbaec(context);
        case 0x800DBC94:
            return resource002_sub_800dbc94(context);
        case 0x800DBFC4:
            return resource002_sub_800dbfc4(context);
        case 0x800DC0A0:
            return resource002_sub_800dc0a0();
        case 0x800DC144:
            return resource002_sub_800dc144();
        case 0x800DC43C:
            return resource002_sub_800dc43c();
        case 0x800DCB5C:
            return resource002_sub_800dcb5c();
        case 0x800DCBE0:
            return resource002_sub_800dcbe0();
        case 0x800DCC80:
            return resource002_sub_800dcc80();
        case 0x800DCDC4:
            return resource002_sub_800dcdc4();
        case 0x800DCE0C:
            return resource002_sub_800dce0c();
        case 0x800DD1D0:
            return resource002_sub_800dd1d0();
        case 0x800DDFA4:
            return resource002_sub_800ddfa4();
        case 0x800DE078:
            return resource002_sub_800de078();
        case 0x800F6CB4:
            resource002_sub_800f6cb4();
            return target;
        case 0x801071B8:
            return resource002_sub_801071b8();
        case 0x80107200:
            return resource002_sub_80107200();
        case 0x80107270:
            return resource002_sub_80107270();
        case 0x80107358:
            return resource002_sub_80107358();
        case 0x80108B7C:
            return resource002_sub_80108b7c();
        case 0x80108D90:
            return resource002_sub_80108d90();
        case 0x80108E88:
            return resource002_sub_80108e88();
        case 0x801092BC:
            return resource002_sub_801092bc();
        case 0x801093E8:
            return resource002_sub_801093e8();
        case 0x80109588:
            return resource002_sub_80109588();
        case 0x80109660:
            return resource002_sub_80109660();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 detail phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_mode_phase_call(uint32 target, uint32 state)
{
    switch (target)
    {
        case 0x800E6288:
            return resource002_sub_800e6288();
        case 0x800E6334:
            return resource002_sub_800e6334();
        case 0x800E63F4:
            return resource002_sub_800e63f4();
        case 0x800E65A4:
            return resource002_sub_800e65a4();
        case 0x800E65F8:
            return resource002_sub_800e65f8();
        case 0x800E660C:
            return resource002_sub_800e660c();
        case 0x800EB9EC:
            return resource002_sub_800eb9ec();
        case 0x800EBBD0:
            return resource002_sub_800ebbd0();
        case 0x800EBC3C:
            return resource002_sub_800ebc3c();
        case 0x800EBD48:
            return resource002_sub_800ebd48();
        case 0x800F0898:
            return resource002_sub_800f0898();
        case 0x800F0A7C:
            return resource002_sub_800f0a7c();
        case 0x800F0DC4:
            return resource002_sub_800f0dc4();
        case 0x800F0F5C:
            return resource002_sub_800f0f5c();
        case 0x800F108C:
            return resource002_sub_800f108c();
        case 0x800F11BC:
            return resource002_sub_800f11bc();
        case 0x800F132C:
            return resource002_sub_800f132c();
        case 0x800F1384:
            return resource002_sub_800f1384();
        case 0x800F13DC:
            return resource002_sub_800f13dc();
        case 0x800F1430:
            return resource002_sub_800f1430();
        case 0x800F16C4:
            return resource002_sub_800f16c4();
        case 0x800F1848:
            return resource002_sub_800f1848();
        case 0x800F1A7C:
            return resource002_sub_800f1a7c();
        case 0x800F1B8C:
            return resource002_sub_800f1b8c();
        case 0x800F1C8C:
            return resource002_sub_800f1c8c();
        case 0x800F1D04:
            return resource002_sub_800f1d04();
        case 0x800F2240:
            return resource002_sub_800f2240();
        case 0x800F23CC:
            return resource002_sub_800f23cc();
        case 0x800F2618:
            return resource002_sub_800f2618();
        case 0x800F26EC:
            return resource002_sub_800f26ec();
        case 0x800F2820:
            return resource002_sub_800f2820();
        case 0x800FABE8:
            return resource002_sub_800fabe8();
        case 0x800FB10C:
            return resource002_sub_800fb10c();
        case 0x800FE444:
            return resource002_sub_800fe444();
        case 0x800FE564:
            return resource002_sub_800fe564();
        case 0x800FE584:
            return resource002_sub_800fe584();
        case 0x80106000:
            return resource002_sub_80106000();
        case 0x8010606C:
            return resource002_sub_8010606c();
        case 0x801062B8:
            return resource002_sub_801062b8();
        case 0x8010655C:
            return resource002_sub_8010655c();
        case 0x801065C8:
            return resource002_sub_801065c8();
        case 0x8010673C:
            return resource002_sub_8010673c();
        case 0x80106780:
            return resource002_sub_80106780();
        case 0x801067B0:
            return resource002_sub_801067b0();
        case 0x80106A34:
            return resource002_sub_80106a34();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 mode phase %08X\n", target);
#endif
            abort();
    }
}

void resource002_selection_phase_call(uint32 target, uint32 context)
{
    switch (target)
    {
        case 0x800E7498:
            resource002_sub_800e7498();
            return;
        case 0x800E7554:
            resource002_sub_800e7554();
            return;
        case 0x800E7758:
            resource002_sub_800e7758();
            return;
        case 0x800E77EC:
            resource002_sub_800e77ec();
            return;
        case 0x800E78A0:
            resource002_sub_800e78a0();
            return;
        case 0x800E7948:
            resource002_sub_800e7948();
            return;
        case 0x800E7B8C:
            resource002_sub_800e7b8c();
            return;
        case 0x800E7CDC:
            resource002_sub_800e7cdc();
            return;
        case 0x800E80B8:
            resource002_sub_800e80b8();
            return;
        case 0x800E8174:
            resource002_sub_800e8174();
            return;
        case 0x800E83AC:
            resource002_sub_800e83ac();
            return;
        case 0x800E8404:
            resource002_sub_800e8404();
            return;
        case 0x800E8454:
            resource002_sub_800e8454();
            return;
        case 0x800E84E8:
            resource002_sub_800e84e8();
            return;
        case 0x800E8600:
            resource002_sub_800e8600();
            return;
        case 0x800E868C:
            resource002_sub_800e868c();
            return;
        case 0x800E8708:
            resource002_sub_800e8708();
            return;
        case 0x800E8910:
            resource002_sub_800e8910();
            return;
        case 0x800E89F8:
            resource002_sub_800e89f8();
            return;
        case 0x800E8B2C:
            resource002_sub_800e8b2c();
            return;
        case 0x800E8B98:
            resource002_sub_800e8b98();
            return;
        case 0x800E8C50:
            resource002_sub_800e8c50();
            return;
        case 0x800E8D08:
            resource002_sub_800e8d08();
            return;
        case 0x800E8D78:
            resource002_sub_800e8d78();
            return;
        case 0x800E8E28:
            resource002_sub_800e8e28();
            return;
        case 0x800E8EF0:
            resource002_sub_800e8ef0();
            return;
        case 0x800E8FC4:
            resource002_sub_800e8fc4();
            return;
        case 0x800E9068:
            resource002_sub_800e9068();
            return;
        case 0x800E927C:
            resource002_sub_800e927c();
            return;
        case 0x800E9310:
            resource002_sub_800e9310();
            return;
        case 0x800E93B8:
            resource002_sub_800e93b8();
            return;
        case 0x800E94E0:
            resource002_sub_800e94e0();
            return;
        case 0x800E955C:
            resource002_sub_800e955c();
            return;
        case 0x800E95DC:
            resource002_sub_800e95dc();
            return;
        case 0x800E965C:
            resource002_sub_800e965c();
            return;
        case 0x800E97A4:
            resource002_sub_800e97a4();
            return;
        case 0x800E9824:
            resource002_sub_800e9824();
            return;
        case 0x800E9AB4:
            resource002_sub_800e9ab4();
            return;
        case 0x800E9C40:
            resource002_sub_800e9c40();
            return;
        case 0x800E9E20:
            resource002_sub_800e9e20();
            return;
        case 0x800E9EC4:
            resource002_sub_800e9ec4();
            return;
        case 0x800E9F64:
            resource002_sub_800e9f64();
            return;
        case 0x800EA004:
            resource002_sub_800ea004();
            return;
        case 0x800EA0C8:
            resource002_sub_800ea0c8();
            return;
        case 0x800EA1C4:
            resource002_sub_800ea1c4();
            return;
        case 0x800EA2E8:
            resource002_sub_800ea2e8();
            return;
        case 0x800EA3F4:
            resource002_sub_800ea3f4();
            return;
        case 0x800EA744:
            resource002_sub_800ea744();
            return;
        case 0x800EA8AC:
            resource002_sub_800ea8ac();
            return;
        case 0x800EAC04:
            resource002_sub_800eac04();
            return;
        case 0x800EAF44:
            resource002_sub_800eaf44();
            return;
        case 0x800EB064:
            resource002_sub_800eb064();
            return;
        case 0x800EB174:
            resource002_sub_800eb174();
            return;
        case 0x800EB280:
            resource002_sub_800eb280();
            return;
        case 0x800EB388:
            resource002_sub_800eb388();
            return;
        case 0x800EB42C:
            resource002_sub_800eb42c();
            return;
        case 0x800EB4F4:
            resource002_sub_800eb4f4();
            return;
        case 0x800EB57C:
            resource002_sub_800eb57c();
            return;
        case 0x800EB73C:
            resource002_sub_800eb73c();
            return;
        case 0x800EB944:
            resource002_sub_800eb944();
            return;
        case 0x800EB98C:
            resource002_sub_800eb98c();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 selection phase %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800dee60(uint32 index)
{
    uint32 owner, labels;
    FUNCTION_MARKER(0x800DEE60, "ACE3.BIN");
    owner = r_u32(0x80112DC0);
    labels = r_u32(owner + 92);
    return r_u8(labels + index);
}
