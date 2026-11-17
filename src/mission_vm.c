#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    MISSION_SCRIPT_REFERENCE reference;
    uint32 offset;
} MISSION_SCRIPT_CURSOR;

static MISSION_SCRIPT_REFERENCE mission_script_source;
static MISSION_SCRIPT_REFERENCE mission_resource_source;
static MISSION_SCRIPT_REFERENCE scene_script_source;
static MISSION_SCRIPT_REFERENCE aircraft_script_source;

MISSION_SCRIPT_REFERENCE mission_script_primary_source(void)
{
    return mission_script_source;
}

MISSION_SCRIPT_REFERENCE mission_script_scene_source(void)
{
    return scene_script_source;
}

void mission_script_publish_scene(void)
{
    scene_script_source = mission_script_source;
}

static uint32 mission_script_read_signed_halfword(MISSION_SCRIPT_CURSOR source);
static void mission_script_run(MISSION_SCRIPT_CURSOR cursor);

static MISSION_SCRIPT_CURSOR mission_script_cursor_advance(MISSION_SCRIPT_CURSOR cursor, uint32 bytes)
{
    cursor.offset += bytes;
    return cursor;
}

static uint32 mission_script_cursor_address(MISSION_SCRIPT_CURSOR cursor)
{
    return cursor.reference.legacy_address + cursor.offset;
}

static void mission_script_bind_source(MISSION_SCRIPT_REFERENCE *region, const uint8 *data, uint32 base, uint32 end)
{
    region->bytes = NULL;
    region->legacy_address = 0;
    region->size = 0;
    if (base < 0x80000000u || end <= base || end > 0x80000000u + sizeof(DRAM))
        return;

    region->bytes = data;
    region->legacy_address = base;
    region->size = end - base;
}

void mission_script_reset_region(void)
{
    mission_script_source = (MISSION_SCRIPT_REFERENCE){0};
    mission_resource_source = (MISSION_SCRIPT_REFERENCE){0};
    scene_script_source = (MISSION_SCRIPT_REFERENCE){0};
    aircraft_script_source = (MISSION_SCRIPT_REFERENCE){0};
}

void mission_script_bind_region(const uint8 *data, uint32 base, uint32 end, MISSION_SCRIPT_REFERENCE backing)
{
    mission_script_reset_region();
    mission_script_bind_source(&mission_script_source, data, base, end);
    if (!mission_script_source.bytes)
        return;
    if (!backing.bytes || backing.legacy_address < 0x80000000u || backing.legacy_address > 0x80000000u + PSX_DRAM_SIZE || backing.legacy_address > base || end < backing.legacy_address || end - backing.legacy_address > backing.size || data != backing.bytes + (base - backing.legacy_address) || backing.size > 0x80000000u + PSX_DRAM_SIZE - backing.legacy_address)
        abort();
    mission_resource_source = backing;
}

void mission_script_bind_aircraft_region(const uint8 *data, uint32 base, uint32 end)
{
    mission_script_bind_source(&aircraft_script_source, data, base, end);
}

static uint8 mission_script_byte(MISSION_SCRIPT_CURSOR cursor)
{
    if (cursor.offset < cursor.reference.size)
        return cursor.reference.bytes[cursor.offset];
    MISSION_SCRIPT_REFERENCE current = mission_script_reference(mission_script_cursor_address(cursor));
    if (current.bytes && current.size)
        return current.bytes[0];
    abort();
}

static uint32 mission_script_read_word(MISSION_SCRIPT_CURSOR bytes)
{
    uint32 a, b, c, d;
    FUNCTION_MARKER(0x80056164, "ACE3.BIN");
    a = mission_script_byte(bytes);
    b = mission_script_byte(mission_script_cursor_advance(bytes, 1));
    c = mission_script_byte(mission_script_cursor_advance(bytes, 2));
    d = mission_script_byte(mission_script_cursor_advance(bytes, 3));
    return a | (b << 8) | (c << 16) | (d << 24);
}

