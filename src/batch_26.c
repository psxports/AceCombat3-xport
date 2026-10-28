#include "game_functions.h"
#include <stdlib.h>

uint32 resource002_sub_800f1384(void)
{
    uint32 state, table, owner, value;

    FUNCTION_MARKER(0x800F1384, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    table = r_u32(state + 364);
    value = r_s16(table + 190);
    if (value != 1)
        return 0x80110000;
    owner = r_u32(0x80112DB0);
    if (r_u32(owner + 12) == 0)
    {
        w_u16(state + 134, 15);
        return 15;
    }
    w_u16(state + 134, 4);
    w_u32(owner + 16, 0);
    return 4;
}

uint32 resource002_sub_800f2990(void)
{
    uint32 state, owner, value;

    FUNCTION_MARKER(0x800F2990, "ACE3.BIN");
    value = resource002_sub_800dee84(1);
    if (value != 0)
        return value;
    state = r_u32(0x80112DC4);
    w_u8(state + 124, 3);
    state = r_u32(0x80112DC4);
    w_u8(state + 125, 3);
    owner = r_u32(0x80112DB0);
    state = r_u32(0x80112DC4);
    w_u16(state + 454, 6);
    return arena_set_cursor(owner);
}

uint32 resource002_sub_800f1c8c(void)
{
    uint32 state, owner, value;

    FUNCTION_MARKER(0x800F1C8C, "ACE3.BIN");
    resource002_sub_800ee9e4();
    if (sub_8002b59c() != 0)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 11);
        return 11;
    }
    owner = r_u32(0x80112DB0);
    value = r_u32(owner + 3908) - 1;
    w_u32(owner + 3908, value);
    if ((sint32)value >= 0)
        return value;
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 11);
    w_u32(owner + 3896, 2);
    return 2;
}

uint32 sub_8002cae4(uint32 id)
{
    uint32 context;

    FUNCTION_MARKER(0x8002CAE4, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    return sub_8002cae8(id, context);
}

uint32 resource002_sub_800f07b8(uint32 owner)
{
    FUNCTION_MARKER(0x800F07B8, "ACE3.BIN");
    if (r_u32(owner + 5784) == 0)
        return 0;
    resource_init_queue(0, 0);
    return 1;
}

uint32 resource002_sub_800f07b0(void)
{
    FUNCTION_MARKER(0x800F07B0, "ACE3.BIN");
    return resource002_sub_800f07b8(r_u32(0x80112DB0));
}

uint32 resource002_sub_800f0174(uint32 kind)
{
    uint32 index, result, target;

    FUNCTION_MARKER(0x800F0174, "ACE3.BIN");
    for (index = 1; index <= 5; ++index)
        result = resource002_sub_800dee08(index, 0, 0, 0);
    if (kind >= 11)
        return result;
    target = r_u32(0x800CCF00 + kind * 4);
    switch (target)
    {
        case 0x800F0210:
            return 0;
        case 0x800F0218:
            return 2;
        case 0x800F0220:
            return 3;
        case 0x800F0228:
            return 4;
        case 0x800F0230:
            return 5;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 transition branch 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800f1848(void)
{
    uint32 transition, owner, state, result, timer, flag, game, shift, bits;
    uint32 selection, id, kind, value, slot, pointer, menu;

    FUNCTION_MARKER(0x800F1848, "ACE3.BIN");
    transition = resource002_sub_800dee84(3);
    resource002_sub_800ee9e4();
    owner = r_u32(0x80112DB0);
    result = r_u32(owner + 5784);
    if (result != 0)
    {
        if (sub_8002b59c() == 0)
        {
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 3908);
            if ((sint32)timer > 0)
            {
                w_u32(owner + 3908, timer - 1);
                return 0;
            }
        }
        owner = r_u32(0x80112DB0);
        flag = r_u32(owner + 5784);
        result = 0x80110000;
        if (flag != 0)
        {
            state = r_u32(0x80112DC4);
            id = r_u32(state + 300);
            value = sub_8002cae4(id);
            if (value == 0xFFFFFFFF)
                result = resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            else
            {
                game = r_u32(0x800BCE60);
                result = r_u32(game + 300) & 16;
                if (result != 0)
                {
                    resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
                    result = resource_init_queue(0, 0);
                }
            }
        }
    }
    if (transition != 0)
        return result;
    resource002_sub_800f07b0();
    owner = r_u32(0x80112DB0);
    shift = r_u32(owner + 28);
    bits = r_u32(owner + 3888);
    resource002_sub_800dedf0(0, bits | (1u << (shift & 31)));
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    value = resource002_sub_800f0174(kind);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 5804, value);
    resource002_sub_800dee08(1, 1, 0, 15);
    owner = r_u32(0x80112DB0);
    slot = r_u32(owner + 5804);
    if (slot != 0)
        resource002_sub_800dee08(slot, 1, 0, 15);
    sub_8002db4c(100);
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    if (kind == 0 || kind == 1)
    {
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50, kind);
    }
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 8);
    return 8;
}

uint32 resource002_sub_800f1a7c(void)
{
    uint8 rectangle[8];
    uint32 state, owner, value, index, first, second, result;

    FUNCTION_MARKER(0x800F1A7C, "ACE3.BIN");
    resource002_sub_800ee9e4();
    if (resource002_sub_800dee84(1) == 2)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 9);
        owner = r_u32(0x80112DB0);
        w_u32(state + 420, 0);
        w_u32(owner + 16, 0xFFFFFFFF);
        value = r_u32(state + 304);
        result = sound_set_voice_fade(value, 50, 30);
        for (index = 1; index < 5; ++index)
        {
            state = r_u32(0x80112DC4);
            value = r_u32(state + 304 + index * 4);
            result = sound_set_voice_fade(value, 50, 30);
        }
        return result;
    }
    state = r_u32(0x80112DC4);
    value = r_u32(state + 420) + 1;
    w_u32(state + 420, value);
    first = r_u32(0x800CCF70);
    first = r_u32(0x800CCF70);
    second = r_u32(0x800CCF74);
    second = r_u32(0x800CCF74);
    xport_store_le32(rectangle, first);
    xport_store_le32(rectangle + 4, second);
    return sub_800a8318(0, 0, 0, 0, rectangle);
}

uint32 resource002_sub_800f1b8c(void)
{
    uint32 owner, selection, id, kind, state, resource, first, second;

    FUNCTION_MARKER(0x800F1B8C, "ACE3.BIN");
    resource002_sub_800ee9e4();
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    resource002_sub_800f0244(kind);
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    state = r_u32(0x80112DC4);
    resource = r_s16(0x8010B400 + id * 8 + 2);
    w_u32(state + 300, resource);
    selection = r_u32(owner + 6300);
    w_u32(owner + 16, 0);
    first = r_u32(state + 176);
    id = r_s16(selection);
    second = r_u32(state + 180);
    kind = r_u8(0x8010B400 + id * 8 + 4);
    sub_8002cc44(kind, first, second);
    if (resource002_sub_800f072c() != 0)
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 10);
        owner = r_u32(0x80112DB0);
        w_u32(owner + 3908, 180);
    }
    else
    {
        state = r_u32(0x80112DC4);
        w_u16(state + 134, 11);
    }
    state = r_u32(0x80112DC4);
    w_u32(state + 296, 0);
    return 1;
}

uint32 resource002_sub_800fee68(uint32 resource)
{
    uint32 group, index, value, game, entry;

    FUNCTION_MARKER(0x800FEE68, "ACE3.BIN");
    for (group = 0; group < 62; ++group)
    {
        for (index = 0; index < 8; ++index)
        {
            value = r_s16(0x80112330 + group * 16 + index * 2);
            if (value == resource)
            {
                game = r_u32(0x800BCE60);
                entry = game + group;
                if (r_u8(entry + 3400) == 0)
                    w_u8(entry + 3400, 1);
            }
        }
    }
    return 0;
}

