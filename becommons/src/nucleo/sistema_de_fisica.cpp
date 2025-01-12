#include "sistema_de_fisica.hpp"
#include "fase.hpp"
#include <os/janela.hpp>
#include <src/componentes/transformacao.hpp>
#include <src/componentes/fisica.hpp>

bubble::sistemaFisica::sistemaFisica()
{
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(btVector3(0, -10, 0));
}
bubble::sistemaFisica::~sistemaFisica()
{
    pararThread();

    // Libere os outros componentes
    delete solucionador;
    delete faseAmpla;
    delete expedidor;
    delete configColisao;
    delete mundoDinamico;
}


void bubble::sistemaFisica::atualizar(double deltaTime)
{
    mundoDinamico->stepSimulation(deltaTime);
}

void bubble::sistemaFisica::inicializar(bubble::fase* f)
{
    sistemaFisica::sistema::inicializar(f);
    mundoDinamicoPrincipal = mundoDinamico;
    reg->cada<bubble::fisica, bubble::transformacao>([&](const uint32_t entidade)
        {
            /// adiciona corpos rigidos
            auto comp_fisica = reg->obter<fisica>(entidade);
            // Adicionar ao mundo com grupo e máscara
            int camada = comp_fisica->camada_colisao;
            int mascara = comp_fisica->camada_colisao;
            mundoDinamicoPrincipal->addRigidBody(comp_fisica->obterCorpoRigido(), camada, mascara);
        }
    );
}

void bubble::sistemaFisica::iniciarThread()
{
    rodando = true;
    fisicaThread = std::thread([this]() {
        while (rodando) {
            {
                this->atualizar(0.016666);
                reg->cada<bubble::fisica, bubble::transformacao>([&](const uint32_t entidade)
                    {
                        /// adiciona corpos rigidos
                        reg->obter<fisica>(entidade)->atualizarTransformacao();
                    }
                );
                std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Aguarda 16ms
            }
        }
        });
}

void bubble::sistemaFisica::pararThread()
{
    rodando = false;
    if (fisicaThread.joinable()) {
        fisicaThread.join();
    }
}

void bubble::sistemaFisica::remover(btRigidBody*& corpo)
{
    if (mundoDinamico)
        mundoDinamico->removeRigidBody(corpo);
}

btDiscreteDynamicsWorld* bubble::sistemaFisica::mundo()
{
    return mundoDinamico;
}

bubble::resultadoRaio bubble::raioIntersecta(const raio& raio)
{
    // Configuração do ponto inicial e final do raio no espaço 3D
    btVector3 origem(raio.origem.x, raio.origem.y, raio.origem.z);
    btVector3 destino = origem + btVector3(raio.direcao.x, raio.direcao.y, raio.direcao.z) * 300.0f;

    // Criar o callback com máscaras de camada
    btCollisionWorld::ClosestRayResultCallback callback(origem, destino);
    callback.m_collisionFilterGroup = fisica::COLISAO_PADRAO | fisica::COLISAO_ESPECIAL; // Raycast pode detectar ambas
    callback.m_collisionFilterMask = fisica::COLISAO_ESPECIAL;                  // Detectar apenas o especial

    // Executar o raycast
    mundoDinamicoPrincipal->rayTest(origem, destino, callback);

    // Estrutura de retorno
    resultadoRaio resultado = { false, glm::vec3(0.0f), glm::vec3(0.0f), nullptr };

    if (callback.hasHit()) {
        btVector3 ponto = callback.m_hitPointWorld;
        resultado.pontoDeColisao = glm::vec3(ponto.getX(), ponto.getY(), ponto.getZ());

        btVector3 normal = callback.m_hitNormalWorld;
        resultado.normalAtingida = glm::normalize(glm::vec3(normal.getX(), normal.getY(), normal.getZ()));

        resultado.objetoAtingido = callback.m_collisionObject;
        resultado.atingiu = true;
    }

    return resultado;
}