#include "sokol.h"

#include "opengl.h"

#include <sokol_args.h>
#include <sokol_gfx.h>

void vx_context_load_sokol(GLFWwindow* window) {
#ifdef SOKOL_GLCORE33
    vx_context_load_opengl(window);
#endif

    sg_setup(&(sg_desc){ 0 });
}