uint32 resource002_sub_800f0244(uint32 kind)
{
    uint32 state, owner, selection, resource, value, target, first, second;

    FUNCTION_MARKER(0x800F0244, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DB0);
    w_u32(state + 276, 0);
    w_u32(owner + 3896, 0);
    w_u32(state + 276, 0);
    selection = r_u32(owner + 6300);
    w_u32(owner + 5784, 0);
    resource = r_s16(selection);
    resource002_sub_800fee68(resource);
    kind = (uint32)(sint32)(sint16)kind;
    if (kind >= 12)
        return 0x800D0000;
    target = r_u32(0x800CCF30 + kind * 4);
    switch (target)
    {
        case 0x800F02B8:
        case 0x800F031C:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 3896, 1);
            w_u32(owner + 5784, 1);
            w_u32(owner + 5712, 1);
            w_u16(owner + 5716, 120);
            w_u16(owner + 5718, 93);
            w_u16(owner + 5720, 80);
            w_u16(owner + 5722, 160);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5752, 80);
            w_u16(owner + 5754, 80);
            resource002_sub_800edcc0(kind);
            return sub_8002db4c(target == 0x800F02B8 ? 106 : 103);
        case 0x800F0370:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 3896, 1);
            w_u32(owner + 5784, 1);
            w_u32(owner + 5712, 2);
            w_u16(owner + 5716, 70);
            w_u16(owner + 5718, 93);
            w_u16(owner + 5720, 80);
            w_u16(owner + 5722, 160);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5752, 80);
            w_u16(owner + 5754, 80);
            w_u16(owner + 5724, 170);
            w_u16(owner + 5726, 93);
            w_u16(owner + 5728, 80);
            w_u16(owner + 5730, 160);
            w_u16(owner + 5756, 80);
            w_u16(owner + 5758, 0);
            w_u16(owner + 5760, 80);
            w_u16(owner + 5762, 80);
            resource002_sub_800edcc0(kind);
            return sub_8002db4c(103);
        case 0x800F0400:
            owner = r_u32(0x80112DB0);
            w_u16(owner + 5716, 74);
            w_u16(owner + 5724, 150);
            w_u16(owner + 5730, 128);
            w_u16(owner + 5762, 64);
            selection = r_u32(owner + 6300);
            w_u16(owner + 5720, 48);
            w_u16(owner + 5752, 48);
            w_u16(owner + 5754, 48);
            w_u16(owner + 5756, 48);
            w_u32(owner + 5712, 2);
            w_u16(owner + 5718, 99);
            w_u16(owner + 5722, 96);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5726, 99);
            w_u16(owner + 5728, 96);
            w_u16(owner + 5758, 0);
            w_u16(owner + 5760, 96);
            resource = r_s16(selection);
            value = r_u8(0x8010B400 + resource * 8 + 7);
            if (value == 2)
            {
                w_u32(owner + 3896, 1);
                w_u32(owner + 5784, 1);
                resource002_sub_800edcc0(kind);
            }
            else if (value == 1)
            {
                state = r_u32(0x80112DC4);
                w_u32(state + 276, 0x4000);
            }
            state = r_u32(0x80112DC4);
            first = r_u32(state + 176);
            second = r_u32(state + 180);
            return sub_8002cc44(5, first, second);
        case 0x800F04E8:
            owner = r_u32(0x80112DB0);
            w_u16(owner + 5718, 70);
            w_u16(owner + 5722, 192);
            selection = r_u32(owner + 6300);
            w_u16(owner + 5720, 128);
            w_u16(owner + 5752, 128);
            w_u32(owner + 5712, 1);
            w_u16(owner + 5716, 96);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5754, 96);
            resource = r_s16(selection);
            value = r_u8(0x8010B400 + resource * 8 + 7);
            if (value == 2)
            {
                w_u32(owner + 3896, 1);
                w_u32(owner + 5784, 1);
                return resource002_sub_800edcc0(kind);
            }
            state = r_u32(0x80112DC4);
            w_u32(state + 276, 0x4000);
            return 0x4000;
        case 0x800F056C:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 5712, 3);
            w_u16(owner + 5716, 52);
            w_u16(owner + 5724, 221);
            w_u16(owner + 5722, 96);
            w_u16(owner + 5730, 96);
            w_u16(owner + 5756, 112);
            w_u16(owner + 5734, 115);
            w_u16(owner + 5736, 64);
            w_u16(owner + 5768, 64);
            w_u16(owner + 5770, 64);
            w_u16(owner + 5718, 93);
            w_u16(owner + 5720, 48);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5752, 48);
            w_u16(owner + 5754, 48);
            w_u16(owner + 5726, 93);
            w_u16(owner + 5728, 48);
            w_u16(owner + 5758, 0);
            w_u16(owner + 5760, 48);
            w_u16(owner + 5762, 48);
            w_u16(owner + 5732, 128);
            w_u16(owner + 5738, 128);
            w_u16(owner + 5764, 48);
            w_u16(owner + 5766, 0);
            w_u32(owner + 3896, 1);
            w_u32(owner + 5784, 1);
            return resource002_sub_800edcc0(kind);
        case 0x800F0620:
            owner = r_u32(0x80112DB0);
            w_u16(owner + 5716, 120);
            w_u16(owner + 5718, 87);
            w_u32(owner + 5712, 1);
            w_u16(owner + 5720, 80);
            w_u16(owner + 5722, 160);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5752, 80);
            w_u16(owner + 5754, 80);
            w_u32(owner + 3896, 1);
            w_u32(owner + 5784, 1);
            return resource002_sub_800edcc0(kind);
        case 0x800F067C:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 5784, 1);
            return 1;
        case 0x800F0690:
            owner = r_u32(0x80112DB0);
            w_u32(owner + 5712, 1);
            w_u16(owner + 5718, 100);
            w_u16(owner + 5716, 96);
            w_u16(owner + 5720, 128);
            w_u16(owner + 5722, 192);
            w_u16(owner + 5748, 0);
            w_u16(owner + 5750, 0);
            w_u16(owner + 5752, 128);
            w_u16(owner + 5754, 96);
            state = r_u32(0x80112DC4);
            w_u32(state + 276, 0x4000);
            return 0x4000;
        case 0x800F06E4:
            state = r_u32(0x80112DC4);
            owner = r_u32(0x800BCE60);
            w_u32(state + 284, 1);
            value = r_u8(owner + 2398);
            w_u8(owner + 2398, value + 1);
            owner = r_u32(0x800BCE60);
            w_u16(owner + 2388, 2);
            return 2;
        case 0x800F0718:
            return target;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 selection layout branch 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800ec6b4(uint32 rectangle, uint32 source_rectangles, uint32 palette, uint32 destination_rectangles, uint32 resource, uint32 count, const uint8 *native_rectangle, const uint8 *native_palette)
{
    uint32 owner, entries, table, value, base, index, offset, bound;

    FUNCTION_MARKER(0x800EC6B4, "ACE3.BIN");
    owner = arena_alloc(0xCDCC);
    w_u32(0x80112DFC, owner);
    entries = sub_80027338(resource, 0);
    owner = r_u32(0x80112DFC);
    value = r_u32(entries);
    w_u32(owner + 420, value);
    value = r_u32(entries + 8);
    table = r_u32(owner + 420);
    w_u32(owner + 428, value);
    bound = r_u32(table);
    if ((sint32)bound > 0)
    {
        index = 0;
        offset = 0;
        do
        {
            table = r_u32(owner + 420);
            base = r_u32(entries + 4);
            table += offset;
            value = r_u32(table + 12);
            w_u32(table + 12, value + base);
            table = r_u32(owner + 420);
            ++index;
            bound = r_u32(table);
            offset += 12;
        } while ((sint32)index < (sint32)bound);
    }
    owner = r_u32(0x80112DFC);
    table = r_u32(owner + 428);
    bound = r_u32(table);
    if ((sint32)bound > 0)
    {
        index = 0;
        do
        {
            table = r_u32(owner + 428) + index * 4;
            value = r_u32(table + 516);
            base = r_u32(entries + 12);
            w_u32(table + 516, value + base);
            table = r_u32(owner + 428);
            bound = r_u32(table);
            ++index;
        } while ((sint32)index < (sint32)bound);
    }
    resource002_sub_800ec4ec(rectangle, source_rectangles, palette, destination_rectangles, count, native_rectangle, native_palette);
    owner = r_u32(0x80112DFC);
    table = r_u32(owner + 420);
    return r_u32(table);
}

