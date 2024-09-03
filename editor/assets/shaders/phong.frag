#version 330 core
out vec4 FragColor;

struct Material
{
    vec3 cor_difusa;
};
uniform Material material;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, -R).rgb* material.cor_difusa, 1.0);
}