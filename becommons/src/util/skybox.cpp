#include "skybox.hpp"
#include "glad/glad.h"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/imageloader.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "src/cena/scenemanager.hpp"

namespace Bubble::Util
{
    unsigned int cubemapTexture;
    unsigned int obterSkyboxTexture()
    {
        return cubemapTexture;
    }
    Skybox::Skybox() : Path("assets/texturas/skybox/") {
        configurarBuffers();
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

        // Calcular tamanho total necess�rio para v�rtices, normais e UVs
        size_t vertexSize = malha.vertices.size() * sizeof(float);
        size_t totalSize = vertexSize;

        // Alocar buffer com tamanho total
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        // Preencher buffer com dados de v�rtices
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &malha.vertices[0]);

        // Atributo de posi��o
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Carregar cubemap
        loadCubemapFromSingleTexture(Path);
    }
    void Skybox::renderizar(const glm::mat4& projectionMat, const glm::mat4& viewMat) const
    {

        glm::mat4 view = glm::mat4(glm::mat3(viewMat));
        glDepthMask(GL_FALSE);
        shader.use();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projectionMat));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        shader.setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, malha.vertices.size());
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glDepthMask(GL_TRUE);
    }
    unsigned int Skybox::loadCubemapFromSingleTexture(const char* path)
    {
        glGenTextures(1, &cubemapTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        std::vector<std::string> faces
        {
                "right.png",
                 "left.png",
                  "top.png",
               "bottom.png",
                "front.png",
                 "back.png"
        };
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            auto image = Bubble::Arquivadores::ImageLoader(path + faces[i]);
            auto data = image.obterDados();
            int width = image.getWidth();
            int height = image.getHeight();
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
                );
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return cubemapTexture;
    }
}