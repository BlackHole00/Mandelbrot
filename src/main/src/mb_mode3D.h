#pragma once

#include <os/os.h>

#include "mb_data_structures.h"

void mb_mode3d_init(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window);
vx_StateUID mb_mode3d_logic(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window, vx_WindowInputHelper* input);
void mb_mode3d_draw(mb_GeneralData* general_data, mb_Mode3DData* state);
void mb_mode3d_resize(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window, u32 width, u32 height);
void mb_mode3d_close(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window);