static MISSION_SCRIPT_CURSOR mission_script_branch_threshold(MISSION_SCRIPT_CURSOR script, uint32 threshold)
{
    uint32 value, length;
    MISSION_SCRIPT_CURSOR next, branch;
    FUNCTION_MARKER(0x80055C4C, "ACE3.BIN");
    value = mission_script_read_word(script);
    script = mission_script_cursor_advance(script, 4);
    length = mission_script_byte(script);
    next = mission_script_cursor_advance(script, length);
    script = mission_script_cursor_advance(script, 1);
    if ((sint32)value < (sint32)threshold)
    {
        length = mission_script_byte(script);
        branch = mission_script_cursor_advance(script, length);
    }
    else
        branch = mission_script_cursor_advance(script, 1);
    mission_script_run(branch);
    return next;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_12(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055CBC, "ACE3.BIN");
    threshold = mission_script_read_word(script);
    script = mission_script_cursor_advance(script, 4);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    value = game_runtime.metrics.score;
    cursor = mission_script_cursor_advance(script, 1);
    if ((sint32)value < (sint32)threshold)
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    else
        cursor = mission_script_cursor_advance(cursor, 1);
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_13(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055D2C, "ACE3.BIN");
    threshold = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    value = game_runtime.metrics.script_count;
    cursor = mission_script_cursor_advance(script, 1);
    if (value < threshold)
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    else
        cursor = mission_script_cursor_advance(cursor, 1);
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_14(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055D94, "ACE3.BIN");
    threshold = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    value = game_runtime.metrics.flagged_count[1];
    cursor = mission_script_cursor_advance(script, 1);
    if (threshold < value)
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    else
        cursor = mission_script_cursor_advance(cursor, 1);
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_15(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055DFC, "ACE3.BIN");
    threshold = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    value = game_runtime.metrics.flagged_count[0];
    cursor = mission_script_cursor_advance(script, 1);
    if (value < threshold)
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    else
        cursor = mission_script_cursor_advance(cursor, 1);
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_16(MISSION_SCRIPT_CURSOR script)
{
    uint32 value;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055E64, "ACE3.BIN");
    mission_script_read_word(script);
    script = mission_script_cursor_advance(script, 4);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    value = game_runtime.metrics.special_result;
    cursor = mission_script_cursor_advance(script, 1);
    if (value == 1)
        cursor = mission_script_cursor_advance(cursor, 1);
    else
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_17(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055ED8, "ACE3.BIN");
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    threshold = game_runtime.metrics.category0_4_count;
    value = game_runtime.metrics.category1_count;
    cursor = mission_script_cursor_advance(script, 1);
    if (value < threshold)
        cursor = mission_script_cursor_advance(cursor, 1);
    else
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_18(MISSION_SCRIPT_CURSOR script)
{
    uint32 value, threshold;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055F3C, "ACE3.BIN");
    threshold = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    if (threshold < GAME_MISSION_COUNT)
        value = game_runtime.progress.mission_rating[4][threshold];
    else if (threshold < GAME_MISSION_COUNT + 80)
        value = game_runtime.progress.archive_status[threshold - GAME_MISSION_COUNT];
    else
        value = game_runtime.progress.scene_status[threshold - GAME_MISSION_COUNT - 80];
    cursor = mission_script_cursor_advance(script, 1);
    if (value != 0)
        cursor = mission_script_cursor_advance(cursor, 1);
    else
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_19(MISSION_SCRIPT_CURSOR script)
{
    uint32 value;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80055FA4, "ACE3.BIN");
    value = mission_script_byte(script);
    end = mission_script_cursor_advance(script, value);
    script = mission_script_cursor_advance(script, 1);
    value = game_random();
    cursor = mission_script_cursor_advance(script, 1);
    if ((value & 1) == 0)
    {
        value = mission_script_byte(script);
        cursor = mission_script_cursor_advance(script, value);
    }
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_20(MISSION_SCRIPT_CURSOR script)
{
    MISSION_ACTOR_TRAITS *actor_traits_root;
    uint32 actor_index_root;

    uint32 value, threshold, length;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80056004, "ACE3.BIN");
    value = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    threshold = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    actor_index_root = 0;
    length = mission_script_byte(script);
    actor_traits_root = mission_actor_traits((actor_index_root + ((uint32)((sint32)(value * 4) >> 2))));
    end = mission_script_cursor_advance(script, length);
    value = (sint16)actor_traits_root->health;
    cursor = mission_script_cursor_advance(script, 1);
    if ((sint32)value < (sint32)threshold)
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    else
        cursor = mission_script_cursor_advance(cursor, 1);
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_61(MISSION_SCRIPT_CURSOR script)
{
    uint32 operand, length, value;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x80056080, "ACE3.BIN");
    operand = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    length = mission_script_byte(script);
    end = mission_script_cursor_advance(script, length);
    value = (uint8)game_runtime.mission_control.argument;
    cursor = mission_script_cursor_advance(script, 1);
    if (operand == value)
        cursor = mission_script_cursor_advance(cursor, 1);
    else
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    mission_script_run(cursor);
    return end;
}

