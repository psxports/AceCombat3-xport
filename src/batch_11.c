#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8007e530(uint32 kind, uint32 index, uint32 value, uint32 previous)
{
    uint32 packet, tag;
    FUNCTION_MARKER(0x8007E530, "ACE3.BIN");
    packet = sub_8007df28(2);
    previous = (previous & 0x1FF) << 14;
    tag = r_u32(packet);
    value <<= 23;
    w_u8(packet + 4, kind);
    w_u8(packet + 5, index);
    tag = (((tag & 0xFFFFFFF0) | 2) & 0xFFFFFF8F) | 0x40;
    tag &= 0x3FC2;
    w_u32(packet, tag | previous | value);
    return packet;
}

uint32 sub_8007f9cc(uint32 kind, uint32 value)
{
    uint32 state;
    FUNCTION_MARKER(0x8007F9CC, "ACE3.BIN");
    state = r_u32(0x800BCF90);
    w_u32(state + 0x3948, 1);
    state += 0x3948;
    w_u16(state + 4, kind);
    w_u16(state + 6, value);
    return state;
}

uint32 sub_8006cb10(void)
{
    FUNCTION_MARKER(0x8006CB10, "ACE3.BIN");
    return sub_8006c9b8();
}

uint32 sub_8006c9b8(void)
{
    uint32 root, masks, group, bits, index, bit, record, value, vx, vy, vz, a, b, flags;
    FUNCTION_MARKER(0x8006C9B8, "ACE3.BIN");
    root = r_u32(0x800BD080);
    masks = root + 0x33D0;
    xport_gte_write_data(8, 4000);
    for (group = 0; group < 128; group += 32)
    {
        bits = r_u32(masks);
        if (bits != 0)
        {
            for (index = 0; index < 32; ++index)
            {
                bit = 1u << index;
                if ((bits & bit) == 0)
                    continue;
                root = r_u32(0x800BD080);
                record = root + (group + index) * 60 + 0x15D0;
                value = r_u32(record + 4) - 1;
                w_u32(record + 4, value);
                if ((sint32)value < 0)
                {
                    bits &= ~bit;
                    continue;
                }
                vx = r_u32(record + 40);
                vy = r_u32(record + 44);
                a = r_u32(record + 8);
                vz = r_u32(record + 48);
                b = r_u32(record + 16);
                w_u32(record + 8, a + vx);
                a = r_u32(record + 12);
                w_u32(record + 16, b + vz);
                b = r_u32(record + 28);
                w_u32(record + 12, a + vy);
                a = r_u32(record + 24);
                w_u32(record + 28, b + vy);
                flags = r_u32(record);
                w_u32(record + 24, a + vx);
                a = r_u32(record + 32);
                w_u32(record + 32, a + vz);
                if ((flags & 2) == 0)
                    vy += 0x444;
                vx = (sint32)vx >> 6;
                vy = (sint32)vy >> 6;
                vz = (sint32)vz >> 6;
                if ((flags & 1) == 0)
                {
                    xport_gte_write_data(9, vx);
                    xport_gte_write_data(10, vy);
                    xport_gte_write_data(11, vz);
                    xport_gte_execute(0x198003D);
                    vx = xport_gte_read_data(25);
                    vy = xport_gte_read_data(26);
                    vz = xport_gte_read_data(27);
                }
                w_u32(record + 40, vx << 6);
                w_u32(record + 44, vy << 6);
                w_u32(record + 48, vz << 6);
            }
            w_u32(masks, bits);
        }
        masks += 4;
    }
    return 0;
}

uint32 sub_8006c6b8(uint32 index, uint32 kind, uint32 source, const NATIVE_POSE *native_source)
{
    uint32 root, record, flags, value;
    FUNCTION_MARKER(0x8006C6B8, "ACE3.BIN");
    root = r_u32(0x800BD080);
    record = root + 200 + index * 164;
    flags = r_u32(root + 0x1548);
    w_u32(root + 0x1548, flags | (1u << (index & 31)));
    w_u8(record + 52, kind);
    w_u8(record + 53, (sint32)kind >> 8);
    value = (native_source != 0 ? native_source->position[0] : r_u32(source));
    w_u32(record + 28, value);
    value = (native_source != 0 ? native_source->position[1] : r_u32(source + 4));
    w_u32(record + 32, value);
    value = (native_source != 0 ? native_source->position[2] : r_u32(source + 8));
    w_u32(record + 36, value);
    value = (native_source != 0 ? native_source->angles[0] : r_u16(source + 16));
    w_u16(record + 44, value);
    value = (native_source != 0 ? native_source->angles[1] : r_u16(source + 18));
    w_u16(record + 46, value);
    value = (native_source != 0 ? native_source->angles[2] : r_u16(source + 20));
    w_u16(record + 48, value);
    return value;
}

uint32 sub_8006c740(uint32 index)
{
    uint32 root, mask, flags;
    FUNCTION_MARKER(0x8006C740, "ACE3.BIN");
    root = r_u32(0x800BD080);
    mask = 1u << (index & 31);
    flags = r_u32(root + 0x1548);
    mask = ~mask;
    w_u32(root + 0x1548, flags & mask);
    return mask;
}

uint32 sub_800762d0(void)
{
    uint32 state;
    FUNCTION_MARKER(0x800762D0, "ACE3.BIN");
    state = r_u32(0x800BD094);
    w_u32(state, 2048);
    w_u32(state + 4, 4);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_80076274(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80076274, "ACE3.BIN");
    state = r_u32(0x800BD094);
    w_u32(state, 1);
    w_u32(state + 4, 1);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_8007953c(void)
{
    uint32 data, option;
    FUNCTION_MARKER(0x8007953C, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    option = r_u8(data + 927);
    return sub_800794d8(option);
}

uint32 sub_800794d8(uint32 option)
{
    uint32 scene, camera, flags, table, object;
    FUNCTION_MARKER(0x800794D8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    flags = r_u32(camera + 80) & 0x800;
    if (flags != 0)
        return flags;
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    sub_800793c4(camera, option != 0 ? 2 : 1, object);
    return sub_80035740(1);
}

uint32 sub_80079680(void)
{
    uint32 scene, table, camera, object;
    FUNCTION_MARKER(0x80079680, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    camera = r_u32(scene + 40);
    object = r_u32(table);
    sub_800793c4(camera, 512, object);
    return sub_80035740(0);
}

uint32 sub_800796b8(uint32 source, uint32 vector)
{
    uint32 scene, camera, value;
    FUNCTION_MARKER(0x800796B8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u32(source);
    w_u32(camera + 268, value);
    value = r_u32(source + 4);
    w_u32(camera + 272, value);
    value = r_u32(source + 8);
    w_u32(camera + 276, value);
    value = r_u16(source + 16);
    w_u16(camera + 284, value);
    value = r_u16(source + 18);
    w_u16(camera + 286, value);
    value = r_u16(source + 20);
    w_u16(camera + 288, value);
    value = r_u32(vector);
    w_u32(camera + 292, value);
    value = r_u32(vector + 4);
    w_u32(camera + 296, value);
    value = r_u32(vector + 8);
    w_u32(camera + 300, value);
    return value;
}

uint32 sub_8006e874(sint32 distance, uint32 vector)
{
    uint32 scale, x, y, z;
    FUNCTION_MARKER(0x8006E874, "ACE3.BIN");
    scale = distance > 86015 ? 4096 : distance / 21;
    y = (uint32)r_s16(vector + 2) * scale;
    z = (uint32)r_s16(vector + 4);
    x = (uint32)r_s16(vector);
    z *= scale;
    x *= scale;
    y = (sint32)y / 4096;
    z = (sint32)z / 4096;
    x = (sint32)x / 4096;
    w_u16(vector + 2, y);
    w_u16(vector + 4, z);
    w_u16(vector, x);
    return z;
}

uint32 sub_8002f19c(uint32 kind, uint32 value)
{
    uint32 slot, index;
    FUNCTION_MARKER(0x8002F19C, "ACE3.BIN");
    slot = 0x800C6550;
    for (index = 0; index < 8; ++index, slot += 4)
    {
        if (r_s16(slot + 2) == -1)
        {
            w_u8(slot, kind);
            w_u16(slot + 2, value);
            return index;
        }
    }
    return 0xFFFFFFFF;
}

uint32 sub_8002939c(uint32 kind)
{
    uint32 slot, index, occupied, priority;
    FUNCTION_MARKER(0x8002939C, "ACE3.BIN");
    slot = r_u32(0x800BCBA4);
    for (index = 0; index < 6; ++index, slot += 4)
    {
        occupied = r_u8(slot);
        if (occupied != 0)
        {
            priority = r_u8(slot + 1);
            if ((sint32)kind < (sint32)priority)
                continue;
        }
        w_u8(slot + 1, kind);
        return slot;
    }
    return 0;
}

uint32 sub_80029448(uint32 mode)
{
    uint32 data, result, slot;
    FUNCTION_MARKER(0x80029448, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    result = r_u32(data + 288) & 3;
    if (result != 3)
        return result;
    if (mode == 7 || mode == 11)
        return 11;
    slot = sub_8002939c(9);
    if (slot == 0)
        return 255;
    w_u8(slot + 2, 255);
    result = mode == 6 || mode == 10 ? 2 : 1;
    w_u8(slot, result);
    return result;
}

uint32 sub_800294c4(uint32 mode)
{
    uint32 data, result, slot, words[3], index, pair;
    FUNCTION_MARKER(0x800294C4, "ACE3.BIN");
    data = r_u32(0x800BCE60);
    result = r_u32(data + 288) & 3;
    if (result != 3)
        return result;
    slot = sub_8002939c(3);
    if (slot == 0)
        return 0x80010000;
    words[0] = r_u32(0x80010380);
    words[1] = r_u32(0x80010384);
    words[2] = r_u32(0x80010388);
    index = mode - 1;
    if (index >= 6)
    {
        w_u8(slot, 0);
        return 0;
    }
    pair = words[index >> 1] >> ((index & 1) * 16);
    w_u8(slot + 2, pair);
    w_u8(slot, pair >> 8);
    if (index != 2)
        w_u8(slot + 1, 1);
    return 1;
}

uint32 sub_80090110(uint32 value)
{
    uint32 root;
    FUNCTION_MARKER(0x80090110, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    w_u32(root + 0x394C, value);
    w_u32(root + 0x3948, 1);
    return sub_8007efbc(value & 15, (sint32)value >> 16);
}

uint32 sub_800714e0(void)
{
    uint32 state, result, flags;
    FUNCTION_MARKER(0x800714E0, "ACE3.BIN");
    state = r_u32(0x800BD094);
    result = r_s16(state + 86);
    if (result != 0)
        return result;
    flags = r_u32(state + 16);
    w_u32(state + 16, flags | 0x2000);
    sub_800713b0(0);
    sub_8002db4c(17);
    sub_800294c4(1);
    sub_80090110(0x20000);
    state = r_u32(0x800BD094);
    w_u16(state + 86, 2);
    return 2;
}

uint32 sub_8006b034(uint32 kind, uint32 position, uint32 vector, uint32 mode)
{
    uint32 root, count, mask_address, bits, group, bit, record;
    FUNCTION_MARKER(0x8006B034, "ACE3.BIN");
    root = r_u32(0x800BD080);
    count = r_u32(root + 0x154C);
    if ((sint32)count >= 8)
        return 0;
    mask_address = root + 0x33D0;
    bits = r_u32(mask_address);
    group = 0;
    while (bits == 0xFFFFFFFF)
    {
        group += 32;
        mask_address += 4;
        if (group >= 128)
            return 0;
        bits = r_u32(mask_address);
    }
    bit = 0;
    while ((bits & (1u << bit)) != 0)
        ++bit;
    bits = r_u32(mask_address);
    w_u32(mask_address, bits | (1u << bit));
    root = r_u32(0x800BD080);
    count = r_u32(root + 0x154C);
    w_u32(root + 0x154C, count + 1);
    record = root + 0x1550 + (count << 4);
    w_u16(record, bit + group);
    w_u16(record + 2, mode);
    w_u32(record + 4, kind);
    w_u32(record + 8, position);
    w_u32(record + 12, vector);
    return 1;
}

uint32 sub_800713b0(uint32 mode)
{
    MATRIX matrix;
    uint32 table, scene, object, vector, a, b, c, d, e, x, y, z, value, kind;
    FUNCTION_MARKER(0x800713B0, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    scene = r_u32(0x800BCE50);
    object = r_u32(table);
    vector = r_u32(scene + 12) + 144;
    rotation_build(object + 24, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(vector);
    xport_gte_write_data(0, a);
    b = r_u32(vector + 4);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    w_u32(0x800C7BC8, x);
    y = xport_gte_read_data(26);
    w_u32(0x800C7BCC, y);
    z = xport_gte_read_data(27);
    w_u32(0x800C7BD0, z);
    x = r_u32(0x800C7BC8) << 4;
    w_u32(0x800C7BC8, x);
    y = r_u32(0x800C7BCC);
    z = r_u32(0x800C7BD0);
    y <<= 4;
    z <<= 4;
    w_u32(0x800C7BCC, y);
    w_u32(0x800C7BD0, z);
    value = r_u32(object + 8);
    x += value;
    w_u32(0x800C7BC8, x);
    value = r_u32(object + 12);
    y += value;
    w_u32(0x800C7BCC, y);
    value = r_u32(object + 16);
    z += value;
    w_u32(0x800C7BD0, z);
    value = r_u16(object + 24);
    w_u16(0x800C7BD8, value);
    value = r_u16(object + 26);
    w_u16(0x800C7BDA, value);
    value = r_u16(object + 28);
    w_u16(0x800C7BDC, value);
    kind = r_u32(object);
    return sub_8006b034(kind, 0x800C7BC8, object + 56, mode);
}

uint32 sub_80079730(uint32 source, uint32 vector, uint32 mode)
{
    uint32 scene, camera, value;
    FUNCTION_MARKER(0x80079730, "ACE3.BIN");
    if (source != 0 && mode != 0)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(source);
        camera = r_u32(scene + 40);
        w_u32(camera + 308, value);
        value = r_u32(source + 4);
        w_u32(camera + 312, value);
        value = r_u32(source + 8);
        w_u32(camera + 316, value);
        value = r_u16(source + 16);
        w_u16(camera + 324, value);
        value = r_u16(source + 18);
        w_u16(camera + 326, value);
        value = r_u16(source + 20);
        w_u16(camera + 328, value);
        value = r_u32(vector);
        w_u32(camera + 332, value);
        value = r_u32(vector + 4);
        w_u32(camera + 336, value);
        value = r_u32(vector + 8);
        w_u32(camera + 340, value);
        camera = r_u32(scene + 40);
        value = r_s16(camera + 348);
        if (value == 0)
            w_u16(camera + 350, 1);
        scene = r_u32(0x800BCE54);
        camera = r_u32(scene + 40);
        w_u16(camera + 348, mode);
    }
    else
    {
        scene = r_u32(0x800BCE54);
        camera = r_u32(scene + 40);
        w_u16(camera + 348, 0);
    }
    return camera;
}

uint32 sub_80079ae8(uint32 state, const uint32 *native_context)
{
    uint32 vector, x, y, z;
    FUNCTION_MARKER(0x80079AE8, "ACE3.BIN");
    vector = (native_context != 0 ? native_context[2] : r_u32(state + 8));
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    return sub_80083f40(x, y, z) << 3;
}

uint32 sub_80031cd8(uint32 vector, uint32 angles, const uint32 *native_vector)
{
    uint32 raw_x, raw_y, raw_z, x, y, z, ax, az, sum, length, shift, pitch, yaw, result;
    FUNCTION_MARKER(0x80031CD8, "ACE3.BIN");
    if (native_vector)
    {
        raw_x = native_vector[0];
        raw_y = native_vector[1];
        raw_z = native_vector[2];
    }
    else
    {
        raw_x = r_u32(vector);
        raw_y = r_u32(vector + 4);
        raw_z = r_u32(vector + 8);
    }
    x = (sint32)raw_x >> 12;
    y = (sint32)raw_y >> 12;
    z = (sint32)raw_z >> 12;
    ax = (sint32)x < 0 ? 0 - x : x;
    az = (sint32)z < 0 ? 0 - z : z;
    sum = ax + az;
    if ((sint32)sum > 0xAFFFF)
        length = 0x100000;
    else
    {
        shift = 0;
        ax = x;
        az = z;
        if ((sint32)sum > 0xB000)
        {
            ax = (sint32)raw_x >> 16;
            az = (sint32)raw_z >> 16;
            shift = 4;
        }
        length = sub_80083f40(ax, 0, az) << shift;
    }
    pitch = sub_80030d54(0 - y, length);
    yaw = sub_80030d54(x, z);
    if ((sint32)pitch >= 4097)
    {
        pitch = 0x2000 - pitch;
        yaw += 0x2000;
    }
    if ((sint32)pitch < -4096)
    {
        pitch = 0xFFFFE000 - pitch;
        yaw += 0x2000;
    }
    if ((sint32)yaw >= 8192)
        yaw -= 0x4000;
    result = (sint32)yaw < -8192;
    if (result != 0)
        yaw += 0x4000;
    w_u16(angles, pitch);
    w_u16(angles + 2, yaw);
    w_u16(angles + 4, 0);
    return result;
}

uint32 sub_8007a060(uint32 state, uint32 position, uint32 output, const uint32 *native_context)
{
    uint32 origin, x, y, z, a, b, c, difference[3], value;
    FUNCTION_MARKER(0x8007A060, "ACE3.BIN");
    origin = (native_context != 0 ? native_context[0] : r_u32(state));
    x = r_u32(position);
    y = r_u32(position + 4);
    z = r_u32(position + 8);
    a = r_u32(origin);
    b = r_u32(origin + 4);
    c = r_u32(origin + 8);
    difference[0] = a - x;
    difference[1] = b - y;
    difference[2] = c - z;
    sub_80031cd8(0, output + 16, difference);
    value = r_u32(position);
    w_u32(output, value);
    value = r_u32(position + 4);
    w_u32(output + 4, value);
    value = r_u32(position + 8);
    w_u32(output + 8, value);
    return value;
}

uint32 sub_8007995c(uint32 state, uint32 scale, uint32 output, uint32 *native_output, const uint32 *native_context)
{
    uint32 vector, origin, x, y, z, ox, oy, oz, a, b, c;
    FUNCTION_MARKER(0x8007995C, "ACE3.BIN");
    vector = (native_context != 0 ? native_context[2] : r_u32(state + 8));
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    x = (sint32)x >> 5;
    y = (sint32)y >> 5;
    z = (sint32)z >> 5;
    origin = (native_context != 0 ? native_context[0] : r_u32(state));
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x190003D);
    oy = r_u32(origin + 4);
    oz = r_u32(origin + 8);
    ox = r_u32(origin);
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    b += oy;
    c += oz;
    if (native_output)
    {
        native_output[1] = b;
        native_output[2] = c;
    }
    else
    {
        w_u32(output + 4, b);
        w_u32(output + 8, c);
    }
    a += ox;
    if (native_output)
        native_output[0] = a;
    else
        w_u32(output, a);
    return a;
}

uint32 sub_8007fe28(uint32 position, uint32 kind, const uint32 *native_position)
{
    uint32 root, x, y, z, record, base, remaining, random, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x8007FE28, "ACE3.BIN");
    if (kind >= 6)
        return 0;
    root = r_u32(0x800BCF90);
    if (native_position)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        base = 50;
        w_u32(record, 0x8FFFF);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        if (kind == 2)
            base = 100;
        w_u16(record + 24, 0x100);
        random = game_random();
        w_u16(record + 26, base + (random & 31));
    }
    remaining = kind == 2 ? 5 : 2;
    do
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            break;
        w_u16(record, 0);
        w_u16(record + 2, remaining + 29);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 127) - 64) << 10);
        py = y + ((((uint32)((sint32)random >> 3) & 127) - 64) << 10);
        pz = z + ((((uint32)((sint32)random >> 6) & 127) - 64) << 10);
        w_u16(record + 16, (rotated & 127) - 64);
        w_u16(record + 18, ((uint32)((sint32)rotated >> 3) & 127) - 80);
        w_u16(record + 20, ((uint32)((sint32)rotated >> 6) & 127) - 64);
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        if (remaining == 1)
        {
            w_u16(record + 24, 0x1604);
            w_u16(record + 26, (rotated & 63) + 110);
            w_u16(record + 22, 0);
            value = 0x91;
        }
        else
        {
            w_u16(record + 26, remaining + (rotated & 3) + 11);
            w_u16(record + 24, 0);
            value = 0x188;
        }
        --remaining;
        w_u16(record + 22, value);
    } while (remaining != 0);
    if (kind < 3)
        return 1;
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    if (record == 0)
        return 4;
    if (kind == 3)
    {
        w_u32(record, 0xE0001);
        w_u16(record + 24, 0x801);
        random = game_random();
        value = (random & 31) + 144;
    }
    else
    {
        w_u32(record, 0x120001);
        w_u16(record + 24, kind == 4 ? 0xC00 : 0xC03);
        random = game_random();
        value = (random & 31) + 184;
    }
    w_u16(record + 26, value);
    w_u32(record + 4, x);
    w_u32(record + 8, y);
    w_u32(record + 12, z);
    random = game_random();
    w_u16(record + 16, (random & 63) - 32);
    w_u16(record + 18, ((uint32)((sint32)random >> 3) & 63) - 132);
    w_u16(record + 20, ((uint32)((sint32)random >> 6) & 63) - 32);
    w_u16(record + 22, 144);
    return 144;
}

uint32 sub_800800e0(uint32 position, uint32 kind)
{
    uint32 packet, value;
    FUNCTION_MARKER(0x800800E0, "ACE3.BIN");
    packet = sub_8007ece8(0, position, 0);
    value = r_u32(packet + 8);
    value = (value & 0xFFFFF00F) | ((kind & 255) << 4);
    w_u32(packet + 8, value);
    return sub_8007fe28(position, kind, 0);
}

uint32 sub_80080534(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 root, x, y, z, record, random, sx, sy, sz, index, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x80080534, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    if (native_position)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        w_u32(record, 0xEFFFF);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u32(record + 24, 0x1000100);
    }
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record != 0)
    {
        random = game_random();
        w_u16(record, 0);
        w_u16(record + 2, (random & 3) + 9);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 20, 0);
        w_u16(record + 22, (random & 63) + 400);
    }
    if (native_vector)
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    else
    {
        sx = r_u32(vector);
        sy = r_u32(vector + 4);
        sz = r_u32(vector + 8);
    }
    sx = (sint32)sx >> 9;
    sy = (sint32)sy >> 9;
    sz = ((uint32)((sint32)sz >> 9)) - 128;
    for (index = 0; index < 8; ++index)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return index + 32;
        w_u16(record, 0);
        w_u16(record + 2, index + 32);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 8);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 8);
        w_u32(record + 4, px);
        w_u16(record + 16, sx - 128 + (rotated & 255));
        w_u16(record + 18, sy + ((uint32)((sint32)rotated >> 3) & 255) - 256);
        w_u16(record + 20, sz + ((uint32)((sint32)rotated >> 6) & 255));
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        if (index < 2)
        {
            w_u16(record + 22, 0x93);
            w_u16(record + 24, 0x1504);
            value = (rotated & 31) + 240;
        }
        else
        {
            w_u16(record + 22, 0x188);
            w_u16(record + 24, 0);
            value = index + 20;
        }
        w_u16(record + 26, value);
    }
    return 0;
}

