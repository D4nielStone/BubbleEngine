#ifndef CODIGO_H
#define CODIGO_H

#include "src/comum/componente.h"
#include <string>
#include <lua.hpp>

namespace Bubble {
    namespace Componentes {
        class Codigo : public Bubble::Comum::Componente {
        public:
            Codigo(const std::string& path);
            ~Codigo();

            void configurar() override;
            void atualizar(float deltaTime) override;

        private:
            std::string scriptPath;
            lua_State* L;
        };
    }
}

#endif // CODIGO_H
