#include <glad/glad.h>
#include "janela.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"

static void callbackSize(GLFWwindow* window, int w, int h)
{
    auto janela = static_cast<Janela*>(glfwGetWindowUserPointer(window));
    janela->tamanho.x = w;
    janela->tamanho.y = h;
}

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
    auto icone_ = ImageLoader("icon.ico");
    const GLFWimage icone = icone_.converterParaGlfw();

    if (icone_.carregado)   glfwSetWindowIcon(window, 1, &icone);

    // ativa blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowSizeCallback(window, callbackSize);
    glfwSetWindowUserPointer(window, this);
    glfwGetWindowSize(window, &tamanho.x, &tamanho.y);
}

double elapsedTime{ 0 };

void Janela::iniciarLoop() const
{
    /// Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, tamanho.x, tamanho.y);
        double deltaTime = glfwGetTime() - elapsedTime;
        glfwPollEvents();

        for (auto& [tipo, sistema] : sistemas) {
                sistema->atualizar(deltaTime);
        }

        elapsedTime = glfwGetTime();

        glfwSwapBuffers(window);
    }
}
