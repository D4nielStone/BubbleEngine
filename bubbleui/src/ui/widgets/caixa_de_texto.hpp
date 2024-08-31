#pragma once
#include "src/ui/widgets/widget.hpp"
#include "src/ui/contexto/contexto.hpp"
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
            CaixaTexto(std::string* buffer, std::string mensagem);

            void atualizar() override;
            void renderizar() override;
            void defPainel(Painel* painel) override;
        private:
            void processarEntrada(char c);
            void atualizarInputs();
            void iniciarSelecao();
            Bubble::Inputs::Inputs* inputs{ nullptr };
            Contexto* contexto{ nullptr };
            std::string* mensagem{ nullptr };
            std::string* buffer_texto{ nullptr };
            std::string texto;
            bool gatilho1, gatilho2, selecionado{ false }, selecionando_texto{ false };
            Vector2 mouse_pos_ini;
            double cronometro;
        };
    }
}
