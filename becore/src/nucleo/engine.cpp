#include "engine.hpp"
#include "gerenciador.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

float deltaTime = 0;
namespace Bubble::Nucleo 
{
    Engine::Engine() {}
    void Engine::defInputs(Inputs::Inputs* inp)
    {
        inputs = inp;
        gerenciadorDeCenas.cenaAtual()->camera_editor.inputs = inp;
    }
    // inicialização GLFW e GLAD
    bool Engine::inicializacao(Gerenciador* w)
    {
        // inicia glfw
        if (!glfwInit())
        {
            Debug::emitir(Debug::Erro, "GLFW não inicializado");
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindow = w->obterJanela();

        defInputs(&w->ui.inputs);

        return true;
    }
    int Engine::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }
    void Engine::atualizar()
    {
        float st = glfwGetTime();
        // Atualizar cena
        gerenciadorDeCenas.atualizarCenaAtual(deltaTime);
        // Calcular deltaTime
        deltaTime = glfwGetTime() - st;
    }
    void Engine::renderizar(Modo m, Vector2 viewportSize)
    {
        auto cena = gerenciadorDeCenas.cenaAtual();
        Componentes::Camera* cam = nullptr;

        if (m == Modo::Editor)
            cam = &cena->camera_editor;
            
        else if (cena->camera_principal)
            cam = cena->camera_principal;

        if (cam)
        {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            // Bind framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, cam->FBO);
            
            // Redimensionar o texture color buffer
            glBindTexture(GL_TEXTURE_2D, cam->textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportSize.w, viewportSize.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // Redimensionar o renderbuffer
            glBindRenderbuffer(GL_RENDERBUFFER, cam->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.w, viewportSize.h);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            gerenciadorDeCenas.renderizarCenaAtual(viewportSize);
            
            // Desligar framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

    }

    void Engine::limpar() const 
    {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }
    bool Engine::salvarCena(unsigned int idx)
    {
        return true;
    }
}