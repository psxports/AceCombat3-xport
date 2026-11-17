#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

#pragma pack(push, 1)

typedef struct
{
    uint16 kind;
    uint8 source_actor;
    uint8 destination_actor;
    uint32 offset[3];
    uint8 unknown16[4];
} MISSION_LINK_RECORD;

#pragma pack(pop)

typedef char mission_link_wire_layout_check[sizeof(MISSION_LINK_RECORD) == 20 && offsetof(MISSION_LINK_RECORD, kind) == 0 && (offsetof(MISSION_LINK_RECORD, offset) + 0 * sizeof(((MISSION_LINK_RECORD *)0)->offset[0])) == 4 && (offsetof(MISSION_LINK_RECORD, offset) + 1 * sizeof(((MISSION_LINK_RECORD *)0)->offset[0])) == 8 && (offsetof(MISSION_LINK_RECORD, offset) + 2 * sizeof(((MISSION_LINK_RECORD *)0)->offset[0])) == 12 && offsetof(MISSION_LINK_RECORD, source_actor) == 2 && offsetof(MISSION_LINK_RECORD, destination_actor) == 3 ? 1 : -1];

enum
{
    MISSION_LINK_CAPACITY = 255
};

typedef struct
{
    uint8 enabled;
    uint8 phase_reversed;
    uint16 kind;
    uint8 source_actor;
    uint8 destination_actor;
    uint32 offset[3];
} MISSION_LINK;

typedef struct
{
    uint32 count;
    MISSION_LINK entries[MISSION_LINK_CAPACITY];
} MISSION_LINK_STATE;

static MISSION_LINK_STATE links;

void mission_links_reset(void)
{
    memset(&links, 0, sizeof(links));
}

static MISSION_LINK *link_at(uint32 index)
{
    if (index >= links.count)
        abort();
    return &links.entries[index];
}

void mission_links_bind(const uint8 *source, uint32 count)
{
    uint32 index;
    const MISSION_LINK_RECORD *record;
    MISSION_LINK *link;

    if (count > MISSION_LINK_CAPACITY)
        abort();
    links.count = count;
    for (index = 0; index < count; ++index)
    {
        link = link_at(index);
        record = (const MISSION_LINK_RECORD *)(source + index * 20);
        link->kind = record->kind;
        link->source_actor = record->source_actor;
        link->destination_actor = record->destination_actor;
        link->offset[0] = record->offset[0];
        link->offset[1] = record->offset[1];
        link->offset[2] = record->offset[2];
        link->enabled = link->kind != 4;
        /* start each new link in its primary phase */
        link->phase_reversed = 0;
    }
}

void mission_links_mark_actors(void)
{
    MISSION_LINK *link;
    uint32 count, index, value;
    uint32 *target;
    FUNCTION_MARKER(0x8004BE34, "ACE3.BIN");
    count = links.count;
    if (count == 0)
        return;
    for (index = 0; index < count; ++index)
    {
        link = link_at(index);
        if (!link->enabled)
            continue;
        target = mission_actor_flags(link->source_actor);
        value = (*target);
        *target = (value | 0x200000);
        target = mission_actor_flags(link->destination_actor);
        value = (*target);
        *target = (value | 0x400000);
    }
}

