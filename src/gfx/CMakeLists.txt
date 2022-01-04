add_library(Gfx
    gfx.h
    camera.h    camera.c
)

target_include_directories(Gfx PUBLIC ..)

target_link_libraries(Gfx PRIVATE VxUtils)
target_link_libraries(Gfx PRIVATE Sokol)
target_link_libraries(Gfx PRIVATE glfw)
target_link_libraries(Gfx PRIVATE Nuklear)
target_link_libraries(Gfx PRIVATE Glad)
target_link_libraries(Gfx PRIVATE HandmadeMath)