#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    MISSION_ACTOR_SNAPSHOT initial[MISSION_ACTOR_COUNT];
    MISSION_ACTOR_SNAPSHOT previous[MISSION_ACTOR_COUNT];
    MISSION_ACTOR_SNAPSHOT playback[MISSION_ACTOR_COUNT];
    MISSION_ACTOR_SNAPSHOT saved[MISSION_ACTOR_COUNT];
    uint32 player_velocity[3];
    uint32 target_velocity[3];
    uint32 frame_count;
    uint8 *ring_data;
    uint8 *demo_data;
    uint32 demo_bytes;
    uint32 ring_limit;
    uint32 write_cursor;
    uint32 oldest_cursor;
    uint32 frame_complete;
    uint32 played_frames;
    uint32 play_cursor;
    uint32 target_index;
    uint32 camera_target;
    uint32 camera_target_delay;
    uint32 record_target;
    uint8 playback_ready;
    uint8 ring_owned;
} MISSION_REPLAY;

static MISSION_REPLAY mission_replay;
static void mission_actor_snapshot_decode_pose(MISSION_ACTOR_SNAPSHOT *snapshot, const uint8 *src);
static void mission_actor_snapshot_decode(MISSION_ACTOR_SNAPSHOT *snapshot, const uint8 *src);

static void mission_replay_apply_effect_pose(const uint8 *packet);
static void mission_replay_apply_effect(const uint8 *packet);
static void mission_replay_spawn_position(uint32 kind, const uint8 *position);
static void mission_replay_spawn_vector(uint32 kind, const uint8 *packet);
static void mission_replay_spawn_segment(uint32 kind, const uint8 *packet);
static uint8 *mission_replay_reserve_words(uint32 words);
static void mission_replay_release_words(uint32 words);
static void mission_replay_write_actor_control(uint32 action, uint32 index, uint32 value, uint32 extra);
static void mission_replay_write_actor_pose(uint32 index, const NATIVE_POSE *pose);
static void mission_replay_write_effect_pose(uint32 action, uint32 index, uint32 value, uint32 extra, const NATIVE_POSE *pose, const uint32 *velocity);
static void mission_replay_write_tile_change(uint32 kind, uint32 index, uint32 value, uint32 previous);
static uint32 mission_replay_apply_auxiliary_packet(uint32 cursor, uint32 forward, uint32 *complete);
static void mission_replay_apply_tile_change(const uint8 *packet);
static void mission_replay_restore_tile_change(const uint8 *packet);
static void mission_replay_scan_auxiliary_packets(uint32 forward);

void mission_replay_reset_state(void)
{
    free(mission_replay.demo_data);
    if (mission_replay.ring_owned)
        free(mission_replay.ring_data);
    memset(&mission_replay, 0, sizeof(mission_replay));
}

static void mission_replay_apply_control(const uint8 *record, MISSION_ACTOR_SNAPSHOT *context);
static void mission_replay_apply_pose(const uint8 *record, MISSION_ACTOR_SNAPSHOT *context);
static uint32 mission_replay_actor_changes(const MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor);
static void mission_replay_actor_remember(MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor);

void mission_replay_initialize(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 bytes, block, index;
    FUNCTION_MARKER(0x8007DB30, "ACE3.BIN");
    arena_alloc(0x2054);
    mission_replay.playback_ready = 0;
    mission_replay.frame_count = 0;
    mission_replay.played_frames = 0;
    mission_replay.record_target = 0xFFFFFFFF;
    bytes = arena_available() - 0x8000;
    buffer_allocation = arena_alloc_backed(bytes);
    block = buffer_allocation.encoded_begin;
    bytes &= 0xFFFFFFFC;
    if (block == 0 || bytes <= 64)
        abort();
    if (mission_replay.ring_owned)
        free(mission_replay.ring_data);
    mission_replay.ring_data = buffer_allocation.buffer.data;
    mission_replay.ring_owned = 0;
    mission_replay.ring_limit = bytes - 64;
    mission_replay.oldest_cursor = 0;
    mission_replay.write_cursor = 0;
    for (index = 0; index < 64; ++index)
        mission_replay.initial[63 - index].flags = 0;
}

static void mission_replay_bind_resource(uint8 *data, uint32 bytes)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint16 wire_halfword_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    const uint8 *header;
    uint8 *directory, *ring;
    uint32 directory_offset, header_offset, ring_offset, size, index;
    FUNCTION_MARKER(0x8007DBC4, "ACE3.BIN");
    if (data == NULL || bytes < 12 || (memcpy(&wire_halfword_0, data, sizeof(wire_halfword_0)), wire_halfword_0) != 1)
        abort();
    directory_offset = (memcpy(&wire_word_1, data + 4, sizeof(wire_word_1)), wire_word_1);
    if (directory_offset < 8 || directory_offset > bytes - 12 || (directory_offset & 3))
        abort();
    directory = data + directory_offset;
    bytes -= directory_offset;
    if ((memcpy(&wire_halfword_2, directory, sizeof(wire_halfword_2)), wire_halfword_2) != 2)
        abort();
    header_offset = (memcpy(&wire_word_3, directory + 4, sizeof(wire_word_3)), wire_word_3);
    ring_offset = (memcpy(&wire_word_4, directory + 8, sizeof(wire_word_4)), wire_word_4);
    if (header_offset < 12 || ring_offset < header_offset || ring_offset > bytes || ring_offset - header_offset < 0x2054 || ((header_offset | ring_offset) & 3))
        abort();
    header = directory + header_offset;
    mission_replay.frame_count = (memcpy(&wire_word_5, header, sizeof(wire_word_5)), wire_word_5);
    size = (memcpy(&wire_word_6, header + 0x2050, sizeof(wire_word_6)), wire_word_6) & 0xFFFFFFFC;
    if (size <= 64 || size > bytes - ring_offset)
        abort();
    ring = malloc(size);
    if (!ring)
        abort();
    memcpy(ring, directory + ring_offset, size);
    if (mission_replay.ring_owned)
        free(mission_replay.ring_data);
    mission_replay.ring_data = ring;
    mission_replay.ring_owned = 1;
    mission_replay.ring_limit = size - 64;
    mission_replay.oldest_cursor = (memcpy(&wire_word_7, header + 16, sizeof(wire_word_7)), wire_word_7);
    mission_replay.write_cursor = (memcpy(&wire_word_8, header + 12, sizeof(wire_word_8)), wire_word_8);
    if (mission_replay.oldest_cursor >= mission_replay.ring_limit || mission_replay.write_cursor >= mission_replay.ring_limit || ((mission_replay.oldest_cursor | mission_replay.write_cursor) & 3))
        abort();
    mission_replay.played_frames = 0;
    mission_replay.record_target = 0xFFFFFFFF;
    for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
    {
        mission_actor_snapshot_decode(&mission_replay.initial[index], header + 24 + index * 32);
        mission_actor_snapshot_decode(&mission_replay.previous[index], header + 0x818 + index * 32);
        mission_actor_snapshot_decode(&mission_replay.playback[index], header + 0x1020 + index * 32);
        mission_actor_snapshot_decode(&mission_replay.saved[index], header + 0x1850 + index * 32);
    }
    mission_replay.playback_ready = 1;
}

static void mission_replay_apply_control(const uint8 *record, MISSION_ACTOR_SNAPSHOT *context)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    MISSION_ACTOR_SNAPSHOT *target;
    uint32 header, mode, value, offset;
    FUNCTION_MARKER(0x8007DC3C, "ACE3.BIN");
    mode = ((memcpy(&wire_word_0, record, sizeof(wire_word_0)), wire_word_0) >> 7) & 7;
    switch (mode)
    {
        case 0:
            mission_replay.frame_complete = 1;
            return;
        case 1:
            header = (memcpy(&wire_word_1, record, sizeof(wire_word_1)), wire_word_1);
            target = context + ((header >> 10) & 63);
            value = target->flags;
            if (header & 0x10000)
                value |= 0x14000;
            else
                value &= 0xFFFEBFFF;
            target->flags = value;
            return;
        case 2:
            header = (memcpy(&wire_word_2, record, sizeof(wire_word_2)), wire_word_2);
            value = record[2];
            target = context + ((header >> 10) & 63);
            target->model = (uint8)value;
            value = record[3];
            target->render_flags = (uint8)value;
            return;
        case 3:
            header = (memcpy(&wire_word_3, record, sizeof(wire_word_3)), wire_word_3);
            offset = (header >> 5) & 0x7E0;
            context[offset >> 5].parameter = (uint16)((header >> 16) & 0x7FFF);
            return;
        case 4:
            header = (memcpy(&wire_word_4, record, sizeof(wire_word_4)), wire_word_4);
            value = (header >> 10) & 0x3F;
            if (value == 0)
                value = 0xFFFFFFFF;
            mission_replay.target_index = value;
            mission_replay.target_velocity[0] = (uint32)(0);
            mission_replay.target_velocity[1] = (uint32)(0);
            mission_replay.target_velocity[2] = (uint32)(0);
            return;
        default:
            return;
    }
}

