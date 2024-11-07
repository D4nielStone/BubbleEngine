#pragma once
#include "widget.hpp"
#include "caixa_de_texto.hpp"
#include "botao.hpp"
#include "texto.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Opcoes : public Widget
		{
		public:
			Opcoes();
			void atualizar() override;
			void renderizar() const override;
			void recarregar();
			void adiBotao(const std::string& label, const std::string& icone);
		private:
			std::vector<std::unique_ptr<Botao>> botoes;
			std::vector<std::pair<std::string,std::string>> diretorios;
			std::unique_ptr<CaixaTexto> text_box{ nullptr };
			Texto texto;
		};
	}
}