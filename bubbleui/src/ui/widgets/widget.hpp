#pragma once
#include "bubbleui.hpp"
#include "src/ui/formas/rect.hpp"

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
		Formas::Rect* corpo_do_widget = nullptr;
		Painel* pai;
	};
}