static void mission_replay_apply_pose(const uint8 *record, MISSION_ACTOR_SNAPSHOT *context)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    MISSION_ACTOR_SNAPSHOT *target;
    uint32 header, mode, value, result;
    FUNCTION_MARKER(0x8007DD58, "ACE3.BIN");
    header = (memcpy(&wire_word_0, record, sizeof(wire_word_0)), wire_word_0);
    mode = (header >> 7) & 3;
    if (mode)
    {
        (void)(uint32)(mode);
        return;
    }
    target = context + ((header >> 9) & 63);
    value = (memcpy(&wire_word_1, record + 4, sizeof(wire_word_1)), wire_word_1);
    target->pose.position[0] = value & 0xFFFFF000;
    value = (memcpy(&wire_word_2, record + 8, sizeof(wire_word_2)), wire_word_2);
    target->pose.position[1] = value & 0xFFFFF000;
    value = (memcpy(&wire_word_3, record + 12, sizeof(wire_word_3)), wire_word_3);
    target->pose.position[2] = value & 0xFFFFF000;
    value = (memcpy(&wire_word_4, record + 4, sizeof(wire_word_4)), wire_word_4);
    target->pose.angles[0] = (uint16)((sint32)(value << 20) >> 18);
    value = (memcpy(&wire_word_5, record + 8, sizeof(wire_word_5)), wire_word_5);
    target->pose.angles[1] = (uint16)((sint32)(value << 20) >> 18);
    value = (memcpy(&wire_word_6, record + 12, sizeof(wire_word_6)), wire_word_6);
    result = ((sint32)(value << 20) >> 18);
    target->pose.angles[2] = (uint16)result;
    {
        (void)(uint32)(result);
        return;
    }
}

static uint32 mission_replay_release_packet(const uint8 *record)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 tag, kind;
    FUNCTION_MARKER(0x8007DDF4, "ACE3.BIN");
    tag = (memcpy(&wire_word_0, record, sizeof(wire_word_0)), wire_word_0);
    kind = (tag >> 4) & 7;
    if (kind == 0)
        mission_replay_apply_control(record, mission_replay.initial);
    else if (kind == 1)
        mission_replay_apply_pose(record, mission_replay.initial);
    return (memcpy(&wire_word_1, record, sizeof(wire_word_1)), wire_word_1) & 15;
}

static uint32 mission_replay_actor_changes(const MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor)
{
    const MISSION_ACTOR_TRAITS *actor_traits = &actor->traits;
    uint32 old_value, old_flags, flags, value, changed, x0, y0, z0, x1, y1, z1;
    FUNCTION_MARKER(0x8007E5BC, "ACE3.BIN");
    old_value = (sint16)(snapshot->model | ((uint32)snapshot->render_flags << 8));
    old_flags = snapshot->flags;
    flags = actor_traits->flags;
    value = (sint16)((uint32)actor_traits->model | ((uint32)actor_traits->render_flags << 8));
    changed = ((flags ^ old_flags) & 0x14000) != 0;
    if (old_value != value)
        changed |= 4;
    value = actor_traits->render_flags;
    if (value & 4)
    {
        old_value = (sint16)snapshot->parameter;
        value = (sint16)actor_traits->parameter;
        if (old_value != value)
            changed |= 8;
    }
    x0 = snapshot->pose.position[0];
    y0 = snapshot->pose.position[1];
    z0 = snapshot->pose.position[2];
    x1 = actor->pose.position[0];
    y1 = actor->pose.position[1];
    z1 = actor->pose.position[2];
    if (((sint32)x0 >> 12) != ((sint32)x1 >> 12))
        changed |= 16;
    if (((sint32)y0 >> 12) != ((sint32)y1 >> 12))
        changed |= 16;
    if (((sint32)z0 >> 12) != ((sint32)z1 >> 12))
        changed |= 16;
    x0 = snapshot->pose.angles[0];
    y0 = snapshot->pose.angles[1];
    y1 = actor->pose.angles[1];
    z0 = snapshot->pose.angles[2];
    x1 = actor->pose.angles[0];
    z1 = actor->pose.angles[2];
    if (((sint32)(x0 << 16) >> 18) != ((sint32)(x1 << 16) >> 18))
        changed |= 16;
    if (((sint32)(y0 << 16) >> 18) != ((sint32)(y1 << 16) >> 18))
        changed |= 16;
    if (((sint32)(z0 << 16) >> 18) != ((sint32)(z1 << 16) >> 18))
        changed |= 16;
    return changed;
}

static void mission_replay_actor_remember(MISSION_ACTOR_SNAPSHOT *snapshot, const MISSION_ACTOR *actor)
{
    const MISSION_ACTOR_TRAITS *actor_traits = &actor->traits;
    uint32 flags, value, x, y, z;
    FUNCTION_MARKER(0x8007E6D8, "ACE3.BIN");
    flags = actor_traits->flags;
    value = (~flags) & 0x4000;
    if (value)
    {
        snapshot->flags = flags;
        {
            (void)(uint32)(value);
            return;
        }
    }
    value = ((uint32)actor_traits->model | ((uint32)actor_traits->render_flags << 8) | ((uint32)actor_traits->parameter << 16));
    snapshot->flags = flags;
    snapshot->model = (uint8)value;
    snapshot->render_flags = (uint8)(value >> 8);
    snapshot->parameter = (uint16)(value >> 16);
    x = actor->pose.position[0];
    y = actor->pose.position[1];
    z = actor->pose.position[2];
    snapshot->pose.position[0] = x;
    snapshot->pose.position[1] = y;
    snapshot->pose.position[2] = z;
    {
        (void)(uint32)(z);
        return;
    }
}

void mission_replay_record_actors(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    MISSION_ACTOR_SNAPSHOT *snapshot;
    uint32 index, selector, changed, forced, flags;
    FUNCTION_MARKER(0x8007E720, "ACE3.BIN");
    for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
    {
        forced = 0;
        actor_index_table = 0;
        selector = mission_replay.record_target;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        actor_traits_object = &actor_object->traits;
        snapshot = &mission_replay.previous[index];
        if (index == (selector & 63))
            forced = (~selector) >> 31;
        flags = snapshot->flags;
        if ((flags & 0x14000) != 0x14000)
        {
            flags = actor_traits_object->flags;
            if ((flags & 0x14000) != 0x14000 && !forced)
            {
                mission_replay_actor_remember(snapshot, actor_object);
                continue;
            }
        }
        changed = mission_replay_actor_changes(snapshot, actor_object);
        flags = actor_traits_object->flags;
        if ((flags & 0x2000) == 0 && (changed & 1) == 0)
            continue;
        if (changed & 1)
            mission_replay_write_actor_control(1, index, (flags & 0x14000) == 0x14000, 0);
        if (changed & 4)
        {
            flags = actor_traits_object->model;
            selector = actor_traits_object->render_flags;
            mission_replay_write_actor_control(2, index, flags, selector);
        }
        if (changed & 8)
            mission_replay_write_actor_control(3, index, (sint16)actor_traits_object->parameter, 0);
        if (changed & 16)
            mission_replay_write_actor_pose(index, &pose_owner_object->pose);
        mission_replay_actor_remember(snapshot, actor_object);
    }
    return;
}

uint32 mission_replay_record_frame(void)
{
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 value, index;
    FUNCTION_MARKER(0x8007F0F8, "ACE3.BIN");
    if (mission_replay.frame_count == 0)
    {
        mission_replay.oldest_cursor = 0;
        mission_replay.write_cursor = 0;
        for (index = 0; index < MISSION_ACTOR_COUNT; ++index)
        {
            actor_index_table = 0;
            actor_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            actor_traits_object = &actor_object->traits;
            mission_actor_snapshot_capture(&mission_replay.initial[index], actor_object);
            mission_actor_snapshot_capture(&mission_replay.previous[index], actor_object);
        }
    }
    mission_replay_record_actors();
    mission_replay_write_actor_control(0, 0, 0, 0);
    value = mission_replay.frame_count + 1;
    mission_replay.frame_count = value;
    return value;
}

