#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "becore.h"

namespace Bubble {
    namespace Arquivadores {
        class BECORE_DLL_API ImageLoader {
        public:
            ImageLoader(const std::string& filepath);
            ~ImageLoader();

            int getWidth() const { return width; }
            int getHeight() const { return height; }
            int getChannels() const { return channels; }
            unsigned char* obterDados() const { return data; }
            GLFWimage converterParaGlfw();
            bool carregado;
        private:
            void flipVertical();
            const char* path;
            int width, height, channels;
            unsigned char* data;
        };
    }
}
#endif