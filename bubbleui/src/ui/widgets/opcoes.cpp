#include "opcoes.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Opcoes::Opcoes()
{
}
void BubbleUI::Widgets::Opcoes::atualizar()
{
    const int origem = (int)painel->obterRetangulo().y;
    const int diferenca = painel->posicaoWidget.y - origem;
    Moldura::defCor({ 0.13f, 0.11f, 0.16f, 1.f });
    Moldura::definirPosicao({ painel->widgetPadding.x + painel->posicaoWidget.x, origem + diferenca + painel->widgetPadding.y});
    Moldura::definirTamanho({ static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x * 2), static_cast<int>(painel->obterRetangulo().h - diferenca - painel->widgetPadding.y*2)});
    Moldura::atualizar();

    posicaoWidget = { 0,0 };
    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
}

void BubbleUI::Widgets::Opcoes::renderizar() const
{
    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
}

void BubbleUI::Widgets::Opcoes::recarregar()
{
}

void BubbleUI::Widgets::Opcoes::adiBotao(const std::string& label)
{
    botoes.push_back(std::make_unique<Botao>(label, nullptr, false));
    botoes[botoes.size() - 1]->definirPai(this);
}
