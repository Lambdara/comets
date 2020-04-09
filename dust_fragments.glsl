#version 330 core

in vec3 fragment_position;

out vec4 pixel_color;

void main()
{
    float diffuse_light = 1.0;
    float ambient_light = 0.025;
    float light = diffuse_light + ambient_light;
    float dist = (1000-length(fragment_position))/1000;

    light = light * dist * dist;

    if (length(fragment_position) > 800.0) {
        light = light * (1000 - length(fragment_position)) / 200.0;
    }

    pixel_color = light*vec4(1.0, 1.0, 1.0, 1.0);
}
