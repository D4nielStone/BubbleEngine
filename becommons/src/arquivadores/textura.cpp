#include "glad/glad.h"
#include "src/depuracao/debug.hpp"
#include "textura.hpp"

Bubble::Arquivadores::Textura::Textura(ImageLoader* img) : imagem(img)
{
	glGenTextures(1, &ID);
}

void Bubble::Arquivadores::Textura::alterarBitMap(unsigned char* data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagem->getWidth(), imagem->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Bubble::Arquivadores::Textura::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Bubble::Arquivadores::Textura::configurarBuffers()
{
    glBindTexture(GL_TEXTURE_2D, ID);

    // Defina os parâmetros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carregue a imagem na textura
    if (imagem) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagem->getWidth(), imagem->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem->obterDados());
        glGenerateMipmap(GL_TEXTURE_2D); // Gere mipmaps para a textura
    }
    else {
        Debug::emitir(Debug::Erro, "Ponteiro de ImageLoader inválido");
    }
}
