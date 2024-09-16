#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "manager.hpp"
#include "src/nucleo/engine.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/jogo.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include <future>

// Inicia paineis padrão
void BubbleUI::Manager::iniPaineisPadrao()
{
	int margem = 10;
	lista_paineis.push_back(
		std::make_shared<Paineis::Editor>(contexto, engine->obterGC(), Vector4{ (float)margem, (float)margem,  contexto->tamanho.width / 2 - 15, contexto->tamanho.height - margem * 2}));
	lista_paineis.push_back(
		std::make_shared<Paineis::Jogo>(contexto, engine->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2,  (float)margem,  contexto->tamanho.width / 2 - 15, contexto->tamanho.height /2 - margem*2 }));
	lista_paineis.push_back(
		std::make_shared<Paineis::Entidades>(contexto, engine->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2, contexto->tamanho.height / 2.f, contexto->tamanho.width / 4 - margem * 2, contexto->tamanho.height / 2 - margem}));
	lista_paineis.push_back(
<<<<<<< HEAD
		std::make_shared<Paineis::Inspetor>(contexto, engine->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2 + (contexto->tamanho.width / 4.f) - margem, contexto->tamanho.height /2.f, contexto->tamanho.width / 4 - margem, contexto->tamanho.height/2 - margem }));
=======
		std::make_shared<Paineis::Inspetor>(contexto, engine->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2 + (contexto->tamanho.width / 4.f) - margem, contexto->tamanho.height /2.f, contexto->tamanho.width / 4 - margem/2, contexto->tamanho.height/2 - margem }));
>>>>>>> master
}

// Seleciona o painel
// \param Painel
void BubbleUI::Manager::painelSelecionado(std::shared_ptr<Painel> painel)
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
BubbleUI::Manager::Manager(Bubble::Nucleo::Engine* i) : engine(i)
{
	contexto = std::make_shared<Contexto>();
	colisao_painel = Colisao2d({}, contexto);
	contexto->glfwWindow = engine->obterJanela();
	contexto->inputs = engine->obterGI();
	glfwGetFramebufferSize(contexto->glfwWindow, &contexto->tamanho.width, &contexto->tamanho.height);
	iniPaineisPadrao();
	if (lista_paineis.size() > 0)
		lista_paineis[lista_paineis.size() - 1]->selecionado = true;
}

// Renderiza paineis
void BubbleUI::Manager::renderizar() const
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, contexto->tamanho.width, contexto->tamanho.height);

	glEnable(GL_SCISSOR_TEST);
	for (auto& painel : lista_paineis)
	{
		painel->renderizar();
	}
	glDisable(GL_SCISSOR_TEST);
}

#include "src/threadpool.hpp" // Assumindo que você salvou a implementação do thread pool em um arquivo

void BubbleUI::Manager::atualizar()
{
	glfwGetFramebufferSize(contexto->glfwWindow, &contexto->tamanho.width, &contexto->tamanho.height);
	verificarSelecionado();

	std::vector<std::future<void>> futures;

	// Atualiza cada painel de forma assíncrona usando o future
	for (const auto& painel : lista_paineis) {
		futures.push_back(std::async(std::launch::async, [painel](){
			painel->atualizar();
			}));
	}

	// Aguarda todas as tarefas terminarem
	for (auto& f : futures) {
		f.get();
	}
}

// Verifica Painel selecionado
void BubbleUI::Manager::verificarSelecionado()
{
	bool cursor = true, depth = false;
	for (size_t i = lista_paineis.size(); i > 0; i--)
	{
		auto &painel = lista_paineis[i - 1];

		// Seleciona o painel se está na frente, e for clicado na hora certa
		colisao_painel.defRect(painel->obterRetangulo());

		if (colisao_painel.mouseEmCima() && !depth && cursor)
		{
			if (contexto->inputs->mouseEnter == GLFW_RELEASE)
			{
				painel->mouse1click = true;
			}
			if (contexto->inputs->mouseEnter == GLFW_PRESS && painel->mouse1click)
			{
				painelSelecionado(painel);

				depth = true;
				painel->mouse1click = false;
			}
			if (contexto->inputs->mouseEnter == GLFW_PRESS && contexto->inputs->mouseButton == GLFW_MOUSE_BUTTON_RIGHT)
			{
				painel->mostrarPopup = true;
			}
			else if (contexto->inputs->mouseEnter == GLFW_PRESS && contexto->inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
				painel->esconderPopup = true;
		}
		else
			painel->mouse1click = false;

		// Faz correção do cursor
		if (!painel->cursorNormal())
			cursor = false;
	}
	if (cursor)
		contexto->cursor = contexto->cursor_normal;
	// Verifica se o cursor mudou e atualiza
	if (contexto->cursor != cursor_antigo)
	{
		cursor_antigo = contexto->cursor;
		glfwSetCursor(contexto->glfwWindow, contexto->cursor);
	}
}