uint32 sub_8008074c(uint32 position, uint32 vector, uint32 kind)
{
    uint32 packet;
    FUNCTION_MARKER(0x8008074C, "ACE3.BIN");
    packet = sub_8007ed9c(0, position, vector, 0);
    w_u8(packet + 2, kind);
    return sub_80080534(position, vector, 0, 0);
}

uint32 sub_8002ea24(uint32 voice, uint32 direction)
{
    uint32 state, value;
    FUNCTION_MARKER(0x8002EA24, "ACE3.BIN");
    if (voice == 255)
        return 255;
    state = 0x800C5830 + voice * 20;
    value = r_u8(state + 1);
    w_u8(state + 1, value | 0x10);
    if ((sint32)direction < 0)
    {
        value = r_u16(state + 4);
        w_u16(state + 4, 0 - value);
    }
    return voice;
}

uint32 sub_8002c6f4(uint32 argument)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8002C6F4, "ACE3.BIN");
    first = r_u32(0x800BCBBC);
    w_u8(first + 332, 6);
    second = r_u32(0x800BCBBC);
    w_u32(first + 328, 1);
    w_u32(second + 336, argument);
    w_u32(second + 348, 0);
    w_u8(second + 333, 0);
    return 1;
}

uint32 sub_8002abe0(uint32 enabled)
{
    uint32 state, flags, previous;
    FUNCTION_MARKER(0x8002ABE0, "ACE3.BIN");
    state = r_u32(0x800BCBAC);
    flags = r_u32(state);
    previous = (flags >> 30) & 1;
    if ((sint32)flags >= 0)
        return 0xFFFFFFFF;
    if (enabled != 0)
    {
        if (flags & 0x40000000)
            return previous;
    }
    else if ((flags & 0x40000000) == 0)
        return previous;
    w_u32(state, flags | 0x40);
    return previous;
}

uint32 sub_8002c41c(uint32 request)
{
    uint32 value, state;
    FUNCTION_MARKER(0x8002C41C, "ACE3.BIN");
    value = r_u32(request + 4);
    sub_8002abe0(value);
    state = r_u32(0x800BCBBC);
    value = r_u32(request + 4);
    w_u32(state + 376, value);
    return 1;
}

uint32 sub_80031fa8(uint32 index)
{
    uint32 table, script;
    FUNCTION_MARKER(0x80031FA8, "ACE3.BIN");
    table = r_u32(0x800BCEAC);
    script = r_u32(table + (index << 2) + 24);
    return sub_80054ebc(script);
}

uint32 sub_800304cc(void)
{
    FUNCTION_MARKER(0x800304CC, "ACE3.BIN");
    sub_8002f620();
    return 0;
}

uint32 sub_80031efc(void)
{
    FUNCTION_MARKER(0x80031EFC, "ACE3.BIN");
    return resource_cancel_streams();
}

