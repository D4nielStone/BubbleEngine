#include <glad/glad.h>
#include "sistema_de_renderizacao.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/camera.hpp"
#include "src/arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"

Shader* shader_phong{ nullptr };

void SistemaDeRenderizacao::atualizar(double deltaTime)
{
    glClearColor(0.1F, 0.1F, 0.5F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ge->paraCadaEntidade<Camera>([&](const uint32_t ent_cam) {
        auto camera = ge->obterComponete<Camera>(ent_cam);
        ge->paraCadaEntidade<Renderizador>([&](const uint32_t ent_ren) {
            auto render = ge->obterComponete<Renderizador>(ent_ren);

            shader_phong->use();
            shader_phong->setMat4("view", glm::value_ptr(camera->obtViewMatrix()));
            shader_phong->setMat4("projection", glm::value_ptr(camera->obtProjectionMatrix()));

            if(ge->temComponent<Transformacao>(ent_ren))
            {
                auto transform = ge->obterComponete<Transformacao>(ent_ren);
                transform->calcularMatriz();
                shader_phong->setMat4("modelo", transform->obterMatrizPtr());
            }
            else
            {
                shader_phong->setMat4("modelo", glm::value_ptr(glm::mat4(1.F)));
            }


            render->modelo->Desenhar(*shader_phong);
            });
        });
    Debug::emitir("FPS", std::to_string(1/deltaTime));
}

void SistemaDeRenderizacao::inicializar()
{
    if (!shader_phong) shader_phong = new Shader();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}