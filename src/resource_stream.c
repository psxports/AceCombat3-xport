#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    RESOURCE_READ_BUFFER buffer;
    /* encoded origin retained only for progress */
    uint32 encoded_begin;
    uint32 byte_offset;
} RESOURCE_READ_CURSOR;

typedef enum
{
    RESOURCE_TRANSFER_NONE,
    RESOURCE_TRANSFER_READ,
    RESOURCE_TRANSFER_AUDIO
} RESOURCE_TRANSFER_KIND;

typedef struct
{
    uint32 flags;
    uint32 audio_poll_enabled;
    uint32 completion_enabled;
    RESOURCE_TRANSFER_KIND transfer_kind;
    uint32 start_sector;
    uint32 remaining;
    RESOURCE_READ_CURSOR destination;
    uint8 filter[2];
    uint32 phase;
    uint32 cur_sector;
    uint32 estimated_sector;
    uint32 end_sector;
    uint32 timeout;
    uint32 error;
    uint32 deadline;
    uint32 elapsed;
    uint32 last_clock;
    uint32 mdec_pending;
} RESOURCE_STREAM;

static RESOURCE_STREAM resource_stream;

void resource_stream_reset(void)
{
    memset(&resource_stream, 0, sizeof(resource_stream));
}

RESOURCE_STREAM_STATUS resource_stream_status(void)
{
    RESOURCE_STREAM_STATUS status;
    status.flags = resource_stream.flags;
    status.phase = resource_stream.phase;
    status.channel = resource_stream.filter[1];
    return status;
}

void resource_stream_select_channel(uint32 channel)
{
    resource_stream.filter[1] = (uint8)channel;
    resource_metadata.channels = 0;
}

void resource_stream_begin_file(uint32 flags, uint32 data_stream, uint32 remaining, uint32 end_sector)
{
    RESOURCE_STREAM *stream = &resource_stream;
    stream->flags = data_stream ? stream->flags | 0x80000010 : 0x80000410 | ((flags & 0x1000) ? 0x4000 : 0);
    stream->audio_poll_enabled = 0;
    stream->phase = 64;
    stream->remaining = remaining;
    stream->end_sector = end_sector;
    stream->elapsed = 0;
}

void resource_stream_file_position(uint32 sector, uint32 end_sector, uint32 frames)
{
    RESOURCE_STREAM *stream = &resource_stream;
    stream->flags = (stream->flags | 0x01000000) & ~0x10;
    stream->cur_sector = sector;
    stream->estimated_sector = sector;
    stream->remaining = sector < end_sector ? end_sector - sector : 0;
    stream->elapsed = (uint32)((uint64)frames * 60 / 44100);
}

void resource_stream_end_file(void)
{
    resource_stream.flags = 0;
    resource_stream.remaining = 0;
}

uint32 resource_stream_accepts_audio_sector(void)
{
    RESOURCE_STREAM *stream = &resource_stream;
    uint32 flags = stream->flags;
    return !(stream->transfer_kind != RESOURCE_TRANSFER_AUDIO || stream->phase != 64 ||
        (flags & 0xC0000080) != 0x80000000 || ((flags & 0x400) && !(flags & 0x4000)) ||
        resource_stream.mdec_pending);
}

intptr resource_poll_transfer(uint32 result)
{
    RESOURCE_CONTROLLER *state = &resource_controller;
    RESOURCE_STREAM *stream;
    uint32 audio_poll_enabled;
    uint32 flags, callback_result, timer;
    stream = &resource_stream;
    audio_poll_enabled = stream->audio_poll_enabled;
    callback_result = 0;
    if (audio_poll_enabled && stream->transfer_kind == RESOURCE_TRANSFER_AUDIO && (stream->flags & 0x80000080) == 0x80000000)
    {
        // Advance file-backed stream commands from their native completion
        resource_sync_callback(result, state->response);
        stream = &resource_stream;
        audio_poll_enabled = stream->audio_poll_enabled;
    }
    if (audio_poll_enabled)
        callback_result = cd_audio_poll(result);
    else if (stream->transfer_kind == RESOURCE_TRANSFER_READ && (stream->flags & 0x80000080) == 0x80000000)
    {
        if (result == CdlComplete)
            callback_result = resource_read_phase();
        else if (result == CdlDiskError)
        {
            state = &resource_controller;
            callback_result = resource_sync_callback(result, state->response);
        }
    }
    stream = &resource_stream;
    result = stream->flags & 0x10;
    if (!result || callback_result)
        return result;
    timer = stream->timeout;
    result = timer - 1;
    stream->timeout = result;
    if ((sint32)timer > 0)
        return result;
    flags = stream->flags;
    stream->phase = 255;
    stream->flags = (flags | 0x20) & 0xFFFFFFEF;
    return CdControlF(9, NULL);
}

