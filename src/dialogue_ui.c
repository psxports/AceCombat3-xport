#include <string.h>

#include "game_functions.h"

#include <stdlib.h>

static const uint8 dialogue_route_kind[] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0xE8, 0x29, 0x0F, 0x80, 0x8C, 0x2A, 0x0F, 0x80, 0x6C, 0x2B, 0x0F, 0x80, 0x34, 0x2C, 0x0F, 0x80, 0x1C, 0x2D, 0x0F, 0x80, 0xC0, 0x2D, 0x0F, 0x80, 0xB0, 0x2E, 0x0F, 0x80, 0x20, 0x30, 0x0F, 0x80, 0x64, 0x30, 0x0F, 0x80, 0xD4, 0x30, 0x0F, 0x80, 0x98, 0x08, 0x0F, 0x80, 0x5C, 0x0F, 0x0F, 0x80, 0xBC, 0x11, 0x0F, 0x80, 0x2C, 0x13, 0x0F, 0x80, 0xDC, 0x13, 0x0F, 0x80, 0x30, 0x14, 0x0F, 0x80, 0xC4, 0x16, 0x0F, 0x80,
    0x48, 0x18, 0x0F, 0x80, 0x7C, 0x1A, 0x0F, 0x80, 0x8C, 0x1B, 0x0F, 0x80, 0x8C, 0x1C, 0x0F, 0x80, 0x04, 0x1D, 0x0F, 0x80, 0x40, 0x22, 0x0F, 0x80, 0xCC, 0x23, 0x0F, 0x80, 0x18, 0x26, 0x0F, 0x80, 0xEC, 0x26, 0x0F, 0x80, 0x7C, 0x0A, 0x0F, 0x80, 0x8C, 0x10, 0x0F, 0x80, 0xBC, 0x11, 0x0F, 0x80, 0x84, 0x13, 0x0F, 0x80, 0xDC, 0x13, 0x0F, 0x80, 0x30, 0x14, 0x0F, 0x80, 0xC4, 0x16, 0x0F, 0x80, 0x48, 0x18, 0x0F, 0x80, 0x7C, 0x1A, 0x0F, 0x80, 0x8C, 0x1B, 0x0F, 0x80, 0x8C, 0x1C, 0x0F, 0x80, 0x04, 0x1D, 0x0F, 0x80, 0x40, 0x22, 0x0F, 0x80, 0xCC, 0x23, 0x0F, 0x80, 0x18, 0x26, 0x0F, 0x80, 0x20, 0x28, 0x0F, 0x80, 0xC4, 0x0D, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x13, 0x0F, 0x80, 0xDC, 0x13, 0x0F, 0x80, 0x30, 0x14, 0x0F, 0x80, 0xC4, 0x16, 0x0F, 0x80,
};

DIALOGUE_UI dialogue_ui;

void dialogue_ui_reset(void)
{
    memset(&dialogue_ui, 0, sizeof(dialogue_ui));
}

static uint32 wait_open(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800F13E4, "ACE3.BIN");
    value = dialogue_ui.frame_ticks;
    if ((sint32)value < 60)
        dialogue_ui.frame_ticks = (uint32)(value + 1);
    else
    {
        mission_flow.mode_phase = 5;
    }
    return dialogue_ui_draw_panel_frame();
}

uint32 dialogue_ui_open_step(void)
{
    FUNCTION_MARKER(0x800F13DC, "ACE3.BIN");
    return wait_open();
}

