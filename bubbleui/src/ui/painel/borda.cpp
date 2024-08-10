#include "borda.hpp"
#include "painel.hpp"

using namespace BubbleUI;

Borda::Borda(Lado side, Painel* p) : lado(side), painel(p), linha(new Formas::Linha({}, p->obtCtx()))
{
	glLineWidth(2.f);
	linha->defCor({0.4f, 0.f, 0.4f});
}

void BubbleUI::Borda::atualizar(float deltaTime)
{
	switch (lado)
	{
	case DIREITA:
		linha->defPos({
			painel->obtRect().x + painel->obtRect().z,
			painel->obtRect().y - 1.f,
			0,
			painel->obtRect().w + 2.f
			});

		break;
	case ESQUERDA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y - 1.f,
			0,
			painel->obtRect().w + 2.f
			});
		break;
	case CIMA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y,
			painel->obtRect().z,
			0
			});
		break;
	case BAIXO:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y + painel->obtRect().w,
			painel->obtRect().z,
			0
			});
		break;
	default:
		break;
	}
	linha->atualizar(deltaTime);
}

void BubbleUI::Borda::renderizar()
{
	linha->renderizar();
}
