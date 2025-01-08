#include "sistema_de_fisica.hpp"
#include "fase.hpp"
#include <os/janela.hpp>
#include <src/componentes/transformacao.hpp>

bubble::sistemaFisica::sistemaFisica()
{
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(btVector3(0, -9.8, 0));
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
    reg->cada<bubble::fisica, bubble::transformacao>([&](const uint32_t entidade)
        {
            /// adiciona corpos rigidos
            auto comp_fisica = reg->obter<fisica>(entidade);
            mundoDinamico->addRigidBody(comp_fisica->obterCorpoRigido());
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