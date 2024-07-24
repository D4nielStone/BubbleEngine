#ifndef LAYOUT_HPP
#define LAYOUT_HPP
#include "src/interface/base/quadrado.hpp"
#include "src/interface/imagem/imagem.h"

class GLFWcursor;
namespace Bubble
{
	namespace Interface
	{
		class BECORE_DLL_API Layout : public Quadrado
		{
		public:
			Layout();
			void adicImagem(Imagem& img);
			void renderizar() override;
			void atualizar() override;
		private:
			GLFWcursor* cursorMao;
			Quadrado abajanela;
			std::vector<Imagem> imagems;
			std::string label;
		};
	}
}
#endif