uint32 sub_8007f8e4(void)
{
    uint32 scene, mode, result, data, active, flags, argument;
    FUNCTION_MARKER(0x8007F8E4, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    mode = r_u32(scene + 4);
    if (mode == 128)
        return sub_8007f448();
    if (mode == 512)
        return sub_8007f808();
    result = sub_8007f5b4();
    data = r_u32(0x800BCE60);
    active = r_u8(data + 933);
    if (active != 0)
    {
        flags = r_u32(data + 300);
        if ((flags & 0x10) == 0 && result != 1)
            return 1;
    }
    else
    {
        flags = r_u32(data + 300);
        if ((flags & 0x40) == 0)
        {
            if (result != 1)
                return 1;
            sub_80079680();
            sub_8007f298();
            return sub_8007ec54(0);
        }
    }
    scene = r_u32(0x800BCE54);
    argument = r_u32(scene + 8);
    return sub_80032f18(512, argument);
}

uint32 sub_800330cc(void)
{
    FUNCTION_MARKER(0x800330CC, "ACE3.BIN");
    sub_8007f8e4();
    sub_8006cb10();
    sub_8007dac8();
    return sub_800850d8();
}

uint32 sub_80089e48(uint32 vector)
{
    uint32 x, y, z, yaw, pitch, a, b, length;
    FUNCTION_MARKER(0x80089E48, "ACE3.BIN");
    x = r_s16(vector);
    z = r_s16(vector + 4);
    y = r_s16(vector + 2);
    if (x == 0 && z == 0)
    {
        yaw = 0;
        pitch = (sint32)y > 0 ? 0xFFFFFC00 : 0x400;
    }
    else
    {
        xport_gte_write_data(9, x);
        xport_gte_write_data(10, z);
        yaw = (uint32)ratan2(x, z);
        xport_gte_execute(0xA00428);
        a = xport_gte_read_data(25);
        b = xport_gte_read_data(26);
        length = sub_80083d88(a + b);
        pitch = (uint32)ratan2(0 - y, length);
    }
    return (yaw << 16) | (pitch & 0xFFFF);
}

uint32 sub_800809f0(uint32 position, uint32 vector, uint32 normal, uint32 kind)
{
    uint32 direction, packet, value;
    FUNCTION_MARKER(0x800809F0, "ACE3.BIN");
    direction = sub_80089e48(normal);
    packet = sub_8007eeac(0, position, vector, 0, 0);
    value = r_u32(packet + 16);
    w_u8(packet + 2, kind);
    value = ((value & 0xFFFFF000) | (direction & 0xFFF)) & 0xFF000FFF;
    value |= (((uint32)((sint32)direction >> 16)) & 0xFFF) << 12;
    w_u32(packet + 16, value);
    return sub_800807a4(position, vector, direction, 0, 0);
}

uint32 sub_80069c8c(uint32 state)
{
    uint32 flags, result;
    FUNCTION_MARKER(0x80069C8C, "ACE3.BIN");
    flags = r_u32(state);
    if (flags & 4)
    {
        sub_800809f0(state + 28, state + 140, state + 16, 0);
        return sub_8002f3c0(29, state + 28, 0, 0);
    }
    sub_8008074c(state + 28, state + 140, 0);
    result = (r_u32(state + 112) & 63) < 2;
    if (result == 0)
        return result;
    return sub_8002f3c0(28, state + 28, 0, 0);
}

uint32 sub_8004a018(uint32 index)
{
    uint32 root, state, table, mode, object;
    FUNCTION_MARKER(0x8004A018, "ACE3.BIN");
    root = r_u32(0x800BCEC0);
    state = root + index * 184;
    table = r_u32(0x800BCE5C);
    mode = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (mode == 1)
        return table;
    if (mode >= 2)
    {
        if (mode == 2)
            w_u8(state + 99, 255);
        else
            w_u8(state + 100, 0);
        return 255;
    }
    w_u8(state + 100, 1);
    w_u32(object + 56, 0);
    w_u32(object + 60, 0);
    w_u32(object + 64, 0);
    w_u32(object + 32, 0);
    w_u32(object + 36, 0);
    w_u32(object + 40, 0);
    w_u16(state + 128, 0);
    w_u16(state + 130, 0);
    w_u16(state + 132, 0);
    return 1;
}

uint32 sub_80081298(uint32 position, uint32 vector, uint32 mode)
{
    uint32 scene, flags, packet;
    FUNCTION_MARKER(0x80081298, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if (flags & 0x100)
        mode = 1;
    packet = sub_8007ed9c(1, position, vector, 0);
    w_u8(packet + 2, mode);
    return sub_80080f70(position, vector, mode, 0, 0);
}

uint32 sub_8004b308(uint32 index)
{
    uint32 table, root, state, mode, object, variant;
    FUNCTION_MARKER(0x8004B308, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BCEC0);
    state = root + index * 184;
    mode = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (mode == 1)
    {
        w_u8(state + 100, 2);
        return 2;
    }
    if (mode >= 2)
    {
        if (mode == 2)
            return sub_80038320(index);
        w_u8(state + 100, 0);
        return 2;
    }
    w_u8(state + 100, 1);
    variant = r_s16(object + 72) == 44;
    return sub_80081298(object + 8, object + 56, variant);
}

uint32 sub_8003ec0c(uint32 position, uint32 output, uint32 origin, uint16 *native_output, const uint16 *native_origin)
{
    uint32 x, z, base, value;
    sint32 out_x, out_z;
    FUNCTION_MARKER(0x8003EC0C, "ACE3.BIN");
    x = r_u32(position);
    z = r_u32(position + 8);
    x = ((uint32)((sint32)x >> 23)) + 128;
    z = ((uint32)((sint32)(0 - z) >> 23)) + 128;
    if ((sint32)x >= 256)
        x = 255;
    if ((sint32)x < 0)
        x = 0;
    if ((sint32)z >= 256)
        z = 255;
    if ((sint32)z < 0)
        z = 0;
    base = native_origin ? native_origin[0] : r_u16(origin);
    value = x - base;
    if (native_output)
        native_output[0] = value;
    else
        w_u16(output, value);
    base = native_origin ? native_origin[1] : r_u16(origin + 2);
    out_x = native_output ? (sint16)native_output[0] : r_s16(output);
    value = z - base;
    if (native_output)
        native_output[1] = value;
    else
        w_u16(output + 2, value);
    if (out_x < 0)
        return 0xFFFFFFFF;
    out_z = (sint16)value;
    if (out_z < 0 || out_x >= 128 || out_z >= 128)
        return 0xFFFFFFFF;
    return 0;
}

uint32 sub_80034360(uint32 x, uint32 y, uint32 ordering, uint32 packet, uint32 dimensions, uint32 color)
{
    uint32 value;
    FUNCTION_MARKER(0x80034360, "ACE3.BIN");
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    value = color | (color << 8) | (color << 16) | 0x66000000;
    w_u32(packet + 4, value);
    value = r_u32(dimensions);
    w_u32(packet + 12, value);
    value = r_u32(dimensions + 4);
    w_u32(packet + 16, value);
    value = r_u32(ordering);
    w_u32(packet, value | 0x4000000);
    w_u32(ordering, packet & 0xFFFFFF);
    return packet + 20;
}

uint32 sub_8007f448(void)
{
    uint32 data, flags, active, index, descriptor, root, table, object, offset, scene, argument;
    sint32 selector;
    FUNCTION_MARKER(0x8007F448, "ACE3.BIN");
    pad_reset_slots();
    data = r_u32(0x800BCE60);
    flags = r_u32(data);
    active = r_u8(data + 933);
    w_u32(data, flags | 0x10000000);
    if (active != 0)
    {
        selector = r_s16(data + 908);
        index = sub_80056964(selector);
        descriptor = r_u32(0x800B2334 + (index << 2));
        selector = r_s16(descriptor);
        if (resource_is_cd_audio(selector) == 0)
        {
            data = r_u32(0x800BCE60);
            selector = r_s16(data + 908);
            selector = (sint16)(selector % 6);
            descriptor = 0x800B4334 + ((uint32)selector + 1) * 12;
        }
    }
    else
        descriptor = 0x800B4334;
    resource_queue_audio(descriptor, 0, 0);
    sub_8006c530();
    sub_80079680();
    offset = 0x1850;
    for (index = 0; index < 64; ++index)
    {
        root = r_u32(0x800BD0B8);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (index << 2));
        copy_words(root + offset, object, 8);
        offset += 32;
    }
    sub_8007f298();
    sub_8007ec54(0);
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    argument = r_u32(scene + 8);
    w_u32(scene, flags & 0xEFFFFFFF);
    return sub_80032f18(256, argument);
}

uint32 sub_8007f808(void)
{
    uint32 root, data, flags, index, offset, table, object, scene, active;
    FUNCTION_MARKER(0x8007F808, "ACE3.BIN");
    resource_cancel_streams();
    sub_8007f298();
    sub_8007ec54(1);
    root = r_u32(0x800BD0B8);
    data = r_u32(0x800BCE60);
    w_u32(root, 0);
    w_u32(root + 0x1018, 0);
    flags = r_u32(data);
    w_u32(data, flags & 0xEFFFFFFF);
    offset = 0x1850;
    for (index = 0; index < 64; ++index)
    {
        root = r_u32(0x800BD0B8);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (index << 2));
        copy_words(object, root + offset, 8);
        offset += 32;
    }
    sub_8006c530();
    data = r_u32(0x800BCE60);
    active = r_u8(data + 933);
    if (active == 0)
        return sub_80037e50();
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags | 0x10000000);
    return scene;
}

uint32 sub_8007f298(void)
{
    MATRIX matrix;
    uint32 root, cursor, a, b, c, d, e, x, y, z, index, table, object;
    FUNCTION_MARKER(0x8007F298, "ACE3.BIN");
    root = r_u32(0x800BD0B8);
    cursor = r_u32(root + 16);
    w_u32(root + 0x1018, 0);
    w_u32(root + 0x1840, 0xFFFFFFFF);
    w_u32(root + 0x1844, 0xFFFFFFFF);
    w_u32(root + 0x1848, 0);
    w_u32(root + 0x101C, cursor);
    root = r_u32(0x800BD0B8);
    rotation_build(root + 0x1038, 0, &matrix, 0);
    a = xport_load_le32((uint8 *)&matrix);
    b = xport_load_le32((uint8 *)&matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = xport_load_le32((uint8 *)&matrix + 8);
    d = xport_load_le32((uint8 *)&matrix + 12);
    e = xport_load_le16((uint8 *)&matrix + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x4800);
    xport_gte_execute(0x486012);
    root = r_u32(0x800BD0B8);
    x = xport_gte_read_data(25);
    w_u32(root + 0x1820, x);
    y = xport_gte_read_data(26);
    w_u32(root + 0x1824, y);
    z = xport_gte_read_data(27);
    w_u32(root + 0x1828, z);
    root = r_u32(0x800BD0B8);
    x = r_u32(root + 0x1820);
    z = r_u32(root + 0x1828);
    w_u32(root + 0x1830, 0);
    w_u32(root + 0x1834, 0);
    w_u32(root + 0x1838, 0);
    w_u32(root + 0x1820, x << 3);
    y = r_u32(root + 0x1824);
    w_u32(root + 0x1828, z << 3);
    w_u32(root + 0x1824, y << 3);
    sub_800849ec();
    sub_8007da1c();
    sub_8006c530();
    for (index = 0; index < 64; ++index)
    {
        root = r_u32(0x800BD0B8);
        copy_words(root + 0x1020 + (index << 5), root + 24 + (index << 5), 8);
    }
    for (index = 0; index < 64; ++index)
    {
        root = r_u32(0x800BD0B8);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (index << 2));
        copy_words(object, root + 24 + (index << 5), 8);
    }
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BD0B8);
    object = r_u32(table);
    sub_800796b8(object + 8, root + 0x1820);
    return sub_80079730(0, 0, 0);
}

uint32 sub_8007f5b4(void)
{
    uint32 root, count, total, previous, cursor, end, complete, selected, timer, table, object, snapshot;
    uint32 x, y, z, a, b, c, index, target, mode, kind;
    FUNCTION_MARKER(0x8007F5B4, "ACE3.BIN");
    root = r_u32(0x800BD0B8);
    count = r_u32(root + 0x1018);
    total = r_u32(root);
    if ((sint32)count >= (sint32)total)
        return 1;
    previous = r_u32(root + 0x1840);
    cursor = r_u32(root + 0x101C);
    w_u32(root + 0x1018, count + 1);
    w_u32(root + 20, 0);
    do
    {
        root = r_u32(0x800BD0B8);
        end = r_u32(root + 8);
        if ((sint32)cursor >= (sint32)end)
            cursor = r_u32(root + 4);
        cursor = sub_8007f1d4(cursor);
        root = r_u32(0x800BD0B8);
        complete = r_u32(root + 20);
    } while (complete == 0);
    selected = r_u32(root + 0x1840);
    w_u32(root + 0x101C, cursor);
    if (previous != selected)
    {
        w_u32(root + 0x1848, 150);
        if (previous == 0xFFFFFFFF)
        {
            selected = r_u32(root + 0x1840);
            w_u32(root + 0x1844, selected);
        }
    }
    root = r_u32(0x800BD0B8);
    timer = r_u32(root + 0x1848);
    if (timer != 0)
        w_u32(root + 0x1848, timer - 1);
    else
    {
        selected = r_u32(root + 0x1840);
        w_u32(root + 0x1844, selected);
    }
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    root = r_u32(0x800BD0B8);
    x = r_u32(object + 8);
    y = r_u32(object + 12);
    z = r_u32(object + 16);
    snapshot = root + 0x1020;
    a = r_u32(snapshot + 8);
    b = r_u32(snapshot + 12);
    a -= x;
    b -= y;
    c = r_u32(snapshot + 16);
    selected = r_u32(root + 0x1844);
    w_u32(root + 0x1820, a);
    w_u32(root + 0x1824, b);
    c -= z;
    w_u32(root + 0x1828, c);
    if ((sint32)selected >= 0)
    {
        object = r_u32(table + (selected << 2));
        x = r_u32(object + 8);
        y = r_u32(object + 12);
        z = r_u32(object + 16);
        snapshot = root + 0x1020 + (selected << 5);
        a = r_u32(snapshot + 8);
        c = r_u32(snapshot + 16);
        a -= x;
        b = r_u32(snapshot + 12);
        c -= z;
        w_u32(root + 0x1830, a);
        w_u32(root + 0x1838, c);
        b -= y;
        w_u32(root + 0x1834, b);
    }
    for (index = 0; index < 64; ++index)
    {
        root = r_u32(0x800BD0B8);
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (index << 2));
        copy_words(object, root + 0x1020 + (index << 5), 8);
    }
    table = r_u32(0x800BCE5C);
    root = r_u32(0x800BD0B8);
    object = r_u32(table);
    sub_800796b8(object + 8, root + 0x1820);
    root = r_u32(0x800BD0B8);
    selected = r_u32(root + 0x1844);
    target = 0;
    mode = 0;
    if ((sint32)selected >= 0)
    {
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (selected << 2));
        target = object + 8;
        if ((sint32)selected < 32)
            mode = 1;
        else
        {
            kind = r_s16(object + 72);
            mode = kind == 35 || kind == 36 || kind == 43 || kind == 61 ? 1 : 2;
        }
    }
    root = r_u32(0x800BD0B8);
    sub_80079730(target, root + 0x1830, mode);
    return 0;
}

uint32 sub_800807a4(uint32 position, uint32 vector, uint32 direction, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 root, x, y, z, record, random, sx, sy, sz, index, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x800807A4, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    if (native_position)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        w_u32(record, 0xEFFFF);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u32(record + 24, 0x1000100);
    }
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record != 0)
    {
        random = game_random();
        w_u16(record + 2, (random & 3) + 9);
        w_u16(record, 0);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u32(record + 16, direction);
        w_u16(record + 20, 1);
        w_u16(record + 22, (random & 63) + 400);
    }
    if (native_vector)
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    else
    {
        sx = r_u32(vector);
        sy = r_u32(vector + 4);
        sz = r_u32(vector + 8);
    }
    sx = (sint32)(0 - sx) >> 11;
    sy = (sint32)(0 - sy) >> 11;
    sz = (sint32)(0 - sz) >> 11;
    for (index = 0; index < 8; ++index)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return index + 24;
        w_u16(record, 0);
        w_u16(record + 2, index + 24);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 9);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 9);
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 9);
        w_u16(record + 16, sx + (((rotated & 255) - 128) << 1));
        w_u16(record + 18, sy + ((uint32)((sint32)rotated >> 2) & 0x1FE) - 304);
        w_u16(record + 20, sz + ((((uint32)((sint32)rotated >> 6) & 255) - 128) << 1));
        w_u32(record + 4, px);
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        if (index < 2)
        {
            w_u16(record + 22, 0x8B);
            w_u16(record + 24, 0x1504);
            value = r_u16(record + 18);
            w_u16(record + 26, (rotated & 31) + 240);
            w_u16(record + 18, value - 128);
        }
        else
        {
            w_u16(record + 22, 0x188);
            w_u16(record + 24, 0);
            w_u16(record + 26, index + 20);
        }
    }
    return 0;
}

