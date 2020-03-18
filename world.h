#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <cglm/cglm.h>

typedef struct {
    int vertices_length;
    vec3 *vertices;
    vec3 *normals;
    float x, y, z;
    float rotation_speed;
    float angle;
    vec3 axis;
} asteroid_t;

typedef struct asteroid_list_t {
    asteroid_t *this;
    struct asteroid_list_t *next;
} asteroid_list_t;

vec3 camera_location;
float camera_angle;

asteroid_t *create_asteroid(float, float, float);

asteroid_list_t *create_asteroid_list();

asteroid_list_t *asteroid_list_cons(asteroid_t*, asteroid_list_t*);

#endif
