#ifndef RENDERIZADOR_H
#define RENDERIZADOR_H
#include "src/comum/componente.h"
#include "src/util/includes.h"
#include "src/depuracao/debug.h"
#include "src/arquivadores/arquivo3d.h"

namespace Bubble{
    namespace Componentes {
        class Renderizador : public Bubble::Comum::Componente {
        private:
            std::vector<Material>materiais;
            std::vector<Vertex>vertices;
            void configurarBuffers(Vertex& v);
            void desenharModelo(Vertex& v);
            void atualizarCores(Material& m);
        public:
            Renderizador();
            explicit Renderizador(Arquivadores::Arquivo3d& objf);
            void configurar() override;
            void atualizar(float deltaTime) override;
        };
    }
}
#endif