static uint32 mission_replay_apply_packet(uint32 cursor)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    const uint8 *packet;
    MISSION_ACTOR_SNAPSHOT *context;
    uint32 tag, type;
    FUNCTION_MARKER(0x8007F1D4, "ACE3.BIN");
    packet = mission_replay.ring_data + cursor;
    context = mission_replay.playback;
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    type = (tag >> 4) & 7;
    switch (type)
    {
        case 0:
            mission_replay_apply_control(packet, context);
            break;
        case 1:
            mission_replay_apply_pose(packet, context);
            break;
        case 2:
            mission_replay_apply_effect_pose(packet);
            break;
        case 3:
            mission_replay_apply_effect(packet);
            break;
        case 4:
            mission_replay_apply_tile_change(packet);
            break;
    }
    tag = (memcpy(&wire_word_1, packet, sizeof(wire_word_1)), wire_word_1);
    return cursor + ((tag & 15) << 2);
}

void mission_replay_reset(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    MATRIX matrix;
    uint32 cursor, a, b, c, d, e, x, y, z, index;
    FUNCTION_MARKER(0x8007F298, "ACE3.BIN");
    cursor = mission_replay.oldest_cursor;
    mission_replay.played_frames = 0;
    mission_replay.target_index = 0xFFFFFFFF;
    mission_replay.camera_target = 0xFFFFFFFF;
    mission_replay.camera_target_delay = 0;
    mission_replay.play_cursor = cursor;
    rotation_build(&matrix, mission_replay.playback_ready ? mission_replay.playback[0].pose.angles : mission_replay.initial[0].pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x4800);
    xport_gte_execute(0x486012);
    x = xport_gte_read_data(25);
    mission_replay.player_velocity[0] = (uint32)(x);
    y = xport_gte_read_data(26);
    mission_replay.player_velocity[1] = (uint32)(y);
    z = xport_gte_read_data(27);
    mission_replay.player_velocity[2] = (uint32)(z);
    x = mission_replay.player_velocity[0];
    z = mission_replay.player_velocity[2];
    mission_replay.target_velocity[0] = (uint32)(0);
    mission_replay.target_velocity[1] = (uint32)(0);
    mission_replay.target_velocity[2] = (uint32)(0);
    mission_replay.player_velocity[0] = (uint32)(x << 3);
    y = mission_replay.player_velocity[1];
    mission_replay.player_velocity[2] = (uint32)(z << 3);
    mission_replay.player_velocity[1] = (uint32)(y << 3);
    effect_pools_reset();
    effect_trails_reset();
    effect_objects_reset();
    for (index = 0; index < 64; ++index)
    {
        mission_replay.playback[index] = mission_replay.initial[index];
    }
    mission_replay.playback_ready = 1;
    for (index = 0; index < 64; ++index)
    {
        actor_index_table = 0;
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        mission_actor_snapshot_apply(&mission_replay.initial[index], actor_object);
    }
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_object = mission_actor(actor_index_table);
    actor_traits_object = &actor_object->traits;
    camera_shots_set_player(&pose_owner_object->pose, mission_replay.player_velocity);
    {
        camera_shots_set_target(0, NULL, NULL, 0);
        return;
    }
}

uint32 mission_replay_begin(void)
{
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 flags, active, index, argument;
    sint32 selector;
    const RESOURCE_AUDIO_TRACK *descriptor;
    FUNCTION_MARKER(0x8007F448, "ACE3.BIN");
    pad_reset_slots();
    flags = game_runtime.flags;
    active = game_runtime.playback.demo_mode;
    game_runtime.flags = flags | 0x10000000;
    if (active != 0)
    {
        selector = (sint16)game_runtime.selection.mission;
        index = mission_audio_music_index(selector);
        descriptor = audio_main_track(index);
        selector = descriptor->id;
        if (resource_is_cd_audio(selector) == 0)
        {
            selector = (sint16)game_runtime.selection.mission;
            selector = (sint16)(selector % 6);
            descriptor = audio_fallback_track((uint32)selector + 1);
        }
    }
    else
        descriptor = audio_fallback_track(0);
    resource_queue_audio(descriptor, 0, 0);
    effect_objects_reset();
    camera_enter_scripted_view();
    for (index = 0; index < 64; ++index)
    {
        actor_index_table = 0;
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        mission_actor_snapshot_capture(&mission_replay.saved[index], actor_object);
    }
    mission_replay_reset();
    mission_replay_scan_auxiliary_packets(0);
    flags = game_runtime.mission_control.flags;
    argument = game_runtime.mission_control.argument;
    game_runtime.mission_control.flags = (uint32)(flags & 0xEFFFFFFF);
    return mission_request_mode(256, argument);
}

uint32 mission_replay_update(void)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const MISSION_ACTOR_SNAPSHOT *snapshot;
    uint32 count, total, previous, cursor, end, complete, selected, timer;
    uint32 x, y, z, a, b, c, index, target, mode, kind;
    FUNCTION_MARKER(0x8007F5B4, "ACE3.BIN");
    count = mission_replay.played_frames;
    total = mission_replay.frame_count;
    if ((sint32)count >= (sint32)total)
        return 1;
    previous = mission_replay.target_index;
    cursor = mission_replay.play_cursor;
    mission_replay.played_frames = count + 1;
    mission_replay.frame_complete = 0;
    do
    {
        end = mission_replay.ring_limit;
        if (cursor >= end)
            cursor = 0;
        cursor = mission_replay_apply_packet(cursor);
        complete = mission_replay.frame_complete;
    } while (complete == 0);
    selected = mission_replay.target_index;
    mission_replay.play_cursor = cursor;
    if (previous != selected)
    {
        mission_replay.camera_target_delay = 150;
        if (previous == 0xFFFFFFFF)
        {
            selected = mission_replay.target_index;
            mission_replay.camera_target = selected;
        }
    }
    timer = mission_replay.camera_target_delay;
    if (timer != 0)
        mission_replay.camera_target_delay = timer - 1;
    else
    {
        selected = mission_replay.target_index;
        mission_replay.camera_target = selected;
    }
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_object = mission_actor(actor_index_table);
    actor_traits_object = &actor_object->traits;
    x = pose_owner_object->pose.position[0];
    y = pose_owner_object->pose.position[1];
    z = pose_owner_object->pose.position[2];
    snapshot = &mission_replay.playback[0];
    a = snapshot->pose.position[0];
    b = snapshot->pose.position[1];
    a -= x;
    b -= y;
    c = snapshot->pose.position[2];
    selected = mission_replay.camera_target;
    mission_replay.player_velocity[0] = (uint32)(a);
    mission_replay.player_velocity[1] = (uint32)(b);
    c -= z;
    mission_replay.player_velocity[2] = (uint32)(c);
    if ((sint32)selected >= 0)
    {
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        x = pose_owner_object->pose.position[0];
        y = pose_owner_object->pose.position[1];
        z = pose_owner_object->pose.position[2];
        snapshot = &mission_replay.playback[selected];
        a = snapshot->pose.position[0];
        c = snapshot->pose.position[2];
        a -= x;
        b = snapshot->pose.position[1];
        c -= z;
        mission_replay.target_velocity[0] = (uint32)(a);
        mission_replay.target_velocity[2] = (uint32)(c);
        b -= y;
        mission_replay.target_velocity[1] = (uint32)(b);
    }
    for (index = 0; index < 64; ++index)
    {
        actor_index_table = 0;
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        mission_actor_snapshot_apply(&mission_replay.playback[index], actor_object);
    }
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_object = mission_actor(actor_index_table);
    actor_traits_object = &actor_object->traits;
    camera_shots_set_player(&pose_owner_object->pose, mission_replay.player_velocity);
    selected = mission_replay.camera_target;
    target = 0;
    mode = 0;
    if ((sint32)selected >= 0)
    {
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((selected << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        target = 1;
        if ((sint32)selected < 32)
            mode = 1;
        else
        {
            kind = (sint16)actor_traits_object->kind;
            mode = kind == 35 || kind == 36 || kind == 43 || kind == 61 ? 1 : 2;
        }
    }
    camera_shots_set_target(target != 0, &pose_owner_object->pose, mission_replay.target_velocity, mode);
    return 0;
}

void mission_replay_end(void)
{
    MISSION_ACTOR *actor_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 flags, index, active;
    FUNCTION_MARKER(0x8007F808, "ACE3.BIN");
    resource_cancel_streams();
    mission_replay_reset();
    mission_replay_scan_auxiliary_packets(1);
    mission_replay.frame_count = 0;
    mission_replay.played_frames = 0;
    flags = game_runtime.flags;
    game_runtime.flags = flags & 0xEFFFFFFF;
    for (index = 0; index < 64; ++index)
    {
        actor_index_table = 0;
        actor_object = mission_actor((actor_index_table + ((uint32)((sint32)((index << 2)) >> 2))));
        actor_traits_object = &actor_object->traits;
        mission_actor_snapshot_apply(&mission_replay.saved[index], actor_object);
    }
    effect_objects_reset();
    active = game_runtime.playback.demo_mode;
    if (active == 0)
    {
        mission_dispatch_scene_request();
        return;
    }
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags | 0x10000000);
    return;
}

