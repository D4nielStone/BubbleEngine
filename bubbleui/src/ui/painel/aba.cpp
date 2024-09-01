#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Painel* painel) : painel(painel)
{
    frase = painel->nome();
    pai = painel;
    resolucao = 14;
    letra_padding = {4, 4};
    configurar();
    corpo_rect = std::make_unique<Formas::Rect>(painel->obtCtx());
}

void BubbleUI::Aba::atualizar()
{
    Texto::atualizar();

    painel->widget_pos = { 0, -(float)pai->widget_padding.y - letra_padding.y };

    renderizar_texto();

    painel->widget_pos = { 0, (float)corpo_rect->obtRect().h };

    corpo_rect->defPos({painel->obtRect().x + 1, painel->obtRect().y});
    corpo_rect->defTam({ (float)painel->obtRect().w - 2, 14});
    box_pos = {corpo_rect->obtRect().x, corpo_rect->obtRect().y};
    box_size = { (float)corpo_rect->obtRect().w, (float)corpo_rect->obtRect().h };
    corpo_rect->atualizar();
}

void BubbleUI::Aba::renderizar()
{
    corpo_rect->renderizar(GL_TRIANGLES);
    Texto::renderizar();
}

BubbleUI::Formas::Rect* BubbleUI::Aba::obterCorpo() const
{
    return corpo_rect.get();
}
