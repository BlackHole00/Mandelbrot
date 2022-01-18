#include "mb_global.h"

#include <vx_utils.h>

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
    global_data->gfxData.vertexBuffer = sg_make_buffer(&(sg_buffer_desc){
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
    global_data->gfxData.indexBuffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    /* Get the sources of the shaders. */
    char* vs_source = vx_filepath_get_content("res/shaders/mandelbrot.vs");
    char* fs_source = vx_filepath_get_content("res/shaders/mandelbrot.fs");

    /* Tell the GPU how to use the sources and how to read the GPU shared data (uniforms, not buffers). */
    global_data->gfxData.shader = sg_make_shader(&(sg_shader_desc){
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

    /* a pipeline state object. Tell the GPU how to draw and how to interpret the buffers data. */
    global_data->gfxData.pipelines = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = global_data->gfxData.shader,
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
    global_data->gfxData.bindings = (sg_bindings){
        .vertex_buffers[0] = global_data->gfxData.vertexBuffer,
        .index_buffer = global_data->gfxData.indexBuffer
    };

    /* default pass action */
    global_data->gfxData.passAction = (sg_pass_action){ 0 };

    global_data->transformBlock.model = HMM_Mat4d(1.0f);
}

void mb_global_close(mb_GlobalData* global_data) {
    /* Free the resources used. */
    sg_destroy_buffer(global_data->gfxData.vertexBuffer);
    sg_destroy_buffer(global_data->gfxData.indexBuffer);
    sg_destroy_pipeline(global_data->gfxData.pipelines);
    sg_destroy_shader(global_data->gfxData.shader);
}