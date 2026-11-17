#include "game_functions.h"

static const uint16 camera_shake_initial[8][2] = {
    {0, 2}, {5, 6}, {6, 6}, {0, 2}, {5, 3}, {0, 2}, {6, 4}, {3, 1},
};

static const uint16 camera_shake_default[8][2] = {
    {0, 0}, {1, 0}, {1, 2}, {2, 0}, {0, 1}, {1, 2}, {0, 1}, {1, 1},
};

static const uint16 camera_shake_alternate[8][2] = {
    {0, 0}, {0, 1}, {0, 0}, {0, 1}, {0, 0}, {0, 1}, {0, 0}, {0, 1},
};

static const GAME_CAMERA_SHOT_DESC camera_shot_main[6] = {
    {camera_shot_hold, 90, 0}, {camera_shot_track, 150, 0}, {camera_shot_overhead, 210, 0}, {camera_shot_dolly, 270, 0}, {camera_shot_orbit, 270, 1}, {camera_shot_trail, 450, 1},
};

static const GAME_CAMERA_SHOT_DESC camera_shot_alternate[2] = {
    {camera_shot_target, 390, 1},
    {camera_shot_target_pan, 390, 1},
};

static const GAME_CAMERA_SHOT_DESC camera_shot_mission[3] = {
    {camera_shot_dolly, 450, 0},
    {camera_shot_orbit, 450, 1},
    {camera_shot_trail, 450, 1},
};

static const GAME_CAMERA_SHOT_DESC camera_shot_mission_target = {camera_shot_target_pan, 390, 1};

static const uint8 camera_shot_order[32] = {5, 2, 4, 1, 0, 2, 3, 0, 3, 5, 1, 2, 4, 3, 0, 4, 4, 1, 0, 3, 4, 2, 1, 5, 0, 3, 5, 2, 1, 4, 3, 2};

