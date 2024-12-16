/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "vertice.hpp"
#include <vector>
#include "textura.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/util/material.hpp"

namespace bubble
{
    class malha
    {
    public:
        std::vector<vertice> vertices;
        std::vector<unsigned int> indices;
        material material;

        malha(std::vector<vertice> vertices, std::vector<unsigned int> indices, bubble::material& material);
        ~malha();
        void desenhar(bubble::shader& shader);
    private:
        //  dados de renderizacao
        unsigned int VAO, VBO, EBO;

        void definirBuffers();
    };
}