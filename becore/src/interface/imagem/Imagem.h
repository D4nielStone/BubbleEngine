#pragma once
#include "src/interface/base/quadrado.hpp"
#include "string"
#include "becore.h"
namespace Bubble
{
    namespace Interface
    {
        class BECORE_DLL_API Imagem : public Quadrado
        {
        public:
            Imagem() {}
            Imagem(std::string path, float escala);
            Imagem(unsigned int id);
            void renderizar() override;
            void atualizar() override;
            void preencher(bool = true);
        private:
            unsigned int ID, escala = 32;
            Vector2 tamanhoOri;
            bool preenchervar;
        };
    }
}
