#include "linha.hpp"
#include "src/depuracao/debug.hpp"

using namespace BubbleUI::Formas;

Linha::Linha(Vector4f pos, Contexto* ctx) : posicoes(pos), contexto(ctx)
{
    defCor({ 0.4f, 0.0f, 0.4f });
    if (!linha_vertex.carregado)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            std::cout << "Failed to initialize GLAD" << std::endl;
        glGenBuffers(1, &linha_vertex.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, linha_vertex.VBO);
        glGenVertexArrays(1, &linha_vertex.VAO);
        glBindVertexArray(linha_vertex.VAO);
        linha_vertex.carregado = true;
    }
}
void Linha::defPos(Vector4f pos)
{
    posicoes = pos;
}
// Deve definir cor base
void Linha::defCor(Color cor)
{
    cor_base = cor;
}
// Deve transformar coordenadas pixel para NDC
Vector4f Linha::paraNDC()
{
    // Converte as coordenadas de pixels para NDC com o sistema top-left
    coord_ndc.x = (posicoes.x * 2.f) / contexto->tamanho.width - 1.f;

    // Inverter a coordenada y para top-down
    coord_ndc.y = 1.0f - (2.0f * posicoes.y / contexto->tamanho.height);

    // Calcular para as coordenadas da segunda posição (z, w)
    coord_ndc.z = (posicoes.z * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.w = 1.0f - (2.0f * posicoes.w / contexto->tamanho.height);
    return coord_ndc;
}

void Linha::atualizar()
{
    paraNDC();
}
// Deve renderizar
void Linha::renderizar()
{
    shader.use();
    shader.setVec2("linha.pos1", coord_ndc.x, coord_ndc.y);
    shader.setVec2("linha.pos2", coord_ndc.z, coord_ndc.w);
    shader.setVec3("cor", cor_base.r, cor_base.g, cor_base.b);

    glBindVertexArray(linha_vertex.VAO);
    glDrawArrays(GL_LINES, 0, 2);

}