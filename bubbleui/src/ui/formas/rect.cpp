#include "rect.hpp"
#include "src/depuracao/debug.h"
#include "src/depuracao/assert.hpp"
#include "glad/glad.h"

using namespace BubbleUI::Formas;
Rect::Rect(Vector4 rect, Contexto* ctx) : retangulo(rect), contexto(ctx)
{
    definirBuffers(vertex);
}

BubbleUI::Formas::Rect::Rect(Contexto* ctx) : contexto(ctx)
{
}

Vector4 BubbleUI::Formas::Rect::obtRect() const
{
    return retangulo;
}

void Rect::atualizar(float deltaTime)
{
    contexto->shader.use();
    contexto->shader.setVec2("quadrado.tamanho", retangulo.w, retangulo.h);
    contexto->shader.setVec2("quadrado.posicao", retangulo.x, retangulo.y);
}
void BubbleUI::Formas::Rect::renderizar()
{
    contexto->shader.use();
    glBindVertexArray(vertex.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Rect::definirBuffers()
{
    if (vertex.carregado)
        return;
    // Define os vértices para um quadrado
    vertex.vertices = {
        // Posições     // Coordenadas de textura
        0.0f, 0.0f,     0.0f, 0.0f,
        1, 0.0f,        1.0f, 0.0f,
        1, 1,           1.0f, 1.0f,
        0.0f, 1,        0.0f, 1.0f
    };

    vertex.indices = {
        0, 1, 2, // Primeiro triângulo
        2, 3, 0  // Segundo triângulo
    };
    if (!gladLoadGL()) 
    {
        Debug::emitir(Debug::Erro, "Glad");
        return;
    }
    glGenVertexArrays(1, &vertex.VAO);
    glGenBuffers(1, &vertex.VBO);
    glGenBuffers(1, &vertex.EBO);

    glBindVertexArray(vertex.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertex.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex.vertices), vertex.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex.indices), vertex.indices.data(), GL_STATIC_DRAW);

    // Atributos de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertex.carregado = true;
}