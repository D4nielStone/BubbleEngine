#include "codigo.h"
#include "src/entidades/entidade.h"
#include <iostream>

namespace Bubble {
    namespace Componentes {
        Codigo::Codigo(const std::string& path)
            : scriptPath(path) {
            L = luaL_newstate();
            luaL_openlibs(L);
        }

        Codigo::~Codigo() {
            lua_close(L);
        }

        void Codigo::configurar() {

            
            if (luaL_loadfile(L, scriptPath.c_str()) || lua_pcall(L, 0, 0, 0)) {
                std::cerr << "Não foi possível carregar o script: " << scriptPath << "\n";
                std::cerr << lua_tostring(L, -1) << "\n";
            }
            else {
                lua_getglobal(L, "iniciar");
                if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                    std::cerr << "Erro ao executar a função de inicialização: \n";
                    std::cerr << lua_tostring(L, -1) << "\n";
                }
            }
        }

        void Codigo::atualizar() {
            meuObjeto->obterTransformacao()->Rotacionar(1, 1, 1);
            lua_getglobal(L, "atualizar");
            if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                std::cerr << "Erro ao executar a função de atualização: \n";
                std::cerr << lua_tostring(L, -1) << "\n";
            }
        }
    }
}
