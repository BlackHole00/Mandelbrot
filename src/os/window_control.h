/** 
 * @file os/window_control.h
 * @author Vicix
 * @date 22/12/2021
 */
#pragma once

#include "window.h"

/**
 * @brief A helper structure used to easily control a window without interfacing directly with a vx_Window.
 */
typedef struct vx_WindowControl {
    GLFWwindow* glfw_window;
    bool* mouse_grabbed;
} vx_WindowControl;

/**
 * @brief The vx_WindowControl construtor.
 * @param window A ponter to a window. The window must live *more* than the window control object.
 * @return The just constructed object.
 */
vx_WindowControl vx_windowcontrol_new(vx_Window* window);

/**
 * @brief Sets the mouse input mode of the bound window.
 * @param window_control The window control object.
 * @param grab True if the mouse should be grabbed. False if it should be normal.
 */
void vx_windowcontrol_set_mouse_grab(vx_WindowControl* window_control, bool grab);

/**
 * @brief Requests to close to bound the window.
 * @param window_control The window control object.
 */
void vx_windowcontrol_exit(vx_WindowControl* window_control);