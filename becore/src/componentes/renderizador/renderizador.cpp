#include "renderizador.hpp"
#include <glad/glad.h>

namespace Bubble::Componentes
{
    void Renderizador::configurar() 
    { 
        for (auto& vertex : arquivo_obj->vertices)
        {
            configurarBuffers(vertex);
        }
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador configurado");
    }
    void Renderizador::atualizar(float deltaTime)
    {
        for (auto& material : arquivo_obj->materiais)
        {
            atualizarCores(material);
        }
        for (auto& vertex : arquivo_obj->vertices)
        {
        desenharModelo(vertex);
        }
    }
	void Renderizador::configurarBuffers(Vertex mesh)
    {
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        // Calculate the total size needed for vertices, normals, and uvs
        size_t vertexSize = mesh.vertices.size() * sizeof(float);
        size_t normalSize = mesh.normals.size() * sizeof(float);
        size_t uvSize = mesh.uvs.size() * sizeof(float);
        size_t totalSize = vertexSize + normalSize + uvSize;

        // Allocate buffer with total size
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        // Fill buffer with vertices data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &mesh.vertices[0]);

        // Fill buffer with normals data if available
        if (!mesh.normals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &mesh.normals[0]);
        }

        // Fill buffer with uvs data if available
        if (!mesh.uvs.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, &mesh.uvs[0]);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        if (!mesh.normals.empty()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
            glEnableVertexAttribArray(1);
        }

        // UV attribute
        if (!mesh.uvs.empty()) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void Renderizador::desenharModelo(Vertex mesh)
    {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
    }
    void Renderizador::atualizarCores(Material material)
    {
        if (shader)
            shader->use();
        shader->setVec3("objectColor", material.difusa.r, material.difusa.g, material.difusa.b);
    }
    Renderizador::Renderizador()
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    };

    Renderizador::Renderizador(Arquivadores::Arquivo3d& objf) : arquivo_obj(&objf) 
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    };
}