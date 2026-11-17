#include <stdlib.h>

#include "game_functions.h"

typedef char dialogue_caption_packet_layout_check[sizeof(SPRT) == 20 && sizeof(DR_MODE) == 12 && sizeof(DIALOGUE_CAPTION_PACKETS) == 488 && offsetof(DIALOGUE_CAPTION_PACKETS, modes) == 440 && 5812 + sizeof(DIALOGUE_CAPTION_PACKETS) <= 6304 ? 1 : -1];

static DIALOGUE_CAPTION_PACKETS caption_packets;

typedef struct
{
    sint16 resource;
    uint32 count;
    uint8 speakers[8][2];
} CAPTION_SCRIPT;

static const CAPTION_SCRIPT scripts[6] = {{365, 2, {{1, 0}, {2, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {368, 2, {{1, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {371, 2, {{1, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {376, 2, {{1, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {389, 2, {{1, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}, {400, 8, {{1, 2}, {3, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}};

static const CAPTION_SCRIPT *script_row(uint32 row)
{
    if (row >= 6)
        abort();
    return &scripts[row];
}

static uint32 speaker_value(uint32 row, sint32 pos, uint32 speaker)
{
    const CAPTION_SCRIPT *script = script_row(row);

    if (speaker >= 2 || pos < -1 || pos >= 8)
        abort();
    if (pos == -1)
        return (uint8)(script->count >> (16 + speaker * 8));
    return script->speakers[pos][speaker];
}

static uint32 find_script(uint32 resource)
{
    uint32 index;

    FUNCTION_MARKER(0x800EDC80, "ACE3.BIN");
    resource = (sint16)resource;
    for (index = 0; index < 6; ++index)
    {
        if ((uint32)(sint32)scripts[index].resource == resource)
            return index;
    }
    return 0;
}

void dialogue_caption_init(void)
{
    DIALOGUE_CAPTION_PACKETS *packets;
    DR_MODE *draw_mode;
    SPRT *spr;
    uint32 owner, id, resource, value, bank, base, packet, index;

    FUNCTION_MARKER(0x800EDDF4, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    dialogue_ui.caption.pos = (uint16)(0xFFFF);
    dialogue_ui.caption.speaker = (uint8)(0);
    ui_widget_set(1, 0);
    ui_widget_set(2, 0);
    ui_animation_set(3, 0, 0, 0);
    ui_animation_set(4, 0, 0, 0);
    owner = dialogue_ui.arena_begin;
    id = dialogue_sequence_id(0);
    resource = dialogue_sequence_entry(id)->resource;
    value = find_script(resource);
    owner = dialogue_ui.arena_begin;
    dialogue_ui.caption.script_idx = (uint8)(value);
    dialogue_ui.caption.packets = &caption_packets;
    if (!gpu_register_packet_range(&caption_packets, sizeof(caption_packets)))
        abort();
    packets = dialogue_ui.caption.packets;
    for (bank = 0; bank < 2; ++bank)
    {
        owner = dialogue_ui.arena_begin;
        base = owner + 5812 + bank * 220;
        draw_mode = &packets->modes[bank][0];
        SetDrawMode(draw_mode, 0, 0, 0, NULL);
        packet = base;
        for (index = 0; index < 11; ++index)
        {
            spr = &packets->lines[bank][index];
            setSprt(spr);
            spr->rgbc.r = 128;
            spr->rgbc.g = 128;
            spr->rgbc.b = 128;
            spr->p[0].xy.x = 112;
            spr->p[0].xy.y = (sint16)(uint16)(88 + index * 24);
            spr->w = 135;
            spr->h = 18;
            spr->p[0].uv.u = 0;
            spr->p[0].uv.v = (uint8)(index * 18);
            packet += 20;
        }
    }
    {
        (void)(uint32)(sound_play(62));
        return;
    }
}

void dialogue_caption_draw_overlay(void)
{
    DIALOGUE_CAPTION_PACKETS *packets;
    DR_MODE *draw_mode;
    SPRT *spr;
    uint32 *ot_entry;
    uint32 bank, owner, packet, page_packet, ordering, tag, link;
    uint32 id, kind, index, value, count, row, item, result;
    sint32 position, half, remainder;

    FUNCTION_MARKER(0x800EE0D8, "ACE3.BIN");
    bank = game_runtime.display_buffer;
    owner = dialogue_ui.arena_begin;
    packet = owner + 5812 + bank * 220;
    page_packet = owner + 6252 + bank * 24;
    ordering = ui_packets.scene_ot;
    packets = dialogue_ui.caption.packets;
    if (packets && bank < 2)
    {
        packet = gpu_packet_link((uintptr_t)&packets->lines[bank][0]);
        page_packet = gpu_packet_link((uintptr_t)&packets->modes[bank][0]);
    }
    draw_mode = packets && bank < 2 ? &packets->modes[bank][0] : (DR_MODE *)ui_packet_span(page_packet, sizeof(*draw_mode));
    ot_entry = (uint32 *)ui_scene_ot_span(ordering, sizeof(*ot_entry));
    tag = draw_mode->tag;
    link = *ot_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (page_packet & 0xFFFFFF);
    id = dialogue_sequence_id(0);
    kind = dialogue_sequence_entry(id)->image_kind;
    page_packet += 12;
    if (kind != 8)
    {
        for (index = 0; index < 11; ++index)
        {
            position = (sint16)dialogue_ui.caption.pos;
            remainder = position % 4;
            value = ((uint32)(remainder + 296) << 6) | 22;
            spr = packets && bank < 2 ? &packets->lines[bank][index] : (SPRT *)ui_packet_span(packet, sizeof(*spr));
            tag = spr->tag;
            spr->p[0].attr = (uint16)value;
            link = *ot_entry;
            spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            link = *ot_entry;
            *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
            packet += 20;
        }
        owner = dialogue_ui.arena_begin;
        position = (sint16)dialogue_ui.caption.pos;
        half = (sint16)(position / 2);
        remainder = half - 2 * (half / 2);
        value = ((((uint32)remainder << 6) + 576) & 1023) >> 6;
        draw_mode = packets && bank < 2 ? &packets->modes[bank][1] : (DR_MODE *)ui_packet_span(page_packet, sizeof(*draw_mode));
        SetDrawMode(draw_mode, 0, 0, value | 16, NULL);
        tag = draw_mode->tag;
        link = *ot_entry;
        draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (page_packet & 0xFFFFFF);
    }
    if (ui_animation_phase(5) == 0)
    {
        owner = dialogue_ui.arena_begin;
        row = dialogue_ui.caption.script_idx;
        count = script_row(row)->count;
        position = (sint16)dialogue_ui.caption.pos;
        if (position < (sint32)(count - 2))
        {
            dialogue_ui.caption.speaker = (uint8)(0);
            owner = dialogue_ui.arena_begin;
            value = dialogue_ui.caption.pos;
            dialogue_ui.caption.pos = (uint16)(value + 1);
            ui_animation_set(5, 1, 0, 10);
        }
        else
        {
            dialogue_ui.entry.complete = (uint32)(1);
        }
    }
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    value = speaker_value(row, (sint32)item, 0);
    ui_widget_set(1, value);
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    if (speaker_value(row, (sint32)item, 0) != 0 && ui_animation_phase(3) == 0)
    {
        if (dialogue_ui.entry.complete == 0)
            ui_animation_set(3, 1, 0, 10);
        owner = dialogue_ui.arena_begin;
        dialogue_ui.caption.speaker = (uint8)(1);
    }
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    value = speaker_value(row, (sint32)item, 1);
    ui_widget_set(2, value);
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    if (speaker_value(row, (sint32)item, 1) != 0 && ui_animation_phase(4) == 0)
    {
        if (dialogue_ui.entry.complete == 0)
            ui_animation_set(4, 1, 0, 10);
        owner = dialogue_ui.arena_begin;
        dialogue_ui.caption.speaker = (uint8)(2);
    }
    owner = dialogue_ui.arena_begin;
    value = dialogue_ui.caption.speaker;
    ui_widget_set(3, value);
    result = game_input.pressed & 16;
    if (result != 0)
    {
        if (ui_animation_phase(3) == 2)
            ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        if (ui_animation_phase(4) == 2)
            ui_animation_set(4, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        {
            ui_animation_set(5, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            return;
        }
    }
    return;
}

void dialogue_caption_draw(void)
{
    DIALOGUE_CAPTION_PACKETS *packets;
    DR_MODE *draw_mode;
    SPRT *spr;
    uint32 *ot_entry;
    uint32 bank, owner, packet, page_packet, ordering, tag, link;
    uint32 index, value, count, row, item, result;
    sint32 position, half, remainder;

    FUNCTION_MARKER(0x800EE570, "ACE3.BIN");
    owner = dialogue_ui.arena_begin;
    bank = game_runtime.display_buffer;
    packet = owner + 5812 + bank * 220;
    page_packet = owner + 6252 + bank * 24;
    ordering = ui_packets.scene_ot;
    packets = dialogue_ui.caption.packets;
    if (packets && bank < 2)
    {
        packet = gpu_packet_link((uintptr_t)&packets->lines[bank][0]);
        page_packet = gpu_packet_link((uintptr_t)&packets->modes[bank][0]);
    }
    draw_mode = packets && bank < 2 ? &packets->modes[bank][0] : (DR_MODE *)ui_packet_span(page_packet, sizeof(*draw_mode));
    ot_entry = (uint32 *)ui_scene_ot_span(ordering, sizeof(*ot_entry));
    tag = draw_mode->tag;
    link = *ot_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (page_packet & 0xFFFFFF);
    page_packet += 12;
    for (index = 0; index < 11; ++index)
    {
        position = (sint16)dialogue_ui.caption.pos;
        remainder = position % 4;
        value = ((uint32)(remainder + 296) << 6) | 22;
        spr = packets && bank < 2 ? &packets->lines[bank][index] : (SPRT *)ui_packet_span(packet, sizeof(*spr));
        tag = spr->tag;
        spr->p[0].attr = (uint16)value;
        link = *ot_entry;
        spr->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *ot_entry;
        *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
        packet += 20;
    }
    owner = dialogue_ui.arena_begin;
    position = (sint16)dialogue_ui.caption.pos;
    half = (sint16)(position / 2);
    remainder = half - 2 * (half / 2);
    value = ((((uint32)remainder << 6) + 576) & 1023) >> 6;
    draw_mode = packets && bank < 2 ? &packets->modes[bank][1] : (DR_MODE *)ui_packet_span(page_packet, sizeof(*draw_mode));
    SetDrawMode(draw_mode, 0, 0, value | 16, NULL);
    tag = draw_mode->tag;
    link = *ot_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (page_packet & 0xFFFFFF);
    if (ui_animation_phase(5) == 0)
    {
        owner = dialogue_ui.arena_begin;
        row = dialogue_ui.caption.script_idx;
        count = script_row(row)->count;
        position = (sint16)dialogue_ui.caption.pos;
        if (position < (sint32)(count - 1))
        {
            dialogue_ui.caption.speaker = (uint8)(0);
            owner = dialogue_ui.arena_begin;
            value = dialogue_ui.caption.pos;
            dialogue_ui.caption.pos = (uint16)(value + 1);
            ui_animation_set(5, 1, 0, 10);
        }
        else
        {
            dialogue_ui.entry.complete = (uint32)(1);
        }
    }
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    value = speaker_value(row, (sint32)item, 0);
    ui_widget_set(1, value);
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    if (speaker_value(row, (sint32)item, 0) != 0 && ui_animation_phase(3) == 0)
    {
        if (dialogue_ui.entry.complete == 0)
            ui_animation_set(3, 1, 0, 10);
        owner = dialogue_ui.arena_begin;
        dialogue_ui.caption.speaker = (uint8)(1);
    }
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    value = speaker_value(row, (sint32)item, 1);
    ui_widget_set(2, value);
    owner = dialogue_ui.arena_begin;
    item = (sint16)dialogue_ui.caption.pos;
    row = dialogue_ui.caption.script_idx;
    if (speaker_value(row, (sint32)item, 1) != 0 && ui_animation_phase(4) == 0)
    {
        if (dialogue_ui.entry.complete == 0)
            ui_animation_set(4, 1, 0, 10);
        owner = dialogue_ui.arena_begin;
        dialogue_ui.caption.speaker = (uint8)(2);
    }
    owner = dialogue_ui.arena_begin;
    value = dialogue_ui.caption.speaker;
    ui_widget_set(3, value);
    result = game_input.pressed & 16;
    if (result != 0)
    {
        if (ui_animation_phase(3) == 2)
            ui_animation_set(3, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        if (ui_animation_phase(4) == 2)
            ui_animation_set(4, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        {
            ui_animation_set(5, 3, 0xFFFFFFFF, 0xFFFFFFFF);
            return;
        }
    }
    return;
}