static MISSION_SCRIPT_CURSOR mission_script_branch_62(MISSION_SCRIPT_CURSOR script)
{
    uint32 operand, length, value;
    MISSION_SCRIPT_CURSOR end, cursor;
    FUNCTION_MARKER(0x800560E4, "ACE3.BIN");
    operand = mission_script_byte(script);
    script = mission_script_cursor_advance(script, 1);
    length = mission_script_byte(script);
    end = mission_script_cursor_advance(script, length);
    value = (uint8)game_runtime.selection.difficulty;
    cursor = mission_script_cursor_advance(script, 1);
    if (operand == value)
        cursor = mission_script_cursor_advance(cursor, 1);
    else
    {
        value = mission_script_byte(cursor);
        cursor = mission_script_cursor_advance(cursor, value);
    }
    mission_script_run(cursor);
    return end;
}

static void mission_script_run(MISSION_SCRIPT_CURSOR cursor)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    const MISSION_ACTOR_DEFINITION *definition;
    uint32 actor_index_slot;
    uint32 actor_index_table;

    uint32 opcode, index, mode, value, other, target, mask, count, i;
    uint32 *flag_slot;
    sint32 parameter;
    /* Defines the pose bytes omitted by the original script command */
    ACTOR_POSE pose = {0};
    MISSION_SCREEN_MARKER *marker_state;
    FUNCTION_MARKER(0x80054EBC, "ACE3.BIN");
    opcode = mission_script_byte(cursor);
    while (opcode != 0)
    {
        cursor = mission_script_cursor_advance(cursor, 1);
        switch (opcode)
        {
            case 1:
                value = mission_script_byte(cursor);
                mission_flow.request.mode = (uint8)(value);
                mode = game_runtime.mission_control.mode;
                cursor = mission_script_cursor_advance(cursor, 1);
                if (mode == 4)
                {
                    mission_request_mode(8, mission_flow.request.restore_arg);
                    value = game_runtime.flags;
                    game_runtime.flags = value | 0x10000000;
                    if (mission_flow.request.mode != 0)
                        resource_cancel_streams();
                }
                else if (mode == 0)
                {
                    mission_dispatch_scene_request();
                    value = game_runtime.mission_control.flags;
                    game_runtime.mission_control.flags = (uint32)(value & 0xDFFFFFFF);
                }
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                if (opcode == 2)
                    mission_flow.request.script_arg = (uint8)value;
                else if (opcode == 3)
                    mission_flow.request.mission_arg = (uint8)value;
                else if (opcode == 4)
                    mission_flow.request.scene_arg = (uint8)value;
                else
                    mission_flow.request.movie_arg = (uint8)value;
                break;
            case 6:
                value = mission_script_byte(cursor);
                if (value <= 7)
                {
                    if (value == 3)
                        value = 8;
                    else if (value >= 5)
                        value = (uint32)1 << (value - 1);
                    mission_flow.request.restore_arg = (uint8)(value);
                }
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 7:
                value = mission_flow.request.flags;
                mission_flow.request.flags = (uint8)(value | 4);
                mission_flow.request.mode = (uint8)(9);
                break;
            case 8:
                mission_flow.request.mode = (uint8)(9);
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                game_runtime.mission_result.route_mission = (uint8)value;
                break;
            case 9:
                value = mission_flow.request.flags;
                mission_flow.request.flags = (uint8)(value | 1);
                break;
            case 10:
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                game_runtime.account.result_kind = (uint8)(value);
                break;
            case 11:
                value = game_runtime.metrics.phase_ticks;
                cursor = mission_script_branch_threshold(cursor, value);
                break;
            case 12:
                cursor = mission_script_branch_12(cursor);
                break;
            case 13:
                cursor = mission_script_branch_13(cursor);
                break;
            case 14:
                cursor = mission_script_branch_14(cursor);
                break;
            case 15:
                cursor = mission_script_branch_15(cursor);
                break;
            case 16:
                cursor = mission_script_branch_16(cursor);
                break;
            case 17:
                cursor = mission_script_branch_17(cursor);
                break;
            case 18:
                cursor = mission_script_branch_18(cursor);
                break;
            case 19:
                cursor = mission_script_branch_19(cursor);
                break;
            case 20:
                cursor = mission_script_branch_20(cursor);
                break;
            case 21:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                if (index < 32)
                {
                    mission_primary_show_actor(index);
                    mission_primary_activate_actor(index);
                }
                else
                {
                    index -= 32;
                    mission_actor_show_secondary(index);
                    mission_secondary_activate_actor(index);
                }
                break;
            case 22:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                actor_index_table = 0;
                actor_index_slot = (actor_index_table + ((uint32)((sint32)(index * 4) >> 2)));
                actor_traits_object = mission_actor_traits(actor_index_slot);
                value = actor_traits_object->flags;
                actor_traits_object->flags = (uint32)(value & 0xFFFFBFFF);
                if (index < 32)
                {
                    actor_traits_object = mission_actor_traits(actor_index_slot);
                    value = actor_traits_object->flags;
                    actor_traits_object->flags = (uint32)(value & 0xFFFEFFFF);
                }
                break;
            case 23:
            case 25:
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 24:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                actor_index_table = 0;
                actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                value = mission_script_read_signed_halfword(cursor);
                parameter = (sint16)value;
                if (parameter == -1)
                {
                    if (index < 32)
                    {
                        definition = &mission_primary_binding(index & 0x03FFFFFFu)->definition;
                    }
                    else
                    {
                        definition = &mission_secondary_binding(index - 32)->definition;
                    }
                    value = definition->health;
                    actor_traits_object->health = (uint16)(value);
                }
                else if (parameter == -2)
                {
                    value = game_runtime.actor_resources.script_saved_health;
                    actor_traits_object->health = (uint16)(value);
                }
                else if (parameter == -3)
                {
                    value = actor_traits_object->health;
                    game_runtime.actor_resources.script_saved_health = (uint16)value;
                }
                else
                    actor_traits_object->health = (uint16)(value);
                cursor = mission_script_cursor_advance(cursor, 2);
                break;
            case 26:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                flag_slot = mission_actor_flags(index);
                value = (*flag_slot);
                *flag_slot = (value | 0x20000);
                break;
            case 27:
            case 28:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mode = mission_script_byte(cursor);
                if (mode == 1)
                {
                    flag_slot = mission_actor_flags(index);
                    value = (*flag_slot);
                    actor_index_table = 0;
                    actor_index_slot = (actor_index_table + ((uint32)((sint32)(index * 4) >> 2)));
                    *flag_slot = (value | (opcode == 27 ? 0x100 : 0x200));
                    actor_traits_object = mission_actor_traits(actor_index_slot);
                    value = actor_traits_object->flags;
                    actor_traits_object->flags = (uint32)(value & 0xFFFFFE3F);
                    actor_traits_object = mission_actor_traits(actor_index_slot);
                    value = actor_traits_object->flags;
                    actor_traits_object->flags = (uint32)(value | (opcode == 27 ? 0x80 : 0x40));
                    marker_state = mission_actor_marker(index);
                    marker_state->affiliation = (uint8)(opcode == 27 ? 1 : 3);
                    hud_radar_contact(index)->color = (uint8)(opcode == 27 ? 1 : 2);
                    value = (*flag_slot);
                    *flag_slot = (value & (opcode == 27 ? 0xFFFFFDFF : 0xFFFFFEFF));
                    if (opcode == 28 && index == hud_control.selected_target_idx)
                    {
                        value = mission_target_links_release(0);
                        hud_control.selected_target_idx = (uint8)(value);
                    }
                }
                else if (mode == 0)
                {
                    flag_slot = mission_actor_flags(index);
                    value = (*flag_slot);
                    *flag_slot = (value & (opcode == 27 ? 0xFFFFFEFF : 0xFFFFFDFF));
                    marker_state = mission_actor_marker(index);
                    marker_state->affiliation = (uint8)(0);
                    hud_radar_contact(index)->color = (uint8)(0);
                }
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 29:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                value = mission_script_byte(cursor);
                flag_slot = mission_actor_flags(index);
                other = (*flag_slot);
                *flag_slot = ((value & 2) != 0 ? other | 0x4000 : other & 0xFFFFBFFF);
                value = mission_script_byte(cursor);
                other = (*flag_slot);
                *flag_slot = ((value & 1) != 0 ? other | 0x8000 : other & 0xFFFF7FFF);
                value = hud_control.selected_target_idx;
                cursor = mission_script_cursor_advance(cursor, 1);
                if (value == index)
                {
                    hud_control.selected_target_idx = (uint8)(255);
                    mission_target_links_release(0);
                    mission_target_links_assign(0, 255);
                }
                break;
            case 30:
                cursor = mission_script_cursor_advance(cursor, 2);
                break;
            case 31:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                if (index >= 2)
                    mission_primary_ai((0) + (index))->behavior_kind = (uint8)(255);
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mission_primary_binding(index & 0x03FFFFFFu)->behavior_index = (uint32)(value - 1);
                break;
            case 32:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mission_secondary_select_motion(index, value);
                break;
            case 33:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mode = mission_script_byte(cursor);
                mission_primary_select_controls(index, mode);
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 34:
            case 35:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mode = mission_script_byte(cursor);
                flag_slot = mission_actor_flags(index);
                value = (*flag_slot);
                mask = opcode == 34 ? 0x1000 : 0x2000;
                *flag_slot = (mode == 1 ? value | mask : value & ~mask);
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 36:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mode = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                if (mode == 1)
                    mission_link_activate(index);
                else
                    mission_link_deactivate(index);
                break;
            case 37:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                value = mission_script_read_word(cursor);
                cursor = mission_script_cursor_advance(cursor, 4);
                pose.pose.position[0] = value << 12;
                value = mission_script_read_word(cursor);
                cursor = mission_script_cursor_advance(cursor, 4);
                pose.pose.position[1] = value << 12;
                value = mission_script_read_word(cursor);
                cursor = mission_script_cursor_advance(cursor, 4);
                pose.pose.position[2] = value << 12;
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                pose.pose.angles[0] = value;
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                pose.pose.angles[1] = value;
                value = mission_script_read_signed_halfword(cursor);
                pose.pose.angles[2] = value;
                cursor = mission_script_cursor_advance(cursor, 2);
                if (index == 0)
                    mission_player_set_pose(&pose, NULL);
                else
                {
                    actor_index_table = 0;
                    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
                    pose_owner_object->pose = pose.pose;
                    i = 6;
                }
                break;
            case 41:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                speech_queue_push(index, (sint16)value);
                break;
            case 42:
                index = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                sound_queue_push(index, value);
                break;
            case 43:
                count = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                i = 0;
                while (i < count)
                {
                    index = mission_script_byte(cursor);
                    cursor = mission_script_cursor_advance(cursor, 1);
                    value = mission_script_byte(cursor);
                    cursor = mission_script_cursor_advance(cursor, 1);
                    i += 1;
                    sound_queue_push(index, value);
                }
                break;
            case 45:
            case 47:
            case 52:
            case 54:
            case 57:
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                target = opcode == 45 ? 30 : opcode == 47 ? 31 : opcode == 52 ? 43 : opcode == 54 ? 44 : 28;
                if (target == 30 || target == 31)
                    game_runtime.metrics.flagged_target[target - 30] = (uint8)value;
                else if (target == 28)
                    game_runtime.metrics.timer_mode = (uint8)value;
                else if (target == 43)
                    mission_flow.request.event_id = (uint8)value;
                else
                    mission_flow.request.music_id = (uint8)value;
                break;
            case 46:
            case 48:
            case 50:
                if (opcode == 50)
                    game_runtime.metrics.script_count = 0;
                else
                    game_runtime.metrics.flagged_count[opcode == 46 ? 0 : 1] = 0;
                break;
            case 49:
                value = game_runtime.metrics.script_count;
                game_runtime.metrics.script_count = (uint8)(value + 1);
                break;
            case 51:
                mission_status_text_show(mission_flow.request.event_id);
                break;
            case 53:
                value = game_runtime.metrics.flagged_count[0];
                game_runtime.metrics.flagged_count[0] = (uint8)(value << 1);
                break;
            case 55:
                audio_play_main_track(mission_flow.request.music_id);
                break;
            case 56:
                audio_play_main_track(mission_script_byte(cursor));
                cursor = mission_script_cursor_advance(cursor, 1);
                break;
            case 58:
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                game_runtime.metrics.display_ticks = (uint32)((uint32)(sint32)(sint16)value);
                break;
            case 59:
                scene_geometry_enable_bucket_copies(1);
                break;
            case 60:
                value = game_runtime.metrics.elapsed_ticks;
                cursor = mission_script_branch_threshold(cursor, value);
                break;
            case 61:
                cursor = mission_script_branch_61(cursor);
                break;
            case 62:
                cursor = mission_script_branch_62(cursor);
                break;
            case 70:
                value = mission_flow.request.flags;
                mission_flow.request.flags = (uint8)(value | 2);
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                mission_flow.request.effect_id = (uint8)(value);
                break;
            case 71:
                value = mission_flow.request.flags;
                cursor = mission_script_cursor_advance(cursor, 1);
                mission_flow.request.flags = (uint8)(value | 4);
                break;
            case 72:
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                game_runtime.metrics.fade.mode = (uint8)(value);
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                game_runtime.metrics.fade.duration = (uint16)(value);
                break;
            case 73:
                game_runtime.metrics.fade.active = (uint8)(1);
                game_runtime.metrics.fade.ticks = (uint16)(0);
                break;
            case 74:
                for (i = 52; i <= 55; ++i)
                {
                    value = mission_script_byte(cursor);
                    cursor = mission_script_cursor_advance(cursor, 1);
                    if (i == 52)
                        game_runtime.mission_result.rating_rules.mode = (uint8)value;
                    else
                        game_runtime.mission_result.rating_rules.count_threshold[i - 53] = (uint8)value;
                }
                break;
            case 75:
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                other = game_runtime.metrics.score;
                game_runtime.metrics.score = (uint32)(other + (uint32)(sint32)(sint16)value);
                break;
            case 76:
                value = mission_script_byte(cursor);
                cursor = mission_script_cursor_advance(cursor, 1);
                game_runtime.mission_result.rating_rules.mode = (uint8)value;
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                game_runtime.mission_result.rating_rules.value_threshold[0] = (uint16)value;
                value = mission_script_read_signed_halfword(cursor);
                cursor = mission_script_cursor_advance(cursor, 2);
                game_runtime.mission_result.rating_rules.value_threshold[1] = (uint16)value;
                break;
            default:
                break;
        }
        opcode = mission_script_byte(cursor);
    }
    return;
}

