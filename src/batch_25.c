#include "game_functions.h"
#include <stdlib.h>

uint32 resource002_sub_800e1334(void)
{
    FUNCTION_MARKER(0x800E1334, "ACE3.BIN");
    return resource002_sub_800e133c(r_u32(0x80112DC0));
}

uint32 resource002_sub_800e14d0(void)
{
    uint32 game, bank, owner, cursor, state, pointer, depth, ordering, tag, link;
    uint32 corner, x, y, value, time, packet, row, column, layout, visible, bit;

    FUNCTION_MARKER(0x800E14D0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DC0);
    cursor = owner + bank * 40 + 0x56C8;
    for (corner = 0; corner < 4; ++corner)
    {
        value = r_s16(owner + 372);
        x = value * 5 + ((corner & 1) ? 162 : 150);
        w_u16(cursor + 8 + corner * 8, x);
        value = r_s16(owner + 374);
        y = r_s16(owner + 372);
        y = (uint32)((sint32)(y + 1) % 2);
        y = value * 10 + y * 5 + (corner < 2 ? 190 : 210);
        w_u16(cursor + 10 + corner * 8, y);
    }
    value = (uint32)(r_s32(game + 32) % 128);
    value = 64 - value;
    if ((sint32)value < 0)
        value = 0 - value;
    value *= 2;
    w_u8(cursor + 4, value);
    w_u8(cursor + 5, value);
    w_u8(cursor + 6, value);
    owner = r_u32(0x80112DC0);
    pointer = r_u32(owner + 60);
    state = r_u32(0x80112DC4);
    depth = r_s16(pointer + 14);
    ordering = r_u32(state + 8);
    tag = r_u32(cursor);
    link = r_u32(ordering + depth * 4);
    w_u32(cursor, (tag & 0xFF000000) | (link & 0xFFFFFF));
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    ordering = r_u32(state + 8);
    link = r_u32(ordering + depth * 4);
    w_u32(ordering + depth * 4, (link & 0xFF000000) | (cursor & 0xFFFFFF));
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    time = r_s16(owner + 368);
    packet = owner + bank * 10920 + 376;
    pointer = r_u32(owner + 60);
    depth = r_s16(pointer + 14);
    ordering = r_u32(state + 8) + depth * 4;
    for (row = 0; row < 32; ++row)
    {
        owner = r_u32(0x80112DC0);
        layout = r_u32(0x8010AD2C + row * 4);
        visible = r_u32(owner + 240 + row * 4);
        for (column = 0; column < 32; ++column)
        {
            bit = (uint32)1 << column;
            if (!(layout & bit) || !(visible & bit))
                continue;
            y = 190 + ((row + 1) % 2) * 5 + column * 10;
            w_u16(packet + 26, y + 12);
            w_u16(packet + 34, y + 12);
            w_u16(packet + 8, 150 + row * 5);
            w_u16(packet + 10, y);
            w_u16(packet + 16, 158 + row * 5);
            w_u16(packet + 18, y);
            w_u16(packet + 24, 150 + row * 5);
            w_u16(packet + 32, 158 + row * 5);
            if ((sint32)time >= 31)
            {
                value = 128 - (uint32)((sint32)((time - 30) << 7) / 90);
                w_u8(packet + 4, value);
                w_u8(packet + 5, value);
                w_u8(packet + 6, value);
            }
            tag = r_u32(packet);
            link = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            link = r_u32(ordering);
            w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
            packet += 40;
        }
    }
    return 0;
}

uint32 resource002_sub_800ceda8(void)
{
    uint32 game, state, value, mode, mission;

    FUNCTION_MARKER(0x800CEDA8, "ACE3.BIN");
    resource002_sub_800e18b8();
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) && !(r_u32(game + 296) & 0x2F))
    {
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) == 3)
        {
            value = r_u16(state + 452);
            mode = r_u8(state + 126);
            w_u16(state + 452, value + 1);
            if (mode == 0)
            {
                if (!(r_u32(game + 2412) & 16))
                    resource002_sub_800decd4(0, 1);
                state = r_u32(0x80112DC4);
                if (r_u8(state + 126) == 0)
                {
                    game = r_u32(0x800BCE60);
                    mission = r_s16(game + 908);
                    if (mission == 59 || r_s16(game + 2388) == 3)
                        w_u16(state + 454, 8);
                }
            }
            state = r_u32(0x80112DC4);
            if (r_u8(state + 126) == 1)
            {
                game = r_u32(0x800BCE60);
                if (r_u32(game + 2412) & 16)
                    w_u16(state + 454, 3);
                else
                {
                    arena_set_cursor(r_u32(state + 264));
                    state = r_u32(0x80112DC4);
                    w_u16(state + 454, 4);
                }
            }
            state = r_u32(0x80112DC4);
            if (r_u8(state + 126) == 2)
            {
                arena_set_cursor(r_u32(state + 264));
                value = arena_alloc(188);
                w_u32(0x80112DB8, value);
                state = r_u32(0x80112DC4);
                w_u16(state + 454, 6);
            }
            resource002_sub_800dee08(1, 3, -1, -1);
            resource002_sub_800dee08(2, 3, -1, -1);
            state = r_u32(0x80112DC4);
            if (r_u8(state + 126) == 0)
                resource002_sub_80100994();
            state = r_u32(0x80112DC4);
            if (r_u8(state + 126) == 3)
            {
                arena_set_cursor(r_u32(state + 264));
                state = r_u32(0x80112DC4);
                w_u16(state + 454, 10);
            }
            sub_8002db4c(60);
        }
    }
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 16)
    {
        if (r_u32(game + 304) & 8)
        {
            state = r_u32(0x80112DC4);
            if (r_s16(state + 454) == 3)
            {
                if (resource002_sub_800ded38(0, 1) == 2)
                    goto finish_menu_input;
                state = r_u32(0x80112DC4);
                if (r_s16(state + 454) != 3)
                    goto finish_menu_input;
                mode = r_u8(state + 126);
                w_u8(state + 126, mode == 0 ? mode + 1 : 0);
                sub_8002db4c(61);
                goto finish_menu_input;
            }
        }
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 304) & 4))
            goto finish_menu_input;
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) != 3 || resource002_sub_800ded38(0, 1) == 2)
            goto finish_menu_input;
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) != 3)
            goto finish_menu_input;
        mode = r_u8(state + 126);
        w_u8(state + 126, mode == 0 ? 1 : mode - 1);
        sub_8002db4c(61);
    }
    else
    {
        if (r_u32(game + 304) & 8)
        {
            state = r_u32(0x80112DC4);
            if (r_s16(state + 454) == 3)
            {
                if (resource002_sub_800ded38(0, 1) == 2)
                    goto finish_menu_input;
                state = r_u32(0x80112DC4);
                if (r_s16(state + 454) != 3)
                    goto finish_menu_input;
                mode = r_u8(state + 126);
                w_u8(state + 126, mode < 3 ? mode + 1 : 0);
                sub_8002db4c(61);
                goto finish_menu_input;
            }
        }
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 304) & 4))
            goto finish_menu_input;
        state = r_u32(0x80112DC4);
        if (r_s16(state + 454) != 3 || resource002_sub_800ded38(0, 1) == 2)
            goto finish_menu_input;
        state = r_u32(0x80112DC4);
        value = r_s16(state + 454);
        if (value != 3)
            goto finish_menu_input;
        mode = r_u8(state + 126);
        w_u8(state + 126, mode == 0 ? value : mode - 1);
        sub_8002db4c(61);
    }
finish_menu_input:
    resource002_sub_800e1334();
    resource002_sub_800e14d0();
    return resource002_sub_800e1258();
}

uint32 resource002_loading_phase_call(uint32 target, uint32 mode, uint32 incoming_buffer)
{
    switch (target)
    {
        case 0x800CE554:
            return resource002_sub_800ce554(mode, incoming_buffer);
        case 0x800CE6B0:
            return resource002_sub_800ce6b0();
        case 0x800CE6EC:
            return resource002_sub_800ce6ec();
        case 0x800CE754:
            return resource002_sub_800ce754(target);
        case 0x800CE75C:
            return resource002_sub_800ce75c();
        case 0x800CE7EC:
            return resource002_sub_800ce7ec();
        case 0x800CE894:
            return resource002_sub_800ce894();
        case 0x800CE924:
            return resource002_sub_800ce924();
        case 0x800CE9E4:
            return resource002_sub_800ce9e4();
        case 0x800CEA6C:
            return resource002_sub_800cea6c();
        case 0x800CEAFC:
            return resource002_sub_800ceafc();
        case 0x800CEB94:
            return resource002_sub_800ceb94();
        case 0x800CEC70:
            return resource002_sub_800cec70();
        case 0x800CED08:
            return resource002_sub_800ced08();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 loading phase %08X\n", target);
#endif
            abort();
            return 0;
    }
}

