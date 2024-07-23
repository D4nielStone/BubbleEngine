#include "layout.hpp"
#include <src/util/includes.h>
#include <src/depuracao/debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/interface/imagem/Imagem.h"

Bubble::Interface::Layout::Layout(TipoLayout j)
{
	cursorMao = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	tipo_layout = j;
	switch (j)
	{
	case Bubble::Interface::L_MENU:
		tamanho = { 2, 0.05 };
		cor = { 0.15f, 0.15f, 0.15f };
		posicao = {-1, 0.95};
		break;
	case Bubble::Interface::L_JANELA:
		cor = { 0.55f, 0.55f, 0.55f };
		tamanho = { 1, 0.2};
		posicao = Vector2{ abajanela.obtPos().x, abajanela.obtPos().y - tamanho.y };
		abajanela.defPos(Vector2{ -0.5, 0 });
		abajanela.defCor(Color{ 0.3, 0, 0.5 });
		break;
	default:
		break;
	}
}
void Bubble::Interface::Layout::adicImagem(Imagem& quad)
{
	quad.defPai(this); imagems.push_back(quad);
}
void Bubble::Interface::Layout::renderizar()
{
	switch (tipo_layout)
	{
	case Bubble::Interface::L_MENU:
		posicao = { -1 * aspect * tamanho_ui, 0.95 };
		tamanho = { 2*aspect*tamanho_ui, 0.05 };
		Quadrado::renderizar();
		break;
	case Bubble::Interface::L_JANELA:
		Quadrado::renderizar();
		abajanela.renderizar();
		for (Imagem& quad : imagems)
		{
			quad.renderizar();
		}
		break;
	default:
		break;
	}
}
void Bubble::Interface::Layout::atualizar()
{
	switch (tipo_layout)
	{
	case Bubble::Interface::L_MENU:
		Quadrado::atualizar();
		break;
	case Bubble::Interface::L_JANELA:
		// atualiza aba da janela
		abajanela.defTam(Vector2{ tamanho.x, 0.05f });
		if (abajanela.arrastando())
			abajanela.defPos(abajanela.ArrastoPos);
			posicao = Vector2{ abajanela.obtPos().x, abajanela.obtPos().y - tamanho.y };
		Quadrado::atualizar();
		abajanela.defInputs(inputs);
		abajanela.atualizar();
		// atualiza items
		// items dentro do layout
		//glEnable(GL_SCISSOR_TEST);
		//glScissor(posicao.x, posicao.y, tamanho.x, tamanho.y);
		for (Imagem& quad : imagems)
		{
			quad.atualizar();
		}
		//glDisable(GL_SCISSOR_TEST);
		break;
	default:
		break;
	}
}