void camera_clamp_position(const uint32 *first, const uint32 *second, uint32 *output)
{
    uint32 i, value, bound, result;
    FUNCTION_MARKER(0x80079CFC, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        value = output[i];
        bound = first[i];
        if ((sint32)value < (sint32)bound)
            output[i] = (bound);
    }
    for (i = 0; i < 3; ++i)
    {
        value = output[i];
        bound = second[i];
        result = (sint32)bound < (sint32)value;
        if (result != 0)
            output[i] = (bound);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_debug_move(NATIVE_POSE *pose, uint32 input)
{
    uint32 work[4], displacement[3];
    uint16 turn[4];
    uint32 pitch, yaw, roll, x, y, z, shift, value, delta;
    FUNCTION_MARKER(0x80077004, "ACE3.BIN");
    pitch = 0;
    yaw = 0;
    roll = 0;
    x = 0;
    y = 0;
    z = 0;
    shift = (input << 1) & 4;
    if ((input & 0x10) != 0)
        pitch -= 64;
    if ((input & 0x40) != 0)
        pitch += 64;
    if ((input & 0x20) != 0)
        roll += 64;
    if ((input & 0x80) != 0)
        roll -= 64;
    if ((input & 0x200) != 0)
        yaw += 64;
    if ((input & 0x100) != 0)
        yaw -= 64;
    if ((input & 0x14) == 0x14)
        pitch += 60;
    if ((input & 0x44) == 0x44)
        pitch -= 60;
    if ((input & 0x24) == 0x24)
        roll -= 60;
    if ((input & 0x84) == 0x84)
        roll += 60;
    if ((input & 0x204) == 0x204)
        yaw -= 60;
    if ((input & 0x104) == 0x104)
        yaw += 60;
    pitch = (pitch << shift) & 0xFFFF;
    roll = (roll << shift) & 0xFFFF;
    yaw = (yaw << shift) & 0xFFFF;
    if ((input & 0x1000) != 0)
        z += 0x40000;
    if ((input & 0x4000) != 0)
        z -= 0x40000;
    if ((input & 0x8000) != 0)
        x -= 0x40000;
    if ((input & 0x2000) != 0)
        x += 0x40000;
    if ((input & 0x400) != 0)
        y -= 0x40000;
    if ((input & 0x800) != 0)
        y += 0x40000;
    if ((input & 0x1004) == 0x1004)
        z -= 0x3F000;
    if ((input & 0x4004) == 0x4004)
        z += 0x3F000;
    if ((input & 0x8004) == 0x8004)
        x += 0x3F000;
    if ((input & 0x2004) == 0x2004)
        x -= 0x3F000;
    if ((input & 0x404) == 0x404)
        y += 0x3F000;
    if ((input & 0x804) == 0x804)
        y -= 0x3F000;
    x <<= shift;
    z <<= shift;
    y <<= shift;
    work[0] = (x);
    work[1] = (y);
    work[2] = (z);
    work[3] = 0;
    {
        uint32 word = pitch | (yaw << 16);
        turn[0] = word;
        turn[1] = (word >> 16);
    };
    {
        uint32 word = roll;
        turn[2] = word;
        turn[3] = (word >> 16);
    };
    game_angles_apply_delta(turn, pose->angles);
    game_rotate_vector(pose->angles, work, displacement);
    if ((input & 0xC00) == 0xC00)
    {
        pose->angles[0] = (uint16)(0xF000);
        pose->angles[1] = (uint16)(0);
        pose->angles[2] = (uint16)(0);
    }
    if ((input & 6) == 6)
    {
        pose->angles[0] = (uint16)(0);
        pose->angles[1] = (uint16)(0);
        pose->angles[2] = (uint16)(0);
    }
    value = pose->position[0];
    delta = displacement[0];
    pose->position[0] = (uint32)(value + delta);
    value = pose->position[1];
    delta = displacement[1];
    pose->position[1] = (uint32)(value + delta);
    value = pose->position[2];
    delta = displacement[2];
    value += delta;
    pose->position[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 camera_ground_offset(const uint32 *position)
{
    uint16 normal[3];
    uint32 surface, result, altitude;
    FUNCTION_MARKER(0x80079CB0, "ACE3.BIN");
    result = scene_query_ground(position, normal, &surface);
    altitude = position[1];
    result = result + 0xFFF7C000 - altitude;
    return (sint32)result < 0 ? result : 0;
}

void camera_mark_dirty(void)
{
    uint32 flags;

    FUNCTION_MARKER(0x8007786C, "ACE3.BIN");
    flags = game_runtime.camera_control.flags | 0x400;
    game_runtime.camera_control.flags = (uint32)(flags);
    {
        (void)(uint32)(flags);
        return;
    }
}

void camera_set_clip(GAME_CAMERA_PROJ *proj, uint32 near_value, uint32 far_value, uint32 height, uint16 *buckets)
{
    FUNCTION_MARKER(0x800777B0, "ACE3.BIN");
    if ((sint32)far_value < (sint32)near_value)
        return;
    if ((sint32)near_value < 1000)
        near_value = 1000;
    else if ((sint32)near_value >= 31768)
        near_value = 31767;
    if ((sint32)far_value < 1000)
        far_value = 1000;
    else if ((sint32)far_value >= 31768)
        far_value = 31767;
    scene_build_depth_buckets(near_value, far_value, buckets);
    proj->near_plane = (uint16)(near_value);
    proj->far_plane = (uint16)(far_value);
    if (height == 0)
        height = ((sint16)proj->distance);
    camera_set_proj(proj, height);
    return;
}

void camera_set_mode(uint32 mode, const MISSION_ACTOR_TRAITS *actor_traits, uint32 actor_index)
{
    uint32 previous = 0xFFFFFFFF;
    uint32 flags;
    uint32 value;
    uint32 result;

    FUNCTION_MARKER(0x800793C4, "ACE3.BIN");
    camera_mark_dirty();
    if (mode & 0x401)
        previous = actor_traits->flags & 63;
    if (mode & 0xDFF)
    {
        flags = game_runtime.camera_control.flags & 0xFFFFFFC0;
        game_runtime.camera_control.flags = (uint32)(flags);
        value = actor_traits->flags;
        game_runtime.camera_control.flags = (uint32)(flags | 0x100 | (value & 63));
        value = actor_traits->flags;
        mission_camera_bind_actor(actor_index);
        game_runtime.camera_follow.offset[0] = (uint32)(0);
        game_runtime.camera_follow.offset[1] = (uint32)(0);
        game_runtime.camera_follow.offset[2] = (uint32)(0);
        game_runtime.camera_follow.angles[0] = (uint16)(0);
        game_runtime.camera_follow.angles[1] = (uint16)(0);
        game_runtime.camera_follow.angles[2] = (uint16)(0);
        game_runtime.camera_control.follow_actor = (uint32)(value & 63);
        result = actor_traits->flags;
        game_runtime.camera_control.excluded_actor = (uint32)(previous);
        game_runtime.camera_control.follow_actor = (uint32)(result & 63);
    }
    else
    {
        flags = game_runtime.camera_control.flags;
        game_runtime.camera_control.excluded_actor = (uint32)(0xFFFFFFFF);
        result = flags & 0xFFFFFEFF;
        game_runtime.camera_control.flags = (uint32)(result);
    }
    game_runtime.camera_control.mode = (uint32)(mode);
    game_runtime.camera_control.mode_ticks = (uint32)(0);
    game_runtime.camera_control.shot_phase = (uint32)(0);
    game_runtime.camera_follow.orbit_angles[0] = (uint16)(0);
    game_runtime.camera_follow.orbit_angles[1] = (uint16)(0);
    game_runtime.camera_follow.orbit_angles[2] = (uint16)(0);
    game_runtime.camera_follow.orbit_offset[0] = (uint32)(0);
    game_runtime.camera_follow.orbit_offset[1] = (uint32)(0);
    game_runtime.camera_follow.orbit_offset[2] = (uint32)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_set_proj(GAME_CAMERA_PROJ *proj, uint32 height)
{
    uint32 near_value, far_value;
    FUNCTION_MARKER(0x80077768, "ACE3.BIN");
    proj->distance = (uint16)(height);
    SetGeomScreen((sint32)height);
    near_value = ((sint16)proj->near_plane);
    far_value = ((sint16)proj->far_plane);
    {
        (void)(scene_geometry_set_depth_cue(near_value, far_value, height));
        return;
    }
}

uint32 camera_shot_dolly(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, value, limit, count, divisor, difference, numerator, quotient, offset, i;
    FUNCTION_MARKER(0x8007A52C, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->phase = (uint32)(1);
        control->limit = (uint32)((sint32)duration > 0 ? duration : 450);
        value = camera_shot_speed(context);
        control->distance = (uint32)(value);
        if ((sint32)value <= 0x29FFF)
            control->distance = (uint32)(0x2A000);
    }
    else if (mode != 1)
        return 0;
    limit = control->limit;
    count = control->ticks;
    divisor = (sint32)limit >> 1;
    difference = count - divisor;
    numerator = ((difference << 4) - difference) << 5;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    value = control->distance;
    camera_shot_offset_forward(context, value, 0 - quotient, control->output.pose.position);
    for (i = 0; i < 3; ++i)
    {
        offset = ((sint16)context->offset[i]);
        value = control->output.pose.position[i];
        control->output.pose.position[i] = (uint32)(value - (offset << 6));
    }
    camera_shot_face_origin(context, control->output.pose.position, &control->output.pose);
    count = control->ticks;
    limit = control->limit;
    ++count;
    control->ticks = (uint32)(count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    control->phase = (uint32)(0);
    return 0;
}

void camera_shot_face_origin(const GAME_CAMERA_SHOT_CONTEXT *context, const uint32 *position, NATIVE_POSE *output)
{
    uint32 x, y, z, a, b, c, difference[3], value;
    FUNCTION_MARKER(0x8007A060, "ACE3.BIN");
    x = position[0];
    y = position[1];
    z = position[2];
    a = context->first->position[0];
    b = context->first->position[1];
    c = context->first->position[2];
    difference[0] = a - x;
    difference[1] = b - y;
    difference[2] = c - z;
    camera_vector_angles(difference, output->angles);
    value = position[0];
    output->position[0] = (uint32)(value);
    value = position[1];
    output->position[1] = (uint32)(value);
    value = position[2];
    output->position[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 camera_shot_hold(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, value, delay, limit, offset, current, i, count;
    FUNCTION_MARKER(0x8007A0F0, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        for (i = 0; i < 3; ++i)
        {
            value = context->first->angles[i];
            control->output.pose.angles[i] = (uint16)(value);
        }
        if ((sint32)duration > 0)
        {
            delay = (sint32)duration < 13 ? (uint32)((sint32)duration >> 1) : 12;
            limit = duration - delay;
        }
        else
        {
            delay = game_runtime.frame_counter & 15;
            limit = delay + 50;
        }
        control->limit = (uint32)(limit);
        camera_shot_predict(context, delay << 5, control->output.pose.position);
        for (i = 0; i < 2; ++i)
        {
            offset = ((sint16)context->offset[i]);
            current = control->output.pose.position[i];
            control->output.pose.position[i] = (uint32)(current + (offset << 6));
        }
        offset = ((sint16)context->offset[2]);
        control->phase = (uint32)(1);
        current = control->output.pose.position[2];
        control->output.pose.position[2] = (uint32)(current + (offset << 6));
        return 0;
    }
    if (mode != 1)
        return 0;
    count = control->ticks;
    limit = control->limit;
    ++count;
    control->ticks = (uint32)(count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    control->phase = (uint32)(0);
    return 0;
}

void camera_shot_offset_forward(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 distance, uint32 scale, uint32 *native_output)
{
    MATRIX matrix;
    uint32 a, b, c, d, e, x, y, z, base_x, base_y, base_z;
    FUNCTION_MARKER(0x800799D4, "ACE3.BIN");
    rotation_build(&matrix, context->first->angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    distance = (sint32)distance >> 5;
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, distance & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x190003D);
    base_x = context->first->position[0];
    base_y = context->first->position[1];
    base_z = context->first->position[2];
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    x += base_x;
    y += base_y;
    z += base_z;
    {
        native_output[0] = x;
        native_output[1] = y;
        native_output[2] = z;
    }
    {
        (void)(uint32)(x);
        return;
    }
}

void camera_shot_offset_orbit(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 scale, const uint16 *native_angles, NATIVE_POSE *output)
{
    MATRIX matrix;
    uint32 value, length, a, b, c, d, e, x, y, z, first_angle, base_x, base_y, base_z;
    FUNCTION_MARKER(0x80079F2C, "ACE3.BIN");
    value = ((sint16)context->offset[2]);
    length = (sint32)(value * scale) >> 18;
    if ((sint32)length > 32767)
        length = 32767;
    if ((sint32)length < -32768)
        length = 0xFFFF8000;
    rotation_build(&matrix, native_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, length & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    first_angle = native_angles[0];
    base_x = context->first->position[0];
    base_y = context->first->position[1];
    base_z = context->first->position[2];
    output->angles[0] = (uint16)(first_angle);
    value = native_angles[1];
    output->angles[1] = (uint16)(value);
    value = native_angles[2];
    output->position[0] = (uint32)(base_x + (x << 12));
    output->position[1] = (uint32)(base_y + (y << 12));
    output->position[2] = (uint32)(base_z + (z << 12));
    output->angles[2] = (uint16)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_shot_offset_target(const GAME_CAMERA_SHOT_CONTEXT *context, const uint32 *native_offset, NATIVE_POSE *output)
{
    MATRIX matrix;
    uint32 direction[3], a, b, c, d, e, x, y, z, ax, ay, az, i;
    uint16 vector[3];
    FUNCTION_MARKER(0x80079DAC, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        a = context->second->position[i];
        b = context->first->position[i];
        direction[i] = a - b;
    }
    camera_vector_angles(direction, output->angles);
    rotation_build(&matrix, output->angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    x = native_offset[0];
    y = native_offset[1];
    z = native_offset[2];
    ax = ((sint16)context->offset[0]);
    ay = ((sint16)context->offset[1]);
    az = ((sint16)context->offset[2]);
    vector[0] = ax + (uint32)((sint32)x >> 6);
    vector[1] = ay + (uint32)((sint32)y >> 6);
    vector[2] = az + (uint32)((sint32)z >> 6);
    xport_gte_write_data(0, vector[0] | ((uint32)vector[1] << 16));
    xport_gte_write_data(1, vector[2]);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    ax = context->first->position[0];
    ay = context->first->position[1];
    az = context->first->position[2];
    x = (x << 6) + ax;
    y = (y << 6) + ay;
    z = (z << 6) + az;
    output->position[0] = (uint32)(x);
    output->position[1] = (uint32)(y);
    output->position[2] = (uint32)(z);
    {
        (void)(uint32)(z);
        return;
    }
}

uint32 camera_shot_orbit(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint16 angles[3];
    uint32 mode, divisor, numerator, quotient, magnitude, flags, value, scale, count, limit;
    FUNCTION_MARKER(0x8007A6CC, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->phase = (uint32)(1);
        control->limit = (uint32)((sint32)duration > 0 ? duration : 320);
        value = (uint8)game_runtime.frame_counter;
        control->flags = (uint32)(value);
    }
    else if (mode != 1)
        return 0;
    limit = control->limit;
    count = control->ticks;
    divisor = (sint32)limit >> 1;
    numerator = (count - divisor) << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    flags = control->flags;
    magnitude = (sint32)quotient < 0 ? 0 - quotient : quotient;
    value = game_wave_at((((magnitude + 0x1000) & 0x3FFF) << 1));
    scale = (0x4000 - value) * ((flags & 3) + 1) + 0x1000;
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
    if ((flags & 0x80) != 0)
    {
        angles[0] = quotient;
        if ((flags & 0x40) == 0)
        {
            angles[0] = 0 - quotient;
            flags = control->flags;
        }
    }
    if ((flags & 0x20) != 0)
        angles[1] = (flags & 0x10) != 0 ? quotient : 0 - quotient;
    camera_shot_offset_orbit(context, scale, angles, &control->output.pose);
    flags = control->flags;
    if ((flags & 8) != 0)
    {
        value = context->first->angles[2];
        if ((flags & 4) == 0)
            value = 0 - value;
        control->output.pose.angles[2] = (uint16)(value);
    }
    count = control->ticks;
    limit = control->limit;
    ++count;
    control->ticks = (uint32)(count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    control->phase = (uint32)(0);
    return 0;
}

uint32 camera_shot_overhead(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, count, limit, numerator, quotient, angle, value, height, plane, product, offset, result;
    FUNCTION_MARKER(0x8007A3B4, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->limit = (uint32)((sint32)duration > 0 ? duration : 150);
        control->phase = (uint32)(1);
    }
    else if (mode != 1)
        return 0;
    count = control->ticks;
    limit = control->limit;
    numerator = count << 14;
    if (limit == 0)
        xport_mips_break(7);
    if (limit == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)limit;
    if ((sint32)quotient > 4096)
        quotient = 4096;
    angle = ((quotient << 1) - 4096) & 0x3FFF;
    value = (uint16)game_wave_at(angle * 2);
    value = (uint32)((sint32)(value << 16) >> 19) + 2048;
    height = context->first->position[1];
    plane = ((sint16)game_runtime.camera_proj.near_plane);
    height = (uint32)((sint32)(plane - 1000) >> 1) + (uint32)((sint32)height >> 12);
    if ((sint32)height < 0)
        height = 0;
    if ((sint32)height > 32767)
        height = 32767;
    product = height * value;
    value = context->first->position[0];
    control->output.pose.position[0] = (uint32)(value);
    offset = (uint32)((sint32)product >> 12) + 200;
    value = context->first->position[1];
    control->output.pose.position[1] = (uint32)(value - (offset << 12));
    count = control->ticks;
    value = context->first->position[2];
    ++count;
    control->output.pose.angles[0] = (uint16)(-4096);
    control->output.pose.angles[1] = (uint16)(0);
    control->output.pose.angles[2] = (uint16)(0);
    control->ticks = (uint32)(count);
    control->output.pose.position[2] = (uint32)(value);
    result = 0;
    if ((sint32)duration > 0)
    {
        limit = control->limit;
        result = (sint32)count >= (sint32)limit;
    }
    return result;
}

void camera_shot_predict(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 scale, uint32 *native_output)
{
    uint32 x, y, z, ox, oy, oz, a, b, c;
    FUNCTION_MARKER(0x8007995C, "ACE3.BIN");
    x = context->velocity[0];
    y = context->velocity[1];
    z = context->velocity[2];
    x = (sint32)x >> 5;
    y = (sint32)y >> 5;
    z = (sint32)z >> 5;
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x190003D);
    oy = context->first->position[1];
    oz = context->first->position[2];
    ox = context->first->position[0];
    a = xport_gte_read_data(25);
    b = xport_gte_read_data(26);
    c = xport_gte_read_data(27);
    b += oy;
    c += oz;
    {
        native_output[1] = b;
        native_output[2] = c;
    }
    a += ox;
    native_output[0] = a;
    {
        (void)(uint32)(a);
        return;
    }
}

void camera_shot_smooth(const uint32 *source_position, const uint16 *source_angles, uint32 *destination_position, uint16 *destination_angles)
{
    uint32 current[3], target[3], step[3], value, magnitude, i;
    FUNCTION_MARKER(0x8007AE40, "ACE3.BIN");
    current[0] = ((sint16)destination_angles[0]);
    current[1] = ((sint16)destination_angles[1]);
    target[1] = ((sint16)source_angles[1]);
    current[2] = ((sint16)destination_angles[2]);
    target[0] = ((sint16)source_angles[0]);
    target[2] = ((sint16)source_angles[2]);
    for (i = 1; i < 3; ++i)
    {
        value = current[i] - target[i];
        if ((sint32)value < -8192)
        {
            current[i] += 0x4000;
            value = current[i] - target[i];
        }
        if ((sint32)value >= 8193)
            current[i] -= 0x4000;
    }
    for (i = 0; i < 3; ++i)
    {
        value = current[i] - target[i];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        step[i] = (sint32)(magnitude + 1) >> (i == 1 ? 3 : 4);
    }
    for (i = 0; i < 3; ++i)
    {
        if ((sint32)current[i] < (sint32)target[i])
            current[i] += step[i];
        if ((sint32)target[i] < (sint32)current[i])
            current[i] -= step[i];
    }
    for (i = 0; i < 3; ++i)
    {
        value = current[i] - target[i];
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude < (sint32)step[i])
            current[i] = step[i];
    }
    destination_angles[0] = (current[0]);
    destination_angles[1] = (current[1]);
    destination_angles[2] = (current[2]);
    value = source_position[0];
    destination_position[0] = (value);
    value = source_position[1];
    destination_position[1] = (value);
    value = source_position[2];
    destination_position[2] = (value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 camera_shot_speed(const GAME_CAMERA_SHOT_CONTEXT *context)
{
    uint32 x, y, z;
    FUNCTION_MARKER(0x80079AE8, "ACE3.BIN");
    x = context->velocity[0];
    y = context->velocity[1];
    z = context->velocity[2];
    return game_vector_length_gte(x, y, z) << 3;
}

uint32 camera_shot_target(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, value, angle, sine, offset[3], flags, count, limit;
    FUNCTION_MARKER(0x8007AAB4, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->phase = (uint32)(1);
        value = game_runtime.frame_counter;
        control->limit = (uint32)(duration);
        control->distance = (uint32)(0);
        control->flags = (uint32)(value & 15);
    }
    else if (mode != 1)
        return 0;
    game_measure_distance_gte(context->first->position, context->second->position);
    value = control->distance;
    if (value != 0)
        control->distance = (uint32)(value - 1);
    angle = context->first->angles[2];
    sine = game_wave_at(((angle & 0x3FFF) << 1));
    angle = context->first->angles[0];
    offset[2] = 0xFFFB0000;
    offset[0] = ((sine << 2) + sine) << 1;
    sine = game_wave_at(((angle & 0x3FFF) << 1));
    offset[1] = (uint32)((sint32)((sine << 4) - sine) >> 1) + 0xFFFEC000;
    camera_shot_offset_target(context, offset, &control->output.pose);
    flags = control->flags;
    if ((flags & 8) != 0)
    {
        value = context->first->angles[2];
        if ((flags & 4) == 0)
            value = 0 - value;
        control->output.pose.angles[2] = (uint16)(value);
    }
    count = control->ticks + 1;
    control->ticks = (uint32)(count);
    if (duration == 0)
        return 0;
    limit = control->limit;
    return (sint32)count >= (sint32)limit;
}

uint32 camera_shot_target_pan(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, count, divisor, numerator, quotient, flags, angle, value, offset[3], limit;
    FUNCTION_MARKER(0x8007AC50, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->phase = (uint32)(1);
        value = game_runtime.frame_counter;
        control->flags = (uint32)(value & 15);
        control->limit = (uint32)((sint32)duration > 0 ? duration : 60);
    }
    else if (mode != 1)
        return 0;
    count = control->ticks;
    divisor = control->limit;
    numerator = count << 12;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    quotient = (sint32)numerator / (sint32)divisor;
    flags = control->flags;
    angle = (((0x1000 - quotient) << 1) - 0x1000) & 0x3FFF;
    value = (uint16)game_wave_at((angle << 1));
    value = (uint32)((sint32)(value << 16) >> 19) + 0x800;
    offset[0] = 0;
    offset[1] = 0xFFFEC000;
    offset[2] = 0xFFFB0000;
    if ((flags & 2) != 0)
    {
        value = ((value << 4) - value) << 15;
        if ((flags & 1) == 0)
            value = 0 - value;
        offset[0] = (sint32)value >> 12;
    }
    camera_shot_offset_target(context, offset, &control->output.pose);
    flags = control->flags;
    if ((flags & 8) != 0)
    {
        value = context->first->angles[2];
        if ((flags & 4) == 0)
            value = 0 - value;
        control->output.pose.angles[2] = (uint16)(value);
    }
    count = control->ticks;
    limit = control->limit;
    if ((sint32)count < (sint32)limit)
        control->ticks = (uint32)(count + 1);
    if ((sint32)duration <= 0)
        return 0;
    count = control->ticks;
    limit = control->limit;
    return (sint32)count >= (sint32)limit;
}

uint32 camera_shot_track(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, delay, limit, offset, value, i, count;
    FUNCTION_MARKER(0x8007A260, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        if ((sint32)duration > 0)
        {
            delay = (sint32)duration < 31 ? (uint32)((sint32)duration >> 1) : 30;
            limit = duration - delay;
        }
        else
        {
            delay = (game_runtime.frame_counter & 31) + 30;
            limit = delay + 90;
        }
        control->limit = (uint32)(limit);
        camera_shot_predict(context, delay << 5, control->output.pose.position);
        for (i = 0; i < 2; ++i)
        {
            offset = ((sint16)context->offset[i]);
            value = control->output.pose.position[i];
            control->output.pose.position[i] = (uint32)(value + (offset << 5));
        }
        offset = ((sint16)context->offset[2]);
        control->phase = (uint32)(1);
        value = control->output.pose.position[2];
        control->output.pose.position[2] = (uint32)(value + (offset << 5));
    }
    else if (mode != 1)
        return 0;
    camera_shot_face_origin(context, control->output.pose.position, &control->output.pose);
    count = control->ticks;
    limit = control->limit;
    ++count;
    control->ticks = (uint32)(count);
    if ((sint32)count < (sint32)limit)
        return 0;
    if ((sint32)duration > 0)
        return 1;
    control->phase = (uint32)(0);
    return 0;
}

uint32 camera_shot_trail(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration)
{
    uint32 mode, value, tick, flags, scale, distance, i, offset, current, count, limit;
    FUNCTION_MARKER(0x8007A8C4, "ACE3.BIN");
    mode = control->phase;
    if (mode == 0)
    {
        control->ticks = (uint32)(0);
        control->phase = (uint32)(1);
        control->limit = (uint32)((sint32)duration > 0 ? duration : 240);
        value = camera_shot_speed(context);
        control->distance = (uint32)(value);
        tick = game_runtime.frame_counter;
        control->flags = (uint32)(tick & 15);
        distance = control->distance;
        if ((tick & 1) != 0)
        {
            if ((sint32)distance < 0x4000)
                control->distance = (uint32)(0x4000);
        }
        else if ((sint32)distance <= 0x30FFF)
            control->distance = (uint32)(0x31000);
    }
    else if (mode != 1)
        return 0;
    flags = control->flags;
    scale = (flags & 1) != 0 ? 128 : 0xFFFFFF80;
    distance = control->distance;
    camera_shot_offset_forward(context, distance, scale, control->output.pose.position);
    for (i = 0; i < 3; ++i)
    {
        offset = ((sint16)context->offset[i]);
        current = control->output.pose.position[i];
        control->output.pose.position[i] = (uint32)(current - (offset << 6));
    }
    camera_shot_face_origin(context, control->output.pose.position, &control->output.pose);
    flags = control->flags;
    if ((flags & 8) != 0)
    {
        value = context->first->angles[2];
        if ((flags & 4) == 0)
            value = 0 - value;
        control->output.pose.angles[2] = (uint16)(value);
    }
    count = control->ticks + 1;
    control->ticks = (uint32)(count);
    if ((sint32)duration <= 0)
        return 0;
    limit = control->limit;
    return (sint32)count >= (sint32)limit;
}

void camera_shots_update(void)
{
    const NATIVE_POSE *source_pose;
    GAME_CAMERA_SHOT_CONTEXT context;
    const GAME_CAMERA_SHOT_DESC *descriptor;
    uint32 flags, active, selected, maximum, swapped, mode, value, a, b, c, d, e, f, count, index, result, duration;
    FUNCTION_MARKER(0x8007AFFC, "ACE3.BIN");
    flags = game_input.pressed;
    maximum = 450;
    selected = 0xFFFFFFFF;
    active = (flags & 0x209B80) != 0;
    value = game_input.held;
    if ((value & 0x209B00) != 0)
        maximum = 0;
    value = game_input.held;
    if ((value & 0x100) != 0)
        selected = 0;
    value = game_input.held;
    if ((value & 0x200) != 0)
        selected = 3;
    value = game_input.held;
    if ((value & 0x800) != 0)
        selected = 1;
    value = game_input.held;
    if ((value & 0x1000) != 0)
        selected = 2;
    value = game_input.held;
    if ((value & 0x8000) != 0)
        selected = 4;
    value = game_input.held;
    if ((value & 0x200000) != 0)
        selected = 5;
    value = ((sint16)game_runtime.camera_shots.target_mode);
    swapped = 0;
    if (value != 0)
    {
        value = game_runtime.frame_counter;
        if ((value & 256) != 0)
            swapped = 1;
    }
    value = ((sint16)game_runtime.camera_shots.target_mode);
    if (value == 2)
        swapped = 0;
    context.offset = game_runtime.camera_shots.offset;
    if (swapped == 0)
    {
        context.first = &game_runtime.camera_shots.subjects[0].pose;
        context.velocity = game_runtime.camera_shots.subjects[0].velocity;
        context.second = &game_runtime.camera_shots.subjects[1].pose;
    }
    else
    {
        context.first = &game_runtime.camera_shots.subjects[1].pose;
        context.velocity = game_runtime.camera_shots.subjects[1].velocity;
        context.second = &game_runtime.camera_shots.subjects[0].pose;
    }
    mode = game_runtime.camera_control.shot_phase;
    if (mode == 0)
    {
        camera_mark_dirty();
        a = game_runtime.camera_view.pose.position[0];
        b = game_runtime.camera_view.pose.position[1];
        c = game_runtime.camera_view.pose.position[2];
        d = game_runtime.camera_view.pose.angles[0];
        e = game_runtime.camera_view.pose.angles[1];
        f = game_runtime.camera_view.pose.angles[2];
        game_runtime.camera_shots.control.phase = (uint32)(0);
        game_runtime.camera_shots.previous.pose.position[0] = (uint32)(a);
        game_runtime.camera_shots.previous.pose.position[1] = (uint32)(b);
        game_runtime.camera_shots.previous.pose.position[2] = (uint32)(c);
        game_runtime.camera_shots.previous.pose.angles[0] = (uint16)(d);
        game_runtime.camera_shots.previous.pose.angles[1] = (uint16)(e);
        game_runtime.camera_shots.previous.pose.angles[2] = (uint16)(f);
        value = selected;
        if ((sint32)value < 0)
        {
            count = game_runtime.camera_control.shot_counter;
            value = camera_shot_order[count & 31];
            game_runtime.camera_control.shot_counter = (uint32)(count + 1);
        }
        game_runtime.camera_control.shot_index = (uint32)(value);
        flags = game_runtime.mission_control.flags;
        if ((flags & 16) == 0)
        {
            value = ((sint16)game_runtime.camera_shots.target_mode);
            if (value == 0)
            {
                index = game_runtime.camera_control.shot_index;
                descriptor = &camera_shot_main[(uint32)((sint32)index % 6)];
            }
            else
            {
                index = game_runtime.camera_control.shot_index;
                descriptor = &camera_shot_alternate[(uint32)((sint32)index % 2)];
            }
        }
        else
        {
            value = ((sint16)game_runtime.camera_shots.target_mode);
            if (value == 0)
            {
                index = game_runtime.camera_control.shot_index;
                descriptor = &camera_shot_mission[(uint32)((sint32)index % 3)];
            }
            else
                descriptor = &camera_shot_mission_target;
        }
        game_runtime.camera_shot_desc = descriptor;
        game_runtime.camera_control.shot_phase = (uint32)(1);
        mode = 1;
    }
    if (mode == 1)
    {
        descriptor = game_runtime.camera_shot_desc;
        value = (sint16)descriptor->duration;
        duration = maximum;
        if ((sint32)value < (sint32)duration)
            duration = value;
        result = descriptor->callback(&context, &game_runtime.camera_shots.control, duration);
        descriptor = game_runtime.camera_shot_desc;
        value = (sint16)descriptor->smooth;
        if (value == 1)
        {
            camera_shot_smooth(game_runtime.camera_shots.control.output.pose.position, game_runtime.camera_shots.control.output.pose.angles, game_runtime.camera_shots.previous.pose.position, game_runtime.camera_shots.previous.pose.angles);
            mission_camera_bind_local(&game_runtime.camera_shots.previous.pose);
        }
        else
            mission_camera_bind_local(&game_runtime.camera_shots.control.output.pose);
        if (result != 0 || active != 0)
        {
            game_runtime.camera_shots.target_changed = (uint16)(0);
            active = 0;
            game_runtime.camera_control.shot_phase = (uint32)(0);
        }
        else
        {
            value = ((sint16)game_runtime.camera_shots.target_changed);
            if (value != 0)
            {
                game_runtime.camera_shots.target_changed = (uint16)(0);
                active = 0;
                game_runtime.camera_control.shot_phase = (uint32)(0);
            }
        }
    }
    source_pose = mission_camera_pose();
    a = (source_pose->position[0]);
    b = (source_pose->position[1]);
    c = (source_pose->position[2]);
    game_runtime.camera_view.pose.position[0] = (uint32)(a);
    game_runtime.camera_view.pose.position[1] = (uint32)(b);
    game_runtime.camera_view.pose.position[2] = (uint32)(c);
    e = ((source_pose->angles[0] | ((uint32)source_pose->angles[1] << 16)));
    f = source_pose->angles[2];
    {
        uint32 camera_angle_word = (e);
        game_runtime.camera_view.pose.angles[0] = (uint16)camera_angle_word;
        game_runtime.camera_view.pose.angles[1] = (uint16)(camera_angle_word >> 16);
    };
    game_runtime.camera_view.pose.angles[2] = (uint16)f;
    return;
}

void camera_update(void)
{
    const NATIVE_POSE *source_pose;
    uint32 camera_actor_index_target;

    MISSION_ACTOR_TRAITS *actor_traits_target;
    uint32 actor_index_table;

    uint32 saved_position[3], saved_angles[3], i, value, magnitude, flags, a, b, c, mode, index, timer, raw, pitch;
    FUNCTION_MARKER(0x8007B438, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
        saved_position[i] = game_runtime.camera_view.pose.position[i];
    for (i = 0; i < 3; ++i)
        saved_angles[i] = game_runtime.camera_view.pose.angles[i];
    for (i = 0; i < 3; ++i)
    {
        value = ((sint16)game_runtime.camera_follow.steer[i]);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        if ((sint32)magnitude >= 49)
            break;
    }
    flags = game_runtime.camera_control.flags;
    if (i < 3)
        flags |= 0x200;
    else
        flags &= 0xFFFFFDFF;
    game_runtime.camera_control.flags = (uint32)(flags);
    flags = game_runtime.mission_control.flags;
    if ((flags & 64) != 0)
    {
        game_runtime.mission_control.flags = (uint32)(flags & 0xFFFFFFBF);
        a = game_input.held;
        b = game_input.repeat;
        game_input.held = (uint32)(a & 0xFFF0FFFF);
        c = game_input.pressed;
        game_input.repeat = (uint32)(b & 0xFFF0FFFF);
        game_input.pressed = (uint32)(c & 0xFFF0FFFF);
    }
    mode = game_runtime.camera_control.mode;
    switch (mode)
    {
        case 1:
            camera_update_mode_1();
            flags = game_input.pressed;
            if ((flags & 0x4000) != 0)
            {
                value = game_runtime.camera_control.flags;
                actor_index_table = 0;
                (void)mission_actor((actor_index_table + ((uint32)((sint32)((value & 63) * 4) >> 2))));
                camera_actor_index_target = (actor_index_table + ((uint32)((sint32)((value & 63) * 4) >> 2)));
                actor_traits_target = mission_actor_traits((actor_index_table + ((uint32)((sint32)((value & 63) * 4) >> 2))));
                camera_set_mode(2, actor_traits_target, camera_actor_index_target);
                game_runtime.options.camera_mode = (uint8)(1);
            }
            break;
        case 2:
            camera_update_mode_2();
            flags = game_input.pressed;
            if ((flags & 0x4000) != 0)
            {
                actor_index_table = 0;
                (void)mission_actor(actor_index_table);
                camera_actor_index_target = actor_index_table;
                actor_traits_target = mission_actor_traits(actor_index_table);
                camera_set_mode(1, actor_traits_target, camera_actor_index_target);
                game_runtime.options.camera_mode = (uint8)(0);
            }
            break;
        case 4:
            camera_update_mode_4();
            break;
        case 16:
            camera_update_mode_16();
            break;
        case 32:
            camera_update_mode_32();
            break;
        case 64:
            camera_update_mode_64();
            break;
        case 256:
            camera_update_mode_256();
            break;
        case 512:
            camera_shots_update();
            camera_update_shot_input();
            value = game_runtime.playback.demo_mode;
            if (value == 0)
            {
                value = game_runtime.camera_control.flags;
                game_runtime.camera_control.flags = (uint32)(value | 0x800);
            }
            break;
        case 1024:
            camera_update_mode_1();
            break;
        case 2048:
            value = game_runtime.camera_control.view_actor;
            camera_update_mode_2048(value);
            flags = game_input.pressed;
            value = game_runtime.flags;
            if ((value & 0x20000000) == 0)
            {
                timer = game_runtime.camera_control.mode_ticks + 1;
                game_runtime.camera_control.mode_ticks = (uint32)(timer);
                if ((sint32)timer >= 181 || (flags & 0x60F800) != 0)
                {
                    value = game_runtime.options.camera_mode;
                    actor_index_table = 0;
                    (void)mission_actor(actor_index_table);
                    camera_actor_index_target = actor_index_table;
                    actor_traits_target = mission_actor_traits(actor_index_table);
                    camera_set_mode(value != 0 ? 2 : 1, actor_traits_target, camera_actor_index_target);
                    mission_apply_hud_visibility(1);
                }
            }
            break;
        case 0x40000000:
            value = pad_state[0].buttons;
            camera_debug_move(&game_runtime.camera_view.pose, value);
            flags = game_input.pressed;
            if ((flags & 0x4000) != 0)
            {
                value = game_runtime.camera_control.flags;
                index = (value & 63) + 1;
                if (index < 64)
                {
                    actor_index_table = (((sint32)(index * 4) >> 2));
                    do
                    {
                        (void)mission_actor(actor_index_table);
                        camera_actor_index_target = actor_index_table;
                        actor_traits_target = mission_actor_traits(actor_index_table);
                        value = actor_traits_target->flags;
                        ++index;
                        if ((value & 0x4000) != 0)
                        {
                            camera_set_mode(0x20000000, actor_traits_target, camera_actor_index_target);
                            break;
                        }
                        actor_index_table += 1;
                    } while (index < 64);
                }
            }
            break;
        case 0x20000000:
            value = pad_state[0].buttons;
            if ((value & 0x8000) != 0)
            {
                value = pad_state[0].buttons;
                camera_update_orbit_input(value);
            }
            else
                camera_update_mode_2();
            flags = game_input.pressed;
            if ((flags & 0x4000) != 0)
            {
                value = game_runtime.camera_control.flags;
                index = (value & 63) + 1;
                if (index < 64)
                {
                    actor_index_table = (((sint32)(index * 4) >> 2));
                    do
                    {
                        (void)mission_actor(actor_index_table);
                        camera_actor_index_target = actor_index_table;
                        actor_traits_target = mission_actor_traits(actor_index_table);
                        value = actor_traits_target->flags;
                        if ((value & 0x4000) != 0)
                        {
                            value = game_runtime.camera_control.mode;
                            camera_set_mode(value, actor_traits_target, camera_actor_index_target);
                            break;
                        }
                        ++index;
                        actor_index_table += 1;
                    } while (index < 64);
                }
                if (index == 64)
                {
                    actor_index_table = 0;
                    (void)mission_actor(actor_index_table);
                    camera_actor_index_target = actor_index_table;
                    actor_traits_target = mission_actor_traits(actor_index_table);
                    camera_set_mode(1, actor_traits_target, camera_actor_index_target);
                    mission_apply_hud_visibility(1);
                }
            }
            break;
        default:
            break;
    }
    flags = game_runtime.camera_control.flags;
    if ((flags & 0x100) != 0)
    {
        source_pose = mission_camera_pose();
        for (i = 0; i < 3; ++i)
        {
            value = (source_pose->position[i]);
            game_runtime.camera_view.pose.position[i] = (uint32)(value);
        }
        for (i = 0; i < 3; ++i)
        {
            a = game_runtime.camera_view.pose.position[i];
            b = game_runtime.camera_follow.offset[i];
            game_runtime.camera_view.pose.position[i] = (uint32)(a + b);
        }
        for (i = 0; i < 3; ++i)
        {
            value = (source_pose->angles[i]);
            game_runtime.camera_view.pose.angles[i] = (uint16)(value);
        }
        for (i = 0; i < 2; ++i)
        {
            a = game_runtime.camera_view.pose.angles[i];
            b = game_runtime.camera_follow.angles[i];
            game_runtime.camera_view.pose.angles[i] = (uint16)(a + b);
        }
        a = game_runtime.camera_view.pose.angles[2];
        b = game_runtime.camera_follow.angles[2];
        pitch = ((sint16)game_runtime.camera_view.pose.angles[0]);
        raw = game_runtime.camera_view.pose.angles[0];
        game_runtime.camera_view.pose.angles[2] = (uint16)(a + b);
        if ((sint32)pitch >= 4097)
        {
            game_runtime.camera_view.pose.angles[0] = (uint16)(8192 - raw);
            a = game_runtime.camera_view.pose.angles[1];
            b = game_runtime.camera_view.pose.angles[2];
            game_runtime.camera_view.pose.angles[1] = (uint16)(a + 8192);
            game_runtime.camera_view.pose.angles[2] = (uint16)(b + 8192);
        }
        pitch = ((sint16)game_runtime.camera_view.pose.angles[0]);
        raw = game_runtime.camera_view.pose.angles[0];
        if ((sint32)pitch < -4096)
        {
            game_runtime.camera_view.pose.angles[0] = (uint16)(0 - 8192 - raw);
            a = game_runtime.camera_view.pose.angles[1];
            b = game_runtime.camera_view.pose.angles[2];
            game_runtime.camera_view.pose.angles[1] = (uint16)(a + 8192);
            game_runtime.camera_view.pose.angles[2] = (uint16)(b + 8192);
        }
        for (i = 0; i < 2; ++i)
        {
            value = ((sint16)game_runtime.camera_view.pose.angles[i + 1]);
            raw = game_runtime.camera_view.pose.angles[i + 1];
            if ((sint32)value >= 8192)
                game_runtime.camera_view.pose.angles[i + 1] = (uint16)(raw - 16384);
            value = ((sint16)game_runtime.camera_view.pose.angles[i + 1]);
            raw = game_runtime.camera_view.pose.angles[i + 1];
            if ((sint32)value < -8192)
                game_runtime.camera_view.pose.angles[i + 1] = (uint16)(raw + 16384);
        }
    }
    flags = game_runtime.camera_control.flags;
    if ((flags & 0x800) != 0 && (flags & 0x1000) == 0)
    {
        game_runtime.camera_control.flags = (uint32)(flags & 0xFFFFF7FF);
        flags = game_runtime.mission_control.flags;
        if ((flags & 16) == 0)
        {
            value = camera_ground_offset(game_runtime.camera_view.pose.position);
            a = game_runtime.camera_view.pose.position[1];
            game_runtime.camera_view.pose.position[1] = (uint32)(a + value);
        }
        else
            camera_clamp_position(game_runtime.camera_view.min, game_runtime.camera_view.max, game_runtime.camera_view.pose.position);
    }
    flags = game_runtime.camera_control.flags;
    if ((flags & 0x400) == 0)
    {
        for (i = 0; i < 3; ++i)
        {
            value = game_runtime.camera_view.pose.position[i];
            game_runtime.camera_delta.position[i] = (uint32)(value - saved_position[i]);
        }
        for (i = 0; i < 3; ++i)
        {
            value = game_runtime.camera_view.pose.angles[i];
            game_runtime.camera_delta.angles[i] = (uint16)(value - saved_angles[i]);
        }
    }
    else
    {
        game_runtime.camera_control.flags = (uint32)(flags & 0xFFFFFBFF);
        game_runtime.camera_delta.position[0] = (uint32)(0);
        game_runtime.camera_delta.position[1] = (uint32)(0);
        game_runtime.camera_delta.position[2] = (uint32)(0);
        game_runtime.camera_delta.angles[0] = (uint16)(0);
        game_runtime.camera_delta.angles[1] = (uint16)(0);
        game_runtime.camera_delta.angles[2] = (uint16)(0);
    }
    value = game_runtime.flags & 1;
    if (value != 0)
    {
        (void)(camera_draw_debug());
        return;
    }
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_vector_angles(const uint32 *vector, uint16 *angles)
{
    uint32 raw_x, raw_y, raw_z, x, y, z, ax, az, sum, length, shift, pitch, yaw, result;
    FUNCTION_MARKER(0x80031CD8, "ACE3.BIN");
    {
        raw_x = vector[0];
        raw_y = vector[1];
        raw_z = vector[2];
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
        length = game_vector_length_gte(ax, 0, az) << shift;
    }
    pitch = game_vector_atan2(0 - y, length);
    yaw = game_vector_atan2(x, z);
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
    angles[0] = (pitch);
    angles[1] = (yaw);
    angles[2] = 0;
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_bind_player(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x80038AF4, "ACE3.BIN");
    flags = game_runtime.camera_control.flags;
    game_runtime.camera_control.flags = (uint32)(flags | 0x100);
    game_runtime.camera_follow.offset[0] = (uint32)(0);
    game_runtime.camera_follow.offset[1] = (uint32)(0);
    game_runtime.camera_follow.offset[2] = (uint32)(0);
    game_runtime.camera_follow.angles[0] = (uint16)(0);
    game_runtime.camera_follow.angles[1] = (uint16)(0);
    game_runtime.camera_follow.angles[2] = (uint16)(0);
    mission_camera_bind_actor(0);
}

void camera_draw_debug(void)
{
    uint32 packet, value, result, i;
    FUNCTION_MARKER(0x80077460, "ACE3.BIN");
    packet = debug_text_print(230, 180, (const uint8 *)"CAM") + 12;
    value = game_runtime.camera_control.flags;
    debug_text_hex(packet, 180, value, 8);
    value = game_runtime.camera_control.mode;
    debug_text_hex(packet, 186, value, 8);
    for (i = 0; i < 3; ++i)
    {
        value = game_runtime.camera_view.pose.position[i];
        debug_text_decimal(packet, 192 + i * 6, (sint32)value >> 12, 0x207);
    }
    for (i = 0; i < 3; ++i)
    {
        value = ((sint16)game_runtime.camera_view.pose.angles[i]);
        result = debug_text_decimal(packet, 210 + i * 6, value, 0x207);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_update_orbit_input(uint32 input)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix;
    uint32 value, raw, pitch, yaw, roll, target_pitch, target_yaw, target_roll, speed, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x8007754C, "ACE3.BIN");
    if ((input & 0x40) != 0)
    {
        value = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
        raw = game_runtime.camera_follow.orbit_angles[0];
        if ((sint32)value < 4096)
            game_runtime.camera_follow.orbit_angles[0] = (uint16)(raw + 128);
    }
    if ((input & 0x10) != 0)
    {
        value = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
        raw = game_runtime.camera_follow.orbit_angles[0];
        if ((sint32)value >= -4095)
            game_runtime.camera_follow.orbit_angles[0] = (uint16)(raw - 128);
    }
    if ((input & 0x80) != 0)
    {
        raw = game_runtime.camera_follow.orbit_angles[1];
        value = raw - 128;
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(value);
        if (((sint32)(value << 16) >> 16) < -8192)
            game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw + 0x3F80);
    }
    if ((input & 0x20) != 0)
    {
        raw = game_runtime.camera_follow.orbit_angles[1];
        value = raw + 128;
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(value);
        if (((sint32)(value << 16) >> 16) >= 8193)
            game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw - 0x3F80);
    }
    if ((input & 0x400) != 0)
    {
        speed = game_runtime.camera_follow.orbit_offset[2];
        if ((sint32)speed <= 0x31FFFF)
            game_runtime.camera_follow.orbit_offset[2] = (uint32)(speed + 0xA000);
    }
    if ((input & 0x800) != 0)
    {
        speed = game_runtime.camera_follow.orbit_offset[2];
        if ((sint32)speed > 0)
            game_runtime.camera_follow.orbit_offset[2] = (uint32)(speed - 0xA000);
    }
    source_pose = mission_camera_pose();
    pitch = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    yaw = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    roll = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    target_pitch = ((sint16)source_pose->angles[0]);
    target_yaw = ((sint16)source_pose->angles[1]);
    target_roll = ((sint16)source_pose->angles[2]);
    speed = game_runtime.camera_follow.orbit_offset[2];
    z = (sint32)(0 - speed) >> 7;
    pitch -= target_pitch;
    yaw -= target_yaw;
    roll -= target_roll;
    rotation_build(&matrix, game_runtime.camera_follow.orbit_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(rx << 7);
    game_runtime.camera_follow.offset[1] = (uint32)(ry << 7);
    game_runtime.camera_follow.angles[0] = (uint16)(pitch);
    game_runtime.camera_follow.angles[1] = (uint16)(yaw);
    game_runtime.camera_follow.angles[2] = (uint16)(roll);
    value = rz << 7;
    game_runtime.camera_follow.offset[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_initialize(uint32 projection, uint32 x, uint32 y, const uint16 angles[3])
{
    uint32 camera_actor_index_descriptor;

    MISSION_ACTOR_TRAITS *actor_traits_descriptor;
    uint32 actor_index_table;

    uint32 value;
    uint32 mode;
    uint32 index;

    FUNCTION_MARKER(0x80077880, "ACE3.BIN");
    game_runtime.camera_proj.origin[0] = (uint16)(-160);
    game_runtime.camera_proj.origin[1] = (uint16)(-120);
    game_runtime.camera_control.flags = (uint32)(0);
    game_runtime.camera_proj.distance = (uint16)(projection);
    game_runtime.camera_control.excluded_actor = (uint32)(0);
    game_runtime.camera_control.follow_actor = (uint32)(0);
    game_runtime.camera_proj.size[0] = (uint16)(320);
    game_runtime.camera_proj.size[1] = (uint16)(240);
    game_runtime.camera_proj.near_plane = (uint16)(x);
    game_runtime.camera_proj.far_plane = (uint16)(y);
    game_runtime.camera_shots.offset[0] = (uint16)((angles[0]) << 6);
    game_runtime.camera_shots.offset[1] = (uint16)((angles[1]) << 6);
    value = angles[2];
    game_runtime.camera_view.pose.position[1] = (uint32)(0xFFB50000);
    game_runtime.camera_view.pose.position[0] = (uint32)(0);
    game_runtime.camera_view.pose.position[2] = (uint32)(0);
    game_runtime.camera_view.pose.angles[0] = (uint16)(0);
    game_runtime.camera_view.pose.angles[1] = (uint16)(0);
    game_runtime.camera_view.pose.angles[2] = (uint16)(0);
    game_runtime.camera_follow.steer[0] = (uint16)(0);
    game_runtime.camera_follow.steer[1] = (uint16)(0);
    game_runtime.camera_follow.steer[2] = (uint16)(0);
    game_runtime.camera_control.look_target = (uint32)(0xFFFFFFFF);
    game_runtime.camera_control.view_actor = (uint32)(0xFFFFFFFF);
    game_runtime.camera_control.shot_index = (uint32)(0);
    game_runtime.camera_control.shot_counter = (uint32)(0);
    game_runtime.camera_shots.subjects[0].pose.angles[0] = (uint16)(0);
    game_runtime.camera_shots.subjects[0].pose.angles[1] = (uint16)(0);
    game_runtime.camera_shots.subjects[0].pose.angles[2] = (uint16)(0);
    game_runtime.camera_shots.offset[2] = (uint16)(value << 6);
    game_runtime.camera_shots.subjects[0].pose.position[0] = (uint32)(0);
    game_runtime.camera_shots.subjects[0].pose.position[1] = (uint32)(0);
    game_runtime.camera_shots.subjects[0].pose.position[2] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].pose.angles[0] = (uint16)(0);
    game_runtime.camera_shots.subjects[1].pose.angles[1] = (uint16)(0);
    game_runtime.camera_shots.subjects[1].pose.angles[2] = (uint16)(0);
    game_runtime.camera_shots.subjects[1].pose.position[0] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].pose.position[1] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].pose.position[2] = (uint32)(0);
    game_runtime.camera_shots.subjects[0].velocity[0] = (uint32)(0);
    game_runtime.camera_shots.subjects[0].velocity[1] = (uint32)(0);
    game_runtime.camera_shots.subjects[0].velocity[2] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].velocity[0] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].velocity[1] = (uint32)(0);
    game_runtime.camera_shots.subjects[1].velocity[2] = (uint32)(0);
    game_runtime.camera_shots.target_mode = (uint16)(0);
    game_runtime.camera_shots.target_changed = (uint16)(0);
    mode = game_runtime.options.camera_mode ? 2 : 1;
    game_runtime.camera_control.mode = (uint32)(mode);
    index = game_runtime.camera_control.follow_actor;
    actor_index_table = 0;
    mode = game_runtime.camera_control.mode;
    (void)mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    camera_actor_index_descriptor = (actor_index_table + ((uint32)((sint32)((index << 2)) >> 2)));
    actor_traits_descriptor = mission_actor_traits((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
    {
        (void)(camera_set_mode(mode, actor_traits_descriptor, camera_actor_index_descriptor));
        return;
    }
}

void camera_update_mode_1(void)
{
    const NATIVE_POSE *origin;
    const NATIVE_POSE *source_pose;
    uint16 angles[3];
    uint16 *field;
    uint32 flags, index, target[3], steps[3], value, other, difference, axis, result;
    FUNCTION_MARKER(0x80077A30, "ACE3.BIN");
    flags = game_input.held;
    index = 0xFFFFFFFF;
    if (flags & 0x2000)
        index = game_runtime.camera_control.look_target;
    flags = game_runtime.camera_control.flags;
    origin = (flags & 0x100) != 0 ? mission_camera_pose() : &game_runtime.camera_view.pose;
    mission_actor_camera_target_angles(index, angles, origin);
    target[0] = (sint16)angles[0];
    target[1] = (sint16)angles[1];
    flags = game_input.axis_mask;
    target[2] = (sint16)angles[2];
    if (flags & 16)
        target[0] -= (uint32)game_input.axis[4];
    if (flags & 32)
        target[1] += (uint32)game_input.axis[5];
    flags = game_input.held;
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) < -0x2000)
        game_runtime.camera_follow.steer[1] = (uint16)(other + 0x4000);
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) >= 0x2001)
        game_runtime.camera_follow.steer[1] = (uint16)(other - 0x4000);
    flags = game_input.pressed & 0x100000;
    if (flags == 0)
        flags = game_input.released & 0x100000;
    if (flags)
    {
        game_runtime.camera_follow.steer[0] = (uint16)(target[0]);
        game_runtime.camera_follow.steer[1] = (uint16)(target[1]);
        game_runtime.camera_follow.steer[2] = (uint16)(target[2]);
        camera_mark_dirty();
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = ((sint16)game_runtime.camera_follow.steer[axis]);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = (uint32)((sint32)difference >> 2) + 2;
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)value < (sint32)target[axis])
                (*field) = (other + steps[axis]);
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)target[axis] < (sint32)value)
                (*field) = (other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                (*field) = (target[axis]);
        }
    }
    flags = game_runtime.camera_control.flags;
    if (flags & 0x100)
    {
        source_pose = mission_camera_pose();
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, (source_pose->angles));
        source_pose = mission_camera_pose();
        value = (sint16)angles[0];
        other = ((sint16)source_pose->angles[0]);
        target[0] = value - other;
        value = ((sint16)source_pose->angles[1]);
        result = ((sint16)source_pose->angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, game_runtime.camera_view.pose.angles);
        value = ((sint16)game_runtime.camera_view.pose.angles[0]);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = ((sint16)game_runtime.camera_view.pose.angles[1]);
        result = ((sint16)game_runtime.camera_view.pose.angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    game_runtime.camera_follow.angles[0] = (uint16)(target[0]);
    game_runtime.camera_follow.angles[1] = (uint16)(target[1]);
    game_runtime.camera_follow.angles[2] = (uint16)(target[2]);
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_update_mode_2(void)
{
    const NATIVE_POSE *origin;
    const NATIVE_POSE *source_pose;
    MATRIX first = {0}, second = {0};
    uint32 a, b, c, d, packed, depth, column0[3], column1[3], vector[3];
    uint16 angles[3];
    uint16 *field;
    uint32 flags, index, target[3], steps[3], value, other, difference, axis, result;
    FUNCTION_MARKER(0x80077DD0, "ACE3.BIN");
    flags = game_input.held;
    index = 0xFFFFFFFF;
    if (flags & 0x2000)
        index = game_runtime.camera_control.look_target;
    flags = game_runtime.camera_control.flags;
    origin = (flags & 0x100) != 0 ? mission_camera_pose() : &game_runtime.camera_view.pose;
    mission_actor_camera_target_angles(index, angles, origin);
    target[0] = (sint16)angles[0];
    target[1] = (sint16)angles[1];
    flags = game_input.axis_mask;
    target[2] = (sint16)angles[2];
    if (flags & 16)
        target[0] -= (uint32)game_input.axis[4];
    if (flags & 32)
        target[1] += (uint32)game_input.axis[5];
    flags = game_input.held;
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) < -0x2000)
        game_runtime.camera_follow.steer[1] = (uint16)(other + 0x4000);
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) >= 0x2001)
        game_runtime.camera_follow.steer[1] = (uint16)(other - 0x4000);
    flags = game_input.pressed & 0x100000;
    if (flags == 0)
        flags = game_input.released & 0x100000;
    if (flags)
    {
        game_runtime.camera_follow.steer[0] = (uint16)(target[0]);
        game_runtime.camera_follow.steer[1] = (uint16)(target[1]);
        game_runtime.camera_follow.steer[2] = (uint16)(target[2]);
        camera_mark_dirty();
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = ((sint16)game_runtime.camera_follow.steer[axis]);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = ((sint32)(difference + 2) >> 2);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)value < (sint32)target[axis])
                (*field) = (other + steps[axis]);
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)target[axis] < (sint32)value)
                (*field) = (other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                (*field) = (target[axis]);
        }
    }
    source_pose = mission_camera_pose();
    rotation_build(&first, (source_pose->angles));
    rotation_build(&second, game_runtime.camera_follow.steer);
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    packed = ((uint16)second.m[0][0]) | (((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16)) & 0xFFFF0000);
    depth = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][1]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) << 16);
    depth = (sint16)((uint16)second.m[2][1]);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][2]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) & 0xFFFF0000);
    depth = ((uint16)second.m[2][2]);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    {
        uint32 matrix_word = (column0[0] & 0xFFFF) | (column1[0] << 16);
        first.m[0][0] = (sint16)matrix_word;
        first.m[0][1] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column0[2] & 0xFFFF) | (column1[2] << 16);
        first.m[2][0] = (sint16)matrix_word;
        first.m[2][1] = (sint16)(matrix_word >> 16);
    }
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    {
        uint32 matrix_word = (a & 0xFFFF) | (column0[1] << 16);
        first.m[0][2] = (sint16)matrix_word;
        first.m[1][0] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column1[1] & 0xFFFF) | (b << 16);
        first.m[1][1] = (sint16)matrix_word;
        first.m[1][2] = (sint16)(matrix_word >> 16);
    }
    first.m[2][2] = (sint16)(xport_gte_read_data(11));
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    a = ((uint32)game_runtime.camera_shots.offset[0] | ((uint32)game_runtime.camera_shots.offset[1] << 16));
    xport_gte_write_data(0, a);
    b = (uint32)game_runtime.camera_shots.offset[2];
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    vector[0] = xport_gte_read_data(25);
    vector[1] = xport_gte_read_data(26);
    vector[2] = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(vector[0] << 6);
    flags = game_runtime.camera_control.flags;
    game_runtime.camera_follow.offset[1] = (uint32)(vector[1] << 6);
    game_runtime.camera_follow.offset[2] = (uint32)(vector[2] << 6);
    if (flags & 0x100)
    {
        source_pose = mission_camera_pose();
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, (source_pose->angles));
        source_pose = mission_camera_pose();
        value = (sint16)angles[0];
        other = ((sint16)source_pose->angles[0]);
        target[0] = value - other;
        value = ((sint16)source_pose->angles[1]);
        result = ((sint16)source_pose->angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, game_runtime.camera_view.pose.angles);
        value = ((sint16)game_runtime.camera_view.pose.angles[0]);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = ((sint16)game_runtime.camera_view.pose.angles[1]);
        result = ((sint16)game_runtime.camera_view.pose.angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    game_runtime.camera_follow.angles[0] = (uint16)(target[0]);
    game_runtime.camera_follow.angles[1] = (uint16)(target[1]);
    game_runtime.camera_follow.angles[2] = (uint16)(target[2]);
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_update_mode_2048(uint32 selected)
{
    const NATIVE_POSE *origin;
    const NATIVE_POSE *source_pose;
    MATRIX first = {0}, second = {0};
    uint32 a, b, c, d, packed, depth, column0[3], column1[3], vector[3];
    uint16 angles[3];
    uint16 *field;
    uint32 flags, index, target[3], steps[3], value, other, difference, axis, result;
    FUNCTION_MARKER(0x80078234, "ACE3.BIN");
    index = selected;
    flags = game_runtime.camera_control.flags;
    origin = (flags & 0x100) != 0 ? mission_camera_pose() : &game_runtime.camera_view.pose;
    mission_actor_camera_target_angles(index, angles, origin);
    target[0] = (sint16)angles[0];
    target[1] = (sint16)angles[1];
    flags = game_input.axis_mask;
    target[2] = (sint16)angles[2];
    if (flags & 16)
        target[0] -= (uint32)game_input.axis[4];
    if (flags & 32)
        target[1] += (uint32)game_input.axis[5];
    flags = game_input.held;
    if (flags & 0x100000)
        target[1] += 0x2000;
    if (flags & 0x10000)
        target[0] += 0x1000;
    if (flags & 0x20000)
        target[0] -= 0x1000;
    if (flags & 0x40000)
        target[1] -= 0x1000;
    if (flags & 0x80000)
        target[1] += 0x1000;
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) < -0x2000)
        game_runtime.camera_follow.steer[1] = (uint16)(other + 0x4000);
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    other = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) >= 0x2001)
        game_runtime.camera_follow.steer[1] = (uint16)(other - 0x4000);
    flags = game_input.pressed & 0x100000;
    if (flags == 0)
        flags = game_input.released & 0x100000;
    if (flags)
    {
        game_runtime.camera_follow.steer[0] = (uint16)(target[0]);
        game_runtime.camera_follow.steer[1] = (uint16)(target[1]);
        game_runtime.camera_follow.steer[2] = (uint16)(target[2]);
        camera_mark_dirty();
    }
    else
    {
        for (axis = 0; axis < 3; ++axis)
        {
            value = ((sint16)game_runtime.camera_follow.steer[axis]);
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            steps[axis] = ((sint32)(difference + 2) >> 2);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)value < (sint32)target[axis])
                (*field) = (other + steps[axis]);
            value = ((sint16)(*field));
            other = (*field);
            if ((sint32)target[axis] < (sint32)value)
                (*field) = (other - steps[axis]);
        }
        for (axis = 0; axis < 3; ++axis)
        {
            field = &game_runtime.camera_follow.steer[axis];
            value = ((sint16)(*field));
            difference = value - target[axis];
            if ((sint32)difference < 0)
                difference = target[axis] - value;
            if ((sint32)difference < (sint32)steps[axis])
                (*field) = (target[axis]);
        }
    }
    source_pose = mission_camera_pose();
    rotation_build(&first, (source_pose->angles));
    rotation_build(&second, game_runtime.camera_follow.steer);
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    packed = ((uint16)second.m[0][0]) | (((uint32)(uint16)second.m[0][2] | ((uint32)(uint16)second.m[1][0] << 16)) & 0xFFFF0000);
    depth = ((uint32)(uint16)second.m[2][0] | ((uint32)(uint16)second.m[2][1] << 16));
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][1]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) << 16);
    depth = (sint16)((uint16)second.m[2][1]);
    column0[0] = xport_gte_read_data(9);
    column0[1] = xport_gte_read_data(10);
    column0[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    packed = ((uint16)second.m[0][2]) | (((uint32)(uint16)second.m[1][1] | ((uint32)(uint16)second.m[1][2] << 16)) & 0xFFFF0000);
    depth = ((uint16)second.m[2][2]);
    column1[0] = xport_gte_read_data(9);
    column1[1] = xport_gte_read_data(10);
    column1[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, packed);
    xport_gte_write_data(1, depth);
    xport_gte_execute(0x486012);
    {
        uint32 matrix_word = (column0[0] & 0xFFFF) | (column1[0] << 16);
        first.m[0][0] = (sint16)matrix_word;
        first.m[0][1] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column0[2] & 0xFFFF) | (column1[2] << 16);
        first.m[2][0] = (sint16)matrix_word;
        first.m[2][1] = (sint16)(matrix_word >> 16);
    }
    a = xport_gte_read_data(9);
    b = xport_gte_read_data(10);
    {
        uint32 matrix_word = (a & 0xFFFF) | (column0[1] << 16);
        first.m[0][2] = (sint16)matrix_word;
        first.m[1][0] = (sint16)(matrix_word >> 16);
    }
    {
        uint32 matrix_word = (column1[1] & 0xFFFF) | (b << 16);
        first.m[1][1] = (sint16)matrix_word;
        first.m[1][2] = (sint16)(matrix_word >> 16);
    }
    first.m[2][2] = (sint16)(xport_gte_read_data(11));
    a = ((uint32)(uint16)first.m[0][0] | ((uint32)(uint16)first.m[0][1] << 16));
    b = ((uint32)(uint16)first.m[0][2] | ((uint32)(uint16)first.m[1][0] << 16));
    c = ((uint32)(uint16)first.m[1][1] | ((uint32)(uint16)first.m[1][2] << 16));
    d = ((uint32)(uint16)first.m[2][0] | ((uint32)(uint16)first.m[2][1] << 16));
    SetRotMatrix(&first);
    a = ((uint32)game_runtime.camera_shots.offset[0] | ((uint32)game_runtime.camera_shots.offset[1] << 16));
    xport_gte_write_data(0, a);
    b = (uint32)game_runtime.camera_shots.offset[2];
    xport_gte_write_data(1, b);
    xport_gte_execute(0x486012);
    vector[0] = xport_gte_read_data(25);
    vector[1] = xport_gte_read_data(26);
    vector[2] = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(vector[0] << 6);
    flags = game_runtime.camera_control.flags;
    game_runtime.camera_follow.offset[1] = (uint32)(vector[1] << 6);
    game_runtime.camera_follow.offset[2] = (uint32)(vector[2] << 6);
    if (flags & 0x100)
    {
        source_pose = mission_camera_pose();
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, (source_pose->angles));
        source_pose = mission_camera_pose();
        value = (sint16)angles[0];
        other = ((sint16)source_pose->angles[0]);
        target[0] = value - other;
        value = ((sint16)source_pose->angles[1]);
        result = ((sint16)source_pose->angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    else
    {
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, game_runtime.camera_view.pose.angles);
        value = ((sint16)game_runtime.camera_view.pose.angles[0]);
        target[0] = (uint32)(sint32)(sint16)angles[0] - value;
        value = ((sint16)game_runtime.camera_view.pose.angles[1]);
        result = ((sint16)game_runtime.camera_view.pose.angles[2]);
        target[1] = (uint32)(sint32)(sint16)angles[1] - value;
    }
    target[2] = (uint32)(sint32)(sint16)angles[2] - result;
    game_runtime.camera_follow.angles[0] = (uint16)(target[0]);
    game_runtime.camera_follow.angles[1] = (uint16)(target[1]);
    game_runtime.camera_follow.angles[2] = (uint16)(target[2]);
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_update_shot_input(void)
{
    uint32 flags, value, raw, delta, i, fast, target[3], step[3];
    FUNCTION_MARKER(0x8007867C, "ACE3.BIN");
    target[0] = 0;
    target[1] = 0;
    target[2] = 0;
    flags = game_input.axis_mask;
    if ((flags & 0x10) != 0)
        target[0] = 0 - (uint32)game_input.axis[4];
    if ((flags & 0x20) != 0)
        target[1] = (uint32)game_input.axis[5];
    flags = game_input.held;
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
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    raw = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) < -8192)
        game_runtime.camera_follow.steer[1] = (uint16)(raw + 0x4000);
    value = ((sint16)game_runtime.camera_follow.steer[1]);
    raw = game_runtime.camera_follow.steer[1];
    if ((sint32)(value - target[1]) > 8192)
        game_runtime.camera_follow.steer[1] = (uint16)(raw - 0x4000);
    flags = game_input.pressed;
    fast = flags & 0x100000;
    if (fast == 0)
    {
        flags = game_input.released;
        fast = flags & 0x100000;
    }
    if (fast != 0)
    {
        game_runtime.camera_follow.steer[0] = (uint16)(target[0]);
        game_runtime.camera_follow.steer[1] = (uint16)(target[1]);
        game_runtime.camera_follow.steer[2] = (uint16)(0);
        camera_mark_dirty();
    }
    else
    {
        for (i = 0; i < 3; ++i)
        {
            value = ((sint16)game_runtime.camera_follow.steer[i]);
            delta = value - target[i];
            if ((sint32)delta < 0)
                delta = target[i] - value;
            step[i] = (sint32)(delta + 2) >> 2;
        }
        for (i = 0; i < 3; ++i)
        {
            value = ((sint16)game_runtime.camera_follow.steer[i]);
            raw = game_runtime.camera_follow.steer[i];
            if ((sint32)value < (sint32)target[i])
                game_runtime.camera_follow.steer[i] = (uint16)(raw + step[i]);
            value = ((sint16)game_runtime.camera_follow.steer[i]);
            raw = game_runtime.camera_follow.steer[i];
            if ((sint32)target[i] < (sint32)value)
                game_runtime.camera_follow.steer[i] = (uint16)(raw - step[i]);
        }
        for (i = 0; i < 3; ++i)
        {
            value = ((sint16)game_runtime.camera_follow.steer[i]);
            delta = value - target[i];
            if ((sint32)delta < 0)
                delta = target[i] - value;
            if ((sint32)delta < (sint32)step[i])
                game_runtime.camera_follow.steer[i] = (uint16)(target[i]);
        }
    }
    {
        (void)(game_angles_compose_delta(game_runtime.camera_view.pose.angles, game_runtime.camera_follow.steer, game_runtime.camera_view.pose.angles));
        return;
    }
}

