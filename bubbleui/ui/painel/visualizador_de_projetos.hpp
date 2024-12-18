#pragma once
#include "painel.hpp"

using namespace BubbleUI;
namespace BubbleUI {
    namespace Paineis {
        class BEUI_DLL_API VisualizadorDeProjetos :
            public Painel
        {
        public:
            VisualizadorDeProjetos(const bool& preencher = true, const bubble::vetor4<int>& retangulo = {10, 10, 100, 400});
            void definirContexto(std::shared_ptr<Contexto> ctx) override;
        private:
            std::map<std::string, std::string> diretorios_projetos;
        };
    }
}