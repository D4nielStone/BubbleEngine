#include "botao.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

Botao::Botao(const std::string& label_shared, std::function<void()>funcao_click, bool completo)
    : completo(completo),
   funcao_click_(funcao_click)
{
    espessuraBorda =3;
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

Botao::Botao(const std::string& label_shared, const std::string& image_path, bool* callback, bool completo)
    : completo(completo), callback(callback)
{
    quebrarLinha = true;
    espessuraBorda = 3;
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    icon = std::make_unique<Imagem>(image_path, Vector2{60, 60});
    letra_padding = { 5, 5 };
}

Botao::Botao(const std::string& label_shared, bool* callback, bool completo)
    : completo(completo), callback(callback)
{
    quebrarLinha = true;
    espessuraBorda = 3;
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = { 5, 5 };
}

Botao::
Botao(const std::string &label_shared, std::function<void()> function, const std::string& imagePath, bool completo) : completo(completo)
,   funcao_click_(function)
{
    espessuraBorda = 3;
    quebrarLinha = true;
    icon = std::make_unique<Imagem>(imagePath, Vector2{ 20, 20 });
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    configurar();
    letra_padding = {5, 5};
}

void Botao::atualizar()
{
    if (icon)
    {
        icon->definirPai(painel);
        icon->atualizar();
    }
    Texto::atualizar();
    if (icon) box_size.y = icon->obtRect().h;
    if (quebrarLinha)painel->posicaoWidget = { (int)painel->obterRetangulo().x, (int)(box_pos.y + box_size.y) };
    else { painel->posicaoWidget.x = letra_padding.x + box_pos.x + largura_texto + painel->widgetPadding.x; };
   Moldura::definirPosicao({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    // largura se completo ou não
    if (completo)
    {
        if(icon)
       Moldura::definirTamanho({ painel->obterRetangulo().w - painel->widgetPadding.x * 4 - icon->obtRect().w, static_cast<int>(box_size.y)});
        else
       Moldura::definirTamanho({ painel->obterRetangulo().w - painel->widgetPadding.x * 2, static_cast<int>(box_size.y) });
        colisao.defRect({ box_pos.x, box_pos.y, painel->obterRetangulo().w - painel->widgetPadding.x * 2, (int)box_size.y});
    }
    else
    {
       Moldura::definirTamanho({ largura_texto + letra_padding.x * 2, static_cast<int>(box_size.y) });
        colisao.defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y });
    }
   Moldura::atualizar();
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho = true;
    //calcula click
    if (colisao.mouseEmCima() && inputs->mouseEnter == GLFW_PRESS && gatilho)
    {
        if (funcao_click_)
        {
            funcao_click_();
            gatilho = false;
        }
        if (callback)*callback = true;
       Moldura::defCor({ 0.39f, 0.32f, 0.46f, 1.f});
    }else
       Moldura::defCor({ 0.17f, 0.14f, 0.2f, 1.f });
}

void Botao::renderizar() const
{
   Moldura::renderizar();
    Texto::renderizar();

    if (icon)
        icon->renderizar();
}
