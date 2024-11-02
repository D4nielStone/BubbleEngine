#include "filtro.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Filtro::Filtro(const std::string& label)
{
}
void BubbleUI::Widgets::Filtro::atualizar()
{
    moldura.defCor({0.25f, 0.21f, 0.29f, 1.f});
    moldura.defPos({painel->widgetPadding.x, painel->posicaoWidget.y + painel->widgetPadding.y});
    moldura.defTam({static_cast<int>(painel->obterRetangulo().w - moldura.obtRect().x- painel->widgetPadding.x), static_cast<int>(painel->obterRetangulo().h - moldura.obtRect().y- painel->widgetPadding.y) });
    moldura.atualizar();
}

void BubbleUI::Widgets::Filtro::renderizar() const
{
    moldura.renderizar();
}