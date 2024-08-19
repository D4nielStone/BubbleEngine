#include "separador.hpp"
#include "painel.hpp"
#include <src/depuracao/debug.hpp>

using namespace BubbleUI;

Separador::Separador(Lado side, Painel* p) : lado(side), painel(p)
{
	colisao = new Colisao2d({}, p->obtCtx());
}

void BubbleUI::Separador::atualizar()
{
	atualizarColisao();
	atualizarCursor();

	if (painel->selecionado)
	{
		atualizarArrasto();
	}
}

bool BubbleUI::Separador::cursor()
{
	if (colisao->mouseEmCima())
		return true;
	else
		return false;
}

void BubbleUI::Separador::atualizarColisao()
{
	switch (lado)
	{
	case DIREITA:
		colisao->defRect({
			painel->obtRect().x + painel->obtRect().w -2.f,
			painel->obtRect().y,
			10,
			painel->obtRect().h
			});
		break;
	case ESQUERDA:
		colisao->defRect({
			painel->obtRect().x - 10,
			painel->obtRect().y,
			12,
			painel->obtRect().h
			});
		break;
	case CIMA:
		colisao->defRect({
			painel->obtRect().x,
			painel->obtRect().y - 10.f,
			painel->obtRect().w,
			12
			});
		break;
	case BAIXO:
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

void BubbleUI::Separador::atualizarCursor()
{
	if (colisao->mouseEmCima() && painel->selecionado)
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

void BubbleUI::Separador::atualizarArrasto()
{
	if (arrastando && painel->obtCtx()->inputs->mouseEnter == GLFW_PRESS)
	{
		painel->arrastando = true;
		painel->redimen_atual = lado;
		switch (lado)
		{
		case DIREITA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			painel->adiTam({ static_cast<int>(painel->obtCtx()->inputs->mousex) - mouse_pos_ini.x, 0 });
			break;
		case ESQUERDA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_horizontal;
			painel->adiPos({ static_cast<float>(painel->obtCtx()->inputs->mousex - mouse_pos_ini.x), 0 });
			painel->adiTam({ -static_cast<int>(painel->obtCtx()->inputs->mousex - mouse_pos_ini.x), 0 });
			break;
		case CIMA:
			painel->obtCtx()->cursor = painel->obtCtx()->cursor_vertical;
			painel->adiPos({ 0,static_cast<float>(painel->obtCtx()->inputs->mousey - mouse_pos_ini.y) });
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
