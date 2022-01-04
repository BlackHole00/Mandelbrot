#include "../includes/hmm_helper.h"

hmm_vec3 HMM_Vec2Direction(hmm_vec2 v) {
    hmm_vec3 direction = { 0 };

    float yaw = v.X;
    float pitch = v.Y;

    direction.X = (float)cos((float)HMM_ToRadians(yaw)) * (float)cos((float)HMM_ToRadians(pitch));
    direction.Y = (float)sin((float)HMM_ToRadians(pitch));
    direction.Z = (float)sin((float)HMM_ToRadians(yaw)) * (float)cos((float)HMM_ToRadians(pitch));

    direction = HMM_NormalizeVec3(direction);

    return direction;
}

/** @todo Actually use z...
 */
hmm_vec3 HMM_Vec3Direction(hmm_vec3 v) {
    hmm_vec3 direction = { 0 };

    float yaw = v.X;
    float pitch = v.Y;

    direction.X = (float)cos((float)HMM_ToRadians(yaw)) * (float)cos((float)HMM_ToRadians(pitch));
    direction.Y = (float)sin((float)HMM_ToRadians(pitch));
    direction.Z = (float)sin((float)HMM_ToRadians(yaw)) * (float)cos((float)HMM_ToRadians(pitch));

    direction = HMM_NormalizeVec3(direction);

    return direction;
}