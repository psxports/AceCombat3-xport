#include "game_functions.h"

/* Complete draft bodies; runtime equivalence remains unverified */
uint32 sub_8006c20c(uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 fifth, uint32 sixth, uint8 *const *native_outputs)
{
    uint32 outputs[6], offsets[6], scene, root, camera, camera_x, camera_y, camera_z, limit, bitmap, group, bit, mask, record, x, y, z, a, b, c, magnitude, sum, shift, flags, list, target, value, depth, tag, i;
    FUNCTION_MARKER(0x8006C20C, "ACE3.BIN");
    outputs[0] = first;
    outputs[1] = second;
    outputs[2] = third;
    outputs[3] = fourth;
    outputs[4] = fifth;
    outputs[5] = sixth;
    for (i = 0; i < 6; ++i)
        offsets[i] = 0;
    scene = r_u32(0x800BCE54);
    root = r_u32(0x800BD080);
    camera = r_u32(scene + 40);
    bitmap = root + 0x33D0;
    camera_x = r_u32(camera + 4);
    camera_y = r_u32(camera + 8);
    value = r_s16(camera + 36);
    camera_z = r_u32(camera + 12);
    limit = (value << 2) + (value << 3);
    for (group = 0; group < 128; group += 32, bitmap += 4)
    {
        mask = r_u32(bitmap);
        if (mask == 0)
            continue;
        for (bit = 0; bit < 32; ++bit)
        {
            if ((mask & (1u << bit)) == 0)
                continue;
            root = r_u32(0x800BD080);
            record = root + 0x15D0 + (group + bit) * 60;
            a = r_u32(record + 8);
            b = r_u32(record + 12);
            x = (sint32)(a - camera_x) >> 12;
            value = b - camera_y;
            c = r_u32(record + 16);
            z = (sint32)(c - camera_z) >> 12;
            a = (sint32)x < 0 ? 0 - x : x;
            y = (sint32)value >> 12;
            if ((sint32)a > 32767)
                continue;
            b = (sint32)y < 0 ? 0 - y : y;
            if ((sint32)b > 32767)
                continue;
            c = (sint32)z < 0 ? 0 - z : z;
            if ((sint32)c > 32767)
                continue;
            sum = a + b + c;
            shift = 0;
            if ((sint32)limit >= (sint32)sum)
                shift = ((sint32)limit >> 1) < (sint32)sum ? 1 : 2;
            x <<= shift;
            y <<= shift;
            z <<= shift;
            xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(1, z);
            a = r_u32(record + 24);
            b = r_u32(record + 28);
            x = (sint32)(a - camera_x) >> 12;
            value = b - camera_y;
            c = r_u32(record + 32);
            z = (sint32)(c - camera_z) >> 12;
            magnitude = (sint32)x < 0 ? 0 - x : x;
            y = (sint32)value >> 12;
            if ((sint32)magnitude > 32767)
                continue;
            magnitude = (sint32)y < 0 ? 0 - y : y;
            if ((sint32)magnitude > 32767)
                continue;
            magnitude = (sint32)z < 0 ? 0 - z : z;
            if ((sint32)magnitude > 32767)
                continue;
            x <<= shift;
            y <<= shift;
            z <<= shift;
            xport_gte_write_data(2, (x & 0xFFFF) | (y << 16));
            xport_gte_write_data(3, z);
            xport_gte_execute(0x280030);
            flags = r_u32(record);
            if ((flags & 0x10) != 0)
                list = 0;
            else if ((flags & 0x20) != 0)
                list = 1;
            else if ((flags & 0x40) != 0)
                list = 2;
            else if ((flags & 0x80) != 0)
                list = 3;
            else if ((flags & 0x100) != 0)
                list = 4;
            else
                list = 5;
            target = outputs[list] + offsets[list];
            value = offsets[list];
            offsets[list] += 16;
            tag = r_u32(record + 4);
            depth = xport_gte_read_data(17);
            if (depth - 1 >= 0x77F7)
                tag = 0;
            depth = (sint32)depth >> shift;
            x = xport_gte_read_data(12);
            if (native_outputs != 0)
            {
                xport_store_le32(native_outputs[list] + value, x);
                xport_store_le16(native_outputs[list] + value + 8, depth);
            }
            else
            {
                w_u32(target, x);
                w_u16(target + 8, depth);
            }
            xport_gte_read_data(12);
            depth = xport_gte_read_data(18);
            if (depth - 1 >= 0x77F7)
                tag = 0;
            depth = (sint32)depth >> shift;
            x = xport_gte_read_data(13);
            if (native_outputs != 0)
            {
                xport_store_le32(native_outputs[list] + value + 4, x);
                xport_store_le16(native_outputs[list] + value + 10, depth);
                xport_store_le32(native_outputs[list] + value + 12, tag);
            }
            else
            {
                w_u32(target + 4, x);
                w_u16(target + 10, depth);
                w_u32(target + 12, tag);
            }
        }
    }
    for (i = 0; i < 6; ++i)
    {
        if (native_outputs != 0)
            xport_store_le32(native_outputs[i] + offsets[i] + 12, 0xFFFFFFFF);
        else
            w_u32(outputs[i] + offsets[i] + 12, 0xFFFFFFFF);
    }
    return 0xFFFFFFFF;
}

uint32 sub_800911b0(uint32 input, uint32 ordering, uint32 packet, const uint8 *native_input)
{
    uint32 cursor, tag, scene, camera, value, near_limit, scale, depth, packed, x, y, magnitude, bucket, size, half;
    FUNCTION_MARKER(0x800911B0, "ACE3.BIN");
    cursor = 12;
    tag = native_input != 0 ? xport_load_le32(native_input + cursor) : r_u32(input + cursor);
    if ((sint32)tag < 0)
        return packet;
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    value = (uint32)(sint32)(sint16)value;
    scale = ((value << 2) + value) << 2;
    near_limit = (sint32)value >> 1;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    do
    {
        if (tag != 0)
        {
            depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor - 2) : (uint32)r_s16(input + cursor - 2);
            packed = native_input != 0 ? xport_load_le32(native_input + cursor - 8) : r_u32(input + cursor - 8);
            y = (sint32)packed >> 16;
            x = (uint32)(sint32)(sint16)packed;
            if ((sint32)near_limit < (sint32)depth)
            {
                magnitude = (sint32)x < 0 ? 0 - x : x;
                if ((sint32)magnitude < 171)
                {
                    magnitude = (sint32)y < 0 ? 0 - y : y;
                    if ((sint32)magnitude < 131)
                    {
                        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
                        xport_gte_write_data(8, value);
                        bucket = ordering + ((uint32)((sint32)depth >> 3) << 2);
                        value = r_u32(bucket);
                        w_u32(packet, value | 0x09000000);
                        w_u32(bucket, packet & 0xFFFFFF);
                        xport_gte_execute(0x780010);
                        if (depth == 0)
                            xport_mips_break(7);
                        if (depth == 0xFFFFFFFF && scale == 0x80000000)
                            xport_mips_break(6);
                        size = (sint32)scale / (sint32)depth;
                        if ((sint32)size < 2)
                            size = 2;
                        half = (sint32)size >> 1;
                        x -= half;
                        y -= half;
                        w_u16(packet + 32, x + size);
                        w_u16(packet + 16, x + size);
                        w_u16(packet + 34, y + size);
                        w_u16(packet + 26, y + size);
                        w_u16(packet + 28, 0x7FE0);
                        w_u16(packet + 24, x);
                        w_u16(packet + 8, x);
                        w_u16(packet + 18, y);
                        w_u16(packet + 10, y);
                        w_u32(packet + 12, 0x451660E0);
                        w_u32(packet + 20, 0x003860FF);
                        w_u16(packet + 36, 0x7FFF);
                        value = xport_gte_read_data(22);
                        w_u32(packet + 4, value);
                        packet += 40;
                    }
                }
            }
        }
        cursor += 16;
        tag = native_input != 0 ? xport_load_le32(native_input + cursor) : r_u32(input + cursor);
    } while ((sint32)tag >= 0);
    return packet;
}

uint32 sub_80091370(uint32 input, uint32 ordering, uint32 packet, const uint8 *native_input)
{
    uint32 cursor, tag, scene, camera, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    FUNCTION_MARKER(0x80091370, "ACE3.BIN");
    cursor = 0;
    tag = native_input != 0 ? xport_load_le32(native_input + 12) : r_u32(input + 12);
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    value = (uint32)(sint32)(sint16)value;
    scale = ((value << 1) + value) << 1;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_91370;
        first_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 8) : (uint32)r_s16(input + cursor + 8);
        second_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 10) : (uint32)r_s16(input + cursor + 10);
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_91370;
        value = (sint32)tag < 14 ? (14 - tag) << 8 : 0;
        xport_gte_write_data(8, value);
        first_xy = native_input != 0 ? xport_load_le32(native_input + cursor) : r_u32(input + cursor);
        second_xy = native_input != 0 ? xport_load_le32(native_input + cursor + 4) : r_u32(input + cursor + 4);
        xport_gte_execute(0x780010);
        w_u32(0x1F80000C, first_xy);
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        w_u32(0x1F800008, second_xy);
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        w_u32(0x1F800010, second_width);
        w_u32(0x1F800014, first_width);
        value = xport_gte_read_data(22);
        w_u32(packet + 4, value);
        x = r_u16(0x1F800008);
        x = (uint32)(sint32)(sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_91370;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_91370;
        }
        y = r_u16(0x1F80000A);
        y = (uint32)(sint32)(sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_91370;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_91370;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        value = r_u32(bucket);
        w_u32(packet, value | 0x09000000);
        w_u32(bucket, packet & 0xFFFFFF);
        w_u16(packet + 28, 0x9FD0);
        w_u32(packet + 20, 0x003860DF);
        w_u32(packet + 12, 0x455660D0);
        w_u16(packet + 36, 0x9FDF);
        sub_8008de18(packet, 0x1F800000);
        packet += 40;
    next_record_91370:
        cursor += 16;
        tag = native_input != 0 ? xport_load_le32(native_input + cursor + 12) : r_u32(input + cursor + 12);
    }
    return packet;
}

uint32 sub_80090ed0(uint32 input, uint32 ordering, uint32 packet, const uint8 *native_input)
{
    uint32 cursor, tag, scene, camera, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    FUNCTION_MARKER(0x80090ED0, "ACE3.BIN");
    cursor = 0;
    tag = native_input != 0 ? xport_load_le32(native_input + 12) : r_u32(input + 12);
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    value = (uint32)(sint32)(sint16)value;
    scale = (value << 1) + value;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_90ed0;
        first_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 8) : (uint32)r_s16(input + cursor + 8);
        second_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 10) : (uint32)r_s16(input + cursor + 10);
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_90ed0;
        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
        xport_gte_write_data(8, value);
        first_xy = native_input != 0 ? xport_load_le32(native_input + cursor) : r_u32(input + cursor);
        second_xy = native_input != 0 ? xport_load_le32(native_input + cursor + 4) : r_u32(input + cursor + 4);
        xport_gte_execute(0x780010);
        w_u32(0x1F80000C, first_xy);
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        w_u32(0x1F800008, second_xy);
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        w_u32(0x1F800010, second_width);
        w_u32(0x1F800014, first_width);
        value = xport_gte_read_data(22);
        w_u32(packet + 4, value);
        x = r_u16(0x1F800008);
        x = (uint32)(sint32)(sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_90ed0;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_90ed0;
        }
        y = r_u16(0x1F80000A);
        y = (uint32)(sint32)(sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_90ed0;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_90ed0;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        value = r_u32(bucket);
        w_u32(packet, value | 0x09000000);
        w_u32(bucket, packet & 0xFFFFFF);
        w_u16(packet + 28, 0x9FC0);
        w_u32(packet + 20, 0x003860CF);
        w_u32(packet + 12, 0x44D660C0);
        w_u16(packet + 36, 0x9FCF);
        sub_8008de18(packet, 0x1F800000);
        packet += 40;
    next_record_90ed0:
        cursor += 16;
        tag = native_input != 0 ? xport_load_le32(native_input + cursor + 12) : r_u32(input + cursor + 12);
    }
    return packet;
}

uint32 sub_80090bf8(uint32 input, uint32 ordering, uint32 packet, const uint8 *native_input)
{
    uint32 cursor, tag, scene, camera, value, scale, near_limit, first_depth, second_depth, first_width, second_width, first_xy, second_xy, x, y, bucket;
    FUNCTION_MARKER(0x80090BF8, "ACE3.BIN");
    cursor = 0;
    tag = native_input != 0 ? xport_load_le32(native_input + 12) : r_u32(input + 12);
    if ((sint32)tag < 0)
        return packet;
    xport_gte_write_data(6, 0x2E838383);
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    scene = r_u32(0x800BCE54);
    camera = r_u32(scene + 40);
    value = r_u16(camera + 36);
    value = (uint32)(sint32)(sint16)value;
    scale = value << 1;
    near_limit = (sint32)value >> 2;
    while ((sint32)tag >= 0)
    {
        if (tag == 0)
            goto next_record_90bf8;
        first_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 8) : (uint32)r_s16(input + cursor + 8);
        second_depth = native_input != 0 ? (uint32)(sint32)(sint16)xport_load_le16(native_input + cursor + 10) : (uint32)r_s16(input + cursor + 10);
        if ((sint32)near_limit >= (sint32)first_depth || (sint32)near_limit >= (sint32)second_depth)
            goto next_record_90bf8;
        value = (sint32)tag < 28 ? (28 - tag) << 7 : 0;
        xport_gte_write_data(8, value);
        first_xy = native_input != 0 ? xport_load_le32(native_input + cursor) : r_u32(input + cursor);
        second_xy = native_input != 0 ? xport_load_le32(native_input + cursor + 4) : r_u32(input + cursor + 4);
        xport_gte_execute(0x780010);
        w_u32(0x1F80000C, first_xy);
        if (first_depth == 0)
            xport_mips_break(7);
        if (first_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        first_width = (sint32)scale / (sint32)first_depth;
        w_u32(0x1F800008, second_xy);
        if (second_depth == 0)
            xport_mips_break(7);
        if (second_depth == 0xFFFFFFFF && scale == 0x80000000)
            xport_mips_break(6);
        second_width = (sint32)scale / (sint32)second_depth;
        if ((sint32)first_width <= 0)
            first_width = 1;
        if ((sint32)second_width <= 0)
            second_width = 1;
        w_u32(0x1F800010, second_width);
        w_u32(0x1F800014, first_width);
        value = xport_gte_read_data(22);
        w_u32(packet + 4, value);
        x = r_u16(0x1F800008);
        x = (uint32)(sint32)(sint16)x;
        if ((sint32)(x + first_width) >= 161)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 161)
                goto next_record_90bf8;
        }
        if ((sint32)(x - first_width) < -160)
        {
            value = r_u16(0x1F80000C);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -160)
                goto next_record_90bf8;
        }
        y = r_u16(0x1F80000A);
        y = (uint32)(sint32)(sint16)y;
        if ((sint32)(y + first_width) >= 121)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value + second_width) >= 121)
                goto next_record_90bf8;
        }
        if ((sint32)(y - first_width) < -120)
        {
            value = r_u16(0x1F80000E);
            value = (uint32)(sint32)(sint16)value;
            if ((sint32)(value - second_width) < -120)
                goto next_record_90bf8;
        }
        bucket = ordering + ((uint32)((sint32)(first_depth + second_depth) >> 4) << 2);
        value = r_u32(bucket);
        w_u32(packet, value | 0x09000000);
        w_u32(bucket, packet & 0xFFFFFF);
        w_u16(packet + 28, 0x9FB0);
        w_u32(packet + 20, 0x003860BF);
        w_u32(packet + 12, 0x449660B0);
        w_u16(packet + 36, 0x9FBF);
        sub_8008de18(packet, 0x1F800000);
        packet += 40;
    next_record_90bf8:
        cursor += 16;
        tag = native_input != 0 ? xport_load_le32(native_input + cursor + 12) : r_u32(input + cursor + 12);
    }
    return packet;
}

uint32 sub_8006c5ec(uint32 ordering, uint32 packet)
{
    uint8 records[6][2064];
    uint8 *outputs[6];
    uint32 i;
    FUNCTION_MARKER(0x8006C5EC, "ACE3.BIN");
    for (i = 0; i < 6; ++i)
        outputs[i] = records[i];
    sub_8006c20c(0, 0, 0, 0, 0, 0, outputs);
    packet = sub_80090a74(0, ordering, packet, records[0]);
    packet = sub_80090bf8(0, ordering, packet, records[1]);
    packet = sub_80090ed0(0, ordering, packet, records[2]);
    packet = sub_800911b0(0, ordering, packet, records[3]);
    return sub_80091370(0, ordering, packet, records[5]);
}

uint32 sub_8008b7ac(uint32 effect, uint32 state)
{
    uint32 packet, table, index, threshold, strength, coefficient, value, product, type, color, uv, edge, palette, radius, link, corner, x, y, tail_seed, ordering, start, tail, address, lane, shift, merge, i, result;
    FUNCTION_MARKER(0x8008B7AC, "ACE3.BIN");
    packet = r_u32(state + 4);
    table = 0x800B440C;
    for (index = 0; index < 10; ++index, table += 8)
    {
        threshold = r_u8(table + 1);
        value = r_u32(effect + 16);
        strength = value - threshold;
        tail_seed = strength << 8;
        if ((sint32)strength <= 0)
            continue;
        coefficient = r_s16(table + 6);
        value = r_s16(effect + 8);
        product = value * coefficient;
        color = strength | (strength << 16) | (strength << 8);
        if ((sint32)product < 0)
            product += 0xFFF;
        value = r_s16(effect + 10);
        y = value * coefficient;
        w_u16(effect + 28, (sint32)product >> 12);
        if ((sint32)y < 0)
            y += 0xFFF;
        w_u16(effect + 30, (sint32)y >> 12);
        type = r_u8(table);
        if (type < 2)
        {
            uv = (type << 5) + 192;
            edge = (type << 5) + 223;
            color |= 0x2E000000;
            link = (packet << 8) + 0x2800;
            palette = r_u16(table + 2);
            w_u16(effect + 44, uv | 0x3F00);
            w_u32(effect + 40, edge | 0x382000);
            w_u16(effect + 46, edge | 0x3F00);
            w_u32(effect + 36, (palette << 16) | uv | 0x2000);
            radius = r_s16(table + 4);
            for (corner = 0; corner < 4; ++corner)
            {
                w_u32(packet, (link >> 8) | 0x09000000);
                w_u32(packet + 4, color);
                value = r_u32(effect + 36);
                w_u32(packet + 12, value);
                value = r_u32(effect + 40);
                w_u32(packet + 20, value);
                value = r_u16(effect + 44);
                w_u16(packet + 28, value);
                value = r_u16(effect + 46);
                w_u16(packet + 36, value);
                value = r_u32(effect + 28);
                w_u32(packet + 32, value);
                x = r_s16(effect + 28);
                value = (corner & 1) != 0 ? x + radius : x - radius;
                w_u16(packet + 24, value);
                value = r_u16(packet + 24);
                w_u16(packet + 8, value);
                value = r_u16(effect + 28);
                w_u16(packet + 16, value);
                y = r_s16(effect + 30);
                value = (corner >> 1) != 0 ? y + radius : y - radius;
                w_u16(packet + 18, value);
                link += 0x2800;
                value = r_u16(packet + 18);
                w_u16(packet + 10, value);
                value = r_u16(effect + 30);
                w_u16(packet + 26, value);
                packet += 40;
            }
            tail_seed = y;
        }
        else
        {
            uv = ((type & 1) << 4) + 224;
            edge = ((type & 1) << 4) + 239;
            w_u32(packet, ((packet + 40) & 0xFFFFFF) | 0x09000000);
            w_u32(packet + 4, color | 0x2E000000);
            palette = r_u16(table + 2);
            w_u32(packet + 20, edge | 0x380000);
            w_u16(packet + 28, uv | 0xF00);
            w_u16(packet + 36, edge | 0xF00);
            w_u32(packet + 12, (palette << 16) | uv);
            radius = r_s16(table + 4);
            value = r_u16(effect + 28);
            w_u16(packet + 8, value - radius);
            value = r_u16(effect + 30);
            w_u16(packet + 10, value - radius);
            value = r_u16(effect + 28);
            w_u16(packet + 16, value + radius);
            value = r_u16(effect + 30);
            w_u16(packet + 18, value - radius);
            value = r_u16(effect + 28);
            w_u16(packet + 24, value - radius);
            value = r_u16(effect + 30);
            w_u16(packet + 26, value + radius);
            value = r_u16(effect + 28);
            w_u16(packet + 32, value + radius);
            value = r_u16(effect + 30);
            w_u16(packet + 34, value + radius);
            packet += 40;
            tail_seed = edge;
        }
    }
    tail = packet - 40;
    ordering = r_u32(state);
    start = r_u32(state + 4);
    address = ordering + 2;
    lane = address & 3;
    merge = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
    if (lane != 3)
        merge |= tail_seed & (0xFFFFFF >> (lane << 3));
    result = start << 8;
    address = tail + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, merge >> (shift + (i << 3)));
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (i = 0; i <= lane; ++i)
        w_u8(address + i, result >> (shift + (i << 3)));
    w_u32(state + 4, packet);
    return result;
}

