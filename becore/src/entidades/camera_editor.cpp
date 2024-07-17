#include "camera_editor.h"
#include "src/depuracao/debug.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Bubble {
    namespace Entidades {
        CameraEditor::CameraEditor(Bubble::Inputs::GameInputs* input)
            : alvoCamera(0, 0, 0), inputs(input), velocidadeDeMovimento(1.0f), sensibilidadeDeRotacao(1.f),
            yaw(-90.0f), pitch(0.0f) {
            FOV = 45.0f;
            aspecto = 4.0f / 3.0f;
            zNear = 0.1f;
            zFar = 300.0f;
            transformacao = std::make_shared<Componentes::Transformacao>();
            atualizarDirecao();
        }
        CameraEditor::CameraEditor()
            : alvoCamera(0, 0, 0), velocidadeDeMovimento(1.0f), sensibilidadeDeRotacao(1.f),
            yaw(-90.0f), pitch(0.0f) {
            FOV = 45.0f;
            aspecto = 4.0f / 3.0f;
            zNear = 0.1f;
            zFar = 300.0f;
            transformacao = std::make_shared<Componentes::Transformacao>();
            atualizarDirecao();
        }

        void CameraEditor::atualizar(float dt)
        {
            if (inputs && inputs->getInputMode() == InputMode::Editor)
            {
                // Movimento
                frente = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                    sin(glm::radians(pitch)),
                    sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
                if (inputs->isKeyPressed(Key::W))
                {
                    transformacao->Move(frente * velocidadeDeMovimento);
                }
                if (inputs->isKeyPressed(Key::S))
                {
                    transformacao->Move(-frente * velocidadeDeMovimento);
                }
                if (inputs->isKeyPressed(Key::A))
                {
                    transformacao->Move(glm::normalize(glm::cross(glm::vec3(0, 1, 0), frente)) * velocidadeDeMovimento);
                }
                if (inputs->isKeyPressed(Key::D))
                {
                    transformacao->Move(glm::normalize(glm::cross(frente, glm::vec3(0, 1, 0))) * velocidadeDeMovimento);
                }

                // Rotação
                if (inputs->isKeyPressed(Key::UP))
                {
                    pitch += sensibilidadeDeRotacao;
                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    atualizarDirecao();
                }
                if (inputs->isKeyPressed(Key::DOWN))
                {
                    pitch -= sensibilidadeDeRotacao;
                    if (pitch < -89.0f)
                        pitch = -89.0f;
                    atualizarDirecao();
                }
                if (inputs->isKeyPressed(Key::LEFT))
                {
                    yaw -= sensibilidadeDeRotacao;
                    atualizarDirecao();
                }
                if (inputs->isKeyPressed(Key::RIGHT))
                {
                    yaw += sensibilidadeDeRotacao;
                    atualizarDirecao();
                }
            }

            glClearColor(ceu[0], ceu[1], ceu[2], 1);

            matrizProjecao = glm::perspective(
                glm::radians(FOV),
                aspecto,
                zNear,
                zFar
            );

            if (transformacao) {
                glm::vec3 posicaoCamera = transformacao->obterPosicao();
                glm::vec3 vetorCima(0, 1, 0);

                // Calcular a matriz de visualização
                matrizVisualizacao = glm::lookAt(posicaoCamera, posicaoCamera + frente, vetorCima);

                if (shader) {
                    shader->use();
                    shader->setMat4("projection", glm::value_ptr(matrizProjecao));
                    shader->setMat4("view", glm::value_ptr(matrizVisualizacao));
                }
            }
            else {
                Debug::emitir(Debug::Tipo::Erro, "transformacao não está definida");
            }
        }

        void CameraEditor::configurar()
        {
            Camera::configurar();
        }

        void CameraEditor::atualizarDirecao()
        {
            glm::vec3 frente;
            frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            frente.y = sin(glm::radians(pitch));
            frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            frente = glm::normalize(frente);
            transformacao->definirRotacao(frente);
        }

        CameraEditor::~CameraEditor()
        {
            Camera::~Camera();
        }

        void CameraEditor::olharPara(glm::vec3 pov)
        {
            alvoCamera = pov;
        }
    }
}
