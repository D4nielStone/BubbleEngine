#include <glad/glad.h>
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

// Callback de erro
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

bubble::janela::~janela()
{
}
bubble::janela::janela(const char* nome, const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
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
    
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetCursorPosCallback(window,bubble::mousePosCallBack);
    glfwSetMouseButtonCallback(window, bubble::mouseButtonCallBack);
    glfwSetKeyCallback(window,bubble::callbackKey);
    glfwSetWindowUserPointer(window, this);

    bubble::vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.w, &tam.h);
    tamanho.y = tam.h;
    tamanho.x = tam.w;
}

void bubble::janela::poll() const
{

    glfwPollEvents();
}

void bubble::janela::swap() 
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    tamanho.y = h;
    tamanho.x = w;
    _Mtempo.calcularDT();
    glfwSwapBuffers(window);
}

void bubble::janela::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void bubble::janela::nome(const char* novo_nome) const
{
    glfwSetWindowTitle(window, novo_nome);
}