#include "quadrado.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/depuracao/debug.h"
#include "cassert"

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

Vector2 Bubble::Interface::Quadrado::obtPos() const
{
    return posicao;
}
Vector2 Bubble::Interface::Quadrado::obtTam() const
{
    return tamanho;
}
Color Bubble::Interface::Quadrado::obtCor() const
{
    return cor;
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

void Bubble::Interface::Quadrado::converterMouse(Vector2* mousepos)
{
    assert(inputs != nullptr); // Defina inputs para o quadrado
    assert(mousepos != nullptr); // Argumento nulo

    // Convertendo para coordenadas normalizadas OpenGL (-1 a 1)
    float mouseX = (inputs->mousex / janelaTam.w) * 2.0f - 1.0f;
    float mouseY = 1.0f - (inputs->mousey / janelaTam.h) * 2.0f;

    mouseX *= aspect * tamanho_ui;

    mousepos->x = mouseX;
    mousepos->y = mouseY;
}


bool Bubble::Interface::Quadrado::mouseEmCima(Vector2 mouse)
{
    if (mouse.x > posicao.x && mouse.x < posicao.x + tamanho.x &&
        mouse.y > posicao.y && mouse.y < posicao.y + tamanho.y)
    {
        Debug::emitir("MOUSE", "Mouse em cima");
        return true;
    }
    return false;
}

void Bubble::Interface::Quadrado::renderizar()
{
    shader.use();
    shader.setInt("imagem", 0);
    shader.setVec2("quadrado.tamanho", tamanho.x , tamanho.y );
    shader.setVec2("quadrado.posicao", posicaoG.x, posicaoG.y);
    shader.setVec3("cor", cor.r, cor.g, cor.b);
    shader.setMat4("projecao", glm::value_ptr(projecao));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Bubble::Interface::Quadrado::atualizar()
{
    // atualiza posição baseado no pai
    if (pai)
    {
        inputs = pai->inputs;
        posicaoG = posicao + pai->obtPos();
    }
    else
        posicaoG = posicao;
    // logica para arrastar janela
    glfwGetFramebufferSize(janelaglfw, &janelaTam.w, &janelaTam.h);
    Vector2 mouse;
    converterMouse(&mouse);
    if (mouseEmCima(mouse) && inputs->mouseClick)
    {
        inicioArrastoPos = posicao;
        inicioMouseArrasto = Vector2(mouse.x, mouse.y);
        mouseArrastando = true;
    }
    if (mouseArrastando && inputs->mouseEnter == GLFW_PRESS)
    {
        ArrastoPos = inicioArrastoPos + Vector2{ mouse.x - inicioMouseArrasto.x, mouse.y - inicioMouseArrasto.y };
    }
    else
    {
        mouseArrastando = false;
    }
    // lógica do click único
    if (inputs->mouseClick) {
        inputs->mouseClick = false;
    }
    if (inputs->mouseEnter == GLFW_PRESS) {
        if (!inputs->mouseClick && once == true) {
            inputs->mouseClick = true;
            once = false;
        }
    }
    else if (inputs->mouseEnter == GLFW_RELEASE) {
        once = true;
        inputs->mouseClick = false;

    }
    if (once) {
        inputs->mouseClick = false;
    }
    
    //projeção ortográfica
    aspect = janelaTam.w / janelaTam.h;
    glViewport(0, 0, janelaTam.w, janelaTam.h);

    float left = -aspect;
    float right = aspect;
    float bottom = -1.0f;
    float top = 1.0f;
    projecao = glm::ortho(left * tamanho_ui, right * tamanho_ui, bottom * tamanho_ui, top * tamanho_ui, 0.1f * tamanho_ui, -1.0f * tamanho_ui);
}

Bubble::Interface::Quadrado* Bubble::Interface::Quadrado::obtPai()
{
    return pai;
}

void Bubble::Interface::Quadrado::defPai(Quadrado* paia)
{
    pai = paia;
}

void Bubble::Interface::Quadrado::defInputs(Inputs::Inputs* inp)
{
    inputs = inp;
}
bool Bubble::Interface::Quadrado::arrastando()
{
    return mouseArrastando;
}
;

