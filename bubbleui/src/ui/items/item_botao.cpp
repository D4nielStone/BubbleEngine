#include "item_botao.hpp"

BubbleUI::Items::Botao::Botao(std::string l, std::function<void()> funcao_click) : funcao_click_(std::move(funcao_click))
{
	ItemMenu::ItemMenu(l);
	label = new std::string(l);
	configurar();
}

void BubbleUI::Items::Botao::atualizar()
{
	ItemMenu::atualizar();
	if (clicado)
	{
		if (funcao_click_)
		{
			funcao_click_();
		}
	}
}