uint32 resource002_sub_800edcc0(uint32 kind)
{
    uint8 rectangle[8], palette[8];
    uint32 width, height, target, owner, resource, count, result;

    FUNCTION_MARKER(0x800EDCC0, "ACE3.BIN");
    /* Initializes dimensions omitted by the original default branch */
    width = 0;
    height = 0;
    if (kind < 10)
    {
        target = r_u32(0x800CCEC0 + kind * 4);
        switch (target)
        {
            case 0x800EDD18:
                width = 80;
                height = 80;
                break;
            case 0x800EDD24:
                width = 144;
                height = 64;
                break;
            case 0x800EDD34:
                width = 128;
                height = 96;
                break;
            case 0x800EDD44:
                width = 160;
                height = 64;
                break;
            case 0x800EDD54:
                width = 160;
                height = 80;
                break;
            case 0x800EDD64:
                break;
            default:
#ifndef NDEBUG
                fprintf(stderr, "Unbound resource002 upload dimension branch 0x%08X\n", target);
#endif
                abort();
        }
    }
    owner = r_u32(0x80112DB0);
    xport_store_le16(rectangle, 896);
    xport_store_le16(rectangle + 2, 256);
    xport_store_le16(rectangle + 4, width);
    xport_store_le16(rectangle + 6, height);
    xport_store_le16(palette, 768);
    xport_store_le16(palette + 2, 1);
    xport_store_le16(palette + 4, 256);
    xport_store_le16(palette + 6, 1);
    resource = r_u32(owner + 3892);
    count = r_u32(owner + 5712);
    result = resource002_sub_800ec6b4(0, owner + 5748, 0, owner + 5716, resource, count, rectangle, palette);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3900, result);
    return result;
}

uint32 resource002_sub_800ec4ec(uint32 rectangle, uint32 source_rectangles, uint32 palette, uint32 destination_rectangles, uint32 count, const uint8 *native_rectangle, const uint8 *native_palette)
{
    uint32 owner, stage, word, index, source, destination, address, offset, shift;
    uint32 high, low, words[2], game, table, value;
    const uint8 *native_source;

    FUNCTION_MARKER(0x800EC4EC, "ACE3.BIN");
    owner = r_u32(0x80112DFC);
    for (stage = 0; stage < 2; ++stage)
    {
        source = stage == 0 ? palette : rectangle;
        native_source = stage == 0 ? native_palette : native_rectangle;
        destination = owner + 336 + stage * 8;
        for (word = 0; word < 2; ++word)
        {
            if (native_source != 0)
                words[word] = xport_load_le32(native_source + word * 4);
            else
            {
                address = source + word * 4 + 3;
                offset = address & 3;
                high = r_masked_u32(address & ~3, (1 << (offset + 1)) - 1);
                high <<= 24 - offset * 8;
                address = source + word * 4;
                offset = address & 3;
                shift = offset * 8;
                low = r_masked_u32(address & ~3, (15 << offset) & 15);
                words[word] = (high & ~(0xFFFFFFFF >> shift)) | (low >> shift);
            }
        }
        for (word = 0; word < 2; ++word)
        {
            address = destination + word * 4 + 3;
            offset = address & 3;
            w_masked_u32(address & ~3, words[word] >> (24 - offset * 8), (1 << (offset + 1)) - 1);
            address = destination + word * 4;
            offset = address & 3;
            w_masked_u32(address & ~3, words[word] << (offset * 8), (15 << offset) & 15);
        }
    }
    w_u32(owner + 416, count);
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        owner = r_u32(0x80112DFC);
        for (stage = 0; stage < 2; ++stage)
        {
            source = (stage == 0 ? source_rectangles : destination_rectangles) + index * 8;
            destination = owner + (stage == 0 ? 352 : 384) + index * 8;
            for (word = 0; word < 2; ++word)
            {
                address = source + word * 4 + 3;
                offset = address & 3;
                high = r_masked_u32(address & ~3, (1 << (offset + 1)) - 1);
                high <<= 24 - offset * 8;
                address = source + word * 4;
                offset = address & 3;
                shift = offset * 8;
                low = r_masked_u32(address & ~3, (15 << offset) & 15);
                words[word] = (high & ~(0xFFFFFFFF >> shift)) | (low >> shift);
            }
            for (word = 0; word < 2; ++word)
            {
                address = destination + word * 4 + 3;
                offset = address & 3;
                w_masked_u32(address & ~3, words[word] >> (24 - offset * 8), (1 << (offset + 1)) - 1);
                address = destination + word * 4;
                offset = address & 3;
                w_masked_u32(address & ~3, words[word] << (offset * 8), (15 << offset) & 15);
            }
        }
    }
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112DFC);
    value = r_u32(game + 4);
    table = r_u32(owner + 420);
    w_u32(owner, value);
    w_u32(owner + 424, table + 4);
    sub_800a8318(rectangle, 0, 0, 0, (uint8 *)native_rectangle);
    for (index = 254; (sint32)index >= 0; --index)
    {
    }
    owner = r_u32(0x80112DFC);
    table = r_u32(owner + 428);
    sub_800a8440(palette, table + 4, (uint8 *)native_palette, 0, 0);
    value = native_rectangle != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_rectangle + 4) : r_s16(rectangle + 4);
    owner = r_u32(0x80112DFC);
    if ((sint32)value < 0)
        value += 7;
    w_u16(owner + 432, (uint32)((sint32)value >> 3));
    value = native_rectangle != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_rectangle + 6) : r_s16(rectangle + 6);
    if ((sint32)value < 0)
        value += 7;
    w_u16(owner + 434, (uint32)((sint32)value >> 3));
    w_u16(owner + 436, 4);
    w_u16(owner + 438, 4);
    w_u32(owner + 440, 0);
    w_u16(owner + 8, 4);
    w_u16(owner + 10, 0);
    resource002_sub_800ec340();
    owner = r_u32(0x80112DFC);
    value = owner + 0x8000;
    w_u32(value + 19908, 0);
    w_u32(value + 19912, 0);
    return value;
}

uint32 resource002_sub_800ec340(void)
{
    uint32 bank, owner, count, packet, index, screen, texture, a, b, value;

    FUNCTION_MARKER(0x800EC340, "ACE3.BIN");
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DFC);
        count = r_u32(owner + 416);
        packet = owner + 26 + bank * 160;
        if ((sint32)count <= 0)
            continue;
        index = 0;
        do
        {
            owner = r_u32(0x80112DFC);
            w_u8(packet - 11, 9);
            w_u8(packet - 7, 44);
            screen = owner + 384 + index * 8;
            a = r_u16(screen);
            w_u16(packet - 6, a);
            a = r_u16(screen + 2);
            w_u16(packet - 4, a);
            a = r_u16(screen);
            b = r_u16(screen + 4);
            w_u16(packet + 2, a + b);
            a = r_u16(screen + 2);
            w_u16(packet + 4, a);
            a = r_u16(screen);
            w_u16(packet + 10, a);
            a = r_u16(screen + 2);
            b = r_u16(screen + 6);
            w_u16(packet + 12, a + b);
            a = r_u16(screen);
            b = r_u16(screen + 4);
            w_u16(packet + 18, a + b);
            a = r_u16(screen + 2);
            b = r_u16(screen + 6);
            texture = owner + 352 + index * 8;
            w_u16(packet + 20, a + b);
            a = r_u8(texture);
            w_u8(packet - 2, a);
            a = r_u8(texture + 2);
            w_u8(packet - 1, a);
            a = r_u8(texture);
            b = r_u8(texture + 4);
            w_u8(packet + 6, a + b);
            a = r_u8(texture + 2);
            w_u8(packet + 7, a);
            a = r_u8(texture);
            w_u8(packet + 14, a);
            a = r_u8(texture + 2);
            b = r_u8(texture + 6);
            w_u8(packet + 15, a + b);
            a = r_u8(texture);
            b = r_u8(texture + 4);
            w_u8(packet + 22, a + b);
            a = r_u8(texture + 2);
            b = r_u8(texture + 6);
            w_u8(packet - 10, 128);
            w_u8(packet - 9, 128);
            w_u8(packet - 8, 128);
            w_u8(packet + 23, a + b);
            owner = r_u32(0x80112DFC);
            w_u16(packet + 8, 158);
            a = r_u16(owner + 338);
            b = r_u16(owner + 336);
            value = (a << 6) | ((b >> 4) & 63);
            w_u16(packet, value);
            count = r_u32(owner + 416);
            ++index;
            packet += 40;
        } while ((sint32)index < (sint32)count);
    }
    return 0;
}

