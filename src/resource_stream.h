#ifndef RESOURCE_STREAM_H
#define RESOURCE_STREAM_H

#include <stddef.h>

#include "psx.h"
#include "resource_directory.h"
#include "resource_queue.h"

enum
{
    RESOURCE_CD_SECTOR_WORDS = 585,
    RESOURCE_CD_HEADER_BYTES = 12,
    RESOURCE_CD_PAYLOAD_BYTES = 2048
};

typedef union
{
    uint32 words[RESOURCE_CD_SECTOR_WORDS];
    struct
    {
        uint8 header[RESOURCE_CD_HEADER_BYTES];
        uint8 payload[RESOURCE_CD_PAYLOAD_BYTES];
        uint8 tail[RESOURCE_CD_SECTOR_WORDS * sizeof(uint32) - RESOURCE_CD_HEADER_BYTES - RESOURCE_CD_PAYLOAD_BYTES];
    } bytes;
} RESOURCE_CD_SECTOR;

typedef char resource_cd_sector_layout_check[
    sizeof(RESOURCE_CD_SECTOR) == 2340 &&
    offsetof(RESOURCE_CD_SECTOR, bytes.payload) == 12 ? 1 : -1];



typedef struct
{
    const RESOURCE_ENTRY *entry;
    /* mode 0 requires a native buffer */
    RESOURCE_STREAM_BUFFER destination;
    uint32 ring_bytes;
    /* copied audio descriptors keep the existing extracted-file rejection path */
    uint8 adjusted_entry;
} RESOURCE_READ_INPUT;



typedef struct
{
    uint32 flags;
    uint32 phase;
    uint32 channel;
} RESOURCE_STREAM_STATUS;

uint32 resource_stream_mdec_pending(void);
void resource_stream_mdec_complete(void);
void resource_stream_reset(void);
RESOURCE_STREAM_STATUS resource_stream_status(void);
void resource_stream_select_channel(uint32 channel);
void resource_stream_begin_file(uint32 flags, uint32 data_stream, uint32 remaining, uint32 end_sector);
void resource_stream_file_position(uint32 sector, uint32 end_sector, uint32 frames);
void resource_stream_end_file(void);
uint32 resource_stream_accepts_audio_sector(void);
intptr resource_poll_transfer(uint32 result);

void game_cd_resource_ready(uint8 event, uint8 *response);
void game_cd_audio_ready(uint8 event, uint8 *response);
void game_cd_resource_sync(uint8 event, uint8 *response);
CdlCB game_cd_ready_callback(CdlCB callback);

sint32 resource_begin_read(RESOURCE_READ_INPUT input, uint32 flags);
uint32 resource_issue_seek(void);
uint32 resource_sync_callback(uint32 event, const uint8 *response);
uint32 resource_read_phase(void);
uint32 resource_sector_ready(uint32 event, const uint8 *response, const RESOURCE_CD_SECTOR *native_sector);
uint32 cd_audio_step(void);
uint32 cd_audio_poll(uint32 event);
void cd_audio_sector(void);
void cd_audio_ready(uint32 event, const uint8 *response);
uint32 resource_stop_cd(uint32 mode);
void resource_set_stream_paused(uint32 enabled);
uint32 resource_get_read_cursor(void);
uint32 resource_get_stream_flags(void);
uint32 resource_get_stream_elapsed(void);
uint32 resource_get_ready_frame_count(void);
void resource_set_cd_muted(uint32 enabled);
void resource_set_cd_volume(sint32 volume, uint32 mono);

void cd_get_sector_async(void *dst, uint32 words);

#endif
