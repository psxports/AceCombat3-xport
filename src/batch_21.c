#include "game_functions.h"
#include <stdlib.h>

uint32 resource001_sub_800d3584(void)
{
    uint32 state, game, port, status, flags, value, i, result;
    sint32 selection;
    FUNCTION_MARKER(0x800D3584, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    port = r_u8(state + 90);
    status = resource001_sub_800d3d90(port);
    resource001_sub_800d7d08(4);
    resource001_sub_800d7c60(128, 128, 128);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7c40(106, 166);
    resource001_sub_800d8aa0(0x800CC078, 0, 0, 0);
    resource001_sub_800d7c40(106, 218);
    resource001_sub_800d8aa0(0x800CC080, 0, 0, 0);
    resource001_sub_800d7c40(106, 270);
    resource001_sub_800d8aa0(0x800CC088, 0, 0, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if (flags & 2)
    {
        selection = r_s16(0x800DD5F0);
        value = r_u16(0x800DD5F0);
        w_u16(0x800DD5F0, selection < 2 ? value + 1 : 0);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if (flags & 1)
    {
        selection = r_s16(0x800DD5F0);
        value = r_u16(0x800DD5F0);
        w_u16(0x800DD5F0, selection > 0 ? value - 1 : 2);
        sub_8002db4c(61);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 300);
    if (flags & 0x10)
    {
        selection = r_s16(0x800DD5F0);
        w_u32(game + 916, (uint32)selection);
        for (i = 0; i < 60; ++i)
        {
            game = r_u32(0x800BCE60);
            w_u8(game + i + 0xD0C, 0);
            game = r_u32(0x800BCE60);
            w_u16(game + i * 2 + 0xBA4, 0xD2F0);
            w_u32(game + i * 4 + 0xE4C, 0);
            w_u32(game + i * 4 + 0xF3C, 0);
        }
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 11);
        w_u16(state + 78, 0);
        sub_8002db4c(89);
        game = r_u32(0x800BCE60);
        w_u16(game + 0x954, 2);
        w_u16(game + 908, 1);
    }
    else if (flags & 0x20)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 9);
        w_u16(state + 78, 0);
        sub_8002db4c(59);
    }
    for (i = 0; i < 3; ++i)
    {
        selection = r_s16(0x800DD5F0);
        resource001_sub_800da9a8(i + 6, i != (uint32)selection);
    }
    result = 6;
    if (status == 2 || status == 6)
    {
        state = r_u32(0x800DD9D0);
        w_u16(state + 76, 2);
        w_u16(state + 78, 0);
        result = resource001_sub_800dbf6c(111, 148, 0, 0);
    }
    return result;
}

uint32 resource001_sub_800d47b8(uint32 slot)
{
    uint32 state, source, destination, i, a, b, c, d;
    FUNCTION_MARKER(0x800D47B8, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    source = r_u32(state + slot * 4 + 748);
    destination = r_u32(state + 760);
    for (i = 0; i < 72; ++i)
    {
        a = r_u32(source);
        b = r_u32(source + 4);
        c = r_u32(source + 8);
        d = r_u32(source + 12);
        w_u32(destination, a);
        w_u32(destination + 4, b);
        w_u32(destination + 8, c);
        w_u32(destination + 12, d);
        source += 16;
        destination += 16;
    }
    return destination;
}

uint32 resource001_sub_800d5ca0(void)
{
    uint32 state, buffer;
    FUNCTION_MARKER(0x800D5CA0, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + 760);
    return (uint32)r_s16(buffer + 22);
}

uint32 resource001_sub_800d2ba0(void)
{
    static const uint32 offsets[5] = {0x2E2, 0x2E7, 0x2D2, 0x2F5, 0x2F0};
    uint32 game, data, group, value, next;
    FUNCTION_MARKER(0x800D2BA0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    data = game + 0x95C;
    for (group = 0; group < 5; ++group)
    {
        value = r_u8(data + offsets[group]);
        next = r_u8(data + offsets[group] + 60);
        value |= next;
        next = r_u8(data + offsets[group] + 120);
        value |= next;
        next = r_u8(data + offsets[group] + 180);
        value |= next;
        if (value == 0)
            return 0;
    }
    return 1;
}

uint32 resource001_sub_800d4df4(uint32 selection)
{
    uint32 state, game, source, data, value, i;
    FUNCTION_MARKER(0x800D4DF4, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    game = r_u32(0x800BCE60);
    source = r_u32(state + (selection & 255) * 4 + 764);
    data = game + 0x95C;
    w_u8(data + 1, selection);
    value = r_u32(source + 0x114);
    game = r_u32(0x800BCE60);
    w_u32(data + 0x6D0, value);
    value = r_u8(source + 0x113);
    w_u16(game + 908, value);
    value = r_u8(source + 0x112);
    w_u32(game + 916, value);
    for (i = 0; i < 60; ++i)
    {
        value = r_u16(source + i * 2 + 4);
        w_u16(data + i * 2 + 0x248, value);
        value = r_u8(source + i + 0x7C);
        w_u8(data + i + 0x3B0, value);
        value = r_u8(source + i + 0x9A);
        w_u32(data + i * 4 + 0x4F0, value);
        value = r_u8(source + i + 0xD6);
        game = r_u32(0x800BCE60);
        w_u32(data + i * 4 + 0x5E0, value);
        value = r_u8(source + 0x118);
        w_u16(game + 910, value);
        value = r_u8(source + 0x119);
        w_u8(game + 937, value);
        game = r_u32(0x800BCE60);
        value = r_u8(source + 0x11A);
        w_u8(game + 938, value);
        game = r_u32(0x800BCE60);
        value = r_u8(source + 0x11B);
        w_u8(game + 939, value);
    }
    for (i = 0; i < 30; ++i)
    {
        value = r_u8(source + i + 0x7C);
        w_u8(data + i * 2 + 0x3B0, value & 15);
        value = r_u8(source + i + 0x7C);
        w_u8(data + i * 2 + 0x3B0 + 1, value >> 4);
    }
    return 0;
}

uint32 resource001_sub_800d4808(uint32 destination)
{
    static const uint32 half_source[4] = {0x22, 0x9A, 0x112, 0x18A};
    static const uint32 half_destination[4] = {0x68, 0xE0, 0x158, 0x1D0};
    static const uint32 nibble_source[4] = {0x202, 0x220, 0x23E, 0x25C};
    static const uint32 nibble_destination[4] = {0x2C0, 0x2FC, 0x338, 0x374};
    uint32 state, buffer, game, data, value, flags, fresh, display, i, j, row, column, group, count;
    uint32 record, offset, source, target, lane, partial, second, words[4];
    FUNCTION_MARKER(0x800D4808, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + 760);
    game = r_u32(0x800BCE60);
    value = r_u8(buffer + 20);
    data = game + 0x95C;
    w_u8(destination, value);
    state = r_u32(0x800DDA48);
    game = r_u32(0x800BCE60);
    fresh = r_u32(state + 760);
    sub_800adea8(game + 0x948, fresh + 8);
    state = r_u32(0x800DDA48);
    fresh = r_u32(state + 760);
    game = r_u32(0x800BCE60);
    value = r_u32(fresh + 4);
    w_u32(game + 0x968, value);
    for (i = 0; i < 60; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = r_u16(buffer + half_source[j] + i * 2);
            w_u16(data + half_destination[j] + i * 2, value);
        }
    for (i = 0; i < 30; ++i)
        for (column = 0; column < 2; ++column)
            for (j = 0; j < 4; ++j)
            {
                value = r_u8(buffer + nibble_source[j] + i);
                w_u8(data + nibble_destination[j] + i * 2 + column, (value >> (column * 4)) & 15);
            }
    value = (uint32)r_s16(buffer + 0x2C0);
    w_u32(data + 0x6D4, value);
    value = (uint32)r_s16(buffer + 0x2C2);
    w_u32(data + 0x6D8, value);
    value = r_u8(buffer + 20);
    w_u8(data + 1, value);
    for (i = 0; i < 6; ++i)
    {
        record = buffer + 0x2C4 + i * 0x44;
        offset = i * 0x5C;
        game = r_u32(0x800BCE60);
        for (j = 0; j < 20; ++j)
        {
            value = r_u16(record + j * 2);
            w_u16(game + offset + 0x172 + j * 2, value);
        }
        game = r_u32(0x800BCE60);
        for (group = 0; group < 2; ++group)
        {
            count = group == 0 ? 4 : 2;
            source = record + (group == 0 ? 0x28 : 0x38);
            target = game + offset + (group == 0 ? 0x19C : 0x1AC);
            for (j = 0; j < count; ++j)
            {
                lane = (source + j * 4 + 3) & 3;
                partial = r_masked_u32((source + j * 4 + 3) & ~3, (1 << (lane + 1)) - 1);
                partial <<= (3 - lane) * 8;
                lane = (source + j * 4) & 3;
                second = r_masked_u32((source + j * 4) & ~3, (15 << lane) & 15);
                words[j] = lane == 0 ? second : (partial & (0xFFFFFFFF << (32 - lane * 8))) | (second >> (lane * 8));
            }
            for (j = 0; j < count; ++j)
            {
                lane = (target + j * 4 + 3) & 3;
                w_masked_u32((target + j * 4 + 3) & ~3, words[j] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
                lane = (target + j * 4) & 3;
                w_masked_u32((target + j * 4) & ~3, words[j] << (lane * 8), (15 << lane) & 15);
            }
        }
        value = r_u32(record + 0x40);
        w_u32(game + offset + 0x1B4, value);
    }
    flags = r_u8(buffer + 0x464);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 0x120);
    w_u32(game + 0x120, flags == 0 ? value | 4 : value & ~4);
    state = r_u32(0x800DDA48);
    fresh = r_u32(state + 760);
    display = r_u32(0x800BCE48);
    value = (uint32)r_s16(fresh + 0x460);
    w_u32(display + 8, value);
    fresh = r_u32(state + 760);
    value = (uint32)r_s16(fresh + 0x462);
    w_u32(display + 12, value);
    game = r_u32(0x800BCE60);
    value = r_u8(buffer + 0x45D);
    w_u8(game + 920, value);
    game = r_u32(0x800BCE60);
    value = r_u8(buffer + 0x45E);
    w_u8(game + 921, value);
    game = r_u32(0x800BCE60);
    value = r_u8(buffer + 0x45C);
    w_u8(game + 922, value);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 922);
    sub_8002bd28((value & 1) != 0);
    sub_8002bd5c();
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 8 : 2;
        source = buffer + (group == 0 ? 26 : 24);
        target = data + (group == 0 ? 24 : 88);
        for (row = 0; row < count; ++row)
            for (column = 0; column < 8; ++column)
            {
                value = r_u8(source + row);
                w_u8(target + row * 8 + column, (value >> column) & 1);
            }
    }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 20 : 45;
        source = buffer + (group == 0 ? 0x27A : 0x28E);
        target = data + (group == 0 ? 0x3EC : 0x43C);
        for (row = 0; row < count; ++row)
            for (column = 0; column < 4; ++column)
            {
                value = r_u8(source + row);
                w_u8(target + row * 4 + column, (value >> (column * 2)) & 3);
            }
    }
    flags = r_u32(buffer + 0x468);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 924);
    w_u8(game + 924, flags & 1 ? value | 1 : value & 254);
    flags = r_u32(buffer + 0x468);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 0x120);
    w_u32(game + 0x120, flags & 2 ? value | 1 : value & ~1);
    flags = r_u32(buffer + 0x468);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 0x120);
    w_u32(game + 0x120, flags & 4 ? value | 2 : value & ~2);
    flags = r_u32(buffer + 0x468);
    game = r_u32(0x800BCE60);
    w_u8(game + 0x95A, (flags & 8) != 0);
    display = r_u32(0x800BCE48);
    value = r_u32(display) | 2;
    w_u32(display, value);
    return value;
}

