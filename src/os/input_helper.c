#include "input_helper.h"

vx_WindowInputHelper vx_windowinputhelper_new(vx_Window* window) {
    vx_WindowInputHelper helper;

    helper.keys = window->input_data.keyboard_data.keys;
    helper.mouse.mouse_buttons = window->input_data.mouse_data.mouse_buttons;

    vx_windowinputhelper_update(&helper, window, 0.0);

    return helper;
}

void vx_windowinputhelper_update(vx_WindowInputHelper* helper, vx_Window* window, f64 delta_time) {
    helper->mouse.grabbed   = window->input_data.mouse_data.grabbed;
    helper->mouse.moved     = window->input_data.mouse_data.moved;
    helper->mouse.scrolled  = window->input_data.mouse_data.scrolled;
    helper->mouse.offset_x  = (window->input_data.mouse_data.moved ? window->input_data.mouse_data.offset_x : 0.0f);
    helper->mouse.offset_y  = (window->input_data.mouse_data.moved ? window->input_data.mouse_data.offset_y : 0.0f);
    helper->mouse.pos_x     = window->input_data.mouse_data.pos_x;
    helper->mouse.pos_y     = window->input_data.mouse_data.pos_y;
    helper->mouse.scroll_offset_x = window->input_data.mouse_data.scroll_offset_x;
    helper->mouse.scroll_offset_y = window->input_data.mouse_data.scroll_offset_y;
    helper->delta_time      = delta_time;
}