add_library(Glad
    src/glad.c
    includes/glad/glad.h
    includes/KHR/khrplatform.h
)

target_include_directories(Glad PUBLIC includes)