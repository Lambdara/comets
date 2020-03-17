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

    asteroid->vertices_length = 12;
    asteroid->vertices = malloc(asteroid->vertices_length * sizeof(vector3f_t));
    asteroid->normals = malloc(asteroid->vertices_length * sizeof(vector3f_t));

    float longitudes[4];
    float colatitudes[4];

    vector3f_t vertices[4];

    for (int i = 0; i < 4; i++) {
        longitudes[i] = rand() / (float) RAND_MAX * 3.14159 * 2;
        colatitudes[i] = rand() / (float) RAND_MAX * 3.14159;
        vertices[i] = make_vector3f (cos(longitudes[i])*sin(colatitudes[i]),
                                               sin(longitudes[i])*sin(colatitudes[i]),
                                               cos(colatitudes[i]));
    }
 
    asteroid->vertices[0] = vertices[0];
    asteroid->vertices[1] = vertices[1];
    asteroid->vertices[2] = vertices[2];
    for (int i = 0; i < 3; i++) {
        float avg_x = (vertices[0].x + vertices[1].x + vertices[2].x)/3.0f;
        float avg_y = (vertices[0].y + vertices[1].y + vertices[2].y)/3.0f;
        float avg_z = (vertices[0].z + vertices[1].z + vertices[2].z)/3.0f;
        asteroid->normals[i] = make_vector3f(avg_x, avg_y, avg_z);
    }

    asteroid->vertices[3] = vertices[0];
    asteroid->vertices[4] = vertices[1];
    asteroid->vertices[5] = vertices[3];
    for (int i = 3; i < 6; i++) {
        float avg_x = (vertices[0].x + vertices[1].x + vertices[3].x)/3.0f;
        float avg_y = (vertices[0].y + vertices[1].y + vertices[3].y)/3.0f;
        float avg_z = (vertices[0].z + vertices[1].z + vertices[3].z)/3.0f;
        asteroid->normals[i] = make_vector3f(avg_x, avg_y, avg_z);
    }

    asteroid->vertices[6] = vertices[0];
    asteroid->vertices[7] = vertices[2];
    asteroid->vertices[8] = vertices[3];
        for (int i = 6; i < 9; i++) {
        float avg_x = (vertices[0].x + vertices[2].x + vertices[3].x)/3.0f;
        float avg_y = (vertices[0].y + vertices[2].y + vertices[3].y)/3.0f;
        float avg_z = (vertices[0].z + vertices[2].z + vertices[3].z)/3.0f;
        asteroid->normals[i] = make_vector3f(avg_x, avg_y, avg_z);
    }

    asteroid->vertices[9] = vertices[1];
    asteroid->vertices[10] = vertices[2];
    asteroid->vertices[11] = vertices[3];
    for (int i = 9; i < 12; i++) {
        float avg_x = (vertices[1].x + vertices[2].x + vertices[3].x)/3.0f;
        float avg_y = (vertices[1].y + vertices[2].y + vertices[3].y)/3.0f;
        float avg_z = (vertices[1].z + vertices[2].z + vertices[3].z)/3.0f;
        asteroid->normals[i] = make_vector3f(avg_x, avg_y, avg_z);
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
