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
    struct resultadoRaio {
        bool atingiu;                      // Se o raio atingiu algo
        glm::vec3 pontoDeColisao;            // Ponto de colisão
        glm::vec3 normalAtingida;           // Normal da superfície atingida
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
    inline static resultadoRaio novoRaio(
        const glm::vec3& comeco,
        const glm::vec3& fim) {

        btVector3 start(comeco.x, comeco.y, comeco.z);
        btVector3 end(fim.x, fim.y, fim.z);

        resultadoRaio result;
        result.atingiu = false;
        result.objetoAtingido = nullptr;

        btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
        mundoDinamicoPrincipal->rayTest(start, end, rayCallback);

        if (rayCallback.hasHit()) {
            result.atingiu = true;
            result.pontoDeColisao = { rayCallback.m_hitPointWorld.getX(),rayCallback.m_hitPointWorld.getY(),rayCallback.m_hitPointWorld.getZ()};
            result.normalAtingida = { rayCallback.m_hitNormalWorld.getX(),rayCallback.m_hitNormalWorld.getY(),rayCallback.m_hitNormalWorld.getZ() };
            result.objetoAtingido = rayCallback.m_collisionObject;
        }

        return result;
    }
}