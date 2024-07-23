#include "engine.hpp"
#include "gerenciador.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

float deltaTime = 0;
namespace Bubble::Nucleo 
{
    Engine::Engine() {}
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

        inputs = &w->ui.inputs;

        return true;
    }
    int Engine::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }
    void Engine::renderizar(Modo m, Vector2 viewportPos, Vector2 viewportSize)
    {
        float st = glfwGetTime();
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportSize.x, viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // Redimensionar o renderbuffer
            glBindRenderbuffer(GL_RENDERBUFFER, cam->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            // Atualizar cena
            gerenciadorDeCenas.cenaAtual()->camera_editor.inputs = inputs;
            int width = 0, height = 0;
            glfwGetFramebufferSize(glfwWindow, &width, &height);
            if(viewportSize.x == 0 && viewportSize.y == 0)
            gerenciadorDeCenas.atualizarCenaAtual(m, deltaTime, 0, 0, static_cast<float>(width), static_cast<float>(height));
            else
            gerenciadorDeCenas.atualizarCenaAtual(m, deltaTime, static_cast<float>(viewportPos.x), static_cast<float>(viewportPos.y), static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y));

            // Desligar framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Calcular deltaTime
        deltaTime = glfwGetTime() - st;
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