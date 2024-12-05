#pragma once
#include "bubbleui.hpp"
#include "src/ui/formas/rect.hpp"
#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"
#include <memory>

namespace BubbleUI
{
	//Pre-declarações
	class Painel;
	namespace Widgets 
	{
		class Arvore;
	};
	enum class Alinhamento
	{
		Direita,
		Esquerda,
		Centro,
		Cima,
		Baixo
	};
	//Classe
	class BEUI_DLL_API Widget : public Formas::Moldura
	{
	public:
		Widget() = default;
		virtual void atualizar() = 0;
		virtual void renderizar() = 0;
		virtual void definirPai(Formas::Moldura* painel);
		void defAlinhamento(const Alinhamento& lado);
		std::shared_ptr<Widgets::Arvore> arvore_pai{ nullptr };
		bool quebrarLinha{ false };
	protected:
		std::shared_ptr<Colisao2d> colisao = std::make_shared<Colisao2d>();
		Formas::Moldura *painel{ nullptr };
		Alinhamento alinhamentoHorizontal = Alinhamento::Esquerda;
		std::shared_ptr<Contexto> contexto{ nullptr };
		std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
		Vector2f posicao_antiga{0,0};
	};
}