#include "filtro.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Filtro::Filtro(const std::string& label) : label(label)
{
}
void BubbleUI::Widgets::Filtro::atualizar()
{
    Moldura::defCor({0.25f, 0.21f, 0.29f, 1.f});
    Moldura::definirPosicao({painel->widgetPadding.x, painel->posicaoWidget.y + painel->widgetPadding.y});
    Moldura::definirTamanho({static_cast<int>(painel->obterRetangulo().w - Moldura::obterRetangulo().x- painel->widgetPadding.x), static_cast<int>(painel->obterRetangulo().h - Moldura::obterRetangulo().y- painel->widgetPadding.y) });
    Moldura::atualizar();

    posicaoWidget = { 0,0 };
    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
    text_box->atualizar();
    projetos->atualizar();
}

void BubbleUI::Widgets::Filtro::renderizar() const
{
    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
    text_box->renderizar();
    projetos->renderizar();
}

void BubbleUI::Widgets::Filtro::recarregar()
{
    botoes.clear();
    adiBotao("Novo Projeto");
    adiBotao("Abrir Projeto");
    text_box = std::make_unique<CaixaTexto>(label);
    text_box->definirPai(this);
    projetos = std::make_unique<Opcoes>();
    projetos->definirPai(this);
}

void BubbleUI::Widgets::Filtro::adiBotao(const std::string& label)
{
    botoes.push_back(std::make_unique<Botao>(label, nullptr, false));
    botoes[botoes.size() - 1]->definirPai(this);
}
