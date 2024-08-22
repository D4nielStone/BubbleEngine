#include "pop_up.hpp"

BubbleUI::Util::PopUp::PopUp(Contexto* contexto)
{
	this->contexto = contexto;
	inputs = contexto->inputs;
	this->retangulo = {2, 2, 50, 50};
	linha_d = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_b = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_e = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_c = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
}

void BubbleUI::Util::PopUp::preAtualizacao()
{
}

void BubbleUI::Util::PopUp::mostrar()
{
	this->retangulo = { (float)inputs->mousex, (float)inputs->mousey, 50, 50 };
	vmostrar = true;
}

void BubbleUI::Util::PopUp::esconder()
{
	vmostrar = false;
}

void BubbleUI::Util::PopUp::renderizar(GLenum mode)
{
	if (vmostrar)
		Moldura::renderizar(mode);
}
