#include "graphics.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

GLFWwindow *window;

asteroid_list_t *asteroids;

int main(int argc, char *argv[]) {
    srand(time(0));

    float camera_speed = 10.0f;
    camera_location.x = 0.5f;
    camera_location.y = 0.0f;
    camera_location.z = -20.0f;
    camera_angle = 0.0;

    asteroids = create_asteroid_list();
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            asteroids = asteroid_list_cons(create_asteroid((x-2)*2.0f, 0.0f, (y-2)*2.0f), asteroids);
        }
    }
    asteroid_t *sun = create_asteroid(10000.0f, 0.0f, 0.0f);

    for (int i = 0; i < 12; i++) {
        sun->normals[i].x = -sun->normals[i].x;
        sun->normals[i].y = -sun->normals[i].y;
        sun->normals[i].z = -sun->normals[i].z;
        sun->vertices[i].x = 100.0f*sun->vertices[i].x;
        sun->vertices[i].y = 100.0f*sun->vertices[i].y;
        sun->vertices[i].z = 100.0f*sun->vertices[i].z;
    }

    asteroids = asteroid_list_cons(sun, asteroids);

    /* asteroids = asteroid_list_cons(create_asteroid(-3.0f, 1.0f, 0.0f), asteroids); */
    /* asteroids = asteroid_list_cons(create_asteroid(2.0f, -1.0f, 2.0f), asteroids); */


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
            camera_location.z += (float) delta * camera_speed * cos(camera_angle);
            camera_location.x += (float) delta * camera_speed * sin(camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_location.z -= (float) delta * camera_speed * cos(camera_angle);
            camera_location.x -= (float) delta * camera_speed * sin(camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_location.x -= (float) delta * camera_speed * cos(-camera_angle);
            camera_location.z -= (float) delta * camera_speed * sin(-camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_location.x += (float) delta * camera_speed * cos(-camera_angle);
            camera_location.z += (float) delta * camera_speed * sin(-camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera_angle += (float) delta;
            camera_angle = fmod(camera_angle, 3.14159f * 2);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera_angle -= (float) delta;
            camera_angle = fmod(camera_angle, 3.14159f * 2);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera_location.y -= (float) delta * camera_speed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera_location.y += (float) delta * camera_speed;
        }

        collect_vertices(asteroids);
        render(window);
        glfwPollEvents();
    }

    return 0;
}
