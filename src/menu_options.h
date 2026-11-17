#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H

#include "psx.h"

typedef union
{
    struct
    {
        uint8 captions_choice;
        uint8 button_mode_choice;
        uint8 hold_action_choice;
        uint8 selection;
    };
    uint8 values[4];
} MENU_OPTIONS;

void menu_options_reset(void);
void menu_options_start(void);
void menu_options_step(void);
MENU_OPTIONS *menu_options_current(void);
uint32 menu_options_begin(MENU_OPTIONS *options);
uint32 menu_options_initialize(MENU_OPTIONS *options);
uint32 menu_options_input(MENU_OPTIONS *options);
uint32 menu_options_update(MENU_OPTIONS *options);
uint32 menu_options_apply(MENU_OPTIONS *options);

#endif
