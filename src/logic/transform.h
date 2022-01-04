#pragma once

#include <vx_types.h>
#include <HandmadeMath.h>
#include "components/components.h"

void vx_position_set(vx_Position* position, hmm_vec3 new_position);
void vx_position_move(vx_Position* position, hmm_vec3 offset, f32 amount);

void vx_position_move_cross(vx_Position* position, const vx_Rotation* rotation, hmm_vec3 cross_vector, f32 amount);

void vx_position_move_forward(vx_Position* position, const vx_Rotation* rotation, f32 amount);
void vx_position_move_backward(vx_Position* position, const vx_Rotation* rotation, f32 amount);
void vx_position_move_right(vx_Position* position, const vx_Rotation* rotation, f32 amount);
void vx_position_move_left(vx_Position* position, const vx_Rotation* rotation, f32 amount);

void vx_rotation_set(vx_Rotation* rotation, hmm_vec3 new_rotation);
void vx_rotation_rotate(vx_Rotation* rotation, hmm_vec3 offset, f32 amount);