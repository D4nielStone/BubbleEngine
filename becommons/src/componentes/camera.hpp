/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "src/util/vetor3.hpp"
#include "src/util/vetor4.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <src/util/cor.hpp>

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

		glm::vec3 posicao	{ 0,0,0 };
		glm::vec3* alvo		{ new glm::vec3(0, 0, 0) };
		glm::vec3 cima		{ 0,1,0 };
		glm::vec3 forward;

		glm::mat4 viewMatrix;

		float fov			{ 75.f };
		float aspecto		{ 0.f };
		float corte_curto	{ 0.1f }; 
		float corte_longo	{ 300.f };
		float escala		{ 5.f };
		float yaw			{ 0.f };
		float pitch			{ 0.f };

		bool flag_alvo		{ false };
		bool flag_orth		{ false };

		static constexpr mascara mascara = COMPONENTE_CAM;

		vetor4<float>* viewport_ptr{ nullptr };

		camera() = default;
		camera(const vetor3<float>& pos, const bool alvo = false, const bool ortho = false);
		glm::mat4 obtViewMatrix() {
			if (flag_alvo)
			{
				viewMatrix = glm::lookAt(posicao, *alvo, cima);
			}
			else
			{
				// Cria a matriz de rotação com base no yaw e pitch
				glm::mat4 rotacaoYaw = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 rotacaoPitch = glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f));

				// Combina as rotações
				glm::mat4 rotacao = rotacaoPitch * rotacaoYaw;

				// Translada a câmera para a posição inversa
				glm::mat4 translacao = glm::translate(glm::mat4(1.0f), -posicao);

				// A matriz de visualização é a combinação das duas
				viewMatrix =  rotacao * translacao;
			}
			forward.x = -viewMatrix[0][2]; // Elemento na 3ª coluna, 1ª linha
			forward.y = -viewMatrix[1][2]; // Elemento na 3ª coluna, 2ª linha
			forward.z = -viewMatrix[2][2]; // Elemento na 3ª coluna, 3ª linha

			yaw = atan2(forward.x, forward.z); // Yaw em radianos
			pitch = asin(forward.y);          // Pitch em radianos

			return viewMatrix;
		}

		glm::mat4 obtProjectionMatrix()
		{
			if (flag_orth && viewport_ptr)
			{
				float largura = viewport_ptr->w;
				float altura = viewport_ptr->h;
				aspecto = largura / altura;

				return glm::ortho(-escala * aspecto, escala * aspecto, -escala, escala, corte_curto, corte_longo);

			}
			else if(viewport_ptr)
			{
				float largura = viewport_ptr->w;
				float altura = viewport_ptr->h;
				aspecto = largura / altura;

				return glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
			}
		}
		void olhar(const uint32_t& ent);
		void olharPara(const glm::vec3& pos);
	};
}