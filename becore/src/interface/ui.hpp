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
	enum TipoLayout
	{
		L_MENU,
		L_JANELA
	};
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
			std::vector<Quadrado*>layouts;
			GLFWwindow* contexto;
			Layout* janela_editor;
			Imagem* pano_de_fundo;
			
			void limpar();
			std::wstring desktopPath();
		public:
			Vector2 viewportEditor;
			UI(){};
			void definirPanoDeFundo(std::string = "R.jfif");
			void pollevents();
			void inicializar(Nucleo::Gerenciador* = nullptr);
			void renderizar();
			void contextoAtual(GLFWwindow*, Estilo = Gerenciador);
			Quadrado* novaJanela(TipoLayout);
			Inputs::Inputs inputs;
		};
	}
}
#endif