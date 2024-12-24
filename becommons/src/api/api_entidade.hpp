/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <src/componentes/camera.hpp>
#include <cstdint>
#include <src/componentes/transformacao.hpp>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace bapi
{
	class entidade
	{
	public:
		bubble::transformacao* _Mtransformacao{ nullptr };
		bubble::camera* _Mcamera{ nullptr };
		static void definir(lua_State* L) 
		{
			luabridge::getGlobalNamespace(L).
				beginClass<glm::vec3>("vetor3").		///< define vetor3
				addConstructor<void(*)(float, float, float)>().
				addData<float>("x", &glm::vec3::x).
				addData<float>("y", &glm::vec3::y).
				addData<float>("z", &glm::vec3::z).
				endClass().
				beginClass<bubble::transformacao>("transformacao").			///< define transformacao
				addConstructor<void(*)()>().
				addData<glm::vec3>("posicao", &bubble::transformacao::posicao, true).
				endClass().
				beginClass<bubble::camera>("camera").			///< define camera
				addConstructor<void(*)()>().
				addFunction("olhar", &bubble::camera::olhar).
				endClass().
				beginClass<bapi::entidade>("entidade").			///< define entidade
				addConstructor<void(*)(int)>().
				addData("transformacao", &bapi::entidade::_Mtransformacao, true).
				addData("camera", &bapi::entidade::_Mcamera, true).
				endClass();
		};
		void mover(float x, float y, float z) const
		{
			_Mtransformacao->posicao.x += x;
			_Mtransformacao->posicao.y += y;
			_Mtransformacao->posicao.z += z;
		}
		entidade(const uint32_t& id);
	private:
		const uint32_t id;
	};
}