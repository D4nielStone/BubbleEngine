#include "camera.hpp"
#include "transformacao.hpp"
#include <src/nucleo/fase.hpp>
#include <os/janela.hpp>

bubble::camera::camera(const bool orth)
    : flag_orth(orth) {
}

glm::mat4 bubble::camera::obtViewMatrix() {
    if (!transform)
        transform = fase_atual->obterRegistro()->obter<transformacao>(meu_objeto);

    posicao = transform->posicao;
    glm::vec3 up = transform->cima;

    glm::vec3 alvo;
    if (transform->alvo) {
        alvo = *transform->alvo;
    }
    else {
        glm::vec3 frente = glm::vec3(
            cos(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x)),
            sin(glm::radians(transform->rotacao.x)),
            sin(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x))
        );
        alvo = posicao + glm::normalize(frente);
    }

    viewMatrix = glm::lookAt(posicao, alvo, up);
    return viewMatrix;
}

glm::mat4 bubble::camera::obtProjectionMatrix() {
    if (flag_orth && viewport_ptr) {
        float largura = viewport_ptr->w;
        float altura = viewport_ptr->h;
        aspecto = largura / altura;

        left = -escala * aspecto;
        right = escala * aspecto;
        bottom = -escala;
        top = escala;

        projMatriz = glm::ortho(left, right, bottom, top, corte_curto, corte_longo);
    }
    else if (viewport_ptr) {
        float largura = viewport_ptr->w;
        float altura = viewport_ptr->h;
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
    float ndcX = (2.0f * screenPoint.x) / viewport_ptr->w - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPoint.y) / viewport_ptr->h;
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, profundidade, 1.0f);

    glm::vec4 eyeCoords = glm::inverse(projMatriz) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    glm::vec4 worldCoords = glm::inverse(viewMatrix) * eyeCoords;
    return glm::normalize(glm::vec3(worldCoords));
}