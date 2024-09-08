#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/arquivadores/imageloader.hpp"
#include "engine.hpp"
#include <thread>
#include <future>

using namespace Bubble::Nucleo;

Engine::Engine()
    {
        gerenciadorDeCenas = std::make_shared<Cena::SceneManager>();
        Bubble::Cena::definirSceneManager(gerenciadorDeCenas);
        inicializacao();
        gerenciadorUi = std::make_shared< BubbleUI::Manager>(this);
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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindow = glfwCreateWindow(800, 500, "BubbleEditor v0.1-alpha", NULL, NULL);
        glfwMaximizeWindow(glfwWindow);

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
        defInputs(std::make_shared<Inputs::Inputs>());

        obterGC()->novaCena();

        return true;
    }
std::shared_ptr<Bubble::Inputs::Inputs> Engine::obterGI() const
{
    return inputs;
}
std::shared_ptr < BubbleUI::Manager> Bubble::Nucleo::Engine::obterGU() const
{
    return gerenciadorUi;
}

// Deve definir inputs
void Engine::defInputs(std::shared_ptr<Inputs::Inputs> inp)
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
int Engine::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }

// Deve Atualizar cena atual
void Engine::atualizar()
{
    glfwPollEvents();  // Processa eventos do GLFW
    // Atualiza cena e UI
    gerenciadorDeCenas->atualizarCenaAtual();
    gerenciadorUi->atualizar();

}

// Deve renderizar cena Atual
void Engine::renderizar(Vector4 rect_size)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Renderizar cena e UI
    gerenciadorDeCenas->renderizarCenaAtual();
    gerenciadorUi->renderizar();

    glfwSwapBuffers(glfwWindow);
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
