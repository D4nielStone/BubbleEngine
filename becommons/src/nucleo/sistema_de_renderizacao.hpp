/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_renderizacao.hpp
 * @brief Gerencia a renderizacao
 */

#pragma once
#include "sistema.hpp"

namespace bubble
{
	class sistemaRenderizacao : public sistema
	{
	public:
		explicit sistemaRenderizacao(registro* reg) { this->reg = reg; };
		void atualizar(double deltaTime) override;
		void inicializar() override;
	};
}