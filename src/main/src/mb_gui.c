#include "mb_gui.h"

#include <hmm_helper.h>

void mb_control_ui_common(struct nk_context* ctx, mb_GlobalData* global_data) {
    nk_layout_row_dynamic(ctx, 10, 1);
    nk_label(ctx, "position", NK_TEXT_LEFT);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 1);
    nk_layout_row_push(ctx, 1.0f);
    if (nk_button_label(ctx, "^")) {
        global_data->mandelbrot_info_block.y_position += 0.25f / global_data->mandelbrot_info_block.y_scale;
    }
    nk_layout_row_end(ctx);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
    nk_layout_row_push(ctx, 1.0f / 3.0f);
    if (nk_button_label(ctx, "<")) {
        global_data->mandelbrot_info_block.x_position -= 0.25f / global_data->mandelbrot_info_block.x_scale;
    }
    nk_layout_row_push(ctx, 1.0f / 3.0f);
    if (nk_button_label(ctx, "°")) {
        global_data->mandelbrot_info_block.y_position = 0;
        global_data->mandelbrot_info_block.x_position = 0;
        global_data->mandelbrot_info_block.x_scale = 1;
        global_data->mandelbrot_info_block.y_scale = 1;
    }
    nk_layout_row_push(ctx, 1.0f / 3.0f);
    if (nk_button_label(ctx, ">")) {
        global_data->mandelbrot_info_block.x_position += 0.25f / global_data->mandelbrot_info_block.x_scale;
    }
    nk_layout_row_end(ctx);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 1);
    nk_layout_row_push(ctx, 1.0f);
    if (nk_button_label(ctx, "V")) {
        global_data->mandelbrot_info_block.y_position -= 0.25f / global_data->mandelbrot_info_block.y_scale;
    }
    nk_layout_row_end(ctx);

    nk_layout_row_dynamic(ctx, 10, 1);
    nk_spacing(ctx, 1);
    nk_label(ctx, "scale", NK_TEXT_LEFT);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
    nk_layout_row_push(ctx, 0.5f);
    if (nk_button_label(ctx, "-")) {
        global_data->mandelbrot_info_block.x_scale -= 0.25f * global_data->mandelbrot_info_block.x_scale;
        global_data->mandelbrot_info_block.y_scale -= 0.25f * global_data->mandelbrot_info_block.y_scale;
    }
    nk_layout_row_push(ctx, 1.0f);
    if (nk_button_label(ctx, "+")) {
        global_data->mandelbrot_info_block.x_scale += 0.25f * global_data->mandelbrot_info_block.x_scale;
        global_data->mandelbrot_info_block.y_scale += 0.25f * global_data->mandelbrot_info_block.y_scale;
    }
    nk_layout_row_end(ctx);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
    nk_layout_row_push(ctx, 0.45f);
    nk_label(ctx, "iterations: ", NK_TEXT_LEFT);
    nk_layout_row_push(ctx, 1.0f);
    nk_slider_float(ctx, 10.0f, &global_data->mandelbrot_info_block.max_iterations, 2000.0f, 1.0f);
    nk_layout_row_end(ctx);

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
    nk_layout_row_push(ctx, 0.45f);
    nk_label(ctx, "max value: ", NK_TEXT_LEFT);
    nk_layout_row_push(ctx, 1.0f);
    nk_slider_float(ctx, 1.0f, &global_data->mandelbrot_info_block.max_value, 4.0f, 0.001f);
    nk_layout_row_end(ctx);

    if (!global_data->disco_mode) {
        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "color: ", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 200, 1);
        nk_layout_row_push(ctx, 1.0f);
        global_data->mandelbrot_info_block.color = nk_color_picker(ctx, global_data->mandelbrot_info_block.color, NK_RGBA);
        nk_layout_row_end(ctx);
    } else {
        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.45f);
        nk_label(ctx, "disco speed: ", NK_TEXT_LEFT);
        nk_layout_row_push(ctx, 1.0f);
        nk_slider_float(ctx, 0.0f, &global_data->disco_speed, 5.0f, 0.01f);
        nk_layout_row_end(ctx);
    }
    nk_layout_row_dynamic(ctx, 10, 1);
    nk_checkbox_label(ctx, "disco", (nk_bool*)(&global_data->disco_mode));

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
    nk_layout_row_push(ctx, 0.45f);
    nk_label(ctx, "bloom: ", NK_TEXT_LEFT);
    nk_layout_row_push(ctx, 1.0f);
    nk_slider_float(ctx, 1.0f, &global_data->bloom, 50.0f, 0.1f);
    nk_layout_row_end(ctx);

    nk_layout_row_dynamic(ctx, 10, 1);
    nk_spacing(ctx, 1);
    nk_label(ctx, "mode: ", NK_TEXT_LEFT);
}

