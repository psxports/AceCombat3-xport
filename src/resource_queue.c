#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint8 kind;
    uint8 phase;
    uint16 id;

    union
    {
        uint32 argument;
        RESOURCE_STREAM_BUFFER destination;
    } payload;

    uint32 auxiliary;
    uint32 flags;
    const RESOURCE_AUDIO_TRACK *audio;
    RESOURCE_COMPLETION completion;
} RESOURCE_REQUEST;

typedef struct
{
    uint32 consumed;
    uint32 queued;
    RESOURCE_REQUEST requests[16];
    uint32 priority_active;
    RESOURCE_REQUEST priority;
    uint32 mono;
    uint32 volume;
    uint32 read_complete;
    uint32 read_result;
    uint32 fade_start;
    uint32 current_id;
    uint32 paused;
    uint32 read_begin;
    uint32 read_end;
} RESOURCE_QUEUE;

static RESOURCE_QUEUE resource_queue;

typedef struct
{
    uint32 sequence;
    uint32 queued;
} RESOURCE_COMPLETION_STATE;

static RESOURCE_COMPLETION_STATE completion_state;

uint32 resource_queue_next_sequence(void)
{
    return resource_queue.queued;
}

uint32 resource_queue_is_completing(uint32 sequence)
{
    return completion_state.queued && completion_state.sequence == sequence;
}

static uint32 resource_poll_request(RESOURCE_REQUEST *request, uint32 sequence, uint32 queued);
static uint32 resource_empty_request(RESOURCE_REQUEST *request);
static uint32 resource_stop_request(RESOURCE_REQUEST *request);
static uint32 resource_read_request(RESOURCE_REQUEST *request);
static uint32 resource_audio_request(RESOURCE_REQUEST *request);
static uint32 resource_stream_request(RESOURCE_REQUEST *request);
static uint32 resource_fade_request(RESOURCE_REQUEST *request);
static uint32 resource_pause_request(RESOURCE_REQUEST *request);
static void resource_apply_queue_mix(RESOURCE_QUEUE *audio_state);
static uint32 resource_query_stream_time(uint32 id, RESOURCE_QUEUE *context);

void resource_queue_reset(void)
{
    memset(&resource_queue, 0, sizeof(resource_queue));
    memset(&completion_state, 0, sizeof(completion_state));
}

RESOURCE_QUEUE_STATUS resource_queue_status(void)
{
    RESOURCE_QUEUE_STATUS status;
    status.consumed = resource_queue.consumed;
    status.queued = resource_queue.queued;
    status.priority_active = resource_queue.priority_active;
    status.current_id = resource_queue.current_id;
    return status;
}

void resource_init(void)
{
    RESOURCE_QUEUE *state;

    FUNCTION_MARKER(0x8002BCD8, "ACE3.BIN");
    resource_init_controller(&game_runtime.field_counter);
    state = &resource_queue;
    state->volume = 255;
    state->consumed = 0;
    state->queued = 0;
    state->priority_active = 0;
    state->mono = 0;
    state->read_complete = 1;
    state->read_result = 0;
    state->paused = 0;
}

void resource_init_queue(uint32 begin, RESOURCE_COMPLETION callback)
{
    uint32 i;
    RESOURCE_QUEUE *context;
    RESOURCE_QUEUE *result;

    FUNCTION_MARKER(0x8002C6A8, "ACE3.BIN");
    for (i = 0; i < 16; ++i)
        resource_queue.requests[i].kind = 0;
    context = &resource_queue;
    context->priority.kind = 1;
    result = &resource_queue;
    context->priority_active = 1;
    result->priority.payload.argument = begin;
    result->priority.completion = callback;
    result->priority.phase = 0;
}

void resource_pause_stream(uint32 argument)
{
    RESOURCE_QUEUE *second;
    RESOURCE_QUEUE *first;
    FUNCTION_MARKER(0x8002C6F4, "ACE3.BIN");
    first = &resource_queue;
    first->priority.kind = 6;
    second = &resource_queue;
    first->priority_active = 1;
    second->priority.payload.argument = argument;
    second->priority.completion = 0;
    second->priority.phase = 0;
    return;
}

