#include <glad/glad.h>
#include "janela.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"

static void callbackSize(GLFWwindow* window, int w, int h)
{
    auto janela = static_cast<bubble::janela*>(glfwGetWindowUserPointer(window));
    janela->tamanho.w = w;
    janela->tamanho.h = h;
}
bubble::janela::~janela()
{
}
bubble::janela::janela(const char* nome, const char* icon_path)
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

    bubble::imageLoader icone_;

    // define o ícone da janela
    if(icon_path)
    icone_ = bubble::imageLoader(icon_path);
    else
    icone_ = bubble::imageLoader("icon.ico");
    const GLFWimage icone = icone_.converterParaGlfw();

    if (icone_.carregado)   glfwSetWindowIcon(window, 1, &icone);

    // ativa blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowSizeCallback(window, callbackSize);
    glfwSetCursorPosCallback(window,bubble::mousePosCallBack);
    glfwSetKeyCallback(window,bubble::callbackKey);
    glfwSetWindowUserPointer(window, this);

    bubble::vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.w, &tam.h);
    tamanho.h = tam.h;
    tamanho.w = tam.w;
}

void bubble::janela::poll() const
{
    glfwPollEvents();
}

void bubble::janela::swap() const
{
    glfwSwapBuffers(window);
}

void bubble::janela::viewport() const
{
    glViewport(0, 0, tamanho.w, tamanho.h);
}

void bubble::janela::nome(const char* novo_nome) const
{
    glfwSetWindowTitle(window, novo_nome);
}