uint32 sub_80080f70(uint32 position, uint32 vector, uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 root, x, y, z, record, random, index, sx, sy, sz, rotated, px, py, pz, value;
    FUNCTION_MARKER(0x80080F70, "ACE3.BIN");
    if (native_position)
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    else
    {
        x = r_u32(position);
        y = r_u32(position + 4);
        z = r_u32(position + 8);
    }
    if (mode == 1)
        return sub_80089504(x, y, z);
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x600, 3, 7);
    if (record != 0)
    {
        random = game_random();
        w_u32(record, 0x1CFFFF);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u16(record + 24, 0x210);
        w_u16(record + 26, (random & 63) + 588);
    }
    for (index = 0; index < 4; ++index)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x600, 3, 7);
        if (record == 0)
            break;
        random = game_random();
        w_u16(record, 0 - (random & 1) - (index << 1));
        w_u16(record + 2, ((uint32)((sint32)random >> 1) & 3) + 15);
        w_u32(record + 4, x + (((random & 255) - 128) << 12));
        w_u32(record + 8, y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 12));
        w_u32(record + 12, z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 12));
        w_u16(record + 24, 0x100);
        w_u16(record + 26, ((uint32)((sint32)random >> 7) & 127) + 300 - (index << 5));
        w_u16(record + 22, 0);
    }
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (record != 0)
    {
        random = game_random();
        w_u16(record, 0);
        w_u16(record + 2, (random & 3) + 16);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 20, 0);
        w_u16(record + 22, (random & 255) + 1600);
    }
    if (native_vector)
    {
        sx = native_vector[0];
        sy = native_vector[1];
        sz = native_vector[2];
    }
    else
    {
        sx = r_u32(vector);
        sy = r_u32(vector + 4);
        sz = r_u32(vector + 8);
    }
    sx = (sint32)sx >> 9;
    sy = (sint32)sy >> 9;
    sz = ((uint32)((sint32)sz >> 9)) - 128;
    for (index = 0; index < 16; ++index)
    {
        root = r_u32(0x800BCF90);
        record = sub_8007fdb4(root + 0x1098, 10, 7);
        if (record == 0)
            return index + 28;
        w_u16(record, 0);
        w_u16(record + 2, index + 28);
        random = game_random();
        rotated = ((uint32)((sint32)random >> 8)) | (random << 7);
        px = x + (((random & 255) - 128) << 9);
        py = y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 9) + 0xFFFD8000;
        pz = z + ((((uint32)((sint32)random >> 6) & 255) - 128) << 9);
        w_u32(record + 4, px);
        w_u16(record + 16, sx - 128 + (rotated & 255));
        w_u16(record + 18, sy + ((uint32)((sint32)rotated >> 3) & 255) - 192);
        w_u16(record + 20, sz + ((uint32)((sint32)rotated >> 6) & 255));
        w_u32(record + 8, py);
        w_u32(record + 12, pz);
        if (index < 3)
        {
            w_u16(record + 22, 0x8C);
            w_u16(record + 24, 0x1504);
            value = r_u16(record + 18);
            w_u16(record + 26, (index << 5) + 280);
            w_u16(record + 18, value - 128);
        }
        else
        {
            w_u16(record + 22, 0x188);
            w_u16(record + 24, 0);
            w_u16(record + 26, index + 24);
        }
    }
    return 0;
}

uint32 sub_8006c99c(uint32 index)
{
    uint32 root, record;
    FUNCTION_MARKER(0x8006C99C, "ACE3.BIN");
    root = r_u32(0x800BD080);
    record = root + index * 60;
    w_u32(record + 0x15D4, 0);
    return record;
}

uint32 sub_8008488c(uint32 ordering, uint32 packet, uint32 factor)
{
    uint32 mode, result, carrier, address, lane, value, i;
    FUNCTION_MARKER(0x8008488C, "ACE3.BIN");
    if (factor == 0)
        return packet;
    mode = 1;
    if ((sint32)factor < 0)
    {
        mode = 2;
        factor = 0 - factor;
    }
    w_u32(packet, ((packet + 16) & 0xFFFFFF) | 0x3000000);
    w_u32(packet + 4, 0xE1000218 | (mode << 5));
    w_u32(packet + 8, 0xE20C4318);
    w_u32(packet + 12, 0);
    xport_gte_write_data(6, 0x66008300);
    xport_gte_write_data(8, 0x1000 - factor);
    result = sub_80084318(packet + 16, 0x46D6, 0xFF88FF60, 0xF00140, &carrier);
    w_u32(result + 4, 0xE1000220);
    w_u32(result, 0x3000000);
    w_u32(result + 8, 0xE2000000);
    w_u32(result + 12, 0);
    address = ordering + 2;
    lane = address & 3;
    value = (carrier & (0xFFFFFF >> (lane * 8))) | (r_u32(address & ~3) << ((3 - lane) * 8));
    address = result + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    value = packet << 8;
    address = ordering + 2;
    lane = address & 3;
    for (i = 0; i <= lane; ++i)
        w_u8((address & ~3) + i, value >> ((3 - lane + i) * 8));
    return result + 16;
}

uint32 sub_80086948(uint32 matrix)
{
    uint32 x, y, z, a, b, c, first_x, first_y, first_z, second_x, second_y, second_z, result;
    FUNCTION_MARKER(0x80086948, "ACE3.BIN");
    x = r_s16(matrix);
    y = r_s16(matrix + 6);
    z = r_s16(matrix + 12);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    a = r_s16(matrix + 2);
    b = r_s16(matrix + 8);
    xport_gte_execute(0x49E012);
    c = r_s16(matrix + 14);
    first_x = xport_gte_read_data(9);
    first_y = xport_gte_read_data(10);
    first_z = xport_gte_read_data(11);
    xport_gte_write_data(9, a);
    xport_gte_write_data(10, b);
    xport_gte_write_data(11, c);
    first_x &= 0xFFFF;
    first_z &= 0xFFFF;
    xport_gte_execute(0x49E012);
    first_y <<= 16;
    x = r_s16(matrix + 4);
    y = r_s16(matrix + 10);
    z = r_s16(matrix + 16);
    second_x = xport_gte_read_data(9);
    second_y = xport_gte_read_data(10);
    second_z = xport_gte_read_data(11);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    second_x <<= 16;
    second_z <<= 16;
    xport_gte_execute(0x49E012);
    second_y &= 0xFFFF;
    first_x |= second_x;
    first_z |= second_z;
    result = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    result &= 0xFFFF;
    first_y |= result;
    second_y |= y << 16;
    xport_gte_write_control(0, first_x);
    xport_gte_write_control(1, first_y);
    xport_gte_write_control(2, second_y);
    xport_gte_write_control(3, first_z);
    xport_gte_write_control(4, z);
    return result;
}

uint32 sub_80082a3c(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 root, record, random, x, y, z, base, value, result;
    FUNCTION_MARKER(0x80082A3C, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    record = sub_8007fdb4(root + 0x1098, 10, 7);
    if (record == 0)
        return 0;
    random = game_random();
    w_u32(record, ((random & 3) + 9) << 16);
    w_u16(record + 24, 0x1C04);
    w_u16(record + 26, (random & 63) + 50);
    random = game_random();
    x = (native_vector ? native_vector[0] : r_u32(vector));
    base = native_position ? native_position[0] : r_u32(position);
    y = (native_vector ? native_vector[1] : r_u32(vector + 4));
    z = (native_vector ? native_vector[2] : r_u32(vector + 8));
    value = base + x + (((random & 255) - 128) << 8);
    w_u32(record + 4, value);
    base = native_position ? native_position[1] : r_u32(position + 4);
    value = base + y + ((((uint32)((sint32)random >> 3) & 255) - 128) << 8);
    x = (sint32)x >> 10;
    y = ((uint32)((sint32)y >> 10)) - 144;
    w_u32(record + 8, value);
    base = native_position ? native_position[2] : r_u32(position + 8);
    w_u16(record + 20, (sint32)z >> 10);
    w_u16(record + 22, 0x8A);
    result = (((uint32)((sint32)random >> 6) & 255) - 128) << 8;
    w_u16(record + 16, x);
    w_u16(record + 18, y);
    w_u32(record + 12, base + z + result);
    return result;
}

uint32 sub_80082b54(uint32 position, uint32 vector, uint32 kind, const uint32 *native_position)
{
    uint32 packet;
    FUNCTION_MARKER(0x80082B54, "ACE3.BIN");
    packet = sub_8007ed9c(7, position, vector, native_position);
    w_u8(packet + 2, kind);
    return sub_80082a3c(position, vector, native_position, 0);
}

uint32 sub_80082bac(uint32 first, uint32 vector, uint32 second, uint32 kind)
{
    uint32 x, y, z, a, b, c, position[3];
    FUNCTION_MARKER(0x80082BAC, "ACE3.BIN");
    x = r_u32(first);
    a = r_u32(second);
    y = r_u32(first + 4);
    z = r_u32(first + 8);
    position[0] = x + (uint32)((sint32)(a - x) >> 2);
    b = r_u32(second + 4);
    position[1] = y + (uint32)((sint32)(b - y) >> 2);
    c = r_u32(second + 8);
    position[2] = z + (uint32)((sint32)(c - z) >> 2);
    return sub_80082b54(0, vector, kind, position);
}

uint32 sub_8007fca0(uint32 kind, uint32 packet)
{
    uint32 a, b, c, position[3], vector[3], packed, shift, scale, flag;
    FUNCTION_MARKER(0x8007FCA0, "ACE3.BIN");
    a = r_u32(packet + 4);
    b = r_u32(packet + 8);
    c = r_u32(packet + 12);
    position[0] = a << 12;
    position[1] = b << 12;
    position[2] = c << 12;
    a = r_u32(packet + 4);
    c = r_u32(packet + 12);
    b = r_u32(packet + 8);
    vector[0] = ((uint32)((sint32)a >> 20)) << 12;
    vector[2] = ((uint32)((sint32)c >> 20)) << 12;
    vector[1] = ((uint32)((sint32)b >> 20)) << 12;
    if (kind == 0)
    {
        packed = r_u32(packet + 16);
        scale = (packed & 0xFFF) | (((packed >> 12) & 0xFFF) << 16);
        return sub_800807a4(0, 0, scale, position, vector);
    }
    if (kind != 1)
        return 1;
    packed = r_u32(packet + 16);
    flag = r_u8(packet + 2);
    shift = packed & 0xFFF;
    scale = (packed >> 12) & 0xFFF;
    if (shift != 0)
    {
        vector[0] <<= shift & 31;
        vector[1] <<= shift & 31;
        vector[2] <<= shift & 31;
    }
    vector[0] += position[0];
    vector[2] += position[2];
    vector[1] += position[1];
    return sub_80083054(0, 0, scale, flag, position, vector);
}

uint32 sub_80042bf4(void)
{
    uint32 state, data, index, result;
    FUNCTION_MARKER(0x80042BF4, "ACE3.BIN");
    state = r_u32(0x800BCFE0);
    data = r_u32(0x800BCE60);
    w_u32(state + 16, 0);
    w_u32(state + 12, 0);
    w_u32(state + 8, 0);
    index = r_u32(data + 4);
    result = r_u32(state + (index << 2));
    w_u32(0x800BCFE4, result);
    return result;
}

uint32 sub_80042c2c(void)
{
    uint32 state, scene, count, ordering, packet, index, tag, head, result;
    FUNCTION_MARKER(0x80042C2C, "ACE3.BIN");
    state = r_u32(0x800BCFE0);
    scene = r_u32(0x800BCE54);
    count = r_u32(state + 8);
    ordering = r_u32(scene + 16);
    if ((sint32)count > 0)
    {
        packet = r_u32(0x800BCFE4);
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 8);
            packet += 24;
        } while ((sint32)index < (sint32)count);
    }
    state = r_u32(0x800BCFE0);
    count = r_u32(state + 12);
    if ((sint32)count > 0)
    {
        packet = r_u32(0x800BCFE4) + 0xC00;
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 12);
            packet += 16;
        } while ((sint32)index < (sint32)count);
        state = r_u32(0x800BCFE0);
    }
    result = r_u32(state + 16);
    if ((sint32)result > 0)
    {
        packet = r_u32(0x800BCFE4) + 0x1400;
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 16);
            result = (sint32)index < (sint32)count;
            packet += 20;
        } while (result != 0);
    }
    return result;
}

void sub_80043bf0(void)
{
    FUNCTION_MARKER(0x80043BF0, "ACE3.BIN");
}

uint32 sub_80042b20(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 index, descriptor, flags, kind;
    FUNCTION_MARKER(0x80042B20, "ACE3.BIN");
    index = r_u32(0x800BCEA8);
    descriptor = 0x800B1FC4 + (index << 2);
    sub_80042bf4();
    flags = sub_8006cb30() & 0x240;
    if (flags == 0)
    {
        kind = r_u8(descriptor);
        if (kind < 2)
            sub_80042d98(state, native_state);
        else if (((kind - 2) & 255) < 2)
            sub_8004357c(state, native_state);
        else if (((kind - 5) & 255) < 2)
            sub_80043bf0();
        else if (kind == 4)
            sub_80043bf8();
    }
    return sub_80042c2c();
}

uint32 sub_80043bf8(void)
{
    uint32 table, object, raw_pitch, value, state, count, base, packet, line_y;
    sint32 roll, pitch;
    FUNCTION_MARKER(0x80043BF8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    roll = r_s16(object + 28);
    if (roll >= 456)
    {
        sub_80043efc(2, 80, 120);
        sub_80043efc(3, 240, 120);
    }
    if (roll < -455)
    {
        sub_80043efc(3, 80, 120);
        sub_80043efc(2, 240, 120);
    }
    value = r_u16(object + 24);
    raw_pitch = 0x71C - value;
    pitch = (sint16)raw_pitch;
    if (pitch >= 228)
        sub_80043efc(2, 160, 60);
    if (pitch < -227)
        sub_80043efc(3, 160, 180);
    value = r_u16(0x800104AC + (((0 - (uint32)pitch) & 0x3FFF) << 1));
    state = r_u32(0x800BCFE0);
    line_y = (sint32)(value << 16) >> 24;
    count = r_u32(state + 12);
    base = r_u32(0x800BCFE4);
    packet = base + 0xC00 + (count << 4);
    if (((raw_pitch + 227) & 0xFFFF) >= 455)
    {
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 0);
    }
    else
    {
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 128);
    }
    w_u8(packet + 6, 0);
    line_y += 120;
    state = r_u32(0x800BCFE0);
    w_u16(packet + 8, 140);
    w_u16(packet + 10, line_y);
    w_u16(packet + 12, 180);
    w_u16(packet + 14, line_y);
    packet += 16;
    count = r_u32(state + 12);
    w_u32(state + 12, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 64);
    w_u8(packet + 6, 0);
    state = r_u32(0x800BCFE0);
    w_u16(packet + 8, 140);
    w_u16(packet + 10, 120);
    w_u16(packet + 12, 180);
    w_u16(packet + 14, 120);
    count = r_u32(state + 12);
    packet += 16;
    w_u32(state + 12, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 32);
    w_u8(packet + 6, 0);
    state = r_u32(0x800BCFE0);
    w_u16(packet + 8, 140);
    w_u16(packet + 10, 115);
    w_u16(packet + 12, 180);
    w_u16(packet + 14, 115);
    count = r_u32(state + 12);
    packet += 16;
    w_u32(state + 12, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 32);
    w_u8(packet + 6, 0);
    w_u16(packet + 8, 140);
    w_u16(packet + 10, 125);
    w_u16(packet + 12, 180);
    w_u16(packet + 14, 125);
    state = r_u32(0x800BCFE0);
    count = r_u32(state + 12) + 1;
    w_u32(state + 12, count);
    return count;
}

