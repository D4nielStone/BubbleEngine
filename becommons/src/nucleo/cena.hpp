/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file cena.hpp
 * @brief Gerencia sistemas numa cena
 */

#include "src/entidades/entidade.hpp"
#include "src/componentes/camera.hpp"
#include "sistema_de_renderizacao.hpp"
#include "sistema_de_interface.hpp"
#include "sistema_de_codigo.hpp"
#include <memory>

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
		sistemaCodigo scodigo;
	};
}

inline bubble::cena* cena_atual{ nullptr };