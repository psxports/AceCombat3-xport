#ifndef RESOURCE_RING_H
#define RESOURCE_RING_H

#include <stddef.h>

#include "psx.h"
#include "resource_metadata.h"

/* The sector prefix is shared by MDEC, texture and scripted-frame codecs */
typedef union
{
    struct
    {
        uint8 status;
        uint8 part;
        uint8 parts;
        uint8 control;
        uint16 code;
        uint16 frame;
        union
        {
            struct
            {
                sint16 width;
                sint16 height;
            } image;
            uint8 scripted[4];
        } geometry;
        union
        {
            uint8 bytes[20];
            uint16 halfwords[10];
            uint32 words[5];
        } codec;
    } info;
    uint32 words[8];
} RESOURCE_SECTOR_HEADER;

typedef char RESOURCE_SECTOR_HEADER_LAYOUT_CHECK[
    sizeof(RESOURCE_SECTOR_HEADER) == 32 &&
    offsetof(RESOURCE_SECTOR_HEADER, info.frame) == 6 &&
    offsetof(RESOURCE_SECTOR_HEADER, info.geometry) == 8 &&
    offsetof(RESOURCE_SECTOR_HEADER, info.codec) == 12 ? 1 : -1];

typedef struct
{
    uint8 header[24];
    union
    {
        uint8 bytes[2328];
        RESOURCE_METADATA_RECORD metadata;
        RESOURCE_SECTOR_HEADER sector_header;
    } payload;
} RESOURCE_SCRIPT_SECTOR;

typedef char resource_script_sector_layout_check[
    sizeof(RESOURCE_SCRIPT_SECTOR) == 2352 &&
    offsetof(RESOURCE_SCRIPT_SECTOR, payload) == 24 &&
    sizeof(((RESOURCE_SCRIPT_SECTOR *)0)->payload.bytes) == 2328 ? 1 : -1];

typedef struct
{
    RESOURCE_SECTOR_HEADER *header;
    uint8 *payload;
} RESOURCE_RING_FRAME;

void resource_ring_reset_storage(void);
uint32 resource_ring_ready_count(void);
uint32 resource_frame_payload_bytes(const RESOURCE_SECTOR_HEADER *header);

void resource_init_ring(void *buffer, uint32 bytes, uint32 mode);
void resource_reset_ring(void);
uint32 cd_stream_sector(const RESOURCE_SCRIPT_SECTOR *native_sector);
uint32 cd_stream_get_frame(RESOURCE_RING_FRAME *frame);
uint32 cd_stream_free_frame(RESOURCE_SECTOR_HEADER *header);

#endif
