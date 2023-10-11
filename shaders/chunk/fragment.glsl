#version 450

in vec2 texture_coords;
in flat int texture_index;
in float diffuse;
in vec3 frag_position;
in vec3 frag_normal;

out vec4 color;

uniform sampler2DArray texture_sampler;
uniform vec3 eye_pos;

vec3 sun_dir = normalize(vec3(5, 4, 3));

void main()
{
    vec3 look_dir = normalize(eye_pos - frag_position);
    float specular = pow(max(dot(reflect(-sun_dir, frag_normal), look_dir), 0), 16);
    float brightness = diffuse + 0.5 * specular;
    color = vec4(vec3(brightness), 1) * texture(texture_sampler, vec3(texture_coords, texture_index));}
