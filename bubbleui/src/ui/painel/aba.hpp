#pragma once
#include "src/ui/widgets/texto.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Aba : public Widgets::Texto
	{
	public:
		Aba() = default;
		Aba(Formas::Moldura* painel);
		void atualizar() override;
		void renderizar() const override;
		Formas::Rect* obterCorpo() const;
	private:
		bool painel_flag{ true };
		std::vector<LetraRect> letras_rect;
		std::unique_ptr<Formas::Rect>corpo_rect;
	};
}