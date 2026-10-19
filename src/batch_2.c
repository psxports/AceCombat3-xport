#include "game_functions.h"
#include <stdlib.h>

/* Draft translations, pending final integration and runtime verification */
uint32 pad_update_port(uint32 packet, uint32 port)
{
    uint32 state;
    uint32 header;
    uint32 old_type;
    uint32 type;
    uint32 first;
    uint32 second;
    uint32 buttons;
    uint32 prev_buttons;
    uint32 result;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x80026BA4, "ACE3.BIN");
    state = r_u32(port + 4);
    header = r_u16(packet);
    old_type = r_u8(state + 12);
    w_u8(state + 13, 0);
    if (!old_type && !(header & 255))
    {
        if (!pad_negotiate(port))
        {
            header = 0;
            w_u8(state + 13, 2);
        }
    }
    else if (r_u8(port + 14))
    {
        if (pad_negotiate(port))
            w_u8(port + 14, 0);
        else
            w_u8(state + 13, 2);
    }
    if (header & 255)
    {
        old_type = r_u8(state + 12);
        result = 1;
        w_u8(state + 13, 1);
        if (old_type)
        {
            result = 8;
            w_u8(state + 13, 8);
        }
        w_u8(port + 1, 0);
        w_u8(state, 0);
        w_u8(state + 1, 0);
        w_u8(state + 7, 0);
        w_u8(state + 6, 0);
        w_u8(state + 5, 0);
        w_u8(state + 4, 0);
        w_u8(state + 3, 0);
        w_u8(state + 2, 0);
        w_u16(state + 8, 0);
        w_u16(state + 10, 0);
        w_u8(state + 12, 0);
        w_u16(state + 16, 0);
        w_u16(state + 14, 0);
    }
    else
    {
        type = header >> 8;
        if (r_u8(state + 12) && r_u16(state + 10))
        {
            PadGetState(r_u8(port));
            result = PadInfoMode(r_u8(port), 2, 0);
            if (result != r_u16(state + 10))
            {
                type = 0;
                w_u8(state + 13, 4);
                w_u8(port + 1, 1);
                w_u8(port + 14, 1);
            }
        }
        w_u8(state + 12, type);
        buttons = r_u16(packet + 2);
        prev_buttons = r_u16(state + 14);
        buttons = ~buttons;
        prev_buttons = ~prev_buttons;
        w_u16(state + 14, buttons);
        w_u16(state + 16, buttons & prev_buttons);
        if (type == 0x23 || type == 0x53 || type == 0x73)
        {
            first = r_u16(packet + 4);
            second = r_u16(packet + 6);
            w_u16(state + 18, first >> 8);
            w_u16(state + 20, first & 255);
            w_u16(state + 22, second >> 8);
            result = second & 255;
            w_u16(state + 24, result);
            return result;
        }
        if (type == 0x12)
        {
            first = r_u16(packet + 4);
            w_u16(state + 18, (sint8)(first >> 8));
            x = r_u16(state + 18);
            y = r_u16(state + 24);
            w_u16(state + 20, (sint8)first);
            result = r_u16(state + 22);
            second = r_u16(state + 20);
            result += x;
            w_u16(state + 22, result);
            w_u16(state + 24, y + second);
            return result;
        }
        if (type == 0x63)
        {
            first = r_u16(packet + 4);
            second = r_u16(packet + 6);
            w_u16(state + 24, 0);
            w_u16(state + 22, 0);
            w_u16(state + 18, first);
            w_u16(state + 20, second);
            return 1;
        }
        if (type == 0xE3)
        {
            first = r_u16(packet + 4);
            second = r_u16(packet + 6);
            x = r_u16(state + 18);
            w_u16(state + 24, 0);
            result = first - x;
            w_u16(state + 22, result);
            w_u16(state + 18, first);
            w_u16(state + 20, second);
            return result;
        }
        if (type == 0x43)
            result = 0x43;
        else if (type < 0x44)
            result = 0x12;
        else if (type < 0x64)
            result = 0x53;
        else
            result = 0xE3;
    }
    w_u16(state + 24, 0);
    w_u16(state + 22, 0);
    w_u16(state + 20, 0);
    w_u16(state + 18, 0);
    return result;
}

uint32 format_emit(uint32 *context, uint32 text, const uint8 *native_text, uint32 flags, uint32 sign, sint32 width, sint32 precision, sint32 length, uint8 *native_destination, uint32 native_capacity)
{
    uint32 total;
    uint32 padded;
    uint32 i;
    uint32 ch;

    FUNCTION_MARKER(0x80027E80, "ACE3.BIN");
    total = ((flags >> 9) & 1) + (uint32)length + (sign != 0);
    if (flags & 0x180)
        total += 2;
    padded = (sint32)total < precision ? (uint32)precision : total;
    if (!(flags & 0x60))
        for (i = padded; (sint32)i < width; ++i)
            format_put_char(' ', context, native_destination, native_capacity);
    if (sign)
        format_put_char(sign, context, native_destination, native_capacity);
    else if (flags & 0x380)
    {
        format_put_char('0', context, native_destination, native_capacity);
        if (flags & 0x80)
            format_put_char('x', context, native_destination, native_capacity);
        else if (flags & 0x100)
            format_put_char('X', context, native_destination, native_capacity);
        else if (flags & 0x400)
            format_put_char('b', context, native_destination, native_capacity);
    }
    if ((flags & 0x60) == 0x40)
        for (i = padded; (sint32)i < width; ++i)
            format_put_char('0', context, native_destination, native_capacity);
    for (i = total; (sint32)i < precision; ++i)
        format_put_char('0', context, native_destination, native_capacity);
    for (i = 0; (sint32)i < length; ++i)
    {
        ch = native_text ? native_text[i] : r_u8(text + i);
        format_put_char(ch, context, native_destination, native_capacity);
    }
    if (flags & 0x20)
        for (i = padded; (sint32)i < width; ++i)
            format_put_char(' ', context, native_destination, native_capacity);
    return (sint32)padded < width ? (uint32)width : padded;
}

