/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <src/componentes/camera.hpp>
#include <cstdint>
#include <src/componentes/transformacao.hpp>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <src/util/vetor2.hpp>
#include <src/util/raio.hpp>
#include <src/componentes/texto.hpp>
#include <src/componentes/imagem.hpp>
#include <src/componentes/fisica.hpp>
#include "mat.hpp"
#include <src/nucleo/sistema_de_fisica.hpp>

namespace bapi
{
	struct entidade
	{
		bubble::transformacao* _Mtransformacao{ nullptr };
		bubble::fisica* _Mfisica{ nullptr };
		bubble::camera* _Mcamera{ nullptr };
		bubble::texto* _Mtexto{ nullptr };
		bubble::imagem* _Mimagem{ nullptr };
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
				beginClass<bubble::vetor2<int>>("vetor2i").		///< define vetor3
				addConstructor<void(*)(int, int)>().
				addConstructor<void(*)(float, float)>().
				addData<int>("x", &bubble::vetor2<int>::x).
				addData<int>("y", &bubble::vetor2<int>::y).
				endClass().
				beginClass<bubble::vetor2<double>>("vetor2d").		///< define vetor3
				addConstructor<void(*)(double, double)>().
				addData<double>("x", &bubble::vetor2<double>::x).
				addData<double>("y", &bubble::vetor2<double>::y).
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
				addData<glm::vec3>("escala", &bubble::transformacao::escala, true).
				addData<glm::vec3>("rotacao", &bubble::transformacao::rotacao, true).
				endClass().
				beginClass<bubble::imagem>("imagem").			///< define transformacao
				addConstructor<void(*)(std::string)>().
				addData<bubble::vetor2<int>>("padding", &bubble::imagem::padding, true).
				addData<bubble::vetor2<int>>("limite", &bubble::imagem::limite, true).
				endClass().
				beginClass<bubble::camera>("camera").			///< define camera
				addConstructor<void(*)()>().
				addFunction("olhar", &bubble::camera::olhar).
				addFunction("olharPara", &bubble::camera::olharPara).
				addFunction("pontoParaRaio", &bubble::camera::pontoParaRaio).
				addData("posicao", &bubble::camera::posicao).
				addData("fov", &bubble::camera::fov).
				addData("corte_curto", &bubble::camera::corte_curto).
				addData("corte_longo", &bubble::camera::corte_longo).
				addData("frente", &bubble::camera::forward).
				addData("yaw", &bubble::camera::yaw).
				addData("pitch", &bubble::camera::pitch).
				addData("ceu", &bubble::camera::ceu).
				addData("escala", &bubble::camera::escala).
				addData("flag_alvo", &bubble::camera::flag_alvo).
				addData("flag_ortho", &bubble::camera::flag_orth).
				endClass().
				beginClass<bapi::entidade>("entidade").			///< define entidade
				addConstructor<void(*)(int)>().
				addData("transformacao", &bapi::entidade::_Mtransformacao, true).
				addData("camera", &bapi::entidade::_Mcamera, true).
				addData("imagem", &bapi::entidade::_Mimagem, true).
				addData("fisica", &bapi::entidade::_Mfisica, true).
				addData("id", &bapi::entidade::id, false).
				addFunction("destruir", &bapi::entidade::destruir).
				endClass();
		};
		void destruir() const;
		entidade(const uint32_t& id);
	};
	inline static void definirFisica(lua_State* L)
	{
		luabridge::getGlobalNamespace(L).
			beginClass<btCollisionObject>("objetoDeColisao").			///< define transformacao
			addConstructor<void(*)()>().
			endClass().
			beginClass<bubble::raio>("raio").			///< define transformacao
			addConstructor<void(*)()>().
			addData("origem", &bubble::raio::origem).
			addData("direcao", &bubble::raio::direcao).
			endClass().
			beginClass<bubble::resultadoRaio>("resultadoRaio").			///< define transformacao
			addConstructor<void(*)()>().
			addData("objetoAtingido", &bubble::resultadoRaio::objetoAtingido, false).
			addData("atingiu", &bubble::resultadoRaio::atingiu, false).
			addData("pontoDeColisao", &bubble::resultadoRaio::pontoDeColisao, false).
			addData("normalAtingida", &bubble::resultadoRaio::normalAtingida, false).
			endClass().
			beginClass<bubble::fisica>("fisica").			///< define transformacao
			addConstructor<void(*)()>().
			addFunction("aplicarForca", &bubble::fisica::aplicarForca).
			addFunction("defVelocidade", &bubble::fisica::aplicarVelocidade).
			addFunction("obtVelocidade", &bubble::fisica::obterVelocidade).
			addFunction("corpoRigido", &bubble::fisica::obterCorpoRigido).
			endClass();
	}
}