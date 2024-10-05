#include "src/nucleo/projeto.hpp"
#include "memory"
#include "Windows.h"

using namespace Bubble::Nucleo;

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
#endif

int INIT
{
    Projeto projeto("Projeto");

    while (!projeto.pararloop())
    {
        projeto.atualizar();
        projeto.renderizar();
    }
    return 0;   
}
