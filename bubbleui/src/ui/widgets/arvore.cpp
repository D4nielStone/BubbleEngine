
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "arvore.hpp"
#include "src/ui/painel/painel.hpp"

using namespace BubbleUI::Widgets;

// Construtor da classe Arvore, inicializa os membros da classe.
Arvore::Arvore(const std::string &l, bool* retorno) : retorno(retorno)
{
    quebrarLinha = true;
    frase = l;

    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 4, 4 };
}
Arvore::Arvore(std::shared_ptr<std::string> label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    quebrarLinha = true;
    espessuraBorda = 2;
    icone = std::make_unique<Imagem>(imagePath, Vector2{20, 20});
    icone->padding = true;

    label_shared = label;

    frase = "";
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };
}
Arvore::Arvore(const std::string& label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    icone = std::make_unique<Imagem>(imagePath, Vector2{ 20, 20 });
    quebrarLinha = true;
    icone->padding = true;

    frase = label;
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };
}
// Construtor da classe Arvore, inicializa os membros da classe.
Arvore::Arvore(std::shared_ptr<std::string>l, bool* retorno) : retorno(retorno)
{
    quebrarLinha = true;
    espessuraBorda = 2;
    label_shared = l;

    frase = "";
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };

}

// M�todo para atualizar o estado do widget Arvore a cada frame
void Arvore::atualizar()
{
    // Atualiza a moldura
    auto p = painel->widgetPadding.y;
    painel->widgetPadding.y = 0;
    // Se houver uma �rvore pai, ajusta o padding horizontal
    if (arvore_pai)   painel->posicaoWidget.x = arvore_pai->box_pos.x + 5.F;
    //
    // Se houver icone, o-atualiza
    if(icone)   icone->atualizar();
    //
    // Atualiza o texto baseado no ponteiro caso seja != nullptr
    if (label_shared)
    {
        // Se aberto
        if (aberto)
        {
            frase = (*label_shared);
            renderizar_texto(frase);
            // Atualiza os filhos
            if (!filhos.empty()) painel->posicaoWidget.y += painel->widgetPadding.y; //<<Reajuste do padding
            for (const auto& filho : filhos)
            {
                filho->atualizar();
            }
        }
        else
        {
            frase = (*label_shared);
            renderizar_texto(frase);
        }
    }
    else
    {
        if (aberto)
        {
            renderizar_texto(frase);

            for (const auto& filho : filhos)
            {
                filho->atualizar();
            }
        }
        else
        {
            renderizar_texto(frase);
        }
    }
    // Restaura o padding original do pai
    painel->posicaoWidget.x = painel->obterRetangulo().x;
    //
    // Define o ret�ngulo de colis�o para a detec��o de mouse
    colisao->definirBounds({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y), painel->obterRetangulo().w, (int)box_size.y });
    // Define a posi��o e o tamanho da moldura
    definirPosicao({ static_cast<int>(painel->obterRetangulo().x + painel->widgetPadding.x), static_cast<int>(box_pos.y) });
    definirTamanho({ painel->obterRetangulo().w - painel->widgetPadding.x * 2, static_cast<int>(box_size.y) });
    //
    //
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho_click = true; //<< Reseta gatilho de click
    
    // Se a cor da �rvore do pai estiver no valor padr�o, define uma cor mais clara
    if (painel->arvoreCor.r == 0.1f)
    {
        cor = { 0.35f, 0.31f, 0.39f, 1.f };
        painel->arvoreCor = { 0.35f, 0.31f, 0.39f, 1.f };
    }
    else
    {
        // Caso contr�rio, define a cor padr�o mais escura
        cor = ROXO_CLARO;
        painel->arvoreCor = ROXO_CLARO;
    }
    //
    Moldura::defCor(cor);
    // Se o mouse n�o estiver sobre o widget, define a cor padr�o da moldura
    if(!painel->selecionado)return;
    if (!colisao->mouseEmCima())
    {
        if (inputs->mouseEnter == GLFW_PRESS)
        {
            if (retorno)
                *retorno = false;
        }
    }
    else
    {
        {
            // Se o mouse estiver sobre o widget, altera a cor da moldura
            Moldura::defCor({ 0.45f, 0.41f, 0.49f, 1.f });
            if (inputs->mouseEnter == GLFW_PRESS && inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
            {
                if (retorno)    *retorno = true; // ponteiro do retorno
                if (aberto && gatilho_click) { aberto = false; gatilho_click = false; }
                else if (gatilho_click && !filhos.empty()) { aberto = true; gatilho_click = false; }
            }
        }
    }
    painel->widgetPadding.y = p;
    Moldura::atualizar();
}
// M�todo para renderizar o widget Arvore na tela
void Arvore::renderizar()
{
    // Renderiza a moldura utilizando tri�ngulos
    if (Moldura::obterRetangulo().y > box_pos.y + box_size.y)return;
    Moldura::renderizar();


    // Renderiza o texto da �rvore
    Texto::renderizar();

    // Se a �rvore estiver aberta, renderiza os filhos
    if (aberto)
    {
        for (const auto& filho : filhos)
        {
            filho->renderizar();
        }
    }
    if (icone)
        icone->renderizar();
}

// M�todo para definir o painel associado ao widget Arvore
void Arvore::definirPai(Formas::Moldura* painel)
{
    // Chama o m�todo de base para associar o painel ao texto
    Texto::definirPai(painel);
    definirFonte();
    inputs = painel->obterContexto()->inputs;

    if (icone)
    icone->definirPai(painel);
}

// M�todo para adicionar um filho � �rvore
void Arvore::adiFilho(std::shared_ptr<Widget> filho)
{
    // Define a �rvore pai do filho como a inst�ncia atual
    filho->arvore_pai = shared_from_this();

    // Associa o painel ao filho
    filho->definirPai(painel);

    // Adiciona o filho � lista de filhos
    filhos.push_back(filho);
}
