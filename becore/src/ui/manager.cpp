#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "manager.hpp"
#include "src/nucleo/engine.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/entidades.hpp"

// Inicia paineis padrão
void BubbleUI::Manager::iniPaineisPadrao()
{
	lista_paineis.push_back(new Paineis::Depurador(&contexto));
	lista_paineis.push_back(new Paineis::Entidades(&contexto, engine->obterGC(), {300, 10, 400, 300}));
	lista_paineis.push_back(new Paineis::Editor(&contexto, engine->obterGC(), {10, 10, 400, 300}));
}

// Seleciona o painel
// \param Painel
void BubbleUI::Manager::painelSelecionado(Painel* painel)
{
	painel->selecionado = true;

	// Guarda index do painel
	size_t idx_painel = 0;
	for (size_t i = 0; i < lista_paineis.size(); i++)
	{
		if (lista_paineis[i] == painel)
		{
			idx_painel = i;
		}
		else
			lista_paineis[i]->selecionado = false;
	}

	// Traz todos os paineis á frente para o index inicial
	for (size_t i = idx_painel; i < lista_paineis.size(); i++)
	{
		if(i+1 != lista_paineis.size())
		lista_paineis[i] = lista_paineis[i+1];
	}

	// Leva o painel selecionado para frente
	lista_paineis[lista_paineis.size()-1] = painel;
}

// Inicia manager
BubbleUI::Manager::Manager(Bubble::Nucleo::Engine* i) : engine(i), colisao_painel({}, &contexto)
{
	contexto.glfwWindow = engine->obterJanela();
	contexto.inputs = engine->obterGI();
	iniPaineisPadrao();
	if (lista_paineis.size() > 0)
		lista_paineis[lista_paineis.size() - 1]->selecionado = true;
}

// Renderiza paineis
void BubbleUI::Manager::renderizar()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, contexto.tamanho.width, contexto.tamanho.height);

	glEnable(GL_SCISSOR_TEST);
	for (Painel* painel : lista_paineis)
	{
		painel->renderizar();
	}
	glDisable(GL_SCISSOR_TEST);

	glfwSetCursor(contexto.glfwWindow, contexto.cursor);
}

// Atualiza paineis
void BubbleUI::Manager::atualizar(float deltaTime)
{
	glfwGetFramebufferSize(contexto.glfwWindow, &contexto.tamanho.width, &contexto.tamanho.height);
	bool cursor = true, depth = false;
	for (size_t i = lista_paineis.size(); i > 0; i--)
	{
		Painel* painel = lista_paineis[i-1];
		// Atualiza Painel
		painel->atualizar(deltaTime);

		// Seleciona o painel se está na frente, e for clicado na hora certa
		colisao_painel.defRect(painel->obtRect());

		if (colisao_painel.mouseEmCima() && !depth && cursor)
		{
			if (contexto.inputs->mouseEnter == GLFW_RELEASE)
			{
				painel->mouse1click = true;
			}
			if (contexto.inputs->mouseEnter == GLFW_PRESS && painel->mouse1click)
			{
				painelSelecionado(painel);
				depth = true;
				painel->mouse1click = false;
			}
		}
		else
			painel->mouse1click = false;

		// Faz correção do cursor
		if (!painel->cursorNormal())
			cursor = false;
	}
	if(cursor)
		contexto.cursor = contexto.cursor_normal;
}