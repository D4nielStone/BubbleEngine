/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_fisica.hpp
 * @brief Gerencia a fisica de jogo com o bullet3
 */

#pragma once
#include "sistema.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <atomic>

namespace bubble
{
    inline btDiscreteDynamicsWorld* mundoDinamicoPrincipal;
    // Estrutura para armazenar os resultados do Raycast
    struct ResultadoRaio {
        bool atingiu;                      // Se o raio atingiu algo
        btVector3 pontoDeColisao;            // Ponto de colisão
        btVector3 normalAtingida;           // Normal da superfície atingida
        const btCollisionObject* objetoAtingido; // Objeto atingido
    };
    class sistemaFisica : public sistema {
    public:
         sistemaFisica();
        ~sistemaFisica();

        void atualizar(double deltaTime) override;
        void inicializar(bubble::fase* f) override;
        void iniciarThread();
        void pararThread();
        void remover(btRigidBody*& corpo);
        btDiscreteDynamicsWorld* mundo();

    private:
        std::atomic<bool> rodando{ false }; // Controle da thread
        std::thread fisicaThread;

        btDefaultCollisionConfiguration* configColisao;
        btCollisionDispatcher* expedidor;
        btBroadphaseInterface* faseAmpla;
        btSequentialImpulseConstraintSolver* solucionador;
        btDiscreteDynamicsWorld* mundoDinamico;
    };

    // Função de Raycast
    inline static ResultadoRaio novoRaio(
        const btVector3& start,
        const btVector3& end) {
        ResultadoRaio result;
        result.atingiu = false;
        result.objetoAtingido = nullptr;

        btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
        mundoDinamicoPrincipal->rayTest(start, end, rayCallback);

        if (rayCallback.hasHit()) {
            result.atingiu = true;
            result.pontoDeColisao = rayCallback.m_hitPointWorld;
            result.normalAtingida = rayCallback.m_hitNormalWorld;
            result.objetoAtingido = rayCallback.m_collisionObject;
        }

        return result;
    }
}