uint32 sub_8006466c(void)
{
    MATRIX first, second, third;
    uint16 angles[3];
    uint32 scene, table, object, a, b, c, d, e, x, y, z, p, q, r, u, v, w;
    FUNCTION_MARKER(0x8006466C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    object = r_u32(scene + 40);
    angles[0] = r_u16(object + 20);
    object = r_u32(scene + 40);
    angles[1] = r_u16(object + 22);
    object = r_u32(scene + 40);
    angles[2] = r_u16(object + 24);
    sub_800308f0(0, 0, &first, angles);
    table = r_u32(0x800BCE5C);
    object = r_u32(table);
    angles[0] = r_u16(object + 24);
    object = r_u32(table);
    angles[1] = r_u16(object + 26);
    object = r_u32(table);
    angles[2] = r_u16(object + 28);
    rotation_build(0, 0, &second, angles);
    angles[1] = 0;
    rotation_build(0, 0, &third, angles);
    xport_store_le32((uint8 *)&first + 20, 0);
    xport_store_le32((uint8 *)&third + 20, 0);
    xport_store_le32((uint8 *)&second + 20, 0);
    xport_store_le32((uint8 *)&first + 24, 0);
    xport_store_le32((uint8 *)&third + 24, 0);
    xport_store_le32((uint8 *)&second + 24, 0);
    xport_store_le32((uint8 *)&first + 28, 1024);
    xport_store_le32((uint8 *)&third + 28, 1024);
    xport_store_le32((uint8 *)&second + 28, 1024);
    a = xport_load_le32((uint8 *)&first);
    b = xport_load_le32((uint8 *)&first + 4);
    c = xport_load_le32((uint8 *)&first + 8);
    d = xport_load_le32((uint8 *)&first + 12);
    e = xport_load_le16((uint8 *)&first + 16);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = xport_load_le16((uint8 *)&second);
    b = xport_load_le32((uint8 *)&second + 4);
    c = xport_load_le32((uint8 *)&second + 12);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16((uint8 *)&second + 2);
    b = xport_load_le32((uint8 *)&second + 8);
    c = (uint32)(sint32)(sint16)xport_load_le16((uint8 *)&second + 14);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = xport_load_le16((uint8 *)&second + 4);
    b = xport_load_le32((uint8 *)&second + 8);
    c = xport_load_le16((uint8 *)&second + 16);
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
    a = xport_load_le32((uint8 *)&second + 20);
    b = xport_load_le32((uint8 *)&second + 24);
    c = xport_load_le32((uint8 *)&second + 28);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return sub_8006477c(160, 200);
}

uint32 sub_8008ba8c(uint32 state, uint32 flags, uint32 distance)
{
    uint32 packet, a, b, c, d, e, value, result, x, y, sum, numerator, high, ratio, remaining, size, color, link, i, scene, ordering, start, tail, lane, shift, seed, random;
    FUNCTION_MARKER(0x8008BA8C, "ACE3.BIN");
    a = r_u32(state + 8);
    b = r_u32(state + 12);
    w_u32(0x1F80001C, a);
    w_u32(0x1F800020, b);
    a = r_u32(0x1F80001C);
    xport_gte_write_data(0, a);
    b = r_u32(0x1F800020);
    xport_gte_write_data(1, b);
    xport_gte_execute(0x180001);
    packet = r_u32(0x1F800004);
    w_u32(packet + 20, 0x0039209F);
    w_u16(packet + 28, 0x3F80);
    w_u32(packet + 4, 0x2E808080);
    w_u32(packet + 12, 0x4F162080);
    w_u16(packet + 36, 0x3F9F);
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return 0x3F9F;
    value = xport_gte_read_data(14);
    w_u32(0x1F800024, value);
    x = r_s16(0x1F800024);
    if ((sint32)x < 0)
        x = 0 - x;
    if ((sint32)x >= 177)
        return 0;
    y = r_s16(0x1F800026);
    if ((sint32)y < 0)
        y = 0 - y;
    result = flags & 0xFF00;
    if ((sint32)y >= 137)
        return result;
    if (result != 0)
    {
        a = r_u32(state + 16);
        xport_gte_write_data(0, a);
        a = r_u32(state + 20);
        xport_gte_write_data(1, a);
        a = r_u32(state + 24);
        xport_gte_write_data(2, a);
        a = r_u32(state + 28);
        xport_gte_write_data(3, a);
        a = r_u32(state + 32);
        xport_gte_write_data(4, a);
        a = r_u32(state + 36);
        xport_gte_write_data(5, a);
        xport_gte_execute(0x280030);
        sum = x + y;
        numerator = (312 - sum) << 12;
        high = (uint32)(((sint64)(sint32)numerator * (sint64)(sint32)0xD20D20D3) >> 32);
        ratio = (uint32)((sint32)(high + numerator) >> 8) - (uint32)((sint32)numerator >> 31);
        w_u32(0x1F800028, ratio);
        value = xport_gte_read_data(12);
        w_u32(packet + 8, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 24, value);
        a = r_u32(state + 40);
        xport_gte_write_data(0, a);
        a = r_u32(state + 44);
        xport_gte_write_data(1, a);
        xport_gte_execute(0x180001);
        link = (packet + 40) << 8;
        x = r_s16(0x1F800024);
        y = r_s16(0x1F800026);
        w_u32(packet, (link >> 8) | 0x09000000);
        if ((sint32)x < 0)
            x = 0 - x;
        if ((sint32)y < 0)
            y = 0 - y;
        sum = x + y;
        remaining = 208 - sum;
        value = xport_gte_read_data(14);
        w_u32(packet + 32, value);
        packet += 40;
        seed = sum;
        if ((sint32)remaining > 0)
        {
            a = r_u16(0x1F800024);
            b = r_u16(0x1F800026);
            w_u32(0x1F80004C, 0);
            w_u16(0x1F800050, (a << 2) + (b << 3));
            RotMatrix((SVECTOR *)psx_addr(0x1F80004C, 6), (MATRIX *)psx_addr(0x1F800064, 18));
            a = r_u32(0x1F800064);
            b = r_u32(0x1F800068);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = r_u32(0x1F80006C);
            d = r_u32(0x1F800070);
            e = r_u32(0x1F800074);
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            color = (uint32)((sint32)remaining >> 2) + 23;
            size = (uint32)((sint32)remaining >> 2) + 14;
            w_u32(0x1F80002C, color | (color << 8) | (color << 16) | 0x2E000000);
            w_u16(0x1F800050, 1024);
            RotMatrix((SVECTOR *)psx_addr(0x1F80004C, 6), (MATRIX *)psx_addr(0x1F800084, 18));
            a = r_s16(0x1F800024);
            xport_gte_write_control(5, a);
            a = r_s16(0x1F800026);
            xport_gte_write_control(6, a);
            a = r_u16(0x1F80001A);
            xport_gte_write_control(7, a);
            value = 0 - size;
            w_u16(0x1F80004C, value);
            w_u16(0x1F80004E, value);
            w_u16(0x1F800050, 0);
            w_u16(0x1F800054, 0);
            w_u16(0x1F800056, value);
            w_u16(0x1F800058, 0);
            w_u16(0x1F80005C, value);
            w_u16(0x1F80005E, 0);
            w_u16(0x1F800060, 0);
            a = r_u32(0x1F80004C);
            xport_gte_write_data(0, a);
            a = r_u32(0x1F800050);
            xport_gte_write_data(1, a);
            a = r_u32(0x1F800054);
            xport_gte_write_data(2, a);
            a = r_u32(0x1F800058);
            xport_gte_write_data(3, a);
            a = r_u32(0x1F80005C);
            xport_gte_write_data(4, a);
            a = r_u32(0x1F800060);
            xport_gte_write_data(5, a);
            link += 0x2800;
            for (i = 0; i < 4; ++i)
            {
                if (i != 0)
                    sub_80086948(0x1F800084);
                value = r_u32(0x1F800024);
                w_u32(packet + 32, value);
                xport_gte_execute(0x280030);
                w_u32(packet, (link >> 8) | 0x09000000);
                w_u16(packet + 28, 0x3FA0);
                w_u32(packet + 12, 0x4F5620A0);
                w_u32(packet + 20, 0x003920BF);
                w_u16(packet + 36, 0x3FBF);
                value = r_u32(0x1F80002C);
                w_u32(packet + 4, value);
                value = xport_gte_read_data(12);
                w_u32(packet + 8, value);
                value = xport_gte_read_data((i & 1) != 0 ? 14 : 13);
                w_u32(packet + 16, value);
                value = xport_gte_read_data((i & 1) != 0 ? 13 : 14);
                w_u32(packet + 24, value);
                link += 0x2800;
                packet += 40;
            }
            scene = r_u32(0x800BCE54);
            a = r_u32(scene + 76);
            b = r_u32(scene + 80);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            c = r_u32(scene + 84);
            d = r_u32(scene + 88);
            e = r_u32(scene + 92);
            xport_gte_write_control(2, c);
            xport_gte_write_control(3, d);
            xport_gte_write_control(4, e);
            a = r_u32(scene + 96);
            b = r_u32(scene + 100);
            xport_gte_write_control(5, a);
            c = r_u32(scene + 104);
            xport_gte_write_control(6, b);
            xport_gte_write_control(7, c);
            seed = 0x003920BF;
        }
        tail = packet - 40;
        ordering = r_u32(0x1F800000);
        start = r_u32(0x1F800004);
        ordering += 0x3FF8;
        a = ordering + 2;
        lane = a & 3;
        value = r_u32(a & 0xFFFFFFFC) << (24 - (lane << 3));
        if (lane != 3)
            value |= seed & (0xFFFFFF >> (lane << 3));
        result = start << 8;
        a = tail + 2;
        lane = a & 3;
        shift = 24 - (lane << 3);
        a &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(a + i, value >> (shift + (i << 3)));
        a = ordering + 2;
        lane = a & 3;
        shift = 24 - (lane << 3);
        a &= 0xFFFFFFFC;
        for (i = 0; i <= lane; ++i)
            w_u8(a + i, result >> (shift + (i << 3)));
        w_u32(0x1F800004, packet);
    }
    else
    {
        if ((flags & 255) == 0)
            return 0;
        numerator = (312 - (x + y)) << 12;
        high = (uint32)(((sint64)(sint32)numerator * (sint64)(sint32)0xD20D20D3) >> 32);
        result = (uint32)((sint32)numerator >> 31);
        ratio = (uint32)((sint32)(high + numerator) >> 8) - result;
        w_u32(0x1F800028, ratio);
    }
    flags &= 255;
    if (flags == 0)
        return result;
    ratio = r_u32(0x1F800028);
    if ((sint32)ratio > 2048)
    {
        if (flags == 1)
        {
            value = r_u32(state);
            w_u32(state, value | 1);
            ratio = r_u32(0x1F800028);
            value = ((ratio - 2048) * 5) << 4;
            if ((sint32)value < 0)
                value += 2047;
            color = (sint32)value >> 11;
            color |= (color << 16) | (color << 8);
            ordering = r_u32(0x1F800000);
            value = r_u32(ordering);
            packet = r_u32(0x1F800004);
            w_u32(packet, value | 0x03000000);
            ordering = r_u32(0x1F800000);
            w_u32(ordering, packet & 0xFFFFFF);
            w_u16(packet + 8, -160);
            w_u32(packet + 4, color | 0x62000000);
            w_u16(packet + 10, -120);
            w_u32(packet + 12, 0x00F00140);
            w_u32(0x1F800004, packet + 16);
        }
        else
        {
            value = r_u32(state);
            w_u32(state, value | 2);
            ratio = r_u32(0x1F800028);
            w_u32(state + 4, (ratio - 2048) << 1);
        }
        ratio = r_u32(0x1F800028);
        if ((sint32)ratio > 3072)
            w_u32(0x1F800028, 12288 - ((ratio << 1) + ratio));
    }
    ratio = r_u32(0x1F800028);
    size = (uint32)((sint32)(ratio - 512) >> 5) + 24;
    if ((sint32)size >= 241)
        size = 240;
    else
    {
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        size += random & 7;
    }
    value = r_u32(0x1F80000C);
    distance += 0xFFC80000;
    result = value - distance;
    if ((sint32)distance < (sint32)value)
    {
        result = (sint32)result >> 14;
        size -= result;
    }
    if ((sint32)size <= 0)
        return result;
    w_u32(0x1F80002C, size);
    return sub_8008b7ac(0x1F80001C, 0x1F800000);
}

uint32 sub_8008b310(uint32 state, uint32 color)
{
    uint32 camera, a, b, c, d, e, gap, product, size, third, value, x, y, start, packet, i, j, angle, next, output, scale[3], low, high, ordering, address, lane, shift, scene;
    FUNCTION_MARKER(0x8008B310, "ACE3.BIN");
    camera = r_u32(0x1F80000C);
    if ((sint32)camera < (sint32)0xF8800000)
        return 1;
    a = r_u32(state + 8);
    b = r_u32(state + 12);
    w_u32(0x1F80001C, a);
    w_u32(0x1F800020, b);
    value = r_s16(0x1F80001E);
    gap = 0 - (uint32)((sint32)camera >> 12) - value;
    if ((sint32)gap > 32767)
    {
        a = r_u16(0x1F80001C);
        w_u16(0x1F80001E, (sint32)gap >> 1);
        b = r_u16(0x1F800020);
        w_u16(0x1F80001C, (sint16)a >> 1);
        w_u16(0x1F800020, (sint16)b >> 1);
    }
    else
        w_u16(0x1F80001E, gap);
    a = r_u32(0x1F80001C);
    xport_gte_write_data(0, a);
    a = r_u32(0x1F800020);
    xport_gte_write_data(1, a);
    xport_gte_execute(0x180001);
    value = r_s16(0x1F80001A);
    product = (((value << 2) + value) * 16 - ((value << 2) + value)) << 4;
    if ((sint32)product < 0)
        product += 4095;
    size = (sint32)product >> 12;
    w_u32(0x1F80002C, size);
    if ((sint32)gap <= 8192)
    {
        third = (uint32)(((sint64)(sint32)size * 0x55555556) >> 32) - (uint32)((sint32)product >> 31);
        value = (size - third) * gap;
        if ((sint32)value < 0)
            value += 8191;
        w_u32(0x1F800028, (uint32)((sint32)value >> 13) + third);
    }
    else
    {
        w_u32(0x1F800028, size);
        if ((sint32)gap > 16384)
            gap = 16384;
    }
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return 0x1F800024;
    value = xport_gte_read_data(14);
    w_u32(0x1F800024, value);
    x = r_s16(0x1F800024);
    size = r_u32(0x1F80002C);
    if ((sint32)x < 0)
        x = 0 - x;
    if ((sint32)(size + 160) < (sint32)x)
        return size + 120;
    y = r_s16(0x1F800026);
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return 1;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, color);
    xport_gte_write_data(8, (sint32)gap >> 3);
    start = r_u32(0x1F800004);
    packet = start + 8;
    xport_gte_execute(0x780010);
    w_u32(start, (packet & 0xFFFFFF) | 0x01000000);
    w_u32(start + 4, 0xE6000002);
    color = xport_gte_read_data(22);
    value = r_u16(0x1F800018);
    w_u32(0x1F80001C, 0);
    w_u16(0x1F800020, 0 - value);
    rotation_build(0x1F80001C, 0x1F800038, 0, 0);
    w_u32(0x1F800030, 0);
    scale[0] = r_u32(0x1F800028);
    scale[1] = r_u32(0x1F80002C);
    scale[2] = r_u32(0x1F800030);
    for (i = 0; i < 4; ++i)
    {
        value = r_u32(0x1F800038 + i * 4);
        j = (i * 2) % 3;
        low = (uint32)(sint32)(sint16)value * scale[j];
        high = (uint32)((sint32)value >> 16) * scale[(j + 1) % 3];
        w_u32(0x1F800038 + i * 4, ((uint32)((sint32)low >> 12) & 0xFFFF) | ((uint32)((sint32)high >> 12) << 16));
    }
    value = r_u32(0x1F800048);
    low = (uint32)(sint32)(sint16)value * scale[2];
    w_u32(0x1F800048, (sint32)low >> 12);
    a = r_u32(0x1F800038);
    b = r_u32(0x1F80003C);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(0x1F800040);
    d = r_u32(0x1F800044);
    e = r_u32(0x1F800048);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    x = r_s16(0x1F800024);
    y = r_s16(0x1F800026);
    w_u32(0x1F800054, 0);
    w_u32(0x1F80004C, x);
    w_u32(0x1F800050, y);
    a = r_u32(0x1F80004C);
    b = r_u32(0x1F800050);
    xport_gte_write_control(5, a);
    c = r_u32(0x1F800054);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    value = r_u32(0x800BCE60);
    angle = (r_u32(value + 36) << 6) & 4095;
    a = r_u16(0x800B7106 + angle * 4);
    angle = (angle << 1) & 4095;
    b = r_u16(0x1F800024);
    w_u16(0x1F800024, b + ((sint16)a >> 9));
    a = r_u16(0x800B7104 + angle * 4);
    w_u16(0x1F80001E, 4096);
    w_u16(0x1F800020, 0);
    w_u16(0x1F80001C, (sint16)a >> 4);
    for (i = 0; i < 16; ++i)
    {
        a = r_u32(0x1F80001C);
        xport_gte_write_data(0, a);
        a = r_u32(0x1F800020);
        xport_gte_write_data(1, a);
        xport_gte_execute(0x480012);
        angle = (angle + 0x555) & 4095;
        next = i + 1;
        a = r_u32(0x800B7104 + (next << 10));
        b = r_u16(0x800B7104 + angle * 4);
        w_u32(0x1F80001C, a);
        a = r_u16(0x1F80001C);
        w_u16(0x1F80001C, a + ((sint16)b >> 3));
        output = 0x1F800058 + i * 4;
        a = xport_gte_read_data(9);
        b = xport_gte_read_data(10);
        c = xport_gte_read_data(11);
        w_u16(output, a);
        w_u16(output + 2, b);
        w_u16(output + 4, c);
    }
    value = r_u32(0x1F800058);
    w_u32(0x1F800098, value);
    for (i = 0; i < 16; i += 2)
    {
        next = packet + 36;
        w_u32(packet, (next & 0xFFFFFF) | 0x08000000);
        w_u32(packet + 4, 0x3A030303);
        w_u32(packet + 12, color);
        w_u32(packet + 20, 0x00030303);
        w_u32(packet + 28, 0x00030303);
        value = r_u32(0x1F800058 + i * 4);
        w_u32(packet + 8, value);
        value = r_u32(0x1F800024);
        w_u32(packet + 16, value);
        value = r_u32(0x1F800058 + (i + 1) * 4);
        w_u32(packet + 24, value);
        value = r_u32(0x1F800058 + (i + 2) * 4);
        w_u32(packet + 32, value);
        packet = next;
    }
    w_u32(packet, 0x01000000);
    w_u32(packet + 4, 0xE6000000);
    ordering = r_u32(0x1F800000);
    start = r_u32(0x1F800004);
    ordering += 120;
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
    if (lane != 3)
        value |= packet & (0xFFFFFF >> (lane << 3));
    start <<= 8;
    address = packet + 2;
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
        w_u8(address + i, start >> (shift + (i << 3)));
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800004, packet + 8);
    a = r_u32(scene + 76);
    b = r_u32(scene + 80);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(scene + 84);
    d = r_u32(scene + 88);
    e = r_u32(scene + 92);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u32(scene + 96);
    b = r_u32(scene + 100);
    xport_gte_write_control(5, a);
    c = r_u32(scene + 104);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    return scene + 76;
}

uint32 sub_80085eac(uint32 state, uint32 geometry, uint32 frame)
{
    uint32 packed, mode, packet, divisor, counter, numerator, u, v, flags, color, index, ordering, value, height;
    FUNCTION_MARKER(0x80085EAC, "ACE3.BIN");
    packed = r_u32(frame);
    mode = r_u32(state + 12);
    packet = r_u32(state + 4);
    divisor = (sint32)packed >> 16;
    counter = (sint16)packed;
    if (divisor == 0)
        divisor = 1;
    w_u16(packet + 14, ((mode + 260) << 6) | 0x16);
    numerator = mode == 3 ? counter << 3 : ((counter << 1) + counter) << 1;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    u = (uint32)((sint32)numerator / (sint32)divisor) << 5;
    v = (sint32)mode < 2 ? 0 : mode == 3 ? 64 : 32;
    w_u16(packet + 20, (v << 8) | (u + 31));
    w_u16(packet + 12, (v << 8) | u);
    w_u16(packet + 28, ((v + 31) << 8) | u);
    w_u16(packet + 36, ((v + 31) << 8) | (u + 31));
    flags = r_u32(state + 8);
    color = r_u32(state + 96);
    w_u32(packet + 4, color);
    if ((flags & 1) == 0)
    {
        w_u16(packet + 22, ((flags & 3) << 5) | 0x18);
        w_u32(packet + 40, 0xE1000220);
        index = r_s16(geometry + 4);
        ordering = r_u32(state);
        value = r_u32(ordering + (index << 2));
        w_u32(packet, value | 0x0A000000);
        index = r_s16(geometry + 4);
        ordering = r_u32(state);
        w_u32(ordering + (index << 2), packet & 0xFFFFFF);
        value = packet + 44;
    }
    else
    {
        w_u16(packet + 22, 0x38);
        index = r_s16(geometry + 4);
        ordering = r_u32(state);
        value = r_u32(ordering + (index << 2));
        w_u32(packet, value | 0x09000000);
        index = r_s16(geometry + 4);
        ordering = r_u32(state);
        w_u32(ordering + (index << 2), packet & 0xFFFFFF);
        value = packet + 40;
    }
    w_u32(state + 4, value);
    height = r_s16(geometry + 6);
    w_u16(state + 20, height);
    if ((sint32)v < 64)
        height += (uint32)((sint32)height >> 1);
    value = (uint32)((sint32)((uint32)((sint32)height >> 3) - height) >> 1);
    w_u16(state + 22, height);
    w_u16(state + 24, 0);
    w_u16(state + 26, value);
    value = r_u32(geometry);
    w_u32(state + 28, value);
    return sub_8008591c(state, packet);
}

uint32 sub_800854b8(uint32 records)
{
    uint32 total, group, mask, record, bit, output, value, camera, x, y, z, radius, ax, ay, az, doubled, packed, h, product, depth, size, screen, bucket, a, b, c, d, e, scene, settings, count, kind, target;
    FUNCTION_MARKER(0x800854B8, "ACE3.BIN");
    total = 0;
    for (group = 0; group < 3; ++group)
    {
        mask = r_u32(records + group * 4);
        w_u32(0x1F800044 + group * 4, 0);
        record = records + 12 + group * 0x380;
        bit = 0;
        output = 0x1F800080 + total * 8;
        while (mask != 0)
        {
            if ((mask & 1) == 0 || r_s16(record) < 0)
                goto next_effect;
            value = r_u32(record + 4);
            camera = r_u32(0x1F800008);
            x = (sint32)(value - camera) >> 12;
            if (x + 0x8000 > 0xFFFF)
                goto next_effect;
            value = r_u32(record + 8);
            camera = r_u32(0x1F80000C);
            y = (sint32)(value - camera) >> 12;
            if (y + 0x8000 > 0xFFFF)
                goto next_effect;
            value = r_u32(record + 12);
            camera = r_u32(0x1F800010);
            z = (sint32)(value - camera) >> 12;
            if (z + 0x8000 > 0xFFFF)
                goto next_effect;
            radius = r_u16(record + 26);
            w_u16(output + 6, radius);
            radius = (sint16)radius;
            doubled = 0;
            if ((sint32)radius < 256)
            {
                ax = (sint32)x < 0 ? 0 - x : x;
                ay = (sint32)y < 0 ? 0 - y : y;
                az = (sint32)z < 0 ? 0 - z : z;
                if ((sint32)(ax + ay + az) < 1024)
                    doubled = 1;
            }
            if (doubled != 0)
            {
                packed = ((x << 1) & 0xFFFF) | (y << 17);
                z <<= 1;
            }
            else
                packed = (x & 0xFFFF) | (y << 16);
            xport_gte_write_data(0, packed);
            xport_gte_write_data(1, z);
            h = r_s16(0x1F80001A);
            xport_gte_execute(0x180001);
            product = radius * h;
            depth = xport_gte_read_data(27);
            value = (sint32)depth < (sint32)h;
            depth = (sint32)depth >> doubled;
            if (value != 0)
                goto next_effect;
            if (depth == 0)
                xport_mips_break(7);
            if (depth == 0xFFFFFFFF && product == 0x80000000)
                xport_mips_break(6);
            size = (sint32)product / (sint32)depth;
            if (size == 0)
                goto next_effect;
            screen = xport_gte_read_data(14);
            x = (sint16)screen;
            if ((sint32)x < 0)
                x = 0 - x;
            value = (sint32)(size + 160) < (sint32)x;
            w_u16(output + 6, size);
            if (value != 0)
                goto next_effect;
            y = (sint32)screen >> 16;
            if ((sint32)y < 0)
                y = 0 - y;
            if ((sint32)(size + 120) < (sint32)y)
                goto next_effect;
            bucket = (uint32)((sint32)depth >> 3) - 1;
            w_u32(output, screen);
            if ((sint32)bucket >= 4095)
                bucket = 4094;
            else if ((sint32)bucket < 0)
                bucket = 0;
            w_u16(output + 4, bucket);
            output += 8;
            ++total;
            value = r_u32(0x1F800044 + group * 4);
            w_u32(0x1F800044 + group * 4, value | (1u << (bit & 31)));
        next_effect:
            ++bit;
            record += 28;
            mask >>= 1;
        }
    }
    if (total == 0)
        return 0x1F800054;
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(0x1F800054, a);
    w_u32(0x1F800058, b);
    c = xport_gte_read_control(2);
    d = xport_gte_read_control(3);
    e = xport_gte_read_control(4);
    w_u32(0x1F80005C, c);
    w_u32(0x1F800060, d);
    w_u32(0x1F800064, e);
    xport_gte_write_data(1, 0);
    a = r_u32(0x1F800000);
    b = r_u32(0x1F800004);
    scene = r_u32(0x800BCE54);
    w_u32(0x1F800074, 0xFFFFFFFF);
    w_u32(0x1F80001C, a);
    w_u32(0x1F800020, b);
    settings = r_u32(scene + 0x268);
    value = r_u32(settings + 16);
    w_u32(0x1F80007C, (value & 0xFFFFFF) | 0x2E000000);
    count = 0;
    for (group = 0; group < 2; ++group)
    {
        mask = r_u32(0x1F800044 + group * 4);
        record = records + 12 + group * 0x380;
        output = 0x1F800080 + count * 8;
        while (mask != 0)
        {
            if ((mask & 1) != 0)
            {
                value = r_s16(0x1F800018);
                w_u32(0x1F80002C, 0 - value);
                packed = r_u16(record + 24);
                kind = (packed >> 4) & 15;
                if (kind >= 4)
                    kind = 3;
                w_u32(0x1F800024, (packed >> 8) & 3);
                w_u32(0x1F800028, packed & 15);
                target = r_u32(0x800B439C + kind * 4);
                ++count;
                value = output;
                output += 8;
                xport_guest_call3(target, 0x1F80001C, value, record);
            }
            mask >>= 1;
            record += 28;
        }
    }
    value = r_u32(0x1F800020);
    w_u32(0x1F800004, value);
    a = r_u32(0x1F800054);
    b = r_u32(0x1F800058);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(0x1F80005C);
    d = r_u32(0x1F800060);
    e = r_u32(0x1F800064);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    return 0x1F800054;
}

