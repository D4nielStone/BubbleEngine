
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Formas::Moldura *painel)
{
    quebrarLinha = true;
    frase = static_cast<Painel*>(painel) ? static_cast<Painel*>(painel)->nome() : "Moldura sem nome";
    if (frase == "Moldura sem nome")painel_flag = false;
    this->painel = painel;
    resolucao = 14;
    letra_padding = {4, 2};
    definirFonte();
    Texto::Texto(frase);
    corpo_rect = std::make_unique<Formas::Rect>(painel->obterContexto(), bubble::vetor4<int>{});
}

void BubbleUI::Aba::atualizar()
{
    auto padding_antigo = painel->widgetPadding.y;
    painel->widgetPadding.y = 0;
    renderizar_texto(frase);
    painel->widgetPadding.y = padding_antigo;

    corpo_rect->definirPosicao({static_cast<int>(painel->obterRetangulo().x), static_cast<int>(painel->obterRetangulo().y)});
    corpo_rect->definirTamanho({ painel->obterRetangulo().w, 16});
    box_pos = {static_cast<float>(corpo_rect->obterRetangulo().x), static_cast<float>(corpo_rect->obterRetangulo().y)};
    box_size = { static_cast<float>(corpo_rect->obterRetangulo().w), static_cast<float>(corpo_rect->obterRetangulo().h) };
    corpo_rect->atualizar();
}

void BubbleUI::Aba::renderizar()
{
    corpo_rect->renderizar();
    Texto::renderizar();
}

BubbleUI::Formas::Rect* BubbleUI::Aba::obterCorpo() const
{
    return corpo_rect.get();
}
