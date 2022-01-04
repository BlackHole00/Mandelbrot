#include "mb_global.h"

#include <vx_utils.h>

#include <nuklear_args.h>
#include <nuklear.h>
#include <nuklear_sokol.h>

void mb_global_init(mb_GeneralData* general_data) {
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
    general_data->gfxData.vertexBuffer = sg_make_buffer(&(sg_buffer_desc){
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
    general_data->gfxData.indexBuffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE
    });

    char* vs_source = vx_filepath_get_content("res/shaders/mandelbrot.vs");
    char* fs_source = vx_filepath_get_content("res/shaders/mandelbrot.fs");

    /* a shader */
    general_data->gfxData.shader = sg_make_shader(&(sg_shader_desc){
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
    general_data->gfxData.pipelines = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = general_data->gfxData.shader,
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
    general_data->gfxData.bindings = (sg_bindings){
        .vertex_buffers[0] = general_data->gfxData.vertexBuffer,
        .index_buffer = general_data->gfxData.indexBuffer
    };

    /* default pass action (clear to grey) */
    general_data->gfxData.passAction = (sg_pass_action){ 0 };

    general_data->transformBlock.model = HMM_Mat4d(1.0f);
}

void mb_global_close(mb_GeneralData* general_data) {
    sg_destroy_buffer(general_data->gfxData.vertexBuffer);
    sg_destroy_buffer(general_data->gfxData.indexBuffer);
    sg_destroy_pipeline(general_data->gfxData.pipelines);
    sg_destroy_shader(general_data->gfxData.shader);
}