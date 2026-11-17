#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

static const char *const sound_test_track_titles[] = {
    "Linkage", "Replay", "Transparent Blue", "Eye of The Storm", "Physical Layer", "The Protocol", "Montage", "Alert", "Quartz", "The Maneuver", "Mind Flow", "Frozen Soul", "Miles Above", "Void", "Black Lotus", "Superstition", "Shattered Stones", "Code of Honor", "Turbulence", "Analogue", "The Execution", "Freefall", "Floe", "Liquid Air", "Morceaux", "Lithium", "Shorelines", "Gordian Knot", "Kernel", "Amorphous", "Revelation", "Zero-Sum", "Armory",
};

static const sint16 sound_test_divisor_x[5] = {4, 4, 12, 12, 8};

static const sint16 sound_test_divisor_y[5] = {4, 4, 12, 12, 8};

static const sint16 sound_test_frequency[5] = {32, -48, 56, -24, 64};

typedef struct
{
    uint8 selection;
    uint8 saved_mono;
    uint8 saved_volume[2];
} MENU_AUDIO_EDIT;

static MENU_AUDIO_EDIT audio_edit;

static uint32 menu_voice_ids[MENU_VOICE_COUNT];

void menu_voices_reset(void)
{
    uint32 index;
    for (index = 0; index < MENU_VOICE_COUNT; ++index)
        menu_voice_ids[index] = 255;
}

uint32 *menu_voice_slot(uint32 index)
{
    if (index >= MENU_VOICE_COUNT)
        abort();
    return menu_voice_ids + index;
}

static void menu_audio_initialize(uint32 resource);
static uint32 menu_audio_update(void);

void menu_audio_reset(void)
{
    memset(&audio_edit, 0, sizeof(audio_edit));
    menu_voices_reset();
}

void menu_audio_begin(void)
{
    FUNCTION_MARKER(0x800DA4F4, "ACE3.BIN");
    menu_audio_initialize(mission_flow.assets.audio_script);
}

static void menu_audio_initialize(uint32 resource)
{
    uint32 value;
    FUNCTION_MARKER(0x800DA4FC, "ACE3.BIN");
    scene_texture_load_group(resource, mission_flow.assets.options_source);
    menu_state.header.phase = (uint8)(1);
    ui_item_reset_motion(2, 12);
    ui_group_set_mask(0, 1);
    ui_group_set_mask(2, 4096);
    audio_edit.selection = (uint8)(0);
    value = audio_settings.flags;
    audio_edit.saved_mono = (uint8)(value & 1);
    value = audio_settings.volume[0];
    audio_edit.saved_volume[0] = (uint8)(value);
    value = audio_settings.volume[1];
    audio_edit.saved_volume[1] = (uint8)(value);
    menu_state.header.action = (uint8)(0);
    value = menu_state.options_choice;
    ui_widget_set(5, value);
    ui_widget_set(6, 0);
    ui_animation_set(1, 1, 0, 30);
    mission_audio_bind_tracks(&audio_tracks_options);
}

uint32 menu_audio_frame(void)
{
    FUNCTION_MARKER(0x800DAA6C, "ACE3.BIN");
    return menu_audio_update();
}

