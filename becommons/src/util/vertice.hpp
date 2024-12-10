/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "vetor3.hpp"
#include "vetor2.hpp"
struct Vertice
{
	Vetor3<float> posicao, normal;
	Vetor2<float> uvcoords;
};