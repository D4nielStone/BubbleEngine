#pragma once
#include "src/inputs/inputs.hpp"
#include "src/cena/scenemanager.hpp"
#include "src/ui/manager.hpp"
#include "src/util/includes.hpp"
#include "becore.hpp"
#include "glfw/glfw3.h"

namespace Bubble
{
	namespace Nucleo
	{
		class BECORE_DLL_API Engine
		{
		private:
			Inputs::Inputs* inputs{ nullptr };
			Cena::SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow{ nullptr };
			BubbleUI::Manager* gerenciadorUi{ nullptr };
		public:
			Inputs::Inputs* obterGI() const;
			BubbleUI::Manager* obterGU() const;
			Cena::SceneManager* obterGC() { return &gerenciadorDeCenas; };
			GLFWwindow* obterJanela() const { return glfwWindow; };
			void defInputs(Inputs::Inputs* inp);
			bool inicializacao();
			int pararloop() const;
			void limpar() const;
			bool salvarCena(unsigned int idx);
			void atualizar();
			void renderizar(Vector4 rect_size = {0, 0, 640, 480});
			Engine();
			~Engine() { limpar(); };
		};
	}
}