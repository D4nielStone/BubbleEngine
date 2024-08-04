#include "engine.hpp"
#include "gerenciador.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

float deltaTime = 0;
namespace Bubble::Nucleo 
{
    Engine::Engine() 
    {
        inicializacao();
    }
    // inicialização GLFW e GLAD
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

        defInputs(new Inputs::Inputs());

        return true;
    }
    // Deve definir inputs
    void Engine::defInputs(Inputs::Inputs* inp)
    {
        inputs = inp;
        gerenciadorDeCenas.defIputs(inp);
        glfwSetWindowUserPointer(glfwWindow, inputs);
        glfwSetCursorPosCallback(glfwWindow, mousePosCallBack);
        glfwSetKeyCallback(glfwWindow, keyCallback);
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
        float st = glfwGetTime();
        // Atualizar cena
        gerenciadorDeCenas.atualizarCenaAtual(deltaTime);
        // Calcular deltaTime
        deltaTime = glfwGetTime() - st;
    }
    // Deve renderizar cena Atual
    void Engine::renderizar()
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //gerenciadorDeCenas.cenaAtual()->camera_editor.desenharFrame();

        gerenciadorDeCenas.renderizarCenaAtual(Vector2{0, 0, 600, 480});
            
        // Desligar framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
}