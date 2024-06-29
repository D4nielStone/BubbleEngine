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
            float FoV = 45.0f;
            Camera() {};
            void configurar() override;
            void atualizar(float deltaTime) override;
        private:
            glm::mat4 matrizProjecao;
            glm::mat4 matrizVisualizacao;
        };
    }
}

#endif //!CAMERA_H