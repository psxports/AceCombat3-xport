#include "game_functions.h"

static const uint8 button_code_by_action[20] = {13, 12, 17, 19, 16, 8, 9, 10, 11, 18, 14, 15, 0, 1, 2, 3, 6, 7, 4, 5};
static const uint8 profile_kind_by_slot[5] = {2, 0, 0, 3, 1};
static const uint16 button_mask_by_key[15] = {256, 1024, 16, 32, 128, 64, 2, 512, 2048, 4096, 32768, 8192, 16384, 4, 1};
static const uint16 axis_mask_by_binding[4] = {2, 3, 0, 1};

void pad_settings_read_profiles(void)
{
    uint8 *binding_destination;
    uint8 *binding_choices;
    uint8 *binding_enabled;
    uint32 input, slot, index, key;
    const PAD_PROFILE *profile;
    uint32 binding, flags;
    uint32 type, value, result, quotient;
    sint32 input_index, reverse;
    sint32 mask;
    uint64 product;

    FUNCTION_MARKER(0x800D1B6C, "ACE3.BIN");
    input = game_input.kind;
    input_index = pad_profile_index(input);
    for (slot = 0; slot < 5; ++slot)
    {
        profile = &pad_profiles[slot];
        type = profile_kind_by_slot[slot];
        binding_destination = menu_bindings_profile((uint32)type)->button;
        for (reverse = 14; reverse >= 0; --reverse)
            binding_destination[(uint32)reverse] = (20);
        for (key = 11; key < 31; ++key)
        {
            for (index = 0; index < 15; ++index)
            {
                mask = (sint16)button_mask_by_key[index];
                value = profile->binding[key];
                if ((uint32)mask & value)
                    binding_destination[index] = (uint8)(button_code_by_action[key - 11]);
            }
        }
        type = profile_kind_by_slot[slot];
        binding_choices = menu_bindings_profile((uint32)type)->axis;
        binding_enabled = menu_bindings_profile((uint32)type)->qualifier;
        for (reverse = 3; reverse >= 0; --reverse)
            binding_choices[(uint32)reverse] = (12);
        for (key = 0; key < 6; ++key)
        {
            for (binding = 0; binding < 4; ++binding)
            {
                mask = (sint16)axis_mask_by_binding[binding];
                value = profile->axis_map[key];
                if (mask == (sint32)value)
                {
                    binding_choices[binding] = (key * 2);
                    if (!key)
                        binding_enabled[binding] = (!(profile->axis_flags & 4));
                    else if (key == 3)
                        binding_enabled[binding] = (!(profile->axis_flags & 8));
                    else if (key == 4)
                        binding_enabled[binding] = ((profile->axis_flags & 0x10) != 0);
                }
            }
        }
        flags = profile->axis_flags;
        type = profile_kind_by_slot[slot];
        value = flags & 2 ? 0 : (flags & 1 ? 2 : 1);
        menu_bindings_profile((uint32)type)->axis_mode = (uint8)(value);
    }
    flags = game_input.flags & 4;
    if (flags)
    {
        menu_bindings.vibration_choice = (uint8)(0);
    }
    else
    {
        result = 1;
        menu_bindings.vibration_choice = (uint8)(result);
    }
    if (input_index < 0)
    {
        menu_bindings.dead_zone_choice = 0;
        return;
    }
    profile = &pad_profiles[input_index];
    flags = game_input.kind;
    value = profile->cal[0].dead_zone;
    if (flags == 4)
    {
        product = (uint64)(value >> 1) * 0x6BCA1AF3;
        quotient = (uint32)(product >> 32) >> 3;
    }
    else
    {
        quotient = value >> 4;
    }
    menu_bindings.dead_zone_choice = (uint8)(quotient - 1);
    flags = profile->flags;
    if (flags & 8)
    {
        result = 1;
        menu_bindings.delay_option[0] = (uint8)(result);
    }
    else
    {
        menu_bindings.delay_option[0] = (uint8)(0);
    }
    if (flags & 0x10)
    {
        result = 1;
        menu_bindings.delay_option[1] = (uint8)(result);
    }
    else
    {
        menu_bindings.delay_option[1] = (uint8)(0);
    }
    return;
}

