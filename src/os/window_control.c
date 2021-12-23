#include "window_control.h"

vx_WindowControl vx_windowcontrol_new(vx_Window* window) {
    vx_WindowControl window_control;

    window_control.glfw_window = window->glfw_window;
    window_control.mouse_grabbed = &window->input_data.mouse_data.grabbed;

    return window_control;
}

void vx_windowcontrol_set_mouse_grab(vx_WindowControl* window_control, bool grab) {
    *(window_control->mouse_grabbed) = grab;
    glfwSetInputMode(window_control->glfw_window, GLFW_CURSOR, (grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
}

void vx_windowcontrol_exit(vx_WindowControl* window_control) {
    glfwSetWindowShouldClose(window_control->glfw_window, true);
}