uint32 format_core(uint32 format, uint32 arguments, uint32 *context, uint8 *native_destination, const uint8 *native_arguments, uint32 native_argument_bytes, uint32 native_capacity)
{
    uint32 count = 0;
    uint32 native_offset = 0;
    uint32 ch;
    uint32 flags;
    uint32 sign;
    uint32 low;
    uint32 high;
    uint32 radix;
    uint32 digits;
    uint32 text;
    uint32 length;
    uint32 numeric;
    uint32 negative;
    uint32 output;
    uint8 character;
    sint32 width;
    sint32 precision;

    FUNCTION_MARKER(0x80028074, "ACE3.BIN");
    for (;;)
    {
        ch = r_u8(format++);
        while (ch && ch != '%')
        {
            format_put_char(ch, context, native_destination, native_capacity);
            ++count;
            ch = r_u8(format++);
        }
        --format;
        if (!ch)
            return count;
        flags = 0;
        width = 0;
        precision = -1;
        sign = 0;
        for (;;)
        {
            ch = r_u8(++format);
            if (!ch)
                return count;
            if (ch == ' ')
            {
                if (!sign)
                    sign = ' ';
                continue;
            }
            if (ch == '#')
            {
                flags |= 0x10;
                continue;
            }
            if (ch == '+')
            {
                sign = '+';
                continue;
            }
            if (ch == '-')
            {
                flags |= 0x20;
                continue;
            }
            if (ch == '0')
            {
                if (sign != '-' && precision <= 0)
                    flags |= 0x40;
                continue;
            }
            if (ch == 'h' || ch == 'l' || ch == 'L')
            {
                flags |= ch == 'h' ? 1 : (ch == 'l' ? 2 : 12);
                continue;
            }
            if (ch == '.')
            {
                ch = r_u8(++format);
                if (ch == '*')
                {
                    if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                    {
#ifndef NDEBUG
                        fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                        abort();
                    }
                    precision = native_arguments ? (sint32)xport_load_le32(native_arguments + native_offset) : r_s32(arguments);
                    native_offset += 4;
                    arguments += 4;
                }
                else
                {
                    output = 0;
                    while (r_u8(0x800BCA55 + ch) & 4)
                    {
                        output = output * 10 - '0' + ch;
                        ch = r_u8(++format);
                    }
                    --format;
                    precision = (sint32)output;
                }
                continue;
            }
            if (ch == '*' || (ch >= '1' && ch <= '9'))
            {
                if (ch == '*')
                {
                    if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                    {
#ifndef NDEBUG
                        fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                        abort();
                    }
                    width = native_arguments ? (sint32)xport_load_le32(native_arguments + native_offset) : r_s32(arguments);
                    native_offset += 4;
                    arguments += 4;
                    if (width < 0)
                    {
                        width = (sint32)(0 - (uint32)width);
                        flags |= 0x20;
                    }
                }
                else
                {
                    output = 0;
                    do
                    {
                        ch = r_u8(format++);
                        output = output * 10 - '0' + ch;
                    } while (r_u8(0x800BCA55 + r_u8(format)) & 4);
                    --format;
                    width = (sint32)output;
                }
                continue;
            }
            break;
        }
        numeric = ch == 'X' || ch == 'p' || ch == 'x' || ch == 'o' || ch == 'b' || ch == 'u' || ch == 'U' || ch == 'Z' || ch == 'D' || ch == 'I' || ch == 'd' || ch == 'i';
        if (numeric)
        {
            if (ch == 'U' || ch == 'Z' || ch == 'D' || ch == 'I')
                flags |= 2;
            if (ch == 'p' && precision == -1)
                precision = 8;
            high = 0;
            negative = ch == 'D' || ch == 'I' || ch == 'd' || ch == 'i';
            if (flags & 4)
            {
                if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 8))
                {
#ifndef NDEBUG
                    fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                    abort();
                }
                low = native_arguments ? xport_load_le32(native_arguments + native_offset) : r_u32(arguments);
                high = native_arguments ? xport_load_le32(native_arguments + native_offset + 4) : r_u32(arguments + 4);
                native_offset += 8;
                arguments += 8;
            }
            else
            {
                if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
                {
#ifndef NDEBUG
                    fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                    abort();
                }
                if (flags & 1)
                {
                    low = native_arguments ? xport_load_le16(native_arguments + native_offset) : r_u16(arguments);
                    if (negative)
                        low = (uint32)(sint32)(sint16)low;
                }
                else
                    low = native_arguments ? xport_load_le32(native_arguments + native_offset) : r_u32(arguments);
                native_offset += 4;
                arguments += 4;
                if (negative)
                    high = (uint32)((sint32)low >> 31);
            }
            radix = 10;
            digits = 0x80010148;
            if (ch == 'X' || ch == 'x' || ch == 'p')
            {
                radix = 16;
                if (ch == 'X')
                    digits = 0x80010134;
                if ((flags & 0x10) && (low | high))
                    flags |= ch == 'X' ? 0x100 : 0x80;
            }
            else if (ch == 'o')
            {
                radix = 8;
                if ((flags & 0x10) && (low | high))
                    flags |= 0x200;
            }
            else if (ch == 'b')
            {
                radix = 2;
                if ((flags & 0x10) && (low | high))
                    flags |= 0x400;
            }
            if (negative && (sint32)high < 0)
            {
                low = 0 - low;
                high = 0 - high - (low != 0);
                sign = '-';
            }
            text = format_number(low, high, flags, precision, radix, &length, digits);
            count += format_emit(context, text, 0, flags, negative ? sign : 0, width, precision, (sint32)length, native_destination, native_capacity);
        }
        else if (ch == 'n')
        {
            if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
            {
#ifndef NDEBUG
                fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                abort();
            }
            output = native_arguments ? xport_load_le32(native_arguments + native_offset) : r_u32(arguments);
            native_offset += 4;
            arguments += 4;
            if (!(flags & 2) && (flags & 1))
                w_u16(output, count);
            else
                w_u32(output, count);
        }
        else if (ch == 'c')
        {
            if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
            {
#ifndef NDEBUG
                fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                abort();
            }
            character = native_arguments ? native_arguments[native_offset] : r_u8(arguments);
            native_offset += 4;
            arguments += 4;
            count += format_emit(context, 0, &character, flags, 0, width, precision, 1, native_destination, native_capacity);
        }
        else if (ch == 's')
        {
            if (native_arguments && (native_offset > native_argument_bytes || native_argument_bytes - native_offset < 4))
            {
#ifndef NDEBUG
                fprintf(stderr, "Native format argument buffer exhausted\n");
#endif
                abort();
            }
            text = native_arguments ? xport_load_le32(native_arguments + native_offset) : r_u32(arguments);
            native_offset += 4;
            arguments += 4;
            if (!text)
                text = 0x800BCB9C;
            length = 0;
            if (precision < 0)
            {
                while (r_u8(text + length))
                    ++length;
            }
            else
            {
                while (length < (uint32)precision && r_u8(text + length))
                    ++length;
            }
            count += format_emit(context, text, 0, flags, 0, width, (sint32)length, (sint32)length, native_destination, native_capacity);
        }
        else
        {
            ++count;
            format_put_char(r_u8(format), context, native_destination, native_capacity);
        }
        ++format;
    }
}

uint32 pad_negotiate(uint32 port)
{
    sint16 modes[8];
    uint8 actuator[3];
    uint32 number;
    uint32 status;
    uint32 mode;
    uint32 version;
    uint32 phase;
    uint32 state;
    uint32 flags;
    uint32 count;
    uint32 selected;
    uint32 mask;
    uint32 dst;
    uint32 param0;
    uint32 param1;
    sint32 mode_count;
    sint32 id;
    sint32 i;

    FUNCTION_MARKER(0x8002685C, "ACE3.BIN");
    number = r_u8(port);
    status = PadGetState(number);
    mode = PadInfoMode(number, 1, 0);
    version = PadInfoMode(number, 2, 0);
    w_u8(port + 2, status);
    if (!status)
    {
        w_u8(r_u32(port + 4), 0);
        w_u8(r_u32(port + 4) + 1, 0);
        w_u16(r_u32(port + 4) + 10, 0);
        w_u8(port + 1, 0);
        return 0;
    }
    phase = r_u8(port + 1);
    if (!phase)
    {
        if (status == 2)
        {
            id = pad_map_mode(mode);
            w_u8(r_u32(port + 4), id);
            state = r_u32(0x800BCB84);
            flags = r_u8(state + 0xCC);
            w_u8(r_u32(port + 4) + 1, flags & 1);
            dst = r_u32(port + 4) + 2;
            PadSetAct(number, psx_addr(dst, 2), 2);
            w_u16(r_u32(port + 4) + 10, version);
            return mode;
        }
        if (status == 6)
        {
            mode_count = (sint32)PadInfoMode(number, 4, -1);
            for (i = 7; i >= 0; --i)
                modes[i] = 0;
            for (i = 0; i < mode_count && i < 7; ++i)
                modes[i] = (sint16)PadInfoMode(number, 4, i);
            id = pad_find_mode(modes);
            param0 = pad_get_mode_param0((uint32)id);
            param1 = pad_get_mode_param1((uint32)id);
            w_u8(r_u32(port + 4), id);
            PadSetMainMode(number, param0, param1);
            w_u8(r_u32(port + 4) + 1, 0);
            w_u8(port + 1, 1);
        }
        return 0;
    }
    if (phase == 1)
    {
        if (status != 6)
            return 0;
        w_u8(port + 13, 255);
        w_u8(port + 12, 255);
        w_u8(port + 11, 255);
        w_u8(port + 10, 255);
        w_u8(port + 9, 255);
        w_u8(port + 8, 255);
        count = PadInfoAct(number, -1, 0) & 255;
        selected = 0;
        for (i = 0; (uint32)i < count && i < 6; ++i)
        {
            actuator[0] = (uint8)PadInfoAct(number, i, 1);
            actuator[1] = (uint8)PadInfoAct(number, i, 2);
            actuator[2] = (uint8)PadInfoAct(number, i, 3);
            mask = pad_actuator_mask(actuator);
            if (mask)
                w_u8(port + 8 + (selected++ & 255), i);
            state = r_u32(port + 4);
            w_u8(state + 1, r_u8(state + 1) | mask);
        }
        PadSetActAlign(number, psx_addr(port + 8, 6));
        w_u8(port + 1, 2);
        return 0;
    }
    if (phase == 2)
    {
        if (status != 6)
            return 0;
        count = 0;
        flags = r_u8(r_u32(port + 4) + 1);
        for (i = 0; i < 6; ++i)
            if ((flags >> i) & 1)
                ++count;
        dst = r_u32(port + 4) + 2;
        PadSetAct(number, psx_addr(dst, count), count);
        w_u16(r_u32(port + 4) + 10, version);
        return mode;
    }
    w_u8(port + 1, 0);
    return 0;
}

