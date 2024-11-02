#include <windows.h>
#include <glad/glad.h>
#include "janela.hpp"

static bool iniciar_()
{
    // inicia glfw
    if (!glfwInit())
        return false;

    janela = glfwCreateWindow(800, 400, "Bubble Engine", NULL, NULL);
    if (!janela) return false;
    glfwSetWindowTitle(janela, title);

    glfwMakeContextCurrent(janela);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    // define o ícone da janela
    auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
    const GLFWimage icone = icone_.converterParaGlfw();
    
    if (icone_.carregado)   glfwSetWindowIcon(janela, 1, &icone);
    

    // defini contexto usado para interface gráfica
    contexto_ui = std::make_shared<BubbleUI::Contexto>();
    contexto_ui->def_inputs(janela);

    // Defini o painel de teste
    painel = std::make_shared<BubbleUI::Paineis::VisualizadorDeProjetos>(contexto_ui);

    return true;
}
static void novo_loop()
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, contexto_ui->tamanho.width, contexto_ui->tamanho.height);

    painel->atualizar();
    painel->renderizar();

    glfwSwapBuffers(janela);
    glfwPollEvents();
}
#ifdef NDEBUG
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#else
#define INIT main()
#endif
int INIT
{
    if (!iniciar_())return -1;

    // Loop principal aqui
    while (!glfwWindowShouldClose(janela))
    {
        novo_loop();
    }

    glfwTerminate();

	return 0;
}