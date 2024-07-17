#ifndef UI_H
#define UI_H
#include "vector"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "src/arquivadores/imageloader.h"

class GLFWwindow;
namespace Bubble
{
	namespace Nucleo 
	{
		class Engine;
		class Gerenciador;
	}
	namespace Comum
	{
		class Componente;
	}
	namespace Interface
	{
		enum BECORE_DLL_API Janela
		{
			Editor,
			Projetos,
			Preview,
			Entidades,
			Arquivos,
			Inpetor,
			Console
		};
		class BECORE_DLL_API UI
		{
		private:
			std::string nome;
			std::string path;
			bool criarPadrao = true;

			std::vector<Janela>janelas;
			Nucleo::Gerenciador* gerenciador;
			std::vector<GLFWwindow*> janelasGLFW;
			std::vector<ImGuiContext*> contextosImGui;
			
			void desenharProjetos(unsigned int depth);
			void desenharEditor(unsigned int depth);
			void desenharPreview(unsigned int depth);
			void desenharConsole(unsigned int depth);
			void desenharInspetor(unsigned int depth);
			void desenharArquivos(unsigned int depth);
			void desenharEntidades(unsigned int depth);
			void iniciarJanelas();
			void desenharComponente(Comum::Componente* componente);
			const char* desktopPath();
			void menuBar();
		public:
			UI();
			~UI();
			void inicializarImGui(Nucleo::Gerenciador& gen);
			void renderizar();
			void limpar();
			void novoContexto(GLFWwindow* w);
			void novaJanela(Janela j);
		};
	}
}
#endif