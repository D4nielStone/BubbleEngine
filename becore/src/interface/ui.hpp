#ifndef UI_HPP
#define UI_HPP
#include "vector"
#include "unordered_map"
#include "src/util/includes.h"
#include "src/arquivadores/imageloader.h"
#include "src/interface/layout/layout.hpp"
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
			Layout* janela_editor;
			
			void limpar();
			std::wstring desktopPath();
		public:
			UI(){};
			void pollevents();
			void inicializar(Nucleo::Gerenciador* gen, GLFWwindow* win);
			void renderizar();
			void contextoAtual(GLFWwindow* w, Estilo = Gerenciador);
			Layout* novaJanela(TipoLayout j);
			Inputs::Inputs inputs;
		};
	}
}
#endif