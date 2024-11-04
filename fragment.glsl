#version 330 core

in vec3 frag_pos;
in vec3 normal;
out vec4 frag_color;

void main(){
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 light_source = vec3(1.0, 0.0, 0.0);
    float diffuse_strength = max(0.0, dot(light_source, normal));
    vec3 diffuse = light_color * diffuse_strength;
    
    vec3 ambient = vec3(0.5, 0.5, 0.5);
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    lighting = diffuse + ambient;
    vec3 model_color = vec3(0.7, 0.3, 0.4);
    vec3 color = model_color * lighting;
    frag_color = vec4(color, 1.0f);
}
