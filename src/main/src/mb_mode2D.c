#include "mb_mode2D.h"

#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>
#include <nuklear_glfw_sokol.h>
#include <os/nuklear/input_helper.h>

#include "mb_input.h"
#include "mb_gui.h"
#include "mb_global.h"

void mb_mode2d_init(mb_GlobalData* global_data, mb_Mode2DData* state, vx_WindowControl* window) {
    vx_windowcontrol_set_mouse_grab(window, false);
    global_data->mandelbrot_info_block.resolution_rateo = (f32)window->window_size.width / (f32)window->window_size.height;

    state->camera = vx_camera_new(&(vx_CameraDescriptor){
        .camera_type = VX_CAMERATYPE_ORTHOGRAPHIC,
        .position = { 0, 0, 6 },
        .rotation = { -90, 0 },
        .left = -1.0f,
        .right = 1.0f,
        .top = 1.0,
        .bottom = -1.0f,
        .near = 0.001f,
        .far = 100.0f
    });
}

vx_StateUID mb_mode2d_logic(mb_GlobalData* global_data, mb_Mode2DData* state, vx_WindowControl* window, vx_WindowInputHelper* input) {
    vx_StateUID returnUID = MB_MODE_2D;

    struct nk_context *ctx = snk_new_frame();
    nk_glfw_set_context(ctx);
    returnUID = mb_control_ui_mode2d(ctx, global_data, state);

    if (input->keys[GLFW_KEY_G].just_pressed) {
        global_data->gui_enabled = !global_data->gui_enabled;
    }

    if ((global_data->gui_enabled && !vx_inputhelper_update_nuklear_input(input, false)) || !global_data->gui_enabled) {
        mb_input_mode2d(global_data, input);
    }

    if(input->keys[GLFW_KEY_ESCAPE].just_pressed) {
        vx_windowcontrol_exit(window);
    }

    if (global_data->disco_mode) {
        mb_shared_disco_update_colors(global_data);
    }

    return returnUID;
}

void mb_mode2d_draw(mb_GlobalData* global_data, mb_Mode2DData* state) {
    global_data->gfx_data.pass_action.colors[0].action = SG_ACTION_CLEAR;
    global_data->gfx_data.pass_action.colors[0].value = *((sg_color*)&global_data->gfx_data.mode3d.bgColor);

    vx_camera_apply(&state->camera, &global_data->transfrom_block.projection, &global_data->transfrom_block.view, true);

    sg_begin_default_pass(&global_data->gfx_data.pass_action, global_data->gfx_data.screenWidth, global_data->gfx_data.screenHeight);
    sg_apply_pipeline(global_data->gfx_data.pipelines);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &SG_RANGE(global_data->transfrom_block));

    /* Apply bloom */
    struct nk_colorf backup = global_data->mandelbrot_info_block.color;
    global_data->mandelbrot_info_block.color.r *= global_data->bloom;
    global_data->mandelbrot_info_block.color.g *= global_data->bloom;
    global_data->mandelbrot_info_block.color.b *= global_data->bloom;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(global_data->mandelbrot_info_block));
    global_data->mandelbrot_info_block.color = backup;

    sg_apply_bindings(&global_data->gfx_data.bindings);
    sg_draw(0, 6, 1);
    if (global_data->gui_enabled) {
        snk_render(global_data->gfx_data.screenWidth, global_data->gfx_data.screenHeight);
    }
    sg_end_pass();
    sg_commit();
}

void mb_mode2d_resize(mb_GlobalData* global_data, mb_Mode2DData* state, vx_WindowControl* window) {
    global_data->gfx_data.screenWidth = window->window_size.width;
    global_data->gfx_data.screenHeight = window->window_size.height;
    global_data->mandelbrot_info_block.resolution_rateo = (f32)window->window_size.width / (f32)window->window_size.height;
    vx_camera_set_viewport_size(&state->camera, window->window_size.width, window->window_size.height);
}

void mb_mode2d_close(mb_GlobalData* global_data, mb_Mode2DData* state, vx_WindowControl* window) {}