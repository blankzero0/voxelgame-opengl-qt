#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords_in;
layout (location = 2) in int texture_index_in;
layout (location = 3) in vec3 normal;

uniform mat4 mvp;

out vec2 texture_coords;
out flat int texture_index;
out float diffuse;
out vec3 frag_position;
out vec3 frag_normal;

vec3 sun_dir = normalize(vec3(5, 4, 3));

void main()
{
    texture_coords = texture_coords_in;
    texture_index = texture_index_in;
    diffuse = (dot(normal, sun_dir) + 1) / 2;
    frag_position = position;
    frag_normal = normal;

    gl_Position = mvp * vec4(position, 1);
}