uint32 sub_80043efc(uint32 kind, uint32 x, uint32 y)
{
    uint32 state, count, base, packet, source, value, line_count;
    FUNCTION_MARKER(0x80043EFC, "ACE3.BIN");
    state = r_u32(0x800BCFE0);
    count = r_u32(state + 16);
    base = r_u32(0x800BCFE4);
    packet = base + 0x1400 + count * 20;
    source = 0x800B14F8 + (kind << 3);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    value = r_u8(source);
    w_u16(packet + 12, x + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 1);
    w_u16(packet + 14, y + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 2);
    w_u16(packet + 16, x + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 3);
    w_u16(packet + 18, y + (uint32)(sint32)(sint8)value);
    count = r_u32(state + 16);
    line_count = r_u32(state + 12);
    packet = base + 0xC00 + (line_count << 4);
    w_u32(state + 16, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    value = r_u8(source + 4);
    w_u16(packet + 8, x + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 5);
    w_u16(packet + 10, y + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 6);
    w_u16(packet + 12, x + (uint32)(sint32)(sint8)value);
    value = r_u8(source + 7);
    state = r_u32(0x800BCFE0);
    w_u16(packet + 14, y + (uint32)(sint32)(sint8)value);
    count = r_u32(state + 12) + 1;
    w_u32(state + 12, count);
    return count;
}

uint32 sub_8003a68c(void)
{
    uint32 red, green, blue, color, data, scene, value, first, second, index, active, flags, scores, row, title, y;
    uint32 ordering, packet, result, x, quotient;
    uint8 digit[2];
    FUNCTION_MARKER(0x8003A68C, "ACE3.BIN");
    blue = r_u8(0x800C6D6D);
    green = r_u8(0x800C6D6C);
    red = r_u8(0x800C6D6B);
    color = ((blue >> 1) << 16) | ((green >> 1) << 8) | (red >> 1);
    data = r_u32(0x800BCE60);
    value = r_s16(data + 908);
    if (value == 30)
    {
        scene = r_u32(0x800BCE54);
        first = r_u32(scene + 8);
        if (first == 0)
        {
            second = r_u32(scene + 12);
            if (second == 0)
            {
                for (index = 0; index < 16; ++index)
                {
                    data = r_u32(0x800BCE60);
                    value = r_u8(data + index + 2376);
                    if (value - 97 < 26)
                        value -= 32;
                    w_u8(0x800C7150 + index, value);
                }
            }
        }
        data = r_u32(0x800BCE60);
        value = r_s16(data + 908);
        if (value == 30)
        {
            data = r_u32(0x800BCEB4);
            active = r_u8(data + 61);
            flags = 2;
            if (active != 0)
            {
                scene = r_u32(0x800BCE54);
                flags = r_u32(scene + 4) & 2;
            }
            if (active == 0 || flags != 0)
            {
                scene = r_u32(0x800BCE54);
                for (row = 0; row < 2; ++row)
                {
                    title = row == 0 ? 0x800C7150 : 0x800BCC1C;
                    y = row == 0 ? 80 : 88;
                    ordering = r_u32(scene + 16);
                    packet = r_u32(scene + 20);
                    result = sub_8003563c(ordering, packet, 222, y, color, title, 0);
                    scene = r_u32(0x800BCE54);
                    scores = r_u32(0x800BCEB4);
                    w_u32(scene + 20, result);
                    value = r_u8(scores + (row == 0 ? 46 : 45));
                    digit[0] = value / 10 + 48;
                    digit[1] = 0;
                    ordering = r_u32(scene + 16);
                    packet = r_u32(scene + 20);
                    result = sub_8003563c(ordering, packet, 296, y, color, 0, digit);
                    scene = r_u32(0x800BCE54);
                    scores = r_u32(0x800BCEB4);
                    w_u32(scene + 20, result);
                    value = r_u8(scores + (row == 0 ? 46 : 45));
                    digit[0] = value % 10 + 48;
                    ordering = r_u32(scene + 16);
                    packet = r_u32(scene + 20);
                    result = sub_8003563c(ordering, packet, 304, y, color, 0, digit);
                    scene = r_u32(0x800BCE54);
                    w_u32(scene + 20, result);
                }
            }
        }
    }
    data = r_u32(0x800BCE60);
    value = r_s16(data + 908);
    result = 3;
    if (value != 3)
        return result;
    data = r_u32(0x800BCEB4);
    active = r_u8(data + 61);
    if (active != 0 && active != 3)
    {
        scene = r_u32(0x800BCE54);
        result = r_u32(scene + 4) & 2;
        if (result == 0)
            return result;
    }
    scene = r_u32(0x800BCE54);
    digit[1] = 0;
    ordering = r_u32(scene + 16);
    packet = r_u32(scene + 20);
    result = sub_8003563c(ordering, packet, 272, 80, color, 0x800BCC24, 0);
    scene = r_u32(0x800BCE54);
    scores = r_u32(0x800BCEB4);
    w_u32(scene + 20, result);
    value = r_u32(scores + 4);
    x = 304;
    while (value != 0)
    {
        quotient = (sint32)value / 10;
        scene = r_u32(0x800BCE54);
        digit[0] = value - quotient * 10 + 48;
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8003563c(ordering, packet, x, 88, color, 0, digit);
        x -= 8;
        scene = r_u32(0x800BCE54);
        value = quotient;
        w_u32(scene + 20, result);
    }
    return result;
}

uint32 sub_8003d5c0(void)
{
    uint32 state, radius, center_x, center_y, left, right, top, bottom, cosine, sine, dx0, dx1, dy0, dy1;
    uint32 px0, px1, py0, py1, qx0, qx1, qy0, qy1, numerators[8], projected[8], index, packet, value, live;
    FUNCTION_MARKER(0x8003D5C0, "ACE3.BIN");
    state = r_u32(0x800BCFB0);
    radius = r_u32(0x800C6D7C);
    center_x = r_u32(state);
    center_y = r_u32(state + 4);
    left = (center_x - radius) & 0xFFFFF000;
    right = ((center_x + radius) & 0xFFFFF000) + 0x1000;
    top = ((center_y + radius) & 0xFFFFF000) + 0x1000;
    bottom = (center_y - radius) & 0xFFFFF000;
    if ((sint32)left > 0x3FFFF)
        left = 0x3FFFF;
    if ((sint32)left < -262144)
        left = 0xFFFC0000;
    if ((sint32)right > 0x3FFFF)
        right = 0x3FFFF;
    if ((sint32)right < -262144)
        right = 0xFFFC0000;
    if ((sint32)top > 0x40000)
        top = 0x40000;
    if ((sint32)top <= -262144)
        top = 0xFFFC0001;
    if ((sint32)bottom > 0x40000)
        bottom = 0x40000;
    if ((sint32)bottom <= -262144)
        bottom = 0xFFFC0001;
    cosine = r_u32(state + 12);
    dx0 = left - center_x;
    px0 = dx0 * cosine;
    sine = r_u32(state + 8);
    dy0 = center_y - top;
    py0 = dy0 * sine;
    qx0 = (0 - dx0) * sine;
    qy0 = dy0 * cosine;
    dx1 = right - center_x;
    px1 = dx1 * cosine;
    qx1 = (0 - dx1) * sine;
    dy1 = center_y - bottom;
    py1 = dy1 * sine;
    qy1 = dy1 * cosine;
    numerators[0] = px0 + py0;
    numerators[1] = qx0 + qy0;
    numerators[2] = px1 + py0;
    numerators[3] = qx1 + qy0;
    numerators[4] = px0 + py1;
    numerators[5] = qx0 + qy1;
    numerators[6] = px1 + py1;
    numerators[7] = qx1 + qy1;
    for (index = 0; index < 8; ++index)
    {
        value = ((uint32)((sint32)numerators[index] >> 12)) << 5;
        if (radius == 0)
            xport_mips_break(7);
        if (radius == 0xFFFFFFFF && value == 0x80000000)
            xport_mips_break(6);
        projected[index] = (sint32)value / (sint32)radius;
    }
    packet = r_u32(0x800BCFAC) + 0xAA4;
    w_u16(packet + 8, projected[0] + 44);
    w_u16(packet + 10, projected[1] + 46);
    w_u16(packet + 16, projected[2] + 44);
    w_u16(packet + 18, projected[3] + 46);
    w_u16(packet + 24, projected[4] + 44);
    w_u16(packet + 26, projected[5] + 46);
    w_u16(packet + 32, projected[6] + 44);
    w_u16(packet + 34, projected[7] + 46);
    value = r_u32(state);
    value = ((uint32)((sint32)(dx0 + value) >> 12)) + 64;
    w_u8(packet + 28, value);
    w_u8(packet + 12, value);
    live = r_u32(0x800BCFB0);
    value = r_u32(live);
    value = ((uint32)((sint32)(dx1 + value) >> 12)) + 64;
    w_u8(packet + 36, value);
    w_u8(packet + 20, value);
    live = r_u32(0x800BCFB0);
    value = r_u32(live + 4);
    value = ((uint32)((sint32)(dy0 - value) >> 12)) + 64;
    w_u8(packet + 21, value);
    w_u8(packet + 13, value);
    live = r_u32(0x800BCFB0);
    value = r_u32(live + 4);
    value = ((uint32)((sint32)(dy1 - value) >> 12)) + 64;
    w_u8(packet + 37, value);
    w_u8(packet + 29, value);
    return value;
}

uint32 sub_8003c5b0(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 data, result;
    FUNCTION_MARKER(0x8003C5B0, "ACE3.BIN");
    sub_8003c650();
    sub_8003d08c(state, native_state);
    sub_8003d138();
    sub_8003d280();
    sub_8003d458();
    sub_8003d5c0();
    sub_8003d9c8();
    sub_8003de5c();
    sub_8003c690();
    data = r_u32(0x800BCE60);
    result = r_u8(data + 2394);
    if (result != 0)
        return result;
    result = r_u32(data + 296) & 0x8000;
    if (result == 0)
        return result;
    sub_8003e69c();
    return sub_8003cb90();
}

uint32 sub_8003efe0(void)
{
    FUNCTION_MARKER(0x8003EFE0, "ACE3.BIN");
    sub_8003f018();
    sub_8003f520();
    sub_80040e70();
    return sub_8003f054();
}

uint32 sub_80063e4c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 object, flags, saved_height, scene, selected, table, mode;
    FUNCTION_MARKER(0x80063E4C, "ACE3.BIN");
    sub_80063f80();
    object = (native_state ? native_state->words[13] : r_u32(state + 52));
    flags = r_u32(object + 80) & 0x401;
    if (flags != 0)
    {
        sub_80064400(state, native_state);
        sub_80064cb4();
        saved_height = sub_800a70e8();
        sub_8006447c(state, native_state);
        sub_80064d98(state, native_state);
        geom_set_screen(120);
        sub_80065598(state, native_state);
        geom_set_screen(saved_height);
        sub_800660f8(state, native_state);
    }
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 4) & 4;
    if (flags != 0)
    {
        selected = r_u8(0x800C6D6E);
        sub_80065d8c();
        if (selected != 255)
        {
            table = r_u32(0x800BCE5C);
            object = r_u32(table + (selected << 2));
            flags = r_u32(object);
            if ((flags & 0x40) == 0)
                sub_800662b4();
        }
        mode = r_u8(0x800C6D70);
        if (mode == 1)
            sub_80066590(state, native_state);
    }
    return sub_80063fcc();
}

uint32 sub_8003a0b4(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    uint32 scene, result;
    FUNCTION_MARKER(0x8003A0B4, "ACE3.BIN");
    sub_8003a12c(state, native_state);
    sub_8003a68c();
    sub_800413fc(state, native_state);
    sub_8003c5b0(state, native_state);
    sub_8003efe0();
    sub_80063e4c(state, native_state);
    sub_8005e31c(state, native_state);
    scene = r_u32(0x800BCE54);
    result = r_u32(scene + 4) & 0x800;
    if (result != 0)
        return sub_80042b20(state, native_state);
    return result;
}

uint32 sub_8003c690(void)
{
    uint32 state, scene, flags, ordering, base, packet, tag, head, count, index;
    FUNCTION_MARKER(0x8003C690, "ACE3.BIN");
    state = r_u32(0x800BCFA8);
    scene = r_u32(0x800BCE54);
    flags = r_u32(state + 24);
    ordering = r_u32(scene + 16);
    if (flags & 2)
    {
        base = r_u32(0x800BCFAC);
        packet = base + 0xC74;
        for (index = 0; index < 3; ++index, packet += 20)
        {
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    state = r_u32(0x800BCFA8);
    count = r_u32(state + 16);
    if ((sint32)count > 0)
    {
        base = r_u32(0x800BCFAC);
        packet = base + 0x918;
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 16);
            packet += 16;
        } while ((sint32)index < (sint32)count);
    }
    base = r_u32(0x800BCFAC);
    head = r_u32(ordering);
    tag = r_u32(base + 0xC0C);
    w_u32(base + 0xC0C, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xC0C) & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 0xC20);
    w_u32(base + 0xC20, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xC20) & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 12);
    w_u32(base + 12, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 12) & 0xFFFFFF);
    w_u32(ordering, head);
    w_u32(base + 0xA88, head | 0x2000000);
    w_u32(ordering, (base + 0xA88) & 0xFFFFFF);
    state = r_u32(0x800BCFA8);
    flags = r_u32(state + 24);
    if (flags & 1)
    {
        base = r_u32(0x800BCFAC);
        packet = base + 0xC54;
        for (index = 0; index < 2; ++index, packet += 16)
        {
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
        }
    }
    state = r_u32(0x800BCFA8);
    base = r_u32(0x800BCFAC);
    count = r_u32(state + 20);
    packet = base + 0x998;
    if ((sint32)count > 0)
    {
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 20);
            packet += 24;
        } while ((sint32)index < (sint32)count);
    }
    state = r_u32(0x800BCFA8);
    count = r_u32(state + 8);
    if ((sint32)count > 0)
    {
        base = r_u32(0x800BCFAC);
        packet = base + 24;
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 8);
            packet += 20;
        } while ((sint32)index < (sint32)count);
    }
    head = r_u32(ordering);
    base = r_u32(0x800BCFAC);
    w_u32(base + 0xA7C, head | 0x2000000);
    head = (base + 0xA7C) & 0xFFFFFF;
    w_u32(ordering, head);
    base = r_u32(0x800BCFAC);
    tag = r_u32(base + 0xBE4);
    w_u32(base + 0xBE4, (tag & 0xFF000000) | head);
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xB6C) & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 0xC44);
    w_u32(base + 0xC44, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xC44) & 0xFFFFFF);
    w_u32(ordering, head);
    w_u32(base + 0xA70, head | 0x2000000);
    head = (base + 0xA70) & 0xFFFFFF;
    w_u32(ordering, head);
    base = r_u32(0x800BCFAC);
    tag = r_u32(base + 0xA94);
    w_u32(base + 0xA94, (tag & 0xFF000000) | head);
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xA94) & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 0xAA4);
    w_u32(base + 0xAA4, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xAA4) & 0xFFFFFF);
    w_u32(ordering, head);
    w_u32(base + 0xA64, head | 0x2000000);
    head = (base + 0xA64) & 0xFFFFFF;
    w_u32(ordering, head);
    base = r_u32(0x800BCFAC);
    tag = r_u32(base + 0xB44);
    w_u32(base + 0xB44, (tag & 0xFF000000) | head);
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xACC) & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 0xC34);
    w_u32(base + 0xC34, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xC34) & 0xFFFFFF);
    w_u32(ordering, head);
    w_u32(base + 0xA58, head | 0x2000000);
    head = (base + 0xA58) & 0xFFFFFF;
    w_u32(ordering, head);
    base = r_u32(0x800BCFAC);
    tag = r_u32(base);
    w_u32(base, (tag & 0xFF000000) | head);
    head = r_u32(ordering);
    head = (head & 0xFF000000) | (base & 0xFFFFFF);
    w_u32(ordering, head);
    tag = r_u32(base + 0xA9C);
    w_u32(base + 0xA9C, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    head = (head & 0xFF000000) | ((base + 0xA9C) & 0xFFFFFF);
    w_u32(ordering, head);
    return head;
}

