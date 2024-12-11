/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "vertice.hpp"
#include <vector>
#include "textura.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/util/material.hpp"

class Malha 
{
public:
    std::vector<Vertice> vertices;
    std::vector<unsigned int> indices;
    Material material;

    Malha(std::vector<Vertice> vertices, std::vector<unsigned int> indices, Material& material);
    void desenhar(Shader& shader);
private:
    //  dados de renderizacao
    unsigned int VAO, VBO, EBO;

    void definirBuffers();
};