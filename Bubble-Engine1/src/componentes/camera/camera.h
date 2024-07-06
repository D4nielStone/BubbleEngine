#ifndef CAMERA_H
#define CAMERA_H
#include "src/comum/componente.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Bubble {
    namespace Componentes {
        class Camera : public Bubble::Comum::Componente
        {
        public:
            Camera();
            void configurar() override;
            void atualizar(float deltaTime) override;
            void atualizarAspecto(float aspect);
        protected:
            float FOV, aspecto, zFar, zNear;
            glm::mat4 matrizProjecao;
            glm::mat4 matrizVisualizacao;
        };
    }
}

#endif //!CAMERA_H