#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct {
    int vertices_length;
    vec3 *vertices;
    vec3 *normals;
    GLuint vbo;
    GLuint nbo;
    vec3 location;
    float rotation_speed;
    vec3 direction;
    float speed;
    float angle;
    vec3 axis;
    float size;
} asteroid_t;

typedef struct {
    vec3 *vertices;
    vec3 direction;
    vec3 location;
    float speed;
} bullet_t;

typedef struct {
    vec3* vertices;
    vec3* normals;
    vec3 direction;
    float speed;
} ship_t;

typedef struct asteroid_list_t {
    asteroid_t *this;
    struct asteroid_list_t *next;
} asteroid_list_t;

typedef struct bullet_list_t {
    bullet_t *this;
    struct bullet_list_t *next;
} bullet_list_t;

vec3 camera_location;
float camera_angle;

float line_triangle_intersect(vec3, vec3, vec3, vec3, vec3);

asteroid_t *create_asteroid(vec3, float, float);

asteroid_list_t *create_asteroid_list();

asteroid_list_t *asteroid_list_cons(asteroid_t*, asteroid_list_t*);

bullet_t *create_bullet(vec3, vec3, float);
bullet_list_t *create_bullet_list();
bullet_list_t *bullet_list_cons(bullet_t *, bullet_list_t *);

ship_t *create_ship(vec3);

#endif
