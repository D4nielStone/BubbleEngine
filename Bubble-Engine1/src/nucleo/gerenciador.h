#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes.h"
#include "src/nucleo/scenemanager.h"

namespace Bubble {
	namespace Nucleo {
		class Gerenciador
		{
		private:
		public:
			SceneManager gerenciadorDeCenas;
			GLFWwindow* glfwWindow;
			bool inicializacao();
			int pararloop();
			void renderizar();
			void limpar();
		};
	}
}
#endif

