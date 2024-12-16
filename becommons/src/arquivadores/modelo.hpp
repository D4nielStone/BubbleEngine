/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "src/util/malha.hpp"
#include "src/util/textura.hpp"
#include <assimp/scene.h>

namespace bubble
{
    class modelo
    {
    public:
        modelo(const char* path)
        {
            carregarmodelo(path);
        }
        void desenhar(bubble::shader& shader);
    private:
        // dados de malha
        std::vector<malha> malhas;
        std::string diretorio;

        void carregarmodelo(const std::string& path);
        void processarNo(aiNode* node, const aiScene* scene);
        malha processarMalha(aiMesh* mesh, const aiScene* scene);
        std::vector<textura> carregarMaterialETexturas(aiMaterial* mat, aiTextureType type,
            std::string typeName) const;
    };
}