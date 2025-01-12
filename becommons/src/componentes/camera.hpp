/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/raio.hpp"
#include "src/util/vetor2.hpp"
#include "src/util/vetor3.hpp"
#include "src/util/vetor4.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <src/util/cor.hpp>
#include "transformacao.hpp"
#include <memory>

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

		cor ceu				{0.43F, 0.78F, 0.86F, 1.0F};

		glm::vec3 forward, posicao;
		std::shared_ptr<bubble::transformacao> transform{ nullptr };

		glm::mat4 viewMatrix, projMatriz;

		float fov			{ 75.f };
		float aspecto		{ 0.f };
		float corte_curto	{ 0.1f }; 
		float corte_longo	{ 300.f };
		float escala		{ 5.f };

		float left, top, bottom, right;

		bool flag_orth		{ false };

		static constexpr mascara mascara = COMPONENTE_CAM;

		vetor4<float>* viewport_ptr{ nullptr };

		camera() = default;
		camera(const bool ortho);
		glm::mat4 obtViewMatrix();

		glm::mat4 obtProjectionMatrix();

		raio pontoParaRaio(vet2 screenPoint) const;

		glm::vec3 telaParaMundo(const vet2& screenPoint, float profundidade) const;
	};
}