void mission_replay_step(void)
{
    uint32 mode, result, active, flags, argument;
    FUNCTION_MARKER(0x8007F8E4, "ACE3.BIN");
    mode = game_runtime.mission_control.mode;
    if (mode == 128)
    {
        mission_replay_begin();
        return;
    }
    if (mode == 512)
    {
        mission_replay_end();
        return;
    }
    result = mission_replay_update();
    active = game_runtime.playback.demo_mode;
    if (active != 0)
    {
        flags = game_input.pressed;
        if ((flags & 0x10) == 0 && result != 1)
            return;
    }
    else
    {
        flags = game_input.pressed;
        if ((flags & 0x40) == 0)
        {
            if (result != 1)
                return;
            camera_enter_scripted_view();
            mission_replay_reset();
            {
                mission_replay_scan_auxiliary_packets(0);
                return;
            }
        }
    }
    argument = game_runtime.mission_control.argument;
    {
        mission_request_mode(512, argument);
        return;
    }
}

typedef struct
{
    sint16 resource;
    uint8 mission;
    uint8 aircraft;
} MISSION_DEMO_SPEC;

static const MISSION_DEMO_SPEC mission_demo_specs[GAME_DEMO_COUNT] = {
    {465, 1, 10}, {466, 2, 11}, {467, 3, 12}, {468, 4, 13}, {470, 6, 10}, {471, 7, 11}, {472, 8, 12}, {473, 9, 12}, {476, 12, 15}, {477, 13, 16}, {479, 15, 17}, {481, 17, 17}, {485, 21, 4}, {487, 23, 2}, {489, 25, 9}, {494, 30, 8}, {504, 40, 23}, {505, 41, 20}, {508, 44, 22}, {509, 45, 22}, {515, 1, 10}, {517, 3, 12}, {519, 5, 13}, {524, 10, 13}, {525, 11, 14}, {528, 14, 21}, {534, 20, 2}, {536, 22, 5}, {538, 24, 6}, {540, 26, 2}, {541, 27, 4}, {542, 28, 6}, {543, 29, 7}, {549, 35, 28}, {556, 42, 24}, {557, 43, 21}, {564, 50, 22},
};

void mission_demo_select_next(void)
{
    const MISSION_DEMO_SPEC *entry;
    uint32 value, high, quotient, result;
    FUNCTION_MARKER(0x80036130, "ACE3.BIN");
    value = game_runtime.playback.demo_idx;
    game_runtime.playback.demo_idx = (uint8)(value + 1);
    value = game_runtime.playback.demo_idx;
    high = (((uint64)value * 0xBACF914D) >> 32);
    quotient = (high + ((value - high) >> 1)) >> 5;
    game_runtime.playback.demo_idx = (uint8)(value - quotient * 37);
    value = game_runtime.playback.demo_idx;
    entry = &mission_demo_specs[value];
    value = entry->mission;
    game_runtime.selection.mission = (uint16)(value);
    result = entry->aircraft;
    game_runtime.selection.aircraft = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 mission_demo_load_start(void)
{
    uint32 index, resource, bytes, buffer;
    FUNCTION_MARKER(0x800361BC, "ACE3.BIN");
    index = game_runtime.playback.demo_idx;
    resource = (sint32)mission_demo_specs[index].resource;
    bytes = resource_total_size(&resource, 1);
    buffer = arena_alloc(bytes);
    if (buffer == 0)
        abort();
    mission_replay.demo_data = malloc(bytes);
    if (!mission_replay.demo_data)
        abort();
    mission_replay.demo_bytes = bytes;
    mission_flow.loading.pending = 1;
    mission_flow.loading.read_sequence = resource_queue_next_sequence();
    resource_queue_read(resource, buffer, mission_asset_load_complete, (RESOURCE_READ_BUFFER){mission_replay.demo_data, mission_replay.demo_bytes});
    mission_asset_load_poll();
    return 0x04000000;
}

void mission_replay_record_target(uint32 index)
{
    FUNCTION_MARKER(0x8007E890, "ACE3.BIN");
    mission_replay.record_target = index;
    {
        (void)(mission_replay_write_actor_control(4, index, 0, 0));
        return;
    }
}

static uint32 mission_replay_apply_auxiliary_packet(uint32 cursor, uint32 forward, uint32 *complete)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    const uint8 *packet;
    uint32 tag, type;
    FUNCTION_MARKER(0x8007EBB4, "ACE3.BIN");
    *complete = 0;
    packet = mission_replay.ring_data + cursor;
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    type = (tag >> 4) & 7;
    if (type == 0)
    {
        if (((tag >> 7) & 7) == 0)
            *complete = 1;
    }
    else if (type == 4)
    {
        if (forward)
            mission_replay_apply_tile_change(packet);
        else
            mission_replay_restore_tile_change(packet);
    }
    tag = (memcpy(&wire_word_1, packet, sizeof(wire_word_1)), wire_word_1);
    return cursor + ((tag & 15) << 2);
}

static void mission_replay_scan_auxiliary_packets(uint32 forward)
{
    uint32 count, packet, index, complete, end;
    FUNCTION_MARKER(0x8007EC54, "ACE3.BIN");
    count = mission_replay.frame_count;
    packet = mission_replay.oldest_cursor;
    if ((sint32)count <= 0)
        return;
    index = 0;
    do
    {
        complete = 0;
        do
        {
            end = mission_replay.ring_limit;
            if (packet >= end)
                packet = 0;
            packet = mission_replay_apply_auxiliary_packet(packet, forward, &complete);
        } while (complete == 0);
        count = mission_replay.frame_count;
        ++index;
    } while ((sint32)index < (sint32)count);
}

static uint8 *mission_replay_reserve_words(uint32 words)
{
    uint32 start, end, next, read, release;
    FUNCTION_MARKER(0x8007DF28, "ACE3.BIN");
    if (mission_replay.ring_data == NULL)
        abort();
    start = mission_replay.write_cursor;
    end = mission_replay.ring_limit;
    next = start + (words << 2);
    read = mission_replay.oldest_cursor;
    release = 0;
    if (next >= end)
    {
        next = 0;
        if (!(read < start && next < read))
            release = 1;
    }
    else if (read >= start && next >= read && mission_replay.frame_count != 0)
        release = 1;
    if (release)
        mission_replay_release_words(words);
    mission_replay.write_cursor = next;
    return mission_replay.ring_data + start;
}

static void mission_replay_release_words(uint32 words)
{
    uint32 wire_word_0;
    const uint8 *packet;
    uint32 record, end, consumed, value;
    FUNCTION_MARKER(0x8007DE64, "ACE3.BIN");
    record = mission_replay.oldest_cursor;
    end = mission_replay.ring_limit;
    consumed = 0;
    while ((sint32)consumed < (sint32)words)
    {
        packet = mission_replay.ring_data + record;
        if ((memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0) == 1)
        {
            value = mission_replay.frame_count;
            mission_replay.frame_count = value - 1;
        }
        value = mission_replay_release_packet(packet);
        record += value << 2;
        consumed += value;
        if (record >= end)
        {
            record = 0;
        }
    }
    mission_replay.oldest_cursor = record;
    return;
}

static void mission_actor_snapshot_decode_pose(MISSION_ACTOR_SNAPSHOT *snapshot, const uint8 *src)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 angles;
    snapshot->pose.position[0] = (memcpy(&wire_word_0, src + 8, sizeof(wire_word_0)), wire_word_0);
    snapshot->pose.position[1] = (memcpy(&wire_word_1, src + 12, sizeof(wire_word_1)), wire_word_1);
    snapshot->pose.position[2] = (memcpy(&wire_word_2, src + 16, sizeof(wire_word_2)), wire_word_2);
    angles = (memcpy(&wire_word_4, src + 24, sizeof(wire_word_4)), wire_word_4);
    snapshot->pose.angles[0] = (uint16)angles;
    snapshot->pose.angles[1] = (uint16)(angles >> 16);
    angles = (memcpy(&wire_word_5, src + 28, sizeof(wire_word_5)), wire_word_5);
    snapshot->pose.angles[2] = (uint16)angles;
}

