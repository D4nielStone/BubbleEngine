/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/cor.hpp"
#include <string>
#include <src/util/vetor2.hpp>

namespace bubble
{
	struct texto : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_TEXTO;
		std::string frase;
		const float escala;
		bubble::vetor2<int> padding;
		bubble::cor cor;
		texto(const std::string& texto, float escala = 1.f) : frase(texto), escala(escala), cor({ 1.f, 1.f, 1.f, 1.f }), padding({0, 0}) {}
	};
} 