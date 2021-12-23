#define NK_IMPLEMENTATION

#include <nuklear_args.h>
#include <nuklear.h>
#ifdef NK_USE_GL3
#include <glad/glad.h>
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear_glfw_gl3.h>
#else
#include <glad/glad.h>
#include <sokol_args.h>
#include <sokol_gfx.h>
#define SOKOL_NUKLEAR_IMPL
#include <nuklear_sokol.h>
#define NUKLEAR_GLFW_SOKOL_IMPLEMENTATION
#include <nuklear_glfw_sokol.h>
#endif