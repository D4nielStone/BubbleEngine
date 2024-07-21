#ifndef UI_HPP
#define UI_HPP
#include "vector"
#include "unordered_map"
#include "src/util/includes.h"
#include "src/arquivadores/imageloader.h"
#include "src/inputs/inputs.h"

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
		class Layout;
		enum BECORE_DLL_API Estilo
		{
			Motor,
			Gerenciador
		};
		class BECORE_DLL_API UI
		{
		private:
			//ImFont* notosans;
			char nomeBuffer[64];
			char pathBuffer[1024];
			bool criarPadrao = true;

			Nucleo::Gerenciador* gerenciador;
			std::vector<Layout*>layouts;
			GLFWwindow* contexto;
			
			void desenharMenuArquivos();
			void desenharMenuCena();
			void desenharMenuEditar();
			void desenharMenuCriarP();
			void desenharProjetos(unsigned int depth);
			void desenharEditor(unsigned int depth);
			void desenharPreview(unsigned int depth);
			void desenharConsole(unsigned int depth);
			void desenharInspetor(unsigned int depth);
			void desenharArquivos(unsigned int depth);
			void desenharEntidades(unsigned int depth);
			void iniciarJanelas();
			void desenharComponente(Comum::Componente* componente);
			void limpar();
			std::wstring desktopPath();
		public:
			UI(){};
			void pollevents();
			void inicializarImGui(Nucleo::Gerenciador* gen, GLFWwindow* win);
			void renderizar();
			void contextoAtual(GLFWwindow* w, Estilo = Gerenciador);
			void novaJanela(Janela j);
			Inputs::Inputs inputs;
		};
	}
}
#endif