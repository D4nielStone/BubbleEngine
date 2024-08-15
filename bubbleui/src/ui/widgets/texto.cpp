#include "texto.hpp"
#include "src/arquivadores/textura.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

BubbleUI::Widgets::Texto::Texto(std::string* label) : label(label), resolucao(12)
{
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string* label, unsigned int pxl) : label(label), resolucao(pxl)
{
    configurar(resolucao);
}
BubbleUI::Widgets::Texto::Texto(std::string l) : resolucao(16)
{
    label = new std::string(l);
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string l, unsigned int pxl) : resolucao(pxl)
{
    label = new std::string(l);
    configurar(resolucao);
}

void BubbleUI::Widgets::Texto::atualizar(float deltaTime)
{
    

}

void BubbleUI::Widgets::Texto::renderizar()
{
    float x = 0.0f;
    float y = 0.0f;
    for (char c : *label) {
        Character ch = (*obterCaracteres())[c];

        float xpos = x + ch.Bearing.x;
        float ypos = resolucao + (y - ch.Bearing.y);
        float w = ch.Size.x;
        float h = ch.Size.y;

        if (c == '\n') {
            x = 0.0f;
            pai->widget_pos = { static_cast<int>(pai->obtRect().x), static_cast<int>(ypos + pai->widget_pos.y + h) };
            continue;
        }
        // Atualiza o retângulo do corpo_do_widget para o caractere
        corpo_do_widget->defPos({ xpos + (pai->widget_pos.x + pai->widget_padding.x), ypos + (pai->widget_pos.y + pai->widget_padding.y)});
        corpo_do_widget->defTam({ w, h });

        pai->obtCtx()->shader.use();
        pai->obtCtx()->shader.setBool("texto", true);
        pai->obtCtx()->shader.setVec3("cor_texto", 1, 1, 1);
        pai->obtCtx()->shader.setInt("textura", 0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        corpo_do_widget->renderizar(GL_TRIANGLES);

        x += (ch.Advance >> 6); // 1/64 pixels

    }
}

void BubbleUI::Widgets::Texto::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path);
    definirResolucao(resolucao);
}
// Destrutor para liberar recursos
BubbleUI::Widgets::Texto::~Texto() {
}