uint32 sub_8003e69c(void)
{
    uint32 table, object, x, y, offset_x, offset_y, base, packet, index, ax, ay, bx, by, a, b, c, value, angle, next_angle, last_angle;
    uint32 center_x, center_y, state, count, end, cursor, flags, object_flags, opcode, result;
    uint16 origin[2], point[2];
    FUNCTION_MARKER(0x8003E69C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    x = r_u32(object + 8);
    y = r_u32(object + 16);
    x = ((uint32)((sint32)x >> 23)) + 128;
    y = ((uint32)((sint32)(0 - y) >> 23)) + 128;
    if ((sint32)x >= 256)
        x = 255;
    if ((sint32)x < 0)
        x = 0;
    if ((sint32)y >= 256)
        y = 255;
    if ((sint32)y < 0)
        y = 0;
    origin[0] = x < 64 ? 0 : x >= 193 ? 128 : x - 64;
    origin[1] = y < 64 ? 0 : y >= 193 ? 128 : y - 64;
    offset_x = origin[0] >> 1;
    offset_y = origin[1] >> 1;
    base = r_u32(0x800BCFAC);
    packet = base + 0xCF9;
    for (index = 0; index < 7; ++index, packet += 40)
    {
        ax = 0x800BCC70 + index;
        ay = 0x800BCC78 + index;
        bx = 0x800BCC80 + index;
        by = 0x800BCC88 + index;
        value = r_u8(ax);
        w_u8(packet - 25, value + offset_x);
        value = r_u8(ay);
        w_u8(packet - 24, value + offset_y);
        value = r_u8(ax);
        a = r_u8(bx);
        w_u8(packet - 17, a + value + offset_x);
        value = r_u8(ay);
        w_u8(packet - 16, value + offset_y);
        value = r_u8(ax);
        w_u8(packet - 9, value + offset_x);
        value = r_u8(ay);
        a = r_u8(by);
        w_u8(packet - 8, a + value + offset_y);
        value = r_u8(ax);
        a = r_u8(bx);
        w_u8(packet - 1, a + value + offset_x);
        value = r_u8(ay);
        a = r_u8(by);
        w_u8(packet, a + value + offset_y);
    }
    packet = r_u32(0x800BCFAC) + 0xFFC;
    sub_8003ec0c(object + 8, 0, 0, point, origin);
    center_x = point[0] + 96;
    center_y = point[1] + 56;
    w_u16(packet + 8, point[0] + 92);
    w_u16(packet + 10, point[1] + 52);
    value = r_u16(object + 26);
    w_u8(packet + 13, 59);
    w_u8(packet + 12, (((0x4200 - (value & 0x3FFF)) >> 7) & 0x78) - 128);
    value = r_s16(object + 26);
    base = r_u32(0x800BCFAC);
    angle = (0x1AAC - value) & 0x3FFF;
    packet = base + 0xFE2;
    for (index = 0; index < 2; ++index, packet += 24)
    {
        next_angle = (angle + 0x2AA) & 0x3FFF;
        last_angle = (next_angle + 0x2AA) & 0x3FFF;
        a = r_u16(0x800104AC + (angle << 1));
        b = r_u16(0x800104AC + ((angle + 0x1000) << 1));
        c = r_u16(0x800104AC + ((next_angle + 0x1000) << 1));
        w_u16(packet - 10, center_x);
        w_u16(packet - 8, center_y);
        w_u16(packet - 12, center_y + (uint32)((sint16)b >> 10));
        w_u16(packet - 14, center_x + (uint32)((sint16)a >> 10));
        a = r_u16(0x800104AC + (next_angle << 1));
        w_u16(packet - 4, center_y + (uint32)((sint16)c >> 10));
        w_u16(packet - 6, center_x + (uint32)((sint16)a >> 10));
        a = r_u16(0x800104AC + (last_angle << 1));
        b = r_u16(0x800104AC + ((last_angle + 0x1000) << 1));
        w_u16(packet - 2, center_x + (uint32)((sint16)a >> 10));
        w_u16(packet, center_y + (uint32)((sint16)b >> 10));
        angle = last_angle;
    }
    state = r_u32(0x800BCFA8);
    count = r_u32(state + 12);
    base = r_u32(0x800BCFAC);
    end = base + 0x518 + (count << 4);
    cursor = end + 10;
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + (index << 2));
        object = r_u32(table + (index << 2));
        if ((flags & 0x100) == 0)
            continue;
        object_flags = r_u32(object);
        if ((object_flags & 0x4000) == 0 || (flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        result = sub_8003ec0c(object + 8, 0, 0, point, origin);
        if ((sint32)result < 0)
            continue;
        end += 16;
        x = point[0];
        opcode = r_u8(cursor - 3);
        w_u16(cursor - 2, x + 96);
        y = point[1];
        w_u8(cursor - 6, 255);
        w_u8(cursor - 5, 64);
        w_u8(cursor - 4, 64);
        w_u16(cursor + 2, 2);
        w_u16(cursor + 4, 2);
        value = r_u16(cursor - 2);
        w_u8(cursor - 3, opcode & 0xFD);
        state = r_u32(0x800BCFA8);
        y += 56;
        w_u16(cursor, y);
        w_u16(cursor - 2, value - 1);
        w_u16(cursor, y - 1);
        count = r_u32(state + 12);
        cursor += 16;
        w_u32(state + 12, count + 1);
    }
    cursor = end + 7;
    for (index = 2; index < 64; ++index)
    {
        table = r_u32(0x800BCE5C);
        flags = r_u32(0x800C6FD0 + (index << 2));
        object = r_u32(table + (index << 2));
        if (flags & 0x100)
            continue;
        object_flags = r_u32(object);
        if ((object_flags & 0x4000) == 0 || (flags & 1) == 0 || (flags & 0x2000) == 0)
            continue;
        result = sub_8003ec0c(object + 8, 0, 0, point, origin);
        if ((sint32)result < 0)
            continue;
        w_u16(cursor + 1, point[0] + 96);
        w_u16(cursor + 3, point[1] + 56);
        object_flags = r_u32(object);
        if (object_flags & 0x40)
        {
            w_u8(cursor - 3, 0);
            w_u8(cursor - 2, 0);
            w_u8(cursor - 1, 255);
        }
        else if (object_flags & 0x100)
        {
            w_u8(cursor - 3, 255);
            w_u8(cursor - 2, 255);
            w_u8(cursor - 1, 0);
        }
        else
        {
            w_u8(cursor - 3, 64);
            w_u8(cursor - 2, 64);
            w_u8(cursor - 1, 64);
        }
        w_u16(cursor + 5, 2);
        w_u16(cursor + 7, 2);
        value = r_u8(cursor);
        w_u8(cursor, value | 2);
        state = r_u32(0x800BCFA8);
        count = r_u32(state + 12);
        cursor += 16;
        w_u32(state + 12, count + 1);
    }
    return 0;
}

uint32 sub_8003cb90(void)
{
    uint32 scene, flags, ordering, base, state, tag, head, packet, count, index, result;
    FUNCTION_MARKER(0x8003CB90, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 12);
    ordering = r_u32(scene + 16);
    if (flags & 8)
    {
        base = r_u32(0x800BCFAC);
        head = r_u32(ordering);
        tag = r_u32(base + 0xFFC);
        w_u32(base + 0xFFC, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000) | ((base + 0xFFC) & 0xFFFFFF));
    }
    base = r_u32(0x800BCFAC);
    head = r_u32(ordering);
    state = r_u32(0x800BCFA8);
    tag = r_u32(base + 0x1078);
    w_u32(base + 0x1078, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    w_u32(ordering, (head & 0xFF000000) | ((base + 0x1078) & 0xFFFFFF));
    count = r_u32(state + 12);
    if ((sint32)count > 0)
    {
        packet = base + 0x518;
        index = 0;
        do
        {
            ++index;
            tag = r_u32(packet);
            head = r_u32(ordering);
            w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
            head = r_u32(ordering);
            w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
            count = r_u32(state + 12);
            packet += 16;
        } while ((sint32)index < (sint32)count);
    }
    head = r_u32(ordering);
    base = r_u32(0x800BCFAC);
    tag = r_u32(base + 0x1070);
    w_u32(base + 0x1070, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    w_u32(ordering, (head & 0xFF000000) | ((base + 0x1070) & 0xFFFFFF));
    packet = base + 0xFCC;
    for (index = 0; index < 2; ++index, packet += 24)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = r_u32(0x800BCFAC);
    packet = base + 0x1010;
    for (index = 0; index < 4; ++index, packet += 20)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, head);
    }
    base = r_u32(0x800BCFAC);
    tag = r_u32(base + 0x1068);
    w_u32(base + 0x1068, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ordering);
    w_u32(ordering, (head & 0xFF000000) | ((base + 0x1068) & 0xFFFFFF));
    packet = base + 0xF2C;
    for (index = 0; index < 10; ++index, packet += 16)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = r_u32(0x800BCFAC);
    packet = base + 0xECC;
    for (index = 0; index < 4; ++index, packet += 24)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = r_u32(0x800BCFAC);
    packet = base + 0xCD4;
    for (index = 0; index < 3; ++index, packet += 40)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, head);
    }
    base = r_u32(0x800BCFAC);
    w_u32(base + 0xCC8, head | 0x2000000);
    w_u32(ordering, (base + 0xCC8) & 0xFFFFFF);
    base = r_u32(0x800BCFAC);
    packet = base + 0xD4C;
    for (index = 3; index < 7; ++index, packet += 40)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, head);
    }
    base = r_u32(0x800BCFAC);
    w_u32(base + 0xCBC, head | 0x2000000);
    w_u32(ordering, (base + 0xCBC) & 0xFFFFFF);
    base = r_u32(0x800BCFAC);
    packet = base + 0xDEC;
    for (index = 0; index < 4; ++index, packet += 40)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        w_u32(ordering, (head & 0xFF000000) | (packet & 0xFFFFFF));
    }
    base = r_u32(0x800BCFAC);
    packet = base + 0xE8C;
    for (index = 0; index < 4; ++index, packet += 16)
    {
        tag = r_u32(packet);
        head = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ordering);
        head = (head & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, head);
    }
    base = r_u32(0x800BCFAC);
    w_u32(base + 0xCB0, head | 0x2000000);
    result = (base + 0xCB0) & 0xFFFFFF;
    w_u32(ordering, result);
    return result;
}

void sub_80040740(uint32 kind, uint32 x, uint32 y)
{
    uint32 source, state, count, base, packet, value;
    FUNCTION_MARKER(0x80040740, "ACE3.BIN");
    kind &= 255;
    if (kind == 0)
        return;
    source = 0x800B10CC + (kind << 2);
    state = r_u32(0x800BCFB8);
    count = r_u32(state + 16);
    base = r_u32(0x800BCFBC);
    value = r_u8(source + 2);
    packet = base + 0x1008 + count * 20;
    w_u16(packet + 10, y);
    w_u16(packet + 8, x - value - 2);
    value = r_u8(source);
    w_u8(packet + 12, value);
    value = r_u8(source + 1);
    w_u8(packet + 13, value);
    value = r_u8(source + 2);
    w_u16(packet + 16, value);
    value = r_u8(source + 3);
    w_u16(packet + 18, value);
    if (kind == 1)
    {
        w_u8(packet + 4, 0x83);
        w_u8(packet + 5, 0);
        w_u8(packet + 6, 0);
    }
    else if (kind == 2)
    {
        w_u8(packet + 4, 0x83);
        w_u8(packet + 5, 0x83);
        w_u8(packet + 6, 0);
    }
    else if (kind == 3)
    {
        w_u8(packet + 5, 0x23);
        w_u8(packet + 4, 0);
        w_u8(packet + 6, 0x83);
    }
    state = r_u32(0x800BCFB8);
    count = r_u32(state + 16);
    w_u32(state + 16, count + 1);
}

