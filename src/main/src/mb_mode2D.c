#include "mb_mode2D.h"

#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>
#include <nuklear_glfw_sokol.h>
#include <os/nuklear/input_helper.h>

#include "mb_input.h"
#include "mb_gui.h"
#include "mb_global.h"

void mb_mode2d_init(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window) {
    vx_windowcontrol_set_mouse_grab(window, false);

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

vx_StateUID mb_mode2d_logic(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window, vx_WindowInputHelper* input) {
    vx_StateUID returnUID = MB_MODE_2D;

    struct nk_context *ctx = snk_new_frame();
    nk_glfw_set_context(ctx);
    returnUID = mb_control_ui_mode2d(ctx, general_data, state);

    if (input->keys[GLFW_KEY_G].just_pressed) {
        general_data->gui_enabled = !general_data->gui_enabled;
    }

    if ((general_data->gui_enabled && !vx_inputhelper_update_nuklear_input(input, false)) || !general_data->gui_enabled) {
        mb_input_mode2d(general_data, input);
    }

    if(input->keys[GLFW_KEY_ESCAPE].just_pressed) {
        vx_windowcontrol_exit(window);
    }

    if (general_data->disco_mode) {
        mb_shared_disco_update_colors(general_data);
    }

    return returnUID;
}

void mb_mode2d_draw(mb_GlobalData* general_data, mb_Mode2DData* state) {
    general_data->gfxData.passAction.colors[0].action = SG_ACTION_CLEAR;
    general_data->gfxData.passAction.colors[0].value = *((sg_color*)&general_data->gfxData.mode3d.bgColor);
    general_data->mandelbrotInfoBlock.resolution.screenWidth =  (float)general_data->gfxData.screenWidth * general_data->gfxData.resolutionMultiplier;
    general_data->mandelbrotInfoBlock.resolution.screenHeight = (float)general_data->gfxData.screenHeight * general_data->gfxData.resolutionMultiplier;

    vx_camera_apply(&state->camera, &general_data->transformBlock.projection, &general_data->transformBlock.view, true);

    sg_begin_default_pass(&general_data->gfxData.passAction, general_data->gfxData.screenWidth, general_data->gfxData.screenHeight);
    sg_apply_pipeline(general_data->gfxData.pipelines);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &SG_RANGE(general_data->transformBlock));

    /* Apply bloom */
    struct nk_colorf backup = general_data->mandelbrotInfoBlock.color;
    general_data->mandelbrotInfoBlock.color.r *= general_data->bloom;
    general_data->mandelbrotInfoBlock.color.g *= general_data->bloom;
    general_data->mandelbrotInfoBlock.color.b *= general_data->bloom;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(general_data->mandelbrotInfoBlock));
    general_data->mandelbrotInfoBlock.color = backup;

    sg_apply_bindings(&general_data->gfxData.bindings);
    sg_draw(0, 6, 1);
    if (general_data->gui_enabled) {
        snk_render(general_data->gfxData.screenWidth, general_data->gfxData.screenHeight);
    }
    sg_end_pass();
    sg_commit();
}

void mb_mode2d_resize(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window, u32 width, u32 height) {
    general_data->gfxData.screenWidth = width;
    general_data->gfxData.screenHeight = height;
}

void mb_mode2d_close(mb_GlobalData* general_data, mb_Mode2DData* state, vx_WindowControl* window) {}