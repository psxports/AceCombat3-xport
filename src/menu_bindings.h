#ifndef MENU_BINDINGS_H
#define MENU_BINDINGS_H

#include "psx.h"

typedef struct
{
    uint8 button[15];
    uint8 axis[4];
    uint8 qualifier[4];
    uint8 axis_mode;
} MENU_BINDINGS_PROFILE;

typedef struct
{
    MENU_BINDINGS_PROFILE profile[4];
    uint8 delay_option[2];
    uint8 vibration_choice;
    uint8 dead_zone_choice;
} MENU_BINDINGS;

extern MENU_BINDINGS menu_bindings;
void menu_bindings_reset(void);
MENU_BINDINGS_PROFILE *menu_bindings_profile(uint32 profile);
uint8 menu_bindings_axis_mode(uint32 profile);
void menu_bindings_default_analog(void);
void menu_bindings_default_negcon(void);
void menu_bindings_default_digital(void);
void menu_bindings_default_joystick(void);

void menu_bindings_prepare_analog_labels(void);
void menu_bindings_prepare_negcon_labels(void);
void menu_bindings_prepare_digital_labels(void);
void menu_bindings_prepare_joystick_labels(void);
void menu_bindings_prepare_labels(void);
uint32 menu_bindings_label_variant(uint32 type);
void menu_bindings_draw_analog_buttons_frame(void);
void menu_bindings_draw_analog_buttons(void);
void menu_bindings_draw_negcon_buttons(void);
void menu_bindings_draw_digital_buttons(void);
void menu_bindings_draw_joystick_buttons(void);
void menu_bindings_draw_buttons(void);
void menu_bindings_draw_analog_axes_frame(void);
void menu_bindings_draw_analog_axes(void);
void menu_bindings_draw_negcon_axes_frame(void);
void menu_bindings_draw_negcon_axes(void);
void menu_bindings_draw_joystick_axes_frame(void);
void menu_bindings_draw_joystick_axes(void);
void menu_bindings_draw_axes(void);
void menu_bindings_edit_begin(void);
void menu_bindings_options_frame(void);
void menu_bindings_options_tick(void);

void menu_bindings_toggle_tab(void);
void menu_bindings_button_option_frame(void);
void menu_bindings_button_option_tick(void);
void menu_bindings_button_action_tick(void);
void menu_bindings_button_cursor_tick(void);
void menu_bindings_axis_qualifier_tick(void);
void menu_bindings_axis_action_frame(void);
void menu_bindings_axis_action_tick(void);
void menu_bindings_axis_cursor_tick(void);
void menu_bindings_edit_tick(void);

void menu_bindings_screen_begin(void);
void menu_bindings_screen_frame(void);
void menu_bindings_screen_tick(void);

#endif
