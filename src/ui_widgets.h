#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include "psx.h"

enum
{
    UI_WIDGET_COUNT = 64
};

enum
{
    UI_ACCOUNT_KEY_BANKS = 2,
    UI_ACCOUNT_KEY_COUNT = 51
};

typedef struct
{
    sint16 x;
    sint16 y;
    uint8 glyph[2];
} UI_ACCOUNT_KEY;

extern const UI_ACCOUNT_KEY ui_account_keys[UI_ACCOUNT_KEY_BANKS][UI_ACCOUNT_KEY_COUNT];

extern const char ui_default_account_name[5];
extern const char ui_card_account_paths[2][23];

void ui_widgets_reset(void);
uint8 *ui_widget_value(uint32 index);
uint32 ui_widget_get(uint32 index);
void ui_widget_set(uint32 index, uint32 value);

#endif
