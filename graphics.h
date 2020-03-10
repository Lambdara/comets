#ifndef GRAPHICS_H
#define GRAPHICS_H

#define VERTEX_SHADER_PATH "vertexshader.vs"
#define FRAGMENT_SHADER_PATH "fragmentshader.fs"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include "world.h"

int intialize_window(GLFWwindow **);

void collect_vertices(asteroid_list_t*);

void render (GLFWwindow *);

void asteroid_translation_matrix(asteroid_t*, mat4);

#endif
