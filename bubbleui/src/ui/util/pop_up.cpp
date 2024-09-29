#include "pop_up.hpp"
#include "src/ui/items/item_menu.hpp"

BubbleUI::Util::PopUp::PopUp(std::shared_ptr<Contexto> contexto)
{
	Moldura::Moldura(contexto);
	inputs = contexto->inputs; 
	this->contexto = contexto;
	this->retangulo = retangulo;
	Rect::Rect(contexto, retangulo);
	colisao = Colisao2d({},contexto);
	linha_d = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_b = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_e = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_c = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_d->defCor({ 0.35, 0.35, 0.35 });
	linha_b->defCor({ 0.35, 0.35, 0.35 });
	linha_e->defCor({ 0.55, 0.55, 0.55 });
	linha_c->defCor({ 0.55, 0.55, 0.55 });
}

void BubbleUI::Util::PopUp::posAtualizacao()
{
	altura = 0;
	posicaoWidget = { 0, 0 };
	for (auto& item : lista_items)
	{
		item->atualizar();
			if (item->largura > largura)
				largura = item->largura;
			altura += item->altura;
	}
	colisao.defRect(retangulo);
	mouseEmCima = colisao.mouseEmCima();
	if (vmostrar && inputs->mouseEnter == GLFW_RELEASE)
		podeEsconder = true;
	if (vmostrar && inputs->mouseEnter == GLFW_PRESS && !mouseEmCima && podeEsconder)
	{
		esconder();
		podeEsconder = false;
	}
}

void BubbleUI::Util::PopUp::mostrar()
{
	if (!vmostrar)
	{
		this->retangulo = { static_cast<float>(inputs->mousex), static_cast<float>(inputs->mousey), largura, altura };
		vmostrar = true;
	}
}

void BubbleUI::Util::PopUp::esconder()
{
	vmostrar = false;
}

void BubbleUI::Util::PopUp::renderizar() const
{

	if (vmostrar)
	{
		Moldura::renderizar();
		for (auto& item : lista_items)
		{
			item->renderizar();
		}
	}
}

void BubbleUI::Util::PopUp::adiItem(std::shared_ptr<BubbleUI::Items::ItemMenu> item)
{
	item->defMoldura(this);
	lista_items.emplace_back(item);
}