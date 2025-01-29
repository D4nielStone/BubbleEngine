/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include "tempo.hpp"
#include <GLFW/glfw3.h>
#include "util/vetor4.hpp"
#include "nucleo/sistema.hpp"
#include "nucleo/sistema_de_renderizacao.hpp"
#include "inputs/inputs.hpp"
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
		tempo _Mtempo;
		vet2 tamanho;
		GLFWwindow* window;
		bubble::inputs inputs;
		janela(const char* nome, const char* icon_path = nullptr);
		~janela();
		void poll() const;
		void swap();
		void viewport() const;
		void nome(const char* novo_nome) const;
	};
	inline janela* instanciaJanela{ nullptr };
}
