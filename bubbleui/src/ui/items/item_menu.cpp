#include "item_menu.hpp"
#include "src/depuracao/debug.hpp"
#include <src/tempo/delta_time.hpp>

using namespace Bubble::Arquivadores;

BubbleUI::Items::ItemMenu::ItemMenu(std::string* label) : label(label), texto(""), letra_padding({ 5, 4 })
{
    lines_box_limite = 3;
    configurar();
}

BubbleUI::Items::ItemMenu::ItemMenu(std::string* label, unsigned int pxl) : label(label), resolucao(pxl)
{
    configurar(resolucao);
}
BubbleUI::Items::ItemMenu::ItemMenu(std::string l) : resolucao(16)
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar();
}

BubbleUI::Items::ItemMenu::ItemMenu(std::string l, unsigned int pxl) : resolucao(pxl)
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar(resolucao);
}

void BubbleUI::Items::ItemMenu::atualizar()
{
    renderizar_texto();
    altura = box_pos.y + box_size.y - pai->obtRect().y;
    if (label)
        frase = *label;
    clicado = false;
    if (!colisao->mouseEmCima())
    {
        mouseEmCima = true;
        moldura.defCor({ 0.298f, 0.286f, 0.322f });
    }
    else
    {
        if (pai->obtCtx()->inputs->mouseEnter == GLFW_PRESS && pai->obtCtx()->inputs->mouseButton == GLFW_MOUSE_BUTTON_LEFT)
            clicado = true;
        mouseEmCima = false;
        moldura.defCor({ 0.4, 0.4, 0.4 });
    }
    moldura.defPos({ box_pos.x, box_pos.y});
    moldura.defTam({ (float)largura_texto + letra_padding.x * 2, (float)box_size.y });
    moldura.atualizar(Bubble::Tempo::delta_time);
}

void BubbleUI::Items::ItemMenu::renderizar()
{
    moldura.renderizar(GL_TRIANGLES);
    colisao->defRect({ box_pos.x, box_pos.y, (int)largura_texto + letra_padding.x * 2, (int)box_size.y });
    for (auto& rect : letras_rect)
    {
        shader.use();
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b);
        shader.setVec2("quadrado.posicao", rect.rect.x, rect.rect.y);
        shader.setVec2("quadrado.tamanho", rect.rect.z, rect.rect.w);
        shader.setInt("textura", 0);

        glBindTexture(GL_TEXTURE_2D, rect.ID);
        glBindVertexArray(rect_vertex.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void BubbleUI::Items::ItemMenu::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x = pai->obtRect().x + pai->widget_padding.x + pai->widget_pos.x;
    box_pos.y = pai->obtRect().y + pai->widget_padding.y + pai->widget_pos.y;
    box_size.x = pai->obtRect().w - pai->widget_padding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // Dimensões da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear();

    Bubble::Arquivadores::Character ch;

    for (char c : frase) {
        ch = (*Bubble::Arquivadores::obterCaracteres())[c];

        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + 10 - ch.Bearing.y + letra_padding.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y;
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Defini retângulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // Renderiza letra
        letras_rect.push_back({ paraNDC(), ch.TextureID });

        w_line += (ch.Advance >> 6); // 1/64 pixels
        if (w_line > largura_texto)
            largura_texto = w_line;
    }
    // Próximo widget
    box_size.y = line_pos.y + 12 + letra_padding.y * 2;  // Altura do texto mais padding
    pai->widget_pos.y = box_pos.y + box_size.y - pai->obtRect().y;

    // largura para pop_up
    largura = largura_texto + letra_padding.x * 2 + pai->widget_padding.x * 2;
}


// Deve transformar coordenadas pixel para NDC
Vector4f BubbleUI::Items::ItemMenu::paraNDC()
{
    Vector4f coord_ndc;

    coord_ndc.z = (char_rect.w * 2.f) / pai->obtCtx()->tamanho.width;
    coord_ndc.w = -(2.0f * char_rect.h) / pai->obtCtx()->tamanho.height;
    coord_ndc.x = (char_rect.x * 2.f) / pai->obtCtx()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * char_rect.y) / pai->obtCtx()->tamanho.height;

    return coord_ndc;
}

void BubbleUI::Items::ItemMenu::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path);
    definirResolucao(resolucao);
}
void BubbleUI::Items::ItemMenu::defMoldura(Formas::Moldura* m)
{
    pai = m;
    moldura = Formas::Moldura(m->obtCtx());
    colisao = new Colisao2d({},m->obtCtx());
}
// Destrutor para liberar recursos
BubbleUI::Items::ItemMenu::~ItemMenu()
{
}