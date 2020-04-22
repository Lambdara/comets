#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_matrix;

out vec3 fragment_position;
out vec4 fragment_position_shadow;

void main()
{
    gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
    fragment_position = position;
    fragment_position_shadow = light_matrix * vec4(position, 1.0);
}
