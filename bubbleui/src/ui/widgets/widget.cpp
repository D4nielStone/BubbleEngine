#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

void BubbleUI::Widget::defPainel(Painel* p)
{
	pai = p;	
	moldura = Formas::Moldura(pai->obtCtx());
	colisao = new Colisao2d({}, pai->obtCtx());
}
