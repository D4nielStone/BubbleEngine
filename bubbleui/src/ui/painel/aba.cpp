#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Painel *painel)
{
    frase = painel->nome();
    this->painel = painel;
    resolucao = 14;
    letra_padding = {4, 4};
    configurar();
    Texto::Texto(frase);
    corpo_rect = std::make_unique<Formas::Rect>(painel->obtCtx(), Vector4{});
}

void BubbleUI::Aba::atualizar()
{
    painel->widget_pos = { 0, -letra_padding.y*2.f };

    renderizar_texto();

    painel->widget_pos = { 0, static_cast<float>(corpo_rect->obtRect().h) };

    corpo_rect->defPos({static_cast<int>(painel->obtRect().x + 1), static_cast<int>(painel->obtRect().y)});
    corpo_rect->defTam({ painel->obtRect().w - 2, 14});
    box_pos = {corpo_rect->obtRect().x, corpo_rect->obtRect().y};
    box_size = { static_cast<float>(corpo_rect->obtRect().w), static_cast<float>(corpo_rect->obtRect().h) };
    corpo_rect->atualizar();
}

void BubbleUI::Aba::renderizar() const
{
    corpo_rect->renderizar();
    Texto::renderizar();
}

BubbleUI::Formas::Rect* BubbleUI::Aba::obterCorpo() const
{
    return corpo_rect.get();
}
