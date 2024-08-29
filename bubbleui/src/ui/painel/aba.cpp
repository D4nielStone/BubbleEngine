#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Painel* painel) : painel(painel)
{
    frase = painel->nome();
    pai = painel;
    resolucao = 12;
    configurar();
    corpo_rect = new Formas::Rect(painel->obtCtx());
    corpo_rect->defCor({0.4, 0, 0.4});
}

void BubbleUI::Aba::atualizar(float deltaTime)
{
    Texto::atualizar(deltaTime);

    painel->widget_pos = { 0, -(float)pai->widget_padding.y };

    renderizar_texto();

    painel->widget_pos = { 0, (float)corpo_rect->obtRect().h };

    corpo_rect->defPos({painel->obtRect().x, painel->obtRect().y});
    corpo_rect->defTam({ (float)painel->obtRect().w, 15});
    box_pos = {corpo_rect->obtRect().x, corpo_rect->obtRect().y};
    box_size = { (float)corpo_rect->obtRect().w, (float)corpo_rect->obtRect().h };
    corpo_rect->atualizar(deltaTime);
}

void BubbleUI::Aba::renderizar()
{
    corpo_rect->renderizar(GL_TRIANGLES);
    Texto::renderizar();
}