#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords_in;

uniform mat4 mvp;

out vec2 texture_coords;
out float diffuse;
out vec3 frag_position;

vec3 sun_dir = normalize(vec3(5, 4, 3));
vec3 normal = vec3(0, 1, 0);

void main()
{
    texture_coords = texture_coords_in;
    diffuse = (dot(normal, sun_dir) + 1) / 2;
    frag_position = position;

    gl_Position = mvp * vec4(position, 1);
}
