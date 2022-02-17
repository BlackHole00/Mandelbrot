/**
 * This file holds all the functions GUI related.
 * @file mb_gui.h
 * @author Vicix
 * @date 07/01/2022
 */
#pragma once

#include <os/state_manager.h>
#include <nuklear_args.h>
#include <nuklear.h>

#include "mb_data_structures.h"

/**
 * This functions draws the top part of the GUI, it is used in mode2d and in mode3d.
 */
void mb_control_ui_common(struct nk_context* ctx, mb_GlobalData* global_data);

/**
 * This functions draws the bottom part of the GUI when in mode 2d state.
 */
vx_StateUID mb_control_ui_mode2d(struct nk_context* ctx, mb_GlobalData* global_data, mb_Mode2DData* state);

/**
 * This functions draws the bottom part of the GUI when in mode 3d state.
 */
vx_StateUID mb_control_ui_mode3d(struct nk_context* ctx, mb_GlobalData* global_data, mb_Mode3DData* state);
