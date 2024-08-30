#pragma once
#include "bubbleui.hpp"
#include "src/ui/formas/rect.hpp"
#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Widget
	{
	public:
		Widget() = default;
		virtual void atualizar() = 0;
		virtual void renderizar() = 0;
		virtual void defPainel(Painel*);
	protected:
		Colisao2d* colisao{ nullptr };
		Formas::Moldura moldura;
		Painel* pai{ nullptr };
	};
}