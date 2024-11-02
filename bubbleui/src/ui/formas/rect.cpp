#include "rect.hpp"
#include "src/depuracao/debug.hpp"
#include "src/depuracao/assert.hpp"

using namespace BubbleUI::Formas;

Rect::Rect(std::shared_ptr<Contexto> ctx, const Vector4 &rect) : retangulo(rect), contexto(ctx)
{
    definirBuffers();
}

// Deve retornar o tamanho/posicao
Vector4 Rect::obtRect() const
{
    return retangulo;
}

Vector4f BubbleUI::Formas::Rect::obtRectNDC() const
{
    return coord_ndc;
}

void Rect::defTam(const Vector2 &tam)
{
    retangulo.w = tam.x;
    retangulo.h = tam.y;
}

void Rect::defPos(const Vector2 &pos)
{
    retangulo.x = pos.x;
    retangulo.y = pos.y;
}

void Rect::adiTam(const Vector2 &tam)
{
    retangulo.w += tam.x;
    retangulo.h += tam.y;
}

void Rect::adiPos(const Vector2 &pos)
{
    retangulo.x += pos.x;
    retangulo.y += pos.y;
}
void BubbleUI::Formas::Rect::definirRetangulo(const Vector4& rect)
{
    retangulo = rect;
}
// Deve definir cor base
void Rect::defCor(const Color &cor)
{
    *cor_base = cor;
}
void BubbleUI::Formas::Rect::defCor(Color* cor)
{
    cor_base = cor;
}
// Deve atualizar
void Rect::atualizar()
{
    if (tamanhoDoContextoAnterior != contexto->tamanho || tamanhoAnterior != retangulo)
    {
        paraNDC();
        tamanhoAnterior = retangulo;
        tamanhoDoContextoAnterior = contexto->tamanho;
    }
}

// Deve renderizar
void Rect::renderizar() const
{
    shader.use();
    shader.setVec2("quadrado.tamanho", coord_ndc.z, coord_ndc.w);
    shader.setVec2("quadrado.posicao", coord_ndc.x, coord_ndc.y);
    shader.setCor("quadrado.cor", *cor_base);
    glBindVertexArray(rect_vertex.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
// Deve transformar coordenadas pixel para NDC
Vector4f Rect::paraNDC()
{
    coord_ndc.z = (retangulo.w * 2.f) / contexto->tamanho.width;
    coord_ndc.w = -(2.0f * retangulo.h) / contexto->tamanho.height;
    coord_ndc.x = (retangulo.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * retangulo.y) / contexto->tamanho.height;

    return coord_ndc;
}
// deve definir buffers do quadrado
void Rect::definirBuffers()
{
    if (rect_vertex.carregado)    return;
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Debug::emitir(Debug::Erro, "Glad");
        return;
    }

    // Define os vértices para um quadrado
    rect_vertex.vertices = {
        // Posições     // Coordenadas de textura
        0.f, 0.f,     0.0f, 0.0f,
        1.f, 0.f,     1.0f, 0.0f,
        1.f, 1.f,     1.0f, 1.0f,
        0.f, 1.f,     0.0f, 1.0f
    };

    rect_vertex.indices = {
        0, 2, 1, // Primeiro triângulo
        2, 0, 3  // Segundo triângulo
    };

    glGenVertexArrays(1, &rect_vertex.VAO);
    glGenBuffers(1, &rect_vertex.VBO);
    glGenBuffers(1, &rect_vertex.EBO);

    glBindVertexArray(rect_vertex.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, rect_vertex.VBO);
    glBufferData(GL_ARRAY_BUFFER, rect_vertex.vertices.size() * sizeof(float), rect_vertex.vertices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_vertex.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect_vertex.indices.size() * sizeof(unsigned int), rect_vertex.indices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    // Atributos de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    rect_vertex.carregado = true;
}