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
			GLFWwindow* janelaGerenciador;
			Scene criarCenaPadrao();
			bool escanearProjetos();
			bool salvarListaProjetos();
		public:
			Interface::UI ui;
			Engine* engineAtual = nullptr;
			GLFWwindow* obterJanela() { return janelaGerenciador; };
			std::vector<Projeto>* obterProjetos();
			void limpar();
			bool inicializacao();
			bool carregarProjeto(Projeto proj);
			bool criarProjeto(const std::string& path, const std::string& nome, bool criarCenaPadrao = true);
			void renderizar();
			int pararLoop();
			Gerenciador() {};
		};
	}
}
#endif

