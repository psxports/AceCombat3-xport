#ifndef EFFECT_EVENTS_H
#define EFFECT_EVENTS_H

#include "psx.h"
#include "effect_frame.h"
#include "effect_objects.h"

typedef struct
{
    XY endpoint[2];
    uint32 scale[2];
} EFFECT_QUAD;

void effect_quad_build(POINT_T *vertices, const EFFECT_QUAD *state);


enum { EFFECT_SPAWN_CAPACITY = 8, EFFECT_DELAYED_CAPACITY = 32 };

typedef struct
{
    uint16 particle_index;
    uint16 effect_kind;
    uint32 flags;
    const NATIVE_POSE *pose;
    const uint32 *velocity;
} EFFECT_SPAWN_REQUEST;

typedef struct
{
    uint16 delay;
    uint16 event_bits;
    uint32 actor_flags;
    uint32 condition_flags;
} EFFECT_DELAYED_EVENT;

typedef void (*EFFECT_TIMED_CALLBACK)(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);

enum { EFFECT_TIMED_CAPACITY = 32 };

typedef struct
{
    uint16 ticks;
    uint16 duration;
    uint32 position[3];
    EFFECT_TIMED_CALLBACK callback;
} EFFECT_TIMED_EVENT;

typedef struct
{
    uint32 active;
    uint16 ticks;
    uint32 position[3];
    uint32 start[3];
    uint32 end[3];
} EFFECT_BURST;

enum { EFFECT_SEGMENT_CAPACITY = 16 };

typedef struct
{
    uint16 phase;
    uint16 kind;
    uint32 origin[3];
    uint32 finish[3];
} EFFECT_SEGMENT;

typedef struct
{
    uint32 ticks;
    uint32 control;
    uint16 line_xy[4];
    uint32 trail_xy[2];
    uint32 center_xy;
} EFFECT_AIRCRAFT;

enum { EFFECT_RENDER_CAPACITY = 32 };

typedef struct
{
    uint16 ticks;
    uint16 duration;
    uint32 position[3];
    uint16 angles[2];
    uint16 kind;
    uint16 radius;
} EFFECT_RENDER;

typedef struct
{
    uint16 ticks;
    uint16 duration;
    uint32 position[3];
    uint16 vector[3];
    uint16 flags;
    uint16 style;
    uint16 radius;
} EFFECT_DRIFT;

enum { EFFECT_DRIFT_SEED_RESET = 0xC0 };

typedef struct
{
    uint32 seed_phase;
    uint32 primary_mask[3];
    uint32 primary_pending[3];
    EFFECT_DRIFT primary[96];
    uint32 secondary_mask[10];
    EFFECT_DRIFT secondary[320];
} EFFECT_DRIFT_STATE;

typedef struct
{
    uint32 *mask;
    EFFECT_DRIFT *slots;
    uint32 group_count;
} EFFECT_DRIFT_POOL;

extern EFFECT_DRIFT_STATE effect_drift;
extern EFFECT_DRIFT_POOL const effect_drift_primary_pool;
extern EFFECT_DRIFT_POOL const effect_drift_secondary_pool;

typedef struct
{
    uint16 x;
    uint16 y;
    uint16 z;
} EFFECT_LIGHT_VERTEX;

typedef struct
{
    uint32 flags;
    uint32 intensity;
    uint32 dir_xy;
    uint16 dir_z;
    EFFECT_LIGHT_VERTEX corner[4];
    uint32 inner_xy;
    uint16 inner_z;
} EFFECT_LIGHT;

extern EFFECT_LIGHT effect_light;

typedef struct
{
    uint16 sample[528];
    uint32 vector[106];
} EFFECT_NOISE;

extern EFFECT_NOISE effect_noise;