void camera_update_mode_4(void)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix = {0}, adjustment = {0};
    uint16 angles[3];
    uint32 ticks, sine, value, x, y, z, a, b, c, d, e, rx, ry, rz, flags, pitch, yaw, roll, dx, dy, dz;
    FUNCTION_MARKER(0x8007895C, "ACE3.BIN");
    source_pose = mission_camera_pose();
    ticks = game_runtime.mission_control.ticks;
    game_runtime.camera_follow.steer[2] = (uint16)(0);
    sine = (uint16)game_wave_at(((ticks << 7) & 0x1F80));
    sine = (sint32)(sine << 16) >> 18;
    value = 4096 - sine;
    game_runtime.camera_follow.steer[0] = (uint16)(0 - value);
    game_runtime.camera_follow.steer[1] = (uint16)(value << 1);
    rotation_build(&matrix, (source_pose->angles));
    rotation_build(&adjustment, game_runtime.camera_follow.steer);
    MulMatrix0(&matrix, &adjustment, &matrix);
    x = ((sint16)game_runtime.camera_shots.offset[0]);
    y = ((sint16)game_runtime.camera_shots.offset[1]);
    z = ((sint16)game_runtime.camera_shots.offset[2]);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, (x & 0xFFFF) | (y << 16));
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(rx << 6);
    flags = game_runtime.camera_control.flags;
    game_runtime.camera_follow.offset[1] = (uint32)(ry << 6);
    game_runtime.camera_follow.offset[2] = (uint32)(rz << 6);
    if ((flags & 0x100) != 0)
    {
        source_pose = mission_camera_pose();
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, (source_pose->angles));
        source_pose = mission_camera_pose();
        pitch = ((sint16)source_pose->angles[0]);
        yaw = ((sint16)source_pose->angles[1]);
        roll = ((sint16)source_pose->angles[2]);
    }
    else
    {
        game_angles_compose_delta(angles, game_runtime.camera_follow.steer, game_runtime.camera_view.pose.angles);
        pitch = ((sint16)game_runtime.camera_view.pose.angles[0]);
        yaw = ((sint16)game_runtime.camera_view.pose.angles[1]);
        roll = ((sint16)game_runtime.camera_view.pose.angles[2]);
    }
    dx = (uint32)(sint32)(sint16)angles[0] - pitch;
    dy = (uint32)(sint32)(sint16)angles[1] - yaw;
    dz = (uint32)(sint32)(sint16)angles[2] - roll;
    game_runtime.camera_follow.angles[0] = (uint16)(dx);
    game_runtime.camera_follow.angles[1] = (uint16)(dy);
    game_runtime.camera_follow.angles[2] = (uint16)(dz);
    {
        (void)(uint32)(roll);
        return;
    }
}

