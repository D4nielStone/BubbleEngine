#pragma once
#include "src/inputs/inputs.hpp"
#include "src/cena/scenemanager.hpp"
#include "src/ui/manager.hpp"
#include "src/util/includes.hpp"
#include "becore.hpp"
class GLFWwindow;
namespace Bubble
{
	namespace Nucleo
	{
		class BECORE_DLL_API Engine
		{
		private:
			Inputs::Inputs* inputs;
			Cena::SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
			BubbleUI::Manager* gerenciadorUi = nullptr;
		public:
			Inputs::Inputs* obterGI() const;
			Cena::SceneManager* obterGC() { return &gerenciadorDeCenas; };
			GLFWwindow* obterJanela() { return glfwWindow; };
			void defInputs(Inputs::Inputs* inp);
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