#include "skybox.hpp"
#include "glad/glad.h"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

////////////
// 
// TODO: Consertar essa classe, não está funcionando
// 
// 

namespace Bubble::Util
{
    Skybox::Skybox() : Path("assets/texturas/cubemap.jpg") {
    }
    Skybox::Skybox(const char* path) : Path(path) {}
    void Skybox::configurarBuffers() {
        shader = Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

        // Configurar modelo do cubo
        auto object_file = Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/cube.dae");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Calcular tamanho total necessário para vértices, normais e UVs
        size_t vertexSize = mVertex->vertices.size() * sizeof(float);
        size_t normalSize = mVertex->normals.size() * sizeof(float);
        size_t uvSize = mVertex->uvs.size() * sizeof(float);
        size_t totalSize = vertexSize + normalSize + uvSize;

        // Alocar buffer com tamanho total
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        // Preencher buffer com dados de vértices
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &mVertex->vertices[0]);

        // Preencher buffer com dados de normais se disponíveis
        if (!mVertex->normals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &mVertex->normals[0]);
        }

        // Preencher buffer com dados de UVs se disponíveis
        if (!mVertex->uvs.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, &mVertex->uvs[0]);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertex->indices.size() * sizeof(unsigned int), &mVertex->indices[0], GL_STATIC_DRAW);

        // Atributo de posição
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Atributo de normal
        if (!mVertex->normals.empty()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
            glEnableVertexAttribArray(1);
        }

        // Atributo de UV
        if (!mVertex->uvs.empty()) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Carregar cubemap
        cubemapTexture = loadCubemapFromSingleTexture(Path);
    }
    void Skybox::renderizar() {
        shader.use();

        glm::mat4 matriz_de_modelo = glm::mat4(1.f);
        shader.setMat4("model", glm::value_ptr(matriz_de_modelo));

        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        shader.setInt("texturea", 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertex->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    unsigned int Skybox::loadCubemapFromSingleTexture(const char* path) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto image = Bubble::Arquivadores::ImageLoader(path);
        auto data = image.obterDados();
        int width = image.getWidth();
        int height = image.getHeight();

        if (data) {
            for (unsigned int i = 0; i < 6; ++i) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        else {
            Debug::emitir(Debug::Tipo::Erro, "Não foi possível carregar a textura do skybox");
        }
        glActiveTexture(GL_TEXTURE1);

        return texture;
    }
}
