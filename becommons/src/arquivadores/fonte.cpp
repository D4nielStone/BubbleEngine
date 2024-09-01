#include "fonte.hpp"
#include "src/depuracao/debug.hpp"
#include "glad/glad.h"
#include <map>
#include <string>
#include <glm/glm.hpp>

using namespace Bubble::Arquivadores;

FT_Face face;
FT_Library library;
bool ft_carregado = false;
std::map<char32_t, Character> Characters;

BECOMMONS_DLL_API std::map<char32_t, Character>* Bubble::Arquivadores::obterCaracteres()
{   
    return &Characters;
}

BECOMMONS_DLL_API void Bubble::Arquivadores::carregarFonte(std::string path)
{
    if (!ft_carregado)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Inicia Biblioteca Freetype
        if (FT_Init_FreeType(&library))
        {
            Debug::emitir(Debug::Erro, "FreeType inicializacao falhou");
            return;
        }

        // Carrega Fonte
        if (FT_New_Face(library, path.c_str(), 0, &face) == FT_Err_Unknown_File_Format)
        {
            Debug::emitir(Debug::Erro, "FreeType: Formato de arquivo de fonte desconhecido");
            return;
        }

        definirResolucao(14);  // Defina o tamanho inicial da resolução
        ft_carregado = true;
    }
}

std::u32string utf8_to_utf32(const std::string& utf8) {
    std::u32string utf32;
    char32_t codepoint = 0;
    int bytes = 0;

    for (unsigned char c : utf8) {
        if (c <= 0x7F) { // ASCII range
            utf32.push_back(c);
        }
        else if (c <= 0xBF) { // Continuation bytes
            codepoint = (codepoint << 6) | (c & 0x3F);
            if (--bytes == 0) utf32.push_back(codepoint);
        }
        else if (c <= 0xDF) { // Start of 2-byte sequence
            codepoint = c & 0x1F;
            bytes = 1;
        }
        else if (c <= 0xEF) { // Start of 3-byte sequence
            codepoint = c & 0x0F;
            bytes = 2;
        }
        else { // Start of 4-byte sequence
            codepoint = c & 0x07;
            bytes = 3;
        }
    }

    return utf32;
}

BECOMMONS_DLL_API void Bubble::Arquivadores::definirResolucao(int resolution)
{
    // Limpar caracteres anteriores
    for (const auto& pair : Characters) {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    Characters.clear();

    FT_Set_Pixel_Sizes(face, 0, resolution);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Desabilitar restrição de alinhamento de byte

    // Iterar sobre um intervalo de caracteres Unicode (Exemplo: ASCII estendido de 0x20 a 0x7E)
    for (unsigned long charcode = 0x20; charcode <= 0x7E; ++charcode) {
        // Verificar se o glifo existe na fonte
        FT_UInt glyph_index = FT_Get_Char_Index(face, charcode);
        if (glyph_index == 0) {
            continue; // Pular caracteres não suportados pela fonte
        }

        // Carregar o glifo do caractere
        if (FT_Load_Char(face, charcode, FT_LOAD_RENDER)) {
            Debug::emitir(Debug::Erro, "FREETYPE: Falha ao carregar glifo para charcode " + std::to_string(charcode));
            continue;
        }

        // Gerar a textura
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Definir opções de textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Armazenar o caractere para uso futuro
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::make_pair(charcode, character));
    }
}
