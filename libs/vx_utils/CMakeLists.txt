add_library(VxUtils
    src/vx_default.h
    src/vx_file.h       src/vx_file.c
    src/vx_functions.h  src/vx_functions.c
    src/vx_map.h
    src/vx_math.h       src/vx_math.c
    src/vx_mem.h        src/vx_mem.c
    src/vx_option.h
    src/vx_panic.h      src/vx_panic.c
    src/vx_template.h
    src/vx_types.h
    src/vx_utils.h
    src/vx_vector.h
)

target_compile_definitions(VxUtils PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(VxUtils PUBLIC VX_DO_NOT_USE_TYPEOF)

target_include_directories(VxUtils PUBLIC src)