static void mission_actor_snapshot_decode(MISSION_ACTOR_SNAPSHOT *snapshot, const uint8 *src)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 model;
    snapshot->flags = (memcpy(&wire_word_0, src, sizeof(wire_word_0)), wire_word_0);
    model = (memcpy(&wire_word_1, src + 4, sizeof(wire_word_1)), wire_word_1);
    snapshot->model = (uint8)model;
    snapshot->render_flags = (uint8)(model >> 8);
    snapshot->parameter = (uint16)(model >> 16);
    mission_actor_snapshot_decode_pose(snapshot, src);
}

static void mission_replay_apply_tile_change(const uint8 *packet)
{
    uint32 wire_word_0;
    uint32 tag, kind, index;
    FUNCTION_MARKER(0x8007EB50, "ACE3.BIN");
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    kind = packet[4];
    index = packet[5];
    scene_terrain_change_tile(kind, index, tag >> 23);
}

static void mission_replay_restore_tile_change(const uint8 *packet)
{
    uint32 wire_word_0;
    uint32 tag, kind, index;
    FUNCTION_MARKER(0x8007EB80, "ACE3.BIN");
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    kind = packet[4];
    index = packet[5];
    scene_terrain_change_tile(kind, index, (tag >> 14) & 0x1FF);
}

uint32 mission_demo_load_finish(void)
{
    FUNCTION_MARKER(0x8003623C, "ACE3.BIN");
    mission_asset_load_poll();
    mission_replay_bind_resource(mission_replay.demo_data, mission_replay.demo_bytes);
    free(mission_replay.demo_data);
    mission_replay.demo_data = NULL;
    mission_replay.demo_bytes = 0;
    game_runtime.mission_control.mode = (uint32)(128);
    game_runtime.mission_control.argument = (uint32)(0);
    return mission_apply_mode_transition(0, 0);
}

void mission_replay_record_particle_spawn(uint32 index, uint32 extra, const NATIVE_POSE *pose, const uint32 *velocity)
{
    FUNCTION_MARKER(0x8007E8C4, "ACE3.BIN");
    mission_replay_write_effect_pose(0, index, 0, extra, pose, velocity);
}

void mission_replay_record_particle_expiration(uint32 index)
{
    FUNCTION_MARKER(0x8007E900, "ACE3.BIN");
    mission_replay_write_effect_pose(1, index, 0, 0, NULL, NULL);
}

void mission_replay_record_aircraft_effect(uint32 kind, uint32 value)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint8 *packet;
    uint32 tag;
    FUNCTION_MARKER(0x8007EFBC, "ACE3.BIN");
    packet = mission_replay_reserve_words(1);
    kind = (kind & 255) << 9;
    value = (value & 0xFFF) << 17;
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    tag = (((tag & 0xFFFFFFF0) | 1) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    tag = (tag & 0xFFFE01FF) | kind;
    tag = (tag & 0xE001FFFF) | value;
    (wire_word_1 = (tag), (void)memcpy(packet, &wire_word_1, sizeof(wire_word_1)));
}

void mission_replay_record_tile_change(uint32 kind, uint32 index, uint32 value, uint32 previous)
{
    FUNCTION_MARKER(0x8007EB30, "ACE3.BIN");
    mission_replay_write_tile_change(kind, index, value, previous);
}

static void mission_replay_write_tile_change(uint32 kind, uint32 index, uint32 value, uint32 previous)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint8 *packet;
    uint32 tag;
    FUNCTION_MARKER(0x8007E530, "ACE3.BIN");
    packet = mission_replay_reserve_words(2);
    previous = (previous & 0x1FF) << 14;
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    value <<= 23;
    packet[4] = (kind);
    packet[5] = (index);
    tag = (((tag & 0xFFFFFFF0) | 2) & 0xFFFFFF8F) | 0x40;
    tag &= 0x3FC2;
    (wire_word_1 = (tag | previous | value), (void)memcpy(packet, &wire_word_1, sizeof(wire_word_1)));
}

void mission_replay_record_effect_pose(uint32 index, uint32 value, const NATIVE_POSE *pose)
{
    FUNCTION_MARKER(0x8007E930, "ACE3.BIN");
    mission_replay_write_effect_pose(2, index, value, 0, pose, NULL);
}

void mission_replay_record_effect_hide(uint32 index)
{
    FUNCTION_MARKER(0x8007E968, "ACE3.BIN");
    mission_replay_write_effect_pose(3, index, 0, 0, NULL, NULL);
}

static void mission_replay_write_effect_pose(uint32 action, uint32 index, uint32 value, uint32 extra, const NATIVE_POSE *pose, const uint32 *velocity)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint16 wire_halfword_13;
    uint32 wire_word_14;
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint8 *target;
    uint32 flags, x, y, z, ax, ay, az, packed_x, packed_y, packed_z;
    FUNCTION_MARKER(0x8007E2B0, "ACE3.BIN");
    if (action == 1 || action == 3)
    {
        target = mission_replay_reserve_words(1);
        flags = (memcpy(&wire_word_0, target, sizeof(wire_word_0)), wire_word_0);
        flags = (flags & 0xFFFF0000) | (action == 1 ? 0xA1 : 0x1A1) | ((index & 0x7F) << 9);
        (wire_word_1 = (flags), (void)memcpy(target, &wire_word_1, sizeof(wire_word_1)));
        return;
    }
    if (action == 0)
    {
        target = mission_replay_reserve_words(6);
        (wire_word_2 = (extra), (void)memcpy(target + 20, &wire_word_2, sizeof(wire_word_2)));
        flags = (memcpy(&wire_word_3, target, sizeof(wire_word_3)), wire_word_3);
        (wire_word_4 = ((flags & 0xFFFF0000) | 0x26 | ((index & 0x7F) << 9)), (void)memcpy(target, &wire_word_4, sizeof(wire_word_4)));
        x = velocity[0];
        z = velocity[2];
        y = velocity[1];
        (wire_halfword_5 = (((sint32)x >> 6)), (void)memcpy(target + 2, &wire_halfword_5, sizeof(wire_halfword_5)));
        (wire_halfword_6 = (((sint32)z >> 6)), (void)memcpy(target + 6, &wire_halfword_6, sizeof(wire_halfword_6)));
        (wire_halfword_7 = (((sint32)y >> 6)), (void)memcpy(target + 4, &wire_halfword_7, sizeof(wire_halfword_7)));
        x = pose->position[0];
        y = pose->position[1];
        ax = pose->angles[0];
        z = pose->position[2];
        ay = pose->angles[1];
        az = pose->angles[2];
        packed_x = ((ax >> 2) & 0xFFF) | (x & 0xFFFFF000);
        packed_y = ((ay >> 2) & 0xFFF) | (y & 0xFFFFF000);
        (wire_word_8 = (packed_x), (void)memcpy(target + 8, &wire_word_8, sizeof(wire_word_8)));
        (wire_word_9 = (packed_y), (void)memcpy(target + 12, &wire_word_9, sizeof(wire_word_9)));
        packed_z = ((az >> 2) & 0xFFF) | (z & 0xFFFFF000);
        (wire_word_10 = (packed_z), (void)memcpy(target + 16, &wire_word_10, sizeof(wire_word_10)));
        return;
    }
    if (action == 2)
    {
        target = mission_replay_reserve_words(4);
        flags = (memcpy(&wire_word_11, target, sizeof(wire_word_11)), wire_word_11);
        (wire_word_12 = ((flags & 0xFFFF0000) | 0x124 | ((index & 0x7F) << 9)), (void)memcpy(target, &wire_word_12, sizeof(wire_word_12)));
        (wire_halfword_13 = (value), (void)memcpy(target + 2, &wire_halfword_13, sizeof(wire_halfword_13)));
        ax = pose->angles[0];
        x = pose->position[0];
        ay = pose->angles[1];
        az = pose->angles[2];
        y = pose->position[1];
        z = pose->position[2];
        packed_x = ((ax >> 2) & 0xFFF) | (x & 0xFFFFF000);
        packed_y = ((ay >> 2) & 0xFFF) | (y & 0xFFFFF000);
        (wire_word_14 = (packed_x), (void)memcpy(target + 4, &wire_word_14, sizeof(wire_word_14)));
        (wire_word_15 = (packed_y), (void)memcpy(target + 8, &wire_word_15, sizeof(wire_word_15)));
        packed_z = ((az >> 2) & 0xFFF) | (z & 0xFFFFF000);
        (wire_word_16 = (packed_z), (void)memcpy(target + 12, &wire_word_16, sizeof(wire_word_16)));
        return;
    }
}

