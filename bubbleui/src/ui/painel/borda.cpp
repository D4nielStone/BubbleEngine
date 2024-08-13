#include "borda.hpp"
#include "painel.hpp"
#include <src/depuracao/debug.hpp>

using namespace BubbleUI;

Borda::Borda(Lado side, Painel* p) : lado(side), painel(p), linha(new Formas::Linha({}, p->obtCtx()))
{
	glLineWidth(2.f);
	colisao = new Colisao2d({}, p->obtCtx());
}

void BubbleUI::Borda::atualizar(float deltaTime)
{
	atualizarColisao();
	linha->atualizar(deltaTime);

	if (painel->selecionado)
	{
		atualizarCursor();
		atualizarArrasto();
	}
}

void BubbleUI::Borda::renderizar()
{
	if (painel->selecionado)
		linha->defCor({ 0.4f, 0.f, 0.4f });
	else
		linha->defCor({ 0.4f, 0.4f, 0.4f });

	linha->renderizar();
}

bool BubbleUI::Borda::cursor()
{
	if (colisao->mouseEmCima() || colisao->mouseEmCima() && arrastando)
		return true;
	else
		return false;
}

void BubbleUI::Borda::atualizarColisao()
{
	switch (lado)
	{
	case DIREITA:
		linha->defPos({
			painel->obtRect().x + painel->obtRect().w,
			painel->obtRect().y - 1.f,
			0,
			painel->obtRect().h + 2.f
			});
		colisao->defRect({
			painel->obtRect().x + painel->obtRect().w -2.f,
			painel->obtRect().y,
			10,
			painel->obtRect().h
			});
		break;
	case ESQUERDA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y - 1.f,
			0,
			painel->obtRect().h + 2.f
			});
		colisao->defRect({
			painel->obtRect().x - 10,
			painel->obtRect().y,
			12,
			painel->obtRect().h
			});
		break;
	case CIMA:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y,
			static_cast<float>(painel->obtRect().w),
			0
			});
		colisao->defRect({
			painel->obtRect().x,
			painel->obtRect().y - 10.f,
			painel->obtRect().w,
			12
			});
		break;
	case BAIXO:
		linha->defPos({
			painel->obtRect().x,
			painel->obtRect().y + painel->obtRect().h,
			static_cast<float>(painel->obtRect().w),
			0
			});
		colisao->defRect({
			painel->obtRect().x,
			painel->obtRect().y + painel->obtRect().h - 2.f,
			painel->obtRect().w,
			12
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
		if (painel->obtCtx()->inputs->mouseEnter == GLFW_RELEASE)
			mouse_1click = true;
		if (mouse_1click && painel->obtCtx()->inputs->mouseEnter == GLFW_PRESS)
		{
			arrastando = true;
			mouse_1click = false;
		}
	}
	else
		mouse_1click = false;

}

void BubbleUI::Borda::atualizarArrasto()
{
	if (arrastando && painel->obtCtx()->inputs->mouseEnter == GLFW_PRESS)
	{
		painel->arrastando = true;
		painel->redimen_atual = lado;
		switch (lado)
		{
		case DIREITA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			painel->adiTam({ static_cast<int>(painel->obtCtx()->inputs->mousex - mouse_pos_ini.x), 0 });
			break;
		case ESQUERDA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			painel->adiPos({ static_cast<int>(painel->obtCtx()->inputs->mousex - mouse_pos_ini.x), 0 });
			painel->adiTam({ -static_cast<int>(painel->obtCtx()->inputs->mousex - mouse_pos_ini.x), 0 });
			break;
		case CIMA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_vertical;
			painel->adiPos({ 0,static_cast<int>(painel->obtCtx()->inputs->mousey - mouse_pos_ini.y) });
			painel->adiTam({ 0,-static_cast<int>(painel->obtCtx()->inputs->mousey - mouse_pos_ini.y) });
			break;
		case BAIXO:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_vertical;
			painel->adiTam({ 0, static_cast<int>(painel->obtCtx()->inputs->mousey - mouse_pos_ini.y) });
			break;
		default:
			break;
		}
	}
	else
		arrastando = false;
	mouse_pos_ini = { static_cast<int>(painel->obtCtx()->inputs->mousex), static_cast<int>(painel->obtCtx()->inputs->mousey) };
}
