#ifndef RESOURCE_CONTROLLER_H
#define RESOURCE_CONTROLLER_H

#include "psx.h"

typedef struct
{
    uint32 phase;
    uint32 subphase;
    uint32 phase_start;
    uint32 status;
    uint32 bp_sector;
    uint32 sp_sector;
    const uint32 *clock;
    uint32 wait_mdec;
    union
    {
        uint32 response_words[2];
        uint8 response[8];
    };
} RESOURCE_CONTROLLER;

extern RESOURCE_CONTROLLER resource_controller;

void resource_init_controller(const uint32 *counter);
sint32 resource_eject_step(uint8 *response);
sint32 resource_detect_step(uint8 *response);
uint32 resource_begin_recovery(void);
uint32 resource_request_standby(RESOURCE_CONTROLLER *state);
uint32 resource_try_standby(void);
uint32 resource_get_recovery_state(void);
intptr resource_poll_cd(void);

sint32 cd_resource_state(void);

#endif
