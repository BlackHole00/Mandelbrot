//------------------------------------------------------------------------------
//  triangle-glfw.c
//  Vertex buffer, simple shader, pipeline state object.
//------------------------------------------------------------------------------
#include <malloc.h>
#include <math.h>

#include <glad/glad.h>
#include <sokol_args.h>
#include <sokol_gfx.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>
#include <nuklear_glfw_sokol.h>
#include <HandmadeMath.h>
#include <vx_utils.h>
#include <os/os.h>
#include <os/nuklear/input_helper.h>

#include "mb_data_structures.h"
#include "mb_gui.h"
#include "mb_hmm_helper.h"
#include "mb_input.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

void mb_init(mb_State* state, GLFWwindow* window) {
    snk_setup(&(snk_desc_t){ 0 });

    /* a vertex buffer */
    const float vertices[] = {
        /* pos                   uvs */
        -1.0f, -1.0f,  1.0f,     -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,      1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,      1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,     -1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,      1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,     -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,     -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,      1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,      1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,      1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,     -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,     -1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,     -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,     -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,      1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,      1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,     -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,      1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,      1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,     -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,     -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,      1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,      1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,     -1.0f,  1.0f
    };
    state->gfxData.vertexBuffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    uint16_t indices[] = {
        0,  1,  2,   0,  2,  3,
        6,  5,  4,   7,  6,  4,
        8,  9,  10,  8,  10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20
    };
    state->gfxData.indexBuffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    char* vs_source = vx_filepath_get_content("res/shaders/mandelbrot.vs");
    char* fs_source = vx_filepath_get_content("res/shaders/mandelbrot.fs");

    /* a shader */
    state->gfxData.shader = sg_make_shader(&(sg_shader_desc){
        .vs = {
            .source = vs_source,
            .uniform_blocks[0] = {
                .size = sizeof(mb_UniformTransformBlock),
                .uniforms = {
                    [0] = {
                        .name = "projection",
                        .type = SG_UNIFORMTYPE_MAT4,
                    },
                    [1] = {
                        .name = "view",
                        .type = SG_UNIFORMTYPE_MAT4,
                    },
                    [2] = {
                        .name = "model",
                        .type = SG_UNIFORMTYPE_MAT4,
                    }
                }
            }
        },
        .fs = {
            .source = fs_source,
            .uniform_blocks[0] = {
                .size = sizeof(mb_UniformMandelbrotInfoBlock),
                .uniforms = {
                    [0] = {
                        .name = "maxIterations",
                        .type = SG_UNIFORMTYPE_FLOAT,
                    },
                    [1] = {
                        .name = "position",
                        .type = SG_UNIFORMTYPE_FLOAT2,
                    },
                    [2] = {
                        .name = "scale",
                        .type = SG_UNIFORMTYPE_FLOAT2,
                    },
                    [3] = {
                        .name = "color",
                        .type = SG_UNIFORMTYPE_FLOAT4,
                    },
                    [4] = {
                        .name = "resolution",
                        .type = SG_UNIFORMTYPE_FLOAT2,
                    },
                    [5] = {
                        .name = "maxValue",
                        .type = SG_UNIFORMTYPE_FLOAT,
                    }
                }
            }
        }
    });

    free(vs_source);
    free(fs_source);

    /* a pipeline state object (default render states are fine for triangle) */
    state->gfxData.pipelines = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = state->gfxData.shader,
        .index_type = SG_INDEXTYPE_UINT16,
        .layout = {
            .attrs = {
                [0].format = SG_VERTEXFORMAT_FLOAT3,
                [1].format = SG_VERTEXFORMAT_FLOAT2,
            }
        },
        .cull_mode = SG_CULLMODE_NONE,
        .depth = {
            .write_enabled = true,
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
        },
    });

    /* resource bindings */
    state->gfxData.bindings = (sg_bindings){
        .vertex_buffers[0] = state->gfxData.vertexBuffer,
        .index_buffer = state->gfxData.indexBuffer
    };

    /* default pass action (clear to grey) */
    state->gfxData.passAction = (sg_pass_action){ 0 };

    state->perspCameraData.position = HMM_Vec3(0.0f, 0.0f, 6.0f);
    state->perspCameraData.front = HMM_Vec2Direction(state->perspCameraData.rotation);
    if (state->mode == MODE_2D_VIEW) {
        state->transformBlock.projection = HMM_Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.001f, 100.0f);
        state->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_AddVec3(state->perspCameraData.position, state->perspCameraData.front), HMM_Vec3(0.0f, 1.0f, 0.0f));
    } else {
        state->transformBlock.projection = HMM_Perspective(state->perspCameraData.fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 100.0f);
        state->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    }
    state->transformBlock.model = HMM_Mat4d(1.0f);
}

