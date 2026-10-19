#include "game_functions.h"
#include <stdlib.h>

void resource002_sub_800d31a8(void)
{
    FUNCTION_MARKER(0x800D31A8, "ACE3.BIN");
}

uint32 resource002_sub_800d31b0(void)
{
    FUNCTION_MARKER(0x800D31B0, "ACE3.BIN");
    resource002_sub_800f35b8(6);
    return resource002_sub_800f33d0(128, 128, 128);
}

uint32 resource002_sub_800d31e0(void)
{
    FUNCTION_MARKER(0x800D31E0, "ACE3.BIN");
    resource002_sub_800f35b8(6);
    return resource002_sub_800f33d0(128, 128, 128);
}

uint32 resource002_sub_800d3210(void)
{
    FUNCTION_MARKER(0x800D3210, "ACE3.BIN");
    resource002_sub_800f35b8(6);
    return resource002_sub_800f33d0(128, 128, 128);
}

uint32 resource002_sub_800d3240(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D3240, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    if (!r_s16(context + 12))
        resource002_sub_800d31a8();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 1)
        resource002_sub_800d31b0();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 2)
        resource002_sub_800d31e0();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 3)
        return resource002_sub_800d3210();
    return 3;
}

uint32 resource002_sub_800d4394(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D4394, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    if (!r_s16(context + 12))
        resource002_sub_800d3388();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 1)
        resource002_sub_800d38ec();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 2)
        resource002_sub_800d3be0();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 3)
        return resource002_sub_800d3fa0();
    return 3;
}

uint32 resource002_sub_800d4ca4(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D4CA4, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    if (!r_s16(context + 12))
        resource002_sub_800d4430();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 1)
        resource002_sub_800d47dc();
    context = r_u32(0x80112DDC);
    if (r_s16(context + 12) == 3)
        return resource002_sub_800d48f8();
    return 3;
}

uint32 resource002_sub_80106000(void)
{
    uint32 owner, state, index;

    FUNCTION_MARKER(0x80106000, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2517, 1);
    state = r_u32(0x80112DC4);
    w_u32(state + 80, 0);
    resource002_sub_800dee08(1, 1, 0, 120);
    for (index = 2; index < 5; ++index)
        resource002_sub_800def94(index, 0);
    return 0;
}

uint32 resource002_sub_8010606c(void)
{
    uint32 owner, state, game, value, result;
    sint32 counter;

    FUNCTION_MARKER(0x8010606C, "ACE3.BIN");
    resource002_sub_80102184();
    state = r_u32(0x80112DC4);
    counter = r_s32(state + 80);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2521, counter < 40 ? 1 : (counter < 270 ? 2 : 0));
    state = r_u32(0x80112DC4);
    counter = r_s32(state + 80);
    if (counter < 30 || counter >= 55)
        value = 0;
    else if (counter < 35)
        value = 1;
    else if (counter < 45)
        value = 2;
    else if (counter < 50)
        value = 3;
    else
        value = 4;
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2520, value);
    owner = r_u32(0x80112DAC);
    resource002_sub_800def94(1, r_u8(owner + 2520));
    state = r_u32(0x80112DC4);
    counter = r_s32(state + 80);
    if (counter >= 121)
    {
        owner = r_u32(0x80112DAC);
        value = r_u8(owner + 2516) + 1;
        w_u8(owner + 2516, value);
        owner = r_u32(0x80112DAC);
        w_u8(owner + 2517, 0);
        state = r_u32(0x80112DC4);
        counter = r_s32(state + 80);
    }
    if (counter < 121)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 0x10) && !(r_u32(game + 296) & 0x2F))
        {
            owner = r_u32(0x80112DAC);
            value = r_u8(owner + 2516) + 1;
            w_u8(owner + 2516, value);
            owner = r_u32(0x80112DAC);
            w_u8(owner + 2517, 0);
            resource002_sub_800deca8(0, 5);
            resource002_sub_800deca8(0, 7);
            resource002_sub_800dee08(1, 2, 120, 120);
            resource002_sub_800def94(1, 0);
            resource002_sub_800def94(2, 0);
            state = r_u32(0x80112DC4);
            w_u32(state + 420, 400);
        }
    }
    state = r_u32(0x80112DC4);
    result = r_u32(state + 80) + 1;
    w_u32(state + 80, result);
    return result;
}

uint32 resource002_sub_801062b8(void)
{
    uint32 owner, state, game, value, buffer, packet, commands, index, bank;
    uint32 packet_offset, command_offset;
    void *native_command;

    FUNCTION_MARKER(0x801062B8, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    state = r_u32(0x80112DC4);
    w_u16(0x80112E2E, 0);
    w_u16(0x80112E2C, 0);
    w_u32(owner + 2512, 0);
    w_u32(state + 80, 0);
    value = r_u8(owner + 2517) + 1;
    w_u8(owner + 2517, value);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2520, 0);
    game = r_u32(0x800BCE60);
    resource002_sub_800f7524(1, (uint32)r_s16(game + 908));
    for (bank = 0; bank < 2; ++bank)
    {
        state = r_u32(0x80112DC4);
        buffer = r_u32(state + 56 + bank * 4);
        for (index = 0; index < 32; ++index)
        {
            packet = buffer + index * 40 + 2956;
            w_u8(packet + 3, 9);
            w_u8(packet + 7, 44);
            w_u16(packet + 22, 15);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
        }
    }
    packet_offset = 2260;
    command_offset = 2340;
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DAC);
        packet = owner + packet_offset;
        commands = owner + command_offset;
        native_command = psx_addr(commands, 12);
        SetDrawMode(native_command, 0, 0, 0, NULL);
        native_command = psx_addr(commands + 12, 12);
        SetDrawMode(native_command, 0, 0, 76, NULL);
        native_command = psx_addr(commands + 24, 12);
        SetDrawMode(native_command, 0, 0, 11, NULL);
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 102);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u16(packet + 16, 60);
        w_u16(packet + 18, 69);
        packet += 20;
        command_offset += 36;
        packet_offset += 40;
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 102);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u16(packet + 16, 60);
        w_u16(packet + 18, 69);
    }
    packet_offset = 2412;
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DAC);
        packet = owner + packet_offset;
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 100);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u16(packet + 16, 56);
        w_u16(packet + 18, 12);
        packet += 20;
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 100);
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        w_u16(packet + 16, 56);
        w_u16(packet + 18, 12);
        packet_offset += 40;
    }
    resource002_sub_80102184();
    resource002_sub_80103834();
    return resource002_sub_80103854();
}

uint32 resource002_sub_8010655c(void)
{
    uint32 game;

    FUNCTION_MARKER(0x8010655C, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    return resource002_sub_80106564(game);
}

uint32 resource002_sub_80106564(uint32 game)
{
    uint32 state, owner, result;

    FUNCTION_MARKER(0x80106564, "ACE3.BIN");
    resource002_sub_800f7524(1, (uint32)r_s16(game + 908));
    resource002_sub_80102184();
    resource002_sub_80103834();
    resource002_sub_80103854();
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DAC);
    w_u32(state + 332, 180);
    result = r_u8(owner + 2517) + 1;
    w_u8(owner + 2517, result);
    return result;
}

uint32 resource002_sub_801065c8(void)
{
    uint32 state, owner, game, packet, ordering, value, result;
    sint32 timer;

    FUNCTION_MARKER(0x801065C8, "ACE3.BIN");
    resource002_sub_80102184();
    resource002_sub_80105e20();
    resource002_sub_80103834();
    resource002_sub_80103854();
    if (!sub_8002b59c())
    {
        state = r_u32(0x80112DC4);
        timer = r_s32(state + 332);
        if (timer >= 0)
        {
            w_u32(state + 332, (uint32)timer - 1);
            return 0;
        }
    }
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    packet = r_u32(state + 628);
    value = sub_8002d3f4(ordering, packet);
    state = r_u32(0x80112DC4);
    w_u32(state + 628, value);
    value = resource002_sub_800f787c();
    value = sub_8002cae4(value);
    if (value != 0xFFFFFFFF)
    {
        game = r_u32(0x800BCE60);
        result = r_u32(game + 300) & 0x10;
        if (!result)
            return result;
        result = r_u32(game + 296) & 0x2F;
        if (result)
            return result;
    }
    owner = r_u32(0x80112DAC);
    value = r_u8(owner + 2516) + 1;
    w_u8(owner + 2516, value);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2517, 0);
    owner = r_u32(0x80112DAC);
    w_u8(owner + 2518, 1);
    resource002_sub_800decd4(0, 5);
    resource002_sub_800decd4(0, 7);
    resource002_sub_800dee08(1, 3, -1, -1);
    sub_8002db4c(91);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 0x10;
    if (result)
    {
        result = r_u32(game + 296) & 0x2F;
        if (!result)
            return sub_8002db4c(60);
    }
    return result;
}

