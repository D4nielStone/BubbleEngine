/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/vetor3.hpp"
#include <glm/ext/vector_float3.hpp>

namespace bubble
{
	struct camera : componente
	{
		glm::vec3 posicao; ///< Posicao da camera ao mundo
		glm::vec3 alvo;	///< Alvo que a camera deve olhar
		glm::vec3 cima;	///< Vetor "up"
		float fov;
		float aspecto;
		float corte_curto;
		float corte_longo;
		static constexpr mascara mascara = COMPONENTE_CAM;

		camera(const vetor3<float>& pos, const vetor3<float>& target = { 0.f, 0.f, 0.f }, float fov = 45.f, float aspect = 1.f, float near = 0.1f, float far = 100.f)
			: posicao({ pos.x,pos.y,pos.z }), alvo({ target.x,target.y,target.z }),
			cima(0.0f, 1.0f, 0.0f), fov(fov), aspecto(aspect), corte_curto(near), corte_longo(far)
		{
		}
		glm::mat4 obtViewMatrix() const {
			return glm::lookAt(posicao, alvo, cima);
		}

		glm::mat4 obtProjectionMatrix() const {
			return glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
		}
	};
}