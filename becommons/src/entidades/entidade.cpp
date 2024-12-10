/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/arquivadores/shader.hpp"

Shader* shader_phong{ nullptr };

Entidade GerenciadorDeEntidades::criarEntidade()
{
    if (!shader_phong) shader_phong = new Shader();
    proxima_entidade++;
    Debug::emitir("GenEnt", "nova entidade: " + std::to_string(proxima_entidade ));
    return proxima_entidade;
}

void GerenciadorDeEntidades::atualizarRenderizadores(GerenciadorDeEntidades& gc)
{
    gc.paraCadaEntidade<Renderizador>([&](Entidade ent) {
        auto render = gc.obterComponete<Renderizador>(ent);
        render->modelo->Desenhar(*shader_phong);
        });
}
