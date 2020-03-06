#include "graphics.h"

void resize_framebuffer(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int intialize_window(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW failed to init\n");
        return 1;
    }

    *window = glfwCreateWindow(1920, 1080, "Particles", NULL, NULL);

    if (!*window) {
        fprintf(stderr, "Window failed to create\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, resize_framebuffer);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(res));
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 0;
}
