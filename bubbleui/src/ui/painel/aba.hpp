#pragma once
#include "src/ui/formas/rect.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Aba
	{
	public:
		Aba(Painel* painel);
		void atualizar(float deltaTime);
		void renderizar();
	private:
		void renderizar_texto();
		Painel* painel;
		Formas::Rect* corpo_do_texto, *corpo_rect;
	};
}