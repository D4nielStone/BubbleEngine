#include "filtro.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

Filtro::Filtro(const std::string& label) : label(label)
{
}
void Filtro::atualizar()
{
    defCor({0.25f, 0.21f, 0.29f, 1.f});
    definirPosicao({painel->widgetPadding.x, painel->posicaoWidget.y + painel->widgetPadding.y});
    definirTamanho({static_cast<int>(painel->obterRetangulo().w - obterRetangulo().x- painel->widgetPadding.x), static_cast<int>(painel->obterRetangulo().h - obterRetangulo().y- painel->widgetPadding.y) });
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
}
