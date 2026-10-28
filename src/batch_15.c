#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_80068334(uint32 scene, uint32 ordering, uint32 packet, uint32 scratch, uint8 *native_scratch)
{
    uint8 geometry[6800];
    uint8 *meshes = geometry + 1536, *animated = geometry + 5176, *fixed = geometry + 5656, *sprites = geometry + 6136;
    uint32 offset, root, a, b, c, d, e, value;
    FUNCTION_MARKER(0x80068334, "ACE3.BIN");
    if (native_scratch)
        xport_store_le32(native_scratch, 0);
    else
        w_u32(scratch, 0);
    offset = sub_80067144(0, geometry);
    sub_80067380(0, geometry + offset);
    sub_80067494(scene, geometry, meshes, animated, fixed, sprites, scratch, native_scratch);
    w_u32(0x1F800000, ordering);
    w_u32(0x1F800004, packet);
    sub_80066c80(0, 0x1F800000, sprites);
    sub_80066dcc(0, 0x1F800000, meshes);
    sub_80067088(0, 0x1F800000, animated);
    root = r_u32(0x800BCE54);
    a = r_u32(root + 76);
    b = r_u32(root + 80);
    c = r_u32(root + 84);
    d = r_u32(root + 88);
    e = r_u32(root + 92);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    sub_800626e0(0, 0x1F800000, sprites);
    sub_80062bd4(0, 0x1F800000, meshes);
    root = r_u32(0x800BCE54);
    value = r_u16(root + 12);
    w_u16(0x1F8003F4, value);
    sub_800637b0(0, 0x1F800000, animated);
    sub_800637b0(0, 0x1F800000, fixed);
    return r_u32(0x1F800004);
}

/* Native SDK adapters retain the original CPU matrix and GTE instruction contracts */
void sky_load_rotation(const MATRIX *matrix)
{
    uint32 a = xport_load_le32((const uint8 *)matrix), b = xport_load_le32((const uint8 *)matrix + 4), c = xport_load_le32((const uint8 *)matrix + 8), d = xport_load_le32((const uint8 *)matrix + 12), e = xport_load_le16((const uint8 *)matrix + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
}

void sky_load_translation(const MATRIX *matrix)
{
    uint32 a = xport_load_le32((const uint8 *)matrix + 20), b = xport_load_le32((const uint8 *)matrix + 24), c = xport_load_le32((const uint8 *)matrix + 28);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
}

void sky_transform(const uint16 vector[3], uint32 output[3])
{
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x480012);
    output[0] = xport_gte_read_data(25);
    output[1] = xport_gte_read_data(26);
    output[2] = xport_gte_read_data(27);
    xport_gte_read_control(31);
}

void sky_project(const uint16 vector[3], uint16 output[2])
{
    uint32 screen;
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x180001);
    screen = xport_gte_read_data(14);
    output[0] = screen;
    output[1] = screen >> 16;
    xport_gte_read_data(8);
    xport_gte_read_control(31);
    xport_gte_read_data(19);
}

uint32 sky_order_boundary(const uint32 points[4][4], uint32 minimum, uint32 maximum, uint32 ordered[4])
{
    uint32 remaining[4], count = 0, i, choice, first, second, difference = minimum - maximum;
    for (i = 0; i < 4; ++i)
        if ((~(1 << minimum) & 15 & ~(1 << maximum) & (1 << i)) != 0)
            remaining[count++] = i;
    if ((sint32)difference < 0)
        difference = 0 - difference;
    if (difference == 2)
    {
        choice = (sint32)points[remaining[1]][2] < (sint32)points[remaining[0]][2];
        ordered[0] = remaining[choice];
        ordered[1] = minimum;
        ordered[2] = remaining[choice ^ 1];
        ordered[3] = maximum;
        return 1;
    }
    choice = (sint32)points[remaining[1]][0] < (sint32)points[remaining[0]][0];
    first = remaining[choice ^ 1];
    second = remaining[choice];
    if ((sint32)points[first][2] < (sint32)points[maximum][2])
    {
        ordered[0] = second;
        ordered[1] = minimum;
        ordered[2] = maximum;
        ordered[3] = first;
    }
    else
    {
        ordered[0] = minimum;
        ordered[1] = second;
        ordered[2] = first;
        ordered[3] = maximum;
    }
    return 0;
}

