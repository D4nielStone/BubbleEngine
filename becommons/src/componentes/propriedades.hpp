/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include <string>

struct Propriedades : Componente
{
	std::string nome;
	static constexpr MascaraComponente mascara = COMPONENTE_PROPRIEDADES;
	std::string tag;
	bool ativo;
	Propriedades(const std::string& name = "", const std::string& tag = "", bool active = true)
		: nome(name), tag(tag), ativo(active) {
	}
};