#include "caixa_de_texto.hpp"
#include "src/ui/painel/painel.hpp"
#include <src/depuracao/debug.hpp>

BubbleUI::Widgets::CaixaTexto::CaixaTexto(const std::string &mensagem)
    : gatilho1(false), gatilho2(false), mensagem(mensagem)
{
    quebrarLinha = true;
    resolucao = 12;
    lines_box_limite = 3;
    configurar(); 
}

BubbleUI::Widgets::CaixaTexto::CaixaTexto(std::shared_ptr<std::string> buffer, const std::string& mensagem)
    : mensagem(mensagem), buffer_texto(buffer) 
{
    quebrarLinha = true;
    if (buffer) {
        texto = buffer->c_str(); texto_cursor_index = buffer->size() - 1;
    }
    resolucao = 12;
    lines_box_limite = 3;
    configurar(); 
}

BubbleUI::Widgets::CaixaTexto::CaixaTexto(std::string* buffer, const std::string& mensagem)
    : mensagem(mensagem), buffer_texto(buffer)
{
    quebrarLinha = true;
    if (buffer) {
        texto = buffer->c_str(); texto_cursor_index = buffer->size() - 1;
    }
    resolucao = 12;
    lines_box_limite = 3;
    configurar();
}


void BubbleUI::Widgets::CaixaTexto::atualizar()
{
    Texto::atualizar(); // Atualiza texto

    Moldura::defCor({ 0.17f, 0.14f, 0.2f, 1.f });
    if (painel->selecionado) 
    {
        if (colisao.mouseEmCima()) {
            contexto->cursor = contexto->cursor_texto;
            if (inputs->mouseEnter == GLFW_PRESS)iniciarSelecao();
            else selecionando_texto = false;
        } // atualiza Cursor
        if (selecionado) atualizarInputs(); // Atualiza entrada
        if (selecionando_texto) area_de_selecao = { static_cast<float>(mouse_pos_ini.x), static_cast<float>(mouse_pos_ini.y), (int)inputs->mousex, (int)inputs->mousey };
    }

    //for (auto& letra : letras_rect) // Desenha cursor do texto
    //{
    //    if (letra.index == texto_cursor_index) texto_cursor_pos = letra.rect; texto_cursor_pos.w = 1;
    //}
    // Atribui texto da frase para o buffer
    if (buffer_texto.get())
    {
        buffer_texto->clear();
        for (size_t i = 0; i < texto.size(); i++)   buffer_texto->push_back(texto[i]);
    }

    // Defini mensagem caso texto vazio
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

    // Redimensiona moldura e colisao
    colisao.defRect({box_pos.x, box_pos.y, (int)box_size.x, (int)box_size.y});
    Moldura::definirTamanho({ static_cast<int>(box_size.x), static_cast<int>(box_size.y) }); Moldura::definirPosicao({ static_cast<int>(box_pos.x), static_cast<int>(box_pos.y) });
    Moldura::atualizar();

    painel->posicaoWidget.y += painel->widgetPadding.y;
}

void BubbleUI::Widgets::CaixaTexto::renderizar() const
{
    Moldura::renderizar();
    Texto::renderizar();
    // Renderiza o cursor
    //if (Bubble::Tempo::s_passados % 2 == 0)
    //{
    //    shaderQuad.use();
    //    shaderQuad.setCor("quadrado.cor", { 1, 1, 1, 0.95f });
    //    shaderQuad.setVec2("quadrado.posicao", texto_cursor_pos.x, texto_cursor_pos.y);
    //    shaderQuad.setVec2("quadrado.tamanho", texto_cursor_pos.z, texto_cursor_pos.w);
    //
    //    glBindVertexArray(rect_vertex.VAO);
    //    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
    //    glBindVertexArray(0);
    //}
}

void BubbleUI::Widgets::CaixaTexto::definirPai(Formas::Moldura* painel)
{
    BubbleUI::Widgets::Texto::definirPai(painel);
    contexto = painel->obterContexto();
    inputs = painel->obterContexto()->inputs; // Simplifica o acesso
}

void BubbleUI::Widgets::CaixaTexto::processarEntrada(char c)
{
    if (c == '\b' && texto_cursor_index != 0) // Backspace
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

double tempo_inicial = 0.0;

void BubbleUI::Widgets::CaixaTexto::atualizarInputs()
{
    double tempo_atual = glfwGetTime(); // Obtenha o tempo atual
    if (gatilho1) {
        cronometro = tempo_atual - tempo_inicial;
    }

    if (inputs->isKeyPressed(BS) && !texto.empty() && !gatilho1) // Backspace
    {
        inputs->char_press = true;
        inputs->letra = '\b';
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

void BubbleUI::Widgets::CaixaTexto::iniciarSelecao()
{
    if (!selecionando_texto) { selecionado = false; mouse_pos_ini = { (int)inputs->mousex, (int)inputs->mousey }; selecionando_texto = true; }
    else selecionado = true;
}