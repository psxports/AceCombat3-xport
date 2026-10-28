#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8003f284(void)
{
    uint32 index, state;
    FUNCTION_MARKER(0x8003F284, "ACE3.BIN");
    for (index = 0; index < 64; ++index)
    {
        state = r_u32(0x800C67A0 + index * 4);
        if (state != 0)
        {
            w_u8(state, 0);
            w_u8(state + 1, 0);
            w_u16(state + 6, 0);
            w_u16(state + 8, 0);
            w_u16(state + 10, 0);
        }
    }
    return 0;
}

uint32 sub_800409ac(void)
{
    FUNCTION_MARKER(0x800409AC, "ACE3.BIN");
    w_u16(0x800BCECC, 160);
    w_u16(0x800BCECE, 120);
    w_u8(0x800BCEC8, 0);
    w_u8(0x800BCEC9, 0);
    w_u16(0x800BCECA, 64);
    return 64;
}

void sub_80041640(void)
{
    FUNCTION_MARKER(0x80041640, "ACE3.BIN");
    w_u8(0x800BCFD4, 0);
    w_u32(0x800BCFD0, 0);
    w_u16(0x800BCFD6, 0);
}

uint32 sub_80041900(void)
{
    FUNCTION_MARKER(0x80041900, "ACE3.BIN");
    w_u8(0x800C7173, 255);
    w_u8(0x800C7174, 255);
    w_u8(0x800C7172, 0);
    w_u16(0x800C7170, 0);
    return 0x800C0000;
}

uint32 sub_80042524(void)
{
    FUNCTION_MARKER(0x80042524, "ACE3.BIN");
    w_u32(0x800C7180, 0xFFFFFFFF);
    w_u32(0x800C7184, 0);
    w_u8(0x800C7188, 0);
    w_u8(0x800C718C, 0);
    return 0x800C7180;
}

void sub_800428ec(void)
{
    FUNCTION_MARKER(0x800428EC, "ACE3.BIN");
    w_u8(0x800BCFD8, 0);
}

uint32 sub_80041120(void)
{
    uint32 group, index, block, table, packet, first, second, result;
    FUNCTION_MARKER(0x80041120, "ACE3.BIN");
    block = arena_alloc(16);
    w_u32(0x800BCFC4, block);
    for (group = 0; group < 2; ++group)
    {
        block = arena_alloc(632);
        table = r_u32(0x800BCFC4);
        w_u32(0x800BCFC8, block);
        w_u32(table + group * 4, block);
        for (index = 0; index < 16; ++index)
        {
            w_u8(r_u32(0x800BCFC8) + index * 20 + 107, 4);
            w_u8(r_u32(0x800BCFC8) + index * 20 + 111, 100);
            packet = r_u32(0x800BCFC8) + index * 20 + 104;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 3; ++index)
        {
            packet = r_u32(0x800BCFC8) + index * 20;
            w_u8(packet + 7, 100);
            w_u8(packet + 7, 102);
            w_u8(packet + 3, 4);
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 131);
            w_u8(packet + 6, 0);
            w_u16(packet + 14, 0x7F98);
        }
        first = r_u32(0x800BCFC8);
        w_u8(first + 12, 170);
        w_u8(first + 13, 0);
        second = r_u32(0x800BCFC8);
        w_u16(first + 16, 49);
        w_u16(first + 18, 15);
        packet = second + 20;
        w_u8(packet + 12, 204);
        w_u8(packet + 13, 16);
        w_u16(packet + 16, 14);
        w_u16(packet + 18, 40);
        for (index = 0; index < 3; ++index)
        {
            packet = r_u32(0x800BCFC8) + 40 + index * 16;
            w_u8(packet + 3, 3);
            w_u8(packet + 7, 98);
        }
        w_u8(r_u32(0x800BCFC8) + 91, 1);
        first = r_u32(0x800BCFC8);
        w_u8(first + 99, 1);
        second = r_u32(0x800BCFC8);
        w_u32(first + 92, 0xE1000636);
        packet = second + 0x1EF;
        w_u32(second + 100, 0xE1000616);
        for (index = 0; index < 2; ++index)
        {
            w_u8(packet, 44);
            w_u8(packet - 4, 9);
            w_u8(packet - 3, 131);
            w_u8(packet - 2, 131);
            w_u8(packet - 1, 131);
            w_u8(packet, 46);
            packet += 40;
        }
        block = r_u32(0x800BCFC8);
        w_u16(block + 510, 37);
        packet = block + 528;
        w_u16(packet + 22, 54);
        w_u8(packet + 13, 87);
        w_u8(packet + 21, 87);
        w_u8(packet + 12, 80);
        w_u8(packet + 20, 123);
        w_u8(packet + 28, 80);
        w_u8(packet + 29, 128);
        w_u8(packet + 36, 123);
        w_u8(packet + 37, 128);
        block = r_u32(0x800BCFC8);
        w_u16(packet + 14, 0x7F98);
        for (index = 0; index < 3; ++index)
        {
            packet = block + 575 + index * 16;
            w_u8(packet, 96);
            w_u8(packet - 4, 3);
            w_u8(packet - 3, 0);
            w_u8(packet - 2, 0);
            w_u8(packet - 1, 0);
            w_u8(packet, 98);
        }
        w_u8(r_u32(0x800BCFC8) + 619, 1);
        first = r_u32(0x800BCFC8);
        w_u8(first + 627, 1);
        second = r_u32(0x800BCFC8);
        w_u32(first + 620, 0xE1000616);
        w_u32(second + 628, 0xE1000616);
    }
    sub_80041640();
    sub_80041900();
    result = sub_80042524();
    sub_800428ec();
    return result;
}

uint32 sub_80042938(void)
{
    uint32 group, index, block, table, packet, other;
    FUNCTION_MARKER(0x80042938, "ACE3.BIN");
    block = arena_alloc(20);
    w_u32(0x800BCFE0, block);
    for (group = 0; group < 2; ++group)
    {
        block = arena_alloc(5200);
        table = r_u32(0x800BCFE0);
        w_u32(0x800BCFE4, block);
        w_u32(table + group * 4, block);
        for (index = 0; index < 128; ++index)
        {
            w_u8(r_u32(0x800BCFE4) + index * 24 + 3, 5);
            w_u8(r_u32(0x800BCFE4) + index * 24 + 7, 72);
            packet = r_u32(0x800BCFE4) + index * 24;
            w_u8(packet + 4, 0);
            other = r_u32(0x800BCFE4);
            w_u32(packet + 20, 0x55555555);
            w_u8(other + index * 24 + 5, 255);
            w_u8(r_u32(0x800BCFE4) + index * 24 + 6, 0);
            packet = r_u32(0x800BCFE4) + index * 24;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 128; ++index)
        {
            w_u8(r_u32(0x800BCFE4) + index * 16 + 0xC03, 3);
            w_u8(r_u32(0x800BCFE4) + index * 16 + 0xC07, 64);
            packet = r_u32(0x800BCFE4) + index * 16 + 0xC00;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
        for (index = 0; index < 4; ++index)
        {
            w_u8(r_u32(0x800BCFE4) + index * 20 + 0x1403, 4);
            w_u8(r_u32(0x800BCFE4) + index * 20 + 0x1407, 32);
            w_u8(r_u32(0x800BCFE4) + index * 20 + 0x1404, 0);
            w_u8(r_u32(0x800BCFE4) + index * 20 + 0x1405, 255);
            w_u8(r_u32(0x800BCFE4) + index * 20 + 0x1406, 0);
            packet = r_u32(0x800BCFE4) + index * 20 + 0x1400;
            w_u8(packet + 7, r_u8(packet + 7) | 2);
        }
    }
    return 0;
}

uint32 sub_8007da98(void)
{
    uint32 state;
    FUNCTION_MARKER(0x8007DA98, "ACE3.BIN");
    state = arena_alloc(0x1C248);
    w_u32(0x800BD0B0, state);
    return sub_8007da1c();
}

uint32 sub_8007da1c(void)
{
    uint32 state, index;
    FUNCTION_MARKER(0x8007DA1C, "ACE3.BIN");
    state = r_u32(0x800BD0B0);
    w_u32(state, 0);
    w_u32(state + 0x12124, 0);
    for (index = 0; index < 32; ++index)
        w_u8(r_u32(0x800BD0B0) + index + 0x12104, 255);
    for (index = 0; index < 32; ++index)
        w_u8(r_u32(0x800BD0B0) + index + 0x1C228, 255);
    return 0;
}

uint32 sub_80084b84(uint32 state)
{
    uint32 index, axis, sample, seed, value, sign, sum, packed, components[3];
    FUNCTION_MARKER(0x80084B84, "ACE3.BIN");
    for (index = 0; index < 528; ++index)
    {
        seed = r_u32(0x800CBF58);
        value = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        seed = r_u32(0x800CBF58);
        sign = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        if ((sign & 1) != 0)
            value = 0 - value;
        w_u16(state + index * 2, value);
    }
    for (index = 0; index < 106; ++index)
    {
        for (axis = 0; axis < 3; ++axis)
        {
            sum = 0;
            for (sample = 0; sample < 4; ++sample)
            {
                seed = r_u32(0x800CBF58);
                value = psyq_rand_next(&seed);
                w_u32(0x800CBF58, seed);
                sum += value & 0x7FF;
            }
            components[axis] = sum >> 2;
        }
        packed = ((components[0] - 1024) & 0x7FF) | (((components[1] - 1024) & 0x7FE) << 10) | ((components[2] - 1024) << 21);
        w_u32(state + 0x420 + index * 4, packed);
    }
    return 0;
}

uint32 sub_800900a4(uint32 state)
{
    uint32 source, a, b, c, d;
    FUNCTION_MARKER(0x800900A4, "ACE3.BIN");
    source = r_u32(r_u32(0x800BCE50) + 12);
    a = r_u32(source + 124);
    b = r_u32(source + 128);
    c = r_u32(source + 132);
    d = r_u32(source + 136);
    w_u32(state + 8, a);
    w_u32(state + 12, b);
    w_u32(state + 16, c);
    w_u32(state + 20, d);
    w_u32(state + 24, r_u32(source + 140));
    return source;
}

uint32 sub_800849ec(void)
{
    uint32 state, index;
    FUNCTION_MARKER(0x800849EC, "ACE3.BIN");
    state = r_u32(0x800BCF90);
    for (index = 0; index < 3; ++index)
    {
        w_u32(state + index * 4 + 0x600, 0);
        w_u32(state + index * 4 + 0x108C, 0);
    }
    state = r_u32(0x800BCF90) + 36;
    for (index = 0; index < 10; ++index)
        w_u32(state - index * 4 + 0x1098, 0);
    state = r_u32(0x800BCF90);
    w_u32(state + 0x36C4, 0);
    for (index = 0; index < 32; ++index)
        w_u32(state + 0x26C - index * 20 + 0x36D8, 0);
    state = r_u32(0x800BCF90);
    w_u32(state + 0x33C0, 0);
    w_u32(state + 0x3948, 0);
    w_u32(state + 0x394C, 0);
    w_u32(state + 0x3964, 0);
    w_u32(state + 0x3B28, 0);
    return state;
}

uint32 sub_8008b178(uint32 state)
{
    uint32 value, a, b, c, d, e, index, out;
    FUNCTION_MARKER(0x8008B178, "ACE3.BIN");
    value = r_u32(r_u32(r_u32(0x800BCE54) + 580) + 60);
    w_u16(0x1F800020, 0);
    w_u32(0x1F80001C, value);
    RotMatrixYXZ((SVECTOR *)psx_addr(0x1F80001C, 8), (MATRIX *)psx_addr(0x1F800024, 32));
    a = r_u32(0x1F800024);
    b = r_u32(0x1F800028);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(0x1F80002C);
    d = r_u32(0x1F800030);
    e = r_u32(0x1F800034);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x4000);
    w_u32(state, 0);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    c = xport_gte_read_data(11);
    w_u16(0x1F80001C, a);
    w_u16(0x1F80001E, b);
    w_u16(0x1F800020, c);
    a = r_u32(0x1F80001C);
    b = r_u32(0x1F800020);
    w_u32(state + 8, a);
    w_u32(state + 12, b);
    out = state + 16;
    for (index = 0; index < 4; ++index)
    {
        value = ((index & 1) << 1) - 1;
        w_u16(0x1F80001C, value * 442);
        value = (index & 2) - 1;
        w_u16(0x1F80001E, value * 442);
        w_u16(0x1F800020, 0x4000);
        xport_gte_write_data(0, r_u32(0x1F80001C));
        xport_gte_write_data(1, r_u32(0x1F800020));
        xport_gte_execute(0x486012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        w_u16(out, a);
        w_u16(out + 2, b);
        w_u16(out + 4, c);
        out += 8;
    }
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 4096);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    c = xport_gte_read_data(11);
    w_u16(0x1F80001C, a);
    w_u16(0x1F80001E, b);
    w_u16(0x1F800020, c);
    a = r_u32(0x1F80001C);
    b = r_u32(0x1F800020);
    w_u32(out, a);
    w_u32(out + 4, b);
    return 0;
}

