#version 330 core

in vec3 Normal;
in vec2 UV;
uniform sampler2D texture_albedo;
//uniform sampler2D tex;
out vec4 FragColor;
void main(){
    //vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    //float diff = max(dot(norm, lightDir), 0.3);
    vec4 texColor = texture(texture_albedo, UV);
    FragColor = vec4(texColor);
}