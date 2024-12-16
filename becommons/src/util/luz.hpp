/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "cor.hpp"
#include "vetor3.hpp"
namespace bubble
{
    struct luz
    {
        vetor3<float> posicao;
        bubble::cor cor_ambiente;
        bubble::cor cor_difusa;
        bubble::cor cor_especular;
    };
}