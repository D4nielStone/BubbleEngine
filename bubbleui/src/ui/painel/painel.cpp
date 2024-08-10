#include "painel.hpp"

BubbleUI::Painel::Painel(Contexto* ctx) : corpo_rect(new Formas::Rect({0, 0, 100, 100}, ctx))
{
	// Customizacao do painel
	corpo_rect->defCor({0.23, 0.23, 0.23});
}

void BubbleUI::Painel::atualizar(float deltaTime)
{
	corpo_rect->atualizar(deltaTime);
}

void BubbleUI::Painel::renderizar()
{
	corpo_rect->renderizar();
}
