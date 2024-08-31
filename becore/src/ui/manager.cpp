#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "manager.hpp"
#include "src/nucleo/engine.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include <future>

// Inicia paineis padrão
void BubbleUI::Manager::iniPaineisPadrao()
{
	lista_paineis.push_back(new Paineis::Depurador(&contexto));
	lista_paineis.push_back(new Paineis::Entidades(&contexto, engine->obterGC(), {2, 2, 100, 478 }));
	lista_paineis.push_back(new Paineis::Editor(&contexto, engine->obterGC(), {112, 2, 300, 478 }));
	lista_paineis.push_back(new Paineis::Inspetor(&contexto, engine->obterGC(), {422, 2, 218, 478}));
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
	for (auto& painel : lista_paineis)
	{
		painel->renderizar();
	}
	glDisable(GL_SCISSOR_TEST);

	if (contexto.cursor != cursor_antigo)
	{
		cursor_antigo = contexto.cursor;
		glfwSetCursor(contexto.glfwWindow, contexto.cursor);
	}
}

// Atualiza paineis
void BubbleUI::Manager::atualizar()
{
	glfwGetFramebufferSize(contexto.glfwWindow, &contexto.tamanho.width, &contexto.tamanho.height);
	
	verificarSelecionado();

	std::vector<std::future<void>> futures;

	for (auto& painel : lista_paineis)
	{
		futures.push_back(std::async(std::launch::async, [&painel]() {
			try {
				painel->atualizar();
			}
			catch (const std::exception& e) {
				Debug::emitir(Debug::Erro, "Não foi possível atualizar painel");
				return;
			}
			}));
	}
	// Esperar todos os painéis terminarem de atualizar
	for (auto& future : futures) {
		future.get();
	}

	renderizar();
}

// Verifica Painel selecionado
void BubbleUI::Manager::verificarSelecionado()
{
	bool cursor = true, depth = false;
	for (size_t i = lista_paineis.size(); i > 0; i--)
	{
		auto painel = lista_paineis[i - 1];

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
			if (contexto.inputs->mouseEnter == GLFW_PRESS && contexto.inputs->mouseButton == GLFW_MOUSE_BUTTON_RIGHT)
			{
				painel->mostrar_popup = true;
			}
			else if (contexto.inputs->mouseEnter == GLFW_PRESS && contexto.inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
				painel->esconder_popup = true;
		}
		else
			painel->mouse1click = false;

		// Faz correção do cursor
		if (!painel->cursorNormal())
			cursor = false;
	}
	if (cursor)
		contexto.cursor = contexto.cursor_normal;
}