uint32 sub_8002d6e0(void)
{
    FUNCTION_MARKER(0x8002D6E0, "ACE3.BIN");
    return 0;
}

uint32 resource002_sub_800edc80(uint32 table, uint32 resource, uint32 count)
{
    uint32 index;

    FUNCTION_MARKER(0x800EDC80, "ACE3.BIN");
    if ((sint32)count <= 0)
        return 0;
    resource = (uint32)(sint32)(sint16)resource;
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        if ((uint32)r_s16(table) == resource)
            return index;
        table += 2;
    }
    return 0;
}

uint32 resource002_sub_800ece38(uint32 resource, uint32 bytes)
{
    uint32 temporary, entries, count;

    FUNCTION_MARKER(0x800ECE38, "ACE3.BIN");
    temporary = arena_alloc(bytes);
    if (r_u32(resource) == 0x1A7A6C55)
    {
        sub_800afd08(resource, temporary);
        entries = sub_80027338(temporary, 0);
        count = r_u32(temporary);
    }
    else
    {
        entries = sub_80027338(resource, 0);
        count = r_u32(resource);
    }
    sub_8002722c(entries, count);
    DrawSync(0);
    arena_set_cursor(temporary);
    return 1;
}

uint32 resource002_sub_800eddf4(void)
{
    uint32 owner, selection, id, resource, value, bank, base, packet, index;

    FUNCTION_MARKER(0x800EDDF4, "ACE3.BIN");
    owner = r_u32(0x80112DB0);
    w_u16(owner + 5810, 0xFFFF);
    w_u8(owner + 5809, 0);
    resource002_sub_800def94(1, 0);
    resource002_sub_800def94(2, 0);
    resource002_sub_800dee08(3, 0, 0, 0);
    resource002_sub_800dee08(4, 0, 0, 0);
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    resource = r_s16(0x8010B400 + id * 8);
    value = resource002_sub_800edc80(0x8010B37C, resource, 6);
    owner = r_u32(0x80112DB0);
    w_u8(owner + 5808, value);
    for (bank = 0; bank < 2; ++bank)
    {
        owner = r_u32(0x80112DB0);
        base = owner + 5812 + bank * 220;
        sub_800ac7f8(owner + 6252 + bank * 24, 0, 0, 0, 0);
        packet = base + 13;
        for (index = 0; index < 11; ++index)
        {
            w_u8(packet - 10, 4);
            w_u8(packet - 6, 100);
            w_u8(packet - 9, 128);
            w_u8(packet - 8, 128);
            w_u8(packet - 7, 128);
            w_u16(packet - 5, 112);
            w_u16(packet - 3, 88 + index * 24);
            w_u16(packet + 3, 135);
            w_u16(packet + 5, 18);
            w_u8(packet - 1, 0);
            w_u8(packet, index * 18);
            packet += 20;
        }
    }
    return sub_8002db4c(62);
}

uint32 resource002_sub_800f1430(void)
{
    uint8 capture_rectangle[8], move_rectangle[8];
    uint32 game, context, resource, entries, state, data, owner, pointer, selection;
    uint32 id, menu, kind, table, count, first, second, index, buffer;

    FUNCTION_MARKER(0x800F1430, "ACE3.BIN");
    resource002_sub_800ee9e4();
    game = r_u32(0x800BCE60);
    if (r_u32(game + 68) != 0)
        return 0;
    context = r_u32(0x800BCE50);
    resource = r_u32(context + 96);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x80112DC4);
    w_u32(state + 40, resource);
    data = r_u32(entries + 12);
    if (r_u32(data) != 0)
        sub_8002d6e0();
    owner = r_u32(0x80112DB0);
    data = r_u32(entries);
    pointer = r_u32(0x80112DC0);
    w_u32(owner + 3892, data);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    menu = r_u32(pointer + 92);
    kind = r_u8(0x8010B400 + id * 8 + 5);
    w_u8(menu + 51, kind);
    table = r_u32(entries + 4);
    state = r_u32(0x80112DC4);
    count = r_u32(table);
    first = r_u32(0x800CCF60);
    first = r_u32(0x800CCF60);
    second = r_u32(0x800CCF64);
    second = r_u32(0x800CCF64);
    xport_store_le32(capture_rectangle, first);
    xport_store_le32(capture_rectangle + 4, second);
    w_u32(state + 176, 0);
    w_u32(state + 180, 0);
    table = sub_80027338(r_u32(entries + 4), 0);
    first = r_u32(0x800CCF68);
    first = r_u32(0x800CCF68);
    second = r_u32(0x800CCF6C);
    second = r_u32(0x800CCF6C);
    xport_store_le32(move_rectangle, first);
    xport_store_le32(move_rectangle + 4, second);
    sub_800a8500(0, 832, 256, move_rectangle);
    DrawSync(0);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 5780, 0);
    if (count >= 1 && count <= 4)
    {
        for (index = 0; index < count; ++index)
            resource002_sub_800ecf88(r_u32(table + index * 4));
    }
    data = r_u32(entries + 8);
    if (r_u32(data) != 0)
    {
        resource002_sub_800ece38(data, 0x4000);
        resource002_sub_800eddf4();
    }
    arena_set_cursor(r_u32(table));
    buffer = arena_alloc(0x8100);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 5780, buffer);
    sub_800a84a0(0, buffer, capture_rectangle, 0, 0);
    DrawSync(0);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 6);
    return 6;
}

uint32 resource002_sub_800f2240(void)
{
    uint32 game, owner, bits, selection, id, resource, state, pointer, menu;

    FUNCTION_MARKER(0x800F2240, "ACE3.BIN");
    resource002_sub_800ee9e4();
    game = r_u32(0x800BCE60);
    if ((r_u32(game + 300) & 16) != 0)
        resource002_sub_800dee08(1, 0, 0, 0);
    if (resource002_sub_800dee84(1) != 0)
        return 1;
    owner = r_u32(0x80112DB0);
    bits = r_u32(owner + 3888);
    resource002_sub_800dedf0(0, bits);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 5784, 1);
    selection = r_u32(owner + 6300);
    id = r_s16(selection - 4);
    resource = r_s16(0x8010B400 + id * 8);
    if (resource == 0x135)
    {
        state = r_u32(0x80112DC4);
        w_u32(state + 300, 0x916);
    }
    else if (resource == 0x10E || resource == 0x14B || resource == 0x15D)
    {
        state = r_u32(0x80112DC4);
        w_u32(state + 300, 0x914);
    }
    else
    {
        owner = r_u32(0x80112DB0);
        w_u32(owner + 5784, 0);
    }
    if (resource002_sub_800f072c() != 0)
    {
        resource002_sub_800dedf0(0, 0x808000);
        resource002_sub_800dee08(3, 1, 0, 10);
        pointer = r_u32(0x80112DC0);
        menu = r_u32(pointer + 92);
        w_u8(menu + 50, 1);
    }
    else
        resource002_sub_800dee08(3, 0, 0, 0);
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 13);
    owner = r_u32(0x80112DB0);
    w_u32(owner + 3908, 180);
    return 180;
}

