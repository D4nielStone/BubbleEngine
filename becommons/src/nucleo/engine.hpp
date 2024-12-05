/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <string>
#include "src/cena/scenemanager.hpp"

namespace Bubble
{
	class Engine
	{
	private:
		std::shared_ptr<Cena::SceneManager> gerenciador_dc{ nullptr };
	public:
		Engine();
		void definirInputs(std::shared_ptr<Inputs::Inputs> inputs);
		std::shared_ptr<Cena::SceneManager> obterGerenciadorDeCenas();
		bool carregarProjeto(const std::string& path);
		void atualizar();
		void renderizar();
	};
}