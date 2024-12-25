/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <src/componentes/camera.hpp>
#include <cstdint>
#include <src/componentes/transformacao.hpp>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace bapi
{
	struct entidade
	{
		bubble::transformacao* _Mtransformacao{ nullptr };
		bubble::camera* _Mcamera{ nullptr };
		uint32_t id;
		static void definir(lua_State* L) 
		{
			luabridge::getGlobalNamespace(L).
				beginClass<glm::vec3>("vetor3").		///< define vetor3
				addConstructor<void(*)(float, float, float)>().
				addData<float>("x", &glm::vec3::x).
				addData<float>("y", &glm::vec3::y).
				addData<float>("z", &glm::vec3::z).
				endClass().
				beginClass<bubble::cor>("cor").		///< define vetor3
				addConstructor<void(*)(float, float, float, float)>().
				addData<float>("r", &bubble::cor::r).
				addData<float>("g", &bubble::cor::g).
				addData<float>("b", &bubble::cor::b).
				addData<float>("a", &bubble::cor::a).
				endClass().
				beginClass<bubble::transformacao>("transformacao").			///< define transformacao
				addConstructor<void(*)()>().
				addData<glm::vec3>("posicao", &bubble::transformacao::posicao, true).
				endClass().
				beginClass<bubble::camera>("camera").			///< define camera
				addConstructor<void(*)()>().
				addFunction<void, const uint32_t&>("olhar", &bubble::camera::olhar).
				addFunction<void, const glm::vec3&>("olharPara", &bubble::camera::olharPara).
				addData("posicao", &bubble::camera::posicao).
				addData("fov", &bubble::camera::fov).
				addData("yaw", &bubble::camera::yaw).
				addData("pitch", &bubble::camera::pitch).
				addData("ceu", &bubble::camera::ceu).
				endClass().
				beginClass<bapi::entidade>("entidade").			///< define entidade
				addConstructor<void(*)(int)>().
				addData("transformacao", &bapi::entidade::_Mtransformacao, true).
				addData("camera", &bapi::entidade::_Mcamera, true).
				addData("id", &bapi::entidade::id, false).
				endClass();
		};
		void mover(float x, float y, float z) const
		{
			_Mtransformacao->posicao.x += x;
			_Mtransformacao->posicao.y += y;
			_Mtransformacao->posicao.z += z;
		}
		entidade(const uint32_t& id);
	};
}