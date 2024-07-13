#include "camera_editor.h"
#include "src/depuracao/debug.h"

namespace Bubble {
	namespace Entidades {
		CameraEditor::CameraEditor(Bubble::Inputs::GameInputs* input) : alvoCamera(0,0,0), inputs(input) {
            transformacao = std::make_shared<Componentes::Transformacao>();
            FOV = 45.0f;
            aspecto = 4 / 3;
            zNear = 0.1;
            zFar = 300.f;
        }
        CameraEditor::CameraEditor() : alvoCamera(0,0,0){
            transformacao = std::make_shared<Componentes::Transformacao>();
            FOV = 45.0f;
            aspecto = 4 / 3;
            zNear = 0.1;
            zFar = 300.f;
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
            if (inputs && inputs->getInputMode() == InputMode::Editor)
            {
                if (inputs->isKeyPressed(Key::W))
                {
                    transformacao->Move(0, 0, dt * 10);
                }
            }
            matrizProjecao = glm::perspective(
                glm::radians(FOV),
                aspecto,
                zNear,
                zFar
            );

            if (transformacao) {
                glm::vec3 posicaoCamera = transformacao->obterPosicao();
                glm::vec3 vetorCima(0, 1, 0);
                timea += dt;
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
                Debug::emitir(Debug::Tipo::Erro,"meuObjeto não está definido");
            }
		}
		void CameraEditor::configurar()
		{
            Camera::configurar();
		}
        CameraEditor::~CameraEditor()
        {
            Camera::~Camera();
        }
        void CameraEditor::olharPara(glm::vec3 pov)
        {
            glm::vec3 alvoCamera = pov;
        }
	}
}
