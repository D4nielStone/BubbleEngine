/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/vetor3.hpp"
#include "src/util/vetor4.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <src/util/cor.hpp>

namespace bapi { class entidade; }
namespace bubble
{
	/**
	 * @struct camera
	 * @brief componente que facilita a manipuacao da matriz de visualizacao e projeco
	 */
	struct camera : componente
	{
		/**
		 * @enum configCamera
		 * @brief configuracao da camera;
		 */
		enum configCamera : uint8_t { ORTHO, PERSPECTIVA, A, LIVRE };
		configCamera projecao;

		cor ceu{0.43f, 0.78f, 0.86, 1.f};

		glm::vec3 posicao, *alvo, cima;

		float fov, aspecto, corte_curto, corte_longo, escala{5.f};

		static constexpr mascara mascara = COMPONENTE_CAM;

		vetor4<float>* viewport_ptr{ nullptr };

		camera() = default;
		camera(const configCamera& projecao, const vetor3<float>& pos, const vetor3<float>& target = { 0.f, 0.f, 0.f }, float fov = 45.f, float aspect = 1.f, float near = 0.1f, float far = 100.f)

			: posicao({ pos.x,pos.y,pos.z }), alvo(new glm::vec3{ target.x,target.y,target.z }),
			cima(0.0f, 1.0f, 0.0f), fov(fov), aspecto(aspect), corte_curto(near), corte_longo(far),
			projecao(projecao)
		{
		}
		glm::mat4 obtViewMatrix() const {
			return glm::lookAt(posicao, *alvo, cima);
		}

		glm::mat4 obtProjectionMatrix()
		{
			if (projecao == configCamera::ORTHO)
			{
				if (viewport_ptr)
				{
					int largura = viewport_ptr->w;
					int altura = viewport_ptr->h;
					aspecto = static_cast<float>(largura) / altura;

					return glm::ortho(-escala * aspecto, escala *aspecto, -escala, escala, corte_curto, corte_longo);
				}
				return glm::mat4(1);
			}
			else
			{
				if (viewport_ptr)
				{
					int largura = viewport_ptr->w;
					int altura = viewport_ptr->h;
					aspecto = static_cast<float>(largura) / altura;
				}
				return glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
			}
		}
		void olhar(const uint32_t& ent);
	};
}