void pad_settings_write_profiles(void)
{
    uint8 *binding_descriptor;
    uint8 *binding_choices;
    uint8 *binding_qualifiers;
    uint32 record;
    PAD_PROFILE *profile;
    uint32 index, entry, mapping, value, flags, condition, result, command;
    sint32 active;
    FUNCTION_MARKER(0x800D1F54, "ACE3.BIN");
    active = pad_profile_index(game_input.kind);
    for (record = 0; record < 5; ++record)
    {
        profile = &pad_profiles[record];
        entry = profile_kind_by_slot[record];
        binding_descriptor = menu_bindings_profile((uint32)entry)->button;
        for (index = 11; index < 32; ++index)
            profile->binding[index] = 0;
        for (index = 0; index < 15; ++index)
        {
            entry = binding_descriptor[index];
            mapping = menu_binding_action_key(entry);
            if (mapping != 31)
            {
                value = button_mask_by_key[index];
                profile->binding[mapping] = (uint16)value;
            }
        }
        entry = profile_kind_by_slot[record];
        binding_choices = menu_bindings_profile((uint32)entry)->axis;
        binding_qualifiers = menu_bindings_profile((uint32)entry)->qualifier;
        for (index = 0; index < 6; ++index)
            profile->axis_map[5 - index] = 255;
        for (index = 0; index < 4; ++index)
        {
            value = binding_choices[index];
            mapping = value >> 1;
            if (mapping < 6)
            {
                value = (uint8)axis_mask_by_binding[index];
                profile->axis_map[mapping] = (uint8)value;
                value = binding_choices[index];
                if (value == 0 || value == 6 || value == 8)
                {
                    condition = binding_qualifiers[index];
                    flags = profile->axis_flags;
                    if (value == 0)
                        flags = condition ? flags & 0xFFFB : flags | 4;
                    else if (value == 6)
                        flags = condition ? flags & 0xFFF7 : flags | 8;
                    else
                        flags = condition == 1 ? flags | 16 : flags & 0xFFEF;
                    profile->axis_flags = (uint16)(flags);
                }
            }
        }
        flags = profile->axis_flags & 0xFFFC;
        profile->axis_flags = (uint16)(flags);
        entry = profile_kind_by_slot[record];
        value = menu_bindings_profile((uint32)entry)->axis_mode;
        if (value == 0)
            profile->axis_flags = (uint16)(flags | 2);
        else if (value == 2)
            profile->axis_flags = (uint16)(flags | 1);
    }
    flags = game_input.flags;
    flags &= 0xFFFFFFFB;
    game_input.flags = (uint32)(flags);
    condition = menu_bindings.vibration_choice;
    result = flags | 4;
    if (!condition)
        game_input.flags = (uint32)(result);
    if (active < 0)
        return;
    profile = &pad_profiles[active];
    condition = menu_bindings.delay_option[0];
    flags = profile->flags;
    flags = condition == 1 ? flags | 8 : flags & 0xFFFFFFF7;
    profile->flags = flags;
    condition = menu_bindings.delay_option[1];
    flags = profile->flags;
    flags = condition == 1 ? flags | 16 : flags & 0xFFFFFFEF;
    profile->flags = flags;
    profile = &pad_profiles[active];
    value = menu_bindings.dead_zone_choice;
    if (value > 2)
        return;
    condition = game_input.kind;
    command = condition == 4 ? (value + 1) * 38 : (value + 1) * 16;
    for (index = 0; index < 4; ++index)
    {
        profile->cal[3 - index].dead_zone = (uint8)command;
    }
    return;
}
