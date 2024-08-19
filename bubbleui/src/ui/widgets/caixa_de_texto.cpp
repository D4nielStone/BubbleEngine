#include "caixa_de_texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/tempo/delta_time.hpp"
#include <src/depuracao/debug.hpp>

BubbleUI::Widgets::CaixaTexto::CaixaTexto() : letra_padding({ 5, 0 }), letra_antiga(' '), gatilho1(false), gatilho2(false)
{
    resolucao=(12);
    label = new std::string();
    Texto::Texto(label, resolucao);
}

void BubbleUI::Widgets::CaixaTexto::atualizar(float deltaTime)
{
    cronometro += Bubble::Tempo::delta_time;

    if (pai->obtCtx()->inputs->teclado_action == GLFW_PRESS && !gatilho1) // Detecta primeiro toque
    {
        gatilho1 = true;
        processarEntrada(pai->obtCtx()->inputs->letra);
        cronometro = 0.0;
    }

    if (cronometro >= 0.4) // Ativa repetição após o tempo inicial de 0.4 segundos
        gatilho2 = true;

    if (pai->obtCtx()->inputs->teclado_action == GLFW_RELEASE)
    {
        gatilho2 = false;
        gatilho1 = false;
    }

    // Processa repetição com intervalo de 1 milissegundo
    if (gatilho1 && gatilho2)
    {
        if (cronometro >= 0.05) // 5 centesimos de segundos
        {
            processarEntrada(pai->obtCtx()->inputs->letra);
            cronometro = 0; // Reseta o cronômetro após cada repetição
        }
    }

    largura = pai->obtRect().w - pai->widget_padding.x * 2;
    if (posicao_antiga != pai->widget_pos)
    {
        moldura.defPos({ pai->widget_pos.x, pai->widget_pos.y });
    }
    moldura.defTam({ (float)largura, (float)altura_frase + letra_padding.y });
    moldura.atualizar(Bubble::Tempo::delta_time);
}


void BubbleUI::Widgets::CaixaTexto::renderizar()
{
    posicao_antiga = pai->widget_pos;
    pai->widget_pos.x;
    pai->widget_pos.y;

    label->push_back('\n');
    moldura.renderizar(GL_TRIANGLES);
    renderizar_texto();
    label->pop_back();
}


void BubbleUI::Widgets::CaixaTexto::renderizar_texto()
{
    int y = 0, xpos = 0, ypos = 0, w = 0, h = 0;
    int largura_frase = 0;
    xori = pai->widget_pos.x;
    yori = pai->widget_pos.y;
    altura_frase = 0;
    for (char c : *label) {
        // Procura caractere na lista de caracteres
        Bubble::Arquivadores::Character ch = (*Bubble::Arquivadores::obterCaracteres())[c];
        // Defini posicao e dimensao
        ypos = resolucao + (y - ch.Bearing.y) + yori;
        xpos = largura_frase + ch.Bearing.x + xori;
        w = ch.Size.x;
        h = ch.Size.y;

        // Defini limite do tamanho do texto
        if (largura_frase + (resolucao + letra_padding.x * 2) >= largura || c == '\n')
        {
            largura_frase = 0;
            ypos += h;
            altura_frase = ypos;
            pai->widget_pos = { (float)xori ,(float)ypos };
            xori = pai->widget_pos.x;
            yori = pai->widget_pos.y;
            continue;
        }

        // Atualiza o retângulo do corpo_do_widget para o caractere
        corpo_do_widget.defPos({ (float)xpos + letra_padding.x, (float)ypos + letra_padding.y });
        corpo_do_widget.defTam({ (float)w, (float)h });

        pai->obtCtx()->shader.use();
        pai->obtCtx()->shader.setBool("texto", true);
        pai->obtCtx()->shader.setVec3("cor_texto", 1, 1, 1);
        pai->obtCtx()->shader.setInt("textura", 0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        corpo_do_widget.renderizar(GL_TRIANGLES);

        largura_frase += (ch.Advance >> 6); // 1/64 pixels
    }
    pai->widget_pos = { pai->obtRect().x, 0};
}


void BubbleUI::Widgets::CaixaTexto::processarEntrada(char c)
{
    if (c == '\b' && !label->empty()) // Backspace
    {
        label->pop_back();
    }
    else if (c >= 32 && c <= 126) // Caracteres imprimíveis
    {
        label->push_back(c);
    }
}
