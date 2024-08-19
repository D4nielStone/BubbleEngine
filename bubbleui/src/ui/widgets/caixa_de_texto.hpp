#pragma once
#include "src/ui/widgets/widget.hpp"
#include "src/arquivadores/fonte.hpp"
#include "texto.hpp"
#include <string>

namespace BubbleUI
{
    namespace Widgets
    {
        class BEUI_DLL_API CaixaTexto : public Texto
        {
        public:
            CaixaTexto();

            void atualizar(float deltaTime) override;
            void renderizar() override;
        private:
            void renderizar_texto();
            void processarEntrada(char c);
            bool gatilho1, gatilho2;
            double cronometro;
            int largura, xori, yori, altura_frase;
            Vector2 letra_padding;
            char letra_antiga;
        };
    }
}
