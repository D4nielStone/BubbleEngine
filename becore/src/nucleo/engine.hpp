#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "src/util/includes.h"
#include "src/inputs/gameinputs.h"
#include "src/nucleo/scenemanager.h"
#include "src/interface/ui.h"

struct ImVec2;
class GLFWwindow;
namespace Bubble
{
	namespace Nucleo
	{
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
	}
}
#endif