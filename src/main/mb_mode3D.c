#include "mb_mode3D.h"

#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>
#include <nuklear_glfw_sokol.h>
#include <os/nuklear/input_helper.h>

#include "mb_hmm_helper.h"
#include "mb_gui.h"
#include "mb_input.h"

void mb_mode3d_init(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window) {
    state->perspCameraData.position = HMM_Vec3(0.0f, 0.0f, 6.0f);
    state->perspCameraData.rotation = HMM_Vec2(-90.0f, 0.0f);
    state->perspCameraData.front = HMM_Vec2Direction(state->perspCameraData.rotation);
    general_data->transformBlock.projection = HMM_Perspective(state->perspCameraData.fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 100.0f);
    general_data->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_AddVec3(state->perspCameraData.position, state->perspCameraData.front), HMM_Vec3(0.0f, 1.0f, 0.0f));
    general_data->transformBlock.model = HMM_Mat4d(1.0f);
}

vx_StateUID mb_mode3d_logic(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window, vx_WindowInputHelper* input) {
    vx_StateUID returnUID;

    struct nk_context *ctx = snk_new_frame();
    nk_glfw_set_context(ctx);
    returnUID = mb_control_ui_mode3d(ctx, general_data, state);

    general_data->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_AddVec3(state->perspCameraData.position, state->perspCameraData.front), HMM_Vec3(0.0f, 1.0f, 0.0f));
    if (state->perspCameraData.fovChanged) {
        general_data->transformBlock.projection = HMM_Perspective(state->perspCameraData.fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 100.0f);
        state->perspCameraData.fovChanged = false;
    }

    if (!vx_inputhelper_update_nuklear_input(input, false)) {
        mb_input_mode3d(state, input, window);
        mb_camera_controls_3dmode(state, input);
    }

    if(input->keys[GLFW_KEY_ESCAPE].just_pressed) {
        vx_windowcontrol_exit(window);
    }

    return returnUID;
}

void mb_mode3d_draw(mb_GeneralData* general_data, mb_Mode3DData* state) {
    general_data->gfxData.passAction.colors[0].action = SG_ACTION_CLEAR;
    general_data->gfxData.passAction.colors[0].value = *((sg_color*)&general_data->gfxData.mode3d.bgColor);
    general_data->mandelbrotInfoBlock.resolution.screenWidth =  (float)general_data->gfxData.screenWidth * general_data->gfxData.resolutionMultiplier;
    general_data->mandelbrotInfoBlock.resolution.screenHeight = (float)general_data->gfxData.screenHeight * general_data->gfxData.resolutionMultiplier;

    sg_begin_default_pass(&general_data->gfxData.passAction, general_data->gfxData.screenWidth, general_data->gfxData.screenHeight);
    sg_apply_pipeline(general_data->gfxData.pipelines);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &SG_RANGE(general_data->transformBlock));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(general_data->mandelbrotInfoBlock));
    sg_apply_bindings(&general_data->gfxData.bindings);
    sg_draw(0, 36, 1);
    snk_render(general_data->gfxData.screenWidth, general_data->gfxData.screenHeight);
    sg_end_pass();
    sg_commit();
}

void mb_mode3d_resize(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window, u32 width, u32 height) {
    general_data->gfxData.screenWidth = width;
    general_data->gfxData.screenHeight = height;
}

void mb_mode3d_close(mb_GeneralData* general_data, mb_Mode3DData* state, vx_WindowControl* window) {}