#pragma once
#include "src/comum/componente.hpp"
#include "src/depuracao/debug.hpp"
#include "becommons.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Bubble {
    namespace Componentes {
        enum Estado {
            ESTATICO,
            DINAMICO
        };
        class BECOMMONS_DLL_API Transformacao : public Bubble::Comum::Componente {
        private:
            glm::vec3 posicao;
            glm::vec3 escala;
            glm::mat4 matriz_de_modelo;
            glm::quat rotacao;
        public:
            Transformacao();

            void atualizar() override;
            void configurar() override;
            float* obterMatrizGlobal() const;
            glm::mat4 obterMatriz() const;
            rapidjson::Value serializar(rapidjson::Document* doc) const override;

            glm::vec3 obterPosicao() const;
            glm::quat obterRotacao() const;
            glm::vec3 obterEscala() const;

            void definirPosicao(const glm::vec3& newPosition);
            void definirRotacao(const glm::quat& newRotation);
            void definirEscala(const glm::vec3& newScale);

            void Move(glm::vec3 pos);
            void Rotacionar(const float x, const float y, const float z);
            glm::vec3 obterDirecao() const;

            void decomporMatriz(glm::vec3* position, glm::vec3* rotation, glm::vec3* scale) const;
            void comporMatriz(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

            void definirMatriz(glm::mat4 matriz_nova);

            Estado estado = DINAMICO;
        };
    }
}