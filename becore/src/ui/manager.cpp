#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "manager.hpp"
#include "src/nucleo/engine.hpp"

void BubbleUI::Manager::iniPaineisPadrao()
{
	lista_paineis.push_back(new Painel(&contexto));
}

BubbleUI::Manager::Manager(Bubble::Nucleo::Engine* i) : engine(i)
{
	contexto.glfwWindow = engine->obterJanela();
	contexto.inputs = engine->obterGI();
	glfwGetFramebufferSize(contexto.glfwWindow, &contexto.tamanho_ini.width, &contexto.tamanho_ini.height);
	iniPaineisPadrao();
}

void BubbleUI::Manager::renderizar()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, contexto.tamanho.width, contexto.tamanho.height);

	for (Painel* painel : lista_paineis)
	{
		painel->renderizar();
	}

	glfwSetCursor(contexto.glfwWindow, contexto.cursor);
}

void BubbleUI::Manager::atualizar(float deltaTime)
{
	glfwGetFramebufferSize(contexto.glfwWindow, &contexto.tamanho.width, &contexto.tamanho.height);
	for (Painel* painel : lista_paineis)
	{
		painel->atualizar(deltaTime);
	}
}