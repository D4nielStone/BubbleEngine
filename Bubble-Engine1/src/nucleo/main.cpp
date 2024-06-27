#include "gerenciador.h"

int main() {

    Bubble::Nucleo::Gerenciador gerenciador;

    if (!gerenciador.inicializacao()){
        std::cerr << "\n>> Não foi possível inicializar o gerenciador\n";
        return -1;
    }
    Shader phong;
    phong.compilar("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    phong.use();

    Bubble::Arquivadores::Arquivo3d arma("assets/models/minecraft-tree/source/Mineways2Skfb.obj");
    Bubble::Entidades::Entidade ent(arma);
    Bubble::Entidades::Entidade camera(arma);

    camera.obterTransformacao()->definirPosicao(glm::vec3(0,0,-3));

    camera.adicionarComponente(std::make_shared<Bubble::Componentes::Camera>());
    ent.adicionarComponente(std::make_shared<Bubble::Componentes::Codigo>("assets/scripts/rotacionar.lua"));

    for (auto& c : ent.listaDeComponentes()) {
        c->configurar();
        c->definirShader(phong);
    }
    for (auto& c : camera.listaDeComponentes()) {
        c->configurar();
        c->definirShader(phong);
    }
    while (!gerenciador.pararloop()) {
        gerenciador.atualizar(&camera);
        gerenciador.renderizar(&camera);
        gerenciador.atualizar(&ent);
        gerenciador.renderizar(&ent);
    }
    gerenciador.limpar();
    return 0;
}
