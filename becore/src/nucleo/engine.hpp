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
			std::shared_ptr<Inputs::Inputs> inputs{ nullptr };
			std::shared_ptr < Cena::SceneManager> gerenciadorDeCenas{ nullptr };
			GLFWwindow* glfwWindow{ nullptr };
			std::shared_ptr<BubbleUI::Manager> gerenciadorUi{ nullptr };
		public:
			std::shared_ptr<Inputs::Inputs> obterGI() const;
			std::shared_ptr < BubbleUI::Manager> obterGU() const;
			std::shared_ptr < Cena::SceneManager> obterGC() const { return gerenciadorDeCenas; };
			GLFWwindow* obterJanela() const { return glfwWindow; };
			void defInputs(std::shared_ptr<Inputs::Inputs> inp);
			bool inicializacao();
			int pararloop() const;
			void limpar() const;
			bool salvarCena(unsigned int idx);
			void atualizar();
			void renderizar() const;
			Engine();
			~Engine() { limpar(); };
		};
	}
}