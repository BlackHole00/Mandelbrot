#include "window.h"

#include <stdio.h>
#include <string.h>

#include <glad/glad.h>
#include <sokol_args.h>
#include <sokol_gfx.h>

#include "window_control.h"
#include "input_helper.h"

static bool _is_glfw_initialized = false;
static void _internal_glfw_resize(GLFWwindow* window, i32 width, i32 height) {
    vx_Window* w = glfwGetWindowUserPointer(window);

    w->callback_functions.resize(w->user_state, w->glfw_window, width, height);
}
static void _internal_glfw_mouse_pos(GLFWwindow* window, f64 pos_x, f64 pos_y) {
    vx_Window* w = glfwGetWindowUserPointer(window);

    w->input_data.mouse_data.moved       = true;
    w->input_data.mouse_data.offset_x    = w->input_data.mouse_data.pos_x - pos_x;
    w->input_data.mouse_data.offset_y    = w->input_data.mouse_data.pos_y - pos_y;
    w->input_data.mouse_data.pos_x       = pos_x;
    w->input_data.mouse_data.pos_y       = pos_y;
}
static void _internal_glfw_mouse_scroll(GLFWwindow* window, double x_offset, double y_offset) {
    vx_Window* w = glfwGetWindowUserPointer(window);

    w->input_data.mouse_data.scrolled = true;
    w->input_data.mouse_data.scroll_offset_x = (float)x_offset;
    w->input_data.mouse_data.scroll_offset_y = (float)y_offset;
}
void _internal_glfw_mouse_button(GLFWwindow* window, int button, int action, int mods) {
    vx_Window* w = glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        w->input_data.mouse_data.mouse_buttons[button].just_pressed = true;
        w->input_data.mouse_data.mouse_buttons[button].pressed = true;
    } else if (action == GLFW_RELEASE) {
        w->input_data.mouse_data.mouse_buttons[button].just_released = true;
        w->input_data.mouse_data.mouse_buttons[button].pressed = false;
    }
}
static void _internal_glfw_keys(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    vx_Window* w = glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        w->input_data.keyboard_data.keys[key].just_pressed = true;
        w->input_data.keyboard_data.keys[key].pressed = true;
    } else if (action == GLFW_RELEASE) {
        w->input_data.keyboard_data.keys[key].just_released = true;
        w->input_data.keyboard_data.keys[key].pressed = false;
    }
}
static void _update_keys_and_buttons(vx_Window* window) {
    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        window->input_data.keyboard_data.keys[i].just_pressed  = false;
        window->input_data.keyboard_data.keys[i].just_released = false;
    }

    for (i32 i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        window->input_data.mouse_data.mouse_buttons[i].just_pressed  = false;
        window->input_data.mouse_data.mouse_buttons[i].just_released = false;
    }

    window->input_data.mouse_data.moved    = false;
    window->input_data.mouse_data.scrolled = false;
}

void vx_glfw_init() {
    if (!_is_glfw_initialized) {
        VX_ASSERT("Could not initialize glfw!", glfwInit());
        _is_glfw_initialized = true;
    }
}

void vx_glfw_terminate() {
    if (_is_glfw_initialized) {
        glfwTerminate();
        _is_glfw_initialized = false;
    }
}

