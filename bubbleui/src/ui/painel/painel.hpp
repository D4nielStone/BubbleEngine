#pragma once
#include "src/ui/formas/moldura.hpp"
#include "src/ui/widgets/widget.hpp"
#include "separador.hpp"
#include "bubbleui.hpp"
#include "aba.hpp"
#include "src/ui/util/pop_up.hpp"

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		Painel() = default;
		Painel(Contexto* ctx);
		Painel(Vector4 rext, Contexto* ctx);
		void defTam(Vector2 tam);
		void defPos(Vector2f pos);
		void adiTam(Vector2 tam);
		void adiPos(Vector2f pos);
		void adiWidget(std::shared_ptr<Widget> widget);
		Vector4 obtRect() const;
		Vector2 obtTamMin() const;
		Contexto* obtCtx() const;
		void atualizar();
		void renderizar();
		bool cursorNormal();
		std::string nome() const;
		bool selecionado {false}, arrastando{false}, mouse1click{false}, mostrar_popup{false}, esconder_popup{false};
		Lado redimen_atual;
		Vector2f widget_pos;
		Vector2 widget_padding;
		Color arvore_cor{ 0.1f, 0.1f, 0.1f };
	protected:
		void configurar(Contexto* ctx, Vector4 rect = { 2, 2, 100, 50 });
		virtual void preAtualizacao();
		virtual void preRenderizacao();
		void corrigirLimite();

		std::string Nome = "Painel";
		Formas::Moldura* moldura{ nullptr };
		Util::PopUp* menu_de_contexto{ nullptr };
		bool renderizar_corpo{ true };
		Contexto* contexto{nullptr};
		Separador* borda_e{ nullptr },
			* borda_d{ nullptr },
			* borda_c{ nullptr },
			* borda_b{ nullptr };
		Aba* m_aba{ nullptr };
		std::vector<std::shared_ptr<Widget>> lista_widgets;
		Vector4 retangulo;
		Vector2 limite_min_tam;
	};
}