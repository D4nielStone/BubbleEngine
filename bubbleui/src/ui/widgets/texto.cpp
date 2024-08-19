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
        corpo_do_widget.defPos({ box_pos.x + (float)x_letter + line_pos.x, (float)y_letter + line_pos.y});
        corpo_do_widget.defTam({ (float)w_letter, (float)h_letter });

        // renderiza letra
        pai->obtCtx()->shader.use();
        pai->obtCtx()->shader.setBool("texto", true);
        pai->obtCtx()->shader.setVec3("cor_texto", cor.r, cor.g, cor.b);
        pai->obtCtx()->shader.setInt("textura", 0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        corpo_do_widget.renderizar(GL_TRIANGLES);

        w_line += (ch.Advance >> 6); // 1/64 pixels
    }
            box_size.y = line_pos.y + resolucao + letra_padding.y;
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
