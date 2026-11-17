#include "game_functions.h"

RESOURCE_CONTROLLER resource_controller;

void resource_init_controller(const uint32 *counter)
{
    RESOURCE_CONTROLLER *state;
    sint32 result;

    FUNCTION_MARKER(0x8002B034, "ACE3.BIN");
    state = &resource_controller;
    state->clock = counter;
    state->phase = 0;
    state->subphase = 0;
    state->status = 0;
    state->phase_start = 0;
    state->wait_mdec = 0;
    resource_set_cd_volume(255, 0);
    for (;;)
    {
        result = resource_init_archives();
        if (result)
            { (void)(uint32)(result); return; }
        VSync(0);
    }
}

sint32 resource_eject_step(uint8 *response)
{
    uint32 flags, phase;
    RESOURCE_CONTROLLER *state;
    uint8 mode = 0;
    FUNCTION_MARKER(0x8002B094, "ACE3.BIN");
    flags = response[0];
    if ((flags & 16) != 0)
        return 1;
    if (CdSync(1, (uint8 *)response) != 2)
        return 0;
    state = &resource_controller;
    phase = state->subphase;
    if (phase == 0)
    {
        if (CdControlB(14, (uint8 *)&mode, (uint8 *)response) != 0)
        {
            state = &resource_controller;
            state->subphase = 1;
        }
    }
    else if (phase == 1)
    {
        if (CdControl(8, NULL, (uint8 *)response) != 0)
        {
            state = &resource_controller;
            state->subphase = 2;
        }
    }
    else if (phase == 2)
        CdControlB(1, NULL, (uint8 *)response);
    return 0;
}

sint32 resource_detect_step(uint8 *response)
{
    uint32 flags, other, phase, saved, now;
    const uint32 *clock;
    RESOURCE_CONTROLLER *state;
    uint8 read_mode = 128;
    uint8 reset_pos[4] = {0, 2, 0, 0};
    FUNCTION_MARKER(0x8002B194, "ACE3.BIN");
    flags = response[0];
    if ((flags & 1) != 0)
    {
        other = response[1];
        if ((other & 64) != 0)
            goto cd_error_2b194;
    }
    flags = response[0];
    if ((flags & 4) != 0)
    {
        other = response[1];
        if ((other & 4) != 0)
            goto cd_error_2b194;
    }
    flags = response[0];
    if ((flags & 16) != 0)
    {
        state = &resource_controller;
        state->subphase = 0;
        state->status = 4;
    }
    if (CdSync(1, (uint8 *)response) != 2)
        return 0;
    state = &resource_controller;
    phase = state->subphase;
    switch (phase)
    {
        case 0:
            flags = response[0];
            if ((flags & 16) == 0)
            {
                state = &resource_controller;
                clock = state->clock;
                state->subphase = 1;
                state->status = 0;
                now = *(clock);
                state->phase_start = now;
            }
            CdControlB(1, NULL, (uint8 *)response);
            return 0;
        case 1:
            state = &resource_controller;
            clock = state->clock;
            saved = state->phase_start;
            now = *(clock);
            if ((sint32)(saved + 180) < (sint32)now)
            {
                state->status = 4;
                return -2;
            }
            flags = response[0];
            if ((flags & 2) != 0)
                state->subphase = 2;
            CdControlB(1, NULL, (uint8 *)response);
            return 0;
        case 2:
            if (CdControlB(19, NULL, (uint8 *)response) != 0)
            {
                state = &resource_controller;
                state->subphase = 3;
            }
            return 0;
        case 3:
            if (CdControl(2, (uint8 *)reset_pos, (uint8 *)response) != 0)
            {
                state = &resource_controller;
                state->subphase = 4;
            }
            return 0;
        case 4:
            if (CdControl(21, NULL, (uint8 *)response) != 0)
            {
                state = &resource_controller;
                state->subphase = 5;
            }
            return 0;
        case 5:
            if (CdControl(1, NULL, (uint8 *)response) != 0)
            {
                state = &resource_controller;
                state->subphase = 6;
            }
            return 0;
        case 6:
            if (CdControl(14, (uint8 *)&read_mode, (uint8 *)response) != 0)
            {
                state = &resource_controller;
                clock = state->clock;
                state->subphase = 7;
                now = *(clock);
                state->phase_start = now;
            }
            return 0;
        case 7:
            state = &resource_controller;
            clock = state->clock;
            saved = state->phase_start;
            now = *(clock);
            return (sint32)(saved + 3) < (sint32)now;
        default:
            return 0;
    }
cd_error_2b194:
    state = &resource_controller;
    state->status = 5;
    return -1;
}

