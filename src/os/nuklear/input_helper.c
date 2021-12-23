#include <sokol_args.h>
#include <sokol_gfx.h>
#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>
#include <nuklear_glfw_sokol.h>
#include <vx_panic.h>

#include "input_helper.h"

bool vx_inputhelper_update_nuklear_input(vx_WindowInputHelper* input, bool always_enabled) {
    struct nk_context* ctx = nk_glfw_get_context();
    VX_NULL_CHECK(ctx, false);

    nk_glfw_input_begin();

    /*  Handle mouse position   */
    if (input->mouse.moved && !input->mouse.grabbed) {
        nk_input_motion(ctx, (int)input->mouse.pos_x, (int)input->mouse.pos_y);
    }

    if (!always_enabled && !nk_window_is_any_hovered(ctx)) {
        return false;
    }

    /*  Handle keys */
    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        /*  If the key is a character, put it in the text buffer if it is pressed.  */
        if (i >= 65 && i <= 90 && ctx->input.keyboard.text_len < NK_INPUT_MAX) {
            if (input->keys[i].pressed) {
                ctx->input.keyboard.text[ctx->input.keyboard.text_len] = (char)i;
                ctx->input.keyboard.text_len++;

                continue;
            }
        }

        /*  Check if the key is valid   */
        enum nk_keys key = nk_glfw_key_to_nuklear_key(i);
        if (key != NK_KEY_NONE) {
            ctx->input.keyboard.keys[key].down = input->keys[i].pressed;
            ctx->input.keyboard.keys[key].clicked = input->keys[i].just_pressed;
        }
    }

    /*  Handle mouse keys   */
    for (i32 i = 0; i < 1; i++) {
        ctx->input.mouse.buttons[nk_glfw_button_to_nuklear_button(i)].down = input->mouse.mouse_buttons[i].pressed;
        ctx->input.mouse.buttons[nk_glfw_button_to_nuklear_button(i)].clicked = input->mouse.mouse_buttons[i].just_pressed;
        if (input->mouse.mouse_buttons[i].just_pressed) {
            ctx->input.mouse.buttons[nk_glfw_button_to_nuklear_button(i)].clicked_pos.x = (float)input->mouse.pos_x;
            ctx->input.mouse.buttons[nk_glfw_button_to_nuklear_button(i)].clicked_pos.y = (float)input->mouse.pos_y;
        }
    }

    /*  Handle mouse scroll */
    if (input->mouse.scrolled) {
        ctx->input.mouse.scroll_delta.x = (float)input->mouse.scroll_offset_x;
        ctx->input.mouse.scroll_delta.y = (float)input->mouse.scroll_offset_y;
    }

    nk_glfw_input_end();

    return true;
}