void mb_logic(mb_State* state, vx_WindowControl* window, vx_WindowInputHelper* input) {
    static bool first_frame = true;

    struct nk_context *ctx = snk_new_frame();
    nk_glfw_set_context(ctx);
    mb_control_ui(ctx, state);

    if (state->modeJustChanged) {
        if (state->mode == MODE_2D_VIEW) {
            vx_windowcontrol_set_mouse_grab(window, false);
            state->perspCameraData.position = HMM_Vec3(0.0f, 0.0f, 6.0f);
            state->transformBlock.projection = HMM_Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.001f, 100.0f);
            state->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
            state->transformBlock.model = HMM_Mat4d(1.0f);
        } else {
            state->perspCameraData.rotation = HMM_Vec2(-90.0f, 0.0f);
            state->transformBlock.projection = HMM_Perspective(state->perspCameraData.fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 100.0f);
            state->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_AddVec3(state->perspCameraData.position, state->perspCameraData.front), HMM_Vec3(0.0f, 1.0f, 0.0f));
            state->transformBlock.model = HMM_Mat4d(1.0f);
        }
    }
    if (state->mode == MODE_3D_VIEW) {
        state->transformBlock.view = HMM_LookAt(state->perspCameraData.position, HMM_AddVec3(state->perspCameraData.position, state->perspCameraData.front), HMM_Vec3(0.0f, 1.0f, 0.0f));
        if (state->perspCameraData.fovChanged) {
            state->transformBlock.projection = HMM_Perspective(state->perspCameraData.fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001f, 100.0f);
            state->perspCameraData.fovChanged = false;
        }
    }

    if (!vx_inputhelper_update_nuklear_input(input, false)) {
        if (state->mode == MODE_2D_VIEW) {
            mb_input_mode2d(state, input);
        } else {    /*  MODE_3D_VIEW    */
            mb_input_mode3d(state, input, window);
        }
        mb_camera_controls_3dmode(state, input);
    }

    if(input->keys[GLFW_KEY_ESCAPE].just_pressed) {
        vx_windowcontrol_exit(window);
    }
}

void mb_draw(mb_State* state) {
    state->gfxData.passAction.colors[0].action = SG_ACTION_CLEAR;
    state->gfxData.passAction.colors[0].value = *((sg_color*)&state->gfxData.mode3d.bgColor);
    state->mandelbrotInfoBlock.resolution.screenWidth =  (float)state->gfxData.screenWidth * state->gfxData.resolutionMultiplier;
    state->mandelbrotInfoBlock.resolution.screenHeight = (float)state->gfxData.screenHeight * state->gfxData.resolutionMultiplier;

    sg_begin_default_pass(&state->gfxData.passAction, state->gfxData.screenWidth, state->gfxData.screenHeight);
    sg_apply_pipeline(state->gfxData.pipelines);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &SG_RANGE(state->transformBlock));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(state->mandelbrotInfoBlock));
    sg_apply_bindings(&state->gfxData.bindings);
    if (state->mode == MODE_2D_VIEW) {
        sg_draw(0, 6, 1);
    } else {
        sg_draw(0, 36, 1);
    }
    snk_render(state->gfxData.screenWidth, state->gfxData.screenHeight);
    sg_end_pass();
    sg_commit();
}

void mb_resize(mb_State* state, GLFWwindow* window, u32 width, u32 height) {
    state->gfxData.screenWidth = width;
    state->gfxData.screenHeight = height;
}

void mb_close(mb_State* state, GLFWwindow* window) {
    sg_destroy_buffer(state->gfxData.vertexBuffer);
    sg_destroy_buffer(state->gfxData.indexBuffer);
    sg_destroy_pipeline(state->gfxData.pipelines);
    sg_destroy_shader(state->gfxData.shader);
}

int main() {
    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title = "Mandelbrot";
    descriptor.resizable    = true;
    descriptor.grab_cursor  = false;
    descriptor.width        = WINDOW_WIDTH;
    descriptor.height       = WINDOW_HEIGHT;
    descriptor.swap_interval = 1;
    descriptor.init         = (vx_Callback)mb_init;
    descriptor.logic        = (vx_Callback)mb_logic;
    descriptor.draw         = (vx_Callback)mb_draw;
    descriptor.resize       = (vx_Callback)mb_resize;
    descriptor.close        = (vx_Callback)mb_close;

    vx_Window window = vx_window_new(&descriptor);

    mb_State state = VX_DEFAULT(mb_State);
    vx_window_run(&window, (vx_UserStatePtr)&state);

    return 0;
}
