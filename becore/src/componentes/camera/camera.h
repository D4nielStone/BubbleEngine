#ifndef CAMERA_H
#define CAMERA_H
#include "src/comum/componente.h"
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "becore.h"

namespace Bubble {
    namespace Componentes {
        class BECORE_DLL_API Camera : public Bubble::Comum::Componente
        {
        public:
            Camera();
            ~Camera();
            void configurar() override;
            void atualizar(float deltaTime) override;
            void atualizarAspecto(float aspect);
            const float* obterViewMatrix();
            const float* obterProjMatrix();
            glm::mat4 obterViewMatrixMat() const;
            glm::mat4 obterProjMatrixMat() const;
            GLuint FBO, textureColorbuffer, rbo;
        protected:
            float FOV, aspecto, zFar, zNear;
            glm::mat4 matrizProjecao;
            glm::mat4 matrizVisualizacao;
        };
    }
}

#endif //!CAMERA_H