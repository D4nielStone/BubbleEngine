
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "skybox.hpp"
#include "glad/glad.h"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace Bubble::Util
{
    unsigned int cubemapTexture;
    unsigned int obterSkyboxTexture()
    {
        return cubemapTexture;
    }
    Skybox::Skybox() : Path("skybox") {
    }
    Skybox::Skybox(const char* path) : Path(path) {}
    void Skybox::configurarBuffers() {
        std::vector<float> skyboxVertices = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        malha.vertices = skyboxVertices;

        glGenBuffers(1, &malha.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, malha.VBO);

        // Calcular tamanho total necessário para vértices, normais e UVs
        size_t vertexSize = malha.vertices.size() * sizeof(float);
        size_t totalSize = vertexSize;

        // Alocar buffer com tamanho total
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        // Preencher buffer com dados de vértices
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &malha.vertices[0]);

        // Atributo de posição
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Carregar cubemap
        loadCubemapFromSingleTexture(Path);
    }
    void Skybox::renderizar(const glm::mat4& projectionMat, const glm::mat4& viewMat)
    {
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glm::mat4 view = glm::mat4(glm::mat3(viewMat));
        shader.use();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projectionMat));
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        shader.setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(malha.vertices.size()));
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glDepthMask(GL_TRUE);
    }
    unsigned int Skybox::loadCubemapFromSingleTexture(const char* path)
    {

        std::vector<std::string> faces
        {
                "right.jpg",
                 "left.jpg",
                  "top.jpg",
               "bottom.jpg",
                "front.jpg",
                 "back.jpg"
        }; 

        return Bubble::Arquivadores::TextureLoader::getInstance().carregarSkybox(path, faces);
    }
}
