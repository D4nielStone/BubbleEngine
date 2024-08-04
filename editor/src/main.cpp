#include "src/nucleo/gerenciador.hpp"
#include "src/componentes/prototipo/terreno.h"
#include "src/arquivadores/arquivo3d.h"
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
    Bubble::Nucleo::Engine engine;

    engine.obterGC()->novaCena();

    while (!engine.pararloop())
    {
        engine.atualizar();
        engine.renderizar();
    }
    return 0;   
}
