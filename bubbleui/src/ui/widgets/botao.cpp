
// Copyright (c) 2024 Daniel Oliveira

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
    definirFonte();
    letra_padding = {5, 5};
}

Botao::Botao(const std::string& label_shared, const std::string& image_path, bool* callback, bool completo)
    : completo(completo), callback(callback)
{
    //quebrarLinha = true;
    espessuraBorda = 3;
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 13;
    definirFonte();
    icon = std::make_unique<Imagem>(image_path, Vector2{60, 60});
    letra_padding = { 5, 5 };
}

Botao::Botao(const std::string& label_shared, bool* callback, bool completo)
    : completo(completo), callback(callback)
{
    //quebrarLinha = true;
    espessuraBorda = 3;
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    definirFonte();
    letra_padding = { 5, 5 };
}

void BubbleUI::Widgets::Botao::definirPai(Moldura* pai)
{
    Texto::definirPai(pai);
    if(icon)icon->definirPai(pai);
}

Botao::
Botao(const std::string &label_shared, std::function<void()> function, const std::string& imagePath, bool completo) : completo(completo)
,   funcao_click_(function)
{
    espessuraBorda = 3;
    quebrarLinha = true;
    icon = std::make_unique<Imagem>(imagePath, Vector2{ _Meu_iconsize, _Meu_iconsize });
    cor = { 1, 1, 1, 1 };
    frase = label_shared;
    resolucao = 12;
    definirFonte();
    letra_padding = {5, 5};
}

void Botao::atualizar()
{
    /// Verifica se o botao esta visivel
    if (posicao_antiga.y > painel->obterRetangulo().y + painel->obterRetangulo().h)
        return;

    /// Caso possua um icone, atualise-o
    if (icon)
        icon->atualizar();
    
    /// Atualiza o texto
    Texto::atualizar();

    /// Atualiza altura do botao
    if (icon) box_size.y = icon->obtRect().h;

    if (quebrarLinha)
    {
        painel->posicaoWidget = { (int)painel->obterRetangulo().x, (int)(box_pos.y + box_size.y) };
    }
    else
    {
        painel->posicaoWidget.x = box_pos.x + largura_texto + painel->widgetPadding.x*3;
    }
    
    Moldura::definirPosicao({ static_cast<int>(box_pos.x ), static_cast<int>(box_pos.y) });
    // largura se completo ou não
    if (completo)
    {
        if(icon)
       Moldura::definirTamanho({ painel->obterRetangulo().w - painel->widgetPadding.x * 2 - icon->obtRect().w, static_cast<int>(box_size.y)});
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
    if (colisao.mouseEmCima())
    {
       Moldura::defCor(ROXO_ESCURO_2);
       if (inputs->mouseEnter == GLFW_PRESS && gatilho)
       {
           if (funcao_click_)
           {
               funcao_click_();
               gatilho = false;
       Moldura::defCor(ROXO_CLARO_2);
           }else
           if (callback)
           {
               *callback = true;
               gatilho = false;
       Moldura::defCor(ROXO_CLARO_2);
           }
       }
    }else
       Moldura::defCor(ROXO_ESCURO);
}

void Botao::renderizar() const
{

    if (obterRetangulo().y > painel->obterRetangulo().y + painel->obterRetangulo().h)
        return;
   Moldura::renderizar();
    Texto::renderizar();

    if (icon)
        icon->renderizar();
}
