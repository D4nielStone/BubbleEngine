// Copyright (c) 2024 Daniel Oliveira

#pragma once
#include <glad/glad.h>
#include "becommons.hpp"
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <ft2build.h>
#include <src/util/includes.hpp>
#include FT_FREETYPE_H

namespace Bubble
{
    namespace Arquivadores
    {
        // Estrutura base da letra
        struct BECOMMONS_DLL_API Character
        {
            unsigned int TextureID;  // ID handle of the glyph texture
            glm::ivec2   Size;       // Size of glyph
            glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
            unsigned int Advance;    // Offset to advance to next glyph
        };


        class BECOMMONS_DLL_API GerenciadorDeFontes
        {
        public:
            static GerenciadorDeFontes& obterInstancia();

            // Inicializa o FreeType
            GerenciadorDeFontes();

            // Destrutor
            ~GerenciadorDeFontes();

            // Carrega uma fonte com a resolução especificada
            void carregarFonte(const std::string& nome_da_fonte, unsigned int resolucao);

            // Obtém um ponteiro para os caracteres de uma fonte já carregada
            const std::map<char32_t, Character>* obterCaracteres(const std::string& nome_da_fonte) const;

        private:
            FT_Library ft; // FreeType library
            std::map<std::string, std::map<char32_t, Character>> fontes; // Mapa para armazenar as fontes carregadas
        };
    }
}