sint32 resource_begin_read(RESOURCE_READ_INPUT input, uint32 flags)
{
    const RESOURCE_ENTRY *entry = input.entry;
    uint32 dst = input.destination.encoded_begin;
    uint32 aux = input.ring_bytes;
    RESOURCE_CONTROLLER *state;
    RESOURCE_STREAM *stream;
    RESOURCE_STREAM *other;
    uint32 word;
    uint32 mode;
    uint32 counter;
    uint32 units;
    uint32 start;
    uint32 stream_flags;
    uint32 sync;
    sint32 sectors;
    RESOURCE_READ_BUFFER buffer = input.destination.buffer;
    FUNCTION_MARKER(0x8002A79C, "ACE3.BIN");
    state = &resource_controller;
    if (state->phase)
        return -1;
    sync = game_use_extracted_assets() ? CdlComplete : CdSync(1, (uint8 *)state->response);
    if (sync != 2)
        return 0;
    if ((sint32)entry->flags >= 0)
    {
        resource_metadata.channels = 0;
        resource_read_complete(0xFFFFFFFD);
        return 1;
    }
    game_stop_resource_music();
    state = &resource_controller;
    stream = &resource_stream;
    state->wait_mdec = 0;
    stream->flags = 0;
    start = entry->sector;
    stream->elapsed = 0;
    stream->error = 0;
    stream->completion_enabled = 1;
    stream->start_sector = start;
    word = entry->flags;
    mode = (word >> 27) & 3;
    counter = *(state->clock);
    switch (mode)
    {
        case 0:
            stream->transfer_kind = RESOURCE_TRANSFER_READ;
            stream->audio_poll_enabled = 0;
            stream->destination.encoded_begin = dst;
            stream->destination.byte_offset = 0;
            if (!buffer.data)
                abort();
            stream->destination.buffer = buffer;
            stream->remaining = entry->flags & 0x7FFFF;
            units = entry->flags & 0x7FFFF;
            sectors = (sint32)units - 1;
            if (sectors < 0)
                sectors = (sint32)units + 510;
            start = stream->start_sector;
            stream->end_sector = start + (uint32)(sectors >> 9);
            if (game_use_extracted_assets())
            {
                sint32 loaded = 0;
                if (!input.adjusted_entry)
                {
                    void *destination;
                    if (!buffer.data || units * 4 > buffer.bytes)
                        abort();
                    destination = buffer.data;
                    loaded = game_read_resource_file(entry, destination, units * 4);
                }
                stream->destination.encoded_begin = dst;
                stream->destination.byte_offset = loaded ? units * 4 : 0;
                stream->destination.buffer = buffer;
                stream->remaining = 0;
                resource_metadata.channels = 0;
                if (!loaded)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Extracted BP read failed: entry=%p native=%p destination=%08X bytes=%u resource=%08X\n", (const void *)(input.adjusted_entry ? NULL : entry), (const void *)(input.adjusted_entry ? entry : NULL), dst, units * 4, (uint32)sound_get_cur_resource());
#endif
                    xport_message_error("Ace Combat 3 - Resource error", "Cannot read extracted ACE_BPB resource");
                    abort();
                }
                resource_read_complete(0);
                return 1;
            }
            break;
        case 1:
            stream->transfer_kind = RESOURCE_TRANSFER_AUDIO;
            stream_flags = stream->flags;
            stream->audio_poll_enabled = 1;
            stream->destination.encoded_begin = dst;
            stream->destination.byte_offset = 0;
            stream->destination.buffer = (RESOURCE_READ_BUFFER){NULL, 0};
            stream->filter[0] = 1;
            stream->flags = stream_flags | 0x2000;
            word = entry->flags;
            other = &resource_stream;
            other->filter[1] = (word >> 19) & 31;
            word = entry->flags;
            stream = &resource_stream;
            start = stream->start_sector;
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            stream->remaining = units;
            stream->end_sector = start + units - 1;
            if (flags & 0x800)
                resource_controller.wait_mdec = 1;
            if ((dst & 3) || (sint32)aux / ((flags & 0x2000) ? 0x918 : 0x800) < 0)
                abort();
            if (!buffer.data || aux > buffer.bytes)
                abort();
            resource_init_ring(buffer.data, aux, (flags & 0x2000) != 0);
            resource_reset_ring();
            stream = &resource_stream;
            units = stream->remaining;
            stream->deadline = counter + 2;
            stream->deadline = counter + 2 + (uint32)((sint32)(units * 6) / 15);
            break;
        case 2:
            if (game_use_extracted_assets() && game_start_resource_music(start, flags, 0))
                return 1;
            resource_set_cd_muted(1);
            if (flags & 0x1000)
            {
                stream = &resource_stream;
                stream->flags = stream->flags | 0x4000;
            }
            stream = &resource_stream;
            stream->transfer_kind = RESOURCE_TRANSFER_AUDIO;
            stream_flags = stream->flags;
            stream->audio_poll_enabled = 1;
            stream->filter[0] = 1;
            stream->flags = stream_flags | 0x400;
            word = entry->flags;
            other = &resource_stream;
            other->filter[1] = (word >> 19) & 31;
            word = entry->flags;
            stream = &resource_stream;
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            stream->remaining = units;
            start = stream->start_sector;
            stream->deadline = counter + 2;
            stream->end_sector = start + units - 1;
            stream->deadline = counter + 2 + (uint32)((sint32)(units * 6) / 15);
            break;
        case 3:
            resource_set_cd_muted(1);
            if (flags & 0x1000)
            {
                stream = &resource_stream;
                stream->flags = stream->flags | 0x4000;
            }
            else if (flags & 0x4000)
            {
                stream = &resource_stream;
                stream->flags = stream->flags | 0x8000;
            }
            if (flags & 0x800)
                resource_controller.wait_mdec = 1;
            stream = &resource_stream;
            stream->transfer_kind = RESOURCE_TRANSFER_AUDIO;
            stream->audio_poll_enabled = 1;
            stream_flags = stream->flags;
            stream->filter[0] = 1;
            other = &resource_stream;
            stream->destination.encoded_begin = dst;
            stream->destination.byte_offset = 0;
            stream->destination.buffer = (RESOURCE_READ_BUFFER){NULL, 0};
            stream->flags = stream_flags | 0x2000;
            word = entry->flags;
            other->filter[1] = (word >> 19) & 31;
            word = entry->flags;
            stream = &resource_stream;
            start = stream->start_sector;
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            stream->remaining = units;
            stream->end_sector = start + units - 1;
            if ((dst & 3) || (sint32)aux / ((flags & 0x2000) ? 0x918 : 0x800) < 0)
                abort();
            if (!buffer.data || aux > buffer.bytes)
                abort();
            resource_init_ring(buffer.data, aux, (flags & 0x2000) != 0);
            resource_reset_ring();
            stream = &resource_stream;
            units = stream->remaining;
            stream->deadline = counter + 2;
            stream->deadline = counter + 2 + (uint32)((sint32)(units * 6) / 15);
            break;
        default:
            return -1;
    }
    if (mode == 3 && game_use_extracted_assets() && game_start_resource_music(start, flags, 1))
        return 1;
    resource_issue_seek();
    return 1;
}

