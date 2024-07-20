#include "src/nucleo/engine.hpp"
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
    Engine engine;
    if(!engine.inicializacao())
        return -1;
    //Cria cena
    auto scene = std::make_shared<Scene>("Cena1");
    //Cira e configura entidade Terreno
    auto terreno = std::make_shared<Bubble::Entidades::Entidade>("Terreno");
    terreno->adicionarComponente(std::make_shared<Bubble::Componentes::Terreno>());
    
    //Cria e configura entidade Camera
    auto camera = std::make_shared<Bubble::Entidades::Entidade>("Cam");
    camera->adicionarComponente(std::make_shared<Bubble::Componentes::Camera>());
    camera->obterTransformacao()->definirPosicao(glm::vec3(10, 10, 10));
    //Cria e configura entidade Esfera
    auto esfera = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));
    
    scene->adicionarEntidade(esfera);
    scene->adicionarEntidade(camera);
    scene->adicionarEntidade(terreno);

    engine.obterGC()->adicionarCena(scene);
    engine.obterGC()->carregarCena(0);

    while (!engine.pararloop())
    {
        engine.renderizar(Modo::Editor, ImVec2(0, 0), ImVec2(640, 480));
    }

    engine.limpar();

    return 0;   
}
