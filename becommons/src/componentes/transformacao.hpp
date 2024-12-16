/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/vetor3.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

/**
 * @file transformacao.hpp
 * @brief Calcula a mtriz de kodelo para um objeto 3d qualquer
 *
*/

namespace bubble
{
	struct transformacao : componente
	{
		glm::vec3 posicao{}, rotacao{}, escala{};
		glm::mat4 matrizmodelo;
		static constexpr mascara mascara = COMPONENTE_TRANSFORMACAO;

		transformacao(const vetor3<float>& p = vetor3(0.f,0.f,0.f),
			const vetor3<float>& r = vetor3(0.f, 0.f, 0.f), 
			const vetor3<float>& e = vetor3(1.f, 1.f, 1.f)) :
			posicao({ p.x,p.y,p.z }),
			rotacao({ r.x,r.y,r.z }),
			escala({ e.x,e.y,e.z })
		{
		}
		glm::mat4 calcular()
		{
			matrizmodelo = glm::translate(glm::mat4(1.f), posicao);

			matrizmodelo *= glm::toMat4(glm::quat(glm::radians(rotacao)));

			matrizmodelo = glm::scale(matrizmodelo, escala);
			return matrizmodelo;
		}
		float* obter() { return glm::value_ptr(matrizmodelo); }
	};
}