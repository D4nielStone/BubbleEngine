#include "gerenciador.h" // Suponho que este seja o arquivo onde você incluiu o SceneManager e outras classes
#include "src/arquivadores/shader.h"

int main() {
    Bubble::Nucleo::Gerenciador gerenciador;
    auto cena1 = std::make_shared<Bubble::Nucleo::Scene>("Cena de teste");

    if (!gerenciador.inicializacao()) {
        std::cerr << "\n>> Não foi possível inicializar o gerenciador\n";
        return -1;
    }

    auto bola = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));
    auto chao = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/cube.dae"));
    auto cam = std::make_shared<Bubble::Entidades::Entidade>();

    chao->obterTransformacao()->definirPosicao(glm::vec3(0, -2, 0));
    chao->obterTransformacao()->definirEscala(glm::vec3(2, 0.5, 2));
    cam->obterTransformacao()->definirPosicao(glm::vec3(0,0,-5));
    
    cam->adicionarComponente(std::make_shared < Bubble::Componentes::Camera>());
    bola->adicionarComponente(std::make_shared<Bubble::Componentes::Codigo>("assets/scripts/rotacionar.lua"));

    cena1->adicionarEntidade(cam);
    cena1->adicionarEntidade(chao);
    cena1->adicionarEntidade(bola);

    gerenciador.gerenciadorDeCenas.adicionarCena(cena1);
    gerenciador.gerenciadorDeCenas.carregarCena(0);
    
    while (!gerenciador.pararloop()) {
        gerenciador.renderizar();
    }

    gerenciador.limpar();
    return 0;
}
