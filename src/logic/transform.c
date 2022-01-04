#include "transform.h"

#include <hmm_helper.h>

void vx_position_set(vx_Position* position, hmm_vec3 new_position) {
    position->position_vector = new_position;
}

void vx_position_move(vx_Position* position, hmm_vec3 offset, f32 amount) {
    hmm_vec3 tmp = HMM_MultiplyVec3f(offset, amount);
    position->position_vector = HMM_AddVec3(position->position_vector, tmp);
}

void vx_position_move_cross(vx_Position* position, const vx_Rotation* rotation, hmm_vec3 cross_vector, f32 amount) {
    /**  @todo: Do not use HMM_Vec3(0.0f, 1.0f, 0.0f) as world top, calculate it using the rotation.    */
    hmm_vec3 tmp = HMM_NormalizeVec3(HMM_Cross(rotation->front_vector, cross_vector));

    vx_position_move(position, tmp, amount);
}

void vx_position_move_forward(vx_Position* position, const vx_Rotation* rotation, f32 amount) {
    vx_position_move(position, rotation->front_vector, amount);
}

void vx_position_move_backward(vx_Position* position, const vx_Rotation* rotation, f32 amount) {
    vx_position_move_forward(position, rotation, -amount);
}

void vx_position_move_right(vx_Position* position, const vx_Rotation* rotation, f32 amount) {
    /**  @todo: Do not use HMM_Vec3(0.0f, 1.0f, 0.0f) as world top, calculate it using the rotation.    */
    vx_position_move_cross(position, rotation, HMM_Vec3(0.0f, 1.0f, 0.0f), amount);
}

void vx_position_move_left(vx_Position* position, const vx_Rotation* rotation, f32 amount) {
    vx_position_move_right(position, rotation, -amount);
}

void vx_rotation_set(vx_Rotation* rotation, hmm_vec3 new_rotation) {
    rotation->rotation_vector = new_rotation;

    rotation->front_vector = HMM_Vec3Direction(rotation->rotation_vector);
}

void vx_rotation_rotate(vx_Rotation* rotation, hmm_vec3 offset, f32 amount) {
    hmm_vec3 tmp = HMM_MultiplyVec3f(offset, amount);
    rotation->rotation_vector = HMM_AddVec3(rotation->rotation_vector, tmp);

    if(rotation->rotation_vector.Y > 89.0f) {
        rotation->rotation_vector.Y = 89.0f;
    }
    if(rotation->rotation_vector.Y < -89.0f) {
        rotation->rotation_vector.Y = -89.0f;
    }

    rotation->front_vector = HMM_Vec3Direction(rotation->rotation_vector);
}