static void mission_replay_write_actor_control(uint32 action, uint32 index, uint32 value, uint32 extra)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint16 wire_halfword_4;
    uint32 wire_word_5;
    uint8 *target;
    uint32 packed, result;
    FUNCTION_MARKER(0x8007DFC4, "ACE3.BIN");
    target = mission_replay_reserve_words(1);
    if (action >= 5)
        return;
    if (action == 0)
    {
        (wire_word_0 = (1), (void)memcpy(target, &wire_word_0, sizeof(wire_word_0)));
        return;
    }
    packed = (memcpy(&wire_word_1, target, sizeof(wire_word_1)), wire_word_1);
    packed = ((packed & 0xFFFFFFF0) | 1) & 0xFFFFFF8F;
    packed = (packed & 0xFFFFFC7F) | ((action & 7) << 7);
    if (action == 4)
    {
        (wire_word_2 = (packed), (void)memcpy(target, &wire_word_2, sizeof(wire_word_2)));
        if ((sint32)index < 0)
            index = 0;
        result = (index & 63) << 10;
        (wire_word_3 = ((packed & 0xFFFF03FF) | result), (void)memcpy(target, &wire_word_3, sizeof(wire_word_3)));
        (wire_halfword_4 = (0), (void)memcpy(target + 2, &wire_halfword_4, sizeof(wire_halfword_4)));
        {
            (void)(uint32)(result);
            return;
        }
    }
    packed = (packed & 0xFFFF03FF) | ((index & 63) << 10);
    if (action == 1)
    {
        packed = (packed & 0xFFFEFFFF) | ((value & 1) << 16);
        packed &= 0x1FFFF;
    }
    else if (action == 3)
        packed = (packed & 0xFF81) | ((value & 0x7FFF) << 16);
    (wire_word_5 = (packed), (void)memcpy(target, &wire_word_5, sizeof(wire_word_5)));
    if (action == 2)
    {
        target[2] = (value);
        target[3] = (extra);
    }
    {
        (void)(uint32)(packed);
        return;
    }
}

static void mission_replay_write_actor_pose(uint32 index, const NATIVE_POSE *pose)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint8 *target;
    uint32 value, a, b, c, x, y, z;
    FUNCTION_MARKER(0x8007E1CC, "ACE3.BIN");
    target = mission_replay_reserve_words(4);
    value = (memcpy(&wire_word_0, target, sizeof(wire_word_0)), wire_word_0);
    value = ((value & 0xFFFFFFF0) | 4) & 0xFFFFFF8F;
    value = (((value | 16) & 0xFFFFFE7F) & 0xFFFF81FF) | ((index & 63) << 9);
    (wire_word_1 = (value), (void)memcpy(target, &wire_word_1, sizeof(wire_word_1)));
    a = pose->angles[0];
    x = pose->position[0];
    b = pose->angles[1];
    c = pose->angles[2];
    a = ((a >> 2) & 0xFFF) | (x & 0xFFFFF000);
    y = pose->position[1];
    b = ((b >> 2) & 0xFFF) | (y & 0xFFFFF000);
    z = pose->position[2];
    (wire_word_2 = (a), (void)memcpy(target + 4, &wire_word_2, sizeof(wire_word_2)));
    (wire_word_3 = (b), (void)memcpy(target + 8, &wire_word_3, sizeof(wire_word_3)));
    value = z & 0xFFFFF000;
    (wire_word_4 = (((c >> 2) & 0xFFF) | value), (void)memcpy(target + 12, &wire_word_4, sizeof(wire_word_4)));
}

static void mission_replay_apply_effect_pose(const uint8 *packet)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint16 wire_halfword_13;
    NATIVE_POSE pose;
    uint32 vector[3], packed, kind, a, b, c, flags, mode;
    FUNCTION_MARKER(0x8007E998, "ACE3.BIN");
    packed = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    kind = (packed >> 7) & 3;
    switch (kind)
    {
        case 0:
            a = (sint16)(memcpy(&wire_halfword_1, packet + 2, sizeof(wire_halfword_1)), wire_halfword_1);
            c = (sint16)(memcpy(&wire_halfword_2, packet + 6, sizeof(wire_halfword_2)), wire_halfword_2);
            vector[0] = a << 6;
            b = (sint16)(memcpy(&wire_halfword_3, packet + 4, sizeof(wire_halfword_3)), wire_halfword_3);
            vector[1] = b << 6;
            vector[2] = c << 6;
            a = (memcpy(&wire_word_4, packet + 8, sizeof(wire_word_4)), wire_word_4);
            b = (memcpy(&wire_word_5, packet + 12, sizeof(wire_word_5)), wire_word_5);
            c = (memcpy(&wire_word_6, packet + 16, sizeof(wire_word_6)), wire_word_6);
            pose.position[0] = a & 0xFFFFF000;
            pose.position[1] = b & 0xFFFFF000;
            pose.position[2] = c & 0xFFFFF000;
            pose.angles[2] = (sint32)(c << 20) >> 18;
            pose.angles[0] = (sint32)(a << 20) >> 18;
            pose.angles[1] = (sint32)(b << 20) >> 18;
            flags = (memcpy(&wire_word_7, packet, sizeof(wire_word_7)), wire_word_7);
            mode = (memcpy(&wire_word_8, packet + 20, sizeof(wire_word_8)), wire_word_8);
            particle_initialize((flags >> 9) & 127, mode, &pose, vector);
            return;
        case 1:
            particle_expire((packed >> 9) & 127);
            return;
        case 2:
            a = (memcpy(&wire_word_9, packet + 4, sizeof(wire_word_9)), wire_word_9);
            b = (memcpy(&wire_word_10, packet + 8, sizeof(wire_word_10)), wire_word_10);
            c = (memcpy(&wire_word_11, packet + 12, sizeof(wire_word_11)), wire_word_11);
            pose.position[0] = a & 0xFFFFF000;
            pose.position[1] = b & 0xFFFFF000;
            pose.position[2] = c & 0xFFFFF000;
            pose.angles[0] = (sint32)(a << 20) >> 18;
            pose.angles[1] = (sint32)(b << 20) >> 18;
            pose.angles[2] = (sint32)(c << 20) >> 18;
            flags = (memcpy(&wire_word_12, packet, sizeof(wire_word_12)), wire_word_12);
            mode = (memcpy(&wire_halfword_13, packet + 2, sizeof(wire_halfword_13)), wire_halfword_13);
            effect_object_publish_pose((flags >> 9) & 127, mode, &pose);
            return;
        default:
            effect_object_hide((packed >> 9) & 127);
            return;
    }
}