vx_Window vx_window_new(vx_WindowDescriptor* descriptor) {
    VX_NULL_ASSERT(descriptor);

    vx_Window window = VX_DEFAULT(vx_Window);

    /* Initialize the library and set glfw hints.   */
    vx_glfw_init();
    vx_glfw_window_hint(GLFW_DECORATED,                 descriptor->decorated);
    vx_glfw_window_hint(GLFW_RESIZABLE,                 descriptor->resizable);
    vx_glfw_window_hint(GLFW_TRANSPARENT_FRAMEBUFFER,   descriptor->transparent_framebuffer);

    /* Create a window */
    window.glfw_window = glfwCreateWindow(descriptor->width, descriptor->height, descriptor->title, descriptor->fullscreen ? glfwGetPrimaryMonitor(): NULL, NULL);

    /* Crash if the window is NULL */
    VX_ASSERT_EXIT_OP("Could not open glfw window!", window.glfw_window, vx_glfw_terminate());

    /*  Set window callbacks    */
    glfwSetWindowSizeCallback(window.glfw_window, _internal_glfw_resize);
    glfwSetCursorPosCallback(window.glfw_window, _internal_glfw_mouse_pos);
    glfwSetScrollCallback(window.glfw_window, _internal_glfw_mouse_scroll);
    glfwSetKeyCallback(window.glfw_window, _internal_glfw_keys);
    glfwSetMouseButtonCallback(window.glfw_window, _internal_glfw_mouse_button);

    descriptor->backend_init_fn(window.glfw_window);

    /* make sure that the function pointers are not NULL. If they are NULL we can't call them. */
    window.callback_functions.init   = VX_SAFE_FUNC_PTR(descriptor->init);
    window.callback_functions.logic  = VX_SAFE_FUNC_PTR(descriptor->logic);
    window.callback_functions.draw   = VX_SAFE_FUNC_PTR(descriptor->draw);
    window.callback_functions.resize = VX_SAFE_FUNC_PTR(descriptor->resize);
    window.callback_functions.close  = VX_SAFE_FUNC_PTR(descriptor->close);

    /*  Init mouse data and input mode.    */
    window.input_data.mouse_data.grabbed     = descriptor->grab_cursor;
    window.input_data.mouse_data.pos_x       = descriptor->width / 2.0f;
    window.input_data.mouse_data.pos_y       = descriptor->height / 2.0f;
    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, descriptor->grab_cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    /*  Copy information data from the descriptor to the window.    */
    window.info_data.title      = descriptor->title;
    window.info_data.width      = descriptor->width;
    window.info_data.height     = descriptor->height;
    window.info_data.fullscreen = descriptor->fullscreen;
    window.info_data.resizable  = descriptor->resizable;
    window.info_data.decorated  = descriptor->decorated;
    window.info_data.swap_interval = descriptor->swap_interval,
    window.info_data.show_fps_in_title = descriptor->show_fps_in_title;
    window.info_data.transparent_framebuffer = descriptor->transparent_framebuffer;

    /*  Apply the swap interval.    */
    glfwSwapInterval(descriptor->swap_interval);

    return window;
}

void vx_window_run(vx_Window* self, vx_UserStatePtr user_state) {
    VX_NULL_ASSERT(self);

    /*  Set the window user pointer, so we can acces the window's data in the GLFW callback functions.   */
    glfwSetWindowUserPointer(self->glfw_window, self);

    /*  Create helper structs for the user. */
    vx_WindowInputHelper input_helper = vx_windowinputhelper_new(self);
    vx_WindowControl control = vx_windowcontrol_new(self);

    /*  Time counting data  initialization. */
    f64 last_time       = glfwGetTime();
    f64 current_time    = 0.0f;
    f64 delta           = 0.0f;
    f64 counter         = 0.0f;
    u64 frames          = 0;    /*  Frames per second. Used for ms. */

    /*  Set the user state data.    */
    self->user_state = user_state;

    /*  Call the user's init function.  */
    self->callback_functions.init(self->user_state, &control);

    /*  Main loop   */
    while (!glfwWindowShouldClose(self->glfw_window)) {
        /*  Time calculation stuff. */
        current_time = glfwGetTime();
        delta = current_time - last_time;
        last_time = current_time;

        /*  Once a second update the title bar if needed.   */
        if (self->info_data.show_fps_in_title) {
            counter += delta;
            frames++;
            if (counter >= 1.0f) {
                char title[256];
                snprintf(title, 255, "%s (%ld fps - %4.2lf ms)", self->info_data.title, (long)frames, (double)counter / (double)frames);
                glfwSetWindowTitle(self->glfw_window, title);

                frames = 0;
                counter = 0.0f;
            }
        }

        /*  Update the window input helper. */
        vx_windowinputhelper_update(&input_helper, self, delta);

        /*  Call the user's functions.  */
        self->callback_functions.logic(self->user_state, &control, &input_helper);
        self->callback_functions.draw(self->user_state);

        /*  Clear the input's keys and buttons for the next frame.  */
        _update_keys_and_buttons(self);
        glfwSwapBuffers(self->glfw_window);
        glfwPollEvents();
    }

    /*  Call the user's close function and destroy the window.  */
    self->callback_functions.close(self->user_state, &control);
    glfwDestroyWindow(self->glfw_window);

    vx_glfw_terminate();
}