uint32 resource_issue_seek(void)
{
    CdlLOC position;
    RESOURCE_STREAM *stream;
    uint32 sector;
    uint32 flags;
    RESOURCE_CONTROLLER *state;

    FUNCTION_MARKER(0x80029DFC, "ACE3.BIN");
    stream = &resource_stream;
    sector = stream->start_sector;
    flags = stream->flags;
    resource_stream.mdec_pending = 0;
    stream->phase = 0;
    stream->flags = (flags | 0x80000000) & 0xFCFFEFEF;
    CdIntToPos((sint32)sector, &position);
    if (stream->transfer_kind == RESOURCE_TRANSFER_READ || stream->transfer_kind == RESOURCE_TRANSFER_AUDIO)
        CdSyncCallback(0);
    else
        CdSyncCallback(game_cd_resource_sync);
    state = &resource_controller;
    state->response_words[0] = 0;
    state->response_words[1] = 0;
    resource_metadata.channels = 0;
    if (stream->transfer_kind == RESOURCE_TRANSFER_READ || stream->transfer_kind == RESOURCE_TRANSFER_AUDIO)
    {
        if (!CdControlB(CdlPause, NULL, NULL))
            return 0;
        return CdControlB(2, (uint8 *)&position, NULL);
    }
    return CdControlF(2, (uint8 *)((const uint8 *)&position));
}

