#version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;

out vec4 pixel_color;

void main()
{
    vec3 light_position = vec3(100000.0, 25000.0, 0.0);

    vec3 light_direction = normalize(light_position - fragment_position);

    float diffuse_light = max(dot(light_direction, fragment_normal), 0);
    float ambient_light = 0.02500000000000000001;
    float light = diffuse_light + ambient_light;

    float dist = (1000-length(fragment_position))/1000;
    light = light * dist * dist;

    if (length (fragment_position) > 800.0) {
        light = light * (1000 - length(fragment_position))/200.0;
    }

    pixel_color = light*vec4(0.8, 0.8, 0.8, 1.0);
}
