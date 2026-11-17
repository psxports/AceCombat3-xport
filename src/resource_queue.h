#ifndef RESOURCE_QUEUE_H
#define RESOURCE_QUEUE_H

#include <stddef.h>

#include "psx.h"

typedef struct RESOURCE_AUDIO_TRACK
{
    sint16 id;
    sint16 fade;
    sint16 duration;
    sint16 sector_offset;
    const struct RESOURCE_AUDIO_TRACK *next;
} RESOURCE_AUDIO_TRACK;

typedef void (*RESOURCE_COMPLETION)(void);

typedef struct
{
    const uint8 *data;
    uint32 bytes, encoded_base;
} RESOURCE_ASSET_SOURCE;

typedef struct
{
    uint8 *data;
    uint32 bytes;
} RESOURCE_READ_BUFFER;

typedef struct
{
    RESOURCE_READ_BUFFER buffer;
    /* encoded origin is progress data, not a host pointer */
    uint32 encoded_begin;
} RESOURCE_STREAM_BUFFER;

typedef struct
{
    uint32 resource_id;
    /* encoded base retained at remaining resource/device boundaries */
    uint32 encoded_base;
    uint32 read_bytes;
    uint8 *data;
    uint32 request_sequence;
    RESOURCE_STREAM_BUFFER backing;
} RESOURCE_ASSET_SLOT;

typedef struct
{
    uint32 consumed;
    uint32 queued;
    uint32 priority_active;
    uint32 current_id;
} RESOURCE_QUEUE_STATUS;

void resource_queue_reset(void);
uint32 resource_queue_next_sequence(void);
uint32 resource_queue_is_completing(uint32 sequence);
RESOURCE_QUEUE_STATUS resource_queue_status(void);

void resource_init(void);
void resource_init_queue(uint32 begin, RESOURCE_COMPLETION callback);
void resource_pause_stream(uint32 mode);
void resource_cancel_streams(void);
uint32 resource_queue_read(uint32 id, uint32 dst, RESOURCE_COMPLETION completion, RESOURCE_READ_BUFFER buffer);
uint32 resource_queue_asset(RESOURCE_ASSET_SLOT *slot, RESOURCE_STREAM_BUFFER destination, RESOURCE_COMPLETION completion);
uint32 resource_queue_audio(const RESOURCE_AUDIO_TRACK *descriptor, uint32 enabled, RESOURCE_COMPLETION callback);
void resource_queue_stream(uint32 id, RESOURCE_STREAM_BUFFER destination, uint32 flags, RESOURCE_COMPLETION callback);
uint32 resource_wait(uint32 mode);
void resource_poll(void);
uint32 resource_read_complete(uint32 result);
sint32 sound_get_cur_resource(void);
void resource_set_mono(uint32 mode);
void resource_apply_mix(void);
void resource_begin_movie(uint32 id);
void resource_end_movie(void);
uint32 resource_poll_frame(uint32 *frame, sint32 *auxiliary);
sint32 resource_read_progress(uint32 *output);
uint32 resource_get_stream_time(uint32 id);

void resource_asset_read_finished(void);

#endif
