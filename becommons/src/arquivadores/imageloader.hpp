#pragma once
#include <string>
#include "unordered_map"
#include "becommons.hpp"

class GLFWimage;
namespace Bubble {
    namespace Arquivadores {
        class BECOMMONS_DLL_API ImageLoader {
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
            const char* path;
            int width, height, channels;
            unsigned char* data;
        };
        BECOMMONS_DLL_API unsigned int TextureFromFile(const char* path, const std::string& directory);
        BECOMMONS_DLL_API unsigned int TextureFromFile(unsigned char* data, unsigned int width, unsigned int height);
    }
}
