/** 
 * @file os/input_helper.h
 * @author Vicix
 * @date 22/12/2021
 */
#pragma once
#include "window.h"

/**
 * @brief A helper structure used to easily read a window's input.
 */
typedef struct {
    f64 delta_time;

    struct {
        bool grabbed;
        bool moved;
        f64 offset_x;
        f64 offset_y;
        f64 pos_x;
        f64 pos_y;

        bool scrolled;
        f64 scroll_offset_x;
        f64 scroll_offset_y;

        vx_KeyState* mouse_buttons;
    } mouse;

    /*  This is temporary. Should use a table...    */
    vx_KeyState* keys;
} vx_WindowInputHelper;

/**
 * @brief [INTERNAL] The vx_WindowInputHelper construtor.
 * @param window A ponter to a window. The window must live *more* than the window control object.
 * @return The just constructed object.
 */
vx_WindowInputHelper vx_windowinputhelper_new(vx_Window* window);

/**
 * @brief [INTERNAL] Used to update an helper already created. Can be used as long as the bound window remains the *same*.
 * @param helper The helper object.
 * @param window A ponter to the the window. The window must live *more* than the window control object and 
 *  must remain the *same* as the one used in the constructor.
 * @param delta_time The time passed from the last frame.
 */
void vx_windowinputhelper_update(vx_WindowInputHelper* helper, vx_Window* window, f64 delta_time);