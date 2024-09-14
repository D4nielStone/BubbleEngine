#include "imageloader.hpp"
#include "src/depuracao/debug.hpp"
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
        data = nullptr; // Precau��o para evitar acesso duplo
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

    // Obt�m as dimens�es da imagem  
    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    channels = 4;  // RGBA  

    // Aloca mem�ria para os dados da imagem  
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
    flipVertical();

    // Finaliza o FreeImage  
    FreeImage_DeInitialise();
}
GLFWimage ImageLoader::converterParaGlfw()
{
    GLFWimage image = {};
    if (!carregado) {
        Debug::emitir(Debug::Tipo::Erro, "tentativa de converter imagem n�o carregada para GLFW");
        return image;
    }

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

unsigned int Bubble::Arquivadores::TextureFromFile(const std::string& directory, int* width_ptr , int* height_ptr) {
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    ImageLoader img(directory.c_str());
    auto data = img.obterDados();
    nrComponents = img.getChannels();
    width = img.getWidth();
    height = img.getHeight();
    if (width_ptr) *width_ptr = width;
    if (height_ptr) *height_ptr = height;
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
        std::cerr << "Failed to load texture: " << directory << std::endl;
    }

    return textureID;
}

unsigned int Bubble::Arquivadores::TextureFromFile(unsigned char* data, unsigned int width, unsigned int height, int nrComponents) {
    Debug::emitir("TextureFromFile", "Tentando carregar textura embutida");

    unsigned int textureID;
    glGenTextures(1, &textureID);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, 500, 500, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Debug::emitir("TextureFromFile", "Textura embutida carregada com sucesso");
    }
    else {
        Debug::emitir("TextureFromFile", "Falha ao carregar a textura embutida");
        std::cerr << "Failed to load texture: Textura embutida" << std::endl;
    }

    return textureID;
}

TextureLoader& TextureLoader::getInstance()
{
    static TextureLoader instance;
    return instance;
}

GLuint Bubble::Arquivadores::TextureLoader::carregarTextura(const std::string& caminho, int *width, int *height)
{
    // Verificar se a textura j� foi carregada
    if (texturasCarregadas.find(caminho) != texturasCarregadas.end()) {
        return texturasCarregadas[caminho]; // Retorna ID da textura j� carregada
    }

    // Carregar nova textura
    GLuint id = Bubble::Arquivadores::TextureFromFile(caminho.c_str(), width, height);
    texturasCarregadas[caminho] = id; // Armazena o ID da textura no mapa

    return id;
}

GLuint Bubble::Arquivadores::TextureLoader::carregarAiTexture(const aiTexture* texture)
{
    GLuint ID{};
    if (texture) {
        FIMEMORY* fiMemory = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(texture->pcData), texture->mWidth);
        FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(fiMemory);

        if (format != FIF_UNKNOWN) {
            FIBITMAP* dib = FreeImage_LoadFromMemory(format, fiMemory);
            if (dib) {
                BYTE* data = FreeImage_GetBits(dib);
                unsigned int width = FreeImage_GetWidth(dib);
                unsigned int height = FreeImage_GetHeight(dib);
                unsigned int bpp = FreeImage_GetBPP(dib);
                int numChannels = bpp / 8;

                // Usar GerenciadorDeTexturas para carregar a textura
                ID = TextureFromFile(data, width, height, numChannels);

                FreeImage_Unload(dib);
            }
            else {
                Debug::emitir("Error", "Failed to load texture from memory.");
            }
        }
        FreeImage_CloseMemory(fiMemory);
    }
    return ID;
}
