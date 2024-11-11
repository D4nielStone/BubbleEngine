
// Copyright (c) 2024 Daniel Oliveira

#include "filtro.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

Filtro::Filtro(const std::string& label) : label(label)
{
}
void Filtro::atualizar()
{
    defCor(ROXO_CLARO);
    definirPosicao({ painel->posicaoWidget.x + painel->widgetPadding.x, painel->posicaoWidget.y + painel->widgetPadding.y});
    definirTamanho({static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x*2), static_cast<int>(painel->obterRetangulo().h - painel->widgetPadding.y*2) });
    Moldura::atualizar();

    posicaoWidget = { (int)obterRetangulo().x, (int)obterRetangulo().y};
    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
    text_box->atualizar();
    projetos->atualizar();
}

void Filtro::renderizar() const
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
    botoes.clear();
    adiBotao("Novo Projeto");
    adiBotao("Abrir Projeto");
    text_box = std::make_unique<CaixaTexto>(label);
    text_box->definirPai(this);
    projetos = std::make_unique<Opcoes>();
    projetos->definirPai(this);
    projetos->recarregar();
}

void Filtro::adiBotao(const std::string& label)
{
    botoes.push_back(std::make_unique<Widgets::Botao>(label, nullptr, false));
    botoes[botoes.size() - 1]->definirPai(this);
    botoes[botoes.size() - 1]->quebrarLinha = false;
}
