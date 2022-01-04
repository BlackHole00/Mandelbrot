#pragma once

#include <HandmadeMath.h>

#include <hmm_helper.h>

typedef struct vx_Rotation {
    hmm_vec3 rotation_vector;
    hmm_vec3 front_vector;
} vx_Rotation;

static inline vx_Rotation vx_rotation_new(hmm_vec3 rotation_vector) {
    vx_Rotation rotation;

    rotation.rotation_vector = rotation_vector;
    rotation.front_vector = HMM_Vec3Direction(rotation_vector);

    return rotation;
}