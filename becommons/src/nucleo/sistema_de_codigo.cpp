#include "sistema_de_codigo.hpp"
#include "fase.hpp"
#include <src/componentes/codigo.hpp>
#include <os/janela.hpp>
#include <mutex>

std::mutex mtx;

namespace bubble
{
    void sistemaCodigo::atualizar()
    {
        std::lock_guard<std::mutex> lock(mtx); // Protege o acesso ao registro
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar();
            }
        );
    }

    void sistemaCodigo::inicializar(bubble::fase* fase)
    {
        std::lock_guard<std::mutex> lock(mtx); // Protege o registro durante a inicialização
        this->fase = fase;
        this->reg = fase->obterRegistro();

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
                {
                    this->atualizar();
                }
            }
            });
    }

    void sistemaCodigo::pararThread() {
        // Chama a função `encerrar` para todos os componentes de código
        std::lock_guard<std::mutex> lock(mtx); // Protege o registro durante o encerramento
       
        rodando = false;
        if (codigoThread.joinable()) {
            codigoThread.join();
        }
    }

    sistemaCodigo::~sistemaCodigo() {
    }
}