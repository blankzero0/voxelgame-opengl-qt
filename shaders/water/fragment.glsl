#version 450

in vec2 texture_coords;
in float diffuse;
in vec3 frag_position;

out vec4 color;

uniform sampler2DArray texture_sampler;
uniform vec3 eye_pos;

vec3 sun_dir = normalize(vec3(5, 4, 3));
vec3 normal = vec3(0, 1, 0);

void main()
{
    vec3 look_dir = normalize(eye_pos - frag_position);
    float specular = pow(max(dot(reflect(-sun_dir, normal), look_dir), 0), 16);
    float brightness = diffuse + 4 * specular;
    color = vec4(vec3(brightness), 0.95) * texture(texture_sampler, vec3(texture_coords, 6));
}
