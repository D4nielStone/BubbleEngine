#include "botao.hpp"
#include <src/tempo/delta_time.hpp>
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Botao::Botao(std::string label)
{
    frase = label;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

void BubbleUI::Widgets::Botao::atualizar()
{
    Texto::atualizar();
}

void BubbleUI::Widgets::Botao::renderizar()
{
    cor = { 1, 1, 1 };
    moldura.renderizar(GL_TRIANGLES);
    renderizar_texto();
    colisao->defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y});
    if(!colisao->mouseEmCima())
    moldura.defCor({ 0.298f, 0.286f, 0.322f });
    else
    moldura.defCor({ 0.4, 0.4, 0.4});
    moldura.defPos(box_pos);
    moldura.defTam({ (float)largura_texto + letra_padding.x * 2, box_size.y});
    moldura.atualizar();
}
