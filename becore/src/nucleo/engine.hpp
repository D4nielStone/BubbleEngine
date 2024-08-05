#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "src/util/includes.h"
#include "src/inputs/inputs.h"
#include "src/nucleo/scenemanager.h"
#include "src/interface/uimanager.hpp"

struct GLFWwindow;
namespace Bubble
{
	namespace Nucleo
	{
		class BECORE_DLL_API Engine
		{
		private:
			Inputs::Inputs* inputs;
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
		public:
			void defInputs(Inputs::Inputs* inp);
			Inputs::Inputs* obterGI() { return inputs; };
			SceneManager* obterGC() { return &gerenciadorDeCenas; };
			GLFWwindow* obterJanela() { return glfwWindow; };
			bool inicializacao();
			int pararloop() const;
			void limpar() const;
			bool salvarCena(unsigned int idx);
			Engine();
			~Engine() { limpar(); };
			void atualizar();
			void renderizar();
		};
	}
}
#endif