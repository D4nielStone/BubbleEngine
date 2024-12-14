/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/arquivadores/modelo.hpp"

struct Renderizador : Componente
{
	Modelo* modelo;
	static constexpr MascaraComponente mascara = COMPONENTE_RENDER;

	explicit Renderizador(Modelo* malha) : modelo(malha)
	{
	}
};