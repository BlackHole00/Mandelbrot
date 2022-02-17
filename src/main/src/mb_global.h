/**
 * This file holds all the functions used in the state manager for the
 * initialization and deinitialization of the global data.
 * @file mb_global.h
 * @author Vicix
 * @date 07/01/2022
 */
#pragma once

#include "mb_data_structures.h"

/**
 * This function is used as the first_init function for the state manager.
 * It is run once when the state manager has been initialized, before the
 * first state has been run.
 */
void mb_global_init(mb_GlobalData* global_data);

/**
 * This function is used as the close function for the state manager.
 * It is run once when the state manager should close, before after the
 * last state has been run.
 */
void mb_global_close(mb_GlobalData* global_data);

/**
 * Used to update the fractals' color in disco mode.
 */
void mb_shared_disco_update_colors(mb_GlobalData* global_data);