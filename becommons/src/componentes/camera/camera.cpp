// Copyright (c) 2024 Daniel Oliveira

#include "camera.hpp"
#include <iostream>
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "src/entidades/entidade.hpp"
#include "src/depuracao/debug.hpp"

using namespace Bubble::Componentes;

Camera::Camera() : corSolida(true), FOV(45.0f), aspecto(1.333f), zNear(0.1f), zFar(300)
{
    Nome = "Camera";
}
Camera::~Camera() {}

void Camera::configurar() {
    // Framebuffer configuration
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create a texture to attach to the framebuffer
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 700, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 700, 480);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Debug::emitir(Debug::Tipo::Erro, "Framebuffer incompleto");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Debug::emitir(Debug::Tipo::Mensagem, "Camera configurada");
    carregadov = true;
}
void Camera::atualizarAspecto(float aspect)
{
    aspecto = aspect;
}
void Camera::desenharFrame(const Vector4 &viewportRect) const
{
    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Redimensionar o texture color buffer
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportRect.w * resolucaoDoFB, viewportRect.h * resolucaoDoFB, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Redimensionar o renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportRect.w * resolucaoDoFB, viewportRect.h * resolucaoDoFB);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Check if framebuffer is still complete after resizing
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Debug::emitir(Debug::Tipo::Erro, "Framebuffer incompleto após redimensionamento");
    }

    // Update the viewport after resizing
    glViewport(0, 0, viewportRect.w, viewportRect.h);

    // Clear buffers
    glClearColor(ceu.r, ceu.g, ceu.b, ceu.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Camera::renderizar() const
{
    shader.use();
    shader.setMat4("projection", glm::value_ptr(matrizProjecao));
    shader.setMat4("view", glm::value_ptr(matrizVisualizacao));
}
void Camera::atualizar() {
    matrizProjecao = glm::perspective(
        glm::radians(FOV),
        aspecto,
        zNear,
        zFar
    );

        glm::vec3 posicaoCamera = meuObjeto->obterTransformacao()->obterPosicao();
        glm::vec3 vetorCima(0, 1, 0);

        // Calculate view matrix
        matrizVisualizacao = glm::lookAt(posicaoCamera, alvoCamera, vetorCima);

        shader.use();
        shader.setMat4("projection", glm::value_ptr(matrizProjecao));
        shader.setMat4("view", glm::value_ptr(matrizVisualizacao));
   
}
const float* Camera::obterViewMatrix()
    {
        return glm::value_ptr(matrizVisualizacao);
    }
const float* Camera::obterProjMatrix()
    {
        return glm::value_ptr(matrizProjecao);
    }
glm::mat4 Camera::obterViewMatrixMat()const
    {
        return matrizVisualizacao;
    }
glm::mat4 Camera::obterProjMatrixMat()const
    {
        return matrizProjecao;
    }