uint32 resource002_sub_800f23cc(void)
{
    uint32 transition, owner, state, result, flag, timer, value, game;
    uint32 selection, id, kind, resource, target;

    FUNCTION_MARKER(0x800F23CC, "ACE3.BIN");
    transition = resource002_sub_800dee84(3);
    resource002_sub_800ee9e4();
    owner = r_u32(0x80112DB0);
    result = r_u32(owner + 5784);
    if (result != 0)
    {
        if (sub_8002b59c() == 0)
        {
            owner = r_u32(0x80112DB0);
            timer = r_u32(owner + 3908);
            w_u32(owner + 3908, timer - 1);
            return 0;
        }
        owner = r_u32(0x80112DB0);
        flag = r_u32(owner + 5784);
        result = 0x80110000;
        if (flag != 0)
        {
            state = r_u32(0x80112DC4);
            resource = r_u32(state + 300);
            value = sub_8002cae4(resource);
            if (value == 0xFFFFFFFF)
                result = resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            else
            {
                game = r_u32(0x800BCE60);
                result = r_u32(game + 300) & 16;
                if (result != 0)
                {
                    resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
                    result = resource_init_queue(0, 0);
                }
            }
        }
    }
    if (transition != 0)
        return result;
    resource002_sub_800f07b0();
    resource002_sub_800f07f4();
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    result = resource002_sub_800f00a8(kind);
    owner = r_u32(0x80112DB0);
    flag = r_u32(owner + 12);
    w_u32(owner + 28, result);
    if (flag != 0)
    {
        selection = r_u32(owner + 6300);
        id = r_s16(selection);
        kind = r_u8(0x8010B400 + id * 8 + 6);
        if (kind != 10)
        {
            if (kind >= 12)
                return 0;
            target = r_u32(0x800CCF78 + kind * 4);
            switch (target)
            {
                case 0x800F2588:
                    owner = r_u32(0x80112DB0);
                    value = r_u32(owner + 36);
                    if (value < 3)
                    {
                        state = r_u32(0x80112DC4);
                        w_u16(state + 134, 14);
                        w_u32(state + 420, 30);
                        return 30;
                    }
                    break;
                case 0x800F25C0:
                    owner = r_u32(0x80112DB0);
                    value = r_u32(owner + 36);
                    if (value >= 3)
                    {
                        state = r_u32(0x80112DC4);
                        w_u16(state + 134, 14);
                        w_u32(owner + 16, 30);
                        return 30;
                    }
                    break;
                case 0x800F2604:
                    return target;
                default:
#ifndef NDEBUG
                    fprintf(stderr, "Unbound resource002 restart branch 0x%08X\n", target);
#endif
                    abort();
            }
            state = r_u32(0x80112DC4);
            w_u16(state + 134, 5);
            return 5;
        }
    }
    state = r_u32(0x80112DC4);
    w_u16(state + 134, 14);
    w_u32(state + 420, 30);
    return 0;
}

uint32 resource002_sub_800f2618(void)
{
    uint32 owner, timer, selection, id, kind, state, phase, value, item;

    FUNCTION_MARKER(0x800F2618, "ACE3.BIN");
    resource002_sub_800ee9e4();
    owner = r_u32(0x80112DB0);
    timer = r_u32(owner + 16);
    if ((sint32)timer > 0)
    {
        w_u32(owner + 16, timer - 1);
        return timer - 1;
    }
    if (r_u32(owner + 12) != 0)
    {
        selection = r_u32(owner + 6300);
        id = r_s16(selection);
        kind = r_u8(0x8010B400 + id * 8 + 6);
        if (kind != 10)
        {
            state = r_u32(0x80112DC4);
            w_u16(state + 134, 4);
            value = r_u32(owner + 40);
            item = r_u32(owner + 28);
            w_u32(owner + 40, 1 - value);
            return resource002_sub_800debb8(0, item);
        }
    }
    state = r_u32(0x80112DC4);
    phase = r_u16(state + 134);
    w_u16(state + 134, phase + 1);
    resource002_sub_800decd4(0, 15);
    return sub_8002db4c(91);
}

uint32 resource002_sub_800f1d04(void)
{
    uint8 rectangle[8];
    uint32 display_carrier;
    uint32 owner, selection, id, kind, state, count, index, packet, ordering, screen, texture;
    uint32 value, result, game, table, byte, first, second, pixels, slot, item, other;

    FUNCTION_MARKER(0x800F1D04, "ACE3.BIN");
    resource002_sub_800ee9e4();
    display_carrier = 0x80110000;
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    if (kind == 8)
        resource002_sub_800ee570();
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    if (kind == 11)
        resource002_sub_800ee0d8();
    state = r_u32(0x80112DC4);
    if (r_u32(state + 276) != 0)
    {
        owner = r_u32(0x80112DB0);
        selection = r_u32(owner + 6300);
        id = r_s16(selection);
        kind = r_u8(0x8010B400 + id * 8 + 6);
        resource002_sub_800fd408(kind == 9 ? 64 : 16);
        state = r_u32(0x80112DC4);
        if (r_u32(state + 296) == 0)
        {
            owner = r_u32(0x80112DB0);
            count = r_u32(owner + 5712);
            index = 0;
            while ((sint32)index < (sint32)count)
            {
                owner = r_u32(0x80112DB0);
                state = r_u32(0x80112DC4);
                screen = owner + 5716 + index * 8;
                texture = owner + 5748 + index * 8;
                ordering = r_u32(state + 8);
                packet = r_u32(state + 628);
                result = resource002_sub_800fd48c(packet, ordering + 8, screen, texture);
                state = r_u32(0x80112DC4);
                owner = r_u32(0x80112DB0);
                w_u32(state + 628, result);
                count = r_u32(owner + 5712);
                ++index;
                display_carrier = 5716 + index * 8;
            }
        }
    }
    value = resource002_sub_800ef4f4(display_carrier);
    if (value == 1)
    {
        owner = r_u32(0x80112DB0);
        w_u32(owner + 3896, 2);
    }
    else if (value == 2)
    {
        resource002_sub_800ecd20();
        owner = r_u32(0x80112DB0);
        state = r_u32(0x80112DC4);
        w_u32(owner + 3896, 0);
        w_u32(state + 296, 1);
    }
    owner = r_u32(0x80112DB0);
    if (r_u32(owner + 3896) == 1)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 300) & 16) != 0)
            w_u32(owner + 3896, 2);
    }
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    packet = r_u32(state + 628);
    result = sub_8002d3f4(ordering, packet);
    state = r_u32(0x80112DC4);
    owner = r_u32(0x80112DB0);
    w_u32(state + 628, result);
    value = r_s16(owner + 2);
    if (value != 0 && r_u32(state + 296) != 1)
    {
        result = r_u32(owner + 16) + 1;
        w_u32(owner + 16, result);
        return result;
    }
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    w_u16(state + 134, 12);
    selection = r_u32(owner + 6300);
    other = r_s16(selection);
    kind = r_u8(0x8010B400 + other * 8 + 6);
    w_u32(owner + 36, kind);
    game = r_u32(0x800BCE60);
    table = game + id + 3480;
    byte = r_u8(table);
    if (byte == 0)
    {
        selection = r_u32(owner + 6300);
        value = r_s16(selection);
        if (value != 88 && value != 167 && value != 127 && value != 149)
            w_u8(table, 1);
    }
    else if (byte == 1)
        w_u8(table, 2);
    owner = r_u32(0x80112DB0);
    first = r_u32(0x800CCF68);
    first = r_u32(0x800CCF68);
    second = r_u32(0x800CCF6C);
    second = r_u32(0x800CCF6C);
    xport_store_le32(rectangle, first);
    xport_store_le32(rectangle + 4, second);
    pixels = r_u32(owner + 5780);
    if (pixels != 0)
    {
        sub_800a8440(0, pixels, rectangle, 0, 0);
        DrawSync(0);
    }
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    value = r_s16(0x8010B400 + id * 8);
    if (value == 0x153)
    {
        game = r_u32(0x800BCE60);
        if ((r_u32(game + 2412) & 32) != 0)
        {
            state = r_u32(0x80112DC4);
            w_u32(state + 148, 0x60000005);
            w_u32(game + 928, 3);
            w_u8(game + 936, 29);
            other = r_u32(0x800BCE60);
            w_u16(game + 2388, 1);
            w_u8(other + 2398, 0);
            return 1;
        }
    }
    owner = r_u32(0x80112DB0);
    selection = r_u32(owner + 6300);
    w_u32(owner + 6300, selection + 4);
    value = r_u16(owner);
    game = r_u32(0x800BCE60);
    w_u16(owner, value + 1);
    value = r_u8(game + 2398);
    w_u8(game + 2398, value + 1);
    owner = r_u32(0x80112DB0);
    item = r_u32(owner + 28);
    w_u16(owner + 2, 1);
    w_u32(owner + 16, 0);
    resource002_sub_800decd4(0, item);
    state = r_u32(0x80112DC4);
    w_u32(state + 420, 60);
    resource002_sub_800dee08(1, 3, 0xFFFFFFFF, 0xFFFFFFFF);
    owner = r_u32(0x80112DB0);
    slot = r_u32(owner + 5804);
    if (slot != 0)
    {
        resource002_sub_800dee08(slot, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        owner = r_u32(0x80112DB0);
    }
    state = r_u32(0x80112DC4);
    w_u32(owner + 3896, 0);
    arena_set_cursor(r_u32(state + 40));
    owner = r_u32(0x80112DB0);
    value = r_u32(owner + 12);
    w_u32(owner + 12, value - 1);
    resource002_sub_800f07b0();
    sub_8002db4c(101);
    for (index = 0; index < 5; ++index)
    {
        state = r_u32(0x80112DC4);
        value = r_u32(state + 304 + index * 4);
        result = sound_set_voice_fade(value, 100, 30);
    }
    return result;
}

