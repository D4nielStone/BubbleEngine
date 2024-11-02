#pragma once
#include "widget.hpp"
#include "caixa_de_texto.hpp"
#include "botao.hpp"

namespace BubbleUI 
{
	namespace Widgets
	{
		class BEUI_DLL_API Filtro : public Widget
		{
		public:
			Filtro(const std::string &label);
			void atualizar() override;
			void renderizar() const override;
			void recarregar();
		private:
			std::vector<std::unique_ptr<Botao>> botoes;
			void adiBotao(const std::string &label);
			std::unique_ptr<CaixaTexto> text_box{ nullptr };
			std::string label;
		};
	}
}