static uint8 *mission_replay_write_segment_effect(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint8 *packet;
    uint32 tag, a, b, c, x, y, z, high;
    FUNCTION_MARKER(0x8007EEAC, "ACE3.BIN");
    packet = mission_replay_reserve_words(5);
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    a = (memcpy(&wire_word_1, packet + 4, sizeof(wire_word_1)), wire_word_1);
    b = (memcpy(&wire_word_2, packet + 8, sizeof(wire_word_2)), wire_word_2);
    c = (memcpy(&wire_word_3, packet + 12, sizeof(wire_word_3)), wire_word_3);
    tag = (((tag & 0xFFFFFFF0) | 5) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    a &= 0xFFF00000;
    b &= 0xFFF00000;
    c &= 0xFFF00000;
    (wire_word_4 = (tag), (void)memcpy(packet, &wire_word_4, sizeof(wire_word_4)));
    packet[3] = (kind);
    {
        x = native_position[0];
        y = native_position[1];
        z = native_position[2];
    }
    a |= x >> 12;
    b |= y >> 12;
    c |= z >> 12;
    (wire_word_5 = (a), (void)memcpy(packet + 4, &wire_word_5, sizeof(wire_word_5)));
    (wire_word_6 = (c), (void)memcpy(packet + 12, &wire_word_6, sizeof(wire_word_6)));
    a &= 0xFFFFF;
    (wire_word_7 = (b), (void)memcpy(packet + 8, &wire_word_7, sizeof(wire_word_7)));
    b &= 0xFFFFF;
    {
        x = native_vector[0];
        y = native_vector[1];
        z = native_vector[2];
    }
    a |= ((uint32)((sint32)x >> 12)) << 20;
    b |= ((uint32)((sint32)y >> 12)) << 20;
    high = ((uint32)((sint32)z >> 12)) << 20;
    c = (memcpy(&wire_word_8, packet + 12, sizeof(wire_word_8)), wire_word_8);
    (wire_word_9 = (a), (void)memcpy(packet + 4, &wire_word_9, sizeof(wire_word_9)));
    (wire_word_10 = (b), (void)memcpy(packet + 8, &wire_word_10, sizeof(wire_word_10)));
    (wire_word_11 = ((c & 0xFFFFF) | high), (void)memcpy(packet + 12, &wire_word_11, sizeof(wire_word_11)));
    return packet;
}

static uint8 *mission_replay_write_position_effect(uint32 kind, const uint32 *native_position)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint8 *target;
    uint32 flags, a, b, x, y, z;
    FUNCTION_MARKER(0x8007ECE8, "ACE3.BIN");
    target = mission_replay_reserve_words(3);
    flags = (memcpy(&wire_word_0, target, sizeof(wire_word_0)), wire_word_0);
    target[4] = (kind);
    a = (memcpy(&wire_word_1, target + 4, sizeof(wire_word_1)), wire_word_1) & 0xFFF;
    b = (memcpy(&wire_word_2, target + 8, sizeof(wire_word_2)), wire_word_2) & 0xFFF;
    flags = (flags & 0xFFFFFE00) | 0x33;
    (wire_word_3 = (flags), (void)memcpy(target, &wire_word_3, sizeof(wire_word_3)));
    x = (native_position[0]);
    y = (native_position[1]);
    z = (native_position[2]);
    (wire_word_4 = ((flags & 0xFFF) | (x & 0xFFFFF000)), (void)memcpy(target, &wire_word_4, sizeof(wire_word_4)));
    (wire_word_5 = (a | (y & 0xFFFFF000)), (void)memcpy(target + 4, &wire_word_5, sizeof(wire_word_5)));
    (wire_word_6 = (b | (z & 0xFFFFF000)), (void)memcpy(target + 8, &wire_word_6, sizeof(wire_word_6)));
    return target;
}

static void mission_replay_spawn_position(uint32 kind, const uint8 *position)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 vector[3], a, b, c, value, mode;
    FUNCTION_MARKER(0x8007F9E8, "ACE3.BIN");
    a = (memcpy(&wire_word_0, position, sizeof(wire_word_0)), wire_word_0);
    b = (memcpy(&wire_word_1, position + 4, sizeof(wire_word_1)), wire_word_1);
    c = (memcpy(&wire_word_2, position + 8, sizeof(wire_word_2)), wire_word_2);
    vector[0] = a & 0xFFFFF000;
    vector[1] = b & 0xFFFFF000;
    vector[2] = c & 0xFFFFF000;
    switch (kind)
    {
        case 0:
            value = (memcpy(&wire_word_3, position + 8, sizeof(wire_word_3)), wire_word_3);
            {
                (void)(effect_spawn_position_type0((value >> 4) & 255, vector));
                return;
            }
        case 1:
            value = (memcpy(&wire_word_4, position + 8, sizeof(wire_word_4)), wire_word_4);
            {
                (void)(effect_spawn_material_hit(value & 15, (value >> 4) & 255, vector));
                return;
            }
        case 2:
            value = (memcpy(&wire_word_5, position + 4, sizeof(wire_word_5)), wire_word_5);
            {
                (void)(effect_spawn_position_type2((value >> 8) & 15, vector));
                return;
            }
        case 3:
            value = (memcpy(&wire_word_6, position + 8, sizeof(wire_word_6)), wire_word_6);
            {
                (void)(effect_drift_emit_scaled(vector, (value >> 4) & 255));
                return;
            }
        case 4:
            value = (memcpy(&wire_word_7, position + 8, sizeof(wire_word_7)), wire_word_7);
            {
                (void)(effect_spawn_callback_kind((value >> 4) & 255, vector));
                return;
            }
        case 5:
            value = (memcpy(&wire_word_8, position + 8, sizeof(wire_word_8)), wire_word_8);
            {
                (void)(effect_spawn_burst((value >> 4) & 255, vector));
                return;
            }
        case 6:
            value = (memcpy(&wire_word_9, position + 8, sizeof(wire_word_9)), wire_word_9);
            mode = (memcpy(&wire_word_10, position + 4, sizeof(wire_word_10)), wire_word_10);
            {
                (void)(effect_emit_position_style((value >> 4) & 255, (mode >> 8) & 15, vector));
                return;
            }
        default:
            return;
    }
}

static void mission_replay_apply_effect(const uint8 *packet)
{
    uint32 wire_word_0;
    uint32 tag, kind, value;
    FUNCTION_MARKER(0x8007F048, "ACE3.BIN");
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    kind = tag & 15;
    if (kind == 3)
    {
        value = packet[4];
        {
            mission_replay_spawn_position(value, packet);
            return;
        }
    }
    if (kind < 4)
    {
        if (kind == 1)
        {
            effect_aircraft_start((tag >> 9) & 255, (tag >> 17) & 0xFFF);
            return;
        }
        return;
    }
    if (kind == 4)
    {
        value = packet[3];
        {
            mission_replay_spawn_vector(value, packet);
            return;
        }
    }
    if (kind == 5)
    {
        value = packet[3];
        {
            mission_replay_spawn_segment(value, packet);
            return;
        }
    }
    return;
}

static void mission_replay_spawn_segment(uint32 kind, const uint8 *packet)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 a, b, c, position[3], vector[3], packed, shift, scale, flag;
    FUNCTION_MARKER(0x8007FCA0, "ACE3.BIN");
    a = (memcpy(&wire_word_0, packet + 4, sizeof(wire_word_0)), wire_word_0);
    b = (memcpy(&wire_word_1, packet + 8, sizeof(wire_word_1)), wire_word_1);
    c = (memcpy(&wire_word_2, packet + 12, sizeof(wire_word_2)), wire_word_2);
    position[0] = a << 12;
    position[1] = b << 12;
    position[2] = c << 12;
    a = (memcpy(&wire_word_3, packet + 4, sizeof(wire_word_3)), wire_word_3);
    c = (memcpy(&wire_word_4, packet + 12, sizeof(wire_word_4)), wire_word_4);
    b = (memcpy(&wire_word_5, packet + 8, sizeof(wire_word_5)), wire_word_5);
    vector[0] = ((uint32)((sint32)a >> 20)) << 12;
    vector[2] = ((uint32)((sint32)c >> 20)) << 12;
    vector[1] = ((uint32)((sint32)b >> 20)) << 12;
    if (kind == 0)
    {
        packed = (memcpy(&wire_word_6, packet + 16, sizeof(wire_word_6)), wire_word_6);
        scale = (packed & 0xFFF) | (((packed >> 12) & 0xFFF) << 16);
        {
            effect_spawn_surface_hit(scale, position, vector);
            return;
        }
    }
    if (kind != 1)
        return;
    packed = (memcpy(&wire_word_7, packet + 16, sizeof(wire_word_7)), wire_word_7);
    flag = packet[2];
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
    {
        effect_segment_emit(scale, flag, position, vector);
        return;
    }
}

