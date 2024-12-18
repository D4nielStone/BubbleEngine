
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"
#include <cstdlib>
// em src/ui/widgets/texto.cpp

using namespace BubbleUI::Widgets;

using namespace Bubble::Arquivadores;

Texto::Texto(std::string* label_shared) : label_shared(label_shared), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    definirFonte();
}

Texto::Texto(const std::string& l) : resolucao(16), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    frase = l;
    definirFonte();
}

void Texto::definirTexto(const std::string& texto)
{
    frase = texto;
}

// M�todo gen�rico de atualizacao
void Texto::atualizar()
{
    // Renderiza texto de label_shared, o ponteiro
    if (label_shared) frase = *label_shared;
    renderizar_texto(frase);
}

// M�todo para renderizar o texto
void Texto::renderizar()
{
    // Configura o shader para renderizar o texto
    shader.use();
    for (auto& letra : letras_rect)
    {
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b);
        shader.setVec2("quadrado.posicao", letra.rect.x, letra.rect.y);
        shader.setVec2("quadrado.tamanho", letra.rect.w, letra.rect.h);
        shader.setInt("textura", 0);

        glBindTexture(GL_TEXTURE_2D, letra.ID);
        glActiveTexture(GL_TEXTURE0 + letra.ID);
        glBindVertexArray(rect_vertex.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// M�todo para atualizar retangulo das letras do texto
void Texto::renderizar_texto(std::string &frase)
{
    // Posiciona o box dentro do widget, com padding do pai 
        box_pos.x = painel->widgetPadding.x + painel->posicaoWidget.x;
    box_pos.y =painel->widgetPadding.y + painel->posicaoWidget.y;
    box_size.x = painel->obterRetangulo().w - (painel->posicaoWidget.x - painel->obterRetangulo().x) - painel->widgetPadding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // n�o renderiza se n�o vis�vel
    // Vari�veis para as dimens�es da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear(); // Limpa a lista de ret�ngulos das letras

    Bubble::Arquivadores::Character ch;

    // Itera sobre cada caractere na frase
    bool alinhamentoEsquerda = (alinhamentoHorizontal == Alinhamento::Esquerda);
    size_t i = alinhamentoEsquerda ? 0 : frase.size() - 1;
    size_t step = alinhamentoEsquerda ? 1 : -1;

        auto caracteres = Bubble::Arquivadores::GerenciadorDeFontes::obterInstancia().obterCaracteres(nome_da_fonte);
    // Loop at� que i seja fora dos limites
    while (i < frase.size() && i >= 0) // o limite superior � necess�rio apenas se `frase.size()` pode ser 0.
    {
        if (box_pos.y > painel->obterRetangulo().y + painel->obterRetangulo().h)
        {
            i += step;
            continue;
        }
        char& c = frase[i];

        if(caracteres)
            ch = caracteres->at(c); // Obt�m o caractere atual
        else
        {
            return;
        }

        // Define as dimens�es e posi��o da letra
        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + 10 - ch.Bearing.y + letra_padding.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y; // Ajusta a posi��o vertical
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Define o ret�ngulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // se letra ultrapassa o painel:
        if (char_rect.x + char_rect.w > painel->obterRetangulo().x + painel->obterRetangulo().w)
            break;

        // Adiciona o ret�ngulo da letra para renderiza��o
        letras_rect.push_back({ paraNDC(char_rect), ch.TextureID, });

        // Incrementa ou decrementa a largura da linha com o avan�o do caractere
        if (alinhamentoEsquerda)
            w_line += (ch.Advance >> 6);
        else
            w_line -= (ch.Advance >> 6);

        // Atualiza a largura do texto se necess�rio
        if (abs(w_line) > largura_texto)
            largura_texto = abs(w_line);

        // Atualiza o �ndice para o pr�ximo caractere
        i += step;
    }
    // Atualiza o tamanho do box para o pr�ximo widget
    box_size.y = line_pos.y + 13 + letra_padding.y * 2;  // Altura do texto mais padding
    if (quebrarLinha)painel->posicaoWidget = { (int)painel->obterRetangulo().x, (int)(box_pos.y + box_size.y) };
    else { painel->posicaoWidget.x = letra_padding.x + box_pos.x + largura_texto + painel->widgetPadding.x; };
}

// Converte coordenadas de pixel para NDC (Normalized Device Coordinates)
bubble::vetor4<float> Texto::paraNDC(const bubble::vetor4<int>& coord)
{
    bubble::vetor4<float> coord_ndc;

    coord_ndc.w = (coord.w * 2.f) / painel->obterContexto()->tamanho.width;
    coord_ndc.h = -(2.0f * coord.h) / painel->obterContexto()->tamanho.height;
    coord_ndc.x = (coord.x * 2.f) / painel->obterContexto()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * coord.y) / painel->obterContexto()->tamanho.height;

    return coord_ndc;
}

// Configura o texto com a fonte e resolu��o especificada
void Texto::definirFonte(unsigned int resolucao, const std::string& font_path)
{
    nome_da_fonte = font_path;
    Bubble::Arquivadores::GerenciadorDeFontes::obterInstancia().carregarFonte(font_path, resolucao); // Carrega a fonte
}

// M�todo para detextar caract�res selecionados
bool Texto::desenharSelecao(Vector2 mouse_inicial, Vector2 mouse_final, bubble::vetor4<int> char_rect, size_t letra_idx)
{
    // Verifica se a letra est� na �rea de sele��o
    bool intersecta_verticalmente = char_rect.y + char_rect.h > mouse_inicial.y && char_rect.y < mouse_final.y; // Verifica interse��o horizontal
    bool intersecta_horizontalmente = char_rect.x + char_rect.w > mouse_inicial.x && char_rect.x < mouse_final.x; // Verifica interse��o vertical
    bool atraz = char_rect.y > mouse_inicial.y && char_rect.y < mouse_final.y && char_rect.x < mouse_final.x || char_rect.y + char_rect.h > mouse_inicial.y && char_rect.y + char_rect.h < mouse_final.y && char_rect.x > mouse_inicial.x;
    if (intersecta_verticalmente && intersecta_horizontalmente || atraz) {
        // O ret�ngulo do caractere intersecta a �rea de sele��o tanto vertical quanto horizontalmente
        return true;
    }
    else {
        // O caractere est� fora da �rea de sele��o
        return false;
    }
}