sint32 pad_find_mode(const sint16 *modes)
{
    uint32 table;
    uint32 expected;
    uint32 i;

    FUNCTION_MARKER(0x80026628, "ACE3.BIN");
    table = 0x800B01C4;
    for (i = 0; i < 2; ++i)
    {
        expected = r_u32(table + 4);
        if (!pad_compare_modes(modes, expected))
            return r_s32(table);
        table += 8;
    }
    return -2;
}

sint32 pad_compare_modes(const sint16 *modes, uint32 expected)
{
    sint32 actual;
    sint32 wanted;
    uint32 terminator;

    FUNCTION_MARKER(0x800265F0, "ACE3.BIN");
    for (;;)
    {
        actual = *modes;
        wanted = r_s16(expected);
        terminator = (uint16)*modes;
        if (actual != wanted)
            return actual - wanted;
        ++modes;
        if (!terminator)
            return 0;
        expected += 2;
    }
}

sint32 pad_map_mode(uint32 mode)
{
    FUNCTION_MARKER(0x80026694, "ACE3.BIN");
    switch (mode)
    {
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 5;
        case 4:
            return 1;
        case 5:
            return 4;
        case 6:
            return 7;
        case 7:
            return 6;
        default:
            return -1;
    }
}

uint32 pad_get_mode_param0(uint32 mode)
{
    uint32 state;
    uint32 ptr;
    sint32 count;
    sint32 i;

    FUNCTION_MARKER(0x80026700, "ACE3.BIN");
    state = r_u32(0x800BCB84);
    count = r_s32(state + 0xC4);
    if (count <= 0)
        return 0;
    ptr = r_u32(state + 0xC8);
    for (i = 0; i < count; ++i)
    {
        if (r_u8(ptr) == mode)
            return r_u8(ptr + 1);
        ptr += 3;
    }
    return 0;
}

uint32 pad_get_mode_param1(uint32 mode)
{
    uint32 state;
    uint32 ptr;
    sint32 count;
    sint32 i;

    FUNCTION_MARKER(0x80026754, "ACE3.BIN");
    state = r_u32(0x800BCB84);
    count = r_s32(state + 0xC4);
    if (count <= 0)
        return 0;
    ptr = r_u32(state + 0xC8);
    for (i = 0; i < count; ++i)
    {
        if (r_u8(ptr) == mode)
            return r_u8(ptr + 2);
        ptr += 3;
    }
    return 0;
}

uint32 pad_actuator_mask(const uint8 *actuator)
{
    uint32 table;
    uint32 i;

    FUNCTION_MARKER(0x800267F0, "ACE3.BIN");
    table = 0x800B01D4;
    for (i = 0; i < 3; ++i)
    {
        if (!pad_compare_actuator(actuator, table + 1))
            return r_u8(table);
        table += 4;
    }
    return 0;
}

uint32 pad_compare_actuator(const uint8 *actuator, uint32 expected)
{
    FUNCTION_MARKER(0x800267A8, "ACE3.BIN");
    if (actuator[0] != r_u8(expected))
        return 1;
    if (actuator[1] != r_u8(expected + 1))
        return 1;
    return actuator[2] != r_u8(expected + 2);
}

sint32 sound_get_cur_resource(void)
{
    FUNCTION_MARKER(0x8002CAD0, "ACE3.BIN");
    return r_s32(r_u32(0x800BCBBC) + 0x174);
}

void sound_set_cd_volume(uint32 volume)
{
    FUNCTION_MARKER(0x8002D844, "ACE3.BIN");
    w_u32(0x800C5800, 0xC0);
    w_u16(0x800C5812, volume << 6);
    w_u16(0x800C5810, volume << 6);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(0x800C5800, 40));
}

void sound_set_master_volume(uint32 volume)
{
    FUNCTION_MARKER(0x8002D80C, "ACE3.BIN");
    w_u32(0x800C5800, 3);
    w_u16(0x800C5804, volume);
    w_u16(0x800C5806, volume);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(0x800C5800, 40));
}

uint32 sound_update_slots(void)
{
    uint32 i;
    uint32 slot;
    uint32 timer;
    uint32 next;
    sint32 signed_timer;

    FUNCTION_MARKER(0x8002F1E0, "ACE3.BIN");
    slot = 0x800C6550;
    for (i = 0; i < 8; ++i)
    {
        signed_timer = r_s16(slot + 2);
        timer = r_u16(slot + 2);
        if (signed_timer >= 0)
        {
            next = timer - 1;
            w_u16(slot + 2, next);
            if (!(next << 16))
            {
                sub_8002db4c(r_u8(slot));
                sound_reset_slot(i);
            }
        }
        slot += 4;
    }
    return 0;
}

uint32 sound_update_queue(void)
{
    uint32 head;
    uint32 slot;
    uint32 voice;
    uint32 state;
    uint32 result;
    sint32 timer;

    FUNCTION_MARKER(0x8002F2CC, "ACE3.BIN");
    head = r_u8(0x800BCE98);
    slot = 0x800C6570 + head * 4;
    voice = r_u8(slot + 1);
    if (voice != 255)
    {
        result = r_u8(0x800C5830 + voice * 20 + 3);
        if (!result)
        {
            w_u8(slot + 1, 255);
            w_u8(slot, 255);
            w_u16(slot + 2, 0xFFFF);
            result = (r_u8(0x800BCE98) + 1) & 31;
            w_u8(0x800BCE98, result);
        }
        return result;
    }
    timer = r_s16(slot + 2);
    if (timer == -1)
    {
        w_u8(0x800BCE99, head);
        return 0xFFFFFFFF;
    }
    state = r_u32(0x800BCE60);
    result = ~r_u32(state) & 0x20000000;
    if (result)
    {
        if (!timer)
            w_u8(slot + 1, sub_8002db4c(r_u8(slot)));
        result = r_u16(slot + 2) - 1;
        w_u16(slot + 2, result);
    }
    return result;
}

