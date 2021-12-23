#pragma once
#include <GLFW/glfw3.h>

#ifndef NK_NUKLEAR_H_
#error "Include nuklear first"
#endif

#define NK_GLFW_SOKOL_INCLUDED

enum nk_keys nk_glfw_key_to_nuklear_key(int key);
enum nk_buttons nk_glfw_button_to_nuklear_button(int button);
void nk_glfw_input_begin();
void nk_glfw_input_end();
void nk_glfw_process_key(int key, int scancode, int action, int mods);
void nk_glfw_process_mouse(GLFWwindow* window);
void nk_glfw_process_mouse_position(double xpos, double ypos);
void nk_glfw_process_mouse_scroll(double xoffset, double yoffset);
void nk_glfw_process_mouse_event(int button, int action, int mods);
void nk_glfw_set_context(struct nk_context* ctx);
struct nk_context* nk_glfw_get_context();

#ifdef NUKLEAR_GLFW_SOKOL_IMPLEMENTATION

static struct nk_context* ctx = NULL;

enum nk_buttons nk_glfw_button_to_nuklear_button(int button) {
    if (button > GLFW_MOUSE_BUTTON_3) {
        return -1;
    }

    return button;
}

enum nk_keys nk_glfw_key_to_nuklear_key(int key) {
    switch (key) {
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT: { return NK_KEY_SHIFT; }
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL: { return NK_KEY_CTRL; }
        case GLFW_KEY_DELETE: { return NK_KEY_DEL; }
        case GLFW_KEY_ENTER: { return NK_KEY_ENTER; }
        case GLFW_KEY_TAB: { return NK_KEY_TAB; }
        case GLFW_KEY_BACKSPACE: { return NK_KEY_BACKSPACE; }
        case GLFW_KEY_UP: { return NK_KEY_UP; }
        case GLFW_KEY_DOWN: { return NK_KEY_DOWN; }
        case GLFW_KEY_LEFT: { return NK_KEY_LEFT; }
        case GLFW_KEY_RIGHT: { return NK_KEY_RIGHT; }
        default: { return NK_KEY_NONE; }
    }
}

void nk_glfw_process_key(int key, int scancode, int action, int mods) {
    if (key >= 65 && key <= 90 && ctx->input.keyboard.text_len < NK_INPUT_MAX) {
        ctx->input.keyboard.text[ctx->input.keyboard.text_len] = (char)key;
        ctx->input.keyboard.text_len++;
    } else {
        if (key == GLFW_PRESS) {
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].down = true;
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].clicked = true;
        } else if (key == GLFW_RELEASE) {
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].down = false;
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].clicked = false;
        } else if (key == GLFW_REPEAT) {
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].down = true;
            ctx->input.keyboard.keys[nk_glfw_key_to_nuklear_key(key)].clicked = false;
        }
    }
}

void nk_glfw_process_mouse(GLFWwindow* window) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        nk_glfw_process_mouse_event(GLFW_MOUSE_BUTTON_LEFT, leftState, 0);
        nk_glfw_process_mouse_position(xpos, ypos);
    }
}

void nk_glfw_process_mouse_position(double xpos, double ypos) {
    nk_input_motion(ctx, (int)xpos, (int)ypos);
}

void nk_glfw_process_mouse_event(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        ctx->input.mouse.buttons[button].clicked = (!ctx->input.mouse.buttons[button].down ? true : false);
        ctx->input.mouse.buttons[button].down = true;
        ctx->input.mouse.buttons[button].clicked_pos = ctx->input.mouse.pos;
    } else if (action == GLFW_RELEASE) {
        //printf("Release: %d %d\n", ctx->input.mouse.buttons[button].down, ctx->input.mouse.buttons[button].clicked);
        ctx->input.mouse.buttons[button].down = false;
        ctx->input.mouse.buttons[button].clicked = false;
    }
}

void nk_glfw_process_mouse_scroll(double xoffset, double yoffset) {
    ctx->input.mouse.scroll_delta.x = (float)xoffset;
    ctx->input.mouse.scroll_delta.y = (float)yoffset;
}

void nk_glfw_set_context(struct nk_context* ct) {
    ctx = ct;
}

struct nk_context* nk_glfw_get_context() {
    return ctx;
}

void nk_glfw_input_begin() {
    nk_input_begin(ctx);
}

void nk_glfw_input_end() {
    nk_input_end(ctx);
}

#endif