/** @copyright Copyright(c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_codigo.hpp
 * @brief Gerencia a api lua
 */

#pragma once
#include "sistema.hpp"

namespace bubble
{
	class sistemaCodigo : public sistema
	{
	public:
		sistemaCodigo() = default;
		void atualizar(double deltaTime) override;
		void inicializar(bubble::cena* cena) override;
	};
}