#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include "becore.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "includes.h"
#include "src/inputs/gameinputs.h"
#include "src/nucleo/scenemanager.h"
#include "src/interface/ui.h"

namespace Bubble {
	namespace Nucleo {
		struct BECORE_DLL_API Projeto
		{
			std::string nome;
			std::string path;
		};
		class BECORE_DLL_API Engine
		{
		private:
			Projeto projeto;
			Inputs::GameInputs inputs;
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
		public:
			Projeto* obterProjeto() { return &projeto; };
			Inputs::GameInputs* obterGI() { return &inputs; };
			SceneManager* obterGC() { return &gerenciadorDeCenas; };
			GLFWwindow* obterJanela() { return glfwWindow; };
			bool inicializacao();
			int pararloop() const;
			void renderizar(Modo m, ImVec2 viewportPos, ImVec2 viewportSize);
			void limpar() const;
			bool salvarCena(unsigned int idx);
			Engine();
			~Engine() { limpar(); };
		};
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

