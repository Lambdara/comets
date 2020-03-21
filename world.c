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

asteroid_t *create_asteroid(vec3 location) {
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

    // Have center of mass in the center
    vec3 com = {0.0f, 0.0f, 0.0f};
    for(int i = 0; i < 4; i++) {
        glm_vec3_add(vertices[i], com, com);
    }
    glm_vec3_scale(com, 1.0f/4.0f, com);
    for(int i = 0; i < 4; i++) {
        glm_vec3_sub(vertices[i], com, vertices[i]);
    }

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

    glm_vec3_copy(location, asteroid->location);

    asteroid->rotation_speed = rand() / (float) RAND_MAX * 3.14159;
    for (int i = 0; i < 3; i++)
        asteroid->axis[i] = rand() / (float) RAND_MAX;
    glm_vec3_normalize(asteroid->axis);
    asteroid->angle = rand() / (float) RAND_MAX * 3.14159 * 2;

    glm_vec3_copy((vec3) {rand() / (float) RAND_MAX,
                              rand() / (float) RAND_MAX,
                              rand() / (float) RAND_MAX},
        asteroid->direction);
    glm_vec3_normalize(asteroid->direction);
    asteroid->speed = rand() / (float) RAND_MAX;


    return asteroid;
}

asteroid_list_t *create_asteroid_list() {
    asteroid_list_t *asteroids = malloc(sizeof(asteroid_list_t));

    asteroids->this = NULL;
    asteroids->next = NULL;

    return asteroids;
}

asteroid_list_t *asteroid_list_cons(asteroid_t* asteroid, asteroid_list_t* asteroids) {
    asteroid_list_t *node = malloc(sizeof(asteroid_list_t));

    node->this = asteroid;
    node->next = asteroids;

    return node;
}

bullet_t *create_bullet(vec3 location, vec3 direction) {
    bullet_t *bullet = malloc(sizeof(bullet_t));

    glm_vec3_copy(location, bullet->location);
    glm_vec3_copy(direction, bullet->direction);
    glm_vec3_divs(direction, 10.0f, bullet->to);
    glm_vec3_divs(direction, -10.0f, bullet->from);
    bullet->speed = 50.0f;

    return bullet;
}

bullet_list_t *create_bullet_list() {
    bullet_list_t *bullets = malloc(sizeof(bullet_list_t));

    bullets->this = NULL;
    bullets->next = NULL;

    return bullets;
}

bullet_list_t *bullet_list_cons(bullet_t* bullet, bullet_list_t* bullets) {
    bullet_list_t *node = malloc(sizeof(bullet_list_t));

    node->this = bullet;
    node->next = bullets;

    return node;
}

ship_t *create_ship(vec3 direction) {
    ship_t *ship = malloc(sizeof(ship_t));

    glm_vec3_copy(direction, ship->direction);
    ship->speed = 0.0f;

    vec3 vertices[5] = {{0.0f, 0.0f, -2.0f}, // front       0
                        {-1.0f, 0.0f, 1.0f}, // back, left  1
                        {1.0f, 0.0f, 1.0f}, // back, right  2
                        {0.0f, 0.2f, 1.0f}, // back, up     3
                        {0.0f, -0.2f, 1.0f}}; // back, down 4

    ship->vertices = malloc(6*3*sizeof(vec3)); // 6 surfaces of 3 vertices
    ship->normals = malloc(6*3*sizeof(vec3)); // idem

    glm_vec3_copy(vertices[0], ship->vertices[0]);
    glm_vec3_copy(vertices[1], ship->vertices[1]);
    glm_vec3_copy(vertices[3], ship->vertices[2]);

    glm_vec3_copy(vertices[0], ship->vertices[3]);
    glm_vec3_copy(vertices[2], ship->vertices[4]);
    glm_vec3_copy(vertices[3], ship->vertices[5]);

    glm_vec3_copy(vertices[0], ship->vertices[6]);
    glm_vec3_copy(vertices[1], ship->vertices[7]);
    glm_vec3_copy(vertices[4], ship->vertices[8]);

    glm_vec3_copy(vertices[0], ship->vertices[9]);
    glm_vec3_copy(vertices[2], ship->vertices[10]);
    glm_vec3_copy(vertices[4], ship->vertices[11]);

    glm_vec3_copy(vertices[3], ship->vertices[12]);
    glm_vec3_copy(vertices[1], ship->vertices[13]);
    glm_vec3_copy(vertices[4], ship->vertices[14]);

    glm_vec3_copy(vertices[3], ship->vertices[15]);
    glm_vec3_copy(vertices[2], ship->vertices[16]);
    glm_vec3_copy(vertices[4], ship->vertices[17]);

    for (int i = 0; i < 6; i++) {
        for (int j = i * 3; j < i * 3 + 3; j++) {
            make_normal(ship->vertices[i*3],
                        ship->vertices[i*3+1],
                        ship->vertices[i*3+2],
                        ship->normals[j]);
        }
    }
    
    return ship;
}
