add_library(Logic
    logic.h
    transform.h transform.c
    components/components.h
    components/position.h
    components/rotation.h
)

target_include_directories(Logic PUBLIC ..)

target_link_libraries(Logic PRIVATE VxUtils)
target_link_libraries(Logic PRIVATE Sokol)
target_link_libraries(Logic PRIVATE glfw)
target_link_libraries(Logic PRIVATE Nuklear)
target_link_libraries(Logic PRIVATE Glad)
target_link_libraries(Logic PRIVATE HandmadeMath)