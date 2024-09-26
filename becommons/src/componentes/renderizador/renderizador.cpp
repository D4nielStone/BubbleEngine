#include "renderizador.hpp"
#include "src/arquivadores/imageloader.hpp"
#include <glad/glad.h>
#include <algorithm>
#include <src/util/skybox.hpp>

using namespace Bubble::Componentes;
void Renderizador::configurar()
{
    configurarBuffers();
    carregadov = true;
}
void Renderizador::atualizar()
{ 
    glBindVertexArray(malha.VAO);
    GLenum mode{};
    if (visualizarWireFrame)
        mode = GL_LINE_LOOP;
    else
        mode = GL_TRIANGLES;
    glDrawElements(mode, static_cast<GLsizei>(malha.indices.size()), GL_UNSIGNED_INT, 0);
}
void Renderizador::configurarBuffers()
    {
        //vertex buffer
        glGenVertexArrays(1, &malha.VAO);
        glGenBuffers(1, &malha.VBO);
        glGenBuffers(1, &malha.EBO);

        glBindVertexArray(malha.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, malha.VBO);

        size_t vertexSize = malha.vertices.size() * sizeof(float);
        size_t normalSize = malha.normals.size() * sizeof(float);
        size_t uvSize = malha.uvs.size() * sizeof(float);
        size_t totalSize = vertexSize + normalSize + uvSize;

        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, malha.vertices.data());

        if (!malha.normals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, malha.normals.data());
        }

        if (!malha.uvs.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, malha.uvs.data());
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malha.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, malha.indices.size() * sizeof(unsigned int), malha.indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (!malha.normals.empty()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
            glEnableVertexAttribArray(1);
        }

        if (!malha.uvs.empty()) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }
Renderizador::Renderizador(const Vertex& malha) : malha(malha)
{
    Nome = "Renderizador";
    Debug::emitir("Renderizador", "malha: " + malha.nome);
}
Renderizador::~Renderizador()
{
    // Liberar texturas e buffers
    glDeleteVertexArrays(1, &malha.VAO);
    glDeleteBuffers(1, &malha.VBO);
    glDeleteBuffers(1, &malha.EBO);
}

Vertex& Bubble::Componentes::Renderizador::obterMalha()
{
    return malha;
}
void Bubble::Componentes::atualizarMaterial(Material material, Shader shader)
{
    //Debug::emitir("Renderizador", "atualizando material: " + material.nome);
    shader.use();
    shader.setVec3("material.cor_difusa", material.difusa.r, material.difusa.g, material.difusa.b);
    shader.setVec3("material.cor_especular", material.especular.r, material.especular.g, material.especular.b);
    shader.setFloat("material.shininess", material.shininess);
    shader.setFloat("material.reflexao", material.reflexao);
    shader.setBool("textura_difusa_ativo", false);
    for (size_t i = 0; i < material.texturas.size(); i++)
    {
        glActiveTexture(GL_TEXTURE1 + i);
        glBindTexture(GL_TEXTURE_2D, material.texturas[i].ID);
        shader.setInt(material.texturas[i].tipo, 1 + i);
        shader.setBool(material.texturas[i].tipo + "_ativo", true);
    }
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Bubble::Util::obterSkyboxTexture());
}