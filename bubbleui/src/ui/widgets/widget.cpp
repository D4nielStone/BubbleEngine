#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

void BubbleUI::Widget::defPainel(Painel* p)
{
	pai = p;	
	corpo_do_widget = new Formas::Rect(pai->obtCtx());
}
