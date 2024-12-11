inline const char* phong_frag = R"(
#version 330 core
out vec4 FragColor;

in vec2 Uv; // Coordenadas da textura
in vec3 Normal;    // Normal da superf�cie
in vec3 Position;   // Posi��o do fragmento

struct Material
{
    vec4 cor_difusa;
    vec4 cor_especular;
};

uniform Material material;

void main()
{              
    FragColor = material.cor_difusa;
}
)";