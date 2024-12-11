/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include <GLFW/glfw3.h>
#include "src/util/vetor2.hpp"
#include "src/nucleo/sistema.hpp"
#include "src/nucleo/sistema_de_renderizacao.hpp"
#include <memory>
#include <unordered_map>

struct Janela
{
	std::unordered_map<std::type_index, std::shared_ptr<Sistema>> sistemas;
	GLFWwindow* window;
	Janela(const char* nome);
	template<typename T>
	void adicionarSistema(GerenciadorDeEntidades* ge);
	void iniciarLoop() const;
};

template<typename T>
inline void Janela::adicionarSistema(GerenciadorDeEntidades* ge)
{
	auto sys = std::make_shared<T>(ge);
	sys->inicializar();
	sistemas[std::type_index(typeid(T))] = sys;
}