uint32 resource002_sub_8010673c(void)
{
    uint32 owner;

    FUNCTION_MARKER(0x8010673C, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    return resource002_sub_80106744(owner);
}

uint32 resource002_sub_80106744(uint32 owner)
{
    uint32 value;

    FUNCTION_MARKER(0x80106744, "ACE3.BIN");
    value = r_u8(owner + 2517) + 1;
    w_u8(owner + 2517, value);
    resource002_sub_80102184();
    resource002_sub_80103834();
    return resource002_sub_80103854();
}

uint32 resource002_sub_80106780(void)
{
    FUNCTION_MARKER(0x80106780, "ACE3.BIN");
    resource002_sub_80102184();
    resource002_sub_80103834();
    return resource002_sub_80103854();
}

uint32 resource002_sub_800f787c(void)
{
    uint32 owner, index, table;

    FUNCTION_MARKER(0x800F787C, "ACE3.BIN");
    owner = r_u32(0x80112E0C);
    index = r_u32(owner + 4);
    table = r_u32(owner + 4120);
    return (uint32)r_s16(table + index * 12);
}

uint32 resource002_sub_800d1588(uint32 x, uint32 y, uint32 descriptor, uint32 blend, uint32 packet)
{
    uint32 value, coordinate, component, page, state, ordering, tag, link;
    sint32 remainder;

    FUNCTION_MARKER(0x800D1588, "ACE3.BIN");
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 46);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    value = r_u16(descriptor + 4);
    w_u16(packet + 24, x);
    x -= 1;
    w_u16(packet + 18, y);
    w_u16(packet + 16, value + x);
    value = r_u16(descriptor + 6);
    y -= 1;
    w_u16(packet + 26, value + y);
    value = r_u16(descriptor + 4);
    w_u16(packet + 32, value + x);
    value = r_u16(descriptor + 6);
    w_u16(packet + 34, value + y);
    remainder = r_s16(descriptor) % 64;
    w_u8(packet + 12, (uint32)(remainder * 4));
    remainder = r_s16(descriptor + 2) % 256;
    w_u8(packet + 13, (uint32)remainder);
    remainder = r_s16(descriptor) % 64;
    value = r_u8(descriptor + 4);
    w_u8(packet + 20, (uint32)(remainder * 4) + value - 1);
    remainder = r_s16(descriptor + 2) % 256;
    w_u8(packet + 21, (uint32)remainder);
    remainder = r_s16(descriptor) % 64;
    w_u8(packet + 28, (uint32)(remainder * 4));
    remainder = r_s16(descriptor + 2) % 256;
    value = r_u8(descriptor + 6);
    w_u8(packet + 29, (uint32)remainder + value - 1);
    remainder = r_s16(descriptor) % 64;
    value = r_u8(descriptor + 4);
    w_u8(packet + 36, (uint32)(remainder * 4) + value - 1);
    remainder = r_s16(descriptor + 2) % 256;
    value = r_u8(descriptor + 6);
    w_u8(packet + 4, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 6, 128);
    w_u8(packet + 37, (uint32)remainder + value - 1);
    component = (blend & 3) << 5;
    coordinate = r_u16(descriptor + 2);
    value = r_u16(descriptor);
    w_u16(packet + 14, 0x55A);
    page = component | ((coordinate & 0x100) >> 4) | ((value & 0x3FF) >> 6) | ((coordinate & 0x200) << 2);
    state = r_u32(0x80112DC4);
    w_u16(packet + 22, page);
    ordering = r_u32(state + 8);
    tag = r_u32(packet);
    link = r_u32(ordering + 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 8);
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 40;
}

uint32 resource002_sub_800d32fc(uint32 type)
{
    uint32 context;

    FUNCTION_MARKER(0x800D32FC, "ACE3.BIN");
    if (type == 19)
    {
        context = r_u32(0x80112DDC);
        if (r_u8(context + 180) == 1)
            return 21;
    }
    if (type == 14)
    {
        context = r_u32(0x80112DDC);
        if (r_u8(context + 181) == 1)
            return 22;
    }
    if (type == 15)
    {
        context = r_u32(0x80112DDC);
        if (r_u8(context + 181) == 1)
            return 23;
    }
    return type;
}

uint32 resource002_sub_800d3388(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D3388, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    return resource002_sub_800d3390(context);
}

