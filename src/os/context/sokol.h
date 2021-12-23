/** 
 * @file os/context/sokol.h
 * @author Vicix
 * @date 23/12/2021
 */
#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/**
* @brief Loads a sokol context. Can call other api's creation context functions.
* @param window A window.
*/
void vx_context_load_sokol(GLFWwindow* window);