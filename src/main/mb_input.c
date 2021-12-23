#include "mb_input.h"

#include "mb_hmm_helper.h"

void mb_camera_controls_3dmode(mb_State* state, vx_WindowInputHelper* input) {
    if (input->keys[GLFW_KEY_W].pressed) {
        state->perspCameraData.position = HMM_AddVec3(state->perspCameraData.position, HMM_MultiplyVec3f(state->perspCameraData.front, state->perspCameraData.movementSpeed));
    } else if (input->keys[GLFW_KEY_S].pressed) {
        state->perspCameraData.position = HMM_SubtractVec3(state->perspCameraData.position, HMM_MultiplyVec3f(state->perspCameraData.front, state->perspCameraData.movementSpeed));
    }
    if (input->keys[GLFW_KEY_A].pressed) {
        hmm_vec3 tmp = HMM_NormalizeVec3(HMM_Cross(state->perspCameraData.front, HMM_Vec3(0.0f, 1.0f, 0.0f)));
        state->perspCameraData.position = HMM_SubtractVec3(state->perspCameraData.position, HMM_MultiplyVec3f(tmp, state->perspCameraData.movementSpeed));
    } else if (input->keys[GLFW_KEY_D].pressed) {
        hmm_vec3 tmp = HMM_NormalizeVec3(HMM_Cross(state->perspCameraData.front, HMM_Vec3(0.0f, 1.0f, 0.0f)));
        state->perspCameraData.position = HMM_AddVec3(state->perspCameraData.position, HMM_MultiplyVec3f(tmp, state->perspCameraData.movementSpeed));
    }

    if (input->keys[GLFW_KEY_SPACE].pressed) {
        state->perspCameraData.position.Y += state->perspCameraData.movementSpeed;
    } else if (input->keys[GLFW_KEY_LEFT_SHIFT].pressed) {
        state->perspCameraData.position.Y -= state->perspCameraData.movementSpeed;
    }

    if ((input->mouse.grabbed && input->mouse.moved) ||
        (!input->mouse.grabbed && input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed)
    ) {
        state->perspCameraData.rotation.X -= (float)input->mouse.offset_x * 0.25f;
        state->perspCameraData.rotation.Y += (float)input->mouse.offset_y * 0.25f;

        if(state->perspCameraData.rotation.Y > 89.0f) {
            state->perspCameraData.rotation.Y = 89.0f;
        }
        if(state->perspCameraData.rotation.Y < -89.0f) {
            state->perspCameraData.rotation.Y = -89.0f;
        }

        state->perspCameraData.front = HMM_Vec2Direction(state->perspCameraData.rotation);
    }

    if (input->mouse.scrolled) {
        state->perspCameraData.fov -= (f32)input->mouse.scroll_offset_y;
        if (state->perspCameraData.fov >= 130.0f) {
            state->perspCameraData.fov = 130.0f;
        } else if (state->perspCameraData.fov <= 10.0f) {
            state->perspCameraData.fov = 10.0f;
        }
        state->perspCameraData.fovChanged = true;
    }
}

void mb_input_mode2d(mb_State* state, vx_WindowInputHelper* input) {
    if (input->mouse.scrolled) {
        state->mandelbrotInfoBlock.xScale += (f32)input->mouse.scroll_offset_y * state->mandelbrotInfoBlock.xScale / 10.0f;
        state->mandelbrotInfoBlock.yScale += (f32)input->mouse.scroll_offset_y * state->mandelbrotInfoBlock.yScale / 10.0f;
    }

    if (input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed) {
        state->mandelbrotInfoBlock.xPosition += (f32)input->mouse.offset_x / (f32)state->gfxData.screenWidth   / state->mandelbrotInfoBlock.xScale;
        state->mandelbrotInfoBlock.yPosition += (f32)input->mouse.offset_y / -(f32)state->gfxData.screenHeight / state->mandelbrotInfoBlock.yScale;
    }
}

void mb_input_mode3d(mb_State* state, vx_WindowInputHelper* input, vx_WindowControl* window) {
    if (input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].just_pressed || input->mouse.mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].just_released) {
        vx_windowcontrol_set_mouse_grab(window, !input->mouse.grabbed);
    }

    mb_camera_controls_3dmode(state, input);
}