static uint32 menu_audio_update(void)
{
    uint8 *flag_address, *level0, *level1, *address;
    uint32 enabled, value, scaled, packet, ordering, result;
    uint32 flags, selection, changed, action, index, product;
    sint32 mode = 1, high, quotient;
    MENU_FILL_DESCRIPTOR rectangle;
    FUNCTION_MARKER(0x800DAA74, "ACE3.BIN");
    flag_address = &audio_settings.flags;
    level0 = &audio_settings.volume[0];
    value = *flag_address;
    enabled = value & 1;
    level1 = &audio_settings.volume[1];
    if (!enabled)
    {
        resource_set_mono(0);
        resource_apply_mix();
        value = audio_edit.selection != 0;
    }
    else
        value = 2;
    ui_widget_set(6, value);
    if (enabled == 1)
    {
        resource_set_mono(1);
        resource_apply_mix();
        value = audio_edit.selection != 0;
    }
    else
        value = 2;
    ui_widget_set(7, value);
    ui_widget_set(50, 0);
    value = *level0;
    scaled = value * 221 / 255;
    rectangle.y = (uint16)(194);
    rectangle.blue = (uint8)(0);
    rectangle.green = (uint8)(0);
    rectangle.red = (uint8)(0);
    rectangle.x = (uint16)(scaled + 55);
    rectangle.width = (uint16)(2);
    rectangle.height = (uint16)(64);
    rectangle.semi = (uint8)(0);
    ordering = ui_packets.scene_ot + 8;
    packet = menu_state.packet_cursor;
    result = ui_emit_menu_fill(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    selection = audio_edit.selection;
    menu_state.packet_cursor = (uint32)(result);
    rectangle.x = (uint16)(56);
    rectangle.y = (uint16)(194);
    if (selection == 1)
    {
        rectangle.red = (uint8)(207);
        rectangle.green = (uint8)(139);
        rectangle.blue = (uint8)(23);
        ui_widget_set(50, 1);
    }
    else
    {
        rectangle.red = (uint8)(229);
        rectangle.green = (uint8)(229);
        rectangle.blue = (uint8)(213);
    }
    value = *level0;
    rectangle.height = (uint16)(64);
    rectangle.width = (uint16)(value * 221 / 255);
    ordering = ui_packets.scene_ot + 8;
    packet = menu_state.packet_cursor;
    result = ui_emit_menu_fill(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    menu_state.packet_cursor = (uint32)(result);
    value = *level1;
    scaled = value * 221 / 255;
    rectangle.blue = (uint8)(0);
    rectangle.green = (uint8)(0);
    rectangle.red = (uint8)(0);
    rectangle.y = (uint16)(268);
    rectangle.x = (uint16)(scaled + 55);
    rectangle.width = (uint16)(2);
    rectangle.height = (uint16)(64);
    ordering = ui_packets.scene_ot + 8;
    packet = menu_state.packet_cursor;
    result = ui_emit_menu_fill(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    selection = audio_edit.selection;
    menu_state.packet_cursor = (uint32)(result);
    rectangle.x = (uint16)(56);
    rectangle.y = (uint16)(268);
    if (selection == 2)
    {
        rectangle.red = (uint8)(207);
        rectangle.green = (uint8)(139);
        rectangle.blue = (uint8)(23);
        ui_widget_set(50, 2);
    }
    else
    {
        rectangle.red = (uint8)(229);
        rectangle.green = (uint8)(229);
        rectangle.blue = (uint8)(213);
    }
    value = *level1;
    rectangle.height = (uint16)(64);
    rectangle.width = (uint16)(value * 221 / 255);
    ordering = ui_packets.scene_ot + 8;
    packet = menu_state.packet_cursor;
    result = ui_emit_menu_fill(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    menu_state.packet_cursor = (uint32)(result);
    result = mission_audio_mean_magnitude();
    product = result * 15;
    high = (((sint64)(sint32)product * (sint32)0x80010003u) >> 32);
    rectangle.x = (uint16)(50);
    rectangle.y = (uint16)(189);
    rectangle.height = (uint16)(12);
    rectangle.red = (uint8)(207);
    quotient = ((sint32)((uint32)high + product) >> 14) - ((sint32)product >> 31);
    rectangle.width = (uint16)((uint32)quotient * 7);
    rectangle.green = (uint8)(139);
    rectangle.blue = (uint8)(23);
    ordering = ui_packets.scene_ot + 4;
    packet = menu_state.packet_cursor;
    result = ui_emit_menu_fill(&rectangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    menu_state.packet_cursor = (uint32)(result);
    if (game_input.repeat & 2)
    {
        value = audio_edit.selection;
        audio_edit.selection = (uint8)(value < 2 ? value + 1 : 0);
        sound_play(61);
    }
    if (game_input.repeat & 1)
    {
        value = audio_edit.selection;
        audio_edit.selection = (uint8)(value ? value - 1 : 2);
        sound_play(61);
    }
    ui_widget_set(5, 0);
    flags = game_input.held;
    if (flags & 1)
        ui_widget_set(5, 1);
    else if (flags & 2)
        ui_widget_set(5, 2);
    for (action = 0; action < 2; ++action)
    {
        if (game_input.repeat & (action ? 4 : 8))
        {
            selection = audio_edit.selection;
            if (selection == 0)
            {
                if (enabled)
                    *flag_address = (0);
                else
                {
                    value = *flag_address;
                    *flag_address = (value | 1);
                }
                sound_play(61);
            }
            else if (selection == 1 || selection == 2)
            {
                address = selection == 1 ? level0 : level1;
                value = *address;
                changed = 0;
                if (action == 0 && value != 255)
                {
                    *address = (value < 240 ? value + 16 : 255);
                    changed = 1;
                }
                else if (action == 1 && value != 0)
                {
                    *address = (value >= 17 ? value - 16 : 0);
                    changed = 1;
                }
                if (changed)
                    sound_play(46);
            }
        }
    }
    for (action = 0; action < 2; ++action)
    {
        if ((game_input.pressed & (action ? 32 : 16)) && !(game_input.held & (action ? 0x5F : 0x2F)))
        {
            if (!action)
            {
                value = audio_settings.flags;
                audio_edit.saved_mono = (uint8)(value & 1);
                value = audio_settings.volume[0];
                audio_edit.saved_volume[0] = (uint8)(value);
                value = audio_settings.volume[1];
                audio_edit.saved_volume[1] = (uint8)(value);
            }
            else
            {
                value = audio_edit.saved_mono;
                flags = audio_settings.flags;
                audio_settings.flags = (uint8)(value == 1 ? flags | 1 : flags & 0xFE);
                value = audio_edit.saved_volume[0];
                audio_settings.volume[0] = (uint8)(value);
                value = audio_edit.saved_volume[1];
                audio_settings.volume[1] = (uint8)(value);
            }
            ui_animation_set(1, 1, 0, 30);
            menu_state.header.phase = (uint8)(1);
            ui_group_set_mask(2, 4096);
            sound_play(action ? 59 : 60);
            value = menu_state.options_choice;
            ui_widget_set(5, value);
            ui_widget_set(6, 0);
            for (index = 0; index < 5; ++index)
            {
                result = sound_play(93 + index);
                *menu_voice_slot(index) = (uint32)(result);
            }
            mode = 3;
        }
    }
    return mission_audio_control_track(mode, 0);
}

uint32 menu_voices_restart(uint32 adjustment)
{
    uint32 index, voice, value, result;

    FUNCTION_MARKER(0x800F3218, "ACE3.BIN");
    for (index = 0; index < 5; ++index)
    {
        voice = *menu_voice_slot(index);
        sound_stop_voice(voice);
    }
    for (index = 0; index < 5; ++index)
    {
        voice = sound_play(93 + index);
        *menu_voice_slot(index) = (uint32)(voice);
    }
    result = 0;
    for (index = 0; index < 5; ++index)
    {
        value = sound_get_note(93 + index);
        value = (sint16)value;
        voice = *menu_voice_slot(index);
        result = sound_set_voice_note(voice, (adjustment + value) & 0xFFFF);
    }
    return result;
}

const uint8 *title_audio_capture_channel(uint32 channel)
{
    sint32 bank;

    FUNCTION_MARKER(0x800DA2A8, "ACE3.BIN");
    bank = game_spu_read_decoded_native(menu_audio_capture.bytes, sizeof(menu_audio_capture.bytes), 5);
    SpuIsTransferCompleted(1);
    if (channel == 0)
        return menu_audio_capture.bytes + (bank ? 0 : 200);
    if (channel == 1)
        return menu_audio_capture.bytes + (bank ? 1024 : 1224);
    abort();
}

sint32 title_audio_mean_magnitude(void)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    const uint8 *first, *second;
    uint32 first_total = 0, second_total = 0, index;
    sint32 first_sample, second_sample;

    FUNCTION_MARKER(0x800DA340, "ACE3.BIN");
    first = title_audio_capture_channel(0);
    second = title_audio_capture_channel(1);
    for (index = 0; index < 100; ++index)
    {
        first_sample = (sint16)(memcpy(&wire_halfword_0, first, sizeof(wire_halfword_0)), wire_halfword_0);
        first += 2;
        second_sample = (sint16)(memcpy(&wire_halfword_1, second, sizeof(wire_halfword_1)), wire_halfword_1);
        second += 2;
        if (first_sample < 0)
            first_sample = -first_sample;
        first_total += first_sample;
        if (second_sample < 0)
            second_sample = -second_sample;
        second_total += second_sample;
    }
    return (sint16)((first_total / 100 + second_total / 100) / 2);
}

AUDIO_PCM_CAPTURE menu_audio_capture;
static SOUND_TEST_PACKET_STORAGE sound_test_packets;

MISSION_SOUND_TEST mission_sound_test;

uint32 mission_audio_mean_magnitude(void)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    const uint8 *left, *right;
    uint32 index, left_sum = 0, right_sum = 0;
    sint32 first, second;
    FUNCTION_MARKER(0x800F77A4, "ACE3.BIN");
    left = mission_audio_capture_channel(0);
    right = mission_audio_capture_channel(1);
    for (index = 0; index < 100; ++index)
    {
        first = (sint16)(memcpy(&wire_halfword_0, left + index * 2, sizeof(wire_halfword_0)), wire_halfword_0);
        second = (sint16)(memcpy(&wire_halfword_1, right + index * 2, sizeof(wire_halfword_1)), wire_halfword_1);
        left_sum += (uint32)(first < 0 ? -first : first);
        right_sum += (uint32)(second < 0 ? -second : second);
    }
    return (sint16)((left_sum / 100 + right_sum / 100) / 2);
}

const uint8 *mission_audio_capture_channel(uint32 channel)
{
    uint32 bank;

    FUNCTION_MARKER(0x800F770C, "ACE3.BIN");
    bank = game_spu_read_decoded_native(mission_audio_player.capture.bytes, sizeof(mission_audio_player.capture.bytes), 5);
    SpuIsTransferCompleted(1);
    if (channel == 0)
        return mission_audio_player.capture.bytes + (bank ? 0 : 200);
    if (channel == 1)
        return mission_audio_player.capture.bytes + (bank ? 1024 : 1224);
    abort();
}

static uint8 *mission_sound_test_packet_span(const MISSION_SOUND_TEST *state, uint32 address, size_t bytes)
{
    if (state->packet_data && address >= state->packet_link && address - state->packet_link <= sizeof(*state->packet_data) && bytes <= sizeof(*state->packet_data) - (address - state->packet_link))
        return (uint8 *)state->packet_data + (address - state->packet_link);
    abort();
}

uint32 mission_sound_test_begin(void)
{
    MISSION_SOUND_TEST *state = &mission_sound_test;
    uint32 owner, bank, column, row, first, second, x, y, index, value;
    DR_MODE *command;
    SPRT *sprite;
    FUNCTION_MARKER(0x800DCE0C, "ACE3.BIN");
    (void)arena_alloc(9148);
    if (!gpu_register_packet_range(&sound_test_packets, sizeof(sound_test_packets)))
        abort();
    state->packet_link = gpu_packet_link((uintptr_t)&sound_test_packets);
    state->option = (uint8)(2);
    state->orbit_tick = (uint16)(0);
    state->random_track = (uint8)(0);
    mission_audio_bind_tracks(&audio_tracks_sound_test);
    state->packet_data = &sound_test_packets;
    for (bank = 0; bank < 2; ++bank)
    {
        owner = state->packet_link;
        first = owner + bank * 2160;
        second = owner + bank * 2160 + 4320;
        for (column = 0; column < 18; ++column)
        {
            x = 156 + column * 8;
            for (row = 0; row < 6; ++row)
            {
                y = 350 - row * 16;
                sprite = &state->packet_data->gpu.levels[bank * 108 + column * 6 + row];
                setlen(sprite, 4);
                sprite->rgbc.code = (uint8)(102);
                sprite->rgbc.r = (uint8)(128);
                sprite->rgbc.g = (uint8)(128);
                sprite->rgbc.b = (uint8)(128);
                sprite->p[0].xy.x = (sint16)(x);
                sprite->p[0].xy.y = (sint16)(y);
                sprite->w = (sint16)(8);
                sprite->h = (sint16)(16);
                sprite->p[0].uv.u = (uint8)(row < 3 ? 220 : 228);
                sprite->p[0].uv.v = (uint8)(row < 3 ? 36 : 20);
                sprite->p[0].attr = (uint16)(row < 3 ? 3803 : 3867);
                first += 20;
                sprite = &state->packet_data->gpu.levels[216 + bank * 108 + column * 6 + row];
                sprite->p[0].uv.u = (uint8)(236);
                sprite->p[0].uv.v = (uint8)(24);
                setlen(sprite, 4);
                sprite->rgbc.code = (uint8)(102);
                sprite->rgbc.r = (uint8)(128);
                sprite->rgbc.g = (uint8)(128);
                sprite->rgbc.b = (uint8)(128);
                sprite->p[0].xy.x = (sint16)(x);
                sprite->p[0].xy.y = (sint16)(y);
                sprite->w = (sint16)(8);
                sprite->h = (sint16)(16);
                sprite->p[0].attr = (uint16)(3739);
                second += 20;
            }
        }
        command = &state->packet_data->gpu.level_modes[bank];
        SetDrawMode(command, 0, 0, 29, NULL);
        command = &state->packet_data->gpu.level_modes[2 + bank];
        SetDrawMode(command, 0, 0, 93, NULL);
    }
    for (bank = 0; bank < 2; ++bank)
    {
        owner = state->packet_link;
        first = owner + 8688 + bank * 100;
        for (index = 0; index < 5; ++index)
        {
            sprite = &state->packet_data->gpu.orbit[bank * 5 + index];
            setlen(sprite, 4);
            sprite->rgbc.code = (uint8)(102);
            sprite->rgbc.r = (uint8)(128);
            sprite->rgbc.g = (uint8)(128);
            sprite->rgbc.b = (uint8)(128);
            sprite->w = (sint16)(76);
            sprite->h = (sint16)(84);
            sprite->p[0].uv.u = (uint8)(0);
            sprite->p[0].uv.v = (uint8)(0);
            sprite->p[0].attr = (uint16)(3419);
            first += 20;
        }
        command = &state->packet_data->gpu.orbit_modes[bank];
        SetDrawMode(command, 0, 0, 93, NULL);
    }

    ui_item_reset_motion(2, 19);
    ui_group_set_mask(2, 0x80000);
    ui_item_reset_motion(0, 14);
    ui_group_set_mask(0, 0x4001);
    scene_texture_load_group(mission_flow.assets.sound_test_assets, mission_flow.assets.options_source);
    for (index = 0; index < 5; ++index)
        sound_stop_voice(*menu_voice_slot(index));
    value = menu_state.header.phase + 1;
    menu_state.header.phase = (uint8)(value);
    return value;
}

uint32 mission_sound_test_tick(void)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    MISSION_SOUND_TEST *state = &mission_sound_test;
    uint32 levels[19] = {0};
    const uint8 *left, *right;
    uint32 index, group = 0, column, row, first, second, owner;
    uint32 value, packet, ordering, tag, link, frame, mode = 0, result;
    uint8 arguments[12];
    uint32 *packet_tag, *ot_entry;
    SPRT *sprite;
    sint32 sample, divisor_x, divisor_y, frequency, numerator, quotient, elapsed, time;
    FUNCTION_MARKER(0x800DD1D0, "ACE3.BIN");
    left = mission_audio_capture_channel(0);
    right = mission_audio_capture_channel(1);
    frame = game_runtime.display_buffer;
    owner = state->packet_link;
    first = owner + frame * 2160;
    second = owner + frame * 2160 + 4320;
    for (index = 0; index < 100; ++index)
    {
        sample = (sint16)(memcpy(&wire_halfword_0, left + index * 2, sizeof(wire_halfword_0)), wire_halfword_0);
        if (sample < 0)
            sample = -sample;
        levels[group] += (uint32)sample;
        sample = (sint16)(memcpy(&wire_halfword_1, right + index * 2, sizeof(wire_halfword_1)), wire_halfword_1);
        if (sample < 0)
            sample = -sample;
        levels[group] += (uint32)sample;
        if (index % 5 == 4)
        {
            levels[group] = ((sint32)levels[group] / 10);
            value = levels[group] * 7;
            levels[group] = ((sint32)value / 32768);
            if (index / 5 < 18)
                ++group;
        }
    }
    for (column = 0; column < 18; ++column)
    {
        for (row = 0; (sint32)row < (sint32)levels[column]; ++row)
        {
            packet = first + (column * 6 + row) * 20;
            ordering = ui_packets.scene_ot;
            packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
            ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
            tag = *packet_tag;
            link = *ot_entry;
            *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            ordering = ui_packets.scene_ot;
            ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
            tag = *ot_entry;
            *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
        }
    }
    frame = game_runtime.display_buffer;
    owner = state->packet_link;
    packet = owner + frame * 12 + 8640;
    ordering = ui_packets.scene_ot;
    packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *packet_tag;
    link = *ot_entry;
    *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    frame = game_runtime.display_buffer;
    ordering = ui_packets.scene_ot;
    packet = owner + frame * 12 + 8640;
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    for (column = 0; column < 18; ++column)
    {
        for (row = 0; (sint32)row < (sint32)levels[column]; ++row)
        {
            packet = second + (column * 6 + row) * 20;
            ordering = ui_packets.scene_ot;
            packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
            ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
            tag = *packet_tag;
            link = *ot_entry;
            *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            ordering = ui_packets.scene_ot;
            ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
            tag = *ot_entry;
            *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
        }
    }
    frame = game_runtime.display_buffer;
    owner = state->packet_link;
    packet = owner + frame * 12 + 8664;
    ordering = ui_packets.scene_ot;
    packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *packet_tag;
    link = *ot_entry;
    *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    frame = game_runtime.display_buffer;
    ordering = ui_packets.scene_ot;
    packet = owner + frame * 12 + 8664;
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    frame = game_runtime.display_buffer;
    packet = owner + frame * 100 + 8688;
    mission_audio_mean_magnitude();
    for (index = 0; index < 5; ++index)
    {
        value = ((sint16)state->orbit_tick);
        frequency = sound_test_frequency[index];
        divisor_x = sound_test_divisor_x[index];
        divisor_y = sound_test_divisor_y[index];
        numerator = ((uint32)rcos((sint32)(value * (uint32)frequency)) * 76);
        if (!divisor_x || (numerator == (sint32)0x80000000u && divisor_x == -1))
            abort();
        quotient = numerator / divisor_x;

        sprite = (SPRT *)mission_sound_test_packet_span(state, packet, sizeof(*sprite));
        sprite->p[0].xy.x = (sint16)(quotient / 4096 + 37);
        value = ((sint16)state->orbit_tick);
        numerator = ((uint32)rsin((sint32)(value * (uint32)frequency)) * 84);
        if (!divisor_y || (numerator == (sint32)0x80000000u && divisor_y == -1))
            abort();
        quotient = numerator / divisor_y;
        sprite->p[0].xy.y = (sint16)(quotient / 4096 + 302);
        ordering = ui_packets.scene_ot;
        packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
        ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
        tag = *packet_tag;
        link = *ot_entry;
        *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ordering = ui_packets.scene_ot;
        ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
        tag = *ot_entry;
        *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
        packet += 20;
    }
    frame = game_runtime.display_buffer;
    owner = state->packet_link;
    packet = owner + frame * 12 + 9088;
    ordering = ui_packets.scene_ot;
    packet_tag = (uint32 *)mission_sound_test_packet_span(state, packet, sizeof(*packet_tag));
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *packet_tag;
    link = *ot_entry;
    *packet_tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    frame = game_runtime.display_buffer;
    ordering = ui_packets.scene_ot;
    packet = owner + frame * 12 + 9088;
    ot_entry = ui_scene_ot_span(ordering + 4, sizeof(*ot_entry));
    tag = *ot_entry;
    *ot_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    if (game_input.repeat & 8)
    {
        value = state->option;
        if (value == 1)
            state->option = (uint8)(0);
        else if (value == 7)
            state->option = (uint8)(2);
        else
        {
            value = state->option;
            state->option = (uint8)(value + 1);
        }
    }
    if (game_input.repeat & 4)
    {
        value = state->option;
        if (value == 0)
            value = 1;
        else if (value == 2)
            value = 7;
        else
        {
            value = state->option - 1;
        }
        state->option = (uint8)(value);
    }
    if (game_input.repeat & 3)
    {
        value = state->option;
        switch (value)
        {
            case 0:
                state->option = (uint8)(2);
                break;
            case 1:
                state->option = (uint8)(4);
                break;
            case 2:
            case 3:
                state->option = (uint8)(0);
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                state->option = (uint8)(1);
                break;
        }
    }
    if ((game_input.repeat & 16) && !(game_input.held & 32))
    {
        value = state->option;
        switch (value)
        {
            case 0:
            case 1:
                if (!state->random_track)
                {
                    frame = mission_audio_player.requested_idx;
                    mission_audio_player.requested_idx = value ? frame + 1 : frame - 1;
                }
                if (state->random_track == 1)
                {
                    frame = game_runtime.field_counter;
                    mission_audio_player.requested_idx = (uint32)((sint32)frame % 32);
                }
                break;
            case 2:
                state->random_track = (uint8)(0);
                break;
            case 3:
                state->random_track = (uint8)(1);
                break;
            case 4:
                mode = 1;
                break;
            case 5:
                mode = 3;
                break;
            case 6:
                mode = 2;
                break;
            case 7:
                mission_menu_start_item_motion(0, 1);
                menu_state.header.kind = (uint8)(0);
                menu_state.header.phase = (uint8)(0);
                resource_init_queue(0, 0);
                value = sound_play(93);
                for (index = 0; index < 4; ++index)
                {
                    *menu_voice_slot(index) = (uint32)(value);
                    value = sound_play(94 + index);
                }
                *menu_voice_slot(4) = (uint32)(value);
                break;
        }
    }

    value = state->option;
    *ui_widget_value(1) = (uint8)(value);
    if (!resource_wait(1))
    {
        if (mission_audio_player.flags & 1)
        {
            if (!state->random_track)
            {
                value = mission_audio_player.requested_idx;
                mission_audio_player.requested_idx = value + 1;
            }
            if (state->random_track == 1)
            {
                frame = game_runtime.field_counter;
                mission_audio_player.requested_idx = (uint32)((sint32)frame % 32);
            }
        }
    }
    time = (sint32)mission_audio_player.requested_idx;
    if (time < 0)
    {
        mission_audio_player.requested_idx = 32;
        time = (sint32)mission_audio_player.requested_idx;
    }
    if (time >= 33)
        mission_audio_player.requested_idx = 0;
    value = resource_get_stream_flags();
    result = mission_audio_player.flags;
    mission_audio_player.flags = value & 0x40000000 ? result | 2 : result & 0xFFFFFFFD;
    mission_audio_control_track((sint32)mode, mission_audio_player.requested_idx);
    elapsed = resource_get_stream_elapsed();
    text_select_font(3);
    text_set_blend(2);
    text_set_color(128, 128, 128);
    text_set_pos(102, 144);
    value = mission_audio_player.requested_idx;
    (wire_word_2 = (value), (void)memcpy(arguments, &wire_word_2, sizeof(wire_word_2)));
    text_print_digits((const uint8 *)"%2d", arguments, 4);
    text_set_pos(124, 144);
    value = mission_audio_player.requested_idx;
    if (value >= sizeof(sound_test_track_titles) / sizeof(*sound_test_track_titles))
        abort();
    text_print((const uint8 *)sound_test_track_titles[value], NULL, 0);
    text_select_font(4);
    text_set_blend(2);
    text_set_pos(200, 394);
    value = resource_get_stream_flags();
    if (!(value & 0x40000000) || ((sint32)game_runtime.field_counter % 60) / 30 != 0)
    {
        (wire_word_3 = ((elapsed / 3600)), (void)memcpy(arguments, &wire_word_3, sizeof(wire_word_3)));
        (wire_word_4 = ((elapsed / 60 - 60 * (elapsed / 3600))), (void)memcpy(arguments + 4, &wire_word_4, sizeof(wire_word_4)));
        (wire_word_5 = ((100 * (elapsed % 60) / 60)), (void)memcpy(arguments + 8, &wire_word_5, sizeof(wire_word_5)));
        text_print_digits((const uint8 *)"%2d:%2d.%2d", arguments, 12);
    }
    value = resource_get_stream_flags();
    if (!(value & 0x02000000) || (resource_get_stream_flags() & 0x40000000))
    {
        value = state->orbit_tick;
        state->orbit_tick = (uint16)(value + 1);
    }
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            mission_menu_start_item_motion(0, 1);
            menu_state.header.kind = (uint8)(0);
            menu_state.header.phase = (uint8)(0);
            resource_init_queue(0, 0);
            value = sound_play(93);
            *menu_voice_slot(0) = (uint32)(value);
            value = sound_play(94);
            *menu_voice_slot(1) = (uint32)(value);
            result = sound_play(95);
            *menu_voice_slot(2) = (uint32)(result);
            game_input.pressed = (uint32)(0);
        }
    }
    return result;
}