uint32 resource001_sub_800d6ab4(uint32 port, uint32 slot, uint32 finalize)
{
    uint32 state, buffer, mask, descriptor, phase, active, retry, offset, outcome, checksum, value, i;
    sint32 index, attempts;
    FUNCTION_MARKER(0x800D6AB4, "ACE3.BIN");
    if (!resource001_sub_800d3d60())
        return 0;
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + slot * 4 + 748);
    index = r_s16(state + 80);
    mask = (uint32)r_s16(buffer + 22);
    if (((mask >> ((uint32)index & 31)) & 1) == 0)
        goto advance;
    if (!resource001_sub_800d3d60())
        return 0;
    state = r_u32(0x800DDA48);
    phase = (uint32)r_s16(state + 858);
    if (phase == 0)
    {
        w_u16(state + 858, 1);
        w_u16(state + 856, 0);
        state = r_u32(0x800DDA48);
    }
    active = r_u8(state + 822);
    if (active == 0)
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
            w_u16(state + 858, 0);
            resource001_sub_800d3ce8();
            state = r_u32(0x800DDA48);
            w_u16(state + 80, 0);
            return 0xFFFFFFFF;
        }
        index = r_s16(state + 80);
        offset = slot * 2304 + 0x1080 + (uint32)index * 384;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        state = r_u32(0x800DDA48);
        index = r_s16(state + 80);
        descriptor = r_u32(state + 60);
        buffer = r_u32(state + (uint32)index * 4 + 764);
        game_bios_read_guest(descriptor, buffer, 384);
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
        state = r_u32(0x800DDA48);
        w_u16(state + 80, 0);
        return 0xFFFFFFFF;
    }
    state = r_u32(0x800DDA48);
    index = r_s16(state + 80);
    buffer = r_u32(state + (uint32)index * 4 + 764);
    checksum = 0x41434533;
    for (i = 0; i < 95; ++i)
    {
        value = r_u32(buffer + 4 + i * 4);
        checksum ^= value;
    }
    state = r_u32(0x800DDA48);
    index = r_s16(state + 80);
    buffer = r_u32(state + (uint32)index * 4 + 764);
    value = r_u32(buffer);
    if (value != checksum)
    {
        resource001_sub_800d3ce8();
        state = r_u32(0x800DDA48);
        w_u16(state + 858, 0);
        w_u16(state + 80, 0);
        return 2;
    }
advance:
    state = r_u32(0x800DDA48);
    index = r_s16(state + 80);
    value = r_u16(state + 80);
    if (index < 6)
    {
        w_u16(state + 80, value + 1);
        return 0;
    }
    if (finalize == 1)
        resource001_sub_800d3ce8();
    state = r_u32(0x800DDA48);
    w_u16(state + 858, 0);
    w_u16(state + 80, 0);
    return 1;
}

uint32 resource001_sub_800d2c84(uint32 slot, uint32 selection)
{
    uint32 value, text;
    FUNCTION_MARKER(0x800D2C84, "ACE3.BIN");
    resource001_sub_800d7d08(4);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7ce4(1, 10, 20);
    value = (uint32)resource001_sub_800da8bc(15);
    resource001_sub_800d7cd4(value);
    resource001_sub_800d7c60(128, 128, 128);
    resource001_sub_800d7c40(64, 50);
    text = resource001_sub_800d5970(0);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    resource001_sub_800d7d08(3);
    selection &= 255;
    if (selection == 6)
    {
        resource001_sub_800d7c40(140, 327);
        return resource001_sub_800d8aa0(0x800CC038, 0, 0, 0);
    }
    slot &= 255;
    if (selection == 7)
    {
        resource001_sub_800d7c40(140, 327);
        return resource001_sub_800d8aa0(0x800CC044, 0, 0, 0);
    }
    if (!resource001_sub_800d59c0(slot, selection))
    {
        resource001_sub_800d7c40(140, 327);
        return resource001_sub_800d8aa0(0x800CC070, 0, 0, 0);
    }
    resource001_sub_800d7c40(106, 280);
    text = resource001_sub_800d5bf4(selection);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    resource001_sub_800d7c40(106, 320);
    resource001_sub_800d8aa0(0x800CC050, 0, 0, 0);
    resource001_sub_800d7c40(150, 320);
    text = resource001_sub_800d5a70(selection);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    resource001_sub_800d7c40(106, 340);
    resource001_sub_800d8aa0(0x800CC058, 0, 0, 0);
    resource001_sub_800d7c40(150, 340);
    text = resource001_sub_800d5ac8(selection);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    resource001_sub_800d7c40(106, 360);
    resource001_sub_800d8aa0(0x800CC060, 0, 0, 0);
    resource001_sub_800d7c40(150, 360);
    text = resource001_sub_800d5c2c(selection);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    resource001_sub_800d7c40(106, 380);
    resource001_sub_800d8aa0(0x800CC068, 0, 0, 0);
    resource001_sub_800d7c40(150, 380);
    text = resource001_sub_800d5cc0(slot);
    return resource001_sub_800d8aa0(text, 0, 0, 0);
}

uint32 resource001_sub_800d59c0(uint32 slot, uint32 selection)
{
    uint32 state, buffer, mask;
    FUNCTION_MARKER(0x800D59C0, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + (slot == 0xFFFFFFFF ? 760 : slot * 4 + 748));
    mask = (uint32)r_s16(buffer + 22);
    return (mask >> (selection & 31)) & 1;
}

uint32 resource001_sub_800d5a70(uint32 selection)
{
    uint32 state, buffer, index;
    FUNCTION_MARKER(0x800D5A70, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + (selection & 255) * 4 + 764);
    index = r_u8(buffer + 0x112);
    return r_u32(0x800DD7F0 + index * 4);
}

uint32 resource001_sub_800d5bf4(uint32 selection)
{
    uint32 state, buffer, index;
    FUNCTION_MARKER(0x800D5BF4, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + (selection & 255) * 4 + 764);
    index = r_u8(buffer + 0x113);
    return r_u32(0x800DD6A4 + index * 4);
}

uint32 resource001_sub_800d5c2c(uint32 selection)
{
    uint32 state, buffer, index;
    FUNCTION_MARKER(0x800D5C2C, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + (selection & 255) * 4 + 764);
    index = r_u8(buffer + 0x113);
    index = r_u8(0x800DD7B0 + index);
    return r_u32(0x800DD798 + index * 4);
}

uint32 resource001_sub_800d5cc0(uint32 slot)
{
    uint32 state, buffer, index;
    FUNCTION_MARKER(0x800D5CC0, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + slot * 4 + 748);
    index = r_u8(buffer + 0x464);
    return r_u32(0x800DD800 + index * 4);
}

uint32 resource001_sub_800d5ac8(uint32 selection)
{
    uint32 state, buffer, ticks, arguments[3], i, value;
    sint32 elapsed;
    FUNCTION_MARKER(0x800D5AC8, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + (selection & 255) * 4 + 764);
    elapsed = r_s32(buffer + 0x114);
    if (elapsed > 107999999)
        elapsed = 107999999;
    if (elapsed < 0)
        elapsed = 0;
    ticks = (uint32)elapsed;
    arguments[0] = ticks / 108000;
    arguments[1] = ticks / 1800 - arguments[0] * 60;
    arguments[2] = ticks / 30 - (ticks / 1800) * 60;
    game_sprintf_guest_words(0x800DDA38, 0x800CC52C, arguments, 3);
    for (i = 0; i < 12; ++i)
    {
        value = r_u8(0x800DDA38 + i);
        if (value == 32)
            w_u8(0x800DDA38 + i, 48);
    }
    return 0x800DDA38;
}

uint32 resource001_sub_800dc098(uint32 layout, uint32 hide_selection)
{
    static const uint32 first_y[3] = {150, 204, 257};
    static const uint32 second_y[3] = {189, 242, 293};
    uint32 available, state, selection, render, flags, i, x, present, text, result;
    FUNCTION_MARKER(0x800DC098, "ACE3.BIN");
    available = resource001_sub_800d5c80();
    if (hide_selection == 1)
    {
        state = r_u32(0x800DD9D0);
        selection = r_u8(state + 93);
        available &= ~((uint32)1 << (selection & 31));
    }
    render = r_u32(0x800DD9DC);
    flags = r_u32(render + 84) & 2;
    if (flags == 0)
        return 0;
    if (layout != 0 && layout != 1)
        return 1;
    resource001_sub_800d7d08(4);
    if (layout == 0)
    {
        resource001_sub_800d7ce4(1, 1, 1);
        resource001_sub_800d7c88(2);
    }
    else
    {
        resource001_sub_800d7c88(2);
        resource001_sub_800d7ce4(1, 1, 1);
    }
    resource001_sub_800d7cd4(1);
    result = 1;
    for (i = 0; i < 3; ++i)
    {
        x = 106;
        if (layout == 1)
        {
            render = r_u32(0x800DD9DC);
            flags = r_u32(render + 84);
            if (flags & 8)
                x = 81;
        }
        resource001_sub_800d7c40(x, layout == 0 ? first_y[i] : second_y[i]);
        present = (available >> i) & 1;
        resource001_sub_800d7c60(present ? 128 : 64, present ? 128 : 64, present ? 128 : 64);
        if (layout == 1 && i != 1)
            resource001_sub_800da9a8(i == 0 ? 2 : 4, present == 0);
        text = present ? resource001_sub_800d5970(i + 1) : 0x800CC5B0;
        result = resource001_sub_800d8aa0(text, 0, 0, 0);
    }
    return layout == 0 ? 1 : result;
}