void resource_cancel_streams(void)
{
    RESOURCE_QUEUE *context;
    RESOURCE_QUEUE *other;
    uint32 sequence;
    RESOURCE_REQUEST *request;
    uint32 kind;
    uint32 end;

    FUNCTION_MARKER(0x8002C71C, "ACE3.BIN");
    context = &resource_queue;
    context->priority.kind = 5;
    other = &resource_queue;
    context->priority_active = 1;
    other->priority.phase = 0;
    context = &resource_queue;
    other->priority.completion = 0;
    sequence = context->consumed;
    end = context->queued;
    while ((sint32)end >= (sint32)sequence)
    {
        request = context->requests + (sequence & 15);
        kind = request->kind;
        if (kind - 3 < 2)
            request->kind = 0;
        context = &resource_queue;
        end = context->queued;
        ++sequence;
    }
    {
        (void)(uint32)(sequence & 15);
        return;
    }
}

uint32 resource_queue_read(uint32 id, uint32 dst, RESOURCE_COMPLETION completion, RESOURCE_READ_BUFFER buffer)
{
    RESOURCE_QUEUE *state;
    uint32 count;
    RESOURCE_REQUEST *request;
    uint32 size;
    uint32 end;

    FUNCTION_MARKER(0x8002C7AC, "ACE3.BIN");
    state = &resource_queue;
    count = state->queued;
    state->read_begin = dst;
    state->queued = count + 1;
    request = state->requests + (count & 15);
    request->kind = 2;
    request->id = id;
    request->payload.destination = (RESOURCE_STREAM_BUFFER){buffer, dst};
    request->completion = completion;
    request->phase = 0;
    size = resource_get_size(id);
    end = dst + (size & 0xFFFFFFFC);
    state = &resource_queue;
    state->read_end = end;
    return end;
}

uint32 resource_queue_asset(RESOURCE_ASSET_SLOT *slot, RESOURCE_STREAM_BUFFER destination, RESOURCE_COMPLETION completion)
{
    uint32 resource_id = slot->resource_id;
    uint32 sequence = resource_queue.queued;
    uint32 dst = destination.encoded_begin;
    uint32 end, bytes;
    RESOURCE_READ_BUFFER buffer;

    if (!dst)
        abort();
    bytes = resource_get_size(resource_id) & 0xFFFFFFFC;
    if (!destination.buffer.data || bytes > destination.buffer.bytes)
        abort();
    buffer = (RESOURCE_READ_BUFFER){destination.buffer.data, bytes};
    end = resource_queue_read(resource_id, dst, completion, buffer);

    slot->request_sequence = sequence;
    slot->encoded_base = dst;
    slot->read_bytes = end - dst;
    slot->data = resource_queue.requests[sequence & 15].payload.destination.buffer.data;
    slot->backing = destination;
    return end;
}

uint32 resource_queue_audio(const RESOURCE_AUDIO_TRACK *descriptor, uint32 enabled, RESOURCE_COMPLETION callback)
{
    RESOURCE_QUEUE *context;
    uint32 sequence;
    RESOURCE_REQUEST *request;

    FUNCTION_MARKER(0x8002C8D8, "ACE3.BIN");
    context = &resource_queue;
    sequence = context->queued;
    context->queued = sequence + 1;
    request = context->requests + (sequence & 15);
    request->kind = 3;
    request->id = 0;
    request->audio = descriptor;
    request->auxiliary = enabled;
    request->completion = callback;
    request->phase = 0;
    return 3;
}

