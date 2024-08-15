#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/arquivadores/imageloader.hpp"
#include "src/tempo/delta_time.hpp"
#include "engine.hpp"

using namespace Bubble::Nucleo;

Engine::Engine()
    {
        inicializacao();
        gerenciadorUi = new BubbleUI::Manager(this);
    }
// Inicialização
bool Engine::inicializacao()
    {
        // inicia glfw
        if (!glfwInit())
        {
            Debug::emitir(Debug::Erro, "GLFW não inicializado");
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindow = glfwCreateWindow(600, 480, "BubbleEngine", NULL, NULL);

        glfwMakeContextCurrent(glfwWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        // define o ícone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        GLFWimage icone = icone_.converterParaGlfw();

        if (icone_.carregado)
        {
            glfwSetWindowIcon(glfwWindow, 1, &icone);
        }
        defInputs(new Inputs::Inputs());

        return true;
    }
Bubble::Inputs::Inputs* Engine::obterGI() const
{
    return inputs;
};
// Deve definir inputs
void Engine::defInputs(Inputs::Inputs* inp)
{
    inputs = inp;
    gerenciadorDeCenas.defIputs(inp);
    glfwSetWindowUserPointer(glfwWindow, inputs);
    glfwSetCursorPosCallback(glfwWindow, mousePosCallBack);
    glfwSetKeyCallback(glfwWindow, callbackKey);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallBack);
}
// Deve retornar se deve fechar janela
int Engine::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }
// Deve Atualizar cena atual
void Engine::atualizar()
{
    glfwPollEvents();
    // Atualizar cena
    gerenciadorDeCenas.atualizarCenaAtual(Tempo::delta_time);
    // Atualizar UI
    gerenciadorUi->atualizar(Tempo::delta_time);
}
// Deve renderizar cena Atual
void Engine::renderizar(Vector4 rect_size)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Renderizar UI
    gerenciadorUi->renderizar();

    glfwSwapBuffers(glfwWindow);
    Tempo::endDT();
    Tempo::iniDT();
}
// Deve limpar engine
void Engine::limpar() const 
    {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }
// Deve salvar Cena 
bool Engine::salvarCena(unsigned int idx)
    {
        return true;
    }
