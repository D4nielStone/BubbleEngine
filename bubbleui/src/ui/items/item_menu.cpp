#include "item_menu.hpp"
#include "src/depuracao/debug.hpp"
#include <src/tempo/delta_time.hpp>

using namespace Bubble::Arquivadores;

// Construtor que inicializa o ItemMenu com uma label e configura��es padr�o
BubbleUI::Items::ItemMenu::ItemMenu(std::string* label) : label(label), texto(""), letra_padding({ 5, 4 })
{
    lines_box_limite = 3; // Limite de linhas na caixa
    configurar(); // Configura o item com as configura��es padr�o
}

// Construtor que inicializa o ItemMenu com uma label e uma resolu��o especificada
BubbleUI::Items::ItemMenu::ItemMenu(std::string* label, unsigned int pxl) : label(label), resolucao(pxl)
{
    configurar(resolucao); // Configura o item com a resolu��o especificada
}

// Construtor que inicializa o ItemMenu com uma label passada como string
BubbleUI::Items::ItemMenu::ItemMenu(std::string l) : resolucao(16)
{
    lines_box_limite = 3; // Limite de linhas na caixa
    label = new std::string(l); // Cria uma nova string para a label
    configurar(); // Configura o item com as configura��es padr�o
}

// Construtor que inicializa o ItemMenu com uma label passada como string e uma resolu��o especificada
BubbleUI::Items::ItemMenu::ItemMenu(std::string l, unsigned int pxl) : resolucao(pxl)
{
    lines_box_limite = 3; // Limite de linhas na caixa
    label = new std::string(l); // Cria uma nova string para a label
    configurar(resolucao); // Configura o item com a resolu��o especificada
}

// M�todo para atualizar o estado do ItemMenu, incluindo a l�gica de intera��o
void BubbleUI::Items::ItemMenu::atualizar()
{
    renderizar_texto(); // Renderiza o texto do item
    if (label)
        frase = *label; // Atualiza a frase com o valor da label
    clicado = false; // Inicializa o estado de clique como falso

    // Verifica se o mouse est� sobre o item
    if (!colisao->mouseEmCima())
    {
        mouseEmCima = true; // Marca que o mouse est� sobre o item
        moldura.defCor({ 0.298f, 0.286f, 0.322f }); // Define a cor da moldura
    }
    else
    {
        // Verifica se o bot�o esquerdo do mouse foi pressionado
        if (pai->obtCtx()->inputs->mouseEnter == GLFW_PRESS && pai->obtCtx()->inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
            clicado = true; // Marca que o item foi clicado
        mouseEmCima = false; // Marca que o mouse n�o est� mais sobre o item
        moldura.defCor({ 0.4, 0.4, 0.4 }); // Define uma cor diferente para a moldura
    }
    // Atualiza a posi��o e tamanho da moldura
    moldura.defPos({ box_pos.x, box_pos.y });
    moldura.defTam({ (float)largura_texto + letra_padding.x * 2, (float)box_size.y });
    moldura.atualizar(Bubble::Tempo::delta_time); // Atualiza a moldura com base no delta time
}

// M�todo para renderizar o ItemMenu na tela
void BubbleUI::Items::ItemMenu::renderizar()
{
    moldura.renderizar(GL_TRIANGLES); // Renderiza a moldura usando tri�ngulos
    colisao->defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y }); // Define o ret�ngulo de colis�o

    // Renderiza cada letra do texto
    for (auto& rect : letras_rect)
    {
        shader.use(); // Usa o shader para renderiza��o
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b); // Define a cor do quadrado/letra
        shader.setVec2("quadrado.posicao", rect.rect.x, rect.rect.y); // Define a posi��o da letra
        shader.setVec2("quadrado.tamanho", rect.rect.z, rect.rect.w); // Define o tamanho da letra
        shader.setInt("textura", 0); // Define a textura para o shader

        glBindTexture(GL_TEXTURE_2D, rect.ID); // Associa a textura da letra
        glBindVertexArray(rect_vertex.VAO); // Associa o VAO
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0); // Desenha a letra como tri�ngulos
        glBindVertexArray(0); // Desassocia o VAO
        glBindTexture(GL_TEXTURE_2D, 0); // Desassocia a textura
    }
}

// M�todo para renderizar o texto dentro do ItemMenu
void BubbleUI::Items::ItemMenu::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x = pai->obtRect().x + pai->widget_padding.x + pai->widget_pos.x;
    box_pos.y = pai->obtRect().y + pai->widget_padding.y + pai->widget_pos.y;
    box_size.x = pai->obtRect().w - pai->widget_padding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // Vari�veis para as dimens�es da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear(); // Limpa a lista de ret�ngulos das letras

    Bubble::Arquivadores::Character ch;

    // Itera sobre cada caractere na frase
    for (char c : frase) {
        ch = (*Bubble::Arquivadores::obterCaracteres())[c]; // Obt�m o caractere atual

        // Define as dimens�es e posi��o da letra
        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + 10 - ch.Bearing.y + letra_padding.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y; // Ajusta a posi��o vertical
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Define o ret�ngulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // Adiciona o ret�ngulo da letra para renderiza��o
        letras_rect.push_back({ paraNDC(), ch.TextureID });

        w_line += (ch.Advance >> 6); // Incrementa a largura da linha com o avan�o do caractere
        if (w_line > largura_texto)
            largura_texto = w_line; // Atualiza a largura do texto
    }
    // Atualiza o tamanho do box para o pr�ximo widget
    box_size.y = line_pos.y + 12 + letra_padding.y * 2;  // Altura do texto mais padding
    altura = box_size.y + pai->widget_padding.y * 2;
    pai->widget_pos.y = box_pos.y + box_size.y - pai->obtRect().y;

    // Atualiza a largura para pop_up
    largura = largura_texto + letra_padding.x * 2 + pai->widget_padding.x * 2;
}

// M�todo para transformar coordenadas de pixel para NDC (Normalized Device Coordinates)
Vector4f BubbleUI::Items::ItemMenu::paraNDC()
{
    Vector4f coord_ndc;

    // Calcula as coordenadas normalizadas
    coord_ndc.z = (char_rect.w * 2.f) / pai->obtCtx()->tamanho.width;
    coord_ndc.w = -(2.0f * char_rect.h) / pai->obtCtx()->tamanho.height;
    coord_ndc.x = (char_rect.x * 2.f) / pai->obtCtx()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * char_rect.y) / pai->obtCtx()->tamanho.height;

    return coord_ndc;
}

// M�todo para configurar o ItemMenu com uma resolu��o e um caminho de fonte
void BubbleUI::Items::ItemMenu::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path); // Carrega a fonte especificada
    definirResolucao(resolucao); // Define a resolu��o da fonte
}

// M�todo para definir a moldura do ItemMenu
void BubbleUI::Items::ItemMenu::defMoldura(Formas::Moldura* m)
{
    pai = m;
    moldura = Formas::Moldura(m->obtCtx()); // Define a moldura com base no contexto do pai
    colisao = new Colisao2d({}, m->obtCtx()); // Cria uma nova inst�ncia de colis�o 2D
}

// Destrutor do ItemMenu que libera a mem�ria alocada
BubbleUI::Items::ItemMenu::~ItemMenu()
{
    delete colisao;
}