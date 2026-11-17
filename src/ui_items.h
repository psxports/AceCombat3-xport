#ifndef UI_ITEMS_H
#define UI_ITEMS_H

#include "psx.h"

enum
{
    UI_ITEM_GROUP_COUNT = 3,
    UI_ITEM_COUNT = 32
};

typedef struct
{
    uint16 x;
    uint16 y;
    uint16 velocity;
    uint16 phase;
} UI_ITEM;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} UI_COMMAND_STREAM;

extern const UI_COMMAND_STREAM ui_boot_commands;

UI_COMMAND_STREAM ui_group_directory(uint32 group);
UI_COMMAND_STREAM ui_command_directory(const uint8 *data, uint32 data_bytes, uint32 begin, uint32 end);
UI_COMMAND_STREAM ui_group_command_stream(uint32 group, uint32 index);
void ui_group_bind(UI_COMMAND_STREAM directory, uint32 group);
void ui_group_set_mask(uint32 group, uint32 mask);

void ui_items_reset(void);
UI_ITEM *ui_item_at(uint32 group, uint32 index);
void ui_item_set_motion(uint32 group, uint32 index, uint32 phase, uint32 x, uint32 y, uint32 velocity);
void ui_item_reset_motion(uint32 group, uint32 index);
void ui_item_reverse_motion(uint32 group, uint32 index);
void ui_item_set_pos(uint32 group, uint32 index, sint32 x, sint32 y);
uint32 ui_item_motion_phase(uint32 group, uint32 index);
void ui_item_get_pos(uint32 group, uint32 item, uint16 *position);
void ui_item_step_motion(uint32 index);

void ui_item_final_hook_default(void);
void ui_item_final_hook_0_12(void);
void ui_item_final_hook_0_15(void);
void ui_item_final_hook_1_0(void);
void ui_item_final_hook_1_4(void);
void ui_groups_render(void);
void ui_item_render(uint32 index, uint32 group);

#endif
