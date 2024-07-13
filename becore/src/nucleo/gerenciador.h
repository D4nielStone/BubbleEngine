#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include "becore.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "includes.h"
#include "src/inputs/gameinputs.h"
#include "src/nucleo/scenemanager.h"
#include "imgui.h"

namespace Bubble {
	namespace Nucleo {
		class BECORE_DLL_API Engine
		{
		public:
			Bubble::Inputs::GameInputs inputs;
			Engine();
			~Engine();
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
			bool inicializacao();
			int pararloop() const;
			void renderizar(Modo m, ImVec2 tamanhoJanela = ImVec2(0,0));
			void limpar() const;
			bool carregarProjeto(const std::string& path);
			bool criarProjeto(const std::string& path, const std::string& nome);
			std::shared_ptr<Bubble::Nucleo::Scene> criarProjetoPadrao();
		};
	}
}
#endif

