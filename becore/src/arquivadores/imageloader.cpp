#include "imageloader.h"
#include "iostream"
#include "src/depuracao/debug.h"
#include "freeimage.h"

Bubble::Arquivadores::ImageLoader::ImageLoader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr), path(filepath.c_str()), carregado(false)
{
    // Inicializa o FreeImage
    FreeImage_Initialise();

    // Determina o formato da imagem
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(path);
    }

    if (format == FIF_UNKNOWN) {
        Debug::emitir(Debug::Tipo::Erro, std::string("erro ao determinar formato da imagem: " + filepath).c_str());
        return;
    }

    // Carrega a imagem
    FIBITMAP* bitmap = FreeImage_Load(format, path);
    if (!bitmap) {
        Debug::emitir(Debug::Tipo::Erro, std::string("erro ao carregar imagem: " + filepath).c_str());
        return;
    }

    // Converte a imagem para 32 bits
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted) {
        Debug::emitir(Debug::Tipo::Erro, std::string("erro ao converter a imagem para 32 bits: " + filepath).c_str());
        return;
    }

    // Obtém as dimensões da imagem
    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    channels = 4;  // RGBA

    // Copia os dados da imagem
    data = new unsigned char[width * height * channels];
    memcpy(data, FreeImage_GetBits(converted), width * height * channels);

    // Flipando a imagem no eixo Y
    flipVertical();

    FreeImage_Unload(converted);

    // Indica que a imagem foi carregada com sucesso
    carregado = true;

    // Finaliza o FreeImage
    FreeImage_DeInitialise();
}

Bubble::Arquivadores::ImageLoader::~ImageLoader()
{
    if (data) {
        delete[] data;
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

GLFWimage Bubble::Arquivadores::ImageLoader::converterParaGlfw()
{
    GLFWimage image = {};
    if (!carregado)
        return image;

    image.width = width;
    image.height = height;
    image.pixels = data;
    return image;
}
