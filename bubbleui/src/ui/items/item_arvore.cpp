#include "item_arvore.hpp"
#include "src/ui/util/pop_up.hpp"

BubbleUI::Items::Arvore::Arvore(std::string l, std::shared_ptr<Util::PopUp> popup) : m_popup(popup)
{
	ItemMenu::ItemMenu(l + " -");
	label = new std::string(l + " -");
	configurar();
}

void BubbleUI::Items::Arvore::atualizar()
{
	ItemMenu::atualizar();
	m_popup->defPos({ box_pos.x + box_size.x - 10, box_pos.y });
	if (!mouseEmCima)
	{
		label->pop_back();
		label->push_back('-');
		m_popup->mostrar();
	}
	else if(!m_popup->mouseEmCima)
	{
		label->pop_back();
		label->push_back('+');
		m_popup->esconder();
	}
	m_popup->atualizar(0);
}

void BubbleUI::Items::Arvore::renderizar()
{
	ItemMenu::renderizar();
	m_popup->renderizar(GL_TRIANGLES);
}
