#include "mb_global.h"

#include <vx_utils.h>

#include <sokol_gfx.h>
#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>

void mb_global_init(mb_GlobalData* global_data) {
    /* Nuklear (GUI) initialization. */
    snk_setup(&(snk_desc_t){ 0 });

    /* The CUBE vertex data. */
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
    /* Upload the buffer to the GPU. */
    global_data->gfx_data.vertex_buffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    /* The CUBE index data. */
    uint16_t indices[] = {
        0,  1,  2,   0,  2,  3,
        6,  5,  4,   7,  6,  4,
        8,  9,  10,  8,  10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20
    };
    global_data->gfx_data.index_buffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    /* Get the sources of the shaders. */
    char* vs_source = vx_filepath_get_content("res/shaders/mandelbrot.vs");
    char* fs_source = vx_filepath_get_content("res/shaders/mandelbrot.fs");
    VX_ASSERT("vs_source is NULL. Does the source file exists?", vs_source != NULL);
    VX_ASSERT("fs_source is NULL. Does the source file exists?", fs_source != NULL);

    /* Tell the GPU how to use the sources and how to read the GPU shared data (uniforms, not buffers). */
    global_data->gfx_data.shader = sg_make_shader(&(sg_shader_desc){
        .vs = {
            .source = vs_source,
            .uniform_blocks[0] = {
                .size = sizeof(mb_Uniformtransfrom_block),
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
                .size = sizeof(mb_Uniformmandelbrot_info_block),
                .uniforms = {
                    [0] = {
                        .name = "max_iterations",
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
                        .name = "resolution_rateo",
                        .type = SG_UNIFORMTYPE_FLOAT,
                    },
                    [5] = {
                        .name = "max_value",
                        .type = SG_UNIFORMTYPE_FLOAT,
                    }
                }
            }
        }
    });

    free(vs_source);
    free(fs_source);

    /* a pipeline state object. Tell the GPU how to draw and how to interpret the buffers data. */
    global_data->gfx_data.pipelines = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = global_data->gfx_data.shader,
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
    global_data->gfx_data.bindings = (sg_bindings){
        .vertex_buffers[0] = global_data->gfx_data.vertex_buffer,
        .index_buffer = global_data->gfx_data.index_buffer
    };

    /* default pass action */
    global_data->gfx_data.pass_action = (sg_pass_action){ 0 };

    global_data->transfrom_block.model = HMM_Mat4d(1.0f);
}

void mb_global_close(mb_GlobalData* global_data) {
    /* Free the resources used. */
    sg_destroy_buffer(global_data->gfx_data.vertex_buffer);
    sg_destroy_buffer(global_data->gfx_data.index_buffer);
    sg_destroy_pipeline(global_data->gfx_data.pipelines);
    sg_destroy_shader(global_data->gfx_data.shader);
}

void mb_shared_disco_update_colors(mb_GlobalData* global_data) {
    global_data->mandelbrot_info_block.color.r += 1.0f/255.0f * global_data->disco_speed * (global_data->color_modes.r ? -1.0 : 1.0);
    global_data->mandelbrot_info_block.color.g += 1.0f/255.0f * global_data->disco_speed * 1.5 * (global_data->color_modes.g ? -1.0 : 1.0);
    global_data->mandelbrot_info_block.color.b += 1.0f/255.0f * global_data->disco_speed * 1.5 * 1.75 * (global_data->color_modes.b ? -1.0 : 1.0);

    /* Not going to 1.0 or 0.0 because bloom. */
    if (global_data->mandelbrot_info_block.color.r > 0.95f) {
        global_data->color_modes.r = true;
        global_data->mandelbrot_info_block.color.r = 0.95f;
    }
    if (global_data->mandelbrot_info_block.color.g > 0.95f) {
        global_data->color_modes.g = true;
        global_data->mandelbrot_info_block.color.g = 0.95f;
    }
    if (global_data->mandelbrot_info_block.color.b > 0.95f) {
        global_data->color_modes.b = true;
        global_data->mandelbrot_info_block.color.b = 0.95f;
    }
    if (global_data->mandelbrot_info_block.color.r < 0.075f) {
        global_data->color_modes.r = false;
        global_data->mandelbrot_info_block.color.r = 0.05f;
    }
    if (global_data->mandelbrot_info_block.color.g < 0.05f) {
        global_data->color_modes.g = false;
        global_data->mandelbrot_info_block.color.g = 0.05f;
    }
    if (global_data->mandelbrot_info_block.color.b < 0.05f) {
        global_data->color_modes.b = false;
        global_data->mandelbrot_info_block.color.b = 0.05f;
    }
}