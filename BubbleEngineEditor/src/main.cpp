#include "src/nucleo/gerenciador.h"
#include "interface/ui.h"

float image_w = 640, image_h = 480;

int main() {
    Bubble::Nucleo::Gerenciador gerenciador;
    Bubble::Interface::UI ui;

    if (!gerenciador.inicializacao())
        return -1;
    
    const char* path = "C:/Users/Daniel/Desktop";
    gerenciador.criarProjeto(path, "ProjetoBubble");
    auto ze = std::make_shared<Bubble::Entidades::Entidade>("Zé");
    ze->adicionarComponente(std::make_shared<Bubble::Componentes::Camera>());
    ze->obterTransformacao()->definirPosicao(glm::vec3(0,0,-50));
    gerenciador.gerenciadorDeCenas.cenaAtual()->adicionarEntidade(ze);

    ui.configurar(gerenciador.glfwWindow);
    ui.novaJanela(Bubble::Interface::Janela::Editor, gerenciador.gerenciadorDeCenas.camera_do_editor.FBO);
    ui.novaJanela(Bubble::Interface::Janela::Preview);
    
    while (!gerenciador.pararloop())
    {
        ui.novoFrame();

        gerenciador.renderizar(Modo::Editor, ui.window_size);
        
        ui.renderizar();
    }
    ui.limpar();
    gerenciador.limpar();
    return 0;   
}
