#pragma once

#include <vx_default.h>
#include <HandmadeMath.h>
#include <sokol_args.h>
#include <sokol_gfx.h>
#include <nuklear_args.h>
#include <nuklear.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MODE_2D_VIEW 0
#define MODE_3D_VIEW 1

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

typedef struct mb_UniformTransformBlock {
    hmm_mat4 projection;
    hmm_mat4 view;
    hmm_mat4 model;
} mb_UniformTransformBlock;

typedef struct mb_PerspCameraData {
    hmm_vec3 position;
    hmm_vec2 rotation;
    hmm_vec3 front;
    float fov;
    bool fovChanged;
    float movementSpeed;
} mb_PerspCameraData;

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

typedef struct State {
    mb_UniformMandelbrotInfoBlock mandelbrotInfoBlock;
    mb_UniformTransformBlock transformBlock;
    mb_PerspCameraData perspCameraData;
    mb_GfxData gfxData;
    int mode;
    bool modeJustChanged;
} mb_State;
VX_CREATE_DEFAULT(mb_State,
    .mandelbrotInfoBlock = {
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
    },
    .transformBlock = {
        .projection = { 0 },
        .model = { 0 },
        .view = { 0 }
    },
    .perspCameraData = {
        .position = { 0 },
        .rotation = { -90, 0 },
        .front = { 0 },
        .fov = 90.0f,
        .fovChanged = false,
        .movementSpeed = 0.025f,
    },
    .gfxData = { 0 },
    .mode = MODE_2D_VIEW,
    .modeJustChanged = true,
    .gfxData.screenWidth = WINDOW_WIDTH,
    .gfxData.screenHeight = WINDOW_HEIGHT,
    .gfxData.mode3d.bgColor = { 0.75f, 0.75f, 0.75f },
    .gfxData.resolutionMultiplier = 1.0f,
)