#include "item_arvore.hpp"
#include "src/ui/util/pop_up.hpp"

BubbleUI::Items::Arvore::Arvore(const std::string &l, std::shared_ptr<Util::PopUp> popup) : m_popup(popup)
{
	ItemMenu::ItemMenu(l);
	resolucao = (12);
	frase = (l);
	letra_padding = { 4, 4 };
}

void BubbleUI::Items::Arvore::atualizar()
{
	ItemMenu::atualizar();
	m_popup->defPos({ static_cast<int>(box_pos.x + largura - 10), static_cast<int>(box_pos.y) });
	m_popup->atualizar();
	if (mouseEmCima)	m_popup->mostrar();
	else if(!m_popup->mouseEmCima)	m_popup->esconder();

}

void BubbleUI::Items::Arvore::renderizar() const
{
	ItemMenu::renderizar();
	m_popup->renderizar();
}