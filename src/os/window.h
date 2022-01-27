/** 
 * @file os/window.h
 * @author Vicix
 * @date 22/12/2021
 */
#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vx_utils.h>

#include "key_state.h"
#include "window_descriptor.h"
#include "user_state_ptr.h"

/**
 * @brief The window structure. Hold all the data used by a window.
 * @note VX_DEFAULT available.
 */
typedef struct {
    struct {
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

            vx_KeyState mouse_buttons[GLFW_MOUSE_BUTTON_LAST];
        } mouse_data;

        struct {
            vx_KeyState keys[GLFW_KEY_LAST];
        } keyboard_data;
    } input_data;

    struct {
        char* title;
        i32 width;
        i32 height;
        bool fullscreen;
        bool decorated;
        bool transparent_framebuffer;
        bool resizable;
        bool show_fps_in_title;

        u32 swap_interval;
    } info_data;

    struct {
        VX_CALLBACK(init, void, vx_UserStatePtr, vx_WindowControl*);
        VX_CALLBACK(logic, void, vx_UserStatePtr, vx_WindowControl*, vx_WindowInputHelper*);
        VX_CALLBACK(draw, void, vx_UserStatePtr);
        VX_CALLBACK(resize, void, vx_UserStatePtr, vx_WindowControl*, u32, u32);
        VX_CALLBACK(close, void, vx_UserStatePtr, vx_WindowControl*);
    } callback_functions;

    vx_UserStatePtr user_state;
    GLFWwindow* glfw_window;
} vx_Window;
VX_CREATE_DEFAULT(vx_Window,
    .user_state     = NULL,
    .glfw_window    = NULL,
    .input_data = {
        .keyboard_data = {
            .keys = { VX_DEFAULT(vx_KeyState) },
        },
        .mouse_data = {
            .grabbed    = false,
            .moved      = false,
            .offset_x   = 0,
            .offset_y   = 0,
            .pos_x      = 0,
            .pos_y      = 0,
            .scrolled   = false,
            .scroll_offset_x = 0,
            .scroll_offset_y = 0,
            .mouse_buttons = { VX_DEFAULT(vx_KeyState) },
        }
    },
    .info_data = {
        .title      = "Window",
        .width      = 640,
        .height     = 480,
        .fullscreen = 0,
        .resizable  = false,
        .decorated  = true,
        .swap_interval = 0,
        .transparent_framebuffer = false,
        .show_fps_in_title = true,
    },
    .callback_functions = {
        .init       = NULL,
        .logic      = NULL,
        .draw       = NULL,
        .resize     = NULL,
        .close      = NULL,
    }
)

/**
 * @brief initializes GLFW if it was not initalized.
 */
void vx_glfw_init();

/**
 * @brief terminates glfw if it was already initialized.
 */
void vx_glfw_terminate();

/**
 * @brief Idiomatic wrapper around glfwWindowHint.
 * @param _HINT The window hint to set.
 * @param _VALUE The new value of the window hint.
 */
#define vx_glfw_window_hint(_HINT, _VALUE) glfwWindowHint((_HINT), (_VALUE));

/**
 * @brief The vx_Window constructor.
 * @param descriptor The descriptor of the window.
 * @return The just created window object.
 */
vx_Window vx_window_new(vx_WindowDescriptor* descriptor);

/**
 * @brief Enables the window and runs the main app loop.
 * @param window The pointer to the window.
 * @param user_data A pointer to the user data. Can be NULL.
 */
void vx_window_run(vx_Window* window, vx_UserStatePtr user_data);
