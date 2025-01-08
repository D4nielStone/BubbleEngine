/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "transformacao.hpp"
#include <bullet/btBulletDynamicsCommon.h>

namespace bubble
{

    struct fisica : componente {
        static constexpr mascara mascara { COMPONENTE_FISICA };
        fisica() = default;
        fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial);
        fisica(bool malha, btScalar massa, btVector3 posicaoInicial);
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