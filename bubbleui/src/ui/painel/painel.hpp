#pragma once
#include "src/ui/formas/rect.hpp"
#include "borda.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		Painel() = default;
		Painel(Contexto* ctx);
		void defTam(Vector2 tam);
		void defPos(Vector2 pos);
		Vector4f obtRect() const;
		Contexto* obtCtx() const;
		void atualizar(float deltaTime);
		void renderizar();
	private:
		Contexto* contexto;
		Vector4f retangulo;
		Formas::Rect* corpo_rect;
		Borda* borda_e = nullptr;
		Borda* borda_d = nullptr;
		Borda* borda_c = nullptr;
		Borda* borda_b = nullptr;
	};
}