uint32 dialogue_ui_begin_result(void)
{
    uint32 owner, value, index, context;
    DIALOGUE_SEQUENCE_SOURCE pointer;
    const DIALOGUE_CATALOG_ENTRY *descriptor;
    FUNCTION_MARKER(0x800F0898, "ACE3.BIN");
    owner = arena_alloc(6304);
    dialogue_ui.completed_entries = (uint16)(0);
    dialogue_ui.wait_for_completion = (uint16)(1);
    dialogue_ui.frame_ticks = (uint32)(0);
    dialogue_ui.arena_begin = (uint32)(owner);
    dialogue_ui.caption.packets = NULL;
    value = game_runtime.progress.flow_flags;
    game_runtime.progress.flow_flags = (uint32)((value | 64) & 0xFFFFFF7F);
    subtitles_initialize();
    owner = dialogue_ui.arena_begin;
    value = game_runtime.account.result_kind;
    pointer = mission_dialogue_result((uint8)value);
    dialogue_sequence_bind(pointer, game_runtime.progress.sequence_idx);
    dialogue_ui_queue_entry_frame();
    owner = dialogue_ui.arena_begin;
    dialogue_ui.render_state = (uint32)(0);
    owner = dialogue_ui.arena_begin;
    for (index = 16; index > 0; --index)
    {
    }
    value = dialogue_sequence_remaining();
    if (value)
    {
        mission_flow.mode_phase = 1;
        value = 0;
    }
    else
    {
        mission_flow.mode_phase = 3;
        mission_menu_start_item_motion(0, 15);
        value = 0x8000;
    }
    ui_group_set_mask(0, value);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.item_mask = (uint32)(0x8000);
    if (dialogue_sequence_remaining() == 0)
        return sound_play(90);
    descriptor = dialogue_sequence_entry((uint32)dialogue_sequence_id(0));
    value = dialogue_ui_is_detail_layout(descriptor->image_kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.panel_style = (uint32)(value);
    descriptor = dialogue_sequence_entry((uint32)dialogue_sequence_id(0));
    value = dialogue_ui_layout_item(descriptor->image_kind);
    context = dialogue_ui.arena_begin;
    dialogue_ui.active_item = (uint32)(value);
    return sound_play(90);
}

uint32 dialogue_ui_briefing_frame(void)
{
    FUNCTION_MARKER(0x800F0A7C, "ACE3.BIN");
    return dialogue_ui_begin_briefing();
}

uint32 dialogue_ui_begin_briefing(void)
{
    uint32 flags, kind, owner, value, count, index;
    DIALOGUE_SEQUENCE_SOURCE pointer;
    const DIALOGUE_CATALOG_ENTRY *descriptor;
    sint32 mission;
    FUNCTION_MARKER(0x800F0A84, "ACE3.BIN");
    flags = game_runtime.progress.flow_flags;
    game_runtime.progress.flow_flags = (uint32)((flags | 128) & 0xFFFFFFBF);
    mission = (sint16)game_runtime.selection.mission;
    kind = dialogue_route_kind[(uint8)mission];
    if (kind == 0)
    {
        mission_flow.next_scene = 0x60000002;
        game_runtime.account.route_mode = (uint16)(1);
    }
    else if (kind == 2)
    {
        if (mission == 16)
            game_runtime.playback.movie_idx = (uint8)(8);
        if ((sint16)game_runtime.selection.mission == 37)
        {
            game_runtime.playback.movie_idx = (uint8)(26);
            game_runtime.selection.mission = (uint16)(57);
        }
        game_runtime.playback.return_mode = (uint32)(2);
        game_runtime.account.route_mode = (uint16)(1);
        mission_flow.next_scene = 0x60000005;
    }
    owner = arena_alloc(6304);
    dialogue_ui.arena_begin = (uint32)(owner);
    dialogue_ui.caption.packets = NULL;
    subtitles_initialize();
    owner = dialogue_ui.arena_begin;
    dialogue_ui.completed_entries = (uint16)(0);
    dialogue_ui.wait_for_completion = (uint16)(1);
    dialogue_ui.frame_ticks = (uint32)(0);
    pointer = mission_flow.assets.dialogue_sequences.briefing;
    dialogue_sequence_bind(pointer, game_runtime.progress.sequence_idx);
    dialogue_ui_queue_entry_frame();
    dialogue_ui.render_state = (uint32)(0);
    ui_group_set_mask(1, 0);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.item_mask = (uint32)(0x8000);
    ui_group_bind(ui_group_directory(0), 0);
    owner = dialogue_ui.arena_begin;
    owner = dialogue_ui.arena_begin;
    for (index = 16; index > 0; --index)
    {
    }
    owner = dialogue_ui.arena_begin;
    mission_flow.mode_phase = 1;
    if (dialogue_sequence_remaining() == 0)
        return 0;
    descriptor = dialogue_sequence_entry((uint32)dialogue_sequence_id(0));
    value = dialogue_ui_is_detail_layout(descriptor->image_kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.panel_style = (uint32)(value);
    descriptor = dialogue_sequence_entry((uint32)dialogue_sequence_id(0));
    value = dialogue_ui_layout_item(descriptor->image_kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.active_item = (uint32)(value);
    descriptor = dialogue_sequence_entry((uint32)dialogue_sequence_id(0));
    if (descriptor->image_kind != 10)
        return 10;
    count = dialogue_sequence_remaining();
    if (!count)
        return count;
    mission_flow.next_scene = 0x60000005;
    value = game_runtime.progress.sequence_idx;
    game_runtime.playback.return_mode = (uint32)(3);
    game_runtime.progress.sequence_idx = (uint8)(value + 1);
    owner = dialogue_ui.arena_begin;
    game_runtime.account.route_mode = (uint16)(2);
    value = dialogue_sequence_movie_index((uint32)dialogue_sequence_id(0));
    game_runtime.playback.movie_idx = (uint8)(value);
    return dialogue_sequence_unlock_archive((uint32)dialogue_sequence_id(0));
}

uint32 dialogue_ui_result_fade_tick(void)
{
    uint32 owner, old, value, result;
    FUNCTION_MARKER(0x800F0F5C, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    old = dialogue_ui.frame_ticks;
    dialogue_ui.frame_ticks = (uint32)(old + 1);
    if ((sint32)old >= 32)
    {
        mission_effect_open_ring();
        dialogue_ui.frame_ticks = (uint32)(0);
        mission_flow.mode_phase = 2;
        return 2;
    }
    value = (old + 2) * 143;
    game_runtime.display.background[0] = (uint8)((uint32)((sint32)value / 32));
    value = (dialogue_ui.frame_ticks + 1) * 135;
    game_runtime.display.background[1] = (uint8)((uint32)((sint32)value / 32));
    value = (dialogue_ui.frame_ticks + 1) * 83;
    game_runtime.display.background[2] = (uint8)((uint32)((sint32)value / 32));
    result = game_runtime.display.flags | 8;
    game_runtime.display.flags = (uint32)(result);
    return result;
}

uint32 dialogue_ui_briefing_fade_tick(void)
{
    uint32 owner, old, value, result;
    FUNCTION_MARKER(0x800F108C, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    old = dialogue_ui.frame_ticks;
    dialogue_ui.frame_ticks = (uint32)(old + 1);
    if ((sint32)old >= 32)
    {
        mission_effect_open_ring();
        dialogue_ui.frame_ticks = (uint32)(0);
        mission_flow.mode_phase = 2;
        return 2;
    }
    value = (old + 2) * 143;
    game_runtime.display.background[0] = (uint8)((uint32)((sint32)value / 32));
    value = (dialogue_ui.frame_ticks + 1) * 135;
    game_runtime.display.background[1] = (uint8)((uint32)((sint32)value / 32));
    value = (dialogue_ui.frame_ticks + 1) * 83;
    game_runtime.display.background[2] = (uint8)((uint32)((sint32)value / 32));
    result = game_runtime.display.flags | 8;
    game_runtime.display.flags = (uint32)(result);
    return result;
}

uint32 dialogue_ui_wait_header_tick(void)
{
    uint32 owner;
    FUNCTION_MARKER(0x800F132C, "ACE3.BIN");
    if ((sint16)ui_item_at(0, 15)->phase != 1)
        return 0x80110000;
    owner = dialogue_ui.arena_begin;
    if (!dialogue_sequence_remaining())
    {
        mission_flow.mode_phase = 15;
        return 15;
    }
    mission_flow.mode_phase = 4;
    dialogue_ui.frame_ticks = (uint32)(0);
    return 4;
}

void dialogue_ui_return_account(void)
{
    uint32 owner, kind, value;
    FUNCTION_MARKER(0x800F26EC, "ACE3.BIN");
    ui_item_at(0, 15)->phase = (uint16)(1);
    mission_flow.root_variant = 0;
    owner = dialogue_ui.arena_begin;
    mission_flow.mode = 6;
    mission_flow.mode_phase = 0;
    mission_flow.account_mode = (uint32)(0);
    kind = 0;
    if (dialogue_sequence_remaining())
    {
        kind = dialogue_sequence_entry((uint32)dialogue_sequence_id(0))->image_kind;
    }
    if (kind == 10)
    {
        mission_flow.playback_requested = (uint32)(1);
        value = game_runtime.progress.sequence_idx;
        game_runtime.progress.sequence_idx = (uint8)(value + 1);
        owner = dialogue_ui.arena_begin;
        game_runtime.account.route_mode = (uint16)(2);
        value = dialogue_sequence_id(0);
        value = dialogue_sequence_movie_index(value);
        game_runtime.playback.movie_idx = (uint8)(value);
        dialogue_sequence_unlock_archive((uint32)dialogue_sequence_id(0));
        return;
    }
    game_runtime.progress.sequence_idx = (uint8)(0);
    owner = dialogue_ui.arena_begin;
    value = game_runtime.progress.flow_flags;
    game_runtime.progress.flow_flags = (uint32)(value & 0xFFFFFFBF);
    arena_set_cursor(owner);
    {
        ui_group_set_mask(0, 0x8000);
        return;
    }
}

uint32 dialogue_ui_return_root_tick(void)
{
    uint32 result, owner, kind = 0, value, map;
    FUNCTION_MARKER(0x800F2820, "ACE3.BIN");
    result = ui_animation_phase(1);
    if (result)
        return result;
    mission_flow.root_kind = 0;
    mission_flow.root_phase = 0;
    mission_flow.root_variant = 0;
    arena_set_cursor(dialogue_ui.arena_begin);
    mission_menu_start_item_motion(0, 1);
    ui_group_set_mask(0, 3);
    ui_animation_set(15, 1, 0, 25);
    mission_effect_open_sprs();
    owner = dialogue_ui.arena_begin;
    if (dialogue_sequence_remaining())
    {
        kind = dialogue_sequence_entry((uint32)dialogue_sequence_id(0))->image_kind;
    }
    if (kind == 10)
    {
        mission_flow.playback_requested = (uint32)(1);
        value = game_runtime.progress.sequence_idx;
        game_runtime.progress.sequence_idx = (uint8)(value + 1);
        owner = dialogue_ui.arena_begin;
        game_runtime.account.route_mode = (uint16)(2);
        value = dialogue_sequence_id(0);
        map = dialogue_sequence_movie_index(value);
        dialogue_sequence_unlock_archive(value);
        game_runtime.playback.movie_idx = (uint8)(map);
        return 0;
    }
    game_runtime.progress.sequence_idx = (uint8)(0);
    value = game_runtime.progress.flow_flags;
    game_runtime.account.route_mode = (uint16)(0);
    result = value & 0xFFFFFF7F;
    game_runtime.progress.flow_flags = (uint32)(result);
    return result;
}

/* the original mission index can also select bytes beyond the route rows */

uint32 dialogue_ui_draw_effects(uint32 fallback_result)
{
    uint32 owner, ordering, value;
    sint32 frame;

    FUNCTION_MARKER(0x800EF4F4, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    value = dialogue_ui.render_state;
    if (value == 1)
    {
        ordering = ui_packets.scene_ot;
        fallback_result = dialogue_video_draw(ordering + 8, &dialogue_video.frame);
        owner = dialogue_ui.arena_begin;
        frame = dialogue_sequence_id(0);
        if (!dialogue_sequence_entry((uint32)frame)->image_kind)
            dialogue_ui_draw_movie_progress();
    }
    else if (value == 2)
        fallback_result = 2;
    return fallback_result;
}

void dialogue_ui_draw_movie_progress(void)
{
    MENU_POLYGON_DESCRIPTOR descriptor;
    uint32 owner, numerator, denominator, quotient, packet, ordering, result;

    FUNCTION_MARKER(0x800EDF84, "ACE3.BIN");
    descriptor.color[0][3] = 0;
    owner = dialogue_ui.arena_begin;
    descriptor.points[0].x = (uint16)(107);
    descriptor.points[0].y = (uint16)(151);
    descriptor.points[1].x = (uint16)(109);
    descriptor.points[1].y = (uint16)(151);
    descriptor.points[2].x = (uint16)(107);
    numerator = dialogue_video.frame * 106;
    denominator = dialogue_video.frames;
    quotient = game_divide_signed(numerator, denominator);
    descriptor.points[3].x = (uint16)(109);
    descriptor.points[2].y = (uint16)(quotient + 151);
    numerator = dialogue_video.frame * 106;
    denominator = dialogue_video.frames;
    quotient = game_divide_signed(numerator, denominator);
    descriptor.color[0][2] = 213;
    descriptor.color[1][2] = 213;
    descriptor.color[2][2] = 213;
    descriptor.color[3][2] = 213;
    descriptor.color[0][0] = 229;
    descriptor.color[0][1] = 229;
    descriptor.color[1][0] = 229;
    descriptor.color[1][1] = 229;
    descriptor.color[2][0] = 229;
    descriptor.color[2][1] = 229;
    descriptor.color[3][0] = 229;
    descriptor.color[3][1] = 229;
    descriptor.points[3].y = (uint16)(quotient + 151);
    packet = ui_packets.scene_packet_cursor;
    ordering = ui_packets.scene_ot;
    result = ui_emit_colored_quad(&descriptor, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    ui_packets.scene_packet_cursor = (uint32)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 dialogue_ui_wait_selection_tick(void)
{
    uint32 owner, value;

    FUNCTION_MARKER(0x800F1384, "ACE3.BIN");
    value = (sint16)ui_item_at(0, 15)->phase;
    if (value != 1)
        return 0x80110000;
    owner = dialogue_ui.arena_begin;
    if (dialogue_sequence_remaining() == 0)
    {
        mission_flow.mode_phase = 15;
        return 15;
    }
    mission_flow.mode_phase = 4;
    dialogue_ui.frame_ticks = (uint32)(0);
    return 4;
}

uint32 dialogue_ui_finish_sequence(void)
{
    uint32 owner, value;

    FUNCTION_MARKER(0x800F2990, "ACE3.BIN");
    value = ui_animation_phase(1);
    if (value != 0)
        return value;
    mission_flow.root_kind = 3;
    mission_flow.root_phase = 3;
    owner = dialogue_ui.arena_begin;
    mission_flow.transition_phase = 6;
    return arena_set_cursor(owner);
}

uint32 dialogue_ui_wait_entry_audio(void)
{
    uint32 owner, value;

    FUNCTION_MARKER(0x800F1C8C, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    if (resource_get_stream_elapsed() != 0)
    {
        mission_flow.mode_phase = 11;
        return 11;
    }
    owner = dialogue_ui.arena_begin;
    value = dialogue_ui.wait_ticks - 1;
    dialogue_ui.wait_ticks = (uint32)(value);
    if ((sint32)value >= 0)
        return value;
    mission_flow.mode_phase = 11;
    dialogue_ui.render_state = (uint32)(2);
    return 2;
}

void dialogue_ui_stop_audio(void)
{
    FUNCTION_MARKER(0x800F07B8, "ACE3.BIN");
    if (dialogue_audio.enabled == 0)
        return;
    resource_init_queue(0, 0);
    return;
}

void dialogue_ui_stop_audio_frame(void)
{
    FUNCTION_MARKER(0x800F07B0, "ACE3.BIN");
    {
        (void)(dialogue_ui_stop_audio());
        return;
    }
}

uint32 dialogue_ui_reset_entry_animations(uint32 kind)
{
    uint32 index;

    FUNCTION_MARKER(0x800F0174, "ACE3.BIN");
    for (index = 1; index <= 5; ++index)
        ui_animation_set(index, 0, 0, 0);
    return kind >= 3 && kind <= 6 ? kind - 1 : 0;
}

void dialogue_ui_wait_intro_audio(void)
{
    uint32 transition, owner, result, timer, flag, shift, bits;
    uint32 id, kind, value, slot;

    FUNCTION_MARKER(0x800F1848, "ACE3.BIN");
    transition = ui_animation_phase(3);
    dialogue_ui_draw_panel_frame();
    owner = dialogue_ui.arena_begin;
    result = dialogue_audio.enabled;
    if (result != 0)
    {
        if (resource_get_stream_elapsed() == 0)
        {
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.wait_ticks;
            if ((sint32)timer > 0)
            {
                dialogue_ui.wait_ticks = (uint32)(timer - 1);
                return;
            }
        }
        owner = dialogue_ui.arena_begin;
        flag = dialogue_audio.enabled;
        result = 0x80110000;
        if (flag != 0)
        {
            id = dialogue_ui.entry.audio_resource;
            value = resource_get_stream_time(id);
            if (value == 0xFFFFFFFF)
                ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            else
            {
                result = game_input.pressed & 16;
                if (result != 0)
                {
                    ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
                    resource_init_queue(0, 0);
                }
            }
        }
    }
    if (transition != 0)
        return;
    dialogue_ui_stop_audio_frame();
    owner = dialogue_ui.arena_begin;
    shift = dialogue_ui.active_item;
    bits = dialogue_ui.item_mask;
    ui_group_set_mask(0, bits | (1u << (shift & 31)));
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    value = dialogue_ui_reset_entry_animations(kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.anim_slot = (uint32)(value);
    ui_animation_set(1, 1, 0, 15);
    owner = dialogue_ui.arena_begin;
    slot = dialogue_ui.anim_slot;
    if (slot != 0)
        ui_animation_set(slot, 1, 0, 15);
    sound_play(100);
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    if (kind == 0 || kind == 1)
    {
        *ui_widget_value(50) = (uint8)(kind);
    }
    mission_flow.mode_phase = 8;
    return;
}

uint32 dialogue_ui_open_entry_fade_tick(void)
{
    PSX_RECT rectangle = {832, 256, 196, 256};
    uint32 owner, value, index, result;

    FUNCTION_MARKER(0x800F1A7C, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    if (ui_animation_phase(1) == 2)
    {
        mission_flow.mode_phase = 9;
        owner = dialogue_ui.arena_begin;
        mission_flow.ui_timer = (uint32)(0);
        dialogue_ui.frame_ticks = (uint32)(0xFFFFFFFF);
        value = *menu_voice_slot(0);
        result = sound_set_voice_fade(value, 50, 30);
        for (index = 1; index < 5; ++index)
        {
            value = *menu_voice_slot(index);
            result = sound_set_voice_fade(value, 50, 30);
        }
        return result;
    }
    value = mission_flow.ui_timer + 1;
    mission_flow.ui_timer = (uint32)(value);
    return (uint32)ClearImage(&rectangle, 0, 0, 0);
}

uint32 dialogue_ui_begin_entry(void)
{
    uint32 owner, id, kind, resource, first, second;

    FUNCTION_MARKER(0x800F1B8C, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    dialogue_ui_configure_entry_layout(kind);
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    resource = dialogue_sequence_entry(id)->audio_resource;
    dialogue_ui.entry.audio_resource = (uint32)(resource);
    dialogue_ui.frame_ticks = (uint32)(0);
    first = dialogue_ui.entry.audio_argument[0];
    id = dialogue_sequence_id(0);
    second = dialogue_ui.entry.audio_argument[1];
    kind = dialogue_sequence_entry(id)->audio_kind;
    subtitles_configure(kind, first, second);
    if (dialogue_ui_queue_audio_frame() != 0)
    {
        mission_flow.mode_phase = 10;
        owner = dialogue_ui.arena_begin;
        dialogue_ui.wait_ticks = (uint32)(180);
    }
    else
    {
        mission_flow.mode_phase = 11;
    }
    dialogue_ui.entry.complete = (uint32)(0);
    return 1;
}

void dialogue_ui_configure_entry_layout(uint32 kind)
{
    uint32 owner, resource, value, first, second;

    FUNCTION_MARKER(0x800F0244, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    dialogue_ui.entry.image_mode = (uint32)(0);
    dialogue_ui.render_state = (uint32)(0);
    dialogue_ui.entry.image_mode = (uint32)(0);
    dialogue_audio.enabled = 0;
    resource = dialogue_sequence_id(0);
    dialogue_sequence_unlock_archive(resource);
    kind = (uint32)(sint32)(sint16)kind;
    if (kind >= 12)
        return;
    switch (kind)
    {
        case 0:
        case 1:
            owner = dialogue_ui.arena_begin;
            dialogue_ui.render_state = (uint32)(1);
            dialogue_audio.enabled = 1;
            dialogue_video.rect_count = (uint32)(1);
            dialogue_video.scr[0].x = (uint16)(120);
            dialogue_video.scr[0].y = (uint16)(93);
            dialogue_video.scr[0].w = (uint16)(80);
            dialogue_video.scr[0].h = (uint16)(160);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].w = (uint16)(80);
            dialogue_video.tex[0].h = (uint16)(80);
            dialogue_video_init(kind);
            {
                (void)(uint32)(sound_play(kind == 0 ? 106 : 103));
                return;
            }
        case 2:
            owner = dialogue_ui.arena_begin;
            dialogue_ui.render_state = (uint32)(1);
            dialogue_audio.enabled = 1;
            dialogue_video.rect_count = (uint32)(2);
            dialogue_video.scr[0].x = (uint16)(70);
            dialogue_video.scr[0].y = (uint16)(93);
            dialogue_video.scr[0].w = (uint16)(80);
            dialogue_video.scr[0].h = (uint16)(160);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].w = (uint16)(80);
            dialogue_video.tex[0].h = (uint16)(80);
            dialogue_video.scr[1].x = (uint16)(170);
            dialogue_video.scr[1].y = (uint16)(93);
            dialogue_video.scr[1].w = (uint16)(80);
            dialogue_video.scr[1].h = (uint16)(160);
            dialogue_video.tex[1].x = (uint16)(80);
            dialogue_video.tex[1].y = (uint16)(0);
            dialogue_video.tex[1].w = (uint16)(80);
            dialogue_video.tex[1].h = (uint16)(80);
            dialogue_video_init(kind);
            {
                (void)(uint32)(sound_play(103));
                return;
            }
        case 3:
            owner = dialogue_ui.arena_begin;
            dialogue_video.scr[0].x = (uint16)(74);
            dialogue_video.scr[1].x = (uint16)(150);
            dialogue_video.scr[1].h = (uint16)(128);
            dialogue_video.tex[1].h = (uint16)(64);
            dialogue_video.scr[0].w = (uint16)(48);
            dialogue_video.tex[0].w = (uint16)(48);
            dialogue_video.tex[0].h = (uint16)(48);
            dialogue_video.tex[1].x = (uint16)(48);
            dialogue_video.rect_count = (uint32)(2);
            dialogue_video.scr[0].y = (uint16)(99);
            dialogue_video.scr[0].h = (uint16)(96);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.scr[1].y = (uint16)(99);
            dialogue_video.scr[1].w = (uint16)(96);
            dialogue_video.tex[1].y = (uint16)(0);
            dialogue_video.tex[1].w = (uint16)(96);
            resource = dialogue_sequence_id(0);
            value = dialogue_sequence_entry(resource)->image_layout;
            if (value == 2)
            {
                dialogue_ui.render_state = (uint32)(1);
                dialogue_audio.enabled = 1;
                dialogue_video_init(kind);
            }
            else if (value == 1)
            {
                dialogue_ui.entry.image_mode = (uint32)(0x4000);
            }
            first = dialogue_ui.entry.audio_argument[0];
            second = dialogue_ui.entry.audio_argument[1];
            {
                (void)(uint32)(subtitles_configure(5, first, second));
                return;
            }
        case 4:
            owner = dialogue_ui.arena_begin;
            dialogue_video.scr[0].y = (uint16)(70);
            dialogue_video.scr[0].h = (uint16)(192);
            dialogue_video.scr[0].w = (uint16)(128);
            dialogue_video.tex[0].w = (uint16)(128);
            dialogue_video.rect_count = (uint32)(1);
            dialogue_video.scr[0].x = (uint16)(96);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].h = (uint16)(96);
            resource = dialogue_sequence_id(0);
            value = dialogue_sequence_entry(resource)->image_layout;
            if (value == 2)
            {
                dialogue_ui.render_state = (uint32)(1);
                dialogue_audio.enabled = 1;
                {
                    (void)(dialogue_video_init(kind));
                    return;
                }
            }
            dialogue_ui.entry.image_mode = (uint32)(0x4000);
            return;
        case 5:
            owner = dialogue_ui.arena_begin;
            dialogue_video.rect_count = (uint32)(3);
            dialogue_video.scr[0].x = (uint16)(52);
            dialogue_video.scr[1].x = (uint16)(221);
            dialogue_video.scr[0].h = (uint16)(96);
            dialogue_video.scr[1].h = (uint16)(96);
            dialogue_video.tex[1].x = (uint16)(112);
            dialogue_video.scr[2].y = (uint16)(115);
            dialogue_video.scr[2].w = (uint16)(64);
            dialogue_video.tex[2].w = (uint16)(64);
            dialogue_video.tex[2].h = (uint16)(64);
            dialogue_video.scr[0].y = (uint16)(93);
            dialogue_video.scr[0].w = (uint16)(48);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].w = (uint16)(48);
            dialogue_video.tex[0].h = (uint16)(48);
            dialogue_video.scr[1].y = (uint16)(93);
            dialogue_video.scr[1].w = (uint16)(48);
            dialogue_video.tex[1].y = (uint16)(0);
            dialogue_video.tex[1].w = (uint16)(48);
            dialogue_video.tex[1].h = (uint16)(48);
            dialogue_video.scr[2].x = (uint16)(128);
            dialogue_video.scr[2].h = (uint16)(128);
            dialogue_video.tex[2].x = (uint16)(48);
            dialogue_video.tex[2].y = (uint16)(0);
            dialogue_ui.render_state = (uint32)(1);
            dialogue_audio.enabled = 1;
            {
                (void)(dialogue_video_init(kind));
                return;
            }
        case 6:
            owner = dialogue_ui.arena_begin;
            dialogue_video.scr[0].x = (uint16)(120);
            dialogue_video.scr[0].y = (uint16)(87);
            dialogue_video.rect_count = (uint32)(1);
            dialogue_video.scr[0].w = (uint16)(80);
            dialogue_video.scr[0].h = (uint16)(160);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].w = (uint16)(80);
            dialogue_video.tex[0].h = (uint16)(80);
            dialogue_ui.render_state = (uint32)(1);
            dialogue_audio.enabled = 1;
            {
                (void)(dialogue_video_init(kind));
                return;
            }
        case 7:
        case 11:
            owner = dialogue_ui.arena_begin;
            dialogue_audio.enabled = 1;
            return;
        case 9:
            owner = dialogue_ui.arena_begin;
            dialogue_video.rect_count = (uint32)(1);
            dialogue_video.scr[0].y = (uint16)(100);
            dialogue_video.scr[0].x = (uint16)(96);
            dialogue_video.scr[0].w = (uint16)(128);
            dialogue_video.scr[0].h = (uint16)(192);
            dialogue_video.tex[0].x = (uint16)(0);
            dialogue_video.tex[0].y = (uint16)(0);
            dialogue_video.tex[0].w = (uint16)(128);
            dialogue_video.tex[0].h = (uint16)(96);
            dialogue_ui.entry.image_mode = (uint32)(0x4000);
            return;
        case 10:
        {
        }
            mission_flow.playback_requested = (uint32)(1);
            value = game_runtime.progress.sequence_idx;
            game_runtime.progress.sequence_idx = (uint8)(value + 1);
            game_runtime.account.route_mode = (uint16)(2);
            return;
        case 8:
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 selection layout kind %u\n", kind);
#endif
            abort();
    }
}

uint32 dialogue_ui_bind_entry_asset(void)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 wire_word_6;
    uint32 resource, data, id, kind, index, buffer;
    uint32 directory_count, resource_offset, next_offset, textures_address, captions_address;
    uint32 image_offset, payload_bytes;
    uint8 *payload;
    MISSION_IMAGE_DIRECTORY textures;

    FUNCTION_MARKER(0x800F1430, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    if (game_runtime.resource_pending != 0)
        return 0;
    resource = game_runtime.resource_slot.encoded_base;
    if (dialogue_video.load_bytes < 20)
        abort();
    payload_bytes = dialogue_video.load_bytes;
    payload = game_runtime.resource_slot.data;
    if (!payload || payload_bytes > game_runtime.resource_slot.read_bytes)
        abort();
    directory_count = (memcpy(&wire_halfword_0, payload, sizeof(wire_halfword_0)), wire_halfword_0);
    if (directory_count < 4 || directory_count > (dialogue_video.load_bytes - 4) / 4)
        abort();
    mission_flow.assets.arena_begin = resource;
    image_offset = (memcpy(&wire_word_1, payload + 16, sizeof(wire_word_1)), wire_word_1);
    data = mission_image_address(resource, image_offset);
    if (data - resource > payload_bytes || sizeof(uint32) > payload_bytes - (data - resource))
        abort();
    if ((memcpy(&wire_word_2, payload + (data - resource), sizeof(wire_word_2)), wire_word_2) != 0)
        mission_image_optional_data_hook();
    image_offset = (memcpy(&wire_word_3, payload + 4, sizeof(wire_word_3)), wire_word_3);
    data = mission_image_address(resource, image_offset);
    image_offset = (memcpy(&wire_word_4, payload + 8, sizeof(wire_word_4)), wire_word_4);
    textures_address = mission_image_address(resource, image_offset);
    image_offset = (memcpy(&wire_word_5, payload + 12, sizeof(wire_word_5)), wire_word_5);
    captions_address = mission_image_address(resource, image_offset);
    resource_offset = data - resource;
    next_offset = textures_address - resource;
    if (resource_offset > next_offset || next_offset > dialogue_video.load_bytes)
        abort();
    dialogue_video.resource.bytes = next_offset - resource_offset;
    dialogue_video.resource.data = payload + resource_offset;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->widget_value;
    *ui_widget_value(51) = (uint8)(kind);
    dialogue_ui.entry.audio_argument[0] = 0;
    dialogue_ui.entry.audio_argument[1] = 0;
    textures = mission_image_directory_bind(textures_address, captions_address, resource, payload, payload_bytes);
    dialogue_video_move_capture();
    DrawSync(0);
    dialogue_video_clear_capture();
    if (textures.count >= 1 && textures.count <= 4)
    {
        for (index = 0; index < textures.count; ++index)
            scene_texture_load_image(mission_image_directory_at(&textures, index), (RESOURCE_ASSET_SOURCE){payload, payload_bytes, resource}, SCENE_TEXTURE_DECLARED_SIZE);
    }
    if (captions_address - resource > payload_bytes || sizeof(uint32) > payload_bytes - (captions_address - resource))
        abort();
    if ((memcpy(&wire_word_6, payload + (captions_address - resource), sizeof(wire_word_6)), wire_word_6) != 0)
    {
        scene_texture_load_group_sized(captions_address, 0x4000, (RESOURCE_ASSET_SOURCE){payload, payload_bytes, resource}, SCENE_TEXTURE_DECLARED_SIZE);
        dialogue_caption_init();
    }
    arena_set_cursor(mission_image_directory_at(&textures, 0));
    buffer = arena_alloc(0x8100);
    dialogue_video_store_capture(buffer != 0);
    DrawSync(0);
    mission_flow.mode_phase = 6;
    return 6;
}

uint32 dialogue_ui_begin_outro_audio(void)
{
    uint32 owner, bits, id, resource;

    FUNCTION_MARKER(0x800F2240, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    if ((game_input.pressed & 16) != 0)
        ui_animation_set(1, 0, 0, 0);
    if (ui_animation_phase(1) != 0)
        return 1;
    owner = dialogue_ui.arena_begin;
    bits = dialogue_ui.item_mask;
    ui_group_set_mask(0, bits);
    owner = dialogue_ui.arena_begin;
    dialogue_audio.enabled = 1;
    id = dialogue_sequence_id(-1);
    resource = dialogue_sequence_entry(id)->resource;
    if (resource == 0x135)
    {
        dialogue_ui.entry.audio_resource = (uint32)(0x916);
    }
    else if (resource == 0x10E || resource == 0x14B || resource == 0x15D)
    {
        dialogue_ui.entry.audio_resource = (uint32)(0x914);
    }
    else
    {
        owner = dialogue_ui.arena_begin;
        dialogue_audio.enabled = 0;
    }
    if (dialogue_ui_queue_audio_frame() != 0)
    {
        ui_group_set_mask(0, 0x808000);
        ui_animation_set(3, 1, 0, 10);
        *ui_widget_value(50) = (uint8)(1);
    }
    else
        ui_animation_set(3, 0, 0, 0);
    mission_flow.mode_phase = 13;
    owner = dialogue_ui.arena_begin;
    dialogue_ui.wait_ticks = (uint32)(180);
    return 180;
}

void dialogue_ui_wait_outro_audio(void)
{
    uint32 transition, owner, result, flag, timer, value;
    uint32 id, kind, resource;

    FUNCTION_MARKER(0x800F23CC, "ACE3.BIN");
    transition = ui_animation_phase(3);
    dialogue_ui_draw_panel_frame();
    owner = dialogue_ui.arena_begin;
    result = dialogue_audio.enabled;
    if (result != 0)
    {
        if (resource_get_stream_elapsed() == 0)
        {
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.wait_ticks;
            dialogue_ui.wait_ticks = (uint32)(timer - 1);
            return;
        }
        owner = dialogue_ui.arena_begin;
        flag = dialogue_audio.enabled;
        result = 0x80110000;
        if (flag != 0)
        {
            resource = dialogue_ui.entry.audio_resource;
            value = resource_get_stream_time(resource);
            if (value == 0xFFFFFFFF)
                ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            else
            {
                result = game_input.pressed & 16;
                if (result != 0)
                {
                    ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
                    resource_init_queue(0, 0);
                }
            }
        }
    }
    if (transition != 0)
        return;
    dialogue_ui_stop_audio_frame();
    dialogue_ui_queue_entry_frame();
    owner = dialogue_ui.arena_begin;
    flag = dialogue_sequence_remaining();
    if (flag == 0)
    {
        mission_flow.mode_phase = 14;
        mission_flow.ui_timer = (uint32)(30);
        return;
    }
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    result = dialogue_ui_layout_item(kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.active_item = (uint32)(result);
    if (flag != 0)
    {
        id = dialogue_sequence_id(0);
        kind = dialogue_sequence_entry(id)->image_kind;
        if (kind != 10)
        {
            if (kind >= 12)
                return;
            value = dialogue_ui.prev_kind;
            if (kind >= 3 && value < 3)
            {
                mission_flow.mode_phase = 14;
                mission_flow.ui_timer = 30;
                return;
            }
            if (kind < 3 && value >= 3)
            {
                mission_flow.mode_phase = 14;
                dialogue_ui.frame_ticks = 30;
                return;
            }
            mission_flow.mode_phase = 5;
            return;
        }
    }
    mission_flow.mode_phase = 14;
    mission_flow.ui_timer = (uint32)(30);
    return;
}

void dialogue_ui_advance_panel_tick(void)
{
    uint32 owner, timer, id, kind, phase, value, item;

    FUNCTION_MARKER(0x800F2618, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    owner = dialogue_ui.arena_begin;
    timer = dialogue_ui.frame_ticks;
    if ((sint32)timer > 0)
    {
        dialogue_ui.frame_ticks = (uint32)(timer - 1);
        return;
    }
    if (dialogue_sequence_remaining() != 0)
    {
        id = dialogue_sequence_id(0);
        kind = dialogue_sequence_entry(id)->image_kind;
        if (kind != 10)
        {
            mission_flow.mode_phase = 4;
            value = dialogue_ui.panel_style;
            item = dialogue_ui.active_item;
            dialogue_ui.panel_style = (uint32)(1 - value);
            {
                mission_menu_start_item_motion(0, item);
                return;
            }
        }
    }
    phase = mission_flow.mode_phase;
    mission_flow.mode_phase = phase + 1;
    ui_item_reverse_motion(0, 15);
    {
        sound_play(91);
        return;
    }
}

uint32 dialogue_ui_entry_tick(void)
{
    uint32 display_carrier;
    uint32 owner, id, kind, count, index, packet, ordering;
    const DIALOGUE_VIDEO_RECT *screen, *texture;
    uint32 value, result, byte, slot, item, other;
    uint8 *table;

    FUNCTION_MARKER(0x800F1D04, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    display_carrier = 0x80110000;
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    if (kind == 8)
        dialogue_caption_draw();
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    if (kind == 11)
        dialogue_caption_draw_overlay();
    if (dialogue_ui.entry.image_mode != 0)
    {
        owner = dialogue_ui.arena_begin;
        id = dialogue_sequence_id(0);
        kind = dialogue_sequence_entry(id)->image_kind;
        overlay_movie_update_image_mode(kind == 9 ? 64 : 16);
        if (dialogue_ui.entry.complete == 0)
        {
            owner = dialogue_ui.arena_begin;
            count = dialogue_video.rect_count;
            index = 0;
            while ((sint32)index < (sint32)count)
            {
                owner = dialogue_ui.arena_begin;
                screen = dialogue_video.scr + index;
                texture = dialogue_video.tex + index;
                ordering = ui_packets.scene_ot;
                packet = ui_packets.scene_packet_cursor;
                result = dialogue_video_draw_rect(packet, ordering + 8, screen, texture);
                owner = dialogue_ui.arena_begin;
                ui_packets.scene_packet_cursor = (uint32)(result);
                count = dialogue_video.rect_count;
                ++index;
                display_carrier = 5716 + index * 8;
            }
        }
    }
    value = dialogue_ui_draw_effects(display_carrier);
    if (value == 1)
    {
        owner = dialogue_ui.arena_begin;
        dialogue_ui.render_state = (uint32)(2);
    }
    else if (value == 2)
    {
        resource_rewind_current_asset();
        owner = dialogue_ui.arena_begin;
        dialogue_ui.render_state = (uint32)(0);
        dialogue_ui.entry.complete = (uint32)(1);
    }
    owner = dialogue_ui.arena_begin;
    if (dialogue_ui.render_state == 1)
    {
        if ((game_input.pressed & 16) != 0)
            dialogue_ui.render_state = (uint32)(2);
    }
    ordering = ui_packets.scene_ot;
    packet = ui_packets.scene_packet_cursor;
    result = subtitles_draw(ordering, packet);
    owner = dialogue_ui.arena_begin;
    ui_packets.scene_packet_cursor = (uint32)(result);
    value = (sint16)dialogue_ui.wait_for_completion;
    if (value != 0 && dialogue_ui.entry.complete != 1)
    {
        result = dialogue_ui.frame_ticks + 1;
        dialogue_ui.frame_ticks = (uint32)(result);
        return result;
    }
    id = dialogue_sequence_id(0);
    mission_flow.mode_phase = 12;
    other = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(other)->image_kind;
    dialogue_ui.prev_kind = (uint32)(kind);
    table = game_runtime.progress.scene_status + id;
    byte = *table;
    if (byte == 0)
    {
        value = dialogue_sequence_id(0);
        if (value != 88 && value != 167 && value != 127 && value != 149)
            *table = 1;
    }
    else if (byte == 1)
        *table = 2;
    owner = dialogue_ui.arena_begin;
    dialogue_video_restore_capture();
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    value = dialogue_sequence_entry(id)->resource;
    if (value == 0x153)
    {
        if ((game_runtime.progress.flow_flags & 32) != 0)
        {
            mission_flow.next_scene = 0x60000005;
            game_runtime.playback.return_mode = (uint32)(3);
            game_runtime.playback.movie_idx = (uint8)(29);
            game_runtime.account.route_mode = (uint16)(1);
            game_runtime.progress.sequence_idx = (uint8)(0);
            return 1;
        }
    }
    owner = dialogue_ui.arena_begin;
    value = dialogue_ui.completed_entries;
    dialogue_ui.completed_entries = (uint16)(value + 1);
    value = game_runtime.progress.sequence_idx;
    game_runtime.progress.sequence_idx = (uint8)(value + 1);
    owner = dialogue_ui.arena_begin;
    item = dialogue_ui.active_item;
    dialogue_ui.wait_for_completion = (uint16)(1);
    dialogue_ui.frame_ticks = (uint32)(0);
    ui_item_reverse_motion(0, item);
    mission_flow.ui_timer = (uint32)(60);
    ui_animation_set(1, 3, 0xFFFFFFFF, 0xFFFFFFFF);
    owner = dialogue_ui.arena_begin;
    slot = dialogue_ui.anim_slot;
    if (slot != 0)
    {
        ui_animation_set(slot, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        owner = dialogue_ui.arena_begin;
    }
    dialogue_ui.render_state = (uint32)(0);
    arena_set_cursor(mission_flow.assets.arena_begin);
    owner = dialogue_ui.arena_begin;
    dialogue_sequence_advance();
    dialogue_ui_stop_audio_frame();
    sound_play(101);
    for (index = 0; index < 5; ++index)
    {
        value = *menu_voice_slot(index);
        result = sound_set_voice_fade(value, 100, 30);
    }
    return result;
}

void dialogue_ui_dispatch_phase(void)
{
    sint16 phase;

    FUNCTION_MARKER(0x800D1214, "ACE3.BIN");
    phase = (sint16)mission_flow.mode_phase;
    switch (phase)
    {
        case 0:
        {
            dialogue_ui_begin_sequence();
            return;
        }
        case 3:
        case -13:
        {
            dialogue_ui_wait_selection_tick();
            return;
        }
        case 4:
        case -28:
        case -12:
        {
            dialogue_ui_open_step();
            return;
        }
        case 5:
        case -27:
        case -11:
        {
            dialogue_ui_bind_entry_asset();
            return;
        }
        case 6:
        case -26:
        case -10:
        {
            dialogue_ui_begin_intro_audio();
            return;
        }
        case 7:
        case -25:
        case -9:
        {
            dialogue_ui_wait_intro_audio();
            return;
        }
        case 8:
        case -24:
        case -8:
        {
            dialogue_ui_open_entry_fade_tick();
            return;
        }
        case 9:
        case -23:
        case -7:
        {
            dialogue_ui_begin_entry();
            return;
        }
        case 10:
        case -22:
        case -6:
        {
            dialogue_ui_wait_entry_audio();
            return;
        }
        case 11:
        case -21:
        case -5:
        {
            dialogue_ui_entry_tick();
            return;
        }
        case 12:
        case -20:
        case -4:
        {
            dialogue_ui_begin_outro_audio();
            return;
        }
        case 13:
        case -19:
        case -3:
        {
            dialogue_ui_wait_outro_audio();
            return;
        }
        case 14:
        case -18:
        case -2:
        {
            dialogue_ui_advance_panel_tick();
            return;
        }
        case 15:
        {
            dialogue_ui_finish_sequence();
            return;
        }
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource002 detail phase %d\n", phase);
#endif
            abort();
    }
}

uint32 dialogue_ui_is_detail_layout(uint8 kind)
{
    FUNCTION_MARKER(0x800F0144, "ACE3.BIN");
    return kind >= 3 && kind < 11;
}

uint32 dialogue_ui_layout_item(uint32 kind)
{
    sint32 value;

    FUNCTION_MARKER(0x800F00A8, "ACE3.BIN");
    value = (sint16)(uint16)kind;
    switch (value)
    {
        case 0:
        case 1:
            return 18;
        case 2:
            return 19;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            return 16;
        case 8:
        case 11:
            return 17;
        case 9:
            return 22;
        case 10:
            return 18;
        default:
            return 10;
    }
}

void dialogue_ui_queue_entry_asset(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 id, resource, size, buffer;

    FUNCTION_MARKER(0x800F07FC, "ACE3.BIN");
    if (dialogue_sequence_remaining() == 0)
        return;
    id = dialogue_sequence_id(0);
    resource = dialogue_sequence_entry(id)->resource;
    game_runtime.resource_slot.resource_id = (uint32)(resource);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    dialogue_video.load_bytes = size;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    return;
}

void dialogue_ui_queue_entry_frame(void)
{
    FUNCTION_MARKER(0x800F07F4, "ACE3.BIN");
    {
        (void)(dialogue_ui_queue_entry_asset());
        return;
    }
}

uint32 dialogue_ui_begin_sequence(void)
{
    uint32 owner, pointer, index, id, kind, value;
    const DIALOGUE_CATALOG_ENTRY *entry;

    FUNCTION_MARKER(0x800F0DC4, "ACE3.BIN");
    owner = arena_alloc(6304);
    dialogue_ui.arena_begin = (uint32)(owner);
    dialogue_ui.caption.packets = NULL;
    subtitles_initialize();
    owner = dialogue_ui.arena_begin;
    dialogue_ui.wait_for_completion = (uint16)(1);
    dialogue_ui.completed_entries = (uint16)(0);
    dialogue_ui.frame_ticks = (uint32)(0);
    dialogue_sequence_bind_single();
    dialogue_ui_queue_entry_frame();
    owner = dialogue_ui.arena_begin;
    owner = dialogue_ui.arena_begin;
    dialogue_ui.render_state = (uint32)(0);
    ui_item_reset_motion(0, 21);
    ui_item_reset_motion(0, 14);
    ui_animation_set(11, 1, 0, 25);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.item_mask = (uint32)(0x204001);
    ui_group_set_mask(0, 0x204001);
    owner = dialogue_ui.arena_begin;
    owner = dialogue_ui.arena_begin;
    owner = dialogue_ui.arena_begin;
    pointer = owner + 2400;
    for (index = 0; index < 16; ++index)
    {
        pointer -= 160;
    }
    owner = dialogue_ui.arena_begin;
    pointer = owner + 480;
    for (index = 0; index < 16; ++index)
    {
        pointer -= 32;
    }
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    entry = dialogue_sequence_entry(id);
    kind = entry->image_kind;
    value = dialogue_ui_is_detail_layout((uint8)kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.panel_style = (uint32)(value);
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    value = dialogue_ui_layout_item(kind);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.active_item = (uint32)(value);
    mission_flow.mode_phase = 4;
    return sound_play(90);
}

uint32 dialogue_ui_draw_panel_frame(void)
{
    FUNCTION_MARKER(0x800EE9E4, "ACE3.BIN");
    return dialogue_ui_draw_panel();
}

uint32 dialogue_ui_queue_audio(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800F0734, "ACE3.BIN");
    if (dialogue_audio.enabled == 0)
        return 0;
    value = dialogue_ui.entry.audio_resource;
    value = value == 0 ? 0x912 : (uint16)dialogue_ui.entry.audio_resource;
    dialogue_audio.track.id = (sint16)value;
    dialogue_audio.track.fade = 0;
    dialogue_audio.track.duration = 0;
    dialogue_audio.track.sector_offset = 0;
    dialogue_audio.track.next = NULL;
    resource_queue_audio(&dialogue_audio.track, 1, 0);
    return 1;
}

uint32 dialogue_ui_queue_audio_frame(void)
{
    FUNCTION_MARKER(0x800F072C, "ACE3.BIN");
    return dialogue_ui_queue_audio();
}

uint32 dialogue_ui_begin_intro_audio(void)
{
    uint32 owner, id, resource, bits;

    FUNCTION_MARKER(0x800F16C4, "ACE3.BIN");
    dialogue_ui_draw_panel_frame();
    owner = dialogue_ui.arena_begin;
    dialogue_audio.enabled = 1;
    id = dialogue_sequence_id(0);
    resource = dialogue_sequence_entry(id)->resource;
    switch (resource)
    {
        case 0x16A:
        case 0x174:
        case 0x178:
        {
        }
            dialogue_ui.entry.audio_resource = (uint32)(0x912);
            break;
        case 0x10E:
        case 0x14B:
        {
        }
            dialogue_ui.entry.audio_resource = (uint32)(0x913);
            break;
        case 0x135:
        {
        }
            dialogue_ui.entry.audio_resource = (uint32)(0x915);
            break;
        default:
            owner = dialogue_ui.arena_begin;
            dialogue_audio.enabled = 0;
            break;
    }
    if (dialogue_ui_queue_audio_frame() != 0)
    {
        owner = dialogue_ui.arena_begin;
        bits = dialogue_ui.item_mask;
        ui_group_set_mask(0, bits | 0x800000);
        ui_animation_set(3, 1, 0, 10);
        *ui_widget_value(50) = (uint8)(0);
    }
    owner = dialogue_ui.arena_begin;
    if (dialogue_audio.enabled != 0)
        sound_play(55);
    mission_flow.mode_phase = 7;
    owner = dialogue_ui.arena_begin;
    dialogue_ui.wait_ticks = (uint32)(180);
    return 180;
}

static uint32 dialogue_panel_scaled_quotient(uint32 value, uint32 multiplier, uint32 shift, uint32 add_value)
{
    sint64 product;
    uint32 high;

    product = (sint64)(sint32)value * (sint32)multiplier;
    high = (product >> 32);
    if (add_value != 0)
        high += value;
    return (uint32)((sint32)high >> shift) - (uint32)((sint32)value >> 31);
}

uint32 dialogue_ui_draw_panel(void)
{
    uint32 phase, owner, timer, width, height, mode, count, index, angle;
    uint32 value, x, y, y_outer, left, right, top, outer_left, outer_right, outer_top;

    FUNCTION_MARKER(0x800EE9EC, "ACE3.BIN");
    phase = (sint16)mission_flow.mode_phase;
    if ((sint32)phase >= 0 && (phase < 3 || phase == 15))
        return 0;
    phase = mission_flow.mode_phase;
    if (phase - 5 >= 9)
    {
        owner = dialogue_ui.arena_begin;
        timer = dialogue_ui.frame_ticks;
        if ((sint32)timer < 23)
        {
            width = dialogue_panel_scaled_quotient((timer * 5) << 5, 0xB21642C9, 4, 1);
            ui_draw_panel_tile(160 - width, 240, width, 2, 0);
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.frame_ticks;
            width = dialogue_panel_scaled_quotient((timer * 5) << 5, 0xB21642C9, 4, 1);
            ui_draw_panel_tile(160, 240, width, 2, 0);
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.frame_ticks;
            height = dialogue_panel_scaled_quotient((timer * 15) << 4, 0xB21642C9, 4, 1);
            ui_draw_panel_line(160, 240 - height, 160, 240);
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.frame_ticks;
            height = dialogue_panel_scaled_quotient((timer * 15) << 4, 0xB21642C9, 4, 1);
            ui_draw_panel_line(160, 240, 160, 240 + height);
            goto frame_detail;
        }
    }
    ui_draw_panel_tile(0, 240, 160, 2, 0);
    ui_draw_panel_tile(160, 240, 160, 2, 0);
    ui_draw_panel_line(160, 0, 160, 240);
    ui_draw_panel_line(160, 240, 160, 480);
frame_detail:
    owner = dialogue_ui.arena_begin;
    mode = dialogue_ui.panel_style;
    if (mode == 1)
    {
        phase = mission_flow.mode_phase;
        if (phase - 5 >= 9)
        {
            timer = dialogue_ui.frame_ticks;
            if ((sint32)timer < 11)
            {
                width = dialogue_panel_scaled_quotient((timer * 15) << 3, 0x2E8BA2E9, 1, 0);
                height = dialogue_panel_scaled_quotient((timer * 5) << 5, 0x2E8BA2E9, 1, 0);
                width = (sint16)width;
                x = 140 - width;
                ui_draw_panel_tile(x, 220, width, 2, 0);
                ui_draw_panel_tile(180, 220, width, 2, 0);
                ui_draw_panel_tile(x, 260, width, 2, 0);
                ui_draw_panel_tile(180, 260, width, 2, 0);
                height = (sint16)height;
                ui_draw_panel_line(140, 220 - height, 140, 220);
                ui_draw_panel_line(180, 220 - height, 180, 220);
                ui_draw_panel_line(140, 260, 140, 260 + height);
                ui_draw_panel_line(180, 260, 180, 260 + height);
                goto frame_mode;
            }
            if ((sint32)timer < 23)
            {
                value = (timer - 11) * 5;
                height = dialogue_panel_scaled_quotient(value << 5, 0x2AAAAAAB, 1, 0);
                ui_draw_panel_tile(20, 220, 120, 2, 0);
                ui_draw_panel_tile(180, 220, 120, 2, 0);
                ui_draw_panel_tile(20, 260, 120, 2, 0);
                ui_draw_panel_tile(180, 260, 120, 2, 0);
                ui_draw_panel_line(140, 60, 140, 220);
                ui_draw_panel_line(180, 60, 180, 220);
                ui_draw_panel_line(140, 260, 140, 420);
                ui_draw_panel_line(180, 260, 180, 420);
                width = (sint16)(value << 1);
                x = 140 - width;
                ui_draw_panel_tile(x, 60, width, 2, 0);
                ui_draw_panel_tile(180, 60, width, 2, 0);
                ui_draw_panel_tile(x, 420, width, 2, 0);
                ui_draw_panel_tile(180, 420, width, 2, 0);
                height = (sint16)height;
                ui_draw_panel_line(20, 220 - height, 20, 220);
                ui_draw_panel_line(300, 220 - height, 300, 220);
                ui_draw_panel_line(20, 260, 20, 260 + height);
                ui_draw_panel_line(300, 260, 300, 260 + height);
                goto frame_mode;
            }
        }
        ui_draw_panel_tile(20, 220, 120, 2, 0);
        ui_draw_panel_tile(180, 220, 120, 2, 0);
        ui_draw_panel_tile(20, 260, 120, 2, 0);
        ui_draw_panel_tile(180, 260, 120, 2, 0);
        ui_draw_panel_line(140, 60, 140, 220);
        ui_draw_panel_line(180, 60, 180, 220);
        ui_draw_panel_line(140, 260, 140, 420);
        ui_draw_panel_line(180, 260, 180, 420);
        ui_draw_panel_tile(20, 60, 120, 2, 0);
        ui_draw_panel_tile(180, 60, 120, 2, 0);
        ui_draw_panel_tile(20, 420, 120, 2, 0);
        ui_draw_panel_tile(180, 420, 120, 2, 0);
        ui_draw_panel_line(20, 60, 20, 220);
        ui_draw_panel_line(300, 60, 300, 220);
        ui_draw_panel_line(20, 260, 20, 420);
        ui_draw_panel_line(300, 260, 300, 420);
    }
frame_mode:
    owner = dialogue_ui.arena_begin;
    mode = dialogue_ui.panel_style;
    if (mode != 0)
        return mode;
    phase = mission_flow.mode_phase;
    if (phase - 5 >= 9)
    {
        timer = dialogue_ui.frame_ticks;
        if ((sint32)timer < 7)
        {
            width = dialogue_panel_scaled_quotient((timer * 5) << 4, 0x92492493, 2, 1);
            ui_draw_panel_tile(160 - width, 290, width, 2, 0);
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.frame_ticks;
            width = dialogue_panel_scaled_quotient((timer * 5) << 4, 0x92492493, 2, 1);
            ui_draw_panel_tile(160, 290, width, 2, 0);
            goto frame_upper;
        }
    }
    ui_draw_panel_tile(80, 290, 80, 2, 0);
    ui_draw_panel_tile(160, 290, 80, 2, 0);
frame_upper:
{
}
    phase = mission_flow.mode_phase;
    if (phase - 5 >= 9)
    {
        owner = dialogue_ui.arena_begin;
        timer = dialogue_ui.frame_ticks;
        if ((sint32)timer < 23)
            goto frame_arc;
        if ((sint32)timer < 30)
        {
            width = dialogue_panel_scaled_quotient(((timer - 23) * 5) << 4, 0x92492493, 2, 1);
            ui_draw_panel_tile(80, 73, width, 2, 0);
            owner = dialogue_ui.arena_begin;
            timer = dialogue_ui.frame_ticks;
            width = dialogue_panel_scaled_quotient(((timer - 23) * 5) << 4, 0x92492493, 2, 1);
            ui_draw_panel_tile(240 - width, 73, width, 2, 0);
            goto frame_arc;
        }
    }
    ui_draw_panel_tile(80, 73, 80, 2, 0);
    ui_draw_panel_tile(160, 73, 80, 2, 0);
frame_arc:
{
}
    phase = mission_flow.mode_phase;
    count = 16;
    if (phase - 5 >= 9)
    {
        owner = dialogue_ui.arena_begin;
        timer = dialogue_ui.frame_ticks;
        if ((sint32)timer < 7)
            count = 0;
        else if ((sint32)timer < 23)
            count = timer - 7;
    }
    left = 80;
    right = 240;
    top = 290;
    outer_left = 80;
    outer_right = 240;
    outer_top = 291;
    for (index = 1; index <= count; ++index)
    {
        angle = 3072 - (index << 7);
        value = (uint32)rcos((sint32)angle) * 54;
        if ((sint32)value < 0)
            value += 4095;
        x = ((sint32)value >> 12);
        value = (uint32)rsin((sint32)angle) * 108;
        if ((sint32)value < 0)
            value += 4095;
        y = ((sint32)value >> 12);
        ui_draw_panel_line((uint32)(sint32)(sint16)left, (uint32)(sint32)(sint16)top, 80 + (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y);
        ui_draw_panel_line((uint32)(sint32)(sint16)right, (uint32)(sint32)(sint16)top, 240 - (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y);
        left = 80 + x;
        right = 240 - x;
        top = 182 - y;
        value = (uint32)rsin((sint32)angle) * 109;
        if ((sint32)value < 0)
            value += 4095;
        y_outer = ((sint32)value >> 12);
        ui_draw_panel_line((uint32)(sint32)(sint16)outer_left, (uint32)(sint32)(sint16)outer_top, 80 + (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y_outer);
        ui_draw_panel_line((uint32)(sint32)(sint16)outer_right, (uint32)(sint32)(sint16)outer_top, 240 - (uint32)(sint32)(sint16)x, 182 - (uint32)(sint32)(sint16)y_outer);
        outer_left = left;
        outer_right = right;
        outer_top = 182 - y_outer;
    }
    return 1;
}