uint32 resource001_sub_800dbfe4(void)
{
    uint32 render, flags;
    sint32 x, y;
    FUNCTION_MARKER(0x800DBFE4, "ACE3.BIN");
    render = r_u32(0x800DD9DC);
    flags = r_u32(render + 84) & 1;
    if (flags == 0)
        return 0;
    x = r_s16(render + 80);
    y = r_s16(render + 82);
    return resource001_sub_800dbd20((uint32)x, (uint32)y, 0);
}

uint32 resource001_sub_800d8094(uint32 first, uint32 second, uint16 *native_first, uint16 *native_second)
{
    uint32 context, value;
    FUNCTION_MARKER(0x800D8094, "ACE3.BIN");
    context = r_u32(0x800DDA58);
    value = r_u16(context + 32);
    if (native_first)
        *native_first = (uint16)value;
    else
        w_u16(first, value);
    context = r_u32(0x800DDA58);
    value = r_u16(context + 34);
    if (native_second)
        *native_second = (uint16)value;
    else
        w_u16(second, value);
    return value;
}

uint32 resource001_sub_800d9024(uint32 descriptor, uint32 packet, uint32 ordering, const uint8 *native_descriptor)
{
    uint32 value, tag, link;
    FUNCTION_MARKER(0x800D9024, "ACE3.BIN");
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x40);
    value = native_descriptor ? native_descriptor[11] : r_u8(descriptor + 11);
    w_u8(packet + 7, value ? 0x42 : 0x40);
    value = native_descriptor ? native_descriptor[8] : r_u8(descriptor + 8);
    w_u8(packet + 4, value);
    value = native_descriptor ? native_descriptor[9] : r_u8(descriptor + 9);
    w_u8(packet + 5, value);
    value = native_descriptor ? native_descriptor[10] : r_u8(descriptor + 10);
    w_u8(packet + 6, value);
    value = native_descriptor ? native_descriptor[0] | (native_descriptor[1] << 8) : r_u16(descriptor);
    w_u16(packet + 8, value);
    value = native_descriptor ? native_descriptor[2] | (native_descriptor[3] << 8) : r_u16(descriptor + 2);
    w_u16(packet + 10, value);
    value = native_descriptor ? native_descriptor[4] | (native_descriptor[5] << 8) : r_u16(descriptor + 4);
    w_u16(packet + 12, value);
    value = native_descriptor ? native_descriptor[6] | (native_descriptor[7] << 8) : r_u16(descriptor + 6);
    tag = r_u32(packet);
    w_u16(packet + 14, value);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 16;
}

uint32 resource001_sub_800d90e8(uint32 descriptor, uint32 packet, uint32 ordering, uint8 *native_descriptor)
{
    uint32 result, first_y, second_y;
    FUNCTION_MARKER(0x800D90E8, "ACE3.BIN");
    result = resource001_sub_800d9024(descriptor, packet, ordering, native_descriptor);
    first_y = native_descriptor ? native_descriptor[2] | (native_descriptor[3] << 8) : r_u16(descriptor + 2);
    second_y = native_descriptor ? native_descriptor[6] | (native_descriptor[7] << 8) : r_u16(descriptor + 6);
    if (native_descriptor)
    {
        first_y += 1;
        second_y += 1;
        native_descriptor[2] = (uint8)first_y;
        native_descriptor[3] = (uint8)(first_y >> 8);
        native_descriptor[6] = (uint8)second_y;
        native_descriptor[7] = (uint8)(second_y >> 8);
    }
    else
    {
        w_u16(descriptor + 2, first_y + 1);
        w_u16(descriptor + 6, second_y + 1);
    }
    return resource001_sub_800d9024(descriptor, result, ordering, native_descriptor);
}

uint32 resource001_sub_800dbd20(uint32 x, uint32 y, uint32 label)
{
    static const uint16 offsets[6][4] = {{0, 0, 8, 0}, {0, 9, 8, 9}, {0, 18, 8, 18}, {0, 0, 0, 18}, {4, 0, 4, 18}, {8, 0, 8, 18}};
    uint16 origin[2], coordinates[4];
    uint8 descriptor[12];
    uint32 value, text, render, state, flags, packet, ordering, i, j;
    FUNCTION_MARKER(0x800DBD20, "ACE3.BIN");
    resource001_sub_800d7d08(4);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7ce4(1, 30, 40);
    value = (uint32)resource001_sub_800da8bc(15);
    resource001_sub_800d7cd4(value);
    resource001_sub_800d7c60(128, 128, 128);
    resource001_sub_800d7c40((uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y);
    text = resource001_sub_800d5970(label);
    resource001_sub_800d8aa0(text, 0, 0, 0);
    render = r_u32(0x800DD9DC);
    flags = r_u32(render + 84) & 4;
    if (flags == 0)
        return 0;
    descriptor[8] = 40;
    descriptor[9] = 40;
    descriptor[10] = 40;
    descriptor[11] = 0;
    resource001_sub_800d8094(0, 0, &origin[0], &origin[1]);
    state = 0;
    value = 0;
    for (i = 0; i < 6; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            coordinates[j] = (uint16)(origin[j & 1] + offsets[i][j]);
            descriptor[j * 2] = (uint8)coordinates[j];
            descriptor[j * 2 + 1] = (uint8)(coordinates[j] >> 8);
        }
        if (i == 0)
            state = r_u32(0x800DD9D0);
        render = r_u32(0x800DD9DC);
        packet = r_u32(state + 8);
        ordering = r_u32(render + 16);
        value = resource001_sub_800d90e8(0, packet, ordering, descriptor);
        state = r_u32(0x800DD9D0);
        w_u32(state + 8, value);
    }
    return value;
}

uint32 resource001_sub_800d5c80(void)
{
    uint32 state, header;
    FUNCTION_MARKER(0x800D5C80, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    header = r_u32(state + 744);
    return r_u8(header + 8);
}

static void resource001_primary_menu_call(uint32 target)
{
    switch (target)
    {
        case 0x800CD9A8:
            resource001_sub_800cd9a8();
            return;
        case 0x800CDB64:
            resource001_sub_800cdb64();
            return;
        case 0x800CE24C:
            resource001_sub_800ce24c();
            return;
        case 0x800CE290:
            resource001_sub_800ce290();
            return;
        case 0x800CE518:
            resource001_sub_800ce518();
            return;
        case 0x800CE5AC:
            resource001_sub_800ce5ac();
            return;
        case 0x800CEB20:
            resource001_sub_800ceb20();
            return;
        case 0x800CEB90:
            resource001_sub_800ceb90();
            return;
        case 0x800CEC70:
            resource001_sub_800cec70();
            return;
        case 0x800CECF8:
            resource001_sub_800cecf8();
            return;
        case 0x800CEECC:
            resource001_sub_800ceecc();
            return;
        case 0x800CEF44:
            resource001_sub_800cef44();
            return;
        case 0x800CF004:
            resource001_sub_800cf004();
            return;
        case 0x800CF06C:
            resource001_sub_800cf06c();
            return;
        case 0x800CF750:
            resource001_sub_800cf750();
            return;
        case 0x800CF7B4:
            resource001_sub_800cf7b4();
            return;
        case 0x800CFA34:
            resource001_sub_800cfa34();
            return;
        case 0x800CFA94:
            resource001_sub_800cfa94();
            return;
        case 0x800CFB2C:
            resource001_sub_800cfb2c();
            return;
        case 0x800CFBD0:
            resource001_sub_800cfbd0();
            return;
        case 0x800CFC38:
            resource001_sub_800cfc38();
            return;
        case 0x800CFC94:
            resource001_sub_800cfc94();
            return;
        case 0x800CFD28:
            resource001_sub_800cfd28();
            return;
        case 0x800CFD60:
            resource001_sub_800cfd60();
            return;
        case 0x800CFDCC:
            resource001_sub_800cfdcc();
            return;
        case 0x800CFEA4:
            resource001_sub_800cfea4();
            return;
        case 0x800CFF90:
            resource001_sub_800cff90();
            return;
        case 0x800D0074:
            resource001_sub_800d0074();
            return;
        case 0x800D02DC:
            resource001_sub_800d02dc();
            return;
        case 0x800D0384:
            resource001_sub_800d0384();
            return;
        case 0x800D0460:
            resource001_sub_800d0460();
            return;
        case 0x800D0548:
            resource001_sub_800d0548();
            return;
        case 0x800D061C:
            resource001_sub_800d061c();
            return;
        case 0x800D0668:
            resource001_sub_800d0668();
            return;
        case 0x800D06D4:
            resource001_sub_800d06d4();
            return;
        case 0x800D076C:
            resource001_sub_800d076c();
            return;
        case 0x800D0848:
            resource001_sub_800d0848();
            return;
        case 0x800D08DC:
            resource001_sub_800d08dc();
            return;
        case 0x800D0978:
            resource001_sub_800d0978();
            return;
        case 0x800D0A10:
            resource001_sub_800d0a10();
            return;
        case 0x800D0AEC:
            resource001_sub_800d0aec();
            return;
        case 0x800D0B0C:
            resource001_sub_800d0b0c();
            return;
        case 0x800D0BFC:
            resource001_sub_800d0bfc();
            return;
        case 0x800D0CEC:
            resource001_sub_800d0cec();
            return;
        case 0x800D0DC8:
            resource001_sub_800d0dc8();
            return;
        case 0x800D0E54:
            resource001_sub_800d0e54();
            return;
        case 0x800D1080:
            resource001_sub_800d1080();
            return;
        case 0x800D1108:
            resource001_sub_800d1108();
            return;
        case 0x800D12A4:
            resource001_sub_800d12a4();
            return;
        case 0x800D1300:
            resource001_sub_800d1300();
            return;
        case 0x800D13B8:
            resource001_sub_800d13b8();
            return;
        case 0x800D1488:
            resource001_sub_800d1488();
            return;
        case 0x800D1554:
            resource001_sub_800d1554();
            return;
        case 0x800D15C4:
            resource001_sub_800d15c4();
            return;
        case 0x800D1618:
            resource001_sub_800d1618();
            return;
        case 0x800D16D4:
            resource001_sub_800d16d4();
            return;
        case 0x800D1738:
            resource001_sub_800d1738();
            return;
        default:
            abort();
    }
}

uint32 resource001_sub_800d181c(void)
{
    uint32 state, game, phase, branch, x, y, angle, packet, result, group, target, value;
    sint32 tick, category, step, adjusted;
    FUNCTION_MARKER(0x800D181C, "ACE3.BIN");
    if (cd_resource_state() == 0)
    {
        game = r_u32(0x800BCE60);
        w_u32(game + 300, 0);
        w_u32(game + 296, 0);
    }
    state = r_u32(0x800DD9D0);
    phase = r_u16(state + 76);
    adjusted = (sint16)(phase - 1);
    branch = 0x800D1944;
    if ((uint32)adjusted < 11)
        branch = r_u32(0x800CC00C + (uint32)adjusted * 4);
    if (branch != 0x800D1944)
    {
        switch (branch)
        {
            case 0x800D1890:
                x = 249;
                y = 361;
                break;
            case 0x800D18C0:
                x = 70;
                y = 410;
                break;
            case 0x800D18F0:
                x = 58;
                y = 370;
                break;
            default:
                abort();
        }
        game = r_u32(0x800BCE60);
        tick = r_s32(game + 32);
        angle = ((uint32)tick - (uint32)(tick / 64) * 64) << 6;
        state = r_u32(0x800DD9D0);
        packet = r_u32(state + 8);
        result = resource001_sub_800d9a08(x, y, angle, packet);
        state = r_u32(0x800DD9D0);
        w_u32(state + 8, result);
    }
    state = r_u32(0x800DD9D0);
    category = r_s16(state + 76);
    step = r_s16(state + 78);
    group = r_u32(0x800DD598 + (uint32)category * 4);
    target = r_u32(group + (uint32)step * 4);
    resource001_primary_menu_call(target);
    resource001_sub_800dbfe4();
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 210);
    resource001_sub_800dc098(1, value);
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 208);
    resource001_sub_800da9a8(17, value);
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 209);
    value = value == 1 ? 2 : r_u8(state + 90);
    resource001_sub_800da9a8(16, value);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 916);
    resource001_sub_800da9a8(3, value);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 292);
    resource001_sub_800da9a8(5, (value & 0x10) != 0);
    result = resource_wait(1);
    if (result == 0)
    {
        state = r_u32(0x800DD9D0);
        return resource_queue_audio(state + 124, 1, 0);
    }
    return result;
}

