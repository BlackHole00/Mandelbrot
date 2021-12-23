add_executable(Main
    main.c
    mb_data_structures.h
    mb_gui.h    mb_gui.c
    mb_hmm_helper.h mb_hmm_helper.c
    mb_input.h  mb_input.c
)

target_link_libraries(Main PRIVATE VxUtils)
target_link_libraries(Main PRIVATE Sokol)
target_link_libraries(Main PRIVATE glfw)
target_link_libraries(Main PRIVATE Nuklear)
target_link_libraries(Main PRIVATE Glad)
target_link_libraries(Main PRIVATE HandmadeMath)
target_link_libraries(Main PRIVATE Os)