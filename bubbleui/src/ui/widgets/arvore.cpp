#include "arvore.hpp"
#include "src/ui/painel/painel.hpp"

// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(const std::string &l, bool* retorno) : retorno(retorno)
{
    frase = l;

    // Define a resolução padrão para o texto
    resolucao = 12;

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 4, 4 };
}
BubbleUI::Widgets::Arvore::Arvore(std::shared_ptr<std::string> label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    icone = std::make_unique<Imagem>(imagePath, Vector2{20, 20});
    icone->padding = true;

    label_shared = label;

    frase = "";
    // Define a resolução padrão para o texto
    resolucao = 12;

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 3, 4 };
}
BubbleUI::Widgets::Arvore::Arvore(const std::string& label, bool* retorno, const std::string& imagePath)
    : retorno(retorno)
{
    icone = std::make_unique<Imagem>(imagePath, Vector2{ 20, 20 });
    icone->padding = true;

    frase = label;
    // Define a resolução padrão para o texto
    resolucao = 12;

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 3, 4 };
}
// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(std::shared_ptr<std::string>l, bool* retorno) : retorno(retorno)
{
    label_shared = l;

    frase = "";
    // Define a resolução padrão para o texto
    resolucao = 12;

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 3, 4 };

}

// Método para atualizar o estado do widget Arvore a cada frame
void BubbleUI::Widgets::Arvore::atualizar()
{
    // Salva o padding antigo do pai
    float padding_antigoy = painel->widgetPadding.y;

    // Ajusta temporariamente o padding vertical do pai para 0
    painel->widgetPadding.y = 0;

    // Se houver uma árvore pai, ajusta o padding horizontal
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

    // Define o retângulo de colisão para a detecção de mouse
    colisao.defRect({ box_pos.x, box_pos.y, painel->obterRetangulo().w, (int)box_size.y });
    // Define a posição e o tamanho da moldura
    moldura.defPos({ static_cast<int>(painel->obterRetangulo().x + painel->widgetPadding.x), static_cast<int>(box_pos.y) });
    moldura.defTam({ painel->obterRetangulo().w - painel->widgetPadding.x * 2, static_cast<int>(box_size.y) });


    // Reseta gatilho de click
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho_click = true;

    // Se a cor da árvore do pai estiver no valor padrão, define uma cor mais clara
    if (painel->arvoreCor.r == 0.1f)
    {
        cor = { 0.15f, 0.15f, 0.15f };
        painel->arvoreCor = { 0.15f, 0.15f, 0.15f };
    }
    else
    {
        // Caso contrário, define a cor padrão mais escura
        cor = { 0.1f, 0.1f, 0.1f };
        painel->arvoreCor = { 0.1f, 0.1f, 0.1f };
    }

    moldura.defCor(cor);
    // Se o mouse não estiver sobre o widget, define a cor padrão da moldura
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
                moldura.ocultar_linhas = false; // não oculta linhas da moldura
                if (aberto && gatilho_click) { aberto = false; gatilho_click = false; }
                else if (gatilho_click && !filhos.empty()) { aberto = true; gatilho_click = false; }
            }
        }
    }

    // Atualiza a moldura
    moldura.atualizar();

}
// Método para renderizar o widget Arvore na tela
void BubbleUI::Widgets::Arvore::renderizar() const
{
    // Renderiza a moldura utilizando triângulos
    if (moldura.obtRect().y > box_pos.y + box_size.y)return;
    moldura.renderizar();


    // Renderiza o texto da árvore
    Texto::renderizar();

    // Se a árvore estiver aberta, renderiza os filhos
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

// Método para definir o painel associado ao widget Arvore
void BubbleUI::Widgets::Arvore::defPainel(Painel* painel)
{
    // Chama o método de base para associar o painel ao texto
    Texto::defPainel(painel);
    inputs = painel->obterContexto()->inputs;

    if (icone)
    icone->defPainel(painel);
}

// Método para adicionar um filho à árvore
void BubbleUI::Widgets::Arvore::adiFilho(std::shared_ptr<Widget> filho)
{
    // Define a árvore pai do filho como a instância atual
    filho->arvore_pai = shared_from_this();

    // Associa o painel ao filho
    filho->defPainel(painel);

    // Adiciona o filho à lista de filhos
    filhos.push_back(filho);
}