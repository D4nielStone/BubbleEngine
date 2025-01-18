#include <glad/glad.h>
#include "sistema_de_renderizacao.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/camera.hpp"
#include "src/nucleo/fase.hpp"
#include "src/arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <os/janela.hpp>

namespace bubble
{
    void sistemaRenderizacao::atualizar()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        auto camera = fase->obterCamera();
        if (!camera) return;

        camera->desenharFB();

        reg->cada<transformacao>([&](const uint32_t ent) {
            auto transform = reg->obter<transformacao>(ent);
            transform->calcular();
            });
        reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
            auto render = reg->obter<renderizador>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);
            
            auto s = render->modelo->shader();

            s.use();
            s.setMat4("view", glm::value_ptr(camera->obtViewMatrix()));
            s.setVec3("viewPos", camera->posicao.x,camera->posicao.y,camera->posicao.z);
            s.setMat4("projection", glm::value_ptr(camera->obtProjectionMatrix()));
            s.setVec2("resolution", instanciaJanela->tamanho.w, instanciaJanela->tamanho.h);
            s.setMat4("modelo", transform->obter());
            render->modelo->desenhar(s);
            });


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void sistemaRenderizacao::inicializar(bubble::fase* fase)
    {
        this->fase = fase;
        this->reg = fase->obterRegistro();

        glCullFace(GL_BACK);
    }
}