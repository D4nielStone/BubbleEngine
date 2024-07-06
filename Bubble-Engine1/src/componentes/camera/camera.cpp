#include "camera.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "src/entidades/entidade.h"

Bubble::Componentes::Camera::Camera() : FOV(45.0f), aspecto(4 / 3), zNear(0.1), zFar(100) {}

void Bubble::Componentes::Camera::configurar() {
	std::cout << ">> Camera configurada\n";
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
