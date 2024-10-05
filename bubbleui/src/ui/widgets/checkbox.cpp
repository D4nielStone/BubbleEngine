#include "checkbox.hpp"
#include "src/ui/painel/painel.hpp"
#include <src/arquivadores/imageloader.hpp>

BubbleUI::Widgets::CheckBox::CheckBox(bool* retorno, const std::string& texto, const Lado& alinhamento) 
    : retorno(retorno)
{
    Alinhamento = alinhamento;
    frase = texto;
    quebrarLinha = true;
    auto& gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    IDimagem = gerenciador.carregarTextura("assets/texturas/icons/check.png");
}

void BubbleUI::Widgets::CheckBox::atualizar()
{
    int x = painel->obterRetangulo().x + painel->posicaoWidget.x + painel->widgetPadding.x;
    int y = painel->obterRetangulo().y + painel->posicaoWidget.y + painel->widgetPadding.y;

    switch (Alinhamento)
    {
    case ESQUERDA:
        x = painel->obterRetangulo().x + painel->widgetPadding.x;  // Alinha à esquerda do painel
        break;
    case DIREITA:
        x = painel->obterRetangulo().x + painel->obterRetangulo().w - size - painel->widgetPadding.x;  // Alinha à direita do painel
        break;
    }

    moldura.defPos({ x, y });
    moldura.defTam({ size, size });
    colisao.defRect(moldura.obtRect());

    // Atualiza o texto se houver
    if (!frase.empty())
    {
        auto posicao_antiga = painel->posicaoWidget.y;
        switch (Alinhamento)
        {
        case ESQUERDA:
            painel->posicaoWidget.x += size + painel->widgetPadding.x;
            break;
        case DIREITA:
            painel->posicaoWidget.x = painel->widgetPadding.x;
            break;
        }
        Texto::atualizar();
        painel->posicaoWidget.y = posicao_antiga;
    }

    // Lógica de clique no checkbox
    if (inputs->mouseEnter == GLFW_RELEASE)
        gatilho = false;

    if (retorno)
    {
        deveRenderizar = *retorno;
        if (colisao.mouseEmCima() && !gatilho && inputs->mouseEnter == GLFW_PRESS && *retorno)
        {
            gatilho = true;
            *retorno = false;
        }
        else if (colisao.mouseEmCima() && !gatilho && inputs->mouseEnter == GLFW_PRESS && !*retorno)
        {
            gatilho = true;
            *retorno = true;
        }
    }

    moldura.atualizar();

    // Controle da posição do widget no painel
    if (quebrarLinha)
        painel->posicaoWidget.y += size + painel->widgetPadding.x * 2;
    else
        painel->posicaoWidget.x += size + painel->widgetPadding.x;
}

void BubbleUI::Widgets::CheckBox::renderizar() const
{
    moldura.renderizar();
    if (!frase.empty())
        Texto::renderizar();
    if (!deveRenderizar)
        return;
    renderizarImg();

}

void BubbleUI::Widgets::CheckBox::renderizarImg() const
{
    shaderImg.use();
    shaderImg.setVec2("quadrado.posicao", moldura.obtRectNDC().x, moldura.obtRectNDC().y);
    shaderImg.setVec2("quadrado.tamanho", moldura.obtRectNDC().z, moldura.obtRectNDC().w);
    shaderImg.setInt("textura", 0);
    shaderImg.setBool("flip", false);

    glBindTexture(GL_TEXTURE_2D, IDimagem);
    glBindVertexArray(rect_vertex.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}