void resource_queue_stream(uint32 id, RESOURCE_STREAM_BUFFER destination, uint32 flags, RESOURCE_COMPLETION callback)
{
    RESOURCE_QUEUE *context;
    uint32 sequence;
    RESOURCE_REQUEST *request;

    FUNCTION_MARKER(0x8002C928, "ACE3.BIN");
    context = &resource_queue;
    sequence = context->queued;
    context->queued = sequence + 1;
    request = context->requests + (sequence & 15);
    request->kind = 4;
    request->id = id;
    request->payload.destination = destination;
    request->auxiliary = destination.buffer.bytes;
    request->flags = flags;
    request->phase = 0;
    request->completion = callback;
    return;
}

uint32 resource_wait(uint32 mode)
{
    RESOURCE_QUEUE *state;
    uint32 written;
    uint32 read;

    FUNCTION_MARKER(0x8002C580, "ACE3.BIN");
    if (mode)
    {
        state = &resource_queue;
        written = state->queued;
        read = state->consumed;
        return written - read;
    }
    for (;;)
    {
        state = &resource_queue;
        written = state->queued;
        read = state->consumed;
        if (written == read)
            return 0;
        resource_poll();
        VSync(0);
    }
}

void resource_poll(void)
{
    RESOURCE_QUEUE *state;
    uint32 read, written, result;
    FUNCTION_MARKER(0x8002C4DC, "ACE3.BIN");
    resource_poll_cd();
    state = &resource_queue;
    if (state->priority_active)
    {
        result = resource_poll_request(&state->priority, 0, 0);
        if (result)
        {
            resource_queue.priority_active = 0;
        }
        return;
    }
    read = state->consumed;
    written = state->queued;
    result = read & 15;
    if ((sint32)read < (sint32)written)
    {
        result = resource_poll_request(state->requests + result, read, 1);
        if (result)
        {
            state = &resource_queue;
            result = state->consumed + 1;
            state->consumed = result;
        }
    }
    return;
}

static uint32 resource_poll_request(RESOURCE_REQUEST *request, uint32 sequence, uint32 queued)
{
    static uint32 (*const handlers[7])(RESOURCE_REQUEST *) = {resource_empty_request, resource_stop_request, resource_read_request, resource_audio_request, resource_stream_request, resource_fade_request, resource_pause_request};
    uint32 kind, result;
    RESOURCE_COMPLETION completion;
    RESOURCE_COMPLETION_STATE previous;
    FUNCTION_MARKER(0x8002C45C, "ACE3.BIN");
    kind = request->kind;
    if (kind >= 7)
        abort();
    result = handlers[kind](request);
    if (result)
    {
        completion = request->completion;
        if (completion)
        {
            previous = completion_state;
            completion_state = (RESOURCE_COMPLETION_STATE){sequence, queued};
            completion();
            completion_state = previous;
        }
    }
    return result;
}

static uint32 resource_empty_request(RESOURCE_REQUEST *request)
{
    FUNCTION_MARKER(0x8002BDC4, "ACE3.BIN");
    return 1;
}

static uint32 resource_stop_request(RESOURCE_REQUEST *request)
{
    RESOURCE_QUEUE *context;
    uint32 result;

    FUNCTION_MARKER(0x8002BDCC, "ACE3.BIN");
    context = &resource_queue;
    context->current_id = 0xFFFFFFFF;
    result = resource_stop_cd(request->payload.argument);
    if (result)
        resource_queue.paused = 0;
    return result;
}

static uint32 resource_read_request(RESOURCE_REQUEST *request)
{
    uint32 phase;
    RESOURCE_QUEUE *state;
    const RESOURCE_ENTRY *entry;
    sint32 id;
    sint32 result;

    FUNCTION_MARKER(0x8002BE14, "ACE3.BIN");
    phase = request->phase;
    if (!phase)
    {
        state = &resource_queue;
        id = (sint16)request->id;
        state->current_id = id;
        state->read_complete = 0;
        entry = resource_get_entry((sint16)request->id);
        result = resource_begin_read((RESOURCE_READ_INPUT){entry, request->payload.destination, 0, 0}, 0);
        if (result == 1)
            request->phase = 1;
        return 0;
    }
    if (phase == 1)
    {
        state = &resource_queue;
        if (state->read_complete)
        {
            if (state->read_result != 0xFFFFFFFE)
            {
                state->current_id = 0xFFFFFFFF;
                return 1;
            }
            request->phase = 0;
        }
    }
    return 0;
}

