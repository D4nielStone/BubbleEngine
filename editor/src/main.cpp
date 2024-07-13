#include "src/nucleo/gerenciador.h"
#include "interface/ui.h"
#include "Windows.h"

using namespace Bubble;

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
#endif

int INIT {
    Nucleo::Engine engine;
    Interface::UI ui;

    if (!engine.inicializacao())
        return -1;
    
    const char* path = "C:/Users/Daniel/Desktop";
    engine.criarProjeto(path, "ProjetoBubble");
    
    ui.configurar(&engine);

    ui.novaJanela(Bubble::Interface::Janela::Editor);
//    ui.novaJanela(Bubble::Interface::Janela::Preview);
    
    while (!engine.pararloop())
    {
        ui.novoFrame();
        
        ui.renderizar();
    }
    ui.limpar();
    engine.limpar();
    return 0;   
}
