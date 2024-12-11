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

uniform sampler2D texture_diffuse1;

void main()
{              
    // Verifica se a textura existe ou se deve usar a cor base
    vec4 texColor = texture(texture_diffuse1, Uv);  // Obt�m a cor da textura
    if (texColor.a < 0.1) {  // Verifica se a textura � praticamente transparente
        FragColor = material.cor_difusa;  // Se a textura n�o estiver presente, usa a cor base
    } else {
        FragColor = texColor;  // Caso contr�rio, usa a cor da textura
    }
}
)";