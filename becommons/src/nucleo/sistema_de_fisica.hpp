/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_fisica.hpp
 * @brief Gerencia a fisica de jogo com o bullet3
 */

#pragma once
#include "sistema.hpp"
#include "src/util/raio.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <atomic>

namespace bubble
{
    inline btDiscreteDynamicsWorld* mundoDinamicoPrincipal;
    
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
    inline static resultadoRaio novoRaio(const raio& raio) {
        // Configuração do ponto inicial e final do raio no espaço 3D
        btVector3 origem(raio.origem.x, raio.origem.y, raio.origem.z);
        btVector3 destino = origem + btVector3(raio.direcao.x, raio.direcao.y, raio.direcao.z) * 1000.0f; // Alcança até 1000 unidades

        // Criar o callback de raycast
        btCollisionWorld::ClosestRayResultCallback callback(origem, destino);

        // Executar o raycast no mundo físico
        mundoDinamicoPrincipal->rayTest(origem, destino, callback);

        // Estrutura de retorno
        resultadoRaio resultado = { false, glm::vec3(0.0f), glm::vec3(0.0f), nullptr };

        // Verificar se houve colisão
        if (callback.hasHit()) {
            resultado.atingiu = true;

            // Ponto de colisão
            btVector3 ponto = callback.m_hitPointWorld;
            resultado.pontoDeColisao = glm::vec3(ponto.getX(), ponto.getY(), ponto.getZ());

            // Normal da superfície atingida
            btVector3 normal = callback.m_hitNormalWorld;
            resultado.normalAtingida = glm::normalize(glm::vec3(normal.getX(), normal.getY(), normal.getZ()));

            // Objeto atingido
            resultado.objetoAtingido = callback.m_collisionObject;
        }

        return resultado;
    }
}