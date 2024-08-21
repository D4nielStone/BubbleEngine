#include "texto.hpp"
#include "src/arquivadores/textura.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

BubbleUI::Widgets::Texto::Texto(std::string* label) : label(label), texto(""), letra_padding({ 5, 4 })
{
    lines_box_limite = 3;
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string* label, unsigned int pxl) : label(label), resolucao(pxl)
{
    configurar(resolucao);
}
BubbleUI::Widgets::Texto::Texto(std::string l) : resolucao(16)
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string l, unsigned int pxl) : resolucao(pxl)
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar(resolucao);
}

void BubbleUI::Widgets::Texto::atualizar(float deltaTime)
{
    if(label)
    frase = *label;
    // posiciona a moldura no widgetpos
    box_pos.x = pai->widget_pos.x;
    box_pos.y = pai->widget_pos.y;
    box_size.x = pai->obtRect().w - pai->widget_padding.x * 2;
}

void BubbleUI::Widgets::Texto::renderizar()
{
    renderizar_texto();
}

void BubbleUI::Widgets::Texto::renderizar_texto()
{
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;

    Bubble::Arquivadores::Character ch;

    for (char c : frase) {
        ch = (*Bubble::Arquivadores::obterCaracteres())[c];

        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = y_letter = (box_pos.y + resolucao - ch.Bearing.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (x_letter + w_letter + letra_padding.x >= box_size.x || c == '\n')
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

        // renderiza letra
        Vector4f char_rectf = paraNDC();

        shader.use();
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b);
        shader.setVec2("quadrado.posicao", char_rectf.x, char_rectf.y);
        shader.setVec2("quadrado.tamanho", char_rectf.z, char_rectf.w);
        shader.setInt("textura", 0);

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindVertexArray(rect_vertex.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        w_line += (ch.Advance >> 6); // 1/64 pixels
    }
            box_size.y = line_pos.y + resolucao + letra_padding.y;
}

// Deve transformar coordenadas pixel para NDC
Vector4f BubbleUI::Widgets::Texto::paraNDC()
{
    Vector4f coord_ndc;

    coord_ndc.z = (char_rect.w * 2.f) / pai->obtCtx()->tamanho.width;
    coord_ndc.w = -(2.0f * char_rect.h) / pai->obtCtx()->tamanho.height;
    coord_ndc.x = (char_rect.x * 2.f) / pai->obtCtx()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * char_rect.y) / pai->obtCtx()->tamanho.height;

    return coord_ndc;
}

void BubbleUI::Widgets::Texto::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path);
    definirResolucao(resolucao);
}
// Destrutor para liberar recursos
BubbleUI::Widgets::Texto::~Texto() 
{
}
