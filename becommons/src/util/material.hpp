/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "cor.hpp"
#include "textura.hpp"
#include <vector>

struct Material
{
	Material() = default;
	Material(const std::vector<Textura>& texturas, const Cor& difusa = Cor(1, 1, 1), const Cor& especular = Cor(1.f, 1.f, 1.f)) :difusa(difusa), especular(especular), texturas(texturas){}
	std::vector<Textura> texturas;
	Cor difusa;
	Cor especular;
};