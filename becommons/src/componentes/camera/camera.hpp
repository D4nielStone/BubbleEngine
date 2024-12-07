#pragma once
#include "src/comum/componente.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/util/skybox.hpp"
#include "becommons.hpp"
#include "src/util/utils.hpp"

namespace Bubble {
    namespace Componentes {
        class BECOMMONS_DLL_API Camera : public Bubble::Comum::Componente
        {
        public:
            Camera();
            ~Camera();
            void configurar() override;
            void atualizar() override;
            virtual void renderizar();
            void atualizarShader();
            void atualizarAspecto(float aspect);
            void desenharFrame(const Vector4<int> &viewportRect) const;
            const float* obterViewMatrix();
            const float* obterProjMatrix();
            Color ceu{ 0.7f, 0.7f, 1.0f, 1 };
            glm::mat4 obterViewMatrixMat() const;
            glm::mat4 obterProjMatrixMat() const;
            bool corSolida{ false };
            unsigned int FBO, textureColorbuffer, rbo;
            float FOV, aspecto, zFar, zNear, resolucaoDoFB{ 1.F};
            std::string NomeCam{"no name"};
        protected:
            std::unique_ptr<Util::Skybox> skybox{ nullptr };
            glm::mat4 matrizProjecao;
            glm::mat4 matrizVisualizacao;
            glm::vec3 alvoCamera = glm::vec3(0, 0, 0);
        };
    }
}