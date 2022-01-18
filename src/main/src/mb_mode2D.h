/**
 * This file holds all the functions used in the state manager for the
 * initialization, deinitialization and behaviour of the mode 2d state.
 * @file mb_mode2D.h
 * @author Vicix
 * @date 07/01/2022
 */
#pragma once

#include <os/os.h>

#include "mb_data_structures.h"

/**
 * Called everytime the state is loaded.
 */
void mb_mode2d_init(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window);

/**
 * Called every frame. Update the state logic.
 */
vx_StateUID mb_mode2d_logic(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window, vx_WindowInputHelper* input);

/**
 * Called every frame. Draws to the screen.
 */
void mb_mode2d_draw(mb_GlobalData* general_data, mb_Mode2DData* state);

/**
 * Called every time the window is resized.
 */
void mb_mode2d_resize(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window, u32 width, u32 height);

/**
 * Called when the application is closing or when the state is being switched to mode 3d.
 */
void mb_mode2d_close(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window);