#include "codigo.hpp"
#include "../nucleo/fase.hpp"
#include "../api/api_lua.hpp"
#include "../api/mat.hpp"
#include "../inputs/inputs.hpp"
#include "../../os/janela.hpp"
#include <cmath>

template <class T>
T lerp(T start, T end, T alpha) {
    return start + alpha * (end - start);
}

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
		.addFunction("lerp", &lerp<float>)
		.addFunction("lerpV3", &bubble::lerpV3)
		.addFunction("clamp", &std::clamp<float>)
		.addFunction("distanciaV3", &bubble::distancia3)
		.addFunction("distanciaV2", &bubble::distancia2)
		.addFunction("normalizarV3", &glm::normalize<3, float, glm::packed_highp>)
		.endNamespace();
			
	/*-------------------------*/
}

void bubble::codigo::iniciar() const
{
	luabridge::setGlobal(L, new bapi::entidade(meu_objeto), "eu");
	luabridge::setGlobal(L, &fase_atual, "faseAtual");
	// Tentar obter a fun��o "iniciar" definida localmente no script
	lua_getglobal(L, "iniciar");
}

void bubble::codigo::atualizar() const
{
	// Chamar fun��o de atualiza��o no Lua, se existir
	lua_getglobal(L, "atualizar");
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