uint32 sub_800343c8(uint32 ordering, uint32 packet, uint32 rectangle, uint32 mode, const uint8 *native_rectangle)
{
    uint32 x, y, width, height, right, bottom, corner_color, edge_color, fill_color, palette, i, current, value, uv;
    uint32 fields[7][4];
    FUNCTION_MARKER(0x800343C8, "ACE3.BIN");
    x = native_rectangle != 0 ? (sint16)xport_load_le16(native_rectangle) : r_s16(rectangle);
    y = native_rectangle != 0 ? (sint16)xport_load_le16(native_rectangle + 2) : r_s16(rectangle + 2);
    width = native_rectangle != 0 ? (sint16)xport_load_le16(native_rectangle + 4) : r_s16(rectangle + 4);
    height = native_rectangle != 0 ? (sint16)xport_load_le16(native_rectangle + 6) : r_s16(rectangle + 6);
    if (mode == 1)
    {
        palette = 0x5DD6;
        corner_color = 0x76808080;
        edge_color = 0x62000000;
        fill_color = 0x62484848;
    }
    else if (mode == 2)
    {
        palette = 0x5D56;
        corner_color = 0x76808080;
        edge_color = 0x62000098;
        fill_color = 0x620030F8;
    }
    else
    {
        palette = 0x5D56;
        corner_color = 0x74808080;
        edge_color = 0x60000098;
        fill_color = 0x600030F8;
    }
    right = x + width;
    bottom = y + height;
    for (i = 0; i < 4; ++i)
    {
        current = packet + i * 16;
        uv = (palette << 16) | ((i & 1) != 0 ? 0x40F8 : 0x40E8) | ((i & 2) != 0 ? 0x1000 : 0);
        w_u32(current + 4, corner_color);
        w_u16(current + 8, (i & 1) != 0 ? right - 8 : x);
        w_u16(current + 10, (i & 2) != 0 ? bottom - 8 : y);
        w_u32(current + 12, uv);
        value = r_u32(ordering);
        w_u32(current, value | 0x03000000);
        w_u32(ordering, current & 0xFFFFFF);
    }
    fields[0][0] = x;
    fields[0][1] = y + 8;
    fields[0][2] = 2;
    fields[0][3] = height - 16;
    fields[1][0] = right - 2;
    fields[1][1] = y + 8;
    fields[1][2] = 2;
    fields[1][3] = height - 16;
    fields[2][0] = x + 8;
    fields[2][1] = y;
    fields[2][2] = width - 16;
    fields[2][3] = 2;
    fields[3][0] = x + 8;
    fields[3][1] = bottom - 2;
    fields[3][2] = width - 16;
    fields[3][3] = 2;
    fields[4][0] = x + 8;
    fields[4][1] = y + 2;
    fields[4][2] = width - 16;
    fields[4][3] = 6;
    fields[5][0] = x + 2;
    fields[5][1] = y + 8;
    fields[5][2] = width - 4;
    fields[5][3] = height - 16;
    fields[6][0] = x + 8;
    fields[6][1] = bottom - 8;
    fields[6][2] = width - 16;
    fields[6][3] = 6;
    for (i = 0; i < 7; ++i)
    {
        current = packet + 64 + i * 16;
        w_u32(current + 4, i < 4 ? edge_color : fill_color);
        w_u16(current + 8, fields[i][0]);
        w_u16(current + 10, fields[i][1]);
        w_u16(current + 12, fields[i][2]);
        w_u16(current + 14, fields[i][3]);
        value = r_u32(ordering);
        w_u32(current, value | 0x03000000);
        w_u32(ordering, current & 0xFFFFFF);
    }
    return packet + 176;
}

uint32 sub_80048758(uint32 index)
{
    uint32 scene, table, header, entry, object, slot, value, flags, mode, stats, kind, counter, coordinate;
    FUNCTION_MARKER(0x80048758, "ACE3.BIN");
    scene = r_u32(0x800BCE60);
    table = r_u32(0x800BCE5C);
    header = scene + 0x3B8;
    slot = r_u8(header + 10);
    object = r_u32(table + (index << 2));
    value = r_u8(object + 72);
    entry = scene + 0x748 + slot * 8;
    w_u8(entry, value);
    flags = r_u32(0x800C6FD0 + (index << 2));
    if ((flags & 0x800) == 0)
        return 45;
    kind = r_s16(object + 72);
    if (kind == 45)
    {
        stats = r_u32(0x800BCEB4);
        scene = r_u32(0x800BCE60);
        value = r_u8(stats + 60);
        mode = r_s16(scene + 0x38C);
        w_u8(stats + 60, value + 1);
        stats = r_u32(0x800BCEB4);
        value = r_u8(stats + 60);
        if (value < 4)
            return 17;
        if (mode != 17 && mode != 31 && mode != 38 && mode != 46 && mode != 52)
            return 52;
    }
    flags = r_u32(object);
    if ((flags & 0x80) == 0)
        w_u8(entry, 0x74);
    else
    {
        kind = r_u8(entry);
        if (kind == 48)
            w_u8(entry, 0x72);
        else if (kind == 49)
            w_u8(entry, 0x73);
        else if (kind > 49)
        {
            flags = r_u32(0x800C6FD0 + (index << 2));
            w_u8(entry, (flags & 0x400) != 0 ? 0x70 : 0x71);
        }
    }
    scene = r_u32(0x800BCE60);
    mode = r_s16(scene + 0x38C);
    if (mode == 33 && r_s16(object + 72) == 27)
        goto active_record;
    flags = r_u32(0x800C6FD0 + (index << 2));
    if ((flags & 0x100) != 0)
    {
        value = r_u32(object + 80);
        if ((value & 63) != 0)
            goto active_record;
        w_u8(entry + 1, 4);
        goto empty_record;
    }
    flags = r_u32(object);
    if ((flags & 0x100) != 0)
    {
        w_u8(entry + 1, 3);
        value = r_u32(object + 80);
        if ((value & 63) == 0)
        {
            stats = r_u32(0x800BCEB4);
            value = r_u8(stats + 11);
            w_u8(stats + 11, value + 1);
        }
        goto finish_record;
    }
    value = r_u32(object + 80);
    if ((value & 63) == 0)
    {
        w_u8(entry + 1, 0);
        goto empty_record;
    }
    if ((flags & 0x40) != 0)
    {
        w_u8(entry + 1, 2);
        goto finish_record;
    }
active_record:
    w_u8(entry + 1, 1);
    stats = r_u32(0x800BCEB4);
    value = r_u8(stats + 45);
    w_u8(stats + 45, value + 1);
    goto finish_record;
empty_record:
    stats = r_u32(0x800BCEB4);
    value = r_u8(stats + 46);
    w_u8(stats + 46, value + 1);
finish_record:
    w_u8(entry + 6, index);
    coordinate = r_u32(object + 8);
    coordinate = (uint32)((sint32)coordinate >> 12) + 0x40000;
    coordinate = (sint32)coordinate >> 11;
    if ((sint32)coordinate < 0)
        coordinate = 0;
    if ((sint32)coordinate >= 256)
        coordinate = 255;
    w_u8(entry + 4, coordinate);
    coordinate = r_u32(object + 16);
    coordinate = (uint32)((sint32)coordinate >> 12) + 0x40000;
    coordinate = (sint32)coordinate >> 11;
    if ((sint32)coordinate < 0)
        coordinate = 0;
    if ((sint32)coordinate >= 256)
        coordinate = 255;
    w_u8(entry + 5, coordinate);
    value = r_u16(header + 2);
    counter = (uint32)(((uint64)value * 0x88888889) >> 32) >> 4;
    w_u16(entry + 2, counter);
    value = r_u8(header + 10) + 1;
    w_u8(header + 10, value);
    return value;
}

uint32 sub_8007867c(uint32 state)
{
    uint32 scene, flags, value, raw, delta, i, fast, target[3], step[3];
    FUNCTION_MARKER(0x8007867C, "ACE3.BIN");
    target[0] = 0;
    target[1] = 0;
    target[2] = 0;
    scene = r_u32(0x800BCE60);
    flags = r_u32(scene + 0x138);
    if ((flags & 0x10) != 0)
        target[0] = 0 - r_u32(scene + 0x14C);
    if ((flags & 0x20) != 0)
        target[1] = r_u32(scene + 0x150);
    flags = r_u32(scene + 0x128);
    if ((flags & 0x100000) != 0)
        target[1] += 0x2000;
    if ((flags & 0x10000) != 0)
        target[0] += 0x1000;
    if ((flags & 0x20000) != 0)
        target[0] -= 0x1000;
    if ((flags & 0x40000) != 0)
        target[1] -= 0x1000;
    if ((flags & 0x80000) != 0)
        target[1] += 0x1000;
    value = r_s16(state + 0xB6);
    raw = r_u16(state + 0xB6);
    if ((sint32)(value - target[1]) < -8192)
        w_u16(state + 0xB6, raw + 0x4000);
    value = r_s16(state + 0xB6);
    raw = r_u16(state + 0xB6);
    if ((sint32)(value - target[1]) > 8192)
        w_u16(state + 0xB6, raw - 0x4000);
    scene = r_u32(0x800BCE60);
    flags = r_u32(scene + 0x12C);
    fast = flags & 0x100000;
    if (fast == 0)
    {
        flags = r_u32(scene + 0x134);
        fast = flags & 0x100000;
    }
    if (fast != 0)
    {
        w_u16(state + 0xB4, target[0]);
        w_u16(state + 0xB6, target[1]);
        w_u16(state + 0xB8, 0);
        sub_8007786c(state);
    }
    else
    {
        for (i = 0; i < 3; ++i)
        {
            value = r_s16(state + 0xB4 + i * 2);
            delta = value - target[i];
            if ((sint32)delta < 0)
                delta = target[i] - value;
            step[i] = (sint32)(delta + 2) >> 2;
        }
        for (i = 0; i < 3; ++i)
        {
            value = r_s16(state + 0xB4 + i * 2);
            raw = r_u16(state + 0xB4 + i * 2);
            if ((sint32)value < (sint32)target[i])
                w_u16(state + 0xB4 + i * 2, raw + step[i]);
            value = r_s16(state + 0xB4 + i * 2);
            raw = r_u16(state + 0xB4 + i * 2);
            if ((sint32)target[i] < (sint32)value)
                w_u16(state + 0xB4 + i * 2, raw - step[i]);
        }
        for (i = 0; i < 3; ++i)
        {
            value = r_s16(state + 0xB4 + i * 2);
            delta = value - target[i];
            if ((sint32)delta < 0)
                delta = target[i] - value;
            if ((sint32)delta < (sint32)step[i])
                w_u16(state + 0xB4 + i * 2, target[i]);
        }
    }
    return sub_800313a8(state + 20, state + 0xB4, state + 20, 0);
}

uint32 sub_8004f0c8(uint32 index)
{
    uint32 states, table, records, state, record, object, player, settings, behavior, phase, scene, value, a, b, tick, angle, sine, cosine, product, result, mode, direction, seed, random;
    FUNCTION_MARKER(0x8004F0C8, "ACE3.BIN");
    states = r_u32(0x800BCEC0);
    table = r_u32(0x800BCE5C);
    records = r_u32(0x800BCEF8);
    state = states + index * 184;
    record = records + (index << 6);
    object = r_u32(table + (index << 2));
    player = r_u32(table);
    settings = r_u32(record + 24);
    phase = r_u8(state + 62);
    behavior = r_u32(record + 20);
    if (phase == 0)
    {
        w_u8(state + 62, 1);
        value = r_u16(settings + 14);
        scene = r_u32(0x800BCE60);
        w_u16(state + 64, value);
        mode = r_u32(scene + 0x394);
        if ((sint32)mode >= 2)
            w_u16(state + 64, (sint16)value >> 1);
        result = r_u32(player + 12);
        w_u32(state + 108, result);
        return result;
    }
    if (phase == 1)
    {
        w_u8(state + 63, 0);
        a = r_u32(player + 56);
        scene = r_u32(0x800BCE54);
        b = r_u32(player + 8);
        tick = r_u8(scene + 12);
        w_u32(state + 104, b + (a << 12));
        a = r_u32(player + 64);
        b = r_u32(player + 16);
        scene = r_u32(0x800BCE60);
        w_u32(state + 112, b + (a << 12));
        mode = r_u32(scene + 0x394);
        if ((sint32)mode >= 2)
        {
            angle = (tick << 6) & 0x3FFF;
            sine = r_s16(0x800104AC + angle * 2);
            value = r_u8(settings + 1);
            product = sine * value;
            cosine = r_s16(0x800104AC + (angle + 0x1000) * 2);
            value = r_u32(state + 104);
            w_u32(state + 104, value + (product << 12));
            a = r_u8(settings + 1);
            product = cosine * a;
            value = r_u32(state + 112);
            w_u32(state + 112, value + (product << 12));
        }
        sub_800505dc(index, state + 104);
        w_u8(state + 99, 4);
        value = r_u16(state + 64);
        w_u8(state + 97, 3);
        --value;
        w_u16(state + 64, value);
        result = value << 16;
        if (result == 0)
        {
            w_u8(state + 62, 2);
            result = 2;
        }
        return result;
    }
    if (phase == 2)
    {
        w_u8(state + 62, 3);
        mode = r_u32(behavior);
        if (mode == 3)
            return 3;
        if (mode == 1)
        {
            a = r_s16(behavior + 20);
            if (a == 0)
                a = 4096;
            scene = r_u32(0x800BCE54);
            value = r_u32(scene + 12);
            if ((value & 1) == 0)
                a = 0 - a;
            w_u16(state + 120, 2048);
            w_u16(state + 122, 0);
            w_u16(state + 124, a);
            result = 12;
            goto select_behavior;
        }
        scene = r_u32(0x800BCE60);
        mode = r_s16(scene + 0x38C);
        if (mode == 32 || mode == 47 || mode == 53)
        {
            scene = r_u32(0x800BCE54);
            w_u16(state + 120, 8192);
            w_u16(state + 122, 0);
            value = r_u32(scene + 12);
            a = r_u16(object + 28);
            w_u16(state + 124, ((value & 1) != 0 ? 0xFFFFF000 : 0x1000) - a);
            result = 12;
            goto select_behavior;
        }
        value = r_u8(settings);
        if (value >= 2)
        {
            scene = r_u32(0x800BCE54);
            tick = r_u32(scene + 12);
            if ((tick & 7) == 0)
            {
                direction = r_u32(object + 8) & 3;
                if (direction < 2)
                {
                    w_u16(state + 120, direction == 1 ? 8192 : 4096);
                    w_u16(state + 122, 0);
                    w_u16(state + 124, 0);
                }
                else if (direction == 2)
                {
                    w_u16(state + 120, 4096);
                    w_u16(state + 122, 0);
                    w_u16(state + 124, 4096);
                }
                else
                {
                    a = r_u16(object + 12);
                    w_u16(state + 122, 0);
                    w_u16(state + 120, (a & 0x1FFF) + 8192);
                    a = r_u16(object + 16);
                    w_u16(state + 124, a & 0x1FFF);
                }
                result = 15;
                goto select_behavior;
            }
        }
        value = r_u8(settings);
        if (value != 0)
        {
            scene = r_u32(0x800BCE54);
            tick = r_u32(scene + 12);
            if ((tick & 7) == 1)
            {
                direction = r_u32(object + 8) & 3;
                result = direction == 0 ? 10 : direction == 1 ? 11 : direction == 2 ? 13 : 14;
                goto select_behavior;
            }
        }
        w_u16(state + 120, 8192);
        w_u16(state + 122, 0);
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        w_u16(state + 124, (random & 0x3FFF) - 8192);
        result = 12;
    select_behavior:
        w_u8(state + 99, result);
        w_u8(state + 97, 0);
        return result;
    }
    if (phase == 3)
    {
        result = r_u32(state) & 0x400000;
        if (result != 0)
        {
            w_u8(state + 61, 255);
            result = 255;
        }
        return result;
    }
    return 3;
}

uint32 sub_8004f570(uint32 index)
{
    uint32 records, pool, states, table, record, settings, behavior, state, effect, object, slot, phase, value, mode, a, b, c, d, scene, tick, seed, random, difficulty, angle, yaw, raw, result, shift;
    FUNCTION_MARKER(0x8004F570, "ACE3.BIN");
    records = r_u32(0x800BCEF8);
    pool = r_u32(0x800BD080);
    states = r_u32(0x800BCEC0);
    table = r_u32(0x800BCE5C);
    record = records + (index << 6);
    slot = r_u8(pool + index + 8);
    settings = r_u32(record + 24);
    behavior = r_u32(record + 20);
    state = states + index * 184;
    effect = pool + 200 + slot * 164;
    phase = r_u8(state + 62);
    object = r_u32(table + (index << 2));
    if (phase == 0)
    {
        mode = r_u32(behavior);
        if (mode != 0 && mode != 5 && mode != 2 && mode != 1)
            w_u8(state + 61, 255);
        w_u8(state + 62, 1);
        a = r_u8(settings + 1);
        b = r_u16(settings + 16);
        c = r_u16(settings + 18);
        w_u16(state + 64, c + b - (a - 22));
        value = r_u32(0x800C6FD0 + (index << 2));
        if ((value & 0x800000) == 0)
        {
            pool = r_u32(0x800BD080);
            slot = r_u8(pool + index + 8);
            value = r_u32(pool + slot * 164 + 0x134);
            if ((value & 63) != 0 && (value & 0x40) != 0)
                w_u8(state + 62, 16);
        }
        value = r_u8(state + 28);
        if (value == 10)
        {
            scene = r_u32(0x800BCE54);
            tick = r_u32(scene + 12);
            if ((tick & 3) != 0)
                w_u8(state + 62, 7);
        }
    }
    value = r_u16(state + 64) - 1;
    w_u16(state + 64, value);
    if ((sint32)(value << 16) <= 0)
        w_u8(state + 61, 255);
    phase = r_u8(state + 62);
    if (phase == 16)
    {
        value = r_u8(object + 5);
        w_u8(object + 5, value | 8);
        w_u8(state + 99, 0);
        w_u8(state + 97, 0);
        return 0;
    }
    a = r_s16(state + 64);
    b = r_s16(settings + 18);
    if ((sint32)b < (sint32)a)
        return 0;
    value = r_u8(object + 5);
    w_u8(object + 5, value | 8);
    phase = r_u8(state + 62);
    if (phase == 7)
    {
        table = r_u32(0x800BCE5C);
        value = r_u32(table);
        a = r_u32(value + 8);
        b = r_u32(value + 12);
        c = r_u32(value + 16);
        d = r_u32(value + 20);
        w_u32(state + 104, a);
        w_u32(state + 108, b);
        w_u32(state + 112, c);
        w_u32(state + 116, d);
        a = r_u32(value + 24);
        b = r_u32(value + 28);
        w_u32(state + 120, a);
        w_u32(state + 124, b);
        w_u8(state + 99, 4);
        w_u8(state + 97, 6);
        return 0;
    }
    if (phase == 3)
    {
        scene = r_u32(0x800BCE60);
        difficulty = r_u32(scene + 0x394);
        if (difficulty == 3)
            w_u8(state + 62, 1);
        else if (difficulty == 2)
        {
            value = r_s16(state + 102);
            w_u8(state + 99, 0);
            if (value == 60)
                w_u8(state + 62, 1);
        }
        else if (difficulty == 1)
        {
            value = r_s16(state + 102);
            w_u8(state + 99, 0);
            if (value == 90)
                w_u8(state + 62, 1);
        }
        else if (difficulty == 0)
            w_u8(state + 99, 0);
    }
    phase = r_u8(state + 62);
    if (phase == 2)
    {
        value = r_u8(state + 99);
        if (value == 255)
        {
            value = r_u8(state + 28);
            w_u8(state + 99, 50);
            w_u8(state + 97, value == 10);
            w_u8(state + 62, 3);
        }
    }
    phase = r_u8(state + 62);
    if (phase != 1)
        return 1;
    a = r_s16(state + 64);
    b = r_s16(settings + 18);
    result = (sint32)b < (sint32)a;
    if (result != 0)
        return result;
    w_u8(state + 62, 2);
    scene = r_u32(0x800BCE60);
    difficulty = r_u32(scene + 0x394) & 3;
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    angle = (random & 0x3FFF) - 8192;
    mode = r_u32(behavior);
    if (mode == 1)
    {
        a = r_s16(behavior + 20);
        if (a == 0)
            a = 4096;
        scene = r_u32(0x800BCE54);
        tick = r_u32(scene + 12);
        if ((tick & 1) == 0)
            a = 0 - a;
        w_u16(state + 124, a);
        value = r_u8(state + 28);
        w_u16(state + 120, 2048);
        w_u8(state + 99, 12);
        w_u16(state + 122, 0);
        w_u8(state + 97, value == 10 ? mode : 0);
        return 0;
    }
    scene = r_u32(0x800BCE60);
    mode = r_s16(scene + 0x38C);
    if (mode == 32 || mode == 47 || mode == 53)
    {
        scene = r_u32(0x800BCE54);
        tick = r_u32(scene + 12);
        raw = r_u16(object + 28);
        angle = ((tick & 1) != 0 ? 0xFFFFF000 : 0x1000) - raw;
    }
    else if (difficulty == 3)
    {
        value = r_u32(effect + 104);
        if ((sint32)value < 1024)
            angle = (sint32)(angle << 16) >> 19;
        else if ((sint32)value < 2048)
            angle = (sint32)(angle << 16) >> 18;
        else if ((sint32)value < 4096)
            angle = (sint32)(angle << 16) >> 17;
    }
    else
    {
        if (difficulty == 0)
        {
            value = r_u8(settings);
            if (value != 3)
                goto align_yaw;
        }
        value = r_u8(settings);
        if (value != 0)
            goto create_turn;
    align_yaw:
        yaw = r_s16(object + 28);
        raw = r_u16(object + 28);
        value = (sint32)yaw < 0 ? 0 - yaw : yaw;
        if ((sint32)value > 4096)
        {
            w_u16(state + 120, 0);
            w_u16(state + 122, 0);
            a = r_u16(object + 28);
            w_u8(state + 99, 12);
            value = r_u8(state + 28);
            w_u16(state + 124, 0 - a);
            w_u8(state + 97, value == 10);
            return 0;
        }
        angle = ((sint32)yaw > 0 ? 0xFFFFF000 : 0x1000) - raw;
    }
create_turn:
    a = r_u8(settings + 1);
    shift = r_u32(0x800B2038 + difficulty * 4);
    b = r_u16(0x800B2028 + difficulty * 4);
    w_u8(state + 99, 12);
    w_u16(state + 120, b + (a << (shift & 31)));
    value = r_u8(state + 28);
    w_u16(state + 122, 0);
    w_u16(state + 124, angle);
    result = value == 10 ? 1 : 10;
    w_u8(state + 97, value == 10);
    return result;
}

