#pragma once
#include "src/comum/componente.h"
#include "src/arquivadores/shader.h"
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
        class Transformador : public Bubble::Comum::Componente {
        private:
            glm::vec3 posicao;
            glm::vec3 rotacao; // Euler angles
            glm::vec3 escala;
            Shader shader;
        public:
            Transformador()
                : posicao(0.f, 0.f, 0.f), rotacao(0.0f), escala(0.5f) {
                Nome = "Transformador";
            }

            void atualizar() override {
                shader.setMat4("model", glm::value_ptr(obterMatrizGlobal()));
            }
            void configurar() override {
                std::cout << ">> Transformador configurado\n";
            }
            glm::mat4 obterMatrizGlobal() const {
                glm::mat4 matriz_de_transformacao = glm::translate(glm::mat4(1.0f), posicao);
                glm::mat4 matriz_de_rotacao = glm::toMat4(glm::quat(rotacao));
                glm::mat4 matriz_de_escala = glm::scale(glm::mat4(1.0f), escala);

                return matriz_de_transformacao * matriz_de_rotacao * matriz_de_escala;
            }

            glm::vec3 obterPosicao() const { return posicao; }
            glm::vec3 obterRotacao() const { return rotacao; }
            glm::vec3 obterEscala() const { return escala; }

            void definirPosicao(const glm::vec3& newPosition) { posicao = newPosition; }
            void definirRotacao(const glm::vec3& newRotation) { rotacao = newRotation; }
            void definirEscla(const glm::vec3& newScale) { escala = newScale; }
            void Rotacionar(const glm::vec3& newRotation) { rotacao += newRotation; }

            void definirShader(Shader& s) { shader = s; };

        };
    }
}