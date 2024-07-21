#include "quadrado.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Bubble::Interface::Quadrado::Quadrado() : janelaglfw(glfwGetCurrentContext())
{
    // Inicializar valores padrão, se necessário
    this->posicao = Vector2(0.0f, 0.0f);
    this->tamanho = Vector2(1.0f, 1.0f);
    definirBuffer();
}

Bubble::Interface::Quadrado::Quadrado(Vector2 posicao, Vector2 tamanho):janelaglfw(glfwGetCurrentContext())
{
    this->posicao = posicao;
    this->tamanho = tamanho;
    definirBuffer();
}

void Bubble::Interface::Quadrado::defPos(Vector2 pos)
{
    posicao = pos;
}

void Bubble::Interface::Quadrado::defTam(Vector2 tam)
{
    tamanho = tam;
}

void Bubble::Interface::Quadrado::defCor(Color co)
{
    cor = co;
}

void Bubble::Interface::Quadrado::definirBuffer()
{
    shader.compilar("assets/shaders/layout.vert", "assets/shaders/layout.frag");
    // Define os vértices para um quadrado
    float vertices[] = {
        // Posições         // Coordenadas de textura
        0.0f, 0.0f,         0.0f, 0.0f,
        tamanho.x, 0.0f,    1.0f, 0.0f,
        tamanho.x, tamanho.y, 1.0f, 1.0f,
        0.0f, tamanho.y,    0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2, // Primeiro triângulo
        2, 3, 0  // Segundo triângulo
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atributos de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Bubble::Interface::Quadrado::renderizar()
{
    shader.use();
    shader.setVec2("quadrado.tamanho", tamanho.x, tamanho.y);
    shader.setVec2("quadrado.posicao", posicaoG.x, posicaoG.y);
    shader.setVec3("cor", cor.r, cor.g, cor.b);
    shader.setMat4("projecao", glm::value_ptr(projecao));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Bubble::Interface::Quadrado::atualizar()
{
    if (pai)
    {
        posicaoG = posicao + pai->obtPos();
    }
    else
    {
        posicaoG = posicao;
    }
    int w, h;
    glfwGetFramebufferSize(janelaglfw, &w, &h);
    float aspect = static_cast<float>(w) / h;  // Corrigir a divisão para float
    glViewport(0, 0, w, h);
    float left = -aspect;
    float right = aspect;
    float bottom = -1.0f;
    float top = 1.0f;
    projecao = glm::ortho(left, right, bottom, top, 0.1f, -1.0f);
}

