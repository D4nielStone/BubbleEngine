#pragma once
#include "painel.hpp"

using namespace BubbleUI;
namespace BubbleUI {
    namespace Paineis {
        class BEUI_DLL_API VisualizadorDeProjetos :
            public Painel
        {
        public:
            VisualizadorDeProjetos(const bool& preencher = true, const Vector4& retangulo = {10, 10, 100, 400});
            void posAtualizacao() override;
            void definirContexto(std::shared_ptr<Contexto> ctx) override;
        private:
            bool preencher{};   // flag para preencher a tela com o painel
            void recarregar();
            std::map<std::string, std::string> diretorios_projetos;
        };
    }
}