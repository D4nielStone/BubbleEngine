#pragma once
#include "bubbleui.hpp"
#include "src/ui/formas/rect.hpp"
#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"
#include <memory>

namespace BubbleUI
{
	class Painel;
	namespace Widgets 
	{
		class Arvore;
	};
	struct BEUI_DLL_API LetraRect
	{
		Vector4f rect;
		unsigned int ID;
	};
	class BEUI_DLL_API Widget
	{
	public:
		Widget() = default;
		virtual void atualizar() = 0;
		virtual void renderizar() const = 0;
		virtual void defPainel(Painel* painel);
		std::shared_ptr<Widgets::Arvore> arvore_pai{ nullptr };
		bool quebrarLinha{ false };
	protected:
		Colisao2d colisao;
		Formas::Moldura moldura;
		Painel* painel{ nullptr };
		std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
	};
}