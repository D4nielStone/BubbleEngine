#include "caixa_de_texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/tempo/delta_time.hpp"
#include <src/depuracao/debug.hpp>

BubbleUI::Widgets::CaixaTexto::CaixaTexto(std::string mensagem)
    : gatilho1(false), gatilho2(false), mensagem(mensagem)
{
    // Inicialize corretamente a classe Texto, se necessário
    texto = "";
    letra_padding = { 5, 5 };
    resolucao = (12);
    lines_box_limite = 3;
    configurar();
    moldura.defCor({ 0.05f, 0.05f, 0.05f});
}

void BubbleUI::Widgets::CaixaTexto::atualizar()
{
    Texto::atualizar();
    cronometro += Bubble::Tempo::delta_time;
    auto* inputs = pai->obtCtx()->inputs; // Simplifica o acesso

    if (inputs->isKeyPressed(BS) && !texto.empty() && !gatilho1) // Backspace
    {
        inputs->char_press = true;
        inputs->letra = '\b';
    }
    if (inputs->isKeyPressed(ENTER) && !texto.empty() && !gatilho1) // Enter
    {
        inputs->char_press = true;
        inputs->letra = '\n';
    }
    if (inputs->char_press && !gatilho1) // Detecta primeiro toque
    {
        gatilho1 = true;
        inputs->char_press = false;
        processarEntrada(inputs->letra);
        cronometro = 0.0f;
    }

    if (cronometro >= 0.4f) // Ativa repetição após o tempo inicial de 0.4 segundos
    {
        gatilho2 = true;
    }

    if (inputs->teclado_action == GLFW_RELEASE)
    {
        gatilho2 = false;
        gatilho1 = false;
    }

    // Processa repetição com intervalo de 1 milissegundo
    if (gatilho1 && gatilho2)
    {
        if (cronometro >= 0.05f) // 5 centésimos de segundos
        {
            processarEntrada(inputs->letra);
            cronometro = 0.0f; // Reseta o cronômetro após cada repetição
        }
    }
    if (texto.empty())
    {
        frase = mensagem;
        cor = { 0.7, 0.7, 0.7 };
    }
    else
    {
        frase = texto;
        cor = { 1, 1, 1 };
    }
    moldura.defTam(box_size);
    moldura.defPos(box_pos);
    moldura.atualizar();
}

void BubbleUI::Widgets::CaixaTexto::renderizar()
{
    moldura.renderizar(GL_TRIANGLES);
    Texto::renderizar();
}

void BubbleUI::Widgets::CaixaTexto::processarEntrada(char c)
{
    if (c == '\b' && !texto.empty()) // Backspace
    {
        texto.pop_back();
        return;
    }
    else if (c != '\b')
    texto.push_back(c);
}