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
#include "mb_global.h"
#include "mb_mode2D.h"
#include "mb_mode3D.h"

int main() {
    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title = "Mandelbrot";
    descriptor.resizable    = true;
    descriptor.grab_cursor  = false;
    descriptor.width        = WINDOW_WIDTH;
    descriptor.height       = WINDOW_HEIGHT;
    descriptor.swap_interval = 1;

    vx_Window window = vx_window_new(&descriptor);

    mb_GeneralData data = VX_DEFAULT(mb_GeneralData);
    mb_Mode2DData mode_2d_data = VX_DEFAULT(mb_Mode2DData);
    mb_Mode3DData mode_3d_data = VX_DEFAULT(mb_Mode3DData);

    vx_StateManager manager = vx_statemanager_new(&(vx_StateManagerDescriptor){
        .first_init = mb_global_init,
        .close = mb_global_close,
        .general_data = &data,
    });

    vx_statemanager_register_state(&manager, 0, &(vx_StateDescriptor){
        .init = mb_mode2d_init,
        .logic = mb_mode2d_logic,
        .draw = mb_mode2d_draw,
        .resize = mb_mode2d_resize,
        .close = mb_mode2d_close,
        .user_data = &mode_2d_data,
    });

    vx_statemanager_register_state(&manager, 1, &(vx_StateDescriptor){
        .init = mb_mode3d_init,
        .logic = mb_mode3d_logic,
        .draw = mb_mode3d_draw,
        .resize = mb_mode3d_resize,
        .close = mb_mode3d_close,
        .user_data = &mode_3d_data,
    });

    vx_statemanager_run(&manager, &window, 0);

    vx_statemanager_free(&manager);

    return 0;
}
