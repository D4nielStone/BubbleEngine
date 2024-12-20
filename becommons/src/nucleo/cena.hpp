/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file cena.hpp
 * @brief Gerencia sistemas numa cena
 */

#include "src/entidades/entidade.hpp"
#include "src/componentes/camera.hpp"
#include "src/nucleo/sistema_de_renderizacao.hpp"
#include "src/nucleo/sistema_de_interface.hpp"
/**
 * @class cena
 */

namespace bubble
{
	class cena
	{
	public:
		cena();
		void pausar();
		void parar();
		void iniciar();
		void atualizar(double deltaTime);
		void definirCamera(const entidade &ent);
		std::shared_ptr<camera> obterCamera() const;
		registro* obterRegistro();
	private:
		registro reg;
		std::shared_ptr<camera> camera_atual{ nullptr };
		bool rodando = false;
		sistemaRenderizacao srender;
		sistemaInterface sinterface;
	};
}