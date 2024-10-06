#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "manager.hpp"
#include "src/nucleo/projeto.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/jogo.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include <future>

// Inicia paineis padrão
void BubbleUI::Manager::iniPaineisPadrao()
{
	int margem = 10, y_start = 34;
	lista_paineis.push_back(
		std::make_shared<Paineis::Editor>(contexto, projeto->obterGC(), Vector4{ (float)margem, (float)margem+y_start,  contexto->tamanho.width / 2 - 15, contexto->tamanho.height - margem * 2 - y_start }));
	lista_paineis.push_back(
		std::make_shared<Paineis::Jogo>(contexto, projeto->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2,  (float)margem+y_start,  contexto->tamanho.width / 2 - 15, contexto->tamanho.height /2 - margem*2 - y_start }));
	lista_paineis.push_back(
		std::make_shared<Paineis::Entidades>(contexto, projeto->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2, contexto->tamanho.height / 2.f + y_start, contexto->tamanho.width / 4 - margem * 2, contexto->tamanho.height / 2 - margem - y_start }));
	lista_paineis.push_back(
		std::make_shared<Paineis::Inspetor>(contexto, projeto->obterGC(), Vector4{ contexto->tamanho.width / 2.f + margem / 2 + (contexto->tamanho.width / 4.f) - margem, contexto->tamanho.height /2.f + y_start, contexto->tamanho.width / 4 - margem/2, contexto->tamanho.height/2 - margem - y_start }));
	lista_paineis.push_back(
		std::make_shared<Paineis::Depurador>(contexto));
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
BubbleUI::Manager::Manager(Bubble::Nucleo::Projeto* i) : projeto(i)
{
	contexto = std::make_shared<Contexto>();
	colisao_painel = Colisao2d({}, contexto);
	contexto->glfwWindow = projeto->obterJanela();
	contexto->inputs = projeto->obterGI();
	contexto->NomeDoProjeto = projeto->obterNome();
	contexto->NomeGpu = projeto->obterNomeGPU();
	contexto->VercaoOpengl = projeto->obterVercaoGL();
	glfwGetFramebufferSize(contexto->glfwWindow, &contexto->tamanho.width, &contexto->tamanho.height);
	iniPaineisPadrao();
	barra_de_menu.defContexto(contexto);
	if (lista_paineis.size() > 0)
		lista_paineis[lista_paineis.size() - 1]->selecionado = true;
}

// Renderiza paineis
void BubbleUI::Manager::renderizar() const
{
	glClearColor(0.2, 0.2, 0.2, 1);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, contexto->tamanho.width, contexto->tamanho.height);

	glEnable(GL_SCISSOR_TEST);
	for (auto& painel : lista_paineis)
	{
		painel->renderizar();
	}
	glDisable(GL_SCISSOR_TEST);
	barra_de_menu.renderizar();
}

void BubbleUI::Manager::atualizar()
{
	glfwGetFramebufferSize(contexto->glfwWindow, &contexto->tamanho.width, &contexto->tamanho.height);
	verificarSelecionado();


	// Atualiza cada painel de forma assíncrona usando o future
	for (const auto& painel : lista_paineis) {
			painel->atualizar();
	}

	barra_de_menu.atualizar();
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