uint32 sub_8007a260(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, delay, limit, root, source, offset, value, i, count;
    FUNCTION_MARKER(0x8007A260, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        if ((sint32)duration > 0)
        {
            delay = (sint32)duration < 31 ? (uint32)((sint32)duration >> 1) : 30;
            limit = duration - delay;
        }
        else
        {
            root = r_u32(0x800BCE60);
            delay = (r_u32(root + 36) & 31) + 30;
            limit = delay + 90;
        }
        w_u32(control + 32, limit);
        sub_8007995c(state, delay << 5, control + 4, 0, native_context);
        for (i = 0; i < 2; ++i)
        {
            source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
            offset = r_s16(source + i * 2);
            value = r_u32(control + 4 + i * 4);
            w_u32(control + 4 + i * 4, value + (offset << 5));
        }
        source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
        offset = r_s16(source + 4);
        w_u32(control, 1);
        value = r_u32(control + 12);
        w_u32(control + 12, value + (offset << 5));
    }
    else if (mode != 1)
        return 0;
    sub_8007a060(state, control + 4, control + 4, native_context);
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    ++count;
    w_u32(control + 28, count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    w_u32(control, 0);
    return 0;
}

uint32 sub_8007a52c(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, value, limit, count, divisor, difference, numerator, quotient, source, offset, i;
    FUNCTION_MARKER(0x8007A52C, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        w_u32(control, 1);
        w_u32(control + 32, (sint32)duration > 0 ? duration : 450);
        value = sub_80079ae8(state, native_context);
        w_u32(control + 40, value);
        if ((sint32)value <= 0x29FFF)
            w_u32(control + 40, 0x2A000);
    }
    else if (mode != 1)
        return 0;
    limit = r_u32(control + 32);
    count = r_u32(control + 28);
    divisor = (sint32)limit >> 1;
    difference = count - divisor;
    numerator = ((difference << 4) - difference) << 5;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    value = r_u32(control + 40);
    sub_800799d4(state, value, 0 - quotient, control + 4, 0, native_context);
    for (i = 0; i < 3; ++i)
    {
        source = (native_context != 0 ? native_context[3] : r_u32(state + 12));
        offset = r_s16(source + i * 2);
        value = r_u32(control + 4 + i * 4);
        w_u32(control + 4 + i * 4, value - (offset << 6));
    }
    sub_8007a060(state, control + 4, control + 4, native_context);
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    ++count;
    w_u32(control + 28, count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    w_u32(control, 0);
    return 0;
}

uint32 sub_80086320(uint32 effect, uint32 state, uint32 context)
{
    uint32 value, camera, x, y, z, a, b, c, d, e, root, radius, h, product, depth, size, near_depth, screen, bucket, packet, ordering, color, angle, shift, tx, ty, tz, words, result;
    FUNCTION_MARKER(0x80086320, "ACE3.BIN");
    value = r_u32(effect + 4);
    camera = r_u32(state + 8);
    x = (sint32)(value - camera) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(effect + 8);
    camera = r_u32(state + 12);
    y = (sint32)(value - camera) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(effect + 12);
    camera = r_u32(state + 16);
    z = (sint32)(value - camera) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return 1;
    a = r_u32(context);
    b = r_u32(context + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(context + 8);
    d = r_u32(context + 12);
    e = r_u32(context + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    root = r_u32(0x800BCF90);
    angle = r_u32(root + (effect & 0x1FC));
    xport_gte_execute(0x180001);
    w_u32(context + 64, angle);
    value = r_u16(effect + 22);
    if ((value & 0x100) != 0)
    {
        value = r_u16(effect);
        w_u16(context + 68, value << 8);
    }
    else
        w_u16(context + 68, 0);
    radius = r_s16(effect + 26);
    w_u32(context + 112, radius);
    h = r_s16(state + 26);
    product = radius * h;
    depth = xport_gte_read_data(19);
    if ((sint32)depth <= 0)
        return h;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    near_depth = depth - size;
    if ((sint32)near_depth < ((sint32)h >> 4))
        return 1;
    screen = xport_gte_read_data(14);
    y = (sint32)screen >> 16;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return 1;
    x = (sint16)screen;
    if ((sint32)x < 0)
        x = 0 - x;
    result = (sint32)size < 3;
    if ((sint32)(size + 160) < (sint32)x)
        return result;
    if ((sint32)size < 3)
    {
        bucket = (sint32)depth >> 3;
        packet = r_u32(state + 4);
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        ordering = r_u32(state);
        value = r_u32(ordering + bucket * 4);
        w_u32(packet, value | 0x02000000);
        ordering = r_u32(state);
        w_u32(ordering + bucket * 4, packet & 0xFFFFFF);
        angle = r_u32(context + 64);
        value = r_s16(context + 68);
        value += (uint32)((sint32)angle >> 16);
        value += angle;
        w_u32(packet + 4, (value & 0x800) != 0 ? 0x68636363 : 0x68181818);
        w_u32(packet + 8, screen);
        result = packet + 12;
        w_u32(state + 4, result);
        return result;
    }
    w_u32(context + 116, size);
    w_u32(context + 104, depth);
    h = r_s16(state + 26);
    if ((sint32)near_depth < (sint32)h)
    {
        value = r_u32(context + 112);
        w_u32(context + 108, 4);
        w_u32(context + 112, value << 4);
    }
    else
        w_u32(context + 108, 0);
    tx = xport_gte_read_data(25);
    ty = xport_gte_read_data(26);
    tz = xport_gte_read_data(27);
    shift = r_u32(context + 108);
    if (shift != 0)
    {
        tx <<= shift & 31;
        ty <<= shift & 31;
        tz <<= shift & 31;
    }
    value = r_u32(context + 112);
    sub_800867e4(context + 64, value, context + 32, 0);
    sub_80086948(context + 32);
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0xFFFFF800);
    xport_gte_write_data(2, 0x04000000);
    xport_gte_write_data(3, 512);
    xport_gte_write_data(4, 0xFD000000);
    value = r_u16(context + 64);
    xport_gte_write_data(5, (value & 7) << 9);
    size = r_u32(context + 116);
    depth = r_u32(context + 104);
    xport_gte_execute(0x280030);
    bucket = (sint32)depth >> 3;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    if ((sint32)size >= 7)
    {
        ordering = r_u32(state);
        angle = r_s16(context + 64);
        value = r_u32(ordering + bucket * 4);
        packet = r_u32(state + 4);
        w_u32(packet, value | 0x07000000);
        ordering = r_u32(state);
        w_u32(ordering + bucket * 4, packet & 0xFFFFFF);
        w_u32(packet + 12, (angle & 1) != 0 ? 0x45D610E0 : 0x45D61FEF);
        w_u32(packet + 20, 0x003810EF);
        w_u16(packet + 28, 0x1FE0);
        words = 8;
        color = 0x24000000;
        value = xport_gte_read_data(12);
        w_u32(packet + 8, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 24, value);
    }
    else
    {
        ordering = r_u32(state);
        value = r_u32(ordering + bucket * 4);
        packet = r_u32(state + 4);
        w_u32(packet, value | 0x04000000);
        ordering = r_u32(state);
        value = r_u32(state + 4);
        w_u32(ordering + bucket * 4, value & 0xFFFFFF);
        words = 5;
        color = 0x20000000;
        value = xport_gte_read_data(12);
        w_u32(packet + 8, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 12, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 16, value);
    }
    xport_gte_execute(0x1400006);
    value = xport_gte_read_data(24);
    color |= (sint32)value < 0 ? 0x636363 : 0x181818;
    packet = r_u32(state + 4);
    w_u32(packet + 4, color);
    result = r_u32(state + 4) + words * 4;
    w_u32(state + 4, result);
    return result;
}

uint32 sub_80086a0c(uint32 effect, uint32 state, uint32 context)
{
    uint32 value, camera, x, y, z, a, b, c, d, e, root, geometry, noise, angle, counter, divisor, radius, numerator, ratio, product, result, depth, h, size, near_depth, screen, fog, material, count, limit, bucket, shift, tx, ty, tz, packet, next, phase, modulation, i, j, word, lo, mid, hi, seed, ordering, start, tail, address, lane;
    FUNCTION_MARKER(0x80086A0C, "ACE3.BIN");
    value = r_u32(effect + 4);
    camera = r_u32(state + 8);
    x = (sint32)(value - camera) >> 12;
    if (x + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(effect + 8);
    camera = r_u32(state + 12);
    y = (sint32)(value - camera) >> 12;
    if (y + 0x8000 > 0xFFFF)
        return 1;
    value = r_u32(effect + 12);
    camera = r_u32(state + 16);
    z = (sint32)(value - camera) >> 12;
    if (z + 0x8000 > 0xFFFF)
        return 1;
    a = r_u32(context);
    b = r_u32(context + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(context + 8);
    d = r_u32(context + 12);
    e = r_u32(context + 16);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z);
    root = r_u32(0x800BCF90);
    geometry = root + 0x420;
    xport_gte_execute(0x180001);
    angle = r_u32(root + (effect & 0x1FC));
    w_u32(context + 64, angle);
    value = r_u16(effect + 22);
    counter = r_s16(effect);
    w_u16(context + 68, (value & 0x100) != 0 ? counter << 6 : 0);
    geometry += (uint32)((sint32)angle >> 4) & 0x1C;
    noise = root + (angle & 63) * 4;
    divisor = r_s16(effect + 2);
    radius = r_s16(effect + 26);
    if (divisor == 0)
        divisor = 1;
    numerator = counter << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    ratio = (sint32)numerator / (sint32)divisor;
    w_u32(context + 144, ratio);
    if ((sint32)ratio >= 0xAAB)
        noise += (counter << 1) & 12;
    value = r_s16(0x800B7104 + ((uint32)((sint32)ratio >> 2) << 2));
    product = radius * value;
    result = product;
    if ((sint32)result < 0)
        result += 4095;
    radius = (sint32)result >> 12;
    depth = xport_gte_read_data(19);
    w_u32(context + 112, radius);
    if ((sint32)depth <= 0)
        return result;
    h = r_s16(state + 26);
    product = radius * h;
    if (depth == 0)
        xport_mips_break(7);
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    near_depth = depth - size;
    if ((sint32)near_depth < ((sint32)h >> 4))
        return 1;
    screen = xport_gte_read_data(14);
    y = (sint32)screen >> 16;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)(size + 120) < (sint32)y)
        return 1;
    x = (sint16)screen;
    if ((sint32)x < 0)
        x = 0 - x;
    result = depth - 1024;
    if ((sint32)(size + 160) < (sint32)x)
        return result;
    fog = (sint32)result >> 2;
    if ((sint32)fog >= 4096)
        return 0;
    if ((sint32)fog < 0)
        fog = 0;
    xport_gte_write_data(8, fog);
    value = xport_gte_read_data(25);
    w_u32(context + 88, value);
    value = xport_gte_read_data(26);
    w_u32(context + 92, value);
    value = xport_gte_read_data(27);
    w_u32(context + 96, value);
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_write_data(6, 0x6A000000);
    material = r_u16(effect + 24);
    value = material & 255;
    if (value >= 7)
        value = 6;
    value = 0x800B43AC + value * 12;
    a = r_u32(value);
    xport_gte_write_data(20, a);
    a = r_u32(value + 4);
    xport_gte_write_data(21, a);
    a = r_u32(value + 8);
    xport_gte_write_data(22, a);
    count = (material >> 8) & 31;
    xport_gte_execute(0xF8002A);
    h = r_s16(state + 26);
    if ((sint32)near_depth < (sint32)h)
    {
        value = r_u32(context + 112);
        w_u32(context + 108, 4);
        w_u32(context + 112, value << 4);
    }
    else
        w_u32(context + 108, 0);
    if ((count & 16) != 0)
    {
        limit = (sint32)(size - 4) >> 1;
        count &= 15;
        if ((sint32)limit <= 0)
            limit = 1;
        if ((sint32)limit < (sint32)count)
            count = limit;
    }
    bucket = (sint32)depth >> 3;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    w_u32(context + 104, bucket);
    value = xport_gte_read_data(20);
    w_u32(context + 148, value);
    value = xport_gte_read_data(21);
    w_u32(context + 152, value);
    value = xport_gte_read_data(22);
    w_u32(context + 156, value);
    value = r_u32(context + 112);
    sub_800867e4(context + 64, value << 2, context + 32, 0);
    sub_80086948(context + 32);
    seed = xport_gte_read_control(2);
    tx = r_u32(context + 88);
    ty = r_u32(context + 92);
    shift = r_u32(context + 108);
    tz = r_u32(context + 96);
    if (shift != 0)
    {
        tx <<= shift & 31;
        ty <<= shift & 31;
        tz <<= shift & 31;
    }
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    noise += 4;
    phase = r_u32(context + 144) - 512;
    packet = r_u32(state + 4);
    modulation = phase + (tx & 1023);
    while ((sint32)count > 0)
    {
        xport_gte_write_data(8, modulation);
        value = r_u32(context + 148);
        xport_gte_write_data(20, value);
        value = r_u32(context + 152);
        xport_gte_write_data(21, value);
        value = r_u32(context + 156);
        xport_gte_write_data(22, value);
        word = r_u32(geometry);
        geometry += 4;
        xport_gte_execute(0xF8002A);
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 122, lo);
        w_u16(context + 124, hi);
        w_u16(context + 120, mid);
        word = r_u32(geometry);
        geometry += 4;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 128, lo);
        w_u16(context + 130, hi);
        w_u16(context + 132, mid);
        word = r_u32(geometry);
        geometry += 4;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 136, hi);
        w_u16(context + 138, mid);
        w_u16(context + 140, lo);
        value = xport_gte_read_data(20);
        w_u32(packet + 4, value);
        value = xport_gte_read_data(20);
        w_u32(packet + 12, value);
        value = xport_gte_read_data(21);
        w_u32(packet + 20, value);
        value = xport_gte_read_data(21);
        w_u32(packet + 28, value);
        value = xport_gte_read_data(22);
        w_u32(packet + 36, value);
        value = xport_gte_read_data(22);
        w_u32(packet + 44, value);
        for (i = 0; i < 6; ++i)
        {
            value = r_u32(context + 120 + i * 4);
            xport_gte_write_data(i, value);
        }
        word = r_u32(geometry);
        geometry += 4;
        xport_gte_execute(0x280030);
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 122, mid);
        w_u16(context + 124, lo);
        w_u16(context + 120, hi);
        word = r_u32(geometry);
        geometry += 4;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 128, mid);
        w_u16(context + 130, lo);
        w_u16(context + 132, hi);
        word = r_u32(geometry);
        geometry += 4;
        lo = (sint32)(word << 21) >> 21;
        mid = (sint32)(word << 11) >> 21;
        hi = (sint32)word >> 21;
        w_u16(context + 136, lo);
        w_u16(context + 138, hi);
        w_u16(context + 140, mid);
        value = xport_gte_read_data(12);
        w_u32(packet + 8, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 24, value);
        for (i = 0; i < 6; ++i)
        {
            value = r_u32(context + 120 + i * 4);
            xport_gte_write_data(i, value);
        }
        next = packet + 52;
        w_u32(packet, (next & 0xFFFFFF) | 0x0C000000);
        xport_gte_execute(0x280030);
        value = r_u32(noise);
        noise += 4;
        modulation = phase + (value & 1023);
        value = xport_gte_read_data(12);
        w_u32(packet + 32, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 40, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 48, value);
        --count;
        packet = next;
        seed = next;
    }
    tail = packet - 52;
    bucket = r_u32(context + 104);
    ordering = r_u32(state);
    ordering += bucket * 4;
    start = r_u32(state + 4);
    address = ordering + 2;
    lane = address & 3;
    value = r_u32(address & 0xFFFFFFFC) << (24 - (lane << 3));
    if (lane != 3)
        value |= seed & (0xFFFFFF >> (lane << 3));
    result = start << 8;
    address = tail + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (j = 0; j <= lane; ++j)
        w_u8(address + j, value >> (shift + j * 8));
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - (lane << 3);
    address &= 0xFFFFFFFC;
    for (j = 0; j <= lane; ++j)
        w_u8(address + j, result >> (shift + j * 8));
    w_u32(state + 4, packet);
    return result;
}

uint32 sub_80068ba8(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 effect;
    FUNCTION_MARKER(0x80068BA8, "ACE3.BIN");
    effect = sub_800685c0(flags, position, vector, owner, native_position);
    if (effect == 0)
        return 0;
    w_u16(effect + 26, 4096);
    w_u8(effect + 5, 3);
    w_u32(effect + 132, 5120);
    w_u32(effect + 136, 5120);
    w_u8(effect + 6, 0);
    w_u8(effect + 7, 0);
    w_u32(effect + 128, 0x12C000);
    w_u16(effect + 24, 90);
    return effect;
}

uint32 sub_800689e8(uint32 flags, uint32 position, uint32 vector, uint32 owner, const NATIVE_POSE *native_position)
{
    uint32 effect, value;
    FUNCTION_MARKER(0x800689E8, "ACE3.BIN");
    effect = sub_800685c0(flags, position, vector, owner, native_position);
    if (effect == 0)
        return 0;
    w_u16(effect + 26, 8192);
    w_u8(effect + 52, 4);
    w_u8(effect + 5, 2);
    w_u16(effect + 24, 300);
    w_u32(effect + 156, 400);
    value = r_u32(effect);
    w_u8(effect + 6, 0);
    w_u8(effect + 7, 0);
    w_u32(effect + 160, 192);
    w_u32(effect, value | 0x10);
    return effect;
}

uint32 sub_80047d68(uint32 index)
{
    uint32 states, records, table, state, target_index, object, target, trigger, settings, scene, mode, value, pool, flags, duration, count, kind, type, parameter, owner, sound;
    FUNCTION_MARKER(0x80047D68, "ACE3.BIN");
    states = r_u32(0x800BCEC0);
    records = r_u32(0x800BCEF8);
    state = states + index * 184;
    table = r_u32(0x800BCE5C);
    target_index = r_u8(state + 96);
    object = r_u32(table + (index << 2));
    target = r_u32(table + target_index * 4);
    trigger = r_u32(records + (index << 6) + 28);
    settings = r_u32(records + (index << 6) + 24);
    if (trigger == 0)
        return 0;
    if (r_u16(trigger) == 0)
        return 0;
    scene = r_u32(0x800BCE60);
    mode = r_s16(scene + 0x38C);
    if (mode == 23)
    {
        scene = r_u32(0x800BCE54);
        value = r_u32(scene + 8);
        if (value == 0)
        {
            value = r_u32(object + 12);
            if ((sint32)0xFC400000 < (sint32)value)
                return 0;
            value = r_u32(object + 8);
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 0x0946FFFF)
                return 0;
            value = r_u32(object + 16);
            if ((sint32)value < 0)
                value = 0 - value;
            if ((sint32)value <= 0x0946FFFF)
                return 0;
        }
    }
    target_index = r_u8(state + 96);
    if (target_index < 32)
    {
        pool = r_u32(0x800BD080);
        target_index = r_u8(state + 96);
        value = r_u32(pool);
        if ((value & (1u << (target_index & 31))) != 0)
            goto reset_charge;
    }
    else
    {
        target_index = r_u8(state + 96);
        pool = r_u32(0x800BD080);
        value = r_u32(pool + 4);
        if ((value & (1u << ((target_index - 32) & 31))) != 0)
            goto reset_charge;
    }
    value = r_s16(state + 180);
    mode = r_u32(trigger + 4);
    if ((sint32)value >= (sint32)mode)
        goto reset_charge;
    count = r_u32(state + 160);
    value = r_u32(trigger + 8);
    if (value < count)
        goto reset_charge;
    value = r_u16(state + 94);
    w_u16(state + 94, value + 1);
    flags = r_u32(object);
    duration = r_s16(settings + 22);
    if ((flags & 0x80) != 0)
    {
        scene = r_u32(0x800BCE60);
        mode = r_u32(scene + 0x394);
        if (mode == 0)
            duration <<= 1;
        if ((sint32)mode >= 2)
        {
            duration = (sint32)duration >> 1;
            if ((sint32)duration < 60)
                duration = 60;
        }
    }
    count = r_s16(state + 94);
    if ((sint32)count < (sint32)duration)
    {
        target_index = r_u8(state + 96);
        if (target_index == 0)
            goto charge_feedback;
        value = r_u32(0x800C6FD0 + target_index * 4);
        if ((value & 0x400) != 0)
            goto finish_fire;
    }
    kind = r_u16(trigger);
    if (kind == 3)
    {
        target_index = r_u8(state + 96);
        flags = r_u32(0x800C6FD0 + target_index * 4);
        if ((flags & 0x400) != 0)
            kind = 1;
    }
    if (kind == 1 || kind == 4)
    {
        scene = r_u32(0x800BCE60);
        mode = r_u32(scene + 0x394);
        type = 0;
        if (mode != 0 || (r_u32(object) & 0x80) == 0)
        {
            records = r_u32(0x800BCEF8);
            value = r_u32(records + (index << 6) + 24);
            type = r_u8(value);
        }
        if (type == 0)
            parameter = 0x08000002;
        else if (type == 1)
            parameter = 0x102;
        else if (type == 2)
            parameter = 0x10102;
        else if (type == 3)
        {
            value = r_u32(object);
            if ((value & 0x40) != 0)
                parameter = 0x20202;
            else
            {
                scene = r_u32(0x800BCE60);
                value = r_u32(scene + 0x394);
                parameter = value == 3 ? 0x20202 : 0x10202;
            }
        }
        else
            parameter = 0x10202;
        flags = r_u32(object);
        owner = r_u32(target);
        sub_80068800(flags, object + 8, object + 56, owner, parameter, 0);
    }
    else if (kind == 2 || kind == 3 || kind == 8 || kind == 9)
    {
        flags = r_u32(object);
        owner = r_u32(target);
        if (kind == 2)
            sub_80068c0c(flags, object + 8, object + 56, owner, 0);
        else if (kind == 3)
            sub_80068c80(flags, object + 8, object + 56, owner, 0);
        else if (kind == 8)
            sub_80068ba8(flags, object + 8, object + 56, owner, 0);
        else
            sub_800689e8(flags, object + 8, object + 56, owner, 0);
    }
    w_u16(state + 94, 0);
    flags = r_u32(0x800C6FD0 + (index << 2));
    if ((flags & 0x800000) != 0)
    {
        table = r_u32(0x800BCE5C);
        value = r_u32(table + (index << 2));
        flags = r_u32(value);
        if ((flags & 0x40) != 0)
        {
            target_index = r_u8(state + 96);
            if (target_index < 32)
            {
                records = r_u32(0x800BCEF8);
                value = r_u32(records + (index << 6) + 16);
                sound = r_u8(value + 3);
                sub_80048c0c(3, 0, sound, 0);
            }
        }
        table = r_u32(0x800BCE5C);
        value = r_u32(table + (index << 2));
        flags = r_u32(value);
        if ((flags & 0x80) != 0)
        {
            records = r_u32(0x800BCEF8);
            value = r_u32(records + (index << 6) + 16);
            sound = r_u8(value + 3);
            sub_80048c0c(7, 0, sound, 0);
        }
    }
finish_fire:
    target_index = r_u8(state + 96);
    if (target_index != 0)
        return 0;
charge_feedback:
    count = r_s16(state + 94);
    if (((sint32)duration >> 1) < (sint32)count)
    {
        value = r_s16(settings + 22);
        if ((sint32)count < (sint32)value)
        {
            value = r_u32(0x800C6ECC);
            w_u32(0x800C6ECC, value | 1);
        }
        count = r_s16(state + 94);
    }
    value = (sint32)duration >> 2;
    if (count != value)
        return value;
    flags = r_u32(0x800C6FD0 + (index << 2));
    if ((flags & 0x800000) != 0 && (r_u32(object) & 0x80) != 0)
    {
        records = r_u32(0x800BCEF8);
        value = r_u32(records + (index << 6) + 16);
        sound = r_u8(value + 3);
        return sub_80048c0c(6, 0, sound, 0);
    }
    return sub_80048c0c(0, 0, 0, 0);
reset_charge:
    w_u16(state + 94, 0);
    return 0;
}

uint32 sub_8004925c(uint32 id, uint32 mission, uint32 kind)
{
    uint32 slot, value, mask;
    FUNCTION_MARKER(0x8004925C, "ACE3.BIN");
    if ((sint32)mission < 31)
        return 0;
    switch (mission)
    {
        case 31:
            slot = 0;
            break;
        case 32:
            slot = 1;
            break;
        case 33:
            slot = 2;
            break;
        case 38:
            slot = 3;
            break;
        case 39:
            slot = 4;
            break;
        case 47:
            slot = 5;
            break;
        case 48:
            slot = 6;
            break;
        case 53:
            slot = 7;
            break;
        case 58:
            slot = 8;
            break;
        default:
            return 0;
    }
    if (id == 1)
    {
        switch (kind)
        {
            case 6:
                mask = 0x10000000;
                break;
            case 7:
                mask = 0x100000;
                break;
            case 8:
                mask = 0x1000;
                break;
            case 9:
                mask = 0x10;
                break;
            default:
                return 0;
        }
    }
    else if (id == 4)
    {
        switch (kind)
        {
            case 6:
                mask = 0x1000000;
                break;
            case 7:
                mask = 0x10000;
                break;
            case 8:
                mask = 0x100;
                break;
            case 9:
                mask = 1;
                break;
            default:
                return 0;
        }
    }
    else
        return 0;
    value = r_u32(0x800B17C4 + slot * 4);
    return value & mask;
}

