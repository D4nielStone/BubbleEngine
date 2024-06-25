#include "gerenciador.h"

int main() {
    Bubble::Nucleo::Gerenciador gerenciador;

    if (!gerenciador.inicializacao()){
        std::cerr << "\n>> Não foi possível inicializar o gerenciador\n";
        return -1;
    }

    Bubble::Arquivadores::Arquivo3d arma("assets/models/gun/scene.gltf");
    Bubble::Entidades::ObjetoGeral og(arma);

    for (auto& c : og.listaDeComponentes()) {
        c->configurar();
    }
    while (!gerenciador.pararloop()) {
        gerenciador.renderizar(&og);
    }
    gerenciador.limpar();
    return 0;
}
