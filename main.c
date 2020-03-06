#include "graphics.h"
#include "world.h"
#include <stdlib.h>

GLFWwindow *window;

int main(int argc, char *argv[]) {

    // Stub to test world
    asteroid_list_t *asteroids = create_asteroid_list();
    asteroids = asteroid_list_cons(create_asteroid(0.3f, 0.4f, 0.2f), asteroids);
    asteroids = asteroid_list_cons(create_asteroid(0.1f, 0.7f, 0.1f), asteroids);

    asteroid_list_t *as = asteroids;
    while(as->this != NULL) {
        printf("(%f,%f,%f)\n", as->this->x, as->this->y, as->this->z);
        as = as->next;
    }

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
