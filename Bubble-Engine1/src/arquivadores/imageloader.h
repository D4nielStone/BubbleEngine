#define STB_IMAGE_IMPLEMENTATION
#pragma once
#include <stb_image.h>
#include <string>

namespace Bubble {
    namespace Arquivadores {
        class ImageLoader {
        public:
            ImageLoader(const std::string& filepath);
            ~ImageLoader();

            int getWidth() const { return width; }
            int getHeight() const { return height; }
            int getChannels() const { return channels; }
            unsigned char* obterDados() const { return data; }

        private:
            int width, height, channels;
            unsigned char* data;
        };
    }
}