static void resource001_secondary_menu_call(uint32 target)
{
    switch (target)
    {
        case 0x800D1798:
            resource001_sub_800d1798();
            return;
        case 0x800D181C:
            resource001_sub_800d181c();
            return;
        case 0x800D1A5C:
            resource001_sub_800d1a5c();
            return;
        case 0x800D1ADC:
            resource001_sub_800d1adc();
            return;
        case 0x800D1B2C:
            resource001_sub_800d1b2c();
            return;
        case 0x800D1C18:
            resource001_sub_800d1c18();
            return;
        case 0x800D1CF0:
            resource001_sub_800d1cf0();
            return;
        case 0x800D1DCC:
            resource001_sub_800d1dcc();
            return;
        case 0x800D202C:
            resource001_sub_800d202c();
            return;
        case 0x800D20BC:
            resource001_sub_800d20bc();
            return;
        case 0x800D2154:
            resource001_sub_800d2154();
            return;
        case 0x800D21D0:
            resource001_sub_800d21d0();
            return;
        case 0x800D2268:
            resource001_sub_800d2268();
            return;
        case 0x800D22E4:
            resource001_sub_800d22e4();
            return;
        case 0x800D237C:
            resource001_sub_800d237c();
            return;
        case 0x800D23CC:
            resource001_sub_800d23cc();
            return;
        case 0x800D248C:
            resource001_sub_800d248c();
            return;
        case 0x800D255C:
            resource001_sub_800d255c();
            return;
        case 0x800D2600:
            resource001_sub_800d2600();
            return;
        case 0x800D26F0:
            resource001_sub_800d26f0();
            return;
        case 0x800D2954:
            resource001_sub_800d2954();
            return;
        case 0x800D2994:
            resource001_sub_800d2994();
            return;
        case 0x800D29FC:
            resource001_sub_800d29fc();
            return;
        case 0x800D2AE8:
            resource001_sub_800d2ae8();
            return;
        case 0x800D2E74:
            resource001_sub_800d2e74();
            return;
        case 0x800D2FB8:
            resource001_sub_800d2fb8();
            return;
        case 0x800D34FC:
            resource001_sub_800d34fc();
            return;
        case 0x800D3584:
            resource001_sub_800d3584();
            return;
        case 0x800D37FC:
            resource001_sub_800d37fc();
            return;
        case 0x800D3840:
            resource001_sub_800d3840();
            return;
        case 0x800D3A24:
            resource001_sub_800d3a24();
            return;
        case 0x800D3A8C:
            resource001_sub_800d3a8c();
            return;
        default:
            abort();
    }
}

uint32 resource001_sub_800d3b3c(void)
{
    uint32 state, game, mode, angle, packet, result, group, target, value;
    sint32 tick, category, step;
    FUNCTION_MARKER(0x800D3B3C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    mode = r_u8(state + 211);
    if (mode == 0 || mode == 1)
    {
        game = r_u32(0x800BCE60);
        tick = r_s32(game + 32);
        angle = ((uint32)tick - (uint32)(tick / 64) * 64) << 6;
        packet = r_u32(state + 8);
        result = resource001_sub_800d9a08(mode == 1 ? 51 : 70, mode == 1 ? 397 : 410, angle, packet);
        state = r_u32(0x800DD9D0);
        w_u32(state + 8, result);
    }
    state = r_u32(0x800DD9D0);
    category = r_s16(state + 76);
    step = r_s16(state + 78);
    group = r_u32(0x800DD66C + (uint32)category * 4);
    target = r_u32(group + (uint32)step * 4);
    resource001_secondary_menu_call(target);
    resource001_sub_800dbfe4();
    resource001_sub_800dc098(0, 0);
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 208);
    resource001_sub_800da9a8(17, value);
    state = r_u32(0x800DD9D0);
    value = r_u8(state + 209);
    value = value == 1 ? 2 : r_u8(state + 90);
    resource001_sub_800da9a8(16, value);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 916);
    resource001_sub_800da9a8(3, value);
    game = r_u32(0x800BCE60);
    value = r_u32(game + 292);
    resource001_sub_800da9a8(5, (value & 0x10) != 0);
    result = resource_wait(1);
    if (result == 0)
    {
        state = r_u32(0x800DD9D0);
        return resource_queue_audio(state + 124, 1, 0);
    }
    return result;
}

static void resource001_top_menu_call(uint32 target)
{
    switch (target)
    {
        case 0x800CD9A8:
        case 0x800CDB64:
        case 0x800CE24C:
        case 0x800CE290:
        case 0x800CE518:
        case 0x800CE5AC:
        case 0x800CEB20:
        case 0x800CEB90:
        case 0x800CEC70:
        case 0x800CECF8:
        case 0x800CEECC:
        case 0x800CEF44:
        case 0x800CF004:
        case 0x800CF06C:
        case 0x800CF750:
        case 0x800CF7B4:
        case 0x800CFA34:
        case 0x800CFA94:
        case 0x800CFB2C:
        case 0x800CFBD0:
        case 0x800CFC38:
        case 0x800CFC94:
        case 0x800CFD28:
        case 0x800CFD60:
        case 0x800CFDCC:
        case 0x800CFEA4:
        case 0x800CFF90:
        case 0x800D0074:
        case 0x800D02DC:
        case 0x800D0384:
        case 0x800D0460:
        case 0x800D0548:
        case 0x800D061C:
        case 0x800D0668:
        case 0x800D06D4:
        case 0x800D076C:
        case 0x800D0848:
        case 0x800D08DC:
        case 0x800D0978:
        case 0x800D0A10:
        case 0x800D0AEC:
        case 0x800D0B0C:
        case 0x800D0BFC:
        case 0x800D0CEC:
        case 0x800D0DC8:
        case 0x800D0E54:
        case 0x800D1080:
        case 0x800D1108:
        case 0x800D12A4:
        case 0x800D1300:
        case 0x800D13B8:
        case 0x800D1488:
        case 0x800D1554:
        case 0x800D15C4:
        case 0x800D1618:
        case 0x800D16D4:
        case 0x800D1738:
            resource001_primary_menu_call(target);
            return;
        case 0x800D1798:
        case 0x800D181C:
        case 0x800D1A5C:
        case 0x800D1ADC:
        case 0x800D1B2C:
        case 0x800D1C18:
        case 0x800D1CF0:
        case 0x800D1DCC:
        case 0x800D202C:
        case 0x800D20BC:
        case 0x800D2154:
        case 0x800D21D0:
        case 0x800D2268:
        case 0x800D22E4:
        case 0x800D237C:
        case 0x800D23CC:
        case 0x800D248C:
        case 0x800D255C:
        case 0x800D2600:
        case 0x800D26F0:
        case 0x800D2954:
        case 0x800D2994:
        case 0x800D29FC:
        case 0x800D2AE8:
        case 0x800D2E74:
        case 0x800D2FB8:
        case 0x800D34FC:
        case 0x800D3584:
        case 0x800D37FC:
        case 0x800D3840:
        case 0x800D3A24:
        case 0x800D3A8C:
            resource001_secondary_menu_call(target);
            return;
        case 0x800CCC90:
            resource001_sub_800ccc90();
            return;
        case 0x800CCD48:
            resource001_sub_800ccd48();
            return;
        case 0x800D3AE8:
            resource001_sub_800d3ae8();
            return;
        case 0x800D3B3C:
            resource001_sub_800d3b3c();
            return;
        case 0x800D75E4:
            resource001_sub_800d75e4();
            return;
        case 0x800D7600:
            resource001_sub_800d7600();
            return;
        default:
            abort();
    }
}

