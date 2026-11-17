#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

MENU_BINDINGS menu_bindings;

void menu_bindings_reset(void)
{
    memset(&menu_bindings, 0, sizeof(menu_bindings));
}

MENU_BINDINGS_PROFILE *menu_bindings_profile(uint32 profile)
{
    if (profile >= 4)
        abort();
    return menu_bindings.profile + profile;
}

uint8 menu_bindings_axis_mode(uint32 profile)
{
    if (profile >= 6)
        abort();
    if (profile >= 4)
        return 0;
    return menu_bindings.profile[profile].axis_mode;
}

void menu_bindings_default_analog(void)
{
    uint8 *binding_table;

    FUNCTION_MARKER(0x800D180C, "ACE3.BIN");
    binding_table = menu_bindings_profile(0)->button;
    *binding_table = (4);
    binding_table[1] = (7);
    binding_table[3] = (11);
    binding_table[5] = (9);
    binding_table[6] = (20);
    binding_table[7] = (5);
    binding_table[8] = (6);
    binding_table[9] = (14);
    binding_table[11] = (12);
    binding_table[10] = (16);
    binding_table[12] = (13);
    binding_table[13] = (18);
    binding_table[2] = (8);
    binding_table[4] = (10);
    binding_table[14] = (19);
    binding_table = menu_bindings_profile(0)->axis;
    *binding_table = (0);
    binding_table[1] = (2);
    binding_table[2] = (8);
    binding_table[3] = (10);
    binding_table = menu_bindings_profile(0)->qualifier;
    *binding_table = (1);
    binding_table[1] = (0);
    binding_table[2] = (0);
    binding_table[3] = (0);
    menu_bindings.delay_option[1] = (uint8)(1);
    menu_bindings.delay_option[0] = (uint8)(1);
    return;
}

void menu_bindings_default_negcon(void)
{
    uint8 *binding_table;

    FUNCTION_MARKER(0x800D18E4, "ACE3.BIN");
    binding_table = menu_bindings_profile(1)->button;
    *binding_table = (20);
    binding_table[1] = (14);
    binding_table[3] = (5);
    binding_table[4] = (4);
    binding_table[8] = (13);
    binding_table[9] = (16);
    binding_table[10] = (7);
    binding_table[2] = (1);
    binding_table[5] = (0);
    binding_table[6] = (20);
    binding_table[7] = (20);
    binding_table[11] = (12);
    binding_table[12] = (6);
    binding_table[13] = (20);
    binding_table[14] = (20);
    binding_table = menu_bindings_profile(1)->axis;
    *binding_table = (12);
    binding_table[1] = (12);
    binding_table[2] = (12);
    binding_table[3] = (2);
    binding_table = menu_bindings_profile(1)->qualifier;
    *binding_table = (0);
    binding_table[1] = (0);
    binding_table[2] = (0);
    binding_table[3] = (0);
    menu_bindings.delay_option[1] = (uint8)(1);
    menu_bindings.delay_option[0] = (uint8)(1);
    return;
}

void menu_bindings_default_digital(void)
{
    uint8 *binding_table;

    FUNCTION_MARKER(0x800D19A8, "ACE3.BIN");
    binding_table = menu_bindings_profile(2)->button;
    *binding_table = (4);
    binding_table[1] = (7);
    binding_table[3] = (3);
    binding_table[4] = (2);
    binding_table[7] = (5);
    binding_table[8] = (6);
    binding_table[9] = (14);
    binding_table[11] = (12);
    binding_table[10] = (16);
    binding_table[12] = (13);
    binding_table[2] = (1);
    binding_table[5] = (0);
    binding_table[6] = (20);
    binding_table[13] = (20);
    binding_table[14] = (19);
    menu_bindings.delay_option[1] = (uint8)(1);
    menu_bindings.delay_option[0] = (uint8)(1);
    return;
}

void menu_bindings_default_joystick(void)
{
    uint8 *binding_table;

    FUNCTION_MARKER(0x800D1A3C, "ACE3.BIN");
    binding_table = menu_bindings_profile(3)->button;
    *binding_table = (20);
    binding_table[2] = (8);
    binding_table[3] = (11);
    binding_table[4] = (10);
    binding_table[5] = (9);
    binding_table[9] = (16);
    binding_table[11] = (14);
    binding_table[10] = (13);
    binding_table[12] = (12);
    binding_table[1] = (20);
    binding_table[6] = (20);
    binding_table[7] = (20);
    binding_table[8] = (20);
    binding_table[13] = (20);
    binding_table[14] = (19);
    binding_table = menu_bindings_profile(3)->axis;
    *binding_table = (6);
    binding_table[1] = (4);
    binding_table[2] = (0);
    binding_table[3] = (2);
    binding_table = menu_bindings_profile(3)->qualifier;
    *binding_table = (0);
    binding_table[1] = (0);
    binding_table[2] = (1);
    binding_table[3] = (0);
    menu_bindings.delay_option[1] = (uint8)(1);
    menu_bindings.delay_option[0] = (uint8)(1);
    return;
}

void menu_bindings_prepare_analog_labels(void)
{
    FUNCTION_MARKER(0x800D31A8, "ACE3.BIN");
}

void menu_bindings_prepare_negcon_labels(void)
{
    FUNCTION_MARKER(0x800D31B0, "ACE3.BIN");
    text_select_font(6);
    text_set_color(128, 128, 128);
}

void menu_bindings_prepare_digital_labels(void)
{
    FUNCTION_MARKER(0x800D31E0, "ACE3.BIN");
    text_select_font(6);
    text_set_color(128, 128, 128);
}

void menu_bindings_prepare_joystick_labels(void)
{
    FUNCTION_MARKER(0x800D3210, "ACE3.BIN");
    text_select_font(6);
    text_set_color(128, 128, 128);
}

void menu_bindings_prepare_labels(void)
{
    FUNCTION_MARKER(0x800D3240, "ACE3.BIN");
    if (!(sint16)menu_state.profile_kind)
        menu_bindings_prepare_analog_labels();
    if ((sint16)menu_state.profile_kind == 1)
        menu_bindings_prepare_negcon_labels();
    if ((sint16)menu_state.profile_kind == 2)
        menu_bindings_prepare_digital_labels();
    if ((sint16)menu_state.profile_kind == 3)
        menu_bindings_prepare_joystick_labels();
}

void menu_bindings_draw_buttons(void)
{
    FUNCTION_MARKER(0x800D4394, "ACE3.BIN");
    if (!(sint16)menu_state.profile_kind)
        menu_bindings_draw_analog_buttons_frame();
    if ((sint16)menu_state.profile_kind == 1)
        menu_bindings_draw_negcon_buttons();
    if ((sint16)menu_state.profile_kind == 2)
        menu_bindings_draw_digital_buttons();
    if ((sint16)menu_state.profile_kind == 3)
    {
        (void)(menu_bindings_draw_joystick_buttons());
        return;
    }
    return;
}

void menu_bindings_draw_axes(void)
{
    FUNCTION_MARKER(0x800D4CA4, "ACE3.BIN");
    if (!(sint16)menu_state.profile_kind)
        menu_bindings_draw_analog_axes_frame();
    if ((sint16)menu_state.profile_kind == 1)
        menu_bindings_draw_negcon_axes_frame();
    if ((sint16)menu_state.profile_kind == 3)
    {
        (void)(menu_bindings_draw_joystick_axes_frame());
        return;
    }
    return;
}

uint32 menu_bindings_label_variant(uint32 type)
{
    FUNCTION_MARKER(0x800D32FC, "ACE3.BIN");
    if (type == 19)
    {
        if (menu_bindings.delay_option[0] == 1)
            return 21;
    }
    if (type == 14)
    {
        if (menu_bindings.delay_option[1] == 1)
            return 22;
    }
    if (type == 15)
    {
        if (menu_bindings.delay_option[1] == 1)
            return 23;
    }
    return type;
}

void menu_bindings_draw_analog_buttons_frame(void)
{
    FUNCTION_MARKER(0x800D3388, "ACE3.BIN");
    {
        (void)(menu_bindings_draw_analog_buttons());
        return;
    }
}

