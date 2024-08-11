#include "borda.hpp"
#include "painel.hpp"

using namespace BubbleUI;

Borda::Borda(Lado side, Painel* p) : lado(side), painel(p), linha(new Formas::Linha({}, p->obtCtx()))
{
	glLineWidth(2.f);
	linha->defCor({0.4f, 0.f, 0.4f});
	colisao = new Colisao2d({}, p->obtCtx());
}

void BubbleUI::Borda::atualizar(float deltaTime)
{
	atualizarColisao();
	atualizarCursor();

	linha->atualizar(deltaTime);
}

void BubbleUI::Borda::renderizar()
{
	linha->renderizar();
}

bool BubbleUI::Borda::cursor()
{
	return colisao->mouseEmCima();
}

void BubbleUI::Borda::atualizarColisao()
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
		colisao->defRect({
			painel->obtRect().x + painel->obtRect().z,
			painel->obtRect().y,
			static_cast<int>(painel->obtRect().x + painel->obtRect().z + 10),
			static_cast<int>(painel->obtRect().w + painel->obtRect().y)
			});
		break;
	case ESQUERDA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y - 1.f,
			0,
			painel->obtRect().w + 2.f
			});
		colisao->defRect({
			painel->obtRect().x - 10,
			painel->obtRect().y,
			static_cast<int>(painel->obtRect().x),
			static_cast<int>(painel->obtRect().w + painel->obtRect().y)
			});
		break;
	case CIMA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y,
			painel->obtRect().z,
			0
			});
		colisao->defRect({
			painel->obtRect().x,
			painel->obtRect().y - 10.f,
			static_cast<int>(painel->obtRect().x + painel->obtRect().z),
			static_cast<int>(painel->obtRect().y)
			});
		break;
	case BAIXO:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y + painel->obtRect().w,
			painel->obtRect().z,
			0
			});
		colisao->defRect({
			painel->obtRect().x,
			painel->obtRect().y + painel->obtRect().w,
			static_cast<int>(painel->obtRect().x + painel->obtRect().z),
			static_cast<int>(painel->obtRect().y+ painel->obtRect().w+10.f)
			});
		break;
	default:
		break;
	}
}

void BubbleUI::Borda::atualizarCursor()
{
	if (colisao->mouseEmCima())
	{
		switch (lado)
		{
		case DIREITA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			break;
		case ESQUERDA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			break;
		case CIMA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_vertical;
			break;
		case BAIXO:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_vertical;
			break;
		default:
			break;
		}
	}
}
