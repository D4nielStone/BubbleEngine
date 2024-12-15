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

struct Transformacao : Componente
{
	glm::vec3 posicao{}, rotacao{}, escala{};
	glm::mat4 matrizModelo;
	static constexpr MascaraComponente mascara = COMPONENTE_TRANSFORMACAO;

	Transformacao(Vetor3<float> p,Vetor3<float> r,Vetor3<float> e) : 
		posicao({ p.x,p.y,p.z }), 
		rotacao({ r.x,r.y,r.z }),
		escala({ e.x,e.y,e.z })
	{
	}
	glm::mat4 calcularMatriz() 
	{ 
		matrizModelo = glm::translate(glm::mat4(1.f), posicao);

		matrizModelo *= glm::toMat4(glm::quat(glm::radians(rotacao)));

		glm::scale(matrizModelo, escala);
		return matrizModelo; 
	}
	float* obterMatrizPtr() { return glm::value_ptr(matrizModelo); }
};