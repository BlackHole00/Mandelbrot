add_library(Nuklear
    src/nuklear_impl.c
    includes/nuklear.h
    includes/nuklear_args.h
    includes/nuklear_glfw_gl3.h
    includes/nuklear_sokol.h
    includes/nuklear_sokol.h
)

target_compile_definitions(Nuklear PRIVATE _CRT_SECURE_NO_WARNINGS)

target_include_directories(Nuklear PUBLIC includes)

target_link_libraries(Nuklear PRIVATE glfw)
target_link_libraries(Nuklear PRIVATE Glad)
target_link_libraries(Nuklear PRIVATE Sokol)