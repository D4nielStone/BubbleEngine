#include "barra_menu.hpp"
#include "src/ui/formas/linha.hpp"
#include "pop_up.hpp"

void BubbleUI::Util::BarraMenu::renderizar() const
{
	Moldura::renderizar();
	texto_nome_projeto.renderizar();
	for (const auto& botao : botoes)
	{
		botao->renderizar();
	}
}

void BubbleUI::Util::BarraMenu::atualizar()
{
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
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("salvar projeto atual"));
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("abrir projeto"));
	popupCena->adiItem(std::make_shared<Items::ItemMenu>("salver cena atual"));

	adicionarBotao(std::make_unique<Items::Arvore>("Configs.Cena", popupCena));
	texto_nome_projeto = Items::Texto(contexto->NomeDoProjeto + " | " + contexto->VercaoDaEngine);

	texto_nome_projeto.defMoldura(this);
	texto_nome_projeto.quebrarLinha(false);
}