uint32 sub_80033814(void)
{
    NATIVE_HUD_CONTEXT context;
    uint16 fallback[4] = {0, 0, 0, 0};
    uint32 table, object, controller, numerator, divisor, ratio, scene, camera, value, other, output, flags, target, index, flight;
    FUNCTION_MARKER(0x80033814, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    controller = r_u32(0x800BD098);
    numerator = (uint32)r_s16(object + 78) << 12;
    divisor = r_u32(controller + 20);
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (sint32)numerator / (sint32)divisor;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    context.words[6] = ratio;
    if ((sint32)ratio > 4096)
        context.words[6] = 4096;
    if ((sint32)context.words[6] < 0)
        context.words[6] = 0;
    context.words[0] = r_u32(controller + 36);
    value = r_u32(controller + 4);
    context.words[9] = value;
    other = r_u32(controller + 32);
    context.words[10] = other;
    if ((sint32)value >= 7)
        context.words[9] = 6;
    if ((sint32)other >= 7)
        context.words[10] = 6;
    value = r_u32(controller + 28);
    output = r_u32(0x800BD0A0);
    context.words[7] = value;
    value = r_u32(controller + 8);
    context.words[13] = camera;
    context.words[4] = camera + 180;
    context.words[14] = output;
    context.words[8] = value;
    context.words[11] = r_u32(controller + 40);
    context.words[12] = r_u32(controller + 44);
    flags = r_u32(camera);
    index = flags & 63;
    context.delta_angles = fallback;
    if ((flags & 0x100) != 0)
    {
        target = table + (index << 2);
        object = r_u32(target);
        context.words[1] = object + 8;
        context.words[2] = object + 24;
        if (index < 2)
        {
            flight = r_u32(0x800BD094);
            value = r_u32(flight + 72);
            context.words[3] = flight + 56;
            context.words[5] = value;
            context.delta_angles = 0;
        }
        else
        {
            object = r_u32(target);
            value = r_u32(object + 40);
            context.words[5] = value;
        }
    }
    else
    {
        context.words[1] = camera + 4;
        context.words[2] = camera + 20;
        context.words[5] = 0;
    }
    return sub_8003a0b4(0, &context);
}

uint32 sub_80066a74(uint32 distance, uint32 speed)
{
    uint32 total = 0, ticks = 0;
    FUNCTION_MARKER(0x80066A74, "ACE3.BIN");
    if ((sint32)distance > 0)
        do
        {
            speed += 669;
            total += speed;
            ++ticks;
        } while ((sint32)total < (sint32)distance);
    return ticks;
}

uint32 sub_80066590(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    static const sint16 offsets[6][4] = {{-10, -10, 10, 10}, {-10, 10, 10, -10}, {0, -6, 6, 0}, {6, 0, 0, 6}, {0, 6, -6, 0}, {-6, 0, 0, -6}};
    MATRIX matrix;
    uint16 vector[3], screen[2];
    uint32 table, selected, object, data, mission, flags, target, target_y, value, other, distance, ticks, direct, dx, dz, scene, camera, root, base, count, packet, i, result;
    FUNCTION_MARKER(0x80066590, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    selected = r_u8(0x800C6D6E);
    object = r_u32(table);
    if (selected == 255)
        goto fallback_height_66590;
    data = r_u32(0x800BCE60);
    mission = r_s16(data + 908);
    if (mission != 13)
    {
        flags = r_u32(0x800C6FD0 + (selected << 2));
        if ((~flags & 0x400) == 0)
            goto fallback_height_66590;
    }
    target = r_u32(table + (selected << 2));
    target_y = r_u32(target + 12);
    value = r_u32(object + 12);
    if ((sint32)target_y >= (sint32)value)
        goto height_ready_66590;
fallback_height_66590:
    target_y = native_state ? native_state->words[11] : r_u32(state + 44);
height_ready_66590:
    value = r_u32(object + 12);
    distance = (sint32)value < (sint32)target_y ? target_y - value : 0;
    value = r_u32(object + 60);
    ticks = sub_80066a74(distance, value + (uint32)((sint32)value >> 1));
    data = r_u32(0x800BCE60);
    direct = r_u8(data + 938) == 9;
    if (direct == 0 && sub_8003a5c0() == 1)
    {
        data = r_u32(0x800BCE60);
        direct = r_u8(data + 939) == 9;
    }
    if (direct != 0)
    {
        value = r_u32(object + 56);
        dx = value * ticks;
        value = r_u32(object + 64);
        dz = value * ticks;
    }
    else
    {
        value = r_u32(object + 56);
        dx = (value + (uint32)((sint32)value >> 1)) * ticks;
        value = r_u32(object + 64);
        dz = (value + (uint32)((sint32)value >> 1)) * ticks;
    }
    scene = r_u32(0x800BCE54);
    value = r_u32(object + 8);
    camera = r_u32(scene + 40);
    other = r_u32(camera + 4);
    vector[0] = (sint32)(value + dx - other) >> 12;
    value = r_u32(camera + 8);
    vector[1] = (sint32)(target_y - value) >> 12;
    value = r_u32(object + 16);
    other = r_u32(camera + 12);
    vector[2] = (sint32)(value + dz - other) >> 12;
    camera = r_u32(scene + 40);
    sub_800308f0(camera + 20, 0, &matrix, 0);
    matrix.t[2] = 0;
    matrix.t[1] = 0;
    matrix.t[0] = 0;
    sky_load_rotation(&matrix);
    sky_load_translation(&matrix);
    sky_project(vector, screen);
    root = r_u32(0x800BD074);
    base = r_u32(0x800BD078);
    count = r_u32(root + 20);
    packet = base + (count << 4) + 984;
    for (i = 0; i < 6; ++i)
    {
        value = r_u8(0x800C6D68);
        w_u8(packet + 4, value);
        value = r_u8(0x800C6D69);
        w_u8(packet + 5, value);
        value = r_u8(0x800C6D6A);
        w_u8(packet + 6, value);
        w_u16(packet + 8, screen[0] + offsets[i][0]);
        w_u16(packet + 10, screen[1] + offsets[i][1]);
        if (i == 2 || i == 4)
            root = r_u32(0x800BD074);
        w_u16(packet + 12, screen[0] + offsets[i][2]);
        if (i == 0)
            root = r_u32(0x800BD074);
        w_u16(packet + 14, screen[1] + offsets[i][3]);
        if ((i & 1) != 0)
            root = r_u32(0x800BD074);
        result = r_u32(root + 20) + 1;
        w_u32(root + 20, result);
        packet += 16;
    }
    return result;
}

uint32 sub_8004291c(uint32 kind)
{
    FUNCTION_MARKER(0x8004291C, "ACE3.BIN");
    w_u8(0x800BCFD8, 1);
    w_u8(0x800BCFD9, kind + 12);
    w_u16(0x800BCFDA, 0);
    return 0x800BCFD8;
}

uint32 sub_8004357c(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    static const sint16 diamond[8][4] = {{160, 156, 184, 180}, {184, 180, 160, 204}, {160, 204, 136, 180}, {136, 180, 160, 156}, {160, 156, 184, 180}, {184, 180, 160, 204}, {160, 204, 136, 180}, {136, 180, 160, 156}};
    static const uint8 root_after[8] = {1, 0, 2, 3, 0, 0, 0, 0};
    uint32 table, target, object, x, z, angle, scene, value, root, count, base, packet, i, j, coordinates[4];
    sint32 pitch, offset;
    FUNCTION_MARKER(0x8004357C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    target = native_state ? native_state->words[14] : r_u32(state + 56);
    object = r_u32(table);
    if ((sub_8006cb30() & 0x80) != 0)
        return 0;
    if (r_s32(target + 24) >= 8193)
    {
        x = r_u32(target + 28);
        z = r_u32(target + 36);
        angle = sub_80030d54((sint32)x >> 12, (sint32)(0 - z) >> 12);
        value = r_u16(object + 26);
        angle = (value + angle) & 0x3FFF;
        if (angle >= 0x2000)
            angle |= 0xC000;
        if ((sint16)angle > 256)
        {
            if ((sint16)angle >= 1025)
            {
                scene = r_u32(0x800BCE54);
                if ((r_u32(scene + 12) & 127) == 0)
                    sub_8004291c(13);
            }
            sub_80043efc(0, 80, 120);
        }
        else if ((sint16)angle < -256)
        {
            if ((sint16)angle < -1024)
            {
                scene = r_u32(0x800BCE54);
                if ((r_u32(scene + 12) & 127) == 0)
                    sub_8004291c(12);
            }
            sub_80043efc(1, 240, 120);
        }
        value = r_u32(target + 32);
        if ((sint32)value > 0)
        {
            pitch = r_s16(object + 24);
            if (pitch < 0)
            {
                if (pitch < -2048)
                {
                    scene = r_u32(0x800BCE54);
                    if ((r_u32(scene + 12) & 127) == 64)
                        sub_8004291c(10);
                }
                sub_80043efc(2, 160, 60);
            }
            value = r_u32(target + 32);
        }
        if ((sint32)value < -1)
        {
            pitch = r_s16(object + 24);
            if (pitch > 0)
            {
                if (pitch >= 2049)
                {
                    scene = r_u32(0x800BCE54);
                    if ((r_u32(scene + 12) & 127) == 64)
                        sub_8004291c(11);
                }
                sub_80043efc(3, 160, 180);
            }
        }
    }
    offset = (r_s32(target + 28) >> 12) / -50;
    if (offset > 64)
        offset = 64;
    if (offset < -64)
        offset = -64;
    root = r_u32(0x800BCFE0);
    count = r_u32(root + 12);
    base = r_u32(0x800BCFE4);
    packet = base + (count << 4) + 3072;
    w_u8(packet + 4, 0);
    w_u8(packet + 5, offset != 0 ? 255 : 0);
    w_u8(packet + 6, offset != 0 ? 0 : 255);
    w_u16(packet + 8, offset + 160);
    w_u16(packet + 12, offset + 160);
    root = r_u32(0x800BCFE0);
    w_u16(packet + 10, 156);
    w_u16(packet + 14, 204);
    count = r_u32(root + 12);
    w_u32(root + 12, count + 1);
    offset = (r_s32(target + 32) >> 12) / -80;
    if (offset > 64)
        offset = 64;
    if (offset < -64)
        offset = -64;
    packet += 16;
    w_u8(packet + 4, 0);
    w_u8(packet + 5, offset != 0 ? 255 : 0);
    w_u8(packet + 6, offset != 0 ? 0 : 255);
    root = r_u32(0x800BCFE0);
    w_u16(packet + 8, 136);
    w_u16(packet + 10, offset + 180);
    w_u16(packet + 12, 184);
    w_u16(packet + 14, offset + 180);
    count = r_u32(root + 12);
    w_u32(root + 12, count + 1);
    offset = (r_s32(target + 36) >> 12) / -150;
    if (offset > 24)
        offset = 24;
    if (offset < -24)
        offset = -24;
    for (i = 0; i < 8; ++i)
    {
        packet += 16;
        w_u8(packet + 4, 0);
        w_u8(packet + 5, offset != 0 ? 255 : 0);
        w_u8(packet + 6, offset != 0 ? 0 : 255);
        for (j = 0; j < 4; ++j)
            coordinates[j] = diamond[i][j];
        if (i < 4)
        {
            if (i == 0)
            {
                coordinates[1] -= offset;
                coordinates[2] += offset;
            }
            else if (i == 1)
            {
                coordinates[0] += offset;
                coordinates[3] += offset;
            }
            else if (i == 2)
            {
                coordinates[1] += offset;
                coordinates[2] -= offset;
            }
            else
            {
                coordinates[0] -= offset;
                coordinates[3] -= offset;
            }
        }
        for (j = 0; j < 4; ++j)
        {
            if (j == root_after[i])
                root = r_u32(0x800BCFE0);
            w_u16(packet + 8 + j * 2, coordinates[j]);
        }
        count = r_u32(root + 12);
        w_u32(root + 12, count + 1);
    }
    return 0;
}

uint32 sub_80043e0c(uint32 first, uint32 second, uint32 third, uint32 phase)
{
    uint32 root, count, base, scene, frame, packet, result;
    FUNCTION_MARKER(0x80043E0C, "ACE3.BIN");
    root = r_u32(0x800BCFE0);
    count = r_u32(root + 8);
    base = r_u32(0x800BCFE4);
    scene = r_u32(0x800BCE54);
    frame = r_u32(scene + 12);
    packet = base + count * 24;
    w_u8(packet + 4, 0);
    w_u8(packet + 6, 0);
    w_u32(packet + 8, first);
    w_u32(packet + 12, second);
    w_u32(packet + 16, third);
    w_u8(packet + 5, ((frame + phase) & 15) << 4);
    root = r_u32(0x800BCFE0);
    result = r_u32(root + 8) + 1;
    w_u32(root + 8, result);
    return result;
}

uint32 sub_80043e74(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    uint32 root, count, base, packet, result;
    FUNCTION_MARKER(0x80043E74, "ACE3.BIN");
    root = r_u32(0x800BCFE0);
    count = r_u32(root + 8);
    base = r_u32(0x800BCFE4);
    packet = base + count * 24;
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    w_u32(packet + 8, first);
    w_u32(packet + 12, second);
    w_u32(packet + 16, fourth);
    root = r_u32(0x800BCFE0);
    count = r_u32(root + 8);
    packet += 24;
    w_u32(root + 8, count + 1);
    w_u8(packet + 4, 0);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    w_u32(packet + 8, first);
    w_u32(packet + 12, third);
    w_u32(packet + 16, fourth);
    root = r_u32(0x800BCFE0);
    result = r_u32(root + 8) + 1;
    w_u32(root + 8, result);
    return result;
}

/* SDK local-vector boundaries preserve the original split GTE operations */
static void hud_apply_long(const uint32 input[3], uint32 output[3])
{
    uint32 low[3], high[3], i, magnitude, value;
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = (sint32)value >> 15;
            low[i] = value & 0x7FFF;
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x41E012);
    high[0] = xport_gte_read_data(25);
    high[1] = xport_gte_read_data(26);
    high[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x49E012);
    low[0] = xport_gte_read_data(25);
    low[1] = xport_gte_read_data(26);
    low[2] = xport_gte_read_data(27);
    output[0] = low[0] + (high[0] << 3);
    output[1] = low[1] + (high[1] << 3);
    output[2] = low[2] + (high[2] << 3);
}

static void hud_load_scene_rotation(uint32 matrix)
{
    uint32 a = r_u32(matrix), b = r_u32(matrix + 4), c = r_u32(matrix + 8), d = r_u32(matrix + 12), e = r_u32(matrix + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
}

static void hud_compose_rotation(const MATRIX *matrix)
{
    const uint8 *data = (const uint8 *)matrix;
    uint32 a, b, c, x, y, z, p, q, r, u, v, w;
    a = xport_load_le16(data);
    b = xport_load_le32(data + 4);
    c = xport_load_le32(data + 12);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16(data + 2);
    b = xport_load_le32(data + 8);
    c = (sint32)(sint16)xport_load_le16(data + 14);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16(data + 4);
    b = xport_load_le32(data + 8);
    c = xport_load_le16(data + 16);
    p = xport_gte_read_data(9);
    q = xport_gte_read_data(10);
    r = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    u = xport_gte_read_data(9);
    v = xport_gte_read_data(10);
    w = xport_gte_read_data(11);
    xport_gte_write_control(0, (x & 0xFFFF) | (p << 16));
    xport_gte_write_control(1, (u & 0xFFFF) | (y << 16));
    xport_gte_write_control(2, (q & 0xFFFF) | (v << 16));
    xport_gte_write_control(3, (z & 0xFFFF) | (r << 16));
    xport_gte_write_control(4, w);
}

static uint32 hud_sdk_sine(uint32 angle)
{
    uint32 negative = (sint32)angle < 0, value;
    if (negative)
        angle = 0 - angle;
    angle &= 0xFFF;
    if (angle <= 1024)
        value = r_s16(0x800B65C4 + angle * 2);
    else if (angle <= 2048)
        value = r_s16(0x800B65C4 + (2048 - angle) * 2);
    else if (angle <= 3072)
        value = 0 - (uint32)r_s16(0x800B55C4 + angle * 2);
    else
        value = 0 - (uint32)r_s16(0x800B65C4 + (4096 - angle) * 2);
    return negative ? 0 - value : value;
}

static uint32 hud_sdk_cosine(uint32 angle)
{
    if ((sint32)angle < 0)
        angle = 0 - angle;
    angle &= 0xFFF;
    if (angle <= 1024)
        return r_s16(0x800B65C4 + (1024 - angle) * 2);
    if (angle <= 2048)
        return 0 - (uint32)r_s16(0x800B5DC4 + angle * 2);
    if (angle <= 3072)
        return 0 - (uint32)r_s16(0x800B65C4 + (3072 - angle) * 2);
    return r_s16(0x800B4DC4 + angle * 2);
}

uint32 sub_80042d98(uint32 state, const NATIVE_HUD_CONTEXT *native_state)
{
    MATRIX matrix;
    uint16 angles[4], vector[3];
    uint32 input[3], output[3], screen[4], scene, target, table, object, camera, a, b, value, shift, group, step, index, vertices, xbase, ybase, depth, flags, angle, distance, root, count, base, packet, result;
    sint32 pitch, dx, dy;
    FUNCTION_MARKER(0x80042D98, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    target = native_state ? native_state->words[14] : r_u32(state + 56);
    table = r_u32(0x800BCE5C);
    camera = r_u32(scene + 40);
    object = r_u32(table);
    a = r_u32(target);
    b = r_u32(camera + 4);
    input[0] = (sint32)(a - b) >> 12;
    camera = r_u32(scene + 40);
    a = r_u32(target + 4);
    b = r_u32(camera + 8);
    input[1] = (sint32)(a - b) >> 12;
    camera = r_u32(scene + 40);
    a = r_u32(target + 8);
    b = r_u32(camera + 12);
    input[2] = (sint32)(a - b) >> 12;
    value = r_u32(0x800BCE60);
    shift = r_s16(value + 908) == 25 ? 3 : 8;
    value = r_s16(target + 18);
    input[0] += (uint32)((sint32)hud_sdk_sine(value) >> shift);
    value = r_s16(target + 18);
    input[2] += (uint32)((sint32)hud_sdk_cosine(value) >> shift);
    scene = r_u32(0x800BCE54);
    hud_load_scene_rotation(scene + 76);
    hud_apply_long(input, output);
    matrix.t[0] = output[0];
    matrix.t[1] = output[1];
    matrix.t[2] = output[2];
    rotation_build(target + 16, 0, &matrix, 0);
    scene = r_u32(0x800BCE54);
    hud_load_scene_rotation(scene + 76);
    hud_compose_rotation(&matrix);
    sky_load_translation(&matrix);
    for (group = 0; group < 3; ++group)
    {
        xbase = group == 0 ? 0x800B14C8 : group == 1 ? 0x800B14D0 : 0x800B14E8;
        ybase = group < 2 ? 0x800B14D8 : 0x800B14F0;
        vertices = group < 2 ? 3 : 4;
        for (step = 0; step < 16; ++step)
        {
            for (index = 0; index < vertices; ++index)
            {
                vector[0] = r_u16(xbase + index * 2);
                value = r_u16(ybase + index * 2);
                if (group < 2)
                {
                    vector[1] = value - 8 * step * step;
                    value = r_u16(0x800B14E0 + index * 2);
                    vector[2] = value - step * 1024;
                }
                else
                {
                    vector[2] = 0 - step * 1024;
                    vector[1] = value - 8 * step * step;
                }
                xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
                xport_gte_write_data(1, vector[2]);
                xport_gte_execute(0x180001);
                screen[index] = xport_gte_read_data(14);
                xport_gte_read_data(8);
                flags = xport_gte_read_control(31);
                depth = xport_gte_read_data(19);
                if ((sint32)flags < 0)
                    break;
            }
            if (index == vertices)
            {
                if (group < 2)
                    sub_80043e0c(screen[0], screen[1], screen[2], step);
                else
                    sub_80043e74(screen[0], screen[1], screen[2], screen[3]);
            }
        }
    }
    a = r_u32(target + 16);
    b = r_u32(target + 20);
    angles[0] = a;
    angles[1] = 0x4000 - (a >> 16);
    angles[2] = b;
    angles[3] = b >> 16;
    table = r_u32(0x800BCE5C);
    value = r_u32(table);
    b = r_u32(target);
    a = r_u32(value + 8);
    input[0] = (sint32)(a - b) >> 12;
    value = r_u32(table);
    b = r_u32(target + 4);
    a = r_u32(value + 12);
    input[1] = (sint32)(a - b) >> 12;
    value = r_u32(table);
    b = r_u32(target + 8);
    a = r_u32(value + 16);
    input[2] = (sint32)(a - b) >> 12;
    rotation_build(0, 0, &matrix, angles);
    sky_load_rotation(&matrix);
    hud_apply_long(input, output);
    a = r_u32(target + 28);
    b = r_u32(target + 36);
    angle = sub_80030d54((sint32)a >> 12, (sint32)(0 - b) >> 12);
    value = r_u16(target + 46);
    angle = ((angle << 1) + value) & 0x3FFF;
    if (angle - 513 < 0x1DFF)
    {
        if (angle >= 2049)
        {
            scene = r_u32(0x800BCE54);
            if ((r_u32(scene + 12) & 127) == 0)
                sub_8004291c(13);
        }
        sub_80043efc(0, 80, 120);
    }
    else if (angle - 0x2000 < 0x1E00)
    {
        if (angle < 0x3800)
        {
            scene = r_u32(0x800BCE54);
            if ((r_u32(scene + 12) & 127) == 0)
                sub_8004291c(12);
        }
        sub_80043efc(1, 240, 120);
    }
    if (r_u32(target + 52) == 1)
    {
        scene = r_u32(0x800BCE54);
        if ((r_u32(scene + 12) & 127) == 0)
            sub_8004291c(3);
    }
    else
    {
        a = r_u32(target + 32);
        b = r_u32(target + 36);
        value = sub_80030d54((sint32)a >> 12, (sint32)(0 - b) >> 12);
        a = r_u16(object + 24);
        pitch = (sint16)(a - value);
        if (pitch > 512)
        {
            distance = r_u32(target + 24);
            if ((sint32)distance >= 2049 && (sint32)distance < 16384)
            {
                value = r_u32(target + 36);
                if ((sint32)value < 0 && pitch >= 2049)
                {
                    scene = r_u32(0x800BCE54);
                    if ((r_u32(scene + 12) & 127) == 64)
                        sub_8004291c(11);
                }
                sub_80043efc(3, 160, 180);
                goto hud_crosshair_42d98;
            }
        }
        value = native_state ? native_state->words[12] : r_u32(state + 48);
        if ((sint32)value <= 0xFFFFF || pitch < -512)
        {
            scene = r_u32(0x800BCE54);
            if ((r_u32(scene + 12) & 127) == 64)
                sub_8004291c(10);
            sub_80043efc(2, 160, 60);
        }
    }
hud_crosshair_42d98:
    dx = (sint32)(0 - output[0]) / 100;
    if (dx > 64)
        dx = 64;
    if (dx < -64)
        dx = -64;
    dy = (sint32)(0 - output[1] - 70) / 100;
    if (dy > 64)
        dy = 64;
    if (dy < -64)
        dy = -64;
    root = r_u32(0x800BCFE0);
    count = r_u32(root + 12);
    base = r_u32(0x800BCFE4);
    packet = base + (count << 4) + 3072;
    for (index = 0; index < 4; ++index)
    {
        w_u8(packet + 4, 0);
        w_u8(packet + 5, index < 2 ? 255 : 64);
        w_u8(packet + 6, 0);
        root = r_u32(0x800BCFE0);
        if ((index & 1) == 0)
        {
            value = index == 0 ? dx + 160 : 160;
            w_u16(packet + 12, value);
            w_u16(packet + 8, value);
            w_u16(packet + 10, 100);
            w_u16(packet + 14, 140);
        }
        else
        {
            value = index == 1 ? dy + 120 : 120;
            w_u16(packet + 8, 140);
            w_u16(packet + 12, 180);
            w_u16(packet + 14, value);
            w_u16(packet + 10, value);
        }
        result = r_u32(root + 12) + 1;
        w_u32(root + 12, result);
        packet += 16;
    }
    return result;
}

uint32 sub_8008f1b8(void)
{
    uint32 scene, descriptor, count, density, value, result, a, b, c, x, y, z, origin, packet, random, depth, fade, size, start, tail, ordering, address, lane, shift, i;
    sint32 signed_value;
    FUNCTION_MARKER(0x8008F1B8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    descriptor = r_u32(scene + 616);
    count = r_u8(descriptor + 29);
    density = r_u32(descriptor + 28);
    result = scene;
    if (count == 0)
        return result;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x2E838383);
    density = (sint32)density >> 16;
    xport_gte_write_data(8, density);
    result = (sint32)density < 4096;
    if (result == 0)
        return result;
    xport_gte_execute(0x780010);
    value = r_u32(0x1F80000C);
    y = r_s16(descriptor + 8);
    y -= (uint32)((sint32)value >> 12);
    density = xport_gte_read_data(22);
    result = (sint32)y < 4096;
    if (result == 0)
        return result;
    xport_gte_write_data(6, density);
    xport_gte_write_data(8, y);
    if ((sint32)y > 0)
    {
        xport_gte_execute(0x780010);
        value = count * y;
        if ((sint32)value < 0)
            value += 4095;
        result = (sint32)value >> 12;
        count -= result;
        y = xport_gte_read_data(22);
        if ((sint32)count <= 0)
            return result;
        xport_gte_write_data(6, y);
    }
    xport_gte_write_control(21, 256);
    xport_gte_write_control(22, 256);
    xport_gte_write_control(23, 256);
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
    scene = r_u32(0x800BCE54);
    a = r_u32(scene + 44);
    b = r_u32(scene + 48);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(scene + 52);
    b = r_u32(scene + 56);
    c = r_u32(scene + 60);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x3800);
    xport_gte_execute(0x486012);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F80000C);
    x = (sint32)(0 - a) >> 8;
    y = (sint32)(0 - b) >> 8;
    b = r_u32(0x1F800010);
    a = r_u32(0x800BCE60);
    b = 0 - b;
    a = r_u32(a + 40);
    z = (sint32)b >> 8;
    y += a << 7;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
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
    xport_gte_write_control(7, 0x3800);
    packet = r_u32(0x1F800004);
    w_u16(0x1F80003C, (x & 0x3FFF) - 0x2000);
    w_u16(0x1F80003E, (y & 0x3FFF) - 0x2000);
    origin = r_u32(0x800BCF90);
    result = (z & 0x3FFF) - 0x2000;
    w_u16(0x1F800040, result);
    while ((sint32)count > 0)
    {
        value = r_u32(0x1F80003C);
        xport_gte_write_data(0, value);
        value = r_u32(0x1F800040);
        xport_gte_write_data(1, value);
        random = r_u32(origin);
        origin += 4;
        xport_gte_execute(0x180001);
        a = x + random;
        random = (sint32)random >> 9;
        b = y + random;
        random = (sint32)random >> 9;
        w_u16(0x1F80003C, (a & 0x3FFF) - 0x2000);
        value = z + random;
        w_u16(0x1F800040, (value & 0x3FFF) - 0x2000);
        w_u16(0x1F80003E, (b & 0x3FFF) - 0x2000);
        w_u16(packet + 28, 0x1FF0);
        w_u32(packet + 12, 0x461610F0);
        w_u32(packet + 20, 0x003810FF);
        w_u16(packet + 36, 0x1FFF);
        depth = xport_gte_read_data(19);
        value = xport_gte_read_data(14);
        w_u32(0x1F80008C, value);
        value = depth - 7168;
        fade = (sint32)((sint32)value < 0 ? 7168 - depth : value) >> 2;
        if ((sint32)fade >= 4097)
            fade = 4096;
        xport_gte_write_data(8, fade);
        size = (sint32)(23686 - depth) >> 12;
        xport_gte_execute(0x780010);
        if ((sint32)size < 2)
            size = 2;
        signed_value = r_s16(0x1F80008C);
        if (signed_value < 0)
            signed_value = -signed_value;
        result = (sint32)((uint32)signed_value - size) < 161;
        if (result != 0)
        {
            signed_value = r_s16(0x1F80008E);
            if (signed_value < 0)
                signed_value = -signed_value;
            result = (sint32)((uint32)signed_value - size) < 121;
            if (result != 0)
            {
                w_u32(packet, ((packet + 40) & 0xFFFFFF) | 0x09000000);
                value = r_s16(0x1F80008C);
                a = value - (uint32)((sint32)size >> 1);
                value = a + size;
                w_u16(packet + 24, a);
                w_u16(packet + 8, a);
                w_u16(packet + 32, value);
                w_u16(packet + 16, value);
                value = r_s16(0x1F80008E);
                a = value - (uint32)((sint32)size >> 1);
                result = a + size;
                w_u16(packet + 18, a);
                w_u16(packet + 10, a);
                w_u16(packet + 34, result);
                w_u16(packet + 26, result);
                value = xport_gte_read_data(22);
                w_u32(packet + 4, value);
                packet += 40;
            }
        }
        --count;
    }
    start = r_u32(0x1F800004);
    tail = packet - 40;
    if (start != packet)
    {
        ordering = r_u32(0x1F800000);
        result = ordering + 112;
        /* DMA packets and ordering heads are word aligned; unused LWL bits do not reach SWL */
        address = result + 2;
        lane = address & 3;
        value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
        address = tail + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        value = start << 8;
        address = result + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        w_u32(0x1F800004, packet);
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
    xport_gte_write_control(7, 0);
    return result;
}

uint32 sub_8008f580(void)
{
    uint32 a, b, c, scene, origin, packet, value, x, y, z, descriptor, count, result, random, old_x, old_z, extent_x, extent_z, depth, fade, uv, ordering, slot;
    sint32 absolute;
    FUNCTION_MARKER(0x8008F580, "ACE3.BIN");
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
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
    scene = r_u32(0x800BCE54);
    a = r_u32(scene + 44);
    b = r_u32(scene + 48);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(scene + 52);
    b = r_u32(scene + 56);
    c = r_u32(scene + 60);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x2400);
    origin = r_u32(0x800BCF90);
    packet = r_u32(0x1F800004);
    xport_gte_execute(0x486012);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F800010);
    x = (sint32)(0 - a) >> 12;
    z = (sint32)(0 - b) >> 12;
    a = r_u32(0x1F80000C);
    descriptor = r_u32(scene + 616);
    a = 0 - a;
    count = r_u8(descriptor + 28);
    y = (sint32)a >> 12;
    if ((sint32)count <= 0)
        count = 1;
    value = xport_gte_read_data(25);
    x -= value;
    a = xport_gte_read_data(26);
    value = xport_gte_read_data(27);
    z -= value;
    y -= a;
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
    xport_gte_write_control(7, 0x2400);
    w_u16(0x1F80003C, (x & 0x3FFF) - 0x2000);
    w_u16(0x1F80004E, y);
    w_u16(0x1F800046, y);
    w_u16(0x1F80003E, y);
    result = (z & 0x3FFF) - 0x2000;
    w_u16(0x1F800040, result);
    uv = packet + 22;
    do
    {
        value = r_u32(0x1F80003C);
        xport_gte_write_data(0, value);
        value = r_u32(0x1F800040);
        xport_gte_write_data(1, value);
        random = r_u32(origin);
        origin += 4;
        xport_gte_execute(0x180001);
        value = ((x + random) & 0x3FFF) - 0x2000;
        extent_z = ((uint32)((sint32)random >> 16) & 255) + 800;
        old_x = r_u16(0x1F80003C);
        old_z = r_u16(0x1F800040);
        w_u16(0x1F80003C, value);
        extent_x = old_x + (random & 255) + 800;
        random = (sint32)random >> 9;
        w_u16(0x1F800050, old_z + extent_z);
        result = ((z + random) & 0x3FFF) - 0x2000;
        w_u16(0x1F800044, old_x);
        w_u16(0x1F800048, old_z);
        w_u16(0x1F80004C, extent_x);
        w_u16(0x1F800040, result);
        depth = xport_gte_read_data(19);
        value = xport_gte_read_data(14);
        w_u32(0x1F80008C, value);
        if ((sint32)depth <= 0)
            goto next_effect_8f580;
        absolute = r_s16(0x1F80008E);
        if (absolute < 0)
            absolute = -absolute;
        result = absolute < 161;
        if (result == 0)
            goto next_effect_8f580;
        absolute = r_s16(0x1F80008C);
        if (absolute < 0)
            absolute = -absolute;
        result = (sint32)depth < 2192;
        if (absolute >= 251)
            goto next_effect_8f580;
        fade = (sint32)depth < 2192 ? (2192 - depth) << 1 : (uint32)((sint32)(depth - 4415) >> 2);
        result = (sint32)fade < 4097;
        if (result == 0)
            goto next_effect_8f580;
        value = r_u32(0x1F800050);
        xport_gte_write_data(1, value);
        value = r_u32(0x1F80004C);
        xport_gte_write_data(2, value);
        value = r_u32(0x1F800048);
        xport_gte_write_data(3, value);
        value = r_u32(0x1F80004C);
        xport_gte_write_data(4, value);
        value = r_u32(0x1F800050);
        xport_gte_write_data(5, value);
        value = r_u32(0x1F80008C);
        w_u32(uv - 14, value);
        xport_gte_execute(0x280030);
        if ((sint32)fade <= 0)
            w_u32(uv - 18, 0x2E838383);
        if ((random & 1) != 0)
        {
            w_u8(uv + 6, 0);
            w_u8(uv - 10, 0);
            w_u8(uv + 14, 63);
            w_u8(uv - 2, 63);
        }
        else
        {
            w_u8(uv + 6, 63);
            w_u8(uv - 10, 63);
            w_u8(uv + 14, 0);
            w_u8(uv - 2, 0);
        }
        if ((random & 2) != 0)
        {
            w_u8(uv - 1, 128);
            w_u8(uv - 9, 128);
            w_u8(uv + 15, 191);
            w_u8(uv + 7, 191);
        }
        else
        {
            w_u8(uv - 1, 191);
            w_u8(uv - 9, 191);
            w_u8(uv + 15, 128);
            w_u8(uv + 7, 128);
        }
        slot = (sint32)depth >> 2;
        w_u16(uv - 8, 0x4ED6);
        w_u16(uv, 57);
        if ((sint32)slot >= 4095)
            slot = 4094;
        result = xport_gte_read_data(19);
        slot <<= 2;
        if ((sint32)result <= 0)
            goto next_effect_8f580;
        ordering = r_u32(0x1F800000);
        value = r_u32(ordering + slot);
        w_u32(packet, value | 0x09000000);
        ordering = r_u32(0x1F800000);
        result = packet & 0xFFFFFF;
        w_u32(ordering + slot, result);
        value = xport_gte_read_data(12);
        w_u32(packet + 24, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 32, value);
        if ((sint32)fade > 0)
        {
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            value = xport_gte_read_data(22);
            w_u32(packet + 4, value);
        }
        uv += 40;
        packet += 40;
    next_effect_8f580:
        --count;
    } while ((sint32)count > 0);
    w_u32(0x1F800004, packet);
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
    xport_gte_write_control(7, 0);
    return result;
}

uint32 sub_8008eb58(void)
{
    uint32 scene, descriptor, packed, count, density, camera_y, height, value, result, a, b, c, x, y, z, packet, color_packet, origin, random, depth, fade, group, vertex, camera, scratch, ordering, start, tail, address, lane, shift, i;
    FUNCTION_MARKER(0x8008EB58, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    descriptor = r_u32(scene + 616);
    packed = r_u32(descriptor + 12);
    count = r_u8(descriptor + 29);
    density = r_u32(descriptor + 28);
    result = descriptor;
    if (count == 0)
        return result;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, 0x5225211D);
    density = (sint32)density >> 16;
    xport_gte_write_data(8, density);
    result = (sint32)density < 4096;
    if (result == 0)
        return result;
    xport_gte_execute(0x780010);
    value = r_u32(0x800BCEB0);
    camera_y = r_u32(0x1F80000C);
    value = r_u32(value + 4);
    height = camera_y + value;
    if (count >= 86)
        count = 85;
    if ((sint32)height < 0)
        height = (uint32)(sint32)(sint16)packed - (((uint32)((sint32)packed >> 17) & 127) * (packed >> 24)) - ((uint32)((sint32)camera_y >> 12) + 2048);
    else
        height = (sint32)height >> 8;
    density = xport_gte_read_data(22);
    result = (sint32)height < 4096;
    if (result == 0)
        return result;
    xport_gte_write_data(8, height);
    xport_gte_write_data(6, density);
    if ((sint32)height > 0)
    {
        xport_gte_execute(0x780010);
        value = count * height;
        if ((sint32)value < 0)
            value += 4095;
        result = (sint32)value >> 12;
        count -= result;
        height = xport_gte_read_data(22);
        if ((sint32)count <= 0)
            return result;
        xport_gte_write_data(6, height);
    }
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
    scene = r_u32(0x800BCE54);
    a = r_u32(scene + 44);
    b = r_u32(scene + 48);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(scene + 52);
    b = r_u32(scene + 56);
    c = r_u32(scene + 60);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x2000);
    w_u32(0x1F800090, count);
    xport_gte_execute(0x486012);
    a = r_u32(0x1F800008);
    b = r_u32(0x1F80000C);
    x = (sint32)(0 - a) >> 8;
    y = (sint32)(0 - b) >> 8;
    b = r_u32(0x1F800010);
    a = r_u32(0x800BCE60);
    b = 0 - b;
    a = r_u32(a + 40);
    z = (sint32)b >> 8;
    y += a << 7;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
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
    xport_gte_write_control(7, 0x2000);
    packet = r_u32(0x1F800004);
    w_u16(0x1F80003C, (x & 0x1FFF) - 4096);
    w_u16(0x1F80003E, (y & 0x1FFF) - 4096);
    origin = r_u32(0x800BCF90);
    w_u16(0x1F800040, (z & 0x1FFF) - 4096);
    for (group = 0; group < count; ++group)
    {
        color_packet = packet;
        for (vertex = 0; vertex < 3; ++vertex)
        {
            value = r_u32(0x1F80003C);
            xport_gte_write_data(0, value);
            value = r_u32(0x1F800040);
            xport_gte_write_data(1, value);
            random = r_u32(origin);
            origin += 4;
            xport_gte_execute(0x180001);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            w_u16(0x1F80003C, (a & 0x1FFF) - 4096);
            value = z + random;
            w_u16(0x1F80003E, (b & 0x1FFF) - 4096);
            w_u16(0x1F800040, (value & 0x1FFF) - 4096);
            depth = xport_gte_read_data(19);
            value = xport_gte_read_data(14);
            fade = depth - 8192;
            w_u32(color_packet + 8, value);
            if ((sint32)fade < 0)
                fade = 8192 - depth;
            if ((sint32)fade >= 4097)
                fade = 4096;
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            w_u32(color_packet + 12, 0x30303);
            value = xport_gte_read_data(22);
            w_u32(color_packet + 4, value);
            color_packet += 16;
        }
        packet += 52;
    }
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800014);
    b = r_u16(camera + 172);
    w_u16(0x1F80003C, a - b);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800016);
    b = r_u16(camera + 174);
    w_u16(0x1F80003E, a - b);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800018);
    b = r_u16(camera + 176);
    w_u16(0x1F800040, a - b);
    sub_800308f0(0x1F80003C, 0x1F80005C, 0, 0);
    a = r_u32(0x1F80005C);
    b = r_u32(0x1F800060);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(0x1F800064);
    b = r_u32(0x1F800068);
    c = r_u32(0x1F80006C);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 156);
    w_u16(0x1F80003C, (sint32)value >> 8);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 160);
    value = (sint32)value >> 8;
    w_u16(0x1F80003E, value);
    camera = r_u32(scene + 40);
    b = r_u32(camera + 164);
    w_u16(0x1F80003E, value - 128);
    w_u16(0x1F800040, (sint32)b >> 8);
    value = r_u32(0x1F80003C);
    xport_gte_write_data(0, value);
    value = r_u32(0x1F800040);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    value = xport_gte_read_data(25);
    xport_gte_write_control(5, value);
    value = xport_gte_read_data(26);
    xport_gte_write_control(6, value);
    value = xport_gte_read_data(27);
    xport_gte_write_control(7, value + 8192);
    packet = r_u32(0x1F800004);
    w_u16(0x1F80003C, (x & 0x1FFF) - 4096);
    w_u16(0x1F80003E, (y & 0x1FFF) - 4096);
    origin = r_u32(0x800BCF90);
    count = r_u32(0x1F800090);
    w_u16(0x1F800040, (z & 0x1FFF) - 4096);
    for (vertex = 0; vertex < 2; ++vertex)
    {
        random = r_u32(origin);
        origin += 4;
        a = x + random;
        random = (sint32)random >> 8;
        b = y + random;
        random = (sint32)random >> 8;
        scratch = 0x1F800044 + vertex * 8;
        w_u16(scratch, (a & 0x1FFF) - 4096);
        value = z + random;
        w_u16(scratch + 2, (b & 0x1FFF) - 4096);
        w_u16(scratch + 4, (value & 0x1FFF) - 4096);
    }
    for (group = 0; (sint32)group < (sint32)count; ++group)
    {
        for (vertex = 0; vertex < 6; ++vertex)
        {
            value = r_u32(0x1F80003C + vertex * 4);
            xport_gte_write_data(vertex, value);
        }
        for (vertex = 0; vertex < 3; ++vertex)
        {
            random = r_u32(origin);
            origin += 4;
            if (vertex == 0)
                xport_gte_execute(0x280030);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            scratch = 0x1F80003C + vertex * 8;
            w_u16(scratch, (a & 0x1FFF) - 4096);
            value = z + random;
            if (vertex == 2)
            {
                w_u16(scratch + 4, (value & 0x1FFF) - 4096);
                w_u16(scratch + 2, (b & 0x1FFF) - 4096);
            }
            else
            {
                w_u16(scratch + 2, (b & 0x1FFF) - 4096);
                w_u16(scratch + 4, (value & 0x1FFF) - 4096);
            }
        }
        w_u32(packet, ((packet + 52) & 0xFFFFFF) | 0x0C000000);
        value = xport_gte_read_data(12);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 32, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 48, value);
        packet += 52;
    }
    tail = packet - 52;
    ordering = r_u32(0x1F800000);
    start = r_u32(0x1F800004);
    ordering += 112;
    /* DMA packets and ordering heads are word aligned; unused LWL bits do not reach SWL */
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
    result = start << 8;
    address = tail + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, value >> (shift + (i << 3)));
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, result >> (shift + (i << 3)));
    w_u32(0x1F800004, packet);
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
    return result;
}

