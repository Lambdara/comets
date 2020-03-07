#include "world.h"

vector3f_t make_vector3f(float x, float y, float z) {
    vector3f_t vector = {x, y, z};
    return vector;
}

asteroid_t *create_asteroid(float x, float y, float z) {
    asteroid_t *asteroid = malloc(sizeof(asteroid_t));

    // Temporary simple generation
    asteroid->vertices_length = 4;
    asteroid->vertices = malloc(4 * sizeof(vector3f_t));
    asteroid->vertices[0] = make_vector3f (x - 0.01, y - 0.01, z - 0.01);
    asteroid->vertices[1] = make_vector3f (x + 0.01, y - 0.01, z - 0.01);
    asteroid->vertices[2] = make_vector3f (x, y + 0.01, z - 0.01);
    asteroid->vertices[3] = make_vector3f (x, y, z + 0.01);

    return asteroid;
}

asteroid_list_t *create_asteroid_list() {
    asteroid_list_t *asteroids = malloc(sizeof(asteroid_list_t));

    asteroids->this = NULL;
    asteroids->next = NULL;

    return asteroids;
}

asteroid_list_t* asteroid_list_cons(asteroid_t* asteroid, asteroid_list_t* asteroids) {
    asteroid_list_t *node = malloc(sizeof(asteroid_list_t));

    node->this = asteroid;
    node->next = asteroids;

    return node;
}
