#include "codigo.hpp"
#include "src/nucleo/fase.hpp"
#include "src/api/api_lua.hpp"
#include "src/api/mat.hpp"
#include <src/inputs/inputs.hpp>
#include "os/janela.hpp"
#include <cmath>

bubble::codigo::codigo(const std::string& arquivo) : L(luaL_newstate()), arquivo(arquivo)

{
	luaL_openlibs(L);

	bapi::definirFisica(L);
	bapi::entidade::definir(L);

	std::function<double()> obterDeltaTimeFunc = []() -> double {
		if (!fase_atual) {
			return 0.0;
		}
		return instanciaJanela->_Mtempo.obterDeltaTime();
		};
	/*-------------------------*/
	luabridge::getGlobalNamespace(L)
		.beginNamespace("inputs")
		.addFunction("pressionada", &bubble::pressionada)
		.addFunction("mouse", &bubble::obterMouse)
		.addFunction("tamanhoTela", &bubble::tamanhoJanela)
		.endNamespace()
		.beginNamespace("tempo")
		.addFunction<double>("obterDeltaTime", obterDeltaTimeFunc)
		.endNamespace()
		.beginNamespace("util")
		.addFunction("lerp", &std::lerp<float, float, float>)
		.addFunction("lerpV3", &bubble::lerpV3)
		.addFunction("clamp", &std::clamp<float>)
		.addFunction("distanciaV3", &bubble::distancia3)
		.addFunction("distanciaV2", &bubble::distancia2)
		.addFunction("normalizarV3", &glm::normalize<3, float, glm::packed_highp>)
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
	luabridge::setGlobal(L, &fase_atual, "faseAtual");
	// Tentar obter a função "iniciar" definida localmente no script
	lua_getglobal(L, "iniciar");

	// Verificar se a função foi encontrada e é válida
	if (lua_isfunction(L, -1))
	{
		// Chamar a função Lua "iniciar" - é uma função local dentro do escopo do script
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			lua_pop(L, 1);  // Remover a mensagem de erro da pilha
		}
	}
	else
	{
		lua_pop(L, 1); // Remover da pilha se não for uma função válida
	}
}

void bubble::codigo::atualizar() const
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
void bubble::codigo::encerrar()
{
	if (L) {
		lua_pushnil(L);
		lua_setglobal(L, "eu");
		lua_setglobal(L, "faseAtual");
		lua_close(L);
		L = nullptr;
	}
}
bubble::codigo::~codigo()
{
	encerrar();
}