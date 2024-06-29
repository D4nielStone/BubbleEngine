// ImageLoader.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "imageloader.h"
#include <iostream>

Bubble::Arquivadores::ImageLoader::ImageLoader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr)
{
    data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
    }
}

Bubble::Arquivadores::ImageLoader::~ImageLoader() {
    if (data) {
        stbi_image_free(data);
    }
}
