#include "quadrado.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/depuracao/debug.h"
#include "src/depuracao/assert.hpp"

Bubble::Interface::Quadrado::Quadrado() : janelaglfw(glfwGetCurrentContext())
{
    // Inicializar valores padrão, se necessário
    glfwGetFramebufferSize(janelaglfw, &inicioJanelaTam.w, &inicioJanelaTam.h);
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
Vector2 Bubble::Interface::Quadrado::obtPosNorm() const
{
    return Vector2();
}
Vector2 Bubble::Interface::Quadrado::obtTamNorm() const
{
    return Vector2();
}
Vector2 Bubble::Interface::Quadrado::obtNorm() const
{
    Vector2 norm;
    norm.x = posicao.x * (inicioJanelaTam.w / 2) + (inicioJanelaTam.w / 2);
    norm.y = posicao.y * (inicioJanelaTam.h / 2) + (inicioJanelaTam.h / 2);
    norm.w = tamanho.x * inicioJanelaTam.w;
    norm.h = tamanho.y * inicioJanelaTam.h;

    return norm;
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
       1, 0.0f,    1.0f, 0.0f,
       1, 1, 1.0f, 1.0f,
        0.0f, 1,    0.0f, 1.0f
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
    ASSERT(inputs != nullptr); // Defina inputs para o quadrado
    ASSERT(mousepos != nullptr); // Argumento nulo

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
        return true;
    }
    return false;
}

void Bubble::Interface::Quadrado::renderizar()
{
    glViewport(0, 0, janelaTam.w, janelaTam.h);
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
    // Atualiza a matriz de projeção somente se a dimensão da janela mudar
    if (janelaTam.w != ultimoTam.w || janelaTam.h != ultimoTam.y) {
        ultimoTam.w = janelaTam.w;
        ultimoTam.y = janelaTam.h;

        aspect = static_cast<float>(janelaTam.w) / janelaTam.h;
        tamanho_ui = 1.0f;

        left = -aspect * tamanho_ui;
        right = aspect * tamanho_ui;
        bottom = -1.0f * tamanho_ui;
        top = 1.0f * tamanho_ui;

        projecao = glm::ortho(left, right, bottom, top, 0.1f, -1.0f);
    }
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

