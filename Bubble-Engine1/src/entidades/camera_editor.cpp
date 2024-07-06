#include "camera_editor.h"

namespace Bubble {
	namespace Entidades {
		CameraEditor::CameraEditor() {
            transformacao = std::make_shared<Componentes::Transformacao>();
            FOV = 45.0f;
            aspecto = 4 / 3;
            zNear = 0.1;
            zFar = 100.f;
        }
        CameraEditor::CameraEditor(float fov, float aspect, float znear, float zfar)
        {
            transformacao = std::make_shared<Componentes::Transformacao>();
            FOV = fov;
            aspecto = aspect;
            zNear = znear;
            zFar = zfar;
        }
		void CameraEditor::atualizar(float dt)
		{
            matrizProjecao = glm::perspective(
                glm::radians(FOV),
                aspecto,
                zNear,
                zFar
            );

            if (transformacao) {
                glm::vec3 posicaoCamera = transformacao->obterPosicao();
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
		void CameraEditor::configurar()
		{
            Camera::configurar();
		}
	}
}
