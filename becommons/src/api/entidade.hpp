/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <cstdint>
#include <src/componentes/transformacao.hpp>
namespace bapi
{
	class entidade
	{
	public:
		entidade(uint32_t id) : id(id) {}
		int mostrar_transformacao(lua_State* L) const

		{
			std::shared_ptr<bubble::transformacao> transform = cena_atual->obterRegistro()->obter<bubble::transformacao>(id);

			// Retornar os valores da transformação (exemplo: posição)
			lua_newtable(L);
			lua_pushnumber(L, transform->posicao.x);
			lua_setfield(L, -2, "x");
			lua_pushnumber(L, transform->posicao.y);
			lua_setfield(L, -2, "y");
			lua_pushnumber(L, transform->posicao.z);
			lua_setfield(L, -2, "z");

			return 1; // Retorna a tabela para o Lua
		}
		void mover(float x, float y, float z) const

		{
			std::shared_ptr<bubble::transformacao> transform = cena_atual->obterRegistro()->obter<bubble::transformacao>(id);

			transform->posicao.x += x;
			transform->posicao.y += y;
			transform->posicao.z += z;
		}
	private:
		uint32_t id;
	};
}