add_library(Os
    os.h
    key_state.h
    window_descriptor.h
    window.h window.c
    window_control.h window_control.c
    input_helper.h input_helper.c
    state_manager.h state_manager.c
    nuklear/input_helper.h nuklear/input_helper.c
    context/opengl.h context/opengl.c
    context/sokol.h context/sokol.c
)

target_include_directories(Os PUBLIC ..)

target_link_libraries(Os PRIVATE VxUtils)
target_link_libraries(Os PRIVATE Sokol)
target_link_libraries(Os PRIVATE glfw)
target_link_libraries(Os PRIVATE Nuklear)
target_link_libraries(Os PRIVATE Glad)