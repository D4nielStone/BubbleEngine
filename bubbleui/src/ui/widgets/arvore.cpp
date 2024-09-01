#include "arvore.hpp"
#include <src/tempo/delta_time.hpp>
#include "src/ui/painel/painel.hpp"

// Construtor da classe Arvore, inicializa os membros da classe.
BubbleUI::Widgets::Arvore::Arvore(std::string l, bool* retorno) : retorno(retorno)
{
    // Define a frase que será exibida na árvore, precedida por "[-]"
    frase = "[-]" + l;

    // Define a resolução padrão para o texto
    resolucao = 12;

    // Configura os parâmetros iniciais do widget
    configurar();

    // Define o padding (espaçamento) ao redor do texto
    letra_padding = { 5, 4 };
}

// Método para atualizar o estado do widget Arvore a cada frame
void BubbleUI::Widgets::Arvore::atualizar()
{
    // Salva o padding antigo do pai
    float padding_antigoy = pai->widget_padding.y;

    // Ajusta temporariamente o padding vertical do pai para 0
    pai->widget_padding.y = 0;

    // Se houver uma árvore pai, ajusta o padding horizontal
    if (arvore_pai)
    {
        pai->widget_pos.x = arvore_pai->box_pos.x - pai->obtRect().x + 7;
    }

    // Atualiza o texto
    Texto::atualizar();

    // Restaura o padding original do pai
    pai->widget_padding.y = padding_antigoy;
    pai->widget_pos.x = 0;

    // Define o retângulo de colisão para a detecção de mouse
    colisao->defRect({ box_pos.x, box_pos.y, pai->obtRect().w, (int)box_size.y });

    // Se o mouse não estiver sobre o widget, define a cor padrão da moldura
        if (!colisao->mouseEmCima())
        {
            moldura.defCor(cor);
            if (pai->obtCtx()->inputs->mouseEnter == GLFW_PRESS)
            {
                *retorno = false;
                moldura.ocultar_linhas = true;
            }
        }
        else if (pai->selecionado)
        {
            {
                // Se o mouse estiver sobre o widget, altera a cor da moldura
                moldura.defCor({ 0.2f, 0.2f, 0.2f });
                if (pai->obtCtx()->inputs->mouseEnter == GLFW_PRESS)
                {
                    *retorno = true;
                    moldura.ocultar_linhas = false;
                    if (aberto) aberto = false;
                    else aberto = true;
                }
            }
        }
    // Define a posição e o tamanho da moldura
    moldura.defPos({ pai->obtRect().x + pai->widget_padding.x, box_pos.y });
    moldura.defTam({ (float)pai->obtRect().w - pai->widget_padding.x * 2, box_size.y });

    // Atualiza a moldura
    moldura.atualizar();

    // Se a árvore estiver aberta, atualiza os filhos
    if (aberto)
    {
        for (auto& filho : filhos)
        {
            filho->atualizar();
        }
    }
}

// Método para renderizar o widget Arvore na tela
void BubbleUI::Widgets::Arvore::renderizar()
{
    // Renderiza a moldura utilizando triângulos
    moldura.renderizar(GL_TRIANGLES);

    // Renderiza o texto da árvore
    Texto::renderizar();

    // Se a árvore estiver aberta, renderiza os filhos
    if (aberto)
    {
        for (auto& filho : filhos)
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

    // Se a cor da árvore do pai estiver no valor padrão, define uma cor mais clara
    if (pai->arvore_cor.r == 0.1f)
    {
        cor = { 0.15f, 0.15f, 0.15f };
        pai->arvore_cor = { 0.15f, 0.15f, 0.15f };
    }
    else
    {
        // Caso contrário, define a cor padrão mais escura
        cor = { 0.1f, 0.1f, 0.1f };
        pai->arvore_cor = { 0.1f, 0.1f, 0.1f };
    }
}

// Método para adicionar um filho à árvore
void BubbleUI::Widgets::Arvore::adiFilho(std::shared_ptr<Arvore> filho)
{
    // Define a árvore pai do filho como a instância atual
    filho->arvore_pai = this;

    // Associa o painel ao filho
    filho->defPainel(pai);

    // Adiciona o filho à lista de filhos
    filhos.push_back(filho);
}