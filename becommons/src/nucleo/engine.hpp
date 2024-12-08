/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <string>
#include "src/cena/scenemanager.hpp"
#include <memory>

namespace Bubble
{
	class Engine
	{
	private:
		std::shared_ptr<Cena::SceneManager> gerenciador_dc{ nullptr };
		static std::shared_ptr<Engine> instance;
	public:
		std::string diretorio_projeto;
		Engine();
		Engine(const std::string& path);
		void definirInputs(std::shared_ptr<Inputs::Inputs> inputs);
		std::shared_ptr<Cena::SceneManager> obterGerenciadorDeCenas();
		bool carregarProjeto(const std::string& path);
		void salvarProjeto();
		void atualizar();
		static std::shared_ptr<Engine> getInstance()
		{
			if (!instance)
				instance = std::make_shared<Engine>();
			return instance;
		}
		void renderizar();
	};
}