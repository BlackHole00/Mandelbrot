add_executable(Main
    src/main.c
    src/mb_data_structures.h
    src/mb_gui.h    src/mb_gui.c
    src/mb_input.h  src/mb_input.c
    src/mb_mode2D.h src/mb_mode2D.c
    src/mb_mode3D.h src/mb_mode3D.c
    src/mb_global.h src/mb_global.c
)

target_link_libraries(Main PRIVATE VxLib)