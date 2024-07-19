#include "src/nucleo/gerenciador.h"
#include "Windows.h"

using namespace Bubble;

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
#endif

int INIT 
{
    Nucleo::Gerenciador gerenciador;

    if(!gerenciador.inicializacao())
        return -1;
    
    return 0;   
}
