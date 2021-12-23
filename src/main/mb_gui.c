#include "mb_gui.h"

#include "mb_hmm_helper.h"

void mb_control_ui(struct nk_context* ctx, mb_State* state) {
    if (nk_begin(ctx, "Zoom", nk_rect(50, 50, 250, 310),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)
    ) {
        nk_layout_row_dynamic(ctx, 10, 1);
        nk_label(ctx, "position", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 1);
        nk_layout_row_push(ctx, 1.0f);
        if (nk_button_label(ctx, "^")) {
            state->mandelbrotInfoBlock.yPosition += 0.25f / state->mandelbrotInfoBlock.yScale;
        }
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
        nk_layout_row_push(ctx, 1.0f / 3.0f);
        if (nk_button_label(ctx, "<")) {
            state->mandelbrotInfoBlock.xPosition -= 0.25f / state->mandelbrotInfoBlock.xScale;
        }
        nk_layout_row_push(ctx, 1.0f / 3.0f);
        if (nk_button_label(ctx, "°")) {
            state->mandelbrotInfoBlock.yPosition = 0;
            state->mandelbrotInfoBlock.xPosition = 0;
            state->mandelbrotInfoBlock.xScale = 1;
            state->mandelbrotInfoBlock.yScale = 1;
        }
        nk_layout_row_push(ctx, 1.0f / 3.0f);
        if (nk_button_label(ctx, ">")) {
            state->mandelbrotInfoBlock.xPosition += 0.25f / state->mandelbrotInfoBlock.xScale;
        }
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 1);
        nk_layout_row_push(ctx, 1.0f);
        if (nk_button_label(ctx, "V")) {
            state->mandelbrotInfoBlock.yPosition -= 0.25f / state->mandelbrotInfoBlock.yScale;
        }
        nk_layout_row_end(ctx);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "scale", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.5f);
        if (nk_button_label(ctx, "-")) {
            state->mandelbrotInfoBlock.xScale -= 0.25f * state->mandelbrotInfoBlock.xScale;
            state->mandelbrotInfoBlock.yScale -= 0.25f * state->mandelbrotInfoBlock.yScale;
        }
        nk_layout_row_push(ctx, 1.0f);
        if (nk_button_label(ctx, "+")) {
            state->mandelbrotInfoBlock.xScale += 0.25f * state->mandelbrotInfoBlock.xScale;
            state->mandelbrotInfoBlock.yScale += 0.25f * state->mandelbrotInfoBlock.yScale;
        }
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.45f);
        nk_label(ctx, "iterations: ", NK_TEXT_LEFT);
        nk_layout_row_push(ctx, 1.0f);
        nk_slider_float(ctx, 10.0f, &state->mandelbrotInfoBlock.maxIterations, 2000.0f, 1.0f);
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.45f);
        nk_label(ctx, "max value: ", NK_TEXT_LEFT);
        nk_layout_row_push(ctx, 1.0f);
        nk_slider_float(ctx, 1.0f, &state->mandelbrotInfoBlock.maxValue, 4.0f, 0.001f);
        nk_layout_row_end(ctx);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "color: ", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 200, 1);
        nk_layout_row_push(ctx, 1.0f);
        state->mandelbrotInfoBlock.color = nk_color_picker(ctx, state->mandelbrotInfoBlock.color, NK_RGBA);
        nk_layout_row_end(ctx);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "mode: ", NK_TEXT_LEFT);

        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
        nk_layout_row_push(ctx, 0.5f);
        if (nk_option_label(ctx, "2D View", state->mode == MODE_2D_VIEW)) {
            state->modeJustChanged = (state->mode != MODE_2D_VIEW);
            state->mode = MODE_2D_VIEW;
        }
        nk_layout_row_push(ctx, 1.0f);
        if (nk_option_label(ctx, "3D View", state->mode == MODE_3D_VIEW)) {
            state->modeJustChanged = (state->mode != MODE_3D_VIEW);
            state->mode = MODE_3D_VIEW;
        }
        nk_layout_row_end(ctx);

        if (state->mode == MODE_3D_VIEW) {
            nk_layout_row_dynamic(ctx, 10, 1);
            nk_spacing(ctx, 1);
            nk_label(ctx, "3D:", NK_TEXT_LEFT);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
            nk_layout_row_push(ctx, 0.25f);
            nk_label(ctx, "fov: ", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 1.0f);
            if (nk_slider_float(ctx, 10.0f, &state->perspCameraData.fov, 120.0f, 1.0f)) {
                state->perspCameraData.fovChanged = true;
            }
            nk_layout_row_end(ctx);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(ctx, 0.25f);
            nk_label(ctx, "speed: ", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 1.0f);
            nk_slider_float(ctx, 0.005f, &state->perspCameraData.movementSpeed, 0.075f, 0.0005f);
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 10, 1);
            nk_label(ctx, "Positions: ", NK_TEXT_LEFT);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
            nk_layout_row_push(ctx, 0.5f);
            if (nk_button_label(ctx, "Move Inside")) {
                state->perspCameraData.position = HMM_Vec3(0.0f, 0.0f, 0.0f);
            }
            if (nk_button_label(ctx, "Default")) {
                state->perspCameraData.position = HMM_Vec3(0.0f, 0.0f, 6.0f);
                state->perspCameraData.rotation = HMM_Vec2(-90.0f, 0.0f);
                state->perspCameraData.front = HMM_Vec2Direction(state->perspCameraData.rotation);
                state->perspCameraData.fov = (float)90.0;
                state->perspCameraData.fovChanged = true;
            }
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 10, 1);
            nk_label(ctx, "BackgroundColor: ", NK_TEXT_LEFT);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 200, 1);
            nk_layout_row_push(ctx, 1.0f);
            state->gfxData.mode3d.bgColor = nk_color_picker(ctx, state->gfxData.mode3d.bgColor, NK_RGBA);
            nk_layout_row_end(ctx);
        }
    }
    nk_end(ctx);
}