uint32 sub_8008e4f8(void)
{
    uint32 scene, height, camera_y, value, result, a, b, c, x, y, z, packet, color_packet, origin, random, depth, fade, group, vertex, camera, scratch, ordering, start, tail, address, lane, shift, i;
    FUNCTION_MARKER(0x8008E4F8, "ACE3.BIN");
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
    scene = r_u32(0x800BCE54);
    a = r_u32(scene + 44);
    b = r_u32(scene + 48);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(scene + 52);
    b = r_u32(scene + 56);
    c = r_u32(scene + 60);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 8192);
    value = r_u32(0x1F800008);
    x = (sint32)(0 - value) >> 10;
    xport_gte_execute(0x486012);
    camera_y = r_u32(0x1F80000C);
    value = r_u32(0x800BCEB0);
    y = (sint32)(0 - camera_y) >> 10;
    a = r_u32(0x1F800010);
    value = r_u32(value + 4);
    a = 0 - a;
    height = (sint32)(camera_y + value) >> 14;
    z = (sint32)a >> 10;
    if ((sint32)height < 0)
        height = 0;
    else if ((sint32)height >= 4097)
        height = 4096;
    value = xport_gte_read_data(25);
    x -= value;
    value = xport_gte_read_data(26);
    y -= value;
    value = xport_gte_read_data(27);
    z -= value;
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
    xport_gte_write_control(7, 8192);
    if ((sint32)height > 0)
    {
        xport_gte_write_control(21, 48);
        xport_gte_write_control(22, 0);
        xport_gte_write_control(23, 0);
        xport_gte_write_data(6, 0x6203135F);
        fade = 4224 - (height << 1);
        if ((sint32)fade < 0)
            fade = 0;
        xport_gte_write_data(8, fade);
        packet = r_u32(0x1F800004);
        w_u32(0x1F800004, packet + 16);
        xport_gte_execute(0x780010);
        ordering = r_u32(0x1F800000);
        value = r_u32(ordering + 4);
        w_u32(packet, value | 0x03000000);
        value = packet & 0xFFFFFF;
        ordering = r_u32(0x1F800000);
        w_u32(ordering + 4, value);
        w_u32(packet + 8, 0xFF88FF60);
        w_u32(packet + 12, 0x00F00140);
        value = xport_gte_read_data(22);
        w_u32(packet + 4, value);
    }
    xport_gte_write_data(6, 0x52534333);
    xport_gte_write_control(21, 0xC30);
    xport_gte_write_control(22, 0xB30);
    xport_gte_write_control(23, 0x1F0);
    xport_gte_write_data(8, height);
    packet = r_u32(0x1F800004);
    origin = r_u32(0x800BCF90);
    xport_gte_execute(0x780010);
    w_u16(0x1F80003C, (x & 0x1FFF) - 4096);
    w_u16(0x1F80003E, (y & 0x1FFF) - 4096);
    w_u16(0x1F800040, (z & 0x1FFF) - 4096);
    value = xport_gte_read_data(22);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, value);
    for (group = 0; group < 64; ++group)
    {
        color_packet = packet;
        for (vertex = 0; vertex < 3; ++vertex)
        {
            value = r_u32(0x1F80003C);
            xport_gte_write_data(0, value);
            value = r_u32(0x1F800040);
            xport_gte_write_data(1, value);
            random = r_u32(origin);
            origin += 4;
            xport_gte_execute(0x180001);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            w_u16(0x1F80003C, (a & 0x1FFF) - 4096);
            value = z + random;
            w_u16(0x1F80003E, (b & 0x1FFF) - 4096);
            w_u16(0x1F800040, (value & 0x1FFF) - 4096);
            depth = xport_gte_read_data(19);
            value = xport_gte_read_data(14);
            fade = depth - 8192;
            w_u32(color_packet + 8, value);
            if ((sint32)fade < 0)
                fade = 8192 - depth;
            if ((sint32)fade >= 4097)
                fade = 4096;
            xport_gte_write_data(8, fade);
            xport_gte_execute(0x780010);
            w_u32(color_packet + 12, 0x30303);
            value = xport_gte_read_data(22);
            w_u32(color_packet + 4, value);
            color_packet += 16;
        }
        packet += 52;
    }
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800014);
    b = r_u16(camera + 172);
    w_u16(0x1F80003C, a - b);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800016);
    b = r_u16(camera + 174);
    w_u16(0x1F80003E, a - b);
    camera = r_u32(scene + 40);
    a = r_u16(0x1F800018);
    b = r_u16(camera + 176);
    w_u16(0x1F800040, a - b);
    sub_800308f0(0x1F80003C, 0x1F80005C, 0, 0);
    a = r_u32(0x1F80005C);
    b = r_u32(0x1F800060);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(0x1F800064);
    b = r_u32(0x1F800068);
    c = r_u32(0x1F80006C);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 156);
    w_u16(0x1F80003C, (sint32)value >> 9);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 160);
    value = (sint32)value >> 9;
    w_u16(0x1F80003E, value);
    camera = r_u32(scene + 40);
    b = r_u32(camera + 164);
    w_u16(0x1F800040, (sint32)b >> 9);
    value = r_u32(0x1F80003C);
    xport_gte_write_data(0, value);
    value = r_u32(0x1F800040);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    value = xport_gte_read_data(25);
    xport_gte_write_control(5, value);
    value = xport_gte_read_data(26);
    xport_gte_write_control(6, value);
    value = xport_gte_read_data(27);
    xport_gte_write_control(7, value + 8192);
    packet = r_u32(0x1F800004);
    w_u16(0x1F80003C, (x & 0x1FFF) - 4096);
    w_u16(0x1F80003E, (y & 0x1FFF) - 4096);
    origin = r_u32(0x800BCF90);
    w_u16(0x1F800040, (z & 0x1FFF) - 4096);
    for (vertex = 0; vertex < 2; ++vertex)
    {
        random = r_u32(origin);
        origin += 4;
        a = x + random;
        random = (sint32)random >> 8;
        b = y + random;
        random = (sint32)random >> 8;
        scratch = 0x1F800044 + vertex * 8;
        w_u16(scratch, (a & 0x1FFF) - 4096);
        value = z + random;
        w_u16(scratch + 2, (b & 0x1FFF) - 4096);
        w_u16(scratch + 4, (value & 0x1FFF) - 4096);
    }
    for (group = 0; group < 64; ++group)
    {
        for (vertex = 0; vertex < 6; ++vertex)
        {
            value = r_u32(0x1F80003C + vertex * 4);
            xport_gte_write_data(vertex, value);
        }
        for (vertex = 0; vertex < 3; ++vertex)
        {
            random = r_u32(origin);
            origin += 4;
            if (vertex == 0)
                xport_gte_execute(0x280030);
            a = x + random;
            random = (sint32)random >> 8;
            b = y + random;
            random = (sint32)random >> 8;
            scratch = 0x1F80003C + vertex * 8;
            w_u16(scratch, (a & 0x1FFF) - 4096);
            value = z + random;
            if (vertex == 2)
            {
                w_u16(scratch + 4, (value & 0x1FFF) - 4096);
                w_u16(scratch + 2, (b & 0x1FFF) - 4096);
            }
            else
            {
                w_u16(scratch + 2, (b & 0x1FFF) - 4096);
                w_u16(scratch + 4, (value & 0x1FFF) - 4096);
            }
        }
        w_u32(packet, ((packet + 52) & 0xFFFFFF) | 0x0C000000);
        value = xport_gte_read_data(12);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 32, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 48, value);
        packet += 52;
    }
    start = r_u32(0x1F800004);
    tail = packet - 52;
    result = 0;
    if (start != packet)
    {
        ordering = r_u32(0x1F800000);
        result = ordering + 16376;
        /* DMA packets and ordering heads are word aligned; unused LWL bits do not reach SWL */
        address = result + 2;
        lane = address & 3;
        value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
        address = tail + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        value = start << 8;
        address = result + 2;
        lane = address & 3;
        shift = 24 - (lane << 3);
        address &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(address + i, value >> (shift + (i << 3)));
        w_u32(0x1F800004, packet);
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
    return result;
}

uint32 sub_80085204(uint32 ot, uint32 packet)
{
    uint32 scene, camera, value, data, root, state, record, flags, distance, color, mission, selector, target;
    sint32 count;
    FUNCTION_MARKER(0x80085204, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800000, ot);
    w_u32(0x1F800004, packet);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 4);
    w_u32(0x1F800008, value);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 8);
    w_u32(0x1F80000C, value);
    camera = r_u32(scene + 40);
    value = r_u32(camera + 12);
    w_u32(0x1F800010, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 20);
    w_u16(0x1F800014, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 22);
    w_u16(0x1F800016, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 24);
    w_u16(0x1F800018, value);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    w_u16(0x1F80001A, value);
    camera = r_u32(scene + 40);
    flags = r_u32(camera + 80) & 0x870;
    if (flags == 0)
    {
        data = r_u32(0x800BCE60);
        flags = r_u32(data) & 0x20000000;
    }
    if (flags != 0)
    {
        root = r_u32(0x800BCF90);
        value = r_u32(root + 1480);
        w_u32(root + 1480, value | 1);
    }
    scene = r_u32(0x800BCE54);
    record = r_u32(scene + 580);
    count = r_s16(record + 108);
    if (count != 0)
    {
        root = r_u32(0x800BCF90);
        data = r_u32(0x800BCEB0);
        state = root + 1480;
        distance = r_u32(data + 4);
        sub_8008ba8c(state, count, 0 - distance);
        scene = r_u32(0x800BCE54);
        record = r_u32(scene + 580);
        color = r_u32(record + 112);
        if (color != 0)
        {
            data = r_u32(0x800BCE60);
            mission = r_s16(data + 910);
            if (mission == 25)
            {
                camera = r_u32(scene + 40);
                flags = r_u32(camera + 80);
                if (flags == 2)
                {
                    value = r_u32(root + 1480);
                    w_u32(root + 1480, value | 4);
                }
                else
                    sub_8008b310(state, color);
            }
            else
                sub_8008b310(state, color);
        }
    }
    scene = r_u32(0x800BCE54);
    record = r_u32(scene + 580);
    value = r_u8(record + 110);
    if (value != 0)
        sub_8008d9f4();
    scene = r_u32(0x800BCE54);
    record = r_u32(scene + 580);
    selector = r_u8(record + 111);
    if (selector > 0 && selector < 6)
    {
        target = r_u32(0x800B4388 + (selector - 1) * 4);
        /* All five targets are argumentless; bridge padding carries no guest state */
        xport_guest_call3(target, 0, selector, 0);
    }
    root = r_u32(0x800BCF90);
    sub_800854b8(root + 1536);
    root = r_u32(0x800BCF90);
    sub_8008700c(root + 4248);
    root = r_u32(0x800BCF90);
    sub_8008b048(root + 13248);
    root = r_u32(0x800BCF90);
    sub_8009044c(root + 14664);
    root = r_u32(0x800BCF90);
    sub_80092850(root + 14692);
    data = r_u32(0x800BCE60);
    value = r_u8(data + 933);
    if (value == 0)
    {
        scene = r_u32(0x800BCE54);
        camera = r_u32(scene + 40);
        value = r_u32(camera + 80) & 0x200;
    }
    if (value != 0)
        sub_8008ffac(0x1F800000);
    return r_u32(0x1F800004);
}

uint32 sub_8002bd60(uint32 audio_state)
{
    uint32 volume, mono;
    FUNCTION_MARKER(0x8002BD60, "ACE3.BIN");
    volume = r_u32(audio_state + 356);
    mono = r_u32(audio_state + 352);
    return sub_800299d8(volume, mono);
}

uint32 sub_8002bd5c(void)
{
    uint32 audio_state;
    FUNCTION_MARKER(0x8002BD5C, "ACE3.BIN");
    audio_state = r_u32(0x800BCBBC);
    return sub_8002bd60(audio_state);
}

uint32 sub_800a8ef8(uint32 driver_table)
{
    FUNCTION_MARKER(0x800A8EF8, "ACE3.BIN");
    (void)driver_table;
    return (uint32)GetODE();
}

uint32 sub_800a8ef0(void)
{
    uint32 driver_table;
    FUNCTION_MARKER(0x800A8EF0, "ACE3.BIN");
    driver_table = r_u32(0x800BB95C);
    return sub_800a8ef8(driver_table);
}

void sub_80027064(void)
{
    FUNCTION_MARKER(0x80027064, "ACE3.BIN");
    PadStopCom();
}

uint32 sub_80025fe8(void)
{
    FUNCTION_MARKER(0x80025FE8, "ACE3.BIN");
    DrawSync(0);
    VSyncCallback(NULL);
    CdFlush();
    sub_80027064();
    StopCARD();
    ResetGraph(3);
    StopCallback();
#ifndef NDEBUG
    return printf("      bye bye..\n");
#else
    return 0;
#endif
}

