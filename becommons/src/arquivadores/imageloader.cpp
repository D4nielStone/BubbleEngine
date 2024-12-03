
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "imageloader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "assets/imagems_na_memoria.hpp"
#include <map>
#include <filesystem>
#include <iostream>

using namespace Bubble::Arquivadores;

const std::map<const std::string, std::pair<BYTE*, const unsigned int>> imagems_memoria
{
    {"icon.ico", std::pair(icon_png, icon_png_len)},
    {"banner.png", std::pair(banner_png, banner_png_len)},
    {"folder.png", std::pair(folder_png, folder_png_len)}
};

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
        //delete[] data;
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
    //auto it = imagens_carregadas.find(filepath);
    //if (it != imagens_carregadas.end())
    //{
    //    data = it->second->data;
    //    channels = it->second->channels;
    //    width = it->second->width;
    //    height = it->second->height;
    //    carregado = true;
    //    return;
    //}
    // Debug::emitir("IMAGE_LOADER", "nova imagem:" + filepath);
     // Inicializa o FreeImage  
    FreeImage_Initialise();

    const std::string nome_arquivo = std::filesystem::path(filepath).filename().string();
    if (imagems_memoria.find(nome_arquivo) != imagems_memoria.end())
    {
        embutida(imagems_memoria.at(nome_arquivo).first, imagems_memoria.at(nome_arquivo).second);
        return;
    }
    // Determina o formato da imagem  
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(path);
    }

    if (format == FIF_UNKNOWN) {
        return;
    }

    // Carrega a imagem  
    FIBITMAP* bitmap = FreeImage_Load(format, path);
    if (!bitmap) {
        return;
    }

    // Converte a imagem para 32 bits  
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted) {
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
    flipVertical();

    // Finaliza o FreeImage  
    FreeImage_DeInitialise();
}
void ImageLoader::embutida(BYTE* data, const unsigned int tamanho) 
{
    // Cria um stream de memória com o buffer da imagem
    FIMEMORY* memoryStream = FreeImage_OpenMemory(data, tamanho);
    if (!memoryStream) {
        fprintf(stderr, "Erro ao criar o stream de memória.");
        return;
    }

    // Detecta o formato da imagem no stream de memória
    FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(memoryStream, 0);
    if (format == FIF_UNKNOWN) {
        fprintf(stderr, "Formato de imagem desconhecido.");
        FreeImage_CloseMemory(memoryStream);
        return;
    }

    // Carrega a imagem do stream de memória
    FIBITMAP* bitmap_ = FreeImage_LoadFromMemory(format, memoryStream, 0);

    if (!bitmap_) {
        fprintf(stderr, "Erro ao carregar a imagem.");
        FreeImage_CloseMemory(memoryStream);
        return;
    }
    // Converte a imagem para 32 bits  
    FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(bitmap_);
    // Obtém as propriedades da imagem (largura, altura, canais, dados)
    width = FreeImage_GetWidth(bitmap);     // Largura da imagem
    height = FreeImage_GetHeight(bitmap);   // Altura da imagem
    channels = FreeImage_GetBPP(bitmap)/8; // Canais de cor (assumindo 8 bits por canal)

    // Aloca memória para os dados da imagem  
    this->data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(bitmap);

    // Copia os dados e corrige a ordem dos canais (ARGB para RGBA)  
    for (int i = 0; i < width * height; ++i) {
        this->data[i * 4 + 0] = bits[i * 4 + 2]; // R  
        this->data[i * 4 + 1] = bits[i * 4 + 1]; // G  
        this->data[i * 4 + 2] = bits[i * 4 + 0]; // B  
        this->data[i * 4 + 3] = bits[i * 4 + 3]; // A  
    }

    // Libera os recursos
    FreeImage_Unload(bitmap);
    FreeImage_CloseMemory(memoryStream);

    // Indica que a imagem foi carregada com sucesso  
    carregado = true;
    imagens_carregadas.insert(std::pair(path, this));
    flipVertical();

    FreeImage_DeInitialise();

    return;
}
GLFWimage ImageLoader::converterParaGlfw()
{
    GLFWimage image = {};
    if (!carregado) {
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

    }
    else {
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
    // Verificar se a textura já foi carregada
    if (texturasCarregadas.find(caminho) != texturasCarregadas.end()) {
        return texturasCarregadas[caminho]; // Retorna ID da textura já carregada
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
            }
        }
        FreeImage_CloseMemory(fiMemory);
    }
    return ID;
}
