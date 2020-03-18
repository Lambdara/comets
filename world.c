#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void make_normal(vec3 a, vec3 b, vec3 c, vec3 n) {
    vec3 v, w;
    glm_vec3_sub(c, a, v);
    glm_vec3_sub(c, b, w);
    glm_vec3_crossn(v, w, n);
    if (glm_vec3_angle(a,n) > 3.1415926535f/2.0f) {
        glm_vec3_negate(n);
    }
}

asteroid_t *create_asteroid(float x, float y, float z) {
    asteroid_t *asteroid = malloc(sizeof(asteroid_t));

    asteroid->vertices_length = 12;
    asteroid->vertices = malloc(asteroid->vertices_length * sizeof(vec3));
    asteroid->normals = malloc(asteroid->vertices_length * sizeof(vec3));

    float longitudes[4];
    float colatitudes[4];

    vec3 vertices[4];

    for (int i = 0; i < 3; i++) {
        longitudes[i] = rand() / (float) RAND_MAX * 3.14159 * 2;
        colatitudes[i] = rand() / (float) RAND_MAX * 3.14159;
        vertices[i][0] = cos(longitudes[i])*sin(colatitudes[i]);
        vertices[i][1] = sin(longitudes[i])*sin(colatitudes[i]);
        vertices[i][2] = cos(colatitudes[i]);
    }

    vertices[3][0] = -(vertices[0][0] + vertices[1][0] + vertices[2][0])/3.0f;
    vertices[3][1] = -(vertices[0][1] + vertices[1][1] + vertices[2][1])/3.0f;
    vertices[3][2] = -(vertices[0][2] + vertices[1][2] + vertices[2][2])/3.0f;

    int v = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (i != j)
                glm_vec3_copy(vertices[j], asteroid->vertices[v++]);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            make_normal(asteroid->vertices[i*3],
                        asteroid->vertices[i*3+1],
                        asteroid->vertices[i*3+2],
                        asteroid->normals[i*3+j]);

    asteroid->x = x;
    asteroid->y = y;
    asteroid->z = z;

    asteroid->rotation_speed = rand() / (float) RAND_MAX * 3.14159;
    for (int i = 0; i < 3; i++)
        asteroid->axis[i] = rand() / (float) RAND_MAX;
    glm_vec3_normalize(asteroid->axis);
    asteroid->angle = rand() / (float) RAND_MAX * 3.14159 * 2;

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
