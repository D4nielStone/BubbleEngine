#include "arvore.hpp"
#include <src/tempo/delta_time.hpp>
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Arvore::Arvore(std::string l)
{
    frase = l;
    resolucao = 12;
    configurar();
    label = new std::string(l);
    letra_padding = { 5, 5 };
    cor = { 1, 1, 1 };
}

void BubbleUI::Widgets::Arvore::atualizar(float deltaTime)
{
    Texto::atualizar(Bubble::Tempo::delta_time);
    renderizar_texto();
}

void BubbleUI::Widgets::Arvore::renderizar()
{
    moldura.renderizar(GL_TRIANGLES);

    Texto::renderizar();

    colisao->defRect({ box_pos.x, box_pos.y, pai->obtRect().w, (int)box_size.y});
    if (!colisao->mouseEmCima())
        moldura.defCor({ 0.298f, 0.286f, 0.322f });
    else
        moldura.defCor({ 0.4, 0.4, 0.4 });

    moldura.defPos({ pai->obtRect().x, box_pos.y });
    moldura.defTam({ (float)largura_texto + letra_padding.x * 2 + pai->widget_padding.x*2, box_size.y });
    moldura.atualizar(Bubble::Tempo::delta_time);
}
