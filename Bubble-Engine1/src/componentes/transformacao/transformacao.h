#pragma once
#include "src/comum/componente.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>

namespace Bubble {
    namespace Componentes {
        class Transformacao : public Bubble::Comum::Componente {
        private:
            glm::vec3 posicao;
            glm::quat rotacao;
            glm::vec3 escala;
            glm::mat4 matriz_de_modelo;
        public:
            Transformacao()
                : posicao(0.f, 0.f, 0.f), rotacao(1.0f, 0, 0, 0), escala(1.0f) {
                Nome = "Transformacao";
            }

            void atualizar(float deltaTime) override {
                glm::mat4 matriz_de_transformacao = glm::translate(glm::mat4(1.0f), posicao);
                glm::mat4 matriz_de_rotacao = glm::toMat4(rotacao);
                glm::mat4 matriz_de_escala = glm::scale(glm::mat4(1.0f), escala);

                matriz_de_modelo = matriz_de_transformacao * matriz_de_rotacao * matriz_de_escala;
                if (shader) {
                    shader->use();

                    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(matriz_de_modelo)));
                    shader->setMat4("model", glm::value_ptr(matriz_de_modelo));
                    shader->setMat3("normalMatrix", glm::value_ptr(normalMatrix));

                    std::cout << ">> Transformacao atualizada\n";
                }
            }
            void configurar() override {
                std::cout << ">> Transformacao configurada\n";
            }
            glm::mat4 obterMatrizGlobal() const {
                return matriz_de_modelo;
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
            void definirRotacao(const glm::vec3& newRotation) { rotacao = newRotation; }
            void definirEscala(const glm::vec3& newScale) { escala = newScale; }
            void Rotacionar(const float x, const float y, const float z) {
                // Converta a nova rota��o de Euler para um quaternion
                glm::quat quaternionRotation = glm::quat(glm::radians(glm::vec3(x, y, z)));

                // Atualize a rota��o atual multiplicando pelo novo quaternion
                rotacao = quaternionRotation * rotacao;

                // Opcional: Normalizar o quaternion para evitar acumula��o de erro num�rico
                rotacao = glm::normalize(rotacao);
            }
        };
    }
}