static uint32 resource_stream_request(RESOURCE_REQUEST *request)
{
    uint32 phase;
    RESOURCE_QUEUE *context;
    uint32 flags;
    const RESOURCE_ENTRY *entry;
    uint32 result;
    uint32 id;

    FUNCTION_MARKER(0x8002C1D4, "ACE3.BIN");
    if (game_movie_matches((uint32)(sint16)request->id))
    {
        if (!request->phase)
        {
#if !AP_WEB
            context = &resource_queue;
            flags = context->mono;
            context->volume = 255;
            resource_set_cd_volume(255, flags);
#endif
            flags = request->flags;
            if (!(request->id & 0x1000))
                flags &= ~0x1000u;
            game_movie_request_begin(flags);
            context = &resource_queue;
            context->current_id = (uint32)(sint16)request->id;
            context->read_complete = 0;
            request->phase = 1;
        }
        if (!game_movie_request_complete())
            return 0;
        resource_read_complete(0);
        context = &resource_queue;
        context->current_id = 0xFFFFFFFF;
        return 1;
    }
    phase = request->phase;
    if (!phase)
    {
        context = &resource_queue;
        flags = context->mono;
        context->volume = 255;
        resource_set_cd_volume(255, flags);
        id = (uint32)(sint16)request->id;
        flags = request->flags;
        if (~id & 0x1000)
            flags &= 0xFFFFEFFF;
        context = &resource_queue;
        context->read_complete = 0;
        id = (uint32)(sint16)request->id;
        context->current_id = id;
        entry = resource_get_entry((uint32)(sint16)request->id);
        result = (uint32)resource_begin_read((RESOURCE_READ_INPUT){entry, request->payload.destination, request->auxiliary, 0}, flags);
        if (result == 1)
            request->phase = 1;
        return 0;
    }
    if (phase != 1)
        return 0;
    context = &resource_queue;
    if (!context->read_complete)
        return 0;
    if (context->read_result == 0xFFFFFFFE)
    {
        request->phase = 0;
        return 0;
    }
    context->current_id = 0xFFFFFFFF;
    return 1;
}