static uint32 mission_script_read_signed_halfword(MISSION_SCRIPT_CURSOR source)
{
    uint32 high, low;
    FUNCTION_MARKER(0x80056148, "ACE3.BIN");
    high = mission_script_byte(mission_script_cursor_advance(source, 1));
    low = mission_script_byte(source);
    return (sint16)(low | (high << 8));
}

void mission_script_start_legacy(MISSION_SCRIPT_REFERENCE script)
{
    MISSION_SCRIPT_CURSOR cursor;

    cursor.reference = script;
    cursor.offset = 0;
    mission_script_run(cursor);
}

MISSION_SCRIPT_REFERENCE mission_script_reference(uint32 address)
{
    MISSION_SCRIPT_REFERENCE reference = {NULL, 0, 0};
    uint32 offset;

    reference.legacy_address = address;
    offset = address - mission_script_source.legacy_address;
    if (offset < mission_script_source.size)
    {
        reference.bytes = mission_script_source.bytes + offset;
        reference.size = mission_script_source.size - offset;
        return reference;
    }
    offset = address - aircraft_script_source.legacy_address;
    if (offset < aircraft_script_source.size)
    {
        reference.bytes = aircraft_script_source.bytes + offset;
        reference.size = aircraft_script_source.size - offset;
        return reference;
    }
    offset = address - mission_resource_source.legacy_address;
    if (offset < mission_resource_source.size)
    {
        reference.bytes = mission_resource_source.bytes + offset;
        reference.size = mission_resource_source.size - offset;
    }
    return reference;
}

uint32 mission_script_reference_present(MISSION_SCRIPT_REFERENCE reference)
{
    return reference.legacy_address != 0;
}
