#include "codigo.hpp"
#include "src/entidades/entidade.hpp"
#include <iostream>

using namespace Bubble::Componentes;
Codigo::Codigo(const std::string& path)
    : scriptPath(path) {
    L = luaL_newstate();
    luaL_openlibs(L);
}
Codigo::~Codigo() {
    lua_close(L);
}
// Deve configurar o luastate
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
    carregadov = true;
}
// Deve atualizar o script
void Codigo::atualizar(float deltaTime) {
    //mais tare vou criar a api
    lua_getglobal(L, "atualizar");
    if (luaL_loadfile(L, scriptPath.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Não foi possível carregar o script: " << scriptPath << "\n";
        std::cerr << lua_tostring(L, -1) << "\n";
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        std::cerr << "Erro ao executar a função de atualização: \n";
        std::cerr << lua_tostring(L, -1) << "\n";
    }
}