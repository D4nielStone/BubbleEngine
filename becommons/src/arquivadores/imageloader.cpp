#include "imageloader.hpp"
#include "src/depuracao/debug.hpp"
#include <freeimage.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

using namespace Bubble::Arquivadores;

ImageLoader::ImageLoader()
{
}
ImageLoader::ImageLoader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr), path(filepath.c_str()), carregado(false)
{
    carregarImagem(filepath);
}
ImageLoader::~ImageLoader()
{
    if (data) {
        delete[] data;
        data = nullptr; // Precaução para evitar acesso duplo
    }
}
void ImageLoader::flipVertical()
{
    int rowSize = width * channels;
    unsigned char* tempRow = new unsigned char[rowSize];
    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = data + y * rowSize;
        unsigned char* row2 = data + (height - 1 - y) * rowSize;
        memcpy(tempRow, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, tempRow, rowSize);
    }
    delete[] tempRow;
}
void ImageLoader::carregarImagem(const std::string& filepath)
{
    auto it = imagens_carregadas.find(filepath);
    if (it != imagens_carregadas.end())
    {
        data = it->second->data;
        channels = it->second->channels;
        width = it->second->width;
        height = it->second->height;
        carregado = true;
        return;
    }
    Debug::emitir("IMAGE_LOADER", "nova imagem:" + filepath);
    // Inicializa o FreeImage  
    FreeImage_Initialise();

    // Determina o formato da imagem  
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(path);
    }

    if (format == FIF_UNKNOWN) {
        Debug::emitir(Debug::Tipo::Erro, std::string(std::string("erro ao determinar formato da imagem: ") + filepath).c_str());
        return;
    }

    // Carrega a imagem  
    FIBITMAP* bitmap = FreeImage_Load(format, path);
    if (!bitmap) {
        Debug::emitir(Debug::Tipo::Erro, std::string(std::string("erro ao carregar imagem: ") + filepath).c_str());
        return;
    }

    // Converte a imagem para 32 bits  
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted) {
        Debug::emitir(Debug::Tipo::Erro, std::string(std::string("erro ao converter a imagem para 32 bits: ") + filepath).c_str());
        return;
    }

    // Obtém as dimensões da imagem  
    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    channels = 4;  // RGBA  

    // Aloca memória para os dados da imagem  
    data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(converted);

    // Copia os dados e corrige a ordem dos canais (ARGB para RGBA)  
    for (int i = 0; i < width * height; ++i) {
        data[i * 4 + 0] = bits[i * 4 + 2]; // R  
        data[i * 4 + 1] = bits[i * 4 + 1]; // G  
        data[i * 4 + 2] = bits[i * 4 + 0]; // B  
        data[i * 4 + 3] = bits[i * 4 + 3]; // A  
    }


    FreeImage_Unload(converted);

    // Indica que a imagem foi carregada com sucesso  
    carregado = true;
    imagens_carregadas.insert(std::pair(path, this));

    // Finaliza o FreeImage  
    FreeImage_DeInitialise();
}
GLFWimage ImageLoader::converterParaGlfw()
{
    GLFWimage image = {};
    if (!carregado) {
        Debug::emitir(Debug::Tipo::Erro, "tentativa de converter imagem não carregada para GLFW");
        return image;
    }

    flipVertical();

    image.width = width;
    image.height = height;
    image.pixels = data;
    return image;
}
int ImageLoader::getWidth() const
{
    return width;
}
int ImageLoader::getHeight() const
{
    return height;
}
int ImageLoader::getChannels() const
{
    return channels;
}
unsigned char* ImageLoader::obterDados() const
{
    return data;
}

unsigned int Bubble::Arquivadores::TextureFromFile(const char* path, const std::string& directory) {
    // Concatena o diretório e o caminho do arquivo para obter o caminho completo
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    Debug::emitir("TextureFromFile", filename);
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    ImageLoader img(filename.c_str());
    auto data = img.obterDados();
    nrComponents = img.getChannels();
    width = img.getWidth();
    height = img.getHeight();
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }

    return textureID;
}

unsigned int Bubble::Arquivadores::TextureFromFile(unsigned char* data, unsigned int width, unsigned int height)
{
    Debug::emitir("TextureFromFile", "Textura embutida");
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    unsigned int nrComponents = 4;
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "Failed to load texture: " << "Textura embutida" << std::endl;
    }

    return textureID;
}