typedef struct
{
    uint32 spawn_count;
    EFFECT_SPAWN_REQUEST spawn[EFFECT_SPAWN_CAPACITY];
    uint32 delayed_mask;
    EFFECT_DELAYED_EVENT delayed[EFFECT_DELAYED_CAPACITY];
    uint32 timed_mask;
    EFFECT_TIMED_EVENT timed[EFFECT_TIMED_CAPACITY];
    EFFECT_BURST burst;
    uint32 segment_flags;
    EFFECT_SEGMENT segments[EFFECT_SEGMENT_CAPACITY];
    EFFECT_AIRCRAFT aircraft;
    uint32 render_mask;
    EFFECT_RENDER render[EFFECT_RENDER_CAPACITY];
} EFFECT_EVENT_STATE;

extern EFFECT_EVENT_STATE effect_events;
typedef struct
{
    uint32 target_mask[2];
    uint8 last_effect[64];
    uint8 source_count[64];
    uint8 target_count[64];
} EFFECT_ACTOR_TRACKING;

extern EFFECT_ACTOR_TRACKING effect_actor_tracking;


uint32 effect_spawn_enqueue(uint32 kind, const uint32 *velocity, uint32 mode, const NATIVE_POSE *native_position);
uint32 effect_spawn_player(uint32 mode);
void effect_spawn_process(void);
EFFECT_DELAYED_EVENT *effect_event_schedule(uint32 mode, uint32 flags, uint32 parameter, uint32 delay);
void effect_event_tick(void);
intptr effect_event_schedule_from_object(const EFFECT_OBJECT *state);

EFFECT_TIMED_EVENT *effect_timed_allocate(void);
void effect_timed_tick(void);
void effect_timed_draw_fade(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_0(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_1(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_2(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_3(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_4(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_5(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);
void effect_timed_emit_6(uint32 ticks, uint16 *counter, const uint16 *duration, const uint32 *position);

uint32 effect_burst_start(const uint32 *pos, const uint32 *vel);
uint32 effect_burst_trigger(const uint32 *pos, const uint32 *vel);
uint32 effect_burst_tick(void);
void effect_segment_emit(uint32 scale, uint32 kind, const uint32 *origin, const uint32 *finish);

EFFECT_SEGMENT *effect_segment_allocate(void);
void effect_segment_update(void);
void effect_segment_draw(EFFECT_FRAME_CONTEXT *frame);
void effect_layer_draw(EFFECT_FRAME_CONTEXT *frame);

void effect_aircraft_start(uint32 kind, uint32 value);
void effect_aircraft_init(void);
void effect_aircraft_trigger(uint32 value);
void effect_aircraft_update(void);
void effect_aircraft_draw(EFFECT_FRAME_CONTEXT *frame);

EFFECT_RENDER *effect_render_allocate(void);
void effect_render_update(void);
void effect_render_draw(EFFECT_FRAME_CONTEXT *frame);

EFFECT_DRIFT *effect_drift_allocate(const EFFECT_DRIFT_POOL *pool);
void effect_drift_update(const EFFECT_DRIFT_POOL *pool);
EFFECT_DRIFT *effect_drift_allocate_pending(void);
void effect_drift_project(EFFECT_FRAME_CONTEXT *frame);
void effect_drift_draw(EFFECT_FRAME_CONTEXT *frame);

void effect_light_init(EFFECT_LIGHT *state);
uint32 effect_light_prepend_chain(uint32 *ot_tag, uint32 ot_lane, uint32 first, uint32 tail, uint32 seed);
void effect_light_draw_halo(EFFECT_LIGHT *state, uint32 color, EFFECT_FRAME_CONTEXT *frame);
void effect_light_draw_flare(EFFECT_LIGHT *state, uint32 flags, uint32 distance, EFFECT_FRAME_CONTEXT *frame);

void effect_noise_init(EFFECT_NOISE *state);

void effect_system_init(void);
void effect_pools_reset(void);
void effect_transients_clear(void);

uint32 effect_tick_frame(void);

void effect_schedule_timed_positions(uint32 kind);

#endif