void sound_update(void)
{
    uint8 key_status[24];
    uint32 id;
    uint32 state;
    uint32 voice;
    uint32 flags;
    uint32 key_on = 0;
    uint32 key_off = 0;
    uint32 i;
    uint32 volume;
    uint32 cur;
    uint32 timer;
    sint32 steps;
    sint32 remaining;
    sint32 delta;

    FUNCTION_MARKER(0x8002E1F4, "ACE3.BIN");
    id = (uint32)sound_get_cur_resource() & 0x7FF;
    state = r_u32(0x800BCE60);
    if (id == 0x1E6 || id - 0x23 < 0x24)
        volume = r_u8(state + 0x399);
    else
        volume = r_u8(state + 0x398);
    sound_set_cd_volume(volume);
    state = r_u32(0x800BCE60);
    sound_set_master_volume((r_u32(state) & 0x20000000) ? 0 : 0x3FFF);
    state = r_u32(0x800BCE60);
    if (!(r_u32(state) & 0x20000000))
        sound_update_slots();
    sound_update_queue();
    voice = 0x800C5830;
    for (i = 0; i < 24; ++i)
    {
        flags = r_u8(voice + 1);
        if (flags & 1)
        {
            key_on |= 1 << i;
            w_u8(voice + 1, flags & 0xFE);
            sub_8002e0d0(i);
            flags = r_u8(voice + 1);
            if (flags & 0x10)
                w_u8(voice + 1, flags & 0xEF);
        }
        steps = r_s16(voice + 18);
        if (steps > 0)
        {
            volume = r_u8(voice + 16);
            cur = r_u8(voice + 17);
            delta = ((sint32)volume - (sint32)cur) / steps;
            remaining = r_s16(voice + 18);
            w_u8(voice + 17, cur + (uint32)delta);
            timer = r_u16(voice + 18);
            if (remaining > 0)
                w_u16(voice + 18, timer - 1);
        }
        else
            w_u8(voice + 17, r_u8(voice + 16));
        if (r_u8(voice + 17) != 128)
            w_u8(voice + 1, r_u8(voice + 1) | 0x10);
        if (r_u8(voice + 1) & 0x10)
        {
            sub_8002df98(i);
            w_u8(voice + 1, r_u8(voice + 1) & 0xEF);
        }
        voice += 20;
    }
    SpuSetKey(1, key_on);
    SpuGetAllKeysStatus(key_status);
    voice = 0x800C5830;
    for (i = 0; i < 24; ++i)
    {
        if (key_status[i] == 1)
            w_u8(voice + 2, 8);
        else if (key_status[i] != 2)
        {
            if (!key_status[i])
            {
                if (r_u8(voice + 3))
                    w_u8(voice + 1, 4);
            }
            else
            {
                timer = r_u8(voice + 2);
                if (!timer)
                    sub_8002dda8(i);
                else
                {
                    --timer;
                    w_u8(voice + 2, timer);
                    if (!(timer & 255))
                        sub_8002dda8(i);
                }
            }
        }
        voice += 20;
    }
    voice = 0x800C5830;
    for (i = 0; i < 24; ++i)
    {
        if (r_u8(voice + 1) & 4)
            sub_8002e5b4(i);
        voice += 20;
    }
    voice = 0x800C5830;
    for (i = 0; i < 24; ++i)
    {
        if (r_u8(voice + 1) & 2)
            key_off |= 1 << i;
        voice += 20;
    }
    SpuSetKey(0, key_off);
}

uint32 pad_rumble_mix(uint32 output)
{
    uint32 slot;
    uint32 effect;
    uint32 ttl;
    uint32 kind;
    uint32 strength;
    uint32 max0 = 0;
    uint32 max1 = 0;
    uint32 i;
    uint32 decay;
    uint32 result;

    FUNCTION_MARKER(0x80029734, "ACE3.BIN");
    slot = r_u32(0x800BCBA4);
    effect = slot + 1;
    for (i = 0; i < 6; ++i)
    {
        ttl = r_u8(slot);
        if (ttl)
        {
            kind = r_u8(effect);
            strength = r_u8(effect + 1);
            switch (kind)
            {
                case 1:
                case 2:
                case 3:
                case 9:
                    if ((r_u8(effect) & 1) && max0 < strength)
                        max0 = strength;
                    if (r_u8(effect) & 2)
                    {
                        if (max1 < strength)
                            max1 = strength;
                        w_u8(effect + 1, strength - (strength >> 2));
                    }
                    break;
                case 5:
                case 0xA0:
                    if (max1 < strength)
                        max1 = strength;
                    break;
                case 0x10:
                    if (max1 < strength)
                        max1 = strength;
                    w_u8(effect + 1, strength - (strength >> 3));
                    break;
                case 0x14:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < (strength >> 1))
                        max1 = strength >> 1;
                    w_u8(effect + 1, strength - (strength >> 4));
                    break;
                case 0x1E:
                    if (max0 < (strength >> 1))
                        max0 = strength >> 1;
                    if (max1 < strength)
                        max1 = strength;
                    w_u8(effect + 1, strength - (strength >> 5));
                    break;
                case 0xA1:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < strength)
                        max1 = strength;
                    break;
                case 0xB4:
                case 0xFA:
                    if (max0 < strength)
                        max0 = strength;
                    if (max1 < strength)
                        max1 = strength;
                    if (ttl < (uint32)(kind == 0xB4 ? 6 : 57))
                    {
                        decay = strength >> (kind == 0xB4 ? 4 : 5);
                        w_u8(effect + 1, strength - decay);
                    }
                    break;
                default:
                    break;
            }
            --ttl;
            w_u8(slot, ttl);
            if (!ttl)
                w_u8(effect, 0);
        }
        effect += 4;
        slot += 4;
    }
    result = max1 < 256;
    if (max0 >= 256)
        max0 = 255;
    if (max1 >= 256)
        max1 = 255;
    w_u8(output, max0);
    w_u8(output + 1, max1);
    return result;
}

uint32 format_put_char(uint32 ch, uint32 *context, uint8 *native_destination, uint32 native_capacity)
{
    uint32 cursor;

    FUNCTION_MARKER(0x80027C24, "ACE3.BIN");
    cursor = context[0];
    if (native_destination)
    {
        if (cursor >= native_capacity)
        {
#ifndef NDEBUG
            fprintf(stderr, "Native format output buffer exhausted\n");
#endif
            abort();
        }
        xport_store_u8(native_destination + cursor, ch);
    }
    else
        w_u8(cursor, ch);
    context[0] = cursor + 1;
    return ch;
}

uint32 format_to_buffer(uint32 dst, uint32 format, uint32 arguments)
{
    uint32 context[2];
    uint32 length;

    FUNCTION_MARKER(0x8002877C, "ACE3.BIN");
    context[1] = dst;
    context[0] = dst;
    length = format_core(format, arguments, context, 0, 0, 0, 0);
    w_u8(dst + length, 0);
    return length;
}