uint32 resource002_sub_800d3390(uint32 context)
{
    static const uint16 positions[15][2] = {{40, 149}, {40, 183}, {40, 216}, {40, 249}, {40, 282}, {40, 315}, {40, 348}, {252, 138}, {252, 169}, {252, 199}, {252, 229}, {252, 259}, {252, 289}, {252, 319}, {252, 349}};
    uint32 row, buttons, selected, index, state, object, value, entry, mask;
    uint32 packet, result, publish;

    FUNCTION_MARKER(0x800D3390, "ACE3.BIN");
    row = context + 36;
    publish = r_u8(context + 35);
    buttons = context + 132;
    for (index = 10; index < 32; ++index)
    {
        state = r_u32(0x80112DC0);
        object = r_u32(state + 92);
        w_u8(object + index, 0);
    }
    if (publish)
    {
        for (index = 0; index < 15; ++index)
        {
            state = r_u32(0x80112DC0);
            value = r_u8(row + index);
            object = r_u32(state + 92);
            w_u8(object + value + 10, 1);
        }
        for (index = 0; index < 4; ++index)
        {
            entry = buttons + index;
            if (r_u8(entry) < 8)
            {
                state = r_u32(0x80112DC0);
                value = r_u8(entry);
                object = r_u32(state + 92);
                w_u8(object + value + 10, 1);
                state = r_u32(0x80112DC0);
                value = r_u8(entry);
                object = r_u32(state + 92);
                w_u8(object + value + 11, 1);
            }
        }
    }
    context = r_u32(0x80112DDC);
    selected = r_u8(row);
    value = r_u8(context + 31);
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 15; ++index)
    {
        if (index)
            selected = r_u8(row + index);
        value = resource002_sub_800d32fc(selected);
        context = r_u32(0x80112DDC);
        packet = r_u32(context + 8);
        result = resource002_sub_800d1588(positions[index][0], positions[index][1], 0x8010A7C0 + (value << 3), ((mask >> (index + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    return result;
}

uint32 resource002_sub_800d38ec(void)
{
    static const uint8 offsets[10] = {1, 2, 3, 4, 5, 8, 9, 10, 11, 12};
    static const uint16 positions[10][2] = {{40, 183}, {40, 216}, {40, 249}, {40, 282}, {40, 315}, {252, 183}, {252, 216}, {252, 249}, {252, 282}, {252, 315}};
    uint32 context, row, mask, selected, index, value, packet, result;

    FUNCTION_MARKER(0x800D38EC, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    row = context + 52;
    value = r_u8(context + 31);
    selected = r_u8(row + 1);
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 10; ++index)
    {
        if (index)
            selected = r_u8(row + offsets[index]);
        value = resource002_sub_800d32fc(selected);
        context = r_u32(0x80112DDC);
        packet = r_u32(context + 8);
        result = resource002_sub_800d1588(positions[index][0], positions[index][1], 0x8010A7C0 + (value << 3), ((mask >> (index + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    return result;
}

uint32 resource002_sub_800d3be0(void)
{
    static const uint8 offsets[13] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14};
    static const uint16 positions[13][2] = {{40, 160}, {40, 193}, {40, 226}, {40, 259}, {40, 292}, {40, 325}, {252, 149}, {252, 180}, {252, 211}, {252, 242}, {252, 273}, {252, 304}, {252, 335}};
    uint32 context, row, mask, selected, index, value, packet, result;

    FUNCTION_MARKER(0x800D3BE0, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    row = context + 68;
    selected = r_u8(row);
    value = r_u8(context + 31);
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 13; ++index)
    {
        if (index)
            selected = r_u8(row + offsets[index]);
        value = resource002_sub_800d32fc(selected);
        context = r_u32(0x80112DDC);
        packet = r_u32(context + 8);
        result = resource002_sub_800d1588(positions[index][0], positions[index][1], 0x8010A7C0 + (value << 3), ((mask >> (index + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    return result;
}

uint32 resource002_sub_800d3fa0(void)
{
    static const uint8 offsets[13] = {1, 8, 0, 7, 14, 2, 3, 4, 5, 11, 12, 10, 9};
    static const uint16 positions[13][2] = {{41, 150}, {41, 181}, {41, 238}, {41, 268}, {41, 340}, {144, 145}, {144, 177}, {144, 208}, {144, 241}, {252, 150}, {252, 181}, {252, 238}, {252, 268}};
    uint32 context, row, mask, selected, index, value, packet, result, state;

    FUNCTION_MARKER(0x800D3FA0, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    row = context + 84;
    value = r_u8(context + 31);
    selected = r_u8(row + 1);
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 13; ++index)
    {
        if (index)
            selected = r_u8(row + offsets[index]);
        value = resource002_sub_800d32fc(selected);
        context = r_u32(0x80112DDC);
        packet = r_u32(context + 8);
        result = resource002_sub_800d1588(positions[index][0], positions[index][1], 0x8010A7C0 + (value << 3), ((mask >> (index + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, result);
    }
    if (resource002_sub_800dee84(6) == 2)
    {
        state = r_u32(0x80112DC4);
        resource002_sub_800f3ef4(r_u32(state + 8));
    }
    return resource002_sub_800f3ef4(0xFFFFFFFF);
}

uint32 resource002_sub_800f3ef4(uint32 ordering)
{
    uint32 owner, value;

    FUNCTION_MARKER(0x800F3EF4, "ACE3.BIN");
    owner = r_u32(0x80112E00) + 0x8000;
    if (ordering == 0xFFFFFFFF)
    {
        value = r_u32(owner + 0x28E0);
        w_u32(owner + 0x28DC, value);
    }
    else
        w_u32(owner + 0x28DC, ordering);
    return owner;
}

uint32 resource002_sub_800deda0(uint32 group, uint32 item, uint32 destination, uint16 *native_position)
{
    uint32 state, field, table, offset, value;

    FUNCTION_MARKER(0x800DEDA0, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    field = state + (group << 2) + 364;
    offset = ((item << 1) + item) << 2;
    table = r_u32(field);
    value = r_u16(table + offset + 4);
    if (native_position)
        native_position[0] = (uint16)value;
    else
        w_u16(destination, value);
    table = r_u32(field);
    value = r_u16(table + offset + 6);
    if (native_position)
        native_position[1] = (uint16)value;
    else
        w_u16(destination + 2, value);
    return 1;
}

uint32 resource002_sub_800d4430(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D4430, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    return resource002_sub_800d4438(context);
}

uint32 resource002_sub_800d4438(uint32 context)
{
    static const sint16 x_offsets[8] = {45, 45, -1, 82, 235, 235, 188, 271};
    static const uint16 y_positions[8] = {155, 304, 231, 231, 155, 304, 231, 231};
    uint16 position[2];
    uint32 row, adjustments[4], index, type, mask, button, descriptor, packet, x;

    FUNCTION_MARKER(0x800D4438, "ACE3.BIN");
    row = context + 132;
    for (index = 0; index < 4; ++index)
    {
        type = r_u8(row + index);
        if (!type || type == 6 || type == 8)
            adjustments[index] = r_u8(context + 156 + index);
        else
            adjustments[index] = 0;
    }
    context = r_u32(0x80112DDC);
    mask = (uint32)1 << (r_u8(context + 30) & 31);
    resource002_sub_800deda0(2, 9, 0, position);
    packet = 0;
    for (index = 0; index < 8; ++index)
    {
        button = index >> 1;
        x = (uint32)(sint32)(sint16)(uint16)(position[0] + x_offsets[index]);
        type = r_u8(row + button);
        if (index & 1)
            type = type + 1 - adjustments[button];
        else
            type += adjustments[button];
        descriptor = 0x8010A880 + (type << 3);
        if (!index)
        {
            context = r_u32(0x80112DDC);
            packet = r_u32(context + 8);
        }
        packet = resource002_sub_800d1588(x, y_positions[index], descriptor, ((mask >> (button + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, packet);
    }
    return packet;
}

uint32 resource002_sub_800d47dc(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D47DC, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    return resource002_sub_800d47e4(context);
}

uint32 resource002_sub_800d47e4(uint32 context)
{
    uint32 row, type, adjustment, current, mask, packet, descriptor;

    FUNCTION_MARKER(0x800D47E4, "ACE3.BIN");
    row = context + 136;
    type = r_u8(row + 3);
    adjustment = 0;
    if (!type || type == 6 || type == 8)
        adjustment = r_u8(context + 160);
    type = r_u8(row + 3);
    current = r_u32(0x80112DDC);
    descriptor = 0x8010A880 + ((type + adjustment) << 3);
    mask = (uint32)1 << (r_u8(current + 30) & 31);
    mask = ((mask >> 4) & 1) + 1;
    packet = r_u32(current + 8);
    packet = resource002_sub_800d1588(40, 335, descriptor, mask, packet);
    current = r_u32(0x80112DDC);
    w_u32(current + 8, packet);
    type = r_u8(row + 3);
    descriptor = 0x8010A880 + ((type + 1 - adjustment) << 3);
    packet = resource002_sub_800d1588(122, 335, descriptor, mask, packet);
    current = r_u32(0x80112DDC);
    w_u32(current + 8, packet);
    return packet;
}

uint32 resource002_sub_800d48f8(void)
{
    uint32 context;

    FUNCTION_MARKER(0x800D48F8, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    return resource002_sub_800d4900(context);
}

uint32 resource002_sub_800d4900(uint32 context)
{
    static const sint16 x_offsets[8] = {45, 45, -1, 82, 235, 235, 188, 271};
    static const uint16 y_positions[8] = {155, 304, 231, 231, 155, 304, 231, 231};
    uint16 position[2];
    uint32 row, adjustments[4], index, type, mask, button, descriptor, packet, x;

    FUNCTION_MARKER(0x800D4900, "ACE3.BIN");
    row = context + 144;
    for (index = 0; index < 4; ++index)
    {
        type = r_u8(row + index);
        if (!type || type == 6 || type == 8)
            adjustments[index] = r_u8(context + 168 + index);
        else
            adjustments[index] = 0;
    }
    context = r_u32(0x80112DDC);
    mask = (uint32)1 << (r_u8(context + 30) & 31);
    resource002_sub_800deda0(2, 11, 0, position);
    packet = 0;
    for (index = 0; index < 8; ++index)
    {
        button = index >> 1;
        x = (uint32)(sint32)(sint16)(uint16)(position[0] + x_offsets[index]);
        type = r_u8(row + button);
        if (index & 1)
            type = type + 1 - adjustments[button];
        else
            type += adjustments[button];
        descriptor = 0x8010A880 + (type << 3);
        if (!index)
        {
            context = r_u32(0x80112DDC);
            packet = r_u32(context + 8);
        }
        packet = resource002_sub_800d1588(x, y_positions[index], descriptor, ((mask >> (button + 1)) & 1) + 1, packet);
        context = r_u32(0x80112DDC);
        w_u32(context + 8, packet);
    }
    return packet;
}

uint32 resource002_sub_80102120(uint32 matrix)
{
    uint32 first, second, third, fourth, value;
    sint32 result;

    FUNCTION_MARKER(0x80102120, "ACE3.BIN");
    first = r_u16(matrix);
    second = r_u16(matrix + 2);
    third = r_u16(matrix + 14);
    w_u16(matrix, (uint32)((sint32)(sint16)first >> 1));
    value = r_u16(matrix + 4);
    w_u16(matrix + 2, (uint32)((sint32)(sint16)second >> 1));
    fourth = r_u16(matrix + 12);
    w_u16(matrix + 14, (uint32)((sint32)(sint16)third >> 1));
    w_u16(matrix + 4, (uint32)((sint32)(sint16)value >> 1));
    value = r_u16(matrix + 16);
    w_u16(matrix + 12, (uint32)((sint32)(sint16)fourth >> 1));
    result = (sint32)(sint16)value >> 1;
    w_u16(matrix + 16, (uint32)result);
    return (uint32)result;
}

uint32 resource002_sub_80102184(void)
{
    MATRIX matrix = {0};
    VECTOR vector, transformed;
    uint32 state, owner, table, frame, delta, scaled, value, factor, x, y;
    uint32 distance, dx, dy, dz, squared, root, angle, a, b, c, d;
    sint32 counter;

    FUNCTION_MARKER(0x80102184, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + 364);
    x = (uint32)r_s16(table + 64);
    y = (uint32)r_s16(table + 66);
    SetGeomOffset((sint32)(x + 154), (sint32)(y + 154));
    owner = r_u32(0x80112DAC);
    if (!r_u8(owner + 2516))
    {
        state = r_u32(0x80112DC4);
        frame = r_u32(state + 80);
        counter = (sint32)frame;
        if (counter >= 82 && counter < 114)
        {
            delta = frame - 82;
            w_u16(owner + 24, 1024 - (delta << 4));
            frame = r_u32(state + 80);
            w_u16(owner + 28, 0);
            w_u16(owner + 26, (frame << 3) + 1392);
            delta = r_u32(state + 80) - 82;
            value = (delta << 3) + delta;
            scaled = value << 6;
            value += scaled;
            value <<= 2;
            value -= delta;
            scaled = ((value << 2) - value) << 2;
            x = r_u32(owner + 32);
            w_u32(owner + 112, 0x8000 - (uint32)((sint32)scaled / 32));
            frame = r_u32(state + 80);
            factor = 114 - frame;
            scaled = (uint32)((sint32)x / 256) * factor;
            x = (uint32)((sint32)scaled / 32);
            table = r_u32(state + 364);
            value = r_u32(owner + 40);
            a = (uint32)r_s16(table + 64);
            b = (uint32)r_s16(table + 66);
            x += a + 154;
            scaled = (uint32)((sint32)value / 256) * factor;
            y = b + 154 - ((uint32)((sint32)scaled / 32) << 1);
            goto geometry_ready;
        }
    }
    owner = r_u32(0x80112DAC);
    w_u16(owner + 24, 512);
    w_u16(owner + 26, 2304);
    state = r_u32(0x80112DC4);
    w_u16(owner + 28, 0);
    w_u32(owner + 112, 4700);
    table = r_u32(state + 364);
    x = (uint32)r_s16(table + 64) + 154;
    y = (uint32)r_s16(table + 66) + 154;
geometry_ready:
    SetGeomOffset((sint32)x, (sint32)y);
    owner = r_u32(0x80112DAC);
    distance = r_u16(owner + 112);
    RotMatrixYXZ((SVECTOR *)psx_addr(owner + 24, 6), &matrix);
    owner = r_u32(0x80112DAC);
    TransMatrix(&matrix, (VECTOR *)psx_addr(owner + 32, 12));
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, distance);
    xport_gte_execute(0x480012);
    owner = r_u32(0x80112DAC);
    w_u32(owner + 8, xport_gte_read_data(25));
    w_u32(owner + 12, xport_gte_read_data(26));
    w_u32(owner + 16, xport_gte_read_data(27));
    (void)xport_gte_read_control(31);
    owner = r_u32(0x80112DAC);
    a = r_u32(owner + 32);
    b = r_u32(owner + 8);
    dx = a - b;
    squared = dx * dx;
    a = r_u32(owner + 40);
    b = r_u32(owner + 16);
    dz = a - b;
    a = r_u32(owner + 36);
    b = r_u32(owner + 12);
    dy = a - b;
    squared += dz * dz;
    root = SquareRoot0((sint32)squared);
    angle = (uint32)ratan2((sint32)dy, (sint32)root);
    owner = r_u32(0x80112DAC);
    w_u16(owner, 0 - angle);
    angle = (uint32)ratan2((sint32)dx, (sint32)dz);
    owner = r_u32(0x80112DAC);
    w_u16(owner + 2, angle);
    w_u16(owner + 4, 0);
    RotMatrixYXZ((SVECTOR *)psx_addr(owner, 6), (MATRIX *)psx_addr(owner + 48, 32));
    owner = r_u32(0x80112DAC);
    vector.vx = (sint32)(0 - r_u32(owner + 8));
    vector.vy = (sint32)(0 - r_u32(owner + 12));
    vector.vz = (sint32)(0 - r_u32(owner + 16));
    TransMatrix((MATRIX *)psx_addr(owner + 48, 32), &vector);
    owner = r_u32(0x80112DAC);
    a = r_u32(owner + 48);
    b = r_u32(owner + 52);
    c = r_u32(owner + 56);
    d = r_u32(owner + 60);
    w_u32(owner + 80, a);
    w_u32(owner + 84, b);
    w_u32(owner + 88, c);
    w_u32(owner + 92, d);
    a = r_u32(owner + 64);
    b = r_u32(owner + 68);
    c = r_u32(owner + 72);
    d = r_u32(owner + 76);
    w_u32(owner + 96, a);
    w_u32(owner + 100, b);
    w_u32(owner + 104, c);
    w_u32(owner + 108, d);
    TransposeMatrix((MATRIX *)psx_addr(owner + 48, 18), (MATRIX *)psx_addr(owner + 80, 18));
    owner = r_u32(0x80112DAC);
    resource002_sub_80102120(owner + 80);
    owner = r_u32(0x80112DAC);
    ApplyMatrixLV((MATRIX *)psx_addr(owner + 80, 18), &vector, &transformed);
    owner = r_u32(0x80112DAC);
    TransMatrix((MATRIX *)psx_addr(owner + 80, 32), &transformed);
    return owner + 80;
}

uint32 resource002_sub_80103834(void)
{
    FUNCTION_MARKER(0x80103834, "ACE3.BIN");
    return resource002_sub_80103100();
}

uint32 resource002_sub_80102658(uint32 event_time)
{
    static const uint16 uv_offsets[8] = {2968, 2969, 2976, 2977, 2984, 2985, 2992, 2993};
    static const sint16 uv_bias[8] = {32, -64, 47, -64, 32, -49, 47, -49};
    uint32 owner, event, slot, object, flags, type, value, state, buffer, bank, index;
    sint32 signed_value, coordinate;

    FUNCTION_MARKER(0x80102658, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    event = r_u32(owner + 2504);
    slot = r_u8(event + 3);
    object = r_u32(owner + 2524 + (slot << 2));
    flags = r_u32(owner + 2512);
    event = r_u32(owner + 2504);
    w_u32(owner + 2512, flags | ((uint32)1 << (slot & 31)));
    type = r_u8(event + 2);
    value = event_time + 15;
    w_u16(object + 4, value);
    w_u16(object + 6, value);
    w_u16(object + 2, type);
    event = r_u32(owner + 2504);
    value = r_u16(event + 4);
    w_u16(object, value);
    event = r_u32(owner + 2504);
    value = (uint32)r_s16(event + 6);
    w_u16(object + 12, (value - 128) << 7);
    event = r_u32(owner + 2504);
    signed_value = -r_s16(event + 8);
    w_u16(object + 14, (uint32)(signed_value / 10));
    event = r_u32(owner + 2504);
    value = (uint32)r_s16(event + 10);
    w_u16(object + 20, 0);
    w_u16(object + 16, (value - 128) << 7);
    event = r_u32(owner + 2504);
    value = (uint32)r_s16(event + 12) << 12;
    w_u16(object + 24, 0);
    w_u16(object + 22, (uint32)((sint32)value / 360));
    for (bank = 0; bank < 2; ++bank)
    {
        state = r_u32(0x80112DC4);
        owner = r_u32(0x80112DAC);
        event = r_u32(owner + 2504);
        buffer = r_u32(state + 56 + bank * 4);
        slot = r_u8(event + 3);
        w_u16(buffer + slot * 40 + 2970, 0x303F);
        for (index = 0; index < 8; ++index)
        {
            value = r_u16(object);
            signed_value = (sint16)value;
            if (index)
                owner = r_u32(0x80112DAC);
            event = r_u32(owner + 2504);
            slot = r_u8(event + 3);
            if (index & 1)
                coordinate = signed_value / 10;
            else
                coordinate = signed_value % 10;
            coordinate = coordinate * 16 + uv_bias[index];
            w_u8(buffer + slot * 40 + uv_offsets[index], (uint32)coordinate);
        }
    }
    return 0;
}

void resource002_sub_80102650(void)
{
    FUNCTION_MARKER(0x80102650, "ACE3.BIN");
}

uint32 resource002_sub_80102aa8(void)
{
    uint32 owner, event, object, slot, value, old_x, old_z, x, z, dx, dz, angle, root;
    sint32 divisor, result, short_dx, short_dz;

    FUNCTION_MARKER(0x80102AA8, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    event = r_u32(owner + 2504);
    slot = r_u8(event + 3);
    object = r_u32(owner + 2524 + (slot << 2));
    value = r_u8(event + 2);
    w_u16(object + 2, value);
    event = r_u32(owner + 2504);
    value = r_u16(event + 4);
    w_u16(object + 4, value);
    event = r_u32(owner + 2504);
    old_x = r_u16(object + 12);
    value = r_u16(event + 4);
    w_u16(object + 6, value);
    event = r_u32(owner + 2504);
    x = (uint32)r_s16(event + 6) - 128;
    z = (uint32)r_s16(event + 10) - 128;
    x <<= 7;
    z <<= 7;
    dx = x - old_x;
    old_z = r_u16(object + 16);
    dz = z - old_z;
    angle = (uint32)ratan2((sint32)(sint16)(uint16)x - (sint32)(sint16)old_x, (sint32)(sint16)(uint16)z - (sint32)(sint16)old_z);
    short_dx = (sint16)(uint16)dx;
    short_dz = (sint16)(uint16)dz;
    w_u16(object + 22, angle);
    root = SquareRoot0((sint32)((uint32)(short_dx * short_dx) + (uint32)(short_dz * short_dz)));
    owner = r_u32(0x80112DAC);
    event = r_u32(owner + 2504);
    divisor = r_s16(event + 4);
    if (!divisor)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102AA8 BREAK7\n");
#endif
        abort();
    }
    if (divisor == -1 && root == 0x80000000)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102AA8 BREAK6\n");
#endif
        abort();
    }
    result = (sint32)root / divisor;
    w_u16(object + 8, (uint32)result);
    return (uint32)result;
}

uint32 resource002_sub_80102bfc(void)
{
    uint32 owner, slot, shifted, flags, event, object, value;

    FUNCTION_MARKER(0x80102BFC, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    shifted = r_u8(owner + 2510);
    slot = r_u8(owner + 2510);
    flags = r_u16(owner + 2508);
    w_u16(owner + 2508, flags | ((uint32)1 << (shifted & 31)));
    event = r_u32(owner + 2504);
    object = r_u32(owner + (slot << 2) + 2652);
    value = r_u8(event + 4);
    w_u8(object, value);
    owner = r_u32(0x80112DAC);
    event = r_u32(owner + 2504);
    value = r_u8(event + 3);
    w_u8(object + 1, value);
    owner = r_u32(0x80112DAC);
    value = r_u8(owner + 2510) + 1;
    w_u8(owner + 2510, value);
    resource002_sub_800dee08(2, 1, 0, 30);
    resource002_sub_800def94(5, r_u8(object));
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2510) >= 2)
    {
        resource002_sub_800dee08(3, 1, 0, 30);
        resource002_sub_800def94(6, r_u8(object - 4));
    }
    owner = r_u32(0x80112DAC);
    if (r_u8(owner + 2510) >= 3)
    {
        resource002_sub_800dee08(4, 1, 0, 30);
        resource002_sub_800def94(7, r_u8(object - 8));
    }
    return sub_8002db4c(100);
}

uint32 resource002_sub_80102d1c(void)
{
    uint32 owner, first_event, second_event, value, state, buffer, bank, first, second;

    FUNCTION_MARKER(0x80102D1C, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    first_event = r_u32(owner + 2504);
    second_event = r_u32(owner + 2504);
    value = (uint32)r_s16(first_event + 4);
    w_u32(owner + 140, value);
    value = r_u16(second_event + 12);
    w_u32(owner + 136, 0);
    w_u16(owner + 132, value);
    for (bank = 0; bank < 2; ++bank)
    {
        state = r_u32(0x80112DC4);
        owner = r_u32(0x80112DAC);
        buffer = r_u32(state + 56 + bank * 4);
        w_u16(buffer + 4724, 32);
        w_u16(buffer + 4726, 300);
        w_u16(buffer + 4732, 288);
        w_u16(buffer + 4734, 300);
        w_u16(buffer + 4740, 32);
        value = (uint32)r_s16(owner + 132);
        w_u16(buffer + 4748, 288);
        w_u16(buffer + 4742, value * 24 + 300);
        value = (uint32)r_s16(owner + 132);
        w_u8(buffer + 4728, 0);
        owner = r_u32(0x80112DAC);
        w_u16(buffer + 4750, value * 24 + 300);
        value = (uint32)r_s16(owner + 134);
        w_u8(buffer + 4736, 255);
        w_u8(buffer + 4729, value * 12);
        owner = r_u32(0x80112DAC);
        value = (uint32)r_s16(owner + 134);
        w_u8(buffer + 4744, 0);
        w_u8(buffer + 4737, value * 12);
        owner = r_u32(0x80112DAC);
        first = (uint32)r_s16(owner + 134);
        second = (uint32)r_s16(owner + 132);
        w_u8(buffer + 4752, 255);
        w_u8(buffer + 4745, first * 12 + second * 12 - 2);
        owner = r_u32(0x80112DAC);
        first = (uint32)r_s16(owner + 134);
        second = (uint32)r_s16(owner + 132);
        w_u8(buffer + 4753, first * 12 + second * 12 - 2);
    }
    return 0;
}

uint32 resource002_sub_80102e78(void)
{
    uint32 owner, first_event, second_event, value, x, z, old_x, old_z, dx, dz, root;
    sint32 divisor, quotient;

    FUNCTION_MARKER(0x80102E78, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    first_event = r_u32(owner + 2504);
    second_event = r_u32(owner + 2504);
    value = (uint32)r_s16(first_event + 4);
    w_u32(owner + 120, value);
    x = (uint32)r_s16(second_event + 6);
    old_x = r_u32(owner + 32);
    dx = ((x - 128) << 7) - old_x;
    z = (uint32)r_s16(second_event + 10);
    old_z = r_u32(owner + 40);
    dz = ((z - 128) << 7) - old_z;
    root = SquareRoot0((sint32)(dx * dx + dz * dz));
    owner = r_u32(0x80112DAC);
    divisor = r_s32(owner + 120);
    if (!divisor)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102E78 BREAK7\n");
#endif
        abort();
    }
    if (divisor == -1 && root == 0x80000000)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102E78 BREAK6\n");
#endif
        abort();
    }
    quotient = (sint32)root / divisor;
    w_u32(owner + 116, (uint32)quotient);
    value = (uint32)ratan2((sint32)dx, (sint32)dz);
    owner = r_u32(0x80112DAC);
    w_u16(owner + 126, value);
    return value;
}

uint32 resource002_sub_80102f5c(void)
{
    uint32 owner, first_event, second_event, value;

    FUNCTION_MARKER(0x80102F5C, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    first_event = r_u32(owner + 2504);
    second_event = r_u32(owner + 2504);
    value = (uint32)r_s16(first_event + 6);
    w_u32(owner + 36, 0);
    w_u32(owner + 32, (value - 128) << 7);
    value = (uint32)r_s16(second_event + 10);
    value = (value - 128) << 7;
    w_u32(owner + 40, value);
    return value;
}

uint32 resource002_sub_80102f9c(void)
{
    uint32 owner, first_event, second_event, value;

    FUNCTION_MARKER(0x80102F9C, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    first_event = r_u32(owner + 2504);
    value = (uint32)r_s16(first_event + 6);
    second_event = r_u32(owner + 2504);
    w_u16(owner + 2492, (value - 128) << 7);
    value = r_u16(second_event + 8);
    w_u16(owner + 2494, value);
    value = (uint32)r_s16(second_event + 10);
    w_u16(owner + 2496, (value - 128) << 7);
    value = (uint32)r_s16(second_event + 4);
    w_u32(owner + 2500, value);
    return value;
}

void resource002_sub_80105e18(void)
{
    FUNCTION_MARKER(0x80105E18, "ACE3.BIN");
}

void resource002_mode_event_call(uint32 target, uint32 event_time)
{
    switch (target)
    {
        case 0x80102650:
            resource002_sub_80102650();
            return;
        case 0x80102658:
            resource002_sub_80102658(event_time);
            return;
        case 0x80102AA8:
            resource002_sub_80102aa8();
            return;
        case 0x80102BFC:
            resource002_sub_80102bfc();
            return;
        case 0x80102D1C:
            resource002_sub_80102d1c();
            return;
        case 0x80102E78:
            resource002_sub_80102e78();
            return;
        case 0x80102F5C:
            resource002_sub_80102f5c();
            return;
        case 0x80102F9C:
            resource002_sub_80102f9c();
            return;
        case 0x80105E18:
            resource002_sub_80105e18();
            return;
        case 0x80105E20:
            resource002_sub_80105e20();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource2 mode event %08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_80105e20(void)
{
    uint32 owner, event, state, frame, type, target, result;
    sint32 event_time;

    FUNCTION_MARKER(0x80105E20, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    event = r_u32(owner + 2504);
    state = r_u32(0x80112DC4);
    event_time = r_s16(event);
    frame = r_u32(state + 80);
    if ((uint32)event_time == frame)
    {
        do
        {
            owner = r_u32(0x80112DAC);
            event = r_u32(owner + 2504);
            type = r_u8(event + 2);
            event_time = r_s16(event);
            target = r_u32(0x80112B9C + (type << 2));
            resource002_mode_event_call(target, (uint32)event_time);
            owner = r_u32(0x80112DAC);
            event = r_u32(owner + 2504);
            w_u32(owner + 2504, event + 16);
            state = r_u32(0x80112DC4);
            event_time = r_s16(event + 16);
            frame = r_u32(state + 80);
        } while ((uint32)event_time == frame);
    }
    resource002_sub_80105d54();
    resource002_sub_801030e0();
    state = r_u32(0x80112DC4);
    result = r_u32(state + 80) + 1;
    w_u32(state + 80, result);
    return result;
}

uint32 resource002_sub_80105d54(void)
{
    uint32 owner, product, speed, first_delta, second_delta, x, z, remaining;
    sint32 angle, component;

    FUNCTION_MARKER(0x80105D54, "ACE3.BIN");
    owner = r_u32(0x80112DAC);
    if (r_s32(owner + 120) <= 0)
        return 0;
    angle = r_s16(owner + 126);
    component = rsin(angle);
    owner = r_u32(0x80112DAC);
    speed = r_u32(owner + 116);
    product = speed * (uint32)component;
    first_delta = (uint32)((sint32)product / 4096);
    angle = r_s16(owner + 126);
    component = rcos(angle);
    owner = r_u32(0x80112DAC);
    speed = r_u32(owner + 116);
    product = speed * (uint32)component;
    second_delta = (uint32)((sint32)product / 4096);
    x = r_u32(owner + 32);
    remaining = r_u32(owner + 120);
    w_u32(owner + 32, x + first_delta);
    z = r_u32(owner + 40);
    w_u32(owner + 120, remaining - 1);
    w_u32(owner + 40, z + second_delta);
    return 1;
}

uint32 resource002_sub_80102ff0(void)
{
    uint32 index, owner, object, product, first_delta, second_delta, x, z, remaining;
    sint32 angle, component, speed;

    FUNCTION_MARKER(0x80102FF0, "ACE3.BIN");
    for (index = 0; index < 32; ++index)
    {
        owner = r_u32(0x80112DAC);
        object = r_u32(owner + (index << 2) + 2524);
        if (r_s16(object + 2) != 2 || r_s16(object + 4) <= 0)
            continue;
        angle = r_s16(object + 22);
        component = rsin(angle);
        speed = r_s16(object + 8);
        product = (uint32)speed * (uint32)component;
        first_delta = (uint32)((sint32)product / 4096);
        angle = r_s16(object + 22);
        component = rcos(angle);
        speed = r_s16(object + 8);
        product = (uint32)speed * (uint32)component;
        second_delta = (uint32)((sint32)product / 4096);
        x = r_u16(object + 12);
        remaining = r_u16(object + 4);
        w_u16(object + 12, x + first_delta);
        z = r_u16(object + 16);
        w_u16(object + 4, remaining - 1);
        w_u16(object + 16, z + second_delta);
    }
    return 0;
}

uint32 resource002_sub_801030e0(void)
{
    FUNCTION_MARKER(0x801030E0, "ACE3.BIN");
    return resource002_sub_80102ff0();
}

uint32 resource002_sub_80103100(void)
{
    uint32 state;

    FUNCTION_MARKER(0x80103100, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    return resource002_sub_80103108(state);
}

uint32 resource002_sub_80103108(uint32 state)
{
    static const sint8 signs[8] = {-1, 1, 1, 1, -1, -1, 1, -1};
    MATRIX rotation = {0}, composed = {0};
    VECTOR translation;
    SVECTOR vertices[4] = {0};
    uint32 base, index, owner, object, distance, value, remaining, numerator, slot;
    uint32 column, row, screen[4], packet, ordering, tag, link, camera, offset;
    sint32 divisor, quotient, coordinate, first, second;

    FUNCTION_MARKER(0x80103108, "ACE3.BIN");
    base = r_u32(state + 64);
    offset = 0;
    for (index = 0; index < 32; ++index, offset += 40)
    {
        owner = r_u32(0x80112DAC);
        if (!((r_u32(owner + 2512) >> index) & 1))
            continue;
        object = r_u32(owner + (index << 2) + 2524);
        w_u16(object + 28, 0xFFFF);
        w_u16(object + 30, 0xFFFF);
        value = (uint32)r_s16(object + 12);
        distance = value - r_u32(owner + 32);
        if ((sint32)distance < 0)
            distance = 0 - distance;
        if ((sint32)distance >= 1025)
            continue;
        value = (uint32)r_s16(object + 16);
        distance = value - r_u32(owner + 40);
        if ((sint32)distance < 0)
            distance = 0 - distance;
        if ((sint32)distance >= 1025)
            continue;
        if (r_s16(object + 2) == 1 && r_s16(object + 4) > 0)
        {
            for (slot = 0; slot < 8; ++slot)
            {
                divisor = r_s16(object + 6);
                remaining = (uint32)r_s16(object + 4);
                numerator = ((uint32)divisor - remaining) << 7;
                if (signs[slot] < 0)
                    numerator = 0 - numerator;
                if (!divisor)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource2 object 80103108 BREAK7\n");
#endif
                    abort();
                }
                if (divisor == -1 && numerator == 0x80000000)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource2 object 80103108 BREAK6\n");
#endif
                    abort();
                }
                quotient = (sint32)numerator / divisor;
                if (!(slot & 1))
                {
                    vertices[slot >> 1].vy = 0;
                    vertices[slot >> 1].vx = (sint16)quotient;
                }
                else
                    vertices[slot >> 1].vz = (sint16)quotient;
            }
            value = r_u16(object + 4);
            w_u16(object + 4, value - 1);
        }
        else
        {
            vertices[0].vx = -128;
            vertices[0].vy = 0;
            vertices[0].vz = 128;
            vertices[1].vx = 128;
            vertices[1].vy = 0;
            vertices[1].vz = 128;
            vertices[2].vx = -128;
            vertices[2].vy = 0;
            vertices[2].vz = -128;
            vertices[3].vx = 128;
            vertices[3].vy = 0;
            vertices[3].vz = -128;
        }
        RotMatrix((SVECTOR *)psx_addr(object + 20, 6), &rotation);
        translation.vx = r_s16(object + 12);
        translation.vy = r_s16(object + 14);
        translation.vz = r_s16(object + 16);
        TransMatrix(&rotation, &translation);
        owner = r_u32(0x80112DAC);
        camera = owner + 80;
        SetRotMatrix((MATRIX *)psx_addr(camera, 20));
        for (column = 0; column < 3; ++column)
        {
            for (row = 0; row < 3; ++row)
                xport_gte_write_data(9 + row, (uint16)rotation.m[row][column]);
            xport_gte_execute(0x49E012);
            for (row = 0; row < 3; ++row)
                composed.m[row][column] = (sint16)xport_gte_read_data(9 + row);
        }
        owner = r_u32(0x80112DAC);
        SetTransMatrix((MATRIX *)psx_addr(owner + 80, 32));
        value = (uint16)rotation.t[0] | ((uint32)(uint16)rotation.t[1] << 16);
        xport_gte_write_data(0, value);
        xport_gte_write_data(1, (uint32)rotation.t[2]);
        xport_gte_execute(0x480012);
        composed.t[0] = (sint32)xport_gte_read_data(25);
        composed.t[1] = (sint32)xport_gte_read_data(26);
        composed.t[2] = (sint32)xport_gte_read_data(27);
        SetRotMatrix(&composed);
        SetTransMatrix(&composed);
        for (slot = 0; slot < 3; ++slot)
        {
            value = (uint16)vertices[slot].vx | ((uint32)(uint16)vertices[slot].vy << 16);
            xport_gte_write_data(slot * 2, value);
            xport_gte_write_data(slot * 2 + 1, (uint16)vertices[slot].vz);
        }
        xport_gte_execute(0x280030);
        screen[0] = xport_gte_read_data(12);
        screen[1] = xport_gte_read_data(13);
        screen[2] = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        value = (uint16)vertices[3].vx | ((uint32)(uint16)vertices[3].vy << 16);
        xport_gte_write_data(0, value);
        xport_gte_write_data(1, (uint16)vertices[3].vz);
        xport_gte_execute(0x180001);
        screen[3] = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        first = (sint16)screen[0];
        second = (sint16)screen[3];
        coordinate = (first + second) / 2;
        w_u16(object + 28, (uint32)coordinate);
        first = (sint16)(screen[0] >> 16);
        second = (sint16)(screen[3] >> 16);
        coordinate = (first + second) / 2;
        w_u16(object + 30, (uint32)coordinate);
        packet = base + offset + 2956;
        for (slot = 0; slot < 4; ++slot)
        {
            w_u16(packet + 8 + slot * 8, screen[slot]);
            w_u16(packet + 10 + slot * 8, screen[slot] >> 16);
        }
        state = r_u32(0x80112DC4);
        ordering = r_u32(state + 20);
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        ordering = r_u32(state + 20);
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | ((base + offset + 2956) & 0xFFFFFF));
    }
    return 0;
}

uint32 resource002_sub_80103854(void)
{
    uint32 phase2, phase3, phase4, progress3, progress4, game, bank, owner, state;
    uint32 left, right, command, ordering, head, tag, link, count, object, value, result;
    sint32 item, scroll;

    FUNCTION_MARKER(0x80103854, "ACE3.BIN");
    phase2 = resource002_sub_800dee84(2);
    phase3 = resource002_sub_800dee84(3);
    phase4 = resource002_sub_800dee84(4);
    progress3 = resource002_sub_800deea8(3);
    progress4 = resource002_sub_800deea8(4);
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DAC);
    left = owner + bank * 40 + 2260;
    command = owner + bank * 36 + 2340;
    right = owner + bank * 40 + 2412;
    state = r_u32(0x80112DC4);
    tag = r_u32(command);
    ordering = r_u32(state + 8);
    link = r_u32(ordering + 4);
    head = ordering + 4;
    w_u32(command, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering + 4);
    w_u32(ordering + 4, (link & 0xFF000000) | (command & 0xFFFFFF));
    count = r_u8(owner + 2510);
    command += 12;
    if (count >= 2 || (count == 1 && phase2 != 1))
    {
        if (!phase3 || phase3 == 2)
        {
            item = (sint32)r_u8(owner + 2510) - 1;
            w_u8(left + 12, (uint32)((item % 4) * 60));
            w_u8(left + 13, 187);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 1;
            w_u16(left + 14, ((uint32)(item / 4 + 16) << 6) | 27);
            w_u16(left + 8, 17);
            w_u16(left + 10, 73);
            item = (sint32)r_u8(owner + 2510) - 1;
            w_u8(right + 12, (uint32)((item % 4) * 56 + 32));
            w_u8(right + 13, 180);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 1;
            w_u16(right + 14, ((uint32)(item / 4 + 51) << 6) | 25);
            w_u16(right + 8, 17);
            w_u16(right + 10, 147);
            count = r_u8(owner + 2510);
            object = r_u32(owner + ((count - 1) << 2) + 2652);
            value = r_u8(object);
            object = r_u32(owner + (value << 2) + 2524);
            (void)r_u16(object + 28);
            (void)r_u16(object + 30);
        }
        else
        {
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u8(left + 12, (uint32)((item % 4) * 60));
            w_u8(left + 13, 187);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u16(left + 14, ((uint32)(item / 4 + 16) << 6) | 27);
            w_u16(left + 8, 17);
            scroll = (sint32)(progress3 * 107) / 30;
            w_u16(left + 10, (uint32)(scroll + 73));
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u8(right + 12, (uint32)((item % 4) * 56 + 32));
            w_u8(right + 13, 180);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 2;
            value = GetClut(400, item / 4 + 51);
            w_u16(right + 14, value);
            w_u16(right + 8, 17);
            w_u16(right + 10, (uint32)(scroll + 147));
        }
    }
    owner = r_u32(0x80112DAC);
    left += 20;
    count = r_u8(owner + 2510);
    right += 20;
    if (count >= 3 || (count == 2 && phase3 != 1))
    {
        if (!phase4 || phase4 == 2)
        {
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u8(left + 12, (uint32)((item % 4) * 60));
            w_u8(left + 13, 187);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u16(left + 14, ((uint32)(item / 4 + 16) << 6) | 27);
            w_u16(left + 8, 17);
            w_u16(left + 10, 180);
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u8(right + 12, (uint32)((item % 4) * 56 + 32));
            w_u8(right + 13, 180);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 2;
            w_u16(right + 14, ((uint32)(item / 4 + 51) << 6) | 25);
            w_u16(right + 8, 17);
            w_u16(right + 10, 254);
        }
        else
        {
            item = (sint32)r_u8(owner + 2510) - 3;
            w_u8(left + 12, (uint32)((item % 4) * 60));
            w_u8(left + 13, 187);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 3;
            w_u16(left + 14, ((uint32)(item / 4 + 16) << 6) | 27);
            w_u16(left + 8, 17);
            scroll = (sint32)(progress4 * 311) / 30;
            w_u16(left + 10, (uint32)(scroll + 180));
            item = (sint32)r_u8(owner + 2510) - 3;
            w_u8(right + 12, (uint32)((item % 4) * 56 + 32));
            w_u8(right + 13, 180);
            owner = r_u32(0x80112DAC);
            item = (sint32)r_u8(owner + 2510) - 3;
            w_u16(right + 14, ((uint32)(item / 4 + 51) << 6) | 25);
            w_u16(right + 8, 17);
            w_u16(right + 10, (uint32)(scroll + 254));
        }
    }
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DAC);
    count = r_u8(owner + 2510);
    left = owner + bank * 40 + 2260;
    if ((count >= 2 || (count == 1 && phase2 != 1)) && !r_u8(owner + 2518))
    {
        tag = r_u32(left);
        link = r_u32(head);
        value = left & 0xFFFFFF;
        w_u32(left, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(head);
        left += 20;
        w_u32(head, (link & 0xFF000000) | value);
    }
    owner = r_u32(0x80112DAC);
    count = r_u8(owner + 2510);
    if (count >= 3 || (count == 2 && phase3 != 1))
    {
        if (r_u8(owner + 2518))
            goto right_links;
        tag = r_u32(left);
        link = r_u32(head);
        w_u32(left, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(head);
        w_u32(head, (link & 0xFF000000) | (left & 0xFFFFFF));
    }
    owner = r_u32(0x80112DAC);
    if (!r_u8(owner + 2518))
    {
        tag = r_u32(command);
        link = r_u32(head);
        value = command & 0xFFFFFF;
        w_u32(command, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(head);
        command += 12;
        w_u32(head, (link & 0xFF000000) | value);
    }
right_links:
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DAC);
    count = r_u8(owner + 2510);
    right = owner + bank * 40 + 2412;
    if ((count >= 2 || (count == 1 && phase2 != 1)) && !r_u8(owner + 2518))
    {
        tag = r_u32(right);
        link = r_u32(head);
        value = right & 0xFFFFFF;
        w_u32(right, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(head);
        right += 20;
        w_u32(head, (link & 0xFF000000) | value);
    }
    owner = r_u32(0x80112DAC);
    count = r_u8(owner + 2510);
    if ((count >= 3 || (count == 2 && phase3 != 1)) && !r_u8(owner + 2518))
    {
        tag = r_u32(right);
        link = r_u32(head);
        w_u32(right, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(head);
        w_u32(head, (link & 0xFF000000) | (right & 0xFFFFFF));
    }
    tag = r_u32(command);
    link = r_u32(head);
    w_u32(command, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(head);
    result = (link & 0xFF000000) | (command & 0xFFFFFF);
    w_u32(head, result);
    return result;
}

uint32 resource002_sub_800d4d20(void)
{
    uint32 context, next, object, index, mask;
    FUNCTION_MARKER(0x800D4D20, "ACE3.BIN");
    context = r_u32(0x80112DDC);
    w_u8(context + 30, 0);
    next = r_u32(0x80112DDC);
    w_u32(context + 16, 1);
    w_u8(next + 31, 0);
    for (index = 0; index < 15; ++index)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        w_u8(object + index + 8, 0);
    }
    if (r_s16(r_u32(0x80112DDC) + 12) == 0)
    {
        resource002_sub_800deca8(2, 4);
        if (r_u8(r_u32(0x80112DDC) + 28))
        {
            resource002_sub_800deca8(2, 5);
            mask = 48;
        }
        else
        {
            resource002_sub_800deca8(2, 9);
            mask = 528;
        }
        resource002_sub_800dedf0(2, mask);
    }
    if (r_s16(r_u32(0x80112DDC) + 12) == 1)
    {
        resource002_sub_800deca8(2, 4);
        if (r_u8(r_u32(0x80112DDC) + 28))
        {
            resource002_sub_800deca8(2, 6);
            mask = 80;
        }
        else
        {
            resource002_sub_800deca8(2, 10);
            mask = 1040;
        }
        resource002_sub_800dedf0(2, mask);
    }
    if (r_s16(r_u32(0x80112DDC) + 12) == 2)
    {
        resource002_sub_800deca8(2, 4);
        resource002_sub_800deca8(2, 7);
        resource002_sub_800dedf0(2, 144);
        w_u8(r_u32(0x80112DDC) + 28, 1);
    }
    if (r_s16(r_u32(0x80112DDC) + 12) == 3)
    {
        resource002_sub_800deca8(2, 4);
        if (r_u8(r_u32(0x80112DDC) + 28))
        {
            resource002_sub_800deca8(2, 8);
            mask = 272;
        }
        else
        {
            resource002_sub_800deca8(2, 11);
            mask = 2064;
        }
        resource002_sub_800dedf0(2, mask);
        resource002_sub_800dee08(4, 2, 10, 10);
        resource002_sub_800dee08(5, 2, 10, 10);
        resource002_sub_800dee08(6, 0, 0, 0);
        resource002_sub_800dee08(7, 0, 0, 0);
        resource002_sub_800dee08(2, 0, 0, 0);
    }
    return resource002_sub_800dee08(1, 0, 0, 0);
}

uint32 resource002_sub_800d4f78(void)
{
    FUNCTION_MARKER(0x800D4F78, "ACE3.BIN");
    return resource002_sub_800d4f80(r_u32(0x80112DDC));
}

uint32 resource002_sub_800d4f80(uint32 context)
{
    static const uint8 mode2_offsets[] = {12, 13, 14, 4, 5, 6, 7, 34, 9};
    static const uint8 mode45_offsets[] = {12, 13, 14, 2, 3, 4, 5, 6, 7, 34, 9};
    uint32 game, next, raw, value, state, object, index, result;
    sint32 mode, selected;
    FUNCTION_MARKER(0x800D4F80, "ACE3.BIN");
    mode = r_s16(context + 12);
    if (mode == 2 && (uint32)(r_u16(context + 14) - 2u) < 2)
        w_u16(context + 14, 0);
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 0x40) && !(r_u32(game + 296) & 0x2F))
    {
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) != 4)
        {
            sub_8002db4c(60);
            game = r_u32(0x800BCE60);
            context = r_u32(0x80112DDC);
            w_u32(game + 300, 0);
            w_u8(context + 30, 0);
            next = r_u32(0x80112DDC);
            w_u16(context + 14, 4);
            w_u8(next + 31, 0);
        }
    }
    if (mode < 4)
    {
        if (r_u32(r_u32(0x800BCE60) + 304) & 2)
        {
            sub_8002db4c(61);
            context = r_u32(0x80112DDC);
            selected = r_s16(context + 14);
            raw = r_u16(context + 14);
            w_u16(context + 14, selected < 4 ? raw + 1 : 0);
            if (mode == 2)
            {
                context = r_u32(0x80112DDC);
                selected = r_s16(context + 14);
                raw = r_u16(context + 14);
                if (selected == 2)
                    w_u16(context + 14, raw + 2);
            }
        }
        if (r_u32(r_u32(0x800BCE60) + 304) & 1)
        {
            sub_8002db4c(61);
            context = r_u32(0x80112DDC);
            selected = r_s16(context + 14);
            raw = r_u16(context + 14);
            w_u16(context + 14, selected > 0 ? raw - 1 : 4);
            if (mode == 2)
            {
                context = r_u32(0x80112DDC);
                selected = r_s16(context + 14);
                raw = r_u16(context + 14);
                if (selected == 3)
                    w_u16(context + 14, raw - 2);
            }
        }
    }
    else
        w_u16(r_u32(0x80112DDC) + 14, 0);
    if (r_u32(r_u32(0x800BCE60) + 304) & 8)
    {
        if (r_s16(r_u32(0x80112DDC) + 14) < 4)
            sub_8002db4c(61);
        if (r_s16(r_u32(0x80112DDC) + 14) == 0 && mode < 4)
            resource002_sub_800d4d20();
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 1)
        {
            value = r_u8(context + 20);
            w_u8(context + 20, 1u - value);
        }
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 2)
        {
            context += (uint32)mode;
            value = r_u8(context + 21);
            w_u8(context + 21, value < 2 ? value + 1 : 0);
        }
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 3)
        {
            value = r_u8(context + 27);
            w_u8(context + 27, value < 2 ? value + 1 : 0);
        }
    }
    if (r_u32(r_u32(0x800BCE60) + 304) & 4)
    {
        if (r_s16(r_u32(0x80112DDC) + 14) < 4)
            sub_8002db4c(61);
        if (r_s16(r_u32(0x80112DDC) + 14) == 0 && mode < 4)
            resource002_sub_800d4d20();
        context = r_u32(0x80112DDC);
        selected = r_s16(context + 14);
        if (selected == 1)
        {
            value = r_u8(context + 20);
            w_u8(context + 20, 1u - value);
            context = r_u32(0x80112DDC);
            selected = r_s16(context + 14);
        }
        if (selected == 2)
        {
            context += (uint32)mode;
            value = r_u8(context + 21);
            w_u8(context + 21, value ? value - 1 : 2);
        }
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 3)
        {
            value = r_u8(context + 27);
            w_u8(context + 27, value ? value - 1 : 2);
        }
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 0x10) && !(r_u32(game + 296) & 0x2F) && r_s16(r_u32(0x80112DDC) + 14) == 4)
    {
        resource002_sub_800d1f54();
        sub_8002db4c(60);
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
    }
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 0x20) && !(r_u32(game + 296) & 0x5F))
    {
        sub_8002db4c(59);
        resource002_sub_800debb8(0, 1);
        w_u8(r_u32(0x80112DDC), 0);
        w_u8(r_u32(0x80112DDC) + 1, 0);
        w_u32(r_u32(0x800BCE60) + 300, 0);
    }
    state = r_u32(0x80112DC0);
    context = r_u32(0x80112DDC);
    object = r_u32(state + 92);
    value = r_u8(context + 14);
    w_u8(object + 1, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 20) ? 1 : (r_s16(context + 14) == 1 ? 2 : 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 2, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 20) == 1 ? (r_s16(context + 14) == 1 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 3, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + (uint32)mode + 21) ? 1 : (r_s16(context + 14) == 2 ? 2 : 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 4, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + (uint32)mode + 21) == 1 ? (r_s16(context + 14) == 2 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 5, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + (uint32)mode + 21) == 2 ? (r_s16(context + 14) == 2 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 6, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 27) ? 1 : (r_s16(context + 14) == 3 ? 2 : 0);
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 7, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 27) == 1 ? (r_s16(context + 14) == 3 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 34, value);
    context = r_u32(0x80112DDC);
    value = r_u8(context + 27) == 2 ? (r_s16(context + 14) == 3 ? 2 : 0) : 1;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 9, value);
    value = r_s16(r_u32(0x80112DDC) + 14) == 4 ? 2 : 0;
    w_u8(r_u32(r_u32(0x80112DC0) + 92) + 10, value);
    if ((uint32)mode < 2 || mode == 3)
    {
        for (index = 12; index < 15; ++index)
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + index, 0);
    }
    if (mode == 2)
    {
        for (index = 0; index < sizeof(mode2_offsets); ++index)
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + mode2_offsets[index], index ? 1 : 0);
    }
    if ((uint32)(mode - 4) < 2)
    {
        for (index = 0; index < sizeof(mode45_offsets); ++index)
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + mode45_offsets[index], 1);
    }
    if (mode < 4)
    {
        if (r_s16(r_u32(0x80112DDC) + 14) == 0)
            w_u8(r_u32(r_u32(0x80112DC0) + 92) + 20, 0);
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 1)
        {
            state = r_u32(0x80112DC0);
            value = r_u8(context + 20);
            object = r_u32(state + 92);
            w_u8(object + 20, value + 1);
        }
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 2)
        {
            context += (uint32)mode;
            state = r_u32(0x80112DC0);
            value = r_u8(context + 21);
            object = r_u32(state + 92);
            w_u8(object + 20, value + 3);
        }
        context = r_u32(0x80112DDC);
        if (r_s16(context + 14) == 3)
        {
            state = r_u32(0x80112DC0);
            value = r_u8(context + 27);
            object = r_u32(state + 92);
            w_u8(object + 20, value + 101);
        }
        if (r_s16(r_u32(0x80112DDC) + 14) != 4)
            return 4;
        object = r_u32(r_u32(0x80112DC0) + 92);
        result = mode == 1 ? 9 : 8;
    }
    else if (mode == 4)
    {
        object = r_u32(r_u32(0x80112DC0) + 92);
        result = 86;
    }
    else
    {
        if (mode != 5)
            return 5;
        object = r_u32(r_u32(0x80112DC0) + 92);
        result = 87;
    }
    w_u8(object + 20, result);
    return result;
}