uint32 resource_begin_recovery(void)
{
    uint32 result;
    RESOURCE_CONTROLLER *state;
    FUNCTION_MARKER(0x8002B428, "ACE3.BIN");
    state = &resource_controller;
    result = state->phase;
    if (result != 0)
        return result;
    resource_stop_cd(1);
    state = &resource_controller;
    state->phase = 2;
    state->subphase = 0;
    return 2;
}

uint32 resource_request_standby(RESOURCE_CONTROLLER *state)
{
    uint32 value, phase;
    RESOURCE_CONTROLLER *root;
    FUNCTION_MARKER(0x8002B470, "ACE3.BIN");
    value = state->response[0];
    if ((value & 16) != 0)
        return 0;
    value = state->status;
    if (value - 1 >= 2)
        return 0;
    phase = state->phase;
    if (phase != 2)
        return 0;
    value = state->subphase;
    if (value != phase)
        return 0;
    if (CdControl(7, NULL, (uint8 *)state->response) == 0)
        return 0;
    root = &resource_controller;
    root->phase = 0;
    root->subphase = 0;
    return 1;
}

uint32 resource_try_standby(void)
{
    RESOURCE_CONTROLLER *state;

    FUNCTION_MARKER(0x8002B46C, "ACE3.BIN");
    state = &resource_controller;
    return resource_request_standby(state);
}

uint32 resource_get_recovery_state(void)
{
    uint32 state, phase;
    RESOURCE_CONTROLLER *context;

    FUNCTION_MARKER(0x8002B528, "ACE3.BIN");
    context = &resource_controller;
    state = context->phase;
    if (state == 2)
    {
        phase = context->subphase;
        return phase == 2 ? 2 : 1;
    }
    if (state == 3)
    {
        phase = context->subphase;
        return phase == 0 ? 3 : 1;
    }
    return state == 4;
}

intptr resource_poll_cd(void)
{
    RESOURCE_CONTROLLER *state;
    uint32 result;
    sint32 progress;

    FUNCTION_MARKER(0x8002A2C0, "ACE3.BIN");
    if (game_resource_music_active())
    {
        game_poll_resource_music();
        return 0;
    }
    state = &resource_controller;
    switch (state->phase)
    {
        case 0:
            state = &resource_controller;
            if (state->response[0] & 0x10)
            {
                state->status = 4;
                return resource_begin_recovery();
            }
            game_cd_poll_stream();
            result = CdSync(1, (uint8 *)state->response);
            return resource_poll_transfer(result);
        case 2:
            progress = resource_eject_step(resource_controller.response);
            result = 3;
            if (progress == 1)
            {
                state = &resource_controller;
                state->phase = 3;
                state->subphase = 0;
            }
            return result;
        case 3:
            progress = resource_detect_step(resource_controller.response);
            if (progress < 0)
            {
                state = &resource_controller;
                state->phase = 2;
                state->subphase = 0;
                return 2;
            }
            if (progress == 1)
            {
                state = &resource_controller;
                state->phase = 4;
                state->subphase = 0;
            }
            return 4;
        case 4:
            state = &resource_controller;
            if (state->response[0] & 0x10)
                return resource_begin_recovery();
            progress = resource_init_archives();
            if (progress == 1)
            {
                state = &resource_controller;
                state->phase = 0;
                state->subphase = 0;
                return (intptr)state;
            }
            if (progress < 0)
            {
                state = &resource_controller;
                state->phase = 2;
                state->subphase = 0;
                return 2;
            }
            return 1;
        default:
            return 0x80010000;
    }
}


sint32 cd_resource_state(void)
{
    sint32 state;

    FUNCTION_MARKER(0x8002B504, "ACE3.BIN");
    state = (sint32)resource_controller.status;
    return state == 5 ? -1 : state;
}
