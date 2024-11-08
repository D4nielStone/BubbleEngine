// Copyright (c) 2024 Daniel Oliveira
// Licenciado sob a licença MIT. Consulte o arquivo LICENSE para mais informações.
#include "separador.hpp"
#include "painel.hpp"
#include <src/depuracao/debug.hpp>

using namespace BubbleUI;

Separador::Separador(const Lado side, Painel* p) :contexto(p->obterContexto()), inputs(p->obterContexto()->inputs), lado(side), painel(p), colisao(Colisao2d({}, p->obterContexto()))
{
}

void BubbleUI::Separador::atualizar()
{
	if (!painel->selecionado || !painel->redimensionavel)
		return;
	atualizarColisao();
	atualizarArrasto();
	atualizarCursor();
}

bool BubbleUI::Separador::cursor() const
{
	return colisao.mouseEmCima();
}

void BubbleUI::Separador::atualizarColisao()
{
	switch (lado)
	{
	case DIREITA:
		colisao.defRect({
			painel->obterRetangulo().x + painel->obterRetangulo().w -2.f,
			painel->obterRetangulo().y,
			10,
			painel->obterRetangulo().h
			});
		break;
	case ESQUERDA:
		colisao.defRect({
			painel->obterRetangulo().x - 10,
			painel->obterRetangulo().y,
			12,
			painel->obterRetangulo().h
			});
		break;
	case CIMA:
		colisao.defRect({
			painel->obterRetangulo().x,
			painel->obterRetangulo().y - 10.f,
			painel->obterRetangulo().w,
			12
			});
		break;
	case BAIXO:
		colisao.defRect({
			painel->obterRetangulo().x,
			painel->obterRetangulo().y + painel->obterRetangulo().h - 2.f,
			painel->obterRetangulo().w,
			12
			});
		break;
	default:
		break;
	}
}

void BubbleUI::Separador::atualizarCursor()
{
	if (colisao.mouseEmCima() && painel->selecionado)
	{
		switch (lado)
		{
		case DIREITA:
			contexto->cursor = contexto->cursor_horizontal;
			break;
		case ESQUERDA:
			contexto->cursor = contexto->cursor_horizontal;
			break;
		case CIMA:
			contexto->cursor = contexto->cursor_vertical;
			break;
		case BAIXO:
			contexto->cursor = contexto->cursor_vertical;
			break;
		default:
			break;
		}
		if (inputs->mouseEnter == GLFW_RELEASE)
			mouse_1click = true;
		if (mouse_1click && inputs->mouseEnter == GLFW_PRESS)
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
	if (arrastando && inputs->mouseEnter == GLFW_PRESS)
	{
		painel->arrastando = true;
		painel->redimensionamentoAtual = lado;
		switch (lado)
		{
		case DIREITA:
			contexto->cursor = contexto->cursor_horizontal;
				painel->adiTam({ static_cast<int>(inputs->mousex) - mouse_pos_ini.x, 0 });
			break;
		case ESQUERDA:
			contexto->cursor = contexto->cursor_horizontal;
				painel->adiPos({ static_cast<int>(inputs->mousex - mouse_pos_ini.x), 0 });
				painel->adiTam({ -static_cast<int>(inputs->mousex - mouse_pos_ini.x), 0 });
			break;
		case CIMA:
			contexto->cursor = contexto->cursor_vertical;
				painel->adiPos({ 0,static_cast<int>(inputs->mousey - mouse_pos_ini.y) });
				painel->adiTam({ 0,-static_cast<int>(inputs->mousey - mouse_pos_ini.y) });
			break;
		case BAIXO:
			contexto->cursor = contexto->cursor_vertical;
				painel->adiTam({ 0, static_cast<int>(inputs->mousey - mouse_pos_ini.y) });
			break;
		default:
			break;
		}
	}
	else
		arrastando = false;
	mouse_pos_ini = { static_cast<int>(inputs->mousex), static_cast<int>(inputs->mousey) };
}
