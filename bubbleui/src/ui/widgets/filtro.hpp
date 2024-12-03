
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include "widget.hpp"
#include "caixa_de_texto.hpp"
#include "arquivos.hpp"
#include "botao.hpp"

namespace BubbleUI 
{
	namespace Widgets
	{
		class BEUI_DLL_API Filtro : public Widget
		{
		public:
			Filtro(const std::string& label);
			void atualizar() override;
			void renderizar() override;
			void recarregar();
			void definirPai(Moldura* ctx) override;
		private:
			std::vector<std::unique_ptr<Botao>> botoes;
			void adiBotao(const std::string& label, std::function<void()> funcao);
			std::unique_ptr<CaixaTexto> text_box{ nullptr };
			std::string label;
			std::unique_ptr<Arquivos> projetos{ nullptr };
		};
	}
}