uint32 sub_80048c0c(uint32 kind, uint32 value, uint32 id, uint32 code)
{
    uint32 scene, root, flags, mission, tick, offset, bit, entry, mask;
    FUNCTION_MARKER(0x80048C0C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    root = r_u32(0x800BCE60);
    flags = r_u32(scene + 4);
    mission = r_s16(root + 0x38C);
    if ((flags & 4) == 0)
        return 0;
    if (r_s16(0x800C7194) != 1)
        return 0;
    if (kind < 10)
    {
        scene = r_u32(0x800BCE54);
        tick = r_u32(scene + 12);
        mask = kind == 8 ? 15 : kind == 1 || kind == 2 || kind == 3 || kind == 7 ? 3 : 7;
        if ((tick & mask) != 0)
            return 0;
        if (kind <= 2)
        {
            flags = r_u32(0x800C7190);
            if (kind == 0)
            {
                bit = 0x10;
                offset = 0;
                code = 0x68;
                if ((flags & bit) != 0)
                {
                    bit = 0x20;
                    offset = 1;
                    code = 0x69;
                }
            }
            else if (kind == 1)
            {
                bit = 0x40;
                offset = 2;
                code = 0x6A;
                if ((flags & bit) != 0)
                {
                    bit = 0x80;
                    offset = 3;
                    code = 0x6B;
                }
            }
            else
            {
                bit = 0x100;
                offset = 4;
                code = 0x6C;
            }
            if ((flags & bit) != 0)
                return 0;
            id = r_u8(0x800B1520 + mission * 11 + offset);
            if (id == 0)
                return 0;
            w_u32(0x800C7190, flags | bit);
        }
        else if (kind == 3)
        {
            entry = id & 255;
            if (entry >= 7)
                return 0;
            offset = r_u8(0x800B1518 + entry);
            mask = r_u8(0x800B1520 + mission * 11 + offset);
            if (entry != mask)
                return 0;
            if (offset == 255)
                return 0;
            flags = r_u32(0x800C7190);
            bit = 1u << (offset & 31);
            if ((flags & bit) != 0)
                return 0;
            code = r_u8(0x800B17C0 + offset);
            w_u32(0x800C7190, flags | bit);
        }
        else if (kind == 4 || kind == 5)
        {
            flags = r_u32(0x800C7190);
            bit = kind == 4 ? 0x200 : 0x400;
            if ((flags & bit) != 0)
                return 0;
            entry = r_u8(0x800B1520 + mission * 11 + (kind == 4 ? 9 : 10));
            if ((id & 255) != entry)
                return 0;
            w_u32(0x800C7190, flags | bit);
            code = kind == 4 ? 0x75 : 0x76;
        }
        else
        {
            entry = id & 255;
            if (sub_8004925c(entry, mission, kind) == 0)
                return 0;
            if (entry != 1 && entry != 4)
                return 0;
            flags = r_u32(0x800C7190);
            offset = (kind - 6) * 2 + (entry == 4);
            bit = 1u << (11 + offset);
            code = 0x78 + offset;
            if ((flags & bit) != 0)
                return 0;
            w_u32(0x800C7190, flags | bit);
        }
    }
    w_u8(0x800C7199, code);
    w_u16(0x800C7196, value);
    w_u8(0x800C719A, id);
    return 1;
}

uint32 sub_8006d0dc(uint32 input, uint32 control, uint32 output, uint32 smooth)
{
    uint32 axis[4], bounds[3], target[3], step[3], flags, mode, value, raw, magnitude, current, product, high, delta, i, limit;
    FUNCTION_MARKER(0x8006D0DC, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;
    axis[3] = 0;
    bounds[0] = 4096;
    bounds[1] = 4096;
    bounds[2] = 4096;
    mode = r_u32(input + 24);
    flags = r_u32(input + 8);
    if ((mode & 1) != 0)
    {
        axis[0] = r_u32(input + 28);
        bounds[0] = (sint32)axis[0] < 0 ? 0 - axis[0] : axis[0];
    }
    else
    {
        if ((flags & 0x800000) != 0)
            axis[0] = 4096;
        if ((flags & 0x1000000) != 0)
            axis[0] -= 4096;
    }
    mode = r_u32(input + 24);
    if ((mode & 4) != 0)
    {
        axis[1] = r_u32(input + 36);
        bounds[1] = (sint32)axis[1] < 0 ? 0 - axis[1] : axis[1];
    }
    else
    {
        if ((flags & 0x20000000) != 0)
            axis[1] -= 4096;
        if ((flags & 0x40000000) != 0)
            axis[1] += 4096;
    }
    mode = r_u32(input + 24);
    if ((mode & 2) != 0)
    {
        axis[2] = r_u32(input + 32);
        bounds[2] = (sint32)axis[2] < 0 ? 0 - axis[2] : axis[2];
    }
    else
    {
        if ((flags & 0x2000000) != 0)
            axis[2] -= 4096;
        if ((flags & 0x4000000) != 0)
            axis[2] += 4096;
    }
    mode = r_u32(input + 24);
    if ((mode & 8) != 0)
        axis[3] = r_u32(input + 40);
    else
    {
        if ((flags & 0x8000000) != 0)
            axis[3] += 4096;
        if ((flags & 0x10000000) != 0)
            axis[3] -= 4096;
    }
    for (i = 0; i < 4; ++i)
    {
        value = r_s16(control + i * 2);
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (uint32)(((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (uint32)(((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            w_u16(control + i * 2, value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)target[i] < (sint32)value)
            w_u16(smooth + i * 2, raw - step[i]);
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)value < (sint32)target[i])
            w_u16(smooth + i * 2, raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            w_u16(smooth + i * 2, target[i]);
    }
    limit = 0;
    if ((sint32)axis[3] < -2048)
        limit = (0 - (axis[3] + 2048)) << 1;
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < (sint32)limit)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)limit < (sint32)value)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(control + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    value = r_u16(control);
    w_u16(control, value + (uint32)((sint32)axis[0] >> 3));
    value = r_u16(control + 2);
    w_u16(control + 2, value + (uint32)((sint32)axis[1] >> 2));
    value = r_u16(control + 4);
    w_u16(control + 4, value + (uint32)((sint32)axis[2] >> 5));
    value = r_u16(control + 6);
    w_u16(control + 6, value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(control + i * 2);
        if ((sint32)value < (sint32)(0 - bounds[i]))
            w_u16(control + i * 2, 0 - bounds[i]);
        value = r_s16(control + i * 2);
        if ((sint32)bounds[i] < (sint32)value)
            w_u16(control + i * 2, bounds[i]);
    }
    value = r_s16(control + 6);
    if ((sint32)value < -4096)
        w_u16(control + 6, -4096);
    value = r_s16(control + 6);
    if ((sint32)value > 4096)
        w_u16(control + 6, 4096);
    value = r_u16(control);
    w_u16(output + 4, value);
    value = r_u16(control + 2);
    w_u16(output + 6, value);
    value = r_u16(control + 4);
    w_u16(output + 8, value);
    value = r_u16(control + 6);
    w_u16(output + 14, value);
    return value;
}

uint32 sub_80073b18(uint32 input, uint32 control, uint32 output, uint32 smooth, uint32 enabled)
{
    uint32 axis[4], bounds[3], target[3], step[3], value, raw, magnitude, current, product, high, delta, i, limit, roll, height, pitch;
    FUNCTION_MARKER(0x80073B18, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[3] = 0;
    roll = r_s16(input + 28);
    axis[2] = (0 - roll) << 2;
    height = r_u32(input + 12);
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    if ((sint32)height < (sint32)0xFF448000 || enabled == 0)
    {
        if ((sint32)magnitude < 2048)
        {
            pitch = r_s16(input + 24);
            axis[0] = (0 - pitch) << 4;
        }
        pitch = r_s16(input + 24);
        value = (sint32)pitch < 0 ? 0 - pitch : pitch;
        if ((sint32)value < 182)
            axis[0] = (sint32)axis[0] >> 1;
    }
    else if ((sint32)magnitude < 2048)
    {
        pitch = r_s16(input + 24);
        if ((sint32)pitch < 2048)
            axis[0] = (2048 - pitch) << 4;
        axis[3] = 4096;
    }
    roll = r_s16(input + 28);
    magnitude = (sint32)roll < 0 ? 0 - roll : roll;
    if ((sint32)magnitude < 455)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)magnitude < 273)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)magnitude < 136)
        axis[2] = (sint32)axis[2] >> 1;
    if ((sint32)axis[0] < -4096)
        axis[0] = -4096;
    else if ((sint32)axis[0] > 4096)
        axis[0] = 4096;
    if ((sint32)axis[2] < -4096)
        axis[2] = -4096;
    else if ((sint32)axis[2] > 4096)
        axis[2] = 4096;
    bounds[0] = (sint32)axis[0] < 0 ? 0 - axis[0] : axis[0];
    bounds[1] = 0;
    bounds[2] = (sint32)axis[2] < 0 ? 0 - axis[2] : axis[2];
    for (i = 0; i < 4; ++i)
    {
        value = r_s16(control + i * 2);
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (uint32)(((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (uint32)(((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            w_u16(control + i * 2, value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)target[i] < (sint32)value)
            w_u16(smooth + i * 2, raw - step[i]);
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)value < (sint32)target[i])
            w_u16(smooth + i * 2, raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            w_u16(smooth + i * 2, target[i]);
    }
    limit = 0;
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < (sint32)limit)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)limit < (sint32)value)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(control + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    value = r_u16(control);
    raw = r_u16(control + 2);
    w_u16(control, value + (uint32)((sint32)axis[0] >> 3));
    value = r_u16(control + 4);
    w_u16(control + 4, value + (uint32)((sint32)axis[2] >> 5));
    value = r_u16(control + 6);
    w_u16(control + 6, value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(control + i * 2);
        if ((sint32)value < (sint32)(0 - bounds[i]))
            w_u16(control + i * 2, 0 - bounds[i]);
        value = r_s16(control + i * 2);
        if ((sint32)bounds[i] < (sint32)value)
            w_u16(control + i * 2, bounds[i]);
    }
    value = r_s16(control + 6);
    if ((sint32)value < -4096)
        w_u16(control + 6, -4096);
    value = r_s16(control + 6);
    if ((sint32)value > 4096)
        w_u16(control + 6, 4096);
    value = r_u16(control);
    w_u16(output + 4, value);
    value = r_u16(control + 2);
    w_u16(output + 6, value);
    value = r_u16(control + 4);
    w_u16(output + 8, value);
    value = r_u16(control + 6);
    w_u16(output + 14, value);
    return value;
}

uint32 sub_80072938(uint32 object, uint32 settings)
{
    uint32 root, flags, control, output, smooth, physics;
    FUNCTION_MARKER(0x80072938, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    flags = r_u32(root + 0x128);
    if ((sint32)flags < 0)
    {
        control = r_u32(0x800BD088);
        output = r_u32(0x800BD08C);
        smooth = r_u32(0x800BD090);
        return sub_80073b18(object, control, output, smooth, 0);
    }
    flags = r_u32(root + 0x120);
    if ((flags & 4) != 0)
    {
        control = r_u32(0x800BD088);
        output = r_u32(0x800BD08C);
        smooth = r_u32(0x800BD090);
        physics = r_u32(0x800BD094);
        return sub_8006dd5c(root + 0x120, control, output, smooth, object, settings, physics);
    }
    control = r_u32(0x800BD088);
    output = r_u32(0x800BD08C);
    smooth = r_u32(0x800BD090);
    return sub_8006d0dc(root + 0x120, control, output, smooth);
}

uint32 sub_8006dd5c(uint32 input, uint32 control, uint32 output, uint32 smooth, uint32 object, uint32 settings, uint32 physics)
{
    uint32 axis[4], bounds[3], target[3], step[3], flags, mode, value, raw, magnitude, current, product, high, delta, i, limit;
    uint32 command_roll, command_pitch, roll, pitch, abs_roll, abs_pitch, factor, correction, strength, sensitivity, term, tilt, blend;
    FUNCTION_MARKER(0x8006DD5C, "ACE3.BIN");
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;
    axis[3] = 0;
    bounds[1] = 4096;
    command_roll = 0;
    command_pitch = 0;
    mode = r_u32(input + 24);
    flags = r_u32(input + 8);
    if ((mode & 2) != 0)
        command_roll = r_u32(input + 32);
    else
    {
        if ((flags & 0x2000000) != 0)
            command_roll = -4096;
        if ((flags & 0x4000000) != 0)
            command_roll += 4096;
    }
    mode = r_u32(input + 24);
    if ((mode & 1) != 0)
        command_pitch = r_u32(input + 28);
    else
    {
        if ((flags & 0x800000) != 0)
            command_pitch += 4096;
        if ((flags & 0x1000000) != 0)
            command_pitch -= 4096;
    }
    magnitude = (sint32)command_roll < 0 ? 0 - command_roll : command_roll;
    roll = r_s16(object + 28);
    pitch = r_s16(object + 24);
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    factor = 4096 - abs_pitch;
    correction = 0;
    if ((sint32)magnitude < 2048)
    {
        product = (0 - roll) * (2048 - magnitude);
        if ((sint32)product < 0)
            product += 2047;
        term = (sint32)product >> 11;
        product = term * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction = (sint32)product >> 12;
    }
    value = r_s16(object + 28);
    if ((sint32)value > 2048)
    {
        product = (abs_roll - 2048) * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction -= (uint32)((sint32)product >> 12);
    }
    value = r_s16(object + 28);
    if ((sint32)value < -2048)
    {
        product = (abs_roll - 2048) * factor;
        if ((sint32)product < 0)
            product += 4095;
        correction += (uint32)((sint32)product >> 12);
    }
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    product = correction * (4096 - magnitude);
    if ((sint32)product < 0)
        product += 4095;
    correction = (sint32)product >> 12;
    roll = r_s16(object + 28);
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    strength = 4096;
    if (((sint32)roll > 0 && (sint32)command_roll > 0) || ((sint32)roll < 0 && (sint32)command_roll < 0))
    {
        if ((sint32)abs_roll >= 1024)
        {
            product = (abs_roll - 1024) << 12;
            strength -= (uint32)((sint32)product / 1024);
        }
        if ((sint32)strength > 4096)
            strength = 4096;
        if ((sint32)strength < 0)
            strength = 0;
        sensitivity = r_u32(settings);
    }
    else
        sensitivity = r_u32(settings + 4);
    product = strength * command_roll;
    if ((sint32)product < 0)
        product += 4095;
    correction += (uint32)((sint32)product >> 12);
    value = r_u16(object + 28);
    value = r_u16(0x800104AC + ((value & 0x3FFF) << 1));
    term = (sint16)value >> 2;
    if ((sint32)term > 4096)
        term = 4096;
    if ((sint32)term < -4096)
        term = -4096;
    product = term * sensitivity;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    value = r_s16(physics + 76);
    product = term * value;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    raw = r_u16(object + 26);
    w_u16(object + 26, raw + term);
    roll = r_s16(object + 28);
    abs_roll = (sint32)roll < 0 ? 0 - roll : roll;
    strength = 4096;
    if ((sint32)abs_roll >= 2048)
    {
        product = (abs_roll - 2048) << 12;
        strength -= (uint32)((sint32)product / 2048);
    }
    product = strength * command_pitch;
    if ((sint32)product < 0)
        product += 4095;
    tilt = (sint32)product >> 12;
    if ((sint32)abs_roll >= 4096)
        tilt = 0 - tilt;
    strength = 0;
    if ((sint32)abs_roll >= 2048)
    {
        product = (abs_roll - 2048) << 12;
        strength = (sint32)product / 2048;
    }
    value = r_s16(object + 28);
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    product = ((sint32)value < 0 ? strength : 0 - strength) * magnitude;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    pitch = r_s16(object + 24);
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    product = term * (4096 - abs_pitch);
    if ((sint32)abs_roll < 4096)
    {
        if ((sint32)product < 0)
            product += 4095;
        correction += (uint32)((sint32)product >> 12);
    }
    pitch = r_s16(object + 24);
    magnitude = (sint32)command_pitch < 0 ? 0 - command_pitch : command_pitch;
    abs_pitch = (sint32)pitch < 0 ? 0 - pitch : pitch;
    product = abs_pitch * magnitude;
    raw = r_u16(object + 24);
    if ((sint32)product < 0)
        product += 4095;
    blend = (sint32)product >> 12;
    sensitivity = r_u32(settings + 4);
    if ((sint32)abs_roll > 4096)
        sensitivity = 0 - sensitivity;
    value = r_s16(physics + 76);
    product = sensitivity * value;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    product = term * blend;
    if ((sint32)product < 0)
        product += 4095;
    term = (sint32)product >> 12;
    axis[0] = tilt;
    bounds[0] = (sint32)tilt < 0 ? 0 - tilt : tilt;
    axis[2] = correction;
    w_u16(object + 24, raw + term);
    mode = r_u32(input + 24);
    bounds[2] = (sint32)correction < 0 ? 0 - correction : correction;
    if ((mode & 4) != 0)
    {
        axis[1] = r_u32(input + 36);
        bounds[1] = (sint32)axis[1] < 0 ? 0 - axis[1] : axis[1];
    }
    else
    {
        if ((flags & 0x20000000) != 0)
            axis[1] -= bounds[1];
        if ((flags & 0x40000000) != 0)
            axis[1] += bounds[1];
    }
    if ((sint32)axis[1] > 4096)
        axis[1] = 4096;
    if ((sint32)axis[1] < -4096)
        axis[1] = -4096;
    if ((sint32)bounds[1] > 4096)
        bounds[1] = 4096;
    if ((sint32)bounds[1] < -4096)
        bounds[1] = -4096;
    mode = r_u32(input + 24);
    if ((mode & 8) != 0)
        axis[3] = r_u32(input + 40);
    else
    {
        if ((flags & 0x8000000) != 0)
            axis[3] += 4096;
        if ((flags & 0x10000000) != 0)
            axis[3] -= 4096;
    }
    for (i = 0; i < 4; ++i)
    {
        value = r_s16(control + i * 2);
        magnitude = (sint32)axis[i] < 0 ? 0 - axis[i] : axis[i];
        current = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)current)
        {
            product = value << 7;
            if (i < 3)
            {
                high = (uint32)(((sint64)(sint32)product * 0x51EB851F) >> 32);
                value = (uint32)((sint32)high >> 6) - (uint32)((sint32)product >> 31);
            }
            else
            {
                high = (uint32)(((sint64)(sint32)product * (sint64)(sint32)0xEA0EA0EB) >> 32);
                value = (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
            }
            w_u16(control + i * 2, value);
        }
    }
    target[0] = axis[2];
    target[1] = axis[0];
    target[2] = axis[1];
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        step[i] = (uint32)((sint32)delta >> 2) + 2;
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)target[i] < (sint32)value)
            w_u16(smooth + i * 2, raw - step[i]);
        value = r_s16(smooth + i * 2);
        raw = r_u16(smooth + i * 2);
        if ((sint32)value < (sint32)target[i])
            w_u16(smooth + i * 2, raw + step[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(smooth + i * 2);
        delta = value - target[i];
        if ((sint32)delta < 0)
            delta = target[i] - value;
        if ((sint32)delta < (sint32)step[i])
            w_u16(smooth + i * 2, target[i]);
    }
    limit = 0;
    if ((sint32)axis[3] < -2048)
        limit = (0 - (axis[3] + 2048)) << 1;
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)value < (sint32)limit)
        w_u16(smooth + 8, raw + 128);
    value = r_s16(smooth + 8);
    raw = r_u16(smooth + 8);
    if ((sint32)limit < (sint32)value)
        w_u16(smooth + 8, raw - 128);
    value = r_s16(control + 6);
    w_u16(smooth + 6, (sint32)(value + 4096) >> 1);
    value = r_u16(control);
    w_u16(control, value + (uint32)((sint32)axis[0] >> 3));
    value = r_u16(control + 2);
    w_u16(control + 2, value + (uint32)((sint32)axis[1] >> 2));
    value = r_u16(control + 4);
    w_u16(control + 4, value + (uint32)((sint32)axis[2] >> 5));
    value = r_u16(control + 6);
    w_u16(control + 6, value + (uint32)((sint32)axis[3] >> 5));
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(control + i * 2);
        if ((sint32)value < (sint32)(0 - bounds[i]))
            w_u16(control + i * 2, 0 - bounds[i]);
        value = r_s16(control + i * 2);
        if ((sint32)bounds[i] < (sint32)value)
            w_u16(control + i * 2, bounds[i]);
    }
    value = r_s16(control + 6);
    if ((sint32)value < -4096)
        w_u16(control + 6, -4096);
    value = r_s16(control + 6);
    if ((sint32)value > 4096)
        w_u16(control + 6, 4096);
    value = r_u16(control);
    w_u16(output + 4, value);
    value = r_u16(control + 2);
    w_u16(output + 6, value);
    value = r_u16(control + 4);
    w_u16(output + 8, value);
    value = r_u16(control + 6);
    w_u16(output + 14, value);
    return value;
}

uint32 sub_8008c0a0(uint32 state)
{
    uint32 root, data, packed, count, result, record, strength, context, value, color, scene, source, i, offset, a, b, c, input, component, product, packet, seed, depth, bucket, ordering, start, address, lane, shift, j;
    FUNCTION_MARKER(0x8008C0A0, "ACE3.BIN");
    root = r_u32(0x800BCE50);
    data = r_u32(root + 12);
    packed = r_u32(data + 12);
    count = packed & 255;
    result = (sint32)count < 3;
    if (count == 0)
        return result;
    if (count >= 3)
        count = 2;
    record = r_u32(state + 0x3FC);
    w_u32(state + 16, count);
    strength = r_s16(record + 6);
    if ((sint32)strength <= 0)
        return result;
    context = state + 16;
    if ((sint32)strength > 4096)
        strength = 4096;
    input = 0;
    if (((packed >> 8) & 255) != 0)
        input = r_u32(record);
    w_u32(context + 8, input);
    scene = r_u32(0x800BCE60);
    value = r_u32(scene + 40);
    w_u32(context + 4, value);
    if ((value & 1) != 0)
        color = 0x333333;
    else
    {
        xport_gte_write_data(8, strength);
        data = r_u32(root + 12);
        value = r_u32(data + 16);
        xport_gte_write_data(6, value);
        packed = r_u32(data + 20);
        xport_gte_write_control(21, (packed & 255) << 4);
        packed = (sint32)packed >> 8;
        xport_gte_write_control(22, (packed & 255) << 4);
        xport_gte_write_control(23, ((uint32)((sint32)packed >> 4)) & 0xFF0);
        xport_gte_execute(0x780010);
        color = xport_gte_read_data(22) & 0xFFFFFF;
    }
    scene = r_u32(0x800BCE60);
    value = r_u32(scene);
    w_u32(context + 80, value & 0x20000000);
    record = r_u32(state + 0x3FC);
    packed = r_u32(record + 12);
    value = r_u32(context + 80);
    w_u32(context + 12, packed);
    if (value == 0)
        w_u32(context + 12, packed << 2);
    root = r_u32(0x800BCE50);
    data = r_u32(root + 12);
    value = r_u32(data + 120);
    w_u32(context + 76, value);
    for (i = 0, offset = 0; i < count; ++i, offset += 32)
    {
        root = r_u32(0x800BCE50);
        data = r_u32(root + 12);
        source = data + 88 + i * 16;
        a = r_u32(source);
        b = r_u32(source + 4);
        w_u32(context + 20, a);
        w_u32(context + 24, b);
        a = r_u32(source + 8);
        b = r_u32(source + 12);
        w_u32(context + 28, a);
        w_u32(context + 32, b);
        input = r_u32(context + 8);
        if (input != 0)
        {
            component = (sint16)input;
            if (component != 0)
            {
                root = r_u32(0x800BCE50);
                data = r_u32(root + 12);
                value = r_s16(data + offset + ((sint32)component < 0 ? 30 : 38));
                product = value * ((sint32)component < 0 ? 0 - component : component);
                value = r_u16(context + 30);
                w_u16(context + 30, value + (uint32)((sint32)product >> 12));
            }
            component = (sint32)input >> 16;
            if (component != 0)
            {
                root = r_u32(0x800BCE50);
                data = r_u32(root + 12);
                value = r_s16(data + offset + ((sint32)component < 0 ? 54 : 46));
                product = value * ((sint32)component < 0 ? 0 - component : component);
                value = r_u16(context + 30);
                w_u16(context + 30, value + (uint32)((sint32)product >> 12));
            }
        }
        value = r_s16(context + 20);
        xport_gte_write_data(9, value);
        value = r_s16(context + 22);
        xport_gte_write_data(10, value);
        value = r_s16(context + 24);
        xport_gte_write_data(11, value);
        value = r_s16(context + 28);
        xport_gte_write_control(21, value);
        value = r_s16(context + 30);
        xport_gte_write_control(22, value);
        value = r_s16(context + 32);
        xport_gte_write_control(23, value);
        xport_gte_write_data(8, strength);
        root = r_u32(0x800BCE50);
        data = r_u32(root + 12);
        source = data + offset + 24;
        xport_gte_execute(0x980011);
        a = r_u32(source);
        b = r_u32(source + 4);
        w_u32(context + 44, a);
        w_u32(context + 48, b);
        a = r_u32(source + 8);
        b = r_u32(source + 12);
        w_u32(context + 52, a);
        w_u32(context + 56, b);
        value = xport_gte_read_data(25);
        w_u16(context + 36, value);
        value = xport_gte_read_data(26);
        w_u16(context + 38, value);
        value = xport_gte_read_data(27);
        w_u16(context + 40, value);
        value = r_u32(context + 44);
        xport_gte_write_data(0, value);
        value = r_u32(context + 48);
        xport_gte_write_data(1, value);
        value = r_u32(context + 52);
        xport_gte_write_data(2, value);
        value = r_u32(context + 56);
        xport_gte_write_data(3, value);
        value = r_u32(source + 16);
        xport_gte_write_data(4, value);
        value = r_u32(source + 20);
        xport_gte_write_data(5, value);
        packet = r_u32(state + 4);
        w_u32(packet + 20, color);
        xport_gte_execute(0x280030);
        w_u32(packet, ((packet + 36) & 0xFFFFFF) | 0x08000000);
        w_u32(packet + 56, color);
        w_u32(packet + 4, 0x3A030303);
        w_u32(packet + 36, ((packet + 72) & 0xFFFFFF) | 0x08000000);
        w_u32(packet + 40, 0x3A030303);
        value = r_u32(context + 80);
        if (value == 0)
        {
            seed = r_u32(0x800CBF58);
            value = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            if ((sint32)(value & 4095) >= (sint32)((0xF7E - strength) << 1))
            {
                value = r_u32(context + 12);
                w_u32(context + 12, value | (1u << (i & 31)));
            }
        }
        value = r_u32(context + 12);
        w_u32(context + 16, (value >> (i & 31)) & 0x55555555);
        value = xport_gte_read_data(12);
        w_u32(packet + 44, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 68, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 8, value);
        w_u32(packet + 12, 0x10101);
        w_u32(packet + 48, 0x10101);
        xport_gte_execute(0x158002D);
        a = r_u16(context + 20);
        seed = r_u16(context + 52);
        b = r_u16(context + 22);
        c = r_u16(context + 46);
        w_u16(context + 72, a - seed);
        w_u16(context + 74, b - c);
        depth = xport_gte_read_data(24);
        value = r_u32(context + 20);
        xport_gte_write_data(0, value);
        value = r_u32(context + 24);
        xport_gte_write_data(1, value);
        value = r_u32(context + 36);
        xport_gte_write_data(2, value);
        value = r_u32(context + 40);
        xport_gte_write_data(3, value);
        value = r_u32(source + 24);
        xport_gte_write_data(4, value);
        value = r_u32(source + 28);
        xport_gte_write_data(5, value);
        w_u32(packet + 28, 0x30303);
        w_u32(packet + 64, 0x30303);
        xport_gte_execute(0x280030);
        shift = r_u32(state + 12);
        value = r_s16(state + 0x3F6);
        bucket = (uint32)((sint32)depth >> ((shift + 10) & 31)) + 4 - value;
        if ((sint32)bucket < -75)
            bucket = -75;
        else if ((sint32)bucket >= 77)
            bucket = 76;
        bucket = (sint32)bucket >> 2;
        w_u32(context + 68, bucket);
        value = xport_gte_read_data(12);
        w_u32(packet + 24, value);
        value = xport_gte_read_data(12);
        w_u32(packet + 60, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 16, value);
        value = xport_gte_read_data(13);
        w_u32(packet + 52, value);
        value = xport_gte_read_data(14);
        w_u32(packet + 32, value);
        ordering = r_u32(state + 0x3F8) + bucket * 4;
        start = r_u32(state + 4);
        address = ordering + 2;
        lane = address & 3;
        value = r_u32(address & 0xFFFFFFFC) << (24 - lane * 8);
        if (lane != 3)
            value |= seed & (0xFFFFFF >> (lane * 8));
        start <<= 8;
        address = packet + 38;
        lane = address & 3;
        shift = 24 - lane * 8;
        address &= 0xFFFFFFFC;
        for (j = 0; j <= lane; ++j)
            w_u8(address + j, value >> (shift + j * 8));
        address = ordering + 2;
        lane = address & 3;
        shift = 24 - lane * 8;
        address &= 0xFFFFFFFC;
        for (j = 0; j <= lane; ++j)
            w_u8(address + j, start >> (shift + j * 8));
        w_u32(state + 4, packet + 72);
        value = r_u32(context + 16);
        if (value != 0)
            sub_8008c644(state, context);
        value = r_u32(context + 4);
        w_u32(context + 4, value + 32);
    }
    result = r_u32(context + 80);
    if (result == 0)
    {
        record = r_u32(state + 0x3FC);
        result = r_u32(context + 12);
        w_u32(record + 12, result);
    }
    return result;
}

