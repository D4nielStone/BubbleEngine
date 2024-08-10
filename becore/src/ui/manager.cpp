#include "manager.hpp"
#include "src/nucleo/engine.hpp"
#include "glad/glad.h"4
#include "GLFW/glfw3.h"

void BubbleUI::Manager::iniPaineisPadrao()
{
	retangulotest = new BubbleUI::Formas::Rect({ 0, 0, 100, 100 }, &contexto);
}

BubbleUI::Manager::Manager(Bubble::Nucleo::Engine* i) : engine(i)
{
	contexto.glfwWindow = engine->obterJanela();
	contexto.inputs = engine->obterGI();
	iniPaineisPadrao();
}

void BubbleUI::Manager::renderizar()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);
	retangulotest->renderizar();
}

void BubbleUI::Manager::atualizar(float deltaTime)
{
	glfwGetFramebufferSize(contexto.glfwWindow, &contexto.tamanho.width, &contexto.tamanho.height);
	retangulotest->atualizar(deltaTime);
}