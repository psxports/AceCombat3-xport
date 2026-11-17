#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

enum { MISSION_GROUP_CAPACITY = 255, MISSION_GROUP_MEMBERS = 8 };

typedef struct
{
    uint32 flags;
    uint16 member_count;
    uint8 actor_ids[MISSION_GROUP_MEMBERS];
    uint16 active_count;
    uint8 leader_actor;
    uint8 triggered;
    uint32 elapsed_ticks;
} MISSION_GROUP;

typedef struct
{
    uint32 count;
    MISSION_GROUP entries[MISSION_GROUP_CAPACITY];
} MISSION_GROUP_STATE;

static MISSION_GROUP_STATE groups;

static void mission_group_disable(uint32 index);

static const uint32 formation_offsets[9][4] =
{
    {0xfff9c000, 0x14000, 0x0, 0x0},
    {0xfff9c000, 0x14000, 0xfff9c000, 0x0},
    {0x64000, 0x14000, 0xfff9c000, 0x0},
    {0xfff38000, 0x3c000, 0xfff38000, 0x0},
    {0x0, 0x3c000, 0xfff38000, 0x0},
    {0xc8000, 0x3c000, 0xfff38000, 0x0},
    {0xfff9c000, 0xfffec000, 0xffed4000, 0x0},
    {0x0, 0xfffec000, 0xffed4000, 0x0},
    {0x64000, 0xfffec000, 0xffed4000, 0x0}
};

static const uint8 formation_selectors[7][MISSION_GROUP_MEMBERS] =
{
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 0, 0, 0, 0, 0},
    {0, 1, 2, 4, 0, 0, 0, 0},
    {0, 1, 2, 3, 5, 0, 0, 0},
    {0, 1, 2, 3, 4, 5, 0, 0},
    {0, 1, 2, 3, 4, 5, 7, 0},
    {0, 1, 2, 3, 4, 5, 6, 8}
};

static void set_member_offset(uint32 count, uint32 ordinal, uint32 output[3])
{
    const uint32 *offset;
    uint32 selector;
    FUNCTION_MARKER(0x8004BD18, "ACE3.BIN");
    if (count < 2 || count > MISSION_GROUP_MEMBERS || ordinal >= MISSION_GROUP_MEMBERS)
        abort();
    selector = formation_selectors[count - 2][ordinal];
    offset = formation_offsets[selector];
    output[0] = offset[0];
    output[1] = offset[1];
    output[2] = offset[2];
}

void mission_groups_reset(void)
{
    memset(&groups, 0, sizeof(groups));
}

static MISSION_GROUP *group_at(uint32 index)
{
    if (index >= groups.count)
        abort();
    return &groups.entries[index];
}

void mission_groups_bind(const uint8 *source, uint32 count)
{
    uint16 wire_halfword_0;
    MISSION_GROUP *group;
    uint32 index, member, members;
    const uint8 *record;

    if (count > MISSION_GROUP_CAPACITY)
        abort();
    groups.count = count;
    for (index = 0; index < count; ++index)
    {
        group = group_at(index);
        record = source + index * 12;
        members = (memcpy(&wire_halfword_0, record + 2, sizeof(wire_halfword_0)), wire_halfword_0);
        if (members > MISSION_GROUP_MEMBERS)
            abort();
        group->member_count = (uint16)members;
        /* read the first member byte even for an empty group */
        group->actor_ids[0] = record[4];
        for (member = 1; member < members; ++member)
            group->actor_ids[member] = record[member + 4];
        group->flags = 1;
        group->active_count = (uint16)members;
        group->leader_actor = group->actor_ids[0];
        group->triggered = 0;
        group->elapsed_ticks = 0;
    }
}

void mission_groups_update(void)
{
    MISSION_GROUP *group;
    uint32 count, index = 0, value;
    FUNCTION_MARKER(0x8004BA0C, "ACE3.BIN");
    count = groups.count;
    if (count == 0)
        { (void)(uint32)(count); return; }
    do
    {
        group = group_at(index);
        value = group->flags;
        if ((value & 1) != 0)
            mission_group_update(index);
        count = groups.count;
        ++index;
    } while (index < count);
    return;
}

