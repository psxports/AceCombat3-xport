#include "game_functions.h"
#include <stdlib.h>

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8009a944(uint32 velocity, uint32 out, uint32 cursor, uint32 model, uint32 vertices, uint32 relative, const uint32 *native_velocity, uint32 *native_out, const uint16 *native_relative)
{
    SVECTOR vector = {0};
    uint16 normal[3];
    uint8 matrix[32] = {0}, basis[32] = {0}, offset[6];
    uint32 words[5], index, column, a, b, c, maximum, best = 0x7FFFFFFF;
    uint32 scene, normals, list, kind, count, row, polygon;
    uint32 face, face_count, source_normal, initial_normal, row_flags, hit_flags, packed, depth, distance;
    uint32 first, second, third, fourth, chosen_normal, chosen_flags;
    int quad, clipped;
    FUNCTION_MARKER(0x8009A944, "ACE3.BIN");
    vector.vx = native_velocity ? (sint16)native_velocity[0] : (sint16)r_u16(velocity);
    vector.vy = native_velocity ? (sint16)native_velocity[1] : (sint16)r_u16(velocity + 4);
    vector.vz = native_velocity ? (sint16)native_velocity[2] : (sint16)r_u16(velocity + 8);
    a = VectorNormalSS(&vector, &vector);
    maximum = sub_80083d88(a);
    normal[0] = (uint16)vector.vx;
    normal[1] = (uint16)vector.vy;
    normal[2] = (uint16)vector.vz;
    sub_80099428(0, 0, matrix, normal);
    xport_store_le32(matrix + 28, 0);
    xport_store_le32(matrix + 24, 0);
    xport_store_le32(matrix + 20, 0);
    for (index = 0; index < 9; ++index)
        xport_store_le16(basis + index * 2, index % 4 == 0 ? 4096 : 0);
    for (index = 0; index < 5; ++index)
        words[index] = xport_load_le32(matrix + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (column = 0; column < 3; ++column)
    {
        a = xport_load_le16(basis + column * 2);
        b = xport_load_le16(basis + column * 2 + 6);
        c = xport_load_le16(basis + column * 2 + 12);
        xport_gte_write_data(9, a);
        xport_gte_write_data(10, b);
        xport_gte_write_data(11, c);
        xport_gte_execute(0x49E012);
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        xport_store_le16(basis + column * 2, a);
        xport_store_le16(basis + column * 2 + 6, b);
        xport_store_le16(basis + column * 2 + 12, c);
    }
    for (index = 0; index < 3; ++index)
        xport_gte_write_control(index + 5, xport_load_le32(matrix + 20 + index * 4));
    b = xport_load_le16(basis + 24);
    a = xport_load_le16(basis + 20);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, xport_load_le32(basis + 28));
    xport_gte_execute(0x480012);
    xport_store_le32(basis + 20, xport_gte_read_data(25));
    xport_store_le32(basis + 24, xport_gte_read_data(26));
    xport_store_le32(basis + 28, xport_gte_read_data(27));
    for (index = 0; index < 5; ++index)
        words[index] = xport_load_le32(basis + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, words[index]);
    for (index = 0; index < 3; ++index)
        xport_gte_write_control(index + 5, xport_load_le32(basis + 20 + index * 4));
    xport_gte_write_data(14, 0);
    if (native_relative)
    {
        xport_store_le16(offset, native_relative[0]);
        xport_store_le16(offset + 2, native_relative[1]);
        xport_store_le16(offset + 4, native_relative[2]);
    }
    sub_80099750(vertices, relative, cursor, native_relative ? offset : 0);
    scene = r_u32(0x800BCE54);
    list = r_u32(model + 16);
    normals = r_u32(scene + 604);
    for (kind = 0; kind < 2; ++kind)
    {
        if (kind != 0)
            list = r_u32(model + 12);
        count = r_u32(list);
        row = list + 4;
        for (polygon = 0; (sint32)polygon < (sint32)count; ++polygon, row += kind == 0 ? 48 : 36)
        {
            initial_normal = normals + r_u16(row + 4);
            xport_gte_write_data(0, r_u32(initial_normal));
            xport_gte_write_data(1, r_u32(initial_normal + 4));
            row_flags = r_u32(row);
            face_count = (row_flags & 0x200) ? 2 : 1;
            for (face = 0; face < face_count; ++face)
            {
                source_normal = initial_normal;
                if (face != 0)
                {
                    source_normal = normals + r_u16(row + 6);
                    xport_gte_write_data(0, r_u32(source_normal));
                    xport_gte_write_data(1, r_u32(source_normal + 4));
                }
                xport_gte_execute(0x486012);
                packed = r_u32(row + 16);
                a = xport_gte_read_data(9);
                b = xport_gte_read_data(10);
                depth = xport_gte_read_data(11);
                xport_gte_write_data(1, depth);
                xport_gte_write_data(0, (a & 0xFFFF) | (b << 16));
                if ((sint32)depth >= 0)
                    continue;
                first = cursor + (packed & 255) * 8;
                second = cursor + ((packed >> 8) & 255) * 8;
                third = cursor + ((packed >> 16) & 255) * 8;
                fourth = cursor + (packed >> 24) * 8;
                quad = (row_flags & 0x300) == 0;
                /* Fix015 defines the second triangle base even when its first normal was rejected */
                clipped = face != 0 ? collision_ray_clip(second, third, fourth, 0, depth, 0, 1, &distance) : collision_ray_clip(first, second, third, fourth, depth, quad, 0, &distance);
                if (!clipped)
                    continue;
                if (face == 0 && (row_flags & 0x200))
                    hit_flags = row_flags;
                else if (face != 0)
                    hit_flags = r_u32(row);
                else
                {
                    if (kind == 0)
                    {
                        source_normal = normals + r_u16(row + 4);
                        hit_flags = r_u32(row);
                    }
                    else
                    {
                        a = r_u16(row + 4);
                        hit_flags = r_u32(row);
                        source_normal = normals + a;
                    }
                }
                if ((sint32)distance >= 0 && (sint32)maximum >= (sint32)distance && (sint32)distance < (sint32)best)
                {
                    best = distance;
                    chosen_normal = source_normal;
                    chosen_flags = hit_flags;
                }
            }
        }
    }
    if ((sint32)best > 0x7FFF)
    {
        w_u32(0x800BD0C0, 0);
        if (native_out)
        {
            native_out[2] = 0;
            native_out[1] = 0;
            native_out[0] = 0;
        }
        else
        {
            w_u32(out + 8, 0);
            w_u32(out + 4, 0);
            w_u32(out, 0);
        }
    }
    else
    {
        a = (uint32)(sint32)vector.vx * best;
        if (native_out)
            native_out[0] = a;
        else
            w_u32(out, a);
        a = (uint32)(sint32)vector.vy * best;
        if (native_out)
            native_out[1] = a;
        else
            w_u32(out + 4, a);
        a = (uint32)(sint32)vector.vz * best;
        w_u32(0x800BD0C0, chosen_normal);
        w_u32(0x800BD0C4, chosen_flags);
        if (native_out)
            native_out[2] = a;
        else
            w_u32(out + 8, a);
    }
    return best;
}

uint32 sub_80030528(uint32 data)
{
    uint32 pose[6] = {0}, selected[4], mapping[4], sound_position[3];
    uint32 record, index, count, slot, state, value;
    FUNCTION_MARKER(0x80030528, "ACE3.BIN");
    pose[0] = r_u32(data + 12) << 12;
    pose[1] = r_u32(data + 16) << 12;
    pose[2] = r_u32(data + 20) << 12;
    value = r_u16(data + 4);
    pose[4] = value;
    value = r_u16(data + 6);
    pose[4] |= value << 16;
    value = r_u16(data + 8);
    pose[5] = value;
    sub_80083dfc(0, 0x800C6620, pose, 0);
    count = r_u32(0x800C6610);
    w_u32(0x800C6620, pose[0]);
    w_u32(0x800C6624, pose[1]);
    w_u32(0x800C6628, pose[2]);
    /* Fix016 defines the fourth position word copied by the original */
    w_u32(0x800C662C, pose[3]);
    index = 0;
    record = data + 24;
    if ((sint32)count > 0)
    {
        do
        {
            selected[index] = r_s16(record);
            record += 24;
            ++index;
        } while ((sint32)index < r_s32(0x800C6610));
    }
    sub_8002f770(0, 0, selected, mapping);
    count = r_u32(0x800C6610);
    index = 0;
    record = data + 24;
    if ((sint32)count > 0)
    {
        do
        {
            slot = mapping[index];
            value = r_u32(record + 8);
            state = 0x800C6630 + slot * 92;
            w_u32(state + 12, value << 12);
            value = r_u32(record + 12);
            w_u32(state + 16, value << 12);
            value = r_u32(record + 16);
            w_u32(state + 20, value << 12);
            value = r_u32(record + 20);
            ++index;
            sub_8002f6e4(slot, value);
            value = r_u16(record + 2);
            w_u16(state + 28, value);
            value = r_u16(record + 4);
            w_u16(state + 30, value);
            value = r_u16(record + 6);
            w_u16(state + 32, value);
            count = r_u32(0x800C6610);
            record += 24;
        } while ((sint32)index < (sint32)count);
    }
    sub_8002f9ac(0, pose);
    record = data + 120;
    if (r_s16(record) != -1)
    {
        do
        {
            sound_position[0] = r_u32(record + 8) << 12;
            sound_position[1] = r_u32(record + 12) << 12;
            sound_position[2] = r_u32(record + 16) << 12;
            sub_8002f3c0(r_s16(record), 0, 4, sound_position);
        } while (r_s16(record) != -1);
    }
    return 0;
}

