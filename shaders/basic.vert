#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 aInstanceMatrix;
out vec3 Normal;
out vec3 FragPos;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isInstanced;
void main(){
    mat4 finalModelMatrix = isInstanced ? aInstanceMatrix : model;
    FragPos = vec3(finalModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(finalModelMatrix))) * aNormal;
    UV = aUV;
    gl_Position = projection * view * finalModelMatrix * vec4(aPos, 1.0);   
}
