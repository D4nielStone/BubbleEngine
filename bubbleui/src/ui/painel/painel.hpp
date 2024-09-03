#pragma once
#include "src/ui/formas/moldura.hpp"
#include "src/ui/widgets/widget.hpp"
#include "src/ui/util/pop_up.hpp"
#include "separador.hpp"
#include "bubbleui.hpp"
#include "aba.hpp"
#include <memory>

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		Painel() = default;
		Painel(std::shared_ptr<Contexto> ctx, const Vector4& rect);
		void defTam(const Vector2 &tam);
		void defPos(const Vector2 &pos);
		void adiTam(const Vector2 &tam);
		void adiPos(const Vector2 &pos);
		void adiWidget(std::shared_ptr<Widget> widget);
		Vector4 obtRect() const;
		Vector2 obtTamMin() const;
		std::shared_ptr<Contexto> obtCtx() const;
		void atualizar();
		void renderizar() const;
		bool cursorNormal() const;
		std::string nome() const;
		bool selecionado {false}, arrastando{false}, mouse1click{false}, mostrar_popup{false}, esconder_popup{false};
		Lado redimen_atual;
		Vector2f widget_pos;
		Vector2 widget_padding;
		Color arvore_cor{ 0.1f, 0.1f, 0.1f };
	protected:
		void configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect = { 2, 2, 100, 50 });
		virtual void preAtualizacao();
		virtual void posAtualizacao();
		virtual void preRenderizacao() const;
		void corrigirLimite();
		bool renderizar_corpo{ true };

		std::string Nome = "Painel";
		Formas::Moldura moldura;
		std::unique_ptr<Util::PopUp> menu_de_contexto{ nullptr };
		std::shared_ptr<Contexto> contexto{nullptr};
		std::vector<std::shared_ptr<Widget>> lista_widgets;
		std::unique_ptr < Separador >borda_e, borda_d, borda_c, borda_b;
		std::unique_ptr<Aba>m_aba{ nullptr };
		Vector4 retangulo;
		Vector2 limite_min_tam;
	};
}