uint32 sub_80084a6c(void)
{
    uint32 state, game, keys, value, result;
    sint32 mission;
    FUNCTION_MARKER(0x80084A6C, "ACE3.BIN");
    state = arena_alloc(15188);
    w_u32(0x800BCF90, state);
    sub_80084b84(state);
    sub_8008b178(r_u32(0x800BCF90) + 1480);
    sub_800900a4(r_u32(0x800BCF90) + 14664);
    sub_800849ec();
    game = r_u32(0x800BCE60);
    value = r_u32(game + 2412) & 16;
    if (value == 0)
        return 13;
    mission = r_s16(game + 908);
    keys = r_u16(game + 94);
    if (mission == 13)
        return 42;
    if (mission == 42 || mission == 56)
        return 56;
    if ((keys & 0x1008) == 0x1008 && mission != 32 && mission != 47 && mission != 53 && mission != 55)
    {
        value = 7;
        w_u8(game + 938, value);
    }
    else if ((keys & 0x4008) == 0x4008)
    {
        value = 10;
        w_u8(r_u32(0x800BCE60) + 938, value);
    }
    else if ((keys & 0x8008) == 0x8008)
    {
        value = 5;
        w_u8(r_u32(0x800BCE60) + 938, value);
    }
    else
    {
        if ((keys & 0x2008) == 0x2008)
            w_u8(r_u32(0x800BCE60) + 939, 9);
        return 9;
    }
    result = r_u32(0x800BCE60);
    w_u8(result + 939, value);
    return result;
}

uint32 sub_8006c588(void)
{
    uint32 state;
    FUNCTION_MARKER(0x8006C588, "ACE3.BIN");
    state = arena_alloc(0x3564);
    w_u32(0x800BD080, state);
    return sub_8006c530();
}

uint32 sub_8006c530(void)
{
    uint32 state, index;
    FUNCTION_MARKER(0x8006C530, "ACE3.BIN");
    state = r_u32(0x800BD080);
    w_u32(state + 4, 0);
    w_u32(state, 0);
    for (index = 0; index < 64; ++index)
    {
        w_u8(state + index + 8, 255);
        w_u8(state + index + 72, 0);
        w_u8(state + index + 136, 0);
    }
    w_u32(state + 0x154C, 0);
    for (index = 0; index < 4; ++index)
        w_u32(state + 12 - index * 4 + 0x33D0, 0);
    w_u32(state + 0x1548, 0);
    w_u32(state + 0x33E0, 0);
    return state - 4;
}

uint32 sub_8006cce0(void)
{
    uint32 state, index;
    FUNCTION_MARKER(0x8006CCE0, "ACE3.BIN");
    state = arena_alloc(8);
    w_u32(0x800BD088, state);
    for (index = 0; index < 4; ++index)
        w_u16(state + index * 2, 0);
    state = arena_alloc(16);
    w_u32(0x800BD090, state);
    for (index = 0; index < 6; ++index)
        w_u16(state + index * 2, 0);
    w_u32(state + 12, 0);
    state = arena_alloc(16);
    w_u32(0x800BD08C, state);
    state = arena_alloc(112);
    w_u32(0x800BD094, state);
    state = arena_alloc(60);
    w_u32(0x800BD098, state);
    state = arena_alloc(56);
    w_u32(0x800BD0A0, state);
    return state;
}

uint32 sub_8006cd6c(sint32 kind)
{
    uint32 index;
    FUNCTION_MARKER(0x8006CD6C, "ACE3.BIN");
    switch (kind)
    {
        case 1:
        case 12:
            index = 3;
            break;
        case 2:
        case 13:
            index = 4;
            break;
        case 3:
            index = 7;
            break;
        case 4:
            index = 8;
            break;
        case 5:
            index = 9;
            break;
        case 6:
            index = 10;
            break;
        case 7:
            index = 11;
            break;
        case 8:
        case 28:
            index = 19;
            break;
        case 9:
            index = 12;
            break;
        case 10:
            index = 1;
            break;
        case 11:
            index = 2;
            break;
        case 14:
        case 18:
            index = 5;
            break;
        case 15:
        case 19:
            index = 6;
            break;
        case 16:
            index = 13;
            break;
        case 17:
        case 29:
            index = 18;
            break;
        case 20:
            index = 15;
            break;
        case 21:
            index = 16;
            break;
        case 22:
        case 30:
            index = 20;
            break;
        case 23:
            index = 14;
            break;
        case 24:
            index = 17;
            break;
        case 25:
            index = 22;
            break;
        case 26:
            index = 21;
            break;
        case 27:
            index = 23;
            break;
        default:
            index = 0;
            break;
    }
    return r_u32(0x800B3AC8 + index * 4);
}

uint32 sub_80030178(void)
{
    FUNCTION_MARKER(0x80030178, "ACE3.BIN");
    w_u8(0x800BCEA3, 255);
    w_u8(0x800BCEA2, 255);
    w_u8(0x800BCEA1, 255);
    w_u8(0x800BCEA5, 84);
    w_u8(0x800BCEA0, 0);
    w_u8(0x800BCEA4, 0);
    w_u8(0x800BCEA6, 85);
    return 85;
}

uint32 sub_8006ce70(uint32 state)
{
    uint32 table, object, model, control, other, game, value, index;
    sint32 kind, limit, amount;
    FUNCTION_MARKER(0x8006CE70, "ACE3.BIN");
    sub_8006cce0();
    table = r_u32(0x800BCE5C);
    kind = r_s16(state + 72);
    object = r_u32(table);
    model = sub_8006cd6c(kind);
    w_u32(object + 32, 0);
    w_u32(object + 36, 0);
    w_u32(object + 40, 0x20000);
    w_u16(object + 48, 0);
    w_u16(object + 50, 0);
    w_u16(object + 52, 0);
    sub_80030ef8(object + 32, object + 24, object + 56);
    control = r_u32(0x800BD094);
    w_u32(object + 84, 0x80000000);
    w_u32(control, 0);
    w_u32(control + 4, 2);
    w_u32(control + 8, 0);
    w_u32(control + 12, 0);
    w_u32(control + 16, 0);
    w_u16(control + 20, 0);
    w_u16(control + 22, 0);
    w_u16(control + 24, 0);
    w_u32(control + 28, 0);
    w_u32(control + 32, 0);
    w_u32(control + 36, 0);
    w_u32(control + 44, 0);
    w_u16(control + 48, 0);
    w_u16(control + 50, 0);
    w_u16(control + 52, 0);
    w_u16(control + 56, 0);
    w_u16(control + 58, 0);
    w_u16(control + 60, 0);
    w_u16(control + 64, 0);
    w_u16(control + 66, 0);
    w_u16(control + 68, 0);
    value = r_u32(object + 40);
    w_u16(control + 76, 4096);
    w_u16(control + 84, 0);
    w_u16(control + 86, 0);
    w_u32(control + 72, value);
    for (index = 0; index < 12; ++index)
        w_u8(r_u32(0x800BD094) + index + 88, 0);
    for (index = 0; index < 12; ++index)
        w_u8(r_u32(0x800BD094) + index + 100, 0);
    other = r_u32(0x800BD098);
    game = r_u32(0x800BCE60);
    w_u32(other, model);
    value = r_u32(model + 132);
    w_u32(other + 20, value);
    w_u16(object + 78, value);
    if (r_s32(game + 916) >= 2)
    {
        w_u32(other + 20, 30);
        w_u16(object + 78, 30);
    }
    other = r_u32(0x800BD098);
    w_u32(other + 8, r_u32(model + 116));
    w_u32(other + 12, r_s16(model + 128));
    amount = r_s16(model + 130);
    w_u32(other + 24, 0xFFFFFFFF);
    w_u32(other + 16, amount);
    w_u32(other + 28, r_u32(model + 120));
    amount = r_s16(model + 124);
    game = r_u32(0x800BCE60);
    w_u32(other + 4, amount);
    value = r_u8(game + 938);
    limit = r_s16(0x800B3B28 + value * 2);
    if (limit < amount)
        w_u32(other + 4, limit);
    other = r_u32(0x800BD098);
    w_u32(other + 32, 0);
    w_u32(other + 36, 0);
    w_u32(other + 40, 0);
    w_u32(other + 44, 0x7FFFFFFF);
    w_u16(other + 48, 0);
    w_u16(other + 50, 0);
    w_u16(other + 52, 0);
    w_u32(other + 56, 0);
    return sub_80030178();
}

uint32 sub_80030a68(uint32 angles, uint32 out, MATRIX *native_out, const uint16 *native_angles)
{
    uint32 x, y, z, cx, cz, sy, sz, sx, cy, a, b, c, p, q, l, m, value;
    FUNCTION_MARKER(0x80030A68, "ACE3.BIN");
    x = native_angles ? native_angles[0] : r_u16(angles);
    z = native_angles ? native_angles[2] : r_u16(angles + 4);
    y = native_angles ? native_angles[1] : r_u16(angles + 2);
    x &= 0x3FFF;
    z &= 0x3FFF;
    y &= 0x3FFF;
    cx = r_s16(0x800104AC + (x + 4096) * 2);
    cz = r_s16(0x800104AC + (z + 4096) * 2);
    sy = r_s16(0x800104AC + y * 2);
    sz = r_s16(0x800104AC + z * 2);
    xport_gte_write_data(8, cx);
    xport_gte_write_data(9, sy);
    xport_gte_write_data(10, sz);
    xport_gte_write_data(11, cz);
    sx = r_s16(0x800104AC + x * 2);
    xport_gte_execute(0x190003D);
    cy = r_s16(0x800104AC + (y + 4096) * 2);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, cy);
    xport_gte_write_data(9, sz);
    xport_gte_write_data(10, cz);
    xport_gte_write_data(11, cx);
    value = (uint32)((sint32)a >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 4, value);
    else
        w_u16(out + 4, value);
    xport_gte_execute(0x190003D);
    value = (uint32)((sint32)b >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 6, value);
    else
        w_u16(out + 6, value);
    value = (uint32)((sint32)c >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 8, value);
    else
        w_u16(out + 8, value);
    p = xport_gte_read_data(25);
    q = xport_gte_read_data(26);
    value = xport_gte_read_data(27);
    xport_gte_write_data(8, sy);
    xport_gte_write_data(9, cz);
    xport_gte_write_data(10, sz);
    value = (uint32)((sint32)value >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 16, value);
    else
        w_u16(out + 16, value);
    xport_gte_execute(0x190003D);
    value = 0 - sx;
    if (native_out)
        xport_store_le16((uint8 *)native_out + 10, value);
    else
        w_u16(out + 10, value);
    l = xport_gte_read_data(25);
    m = xport_gte_read_data(26);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)l >> 14);
    xport_gte_write_data(10, (sint32)p >> 14);
    xport_gte_write_data(11, (sint32)q >> 14);
    xport_gte_execute(0x190003D);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)m >> 14);
    value = (uint32)((sint32)(a - p) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 2, value);
    else
        w_u16(out + 2, value);
    xport_gte_execute(0x190003D);
    value = (uint32)((sint32)(b - l) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 12, value);
    else
        w_u16(out + 12, value);
    value = (uint32)((sint32)(m + c) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 14, value);
    else
        w_u16(out + 14, value);
    a = xport_gte_read_data(25);
    value = (uint32)((sint32)(q + a) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out, value);
    else
        w_u16(out, value);
    return a;
}

