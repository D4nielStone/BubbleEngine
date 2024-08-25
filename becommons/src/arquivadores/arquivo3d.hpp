#pragma once
#include <string>
#include <vector>
#include "becommons.hpp"
#include "src/util/includes.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "src/util/includes.hpp"

namespace Bubble {
    namespace Arquivadores {
        class BECOMMONS_DLL_API Arquivo3d {
        public:
            std::string Caminho;                    // Caminho para o arquivo 3D
            Node RootNode;            // Estrutura hierárquica de nós na cena

            Arquivo3d();
            Arquivo3d(const std::string& caminho);
            Arquivo3d(const std::wstring& caminho);        // Construtor com caminho em std::wstring
            std::string nome() const;               // Método para obter o nome da cena

        private:
            const aiScene* cena;                    // Ponteiro para a cena carregada
            void carregar(const std::string& caminho);
            std::vector<Textura> processarTextura(aiMaterial* mat, aiTextureType type, const std::string& typeName); // Método para processar texturas
            Node processarNos(aiNode* node, unsigned int depth);                    // Método para processar nós da cena
            Material processarMateriais(aiMaterial* material);                 // Método para processar malhas
        };
    }
};