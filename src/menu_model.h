#ifndef MENU_MODEL_H
#define MENU_MODEL_H

#include "psx.h"

enum { MENU_MODEL_PREVIEW_COUNT = 48, MENU_MODEL_BUFFER_BYTES = 0x4008 };

typedef struct
{
    uint16 model_resource;
    uint16 detail_resource;
} MENU_MODEL_PREVIEW_ENTRY;

const MENU_MODEL_PREVIEW_ENTRY *menu_model_preview_entry(uint32 index);
void menu_model_reset(void);
void menu_model_apply_view(uint32 mode);
uint32 menu_model_load(void);
uint32 menu_model_restore(void);

void menu_model_request(void);
uint32 menu_model_draw_aircraft(uint32 mode);
uint32 menu_model_begin(void);
void menu_model_wait_load(void);
uint32 menu_model_select_tick(void);
void menu_model_select_finish(void);
uint32 menu_model_detail_begin(void);
uint32 menu_model_detail_tick(void);

void menu_model_update_selection(uint32 mode);
void menu_model_draw_selection_preview(void);
void menu_model_initialize_lighting(void);
void menu_model_initialize_camera(void);
void aircraft_preview_initialize(void);

#endif