uint32 resource_sync_callback(uint32 event, const uint8 *response)
{
    RESOURCE_CONTROLLER *state;
    RESOURCE_STREAM *stream;
    uint32 flags;
    uint32 last;
    RESOURCE_TRANSFER_KIND transfer_kind;

    FUNCTION_MARKER(0x8002A200, "ACE3.BIN");
    state = &resource_controller;
    memmove(&state->response_words[0], response, sizeof(state->response_words[0]));
    memmove(&state->response_words[1], response + 4, sizeof(state->response_words[1]));
    event &= 255;
    if (event == 5)
    {
        stream = &resource_stream;
        if (stream->flags & 0x80)
            return 255;
        stream->phase = 255;
        stream->error = 1;
        flags = stream->flags;
        last = stream->cur_sector;
        stream->flags = (flags | 0x20) & 0xFFFFFFEF;
        if (last)
        {
            stream->start_sector = last + 1;
            stream->cur_sector = 0;
        }
    }
    else if (event != 2)
        return 2;
    stream = &resource_stream;
    transfer_kind = stream->transfer_kind;
    if (transfer_kind == RESOURCE_TRANSFER_READ)
        return resource_read_phase();
    if (transfer_kind == RESOURCE_TRANSFER_AUDIO)
        return cd_audio_step();
    return 0;
}

uint32 resource_read_phase(void)
{
    uint8 mode;
    RESOURCE_STREAM *stream;
    uint32 phase;
    uint32 flags;
    uint32 completion_enabled;
    uint32 result;
    sint32 remaining;

    FUNCTION_MARKER(0x80029E74, "ACE3.BIN");
    stream = &resource_stream;
    phase = stream->phase;
    switch (phase)
    {
        case 0:
            mode = 0xA0;
            stream->phase = 1;
            return CdControlB(CdlSetmode, &mode, NULL);
        case 1:
            stream->cur_sector = 0;
            game_cd_ready_callback(game_cd_resource_ready);
            stream = &resource_stream;
            stream->phase = 2;
            resource_read_phase();
            return game_cd_read_resource_sector_sync();
        case 2:
            stream->timeout = 300;
            flags = stream->flags;
            stream->phase = 3;
            result = flags | 0x10;
            stream->flags = result;
            return result;
        case 3:
            if ((sint32)stream->remaining > 0)
            {
                do
                {
                    remaining = (sint32)stream->remaining;
                    game_cd_read_resource_sector_sync();
                    stream = &resource_stream;
                    if (remaining <= 512 || stream->phase != 3)
                        break;
                } while ((sint32)stream->remaining > 0);
                return 0x7EFF0000;
            }
            flags = stream->flags;
            stream->flags = flags & 0x7EFFFFFF;
            CdSyncCallback(0);
            stream = &resource_stream;
            completion_enabled = stream->completion_enabled;
            if (completion_enabled)
                return resource_read_complete(0);
            return 0;
        default:
            stream = &resource_stream;
            flags = stream->flags;
            if (flags & 0x20)
            {
                stream->flags = flags & 0xFFFFFFDF;
                return resource_issue_seek();
            }
            stream->flags = flags | 0x80;
            CdSyncCallback(0);
            stream = &resource_stream;
            completion_enabled = stream->completion_enabled;
            if (completion_enabled)
                return resource_read_complete(0xFFFFFFFE);
            return 0;
    }
}