uint32 sub_80040530(uint32 index)
{
    uint32 record, table, scene, width, object, camera, angle, sine, cosine, half, offset_x, offset_y, value, i, x, y, a, b;
    uint32 state, count, base, packet;
    uint16 points[6];
    FUNCTION_MARKER(0x80040530, "ACE3.BIN");
    record = r_u32(0x800C67A0 + (index << 2));
    if (record == 0)
        return 1;
    table = r_u32(0x800BCE5C);
    scene = r_u32(0x800BCE54);
    width = r_u16(record + 6);
    object = r_u32(table + (index << 2));
    camera = r_u32(scene + 40);
    a = r_u16(object + 26);
    b = r_u16(camera + 22);
    angle = (a - b) & 0x3FFF;
    sine = r_s16(0x800104AC + (angle << 1));
    cosine = r_s16(0x800104AC + ((angle + 0x1000) << 1));
    half = (sint16)width >> 1;
    offset_x = (uint32)((sint32)((cosine - sine) * half) >> 14);
    points[0] = 0;
    value = r_u16(record + 6);
    points[2] = 4;
    points[1] = value - 4;
    value = r_u16(record + 6);
    points[4] = 0xFFFC;
    points[3] = value + 4;
    value = r_u16(record + 6);
    points[5] = value + 4;
    offset_y = (uint32)((sint32)((sine + cosine) * half) >> 14);
    for (i = 0; i < 3; ++i)
    {
        x = (sint16)points[i * 2];
        y = (sint16)points[i * 2 + 1];
        a = cosine * x - sine * y;
        b = sine * x + cosine * y;
        value = r_u16(record + 8);
        points[i * 2] = value + (uint32)((sint32)a >> 14) + offset_x;
        value = r_u16(record + 10);
        points[i * 2 + 1] = value + (uint32)((sint32)b >> 14) + offset_y;
    }
    state = r_u32(0x800BCFB8);
    count = r_u32(state + 8);
    base = r_u32(0x800BCFBC);
    packet = base + 8 + (count << 4);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    state = r_u32(0x800BCFB8);
    w_u16(packet + 8, points[0]);
    w_u16(packet + 10, points[1]);
    w_u16(packet + 12, points[2]);
    w_u16(packet + 14, points[3]);
    count = r_u32(state + 8);
    packet += 16;
    w_u32(state + 8, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    state = r_u32(0x800BCFB8);
    w_u16(packet + 8, points[0]);
    w_u16(packet + 10, points[1]);
    w_u16(packet + 12, points[4]);
    w_u16(packet + 14, points[5]);
    count = r_u32(state + 8);
    w_u32(state + 8, count + 1);
    return 0;
}

uint32 sub_8003fa80(uint32 index, uint32 color)
{
    uint32 record, object, width, a, b, flags, quarter, state, count, base, packet, i, x, y, value;
    uint16 left, right, top, bottom;
    FUNCTION_MARKER(0x8003FA80, "ACE3.BIN");
    record = r_u32(0x800C67A0 + (index << 2));
    color |= 0x4A000000;
    if (record == 0)
        return 1;
    object = r_u32(0x800C6ED0 + (index << 2));
    if (object == 0)
        return 1;
    width = r_u16(record + 6);
    a = r_s16(record + 8);
    left = a - (uint32)(sint32)(sint16)width;
    a = r_s16(record + 8);
    b = r_s16(record + 6);
    right = a + b;
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    top = a - b;
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    bottom = a + b;
    flags = r_u32(object) & 0x100;
    quarter = (sint16)width >> 2;
    state = r_u32(0x800BCFB8);
    count = r_u32(state + 12);
    base = r_u32(0x800BCFBC);
    packet = base + 0x408 + count * 24;
    if (flags != 0)
    {
        w_u32(packet + 4, color);
        w_u16(packet + 12, left);
        w_u16(packet + 8, left);
        w_u16(packet + 16, right);
        w_u16(packet + 10, bottom);
        w_u16(packet + 18, top);
        w_u16(packet + 14, top);
        count = r_u32(state + 12);
        packet += 24;
        w_u32(state + 12, count + 1);
        w_u32(packet + 4, color);
        w_u16(packet + 12, right);
        w_u16(packet + 8, right);
        w_u16(packet + 16, left);
        w_u16(packet + 10, top);
        w_u16(packet + 18, bottom);
        w_u16(packet + 14, bottom);
        count = r_u32(state + 12) + 1;
        w_u32(state + 12, count);
        return count;
    }
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        w_u32(packet + 4, color);
        w_u16(packet + 16, x);
        w_u16(packet + 12, x);
        value = (i & 1) ? x - quarter : x + quarter;
        w_u16(packet + 8, value);
        w_u16(packet + 18, y);
        w_u16(packet + 14, y);
        w_u16(packet + 10, y);
        value = i < 2 ? y + quarter : y - quarter;
        w_u16(packet + 18, value);
        count = r_u32(state + 12) + 1;
        if (i != 3)
            packet += 24;
        w_u32(state + 12, count);
    }
    return count;
}

uint32 sub_8003fd48(uint32 index, uint32 color)
{
    uint32 record, object, width, a, b, quarter, eighth, stretch, flags, state, count, base, packet, value;
    uint16 left, middle, right, top, upper, lower, bottom;
    FUNCTION_MARKER(0x8003FD48, "ACE3.BIN");
    record = r_u32(0x800C67A0 + (index << 2));
    color |= 0x4A000000;
    if (record == 0)
        return 1;
    object = r_u32(0x800C6ED0 + (index << 2));
    if (object == 0)
        return 1;
    width = r_u16(record + 6);
    a = r_s16(record + 8);
    left = a - (uint32)(sint32)(sint16)width;
    middle = r_s16(record + 8);
    a = r_s16(record + 8);
    b = r_s16(record + 6);
    right = a + b;
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    top = a - b;
    quarter = (sint16)width >> 2;
    a = r_u16(record + 6);
    b = r_s16(record + 10);
    upper = b - (uint32)((sint16)a >> 1);
    a = r_u16(record + 6);
    b = r_s16(record + 10);
    lower = b + (uint32)((sint16)a >> 1);
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    bottom = a + b;
    eighth = (sint16)width >> 3;
    flags = r_u32(object) & 0x100;
    stretch = (sint32)((eighth << 3) + eighth) >> 2;
    state = r_u32(0x800BCFB8);
    count = r_u32(state + 12);
    base = r_u32(0x800BCFBC);
    packet = base + 0x408 + count * 24;
    if (flags != 0)
    {
        w_u32(packet + 4, color);
        w_u16(packet + 8, left);
        w_u16(packet + 12, middle);
        w_u16(packet + 16, right);
        w_u16(packet + 10, upper);
        w_u16(packet + 14, top);
        w_u16(packet + 18, upper);
        count = r_u32(state + 12);
        packet += 24;
        w_u32(state + 12, count + 1);
        w_u32(packet + 4, color);
        w_u16(packet + 12, left);
        w_u16(packet + 8, left);
        w_u16(packet + 16, middle);
        w_u16(packet + 10, upper);
        w_u16(packet + 14, lower);
        w_u16(packet + 18, bottom);
        count = r_u32(state + 12);
        packet += 24;
        w_u32(state + 12, count + 1);
        w_u32(packet + 4, color);
        w_u16(packet + 12, right);
        w_u16(packet + 8, right);
        w_u16(packet + 16, middle);
        w_u16(packet + 10, upper);
        w_u16(packet + 14, lower);
        w_u16(packet + 18, bottom);
        count = r_u32(state + 12) + 1;
        w_u32(state + 12, count);
        return count;
    }
    w_u32(packet + 4, color);
    w_u16(packet + 16, middle);
    w_u16(packet + 12, middle);
    w_u16(packet + 8, middle - quarter);
    w_u16(packet + 16, middle + quarter);
    w_u16(packet + 14, top);
    value = top + eighth;
    w_u16(packet + 18, value);
    w_u16(packet + 10, value);
    count = r_u32(state + 12);
    packet += 24;
    w_u32(state + 12, count + 1);
    w_u32(packet + 4, color);
    w_u16(packet + 16, left);
    w_u16(packet + 12, left);
    w_u16(packet + 8, left);
    w_u16(packet + 16, left + quarter);
    w_u16(packet + 18, upper);
    w_u16(packet + 14, upper);
    w_u16(packet + 10, upper + stretch);
    w_u16(packet + 18, upper - eighth);
    count = r_u32(state + 12);
    packet += 24;
    w_u32(state + 12, count + 1);
    w_u32(packet + 4, color);
    w_u16(packet + 16, left);
    w_u16(packet + 12, left);
    w_u16(packet + 8, left);
    w_u16(packet + 16, left + quarter);
    w_u16(packet + 18, lower);
    w_u16(packet + 14, lower);
    w_u16(packet + 10, lower - stretch);
    w_u16(packet + 18, lower + eighth);
    count = r_u32(state + 12) + 1;
    packet = base + 0x408 + count * 24;
    w_u32(state + 12, count);
    w_u32(packet + 4, color);
    w_u16(packet + 16, middle);
    w_u16(packet + 12, middle);
    w_u16(packet + 8, middle - quarter);
    w_u16(packet + 16, middle + quarter);
    w_u16(packet + 14, bottom);
    value = bottom - eighth;
    w_u16(packet + 18, value);
    w_u16(packet + 10, value);
    count = r_u32(state + 12);
    packet += 24;
    w_u32(state + 12, count + 1);
    w_u32(packet + 4, color);
    w_u16(packet + 16, right);
    w_u16(packet + 12, right);
    w_u16(packet + 8, right);
    w_u16(packet + 16, right - quarter);
    w_u16(packet + 18, upper);
    w_u16(packet + 14, upper);
    w_u16(packet + 10, upper + stretch);
    w_u16(packet + 18, upper - eighth);
    count = r_u32(state + 12);
    packet += 24;
    w_u32(state + 12, count + 1);
    w_u32(packet + 4, color);
    w_u16(packet + 16, right);
    w_u16(packet + 12, right);
    w_u16(packet + 8, right);
    w_u16(packet + 16, right - quarter);
    w_u16(packet + 18, lower);
    w_u16(packet + 14, lower);
    w_u16(packet + 10, lower - stretch);
    w_u16(packet + 18, lower + eighth);
    count = r_u32(state + 12) + 1;
    w_u32(state + 12, count);
    return count;
}

uint32 sub_800401ac(uint32 index, uint32 color)
{
    uint32 record, object, width, a, b, state, count, base, packet, quarter, eighth, flags, i, x, y, value;
    uint16 left, right, top, bottom;
    FUNCTION_MARKER(0x800401AC, "ACE3.BIN");
    record = r_u32(0x800C67A0 + (index << 2));
    color |= 0x42000000;
    if (record == 0)
        return 1;
    object = r_u32(0x800C6ED0 + (index << 2));
    if (object == 0)
        return 1;
    width = r_u16(record + 6);
    a = r_s16(record + 8);
    left = a - (uint32)(sint32)(sint16)width;
    a = r_s16(record + 8);
    b = r_s16(record + 6);
    right = a + b;
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    top = a - b;
    a = r_s16(record + 10);
    b = r_s16(record + 6);
    state = r_u32(0x800BCFB8);
    bottom = a + b;
    count = r_u32(state + 8);
    base = r_u32(0x800BCFBC);
    packet = base + 8 + (count << 4);
    quarter = (sint16)width >> 2;
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        w_u32(packet + 4, color);
        w_u16(packet + 8, x);
        value = (i & 1) ? x - quarter : x + quarter;
        w_u16(packet + 12, value);
        w_u16(packet + 10, y);
        value = i < 2 ? y + quarter : y - quarter;
        w_u16(packet + 14, value);
        count = r_u32(state + 8) + 1;
        if (i != 3)
            packet += 16;
        w_u32(state + 8, count);
    }
    flags = r_u32(object) & 0x20;
    packet += 16;
    if (flags == 0)
        return flags;
    width = r_u16(record + 6);
    eighth = (sint16)width >> 3;
    for (i = 0; i < 4; ++i)
    {
        x = (i & 1) ? right : left;
        y = i < 2 ? top : bottom;
        w_u32(packet + 4, color);
        w_u16(packet + 8, x - eighth);
        w_u16(packet + 12, x + eighth);
        value = i == 0 || i == 3 ? y + eighth : y - eighth;
        w_u16(packet + 10, value);
        value = i == 0 || i == 3 ? y - eighth : y + eighth;
        w_u16(packet + 14, value);
        count = r_u32(state + 8);
        packet += 16;
        w_u32(state + 8, count + 1);
    }
    w_u32(packet + 4, color);
    value = r_u32(record + 8);
    w_u32(packet + 8, value);
    value = r_u32(record + 8);
    w_u32(packet + 12, value);
    count = r_u32(state + 8) + 1;
    w_u32(state + 8, count);
    return count;
}

uint32 sub_8004b3c8(uint32 index)
{
    uint32 table, records, record, object, kind, result, limit;
    sint32 tick;
    FUNCTION_MARKER(0x8004B3C8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    records = r_u32(0x800BCEC0);
    record = records + index * 184;
    kind = r_u8(record + 100);
    object = r_u32(table + (index << 2));
    if (kind == 1)
    {
        tick = r_s16(record + 102);
        sub_80081c30(object + 8, object + 56, tick, 1);
        limit = r_u32(object + 8);
        tick = r_s16(record + 102);
        result = (limit & 31) + 16;
        if (tick >= (sint32)result)
        {
            sub_80081298(object + 8, object + 56, 0);
            sub_8002f3c0(27, object + 8, 0, 0);
            result = 2;
            w_u8(record + 100, result);
        }
        return result;
    }
    if (kind == 0)
    {
        w_u8(record + 100, 1);
        return 1;
    }
    if (kind == 2)
        return sub_80038320(index);
    w_u8(record + 100, 0);
    return 2;
}

uint32 sub_800a7418(uint32 first, uint32 second, uint32 third)
{
    FUNCTION_MARKER(0x800A7418, "ACE3.BIN");
    xport_gte_write_data(12, first);
    xport_gte_write_data(14, third);
    xport_gte_write_data(13, second);
    xport_gte_execute(0x1400006);
    return xport_gte_read_data(24);
}

uint32 sub_8005f1c4(uint32 points, uint32 count, const uint32 *native_points)
{
    uint32 last, current, previous, x, y, positives, i;
    FUNCTION_MARKER(0x8005F1C4, "ACE3.BIN");
    last = points + (count << 4);
    x = (sint32)(native_points ? native_points[(count << 2) - 4] : r_u32(last - 16)) >> 1;
    y = (sint32)(native_points ? native_points[(count << 2) - 2] : r_u32(last - 8)) >> 1;
    current = (x & 0xFFFF) | (y << 16);
    positives = 0;
    for (i = 0; (sint32)i < (sint32)count; ++i)
    {
        previous = current;
        x = (sint32)(native_points ? native_points[0] : r_u32(points)) >> 1;
        y = (sint32)(native_points ? native_points[2] : r_u32(points + 8)) >> 1;
        current = (x & 0xFFFF) | (y << 16);
        if ((sint32)sub_800a7418(0, previous, current) > 0)
            ++positives;
        points += 16;
        if (native_points)
            native_points += 4;
    }
    return positives == 0 || positives == count;
}

uint32 sub_80081c30(uint32 position, uint32 vector, uint32 tick, uint32 kind)
{
    uint32 scene, flags, result, effect;
    FUNCTION_MARKER(0x80081C30, "ACE3.BIN");
    if (kind < 2)
    {
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene);
        if ((flags & 0x100) != 0)
        {
            kind = 1;
            tick = 0;
        }
    }
    switch (kind)
    {
        case 0:
            result = tick & 3;
            if (result != 0)
                return result;
            break;
        case 1:
            result = tick & 1;
            if (result != 0)
                return result;
            break;
        case 2:
            result = tick & 3;
            if (result != 0)
                return result;
            result = game_random() & 1;
            if (result != 0)
                return result;
            break;
        case 3:
            result = tick & 1;
            if (result != 0)
                return result;
            result = game_random() & 3;
            if (result == 0)
                return result;
            break;
    }
    effect = sub_8007ed9c(2, position, vector, 0);
    w_u8(effect + 2, kind);
    return sub_80081bc8(position, vector, kind, 0, 0);
}

