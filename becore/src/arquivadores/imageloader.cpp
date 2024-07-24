#include "imageloader.h"
#include "src/depuracao/debug.h"
#include <freeimage.h>
#include "GLFW/glfw3.h"

Bubble::Arquivadores::ImageLoader::ImageLoader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr), path(filepath.c_str()), carregado(false)
{
    carregarImagem(filepath);
}

Bubble::Arquivadores::ImageLoader::~ImageLoader()
{
    if (data) {
        delete[] data;
        data = nullptr; // Precaução para evitar acesso duplo
    }
}

void Bubble::Arquivadores::ImageLoader::flipVertical()
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

void Bubble::Arquivadores::ImageLoader::carregarImagem(const std::string& filepath)
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

GLFWimage Bubble::Arquivadores::ImageLoader::converterParaGlfw()
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