uint32 resource_sector_ready(uint32 event, const uint8 *response, const RESOURCE_CD_SECTOR *native_sector)
{
    uint32 sector_header[4];
    RESOURCE_CONTROLLER *state;
    RESOURCE_STREAM *stream;
    uint32 flags;
    uint32 last;
    uint32 sector;
    uint32 dst;
    uint32 offset;
    size_t bytes;
    RESOURCE_READ_BUFFER buffer;
    void *destination;
    uint32 result = 1;
    uint32 error = 0;
    uint32 left;
    sint32 words = 512;

    FUNCTION_MARKER(0x80029A44, "ACE3.BIN");
    state = &resource_controller;
    memmove(&state->response_words[0], response, sizeof(state->response_words[0]));
    memmove(&state->response_words[1], response + 4, sizeof(state->response_words[1]));
    if ((event & 255) == 1)
    {
        stream = &resource_stream;
        flags = stream->flags | 0x01000000;
        stream->flags = flags;
        if (flags & 0x10)
            stream->flags = flags & 0xFFFFFFEF;
        if (native_sector)
            memcpy(sector_header, native_sector->bytes.header, sizeof(native_sector->bytes.header));
        else
            cd_get_sector(sector_header, 3);
        sector = CdPosToInt((CdlLOC *)sector_header);
        stream = &resource_stream;
        last = stream->cur_sector;
        if (last)
        {
            if (sector != last + 1)
                error = 4;
        }
        else if (sector != stream->start_sector)
            error = 3;
        stream = &resource_stream;
        left = stream->remaining;
        stream->cur_sector = sector;
        if ((sint32)left < words)
            words = left;
        dst = stream->destination.encoded_begin + stream->destination.byte_offset;
        buffer = stream->destination.buffer;
        offset = stream->destination.byte_offset;
        bytes = (size_t)(uint32)words * 4;
        if (!buffer.data || offset > buffer.bytes || bytes > buffer.bytes - offset)
            abort();
        destination = buffer.data + offset;
        if (native_sector)
        {
            if (destination && (uint32)words <= 512)
                memcpy(destination, native_sector->bytes.payload, (uint32)words * 4);
        }
        else
            cd_get_sector(destination, (uint32)words);
        stream = &resource_stream;
        dst = stream->destination.encoded_begin + stream->destination.byte_offset;
        left = stream->remaining;
        result = dst + (uint32)words * 4;
        left -= (uint32)words;
        stream->destination.byte_offset += (uint32)words * 4;
        stream->remaining = left;
        if ((sint32)left <= 0)
        {
            game_cd_ready_callback(0);
            result = CdControlF(9, NULL);
        }
    }
    else
        error = 2;
    if (error)
    {
        stream = &resource_stream;
        flags = stream->flags;
        last = stream->cur_sector;
        stream->phase = 255;
        stream->error = error;
        stream->flags = (flags | 0x20) & 0xFFFFFFEF;
        if (last)
        {
            stream->start_sector = last + 1;
            stream->cur_sector = 0;
        }
        game_cd_ready_callback(0);
        return CdControlF(9, NULL);
    }
    return result;
}