uint32 resource001_sub_800cc764(void)
{
    uint32 state, game, timer, frame_packet, index, packet, value, group, target, display;
    sint32 category, step, age;
    FUNCTION_MARKER(0x800CC764, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    game = r_u32(0x800BCE60);
    timer = r_u32(state + 68);
    frame_packet = r_u32(game + 8);
    w_u32(state + 68, timer + 1);
    index = r_u32(game + 4);
    packet = r_u32(state + index * 4);
    w_u32(state + 8, packet);
    resource001_sub_800dc9ec(frame_packet + 4);
    value = resource001_sub_800da6fc();
    resource001_sub_800d81f0(value + 4);
    resource001_sub_800d7c40(50, 300);
    resource001_sub_800d7c60(64, 64, 64);
    state = r_u32(0x800DD9D0);
    value = r_u32(state + 24);
    if (value != 0)
        resource001_sub_800d5f6c();
    resource001_sub_800dcaa4();
    state = r_u32(0x800DD9D0);
    category = r_s16(state + 12);
    group = r_u32(0x800DCB10 + (uint32)category * 4);
    if (group != 0)
    {
        step = r_s16(state + 14);
        target = r_u32(group + (uint32)step * 4);
        resource001_top_menu_call(target);
    }
    state = r_u32(0x800DD9D0);
    age = r_s32(state + 120);
    if (age >= 601)
    {
        value = r_u32(state + 24);
        if (value != 0)
            resource001_sub_800d58b0();
        game = r_u32(0x800BCE60);
        w_u32(game + 928, 4);
        w_u8(game + 936, 0);
        return 0x60000005;
    }
    value = r_u8(state + 88);
    if (value == 0)
        return 0;
    resource_init_queue(0, 0);
    state = r_u32(0x800DD9D0);
    value = r_u32(state + 24);
    if (value != 0)
        resource001_sub_800d58b0();
    game = r_u32(0x800BCE60);
    value = r_u32(game + 0x96C);
    w_u16(game + 0x954, value & 0x10 ? 0 : 2);
    display = r_u32(0x800BCE48);
    value = r_u32(display);
    w_u32(display, value | 4);
    return 0x60000003;
}

uint32 resource002_sub_800fb2a0(sint32 mission)
{
    uint32 state;
    FUNCTION_MARKER(0x800FB2A0, "ACE3.BIN");
    if (mission == 0)
    {
        state = r_u32(0x80112DC4);
        w_u32(state + 128, 1);
        return 1;
    }
    if (mission < 20)
        return 0;
    if (mission < 35)
        return 1;
    if (mission < 40)
        return 3;
    if (mission < 45)
        return 2;
    if (mission < 49)
        return 3;
    if (mission < 54)
        return 2;
    if (mission == 54)
        return 1;
    if (mission < 57)
        return 2;
    if (mission < 59)
        return 3;
    return 0;
}

uint32 resource002_sub_800de908(uint32 resource, uint32 unused, uint32 slot)
{
    uint32 state;
    FUNCTION_MARKER(0x800DE908, "ACE3.BIN");
    state = r_u32(0x80112DC4) + (slot & 0xFF) * 4;
    w_u32(state + 352, resource);
    return state;
}

uint32 resource002_sub_800dedf0(uint32 slot, uint32 value)
{
    uint32 state;
    FUNCTION_MARKER(0x800DEDF0, "ACE3.BIN");
    state = r_u32(0x80112DC4) + slot * 4;
    w_u32(state + 376, value);
    return state;
}

uint32 resource002_sub_800f74c8(void)
{
    uint32 state;
    FUNCTION_MARKER(0x800F74C8, "ACE3.BIN");
    state = arena_alloc(4124);
    w_u32(state + 12, 0x3FFF);
    w_u32(state + 4, 0);
    w_u32(state + 8, 0);
    w_u32(state, 0);
    w_u32(state + 16, 255);
    w_u32(state + 20, 0);
    w_u32(0x80112E0C, state);
    return state;
}

uint32 resource002_sub_800f7514(uint32 resource)
{
    uint32 state;
    FUNCTION_MARKER(0x800F7514, "ACE3.BIN");
    state = r_u32(0x80112E0C);
    w_u32(state + 4120, resource);
    return state;
}

uint32 resource002_sub_800f5ac4(void)
{
    uint32 i, state;
    FUNCTION_MARKER(0x800F5AC4, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    w_u32(0x80112E08, state + 952);
    for (i = 0; i < 64; ++i)
    {
        state = r_u32(0x80112E04) + i * 4;
        w_u8(state + 19, 0);
    }
    return 0;
}

uint32 resource002_sub_800f5ff8(void)
{
    uint32 i, state;
    FUNCTION_MARKER(0x800F5FF8, "ACE3.BIN");
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 313, 0);
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 312, 0);
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 272, 0);
    }
    for (i = 0; i < 8; ++i)
    {
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 665, 0);
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 664, 0);
        state = r_u32(0x80112E04) + i * 44;
        w_u8(state + 624, 0);
    }
    state = r_u32(0x80112E04);
    w_u8(state + 976, 0);
    state = r_u32(0x80112E04);
    w_u8(state + 977, 0);
    return state;
}

uint32 resource002_sub_800f33f8(uint32 mode)
{
    uint32 state, y, x, texture, result;
    FUNCTION_MARKER(0x800F33F8, "ACE3.BIN");
    if ((sint16)mode == -1)
    {
        state = r_u32(0x80112E00) + 0x8000;
        w_u8(state + 0x2908, 0);
        state = r_u32(0x80112E00) + 0x8000;
        w_u8(state + 0x2909, 0);
    }
    else
    {
        state = r_u32(0x80112E00) + 0x8000;
        w_u8(state + 0x2908, 1);
        state = r_u32(0x80112E00) + 0x8000;
        w_u8(state + 0x2909, mode);
    }
    state = r_u32(0x80112E00) + 0x8000;
    texture = r_u8(state + 0x2909);
    y = r_u16(state + 0x28F6);
    x = r_u16(state + 0x28F4);
    result = (y & 0x100) >> 4;
    w_u16(state + 0x28EC, ((texture & 3) << 5) | result | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2));
    return result;
}