uint32 sub_80030ef8(uint32 vector, uint32 angles, uint32 out)
{
    MATRIX matrix = {0};
    uint32 words[5], low[3], high[3], index, value, magnitude, a, b, c;
    FUNCTION_MARKER(0x80030EF8, "ACE3.BIN");
    sub_80030a68(angles, 0, &matrix, 0);
    for (index = 0; index < 5; ++index)
        words[index] = xport_load_le32((uint8 *)&matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    low[0] = r_u32(vector);
    low[1] = r_u32(vector + 4);
    low[2] = r_u32(vector + 8);
    for (index = 0; index < 3; ++index)
    {
        value = low[index];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[index] = 0 - (uint32)((sint32)magnitude >> 15);
            low[index] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[index] = (uint32)((sint32)value >> 15);
            low[index] = value & 0x7FFF;
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    a <<= 3;
    b <<= 3;
    c <<= 3;
    high[0] = xport_gte_read_data(25);
    high[1] = xport_gte_read_data(26);
    high[2] = xport_gte_read_data(27);
    w_u32(out, high[0] + a);
    w_u32(out + 4, high[1] + b);
    w_u32(out + 8, high[2] + c);
    a = r_u32(out);
    c = r_u32(out + 8);
    w_u32(out, (sint32)a >> 2);
    b = r_u32(out + 4);
    w_u32(out + 8, (sint32)c >> 2);
    value = (uint32)((sint32)b >> 2);
    w_u32(out + 4, value);
    return value;
}

uint32 sub_80030d54(uint32 first, uint32 second)
{
    uint32 offsets[8], index, quadrant = 0, negate = 0, quotient = 0, numerator, denominator, value;
    FUNCTION_MARKER(0x80030D54, "ACE3.BIN");
    for (index = 0; index < 8; ++index)
        offsets[index] = r_u32(0x800234B8 + index * 4);
    if (first == 0 && second == 0)
        return 0;
    if ((sint32)first < 0)
    {
        quadrant += 4;
        first = 0 - first;
        negate ^= 1;
    }
    if ((sint32)second < 0)
    {
        quadrant += 2;
        second = 0 - second;
        negate ^= 1;
    }
    if ((sint32)first < (sint32)second)
    {
        denominator = second;
        numerator = first << 11;
    }
    else
    {
        if (second != first)
        {
            ++quadrant;
            negate ^= 1;
        }
        denominator = first;
        numerator = second << 11;
    }
    if (denominator != 0)
    {
        if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        quotient = (uint32)((sint64)(sint32)numerator / (sint32)denominator);
    }
    value = r_s16(0x8001A4B0 + (quotient << 1));
    if (negate != 0)
        value = 0 - value;
    value += offsets[quadrant];
    if ((sint32)value >= 0x2000)
        value -= 0x4000;
    return value;
}

uint32 sub_8004d794(void)
{
    uint8 record[24];
    uint32 source, a, b, c, d;
    FUNCTION_MARKER(0x8004D794, "ACE3.BIN");
    source = r_u32(r_u32(0x800BCEF8) + 12);
    a = r_u32(source + 4);
    b = r_u32(source + 8);
    c = r_u32(source + 12);
    d = r_u32(source + 16);
    xport_store_le32(record, a);
    xport_store_le32(record + 4, b);
    xport_store_le32(record + 8, c);
    xport_store_le32(record + 12, d);
    a = r_u32(source + 20);
    b = r_u32(source + 24);
    xport_store_le32(record + 16, a);
    xport_store_le32(record + 20, b);
    return sub_80072780(0, record);
}

uint32 sub_800308f0(uint32 angles, uint32 matrix, MATRIX *native_out, const uint16 *native_angles)
{
    uint32 ax;
    uint32 ay;
    uint32 az;
    uint32 cx;
    uint32 cy;
    uint32 cz;
    uint32 sx;
    uint32 sy;
    uint32 sz;
    uint32 a;
    uint32 b;
    uint32 c;
    uint32 d;
    uint32 e;
    uint32 f;
    uint32 g;
    uint32 h;
    uint32 p;
    uint32 q;
    uint32 r;
    uint32 result;

    FUNCTION_MARKER(0x800308F0, "ACE3.BIN");
    ax = native_angles != 0 ? native_angles[0] : r_u16(angles);
    az = native_angles != 0 ? native_angles[2] : r_u16(angles + 4);
    ay = native_angles != 0 ? native_angles[1] : r_u16(angles + 2);
    ax &= 0x3FFF;
    az &= 0x3FFF;
    ay &= 0x3FFF;
    cx = (uint32)r_s16(0x800104AC + (ax + 0x1000) * 2);
    cz = (uint32)r_s16(0x800104AC + (az + 0x1000) * 2);
    sy = (uint32)r_s16(0x800104AC + ay * 2);
    sz = (uint32)r_s16(0x800104AC + az * 2);
    xport_gte_write_data(8, cx);
    xport_gte_write_data(9, sy);
    xport_gte_write_data(10, sz);
    xport_gte_write_data(11, cz);
    sx = (uint32)r_s16(0x800104AC + ax * 2);
    xport_gte_execute(0x190003D);
    cy = (uint32)r_s16(0x800104AC + (ay + 0x1000) * 2);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, cy);
    xport_gte_write_data(9, sz);
    xport_gte_write_data(10, cz);
    xport_gte_write_data(11, cx);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 12, (sint32)a >> 16);
    else
        w_u16(matrix + 12, (sint32)a >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 2, (sint32)b >> 16);
    else
        w_u16(matrix + 2, (sint32)b >> 16);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 8, (sint32)c >> 16);
    else
        w_u16(matrix + 8, (sint32)c >> 16);
    d = xport_gte_read_data(25);
    e = xport_gte_read_data(26);
    f = xport_gte_read_data(27);
    xport_gte_write_data(8, sy);
    xport_gte_write_data(9, cz);
    xport_gte_write_data(10, sz);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 16, (sint32)f >> 16);
    else
        w_u16(matrix + 16, (sint32)f >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 14, (sint32)(0 - sx) >> 2);
    else
        w_u16(matrix + 14, (sint32)(0 - sx) >> 2);
    g = xport_gte_read_data(25);
    h = xport_gte_read_data(26);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)g >> 14);
    xport_gte_write_data(10, (sint32)d >> 14);
    xport_gte_write_data(11, (sint32)e >> 14);
    xport_gte_execute(0x190003D);
    p = xport_gte_read_data(25);
    q = xport_gte_read_data(26);
    r = xport_gte_read_data(27);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)h >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 6, (sint32)(p - d) >> 16);
    else
        w_u16(matrix + 6, (sint32)(p - d) >> 16);
    xport_gte_execute(0x190003D);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 4, (sint32)(q - g) >> 16);
    else
        w_u16(matrix + 4, (sint32)(q - g) >> 16);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 10, (sint32)(h + r) >> 16);
    else
        w_u16(matrix + 10, (sint32)(h + r) >> 16);
    result = xport_gte_read_data(25);
    if (native_out)
        xport_store_le16((uint8 *)native_out, (sint32)(e + result) >> 16);
    else
        w_u16(matrix, (sint32)(e + result) >> 16);
    return result;
}

uint32 sub_80072780(uint32 record, const uint8 *native_record)
{
    MATRIX matrix = {0};
    uint32 object, state, angles, auxiliary, input, flags, a, b, c, d, e, other;
    FUNCTION_MARKER(0x80072780, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C));
    state = r_u32(0x800BD094);
    flags = r_u32(object);
    input = r_u32(0x800BD088);
    w_u32(object, flags | 0x1C000);
    angles = r_u32(0x800BD08C);
    auxiliary = r_u32(0x800BD090);
    w_u16(state + 48, 0);
    w_u16(state + 50, 0);
    w_u16(state + 52, 0);
    w_u16(state + 56, 0);
    w_u16(state + 58, 0);
    w_u16(state + 60, 0);
    w_u16(state + 76, 4096);
    w_u16(angles + 4, 0);
    w_u16(angles + 6, 0);
    w_u16(angles + 8, 0);
    w_u16(angles + 10, 0);
    w_u16(angles + 12, 0);
    w_u16(angles + 14, 4096);
    w_u16(auxiliary, 0);
    w_u16(auxiliary + 2, 0);
    w_u16(auxiliary + 4, 0);
    w_u16(auxiliary + 6, 0);
    w_u16(auxiliary + 8, 0);
    w_u32(auxiliary + 12, 0);
    w_u16(input, 0);
    w_u16(input + 2, 0);
    w_u16(input + 4, 0);
    w_u16(input + 6, 0);
    if (native_record)
    {
        a = xport_load_le32(native_record);
        b = xport_load_le32(native_record + 4);
        c = xport_load_le32(native_record + 8);
        d = xport_load_le32(native_record + 12);
    }
    else
    {
        a = r_u32(record);
        b = r_u32(record + 4);
        c = r_u32(record + 8);
        d = r_u32(record + 12);
    }
    w_u32(object + 8, a);
    w_u32(object + 12, b);
    w_u32(object + 16, c);
    w_u32(object + 20, d);
    if (native_record)
    {
        a = xport_load_le32(native_record + 16);
        b = xport_load_le32(native_record + 20);
    }
    else
    {
        a = r_u32(record + 16);
        b = r_u32(record + 20);
    }
    w_u32(object + 24, a);
    w_u32(object + 28, b);
    w_u32(state + 72, 0x21000);
    rotation_build(object + 24, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le32((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x1080);
    xport_gte_execute(0x486012);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    other = r_u32(0x800BD098);
    a <<= 5;
    b <<= 5;
    c <<= 5;
    w_u32(object + 56, a);
    auxiliary = r_u32(0x800BD094);
    w_u32(object + 60, b);
    w_u32(object + 64, c);
    w_u32(auxiliary + 16, 0);
    w_u32(other + 36, 0);
    return sub_8006ee38(object, state);
}

uint32 sub_8006ee38(uint32 object, uint32 state)
{
    MATRIX first = {0}, second = {0};
    uint32 vector0, vector1, a, b, c, d, e, packed, depth, x, y, z, result;
    uint32 column0[3], column1[3];
    FUNCTION_MARKER(0x8006EE38, "ACE3.BIN");
    vector0 = r_u32(0x800BCE0C);
    vector1 = r_u32(0x800BCE10);
    sub_800308f0(object + 24, 0, &first, 0);
    rotation_build(state + 64, 0, &second, 0);
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    packed = xport_load_le16((uint8 *)&second) | (xport_load_le32((uint8 *)&second + 4) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 12);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 2) | (xport_load_le32((uint8 *)&second + 8) << 16);
    depth = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&second + 14);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = xport_load_le16((uint8 *)&second + 4) | (xport_load_le32((uint8 *)&second + 8) & 0xFFFF0000);
    depth = xport_load_le32((uint8 *)&second + 16);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    xport_store_le32((uint8 *)&first, (column0[0] & 0xFFFF) | (column1[0] << 16));
    xport_store_le32((uint8 *)&first + 12, (column0[2] & 0xFFFF) | (column1[2] << 16));
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    xport_store_le32((uint8 *)&first + 4, (a & 0xFFFF) | (column0[1] << 16));
    xport_store_le32((uint8 *)&first + 8, (column1[1] & 0xFFFF) | (b << 16));
    xport_store_le32((uint8 *)&first + 16, xport_gte_read_data(11));
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le32((uint8 *)&first + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, vector0);
    xport_gte_write_data(1, vector1);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    result = sub_80030d54(0 - y, z);
    w_u16(state + 56, result);
    result = sub_80030d54(x, z);
    w_u16(state + 58, result);
    w_u16(state + 60, 0);
    return result;
}

