
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "item_botao.hpp"
#include "src/ui/widgets/imagem.hpp"

BubbleUI::Items::Botao::Botao(const std::string &l,bool* callback) : callback(callback)
{
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

BubbleUI::Items::Botao::Botao(const std::string& l,bool* callback, const std::string& imagePath) : callback(callback)
{
	icon = std::make_unique<Widgets::Imagem>(imagePath, Vector2{ 20, 20 });
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

void BubbleUI::Items::Botao::atualizar()
{
	if (icon) icon->atualizar();
	ItemMenu::atualizar();
	if(callback)
	*callback = false;
	if (clicado && !escondido)
	{
		if (callback)
		{
			*callback = true;
		}
	} 
}
void BubbleUI::Items::Botao::definirPai(Formas::Moldura* pai)
{
	Items::ItemMenu::definirPai(pai);
	if(icon)
	icon->definirPai(this);
}
void BubbleUI::Items::Botao::renderizar()
{
	ItemMenu::renderizar();
	if (icon) icon->renderizar();

}
