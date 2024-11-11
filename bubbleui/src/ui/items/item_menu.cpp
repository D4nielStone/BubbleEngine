
// Copyright (c) 2024 Daniel Oliveira

#include "item_menu.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

// Construtor que inicializa o ItemMenu com uma label_shared e configurações padrão
BubbleUI::Items::ItemMenu::ItemMenu(std::string* label_shared) : resolucao(12), label_shared(label_shared), texto(""), letra_padding({ 4, 4 })
{
    definirFonte(); // Configura o item com as configurações padrão
}

// Construtor que inicializa o ItemMenu com uma label_shared passada como string
BubbleUI::Items::ItemMenu::ItemMenu(const std::string &l) : resolucao(12), frase(l), letra_padding({5, 4})
{
    definirFonte(); // Configura o item com as configurações padrão
}

// Método para atualizar o estado do ItemMenu, incluindo a lógica de interação
void BubbleUI::Items::ItemMenu::atualizar()
{
    if (label_shared)  frase = *label_shared; // Atualiza a frase com o valor da label_shared
    renderizar_texto(); // Renderiza o texto do item
    
    clicado = false; // Inicializa o estado de clique como falso

    // Verifica se o mouse está sobre o item
    colisao.defRect({ box_pos.x, box_pos.y, largura_texto + letra_padding.x * 2, box_size.y }); // Define o retângulo de colisão
    if (!colisao.mouseEmCima())
    {
        mouseEmCima = false; // Marca que o mouse não está sobre o item
        Moldura::defCor({ 0.298f, 0.286f, 0.322f }); // Define a cor da moldura
    }
    else
    {
        // Verifica se o botão esquerdo do mouse foi pressionado
        if (inputs->mouseEnter == GLFW_PRESS && inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)  clicado = true; // Marca que o item foi clicado
        mouseEmCima = true; // Marca que o mouse está mais sobre o item
        Moldura::defCor({ 0.4F, 0.4F, 0.4F }); // Define uma cor diferente para a moldura
    }
    // Verifica gatilho para toque
    if (inputs->mouseEnter == GLFW_RELEASE)
        gatilho = true;

    // Atualiza a posição e tamanho da moldura
    Moldura::definirPosicao({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    Moldura::definirTamanho({ largura_texto + letra_padding.x * 2, box_size.y });
    Moldura::atualizar(); // Atualiza a moldura
}

// Método para renderizar o ItemMenu na tela
void BubbleUI::Items::ItemMenu::renderizar() const
{
    Moldura::renderizar(); // Renderiza a moldura

    // Renderiza cada letra do texto
    for (auto& rect : letras_rect)
    {
        shader.use(); // Usa o shader para renderização
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b); // Define a cor do quadrado/letra
        shader.setVec2("quadrado.posicao", rect.rect.x, rect.rect.y); // Define a posição da letra
        shader.setVec2("quadrado.tamanho", rect.rect.z, rect.rect.w); // Define o tamanho da letra
        shader.setInt("textura", 0); // Define a textura para o shader

        glBindTexture(GL_TEXTURE_2D, rect.ID); // Associa a textura da letra
        glBindVertexArray(rect_vertex.VAO); // Associa o VAO
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0); // Desenha a letra como triângulos
        glBindVertexArray(0); // Desassocia o VAO
        glBindTexture(GL_TEXTURE_2D, 0); // Desassocia a textura
    }
}

// Método para renderizar o texto dentro do ItemMenu
void BubbleUI::Items::ItemMenu::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x =  pai->obterRetangulo().x + pai->widgetPadding.x + pai->posicaoWidget.x;
    box_pos.y =  pai->obterRetangulo().y + pai->widgetPadding.y + pai->posicaoWidget.y;
    box_size.x = pai->obterRetangulo().w - pai->widgetPadding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // Variáveis para as dimensões da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear(); // Limpa a lista de retângulos das letras

    auto it = Bubble::Arquivadores::GerenciadorDeFontes::obterInstancia().obterCaracteres(nome_da_fonte);
    if (!it)return;
    // Itera sobre cada caractere na frase
    for (char& c : frase) {
        Bubble::Arquivadores::Character ch = it->at(c); // Obtém o caractere atual

        // Define as dimensões e posição da letra
        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + 10 - ch.Bearing.y + letra_padding.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y; // Ajusta a posição vertical
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Define o retângulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // Adiciona o retângulo da letra para renderização
        letras_rect.push_back({ paraNDC(), ch.TextureID });

        w_line += (ch.Advance >> 6); // Incrementa a largura da linha com o avanço do caractere
        if (w_line > largura_texto)
            largura_texto = w_line; // Atualiza a largura do texto
    }
    // Atualiza o tamanho do box para o próximo widget
    box_size.y = line_pos.y + 12 + letra_padding.y * 2;  // Altura do texto mais padding
    altura = box_size.y + pai->widgetPadding.y * 2;
    // Atualiza a largura para pop_up
    largura = largura_texto + letra_padding.x * 2 + pai->widgetPadding.x * 2;
    if(!vquebrarLinha)
        pai->posicaoWidget.x += largura;
    else
        pai->posicaoWidget.y = box_pos.y + box_size.y - pai->obterRetangulo().y;

}

// Método para transformar coordenadas de pixel para NDC (Normalized Device Coordinates)
Vector4f BubbleUI::Items::ItemMenu::paraNDC()
{
    Vector4f coord_ndc;

    // Calcula as coordenadas normalizadas
    coord_ndc.z = (char_rect.w * 2.f) / contexto->tamanho.width;
    coord_ndc.w = -(2.0f * char_rect.h) / contexto->tamanho.height;
    coord_ndc.x = (char_rect.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * char_rect.y) / contexto->tamanho.height;

    return coord_ndc;
}

// Configura o texto com a fonte e resolução especificada
void BubbleUI::Items::ItemMenu::definirFonte(unsigned int resolucao, std::string font_path)
{
    nome_da_fonte = font_path;
    Bubble::Arquivadores::GerenciadorDeFontes::obterInstancia().carregarFonte(font_path, resolucao); // Carrega a fonte
}

// Método para definir a moldura do ItemMenu
void BubbleUI::Items::ItemMenu::definirPai(Formas::Moldura* m)
{
    definirFonte();
    pai = m;
    contexto = m->obterContexto();
    Formas::Moldura::configurar(contexto, {}); // Define a moldura com base no contexto do pai
    colisao = Colisao2d({}, contexto); // Cria uma nova instância de colisão 2D
    inputs = contexto->inputs;
}

void BubbleUI::Items::ItemMenu::definirEscondido(bool boleano)
{
    escondido = boleano;
}

void BubbleUI::Items::ItemMenu::quebrarLinha(bool boleano)
{
    vquebrarLinha = boleano;
}

// Destrutor do ItemMenu que libera a memória alocada
BubbleUI::Items::ItemMenu::~ItemMenu() = default;
