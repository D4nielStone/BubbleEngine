
// Copyright (c) 2024 Daniel Oliveira

#include "imagem.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/arquivadores/imageloader.hpp"

using namespace BubbleUI::Widgets;

Imagem::Imagem(unsigned int id, const Vector2 &size, const bool &auto_resize) : ID(id), rect({ 0, 0, size.x, size.y }), preencher(auto_resize)
{
}

Imagem::Imagem(const std::string& path, int size_percentage, Vector2* posicao) : posicao_ptr(posicao)
{
    auto& gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    ID = gerenciador.carregarTextura(path, &rect.w, &rect.h);

    // Calcula o fator de escala como um valor de ponto flutuante
    float scale_factor = static_cast<float>(size_percentage) / 100.0f;

    // Aplica o fator de escala na largura e altura antes de converter para int
    rect.w = static_cast<int>(rect.w * scale_factor);
    rect.h = static_cast<int>(rect.h * scale_factor);
}

Imagem::Imagem(const std::string& path, const Vector2& size)
{
    auto& gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    ID = gerenciador.carregarTextura(path, &rect.w, &rect.h);
    rect.w = size.x;
    rect.h = size.y;
}


// Deve transformar coordenadas pixel para NDC
Vector4f Imagem::paraNDC() const
{
    Vector4f coord_ndc;

    coord_ndc.z = (rect.w * 2.f) / contexto->tamanho.width;
    coord_ndc.w = -(2.0f * rect.h) / contexto->tamanho.height;
    coord_ndc.x = (rect.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * rect.y) / contexto->tamanho.height;

    return coord_ndc;
}


void Imagem::atualizar()
{
    if (preencher)
    {
        rect.w = painel->obterRetangulo().w - (painel->posicaoWidget.x - painel->obterRetangulo().x);
        rect.h = painel->obterRetangulo().h - (painel->posicaoWidget.y - painel->obterRetangulo().y);
    }
    rect = { (float)painel->posicaoWidget.x + painel->widgetPadding.x, (float)painel->posicaoWidget.y + painel->widgetPadding.y, rect.w, rect.h };
    
    rect.y += padding ? painel->widgetPadding.y : 0;
    switch (alinhamentoHorizontal)
    {
    case Alinhamento::Esquerda:
        rect.x += padding ? painel->widgetPadding.x : 0;
        break;
    case Alinhamento::Direita:
        rect.x += painel->obterRetangulo().w - rect.w;
        rect.x -= padding ? painel->widgetPadding.x : 0;
        break;
    case Alinhamento::Centro:
        rect.x += painel->obterRetangulo().w / 2.f - rect.w / 2.f;
    }
    if (posicao_ptr)
    {
        rect.x = posicao_ptr->x;
        rect.y = posicao_ptr->y;
    }
    if (quebrarLinha)
    {
        painel->posicaoWidget.x = painel->obterRetangulo().x;
        painel->posicaoWidget.y += rect.h + painel->widgetPadding.y*2;
    }
    else
    {
        painel->posicaoWidget.x += rect.w + painel->widgetPadding.x;
    }
}

// Atualiza o retângulo do corpo_do_widget para a imagem
void Imagem::renderizar() const
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

bool Imagem::defID(unsigned int newID)
{
    return ID = newID;
}

Vector4 Imagem::obtRect() const
{
    return rect;
}
