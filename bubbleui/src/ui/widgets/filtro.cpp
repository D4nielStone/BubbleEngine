/** @copyright Copyright (c) 2024 Daniel Oliveira */

/*
    Essa classe e´responsável por criar uma moldura com os projetos na pasta de projetos.
    No momento ela não é genérica.
*/

#include "filtro.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/ui/painel/depurador.hpp"

using namespace BubbleUI::Widgets;

/**
* AS funcoes estaticas sao referentes aos botoes
*/

static void abrirProjeto()
{

}

static void janelaNovoProjeto()
{
    //auto ctx = BubbleUI::janela("Novo projeto");
    //if (!ctx) return;
    //BubbleUI::adicionarPainel(ctx.get(), new BubbleUI::Paineis::Depurador({0, 0, 100, 100}));
}

static void novoProjeto()
{
    //BubbleUI::tarefa(janelaNovoProjeto);
}

Filtro::Filtro(const std::string& label) : label(label)
{
}
void Filtro::atualizar()
{
    definirPosicao({ painel->posicaoWidget.x, painel->posicaoWidget.y });
    definirTamanho({ static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x * 2), static_cast<int>(painel->obterRetangulo().h - painel->widgetPadding.y * 2 - (obterRetangulo().y - painel->obterRetangulo().y)) });
    Moldura::atualizar();

    posicaoWidget = { (int)obterRetangulo().x, (int)obterRetangulo().y };
    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
    text_box->atualizar();
    projetos->atualizar();
    
}

void Filtro::definirPai(Moldura* ctx)
{
    Widget::definirPai(ctx);
    defCor(ROXO_CLARO);
    adiBotao("Novo Projeto", novoProjeto);
    adiBotao("Abrir Projeto", abrirProjeto);
    text_box = std::make_unique<CaixaTexto>(label);
    text_box->definirPai(this);
    projetos = std::make_unique<Arquivos>();
    projetos->definirPai(this);
    recarregar();
}

void Filtro::renderizar()
{

    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
    text_box->renderizar();
    projetos->renderizar();
}

void Filtro::recarregar()
{
    projetos->recarregar(contexto->dirDoProjeto.c_str());
}

void Filtro::adiBotao(const std::string& label, std::function<void()> funcao)
{
    botoes.push_back(std::make_unique<Widgets::Botao>(label, funcao, false));
    botoes[botoes.size() - 1]->definirPai(this);
}
