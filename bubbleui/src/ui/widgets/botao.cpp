#include "botao.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Botao::Botao(const std::string &label_shared)
{
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

void BubbleUI::Widgets::Botao::atualizar()
{
    cor = { 1, 1, 1 };
    colisao.defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y});

    if(!colisao.mouseEmCima())
    moldura.defCor({ 0.298f, 0.286f, 0.322f });
    else
    moldura.defCor({ 0.4, 0.4, 0.4});

    moldura.defPos({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    moldura.defTam({ largura_texto + letra_padding.x * 2, static_cast<int>(box_size.y)});
    moldura.atualizar();
    Texto::atualizar();
}

void BubbleUI::Widgets::Botao::renderizar() const
{
    moldura.renderizar();
    Texto::renderizar();

}
