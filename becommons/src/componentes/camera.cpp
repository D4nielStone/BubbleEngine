#include <glad/glad.h>
#include "camera.hpp"
#include "transformacao.hpp"
#include "../nucleo/fase.hpp"
#include "../../os/janela.hpp"

void bubble::camera::desenharFB() const
{
    if (flag_fb)
    {
        glBindTexture(GL_TEXTURE_2D, textura);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportFBO.x, viewportFBO.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportFBO.x, viewportFBO.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, viewportFBO.x, viewportFBO.y);
    }
    glClearColor(ceu.r, ceu.g, ceu.b, ceu.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, viewport_ptr->x, viewport_ptr->y);
}


bubble::camera::~camera()
{
    desativarFB();
}

bubble::camera::camera(const bool orth)
    : flag_orth(orth) {
}

void bubble::camera::ativarFB()
{
    if (flag_fb) return;
    flag_fb = true;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Criando uma textura para armazenar a imagem renderizada
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anexando a textura ao framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textura, 0);

    // Criando um renderbuffer para armazenar depth e stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Verificando se o framebuffer est� completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Erro: Framebuffer n�o est� completo!" << std::endl;
    }

    // Voltando ao framebuffer padr�o
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void bubble::camera::desativarFB()
{
    if (!flag_fb) return;
    flag_fb = false;

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textura);
}

glm::mat4 bubble::camera::obtViewMatrix() {
    if (!flag_fb) viewportFBO = *viewport_ptr;
    if (!transform)
        transform = fase_atual->obterRegistro()->obter<transformacao>(meu_objeto);

    posicao = transform->posicao;
    glm::vec3 up = transform->cima;

    // Recalcular os vetores de referência
    glm::vec3 frente = glm::vec3(
        cos(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x)),
        sin(glm::radians(transform->rotacao.x)),
        sin(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x))
    );
    forward = glm::normalize(frente);
        
    direita = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));  // Mantém eixo Y fixo
    cima = glm::normalize(glm::cross(direita, forward));

    // Atualiza os vetores na transformação
    transform->cima = cima;

    glm::vec3 alvo;
    if (transform->alvo) {
        alvo = *transform->alvo;
    }
    else {
        alvo = posicao + forward;
    }

    viewMatrix = glm::lookAt(posicao, alvo, up);
    return viewMatrix;
}

void bubble::camera::viewport(const vet2& viewp)
{
    viewportFBO = viewp;
}

glm::mat4 bubble::camera::obtProjectionMatrix() {
    vet2 viewp;
    if (flag_fb)
        viewp = viewportFBO;
    else if(viewport_ptr)
        viewp = *viewport_ptr;
    else return glm::mat4(1.f);

    if (flag_orth)
    {
        float largura = viewp.x;
        float altura = viewp.y;
        aspecto = largura / altura;
        left = -escala * aspecto;
        right = escala * aspecto;
        bottom = -escala;
        top = escala;
        projMatriz = glm::ortho(left, right, bottom, top, corte_curto, corte_longo);
    }
    else 
    {
        float largura = viewp.x;
        float altura = viewp.y;
        aspecto = largura / altura;
        projMatriz = glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
    }
    return projMatriz;
}

bubble::raio bubble::camera::pontoParaRaio(vet2 screenPoint) const 
{
    glm::vec3 worldSpaceDirection = telaParaMundo(screenPoint, 0.0f);

    raio ray{};
    ray.origem = posicao;
    ray.direcao = glm::normalize(worldSpaceDirection);

    return ray;
}

glm::vec3 bubble::camera::telaParaMundo(const vet2 &screenPoint, float profundidade) const
{
    float ndcX = (2.0f * screenPoint.x) / viewportFBO.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPoint.y) / viewportFBO.y;
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, profundidade, 1.0f);

    glm::vec4 eyeCoords = glm::inverse(projMatriz) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    glm::vec4 worldCoords = glm::inverse(viewMatrix) * eyeCoords;
    return glm::normalize(glm::vec3(worldCoords));
}

void bubble::camera::mover(glm::vec3 &pos)
{
    if (!transform)
        transform = fase_atual->obterRegistro()->obter<transformacao>(meu_objeto);

    

    // Atualiza a posição com base na entrada
    transform->posicao += forward * pos.z;  // Move para frente/trás
    transform->posicao += direita * pos.x;  // Move para os lados
    transform->posicao += cima * pos.y;     // Move para cima/baixo
}