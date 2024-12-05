
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "pop_up.hpp"
#include "src/ui/items/item_menu.hpp"

BubbleUI::Util::PopUp::PopUp(std::shared_ptr<Contexto> contexto)
{
	configurar(contexto, {});
	colisao = Colisao2d({}, contexto);
	inputs = contexto->inputs;
}

void BubbleUI::Util::PopUp::posAtualizacao()
{
	altura = 0;
	posicaoWidget = { 0, 0 };
	for (auto& item : lista_items)
	{
		item->definirEscondido(!vmostrar);
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
		this->retangulo = { static_cast<int>(inputs->mousex), static_cast<int>(inputs->mousey), largura, altura };
		vmostrar = true;
	}
}

void BubbleUI::Util::PopUp::esconder()
{
	vmostrar = false;
}

void BubbleUI::Util::PopUp::renderizar()
{
	if (escondido()) return; 
	Moldura::renderizar();
	for (auto& item : lista_items)
	{
		item->renderizar();
	}
}

void BubbleUI::Util::PopUp::adiItem(std::shared_ptr<BubbleUI::Items::ItemMenu> item)
{
	item->definirPai(this);
	lista_items.emplace_back(item);
}

bool BubbleUI::Util::PopUp::temItems() const
{
	return (lista_items.size() != 0);
}

bool BubbleUI::Util::PopUp::escondido() const
{
	return !vmostrar;
}
