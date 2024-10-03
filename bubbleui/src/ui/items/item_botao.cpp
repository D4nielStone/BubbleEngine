#include "item_botao.hpp"

BubbleUI::Items::Botao::Botao(const std::string &l, std::function<void()> funcao_click) : funcao_click_(std::move(funcao_click))
{
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

BubbleUI::Items::Botao::Botao(const std::string& l, std::function<void()> funcao_click, const std::string& imagePath)
{
	resolucao = (12);
	label_shared = new std::string(l);
	texto = ("");
	letra_padding = { 4, 4 };
}

void BubbleUI::Items::Botao::atualizar()
{
	ItemMenu::atualizar();
	if (clicado && gatilho && !escondido)
	{
		if (funcao_click_)
		{
			funcao_click_();
			gatilho = false;
		}
	}
}
void BubbleUI::Items::Botao::renderizar() const
{
	ItemMenu::renderizar();
}