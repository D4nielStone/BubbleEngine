/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file cena.hpp
 * @brief Gerencia sistemas numa cena
 */

#include "src/entidades/entidade.hpp"
#include "src/nucleo/sistema_de_renderizacao.hpp"
/**
 * @class cena
 */

namespace bubble
{
	enum class estadoDeJogo : bool
	{
		PARADO  = true,
		RODANDO = false
	};

	class cena
	{
	public:
		cena();
		void pausar();
		void parar();
		void iniciar();
		void atualizar(double deltaTime);
		registro* obterRegistro();
	private:
		registro reg;
		estadoDeJogo estado = estadoDeJogo::PARADO;
		sistemaRenderizacao srender;
	};
}