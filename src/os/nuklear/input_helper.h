/** 
 * @file os/nuklear/input_helper.h
 * @author Vicix
 * @date 22/12/2021
 */
#pragma once
#include "../input_helper.h"

#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_glfw_sokol.h>

/**
* @brief Updates nuklear's inputs
* @param input The input helper
* @param always_enabled If true nuklear will be updated even if we are not hovering with any window
* @return Returns if the mouse is hovering on any window
*/
bool vx_inputhelper_update_nuklear_input(vx_WindowInputHelper* input, bool always_enabled);