uint32 sub_80035740(uint32 mode)
{
    uint32 scene, value;
    FUNCTION_MARKER(0x80035740, "ACE3.BIN");
    if (mode != 0 && r_u8(r_u32(0x800BCE60) + 926) == 0)
    {
        scene = r_u32(0x800BCE54);
        if ((r_u32(r_u32(scene + 40) + 80) & 0x800) == 0)
        {
            value = r_u32(scene) & 0xFFFFFFFE;
            w_u32(scene, value);
            return value;
        }
    }
    scene = r_u32(0x800BCE54);
    value = r_u32(scene) | 1;
    w_u32(scene, value);
    return value;
}

uint32 sub_800323f8(uint32 cursor)
{
    uint32 index, block, table, object, flags;
    FUNCTION_MARKER(0x800323F8, "ACE3.BIN");
    arena_set_cursor(cursor);
    for (index = 0; index < 2; ++index)
    {
        block = arena_alloc(0x4000);
        w_u32(r_u32(0x800BCE54) + index * 4 + 24, block);
        block = arena_alloc(0x1B800);
        w_u32(r_u32(0x800BCE54) + index * 4 + 32, block);
    }
    table = r_u32(0x800BCE5C);
    for (index = 0; index < 64; ++index)
    {
        object = r_u32(table + index * 4);
        flags = r_u32(object);
        if ((flags & 0x4000) != 0)
            w_u32(object, flags | 0x2000);
    }
    return sub_8007db30();
}

uint32 sub_80027584(void)
{
    FUNCTION_MARKER(0x80027584, "ACE3.BIN");
    return 0x801FB700 - r_u32(r_u32(0x800BCE60) + 28);
}

uint32 sub_8007db30(void)
{
    uint32 state, bytes, block, value, first, result, index;
    FUNCTION_MARKER(0x8007DB30, "ACE3.BIN");
    state = arena_alloc(0x2054);
    w_u32(0x800BD0B8, state);
    w_u32(state, 0);
    w_u32(state + 0x1018, 0);
    w_u32(state + 0x184C, 0xFFFFFFFF);
    bytes = sub_80027584() - 0x8000;
    w_u32(r_u32(0x800BD0B8) + 0x2050, bytes);
    block = arena_alloc(bytes);
    state = r_u32(0x800BD0B8);
    value = r_u32(state + 0x2050);
    w_u32(state + 4, block);
    result = block + ((value >> 2) << 2);
    first = r_u32(state + 4);
    result -= 64;
    w_u32(state + 8, result);
    w_u32(state + 16, first);
    w_u32(state + 12, first);
    for (index = 0; index < 64; ++index)
        w_u32(state + 0x7E0 - index * 32 + 24, 0);
    return result;
}

uint32 sub_80056148(uint32 source)
{
    uint32 high, low;
    FUNCTION_MARKER(0x80056148, "ACE3.BIN");
    high = r_u8(source + 1);
    low = r_u8(source);
    return (uint32)(sint32)(sint16)(low | (high << 8));
}

uint32 sub_80056234(uint32 id, sint32 value)
{
    uint32 index, state, slot, result;
    FUNCTION_MARKER(0x80056234, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        state = r_u32(0x800BD020);
        slot = state + index;
        if (r_u8(slot + 20) == 255)
        {
            result = index << 1;
            w_u8(slot + 20, id);
            slot = r_u32(0x800BD020) + result;
            w_u16(slot + 12, value);
            w_u16(slot + 4, 0);
            return result;
        }
    }
    return 0;
}

uint32 sub_80032518(uint32 index)
{
    uint32 scene, object, flags, result;
    FUNCTION_MARKER(0x80032518, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    object = r_u32(scene + 40);
    switch (index)
    {
        case 6:
            scene = r_u32(0x800BCE54);
            flags = r_u32(scene);
            w_u32(scene + 672, 0x80000001);
            w_u32(scene + 676, 0x7FFFFFFF);
            w_u32(scene + 680, 0x80000001);
            w_u32(scene + 684, 0);
            w_u32(scene + 688, 0x80000001);
            w_u32(scene + 692, 0x7FFFFFFF);
            w_u32(scene, flags | 24);
            w_u32(object + 124, 0x80000001);
            w_u32(object + 128, 0xFF6E0000);
            w_u32(object + 132, 0x80000001);
            w_u32(object + 140, 0x7FFFFFFF);
            w_u32(object + 144, 0xFFEF8000);
            w_u32(object + 148, 0x7FFFFFFF);
            return 0xFF6E0000;
        case 32:
        case 47:
        case 53:
            scene = r_u32(0x800BCE54);
            flags = r_u32(scene);
            w_u32(scene + 680, 0xFF48A000);
            w_u32(scene + 672, 0x80000001);
            w_u32(scene + 676, 0x7FFFFFFF);
            w_u32(scene + 684, 0x01770000);
            w_u32(scene + 688, 0x80000001);
            w_u32(scene + 692, 0x7FFFFFFF);
            w_u32(scene, flags | 16);
            w_u32(object + 124, 0xDE96E000);
            w_u32(object + 128, 0xFF5D4000);
            w_u32(object + 132, 0xEA906000);
            w_u32(object + 140, 0x056F0000);
            w_u32(object + 144, 0xFFEF8000);
            w_u32(object + 148, 0x11764000);
            return 0x056F0000;
        case 55:
            scene = r_u32(0x800BCE54);
            flags = r_u32(scene);
            w_u32(scene + 680, 0xFF48A000);
            w_u32(scene + 672, 0x80000001);
            w_u32(scene + 676, 0x7FFFFFFF);
            w_u32(scene + 684, 0x01770000);
            w_u32(scene + 688, 0x80000001);
            w_u32(scene + 692, 0x7FFFFFFF);
            w_u32(scene, flags | 16);
            w_u32(object + 124, 0x80000001);
            w_u32(object + 128, 0xFF5D4000);
            w_u32(object + 132, 0x80000001);
            w_u32(object + 140, 0x7FFFFFFF);
            w_u32(object + 144, 0xFFEF8000);
            w_u32(object + 148, 0x7FFFFFFF);
            return scene;
        case 42:
        case 58:
            scene = r_u32(0x800BCE54);
            w_u32(scene + 672, 0x80000001);
            w_u32(scene + 676, 0x7FFFFFFF);
            w_u32(scene + 680, 0x80000001);
            w_u32(scene + 684, 0x7FFFFFFF);
            w_u32(scene + 688, 0x80000001);
            w_u32(scene + 692, 0x7FFFFFFF);
            flags = r_u32(object);
            w_u32(object + 124, 0x80000001);
            w_u32(object + 128, 0x80000001);
            w_u32(object + 132, 0x80000001);
            w_u32(object + 140, 0x7FFFFFFF);
            w_u32(object + 144, 0x7FFFFFFF);
            w_u32(object + 148, 0x7FFFFFFF);
            w_u32(object, flags | 0x1000);
            result = r_u32(scene) | 0x300;
            w_u32(scene, result);
            return result;
        default:
            scene = r_u32(0x800BCE54);
            w_u32(object + 124, 0x80000001);
            w_u32(object + 128, 0x80000001);
            w_u32(object + 132, 0x80000001);
            w_u32(object + 140, 0x7FFFFFFF);
            w_u32(object + 144, 0);
            w_u32(object + 148, 0x7FFFFFFF);
            w_u32(scene + 672, 0x80000001);
            w_u32(scene + 676, 0x7FFFFFFF);
            w_u32(scene + 680, 0x80000001);
            w_u32(scene + 684, 0x7FFFFFFF);
            w_u32(scene + 688, 0x80000001);
            w_u32(scene + 692, 0x7FFFFFFF);
            return 0x80000001;
    }
}

uint32 sub_80031e64(uint32 index)
{
    FUNCTION_MARKER(0x80031E64, "ACE3.BIN");
    sub_80054ebc(r_u32(r_u32(0x800BCEAC) + (index << 2) + 8));
    return sub_80038c28();
}

uint32 sub_80038c28(void)
{
    uint32 game, flags;
    FUNCTION_MARKER(0x80038C28, "ACE3.BIN");
    sub_80053da0();
    sub_8004db40();
    sub_8004e620();
    sub_80030464();
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    w_u32(game, flags | 0x10000000);
    return game;
}

uint32 sub_80053da0(void)
{
    uint32 state, scene, table, index, value, result;
    FUNCTION_MARKER(0x80053DA0, "ACE3.BIN");
    state = r_u32(0x800BCEB4);
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCEB0);
    w_u32(state + 16, 0);
    index = r_u32(scene + 8);
    w_u8(state + 28, r_u8(table + index + 52));
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCEB0);
    index = r_u32(scene + 8);
    value = r_u8(table + index + 52);
    result = index << 2;
    if (value == 1)
    {
        state = r_u32(0x800BCEB4);
        result = r_u32(table + result + 56);
        w_u32(state + 20, result);
    }
    return result;
}

uint32 sub_80032080(uint32 mode, uint32 flags)
{
    uint32 scene, previous, index, result = 0, game, value;
    FUNCTION_MARKER(0x80032080, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    previous = r_u32(scene + 4);
    index = r_u32(scene + 8);
    if (mode == 8)
        sub_80031e9c();
    if (mode == 64)
        sub_80031edc();
    if (mode == 512)
        sub_80031f1c();
    if (mode == 0x2000)
        sub_80031f5c();
    if (mode == 0x10000)
        sub_80031fa0();
    if (previous == 2)
        sub_80031e64(index);
    if (previous == 16)
        sub_80031ea4(index);
    if (previous == 128)
        sub_80031efc();
    if (previous == 1024)
        sub_80031f24(index);
    if (previous == 0x4000)
        sub_80031f64(index);
    if (previous == 8)
        sub_80031fa8(flags);
    if (previous == 64)
        sub_80031fd8(flags);
    if (previous == 512)
        sub_80032008();
    if (previous == 0x2000)
        sub_80032010(flags);
    if (previous == 0x10000)
        sub_80032040(flags);
    if (previous == 0x80000)
        sub_80032040(flags);
    if (previous == 0x100000)
        sub_80032070();
    if (previous == 0x200000)
        sub_80032078();
    switch (previous)
    {
        case 1:
            result = r_u32(0x800B09D4);
            resource_init_queue(0, 0);
            break;
        case 2:
            result = r_u32(0x800B09D8);
            break;
        case 16:
            result = r_u32(0x800B09DC);
            break;
        case 128:
            result = r_u32(0x800B09E0);
            break;
        case 1024:
            result = r_u32(0x800B09E4);
            break;
        case 0x4000:
            result = r_u32(0x800B09E8);
            break;
        case 0x20000:
            result = r_u32(0x800B09EC);
            break;
        case 0x100000:
            result = r_u32(0x800B09F0);
            break;
        case 0x200000:
            result = r_u32(0x800B09F4);
            break;
        case 0x4000000:
            result = r_u32(0x800B0A0C);
            break;
        case 0x10000000:
            game = r_u32(0x800BCE60);
            value = r_u16(game + 940);
            result = r_u32(0x800B0A00);
            w_u16(game + 940, value + 1);
            sub_8003048c();
            resource_init_queue(0, 0);
            DecDCToutCallback(NULL);
            break;
        case 0x400000:
            result = r_u32(0x800B09F8);
            break;
        case 0x800000:
            result = r_u32(0x800B09FC);
            break;
        case 0x1000000:
            result = r_u32(0x800B0A04);
            break;
        case 0x2000000:
            result = r_u32(0x800B0A08);
            break;
    }
    if (previous == 0x400000 || previous == 0x800000 || previous == 0x1000000 || previous == 0x2000000)
    {
        sub_8003048c();
        resource_init_queue(0, 0);
        DecDCToutCallback(NULL);
    }
    return result;
}

uint32 sub_8004db40(void)
{
    uint32 selector, mask, index, object, parameters, value;
    FUNCTION_MARKER(0x8004DB40, "ACE3.BIN");
    selector = r_u8(0x800B1FC0 + r_u32(r_u32(0x800BCE54) + 8));
    mask = selector & 0xF0;
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4);
        if ((r_u32(object) & 0x4000) == 0)
            continue;
        if (r_s16(object + 78) <= 0)
            continue;
        parameters = r_u32(r_u32(0x800BCEF8) + index * 64 + 12);
        value = r_u8(parameters + 29);
        if (value == selector || value == mask)
            sub_8004e0fc(index);
        value = r_u8(parameters + 30);
        if (value == selector || value == mask)
            sub_8004e124(index);
    }
    return 0;
}

