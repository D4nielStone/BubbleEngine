#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes.h"
#include "src/entidades/entidade.h"

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
			void atualizar(Bubble::Entidades::Entidade* go);
			void renderizar(Bubble::Entidades::Entidade* go);
			void limpar();
		};
	}
}
#endif

