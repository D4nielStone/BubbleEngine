#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 Uv;
out vec3 cor_texto;

struct Quadrado
{
    vec2 tamanho;
    vec2 posicao;
    vec3 cor;
};
uniform Quadrado quadrado;

void main()
{
    Uv = aUv;
    cor_texto = quadrado.cor;

    // Ajuste a posição e o tamanho do quadrado
    vec2 scaledPos = aPos.xy * quadrado.tamanho;
    vec2 finalPos = scaledPos + quadrado.posicao;
    
    gl_Position = vec4(finalPos, 0.0, 1.0);
}