uint32 sub_80049af8(uint32 index)
{
    uint32 table, flags, object, record, kind, result, first, second, distance;
    FUNCTION_MARKER(0x80049AF8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    flags = r_u32(0x800C6FD0 + (index << 2));
    object = r_u32(table + (index << 2));
    w_u32(0x800C6FD0 + (index << 2), flags | 0x20);
    record = r_u32(0x800BCEC0) + index * 184;
    kind = r_u8(record + 100);
    switch (kind)
    {
        case 0:
            result = sub_80044034(index);
            if (result == 0)
            {
                result = 1;
                w_u8(record + 100, result);
            }
            break;
        case 1:
            result = sub_80044474(index, record + 104);
            if (result == 0)
            {
                result = 2;
                w_u8(record + 100, result);
            }
            break;
        case 2:
            result = sub_80044034(index);
            if (result == 0)
            {
                result = 3;
                w_u8(record + 100, result);
            }
            break;
        case 3:
            sub_80044360(index, record + 104, 0);
            first = r_u32(object + 12);
            second = r_u32(record + 108);
            distance = first - second;
            if ((sint32)distance < 0)
                distance = 0 - distance;
            result = (sint32)distance > 0x3FFFF;
            if (result == 0)
                w_u8(record + 100, 0);
            break;
        case 4:
            result = sub_80044034(index);
            if (result == 0)
            {
                result = 255;
                w_u8(record + 99, result);
            }
            break;
        default:
            result = 0x80020000;
            w_u8(record + 100, 0);
            break;
    }
    return result;
}

uint32 sub_80044474(uint32 index, uint32 position)
{
    uint32 table, parameters, objects, records, object, record, speed, value, x, z, heading, difference, turn, old_turn, step, minimum;
    sint32 signed_speed, signed_turn, magnitude, turn_magnitude, step_magnitude;
    FUNCTION_MARKER(0x80044474, "ACE3.BIN");
    table = r_u32(0x800BCEF8);
    parameters = r_u32(table + (index << 6) + 20);
    objects = r_u32(0x800BCE5C);
    speed = r_u16(parameters + 20);
    records = r_u32(0x800BCEC0);
    value = r_s16(parameters + 20);
    object = r_u32(objects + (index << 2));
    record = records + index * 184;
    if ((sint32)value >= 0x1001)
        speed = 0x1000;
    if (speed == 0)
        speed = 0x1000;
    x = r_u32(position);
    value = r_u32(object + 8);
    x -= value;
    z = r_u32(position + 8);
    value = r_u32(object + 16);
    z -= value;
    heading = sub_80030d54((sint32)x >> 12, (sint32)z >> 12);
    value = r_u16(object + 26);
    difference = (heading - value) & 0x3FFF;
    if (difference == 0)
        return 0;
    turn = difference;
    if (difference >= 0x2000)
        turn |= 0xC000;
    signed_turn = (sint16)turn;
    signed_speed = (sint16)speed;
    if (signed_turn < -signed_speed)
        turn = 0 - speed;
    else if (signed_speed < signed_turn)
        turn = speed;
    old_turn = r_u16(object + 28);
    w_u16(record + 132, turn - old_turn);
    value = r_s16(parameters + 26);
    old_turn = r_s16(object + 28);
    step = (sint32)(value * old_turn) >> 12;
    minimum = r_s16(parameters + 30);
    magnitude = (sint16)step;
    if (magnitude < 0)
        magnitude = -magnitude;
    value = r_u16(parameters + 30);
    if (magnitude < (sint32)minimum)
    {
        if ((sint16)step < 0)
            step = 0 - value;
        else
            step = value;
    }
    turn_magnitude = (sint16)turn;
    if (turn_magnitude < 0)
        turn_magnitude = -turn_magnitude;
    step_magnitude = (sint16)step;
    if (step_magnitude < 0)
        step_magnitude = -step_magnitude;
    if (turn_magnitude < step_magnitude)
        step = turn;
    heading = r_u16(object + 26);
    w_u16(object + 26, heading + step);
    return 1;
}

uint32 sub_8008ffac(uint32 state)
{
    uint32 root, toggles, input, target, flags, first, packet, result;
    FUNCTION_MARKER(0x8008FFAC, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    toggles = r_s16(root + 0x3AE);
    input = r_u16(root + 0x60);
    if ((toggles & 4) != 0)
    {
        target = r_u32(0x800BCF90);
        flags = r_u32(target + 0x5C8);
        toggles &= 0xFFFFFFFB;
        w_u32(target + 0x5C8, flags | 1);
        w_u16(root + 0x3AE, toggles);
    }
    if ((input & 0x300) != 0)
    {
        if ((input & 0x200) != 0)
            toggles ^= 1;
        if ((input & 0x100) != 0)
        {
            if ((toggles & 2) != 0)
                toggles |= 4;
            toggles ^= 2;
        }
        root = r_u32(0x800BCE60);
        w_u16(root + 0x3AE, toggles);
    }
    if ((toggles & 1) != 0)
    {
        first = r_u32(state);
        packet = r_u32(state + 4);
        result = sub_8008f950(first - 4, packet, 0);
        w_u32(state + 4, result);
    }
    result = toggles & 6;
    if (result != 0)
    {
        target = r_u32(0x800BCF90);
        flags = r_u32(target + 0x5C8);
        w_u32(target + 0x5C8, flags | 1);
        result = toggles & 2;
        if (result != 0)
        {
            first = r_u32(state);
            packet = r_u32(state + 4);
            result = sub_8008488c(first, packet, 0x400);
            w_u32(state + 4, result);
        }
    }
    return result;
}

uint32 sub_8008f950(uint32 ordering, uint32 packet, uint32 offset)
{
    uint32 root, page, current, next, tail, x, value, merge_seed, address, lane, shift, i;
    FUNCTION_MARKER(0x8008F950, "ACE3.BIN");
    current = packet + 8;
    root = r_u32(0x800BCE60);
    page = r_u32(root + 4);
    w_u32(packet, (current & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 4, 0xE6000001);
    page = (page == 0) << 8;
    if (offset != 0)
    {
        current = packet + 20;
        w_u32(packet + 8, (current & 0xFFFFFF) | 0x02000000);
        w_u32(packet + 12, (((page + 256) & 256) << 11) | 0xE5000000);
        w_u32(packet + 16, 0);
    }
    for (x = 0; x < 320; x += 256)
    {
        next = current + 24;
        w_u32(current, (next & 0xFFFFFF) | 0x05000000);
        value = ((page & 256) >> 4) | ((x & 1023) >> 6) | 256 | ((page & 512) << 2) | 0xE1000200;
        w_u32(current + 4, value);
        w_u32(current + 8, 0x66838383);
        w_u32(current + 12, x);
        w_u16(current + 16, 0);
        w_u32(current + 20, 0x00F00100);
        current = next;
    }
    if (offset != 0)
    {
        tail = current;
        value = r_u32(offset);
        merge_seed = 0xE5000000;
        w_u32(current, 0x02000000);
        w_u32(current + 8, 0);
        value = (value & 0x7FF) | (((value << 5) >> 21) << 11) | 0xE5000000;
        w_u32(current + 4, value);
        current += 12;
    }
    else
    {
        tail = current - 24;
        merge_seed = current;
    }
    /* Preserve the original LWL and two SWL merges for every byte alignment */
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    value = r_u32(address & 0xFFFFFFFC);
    value = (value << shift) | (merge_seed & (0x00FFFFFF >> (lane << 3)));
    address = tail + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    value = packet << 8;
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    return current;
}

uint32 sub_8004a678(uint32 index)
{
    uint32 table, parameters, records, record, type, root, mission, kind, result, value, first, second;
    FUNCTION_MARKER(0x8004A678, "ACE3.BIN");
    table = r_u32(0x800BCEF8);
    parameters = r_u32(table + (index << 6) + 24);
    records = r_u32(0x800BCEC0);
    type = r_u8(parameters);
    record = records + index * 184;
    if (type >= 2)
    {
        root = r_u32(0x800BCE60);
        mission = r_s16(root + 0x38C);
    }
    if (type < 2 || mission == 32 || mission == 47 || mission == 53)
    {
        kind = r_u8(record + 100);
        switch (kind)
        {
            case 0:
                value = r_u16(record + 124);
                w_u16(record + 130, 0);
                w_u16(record + 128, 0);
                w_u8(record + 100, 2);
                w_u16(record + 132, value);
                /* Fall through to the live countdown read */
            case 2:
                result = r_s16(record + 132);
                if (result == 0)
                {
                    value = r_u16(record + 120);
                    result = 3;
                    w_u8(record + 100, result);
                    w_u16(record + 128, value);
                }
                return result;
            case 3:
                result = r_s16(record + 128);
                if (result != 0)
                    return result;
                w_u8(record + 100, 4);
                return 4;
            case 4:
                w_u8(record + 99, 255);
                return 255;
            default:
                result = kind < 3 ? 1 : 255;
                w_u8(record + 100, 0);
                return result;
        }
    }
    kind = r_u8(record + 100);
    if (kind >= 2)
    {
        if (kind == 4)
            w_u8(record + 99, 255);
        else
            w_u8(record + 100, 0);
        return 255;
    }
    if (kind == 0)
    {
        first = r_u32(record + 120);
        second = r_u32(record + 124);
        w_u32(record + 128, first);
        w_u32(record + 132, second);
        w_u8(record + 100, 1);
    }
    result = r_s16(record + 128);
    if (result != 0)
        return result;
    result = r_s16(record + 132);
    if (result != 0)
        return result;
    w_u8(record + 100, 4);
    return 4;
}

uint32 sub_8004afac(uint32 index)
{
    uint32 table, records, record, parameters, object, kind, upper, lower, result, altitude, position[3];
    FUNCTION_MARKER(0x8004AFAC, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    records = r_u32(0x800BCEC0);
    record = records + index * 184;
    parameters = r_u32(0x800BCEF8);
    object = r_u32(table + (index << 2));
    parameters = r_u32(parameters + (index << 6) + 24);
    kind = r_u8(record + 100);
    upper = r_u32(parameters + 8);
    lower = r_u32(parameters + 4);
    if (kind == 0)
    {
        result = sub_8004409c(index);
        if (result != 0)
            return result;
        altitude = r_u32(object + 12);
        result = (sint32)altitude < (sint32)lower;
        if ((sint32)upper < (sint32)altitude)
        {
            w_u8(record + 100, 1);
            return result;
        }
        if (result != 0)
        {
            w_u8(record + 100, 1);
            return 2;
        }
        w_u8(record + 100, 2);
        return 2;
    }
    if (kind == 1)
    {
        altitude = r_u32(object + 12);
        if ((sint32)upper < (sint32)altitude)
        {
            position[2] = 0;
            position[0] = 0;
            position[1] = lower;
            return sub_80044360(index, 0, position);
        }
        if ((sint32)altitude < (sint32)lower)
        {
            position[2] = 0;
            position[0] = 0;
            position[1] = upper;
            return sub_80044360(index, 0, position);
        }
        w_u8(record + 100, 2);
        return 2;
    }
    if (kind == 2)
        w_u8(record + 99, 255);
    else
        w_u8(record + 100, 0);
    return 255;
}

uint32 sub_8004b4d8(uint32 index)
{
    uint32 table, records, record, object, kind, flags, value, random, scene, mode, root, mission, threshold, altitude, tick, result;
    FUNCTION_MARKER(0x8004B4D8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    records = r_u32(0x800BCEC0);
    record = records + index * 184;
    kind = r_u8(record + 100);
    object = r_u32(table + (index << 2));
    if (kind == 0)
    {
        w_u16(record + 132, 0);
        w_u16(record + 130, 0);
        w_u16(record + 128, 0);
        w_u16(record + 8, 0);
        w_u16(record + 6, 0);
        w_u16(record + 4, 0);
        flags = r_u32(object + 84);
        w_u32(object + 40, 0);
        w_u32(object + 36, 0);
        w_u32(object + 32, 0);
        w_u16(object + 48, 0);
        w_u16(object + 50, 0);
        w_u16(object + 52, 0);
        w_u32(object + 84, flags | 0x800000);
        random = game_random();
        scene = r_u32(0x800BCE54);
        mode = r_u32(scene + 12) & 3;
        switch (mode)
        {
            case 0:
                w_u16(object + 52, (random & 1023) - 512);
                break;
            case 1:
                w_u16(object + 50, (random & 255) - 128);
                break;
            case 2:
                w_u16(object + 48, (random & 255) - 128);
                break;
            case 3:
                w_u16(object + 48, 0xFFF0);
                break;
        }
        w_u8(record + 100, 1);
        return 1;
    }
    if (kind == 2)
        return sub_80038320(index);
    if (kind != 1)
    {
        w_u8(record + 100, 0);
        return 2;
    }
    flags = r_u32(object + 84);
    value = r_u32(object + 60);
    w_u32(object + 84, flags | 0x800000);
    w_u32(object + 60, value + 1024);
    tick = r_s16(record + 102);
    sub_80081c30(object + 8, object + 56, tick, 0);
    root = r_u32(0x800BCE60);
    mission = r_s16(root + 0x38C);
    threshold = mission == 32 || mission == 47 || mission == 53 ? 0 : 0xFF800000;
    altitude = r_u32(object + 12);
    result = (sint32)threshold < (sint32)altitude;
    if (result != 0)
    {
        sub_80081298(object + 8, object + 56, 0);
        sub_8002f3c0(27, object + 8, 0, 0);
        w_u8(record + 100, 2);
        return 2;
    }
    return result;
}

uint32 sub_8004b6e4(uint32 index)
{
    uint32 table, records, record, object, kind, value, flags, speed, tick, altitude, result;
    sint32 angle;
    FUNCTION_MARKER(0x8004B6E4, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    records = r_u32(0x800BCEC0);
    record = records + index * 184;
    kind = r_u8(record + 100);
    object = r_u32(table + (index << 2));
    if (kind == 0)
    {
        w_u16(record + 132, 0);
        w_u16(record + 130, 0);
        w_u16(record + 128, 0);
        w_u16(record + 8, 0);
        w_u16(record + 6, 0);
        w_u16(record + 4, 0);
        w_u32(object + 40, 0);
        w_u32(object + 36, 0);
        w_u32(object + 32, 0);
        w_u8(record + 100, 1);
        return 1;
    }
    if (kind == 2)
        return sub_80038320(index);
    if (kind != 1)
    {
        w_u8(record + 100, 0);
        return 2;
    }
    angle = r_s16(object + 28);
    value = r_u16(object + 28);
    if (angle < 0 && angle >= -1023)
        w_u16(object + 28, value - 4);
    else if (value < 1024)
        w_u16(object + 28, value + 4);
    else
    {
        angle = r_s16(object + 24);
        value = r_u16(object + 24);
        if ((angle < 0 && angle >= -2047) || value < 2048)
            w_u16(object + 24, value - 4);
    }
    flags = r_u32(object + 84);
    speed = r_u32(object + 60);
    w_u32(object + 84, flags | 0x800000);
    w_u32(object + 60, speed + 669);
    tick = r_s16(record + 102);
    sub_80081c30(object + 8, object + 56, tick, 0);
    altitude = r_u32(object + 12);
    result = (sint32)altitude > (sint32)0xFF800000;
    if (result != 0)
    {
        sub_80081298(object + 8, object + 56, 0);
        sub_8002f3c0(27, object + 8, 0, 0);
        w_u8(record + 100, 2);
        return 2;
    }
    return result;
}
