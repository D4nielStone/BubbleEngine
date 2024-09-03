#include "arvore.hpp"
#include <src/tempo/delta_time.hpp>
#include "src/ui/painel/painel.hpp"

// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(std::string l, bool* retorno) : retorno(retorno)
{
    // Define a frase que será exibida na árvore, precedida por "[-]"
    frase = "[+]" + l;

    // Define a resolução padrão para o texto
    resolucao = 12;

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 5, 4 };
}

// Método para atualizar o estado do widget Arvore a cada frame
void BubbleUI::Widgets::Arvore::atualizar()
{
    // Salva o padding antigo do pai
    float padding_antigoy = painel->widget_padding.y;

    // Ajusta temporariamente o padding vertical do pai para 0
    painel->widget_padding.y = 0;

    // Se houver uma árvore pai, ajusta o padding horizontal
    if (arvore_pai)
    {
        painel->widget_pos.x = arvore_pai->box_pos.x - painel->obtRect().x + 7;
    }

    // Atualiza o texto
    renderizar_texto();

    // Restaura o padding original do pai
    painel->widget_padding.y = padding_antigoy;
    painel->widget_pos.x = 0;

    // Define o retângulo de colisão para a detecção de mouse
    colisao.defRect({ box_pos.x, box_pos.y, painel->obtRect().w, (int)box_size.y });
    // Define a posição e o tamanho da moldura
    moldura.defPos({ static_cast<int>(painel->obtRect().x + painel->widget_padding.x), static_cast<int>(box_pos.y )});
    moldura.defTam({ painel->obtRect().w - painel->widget_padding.x * 2, static_cast<int>(box_size.y) });

    // Reseta gatilho de click
    if (inputs->mouseEnter == GLFW_RELEASE)gatilho_click = true;

    // Se o mouse não estiver sobre o widget, define a cor padrão da moldura
    if (!colisao.mouseEmCima() && painel->selecionado)
    {
        moldura.defCor(cor);
        if (inputs->mouseEnter == GLFW_PRESS)
        {
            *retorno = false;
            moldura.ocultar_linhas = true;
        }
    }
    else if (painel->selecionado)
    {
        {
            // Se o mouse estiver sobre o widget, altera a cor da moldura
            moldura.defCor({ 0.2f, 0.2f, 0.2f });
            if (inputs->mouseEnter == GLFW_PRESS)
            {
                *retorno = true;
                moldura.ocultar_linhas = false;
                if (aberto && gatilho_click) { aberto = false; gatilho_click = false; }
                else if (gatilho_click) {
                    aberto = true; gatilho_click = false;
                }
            }
        }
    }

    // Atualiza a moldura
    moldura.atualizar();

    // Se a árvore estiver aberta, atualiza os filhos
    if (aberto)
    {
        if (frase[1] != '-')
        {
            frase.erase(0, 3);
            frase.insert(0, "[-]");
        }
        for (auto& filho : filhos)
        {
            filho->atualizar();
        }
    }
    else if(frase[1] != '+')
    {
        frase.erase(0, 3);
        frase.insert(0, "[+]");
    }
}

// Método para renderizar o widget Arvore na tela
void BubbleUI::Widgets::Arvore::renderizar() const
{
    // Renderiza a moldura utilizando triângulos
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
}

// Método para definir o painel associado ao widget Arvore
void BubbleUI::Widgets::Arvore::defPainel(Painel* painel)
{
    // Chama o método de base para associar o painel ao texto
    Texto::defPainel(painel);
    inputs = painel->obtCtx()->inputs;

    // Se a cor da árvore do pai estiver no valor padrão, define uma cor mais clara
    if (painel->arvore_cor.r == 0.1f)
    {
        cor = { 0.15f, 0.15f, 0.15f };
        painel->arvore_cor = { 0.15f, 0.15f, 0.15f };
    }
    else
    {
        // Caso contrário, define a cor padrão mais escura
        cor = { 0.1f, 0.1f, 0.1f };
        painel->arvore_cor = { 0.1f, 0.1f, 0.1f };
    }
}

// Método para adicionar um filho à árvore
void BubbleUI::Widgets::Arvore::adiFilho(std::shared_ptr<Arvore> filho)
{
    // Define a árvore pai do filho como a instância atual
    filho->arvore_pai = shared_from_this();

    // Associa o painel ao filho
    filho->defPainel(painel);

    // Adiciona o filho à lista de filhos
    filhos.push_back(filho);
}