void mission_group_update(uint32 group_index)
{
    MISSION_PRIMARY_AI *ai_owner_actor;
    uint8 members[MISSION_GROUP_MEMBERS];
    MISSION_GROUP *group;
    uint32 scan, active, cursor, member, flags, flag, count, value, result;
    uint32 *flags_address;
    MISSION_ACTOR_TARGET *leader;
    FUNCTION_MARKER(0x8004BA7C, "ACE3.BIN");
    active = 0;
    scan = 0;
    group = group_at(group_index);
    count = group->member_count;
    cursor = 0;
    if (count != 0)
    {
        do
        {
            member = group->actor_ids[cursor];
            flags_address = mission_actor_flags(member);
            flags = (*flags_address);
            if ((flags & 1) != 0)
            {
                if (active != 0)
                    *flags_address = (flags | 0x100000);
                else
                {
                    group->leader_actor = (uint8)member;
                    member = group->actor_ids[cursor];
                    flags_address = mission_actor_flags(member);
                    flags = (*flags_address);
                    *flags_address = (flags & 0xFFEFFFFF);
                }
                members[active] = group->actor_ids[cursor];
                flag = group->triggered;
                active += 1;
                if (flag != 1)
                {
                    member = group->actor_ids[cursor];
                    flags = effect_actor_tracking.target_mask[0];
                    if ((flags & ((uint32)1 << (member & 31))) != 0)
                    {
                        group->triggered = 1;
                        group->elapsed_ticks = 0;
                    }
                }
            }
            count = group->member_count;
            scan += 1;
            cursor += 1;
        } while ((sint32)scan < (sint32)count);
    }
    group->active_count = (uint16)active;
    if ((active & 0xFFFF) < 2)
        mission_group_disable(group_index);
    member = group->leader_actor;
    leader = mission_actor_target(member);
    if (leader == 0)
        return;
    flag = group->triggered;
    if (flag != 1)
    {
        flags = (leader->flags);
        if ((flags & 32) != 0)
        {
            group->triggered = 1;
            group->elapsed_ticks = 0;
        }
    }
    count = group->active_count;
    scan = 1;
    if ((sint32)scan < (sint32)count)
    {
        do
        {
            member = members[scan];
            ai_owner_actor = mission_primary_ai((0) + (member));
            flag = group->triggered;
            if (flag == 1)
            {
                value = group->elapsed_ticks;
                if (value == 0)
                {
                    value = ai_owner_actor->aim_position[0];
                    ai_owner_actor->requested_maneuver = (uint8)(12);
                    ai_owner_actor->requested_behavior = (uint8)(23);
                    ai_owner_actor->speed_mode = (uint8)(0);
                    ai_owner_actor->requested_angles[0] = (uint16)(4096);
                    ai_owner_actor->requested_angles[1] = (uint16)(0);
                    ai_owner_actor->requested_angles[2] = (uint16)((uint32)((sint32)value >> 7));
                }
                flags = ai_owner_actor->flags;
                if ((flags & 0x400000) != 0)
                {
                    mission_group_disable(group_index);
                    ai_owner_actor->requested_behavior = (uint8)(255);
                    ai_owner_actor->requested_maneuver = (uint8)(255);
                }
            }
            else
            {
                ai_owner_actor->requested_behavior = (uint8)(3);
                member = group->leader_actor;
                ai_owner_actor->behavior_actor = (uint8)(member);
                count = group->active_count;
                set_member_offset(count, scan, ai_owner_actor->aim_position);
            }
            count = group->active_count;
            scan += 1;
        } while ((sint32)scan < (sint32)count);
    }
    result = group->elapsed_ticks + 1;
    group->elapsed_ticks = result;
    { (void)(uint32)(result); return; }
}

static void mission_group_disable(uint32 index)
{
    MISSION_GROUP *group;
    uint32 value, count, member, flags, cursor;
    uint32 *actor_flags;
    FUNCTION_MARKER(0x8004BDA8, "ACE3.BIN");
    group = group_at(index);
    value = group->flags;
    group->flags = value & 0xFFFFFFFE;
    count = group->member_count;
    if (count == 0)
        return;
    cursor = 0;
    do
    {
        member = group->actor_ids[cursor];
        actor_flags = mission_actor_flags(member);
        flags = (*actor_flags);
        *actor_flags = (flags & 0xFFEFFFFF);
        count = group->member_count;
        ++cursor;
    } while ((sint32)cursor < (sint32)count);
}
