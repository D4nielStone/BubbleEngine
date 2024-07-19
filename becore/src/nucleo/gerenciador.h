#ifndef GERENCIADOR_H
#define GERENCIADOR_H
#include "src/nucleo/engine.hpp"
#include "becore.h"

class GLFWindow;
namespace Bubble {
	namespace Nucleo
	{
		class BECORE_DLL_API Gerenciador
		{
		private:
			Interface::UI ui;
			GLFWwindow* janelaGerenciador;
			std::vector<Engine*> engines;
			Scene criarCenaPadrao();
			bool escanearProjetos();
		public:
			Engine* engineAtual = nullptr;
			GLFWwindow* obterJanela() { return janelaGerenciador; };
			std::vector<Engine*>* obterEngines() { return &engines; };
			std::vector<Projeto>* obterProjetos();
			void limpar();
			bool inicializacao();
			bool carregarProjeto(Projeto proj);
			bool criarProjeto(const std::string& path, const std::string& nome, bool criarCenaPadrao = true);
			bool salvarListaProjetos();
			Gerenciador() {};
			~Gerenciador() { limpar(); };
		};
	}
}
#endif

