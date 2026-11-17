#include "game_functions.h"

static const UI_CONTROL default_control = {0, 0, 0, 0, 0, 0, 0, 1};
static UI_COMMAND_STREAM control_record;

void ui_control_reset(void)
{
    control_record.data = NULL;
    control_record.bytes = 0;
}

void ui_control_read(UI_CONTROL *control)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    if (!control_record.data)
    {
        *control = default_control;
        return;
    }
    control->x = (sint16)(memcpy(&wire_halfword_0, control_record.data, sizeof(wire_halfword_0)), wire_halfword_0);
    control->y = (sint16)(memcpy(&wire_halfword_1, control_record.data + 2, sizeof(wire_halfword_1)), wire_halfword_1);
    control->start_tick = (sint16)(memcpy(&wire_halfword_2, control_record.data + 4, sizeof(wire_halfword_2)), wire_halfword_2);
    control->split_tick = (sint16)(memcpy(&wire_halfword_3, control_record.data + 6, sizeof(wire_halfword_3)), wire_halfword_3);
    control->end_tick = (sint16)(memcpy(&wire_halfword_4, control_record.data + 8, sizeof(wire_halfword_4)), wire_halfword_4);
    control->mode = (sint16)(memcpy(&wire_halfword_5, control_record.data + 10, sizeof(wire_halfword_5)), wire_halfword_5);
    control->channel = (sint16)(memcpy(&wire_halfword_6, control_record.data + 12, sizeof(wire_halfword_6)), wire_halfword_6);
    control->layer = (sint16)(memcpy(&wire_halfword_7, control_record.data + 14, sizeof(wire_halfword_7)), wire_halfword_7);
}

const uint8 *ui_control_select(UI_COMMAND_STREAM input)
{
    FUNCTION_MARKER(0x800E01AC, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 16)
        abort();
    control_record = input;
    return command + 16;
}
