#include "sistema_de_codigo.hpp"
#include "fase.hpp"
#include <src/componentes/codigo.hpp>

namespace bubble
{
    void sistemaCodigo::atualizar(double deltaTime)
    {
        //atualiza os componentes
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar(deltaTime);
            }
        );
    }

    void sistemaCodigo::inicializar(bubble::fase* fase)
    {
        this->fase = fase;
        this->reg = fase->obterRegistro();

        //inicializa os componentes
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->iniciar();
            }
        );
    }
    void sistemaCodigo::iniciarThread() {
        rodando = true;
        codigoThread = std::thread([this]() {
            while (rodando) {
                double deltaTime = 0.016; // Simula 60 FPS
                {
                    this->atualizar(deltaTime);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Aguarda 16ms
            }
            });
    }

    void sistemaCodigo::pararThread() {
        rodando = false;
        if (codigoThread.joinable()) {
            codigoThread.join();
        }
    }

    sistemaCodigo::~sistemaCodigo() {
        pararThread();
    }
}