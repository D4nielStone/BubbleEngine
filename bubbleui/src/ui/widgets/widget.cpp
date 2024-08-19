#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

void BubbleUI::Widget::defPainel(Painel* p)
{
	pai = p;	
	corpo_do_widget = Formas::Rect(pai->obtCtx());
	moldura = Formas::Moldura(pai->obtCtx());
}
