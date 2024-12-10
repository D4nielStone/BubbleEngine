/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include <GLFW/glfw3.h>
#include "src/util/vetor2.hpp"

struct Janela
{
	GLFWwindow* window;
	Janela(const char* nome);
	void atualizar() const;
	void desenhar() const;
};

