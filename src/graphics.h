#ifndef GRAPHICS_H
#define GRAPHICS_H
#define GLT_IMPLEMENTATION
#define ASTEROID_VERTEX_SHADER_PATH "src/shaders/asteroid_vertices.glsl"
#define ASTEROID_FRAGMENT_SHADER_PATH "src/shaders/asteroid_fragments.glsl"
#define BULLET_VERTEX_SHADER_PATH "src/shaders/bullet_vertices.glsl"
#define BULLET_FRAGMENT_SHADER_PATH "src/shaders/bullet_fragments.glsl"
#define DUST_VERTEX_SHADER_PATH "src/shaders/dust_vertices.glsl"
#define DUST_FRAGMENT_SHADER_PATH "src/shaders/dust_fragments.glsl"
#define CROSSHAIR_VERTEX_SHADER_PATH "src/shaders/crosshair_vertices.glsl"
#define CROSSHAIR_FRAGMENT_SHADER_PATH "src/shaders/crosshair_fragments.glsl"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <math.h>
#include "world.h"

#include "gltext/gltext.h"

int intialize_window(GLFWwindow **);

void render (GLFWwindow *, world_t *);

void asteroid_translation_matrix(asteroid_t*, mat4);

#endif
