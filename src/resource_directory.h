#ifndef RESOURCE_DIRECTORY_H
#define RESOURCE_DIRECTORY_H

#include "psx.h"

typedef struct
{
    uint32 flags;
    uint32 sector;
    uint32 id;
} RESOURCE_ENTRY;

const RESOURCE_ENTRY *resource_get_entry(uint32 id);
uint32 resource_get_size(uint32 id);
sint32 resource_init_archives(void);

void relocate_pointer_array(uint32 base, uint8 *pointers, sint32 count);
uint32 resource_total_size(const uint32 *ids, sint32 count);
uint32 resource_relocate_directory(uint32 resource, uint32 *native_count, uint8 *native_data, uint32 native_bytes);

uint32 resource_media_kind(uint32 id);

uint32 resource_is_cd_audio(uint32 id);

#endif