static uint32 resource_audio_request(RESOURCE_REQUEST *request)
{
    RESOURCE_QUEUE *context;
    const RESOURCE_AUDIO_TRACK *descriptor;
    const RESOURCE_ENTRY *entry;
    RESOURCE_ENTRY entry_copy;
    static RESOURCE_SECTOR_HEADER empty_buffer;
    RESOURCE_READ_BUFFER buffer = {(uint8 *)&empty_buffer, 0};
    uint32 phase;
    uint32 id;
    uint32 flags;
    uint32 value;
    uint32 elapsed;
    uint32 numerator;
    uint32 shift;
    sint32 offset;
    sint32 duration;
    sint32 volume;

    FUNCTION_MARKER(0x8002BEE4, "ACE3.BIN");
    context = &resource_queue;
    if (context->paused)
        return 0;
    phase = request->phase;
    descriptor = request->audio;
    switch (phase)
    {
        case 0:
            if (!descriptor)
                abort();
            context->volume = descriptor->fade ? 0 : 255;
            context = &resource_queue;
            resource_set_cd_volume((sint32)context->volume, context->mono);
            value = request->auxiliary;
            id = (uint32)descriptor->id;
            if (!value)
                id &= 0xFFFFEFFF;
            context = &resource_queue;
            context->current_id = id;
            entry = resource_get_entry(id);
            entry_copy = *entry;
            offset = descriptor->sector_offset;
            entry_copy.sector += (uint32)offset;
            shift = entry->flags >> 24;
            offset = descriptor->sector_offset;
            value = (entry_copy.flags & 0x7FFFF) - (uint32)(offset >> (shift & 7));
            entry_copy.flags = (entry_copy.flags & 0xFFF80000) | (value & 0x7FFFF);
            context = &resource_queue;
            context->read_complete = 0;
            if (resource_begin_read((RESOURCE_READ_INPUT){&entry_copy, {buffer, 0}, 0, 1}, id & 0x1000) != 1)
                return 0;
            if (!descriptor->fade)
            {
                request->phase = 2;
                return 0;
            }
            request->phase = 1;
            context = &resource_queue;
            context->fade_start = game_runtime.field_counter;
            return 0;
        case 1:
            if (resource_get_stream_flags() & 0x01000000)
            {
                context = &resource_queue;
                elapsed = game_runtime.field_counter - context->fade_start;
                if (!descriptor)
                    abort();
                duration = descriptor->duration;
                numerator = elapsed * 255;
                if (duration)
                {
                    if (duration == -1 && numerator == 0x80000000)
                        trap(6);
                    volume = (sint32)numerator / duration;
                }
                else
                    volume = (sint32)numerator >> 7;
                if (volume < 0)
                    volume = 0;
                if (volume >= 256)
                {
                    volume = 255;
                    request->phase = 2;
                }
                context = &resource_queue;
                flags = context->mono;
                context->volume = (uint32)volume;
                resource_set_cd_volume(volume, flags);
            }
            else
            {
                context = &resource_queue;
                context->fade_start = game_runtime.field_counter;
            }
            if (resource_queue.read_complete)
                request->phase = 2;
            return 0;
        case 2:
            if (!context->read_complete)
                return 0;
            if (context->read_result == 0xFFFFFFFE)
            {
                request->phase = 0;
                return 0;
            }
            if (!descriptor)
                abort();
            if (!descriptor->next)
            {
                context->current_id = 0xFFFFFFFF;
                return 1;
            }
            request->phase = 0;
            request->audio = descriptor->next;
            return 0;
        default:
            return 0;
    }
}

static uint32 resource_fade_request(RESOURCE_REQUEST *request)
{
    RESOURCE_QUEUE *context;
    uint32 phase;
    uint32 elapsed;
    uint32 value;
    uint32 result;
    sint32 remaining;
    sint32 volume;

    FUNCTION_MARKER(0x8002C2E4, "ACE3.BIN");
    context = &resource_queue;
    if (context->paused)
        return 0;
    phase = request->phase;
    if (phase == 2)
    {
        result = resource_stop_cd(0);
        if (result)
            resource_queue.paused = 0;
        return result;
    }
    if (phase > 1)
        return 0;
    if (!phase)
    {
        value = game_runtime.field_counter;
        context->fade_start = value;
        request->phase = 1;
    }
    value = game_runtime.field_counter;
    context = &resource_queue;
    elapsed = value - context->fade_start;
    remaining = 4096 - ((sint32)(elapsed << 12) >> 6);
    if (remaining > 4096)
        remaining = 4096;
    if (remaining < 0)
        remaining = 0;
    value = context->volume;
    volume = (sint32)(value * (uint32)remaining) >> 12;
    if (volume < 5)
    {
        request->phase = 2;
        volume = 0;
        resource_queue.volume = 0;
    }
    resource_set_cd_volume(volume, resource_queue.mono);
    return 0;
}

static uint32 resource_pause_request(RESOURCE_REQUEST *request)
{
    uint32 value;
    RESOURCE_QUEUE *state;
    FUNCTION_MARKER(0x8002C41C, "ACE3.BIN");
    value = request->payload.argument;
    resource_set_stream_paused(value);
    state = &resource_queue;
    value = request->payload.argument;
    state->paused = value;
    return 1;
}

uint32 resource_read_complete(uint32 result)
{
    RESOURCE_QUEUE *state;

    FUNCTION_MARKER(0x8002BDB0, "ACE3.BIN");
    state = &resource_queue;
    state->read_complete = 1;
    state->read_result = result;
    return 1;
}

sint32 sound_get_cur_resource(void)
{
    FUNCTION_MARKER(0x8002CAD0, "ACE3.BIN");
    return (sint32)resource_queue.current_id;
}

