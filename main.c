#include "graphics.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

GLFWwindow *window;

asteroid_list_t *asteroids;
bullet_list_t *bullets;
ship_t *ship;

float max_distance = 1000.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS){
        bullet_t *bullet = create_bullet((vec3) {0.0f, 0.0f, 0.0f}, ship->direction);
        bullets = bullet_list_cons(bullet, bullets);
    }
}

int main(int argc, char *argv[]) {
    srand(time(0));

    int error = intialize_window(&window);
    if (error)
        return error;
    glfwSetKeyCallback(window, key_callback);

    ship = create_ship((vec3) {0.0f, 0.0f, -1.0f});

    bullets = create_bullet_list();

    asteroids = create_asteroid_list();
    for (int i = 0; i < 25; i++) {
        float longitude = rand() / (float) RAND_MAX * 3.14159 * 2;
        float colatitude = rand() / (float) RAND_MAX * 3.14159;
        float distance = rand() / (float) RAND_MAX * max_distance;
        asteroids = asteroid_list_cons(
                                       create_asteroid(
                                                       (vec3) {
                                                           distance * cos(longitude) * sin(colatitude),
                                                           distance * sin(longitude) * sin(colatitude),
                                                           distance * cos(colatitude)
                                                       },
                                                       5.0f),
                                       asteroids);
    }
    asteroid_t *sun = create_asteroid((vec3) {10000.0f, 5000.0f, 0.0f}, 100.0f);

    for (int i = 0; i < 12; i++) {
        sun->normals[i][0] = -sun->normals[i][0];
        sun->normals[i][1] = -sun->normals[i][1];
        sun->normals[i][2] = -sun->normals[i][2];
    }

    asteroids = asteroid_list_cons(sun, asteroids);

    double new_time = 0.0d;
    double last_time = glfwGetTime();
    double delta = 0.0d;
    while(!glfwWindowShouldClose(window)) {
        // Get delta
        new_time = glfwGetTime();
        delta = new_time - last_time;
        last_time = new_time;

        // Handle rotations
        asteroid_list_t *asteroids_head = asteroids;
        while (asteroids_head->next != NULL){
            asteroids_head->this->angle += asteroids_head->this->rotation_speed * delta;
            asteroids_head = asteroids_head->next;
        }

        // Move bullets
        bullet_list_t *bullets_head = bullets;
        while (bullets_head->next != NULL) {
            vec3 diff;
            glm_vec3_scale(bullets_head->this->direction, delta*bullets_head->this->speed, diff);
            glm_vec3_add(bullets_head->this->location, diff, bullets_head->this->location);

            bullets_head = bullets_head->next;
        }

        // Move asteroids
        asteroids_head = asteroids;
        while (asteroids_head->next != NULL) {
            vec3 diff;
            vec3 ship_diff;
            glm_vec3_scale(ship->direction, -ship->speed, ship_diff);
            glm_vec3_scale(asteroids_head->this->direction, delta*asteroids_head->this->speed, diff);
            glm_vec3_add(asteroids_head->this->location, diff, asteroids_head->this->location);
            glm_vec3_add(asteroids_head->this->location, ship_diff, asteroids_head->this->location);
            if (glm_vec3_norm(asteroids_head->this->location) > max_distance) {
                glm_vec3_negate(asteroids_head->this->location);
            }

            asteroids_head = asteroids_head->next;
        }

        // Handle keys
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            ship->speed += delta * 0.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            ship->speed -= delta * 0.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            glm_vec3_rotate(ship->direction, -delta, (vec3) {0.0f, 1.0f, 0.0f});
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            glm_vec3_rotate(ship->direction, delta, (vec3) {0.0f, 1.0f, 0.0f});
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            /* camera_angle -= (float) delta; */
            /* camera_angle = fmod(camera_angle, 3.14159f * 2); */
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            /* camera_angle += (float) delta; */
            /* camera_angle = fmod(camera_angle, 3.14159f * 2); */
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            /* camera_location[1] -= (float) delta * camera_speed; */
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            /* camera_location[1] += (float) delta * camera_speed; */
        }

        collect_vertices(asteroids, bullets, ship);
        render(window);
        glfwPollEvents();
    }

    return 0;
}
