#include "opcoes.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

Opcoes::Opcoes()
{
}
void Opcoes::atualizar()
{
    const int origem = (int)painel->obterRetangulo().y;
    const int diferenca = painel->posicaoWidget.y - origem;
    Moldura::defCor({ 0.13f, 0.11f, 0.16f, 1.f });
    Moldura::definirPosicao({ painel->widgetPadding.x + painel->posicaoWidget.x, origem + diferenca + painel->widgetPadding.y});
    Moldura::definirTamanho({ static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x * 2), static_cast<int>(painel->obterRetangulo().h - diferenca - painel->widgetPadding.y*2)});
    Moldura::atualizar();

    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
    texto.atualizar();
}

void Opcoes::renderizar() const
{
    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
    texto.renderizar();
}

void Opcoes::recarregar()
{
    texto.definirTexto("Nenhum projeto");
    texto.definirPai(this);
}

void Opcoes::adiBotao(const std::string& label)
{
    botoes.push_back(std::make_unique<Botao>(label, nullptr, false));
    botoes[botoes.size() - 1]->definirPai(this);
}
