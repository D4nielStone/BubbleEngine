#ifndef UI_H
#define UI_H
#include "vector"
#include "src/util/includes.h"
#include "src/arquivadores/imageloader.h"
#include "imgui.h"


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
			Console,
			MENU_Arquivos,
			MENU_CriarProjeto,
			MENU_Cena,
			MENU_Editar
		};
		enum BECORE_DLL_API Estilo
		{
			Motor,
			Vazio
		};
		class BECORE_DLL_API UI
		{
		private:
			ImFont* notosans;
			char nomeBuffer[64];
			char pathBuffer[1024];
			bool criarPadrao = true;

			Nucleo::Gerenciador* gerenciador;
			std::vector<std::pair<std::pair<GLFWwindow*, ImGuiContext*>,std::vector<Janela>*>> janelas;
			
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
			void iniciarJanelas(std::pair<std::pair<GLFWwindow*, ImGuiContext*>, std::vector<Janela>*> j);
			void desenharComponente(Comum::Componente* componente);
			std::wstring desktopPath();
		public:
			UI(){};
			void inicializarImGui(Nucleo::Gerenciador& gen);
			void renderizar();
			void limpar(std::pair<std::pair<GLFWwindow*, ImGuiContext*>, std::vector<Janela>*>j);
			void pullevents();
			void novoContexto(GLFWwindow* w, Estilo = Vazio);
			void novaJanela(GLFWwindow* w, Janela j);
			bool gameloop = true;
		};
	}
}
#endif