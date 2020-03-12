#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} vector3f_t;

typedef struct {
    int vertices_length;
    vector3f_t *vertices;
    float x, y, z;
} asteroid_t;

typedef struct asteroid_list_t {
    asteroid_t *this;
    struct asteroid_list_t *next;
} asteroid_list_t;

vector3f_t camera_location;
float camera_angle;

asteroid_t *create_asteroid(float, float, float);

asteroid_list_t *create_asteroid_list();

asteroid_list_t *asteroid_list_cons(asteroid_t*, asteroid_list_t*);

#endif
