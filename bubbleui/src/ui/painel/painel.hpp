#pragma once
#include "src/ui/formas/rect.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		Painel() = default;
		Painel(Contexto* ctx);
		void atualizar(float deltaTime);
		void renderizar();
	private:
		Formas::Rect* corpo_rect;
	};
}