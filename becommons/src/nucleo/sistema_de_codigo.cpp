#include "sistema_de_codigo.hpp"
#include "cena.hpp"
#include <src/componentes/codigo.hpp>

void bubble::sistemaCodigo::atualizar(double deltaTime)
{
    //atualiza os componentes
    reg->cada<codigo>([&](const uint32_t entidade)
        {
            auto componente_codigo = reg->obter<codigo>(entidade);
            componente_codigo->atualizar(deltaTime);
        }
    );
}

void bubble::sistemaCodigo::inicializar(bubble::cena* cena)
{
    this->cena = cena;
    this->reg = cena->obterRegistro();

    //inicializa os componentes
    reg->cada<codigo>([&](const uint32_t entidade)
        {
            auto componente_codigo = reg->obter<codigo>(entidade);
            componente_codigo->iniciar();
        }
    );
}