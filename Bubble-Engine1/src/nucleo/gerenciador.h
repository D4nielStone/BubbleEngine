#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes.h"
#include "src/nucleo/scenemanager.h"
#include "becore.h"

namespace Bubble {
	namespace Nucleo {
		class BECORE_DLL_API Gerenciador
		{
		public:
			Gerenciador();
			~Gerenciador();
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
			bool inicializacao();
			int pararloop();
			void renderizar(Modo m);
			void limpar();
			bool carregarProjeto(const std::string& path);
			bool criarProjeto(const std::string& path, const std::string& nome);
			std::shared_ptr<Bubble::Nucleo::Scene> criarProjetoPadrao();
		};
	}
}
#endif

