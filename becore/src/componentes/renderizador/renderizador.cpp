#include "renderizador.hpp"
#include <glad/glad.h>
#include <algorithm>

namespace Bubble::Componentes
{
    void Renderizador::configurar()
    {
        std::for_each(vertices.begin(), vertices.end(), [this](Vertex& vertex) {
            configurarBuffers(vertex);
            });
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador configurado");
    }

    void Renderizador::atualizar(float deltaTime)
    {
        std::for_each(materiais.begin(), materiais.end(), [this](Material& material) {
            atualizarCores(material);
            });

        std::for_each(vertices.begin(), vertices.end(), [this](Vertex& vertex) {
            desenharModelo(vertex);
            });
    }

    void Renderizador::configurarBuffers(Vertex& mesh)
    {
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        size_t vertexSize = mesh.vertices.size() * sizeof(float);
        size_t normalSize = mesh.normals.size() * sizeof(float);
        size_t uvSize = mesh.uvs.size() * sizeof(float);
        size_t totalSize = vertexSize + normalSize + uvSize;

        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, mesh.vertices.data());

        if (!mesh.normals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, mesh.normals.data());
        }

        if (!mesh.uvs.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, mesh.uvs.data());
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (!mesh.normals.empty()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
            glEnableVertexAttribArray(1);
        }

        if (!mesh.uvs.empty()) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderizador::desenharModelo(Vertex& mesh)
    {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
    }

    void Renderizador::atualizarCores(Material& material)
    {
        if (shader)
        {
            shader->use();
            shader->setVec3("objectColor", material.difusa.r, material.difusa.g, material.difusa.b);
        }
    }

    Renderizador::Renderizador()
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    }

    Renderizador::Renderizador(Arquivadores::Arquivo3d& objf)
    {
        Nome = "Renderizador";
        materiais = objf.materiais;
        vertices = objf.vertices;
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    }
}