uint32 resource002_sub_800cf494(void)
{
    uint32 state, phase, target;

    FUNCTION_MARKER(0x800CF494, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 452);
    target = r_u32(0x80109884 + phase * 4);
    switch (target)
    {
        case 0x800CED38:
            resource002_sub_800ced38();
            break;
        case 0x800CEDA8:
            resource002_sub_800ceda8();
            break;
        case 0x800CF1DC:
            resource002_sub_800cf1dc();
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 menu state %08X\n", target);
#endif
            abort();
            return 0;
    }
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 454);
    target = r_u32(0x8010984C + phase * 4);
    return resource002_loading_phase_call(target, 0, 0);
}

uint32 resource002_sub_800d12e0(void)
{
    uint32 state, pointer, menu, owner, value, game, target;

    FUNCTION_MARKER(0x800D12E0, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    if (r_s16(state + 454) == 3)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50, 1);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 51, 0);
        pointer = r_u32(0x80112DC0);
        owner = r_u32(0x80112DB8);
        menu = r_u32(pointer + 92);
        value = r_u8(owner + 3);
        w_u8(menu + 52, value);
        owner = r_u32(0x80112DB8);
        resource002_sub_800d0830(r_u8(owner + 2));
        owner = r_u32(0x80112DB8);
        resource002_sub_800d0970(r_u8(owner + 3));
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 1);
        resource002_sub_800dedf0(0, 11);
        resource002_sub_800dee08(1, 1, 0, 120);
        resource002_sub_800def94(50, 1);
        game = r_u32(0x800BCE60);
        w_u8(game + 2398, 0);
        state = r_u32(0x80112DC4);
        value = r_u8(state + 126);
        resource002_sub_800def94(1, value + 1);
    }
    state = r_u32(0x80112DC4);
    value = r_s16(state + 454);
    target = r_u32(0x8010984C + value * 4);
    return resource002_loading_phase_call(target, 0, 0);
}

uint32 resource002_sub_800cf504(uint32 page, uint32 item)
{
    uint32 owner, state, id, table, data;

    FUNCTION_MARKER(0x800CF504, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    state = r_u32(0x80112DC4);
    id = r_u8(owner + page * 4 + item + 516);
    table = r_u32(state + 468);
    data = r_u32(table + id * 4);
    return resource002_sub_800ed000(data, 960, 340, 384, 6);
}

uint32 resource002_sub_800cf854(void)
{
    const uint32 fields[4] = {2, 50, 51, 52};
    uint32 game, owner, source, pointer, menu, index, count, value;

    FUNCTION_MARKER(0x800CF854, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112DD0);
    w_u8(owner, 1);
    owner = r_u32(0x80112DD0);
    value = r_u8(owner + 2);
    source = game + 3400;
    resource002_sub_800cf564(value);
    for (index = 0; index < 4; ++index)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + fields[index], 0);
    }
    resource002_sub_800def94(49, 0);
    w_u32(0x80112DD4, 0);
    for (index = 0; index < 4; ++index)
    {
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        if ((sint32)index < (sint32)count)
        {
            value = r_u8(owner + index + 516);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            value = r_u8(source + value);
            w_u8(menu + index + 53, value);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 1);
        }
        else
        {
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 53, 0);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 0);
        }
    }
    return 0;
}

uint32 resource002_sub_800cff5c(void)
{
    uint32 owner, mode, page, item, pointer, menu, state, buffer;

    FUNCTION_MARKER(0x800CFF5C, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    mode = r_u8(owner + 1);
    if (mode == 0)
    {
        w_u8(owner, 3);
        resource002_sub_800deca8(0, 4);
        resource002_sub_800dee08(11, 1, 0, 25);
        resource002_sub_800dedf0(0, 0x4011);
        owner = r_u32(0x80112DD0);
        page = r_u8(owner + 2);
        item = r_u8(owner + 3);
        resource002_sub_800cf504(page, item);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 3, 0);
        return menu;
    }
    if (mode != 1)
        return 1;
    resource002_sub_800dedf0(0, 3);
    state = r_u32(0x80112DC4);
    w_u8(state + 124, 0);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 0);
    state = r_u32(0x80112DC4);
    buffer = r_u32(state + 264);
    w_u16(state + 454, 0);
    return arena_set_cursor(buffer);
}

uint32 resource002_sub_800d0034(void)
{
    uint32 owner, page, item, game, id, pointer, menu, context, entries;

    FUNCTION_MARKER(0x800D0034, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    page = r_u8(owner + 2);
    item = r_u8(owner + 3);
    game = r_u32(0x800BCE60);
    id = r_u8(owner + page * 4 + item + 516);
    w_u8(game + id + 3400, 2);
    resource002_sub_800def94(49, 0);
    owner = r_u32(0x80112DD0);
    game = r_u32(0x800BCE60);
    w_u32(owner + 604, 0);
    if (r_u32(game + 68) != 0)
        return 0x800C0000;
    context = r_u32(0x800BCE50);
    entries = sub_80027338(r_u32(context + 96), 0);
    resource002_sub_800ecda8(r_u32(entries));
    arena_set_cursor(r_u32(entries));
    owner = r_u32(0x80112DD0);
    pointer = r_u32(0x80112DC0);
    w_u32(owner + 600, 0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 1, 0);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 2, 0);
    resource002_sub_800dee08(3, 0, 0, 0);
    resource002_sub_800dee08(4, 0, 0, 0);
    owner = r_u32(0x80112DD0);
    w_u8(owner, 4);
    return 4;
}

uint32 resource002_sub_800cf6b0(void)
{
    uint32 owner;

    FUNCTION_MARKER(0x800CF6B0, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    w_u8(owner + 4, 0);
    return owner;
}

uint32 resource002_sub_800cf6c0(void)
{
    uint32 index, owner, packet, state, page, head, tag, link, base;

    FUNCTION_MARKER(0x800CF6C0, "ACE3.BIN");
    for (index = 0; index < 11; ++index)
    {
        owner = r_u32(0x80112DD0);
        packet = r_u32(owner + 512) + index * 20;
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 100);
        w_u16(packet + 8, 101);
        w_u16(packet + 16, 144);
        w_u16(packet + 10, 120 + index * 24);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u8(packet + 12, 0);
        w_u8(packet + 13, index * 18);
        owner = r_u32(0x80112DD0);
        w_u16(packet + 18, 18);
        page = r_u8(owner + 4);
        state = r_u32(0x80112DC4);
        w_u16(packet + 14, ((page + 210) << 6) | 63);
        head = r_u32(state + 8);
        tag = r_u32(packet);
        link = r_u32(head);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        head = r_u32(state + 8);
        link = r_u32(head);
        w_u32(head, (link & 0xFF000000) | (packet & 0xFFFFFF));
    }
    owner = r_u32(0x80112DD0);
    base = r_u32(owner + 512);
    page = r_u8(owner + 4);
    packet = base + 240;
    page = ((((page >> 1) << 6) + 640) & 1023) >> 6;
    sub_800ac7f8(packet, 0, 0, page | 16, 0);
    state = r_u32(0x80112DC4);
    head = r_u32(state + 8);
    tag = r_u32(base + 240);
    link = r_u32(head);
    w_u32(base + 240, (tag & 0xFF000000) | (link & 0xFFFFFF));
    head = r_u32(state + 8);
    link = r_u32(head);
    link = (link & 0xFF000000) | (packet & 0xFFFFFF);
    w_u32(head, link);
    return link;
}

