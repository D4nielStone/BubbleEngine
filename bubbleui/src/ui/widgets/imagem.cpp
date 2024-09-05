#include "imagem.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Imagem::Imagem(unsigned int id, const Vector2 &size, const bool &auto_resize) : ID(id), rect({ 0, 0, size.x, size.y }), preencher(auto_resize)
{
}


// Deve transformar coordenadas pixel para NDC
Vector4f BubbleUI::Widgets::Imagem::paraNDC() const
{
    Vector4f coord_ndc;

    coord_ndc.z = (rect.w * 2.f) / painel->obterContexto()->tamanho.width;
    coord_ndc.w = -(2.0f * rect.h) / painel->obterContexto()->tamanho.height;
    coord_ndc.x = (rect.x * 2.f) / painel->obterContexto()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * rect.y) / painel->obterContexto()->tamanho.height;

    return coord_ndc;
}


void BubbleUI::Widgets::Imagem::atualizar()
{
    if (preencher)
    {
        rect.w = painel->obterRetangulo().w;
        rect.h = painel->obterRetangulo().h - painel->posicaoWidget.y;
    }
    rect = { painel->obterRetangulo().x + painel->posicaoWidget.x + 1, painel->obterRetangulo().y + painel->posicaoWidget.y, rect.w - 2 , rect.h - 1 };
}

// Atualiza o retângulo do corpo_do_widget para a imagem
void BubbleUI::Widgets::Imagem::renderizar() const
{
    Vector4f rectf = paraNDC();
    shader.use();
    shader.setVec2("quadrado.posicao", rectf.x, rectf.y);
    shader.setVec2("quadrado.tamanho", rectf.z, rectf.w);
    shader.setInt("textura", 0);

    glBindTexture(GL_TEXTURE_2D, ID);
    glBindVertexArray(rect_vertex.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void BubbleUI::Widgets::Imagem::defID(unsigned int newID)
{
    ID = newID;
}

Vector4 BubbleUI::Widgets::Imagem::obtRect() const
{
    return rect;
}
