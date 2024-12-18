
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "caixa_de_texto.hpp"
#include "src/ui/painel/painel.hpp"
#include <src/depuracao/debug.hpp>

using namespace BubbleUI::Widgets;

CaixaTexto::CaixaTexto(const std::string &mensagem)
    : gatilho1(false), gatilho2(false), mensagem(mensagem)
{
    quebrarLinha = true;
    resolucao = 12;
    lines_box_limite = 3;
    definirFonte(); 
}

CaixaTexto::CaixaTexto(std::shared_ptr<std::string> buffer, const std::string& mensagem)
    : mensagem(mensagem), buffer_texto(buffer) 
{
    quebrarLinha = true;
    if (buffer) {
        texto = buffer->c_str(); texto_cursor_index = buffer->size() - 1;
    }
    resolucao = 12;
    lines_box_limite = 3;
    definirFonte();
}

CaixaTexto::CaixaTexto(std::string* buffer, const std::string& mensagem)
    : mensagem(mensagem), buffer_texto(buffer)
{
    quebrarLinha = true;
    if (buffer) {
        texto = buffer->c_str(); texto_cursor_index = buffer->size() - 1;
    }
    resolucao = 12;
    lines_box_limite = 3;
    definirFonte();
}

BubbleUI::Widgets::CaixaTexto::CaixaTexto(float* buffer, const std::string& mensagem) : buffer_numerico(buffer), mensagem(mensagem)
{
    quebrarLinha = true;
    if (buffer) {
        texto = std::to_string(*buffer); texto_cursor_index = texto.size() - 1;
    }
    resolucao = 12;
    lines_box_limite = 3;
    definirFonte();
}


void CaixaTexto::atualizar()
{
    Texto::atualizar(); // Atualiza texto

    Moldura::defCor({ 0.17f, 0.14f, 0.2f, 1.f });
    if (!painel->selecionado)
        return;

    if (colisao->mouseEmCima()) {
        contexto->cursor = contexto->cursor_texto; // Define o cursor
        if (inputs->mouseEnter == GLFW_PRESS)iniciarSelecao(); // Inicializa seleção
        else selecionando_texto = false; // Desativa seleção
    }
    else if (inputs->mouseEnter == GLFW_PRESS)
    {
        selecionado = false;
    }
    // atualiza Cursor
    if (selecionado) {
        atualizarInputs(); defCor(ROXO_ESCURO_2);
    } // Atualiza entrada
    else defCor(ROXO_ESCURO);
    if (selecionando_texto) area_de_selecao = { static_cast<int>(mouse_pos_ini.x), static_cast<int>(mouse_pos_ini.y), static_cast<int>(inputs->mousex), static_cast<int>(inputs->mousey) };

    // Atribui texto da frase para o buffer
    if (buffer_texto.get())
    {
        buffer_texto->clear();
        for (size_t i = 0; i < texto.size(); i++)   buffer_texto->push_back(texto[i]);
    }
    // Atribui texto da frase para o buffer
    if (buffer_numerico) {
        try {
            *buffer_numerico = std::stof(texto);
        }
        catch (const std::invalid_argument&) {
            // Lidando com texto inválido
            *buffer_numerico = 0.0f;
        }
        catch (const std::out_of_range&) {
            // Lidando com valores fora do intervalo de float
            *buffer_numerico = 0.0f;
        }
    }


    // Defini mensagem caso texto vazio
    if (texto.empty())
    {
        frase = mensagem;
        cor = { 0.7F, 0.7F, 0.7F };
    }
    else
    {
        frase = texto;
        cor = { 1.F, 1.F, 1.F };
    }

    // Redimensiona moldura e colisao
    colisao->definirBounds({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y), static_cast<int>(box_size.x), static_cast<int>(box_size.y) });
    Moldura::definirTamanho({ static_cast<int>(box_size.x), static_cast<int>(box_size.y) }); Moldura::definirPosicao({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    Moldura::atualizar();
}

void CaixaTexto::renderizar()
{
    Moldura::renderizar();
    Texto::renderizar();
}

void CaixaTexto::definirPai(Formas::Moldura* painel)
{
    Texto::definirPai(painel);
    contexto = painel->obterContexto();
    inputs = painel->obterContexto()->inputs; // Simplifica o acesso
}

void CaixaTexto::processarEntrada(char c)
{
    if (c == '' && texto_cursor_index != 0) // Backspace
    {
        texto.pop_back();
        texto_cursor_index = texto.size();
        return;
    }
    else if (c != '')
    {
        if (!buffer_numerico)
        {
            texto.insert(texto_cursor_index, 1, c);
            texto_cursor_index++;
        }
        else if (buffer_numerico) {
            if (isdigit(c) || c == '.'|| c == '-') { // Apenas números ou ponto
                if (c == '.' && texto.find('.') != std::string::npos) return; // Evita múltiplos pontos
                texto.insert(texto_cursor_index, 1, c);
                texto_cursor_index++;
            }
        }

    }
}

double tempo_inicial = 0.0;

void CaixaTexto::atualizarInputs()
{
    double tempo_atual = glfwGetTime(); // Obtenha o tempo atual
    if (gatilho1) {
        cronometro = tempo_atual - tempo_inicial;
    }

    if (inputs->isKeyPressed(BS) && !texto.empty() && !gatilho1) // Backspace
    {
        inputs->char_press = true;
        inputs->letra = '';
    }
    if (inputs->isKeyPressed(ENTER) && !gatilho1) // Enter
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
        tempo_inicial = glfwGetTime(); // Armazene o tempo inicial
    }

    if (cronometro >= 0.4f) // Ativa repetição após o tempo inicial de 0.4 segundos
    {
        gatilho2 = true;
    }

    if (inputs->teclado_action == GLFW_RELEASE)
    {
        gatilho2 = false;
        gatilho1 = false;
        tempo_inicial = 0.0f;
    }

    // Processa repetição com intervalo de 0.05 segundos
    if (gatilho1 && gatilho2)
    {
        if (cronometro >= 0.05f) // 5 centésimos de segundos
        {
            processarEntrada(inputs->letra);
            cronometro = 0.0f;
            tempo_inicial = glfwGetTime(); // Reinicie o tempo inicial
        }
    }
}

void CaixaTexto::iniciarSelecao()
{
    if (!selecionando_texto) { selecionado = false; mouse_pos_ini = { (int)inputs->mousex, (int)inputs->mousey }; selecionando_texto = true; }
    else selecionado = true;
}
