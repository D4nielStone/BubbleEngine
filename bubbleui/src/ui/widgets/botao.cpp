#include "botao.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Botao::Botao(const std::string& label_shared, std::function<void()>funcao_click, bool completo)
    : completo(completo)
,   funcao_click_(funcao_click)
{
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

BubbleUI::Widgets::Botao::Botao(const std::string& label_shared, const std::string& image_path, bool* callback, bool completo)
    : completo(completo), callback(callback)
{
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    icon = std::make_unique<Imagem>(image_path, Vector2{60, 60});
    letra_padding = { 5, 5 };
}

BubbleUI::Widgets::Botao::
Botao(const std::string &label_shared, std::function<void()> function, const std::string& imagePath, bool completo) : completo(completo)
,   funcao_click_(function)
{
    icon = std::make_unique<Imagem>(imagePath, Vector2{ 20, 20 });
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

void BubbleUI::Widgets::Botao::atualizar()
{
    if (icon)
    {
        icon->defPainel(painel);
        icon->atualizar();
    }
    Texto::atualizar();
    if (icon) box_size.y = icon->obtRect().h;
    painel->posicaoWidget = { 0,  (int)(box_pos.y + box_size.y - painel->obterRetangulo().y) };
    moldura.defPos({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    // largura se completo ou não
    if (completo)
    {
        if(icon)
        moldura.defTam({ painel->obterRetangulo().w - painel->widgetPadding.x * 2 - icon->obtRect().w, static_cast<int>(box_size.y)});
        else
        moldura.defTam({ painel->obterRetangulo().w - painel->widgetPadding.x * 2, static_cast<int>(box_size.y) });
        colisao.defRect({ box_pos.x, box_pos.y, painel->obterRetangulo().w - painel->widgetPadding.x * 2, (int)box_size.y});
    }
    else
    {
        moldura.defTam({ largura_texto + letra_padding.x * 2, static_cast<int>(box_size.y) });
        colisao.defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y });
    }
    moldura.atualizar();
    //reseta gatilho
    moldura.ocultar_linhas = true;
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho = true;
    //calcula click
    if (colisao.mouseEmCima() && inputs->mouseEnter == GLFW_PRESS && gatilho)
    {
        moldura.ocultar_linhas = false;
        if (funcao_click_)
        {
            funcao_click_();
            gatilho = false;
        }
        if (callback)*callback = true;
    }
    else if(colisao.mouseEmCima() && inputs->mouseEnter == GLFW_PRESS)
    {
        moldura.ocultar_linhas = false;
    }
    //calcula cor
    if(colisao.mouseEmCima())
        moldura.defCor({ 0.4, 0.03f, 0.4, 1});
    else
        moldura.defCor({ 0.25f, 0.25f, 0.25f, 1 });
}

void BubbleUI::Widgets::Botao::renderizar() const
{
    moldura.renderizar();
    Texto::renderizar();

    if (icon)
        icon->renderizar();
}