uint32 sub_8008a61c(uint32 effect, uint32 context)
{
    uint32 i, j, value, other, component, magnitude, duration, counter, product, scale[3], low, high, a, b, c, d, e, x, y, z, u, v, w, p, q, r, packet, end, depth, bucket, ordering;
    FUNCTION_MARKER(0x8008A61C, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(effect + 4 + i * 4);
        other = r_u32(0x1F800008 + i * 4);
        component = (sint32)(value - other) >> 12;
        magnitude = (sint32)component < 0 ? 0 - component : component;
        w_u16(context + 16 + i * 2, component);
        if ((sint32)magnitude > 32767)
            return 1;
    }
    value = r_u32(context + 16);
    xport_gte_write_data(0, value);
    value = r_u32(context + 20);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    duration = r_s16(effect + 2);
    counter = r_s16(effect);
    if ((sint32)duration <= 0)
        duration = 1;
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    xport_gte_write_control(5, a);
    xport_gte_write_control(6, b);
    xport_gte_write_control(7, c);
    value = r_u16(effect + 16);
    w_u16(context + 0xC8, value);
    value = r_u16(effect + 18);
    w_u16(context + 0xCC, counter << 7);
    w_u16(context + 0xCA, value);
    RotMatrixYXZ((SVECTOR *)psx_addr(context + 0xC8, 8), (MATRIX *)psx_addr(context + 0xA8, 32));
    value = (sint32)(counter << 10) / (sint32)duration;
    product = rsin((sint32)value);
    value = r_s16(effect + 22);
    product *= value;
    if ((sint32)product < 0)
        product += 4095;
    value = (sint32)product >> 12;
    w_u32(context + 0xD8, value);
    w_u32(context + 0xD4, value);
    w_u32(context + 0xD0, value);
    scale[0] = r_u32(context + 0xD0);
    scale[1] = r_u32(context + 0xD4);
    scale[2] = r_u32(context + 0xD8);
    for (i = 0; i < 4; ++i)
    {
        value = r_u32(context + 0xA8 + i * 4);
        j = (i * 2) % 3;
        low = (uint32)(sint32)(sint16)value * scale[j];
        high = (uint32)((sint32)value >> 16) * scale[(j + 1) % 3];
        w_u32(context + 0xA8 + i * 4, ((uint32)((sint32)low >> 12) & 0xFFFF) | ((uint32)((sint32)high >> 12) << 16));
    }
    value = r_u32(context + 0xB8);
    low = (uint32)(sint32)(sint16)value * scale[2];
    w_u32(context + 0xB8, (sint32)low >> 12);
    a = r_u32(context + 0x88);
    b = r_u32(context + 0x8C);
    c = r_u32(context + 0x90);
    d = r_u32(context + 0x94);
    e = r_u32(context + 0x98);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    a = r_u16(context + 0xA8);
    b = r_u32(context + 0xAC);
    c = r_u32(context + 0xB4);
    xport_gte_write_data(0, a | (b & 0xFFFF0000));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = r_u16(context + 0xAA);
    b = r_u32(context + 0xB0);
    c = r_s16(context + 0xB6);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    xport_gte_write_data(0, a | (b << 16));
    xport_gte_write_data(1, c);
    xport_gte_execute(0x486012);
    a = r_u16(context + 0xAC);
    b = r_u32(context + 0xB0);
    c = r_u32(context + 0xB8);
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
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0);
    xport_gte_write_data(2, 0x0C000000);
    xport_gte_write_data(3, 0);
    xport_gte_write_data(4, 0x10000000);
    xport_gte_write_data(5, 0);
    packet = r_u32(0x1F800004);
    xport_gte_execute(0x280030);
    value = 36;
    if (((sint32)duration >> 1) < (sint32)counter)
        value = (sint32)((duration - counter) * 72) / (sint32)duration;
    w_u32(context, value);
    value = xport_gte_read_control(31);
    w_u32(context + 8, value);
    value = r_u32(context + 8);
    if ((sint32)value >= 0)
    {
        value = xport_gte_read_data(13);
        w_u32(context + 28, value);
        value = xport_gte_read_data(14);
        w_u32(context + 32, value);
        a = r_u32(context + 28);
        b = r_u32(context + 32);
        w_u32(context + 0x7C, a);
        w_u32(context + 0x80, b);
        depth = xport_gte_read_data(19);
        bucket = (uint32)((sint32)depth >> 3) - 12;
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        value = r_u32(0x1F800000);
        w_u32(context + 12, value + bucket * 4);
        /* The original far-fade branch requires a clamped bucket above 10000 */
    }
    a = r_u32(context + 8);
    b = r_u32(context);
    end = packet + 36;
    w_u32(context + 0x84, a);
    w_u32(context, b | (b << 8) | (b << 16));
    for (i = 0; i < 15; ++i)
    {
        a = r_s16(context + 0x28 + i * 4);
        product = a * 3;
        if ((sint32)product < 0)
            product += 3;
        low = (sint32)product >> 2;
        b = r_s16(context + 0x38 + i * 4);
        product = b * 3;
        if ((sint32)product < 0)
            product += 3;
        high = (sint32)product >> 2;
        xport_gte_write_data(2, (low & 0xFFFF) | (high << 16));
        xport_gte_write_data(4, (a & 0xFFFF) | (b << 16));
        xport_gte_execute(0x280030);
        value = r_u32(context + 8);
        if ((sint32)value < 0)
        {
            value = xport_gte_read_control(31);
            w_u32(context + 8, value);
            value = r_u32(context + 8);
            if ((sint32)value < 0)
                continue;
            value = xport_gte_read_data(13);
            w_u32(context + 28, value);
            value = xport_gte_read_data(14);
            w_u32(context + 32, value);
        }
        else
        {
            w_u32(end - 28, 0x3A000000);
            w_u32(end - 12, 0);
            value = r_u32(context + 28);
            w_u32(end - 24, value);
            a = r_u32(context + 32);
            w_u32(end - 32, 0xE6000001);
            w_u32(end + 4, 0xE6000000);
            w_u32(end - 16, a);
            value = xport_gte_read_control(31);
            w_u32(context + 8, value);
            value = r_u32(context + 8);
            if ((sint32)value < 0)
                continue;
            value = r_u32(context);
            w_u32(end - 20, value);
            value = r_u32(context);
            w_u32(end - 4, value);
            value = xport_gte_read_data(13);
            w_u32(context + 28, value);
            value = xport_gte_read_data(14);
            w_u32(context + 32, value);
            value = r_u32(context + 28);
            w_u32(end - 8, value);
            value = r_u32(context + 32);
            w_u32(end, value);
            ordering = r_u32(context + 12);
            value = r_u32(ordering);
            end += 44;
            w_u32(packet, value | 0x0A000000);
            value = packet & 0xFFFFFF;
            packet += 44;
            ordering = r_u32(context + 12);
            w_u32(ordering, value);
        }
        depth = xport_gte_read_data(19);
        bucket = (uint32)((sint32)depth >> 3) - 12;
        if ((sint32)bucket >= 4095)
            bucket = 4094;
        else if ((sint32)bucket < 0)
            bucket = 0;
        value = r_u32(0x1F800000);
        w_u32(context + 12, value + bucket * 4);
    }
    value = r_u32(context + 8);
    if ((sint32)value >= 0)
    {
        value = r_u32(context + 0x84);
        if ((sint32)value >= 0)
        {
            w_u32(packet + 8, 0x3A000000);
            w_u32(packet + 24, 0);
            value = r_u32(context);
            w_u32(packet + 4, 0xE6000001);
            w_u32(packet + 32, value);
            w_u32(packet + 16, value);
            value = r_u32(context + 0x78);
            w_u32(packet + 40, value);
            value = r_u32(context + 28);
            w_u32(packet + 12, value);
            value = r_u32(context + 32);
            w_u32(packet + 20, value);
            value = r_u32(context + 0x7C);
            w_u32(packet + 28, value);
            value = r_u32(context + 0x80);
            w_u32(packet + 36, value);
            ordering = r_u32(context + 12);
            value = r_u32(ordering);
            w_u32(packet, value | 0x0A000000);
            value = packet & 0xFFFFFF;
            packet += 44;
            ordering = r_u32(context + 12);
            w_u32(ordering, value);
        }
    }
    a = r_u32(context + 0x88);
    b = r_u32(context + 0x8C);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    c = r_u32(context + 0x90);
    d = r_u32(context + 0x94);
    e = r_u32(context + 0x98);
    xport_gte_write_control(2, c);
    xport_gte_write_control(3, d);
    xport_gte_write_control(4, e);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0);
    w_u32(0x1F800004, packet);
    return context + 0x88;
}

void sub_8008b048(uint32 effects)
{
    uint32 mask, i, value, root, a, b, c, record, kind, target;
    FUNCTION_MARKER(0x8008B048, "ACE3.BIN");
    mask = r_u32(effects);
    if (mask == 0)
        return;
    for (i = 0; i < 32; ++i)
    {
        value = r_u16(0x800B7104 + i * 512);
        w_u16(0x1F800040 + i * 2, value);
        if (i < 9)
            w_u16(0x1F800080 + i * 2, value);
    }
    root = r_u32(0x800BCF90);
    value = r_u32(root + 0x5C8);
    w_u32(0x1F800094, (value & 4) != 0 ? 0xE6000001 : 0xE6000000);
    a = xport_gte_read_control(0);
    b = xport_gte_read_control(1);
    w_u32(0x1F8000A4, a);
    w_u32(0x1F8000A8, b);
    a = xport_gte_read_control(2);
    b = xport_gte_read_control(3);
    c = xport_gte_read_control(4);
    w_u32(0x1F8000AC, a);
    w_u32(0x1F8000B0, b);
    w_u32(0x1F8000B4, c);
    record = effects + 4;
    do
    {
        if ((mask & 1) != 0)
        {
            kind = r_s16(record + 20);
            if (kind < 3)
            {
                target = r_u32(0x800B4400 + kind * 4);
                xport_guest_call2(target, record, 0x1F80001C);
            }
        }
        mask >>= 1;
        record += 24;
    } while (mask != 0);
}

uint32 sub_80089f44(uint32 effect, uint32 context)
{
    uint32 i, value, other, component, magnitude, duration, counter, product, depth, quotient, size, bucket, ordering, color, high, packet, cursor, next, last, shift, count, angle, sample, radius, x, y, seed, address, lane, bits, j, result;
    FUNCTION_MARKER(0x80089F44, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(effect + 4 + i * 4);
        other = r_u32(0x1F800008 + i * 4);
        component = (sint32)(value - other) >> 12;
        magnitude = (sint32)component < 0 ? 0 - component : component;
        w_u16(context + 16 + i * 2, component);
        if ((sint32)magnitude > 32767)
            return 1;
    }
    value = r_u32(context + 16);
    xport_gte_write_data(0, value);
    value = r_u32(context + 20);
    xport_gte_write_data(1, value);
    xport_gte_execute(0x180001);
    duration = r_s16(effect + 2);
    counter = r_s16(effect);
    if ((sint32)duration <= 0)
        duration = 1;
    value = 36;
    if (((sint32)duration >> 1) < (sint32)counter)
        value = (sint32)((duration - counter) * 72) / (sint32)duration;
    w_u32(context, value);
    value = xport_gte_read_control(31);
    if ((sint32)value < 0)
        return value;
    depth = xport_gte_read_data(19);
    value = xport_gte_read_data(14);
    w_u32(context + 24, value);
    if (depth == 0)
        depth = 1;
    quotient = (sint32)(counter << 10) / (sint32)duration;
    value = r_u16(0x1F80001A);
    other = r_s16(effect + 22);
    product = other * (uint32)(sint32)(sint16)value;
    if (depth == 0xFFFFFFFF && product == 0x80000000)
        xport_mips_break(6);
    size = (sint32)product / (sint32)depth;
    product = rsin((sint32)quotient) * size;
    if ((sint32)product < 0)
        product += 4095;
    size = (sint32)product >> 12;
    value = r_s16(context + 24);
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = size + 120;
    if ((sint32)(size + 160) < (sint32)magnitude)
        return result;
    value = r_s16(context + 26);
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = (sint32)depth >> 3;
    if ((sint32)(size + 120) < (sint32)magnitude)
        return result;
    w_u16(context + 4, size);
    bucket = result - 12;
    if ((sint32)bucket >= 4095)
        bucket = 4094;
    else if ((sint32)bucket < 0)
        bucket = 0;
    ordering = r_u32(0x1F800000);
    color = r_u32(context);
    w_u32(context + 12, ordering + bucket * 4);
    if ((sint32)depth >= 10001)
    {
        product = color * (0xB3B0 - depth);
        high = (uint32)(((sint64)(sint32)product * (sint64)(sint32)0xB65C6D59) >> 32);
        value = (uint32)((sint32)(high + product) >> 15) - (uint32)((sint32)product >> 31);
        color = value & 255;
    }
    color |= (color << 8) | (color << 16);
    value = r_s16(context + 4);
    w_u32(context, color);
    if ((sint32)value < 2)
    {
        ordering = r_u32(context + 12);
        value = r_u32(ordering);
        packet = r_u32(0x1F800004);
        w_u32(packet, value | 0x02000000);
        ordering = r_u32(context + 12);
        w_u32(ordering, packet & 0xFFFFFF);
        value = r_u32(context);
        w_u32(packet + 4, value | 0x6A000000);
        result = r_u32(context + 24);
        w_u32(packet + 8, result);
        w_u32(0x1F800004, packet + 12);
        return result;
    }
    packet = r_u32(0x1F800004);
    w_u32(packet, ((packet + 8) & 0xFFFFFF) | 0x01000000);
    w_u32(packet + 4, 0xE6000001);
    value = r_s16(context + 4);
    shift = 3;
    if ((sint32)value >= 5)
    {
        shift = 2;
        if ((sint32)value >= 10)
            shift = (sint32)value < 44;
    }
    count = 32 >> shift;
    value = r_s16(context + 4);
    other = r_u16(context + 4);
    cursor = packet + 8;
    if ((sint32)value < 25)
    {
        last = cursor + count * 28 - 28;
        value = r_u16(context + 24);
        w_u16(last + 24, value);
        w_u16(cursor + 16, value);
        value = r_u16(context + 26);
        radius = r_u16(context + 4);
        value += radius;
        w_u16(last + 26, value);
        w_u16(cursor + 18, value);
        for (i = 1; i < count; ++i)
        {
            next = cursor + 28;
            w_u32(cursor, (next & 0xFFFFFF) | 0x06000000);
            w_u32(cursor + 4, 0x32000000);
            value = r_u32(context);
            w_u32(cursor + 20, value);
            w_u32(cursor + 12, value);
            value = r_u32(context + 24);
            w_u32(cursor + 8, value);
            angle = i << shift;
            sample = r_s16(context + 36 + angle * 2);
            radius = r_s16(context + 4);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = r_u16(context + 24) + (uint32)((sint32)product >> 12);
            w_u16(next + 16, x);
            w_u16(cursor + 24, x);
            sample = r_s16(context + 36 + (angle + 8) * 2);
            radius = r_s16(context + 4);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = r_u16(context + 26) + (uint32)((sint32)product >> 12);
            w_u16(next + 18, y);
            w_u16(cursor + 26, y);
            cursor = next;
        }
        next = cursor + 28;
        w_u32(cursor, (next & 0xFFFFFF) | 0x06000000);
        w_u32(cursor + 4, 0x32000000);
        value = r_u32(context);
        w_u32(cursor + 20, value);
        w_u32(cursor + 12, value);
        value = r_u32(context + 24);
        w_u32(cursor + 8, value);
        cursor = next;
    }
    else
    {
        w_u16(context + 6, other - 24);
        last = cursor + count * 36 - 36;
        value = r_u16(context + 24);
        w_u16(last + 32, value);
        w_u16(last + 24, value);
        w_u16(cursor + 16, value);
        w_u16(cursor + 8, value);
        value = r_u16(context + 26);
        radius = r_u16(context + 6);
        value += radius;
        w_u16(last + 26, value);
        w_u16(cursor + 10, value);
        value = r_u16(context + 26);
        radius = r_u16(context + 4);
        value += radius;
        w_u16(last + 34, value);
        w_u16(cursor + 18, value);
        for (i = 1; i < count; ++i)
        {
            next = cursor + 36;
            w_u32(cursor, (next & 0xFFFFFF) | 0x08000000);
            w_u32(cursor + 4, 0x3A000000);
            w_u32(cursor + 20, 0);
            value = r_u32(context);
            angle = i << shift;
            w_u32(cursor + 28, value);
            w_u32(cursor + 12, value);
            sample = r_s16(context + 36 + angle * 2);
            radius = r_s16(context + 6);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = r_u16(context + 24) + (uint32)((sint32)product >> 12);
            w_u16(next + 8, x);
            w_u16(cursor + 24, x);
            sample = r_s16(context + 36 + (angle + 8) * 2);
            radius = r_s16(context + 6);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = r_u16(context + 26) + (uint32)((sint32)product >> 12);
            w_u16(next + 10, y);
            w_u16(cursor + 26, y);
            sample = r_s16(context + 36 + angle * 2);
            radius = r_s16(context + 4);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            x = r_u16(context + 24) + (uint32)((sint32)product >> 12);
            w_u16(next + 16, x);
            w_u16(cursor + 32, x);
            sample = r_s16(context + 36 + (angle + 8) * 2);
            radius = r_s16(context + 4);
            product = sample * radius;
            if ((sint32)product < 0)
                product += 4095;
            y = r_u16(context + 26) + (uint32)((sint32)product >> 12);
            w_u16(next + 18, y);
            w_u16(cursor + 34, y);
            cursor = next;
        }
        next = cursor + 36;
        w_u32(cursor, (next & 0xFFFFFF) | 0x08000000);
        w_u32(cursor + 4, 0x3A000000);
        w_u32(cursor + 20, 0);
        value = r_u32(context);
        w_u32(cursor + 28, value);
        w_u32(cursor + 12, value);
        cursor = next;
    }
    w_u32(cursor, 0x01000000);
    seed = r_u32(context + 120);
    w_u32(cursor + 4, seed);
    ordering = r_u32(context + 12);
    value = r_u32(0x1F800004);
    address = ordering + 2;
    lane = address & 3;
    bits = r_u32(address & 0xFFFFFFFC) << (24 - lane * 8);
    if (lane != 3)
        bits |= seed & (0xFFFFFF >> (lane * 8));
    address = cursor + 2;
    lane = address & 3;
    shift = 24 - lane * 8;
    address &= 0xFFFFFFFC;
    for (j = 0; j <= lane; ++j)
        w_u8(address + j, bits >> (shift + j * 8));
    value <<= 8;
    address = ordering + 2;
    lane = address & 3;
    shift = 24 - lane * 8;
    address &= 0xFFFFFFFC;
    for (j = 0; j <= lane; ++j)
        w_u8(address + j, value >> (shift + j * 8));
    result = cursor + 8;
    w_u32(0x1F800004, result);
    return result;
}

void sub_80089d70(uint32 state)
{
    uint32 mask, record, callback, index, value, counter, limit, target;
    FUNCTION_MARKER(0x80089D70, "ACE3.BIN");
    mask = r_u32(state);
    record = state + 4;
    if (mask == 0)
        return;
    index = 0;
    callback = state + 20;
    do
    {
        if ((mask & 1) != 0)
        {
            value = r_u16(record) + 1;
            w_u16(record, value);
            counter = (uint32)(sint32)(sint16)value;
            limit = r_s16(callback - 14);
            if ((sint32)counter >= (sint32)limit)
            {
                value = r_u32(state);
                w_u32(state, value & ~(1u << (index & 31)));
            }
            else
            {
                target = r_u32(callback);
                if (target != 0)
                    xport_guest_call2(target, counter, record);
            }
        }
        mask >>= 1;
        ++index;
        callback += 20;
        record += 20;
    } while (mask != 0);
}

uint32 sub_80061614(uint32 position, uint32 angles, uint32 geometry, uint32 state, const uint32 *native_position, const uint16 *native_angles, uint8 *native_state)
{
    uint32 count, index, input, output, value;
    FUNCTION_MARKER(0x80061614, "ACE3.BIN");
    sub_8005f90c(geometry, position, angles, state, native_position, native_angles, native_state);
    count = native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24);
    index = 0;
    if ((sint32)count > 0)
    {
        input = 0;
        output = 0;
        do
        {
            value = native_state != 0 ? xport_load_le16(native_state + input + 28) : r_u16(state + input + 28);
            if (native_state != 0)
                xport_store_le16(native_state + output + 224, value);
            else
                w_u16(state + output + 224, value);
            value = native_state != 0 ? xport_load_le16(native_state + 4) : r_u16(state + 4);
            ++index;
            if (native_state != 0)
                xport_store_le16(native_state + output + 226, 0 - value);
            else
                w_u16(state + output + 226, 0 - value);
            value = native_state != 0 ? xport_load_le16(native_state + input + 30) : r_u16(state + input + 30);
            input += 4;
            if (native_state != 0)
                xport_store_le16(native_state + output + 228, value);
            else
                w_u16(state + output + 228, value);
            count = native_state != 0 ? xport_load_le32(native_state + 24) : r_u32(state + 24);
            output += 8;
        } while ((sint32)index < (sint32)count);
    }
    sub_80061534(state, native_state);
    return sub_80060cb8(state, native_state);
}

