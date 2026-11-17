#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

static UI_ANIMATION_CHANNEL animation_channels[UI_ANIMATION_CHANNEL_COUNT];

void ui_animation_reset(void)
{
    memset(animation_channels, 0, sizeof(animation_channels));
}

UI_ANIMATION_CHANNEL *ui_animation_channel(uint32 index)
{
    if (index >= UI_ANIMATION_CHANNEL_COUNT)
        abort();
    return animation_channels + index;
}

void ui_animation_set(uint32 slot, uint32 phase, sint32 ticks, sint32 limit)
{
    UI_ANIMATION_CHANNEL *channel;
    FUNCTION_MARKER(0x800DEE08, "ACE3.BIN");
    channel = ui_animation_channel(slot);
    channel->phase = (uint16)phase;
    if (limit >= 0)
        channel->limit = (uint16)limit;
    if (ticks >= 0)
        channel->ticks = (uint16)ticks;
}

uint32 ui_animation_phase(uint32 slot)
{
    FUNCTION_MARKER(0x800DEE84, "ACE3.BIN");
    return (sint16)ui_animation_channel(slot)->phase;
}

uint32 ui_animation_ticks(uint32 slot)
{
    FUNCTION_MARKER(0x800DEEA8, "ACE3.BIN");
    return (sint16)ui_animation_channel(slot)->ticks;
}

void ui_animation_tick(void)
{
    uint32 index, value;
    sint32 current, limit;
    UI_ANIMATION_CHANNEL *channel;

    FUNCTION_MARKER(0x800DEECC, "ACE3.BIN");
    for (index = 0; index < UI_ANIMATION_CHANNEL_COUNT; ++index)
    {
        channel = ui_animation_channel(index);
        if ((sint16)channel->phase == 1)
        {
            current = (sint16)channel->ticks;
            limit = (sint16)channel->limit;
            value = channel->ticks;
            if (current < limit)
                channel->ticks = (uint16)(value + 1);
            else
                channel->phase = 2;
        }
        if ((sint16)channel->phase == 3)
        {
            current = (sint16)channel->ticks;
            value = channel->ticks;
            if (current > 0)
                channel->ticks = (uint16)(value - 1);
            else
                channel->phase = 0;
        }
    }
    return;
}
