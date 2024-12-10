#pragma once
#include <string>
#include "unordered_map"
#include "becommons.hpp"
#include <freeimage.h>
#include <assimp/scene.h>
#include <glad/glad.h>

struct GLFWimage;
class  ImageLoader
{
public:
    ImageLoader();
    ImageLoader(const std::string& filepath);
    ~ImageLoader();

    static ImageLoader& getInstance();
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    unsigned char* obterDados() const;
    GLFWimage converterParaGlfw();
    bool carregado;
private:
    void flipVertical();
    void carregarImagem(const std::string& filepath);
    void embutida(BYTE* data, const unsigned int tamanho);
    const char* path;
    int width, height, channels;
    unsigned char* data;
};
unsigned int TextureFromFile(const std::string& directory, int* width = nullptr, int* height = nullptr);
unsigned int TextureFromFile(const std::string& directory, GLuint tipo_textura);
unsigned int TextureFromFile(unsigned char* data, unsigned int width, unsigned int height, int format);
class  TextureLoader
{
public:
    static TextureLoader& getInstance();
    GLuint carregarTextura(const std::string& caminho, int* width = nullptr, int* height = nullptr);
    GLuint carregarTextura(const std::string& caminho, GLuint tipo_textura);
    GLuint carregarSkybox(const char*, std::vector<std::string> faces);
    GLuint carregarAiTexture(const aiTexture* texture);
private:
    std::unordered_map<std::string, GLuint> texturasCarregadas;

    // Construtor privado para Singleton
    TextureLoader() {}

    // Desabilitar cópia e atribuição
    TextureLoader(const TextureLoader&) = delete;
    void operator=(const TextureLoader&) = delete;
};