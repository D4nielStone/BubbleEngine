#ifndef UI_H
#define UI_H
#include "GLFW/glfw3.h"
#include "vector"
#include "imgui.h"

namespace Bubble 
{
	namespace Interface
	{
		enum class Janela
		{
			Editor,
			Preview,
			Entidades,
			Arquivos,
			Inpetor
		};
		class UI
		{
		private:
			std::vector<std::pair<Janela, GLuint>>janelas;
		public:
			ImVec2 window_size;
			UI();
			void configurar(GLFWwindow* window);
			void novoFrame();
			void renderizar();
			void limpar();
			void novaJanela(Janela j, GLuint framebuffe = 0);
		};
	}
}

#endif