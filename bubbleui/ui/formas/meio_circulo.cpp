/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "meio_circulo.hpp"
#include "src/depuracao/debug.hpp"
#include "src/depuracao/assert.hpp"

constexpr double M_PI = 3.14159265359;
using namespace BubbleUI::Formas;

MeioCirculo::MeioCirculo(std::shared_ptr<Contexto> ctx, const bubble::vetor4<int> &rect) : retangulo(rect), contexto(ctx)
{
    definirBuffers();
}

// Deve retornar o tamanho/posicao
bubble::vetor4<int> MeioCirculo::obtMeioCirculo() const
{
    return retangulo;
}

bubble::vetor4<float> BubbleUI::Formas::MeioCirculo::obtMeioCirculoNDC() const
{
    return coord_ndc;
}

void MeioCirculo::definirTamanho(const Vector2 &tam)
{
    retangulo.w = tam.x;
    retangulo.h = tam.y;
}

void MeioCirculo::definirPosicao(const Vector2 &pos)
{
    retangulo.x = static_cast<float>(pos.x);
    retangulo.y = static_cast<float>(pos.y);
}

void MeioCirculo::adiTam(const Vector2 &tam)
{
    retangulo.w += tam.x;
    retangulo.h += tam.y;
}

void MeioCirculo::adiPos(const Vector2 &pos)
{
    retangulo.x += pos.x;
    retangulo.y += pos.y;
}
void BubbleUI::Formas::MeioCirculo::definirRetangulo(const bubble::vetor4<int>& rect)
{
    retangulo = rect;
}
// Deve definir cor base
void MeioCirculo::defCor(const bubble::cor &cor)
{
    *cor_base = cor;
}
void BubbleUI::Formas::MeioCirculo::defCor(bubble::cor* cor)
{
    cor_base = cor;
}
// Deve atualizar
void MeioCirculo::atualizar()
{
    if (tamanhoDoContextoAnterior != contexto->tamanho || tamanhoAnterior != retangulo)
    {
        paraNDC();
        tamanhoAnterior = retangulo;
        tamanhoDoContextoAnterior = contexto->tamanho;
    }
}

// Deve renderizar
void MeioCirculo::renderizar()
{
    shader.use();
    shader.setVec2("quadrado.tamanho", coord_ndc.w, coord_ndc.h);
    shader.setVec2("quadrado.posicao", coord_ndc.x, coord_ndc.y);
    shader.setCor("quadrado.cor", *cor_base);
    glBindVertexArray(halfcircle_vertex.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(halfcircle_vertex.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
// Deve transformar coordenadas pixel para NDC
bubble::vetor4<float> MeioCirculo::paraNDC()
{
    coord_ndc.w = (retangulo.w * 2.f) / contexto->tamanho.width;
    coord_ndc.h = -(2.0f * retangulo.h) / contexto->tamanho.height;
    coord_ndc.x = (retangulo.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * retangulo.y) / contexto->tamanho.height;

    return coord_ndc;
}
// deve definir buffers do quadrado
void MeioCirculo::definirBuffers()
{
    if (halfcircle_vertex.carregado)    return;
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Glad";
        return;
    }

    double angle{};
    // Inicializa o ângulo em 0
    angle = 0;

    halfcircle_vertex.vertices.push_back(0);
    while (angle <= 90)
    {
        halfcircle_vertex.vertices.push_back(cos(angle * M_PI / 180)); // Converte para radianos
        halfcircle_vertex.vertices.push_back(sin(angle * M_PI / 180)); // Converte para radianos
        angle += 10; // Incrementa o ângulo
    }

    int numVertices = angle / 10 + 1; // Total de vértices, considerando que angle foi de 0 a 90
    for (int i = 0; i < numVertices - 1; ++i) {
        halfcircle_vertex.indices.push_back(i);      // Primeiro vértice do segmento
        halfcircle_vertex.indices.push_back(i + 1);  // Segundo vértice do segmento
        halfcircle_vertex.indices.push_back(0);
    }

    glGenVertexArrays(1, &halfcircle_vertex.VAO);
    glGenBuffers(1, &halfcircle_vertex.VBO);
    glGenBuffers(1, &halfcircle_vertex.EBO);

    glBindVertexArray(halfcircle_vertex.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, halfcircle_vertex.VBO);
    glBufferData(GL_ARRAY_BUFFER, halfcircle_vertex.vertices.size() * sizeof(float), halfcircle_vertex.vertices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, halfcircle_vertex.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, halfcircle_vertex.indices.size() * sizeof(unsigned int), halfcircle_vertex.indices.data(), GL_STATIC_DRAW); // Corrigido para tamanho em bytes

    // Atributos de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    halfcircle_vertex.carregado = true;
}