void mission_links_update_poses(void)
{
    MISSION_LINK *link;
    MISSION_ACTOR *pose_owner_destination;
    MISSION_ACTOR *pose_owner_source;
    MISSION_ACTOR_MOTION *actor_motion_destination;
    MISSION_ACTOR_MOTION *actor_motion_source;

    MISSION_ACTOR_TRAITS *actor_traits_source, *actor_traits_table;
    MISSION_ACTOR_TRAITS *actor_traits_destination;
    uint32 actor_index_scene;
    uint32 actor_index_table;

    uint32 index, count, table, flags, from, to, kind, value, a, b, c;
    uint32 phase, pitch, sine, cosine, product0, product1, owner;
    FUNCTION_MARKER(0x8004BED4, "ACE3.BIN");
    count = links.count;
    if (count == 0)
        return;
    index = 0;
    do
    {
        link = link_at(index);
        if (!link->enabled)
            goto next_record;
        actor_index_scene = 0;
        from = link->source_actor;
        to = link->destination_actor;
        pose_owner_source = mission_actor((actor_index_scene + ((uint32)((sint32)(from * 4) >> 2))));
        actor_motion_source = mission_actor_motion((actor_index_scene + ((uint32)((sint32)(from * 4) >> 2))));
        actor_traits_source = mission_actor_traits((actor_index_scene + ((uint32)((sint32)(from * 4) >> 2))));
        kind = link->kind;
        pose_owner_destination = mission_actor((actor_index_scene + ((uint32)((sint32)(to * 4) >> 2))));
        actor_motion_destination = mission_actor_motion((actor_index_scene + ((uint32)((sint32)(to * 4) >> 2))));
        actor_traits_destination = mission_actor_traits((actor_index_scene + ((uint32)((sint32)(to * 4) >> 2))));
        if (kind == 3)
        {
            if (from == 0)
            {
                value = actor_traits_destination->render_flags;
                actor_traits_destination->model = (uint8)(0);
                actor_traits_destination->render_flags = (uint8)(value | 1);
            }
            value = pose_owner_source->pose.position[0];
            pose_owner_destination->pose.position[0] = (uint32)(value);
            value = pose_owner_source->pose.position[1];
            pose_owner_destination->pose.position[1] = (uint32)(0xFFFBE000 - value);
            value = pose_owner_source->pose.position[2];
            pose_owner_destination->pose.position[2] = (uint32)(value);
            value = pose_owner_source->pose.angles[0];
            pose_owner_destination->pose.angles[0] = (uint16)(0 - value);
            value = pose_owner_source->pose.angles[1];
            pose_owner_destination->pose.angles[1] = (uint16)(value);
            value = pose_owner_source->pose.angles[2];
            pose_owner_destination->pose.angles[2] = (uint16)(0x2000 - value);
            value = actor_traits_source->flags;
            if ((value & 0x10000) == 0)
            {
                value = actor_traits_destination->flags;
                actor_traits_destination->flags = (uint32)(value & 0xFFFEFFFF);
            }
        }
        else if (kind == 4)
        {
            game_pose_transform_offset(&pose_owner_source->pose, pose_owner_destination->pose.position, link->offset);
            value = pose_owner_source->pose.angles[0];
            pose_owner_destination->pose.angles[0] = (uint16)(value);
            value = pose_owner_source->pose.angles[1];
            pose_owner_destination->pose.angles[1] = (uint16)(value);
            value = pose_owner_source->pose.angles[2];
            pose_owner_destination->pose.angles[2] = (uint16)(value + 0x1FFF);
            a = actor_motion_source->velocity[0];
            b = actor_motion_source->velocity[1];
            c = actor_motion_source->velocity[2];
            actor_motion_destination->velocity[0] = (uint32)(a);
            actor_motion_destination->velocity[1] = (uint32)(b);
            actor_motion_destination->velocity[2] = (uint32)(c);
            value = actor_traits_destination->events;
            actor_traits_destination->events = (uint32)(value & 0x7FFFFFFF);
        }
        else if (kind == 5)
        {
            a = (sint16)pose_owner_destination->pose.angles[1];
            b = (sint16)pose_owner_source->pose.angles[1];
            phase = (a - b) & 0x3FFF;
            if ((phase - 0x1C01) < 0x7FF)
                link->phase_reversed ^= 1;
            phase = (sint16)pose_owner_source->pose.angles[1];
            table = 0;
            phase = (phase + 0x1000) & 0x3FFF;
            if (link->phase_reversed)
                phase = (phase + 0x2000) & 0x3FFF;
            pitch = (sint16)pose_owner_source->pose.angles[0];
            sine = game_wave_at(phase * 2);
            pitch &= 0x3FFF;
            cosine = game_wave_at((pitch + 0x1000) * 2);
            product0 = sine * cosine;
            value = game_wave_at((phase + 0x1000) * 2);
            product1 = value * cosine;
            value = pose_owner_source->pose.position[0];
            pose_owner_destination->pose.position[0] = (uint32)(value + (uint32)((sint32)product0 >> 9));
            a = game_wave_at(pitch * 2);
            value = pose_owner_source->pose.position[1];
            pose_owner_destination->pose.position[1] = (uint32)(value + (a << 5));
            value = pose_owner_source->pose.position[2];
            pose_owner_destination->pose.position[2] = (uint32)(value + (uint32)((sint32)product1 >> 9));
            a = (pose_owner_source->pose.angles[0] | ((uint32)pose_owner_source->pose.angles[1] << 16));
            b = (pose_owner_source->pose.angles[2]);
            {
                uint32 pose_word = (a);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (b);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            value = actor_traits_source->render_flags;
            flags = actor_traits_destination->render_flags;
            actor_traits_destination->render_flags = (uint8)((value & 8) ? flags | 8 : flags & 0xF7);
            if (flight_dynamics.events & 0x4000)
            {
                value = hud_control.locked_target_idx;
                owner = 0xFFFFFFFF;
                if (value != 255)
                {
                    value = hud_control.selected_target_idx;
                    actor_index_table = 0;
                    actor_traits_table = mission_actor_traits((actor_index_table + ((uint32)((sint32)(value * 4) >> 2))));
                    owner = actor_traits_table->flags;
                }
                flags = actor_traits_source->flags;
                effect_object_create_variant_1(flags, owner, 0x102, &pose_owner_destination->pose, actor_motion_source->velocity);
            }
        }
        else
        {
            game_pose_transform_offset(&pose_owner_source->pose, pose_owner_destination->pose.position, link->offset);
            if (kind == 6)
            {
            }
            a = (pose_owner_source->pose.angles[0] | ((uint32)pose_owner_source->pose.angles[1] << 16));
            b = (pose_owner_source->pose.angles[2]);
            {
                uint32 pose_word = (a);
                pose_owner_destination->pose.angles[0] = (uint16)pose_word;
                pose_owner_destination->pose.angles[1] = (uint16)(pose_word >> 16);
            }
            {
                uint32 pose_word = (b);
                pose_owner_destination->pose.angles[2] = (uint16)pose_word;
            }
            if (kind == 6)
            {
                value = (uint16)game_runtime.mission_control.ticks;
                pose_owner_destination->pose.angles[2] = (uint16)((value & 0x3F) << 8);
            }
        }
    next_record:
        count = links.count;
        ++index;
    } while ((sint32)index < (sint32)count);
    return;
}

void mission_link_activate(uint32 index)
{
    MISSION_LINK *link;
    uint32 flags, member, result;
    uint32 *actor_flags;
    FUNCTION_MARKER(0x8004C2C8, "ACE3.BIN");
    link = link_at(index);
    link->enabled = 1;
    member = link->source_actor;
    actor_flags = mission_actor_flags(member);
    flags = (*actor_flags);
    *actor_flags = (flags | 0x200000);
    member = link->destination_actor;
    actor_flags = mission_actor_flags(member);
    result = (*actor_flags) | 0x400000;
    *actor_flags = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_link_deactivate(uint32 index)
{
    MISSION_LINK *link;
    uint32 flags, member, result;
    uint32 *actor_flags;
    FUNCTION_MARKER(0x8004C338, "ACE3.BIN");
    link = link_at(index);
    result = link->enabled;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    link->enabled = 0;
    member = link->source_actor;
    actor_flags = mission_actor_flags(member);
    flags = (*actor_flags);
    *actor_flags = (flags & 0xFFDFFFFF);
    member = link->destination_actor;
    actor_flags = mission_actor_flags(member);
    result = (*actor_flags) & 0xFFBFFFFF;
    *actor_flags = (result);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_links_update_health(void)
{
    MISSION_LINK *link;
    MISSION_ACTOR_TRAITS *actor_traits_first, *actor_traits_second;
    uint32 actor_index_table;

    uint32 count, index = 0, first_index, second_index, value;
    FUNCTION_MARKER(0x8004C3C4, "ACE3.BIN");
    count = links.count;
    if (count == 0)
    {
        (void)(uint32)(count);
        return;
    }
    do
    {
        link = link_at(index);
        if (link->enabled)
        {
            actor_index_table = 0;
            first_index = link->source_actor;
            second_index = link->destination_actor;
            actor_traits_first = mission_actor_traits((actor_index_table + ((uint32)((sint32)(first_index * 4) >> 2))));
            value = (sint16)actor_traits_first->health;
            actor_traits_second = mission_actor_traits((actor_index_table + ((uint32)((sint32)(second_index * 4) >> 2))));
            if ((sint32)value <= 0 && (sint16)actor_traits_second->health > 0)
            {
                if ((~(*mission_actor_flags(second_index)) & 1) != 0 && (actor_traits_second->flags & 0x10000) != 0)
                {
                    if (second_index < 32)
                        mission_actor_begin_deactivation(second_index);
                    else
                        mission_secondary_activate_actor(second_index - 32);
                }
                actor_traits_second->health = (uint16)(0);
                if (link->kind == 5)
                    mission_link_deactivate(index);
            }
            if ((game_runtime.mission_control.mode & 14) != 0 && second_index < 32)
            {
                if (((*mission_actor_flags(first_index)) & 4) != 0 && (~(*mission_actor_flags(second_index)) & 4) != 0)
                    mission_actor_begin_deactivation(second_index);
                if ((~actor_traits_first->flags & 0x4000) != 0)
                {
                    value = actor_traits_second->flags;
                    if ((value & 0x4000) != 0)
                        actor_traits_second->flags = (uint32)(value & 0xFFFFBFFF);
                }
            }
        }
        count = links.count;
        ++index;
    } while (index < count);
    return;
}

void mission_links_update_targets(void)
{
    MISSION_LINK *link;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 count, index, from, to, flags, value;
    MISSION_ACTOR_TARGET *destination;
    MISSION_ACTOR_TARGET *source;
    FUNCTION_MARKER(0x8004C5B8, "ACE3.BIN");
    count = links.count;
    if (count == 0)
        return;
    index = 0;
    do
    {
        link = link_at(index);
        if (link->enabled)
        {
            to = link->destination_actor;
            from = link->source_actor;
            destination = mission_actor_target(to);
            if (from == 0)
                destination->distance = (uint32)(1);
            else
            {
                source = mission_actor_target(from);
                *destination = *source;
            }
            actor_index_table = 0;
            actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(from * 4) >> 2))));
            flags = actor_traits_object->flags;
            if (flags & 0x4000)
            {
                actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(to * 4) >> 2))));
                value = actor_traits_object->flags;
                if ((value & 0x4000) && (flags & 0x2000))
                    actor_traits_object->flags = (uint32)(value | 0x2000);
            }
        }
        count = links.count;
        ++index;
    } while ((sint32)index < (sint32)count);
    return;
}
