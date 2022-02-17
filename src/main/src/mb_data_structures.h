/**
 * @file mb_data_structures.h
 * @brief This file holds all the data structures used in this project.
 * @author Vicix
 * @date 07/01/2022
 */
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

/**
 * Identifies the states. Used as a constant and as UID.
 */
typedef enum mb_ModeUID {
    MB_MODE_2D = 0,
    MB_MODE_3D = 1,
    MB_MODE_COUNT,
} mb_ModeUID;

/**
 * This struct contains the data that is uploaded to the GPU in
 * order to draw the mandelbrot set.
 */
typedef struct mb_UniformMandelbrotInfoBlock {
    float maxIterations;
    float xPosition;
    float yPosition;
    float xScale;
    float yScale;
    struct nk_colorf color;
    float resolution_rateo;
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
    .resolution_rateo = (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT,
    .maxValue = 2.0f
)

/**
 * This struct contains the data that is uploaded to the GPU in
 * order to apply the prospective or orthogonal view.
 */
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

/**
 * This struct contains various stuff used to draw the set.
 */
typedef struct mb_GfxData {
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
)

/**
 * Global data. Always available in every state.
 */
typedef struct mb_GlobalData {
    mb_UniformMandelbrotInfoBlock mandelbrotInfoBlock;
    mb_UniformTransformBlock transformBlock;
    mb_GfxData gfxData;

    bool gui_enabled;
    bool disco_mode;
    struct {
        bool r;
        bool g;
        bool b;
    } color_modes;
    f32 bloom;
    f32 disco_speed;
} mb_GlobalData;
VX_CREATE_DEFAULT(mb_GlobalData,
    .transformBlock = VX_DEFAULT(mb_UniformTransformBlock),
    .mandelbrotInfoBlock = VX_DEFAULT(mb_UniformMandelbrotInfoBlock),
    .gfxData = VX_DEFAULT(mb_GfxData),
    .gui_enabled = true,
    .disco_mode = false,
    .disco_speed = 1.0f,
    .color_modes = { 0 },
    .bloom = 1.0f
)

/**
 * Mode 2d data. The data used in the mode 2d state.
 */
typedef struct mb_Mode2DData {
    /**
     * Camera used in orthographic (2D) mode.
     */
    vx_Camera camera;
} mb_Mode2DData;
VX_CREATE_DEFAULT(mb_Mode2DData)

/**
 * Mode 3d data. The data used in the mode 3d state.
 */
typedef struct mb_Mode3DData {
    /**
     * Camera used in perspective (3D) mode.
     */
    vx_Camera camera;
    f32 movement_speed;
} mb_Mode3DData;
VX_CREATE_DEFAULT(mb_Mode3DData,
    .movement_speed = 1.0f
)
