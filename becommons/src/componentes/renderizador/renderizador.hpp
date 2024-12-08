#pragma once
#include "src/comum/componente.hpp"
#include "src/depuracao/debug.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "src/util/utils.hpp"
#include "becommons.hpp"

namespace Bubble{
    namespace Componentes {
        class Renderizador : public Bubble::Comum::Componente {
        private:
            void lerMalha(const std::string& malha_path, const std::string& malha_id);
            void configurarBuffers();
            Malha malha;
            std::string path_malha;
        public:
            Renderizador(const Malha& malha);
            Renderizador(const std::string& malha_path, const std::string& malha_id);
            Renderizador() { Nome = "Renderizador"; };
            ~Renderizador();
            void liberar() override;
            virtual Malha& obterMalha();
            void configurar() override;
            void atualizar() override;
            void salvarMalha() const;
            rapidjson::Value serializar(rapidjson::Document* doc) const override;
            bool parse(rapidjson::Value& entidade) override;
            bool visualizarWireFrame{ false };
        };
    }
}