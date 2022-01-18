#include <vx_utils.h>
#include <os/os.h>

#include "mb_data_structures.h"
#include "mb_global.h"
#include "mb_mode2D.h"
#include "mb_mode3D.h"

int main() {
    /* Create a descriptor for the window. */
    vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
    descriptor.title = "Mandelbrot";
    descriptor.resizable    = true;
    descriptor.grab_cursor  = false;
    descriptor.width        = WINDOW_WIDTH;
    descriptor.height       = WINDOW_HEIGHT;
    descriptor.swap_interval = 1;

    /* Create a new window. */
    vx_Window window = vx_window_new(&descriptor);

    /* Create the various states' data. */
    mb_GlobalData data = VX_DEFAULT(mb_GlobalData);
    mb_Mode2DData mode_2d_data = VX_DEFAULT(mb_Mode2DData);
    mb_Mode3DData mode_3d_data = VX_DEFAULT(mb_Mode3DData);

    /* Create a state manager. */
    vx_StateManager manager = vx_statemanager_new(&(vx_StateManagerDescriptor){
        .first_init = mb_global_init,
        .close = mb_global_close,
        .global_data = &data,
    });

    /* Register the 2d mode state. */
    vx_statemanager_register_state(&manager, &(vx_StateDescriptor){
        .UID = MB_MODE_2D,
        .init = mb_mode2d_init,
        .logic = mb_mode2d_logic,
        .draw = mb_mode2d_draw,
        .resize = mb_mode2d_resize,
        .close = mb_mode2d_close,
        .user_data = &mode_2d_data,
    });

    /* Register the 3d mode state. */
    vx_statemanager_register_state(&manager, &(vx_StateDescriptor){
        .UID = MB_MODE_3D,
        .init = mb_mode3d_init,
        .logic = mb_mode3d_logic,
        .draw = mb_mode3d_draw,
        .resize = mb_mode3d_resize,
        .close = mb_mode3d_close,
        .user_data = &mode_3d_data,
    });

    /* Run the application. */
    vx_statemanager_run(&manager, &window, MB_MODE_2D);

    /* Free the state manager. */
    vx_statemanager_free(&manager);

    return 0;
}
