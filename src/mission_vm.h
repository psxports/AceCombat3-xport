#ifndef MISSION_VM_H
#define MISSION_VM_H

#include "psx.h"

typedef struct
{
    const uint8 *bytes;
    uint32 size;
    uint32 legacy_address;
} MISSION_SCRIPT_REFERENCE;

typedef struct
{
    MISSION_SCRIPT_REFERENCE root;
    MISSION_SCRIPT_REFERENCE selected;
} MISSION_VARIANT_INPUT;

MISSION_SCRIPT_REFERENCE mission_script_primary_source(void);
MISSION_SCRIPT_REFERENCE mission_script_scene_source(void);
void mission_script_publish_scene(void);
void mission_script_reset_region(void);
void mission_script_bind_region(const uint8 *data, uint32 base, uint32 end, MISSION_SCRIPT_REFERENCE backing);
void mission_script_bind_aircraft_region(const uint8 *data, uint32 base, uint32 end);
MISSION_SCRIPT_REFERENCE mission_script_reference(uint32 address);
uint32 mission_script_reference_present(MISSION_SCRIPT_REFERENCE reference);
void mission_script_start_legacy(MISSION_SCRIPT_REFERENCE script);

#endif
