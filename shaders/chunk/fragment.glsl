#version 450

in vec2 texture_coords;
in flat int texture_index;
in float brightness;

out vec4 color;

uniform sampler2DArray texture_sampler;

void main()
{
    color = brightness * texture(texture_sampler, vec3(texture_coords, texture_index));
}
