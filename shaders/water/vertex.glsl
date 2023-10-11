#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords_in;

uniform mat4 mvp;

out vec2 texture_coords;
out float brightness;

vec3 sun_dir = normalize(vec3(5, 7, 3));
vec3 normal = vec3(0, 1, 0);

void main()
{
    texture_coords = texture_coords_in;
    brightness = (dot(normal, sun_dir) + 1) / 2;

    gl_Position = mvp * vec4(position, 1);
}
