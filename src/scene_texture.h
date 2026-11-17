#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H

#include "psx.h"
#include "resource_queue.h"
#include "psx_gpu.h"
#include "mission_dispatch.h"

void scene_texture_reset(void);
uint32 scene_texture_decode_frame(void);

uint32 scene_texture_read_tim(const uint8 *source, size_t bytes, GpuTimImage *image);
void scene_texture_load_group(uint32 resource, RESOURCE_ASSET_SOURCE parent);
void scene_texture_load_group_sized(uint32 resource, uint32 bytes, RESOURCE_ASSET_SOURCE parent, uint32 header_limited);
void scene_texture_load_mission_images(uint32 resources, const MISSION_BUNDLE_DIRECTORY *bundle);
enum { SCENE_TEXTURE_STANDARD, SCENE_TEXTURE_DECLARED_SIZE };

void scene_texture_load_image(uint32 resource, RESOURCE_ASSET_SOURCE parent, uint32 header_limited);
void scene_texture_load_image_at(const MISSION_IMAGE_DIRECTORY *directory, uint32 index, uint32 x, uint32 y, uint32 clut_x, uint32 clut_y);
void scene_texture_upload_tim_at(const GpuTimImage *image, uint32 pixel_x, uint32 pixel_y, uint32 clut_x, uint32 clut_y);
void scene_texture_upload_tim(uint32 resource, const uint8 *native_data, size_t native_bytes);
void scene_texture_upload_tim_group(uint32 directory, uint32 count, const uint8 *native_data, size_t native_bytes);
uint32 scene_texture_unpack_tim_group(uint32 data, uint32 header, RESOURCE_ASSET_SOURCE parent, RESOURCE_STREAM_BUFFER backing);

void title_texture_upload_image(uint32 resource, const uint8 *data, size_t bytes);

void title_texture_upload_group(const uint8 *input, size_t resource_bytes);

void scene_texture_load_archive_group(const uint8 *input, size_t input_bytes);

#endif
