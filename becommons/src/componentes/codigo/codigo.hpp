#pragma once
#include "src/comum/componente.hpp"
#include <string>
#include "becommons.hpp"
#include <lua.hpp>

namespace Bubble {
    namespace Componentes {
        class BECOMMONS_DLL_API Codigo : public Bubble::Comum::Componente {
        public:
            Codigo(const std::string& path);
            ~Codigo();

            void configurar() override;
            void atualizar() override;

        private:
            std::string scriptPath;
            lua_State* L;
        };
    }
}
