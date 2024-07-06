#include "gerenciador.h" 
#include "src/arquivadores/shader.h"

int main() {
    Bubble::Nucleo::Gerenciador gerenciador;

    if (!gerenciador.inicializacao()) {
        return -1;
    }

    const char* path = "C:/Users/Daniel/Desktop";

    gerenciador.criarProjeto(path, "ProjetoBubble");
    
    while (!gerenciador.pararloop()) {
        gerenciador.renderizar();
    }

    gerenciador.limpar();
    return 0;
}