uint32 sub_80089bb4(uint32 position, uint32 kind, const uint32 *native_position)
{
    uint32 table[21], i, target, x, y, z;
    FUNCTION_MARKER(0x80089BB4, "ACE3.BIN");
    for (i = 0; i < 20; i += 4)
    {
        table[i] = r_u32(0x80024874 + i * 4);
        table[i + 1] = r_u32(0x80024878 + i * 4);
        table[i + 2] = r_u32(0x8002487C + i * 4);
        table[i + 3] = r_u32(0x80024880 + i * 4);
    }
    table[20] = r_u32(0x800248C4);
    if (kind >= 21)
        return kind << 2;
    target = table[kind];
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    return xport_guest_call3(target, x, y, z);
}

uint32 sub_80080eb4(uint32 position, uint32 kind, const uint32 *native_position)
{
    uint32 target, x, y, z;
    FUNCTION_MARKER(0x80080EB4, "ACE3.BIN");
    target = (kind & 1) != 0 ? 0x80080C08 : 0x80080A94;
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    return xport_guest_call3(target, x, y, z);
}

uint32 sub_80082594(uint32 position, uint32 mode, const uint32 *native_position)
{
    uint32 seed, random, value, x, y, z, root, effect, result;
    FUNCTION_MARKER(0x80082594, "ACE3.BIN");
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[0] : r_u32(position);
    x = value + (((random & 63) - 32) << 12);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[1] : r_u32(position + 4);
    y = value + (((random & 63) - 32) << 12);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[2] : r_u32(position + 8);
    z = value + (((random & 63) - 32) << 12);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x600, 3, 7);
    if ((mode & 1) != 0)
    {
        if (effect != 0)
        {
            w_u32(effect, 0x820000);
            w_u16(effect + 16, random & 63);
            w_u16(effect + 18, -112);
            w_u16(effect + 20, ((uint32)((sint32)random >> 6)) & 63);
            w_u16(effect + 22, 128);
            w_u16(effect + 24, 0x210);
            value = (((uint32)((sint32)random >> 3)) & 63) + 78;
            w_u32(effect + 4, x);
            w_u32(effect + 8, y);
            w_u32(effect + 12, z);
            w_u16(effect + 26, value);
        }
    }
    else if (effect != 0)
    {
        w_u32(effect, 0xAFFFC);
        w_u16(effect + 24, 256);
        value = (random & 63) + 48;
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 22, 0);
        w_u16(effect + 26, value);
    }
    result = random & 1;
    if ((mode & 1) != 0 && result != 0)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x1098, 10, 7);
        result = 10;
        if (effect != 0)
        {
            w_u16(effect, 10);
            w_u16(effect + 2, (random & 31) + 40);
            w_u16(effect + 16, random & 63);
            w_u16(effect + 18, -112);
            w_u16(effect + 20, ((uint32)((sint32)random >> 6)) & 63);
            w_u16(effect + 22, 128);
            w_u16(effect + 24, 0x1604);
            result = (((uint32)((sint32)random >> 3)) & 63) + 100;
            w_u32(effect + 4, x);
            w_u32(effect + 8, y);
            w_u32(effect + 12, z);
            w_u16(effect + 26, result);
        }
    }
    return result;
}

uint32 sub_80080140(uint32 position, uint32 kind, uint32 mode, const uint32 *native_position)
{
    uint32 x, y, z, root, effect, seed, random, result, flags, material, i, packed, a, b, c, lifetime;
    FUNCTION_MARKER(0x80080140, "ACE3.BIN");
    if (mode >= 6)
        return 0;
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    if (mode >= 3)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x1098, 10, 7);
        if (effect == 0)
            return 144;
        w_u16(effect + 22, 144);
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 20, 0);
        if (mode == 3)
        {
            w_u32(effect, 0x100000);
            w_u32(effect + 16, 0xFF800000);
            w_u16(effect + 24, 0x601);
        }
        else
        {
            w_u32(effect, 0x140000);
            w_u32(effect + 16, 0xFF600000);
            w_u16(effect + 24, mode == 4 ? 0x800 : 0x803);
        }
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        result = (random & 63) + (mode == 3 ? 208 : 268);
        w_u16(effect + 26, result);
        return result;
    }
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x600, 3, 7);
    w_u32(effect + 4, x);
    w_u32(effect + 8, y);
    w_u32(effect + 12, z);
    if (mode == 2 && kind != 1)
    {
        w_u32(effect, 0xBFFFF);
        w_u16(effect + 22, 0);
        w_u16(effect + 24, 256);
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        w_u16(effect + 26, (random & 127) + 120);
        for (i = 0; i < 4; ++i)
        {
            root = r_u32(0x800BCF90);
            effect = sub_8007fdb4(root + 0x1098, 10, 7);
            result = i + 16;
            if (effect == 0)
                return result;
            w_u16(effect, 0);
            w_u16(effect + 2, result);
            seed = r_u32(0x800CBF58);
            random = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            packed = (uint32)((sint32)random >> 8) | (random << 7);
            lifetime = (i & 3) + 14;
            a = x + (((random & 255) - 128) << 8);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8) - 0x28000;
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
            w_u16(effect + 16, ((packed & 255) - 128) << 2);
            w_u16(effect + 18, (((uint32)((sint32)packed >> 1)) & 0x3FC) - 640);
            w_u16(effect + 20, (((((uint32)((sint32)packed >> 6)) & 255) - 128) << 2));
            w_u16(effect + 22, 394);
            result = i + 1 < 4;
            w_u32(effect + 4, a);
            w_u32(effect + 8, b);
            w_u32(effect + 12, c);
            w_u16(effect + 24, 0);
            w_u16(effect + 26, lifetime);
        }
        return result;
    }
    if (mode == 2)
    {
        flags = 0xEFFFF;
        material = 0x640120;
    }
    else
    {
        switch (kind)
        {
            case 1:
                flags = 0xCFFFF;
                material = 0x500120;
                break;
            case 2:
                flags = 0xAFFFF;
                material = 0x500121;
                break;
            case 3:
                flags = 0xAFFFF;
                material = 0x500122;
                break;
            case 4:
                w_u32(effect, 0xCFFFF);
                seed = r_u32(0x800CBF58);
                random = psyq_rand_next(&seed);
                w_u32(0x800CBF58, seed);
                w_u16(effect + 16, (random & 63) - 32);
                a = (((uint32)((sint32)random >> 3)) & 63) - 32;
                b = (((uint32)((sint32)random >> 6)) & 63) - 32;
                w_u16(effect + 20, b);
                w_u16(effect + 18, a);
                w_u16(effect + 22, 136);
                w_u32(effect + 24, 0x640123);
                return 136;
            case 5:
                flags = 0xAFFFF;
                material = 0x500124;
                break;
            default:
                w_u32(effect, 0xAFFFF);
                w_u16(effect + 22, 0);
                w_u32(effect + 24, 0x500000);
                return 0x500000;
        }
    }
    w_u32(effect, flags);
    w_u16(effect + 22, 0);
    w_u32(effect + 24, material);
    return flags;
}

uint32 sub_80083764(uint32 position, const uint32 *native_position)
{
    uint32 x, y, z, root, effect, i, seed, random, lifetime;
    FUNCTION_MARKER(0x80083764, "ACE3.BIN");
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    y -= 0xF0000;
    for (i = 0; i < 2; ++i)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x1098, 10, 7);
        if (effect == 0)
            return 0;
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        lifetime = 220 + i * 80 + (random & 127);
        w_u16(effect + 2, i + (random & 3) + 30);
        w_u16(effect + 22, 397);
        w_u16(effect + 24, 0x1E00);
        w_u16(effect, 0);
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 16, 0);
        w_u16(effect + 18, 0);
        w_u16(effect + 20, 0);
        w_u16(effect + 26, lifetime);
    }
    return 0;
}

uint32 sub_80083554(uint32 position, uint32 extra, const uint32 *native_position)
{
    uint32 root, x, y, z, effect, seed, random, result, i, height, speed, duration, packed, a, b, c, vertical, lifetime, counter;
    FUNCTION_MARKER(0x80083554, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    x = native_position != 0 ? native_position[0] : r_u32(position);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    y += 0xFFBE6000;
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    result = effect;
    if (effect != 0)
    {
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        w_u16(effect, 10);
        w_u16(effect + 2, (random & 15) + 40);
        w_u16(effect + 16, random & 63);
        w_u16(effect + 18, -352);
        w_u16(effect + 20, ((uint32)((sint32)random >> 6)) & 63);
        w_u16(effect + 22, 133);
        result = (random & 127) + 200;
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 24, 0x1804);
        w_u16(effect + 26, result);
    }
    if (extra == 0)
        return result;
    height = y;
    speed = -320;
    duration = 200;
    for (i = 0; i < 5; ++i)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        if (effect == 0)
            return 0;
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        b = height + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        height -= 0x50000;
        vertical = speed - 128;
        speed -= 96 + (packed & 15);
        lifetime = duration + (packed & 63);
        duration += 70 + (packed & 7);
        counter = i * 4 + (random & 3) + 16;
        a = x + (((random & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        w_u16(effect, 0xFFFF);
        w_u16(effect + 16, (packed & 255) - 128);
        vertical += ((uint32)((sint32)packed >> 3)) & 255;
        w_u16(effect + 20, (((uint32)((sint32)packed >> 6)) & 255) - 128);
        w_u16(effect + 22, 133);
        w_u16(effect + 24, 256);
        w_u16(effect + 2, counter);
        w_u32(effect + 4, a);
        w_u32(effect + 8, b);
        w_u32(effect + 12, c);
        w_u16(effect + 18, vertical);
        w_u16(effect + 26, lifetime);
    }
    return 0;
}

uint32 sub_80083844(uint32 position, uint32 extra, const uint32 *native_position)
{
    uint32 root, x, y, z, effect, seed, random, packed, a, b, c, counter, result;
    FUNCTION_MARKER(0x80083844, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    result = effect;
    if (effect != 0)
    {
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        counter = (random & 15) + 40;
        a = x + (((random & 255) - 128) << 10);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
        w_u16(effect, 10);
        w_u16(effect + 16, (packed + 20) & 127);
        w_u16(effect + 20, ((uint32)((sint32)packed >> 6) + 50) & 127);
        w_u16(effect + 18, -400);
        w_u16(effect + 22, 135);
        w_u16(effect + 24, 0x1a04);
        result = (packed & 127) + 500;
        w_u16(effect + 2, counter);
        w_u32(effect + 4, a);
        w_u32(effect + 8, b);
        w_u32(effect + 12, c);
        w_u16(effect + 26, result);
    }
    if (extra != 0)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        result = effect;
        if (effect != 0)
        {
            seed = r_u32(0x800CBF58);
            random = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            w_u16(effect, 10);
            w_u16(effect + 2, (random & 15) + 100);
            a = x + (((random & 255) - 128) << 11);
            w_u32(effect + 4, a);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 11);
            w_u32(effect + 8, b);
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 11);
            w_u32(effect + 12, c);
            seed = r_u32(0x800CBF58);
            random = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            w_u16(effect + 16, (random & 127) + 20);
            w_u16(effect + 20, (((uint32)((sint32)random >> 7)) & 127) + 50);
            w_u16(effect + 18, 0 - 400 - (random & 15));
            w_u16(effect + 22, 128);
            w_u16(effect + 24, 0x210);
            result = (((uint32)((sint32)random >> 4)) & 127) + 500;
            w_u16(effect + 26, result);
        }
    }
    return result;
}

uint32 sub_80083a40(uint32 position, uint32 extra, const uint32 *native_position)
{
    uint32 root, x, y, z, effect, seed, random, packed, a, b, c, counter, result;
    FUNCTION_MARKER(0x80083A40, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    result = effect;
    if (effect != 0)
    {
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        counter = (random & 7) + 30;
        a = x + (((random & 255) - 128) << 10);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
        w_u16(effect, 10);
        w_u16(effect + 16, (packed + 20) & 127);
        w_u16(effect + 20, ((uint32)((sint32)packed >> 6) + 50) & 127);
        w_u16(effect + 18, -400);
        w_u16(effect + 22, 135);
        w_u16(effect + 24, 0x1904);
        result = (packed & 127) + 400;
        w_u16(effect + 2, counter);
        w_u32(effect + 4, a);
        w_u32(effect + 8, b);
        w_u32(effect + 12, c);
        w_u16(effect + 26, result);
    }
    if (extra != 0)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        result = effect;
        if (effect != 0)
        {
            seed = r_u32(0x800CBF58);
            random = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            w_u16(effect, 8);
            w_u16(effect + 2, (random & 7) + 68);
            a = x + (((random & 255) - 128) << 10);
            w_u32(effect + 4, a);
            b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 10);
            w_u32(effect + 8, b);
            c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 10);
            w_u32(effect + 12, c);
            seed = r_u32(0x800CBF58);
            random = psyq_rand_next(&seed);
            w_u32(0x800CBF58, seed);
            w_u16(effect + 16, (random & 127) + 20);
            w_u16(effect + 20, (((uint32)((sint32)random >> 7)) & 127) + 50);
            w_u16(effect + 18, 0 - 400 - (random & 31));
            w_u16(effect + 22, 128);
            w_u16(effect + 24, 0x210);
            result = (((uint32)((sint32)random >> 4)) & 127) + 400;
            w_u16(effect + 26, result);
        }
    }
    return result;
}

uint32 sub_80083c3c(uint32 position, uint32 value, uint32 mode, const uint32 *native_position)
{
    uint32 table[4];
    FUNCTION_MARKER(0x80083C3C, "ACE3.BIN");
    table[0] = r_u32(0x80024804);
    table[1] = r_u32(0x80024808);
    table[2] = r_u32(0x8002480C);
    table[3] = r_u32(0x80024810);
    if (mode >= 4)
        return 0;
    return xport_guest_call_position2(table[mode], position, value, native_position);
}

uint32 sub_8007f9e8(uint32 kind, uint32 position)
{
    uint32 vector[3], a, b, c, value, mode;
    FUNCTION_MARKER(0x8007F9E8, "ACE3.BIN");
    a = r_u32(position);
    b = r_u32(position + 4);
    c = r_u32(position + 8);
    vector[0] = a & 0xFFFFF000;
    vector[1] = b & 0xFFFFF000;
    vector[2] = c & 0xFFFFF000;
    switch (kind)
    {
        case 0:
            value = r_u32(position + 8);
            return sub_8007fe28(0, (value >> 4) & 255, vector);
        case 1:
            value = r_u32(position + 8);
            return sub_80080140(0, value & 15, (value >> 4) & 255, vector);
        case 2:
            value = r_u32(position + 4);
            return sub_80082594(0, (value >> 8) & 15, vector);
        case 3:
            value = r_u32(position + 8);
            return sub_80082d80(0, (value >> 4) & 255, vector);
        case 4:
            value = r_u32(position + 8);
            return sub_80089bb4(0, (value >> 4) & 255, vector);
        case 5:
            value = r_u32(position + 8);
            return sub_80080eb4(0, (value >> 4) & 255, vector);
        case 6:
            value = r_u32(position + 8);
            mode = r_u32(position + 4);
            return sub_80083c3c(0, (value >> 4) & 255, (mode >> 8) & 15, vector);
        default:
            return 0;
    }
}

uint32 sub_8007e998(uint32 packet)
{
    NATIVE_POSE pose;
    uint32 vector[3], packed, kind, a, b, c, flags, mode;
    FUNCTION_MARKER(0x8007E998, "ACE3.BIN");
    packed = r_u32(packet);
    kind = (packed >> 7) & 3;
    switch (kind)
    {
        case 0:
            a = r_s16(packet + 2);
            c = r_s16(packet + 6);
            vector[0] = a << 6;
            b = r_s16(packet + 4);
            vector[1] = b << 6;
            vector[2] = c << 6;
            a = r_u32(packet + 8);
            b = r_u32(packet + 12);
            c = r_u32(packet + 16);
            pose.position[0] = a & 0xFFFFF000;
            pose.position[1] = b & 0xFFFFF000;
            pose.position[2] = c & 0xFFFFF000;
            pose.angles[2] = (sint32)(c << 20) >> 18;
            pose.angles[0] = (sint32)(a << 20) >> 18;
            pose.angles[1] = (sint32)(b << 20) >> 18;
            flags = r_u32(packet);
            mode = r_u32(packet + 20);
            return sub_8006c760((flags >> 9) & 127, 0, 0, mode, &pose, vector);
        case 1:
            return sub_8006c99c((packed >> 9) & 127);
        case 2:
            a = r_u32(packet + 4);
            b = r_u32(packet + 8);
            c = r_u32(packet + 12);
            pose.position[0] = a & 0xFFFFF000;
            pose.position[1] = b & 0xFFFFF000;
            pose.position[2] = c & 0xFFFFF000;
            pose.angles[0] = (sint32)(a << 20) >> 18;
            pose.angles[1] = (sint32)(b << 20) >> 18;
            pose.angles[2] = (sint32)(c << 20) >> 18;
            flags = r_u32(packet);
            mode = r_u16(packet + 2);
            return sub_8006c6b8((flags >> 9) & 127, mode, 0, &pose);
        default:
            return sub_8006c740((packed >> 9) & 127);
    }
}

uint32 sub_800827e0(uint32 position, uint32 vector, uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 table[22], i, root, effect, x, y, z, a, b, c, word, material, seed, random, result;
    FUNCTION_MARKER(0x800827E0, "ACE3.BIN");
    for (i = 0; i < 20; i += 4)
    {
        table[i] = r_u32(0x800247AC + i * 4);
        table[i + 1] = r_u32(0x800247B0 + i * 4);
        table[i + 2] = r_u32(0x800247B4 + i * 4);
        table[i + 3] = r_u32(0x800247B8 + i * 4);
    }
    table[20] = r_u32(0x800247FC);
    table[21] = r_u32(0x80024800);
    if (kind >= 11)
        return 0;
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    if (effect == 0)
        return 0;
    x = native_vector != 0 ? native_vector[0] : r_u32(vector);
    a = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    z = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    w_u32(effect + 4, a - x - (uint32)((sint32)x >> 2));
    b = native_position != 0 ? native_position[1] : r_u32(position + 4);
    w_u32(effect + 8, b - y - (uint32)((sint32)y >> 2));
    c = native_position != 0 ? native_position[2] : r_u32(position + 8);
    w_u16(effect + 16, (sint32)x >> 8);
    w_u16(effect + 18, (sint32)y >> 8);
    w_u16(effect + 20, (sint32)z >> 8);
    w_u32(effect + 12, c - z - (uint32)((sint32)z >> 2));
    word = table[kind * 2];
    material = table[kind * 2 + 1];
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    w_u32(effect, (uint32)((sint32)word >> 24) << 16);
    w_u16(effect + 22, material >> 16);
    result = word + (random & ((word >> 16) & 255));
    w_u16(effect + 24, material);
    w_u16(effect + 26, result);
    return result;
}

uint32 sub_80082c1c(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 root, effect, x, y, z, value, seed, random, result;
    FUNCTION_MARKER(0x80082C1C, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x600, 3, 7);
    if (effect == 0)
        return 0;
    x = native_vector != 0 ? native_vector[0] : r_u32(vector);
    y = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    value = native_position != 0 ? native_position[0] : r_u32(position);
    z = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    w_u32(effect + 4, value - x);
    value = native_position != 0 ? native_position[1] : r_u32(position + 4);
    w_u32(effect + 8, value - y);
    value = native_position != 0 ? native_position[2] : r_u32(position + 8);
    w_u16(effect + 22, 135);
    w_u32(effect + 12, value - z);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    w_u16(effect, (random & 1) + 4);
    w_u16(effect + 2, (random & 3) + 40);
    x = (uint32)((sint32)(0 - x) >> 6) - 64 + (random & 127);
    y = (uint32)((sint32)(0 - y) >> 6) - 64 + (((uint32)((sint32)random >> 3)) & 127);
    z = (uint32)((sint32)(0 - z) >> 6) - 64 + (((uint32)((sint32)random >> 6)) & 127);
    result = (random & 63) + 240;
    w_u16(effect + 16, x);
    w_u16(effect + 18, y);
    w_u16(effect + 20, z);
    w_u16(effect + 24, 0x111);
    w_u16(effect + 26, result);
    return result;
}

uint32 sub_80081d4c(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 scene, flags, x, y, z, root, effect, vx, vy, vz, value, i, seed, random, packed, a, b, c, result;
    FUNCTION_MARKER(0x80081D4C, "ACE3.BIN");
    scene = r_u32(0x800BCE54);
    x = native_position != 0 ? native_position[0] : r_u32(position);
    y = native_position != 0 ? native_position[1] : r_u32(position + 4);
    flags = r_u32(scene);
    z = native_position != 0 ? native_position[2] : r_u32(position + 8);
    if ((flags & 256) != 0)
        return sub_80089504(x, y, z);
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x600, 3, 7);
    if (effect != 0)
    {
        w_u32(effect, 0xFFFFF);
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 22, 0);
        w_u32(effect + 24, 0x1900100);
    }
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x33C0, 1, 6);
    if (effect != 0)
    {
        w_u32(effect, 0xC0000);
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u32(effect + 20, 0x2BC0000);
    }
    value = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    vy = (sint32)(0 - value) >> 12;
    value = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    vz = (sint32)(0 - value) >> 12;
    value = native_vector != 0 ? native_vector[0] : r_u32(vector);
    vx = (sint32)(0 - value) >> 12;
    for (i = 0; i < 20; ++i)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x1098, 10, 7);
        result = i + 20;
        if (effect == 0)
            return result;
        w_u16(effect, 0);
        w_u16(effect + 2, result);
        seed = r_u32(0x800CBF58);
        random = psyq_rand_next(&seed);
        w_u32(0x800CBF58, seed);
        packed = (uint32)((sint32)random >> 8) | (random << 7);
        a = x + (((random & 255) - 128) << 8);
        b = y + (((((uint32)((sint32)random >> 3)) & 255) - 128) << 8);
        c = z + (((((uint32)((sint32)random >> 6)) & 255) - 128) << 8);
        w_u16(effect + 16, vx + (((packed & 255) - 128) << 1));
        w_u16(effect + 18, vy + (((uint32)((sint32)packed >> 2)) & 0x1FE) - 288);
        w_u16(effect + 20, vz + (((((uint32)((sint32)packed >> 6)) & 255) - 128) << 1));
        w_u32(effect + 4, a);
        w_u32(effect + 8, b);
        w_u32(effect + 12, c);
        if (i < 6)
        {
            w_u16(effect + 22, 138);
            w_u16(effect + 24, 0x1804);
            w_u16(effect + 26, (vx & 63) + 600);
        }
        else
        {
            w_u16(effect + 22, 392);
            w_u16(effect + 24, 0);
            w_u16(effect + 26, (i >> 1) + 18);
        }
    }
    return 0;
}

uint32 sub_80081fdc(uint32 position, uint32 vector, uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 vx, vy, vz, x, y, z, value, seed, random, noise, root, effect, result;
    FUNCTION_MARKER(0x80081FDC, "ACE3.BIN");
    vx = native_vector != 0 ? native_vector[0] : r_u32(vector);
    vy = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    vz = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[0] : r_u32(position);
    x = value + vx + ((random & 63) << 11);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[1] : r_u32(position + 4);
    y = value + vy + ((random & 63) << 11);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[2] : r_u32(position + 8);
    z = value + vz + ((random & 63) << 11);
    seed = r_u32(0x800CBF58);
    noise = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    noise |= random << 15;
    if ((mode & 7) == 0)
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        if (effect != 0)
        {
            w_u32(effect, 0xE0000);
            w_u16(effect + 24, 256);
            w_u32(effect + 4, x);
            w_u32(effect + 8, y);
            w_u32(effect + 12, z);
            w_u16(effect + 22, 0);
            w_u16(effect + 26, (noise & 63) + 58);
        }
    }
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x600, 3, 7);
    if (effect != 0)
    {
        w_u32(effect, 0x100000);
        w_u16(effect + 24, 0x210);
        w_u32(effect + 4, x);
        w_u32(effect + 8, y);
        w_u32(effect + 12, z);
        w_u16(effect + 22, 0);
        w_u16(effect + 26, (noise & 63) + 68);
    }
    result = noise & 3;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    if (effect == 0)
        return 0x180000;
    w_u32(effect, 0x180000);
    w_u16(effect + 16, (uint32)((sint32)vx >> 9) - 64 + (noise & 127));
    w_u16(effect + 18, (uint32)((sint32)vy >> 9) + (((uint32)((sint32)noise >> 3)) & 127) - 96);
    value = (uint32)((sint32)vz >> 9) - 64 + (((uint32)((sint32)noise >> 6)) & 127);
    w_u32(effect + 4, x);
    w_u32(effect + 8, y);
    w_u32(effect + 12, z);
    w_u16(effect + 20, value);
    if ((noise & 1) != 0)
    {
        w_u16(effect + 22, 392);
        w_u32(effect + 24, 0xE0000);
        return 0xE0000;
    }
    w_u16(effect + 22, 139);
    w_u16(effect + 24, 0x1704);
    result = (noise & 31) + 60;
    w_u16(effect + 26, result);
    return result;
}

