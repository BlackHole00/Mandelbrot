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
typedef struct mb_Uniformmandelbrot_info_block {
    float max_iterations;
    float x_position;
    float y_position;
    float x_scale;
    float y_scale;
    struct nk_colorf color;
    float resolution_rateo;
    float max_value;
} mb_Uniformmandelbrot_info_block;
VX_CREATE_DEFAULT(mb_Uniformmandelbrot_info_block, 
    .max_iterations = 500,
    .x_position = 0.0f,
    .y_position = 0.0f,
    .x_scale = 1.0f,
    .y_scale = 1.0f,
    .color = {
        .r = 1.0f,
        .g = 1.0f,
        .b = 1.0f,
        .a = 1.0f,
    },
    .resolution_rateo = (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT,
    .max_value = 2.0f
)

/**
 * This struct contains the data that is uploaded to the GPU in
 * order to apply the prospective or orthogonal view.
 */
typedef struct mb_Uniformtransfrom_block {
    hmm_mat4 projection;
    hmm_mat4 view;
    hmm_mat4 model;
} mb_Uniformtransfrom_block;
VX_CREATE_DEFAULT(mb_Uniformtransfrom_block,
    .projection = { 0 },
    .model = { 0 },
    .view = { 0 }
)

/**
 * This struct contains various stuff used to draw the set.
 */
typedef struct mb_GfxData {
    sg_buffer vertex_buffer;
    sg_buffer index_buffer;
    sg_shader shader;
    sg_pipeline pipelines;
    sg_bindings bindings;
    sg_pass_action pass_action;

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
    mb_Uniformmandelbrot_info_block mandelbrot_info_block;
    mb_Uniformtransfrom_block transfrom_block;
    mb_GfxData gfx_data;

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
    .transfrom_block = VX_DEFAULT(mb_Uniformtransfrom_block),
    .mandelbrot_info_block = VX_DEFAULT(mb_Uniformmandelbrot_info_block),
    .gfx_data = VX_DEFAULT(mb_GfxData),
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