void camera_update_mode_16(void)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix;
    uint32 flags, count, factor, pitch, yaw, roll, speed, value, raw, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078B28, "ACE3.BIN");
    flags = game_runtime.flags & 0x20000000;
    if (flags != 0)
    {
        (void)(uint32)(flags);
        return;
    }
    count = game_runtime.camera_control.mode_ticks;
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = ((sint16)game_runtime.camera_shots.offset[2]);
        yaw = game_runtime.camera_view.pose.angles[1];
        game_runtime.camera_follow.orbit_offset[2] = (uint32)((0 - pitch) << 6);
        value = yaw + 0x2000;
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(value);
        if ((sint16)value < -8192)
            game_runtime.camera_follow.orbit_angles[1] = (uint16)(yaw + 0x6000);
        value = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
        raw = game_runtime.camera_follow.orbit_angles[1];
        if ((sint32)value >= 8193)
            game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw - 0x4000);
    }
    speed = game_runtime.camera_follow.orbit_offset[2];
    if ((sint32)speed <= 0xAEFFFF)
    {
        value = factor * 10;
        value = (sint32)value / 4096;
        game_runtime.camera_follow.orbit_offset[2] = (uint32)(speed + (value << 12));
    }
    value = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    raw = game_runtime.camera_follow.orbit_angles[0];
    if ((sint32)value >= -2047)
        game_runtime.camera_follow.orbit_angles[0] = (uint16)(raw - (uint32)((sint32)factor / 128));
    raw = game_runtime.camera_follow.orbit_angles[1];
    raw += (uint32)((sint32)factor / 128);
    value = (sint32)(raw << 16) >> 16;
    game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw);
    if ((sint32)value >= 8193)
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw - 0x4000);
    count = game_runtime.camera_control.mode_ticks;
    source_pose = mission_camera_pose();
    pitch = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    yaw = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    roll = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    game_runtime.camera_control.mode_ticks = (uint32)(count + 1);
    target_pitch = ((sint16)source_pose->angles[0]);
    target_yaw = ((sint16)source_pose->angles[1]);
    target_roll = ((sint16)source_pose->angles[2]);
    speed = game_runtime.camera_follow.orbit_offset[2];
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(&matrix, game_runtime.camera_follow.orbit_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(rx << 9);
    game_runtime.camera_follow.offset[1] = (uint32)(ry << 9);
    game_runtime.camera_follow.angles[0] = (uint16)(dx);
    game_runtime.camera_follow.angles[1] = (uint16)(dy);
    game_runtime.camera_follow.angles[2] = (uint16)(dz);
    value = rz << 9;
    game_runtime.camera_follow.offset[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_update_mode_32(void)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix;
    uint32 flags, count, factor, pitch, yaw, roll, speed, value, raw, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078D70, "ACE3.BIN");
    flags = game_runtime.flags & 0x20000000;
    if (flags != 0)
    {
        (void)(uint32)(flags);
        return;
    }
    count = game_runtime.camera_control.mode_ticks;
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = ((sint16)game_runtime.camera_shots.offset[2]);
        yaw = game_runtime.camera_view.pose.angles[1];
        roll = game_runtime.camera_view.pose.angles[2];
        game_runtime.camera_follow.orbit_offset[2] = (uint32)(0x190000 - (pitch << 6));
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(yaw);
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(roll);
    }
    speed = game_runtime.camera_follow.orbit_offset[2];
    if ((sint32)speed <= 0xAEFFFF)
    {
        value = factor * 20;
        value = (sint32)value / 4096;
        game_runtime.camera_follow.orbit_offset[2] = (uint32)(speed + (value << 12));
    }
    value = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    raw = game_runtime.camera_follow.orbit_angles[0];
    if ((sint32)value >= -2047)
        game_runtime.camera_follow.orbit_angles[0] = (uint16)(raw - (uint32)((sint32)factor / 128));
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    raw = game_runtime.camera_follow.orbit_angles[2];
    if ((sint32)value >= 17)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(raw - 16);
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    raw = game_runtime.camera_follow.orbit_angles[2];
    if ((sint32)value < -16)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(raw + 16);
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value < 16)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(0);
    raw = game_runtime.camera_follow.orbit_angles[1];
    raw += (uint32)((sint32)factor / 128);
    value = (sint32)(raw << 16) >> 16;
    game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw);
    if ((sint32)value >= 8193)
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw - 0x4000);
    count = game_runtime.camera_control.mode_ticks;
    source_pose = mission_camera_pose();
    pitch = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    yaw = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    roll = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    game_runtime.camera_control.mode_ticks = (uint32)(count + 1);
    target_pitch = ((sint16)source_pose->angles[0]);
    target_yaw = ((sint16)source_pose->angles[1]);
    target_roll = ((sint16)source_pose->angles[2]);
    speed = game_runtime.camera_follow.orbit_offset[2];
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(&matrix, game_runtime.camera_follow.orbit_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(rx << 9);
    game_runtime.camera_follow.offset[1] = (uint32)(ry << 9);
    game_runtime.camera_follow.angles[0] = (uint16)(dx);
    game_runtime.camera_follow.angles[1] = (uint16)(dy);
    game_runtime.camera_follow.angles[2] = (uint16)(dz);
    value = rz << 9;
    game_runtime.camera_follow.offset[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_update_mode_64(void)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix;
    uint32 flags, count, factor, pitch, yaw, roll, speed, value, raw, target_pitch, target_yaw, target_roll, dx, dy, dz, z, a, b, c, d, e, rx, ry, rz;
    FUNCTION_MARKER(0x80078FDC, "ACE3.BIN");
    flags = game_runtime.flags & 0x20000000;
    if (flags != 0)
    {
        (void)(uint32)(flags);
        return;
    }
    count = game_runtime.camera_control.mode_ticks;
    factor = count << 7;
    if ((sint32)factor > 4096)
        factor = 4096;
    if (count == 0)
    {
        pitch = ((sint16)game_runtime.camera_shots.offset[2]);
        yaw = game_runtime.camera_view.pose.angles[1];
        roll = game_runtime.camera_view.pose.angles[2];
        game_runtime.camera_follow.orbit_offset[2] = (uint32)(0x190000 - (pitch << 6));
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(yaw);
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(roll);
    }
    speed = game_runtime.camera_follow.orbit_offset[2];
    if ((sint32)speed <= 0x3E7FFF)
    {
        value = factor * 10;
        value = (sint32)value / 4096;
        game_runtime.camera_follow.orbit_offset[2] = (uint32)(speed + (value << 12));
    }
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    raw = game_runtime.camera_follow.orbit_angles[2];
    if ((sint32)value >= 17)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(raw - 16);
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    raw = game_runtime.camera_follow.orbit_angles[2];
    if ((sint32)value < -16)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(raw + 16);
    value = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)value < 16)
        game_runtime.camera_follow.orbit_angles[2] = (uint16)(0);
    count = game_runtime.camera_control.mode_ticks;
    source_pose = mission_camera_pose();
    pitch = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    yaw = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    roll = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    game_runtime.camera_control.mode_ticks = (uint32)(count + 1);
    target_pitch = ((sint16)source_pose->angles[0]);
    target_yaw = ((sint16)source_pose->angles[1]);
    target_roll = ((sint16)source_pose->angles[2]);
    speed = game_runtime.camera_follow.orbit_offset[2];
    z = (sint32)(0 - speed) >> 9;
    dx = pitch - target_pitch;
    dy = yaw - target_yaw;
    dz = roll - target_roll;
    rotation_build(&matrix, game_runtime.camera_follow.orbit_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, z & 0xFFFF);
    xport_gte_execute(0x486012);
    rx = xport_gte_read_data(25);
    ry = xport_gte_read_data(26);
    rz = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(rx << 9);
    game_runtime.camera_follow.offset[1] = (uint32)(ry << 9);
    game_runtime.camera_follow.angles[0] = (uint16)(dx);
    game_runtime.camera_follow.angles[1] = (uint16)(dy);
    game_runtime.camera_follow.angles[2] = (uint16)(dz);
    value = rz << 9;
    game_runtime.camera_follow.offset[2] = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void camera_update_mode_256(void)
{
    const NATIVE_POSE *source_pose;
    MATRIX matrix;
    uint32 flags, phase, product, pitch, radius, value, raw, a, b, c, d, e, x, y, z, dx, dy, dz, vector_z, count, result;
    FUNCTION_MARKER(0x800791E8, "ACE3.BIN");
    flags = game_runtime.flags & 0x20000000;
    if (flags != 0)
    {
        (void)(uint32)(flags);
        return;
    }
    phase = game_runtime.camera_control.mode_ticks << 5;
    if ((sint32)phase > 4096)
        phase = 4096;
    value = rsin((sint32)phase >> 1);
    product = 0 - (value << 9);
    if ((sint32)product < 0)
        product += 4095;
    pitch = (uint32)((sint32)product >> 12) - 3584;
    value = rsin((sint32)phase >> 2);
    source_pose = mission_camera_pose();
    game_runtime.camera_follow.orbit_angles[0] = (uint16)(pitch);
    raw = (source_pose->angles[1]);
    radius = 8192 - (value << 1);
    raw += radius;
    game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw);
    if ((sint16)raw < -8192)
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw + 16384);
    value = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    raw = game_runtime.camera_follow.orbit_angles[1];
    if ((sint32)value >= 8193)
        game_runtime.camera_follow.orbit_angles[1] = (uint16)(raw - 16384);
    a = ((sint16)game_runtime.camera_shots.offset[2]);
    dx = ((sint16)game_runtime.camera_follow.orbit_angles[0]);
    dy = ((sint16)game_runtime.camera_follow.orbit_angles[1]);
    dz = ((sint16)game_runtime.camera_follow.orbit_angles[2]);
    value = ((0 - a) << 10) - (radius << 9);
    game_runtime.camera_follow.orbit_offset[2] = (uint32)(value);
    count = game_runtime.camera_control.mode_ticks;
    source_pose = mission_camera_pose();
    game_runtime.camera_control.mode_ticks = (uint32)(count + 1);
    a = ((sint16)source_pose->angles[0]);
    b = ((sint16)source_pose->angles[1]);
    c = ((sint16)source_pose->angles[2]);
    value = game_runtime.camera_follow.orbit_offset[2];
    vector_z = (sint32)(0 - value) >> 9;
    dx -= a;
    dy -= b;
    dz -= c;
    rotation_build(&matrix, game_runtime.camera_follow.orbit_angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, vector_z & 0xFFFF);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    game_runtime.camera_follow.offset[0] = (uint32)(x << 9);
    game_runtime.camera_follow.offset[1] = (uint32)(y << 9);
    game_runtime.camera_follow.angles[0] = (uint16)(dx);
    game_runtime.camera_follow.angles[1] = (uint16)(dy);
    game_runtime.camera_follow.angles[2] = (uint16)(dz);
    result = z << 9;
    game_runtime.camera_follow.offset[2] = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void camera_shots_set_player(const NATIVE_POSE *source, const uint32 velocity[3])
{
    uint32 value;
    FUNCTION_MARKER(0x800796B8, "ACE3.BIN");
    value = ((uint32)source->position[0]);
    game_runtime.camera_shots.subjects[0].pose.position[0] = (uint32)(value);
    value = ((uint32)source->position[1]);
    game_runtime.camera_shots.subjects[0].pose.position[1] = (uint32)(value);
    value = ((uint32)source->position[2]);
    game_runtime.camera_shots.subjects[0].pose.position[2] = (uint32)(value);
    value = ((uint16)source->angles[0]);
    game_runtime.camera_shots.subjects[0].pose.angles[0] = (uint16)(value);
    value = ((uint16)source->angles[1]);
    game_runtime.camera_shots.subjects[0].pose.angles[1] = (uint16)(value);
    value = ((uint16)source->angles[2]);
    game_runtime.camera_shots.subjects[0].pose.angles[2] = (uint16)(value);
    value = velocity[0];
    game_runtime.camera_shots.subjects[0].velocity[0] = (uint32)(value);
    value = velocity[1];
    game_runtime.camera_shots.subjects[0].velocity[1] = (uint32)(value);
    value = velocity[2];
    game_runtime.camera_shots.subjects[0].velocity[2] = (uint32)(value);
    return;
}

