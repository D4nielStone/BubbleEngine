#include "camera.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "src/entidades/entidade.h"
#include "src/depuracao/debug.h"

Bubble::Componentes::Camera::Camera() : FOV(45.0f), aspecto(4 / 3), zNear(0.1), zFar(100) {}
Bubble::Componentes::Camera::~Camera() {}

void Bubble::Componentes::Camera::configurar() {
    // Framebuffer configuration
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create a texture to attach to the framebuffer
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Debug::emitir(Debug::Tipo::Erro, "Framebuffer incompleto");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Debug::emitir(Debug::Tipo::Mensagem, "Camera configurada");
}
void Bubble::Componentes::Camera::atualizarAspecto(float aspect)
{
    aspecto = aspect;
}
void Bubble::Componentes::Camera::atualizar(float deltaTime) {
    matrizProjecao = glm::perspective(
        glm::radians(FOV),
        4.0f / 3.0f,
        0.1f,
        100.0f
    );

    if (meuObjeto) {
        glm::vec3 posicaoCamera = meuObjeto->obterTransformacao()->obterPosicao();
        glm::vec3 alvoCamera(0, 0, 0);
        glm::vec3 vetorCima(0, 1, 0);

        // Calculate view matrix
        matrizVisualizacao = glm::lookAt(posicaoCamera, alvoCamera, vetorCima);

        // Assuming 'shader' is a valid pointer or reference to a shader object
        if (shader) {
            shader->use();
            shader->setMat4("projection", glm::value_ptr(matrizProjecao));
            shader->setMat4("view", glm::value_ptr(matrizVisualizacao));
        }
    }
    else {
        std::cerr << "Erro: meuObjeto não está definido.\n";
    }
}
