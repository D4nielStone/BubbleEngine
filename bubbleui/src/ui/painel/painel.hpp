#pragma once
#include "src/ui/formas/rect.hpp"
#include "src/ui/widgets/widget.hpp"
#include "borda.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		Painel() = default;
		Painel(Contexto* ctx);
		Painel(Vector4 rext, Contexto* ctx);
		void defTam(Vector2 tam);
		void defPos(Vector2 pos);
		void adiTam(Vector2 tam);
		void adiPos(Vector2 tam);
		void adiWidget(Widget* widget);
		Vector4 obtRect() const;
		Vector2 obtTamMin() const;
		Contexto* obtCtx() const;
		void atualizar(float deltaTime);
		void renderizar();
		bool cursorNormal();
		Lado redimen_atual;
		bool selecionado = false, arrastando, mouse1click;
		Vector2 widget_pos;
	protected:
		Contexto* contexto;
		Borda* borda_e = nullptr;
		Borda* borda_d = nullptr;
		Borda* borda_c = nullptr;
		Borda* borda_b = nullptr;
		void configurar(Contexto* ctx, Vector4 rect = { 2, 2, 50, 50 });
		virtual void preAtualizacao();
		virtual void preRenderizacao();
		std::vector<Widget*> lista_widgets;
		void corrigirLimite();
		Vector4 retangulo;
		Vector2 limite_min_tam;
		Formas::Rect* corpo_rect;
	};
}