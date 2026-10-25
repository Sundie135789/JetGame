#version 330 core
out vec4 FragColor;
in vec2 UV;
uniform sampler2D texture_albedo;
void main(){
    vec4 tex = texture(texture_albedo, UV);
    FragColor = tex;
}