vx_StateUID mb_control_ui_mode_selector(struct nk_context* ctx, vx_StateUID current_UID) {
    vx_StateUID returnState = current_UID;

    nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
    nk_layout_row_push(ctx, 0.5f);
    if (nk_option_label(ctx, "2D View", returnState == MB_MODE_2D)) {
        returnState = MB_MODE_2D;
    }
    nk_layout_row_push(ctx, 1.0f);
    if (nk_option_label(ctx, "3D View", returnState == MB_MODE_3D)) {
        returnState = MB_MODE_3D;
    }
    nk_layout_row_end(ctx);

    return returnState;
}

vx_StateUID mb_control_ui_mode2d(struct nk_context* ctx, mb_GlobalData* global_data, mb_Mode2DData* state) {
    vx_StateUID returnState = MB_MODE_2D;

    if (nk_begin(ctx, "Mandelbrot - Press 'G' to Hide", nk_rect(50, 50, 250, 310),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)
    ) {
        mb_control_ui_common(ctx, global_data);
        returnState = mb_control_ui_mode_selector(ctx, MB_MODE_2D);
    }
    nk_end(ctx);

    return returnState;
}

vx_StateUID mb_control_ui_mode3d(struct nk_context* ctx, mb_GlobalData* global_data, mb_Mode3DData* state) {
    vx_StateUID returnState = MB_MODE_3D;

    if (nk_begin(ctx, "Mandelbrot - Press 'G' to Hide", nk_rect(50, 50, 250, 310),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)
    ) {
        mb_control_ui_common(ctx, global_data);
        returnState = mb_control_ui_mode_selector(ctx, MB_MODE_3D);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "3D:", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.25f);
        nk_label(ctx, "fov: ", NK_TEXT_LEFT);
        nk_layout_row_push(ctx, 1.0f);
        if (nk_slider_float(ctx, 10.0f, &state->camera.fov, 120.0f, 1.0f));
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
        nk_layout_row_push(ctx, 0.25f);
        nk_label(ctx, "speed: ", NK_TEXT_LEFT);
        nk_layout_row_push(ctx, 1.0f);
        nk_slider_float(ctx, 0.5f, &state->movement_speed, 5.0f, 0.1f);
        nk_layout_row_end(ctx);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_label(ctx, "Positions: ", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.5f);
        if (nk_button_label(ctx, "Move Inside")) {
            vx_position_set(&state->camera.position, HMM_Vec3(0.0f, 0.0f, 0.0f));
        }
        if (nk_button_label(ctx, "Default")) {
            vx_position_set(&state->camera.position, HMM_Vec3(0.0f, 0.0f, 6.0f));
            vx_rotation_set(&state->camera.rotation, HMM_Vec3(-90.0f, 0.0f, 0.0f));
            state->camera.fov = (float)90.0;
        }
        nk_layout_row_end(ctx);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_label(ctx, "BackgroundColor: ", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 200, 1);
        nk_layout_row_push(ctx, 1.0f);
        global_data->gfx_data.mode3d.bgColor = nk_color_picker(ctx, global_data->gfx_data.mode3d.bgColor, NK_RGBA);
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);

    return returnState;
}