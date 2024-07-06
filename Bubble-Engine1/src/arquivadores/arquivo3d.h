#pragma once
#include <string>
#include <vector>
#include "includes.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Bubble {
    namespace Arquivadores {
        class Arquivo3d {
        public:
            std::string Caminho;
            std::vector<Vertex> vertices;
            std::vector<Material> materiais;

            Arquivo3d(std::string caminho) : Caminho(caminho) {
                carregarModelo(Caminho);
            }

            void exibirInformacoes();

        private:
            const aiScene* cena;

            void carregarModelo(const std::string& caminho);

            void extrairDados();

            void extrairMateriais();

            std::string obterExtensaoArquivo(std::string& caminho);
        };
    }
};
