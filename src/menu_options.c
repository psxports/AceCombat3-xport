#include <string.h>

#include "game_functions.h"

static MENU_OPTIONS options_state;
static uint32 options_phase;

void menu_options_reset(void)
{
    memset(&options_state, 0, sizeof(options_state));
    options_phase = 0;
}

void menu_options_start(void)
{
    options_phase = 0;
}

MENU_OPTIONS *menu_options_current(void)
{
    return &options_state;
}

uint32 menu_options_begin(MENU_OPTIONS *options)
{
    FUNCTION_MARKER(0x800DBAEC, "ACE3.BIN");
    return menu_options_initialize(options);
}

uint32 menu_options_initialize(MENU_OPTIONS *options)
{
    uint32 flags, value, phase, index;
    FUNCTION_MARKER(0x800DBAF4, "ACE3.BIN");
    value = game_runtime.options.captions;
    options->captions_choice = (uint8)(value != 1);
    flags = game_input.flags;
    options->button_mode_choice = (uint8)(flags & 1 ? (flags & 2 ? 0 : 1) : 2);
    value = game_runtime.account.hold_action_gate;
    if (value == 0 || value == 1)
        options->hold_action_choice = (uint8)(value);
    options->selection = (uint8)(0);
    ui_item_reset_motion(2, 16);
    ui_group_set_mask(2, 65536);
    phase = options->selection;
    if (phase == 0)
    {
        value = options->captions_choice;
        *ui_widget_value(50) = (uint8)(value);
    }
    else if (phase == 1 || phase == 2)
    {
        value = options->values[phase];
        *ui_widget_value(50) = (uint8)(value + (phase == 1 ? 2 : 5));
    }
    *ui_widget_value(51) = (uint8)(0);
    for (index = 0; index < 3; ++index)
    {
        value = options->values[index];
        *ui_widget_value(52 + index) = (uint8)(value);
    }
    value = options->selection;
    *ui_widget_value(55) = (uint8)(value);
    return 1;
}

uint32 menu_options_input(MENU_OPTIONS *options)
{
    FUNCTION_MARKER(0x800DBC94, "ACE3.BIN");
    return menu_options_update(options);
}

uint32 menu_options_update(MENU_OPTIONS *options)
{
    uint32 flags, phase, value, index;
    FUNCTION_MARKER(0x800DBC9C, "ACE3.BIN");
    flags = game_input.repeat;
    if (flags & 15)
    {
        sound_play(61);
        if (flags & 2)
            options->selection = (uint8)((options->selection + 1) % 3);
        if (flags & 1)
            options->selection = (uint8)((options->selection + 2) % 3);
        if (flags & 12)
        {
            phase = options->selection;
            if (phase == 0)
                options->captions_choice = (uint8)(options->captions_choice ^ 1);
            else if (phase == 1)
            {
                value = options->button_mode_choice;
                options->button_mode_choice = (uint8)((value + (flags & 8 ? 1 : 2)) % 3);
            }
            else if (phase == 2)
                options->hold_action_choice = (uint8)(options->hold_action_choice == 0);
        }
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        sound_play(60);
        return 1;
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        sound_play(59);
        game_input.pressed = (uint32)(0);
        return 2;
    }
    phase = options->selection;
    if (phase == 0)
    {
        value = options->captions_choice;
        *ui_widget_value(50) = (uint8)(value);
    }
    else if (phase == 1 || phase == 2)
    {
        value = options->values[phase];
        *ui_widget_value(50) = (uint8)(value + (phase == 1 ? 2 : 5));
    }
    flags = game_input.held;
    *ui_widget_value(51) = (uint8)(flags & 1 ? 1 : (flags & 2 ? 2 : 0));
    for (index = 0; index < 3; ++index)
    {
        value = options->values[index];
        *ui_widget_value(52 + index) = (uint8)(value);
    }
    value = options->selection;
    *ui_widget_value(55) = (uint8)(value);
    return 0;
}

uint32 menu_options_apply(MENU_OPTIONS *options)
{
    uint32 value, flags;
    FUNCTION_MARKER(0x800DBFC4, "ACE3.BIN");
    value = options->captions_choice;
    game_runtime.options.captions = (uint8)(value == 0);
    value = options->button_mode_choice;
    if (value == 0 || value == 1 || value == 2)
    {
        flags = game_input.flags;
        if (value == 2)
            flags &= 0xFFFFFFFCu;
        else if (value == 1)
            flags = (flags | 1) & 0xFFFFFFFDu;
        else
            flags |= 3;
        game_input.flags = (uint32)(flags);
    }
    value = options->hold_action_choice;
    if (value == 0 || value == 1)
        game_runtime.account.hold_action_gate = (uint8)(value);
    return 2;
}

void menu_options_step(void)
{
    uint32 value, result;
    FUNCTION_MARKER(0x800DC0A0, "ACE3.BIN");
    value = options_phase;
    result = mission_dispatch_detail_aux(value, menu_options_current());
    if (result == 1)
    {
        value = options_phase;
        options_phase = (uint32)(value + 1);
    }
    if (result == 2)
    {
        menu_state.header.phase = (uint8)(1);
        options_phase = (uint32)(0);
        ui_animation_set(1, 1, 0, 30);
        {
            ui_group_set_mask(2, 4096);
            return;
        }
    }
    return;
}
