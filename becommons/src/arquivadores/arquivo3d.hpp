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
            Node RootNode;            // Estrutura hier�rquica de n�s na cena

            Arquivo3d();
            Arquivo3d(const std::string& caminho);
            Arquivo3d(const std::wstring& caminho);        // Construtor com caminho em std::wstring
            std::string nome() const;               // M�todo para obter o nome da cena

        private:
            const aiScene* cena;                    // Ponteiro para a cena carregada
            void carregar(const std::string& caminho);
            std::vector<Textura> processarTextura(aiMaterial* mat, aiTextureType type, const std::string& typeName); // M�todo para processar texturas
            Node processarNos(aiNode* node, unsigned int depth);                    // M�todo para processar n�s da cena
            Material processarMateriais(aiMaterial* material);                 // M�todo para processar malhas
        };
    }
};