uint32 resource002_sub_800cf9b0(uint32 countdown, uint32 incoming_v1)
{
    uint32 owner, count, page, item, last, value, flags, game, pointer, menu;
    uint32 context, resource, size, buffer, source, index;

    FUNCTION_MARKER(0x800CF9B0, "ACE3.BIN");
    if (countdown != 0)
        w_u32(incoming_v1 + 0x2DD4, countdown - 1);
    else
    {
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        page = r_u8(owner + 2);
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        flags = (sint32)page < ((sint32)last >> 2) ? 2 : 0;
        if (page != 0)
            flags |= 1;
        resource002_sub_800def94(49, flags);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 51, 0);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 2) != 0)
    {
        owner = r_u32(0x80112DD0);
        page = r_u8(owner + 2);
        item = r_u8(owner + 3);
        count = r_u32(owner + 596);
        if ((sint32)(item + page * 4) < (sint32)(count - 1))
        {
            if (item < 3)
                w_u8(owner + 3, item + 1);
            else
            {
                w_u8(owner + 3, 0);
                owner = r_u32(0x80112DD0);
                page = r_u8(owner + 2);
                w_u8(owner + 2, page + 1);
                owner = r_u32(0x80112DD0);
                resource002_sub_800cf564(r_u8(owner + 2));
                pointer = r_u32(0x80112DC0);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 2);
                w_u32(0x80112DD4, 12);
            }
            sub_8002db4c(61);
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf640(r_u8(owner + 3));
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 1) != 0)
    {
        owner = r_u32(0x80112DD0);
        item = r_u8(owner + 3);
        if (item != 0)
            w_u8(owner + 3, item - 1);
        else
        {
            page = r_u8(owner + 2);
            if (page == 0)
                goto selection_next_page;
            w_u8(owner + 3, 3);
            owner = r_u32(0x80112DD0);
            page = r_u8(owner + 2);
            w_u8(owner + 2, page - 1);
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf564(r_u8(owner + 2));
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 1);
            w_u32(0x80112DD4, 12);
        }
        sub_8002db4c(61);
        owner = r_u32(0x80112DD0);
        resource002_sub_800cf640(r_u8(owner + 3));
    }
selection_next_page:
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 512) != 0)
    {
        owner = r_u32(0x80112DD0);
        count = r_u32(owner + 596);
        page = r_u8(owner + 2);
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        if ((sint32)page < ((sint32)last >> 2))
        {
            w_u8(owner + 2, page + 1);
            owner = r_u32(0x80112DD0);
            page = r_u8(owner + 2);
            item = r_u8(owner + 3);
            count = r_u32(owner + 596);
            if ((sint32)(item + page * 4) >= (sint32)count)
            {
                last = count - 1;
                value = (sint32)last < 0 ? count + 2 : last;
                w_u8(owner + 3, last - ((uint32)((sint32)value >> 2) << 2));
            }
            sub_8002db4c(61);
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf564(r_u8(owner + 2));
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf640(r_u8(owner + 3));
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 2);
            w_u32(0x80112DD4, 12);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 256) != 0)
    {
        owner = r_u32(0x80112DD0);
        page = r_u8(owner + 2);
        if (page != 0)
        {
            w_u8(owner + 2, page - 1);
            sub_8002db4c(61);
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf564(r_u8(owner + 2));
            owner = r_u32(0x80112DD0);
            resource002_sub_800cf640(r_u8(owner + 3));
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 1);
            w_u32(0x80112DD4, 12);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) != 0 && (r_u32(game + 296) & 47) == 0)
    {
        sub_8002db4c(60);
        resource002_sub_800cf6b0();
        owner = r_u32(0x80112DD0);
        w_u8(owner, 2);
        owner = r_u32(0x80112DD0);
        w_u8(owner + 1, 0);
        owner = r_u32(0x80112DD0);
        page = r_u8(owner + 2);
        item = r_u8(owner + 3);
        value = r_u8(owner + item + page * 4 + 516);
        context = r_u32(0x800BCE50);
        resource = r_u32(0x80109998 + value * 4);
        w_u32(context + 96, resource);
        size = resource_total_size(context + 96, 1);
        buffer = arena_alloc(size);
        context = r_u32(0x800BCE50);
        game = r_u32(0x800BCE60);
        w_u32(game + 68, 1);
        resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) != 0 && (r_u32(game + 296) & 95) == 0)
    {
        sub_8002db4c(59);
        owner = r_u32(0x80112DD0);
        w_u8(owner, 2);
        owner = r_u32(0x80112DD0);
        w_u8(owner + 1, 1);
    }
    game = r_u32(0x800BCE60);
    pointer = r_u32(0x80112DC0);
    owner = r_u32(0x80112DD0);
    menu = r_u32(pointer + 92);
    item = r_u8(owner + 3);
    source = game + 3400;
    w_u8(menu + 52, item);
    for (index = 0; index < 4; ++index)
    {
        owner = r_u32(0x80112DD0);
        page = r_u8(owner + 2);
        count = r_u32(owner + 596);
        value = index + page * 4;
        if ((sint32)value < (sint32)count)
        {
            value = r_u8(owner + value + 516);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            value = r_u8(source + value);
            w_u8(menu + index + 53, value);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 1);
        }
        else
        {
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 53, 0);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 0);
        }
    }
    return 0;
}

uint32 resource002_sub_800cf9a8(void)
{
    uint32 countdown;

    FUNCTION_MARKER(0x800CF9A8, "ACE3.BIN");
    countdown = r_u32(0x80112DD4);
    return resource002_sub_800cf9b0(countdown, 0x80110000);
}

uint32 resource002_sub_800d013c(void)
{
    uint32 owner, page, item, id, pointer, entry, count, last, menu, flags, game, result;

    FUNCTION_MARKER(0x800D013C, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    page = r_u8(owner + 2);
    item = r_u8(owner + 3);
    id = r_u8(owner + page * 4 + item + 516);
    pointer = r_u32(0x80112DC0);
    entry = 0x80109A90 + id * 12;
    menu = r_u32(pointer + 92);
    count = r_u32(entry);
    w_u8(menu + 51, 0);
    owner = r_u32(0x80112DD0);
    item = r_u8(owner + 4);
    last = count - 1;
    flags = (sint32)item < (sint32)last ? 2 : 0;
    if (item != 0)
        flags |= 1;
    resource002_sub_800def94(49, flags);
    if (r_u32(entry) != 0)
    {
        resource002_sub_800cf6c0();
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 2) != 0)
        {
            owner = r_u32(0x80112DD0);
            item = r_u8(owner + 4);
            if ((sint32)item < (sint32)last)
            {
                pointer = r_u32(0x80112DC0);
                w_u32(owner + 600, 1);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 2);
                sub_8002db4c(61);
                owner = r_u32(0x80112DD0);
                item = r_u8(owner + 4);
                w_u8(owner + 4, item + 1);
                pointer = r_u32(0x80112DC0);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 2);
            }
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 1) != 0)
        {
            owner = r_u32(0x80112DD0);
            item = r_u8(owner + 4);
            if (item != 0)
            {
                pointer = r_u32(0x80112DC0);
                w_u32(owner + 600, 2);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 1);
                sub_8002db4c(61);
                owner = r_u32(0x80112DD0);
                item = r_u8(owner + 4);
                w_u8(owner + 4, item - 1);
                pointer = r_u32(0x80112DC0);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 1);
            }
        }
        if (resource002_sub_800dee84(5) == 0)
            resource002_sub_800dee08(5, 1, 0, 10);
        owner = r_u32(0x80112DD0);
        pointer = r_u32(0x80112DC0);
        item = r_u8(owner + 4);
        menu = r_u32(pointer + 92);
        result = r_u8(entry + item * 2 + 4);
        w_u8(menu + 1, result);
        owner = r_u32(0x80112DD0);
        item = r_u8(owner + 4);
        if (r_u8(entry + item * 2 + 4) != 0 && resource002_sub_800dee84(3) == 0)
        {
            resource002_sub_800dee08(3, 1, 0, 10);
            owner = r_u32(0x80112DD0);
            w_u32(owner + 604, 1);
        }
        owner = r_u32(0x80112DD0);
        pointer = r_u32(0x80112DC0);
        item = r_u8(owner + 4);
        menu = r_u32(pointer + 92);
        result = r_u8(entry + item * 2 + 5);
        w_u8(menu + 2, result);
        owner = r_u32(0x80112DD0);
        item = r_u8(owner + 4);
        if (r_u8(entry + item * 2 + 5) != 0 && resource002_sub_800dee84(4) == 0)
        {
            resource002_sub_800dee08(4, 1, 0, 10);
            owner = r_u32(0x80112DD0);
            w_u32(owner + 604, 2);
        }
        pointer = r_u32(0x80112DC0);
        owner = r_u32(0x80112DD0);
        menu = r_u32(pointer + 92);
        result = r_u8(owner + 604);
        w_u8(menu + 3, result);
    }
    else
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 3, 3);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result != 0)
    {
        result = r_u32(game + 296) & 95;
        if (result == 0)
        {
            sub_8002db4c(59);
            owner = r_u32(0x80112DD0);
            w_u8(owner, 5);
            result = 5;
        }
    }
    return result;
}