uint32 resource002_sub_800d1214(void)
{
    uint32 state, phase, target;

    FUNCTION_MARKER(0x800D1214, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    phase = r_s16(state + 134);
    target = r_u32(0x8010BA98 + phase * 4);
    switch (target)
    {
        case 0x800F0DC4:
            return resource002_sub_800f0dc4();
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
        case 0x800F2990:
            return resource002_sub_800f2990();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 detail callback 0x%08X\n", target);
#endif
            abort();
    }
}

uint32 resource002_sub_800fd48c(uint32 packet, uint32 ordering, uint32 screen, uint32 texture)
{
    uint32 a, b, owner, offset, c, tag, link;

    FUNCTION_MARKER(0x800FD48C, "ACE3.BIN");
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 44);
    w_u8(packet + 4, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 6, 128);
    a = r_u16(screen);
    w_u16(packet + 8, a);
    a = r_u16(screen + 2);
    w_u16(packet + 10, a);
    a = r_u16(screen);
    b = r_u16(screen + 4);
    w_u16(packet + 16, a + b);
    a = r_u16(screen + 2);
    w_u16(packet + 18, a);
    a = r_u16(screen);
    w_u16(packet + 24, a);
    a = r_u16(screen + 2);
    b = r_u16(screen + 6);
    w_u16(packet + 26, a + b);
    a = r_u16(screen);
    b = r_u16(screen + 4);
    w_u16(packet + 32, a + b);
    a = r_u16(screen + 2);
    b = r_u16(screen + 6);
    w_u16(packet + 34, a + b);
    a = r_u8(texture);
    w_u8(packet + 12, a);
    owner = r_u32(0x80112E18);
    a = r_u8(owner + 2);
    b = r_u8(texture + 2);
    offset = (1 - a) << 7;
    w_u8(packet + 13, b + offset);
    a = r_u8(texture);
    b = r_u8(texture + 4);
    w_u8(packet + 20, a + b);
    owner = r_u32(0x80112E18);
    a = r_u8(owner + 2);
    b = r_u8(texture + 2);
    offset = (1 - a) << 7;
    w_u8(packet + 21, b + offset);
    a = r_u8(texture);
    w_u8(packet + 28, a);
    owner = r_u32(0x80112E18);
    a = r_u8(texture + 6);
    b = r_u8(owner + 2);
    c = r_u8(texture + 2);
    offset = (1 - b) << 7;
    w_u8(packet + 29, a + c + offset);
    a = r_u8(texture);
    b = r_u8(texture + 4);
    w_u8(packet + 36, a + b);
    owner = r_u32(0x80112E18);
    a = r_u8(texture + 2);
    b = r_u8(texture + 6);
    c = r_u8(owner + 2);
    w_u16(packet + 22, 285);
    offset = (1 - c) << 7;
    w_u8(packet + 37, a + b + offset);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 40;
}

