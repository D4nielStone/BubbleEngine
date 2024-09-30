#pragma once
#include "src/comum/componente.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "src/util/includes.hpp"
#include "becommons.hpp"

namespace Bubble{
    namespace Componentes {
        class BECOMMONS_DLL_API Renderizador : public Bubble::Comum::Componente {
        private:
            void configurarBuffers();
            Vertex malha;
        public:
            Renderizador(const Vertex& malha);
            Renderizador() { Nome = "Renderizador"; };
            ~Renderizador();
            Vertex& obterMalha();
            void configurar() override;
            void atualizar() override;
            bool visualizarWireFrame{ false };
        };
        extern BECOMMONS_DLL_API void atualizarMaterial(Material material, Shader shader);
    }
}