uint32 cd_audio_step(void)
{
    RESOURCE_STREAM *context;
    uint32 phase;
    uint32 flags;
    uint32 completion_enabled;
    uint8 parameters[1];

    FUNCTION_MARKER(0x80029FE4, "ACE3.BIN");
    context = &resource_stream;
    phase = context->phase;
    switch (phase)
    {
        case 0:
            context->phase = 16;
            return CdControlB(13, context->filter, NULL);
        case 16:
            parameters[0] = 0xE8;
            flags = context->flags;
            context->phase = 32;
            context->flags = flags | 0x1000;
            return CdControlB(14, parameters, NULL);
        case 32:
            context->cur_sector = 0;
            game_cd_ready_callback(game_cd_audio_ready);
            context = &resource_stream;
            context->phase = 48;
            return CdControlB(27, NULL, NULL);
        case 48:
            resource_set_cd_muted(0);
            context = &resource_stream;
            context->timeout = 300;
            flags = context->flags;
            context->phase = 64;
            flags = (flags & 0xBFFFFFFF) | 0x10;
            context->flags = flags;
            return flags;
        case 64:
            if ((sint32)context->remaining <= 0)
            {
                context->flags = context->flags & 0x7EFFFFFF;
                CdSyncCallback(0);
                context = &resource_stream;
                context->audio_poll_enabled = 0;
                resource_metadata.channels = 0;
                completion_enabled = context->completion_enabled;
                return completion_enabled ? resource_read_complete(0) : 0;
            }
            flags = context->flags;
            if (!(flags & 0x40000000))
                return 0;
            if (flags & 0x02000000)
            {
                context->flags = flags & 0xFCFFFFFF;
                context->phase = 48;
                return CdControlB(27, NULL, NULL);
            }
            flags |= 0x02000000;
            context->flags = flags;
            return flags;
        default:
            context = &resource_stream;
            flags = context->flags;
            if (flags & 0x20)
            {
                context->flags = flags & 0xFFFFFFDF;
                return resource_issue_seek();
            }
            context->flags = flags | 0x80;
            CdSyncCallback(0);
            context = &resource_stream;
            context->audio_poll_enabled = 0;
            resource_metadata.channels = 0;
            completion_enabled = context->completion_enabled;
            return completion_enabled ? resource_read_complete(0xFFFFFFFE) : 0;
    }
}

uint32 cd_audio_poll(uint32 event)
{
    RESOURCE_STREAM *context;
    uint32 flags;
    RESOURCE_CONTROLLER *other;
    uint32 clock;
    uint32 delta;
    uint32 value;
    uint32 completion_enabled;
    uint32 stop = 0;

    FUNCTION_MARKER(0x8002A498, "ACE3.BIN");
    context = &resource_stream;
    // Music progress follows sectors consumed by the shared XA mixer
    if ((context->flags & 0xC0004400) == 0x80000400 && context->phase == 64)
        cd_audio_sector();
    context = &resource_stream;
    flags = context->flags;
    if (flags & 0x01000000)
    {
        if (!(flags & 0x40000000))
        {
            clock = *(resource_controller.clock);
            delta = clock - context->last_clock;
            value = delta * 5;
            context->estimated_sector = context->cur_sector + (uint32)((sint32)value >> 1);
        }
        context = &resource_stream;
        flags = context->flags;
        if (flags & 0x10)
        {
            context->flags = flags & 0xFFFFFFEF;
            context = &resource_stream;
        }
        if ((sint32)context->remaining <= 0 || (sint32)*(resource_controller.clock) >= (sint32)context->deadline)
        {
            if (event == 2 && !(context->flags & 0x40000000))
            {
                resource_set_cd_muted(1);
                context = &resource_stream;
                context->remaining = 0;
                game_cd_ready_callback(0);
                CdControlF(9, NULL);
                return 1;
            }
            context = &resource_stream;
        }
        flags = context->flags;
        if ((flags & 0x40) && event == 2)
        {
            flags &= 0xFFFFFFBF;
            context->flags = flags;
            if (!(flags & 0x40000000))
                context->flags = flags | 0x40000000;
            CdControlF(9, NULL);
            return 1;
        }
        context = &resource_stream;
        if (!(context->flags & 0x40000000))
        {
            delta = context->cur_sector - context->start_sector;
            value = ((sint32)(delta * 6) / 15);
            clock = *(resource_controller.clock);
            context->elapsed = value + clock - context->last_clock;
        }
        return 0;
    }
    if ((sint32)flags < 0)
    {
        if (!(flags & 0x40000000))
            stop = (sint32)*(resource_controller.clock) >= (sint32)context->deadline;
        if ((~flags & 0x40000010) == 0x40000010)
        {
            value = context->timeout - 1;
            other = &resource_controller;
            context->timeout = value;
            if (!(other->response[0] & 2) || (sint32)value <= 0)
                stop = 1;
        }
    }
    if (!stop)
        return 0;
    resource_set_cd_muted(1);
    context = &resource_stream;
    context->remaining = 0;
    game_cd_ready_callback(0);
    context = &resource_stream;
    context->flags = context->flags & 0x7FFFFFFF;
    CdSyncCallback(0);
    context = &resource_stream;
    context->audio_poll_enabled = 0;
    resource_metadata.channels = 0;
    completion_enabled = context->completion_enabled;
    if (completion_enabled)
        resource_read_complete(0xFFFFFFFF);
    other = &resource_controller;
    if (!CdControl(9, NULL, (uint8 *)other->response))
        CdReset(0);
    return 1;
}

