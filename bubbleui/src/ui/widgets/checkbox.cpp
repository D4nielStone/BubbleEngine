#include "checkbox.hpp"
#include "src/ui/painel/painel.hpp"
#include <src/arquivadores/imageloader.hpp>

BubbleUI::Widgets::CheckBox::CheckBox(bool *retorno) : retorno(retorno)
{
    auto& gerenciador = Bubble::Arquivadores::TextureLoader::getInstance();
    IDimagem = gerenciador.carregarTextura("assets/texturas/icons/check.png");
}

void BubbleUI::Widgets::CheckBox::atualizar()
{
    moldura.defPos({ (int)painel->obterRetangulo().x + painel->posicaoWidget.x + painel->widgetPadding.x, (int)painel->obterRetangulo().y + painel->posicaoWidget.y + painel->widgetPadding .y});
    moldura.defTam({ size, size });
    colisao.defRect(moldura.obtRect());

    if (inputs->mouseEnter == GLFW_RELEASE)
        gatilho = false;

    if (retorno)
    {
        deveRenderizar = *retorno;
        if (colisao.mouseEmCima() && !gatilho && inputs->mouseEnter == GLFW_PRESS && *retorno)
        {
            gatilho = true;
            if (retorno)
                *retorno = false;
        }
        else if (colisao.mouseEmCima() && !gatilho && inputs->mouseEnter == GLFW_PRESS && !*retorno)
        {
            gatilho = true;
            if (retorno)
                *retorno = true;
        }
    }
    moldura.atualizar();
    painel->posicaoWidget.x += size + painel->widgetPadding.x;
}

void BubbleUI::Widgets::CheckBox::renderizar() const
{
    moldura.renderizar();
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