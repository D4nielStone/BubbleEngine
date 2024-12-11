/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/arquivadores/shader.hpp"

Entidade GerenciadorDeEntidades::criarEntidade()
{
    proxima_entidade++;
    Debug::emitir("GenEnt", "nova entidade: " + std::to_string(proxima_entidade ));
    return proxima_entidade;
}
