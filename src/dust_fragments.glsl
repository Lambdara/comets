#version 330 core

in vec3 fragment_position;
in vec4 fragment_position_shadow;

out vec4 pixel_color;

uniform sampler2D depth_map;

void main()
{
    float diffuse_light = 1.0;
    float ambient_light = 0.025;

    // Shadow
    vec3 shadow_coords = fragment_position_shadow.xyz / fragment_position_shadow.w;
    shadow_coords = shadow_coords * 0.5 + 0.5;
    float closest_depth = texture(depth_map, shadow_coords.xy).r;
    float current_depth = shadow_coords.z;
    float shadow = current_depth > closest_depth ? 1.0 : 0.0;


    float light = (1-shadow)*diffuse_light + ambient_light;
    float dist = (1000-length(fragment_position))/1000;

    light = light * dist * dist;

    if (length(fragment_position) > 800.0) {
        light = light * (1000 - length(fragment_position)) / 200.0;
    }

    pixel_color = light*vec4(1.0, 1.0, 1.0, 1.0);
}
