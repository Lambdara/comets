#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

vector3f_t make_vector3f(float x, float y, float z) {
    vector3f_t vector = {x, y, z};
    return vector;
}

asteroid_t *create_asteroid(float x, float y, float z) {
    asteroid_t *asteroid = malloc(sizeof(asteroid_t));

    asteroid->vertices_length = 4;
    asteroid->vertices = malloc(4 * sizeof(vector3f_t));
    asteroid->normals = malloc(4 * sizeof(vector3f_t));

    float longitudes[4];
    float colatitudes[4];

    for (int i = 0; i < 4; i++) {
        longitudes[i] = rand() / (float) RAND_MAX * 3.14159 * 2;
        colatitudes[i] = rand() / (float) RAND_MAX * 3.14159;
        asteroid->vertices[i] = make_vector3f (cos(longitudes[i])*sin(colatitudes[i]),
                                               sin(longitudes[i])*sin(colatitudes[i]),
                                               cos(colatitudes[i]));
        asteroid->normals[i] = make_vector3f (cos(longitudes[i])*sin(colatitudes[i]),
                                               sin(longitudes[i])*sin(colatitudes[i]),
                                               cos(colatitudes[i]));
    }

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
