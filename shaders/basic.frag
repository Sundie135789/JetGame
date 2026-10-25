#version 330 core

in vec3 Normal;
in vec2 UV;
in vec3 FragPos;
uniform sampler2D texture_albedo;
out vec4 FragColor;
uniform  vec3 lightPos;
uniform vec3 viewPos;
void main(){
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    vec3 viewDir = normalize(viewPos -FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = spec * vec3(1.0);
    vec4 texColor = texture(texture_albedo, UV);
    vec3 result = (ambient + diffuse + specular) * texColor.xyz;
    FragColor = vec4(result, texColor.a);
}