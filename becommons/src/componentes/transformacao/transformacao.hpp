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
#include "src/util/utils.hpp"

namespace Bubble {
    namespace Componentes {
        enum Estado {
            ESTATICO,
            DINAMICO
        };
        class BECOMMONS_DLL_API Transformacao : public Bubble::Comum::Componente {
        private:
            Vector3<float> posicao{};
            Vector3<float> escala{};
            glm::mat4 matriz_de_modelo;
            Vector3<float> rotacao{};
        public:
            Transformacao();

            void atualizar() override;
            void configurar() override;
            float* obterMatrizGlobal() const;
            glm::mat4 obterMatriz() const;
            rapidjson::Value serializar(rapidjson::Document* doc) const override;

            Vector3<float> obterPosicao() const;
            glm::vec3 obterRotacao() const;
            Vector3<float> obterEscala() const;

            void definirPosicao(const Vector3<float>& newPosition);
            void definirRotacao(const Vector3<float>& newRotation);
            void definirEscala(const Vector3<float>& newScale);

            void Move(Vector3<float> &pos);
            void Move(glm::vec3 pos);
            void Rotacionar(const float x, const float y, const float z);
            Vector3<float> obterDirecao() const;

            void decomporMatriz(glm::vec3* position, glm::vec3* rotation, glm::vec3* scale) const;
            void comporMatriz(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

            void definirMatriz(glm::mat4 matriz_nova, int factor);

            Estado estado = DINAMICO;
        };
    }
}