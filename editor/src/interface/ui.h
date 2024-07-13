#ifndef UI_H
#define UI_H

#include "vector"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "src/nucleo/gerenciador.h"

class GLFWwindow;
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
			std::vector<Janela>janelas;
			Nucleo::Engine *engine;
		public:
			UI();
			void configurar(Nucleo::Engine * eng);
			void novoFrame();
			void renderizar();
			void limpar();
			void novaJanela(Janela j);
		};
	}
}

#endif