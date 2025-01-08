#include "codigo.hpp"
#include "src/nucleo/fase.hpp"
#include "src/api/api_entidade.hpp"
#include <src/inputs/inputs.hpp>
#include "os/janela.hpp"
#include <cmath>


bubble::codigo::codigo(const std::string& arquivo) : L(luaL_newstate()), arquivo(arquivo)

{
	luaL_openlibs(L);

	bapi::entidade::definir(L);

	/*-------------------------*/
	luabridge::getGlobalNamespace(L)
		.beginNamespace("inputs")
		.addFunction("pressionada", &bubble::pressionada)
		.addFunction("mouse", &bubble::obterMouse)
		.addFunction("tamanhoTela", &bubble::tamanhoJanela)
		.endNamespace()
		.beginNamespace("tempo")
		.addVariable("deltaT", &instanciaJanela->_Mtempo.deltaT)
		.endNamespace()
		.beginNamespace("mat")
		.addFunction("lerp", &std::lerp<float,float,float>)
		.addFunction("clamp", &std::clamp<float>)
		.endNamespace();
	/*-------------------------*/

	// Carregar e executar o script
	if (luaL_dofile(L, arquivo.c_str()) != LUA_OK)
	{
		debug::emitir(Erro, std::string("carregar script : ") + lua_tostring(L, -1));
	}
}

void bubble::codigo::iniciar() const
{
	luabridge::setGlobal(L, new bapi::entidade(meu_objeto), "eu");
	// Tentar obter a função "iniciar" definida localmente no script
	lua_getglobal(L, "iniciar");

	// Verificar se a função foi encontrada e é válida
	if (lua_isfunction(L, -1))
	{
		// Chamar a função Lua "iniciar" - é uma função local dentro do escopo do script
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			debug::emitir(Erro, std::string("funcao 'iniciar': ") + lua_tostring(L, -1));
			lua_pop(L, 1);  // Remover a mensagem de erro da pilha
		}
	}
	else
	{
		debug::emitir(Erro, "Funcao 'iniciar' nao encontrada ou nao eh uma funcao local.");
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
			debug::emitir(Erro, std::string("atualizar script : ") + lua_tostring(L, -1));
		}
	}
	else
	{
		lua_pop(L, 1); // Remover da pilha se não for função
	}
}