uint32 resource002_sub_800d04f4(void)
{
    uint32 owner, state, mode;

    FUNCTION_MARKER(0x800D04F4, "ACE3.BIN");
    owner = r_u32(0x80112DD0);
    w_u8(owner, 0);
    resource002_sub_800deca8(0, 3);
    resource002_sub_800dedf0(0, 11);
    resource002_sub_800dee08(5, 0, 0, 0);
    resource002_sub_800dee08(3, 0, 0, 0);
    resource002_sub_800dee08(4, 0, 0, 0);
    state = r_u32(0x80112DC4);
    mode = r_u8(state + 126);
    return resource002_sub_800def94(1, mode + 1);
}

uint32 resource002_sub_800d07c0(void)
{
    uint32 game, bank, owner, mode, target;

    FUNCTION_MARKER(0x800D07C0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DD0);
    w_u32(owner + 512, owner + bank * 252 + 8);
    mode = r_u8(owner);
    target = r_u32(0x80109D78 + mode * 4);
    switch (target)
    {
        case 0x800CF854:
            return resource002_sub_800cf854();
        case 0x800CF9A8:
            return resource002_sub_800cf9a8();
        case 0x800CFF5C:
            return resource002_sub_800cff5c();
        case 0x800D0034:
            return resource002_sub_800d0034();
        case 0x800D013C:
            return resource002_sub_800d013c();
        case 0x800D04F4:
            return resource002_sub_800d04f4();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 selection callback 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800d180c(void)
{
    uint32 owner, table;

    FUNCTION_MARKER(0x800D180C, "ACE3.BIN");
    owner = r_u32(0x80112DDC);
    table = owner + 36;
    w_u8(table, 4);
    w_u8(table + 1, 7);
    w_u8(table + 3, 11);
    w_u8(table + 5, 9);
    w_u8(table + 6, 20);
    w_u8(table + 7, 5);
    w_u8(table + 8, 6);
    w_u8(table + 9, 14);
    w_u8(table + 11, 12);
    w_u8(table + 10, 16);
    w_u8(table + 12, 13);
    w_u8(table + 13, 18);
    w_u8(table + 2, 8);
    w_u8(table + 4, 10);
    w_u8(table + 14, 19);
    owner = r_u32(0x80112DDC);
    table = owner + 132;
    w_u8(table, 0);
    w_u8(table + 1, 2);
    w_u8(table + 2, 8);
    w_u8(table + 3, 10);
    owner = r_u32(0x80112DDC);
    table = owner + 156;
    w_u8(table, 1);
    w_u8(table + 1, 0);
    w_u8(table + 2, 0);
    w_u8(table + 3, 0);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 181, 1);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 180, 1);
    return owner;
}

uint32 resource002_sub_800d18e4(void)
{
    uint32 owner, table;

    FUNCTION_MARKER(0x800D18E4, "ACE3.BIN");
    owner = r_u32(0x80112DDC);
    table = owner + 52;
    w_u8(table, 20);
    w_u8(table + 1, 14);
    w_u8(table + 3, 5);
    w_u8(table + 4, 4);
    w_u8(table + 8, 13);
    w_u8(table + 9, 16);
    w_u8(table + 10, 7);
    w_u8(table + 2, 1);
    w_u8(table + 5, 0);
    w_u8(table + 6, 20);
    w_u8(table + 7, 20);
    w_u8(table + 11, 12);
    w_u8(table + 12, 6);
    w_u8(table + 13, 20);
    w_u8(table + 14, 20);
    owner = r_u32(0x80112DDC);
    table = owner + 136;
    w_u8(table, 12);
    w_u8(table + 1, 12);
    w_u8(table + 2, 12);
    w_u8(table + 3, 2);
    owner = r_u32(0x80112DDC);
    table = owner + 160;
    w_u8(table, 0);
    w_u8(table + 1, 0);
    w_u8(table + 2, 0);
    w_u8(table + 3, 0);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 181, 1);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 180, 1);
    return owner;
}

uint32 resource002_sub_800d19a8(void)
{
    uint32 owner, table;

    FUNCTION_MARKER(0x800D19A8, "ACE3.BIN");
    owner = r_u32(0x80112DDC);
    table = owner + 68;
    w_u8(table, 4);
    w_u8(table + 1, 7);
    w_u8(table + 3, 3);
    w_u8(table + 4, 2);
    w_u8(table + 7, 5);
    w_u8(table + 8, 6);
    w_u8(table + 9, 14);
    w_u8(table + 11, 12);
    w_u8(table + 10, 16);
    w_u8(table + 12, 13);
    w_u8(table + 2, 1);
    w_u8(table + 5, 0);
    w_u8(table + 6, 20);
    w_u8(table + 13, 20);
    w_u8(table + 14, 19);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 181, 1);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 180, 1);
    return owner;
}

uint32 resource002_sub_800d1a3c(void)
{
    uint32 owner, table;

    FUNCTION_MARKER(0x800D1A3C, "ACE3.BIN");
    owner = r_u32(0x80112DDC);
    table = owner + 84;
    w_u8(table, 20);
    w_u8(table + 2, 8);
    w_u8(table + 3, 11);
    w_u8(table + 4, 10);
    w_u8(table + 5, 9);
    w_u8(table + 9, 16);
    w_u8(table + 11, 14);
    w_u8(table + 10, 13);
    w_u8(table + 12, 12);
    w_u8(table + 1, 20);
    w_u8(table + 6, 20);
    w_u8(table + 7, 20);
    w_u8(table + 8, 20);
    w_u8(table + 13, 20);
    w_u8(table + 14, 19);
    owner = r_u32(0x80112DDC);
    table = owner + 144;
    w_u8(table, 6);
    w_u8(table + 1, 4);
    w_u8(table + 2, 0);
    w_u8(table + 3, 2);
    owner = r_u32(0x80112DDC);
    table = owner + 168;
    w_u8(table, 0);
    w_u8(table + 1, 0);
    w_u8(table + 2, 1);
    w_u8(table + 3, 0);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 181, 1);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 180, 1);
    return owner;
}

uint32 resource002_sub_800de188(void)
{
    const uint32 fields[5] = {4, 5, 6, 2, 28};
    uint32 state, counter, owner, next, index, table, value, context, entries;

    FUNCTION_MARKER(0x800DE188, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    counter = r_u32(state + 624) + 1;
    w_u32(state + 624, counter);
    if ((sint32)counter < 2)
        return 0;
    owner = r_u32(0x80112DDC);
    w_u32(state + 624, 0);
    w_u8(owner, 0);
    next = r_u32(0x80112DDC);
    w_u16(owner + 12, 0);
    w_u8(next + 1, 0);
    for (index = 0; index < 5; ++index)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + fields[index], 0);
    }
    next = r_u32(0x80112DDC);
    w_u32(owner + 320, 0);
    w_u32(owner + 16, 0);
    w_u8(next + 29, 0);
    for (index = 30; index < 36; ++index)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + index, 0);
    }
    resource002_sub_80100994();
    owner = r_u32(0x80112DDC);
    w_u32(owner + 200, 0x8010A784);
    w_u32(owner + 204, 0x8010A794);
    w_u32(owner + 208, 0x8010A7A0);
    w_u32(owner + 212, 0x8010A7B0);
    w_u32(owner + 184, 0x8010A8F0);
    w_u32(owner + 188, 0x8010A9E0);
    w_u32(owner + 192, 0x8010AA80);
    value = r_s16(owner + 12);
    w_u32(owner + 228, 0);
    w_u32(owner + 196, 0x8010AB50);
    if ((sint32)value < 4)
    {
        state = r_u32(0x80112DC4);
        table = r_u32(state + 444);
        resource002_sub_800ecf88(r_u32(table + value * 4));
    }
    resource002_sub_800d180c();
    resource002_sub_800d18e4();
    resource002_sub_800d19a8();
    resource002_sub_800d1a3c();
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 1);
    next = r_u32(0x80112DC4);
    context = r_u32(0x800BCE50);
    w_u32(state + 80, 0);
    w_u16(next + 454, 0);
    entries = sub_80027338(r_u32(context + 96), 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 28, entries);
    table = sub_80027338(r_u32(entries + 4), 0);
    resource002_sub_800de908(table, 22, 2);
    state = r_u32(0x80112DC4);
    entries = r_u32(state + 28);
    table = r_u32(state + 28);
    value = r_u32(entries + 8);
    w_u32(state + 152, value);
    value = r_u32(table + 16);
    w_u32(state + 160, value);
    value = r_u32(table + 20);
    w_u32(state + 164, value);
    value = r_u32(table + 24);
    w_u32(state + 168, value);
    return state;
}