void camera_shots_set_target(uint32 active, const NATIVE_POSE *source, const uint32 velocity[3], uint32 mode)
{
    uint32 value;
    FUNCTION_MARKER(0x80079730, "ACE3.BIN");
    if (active != 0 && mode != 0)
    {
        value = ((uint32)source->position[0]);
        game_runtime.camera_shots.subjects[1].pose.position[0] = (uint32)(value);
        value = ((uint32)source->position[1]);
        game_runtime.camera_shots.subjects[1].pose.position[1] = (uint32)(value);
        value = ((uint32)source->position[2]);
        game_runtime.camera_shots.subjects[1].pose.position[2] = (uint32)(value);
        value = ((uint16)source->angles[0]);
        game_runtime.camera_shots.subjects[1].pose.angles[0] = (uint16)(value);
        value = ((uint16)source->angles[1]);
        game_runtime.camera_shots.subjects[1].pose.angles[1] = (uint16)(value);
        value = ((uint16)source->angles[2]);
        game_runtime.camera_shots.subjects[1].pose.angles[2] = (uint16)(value);
        value = velocity[0];
        game_runtime.camera_shots.subjects[1].velocity[0] = (uint32)(value);
        value = velocity[1];
        game_runtime.camera_shots.subjects[1].velocity[1] = (uint32)(value);
        value = velocity[2];
        game_runtime.camera_shots.subjects[1].velocity[2] = (uint32)(value);
        value = ((sint16)game_runtime.camera_shots.target_mode);
        if (value == 0)
            game_runtime.camera_shots.target_changed = (uint16)(1);
        game_runtime.camera_shots.target_mode = (uint16)(mode);
    }
    else
    {
        game_runtime.camera_shots.target_mode = (uint16)(0);
    }
    return;
}

