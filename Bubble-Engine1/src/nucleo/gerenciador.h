#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes.h"
#include "src/entidades/objeto_geral.h"

namespace Bubble {
	namespace Nucleo {
		class Gerenciador
		{
		private:
		public:
			GLFWwindow* glfwWindow;
			unsigned int shaderProgram, VAO;
			bool inicializacao();
			int pararloop();
			//void atualizar();
			void renderizar(Bubble::Entidades::ObjetoGeral* go);
			void limpar();
		};
	}
}
#endif

