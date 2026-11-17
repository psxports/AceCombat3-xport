#ifndef DIALOGUE_SEQUENCE_H
#define DIALOGUE_SEQUENCE_H

#include "psx.h"

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} DIALOGUE_SEQUENCE_SOURCE;


typedef struct
{
    sint16 resource;
    sint16 audio_resource;
    uint8 audio_kind;
    uint8 widget_value;
    uint8 image_kind;
    uint8 image_layout;
} DIALOGUE_CATALOG_ENTRY;

const DIALOGUE_CATALOG_ENTRY *dialogue_sequence_entry(uint32 id);
uint8 dialogue_sequence_movie_index(uint32 id);

void dialogue_sequence_reset(void);
void dialogue_sequence_bind(DIALOGUE_SEQUENCE_SOURCE source, uint32 skip);
void dialogue_sequence_bind_single(void);
uint32 dialogue_sequence_remaining(void);
sint32 dialogue_sequence_id(sint32 relative);
void dialogue_sequence_advance(void);
void dialogue_sequence_set_sel(uint32 id);
sint32 dialogue_sequence_sel_id(void);
uint32 dialogue_sequence_unlock_archive(uint32 resource);

#endif
