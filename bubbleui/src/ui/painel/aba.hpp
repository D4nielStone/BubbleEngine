#pragma once
#include "src/ui/widgets/texto.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Aba : public Widgets::Texto
	{
	public:
		Aba(Painel* painel);
		Aba() = default;
		void atualizar(float deltaTime) override;
		void renderizar() override;
	private:
		Painel* painel;
		Formas::Rect *corpo_rect;
	};
}