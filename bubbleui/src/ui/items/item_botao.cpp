// Copyright (c) 2024 Daniel Oliveira
// Licenciado sob a licença MIT. Consulte o arquivo LICENSE para mais informações.
#include "item_botao.hpp"

BubbleUI::Items::Botao::Botao(const std::string &l,bool* callback) : callback(callback)
{
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

BubbleUI::Items::Botao::Botao(const std::string& l,bool* callback, const std::string& imagePath) : callback(callback)
{
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

void BubbleUI::Items::Botao::atualizar()
{
	ItemMenu::atualizar();
	if(callback)
	*callback = false;
	if (clicado && gatilho && !escondido)
	{
		if (callback)
		{
			*callback = true;
		}
		gatilho = false;
	}
}
void BubbleUI::Items::Botao::renderizar() const
{
	ItemMenu::renderizar();
}
