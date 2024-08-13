#include "texto.hpp"
#include "src/arquivadores/textura.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/depuracao/debug.hpp"

BubbleUI::Widgets::Texto::Texto(std::string label) : label(label), resolucao(16)
{
}

BubbleUI::Widgets::Texto::Texto(std::string label, unsigned int pxl) : label(label), resolucao(pxl)
{
    // Inicia Biblioteca Freetype
    if (FT_Init_FreeType(&library))
    {
        Debug::emitir(Debug::Erro, "FreeType inicializacao");
    }

    // Carrega Fonte
    if (FT_New_Face(library, "assets/fontes/arial/ARIAL.ttf", 0, &face) == FT_Err_Unknown_File_Format)
    {
        Debug::emitir(Debug::Erro, "FreeType Fonte");
    }

    FT_Set_Pixel_Sizes(face, 0, resolucao);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
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

void BubbleUI::Widgets::Texto::atualizar(float deltaTime)
{
    if (!corpo_do_texto)
        corpo_do_texto = new Formas::Rect(pai->obtCtx());

}

void BubbleUI::Widgets::Texto::renderizar()
{
    float x = 0.0f;
    float y = 0.0f;

    for (char c : label) {
        pai->obtCtx()->shader.use();
        pai->obtCtx()->shader.setBool("texto", true);
        pai->obtCtx()->shader.setVec3("cor_texto", 1, 1, 1);
        pai->obtCtx()->shader.setInt("textura", 0);
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x;
        float ypos = y - (ch.Size.y - ch.Bearing.y);
        float w = ch.Size.x;
        float h = ch.Size.y;

        // Atualiza o retângulo do corpo_do_texto para o caractere
        corpo_do_texto->defPos({ xpos + pai->obtRect().x, ypos + pai->obtRect().y });
        corpo_do_texto->defTam({ w, h });
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        corpo_do_texto->renderizar(GL_TRIANGLES);

        x += (ch.Advance >> 6); // 1/64 pixels
    }
}

// Destrutor para liberar recursos
BubbleUI::Widgets::Texto::~Texto() {
    for (auto& pair : Characters) {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}
