/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file fase.hpp
 * @brief Gerencia sistemas numa fase
 */

#include "src/entidades/entidade.hpp"
#include "src/componentes/camera.hpp"
#include "sistema_de_renderizacao.hpp"
#include "sistema_de_fisica.hpp"
#include "sistema_de_interface.hpp"
#include "sistema_de_codigo.hpp"
#include <string>
#include <memory>

/**
 * @class fase
 */

namespace bubble
{
	class fase
	{
	public:
		fase(const char* diretorio);
		fase();
		std::string nome() const;
		void pausar();
		void parar();
		void iniciar();
		void atualizar(double deltaTime);
		void definirCamera(const entidade &ent);
		void analizar(const char* diretorio);
		std::shared_ptr<camera> obterCamera() const;
		registro* obterRegistro();
		sistemaFisica sfisica;
	private:
		const char* diretorio;
		registro reg;
		std::shared_ptr<camera> camera_atual{ nullptr };
		bool rodando = false;
		std::string _Mnome;
		sistemaRenderizacao srender;
		sistemaInterface sinterface;
		sistemaCodigo scodigo;
	};
}

inline bubble::fase* fase_atual{ nullptr };