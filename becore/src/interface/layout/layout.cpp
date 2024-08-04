#include "layout.hpp"
#include <src/util/includes.h>
#include <src/depuracao/debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/interface/imagem/Imagem.h"

Bubble::Interface::Layout::Layout()
{
	cor = { 0.55f, 0.55f, 0.55f };
	tamanho = { 1, 1 };
	posicao = Vector2{ abajanela.obtPos().x, abajanela.obtPos().y - tamanho.y };
	abajanela.defPos(Vector2{ -0.5, 0 });
	abajanela.defCor(Color{0, 0, 0 });
	auto logo = Imagem("ICON.ico", 0.2f);
	logo.defPos(Vector2{ 0.f, tamanho.y });
	adicImagem(logo);
}
void Bubble::Interface::Layout::adicImagem(Imagem& quad)
{
	quad.defPai(this); imagems.push_back(quad);
}
void Bubble::Interface::Layout::renderizar()
{

	Quadrado::renderizar();
	abajanela.renderizar();
	for (Imagem& quad : imagems)
	{
		quad.renderizar();
	}

}
void Bubble::Interface::Layout::atualizar()
{
	// atualiza aba da janela
	abajanela.defTam(Vector2{ tamanho.x, 0.05f });
	if (abajanela.arrastando())
		abajanela.defPos(abajanela.ArrastoPos);
		posicao = Vector2{ abajanela.obtPos().x, abajanela.obtPos().y - tamanho.y };
	Quadrado::atualizar();
	abajanela.defInputs(inputs);
	abajanela.atualizar();
	// atualiza items dentro do layout
	glEnable(GL_SCISSOR_TEST);
	glScissor(posicao.x, posicao.y, tamanho.x, tamanho.y);
	for (Imagem& quad : imagems)
	{
		quad.atualizar();
	}
	glDisable(GL_SCISSOR_TEST);
}