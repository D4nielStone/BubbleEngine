#include "texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

BubbleUI::Widgets::Texto::Texto(std::string* label_shared) : label_shared(label_shared), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    configurar();
}

BubbleUI::Widgets::Texto::Texto(const std::string& l) : resolucao(16), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    label_shared = new std::string(l);
    configurar();
}

// Método genérico de atualizacao
void BubbleUI::Widgets::Texto::atualizar()
{
    // Renderiza texto de label_shared, o ponteiro
    if (label_shared) frase = *label_shared;
    renderizar_texto();
}

// Método para renderizar o texto
void BubbleUI::Widgets::Texto::renderizar() const
{
    // Configura o shader para renderizar o texto
    shader.use();
    for (auto& letra : letras_rect)
    {
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b);
        shader.setVec2("quadrado.posicao", letra.rect.x, letra.rect.y);
        shader.setVec2("quadrado.tamanho", letra.rect.z, letra.rect.w);
        shader.setInt("textura", 0);

        glBindTexture(GL_TEXTURE_2D, letra.ID);
        glBindVertexArray(rect_vertex.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// Método para atualizar retangulo das letras do texto
void BubbleUI::Widgets::Texto::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x = painel->obterRetangulo().x + painel->widgetPadding.x + painel->posicaoWidget.x;
    box_pos.y = painel->obterRetangulo().y + painel->widgetPadding.y + painel->posicaoWidget.y;
    box_size.x = painel->obterRetangulo().w - painel->widgetPadding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // não renderiza se não visível
    // Variáveis para as dimensões da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear(); // Limpa a lista de retângulos das letras

    Bubble::Arquivadores::Character ch;

    // Itera sobre cada caractere na frase
    for (size_t i = 0; i < frase.size(); i++)
    {
        if (box_pos.y > painel->obterRetangulo().y + painel->obterRetangulo().h)continue;
        char& c = frase[i];
        ch = (*Bubble::Arquivadores::obterCaracteres())[c]; // Obtém o caractere atual

        // Define as dimensões e posição da letra
        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + 10 - ch.Bearing.y + letra_padding.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y; // Ajusta a posição vertical
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Define o retângulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // Adiciona o retângulo da letra para renderização
        letras_rect.push_back({ paraNDC(char_rect), ch.TextureID});

        w_line += (ch.Advance >> 6); // Incrementa a largura da linha com o avanço do caractere
        if (w_line > largura_texto)
            largura_texto = w_line; // Atualiza a largura do texto
    }
    // Atualiza o tamanho do box para o próximo widget
    box_size.y = line_pos.y + 12 + letra_padding.y * 2;  // Altura do texto mais padding
    painel->posicaoWidget.y = box_pos.y + box_size.y - painel->obterRetangulo().y;
}

// Converte coordenadas de pixel para NDC (Normalized Device Coordinates)
Vector4f BubbleUI::Widgets::Texto::paraNDC(const Vector4& coord)
{
    Vector4f coord_ndc;

    coord_ndc.z = (coord.w * 2.f) / painel->obterContexto()->tamanho.width;
    coord_ndc.w = -(2.0f * coord.h) / painel->obterContexto()->tamanho.height;
    coord_ndc.x = (coord.x * 2.f) / painel->obterContexto()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * coord.y) / painel->obterContexto()->tamanho.height;

    return coord_ndc;
}

// Configura o texto com a fonte e resolução especificada
void BubbleUI::Widgets::Texto::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path); // Carrega a fonte
}

// Método para detextar caractéres selecionados
bool BubbleUI::Widgets::Texto::desenharSelecao(Vector2 mouse_inicial, Vector2 mouse_final, Vector4 char_rect, size_t letra_idx)
{
    // Verifica se a letra está na área de seleção
    bool intersecta_verticalmente = char_rect.y + char_rect.h > mouse_inicial.y && char_rect.y < mouse_final.y; // Verifica interseção horizontal
    bool intersecta_horizontalmente = char_rect.x + char_rect.w > mouse_inicial.x && char_rect.x < mouse_final.x; // Verifica interseção vertical
    bool atraz = char_rect.y > mouse_inicial.y && char_rect.y < mouse_final.y && char_rect.x < mouse_final.x || char_rect.y + char_rect.h > mouse_inicial.y && char_rect.y + char_rect.h < mouse_final.y && char_rect.x > mouse_inicial.x;
    if (intersecta_verticalmente && intersecta_horizontalmente || atraz) {
        // O retângulo do caractere intersecta a área de seleção tanto vertical quanto horizontalmente
        return true;
    }
    else {
        // O caractere está fora da área de seleção
        return false;
    }
}