#ifndef FLIGHT_STATUS_H
#define FLIGHT_STATUS_H

#include "flight_model.h"
#include "psx.h"
#include "mission_actor.h"

enum
{
    FLIGHT_WEAPON_SLOT_COUNT = 12,
    FLIGHT_PRIMARY_WEAPON_KIND_COUNT = 7,
    FLIGHT_SECONDARY_WEAPON_KIND_COUNT = 13
};

typedef uint32 (*FLIGHT_PRIMARY_WEAPON_CALLBACK)(void);
typedef intptr (*FLIGHT_SECONDARY_WEAPON_CALLBACK)(uint8 *slots);

extern const sint16 flight_weapon_limits[FLIGHT_SECONDARY_WEAPON_KIND_COUNT];

typedef struct
{
    const FLIGHT_AIRCRAFT_PROFILE *model;
    uint32 weapon_limit;
    uint32 target_angle_limit;
    uint32 target_distance_limit[2];
    uint32 max_health;
    uint32 target_index;
    uint32 ammunition;
    uint32 active_weapons;
    uint32 events;
    uint32 ground_height;
    uint32 ground_clearance;
    uint16 ground_normal[3];
    uint32 surface_flags;
    uint8 weapon_cooldown[2][FLIGHT_WEAPON_SLOT_COUNT];
} FLIGHT_STATUS;

extern FLIGHT_STATUS flight_status;

intptr flight_weapon_fire_variant_1(uint8 *slots);
intptr flight_weapon_fire_variant_3(uint8 *slots);
intptr flight_weapon_fire_variant_4(uint8 *slots);
intptr flight_weapon_fire_variant_5(uint8 *slots);
intptr flight_weapon_fire_variant_6(uint8 *slots);
intptr flight_weapon_fire_variant_7(uint8 *slots);
intptr flight_weapon_fire_variant_2(uint8 *slots);
intptr flight_weapon_fire_variant_8(uint8 *slots);
intptr flight_weapon_fire_variant_9(uint8 *slots);
intptr flight_weapon_fire_variant_10(uint8 *slots);
intptr flight_weapon_fire_variant_11(uint8 *slots);
uint32 flight_status_sample_ground(const uint32 position[3], uint16 normal[3], uint32 *surface);

void flight_weapon_tick_cooldowns(void);
uint32 flight_weapon_update_slots(uint8 *slots, uint32 count, uint32 value);
uint32 flight_weapon_fire_primary_1(void);
uint32 flight_weapon_fire_primary_2(void);
uint32 flight_weapon_fire_primary_3(void);
uint32 flight_weapon_fire_primary_4(void);
uint32 flight_weapon_fire_primary_6(void);
uint32 flight_weapon_fire_primary_5(void);
intptr flight_weapon_fire_secondary(uint32 kind, uint8 *slots);
void flight_weapon_fire_primary(uint32 kind);

void flight_restore_player_health(void);

void flight_status_apply_actor_events(uint32 flags);

void flight_initialize_actor(const MISSION_ACTOR_TRAITS *actor_traits_source);

uint32 flight_primary_weapon_override(void);
uint32 flight_secondary_weapon_override(void);

#endif
