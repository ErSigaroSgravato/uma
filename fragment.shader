#version 330 core

in vec3 frag_pos;
in vec3 normal;
out vec4 frag_color;

uniform vec3 light_dir = normalize(vec3(-0.2, -1.0, -0.3f));
uniform vec3 light_color = vec3(1.0, 1.0, 1.0);
uniform vec3 object_color = vec3(1.0, 0.5, 0.2);

void main(){
    float diff = max(dot(normalize(normal), light_dir), 0.0);
    vec3 diffuse = diff * light_color * object_color;
    frag_color = vec4(diffuse, 1.0f);
}