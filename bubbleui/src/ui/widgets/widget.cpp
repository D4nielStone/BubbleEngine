
// Copyright (c) 2024 Daniel Oliveira

#include "widget.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

void BubbleUI::Widget::definirPai(Formas::Moldura* p)
{
	painel = p;	
	contexto = painel->obterContexto();
	Formas::Moldura::configurar(contexto, {});
	colisao = Colisao2d({}, contexto);
	inputs = contexto->inputs;
}

void BubbleUI::Widget::defAlinhamento(const Alinhamento& lado)
{
	alinhamentoHorizontal = lado;

}
