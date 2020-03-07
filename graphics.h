#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "world.h"

int intialize_window(GLFWwindow **);

void collect_vertices(asteroid_list_t*);

void render (GLFWwindow *);

#endif
