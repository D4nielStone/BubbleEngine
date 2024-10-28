#pragma once
#include "painel.hpp"

using namespace BubbleUI;
namespace BubbleUI {
    namespace Paineis {
        class BEUI_DLL_API VisualizadorDeProjetos :
            public Painel
        {
        public:
            VisualizadorDeProjetos(std::shared_ptr<Contexto> contexto, const bool& preencher = true);
            void posAtualizacao() override;
        private:
            bool preencher{};   // flag para preencher a tela com o painel
            void recarregar();
            std::map<std::string, std::string> diretorios_projetos;
        };
    }
}