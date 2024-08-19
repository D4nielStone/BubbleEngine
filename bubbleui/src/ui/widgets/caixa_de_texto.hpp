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
            CaixaTexto(std::string mensagem = "");

            void atualizar(float deltaTime) override;
            void renderizar() override;
        private:
            void processarEntrada(char c);
            std::string mensagem;
            bool gatilho1, gatilho2;
            double cronometro;
        };
    }
}
