/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include <GLFW/glfw3.h>
#include "src/util/vetor4.hpp"
#include "src/nucleo/sistema.hpp"
#include "src/nucleo/sistema_de_renderizacao.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

namespace bubble
{
	/**
	* @struct janela
	* @brief facilita o uso da janela glfw
	* @brief cuida dos callbacks, parametros iniciais etc.
	*/
	struct janela
	{
		vetor4<float> tamanho;
		GLFWwindow* window;
		janela(const char* nome);
		void poll() const;
		void swap() const;
		void viewport() const;
	};
}