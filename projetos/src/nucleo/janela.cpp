
// Copyright (c) 2024 Daniel Oliveira
#include <windows.h>
#include <glad/glad.h>
#include "janela.hpp"

GLFWwindow* janela;

static bool iniciar_()
{
    // Cria novo contexto ui
    auto ctx = BubbleUI::novoContexto(title, Vector4{100, 100, 800, 400});
    ctx->adicionarVP(true);

    return true;
}
#ifdef NDEBUG
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#else
#define INIT main()
#endif
int INIT
{
    if (!iniciar_()) return -1;

    while (!BubbleUI::parar())
    {
        // Aqui você pode colocar algo opcional, como dormir para evitar alta carga de CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cerr << "closing\n";

    glfwTerminate();

    return 0;
}
