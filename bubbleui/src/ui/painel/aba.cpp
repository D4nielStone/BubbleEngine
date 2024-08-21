#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Painel* painel) : painel(painel)
{
    frase = painel->nome();
    pai = painel;
    resolucao = 12;
    configurar();
    corpo_rect = new Formas::Rect(painel->obtCtx());
    corpo_rect->defCor({0.4, 0, 0.4});
}

void BubbleUI::Aba::atualizar(float deltaTime)
{
    Texto::atualizar(deltaTime);
    corpo_rect->defPos({painel->obtRect().x, painel->obtRect().y});
    corpo_rect->defTam({ (float)painel->obtRect().w, 15});
    box_pos = {corpo_rect->obtRect().x, corpo_rect->obtRect().y};
    box_size = { (float)corpo_rect->obtRect().w, (float)corpo_rect->obtRect().h };
    painel->widget_pos = { painel->obtRect().x, painel->obtRect().y + corpo_rect->obtRect().h };
    corpo_rect->atualizar(deltaTime);
}

void BubbleUI::Aba::renderizar()
{
    corpo_rect->renderizar(GL_TRIANGLES);
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
}