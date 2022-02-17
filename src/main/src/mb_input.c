#include "mb_input.h"

#include <hmm_helper.h>
#include <logic/transform.h>

void mb_camera_controls_3dmode(mb_Mode3DData* state, vx_WindowInputHelper* input) {
    if (input->keys[GLFW_KEY_W].pressed) {
        vx_position_move_forward(&state->camera.position, &state->camera.rotation, input->delta_time * state->movement_speed);
    } else if (input->keys[GLFW_KEY_S].pressed) {
        vx_position_move_backward(&state->camera.position, &state->camera.rotation, input->delta_time * state->movement_speed);
    }
    if (input->keys[GLFW_KEY_A].pressed) {
        vx_position_move_left(&state->camera.position, &state->camera.rotation, input->delta_time * state->movement_speed);
    } else if (input->keys[GLFW_KEY_D].pressed) {
        vx_position_move_right(&state->camera.position, &state->camera.rotation, input->delta_time * state->movement_speed);
    }

    if (input->keys[GLFW_KEY_SPACE].pressed) {
        vx_position_move(&state->camera.position, HMM_Vec3(0.0f, 1.0f, 0.0f), input->delta_time * state->movement_speed);
    } else if (input->keys[GLFW_KEY_LEFT_SHIFT].pressed) {
        vx_position_move(&state->camera.position, HMM_Vec3(0.0f, -1.0f, 0.0f), input->delta_time * state->movement_speed);

    }

    if ((input->mouse.grabbed && input->mouse.moved) ||
        (!input->mouse.grabbed && input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed)
    ) {
        vx_rotation_rotate(&state->camera.rotation, HMM_Vec3(
            -(float)input->mouse.offset_x,
             (float)input->mouse.offset_y,
            0.0f),
            0.25f * state->movement_speed
        );
    }

    if (input->mouse.scrolled) {
        state->camera.fov -= (f32)input->mouse.scroll_offset_y;
        if (state->camera.fov >= 130.0f) {
            state->camera.fov = 130.0f;
        } else if (state->camera.fov <= 10.0f) {
            state->camera.fov = 10.0f;
        }
    }
}

void mb_input_mode2d(mb_GlobalData* global_data, vx_WindowInputHelper* input) {
    if (input->mouse.scrolled) {
        global_data->mandelbrot_info_block.x_scale += (f32)input->mouse.scroll_offset_y * global_data->mandelbrot_info_block.x_scale / 10.0f;
        global_data->mandelbrot_info_block.y_scale += (f32)input->mouse.scroll_offset_y * global_data->mandelbrot_info_block.y_scale / 10.0f;
    }

    if (input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed) {
        global_data->mandelbrot_info_block.x_position += (f32)input->mouse.offset_x / (f32)global_data->gfx_data.screenWidth   / global_data->mandelbrot_info_block.x_scale;
        global_data->mandelbrot_info_block.y_position += (f32)input->mouse.offset_y / -(f32)global_data->gfx_data.screenHeight / global_data->mandelbrot_info_block.y_scale;
    }
}

void mb_input_mode3d(mb_Mode3DData* state, vx_WindowInputHelper* input, vx_WindowControl* window) {
    if (input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].just_pressed || input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].just_released) {
        vx_windowcontrol_set_mouse_grab(window, !input->mouse.grabbed);
    }

    mb_camera_controls_3dmode(state, input);
}