uint32 sub_8004e0fc(uint32 index)
{
    uint32 object, result;
    FUNCTION_MARKER(0x8004E0FC, "ACE3.BIN");
    object = r_u32(r_u32(0x800BCE5C) + (index << 2));
    result = r_u32(object) | 0x10000;
    w_u32(object, result);
    return result;
}

uint32 sub_8004e124(uint32 index)
{
    uint32 table, record, object, parameters, attributes, flags, original, value, result;
    sint32 amount;
    FUNCTION_MARKER(0x8004E124, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    record = r_u32(0x800BCEF8) + (index << 6);
    flags = 0x800C6FD0 + (index << 2);
    object = r_u32(table + (index << 2));
    parameters = r_u32(record + 12);
    original = r_u32(flags);
    attributes = r_u32(record + 16);
    w_u32(flags, original | 1);
    if ((r_u8(parameters + 28) & 1) != 0)
        w_u32(flags, original | 0x101);
    if ((r_u8(parameters + 28) & 2) != 0)
        w_u32(flags, r_u32(flags) | 0x200);
    if ((r_u8(parameters + 28) & 4) != 0)
        w_u32(flags, r_u32(flags) | 0x80000);
    if (r_u8(attributes + 7) != 0)
        w_u32(flags, r_u32(flags) | 0x1000);
    if (r_u8(attributes + 8) != 0)
        w_u32(flags, r_u32(flags) | 0x2000);
    if ((r_u8(attributes + 6) & 2) != 0)
        w_u32(flags, r_u32(flags) | 0x4000);
    if ((r_u8(attributes + 6) & 1) != 0)
        w_u32(flags, r_u32(flags) | 0x8000);
    if (r_u8(attributes + 10) == 0)
        w_u32(flags, r_u32(flags) | 16);
    if (r_u8(attributes + 3) != 255)
        w_u32(flags, r_u32(flags) | 0x800000);
    if (r_u8(attributes + 4) == 0)
        w_u32(flags, r_u32(flags) | 0x400);
    value = r_u32(flags);
    if ((value & 0x1000) != 0 && r_s16(object + 72) != 0)
        w_u32(flags, value | 0x800);
    sub_8004eba4(index, attributes);
    sub_8004ecc4(index, attributes);
    value = r_u8(attributes + 9);
    if (value == 1)
        w_u32(object, r_u32(object) | 0x800);
    else if (value == 2)
        w_u32(object, r_u32(object) | 0x1000);
    amount = r_s16(attributes + 12);
    value = r_u16(attributes + 12);
    if (amount != -1)
    {
        result = r_u32(object);
        w_u16(object + 74, value);
        w_u32(object, result | 0x200);
    }
    else
        w_u16(object + 74, 0);
    amount = r_s16(attributes + 14);
    value = r_u16(attributes + 14);
    result = (uint32)amount;
    if (amount != -1)
    {
        result = r_u32(object);
        w_u16(object + 76, value);
        result |= 0x400;
        w_u32(object, result);
    }
    else
        w_u16(object + 76, 0);
    return result;
}

uint32 sub_8004eba4(uint32 index, uint32 attributes)
{
    uint32 state, table, kind, object, value, flags, result;
    FUNCTION_MARKER(0x8004EBA4, "ACE3.BIN");
    state = r_u32(0x800C67A0 + (index << 2));
    if (state == 0)
        return 0xFFFFFFFF;
    table = r_u32(0x800BCE5C);
    kind = r_u8(attributes + 7);
    object = r_u32(table + (index << 2));
    if (kind == 0)
        w_u8(state + 2, 0);
    else if ((r_u8(attributes + 6) & 2) == 0)
        w_u8(state + 2, 3);
    else
    {
        value = r_u8(attributes + 4);
        if (value == 0)
            w_u8(state + 2, 1);
        else if (value == 1)
            w_u8(state + 2, 2);
    }
    if (r_u8(attributes + 5) == 1)
        w_u8(state + 2, r_u8(state + 2) | 0x80);
    w_u8(state + 4, r_u8(attributes + 3));
    w_u8(state + 3, r_u8(object + 72));
    result = r_u32(0x800C6FD0 + (index << 2)) & 0x100;
    if (result != 0)
    {
        w_u8(state + 5, 1);
        return result;
    }
    flags = r_u32(object);
    result = flags & 0x100;
    if ((flags & 0x40) != 0)
    {
        w_u8(state + 5, 3);
        return 3;
    }
    if (result != 0)
    {
        w_u8(state + 5, 2);
        return 2;
    }
    w_u8(state + 5, 0);
    return result;
}

uint32 sub_8004ecc4(uint32 index, uint32 attributes)
{
    uint32 state, value, flags, result;
    FUNCTION_MARKER(0x8004ECC4, "ACE3.BIN");
    state = 0x800C6A60 + index * 12;
    if (r_u8(attributes + 5) == 1)
        w_u8(state, r_u8(state) | 16);
    value = r_u8(attributes + 4);
    if (value == 0)
        w_u8(state + 1, 0);
    else if (value == 1)
        w_u8(state + 1, 1);
    flags = r_u32(0x800C6FD0 + (index << 2));
    if ((flags & 0x100) != 0)
    {
        w_u8(state + 2, 1);
        return 1;
    }
    flags = r_u32(r_u32(r_u32(0x800BCE5C) + (index << 2)));
    result = flags & 0x100;
    if ((flags & 0x40) != 0)
    {
        w_u8(state + 2, 2);
        return 2;
    }
    if (result != 0)
    {
        w_u8(state + 2, 3);
        return 3;
    }
    w_u8(state + 2, 0);
    return result;
}

uint32 sub_8004e620(void)
{
    uint32 selector, mask, index, object, parameters, value;
    FUNCTION_MARKER(0x8004E620, "ACE3.BIN");
    selector = r_u8(0x800B2024 + r_u32(r_u32(0x800BCE54) + 8));
    mask = selector & 0xF0;
    for (index = 0; index < 32; ++index)
    {
        object = r_u32(r_u32(0x800BCE5C) + index * 4 + 128);
        if ((r_u32(object) & 0x4000) == 0)
            continue;
        if (r_s16(object + 78) <= 0)
            continue;
        parameters = r_u32(r_u32(0x800BCEFC) + index * 40 + 8);
        value = r_u8(parameters + 29);
        if (value == selector || value == mask)
            sub_8004e900(index);
        value = r_u8(parameters + 30);
        if (value == selector || value == mask)
            sub_8004e94c(index);
    }
    if (r_s16(r_u32(0x800BCE60) + 908) == 13 && r_u32(r_u32(0x800BCE54) + 8) == 0)
        sub_80053b5c();
    if (r_s16(r_u32(0x800BCE60) + 908) == 21 && r_u32(r_u32(0x800BCE54) + 8) == 1)
        sub_80053b78();
    if (r_s16(r_u32(0x800BCE60) + 908) == 43 && r_u32(r_u32(0x800BCE54) + 8) == 0)
        sub_80053ba0();
    if (r_s16(r_u32(0x800BCE60) + 908) == 55 && r_u32(r_u32(0x800BCE54) + 8) == 0)
        sub_800539d0();
    return sub_80051824();
}

uint32 sub_80030464(void)
{
    FUNCTION_MARKER(0x80030464, "ACE3.BIN");
    sub_80030178();
    sub_8002f500(2);
    return 0;
}

uint32 sub_8002e834(uint32 state)
{
    FUNCTION_MARKER(0x8002E834, "ACE3.BIN");
    w_u32(state + 4, 0);
    w_u32(state, 0x100000);
    w_u16(state + 8, 0);
    w_u16(state + 10, 0);
    return 0x100000;
}

uint32 sub_8002f500(sint32 count)
{
    uint32 index = 0, record;
    FUNCTION_MARKER(0x8002F500, "ACE3.BIN");
    w_u32(0x800C6610, count);
    if (count > 0)
    {
        do
        {
            w_u8(0x800C6614 + index, 6 + index * 3);
            w_u8(0x800C6618 + index, 7 + index * 3);
            w_u8(0x800C661C + index, 8 + index * 3);
            record = 0x800C6630 + index * 92;
            w_u32(record, 0);
            w_u32(record + 8, 0xFFFFFFFF);
            sub_8002e834(record + 36);
            w_u8(record + 48, 0);
            w_u16(record + 90, 0);
            w_u16(record + 88, 0);
            w_u16(record + 86, 0);
            w_u16(record + 84, 0);
            w_u16(record + 82, 0);
            w_u16(record + 80, 0);
            ++index;
        } while ((sint32)index < count);
    }
    while (index < 4)
    {
        w_u8(0x800C6614 + index, 255);
        w_u8(0x800C6618 + index, 255);
        w_u8(0x800C661C + index, 255);
        ++index;
    }
    return 0;
}

uint32 sub_80083f40(uint32 x, uint32 y, uint32 z)
{
    uint32 a, b, c, sum, leading, value, shift;
    FUNCTION_MARKER(0x80083F40, "ACE3.BIN");
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0xA00428);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    sum = a + b + c;
    xport_gte_write_data(30, sum);
    if ((sint32)sum <= 0)
        return 0;
    leading = xport_gte_read_data(31) & 0xFE;
    if ((sint32)leading < 24)
        value = (uint32)((sint32)sum >> ((24 - leading) & 31));
    else
        value = sum << ((leading - 24) & 31);
    value = r_s16(0x800B6DD4 + ((value - 64) << 1));
    shift = (uint32)((sint32)(31 - leading) >> 1);
    value <<= shift & 31;
    return (uint32)((sint32)value >> 12);
}

uint32 sub_80083d88(uint32 value)
{
    uint32 leading, shift;
    FUNCTION_MARKER(0x80083D88, "ACE3.BIN");
    xport_gte_write_data(30, value);
    if ((sint32)value <= 0)
        return 0;
    leading = xport_gte_read_data(31) & 0x1E;
    if ((sint32)leading < 24)
        value = (uint32)((sint32)value >> ((24 - leading) & 31));
    else
        value <<= (leading - 24) & 31;
    value = r_s16(0x800B6DD4 + ((value - 64) << 1));
    shift = (uint32)((sint32)(31 - leading) >> 1);
    value <<= shift & 31;
    return (uint32)((sint32)value >> 12);
}

uint32 sub_80030eb8(uint32 value)
{
    uint32 magnitude, result;
    FUNCTION_MARKER(0x80030EB8, "ACE3.BIN");
    magnitude = (sint32)value < 0 ? 0 - value : value;
    if ((sint32)magnitude >= 0x4001)
        magnitude = 0x4000;
    result = r_s16(0x8001B4B4 + (magnitude << 1));
    if ((sint32)value < 0)
        result = 0 - result;
    return result;
}

