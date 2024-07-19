#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include <string>
#include "unordered_map"
#include "becore.h"
class GLFWimage;
namespace Bubble {
    namespace Arquivadores {
        class BECORE_DLL_API ImageLoader {
        public:
            ImageLoader(){}
            ImageLoader(const std::string& filepath);
            ~ImageLoader();

            int getWidth() const { return width; }
            int getHeight() const { return height; }
            int getChannels() const { return channels; }
            unsigned char* obterDados() const { return data; }
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
    }
}
#endif