uint32 format_number(uint32 low, uint32 high, uint32 flags, sint32 precision, uint32 radix, uint32 *length, uint32 digits)
{
    uint32 cursor = 0x800C1350;
    uint32 shift = 0;
    uint32 mask = 0;
    uint32 digit;
    uint32 ch;
    uint32 next;
    uint64 value;
    uint64 quotient;
    uint64 divisor;

    FUNCTION_MARKER(0x80027C3C, "ACE3.BIN");
    if (radix == 8)
    {
        shift = 3;
        mask = 7;
    }
    else if (radix == 2)
    {
        shift = 1;
        mask = 1;
    }
    else if (radix == 16)
    {
        shift = 4;
        mask = 15;
    }
    if (precision)
    {
        if (flags & 4)
        {
            value = ((uint64)high << 32) | low;
            if (shift)
            {
                do
                {
                    --cursor;
                    digit = (uint32)value & mask;
                    value >>= shift;
                    ch = r_u8(digits + digit);
                    w_u8(cursor, ch);
                } while (value);
            }
            else
            {
                divisor = (uint64)(sint64)(sint32)radix;
                do
                {
                    if (!divisor)
                        trap(7);
                    quotient = value / divisor;
                    --cursor;
                    digit = (uint32)value - (uint32)quotient * radix;
                    ch = r_u8(digits + digit);
                    value = quotient;
                    w_u8(cursor, ch);
                } while (value);
            }
        }
        else if (shift)
        {
            do
            {
                --cursor;
                ch = r_u8(digits + (low & mask));
                low >>= shift;
                w_u8(cursor, ch);
            } while (low);
        }
        else
        {
            do
            {
                if (!radix)
                    trap(7);
                next = low / radix;
                --cursor;
                ch = r_u8(digits + low - next * radix);
                low = next;
                w_u8(cursor, ch);
            } while (low);
        }
    }
    *length = 0x800C1350 - cursor;
    return cursor;
}

uint32 sound_find_voice(uint32 sound)
{
    uint32 i;
    uint32 voice = 0x800C5920;

    FUNCTION_MARKER(0x8002DCF8, "ACE3.BIN");
    for (i = 12; i < 24; ++i, voice += 20)
        if (r_u8(voice) == sound)
            return i;
    return 255;
}

uint32 sound_reserve_voice(void)
{
    uint32 i;
    uint32 voice = 0x800C5920;

    FUNCTION_MARKER(0x8002DD38, "ACE3.BIN");
    for (i = 12; i < 24; ++i, voice += 20)
    {
        if (!r_u8(voice + 3))
        {
            w_u8(voice + 3, 2);
            return i;
        }
    }
    return 255;
}

uint32 sound_start_voice(uint32 sound, uint32 index)
{
    uint32 voice;
    uint32 descriptor;
    uint32 value;

    FUNCTION_MARKER(0x8002E514, "ACE3.BIN");
    if (index == 255)
        return 255;
    voice = 0x800C5830 + index * 20;
    w_u8(voice + 1, 1);
    w_u8(voice + 3, 1);
    w_u8(voice + 2, 16);
    descriptor = 0x800C5A10 + sound * 20;
    w_u8(voice, sound);
    value = r_u16(descriptor + 4);
    w_u16(voice + 4, value);
    w_u16(voice + 8, value);
    value = r_u16(descriptor + 6);
    w_u16(voice + 6, value);
    w_u16(voice + 10, value);
    value = r_u16(descriptor + 8);
    w_u8(voice + 16, 128);
    w_u8(voice + 17, 128);
    w_u16(voice + 18, 0);
    w_u16(voice + 12, value);
    w_u16(voice + 14, value);
    return index;
}

uint32 sub_8002dbe0(uint32 sound)
{
    uint32 index;

    FUNCTION_MARKER(0x8002DBE0, "ACE3.BIN");
    index = r_u8(0x800C5A10 + sound * 20);
    if (index == 255)
    {
        index = sound_find_voice(sound);
        if (index == 255)
        {
            index = sound_reserve_voice();
            if (index == 255)
                return 255;
        }
    }
    sound_start_voice(sound, index);
    return index;
}

uint32 sub_8002db4c(uint32 sound)
{
    uint32 index;

    FUNCTION_MARKER(0x8002DB4C, "ACE3.BIN");
    if (sound - 46 < 4)
        sub_8002dbe0(sound);
    index = r_u8(0x800C5A10 + sound * 20);
    if (index == 255)
    {
        index = sound_reserve_voice();
        if (index == 255)
            return 255;
    }
    sound_start_voice(sound, index);
    return index;
}

void sub_8002e0d0(uint32 index)
{
    SpuVoiceAttr attr = {0};
    uint32 voice;
    uint32 game;
    uint32 sound;
    uint32 descriptor;
    sint32 volume;
    sint32 left;
    sint32 right;

    FUNCTION_MARKER(0x8002E0D0, "ACE3.BIN");
    voice = 0x800C5830 + index * 20;
    sound = r_u8(voice);
    game = r_u32(0x800BCE60);
    volume = r_u8(game + 920);
    attr.voice = (uint32)1 << (index & 31);
    attr.mask = 0x600E3;
    descriptor = 0x800C5A10 + sound * 20;
    if (r_u8(game + 922) & 1)
    {
        left = r_s16(voice + 4);
        right = r_s16(voice + 6);
        if (left < 0)
            left = -left;
        if (right < 0)
            right = -right;
        attr.volume.left = (sint16)((volume * (left + right)) >> 9);
        attr.volume.right = attr.volume.left;
    }
    else
    {
        attr.volume.left = (sint16)((r_s16(voice + 4) * volume) >> 8);
        attr.volume.right = (sint16)((r_s16(voice + 6) * volume) >> 8);
    }
    attr.note = r_u16(voice + 12);
    attr.sample_note = r_u16(descriptor + 10);
    attr.addr = r_u32(descriptor + 16);
    attr.adsr1 = r_u16(descriptor + 12);
    attr.adsr2 = r_u16(descriptor + 14);
    SpuSetVoiceAttr(&attr);
}

uint32 resource_init_queue(uint32 begin, uint32 callback)
{
    uint32 i;
    uint32 context;
    uint32 result;

    FUNCTION_MARKER(0x8002C6A8, "ACE3.BIN");
    for (i = 0; i < 16; ++i)
        w_u8(r_u32(0x800BCBBC) + i * 20 + 8, 0);
    context = r_u32(0x800BCBBC);
    w_u8(context + 332, 1);
    result = r_u32(0x800BCBBC);
    w_u32(context + 328, 1);
    w_u32(result + 336, begin);
    w_u32(result + 348, callback);
    w_u8(result + 333, 0);
    return result;
}

uint32 arena_alloc(uint32 bytes)
{
    uint32 game;
    uint32 begin;
    uint32 size;
    uint32 end;

    FUNCTION_MARKER(0x80027410, "ACE3.BIN");
    size = ((bytes + 3) >> 2) << 2;
    game = r_u32(0x800BCE60);
    begin = r_u32(game + 28);
    end = begin + size;
    w_u32(game + 28, end);
    if (end <= 0x801FB700)
        return begin;
#ifndef NDEBUG
    printf("Error: MemAlloc: Overflow %d bytes (REQ: %d bytes)\n", (sint32)(end + 0x7FE04900), (sint32)(size));
#endif
    return 0;
}

uint32 resource_total_size(uint32 ids, sint32 count)
{
    uint32 total = 0;
    sint32 i;
    uint32 id;

    FUNCTION_MARKER(0x8002C63C, "ACE3.BIN");
    for (i = 0; i < count; ++i)
    {
        id = r_u32(ids);
        ids += 4;
        total += resource_get_size(id);
    }
    return total;
}

uint32 resource_queue_list(uint32 ids, sint32 count, uint32 dst, uint32 flags)
{
    sint32 remaining;
    sint32 i;
    uint32 id;

    FUNCTION_MARKER(0x8002C82C, "ACE3.BIN");
    remaining = (sint32)((uint32)count - 1);
    for (i = 0; i < remaining; ++i)
    {
        id = r_u32(ids);
        w_u32(ids, dst);
        ids += 4;
        dst = resource_queue_read(id, dst, 0);
    }
    id = r_u32(ids);
    w_u32(ids, dst);
    return resource_queue_read(id, dst, flags);
}

