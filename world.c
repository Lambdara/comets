#include "world.h"
#include <stdio.h>

vector3f_t make_vector3f(float x, float y, float z) {
    vector3f_t vector = {x, y, z};
    return vector;
}

vector3f_t vector3f_cross(vector3f_t a, vector3f_t b) {
    vector3f_t vector = {a.y * b.z - a.z * b.y,
                         a.z * b.x - a.x * b.z,
                         a.x * b.y - a.y * b.x
    };
    return vector;
}

vector3f_t vector3f_minus(vector3f_t a, vector3f_t b) {
    vector3f_t vector = {a.x - b.x,
                         a.y - b.y,
                         a.z - b.z
    };
    return vector;
}

asteroid_t *create_asteroid(float x, float y, float z) {
    asteroid_t *asteroid = malloc(sizeof(asteroid_t));

    // Temporary simple generation
    asteroid->vertices_length = 4;
    asteroid->vertices = malloc(4 * sizeof(vector3f_t));
    asteroid->vertices[0] = make_vector3f (-0.5, -0.5, -0.5);
    asteroid->vertices[1] = make_vector3f (0.5, -0.5, -0.5);
    asteroid->vertices[2] = make_vector3f (0, 0.5, -0.5);
    asteroid->vertices[3] = make_vector3f (0, 0, 0.5);
    asteroid->normals = malloc(4 * sizeof(vector3f_t));
    asteroid->normals[0] = vector3f_cross(vector3f_minus(asteroid->vertices[2], asteroid->vertices[1]),
                                          vector3f_minus(asteroid->vertices[3], asteroid->vertices[1]));
    asteroid->normals[1] = vector3f_cross(vector3f_minus(asteroid->vertices[3], asteroid->vertices[2]),
                                          vector3f_minus(asteroid->vertices[0], asteroid->vertices[2]));
    asteroid->normals[2] = vector3f_cross(vector3f_minus(asteroid->vertices[0], asteroid->vertices[3]),
                                          vector3f_minus(asteroid->vertices[1], asteroid->vertices[3]));
    asteroid->normals[3] = vector3f_cross(vector3f_minus(asteroid->vertices[1], asteroid->vertices[0]),
                                          vector3f_minus(asteroid->vertices[2], asteroid->vertices[0]));
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
