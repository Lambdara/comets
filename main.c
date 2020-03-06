#include "graphics.h"
#include "world.h"
#include <stdlib.h>

GLFWwindow *window;

int main(int argc, char *argv[]) {

    int error = intialize_window(&window);
    if (error)
        return error;

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
