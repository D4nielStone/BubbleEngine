/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "vertice.hpp"
#include <vector>
#include "textura.hpp"
#include "src/arquivadores/shader.hpp"

class Malha 
{
public:
    std::vector<Vertice> vertices;
    std::vector<unsigned int> indices;
    std::vector<Textura> texturas;

    Malha(std::vector<Vertice> vertices, std::vector<unsigned int> indices, std::vector<Textura> textures);
    void desenhar(Shader& shader);
private:
    //  dados de renderizacao
    unsigned int VAO, VBO, EBO;

    void definirBuffers();
};