uint32 resource002_sub_800f34c8(uint32 enabled, uint32 first, uint32 second)
{
    uint32 state;
    FUNCTION_MARKER(0x800F34C8, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    w_u8(state + 0x290B, enabled);
    state = r_u32(0x80112E00) + 0x8000;
    w_u16(state + 0x290C, first);
    w_u16(state + 0x290E, second);
    return state;
}

uint32 resource002_sub_800f34b0(uint32 enabled)
{
    uint32 state;
    FUNCTION_MARKER(0x800F34B0, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    w_u16(state + 0x2910, enabled);
    return state;
}

uint32 resource002_sub_800deb5c(uint32 group, uint32 item, uint32 enabled, uint32 first, uint32 second, uint32 third)
{
    uint32 owner, offset, base;
    FUNCTION_MARKER(0x800DEB5C, "ACE3.BIN");
    owner = r_u32(0x80112DC4) + group * 4;
    offset = item * 12;
    base = r_u32(owner + 364);
    w_u16(base + offset + 10, enabled);
    base = r_u32(owner + 364);
    w_u16(base + offset + 4, first);
    base = r_u32(owner + 364);
    w_u16(base + offset + 6, second);
    base = r_u32(owner + 364);
    w_u16(base + offset + 8, third);
    return base;
}

uint32 resource002_sub_800deca8(uint32 group, uint32 item)
{
    FUNCTION_MARKER(0x800DECA8, "ACE3.BIN");
    return resource002_sub_800deb5c(group, item, 1, 0, 0, 0);
}

uint32 resource002_sub_800e37f8(void)
{
    uint32 state, value, i;
    FUNCTION_MARKER(0x800E37F8, "ACE3.BIN");
    value = arena_alloc(864);
    w_u32(0x80112DF0, value);
    value = arena_alloc(128);
    state = r_u32(0x80112DF0);
    w_u32(state + 744, value);
    for (i = 0; i < 4; ++i)
    {
        value = arena_alloc(1152);
        state = r_u32(0x80112DF0);
        if (i < 3)
            w_u32(state + i * 4 + 748, value);
        else
            w_u32(state + 760, value);
    }
    for (i = 0; i < 7; ++i)
    {
        value = arena_alloc(384);
        state = r_u32(0x80112DF0);
        if (i < 6)
            w_u32(state + i * 4 + 764, value);
        else
            w_u32(state + 788, value);
    }
    return 0;
}

uint32 resource002_sub_800de79c(void)
{
    uint32 state, i, group, packet, offset, tail;
    FUNCTION_MARKER(0x800DE79C, "ACE3.BIN");
    state = r_u32(0x80112DC0);
    for (i = 0; i < 32; ++i)
    {
        w_u32(state + 240, 0);
        w_u32(state + 112, 0);
        state += 4;
    }
    state = r_u32(0x80112DC0);
    packet = r_u32(state + 92);
    w_u16(state + 368, 0);
    w_u16(state + 370, 0);
    w_u8(packet + 2, 0);
    offset = 376;
    tail = 22216;
    for (group = 0; group < 2; ++group)
    {
        state = r_u32(0x80112DC0);
        packet = state + offset + 14;
        for (i = 0; i < 273; ++i)
        {
            w_u8(packet - 11, 9);
            w_u8(packet - 7, 46);
            w_u8(packet - 10, 128);
            w_u8(packet - 9, 128);
            w_u8(packet - 8, 128);
            w_u8(packet - 2, 240);
            w_u8(packet - 1, 126);
            w_u8(packet + 6, 248);
            w_u8(packet + 7, 126);
            w_u8(packet + 14, 240);
            w_u8(packet + 15, 138);
            w_u8(packet + 22, 248);
            w_u8(packet + 23, 138);
            w_u16(packet + 8, 71);
            w_u16(packet, 728);
            packet += 40;
        }
        offset += 10920;
        state = r_u32(0x80112DC0);
        packet = state + tail;
        tail += 40;
        w_u8(packet + 7, 44);
        w_u8(packet + 13, 27);
        w_u8(packet + 21, 27);
        w_u8(packet + 29, 47);
        w_u8(packet + 37, 47);
        w_u16(packet + 22, 7);
        w_u16(packet + 14, 856);
        w_u8(packet + 3, 9);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u8(packet + 12, 212);
        w_u8(packet + 20, 224);
        w_u8(packet + 28, 212);
        w_u8(packet + 36, 224);
    }
    return 0;
}

uint32 resource002_sub_800de924(void)
{
    uint32 state, owner, packet, value, i, j;
    FUNCTION_MARKER(0x800DE924, "ACE3.BIN");
    value = arena_alloc(22296);
    w_u32(0x80112DC0, value);
    for (i = 0; i < 3; ++i)
    {
        value = arena_alloc(216);
        state = r_u32(0x80112DC0) + i * 4;
        w_u32(state + 32, value);
        value = arena_alloc(72);
        state = r_u32(0x80112DC0) + i * 4;
        w_u32(state + 44, value);
        for (j = 0; j < 18; ++j)
        {
            packet = r_u32(state + 32);
            w_u16(packet + j * 12, 0);
            packet = r_u32(state + 32);
            w_u16(packet + j * 12 + 2, 0);
        }
    }
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(64);
        state = r_u32(0x80112DC0) + i * 4;
        w_u32(state + 20, value);
    }
    owner = r_u32(0x80112DC4);
    state = r_u32(0x80112DC0);
    w_u32(state + 96, 0);
    w_u16(owner + 388, 48);
    w_u16(owner + 392, 86);
    w_u16(owner + 396, 124);
    w_u16(owner + 390, 131);
    w_u16(owner + 394, 88);
    w_u16(owner + 398, 131);
    w_u16(owner + 400, 162);
    w_u16(owner + 402, 88);
    for (i = 0; i < 4; ++i)
    {
        w_u16(owner + 404 + i * 2, 3584);
        w_u16(owner + 412 + i * 2, 0);
    }
    state = r_u32(0x80112DC0);
    packet = state + 64;
    w_u32(state + 60, packet);
    w_u16(packet + 14, 1);
    packet = r_u32(state + 60);
    w_u16(packet + 10, 0);
    value = arena_alloc(32);
    state = r_u32(0x80112DC0);
    w_u32(state + 80, value);
    value = arena_alloc(32);
    state = r_u32(0x80112DC0);
    w_u32(state + 88, value);
    value = arena_alloc(32);
    state = r_u32(0x80112DC0);
    w_u32(state + 84, value);
    for (i = 0; i < 16; ++i)
    {
        packet = r_u32(state + 80);
        w_u16(packet + i * 2, 0);
        packet = r_u32(state + 88);
        w_u16(packet + i * 2, 0);
        packet = r_u32(state + 84);
        w_u16(packet + i * 2, 0);
    }
    value = arena_alloc(64);
    state = r_u32(0x80112DC0);
    w_u32(state + 92, value);
    for (i = 0; i < 64; ++i)
    {
        state = r_u32(0x80112DC0);
        packet = r_u32(state + 92);
        w_u8(packet + i, 0);
    }
    resource002_sub_800de79c();
    state = r_u32(0x80112DC0);
    w_u32(state + 28, 0);
    return state;
}

uint32 resource002_sub_800f60f0(void)
{
    uint32 display, flags, state, game, value, i, j, owner, packet, target;
    uint32 first_words[2], second_words[2];
    sint32 mode;
    PSX_RECT first_rect, second_rect;
    FUNCTION_MARKER(0x800F60F0, "ACE3.BIN");
    display = r_u32(0x800BCE48);
    w_u8(display + 68, 1);
    display = r_u32(0x800BCE48);
    flags = r_u32(display);
    w_u32(display + 4, 1);
    w_u32(display + 20, 0);
    w_u32(display, flags | 9);
    resource_init_queue(0, 0);
    display = r_u32(0x800BCE48);
    w_u8(display + 16, 0);
    display = r_u32(0x800BCE48);
    w_u8(display + 17, 0);
    display = r_u32(0x800BCE48);
    w_u8(display + 18, 0);
    sub_8002cbcc();
    value = arena_alloc(644);
    w_u32(0x80112DC4, value);
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(0x8000);
        state = r_u32(0x80112DC4);
        w_u32(state + i * 4 + 632, value);
    }
    game = r_u32(0x800BCE60);
    value = r_u8(game + 956);
    if (value == 54)
    {
        w_u8(game + 956, 34);
        game = r_u32(0x800BCE60);
        value = r_u8(game + 956);
    }
    if (value == 58)
        w_u8(game + 956, 39);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 957);
    if (value == 56)
    {
        w_u8(game + 957, 42);
        game = r_u32(0x800BCE60);
        value = r_u8(game + 957);
    }
    if (value == 57)
        w_u8(game + 957, 37);
    game = r_u32(0x800BCE60);
    if (r_u8(game + 2391) == 1)
    {
        value = r_u8(0x8010E638 + (uint32)r_s16(game + 908));
        w_u16(game + 2388, 0);
        w_u16(game + 908, value);
    }
    game = r_u32(0x800BCE60);
    value = resource002_sub_800fb2a0(r_s16(game + 908));
    state = r_u32(0x80112DC4);
    w_u32(state + 128, value);
    for (i = 0; i < 3; ++i)
    {
        value = arena_alloc(384);
        state = r_u32(0x80112DC4) + i * 4;
        w_u32(state + 364, value);
        for (j = 0; j < 32; ++j)
        {
            owner = r_u32(0x80112DC4) + i * 4;
            packet = r_u32(owner + 364);
            w_u16(packet + j * 12, 0);
            packet = r_u32(owner + 364);
            w_u16(packet + j * 12 + 2, 0);
            resource002_sub_800deca8(i, j);
        }
    }
    value = arena_alloc(16);
    state = r_u32(0x80112DC4);
    w_u32(state + 428, value);
    w_u16(value + 10, 0xFFFF);
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(64);
        state = r_u32(0x80112DC4) + i * 4;
        w_u32(state, value);
        value = arena_alloc(1024);
        state = r_u32(0x80112DC4) + i * 4;
        w_u32(state + 12, value);
    }
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(4764);
        state = r_u32(0x80112DC4) + i * 4;
        w_u32(state + 56, value);
    }
    resource002_sub_800de924();
    value = sub_80027338(0x8010BBF0, NULL);
    resource002_sub_800de908(value, 1, 2);
    resource002_sub_800dedf0(0, 0);
    resource002_sub_800dedf0(1, 0);
    resource002_sub_800dedf0(2, 1);
    resource002_sub_800f3c14();
    value = arena_alloc(700);
    w_u32(0x800BCE54, value);
    resource002_sub_800e37f8();
    resource002_sub_800f74c8();
    resource002_sub_800f7514(0x8010E854);
    state = r_u32(0x80112DC4);
    w_u16(state + 454, 0);
    sound_enable_reverb();
    game = r_u32(0x800BCE60);
    if (r_u32(game + 2412) & 0x10)
    {
        state = r_u32(0x80112DC4);
        w_u32(state + 196, 0);
        if (r_s16(game + 2388) == 1)
            w_u16(game + 2388, 3);
        game = r_u32(0x800BCE60);
        if (r_u32(game + 2412) & 0x10)
            w_u16(game + 908, 59);
    }
    game = r_u32(0x800BCE60);
    if (r_s16(game + 2388) == 1)
    {
        value = r_u8(game + 956);
        target = 0x800F6514;
        if (value - 18 < 36)
            target = r_u32(0x800CD194 + (value - 18) * 4);
        switch (target)
        {
            case 0x800F64A0:
                game = r_u32(0x800BCE60);
                flags = r_u32(game + 2412);
                state = r_u32(0x80112DC4);
                w_u32(game + 2412, flags | 8);
                w_u32(state + 224, 2);
                value = r_u8(game + 956);
                if (resource002_sub_800f5b04(value))
                {
                    game = r_u32(0x800BCE60);
                    flags = r_u32(game + 2412);
                    if (flags & 0x20)
                    {
                        state = r_u32(0x80112DC4);
                        w_u32(game + 2412, flags & ~0x20u);
                        w_u32(state + 224, 3);
                    }
                    else
                        w_u32(game + 2412, flags | 0x20);
                }
                break;
            case 0x800F6514:
                game = r_u32(0x800BCE60);
                flags = r_u32(game + 2412);
                w_u32(game + 2412, flags & ~8u);
                break;
            default:
#ifndef NDEBUG
                fprintf(stderr, "Unbound resource2 initializer branch %08X\n", target);
#endif
                abort();
        }
        game = r_u32(0x800BCE60);
        if (r_u32(game + 2412) & 8)
        {
            resource002_sub_800f5ca4();
            return 0x04000000;
        }
    }
    game = r_u32(0x800BCE60);
    if (r_u8(game + 2399) == 1)
        return 0x04000000;
    if (r_u32(game + 2412) & 1)
        return 0x04000000;
    if (r_u8(game + 2391) == 1)
        return 0x04000000;
    mode = r_s16(game + 2388);
    if (mode == 1)
    {
        flags = r_u32(game + 2412);
        value = r_u8(game + 957);
        w_u16(game + 908, value);
        if (flags & 0x10)
            w_u16(game + 908, 59);
    }
    else if (mode == 0 || mode == 2)
    {
        game = r_u32(0x800BCE60);
        if (r_u32(game + 2412) & 0x10)
            w_u16(game + 908, 59);
        return 0x04000000;
    }
    else if (mode == 3)
        w_u16(game + 908, 59);
    resource002_sub_800f5ca4();
    value = arena_alloc(984);
    w_u32(0x80112E04, value);
    w_u8(value, 0);
    state = r_u32(0x80112E04);
    w_u8(state + 1, 0);
    owner = r_u32(0x80112E04);
    w_u16(state + 2, 0);
    w_u8(owner, 0);
    owner = r_u32(0x80112E04);
    state = r_u32(0x80112DC4);
    w_u16(owner + 980, 0);
    w_u32(state + 616, 0);
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(4096);
        state = r_u32(0x80112E04);
        w_u32(state + i * 4 + 4, value);
    }
    resource002_sub_800f5ac4();
    resource002_sub_800f5ff8();
    resource002_sub_800f35b8(7);
    resource002_sub_800f33f8(1);
    resource002_sub_800f34c8(1, 1, 1);
    resource002_sub_800f34b0(1);
    first_words[0] = r_u32(0x800CD184);
    first_words[1] = r_u32(0x800CD188);
    second_words[0] = r_u32(0x800CD18C);
    second_words[1] = r_u32(0x800CD190);
    first_rect.x = (sint16)first_words[0];
    first_rect.y = (sint16)(first_words[0] >> 16);
    first_rect.w = (sint16)first_words[1];
    first_rect.h = (sint16)(first_words[1] >> 16);
    second_rect.x = (sint16)second_words[0];
    second_rect.y = (sint16)(second_words[0] >> 16);
    second_rect.w = (sint16)second_words[1];
    second_rect.h = (sint16)(second_words[1] >> 16);
    MoveImage(&first_rect, 960, 256);
    DrawSync(0);
    MoveImage(&second_rect, 960, 384);
    DrawSync(0);
    return 0x04000000;
}

