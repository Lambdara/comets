#include "graphics.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

#define ASTEROID_SIZE 24.0f
#define ASTEROID_VARIATION 12.0f
#define MINIMUM_COLLISION_DISTANCE 24.0f

GLFWwindow *window;

world_t *world;

float max_distance = 10000.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS){
        bullet_t *bullet = create_bullet((vec3) {0.0f, 0.0f, 0.0f}, world->ship->direction, 700.0+world->ship->speed);
        world->bullets = bullet_list_cons(bullet, world->bullets);
    }
}

int main(int argc, char *argv[]) {
    srand(time(0));

    world = create_world();

    int error = intialize_window(&window);
    if (error)
        return error;
    glfwSetKeyCallback(window, key_callback);

    for (int i = 0; i < 250; i++) {
        float longitude = rand() / (float) RAND_MAX * 3.14159 * 2;
        float colatitude = rand() / (float) RAND_MAX * 3.14159;
        float distance = rand() / (float) RAND_MAX * (max_distance - 1000.0f) + 1000.0f;
        world->asteroids = asteroid_list_cons(
                                       create_asteroid(
                                                       (vec3) {
                                                               distance * cos(longitude) * sin(colatitude),
                                                               distance * sin(longitude) * sin(colatitude),
                                                               distance * cos(colatitude)
                                                       },
                                                       ASTEROID_SIZE,
                                                       ASTEROID_VARIATION),
                                       world->asteroids);
    }
    /* asteroid_t *sun = create_asteroid((vec3) {10000.0f, 5000.0f, 0.0f}, 100.0f, 0.0f); */

    /* for (int i = 0; i < 1; i++) { */
    /*     sun->normals[i][0] = -sun->normals[i][0]; */
    /*     sun->normals[i][1] = -sun->normals[i][1]; */
    /*     sun->normals[i][2] = -sun->normals[i][2]; */
    /* } */

    /* world->asteroids = asteroid_list_cons(sun, world->asteroids); */

    double new_time = 0.0d;
    double last_time = glfwGetTime();
    double delta = 0.0d;
    while(!glfwWindowShouldClose(window)) {
        // Get delta
        new_time = glfwGetTime();
        delta = new_time - last_time;
        last_time = new_time;

        // Handle rotations
        asteroid_list_t *asteroids_head = world->asteroids;
        while (asteroids_head->next != NULL){
            asteroids_head->this->angle += asteroids_head->this->rotation_speed * delta;
            asteroids_head = asteroids_head->next;
        }

        vec3 ship_diff;
        glm_vec3_scale(world->ship->direction, -world->ship->speed*delta, ship_diff);

        // Move world->bullets
        bullet_list_t *bullets_head = world->bullets;
        bullet_list_t **link = &(world->bullets);
        while (bullets_head->next != NULL) {
            vec3 diff;
            glm_vec3_scale(bullets_head->this->direction, delta*bullets_head->this->speed, diff);
            glm_vec3_add(bullets_head->this->location, diff, bullets_head->this->location);
            glm_vec3_add(bullets_head->this->location, ship_diff, bullets_head->this->location);

            // Yes officer, this memory leak right here!
            if(glm_vec3_norm(bullets_head->this->location) > max_distance)
                *link = bullets_head->next;
            else
                link = &(bullets_head->next);
            bullets_head = bullets_head->next;
        }

        // Move world->asteroids
        asteroids_head = world->asteroids;
        while (asteroids_head->next != NULL) {
            vec3 diff;
            glm_vec3_scale(asteroids_head->this->direction, delta*asteroids_head->this->speed, diff);
            glm_vec3_add(asteroids_head->this->location, diff, asteroids_head->this->location);
            glm_vec3_add(asteroids_head->this->location, ship_diff, asteroids_head->this->location);
            if (glm_vec3_norm(asteroids_head->this->location) > max_distance) {
                glm_vec3_negate(asteroids_head->this->location);
            }

            asteroids_head = asteroids_head->next;
        }

        // Check for collisions
        asteroids_head = world->asteroids;
        asteroid_list_t **asteroids_link = &(world->asteroids);

        while(asteroids_head->next != NULL) {
            asteroid_t *asteroid = asteroids_head->this;
            int asteroid_destroyed = 0;

            for (int i = 0; i < asteroid->vertices_length / 3; i++) {
                vec3 origin, direction, v0, v1, v2;

                glm_vec3_copy(asteroid->vertices[i*3], v0);
                glm_vec3_copy(asteroid->vertices[i*3+1], v1);
                glm_vec3_copy(asteroid->vertices[i*3+2], v2);

                glm_vec3_rotate(v0, asteroid->angle, asteroid->axis);
                glm_vec3_rotate(v1, asteroid->angle, asteroid->axis);
                glm_vec3_rotate(v2, asteroid->angle, asteroid->axis);

                glm_vec3_add(v0, asteroid->location, v0);
                glm_vec3_add(v1, asteroid->location, v1);
                glm_vec3_add(v2, asteroid->location, v2);

                bullets_head = world->bullets;
                bullet_list_t **bullets_link = &(world->bullets);
                while(bullets_head->next != NULL) {
                    bullet_t *bullet = bullets_head->this;

                    glm_vec3_add(bullet->location, bullet->vertices[0], origin);
                    glm_vec3_copy(bullet->direction, direction);

                    float distance;
                    bool intersection;

                    if (glm_vec3_distance(asteroid->location, bullet->location) > (bullet->speed + asteroid->speed)*delta + MINIMUM_COLLISION_DISTANCE*asteroid->size)
                        intersection = 0;
                    else
                        intersection = glm_ray_triangle(origin, direction, v0, v1, v2, &distance);

                    if (intersection && distance <= glm_vec3_distance(bullet->vertices[0], bullet->vertices[1]) + bullet->speed*delta) {
                        // Warning: Memory leak
                        *asteroids_link = asteroids_head->next;
                        asteroid_destroyed = 1;
                        *bullets_link = bullets_head->next;
                        i = asteroid->vertices_length;

                        float size = asteroid->size;
                        // Create world->asteroids
                        if (size > 0.24f){
                            world->score++;
                            size /= 2.0f;
                            asteroid_t *asteroid1 = create_asteroid(asteroid->location, ASTEROID_SIZE*size, ASTEROID_VARIATION*size);
                            asteroid_t *asteroid2 = create_asteroid(asteroid->location, ASTEROID_SIZE*size, ASTEROID_VARIATION*size);
                            asteroid1->size = size;
                            asteroid2->size = size;
                            world->asteroids = asteroid_list_cons(asteroid1, world->asteroids);
                            world->asteroids = asteroid_list_cons(asteroid2, world->asteroids);
                            glm_vec3_ortho(bullet->direction, asteroid1->direction);
                            glm_vec3_copy(asteroid1->direction, asteroid2->direction);
                            glm_vec3_negate(asteroid2->direction);

                            float longitude = rand() / (float) RAND_MAX * 3.14159 * 2;
                            float colatitude = rand() / (float) RAND_MAX * 3.14159;
                            float distance = rand() / (float) RAND_MAX * (max_distance - 1000.0f) + 1000.0f;

                            world->asteroids = asteroid_list_cons(
                                create_asteroid(
                                                (vec3) {
                                                        distance * cos(longitude) * sin(colatitude),
                                                        distance * sin(longitude) * sin(colatitude),
                                                        distance * cos(colatitude)
                                                },
                                                ASTEROID_SIZE,
                                                ASTEROID_VARIATION),
                                world->asteroids);
                            world->asteroids->this->speed += (float) world->score*1000;
                        }

                        break;
                    } else
                        bullets_link = &(bullets_head->next);
                    bullets_head = bullets_head->next;
                }
            }
            if (!asteroid_destroyed)
                asteroids_link = &(asteroids_head->next);
            asteroids_head = asteroids_head->next;
        }

        asteroids_head = world->asteroids;
        while(asteroids_head->next != NULL) {
            asteroid_t *asteroid = asteroids_head->this;
            if(glm_vec3_norm(asteroid->location) < MINIMUM_COLLISION_DISTANCE*asteroid->size) {
                world->running = false;
                world->ship->speed = 0.0f;
            }
            asteroids_head = asteroids_head->next;
        }

        if (world->running) {
            // Handle keys
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                world->ship->speed += delta * 120.0f;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                world->ship->speed -= delta * 120.0f;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                glm_vec3_rotate(world->ship->direction, -delta, (vec3) {0.0f, 1.0f, 0.0f});
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                glm_vec3_rotate(world->ship->direction, delta, (vec3) {0.0f, 1.0f, 0.0f});
            }
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                vec3 axis;
                glm_vec3_cross((vec3) {0.0f, 1.0f, 0.0f}, world->ship->direction, axis);
                glm_vec3_rotate(world->ship->direction, -delta, axis);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                vec3 axis;
                glm_vec3_cross((vec3) {0.0f, 1.0f, 0.0f}, world->ship->direction, axis);
                glm_vec3_rotate(world->ship->direction, delta, axis);
            }
        }
        render(window, world);
        glfwPollEvents();
    }

    return 0;
}