uint32 sub_800293f0(sint32 value)
{
    uint32 state, result;
    sint32 scaled;
    FUNCTION_MARKER(0x800293F0, "ACE3.BIN");
    if (value < 32)
        return 1;
    state = sub_8002939c(5);
    if (state == 0)
        return 1;
    w_u8(state, 1);
    scaled = value >> 5;
    result = scaled < 109;
    if (scaled >= 109)
        scaled = 105;
    w_u8(state + 2, scaled);
    return result;
}

uint32 sub_8006e908(uint32 value, uint32 maximum, uint32 middle, uint32 offset, uint32 speed, uint32 positive, uint32 negative)
{
    uint32 numerator, denominator, factor, product, coefficient, intermediate, high;
    sint64 wide;
    FUNCTION_MARKER(0x8006E908, "ACE3.BIN");
    if ((sint32)value < 0)
        value = 0;
    if ((sint32)maximum < (sint32)value)
        value = maximum;
    if ((sint32)middle < (sint32)value)
    {
        numerator = (value - middle) << 12;
        denominator = maximum - middle;
        if (denominator == 0)
            xport_mips_break(7);
        if (denominator == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        factor = (uint32)((sint64)(sint32)numerator / (sint32)denominator);
        factor = (uint32)((sint32)factor >> 1) + 2048;
    }
    else
    {
        numerator = value << 12;
        if (middle == 0)
            xport_mips_break(7);
        if (middle == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        factor = (uint32)((sint64)(sint32)numerator / (sint32)middle);
        factor = (uint32)((sint32)factor >> 1);
    }
    if ((sint32)factor > 4096)
        factor = 4096;
    if ((sint32)factor < -4096)
        factor = (uint32)-4096;
    intermediate = offset + 4096;
    factor = (uint32)((sint32)intermediate >> 1) - factor;
    product = ((sint32)factor > 0 ? positive : negative) * factor;
    if ((sint32)product < 0)
        product += 4095;
    factor = (uint32)((sint32)product >> 12);
    speed = (uint32)((sint32)speed >> 12);
    if ((sint32)speed < -54822)
        speed = (uint32)-54822;
    intermediate = (speed << 4) - speed;
    intermediate = (intermediate << 4) - speed;
    coefficient = (intermediate << 2) - intermediate;
    coefficient = (coefficient << 2) - speed;
    wide = (sint64)(sint32)coefficient * 0x4C81F747;
    high = (uint32)((uint64)wide >> 32);
    intermediate = (uint32)((sint32)coefficient >> 31) - (uint32)((sint32)high >> 14);
    product = factor * (4096 - intermediate);
    if ((sint32)product < 0)
        product += 4095;
    return (uint32)((sint32)product >> 12);
}

uint32 sub_80033ef0(void)
{
    uint32 scene, game, flags, context, packet, result, value, requested;
    FUNCTION_MARKER(0x80033EF0, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    w_u32(scene, r_u32(scene) | 2);
    sub_800339e0(sub_80032f3c, 0);
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    if ((flags & 0x20000000) != 0)
    {
        sub_80034718();
        game = r_u32(0x800BCE60);
        scene = r_u32(0x800BCE54);
        context = r_u32(game + 12);
        packet = r_u32(scene + 20);
        result = sub_80034930(context, packet);
        scene = r_u32(0x800BCE54);
        game = r_u32(0x800BCE60);
        w_u32(scene + 20, result);
        if ((r_u32(game) & 0x8000000) != 0)
            sub_8002c6f4(0);
    }
    else if ((flags & 0x10000000) == 0)
    {
        value = r_u16(game + 342);
        requested = 0;
        if ((value & 1) != 0)
        {
            w_u16(game + 342, value & 0xFFFE);
            requested = 1;
        }
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 64) != 0)
            requested = 1;
        if (requested != 0)
        {
            w_u32(game, r_u32(game) | 0x4000000);
            sub_8002c6f4(1);
        }
    }
    return sub_80033dc0();
}

uint32 sub_8006ea88(uint32 value, uint32 unused, uint32 state, uint32 out0, uint32 out1, uint32 out2, uint32 *native_out0, uint32 *native_out1, uint32 *native_out2)
{
    uint32 position, bound, start, base, other, end, center, delta, numerator, q0, q1, q2;
    uint32 product0, product1, product2, fraction, wave, result0, result2, cap, quarter, result1, result;
    FUNCTION_MARKER(0x8006EA88, "ACE3.BIN");
    position = (uint32)((sint32)value >> 12);
    bound = r_u32(state + 44);
    start = r_u32(state + 48);
    base = r_u32(state + 52);
    other = r_u32(state + 56);
    end = r_u32(state + 60);
    center = r_u32(state + 64);
    value = r_u32(state + 68);
    if ((sint32)position > 0)
        position = 0;
    if ((sint32)position < (sint32)bound)
        position = bound;
    numerator = 0 - ((position - bound) << 14);
    if (bound == 0)
        xport_mips_break(7);
    if (bound == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q0 = (uint32)((sint64)(sint32)numerator / (sint32)bound);
    delta = end - base;
    product0 = delta * q0;
    wave = (q0 << 2) + q0;
    numerator = position << 14;
    if (bound == 0)
        xport_mips_break(7);
    if (bound == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q1 = (uint32)((sint64)(sint32)numerator / (sint32)bound);
    wave = (uint32)((sint32)wave >> 2) & 0x7FFE;
    product1 = (uint32)r_s16(0x800104AC + wave) * (uint32)((sint32)delta >> 2);
    fraction = (uint32)((sint32)(start - other) >> 2);
    product2 = (uint32)r_s16(0x800104AC + (q1 & 0x7FFE)) * fraction;
    fraction *= q1;
    result2 = (uint32)((sint32)product0 >> 14) + base + (uint32)((sint32)product1 >> 13);
    result0 = (uint32)((sint32)fraction >> 12) + other - (uint32)((sint32)product2 >> 14);
    cap = result0 + (center - other);
    if ((sint32)cap < (sint32)result2)
        result2 = cap;
    quarter = (uint32)((sint32)bound >> 2);
    numerator = (position - (bound - quarter)) << 12;
    if (quarter == 0)
        xport_mips_break(7);
    if (quarter == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    q2 = (uint32)((sint64)(sint32)numerator / (sint32)quarter);
    delta = (uint32)((sint32)(start + base) >> 1) - value;
    product0 = (uint32)((sint32)delta >> 2) * q1;
    if (native_out0)
        *native_out0 = result0;
    else
        w_u32(out0, result0);
    if (native_out2)
        *native_out2 = result2;
    else
        w_u32(out2, result2);
    result1 = (uint32)((sint32)product0 >> 12) + value;
    if (native_out1)
        *native_out1 = result1;
    else
        w_u32(out1, result1);
    result = 4096 - q2;
    if ((sint32)result > 4096)
        result = 4096;
    if ((sint32)result < 0)
        result = 0;
    return result;
}

uint32 sub_8003a444(void)
{
    uint32 selected, value;
    FUNCTION_MARKER(0x8003A444, "ACE3.BIN");
    selected = r_u8(0x800C6D73);
    if (selected == 255)
    {
        value = r_u8(0x800C6D6E);
        if (value != selected)
            selected = value;
        else if ((r_u32(r_u32(0x800BCE60) + 300) & 0x2000) != 0)
            selected = sub_8003a4e8();
    }
    if ((r_u32(r_u32(0x800BCE60) + 296) & 0x2000) == 0)
        selected = r_u8(0x800C6D6E);
    w_u8(0x800C6D73, selected);
    return selected == 255 ? 0xFFFFFFFF : selected;
}

uint32 sub_8003a598(void)
{
    uint32 selected;
    FUNCTION_MARKER(0x8003A598, "ACE3.BIN");
    selected = r_u8(0x800C6D72);
    return selected == 255 ? 0xFFFFFFFF : selected;
}

uint32 sub_8003a5c0(void)
{
    uint32 selected, object;
    FUNCTION_MARKER(0x8003A5C0, "ACE3.BIN");
    selected = r_u8(0x800C6D6E);
    if (selected == 255)
        return 0;
    object = r_u32(r_u32(0x800BCE5C) + (selected << 2));
    if (r_s16(object + 72) == 81)
        return 0;
    return (r_u32(0x800C6FD0 + (selected << 2)) & 0x400) == 0;
}

uint32 sub_8003a23c(uint32 record, uint32 *native_record)
{
    uint32 value, result;
    FUNCTION_MARKER(0x8003A23C, "ACE3.BIN");
    sub_8003aa60();
    sub_8003adb4();
    value = sub_8003a444();
    if (native_record)
        native_record[4] = value;
    else
        w_u32(record + 16, value);
    value = sub_8003a598();
    if (native_record)
        native_record[5] = value;
    else
        w_u32(record + 20, value);
    value = native_record ? native_record[1] : r_u32(record + 4);
    w_u32(0x800C6EC0, value);
    result = sub_8003a5c0();
    if (result == 0)
        value = native_record ? native_record[2] : r_u32(record + 8);
    else
        value = native_record ? native_record[3] : r_u32(record + 12);
    w_u32(0x800C6EC4, value);
    return value;
}

uint32 sub_80032f3c(void)
{
    uint32 record[6], result, object, state, scene, value;
    FUNCTION_MARKER(0x80032F3C, "ACE3.BIN");
    result = ~r_u32(r_u32(0x800BCE60)) & 0x20000000;
    if (result == 0)
        return result;
    object = r_u32(r_u32(0x800BCE54) + 40);
    sub_800729dc();
    sub_80045898();
    sub_80038a3c();
    sub_800517f4();
    sub_8004658c();
    sub_8006c5b4();
    sub_8007dac8();
    sub_800850d8();
    state = r_u32(0x800BD098);
    record[0] = r_u32(state + 28);
    record[1] = r_u32(state + 8);
    record[2] = r_u32(state + 12);
    record[3] = r_u32(state + 16);
    sub_8003a23c(0, record);
    state = r_u32(0x800BD098);
    value = r_u32(state + 24);
    if (value != record[5] && (r_u32(r_u32(0x800BCE54)) & 2) != 0)
        sub_8007e890(record[5]);
    state = r_u32(0x800BD098);
    scene = r_u32(0x800BCE54);
    w_u32(object + 188, record[4]);
    w_u32(state + 24, record[5]);
    result = r_u32(scene) & 2;
    if (result != 0)
        return sub_8007f0f8();
    return result;
}

uint32 sub_800339e0(uint32 (*callback)(void), void (*void_callback)(void))
{
    uint32 game, scene, selected, head, packet, table, tag, value, other, width, height, x, y;
    uint32 offset_x, offset_y, center_x, center_y;
    FUNCTION_MARKER(0x800339E0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    selected = r_u32(game + 4);
    scene = r_u32(0x800BCE54);
    table = scene + (selected << 2);
    head = r_u32(table + 24);
    w_u32(scene + 16, head);
    packet = r_u32(table + 32);
    w_u32(scene + 20, packet);
    ClearOTagR(psx_addr(head, 16384), 4096);
    scene = r_u32(0x800BCE54);
    game = r_u32(0x800BCE60);
    head = r_u32(scene + 16);
    table = r_u32(game + 8);
    tag = r_u32(head);
    value = r_u32(table);
    w_u32(head, (tag & 0xFF000000) | (value & 0xFFFFFF));
    value = r_u32(scene + 16);
    table = r_u32(game + 8);
    tag = r_u32(table);
    w_u32(table, (tag & 0xFF000000) | ((value + 0x3FFC) & 0xFFFFFF));
    w_u16(scene + 652, 0);
    w_u16(scene + 654, 0);
    w_u16(scene + 656, 0);
    w_u16(scene + 658, 0);
    if (callback)
        callback();
    else
        void_callback();
    sub_8007b438(r_u32(r_u32(0x800BCE54) + 40));
    scene = r_u32(0x800BCE54);
    sub_800332e4(r_u32(scene + 40), scene + 44, scene + 76);
    game = r_u32(0x800BCE60);
    selected = r_u32(game + 4);
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE58);
    table = r_u32(table + selected * 4116);
    packet = r_u32(scene + 20);
    x = r_s16(table + 8);
    y = r_s16(table + 10);
    width = r_u16(table + 4);
    height = r_u16(table + 6);
    offset_x = r_u16(scene + 652);
    offset_y = r_u16(scene + 654);
    w_u16(scene + 660, offset_x + x);
    center_x = x + (uint32)((sint32)(width << 16) >> 17);
    w_u16(scene + 662, offset_y + y);
    value = r_u16(scene + 656);
    center_x += value;
    other = r_u16(scene + 658);
    center_y = y + (uint32)((sint32)(height << 16) >> 17) + other;
    w_u16(scene + 666, center_y);
    value = r_s16(scene + 656);
    width = (uint32)(sint32)(sint16)width;
    w_u16(scene + 664, center_x);
    if (value != 0)
    {
        w_u8(packet + 3, 3);
        w_u8(packet + 7, 96);
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
        scene = r_u32(0x800BCE54);
        w_u16(packet + 8, 0);
        w_u16(packet + 10, 0);
        value = r_u16(scene + 656);
        w_u16(packet + 14, 240);
        w_u16(packet + 12, value);
        head = r_u32(scene + 16);
        tag = r_u32(packet);
        value = r_u32(head);
        w_u32(packet, (tag & 0xFF000000) | (value & 0xFFFFFF));
        head = r_u32(scene + 16);
        tag = r_u32(head);
        w_u32(head, (tag & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 16;
    }
    scene = r_u32(0x800BCE54);
    if (r_s16(scene + 658) != 0)
    {
        w_u8(packet + 3, 3);
        w_u8(packet + 7, 96);
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
        scene = r_u32(0x800BCE54);
        w_u16(packet + 8, 0);
        w_u16(packet + 10, 0);
        w_u16(packet + 12, width);
        w_u16(packet + 14, r_u16(scene + 658));
        head = r_u32(scene + 16);
        tag = r_u32(packet);
        value = r_u32(head);
        w_u32(packet, (tag & 0xFF000000) | (value & 0xFFFFFF));
        head = r_u32(scene + 16);
        tag = r_u32(head);
        w_u32(head, (tag & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 16;
    }
    value = (y << 11) | x | 0xE5000000;
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, packet);
    w_u32(packet + 4, value);
    w_u32(packet + 8, 0);
    value = r_u32(r_u32(scene + 16));
    w_u32(packet, value | 0x2000000);
    head = r_u32(scene + 16);
    w_u32(head, packet & 0xFFFFFF);
    scene = r_u32(0x800BCE54);
    x = r_s16(scene + 660);
    y = r_s16(scene + 662);
    other = packet + 12;
    w_u32(scene + 20, other);
    w_u32(other + 8, 0);
    w_u32(other + 4, (y << 11) | x | 0xE5000000);
    value = r_u32(r_u32(scene + 16) + 4);
    w_u32(packet + 12, value | 0x2000000);
    head = r_u32(scene + 16);
    w_u32(head + 4, other & 0xFFFFFF);
    w_u32(scene + 20, packet + 24);
    xport_gte_write_control(24, 0);
    xport_gte_write_control(25, 0);
    sub_80033324();
    sub_800333d8();
    sub_8003342c();
    sub_8003346c();
    scene = r_u32(0x800BCE54);
    packet = r_u32(scene + 20);
    y = r_s16(scene + 666);
    x = r_s16(scene + 664);
    w_u32(packet + 4, (y << 11) | x | 0xE5000000);
    w_u32(packet + 8, 0);
    value = r_u32(r_u32(scene + 16) + 0x3FFC);
    w_u32(packet, value | 0x2000000);
    head = r_u32(scene + 16);
    w_u32(head + 0x3FFC, packet & 0xFFFFFF);
    w_u32(scene + 20, packet + 12);
    xport_gte_write_control(24, 160 << 16);
    xport_gte_write_control(25, 120 << 16);
    sub_8003a2b8();
    sub_800337bc();
    sub_800337dc();
    value = ~r_u32(r_u32(0x800BCE54)) & 1;
    if (value != 0)
        return sub_80033814();
    return value;
}

uint32 sub_8003a4e8(void)
{
    uint32 table, index, object, state, value, best = 0x1000000, selected = 255;
    FUNCTION_MARKER(0x8003A4E8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    for (index = 2; index < 32; ++index)
    {
        object = r_u32(table + index * 4);
        if ((~r_u32(object) & 0xC000) != 0)
            continue;
        if ((r_u32(0x800C6FD0 + index * 4) & 0x800000) == 0)
            continue;
        state = r_u32(0x800C6ED0 + index * 4);
        if (state == 0)
            continue;
        value = r_u32(state + 4);
        if ((sint32)value < (sint32)best)
        {
            best = value;
            selected = index;
        }
    }
    return selected;
}

uint32 sub_80031bb4(uint32 vector, uint32 out, const uint32 *native_vector)
{
    uint32 x, z, y, magnitude, a, b, distance, shift, pitch, yaw, result;
    FUNCTION_MARKER(0x80031BB4, "ACE3.BIN");
    if (native_vector != 0)
    {
        x = native_vector[0];
        z = native_vector[2];
        y = native_vector[1];
    }
    else
    {
        x = r_u32(vector);
        z = r_u32(vector + 8);
        y = r_u32(vector + 4);
    }
    a = (sint32)x < 0 ? 0 - x : x;
    b = (sint32)z < 0 ? 0 - z : z;
    magnitude = a + b;
    if ((sint32)magnitude > 0xAFFFF)
        distance = 0x100000;
    else
    {
        a = x;
        b = z;
        shift = 0;
        if ((sint32)magnitude > 0xB000)
        {
            a = (uint32)((sint32)x >> 4);
            b = (uint32)((sint32)z >> 4);
            shift = 4;
        }
        distance = sub_80083f40(a, 0, b) << shift;
    }
    pitch = sub_80030d54(0 - y, distance);
    yaw = sub_80030d54(x, z);
    if ((sint32)pitch > 4096)
    {
        pitch = 0x2000 - pitch;
        yaw += 0x2000;
    }
    if ((sint32)pitch < -4096)
    {
        pitch = (uint32)-8192 - pitch;
        yaw += 0x2000;
    }
    if ((sint32)yaw >= 0x2000)
        yaw -= 0x4000;
    result = (sint32)yaw < -8192;
    if (result != 0)
        yaw += 0x4000;
    w_u16(out, pitch);
    w_u16(out + 2, yaw);
    w_u16(out + 4, 0);
    return result;
}

uint32 sub_80072114(void)
{
    uint32 context, state, angles, value;
    FUNCTION_MARKER(0x80072114, "ACE3.BIN");
    context = r_u32(0x800BD098);
    state = r_u32(0x800BD094);
    angles = r_u32(0x800BD08C);
    value = r_u32(context + 44);
    w_u32(0x800C7BEC, value);
    value = r_u32(state + 72);
    w_u32(0x800C7BE8, value);
    value = r_u16(angles + 14);
    w_u16(0x800C7BF0, value);
    value = r_u16(state + 48);
    w_u16(0x800C7BF4, value);
    value = r_u16(state + 52);
    w_u16(0x800C7BF2, value);
    return sub_800301f0(0x800C7BE8);
}

uint32 sub_80070a34(uint32 object)
{
    uint32 value, x, vx, vy, vz, z;
    FUNCTION_MARKER(0x80070A34, "ACE3.BIN");
    value = r_u32(object + 84);
    if ((sint32)value < 0)
    {
        x = r_u32(object + 8);
        vx = r_u32(object + 56);
        vy = r_u32(object + 60);
        vz = r_u32(object + 64);
        w_u32(object + 8, x + vx);
        value = r_u32(object + 12);
        z = r_u32(object + 16);
        value += vy;
        z += vz;
        w_u32(object + 12, value);
        w_u32(object + 16, z);
    }
    return value;
}

uint32 sub_8007084c(uint32 object, uint32 state)
{
    MATRIX first = {0}, second = {0};
    uint32 vector0, vector1, a, b, c, d, e, packed, depth, x, y, z, result;
    uint32 column0[3], column1[3];
    FUNCTION_MARKER(0x8007084C, "ACE3.BIN");
    result = ~r_u32(object) & 0xC000;
    if (result != 0)
        return result;
    if ((r_u32(object + 84) & 0x400000) != 0)
        sub_80030f64(object + 24, object + 48, NULL);
    if ((r_u32(object + 84) & 0x100000) != 0)
    {
        sub_80030f64(state + 64, object + 48, NULL);
        if ((r_u32(object + 84) & 0x100000) != 0)
            sub_800313a8(object + 24, object + 48, state + 64, 0);
    }
    if ((r_u32(object + 84) & 0x20000) != 0)
        sub_80030ef8(object + 32, object + 24, object + 56);
    if ((r_u32(object + 84) & 0x10000) != 0)
        sub_80030ef8(object + 32, state + 64, object + 56);
    if ((r_u32(object + 84) & 0xF80000) != 0)
    {
        vector0 = r_u32(0x800BCE0C);
        vector1 = r_u32(0x800BCE10);
        sub_800308f0(object + 24, 0, &first, 0);
        rotation_build(state + 64, 0, &second, 0);
        a = xport_load_le32((uint8 *)&first);
        b = xport_load_le32((uint8 *)&first + 4);
        c = xport_load_le32((uint8 *)&first + 8);
        d = xport_load_le32((uint8 *)&first + 12);
        e = xport_load_le32((uint8 *)&first + 16);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        xport_gte_write_control(2, c);
        xport_gte_write_control(3, d);
        xport_gte_write_control(4, e);
        packed = xport_load_le16((uint8 *)&second) | (xport_load_le32((uint8 *)&second + 4) & 0xFFFF0000);
        depth = xport_load_le32((uint8 *)&second + 12);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        packed = xport_load_le16((uint8 *)&second + 2) | (xport_load_le32((uint8 *)&second + 8) << 16);
        depth = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&second + 14);
        column0[0] = xport_gte_read_data(9);
        column0[1] = xport_gte_read_data(10);
        column0[2] = xport_gte_read_data(11);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        packed = xport_load_le16((uint8 *)&second + 4) | (xport_load_le32((uint8 *)&second + 8) & 0xFFFF0000);
        depth = xport_load_le32((uint8 *)&second + 16);
        column1[0] = xport_gte_read_data(9);
        column1[1] = xport_gte_read_data(10);
        column1[2] = xport_gte_read_data(11);
        xport_gte_write_data(0, packed);
        xport_gte_write_data(1, depth);
        xport_gte_execute(0x486012);
        xport_store_le32((uint8 *)&first, (column0[0] & 0xFFFF) | (column1[0] << 16));
        xport_store_le32((uint8 *)&first + 12, (column0[2] & 0xFFFF) | (column1[2] << 16));
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        xport_store_le32((uint8 *)&first + 4, (a & 0xFFFF) | (column0[1] << 16));
        xport_store_le32((uint8 *)&first + 8, (column1[1] & 0xFFFF) | (b << 16));
        xport_store_le32((uint8 *)&first + 16, xport_gte_read_data(11));
        a = xport_load_le32((uint8 *)&first);
        b = xport_load_le32((uint8 *)&first + 4);
        xport_gte_write_control(0, a);
        xport_gte_write_control(1, b);
        c = xport_load_le32((uint8 *)&first + 8);
        d = xport_load_le32((uint8 *)&first + 12);
        e = xport_load_le32((uint8 *)&first + 16);
        xport_gte_write_control(2, c);
        xport_gte_write_control(3, d);
        xport_gte_write_control(4, e);
        xport_gte_write_data(0, vector0);
        xport_gte_write_data(1, vector1);
        xport_gte_execute(0x486012);
        x = xport_gte_read_data(25);
        y = xport_gte_read_data(26);
        z = xport_gte_read_data(27);
        result = sub_80030d54(0 - y, z);
        w_u16(state + 56, result);
        result = sub_80030d54(x, z);
        w_u16(state + 58, result);
        w_u16(state + 60, 0);
    }
    result = r_u32(object + 84) & 0xFF000000;
    w_u32(object + 84, result);
    return result;
}

uint32 sub_80070a80(uint32 object, uint32 state)
{
    uint32 vx, vy, vz, x, y, z, a, b, c, speed, square, threshold;
    uint32 index, target, flags, radius, dx, dy, dz, sum, result;
    FUNCTION_MARKER(0x80070A80, "ACE3.BIN");
    vx = r_u32(object + 56);
    vy = r_u32(object + 60);
    vz = r_u32(object + 64);
    a = (sint32)vx < 0 ? 0 - vx : vx;
    b = (sint32)vy < 0 ? 0 - vy : vy;
    c = (sint32)vz < 0 ? 0 - vz : vz;
    speed = (uint32)((sint32)(a + b + c) >> 12);
    square = speed * speed;
    threshold = (uint32)((sint64)(sint32)square * 0x38E38E39LL >> 32);
    threshold = (uint32)((sint32)threshold >> 1) - (uint32)((sint32)square >> 31);
    y = (uint32)((sint32)(r_u32(object + 12) + vy) >> 12);
    x = (uint32)((sint32)(r_u32(object + 8) + vx) >> 12);
    z = (uint32)((sint32)(r_u32(object + 16) + vz) >> 12);
    for (index = 0; index < 64; ++index)
    {
        target = r_u32(r_u32(0x800BCE5C) + index * 4);
        flags = r_u32(target);
        if ((flags & 0x4400) != 0x4400)
            continue;
        radius = r_s16(target + 76);
        a = r_u32(target + 8);
        b = r_u32(target + 12);
        dx = (uint32)((sint32)a >> 12) - x;
        dy = (uint32)((sint32)b >> 12) - y;
        a = (sint32)dx < 0 ? 0 - dx : dx;
        dz = (uint32)(r_s32(target + 16) >> 12) - z;
        b = (sint32)dy < 0 ? 0 - dy : dy;
        c = (sint32)dz < 0 ? 0 - dz : dz;
        if ((sint32)(radius * 3 + speed) < (sint32)(a + b + c))
            continue;
        sum = dx * dx + dy * dy + dz * dz;
        if ((sint32)(radius * radius + threshold) < (sint32)sum)
            continue;
        if ((flags & 0x1020) == 0x1020 && (~r_u8(target + 5) & 2) != 0)
        {
            result = sub_8009b4ec(object + 8, object + 56, target, state + 28, 0x1F800000, 0);
            if ((sint32)result > 0x7FFFFFFE)
                continue;
            a = r_u32(state + 16);
            b = r_u32(0x800BD0C4);
            w_u32(state + 44, b);
            w_u32(state + 16, a | 0x20);
        }
        else
        {
            a = r_u32(state + 16);
            w_u32(state + 16, a | 0x20);
            a = r_u32(object + 8);
            w_u32(state + 28, a);
            a = r_u32(object + 12);
            w_u32(state + 32, a);
            a = r_u32(object + 16);
            w_u32(state + 44, 0);
            w_u32(state + 36, a);
        }
    }
    return 0;
}

uint32 sub_80030bdc(uint32 angles, uint32 out, MATRIX *native_out, const uint16 *native_angles)
{
    uint32 x, y, z, cx, cy, sy, sz, sx, cz, p, q, r, a, b, c, d, e, f, g, h, value, result;
    FUNCTION_MARKER(0x80030BDC, "ACE3.BIN");
    x = native_angles ? native_angles[0] : r_u16(angles);
    y = native_angles ? native_angles[1] : r_u16(angles + 2);
    z = native_angles ? native_angles[2] : r_u16(angles + 4);
    x &= 0x3FFF;
    y &= 0x3FFF;
    z &= 0x3FFF;
    cx = r_s16(0x800104AC + (x + 0x1000) * 2);
    cy = r_s16(0x800104AC + (y + 0x1000) * 2);
    sy = r_s16(0x800104AC + y * 2);
    sz = r_s16(0x800104AC + z * 2);
    xport_gte_write_data(8, cx);
    xport_gte_write_data(9, sz);
    xport_gte_write_data(10, sy);
    xport_gte_write_data(11, cy);
    sx = r_s16(0x800104AC + x * 2);
    xport_gte_execute(0x190003D);
    cz = r_s16(0x800104AC + (z + 0x1000) * 2);
    p = xport_gte_read_data(25);
    q = xport_gte_read_data(26);
    r = xport_gte_read_data(27);
    xport_gte_write_data(8, cz);
    xport_gte_write_data(9, cx);
    xport_gte_write_data(10, cy);
    xport_gte_write_data(11, sy);
    value = (uint32)((sint32)(0 - p) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 2, value);
    else
        w_u16(out + 2, value);
    xport_gte_execute(0x190003D);
    value = (uint32)((sint32)(0 - q) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 12, value);
    else
        w_u16(out + 12, value);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 14, sx);
    else
        w_u16(out + 14, sx);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_data(8, sz);
    xport_gte_write_data(9, cy);
    xport_gte_write_data(10, sy);
    value = (uint32)((sint32)r >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 16, value);
    else
        w_u16(out + 16, value);
    xport_gte_execute(0x190003D);
    value = (uint32)((sint32)a >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 8, value);
    else
        w_u16(out + 8, value);
    d = xport_gte_read_data(26);
    e = xport_gte_read_data(25);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)c >> 14);
    xport_gte_write_data(10, (sint32)b >> 14);
    xport_gte_write_data(11, (sint32)d >> 14);
    xport_gte_execute(0x190003D);
    f = xport_gte_read_data(25);
    g = xport_gte_read_data(26);
    h = xport_gte_read_data(27);
    xport_gte_write_data(8, sx);
    xport_gte_write_data(9, (sint32)e >> 14);
    value = (uint32)((sint32)(b - h) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out, value);
    else
        w_u16(out, value);
    xport_gte_execute(0x190003D);
    value = (uint32)((sint32)(e + f) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 6, value);
    else
        w_u16(out + 6, value);
    value = (uint32)((sint32)(d - g) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 10, value);
    else
        w_u16(out + 10, value);
    result = xport_gte_read_data(25);
    value = (uint32)((sint32)(c + result) >> 14);
    if (native_out)
        xport_store_le16((uint8 *)native_out + 4, value);
    else
        w_u16(out + 4, value);
    return result;
}

uint32 sub_800301f0(uint32 record)
{
    uint32 result, next, descriptor, value, left, right, voice, offset, magnitude;
    FUNCTION_MARKER(0x800301F0, "ACE3.BIN");
    if (r_u8(0x800BCEA0) == 0)
    {
        result = sub_8002db4c(r_u8(0x800BCEA4));
        next = r_u8(0x800BCEA5);
        w_u8(0x800BCEA1, result);
        result = sub_8002db4c(next);
        next = r_u8(0x800BCEA6);
        w_u8(0x800BCEA2, result);
        result = sub_8002db4c(next);
        value = r_u8(0x800BCEA0);
        w_u8(0x800BCEA3, result);
        w_u8(0x800BCEA0, value + 1);
    }
    if (r_u8(0x800BCEA3) != 255)
    {
        descriptor = 0x800C5A10 + r_u8(0x800BCEA6) * 20;
        value = r_u16(record + 8);
        result = sub_8002e64c(r_s16(descriptor + 8), (uint32)((sint32)(value << 16) >> 20));
        sound_set_voice_note(r_u8(0x800BCEA3), result & 0xFFFF);
        voice = r_u8(0x800BCEA3);
        value = r_u16(record + 8);
        descriptor = 0x800C5A10 + r_u8(0x800BCEA6) * 20;
        offset = (uint32)((sint32)(value << 16) >> 18);
        left = r_u16(descriptor + 4);
        right = r_u16(descriptor + 6);
        sub_8002dec8(voice, (uint32)(sint32)(sint16)(left + offset), (uint32)(sint32)(sint16)(right + offset));
    }
    if (r_u8(0x800BCEA2) != 255)
    {
        descriptor = 0x800C5A10 + r_u8(0x800BCEA5) * 20;
        value = r_u32(record);
        offset = (uint32)((sint32)((value + 0xFFFDC000) << 9) >> 16);
        result = sub_8002e64c(r_s16(descriptor + 8), offset);
        sound_set_voice_note(r_u8(0x800BCEA2), result & 0xFFFF);
        descriptor = 0x800C5A10 + r_u8(0x800BCEA5) * 20;
        voice = r_u8(0x800BCEA2);
        value = r_s16(record + 10);
        left = r_u16(descriptor + 4);
        right = r_u16(descriptor + 6);
        offset = value << 2;
        sub_8002dec8(voice, (uint32)(sint32)(sint16)(left - offset), (uint32)(sint32)(sint16)(right + offset));
    }
    result = r_u8(0x800BCEA1);
    if (result == 255)
        return result;
    descriptor = 0x800C5A10 + r_u8(0x800BCEA4) * 20;
    value = r_s16(record + 12);
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = sub_8002e64c(r_s16(descriptor + 8), (uint32)((sint32)(magnitude << 18) >> 16));
    sound_set_voice_note(r_u8(0x800BCEA1), result & 0xFFFF);
    value = r_s16(record + 12);
    next = r_u32(record + 4);
    magnitude = (sint32)value < 0 ? 0 - value : value;
    next -= 0x200000;
    if ((sint32)next < 0)
        next = 0;
    if ((sint32)next > 0x400000)
        next = 0x400000;
    offset = (magnitude << 3) + 4096 - (uint32)((sint32)next >> 10);
    descriptor = 0x800C5A10 + r_u8(0x800BCEA4) * 20;
    voice = r_u8(0x800BCEA1);
    left = r_u16(descriptor + 4);
    right = r_u16(descriptor + 6);
    return sub_8002dec8(voice, (uint32)(sint32)(sint16)(left + offset), (uint32)(sint32)(sint16)(right + offset));
}

uint32 sub_8002e64c(uint32 note, uint32 offset)
{
    uint32 value;
    FUNCTION_MARKER(0x8002E64C, "ACE3.BIN");
    offset += ((note & 0xFF00) >> 1) | (note & 0x7F);
    value = ((offset & 0x7F80) << 1) | (offset & 0x7F);
    return (uint32)(sint32)(sint16)value;
}

uint32 sub_8002dec8(uint32 voice, uint32 left, uint32 right)
{
    uint32 target, flags;
    FUNCTION_MARKER(0x8002DEC8, "ACE3.BIN");
    if (voice >= 24)
        return 255;
    if ((sint16)left >= 0x4000)
        left = 0x3FFF;
    if ((sint16)right >= 0x4000)
        right = 0x3FFF;
    target = 0x800C5830 + voice * 20;
    flags = r_u8(target + 1);
    w_u16(target + 4, left);
    w_u16(target + 6, right);
    w_u8(target + 1, flags | 0x10);
    return voice;
}

uint32 sub_8003048c(void)
{
    FUNCTION_MARKER(0x8003048C, "ACE3.BIN");
    sub_800301a8();
    sub_8002f620();
    sound_stop_all();
    sound_init_slots();
    sound_init_queue();
    return 0;
}

uint32 sub_800301a8(void)
{
    FUNCTION_MARKER(0x800301A8, "ACE3.BIN");
    sub_8002dda8(r_u8(0x800BCEA1));
    sub_8002dda8(r_u8(0x800BCEA2));
    sub_8002dda8(r_u8(0x800BCEA3));
    w_u8(0x800BCEA3, 255);
    w_u8(0x800BCEA2, 255);
    w_u8(0x800BCEA1, 255);
    return 255;
}