uint32 resource002_sub_800f33a8(uint32 x, uint32 y)
{
    uint32 state, value;
    FUNCTION_MARKER(0x800F33A8, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    value = (x & 0xFFFF) | (y << 16);
    w_u32(state + 0x28E4, value);
    w_u32(state + 0x28E8, value);
    return 0x8000;
}

uint32 resource002_sub_800f33d0(uint32 red, uint32 green, uint32 blue)
{
    uint32 state;
    FUNCTION_MARKER(0x800F33D0, "ACE3.BIN");
    state = r_u32(0x80112E00) + 0x8000;
    w_u32(state + 0x28CC, red | (green << 8) | (blue << 16));
    return 0x8000;
}

uint32 resource002_sub_800f3c14(void)
{
    uint32 state, owner, packet, value, group, i;
    FUNCTION_MARKER(0x800F3C14, "ACE3.BIN");
    value = arena_alloc(43284);
    w_u32(0x80112E00, value);
    for (group = 0; group < 2; ++group)
    {
        value = arena_alloc(4);
        owner = r_u32(0x80112E00);
        state = owner + 0x8000;
        w_u32(state + group * 4 + 0x28D4, value);
        w_u8(state + 0x2908, 0);
        value = r_u32(0x80112E00);
        w_u32(state + 0xCA4, owner + 24004 + group * 6000);
        w_u8(value + 0x8000 + 0x2909, 0);
        resource002_sub_800f35b8(4);
        for (i = 0; i < 300; ++i)
        {
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u8(packet + 3, 4);
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u8(packet + 7, 100);
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u8(packet + 4, 128);
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u8(packet + 5, 128);
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u8(packet + 6, 128);
            state = r_u32(0x80112E00) + 0x8000;
            packet = r_u32(state + 0xCA4);
            w_u32(state + 0xCA4, packet + 20);
        }
    }
    state = r_u32(0x80112E00) + 0x8000;
    w_u32(state + 0x28D0, 0);
    w_u8(state + 0x290B, 0);
    resource002_sub_800f33a8(10, 200);
    return resource002_sub_800f33d0(128, 128, 128);
}

uint32 resource002_sub_800f5b04(uint32 mission)
{
    uint32 game, progress;
    FUNCTION_MARKER(0x800F5B04, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    progress = game + 2396;
    if (r_s16(game + 2388) != 1)
        return 0;
    if (!r_u8(progress + 0x2E2) && !r_u8(progress + 0x31E) && !r_u8(progress + 0x35A) && !r_u8(progress + 0x396) && mission != 34)
        return 0;
    if (!r_u8(progress + 0x2E7) && !r_u8(progress + 0x323) && !r_u8(progress + 0x35F) && !r_u8(progress + 0x39B) && mission != 39)
        return 0;
    if (!r_u8(progress + 0x2D2) && !r_u8(progress + 0x30E) && !r_u8(progress + 0x34A) && !r_u8(progress + 0x386) && mission != 18)
        return 0;
    if (!r_u8(progress + 0x2F5) && !r_u8(progress + 0x331) && !r_u8(progress + 0x36D) && !r_u8(progress + 0x3A9) && mission != 53)
        return 0;
    if (r_u8(progress + 0x2F0))
        return 1;
    if (r_u8(progress + 0x32C))
        return 1;
    if (r_u8(progress + 0x368))
        return 1;
    if (r_u8(progress + 0x3A4))
        return 1;
    return mission == 48;
}

uint32 resource002_sub_800fe950(uint32 mission)
{
    uint32 table, count, i, game, flag;
    FUNCTION_MARKER(0x800FE950, "ACE3.BIN");
    if (mission >= 60)
        mission = 0;
    table = 0x80111784 + mission * 12;
    count = r_u8(table + 4);
    if (count == 0)
        return table;
    for (i = 0; i < count; ++i)
    {
        game = r_u32(0x800BCE60);
        flag = r_u8(table + i + 5);
        w_u8(game + flag + 2420, 1);
    }
    return 0;
}

uint32 resource002_sub_800fec84(uint32 mission)
{
    uint32 game, table, count, i, item, entry, inner_count, j, flag, offset, target;
    FUNCTION_MARKER(0x800FEC84, "ACE3.BIN");
    if (mission >= 60)
        mission = 0;
    target = 0x800FECE4;
    if (mission - 13 < 32)
        target = r_u32(0x800CD6EC + (mission - 13) * 4);
    switch (target)
    {
        case 0x800FECC0:
            game = r_u32(0x800BCE60);
            w_u8(game + 2498, 1);
            break;
        case 0x800FECD4:
            game = r_u32(0x800BCE60);
            w_u8(game + 2497, 1);
            break;
        case 0x800FECE4:
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 unlock branch %08X\n", target);
#endif
            abort();
    }
    offset = mission * 12;
    table = 0x80111784 + offset;
    count = r_u8(table + 4);
    if (count == 0)
        return table;
    for (i = 0; i < count; ++i)
    {
        item = r_u8(table + i + 5);
        if (item - 1 < 36)
        {
            entry = 0x80111A54 + item * 12;
            inner_count = r_u8(entry);
            for (j = 0; j < inner_count; ++j)
            {
                game = r_u32(0x800BCE60);
                flag = r_u8(entry + j + 4);
                w_u8(game + flag + 2484, 1);
            }
            inner_count = r_u8(entry + 1);
            for (j = 0; j < inner_count; ++j)
            {
                game = r_u32(0x800BCE60);
                flag = r_u8(entry + j + 8);
                w_u8(game + flag + 2490, 1);
            }
        }
    }
    return offset + count;
}

uint32 resource002_sub_800f5ca4(void)
{
    uint32 game, progress, mission, difficulty, rating, profile;
    uint32 owner, value, half, flags, packet, total, count, target;
    sint32 signed_half;
    FUNCTION_MARKER(0x800F5CA4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    progress = game + 2396;
    mission = r_u8(game + 956);
    difficulty = r_u32(game + 916);
    rating = r_u8(game + 963);
    signed_half = r_s16(game + 954);
    half = r_u16(game + 954);
    profile = game + 952;
    if (signed_half != 0)
    {
        owner = progress + mission * 2;
        w_u16(owner + 584, half);
        if (difficulty == 0 && signed_half < (sint32)r_u16(owner + 104))
            w_u16(owner + 104, half);
        if (difficulty == 1)
        {
            owner = progress + (mission & 0xFF) * 2;
            if ((sint16)half < (sint32)r_u16(owner + 224))
                w_u16(owner + 224, half);
        }
        if (difficulty == 2)
        {
            owner = progress + (mission & 0xFF) * 2;
            if ((sint16)half < (sint32)r_u16(owner + 344))
                w_u16(owner + 344, half);
        }
        if (difficulty == 3)
        {
            owner = progress + (mission & 0xFF) * 2;
            if ((sint16)half < (sint32)r_u16(owner + 464))
                w_u16(owner + 464, half);
        }
        total = r_u32(progress + 1744);
        w_u32(progress + 1744, total + (uint32)(sint32)(sint16)half);
    }
    game = r_u32(0x800BCE60);
    resource002_sub_800fe950((uint32)r_s16(game + 908));
    game = r_u32(0x800BCE60);
    resource002_sub_800fec84((uint32)r_s16(game + 908));
    owner = progress + (mission & 0xFF);
    w_u8(owner + 944, rating);
    if (difficulty == 0)
    {
        value = r_u8(owner + 704);
        if (value == 0 || (rating & 0xFF) < value)
            w_u8(owner + 704, rating);
    }
    if (difficulty == 1)
    {
        owner = progress + (mission & 0xFF);
        value = r_u8(owner + 764);
        if (value == 0 || (rating & 0xFF) < value)
            w_u8(owner + 764, rating);
    }
    if (difficulty == 2)
    {
        owner = progress + (mission & 0xFF);
        value = r_u8(owner + 824);
        if (value == 0 || (rating & 0xFF) < value)
            w_u8(owner + 824, rating);
    }
    if (difficulty == 3)
    {
        owner = progress + (mission & 0xFF);
        value = r_u8(owner + 884);
        if (value == 0 || (rating & 0xFF) < value)
            w_u8(owner + 884, rating);
    }
    if ((rating & 0xFF) < 6)
    {
        target = r_u32(0x800CD16C + (rating & 0xFF) * 4);
        switch (target)
        {
            case 0x800F5EDC:
                value = 0;
                break;
            case 0x800F5EF4:
                value = 1;
                break;
            case 0x800F5F0C:
                value = 2;
                break;
            case 0x800F5F24:
                value = 3;
                break;
            case 0x800F5F3C:
                value = 4;
                break;
            case 0x800F5F54:
                value = 5;
                break;
            default:
#ifndef NDEBUG
                fprintf(stderr, "Unbound resource2 rating branch %08X\n", target);
#endif
                abort();
        }
        owner = r_u32(0x80112DC0);
        packet = r_u32(owner + 92);
        w_u8(packet + 1, value);
    }
    owner = r_u32(0x80112DC0);
    packet = r_u32(owner + 92);
    w_u8(packet + 1, rating);
    game = r_u32(0x800BCE60);
    value = r_u8(profile + 10);
    owner = game + (mission & 0xFF) * 4;
    w_u32(owner + 3660, value);
    count = r_u8(profile + 10);
    total = r_u32(game + 4144);
    flags = (uint32)r_s16(game + 2392);
    w_u32(game + 4144, total + count);
    total = r_u32(owner + 3900);
    w_u32(owner + 3900, total + flags);
    flags = (uint32)r_s16(game + 2392);
    total = r_u32(game + 4148) + flags;
    w_u32(game + 4148, total);
    return total;
}

uint32 resource002_sub_800f35b8(uint32 layout)
{
    uint32 state, owner, table, texture, y, x, palette_y, palette_x, mode, target;
    sint32 signed_x, signed_y, remainder;
    FUNCTION_MARKER(0x800F35B8, "ACE3.BIN");
    target = 0x800F3B5C;
    if (layout - 1 < 9)
        target = r_u32(0x800CCFA8 + (layout - 1) * 4);
    switch (target)
    {
        case 0x800F35E4:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 16);
            w_u16(state + 0x28F2, 2);
            w_u16(state + 0x28F4, 320);
            w_u16(state + 0x28F6, 320);
            w_u16(state + 0x28FC, 320);
            w_u8(state + 0x2902, 32);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28FE, 344);
            w_u8(owner + 0x2903, 0);
            goto clear_modes;
        case 0x800F3654:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F2, 2);
            w_u16(state + 0x28F4, 392);
            w_u16(state + 0x28F6, 280);
            w_u16(state + 0x28FC, 384);
            w_u8(state + 0x2902, 32);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 32);
            w_u16(state + 0x28FE, 510);
            w_u8(owner + 0x2903, 0);
            goto clear_modes;
        case 0x800F36C4:
        case 0x800F3758:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 16);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 21);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 16);
            w_u16(state + 0x28F4, 768);
            w_u16(state + 0x28F6, target == 0x800F36C4 ? 382 : 256);
            w_u16(state + 0x28FC, 400);
            w_u8(state + 0x2902, 33);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28FE, 45);
            w_u8(owner + 0x2903, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 4);
            owner = r_u32(0x80112E00) + 0x8000;
            table = target == 0x800F36C4 ? 0x8010BB38 : 0x8010BAD8;
            w_u32(state + 0x2904, table);
            w_u8(owner + 0x2908, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2909, 2);
            break;
        case 0x800F37EC:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 12);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 16);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 21);
            w_u16(state + 0x28F4, 832);
            w_u16(state + 0x28FC, 416);
            w_u8(state + 0x2902, 33);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28F6, 0);
            w_u16(state + 0x28FE, 19);
            w_u8(owner + 0x2903, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 4);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u32(state + 0x2904, 0x8010BBB0);
            w_u8(owner + 0x2908, 0);
            goto clear_mode_index;
        case 0x800F3888:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 12);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 16);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F4, 832);
            w_u8(state + 0x2902, 33);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 21);
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28F6, 0);
            w_u16(state + 0x28FC, 416);
            w_u16(state + 0x28FE, 21);
            w_u8(owner + 0x2903, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 4);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u32(state + 0x2904, 0x8010BBB0);
            w_u8(owner + 0x2908, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2909, 2);
            break;
        case 0x800F3930:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 10);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 25);
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28F4, 384);
            w_u16(state + 0x28F6, 384);
            w_u16(state + 0x28FC, 400);
            w_u8(state + 0x2902, 48);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28FE, 511);
            w_u8(owner + 0x2903, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 6);
        clear_modes:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2908, 0);
        clear_mode_index:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2909, 0);
            break;
        case 0x800F39D4:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 8);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 29);
            state = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 12);
            w_u16(state + 0x28F4, 918);
            w_u16(state + 0x28F6, 384);
            w_u16(state + 0x28FC, 432);
            w_u8(state + 0x2902, 48);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28FE, 47);
            w_u8(owner + 0x2903, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2908, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2909, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2903, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u32(state + 0x2904, 0x8010BB98);
            break;
        case 0x800F3A98:
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2900, 8);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2901, 29);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2902, 48);
            owner = r_u32(0x80112E00) + 0x8000;
            w_u16(state + 0x28F0, 12);
            w_u16(state + 0x28F4, 896);
            w_u16(state + 0x28F6, 384);
            w_u16(state + 0x28F2, 1);
            w_u16(state + 0x28FC, 432);
            w_u16(state + 0x28FE, 48);
            w_u8(owner + 0x2903, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x290A, 6);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2908, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2909, 0);
            state = r_u32(0x80112E00) + 0x8000;
            w_u8(state + 0x2903, 1);
            state = r_u32(0x80112E00) + 0x8000;
            w_u32(state + 0x2904, 0x8010BBA4);
            break;
        case 0x800F3B5C:
            break;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 layout branch %08X\n", target);