uint32 resource002_sub_800de540(void)
{
    const uint32 fields[4] = {6, 2, 28, 29};
    uint32 state, phase, target, owner, next, index, table, value;

    FUNCTION_MARKER(0x800DE540, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 454);
    target = r_u32(0x8010984C + phase * 4);
    resource002_loading_phase_call(target, 0, 0);
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 454);
    if (phase != 3)
        return 3;
    owner = r_u32(0x80112DDC);
    w_u8(owner, 9);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 1, 0);
    owner = r_u32(0x80112DDC);
    w_u8(owner + 4, 0);
    next = r_u32(0x80112DDC);
    w_u16(owner + 12, 0);
    w_u8(next + 5, 0);
    for (index = 0; index < 3; ++index)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + fields[index], 0);
    }
    next = r_u32(0x80112DDC);
    w_u32(owner + 16, 0);
    w_u8(next + fields[3], 0);
    for (index = 30; index < 36; ++index)
    {
        owner = r_u32(0x80112DDC);
        w_u8(owner + index, 0);
    }
    resource002_sub_80100994();
    owner = r_u32(0x80112DDC);
    w_u32(owner + 200, 0x8010A784);
    w_u32(owner + 204, 0x8010A794);
    w_u32(owner + 208, 0x8010A7A0);
    w_u32(owner + 212, 0x8010A7B0);
    w_u32(owner + 184, 0x8010A8F0);
    w_u32(owner + 188, 0x8010A9E0);
    w_u32(owner + 192, 0x8010AA80);
    value = r_s16(owner + 12);
    w_u32(owner + 228, 0);
    w_u32(owner + 196, 0x8010AB50);
    if ((sint32)value < 4)
    {
        state = r_u32(0x80112DC4);
        table = r_u32(state + 444);
        resource002_sub_800ecf88(r_u32(table + value * 4));
    }
    resource002_sub_800d180c();
    resource002_sub_800d18e4();
    resource002_sub_800d19a8();
    resource002_sub_800d1a3c();
    state = r_u32(0x80112DC4);
    w_u32(state + 80, 0);
    w_u8(state + 125, 1);
    return 1;
}

uint32 resource002_sub_800d0c34(void)
{
    uint32 owner, count, page, item, last, value, flags, game, pointer, menu;
    uint32 source, index, status, state, countdown;

    FUNCTION_MARKER(0x800D0C34, "ACE3.BIN");
    status = sub_8002b528();
    resource002_sub_800dedf0(0, 11);
    countdown = r_u32(0x80112DD4);
    if (countdown != 0)
        w_u32(0x80112DD4, countdown - 1);
    else
    {
        owner = r_u32(0x80112DB8);
        count = r_u32(owner + 184);
        page = r_u8(owner + 2);
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        flags = (sint32)page < ((sint32)last >> 2) ? 2 : 0;
        if (page != 0)
            flags |= 1;
        resource002_sub_800def94(49, flags);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 51, 0);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 2) != 0)
    {
        owner = r_u32(0x80112DB8);
        page = r_u8(owner + 2);
        item = r_u8(owner + 3);
        count = r_u32(owner + 184);
        if ((sint32)(item + page * 4) < (sint32)(count - 1))
        {
            if (item < 3)
                w_u8(owner + 3, item + 1);
            else
            {
                w_u8(owner + 3, 0);
                owner = r_u32(0x80112DB8);
                page = r_u8(owner + 2);
                w_u8(owner + 2, page + 1);
                owner = r_u32(0x80112DB8);
                resource002_sub_800d0830(r_u8(owner + 2));
                pointer = r_u32(0x80112DC0);
                menu = r_u32(pointer + 92);
                w_u8(menu + 51, 2);
                w_u32(0x80112DD4, 12);
            }
            sub_8002db4c(61);
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0970(r_u8(owner + 3));
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 1) != 0)
    {
        owner = r_u32(0x80112DB8);
        item = r_u8(owner + 3);
        if (item != 0)
            w_u8(owner + 3, item - 1);
        else
        {
            page = r_u8(owner + 2);
            if (page == 0)
                goto selection_next_page;
            w_u8(owner + 3, 3);
            owner = r_u32(0x80112DB8);
            page = r_u8(owner + 2);
            w_u8(owner + 2, page - 1);
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0830(r_u8(owner + 2));
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 1);
            w_u32(0x80112DD4, 12);
        }
        sub_8002db4c(61);
        owner = r_u32(0x80112DB8);
        resource002_sub_800d0970(r_u8(owner + 3));
    }
selection_next_page:
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 512) != 0)
    {
        owner = r_u32(0x80112DB8);
        count = r_u32(owner + 184);
        page = r_u8(owner + 2);
        last = count - 1;
        if ((sint32)last < 0)
            last = count + 2;
        if ((sint32)page < ((sint32)last >> 2))
        {
            w_u8(owner + 2, page + 1);
            owner = r_u32(0x80112DB8);
            page = r_u8(owner + 2);
            item = r_u8(owner + 3);
            count = r_u32(owner + 184);
            if ((sint32)(item + page * 4) >= (sint32)count)
            {
                last = count - 1;
                value = (sint32)last < 0 ? count + 2 : last;
                w_u8(owner + 3, last - ((uint32)((sint32)value >> 2) << 2));
            }
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0830(r_u8(owner + 2));
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0970(r_u8(owner + 3));
            sub_8002db4c(61);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 2);
            w_u32(0x80112DD4, 12);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 304) & 256) != 0)
    {
        owner = r_u32(0x80112DB8);
        page = r_u8(owner + 2);
        if (page != 0)
        {
            w_u8(owner + 2, page - 1);
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0830(r_u8(owner + 2));
            owner = r_u32(0x80112DB8);
            resource002_sub_800d0970(r_u8(owner + 3));
            sub_8002db4c(61);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + 51, 1);
            w_u32(0x80112DD4, 12);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) != 0 && (r_u32(game + 296) & 47) == 0 && status != 1)
    {
        resource002_sub_800dee08(1, 0, 0, 0);
        resource002_sub_800dee08(2, 0, 0, 0);
        resource002_sub_800dee08(3, 0, 0, 0);
        resource002_sub_800dee08(4, 0, 0, 0);
        resource002_sub_800dee08(5, 0, 0, 0);
        resource002_sub_800def94(48, 1);
        state = r_u32(0x80112DC4);
        owner = r_u32(0x80112DB8);
        w_u16(state + 134, 0);
        page = r_u8(owner + 2);
        item = r_u8(owner + 3);
        value = r_u8(owner + page * 4 + item + 4);
        w_u8(state + 125, 4);
        w_u16(state + 292, value);
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 32) != 0 && (r_u32(game + 296) & 95) == 0 && status != 1)
    {
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 5);
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 0);
    }
    game = r_u32(0x800BCE60);
    pointer = r_u32(0x80112DC0);
    owner = r_u32(0x80112DB8);
    menu = r_u32(pointer + 92);
    item = r_u8(owner + 3);
    source = game + 3480;
    w_u8(menu + 52, item);
    for (index = 0; index < 4; ++index)
    {
        owner = r_u32(0x80112DB8);
        page = r_u8(owner + 2);
        count = r_u32(owner + 184);
        value = index + page * 4;
        if ((sint32)value < (sint32)count)
        {
            value = r_u8(owner + value + 4);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            value = r_u8(source + value);
            w_u8(menu + index + 53, value);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 2);
        }
        else
        {
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 53, 0);
            pointer = r_u32(0x80112DC0);
            menu = r_u32(pointer + 92);
            w_u8(menu + index + 57, 0);
        }
    }
    return 0;
}

