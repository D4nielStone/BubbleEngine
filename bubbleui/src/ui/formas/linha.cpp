#include "linha.hpp"
#include "src/depuracao/debug.hpp"

using namespace BubbleUI::Formas;

Linha::Linha(Vector4f pos, Contexto* ctx) : posicoes(pos), contexto(ctx)
{
	definirBuffers();
}
void Linha::defPos(Vector4f pos)
{
    posicoes = pos;
}
// Deve definir cor base
void Linha::defCor(Color cor)
{
    cor_base = cor;
    contexto->shader.use();
    contexto->shader.setVec3("quadrado.cor", cor_base.r, cor_base.g, cor_base.b);
}
// deve definir buffers da linha
void Linha::definirBuffers()
{
    if (vertex.carregado)
    {
        Debug::emitir(Debug::Mensagem, "Vertex já carregado");
        return;
    }

    // Define os vértices para uma linha
    vertex.vertices = {
        // Posições
        0.f, 0.f,      
        1.f, 1.f
    };

    vertex.indices = {
        0, 1, 
    };

    glGenVertexArrays(1, &vertex.VAO);
    glGenBuffers(1, &vertex.VBO);
    glGenBuffers(1, &vertex.EBO);

    glBindVertexArray(vertex.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertex.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.vertices.size() * sizeof(float), vertex.vertices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex.indices.size() * sizeof(unsigned int), vertex.indices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    // Atributos de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertex.carregado = true;
}
// Deve transformar coordenadas pixel para NDC
Vector4f Linha::paraNDC()
{
    Vector4f coord_ndc;

    coord_ndc.z = (posicoes.z * 2.f) / contexto->tamanho.width;
    coord_ndc.w = (2.0f * -posicoes.w) / contexto->tamanho.height;
    coord_ndc.x = (posicoes.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * posicoes.y) / contexto->tamanho.height;

    return coord_ndc;
}
void Linha::atualizar(float deltaTime)
{
    coord_ndc = paraNDC();
}
// Deve renderizar
void Linha::renderizar()
{
    contexto->shader.use();
    contexto->shader.setVec2("quadrado.tamanho", coord_ndc.z, coord_ndc.w);
    contexto->shader.setVec2("quadrado.posicao", coord_ndc.x, coord_ndc.y);
    contexto->shader.setVec3("quadrado.cor", cor_base.r, cor_base.g, cor_base.b);
    glBindVertexArray(vertex.VAO);
    glDrawElements(GL_LINES, static_cast<GLsizei>(vertex.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}