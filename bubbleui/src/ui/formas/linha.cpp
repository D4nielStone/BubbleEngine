// Copyright (c) 2024 Daniel Oliveira
// Licenciado sob a licença MIT. Consulte o arquivo LICENSE para mais informaçoes.
#include "linha.hpp"
#include "src/depuracao/debug.hpp"

using namespace BubbleUI::Formas;

Linha::Linha(const Vector4 &pos, std::shared_ptr<Contexto> ctx) : posicoes(pos), contexto(ctx)
{
    cor_base = { 0.4f, 0.f, 0.4f, 0.9f };
    if (!linha_vertex.carregado)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Debug::emitir(Debug::Erro, "Failed to initialize GLAD");
            return;
        }
        glGenBuffers(1, &linha_vertex.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, linha_vertex.VBO);
        glGenVertexArrays(1, &linha_vertex.VAO);
        glBindVertexArray(linha_vertex.VAO);
        linha_vertex.carregado = true;
    }
}
void Linha::definirPosicao(const Vector4 &pos)
{
    posicoes = pos;
}
// Deve definir cor base
void Linha::defCor(const Color &cor)
{
    cor_base = cor;
}
void BubbleUI::Formas::Linha::definirCorRef(Color* cor)
{
    cor_referencial = cor;
}
// Deve transformar coordenadas pixel para NDC
Vector4f Linha::paraNDC()
{
    // Converte as coordenadas de pixels para NDC com o sistema top-left
    coord_ndc.x = (posicoes.x * 2.f) / contexto->tamanho.width - 1.f;

    // Inverter a coordenada y para top-down
    coord_ndc.y = 1.0f - (2.0f * posicoes.y / contexto->tamanho.height);

    // Calcular para as coordenadas da segunda posição (z, w)
    coord_ndc.z = (posicoes.w * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.w = 1.0f - (2.0f * posicoes.h / contexto->tamanho.height);
    return coord_ndc;
}

void Linha::atualizar()
{
    if (tamanhoDoContextoAnterior != contexto->tamanho || tamanhoAnterior != posicoes)
    {
        paraNDC();
        tamanhoAnterior = posicoes;
        tamanhoDoContextoAnterior = contexto->tamanho;
    }
    if(cor_referencial)cor_base = (*cor_referencial) + 0.1f;
}
// Deve renderizar
void Linha::renderizar() const
{
    shader.use();
    shader.setVec2("linha.pos1", coord_ndc.x, coord_ndc.y);
    shader.setVec2("linha.pos2", coord_ndc.z, coord_ndc.w);
    shader.setCor("cor", cor_base);

    glBindVertexArray(linha_vertex.VAO);
    glDrawArrays(GL_LINES, 0, 2);
}
