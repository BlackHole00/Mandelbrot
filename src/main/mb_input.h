#pragma once

#include <os/input_helper.h>
#include <os/window_control.h>

#include "mb_data_structures.h"

void mb_camera_controls_3dmode(mb_State* state, vx_WindowInputHelper* input);
void mb_input_mode2d(mb_State* state, vx_WindowInputHelper* input);
void mb_input_mode3d(mb_State* state, vx_WindowInputHelper* input, vx_WindowControl* window);
