/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "src/util/malha.hpp"
#include "src/util/textura.hpp"
#include <assimp/scene.h>
#include <filesystem>

namespace bubble
{
    class modelo
    {
    public:
        modelo(const char* diretorio)
        {
            if (std::filesystem::exists(diretorio))
                carregarmodelo(diretorio);
            else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
            {

                carregarmodelo(std::filesystem::absolute(diretorio).string().c_str());
            }
        }
        malha* obterMalha(int idx) { return &malhas[idx]; };
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