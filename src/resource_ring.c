#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint32 payload_words;
    uint32 payload_bytes;
    uint32 mode;
    uint32 capacity;
    /* Both wire spans borrow the current resource arena until arena ownership migration */
    RESOURCE_SECTOR_HEADER *header_buffer;
    uint8 *payload_buffer;
    uint32 written;
    RESOURCE_SECTOR_HEADER *write_header;
    uint8 *write_payload;
    uint32 frame_id;
    uint32 sequence;
    RESOURCE_SECTOR_HEADER *sector_header;
    uint8 *sector_payload;
    uint32 ready;
    uint32 read_segments;
    RESOURCE_SECTOR_HEADER *read_header;
    uint8 *read_payload;
} RESOURCE_RING;

static RESOURCE_RING resource_ring;

void resource_ring_reset_storage(void)
{
    memset(&resource_ring, 0, sizeof(resource_ring));
}

uint32 resource_ring_ready_count(void)
{
    return resource_ring.ready;
}

uint32 resource_frame_payload_bytes(const RESOURCE_SECTOR_HEADER *header)
{
    return (uint32)header->info.parts * resource_ring.payload_words * 4;
}

void resource_init_ring(void *buffer, uint32 bytes, uint32 mode)
{
    uint32 sector_bytes;
    RESOURCE_RING *context;
    uint8 *payload;
    RESOURCE_SECTOR_HEADER *headers;
    sint32 count;

    FUNCTION_MARKER(0x8002B610, "ACE3.BIN");
    sector_bytes = mode ? 0x918 : 0x800;
    count = (sint32)bytes / (sint32)sector_bytes;
    context = &resource_ring;
    if (count < 0)
        abort();
    payload = buffer;
    context->header_buffer = (RESOURCE_SECTOR_HEADER *)payload;
    context->mode = mode;
    context->written = 0;
    context->sequence = 0;
    context->ready = 0;
    context->read_segments = 0;
    context->capacity = (uint32)count;
    context->payload_buffer = payload + (uint32)count * 32;
    context->write_header = context->header_buffer;
    payload = context->payload_buffer;
    context->payload_words = (sector_bytes - 32) >> 2;
    context->payload_bytes = (uint32)count * (sector_bytes - 32);
    context->write_payload = payload;
    headers = context->write_header;
    payload = context->write_payload;
    context->frame_id = 0xFFFFFFFF;
    context->sector_header = headers;
    context->sector_payload = payload;
    context->read_header = headers;
    context->read_payload = payload;
    return;
}

void resource_reset_ring(void)
{
    RESOURCE_RING *context;
    RESOURCE_SECTOR_HEADER *cursor;
    uint32 words;
    uint32 i = 0;
    sint32 count;

    FUNCTION_MARKER(0x8002B6D0, "ACE3.BIN");
    context = &resource_ring;
    cursor = context->header_buffer;
    count = (sint32)context->capacity;
    words = (sizeof(RESOURCE_SECTOR_HEADER) / sizeof(uint32));
    if (count <= 0)
        { (void)(uint32)(words); return; }
    do
    {
        cursor->info.status = 0;
        context = &resource_ring;
        ++i;
        count = (sint32)context->capacity;
        ++cursor;
    } while ((sint32)i < count);
    return;
}

