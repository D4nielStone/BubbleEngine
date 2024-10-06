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
	retangulo = { 0, 1, contexto->tamanho.width, altura };
	Moldura::atualizar();
	posicaoWidget = { 0, 0 };
	widgetPadding = { 3, 3 };
	for (const auto& botao : botoes)
	{
		botao->atualizar();
	}
	texto_nome_projeto.atualizar();
}

void BubbleUI::Util::BarraMenu::adicionarBotao(std::unique_ptr<Items::ItemMenu> item)
{
	item->defMoldura(this);
	item->quebrarLinha(false);
	botoes.push_back(std::move(item));
}

void BubbleUI::Util::BarraMenu::defContexto(std::shared_ptr<Contexto> ctx)
{
	this->contexto = ctx;
	cor_base = { 0.1, 0.1, 0.1, 1 };
	linha_d = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_b = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_e = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_c = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_d->defCor({ 0.35f, 0.35f, 0.35f, 1.f });
	linha_b->defCor({ 0.35f, 0.35f, 0.35f, 1.f });
	linha_e->defCor({ 0.55f, 0.55f, 0.55f, 1.f });
	linha_c->defCor({ 0.55f, 0.55f, 0.55f, 1.f });

	popupCena = std::make_shared<PopUp>(contexto);
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("salvar projeto"));
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("abrir projeto"));
	popupCena->adiItem(std::make_shared<Items::Botao>("salver cena atual", &callbackSalvarCenaAtual));

	adicionarBotao(std::make_unique<Items::Arvore>("Configs", popupCena));
	texto_nome_projeto = Items::Texto(contexto->NomeDoProjeto + " | " + contexto->VercaoDaEngine);

	texto_nome_projeto.defMoldura(this);
	texto_nome_projeto.quebrarLinha(false);
}
