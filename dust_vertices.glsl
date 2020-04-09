#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 fragment_position;

void main()
{
    gl_Position = projection_matrix * view_matrix * vec4(position, 0.1);
    fragment_position = position;
}