void resource_set_mono(uint32 mode)
{
    RESOURCE_QUEUE *state;

    FUNCTION_MARKER(0x8002BD28, "ACE3.BIN");
    state = &resource_queue;
    state->mono = mode;
}

void resource_apply_mix(void)
{
    RESOURCE_QUEUE *audio_state;
    FUNCTION_MARKER(0x8002BD5C, "ACE3.BIN");
    audio_state = &resource_queue;
    {
        (void)(resource_apply_queue_mix(audio_state));
        return;
    }
}

static void resource_apply_queue_mix(RESOURCE_QUEUE *audio_state)
{
    uint32 volume, mono;
    FUNCTION_MARKER(0x8002BD60, "ACE3.BIN");
    volume = audio_state->volume;
    mono = audio_state->mono;
    {
        (void)(resource_set_cd_volume(volume, mono));
        return;
    }
}

uint32 resource_poll_frame(uint32 *frame, sint32 *auxiliary)
{
    RESOURCE_QUEUE *context;
    const RESOURCE_FRAME *entry;
    sint32 consumed;
    sint32 queued;

    FUNCTION_MARKER(0x8002C980, "ACE3.BIN");
    context = &resource_queue;
    consumed = (sint32)context->consumed;
    queued = (sint32)context->queued;
    if (consumed >= queued || context->priority_active)
        return 0;
    entry = resource_get_frame_metadata();
    if (!entry || !entry->kind)
        return 0;
    if (frame)
        *frame = entry->frame;
    if (auxiliary)
        *auxiliary = entry->auxiliary;
    return 1;
}

sint32 resource_read_progress(uint32 *output)
{
    RESOURCE_QUEUE *context;
    uint32 begin;
    uint32 end;
    uint32 current;
    uint32 total;
    uint32 completed;
    sint32 progress;

    FUNCTION_MARKER(0x8002CA2C, "ACE3.BIN");
    context = &resource_queue;
    begin = context->read_begin;
    end = context->read_end;
    current = resource_get_read_cursor();
    if (output)
        *output = current;
    completed = (current - begin) >> 2;
    total = (end - begin) >> 2;
    progress = total ? (sint32)((completed << 12) / total) : 0;
    if (progress < 0)
        progress = 0;
    if (progress > 4096)
        progress = 4096;
    return progress;
}

uint32 resource_get_stream_time(uint32 id)
{
    RESOURCE_QUEUE *context;

    FUNCTION_MARKER(0x8002CAE4, "ACE3.BIN");
    context = &resource_queue;
    return resource_query_stream_time(id, context);
}

static uint32 resource_query_stream_time(uint32 id, RESOURCE_QUEUE *context)
{
    uint32 consumed;
    uint32 queued;

    FUNCTION_MARKER(0x8002CAE8, "ACE3.BIN");
    if (context->current_id != id)
        return 0xFFFFFFFF;
    consumed = context->consumed;
    queued = context->queued;
    if ((sint32)consumed >= (sint32)queued || context->priority_active)
        return 0xFFFFFFFF;
    if ((resource_get_stream_flags() & 0x81000000) != 0x81000000)
        return 0xFFFFFFFF;
    return resource_get_stream_elapsed();
}

void resource_asset_read_finished(void)
{
    FUNCTION_MARKER(0x8009B904, "ACE3.BIN");
    if (resource_queue_is_completing(game_runtime.resource_slot.request_sequence))
        game_runtime.resource_pending = 0;
}

void resource_begin_movie(uint32 id)
{
    resource_init_queue(0, 0);
    resource_queue.current_id = (uint32)(sint32)(sint16)id;
    resource_queue.read_complete = 0;
    resource_queue.volume = 255;
    resource_set_cd_volume(255, resource_queue.mono);
}

void resource_end_movie(void)
{
    resource_read_complete(0);
    resource_queue.current_id = 0xFFFFFFFF;
    resource_init_queue(0, 0);
}
