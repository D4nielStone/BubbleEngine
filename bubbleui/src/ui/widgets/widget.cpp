#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

void BubbleUI::Widget::defPainel(Painel* painel)
{
	this->painel = painel;	
	moldura = Formas::Moldura(painel->obterContexto());
	colisao = Colisao2d({}, painel->obterContexto());
	inputs = painel->obterContexto()->inputs;
}