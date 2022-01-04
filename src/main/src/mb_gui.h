#pragma once

#include <os/state_manager.h>
#include <nuklear_args.h>
#include <nuklear.h>

#include "mb_data_structures.h"

void mb_control_ui_common(struct nk_context* ctx, mb_GeneralData* general_data);
vx_StateUID mb_control_ui_mode2d(struct nk_context* ctx, mb_GeneralData* general_data, mb_Mode2DData* state);
vx_StateUID mb_control_ui_mode3d(struct nk_context* ctx, mb_GeneralData* general_data, mb_Mode3DData* state);
