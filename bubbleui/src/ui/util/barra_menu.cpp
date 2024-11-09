// Copyright (c) 2024 Daniel Oliveira

#include "barra_menu.hpp"
#include "src/ui/formas/linha.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/cena/scenemanager.hpp"
#include <rapidjson/writer.h>
#include "pop_up.hpp"
#include<filesystem>

void BubbleUI::Util::BarraMenu::renderizar() const
{
	Moldura::renderizar();
	texto_nome_projeto.renderizar();
	for (const auto& botao : botoes)
	{
		botao->renderizar();
	}
}
void BubbleUI::Util::BarraMenu::salvarCenaAtual() const
{
	// serializa cena no documento
	rapidjson::Document documento;
	if (Bubble::Cena::obterSceneManager()->cenaAtual())
		Bubble::Cena::obterSceneManager()->cenaAtual()->serializar(&documento);
	// puxa dados do documento para buffer de texto
	rapidjson::StringBuffer stringbuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringbuffer);
	documento.Accept(writer);

	std::filesystem::create_directories(contexto->dirDoProjeto + "/" + contexto->NomeDoProjeto);
	std::filesystem::create_directories(contexto->dirDoProjeto + "/" + contexto->NomeDoProjeto + "/" + "Cenas");
	std::filesystem::create_directories(contexto->dirDoProjeto + "/" + contexto->NomeDoProjeto + "/" + "Malhas");
	std::ofstream arquivo_de_cena(contexto->dirDoProjeto + "/" + contexto->NomeDoProjeto + "/" + "Cenas/"+ Bubble::Cena::obterSceneManager()->cenaAtual()->nome() + ".cena");
	
	if (!arquivo_de_cena.is_open()) {
		return;
	}

	// Escreve o conteúdo do buffer no arquivo
	arquivo_de_cena << stringbuffer.GetString();
	arquivo_de_cena.close();

}
void BubbleUI::Util::BarraMenu::atualizar()
{
	if (callbackSalvarCenaAtual)
		salvarCenaAtual();
	retangulo = { 4, 1, contexto->tamanho.width - 8, altura };
	Moldura::atualizar();
	posicaoWidget = { 0, 0 };
	widgetPadding = { 3, 3 };
	for (const auto& botao : botoes)
	{
		botao->atualizar();
	}
	texto_nome_projeto.atualizar();
	glfwSetCursor(contexto->glfwWindow, contexto->cursor);
}

void BubbleUI::Util::BarraMenu::adicionarBotao(std::unique_ptr<Items::ItemMenu> item)
{
	item->definirPai(this);
	item->quebrarLinha(false);
	botoes.push_back(std::move(item));
}

void BubbleUI::Util::BarraMenu::defContexto(std::shared_ptr<Contexto> ctx)
{
	configurar(ctx, {});
	*cor_base = { 0.26f, 0.21f, 0.28f, 1 };

	popupCena = std::make_shared<PopUp>(ctx);
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("Novo projeto"));
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("Abrir projeto"));

	adicionarBotao(std::make_unique<Items::Arvore>("Arquivo", popupCena));
	texto_nome_projeto = Widgets::Texto(contexto->NomeDoProjeto + " | " + contexto->VercaoDaEngine);

	texto_nome_projeto.definirPai(this);
	texto_nome_projeto.quebrarLinha = false;
}