void camera_begin_player_loss_view(uint32 mode)
{
    uint32 camera_actor_index_descriptor;

    MISSION_ACTOR_TRAITS *actor_traits_descriptor;
    uint32 actor_index_descriptor;

    static const uint32 modes[3] = {16, 32, 64};
    uint32 selected;
    FUNCTION_MARKER(0x80079564, "ACE3.BIN");
    actor_index_descriptor = 0;
    selected = modes[mode];
    (void)mission_actor(actor_index_descriptor);
    camera_actor_index_descriptor = actor_index_descriptor;
    actor_traits_descriptor = mission_actor_traits(actor_index_descriptor);
    camera_set_mode(selected, actor_traits_descriptor, camera_actor_index_descriptor);
    {
        (void)(mission_apply_hud_visibility(0));
        return;
    }
}

void camera_apply_shake(uint32 mode)
{
    const uint16 *offsets;
    uint32 index, value, delta, result;
    FUNCTION_MARKER(0x8006CB44, "ACE3.BIN");
    result = game_runtime.camera_control.mode & 3;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    index = game_runtime.gameplay_counter & 7;
    if (mode == 1)
    {
        offsets = camera_shake_initial[index];
        value = (uint16)game_runtime.mission_view.offset[0];
        delta = offsets[0];
        game_runtime.mission_view.offset[0] = (uint16)(value + delta);
        value = (uint16)game_runtime.mission_view.offset[1];
        delta = offsets[1];
        game_runtime.mission_view.offset[1] = (uint16)(value + delta);
        value = (uint16)game_runtime.mission_view.shift[0];
        delta = offsets[0];
        game_runtime.mission_view.shift[0] = (uint16)(value + delta);
        value = (uint16)game_runtime.mission_view.shift[1];
        delta = offsets[1];
        game_runtime.mission_view.shift[1] = (uint16)(value + delta);
    }
    offsets = mode == 0 ? camera_shake_default[index] : camera_shake_alternate[index];
    delta = offsets[0];
    value = (uint16)game_runtime.mission_view.offset[0];
    game_runtime.mission_view.offset[0] = (uint16)(value + delta);
    value = (uint16)game_runtime.mission_view.offset[1];
    delta = offsets[1];
    game_runtime.mission_view.offset[1] = (uint16)(value + delta);
    value = (uint16)game_runtime.mission_view.shift[0];
    delta = offsets[0];
    game_runtime.mission_view.shift[0] = (uint16)(value + delta);
    value = (uint16)game_runtime.mission_view.shift[1];
    delta = offsets[1];
    game_runtime.mission_view.shift[1] = (uint16)(value + delta);
    if ((sint16)game_runtime.mission_view.offset[0] >= 7)
    {
        game_runtime.mission_view.offset[0] = (uint16)(6);
    }
    if ((sint16)game_runtime.mission_view.offset[1] >= 7)
        game_runtime.mission_view.offset[1] = (uint16)(6);
    if ((sint16)game_runtime.mission_view.shift[0] >= 7)
    {
        game_runtime.mission_view.shift[0] = (uint16)(6);
    }
    if ((sint16)game_runtime.mission_view.shift[1] >= 7)
        game_runtime.mission_view.shift[1] = (uint16)(6);
    return;
}

