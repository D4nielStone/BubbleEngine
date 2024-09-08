#pragma once
#include "src/comum/componente.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "becommons.hpp"
#include "src/util/includes.hpp"

namespace Bubble {
    namespace Componentes {
        class BECOMMONS_DLL_API Camera : public Bubble::Comum::Componente
        {
        public:
            Camera();
            ~Camera();
            void configurar() override;
            void atualizar() override;
            virtual void renderizar() const;
            void atualizarAspecto(float aspect);
            void desenharFrame(Vector4 viewportRect) const;
            const float* obterViewMatrix();
            const float* obterProjMatrix();
            float ceu[3]{ 0.7f, 0.7f, 1.0f };
            glm::mat4 obterViewMatrixMat() const;
            glm::mat4 obterProjMatrixMat() const;
            bool corSolida;
            unsigned int FBO, textureColorbuffer, rbo;
            float FOV, aspecto, zFar, zNear;
        protected:
            glm::mat4 matrizProjecao;
            glm::mat4 matrizVisualizacao;
        private:
            glm::vec3 alvoCamera = glm::vec3(0, 0, 0);
        };
    }
}