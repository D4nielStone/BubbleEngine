
// Copyright (c) 2024 Daniel Oliveira
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
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
    auto icone_ = Bubble::Arquivadores::ImageLoader("icon.ico");
    const GLFWimage icone = icone_.converterParaGlfw();
    
    if (icone_.carregado)   glfwSetWindowIcon(janela, 1, &icone);
    
    // Cria novo contexto ui
    BubbleUI::novoContexto(janela);
    BubbleUI::adicionarPainel(janela, new BubbleUI::Paineis::VisualizadorDeProjetos(true));

    return true;
}
static void novo_loop()
{
    BubbleUI::atualizarContexto(janela);
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
