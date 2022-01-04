#pragma once

#include <HandmadeMath.h>

typedef struct vx_Position {
    hmm_vec3 position_vector;
} vx_Position;

static inline vx_Position vx_position_new(hmm_vec3 position_vector) {
    vx_Position position;

    position.position_vector = position_vector;

    return position;
}