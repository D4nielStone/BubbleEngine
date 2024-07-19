#ifndef RENDERIZADOR_H
#define RENDERIZADOR_H
#include "src/comum/componente.h"
#include "src/util/includes.h"
#include "src/depuracao/debug.h"

namespace Bubble{
    namespace Componentes {
        class Renderizador : public Bubble::Comum::Componente {
        private:
            Vertex mVertex;
            Material mMaterial;
            unsigned int VAO, EBO, VBO;
            void configurarBuffers();
            void desenharModelo();
            void atualizarCores();
        public:
            Renderizador();
            Renderizador(Vertex vertex, Material material);
            Renderizador(Vertex vertex);
            void configurar() override;
            void atualizar(float deltaTime) override;
        };
    }
}
#endif