static void mission_replay_spawn_vector(uint32 kind, const uint8 *packet)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 position[3], vector[3], a, b, c, value;
    FUNCTION_MARKER(0x8007FB24, "ACE3.BIN");
    a = (memcpy(&wire_word_0, packet + 4, sizeof(wire_word_0)), wire_word_0);
    b = (memcpy(&wire_word_1, packet + 8, sizeof(wire_word_1)), wire_word_1);
    c = (memcpy(&wire_word_2, packet + 12, sizeof(wire_word_2)), wire_word_2);
    position[0] = a << 12;
    position[1] = b << 12;
    position[2] = c << 12;
    a = (memcpy(&wire_word_3, packet + 4, sizeof(wire_word_3)), wire_word_3);
    b = (memcpy(&wire_word_4, packet + 8, sizeof(wire_word_4)), wire_word_4);
    c = (memcpy(&wire_word_5, packet + 12, sizeof(wire_word_5)), wire_word_5);
    vector[0] = (uint32)((sint32)a >> 20) << 12;
    vector[1] = (uint32)((sint32)b >> 20) << 12;
    vector[2] = (uint32)((sint32)c >> 20) << 12;
    switch (kind)
    {
        case 0:
        {
            (void)(effect_spawn_debris(position, vector));
            return;
        }
        case 1:
            value = packet[2];
            {
                (void)(effect_spawn_drift_group(value, position, vector));
                return;
            }
        case 2:
            value = packet[2];
            {
                (void)(effect_emit_vector_style(value, position, vector));
                return;
            }
        case 3:
        {
            (void)(effect_spawn_vector_type3(position, vector));
            return;
        }
        case 4:
            value = (memcpy(&wire_word_6, packet, sizeof(wire_word_6)), wire_word_6);
            {
                (void)(effect_spawn_vector_type4((value >> 9) & 127, position, vector));
                return;
            }
        case 5:
            value = (memcpy(&wire_word_7, packet, sizeof(wire_word_7)), wire_word_7);
            {
                (void)(effect_drift_emit_velocity_burst((value >> 9) & 127, position, vector));
                return;
            }
        case 6:
            value = packet[2];
            {
                (void)(effect_emit_secondary_drift(value, position, vector));
                return;
            }
        case 7:
        {
            (void)(effect_spawn_vector_type7(position, vector));
            return;
        }
        case 8:
        {
            (void)(effect_spawn_vector_type8(position, vector));
            return;
        }
        case 9:
        {
            (void)(effect_burst_start(position, vector));
            return;
        }
        default:
            return;
    }
}

static uint8 *mission_replay_write_vector_effect(uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint8 *packet;
    uint32 tag, a, b, c, x, y, z, high;
    FUNCTION_MARKER(0x8007ED9C, "ACE3.BIN");
    packet = mission_replay_reserve_words(4);
    tag = (memcpy(&wire_word_0, packet, sizeof(wire_word_0)), wire_word_0);
    a = (memcpy(&wire_word_1, packet + 4, sizeof(wire_word_1)), wire_word_1);
    b = (memcpy(&wire_word_2, packet + 8, sizeof(wire_word_2)), wire_word_2);
    c = (memcpy(&wire_word_3, packet + 12, sizeof(wire_word_3)), wire_word_3);
    tag = (((tag & 0xFFFFFFF0) | 4) & 0xFFFFFF8F) | 0x30;
    tag &= 0xFFFFFE7F;
    a &= 0xFFF00000;
    b &= 0xFFF00000;
    c &= 0xFFF00000;
    (wire_word_4 = (tag), (void)memcpy(packet, &wire_word_4, sizeof(wire_word_4)));
    packet[3] = (kind);
    x = native_position[0];
    y = native_position[1];
    z = native_position[2];
    a |= x >> 12;
    b |= y >> 12;
    c |= z >> 12;
    (wire_word_5 = (a), (void)memcpy(packet + 4, &wire_word_5, sizeof(wire_word_5)));
    (wire_word_6 = (c), (void)memcpy(packet + 12, &wire_word_6, sizeof(wire_word_6)));
    a &= 0xFFFFF;
    (wire_word_7 = (b), (void)memcpy(packet + 8, &wire_word_7, sizeof(wire_word_7)));
    b &= 0xFFFFF;
    x = native_vector[0];
    y = native_vector[1];
    z = native_vector[2];
    a |= ((uint32)((sint32)x >> 12)) << 20;
    b |= ((uint32)((sint32)y >> 12)) << 20;
    high = ((uint32)((sint32)z >> 12)) << 20;
    c = (memcpy(&wire_word_8, packet + 12, sizeof(wire_word_8)), wire_word_8);
    (wire_word_9 = (a), (void)memcpy(packet + 4, &wire_word_9, sizeof(wire_word_9)));
    (wire_word_10 = (b), (void)memcpy(packet + 8, &wire_word_10, sizeof(wire_word_10)));
    (wire_word_11 = ((c & 0xFFFFF) | high), (void)memcpy(packet + 12, &wire_word_11, sizeof(wire_word_11)));
    return packet;
}

void mission_replay_record_position_effect(uint32 kind, const uint32 *native_position, uint32 variant, uint32 parameter)
{
    uint8 *packet;
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 value, flags;
    packet = mission_replay_write_position_effect(kind, native_position);
    switch (kind)
    {
        case 1:
            value = (memcpy(&wire_word_0, packet + 8, sizeof(wire_word_0)), wire_word_0);
            value = (value & 0xFFFFF00F) | ((variant & 255) << 4);
            value = (value & 0xFFFFFFF0) | parameter;
            (wire_word_1 = (value), (void)memcpy(packet + 8, &wire_word_1, sizeof(wire_word_1)));
            break;
        case 2:
            value = (memcpy(&wire_word_0, packet + 4, sizeof(wire_word_0)), wire_word_0);
            (wire_word_1 = ((value & 0xFFFFF0FF) | ((variant & 15) << 8)), (void)memcpy(packet + 4, &wire_word_1, sizeof(wire_word_1)));
            break;
        case 3:
            value = (memcpy(&wire_word_0, packet + 8, sizeof(wire_word_0)), wire_word_0);
            (wire_word_1 = ((value & 0xFFFFF00F) | ((variant & 0xFF) << 4)), (void)memcpy(packet + 8, &wire_word_1, sizeof(wire_word_1)));
            break;
        case 6:
            value = (memcpy(&wire_word_0, packet + 8, sizeof(wire_word_0)), wire_word_0);
            flags = (memcpy(&wire_word_1, packet + 4, sizeof(wire_word_1)), wire_word_1);
            value = (value & 0xFFFFF00F) | ((variant & 0xFF) << 4);
            flags = (flags & 0xFFFFF0FF) | ((parameter & 15) << 8);
            (wire_word_2 = (flags), (void)memcpy(packet + 4, &wire_word_2, sizeof(wire_word_2)));
            (wire_word_3 = (value), (void)memcpy(packet + 8, &wire_word_3, sizeof(wire_word_3)));
            break;
        default:
            value = (memcpy(&wire_word_0, packet + 8, sizeof(wire_word_0)), wire_word_0);
            value = (value & 0xFFFFF00F) | ((variant & 255) << 4);
            (wire_word_1 = (value), (void)memcpy(packet + 8, &wire_word_1, sizeof(wire_word_1)));
            break;
    }
}

void mission_replay_record_vector_effect(uint32 kind, const uint32 *native_position, const uint32 *native_vector, uint32 variant)
{
    uint8 *packet;
    uint32 value, native_buffer_value_0, native_buffer_value_1;
    packet = mission_replay_write_vector_effect(kind, native_position, native_vector);
    if (kind == 4)
    {
        value = (memcpy(&native_buffer_value_0, packet, sizeof(native_buffer_value_0)), native_buffer_value_0);
        (native_buffer_value_1 = ((value & 0xFFFF01FF) | ((variant & 127) << 9)), (void)memcpy(packet, &native_buffer_value_1, sizeof(native_buffer_value_1)));
    }
    else if (kind == 0 || kind == 1 || kind == 2 || kind == 6 || kind == 7)
    {
        packet[2] = (variant);
    }
}

void mission_replay_record_surface_hit(uint32 variant, uint32 direction, const uint32 *native_position, const uint32 *native_vector)
{
    uint8 *packet;
    uint32 value, native_buffer_value_0, native_buffer_value_1;
    packet = mission_replay_write_segment_effect(0, native_position, native_vector);
    value = (memcpy(&native_buffer_value_0, packet + 16, sizeof(native_buffer_value_0)), native_buffer_value_0);
    packet[2] = (variant);
    value = ((value & 0xFFFFF000) | (direction & 0xFFF)) & 0xFF000FFF;
    value |= (((uint32)((sint32)direction >> 16)) & 0xFFF) << 12;
    (native_buffer_value_1 = (value), (void)memcpy(packet + 16, &native_buffer_value_1, sizeof(native_buffer_value_1)));
}

void mission_replay_record_segment(uint32 scale, uint32 variant, const uint32 *native_start, const uint32 *direction, uint32 shift)
{
    uint8 *packet;
    uint32 value, native_buffer_value_0, native_buffer_value_1;
    packet = mission_replay_write_segment_effect(1, native_start, direction);
    value = (memcpy(&native_buffer_value_0, packet + 16, sizeof(native_buffer_value_0)), native_buffer_value_0);
    packet[2] = (variant);
    value = ((value & 0xFFFFF000) | shift) & 0xFF000FFF;
    (native_buffer_value_1 = (value | ((scale & 0xFFF) << 12)), (void)memcpy(packet + 16, &native_buffer_value_1, sizeof(native_buffer_value_1)));
}
