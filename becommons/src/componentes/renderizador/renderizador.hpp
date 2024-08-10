#ifndef RENDERIZADOR_H
#define RENDERIZADOR_H
#include "src/comum/componente.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "src/util/includes.hpp"
#include "becommons.hpp"

namespace Bubble{
    namespace Componentes {
        class BECOMMONS_DLL_API Renderizador : public Bubble::Comum::Componente {
        private:
            unsigned int ID;
            std::vector<Material*>materiais;
            std::vector<Vertex>vertices;
            void configurarBuffers(Vertex& v, Material* m = new Material());
            void desenharModelo(Vertex& v);
            void atualizarCores(Material& m);
        public:
            Renderizador();
            explicit Renderizador(Arquivadores::Arquivo3d& objf);
            ~Renderizador();
            void configurar() override;
            void atualizar(float deltaTime) override;
        };
    }
}
#endif