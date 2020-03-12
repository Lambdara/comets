#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragment_position;
out vec3 fragment_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
    fragment_position = vec3(gl_Position);
    fragment_normal = normalize(normal);
}