uint32 sub_800822a4(uint32 position, uint32 vector, uint32 mode, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 vx, vy, vz, x, y, z, value, seed, random, noise, root, effect, result;
    FUNCTION_MARKER(0x800822A4, "ACE3.BIN");
    vx = native_vector != 0 ? native_vector[0] : r_u32(vector);
    vy = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    vz = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    value = native_position != 0 ? native_position[0] : r_u32(position);
    x = value + vx + (((random & 63) - 32) << 12);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    vx = (sint32)vx >> 9;
    value = native_position != 0 ? native_position[1] : r_u32(position + 4);
    y = value + vy + (((random & 63) - 32) << 12);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    vy = (sint32)vy >> 9;
    value = native_position != 0 ? native_position[2] : r_u32(position + 8);
    z = value + vz + (((random & 63) - 32) << 12);
    vz = (sint32)vz >> 9;
    seed = r_u32(0x800CBF58);
    noise = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    if (mode == 1)
        sub_80082d80(position, 512, native_position);
    else
    {
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        if (effect != 0)
        {
            w_u32(effect, 0x100000);
            w_u16(effect + 24, 256);
            w_u32(effect + 4, x);
            w_u32(effect + 8, y);
            w_u32(effect + 12, z);
            w_u16(effect + 22, 0);
            w_u16(effect + 26, (noise & 15) + 90);
        }
        root = r_u32(0x800BCF90);
        effect = sub_8007fdb4(root + 0x600, 3, 7);
        if (effect != 0)
        {
            w_u32(effect, 0x640000);
            w_u16(effect + 16, 0 - vx - 64 + (noise & 127));
            w_u16(effect + 18, (((uint32)((sint32)noise >> 3)) & 127) - 176);
            value = 0 - vz - 64 + (((uint32)((sint32)noise >> 6)) & 127);
            w_u16(effect + 22, 128);
            w_u16(effect + 24, 0x210);
            w_u32(effect + 4, x);
            w_u32(effect + 8, y);
            w_u32(effect + 12, z);
            w_u16(effect + 20, value);
            w_u16(effect + 26, (noise & 63) + 68);
        }
    }
    result = noise & 1;
    if (result == 0)
        return result;
    root = r_u32(0x800BCF90);
    effect = sub_8007fdb4(root + 0x1098, 10, 7);
    if (effect == 0)
        return 0x180000;
    w_u32(effect, 0x180000);
    w_u32(effect + 4, x);
    w_u32(effect + 8, y);
    w_u32(effect + 12, z);
    seed = r_u32(0x800CBF58);
    random = psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    w_u16(effect + 16, 0 - vx - 128 + (random & 255));
    w_u16(effect + 18, 0 - vy - 128 + (((uint32)((sint32)random >> 3)) & 255));
    value = 0 - vz - 128 + (((uint32)((sint32)random >> 6)) & 255);
    w_u16(effect + 22, 392);
    result = (random & 7) + 16;
    w_u16(effect + 20, value);
    w_u16(effect + 24, 0);
    w_u16(effect + 26, result);
    return result;
}

uint32 sub_800833c8(uint32 position, uint32 vector, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 output[3], root, state, vx, vy, vz, value, x, y, z, dx, dz, result;
    FUNCTION_MARKER(0x800833C8, "ACE3.BIN");
    root = r_u32(0x800BCF90);
    state = root + 0x3B28;
    w_u32(state, 1);
    w_u16(state + 6, 0);
    vx = native_vector != 0 ? native_vector[0] : r_u32(vector);
    vy = native_vector != 0 ? native_vector[1] : r_u32(vector + 4);
    vz = native_vector != 0 ? native_vector[2] : r_u32(vector + 8);
    value = native_position != 0 ? native_position[0] : r_u32(position);
    x = value + (vx << 3);
    value = native_position != 0 ? native_position[1] : r_u32(position + 4);
    dx = (sint32)x >> 8;
    y = value + (vy << 3);
    value = native_position != 0 ? native_position[2] : r_u32(position + 8);
    w_u32(state + 8, x);
    w_u32(state + 12, y);
    z = value + (vz << 3);
    w_u32(state + 20, x - dx);
    dz = (sint32)z >> 8;
    w_u32(state + 24, y + 0xFD8F0000);
    w_u32(state + 28, z - dz);
    w_u32(state + 32, x + dx);
    w_u32(state + 36, y + 0x02710000);
    w_u32(state + 16, z);
    w_u32(state + 40, z + dz);
    result = sub_8009a800(root + 0x3B3C, root + 0x3B48, 0, 0x1F800000, 0, 0, output);
    if (result != 0x7FFFFFFF)
    {
        w_u32(state + 32, output[0]);
        w_u32(state + 36, output[1]);
        w_u32(state + 40, output[2]);
    }
    w_u32(state + 20, x - (dx << 3));
    w_u32(state + 24, y + 0xEC780000);
    result = z - (dz << 3);
    w_u32(state + 28, result);
    return result;
}

uint32 sub_8007fb24(uint32 kind, uint32 packet)
{
    uint32 position[3], vector[3], a, b, c, value;
    FUNCTION_MARKER(0x8007FB24, "ACE3.BIN");
    a = r_u32(packet + 4);
    b = r_u32(packet + 8);
    c = r_u32(packet + 12);
    position[0] = a << 12;
    position[1] = b << 12;
    position[2] = c << 12;
    a = r_u32(packet + 4);
    b = r_u32(packet + 8);
    c = r_u32(packet + 12);
    vector[0] = (uint32)((sint32)a >> 20) << 12;
    vector[1] = (uint32)((sint32)b >> 20) << 12;
    vector[2] = (uint32)((sint32)c >> 20) << 12;
    switch (kind)
    {
        case 0:
            return sub_80080534(0, 0, position, vector);
        case 1:
            value = r_u8(packet + 2);
            return sub_80080f70(0, 0, value, position, vector);
        case 2:
            value = r_u8(packet + 2);
            return sub_80081bc8(0, 0, value, position, vector);
        case 3:
            return sub_80081d4c(0, 0, position, vector);
        case 4:
            value = r_u32(packet);
            return sub_80081fdc(0, 0, (value >> 9) & 127, position, vector);
        case 5:
            value = r_u32(packet);
            return sub_800822a4(0, 0, (value >> 9) & 127, position, vector);
        case 6:
            value = r_u8(packet + 2);
            return sub_800827e0(0, 0, value, position, vector);
        case 7:
            return sub_80082a3c(0, 0, position, vector);
        case 8:
            return sub_80082c1c(0, 0, position, vector);
        case 9:
            return sub_800833c8(0, 0, position, vector);
        default:
            return 0x80020000;
    }
}

uint32 sub_8007a3b4(uint32 state, uint32 control, uint32 duration, const uint32 *native_context)
{
    uint32 mode, count, limit, numerator, quotient, angle, value, scene, origin, camera, height, plane, product, offset, result;
    FUNCTION_MARKER(0x8007A3B4, "ACE3.BIN");
    mode = r_u32(control);
    if (mode == 0)
    {
        w_u32(control + 28, 0);
        w_u32(control + 32, (sint32)duration > 0 ? duration : 150);
        w_u32(control, 1);
    }
    else if (mode != 1)
        return 0;
    count = r_u32(control + 28);
    limit = r_u32(control + 32);
    numerator = count << 14;
    if (limit == 0)
        xport_mips_break(7);
    if (limit == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)limit;
    if ((sint32)quotient > 4096)
        quotient = 4096;
    angle = ((quotient << 1) - 4096) & 0x3FFF;
    value = r_u16(0x800104AC + angle * 2);
    scene = r_u32(0x800BCE54);
    origin = native_context != 0 ? native_context[0] : r_u32(state);
    value = (uint32)((sint32)(value << 16) >> 19) + 2048;
    camera = r_u32(scene + 40);
    height = r_u32(origin + 4);
    plane = r_s16(camera + 40);
    height = (uint32)((sint32)(plane - 1000) >> 1) + (uint32)((sint32)height >> 12);
    if ((sint32)height < 0)
        height = 0;
    if ((sint32)height > 32767)
        height = 32767;
    product = height * value;
    value = r_u32(origin);
    w_u32(control + 4, value);
    origin = native_context != 0 ? native_context[0] : r_u32(state);
    offset = (uint32)((sint32)product >> 12) + 200;
    value = r_u32(origin + 4);
    w_u32(control + 8, value - (offset << 12));
    origin = native_context != 0 ? native_context[0] : r_u32(state);
    count = r_u32(control + 28);
    value = r_u32(origin + 8);
    ++count;
    w_u16(control + 20, -4096);
    w_u16(control + 22, 0);
    w_u16(control + 24, 0);
    w_u32(control + 28, count);
    w_u32(control + 12, value);
    result = 0;
    if ((sint32)duration > 0)
    {
        limit = r_u32(control + 32);
        result = (sint32)count >= (sint32)limit;
    }
    return result;
}

uint32 sub_8007affc(uint32 state)
{
    uint32 context[4], root, flags, active, selected, maximum, swapped, mode, value, a, b, c, d, e, f, source, count, scene, descriptor, index, result, target, duration;
    FUNCTION_MARKER(0x8007AFFC, "ACE3.BIN");
    root = r_u32(0x800BCE60);
    flags = r_u32(root + 0x12C);
    maximum = 450;
    selected = 0xFFFFFFFF;
    active = (flags & 0x209B80) != 0;
    value = r_u32(root + 0x128);
    if ((value & 0x209B00) != 0)
        maximum = 0;
    value = r_u32(root + 0x128);
    if ((value & 0x100) != 0)
        selected = 0;
    value = r_u32(root + 0x128);
    if ((value & 0x200) != 0)
        selected = 3;
    value = r_u32(root + 0x128);
    if ((value & 0x800) != 0)
        selected = 1;
    value = r_u32(root + 0x128);
    if ((value & 0x1000) != 0)
        selected = 2;
    value = r_u32(root + 0x128);
    if ((value & 0x8000) != 0)
        selected = 4;
    value = r_u32(root + 0x128);
    if ((value & 0x200000) != 0)
        selected = 5;
    value = r_s16(state + 0x15C);
    swapped = 0;
    if (value != 0)
    {
        value = r_u32(root + 36);
        if ((value & 256) != 0)
            swapped = 1;
    }
    value = r_s16(state + 0x15C);
    if (value == 2)
        swapped = 0;
    context[3] = state + 44;
    if (swapped == 0)
    {
        context[0] = state + 0x10C;
        context[2] = state + 0x124;
        context[1] = state + 0x134;
    }
    else
    {
        context[0] = state + 0x134;
        context[2] = state + 0x14C;
        context[1] = state + 0x10C;
    }
    mode = r_u32(state + 0xCC);
    if (mode == 0)
    {
        sub_8007786c(state);
        a = r_u32(state + 4);
        b = r_u32(state + 8);
        c = r_u32(state + 12);
        d = r_u16(state + 20);
        e = r_u16(state + 22);
        f = r_u16(state + 24);
        w_u32(state + 0x160, 0);
        w_u32(state + 0xF4, a);
        w_u32(state + 0xF8, b);
        w_u32(state + 0xFC, c);
        w_u16(state + 0x104, d);
        w_u16(state + 0x106, e);
        w_u16(state + 0x108, f);
        value = selected;
        if ((sint32)value < 0)
        {
            count = r_u32(state + 0xF0);
            value = r_u8(0x800B4104 + (count & 31));
            w_u32(state + 0xF0, count + 1);
        }
        w_u32(state + 0xEC, value);
        scene = r_u32(0x800BCE54);
        flags = r_u32(scene);
        if ((flags & 16) == 0)
        {
            value = r_s16(state + 0x15C);
            if (value == 0)
            {
                index = r_u32(state + 0xEC);
                descriptor = 0x800B40AC + (uint32)((sint32)index % 6) * 8;
            }
            else
            {
                index = r_u32(state + 0xEC);
                descriptor = 0x800B40DC + (uint32)((sint32)index % 2) * 8;
            }
        }
        else
        {
            value = r_s16(state + 0x15C);
            if (value == 0)
            {
                index = r_u32(state + 0xEC);
                descriptor = 0x800B40EC + (uint32)((sint32)index % 3) * 8;
            }
            else
                descriptor = 0x800BCE28;
        }
        w_u32(0x800BD0A8, descriptor);
        w_u32(state + 0xCC, 1);
        mode = 1;
    }
    if (mode == 1)
    {
        descriptor = r_u32(0x800BD0A8);
        value = r_s16(descriptor + 4);
        duration = maximum;
        if ((sint32)value < (sint32)duration)
            duration = value;
        target = r_u32(descriptor);
        result = xport_guest_call_camera3(target, context, state + 0x160, duration);
        descriptor = r_u32(0x800BD0A8);
        value = r_s16(descriptor + 6);
        if (value == 1)
        {
            sub_8007ae40(state + 0x164, state + 0xF4);
            w_u32(state + 52, state + 0xF4);
        }
        else
            w_u32(state + 52, state + 0x164);
        if (result != 0 || active != 0)
        {
            w_u16(state + 0x15E, 0);
            active = 0;
            w_u32(state + 0xCC, 0);
        }
        else
        {
            value = r_s16(state + 0x15E);
            if (value != 0)
            {
                w_u16(state + 0x15E, 0);
                active = 0;
                w_u32(state + 0xCC, 0);
            }
        }
    }
    source = r_u32(state + 52);
    a = r_u32(source);
    b = r_u32(source + 4);
    c = r_u32(source + 8);
    d = r_u32(source + 12);
    w_u32(state + 4, a);
    w_u32(state + 8, b);
    w_u32(state + 12, c);
    w_u32(state + 16, d);
    e = r_u32(source + 16);
    f = r_u32(source + 20);
    w_u32(state + 20, e);
    w_u32(state + 24, f);
    return source;
}

uint32 sub_8007b438(uint32 object)
{
    uint32 saved_position[3], saved_angles[3], i, value, magnitude, flags, scene, root, a, b, c, mode, table, target, index, timer, source, raw, pitch;
    FUNCTION_MARKER(0x8007B438, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
        saved_position[i] = r_u32(object + 4 + i * 4);
    for (i = 0; i < 3; ++i)
        saved_angles[i] = r_u16(object + 20 + i * 2);
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(object + 180 + i * 2);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude >= 49)
            break;
    }
    flags = r_u32(object);
    if (i < 3)
        flags |= 0x200;
    else
        flags &= 0xFFFFFDFF;
    w_u32(object, flags);
    scene = r_u32(0x800BCE54);
    flags = r_u32(scene);
    if ((flags & 64) != 0)
    {
        root = r_u32(0x800BCE60);
        w_u32(scene, flags & 0xFFFFFFBF);
        a = r_u32(root + 0x128);
        b = r_u32(root + 0x130);
        w_u32(root + 0x128, a & 0xFFF0FFFF);
        c = r_u32(root + 0x12C);
        w_u32(root + 0x130, b & 0xFFF0FFFF);
        w_u32(root + 0x12C, c & 0xFFF0FFFF);
    }
    mode = r_u32(object + 80);
    switch (mode)
    {
        case 1:
            sub_80077a30(object);
            root = r_u32(0x800BCE60);
            flags = r_u32(root + 0x12C);
            if ((flags & 0x4000) != 0)
            {
                value = r_u32(object);
                table = r_u32(0x800BCE5C);
                target = r_u32(table + (value & 63) * 4);
                sub_800793c4(object, 2, target);
                root = r_u32(0x800BCE60);
                w_u8(root + 0x39F, 1);
            }
            break;
        case 2:
            sub_80077dd0(object);
            root = r_u32(0x800BCE60);
            flags = r_u32(root + 0x12C);
            if ((flags & 0x4000) != 0)
            {
                table = r_u32(0x800BCE5C);
                target = r_u32(table);
                sub_800793c4(object, 1, target);
                root = r_u32(0x800BCE60);
                w_u8(root + 0x39F, 0);
            }
            break;
        case 4:
            sub_8007895c(object);
            break;
        case 16:
            sub_80078b28(object);
            break;
        case 32:
            sub_80078d70(object);
            break;
        case 64:
            sub_80078fdc(object);
            break;
        case 256:
            sub_800791e8(object);
            break;
        case 512:
            sub_8007affc(object);
            sub_8007867c(object);
            root = r_u32(0x800BCE60);
            value = r_u8(root + 0x3A5);
            if (value == 0)
            {
                value = r_u32(object);
                w_u32(object, value | 0x800);
            }
            break;
        case 1024:
            sub_80077a30(object);
            break;
        case 2048:
            value = r_u32(object + 192);
            sub_80078234(object, value);
            root = r_u32(0x800BCE60);
            flags = r_u32(root + 0x12C);
            value = r_u32(root);
            if ((value & 0x20000000) == 0)
            {
                timer = r_u32(object + 208) + 1;
                w_u32(object + 208, timer);
                if ((sint32)timer >= 181 || (flags & 0x60F800) != 0)
                {
                    value = r_u8(root + 0x39F);
                    table = r_u32(0x800BCE5C);
                    target = r_u32(table);
                    sub_800793c4(object, value != 0 ? 2 : 1, target);
                    sub_80035740(1);
                }
            }
            break;
        case 0x40000000:
            root = r_u32(0x800BCE60);
            value = r_u16(root + 94);
            sub_80077004(object + 4, object + 100, object + 84, value);
            root = r_u32(0x800BCE60);
            flags = r_u32(root + 0x12C);
            if ((flags & 0x4000) != 0)
            {
                value = r_u32(object);
                index = (value & 63) + 1;
                if (index < 64)
                {
                    table = r_u32(0x800BCE5C) + index * 4;
                    do
                    {
                        target = r_u32(table);
                        value = r_u32(target);
                        ++index;
                        if ((value & 0x4000) != 0)
                        {
                            sub_800793c4(object, 0x20000000, target);
                            break;
                        }
                        table += 4;
                    } while (index < 64);
                }
            }
            break;
        case 0x20000000:
            root = r_u32(0x800BCE60);
            value = r_u16(root + 94);
            if ((value & 0x8000) != 0)
            {
                value = r_u16(root + 94);
                sub_8007754c(object, value);
            }
            else
                sub_80077dd0(object);
            root = r_u32(0x800BCE60);
            flags = r_u32(root + 0x12C);
            if ((flags & 0x4000) != 0)
            {
                value = r_u32(object);
                index = (value & 63) + 1;
                if (index < 64)
                {
                    table = r_u32(0x800BCE5C) + index * 4;
                    do
                    {
                        target = r_u32(table);
                        value = r_u32(target);
                        if ((value & 0x4000) != 0)
                        {
                            value = r_u32(object + 80);
                            sub_800793c4(object, value, target);
                            break;
                        }
                        ++index;
                        table += 4;
                    } while (index < 64);
                }
                if (index == 64)
                {
                    table = r_u32(0x800BCE5C);
                    target = r_u32(table);
                    sub_800793c4(object, 1, target);
                    sub_80035740(1);
                }
            }
            break;
        default:
            break;
    }
    flags = r_u32(object);
    if ((flags & 0x100) != 0)
    {
        source = r_u32(object + 52);
        for (i = 0; i < 3; ++i)
        {
            value = r_u32(source + i * 4);
            w_u32(object + 4 + i * 4, value);
        }
        for (i = 0; i < 3; ++i)
        {
            a = r_u32(object + 4 + i * 4);
            b = r_u32(object + 56 + i * 4);
            w_u32(object + 4 + i * 4, a + b);
        }
        for (i = 0; i < 3; ++i)
        {
            value = r_u16(source + 16 + i * 2);
            w_u16(object + 20 + i * 2, value);
        }
        for (i = 0; i < 2; ++i)
        {
            a = r_u16(object + 20 + i * 2);
            b = r_u16(object + 72 + i * 2);
            w_u16(object + 20 + i * 2, a + b);
        }
        a = r_u16(object + 24);
        b = r_u16(object + 76);
        pitch = r_s16(object + 20);
        raw = r_u16(object + 20);
        w_u16(object + 24, a + b);
        if ((sint32)pitch >= 4097)
        {
            w_u16(object + 20, 8192 - raw);
            a = r_u16(object + 22);
            b = r_u16(object + 24);
            w_u16(object + 22, a + 8192);
            w_u16(object + 24, b + 8192);
        }
        pitch = r_s16(object + 20);
        raw = r_u16(object + 20);
        if ((sint32)pitch < -4096)
        {
            w_u16(object + 20, 0 - 8192 - raw);
            a = r_u16(object + 22);
            b = r_u16(object + 24);
            w_u16(object + 22, a + 8192);
            w_u16(object + 24, b + 8192);
        }
        for (i = 0; i < 2; ++i)
        {
            value = r_s16(object + 22 + i * 2);
            raw = r_u16(object + 22 + i * 2);
            if ((sint32)value >= 8192)
                w_u16(object + 22 + i * 2, raw - 16384);
            value = r_s16(object + 22 + i * 2);
            raw = r_u16(object + 22 + i * 2);
            if ((sint32)value < -8192)
                w_u16(object + 22 + i * 2, raw + 16384);
        }
    }
    flags = r_u32(object);
    if ((flags & 0x800) != 0 && (flags & 0x1000) == 0)
    {
        scene = r_u32(0x800BCE54);
        w_u32(object, flags & 0xFFFFF7FF);
        flags = r_u32(scene);
        if ((flags & 16) == 0)
        {
            value = sub_80079cb0(object + 4);
            a = r_u32(object + 8);
            w_u32(object + 8, a + value);
        }
        else
            sub_80079cfc(object + 124, object + 140, object + 4);
    }
    flags = r_u32(object);
    if ((flags & 0x400) == 0)
    {
        for (i = 0; i < 3; ++i)
        {
            value = r_u32(object + 4 + i * 4);
            w_u32(object + 156 + i * 4, value - saved_position[i]);
        }
        for (i = 0; i < 3; ++i)
        {
            value = r_u16(object + 20 + i * 2);
            w_u16(object + 172 + i * 2, value - saved_angles[i]);
        }
    }
    else
    {
        w_u32(object, flags & 0xFFFFFBFF);
        w_u32(object + 156, 0);
        w_u32(object + 160, 0);
        w_u32(object + 164, 0);
        w_u16(object + 172, 0);
        w_u16(object + 174, 0);
        w_u16(object + 176, 0);
    }
    root = r_u32(0x800BCE60);
    value = r_u32(root) & 1;
    if (value != 0)
        return sub_80077460(object);
    return value;
}

uint32 sub_80079cfc(uint32 first, uint32 second, uint32 output)
{
    uint32 i, value, bound, result;
    FUNCTION_MARKER(0x80079CFC, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(output + i * 4);
        bound = r_u32(first + i * 4);
        if ((sint32)value < (sint32)bound)
            w_u32(output + i * 4, bound);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(output + i * 4);
        bound = r_u32(second + i * 4);
        result = (sint32)bound < (sint32)value;
        if (result != 0)
            w_u32(output + i * 4, bound);
    }
    return result;
}

uint32 sub_80077460(uint32 state)
{
    uint32 packet, value, result, i;
    FUNCTION_MARKER(0x80077460, "ACE3.BIN");
    packet = sub_80027608(230, 180, 0x800BCE24, 0) + 12;
    value = r_u32(state);
    sub_800277f0(packet, 180, value, 8);
    value = r_u32(state + 80);
    sub_800277f0(packet, 186, value, 8);
    for (i = 0; i < 3; ++i)
    {
        value = r_u32(state + 4 + i * 4);
        sub_800276c8(packet, 192 + i * 6, (sint32)value >> 12, 0x207);
    }
    for (i = 0; i < 3; ++i)
    {
        value = r_s16(state + 20 + i * 2);
        result = sub_800276c8(packet, 210 + i * 6, value, 0x207);
    }
    return result;
}
