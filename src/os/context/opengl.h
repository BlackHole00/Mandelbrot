/** 
 * @file os/context/opengl.h
 * @author Vicix
 * @date 23/12/2021
 */
#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/**
* @brief Loads a modern opengl context.
* @param window A window.
*/
void vx_context_load_opengl(GLFWwindow* window);