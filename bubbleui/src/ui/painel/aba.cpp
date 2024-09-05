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
    corpo_rect = std::make_unique<Formas::Rect>(painel->obterContexto(), Vector4{});
}

void BubbleUI::Aba::atualizar()
{
    painel->posicaoWidget = { 0, -letra_padding.y*2 };

    renderizar_texto();

    painel->posicaoWidget = { 0, corpo_rect->obtRect().h };

    corpo_rect->defPos({static_cast<int>(painel->obterRetangulo().x + 1), static_cast<int>(painel->obterRetangulo().y)});
    corpo_rect->defTam({ painel->obterRetangulo().w - 2, 14});
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
