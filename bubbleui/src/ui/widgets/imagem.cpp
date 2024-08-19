#include "imagem.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::Imagem::Imagem(unsigned int id, Vector2 size, bool auto_resize) : ID(id), tamanho(size), preencher(auto_resize)
{
}

void BubbleUI::Widgets::Imagem::atualizar(float deltaTime)
{
    if (preencher)
    {
        tamanho.x = pai->obtRect().w - 1;
        tamanho.y = pai->obtRect().h - 2;
    }
}

// Atualiza o retângulo do corpo_do_widget para a imagem
void BubbleUI::Widgets::Imagem::renderizar()
{
    corpo_do_widget.defPos(pai->widget_pos);
    corpo_do_widget.defTam({ (float)tamanho.x, static_cast<float>(tamanho.y - ( pai->widget_pos.y - pai->obtRect().y)) });
    pai->obtCtx()->shader.use();
    pai->obtCtx()->shader.setBool("imagem", true);
    pai->obtCtx()->shader.setInt("textura", 0);
    glBindTexture(GL_TEXTURE_2D, ID);
    corpo_do_widget.renderizar(GL_TRIANGLES);
}

void BubbleUI::Widgets::Imagem::defID(unsigned int newID)
{
    ID = newID;
}

Vector4 BubbleUI::Widgets::Imagem::obtRect() const
{
    return corpo_do_widget.obtRect();
}
