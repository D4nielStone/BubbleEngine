/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "fonte.hpp"
#include "assets/fontes_na_memoria.hpp"
#include <filesystem>

using namespace Bubble::Arquivadores;

std::map<const std::string, std::pair<const unsigned char*, unsigned int>> fontes_memoria
{
    {"consolas.ttf", std::pair(Consolas_ttf, Consolas_ttf_len)},
    {"consolai.ttf", std::pair(consolai_ttf, consolai_ttf_len)},
    {"consola.ttf", std::pair(Consola_ttf, Consola_ttf_len)},
    {"consolaz.ttf", std::pair(consolaz_ttf, consolaz_ttf_len)},
    {"noto_sans.italic.ttf", std::pair(noto_sans_italic_ttf, noto_sans_italic_ttf_len)},
    {"noto_sans.bold.ttf", std::pair(noto_sans_bold_ttf, noto_sans_bold_ttf_len)},
    {"noto_sans.regular.ttf", std::pair(noto_sans_regular_ttf, noto_sans_regular_ttf_len)}
};

GerenciadorDeFontes::GerenciadorDeFontes()
{
    if (FT_Init_FreeType(&ft))
    {
        throw std::runtime_error("Erro ao inicializar FreeType");
    }
}

GerenciadorDeFontes& GerenciadorDeFontes::obterInstancia()
{
    static GerenciadorDeFontes instance;
    return instance;
}

GerenciadorDeFontes::~GerenciadorDeFontes()
{
    FT_Done_FreeType(ft);
}

void GerenciadorDeFontes::carregarFonte(const std::string& nome_da_fonte, unsigned int resolucao)
{
    if (fontes.find(nome_da_fonte) != fontes.end())
    {
        return; // Fonte já carregada
    }

    FT_Face face{};

    if (fontes_memoria.find(std::filesystem::path(nome_da_fonte).filename().string()) != fontes_memoria.end())
    {
        if (FT_New_Memory_Face(ft, fontes_memoria.at((std::filesystem::path(nome_da_fonte).filename().string())).first, fontes_memoria.at((std::filesystem::path(nome_da_fonte).filename().string())).second, 0, &face))
        {
            throw std::runtime_error("Erro ao carregar a fonte: " + nome_da_fonte);
        }
    }
    else
        if (std::filesystem::exists(nome_da_fonte)) {
            if (FT_New_Face(ft, nome_da_fonte.c_str(), fontes.size(), &face)) {
                throw std::runtime_error("Erro ao carregar a fonte: " + nome_da_fonte);
            }
        }
        else {
            //Fonte não encontrada
            return;
        }


    FT_Set_Pixel_Sizes(face, 0, resolucao);

    std::map<char32_t, Character> caracteres;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned int c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            throw std::runtime_error("Erro ao carregar letra: " + c);
            continue;
        }

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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        caracteres.insert(caracteres.end(), std::pair(c, character));
    }

    fontes[nome_da_fonte] = caracteres;

    FT_Done_Face(face);
}

const std::map<char32_t, Character>* GerenciadorDeFontes::obterCaracteres(const std::string& nome_da_fonte) const
{
    auto it = fontes.find(nome_da_fonte);
    if (it != fontes.end())
    {
        return &it->second;
    }
    obterInstancia().carregarFonte(nome_da_fonte, 12);
    return nullptr;
}