#include "world.h"

asteroid_t *create_asteroid(float x, float y, float z) {
    asteroid_t *asteroid = malloc(sizeof(asteroid_t));

    asteroid->x = x;
    asteroid->y = y;
    asteroid->z = z;

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