uint32 cd_stream_sector(const RESOURCE_SCRIPT_SECTOR *native_sector)
{
    RESOURCE_RING *context;
    RESOURCE_SECTOR_HEADER *header;
    RESOURCE_SECTOR_HEADER *destination;
    uint8 *payload;
    const uint32 *source;
    uint32 *target;
    uint32 words;
    uint32 header_words;
    uint32 payload_words;
    uint32 result;
    uint32 sequence;
    uint32 count;
    uint32 total;
    uint32 capacity;
    uint32 value;
    uint32 i;

    FUNCTION_MARKER(0x8002B760, "ACE3.BIN");
    context = &resource_ring;
    header = context->sector_header;
    if (!context->capacity || header < context->header_buffer || header >= context->header_buffer + context->capacity)
        abort();
    result = header->info.status;
    if (result == 2 || result == 4)
        return result;
    words = (sizeof(RESOURCE_SECTOR_HEADER) / sizeof(uint32));
    if (native_sector)
    {
        if (words > 8)
            abort();
        *header = native_sector->payload.sector_header;
    }
    else
        cd_get_sector(header, words);
    if (header->info.status != 1)
    {
        header->info.status = 0;
        return 1;
    }
    context = &resource_ring;
    if (context->frame_id != header->info.frame)
    {
        result = header->info.part;
        if (result)
            return result;
        destination = context->write_header;
        context->sector_header = destination;
        payload = context->write_payload;
        context->sector_payload = payload;
        count = header->info.parts;
        total = context->written + count;
        capacity = context->capacity;
        if (count > capacity)
            abort();
        if ((sint32)capacity < (sint32)total)
        {
            header->info.status = 4;
            context = &resource_ring;
            context->write_header = context->header_buffer;
            payload = context->payload_buffer;
            destination = context->write_header;
            context->written = 0;
            context->sector_header = destination;
            context->write_payload = payload;
            context->sector_payload = payload;
            result = destination->info.status;
            if (result == 2)
                return result;
        }
        context = &resource_ring;
        value = header->info.frame;
        context->frame_id = value;
        if (header != destination)
        {
            source = header->words;
            target = destination->words;
            words = (sizeof(RESOURCE_SECTOR_HEADER) / sizeof(uint32));
            header = context->write_header;
            i = 0;
            if ((sint32)words > 0)
            {
                do
                {
                    value = *source;
                    ++source;
                    *target = value;
                    words = (sizeof(RESOURCE_SECTOR_HEADER) / sizeof(uint32));
                    ++i;
                    ++target;
                } while ((sint32)i < (sint32)words);
            }
        }
    }
    else
    {
        result = context->sequence + 1;
        sequence = header->info.part;
        if (result != sequence)
            return result;
    }
    context = &resource_ring;
    sequence = header->info.part;
    payload = context->sector_payload;
    payload_words = context->payload_words;
    header_words = (sizeof(RESOURCE_SECTOR_HEADER) / sizeof(uint32));
    context->sequence = sequence;
    if (payload < context->payload_buffer || payload > context->payload_buffer + context->payload_bytes ||
        (payload_words << 2) > context->payload_bytes - (uint32)(payload - context->payload_buffer))
        abort();
    if (native_sector)
    {
        if (header_words + payload_words > 582)
            abort();
        memcpy(payload, native_sector->payload.bytes + (header_words << 2), payload_words << 2);
    }
    else
        cd_get_sector(payload, payload_words);
    context = &resource_ring;
    sequence = context->sequence + 1;
    count = header->info.parts;
    if (sequence == count)
    {
        destination = context->write_header;
        payload = context->write_payload;
        if ((sint32)sequence > 0)
        {
            i = 0;
            do
            {
                destination->info.status = 2;
                ++destination;
                count = header->info.parts;
                ++i;
                payload += payload_words << 2;
            } while (i < count);
            context = &resource_ring;
        }
        context->ready = context->ready + 1;
        count = header->info.parts;
        total = context->written;
        capacity = context->capacity;
        total += count;
        context->written = total;
        if ((sint32)total >= (sint32)capacity)
        {
            destination = context->header_buffer;
            payload = context->payload_buffer;
            context->written = 0;
            context->write_header = destination;
            context->write_payload = payload;
        }
        else
        {
            context->write_header = destination;
            context->write_payload = payload;
        }
        context = &resource_ring;
        destination = context->write_header;
        payload = context->write_payload;
        context->sector_header = destination;
        context->sector_payload = payload;
        return 0;
    }
    ++context->sector_header;
    context->sector_payload += payload_words << 2;
    return 0;
}

