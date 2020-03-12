#version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;

out vec4 pixel_color;

void main()
{
    vec3 light_position = vec3(0.0, 0.0, -20.0);

    vec3 light_direction = normalize(light_position - fragment_position);

    float light = max(dot(light_direction, fragment_normal), dot(light_direction, -fragment_normal));

    pixel_color = vec4(1.0, 0.0, light, 1.0);
}
