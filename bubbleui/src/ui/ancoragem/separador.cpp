/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "separador.hpp"
#include <src/depuracao/debug.hpp>

using namespace BubbleUI;

Separador::Separador(TipoAncoragem side, Ancora* p) : lado(side), pai(p)
{

}

float BubbleUI::Separador::proporcao() const
{
	return _Mproporcao;
}

void BubbleUI::Separador::atualizar()
{
	if (!colisao) colisao = std::make_shared < Colisao2d>(Vector4<int>(), contexto);
	atualizarColisao();
	atualizarArrasto();
	atualizarCursor();
}

void BubbleUI::Separador::atualizarColisao()
{
	switch (lado)
	{
	case Horizontal:
		colisao->definirBounds(Vector4<int>( pai->bounds.x + pai->bounds.w * _Mproporcao - 5, pai->bounds.y, 5, pai->bounds.h  ));
		break;
	case Vertical:
		colisao->definirBounds(Vector4<int>(pai->bounds.x , pai->bounds.y + pai->bounds.h * _Mproporcao - 5, pai->bounds.w  , 5  ));
	default:
		break;
	}
}

void BubbleUI::Separador::atualizarCursor()
{
	if (colisao->mouseEmCima()  )
	{
		switch (lado)
		{
		case Horizontal:
			contexto->cursor = contexto->cursor_horizontal;
			break; 
		case Vertical:
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
		// Obtém a posição do mouse
		auto mousePos = Vector2(inputs->mousex, inputs->mousey);

		switch (lado)
		{
		case Horizontal:
			// Calcula a nova proporção com base na posição X do mouse
			_Mproporcao = static_cast<float>(mousePos.x - pai->bounds.x) / pai->bounds.w;
			break;

		case Vertical:
			// Calcula a nova proporção com base na posição Y do mouse
			_Mproporcao = static_cast<float>(mousePos.y - pai->bounds.y) / pai->bounds.h;
			break;

		default:
			break;
		}

		// Garante que a proporção fique entre 0.0 e 1.0
		if (_Mproporcao < 0.0f)
			_Mproporcao = 0.0f;
		else if (_Mproporcao > 1.0f)
			_Mproporcao = 1.0f;

		// Atualiza a colisão com a nova proporção
		atualizarColisao();
	}
	else if (inputs->mouseEnter == GLFW_RELEASE)
	{
		// Finaliza o arrasto ao liberar o botão do mouse
		arrastando = false;
	}
}
