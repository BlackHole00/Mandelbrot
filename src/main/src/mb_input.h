/**
 * This file holds all the functions used to process user's input.
 * @file mb_input.h
 * @author Vicix
 * @date 07/01/2022
 */
#pragma once

#include <os/input_helper.h>
#include <os/window_control.h>

#include "mb_data_structures.h"

/**
 * This functions moves the camera in the 3d state.
 */
void mb_camera_controls_3dmode(mb_Mode3DData* state, vx_WindowInputHelper* input);

/**
 * This functions handles the input in the 2d state.
 */
void mb_input_mode2d(mb_GlobalData* global_data, vx_WindowInputHelper* input);

/**
 * This functions handles the input in the 3d state.
 */
void mb_input_mode3d(mb_Mode3DData* state, vx_WindowInputHelper* input, vx_WindowControl* window);
