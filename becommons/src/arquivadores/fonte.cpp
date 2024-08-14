#include "fonte.hpp"
#include "src/depuracao/debug.hpp"
#include "glad/glad.h"

using namespace Bubble::Arquivadores;

FT_Face face;
FT_Library library;
bool ft_carregado = false;
std::map<char, Character> Characters;

BECOMMONS_DLL_API std::map<char, Character>* Bubble::Arquivadores::obterCaracteres()
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
            Debug::emitir(Debug::Erro, "FreeType inicializacao");
        }

        // Carrega Fonte
        if (FT_New_Face(library, path.c_str(), 0, &face) == FT_Err_Unknown_File_Format)
        {
            Debug::emitir(Debug::Erro, "FreeType Fonte");
        }

        definirResolucao(16);
        ft_carregado = true;
    }
}

BECOMMONS_DLL_API void Bubble::Arquivadores::definirResolucao(int resolution)
{
    Characters.clear();
    FT_Set_Pixel_Sizes(face, 0, resolution);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Debug::emitir(Debug::Erro, "FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
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
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
}
