#pragma once
#include <string>
#include "unordered_map"
#include "becommons.hpp"
#include <freeimage.h>
#include <assimp/scene.h>
#include <glad/glad.h>

struct GLFWimage;
namespace Bubble {
    namespace Arquivadores {
        class BECOMMONS_DLL_API ImageLoader
        {
        public:
            ImageLoader();
            ImageLoader(const std::string& filepath);
            ~ImageLoader();

            int getWidth() const;
            int getHeight() const;
            int getChannels() const;
            unsigned char* obterDados() const;
            GLFWimage converterParaGlfw();
            bool carregado;
        private:
            std::unordered_map<std::string, ImageLoader*>imagens_carregadas;
            void flipVertical();
            void carregarImagem(const std::string& filepath);
            void embutida(unsigned char* data, const unsigned int tamanho);
            const char* path;
            int width, height, channels;
            unsigned char* data;
        };
        BECOMMONS_DLL_API unsigned int TextureFromFile(const std::string& directory, int* width = nullptr, int* height = nullptr);
        BECOMMONS_DLL_API unsigned int TextureFromFile(unsigned char* data, unsigned int width, unsigned int height, int format);

        class BECOMMONS_DLL_API TextureLoader
        {
        public:
            static TextureLoader& getInstance();
            GLuint carregarTextura(const std::string& caminho, int* width = nullptr, int* height = nullptr);
            GLuint carregarAiTexture(const aiTexture* texture);
        private:
            std::unordered_map<std::string, GLuint> texturasCarregadas;

            // Construtor privado para Singleton
            TextureLoader() {}

            // Desabilitar cópia e atribuição
            TextureLoader(const TextureLoader&) = delete;
            void operator=(const TextureLoader&) = delete;
        };
    }
}