/* Authorized native ABI replaces original scratch stack-address bookkeeping */
void sub_8008700c(uint32 records)
{
    uint32 a, b, c, group, bits, effect, offset = 40;
    FUNCTION_MARKER(0x8008700C, "ACE3.BIN");
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(0x1F80001C, a);
    w_u32(0x1F800020, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(0x1F800024, a);
    w_u32(0x1F800028, b);
    w_u32(0x1F80002C, c);
    for (group = 0; group < 10; ++group)
    {
        bits = r_u32(records + group * 4);
        if (bits != 0)
        {
            effect = records + offset;
            do
            {
                if ((bits & 1) != 0)
                {
                    if (r_s16(effect + 24) == 0)
                        sub_80086320(effect, 0x1F800000, 0x1F80001C);
                    else
                        sub_80086a0c(effect, 0x1F800000, 0x1F80001C);
                }
                effect += 28;
                bits >>= 1;
            } while (bits != 0);
        }
        offset += 896;
    }
    a = r_u32(0x1F80001C);
    b = r_u32(0x1F800020);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(0x1F800024);
    b = r_u32(0x1F800028);
    c = r_u32(0x1F80002C);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
}

/* Authorized native ABI replaces original scratch stack-address bookkeeping */
uint32 sub_8005e6dc(uint32 terrain, uint32 state, uint32 ordering, uint32 packet, const uint8 *native_state)
{
    uint32 previous[32], current[32], vector[3], tiles, value, scene, models, output, tag, result;
    FUNCTION_MARKER(0x8005E6DC, "ACE3.BIN");
    tiles = r_u32(terrain + 4);
    value = sub_8005e4cc(tiles, state, packet, native_state);
    w_u32(0x1F800004, value);
    value = r_u32(0x800BD068);
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800014, value + 31744);
    value = r_u32(0x800BD06C);
    w_u32(0x1F800000, ordering);
    w_u32(0x1F80001C, value + 31744);
    value = r_u32(scene + 100);
    w_u32(0x1F800018, value);
    vector[0] = r_u32(0x1F800014);
    vector[1] = r_u32(0x1F800018);
    vector[2] = r_u32(0x1F80001C);
    rotate_native_vector_a6b18(vector);
    w_u32(0x1F800014, vector[0]);
    w_u32(0x1F800018, vector[1]);
    w_u32(0x1F80001C, vector[2]);
    models = r_u32(terrain + 8);
    sub_80061768(packet, models, 0x1F800000, previous, current);
    output = r_u32(0x1F800004);
    w_u8(output + 3, 1);
    output = r_u32(0x1F800004);
    w_u32(output + 4, 0xE1000228);
    tag = r_u32(ordering + 16376);
    output = r_u32(0x1F800004);
    w_u32(output, tag | 0x01000000);
    output = r_u32(0x1F800004);
    w_u32(ordering + 16376, output & 0xFFFFFF);
    output = r_u32(0x1F800004);
    result = output + 8;
    w_u32(0x1F800004, result);
    return result;
}

uint32 resource001_bootstrap(void)
{
    uint32 state, flags, other, value, ids;

    FUNCTION_MARKER(0x800CCE98, "ACE3.BIN");
    state = r_u32(0x800BCE48);
    w_u32(state + 4, 1);
    flags = r_u32(state);
    w_u8(state + 16, 0);
    other = r_u32(0x800BCE48);
    w_u32(state + 20, 0);
    w_u32(state, flags | 9);
    w_u8(other + 17, 0);
    state = r_u32(0x800BCE48);
    w_u8(state + 18, 0);
    resource_init_queue(0, 0);
    resource001_sub_800dc810();
    value = sub_80027338(0x800DCE68, 0);
    state = r_u32(0x800DD9DC);
    other = r_u32(0x800BCE60);
    w_u32(state + 36, value);
    value = r_u32(other + 0x96C);
    resource001_sub_800da828((value & 2) != 0);
    resource001_sub_800d80c0();
    ids = r_u32(0x800BCE50);
    w_u32(ids + 96, 112);
    value = resource_total_size(ids + 96, 1);
    value = arena_alloc(value);
    state = r_u32(0x800BCE60);
    ids = r_u32(0x800BCE50);
    w_u32(state + 68, 1);
    resource_queue_list(ids + 96, 1, value, 0x8009B904);
    state = r_u32(0x800DD9D0);
    w_u32(state + 72, 120);
    return 0x04000000;
}

uint32 resource001_sub_800da828(uint32 enabled)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA828, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    w_u32(state + 28, enabled);
    return state;
}

uint32 resource001_sub_800da714(uint32 index, uint32 mode, uint32 first, uint32 second, uint32 third)
{
    uint32 state, offset, entries;

    FUNCTION_MARKER(0x800DA714, "ACE3.BIN");
    if (index >= 32)
        return 0x800E0000;
    state = r_u32(0x800DD9DC);
    entries = r_u32(state + 32);
    offset = index * 12;
    w_u16(entries + offset + 10, mode);
    entries = r_u32(state + 32);
    w_u16(entries + offset + 4, first);
    entries = r_u32(state + 32);
    w_u16(entries + offset + 6, second);
    entries = r_u32(state + 32);
    w_u16(entries + offset + 8, third);
    return entries;
}

uint32 resource001_sub_800da77c(uint32 index)
{
    FUNCTION_MARKER(0x800DA77C, "ACE3.BIN");
    return resource001_sub_800da714(index, 1, 0, 0, 0);
}

