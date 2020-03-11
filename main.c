#include "graphics.h"
#include "world.h"
#include <stdlib.h>

GLFWwindow *window;

asteroid_list_t *asteroids;

int main(int argc, char *argv[]) {

    camera_location.x = 0.5f;
    camera_location.y = 0.0f;
    camera_location.z = 5.0f;

    asteroids = create_asteroid_list();
    asteroids = asteroid_list_cons(create_asteroid(-0.5f, -0.2f, 0.0f), asteroids);
    asteroids = asteroid_list_cons(create_asteroid(0.3f, 0.1f, 0.1f), asteroids);


    int error = intialize_window(&window);
    if (error)
        return error;

    while(!glfwWindowShouldClose(window)) {
        collect_vertices(asteroids);
        render(window);
        glfwPollEvents();
    }

    return 0;
}
