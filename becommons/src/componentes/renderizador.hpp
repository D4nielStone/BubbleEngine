/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/arquivadores/modelo.hpp"

namespace bubble
{
	struct renderizador : componente
	{
		modelo* modelo;
		static constexpr mascara mascara = COMPONENTE_RENDER;

		renderizador(bubble::modelo* malha) : modelo(malha)
		{
		}
		renderizador(const char* diretorio = "") {};
	};
}