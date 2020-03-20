#include "graphics.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

GLFWwindow *window;

asteroid_list_t *asteroids;
bullet_list_t *bullets;
ship_t *ship;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS){
        vec3 direction = {sin(camera_angle), 0.0f, -cos(camera_angle)};
        bullet_t *bullet = create_bullet(ship->location, direction);
        bullets = bullet_list_cons(bullet, bullets);
    }
}

int main(int argc, char *argv[]) {
    srand(time(0));

    float camera_speed = 10.0f;
    camera_location[0] = 0.5f;
    camera_location[1] = 0.0f;
    camera_location[2] = 20.0f;
    camera_angle = 0.0;

    ship = create_ship(camera_location, camera_location);

    bullets = create_bullet_list();

    asteroids = create_asteroid_list();
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            asteroids = asteroid_list_cons(create_asteroid((x-2)*2.0f, 0.0f, (y-2)*2.0f), asteroids);
        }
    }
    asteroid_t *sun = create_asteroid(10000.0f, 5000.0f, 0.0f);

    for (int i = 0; i < 12; i++) {
        sun->normals[i][0] = -sun->normals[i][0];
        sun->normals[i][1] = -sun->normals[i][1];
        sun->normals[i][2] = -sun->normals[i][2];
        sun->vertices[i][0] = 100.0f*sun->vertices[i][0];
        sun->vertices[i][1] = 100.0f*sun->vertices[i][1];
        sun->vertices[i][2] = 100.0f*sun->vertices[i][2];
    }

    asteroids = asteroid_list_cons(sun, asteroids);

    int error = intialize_window(&window);
    if (error)
        return error;
    glfwSetKeyCallback(window, key_callback);

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

        // Handle keys
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_location[2] -= (float) delta * camera_speed * cos(camera_angle);
            camera_location[0] += (float) delta * camera_speed * sin(camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_location[2] += (float) delta * camera_speed * cos(camera_angle);
            camera_location[0] -= (float) delta * camera_speed * sin(camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_location[0] += (float) delta * camera_speed * cos(-camera_angle);
            camera_location[2] -= (float) delta * camera_speed * sin(-camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_location[0] -= (float) delta * camera_speed * cos(-camera_angle);
            camera_location[2] += (float) delta * camera_speed * sin(-camera_angle);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera_angle -= (float) delta;
            camera_angle = fmod(camera_angle, 3.14159f * 2);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera_angle += (float) delta;
            camera_angle = fmod(camera_angle, 3.14159f * 2);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera_location[1] -= (float) delta * camera_speed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera_location[1] += (float) delta * camera_speed;
        }

        vec3 dir = {sin(camera_angle), 0.0f, -cos(camera_angle)};
        glm_vec3_copy(dir,ship->direction);
        glm_vec3_copy(camera_location, ship->location);
        glm_vec3_scale(dir, 4.0f, dir);
        glm_vec3_add(dir, ship->location, ship->location);        
        ship->location[1] -= 1.0f;
 
        collect_vertices(asteroids, bullets, ship);
        render(window);
        glfwPollEvents();
    }

    return 0;
}
