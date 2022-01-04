#pragma once

#include <vx_default.h>
#include <HandmadeMath.h>
#include <sokol_args.h>
#include <sokol_gfx.h>
#include <nuklear_args.h>
#include <nuklear.h>
#include <gfx/camera.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MB_MODE_2D 0
#define MB_MODE_3D 1

typedef struct mb_UniformMandelbrotInfoBlock {
    float maxIterations;
    float xPosition;
    float yPosition;
    float xScale;
    float yScale;
    struct nk_colorf color;
    struct {
        f32 screenWidth;
        f32 screenHeight;
    } resolution;
    float maxValue;
} mb_UniformMandelbrotInfoBlock;
VX_CREATE_DEFAULT(mb_UniformMandelbrotInfoBlock, 
    .maxIterations = 500,
    .xPosition = 0.0f,
    .yPosition = 0.0f,
    .xScale = 1.0f,
    .yScale = 1.0f,
    .color = {
        .r = 1.0f,
        .g = 1.0f,
        .b = 1.0f,
        .a = 1.0f,
    },
    .maxValue = 2.0f
)

typedef struct mb_UniformTransformBlock {
    hmm_mat4 projection;
    hmm_mat4 view;
    hmm_mat4 model;
} mb_UniformTransformBlock;
VX_CREATE_DEFAULT(mb_UniformTransformBlock,
    .projection = { 0 },
    .model = { 0 },
    .view = { 0 }
)

typedef struct mb_GfxData {
    float resolutionMultiplier;

    sg_buffer vertexBuffer;
    sg_buffer indexBuffer;
    sg_shader shader;
    sg_pipeline pipelines;
    sg_bindings bindings;
    sg_pass_action passAction;

    u32 screenWidth;
    u32 screenHeight;

    struct {
        struct nk_colorf bgColor;
    } mode3d;
} mb_GfxData;
VX_CREATE_DEFAULT(mb_GfxData,
    .screenWidth = WINDOW_WIDTH,
    .screenHeight = WINDOW_HEIGHT,
    .mode3d.bgColor = { 0.75f, 0.75f, 0.75f },
    .resolutionMultiplier = 1.0f,
)

typedef struct mb_GeneralData {
    mb_UniformMandelbrotInfoBlock mandelbrotInfoBlock;
    mb_UniformTransformBlock transformBlock;
    mb_GfxData gfxData;
} mb_GeneralData;
VX_CREATE_DEFAULT(mb_GeneralData,
    .transformBlock = VX_DEFAULT(mb_UniformTransformBlock),
    .mandelbrotInfoBlock = VX_DEFAULT(mb_UniformMandelbrotInfoBlock),
    .gfxData = VX_DEFAULT(mb_GfxData)
)

typedef struct mb_Mode2DData {
    vx_Camera camera;
} mb_Mode2DData;
VX_CREATE_DEFAULT(mb_Mode2DData)

typedef struct mb_Mode3DData {
    vx_Camera camera;
    f32 movement_speed;
} mb_Mode3DData;
VX_CREATE_DEFAULT(mb_Mode3DData,
    .movement_speed = 1.0f
)
