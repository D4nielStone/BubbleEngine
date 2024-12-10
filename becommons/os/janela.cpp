#include <glad/glad.h>
#include "janela.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"

Janela::Janela(const char* nome)
{
    // inicia glfw
    if (!glfwInit())
    {
        Debug::emitir(Erro, "Iniciando janela glfw");
        abort();
    }
    window = glfwCreateWindow(800, 400, nome, NULL, NULL);
    if (!window) {
        Debug::emitir(Erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Debug::emitir(Erro, "Glad");
        abort();
    }

    // define o ícone da janela
    //auto icone_ = Bubble::Arquivadores::ImageLoader("icon.ico");
    //const GLFWimage icone = icone_.converterParaGlfw();

    ///if (icone_.carregado)   glfwSetWindowIcon(window, 1, &icone);

    // ativa blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Janela::atualizar() const
{
    glfwPollEvents();
}

void Janela::desenhar() const
{
    glfwSwapBuffers(window);
}
