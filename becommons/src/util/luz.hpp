/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "cor.hpp"
#include "vetor3.hpp"
struct Luz
{
    Vetor3<float> posicao;
    Cor cor_ambiente;
    Cor cor_difusa;
    Cor cor_especular;
};