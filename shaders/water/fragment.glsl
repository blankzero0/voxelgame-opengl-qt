#version 450

in vec2 texture_coords;
in float brightness;

out vec4 color;

uniform sampler2DArray texture_sampler;

void main()
{
    color = vec4(vec3(brightness), 0.8) * texture(texture_sampler, vec3(texture_coords, 6));
}
