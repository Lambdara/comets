#ifndef GRAPHICS_H
#define GRAPHICS_H

#define ASTEROID_VERTEX_SHADER_PATH "asteroid_vertices.glsl"
#define ASTEROID_FRAGMENT_SHADER_PATH "asteroid_fragments.glsl"
#define BULLET_VERTEX_SHADER_PATH "bullet_vertices.glsl"
#define BULLET_FRAGMENT_SHADER_PATH "bullet_fragments.glsl"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <math.h>
#include "world.h"

int intialize_window(GLFWwindow **);

void collect_vertices(asteroid_list_t *, bullet_list_t *, ship_t *);

void render (GLFWwindow *);

void asteroid_translation_matrix(asteroid_t*, mat4);

#endif