void cd_audio_sector(void)
{
    uint8 header[12];
    RESOURCE_STREAM *context;
    uint32 flags;
    uint32 channel = 256;
    uint32 position;
    uint32 remaining;
    uint32 clock;
    uint32 result;

    FUNCTION_MARKER(0x80029BD8, "ACE3.BIN");
    context = &resource_stream;
    if ((context->flags & 0x4400) == 0x400)
    {
        if (!CdControlBPSX(16, NULL, header))
            return;
    }
    else
        cd_get_sector(header, 3);
    flags = context->flags | 0x01000000;
    context->flags = flags;
    if (flags & 0x10)
        context->flags = flags & 0xFFFFFFEF;
    result = ~header[6] & 8;
    if (result)
        { (void)(uint32)(result); return; }
    position = (uint32)CdPosToInt((CdlLOC *)header);
    context = &resource_stream;
    if (context->flags & 0x1000)
        channel = header[5];
    remaining = context->end_sector - position;
    context->remaining = remaining;
    context->cur_sector = position;
    clock = *(resource_controller.clock);
    context->last_clock = clock;
    context->deadline = clock + 2;
    context->deadline = clock + 2 + (uint32)((sint32)(remaining * 6) / 15);
    if (!channel && (context->flags & 0x4000))
        cd_get_sector_async(&resource_metadata, 129);
    else
    {
        context = &resource_stream;
        flags = ~context->flags;
        if ((flags & 0x400) && ((flags & 0x2000) || channel == context->filter[1]))
            cd_stream_sector(NULL);
    }
    context = &resource_stream;
    result = context->remaining;
    if ((sint32)result > 0)
        { (void)(uint32)(result); return; }
    resource_set_cd_muted(1);
    game_cd_ready_callback(0);
    { (void)(uint32)(CdControlF(9, NULL)); return; }
}

void cd_audio_ready(uint32 event, const uint8 *response)
{
    RESOURCE_CONTROLLER *context;
    uint32 value;

    FUNCTION_MARKER(0x80029D70, "ACE3.BIN");
    context = &resource_controller;
    memcpy(&value, response, sizeof(value));
    context->response_words[0] = value;
    memcpy(&value, response + 4, sizeof(value));
    event &= 255;
    context->response_words[1] = value;
    if (event != 1)
    {
        resource_stream.error = 5;
        return;
    }
    if (context->wait_mdec)
    {
        value = (uint32)DecDCToutSync(1);
        if (value)
        {
            resource_stream.mdec_pending = 1;
            { (void)(uint32)(value); return; }
        }
    }
    { (void)(cd_audio_sector()); return; }
}

