#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "src/util/includes.h"
#include "src/inputs/gameinputs.h"
#include "src/nucleo/scenemanager.h"
#include "src/interface/ui.hpp"

class Gerenciador;
class GLFWwindow;
namespace Bubble
{
	namespace Nucleo
	{
		class BECORE_DLL_API Engine
		{
		private:
			Projeto projeto;
			Inputs::Inputs* inputs;
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
		public:
			Projeto* obterProjeto() { return &projeto; };
			Inputs::Inputs* obterGI() { return inputs; };
			SceneManager* obterGC() { return &gerenciadorDeCenas; };
			GLFWwindow* obterJanela() { return glfwWindow; };
			bool inicializacao(Gerenciador* w);
			int pararloop() const;
			void renderizar(Modo m, Vector2 viewportSize);
			void limpar() const;
			bool salvarCena(unsigned int idx);
			Engine();
			~Engine() { limpar(); };
		};
	}
}
#endif