uint32 sub_8002b46c(void)
{
    uint32 state;

    FUNCTION_MARKER(0x8002B46C, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    return sub_8002b470(0, 0, state);
}

uint32 resource002_sub_800f29e8(void)
{
    uint32 state, id, value;

    FUNCTION_MARKER(0x800F29E8, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    id = r_s16(state + 292);
    value = r_s16(0x8010B400 + id * 8 + 2);
    if (resource_is_cd_audio(value) == 1)
    {
        resource002_sub_800dedf0(0, 0);
        state = r_u32(0x80112DC4);
        arena_set_cursor(r_u32(state + 40));
        sub_8002db4c(60);
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 2);
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 0);
        return state;
    }
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800f2a8c(void)
{
    uint32 state, id, value, status, pointer, menu;

    FUNCTION_MARKER(0x800F2A8C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    id = r_s16(state + 292);
    value = r_s16(0x8010B400 + id * 8 + 2);
    status = sub_8002cba4(value);
    if (status != 1 && status != 3 && status != 2)
        return 2;
    sub_8002b428();
    resource002_sub_800dedf0(0, 0x0200000B);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 61, status == 2 ? 1 : 0);
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800f2b6c(void)
{
    uint32 status, state, game, result;

    FUNCTION_MARKER(0x800F2B6C, "ACE3.BIN");
    status = sub_8002b528();
    if (status == 3)
    {
        state = r_u32(0x80112DC4);
        result = r_u16(state + 134) + 1;
        w_u16(state + 134, result);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result != 0 && status != 1)
    {
        status = sub_8002b46c();
        state = r_u32(0x80112DC4);
        w_u8(state + 125, status == 1 ? 1 : 5);
        state = r_u32(0x80112DC4);
        w_u16(state + 134, status == 1 ? 0 : 3);
        return state;
    }
    return result;
}

uint32 resource002_sub_800f2c34(void)
{
    uint32 status, state, pointer, menu, game, result;

    FUNCTION_MARKER(0x800F2C34, "ACE3.BIN");
    status = sub_8002b528();
    if (status == 0 || status == 2)
    {
        state = r_u32(0x80112DC4);
        result = r_u16(state + 134) + 1;
        w_u16(state + 134, result);
    }
    else if (status == 1)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 61, 4);
    }
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 32;
    if (result != 0 && status != 1)
    {
        status = sub_8002b46c();
        state = r_u32(0x80112DC4);
        w_u8(state + 125, status == 1 ? 1 : 5);
        state = r_u32(0x80112DC4);
        w_u16(state + 134, status == 1 ? 0 : 3);
        return status == 1 ? state : 3;
    }
    return result;
}

uint32 resource002_sub_800f2d1c(void)
{
    uint32 state, id, value;

    FUNCTION_MARKER(0x800F2D1C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    id = r_s16(state + 292);
    value = r_s16(0x8010B400 + id * 8 + 2);
    if (resource_is_cd_audio(value) == 1)
    {
        resource002_sub_800dedf0(0, 0);
        state = r_u32(0x80112DC4);
        arena_set_cursor(r_u32(state + 40));
        sub_8002db4c(60);
        state = r_u32(0x80112DC4);
        w_u8(state + 125, 2);
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 0);
        return state;
    }
    state = r_u32(0x80112DC4);
    value = r_u16(state + 134) + 1;
    w_u16(state + 134, value);
    return value;
}

uint32 resource002_sub_800f2dc0(void)
{
    uint32 state, id, value, status, pointer, menu;

    FUNCTION_MARKER(0x800F2DC0, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    id = r_s16(state + 292);
    value = r_s16(0x8010B400 + id * 8 + 2);
    status = sub_8002cba4(value);
    if (status != 1 && status != 3 && status != 2)
        return 2;
    sub_8002b428();
    resource002_sub_800dedf0(0, 0x0200000B);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 61, status == 2 ? 3 : 2);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 2);
    return status == 2 ? state : 2;
}

uint32 resource002_sub_800f2eb0(void)
{
    uint32 status, game, mission, kind, state, owner, pointer, menu, result;

    FUNCTION_MARKER(0x800F2EB0, "ACE3.BIN");
    status = cd_resource_state();
    if (status == 1 || status == 2)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        kind = r_u8(0x80112750 + mission);
        if ((status == 1 && (kind == 1 || kind == 3)) || (status == 2 && kind - 2 < 2))
        {
            resource002_sub_800dedf0(0, 3);
            state = r_u32(0x80112DC4);
            w_u8(state + 124, 0);
            state = r_u32(0x80112DC4);
            w_u8(state + 125, 0);
            owner = r_u32(0x80112DB8);
            state = r_u32(0x80112DC4);
            w_u16(state + 454, 0);
            arena_set_cursor(owner);
            return sub_8002db4c(59);
        }
    }
    else if (status != 4 && status != 0xFFFFFFFF)
        return 0xFFFFFFFF;
    sub_8002b428();
    resource002_sub_800dedf0(0, 0x0200000B);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 61, status == 1 ? 1 : 0);
    state = r_u32(0x80112DC4);
    result = r_u16(state + 134) + 1;
    w_u16(state + 134, result);
    return result;
}

uint32 resource002_sub_800f3020(void)
{
    uint32 state, result;

    FUNCTION_MARKER(0x800F3020, "ACE3.BIN");
    if (sub_8002b528() != 3)
        return 0x80110000;
    state = r_u32(0x80112DC4);
    result = r_u16(state + 134) + 1;
    w_u16(state + 134, result);
    return result;
}

uint32 resource002_sub_800f3064(void)
{
    uint32 status, state, result, pointer, menu;

    FUNCTION_MARKER(0x800F3064, "ACE3.BIN");
    status = sub_8002b528();
    if (status == 0 || status == 2)
    {
        state = r_u32(0x80112DC4);
        result = r_u16(state + 134) + 1;
        w_u16(state + 134, result);
        return result;
    }
    if (status != 1)
        return 0x80110000;
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 61, 4);
    return 4;
}

uint32 resource002_sub_800f30d4(void)
{
    uint32 status, game, mission, kind, state, owner, pointer, menu, display;

    FUNCTION_MARKER(0x800F30D4, "ACE3.BIN");
    status = cd_resource_state();
    display = 2;
    if (status == 1 || status == 2)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        kind = r_u8(0x80112750 + mission);
        if (status == 1)
            display = 3;
        if ((status == 1 && (kind == 1 || kind == 3)) || (status == 2 && kind - 2 < 2))
        {
            resource002_sub_800dedf0(0, 3);
            state = r_u32(0x80112DC4);
            w_u8(state + 124, 0);
            state = r_u32(0x80112DC4);
            w_u8(state + 125, 0);
            owner = r_u32(0x80112DB8);
            state = r_u32(0x80112DC4);
            w_u16(state + 454, 0);
            arena_set_cursor(owner);
            return sub_8002db4c(59);
        }
    }
    else if (status != 4 && status != 0xFFFFFFFF)
        return 0xFFFFFFFF;
    sub_8002b428();
    resource002_sub_800dedf0(0, 0x0200000B);
    pointer = r_u32(0x80112DC0);
    menu = r_u32(pointer + 92);
    w_u8(menu + 61, display);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 1);
    return state;
}

