#include "renderizador.hpp"
#include <glad/glad.h>
namespace Bubble::Componentes
{
    void Renderizador::configurar() 
    {
        configurarBuffers();
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador configurado");
    }
    void Renderizador::atualizar(float deltaTime)
    {
        atualizarCores();
        desenharModelo();
    }
	void Renderizador::configurarBuffers()
    {
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
    void Renderizador::desenharModelo()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertex.indices.size()), GL_UNSIGNED_INT, 0);
    }
    void Renderizador::atualizarCores()
    {
        if (shader)
            shader->use();
        shader->setVec3("objectColor", mMaterial.difusa.r, mMaterial.difusa.g, mMaterial.difusa.b);
    }
    Renderizador::Renderizador()
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    };
    Renderizador::Renderizador(Vertex vertex, Material material) : mVertex(vertex), mMaterial(material) 
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    };
    Renderizador::Renderizador(Vertex vertex) : mVertex(vertex) 
    {
        Nome = "Renderizador";
        Debug::emitir(Debug::Tipo::Mensagem, "Renderizador criado");
    };
}