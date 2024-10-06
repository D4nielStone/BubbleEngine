#include "arvore.hpp"
#include "src/ui/painel/painel.hpp"

// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(const std::string &l, bool* retorno) : retorno(retorno)
{
    frase = l;

    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 4, 4 };
}
BubbleUI::Widgets::Arvore::Arvore(std::shared_ptr<std::string> label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    icone = std::make_unique<Imagem>(imagePath, Vector2{20, 20});
    icone->padding = true;

    label_shared = label;

    frase = "";
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };
}
BubbleUI::Widgets::Arvore::Arvore(const std::string& label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    icone = std::make_unique<Imagem>(imagePath, Vector2{ 20, 20 });
    icone->padding = true;

    frase = label;
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };
}
// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(std::shared_ptr<std::string>l, bool* retorno) : retorno(retorno)
{
    label_shared = l;

    frase = "";
    // Define a resolu��o padr�o para o texto
    resolucao = 12;

    // Define o padding (espa�amento) ao redor do texto
    letra_padding = { 3, 4 };

}

// M�todo para atualizar o estado do widget Arvore a cada frame
void BubbleUI::Widgets::Arvore::atualizar()
{
    // Salva o padding antigo do pai
    float padding_antigoy = painel->widgetPadding.y;

    // Ajusta temporariamente o padding vertical do pai para 0
    painel->widgetPadding.y = 0;

    // Se houver uma �rvore pai, ajusta o padding horizontal
    if (arvore_pai)
    {
        painel->posicaoWidget.x = arvore_pai->box_pos.x - painel->obterRetangulo().x + 5;
    }

    if(icone)
    icone->atualizar();

    // Atualiza o texto baseado no ponteiro
    if (label_shared)
        if (aberto)
        {
            frase = (*label_shared);
            renderizar_texto(frase);

            if (!filhos.empty()) painel->posicaoWidget.y += painel->widgetPadding.y;
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
    else
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
    // Restaura o padding original do pai
    painel->widgetPadding.y = padding_antigoy;
    painel->posicaoWidget.x = 0;

    // Define o ret�ngulo de colis�o para a detec��o de mouse
    colisao.defRect({ box_pos.x, box_pos.y, painel->obterRetangulo().w, (int)box_size.y });
    // Define a posi��o e o tamanho da moldura
    moldura.defPos({ static_cast<int>(painel->obterRetangulo().x + painel->widgetPadding.x), static_cast<int>(box_pos.y) });
    moldura.defTam({ painel->obterRetangulo().w - painel->widgetPadding.x * 2, static_cast<int>(box_size.y) });


    // Reseta gatilho de click
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho_click = true;

    // Se a cor da �rvore do pai estiver no valor padr�o, define uma cor mais clara
    if (painel->arvoreCor.r == 0.1f)
    {
        cor = { 0.15f, 0.15f, 0.15f };
        painel->arvoreCor = { 0.15f, 0.15f, 0.15f };
    }
    else
    {
        // Caso contr�rio, define a cor padr�o mais escura
        cor = { 0.1f, 0.1f, 0.1f };
        painel->arvoreCor = { 0.1f, 0.1f, 0.1f };
    }

    moldura.defCor(cor);
    // Se o mouse n�o estiver sobre o widget, define a cor padr�o da moldura
    moldura.ocultar_linhas = true;
    if (!colisao.mouseEmCima() && painel->selecionado)
    {
        if (inputs->mouseEnter == GLFW_PRESS)
        {
            if (retorno)
                *retorno = false;
        }
    }
    else if (painel->selecionado)
    {
        {
            // Se o mouse estiver sobre o widget, altera a cor da moldura
            moldura.defCor({ 0.2f, 0.2f, 0.2f });
            if (inputs->mouseEnter == GLFW_PRESS && inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
            {
                if (retorno)    *retorno = true; // ponteiro do retorno
                moldura.ocultar_linhas = false; // n�o oculta linhas da moldura
                if (aberto && gatilho_click) { aberto = false; gatilho_click = false; }
                else if (gatilho_click && !filhos.empty()) { aberto = true; gatilho_click = false; }
            }
        }
    }

    // Atualiza a moldura
    moldura.atualizar();

}
// M�todo para renderizar o widget Arvore na tela
void BubbleUI::Widgets::Arvore::renderizar() const
{
    // Renderiza a moldura utilizando tri�ngulos
    if (moldura.obtRect().y > box_pos.y + box_size.y)return;
    moldura.renderizar();


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
void BubbleUI::Widgets::Arvore::defPainel(Painel* painel)
{
    // Chama o m�todo de base para associar o painel ao texto
    Texto::defPainel(painel);
    inputs = painel->obterContexto()->inputs;

    if (icone)
    icone->defPainel(painel);
}

// M�todo para adicionar um filho � �rvore
void BubbleUI::Widgets::Arvore::adiFilho(std::shared_ptr<Widget> filho)
{
    // Define a �rvore pai do filho como a inst�ncia atual
    filho->arvore_pai = shared_from_this();

    // Associa o painel ao filho
    filho->defPainel(painel);

    // Adiciona o filho � lista de filhos
    filhos.push_back(filho);
}