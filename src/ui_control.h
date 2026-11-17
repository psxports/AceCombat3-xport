#ifndef UI_CONTROL_H
#define UI_CONTROL_H

#include "ui_items.h"

typedef struct
{
    sint16 x;
    sint16 y;
    sint16 start_tick;
    sint16 split_tick;
    sint16 end_tick;
    sint16 mode;
    sint16 channel;
    sint16 layer;
} UI_CONTROL;

void ui_control_reset(void);
void ui_control_read(UI_CONTROL *control);
const uint8 *ui_control_select(UI_COMMAND_STREAM input);

#endif
