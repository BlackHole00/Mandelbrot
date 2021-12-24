#pragma once

#include <os/os.h>

#include "mb_data_structures.h"

void mb_mode2d_init(mb_GeneralData* general_data, mb_Mode2DData* state, vx_WindowControl* window);
vx_StateUID mb_mode2d_logic(mb_GeneralData* general_data, mb_Mode2DData* state, vx_WindowControl* window, vx_WindowInputHelper* input);
void mb_mode2d_draw(mb_GeneralData* general_data, mb_Mode2DData* state);
void mb_mode2d_resize(mb_GeneralData* general_data, mb_Mode2DData* state, vx_WindowControl* window, u32 width, u32 height);
void mb_mode2d_close(mb_GeneralData* general_data, mb_Mode2DData* state, vx_WindowControl* window);