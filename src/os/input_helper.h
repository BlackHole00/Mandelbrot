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
    /**
     * The time passed (in nanoseconds) from the last frame.
     */
    f64 delta_time;

    /**
     * The mouse state
     */
    struct {
        /**
         * Identifies whatever or not the mouse cursor is grabbed.
         */
        bool grabbed;

        /**
         * Identifies whatever or not the mouse has been moved since the last frame.
         */
        bool moved;

        /**
         * The mouse movement in the x axis from the last frame.
         */
        f64 offset_x;

        /**
         * The mouse movement in the y axis from the last frame.
         */
        f64 offset_y;

        /**
         * The cursor position in the x axis.
         */
        f64 pos_x;

        /**
         * The cursor position in the y axis.
         */
        f64 pos_y;

        /**
         * Identifies whatever or not the mouse scroll has been moved since the last frame.
         */
        bool scrolled;

        /**
         * The scroll offset in the x axis from the last frame.
         */
        f64 scroll_offset_x;

        /**
         * The scroll offset in the y axis from the last frame.
         */
        f64 scroll_offset_y;

        /**
         * The mouse buttons. Use GLFW_MOUSE_BUTTON_XXX as an index.
         */
        vx_KeyState* mouse_buttons;
    } mouse;

    /*  This is temporary. Should use a table...    */
    /**
     * The keys. Use GLFW_KEY_XXX as an index.
     */
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