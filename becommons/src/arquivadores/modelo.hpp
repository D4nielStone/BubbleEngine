/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "src/util/malha.hpp"
#include "src/util/textura.hpp"
#include <assimp/scene.h>

class Modelo
{
public:
    Modelo(const char* path)
    {
        carregarModelo(path);
    }
    void Desenhar(Shader& shader);
private:
    // dados de malha
    std::vector<Malha> malhas;
    std::string diretorio;

    void carregarModelo(const std::string &path);
    void processarNo(aiNode* node, const aiScene* scene);
    Malha processarMalha(aiMesh* mesh, const aiScene* scene);
    std::vector<Textura> carregarMaterialETexturas(aiMaterial* mat, aiTextureType type,
        std::string typeName) const;
};