uint32 sub_800274e0(uint32 bytes)
{
    uint32 state, released, limit, cursor, candidate;
    FUNCTION_MARKER(0x800274E0, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    released = (bytes >> 2) << 2;
    cursor = r_u32(state + 28);
    limit = r_u32(state + 24);
    candidate = cursor + released;
    if (candidate >= limit)
        return r_u32(state + 28);
#ifndef NDEBUG
    printf("Error: MemFree: Underflow %d bytes (REQ: %d bytes)\n", (sint32)(limit - candidate), (sint32)(released));
#endif
    return 0;
}

uint32 sub_8002963c(void)
{
    uint32 record;
    FUNCTION_MARKER(0x8002963C, "ACE3.BIN");
    record = sub_8002939c(250);
    if (record == 0)
        return 60;
    w_u8(record, 60);
    w_u8(record + 2, 255);
    return 255;
}

uint32 sub_80029674(void)
{
    uint32 record, seed, result;
    FUNCTION_MARKER(0x80029674, "ACE3.BIN");
    record = sub_8002939c(160);
    if (record == 0)
        return 1;
    w_u8(record, 1);
    seed = r_u32(0x800CBF58);
    seed = seed * 0x41C64E6D + 12345;
    w_u32(0x800CBF58, seed);
    result = ((seed >> 16) & 63) + 65;
    w_u8(record + 2, result);
    return result;
}

uint32 sub_800296b8(void)
{
    uint32 record;
    FUNCTION_MARKER(0x800296B8, "ACE3.BIN");
    record = sub_8002939c(180);
    if (record == 0)
        return 9;
    w_u8(record, 9);
    w_u8(record + 2, 255);
    return 255;
}

uint32 sub_800296f0(void)
{
    uint32 record, seed, result;
    FUNCTION_MARKER(0x800296F0, "ACE3.BIN");
    record = sub_8002939c(161);
    if (record == 0)
        return 1;
    w_u8(record, 1);
    seed = r_u32(0x800CBF58);
    seed = seed * 0x41C64E6D + 12345;
    w_u32(0x800CBF58, seed);
    result = ((seed >> 16) & 63) + 97;
    w_u8(record + 2, result);
    return result;
}

uint32 sub_8002b428(void)
{
    uint32 state, result;
    FUNCTION_MARKER(0x8002B428, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    result = r_u32(state);
    if (result != 0)
        return result;
    resource_stop_cd(1);
    state = r_u32(0x800BCBA8);
    w_u32(state, 2);
    w_u32(state + 4, 0);
    return 2;
}

uint32 sub_8002b470(uint32 unused0, uint32 unused1, uint32 state)
{
    uint32 value, phase, root;
    FUNCTION_MARKER(0x8002B470, "ACE3.BIN");
    (void)unused0;
    (void)unused1;
    value = r_u8(state + 40);
    if ((value & 16) != 0)
        return 0;
    value = r_u32(state + 12);
    if (value - 1 >= 2)
        return 0;
    phase = r_u32(state);
    if (phase != 2)
        return 0;
    value = r_u32(state + 4);
    if (value != phase)
        return 0;
    if (CdControl(7, NULL, (uint8 *)psx_addr(state + 40, 8)) == 0)
        return 0;
    root = r_u32(0x800BCBA8);
    w_u32(root, 0);
    w_u32(root + 4, 0);
    return 1;
}

uint32 sub_8002bca4(void)
{
    uint32 state, flags;
    FUNCTION_MARKER(0x8002BCA4, "ACE3.BIN");
    state = r_u32(0x800BCBAC);
    flags = r_u32(state + 32);
    if (flags != 64)
        return 0;
    state = r_u32(0x800BCBB4);
    return r_u32(state + 52);
}

uint32 sub_8002cba4(uint32 id)
{
    uint32 entry;
    FUNCTION_MARKER(0x8002CBA4, "ACE3.BIN");
    entry = resource_get_entry(id);
    return (r_u32(entry) >> 29) & 3;
}

uint32 sub_8002d6e8(void)
{
    FUNCTION_MARKER(0x8002D6E8, "ACE3.BIN");
    sub_8009fd48();
    if (SpuSetTransferStartAddr(0x1010) != 0x1010)
        return 0xFFFFFFFF;
    if (spu_clear_reverb_native(0x1010, 0x7EFF0) < 0)
        return 0xFFFFFFFF;
    return (0x7EFF0 + 63) & ~63u;
}

uint32 sub_8002f294(uint32 value, uint32 kind)
{
    uint32 index, entry, result;
    FUNCTION_MARKER(0x8002F294, "ACE3.BIN");
    index = r_u8(0x800BCE99);
    entry = 0x800C6570 + (index << 2);
    w_u16(entry + 2, value);
    w_u8(entry, kind);
    index = r_u8(0x800BCE99);
    result = (index + 1) & 31;
    w_u8(0x800BCE99, result);
    return result;
}

uint32 sub_8002f6e4(uint32 index, uint32 value)
{
    uint32 entry;
    FUNCTION_MARKER(0x8002F6E4, "ACE3.BIN");
    entry = 0x800C6630 + index * 92;
    w_u32(entry + 36, value);
    return entry;
}

uint32 sub_8002d10c(uint32 ot, uint32 packet)
{
    uint32 state, index, table, row, frame, divisor, quotient, remainder, height, v, a, b, y, tag, head;
    FUNCTION_MARKER(0x8002D10C, "ACE3.BIN");
    state = r_u32(0x800BCE88);
    index = r_u32(state);
    table = 0x800B03AC + index * 20;
    row = table;
    for (index = 0; index < 3; ++index, row += 2)
    {
        state = r_u32(0x800BCE88);
        frame = r_u8(state + index + 4);
        if (frame == 0)
            continue;
        divisor = r_u8(table + 2);
        --frame;
        if (divisor == 0)
            xport_mips_break(7);
        quotient = frame / divisor;
        remainder = frame % divisor;
        height = r_u8(table + 1);
        v = remainder * height;
        w_u8(packet + 3, 9);
        w_u8(packet + 7, 44);
        a = r_u8(table + 6);
        w_u8(packet + 7, a != 0 ? 46 : 44);
        a = r_u8(table + 3);
        w_u8(packet + 4, a);
        a = r_u8(table + 4);
        w_u8(packet + 5, a);
        a = r_u8(table + 5);
        w_u8(packet + 6, a);
        a = r_u8(table + 7);
        w_u16(packet + 8, a);
        a = r_u16(row + 8);
        w_u16(packet + 10, a);
        a = r_u8(table + 7);
        b = r_u8(table);
        w_u16(packet + 16, a + b);
        a = r_u16(row + 8);
        w_u16(packet + 18, a);
        a = r_u8(table + 7);
        w_u16(packet + 24, a);
        y = r_u16(row + 8);
        height = r_u8(table + 1);
        w_u16(packet + 26, y - 1 + (height << 1));
        a = r_u8(table + 7);
        b = r_u8(table);
        w_u16(packet + 32, a + b);
        y = r_u16(row + 8);
        height = r_u8(table + 1);
        w_u8(packet + 12, 0);
        w_u8(packet + 13, v);
        w_u16(packet + 34, y - 1 + (height << 1));
        a = r_u8(table);
        w_u8(packet + 21, v);
        w_u8(packet + 28, 0);
        w_u8(packet + 20, a);
        height = r_u8(table + 1);
        w_u8(packet + 29, height + v);
        a = r_u8(table);
        w_u8(packet + 36, a);
        height = r_u8(table + 1);
        w_u8(packet + 37, height + v);
        state = r_u32(0x800BCE88);
        w_u16(packet + 14, ((quotient & 7) << 6) + 0x4A16);
        a = r_u16(state + 28);
        tag = r_u32(packet);
        w_u16(packet + 22, a + (quotient >> 1));
        head = r_u32(ot);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 40;
    }
    return packet;
}

sint32 sub_8002b094(uint32 response)
{
    uint32 flags, state, phase;
    FUNCTION_MARKER(0x8002B094, "ACE3.BIN");
    flags = r_u8(response);
    if ((flags & 16) != 0)
        return 1;
    if (CdSync(1, (uint8 *)psx_addr(response, 8)) != 2)
        return 0;
    state = r_u32(0x800BCBA8);
    phase = r_u32(state + 4);
    if (phase == 0)
    {
        w_u8(0x800BCE70, 0);
        if (CdControlB(14, (uint8 *)psx_addr(0x800BCE70, 1), (uint8 *)psx_addr(response, 8)) != 0)
        {
            state = r_u32(0x800BCBA8);
            w_u32(state + 4, 1);
        }
    }
    else if (phase == 1)
    {
        if (CdControl(8, NULL, (uint8 *)psx_addr(response, 8)) != 0)
        {
            state = r_u32(0x800BCBA8);
            w_u32(state + 4, 2);
        }
    }
    else if (phase == 2)
        CdControlB(1, NULL, (uint8 *)psx_addr(response, 8));
    return 0;
}

sint32 sub_8002b194(uint32 response)
{
    uint32 flags, other, state, phase, clock, saved, now;
    FUNCTION_MARKER(0x8002B194, "ACE3.BIN");
    flags = r_u8(response);
    if ((flags & 1) != 0)
    {
        other = r_u8(response + 1);
        if ((other & 64) != 0)
            goto cd_error_2b194;
    }
    flags = r_u8(response);
    if ((flags & 4) != 0)
    {
        other = r_u8(response + 1);
        if ((other & 4) != 0)
            goto cd_error_2b194;
    }
    flags = r_u8(response);
    if ((flags & 16) != 0)
    {
        state = r_u32(0x800BCBA8);
        w_u32(state + 4, 0);
        w_u32(state + 12, 4);
    }
    if (CdSync(1, (uint8 *)psx_addr(response, 8)) != 2)
        return 0;
    state = r_u32(0x800BCBA8);
    phase = r_u32(state + 4);
    switch (phase)
    {
        case 0:
            flags = r_u8(response);
            if ((flags & 16) == 0)
            {
                state = r_u32(0x800BCBA8);
                clock = r_u32(state + 32);
                w_u32(state + 4, 1);
                w_u32(state + 12, 0);
                now = r_u32(clock);
                w_u32(state + 8, now);
            }
            CdControlB(1, NULL, (uint8 *)psx_addr(response, 8));
            return 0;
        case 1:
            state = r_u32(0x800BCBA8);
            clock = r_u32(state + 32);
            saved = r_u32(state + 8);
            now = r_u32(clock);
            if ((sint32)(saved + 180) < (sint32)now)
            {
                w_u32(state + 12, 4);
                return -2;
            }
            flags = r_u8(response);
            if ((flags & 2) != 0)
                w_u32(state + 4, 2);
            CdControlB(1, NULL, (uint8 *)psx_addr(response, 8));
            return 0;
        case 2:
            if (CdControlB(19, NULL, (uint8 *)psx_addr(response, 8)) != 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state + 4, 3);
            }
            return 0;
        case 3:
            w_u8(0x800BCE74, 0);
            w_u8(0x800BCE75, 2);
            w_u8(0x800BCE76, 0);
            w_u8(0x800BCE77, 0);
            if (CdControl(2, (uint8 *)psx_addr(0x800BCE74, 4), (uint8 *)psx_addr(response, 8)) != 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state + 4, 4);
            }
            return 0;
        case 4:
            if (CdControl(21, NULL, (uint8 *)psx_addr(response, 8)) != 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state + 4, 5);
            }
            return 0;
        case 5:
            if (CdControl(1, NULL, (uint8 *)psx_addr(response, 8)) != 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state + 4, 6);
            }
            return 0;
        case 6:
            w_u8(0x800BCE71, 128);
            if (CdControl(14, (uint8 *)psx_addr(0x800BCE71, 1), (uint8 *)psx_addr(response, 8)) != 0)
            {
                state = r_u32(0x800BCBA8);
                clock = r_u32(state + 32);
                w_u32(state + 4, 7);
                now = r_u32(clock);
                w_u32(state + 8, now);
            }
            return 0;
        case 7:
            state = r_u32(0x800BCBA8);
            clock = r_u32(state + 32);
            saved = r_u32(state + 8);
            now = r_u32(clock);
            return (sint32)(saved + 3) < (sint32)now;
        default:
            return 0;
    }
cd_error_2b194:
    state = r_u32(0x800BCBA8);
    w_u32(state + 12, 5);
    return -1;
}

uint32 sub_80030750(void)
{
    FUNCTION_MARKER(0x80030750, "ACE3.BIN");
    sound_stop_all();
    sub_8002f620();
    return 0;
}

uint32 sub_80038c78(void)
{
    uint32 state, flags, pointer, scene, camera, index, offset = 396, table, source;
    FUNCTION_MARKER(0x80038C78, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    flags = r_u32(state);
    w_u32(state, flags | 0x10000000);
    sub_8006c530();
    pointer = arena_peek(0);
    w_u32(0x800BCF5C, pointer);
    pointer = arena_alloc(2444);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    w_u32(0x800BCF58, pointer);
    copy_words(pointer, camera, 99);
    for (index = 0; index < 64; ++index)
    {
        pointer = r_u32(0x800BCF58);
        table = r_u32(0x800BCE5C);
        source = r_u32(table + (index << 2));
        copy_words(pointer + offset, source, 8);
        offset += 32;
    }
    return 0;
}

uint32 sub_80031ea4(uint32 index)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80031EA4, "ACE3.BIN");
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (index << 2) + 472);
    sub_80054ebc(script);
    return sub_80038c78();
}

uint32 sub_800798a8(void)
{
    uint32 scene, data, record, angles, x, y, camera;
    FUNCTION_MARKER(0x800798A8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    data = r_u32(0x800BCE50);
    record = r_u32(scene + 580);
    angles = r_u32(data + 12);
    x = r_s16(record + 12);
    y = r_s16(record + 14);
    angles += 4;
    camera = r_u32(scene + 40);
    return sub_80077880(camera, 296, x, y, angles, NULL);
}

uint32 sub_80031edc(void)
{
    FUNCTION_MARKER(0x80031EDC, "ACE3.BIN");
    return sub_800798a8();
}

uint32 sub_80031f24(uint32 index)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80031F24, "ACE3.BIN");
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (index << 2) + 568);
    sub_80054ebc(script);
    return sub_8005b654();
}

uint32 sub_80031f64(uint32 index)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80031F64, "ACE3.BIN");
    sub_800798a8();
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (index << 2) + 536);
    return sub_80054ebc(script);
}

uint32 sub_80031fd8(uint32 flags)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80031FD8, "ACE3.BIN");
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (flags << 2) + 504);
    return sub_80054ebc(script);
}

uint32 sub_80032010(uint32 flags)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80032010, "ACE3.BIN");
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (flags << 2) + 584);
    return sub_80054ebc(script);
}

uint32 sub_80032040(uint32 flags)
{
    uint32 state, script;
    FUNCTION_MARKER(0x80032040, "ACE3.BIN");
    state = r_u32(0x800BCEAC);
    script = r_u32(state + (flags << 2) + 552);
    return sub_80054ebc(script);
}

uint32 sub_800324ac(uint32 cursor)
{
    uint32 index, block, scene;
    FUNCTION_MARKER(0x800324AC, "ACE3.BIN");
    arena_set_cursor(cursor);
    for (index = 0; index < 2; ++index)
    {
        block = arena_alloc(0x4000);
        scene = r_u32(0x800BCE54);
        w_u32(scene + (index << 2) + 24, block);
        block = arena_alloc(0x1B800);
        scene = r_u32(0x800BCE54);
        w_u32(scene + (index << 2) + 32, block);
    }
    return 0;
}

uint32 sub_8003307c(void)
{
    uint32 state, result;
    FUNCTION_MARKER(0x8003307C, "ACE3.BIN");
    sub_800396e8();
    state = r_u32(0x800BCE60);
    result = ~r_u32(state) & 0x20000000;
    if (result == 0)
        return result;
    sub_8006cb10();
    return sub_8007dac8();
}

uint32 sub_800332a4(void)
{
    FUNCTION_MARKER(0x800332A4, "ACE3.BIN");
    return sub_80057bbc();
}

uint32 sub_800332c4(void)
{
    FUNCTION_MARKER(0x800332C4, "ACE3.BIN");
    return sub_8005b124();
}