uint32 resource002_sub_800d1258(void)
{
    uint32 state, phase, target;

    FUNCTION_MARKER(0x800D1258, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 134);
    target = r_u32(0x8010B9F0 + phase * 4);
    switch (target)
    {
        case 0x800F29E8:
            return resource002_sub_800f29e8();
        case 0x800F2A8C:
            return resource002_sub_800f2a8c();
        case 0x800F2B6C:
            return resource002_sub_800f2b6c();
        case 0x800F2C34:
            return resource002_sub_800f2c34();
        case 0x800F2D1C:
            return resource002_sub_800f2d1c();
        case 0x800F2DC0:
            return resource002_sub_800f2dc0();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 request callback 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800d129c(void)
{
    uint32 state, phase, target;

    FUNCTION_MARKER(0x800D129C, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 134);
    target = r_u32(0x8010BA08 + phase * 4);
    switch (target)
    {
        case 0x800F2EB0:
            return resource002_sub_800f2eb0();
        case 0x800F3020:
            return resource002_sub_800f3020();
        case 0x800F3064:
            return resource002_sub_800f3064();
        case 0x800F30D4:
            return resource002_sub_800f30d4();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 cancel callback 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800f0144(uint32 kind, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x800F0144, "ACE3.BIN");
    if ((sint32)kind < 0)
        return incoming_v0;
    return kind >= 3 && kind < 11;
}

uint32 resource002_sub_800f00a8(uint32 kind)
{
    sint32 value;

    FUNCTION_MARKER(0x800F00A8, "ACE3.BIN");
    value = (sint16)(uint16)kind;
    switch (value)
    {
        case 0:
        case 1:
            return 18;
        case 2:
            return 19;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            return 16;
        case 8:
        case 11:
            return 17;
        case 9:
            return 22;
        case 10:
            return 18;
        default:
            return 10;
    }
}

uint32 resource002_sub_800f07fc(uint32 owner)
{
    uint32 selection, id, context, resource, size, buffer, game;

    FUNCTION_MARKER(0x800F07FC, "ACE3.BIN");
    if (r_u32(owner + 12) == 0)
        return 0;
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    context = r_u32(0x800BCE50);
    resource = r_s16(0x8010B400 + id * 8);
    w_u32(context + 96, resource);
    size = resource_total_size(context + 96, 1);
    buffer = arena_alloc(size);
    game = r_u32(0x800BCE60);
    context = r_u32(0x800BCE50);
    w_u32(game + 68, 1);
    resource_queue_list(context + 96, 1, buffer, 0x8009B904);
    return 1;
}

uint32 resource002_sub_800f07f4(void)
{
    FUNCTION_MARKER(0x800F07F4, "ACE3.BIN");
    return resource002_sub_800f07fc(r_u32(0x80112DB0));
}

uint32 resource002_sub_800f0dc4(void)
{
    uint32 owner, state, pointer, index, selection, id, entry, kind, value;

    FUNCTION_MARKER(0x800F0DC4, "ACE3.BIN");
    owner = arena_alloc(6304);
    w_u32(0x80112DB0, owner);
    sub_8002cbcc();
    owner = r_u32(0x80112DB0);
    w_u16(owner + 2, 1);
    state = r_u32(0x80112DC4);
    w_u16(owner, 0);
    w_u32(owner + 16, 0);
    w_u32(owner + 12, 1);
    w_u32(owner + 24, state + 292);
    w_u32(owner + 6300, state + 292);
    resource002_sub_800f07f4();
    owner = r_u32(0x80112DB0);
    w_u8(owner + 3886, 0);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3896, 0);
    resource002_sub_800deca8(0, 21);
    resource002_sub_800deca8(0, 14);
    resource002_sub_800dee08(11, 1, 0, 25);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3888, 0x204001);
    resource002_sub_800dedf0(0, 0x204001);
    owner = r_u32(0x80112DB0);
    w_u8(owner + 3912, 0);
    owner = r_u32(0x80112DB0);
    w_u8(owner + 3913, 0);
    owner = r_u32(0x80112DB0);
    pointer = owner + 2400;
    for (index = 0; index < 16; ++index)
    {
        w_u16(pointer + 1324, 0);
        pointer -= 160;
    }
    owner = r_u32(0x80112DB0);
    pointer = owner + 480;
    for (index = 0; index < 16; ++index)
    {
        w_u16(pointer + 5200, 0);
        pointer -= 32;
    }
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    entry = 0x8010B400 + id * 8;
    kind = r_u8(entry + 6);
    value = resource002_sub_800f0144(kind, entry);
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    w_u32(owner + 28, value);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    value = resource002_sub_800f00a8(kind);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 28, value);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 4);
    return sub_8002db4c(90);
}

uint32 resource002_sub_800ee9e4(void)
{
    FUNCTION_MARKER(0x800EE9E4, "ACE3.BIN");
    return resource002_sub_800ee9ec(r_u32(0x80112DC4));
}

uint32 resource002_sub_800f13e4(uint32 owner)
{
    uint32 value, state;

    FUNCTION_MARKER(0x800F13E4, "ACE3.BIN");
    value = r_u32(owner + 16);
    if ((sint32)value < 60)
        w_u32(owner + 16, value + 1);
    else
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 5);
    }
    return resource002_sub_800ee9e4();
}

uint32 resource002_sub_800f13dc(void)
{
    FUNCTION_MARKER(0x800F13DC, "ACE3.BIN");
    return resource002_sub_800f13e4(r_u32(0x80112DB0));
}

uint32 resource002_sub_800f0734(uint32 owner)
{
    uint32 state, value, descriptor;

    FUNCTION_MARKER(0x800F0734, "ACE3.BIN");
    if (r_u32(owner + 5784) == 0)
        return 0;
    state = r_u32(0x80112DC4);
    value = r_u32(state + 300);
    descriptor = owner + 5788;
    value = value == 0 ? 0x912 : r_u16(state + 300);
    w_u16(owner + 5788, value);
    w_u16(descriptor + 2, 0);
    w_u16(descriptor + 4, 0);
    w_u16(descriptor + 6, 0);
    w_u32(descriptor + 8, 0);
    resource_queue_audio(descriptor, 1, 0);
    return 1;
}

uint32 resource002_sub_800f072c(void)
{
    FUNCTION_MARKER(0x800F072C, "ACE3.BIN");
    return resource002_sub_800f0734(r_u32(0x80112DB0));
}

uint32 resource002_sub_800f16c4(void)
{
    uint32 owner, selection, id, resource, state, pointer, menu, bits;

    FUNCTION_MARKER(0x800F16C4, "ACE3.BIN");
    resource002_sub_800ee9e4();
    owner = r_u32(0x80112DB0);
    w_u32(owner + 5784, 1);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    resource = r_s16(0x8010B400 + id * 8);
    switch (resource)
    {
        case 0x16A:
        case 0x174:
        case 0x178:
            state = r_u32(0x80112DC4);
            w_u32(state + 300, 0x912);
            break;
        case 0x10E:
        case 0x14B:
            state = r_u32(0x80112DC4);
            w_u32(state + 300, 0x913);
            break;
        case 0x135:
            state = r_u32(0x80112DC4);
            w_u32(state + 300, 0x915);
            break;
        default:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 5784, 0);
            break;
    }
    if (resource002_sub_800f072c() != 0)
    {
        owner = r_u32(0x80112DB0);
        bits = r_u32(owner + 3888);
        resource002_sub_800dedf0(0, bits | 0x800000);
        resource002_sub_800dee08(3, 1, 0, 10);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50, 0);
    }
    owner = r_u32(0x80112DB0);
    if (r_u32(owner + 5784) != 0)
        sub_8002db4c(55);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 7);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3908, 180);
    return 180;
}

static uint32 resource002_frame_quotient(uint32 value, uint32 multiplier, uint32 shift, uint32 add_value)
{
    sint64 product;
    uint32 high;

    product = (sint64)(sint32)value * (sint32)multiplier;
    high = (uint32)(product >> 32);
    if (add_value != 0)
        high += value;
    return (uint32)((sint32)high >> shift) - (uint32)((sint32)value >> 31);
}

uint32 resource002_sub_800ee9ec(uint32 state)
{
    uint32 phase, owner, timer, width, height, mode, count, index, angle;
    uint32 value, x, y, y_outer, left, right, top, outer_left, outer_right, outer_top;

    FUNCTION_MARKER(0x800EE9EC, "ACE3.BIN");
    phase = r_s16(state + 134);
    if ((sint32)phase >= 0 && (phase < 3 || phase == 15))
        return 0;
    state = r_u32(0x80112DC4);
    phase = r_u16(state + 134);
    if (phase - 5 >= 9)
    {
        owner = r_u32(0x80112DB0);
        timer = r_u32(owner + 16);
        if ((sint32)timer < 23)
        {
            width = resource002_frame_quotient((timer * 5) << 5, 0xB21642C9, 4, 1);
            resource002_sub_800e1040(160 - width, 240, width, 2, 0);
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 16);
            width = resource002_frame_quotient((timer * 5) << 5, 0xB21642C9, 4, 1);
            resource002_sub_800e1040(160, 240, width, 2, 0);
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 16);
            height = resource002_frame_quotient((timer * 15) << 4, 0xB21642C9, 4, 1);
            resource002_sub_800e0f88(160, 240 - height, 160, 240);
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 16);
            height = resource002_frame_quotient((timer * 15) << 4, 0xB21642C9, 4, 1);
            resource002_sub_800e0f88(160, 240, 160, 240 + height);
            goto frame_detail;
        }
    }
    resource002_sub_800e1040(0, 240, 160, 2, 0);
    resource002_sub_800e1040(160, 240, 160, 2, 0);
    resource002_sub_800e0f88(160, 0, 160, 240);
    resource002_sub_800e0f88(160, 240, 160, 480);
