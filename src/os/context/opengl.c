#include "opengl.h"

#include <stdio.h>
#include <glad/glad.h>
#include <vx_utils.h>

void vx_context_load_opengl(GLFWwindow* window) {
    /*  Make the window's context current   */
    glfwMakeContextCurrent(window);

    /*  Initialize GLAD so we can use modern OpenGL */
    VX_ASSERT("Could not Initialize GLAD!", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    printf("Using opengl %s\n", glGetString(GL_VERSION));
}