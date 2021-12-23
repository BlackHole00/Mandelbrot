/** 
 * @file os/key_state.h
 * @author Vicix
 * @date 23/12/2021
 */
#pragma once

#include <vx_utils.h>

/**
 * @brief A struct that holds the state of a key or a button.
 * @note VX_DEFAULT available.
 */
typedef struct {
    bool pressed: 1;
    bool just_pressed: 1;
    bool just_released: 1;
} vx_KeyState;
VX_CREATE_DEFAULT(vx_KeyState,
    .pressed = false,
    .just_pressed = false,
    .just_released = false,
)