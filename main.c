#include "graphics.h"
#include "world.h"
#include <stdlib.h>

GLFWwindow *window;

asteroid_list_t *asteroids;

int main(int argc, char *argv[]) {

    camera_location.x = 0.5f;
    camera_location.y = 0.0f;
    camera_location.z = -5.0f;

    asteroids = create_asteroid_list();
    asteroids = asteroid_list_cons(create_asteroid(-0.5f, -0.2f, 0.0f), asteroids);
    asteroids = asteroid_list_cons(create_asteroid(0.3f, 0.1f, 0.1f), asteroids);


    int error = intialize_window(&window);
    if (error)
        return error;

    double new_time = 0.0d;
    double last_time = glfwGetTime();
    double delta = 0.0d;
    while(!glfwWindowShouldClose(window)) {
        // Get delta
        new_time = glfwGetTime();
        delta = new_time - last_time;
        last_time = new_time;

        // Handle keys
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_location.z += (float) delta;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_location.z -= (float) delta;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_location.x += (float) delta;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_location.x -= (float) delta;
        }


        collect_vertices(asteroids);
        render(window);
        glfwPollEvents();
    }

    return 0;
}
