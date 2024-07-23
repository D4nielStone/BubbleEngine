#ifndef LAYOUT_HPP
#define LAYOUT_HPP
#include "src/interface/base/quadrado.hpp"
#include "src/interface/imagem/imagem.h"

class GLFWcursor;
namespace Bubble
{
	namespace Interface
	{
		enum TipoLayout
		{
			L_MENU,
			L_JANELA
		};
		class BECORE_DLL_API Layout : public Quadrado
		{
		public:
			Layout(TipoLayout j);
			void adicImagem(Imagem& img);
			void renderizar() override;
			void atualizar() override;
		private:
			GLFWcursor* cursorMao;
			Quadrado abajanela;
			std::vector<Imagem> imagems;
			std::string label;
			TipoLayout tipo_layout;
		};
	}
}
#endif