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
            CaixaTexto(const std::string &mensagem = "");
            CaixaTexto(std::shared_ptr<std::string> buffer,const std::string &mensagem);
            CaixaTexto(std::string* buffer,const std::string &mensagem);

            void atualizar() override;
            void renderizar() const override;
            void defPainel(Painel* painel) override;
        private:
            void processarEntrada(char c);
            void atualizarInputs();
            void iniciarSelecao();
            std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
            std::shared_ptr<Contexto> contexto{ nullptr };
            std::string mensagem, texto;
            std::shared_ptr<std::string> buffer_texto;
            bool gatilho1, gatilho2, selecionado{ false };
            Vector2 mouse_pos_ini;
            double cronometro;
        };
    }
}