uint32 sub_80034000(void)
{
    uint32 scene, flags;
    FUNCTION_MARKER(0x80034000, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800339e0(sub_8003307c, 0);
    return sub_80033dc0();
}

uint32 sub_8003423c(void)
{
    uint32 scene, flags;
    FUNCTION_MARKER(0x8003423C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800332a4();
    return sub_80033dc0();
}

uint32 sub_80034274(void)
{
    uint32 scene, flags;
    FUNCTION_MARKER(0x80034274, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800332c4();
    return sub_80033dc0();
}

void sub_8003329c(void)
{
    FUNCTION_MARKER(0x8003329C, "ACE3.BIN");
}

uint32 sub_80035044(void)
{
    uint32 scene, game, flags, slot, value, result, selected;
    FUNCTION_MARKER(0x80035044, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    game = r_u32(0x800BCE60);
    w_u32(scene, flags | 64);
    flags = r_u32(game + 304);
    slot = scene + 670;
    if ((flags & 1) != 0)
    {
        value = r_u16(slot);
        w_u16(slot, value - 1);
    }
    game = r_u32(0x800BCE60);
    flags = r_u32(game + 304);
    if ((flags & 2) != 0)
    {
        value = r_u16(slot);
        w_u16(slot, value + 1);
    }
    value = r_s16(slot);
    if ((sint32)value < 0)
    {
        w_u16(slot, 1);
        value = r_s16(slot);
    }
    if ((sint32)value >= 2)
        w_u16(slot, 0);
    game = r_u32(0x800BCE60);
    result = r_u32(game + 300) & 16;
    if (result == 0)
        return result;
    selected = r_s16(slot);
    if (selected != 0 && selected != 1)
        return 1;
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags | (selected == 0 ? 0x40000000 : 0x80000000));
    return scene;
}

uint32 sub_8003514c(void)
{
    uint32 scene, flags, game, pressed, result;
    FUNCTION_MARKER(0x8003514C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    game = r_u32(0x800BCE60);
    w_u32(scene, flags | 64);
    pressed = r_u32(game + 300) & 64;
    result = flags | 96;
    if (pressed != 0)
        w_u32(scene, result);
    return result;
}

uint32 sub_800353e8(uint32 object, uint32 packet)
{
    uint32 scene, ticks, value, color;
    FUNCTION_MARKER(0x800353E8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    ticks = r_u32(scene + 12);
    value = r_u16(0x800104AC + ((ticks << 9) & 0x7E00));
    value = (sint32)(value << 16) >> 18;
    if ((sint32)value < 0)
        value = 0 - value;
    color = ((uint32)((sint32)value >> 6)) << 8;
    return sub_8003563c(object, packet, 40, 212, color, 0x800235F4, 0);
}

uint32 sub_80034160(void)
{
    uint32 scene, game, flags, object, packet, result;
    FUNCTION_MARKER(0x80034160, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800339e0(sub_80033104, 0);
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene + 4);
    if ((flags & 0x1000) != 0)
    {
        sub_8003514c();
        game = r_u32(0x800BCE60);
        scene = r_u32(0x800BCE54);
        object = r_u32(game + 12);
        packet = r_u32(scene + 20);
        result = sub_800353e8(object, packet);
        scene = r_u32(0x800BCE54);
        game = r_u32(0x800BCE60);
        w_u32(scene + 20, result);
        flags = r_u32(game);
        w_u32(game, flags | 0x02000000);
        flags = r_u32(scene);
        if ((flags & 32) != 0)
        {
            w_u32(scene, flags & 0xFFFFFFDF);
            sound_stop_all();
            pad_reset_slots();
            scene = r_u32(0x800BCE54);
            object = r_u32(scene + 8);
            sub_80032f18(1024, object);
            sub_8006d078();
        }
    }
    return sub_80033dc0();
}

uint32 sub_800342ac(void)
{
    uint32 scene, flags;
    FUNCTION_MARKER(0x800342AC, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_800339e0(sub_800331e0, 0);
    return sub_80033dc0();
}

uint32 sub_800342ec(void)
{
    uint32 game, scene, flags, object, packet, result;
    FUNCTION_MARKER(0x800342EC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    scene = r_u32(0x800BCE54);
    w_u32(game, flags | 0x02000000);
    flags = r_u32(scene);
    w_u32(scene, flags & 0xFFFFFFFD);
    sub_80035044();
    sub_800339e0(0, sub_8003329c);
    game = r_u32(0x800BCE60);
    scene = r_u32(0x800BCE54);
    object = r_u32(game + 12);
    packet = r_u32(scene + 20);
    result = sub_80035184(object, packet);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 20, result);
    return sub_80033dc0();
}

uint32 sub_8006d078(void)
{
    uint32 table, object, flags, kind, data, value, game;
    FUNCTION_MARKER(0x8006D078, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    flags = r_u32(object);
    kind = r_s16(object + 72);
    w_u32(object, flags | 0x1C000);
    data = sub_8006cd6c(kind);
    value = r_u16(data + 132);
    game = r_u32(0x800BCE60);
    w_u16(object + 78, value);
    value = r_u32(game + 916);
    if ((sint32)value >= 2)
        w_u16(object + 78, 30);
    return 30;
}

uint32 sub_80057bbc(void)
{
    uint32 scene, phase, game, buffer, state, ordering, packet, result, context;
    FUNCTION_MARKER(0x80057BBC, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    phase = r_u32(scene + 4);
    if (phase == 0x4000)
        return sub_800573e8();
    if (phase == 0x10000)
        return sub_80057640();
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x800BD044);
    ordering = state + (buffer << 3) + 12;
    w_u32(state + 32, ordering);
    packet = r_u32(state + (buffer << 2) + 4);
    w_u32(state + 28, packet);
    ClearOTagR(psx_addr(ordering, 8), 2);
    if (sub_80057720() != 0)
    {
        scene = r_u32(0x800BCE54);
        context = r_u32(scene + 8);
        return sub_80032f18(0x10000, context);
    }
    context = r_u32(0x800BCD8C);
    if (r_u32(context + 4) != 0)
    {
        state = r_u32(0x800BD044);
        ordering = r_u32(state + 32);
        packet = r_u32(state + 28);
        result = sub_8002d3f4(ordering, packet);
        state = r_u32(0x800BD044);
        w_u32(state + 28, result);
    }
    state = r_u32(0x800BD044);
    ordering = r_u32(state + 32);
    DrawOTag(psx_addr(ordering + 4, 4));
    return 0;
}

uint32 sub_8005b124(void)
{
    uint32 scene, phase, game, buffer, state, ordering, packet, result, context;
    FUNCTION_MARKER(0x8005B124, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    phase = r_u32(scene + 4);
    if (phase == 0x20000)
        return sub_8005ae8c();
    if (phase == 0x80000)
        return sub_8005b234();
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x800BD050);
    ordering = state + buffer * 20 + 12;
    w_u32(state + 56, ordering);
    packet = r_u32(state + (buffer << 2) + 4);
    w_u32(state + 52, packet);
    ClearOTagR(psx_addr(ordering, 20), 5);
    if (sub_800582f8() != 0)
    {
        scene = r_u32(0x800BCE54);
        context = r_u32(scene + 8);
        return sub_80032f18(0x80000, context);
    }
    context = r_u32(0x800BCDA0);
    if (r_u32(context + 4) != 0)
    {
        state = r_u32(0x800BD050);
        ordering = r_u32(state + 56);
        packet = r_u32(state + 52);
        result = sub_8002d3f4(ordering, packet);
        state = r_u32(0x800BD050);
        w_u32(state + 52, result);
    }
    state = r_u32(0x800BD050);
    ordering = r_u32(state + 56);
    DrawOTag(psx_addr(ordering + 16, 4));
    return 0;
}

uint32 sub_80035184(uint32 object, uint32 packet)
{
    uint8 rectangles[16];
    uint32 scene, selection, a, b, value, x, y, offset, color, mode, index;
    FUNCTION_MARKER(0x80035184, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    a = r_u32(0x800BCBD0);
    b = r_u32(0x800BCBD4);
    xport_store_le32(rectangles, a);
    xport_store_le32(rectangles + 4, b);
    a = r_u32(0x800BCBD8);
    b = r_u32(0x800BCBDC);
    xport_store_le32(rectangles + 8, a);
    xport_store_le32(rectangles + 12, b);
    selection = scene + 668;
    value = r_s16(selection + 2);
    color = value == 0 ? 96 : 64;
    mode = value == 0 ? 53 : 85;
    x = (sint16)xport_load_le16(rectangles + 8);
    y = (sint16)xport_load_le16(rectangles + 10);
    offset = r_u32(0x800B0AF4);
    packet = sub_80034360(x + 58, y + offset + 5, object, packet, 0x800B0AB8, color);
    packet = sub_800346c0(object, packet, mode);
    value = r_s16(selection + 2);
    color = value == 1 ? 96 : 64;
    mode = value == 1 ? 53 : 85;
    x = (sint16)xport_load_le16(rectangles + 8);
    y = (sint16)xport_load_le16(rectangles + 10);
    offset = r_u32(0x800B0AF8);
    packet = sub_80034360(x + 64, y + offset + 5, object, packet, 0x800B0AC0, color);
    packet = sub_800346c0(object, packet, mode);
    index = r_s16(selection + 2);
    value = xport_load_le16(rectangles + 10);
    offset = r_u16(0x800B0AF4 + (index << 2));
    xport_store_le16(rectangles + 10, value + offset);
    packet = sub_800343c8(object, packet, 0, 1, rectangles + 8);
    packet = sub_800346c0(object, packet, 85);
    packet = sub_800343c8(object, packet, 0, 2, rectangles);
    packet = sub_80034360(125, 90, object, packet, 0x800B0AC8, 192);
    return sub_800346c0(object, packet, 21);
}

uint32 sub_80038e48(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80038E48, "ACE3.BIN");
    state = r_u32(0x800BD080);
    w_u32(state + 5448, 0);
    return state;
}

uint32 sub_80038e58(void)
{
    uint32 table, index, object, flags;
    FUNCTION_MARKER(0x80038E58, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    for (index = 0; index < 64; ++index)
    {
        object = r_u32(table);
        flags = r_u32(object);
        w_u32(object, flags & 0xFFFEFFFF);
        table += 4;
    }
    return 0;
}

uint32 sub_800396e8(void)
{
    uint32 scene, phase, result, active, value, header, index, payload, frame, game, mission, remaining, flags, limit;
    FUNCTION_MARKER(0x800396E8, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    phase = r_u32(scene + 4);
    if (phase == 16)
    {
        sub_80039430();
        result = sub_800399d4();
        w_u16(0x800BCF8A, 0);
        w_u32(0x800BCF8C, 0);
        return result;
    }
    if (phase == 64)
    {
        sub_800399d4();
        result = sub_800395d4();
        w_u32(0x800BCF64, 0);
        return result;
    }
    value = r_u16(0x800BCF74);
    if (value < 23)
        sub_8002cc44(255, 0, 0);
    sub_800399d4();
    result = sub_8002bca4();
    active = r_u32(0x800BCF6C);
    if (active != 0)
    {
        if ((sint32)result < 2)
        {
            value = r_u32(0x800BCF78);
            if (value != 1)
            {
                w_u32(0x800BCF6C, 0);
                return 1;
            }
        }
        value = r_s16(0x800BCF8A);
        if (value == 0)
        {
            result = cd_stream_get_frame(0, 0, 0x800BCF80, 0x800BCF84);
            if (result != 0)
            {
                result = r_u32(0x800BCF78);
                w_u32(0x800BCF6C, 0);
                if (result == 0)
                    return result;
                goto stop_stream;
            }
            header = r_u32(0x800BCF84);
            value = r_u8(header + 8);
            w_u16(0x800BCF88, value);
        }
        index = r_s16(0x800BCF8A);
        payload = r_u32(0x800BCF80);
        frame = payload + index * 572;
        header = r_u32(0x800BCF84);
        w_u32(0x800BCF60, frame);
        index = r_u16(0x800BCF8A);
        value = r_u16(header + 6);
        game = r_u32(0x800BCE60);
        remaining = value - index;
        mission = r_u32(game + 4152);
        w_u16(0x800BCF74, remaining);
        if (mission == 38)
        {
            remaining = r_u16(0x800BCF74);
            value = r_u32(0x800BCF68);
            sub_80093fc0(value - remaining + 2);
        }
        else if (mission == 72)
        {
            remaining = r_u16(0x800BCF74);
            value = r_u32(0x800BCF68);
            sub_80094380(value - remaining + 2);
        }
        sub_80038e48();
        sub_80038e58();
        frame = r_u32(0x800BCF60);
        flags = r_u32(frame);
        sub_80092ab8();
        frame = r_u32(0x800BCF60);
        sub_800393b8(frame + 4);
        sub_800850d8();
        game = r_u32(0x800BCE60);
        mission = r_u32(game + 4152);
        if (mission != 75 && mission != 37)
        {
            frame = r_u32(0x800BCF60);
            sub_80030528(frame + (flags & 0xFFFFFFFC));
        }
        index = r_u16(0x800BCF8A);
        limit = r_s16(0x800BCF88);
        ++index;
        w_u16(0x800BCF8A, index);
        result = (sint16)index < (sint32)limit;
        if (result == 0)
        {
            header = r_u32(0x800BCF84);
            result = cd_stream_free_frame(header);
            w_u16(0x800BCF8A, 0);
        }
        return result;
    }
    if ((sint32)result < 4)
    {
        value = r_u32(0x800BCF78);
        if (value != 1)
        {
            value = r_u32(0x800BCF8C) + 1;
            w_u32(0x800BCF8C, value);
            if ((sint32)value < 150)
                return 1;
            goto stop_stream;
        }
    }
    result = r_u32(0x800BCF64);
    w_u32(0x800BCF6C, 1);
    w_u16(0x800BCF8A, 0);
    w_u32(0x800BCF8C, 0);
    if (result == 0)
        w_u32(0x800BCF64, 1);
    return result;
stop_stream:
    scene = r_u32(0x800BCE54);
    value = r_u32(scene + 8);
    w_u32(0x800BCF64, 5);
    return sub_80032f18(64, value);
}

uint32 sub_80039430(void)
{
    uint32 game, mission, scene, index, table, record, value, flags, buffer, resource, argument, target, result;
    FUNCTION_MARKER(0x80039430, "ACE3.BIN");
    w_u32(0x800BCF60, 0x80201000);
    game = r_u32(0x800BCE60);
    mission = r_u32(game + 4152);
    scene = r_u32(0x800BCE54);
    index = r_u8(0x800B0FF4 + mission);
    table = r_u32(scene + 620);
    record = r_u32(table + (index << 2));
    value = r_u32(record) - 1;
    w_u32(0x800BCF68, value);
    value = r_u16(0x800BCF68);
    w_u32(0x800BCF70, 0);
    w_u16(0x800BCF74, value);
    sub_800798ec();
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    game = r_u32(0x800BCE60);
    w_u32(scene, flags | 1);
    flags = r_u32(game);
    w_u32(game, flags | 0x10000000);
    resource_init_queue(0, 0);
    buffer = arena_alloc(0x48C0);
    game = r_u32(0x800BCE60);
    mission = r_u32(game + 4152);
    w_u32(0x800BCF78, 0);
    resource = r_s16(0x800B0F34 + (mission << 1));
    sub_8002c928(resource, buffer, 0x48C0, 0x3000, 0x80039420);
    sub_800304f4();
    sub_800849ec();
    sub_8007da1c();
    sub_8006c530();
    sub_8002cc44(1, 0, 0);
    game = r_u32(0x800BCE60);
    value = r_s16(game + 940);
    w_u32(0x800BCF6C, 0);
    if (value != 0)
    {
        mission = r_u32(game + 4152);
        index = mission - 5;
        if (index < 89)
        {
            target = r_u32(0x80023A64 + (index << 2));
            if (target == 0x80039588)
            {
                scene = r_u32(0x800BCE54);
                argument = r_u32(scene + 8);
                sub_80032f18(64, argument);
                w_u32(0x800BCF64, 5);
                return 5;
            }
        }
    }
    scene = r_u32(0x800BCE54);
    argument = r_u32(scene + 8);
    result = sub_80032f18(32, argument);
    w_u32(0x800BCF64, 0);
    return result;
}

uint32 sub_800395d4(void)
{
    uint32 game, mission, scene, index, table, record, flags, restore;
    FUNCTION_MARKER(0x800395D4, "ACE3.BIN");
    resource_init_queue(0, 0);
    game = r_u32(0x800BCE60);
    mission = r_u32(game + 4152);
    scene = r_u32(0x800BCE54);
    index = r_u8(0x800B0FF4 + mission);
    table = r_u32(scene + 620);
    record = r_u32(table + (index << 2));
    w_u32(0x800BCF60, record + 4);
    sub_80038e48();
    sub_80038e58();
    record = r_u32(0x800BCF60);
    sub_800393b8(record);
    sub_8002cc44(255, 0, 0);
    sub_80030750();
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    w_u32(game, flags & 0xEFFFFFFF);
    mission = r_u32(game + 4152);
    restore = mission == 2 || mission == 4 || mission == 20 || mission == 30 || mission == 68 || mission == 85;
    return sub_80038d1c(restore);
}

uint32 sub_800399d4(void)
{
    uint32 remaining, total, game, elapsed, buttons, phase, value, scene, argument, direction, mission, index, target;
    uint32 ordering, packet, result, draw_flags, strength, counter, numerator, high, quotient, threshold, shift, render;
    sint64 product;
    FUNCTION_MARKER(0x800399D4, "ACE3.BIN");
    remaining = r_u16(0x800BCF74);
    total = r_u32(0x800BCF68);
    game = r_u32(0x800BCE60);
    buttons = r_u32(game + 300);
    elapsed = total - remaining + 2;
    if ((buttons & 64) != 0)
    {
        phase = r_u32(0x800BCF64);
        w_u32(0x800BCF70, 1);
        if (phase == 1)
            w_u32(0x800BCF64, 3);
        else if (phase == 2)
        {
            w_u32(0x800BCF64, 4);
            w_u32(0x800BCC10, 0x1680);
            w_u32(0x800BCC14, 0x1800);
        }
    }
    phase = r_u32(0x800BCF64);
    if (phase - 1 < 3)
    {
        total = r_u32(0x800BCF68);
        value = elapsed - 1;
        if ((sint32)total <= 0)
        {
            w_u32(0x800BCC10, 0);
            w_u32(0x800BCC14, 0);
        }
        else
        {
            w_u32(0x800BCC10, value << 7);
            w_u32(0x800BCC14, value << 8);
        }
        value = r_u32(0x800BCC10);
        if ((sint32)value < 0)
            w_u32(0x800BCC10, 0);
        value = r_u32(0x800BCC14);
        if ((sint32)value < 0)
            w_u32(0x800BCC14, 0);
        value = r_u32(0x800BCC10);
        if ((sint32)value >= 0x1800)
        {
            phase = r_u32(0x800BCF64);
            if (phase == 1)
                w_u32(0x800BCF64, 2);
            else if (phase == 3)
            {
                w_u32(0x800BCF64, 4);
                w_u32(0x800BCC10, 0x1680);
                w_u32(0x800BCC14, 0x1800);
            }
        }
        remaining = r_u16(0x800BCF74);
        if (remaining < 49)
        {
            w_u32(0x800BCF64, 4);
            w_u32(0x800BCC10, 0x1680);
            w_u32(0x800BCC14, 0x1800);
        }
    }
    else if (phase == 4)
    {
        value = r_u32(0x800BCC10) - 128;
        w_u32(0x800BCC10, value);
        if ((sint32)value <= 0)
        {
            scene = r_u32(0x800BCE54);
            w_u32(0x800BCC10, 0);
            argument = r_u32(scene + 8);
            w_u32(0x800BCF64, 5);
            sub_80032f18(64, argument);
        }
        value = r_u32(0x800BCF70);
        if (value == 0)
        {
            remaining = r_u16(0x800BCF74);
            if (remaining < 25)
            {
                value = r_u32(0x800BCC14);
                w_u32(0x800BCC14, value - 256);
            }
        }
        value = r_u32(0x800BCC14);
        if ((sint32)value <= 0)
            w_u32(0x800BCC14, 0);
    }
    else if (phase == 0 || phase == 5)
    {
        w_u32(0x800BCC10, 0);
        w_u32(0x800BCC14, 0);
        w_u32(0x800BCC18, 0);
    }
    phase = r_u32(0x800BCF64);
    direction = 0;
    if (phase - 4 < 2)
    {
        game = r_u32(0x800BCE60);
        mission = r_u32(game + 4152);
        index = mission - 9;
        if (index < 83)
        {
            target = r_u32(0x80023BC8 + (index << 2));
            direction = target == 0x80039C50;
        }
    }
    else if (phase == 1 || phase == 3 || phase == 0)
    {
        game = r_u32(0x800BCE60);
        mission = r_u32(game + 4152);
        direction = mission == 42;
    }
    game = r_u32(0x800BCE60);
    scene = r_u32(0x800BCE54);
    value = r_u32(0x800BCC10);
    ordering = r_u32(game + 12);
    packet = r_u32(scene + 20);
    result = sub_80093bc4(ordering, packet, direction, value);
    scene = r_u32(0x800BCE54);
    value = r_u32(0x800BCC14);
    ordering = r_u32(scene + 16);
    w_u32(scene + 20, result);
    result = sub_800939d4(ordering, result, value - 0x800);
    scene = r_u32(0x800BCE54);
    game = r_u32(0x800BCE60);
    w_u32(scene + 20, result);
    mission = r_u32(game + 4152);
    draw_flags = 0;
    strength = 0;
    index = mission - 39;
    if (index < 53)
    {
        target = r_u32(0x80023D18 + (index << 2));
        switch (target)
        {
            case 0x80039D0C:
            case 0x80039D6C:
            case 0x80039D3C:
            case 0x80039E28:
            case 0x80039E8C:
                threshold = target == 0x80039D3C ? 571 : target == 0x80039E28 ? 511 : target == 0x80039E8C ? 331 : 151;
                if ((sint32)elapsed >= (sint32)threshold)
                {
                    total = r_u32(0x800BCF68);
                    if ((sint32)total >= (sint32)elapsed)
                    {
                        counter = r_u32(0x800BCC18);
                        value = total - (threshold - 1);
                        shift = target == 0x80039E8C ? 11 : 12;
                        if (target != 0x80039E8C)
                            draw_flags = target == 0x80039E28 ? 2 : 1;
                        numerator = counter << shift;
                        if (value == 0)
                            xport_mips_break(7);
                        if (value == 0xFFFFFFFF && numerator == 0x80000000)
                            xport_mips_break(6);
                        strength = (sint32)numerator / (sint32)value;
                        if (target == 0x80039E8C)
                            draw_flags = 2;
                    }
                }
                break;
            case 0x80039D9C:
            case 0x80039DE0:
                if (elapsed - 1 < 90)
                {
                    counter = r_u32(0x800BCC18);
                    numerator = counter << 12;
                    product = (sint64)(sint32)numerator * (sint32)0xB60B60B7;
                    high = (uint32)(product >> 32);
                    quotient = (uint32)((sint32)(high + numerator) >> 6) - (uint32)((sint32)numerator >> 31);
                    draw_flags = 1;
                    strength = 0x1000 - quotient;
                }
                break;
            case 0x80039EEC:
                if (elapsed - 1 < 112)
                {
                    counter = r_u32(0x800BCC18);
                    numerator = (counter * 5) << 9;
                    product = (sint64)(sint32)numerator * (sint32)0x8FB823EF;
                    render = r_u32(0x800BCF90);
                    value = r_u32(render + 1480);
                    draw_flags = 2;
                    w_u32(render + 1480, value | 1);
                    high = (uint32)(product >> 32);
                    quotient = (uint32)((sint32)(high + numerator) >> 6) - (uint32)((sint32)numerator >> 31);
                    strength = 2560 - quotient;
                }
                if (elapsed - 113 < 16)
                {
                    render = r_u32(0x800BCF90);
                    draw_flags |= 1;
                    value = r_u32(render + 1480);
                    strength = 1024;
                    w_u32(render + 1480, value | 1);
                }
                break;
        }
    }
    if ((draw_flags & 1) != 0)
    {
        scene = r_u32(0x800BCE54);
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_80084474(ordering + 8, packet, strength);
        scene = r_u32(0x800BCE54);
        render = r_u32(0x800BCF90);
        w_u32(scene + 20, result);
        counter = r_u32(0x800BCC18);
        value = r_u32(render + 1480);
        w_u32(0x800BCC18, counter + 1);
        w_u32(render + 1480, value | 1);
    }
    result = draw_flags & 2;
    if (result != 0)
    {
        scene = r_u32(0x800BCE54);
        ordering = r_u32(scene + 16);
        packet = r_u32(scene + 20);
        result = sub_8008fc9c(ordering, packet, strength);
        counter = r_u32(0x800BCC18);
        scene = r_u32(0x800BCE54);
        w_u32(scene + 20, result);
        w_u32(0x800BCC18, counter + 1);
    }
    return result;
}

uint32 sub_80093bc4(uint32 ordering, uint32 packet, uint32 direction, uint32 progress)
{
    uint32 strength, render, current, value;
    FUNCTION_MARKER(0x80093BC4, "ACE3.BIN");
    if ((sint32)progress >= 4096)
        return packet;
    strength = 4096 - progress;
    w_u32(0x1F800000, 0);
    w_u32(0x1F800004, 0x00F00140);
    if (direction == 0)
        strength = 0 - strength;
    render = r_u32(0x800BCF90);
    w_u32(render + 1480, 1);
    current = sub_80093b2c(ordering, packet, 0x1F800000, strength);
    w_u32(current, 0x01000000);
    w_u32(current + 4, 0xE6000001);
    value = r_u32(ordering);
    w_u32(current, (value & 0xFFFFFF) | 0x01000000);
    value = r_u32(ordering);
    w_u32(ordering, (value & 0xFF000000) | (current & 0xFFFFFF));
    return current + 8;
}

uint32 sub_800939d4(uint32 ordering, uint32 packet, uint32 progress)
{
    uint32 value, edge, height, bottom, game, buffer, first, second, link;
    FUNCTION_MARKER(0x800939D4, "ACE3.BIN");
    if ((sint32)progress <= 0)
        return packet;
    if ((sint32)progress >= 4096)
        value = 4096;
    else
        value = r_s16(0x800B7104 + ((progress >> 2) << 2));
    edge = (sint32)(value << 5) >> 12;
    height = (sint32)((value * 3) << 4) >> 12;
    bottom = 240 - height;
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    w_u32(packet, 0x02000000);
    buffer = (buffer != 0) << 8;
    first = buffer + edge;
    second = ((height << 16) | 320);
    w_u32(packet + 4, ((first & 0x3FF) << 10) | 0xE3000000);
    w_u32(packet + 8, (((first + bottom - edge - 1) & 0x3FF) << 10) | 0xE400013F);
    value = r_u32(ordering + 0x3FFC);
    w_u32(packet, value | 0x02000000);
    w_u32(ordering + 0x3FFC, packet & 0xFFFFFF);
    w_u32(packet + 12, ((packet + 24) & 0xFFFFFF) | 0x02000000);
    w_u32(packet + 16, (buffer << 10) | 0xE3000000);
    w_u32(packet + 20, ((buffer + 239) << 10) | 0xE400013F);
    w_u32(packet + 24, 0x06000000);
    w_u32(packet + 28, 0x60000000);
    w_u32(packet + 40, 0x60000000);
    w_u16(packet + 44, 0);
    w_u16(packet + 32, 0);
    w_u16(packet + 34, 0);
    w_u16(packet + 46, bottom);
    w_u32(packet + 36, (edge << 16) | 320);
    w_u32(packet + 48, second);
    link = r_masked_u32(ordering + 4, 7);
    w_masked_u32(packet + 24, link, 7);
    w_masked_u32(ordering + 4, (packet + 12) & 0xFFFFFF, 7);
    return packet + 52;
}

uint32 sub_800798ec(void)
{
    uint32 scene, table, camera, object;
    FUNCTION_MARKER(0x800798EC, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    table = r_u32(0x800BCE5C);
    camera = r_u32(scene + 40);
    object = r_u32(table);
    sub_800793c4(camera, 4096, object);
    return sub_80035740(0);
}

uint32 sub_80038d1c(uint32 restore)
{
    uint32 snapshot[8];
    uint32 scene, camera, source, near_value, far_value, height, matrix, index, offset, table, object, word, cursor, game, flags;
    FUNCTION_MARKER(0x80038D1C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    source = r_u32(0x800BCF58);
    copy_words(camera, source, 99);
    near_value = r_s16(camera + 40);
    far_value = r_s16(camera + 42);
    scene = r_u32(0x800BCE54);
    height = r_s16(camera + 36);
    matrix = r_u32(scene + 648);
    sub_800777b0(camera, near_value, far_value, height, matrix);
    offset = 396;
    if (restore != 0)
    {
        for (index = 0; index < 64; ++index)
        {
            source = r_u32(0x800BCF58) + offset;
            for (word = 0; word < 8; ++word)
                snapshot[word] = r_u32(source + word * 4);
            table = r_u32(0x800BCE5C);
            object = r_u32(table + (index << 2));
            w_u32(object, snapshot[0]);
            if (index == 0)
                sub_80072780(object + 8, 0);
            offset += 32;
        }
    }
    else
    {
        for (index = 0; index < 64; ++index)
        {
            source = r_u32(0x800BCF58);
            table = r_u32(0x800BCE5C);
            object = r_u32(table + (index << 2));
            copy_words(object, source + offset, 8);
            offset += 32;
        }
    }
    cursor = r_u32(0x800BCF5C);
    arena_set_cursor(cursor);
    sub_800849ec();
    game = r_u32(0x800BCE60);
    flags = r_u32(game);
    w_u32(game, flags & 0xEFFFFFFF);
    sub_8006c530();
    return sub_80037e50();
}

uint32 sub_80077768(uint32 camera, uint32 height)
{
    uint32 near_value, far_value;
    FUNCTION_MARKER(0x80077768, "ACE3.BIN");
    w_u16(camera + 36, height);
    SetGeomScreen((sint32)height);
    near_value = r_s16(camera + 40);
    far_value = r_s16(camera + 42);
    return sub_8005e7ec(near_value, far_value, height);
}

uint32 sub_800777b0(uint32 camera, uint32 near_value, uint32 far_value, uint32 height, uint32 matrix)
{
    FUNCTION_MARKER(0x800777B0, "ACE3.BIN");
    if ((sint32)far_value < (sint32)near_value)
        return 0;
    if ((sint32)near_value < 1000)
        near_value = 1000;
    else if ((sint32)near_value >= 31768)
        near_value = 31767;
    if ((sint32)far_value < 1000)
        far_value = 1000;
    else if ((sint32)far_value >= 31768)
        far_value = 31767;
    sub_8005eab8(near_value, far_value, matrix);
    w_u16(camera + 40, near_value);
    w_u16(camera + 42, far_value);
    if (height == 0)
        height = r_s16(camera + 36);
    sub_80077768(camera, height);
    return 1;
}

uint32 sub_80038e98(uint32 script)
{
    uint32 scene, camera, value;
    FUNCTION_MARKER(0x80038E98, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(script + 4);
    w_u16(camera + 20, value);
    value = r_u16(script + 6);
    w_u16(camera + 22, value);
    value = r_u16(script + 8);
    w_u16(camera + 24, value);
    value = r_u32(script + 12);
    w_u32(camera + 4, value << 12);
    value = r_u32(script + 16);
    w_u32(camera + 8, value << 12);
    value = r_u32(script + 20);
    w_u32(camera + 12, value << 12);
    return script + 24;
}

uint32 sub_80038f00(uint32 script)
{
    uint32 scene, camera, value, height, near_value, far_value, changed, matrix, flags, table, object, kind, mode;
    FUNCTION_MARKER(0x80038F00, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_s16(script + 2);
    height = r_s16(camera + 36);
    near_value = r_s16(camera + 40);
    far_value = r_s16(camera + 42);
    changed = 0;
    if (value != 0 && height != value)
    {
        height = value;
        changed = 1;
    }
    value = r_s16(script + 4);
    if (value != 0 && near_value != value)
    {
        near_value = value;
        changed = 2;
    }
    value = r_s16(script + 6);
    if (value != 0 && far_value != value)
    {
        far_value = value;
        changed = 2;
    }
    if (changed == 1)
        sub_80077768(camera, height);
    else if (changed == 2)
    {
        scene = r_u32(0x800BCE54);
        matrix = r_u32(scene + 648);
        sub_800777b0(camera, near_value, far_value, height, matrix);
    }
    mode = 0x2000;
    flags = r_u32(script);
    table = r_u32(0x800BCE5C);
    kind = (flags >> 8) & 3;
    object = r_u32(table);
    if (kind != 0)
    {
        object = r_u32(table + ((flags >> 8) & 0xFC));
        if (kind == 1 || kind == 2)
            mode = kind;
    }
    sub_800793c4(camera, mode, object);
    return script + 8;
}

uint32 sub_80039028(uint32 script)
{
    uint32 flags, table, object, value;
    FUNCTION_MARKER(0x80039028, "ACE3.BIN");
    flags = r_u32(script);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + ((flags >> 6) & 0xFC));
    value = r_u32(object);
    w_u32(object, value | 0x14000);
    value = r_u16(script + 4);
    w_u16(object + 24, value);
    value = r_u16(script + 6);
    w_u16(object + 26, value);
    value = r_u16(script + 8);
    w_u16(object + 28, value);
    value = r_u32(script + 12);
    w_u32(object + 8, value << 12);
    value = r_u32(script + 16);
    w_u32(object + 12, value << 12);
    value = r_u32(script + 20);
    w_u32(object + 16, value << 12);
    flags = r_u32(script);
    value = r_u8(object + 5);
    w_u8(object + 5, (flags & 0x4000) != 0 ? value | 8 : value & 0xF7);
    return script + 24;
}

uint32 sub_800390e4(uint32 script)
{
    uint32 flags, table, object, value;
    FUNCTION_MARKER(0x800390E4, "ACE3.BIN");
    flags = r_u32(script);
    table = r_u32(0x800BCE5C);
    object = r_u32(table + ((flags >> 6) & 0xFC));
    value = r_u32(object);
    w_u32(object, value | 0x14000);
    value = r_u16(script + 4);
    w_u16(object + 24, value);
    value = r_u16(script + 6);
    w_u16(object + 26, value);
    value = r_u16(script + 8);
    w_u16(object + 28, value);
    value = r_u32(script + 12);
    w_u32(object + 8, value << 12);
    value = r_u32(script + 16);
    w_u32(object + 12, value << 12);
    value = r_u32(script + 20);
    w_u32(object + 16, value << 12);
    return script + 24;
}

uint32 sub_8003916c(uint32 script)
{
    NATIVE_POSE pose;
    uint32 flags, index;
    FUNCTION_MARKER(0x8003916C, "ACE3.BIN");
    pose.position[0] = r_u32(script + 12) << 12;
    pose.position[1] = r_u32(script + 16) << 12;
    pose.position[2] = r_u32(script + 20) << 12;
    pose.angles[0] = r_u16(script + 4);
    pose.angles[1] = r_u16(script + 6);
    pose.angles[2] = r_u16(script + 8);
    flags = r_u32(script);
    if (((flags >> 8) & 7) == 0)
    {
        index = (flags >> 11) & 31;
        if ((flags & 0x200000) != 0)
            sub_8006c6b8(index, 0x1801, 0, &pose);
        else
            sub_8006c740(index);
    }
    return script + 24;
}

uint32 sub_80039230(uint32 script)
{
    uint32 flags, kind, target, count;
    FUNCTION_MARKER(0x80039230, "ACE3.BIN");
    flags = r_u32(script);
    kind = (flags >> 8) & 31;
    target = r_u32(0x800239E4 + (kind << 2));
    switch (target)
    {
        case 0x80039278:
            sub_8009351c(script, kind);
            break;
        case 0x80039288:
            sub_80093484(script, kind - 8);
            break;
        case 0x8003929C:
            count = r_s16(script + 8);
            sub_800935ac(script, count, 0);
            break;
        case 0x800392B4:
            count = r_s16(script + 8);
            sub_800935ac(script, count, 1);
            break;
        case 0x800392CC:
            count = r_s16(script + 8);
            sub_800935ac(script, count, 2);
            break;
        case 0x800392E4:
            sub_80092b80(script);
            break;
        case 0x800392F4:
            sub_80092d90(script);
            break;
        case 0x80039304:
            sub_80093040(script);
            break;
        case 0x80039314:
            sub_8009319c(script);
            break;
        case 0x80039324:
            sub_80093324(script, 0);
            break;
        case 0x80039338:
            sub_80093324(script, 1);
            break;
        case 0x8003934C:
            sub_80093324(script, 2);
            break;
        case 0x80039360:
            sub_800937d8(script, 4);
            break;
    }
    return script + 24;
}

uint32 sub_80039380(uint32 script)
{
    FUNCTION_MARKER(0x80039380, "ACE3.BIN");
    return script + 4;
}

uint32 sub_80039388(uint32 script)
{
    FUNCTION_MARKER(0x80039388, "ACE3.BIN");
    sub_80093944(script);
    return script + 32;
}

uint32 sub_800393b0(uint32 script)
{
    FUNCTION_MARKER(0x800393B0, "ACE3.BIN");
    return script + 4;
}

uint32 sub_800393b8(uint32 script)
{
    uint32 flags, kind, target;
    FUNCTION_MARKER(0x800393B8, "ACE3.BIN");
    flags = r_u32(script);
    kind = flags & 127;
    while (kind != 0)
    {
        target = r_u32(0x800B0F10 + (kind << 2));
        script = xport_guest_call3(target, script, 0, 0);
        flags = r_u32(script);
        kind = flags & 127;
    }
    return script;
}

uint32 sub_80093944(uint32 script)
{
    uint32 record, value, first, second, third, byte, half, result;
    FUNCTION_MARKER(0x80093944, "ACE3.BIN");
    record = sub_80082ff0();
    result = 3;
    if (record != 0)
    {
        value = r_s16(script + 2);
        w_u16(record + 2, 3);
        w_u16(record, value);
        first = r_u32(script + 8);
        second = r_u32(script + 12);
        third = r_u32(script + 16);
        byte = r_u8(script + 4);
        half = r_s16(script + 6);
        w_u32(record + 4, (first & 0xFFFFFF00) | byte);
        w_u32(record + 12, third);
        result = (second & 0xFFFFFF00) | (half & 255);
        w_u32(record + 8, result);
        first = r_u32(script + 20);
        second = r_u32(script + 24);
        third = r_u32(script + 28);
        w_u32(record + 16, first);
        w_u32(record + 20, second);
        w_u32(record + 24, third);
    }
    return result;
}

uint32 sub_80092b04(void)
{
    uint32 render, base, group, masks, offset, bits, mask, record, pending;
    FUNCTION_MARKER(0x80092B04, "ACE3.BIN");
    render = r_u32(0x800BCF90);
    base = render + 1536;
    masks = base;
    offset = 12;
    for (group = 0; group < 3; ++group)
    {
        bits = r_u32(masks);
        if (bits != 0xFFFFFFFF)
        {
            mask = 1;
            record = base + offset;
            while ((bits & mask) != 0)
            {
                mask <<= 1;
                record += 28;
            }
            pending = r_u32(masks + 2700);
            w_u32(masks, bits | mask);
            w_u32(masks + 2700, pending | mask);
            return record;
        }
        masks += 4;
        offset += 896;
    }
    return 0;
}

uint32 sub_8009351c(uint32 script, uint32 kind)
{
    uint32 record, first, second, value, result;
    FUNCTION_MARKER(0x8009351C, "ACE3.BIN");
    record = sub_80092b04();
    result = record;
    if (record != 0)
    {
        first = r_u32(script + 12);
        second = r_u32(script + 16);
        w_u32(record + 4, first);
        w_u32(record + 8, second);
        first = r_u32(script + 20);
        second = r_u32(script + 4);
        w_u32(record + 12, first);
        w_u32(record, second);
        value = r_s16(script + 10);
        w_u16(record + 22, 0);
        w_u16(record + 26, value);
        result = (sint32)kind < 4 ? (kind & 3) << 8 : (((kind & 1) + 1) << 8) | 16;
        w_u16(record + 24, result);
    }
    return result;
}

uint32 sub_80093484(uint32 script, uint32 kind)
{
    uint32 render, record, first, second, third, value, half, result;
    FUNCTION_MARKER(0x80093484, "ACE3.BIN");
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 13248, 1, 6);
    result = record;
    if (record != 0)
    {
        first = r_u32(script + 12);
        second = r_u32(script + 16);
        third = r_u32(script + 20);
        w_u32(record + 4, first);
        w_u32(record + 8, second);
        w_u32(record + 12, third);
        if (kind != 0)
        {
            value = r_u32(script + 4);
            w_u16(record + 20, 1);
            w_u32(record + 16, value);
        }
        else
            w_u16(record + 20, 0);
        value = r_s16(script + 2);
        half = r_s16(script + 10);
        result = value | 0x10000000;
        w_u32(record, result);
        w_u16(record + 22, half);
    }
    return result;
}

uint32 sub_80093324(uint32 script, uint32 mode)
{
    uint32 render, record, x, y, z, value, random, scale, result;
    FUNCTION_MARKER(0x80093324, "ACE3.BIN");
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 4248, 10, 7);
    result = record;
    if (record != 0)
    {
        w_u16(record, 0);
        x = r_s16(script + 4);
        y = r_s16(script + 6);
        z = r_s16(script + 8);
        w_u16(record + 16, x);
        w_u16(record + 18, y);
        w_u16(record + 20, z);
        value = r_u32(script + 12);
        w_u32(record + 4, value - (x << 8) - (x << 6));
        value = r_u32(script + 16);
        w_u32(record + 8, value - (y << 8) - (y << 6));
        value = r_u32(script + 20);
        w_u32(record + 12, value - (z << 8) - (z << 6));
        random = game_random() & 127;
        scale = r_s16(script + 10);
        value = (random + 160) * scale;
        value = (sint32)value / 4096;
        w_u16(record + 26, value);
        value = r_s16(script + 2);
        if (mode == 2)
        {
            value = (sint32)value / 256;
            w_u16(record + 2, value);
            w_u16(record + 22, 140);
            result = 0xA04;
        }
        else
        {
            value *= 80;
            value = (sint32)value / 4096;
            w_u16(record + 2, value);
            w_u16(record + 22, 397);
            result = mode == 0 ? 0x1600 : 0x1603;
        }
        w_u16(record + 24, result);
    }
    return result;
}

void sub_800935ac(uint32 script, uint32 count, uint32 mode)
{
    uint32 saved_random, seed, x, y, z, start, spread, scale, index, random, position, lifetime, record, first, second, third, value, format;
    FUNCTION_MARKER(0x800935AC, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    saved_random = game_random();
    seed = r_s16(script + 2);
    w_u32(0x800CBF58, seed);
    x = r_u32(script + 12);
    y = r_u32(script + 16);
    z = r_u32(script + 20);
    start = r_s16(script + 4);
    spread = r_s16(script + 6);
    scale = r_s16(script + 10);
    index = 0;
    while ((sint32)index < (sint32)count)
    {
        random = game_random();
        if (mode != 0)
        {
            position = start - (index + (random & 3));
            lifetime = (((uint32)((sint32)random >> 2)) & 7) + 20;
        }
        else
        {
            position = start - (index + (random & 1));
            lifetime = (random & 7) + 8;
        }
        if ((sint32)position < 0 || (sint32)position >= (sint32)lifetime)
        {
            ++index;
            game_random();
            game_random();
            continue;
        }
        record = sub_80092b04();
        if (record != 0)
        {
            w_u32(record, position | (lifetime << 16));
            random = game_random();
            first = ((random & 0x1FF) - 256) * spread;
            second = ((((uint32)((sint32)random >> 2)) & 0x1FF) - 256) * spread;
            third = ((((uint32)((sint32)random >> 4)) & 0x1FF) - 256) * spread;
            w_u16(record + 22, 0);
            w_u32(record + 4, x + (first << 4));
            w_u32(record + 8, y + (second << 4));
            w_u32(record + 12, z + (third << 4));
            random = game_random();
            value = scale * ((((uint32)((sint32)random >> 3)) & 0x7FF) + 3072);
            w_u16(record + 26, (sint32)value >> 12);
            if (mode != 0)
                format = mode == 1 ? 0x110 : 0x210;
            else
                format = (random & 3) != 0 ? 0x100 : 0;
            w_u16(record + 24, format);
        }
        ++index;
    }
    w_u32(0x800CBF58, saved_random);
}

void sub_800937d8(uint32 script, uint32 count)
{
    uint32 x, y, z, kind, spread, scale, index, render, record, random, first, second, third, x_offset, y_offset, z_offset, value;
    FUNCTION_MARKER(0x800937D8, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    x = r_u32(script + 12);
    y = r_u32(script + 16);
    z = r_u32(script + 20);
    kind = r_s16(script + 4);
    spread = r_s16(script + 6);
    scale = r_s16(script + 10);
    for (index = 0; (sint32)index < (sint32)count; ++index, kind += 2)
    {
        render = r_u32(0x800BCF90);
        record = sub_8007fdb4(render + 4248, 10, 7);
        if (record == 0)
            return;
        w_u32(record, kind << 16);
        random = game_random();
        first = ((((uint32)((sint32)random >> 2)) & 255) - 128) * spread;
        second = ((((uint32)((sint32)random >> 4)) & 255) - 128) * spread;
        third = ((random & 255) - 128) * spread;
        x_offset = (sint32)first >> 8;
        y_offset = (sint32)second >> 8;
        w_u16(record + 22, 0x188);
        w_u32(record + 4, x + (x_offset << 9));
        w_u32(record + 8, y + (y_offset << 9));
        w_u16(record + 18, x_offset - (uint32)((sint32)first >> 10));
        w_u16(record + 20, y_offset);
        z_offset = (sint32)third >> 8;
        w_u32(record + 12, z + (z_offset << 9));
        w_u16(record + 16, z_offset);
        random = game_random();
        value = scale * ((random & 0xFFF) + 2048);
        value = (sint32)value / 4096;
        w_u16(record + 26, value);
    }
}

uint32 sub_80093040(uint32 script)
{
    uint32 scene, phase, ticks, result, render, scale, record, random, value, coordinate;
    FUNCTION_MARKER(0x80093040, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    phase = r_s16(script + 2);
    ticks = r_u32(scene + 12);
    result = (ticks + phase) & 3;
    if (result != 0)
        return result;
    render = r_u32(0x800BCF90);
    scale = r_s16(script + 10);
    record = sub_8007fdb4(render + 1536, 3, 7);
    if (record == 0)
        return 0x500000;
    w_u32(record, 0x500000);
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = r_u32(script + 12);
    w_u32(record + 4, coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = r_u32(script + 16);
    w_u32(record + 8, coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = r_u32(script + 20);
    w_u32(record + 12, coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    result = 0 - scale * 144;
    w_u16(record + 18, (sint32)result >> 12);
    w_u16(record + 16, (sint32)value >> 12);
    random = game_random();
    value = scale * ((random & 63) - 32);
    w_u16(record + 22, 128);
    w_u16(record + 24, 0x210);
    w_u16(record + 20, (sint32)value >> 12);
    random = game_random();
    value = scale * ((random & 63) + 88);
    result = (sint32)value >> 12;
    w_u16(record + 26, result);
    return result;
}

uint32 sub_8009319c(uint32 script)
{
    uint32 scene, result, render, record, phase, scale, random, value, x, y, z, coordinate;
    FUNCTION_MARKER(0x8009319C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    result = r_u32(scene + 12) & 1;
    if (result != 0)
        return result;
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 1536, 3, 7);
    if (record == 0)
        return record;
    phase = r_s16(script + 2);
    scale = r_s16(script + 10);
    w_u16(record, (sint32)phase / 1024);
    random = game_random();
    value = ((random & 15) + 64) * phase;
    value = (sint32)value / 4096;
    w_u16(record + 2, value);
    x = r_s16(script + 4);
    y = r_s16(script + 6);
    z = r_s16(script + 8);
    w_u16(record + 22, 6);
    w_u16(record + 16, x);
    w_u16(record + 18, y);
    w_u16(record + 20, z);
    coordinate = r_u32(script + 12);
    x = coordinate + (x << 9);
    coordinate = r_u32(script + 16);
    y = coordinate + (y << 9);
    coordinate = r_u32(script + 20);
    z = coordinate + (z << 9);
    random = game_random();
    value = ((random & 255) - 128) * scale;
    w_u32(record + 4, x + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 255) - 128) * scale;
    w_u32(record + 8, y + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 255) - 128) * scale;
    w_u16(record + 24, 0x111);
    w_u32(record + 12, z + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 15) + 240) * scale;
    result = (sint32)value >> 12;
    w_u16(record + 26, result);
    return result;
}

uint32 sub_80092b80(uint32 script)
{
    uint32 scale, random, value, coordinate, x, y, z, render, record, result, ax, ay, az;
    FUNCTION_MARKER(0x80092B80, "ACE3.BIN");
    scale = r_s16(script + 10);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = r_u32(script + 12);
    x = coordinate + (uint32)((sint32)value >> 3);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = r_u32(script + 16);
    y = coordinate + (uint32)((sint32)value >> 3);
    random = game_random();
    value = scale * ((random & 255) - 128);
    render = r_u32(0x800BCF90);
    coordinate = r_u32(script + 20);
    z = coordinate + (uint32)((sint32)value >> 3);
    record = sub_8007fdb4(render + 1536, 3, 7);
    if (record != 0)
    {
        w_u16(record + 2, 10);
        w_u16(record, 0);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u16(record + 24, 0x100);
        random = game_random();
        value = scale * ((random & 63) + 78);
        w_u16(record + 26, (sint32)value >> 12);
    }
    result = (r_u32(script) >> 13) & 1;
    if (result == 0)
        return result;
    result = game_random() & 1;
    if (result == 0)
        return result;
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 4248, 10, 7);
    if (record == 0)
        return record;
    ax = r_u16(script + 4);
    ay = r_u16(script + 6);
    az = r_u16(script + 8);
    w_u16(record, 0);
    ax <<= 16;
    ay <<= 16;
    az <<= 16;
    random = game_random();
    w_u16(record + 2, (random & 7) + 20);
    w_u16(record + 24, 0);
    random = game_random();
    w_u16(record + 26, (random & 7) + 12);
    w_u32(record + 4, x + (uint32)((sint32)ax >> 16));
    w_u32(record + 8, y + (uint32)((sint32)ay >> 16));
    w_u32(record + 12, z + (uint32)((sint32)az >> 16));
    random = game_random();
    value = (uint32)((sint32)ax >> 17) + (random & 255) - 128;
    w_u16(record + 16, value);
    random = game_random();
    value = (uint32)((sint32)ay >> 17) + (random & 255) - 128;
    w_u16(record + 18, value);
    random = game_random();
    value = (uint32)((sint32)az >> 17) + (random & 255) - 128;
    w_u16(record + 20, value);
    w_u16(record + 22, 0x188);
    return 0x188;
}

uint32 sub_80092d90(uint32 script)
{
    uint32 scene, phase, ticks, result, scale, random, value, coordinate, x, y, z, flags, render, record, ax, ay, az;
    FUNCTION_MARKER(0x80092D90, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    phase = r_s16(script + 2);
    ticks = r_u32(scene + 12);
    result = (ticks + phase) & 1;
    if (result != 0)
        return result;
    scale = r_s16(script + 10);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = r_u32(script + 12);
    x = coordinate + (uint32)((sint32)value >> 2);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = r_u32(script + 16);
    y = coordinate + (uint32)((sint32)value >> 2);
    random = game_random();
    value = scale * ((random & 255) - 128);
    flags = r_u32(script);
    coordinate = r_u32(script + 20);
    z = coordinate + (uint32)((sint32)value >> 2);
    if ((flags & 0x4000) != 0)
    {
        random = game_random();
        if ((random & 3) != 0)
        {
            render = r_u32(0x800BCF90);
            record = sub_8007fdb4(render + 1536, 3, 7);
            if (record != 0)
            {
                w_u16(record + 2, 10);
                w_u16(record, 0);
                w_u32(record + 4, x);
                w_u32(record + 8, y);
                w_u32(record + 12, z);
                w_u16(record + 22, 0);
                w_u16(record + 24, 0x100);
                random = game_random();
                value = scale * ((random & 63) + 58);
                w_u16(record + 26, (sint32)value >> 12);
            }
        }
    }
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 1536, 3, 7);
    if (record != 0)
    {
        w_u16(record, -3);
        w_u16(record + 2, 40);
        w_u32(record + 4, x);
        w_u32(record + 8, y);
        w_u32(record + 12, z);
        w_u16(record + 22, 0);
        w_u16(record + 24, 0x210);
        random = game_random();
        value = scale * ((random & 127) + 76);
        w_u16(record + 26, (sint32)value >> 12);
    }
    result = (r_u32(script) >> 13) & 1;
    if (result == 0)
        return result;
    result = game_random() & 1;
    if (result == 0)
        return result;
    render = r_u32(0x800BCF90);
    record = sub_8007fdb4(render + 4248, 10, 7);
    if (record == 0)
        return record;
    ax = r_u16(script + 4);
    ay = r_u16(script + 6);
    az = r_u16(script + 8);
    w_u16(record, 0);
    ax <<= 16;
    ay <<= 16;
    az <<= 16;
    random = game_random();
    w_u16(record + 2, (random & 7) + 20);
    w_u16(record + 24, 0);
    random = game_random();
    w_u16(record + 26, (random & 7) + 12);
    w_u32(record + 4, x + (uint32)((sint32)ax >> 16));
    w_u32(record + 8, y + (uint32)((sint32)ay >> 16));
    w_u32(record + 12, z + (uint32)((sint32)az >> 16));
    random = game_random();
    value = (uint32)((sint32)ax >> 17) + (random & 255) - 128;
    w_u16(record + 16, value);
    random = game_random();
    value = (uint32)((sint32)ay >> 17) + (random & 255) - 128;
    w_u16(record + 18, value);
    random = game_random();
    value = (uint32)((sint32)az >> 17) + (random & 255) - 128;
    w_u16(record + 20, value);
    w_u16(record + 22, 0x188);
    return 0x188;
}

uint32 sub_80036130(void)
{
    uint32 game, value, high, quotient, entry, result;
    FUNCTION_MARKER(0x80036130, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u8(game + 934);
    w_u8(game + 934, value + 1);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 934);
    high = (uint32)(((uint64)value * 0xBACF914D) >> 32);
    quotient = (high + ((value - high) >> 1)) >> 5;
    w_u8(game + 934, value - quotient * 37);
    game = r_u32(0x800BCE60);
    value = r_u8(game + 934);
    entry = 0x800B0BD0 + (value << 2);
    value = r_u8(entry + 2);
    w_u16(game + 908, value);
    result = r_u8(entry + 3);
    w_u16(game + 910, result);
    return result;
}

uint32 sub_800361bc(void)
{
    uint32 game, index, data, resource, bytes, buffer, queue;
    FUNCTION_MARKER(0x800361BC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    index = r_u8(game + 934);
    data = r_u32(0x800BCE50);
    resource = r_s16(0x800B0BD0 + (index << 2));
    w_u32(data + 96, resource);
    bytes = resource_total_size(data + 96, 1);
    buffer = arena_alloc(bytes);
    data = r_u32(0x800BCE50);
    queue = r_u32(0x800BCBE0);
    w_u32(queue + 8, 1);
    resource_queue_list(data + 96, 1, buffer, 0x800357B4);
    sub_800357c0();
    return 0x04000000;
}

uint32 sub_8003623c(void)
{
    uint32 data, resource, scene;
    FUNCTION_MARKER(0x8003623C, "ACE3.BIN");
    sub_800357c0();
    data = r_u32(0x800BCE50);
    resource = r_u32(data + 96);
    resource = sub_80027338(resource, 0);
    resource = r_u32(resource);
    resource = sub_80027338(resource, 0);
    sub_8007dbc4(resource);
    scene = r_u32(0x800BCE54);
    w_u32(scene + 4, 128);
    w_u32(scene + 8, 0);
    return sub_80032080(0, 0);
}

uint32 sub_80036440(uint32 ordering, uint32 packet)
{
    uint32 state, flags, game, buffer;
    FUNCTION_MARKER(0x80036440, "ACE3.BIN");
    state = r_u32(0x800BCE48);
    flags = r_u32(state);
    game = r_u32(0x800BCE60);
    w_u32(state, flags | 4);
    buffer = r_u32(game + 4);
    if (buffer != 0)
        return gpu_copy_rect(ordering, packet, 0, 256, 0, 0, 320, 240);
    return gpu_copy_rect(ordering, packet, 0, 0, 0, 256, 320, 240);
}

uint32 sub_800364c4(uint32 ordering, uint32 packet)
{
    MATRIX matrix;
    uint16 angles[3];
    uint32 game, ticks, initial, first, second, phase, value;
    FUNCTION_MARKER(0x800364C4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    ticks = r_u32(game + 32);
    initial = r_u16(0x800104AC + ((ticks << 2) & 0x7FFC));
    angles[0] = (uint16)initial;
    angles[1] = (uint16)initial;
    angles[2] = (uint16)initial;
    rotation_build(0, 0, &matrix, angles);
    sky_load_rotation(&matrix);
    first = r_u16(0x800104AC + ((ticks << 4) & 0x7FF0));
    phase = ticks << 4;
    second = r_u16(0x800104AC + ((phase & 0x3FFF) << 1));
    first = (sint32)(first << 16) >> 17;
    initial = (sint32)(initial << 16) >> 17;
    second = (sint32)(second << 16) >> 17;
    xport_gte_write_control(5, first);
    xport_gte_write_control(6, initial);
    xport_gte_write_control(7, second);
    value = hud_sdk_cosine(phase);
    value = (uint32)((sint32)value >> 6) + 300;
    xport_gte_write_control(26, value);
    SetGeomOffset(160, 120);
    return sub_80083fe0(ordering, packet);
}

uint32 sub_80036e84(uint32 object, uint32 dst)
{
    uint32 table[17];
    PSX_RECT rectangle;
    uint32 index, queue, state, progress, inverse, packet, selection, value, width, height, result;
    FUNCTION_MARKER(0x80036E84, "ACE3.BIN");
    for (index = 0; index < 16; ++index)
        table[index] = r_u32(0x800239A0 + index * 4);
    queue = r_u32(0x800BCBE0);
    r_masked_u32(0x800239E0, 15);
    table[16] = r_masked_u32(0x800239E0, 15);
    state = r_u8(queue + 6);
    if (state == 1)
        progress = r_s16(queue + 2);
    else
        progress = state == 2 ? 4096 : 0;
    if ((sint32)progress >= 4097)
        progress = 4096;
    inverse = 4096 - progress;
    packet = sub_80036824(object, dst);
    queue = r_u32(0x800BCBE0);
    selection = r_u32(queue + 56);
    if ((sint32)selection < 0)
        selection = 0;
    if ((sint32)selection >= 17)
        selection = 16;
    value = table[selection];
    width = (uint32)((sint32)(sint16)value * 320);
    width = (sint32)width / 4096;
    rectangle.w = (sint16)width;
    height = (uint32)((sint32)(sint16)(value >> 16) * 240);
    height = (sint32)height / 4096;
    rectangle.x = (sint16)((sint32)(320 - width) >> 1);
    rectangle.h = (sint16)height;
    rectangle.y = (sint16)((sint32)(240 - height) >> 1);
    result = sub_800847a8(object, packet, 0, inverse << 1, &rectangle);
    queue = r_u32(0x800BCBE0);
    value = r_u32(queue + 56);
    w_u32(queue + 56, value + 1);
    return result;
}

uint32 sub_80038054(uint32 index, uint32 offset)
{
    uint32 game, value;
    FUNCTION_MARKER(0x80038054, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u8(0x800B0DA0 + offset + (index << 2));
    w_u32(game + 4152, value);
    return value;
}

uint32 sub_80038078(uint32 index, uint32 offset)
{
    uint32 game, value;
    FUNCTION_MARKER(0x80038078, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    value = r_u8(0x800B0E94 + offset + (index << 1));
    w_u8(game + 936, value);
    return value;
}

uint32 sub_80039420(void)
{
    FUNCTION_MARKER(0x80039420, "ACE3.BIN");
    w_u32(0x800BCF78, 1);
    return 1;
}

uint32 sub_8003a07c(void)
{
    FUNCTION_MARKER(0x8003A07C, "ACE3.BIN");
    w_u8(0x800C6D69, 255);
    w_u8(0x800C6D6F, 0);
    w_u16(0x800C6D66, 0);
    w_u8(0x800C6D68, 0);
    w_u8(0x800C6D6A, 0);
    w_u8(0x800C6D6B, 0);
    w_u8(0x800C6D6C, 0x83);
    w_u8(0x800C6D6D, 0);
    w_u8(0x800C6D71, 0);
    return 0x800C6D60;
}

uint32 sub_800428f8(void)
{
    FUNCTION_MARKER(0x800428F8, "ACE3.BIN");
    return sub_80041f88(0x800BCFD8);
}

uint32 sub_80045148(uint32 vector)
{
    uint32 first, second, third, result;
    FUNCTION_MARKER(0x80045148, "ACE3.BIN");
    second = r_u32(vector + 4);
    first = r_u32(vector);
    if ((sint32)second < 0)
        second = 0 - second;
    if ((sint32)first < 0)
        first = 0 - first;
    if ((sint32)second < (sint32)first)
        second = first;
    third = r_u32(vector + 8);
    result = sub_80030d54(second, third);
    if ((sint32)result < 0)
        result = 0 - result;
    return result;
}

uint32 sub_8004596c(void)
{
    uint32 index, slot, flags, table, object;
    FUNCTION_MARKER(0x8004596C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        flags = r_u32(0x800C6FD0 + (index << 2));
        if ((flags & 2) != 0)
            sub_80038320(index);
    }
    for (index = 2; index < 64; ++index)
    {
        slot = 0x800C6FD0 + (index << 2);
        table = r_u32(0x800BCE5C);
        flags = r_u32(slot);
        object = r_u32(table + (index << 2));
        if ((flags & 4) != 0)
        {
            w_u8(0x800C70D0 + index, 0);
            flags = r_u32(slot);
            w_u32(slot, flags & 0xFFFFFFFB);
            flags = r_u32(object);
            w_u32(object, flags & 0xFFFFBFFF);
        }
    }
    return 0;
}

uint32 sub_80048a80(void)
{
    uint32 game, count, entry, value;
    FUNCTION_MARKER(0x80048A80, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    count = r_u8(game + 962);
    if (count == 0)
        return 0xFFFFFFFF;
    entry = game + 1864 + ((count - 1) << 3);
    do
    {
        value = r_u8(entry + 1);
        --count;
        if (value == 0)
            return r_u8(entry + 6);
        entry -= 8;
    } while (count != 0);
    return 0xFFFFFFFF;
}

uint32 sub_80049c4c(uint32 index)
{
    uint32 table, descriptors, object, flags, slot, data, controls, state, phase, target, current, x, z, angle, difference, signed_difference, magnitude, limit, step, result;
    FUNCTION_MARKER(0x80049C4C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    descriptors = r_u32(0x800BCEF8);
    object = r_u32(table + (index << 2));
    slot = 0x800C6FD0 + (index << 2);
    flags = r_u32(slot);
    data = r_u32(descriptors + (index << 6) + 20);
    controls = r_u32(0x800BCEC0);
    w_u32(slot, flags | 32);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    if (phase == 0)
    {
        w_u16(object + 52, 0);
        w_u16(object + 50, 0);
        w_u16(object + 48, 0);
        w_u16(state + 132, 0);
        w_u16(state + 130, 0);
        w_u16(state + 128, 0);
        w_u8(state + 100, 1);
        return 1;
    }
    if (phase == 1)
    {
        target = r_u32(state + 104);
        current = r_u32(object + 8);
        z = r_u32(state + 112);
        x = (sint32)(target - current) >> 12;
        current = r_u32(object + 16);
        z = (sint32)(z - current) >> 12;
        angle = sub_80030d54(x, z);
        current = r_u16(object + 26);
        difference = (angle - current) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        limit = r_s16(data + 30);
        magnitude = signed_difference;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        step = r_u16(data + 30);
        if ((sint32)magnitude < (sint32)limit)
            w_u16(object + 26, current + difference);
        else if ((sint32)signed_difference > 0)
            w_u16(object + 26, current + step);
        else if ((sint32)signed_difference < 0)
            w_u16(object + 26, current - step);
        target = r_u32(state + 108);
        current = r_u32(object + 12);
        difference = target - current;
        if ((sint32)difference > 0x5000)
            result = current + 0x5000;
        else if ((sint32)difference < -0x5000)
            result = current - 0x5000;
        else
            result = current + difference;
        w_u32(object + 12, result);
        return result;
    }
    if (phase == 2)
    {
        result = sub_80044034(index);
        if (result == 0)
            w_u8(state + 100, 0);
        return result;
    }
    if (phase == 3)
    {
        result = sub_80044034(index);
        if (result == 0)
            w_u8(state + 99, 255);
        w_u8(state + 100, 0);
        return 255;
    }
    w_u8(state + 100, 0);
    return 3;
}

uint32 sub_80049ec4(uint32 index)
{
    uint32 offset, controls, state, phase;
    FUNCTION_MARKER(0x80049EC4, "ACE3.BIN");
    offset = index * 184;
    controls = r_u32(0x800BCEC0);
    state = controls + offset;
    phase = r_u8(state + 100);
    if (phase == 1)
        return offset;
    if (phase == 0)
    {
        w_u8(state + 100, 1);
        return 1;
    }
    if (phase == 2)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_80049f38(uint32 index)
{
    uint8 output[8];
    uint32 slot, flags, controls, state, phase, speed, result;
    FUNCTION_MARKER(0x80049F38, "ACE3.BIN");
    slot = 0x800C6FD0 + (index << 2);
    flags = r_u32(slot);
    w_u32(slot, flags | 32);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    w_u8(state + 97, 1);
    if (phase == 0)
    {
        w_u8(state + 100, 1);
        return 1;
    }
    if (phase == 1)
    {
        speed = r_s16(state + 156);
        return sub_80044188(index, state + 140, speed, 0, output, 0);
    }
    if (phase == 2)
    {
        result = sub_80044034(index);
        if (result == 0)
        {
            w_u8(state + 100, 3);
            result = 3;
        }
        return result;
    }
    if (phase == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004a0c0(uint32 index)
{
    uint32 controls, table, state, phase, object, x, z, target_x, target_z, angle, current, difference, signed_difference, value, first, second, result;
    FUNCTION_MARKER(0x8004A0C0, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    table = r_u32(0x800BCE5C);
    phase = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
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
    if (phase == 1)
    {
        target_x = r_u32(state + 104);
        x = r_u32(object + 8);
        target_z = r_u32(state + 112);
        z = r_u32(object + 16);
        x = (sint32)(target_x - x) >> 12;
        z = (sint32)(target_z - z) >> 12;
        angle = sub_80030d54(x, z);
        current = r_u16(object + 26);
        difference = (angle - current) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        if ((sint32)signed_difference > 64)
            value = current + 64;
        else if ((sint32)signed_difference < -64)
            value = current - 64;
        else
            value = current + difference;
        w_u16(object + 26, value);
        result = difference << 16;
        if (result == 0)
        {
            result = 2;
            w_u8(state + 100, result);
        }
        return result;
    }
    if (phase == 2)
    {
        angle = r_u16(object + 26);
        value = r_s16(0x800104AC + ((angle & 0x3FFF) << 1));
        first = value * 5;
        angle = r_u16(object + 26);
        first = (sint32)first >> 2;
        x = r_u32(object + 8);
        value = r_s16(0x800104AC + (((angle & 0x3FFF) + 4096) << 1));
        x += first;
        second = value * 5;
        z = r_u32(object + 16);
        second = (sint32)second >> 2;
        w_u32(object + 8, x);
        z += second;
        w_u32(object + 16, z);
        target_x = r_u32(state + 104);
        target_z = r_u32(state + 112);
        difference = (sint32)(target_x - x) >> 12;
        second = (sint32)(target_z - z) >> 12;
        if ((sint32)difference < 0)
            difference = 0 - difference;
        result = (sint32)difference < 5;
        if (result == 0)
            return result;
        if ((sint32)second < 0)
            second = 0 - second;
        result = (sint32)second < 5;
        if (result == 0)
            return result;
        w_u32(object + 8, target_x);
        target_z = r_u32(state + 112);
        w_u32(object + 16, target_z);
        w_u8(state + 100, 3);
        return 3;
    }
    if (phase == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004a2d8(uint32 index)
{
    uint32 controls, state, phase, result;
    FUNCTION_MARKER(0x8004A2D8, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    if (phase == 0)
    {
        result = sub_80044034(index);
        if (result == 0)
        {
            result = 0x4000;
            w_u8(state + 100, 1);
            w_u16(state + 128, result);
        }
        return result;
    }
    if (phase == 1)
    {
        result = r_s16(state + 128);
        if (result == 0)
        {
            result = 2;
            w_u8(state + 100, result);
        }
        return result;
    }
    if (phase == 2)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004a394(uint32 index)
{
    uint32 controls, state, phase, result;
    FUNCTION_MARKER(0x8004A394, "ACE3.BIN");
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    if (phase == 0)
    {
        result = sub_80044034(index);
        if (result == 0)
        {
            result = 0x2000;
            w_u8(state + 100, 1);
            w_u16(state + 128, result);
        }
        return result;
    }
    if (phase == 1)
    {
        result = r_s16(state + 128);
        if (result == 0)
        {
            result = 2;
            w_u8(state + 100, result);
        }
        return result;
    }
    if (phase == 2)
    {
        result = sub_80044034(index);
        if (result == 0)
        {
            result = 3;
            w_u8(state + 100, result);
        }
        return result;
    }
    if (phase == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004a474(uint32 index)
{
    uint32 table, controls, state, phase, object, position, angle, value, result;
    FUNCTION_MARKER(0x8004A474, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        position = r_u32(object + 16);
        if ((sint32)position > 0)
        {
            angle = r_u16(object + 28);
            value = 0xFFFFC000 - angle;
        }
        else if ((sint32)position < 0)
        {
            angle = r_u16(object + 28);
            value = 0x4000 - angle;
        }
        else
        {
            position = r_u32(object + 8);
            value = (position & 1) != 0 ? 0xFFFFC000 : 0x4000;
        }
        w_u16(state + 132, value);
        w_u8(state + 100, 1);
    }
    else if (phase != 1)
    {
        if (phase == 2)
            w_u8(state + 99, 255);
        else
            w_u8(state + 100, 0);
        return 255;
    }
    result = r_s16(state + 132);
    if (result == 0)
    {
        result = 2;
        w_u8(state + 100, result);
    }
    return result;
}

uint32 sub_8004a56c(uint32 index)
{
    uint32 table, controls, state, phase, object, position, angle, value, flags, result;
    FUNCTION_MARKER(0x8004A56C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        position = r_u32(object + 16);
        if ((sint32)position > 0)
        {
            angle = r_u16(object + 28);
            value = 0xFFFFC000 - angle;
        }
        else if ((sint32)position < 0)
        {
            angle = r_u16(object + 28);
            value = 0x4000 - angle;
        }
        else
        {
            position = r_u32(object + 8);
            value = (position & 1) != 0 ? 0xFFFFC000 : 0x4000;
        }
        w_u16(state + 132, value);
        w_u8(state + 100, 1);
    }
    else if (phase != 1)
    {
        if (phase == 2)
            w_u8(state + 99, 255);
        else
            w_u8(state + 100, 0);
        return 255;
    }
    w_u32(object + 36, 0xFFFEB000);
    flags = r_u32(object + 84);
    w_u32(object + 84, flags | 0x200000);
    result = r_s16(state + 132);
    if (result == 0)
    {
        result = 2;
        w_u32(object + 36, 0);
        w_u8(state + 100, result);
    }
    return result;
}

uint32 sub_8004a7e8(uint32 index)
{
    uint32 table, controls, state, descriptors, phase, object, data, value, magnitude, step, remaining, angle, current, roll, signed_angle, velocity, limit, result;
    FUNCTION_MARKER(0x8004A7E8, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    descriptors = r_u32(0x800BCEF8);
    phase = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    data = r_u32(descriptors + (index << 6) + 20);
    if (phase >= 5)
    {
        w_u8(state + 100, 0);
        return 0x80020000;
    }
    r_u32(0x80024028 + (phase << 2));
    switch (phase)
    {
        case 0:
            w_u8(state + 97, 0);
            result = sub_80044034(index);
            if (result != 0)
                return result;
            w_u8(state + 97, 9);
            w_u32(object + 40, 0);
            w_u32(object + 36, 0);
            w_u32(object + 32, 0);
            value = r_u16(state + 124);
            w_u8(state + 100, 1);
            w_u16(state + 132, value);
            return 1;
        case 1:
            result = r_s16(state + 132);
            if (result == 0)
            {
                result = 2;
                w_u8(state + 100, result);
            }
            return result;
        case 2:
            value = r_u32(object + 56);
            magnitude = value;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude > 0xA000)
                w_u32(object + 56, (sint32)value >> 1);
            value = r_u32(object + 64);
            magnitude = value;
            if ((sint32)magnitude < 0)
                magnitude = 0 - magnitude;
            if ((sint32)magnitude > 0xA000)
                w_u32(object + 64, (sint32)value >> 1);
            step = r_s16(data + 26);
            step <<= 1;
            remaining = r_s16(state + 120);
            if ((sint32)remaining < (sint32)step)
                step = remaining;
            angle = r_u16(object + 24);
            w_u16(object + 24, angle + step);
            remaining = r_u16(state + 120);
            value = remaining - step;
            w_u16(state + 120, value);
            result = value << 16;
            if (result == 0)
            {
                result = 3;
                w_u8(state + 100, result);
            }
            return result;
        case 3:
            w_u8(state + 97, 10);
            signed_angle = r_s16(object + 24);
            current = r_u16(object + 24);
            if ((sint32)signed_angle > 0)
            {
                roll = r_s16(object + 28);
                if ((sint32)roll < 0)
                    roll = 0 - roll;
            }
            else
                roll = 4096;
            if ((sint32)signed_angle > 0 && (sint32)roll < 4096)
            {
                step = r_s16(data + 26);
                value = r_u16(data + 26);
                if ((sint32)step < (sint32)signed_angle)
                    w_u16(object + 24, current - value);
            }
            else
            {
                signed_angle = r_s16(object + 24);
                current = r_u16(object + 24);
                if ((sint32)signed_angle >= -4095)
                {
                    step = r_u16(data + 26);
                    w_u16(object + 24, current - step);
                }
            }
            velocity = r_u32(object + 40);
            limit = r_u32(data + 12);
            result = (sint32)velocity < (sint32)limit;
            if (result != 0)
                return result;
            value = r_s16(state + 122);
            if (value == 0)
            {
                result = sub_80044034(index);
                if (result != 0)
                    return result;
            }
            w_u8(state + 100, 4);
            return 4;
    }
    w_u8(state + 99, 255);
    return 255;
}

uint32 sub_8004aa3c(uint32 index)
{
    uint32 table, controls, state, game, mission, object, duration, speed, phase, counter, numerator, angle, sine, cosine, x, z, remaining, roll, result, height;
    FUNCTION_MARKER(0x8004AA3C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    game = r_u32(0x800BCE60);
    mission = r_s16(game + 908);
    object = r_u32(table + (index << 2));
    duration = mission == 26 ? 2700 : 7200;
    speed = mission == 26 ? 0x5000 : 0x4000;
    phase = r_u8(state + 100);
    w_u8(state + 97, 9);
    if (phase == 0)
    {
        w_u32(object + 8, 0xFDBE0000);
        w_u32(object + 16, 0xF15CC000);
        w_u16(object + 24, 0);
        w_u16(object + 26, 0);
        w_u16(object + 28, 1024);
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u32(object + 32, 0);
        w_u32(object + 36, 0);
        w_u32(object + 40, 0);
        w_u32(object + 12, 0xFF59A000 - speed * duration);
        w_u8(state + 100, 1);
        return 1024;
    }
    if (phase == 1)
    {
        counter = r_s16(state + 102);
        numerator = counter << 15;
        angle = ((uint32)((sint32)numerator / (sint32)duration)) & 0x3FFF;
        sine = r_s16(0x800104AC + (angle << 1));
        cosine = r_s16(0x800104AC + ((angle + 4096) << 1));
        x = 0xFDBE0000 - (sine << 11);
        z = 0xF35CC000 - (cosine << 11);
        w_u32(object + 8, x);
        w_u32(object + 16, z);
        counter = r_s16(state + 102);
        remaining = duration - counter;
        if (counter == duration)
        {
            w_u32(object + 12, 0xFF59A000);
            w_u8(state + 100, 2);
        }
        else
            w_u32(object + 12, 0xFF59A000 - speed * remaining);
        w_u16(object + 26, angle - 4096);
        counter = r_s16(state + 102);
        roll = r_s16(object + 28);
        remaining = duration - counter;
        result = (sint32)remaining < (sint32)roll;
        if (result != 0)
            w_u16(object + 28, remaining);
        return result;
    }
    if (phase == 2)
    {
        counter = r_s16(state + 102);
        height = r_u32(object + 12);
        remaining = 304 - (counter - duration);
        x = r_u32(object + 8);
        w_u32(object + 8, x - (remaining << 9));
        if ((sint32)height > (sint32)0xFFB9A000)
            w_u32(object + 12, 0xFFB9A000);
        else if ((sint32)height < (sint32)0xFFB9A000)
            w_u32(object + 12, height + 0x8000);
        w_u32(object + 16, 0xF15CC000);
        if (remaining == 0)
        {
            w_u8(state + 100, 3);
            return 3;
        }
        return 0xF15CC000;
    }
    if (phase != 3)
        w_u8(state + 100, 0);
    return 3;
}

uint32 sub_8004ac90(uint32 index)
{
    uint32 table, controls, state, descriptors, object, phase, data, value, velocity, decrement, position, result;
    FUNCTION_MARKER(0x8004AC90, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    descriptors = r_u32(0x800BCEF8);
    object = r_u32(table + (index << 2));
    phase = r_u8(state + 100);
    data = r_u32(descriptors + (index << 6) + 20);
    w_u8(state + 97, 9);
    if (phase == 0)
    {
        w_u32(object + 56, 0);
        w_u32(object + 60, 0);
        w_u32(object + 64, 0);
        w_u32(object + 32, 0);
        w_u32(object + 36, 0);
        w_u32(object + 40, 0);
        w_u8(state + 100, 1);
        return 1;
    }
    if (phase == 1)
    {
        value = r_u32(data + 4);
        velocity = r_u32(object + 60);
        value = 0 - value;
        if ((sint32)value < (sint32)velocity)
        {
            decrement = r_u32(data + 16);
            w_u32(object + 60, velocity - decrement);
        }
        position = r_u32(object + 12);
        result = (sint32)position < (sint32)0xE7000000;
        if (result != 0)
        {
            result = 2;
            w_u8(state + 100, result);
        }
        return result;
    }
    if (phase == 2)
    {
        w_u32(object + 60, 0);
        return 3;
    }
    if (phase == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004ad98(uint32 index)
{
    uint32 table, controls, state, object, source, phase, i, slot, factor, velocity, position, value, first, second, third, fourth, game, flag, result;
    FUNCTION_MARKER(0x8004AD98, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    object = r_u32(table + (index << 2));
    source = r_u32(table);
    phase = r_u8(state + 100);
    w_u8(state + 97, 9);
    if (phase == 0)
    {
        for (i = 0; i < 64; ++i)
        {
            slot = 0x800C71A0 + i * 24;
            factor = 64 - i;
            velocity = r_u32(source + 56);
            position = r_u32(source + 8);
            w_u32(slot, position - velocity * factor);
            velocity = r_u32(source + 60);
            position = r_u32(source + 12);
            w_u32(slot + 4, position - velocity * factor);
            velocity = r_u32(source + 64);
            position = r_u32(source + 16);
            w_u32(slot + 8, position - velocity * factor);
            r_masked_u32(source + 24, 15);
            first = r_masked_u32(source + 24, 15);
            r_masked_u32(source + 28, 15);
            second = r_masked_u32(source + 28, 15);
            w_masked_u32(slot + 16, first, 15);
            w_masked_u32(slot + 16, first, 15);
            w_masked_u32(slot + 20, second, 15);
            w_masked_u32(slot + 20, second, 15);
            w_u8(0x800C77A0 + i, 1);
        }
        w_u8(state + 100, 1);
        return 1;
    }
    if (phase == 1)
    {
        game = r_u32(0x800BCE60);
        i = r_u32(game + 36) & 63;
        slot = 0x800C71A0 + i * 24;
        flag = 0x800C77A0 + i;
        first = r_u32(slot);
        second = r_u32(slot + 4);
        third = r_u32(slot + 8);
        fourth = r_u32(slot + 12);
        w_u32(object + 8, first);
        w_u32(object + 12, second);
        w_u32(object + 16, third);
        w_u32(object + 20, fourth);
        first = r_u32(slot + 16);
        second = r_u32(slot + 20);
        w_u32(object + 24, first);
        w_u32(object + 28, second);
        value = r_u8(flag);
        if (value == 0)
            w_u16(object + 78, 0);
        first = r_u32(source + 8);
        second = r_u32(source + 12);
        third = r_u32(source + 16);
        fourth = r_u32(source + 20);
        w_u32(slot, first);
        w_u32(slot + 4, second);
        w_u32(slot + 8, third);
        w_u32(slot + 12, fourth);
        first = r_u32(source + 24);
        second = r_u32(source + 28);
        w_u32(slot + 16, first);
        w_u32(slot + 20, second);
        result = r_s16(source + 78);
        w_u8(flag, (sint32)result > 0);
        first = r_u32(source + 56);
        second = r_u32(source + 60);
        third = r_u32(source + 64);
        fourth = r_u32(source + 68);
        w_u32(object + 56, first);
        w_u32(object + 60, second);
        w_u32(object + 64, third);
        w_u32(object + 68, fourth);
        return result;
    }
    if (phase == 2)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004b108(uint32 index)
{
    uint32 table, controls, state, phase, object, game, mission, x, z, angle, current, difference, signed_difference, roll, value, result;
    FUNCTION_MARKER(0x8004B108, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    phase = r_u8(state + 100);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        x = r_u32(object + 8);
        x = (sint32)x >> 12;
        if (mission == 32 || mission == 47 || mission == 53)
            x += 0xE000;
        z = r_u32(object + 16);
        z = (sint32)z >> 12;
        if (mission == 32 || mission == 47 || mission == 53)
            z += 0x2000;
        angle = sub_80030d54(x, z);
        current = r_u16(object + 26);
        difference = (angle - current - 0x2000) & 0x3FFF;
        if (difference >= 0x2000)
            difference |= 0xC000;
        signed_difference = (sint16)difference;
        w_u16(state + 120, difference);
        w_u16(state + 130, 0);
        w_u16(state + 128, 0);
        if ((sint32)signed_difference > 0)
        {
            roll = r_u16(object + 28);
            w_u16(state + 132, 0x1000 - roll);
        }
        else if ((sint32)signed_difference < 0)
        {
            roll = r_u16(object + 28);
            w_u16(state + 132, 0xFFFFF000 - roll);
        }
        else
            w_u16(state + 132, 0);
        value = r_u16(state + 132);
        value &= 0x3FFF;
        w_u16(state + 132, value);
        if (value >= 0x2000)
            w_u16(state + 132, value - 0x4000);
        w_u8(state + 100, 1);
        w_u8(state + 97, 2);
        return 2;
    }
    if (phase == 1)
    {
        value = r_s16(state + 132);
        if (value == 0)
        {
            value = r_u16(state + 120);
            w_u8(state + 100, 2);
            w_u16(state + 128, value);
        }
        w_u8(state + 97, 2);
        return 2;
    }
    if (phase == 2)
    {
        result = r_s16(state + 128);
        w_u8(state + 97, 2);
        if (result == 0)
        {
            result = 3;
            w_u8(state + 100, result);
            w_u8(state + 97, 0);
        }
        return result;
    }
    if (phase == 3)
    {
        w_u8(state + 97, 0);
        w_u8(state + 99, 255);
    }
    else
        w_u8(state + 100, 0);
    return 255;
}

uint32 sub_8004b88c(uint32 index)
{
    uint32 table, controls, state, descriptors, descriptor, object, phase, data, value, numerator, divisor, quotient, increment, other, position, result;
    FUNCTION_MARKER(0x8004B88C, "ACE3.BIN");
    table = r_u32(0x800BCE5C);
    controls = r_u32(0x800BCEC0);
    state = controls + index * 184;
    descriptors = r_u32(0x800BCEF8);
    descriptor = descriptors + (index << 6);
    object = r_u32(table + (index << 2));
    phase = r_u8(state + 100);
    data = r_u32(descriptor + 20);
    if (phase == 0)
    {
        w_u32(object + 64, 0);
        w_u32(object + 60, 0);
        w_u32(object + 56, 0);
        w_u32(object + 40, 0);
        w_u32(object + 36, 0);
        w_u32(object + 32, 0);
        numerator = r_u32(state + 112);
        divisor = r_u32(data + 12);
        numerator <<= 1;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)numerator / (sint32)divisor;
        if (quotient == 0)
            xport_mips_break(7);
        if (quotient == 0xFFFFFFFF && divisor == 0x80000000)
            xport_mips_break(6);
        increment = (sint32)divisor / (sint32)quotient;
        w_u8(state + 100, 1);
        w_u32(state + 20, increment);
    }
    else if (phase == 1)
    {
        value = r_u32(object + 40);
        increment = r_u32(state + 20);
        value += increment;
        w_u32(object + 40, value);
        divisor = r_u32(data + 12);
        if ((sint32)value >= (sint32)divisor)
        {
            w_u8(state + 100, 2);
            w_u16(state + 128, 1024);
        }
    }
    else if (phase == 2)
    {
        other = r_u32(descriptor + 24);
        position = r_u32(object + 12);
        value = r_u32(other + 8);
        if ((sint32)position < (sint32)value)
            w_u8(state + 100, 3);
    }
    else if (phase == 3)
        w_u8(state + 99, 255);
    else
        w_u8(state + 100, 0);
    result = r_u32(object + 84) | 0x200000;
    w_u32(object + 84, result);
    return result;
}

uint32 sub_8004bda8(uint32 index)
{
    uint32 states, groups, slot, group, value, count, member, flags, cursor, result;
    FUNCTION_MARKER(0x8004BDA8, "ACE3.BIN");
    states = r_u32(0x800BCEF4);
    groups = r_u32(0x800BCEEC);
    slot = states + index * 12;
    value = r_u32(slot);
    group = groups + index * 12;
    w_u32(slot, value & 0xFFFFFFFE);
    count = r_u16(group + 2);
    if (count == 0)
        return count;
    cursor = 0;
    do
    {
        member = r_u8(group + cursor + 4);
        slot = 0x800C6FD0 + (member << 2);
        flags = r_u32(slot);
        w_u32(slot, flags & 0xFFEFFFFF);
        count = r_u16(group + 2);
        ++cursor;
        result = group + cursor;
    } while ((sint32)cursor < (sint32)count);
    return result;
}

uint32 sub_8004c2c8(uint32 index)
{
    uint32 states, slot, flags, records, record, member, result;
    FUNCTION_MARKER(0x8004C2C8, "ACE3.BIN");
    states = r_u32(0x800BCF18);
    slot = states + (index << 2);
    flags = r_u32(slot);
    w_u32(slot, flags | 1);
    records = r_u32(0x800BCF0C);
    record = records + index * 20;
    member = r_u8(record + 2);
    slot = 0x800C6FD0 + (member << 2);
    flags = r_u32(slot);
    w_u32(slot, flags | 0x200000);
    member = r_u8(record + 3);
    slot = 0x800C6FD0 + (member << 2);
    result = r_u32(slot) | 0x400000;
    w_u32(slot, result);
    return result;
}

uint32 sub_8004c338(uint32 index)
{
    uint32 states, slot, flags, records, record, member, result;
    FUNCTION_MARKER(0x8004C338, "ACE3.BIN");
    states = r_u32(0x800BCF18);
    slot = states + (index << 2);
    flags = r_u32(slot);
    result = flags & 1;
    if (result == 0)
        return result;
    w_u32(slot, flags & 0xFFFFFFFE);
    records = r_u32(0x800BCF0C);
    record = records + index * 20;
    member = r_u8(record + 2);
    slot = 0x800C6FD0 + (member << 2);
    flags = r_u32(slot);
    w_u32(slot, flags & 0xFFDFFFFF);
    member = r_u8(record + 3);
    slot = 0x800C6FD0 + (member << 2);
    result = r_u32(slot) & 0xFFBFFFFF;
    w_u32(slot, result);
    return result;
}

uint32 sub_8004dc38(void)
{
    uint32 selection, record, kind, game, mission, clear_objects, table, cursor, slot, index, object, flags, member, controls, state, source, destination, first, second, third, fourth, value, mode, snapshots;
    FUNCTION_MARKER(0x8004DC38, "ACE3.BIN");
    selection = r_u32(0x800BCEA8);
    record = 0x800B1FC4 + (selection << 2);
    kind = r_u8(record);
    clear_objects = kind == 0 || kind == 1 || kind == 2 || kind == 3;
    if (kind == 7)
    {
        game = r_u32(0x800BCE60);
        mission = r_s16(game + 908);
        clear_objects = mission != 7;
    }
    if (clear_objects != 0)
    {
        table = r_u32(0x800BCE5C);
        cursor = table + 8;
        slot = 0x800C6FD8;
        for (index = 2; index < 32; ++index)
        {
            object = r_u32(cursor);
            flags = r_u32(object);
            if ((flags & 64) != 0 && (r_u32(slot) & 0x400000) != 0)
                value = flags & 0xFFFFFBFF;
            else
                value = r_u32(object) & 0xFFFFBFFF;
            w_u32(object, value);
            slot += 4;
            cursor += 4;
        }
    }
    for (index = 0; index < 2; ++index)
    {
        member = r_u8(record + index + 2);
        if (member == 0)
            continue;
        table = r_u32(0x800BCE5C);
        object = r_u32(table + (member << 2));
        flags = r_u32(object);
        controls = r_u32(0x800BCEC0);
        w_u32(object, flags | 0x4000);
        state = controls + member * 184;
        sub_8004e0fc(member);
        sub_8004e124(member);
        w_u8(state + 28, 8);
        w_u8(state + 29, 0);
        w_u8(state + 30, 0);
        w_u32(state + 56, 0);
        w_u8(state + 62, 0);
        w_u8(state + 100, 0);
        slot = 0x800C6FD0 + (member << 2);
        flags = r_u32(slot);
        w_u32(slot, flags & 0xFFEFFFFF);
        w_u16(state + 4, 0);
        w_u16(state + 6, 0);
        w_u16(state + 8, 0);
        w_u32(state + 12, 0);
        w_u32(state + 16, 0);
        w_u32(state + 20, 0);
        w_u16(state + 128, 0);
        w_u16(state + 130, 0);
        w_u16(state + 132, 0);
        kind = r_u8(record);
        if (kind - 2 >= 6)
            continue;
        r_u32(0x800241BC + ((kind - 2) << 2));
        if (kind == 2 || kind == 3)
        {
            table = r_u32(0x800BCE5C);
            game = r_u32(0x800BCE60);
            destination = r_u32(table + (member << 2));
            source = r_u32(table);
            first = r_u32(source + 8);
            second = r_u32(source + 12);
            third = r_u32(source + 16);
            fourth = r_u32(source + 20);
            w_u32(destination + 8, first);
            w_u32(destination + 12, second);
            w_u32(destination + 16, third);
            w_u32(destination + 20, fourth);
            value = r_u32(destination + 16);
            w_u32(destination + 16, value + 0x01000000);
            value = r_u32(destination + 12);
            w_u32(destination + 12, value - 0x100000);
            mode = r_u32(game + 916);
            if ((sint32)mode >= 2)
            {
                value = game_random() & 1;
                first = r_u32(destination + 8);
                w_u32(destination + 8, value != 0 ? first + 0x400000 : first - 0x400000);
            }
            r_masked_u32(source + 24, 15);
            first = r_masked_u32(source + 24, 15);
            r_masked_u32(source + 28, 15);
            second = r_masked_u32(source + 28, 15);
            w_masked_u32(destination + 24, first, 15);
            w_masked_u32(destination + 24, first, 15);
            w_masked_u32(destination + 28, second, 15);
            w_masked_u32(destination + 28, second, 15);
            first = r_u32(source + 56);
            second = r_u32(source + 60);
            third = r_u32(source + 64);
            fourth = r_u32(source + 68);
            w_u32(destination + 56, first);
            w_u32(destination + 60, second);
            w_u32(destination + 64, third);
            w_u32(destination + 68, fourth);
            value = r_u32(source + 64);
            w_u32(destination + 40, value);
            w_u8(state + 61, 19);
        }
        else if (kind == 5 || kind == 6)
        {
            snapshots = r_u32(0x800BCF00);
            source = snapshots + member * 44;
            first = r_u32(source + 4);
            second = r_u32(source + 8);
            third = r_u32(source + 12);
            fourth = r_u32(source + 16);
            w_u32(object + 8, first);
            w_u32(object + 12, second);
            w_u32(object + 16, third);
            w_u32(object + 20, fourth);
            first = r_u32(source + 20);
            second = r_u32(source + 24);
            w_u32(object + 24, first);
            w_u32(object + 28, second);
            w_u8(state + 61, 20);
        }
        else if (kind == 7)
        {
            table = r_u32(0x800BCE5C);
            destination = r_u32(table + (member << 2));
            source = r_u32(table);
            first = r_u32(source + 8);
            second = r_u32(source + 12);
            third = r_u32(source + 16);
            fourth = r_u32(source + 20);
            w_u32(destination + 8, first);
            w_u32(destination + 12, second);
            w_u32(destination + 16, third);
            w_u32(destination + 20, fourth);
            value = r_u32(source + 56);
            first = r_u32(destination + 8);
            w_u32(destination + 8, first + value * 150);
            value = r_u32(source + 64);
            first = r_u32(destination + 16);
            w_u32(destination + 16, first + value * 150);
            r_masked_u32(source + 24, 15);
            first = r_masked_u32(source + 24, 15);
            r_masked_u32(source + 28, 15);
            second = r_masked_u32(source + 28, 15);
            w_masked_u32(destination + 24, first, 15);
            w_masked_u32(destination + 24, first, 15);
            w_masked_u32(destination + 28, second, 15);
            w_masked_u32(destination + 28, second, 15);
            w_u8(state + 61, index == 0 ? 21 : 22);
        }
    }
    return 0;
}

uint32 sub_8004e060(void)
{
    uint32 selection, record, index, member, descriptors, controls, pointer, state, kind, flags;
    FUNCTION_MARKER(0x8004E060, "ACE3.BIN");
    selection = r_u32(0x800BCEA8);
    record = 0x800B1FC4 + (selection << 2);
    for (index = 0; index < 2; ++index)
    {
        member = r_u8(record + index + 2);
        if (member == 0)
            continue;
        descriptors = r_u32(0x800BCEF8);
        controls = r_u32(0x800BCEC0);
        pointer = r_u32(descriptors + (member << 6) + 48);
        state = controls + member * 184;
        kind = r_u8(pointer);
        flags = r_u32(state);
        w_u8(state + 29, 0);
        w_u8(state + 30, 0);
        w_u32(state + 56, 0);
        w_u8(state + 61, 255);
        w_u8(state + 60, 255);
        w_u32(state, flags | 0x200000);
        w_u8(state + 28, kind);
    }
    return 0;
}