uint32 resource_stop_cd(uint32 mode)
{
    uint32 context;
    uint32 other;
    uint32 command = 9;

    FUNCTION_MARKER(0x8002AC3C, "ACE3.BIN");
    game_cd_ready_callback(0);
    game_cd_sync_callback(0);
    context = r_u32(0x800BCBAC);
    other = r_u32(0x800BCBB0);
    w_u32(context, 0);
    w_u32(context + 4, 0);
    w_u32(context + 32, 0);
    w_u32(other, 0);
    sub_8002999c(1);
    if (CdSync(1, (uint8 *)psx_addr(r_u32(0x800BCBA8) + 40, 8)) != 2)
        return 0;
    if (mode)
        command = mode == 1 ? 7 : 8;
    return CdControl(command, NULL, (uint8 *)psx_addr(r_u32(0x800BCBA8) + 40, 8)) != 0;
}

uint32 resource_stop_request(uint32 request)
{
    uint32 context;
    uint32 result;

    FUNCTION_MARKER(0x8002BDCC, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    w_u32(context + 372, 0xFFFFFFFF);
    result = resource_stop_cd(r_u32(request + 4));
    if (result)
        w_u32(r_u32(0x800BCBBC) + 376, 0);
    return result;
}

uint32 sound_enable_reverb(void)
{
    FUNCTION_MARKER(0x8002DA94, "ACE3.BIN");
    SpuClearReverbWorkArea(r_s32(0x800C65F4));
    w_u32(0x800C65F0, 6);
    SpuSetReverbDepth((SpuReverbAttr *)psx_addr(0x800C65F0, 20));
    SpuSetReverb(1);
    return SpuSetReverbVoice(1, 0xFFFFFF);
}

uint32 arena_set_cursor(uint32 cursor)
{
    uint32 game;
    uint32 begin;

    FUNCTION_MARKER(0x80027538, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    begin = r_u32(game + 24);
    w_u32(game + 28, cursor);
    if (cursor >= begin)
        return cursor;
#ifndef NDEBUG
    printf("Error: MemSet: Underflow %d bytes\n", (sint32)(begin - cursor));
#endif
    return 0;
}

uint32 sub_8002b5fc(void)
{
    FUNCTION_MARKER(0x8002B5FC, "ACE3.BIN");
    return r_u32(r_u32(0x800BCBAC) + 24);
}

uint32 sub_80093b2c(uint32 ot, uint32 packet, uint32 rect, uint32 depth)
{
    uint32 mode = 1;
    uint32 head;
    uint32 xy;
    uint32 wh;

    FUNCTION_MARKER(0x80093B2C, "ACE3.BIN");
    if (!depth)
        return packet;
    if ((sint32)depth < 0)
    {
        depth = 0 - depth;
        mode = 2;
    }
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x62FFFFFF);
    xport_gte_write_data(8, 4096 - depth);
    head = r_u32(ot);
    w_u32(packet, head | 0x04000000);
    w_u32(ot, packet & 0xFFFFFF);
    xport_gte_execute(0x780010);
    w_u32(packet + 4, 0xE1000200 | (mode << 5));
    xy = r_u32(rect);
    wh = r_u32(rect + 4);
    w_u32(packet + 12, xy);
    w_u32(packet + 16, wh);
    w_u32(packet + 8, xport_gte_read_data(22));
    return packet + 20;
}

uint32 gpu_copy_rect(uint32 ot, uint32 packet, uint32 src_x, uint32 src_y, uint32 dst_x, uint32 dst_y, uint32 width, uint32 height)
{
    uint32 tag;
    uint32 head;

    FUNCTION_MARKER(0x800363B0, "ACE3.BIN");
    w_u32(packet, 0x05000000);
    w_u32(packet + 4, 0x01000000);
    w_u32(packet + 12, (src_x & 0xFFFF) | (src_y << 16));
    w_u32(packet + 8, 0x80000000);
    w_u32(packet + 16, (dst_x & 0xFFFF) | (dst_y << 16));
    w_u32(packet + 20, (width & 0xFFFF) | (height << 16));
    tag = r_u32(packet);
    head = r_u32(ot);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 24;
}

uint32 sub_8002c8b8(uint32 id, uint32 dst, uint32 flags)
{
    FUNCTION_MARKER(0x8002C8B8, "ACE3.BIN");
    return resource_queue_read(id, dst, flags);
}

void sub_8009b6ac(void)
{
    FUNCTION_MARKER(0x8009B6AC, "ACE3.BIN");
    w_u32(0x800BD0C8, 0);
    game_overlay_load_complete();
}

uint32 sub_8009b6b8(uint32 id, uint32 dst)
{
    uint32 context;
    uint32 flags;
    uint32 mode;
    uint32 game;
    uint32 next;

    FUNCTION_MARKER(0x8009B6B8, "ACE3.BIN");
    context = r_u32(0x800BCE48);
    flags = r_u32(context);
    mode = r_u32(context + 4);
    w_u32(context + 20, 0);
    w_u32(context, flags | 5);
    if (!mode)
    {
        game = r_u32(0x800BCE60);
        if (r_u32(game + 4))
            next = gpu_copy_rect(r_u32(game + 16), r_u32(game + 20), 0, 256, 0, 0, 320, 240);
        else
            next = gpu_copy_rect(r_u32(game + 16), r_u32(game + 20), 0, 0, 0, 256, 320, 240);
        game = r_u32(0x800BCE60);
        w_u32(game + 20, next);
        w_u32(0x800BCE30, 1);
    }
    else if (mode == 1)
        w_u32(0x800BCE30, 2);
    else
        w_u32(0x800BCE30, 0);
    w_u32(0x800BCE34, id);
    w_u32(0x800BD0C8, 1);
    game_overlay_load_begin(id, dst);
    sub_8002c8b8(id, dst, 0x8009B6AC);
    return 0x04000000;
}

sint32 resource_read_progress(uint32 output, uint32 *native_output)
{
    uint32 context;
    uint32 begin;
    uint32 end;
    uint32 current;
    uint32 total;
    uint32 completed;
    sint32 progress;

    FUNCTION_MARKER(0x8002CA2C, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    begin = r_u32(context + 380);
    end = r_u32(context + 384);
    current = sub_8002b5fc();
    if (native_output)
        *native_output = current;
    else if (output)
        w_u32(output, current);
    completed = (current - begin) >> 2;
    total = (end - begin) >> 2;
    progress = total ? (sint32)((completed << 12) / total) : 0;
    if (progress < 0)
        progress = 0;
    if (progress > 4096)
        progress = 4096;
    return progress;
}

uint32 sub_8009b7cc(void)
{
    uint32 game;
    uint32 mode;
    uint32 progress;
    uint32 packet;
    uint32 context;

    FUNCTION_MARKER(0x8009B7CC, "ACE3.BIN");
    if ((uint32)sound_get_cur_resource() == r_u32(0x800BCE34))
    {
        progress = (uint32)resource_read_progress(0, 0);
        mode = r_u32(0x800BCE30);
        if (mode == 1 || mode == 2)
        {
            game = r_u32(0x800BCE60);
            w_u16(0x1F800004, 320);
            w_u16(0x1F800000, 0);
            w_u16(0x1F800002, 0);
            w_u16(0x1F800006, mode == 1 ? 240 : 480);
            packet = sub_80093b2c(r_u32(game + 12), r_u32(game + 20), 0x1F800000, 0 - progress);
            game = r_u32(0x800BCE60);
            w_u32(game + 20, packet);
        }
    }
    if (r_u32(0x800BD0C8))
        return 0;
    context = r_u32(0x800BCE48);
    if (!r_u32(context + 4))
    {
        VSync(0);
        ClearImage((PSX_RECT *)psx_addr(0x800BCE38, 8), 0, 0, 0);
        DrawSync(0);
    }
    context = r_u32(0x800BCE48);
    w_u8(context + 16, 0);
    context = r_u32(0x800BCE48);
    w_u8(context + 17, 0);
    context = r_u32(0x800BCE48);
    w_u8(context + 18, 0);
    context = r_u32(0x800BCE48);
    w_u32(context, r_u32(context) | 8);
    return 0x02000000;
}

uint32 sub_80026350(void)
{
    uint32 game;
    uint32 state;
    uint32 table;
    uint32 target;
    uint32 command;
    uint32 value;

    FUNCTION_MARKER(0x80026350, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    state = r_u32(game + 52);
    if (state == r_u32(game + 56))
    {
        table = r_u32(0x800B0134 + (r_u32(game + 44) << 4));
        target = r_u32(table + (r_u32(game + 48) << 2));
        command = xport_guest_call3(target, state, game, 0);
    }
    else
    {
        target = r_u32(0x800B4674 + (r_u32(game + 48) << 2));
        command = xport_guest_call3(target, state, r_u32(game + 60), 0);
        if (command & 0x02000000)
        {
            game = r_u32(0x800BCE60);
            state = r_u32(game + 52);
            w_u32(game + 48, 0);
            w_u32(game + 56, state);
        }
    }
    if (!command)
        return 0;
    if (command & 0x20000000)
    {
        game = r_u32(0x800BCE60);
        w_u32(game + 44, command & 255);
        w_u32(game + 48, 0);
    }
    else if (command & 0x08000000)
    {
        game = r_u32(0x800BCE60);
        value = r_u32(game + 44);
        w_u32(game + 48, 0);
        w_u32(game + 44, value + 1);
    }
    else if (command & 0x10000000)
    {
        game = r_u32(0x800BCE60);
        w_u32(game + 48, command & 255);
    }
    else if (command & 0x04000000)
    {
        game = r_u32(0x800BCE60);
        w_u32(game + 48, r_u32(game + 48) + 1);
    }
    if (command & 0x28000000)
    {
        game = r_u32(0x800BCE60);
        table = 0x800B0134 + (r_u32(game + 44) << 4);
        value = r_u32(table + 4);
        table = 0x800B0134 + (r_u32(game + 44) << 4);
        w_u32(game + 52, value);
        value = r_u32(table + 8);
        table = 0x800B0134 + (r_u32(game + 44) << 4);
        w_u32(game + 60, value);
        value = r_u32(table + 12);
        state = r_u32(game + 52);
        w_u32(game + 64, value);
        if ((sint32)state < 0)
            w_u32(game + 56, 0xFFFFFFFF);
    }
    if (command & 0x40000000)
    {
        game = r_u32(0x800BCE60);
        game_set_arena(r_u32(game + 64));
        game = r_u32(0x800BCE60);
        w_u32(game, r_u32(game) | 8);
    }
    return 0;
}

uint32 sub_80026044(void)
{
    FUNCTION_MARKER(0x80026044, "ACE3.BIN");
    return 0x60000004;
}

uint32 sub_800278a8(uint32 ot, uint32 packet)
{
    uint32 context;
    uint32 entry;
    uint32 i;
    uint32 first;
    uint32 second;
    uint32 tag;
    uint32 head;
    sint32 count;

    FUNCTION_MARKER(0x800278A8, "ACE3.BIN");
    context = r_u32(0x800BCB98);
    count = r_s32(context);
    entry = context + 4;
    for (i = 0; (sint32)i < count; ++i, packet += 20)
    {
        w_u32(packet + 4, 0x64808080);
        first = r_u32(entry);
        w_u32(packet + 8, first);
        second = r_u32(entry + 4);
        entry += 8;
        w_u32(packet + 16, 0x00060006);
        w_u32(packet + 12, second);
        head = r_u32(ot);
        w_u32(packet, head | 0x04000000);
        w_u32(ot, packet & 0xFFFFFF);
    }
    w_u8(packet + 3, 1);
    w_u32(packet + 4, 0xE1000015);
    tag = r_u32(packet);
    head = r_u32(ot);
    w_u32(0x800BCE68, 0x00100010);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    context = r_u32(0x800BCB98);
    w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
    w_u32(context, 0);
    return packet + 8;
}

uint32 pad_normalize_axes(sint16 *axes, uint32 calibration)
{
    uint32 i;
    sint32 low;
    sint32 high;
    sint32 center;
    sint32 dead_zone;
    sint32 delta;
    sint32 numerator;
    sint32 denominator;

    FUNCTION_MARKER(0x8002895C, "ACE3.BIN");
    for (i = 0; i < 4; ++i, calibration += 4)
    {
        low = r_u8(calibration);
        high = r_u8(calibration + 2);
        if (axes[i] < low)
            axes[i] = (sint16)low;
        if (high < axes[i])
            axes[i] = (sint16)high;
        center = r_u8(calibration + 1);
        dead_zone = r_u8(calibration + 3);
        delta = (sint16)((uint16)axes[i] - center);
        if (delta > dead_zone)
        {
            numerator = (delta - dead_zone) * 8192;
            denominator = 2 * (high - center - dead_zone);
            if (!denominator)
                trap(7);
            axes[i] = (sint16)(numerator / denominator - 1);
        }
        else if (delta < -dead_zone)
        {
            numerator = (delta + dead_zone) * 8192;
            denominator = -2 * (low - center + dead_zone);
            if (!denominator)
                trap(7);
            axes[i] = (sint16)(numerator / denominator);
        }
        else
            axes[i] = 0;
    }
    return 0;
}

sint32 pad_curve_expand(sint32 value)
{
    sint32 half;
    sint32 sine;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x80028A7C, "ACE3.BIN");
    half = (sint16)value / 2;
    sine = rsin((uint32)half & 0xFFF);
    scaled = (sint32)((uint32)sine * 7);
    result = (sint16)((uint32)value + (uint32)(scaled / 24));
    if (result < -4096)
        result = -4096;
    if (result >= 4096)
        result = 4095;
    return result;
}

sint32 pad_curve_contract(sint32 value)
{
    sint32 half;
    sint32 sine;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x80028B1C, "ACE3.BIN");
    half = (sint16)value / 2;
    sine = rsin((uint32)(-half) & 0xFFF);
    scaled = (sint32)((uint32)sine * 7);
    result = (sint16)((uint32)value + (uint32)(scaled / 24));
    if (result < -4096)
        result = -4096;
    if (result >= 4096)
        result = 4095;
    return result;
}

uint32 pad_update_input(uint32 pad, uint32 state, uint32 profiles)
{
    sint16 axes[4];
    uint32 profile = profiles;
    uint32 pressed;
    uint32 buttons = 0;
    uint32 type;
    uint32 kind;
    uint32 i;
    uint32 selector;
    uint32 flags;
    uint32 mask;
    uint32 count;
    uint32 prev;
    uint32 edges;
    uint32 directions;
    uint32 game;
    uint32 result;
    uint32 level0;
    uint32 level1;
    uint32 output;
    uint32 phase;
    sint32 value;
    sint32 limit;
    sint32 horizontal;
    sint32 vertical;
    sint32 dither;

    FUNCTION_MARKER(0x80028C24, "ACE3.BIN");
    pressed = r_u16(pad + 14);
    w_u32(state + 24, 0);
    w_u16(state + 54, 0);
    type = r_u8(pad + 12);
    if (type == 0x41 || type == 0x43)
        w_u32(state + 4, 1);
    else if (type == 0x23 || type == 0x53 || type == 0x73)
    {
        if (type == 0x23)
        {
            kind = 16;
            profile += 368;
        }
        else
        {
            type = r_u8(pad + 12);
            if (type == 0x73)
            {
                if (r_u16(pad + 10) == 7)
                {
                    kind = 4;
                    profile += 184;
                }
                else
                {
                    kind = 2;
                    profile += 92;
                }
            }
            else
            {
                kind = 8;
                profile += 276;
            }
        }
        w_u32(state + 4, kind);
        axes[0] = (sint16)r_u16(pad + 18);
        axes[1] = (sint16)r_u16(pad + 20);
        axes[2] = (sint16)r_u16(pad + 22);
        axes[3] = (sint16)r_u16(pad + 24);
        pad_normalize_axes(axes, profile + 64);
        for (i = 0; i < 6; ++i)
        {
            selector = r_u8(profile + 82 + i);
            if (selector == 255)
                w_u32(state + 28 + i * 4, 0);
            else
            {
                mask = r_u32(state + 24);
                value = axes[selector];
                w_u32(state + 24, mask | ((uint32)1 << i));
                flags = r_u16(profile + 80);
                if (flags & 1)
                    value = (sint16)pad_curve_expand(value);
                else if (flags & 2)
                    value = (sint16)pad_curve_contract(value);
                w_u32(state + 28 + i * 4, (uint32)value);
            }
        }
        if (kind == 16)
        {
            if (axes[0] >= 1537)
                pressed |= 0x4000;
            if (axes[3] >= 1537)
                pressed |= 0x8000;
            if (axes[2] >= 1537)
                pressed |= 0x400;
            if (pressed & 0x400)
                pressed |= 0x100;
            if (pressed & 0x800)
                pressed |= 0x200;
        }
        else
        {
            if (r_u8(pad + 12) == 0x53)
            {
                horizontal = axes[0];
                vertical = axes[1];
                limit = 1536;
            }
            else
            {
                horizontal = axes[2];
                vertical = axes[3];
                limit = 3754;
            }
            if (horizontal < -limit)
                buttons |= 1;
            if (horizontal > limit)
                buttons |= 2;
            if (vertical < -limit)
                buttons |= 4;
            if (vertical > limit)
                buttons |= 8;
        }
    }
    else
    {
        pressed = 0;
        w_u32(state + 4, type ? 32 : 0);
        w_u32(state + 16, 0);
        w_u32(state + 12, 0);
        w_u32(state + 8, 0);
        w_u32(state + 24, 0);
        w_u32(state + 36, 0);
        w_u32(state + 32, 0);
        w_u32(state + 28, 0);
        w_u32(state + 48, 0);
        w_u32(state + 44, 0);
        w_u32(state + 40, 0);
        w_u8(state + 56, 0);
        w_u8(state + 59, 0);
        w_u8(state + 58, 0);
        w_u8(state + 57, 0);
    }
    flags = r_u8(pad + 13);
    if (flags & 1)
        w_u16(state + 54, r_u16(state + 54) | 2);
    else if (flags & 2)
        w_u16(state + 54, r_u16(state + 54) | 8);
    else if (flags & 8)
        w_u16(state + 54, r_u16(state + 54) | 1);
    for (i = 0; i < 32; ++i)
    {
        mask = (uint32)r_s16(profile + i * 2);
        if (mask && (pressed & mask))
            buttons |= (uint32)1 << i;
    }
    if (r_u32(profile + 88) & 0x10)
    {
        if (buttons & 0x200000)
        {
            count = r_u8(state + 57);
            buttons &= 0xFFDFFFFF;
            if (count < 7)
            {
                ++count;
                w_u8(state + 57, count);
            }
            if ((count & 255) >= 7)
                buttons |= 0x2000;
        }
        else
        {
            if ((uint32)(r_u8(state + 57) - 1) < 6)
                buttons |= 0x200000;
            w_u8(state + 57, 0);
        }
    }
    if (r_u32(profile + 88) & 8)
    {
        if (buttons & 0x4000)
        {
            count = r_u8(state + 58);
            buttons &= 0xFFFFBFFF;
            if (count < 10)
            {
                ++count;
                w_u8(state + 58, count);
            }
            if ((count & 255) >= 10)
                buttons |= 0x80000000;
        }
        else
        {
            if ((uint32)(r_u8(state + 58) - 1) < 9)
                buttons |= 0x4000;
            w_u8(state + 58, 0);
        }
    }
    prev = r_u32(state + 8);
    directions = buttons & 15;
    w_u32(state + 8, buttons);
    edges = ~prev & buttons;
    w_u32(state + 12, edges);
    w_u32(state + 16, edges);
    w_u32(state + 20, prev & ~buttons);
    if (directions && directions == (prev & 15))
        w_u8(state + 56, r_u8(state + 56) + 1);
    else
        w_u8(state + 56, 0);
    count = r_u8(state + 56);
    if (count >= 23 && !(count & 3))
        w_u32(state + 16, r_u32(state + 16) | directions);
    if (r_u16(profile + 80) & 4)
        w_u32(state + 28, 0 - r_u32(state + 28));
    if (r_u16(profile + 80) & 8)
        w_u32(state + 40, 0 - r_u32(state + 40));
    if (r_u16(profile + 80) & 0x10)
        w_u32(state + 44, 0 - r_u32(state + 44));
    pad_rumble_mix(state + 52);
    result = r_u32(state) & 1;
    if (result)
    {
        game = r_u32(0x800BCE60);
        result = ~r_u32(game) & 0x20000000;
    }
    if (result)
    {
        level0 = r_u8(state + 52);
        flags = r_u8(pad + 1);
        level1 = r_u8(state + 53);
        if (!(flags & 1))
            level1 = (level1 + level0) >> 1;
        if (!(flags & 2))
            level0 = (level0 + level1) >> 1;
        output = pad + 2;
        phase = r_u32(game + 36) & 31;
        dither = r_s32(0x800B0340 + ((level0 >> 2) & 0x3C));
        mask = (uint32)(dither >> phase) & 1;
        if (!r_u16(pad + 10) && r_u8(pad + 12))
        {
            w_u8(pad + 2, 64);
            output = pad + 3;
        }
        if (r_u8(pad + 1) & 1)
        {
            w_u8(output, mask);
            ++output;
        }
        result = r_u8(pad + 1) & 2;
        if (result)
            w_u8(output, level1);
    }
    else
    {
        w_u8(pad + 2, 0);
        w_u8(pad + 3, 0);
        w_u8(pad + 4, 0);
        w_u8(pad + 5, 0);
        w_u8(pad + 6, 0);
        w_u8(pad + 7, 0);
    }
    return result;
}

uint32 format_to_native_buffer(uint8 *destination, uint32 capacity, uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint32 context[2] = {0, 0};
    uint32 length;
    if (!destination || capacity == 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native format output requires a bounded buffer\n");
#endif
        abort();
    }
    length = format_core(format, arguments, context, destination, native_arguments, native_argument_bytes, capacity);
    if (length >= capacity)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native format terminator exceeds output buffer\n");
#endif
        abort();
    }
    xport_store_u8(destination + length, 0);
    return length;
}
