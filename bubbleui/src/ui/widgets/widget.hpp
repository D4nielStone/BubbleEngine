#pragma once
#include "bubbleui.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Widget
	{
	public:
		Widget() = default;
		virtual void atualizar(float deltaTime) = 0;
		virtual void renderizar() = 0;
		void defPainel(Painel*);
	protected:
		Painel* pai;
	};
}