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
		class BECORE_DLL_API Projeto
		{
		private:
			std::string Nome = "Projeto sem nome", NomeGPU, VercaoGL;
			std::shared_ptr<Inputs::Inputs> inputs{ nullptr };
			std::shared_ptr < Cena::SceneManager> gerenciadorDeCenas{ nullptr };
			GLFWwindow* glfwWindow{ nullptr };
			std::shared_ptr<BubbleUI::Manager> gerenciadorUi{ nullptr };
		public:
			std::shared_ptr<Inputs::Inputs> obterGI() const;
			std::shared_ptr < BubbleUI::Manager> obterGU() const;
			std::shared_ptr < Cena::SceneManager> obterGC() const { return gerenciadorDeCenas; };
			std::string obterNome() const { return Nome; };
			std::string obterNomeGPU() const { return NomeGPU; };
			std::string obterVercaoGL() const { return VercaoGL; };
			GLFWwindow* obterJanela() const { return glfwWindow; };

			void defInputs(std::shared_ptr<Inputs::Inputs> inp);
			bool inicializacao();
			int pararloop() const;
			void limpar() const;
			bool salvarCena(unsigned int idx);
			void atualizar();
			void renderizar() const;
			Projeto(const std::string &nome);
			~Projeto() { limpar(); };
		};
	}
}