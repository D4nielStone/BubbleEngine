#pragma once
#include "src/comum/componente.h"
#include "includes.h"
#include <glad/glad.h>
#include "src/depuracao/debug.h"

namespace Bubble{
    namespace Componentes {
        class Renderizador : public Bubble::Comum::Componente {
        private:
            Vertex mVertex;
            Material mMaterial;
            GLuint VAO, EBO, VBO;
            void configurarBuffers() {
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);

                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                // Calculate the total size needed for vertices, normals, and uvs
                size_t vertexSize = mVertex.vertices.size() * sizeof(float);
                size_t normalSize = mVertex.normals.size() * sizeof(float);
                size_t uvSize = mVertex.uvs.size() * sizeof(float);
                size_t totalSize = vertexSize + normalSize + uvSize;

                // Allocate buffer with total size
                glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

                // Fill buffer with vertices data
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &mVertex.vertices[0]);

                // Fill buffer with normals data if available
                if (!mVertex.normals.empty()) {
                    glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &mVertex.normals[0]);
                }

                // Fill buffer with uvs data if available
                if (!mVertex.uvs.empty()) {
                    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, &mVertex.uvs[0]);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertex.indices.size() * sizeof(unsigned int), &mVertex.indices[0], GL_STATIC_DRAW);

                // Position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal attribute
                if (!mVertex.normals.empty()) {
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
                    glEnableVertexAttribArray(1);
                }

                // UV attribute
                if (!mVertex.uvs.empty()) {
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
                    glEnableVertexAttribArray(2);
                }

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            void desenharModelo() {
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertex.indices.size()), GL_UNSIGNED_INT, 0);
            }
            void atualizarCores() {
                if (shader)
                    shader->use();
                    shader->setVec3("objectColor", mMaterial.difusa.r, mMaterial.difusa.g, mMaterial.difusa.b);
            }
        public:
            Renderizador() {
                Nome = "Renderizador";
                Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
            };
            Renderizador(Vertex vertex, Material materials) : mVertex(vertex), mMaterial(materials) {
                Nome = "Renderizador";
                Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
            };
            Renderizador(Vertex vertex) : mVertex(vertex) {
                Nome = "Renderizador";
                Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
            };
            void configurar() override {
                configurarBuffers();
                Debug::emitir(Debug::Tipo::Mensagem, "Renderizador configurado");
            }
            void atualizar(float deltaTime) override {
                atualizarCores();
                desenharModelo();
            }
        };
    }
}