uint32 resource001_sub_800d7c40(uint32 first, uint32 second)
{
    uint32 state;

    FUNCTION_MARKER(0x800D7C40, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    w_u16(state + 32, first);
    w_u16(state + 36, first);
    w_u16(state + 34, second);
    w_u16(state + 38, second);
    return state;
}

uint32 resource001_sub_800d7c60(uint32 red, uint32 green, uint32 blue)
{
    uint32 state;

    FUNCTION_MARKER(0x800D7C60, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    w_u8(state + 12, red);
    state = r_u32(0x800DDA58);
    w_u8(state + 13, green);
    state = r_u32(0x800DDA58);
    w_u8(state + 14, blue);
    return state;
}

uint32 resource001_sub_800dc810(void)
{
    uint32 state, value, entries, index, offset;

    FUNCTION_MARKER(0x800DC810, "ACE3.BIN");
    value = arena_alloc(88);
    w_u32(0x800DD9DC, value);
    value = arena_alloc(384);
    state = r_u32(0x800DD9DC);
    w_u32(state + 32, value);
    value = arena_alloc(128);
    state = r_u32(0x800DD9DC);
    w_u32(state + 36, value);
    for (index = 0; index < 32; ++index)
    {
        offset = index * 12;
        entries = r_u32(state + 32);
        w_u16(entries + offset, 0);
        entries = r_u32(state + 32);
        w_u16(entries + offset + 2, 0);
    }
    for (index = 0; index < 2; ++index)
    {
        value = arena_alloc(64);
        state = r_u32(0x800DD9DC);
        w_u32(state + 20 + index * 4, value);
        value = arena_alloc(0x8000);
        state = r_u32(0x800DD9DC);
        w_u32(state + 8 + index * 4, value);
    }
    state = r_u32(0x800DD9DC);
    w_u16(state + 62, 1);
    for (offset = 48; offset <= 60; offset += 2)
        w_u16(state + offset, 0);
    w_u32(state + 44, state + 48);
    value = arena_alloc(32);
    state = r_u32(0x800DD9DC);
    w_u32(state + 64, value);
    value = arena_alloc(32);
    state = r_u32(0x800DD9DC);
    w_u32(state + 72, value);
    value = arena_alloc(32);
    state = r_u32(0x800DD9DC);
    w_u32(state + 68, value);
    for (index = 0; index < 16; ++index)
    {
        entries = r_u32(state + 64);
        w_u16(entries + index * 2, 0);
        entries = r_u32(state + 72);
        w_u16(entries + index * 2, 0);
        entries = r_u32(state + 68);
        w_u16(entries + index * 2, 0);
    }
    value = arena_alloc(64);
    state = r_u32(0x800DD9DC);
    w_u32(state + 76, value);
    for (index = 0; index < 64; ++index)
    {
        state = r_u32(0x800DD9DC);
        entries = r_u32(state + 76);
        w_u8(entries + index, 0);
    }
    state = r_u32(0x800DD9DC);
    w_u32(state + 28, 0);
    w_u32(state + 84, 0);
    for (index = 0; index < 32; ++index)
        resource001_sub_800da77c(index);
    return 0;
}

uint32 resource001_sub_800d80c0(void)
{
    uint32 state, value, buffer, index, packet;

    FUNCTION_MARKER(0x800D80C0, "ACE3.BIN");
    value = arena_alloc(76);
    w_u32(0x800DDA58, value);
    for (index = 0; index < 2; ++index)
    {
        value = arena_alloc(40000);
        state = r_u32(0x800DDA58);
        w_u32(state + index * 4, value);
        value = arena_alloc(4);
        state = r_u32(0x800DDA58);
        buffer = r_u32(state + index * 4);
        w_u32(state + 20 + index * 4, value);
        w_u8(state + 64, 0);
        value = r_u32(0x800DDA58);
        w_u32(state + 8, buffer);
        w_u8(value + 65, 0);
        resource001_sub_800d7d08(4);
        for (packet = 0; packet < 1000; ++packet)
        {
            state = r_u32(0x800DDA58);
            buffer = r_u32(state + 8);
            w_u8(buffer + 3, 9);
            state = r_u32(0x800DDA58);
            buffer = r_u32(state + 8);
            w_u8(buffer + 7, 44);
            state = r_u32(0x800DDA58);
            buffer = r_u32(state + 8);
            w_u32(state + 8, buffer + 40);
        }
    }
    state = r_u32(0x800DDA58);
    w_u32(state + 16, 0);
    w_u8(state + 67, 0);
    resource001_sub_800d7c40(10, 200);
    return resource001_sub_800d7c60(128, 128, 128);
}

void resource001_sub_800d7d08(uint32 mode)
{
    uint32 target, state, other, x, y, packed, tail;

    FUNCTION_MARKER(0x800D7D08, "ACE3.BIN");
    if (mode - 1 >= 7)
        return;
    target = r_u32(0x800CC570 + (mode - 1) * 4);
    tail = 0;
    switch (target)
    {
        case 0x800D7D34:
            state = r_u32(0x800DDA58);
            w_u8(state + 56, 6);
            state = r_u32(0x800DDA58);
            w_u8(state + 57, 6);
            state = r_u32(0x800DDA58);
            w_u16(state + 44, 16);
            w_u16(state + 46, 2);
            w_u16(state + 50, 320);
            y = r_u16(state + 50);
            w_u16(state + 48, 320);
            x = r_u16(state + 48);
            w_u16(state + 52, 320);
            w_u16(state + 54, 344);
            w_u8(state + 58, 32);
            break;
        case 0x800D7DDC:
            state = r_u32(0x800DDA58);
            w_u8(state + 56, 6);
            state = r_u32(0x800DDA58);
            w_u8(state + 57, 6);
            state = r_u32(0x800DDA58);
            w_u16(state + 46, 2);
            w_u16(state + 48, 392);
            w_u16(state + 50, 281);
            y = r_u16(state + 50);
            w_u16(state + 44, 32);
            w_u8(state + 58, 32);
            x = r_u16(state + 48);
            w_u16(state + 52, 384);
            w_u16(state + 54, 510);
            break;
        case 0x800D7E3C:
        case 0x800D7F04:
            tail = target == 0x800D7E3C ? 3 : 4;
            state = r_u32(0x800DDA58);
            w_u8(state + 56, 16);
            state = r_u32(0x800DDA58);
            w_u8(state + 57, 21);
            state = r_u32(0x800DDA58);
            w_u16(state + 44, 16);
            w_u16(state + 48, 768);
            w_u16(state + 50, tail == 3 ? 382 : 256);
            y = r_u16(state + 50);
            x = r_u16(state + 48);
            w_u16(state + 52, 400);
            w_u16(state + 54, 45);
            w_u16(state + 46, 1);
            w_u8(state + 58, 33);
            break;
        case 0x800D7FC0:
            tail = 7;
            state = r_u32(0x800DDA58);
            w_u8(state + 56, 6);
            state = r_u32(0x800DDA58);
            w_u8(state + 57, 10);
            state = r_u32(0x800DDA58);
            w_u16(state + 44, 25);
            w_u16(state + 46, 1);
            w_u16(state + 50, 384);
            y = r_u16(state + 50);
            w_u16(state + 48, 384);
            x = r_u16(state + 48);
            w_u16(state + 52, 400);
            w_u16(state + 54, 511);
            w_u8(state + 58, 48);
            break;
        case 0x800D808C:
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 draw-mode branch %08X\n", target);
#endif
            abort();
    }
    packed = ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6);
    y = y & 0x200;
    x = r_u16(state + 54);
    w_u16(state + 40, packed | (y << 2));
    y = r_u16(state + 52);
    other = r_u32(0x800DDA58);
    w_u16(state + 42, (x << 6) | ((y >> 4) & 63));
    w_u8(other + 59, tail == 3 || tail == 4);
    if (tail == 3 || tail == 4)
    {
        state = r_u32(0x800DDA58);
        w_u32(state + 60, tail == 3 ? 0x800DD870 : 0x800DD810);
        w_u8(state + 66, 4);
    }
    else if (tail == 7)
    {
        state = r_u32(0x800DDA58);
        w_u8(state + 66, 6);
        state = r_u32(0x800DDA58);
        w_u8(state + 64, 0);
        state = r_u32(0x800DDA58);
        w_u8(state + 65, 0);
    }
}

uint32 resource001_sub_800d7c88(uint32 selection)
{
    uint32 state;

    FUNCTION_MARKER(0x800D7C88, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    if ((sint16)selection == -1)
    {
        w_u8(state + 64, 0);
        state = r_u32(0x800DDA58);
        w_u8(state + 65, 0);
    }
    else
    {
        w_u8(state + 64, 1);
        state = r_u32(0x800DDA58);
        w_u8(state + 65, selection);
    }
    return state;
}

uint32 resource001_sub_800d7cd4(uint32 value)
{
    uint32 state;

    FUNCTION_MARKER(0x800D7CD4, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    w_u16(state + 72, value);
    return state;
}

uint32 resource001_sub_800d7ce4(uint32 enabled, uint32 first, uint32 second)
{
    uint32 state;

    FUNCTION_MARKER(0x800D7CE4, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    w_u8(state + 67, enabled);
    state = r_u32(0x800DDA58);
    w_u16(state + 68, first);
    w_u16(state + 70, second);
    return state;
}

uint32 resource001_sub_800d81f0(uint32 packet)
{
    uint32 game, index, state, buffer, ordering, link, tag, result;

    FUNCTION_MARKER(0x800D81F0, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    index = r_u32(game + 4);
    state = r_u32(0x800DDA58);
    buffer = r_u32(state + index * 4);
    w_u32(state + 8, buffer);
    index = r_u32(game + 4);
    ordering = r_u32(state + index * 4 + 20);
    w_u32(state + 28, ordering);
    ClearOTagR(psx_addr(ordering, 4), 1);
    state = r_u32(0x800DDA58);
    ordering = r_u32(state + 28);
    link = r_u32(packet);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(packet);
    ordering = r_u32(state + 28);
    result = ordering & 0xFFFFFF;
    w_u32(packet, (link & 0xFF000000) | result);
    return result;
}

uint32 resource001_sub_800d8cec(uint32 resource)
{
    uint32 scratch, input;

    FUNCTION_MARKER(0x800D8CEC, "ACE3.BIN");
    scratch = arena_alloc(0x10000);
    input = resource;
    if (r_u32(resource) == 0x1A7A6C55)
    {
        sub_800afd08(resource, scratch);
        input = scratch;
    }
    sub_800271a0(input);
    DrawSync(0);
    return arena_set_cursor(scratch);
}

uint32 resource001_sub_800d8ba8(uint32 resource)
{
    uint32 scratch, input, header, entries, count;

    FUNCTION_MARKER(0x800D8BA8, "ACE3.BIN");
    scratch = arena_alloc(0x30000);
    header = r_u32(resource);
    input = resource;
    if (header == 0x1A7A6C55)
    {
        sub_800afd08(resource, scratch);
        (void)r_u32(scratch);
        input = scratch;
    }
    /* Relocation returns the original a1 count alongside its table pointer */
    entries = sub_80027338(input, &count);
    sub_8002722c(entries, count);
    DrawSync(0);
    arena_set_cursor(scratch);
    return 1;
}

uint32 resource001_sub_800da484(void)
{
    sint32 remaining = 4;

    FUNCTION_MARKER(0x800DA484, "ACE3.BIN");
    while (remaining-- >= 0)
    {
    }
    return (uint32)remaining;
}

uint32 resource001_sub_800da418(uint32 resource)
{
    uint32 state, entries, index;

    FUNCTION_MARKER(0x800DA418, "ACE3.BIN");
    state = arena_alloc(12);
    w_u32(0x800DDA68, state);
    entries = sub_80027338(resource, 0);
    state = r_u32(0x800DDA68);
    w_u32(state + 8, entries);
    for (index = 0; index < 8; ++index)
    {
        state = r_u32(0x800DDA68);
        w_u8(state + index, 0);
    }
    return resource001_sub_800da484();
}

uint32 resource001_sub_800dc9ec(uint32 packet)
{
    uint32 game, index, state, ordering, buffer, link, tag, result;

    FUNCTION_MARKER(0x800DC9EC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    index = r_u32(game + 4);
    state = r_u32(0x800DD9DC);
    ordering = r_u32(state + index * 4 + 20);
    w_u32(state + 16, ordering);
    index = r_u32(game + 4);
    ordering = r_u32(state + 16);
    buffer = r_u32(state + index * 4 + 8);
    w_u32(state + 4, buffer);
    ClearOTagR(psx_addr(ordering, 64), 16);
    state = r_u32(0x800DD9DC);
    ordering = r_u32(state + 16);
    link = r_u32(packet);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (link & 0xFFFFFF));
    link = r_u32(packet);
    ordering = r_u32(state + 16);
    result = (ordering + 60) & 0xFFFFFF;
    w_u32(packet, (link & 0xFF000000) | result);
    return result;
}

uint32 resource001_sub_800da8e0(void)
{
    uint32 state, index, offset, modes, values, limits, value;
    sint32 current, limit;

    FUNCTION_MARKER(0x800DA8E0, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    for (index = 0; index < 16; ++index)
    {
        offset = index * 2;
        modes = r_u32(state + 72) + offset;
        if (r_s16(modes) == 1)
        {
            values = r_u32(state + 64) + offset;
            limits = r_u32(state + 68) + offset;
            current = r_s16(values);
            limit = r_s16(limits);
            value = r_u16(values);
            if (current < limit)
                w_u16(values, value + 1);
            else
                w_u16(modes, 2);
        }
        modes = r_u32(state + 72) + offset;
        if (r_s16(modes) == 3)
        {
            values = r_u32(state + 64) + offset;
            current = r_s16(values);
            value = r_u16(values);
            if (current > 0)
                w_u16(values, value - 1);
            else
                w_u16(modes, 0);
        }
    }
    return 0;
}

uint32 resource001_sub_800cce38(uint32 resource, uint32 output, uint32 *native_output, uint32 native_capacity)
{
    uint32 data, index, cursor, value, adjusted;
    sint32 count;

    FUNCTION_MARKER(0x800CCE38, "ACE3.BIN");
    data = r_u32(resource);
    count = r_s32(data);
    index = 0;
    cursor = data;
    if (count > 0)
    {
        do
        {
            value = r_u32(cursor + 4);
            adjusted = value;
            if ((sint32)value < 0)
                adjusted += 3;
            adjusted = (uint32)((sint32)adjusted >> 2) << 2;
            value = data + adjusted;
            if (native_output)
            {
                if (index >= native_capacity)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource001 icon pointer output exceeds native capacity\n");
#endif
                    abort();
                }
                native_output[index] = value;
            }
            else
                w_u32(output + index * 4, value);
            value = r_u32(data);
            ++index;
            cursor += 4;
        } while ((sint32)index < (sint32)value);
    }
    return (uint32)count;
}

uint32 resource001_sub_800d4ff0(uint32 resource, uint32 palette)
{
    uint32 icons[4];
    uint32 state, source, destination, first, second, third, fourth;
    uint32 index, image, count, value, previous;

    FUNCTION_MARKER(0x800D4FF0, "ACE3.BIN");
    state = arena_alloc(640);
    w_u32(0x800DDA44, state);
    first = r_u16(0x800CC488);
    second = r_u8(0x800CC48A);
    w_u16(state, first);
    w_u8(state + 2, second);
    state = r_u32(0x800DDA44);
    w_u8(state + 2, 19);
    state = r_u32(0x800DDA44);
    w_u8(state + 3, 2);
    state = r_u32(0x800DDA44);
    destination = state + 4;
    for (source = 0x800CC48C; source != 0x800CC4AC; source += 16, destination += 16)
    {
        first = r_u32(source);
        second = r_u32(source + 4);
        third = r_u32(source + 8);
        fourth = r_u32(source + 12);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
    }
    state = r_u32(0x800DDA44);
    first = r_u32(source);
    second = r_u16(source + 4);
    third = r_u8(source + 6);
    w_u32(destination, first);
    w_u16(destination + 4, second);
    w_u8(destination + 6, third);
    w_u32(state + 76, 0);
    w_u32(state + 72, 0);
    w_u32(state + 68, 0);
    state = r_u32(0x800DDA44);
    w_u16(state + 80, 1);
    first = r_u32(0x800CC4B4);
    first = r_u32(0x800CC4B4);
    second = r_u8(0x800CC4B8);
    /* Preserve the original SWL then SWR order for the unaligned word */
    previous = r_u32(state + 84);
    w_u32(state + 84, (previous & 0xFFFF0000) | (first >> 16));
    previous = r_u32(state + 80);
    w_u32(state + 80, (previous & 0xFFFF) | (first << 16));
    w_u8(state + 86, second);
    w_u8(state + 86, 0);
    state = r_u32(0x800DDA44);
    w_u8(state + 87, 0);
    state = r_u32(0x800DDA44);
    w_u32(state + 88, 0);
    w_u32(state + 92, 0);
    count = resource001_sub_800cce38(resource, 0, icons, 4);
    state = r_u32(0x800DDA44);
    w_u8(state + 2, count + 16);
    if ((sint32)count <= 0 || count > 4)
    {
#ifndef NDEBUG
        fprintf(stderr, "Unsupported resource001 card icon count %u\n", count);
#endif
        abort();
    }
    for (index = 0; index < 8; ++index)
    {
        state = r_u32(0x800DDA44);
        value = r_u32(icons[0] + index * 4 + 20);
        w_u32(state + index * 4 + 96, value);
    }
    for (image = 0; image < count; ++image)
    {
        for (index = 0; index < 32; ++index)
        {
            state = r_u32(0x800DDA44);
            value = r_u32(icons[image] + index * 4 + 64);
            w_u32(state + image * 128 + index * 4 + 128, value);
        }
    }
    state = r_u32(0x800DDA44);
    for (index = 0; index < 32; ++index)
    {
        value = r_u32(palette + index * 4);
        w_u32(state + index * 4 + 512, value);
    }
    return 0;
}

uint32 resource001_sub_800ccfa4(void)
{
    uint32 state, bundle, source, value, game, flags;

    FUNCTION_MARKER(0x800CCFA4, "ACE3.BIN");
    state = r_u32(0x800BCE50);
    bundle = sub_80027338(r_u32(state + 96), 0);
    sub_8002d96c(r_u32(bundle + 32));
    sound_enable_reverb();
    source = r_u32(bundle);
    state = r_u32(0x800DD9DC);
    value = r_u32(source);
    w_u32(state, value);
    value = sub_80027338(r_u32(bundle), 0);
    state = r_u32(0x800DD9DC);
    w_u32(state + 36, value);
    value = r_u32(bundle + 8);
    resource001_sub_800d4ff0(bundle + 4, value);
    resource001_sub_800d8ba8(r_u32(bundle + 16));
    resource001_sub_800d8cec(r_u32(bundle + 24));
    arena_set_cursor(r_u32(bundle + 16));
    resource001_sub_800da418(r_u32(bundle + 12));
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 0x96C);
    w_u32(game + 0x96C, flags & 0xFFFFFFFD);
    return 0x04000000;
}

uint32 resource001_sub_800d82a4(uint32 character)
{
    uint32 state, packet, ordering, relative, width, height, u, v, right, bottom;
    uint32 color, blend, page_y, page_x, tag, link, result, advance;
    uint32 vertex, x, y, product, numerator, extra, quotient, second_quotient;
    sint32 columns, column, row, texture_x, texture_y, scale, end, current, start, divisor, half;
    int interpolate = 0;

    FUNCTION_MARKER(0x800D82A4, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    relative = r_u8(state + 58);
    columns = r_s16(state + 44);
    relative = (character - relative) & 255;
    if (columns == 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource001 glyph division trap7\n");
#endif
        abort();
    }
    row = (sint32)relative / columns;
    column = (sint32)relative % columns;
    width = r_u8(state + 56);
    texture_x = r_s16(state + 48);
    u = (uint32)column * width + (uint32)(texture_x % 64) * 4;
    height = r_u8(state + 57);
    texture_y = r_s16(state + 50);
    v = (uint32)row * height + (uint32)(texture_y % 256);
    packet = r_u32(state + 8);
    w_u8(packet + 12, u);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    w_u8(packet + 13, v);
    state = r_u32(0x800DDA58);
    right = u + 255;
    width = r_u8(state + 56);
    packet = r_u32(state + 8);
    w_u8(packet + 20, width + right);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    w_u8(packet + 21, v);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    w_u8(packet + 28, u);
    state = r_u32(0x800DDA58);
    bottom = v + 255;
    height = r_u8(state + 57);
    packet = r_u32(state + 8);
    w_u8(packet + 29, height + bottom);
    state = r_u32(0x800DDA58);
    width = r_u8(state + 56);
    packet = r_u32(state + 8);
    w_u8(packet + 36, width + right);
    state = r_u32(0x800DDA58);
    height = r_u8(state + 57);
    packet = r_u32(state + 8);
    w_u8(packet + 37, height + bottom);
    state = r_u32(0x800DDA58);
    blend = r_u8(state + 64);
    packet = r_u32(state + 8);
    color = r_u8(packet + 7);
    w_u8(packet + 7, blend ? color | 2 : color & 0xFD);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    color = r_u8(state + 12);
    w_u8(packet + 4, color);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    color = r_u8(state + 13);
    w_u8(packet + 5, color);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    color = r_u8(state + 14);
    w_u8(packet + 6, color);
    state = r_u32(0x800DDA58);
    blend = r_u8(state + 65);
    page_y = r_u16(state + 50);
    page_x = r_u16(state + 48);
    packet = r_u32(state + 8);
    w_u16(packet + 22, ((blend & 3) << 5) | ((page_y & 0x100) >> 4) | ((page_x & 0x3FF) >> 6) | ((page_y & 0x200) << 2));
    packet = r_u32(state + 8);
    color = r_u16(state + 42);
    w_u16(packet + 14, color);
    if (r_u8(state + 67) == 1)
    {
        current = r_s16(state + 72);
        start = r_s16(state + 68);
        if (current < start)
            return 0;
        end = r_s16(state + 70);
        interpolate = current < end;
    }
    for (vertex = 0; vertex < 4; ++vertex)
    {
        if ((vertex & 1) == 0)
        {
            packet = r_u32(state + 8);
            x = r_u16(state + 32);
        }
        else
        {
            width = r_u8(state + 56);
            x = r_u16(state + 32);
            packet = r_u32(state + 8);
            x += width + 0xFFFF;
        }
        w_u16(packet + 8 + vertex * 8, x);
        if (interpolate)
        {
            height = r_u8(state + 57);
            scale = r_s16(state + 46);
            end = r_s16(state + 70);
            current = r_s16(state + 72);
            start = r_s16(state + 68);
            product = height * (uint32)scale;
            half = (sint32)product / 2;
            numerator = (uint32)half * (uint32)(end - current);
            divisor = end - start;
            if (divisor == 0 || (divisor == -1 && numerator == 0x80000000))
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource001 glyph interpolation division trap\n");
#endif
                abort();
            }
            quotient = (uint32)((sint32)numerator / divisor);
            second_quotient = 0;
            if (vertex >= 2)
            {
                extra = (height - 1) * (uint32)scale * (uint32)(current - start);
                if (divisor == -1 && extra == 0x80000000)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource001 glyph interpolation division trap6\n");
#endif
                    abort();
                }
                second_quotient = (uint32)((sint32)extra / divisor);
            }
            y = r_u16(state + 34);
            packet = r_u32(state + 8);
            w_u16(packet + 10 + vertex * 8, y + quotient + second_quotient);
        }
        else if (vertex < 2)
        {
            packet = r_u32(state + 8);
            y = r_u16(state + 34);
            w_u16(packet + 10 + vertex * 8, y);
        }
        else
        {
            height = r_u8(state + 57);
            scale = r_s16(state + 46);
            y = r_u16(state + 34);
            packet = r_u32(state + 8);
            w_u16(packet + 10 + vertex * 8, y + (height - 1) * (uint32)scale);
        }
    }
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    ordering = r_u32(state + 28);
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    ordering = r_u32(state + 28);
    packet = r_u32(state + 8);
    tag = r_u32(ordering);
    w_u32(ordering, (tag & 0xFF000000) | (packet & 0xFFFFFF));
    if (r_u8(state + 59))
    {
        advance = r_u32(state + 60);
        advance = r_u8(advance + relative);
    }
    else
        advance = r_u8(state + 56);
    x = r_u16(state + 32);
    w_u16(state + 32, x + advance);
    state = r_u32(0x800DDA58);
    packet = r_u32(state + 8);
    result = packet + 40;
    w_u32(state + 8, result);
    return result;
}

uint32 resource001_sub_800d8980(uint32 text, const uint8 *native_text, uint32 native_capacity)
{
    uint32 state, count, index, character, height, origin, y, x, advance, result;
    sint32 scale;

    FUNCTION_MARKER(0x800D8980, "ACE3.BIN");
    state = r_u32(0x800DDA58);
    count = r_u32(state + 16);
    result = count;
    index = 0;
    if ((sint32)count > 0)
    {
        do
        {
            if (native_text && index >= native_capacity)
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource001 text exceeds native buffer capacity\n");
#endif
                abort();
            }
            character = native_text ? native_text[index] : r_u8(text + index);
            switch (character)
            {
                case 10:
                    state = r_u32(0x800DDA58);
                    height = r_u8(state + 57);
                    scale = r_s16(state + 46);
                    origin = r_u16(state + 36);
                    y = r_u16(state + 34);
                    w_u16(state + 32, origin);
                    w_u16(state + 34, y + height * (uint32)scale);
                    break;
                case 9:
                    state = r_u32(0x800DDA58);
                    origin = r_u16(state + 36);
                    w_u16(state + 32, origin);
                    break;
                case 32:
                    state = r_u32(0x800DDA58);
                    if (r_u8(state + 59))
                        advance = r_u8(state + 66);
                    else
                        advance = r_u8(state + 56);
                    x = r_u16(state + 32);
                    w_u16(state + 32, x + advance);
                    break;
                default:
                    character = native_text ? native_text[index] : r_u8(text + index);
                    resource001_sub_800d82a4(character);
                    break;
            }
            state = r_u32(0x800DDA58);
            count = r_u32(state + 16);
            ++index;
            result = (sint32)index < (sint32)count;
        } while (result);
    }
    return result;
}

uint32 resource001_sub_800d8aa0(uint32 format, uint32 arguments, const uint8 *native_arguments, uint32 native_argument_bytes)
{
    uint8 buffer[512];
    uint32 length, state;

    FUNCTION_MARKER(0x800D8AA0, "ACE3.BIN");
    length = format_to_native_buffer(buffer, sizeof(buffer), format, arguments, native_arguments, native_argument_bytes);
    state = r_u32(0x800DDA58);
    w_u32(state + 16, length);
    return resource001_sub_800d8980(0, buffer, sizeof(buffer));
}

uint32 resource001_sub_800d3d90(uint32 port)
{
    uint32 state, value;
    sint32 status;

    FUNCTION_MARKER(0x800D3D90, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    value = r_u8(state + port + 840);
    if (!value)
    {
        status = r_s16(state + port * 2 + 44);
        return status == 2 ? 6 : 0;
    }
    status = r_s16(state + port * 2 + 44);
    switch (status)
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 0:
        case 3:
            break;
        default:
            return 0xFFFFFFFF;
    }
    state = r_u32(0x800DDA48);
    status = r_s16(state + port * 2 + 48);
    switch (status)
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 0:
            break;
        default:
            return 0xFFFFFFFF;
    }
    value = r_u8(state + port + 842);
    if (value == 1)
        return 5;
    if (value == 0)
        return 4;
    return 1;
}

sint32 resource001_sub_800da8bc(uint32 index)
{
    uint32 state, values;

    FUNCTION_MARKER(0x800DA8BC, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    values = r_u32(state + 64);
    return r_s16(values + index * 2);
}

uint32 resource001_sub_800dc434(void)
{
    uint8 no_arguments[1] = {0};
    uint32 can_download, can_reopen, state, value;

    FUNCTION_MARKER(0x800DC434, "ACE3.BIN");
    value = resource001_sub_800d3d90(0);
    can_download = 1;
    if (value != 5)
        can_download = resource001_sub_800d3d90(1) == 5;
    state = r_u32(0x800BCE60);
    can_reopen = r_u32(state + 0x964) & 1;
    resource001_sub_800d7d08(4);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7ce4(1, 30, 40);
    value = (uint32)resource001_sub_800da8bc(3);
    resource001_sub_800d7cd4(value);
    resource001_sub_800d7c60(128, 128, 128);
    resource001_sub_800d7c40(106, 150);
    resource001_sub_800d8aa0(0x800CC5B8, 0, no_arguments, 0);
    resource001_sub_800d7c40(106, 204);
    if (can_download)
        resource001_sub_800d8aa0(0x800CC5C4, 0, no_arguments, 0);
    else
    {
        resource001_sub_800d7c88(1);
        resource001_sub_800d7c60(8, 48, 40);
        resource001_sub_800d8aa0(0x800CC5C4, 0, no_arguments, 0);
        resource001_sub_800d7c88(2);
        resource001_sub_800d7c60(128, 128, 128);
    }
    resource001_sub_800d7c40(106, 257);
    if (can_reopen)
        resource001_sub_800d8aa0(0x800CC5D0, 0, no_arguments, 0);
    else
    {
        resource001_sub_800d7c88(1);
        resource001_sub_800d7c60(8, 48, 40);
        resource001_sub_800d8aa0(0x800CC5D0, 0, no_arguments, 0);
        resource001_sub_800d7c88(2);
        resource001_sub_800d7c60(128, 128, 128);
    }
    resource001_sub_800d7ce4(1, 30, 40);
    value = (uint32)resource001_sub_800da8bc(4);
    resource001_sub_800d7cd4(value);
    return resource001_sub_800d7c60(128, 128, 128);
}

void resource001_sub_800dc42c(void)
{
    FUNCTION_MARKER(0x800DC42C, "ACE3.BIN");
}

void resource001_sub_800dc5e4(void)
{
    FUNCTION_MARKER(0x800DC5E4, "ACE3.BIN");
}

void resource001_sub_800dc694(void)
{
    FUNCTION_MARKER(0x800DC694, "ACE3.BIN");
}

void resource001_sub_800dc69c(void)
{
    FUNCTION_MARKER(0x800DC69C, "ACE3.BIN");
}

void resource001_sub_800dc6a4(void)
{
    FUNCTION_MARKER(0x800DC6A4, "ACE3.BIN");
}

void resource001_sub_800dc6ac(void)
{
    FUNCTION_MARKER(0x800DC6AC, "ACE3.BIN");
}

void resource001_sub_800dc6b4(void)
{
    FUNCTION_MARKER(0x800DC6B4, "ACE3.BIN");
}

void resource001_sub_800dc6bc(void)
{
    FUNCTION_MARKER(0x800DC6BC, "ACE3.BIN");
}

uint32 sub_800ac7f8(uint32 packet, uint32 draw_display, uint32 dither, uint32 page, uint32 window)
{
    uint32 command, component, x, y, width, height;

    FUNCTION_MARKER(0x800AC7F8, "ACE3.BIN");
    w_u8(packet + 3, 2);
    command = 0xE1000000;
    if (dither)
        command |= 0x200;
    component = page & 0x9FF;
    if (draw_display)
        component |= 0x400;
    command |= component;
    w_u32(packet + 4, command);
    if (window)
    {
        y = r_u8(window + 2);
        x = r_u8(window);
        height = (uint32)r_s16(window + 6);
        width = (uint32)r_s16(window + 4);
        component = ((0 - width) & 255) >> 3;
        command = 0xE2000000 | ((y >> 3) << 15) | ((x >> 3) << 10) | (((0 - height) << 2) & 0x3E0) | component;
        w_u32(packet + 8, command);
        return component;
    }
    w_u32(packet + 8, 0);
    return command;
}

uint32 resource001_sub_800db254(uint32 command, uint32 ordering)
{
    uint32 state, packet, blend, x, y, mode, config, slot, tag, link;

    FUNCTION_MARKER(0x800DB254, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    w_u32(state + 4, packet + 12);
    blend = r_u8(command + 7);
    y = r_u16(command + 2);
    x = r_u16(command);
    mode = ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    sub_800ac7f8(packet, 0, 0, mode, 0);
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    link = r_u32(ordering + slot * 4);
    w_u32(ordering + slot * 4, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return command + 12;
}

uint32 resource001_sub_800db834(uint32 command, uint32 ordering)
{
    PSX_RECT rectangle;
    uint32 state, packet, slot, tag, link;

    FUNCTION_MARKER(0x800DB834, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    w_u32(state + 4, packet + 12);
    rectangle.x = (sint16)r_u16(command + 4);
    rectangle.y = (sint16)r_u16(command + 6);
    rectangle.w = (sint16)r_u16(command + 8);
    rectangle.h = (sint16)r_u16(command + 10);
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), &rectangle);
    slot = r_u32(command + 16);
    tag = r_u32(packet);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    slot = r_u32(command + 16);
    link = r_u32(ordering + slot * 4);
    w_u32(ordering + slot * 4, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return command + 20;
}

uint32 resource001_sub_800db910(uint32 command)
{
    uint32 state;

    FUNCTION_MARKER(0x800DB910, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    w_u32(state + 44, command);
    return command + 16;
}

uint32 resource001_sub_800da9d4(void)
{
    FUNCTION_MARKER(0x800DA9D4, "ACE3.BIN");
    return 1;
}

uint32 resource001_sub_800dac98(void)
{
    uint32 state, config, modes;
    sint32 index;

    FUNCTION_MARKER(0x800DAC98, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    index = r_s16(config + 12);
    modes = r_u32(state + 72);
    return r_s16(modes + (uint32)index * 2) == 2;
}

uint32 resource001_sub_800da9dc(void)
{
    uint32 state, config, values, modes, output, low, origin_word, numerator, quotient;
    sint32 index, start, middle, end, phase, mode, position, origin, delta, divisor;

    FUNCTION_MARKER(0x800DA9DC, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    values = r_u32(state + 64);
    index = r_s16(config + 12);
    middle = r_s16(config + 6);
    low = r_u16(config + 4);
    phase = r_s16(values + (uint32)index * 2);
    start = (sint16)low;
    config = r_u32(state + 44);
    end = (sint16)r_u16(config + 8);
    config = r_u32(state + 44);
    index = r_s16(config + 12);
    modes = r_u32(state + 72);
    mode = r_s16(modes + (uint32)index * 2);
    output = r_u32(0x800DD9D8);
    if (!mode)
        return 0;
    if (mode == 2)
        return 1;
    if (phase < start)
        return 0;
    if (phase < middle)
    {
        position = r_s16(output);
        origin = r_s16(config);
        numerator = (uint32)(position - origin) * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal coordinate division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = r_u16(config);
        w_u16(output, origin_word + quotient);
        config = r_u32(state + 44);
        position = r_s16(output + 2);
        origin = r_s16(config + 2);
        delta = position - origin;
        divisor = delta < 0 ? -delta : delta;
        if (!divisor)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal direction division trap7\n");
#endif
            abort();
        }
        quotient = (uint32)(delta / divisor);
        origin_word = r_u16(config + 2);
        w_u16(output + 2, origin_word + quotient);
        position = r_s16(output + 4);
        numerator = (uint32)position * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal width division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        w_u16(output + 6, 2);
        w_u16(output + 4, quotient);
    }
    else if (phase < end)
    {
        position = r_s16(output + 2);
        origin = r_s16(config + 2);
        numerator = (uint32)(position - origin) * (uint32)(phase - middle);
        divisor = end - middle;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal vertical division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = r_u16(config + 2);
        w_u16(output + 2, origin_word + quotient);
        position = r_s16(output + 6);
        numerator = (uint32)position * (uint32)(phase - middle);
        divisor = end - middle;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal height division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        w_u16(output + 6, quotient);
    }
    return 1;
}

uint32 resource001_sub_800dacd0(void)
{
    uint32 state, config, values, modes, output, low, numerator, quotient, origin_word;
    sint32 index, start, middle, phase, mode, position, origin, divisor;

    FUNCTION_MARKER(0x800DACD0, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    values = r_u32(state + 64);
    index = r_s16(config + 12);
    middle = r_s16(config + 6);
    low = r_u16(config + 4);
    phase = r_s16(values + (uint32)index * 2);
    start = (sint16)low;
    config = r_u32(state + 44);
    (void)r_u16(config + 8);
    config = r_u32(state + 44);
    index = r_s16(config + 12);
    modes = r_u32(state + 72);
    mode = r_s16(modes + (uint32)index * 2);
    output = r_u32(0x800DD9D8);
    if (!mode)
        return 0;
    if (mode == 2)
        return 1;
    if (phase < start)
        return 0;
    if (phase < middle)
    {
        position = r_s16(output);
        origin = r_s16(config);
        numerator = (uint32)(position - origin) * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 horizontal reveal coordinate division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = r_u16(config);
        w_u16(output, origin_word + quotient);
        position = r_s16(output + 4);
        numerator = (uint32)position * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 horizontal reveal width division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        w_u16(output + 4, quotient);
    }
    return 1;
}

/* Bind the four original zero-argument coordinate callbacks */
static uint32 resource001_transform_call(uint32 target)
{
    switch (target)
    {
        case 0x800DA9D4:
            return resource001_sub_800da9d4();
        case 0x800DA9DC:
            return resource001_sub_800da9dc();
        case 0x800DAC98:
            return resource001_sub_800dac98();
        case 0x800DACD0:
            return resource001_sub_800dacd0();
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 coordinate callback %08X\n", target);
#endif
            abort();
    }
}

uint32 resource001_sub_800dae44(uint32 command, uint32 ordering, uint32 view)
{
    uint32 state, values, packet, scratch, config, target, low, high, value, slot, link;
    sint32 selector, expected, index, x, y, origin_x, origin_y;

    FUNCTION_MARKER(0x800DAE44, "ACE3.BIN");
    selector = r_s16(command + 12);
    if (selector)
    {
        state = r_u32(0x800DD9DC);
        values = r_u32(state + 76);
        expected = r_s16(command + 14);
        value = r_u8(values + (uint32)selector);
        if (expected != (sint32)value)
            return command + 20;
    }
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    scratch = r_u32(0x800DD9D8);
    w_u32(state + 4, packet + 16);
    high = (uint32)r_s16(command + 2);
    low = r_u16(command);
    w_u32(scratch, low | (high << 16));
    high = (uint32)r_s16(command + 6);
    low = r_u16(command + 4);
    w_u32(scratch + 4, low | (high << 16));
    config = r_u32(state + 44);
    index = r_s16(config + 10);
    target = r_u32(0x800DD924 + (uint32)index * 4);
    if (!resource001_transform_call(target))
        return command + 20;
    scratch = r_u32(0x800DD9D8);
    origin_x = r_s16(view + 4);
    x = r_s16(scratch);
    origin_y = r_s16(view + 6);
    y = r_s16(scratch + 2);
    w_u32(packet + 8, ((uint32)(x + origin_x) & 0xFFFF) | ((uint32)(y + origin_y) << 16));
    high = (uint32)r_s16(scratch + 6);
    low = r_u16(scratch + 4);
    w_u32(packet + 12, low | (high << 16));
    value = r_u8(command + 8);
    w_u8(packet + 4, value);
    value = r_u8(command + 9);
    w_u8(packet + 5, value);
    value = r_u8(command + 10);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x60);
    w_u8(packet + 6, value);
    value = r_u8(command + 11);
    w_u8(packet + 7, value ? 0x62 : 0x60);
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, link | 0x03000000);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    w_u32(ordering + slot * 4, packet & 0xFFFFFF);
    return command + 20;
}

uint32 resource001_sub_800db928(uint32 command, uint32 ordering, uint32 view)
{
    uint32 state, values, packet, scratch, config, target, low, high, value, slot, tag, link, coordinate, origin;
    sint32 selector, expected, index;

    FUNCTION_MARKER(0x800DB928, "ACE3.BIN");
    selector = r_s16(command + 12);
    if (selector)
    {
        state = r_u32(0x800DD9DC);
        values = r_u32(state + 76);
        expected = r_s16(command + 14);
        value = r_u8(values + (uint32)selector);
        if (expected != (sint32)value)
            return command + 20;
    }
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    scratch = r_u32(0x800DD9D8);
    w_u32(state + 4, packet + 16);
    high = (uint32)r_s16(command + 2);
    low = r_u16(command);
    w_u32(scratch, low | (high << 16));
    high = (uint32)r_s16(command + 6);
    low = r_u16(command + 4);
    w_u32(scratch + 4, low | (high << 16));
    config = r_u32(state + 44);
    index = r_s16(config + 10);
    target = r_u32(0x800DD924 + (uint32)index * 4);
    if (!resource001_transform_call(target))
        return command + 20;
    scratch = r_u32(0x800DD9D8);
    origin = r_u16(view + 4);
    coordinate = r_u16(scratch);
    w_u16(packet + 8, coordinate + origin);
    coordinate = r_u16(scratch + 2);
    origin = r_u16(view + 6);
    w_u16(packet + 10, coordinate + origin);
    coordinate = r_u16(scratch + 4);
    origin = r_u16(view + 4);
    w_u16(packet + 12, coordinate + origin);
    coordinate = r_u16(scratch + 6);
    origin = r_u16(view + 6);
    w_u16(packet + 14, coordinate + origin);
    value = r_u8(command + 8);
    w_u8(packet + 4, value);
    value = r_u8(command + 9);
    w_u8(packet + 5, value);
    value = r_u8(command + 10);
    w_u8(packet + 3, 3);
    w_u8(packet + 7, 0x40);
    w_u8(packet + 6, value);
    value = r_u8(command + 11);
    w_u8(packet + 7, value ? 0x42 : 0x40);
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    tag = r_u32(packet);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    link = r_u32(ordering + slot * 4);
    w_u32(ordering + slot * 4, (link & 0xFF000000) | (packet & 0xFFFFFF));
    return command + 20;
}

uint32 resource001_sub_800db024(uint32 command, uint32 ordering, uint32 view)
{
    uint32 state, values, packet, config, target, value, slot, link, u, v, clut;
    sint32 selector, expected, index, x, y, origin_x, origin_y, width, height, texture_x, texture_y;

    FUNCTION_MARKER(0x800DB024, "ACE3.BIN");
    selector = r_s16(command + 20);
    if (selector)
    {
        state = r_u32(0x800DD9DC);
        values = r_u32(state + 76);
        expected = r_s16(command + 22);
        value = r_u8(values + (uint32)selector);
        if (expected != (sint32)value)
            return command + 28;
    }
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    w_u32(state + 4, packet + 20);
    config = r_u32(state + 44);
    index = r_s16(config + 10);
    target = r_u32(0x800DD924 + (uint32)index * 4);
    if (!resource001_transform_call(target))
        return command + 28;
    x = r_s16(command);
    origin_x = r_s16(view + 4);
    origin_y = r_s16(view + 6);
    y = r_s16(command + 2);
    w_u32(packet + 8, ((uint32)(x + origin_x) & 0xFFFF) | ((uint32)(y + origin_y) << 16));
    width = r_s16(command + 16);
    height = r_s16(command + 18);
    w_u32(packet + 16, (((uint32)width << 2) & 0xFFFF) | ((uint32)height << 16));
    texture_x = r_s16(command + 12);
    texture_y = r_s16(command + 14);
    u = (uint32)(texture_x % 64) * 4;
    v = (uint32)(texture_y % 256) << 8;
    y = r_s16(command + 10);
    value = r_u16(command + 8);
    clut = ((uint32)y << 6) | ((value >> 4) & 63);
    w_u32(packet + 12, u | v | (clut << 16));
    value = r_u8(command + 4);
    w_u8(packet + 4, value);
    value = r_u8(command + 5);
    w_u8(packet + 5, value);
    value = r_u8(command + 6);
    w_u8(packet + 3, 4);
    w_u8(packet + 7, 0x64);
    w_u8(packet + 6, value);
    value = r_u8(command + 7);
    w_u8(packet + 7, value ? 0x66 : 0x64);
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, link | 0x04000000);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    w_u32(ordering + slot * 4, packet & 0xFFFFFF);
    return command + 28;
}

uint32 resource001_sub_800db374(uint32 command, uint32 ordering, uint32 view)
{
    uint32 state, values, packet, config, target, value, blend = 0, scratch, low, high;
    uint32 width_span, height_span, top, bottom, left, right, clut, depth, page_y, page_x, page, slot, link;
    sint32 selector, expected, index, scale, texture_x, texture_y, flip, u, v, width, actual, height;
    sint32 x, y, origin_x, origin_y;
    int semitransparent;

    FUNCTION_MARKER(0x800DB374, "ACE3.BIN");
    selector = r_s16(command + 24);
    if (selector)
    {
        state = r_u32(0x800DD9DC);
        values = r_u32(state + 76);
        expected = r_s16(command + 26);
        value = r_u8(values + (uint32)selector);
        if (expected != (sint32)value)
            return command + 32;
    }
    state = r_u32(0x800DD9DC);
    packet = r_u32(state + 4);
    w_u32(state + 4, packet + 40);
    value = r_u8(command + 7);
    semitransparent = value != 255;
    if (semitransparent)
        blend = r_u8(command + 7);
    high = (uint32)r_s16(command + 2);
    low = r_u16(command);
    scratch = r_u32(0x800DD9D8);
    w_u32(scratch, low | (high << 16));
    high = (uint32)r_s16(command + 22);
    low = r_u16(command + 20);
    w_u32(scratch + 4, low | (high << 16));
    config = r_u32(state + 44);
    index = r_s16(config + 10);
    target = r_u32(0x800DD924 + (uint32)index * 4);
    if (!resource001_transform_call(target))
        return command + 32;
    scale = r_u8(command + 30) ? 2 : 4;
    flip = r_s16(command + 20);
    texture_x = r_s16(command + 12);
    u = (texture_x % 64) * scale;
    if (flip < 0)
        --u;
    flip = r_s16(command + 22);
    texture_y = r_s16(command + 14);
    v = texture_y % 256;
    if (flip < 0)
        --v;
    width = r_s16(command + 16);
    actual = r_s16(command + 20);
    width *= scale;
    width_span = actual == width ? (uint32)actual : (uint32)width - 1;
    actual = r_s16(command + 22);
    height = r_s16(command + 18);
    value = r_u16(command + 18);
    height_span = actual == height ? value : value - 1;
    if ((sint16)u + (sint16)width_span == 256)
        --width_span;
    if ((sint16)v + (sint16)height_span == 256)
        --height_span;
    value = r_u8(command + 4);
    w_u8(packet + 4, value);
    value = r_u8(command + 5);
    w_u8(packet + 5, value);
    value = r_u8(command + 6);
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 0x2C);
    w_u8(packet + 6, value);
    w_u8(packet + 7, semitransparent ? 0x2E : 0x2C);
    left = (uint32)(sint32)(sint16)u;
    top = (uint32)(sint32)(sint16)v << 8;
    y = r_s16(command + 10);
    value = r_u16(command + 8);
    clut = ((uint32)y << 6) | ((value >> 4) & 63);
    bottom = (uint32)((sint16)v + (sint16)height_span) << 8;
    w_u32(packet + 12, left | top | (clut << 16));
    depth = r_u8(command + 30);
    page_y = r_u16(command + 14);
    page_x = r_u16(command + 12);
    w_u32(packet + 28, left | bottom);
    right = left + (uint32)(sint32)(sint16)width_span;
    w_u32(packet + 36, right | bottom);
    page = ((depth & 3) << 7) | ((blend & 3) << 5) | ((page_y & 0x100) >> 4) | ((page_x & 0x3FF) >> 6) | ((page_y & 0x200) << 2);
    w_u32(packet + 20, right | top | (page << 16));
    scratch = r_u32(0x800DD9D8);
    origin_y = r_s16(view + 6);
    y = r_s16(scratch + 2);
    origin_x = r_s16(view + 4);
    x = r_s16(scratch);
    w_u32(packet + 8, ((uint32)(y + origin_y) << 16) | ((uint32)(x + origin_x) & 0xFFFF));
    y = r_s16(scratch + 2);
    origin_y = r_s16(view + 6);
    origin_x = r_s16(view + 4);
    width = r_s16(scratch + 4);
    x = r_s16(scratch);
    w_u32(packet + 16, ((uint32)(y + origin_y) << 16) | ((uint32)(x + origin_x + width) & 0xFFFF));
    y = r_s16(scratch + 2);
    origin_y = r_s16(view + 6);
    height = r_s16(scratch + 6);
    x = r_s16(scratch);
    origin_x = r_s16(view + 4);
    w_u32(packet + 24, ((uint32)(y + origin_y + height) << 16) | ((uint32)(x + origin_x) & 0xFFFF));
    y = r_s16(scratch + 2);
    origin_y = r_s16(view + 6);
    height = r_s16(scratch + 6);
    width = r_s16(scratch + 4);
    x = r_s16(scratch);
    origin_x = r_s16(view + 4);
    w_u32(packet + 32, ((uint32)(y + origin_y + height) << 16) | ((uint32)(x + origin_x + width) & 0xFFFF));
    state = r_u32(0x800DD9DC);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    link = r_u32(ordering + slot * 4);
    w_u32(packet, link | 0x09000000);
    config = r_u32(state + 44);
    slot = (uint32)r_s16(config + 14);
    w_u32(ordering + slot * 4, packet & 0xFFFFFF);
    return command + 32;
}

uint32 resource001_sub_800dc5ec(void)
{
    uint8 no_arguments[1] = {0};
    uint32 state, page, row, index, value, x, y;

    FUNCTION_MARKER(0x800DC5EC, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    page = r_u8(state + 92);
    row = r_u32(state + page * 4 + 100);
    resource001_sub_800d7d08(3);
    resource001_sub_800d7c88(2);
    resource001_sub_800d7ce4(1, 30, 40);
    value = (uint32)resource001_sub_800da8bc(15);
    resource001_sub_800d7cd4(value);
    resource001_sub_800d7c60(128, 128, 128);
    for (index = 0; index < 50; ++index)
    {
        x = (uint32)r_s16(row);
        y = (uint32)r_s16(row + 2);
        resource001_sub_800d7c40(x, y);
        resource001_sub_800d8aa0(row + 4, 0, no_arguments, 0);
        row += 8;
    }
    return 0;
}

/* Preserve each recovered command producer's semantic argument count */
static uint32 resource001_packet_call(uint32 target, uint32 command, uint32 ordering, uint32 view)
{
    switch (target)
    {
        case 0x800DAE44:
            return resource001_sub_800dae44(command, ordering, view);
        case 0x800DB024:
            return resource001_sub_800db024(command, ordering, view);
        case 0x800DB254:
            return resource001_sub_800db254(command, ordering);
        case 0x800DB374:
            return resource001_sub_800db374(command, ordering, view);
        case 0x800DB834:
            return resource001_sub_800db834(command, ordering);
        case 0x800DB910:
            return resource001_sub_800db910(command);
        case 0x800DB928:
            return resource001_sub_800db928(command, ordering, view);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 packet producer %08X\n", target);
#endif
            abort();
    }
}

/* Original callers discard the final callback's volatile return carrier */
static void resource001_render_callback(uint32 target)
{
    switch (target)
    {
        case 0x800DC42C:
            resource001_sub_800dc42c();
            return;
        case 0x800DC434:
            resource001_sub_800dc434();
            return;
        case 0x800DC5E4:
            resource001_sub_800dc5e4();
            return;
        case 0x800DC5EC:
            resource001_sub_800dc5ec();
            return;
        case 0x800DC694:
            resource001_sub_800dc694();
            return;
        case 0x800DC69C:
            resource001_sub_800dc69c();
            return;
        case 0x800DC6A4:
            resource001_sub_800dc6a4();
            return;
        case 0x800DC6AC:
            resource001_sub_800dc6ac();
            return;
        case 0x800DC6B4:
            resource001_sub_800dc6b4();
            return;
        case 0x800DC6BC:
            resource001_sub_800dc6bc();
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 render callback %08X\n", target);
#endif
            abort();
    }
}

void resource001_sub_800dc6c4(uint32 index)
{
    uint32 state, commands, cursor, ordering, opcode, view, target;

    FUNCTION_MARKER(0x800DC6C4, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    commands = r_u32(state + 36);
    cursor = r_u32(commands + index * 4);
    ordering = r_u32(state + 16);
    opcode = r_u32(cursor);
    if (opcode != 0xFFFFFFFF)
    {
        do
        {
            opcode = r_u32(cursor);
            if (opcode < 7)
            {
                state = r_u32(0x800DD9DC);
                view = r_u32(state + 32);
                target = r_u32(0x800DD934 + opcode * 4);
                cursor = resource001_packet_call(target, cursor + 4, ordering, view + index * 12);
                opcode = r_u32(cursor);
            }
        } while (opcode != 0xFFFFFFFF);
    }
    target = r_u32(0x800DD950 + index * 4);
    resource001_render_callback(target);
}

void resource001_sub_800dc7ac(void)
{
    uint32 state, bits, index = 0;

    FUNCTION_MARKER(0x800DC7AC, "ACE3.BIN");
    state = r_u32(0x800DD9DC);
    bits = r_u32(state + 28);
    while (bits && index < 32)
    {
        if (bits & 1)
            resource001_sub_800dc6c4(index);
        ++index;
        bits >>= 1;
    }
}

uint32 resource001_sub_800dcaa4(void)
{
    uint32 game, index, state, buffer;

    FUNCTION_MARKER(0x800DCAA4, "ACE3.BIN");
    w_u32(0x800DD9D8, 0x1F800000);
    VSync(1);
    game = r_u32(0x800BCE60);
    index = r_u32(game + 4);
    state = r_u32(0x800DD9DC);
    buffer = r_u32(state + index * 4 + 8);
    w_u32(state + 4, buffer);
    resource001_sub_800dc7ac();
    return resource001_sub_800da8e0();
}

uint32 resource001_sub_800cd07c(void)
{
    uint8 no_arguments[1] = {0};
    uint32 state, timer, game;

    FUNCTION_MARKER(0x800CD07C, "ACE3.BIN");
    state = r_u32(0x800DD9D0);
    timer = r_u32(state + 72);
    game = r_u32(0x800BCE60);
    --timer;
    w_u32(state + 72, timer);
    if (!r_u32(game + 68))
    {
        if (!(r_u32(game + 0x96C) & 2) || (r_u32(game + 0x12C) & 0x10) || (sint32)timer < 0)
            return resource001_sub_800ccfa4();
    }
    game = r_u32(0x800BCE60);
    resource001_sub_800d81f0(r_u32(game + 8));
    resource001_sub_800d7d08(7);
    resource001_sub_800d7c88(0xFFFFFFFF);
    resource001_sub_800d7ce4(1, 1, 1);
    resource001_sub_800d7cd4(1);
    resource001_sub_800d7c60(24, 184, 208);
    game = r_u32(0x800BCE60);
    if (r_u32(game + 0x96C) & 2)
    {
        resource001_sub_800d7c40(233, 145);
        resource001_sub_800d8aa0(0x800CBFEC, 0, no_arguments, 0);
    }
    game = r_u32(0x800BCE60);
    resource001_sub_800dc9ec(r_u32(game + 8));
    resource001_sub_800dcaa4();
    return 0;
}

void sub_8009b904(void)
{
    uint32 state;

    FUNCTION_MARKER(0x8009B904, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    w_u32(state + 68, 0);
}

uint32 resource001_sub_800da25c(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800DA25C, "ACE3.BIN");
    state = arena_alloc(0x101C);
    w_u32(state + 12, 0x3FFF);
    w_u32(state + 4, 0);
    w_u32(state + 8, 0);
    w_u32(state, 0);
    w_u32(state + 16, 255);
    w_u32(state + 20, 0);
    w_u32(0x800DDA60, state);
    return state;
}

uint32 resource001_sub_800d4f0c(void)
{
    uint32 state, buffer, index;

    FUNCTION_MARKER(0x800D4F0C, "ACE3.BIN");
    state = arena_alloc(0x360);
    w_u32(0x800DDA48, state);
    buffer = arena_alloc(128);
    state = r_u32(0x800DDA48);
    w_u32(state + 744, buffer);
    for (index = 0; index < 4; ++index)
    {
        buffer = arena_alloc(0x480);
        state = r_u32(0x800DDA48);
        if (index < 3)
            w_u32(state + index * 4 + 748, buffer);
        else
            w_u32(state + 760, buffer);
    }
    for (index = 0; index < 7; ++index)
    {
        buffer = arena_alloc(0x180);
        state = r_u32(0x800DDA48);
        if (index < 6)
            w_u32(state + index * 4 + 764, buffer);
        else
            w_u32(state + 788, buffer);
    }
    return 0;
}

uint32 resource001_sub_800d51fc(void)
{
    uint32 state, buffer, index;

    FUNCTION_MARKER(0x800D51FC, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + 744);
    w_u32(buffer + 4, 0x990426);
    buffer = r_u32(state + 744);
    w_u8(buffer + 8, 0);
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + 744);
    w_u8(buffer + 9, 0);
    for (index = 0; index < 112; ++index)
    {
        state = r_u32(0x800DDA48);
        buffer = r_u32(state + 744);
        w_u8(buffer + index + 16, 0);
    }
    state = r_u32(0x800DDA48);
    buffer = r_u32(state + 744);
    w_u32(buffer + 12, 0);
    return buffer;
}

void resource001_sub_800d5548(void)
{
    FUNCTION_MARKER(0x800D5548, "ACE3.BIN");
}

uint32 resource001_sub_800d3ce8(void)
{
    uint32 state, other;

    FUNCTION_MARKER(0x800D3CE8, "ACE3.BIN");
    state = r_u32(0x800DDA48);
    w_u8(state + 844, 1);
    other = r_u32(0x800DDA48);
    w_u32(state + 40, 1);
    w_u8(other + 840, 0);
    state = r_u32(0x800DDA48);
    w_u8(state + 842, 0);
    state = r_u32(0x800DDA48);
    w_u8(state + 848, 0);
    other = r_u32(0x800DDA48);
    w_u16(state + 48, 0);
    w_u8(other + 845, 1);
    state = r_u32(0x800DDA48);
    w_u8(state + 841, 0);
    state = r_u32(0x800DDA48);
    w_u8(state + 843, 0);
    state = r_u32(0x800DDA48);
    w_u8(state + 849, 0);
    other = r_u32(0x800DDA48);
    w_u16(state + 50, 0);
    w_u32(other + 852, 0);
    return 1;
}

uint32 resource001_sub_800cd1a0(void)
{
    uint32 state, buffer, modes, index, packet, mode;

    FUNCTION_MARKER(0x800CD1A0, "ACE3.BIN");
    state = arena_alloc(36);
    w_u32(0x800DDA18, state);
    w_u32(state + 24, 0);
    w_u32(state + 28, 0);
    w_u32(state + 32, 0);
    for (index = 0; index < 2; ++index)
    {
        buffer = arena_alloc(20000);
        state = r_u32(0x800DDA18);
        w_u32(state + index * 4, buffer);
        modes = arena_alloc(1200);
        state = r_u32(0x800DDA18);
        buffer = r_u32(state + index * 4);
        w_u32(state + index * 4 + 12, modes);
        for (packet = 0; packet < 1000; ++packet)
        {
            w_u8(buffer + packet * 20 + 3, 4);
            w_u8(buffer + packet * 20 + 7, 100);
            w_u16(buffer + packet * 20 + 16, 8);
            w_u16(buffer + packet * 20 + 18, 14);
            w_u16(buffer + packet * 20 + 14, 0x41A8);
        }
        state = r_u32(0x800DDA18);
        modes = r_u32(state + index * 4 + 12);
        for (mode = 0; mode < 100; ++mode)
            sub_800ac7f8(modes + mode * 12, 0, 0, 27, 0);
    }
    return 0;
}

uint32 resource001_sub_800d5290(void)
{
    uint32 state, block, slot, index, row, source, destination, game, value, first, tail;
    uint32 words[4];

    FUNCTION_MARKER(0x800D5290, "ACE3.BIN");
    for (slot = 0; slot < 4; ++slot)
    {
        state = r_u32(0x800DDA48);
        block = r_u32(state + (slot < 3 ? 748 + slot * 4 : 760));
        first = r_u32(0x800CC4BC);
        tail = r_u8(0x800CC4C0);
        w_u32(block + 8, first);
        w_u8(block + 12, tail);
        w_u16(block + 20, 0);
        w_u16(block + 22, 0);
        for (index = 0; index < 60; ++index)
        {
            w_u16(block + index * 2 + 394, 0);
            w_u16(block + index * 2 + 274, 0);
            w_u16(block + index * 2 + 154, 0);
            w_u16(block + index * 2 + 34, 0);
            w_u8(block + index + 514, 0);
            w_u8(block + index + 544, 0);
            w_u8(block + index + 574, 0);
            w_u8(block + index + 604, 0);
        }
        w_u16(block + 704, 0);
        w_u16(block + 706, 0);
        for (row = 0; row < 6; ++row)
        {
            game = r_u32(0x800BCE60);
            destination = block + 708 + row * 68;
            for (index = 0; index < 20; ++index)
            {
                value = r_u16(game + row * 92 + index * 2 + 370);
                w_u16(destination + index * 2, value);
            }
            game = r_u32(0x800BCE60);
            source = game + row * 92 + 412;
            /* Original ULW pairs and USW pairs are aligned on this allocation layout */
            for (index = 0; index < 4; ++index)
            {
                words[index] = r_u32(source + index * 4);
                words[index] = r_u32(source + index * 4);
            }
            for (index = 0; index < 4; ++index)
            {
                w_u32(destination + 40 + index * 4, words[index]);
                w_u32(destination + 40 + index * 4, words[index]);
            }
            for (index = 0; index < 2; ++index)
            {
                words[index] = r_u32(source + 16 + index * 4);
                words[index] = r_u32(source + 16 + index * 4);
            }
            for (index = 0; index < 2; ++index)
            {
                w_u32(destination + 56 + index * 4, words[index]);
                w_u32(destination + 56 + index * 4, words[index]);
            }
            value = r_u32(game + row * 92 + 436);
            w_u32(destination + 64, value);
        }
        w_u8(block + 1116, 0);
        w_u8(block + 1117, 204);
        w_u8(block + 1118, 204);
        game = r_u32(0x800BCE60);
        w_u16(block + 1122, 0);
        w_u16(block + 1120, 0);
        w_u16(block + 22, 0);
        value = r_u32(game + 288);
        w_u8(block + 1124, (value & 4) == 0);
        game = r_u32(0x800BCE60);
        w_u32(block + 1128, 0);
        if (r_u8(game + 924) & 1)
            w_u32(block + 1128, 1);
        game = r_u32(0x800BCE60);
        if (r_u32(game + 288) & 1)
        {
            value = r_u32(block + 1128);
            w_u32(block + 1128, value | 2);
        }
        game = r_u32(0x800BCE60);
        if (r_u32(game + 288) & 2)
        {
            value = r_u32(block + 1128);
            w_u32(block + 1128, value | 4);
        }
        game = r_u32(0x800BCE60);
        if (r_u8(game + 2394) == 1)
        {
            value = r_u32(block + 1128);
            w_u32(block + 1128, value | 8);
        }
        for (index = 0; index < 17; ++index)
            w_u8(block + 1148 - index, 0);
    }
    return 0;
}
