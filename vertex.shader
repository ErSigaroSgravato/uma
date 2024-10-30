#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;    
uniform mat4 model;
uniform mat4 view;

out vec3 frag_pos;
out vec3 normal;
    
void main(){
    frag_pos = vec3(model * vec4(aPos, 1.0f));
    normal = vec3(0.0, 0.0, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

}