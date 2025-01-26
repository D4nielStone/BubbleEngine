#include <glad/glad.h>
#include "sistema_de_renderizacao.hpp"
#include "../componentes/renderizador.hpp"
#include "../componentes/transformacao.hpp"
#include "../componentes/camera.hpp"
#include "../nucleo/fase.hpp"
#include "../arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../../os/janela.hpp"

namespace bubble
{
    void sistemaRenderizacao::atualizar()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        auto camera = _Mfase->obterCamera();
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
            s.setVec2("resolution", instanciaJanela->tamanho.x, instanciaJanela->tamanho.y);
            s.setMat4("modelo", transform->obter());
            render->modelo->desenhar(s);
            });


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, instanciaJanela->tamanho.x, instanciaJanela->tamanho.y);
    }

    void sistemaRenderizacao::inicializar(bubble::fase* fase_ptr)
    {
        this->_Mfase = fase_ptr;
        this->reg = _Mfase->obterRegistro();

        glCullFace(GL_BACK);
    }
}