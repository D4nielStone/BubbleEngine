#include "codigo.hpp"
#include "src/entidades/entidade.hpp"
#include <iostream>
#include <lua.hpp>

using namespace Bubble::Componentes;

// Função personalizada para substituir o `print` no Lua
int luaPrintRedirect(lua_State* L) {
    int n = lua_gettop(L);  // Número de argumentos passados para `print`

    std::string output;
    for (int i = 1; i <= n; i++) {
        if (lua_isstring(L, i)) {
            output += lua_tostring(L, i);  // Concatena os argumentos
        }
        else {
            output += lua_typename(L, lua_type(L, i));  // Se não for string, captura o tipo
        }
        if (i < n) {
            output += "\t";  // Adiciona tabulação entre os argumentos
        }
    }
    Debug::cout(output);  // Envia a mensagem para o Debug::cout()
    return 0;  // Número de valores retornados por `print`
}

Codigo::Codigo(const std::string& path)
    : scriptPath(path) {
    L = luaL_newstate();
    luaL_openlibs(L);
    Nome = "Codigo";

    // Substitui a função `print` do Lua pela nossa função personalizada
    lua_pushcfunction(L, luaPrintRedirect);
    lua_setglobal(L, "print");
}

Codigo::~Codigo() {
    lua_close(L);
}

// Deve configurar o luastate
void Codigo::configurar() {
    if (luaL_loadfile(L, scriptPath.c_str()) || lua_pcall(L, 0, 0, 0)) {
        Debug::cout("Não foi possível carregar o script: " + scriptPath);
        Debug::cout(lua_tostring(L, -1));
    }
    else {
        lua_getglobal(L, "iniciar");
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            Debug::cout("Erro ao executar a função de inicialização:");
            Debug::cout(lua_tostring(L, -1));
        }
    }
    carregadov = true;
}

// Deve atualizar o script
void Codigo::atualizar() {
    lua_getglobal(L, "atualizar");
    if (luaL_loadfile(L, scriptPath.c_str()) || lua_pcall(L, 0, 0, 0)) {
        Debug::cout("Não foi possível carregar o script: " + scriptPath);
        Debug::cout(lua_tostring(L, -1));
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        Debug::cout("Erro ao executar a função de atualização:");
        Debug::cout(lua_tostring(L, -1));
    }
}
