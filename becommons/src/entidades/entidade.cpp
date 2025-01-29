/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "entidades/entidade.hpp"
#include "componentes/renderizador.hpp"
#include "arquivadores/shader.hpp"

bubble::entidade bubble::registro::criar()
{
    proxima_entidade++;
    debug::emitir("registro", "nova entidade: " + std::to_string(proxima_entidade ));
    return { proxima_entidade, componente::COMPONENTE_NONE };
}

bubble::componente::mascara bubble::registro::obterComponentes(const uint32_t& id) const {
    auto it = mascaras.find(id);
    if (it != mascaras.end()) {
        return it->second; // Retorna a m�scara associada � entidade.
    }
    return componente::COMPONENTE_NONE; // Retorna uma m�scara vazia se a entidade n�o existir.
}
