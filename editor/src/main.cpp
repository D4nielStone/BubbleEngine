#include "src/nucleo/engine.hpp"
#include "memory"
#include "Windows.h"

using namespace Bubble::Nucleo;

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
#endif

static int INIT
{
    Bubble::Nucleo::Engine engine;

    while (!engine.pararloop())
    {
        engine.atualizar();
        engine.renderizar();
    }
    return 0;   
}
