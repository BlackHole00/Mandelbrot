add_library(HandmadeMath
    includes/HandmadeMath.h
    includes/hmm_helper.h src/hmm_helper.c
)

target_include_directories(HandmadeMath PUBLIC includes)