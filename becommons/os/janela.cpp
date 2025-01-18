#include <glad/glad.h>
#include "janela.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"

bubble::janela::~janela()
{
}
bubble::janela::janela(const char* nome, const char* icon_path)
{
    // inicia glfw
    if (!glfwInit())
    {
        debug::emitir(Erro, "Iniciando janela glfw");
        abort();
    }
    window = glfwCreateWindow(400, 400, nome, NULL, NULL);
    if (!window) {
        debug::emitir(Erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        debug::emitir(Erro, "Glad");
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

    glfwSetCursorPosCallback(window,bubble::mousePosCallBack);
    glfwSetMouseButtonCallback(window, bubble::mouseButtonCallBack);
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

void bubble::janela::swap() 
{
    bubble::vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.w, &tam.h);
    tamanho.h = tam.h;
    tamanho.w = tam.w;
    _Mtempo.calcularDT();
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