uint32 cd_stream_get_frame(RESOURCE_RING_FRAME *frame)
{
    RESOURCE_STREAM_STATUS stream;
    RESOURCE_RING *ring;
    uint32 flags;
    uint32 channel;
    RESOURCE_SECTOR_HEADER *header;
    uint32 status;
    uint32 value;
    uint32 i;
    uint32 count;
    uint32 total;
    uint32 capacity;
    uint32 payload_words;
    RESOURCE_SECTOR_HEADER *headers;
    uint8 *data;

    FUNCTION_MARKER(0x8002B9EC, "ACE3.BIN");
    stream = resource_stream_status();
    if (stream.phase != 64 || (sint32)resource_ring.ready <= 0)
    {
        flags = stream.flags;
        channel = stream.channel;
        if (flags & 0x8000)
        {
            resource_metadata.channels &= ~((uint32)1 << (channel & 31));
        }
        return 1;
    }
    ring = &resource_ring;
    header = ring->read_header;
    if (!ring->capacity || header < ring->header_buffer || header >= ring->header_buffer + ring->capacity)
        abort();
    status = header->info.status;
    if (status == 4)
    {
        header->info.status = 0;
        ring = &resource_ring;
        headers = ring->header_buffer;
        ring->read_header = headers;
        header = headers;
        data = ring->payload_buffer;
        ring->read_segments = 0;
        ring->read_payload = data;
        status = header->info.status;
    }
    if (status != 2)
        return 1;
    if (ring->read_payload < ring->payload_buffer || ring->read_payload > ring->payload_buffer + ring->payload_bytes ||
        (uint32)header->info.parts * ring->payload_words * 4 >
        ring->payload_bytes - (uint32)(ring->read_payload - ring->payload_buffer))
        abort();
    /* The returned frame stays borrowed until all of its sector headers are freed */
    if (header->info.parts > ring->capacity - (uint32)(header - ring->header_buffer))
        abort();
    stream = resource_stream_status();
    if (stream.flags & 0x8000)
    {
        channel = stream.channel;
        resource_metadata.channels |= (uint32)1 << (channel & 31);
        value = header->words[4];
        resource_metadata.frames[channel].kind = (uint16)value;
        resource_metadata.frames[channel].auxiliary = (sint16)(value >> 16);
        resource_metadata.frames[channel].frame = header->words[5];
        resource_metadata.frames[channel].extra[0] = header->words[6];
        resource_metadata.frames[channel].extra[1] = header->words[7];
    }
    ring = &resource_ring;
    frame->header = header;
    ring = &resource_ring;
    frame->payload = ring->read_payload;
    ring = &resource_ring;
    count = header->info.parts;
    total = ring->read_segments;
    capacity = ring->capacity;
    total += count;
    ring->read_segments = total;
    payload_words = ring->payload_words;
    if ((sint32)total >= (sint32)capacity)
    {
        headers = ring->header_buffer;
        data = ring->payload_buffer;
        ring->read_segments = 0;
        ring->read_header = headers;
        ring->read_payload = data;
    }
    else if (header->info.parts)
    {
        i = 0;
        do
        {
            headers = ring->read_header;
            data = ring->read_payload;
            ++headers;
            data += payload_words << 2;
            ring->read_header = headers;
            ring->read_payload = data;
            count = header->info.parts;
            ++i;
        } while (i < count);
    }
    ring = &resource_ring;
    ring->ready = ring->ready - 1;
    return 0;
}

uint32 cd_stream_free_frame(RESOURCE_SECTOR_HEADER *header)
{
    RESOURCE_SECTOR_HEADER *cursor = header;
    uint32 count;
    uint32 index = 0;
    uint32 value;
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x8002BBE0, "ACE3.BIN");
    count = cursor->info.parts;
    while (index < count)
    {
        value = cursor->info.status;
        if (value != 2)
        {
            first = cursor->info.part;
            second = cursor->info.parts;
#ifndef NDEBUG
            printf("ERR : KCdStFreeRing %d:(%p:%p) %02x %02x %02x\n", (sint32)(index), (void *)header, (void *)cursor, (uint32)(value), (uint32)(first), (uint32)(second));
#endif
#ifndef NDEBUG
            printf("      %p\n", (void *)resource_ring.sector_header);
#endif
            CdControlB(9, NULL, (uint8 *)resource_controller.response);
            StopCallback();
            return 1;
        }
        cursor->info.status = 0;
        ++index;
        ++cursor;
    }
    return 0;
}
