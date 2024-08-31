#include "texto.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

BubbleUI::Widgets::Texto::Texto(std::string* label) : label(label), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string* label, unsigned int pxl) : label(label), resolucao(pxl), letra_padding({ 0, 0 })
{
    configurar(resolucao);
}

BubbleUI::Widgets::Texto::Texto(std::string l) : resolucao(16), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar();
}

BubbleUI::Widgets::Texto::Texto(std::string l, unsigned int pxl) : resolucao(pxl), letra_padding({ 0, 0 })
{
    lines_box_limite = 3;
    label = new std::string(l);
    configurar(resolucao);
}

void BubbleUI::Widgets::Texto::atualizar()
{
    if (label)
        frase = *label;
    renderizar_texto();
}

void BubbleUI::Widgets::Texto::renderizar()
{
    for (auto& letra : letras_rect)
    {
        // Renderiza o fundo da letra selecionada
        if (letra.letra_selecionada)
        {
            shaderQuad.use();
            shaderQuad.setCor("quadrado.cor", letra.cor_de_fundo);
            shaderQuad.setVec2("quadrado.posicao", letra.fundo_rect.x, letra.fundo_rect.y);
            shaderQuad.setVec2("quadrado.tamanho", letra.fundo_rect.z, letra.fundo_rect.w);
            glBindVertexArray(rect_vertex.VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // Renderiza a letra
        shader.use();
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

void BubbleUI::Widgets::Texto::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x = pai->obtRect().x + pai->widget_padding.x + pai->widget_pos.x;
    box_pos.y = pai->obtRect().y + pai->widget_padding.y + pai->widget_pos.y;
    box_size.x = pai->obtRect().w - pai->widget_padding.x * 2;
    box_size.y = 0; // Inicialize como 0, será atualizado com a altura do texto

    // Dimensões da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    Bubble::Arquivadores::Character ch;
    letras_rect.clear();

    for (char c : frase) {
        ch = (*Bubble::Arquivadores::obterCaracteres())[c];

        w_letter = ch.Size.x;
        h_letter = ch.Size.y;
        y_letter = (box_pos.y + resolucao - ch.Bearing.y);
        x_letter = w_line + ch.Bearing.x;

        // Verifica quebra de linha
        if (x_letter + w_letter + letra_padding.x >= box_size.x || c == '\n')
        {
            line_pos.y += resolucao + letra_padding.y;
            w_line = 0;
            x_letter = w_line + ch.Bearing.x;
        }

        // Definir retângulo da letra
        char_rect.x = box_pos.x + x_letter + line_pos.x;
        char_rect.y = y_letter + line_pos.y;
        char_rect.w = w_letter;
        char_rect.h = h_letter;

        // Ajustar o retângulo de fundo para lidar com seleções invertidas
        char_fundo_rect.x = char_rect.x;
        char_fundo_rect.y = line_pos.y + box_pos.y;
        char_fundo_rect.h = resolucao + letra_padding.y;

        // Verificar se o retângulo de fundo precisa ser ajustado
        if (char_fundo_rect.w < 0) {
            char_fundo_rect.x += char_fundo_rect.w;
            char_fundo_rect.w = -char_fundo_rect.w; // Tornar w positivo
        }
        else {
            char_fundo_rect.w = char_rect.w;
        }

        // Verifica se a letra está na área de seleção
        if (char_rect.x + char_rect.w >= area_de_selecao.x &&
            char_rect.x <= area_de_selecao.w &&
            char_rect.y + char_rect.h >= area_de_selecao.y &&
            char_rect.y <= area_de_selecao.h)
        {
            cor_de_selecao = { 0, 0, 1, 0.5 }; // Cor de fundo para seleção
            letra_selecionada = true;
        }
        else
        {
            cor_de_selecao = { 0, 0, 0, 0 }; // Transparente para não selecionado
            letra_selecionada = false;
        }

        letras_rect.push_back({ paraNDC(char_rect), paraNDC(char_fundo_rect), ch.TextureID, cor_de_selecao, letra_selecionada });

        // Atualizar a largura da linha
        w_line += (ch.Advance >> 6); // Avanço em 1/64 pixels
        if (w_line > largura_texto)
            largura_texto = w_line;
    }

    // Atualizar o tamanho da caixa de texto com a altura total
    box_size.y = line_pos.y + resolucao + letra_padding.y;
    pai->widget_pos.y = box_pos.y + box_size.y - pai->obtRect().y;
}

// Converte coordenadas de pixel para NDC (Normalized Device Coordinates)
Vector4f BubbleUI::Widgets::Texto::paraNDC(Vector4 coord)
{
    Vector4f coord_ndc;

    coord_ndc.z = (coord.w * 2.f) / pai->obtCtx()->tamanho.width;
    coord_ndc.w = -(2.0f * coord.h) / pai->obtCtx()->tamanho.height;
    coord_ndc.x = (coord.x * 2.f) / pai->obtCtx()->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * coord.y) / pai->obtCtx()->tamanho.height;

    return coord_ndc;
}

// Configura o texto com a fonte e resolução especificada
void BubbleUI::Widgets::Texto::configurar(unsigned int resolucao, std::string font_path)
{
    carregarFonte(font_path);
}

// Destrutor para liberar recursos
BubbleUI::Widgets::Texto::~Texto()
{
    if (label) delete[] label;
}