uint32 resource_stop_cd(uint32 mode)
{
    RESOURCE_STREAM *context;
    uint32 command = 9;

    FUNCTION_MARKER(0x8002AC3C, "ACE3.BIN");
    game_stop_resource_music();
    game_cd_ready_callback(0);
    CdSyncCallback(0);
    context = &resource_stream;
    context->flags = 0;
    context->audio_poll_enabled = 0;
    context->phase = 0;
    resource_metadata.channels = 0;
    resource_set_cd_muted(1);
    if (!game_use_extracted_assets() && CdSync(1, (uint8 *)resource_controller.response) != 2)
        return 0;
    if (mode)
        command = mode == 1 ? 7 : 8;
    return CdControl(command, NULL, (uint8 *)resource_controller.response) != 0;
}

void resource_set_stream_paused(uint32 enabled)
{
    uint32 flags, previous;
    RESOURCE_STREAM *state;
    FUNCTION_MARKER(0x8002ABE0, "ACE3.BIN");
    state = &resource_stream;
    flags = state->flags;
    previous = (flags >> 30) & 1;
    if ((sint32)flags >= 0)
        return;
    if (enabled != 0)
    {
        if (flags & 0x40000000)
            { (void)(uint32)(previous); return; }
    }
    else if ((flags & 0x40000000) == 0)
        { (void)(uint32)(previous); return; }
    state->flags = flags | 0x40;
    { (void)(uint32)(previous); return; }
}

uint32 resource_get_read_cursor(void)
{
    FUNCTION_MARKER(0x8002B5FC, "ACE3.BIN");
    return resource_stream.destination.encoded_begin + resource_stream.destination.byte_offset;
}

uint32 resource_get_stream_flags(void)
{
    FUNCTION_MARKER(0x8002B588, "ACE3.BIN");
    return resource_stream.flags;
}

uint32 resource_get_stream_elapsed(void)
{
    FUNCTION_MARKER(0x8002B59C, "ACE3.BIN");
    return resource_stream.elapsed;
}

uint32 resource_get_ready_frame_count(void)
{
    uint32 flags;
    FUNCTION_MARKER(0x8002BCA4, "ACE3.BIN");
    flags = resource_stream.phase;
    if (flags != 64)
        return 0;
    return resource_ring_ready_count();
}

const RESOURCE_FRAME *resource_get_frame_metadata(void)
{
    uint32 channel;
    FUNCTION_MARKER(0x8002B5B0, "ACE3.BIN");
    channel = resource_stream.filter[1];
    if (channel >= 32 || !(resource_metadata.channels & ((uint32)1 << channel)))
        return NULL;
    return resource_metadata.frames + channel;
}

void resource_set_cd_muted(uint32 muted)
{
    SpuCommonAttr attr;

    FUNCTION_MARKER(0x8002999C, "ACE3.BIN");
    attr.mask = 0x200;
    attr.cd.mix = muted == 0;
    SpuSetCommonAttr(&attr);
}

void resource_set_cd_volume(sint32 volume, uint32 mono)
{
    CdlATV attr;

    FUNCTION_MARKER(0x800299D8, "ACE3.BIN");
    if (volume >= 151)
        volume = 150;
    else if (volume < 0)
        volume = 0;
    if (mono)
    {
        attr.val3 = volume >> 1;
        attr.val2 = attr.val3;
        attr.val1 = attr.val3;
        attr.val0 = attr.val3;
    }
    else
    {
        attr.val2 = volume;
        attr.val0 = volume;
        attr.val3 = 0;
        attr.val1 = 0;
    }
    { (void)(cd_apply_volume(&attr)); return; }
}


void cd_get_sector_async(void *dst, uint32 words)
{
    FUNCTION_MARKER(0x800A3450, "ACE3.BIN");
    { (void)(game_cd_copy_sector(dst, words)); return; }
}


uint32 resource_stream_mdec_pending(void)
{
    return resource_stream.mdec_pending;
}

void resource_stream_mdec_complete(void)
{
    resource_stream.mdec_pending = 0;
}