uint32 resource002_sub_800ee0d8(void)
{
    uint32 game, bank, owner, packet, page_packet, state, ordering, tag, link;
    uint32 selection, id, kind, index, value, count, row, item, result;
    sint32 position, half, remainder;

    FUNCTION_MARKER(0x800EE0D8, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    bank = r_u32(game + 4);
    owner = r_u32(0x80112DB0);
    packet = owner + 5812 + bank * 220;
    page_packet = owner + 6252 + bank * 24;
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    tag = r_u32(page_packet);
    link = r_u32(ordering);
    w_u32(page_packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (page_packet & 0xFFFFFF));
    selection = r_u32(owner + 6300);
    id = r_s16(selection);
    kind = r_u8(0x8010B400 + id * 8 + 6);
    page_packet += 12;
    if (kind != 8)
    {
        for (index = 0; index < 11; ++index)
        {
            position = r_s16(owner + 5810);
            remainder = position % 4;
            value = ((uint32)(remainder + 296) << 6) | 22;
            tag = r_u32(packet);
            w_u16(packet + 14, value);
            link = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            link = r_u32(ordering);
            w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
            packet += 20;
        }
        owner = r_u32(0x80112DB0);
        position = (sint16)r_u16(owner + 5810);
        half = (sint16)(position / 2);
        remainder = half - 2 * (half / 2);
        value = ((((uint32)remainder << 6) + 576) & 1023) >> 6;
        sub_800ac7f8(page_packet, 0, 0, value | 16, 0);
        tag = r_u32(page_packet);
        link = r_u32(ordering);
        w_u32(page_packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | (page_packet & 0xFFFFFF));
    }
    if (resource002_sub_800dee84(5) == 0)
    {
        owner = r_u32(0x80112DB0);
        row = r_u8(owner + 5808);
        count = r_u32(0x8010B388 + row * 20);
        position = r_s16(owner + 5810);
        if (position < (sint32)(count - 2))
        {
            w_u8(owner + 5809, 0);
            owner = r_u32(0x80112DB0);
            value = r_u16(owner + 5810);
            w_u16(owner + 5810, value + 1);
            resource002_sub_800dee08(5, 1, 0, 10);
        }
        else
        {
            state = r_u32(0x80112DC4);
            w_u32(state + 296, 1);
        }
    }
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    value = r_u8(0x8010B388 + row * 20 + item * 2 + 4);
    resource002_sub_800def94(1, value);
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    if (r_u8(0x8010B388 + row * 20 + item * 2 + 4) != 0 && resource002_sub_800dee84(3) == 0)
    {
        state = r_u32(0x80112DC4);
        if (r_u32(state + 296) == 0)
            resource002_sub_800dee08(3, 1, 0, 10);
        owner = r_u32(0x80112DB0);
        w_u8(owner + 5809, 1);
    }
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    value = r_u8(0x8010B388 + row * 20 + item * 2 + 5);
    resource002_sub_800def94(2, value);
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    if (r_u8(0x8010B388 + row * 20 + item * 2 + 5) != 0 && resource002_sub_800dee84(4) == 0)
    {
        state = r_u32(0x80112DC4);
        if (r_u32(state + 296) == 0)
            resource002_sub_800dee08(4, 1, 0, 10);
        owner = r_u32(0x80112DB0);
        w_u8(owner + 5809, 2);
    }
    owner = r_u32(0x80112DB0);
    value = r_u8(owner + 5809);
    resource002_sub_800def94(3, value);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 16;
    if (result != 0)
    {
        if (resource002_sub_800dee84(3) == 2)
            resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        if (resource002_sub_800dee84(4) == 2)
            resource002_sub_800dee08(4, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        return resource002_sub_800dee08(5, 3, 0xFFFFFFFF, 0xFFFFFFFF);
    }
    return result;
}

uint32 resource002_sub_800ee570(void)
{
    uint32 game, bank, owner, packet, page_packet, state, ordering, tag, link;
    uint32 index, value, count, row, item, result;
    sint32 position, half, remainder;

    FUNCTION_MARKER(0x800EE570, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    owner = r_u32(0x80112DB0);
    bank = r_u32(game + 4);
    packet = owner + 5812 + bank * 220;
    page_packet = owner + 6252 + bank * 24;
    state = r_u32(0x80112DC4);
    ordering = r_u32(state + 8);
    tag = r_u32(page_packet);
    link = r_u32(ordering);
    w_u32(page_packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (page_packet & 0xFFFFFF));
    page_packet += 12;
    for (index = 0; index < 11; ++index)
    {
        position = r_s16(owner + 5810);
        remainder = position % 4;
        value = ((uint32)(remainder + 296) << 6) | 22;
        tag = r_u32(packet);
        w_u16(packet + 14, value);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 20;
    }
    owner = r_u32(0x80112DB0);
    position = (sint16)r_u16(owner + 5810);
    half = (sint16)(position / 2);
    remainder = half - 2 * (half / 2);
    value = ((((uint32)remainder << 6) + 576) & 1023) >> 6;
    sub_800ac7f8(page_packet, 0, 0, value | 16, 0);
    tag = r_u32(page_packet);
    link = r_u32(ordering);
    w_u32(page_packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(ordering);
    w_u32(ordering, (link & 0xFF000000) | (page_packet & 0xFFFFFF));
    if (resource002_sub_800dee84(5) == 0)
    {
        owner = r_u32(0x80112DB0);
        row = r_u8(owner + 5808);
        count = r_u32(0x8010B388 + row * 20);
        position = r_s16(owner + 5810);
        if (position < (sint32)(count - 1))
        {
            w_u8(owner + 5809, 0);
            owner = r_u32(0x80112DB0);
            value = r_u16(owner + 5810);
            w_u16(owner + 5810, value + 1);
            resource002_sub_800dee08(5, 1, 0, 10);
        }
        else
        {
            state = r_u32(0x80112DC4);
            w_u32(state + 296, 1);
        }
    }
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    value = r_u8(0x8010B388 + row * 20 + item * 2 + 4);
    resource002_sub_800def94(1, value);
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    if (r_u8(0x8010B388 + row * 20 + item * 2 + 4) != 0 && resource002_sub_800dee84(3) == 0)
    {
        state = r_u32(0x80112DC4);
        if (r_u32(state + 296) == 0)
            resource002_sub_800dee08(3, 1, 0, 10);
        owner = r_u32(0x80112DB0);
        w_u8(owner + 5809, 1);
    }
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    value = r_u8(0x8010B388 + row * 20 + item * 2 + 5);
    resource002_sub_800def94(2, value);
    owner = r_u32(0x80112DB0);
    item = r_s16(owner + 5810);
    row = r_u8(owner + 5808);
    if (r_u8(0x8010B388 + row * 20 + item * 2 + 5) != 0 && resource002_sub_800dee84(4) == 0)
    {
        state = r_u32(0x80112DC4);
        if (r_u32(state + 296) == 0)
            resource002_sub_800dee08(4, 1, 0, 10);
        owner = r_u32(0x80112DB0);
        w_u8(owner + 5809, 2);
    }
    owner = r_u32(0x80112DB0);
    value = r_u8(owner + 5809);
    resource002_sub_800def94(3, value);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 16;
    if (result != 0)
    {
        if (resource002_sub_800dee84(3) == 2)
            resource002_sub_800dee08(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        if (resource002_sub_800dee84(4) == 2)
            resource002_sub_800dee08(4, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        return resource002_sub_800dee08(5, 3, 0xFFFFFFFF, 0xFFFFFFFF);
    }
    return result;
}

uint32 resource002_sub_800ecd28(uint32 context)
{
    FUNCTION_MARKER(0x800ECD28, "ACE3.BIN");
    return arena_set_cursor(r_u32(context + 96));
}

uint32 resource002_sub_800ecd20(void)
{
    FUNCTION_MARKER(0x800ECD20, "ACE3.BIN");
    return resource002_sub_800ecd28(r_u32(0x800BCE50));
}

uint32 resource002_sub_800fce40(void)
{
    uint8 rectangle[8];
    uint32 game, value, root, owner, buffer, state;

    FUNCTION_MARKER(0x800FCE40, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game);
    root = r_u32(0x800BCE48);
    w_u32(game, value | 0x10000000);
    w_u32(root + 20, 0);
    owner = arena_alloc(68);
    w_u32(0x800BD044, owner);
    buffer = arena_alloc(1024);
    owner = r_u32(0x800BD044);
    w_u32(owner + 4, buffer);
    buffer = arena_alloc(1024);
    owner = r_u32(0x800BD044);
    w_u32(owner + 8, buffer);
    state = r_u32(0x80112DC4);
    w_u32(owner, 0);
    w_u16(owner + 36, 832);
    w_u16(owner + 38, 256);
    w_u16(owner + 40, 832);
    w_u16(owner + 42, 384);
    w_u16(owner + 44, 192);
    w_u16(owner + 46, 128);
    w_u16(owner + 48, 0);
    w_u16(owner + 50, 0);
    value = r_u32(state + 300);
    if (value != 0)
        w_u32(owner + 52, value);
    owner = r_u32(0x800BD044);
    w_u32(owner + 56, 0);
    w_u32(owner + 60, 0);
    w_u32(owner + 64, 120);
    xport_store_le16(rectangle, 832);
    xport_store_le16(rectangle + 2, 256);
    xport_store_le16(rectangle + 4, 192);
    xport_store_le16(rectangle + 6, 128);
    sub_800a8318(0, 0, 0, 0, rectangle);
    xport_store_le16(rectangle, 832);
    xport_store_le16(rectangle + 2, 384);
    sub_800a8318(0, 0, 0, 0, rectangle);
    state = r_u32(0x80112DC4);
    w_u32(state + 276, 0x8000);
    w_u32(state + 280, 1);
    return 1;
}

uint32 resource002_sub_800fcf80(void)
{
    uint8 rectangle[8];
    uint32 game, value, owner, state;

    FUNCTION_MARKER(0x800FCF80, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u32(game);
    w_u32(game, value & 0xEFFFFFFF);
    xport_store_le16(rectangle + 2, 256);
    xport_store_le16(rectangle + 4, 192);
    xport_store_le16(rectangle, 832);
    xport_store_le16(rectangle + 6, 128);
    sub_800a8318(0, 0, 0, 0, rectangle);
    xport_store_le16(rectangle, 832);
    xport_store_le16(rectangle + 2, 384);
    sub_800a8318(0, 0, 0, 0, rectangle);
    owner = r_u32(0x80112E18);
    state = r_u32(0x80112DC4);
    w_u32(state + 296, 1);
    return arena_set_cursor(owner);
}

uint32 resource002_sub_800fd028(uint32 mask)
{
    FUNCTION_MARKER(0x800FD028, "ACE3.BIN");
    return resource002_sub_800fd030(r_u32(0x800BD044), mask);
}

uint32 resource002_sub_800fd408(uint32 mask)
{
    uint32 state, mode;

    FUNCTION_MARKER(0x800FD408, "ACE3.BIN");
    state = r_u32(0x80112DC4);
    mode = r_u32(state + 276);
    if (mode == 0x4000)
    {
        resource002_sub_800fce40();
        return 0;
    }
    if (mode == 0x10000)
    {
        resource002_sub_800fcf80();
        return 1;
    }
    if (resource002_sub_800fd028(mask) != 0)
    {
        state = r_u32(0x80112DC4);
        w_u32(state + 276, 0x10000);
    }
    return 0;
}

uint32 resource002_sub_800fd030(uint32 context, uint32 mask)
{
    uint8 rectangle[8];
    uint32 phase, arena, owner, state, value, game, index, resource, mode;
    sint32 x0, y0, x1, y1;

    FUNCTION_MARKER(0x800FD030, "ACE3.BIN");
    phase = r_u32(context);
    switch (phase)
    {
        case 0:
            arena = arena_peek(0);
            owner = r_u32(0x800BD044);
            value = r_u32(owner + 56) ? 0x1800 : 0x1000;
            state = r_u32(0x800BD044);
            x0 = r_s16(state + 36);
            x0 += r_s16(state + 48);
            y0 = r_s16(state + 38);
            y0 += r_s16(state + 50);
            x1 = r_s16(state + 40);
            x1 += r_s16(state + 48);
            y1 = r_s16(state + 42);
            y1 += r_s16(state + 50);
            resource = r_u32(owner + 52);
            mode = r_u32(owner + 56);
            resource002_sub_800fc764(resource, mode, arena, 0x12300, 0xC404, value, (uint32)x0, (uint32)y0, (uint32)x1, (uint32)y1);
            resource002_sub_800fc854();
            state = r_u32(0x800BD044);
            w_u32(state, 1);
            return 0;
        case 1:
            game = r_u32(0x800BCE60);
            if (!r_u32(game + 4) && resource002_sub_800fc89c(1))
            {
                state = r_u32(0x800BD044);
                w_u32(state, 2);
                movie_current_buffer();
                state = r_u32(0x800BCE48);
                w_u8(state + 40, 0);
                owner = r_u32(0x800BD044);
                state = r_u32(0x800BCE48);
                value = r_u8(owner + 56);
                w_u8(state + 41, value);
                state = r_u32(0x800BCE48);
                value = r_u16(state + 8);
                index = r_u16(state + 12);
                w_u16(state + 32, value);
                w_u16(state + 34, index);
                w_u8(state + 68, 1);
                w_u8(r_u32(0x800BCE48) + 69, 0);
                w_u8(r_u32(0x800BCE48) + 70, 0);
                w_u8(r_u32(0x800BCE48) + 71, 0);
            }
            owner = r_u32(0x80112E18);
            if (r_u8(owner + 4) != 1)
                return 0;
            movie_end();
            state = r_u32(0x800BD044);
            w_u32(state, 3);
            return 0;
        case 2:
            game = r_u32(0x800BCE60);
            if (r_u32(game + 300) & mask)
                w_u8(r_u32(0x80112E18) + 4, 1);
            owner = r_u32(0x80112E18);
            if (r_u8(owner + 4) == 1)
            {
                movie_end();
                state = r_u32(0x800BD044);
                w_u32(state, 3);
            }
            owner = r_u32(0x80112E18);
            if (!r_u32(owner + 0x11064))
            {
                if (resource002_sub_800fc970(1))
                    w_u32(r_u32(0x80112E18) + 0x11064, 1);
            }
            else if (r_u8(owner + 3) == 1)
            {
                resource002_sub_800fca70(owner, 0);
                movie_end();
                index = movie_current_buffer();
                state = r_u32(0x800BD044);
                owner = state + (index << 2);
                xport_store_le16(rectangle, r_u16(owner + 36));
                xport_store_le16(rectangle + 2, r_u16(owner + 38));
                xport_store_le16(rectangle + 6, r_u16(state + 46));
                if (r_u32(state + 56))
                {
                    value = (uint32)r_s16(state + 44);
                    value = (uint32)((sint32)((value << 1) + value) >> 1);
                }
                else
                    value = r_u16(state + 44);
                xport_store_le16(rectangle + 4, value);
                sub_800a8318(0, 0, 0, 0, rectangle);
                state = r_u32(0x800BD044);
                w_u32(state, 3);
                return 0;
            }
            movie_current_buffer();
            state = r_u32(0x800BCE48);
            w_u8(state + 40, 0);
            owner = r_u32(0x800BD044);
            state = r_u32(0x800BCE48);
            value = r_u8(owner + 56);
            w_u8(state + 41, value);
            state = r_u32(0x800BCE48);
            value = r_u16(state + 8);
            index = r_u16(state + 12);
            w_u16(state + 32, value);
            w_u16(state + 34, index);
            w_u8(state + 68, 1);
            w_u8(r_u32(0x800BCE48) + 69, 0);
            w_u8(r_u32(0x800BCE48) + 70, 0);
            w_u8(r_u32(0x800BCE48) + 71, 0);
            return 0;
        case 3:
            state = r_u32(0x800BD044);
            w_u32(state, 4);
            return 0;
        case 4:
            return 1;
        default:
            return 0;
    }
}

uint32 resource002_sub_800fc750(void)
{
    uint32 owner;

    FUNCTION_MARKER(0x800FC750, "ACE3.BIN");
    owner = r_u32(0x80112E18);
    w_u8(owner + 4, 1);
    return 1;
}

uint32 resource002_sub_800fc854(void)
{
    uint32 owner, resource, bytes, destination;

    FUNCTION_MARKER(0x800FC854, "ACE3.BIN");
#if AP_WEB
    owner = r_u32(0x80112E18);
    game_web_movie_prepare(r_u32(owner + 0x11058), owner, 1);
    return sub_8002c928(r_u32(owner + 0x11058), r_u32(owner + 0x11060), r_u32(owner + 0x1105C), 0x1000, 0x800FC750);
#endif
    owner = r_u32(0x80112E18);
    resource = r_u32(owner + 0x11058);
    destination = r_u32(owner + 0x11060);
    bytes = r_u32(owner + 0x1105C);
    return sub_8002c928(resource, destination, bytes, 0x1000, 0x800FC750);
}

uint32 resource002_sub_800fc89c(uint32 nonblocking)
{
    uint32 owner, remaining;

    FUNCTION_MARKER(0x800FC89C, "ACE3.BIN");
#if AP_WEB
    (void)nonblocking;
    return game_web_movie_poll(r_u32(0x80112E18));
#endif
    if (nonblocking)
    {
        owner = r_u32(0x80112E18);
        if (resource002_sub_800fcc7c(owner))
            return 1;
        owner = r_u32(0x80112E18);
        remaining = r_u32(owner + 0x11068) - 1;
        w_u32(owner + 0x11068, remaining);
        if ((sint32)remaining < 0)
            w_u8(owner + 4, 1);
        return 0;
    }
    remaining = 120;
    for (;;)
    {
        owner = r_u32(0x80112E18);
        if (resource002_sub_800fcc7c(owner))
            return 1;
        sub_8002c4dc();
        VSync(0);
        --remaining;
        if ((sint32)remaining < 0)
        {
            owner = r_u32(0x80112E18);
            w_u8(owner + 4, 1);
            return 1;
        }
    }
}

uint32 resource002_sub_800fca70(uint32 owner, uint32 nonblocking)
{
    uint32 remaining = 0x40000;
    uint32 value, state, index, coordinate;

    FUNCTION_MARKER(0x800FCA70, "ACE3.BIN");
    value = r_u8(owner + 3);
    if (nonblocking || value)
        return value;
    for (;;)
    {
        state = r_u32(0x80112E18);
        value = r_u8(state + 4);
        --remaining;
        if ((sint32)remaining <= 0 || value)
        {
            value = r_u8(owner + 2);
            w_u8(owner + 3, 1);
            value = value == 0;
            w_u8(owner + 2, value);
            coordinate = r_u16(owner + value * 8 + 56);
            index = r_u8(owner + 2);
            w_u16(owner + 72, coordinate);
            value = r_u16(owner + index * 8 + 58);
            w_u16(owner + 74, value);
        }
        value = r_u8(owner + 3);
        if (value)
            return value;
    }
}

uint32 resource002_sub_800fc970(uint32 mode)
{
    uint32 owner, index, format, input, output, remaining;
    sint32 words;

    FUNCTION_MARKER(0x800FC970, "ACE3.BIN");
#if AP_WEB
    (void)mode;
    return game_web_movie_decode(r_u32(0x80112E18));
#endif
    owner = r_u32(0x80112E18);
    if (r_u8(owner + 4))
        return 1;
    if (resource002_sub_800fcc7c(owner))
    {
        owner = r_u32(0x80112E18);
        w_u8(owner + 3, 0);
        owner = r_u32(0x80112E18);
        index = r_u8(owner);
        format = r_u8(owner + 5);
        input = r_u32(owner + index * 4 + 8);
        DecDCTin(psx_addr(input, 4 + ((uint32)r_u16(input) << 2)), format | 2);
        owner = r_u32(0x80112E18);
        index = r_u8(owner + 1);
        words = r_s16(owner + 0x11056);
        output = r_u32(owner + index * 4 + 16);
        DecDCTout(psx_addr(output, ((uint32)words & ~31) * 4), words);
        owner = r_u32(0x80112E18);
        resource002_sub_800fca70(owner, mode);
        owner = r_u32(0x80112E18);
        w_u32(owner + 0x11068, 300);
    }
    else
    {
        owner = r_u32(0x80112E18);
        remaining = r_u32(owner + 0x11068);
        if ((sint32)remaining > 0)
            w_u32(owner + 0x11068, remaining - 1);
        else
            w_u8(owner + 4, 1);
    }
    return 0;
}
