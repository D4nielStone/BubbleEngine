#include "item_texto.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Arquivadores;

// Construtor que inicializa o Texto com uma label_shared e configura��es padr�o
BubbleUI::Items::Texto::Texto(std::string* label_shared) : resolucao(12), label_shared(label_shared), texto(""), letra_padding({ 4, 4 })
{
    configurar(); // Configura o item com as configura��es padr�o
}

// Construtor que inicializa o Texto com uma label_shared passada como string
BubbleUI::Items::Texto::Texto(const std::string &l) : resolucao(12), frase(l), letra_padding({5, 4})
{
    configurar(); // Configura o item com as configura��es padr�o
}

// M�todo para atualizar o estado do Texto, incluindo a l�gica de intera��o
void BubbleUI::Items::Texto::atualizar()
{
    if (label_shared)  frase = *label_shared; // Atualiza a frase com o valor da label_shared
    renderizar_texto(); // Renderiza o texto do item
}

// M�todo para renderizar o Texto na tela
void BubbleUI::Items::Texto::renderizar() const
{
    // Renderiza cada letra do texto
    for (auto& rect : letras_rect)
    {
        shader.use(); // Usa o shader para renderiza��o
        shader.setVec3("quadrado.cor", cor.r, cor.g, cor.b); // Define a cor do quadrado/letra
        shader.setVec2("quadrado.posicao", rect.rect.x, rect.rect.y); // Define a posi��o da letra
        shader.setVec2("quadrado.tamanho", rect.rect.z, rect.rect.w); // Define o tamanho da letra
        shader.setInt("textura", 0); // Define a textura para o shader

        glBindTexture(GL_TEXTURE_2D, rect.ID); // Associa a textura da letra
        glBindVertexArray(rect_vertex.VAO); // Associa o VAO
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rect_vertex.indices.size()), GL_UNSIGNED_INT, 0); // Desenha a letra como tri�ngulos
        glBindVertexArray(0); // Desassocia o VAO
        glBindTexture(GL_TEXTURE_2D, 0); // Desassocia a textura
    }
}

// M�todo para renderizar o texto dentro do Texto
void BubbleUI::Items::Texto::renderizar_texto()
{
    // Posiciona o box dentro do widget, com padding do pai
    box_pos.x =  pai->obterRetangulo().x + pai->widgetPadding.x + pai->posicaoWidget.x;
    box_pos.y =  pai->obterRetangulo().y + pai->widgetPadding.y + pai->posicaoWidget.y;
    box_size.x = pai->obterRetangulo().w - pai->widgetPadding.x * 2;
    box_size.y = 0; // Inicialize como 0, vai ser atualizado com a altura do texto

    // Vari�veis para as dimens�es da letra
    int h_letter = 0, w_letter = 0, x_letter = 0, y_letter = 0, w_line = 0;
    line_pos.y = 0;
    line_pos.x = letra_padding.x;
    largura_texto = 0;

    letras_rect.clear(); // Limpa a lista de ret�ngulos das letras

    Bubble::Arquivadores::Character ch;

    // Itera sobre cada caractere na frase
    for (char& c : frase) {
        ch = (*Bubble::Arquivadores::obterCaracteres())[c]; // Obt�m o caractere atual

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

        // Adiciona o ret�ngulo da letra para renderiza��o
        letras_rect.push_back({ paraNDC(), ch.TextureID });

        w_line += (ch.Advance >> 6); // Incrementa a largura da linha com o avan�o do caractere
        if (w_line > largura_texto)
            largura_texto = w_line; // Atualiza a largura do texto
    }
    // Atualiza o tamanho do box para o pr�ximo widget
    box_size.y = line_pos.y + 12 + letra_padding.y * 2;  // Altura do texto mais padding
    altura = box_size.y + pai->widgetPadding.y * 2;
    // Atualiza a largura para pop_up
    largura = largura_texto + letra_padding.x * 2 + pai->widgetPadding.x * 2;
    if(!vquebrarLinha)
        pai->posicaoWidget.x += largura;
    else
        pai->posicaoWidget.y = box_pos.y + box_size.y - pai->obterRetangulo().y;

}

// M�todo para transformar coordenadas de pixel para NDC (Normalized Device Coordinates)
Vector4f BubbleUI::Items::Texto::paraNDC()
{
    Vector4f coord_ndc;

    // Calcula as coordenadas normalizadas
    coord_ndc.z = (char_rect.w * 2.f) / contexto->tamanho.width;
    coord_ndc.w = -(2.0f * char_rect.h) / contexto->tamanho.height;
    coord_ndc.x = (char_rect.x * 2.f) / contexto->tamanho.width - 1.f;
    coord_ndc.y = 1.0f - (2.0f * char_rect.y) / contexto->tamanho.height;

    return coord_ndc;
}

// M�todo para configurar o Texto com uma resolu��o e um caminho de fonte
void BubbleUI::Items::Texto::configurar(const std::string &font_path, unsigned int resolucao)
{
    carregarFonte(font_path); // Carrega a fonte especificada
}

// M�todo para definir a moldura do Texto
void BubbleUI::Items::Texto::definirPai(Formas::Moldura* m)
{
    pai = m;
    contexto = m->obterContexto();
}

void BubbleUI::Items::Texto::definirEscondido(bool boleano)
{
    escondido = boleano;
}

void BubbleUI::Items::Texto::quebrarLinha(bool boleano)
{
    vquebrarLinha = boleano;
}

// Destrutor do Texto que libera a mem�ria alocada
BubbleUI::Items::Texto::~Texto() = default;