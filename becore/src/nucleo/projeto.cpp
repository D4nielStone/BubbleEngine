#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/arquivadores/imageloader.hpp"
#include "projeto.hpp"

using namespace Bubble::Nucleo;

Projeto::Projeto(const std::string &nome) : gerenciadorDeCenas(std::make_shared<Cena::SceneManager>())
{
    Nome = nome;
    Bubble::Cena::definirSceneManager(gerenciadorDeCenas);
    inicializacao();
    gerenciadorUi = std::make_shared< BubbleUI::Manager>(this);
}
// Inicialização
bool Projeto::inicializacao()
{
    // inicia glfw
    if (!glfwInit())
    {
        Debug::emitir(Debug::Erro, "GLFW não inicializado");
        return false;
    }

    glfwWindow = glfwCreateWindow(800, 500, "Bubble Engine-Editor-copyright 2024 Daniel Oliveira", NULL, NULL);
    glfwMaximizeWindow(glfwWindow);
    glfwSwapInterval(1);

    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    NomeGPU = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    VercaoGL = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    glfwSetWindowTitle(glfwWindow, std::string("Bubble Engine-Editor-copyright 2024 Daniel Oliveira | " + Nome + " | GL " + VercaoGL).c_str());
    // define o ícone da janela
    auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
    const GLFWimage icone = icone_.converterParaGlfw();

    if (icone_.carregado)
    {
        glfwSetWindowIcon(glfwWindow, 1, &icone);
    }

    // defini inputs da janela
    defInputs(std::make_shared<Inputs::Inputs>());

    return true;
}
std::shared_ptr<Bubble::Inputs::Inputs> Projeto::obterGI() const
{
    return inputs;
}
std::shared_ptr < BubbleUI::Manager> Bubble::Nucleo::Projeto::obterGU() const
{
    return gerenciadorUi;
}

// Deve definir inputs
void Projeto::defInputs(std::shared_ptr<Inputs::Inputs> inp)
{
    inputs = inp;
    gerenciadorDeCenas->defIputs(inp);
    glfwSetWindowUserPointer(glfwWindow, inputs.get());
    glfwSetCursorPosCallback(glfwWindow, mousePosCallBack);
    glfwSetKeyCallback(glfwWindow, callbackKey);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallBack);
    glfwSetCharCallback(glfwWindow, charCallback);
}
// Deve retornar se deve fechar janela
int Projeto::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }

// Deve Atualizar cena atual
void Projeto::atualizar()
{
    glfwPollEvents();  // Processa eventos do GLFW
    // Atualiza cena e UI
    gerenciadorUi->atualizar();
    gerenciadorDeCenas->atualizarCenaAtual();
}

// Deve renderizar cena Atual
void Projeto::renderizar() const
{
    // Renderizar cena e UI
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gerenciadorDeCenas->renderizarCenaAtual();
    gerenciadorUi->renderizar();

    glfwSwapBuffers(glfwWindow);
}
// Deve limpar Projeto
void Projeto::limpar() const
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}
// Deve salvar Cena 
bool Projeto::salvarCena(unsigned int idx)
{
    return true;
}