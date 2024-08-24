#pragma once
#include <string>
#include <vector>
#include "becommons.hpp"
#include "src/util/includes.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Bubble {
    namespace Arquivadores {
        class BECOMMONS_DLL_API Arquivo3d {
        public:
            std::string Caminho;
            std::vector<Vertex> vertices;
            std::vector<Material*> materiais;

            Arquivo3d();
            Arquivo3d(std::string caminho);
            Arquivo3d(std::wstring caminho);
            void exibirInformacoes();
            std::string nome() const;
        private:
            const aiScene* cena;
            void carregarModelo(const std::string& caminho);
            void extrairDados();
            void extrairMateriais();
            std::string obterExtensaoArquivo(std::string& caminho);
        };
    }
};
