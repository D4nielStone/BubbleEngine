#include "codigo.hpp"
#include "src/nucleo/fase.hpp"
#include "src/api/api_entidade.hpp"
#include <src/inputs/inputs.hpp>
#include "os/janela.hpp"


bubble::codigo::codigo(const std::string& arquivo) : L(luaL_newstate()), arquivo(arquivo)

{
	luaL_openlibs(L);

	bapi::entidade::definir(L);

	/* definindo classe inputs */
	luabridge::getGlobalNamespace(L).
		beginClass<bubble::inputs>("entradas").
		addConstructor<void(*)()>().
		addData("mouse_x", &bubble::inputs::mousex).
		addData("mouse_y", &bubble::inputs::mousey).
		addFunction("estaPressionado", &bubble::inputs::isKeyPressed).
		endClass();
	/*-------------------------*/

	// Carregar e executar o script
	if (luaL_dofile(L, arquivo.c_str()) != LUA_OK)
	{
		Debug::emitir(Erro, std::string("carregar script : ") + lua_tostring(L, -1));
	}
}

void bubble::codigo::iniciar() const
{
	luabridge::setGlobal(L, new bapi::entidade(meu_objeto), "eu");
	luabridge::setGlobal(L, &bubble::instanciaJanela->inputs, "entradas");
	// Tentar obter a função "iniciar" definida localmente no script
	lua_getglobal(L, "iniciar");

	// Verificar se a função foi encontrada e é válida
	if (lua_isfunction(L, -1))
	{
		// Chamar a função Lua "iniciar" - é uma função local dentro do escopo do script
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			Debug::emitir(Erro, std::string("Erro ao chamar função 'iniciar': ") + lua_tostring(L, -1));
			lua_pop(L, 1);  // Remover a mensagem de erro da pilha
		}
	}
	else
	{
		Debug::emitir(Erro, "Função 'iniciar' não encontrada ou não é uma função local.");
		lua_pop(L, 1); // Remover da pilha se não for uma função válida
	}
}

void bubble::codigo::atualizar(double deltaTime) const
{
	// Chamar função de atualização no Lua, se existir
	lua_getglobal(L, "atualizar");
	if (lua_isfunction(L, -1))
	{
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			Debug::emitir(Erro, std::string("atualizar script : ") + lua_tostring(L, -1));
		}
	}
	else
	{
		lua_pop(L, 1); // Remover da pilha se não for função
	}
}