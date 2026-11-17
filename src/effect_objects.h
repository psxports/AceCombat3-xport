#ifndef EFFECT_OBJECTS_H
#define EFFECT_OBJECTS_H

#include "game_math.h"

typedef void (*EFFECT_SPAWN_CALLBACK)(uint32 x, uint32 y, uint32 z);

extern EFFECT_SPAWN_CALLBACK const effect_spawn_callbacks[21];

enum { EFFECT_OBJECT_CAPACITY = 32 };
typedef struct
{
    uint32 flags;
    uint8 kind, launch_kind, guide_kind, impact_kind;
    uint16 phase, ticks;
    uint32 material;
    uint16 normal[3], lifetime, event_bits;
    NATIVE_POSE pose;
    uint8 model_kind, render_flags;
    uint32 velocity[3], local_velocity[3];
    uint16 delta_angles[3];
    uint32 acceleration, initial_speed, distance, source_flags, target_flags;
    /* guidance requires the target pair bound by allocation */
    const uint32 *target_position, *target_velocity;
    uint16 turn_limit, launch_delay;
    uint32 maximum_speed, launch_acceleration, guidance_acceleration;
    uint32 impact_velocity[3], split_distance, area_radius, delay_divisor;
} EFFECT_OBJECT;

typedef struct
{
    uint32 live;
    EFFECT_OBJECT slot[EFFECT_OBJECT_CAPACITY];
} EFFECT_OBJECT_POOL;

typedef intptr (*EFFECT_OBJECT_HANDLER)(EFFECT_OBJECT *state);
extern EFFECT_OBJECT_POOL effect_objects;




EFFECT_OBJECT *effect_object_create_variant_1(uint32 flags, uint32 owner, uint32 mode, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_2(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_3(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_4(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_5(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_6(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_7(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_8(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_9(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_10(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
EFFECT_OBJECT *effect_object_create_variant_11(uint32 flags, uint32 owner, const NATIVE_POSE *native_position, const uint32 *native_vector);
intptr effect_object_launch_1(EFFECT_OBJECT *state);
intptr effect_object_launch_2(EFFECT_OBJECT *state);
intptr effect_object_launch_3(EFFECT_OBJECT *state);
intptr effect_object_launch_4(EFFECT_OBJECT *state);
intptr effect_object_launch_5(EFFECT_OBJECT *state);
intptr effect_object_guide_1(EFFECT_OBJECT *state);
intptr effect_object_guide_2(EFFECT_OBJECT *state);
intptr effect_object_guide_3(EFFECT_OBJECT *state);
intptr effect_object_guide_4(EFFECT_OBJECT *state);
intptr effect_object_guide_5(EFFECT_OBJECT *state);
intptr effect_object_impact_1(EFFECT_OBJECT *state);
intptr effect_object_impact_2(EFFECT_OBJECT *state);
intptr effect_object_impact_3(EFFECT_OBJECT *state);
intptr effect_object_impact_4(EFFECT_OBJECT *state);
void effect_object_noop(void);
void effect_objects_run_modes(void);
void effect_objects_collide(void);
void effect_objects_advance(void);
void effect_objects_expire(void);
void effect_objects_update(void);
void effect_object_schedule_area(EFFECT_OBJECT *state, uint32 value);
void effect_object_publish_pose(uint32 index, uint32 kind, const NATIVE_POSE *native_source);
void effect_object_hide(uint32 index);
void effect_objects_reset(void);
void effect_system_initialize(void);
void effect_system_update(void);

void effect_objects_clear(void);

#endif
