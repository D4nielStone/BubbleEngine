/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/arquivadores/shader.hpp"

bubble::entidade bubble::registro::criar()
{
    proxima_entidade++;
    Debug::emitir("registro", "nova entidade: " + std::to_string(proxima_entidade ));
    return { proxima_entidade, COMPONENTE_NONE };
}