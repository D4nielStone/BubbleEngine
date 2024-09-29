#include "imagem.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/arquivadores/imageloader.hpp"

BubbleUI::Widgets::Imagem::Imagem(unsigned int id, const Vector2 &size, const bool &auto_resize) : ID(id), rect({ 0, 0, size.x, size.y }), preencher(auto_resize)
{
}

BubbleUI::Widgets::Imagem::Imagem(const std::string& path, int size_percentage, Vector2 *posicao) : posicao_ptr(posicao)
{
    auto &gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    ID = gerenciador.carregarTextura(path, &rect.w, &rect.h);
    rect.w *= static_cast<int>(static_cast<float>(size_percentage) / 100);
    rect.h *= static_cast<int>(static_cast<float>(size_percentage) / 100);
}

BubbleUI::Widgets::Imagem::Imagem(const std::string& path, const Vector2& size)
{
    auto& gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    ID = gerenciador.carregarTextura(path, &rect.w, &rect.h);
    rect.w = size.x;
    rect.h = size.y;
}


// Deve transformar coordenadas pixel para NDC
Vector4f BubbleUI::Widgets::Imagem::paraNDC() const
{
    Vector4f coord_ndc;

    coord_ndc.z = (rect.w * 2.f) / contexto->tamanho.width;
    coord_ndc.w = -(2.0f * rect.h) / contexto->tamanho.height;
    coord_ndc.x = (rect.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * rect.y) / contexto->tamanho.height;

    return coord_ndc;
}


void BubbleUI::Widgets::Imagem::atualizar()
{
    if (preencher)
    {
        rect.w = painel->obterRetangulo().w;
        rect.h = painel->obterRetangulo().h - painel->posicaoWidget.y;
    }
    rect = { painel->obterRetangulo().x + painel->posicaoWidget.x, painel->obterRetangulo().y + painel->posicaoWidget.y, rect.w, rect.h};
    if (padding)
    {
        rect.x += painel->widgetPadding.x;
        rect.y += painel->widgetPadding.y;
    }
    if (posicao_ptr)
    {
        rect.x += posicao_ptr->x + rect.w / static_cast<float>(2);
        rect.y += posicao_ptr->y + rect.h / static_cast<float>(2);
    }

    if (quebrarLinha)
        painel->posicaoWidget.y += rect.y + painel->widgetPadding.x * 2;
    else
        painel->posicaoWidget.x += rect.w + painel->widgetPadding.x;
}

// Atualiza o retângulo do corpo_do_widget para a imagem
void BubbleUI::Widgets::Imagem::renderizar() const
{
    if (!deveRenderizar)
        return;
    Vector4f rectf = paraNDC();
    shader.use();
    shader.setVec2("quadrado.posicao", rectf.x, rectf.y);
    shader.setVec2("quadrado.tamanho", rectf.z, rectf.w);
    shader.setInt("textura", 0);
    shader.setBool("flip", flip);

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