void camera_try_kill_view(uint32 flags)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 result, index;
    FUNCTION_MARKER(0x800797FC, "ACE3.BIN");
    result = ~flight_dynamics.flags & 1;
    if (result != 0 || (sint32)flags < 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = game_runtime.options.kill_camera;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    index = flags & 0x3F;
    actor_index_table = 0;
    (void)mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    camera_actor_index_object = (actor_index_table + ((uint32)((sint32)(index * 4) >> 2)));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
    result = ~actor_traits_object->flags & 0x4000;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    game_runtime.camera_control.view_actor = (uint32)(index);
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(2048, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(0));
        return;
    }
}

void camera_apply_option(void)
{
    uint32 option;
    FUNCTION_MARKER(0x8007953C, "ACE3.BIN");
    option = game_runtime.options.camera_mode;
    {
        (void)(camera_apply_player_option(option));
        return;
    }
}

void camera_apply_player_option(uint32 option)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 flags;
    FUNCTION_MARKER(0x800794D8, "ACE3.BIN");
    flags = game_runtime.camera_control.mode & 0x800;
    if (flags != 0)
    {
        (void)(uint32)(flags);
        return;
    }
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(option != 0 ? 2 : 1, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(1));
        return;
    }
}

void camera_enter_scripted_view(void)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FUNCTION_MARKER(0x80079680, "ACE3.BIN");
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(512, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(0));
        return;
    }
}