#endif
            abort();
    }
    state = r_u32(0x80112E00) + 0x8000;
    signed_x = r_s16(state + 0x28F4);
    mode = r_u8(state + 0x2909);
    y = r_u16(state + 0x28F6);
    x = r_u16(state + 0x28F4);
    texture = ((mode & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    palette_y = r_u16(state + 0x28FE);
    palette_x = r_u16(state + 0x28FC);
    w_u16(state + 0x28EC, texture);
    w_u16(state + 0x28EE, (palette_y << 6) | ((palette_x >> 4) & 0x3F));
    remainder = signed_x % 64;
    signed_y = r_s16(state + 0x28F6);
    w_u16(state + 0x28F8, (uint32)(remainder * 4));
    remainder = signed_y % 256;
    w_u16(state + 0x28FA, (uint32)remainder);
    return (uint32)remainder;
}

uint32 resource002_sub_800f3dbc(uint32 ordering, uint32 game)
{
    uint32 state, bank, value, owner, packet, link, tag;
    FUNCTION_MARKER(0x800F3DBC, "ACE3.BIN");
    bank = r_u32(game + 4);
    state = r_u32(0x80112E00);
    w_u32(state + 24000, state + bank * 12000);
    bank = r_u32(game + 4);
    owner = state + 0x8000;
    w_u32(owner + 0xCA4, state + bank * 6000 + 24004);
    bank = r_u32(game + 4);
    w_u32(owner + 0x28C8, state + bank * 3600 + 36008);
    bank = r_u32(game + 4);
    value = r_u32(state + bank * 4 + 0x8000 + 0x28D4);
    w_u32(owner + 0x28DC, value);
    w_u32(owner + 0x28E0, value);
    ClearOTagR(psx_addr(value, 4), 1);
    owner = r_u32(0x80112E00) + 0x8000;
    packet = r_u32(owner + 0x28DC);
    link = r_u32(ordering);
    tag = r_u32(packet);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    tag = r_u32(ordering);
    value = r_u32(owner + 0x28DC) & 0xFFFFFF;
    w_u32(ordering, (tag & 0xFF000000) | value);
    return value;
}

uint32 resource002_sub_800f3db4(uint32 ordering)
{
    uint32 game;
    FUNCTION_MARKER(0x800F3DB4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return resource002_sub_800f3dbc(ordering, game);
}

uint32 resource002_sub_800f4df8(uint32 packet, uint32 state)
{
    uint32 profile, point, x, y, i, fade, shade, previous, write_packet;
    uint32 current_x, current_y, next_x, next_y, owner, ordering, tag, link, result;
    sint32 quarter, distance;
    FUNCTION_MARKER(0x800F4DF8, "ACE3.BIN");
    quarter = r_s16(state + 2) / 4;
    profile = r_u32(0x80112E08);
    point = profile + (uint32)quarter * 2;
    x = r_u8(point + 12);
    y = r_u8(point + 13);
    fade = 2;
    shade = 128;
    write_packet = packet - 20;
    for (i = 0; i < 40; ++i)
    {
        state = r_u32(0x80112E04);
        quarter = r_s16(state + 2) / 4;
        distance = (sint32)((uint32)quarter - i);
        if (distance > 0)
        {
            profile = r_u32(0x80112E08);
            if (distance < r_s16(profile + 8))
            {
                point = profile + (uint32)distance * 2;
                current_x = r_u8(point + 12) - x + 83;
                current_y = (y - r_u8(point + 13)) * 2 + 192;
                point = profile + ((uint32)distance - 1) * 2;
                next_x = r_u8(point + 12) - x + 83;
                write_packet += 20;
                next_y = r_u8(point + 13);
                w_u8(write_packet + 3, 4);
                w_u8(write_packet + 7, 82);
                w_u8(write_packet + 4, shade);
                w_u8(write_packet + 5, shade);
                w_u8(write_packet + 6, shade);
                w_u8(write_packet + 12, 128 - fade);
                w_u8(write_packet + 13, 128 - fade);
                w_u8(write_packet + 14, 128 - fade);
                owner = r_u32(0x80112DC4);
                w_u16(write_packet + 8, current_x);
                w_u16(write_packet + 10, current_y);
                w_u16(write_packet + 16, next_x);
                w_u16(write_packet + 18, (y - next_y) * 2 + 192);
                ordering = r_u32(owner + 8);
                tag = r_u32(write_packet);
                link = r_u32(ordering);
                w_u32(write_packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
                previous = packet;
                ordering = r_u32(owner + 8);
                packet += 20;
                tag = r_u32(ordering);
                w_u32(ordering, (tag & 0xFF000000) | (previous & 0xFFFFFF));
            }
        }
        fade += 2;
        shade -= 2;
    }
    previous = packet;
    result = packet + 12;
    sub_800ac7f8(previous, 0, 0, 32, 0);
    owner = r_u32(0x80112DC4);
    ordering = r_u32(owner + 8);
    tag = r_u32(previous);
    link = r_u32(ordering);
    w_u32(previous, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(owner + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (previous & 0xFFFFFF));
    return result;
}

uint32 resource002_sub_800f4df0(uint32 packet)
{
    uint32 state;
    FUNCTION_MARKER(0x800F4DF0, "ACE3.BIN");
    state = r_u32(0x80112E04);
    return resource002_sub_800f4df8(packet, state);
}

uint32 resource002_sub_800f59b4(void)
{
    uint32 state, value, mode, result;
    sint32 threshold;
    FUNCTION_MARKER(0x800F59B4, "ACE3.BIN");
    state = r_u32(0x80112E04);
    value = r_u16(state + 980);
    mode = r_u8(state);
    value += 1;
    w_u16(state + 980, value);
    threshold = r_s16(0x8010E634 + mode * 2);
    if ((sint16)value < threshold)
        return 1;
    if (mode == 0)
        w_u8(state, mode + 1);
    else
        w_u8(state + 978, 1);
    result = r_u32(0x80112E04);
    w_u16(result + 980, 0);
    return result;
}

uint32 resource002_sub_800f5a24(void)
{
    uint32 state, game, limit, value;
    sint32 elapsed;
    FUNCTION_MARKER(0x800F5A24, "ACE3.BIN");
    state = r_u32(0x80112E04);
    if (r_u8(state) == 0)
        return 0;
    game = r_u32(0x800BCE60);
    limit = r_u16(game + 954);
    elapsed = r_s16(state + 2);
    limit = (uint32)(((unsigned long long)limit * 0x88888889u) >> 36);
    value = r_u16(state + 2) + 1;
    if (elapsed >= (sint32)(limit & 0xFFFF))
        return 0;
    w_u16(state + 2, value);
    return value;
}

uint32 resource002_sub_800dee08(uint32 slot, uint32 first, sint32 second, sint32 third)
{
    uint32 state, result, offset;
    FUNCTION_MARKER(0x800DEE08, "ACE3.BIN");
    state = r_u32(0x80112DC0);
    result = r_u32(state + 88);
    offset = slot * 2;
    result += offset;
    w_u16(result, first);
    if (third >= 0)
    {
        result = r_u32(state + 84) + offset;
        w_u16(result, (uint32)third);
    }
    if (second >= 0)
    {
        state = r_u32(0x80112DC0);
        result = r_u32(state + 80) + offset;
        w_u16(result, (uint32)second);
    }
    return result;
}

uint32 resource002_sub_800ecec8(uint32 resources)
{
    uint32 temporary, entries, i, data;
    sint32 count;
    FUNCTION_MARKER(0x800ECEC8, "ACE3.BIN");
    count = r_s32(resources);
    temporary = arena_alloc(0x9000);
    entries = sub_80027338(resources, NULL);
    for (i = 0; (sint32)i < count; ++i)
    {
        data = r_u32(entries);
        if (r_u32(data) == 0x1A7A6C55)
        {
            sub_800afd08(data, temporary);
            data = temporary;
        }
        entries += 4;
        sub_800271a0(data);
        DrawSync(0);
    }
    arena_set_cursor(temporary);
    return 1;
}