frame_detail:
    owner = r_u32(0x80112DB0);
    mode = r_u32(owner + 40);
    if (mode == 1)
    {
        state = r_u32(0x80112DC4);
        phase = r_u16(state + 134);
        if (phase - 5 >= 9)
        {
            timer = r_u32(owner + 16);
            if ((sint32)timer < 11)
            {
                width = resource002_frame_quotient((timer * 15) << 3, 0x2E8BA2E9, 1, 0);
                height = resource002_frame_quotient((timer * 5) << 5, 0x2E8BA2E9, 1, 0);
                width = (uint32)(sint32)(sint16)width;
                x = 140 - width;
                resource002_sub_800e1040(x, 220, width, 2, 0);
                resource002_sub_800e1040(180, 220, width, 2, 0);
                resource002_sub_800e1040(x, 260, width, 2, 0);
                resource002_sub_800e1040(180, 260, width, 2, 0);
                height = (uint32)(sint32)(sint16)height;
                resource002_sub_800e0f88(140, 220 - height, 140, 220);
                resource002_sub_800e0f88(180, 220 - height, 180, 220);
                resource002_sub_800e0f88(140, 260, 140, 260 + height);
                resource002_sub_800e0f88(180, 260, 180, 260 + height);
                goto frame_mode;
            }
            if ((sint32)timer < 23)
            {
                value = (timer - 11) * 5;
                height = resource002_frame_quotient(value << 5, 0x2AAAAAAB, 1, 0);
                resource002_sub_800e1040(20, 220, 120, 2, 0);
                resource002_sub_800e1040(180, 220, 120, 2, 0);
                resource002_sub_800e1040(20, 260, 120, 2, 0);
                resource002_sub_800e1040(180, 260, 120, 2, 0);
                resource002_sub_800e0f88(140, 60, 140, 220);
                resource002_sub_800e0f88(180, 60, 180, 220);
                resource002_sub_800e0f88(140, 260, 140, 420);
                resource002_sub_800e0f88(180, 260, 180, 420);
                width = (uint32)(sint32)(sint16)(value << 1);
                x = 140 - width;
                resource002_sub_800e1040(x, 60, width, 2, 0);
                resource002_sub_800e1040(180, 60, width, 2, 0);
                resource002_sub_800e1040(x, 420, width, 2, 0);
                resource002_sub_800e1040(180, 420, width, 2, 0);
                height = (uint32)(sint32)(sint16)height;
                resource002_sub_800e0f88(20, 220 - height, 20, 220);
                resource002_sub_800e0f88(300, 220 - height, 300, 220);
                resource002_sub_800e0f88(20, 260, 20, 260 + height);
                resource002_sub_800e0f88(300, 260, 300, 260 + height);
                goto frame_mode;
            }
        }
        resource002_sub_800e1040(20, 220, 120, 2, 0);
        resource002_sub_800e1040(180, 220, 120, 2, 0);
        resource002_sub_800e1040(20, 260, 120, 2, 0);
        resource002_sub_800e1040(180, 260, 120, 2, 0);
        resource002_sub_800e0f88(140, 60, 140, 220);
        resource002_sub_800e0f88(180, 60, 180, 220);
        resource002_sub_800e0f88(140, 260, 140, 420);
        resource002_sub_800e0f88(180, 260, 180, 420);
        resource002_sub_800e1040(20, 60, 120, 2, 0);
        resource002_sub_800e1040(180, 60, 120, 2, 0);
        resource002_sub_800e1040(20, 420, 120, 2, 0);
        resource002_sub_800e1040(180, 420, 120, 2, 0);
        resource002_sub_800e0f88(20, 60, 20, 220);
        resource002_sub_800e0f88(300, 60, 300, 220);
        resource002_sub_800e0f88(20, 260, 20, 420);
        resource002_sub_800e0f88(300, 260, 300, 420);
    }
frame_mode:
    owner = r_u32(0x80112DB0);
    mode = r_u32(owner + 40);
    if (mode != 0)
        return mode;
    state = r_u32(0x80112DC4);
    phase = r_u16(state + 134);
    if (phase - 5 >= 9)
    {
        timer = r_u32(owner + 16);
        if ((sint32)timer < 7)
        {
            width = resource002_frame_quotient((timer * 5) << 4, 0x92492493, 2, 1);
            resource002_sub_800e1040(160 - width, 290, width, 2, 0);
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 16);
            width = resource002_frame_quotient((timer * 5) << 4, 0x92492493, 2, 1);
            resource002_sub_800e1040(160, 290, width, 2, 0);
            goto frame_upper;
        }
    }
    resource002_sub_800e1040(80, 290, 80, 2, 0);
    resource002_sub_800e1040(160, 290, 80, 2, 0);
frame_upper:
    state = r_u32(0x80112DC4);
    phase = r_u16(state + 134);
    if (phase - 5 >= 9)
    {
        owner = r_u32(0x80112DB0);
        timer = r_u32(owner + 16);
        if ((sint32)timer < 23)
            goto frame_arc;
        if ((sint32)timer < 30)
        {
            width = resource002_frame_quotient(((timer - 23) * 5) << 4, 0x92492493, 2, 1);
            resource002_sub_800e1040(80, 73, width, 2, 0);
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 16);
            width = resource002_frame_quotient(((timer - 23) * 5) << 4, 0x92492493, 2, 1);
            resource002_sub_800e1040(240 - width, 73, width, 2, 0);
            goto frame_arc;
        }
    }
    resource002_sub_800e1040(80, 73, 80, 2, 0);
    resource002_sub_800e1040(160, 73, 80, 2, 0);
frame_arc:
    state = r_u32(0x80112DC4);
    phase = r_u16(state + 134);
    count = 16;
    if (phase - 5 >= 9)
    {
        owner = r_u32(0x80112DB0);
        timer = r_u32(owner + 16);
        if ((sint32)timer < 7)
            count = 0;
        else if ((sint32)timer < 23)
            count = timer - 7;
    }
    left = 80;
    right = 240;
    top = 290;
    outer_left = 80;
    outer_right = 240;
    outer_top = 291;
    for (index = 1; index <= count; ++index)
    {
        angle = 3072 - (index << 7);
        value = (uint32)rcos((sint32)angle) * 54;
        if ((sint32)value < 0)
            value += 4095;
        x = (uint32)((sint32)value >> 12);
        value = (uint32)rsin((sint32)angle) * 108;
        if ((sint32)value < 0)
            value += 4095;
        y = (uint32)((sint32)value >> 12);
        resource002_sub_800e0f88((uint32)(sint32)(sint16)left, (uint32)(sint32)(sint16)top, 80 + (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y);
        resource002_sub_800e0f88((uint32)(sint32)(sint16)right, (uint32)(sint32)(sint16)top, 240 - (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y);
        left = 80 + x;
        right = 240 - x;
        top = 182 - y;
        value = (uint32)rsin((sint32)angle) * 109;
        if ((sint32)value < 0)
            value += 4095;
        y_outer = (uint32)((sint32)value >> 12);
        resource002_sub_800e0f88((uint32)(sint32)(sint16)outer_left, (uint32)(sint32)(sint16)outer_top, 80 + (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y_outer);
        resource002_sub_800e0f88((uint32)(sint32)(sint16)outer_right, (uint32)(sint32)(sint16)outer_top, 240 - (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y_outer);
        outer_left = left;
        outer_right = right;
        outer_top = 182 - y_outer;
    }
    return 1;
}
