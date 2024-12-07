
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "camera_editor.hpp"
#include "src/depuracao/debug.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

using namespace Bubble::Entidades;

CameraEditor::CameraEditor(std::shared_ptr<Bubble::Inputs::Inputs> input)
    :  inputs(input), velocidadeDeMovimento(100.f), sensibilidadeDeRotacao(100.f),
    yaw(-90.0f), pitch(0.0f) {
    FOV = 75.f;
    aspecto = 4.0f / 3.0f;
    zNear = 0.1f;
    zFar = 300.0f;
    transformacao = std::make_shared<Componentes::Transformacao>();
    atualizarDirecao();
    glm::vec3 pos = glm::vec3(transformacao->obterPosicao().x, transformacao->obterPosicao().y, transformacao->obterPosicao().z);
    matrizVisualizacao = glm::lookAt(pos, pos + frente, { 0, 1, 0 });

}
CameraEditor::CameraEditor()
    : velocidadeDeMovimento(100.f), sensibilidadeDeRotacao(100.f),
    yaw(-90.0f), pitch(0.0f) {
    FOV = 75.f;
    aspecto = 4.0f / 3.0f;
    zNear = 0.1f;
    zFar = 300.0f;
    transformacao = std::make_shared<Componentes::Transformacao>();
    atualizarDirecao();
    glm::vec3 pos = glm::vec3(transformacao->obterPosicao().x, transformacao->obterPosicao().y, transformacao->obterPosicao().z);
    matrizVisualizacao = glm::lookAt(pos, pos + frente, { 0, 1, 0 });
}
void CameraEditor::atualizar()
{
    deltaTime = glfwGetTime() - tempoPassado;
    tempoPassado = glfwGetTime();
    if (inputs && inputs->getInputMode() == Editor)
    {
        // Movimento
        frente = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
        if (inputs->isKeyPressed(Key::W))
        {
            transformacao->Move(frente * static_cast<float>(velocidadeDeMovimento * deltaTime));

            const auto p = transformacao->obterPosicao();
            const auto s = p + frente;
            // Calcular a matriz de visualização
            matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
        }
        if (inputs->isKeyPressed(Key::S))
        {
            transformacao->Move(-frente * static_cast<float>(velocidadeDeMovimento * deltaTime));

            const auto p = transformacao->obterPosicao();
            const auto s = p + frente;
            // Calcular a matriz de visualização
            matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
        }
        if (inputs->isKeyPressed(Key::A))
        {
            transformacao->Move(glm::normalize(glm::cross(glm::vec3(0, 1, 0), frente)));

            const auto p = transformacao->obterPosicao();
            const auto s = p + frente;
            // Calcular a matriz de visualização
            matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
        }
        if (inputs->isKeyPressed(Key::D))
        {
            transformacao->Move(glm::normalize(glm::cross(frente, glm::vec3(0, 1, 0))));
            const auto p = transformacao->obterPosicao();
            const auto s = p + frente;
            // Calcular a matriz de visualização
            matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });

            // Rotação
            if (inputs->isKeyPressed(Key::UP))
            {
                pitch += static_cast<double>(sensibilidadeDeRotacao) * deltaTime;
                if (pitch > 89.0f)
                    pitch = 89.0f;
                atualizarDirecao();

                const auto p = transformacao->obterPosicao();
                const auto s = p + frente;
                // Calcular a matriz de visualização
                matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
            }
            if (inputs->isKeyPressed(Key::DOWN))
            {
                pitch -= static_cast<double>(sensibilidadeDeRotacao * deltaTime);
                if (pitch < -89.0f)
                    pitch = -89.0f;
                atualizarDirecao();
                const auto p = transformacao->obterPosicao();
                const auto s = p + frente;
                // Calcular a matriz de visualização
                matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
            }
            if (inputs->isKeyPressed(Key::LEFT))
            {
                yaw -= static_cast<double>(sensibilidadeDeRotacao * deltaTime);
                atualizarDirecao();
                const auto p = transformacao->obterPosicao();
                const auto s = p + frente;
                // Calcular a matriz de visualização
                matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
            }
            if (inputs->isKeyPressed(Key::RIGHT))
            {
                yaw += sensibilidadeDeRotacao * deltaTime;
                atualizarDirecao();
                const auto p = transformacao->obterPosicao();
                const auto s = p + frente;
                // Calcular a matriz de visualização
                matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
            }
        }



        matrizProjecao = glm::perspective(
            glm::radians(FOV),
            aspecto,
            zNear,
            zFar
        );
    }
}
void CameraEditor::atualizarDirecao()
{
    frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frente.y = sin(glm::radians(pitch));
    frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    frente = glm::normalize(frente);
    transformacao->definirRotacao({ frente.x, frente.y, frente.z });
}
CameraEditor::~CameraEditor()
{
    Camera::~Camera();
}
void CameraEditor::olharPara(const glm::vec3 &pov)
{
    alvoCamera = pov;
    const auto p = transformacao->obterPosicao();
    const auto s = p + frente;
    // Calcular a matriz de visualização
    matrizVisualizacao = glm::lookAt(glm::vec3(p.x, p.y, p.z), glm::vec3(s.x, s.y, s.z), { 0, 1, 0 });
}