void camera_set_cinematic_mode(uint32 enabled)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 mode;
    FUNCTION_MARKER(0x800795C8, "ACE3.BIN");
    if (enabled != 0)
    {
        mode = 0x100;
        mission_apply_hud_visibility(0);
    }
    else
    {
        mode = 0x80;
        mission_apply_hud_visibility(1);
    }
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    {
        (void)(camera_set_mode(mode, actor_traits_object, camera_actor_index_object));
        return;
    }
}

void camera_restore_player_mode(uint32 flags)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 mode, enabled;
    FUNCTION_MARKER(0x8007961C, "ACE3.BIN");
    if ((flags & 0x6300) != 0)
        mode = 0x400;
    else
    {
        enabled = game_runtime.options.camera_mode;
        mode = enabled != 0 ? 2 : 1;
    }
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(mode, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(1));
        return;
    }
}

void camera_set_scripted_mode(void)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FUNCTION_MARKER(0x80079924, "ACE3.BIN");
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(0x2000, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(0));
        return;
    }
}

void camera_initialize_mission(void)
{
    uint32 x, y;
    FUNCTION_MARKER(0x800798A8, "ACE3.BIN");
    x = game_runtime.render_settings.near_plane;
    y = game_runtime.render_settings.far_plane;
    {
        (void)(camera_initialize(296, x, y, game_runtime.aircraft_mission_metadata.camera_offsets));
        return;
    }
}

void camera_set_movie_mode(void)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FUNCTION_MARKER(0x800798EC, "ACE3.BIN");
    actor_index_table = 0;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    camera_set_mode(4096, actor_traits_object, camera_actor_index_object);
    {
        (void)(mission_apply_hud_visibility(0));
        return;
    }
}
