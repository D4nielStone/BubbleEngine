#include "caixa_de_texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/tempo/delta_time.hpp"
#include <src/depuracao/debug.hpp>

BubbleUI::Widgets::CaixaTexto::CaixaTexto(std::string mensagem)
    : gatilho1(false), gatilho2(false), mensagem(new std::string(mensagem))
{
    letra_padding = { 5, 5 };
    resolucao = 12;
    lines_box_limite = 3;
    configurar();
    moldura.defCor({ 0.05f, 0.05f, 0.05f });
}

BubbleUI::Widgets::CaixaTexto::CaixaTexto(std::string* buffer, std::string mensagem)
    : mensagem(new std::string(mensagem)), buffer_texto(buffer) 
{
    if (buffer) {
        texto = buffer->c_str(); texto_cursor_index = buffer->size() - 1;
    }
    letra_padding = { 5, 5 };
    resolucao = 12;
    lines_box_limite = 3;
    configurar();
    moldura.defCor({ 0.05f, 0.05f, 0.05f });
}

void BubbleUI::Widgets::CaixaTexto::atualizar()
{
    if (pai->selecionado) 
    {
        if (colisao->mouseEmCima()) {
            contexto->cursor = contexto->cursor_texto;
            if (inputs->mouseEnter == GLFW_PRESS)iniciarSelecao();
            else selecionando_texto = false;
        } // atualiza Cursor
        if (selecionado) atualizarInputs(); // Atualiza entrada
        if (selecionando_texto) area_de_selecao = { (float)mouse_pos_ini.x, (float)mouse_pos_ini.y, (int)inputs->mousex, (int)inputs->mousey };
    }
    Texto::atualizar(); // Atualiza texto

    for (auto& letra : letras_rect) // Desenha cursor do texto
    {
        if (letra.index == texto_cursor_index) texto_cursor_pos = letra.rect; texto_cursor_pos.w = 1;
    }
    // Atribui texto da frase para o buffer
    if (buffer_texto)
    {
        buffer_texto->clear();
        for (size_t i = 0; i < texto.size(); i++)   buffer_texto->push_back(texto[i]);
    }

    // Defini mensagem caso texto vazio
    if (texto.empty())
    {
        frase = *mensagem;
        cor = { 0.7, 0.7, 0.7 };
    }
    else
    {
        frase = texto;
        cor = { 1, 1, 1 };
    }

    // Redimensiona moldura e colisao
    colisao->defRect({box_pos.x, box_pos.y, (int)box_size.x, (int)box_size.y});
    moldura.defTam(box_size);moldura.defPos(box_pos);
    moldura.atualizar();
}

void BubbleUI::Widgets::CaixaTexto::renderizar()
{
    moldura.renderizar(0x0004);
    Texto::renderizar();
    // Renderiza o cursor
    if (Bubble::Tempo::s_passados % 2 == 0)
    {
        shaderQuad.use();
        shaderQuad.setCor("quadrado.cor", { 1, 1, 1, 0.95f });
        shaderQuad.setVec2("quadrado.posicao", texto_cursor_pos.x, texto_cursor_pos.y);
        shaderQuad.setVec2("quadrado.tamanho", texto_cursor_pos.z, texto_cursor_pos.w);

        glBindVertexArray(rect_vertex.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void BubbleUI::Widgets::CaixaTexto::defPainel(Painel* painel)
{
    BubbleUI::Widgets::Texto::defPainel(painel);
    contexto = painel->obtCtx();
    inputs = pai->obtCtx()->inputs; // Simplifica o acesso
}

void BubbleUI::Widgets::CaixaTexto::processarEntrada(char c)
{
    if (c == '\b' && !texto.empty() && texto_cursor_index != 0) // Backspace
    {
        texto.pop_back();
        texto_cursor_index = texto.size();
        return;
    }
    else if (c != '\b')
    {
        texto.insert(texto_cursor_index, 1, c);
        texto_cursor_index++;
    }
}

void BubbleUI::Widgets::CaixaTexto::atualizarInputs()
{
    cronometro += Bubble::Tempo::delta_time;

    if (inputs->isKeyPressed(BS) && !frase.empty() && !gatilho1) // Backspace
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
}
void BubbleUI::Widgets::CaixaTexto::iniciarSelecao()
{
    if (!selecionando_texto) { selecionado = false; mouse_pos_ini = { (int)inputs->mousex, (int)inputs->mousey }; selecionando_texto = true; }
    else selecionado = true;
}