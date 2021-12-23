add_library(Sokol
    includes/sokol_gfx.h
    includes/sokol_args.h
    src/sokol_impl.c
)
target_include_directories(Sokol PUBLIC includes)

target_link_libraries(Sokol PRIVATE Glad)