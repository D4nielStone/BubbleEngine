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
    Gerenciador gerenciador;
    if(!gerenciador.inicializacao())
        return -1;

    while (!gerenciador.pararLoop())
    {
        gerenciador.renderizar();
    }

    gerenciador.limpar();

    return 0;   
}
