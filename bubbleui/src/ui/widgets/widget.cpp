#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

void BubbleUI::Widget::defPainel(Painel* painel)
{
	this->painel = painel;	
	moldura = Formas::Moldura(painel->obtCtx());
	colisao = Colisao2d({}, painel->obtCtx());
	inputs = inputs;
}