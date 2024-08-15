#pragma once
#include "src/ui/formas/rect.hpp"
#include "src/ui/widgets/widget.hpp"
#include "borda.hpp"
#include "bubbleui.hpp"
#include "aba.hpp"

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
		std::string nome() const;
		Lado redimen_atual;
		bool selecionado = false, arrastando, mouse1click;
		Vector2 widget_pos, widget_padding;
	protected:
		std::string Nome = "Painel";
		bool renderizar_corpo = true;
		Contexto* contexto;
		Borda* borda_e, *borda_d, *borda_c, *borda_b;
		Aba* m_aba;
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