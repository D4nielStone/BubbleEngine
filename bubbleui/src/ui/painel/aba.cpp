#include "aba.hpp"
#include "src/ui/widgets/texto.hpp"
#include "painel.hpp"

BubbleUI::Aba::Aba(Painel* painel) : painel(painel)
{
	Widgets::Texto::configurar(12, "assets/fontes/noto-sans/notosans-bold.ttf");
    corpo_do_texto = new Formas::Rect(painel->obtCtx());
    corpo_rect = new Formas::Rect(painel->obtCtx());
    corpo_rect->defCor({0.4, 0, 0.4});
}

void BubbleUI::Aba::atualizar(float deltaTime)
{
    corpo_rect->defPos({painel->obtRect().x, painel->obtRect().y});
    corpo_rect->defTam({static_cast<float>(painel->obtRect().w), 15});
    corpo_rect->atualizar(deltaTime);
}

void BubbleUI::Aba::renderizar()
{
    corpo_rect->renderizar(GL_TRIANGLES);
	renderizar_texto();
}

void BubbleUI::Aba::renderizar_texto()
{
    float x = 0.0f;
    float y = 0.0f;
    float ypos, h;
    for (char c : painel->nome()) {
        Bubble::Arquivadores::Character ch = (*Bubble::Arquivadores::obterCaracteres())[c];

        float xpos = x + ch.Bearing.x;
        ypos = 12 + (y - ch.Bearing.y);
        float w = ch.Size.x;
        h = ch.Size.y;

        // Atualiza o ret�ngulo do corpo_do_texto para o caractere
        corpo_do_texto->defPos({ xpos + (painel->obtRect().x + painel->widget_padding.x), ypos + (painel->obtRect().y) });
        corpo_do_texto->defTam({ w, h });

        painel->obtCtx()->shader.use();
        painel->obtCtx()->shader.setBool("texto", true);
        painel->obtCtx()->shader.setVec3("cor_texto", 0.8, 0.8, 0.8);
        painel->obtCtx()->shader.setInt("textura", 0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        corpo_do_texto->renderizar(GL_TRIANGLES);

        x += (ch.Advance >> 6); // 1/64 pixels

    }
    painel->widget_pos = { static_cast<int>(painel->obtRect().x), static_cast<int>(painel->obtRect().y + corpo_rect->obtRect().h) };
}