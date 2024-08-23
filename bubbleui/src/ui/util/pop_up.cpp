#include "pop_up.hpp"
#include "src/ui/items/item_menu.hpp"

BubbleUI::Util::PopUp::PopUp(Contexto* contexto)
{
	this->contexto = contexto;
	inputs = contexto->inputs;
	this->retangulo = {2, 2, 50, 50};
	linha_d = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_b = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_e = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_c = new Formas::Linha({ 0, 0, 0, 0 }, contexto);
	linha_d->defCor({ 0.35, 0.35, 0.35 });
	linha_b->defCor({ 0.35, 0.35, 0.35 });
	linha_e->defCor({ 0.55, 0.55, 0.55 });
	linha_c->defCor({ 0.55, 0.55, 0.55 });
}

void BubbleUI::Util::PopUp::preAtualizacao()
{
	for (auto& item : lista_items)
	{
		item->atualizar();
			if (item->largura > largura)
				largura = item->largura;
			if (item->altura > altura)
				altura = item->altura;
	}
}

void BubbleUI::Util::PopUp::mostrar()
{
	if (!vmostrar)
	{
		this->retangulo = { (float)inputs->mousex, (float)inputs->mousey, largura, altura };
		vmostrar = true;
	}
}

void BubbleUI::Util::PopUp::esconder()
{
	vmostrar = false;
}

void BubbleUI::Util::PopUp::renderizar(GLenum mode)
{

	if (vmostrar)
	{
		Moldura::renderizar(mode);
		widget_pos = { 0, 0 };
		for (auto& item : lista_items)
		{
			item->renderizar();
		}
	}
}

void BubbleUI::Util::PopUp::adiItem(BubbleUI::Items::ItemMenu* item)
{
	item->defMoldura(this);
	lista_items.push_back(item);
}
