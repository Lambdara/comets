#include "graphics.h"

int vertices_length;
vector3f_t *vertices;

void render(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3f_t)*vertices_length, vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices_length);
    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glDeleteBuffers(1, &vbo);
}

void collect_vertices(asteroid_list_t* asteroids) {
    vertices_length = 0;

    asteroid_list_t *asteroids_head = asteroids;
    while (asteroids_head->next != NULL){
        vertices_length += 12;
        asteroids_head = asteroids_head->next;
    }

    vertices = malloc(sizeof(vector3f_t)*vertices_length);

    asteroids_head = asteroids;
    int i = 0;
    while (asteroids_head->this != NULL) {
        asteroid_t *asteroid = asteroids_head->this;

        vertices[i++] = asteroid->vertices[0];
        vertices[i++] = asteroid->vertices[1];
        vertices[i++] = asteroid->vertices[2];

        vertices[i++] = asteroid->vertices[0];
        vertices[i++] = asteroid->vertices[1];
        vertices[i++] = asteroid->vertices[3];

        vertices[i++] = asteroid->vertices[0];
        vertices[i++] = asteroid->vertices[2];
        vertices[i++] = asteroid->vertices[3];

        vertices[i++] = asteroid->vertices[1];
        vertices[i++] = asteroid->vertices[2];
        vertices[i++] = asteroid->vertices[3];

        asteroids_head = asteroids_head->next;
    }
}

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
