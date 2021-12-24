/** 
 * @file os/window_descriptor.h
 * @author Vicix
 * @date 23/12/2021
 */
#pragma once
#include <vx_utils.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern void vx_context_load_sokol(GLFWwindow* window);

/**
 * @brief The window descriptor. It's used by the user to define how a window should behave and look.
 * @note VX_DEFAULT available.
 */
typedef struct {
    char* title;
    i32 width;
    i32 height;
    bool fullscreen;
    bool decorated;
    bool transparent_framebuffer;
    bool resizable;
    bool grab_cursor;
    bool show_fps_in_title;

    u32 swap_interval;

    VX_CALLBACK(backend_init_fn, void, GLFWwindow*);

    VX_CALLBACK(init, void, vx_UserStatePtr, vx_WindowControl*);
    VX_CALLBACK(logic, void, vx_UserStatePtr, vx_WindowControl*, vx_WindowInputHelper*);
    VX_CALLBACK(draw, void, vx_UserStatePtr);
    VX_CALLBACK(resize, void, vx_UserStatePtr, vx_WindowControl*, u32, u32);
    VX_CALLBACK(close, void, vx_UserStatePtr, vx_WindowControl*);
} vx_WindowDescriptor;
VX_CREATE_DEFAULT(vx_WindowDescriptor,
    .title      = "Window",
    .width      = 640,
    .height     = 480,
    .fullscreen = 0,
    .resizable  = false,
    .decorated  = true,
    .grab_cursor = false,
    .swap_interval = 0,
    .transparent_framebuffer = false,
    .show_fps_in_title = true,
    .backend_init_fn = vx_context_load_sokol,
    .init       = NULL,
    .logic      = NULL,
    .draw       = NULL,
    .resize     = NULL,
    .close      = NULL,
)