#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H
#include "src/comum/componente.h"
#include "src/depuracao/debug.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Bubble {
    namespace Componentes {
        enum Estado
        {
            ESTATICO,
            DINAMICO
        };
        class Transformacao : public Bubble::Comum::Componente {
        private:
            glm::vec3 posicao;
            glm::vec3 escala;
            glm::mat4 matriz_de_modelo;
            glm::quat rotacao;
        public:
            Transformacao()
                : matriz_de_modelo(glm::mat4(1.f)), posicao(0.f, 0.f, 0.f), rotacao(1.0f, 0, 0, 0), escala(1.0f) {
                Nome = "Transformacao";
            }

            void atualizar(float deltaTime) override {
                if (shader && estado == DINAMICO) {
                    shader->use();

                    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(matriz_de_modelo)));
                    shader->setMat4("model", glm::value_ptr(matriz_de_modelo));
                    shader->setMat3("normalMatrix", glm::value_ptr(normalMatrix));
                }
            }
            void configurar() override {
                glm::translate(matriz_de_modelo, posicao);
                matriz_de_modelo *= glm::toMat4(rotacao);
                glm::scale(matriz_de_modelo, escala);
                Debug::emitir(Debug::Tipo::Mensagem, "Transformacao configurada");
            }
            float* obterMatrizGlobal() const {
                return (float*)glm::value_ptr(matriz_de_modelo);
            }
            rapidjson::Value serializar(rapidjson::Document* doc) override
            {
                rapidjson::Value obj(rapidjson::kObjectType);
                rapidjson::Value positionarr(rapidjson::kArrayType);

                positionarr.PushBack(rapidjson::Value().SetFloat(posicao.x), doc->GetAllocator());
                positionarr.PushBack(rapidjson::Value().SetFloat(posicao.y), doc->GetAllocator());
                positionarr.PushBack(rapidjson::Value().SetFloat(posicao.z), doc->GetAllocator());

                obj.AddMember("posicao", positionarr, doc->GetAllocator());

                rapidjson::Value rotacaoarr(rapidjson::kArrayType);

                rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.x), doc->GetAllocator());
                rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.y), doc->GetAllocator());
                rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.z), doc->GetAllocator());

                obj.AddMember("rotacao", rotacaoarr, doc->GetAllocator());

                rapidjson::Value escalaarr(rapidjson::kArrayType);

                escalaarr.PushBack(rapidjson::Value().SetFloat(escala.x), doc->GetAllocator());
                escalaarr.PushBack(rapidjson::Value().SetFloat(escala.y), doc->GetAllocator());
                escalaarr.PushBack(rapidjson::Value().SetFloat(escala.z), doc->GetAllocator());

                obj.AddMember("escala", escalaarr, doc->GetAllocator());
                return obj;
            }

            glm::vec3 obterPosicao() const { return posicao; }
            glm::quat obterRotacao() const { return rotacao; }
            glm::vec3 obterEscala() const { return escala; }

            void definirPosicao(const glm::vec3& newPosition) { posicao = newPosition; }
            void definirRotacao(const glm::quat& newRotation) { rotacao = newRotation; }
            void definirEscala(const glm::vec3& newScale) { escala = newScale; }

            void Move(glm::vec3 pos)
            {
                posicao += pos;
                glm::translate(matriz_de_modelo, posicao);
            }

            void Rotacionar(const float x, const float y, const float z) {
                glm::quat quaternionRotation = glm::quat(glm::radians(glm::vec3(x, y, z)));

                rotacao = quaternionRotation * rotacao;

                rotacao = glm::normalize(rotacao);
                matriz_de_modelo *= glm::toMat4(rotacao);
            }

            glm::vec3 obterDirecao() const {
                // Vetor de direção padrão (para frente)
                glm::vec3 direcaoPadrao = glm::vec3(0.0f, 0.0f, -1.0f);
                // Transformar o vetor de direção pelo quaternion de rotação
                return rotacao * direcaoPadrao;
            }
            // Função para decompor uma matriz
            void decomporMatriz(glm::vec3* position, glm::vec3* rotation, glm::vec3* scale) {
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::quat orientation;
                glm::decompose(matriz_de_modelo, *scale, orientation, *position, skew, perspective);
                *rotation = glm::eulerAngles(orientation);
                *rotation = glm::degrees(*rotation);

            }
            
            // Função para recompor uma matriz
            void comporMatriz(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
                matriz_de_modelo = glm::translate(glm::mat4(1.0f), position);
                matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                matriz_de_modelo = glm::scale(matriz_de_modelo, scale);
            }
            Estado estado = DINAMICO;
        };
    }
}
#endif