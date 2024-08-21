#pragma once
#include "bubbleui.hpp"
#include "src/ui/formas/rect.hpp"
#include "src/ui/formas/moldura.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Widget
	{
	public:
		Widget() = default;
		virtual void atualizar(float deltaTime) = 0;
		virtual void renderizar() = 0;
		virtual void defPainel(Painel*);
	protected:
		Formas::Moldura moldura;
		Vector2f posicao_antiga;
		Painel* pai;
	};
}