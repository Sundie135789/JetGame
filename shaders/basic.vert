#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
out vec3 Normal;
out vec2 UV;
uniform mat4 model, view, projection;
void main(){
    Normal = mat3(transpose(inverse(model))) * aNormal;
    UV = aUV;
    gl_Position = projection * view * model * vec4(aPos, 1.0);   
}