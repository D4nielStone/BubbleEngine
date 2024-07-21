#include "layout.hpp"
#include <src/util/includes.h>
#include <src/depuracao/debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Bubble::Interface::Layout::Layout(Janela j) : once(true)
{
	cursorMao = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	cor = {0.2f, 0.2f, 0.25f};
	tamanho = {1, 0.5};
}

void Bubble::Interface::Layout::renderizar()
{
	Quadrado::renderizar();
	abajanela.renderizar();
	// items dentro do layout
	//glEnable(GL_SCISSOR_TEST);
	//glScissor(posicao.x, posicao.y, tamanho.x, tamanho.y);
	for (Quadrado& quad : items)
	{
		quad.renderizar();
	}
	//glDisable(GL_SCISSOR_TEST);
}
void Bubble::Interface::Layout::atualizar()
{
	Vector2 mouse;
	converterMouse(&mouse);
	// atualiza aba da janela
	abajanela.defTam(Vector2{ tamanho.x, 0.05f });
	abajanela.defPos(Vector2{ posicaoG.x, posicaoG.y + tamanho.y});
	abajanela.defCor(Color{ 0.3, 0, 0.5 });

	if (inputs->mouseClick) {
		inputs->mouseClick = false;
	}
	if (inputs->mouseEnter == GLFW_PRESS) {
		if (!inputs->mouseClick && once == true) {
			inputs->mouseClick = true;
			once = false;
		}
	}
	else if (inputs->mouseEnter == GLFW_RELEASE) {
		once = true;
		inputs->mouseClick = false;

	}
	if (once) {
		inputs->mouseClick = false;
	}

	// logica para arrastar janela
	if (mouseEmCima(abajanela.obtPos(), abajanela.obtTam()) && inputs->mouseClick)
	{
		inicioArrastoPos = posicao;
		inicioMouseArrasto = Vector2(mouse.x, mouse.y);
		mouseArrastando = true;
		glfwSetCursor(janelaglfw, cursorMao);
	}
	if (mouseArrastando && inputs->mouseEnter == GLFW_PRESS)
	{
		ArrastoPos = inicioArrastoPos + Vector2{mouse.x - inicioMouseArrasto.x, mouse.y - inicioMouseArrasto.y};

		posicao = ArrastoPos;
	}
	else
	{
		mouseArrastando = false;
	}
	// atualiza items
	Quadrado::atualizar();
	abajanela.atualizar();
	for (Quadrado& quad : items)
	{
		quad.atualizar();
	}
}


// Função para converter coordenadas do mouse
void Bubble::Interface::Layout::converterMouse(Vector2* mousepos)
{
	int windowWidth, windowHeight;
	glfwGetWindowSize(janelaglfw, &windowWidth, &windowHeight);

	// Coordenadas do mouse em pixels
	float mouseX = static_cast<float>(inputs->mousex);
	float mouseY = static_cast<float>(inputs->mousey);

	// Converter para coordenadas normalizadas OpenGL (-1 a 1)
	float mouseXNormalized = (mouseX / windowWidth) * 2.0f - 1.0f;
	float mouseYNormalized = 1.0f - (mouseY / windowHeight) * 2.0f;

	// Coordenadas normalizadas do mouse
	glm::vec4 mousePosNormalized(mouseXNormalized, mouseYNormalized, 0.0f, 1.0f);

	// Converter para coordenadas do espaço da projeção ortográfica
	glm::vec4 mousePosOrtho = glm::inverse(projecao) * mousePosNormalized;

	// Ajustar a posição do mouse para o espaço da projeção ortográfica
	mousepos->x = mousePosOrtho.x;
	mousepos->y = mousePosOrtho.y;
}


bool Bubble::Interface::Layout::mouseEmCima(Vector2 boxpos, Vector2 boxsize)
{
	Vector2 mouse;
	converterMouse(&mouse);

	float posX = boxpos.x;
	float posY = boxpos.y;
	float width =  boxsize.x;
	float height = boxsize.y;

	// coordenadas e dimensões do box em coordenadas de tela
	glm::vec4 boxPosVec4(boxpos.x, boxpos.y, 0.0f, 1.0f);
	glm::vec4 boxSizeVec4(boxsize.x, boxsize.y, 0.0f, 1.0f);

	// aplicando a matriz ortográfica
	glm::vec4 transformedBoxPos = projecao * boxPosVec4;
	glm::vec4 transformedBoxSize = projecao * boxSizeVec4;

	// convertendo para 2D
	Vector2 transformedBoxPos2D(transformedBoxPos.x, transformedBoxPos.y);
	Vector2 transformedBoxSize2D(transformedBoxSize.x, transformedBoxSize.y);

	if (mouse.x > transformedBoxPos2D.x && mouse.x < transformedBoxPos2D.x + transformedBoxSize.x &&
		mouse.y > transformedBoxPos2D.y && mouse.y < transformedBoxPos2D.y + transformedBoxSize.y)
	{
		return true;
	}
	return false;
}
