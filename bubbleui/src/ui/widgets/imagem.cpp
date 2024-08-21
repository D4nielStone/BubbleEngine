#include "imagem.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Imagem::Imagem(unsigned int id, Vector2 size, bool auto_resize) : ID(id), rect({ 0, 0, size.x, size.y }), preencher(auto_resize)
{
}


// Deve transformar coordenadas pixel para NDC
Vector4f BubbleUI::Widgets::Imagem::paraNDC()
{
    Vector4f coord_ndc;

    coord_ndc.z = (rect.w * 2.f) / pai->obtCtx()->tamanho.width;
    coord_ndc.w = -(2.0f * rect.h) / pai->obtCtx()->tamanho.height;
    coord_ndc.x = (rect.x * 2.f) / pai->obtCtx()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * rect.y) / pai->obtCtx()->tamanho.height;

    return coord_ndc;
}


void BubbleUI::Widgets::Imagem::atualizar(float deltaTime)
{
    if (preencher)
    {
        rect.w = pai->obtRect().w - 1;
        rect.h = pai->obtRect().h - 2;
    }
    rect = { pai->widget_pos.x, pai->widget_pos.y, rect.w , (int)(rect.h - (pai->widget_pos.y - pai->obtRect().y)) };
}

// Atualiza o retângulo do corpo_do_widget para a imagem
void BubbleUI::Widgets::Imagem::renderizar()
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
