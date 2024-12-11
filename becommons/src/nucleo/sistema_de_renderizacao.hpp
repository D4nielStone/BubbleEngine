/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_renderizacao.hpp
 * @brief Gerencia a renderizacao
 */

#pragma once
#include "sistema.hpp"

class SistemaDeRenderizacao : public Sistema
{
public:
	explicit SistemaDeRenderizacao(GerenciadorDeEntidades* ge) { this->ge = ge; };
	void atualizar(double deltaTime) override;
	void inicializar() override;	
};