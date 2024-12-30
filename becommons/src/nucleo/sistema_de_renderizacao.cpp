#include <glad/glad.h>
#include "sistema_de_renderizacao.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/camera.hpp"
#include "src/nucleo/fase.hpp"
#include "src/arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"

bubble::shader* shader_phong{ nullptr };

namespace bubble
{
    void sistemaRenderizacao::atualizar(double deltaTime)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        auto camera = fase->obterCamera();
        if (!camera) return;
        glClearColor(camera->ceu.r, camera->ceu.g, camera->ceu.b, camera->ceu.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
            auto render = reg->obter<renderizador>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);

            shader_phong->use();
            shader_phong->setMat4("view", glm::value_ptr(camera->obtViewMatrix()));
            shader_phong->setVec3("viewPos", camera->posicao.x,camera->posicao.y,camera->posicao.z);
            shader_phong->setMat4("projection", glm::value_ptr(camera->obtProjectionMatrix()));
            transform->calcular();
            shader_phong->setMat4("modelo", transform->obter());
            render->modelo->desenhar(*shader_phong);
            });
    }

    void sistemaRenderizacao::inicializar(bubble::fase* fase)
    {
        this->fase = fase;
        this->reg = fase->obterRegistro();

        if (!shader_phong) shader_phong = new bubble::shader();
        glCullFace(GL_BACK);
    }
}