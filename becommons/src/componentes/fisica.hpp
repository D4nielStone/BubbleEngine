/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "transformacao.hpp"
#include <bullet/btBulletDynamicsCommon.h>

namespace bubble
{
    struct fisica : componente {
        static constexpr mascara mascara { COMPONENTE_FISICA };
        enum camada {
            COLISAO_PADRAO = 1 << 0,  // Objetos comuns
            COLISAO_ESPECIAL = 1 << 1 // Objeto especial
        };

        camada camada_colisao{camada::COLISAO_PADRAO};
        fisica() = default;
        fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial, camada camada = COLISAO_PADRAO);
        fisica(bool malha, btScalar massa, btVector3 posicaoInicial, camada camada = fisica::COLISAO_PADRAO);
        virtual ~fisica();

        btRigidBody* obterCorpoRigido();
        void criarMalha();
        void atualizarTransformacao();
        void aplicarForca(const glm::vec3& vetor);
        void aplicarVelocidade(const glm::vec3& vetor);
        glm::vec3 obterVelocidade() const;
        void init();
    private:
        bool malha{ false };
        btCollisionShape* forma;
        btDefaultMotionState* estadoDeMovimento;
        btRigidBody* corpoRigido{ nullptr };
        btScalar massa;
        btVector3 posicaoInicial;
        transformacao* _Mtransformacao;
    };
}