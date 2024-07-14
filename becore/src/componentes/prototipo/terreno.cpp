#include "terreno.h"
#include "src/depuracao/debug.h"
#include "glm/gtc/matrix_transform.hpp"
#include "perlinnoise.h"

namespace Bubble
{
    namespace Componentes
    {
        Terreno::Terreno() : scale(50.0), width(100), depth(100) {}

        Terreno::~Terreno()
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
        void Terreno::atualizarMaterial()
        {
            shader->setVec3("objectColor", mMaterial.difusa.r, mMaterial.difusa.g, mMaterial.difusa.b);
        }
        void Terreno::configurarBuffers() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            size_t vertexSize = mVertex.vertices.size() * sizeof(float);
            size_t normalSize = mVertex.normals.size() * sizeof(float);
            size_t uvSize = mVertex.uvs.size() * sizeof(float);
            size_t totalSize = vertexSize + normalSize + uvSize;

            glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &mVertex.vertices[0]);

            if (!mVertex.normals.empty()) {
                glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &mVertex.normals[0]);
            }

            if (!mVertex.uvs.empty()) {
                glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, &mVertex.uvs[0]);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertex.indices.size() * sizeof(unsigned int), &mVertex.indices[0], GL_STATIC_DRAW);

            // Atributo de Posição
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Atributo de Normal
            if (!mVertex.normals.empty()) {
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
                glEnableVertexAttribArray(1);
            }

            // Atributo de UV
            if (!mVertex.uvs.empty()) {
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
                glEnableVertexAttribArray(2);
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Terreno::gerarHeightMap()
        {
            heightmap.resize(width, std::vector<float>(depth));

            Bubble::Util::PerlinNoise pn;

            for (size_t i = 0; i < width; i++)
            {
                for (size_t z = 0; z < depth; z++)
                {
                    double height = pn.noise(i * 0.1, z * 0.1)*2;
                    heightmap[i][z] = height;
                    mVertex.vertices.push_back(i);
                    mVertex.vertices.push_back(height);
                    mVertex.vertices.push_back(z);
                    mVertex.uvs.push_back(i / static_cast<float>(width));
                    mVertex.uvs.push_back(z / static_cast<float>(depth));
                }
            }
            for (int x = 0; x < width - 1; ++x) {
                for (int z = 0; z < depth - 1; ++z) {
                    int topLeft = x * depth + z;
                    int bottomLeft = (x + 1) * depth + z;
                    int topRight = x * depth + (z + 1);
                    int bottomRight = (x + 1) * depth + (z + 1);

                    mVertex.indices.push_back(topLeft);
                    mVertex.indices.push_back(topRight);
                    mVertex.indices.push_back(bottomLeft);

                    mVertex.indices.push_back(bottomLeft);
                    mVertex.indices.push_back(topRight);
                    mVertex.indices.push_back(bottomRight);
                }
            }
            calcularNormais();
        }

        void Terreno::desenharModelo()
        {
            if (shader)
                shader->use();
                atualizarMaterial();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertex.indices.size()), GL_UNSIGNED_INT, 0);
        }

        void Terreno::atualizar(float deltaTime)
        {
            desenharModelo();
        }

        void Terreno::configurar()
        {
            gerarHeightMap();
            configurarBuffers();
            Debug::emitir(Debug::Tipo::Mensagem, "Terreno configurado");
        }

        void Terreno::calcularNormais()
        {
            mVertex.normals.resize(mVertex.vertices.size(), 0.0f);

            for (size_t i = 0; i < mVertex.indices.size(); i += 3) {
                int index0 = mVertex.indices[i] * 3;
                int index1 = mVertex.indices[i + 1] * 3;
                int index2 = mVertex.indices[i + 2] * 3;

                glm::vec3 v0(mVertex.vertices[index0], mVertex.vertices[index0 + 1], mVertex.vertices[index0 + 2]);
                glm::vec3 v1(mVertex.vertices[index1], mVertex.vertices[index1 + 1], mVertex.vertices[index1 + 2]);
                glm::vec3 v2(mVertex.vertices[index2], mVertex.vertices[index2 + 1], mVertex.vertices[index2 + 2]);

                glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                mVertex.normals[index0] += normal.x;
                mVertex.normals[index0 + 1] += normal.y;
                mVertex.normals[index0 + 2] += normal.z;

                mVertex.normals[index1] += normal.x;
                mVertex.normals[index1 + 1] += normal.y;
                mVertex.normals[index1 + 2] += normal.z;

                mVertex.normals[index2] += normal.x;
                mVertex.normals[index2 + 1] += normal.y;
                mVertex.normals[index2 + 2] += normal.z;
            }

            for (size_t i = 0; i < mVertex.normals.size(); i += 3)
            {
                glm::vec3 normal(mVertex.normals[i], mVertex.normals[i + 1], mVertex.normals[i + 2]);
                normal = glm::normalize(normal);

                mVertex.normals[i] = normal.x;
                mVertex.normals[i + 1] = normal.y;
                mVertex.normals[i + 2] = normal.z;
            }
        }
    }
}