void menu_bindings_draw_analog_buttons(void)
{
    uint8 *binding_row;
    uint8 *binding_buttons;
    uint8 *binding_entry;
    static const uint16 positions[15][2] = {{40, 149}, {40, 183}, {40, 216}, {40, 249}, {40, 282}, {40, 315}, {40, 348}, {252, 138}, {252, 169}, {252, 199}, {252, 229}, {252, 259}, {252, 289}, {252, 319}, {252, 349}};
    uint32 selected, index, value, mask;
    uint32 packet, result, publish;

    FUNCTION_MARKER(0x800D3390, "ACE3.BIN");
    binding_row = menu_bindings_profile(0)->button;
    publish = menu_state.binding.button_phase;
    binding_buttons = menu_bindings_profile(0)->axis;
    for (index = 10; index < 32; ++index)
    {
        *ui_widget_value(index) = (uint8)(0);
    }
    if (publish)
    {
        for (index = 0; index < 15; ++index)
        {
            value = binding_row[index];
            *ui_widget_value(value + 10) = (uint8)(1);
        }
        for (index = 0; index < 4; ++index)
        {
            binding_entry = binding_buttons + index;
            if (*binding_entry < 8)
            {
                value = *binding_entry;
                *ui_widget_value(value + 10) = (uint8)(1);
                value = *binding_entry;
                *ui_widget_value(value + 11) = (uint8)(1);
            }
        }
    }
    selected = *binding_row;
    value = menu_state.binding.button_cursor;
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 15; ++index)
    {
        if (index)
            selected = binding_row[index];
        value = menu_bindings_label_variant(selected);
        packet = menu_state.packet_cursor;
        result = ui_emit_controller_label(positions[index][0], positions[index][1], menu_binding_button_image(value), ((mask >> (index + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_bindings_draw_negcon_buttons(void)
{
    uint8 *binding_row;
    static const uint8 offsets[10] = {1, 2, 3, 4, 5, 8, 9, 10, 11, 12};
    static const uint16 positions[10][2] = {{40, 183}, {40, 216}, {40, 249}, {40, 282}, {40, 315}, {252, 183}, {252, 216}, {252, 249}, {252, 282}, {252, 315}};
    uint32 mask, selected, index, value, packet, result;

    FUNCTION_MARKER(0x800D38EC, "ACE3.BIN");
    binding_row = menu_bindings_profile(1)->button;
    value = menu_state.binding.button_cursor;
    selected = binding_row[1];
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 10; ++index)
    {
        if (index)
            selected = binding_row[offsets[index]];
        value = menu_bindings_label_variant(selected);
        packet = menu_state.packet_cursor;
        result = ui_emit_controller_label(positions[index][0], positions[index][1], menu_binding_button_image(value), ((mask >> (index + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_bindings_draw_digital_buttons(void)
{
    uint8 *binding_row;
    static const uint8 offsets[13] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14};
    static const uint16 positions[13][2] = {{40, 160}, {40, 193}, {40, 226}, {40, 259}, {40, 292}, {40, 325}, {252, 149}, {252, 180}, {252, 211}, {252, 242}, {252, 273}, {252, 304}, {252, 335}};
    uint32 mask, selected, index, value, packet, result;

    FUNCTION_MARKER(0x800D3BE0, "ACE3.BIN");
    binding_row = menu_bindings_profile(2)->button;
    selected = *binding_row;
    value = menu_state.binding.button_cursor;
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 13; ++index)
    {
        if (index)
            selected = binding_row[offsets[index]];
        value = menu_bindings_label_variant(selected);
        packet = menu_state.packet_cursor;
        result = ui_emit_controller_label(positions[index][0], positions[index][1], menu_binding_button_image(value), ((mask >> (index + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_bindings_draw_joystick_buttons(void)
{
    uint8 *binding_row;
    static const uint8 offsets[13] = {1, 8, 0, 7, 14, 2, 3, 4, 5, 11, 12, 10, 9};
    static const uint16 positions[13][2] = {{41, 150}, {41, 181}, {41, 238}, {41, 268}, {41, 340}, {144, 145}, {144, 177}, {144, 208}, {144, 241}, {252, 150}, {252, 181}, {252, 238}, {252, 268}};
    uint32 mask, selected, index, value, packet, result;

    FUNCTION_MARKER(0x800D3FA0, "ACE3.BIN");
    binding_row = menu_bindings_profile(3)->button;
    value = menu_state.binding.button_cursor;
    selected = binding_row[1];
    mask = (uint32)1 << (value & 31);
    for (index = 0; index < 13; ++index)
    {
        if (index)
            selected = binding_row[offsets[index]];
        value = menu_bindings_label_variant(selected);
        packet = menu_state.packet_cursor;
        result = ui_emit_controller_label(positions[index][0], positions[index][1], menu_binding_button_image(value), ((mask >> (index + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(result);
    }
    if (ui_animation_phase(6) == 2)
    {
        text_set_ot(ui_packets.scene_ot);
    }
    {
        (void)(text_set_ot(0xFFFFFFFF));
        return;
    }
}

void menu_bindings_draw_analog_axes_frame(void)
{
    FUNCTION_MARKER(0x800D4430, "ACE3.BIN");
    {
        (void)(menu_bindings_draw_analog_axes());
        return;
    }
}

void menu_bindings_draw_analog_axes(void)
{
    uint8 *binding_row;
    const MENU_BINDING_IMAGE *descriptor;
    static const sint16 x_offsets[8] = {45, 45, -1, 82, 235, 235, 188, 271};
    static const uint16 y_positions[8] = {155, 304, 231, 231, 155, 304, 231, 231};
    uint16 position[2];
    uint32 adjustments[4], index, type, mask, button, packet, x;

    FUNCTION_MARKER(0x800D4438, "ACE3.BIN");
    binding_row = menu_bindings_profile(0)->axis;
    for (index = 0; index < 4; ++index)
    {
        type = binding_row[index];
        if (!type || type == 6 || type == 8)
            adjustments[index] = menu_bindings_profile(0)->qualifier[index];
        else
            adjustments[index] = 0;
    }
    mask = (uint32)1 << (menu_state.binding.axis_cursor & 31);
    ui_item_get_pos(2, 9, position);
    packet = 0;
    for (index = 0; index < 8; ++index)
    {
        button = index >> 1;
        x = (sint16)(uint16)(position[0] + x_offsets[index]);
        type = binding_row[button];
        if (index & 1)
            type = type + 1 - adjustments[button];
        else
            type += adjustments[button];
        descriptor = menu_binding_axis_image(type);
        if (!index)
        {
            packet = menu_state.packet_cursor;
        }
        packet = ui_emit_controller_label(x, y_positions[index], descriptor, ((mask >> (button + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(packet);
    }
    {
        (void)(uint32)(packet);
        return;
    }
}

void menu_bindings_draw_negcon_axes_frame(void)
{
    FUNCTION_MARKER(0x800D47DC, "ACE3.BIN");
    {
        (void)(menu_bindings_draw_negcon_axes());
        return;
    }
}

void menu_bindings_draw_negcon_axes(void)
{
    uint8 *binding_row;
    const MENU_BINDING_IMAGE *descriptor;
    uint32 type, adjustment, mask, packet;

    FUNCTION_MARKER(0x800D47E4, "ACE3.BIN");
    binding_row = menu_bindings_profile(1)->axis;
    type = binding_row[3];
    adjustment = 0;
    if (!type || type == 6 || type == 8)
        adjustment = menu_bindings_profile(1)->qualifier[0];
    type = binding_row[3];
    descriptor = menu_binding_axis_image(type + adjustment);
    mask = (uint32)1 << (menu_state.binding.axis_cursor & 31);
    mask = ((mask >> 4) & 1) + 1;
    packet = menu_state.packet_cursor;
    packet = ui_emit_controller_label(40, 335, descriptor, mask, packet);
    menu_state.packet_cursor = (uint32)(packet);
    type = binding_row[3];
    descriptor = menu_binding_axis_image(type + 1 - adjustment);
    packet = ui_emit_controller_label(122, 335, descriptor, mask, packet);
    menu_state.packet_cursor = (uint32)(packet);
    {
        (void)(uint32)(packet);
        return;
    }
}

void menu_bindings_draw_joystick_axes_frame(void)
{
    FUNCTION_MARKER(0x800D48F8, "ACE3.BIN");
    {
        (void)(menu_bindings_draw_joystick_axes());
        return;
    }
}

void menu_bindings_draw_joystick_axes(void)
{
    uint8 *binding_row;
    const MENU_BINDING_IMAGE *descriptor;
    static const sint16 x_offsets[8] = {45, 45, -1, 82, 235, 235, 188, 271};
    static const uint16 y_positions[8] = {155, 304, 231, 231, 155, 304, 231, 231};
    uint16 position[2];
    uint32 adjustments[4], index, type, mask, button, packet, x;

    FUNCTION_MARKER(0x800D4900, "ACE3.BIN");
    binding_row = menu_bindings_profile(3)->axis;
    for (index = 0; index < 4; ++index)
    {
        type = binding_row[index];
        if (!type || type == 6 || type == 8)
            adjustments[index] = menu_bindings_profile(3)->qualifier[index];
        else
            adjustments[index] = 0;
    }
    mask = (uint32)1 << (menu_state.binding.axis_cursor & 31);
    ui_item_get_pos(2, 11, position);
    packet = 0;
    for (index = 0; index < 8; ++index)
    {
        button = index >> 1;
        x = (sint16)(uint16)(position[0] + x_offsets[index]);
        type = binding_row[button];
        if (index & 1)
            type = type + 1 - adjustments[button];
        else
            type += adjustments[button];
        descriptor = menu_binding_axis_image(type);
        if (!index)
        {
            packet = menu_state.packet_cursor;
        }
        packet = ui_emit_controller_label(x, y_positions[index], descriptor, ((mask >> (button + 1)) & 1) + 1, packet);
        menu_state.packet_cursor = (uint32)(packet);
    }
    {
        (void)(uint32)(packet);
        return;
    }
}

void menu_bindings_edit_begin(void)
{
    uint32 index, mask;
    FUNCTION_MARKER(0x800D4D20, "ACE3.BIN");
    menu_state.binding.axis_cursor = (uint8)(0);
    menu_state.binding_phase = (uint32)(1);
    menu_state.binding.button_cursor = (uint8)(0);
    for (index = 0; index < 15; ++index)
    {
        *ui_widget_value(index + 8) = (uint8)(0);
    }
    if ((sint16)menu_state.profile_kind == 0)
    {
        ui_item_reset_motion(2, 4);
        if (menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 5);
            mask = 48;
        }
        else
        {
            ui_item_reset_motion(2, 9);
            mask = 528;
        }
        ui_group_set_mask(2, mask);
    }
    if ((sint16)menu_state.profile_kind == 1)
    {
        ui_item_reset_motion(2, 4);
        if (menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 6);
            mask = 80;
        }
        else
        {
            ui_item_reset_motion(2, 10);
            mask = 1040;
        }
        ui_group_set_mask(2, mask);
    }
    if ((sint16)menu_state.profile_kind == 2)
    {
        ui_item_reset_motion(2, 4);
        ui_item_reset_motion(2, 7);
        ui_group_set_mask(2, 144);
        menu_state.binding_tab = (uint8)(1);
    }
    if ((sint16)menu_state.profile_kind == 3)
    {
        ui_item_reset_motion(2, 4);
        if (menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 8);
            mask = 272;
        }
        else
        {
            ui_item_reset_motion(2, 11);
            mask = 2064;
        }
        ui_group_set_mask(2, mask);
        ui_animation_set(4, 2, 10, 10);
        ui_animation_set(5, 2, 10, 10);
        ui_animation_set(6, 0, 0, 0);
        ui_animation_set(7, 0, 0, 0);
        ui_animation_set(2, 0, 0, 0);
    }
    {
        ui_animation_set(1, 0, 0, 0);
        return;
    }
}

void menu_bindings_options_frame(void)
{
    FUNCTION_MARKER(0x800D4F78, "ACE3.BIN");
    {
        (void)(menu_bindings_options_tick());
        return;
    }
}

void menu_bindings_options_tick(void)
{
    static const uint8 mode2_offsets[] = {12, 13, 14, 4, 5, 6, 7, 34, 9};
    static const uint8 mode45_offsets[] = {12, 13, 14, 2, 3, 4, 5, 6, 7, 34, 9};
    uint32 raw, value, index, result;
    sint32 mode, selected;
    FUNCTION_MARKER(0x800D4F80, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    if (mode == 2 && (uint32)(menu_state.mapping_selection - 2u) < 2)
        menu_state.mapping_selection = (uint16)(0);
    if ((game_input.pressed & 0x40) && !(game_input.held & 0x2F))
    {
        if ((sint16)menu_state.mapping_selection != 4)
        {
            sound_play(60);
            game_input.pressed = (uint32)(0);
            menu_state.binding.axis_cursor = (uint8)(0);
            menu_state.mapping_selection = (uint16)(4);
            menu_state.binding.button_cursor = (uint8)(0);
        }
    }
    if (mode < 4)
    {
        if (game_input.repeat & 2)
        {
            sound_play(61);
            selected = (sint16)menu_state.mapping_selection;
            raw = menu_state.mapping_selection;
            menu_state.mapping_selection = (uint16)(selected < 4 ? raw + 1 : 0);
            if (mode == 2)
            {
                selected = (sint16)menu_state.mapping_selection;
                raw = menu_state.mapping_selection;
                if (selected == 2)
                    menu_state.mapping_selection = (uint16)(raw + 2);
            }
        }
        if (game_input.repeat & 1)
        {
            sound_play(61);
            selected = (sint16)menu_state.mapping_selection;
            raw = menu_state.mapping_selection;
            menu_state.mapping_selection = (uint16)(selected > 0 ? raw - 1 : 4);
            if (mode == 2)
            {
                selected = (sint16)menu_state.mapping_selection;
                raw = menu_state.mapping_selection;
                if (selected == 3)
                    menu_state.mapping_selection = (uint16)(raw - 2);
            }
        }
    }
    else
        menu_state.mapping_selection = (uint16)(0);
    if (game_input.repeat & 8)
    {
        if ((sint16)menu_state.mapping_selection < 4)
            sound_play(61);
        if ((sint16)menu_state.mapping_selection == 0 && mode < 4)
            menu_bindings_edit_begin();
        if ((sint16)menu_state.mapping_selection == 1)
        {
            value = menu_bindings.vibration_choice;
            menu_bindings.vibration_choice = (uint8)(1u - value);
        }
        if ((sint16)menu_state.mapping_selection == 2)
        {
            value = menu_bindings_axis_mode((uint32)mode);
            menu_bindings_profile((uint32)mode)->axis_mode = (uint8)(value < 2 ? value + 1 : 0);
        }
        if ((sint16)menu_state.mapping_selection == 3)
        {
            value = menu_bindings.dead_zone_choice;
            menu_bindings.dead_zone_choice = (uint8)(value < 2 ? value + 1 : 0);
        }
    }
    if (game_input.repeat & 4)
    {
        if ((sint16)menu_state.mapping_selection < 4)
            sound_play(61);
        if ((sint16)menu_state.mapping_selection == 0 && mode < 4)
            menu_bindings_edit_begin();
        selected = (sint16)menu_state.mapping_selection;
        if (selected == 1)
        {
            value = menu_bindings.vibration_choice;
            menu_bindings.vibration_choice = (uint8)(1u - value);
            selected = (sint16)menu_state.mapping_selection;
        }
        if (selected == 2)
        {
            value = menu_bindings_axis_mode((uint32)mode);
            menu_bindings_profile((uint32)mode)->axis_mode = (uint8)(value ? value - 1 : 2);
        }
        if ((sint16)menu_state.mapping_selection == 3)
        {
            value = menu_bindings.dead_zone_choice;
            menu_bindings.dead_zone_choice = (uint8)(value ? value - 1 : 2);
        }
    }
    if ((game_input.pressed & 0x10) && !(game_input.held & 0x2F) && (sint16)menu_state.mapping_selection == 4)
    {
        pad_settings_write_profiles();
        sound_play(60);
        if (mode == 0 || mode == 2)
        {
            mission_menu_start_item_motion(0, 1);
            menu_state.header.kind = (uint8)(0);
        }
        if (mode == 1 || mode == 3)
        {
            ui_group_set_mask(2, 8);
            menu_state.header.kind = (uint8)(2);
        }
        menu_state.header.phase = (uint8)(0);
    }
    if ((game_input.pressed & 0x20) && !(game_input.held & 0x5F))
    {
        sound_play(59);
        mission_menu_start_item_motion(0, 1);
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
        game_input.pressed = (uint32)(0);
    }
    value = (uint8)menu_state.mapping_selection;
    *ui_widget_value(1) = (uint8)(value);
    value = menu_bindings.vibration_choice ? 1 : ((sint16)menu_state.mapping_selection == 1 ? 2 : 0);
    *ui_widget_value(2) = (uint8)(value);
    value = menu_bindings.vibration_choice == 1 ? ((sint16)menu_state.mapping_selection == 1 ? 2 : 0) : 1;
    *ui_widget_value(3) = (uint8)(value);
    value = menu_bindings_axis_mode((uint32)mode) ? 1 : ((sint16)menu_state.mapping_selection == 2 ? 2 : 0);
    *ui_widget_value(4) = (uint8)(value);
    value = menu_bindings_axis_mode((uint32)mode) == 1 ? ((sint16)menu_state.mapping_selection == 2 ? 2 : 0) : 1;
    *ui_widget_value(5) = (uint8)(value);
    value = menu_bindings_axis_mode((uint32)mode) == 2 ? ((sint16)menu_state.mapping_selection == 2 ? 2 : 0) : 1;
    *ui_widget_value(6) = (uint8)(value);
    value = menu_bindings.dead_zone_choice ? 1 : ((sint16)menu_state.mapping_selection == 3 ? 2 : 0);
    *ui_widget_value(7) = (uint8)(value);
    value = menu_bindings.dead_zone_choice == 1 ? ((sint16)menu_state.mapping_selection == 3 ? 2 : 0) : 1;
    *ui_widget_value(34) = (uint8)(value);
    value = menu_bindings.dead_zone_choice == 2 ? ((sint16)menu_state.mapping_selection == 3 ? 2 : 0) : 1;
    *ui_widget_value(9) = (uint8)(value);
    value = (sint16)menu_state.mapping_selection == 4 ? 2 : 0;
    *ui_widget_value(10) = (uint8)(value);
    if ((uint32)mode < 2 || mode == 3)
    {
        for (index = 12; index < 15; ++index)
            *ui_widget_value(index) = (uint8)(0);
    }
    if (mode == 2)
    {
        for (index = 0; index < sizeof(mode2_offsets); ++index)
            *ui_widget_value(mode2_offsets[index]) = (uint8)(index ? 1 : 0);
    }
    if ((uint32)(mode - 4) < 2)
    {
        for (index = 0; index < sizeof(mode45_offsets); ++index)
            *ui_widget_value(mode45_offsets[index]) = (uint8)(1);
    }
    if (mode < 4)
    {
        if ((sint16)menu_state.mapping_selection == 0)
            *ui_widget_value(20) = (uint8)(0);
        if ((sint16)menu_state.mapping_selection == 1)
        {
            value = menu_bindings.vibration_choice;
            *ui_widget_value(20) = (uint8)(value + 1);
        }
        if ((sint16)menu_state.mapping_selection == 2)
        {
            value = menu_bindings_axis_mode((uint32)mode);
            *ui_widget_value(20) = (uint8)(value + 3);
        }
        if ((sint16)menu_state.mapping_selection == 3)
        {
            value = menu_bindings.dead_zone_choice;
            *ui_widget_value(20) = (uint8)(value + 101);
        }
        if ((sint16)menu_state.mapping_selection != 4)
            return;
        result = mode == 1 ? 9 : 8;
    }
    else if (mode == 4)
    {
        result = 86;
    }
    else
    {
        if (mode != 5)
            return;
        result = 87;
    }
    *ui_widget_value(20) = (uint8)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_bindings_edit_tick(void)
{
    uint32 value, raw, flags, phase;
    uint32 entry, index, packet, ordering, result;
    MENU_POLYLINE3_DESCRIPTOR triangle = {0};
    MENU_POLYLINE4_DESCRIPTOR quad = {0};
    sint32 mode, selected, current_mode;
    FUNCTION_MARKER(0x800D7DB0, "ACE3.BIN");
    flags = game_input.pressed;
    mode = (sint16)menu_state.profile_kind;
    if ((flags & 0x40) && !(game_input.held & 0x2F) && ((sint16)menu_state.mapping_selection != 2 || menu_state.restore_choice == 1) && !(((uint32)menu_state.binding.axis_action | ((uint32)menu_state.binding.button_action << 8) | ((uint32)menu_state.binding.axis_phase << 16) | ((uint32)menu_state.binding.button_phase << 24)) & 0xFFFF0000))
    {
        sound_play(60);
        game_input.pressed = (uint32)(0);
        menu_state.binding.axis_cursor = (uint8)(0);
        menu_state.mapping_selection = (uint16)(2);
        menu_state.binding.button_cursor = (uint8)(0);
        menu_state.restore_choice = (uint8)(0);
        current_mode = (sint16)menu_state.profile_kind;
        if (current_mode == 0)
            ui_item_set_pos(2, 9, 0, 0);
        else if (current_mode == 3)
            ui_item_set_pos(2, 11, 0, 0);
        *ui_widget_value(8) = (uint8)(0);
    }
    if ((sint16)menu_state.mapping_selection >= 3)
    {
        if (!menu_state.binding_tab)
        {
            value = menu_state.binding.axis_phase;
            if (value == 0)
                menu_bindings_axis_cursor_tick();
            else if (value == 1)
                menu_bindings_axis_action_frame();
            else if (value == 2)
                menu_bindings_axis_qualifier_tick();
            goto cancel;
        }
        value = menu_state.binding.button_phase;
        if (value == 0)
            menu_bindings_button_cursor_tick();
        else if (value == 1)
            menu_bindings_button_action_tick();
        else if (value == 2)
            menu_bindings_button_option_frame();
        value = menu_state.binding.button_phase;
        if (value == 1)
            value = menu_state.binding.button_action + 48;
        else if (value == 0 || value == 2)
        {
            current_mode = (sint16)menu_state.profile_kind;
            index = menu_state.binding.button_cursor;
            entry = menu_binding_key((uint32)current_mode, (sint32)index - 1);
            value = menu_bindings_profile((uint32)current_mode)->button[entry] + 48;
        }
        else
            value = 48;
        if (menu_state.binding.button_phase == 2)
        {
            if (value == 62)
                value = (game_input.kind & 16) ? 93 : 88;
            if (value == 63)
                value = (game_input.kind & 16) ? 93 : 88;
            if (value == 67)
                value = (game_input.kind & 16) ? 94 : 89;
        }
        else
        {
            if (value == 62 && menu_bindings.delay_option[1] == 1)
                value = 91;
            if (value == 63 && menu_bindings.delay_option[1] == 1)
                value = 92;
            if (value == 67 && menu_bindings.delay_option[0] == 1)
                value = 90;
        }
        *ui_widget_value(7) = (uint8)(value);
        goto cancel;
    }
    if (game_input.repeat & 2)
    {
        sound_play(61);
        raw = menu_state.mapping_selection + 1;
        menu_state.mapping_selection = (uint16)(raw);
        if ((sint16)raw == 3)
        {
            if (menu_state.binding_tab)
                menu_state.binding.button_cursor = (uint8)(1);
            else
            {
                current_mode = (sint16)menu_state.profile_kind;
                menu_state.binding.axis_cursor = (uint8)(current_mode == 1 ? 4 : 1);
            }
        }
    }
    if (game_input.repeat & 1)
    {
        sound_play(61);
        selected = (sint16)menu_state.mapping_selection;
        raw = menu_state.mapping_selection;
        if (selected > 0)
            menu_state.mapping_selection = (uint16)(raw - 1);
        else
        {
            value = menu_state.binding_tab;
            menu_state.mapping_selection = (uint16)(3);
            if (!value)
                menu_state.binding.axis_cursor = (uint8)(4);
            else if (value == 1)
            {
                current_mode = (sint16)menu_state.profile_kind;
                value = menu_binding_count((uint32)current_mode);
                menu_state.binding.button_cursor = (uint8)(value);
                if ((sint16)menu_state.profile_kind == 3)
                {
                    ui_animation_set(5, 3, -1, -1);
                    ui_animation_set(7, 1, 0, 10);
                }
            }
        }
    }
    selected = (sint16)menu_state.mapping_selection;
    if (selected == 2 && menu_state.restore_choice == 1)
    {
        if (!(game_input.pressed & 16))
            goto cancel;
        flags = game_input.held;
        if (!(flags & 0x2F) && !(flags & 0x40))
        {
            sound_play(60);
            if ((sint16)menu_state.profile_kind == 0)
                menu_bindings_default_analog();
            if ((sint16)menu_state.profile_kind == 1)
                menu_bindings_default_negcon();
            if ((sint16)menu_state.profile_kind == 2)
                menu_bindings_default_digital();
            if ((sint16)menu_state.profile_kind == 3)
                menu_bindings_default_joystick();
            goto cancel;
        }
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F) && (sint16)menu_state.mapping_selection == 2)
    {
        sound_play(60);
        pad_settings_write_profiles();
        if (mode == 0 || mode == 2)
        {
            mission_menu_start_item_motion(0, 1);
            menu_state.header.kind = (uint8)(0);
        }
        if (mode == 1 || mode == 3)
        {
            ui_group_set_mask(2, 8);
            menu_state.header.kind = (uint8)(2);
        }
        menu_state.header.phase = (uint8)(0);
        menu_state.binding_phase = (uint32)(0);
    }
cancel:
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        sound_play(59);
        mission_menu_start_item_motion(0, 1);
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
        menu_state.binding_phase = (uint32)(0);
        game_input.pressed = (uint32)(0);
    }
    if (menu_state.binding_tab == 1)
    {
        current_mode = (sint16)menu_state.profile_kind;
        if (menu_binding_count((uint32)current_mode))
        {
            index = 0;
            do
            {
                current_mode = (sint16)menu_state.profile_kind;
                const MENU_BINDING_LABEL *label = menu_binding_label((uint32)current_mode, index);
                phase = label->phase;
                if (!phase || ui_animation_phase(phase) == 2)
                {
                    packet = menu_state.packet_cursor;
                    triangle.points[0].xy = label->packed_xy[0];
                    value = label->packed_xy[1];
                    triangle.points[1].xy = value;
                    ordering = ui_packets.scene_ot + 4;
                    triangle.points[2].xy = label->packed_xy[2];
                    result = ui_emit_polyline3_pair(&triangle, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
                    menu_state.packet_cursor = (uint32)(result);
                }
                current_mode = (sint16)menu_state.profile_kind;
                value = menu_binding_count((uint32)current_mode);
                ++index;
            } while (index < value);
        }
        if ((sint16)menu_state.profile_kind == 3)
        {
            for (index = 0; index < 3; ++index)
            {
                const MENU_BINDING_QUAD_LABEL *label = menu_binding_extra_label(index);
                phase = label->phase;
                if (!phase || ui_animation_phase(phase) == 2)
                {
                    packet = menu_state.packet_cursor;
                    quad.points[0].xy = label->packed_xy[0];
                    quad.points[1].xy = label->packed_xy[1];
                    value = label->packed_xy[2];
                    quad.points[2].xy = value;
                    ordering = ui_packets.scene_ot + 4;
                    quad.points[3].xy = label->packed_xy[3];
                    result = ui_emit_polyline4_pair(&quad, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
                    menu_state.packet_cursor = (uint32)(result);
                }
            }
        }
    }
    if (game_input.repeat & 8)
    {
        if ((sint16)menu_state.mapping_selection < 3 && ((uint32)menu_state.profile_kind | ((uint32)menu_state.mapping_selection << 16)) != 0x10002)
            sound_play(61);
        if ((sint16)menu_state.mapping_selection == 0)
        {
            menu_state.binding_phase = (uint32)(0);
            ui_item_reset_motion(2, 2);
            ui_item_reset_motion(2, 3);
            ui_group_set_mask(2, 12);
            *ui_widget_value(7) = (uint8)(0);
            *ui_widget_value(20) = (uint8)(0);
        }
        if ((sint16)menu_state.mapping_selection == 1)
            menu_bindings_toggle_tab();
        if ((sint16)menu_state.mapping_selection == 2)
        {
            value = menu_state.restore_choice;
            menu_state.restore_choice = (uint8)(1u - value);
        }
    }
    if (game_input.repeat & 4)
    {
        if ((sint16)menu_state.mapping_selection < 3 && ((uint32)menu_state.profile_kind | ((uint32)menu_state.mapping_selection << 16)) != 0x10002)
            sound_play(61);
        if ((sint16)menu_state.mapping_selection == 0)
        {
            menu_state.binding_phase = (uint32)(0);
            ui_item_reset_motion(2, 2);
            ui_item_reset_motion(2, 3);
            ui_group_set_mask(2, 12);
            *ui_widget_value(7) = (uint8)(0);
            *ui_widget_value(20) = (uint8)(0);
        }
        if ((sint16)menu_state.mapping_selection == 1)
            menu_bindings_toggle_tab();
        if ((sint16)menu_state.mapping_selection == 2)
        {
            value = menu_state.restore_choice;
            menu_state.restore_choice = (uint8)(1u - value);
        }
        else
            goto clear_restore;
    }
    if ((sint16)menu_state.mapping_selection == 2)
        goto publish;
clear_restore:
    menu_state.restore_choice = (uint8)(0);
publish:
{
}
    value = (uint8)menu_state.mapping_selection;
    *ui_widget_value(1) = (uint8)(value);
    value = (sint16)menu_state.profile_kind == 2;
    *ui_widget_value(59) = (uint8)(value);
    value = (sint16)menu_state.mapping_selection != 0;
    *ui_widget_value(2) = (uint8)(value);
    value = menu_state.binding_tab ? 1 : ((sint16)menu_state.mapping_selection == 1 ? 2 : 0);
    *ui_widget_value(3) = (uint8)(value);
    value = menu_state.binding_tab == 1 ? ((sint16)menu_state.mapping_selection == 1 ? 2 : 0) : 1;
    *ui_widget_value(4) = (uint8)(value);
    value = menu_state.restore_choice ? 1 : ((sint16)menu_state.mapping_selection == 2 ? 2 : 0);
    *ui_widget_value(5) = (uint8)(value);
    value = menu_state.restore_choice == 1 ? ((sint16)menu_state.mapping_selection == 2 ? 2 : 0) : 1;
    *ui_widget_value(6) = (uint8)(value);
    *ui_widget_value(21) = (uint8)(0);
    if ((sint16)menu_state.mapping_selection == 0)
        *ui_widget_value(21) = (uint8)(11);
    if ((sint16)menu_state.mapping_selection == 1)
    {
        value = menu_state.binding_tab ? 13 : 12;
        *ui_widget_value(21) = (uint8)(value);
    }
    if ((sint16)menu_state.mapping_selection == 2)
    {
        if (menu_state.restore_choice)
        {
            value = 16;
        }
        else
        {
            value = (game_input.kind & 16) ? 10 : 9;
        }
        *ui_widget_value(21) = (uint8)(value);
    }
    result = (sint16)menu_state.mapping_selection < 3;
    if (result)
    {
        *ui_widget_value(7) = (uint8)(0);
    }
    return;
}

void menu_bindings_toggle_tab(void)
{
    uint32 value, result;
    sint32 mode;
    FUNCTION_MARKER(0x800D5BD4, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    if (mode == 0)
    {
        if (!menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 5);
            ui_group_set_mask(2, 48);
        }
        if (menu_state.binding_tab == 1)
        {
            ui_item_reset_motion(2, 9);
            ui_group_set_mask(2, 528);
        }
    }
    else if (mode == 1)
    {
        if (!menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 6);
            ui_group_set_mask(2, 80);
        }
        if (menu_state.binding_tab == 1)
        {
            ui_item_reset_motion(2, 10);
            ui_group_set_mask(2, 1040);
        }
    }
    else if (mode == 2)
        menu_state.binding_tab = (uint8)(0);
    else if (mode == 3)
    {
        if (!menu_state.binding_tab)
        {
            ui_item_reset_motion(2, 8);
            ui_group_set_mask(2, 272);
        }
        if (menu_state.binding_tab == 1)
        {
            ui_item_reset_motion(2, 11);
            ui_group_set_mask(2, 2064);
        }
    }
    value = menu_state.binding_tab;
    result = 1u - value;
    menu_state.binding_tab = (uint8)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

void menu_bindings_button_option_frame(void)
{
    FUNCTION_MARKER(0x800D5D64, "ACE3.BIN");
    {
        (void)(menu_bindings_button_option_tick());
        return;
    }
}

void menu_bindings_button_option_tick(void)
{
    uint8 *binding_entry;
    uint32 value, option_index, flags, index, entry;
    sint32 mode;
    FUNCTION_MARKER(0x800D5D6C, "ACE3.BIN");
    value = menu_state.binding.button_action;
    if ((value >= 14 && value < 16) || value == 19)
    {
        option_index = value == 19 ? 0 : 1;
        if ((game_input.pressed & 16) && !(game_input.held & 0x6F))
        {
            menu_bindings.delay_option[option_index] = (uint8)(1);
            sound_play(60);
        }
        if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
        {
            sound_play(60);
            menu_bindings.delay_option[option_index] = (uint8)(0);
        }
    }
    flags = game_input.pressed;
    if (((flags & 16) && !(game_input.held & 0x6F)) || ((flags & 32) && !(game_input.held & 0x5F)))
    {
        menu_state.binding.button_phase = (uint8)(0);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        game_input.pressed = (uint32)(0);
    }
    for (index = 10; index < 31; ++index)
        *ui_widget_value(index) = (uint8)(0);
    *ui_widget_value(33) = (uint8)(0);
    *ui_widget_value(35) = (uint8)(0);
    for (index = 0; index < 15; ++index)
    {
        mode = (sint16)menu_state.profile_kind;
        entry = menu_binding_key((uint32)mode, (sint32)index);
        value = menu_bindings_profile((uint32)mode)->button[entry] + 10;
        if (value == 21)
            value = 33;
        if (value == 11)
            value = 35;
        *ui_widget_value(value) = (uint8)(1);
    }
    for (index = 0; index < 4; ++index)
    {
        mode = (sint16)menu_state.profile_kind;
        binding_entry = menu_bindings_profile((uint32)mode)->axis + (index);
        if (*binding_entry < 8)
        {
            value = *binding_entry + 10;
            if (value == 10)
            {
                *ui_widget_value(10) = (uint8)(1);
                *ui_widget_value(35) = (uint8)(1);
            }
            else
            {
                *ui_widget_value(value) = (uint8)(1);
                *ui_widget_value(value + 1) = (uint8)(1);
            }
        }
    }
    return;
}

void menu_bindings_button_action_tick(void)
{
    uint8 *binding_mappings;
    uint8 *binding_choices;
    uint8 *binding_entry;
    uint32 value, old, index, entry;
    sint32 mode;
    FUNCTION_MARKER(0x800D60FC, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    binding_mappings = menu_bindings_profile((uint32)mode)->button;
    binding_choices = menu_bindings_profile((uint32)mode)->axis;
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        sound_play(59);
        menu_state.binding.button_phase = (uint8)(0);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        game_input.pressed = (uint32)(0);
    }
    if (game_input.repeat & 2)
    {
        sound_play(61);
        value = menu_state.binding.button_action;
        menu_state.binding.button_action = (uint8)(value < 20 ? value + 1 : 0);
    }
    if (game_input.repeat & 1)
    {
        sound_play(61);
        value = menu_state.binding.button_action;
        menu_state.binding.button_action = (uint8)(value ? value - 1 : 20);
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x6F))
    {
        sound_play(60);
        for (index = 0; index < 15; ++index)
        {
            old = binding_mappings[index];
            value = menu_state.binding.button_action;
            if (old == value)
                binding_mappings[index] = (20);
        }
        for (index = 0; index < 4; ++index)
        {
            value = menu_state.binding.button_action;
            old = binding_choices[index];
            if (old == (value & 0xFE) && old < 8)
                binding_choices[index] = (12);
        }
        mode = (sint16)menu_state.profile_kind;
        index = menu_state.binding.button_cursor;
        entry = menu_binding_key((uint32)mode, (sint32)index - 1);
        value = menu_state.binding.button_action;
        binding_mappings[entry] = (value);
        value = menu_state.binding.button_action;
        if (value == 14 || value == 19 || value == 15)
            menu_state.binding.button_phase = (uint8)(2);
        else
        {
            menu_state.binding.button_phase = (uint8)(0);
            ui_animation_set(1, 3, -1, -1);
            ui_animation_set(2, 3, -1, -1);
        }
    }
    if (menu_state.binding.button_phase)
    {
        value = menu_state.binding.button_action;
        *ui_widget_value(9) = (uint8)(value + 1);
    }
    else
        *ui_widget_value(9) = (uint8)(0);
    value = menu_state.binding.button_action;
    *ui_widget_value(7) = (uint8)(value + 48);
    for (index = 10; index < 31; ++index)
        *ui_widget_value(index) = (uint8)(0);
    *ui_widget_value(33) = (uint8)(0);
    *ui_widget_value(35) = (uint8)(0);
    for (index = 0; index < 15; ++index)
    {
        mode = (sint16)menu_state.profile_kind;
        entry = menu_binding_key((uint32)mode, (sint32)index);
        value = menu_bindings_profile((uint32)mode)->button[entry] + 10;
        if (value == 21)
            value = 33;
        if (value == 11)
            value = 35;
        *ui_widget_value(value) = (uint8)(1);
    }
    for (index = 0; index < 4; ++index)
    {
        mode = (sint16)menu_state.profile_kind;
        binding_entry = menu_bindings_profile((uint32)mode)->axis + (index);
        if (*binding_entry < 8)
        {
            value = *binding_entry + 10;
            if (value == 10)
            {
                *ui_widget_value(10) = (uint8)(1);
                *ui_widget_value(35) = (uint8)(1);
            }
            else
            {
                *ui_widget_value(value) = (uint8)(1);
                *ui_widget_value(value + 1) = (uint8)(1);
            }
        }
    }
    return;
}

void menu_bindings_button_cursor_tick(void)
{
    static const uint8 down_choices[] = {2, 4, 11, 13};
    static const uint8 down_out[] = {4, 6, 5, 7};
    static const uint8 down_in[] = {6, 4, 7, 5};
    static const uint8 up_choices[] = {5, 3, 12};
    static const uint8 up_out[] = {4, 6, 7};
    static const uint8 up_in[] = {6, 4, 5};
    uint32 value, count, index, entry;
    sint32 mode;
    FUNCTION_MARKER(0x800D65BC, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    if (mode == 0 || mode == 1 || mode == 2)
    {
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            if (mode == 1)
            {
                ui_animation_set(1, 1, 0, 10);
                sound_play(60);
            }
            else
            {
                sound_play(60);
                ui_animation_set(1, 1, 0, 10);
            }
        }
    }
    else if (mode == 3)
    {
        if (game_input.repeat & 2)
        {
            sound_play(61);
            for (index = 0; index < sizeof(down_choices); ++index)
            {
                if (menu_state.binding.button_cursor == down_choices[index])
                {
                    ui_animation_set(down_out[index], 3, -1, -1);
                    ui_animation_set(down_in[index], 1, 0, 10);
                }
            }
        }
        if (game_input.repeat & 1)
        {
            sound_play(61);
            for (index = 0; index < sizeof(up_choices); ++index)
            {
                if (menu_state.binding.button_cursor == up_choices[index])
                {
                    ui_animation_set(up_out[index], 3, -1, -1);
                    ui_animation_set(up_in[index], 1, 0, 10);
                }
            }
        }
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        {
            sound_play(60);
            value = menu_state.binding.button_cursor;
            if ((uint32)(value - 6u) < 4)
                ui_animation_set(2, 1, 0, 20);
            else
            {
                ui_animation_set(1, 1, 0, 20);
                value = menu_state.binding.button_cursor;
                if ((uint32)(value - 6u) < 4)
                    ui_animation_set(2, 1, 0, 20);
            }
        }
    }
    if (game_input.repeat & 2)
    {
        mode = (sint16)menu_state.profile_kind;
        value = menu_state.binding.button_cursor;
        count = menu_binding_count((uint32)mode);
        if (value < count)
            menu_state.binding.button_cursor = (uint8)(value + 1);
        else
        {
            menu_state.binding.button_cursor = (uint8)(0);
            menu_state.mapping_selection = (uint16)(0);
        }
        sound_play(61);
    }
    if (game_input.repeat & 1)
    {
        sound_play(61);
        value = menu_state.binding.button_cursor;
        if (value >= 2)
            menu_state.binding.button_cursor = (uint8)(value - 1);
        else
        {
            menu_state.binding.button_cursor = (uint8)(0);
            menu_state.mapping_selection = (uint16)(2);
        }
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        sound_play(60);
        menu_state.binding.button_phase = (uint8)(1);
        mode = (sint16)menu_state.profile_kind;
        index = menu_state.binding.button_cursor;
        entry = menu_binding_key((uint32)mode, (sint32)index - 1);
        value = menu_bindings_profile((uint32)mode)->button[entry];
        menu_state.binding.button_action = (uint8)(value);
    }
    value = menu_state.binding.button_cursor;
    *ui_widget_value(8) = (uint8)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

void menu_bindings_axis_qualifier_tick(void)
{
    uint8 *binding_qualifier;
    uint32 selection, flags, accepted, value, index, entry, count;
    uint32 first_offset, second_offset;
    sint32 mode, remainder;
    FUNCTION_MARKER(0x800D6B3C, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    selection = menu_state.binding.axis_cursor;
    if (!selection || selection > 4)
        abort();
    flags = game_input.pressed;
    binding_qualifier = menu_bindings_profile((uint32)mode)->qualifier + (selection - 1);
    if ((flags & 32) && !(game_input.held & 0x5F))
    {
        menu_state.binding.axis_phase = (uint8)(1);
        *ui_widget_value(62) = (uint8)(0);
        *ui_widget_value(63) = (uint8)(0);
        sound_play(60);
        game_input.pressed = (uint32)(0);
        return;
    }
    if ((game_input.repeat & 3) && !((menu_state.binding.axis_cursor - 1u) & 1))
    {
        sound_play(61);
        value = *binding_qualifier;
        *binding_qualifier = (1u - value);
        game_input.repeat = (uint32)(0);
    }
    if (game_input.repeat & 12)
    {
        remainder = ((sint32)menu_state.binding.axis_cursor - 1) % 2;
        if (remainder == 1)
        {
            sound_play(61);
            value = *binding_qualifier;
            *binding_qualifier = (1u - value);
            game_input.repeat = (uint32)(0);
        }
    }
    flags = game_input.pressed;
    accepted = 0;
    if (flags & 16)
        accepted = !(game_input.held & 0x2F);
    flags = game_input.held;
    if (accepted & (((flags >> 6) ^ 1) & 1))
    {
        menu_state.binding.axis_phase = (uint8)(0);
        value = menu_state.binding.axis_cursor;
        if ((uint32)(value - 1u) < 2)
            ui_animation_set(1, 3, -1, -1);
        value = menu_state.binding.axis_cursor;
        if ((uint32)(value - 3u) < 2)
            ui_animation_set(2, 3, -1, -1);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        *ui_widget_value(62) = (uint8)(0);
        *ui_widget_value(63) = (uint8)(0);
        sound_play(60);
        return;
    }
    value = menu_state.binding.axis_action;
    *ui_widget_value(7) = (uint8)(value * 5 + 18);
    mode = (sint16)menu_state.profile_kind;
    value = *binding_qualifier;
    first_offset = value ? 63 : 62;
    second_offset = value ? 62 : 63;
    if (mode == 1 || mode == 2)
    {
        *ui_widget_value(first_offset) = (uint8)(15);
        value = menu_state.binding.axis_action;
        *ui_widget_value(second_offset) = (uint8)(value + 16);
    }
    else
    {
        remainder = (sint32)menu_state.binding.axis_cursor - 1;
        remainder %= 2;
        *ui_widget_value(first_offset) = (uint8)((uint32)(remainder * 7 + 1));
        remainder = ((sint32)menu_state.binding.axis_cursor - 1) % 2;
        value = menu_state.binding.axis_action;
        *ui_widget_value(second_offset) = (uint8)(value + (uint32)(remainder * 7 + 2));
    }
    for (index = 10; index < 18; ++index)
        *ui_widget_value(index) = (uint8)(0);
    mode = (sint16)menu_state.profile_kind;
    if (mode == 1)
    {
        value = (menu_bindings_profile(1)->axis[3] >> 1) + 10;
        if (value == 11)
            value = 17;
        *ui_widget_value(value) = (uint8)(1);
    }
    else
    {
        for (index = 0; index < 4; ++index)
        {
            mode = (sint16)menu_state.profile_kind;
            value = (menu_bindings_profile((uint32)mode)->axis[index] >> 1) + 10;
            if (value == 11)
                value = 17;
            *ui_widget_value(value) = (uint8)(1);
        }
    }
    mode = (sint16)menu_state.profile_kind;
    count = menu_binding_count((uint32)mode);
    if (count)
    {
        index = 0;
        do
        {
            mode = (sint16)menu_state.profile_kind;
            entry = menu_binding_key((uint32)mode, (sint32)index);
            value = menu_bindings_profile((uint32)mode)->button[entry];
            if (value < 8)
            {
                value = (value >> 1) + 10;
                if (value == 11)
                    value = 35;
                *ui_widget_value(value) = (uint8)(1);
            }
            mode = (sint16)menu_state.profile_kind;
            count = menu_binding_count((uint32)mode);
            ++index;
        } while (index < count);
    }
    return;
}

void menu_bindings_axis_action_frame(void)
{
    FUNCTION_MARKER(0x800D7194, "ACE3.BIN");
    {
        (void)(menu_bindings_axis_action_tick());
        return;
    }
}

void menu_bindings_axis_action_tick(void)
{
    uint8 *binding_choices;
    uint8 *binding_mappings;
    uint8 *binding_address;
    uint32 value, old, index, count, entry;
    sint32 mode;
    FUNCTION_MARKER(0x800D719C, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    binding_choices = menu_bindings_profile((uint32)mode)->axis;
    binding_mappings = menu_bindings_profile((uint32)mode)->button;
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        menu_state.binding.axis_phase = (uint8)(0);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        sound_play(59);
        game_input.pressed = (uint32)(0);
    }
    if (game_input.repeat & 2)
    {
        sound_play(61);
        value = menu_state.binding.axis_action;
        menu_state.binding.axis_action = (uint8)(value < 6 ? value + 1 : 0);
    }
    if (game_input.repeat & 1)
    {
        sound_play(61);
        value = menu_state.binding.axis_action;
        menu_state.binding.axis_action = (uint8)(value ? value - 1 : 6);
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x6F))
    {
        sound_play(60);
        for (index = 0; index < 4; ++index)
        {
            value = menu_state.binding.axis_action;
            old = binding_choices[index];
            if (old == value * 2)
                binding_choices[index] = (12);
        }
        mode = (sint16)menu_state.profile_kind;
        count = menu_binding_count((uint32)mode);
        if (count)
        {
            index = 0;
            do
            {
                value = menu_state.binding.axis_action * 2;
                if (value < 8)
                {
                    mode = (sint16)menu_state.profile_kind;
                    entry = menu_binding_key((uint32)mode, (sint32)index);
                    binding_address = binding_mappings + entry;
                    old = *binding_address;
                    if (old == value)
                        *binding_address = (20);
                    mode = (sint16)menu_state.profile_kind;
                    entry = menu_binding_key((uint32)mode, (sint32)index);
                    value = menu_state.binding.axis_action;
                    binding_address = binding_mappings + entry;
                    old = *binding_address;
                    if (old == value * 2 + 1)
                        *binding_address = (20);
                }
                mode = (sint16)menu_state.profile_kind;
                count = menu_binding_count((uint32)mode);
                ++index;
            } while (index < count);
        }
        index = menu_state.binding.axis_cursor;
        if (!index || index > 4)
            abort();
        value = menu_state.binding.axis_action;
        binding_choices[index - 1] = (value * 2);
        value = menu_state.binding.axis_action;
        if (value == 0 || value == 3 || value == 4)
            menu_state.binding.axis_phase = (uint8)(2);
        else
        {
            menu_state.binding.axis_phase = (uint8)(0);
            ui_animation_set(1, 3, -1, -1);
            ui_animation_set(2, 3, -1, -1);
        }
    }
    if (menu_state.binding.axis_phase)
    {
        value = menu_state.binding.axis_action;
        *ui_widget_value(9) = (uint8)(value + 1);
    }
    else
        *ui_widget_value(9) = (uint8)(0);
    value = menu_state.binding.axis_action;
    *ui_widget_value(7) = (uint8)(value == 6 ? 68 : value * 5 + 17);
    for (index = 10; index < 18; ++index)
        *ui_widget_value(index) = (uint8)(0);
    mode = (sint16)menu_state.profile_kind;
    if (mode == 1)
    {
        value = (menu_bindings_profile(1)->axis[3] >> 1) + 10;
        if (value == 11)
            value = 17;
        *ui_widget_value(value) = (uint8)(1);
    }
    else
    {
        for (index = 0; index < 4; ++index)
        {
            mode = (sint16)menu_state.profile_kind;
            value = (menu_bindings_profile((uint32)mode)->axis[index] >> 1) + 10;
            if (value == 11)
                value = 17;
            *ui_widget_value(value) = (uint8)(1);
        }
    }
    mode = (sint16)menu_state.profile_kind;
    count = menu_binding_count((uint32)mode);
    if (count)
    {
        index = 0;
        do
        {
            mode = (sint16)menu_state.profile_kind;
            entry = menu_binding_key((uint32)mode, (sint32)index);
            value = menu_bindings_profile((uint32)mode)->button[entry];
            if (value < 8)
            {
                value = (value >> 1) + 10;
                if (value == 11)
                    value = 17;
                *ui_widget_value(value) = (uint8)(1);
            }
            mode = (sint16)menu_state.profile_kind;
            count = menu_binding_count((uint32)mode);
            ++index;
        } while (index < count);
    }
    return;
}

void menu_bindings_axis_cursor_tick(void)
{
    uint32 value, item, entry, result, edit = 0;
    sint32 mode;
    FUNCTION_MARKER(0x800D7750, "ACE3.BIN");
    mode = (sint16)menu_state.profile_kind;
    if (mode == 0 || mode == 3)
    {
        item = mode == 0 ? 9 : 11;
        if (game_input.repeat & 2)
        {
            sound_play(61);
            value = menu_state.binding.axis_cursor;
            if (value < 4)
                menu_state.binding.axis_cursor = (uint8)(value + 1);
            else
            {
                menu_state.binding.axis_cursor = (uint8)(0);
                menu_state.mapping_selection = (uint16)(0);
            }
        }
        if (menu_state.binding.axis_cursor == 0)
            ui_item_set_pos(2, item, 0, 0);
        if (menu_state.binding.axis_cursor == 1)
            ui_item_set_pos(2, item, 40, 0);
        if (menu_state.binding.axis_cursor == 3)
            ui_item_set_pos(2, item, -40, 0);
        if (game_input.repeat & 1)
        {
            sound_play(61);
            value = menu_state.binding.axis_cursor;
            if (value >= 2)
                menu_state.binding.axis_cursor = (uint8)(value - 1);
            else
            {
                menu_state.binding.axis_cursor = (uint8)(0);
                menu_state.mapping_selection = (uint16)(2);
            }
        }
        if (menu_state.binding.axis_cursor == 0)
            ui_item_set_pos(2, item, 0, 0);
        if (menu_state.binding.axis_cursor == 2)
            ui_item_set_pos(2, item, 40, 0);
        if (menu_state.binding.axis_cursor == 4)
            ui_item_set_pos(2, item, -40, 0);
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F) && (menu_state.binding.axis_cursor - 1u) < 4)
        {
            sound_play(60);
            value = menu_state.binding.axis_cursor;
            if ((uint32)(value - 1u) < 2)
                ui_animation_set(1, 1, 0, 10);
            value = menu_state.binding.axis_cursor;
            if ((uint32)(value - 3u) < 2)
                ui_animation_set(2, 1, 0, 10);
            edit = 1;
        }
    }
    else if (mode == 1)
    {
        if (game_input.repeat & 2)
        {
            sound_play(61);
            menu_state.binding.axis_cursor = (uint8)(0);
            menu_state.mapping_selection = (uint16)(0);
        }
        if (game_input.repeat & 1)
        {
            sound_play(61);
            menu_state.binding.axis_cursor = (uint8)(0);
            menu_state.mapping_selection = (uint16)(2);
        }
        if ((game_input.pressed & 16) && !(game_input.held & 0x2F) && (menu_state.binding.axis_cursor - 1u) < 4)
        {
            sound_play(60);
            ui_animation_set(1, 1, 0, 10);
            edit = 1;
        }
    }
    if (edit)
    {
        menu_state.binding.axis_phase = (uint8)(1);
        mode = (sint16)menu_state.profile_kind;
        value = menu_state.binding.axis_cursor;
        entry = menu_bindings_profile((uint32)mode)->axis[value - 1];
        menu_state.binding.axis_action = (uint8)(entry >> 1);
    }
    mode = (sint16)menu_state.profile_kind;
    value = menu_state.binding.axis_cursor;
    if (value > 4)
        abort();
    if (value)
    {
        entry = menu_bindings_profile((uint32)mode)->axis[value - 1];
        value = entry == 12 ? 68 : (entry >> 1) * 5 + 17;
    }
    *ui_widget_value(7) = (uint8)(value);
    mode = (sint16)menu_state.profile_kind;
    if (mode == 1 && menu_state.binding.axis_cursor == 4)
    {
        *ui_widget_value(8) = (uint8)(1);
    }
    else
    {
        result = menu_state.binding.axis_cursor;
        *ui_widget_value(8) = (uint8)(result);
    }
    return;
}

void menu_bindings_screen_begin(void)
{
    uint32 value, index;

    FUNCTION_MARKER(0x800D3088, "ACE3.BIN");
    value = menu_state.header.phase + 1;
    menu_state.header.phase = (uint8)(value);
    menu_state.header.close_requested = (uint8)(0);
    ui_animation_set(1, 0, 0, 0);
    ui_animation_set(2, 0, 0, 0);
    ui_group_set_mask(2, 12);
    ui_group_set_mask(0, 1);
    ui_item_reset_motion(2, 2);
    ui_item_reset_motion(2, 3);
    scene_texture_load_group(mission_flow.assets.pad_settings_textures, mission_flow.assets.options_source);
    menu_state.mapping_selection = (uint16)(0);
    menu_bindings.vibration_choice = (uint8)(0);
    for (index = 0; index < 4; ++index)
    {
        menu_bindings_profile((uint32)index)->axis_mode = (uint8)(0);
    }
    menu_state.pad_pending = (uint32)(1);
    *ui_widget_value(11) = (uint8)(4);
    *ui_widget_value(10) = (uint8)(0);
    ui_widget_set(62, 0);
    pad_settings_read_profiles();
    return;
}

void menu_bindings_screen_frame(void)
{
    FUNCTION_MARKER(0x800D8CA8, "ACE3.BIN");
    {
        menu_bindings_screen_tick();
        return;
    }
}

void menu_bindings_screen_tick(void)
{
    uint32 phase, value, flags, result;
    sint32 input_state, selection;

    FUNCTION_MARKER(0x800D8CB0, "ACE3.BIN");
    if (game_input.status & 0xB)
    {
        menu_state.pad_pending = (uint32)(1);
        menu_state.binding_phase = (uint32)(0);
        ui_item_reset_motion(2, 2);
        ui_item_reset_motion(2, 3);
        ui_group_set_mask(2, 12);
    }
    if (!menu_state.binding_phase)
        menu_bindings_prepare_labels();
    phase = menu_state.binding_phase;
    if (phase == 1)
    {
        if (!menu_state.binding_tab)
            menu_bindings_draw_axes();
        if (menu_state.binding_tab == phase)
            menu_bindings_draw_buttons();
    }
    phase = menu_state.binding_phase;
    if (!phase)
        menu_bindings_options_frame();
    else if (phase == 1)
        menu_bindings_edit_tick();
    if (menu_state.pad_pending)
    {
        input_state = (sint16)game_input.status;
        flags = game_input.status;
        if (!input_state)
        {
            if (game_input.kind & 4)
                menu_state.profile_kind = (uint16)(0);
            if (game_input.kind & 2)
                menu_state.profile_kind = (uint16)(0);
            if (game_input.kind & 0x10)
                menu_state.profile_kind = (uint16)(1);
            if (game_input.kind & 1)
                menu_state.profile_kind = (uint16)(2);
            if (game_input.kind & 8)
                menu_state.profile_kind = (uint16)(3);
            if (game_input.kind & 0x20)
                menu_state.profile_kind = (uint16)(5);
            if ((sint16)menu_state.profile_kind < 4)
            {
                menu_state.pad_refresh = (uint32)(1);
                *ui_widget_value(11) = (uint8)(4);
                menu_state.pad_pending = (uint32)(0);
            }
            value = game_input.kind;
            menu_state.previous_pad_kind = (uint32)(value);
            if (!game_input.kind)
                menu_state.profile_kind = (uint16)(4);
            return;
        }
        if (flags & 8)
        {
            *ui_widget_value(11) = (uint8)(5);
        }
    }
    else
    {
        if ((sint16)game_input.status)
            menu_state.pad_pending = (uint32)(1);
        else
        {
            value = game_input.kind;
            flags = menu_state.previous_pad_kind;
            if (value != flags)
                menu_state.pad_pending = (uint32)(1);
        }
    }
    value = game_input.kind;
    menu_state.previous_pad_kind = (uint32)(value);
    if (!game_input.kind)
        menu_state.profile_kind = (uint16)(4);
    if (menu_state.pad_refresh)
    {
        menu_state.binding_phase = (uint32)(0);
        ui_item_reset_motion(2, 2);
        ui_item_reset_motion(2, 3);
        ui_group_set_mask(2, 12);
        selection = (sint16)menu_state.profile_kind;
        if ((uint32)selection < 4)
            scene_texture_load_image(mission_flow.assets.pad_profile_images[selection], mission_flow.assets.frontend_source, SCENE_TEXTURE_STANDARD);
        *ui_widget_value(11) = (uint8)(4);
        menu_state.binding.button_cursor = (uint8)(0);
        menu_state.pad_refresh = (uint32)(0);
        menu_state.binding.axis_cursor = (uint8)(0);
        menu_state.binding.axis_phase = (uint8)(0);
        menu_state.binding.button_phase = (uint8)(0);
        menu_state.binding.axis_action = (uint8)(0);
        menu_state.binding.button_action = (uint8)(0);
        menu_state.mapping_selection = (uint16)(0);
        pad_settings_read_profiles();
        *ui_widget_value(62) = (uint8)(0);
        *ui_widget_value(63) = (uint8)(0);
        ui_animation_set(1, 3, -1, -1);
        {
            ui_animation_set(2, 3, -1, -1);
            return;
        }
    }
    result = (uint8)menu_state.profile_kind;
    *ui_widget_value(11) = (uint8)(result);
    return;
}
