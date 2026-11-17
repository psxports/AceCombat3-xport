#ifndef RESOURCE_METADATA_H
#define RESOURCE_METADATA_H

#include <stddef.h>

#include "psx.h"

typedef struct
{
    uint16 kind;
    sint16 auxiliary;
    uint32 frame;
    uint32 extra[2];
} RESOURCE_FRAME;

typedef struct
{
    uint32 channels;
    RESOURCE_FRAME frames[32];
} RESOURCE_METADATA;

/* extracted little-endian records, independent of runtime metadata */
typedef struct
{
    uint32 kind_auxiliary;
    uint32 frame;
    uint32 extra[2];
} RESOURCE_FRAME_RECORD;

typedef struct
{
    uint32 channels;
    RESOURCE_FRAME_RECORD frames[32];
} RESOURCE_METADATA_RECORD;

typedef struct
{
    uint32 key;
    RESOURCE_METADATA_RECORD payload;
} RESOURCE_TIMED_METADATA_RECORD;

typedef struct
{
    uint32 number;
    RESOURCE_FRAME_RECORD payload;
} RESOURCE_MOVIE_FRAME_RECORD;

typedef char RESOURCE_METADATA_LAYOUT_CHECK[
    sizeof(RESOURCE_FRAME_RECORD) == 16 && offsetof(RESOURCE_FRAME_RECORD, frame) == 4 &&
    offsetof(RESOURCE_FRAME_RECORD, extra) == 8 &&
    sizeof(RESOURCE_METADATA_RECORD) == 516 && offsetof(RESOURCE_METADATA_RECORD, frames) == 4 &&
    sizeof(RESOURCE_TIMED_METADATA_RECORD) == 520 && offsetof(RESOURCE_TIMED_METADATA_RECORD, payload) == 4 &&
    sizeof(RESOURCE_MOVIE_FRAME_RECORD) == 20 && offsetof(RESOURCE_MOVIE_FRAME_RECORD, payload) == 4 ? 1 : -1];

extern RESOURCE_METADATA resource_metadata;
const RESOURCE_FRAME *resource_get_frame_metadata(void);

#endif
